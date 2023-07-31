#include "global.h"
#include "battle_main.h"
#include "battle_setup.h"
#include "bg.h"
#include "data.h"
#include "daycare.h"
#include "decompress.h"
#include "dexnav.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "event_scripts.h"
#include "field_effect.h"
#include "field_effect_helpers.h"
#include "field_message_box.h"
#include "field_player_avatar.h"
#include "field_screen_effect.h"
#include "fieldmap.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "item.h"
#include "m4a.h"
#include "map_name_popup.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "menu_helpers.h"
#include "metatile_behavior.h"
#include "new_menu_helpers.h"
#include "overworld.h"
#include "palette.h"
#include "party_menu.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "pokemon_summary_screen.h"
#include "random.h"
#include "region_map.h"
#include "scanline_effect.h"
#include "script.h"
#include "script_pokemon_util.h"
#include "sound.h"
#include "sprite.h"
#include "start_menu.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "wild_encounter.h"
#include "window.h"
#include "constants/daycare.h"
#include "constants/map_types.h"
#include "constants/species.h"
#include "constants/maps.h"
#include "constants/field_effects.h"
#include "constants/items.h"
#include "constants/songs.h"
#include "constants/abilities.h"
#include "constants/region_map_sections.h"
#include "gba/m4a_internal.h"

// gui tags
#define ICON_GFX_TAG            55130
#define ICON_PAL_TAG            56000
#define SELECTION_CURSOR_TAG    0x4005

// search tags
#define LIT_STAR_TILE_TAG       0x4010
#define HELD_ITEM_TAG           0xd750

// Defines
enum
{
    WINDOW_INFO,
    WINDOW_MAP_NAME,
    WINDOW_MESSAGE,
};

enum
{
    DEXNAV_MSG_CHOOSE_POKEMON,
    DEXNAV_MSG_SEARCH_POKEMON,
    DEXNAV_MSG_NO_DATA,
};

struct DexNavSearch
{
    /*0x00*/ u16 species;
    /*0x02*/ u16 moves[MAX_MON_MOVES];
    /*0x0A*/ u16 heldItem;
    /*0x0C*/ u8 abilityNum:1;
	         u8 abilityHidden:1;
			 u8 unused:6;
	/*0x0D*/ u8 potential;
	/*0x0E*/ u8 monLevel;
	/*0x0F*/ u8 proximity;
	/*0x10*/ s16 tileX;
	/*0x12*/ s16 tileY;
	/*0x14*/ u8 environment;
	/*0x15*/ u8 fldEffSpriteId;
    /*0x16*/ u8 fldEffId;
	/*0x17*/ u8 movementCount;
	/*0x18*/ u8 windowId;
    /*0x19*/ u8 iconSpriteId;
    /*0x1A*/ u8 itemSpriteId;
    /*0x1B*/ u8 ownedIconSpriteId;
    /*0x1C*/ u8 starSpriteIds[3];
};

struct DexNavGUI
{
    MainCallback savedCallback;
    u16 landSpecies[LAND_WILD_COUNT];
    u16 waterSpecies[WATER_WILD_COUNT];
    u8 tilemapBuffer[0x800];
    u8 environment;
    u8 potential;
    u8 cursorSpriteId;
    u8 cursorRow;
    u8 cursorCol;
};

// Function Declarations
// GUI
static void Task_DexNavMain(u8 taskId);
static void PrintCurrentSpeciesInfo(void);
static void DexNavUpdateSelection(void);
static void PrintDexNavMessage(u8 msgId);
// SEARCH
static bool8 TryStartHiddenMonFieldEffect(u8 environment, u8 xSize, u8 ySize);
static u16 DexNavGenerateMoveset(u16 species, u8 encounterLevel, u16* moveDst);
static u16 DexNavGenerateHeldItem(u16 species);
static bool8 DexNavGetAbilityNum(u16 species);
static u8 DexNavGeneratePotential(void);
static u8 DexNavTryGenerateMonLevel(u16 species, u8 environment);
static u8 GetEncounterLevelFromMapData(u16 species, u8 environment);
static void CreateDexNavWildMon(u16 species, u8 potential, u8 level, u8 abilityNum, u8 abilityHidden, u16 item, u16* moves);
static u8 GetPlayerDistance(s16 x, s16 y);
static u8 DexNavPickTile(u8 environment, u8 xSize, u8 ySize);
static void DexNavProximityUpdate(void);
static void DexNavDrawIcons(void);
static void DexNavUpdateSearchWindow(u8 proximity);
static void Task_DexNavSearch(u8 taskId);
static void EndDexNavSearch(u8 taskId);
static void EndDexNavSearchSetupScript(const u8 *script, u8 taskId);

EWRAM_DATA static struct DexNavSearch *sDexNavSearchDataPtr = NULL;
EWRAM_DATA static struct DexNavGUI *sDexNavUiDataPtr = NULL;
EWRAM_DATA bool8 gDexnavBattle = FALSE;
EWRAM_DATA static u16 sLastSearchedSpecies = 0;
EWRAM_DATA static u8 sDexNavSearchLevel = 0;
EWRAM_DATA static u8 sCurrentDexNavChain = 0;

static const u32 sDexNavGuiTiles[] = INCBIN_U32("graphics/dexnav/gui_tiles.4bpp.lz");
static const u32 sDexNavGuiTilemap[] = INCBIN_U32("graphics/dexnav/gui_tilemap.bin.lz");
static const u32 sDexNavGuiPal[] = INCBIN_U32("graphics/dexnav/gui.gbapal");
static const u32 sSelectionCursorGfx[] = INCBIN_U32("graphics/dexnav/cursor_tiles.4bpp.lz");
static const u16 sSelectionCursorPal[] = INCBIN_U16("graphics/dexnav/cursor.gbapal");
static const u32 sCapturedAllMonsTiles[] = INCBIN_U32("graphics/dexnav/captured_all.4bpp.lz");
static const u32 sNoDataGfx[] = INCBIN_U32("graphics/dexnav/no_data.4bpp.lz");
static const u32 sPotentialStarGfx[] = INCBIN_U32("graphics/dexnav/star.4bpp.lz");

static const struct CompressedSpriteSheet sNoDataIconSpriteSheet = {sNoDataGfx, 0x200, ICON_GFX_TAG};
static const struct CompressedSpriteSheet sCapturedAllPokemonSpriteSheet = {sCapturedAllMonsTiles, 0x20, OWNED_ICON_TAG};
static const struct CompressedSpriteSheet sPotentialStarSpriteSheet = {sPotentialStarGfx, 0x20, LIT_STAR_TILE_TAG};

static const u8 sText_DexNav_NoInfo[] = _("--------");
static const u8 sText_DexNav_CaptureToSee[] = _("Capture first!");
static const u8 sText_DexNav_PressSelectToRegister[] = _("{SELECT_BUTTON}Register {B_BUTTON}Cancel");
static const u8 sText_DexNav_ChoosePokemon[] = _("Choose a Pokémon.");
static const u8 sText_DexNav_SearchForRegisteredSpecies[] = _("Pokémon searchable via {L_BUTTON}!");
static const u8 sText_DexNav_NoDataOfPokemon[] = _("This Pokémon don't have any data!");
static const u8 sText_MonLevel[] = _("{LV}. {STR_VAR_1}");
static const u8 sText_EggMove[] = _("Move: {STR_VAR_1}");
static const u8 sText_DexNavChain[] = _("{NO} {STR_VAR_1}");
static const u8 sText_ArrowLeft[] = _("{LEFT_ARROW}");
static const u8 sText_ArrowRight[] = _("{RIGHT_ARROW}");
static const u8 sText_ArrowUp[] = _("{UP_ARROW}");
static const u8 sText_ArrowDown[] = _("{DOWN_ARROW}");

static const u8 sFontColor_Black[3] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY};
static const u8 sFontColor_White[3] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY};
static const u8 sSearchFontColor[3] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY};

static const struct WindowTemplate sDexNavGuiWindowTemplates[] =
{
    [WINDOW_INFO] =
    {
        .bg = 0,
        .tilemapLeft = 20,
        .tilemapTop = 6,
        .width = 10,
        .height = 12,
        .paletteNum = 15,
        .baseBlock = 1,
    },
    [WINDOW_MAP_NAME] =
    {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 30,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 121,
    },
    [WINDOW_MESSAGE] =
    {
	.bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 50,
        .width = 30,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 181,
    },
    DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sDexNavMenuBgTemplates[2] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .priority = 0
    }, 
    {
        .bg = 1,
        .charBaseIndex = 3,
        .mapBaseIndex = 30,
        .priority = 1
    }
};

static const struct SpritePalette sOwnedIconSpritePal =
{
	.data = sSelectionCursorPal,
	.tag = OWNED_ICON_TAG,
};

static const struct OamData sNoDataIconOam =
{
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 1,
};

static const struct OamData sHeldItemOam =
{
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(8x8),
    .size = SPRITE_SIZE(8x8),
    .priority = 0,
    .paletteNum = 0,
};

static const struct OamData sCapturedAllOam =
{
    .y = 0,
    .affineMode = 1,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};

static const struct OamData sSelectionCursorOam =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = 0,
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0
};

// gui sprite templates
static const struct SpriteTemplate sNoDataIconTemplate =
{
    .tileTag = ICON_GFX_TAG,
    .paletteTag = ICON_PAL_TAG,
    .oam = &sNoDataIconOam,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sCaptureAllMonsSpriteTemplate =
{
    .tileTag = OWNED_ICON_TAG,
    .paletteTag = OWNED_ICON_TAG,
    .oam = &sCapturedAllOam,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sSelectionCursorSpriteTemplate =
{
    .tileTag = SELECTION_CURSOR_TAG,
    .paletteTag = OWNED_ICON_TAG,
    .oam = &sSelectionCursorOam,
    .anims =  gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

// search window sprite templates
static const struct SpriteTemplate sHeldItemTemplate =
{
    .tileTag = HELD_ITEM_TAG,
    .paletteTag = HELD_ITEM_TAG,
    .oam = &sHeldItemOam,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sPotentialStarTemplate =
{
    .tileTag = LIT_STAR_TILE_TAG,
    .paletteTag = OWNED_ICON_TAG,
    .oam = &sHeldItemOam,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

// functions
///////////////////////
//// DEXNAV SEARCH ////
///////////////////////
static s16 GetSearchWindowY(void)
{
    return (GetWindowAttribute(sDexNavSearchDataPtr->windowId, WINDOW_TILEMAP_TOP) * 8);
}

#define SPECIES_ICON_X 28

static void DrawDexNavSearchMonIcon(u16 species, u8 *dst, bool8 caught)
{
    LoadMonIconPalette(species);
	
    *dst = CreateMonIcon(species, SpriteCB_MonIcon, SPECIES_ICON_X - 6, GetSearchWindowY() + 12, 0);
    gSprites[*dst].oam.priority = 0;

    if (caught)
        sDexNavSearchDataPtr->ownedIconSpriteId = CreateSprite(&sCaptureAllMonsSpriteTemplate, SPECIES_ICON_X + 6, GetSearchWindowY() + 4, 0);
}

static void AddSearchWindow(void)
{
    struct WindowTemplate template;
    u16 y = sDexNavSearchDataPtr->tileY > gSaveBlock1Ptr->pos.y + 7 ? 0 : 16;

    template = SetWindowTemplateFields(0, 0, y, 30, 4, 15, 8);
    sDexNavSearchDataPtr->windowId = AddWindow(&template);
    PutWindowTilemap(sDexNavSearchDataPtr->windowId);
    LoadDexNavWindowGfx(sDexNavSearchDataPtr->windowId, 0x200, 0xF0);
    CopyWindowToVram(sDexNavSearchDataPtr->windowId, COPYWIN_MAP);
}

#define WINDOW_COL_0          2
#define WINDOW_COL_1          14
#define WINDOW_X              (SPECIES_ICON_X + 12)
#define WINDOW_MOVE_NAME_X    (WINDOW_X + (GetFontAttribute(sDexNavSearchDataPtr->windowId, FONTATTR_MAX_LETTER_WIDTH) * POKEMON_NAME_LENGTH) - 24)
#define WINDOW_ABILITY_NAME_X (WINDOW_MOVE_NAME_X)
#define SEARCH_ARROW_X        (WINDOW_ABILITY_NAME_X + 124)

static void AddSearchWindowText(u16 species, u8 proximity)
{
    u8 windowId = sDexNavSearchDataPtr->windowId;
    u16 ability;
	    
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
	
    StringCopy(gStringVar1, gSpeciesNames[species]);
    AddTextPrinterParameterized3(windowId, 0, WINDOW_X, WINDOW_COL_0, sSearchFontColor, 0, gStringVar1);

    // level
    ConvertIntToDecimalStringN(gStringVar1, sDexNavSearchDataPtr->monLevel, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar4, sText_MonLevel);
    AddTextPrinterParameterized3(windowId, 0, WINDOW_X, WINDOW_COL_1, sSearchFontColor, 0, gStringVar4);

    if (proximity <= SNEAKING_PROXIMITY)
    {
        PlaySE(SE_POKENAV_ON);
        // move name
        if (sDexNavSearchLevel > 1 && sDexNavSearchDataPtr->moves[0])
        {
            StringCopy(gStringVar1, gMoveNames[sDexNavSearchDataPtr->moves[0]]);
            StringExpandPlaceholders(gStringVar4, sText_EggMove);
            AddTextPrinterParameterized3(windowId, 0, WINDOW_MOVE_NAME_X, WINDOW_COL_0, sSearchFontColor, 0, gStringVar4);
        }
        // ability name
        if (sDexNavSearchLevel > 2)
        {
	    ability = GetAbilityBySpecies(species, sDexNavSearchDataPtr->abilityNum, sDexNavSearchDataPtr->abilityHidden);
	
            StringCopy(gStringVar1, gAbilityNames[ability]);
            AddTextPrinterParameterized3(windowId, 0, WINDOW_ABILITY_NAME_X, WINDOW_COL_1, sSearchFontColor, 0, gStringVar1);
        }
    }

    // chain level
    ConvertIntToDecimalStringN(gStringVar1, sCurrentDexNavChain, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar4, sText_DexNavChain);
    AddTextPrinterParameterized3(windowId, 0, SEARCH_ARROW_X - 18, WINDOW_COL_1, sSearchFontColor, 0, gStringVar4);    

    CopyWindowToVram(windowId, COPYWIN_GFX);
}

static void DrawSearchWindow(u16 species)
{
    AddSearchWindow();
    AddSearchWindowText(species, sDexNavSearchDataPtr->proximity);
}

static void RemoveDexNavWindowAndGfx(void)
{
    u8 i;

    // try remove sprites
    if (sDexNavSearchDataPtr->iconSpriteId != MAX_SPRITES)
        DestroySprite(&gSprites[sDexNavSearchDataPtr->iconSpriteId]);
    if (sDexNavSearchDataPtr->itemSpriteId != MAX_SPRITES)
        DestroySprite(&gSprites[sDexNavSearchDataPtr->itemSpriteId]);
    if (sDexNavSearchDataPtr->ownedIconSpriteId != MAX_SPRITES)
        DestroySprite(&gSprites[sDexNavSearchDataPtr->ownedIconSpriteId]);
    
    for (i = 0; i < 3; i++)
    {
        if (sDexNavSearchDataPtr->starSpriteIds[i] != MAX_SPRITES)
            DestroySprite(&gSprites[sDexNavSearchDataPtr->starSpriteIds[i]]);
    }

    FreeSpriteTilesByTag(HELD_ITEM_TAG);
    FreeSpriteTilesByTag(OWNED_ICON_TAG);
    FreeSpriteTilesByTag(LIT_STAR_TILE_TAG);
    FreeSpritePaletteByTag(HELD_ITEM_TAG);
    FreeSpritePaletteByTag(OWNED_ICON_TAG);
    SafeFreeMonIconPalette(sDexNavSearchDataPtr->species);

    ClearStdWindowAndFrameToTransparent(sDexNavSearchDataPtr->windowId, TRUE);
    RemoveWindow(sDexNavSearchDataPtr->windowId);
}

static u8 GetPlayerDistance(s16 x, s16 y)
{
    u16 deltaX = abs(x - (gSaveBlock1Ptr->pos.x + 7));
    u16 deltaY = abs(y - (gSaveBlock1Ptr->pos.y + 7));
	
    return deltaX + deltaY;
}

static void DexNavProximityUpdate(void)
{
    sDexNavSearchDataPtr->proximity = GetPlayerDistance(sDexNavSearchDataPtr->tileX, sDexNavSearchDataPtr->tileY);
}

// Pick a specific tile based on environment
static bool8 DexNavPickTile(u8 environment, u8 areaX, u8 areaY)
{
    // area of map to cover starting from camera position {-7, -7}
    s16 topX = gSaveBlock1Ptr->pos.x - SCANSTART_X;
    s16 topY = gSaveBlock1Ptr->pos.y - SCANSTART_Y;
    s16 botX = topX + areaX;
    s16 botY = topY + areaY;
    u8 i, scale = 0, weight = 0, tileBuffer = 2, currMapType = GetCurrentMapType();
    bool8 nextIter;

    // loop through every tile in area and evaluate
    while (topY < botY)
    {
        while (topX < botX)
        {
            // Check for objects
            nextIter = FALSE;
			
            if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE | PLAYER_AVATAR_FLAG_ACRO_BIKE | PLAYER_AVATAR_FLAG_RIDE_ANY))
                tileBuffer = SNEAKING_PROXIMITY + 3;
            else if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_DASH))
                tileBuffer = SNEAKING_PROXIMITY + 1;

            if (GetPlayerDistance(topX, topY) <= tileBuffer)
            {
                // tile too close to player
                topX++;
                continue;
            }

            for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
            {
                if (gObjectEvents[i].currentCoords.x == topX && gObjectEvents[i].currentCoords.y == topY)
                {
                    // cannot be on a tile where an object exists
                    nextIter = TRUE;
                    break;
                }
            }

            if (nextIter)
            {
                topX++;
                continue;
            }

            switch (environment)
            {
            case ENCOUNTER_TYPE_LAND:
                if (MapGridGetMetatileAttributeAt(topX, topY, METATILE_ATTRIBUTE_ENCOUNTER_TYPE) == TILE_ENCOUNTER_LAND)
                {
                    if (currMapType == MAP_TYPE_UNDERGROUND)
                    { // inside (cave)
                        if (IsZCoordMismatchAt(gObjectEvents[gPlayerAvatar.objectEventId].currentElevation, topX, topY))
                            break; //occurs at same z coord

                        scale = 440 - (GetPlayerDistance(topX, topY) / 2)  - (2 * (topX + topY));
                        weight = ((Random() % scale) < 1) && !MapGridIsImpassableAt(topX, topY);
                    }
                    else
                    { // outdoors: grass
                        scale = 100 - (GetPlayerDistance(topX, topY) * 2);
                        weight = (Random() % scale <= 5) && !MapGridIsImpassableAt(topX, topY);
                    }
                }
                break;
            case ENCOUNTER_TYPE_WATER:
                if (MetatileBehavior_IsSurfable(MapGridGetMetatileBehaviorAt(topX, topY)))
                {
                    scale = 320 - (GetPlayerDistance(topX, topY) / 2);
					
                    if (IsZCoordMismatchAt(gObjectEvents[gPlayerAvatar.objectEventId].currentElevation, topX, topY))
                        break;

                    weight = (Random() % scale <= 1) && !MapGridIsImpassableAt(topX, topY);
                }
                break;
            default:
                break;
            }

            if (weight > 0)
            {
                sDexNavSearchDataPtr->tileX = topX;
                sDexNavSearchDataPtr->tileY = topY;
                return TRUE;
            }

            topX++;
        }

        topY++;
        topX = gSaveBlock1Ptr->pos.x - SCANSTART_X;
    }

    return FALSE;
}


static bool8 TryStartHiddenMonFieldEffect(u8 environment, u8 xSize, u8 ySize)
{
    u8 metatileBehaviour, fldEffId = 0, currMapType = GetCurrentMapType();

    if (DexNavPickTile(environment, xSize, ySize))
    {
        metatileBehaviour = MapGridGetMetatileBehaviorAt(sDexNavSearchDataPtr->tileX, sDexNavSearchDataPtr->tileY);

        switch (environment)
        {
	case ENCOUNTER_TYPE_LAND:
	    if (currMapType == MAP_TYPE_UNDERGROUND)
	        fldEffId = FLDEFF_CAVE_DUST;
	    else if (IsMapTypeIndoors(currMapType))
	    {
	        if (MetatileBehavior_IsTallGrass_2(metatileBehaviour)) // Grass in cave
		    fldEffId = FLDEFF_SHAKING_GRASS;
		else if (MetatileBehavior_IsLongGrass(metatileBehaviour)) // Really tall grass
		    fldEffId = FLDEFF_SHAKING_LONG_GRASS;
		else if (MetatileBehavior_IsSand(metatileBehaviour))
		    fldEffId = FLDEFF_SAND_HOLE;
		else
		    fldEffId = FLDEFF_CAVE_DUST;
	    }
	    else // outdoor, underwater
	    {
	        if (MetatileBehavior_IsTallGrass_2(metatileBehaviour)) // Regular grass
		    fldEffId = FLDEFF_SHAKING_GRASS;
		else if (MetatileBehavior_IsLongGrass(metatileBehaviour)) // Really tall grass
		    fldEffId = FLDEFF_SHAKING_LONG_GRASS;
		else if (MetatileBehavior_IsSand(metatileBehaviour)) // Desert Sand
		    fldEffId = FLDEFF_SAND_HOLE;
		else if (MetatileBehavior_IsMountain(metatileBehaviour)) // Rough Terrain
		    fldEffId = FLDEFF_CAVE_DUST;
		else
		    fldEffId = FLDEFF_BERRY_TREE_GROWTH_SPARKLE; // default
	    }
	    break;
	case ENCOUNTER_TYPE_WATER:
	    fldEffId = FLDEFF_WATER_SURFACING;
	    break;
	default:
	    return FALSE;
        }

        if (fldEffId)
        {
            gFieldEffectArguments[0] = sDexNavSearchDataPtr->tileX;
            gFieldEffectArguments[1] = sDexNavSearchDataPtr->tileY;
            gFieldEffectArguments[2] = 0xFF; // subpriority
            gFieldEffectArguments[3] = 2;   // priority
            sDexNavSearchDataPtr->fldEffSpriteId = FieldEffectStart(fldEffId);
			
            if (sDexNavSearchDataPtr->fldEffSpriteId == MAX_SPRITES)
                return FALSE;

            sDexNavSearchDataPtr->fldEffId = fldEffId;
            return TRUE;
        }
    }

    return FALSE;
}

static void DrawDexNavSearchHeldItem(u8* dst)
{
    *dst = CreateSprite(&sHeldItemTemplate, SPECIES_ICON_X + 2, GetSearchWindowY() + 22, 0);
	
    if (*dst != MAX_SPRITES)
        gSprites[*dst].invisible = TRUE;
}

static void LoadSearchIconData(void)
{
    LoadHeldItemIcons();
    LoadSpritePalette(&sOwnedIconSpritePal);
    LoadCompressedSpriteSheetUsingHeap(&sPotentialStarSpriteSheet);
    LoadCompressedSpriteSheetUsingHeap(&sCapturedAllPokemonSpriteSheet);
}

#define tProximity   data[0]
#define tFrameCount  data[1]
#define tSpecies     data[2]
#define tEnvironment data[3]

static void Task_SetUpDexNavSearch(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    u8 i, environment = sDexNavSearchDataPtr->environment;

    // init sprites
    sDexNavSearchDataPtr->iconSpriteId = MAX_SPRITES;
    sDexNavSearchDataPtr->itemSpriteId = MAX_SPRITES;
    sDexNavSearchDataPtr->ownedIconSpriteId = MAX_SPRITES;
	
	for (i = 0; i < 3; i++)
		sDexNavSearchDataPtr->starSpriteIds[i] = MAX_SPRITES;
    
    sDexNavSearchDataPtr->species = DexNavGenerateMoveset(sDexNavSearchDataPtr->species, sDexNavSearchDataPtr->monLevel, &sDexNavSearchDataPtr->moves[0]);
    sDexNavSearchDataPtr->heldItem = DexNavGenerateHeldItem(sDexNavSearchDataPtr->species);
    sDexNavSearchDataPtr->abilityHidden = DexNavGetAbilityNum(sDexNavSearchDataPtr->species);
    sDexNavSearchDataPtr->potential = DexNavGeneratePotential();
    
    DexNavProximityUpdate();
    LoadSearchIconData();
    DexNavDrawIcons();
    DexNavUpdateSearchWindow(sDexNavSearchDataPtr->proximity);

    IncrementGameStat(GAME_STAT_DEXNAV_SCANNED);
	
    gPlayerAvatar.creeping = TRUE;  // initialize as true in case mon appears beside you
    task->tProximity = gSprites[gPlayerAvatar.spriteId].x;
    task->tFrameCount = 0;
    task->func = Task_DexNavSearch;
}

static void Task_InitDexNavSearch(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    u16 species = task->tSpecies;
    u8 environment = task->tEnvironment;
    const u8 *script = NULL;

    sDexNavSearchDataPtr = AllocZeroed(sizeof(struct DexNavSearch));

    // assign non-objects to struct
    sDexNavSearchDataPtr->species = species;
    sDexNavSearchDataPtr->environment = environment;
    sDexNavSearchDataPtr->monLevel = DexNavTryGenerateMonLevel(species, environment);

    if (Overworld_GetFlashLevel() > 0)
	    script = EventScript_TooDark;
    else if (sDexNavSearchDataPtr->monLevel == MON_LEVEL_NONEXISTENT || !TryStartHiddenMonFieldEffect(sDexNavSearchDataPtr->environment, 12, 12))
	    script = EventScript_NotFoundNearby;

    if (script != NULL)
    {
	Free(sDexNavSearchDataPtr);
        FreeMonIconPalettes();
        ScriptContext1_SetupScript(script);
        DestroyTask(taskId);
    }
    else
	{
		if (sLastSearchedSpecies != species)
		{
			sLastSearchedSpecies = species;
			sDexNavSearchLevel = 0;
		}
	task->func = Task_SetUpDexNavSearch;
	}
}

static void DexNavDrawPotentialStars(u8 potential, u8* dst)
{
    u8 i, spriteId;

    for (i = 0; i < 3; i++)
    {
        spriteId = MAX_SPRITES;
		
        if (potential > i)
            spriteId = CreateSprite(&sPotentialStarTemplate, SPECIES_ICON_X - 22, GetSearchWindowY() + 5 + (i * 10), 0);

        dst[i] = spriteId;
		
        if (spriteId != MAX_SPRITES)
            gSprites[spriteId].invisible = TRUE;
    }
}

static void DexNavUpdateDirectionArrow(void)
{
    u16 tileX = sDexNavSearchDataPtr->tileX;
    u16 tileY = sDexNavSearchDataPtr->tileY;
    u16 playerX = gSaveBlock1Ptr->pos.x + 7;
    u16 playerY = gSaveBlock1Ptr->pos.y + 7;
    u16 deltaX = abs(tileX - playerX);
    u16 deltaY = abs(tileY - playerY);
    u8 windowId = sDexNavSearchDataPtr->windowId;
    const u8 *str;

    FillWindowPixelRect(windowId, PIXEL_FILL(1), SEARCH_ARROW_X, WINDOW_COL_0, 12, 12);
	
    if (deltaX <= 1 && deltaY <= 1)
        str = gString_Dummy;
    else if (deltaX > deltaY)
    {
        if (playerX > tileX)
            str = sText_ArrowLeft;  // player to right
        else
            str = sText_ArrowRight; // player to left
    }
    else // greater Y diff
    {
        if (playerY > tileY)
            str = sText_ArrowUp;    // player below
        else
            str = sText_ArrowDown;  // player above
    }

    AddTextPrinterParameterized3(windowId, 1, SEARCH_ARROW_X, WINDOW_COL_0, sSearchFontColor, 0, str);
    CopyWindowToVram(windowId, COPYWIN_GFX);
}

static void DexNavDrawIcons(void)
{
    u16 species = sDexNavSearchDataPtr->species;

    DrawSearchWindow(species);
    DrawDexNavSearchMonIcon(species, &sDexNavSearchDataPtr->iconSpriteId, GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT));
    DrawDexNavSearchHeldItem(&sDexNavSearchDataPtr->itemSpriteId);
    DexNavDrawPotentialStars(sDexNavSearchDataPtr->potential, &sDexNavSearchDataPtr->starSpriteIds[0]);
    DexNavUpdateDirectionArrow();
}

/////////////////////
//// SEARCH TASK ////
/////////////////////
bool8 TryStartDexnavSearch(void)
{
    u8 taskId;
    u16 val = VarGet(VAR_DEXNAV_SPECIES);

    if (IsDexNavSearchActive() || !val)
        return FALSE;

    DismissMapNamePopup();
    ChangeBgY_ScreenOff(0, 0, 0);
	
    taskId = CreateTask(Task_InitDexNavSearch, 0);
    gTasks[taskId].tSpecies = val;
    gTasks[taskId].tEnvironment = VarGet(VAR_DEXNAV_ENVIRONMENT);
	
    PlaySE(SE_DEX_SEARCH);
	
    return FALSE; // we dont actually want to enable the script context
}

static void EndDexNavSearch(u8 taskId)
{
    DestroyTask(taskId);
    RemoveDexNavWindowAndGfx();
    FieldEffectStop(&gSprites[sDexNavSearchDataPtr->fldEffSpriteId], sDexNavSearchDataPtr->fldEffId);
    Free(sDexNavSearchDataPtr);
}

static void EndDexNavSearchSetupScript(const u8 *script, u8 taskId)
{
    sCurrentDexNavChain = 0; // reset chain
    EndDexNavSearch(taskId);
    ScriptContext1_SetupScript(script);
}

static u8 GetMovementProximityBySearchLevel(void)
{
    if (sDexNavSearchLevel < 20)
        return 2;
    else if (sDexNavSearchLevel < 50)
        return 3;
    else
        return 4;
}

static void Task_DexNavSearch(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    s16 x, y;
    u16 species;

    // out of range
    if (sDexNavSearchDataPtr->proximity > MAX_PROXIMITY)
    {
        EndDexNavSearchSetupScript(EventScript_LostSignal, taskId);
        return;
    }
    // should be creeping but player walks normally
    if (sDexNavSearchDataPtr->proximity <= CREEPING_PROXIMITY && !gPlayerAvatar.creeping && tFrameCount > 60)
    {
        EndDexNavSearchSetupScript(EventScript_MovedTooFast, taskId);
        return;
    }
    // running/biking too close
    if (sDexNavSearchDataPtr->proximity <= SNEAKING_PROXIMITY && TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_DASH | PLAYER_AVATAR_FLAG_MACH_BIKE | PLAYER_AVATAR_FLAG_ACRO_BIKE | PLAYER_AVATAR_FLAG_RIDE_ANY)) 
    {
        EndDexNavSearchSetupScript(EventScript_MovedTooFast, taskId);
        return;
    }
    // check if script just executed
    if (ScriptContext2_IsEnabled())
    {
        EndDexNavSearch(taskId);
        return;
    }
    // player took too long
    if (tFrameCount > DEXNAV_TIMEOUT * 60)
    {
        EndDexNavSearchSetupScript(EventScript_PokemonGotAway, taskId);
        return;
    }
    // player press B
    if (JOY_NEW(B_BUTTON))
    {
	EndDexNavSearch(taskId);
        return;
    }

    if (sDexNavSearchDataPtr->proximity < 1)
    {
	CreateDexNavWildMon(sDexNavSearchDataPtr->species, sDexNavSearchDataPtr->potential, sDexNavSearchDataPtr->monLevel, sDexNavSearchDataPtr->abilityNum, sDexNavSearchDataPtr->abilityHidden, sDexNavSearchDataPtr->heldItem, sDexNavSearchDataPtr->moves);
	
	    if (gMapHeader.regionMapSectionId != MAPSEC_BATTLE_FRONTIER && sDexNavSearchLevel < 255)
			sDexNavSearchLevel++;
        gDexnavBattle = TRUE;        
        ScriptContext1_SetupScript(EventScript_StartDexNavBattle);
        Free(sDexNavSearchDataPtr);
        DestroyTask(taskId);
        return;
    }

    // Caves and water the pokemon moves around
    if ((sDexNavSearchDataPtr->environment == ENCOUNTER_TYPE_WATER || GetCurrentMapType() == MAP_TYPE_UNDERGROUND) && sDexNavSearchDataPtr->proximity < GetMovementProximityBySearchLevel() && sDexNavSearchDataPtr->movementCount < 2)
    {
        FieldEffectStop(&gSprites[sDexNavSearchDataPtr->fldEffSpriteId], sDexNavSearchDataPtr->fldEffId);
		
        while (1) {
            if (TryStartHiddenMonFieldEffect(sDexNavSearchDataPtr->environment, 10, 10))
                break;
        }

        sDexNavSearchDataPtr->movementCount++;
    }
    DexNavProximityUpdate();
	
    // player has moved
    if (tProximity != sDexNavSearchDataPtr->proximity)
    {
	DexNavUpdateSearchWindow(sDexNavSearchDataPtr->proximity);
        tProximity = sDexNavSearchDataPtr->proximity;
    }
    tFrameCount++;
}

static void DexNavUpdateSearchWindow(u8 proximity)
{
    u8 i;
	
    AddSearchWindowText(sDexNavSearchDataPtr->species, proximity);
    DexNavUpdateDirectionArrow();

    // init hidden sprites
    if (sDexNavSearchDataPtr->itemSpriteId != MAX_SPRITES)
        gSprites[sDexNavSearchDataPtr->itemSpriteId].invisible = TRUE;
	
    for (i = 0; i < 3; i++)
    {
	    if (sDexNavSearchDataPtr->starSpriteIds[i] != MAX_SPRITES)
		    gSprites[sDexNavSearchDataPtr->starSpriteIds[i]].invisible = TRUE;
    }

    if (proximity <= SNEAKING_PROXIMITY)
    {
        if (sDexNavSearchLevel > 2 && sDexNavSearchDataPtr->heldItem)
        {
            // toggle item view
            if (sDexNavSearchDataPtr->itemSpriteId != MAX_SPRITES)
                gSprites[sDexNavSearchDataPtr->itemSpriteId].invisible = FALSE;
        }

        if (sDexNavSearchLevel > 4)
        {
	    for (i = 0; i < 3; i++)
	    {
		if (sDexNavSearchDataPtr->starSpriteIds[i] != MAX_SPRITES)
		    gSprites[sDexNavSearchDataPtr->starSpriteIds[i]].invisible = FALSE;
	    }
        }
    }
}

//////////////////////////////
//// DEXNAV MON GENERATOR ////
//////////////////////////////
static bool8 DexNavTryMakeShinyMon(void)
{
    u32 i, shinyRolls, chainBonus, rndBonus, charmBonus = 0, shinyRate = 0;
    u8 searchLevel = sDexNavSearchLevel;
    u8 chain = sCurrentDexNavChain;

    chainBonus = (chain == 50) ? 5 : (chain == 100) ? 10 : 0;
    rndBonus = (Random() % 100 < 4 ? 4 : 0);
    shinyRolls = 1 + charmBonus + chainBonus + rndBonus;

    if (searchLevel > 200)
    {
        shinyRate += searchLevel - 200;
        searchLevel = 200;
    }
    if (searchLevel > 100)
    {
        shinyRate += (searchLevel * 2) - 200;
        searchLevel = 100;
    }
    if (searchLevel > 0)
        shinyRate += searchLevel * 6;

    shinyRate /= 100;
	
    for (i = 0; i < shinyRolls; i++)
    {
        if (Random() % 10000 < shinyRate)
            return TRUE;
    }
    return FALSE;
}

static void CreateDexNavWildMon(u16 species, u8 potential, u8 level, u8 abilityNum, u8 abilityHidden, u16 item, u16* moves)
{
    struct Pokemon* mon = &gEnemyParty[0];
    u8 i, perfectIv = MAX_PER_STAT_IVS, iv[3] = {0};

    if (DexNavTryMakeShinyMon())
        FlagSet(FLAG_CREATE_SHINY_MON); // just easier this way

    GenerateWildMon(species, level);  // shiny rate bonus handled in CreateBoxMon

    // Pick random, unique IVs to set to 31. The number of perfect IVs that are assigned is equal to the potential
    iv[0] = Random() % NUM_STATS;               // choose 1st perfect stat
	
    do {
        iv[1] = Random() % NUM_STATS;
        iv[2] = Random() % NUM_STATS;
    } while ((iv[1] == iv[0])                   // unique 2nd perfect stat
      || (iv[2] == iv[0] || iv[2] == iv[1]));   // unique 3rd perfect stat
	  
    if (potential > 2 && iv[2] != NUM_STATS)
        SetMonData(mon, MON_DATA_HP_IV + iv[2], &perfectIv);
    if (potential > 1 && iv[1] != NUM_STATS)
        SetMonData(mon, MON_DATA_HP_IV + iv[1], &perfectIv);
    if (potential > 0 && iv[0] != NUM_STATS)
        SetMonData(mon, MON_DATA_HP_IV + iv[0], &perfectIv);

    //Set ability and hidden ability
    SetMonData(mon, MON_DATA_ABILITY_NUM, &abilityNum);
    SetMonData(mon, MON_DATA_ABILITY_HIDDEN, &abilityHidden);

    // Set Held Item
    if (item)
        SetMonData(mon, MON_DATA_HELD_ITEM, &item);

    //Set moves
    for (i = 0; i < MAX_MON_MOVES; i++)
        SetMonMoveSlot(mon, moves[i], i);

    CalculateMonStats(mon);
}

// gets a random level of the species based on map data.
static u8 DexNavTryGenerateMonLevel(u16 species, u8 environment)
{
    u8 levelBase = GetEncounterLevelFromMapData(species, environment), levelBonus = sCurrentDexNavChain / 5;

    if (levelBase == MON_LEVEL_NONEXISTENT)
        return levelBase; // species not found in the area

    if (Random() % 100 < 4)
        levelBonus += 10; // 4% chance of having a +10 level bonus

    if (levelBase + levelBonus > MAX_LEVEL)
        return MAX_LEVEL;
    else
        return levelBase + levelBonus;
}

static u16 DexNavGenerateMoveset(u16 species, u8 encounterLevel, u16* moveDst)
{
    bool8 genMove = FALSE;
    u16 newSpecies, numEggMoves, randVal = Random() % 100, eggMoveBuffer[EGG_MOVES_ARRAY_COUNT];
    u8 i, searchLevel = sDexNavSearchLevel;

    // see if first move slot should be an egg move
    if (searchLevel < 5)
    {
#if (SEARCHLEVEL0_MOVECHANCE != 0)
        if (randVal < SEARCHLEVEL0_MOVECHANCE)
            genMove = TRUE;
#endif
    }
    else if (searchLevel < 10)
    {
#if (SEARCHLEVEL5_MOVECHANCE != 0)
        if (randVal < SEARCHLEVEL5_MOVECHANCE)
            genMove = TRUE;
#endif
    }
    else if (searchLevel < 25)
    {
#if (SEARCHLEVEL10_MOVECHANCE != 0)
        if (randVal < SEARCHLEVEL10_MOVECHANCE)
            genMove = TRUE;
#endif
    }
    else if (searchLevel < 50)
    {
#if (SEARCHLEVEL25_MOVECHANCE != 0)
        if (randVal < SEARCHLEVEL25_MOVECHANCE)
            genMove = TRUE;
#endif
    }
    else if (searchLevel < 100)
    {
#if (SEARCHLEVEL50_MOVECHANCE != 0)
        if (randVal < SEARCHLEVEL50_MOVECHANCE)
            genMove = TRUE;
#endif
    }
    else
    {
#if (SEARCHLEVEL100_MOVECHANCE != 0)
        if (randVal < SEARCHLEVEL100_MOVECHANCE)
            genMove = TRUE;
#endif
    }

    // Generate a wild mon just to get the initial moveset (later overwritten by CreateDexNavWildMon)
    newSpecies = GenerateWildMon(species, encounterLevel);

    // Store generated mon moves into Dex Nav Struct
    for (i = 0; i < MAX_MON_MOVES; i++)
        moveDst[i] = GetMonData(&gEnemyParty[0], MON_DATA_MOVE1 + i, NULL);

    // set first move slot to a random egg move if search level is good enough    
    if (genMove)
    {
        numEggMoves = GetEggMoves(&gEnemyParty[0], eggMoveBuffer);
		
        if (numEggMoves)
            moveDst[0] = eggMoveBuffer[Random() % numEggMoves];
    }
	return newSpecies;
}

static u16 DexNavGenerateHeldItem(u16 species)
{
    u16 randVal = Random() % 100;
    u8 searchLevelInfluence = sDexNavSearchLevel >> 1;
    u16 item1 = gBaseStats[species].item1;
    u16 item2 = gBaseStats[species].item2;

    // if no items can be held, then yeah...no items
    if (!item2 && !item1)
        return ITEM_NONE;

    // if both are the same, 100% to hold
    if (item1 == item2)
        return item1;

    // if only one entry, 50% chance
    if (!item2 && item1)
        return (randVal < 50) ? item1 : ITEM_NONE;

    // if both are distinct item1 = 50% + srclvl/2; item2 = 5% + srchlvl/2
    if (randVal < (50 + searchLevelInfluence + 5 + sDexNavSearchLevel))
        return (randVal > 5 + searchLevelInfluence) ? item1 : item2;
    
    return ITEM_NONE;
}

static bool8 DexNavGetAbilityNum(u16 species)
{
    u8 abilityNum, searchLevel = sDexNavSearchLevel;
    bool8 genAbility = FALSE, abilityHidden = FALSE;
    u16 randVal = Random() % 100;

    if (searchLevel < 5)
    {
#if (SEARCHLEVEL0_ABILITYCHANCE != 0)
        if (randVal < SEARCHLEVEL0_ABILITYCHANCE)
            genAbility = TRUE;
#endif
    }
    else if (searchLevel < 10)
    {
#if (SEARCHLEVEL5_ABILITYCHANCE != 0)
        if (randVal < SEARCHLEVEL5_ABILITYCHANCE)
            genAbility = TRUE;
#endif
    }
    else if (searchLevel < 25)
    {
#if (SEARCHLEVEL10_ABILITYCHANCE != 0)
        if (randVal < SEARCHLEVEL10_ABILITYCHANCE)
            genAbility = TRUE;
#endif
    }
    else if (searchLevel < 50)
    {
#if (SEARCHLEVEL25_ABILITYCHANCE != 0)
        if (randVal < SEARCHLEVEL25_ABILITYCHANCE)
            genAbility = TRUE;
#endif
    }
    else if (searchLevel < 100)
    {
#if (SEARCHLEVEL50_ABILITYCHANCE != 0)
        if (randVal < SEARCHLEVEL50_ABILITYCHANCE)
            genAbility = TRUE;
#endif
    }
    else
    {
#if (SEARCHLEVEL100_ABILITYCHANCE != 0)
        if (randVal < SEARCHLEVEL100_ABILITYCHANCE)
            genAbility = TRUE;
#endif
    }
    // Only give hidden ability if Pokemon has been caught before
    if (genAbility && gBaseStats[species].hiddenAbility && GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
        abilityHidden = TRUE;
		
    //Pick a normal ability of that Pokemon
    if (gBaseStats[species].abilities[1])
	abilityNum = Random() & 1;
    else
        abilityNum = 0;

    sDexNavSearchDataPtr->abilityNum = abilityNum;

    return abilityHidden;
}

static u8 DexNavGeneratePotential(void)
{
    u8 genChance = 0, searchLevel = sDexNavSearchLevel;
    int randVal = Random() % 100;

    if (searchLevel < 5)
    {
        genChance = SEARCHLEVEL0_ONESTAR + SEARCHLEVEL0_TWOSTAR + SEARCHLEVEL0_THREESTAR;
		
        if (randVal < genChance)
        {
            // figure out which star it is
            if (randVal < SEARCHLEVEL0_ONESTAR)
                return 1;
            else if (randVal < (SEARCHLEVEL0_ONESTAR + SEARCHLEVEL0_TWOSTAR))
                return 2;
            else
                return 3;
        }
    }
    else if (searchLevel < 10)
    {
        genChance = SEARCHLEVEL5_ONESTAR + SEARCHLEVEL5_TWOSTAR + SEARCHLEVEL5_THREESTAR;
		
        if (randVal < genChance)
        {
            // figure out which star it is
            if (randVal < SEARCHLEVEL5_ONESTAR)
                return 1;
            else if (randVal < (SEARCHLEVEL5_ONESTAR + SEARCHLEVEL5_TWOSTAR))
                return 2;
            else
                return 3;
        }
    }
    else if (searchLevel < 25)
    {
        genChance = SEARCHLEVEL10_ONESTAR + SEARCHLEVEL10_TWOSTAR + SEARCHLEVEL10_THREESTAR;
		
        if (randVal < genChance)
        {
            // figure out which star it is
            if (randVal < SEARCHLEVEL10_ONESTAR)
                return 1;
            else if (randVal < (SEARCHLEVEL10_ONESTAR + SEARCHLEVEL10_TWOSTAR))
                return 2;
            else
                return 3;
        }
    }
    else if (searchLevel < 50)
    {
        genChance = SEARCHLEVEL25_ONESTAR + SEARCHLEVEL25_TWOSTAR + SEARCHLEVEL25_THREESTAR;
		
        if (randVal < genChance)
        {
            // figure out which star it is
            if (randVal < SEARCHLEVEL25_ONESTAR)
                return 1;
            else if (randVal < (SEARCHLEVEL25_ONESTAR + SEARCHLEVEL25_TWOSTAR))
                return 2;
            else
                return 3;
        }
    }
    else if (searchLevel < 100)
    {
        genChance = SEARCHLEVEL50_ONESTAR + SEARCHLEVEL50_TWOSTAR + SEARCHLEVEL50_THREESTAR;
		
        if (randVal < genChance)
        {
            // figure out which star it is
            if (randVal < SEARCHLEVEL50_ONESTAR)
                return 1;
            else if (randVal < (SEARCHLEVEL50_ONESTAR + SEARCHLEVEL50_TWOSTAR))
                return 2;
            else
                return 3;
        }
    }
    else
    {
        genChance = SEARCHLEVEL100_ONESTAR + SEARCHLEVEL100_TWOSTAR + SEARCHLEVEL100_THREESTAR;
		
        if (randVal < genChance)
        {
            // figure out which star it is
            if (randVal < SEARCHLEVEL100_ONESTAR)
                return 1;
            else if (randVal < (SEARCHLEVEL100_ONESTAR + SEARCHLEVEL100_TWOSTAR))
                return 2;
            else
                return 3;
        }
    }

    return 0;   // No potential
}

static u8 GetEncounterLevelFromMapData(u16 species, u8 environment)
{
    u16 headerId = GetCurrentMapWildMonHeaderId();
    const struct WildPokemonInfo *landMonsInfo = GetWildPokemonInfoByHeaderType(headerId, WILD_HEADER_LAND);
    const struct WildPokemonInfo *waterMonsInfo = GetWildPokemonInfoByHeaderType(headerId, WILD_HEADER_WATER);
    u8 i, min = MAX_LEVEL, max = 0;

    switch (environment)
    {
		case ENCOUNTER_TYPE_LAND:
		    if (landMonsInfo == NULL)
		        return MON_LEVEL_NONEXISTENT;
			
		    for (i = 0; i < LAND_WILD_COUNT; i++)
		    {
			    if (landMonsInfo->wildPokemon[i].species == species)
			    {
				    min = (min < landMonsInfo->wildPokemon[i].minLevel) ? min : landMonsInfo->wildPokemon[i].minLevel;
				    max = (max > landMonsInfo->wildPokemon[i].maxLevel) ? max : landMonsInfo->wildPokemon[i].maxLevel;
			    }
		    }
		    break;
		case ENCOUNTER_TYPE_WATER:
		    if (waterMonsInfo == NULL)
			return MON_LEVEL_NONEXISTENT;
		    
		    for (i = 0; i < WATER_WILD_COUNT; i++)
		    {
			    if (waterMonsInfo->wildPokemon[i].species == species)
			    {
				    min = (min < waterMonsInfo->wildPokemon[i].minLevel) ? min : waterMonsInfo->wildPokemon[i].minLevel;
				    max = (max > waterMonsInfo->wildPokemon[i].maxLevel) ? max : waterMonsInfo->wildPokemon[i].maxLevel;
			    }
		    }
		    break;
		default:
		    return MON_LEVEL_NONEXISTENT;
    }

    if (!max)
        return MON_LEVEL_NONEXISTENT;

    if (min == max)
	return min;
	
    max++;
    return (Random() % (max - min)) + min;
}

///////////
/// GUI ///
///////////
static void DexNav_VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void DexNav_MainCB(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void UpdateCursorPosition(void)
{
    u16 x, y;

    switch (sDexNavUiDataPtr->cursorRow)
    {
		case ROW_WATER:
		    x = ROW_WATER_ICON_X + (24 * sDexNavUiDataPtr->cursorCol);
		    y = ROW_WATER_ICON_Y;
		    sDexNavUiDataPtr->environment = ENCOUNTER_TYPE_WATER;
		    break;
		case ROW_LAND_TOP:
		    x = ROW_LAND_ICON_X + (24 * sDexNavUiDataPtr->cursorCol);
		    y = ROW_LAND_TOP_ICON_Y;
		    sDexNavUiDataPtr->environment = ENCOUNTER_TYPE_LAND;
		    break;
		case ROW_LAND_BOT:
		    x = ROW_LAND_ICON_X + (24 * sDexNavUiDataPtr->cursorCol);
		    y = ROW_LAND_BOT_ICON_Y;
		    sDexNavUiDataPtr->environment = ENCOUNTER_TYPE_LAND;
		    break;
		default:
		    return;
    }
    gSprites[sDexNavUiDataPtr->cursorSpriteId].x = x;
    gSprites[sDexNavUiDataPtr->cursorSpriteId].y = y;

    PrintCurrentSpeciesInfo();
}

static void CreateSelectionCursor(void)
{
    struct CompressedSpriteSheet spriteSheet;

    spriteSheet.data = sSelectionCursorGfx;
    spriteSheet.size = 0x200;
    spriteSheet.tag = SELECTION_CURSOR_TAG;
	
    LoadCompressedSpriteSheet(&spriteSheet);
    LoadSpritePalette(&sOwnedIconSpritePal);

    sDexNavUiDataPtr->cursorSpriteId = CreateSprite(&sSelectionCursorSpriteTemplate, 12, 32, 0);  

    UpdateCursorPosition();
}

static void CreateNoDataIcon(s16 x, s16 y)
{
    CreateSprite(&sNoDataIconTemplate, x, y, 0);
}

// Checks if all Pokemon that can be encountered while walking have been capture
static bool8 CapturedAllLandMons(u16 headerId)
{
    u16 species;
    u8 i, count = 0;
    const struct WildPokemonInfo* landMonsInfo = GetWildPokemonInfoByHeaderType(headerId, WILD_HEADER_LAND);

    if (landMonsInfo != NULL)
    {        
        for (i = 0; i < LAND_WILD_COUNT; ++i)
        {
            species = landMonsInfo->wildPokemon[i].species;
			
            if (species && GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
                count++;
        }

        if (i >= LAND_WILD_COUNT && count > 0) // All land mons caught
            return TRUE;
    }
    else
        return TRUE;    // technically, no mon data means you caught them all

    return FALSE;
}

// Checks if all Pokemon that can be encountered while surfing have been capture
static bool8 CapturedAllWaterMons(u16 headerId)
{
    u16 species;
    u8 i, count = 0;
    const struct WildPokemonInfo* waterMonsInfo = GetWildPokemonInfoByHeaderType(headerId, WILD_HEADER_WATER);

    if (waterMonsInfo != NULL)
    {
        for (i = 0; i < WATER_WILD_COUNT; ++i)
        {
            species = waterMonsInfo->wildPokemon[i].species;
			
            if (species && GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
                count++;
        }

        if (i >= WATER_WILD_COUNT && count > 0) // All water mons caught
            return TRUE;
    }
    else
        return TRUE;    // technically, no mon data means you caught them all

    return FALSE;
}

static void DexNavLoadCapturedAllSymbols(void)
{
    u16 headerId = GetCurrentMapWildMonHeaderId();

    LoadCompressedSpriteSheetUsingHeap(&sCapturedAllPokemonSpriteSheet);

    if (CapturedAllWaterMons(headerId))
        CreateSprite(&sCaptureAllMonsSpriteTemplate, 139, 28, 0);
	
    if (CapturedAllLandMons(headerId))
        CreateSprite(&sCaptureAllMonsSpriteTemplate, 153, 76, 0);
}

static void DexNavGuiFreeResources(void)
{
    Free(sDexNavUiDataPtr);
    FreeAllWindowBuffers();
}

static void CB1_InitDexNavSearch(void)
{
    u8 taskId;

    if (!gPaletteFade.active && !ScriptContext2_IsEnabled() && gMain.callback2 == CB2_Overworld)
    {
        SetMainCallback1(CB1_Overworld);
		
        taskId = CreateTask(Task_InitDexNavSearch, 0);
        gTasks[taskId].tSpecies = gSpecialVar_0x8000;
        gTasks[taskId].tEnvironment = gSpecialVar_0x8001;
    }
}

static void Task_DexNavExitAndSearch(u8 taskId)
{
    DexNavGuiFreeResources();
    DestroyTask(taskId);
    SetMainCallback1(CB1_InitDexNavSearch);
    SetMainCallback2(CB2_ReturnToField);
}

static void Task_DexNavFadeAndExit(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sDexNavUiDataPtr->savedCallback);
        DexNavGuiFreeResources();
        DestroyTask(taskId);
    }
}

static bool8 SpeciesInArray(u16 species, u8 section)
{
    u8 i;
    u16 dexNum = SpeciesToNationalPokedexNum(species);

    switch (section)
    {
    case ENCOUNTER_TYPE_LAND:
        for (i = 0; i < LAND_WILD_COUNT; i++)
        {
            if (SpeciesToNationalPokedexNum(sDexNavUiDataPtr->landSpecies[i]) == dexNum)
                return TRUE;
        }
        break;
    case ENCOUNTER_TYPE_WATER:
        for (i = 0; i < WATER_WILD_COUNT; i++)
        {
            if (SpeciesToNationalPokedexNum(sDexNavUiDataPtr->waterSpecies[i]) == dexNum)
                return TRUE;
        }
        break;
    default:
        break;
    }

    return FALSE;
}

// get unique wild encounters on current map
static void DexNavLoadEncounterData(void)
{
    u8 i, grassIndex = 0, waterIndex = 0;
    u16 species, headerId = GetCurrentMapWildMonHeaderId();
    const struct WildPokemonInfo* landMonsInfo = GetWildPokemonInfoByHeaderType(headerId, WILD_HEADER_LAND);
    const struct WildPokemonInfo* waterMonsInfo = GetWildPokemonInfoByHeaderType(headerId, WILD_HEADER_WATER);

    // nop struct data
    memset(sDexNavUiDataPtr->landSpecies, 0, sizeof(sDexNavUiDataPtr->landSpecies));
    memset(sDexNavUiDataPtr->waterSpecies, 0, sizeof(sDexNavUiDataPtr->waterSpecies));

    // land mons
    if (landMonsInfo != NULL && landMonsInfo->encounterRate)
    {
        for (i = 0; i < LAND_WILD_COUNT; i++)
        {
            species = landMonsInfo->wildPokemon[i].species;
			
            if (species && !SpeciesInArray(species, ENCOUNTER_TYPE_LAND))
                sDexNavUiDataPtr->landSpecies[grassIndex++] = species;
        }
    }
    // water mons
    if (waterMonsInfo != NULL && waterMonsInfo->encounterRate)
    {
        for (i = 0; i < WATER_WILD_COUNT; i++)
        {
            species = waterMonsInfo->wildPokemon[i].species;
			
            if (species && !SpeciesInArray(species, ENCOUNTER_TYPE_WATER))
                sDexNavUiDataPtr->waterSpecies[waterIndex++] = species;
        }
    }
}

static void TryDrawIconInSlot(u16 species, s16 x, s16 y)
{
    if (!species || species > NUM_SPECIES)
        CreateNoDataIcon(x, y);   // 'X' in slot
    else if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_SEEN))
        CreateMonIcon(SPECIES_NONE, SpriteCB_MonIcon, x, y, 0); // question mark
    else
        CreateMonIcon(species, SpriteCB_MonIcon, x, y, 0);
}

static void DrawSpeciesIcons(void)
{
    u8 i;
    s16 x, y;
    u16 species;

    LoadCompressedSpriteSheetUsingHeap(&sNoDataIconSpriteSheet);
	
    for (i = 0; i < LAND_WILD_COUNT; i++)
    {
        species = sDexNavUiDataPtr->landSpecies[i];
		
        x = 20 + (24 * (i % 6));
        y = ROW_LAND_TOP_ICON_Y + (i > 5 ? 28 : 0);
        TryDrawIconInSlot(species, x, y);
    }

    for (i = 0; i < WATER_WILD_COUNT; i++)
    {
        species = sDexNavUiDataPtr->waterSpecies[i];
		
        x = 30 + 24 * i;
        y = ROW_WATER_ICON_Y;
        TryDrawIconInSlot(species, x, y);
    }
}

static u16 DexNavGetSpecies(void)
{
    u16 species;

    switch (sDexNavUiDataPtr->cursorRow)
    {
		case ROW_WATER:
		    species = sDexNavUiDataPtr->waterSpecies[sDexNavUiDataPtr->cursorCol];
		    break;
		case ROW_LAND_TOP:
		    species = sDexNavUiDataPtr->landSpecies[sDexNavUiDataPtr->cursorCol];
		    break;
		case ROW_LAND_BOT:
		    species = sDexNavUiDataPtr->landSpecies[sDexNavUiDataPtr->cursorCol + COL_LAND_COUNT];
		    break;
		default:
		    return SPECIES_NONE;
    }

    if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_SEEN))
        return SPECIES_NONE;

    return species;
}

static void DexNavUpdateSelection(void)
{
	PlaySE(SE_BAG_CURSOR);
	UpdateCursorPosition();
	PrintDexNavMessage(DEXNAV_MSG_CHOOSE_POKEMON);
}

static void PrintCurrentSpeciesInfo(void)
{
    u8 searchLevelBonus = 0;
    u16 species = DexNavGetSpecies(), dexNum = SpeciesToNationalPokedexNum(species);

    if (!GetSetPokedexFlag(dexNum, FLAG_GET_SEEN))
        species = SPECIES_NONE;

    // clear window
    FillWindowPixelBuffer(WINDOW_INFO, PIXEL_FILL(0));

    // species name
    if (!species)
        AddTextPrinterParameterized3(WINDOW_INFO, 0, 0, SPECIES_INFO_Y, sFontColor_Black, 0, sText_DexNav_NoInfo);
    else
        AddTextPrinterParameterized3(WINDOW_INFO, 0, 0, SPECIES_INFO_Y, sFontColor_Black, 0, gSpeciesNames[species]);

    // search level
    if (!species)
        AddTextPrinterParameterized3(WINDOW_INFO, 0, 0, SEARCH_LEVEL_Y, sFontColor_Black, 0, sText_DexNav_NoInfo);
    else
    {
        ConvertIntToDecimalStringN(gStringVar4, sDexNavSearchLevel, STR_CONV_MODE_LEFT_ALIGN, 3);
        AddTextPrinterParameterized3(WINDOW_INFO, 0, 0, SEARCH_LEVEL_Y, sFontColor_Black, 0, gStringVar4);
    }

    // hidden ability
    if (!species)
        AddTextPrinterParameterized3(WINDOW_INFO, 0, 0, HA_INFO_Y, sFontColor_Black, 0, sText_DexNav_NoInfo);
    else if (GetSetPokedexFlag(dexNum, FLAG_GET_CAUGHT))
    {
        if (gBaseStats[species].hiddenAbility)           
            AddTextPrinterParameterized3(WINDOW_INFO, 0, 0, HA_INFO_Y, sFontColor_Black, 0, gAbilityNames[gBaseStats[species].hiddenAbility]);
        else
            AddTextPrinterParameterized3(WINDOW_INFO, 0, 0, HA_INFO_Y, sFontColor_Black, 0, gText_PokeSum_Item_None);
    }
    else
        AddTextPrinterParameterized3(WINDOW_INFO, 0, 0, HA_INFO_Y, sFontColor_Black, 0, sText_DexNav_CaptureToSee);

    // current chain
    ConvertIntToDecimalStringN(gStringVar4, sCurrentDexNavChain, STR_CONV_MODE_LEFT_ALIGN, 3);
    AddTextPrinterParameterized3(WINDOW_INFO, 0, 0, CHAIN_BONUS_Y, sFontColor_Black, 0, gStringVar4);

    CopyWindowToVram(WINDOW_INFO, COPYWIN_BOTH);
    PutWindowTilemap(WINDOW_INFO);
}

static void PrintDexNavInstructions(void)
{
    FillWindowPixelBuffer(WINDOW_MAP_NAME, PIXEL_FILL(0));
    PutWindowTilemap(WINDOW_MAP_NAME);
    GetMapName(gStringVar4, GetCurrentRegionMapSectionId(), 0);
    AddTextPrinterParameterized3(WINDOW_MAP_NAME, 1, 2, 0, sFontColor_White, 0, gStringVar4);
    AddTextPrinterParameterized3(WINDOW_MAP_NAME, 1, 124, 0, sFontColor_White, 0, sText_DexNav_PressSelectToRegister);
    CopyWindowToVram(WINDOW_MAP_NAME, COPYWIN_BOTH);
}

static void PrintDexNavMessage(u8 msgId)
{
	const u8 *str;
	
	FillWindowPixelBuffer(WINDOW_MESSAGE, PIXEL_FILL(0));
	PutWindowTilemap(WINDOW_MESSAGE);
	
	switch (msgId)
	{
		case DEXNAV_MSG_CHOOSE_POKEMON:
		    str = sText_DexNav_ChoosePokemon;
		    break;
		case DEXNAV_MSG_SEARCH_POKEMON:
		    str = sText_DexNav_SearchForRegisteredSpecies;
		    break;
		case DEXNAV_MSG_NO_DATA:
		    str = sText_DexNav_NoDataOfPokemon;
		    break;
	}
	AddTextPrinterParameterized3(WINDOW_MESSAGE, 1, 2, 0, sFontColor_White, 0, str);
	CopyWindowToVram(WINDOW_MESSAGE, COPYWIN_BOTH);
}

static void Task_DexNavWaitFadeIn(u8 taskId)
{
	if (!gPaletteFade.active)
		gTasks[taskId].func = Task_DexNavMain;
}

static void DexNav_RunSetup(void)
{
    u8 taskId;

    switch (gMain.state)
    {
    case 0:
	SetVBlankHBlankCallbacksToNull();
	break;
    case 1:
	ClearScheduledBgCopiesToVram();
        break;
    case 2:
        ScanlineEffect_Stop();
	FreeAllSpritePalettes();
	ResetPaletteFade();
        ResetSpriteData();
        ResetTasks();
        break;
    case 3:
	ResetVramOamAndBgCntRegs();
	ChangeBgX(0, 0, 0);
	ChangeBgY(0, 0, 0);
	ChangeBgX(1, 0, 0);
	ChangeBgY(1, 0, 0);
	ChangeBgX(2, 0, 0);
	ChangeBgY(2, 0, 0);
	ChangeBgX(3, 0, 0);
	ChangeBgY(3, 0, 0);
	ResetBgsAndClearDma3BusyFlags(0);
	InitBgsFromTemplates(0, sDexNavMenuBgTemplates, NELEMS(sDexNavMenuBgTemplates));
	SetBgTilemapBuffer(1, sDexNavUiDataPtr->tilemapBuffer);
	ScheduleBgCopyTilemapToVram(1);
	break;
    case 4:
	SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
	break;
    case 5:
	ShowBg(0);
	ShowBg(1);
	break;
    case 6:
	ResetTempTileDataBuffers();
	DecompressAndCopyTileDataToVram(1, sDexNavGuiTiles, 0, 0, 0);
        break;
    case 7:
        if (FreeTempTileDataBuffersIfPossible())
	    return;
	break;
    case 8:
       LZDecompressWram(sDexNavGuiTilemap, sDexNavUiDataPtr->tilemapBuffer);
	break;
    case 9:
        LoadPalette(sDexNavGuiPal, 0, 32);
	LoadMonIconPalettes();
	break;
    case 10:
        InitWindows(sDexNavGuiWindowTemplates);
	DeactivateAllTextPrinters();
	break;
    case 11:
	ScheduleBgCopyTilemapToVram(0);
	break;
    case 12:
        sDexNavUiDataPtr->cursorRow = ROW_LAND_TOP;
        sDexNavUiDataPtr->cursorCol = 0;
        sDexNavUiDataPtr->environment = ENCOUNTER_TYPE_LAND;
        break;
    case 13:
	PrintDexNavInstructions();
	PrintDexNavMessage(DEXNAV_MSG_CHOOSE_POKEMON);
        DexNavLoadEncounterData();
	break;
    case 14:
        taskId = CreateTask(Task_DexNavWaitFadeIn, 0);
	gTasks[taskId].tSpecies = SPECIES_NONE;
	gTasks[taskId].tEnvironment = sDexNavUiDataPtr->environment;
	break;
    case 15:
	CreateSelectionCursor();
        DrawSpeciesIcons();
        DexNavLoadCapturedAllSymbols();
        break;
    case 16:
	BlendPalettes(0xFFFFFFFF, 16, RGB_BLACK);
	break;
    case 17:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
        break;
    default:
        SetVBlankCallback(DexNav_VBlankCB);
        SetMainCallback2(DexNav_MainCB);
        break;
    }
	gMain.state++;
}

void DexNavGuiInit(MainCallback callback)
{
    sDexNavUiDataPtr = AllocZeroed(sizeof(struct DexNavGUI));
	
    if (sDexNavUiDataPtr == NULL)
    {
        SetMainCallback2(callback);
        return;
    }
    sDexNavUiDataPtr->savedCallback = callback;
    SetMainCallback2(DexNav_RunSetup);
}

static void Task_DexNavMain(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    u16 species;

    if (IsSEPlaying())
        return;

    if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_POKENAV_OFF);
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        task->func = Task_DexNavFadeAndExit;
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if (sDexNavUiDataPtr->cursorRow == ROW_WATER)
            sDexNavUiDataPtr->cursorRow = ROW_LAND_BOT;
        else
        {
            if (sDexNavUiDataPtr->cursorRow == ROW_LAND_TOP && sDexNavUiDataPtr->cursorCol == COL_LAND_MAX)
                sDexNavUiDataPtr->cursorCol = COL_WATER_MAX;

            sDexNavUiDataPtr->cursorRow--;
        }
	DexNavUpdateSelection();
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (sDexNavUiDataPtr->cursorRow == ROW_LAND_BOT)
        {
		sDexNavUiDataPtr->cursorRow = ROW_WATER;
		
		if (sDexNavUiDataPtr->cursorCol == COL_LAND_MAX)
			sDexNavUiDataPtr->cursorCol = COL_WATER_MAX;
        }
        else
            sDexNavUiDataPtr->cursorRow++;
		
	DexNavUpdateSelection();
    }
    else if (JOY_NEW(DPAD_LEFT))
    {
        if (sDexNavUiDataPtr->cursorCol == 0)
        {
            switch (sDexNavUiDataPtr->cursorRow)
            {
		    case ROW_WATER:
			sDexNavUiDataPtr->cursorCol = COL_WATER_MAX;
			break;
		    default:
			sDexNavUiDataPtr->cursorCol = COL_LAND_MAX;
			break;
            }
        }
        else
            sDexNavUiDataPtr->cursorCol--;

        DexNavUpdateSelection();
    }
    else if (JOY_NEW(DPAD_RIGHT))
    {
        switch (sDexNavUiDataPtr->cursorRow)
        {
		case ROW_WATER:
		    if (sDexNavUiDataPtr->cursorCol == COL_WATER_MAX)
			    sDexNavUiDataPtr->cursorCol = 0;
		    else
			    sDexNavUiDataPtr->cursorCol++;
		    break;
		default:
		    if (sDexNavUiDataPtr->cursorCol == COL_LAND_MAX)
			    sDexNavUiDataPtr->cursorCol = 0;
		    else
			    sDexNavUiDataPtr->cursorCol++;
		    break;
        }
        DexNavUpdateSelection();
    }
    else if (JOY_NEW(SELECT_BUTTON))
    {
        species = DexNavGetSpecies();

        if (species)
        {      
            VarSet(VAR_DEXNAV_SPECIES, species);
	    VarSet(VAR_DEXNAV_ENVIRONMENT, sDexNavUiDataPtr->environment);
            PrintDexNavMessage(DEXNAV_MSG_SEARCH_POKEMON);
            PlayCry7(species, 0);
	    return;
        }
	PrintDexNavMessage(DEXNAV_MSG_NO_DATA);
	PlaySE(SE_FAILURE);
    }
    else if (JOY_NEW(A_BUTTON))
    {
        species = DexNavGetSpecies();
		
	if (species)
	{
	    gSpecialVar_0x8000 = species;
            gSpecialVar_0x8001 = sDexNavUiDataPtr->environment;
            PlaySE(SE_DEX_SEARCH);
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
            task->func = Task_DexNavExitAndSearch;
	    return;
	}
	PrintDexNavMessage(DEXNAV_MSG_NO_DATA);
	PlaySE(SE_FAILURE);
    }
}

/////////////////////////
//// GENERAL UTILITY ////
/////////////////////////
bool8 IsDexNavSearchActive(void)
{
	return FuncIsActiveTask(Task_DexNavSearch);
}

void ResetDexNavSearch(void)
{
    sCurrentDexNavChain = 0;    // reset dex nav chaining on new map
	
    if (IsDexNavSearchActive())
        EndDexNavSearch(FindTaskIdByFunc(Task_DexNavSearch));   // moving to new map ends dexnav search
}

void IncrementOrResetDexNavChain(bool8 increment)
{
	if (increment)
	{
		if (sCurrentDexNavChain < DEXNAV_CHAIN_MAX)
			sCurrentDexNavChain++;
	}
	else
		sCurrentDexNavChain = 0;
}

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
#include "field_specials.h"
#include "field_effect.h"
#include "field_effect_helpers.h"
#include "field_message_box.h"
#include "field_player_avatar.h"
#include "field_screen_effect.h"
#include "fieldmap.h"
#include "form_change.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "item.h"
#include "item_menu_icons.h"
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
static void Task_DexNavMain(u32 taskId);
static void PrintCurrentSpeciesInfo(void);
static void DexNavUpdateSelection(void);
static void PrintDexNavMessage(u32 msgId);
// SEARCH
static bool32 TryStartHiddenMonFieldEffect(u32 environment, u32 xSize, u32 ySize);
static u32 DexNavGenerateMoveset(u32 species, u32 encounterLevel, u16* moveDst);
static u32 DexNavGenerateHeldItem(u32 species);
static bool32 DexNavGetAbilityNum(u32 species);
static u32 DexNavGeneratePotential(void);
static u32 DexNavTryGenerateMonLevel(u32 species, u32 environment);
static u32 GetEncounterLevelFromMapData(u32 species, u32 environment);
static void CreateDexNavWildMon(u32 species, u32 potential, u32 level, u32 abilityNum, u32 abilityHidden, u32 item, u16* moves);
static u8 GetPlayerDistance(s16 x, s16 y);
static bool32 DexNavPickTile(u32 environment, u32 xSize, u32 ySize);
static void DexNavProximityUpdate(void);
static void DexNavDrawIcons(void);
static void DexNavUpdateSearchWindow(u32 proximity);
static void Task_DexNavSearch(u32 taskId);
static void EndDexNavSearch(u32 taskId);
static void EndDexNavSearchSetupScript(const u8 *script, u32 taskId);

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
static const u32 sNoDataGfx[] = INCBIN_U32("graphics/dexnav/no_data.4bpp.lz");

static const struct CompressedSpriteSheet sNoDataIconSpriteSheet = {sNoDataGfx, 0x200, ICON_GFX_TAG};

static const u8 sText_DexNav_NoInfo[] = _("--------");

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

static const struct OamData sNoDataIconOam =
{
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 1,
};

static const struct OamData sSelectionCursorOam =
{
    .size = SPRITE_SIZE(32x32),
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

static const struct SpriteTemplate sSelectionCursorSpriteTemplate =
{
    .tileTag = SELECTION_CURSOR_TAG,
    .paletteTag = TAG_8x8_SYMBOLS,
    .oam = &sSelectionCursorOam,
    .anims =  gDummySpriteAnimTable,
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

static void DrawDexNavSearchMonIcon(u32 species, u8 *dst, bool32 caught)
{
    s16 windowY = GetSearchWindowY();
    
    LoadMonIconPalette(species);
    
    *dst = CreateMonIcon(species, SpriteCB_MonIcon, SPECIES_ICON_X - 6, windowY + 12, 0);
    gSprites[*dst].oam.priority = 0;

    if (caught)
        sDexNavSearchDataPtr->ownedIconSpriteId = Create8x8SymbolSprite(SPECIES_ICON_X + 6, windowY + 4, 0, SYMBOL_POKEBALL);
}

static void AddSearchWindow(void)
{
    struct WindowTemplate template = SetWindowTemplateFields(0, 0, sDexNavSearchDataPtr->tileY > gSaveBlock1Ptr->pos.y + 7 ? 0 : 16, 30, 4, 15, 8);
    LoadDexNavWindowGfx(sDexNavSearchDataPtr->windowId, 0x200, 0xF0);
    sDexNavSearchDataPtr->windowId = AddWindow(&template);
    FillWindowPixelBuffer(sDexNavSearchDataPtr->windowId, PIXEL_FILL(1));
    PutWindowTilemap(sDexNavSearchDataPtr->windowId);
}

#define WINDOW_COL_0          2
#define WINDOW_COL_1          14
#define WINDOW_X              (SPECIES_ICON_X + 12)
#define WINDOW_MOVE_NAME_X    (WINDOW_X + (GetFontAttribute(sDexNavSearchDataPtr->windowId, FONTATTR_MAX_LETTER_WIDTH) * POKEMON_NAME_LENGTH) - 24)
#define WINDOW_ABILITY_NAME_X (WINDOW_MOVE_NAME_X)
#define SEARCH_ARROW_X        (WINDOW_ABILITY_NAME_X + 124)

static void AddSearchWindowText(u32 species, u32 proximity)
{
    u32 windowId = sDexNavSearchDataPtr->windowId;
        
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    
    // Species's name
    StringCopy(gStringVar1, gSpeciesInfo[species].name);
    AddTextPrinterParameterized3(windowId, 0, WINDOW_X, WINDOW_COL_0, sSearchFontColor, 0, gStringVar1);

    // Level
    ConvertIntToDecimalStringN(gStringVar1, sDexNavSearchDataPtr->monLevel, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("{LV}. {STR_VAR_1}"));
    AddTextPrinterParameterized3(windowId, 0, WINDOW_X, WINDOW_COL_1, sSearchFontColor, 0, gStringVar4);

    if (proximity <= SNEAKING_PROXIMITY)
    {
        PlaySE(SE_POKENAV_ON);
        // Move name
        if (sDexNavSearchLevel > 1 && sDexNavSearchDataPtr->moves[0])
        {
            StringCopy(gStringVar1, gBattleMoves[sDexNavSearchDataPtr->moves[0]].name);
            StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("Move: {STR_VAR_1}"));
            AddTextPrinterParameterized3(windowId, 0, WINDOW_MOVE_NAME_X, WINDOW_COL_0, sSearchFontColor, 0, gStringVar4);
        }
        // Ability name
        if (sDexNavSearchLevel > 2)
        {
            StringCopy(gStringVar1, gAbilities[GetAbilityBySpecies(species, sDexNavSearchDataPtr->abilityNum, sDexNavSearchDataPtr->abilityHidden)].name);
            AddTextPrinterParameterized3(windowId, 0, WINDOW_ABILITY_NAME_X, WINDOW_COL_1, sSearchFontColor, 0, gStringVar1);
        }
    }
    // Chain level
    ConvertIntToDecimalStringN(gStringVar1, sCurrentDexNavChain, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("{NO} {STR_VAR_1}"));
    AddTextPrinterParameterized3(windowId, 0, SEARCH_ARROW_X - 18, WINDOW_COL_1, sSearchFontColor, 0, gStringVar4);    
    
    CopyWindowToVram(windowId, COPYWIN_MAP);
}

static void RemoveDexNavWindowAndGfx(void)
{
    u32 i;

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
    
    // free graphics and palettes
    FreeSymbolsIconGraphics();
    FreeMonIconPalette(sDexNavSearchDataPtr->species);
    
    // remove window
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
static bool32 DexNavPickTile(u32 environment, u32 areaX, u32 areaY)
{
    // area of map to cover starting from camera position {-7, -7}
    s16 topX = gSaveBlock1Ptr->pos.x - SCANSTART_X;
    s16 topY = gSaveBlock1Ptr->pos.y - SCANSTART_Y;
    s16 botX = topX + areaX;
    s16 botY = topY + areaY;
    u32 i, scale = 0, weight = 0, tileBuffer = 2, currMapType = GetCurrentMapType();
    bool32 nextIter;

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
            case WILD_HEADER_LAND:
                if (MapGridGetMetatileAttributeAt(topX, topY, METATILE_ATTRIBUTE_ENCOUNTER_TYPE) == TILE_ENCOUNTER_LAND)
                {
                    if (currMapType == MAP_TYPE_UNDERGROUND)
                    { // inside (cave)
                        if (IsZCoordMismatchAt(gObjectEvents[gPlayerAvatar.objectEventId].currentElevation, topX, topY))
                            break; //occurs at same z coord

                        scale = 440 - (GetPlayerDistance(topX, topY) / 2)  - (2 * (topX + topY));
                        weight = (RandomMax(scale) < 1) && !MapGridIsImpassableAt(topX, topY);
                    }
                    else
                    { // outdoors: grass
                        scale = 100 - (GetPlayerDistance(topX, topY) * 2);
                        weight = (RandomMax(scale) <= 5) && !MapGridIsImpassableAt(topX, topY);
                    }
                }
                break;
            case WILD_HEADER_WATER:
                if (MetatileBehavior_IsSurfable(MapGridGetMetatileBehaviorAt(topX, topY)))
                {
                    scale = 320 - (GetPlayerDistance(topX, topY) / 2);
                    
                    if (IsZCoordMismatchAt(gObjectEvents[gPlayerAvatar.objectEventId].currentElevation, topX, topY))
                        break;

                    weight = (RandomMax(scale) <= 1) && !MapGridIsImpassableAt(topX, topY);
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


static bool32 TryStartHiddenMonFieldEffect(u32 environment, u32 xSize, u32 ySize)
{
    u32 metatileBehaviour, fldEffId = 0, currMapType = GetCurrentMapType();

    if (DexNavPickTile(environment, xSize, ySize))
    {
        metatileBehaviour = MapGridGetMetatileBehaviorAt(sDexNavSearchDataPtr->tileX, sDexNavSearchDataPtr->tileY);

        switch (environment)
        {
            case WILD_HEADER_LAND:
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
            case WILD_HEADER_WATER:
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
    *dst = Create8x8SymbolSprite(SPECIES_ICON_X + 2, GetSearchWindowY() + 22, 0, SYMBOL_HELDITEM);
    
    if (*dst != MAX_SPRITES)
        gSprites[*dst].invisible = TRUE;
}

#define tSetupStep   data[0]
#define tProximity   data[1]
#define tFrameCount  data[2]
#define tSpecies     data[3]
#define tEnvironment data[4]

static void Task_SetUpDexNavSearch(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    u32 i;
    
    switch (tSetupStep)
    {
        case 0: // init sprites
            sDexNavSearchDataPtr->iconSpriteId = MAX_SPRITES;
            sDexNavSearchDataPtr->itemSpriteId = MAX_SPRITES;
            sDexNavSearchDataPtr->ownedIconSpriteId = MAX_SPRITES;
            
            for (i = 0; i < 3; i++)
                sDexNavSearchDataPtr->starSpriteIds[i] = MAX_SPRITES;
            
            ++tSetupStep;
            break;
        case 1: // Init species data
            sDexNavSearchDataPtr->species = DexNavGenerateMoveset(sDexNavSearchDataPtr->species, sDexNavSearchDataPtr->monLevel, &sDexNavSearchDataPtr->moves[0]);
            sDexNavSearchDataPtr->heldItem = DexNavGenerateHeldItem(sDexNavSearchDataPtr->species);
            sDexNavSearchDataPtr->abilityHidden = DexNavGetAbilityNum(sDexNavSearchDataPtr->species);
            sDexNavSearchDataPtr->potential = DexNavGeneratePotential();
            ++tSetupStep;
            break;
        case 2: // Init search window data
            DexNavProximityUpdate();
            LoadSymbolsIconGraphics();
            AddSearchWindow();
            ++tSetupStep;
            break;
        case 3: // Draw everything
            DexNavDrawIcons();
            DexNavUpdateSearchWindow(sDexNavSearchDataPtr->proximity);
            ++tSetupStep;
            break;
        case 4: // Init search
            IncrementGameStat(GAME_STAT_DEXNAV_SCANNED);
            gPlayerAvatar.creeping = TRUE;  // initialize as true in case mon appears beside you
            gTasks[taskId].tProximity = gSprites[gPlayerAvatar.spriteId].x;
            gTasks[taskId].tFrameCount = 0;
            gTasks[taskId].func = Task_DexNavSearch;
            break;
    }
}

static void Task_InitDexNavSearch(u32 taskId)
{
    u32 species = gTasks[taskId].tSpecies;
    u32 environment = gTasks[taskId].tEnvironment;
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
        FreeMonIconPalette(species);
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
        gTasks[taskId].tSetupStep = 0;
        gTasks[taskId].func = Task_SetUpDexNavSearch;
    }
}

static void DexNavDrawPotentialStars(u32 potential, u8* dst)
{
    s16 windowY = GetSearchWindowY();
    u32 i;

    for (i = 0; i < 3; i++)
    {
        dst[i] = potential > i ? Create8x8SymbolSprite(SPECIES_ICON_X - 22, windowY + 5 + (i * 10), 0, SYMBOL_YELLOWSTAR) : MAX_SPRITES;
        
        if (dst[i] != MAX_SPRITES)
            gSprites[dst[i]].invisible = TRUE;
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
    u32 windowId = sDexNavSearchDataPtr->windowId;
    const u8 *str;

    FillWindowPixelRect(windowId, PIXEL_FILL(1), SEARCH_ARROW_X, WINDOW_COL_0, 12, 12);
    
    if (deltaX <= 1 && deltaY <= 1)
        str = gString_Dummy;
    else if (deltaX > deltaY)
    {
        if (playerX > tileX)
            str = COMPOUND_STRING("{LEFT_ARROW}");  // player to right
        else
            str = COMPOUND_STRING("{RIGHT_ARROW}"); // player to left
    }
    else // greater Y diff
    {
        if (playerY > tileY)
            str = COMPOUND_STRING("{UP_ARROW}");    // player below
        else
            str = COMPOUND_STRING("{DOWN_ARROW}");  // player above
    }

    AddTextPrinterParameterized3(windowId, 1, SEARCH_ARROW_X, WINDOW_COL_0, sSearchFontColor, 0, str);
    CopyWindowToVram(windowId, COPYWIN_GFX);
}

static void DexNavDrawIcons(void)
{
    u32 species = sDexNavSearchDataPtr->species;
    DrawDexNavSearchMonIcon(species, &sDexNavSearchDataPtr->iconSpriteId, GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT));
    DrawDexNavSearchHeldItem(&sDexNavSearchDataPtr->itemSpriteId);
    DexNavDrawPotentialStars(sDexNavSearchDataPtr->potential, &sDexNavSearchDataPtr->starSpriteIds[0]);
}

/////////////////////
//// SEARCH TASK ////
/////////////////////
bool32 TryStartDexnavSearch(void)
{
    u32 taskId, species = VarGet(VAR_DEXNAV_SPECIES);

    if (IsDexNavSearchActive() || !species)
        return FALSE;

    DismissMapNamePopup();
    ChangeBgY_ScreenOff(0, 0, 0);
    
    taskId = CreateTask(Task_InitDexNavSearch, 0);
    gTasks[taskId].tSpecies = species;
    gTasks[taskId].tEnvironment = VarGet(VAR_DEXNAV_ENVIRONMENT);
    
    PlaySE(SE_DEX_SEARCH);
    
    return FALSE; // we dont actually want to enable the script context
}

static void EndDexNavSearch(u32 taskId)
{
    DestroyTask(taskId);
    RemoveDexNavWindowAndGfx();
    FieldEffectStop(&gSprites[sDexNavSearchDataPtr->fldEffSpriteId], sDexNavSearchDataPtr->fldEffId);
    Free(sDexNavSearchDataPtr);
}

static void EndDexNavSearchSetupScript(const u8 *script, u32 taskId)
{
    sCurrentDexNavChain = 0; // reset chain
    EndDexNavSearch(taskId);
    ScriptContext1_SetupScript(script);
}

static u32 GetMovementProximityBySearchLevel(void)
{
    if (sDexNavSearchLevel < 20)
        return 2;
    else if (sDexNavSearchLevel < 50)
        return 3;
    else
        return 4;
}

static void Task_DexNavSearch(u32 taskId)
{
    s16 *data = gTasks[taskId].data;

    // out of range
    if (sDexNavSearchDataPtr->proximity > MAX_PROXIMITY)
    {
        EndDexNavSearchSetupScript(EventScript_LostSignal, taskId);
        return;
    }
#if DEXNAV_CREEPING_SEARCH
    // should be creeping but player walks normally
    if (sDexNavSearchDataPtr->proximity <= CREEPING_PROXIMITY && !gPlayerAvatar.creeping && tFrameCount > 60)
    {
        EndDexNavSearchSetupScript(EventScript_MovedTooFast, taskId);
        return;
    }
#endif
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
    if ((sDexNavSearchDataPtr->environment == WILD_HEADER_WATER || GetCurrentMapType() == MAP_TYPE_UNDERGROUND) && sDexNavSearchDataPtr->proximity < GetMovementProximityBySearchLevel() && sDexNavSearchDataPtr->movementCount < 2)
    {
        FieldEffectStop(&gSprites[sDexNavSearchDataPtr->fldEffSpriteId], sDexNavSearchDataPtr->fldEffId);
        
        while (TRUE) {
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

static void DexNavUpdateSearchWindow(u32 proximity)
{
    u32 i;
    
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
u32 GetDexNavShinyRollsIncrease(u32 rolls)
{
    u32 chain = sCurrentDexNavChain;
    
    if (chain == 50)
        rolls += 5;
    else if (chain == 100)
        rolls += 10;
    
    if (RandomPercentage(RNG_NONE, 4))
        rolls += 4;
    
    return min(rolls, 17);
}

u32 GetDexNavShinyRate(void)
{
    u32 searchLevel = sDexNavSearchLevel;
    u32 shinyRate = 0;
    
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
    
    return shinyRate;
}

static void CreateDexNavWildMon(u32 species, u32 potential, u32 level, u32 abilityNum, u32 abilityHidden, u32 item, u16* moves)
{
    struct Pokemon* mon = &gEnemyParty[0];
    struct PokemonGenerator generator =
    {
        .species = species,
        .level = level,
        .forcedGender = MON_GENDERLESS,
        .otIdType = OT_ID_PLAYER_ID,
        .hasFixedPersonality = FALSE,
        .fixedPersonality = 0,
        .shinyType = GENERATE_SHINY_NORMAL,
        .shinyRollType = SHINY_ROLL_DEXNAV,
        .forcedNature = NUM_NATURES,
        .formChanges = GENERATOR_FORMS(DEFAULT_GENERATOR_FORMS),
        .nPerfectIvs = potential,
    };
    memcpy(generator.moves, moves, sizeof(generator.moves));
    
    ZeroEnemyPartyMons();
    CreateMon(mon, generator); // shiny rate bonus handled in CreateBoxMon

    // Set ability and hidden ability
    SetMonData(mon, MON_DATA_ABILITY_NUM, &abilityNum);
    SetMonData(mon, MON_DATA_ABILITY_HIDDEN, &abilityHidden);

    // Set Held Item
    SetMonData(mon, MON_DATA_HELD_ITEM, &item);
}

// gets a random level of the species based on map data.
static u32 DexNavTryGenerateMonLevel(u32 species, u32 environment)
{
    u32 levelBase = GetEncounterLevelFromMapData(species, environment), levelBonus = sCurrentDexNavChain / 5;

    if (levelBase == MON_LEVEL_NONEXISTENT)
        return levelBase; // species not found in the area

    if (RandomPercentage(RNG_NONE, 4))
        levelBonus += 10; // 4% chance of having a +10 level bonus
    
    levelBase += levelBonus;
    
    return min(MAX_LEVEL, levelBase);
}

static u32 DexNavGenerateMoveset(u32 species, u32 encounterLevel, u16* moveDst)
{
    bool32 genMove = FALSE;
    u16 eggMoveBuffer[EGG_MOVES_ARRAY_COUNT];
    u32 newSpecies, numEggMoves, randVal = RandomMax(100);
    u32 i, searchLevel = sDexNavSearchLevel;

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
    newSpecies = GenerateWildMon(species, encounterLevel, FALSE);

    // Store generated mon moves into Dex Nav Struct
    for (i = 0; i < MAX_MON_MOVES; i++)
        moveDst[i] = GetMonData(&gEnemyParty[0], MON_DATA_MOVE1 + i, NULL);

    // set first move slot to a random egg move if search level is good enough    
    if (genMove)
    {
        numEggMoves = GetEggMoves(&gEnemyParty[0], eggMoveBuffer);
        
        if (numEggMoves)
            moveDst[0] = eggMoveBuffer[RandomMax(numEggMoves)];
    }
    return newSpecies;
}

static u32 DexNavGenerateHeldItem(u32 species)
{
    u32 randVal = RandomMax(100);
    u32 searchLevelInfluence = sDexNavSearchLevel >> 1;
    u32 itemCommon = gSpeciesInfo[species].itemCommon;
    u32 itemRare = gSpeciesInfo[species].itemRare;

    // if no items can be held, then yeah...no items
    if (!itemCommon && !itemRare)
        return ITEM_NONE;

    // if both are the same, 100% to hold
    if (itemCommon == itemRare)
        return itemCommon;

    // if only one entry, 50% chance
    if (itemCommon && !itemRare)
        return (randVal < 50) ? itemCommon : ITEM_NONE;

    // if both are distinct:
    // itemCommon = 50% + srclvl / 2
    // itemRare = 5% + srchlvl / 2
    if (randVal < (50 + searchLevelInfluence + 5 + sDexNavSearchLevel))
        return (randVal > 5 + searchLevelInfluence) ? itemCommon : itemRare;
    
    return ITEM_NONE;
}

static bool32 DexNavGetAbilityNum(u32 species)
{
    u32 searchLevel = sDexNavSearchLevel;
    bool32 genAbility = FALSE, abilityHidden = FALSE;
    u32 randVal = RandomMax(100);

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
    if (genAbility && gSpeciesInfo[species].hiddenAbility && GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
        abilityHidden = TRUE;
        
    //Pick a normal ability of that Pokemon
    sDexNavSearchDataPtr->abilityNum = RandomMax(2);
    
    return abilityHidden;
}

static u32 DexNavGeneratePotential(void)
{
    u32 genChance = 0, searchLevel = sDexNavSearchLevel;
    u32 randVal = RandomMax(100);

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

static u32 GetEncounterLevelFromMapData(u32 species, u32 environment)
{
    u32 i, count, min, max;
    const struct WildPokemonInfo *monsInfo;
    
    switch (environment)
    {
        case WILD_HEADER_LAND:
            count = LAND_WILD_COUNT;
            break;
        case WILD_HEADER_WATER:
            count = WATER_WILD_COUNT;
            break;
        default:
            return MON_LEVEL_NONEXISTENT;
    }
    
    monsInfo = GetWildPokemonInfoByHeaderType(GetCurrentMapWildMonHeaderId(), environment);
    if (monsInfo == NULL)
        return MON_LEVEL_NONEXISTENT;
    
    min = MAX_LEVEL;
    max = 0;
    
    for (i = 0; i < count; i++)
    {
        if (monsInfo->wildPokemon[i].species == species)
        {
            min = (min < monsInfo->wildPokemon[i].minLevel) ? min : monsInfo->wildPokemon[i].minLevel;
            max = (max > monsInfo->wildPokemon[i].maxLevel) ? max : monsInfo->wildPokemon[i].maxLevel;
        }
    }

    if (!max)
        return MON_LEVEL_NONEXISTENT;

    return RandomUniform(RNG_NONE, min, max);
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
    u32 x, y;

    switch (sDexNavUiDataPtr->cursorRow)
    {
        case ROW_WATER:
            x = ROW_WATER_ICON_X + (24 * sDexNavUiDataPtr->cursorCol);
            y = ROW_WATER_ICON_Y;
            sDexNavUiDataPtr->environment = WILD_HEADER_WATER;
            break;
        case ROW_LAND_TOP:
            x = ROW_LAND_ICON_X + (24 * sDexNavUiDataPtr->cursorCol);
            y = ROW_LAND_TOP_ICON_Y;
            sDexNavUiDataPtr->environment = WILD_HEADER_LAND;
            break;
        case ROW_LAND_BOT:
            x = ROW_LAND_ICON_X + (24 * sDexNavUiDataPtr->cursorCol);
            y = ROW_LAND_BOT_ICON_Y;
            sDexNavUiDataPtr->environment = WILD_HEADER_LAND;
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
    struct CompressedSpriteSheet spriteSheet =
    {
        .data = sSelectionCursorGfx,
        .size = 0x200,
        .tag = SELECTION_CURSOR_TAG,
    };
    LoadCompressedSpriteSheet(&spriteSheet);

    sDexNavUiDataPtr->cursorSpriteId = CreateSprite(&sSelectionCursorSpriteTemplate, 12, 32, 0);  

    UpdateCursorPosition();
}

// Checks if all given area Pokemon have been captured
static bool32 CapturedAllMonsOfHeader(u32 headerId, u32 headerType)
{
    u32 species;
    u32 i, count, headerCount = (headerType == WILD_HEADER_WATER) ? WATER_WILD_COUNT : LAND_WILD_COUNT;
    const struct WildPokemonInfo* headerMonsInfo = GetWildPokemonInfoByHeaderType(headerId, headerType);

    if (headerMonsInfo != NULL)
    {        
        for (i = 0, count = 0; i < headerCount; ++i)
        {
            species = headerMonsInfo->wildPokemon[i].species;
            
            if (species && GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
                count++;
        }
        if (i >= headerCount && count > 0) // All mons caught
            return TRUE;
    }
    else
        return TRUE; // technically, no mon data means you caught them all

    return FALSE;
}

static void DexNavLoadCapturedAllSymbols(void)
{
    u32 headerId = GetCurrentMapWildMonHeaderId();

    if (CapturedAllMonsOfHeader(headerId, WILD_HEADER_WATER))
        Create8x8SymbolSprite(139, 28, 0, SYMBOL_POKEBALL);
    
    if (CapturedAllMonsOfHeader(headerId, WILD_HEADER_LAND))
        Create8x8SymbolSprite(153, 76, 0, SYMBOL_POKEBALL);
}

static void DexNavGuiFreeResources(void)
{
    Free(sDexNavUiDataPtr);
    FreeAllWindowBuffers();
}

static void CB1_InitDexNavSearch(void)
{
    u32 taskId;

    if (!gPaletteFade.active && !ScriptContext2_IsEnabled() && gMain.callback2 == CB2_Overworld)
    {
        SetMainCallback1(CB1_Overworld);
        
        taskId = CreateTask(Task_InitDexNavSearch, 0);
        gTasks[taskId].tSpecies = gSpecialVar_0x8000;
        gTasks[taskId].tEnvironment = gSpecialVar_0x8001;
    }
}

static void Task_DexNavExitAndSearch(u32 taskId)
{
    DexNavGuiFreeResources();
    DestroyTask(taskId);
    SetMainCallback1(CB1_InitDexNavSearch);
    SetMainCallback2(CB2_ReturnToField);
}

static void Task_DexNavFadeAndExit(u32 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sDexNavUiDataPtr->savedCallback);
        DexNavGuiFreeResources();
        DestroyTask(taskId);
    }
}

static bool32 SpeciesInArray(u32 species, u32 section)
{
    u32 i;
    
    switch (section)
    {
    case WILD_HEADER_LAND:
        for (i = 0; i < LAND_WILD_COUNT; i++)
        {
            if (sDexNavUiDataPtr->landSpecies[i] == species)
                return TRUE;
        }
        break;
    case WILD_HEADER_WATER:
        for (i = 0; i < WATER_WILD_COUNT; i++)
        {
            if (sDexNavUiDataPtr->waterSpecies[i] == species)
                return TRUE;
        }
        break;
    }
    return FALSE;
}

// get unique wild encounters on current map
static void DexNavLoadEncounterData(void)
{
    u32 i, grassIndex = 0, waterIndex = 0;
    u32 species, headerId = GetCurrentMapWildMonHeaderId();
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
            
            if (species && !SpeciesInArray(species, WILD_HEADER_LAND))
                sDexNavUiDataPtr->landSpecies[grassIndex++] = species;
        }
    }
    // water mons
    if (waterMonsInfo != NULL && waterMonsInfo->encounterRate)
    {
        for (i = 0; i < WATER_WILD_COUNT; i++)
        {
            species = waterMonsInfo->wildPokemon[i].species;
            
            if (species && !SpeciesInArray(species, WILD_HEADER_WATER))
                sDexNavUiDataPtr->waterSpecies[waterIndex++] = species;
        }
    }
}

static void TryDrawIconInSlot(u32 species, s16 x, s16 y)
{
    if (!species || species > NUM_SPECIES)
        CreateSprite(&sNoDataIconTemplate, x, y, 0); // 'X' in slot
    else
    {
        if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_SEEN))
            species = SPECIES_NONE; // question mark
        
        CreateMonIcon(species, SpriteCB_MonIcon, x, y, 0);
    }
}

static void DrawSpeciesIcons(void)
{
    u32 i, species;
    s16 x, y;

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

static u32 DexNavGetSeachableSpecies(void)
{
    switch (sDexNavUiDataPtr->cursorRow)
    {
        case ROW_WATER:
            return sDexNavUiDataPtr->waterSpecies[sDexNavUiDataPtr->cursorCol];
        case ROW_LAND_TOP:
            return sDexNavUiDataPtr->landSpecies[sDexNavUiDataPtr->cursorCol];
        case ROW_LAND_BOT:
            return sDexNavUiDataPtr->landSpecies[sDexNavUiDataPtr->cursorCol + COL_LAND_COUNT];
        default:
            return SPECIES_NONE;
    }
}

static u32 DexNavGetSpecies(void)
{
    u32 species = DexNavGetSeachableSpecies();

    if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_SEEN))
        species = SPECIES_NONE;

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
    u32 species = DexNavGetSpecies(), dexNum = SpeciesToNationalPokedexNum(species);

    if (!GetSetPokedexFlag(dexNum, FLAG_GET_SEEN))
        species = SPECIES_NONE;

    // clear window
    FillWindowPixelBuffer(WINDOW_INFO, PIXEL_FILL(0));

    // species name
    AddTextPrinterParameterized3(WINDOW_INFO, 0, 0, SPECIES_INFO_Y, sFontColor_Black, 0, species ? gSpeciesInfo[species].name : sText_DexNav_NoInfo);

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
        AddTextPrinterParameterized3(WINDOW_INFO, 0, 0, HA_INFO_Y, sFontColor_Black, 0, gSpeciesInfo[species].hiddenAbility ?
        gAbilities[gSpeciesInfo[species].hiddenAbility].name : gText_PokeSum_Item_None);
    else
        AddTextPrinterParameterized3(WINDOW_INFO, 0, 0, HA_INFO_Y, sFontColor_Black, 0, COMPOUND_STRING("Capture first!"));

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
    GetMapName(gStringVar4, GetCurrentRegionMapSectionId());
    AddTextPrinterParameterized3(WINDOW_MAP_NAME, 1, 2, 0, sFontColor_White, 0, gStringVar4);
    AddTextPrinterParameterized3(WINDOW_MAP_NAME, 1, 124, 0, sFontColor_White, 0, COMPOUND_STRING("{SELECT_BUTTON}Register {B_BUTTON}Cancel"));
    CopyWindowToVram(WINDOW_MAP_NAME, COPYWIN_BOTH);
}

static void PrintDexNavMessage(u32 msgId)
{
    const u8 *str;
    
    FillWindowPixelBuffer(WINDOW_MESSAGE, PIXEL_FILL(0));
    PutWindowTilemap(WINDOW_MESSAGE);
    
    switch (msgId)
    {
        case DEXNAV_MSG_CHOOSE_POKEMON:
            str = COMPOUND_STRING("Choose a Pokémon.");
            break;
        case DEXNAV_MSG_SEARCH_POKEMON:
            str = COMPOUND_STRING("Pokémon searchable via {L_BUTTON}!");
            break;
        case DEXNAV_MSG_NO_DATA:
            str = COMPOUND_STRING("This Pokémon don't have any data!");
            break;
    }
    AddTextPrinterParameterized3(WINDOW_MESSAGE, 1, 2, 0, sFontColor_White, 0, str);
    CopyWindowToVram(WINDOW_MESSAGE, COPYWIN_BOTH);
}

static void Task_DexNavWaitFadeIn(u32 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_DexNavMain;
}

static void DexNav_RunSetup(void)
{
    u32 taskId;

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
        ResetAllBgsPos();
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sDexNavMenuBgTemplates, ARRAY_COUNT(sDexNavMenuBgTemplates));
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
        sDexNavUiDataPtr->environment = WILD_HEADER_LAND;
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
        LoadSymbolsIconGraphics();
        CreateSelectionCursor();
        DrawSpeciesIcons();
        DexNavLoadCapturedAllSymbols();
        break;
    case 16:
        BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
        break;
    case 17:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
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

static void Task_DexNavMain(u32 taskId)
{
    struct Task *task = &gTasks[taskId];
    u32 species;

    if (IsSEPlaying())
        return;

    if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_POKENAV_OFF);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
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
        species = DexNavGetSeachableSpecies();

        if (species)
        {
            if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_SEEN))
                PrintDexNavMessage(DEXNAV_MSG_NO_DATA);
            else
            {
                VarSet(VAR_DEXNAV_SPECIES, species);
                VarSet(VAR_DEXNAV_ENVIRONMENT, sDexNavUiDataPtr->environment);
                PrintDexNavMessage(DEXNAV_MSG_SEARCH_POKEMON);
                PlayCry_Normal(species, 0);
                return;
            }
        }
        PlaySE(SE_FAILURE);
    }
    else if (JOY_NEW(A_BUTTON))
    {
        species = DexNavGetSeachableSpecies();
        
        if (species)
        {
            if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_SEEN))
                PrintDexNavMessage(DEXNAV_MSG_NO_DATA);
            else
            {
                gSpecialVar_0x8000 = species;
                gSpecialVar_0x8001 = sDexNavUiDataPtr->environment;
                PlaySE(SE_DEX_SEARCH);
                BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
                task->func = Task_DexNavExitAndSearch;
                return;
            }
        }
        PlaySE(SE_FAILURE);
    }
}

/////////////////////////
//// GENERAL UTILITY ////
/////////////////////////
bool32 IsDexNavSearchActive(void)
{
    return FuncIsActiveTask(Task_DexNavSearch);
}

void ResetDexNavSearch(void)
{
    sCurrentDexNavChain = 0;    // reset dex nav chaining on new map
    
    if (IsDexNavSearchActive())
        EndDexNavSearch(FindTaskIdByFunc(Task_DexNavSearch)); // moving to new map ends dexnav search
}

void IncrementOrResetDexNavChain(bool32 increment)
{
    if (increment)
    {
        if (sCurrentDexNavChain < DEXNAV_CHAIN_MAX)
            sCurrentDexNavChain++;
    }
    else
        sCurrentDexNavChain = 0;
}

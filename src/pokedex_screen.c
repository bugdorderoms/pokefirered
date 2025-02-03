#include "global.h"
#include "battle_main.h"
#include "gflib.h"
#include "graphics.h"
#include "m4a.h"
#include "pokemon_icon.h"
#include "scanline_effect.h"
#include "task.h"
#include "new_menu_helpers.h"
#include "event_data.h"
#include "list_menu.h"
#include "menu_indicators.h"
#include "overworld.h"
#include "strings.h"
#include "util.h"
#include "menu.h"
#include "pokedex_screen.h"
#include "data.h"
#include "text.h"
#include "pokedex.h"
#include "trainer_pokemon_sprites.h"
#include "decompress.h"
#include "constants/songs.h"
#include "constants/sound.h"
#include "constants/pokedex.h"
#include "constants/regions.h"
#include "pokedex_area_markers.h"
#include "field_specials.h"

enum TextMode
{
    TEXT_LEFT,
    TEXT_CENTER,
    TEXT_RIGHT
};

struct PokedexScreenData
{
	u32 unlockedCategories;
    u32 modeSelectInput;
    u16 modeSelectItemsAbove;
    u16 modeSelectCursorPos;
    u8 taskId;
    u8 state;
    u8 data[4];
    u8 modeSelectWindowId;
    u8 selectionIconWindowId;
    u8 dexCountsWindowId;
    u8 modeSelectListMenuId;
	u8 pageNum;
    u8 numMonsOnPage;
    u16 pageSpecies[4];
    u8 categoryMonWindowIds[4];
    u8 categoryMonInfoWindowIds[4];
    u8 category;
    u8 firstPageInCategory;
    u8 lastPageInCategory;
    u8 categoryCursorPosInPage;
    u8 categoryPageSelectionCursorTimer;
    u8 parentOfCategoryMenu;
	u16 kantoOrderMenuItemsAbove;
	u16 kantoOrderMenuCursorPos;
	u8 numericalOrderWindowId;
    u8 orderedListMenuTaskId;
    u32 characteristicMenuInput;
    u16 characteristicOrderMenuItemsAbove;
    u16 characteristicOrderMenuCursorPos;
    u16 nationalOrderMenuItemsAbove;
    u16 nationalOrderMenuCursorPos;
    struct ListMenuItem * listItems;
	u8 windowIds[0x10];
	u16 * bgBufsMem;
    u8 dexOrderId;
    u8 scrollArrowsTaskId;
    u8 categoryPageCursorTaskId;
	u8 unlockedSeviiAreas;
    u16 orderedDexCount;
    u16 dexSpecies;
    u16 modeSelectCursorPosBak;
	u16 *forms;
	u8 formIndex;
};

struct PokedexScreenWindowGfx
{
    const u32 * tiles;
    const u16 * pal;
};

struct PokedexCategoryPage
{
    const u16 * species;
    u8 count;
};

EWRAM_DATA static struct PokedexScreenData * sPokedexScreenData = NULL;

static void Task_PokedexScreen(u8 taskId);
static void DexScreen_InitGfxForTopMenu(void);
static void Task_DexScreen_NumericalOrder(u8 taskId);
static void DexScreen_InitGfxForNumericalOrderList(void);
static void Task_DexScreen_CharacteristicOrder(u8 taskId);
static void DexScreen_CreateCharacteristicListMenu(void);
static u16 DexScreen_CountMonsInOrderedList(u8 orderIdx);
static void DexScreen_InitListMenuForOrderedList(const struct ListMenuTemplate * template, u8 order);
static u8 DexScreen_CreateDexOrderScrollArrows(void);
static void DexScreen_DestroyDexOrderListMenu(u8 order);
static void Task_DexScreen_CategorySubmenu(u8 taskId);
static u8 DexScreen_CreateCategoryMenuScrollArrows(void);
static u8 DexScreen_InputHandler_GetShoulderInput(void);
static void Task_DexScreen_ShowMonPage(u8 taskId);
static bool32 DexScreen_TryScrollMonsVertical(u8 direction);
static void DexScreen_RemoveWindow(u8 *windowId_p);
static void DexScreen_AddTextPrinterParameterized(u8 windowId, u8 fontId, const u8 *str, u8 x, u8 y, u8 colorIdx);
static void DexScreen_PrintNum3RightAlign(u8 windowId, u8 fontId, u16 num, u8 x, u8 y, u8 colorIdx);
static void DexScreen_PrintNum4RightAlign(u8 windowId, u8 fontId, u16 num, u8 x, u8 y, u8 colorIdx);
static void DexScreen_PrintMonDexNo(u8 windowId, u8 fontId, u16 species, u8 x, u8 y);
static u16 DexScreen_GetDexCount(u8 caseId, u8 dexMode);
static void DexScreen_PrintControlInfo(const u8 *src);
static void DexScreen_DestroyCategoryPageMonIconAndInfoWindows(void);
static bool8 DexScreen_CreateCategoryListGfx(bool8 justRegistered);
static void DexScreen_CreateCategoryPageSelectionCursor(u8 cursorPos);
static void DexScreen_UpdateCategoryPageCursorObject(u8 taskId, u8 cursorPos, u8 numMonsInPage);
static bool8 DexScreen_FlipCategoryPageInDirection(u8 direction);
static void DexScreen_DexPageZoomEffectFrame(u8 bg, u8 scale);
static void DexScreen_DrawMonDexPage(bool8 justRegistered);
static void RemoveDexPageWindows(void);
static void DexScreen_DrawMonAreaPage(void);
static bool8 DexScreen_IsPageUnlocked(u8 category, u8 pageNum);
static bool8 DexScreen_IsCategoryUnlocked(u8 category);
static void DexScreen_GetPageLimitsForCategory(u8 category);
static bool8 DexScreen_LookUpCategoryBySpecies(u16 species);
static void DexScreen_DestroyAreaScreenResources(void);
static void DexScreen_CreateCategoryPageSpeciesList(u8 category, u8 pageNum);
static u8 DexScreen_PageNumberToRenderablePages(u16 page);
static void DexScreen_InputHandler_StartToCry(void);
static void DexScreen_PrintStringWithAlignment(const u8 *str, s32 mode);
static void MoveCursorFunc_DexModeSelect(s32 itemIndex, bool8 onInit, struct ListMenu *list);
static void ItemPrintFunc_DexModeSelect(u8 windowId, u32 itemId, u8 y);
static void ItemPrintFunc_OrderedListMenu(u8 windowId, u32 species, u8 y);
static void Task_DexScreen_RegisterMonToPokedex(u8 taskId);
static s8 CompareSpeciesAlphabetically(struct ListMenuItem *dexSlot1, struct ListMenuItem *dexSlot2);
static s8 CompareSpeciesByType(struct ListMenuItem *dexSlot1, struct ListMenuItem *dexSlot2);
static s8 CompareSpeciesByWeight(struct ListMenuItem *dexSlot1, struct ListMenuItem *dexSlot2);
static s8 CompareSpeciesByHeight(struct ListMenuItem *dexSlot1, struct ListMenuItem *dexSlot2);

const u32 sCategoryMonInfoBgTiles[] = INCBIN_U32("graphics/pokedex/mini_page.4bpp.lz");
const u32 sNatDexTiles[] = INCBIN_U32("graphics/pokedex/national_dex_bgtiles.4bpp.lz");

static const u16 sDexScreen_CategoryCursorPals[] =
{
    RGB(24, 22, 17), RGB(26, 24, 20),
    RGB(26, 20, 15), RGB(27, 23, 19),
    RGB(28, 18, 15), RGB(28, 22, 19),
    RGB(30, 16, 13), RGB(29, 21, 18),
    RGB(28, 18, 15), RGB(28, 22, 19),
    RGB(26, 20, 15), RGB(27, 23, 19)
};

static const u16 sNationalDexPalette[0x100] = INCBIN_U16("graphics/pokedex/national_dex_bgpals.gbapal");
static const u32 sTopMenuIconTiles_Cave[] = INCBIN_U32("graphics/pokedex/cat_icon_cave.4bpp.lz");
static const u32 sTopMenuIconTiles_Urban[] = INCBIN_U32("graphics/pokedex/cat_icon_urban.4bpp.lz");
static const u32 sTopMenuSelectionIconTiles_Cancel[] = INCBIN_U32("graphics/pokedex/cat_icon_cancel.4bpp.lz");
static const u32 sTopMenuIconTiles_Forest[] = INCBIN_U32("graphics/pokedex/cat_icon_forest.4bpp.lz");
static const u32 sTopMenuIconTiles_Grassland[] = INCBIN_U32("graphics/pokedex/cat_icon_grassland.4bpp.lz");
static const u32 sTopMenuIconTiles_QuestionMark[] = INCBIN_U32("graphics/pokedex/cat_icon_qmark.4bpp.lz");
static const u32 sTopMenuIconTiles_Mountain[] = INCBIN_U32("graphics/pokedex/cat_icon_mountain.4bpp.lz");
static const u32 sTopMenuIconTiles_Rare[] = INCBIN_U32("graphics/pokedex/cat_icon_rare.4bpp.lz");
static const u32 sTopMenuIconTiles_Sea[] = INCBIN_U32("graphics/pokedex/cat_icon_sea.4bpp.lz");
static const u32 sTopMenuIconTiles_Numerical[] = INCBIN_U32("graphics/pokedex/cat_icon_numerical.4bpp.lz");
static const u32 sTopMenuIconTiles_RoughTerrain[] = INCBIN_U32("graphics/pokedex/cat_icon_rough_terrain.4bpp.lz");
static const u32 sTopMenuIconTiles_WatersEdge[] = INCBIN_U32("graphics/pokedex/cat_icon_waters_edge.4bpp.lz");
static const u32 sTopMenuIconTiles_Type[] = INCBIN_U32("graphics/pokedex/cat_icon_type.4bpp.lz");
static const u32 sTopMenuIconTiles_Lightest[] = INCBIN_U32("graphics/pokedex/cat_icon_lightest.4bpp.lz");
static const u32 sTopMenuIconTiles_Smallest[] = INCBIN_U32("graphics/pokedex/cat_icon_smallest.4bpp.lz");
static const u16 sTopMenuIconPals_Cave[] = INCBIN_U16("graphics/pokedex/cat_icon_cave.gbapal");
static const u16 sTopMenuIconPals_Urban[] = INCBIN_U16("graphics/pokedex/cat_icon_urban.gbapal");
static const u16 sTopMenuSelectionIconPals_Cancel[] = INCBIN_U16("graphics/pokedex/cat_icon_cancel.gbapal");
static const u16 sTopMenuIconPals_Forest[] = INCBIN_U16("graphics/pokedex/cat_icon_forest.gbapal");
static const u16 sTopMenuIconPals_Grassland[] = INCBIN_U16("graphics/pokedex/cat_icon_grassland.gbapal");
static const u16 sTopMenuIconPals_QuestionMark[] = INCBIN_U16("graphics/pokedex/cat_icon_qmark.gbapal");
static const u16 sTopMenuIconPals_Mountain[] = INCBIN_U16("graphics/pokedex/cat_icon_mountain.gbapal");
static const u16 sTopMenuIconPals_Rare[] = INCBIN_U16("graphics/pokedex/cat_icon_rare.gbapal");
static const u16 sTopMenuIconPals_Sea[] = INCBIN_U16("graphics/pokedex/cat_icon_sea.gbapal");
static const u16 sTopMenuIconPals_Numerical[] = INCBIN_U16("graphics/pokedex/cat_icon_numerical.gbapal");
static const u16 sTopMenuIconPals_RoughTerrain[] = INCBIN_U16("graphics/pokedex/cat_icon_rough_terrain.gbapal");
static const u16 sTopMenuIconPals_WatersEdge[] = INCBIN_U16("graphics/pokedex/cat_icon_waters_edge.gbapal");
static const u16 sTopMenuIconPals_Type[] = INCBIN_U16("graphics/pokedex/cat_icon_type.gbapal");
static const u16 sTopMenuIconPals_Lightest[] = INCBIN_U16("graphics/pokedex/cat_icon_lightest.gbapal");
static const u16 sTopMenuIconPals_Smallest[] = INCBIN_U16("graphics/pokedex/cat_icon_smallest.gbapal");
static const u8 sDexScreen_CaughtIcon[] = INCBIN_U8("graphics/pokedex/caught_marker.4bpp");
static const u32 sTilemap_AreaMap_Kanto[] = INCBIN_U32("graphics/pokedex/map_kanto.4bpp.lz");
static const u32 sTilemap_AreaMap_OneIsland[] = INCBIN_U32("graphics/pokedex/map_one_island.4bpp.lz");
static const u32 sTilemap_AreaMap_TwoIsland[] = INCBIN_U32("graphics/pokedex/map_two_island.4bpp.lz");
static const u32 sTilemap_AreaMap_ThreeIsland[] = INCBIN_U32("graphics/pokedex/map_three_island.4bpp.lz");
static const u32 sTilemap_AreaMap_FourIsland[] = INCBIN_U32("graphics/pokedex/map_four_island.4bpp.lz");
static const u32 sTilemap_AreaMap_FiveIsland[] = INCBIN_U32("graphics/pokedex/map_five_island.4bpp.lz");
static const u32 sTilemap_AreaMap_SixIsland[] = INCBIN_U32("graphics/pokedex/map_six_island.4bpp.lz");
static const u32 sTilemap_AreaMap_SevenIsland[] = INCBIN_U32("graphics/pokedex/map_seven_island.4bpp.lz");
static const u16 sBlitTiles_WideEllipse[] = INCBIN_U16("graphics/pokedex/blit_wide_ellipse.4bpp");

#include "data/pokemon/pokedex_categories.h"

static const struct BgTemplate sBgTemplates[] = {
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 5,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0x0000
    },
    {
        .bg = 1,
        .charBaseIndex = 2,
        .mapBaseIndex = 4,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0x0000
    },
    {
        .bg = 2,
        .charBaseIndex = 2,
        .mapBaseIndex = 6,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0x0000
    },
    {
        .bg = 3,
        .charBaseIndex = 0,
        .mapBaseIndex = 7,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0x0000
    },
};

static const struct WindowTemplate sWindowTemplates[] = {
    {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 30,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 0x03c4
    },
    {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 18,
        .width = 30,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 0x0388
    },
    {
        .bg = 255,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 0,
        .height = 0,
        .paletteNum = 0,
        .baseBlock = 0x0000
    },
};

static const struct PokedexScreenData sDexScreenDataInitialState = {
    .modeSelectItemsAbove = 1,
    .modeSelectWindowId = -1,
    .selectionIconWindowId = -1,
    .dexCountsWindowId = -1,
    .pageSpecies = {-1, -1, -1, -1},
    .categoryMonWindowIds = {-1, -1, -1, -1},
    .categoryMonInfoWindowIds = {-1, -1, -1, -1},
    .numericalOrderWindowId = -1,
    .windowIds = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    .scrollArrowsTaskId = -1,
    .categoryPageCursorTaskId = -1,
	.formIndex = 0,
};


static const struct WindowTemplate sWindowTemplate_ModeSelect = {
    .bg = 1,
    .tilemapLeft = 1,
    .tilemapTop = 2,
    .width = 20,
    .height = 16,
    .paletteNum = 0,
    .baseBlock = 0x0008
};

static const struct WindowTemplate sWindowTemplate_SelectionIcon = {
    .bg = 1,
    .tilemapLeft = 21,
    .tilemapTop = 11,
    .width = 8,
    .height = 6,
    .paletteNum = 1,
    .baseBlock = 0x0148
};

static const struct WindowTemplate sWindowTemplate_DexCounts = {
    .bg = 1,
    .tilemapLeft = 21,
    .tilemapTop = 2,
    .width = 9,
    .height = 9,
    .paletteNum = 0,
    .baseBlock = 0x0178
};

static const struct ListMenuItem sListMenuItems_KantoDexModeSelect[] = {
    {gText_PokemonList,                  LIST_HEADER},
    {gText_NumericalMode,                DEX_MODE(NUMERICAL_KANTO)},
    {gText_PokemonHabitats,              LIST_HEADER},
    {gText_DexCategory_GrasslandPkmn,    DEX_CATEGORY_GRASSLAND},
    {gText_DexCategory_ForestPkmn,       DEX_CATEGORY_FOREST},
    {gText_DexCategory_WatersEdgePkmn,   DEX_CATEGORY_WATERS_EDGE},
    {gText_DexCategory_SeaPkmn,          DEX_CATEGORY_SEA},
    {gText_DexCategory_CavePkmn,         DEX_CATEGORY_CAVE},
    {gText_DexCategory_MountainPkmn,     DEX_CATEGORY_MOUNTAIN},
    {gText_DexCategory_RoughTerrainPkmn, DEX_CATEGORY_ROUGH_TERRAIN},
    {gText_DexCategory_UrbanPkmn,        DEX_CATEGORY_URBAN},
    {gText_DexCategory_RarePkmn,         DEX_CATEGORY_RARE},
    {gText_Search,                       LIST_HEADER},
    {gText_AToZMode,                     DEX_MODE(ATOZ)},
    {gText_TypeMode,                     DEX_MODE(TYPE)},
    {gText_LightestMode,                 DEX_MODE(LIGHTEST)},
    {gText_SmallestMode,                 DEX_MODE(SMALLEST)},
    {gText_PokedexOther,                 LIST_HEADER},
    {gText_ClosePokedex,                 LIST_CANCEL},
};

static const struct ListMenuItem sListMenuItems_NatDexModeSelect[] = {
    {gText_PokemonList,                  LIST_HEADER},
    {gText_NumericalModeKanto,           DEX_MODE(NUMERICAL_KANTO)},
    {gText_NumericalModeNational,        DEX_MODE(NUMERICAL_NATIONAL)},
    {gText_PokemonHabitats,              LIST_HEADER},
    {gText_DexCategory_GrasslandPkmn,    DEX_CATEGORY_GRASSLAND},
    {gText_DexCategory_ForestPkmn,       DEX_CATEGORY_FOREST},
    {gText_DexCategory_WatersEdgePkmn,   DEX_CATEGORY_WATERS_EDGE},
    {gText_DexCategory_SeaPkmn,          DEX_CATEGORY_SEA},
    {gText_DexCategory_CavePkmn,         DEX_CATEGORY_CAVE},
    {gText_DexCategory_MountainPkmn,     DEX_CATEGORY_MOUNTAIN},
    {gText_DexCategory_RoughTerrainPkmn, DEX_CATEGORY_ROUGH_TERRAIN},
    {gText_DexCategory_UrbanPkmn,        DEX_CATEGORY_URBAN},
    {gText_DexCategory_RarePkmn,         DEX_CATEGORY_RARE},
    {gText_Search,                       LIST_HEADER},
    {gText_AToZMode,                     DEX_MODE(ATOZ)},
    {gText_TypeMode,                     DEX_MODE(TYPE)},
    {gText_LightestMode,                 DEX_MODE(LIGHTEST)},
    {gText_SmallestMode,                 DEX_MODE(SMALLEST)},
    {gText_PokedexOther,                 LIST_HEADER},
    {gText_ClosePokedex,                 LIST_CANCEL},
};

static const struct ListMenuTemplate sListMenuTemplate_NatDexModeSelect = {
    .items = sListMenuItems_NatDexModeSelect,
    .moveCursorFunc = MoveCursorFunc_DexModeSelect,
    .itemPrintFunc = ItemPrintFunc_DexModeSelect,
    .totalItems = ARRAY_COUNT(sListMenuItems_NatDexModeSelect),
    .maxShowed = 9,
    .windowId = 0,
    .header_X = 0,
    .item_X = 12,
    .cursor_X = 4,
    .upText_Y = 2,
    .cursorPal = 1,
    .fillValue = 0,
    .cursorShadowPal = 3,
    .lettersSpacing = 1,
    .itemVerticalPadding = 0,
    .scrollMultiple = 0,
    .fontId = 2,
    .cursorKind = 0,
};

static const struct ScrollArrowsTemplate sScrollArrowsTemplate_NatDex = {
    .firstArrowType = 2,
    .firstX = 200,
    .firstY = 19,
    .secondArrowType = 3,
    .secondX = 200,
    .secondY = 141,
    .fullyUpThreshold = 0,
    .fullyDownThreshold = 11,
    .tileTag = 2000,
    .palTag = 0xFFFF,
    .palNum = 1
};


static const struct PokedexScreenWindowGfx sTopMenuSelectionIconGfxPtrs[] = {
    [DEX_CATEGORY_GRASSLAND] = {
        .tiles = sTopMenuIconTiles_Grassland,
        .pal   = sTopMenuIconPals_Grassland
    },
    [DEX_CATEGORY_FOREST] = {
        .tiles = sTopMenuIconTiles_Forest,
        .pal   = sTopMenuIconPals_Forest
    },
    [DEX_CATEGORY_WATERS_EDGE] = {
        .tiles = sTopMenuIconTiles_WatersEdge,
        .pal   = sTopMenuIconPals_WatersEdge
    },
    [DEX_CATEGORY_SEA] = {
        .tiles = sTopMenuIconTiles_Sea,
        .pal   = sTopMenuIconPals_Sea
    },
    [DEX_CATEGORY_CAVE] = {
        .tiles = sTopMenuIconTiles_Cave,
        .pal   = sTopMenuIconPals_Cave
    },
    [DEX_CATEGORY_MOUNTAIN] = {
        .tiles = sTopMenuIconTiles_Mountain,
        .pal   = sTopMenuIconPals_Mountain
    },
    [DEX_CATEGORY_ROUGH_TERRAIN] = {
        .tiles = sTopMenuIconTiles_RoughTerrain,
        .pal   = sTopMenuIconPals_RoughTerrain
    },
    [DEX_CATEGORY_URBAN] = {
        .tiles = sTopMenuIconTiles_Urban,
        .pal   = sTopMenuIconPals_Urban
    },
    [DEX_CATEGORY_RARE] = {
        .tiles = sTopMenuIconTiles_Rare,
        .pal   = sTopMenuIconPals_Rare
    },
    [DEX_MODE(NUMERICAL_KANTO)] = {
        .tiles = sTopMenuIconTiles_Numerical,
        .pal   = sTopMenuIconPals_Numerical
    },
    [DEX_MODE(ATOZ)] = {
        .tiles = gDexScreen_TopMenuIconTiles_AtoZ,
        .pal   = gDexScreen_TopMenuIconPals_AtoZ
    },
    [DEX_MODE(TYPE)] = {
        .tiles = sTopMenuIconTiles_Type,
        .pal   = sTopMenuIconPals_Type
    },
    [DEX_MODE(LIGHTEST)] = {
        .tiles = sTopMenuIconTiles_Lightest,
        .pal   = sTopMenuIconPals_Lightest
    },
    [DEX_MODE(SMALLEST)] = {
        .tiles = sTopMenuIconTiles_Smallest,
        .pal   = sTopMenuIconPals_Smallest
    },
    [DEX_MODE(NUMERICAL_NATIONAL)] = {
        .tiles = sTopMenuIconTiles_Numerical,
        .pal   = sTopMenuIconPals_Numerical
    },
};

static const struct WindowTemplate sWindowTemplate_OrderedListMenu = {
    .bg = 1,
    .tilemapLeft = 2,
    .tilemapTop = 2,
    .width = 23,
    .height = 16,
    .paletteNum = 0,
    .baseBlock = 0x0008
};

static const struct ListMenuTemplate sListMenuTemplate_OrderedListMenu = {
    .items = sListMenuItems_KantoDexModeSelect,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .itemPrintFunc = ItemPrintFunc_OrderedListMenu,
    .totalItems = 0,
    .maxShowed = 9,
    .windowId = 0,
    .header_X = 0,
    .item_X = 56,
    .cursor_X = 4,
    .upText_Y = 2,
    .cursorPal = 1,
    .fillValue = 0,
    .cursorShadowPal = 3,
    .lettersSpacing = 1,
    .itemVerticalPadding = 1,
    .scrollMultiple = 1,
    .fontId = 0,
    .cursorKind = 0,
};

static const struct ListMenuWindowRect sListMenuRects_OrderedList[] = {
    {
        .x = 0,
        .y = 0,
        .width = 5,
        .height = 16,
        .palNum = 0
    }, {
        .x = 5,
        .y = 0,
        .width = 2,
        .height = 16,
        .palNum = 1
    }, {
        .x = 7,
        .y = 0,
        .width = 8,
        .height = 16,
        .palNum = 0
    }, {
        .x = 15,
        .y = 0,
        .width = 8,
        .height = 16,
        .palNum = 2,
    }, {
        .x = 0xFF,
        .y = 0xFF,
        .width = 0xFF,
        .height = 0xFF,
        .palNum = 0xFF
    }
};

static const struct ScrollArrowsTemplate sDexOrderScrollArrowsTemplate = {
    .firstArrowType = 2,
    .firstX = 200,
    .firstY = 19,
    .secondArrowType = 3,
    .secondX = 200,
    .secondY = 141,
    .fullyUpThreshold = 0,
    .fullyDownThreshold = 0,
    .tileTag = 2000,
    .palTag = 0xFFFF,
    .palNum = 1,
};

static const struct WindowTemplate sWindowTemplate_CategoryMonIcon = {
    .bg = 2,
    .tilemapLeft = 0,
    .tilemapTop = 0,
    .width = 8,
    .height = 8,
    .paletteNum = 0,
    .baseBlock = 0x0000
};

static const struct WindowTemplate sWindowTemplate_CategoryMonInfo = {
    .bg = 1,
    .tilemapLeft = 0,
    .tilemapTop = 0,
    .width = 8,
    .height = 5,
    .paletteNum = 0,
    .baseBlock = 0x0000
};

static const struct WindowTemplate sWindowTemplate_DexEntry_MonPic = {
    .bg = 1,
    .tilemapLeft = 19,
    .tilemapTop = 3,
    .width = 8,
    .height = 8,
    .paletteNum = 9,
    .baseBlock = 0x01a8
};

static const struct WindowTemplate sWindowTemplate_DexEntry_SpeciesStats = {
    .bg = 1,
    .tilemapLeft = 2,
    .tilemapTop = 3,
    .width = 13,
    .height = 8,
    .paletteNum = 0,
    .baseBlock = 0x01e8
};

static const struct WindowTemplate sWindowTemplate_DexEntry_FlavorText = {
    .bg = 1,
    .tilemapLeft = 0,
    .tilemapTop = 11,
    .width = 30,
    .height = 7,
    .paletteNum = 0,
    .baseBlock = 0x0250
};

static const struct WindowTemplate sWindowTemplate_AreaMap_MonIcon = {
    .bg = 2,
    .tilemapLeft = 1,
    .tilemapTop = 2,
    .width = 4,
    .height = 4,
    .paletteNum = 10,
    .baseBlock = 0x01a8
};

static const struct WindowTemplate sWindowTemplate_AreaMap_SpeciesName = {
    .bg = 2,
    .tilemapLeft = 5,
    .tilemapTop = 2,
    .width = 8,
    .height = 3,
    .paletteNum = 0,
    .baseBlock = 0x01b8
};

static const struct WindowTemplate sWindowTemplate_AreaMap_Size = {
    .bg = 2,
    .tilemapLeft = 2,
    .tilemapTop = 7,
    .width = 10,
    .height = 10,
    .paletteNum = 0,
    .baseBlock = 0x01d0
};

static const struct WindowTemplate sWindowTemplate_AreaMap_Area = {
    .bg = 2,
    .tilemapLeft = 18,
    .tilemapTop = 2,
    .width = 10,
    .height = 2,
    .paletteNum = 0,
    .baseBlock = 0x01e4
};

static const struct WindowTemplate sWindowTemplate_AreaMap_MonTypes = {
    .bg = 2,
    .tilemapLeft = 5,
    .tilemapTop = 5,
    .width = 8,
    .height = 2,
    .paletteNum = 11,
    .baseBlock = 0x01f8
};

static const struct WindowTemplate sWindowTemplate_AreaMap_Kanto = {
    .bg = 2,
    .tilemapLeft = 17,
    .tilemapTop = 4,
    .width = 12,
    .height = 9,
    .paletteNum = 0,
    .baseBlock = 0x0208
};

static const struct WindowTemplate sWindowTemplate_AreaMap_OneIsland = {
    .bg = 2,
    .tilemapLeft = 13,
    .tilemapTop = 4,
    .width = 4,
    .height = 3,
    .paletteNum = 0,
    .baseBlock = 0x0274
};

static const struct WindowTemplate sWindowTemplate_AreaMap_TwoIsland = {
    .bg = 2,
    .tilemapLeft = 13,
    .tilemapTop = 7,
    .width = 4,
    .height = 3,
    .paletteNum = 0,
    .baseBlock = 0x0280
};

static const struct WindowTemplate sWindowTemplate_AreaMap_ThreeIsland = {
    .bg = 2,
    .tilemapLeft = 13,
    .tilemapTop = 10,
    .width = 4,
    .height = 3,
    .paletteNum = 0,
    .baseBlock = 0x028c
};

static const struct WindowTemplate sWindowTemplate_AreaMap_FourIsland = {
    .bg = 2,
    .tilemapLeft = 13,
    .tilemapTop = 13,
    .width = 4,
    .height = 4,
    .paletteNum = 0,
    .baseBlock = 0x0298
};

static const struct WindowTemplate sWindowTemplate_AreaMap_FiveIsland = {
    .bg = 2,
    .tilemapLeft = 17,
    .tilemapTop = 13,
    .width = 4,
    .height = 4,
    .paletteNum = 0,
    .baseBlock = 0x02a8
};

static const struct WindowTemplate sWindowTemplate_AreaMap_SixIsland = {
    .bg = 2,
    .tilemapLeft = 21,
    .tilemapTop = 13,
    .width = 4,
    .height = 4,
    .paletteNum = 0,
    .baseBlock = 0x02b8
};

static const struct WindowTemplate sWindowTemplate_AreaMap_SevenIsland = {
    .bg = 2,
    .tilemapLeft = 25,
    .tilemapTop = 13,
    .width = 4,
    .height = 4,
    .paletteNum = 0,
    .baseBlock = 0x02c8
};

struct {
    const struct WindowTemplate * window;
    const u32 * tiles;
} const sAreaMapStructs_SeviiIslands[] = {
    {&sWindowTemplate_AreaMap_OneIsland,   sTilemap_AreaMap_OneIsland},
    {&sWindowTemplate_AreaMap_TwoIsland,   sTilemap_AreaMap_TwoIsland},
    {&sWindowTemplate_AreaMap_ThreeIsland, sTilemap_AreaMap_ThreeIsland},
    {&sWindowTemplate_AreaMap_FourIsland,  sTilemap_AreaMap_FourIsland},
    {&sWindowTemplate_AreaMap_FiveIsland,  sTilemap_AreaMap_FiveIsland},
    {&sWindowTemplate_AreaMap_SixIsland,   sTilemap_AreaMap_SixIsland},
    {&sWindowTemplate_AreaMap_SevenIsland, sTilemap_AreaMap_SevenIsland},
};

static const u16 sCategoryPageIconWindowBg[] = INCBIN_U16("graphics/pokedex/page_icon_tilemap.bin");

// Circular window x/y; Rectangular window x/y
static const u8 sPageIconCoords_1Mon[1][4] = {
    {11,  3, 11, 11},
};

static const u8 sPageIconCoords_2Mons[2][4] = {
    { 3,  3, 11,  3},
    {18,  9, 10, 11},
};

static const u8 sPageIconCoords_3Mons[3][4] = {
    { 1,  2,  9,  2},
    {11,  9,  3, 11},
    {21,  3, 21, 11}
};

static const u8 sPageIconCoords_4Mons[4][4] = {
    { 0,  2,  6,  3},
    { 7, 10,  0, 12},
    {15, 10, 22, 11},
    {22,  2, 15,  4}
};

static const u8 (*const sCategoryPageIconCoords[])[4] = {
    sPageIconCoords_1Mon,
    sPageIconCoords_2Mons,
    sPageIconCoords_3Mons,
    sPageIconCoords_4Mons,
};

static const u8 * const sDexCategoryNamePtrs[] = {
    gText_DexCategory_GrasslandPkmn,
    gText_DexCategory_ForestPkmn,
    gText_DexCategory_WatersEdgePkmn,
    gText_DexCategory_SeaPkmn,
    gText_DexCategory_CavePkmn,
    gText_DexCategory_MountainPkmn,
    gText_DexCategory_RoughTerrainPkmn,
    gText_DexCategory_UrbanPkmn,
    gText_DexCategory_RarePkmn,
};

static const u16 sPalette_Silhouette[] = INCBIN_U16("graphics/pokedex/silhouette_sprite_pal.gbapal");

static const u8 sDexScreenPageTurnColumns[][30] = {
    {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},
    { 5, 11, 17, 23, 29, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},
    { 2,  5,  8, 11, 14, 17, 20, 23, 26, 29, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},
    { 2,  3,  5,  7,  9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},
    { 2,  4,  5,  7,  8, 10, 11, 13, 14, 16, 17, 19, 20, 22, 23, 25, 26, 28, 29, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},
    { 1,  2,  3,  4,  7,  8,  9, 10, 11, 12, 13, 15, 16, 17, 19, 20, 21, 23, 24, 25, 27, 28, 29, 30, 30, 30, 30, 30, 30, 30},
    { 1,  2,  3,  4,  5,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 21, 22, 23, 25, 26, 27, 28, 29, 30, 30, 30, 30},
    { 1,  2,  3,  4,  5,  6,  7,  8,  9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 30, 30},
    { 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30},
    { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29},
};

static const struct ScrollArrowsTemplate sScrollArrowsTemplate_CategoryMenu = {
    .firstArrowType = 0,
    .firstX = 16,
    .firstY = 80,
    .secondArrowType = 1,
    .secondX = 224,
    .secondY = 80,
    .fullyUpThreshold = 0,
    .fullyDownThreshold = 0,
    .tileTag = 2000,
    .palTag = 0xFFFF,
    .palNum = 1,
};

static const struct CursorStruct sCursorStruct_CategoryPage = {
    .left = 0,
    .top = 160,
    .rowWidth = 64,
    .rowHeight = 40,
    .tileTag = 2002,
    .palTag = 0xFFFF,
    .palNum = 4,
};

void VBlankCB_PokedexScreen(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void CB2_PokedexScreen(void)
{
    if (!gPaletteFade.active || IsDma3ManagerBusyWithBgCopy())
    {
        RunTasks();
        RunTextPrinters();
        AnimateSprites();
        BuildOamBuffer();
    }
    else
        UpdatePaletteFade();
}

void DexScreen_LoadResources(void)
{
    m4aSoundVSyncOff();
    SetVBlankCallback(NULL);
	
    ResetPaletteFade();
    ResetSpriteData();
    ResetTasks();
    ScanlineEffect_Stop();
    ResetBgsAndClearDma3BusyFlags(TRUE);
	
    InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
    SetBgTilemapBuffer(3, (u16*)Alloc(BG_SCREEN_SIZE));
    SetBgTilemapBuffer(2, (u16*)Alloc(BG_SCREEN_SIZE));
    SetBgTilemapBuffer(1, (u16*)Alloc(BG_SCREEN_SIZE));
    SetBgTilemapBuffer(0, (u16*)Alloc(BG_SCREEN_SIZE));
	DecompressAndLoadBgGfxUsingHeap(3, (void*)sNatDexTiles, BG_SCREEN_SIZE, 0, 0);
	
    InitWindows(sWindowTemplates);
    DeactivateAllTextPrinters();
	
    m4aSoundVSyncOn();
    SetVBlankCallback(VBlankCB_PokedexScreen);
    EnableInterrupts(INTR_FLAG_VBLANK);
	
    sPokedexScreenData = Alloc(sizeof(struct PokedexScreenData));
    *sPokedexScreenData = sDexScreenDataInitialState;
    sPokedexScreenData->taskId = CreateTask(Task_PokedexScreen, 0);
    sPokedexScreenData->listItems = Alloc(NATIONAL_DEX_END * sizeof(struct ListMenuItem));
    sPokedexScreenData->forms = Alloc(sizeof(u16) * 30); // To acomodate all Unown forms(28 in total) + 1 for a terminator. so, last halfword are unused
	
    ResetAllBgsPos();
	
    gPaletteFade.bufferTransferDisabled = TRUE;
	
	LoadPalette(sNationalDexPalette, 0, 0x200);

    FillBgTilemapBufferRect(3, 0x001, 0,  0, 32, 32, 0x00);
    FillBgTilemapBufferRect(2, 0x000, 0,  0, 32, 32, 0x11);
    FillBgTilemapBufferRect(1, 0x000, 0,  0, 32, 32, 0x11);
    FillBgTilemapBufferRect(0, 0x003, 0,  0, 32,  2, 0x0F);
    FillBgTilemapBufferRect(0, 0x000, 0,  2, 32, 16, 0x11);
    FillBgTilemapBufferRect(0, 0x003, 0, 18, 32,  2, 0x0F);
}

void CB2_OpenPokedexFromStartMenu(void)
{
    DexScreen_LoadResources();
    ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
    SetMainCallback2(CB2_PokedexScreen);
}

bool8 DoClosePokedex(void)
{
	u8 i;
	void *bgbuff;
	
    switch (gMain.state)
    {
    case 0:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        gMain.state++;
        return FALSE;
    case 1:
        if (!gPaletteFade.active)
            gMain.state = 2;
        else
            UpdatePaletteFade();
        return FALSE;
    case 2:
        FREE_IF_NOT_NULL(sPokedexScreenData->listItems);
		FREE_IF_NOT_NULL(sPokedexScreenData->forms);
        FREE_IF_NOT_NULL(sPokedexScreenData);
        FreeAllWindowBuffers();
		
		for (i = 0; i < 4; i++)
		{
			bgbuff = GetBgTilemapBuffer(i);
			FREE_IF_NOT_NULL(bgbuff);
		}
        break;
    }
    return TRUE;
}

void CB2_ClosePokedex(void)
{
    if (DoClosePokedex())
    {
        SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
        SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
    }
}

static void Task_PokedexScreen(u8 taskId)
{
    int i;
	
    switch (sPokedexScreenData->state)
    {
    case 0:
        sPokedexScreenData->unlockedCategories = 0;
        for (i = 0; i < 9; i++)
            sPokedexScreenData->unlockedCategories |= (DexScreen_IsCategoryUnlocked(i) << i);
		
        sPokedexScreenData->state = 2;
        break;
    case 1:
        RemoveScrollIndicatorArrowPair(sPokedexScreenData->scrollArrowsTaskId);
        DexScreen_RemoveWindow(&sPokedexScreenData->modeSelectWindowId);
        DexScreen_RemoveWindow(&sPokedexScreenData->selectionIconWindowId);
        DexScreen_RemoveWindow(&sPokedexScreenData->dexCountsWindowId);
        SetMainCallback2(CB2_ClosePokedex);
        DestroyTask(taskId);
        break;
    case 2:
        DexScreen_InitGfxForTopMenu();
        sPokedexScreenData->state = 3;
        break;
    case 3:
        CopyBgTilemapBufferToVram(3);
        CopyBgTilemapBufferToVram(2);
        CopyBgTilemapBufferToVram(1);
        CopyBgTilemapBufferToVram(0);
        sPokedexScreenData->state = 4;
        break;
    case 4:
        ShowBg(3);
        ShowBg(2);
        ShowBg(1);
        ShowBg(0);
		
        if (gPaletteFade.bufferTransferDisabled)
        {
            gPaletteFade.bufferTransferDisabled = FALSE;
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0, RGB_WHITEALPHA);
        }
        else
            BeginNormalPaletteFade(~0x8000, 0, 16, 0, RGB_WHITEALPHA);
		
        sPokedexScreenData->state = 5;
        break;
    case 5:
        ListMenuGetScrollAndRow(sPokedexScreenData->modeSelectListMenuId, &sPokedexScreenData->modeSelectCursorPosBak, NULL);
		sPokedexScreenData->scrollArrowsTaskId = AddScrollIndicatorArrowPair(&sScrollArrowsTemplate_NatDex, &sPokedexScreenData->modeSelectCursorPosBak);
        sPokedexScreenData->state = 6;
        break;
    case 6:
        sPokedexScreenData->modeSelectInput = ListMenu_ProcessInput(sPokedexScreenData->modeSelectListMenuId);
        ListMenuGetScrollAndRow(sPokedexScreenData->modeSelectListMenuId, &sPokedexScreenData->modeSelectCursorPosBak, NULL);
		
        if (JOY_NEW(A_BUTTON))
        {
            switch (sPokedexScreenData->modeSelectInput)
            {
            case LIST_CANCEL:
                sPokedexScreenData->state = 1;
                break;
            case DEX_CATEGORY_GRASSLAND:
            case DEX_CATEGORY_FOREST:
            case DEX_CATEGORY_WATERS_EDGE:
            case DEX_CATEGORY_SEA:
            case DEX_CATEGORY_CAVE:
            case DEX_CATEGORY_MOUNTAIN:
            case DEX_CATEGORY_ROUGH_TERRAIN:
            case DEX_CATEGORY_URBAN:
            case DEX_CATEGORY_RARE:
                if (DexScreen_IsCategoryUnlocked(sPokedexScreenData->modeSelectInput))
                {
                    RemoveScrollIndicatorArrowPair(sPokedexScreenData->scrollArrowsTaskId);
                    sPokedexScreenData->category = sPokedexScreenData->modeSelectInput;
                    BeginNormalPaletteFade(~0x8000, 0, 0, 16, RGB_WHITEALPHA);
                    sPokedexScreenData->state = 7;
                }
                break;
            case DEX_MODE(NUMERICAL_KANTO):
            case DEX_MODE(NUMERICAL_NATIONAL):
                RemoveScrollIndicatorArrowPair(sPokedexScreenData->scrollArrowsTaskId);
                sPokedexScreenData->dexOrderId = sPokedexScreenData->modeSelectInput - DEX_CATEGORY_COUNT;
                BeginNormalPaletteFade(~0x8000, 0, 0, 16, RGB_WHITEALPHA);
                sPokedexScreenData->state = 9;
                break;
            case DEX_MODE(ATOZ):
            case DEX_MODE(TYPE):
            case DEX_MODE(LIGHTEST):
            case DEX_MODE(SMALLEST):
                RemoveScrollIndicatorArrowPair(sPokedexScreenData->scrollArrowsTaskId);
                sPokedexScreenData->dexOrderId = sPokedexScreenData->modeSelectInput - DEX_CATEGORY_COUNT;
                sPokedexScreenData->characteristicOrderMenuItemsAbove = sPokedexScreenData->characteristicOrderMenuCursorPos = 0;
                BeginNormalPaletteFade(~0x8000, 0, 0, 16, RGB_WHITEALPHA);
                sPokedexScreenData->state = 8;
                break;
            }
            break;
        }
		
        if (JOY_NEW(B_BUTTON))
            sPokedexScreenData->state = 1;
        break;
    case 7:
        DestroyListMenuTask(sPokedexScreenData->modeSelectListMenuId, &sPokedexScreenData->modeSelectCursorPos, &sPokedexScreenData->modeSelectItemsAbove);
        
		FillBgTilemapBufferRect_Palette0(1, 0, 0, 0, 32, 20);
        CopyBgTilemapBufferToVram(1);
		
        DexScreen_RemoveWindow(&sPokedexScreenData->modeSelectWindowId);
        DexScreen_RemoveWindow(&sPokedexScreenData->selectionIconWindowId);
        DexScreen_RemoveWindow(&sPokedexScreenData->dexCountsWindowId);
		
        sPokedexScreenData->pageNum = 0;
        sPokedexScreenData->categoryCursorPosInPage = 0;
        sPokedexScreenData->parentOfCategoryMenu = 0;
        gTasks[taskId].func = Task_DexScreen_CategorySubmenu;
		
        sPokedexScreenData->state = 0;
        break;
    case 8:
        DestroyListMenuTask(sPokedexScreenData->modeSelectListMenuId, &sPokedexScreenData->modeSelectCursorPos, &sPokedexScreenData->modeSelectItemsAbove);
        
		HideBg(1);
		
        DexScreen_RemoveWindow(&sPokedexScreenData->modeSelectWindowId);
        DexScreen_RemoveWindow(&sPokedexScreenData->selectionIconWindowId);
        DexScreen_RemoveWindow(&sPokedexScreenData->dexCountsWindowId);
		
        gTasks[taskId].func = Task_DexScreen_CharacteristicOrder;
		
        sPokedexScreenData->state = 0;
        break;
    case 9:
        DestroyListMenuTask(sPokedexScreenData->modeSelectListMenuId, &sPokedexScreenData->modeSelectCursorPos, &sPokedexScreenData->modeSelectItemsAbove);
        
		HideBg(1);
		
        DexScreen_RemoveWindow(&sPokedexScreenData->modeSelectWindowId);
        DexScreen_RemoveWindow(&sPokedexScreenData->selectionIconWindowId);
        DexScreen_RemoveWindow(&sPokedexScreenData->dexCountsWindowId);
		
        gTasks[taskId].func = Task_DexScreen_NumericalOrder;
		
        sPokedexScreenData->state = 0;
        break;
    }
}

static void DexScreen_InitGfxForTopMenu(void)
{
    struct ListMenuTemplate listMenuTemplate;
	
    FillBgTilemapBufferRect(3, 0x00E, 0, 0, 30, 20, 0x00);
    FillBgTilemapBufferRect(2, 0x000, 0, 0, 30, 20, 0x11);
    FillBgTilemapBufferRect(1, 0x000, 0, 0, 30, 20, 0x11);
	
    sPokedexScreenData->modeSelectWindowId = AddWindow(&sWindowTemplate_ModeSelect);
    sPokedexScreenData->selectionIconWindowId = AddWindow(&sWindowTemplate_SelectionIcon);
    sPokedexScreenData->dexCountsWindowId = AddWindow(&sWindowTemplate_DexCounts);
	
	listMenuTemplate = sListMenuTemplate_NatDexModeSelect;
	listMenuTemplate.windowId = sPokedexScreenData->modeSelectWindowId;
	
	sPokedexScreenData->modeSelectListMenuId = ListMenuInit(&listMenuTemplate, sPokedexScreenData->modeSelectCursorPos, sPokedexScreenData->modeSelectItemsAbove);
	FillWindowPixelBuffer(sPokedexScreenData->dexCountsWindowId, PIXEL_FILL(0));
	
	DexScreen_AddTextPrinterParameterized(sPokedexScreenData->dexCountsWindowId, 0, gText_Seen, 0, 2, 0);
	DexScreen_AddTextPrinterParameterized(sPokedexScreenData->dexCountsWindowId, 0, gText_Kanto, 8, 13, 0);
    DexScreen_PrintNum3RightAlign(sPokedexScreenData->dexCountsWindowId, 0, DexScreen_GetDexCount(FLAG_GET_SEEN, DEX_MODE_KANTO), 52, 13, 2);
    DexScreen_AddTextPrinterParameterized(sPokedexScreenData->dexCountsWindowId, 0, gText_National, 8, 24, 0);
    DexScreen_PrintNum4RightAlign(sPokedexScreenData->dexCountsWindowId, 0, DexScreen_GetDexCount(FLAG_GET_SEEN, DEX_MODE_NATIONAL), 52, 24, 2);
    DexScreen_AddTextPrinterParameterized(sPokedexScreenData->dexCountsWindowId, 0, gText_Owned, 0, 37, 0);
    DexScreen_AddTextPrinterParameterized(sPokedexScreenData->dexCountsWindowId, 0, gText_Kanto, 8, 48, 0);
    DexScreen_PrintNum3RightAlign(sPokedexScreenData->dexCountsWindowId, 0, DexScreen_GetDexCount(FLAG_GET_CAUGHT, DEX_MODE_KANTO), 52, 48, 2);
    DexScreen_AddTextPrinterParameterized(sPokedexScreenData->dexCountsWindowId, 0, gText_National, 8, 59, 0);
    DexScreen_PrintNum4RightAlign(sPokedexScreenData->dexCountsWindowId, 0, DexScreen_GetDexCount(FLAG_GET_CAUGHT, DEX_MODE_NATIONAL), 52, 59, 2);
	
    FillWindowPixelBuffer(0, PIXEL_FILL(15));
    DexScreen_PrintStringWithAlignment(gText_PokedexTableOfContents, TEXT_CENTER);
    FillWindowPixelBuffer(1, PIXEL_FILL(15));
    DexScreen_PrintControlInfo(gText_PickOK);
	
    PutWindowTilemap(0);
    CopyWindowToVram(0, COPYWIN_GFX);
    PutWindowTilemap(1);
    CopyWindowToVram(1, COPYWIN_GFX);
    PutWindowTilemap(sPokedexScreenData->dexCountsWindowId);
    CopyWindowToVram(sPokedexScreenData->dexCountsWindowId, COPYWIN_GFX);
}

static void MoveCursorFunc_DexModeSelect(s32 itemIndex, bool8 onInit, struct ListMenu *list)
{
    if (!onInit)
        PlaySE(SE_SELECT);
	
    if (itemIndex == LIST_CANCEL)
    {
        CopyToWindowPixelBuffer(sPokedexScreenData->selectionIconWindowId, sTopMenuSelectionIconTiles_Cancel, 0x000, 0x000);
        LoadPalette(sTopMenuSelectionIconPals_Cancel, 0x10, 0x20);
    }
    else
    {
        CopyToWindowPixelBuffer(sPokedexScreenData->selectionIconWindowId, sTopMenuSelectionIconGfxPtrs[itemIndex].tiles, 0x000, 0x000);
        LoadPalette(sTopMenuSelectionIconGfxPtrs[itemIndex].pal, 0x10, 0x20);
    }
    PutWindowTilemap(sPokedexScreenData->selectionIconWindowId);
    CopyWindowToVram(sPokedexScreenData->selectionIconWindowId, COPYWIN_GFX);
}

static void ItemPrintFunc_DexModeSelect(u8 windowId, u32 itemId, u8 y)
{
    if (itemId >= DEX_CATEGORY_COUNT || sPokedexScreenData->unlockedCategories & (1 << itemId))
        ListMenuOverrideSetColors(TEXT_COLOR_WHITE, TEXT_COLOR_TRANSPARENT, TEXT_COLOR_LIGHT_GRAY);
    else
        ListMenuOverrideSetColors(TEXT_DYNAMIC_COLOR_1, TEXT_COLOR_TRANSPARENT, TEXT_DYNAMIC_COLOR_2);
}

static void Task_DexScreen_NumericalOrder(u8 taskId)
{
    switch (sPokedexScreenData->state)
    {
    case 0:
        ListMenuLoadStdPalAt(0x10, 0);
        ListMenuLoadStdPalAt(0x20, 1);
        sPokedexScreenData->orderedDexCount = DexScreen_CountMonsInOrderedList(sPokedexScreenData->dexOrderId);
        sPokedexScreenData->state = 2;
        break;
    case 1:
        DexScreen_DestroyDexOrderListMenu(sPokedexScreenData->dexOrderId);
        HideBg(1);
        DexScreen_RemoveWindow(&sPokedexScreenData->numericalOrderWindowId);
        gTasks[taskId].func = Task_PokedexScreen;
        sPokedexScreenData->state = 0;
        break;
    case 2:
        DexScreen_InitGfxForNumericalOrderList();
        sPokedexScreenData->state = 3;
        break;
    case 3:
        CopyBgTilemapBufferToVram(3);
        CopyBgTilemapBufferToVram(1);
        sPokedexScreenData->state = 4;
        break;
    case 4:
        ShowBg(1);
        BeginNormalPaletteFade(~0x8000, 0, 16, 0, RGB_WHITEALPHA);
        sPokedexScreenData->state = 5;
        break;
    case 5:
        ListMenuGetScrollAndRow(sPokedexScreenData->modeSelectListMenuId, &sPokedexScreenData->modeSelectCursorPosBak, NULL);
        sPokedexScreenData->scrollArrowsTaskId = DexScreen_CreateDexOrderScrollArrows();
        sPokedexScreenData->state = 6;
        break;
    case 6:
        sPokedexScreenData->characteristicMenuInput = ListMenu_ProcessInput(sPokedexScreenData->orderedListMenuTaskId);
        ListMenuGetScrollAndRow(sPokedexScreenData->modeSelectListMenuId, &sPokedexScreenData->modeSelectCursorPosBak, NULL);
		
        if (JOY_NEW(A_BUTTON))
        {
            if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(sPokedexScreenData->characteristicMenuInput), FLAG_GET_SEEN))
            {
				sPokedexScreenData->dexSpecies = sPokedexScreenData->characteristicMenuInput;
                RemoveScrollIndicatorArrowPair(sPokedexScreenData->scrollArrowsTaskId);
                BeginNormalPaletteFade(~0x8000, 0, 0, 16, RGB_WHITEALPHA);
                sPokedexScreenData->state = 7;
            }
        }
        else if (JOY_NEW(B_BUTTON))
        {
            RemoveScrollIndicatorArrowPair(sPokedexScreenData->scrollArrowsTaskId);
            BeginNormalPaletteFade(~0x8000, 0, 0, 16, RGB_WHITEALPHA);
            sPokedexScreenData->state = 1;
        }
        break;
    case 7:
        DexScreen_DestroyDexOrderListMenu(sPokedexScreenData->dexOrderId);
        FillBgTilemapBufferRect_Palette0(1, 0x000, 0, 0, 32, 20);
        CopyBgTilemapBufferToVram(1);
        DexScreen_RemoveWindow(&sPokedexScreenData->numericalOrderWindowId);
        gTasks[taskId].func = Task_DexScreen_ShowMonPage;
        sPokedexScreenData->state = 0;
        break;
    }
}

static void DexScreen_InitGfxForNumericalOrderList(void)
{
    struct ListMenuTemplate template;
	
    FillBgTilemapBufferRect(3, 0x00E, 0, 0, 30, 20, 0x00);
    FillBgTilemapBufferRect(1, 0x000, 0, 0, 32, 32, 0x11);
	
    sPokedexScreenData->numericalOrderWindowId = AddWindow(&sWindowTemplate_OrderedListMenu);
    template = sListMenuTemplate_OrderedListMenu;
    template.items = sPokedexScreenData->listItems;
    template.windowId = sPokedexScreenData->numericalOrderWindowId;
    template.totalItems = sPokedexScreenData->orderedDexCount;
    DexScreen_InitListMenuForOrderedList(&template, sPokedexScreenData->dexOrderId);
    FillWindowPixelBuffer(0, PIXEL_FILL(15));
    DexScreen_PrintStringWithAlignment(gText_PokemonListNoColor, TEXT_CENTER);
    FillWindowPixelBuffer(1, PIXEL_FILL(15));
    DexScreen_PrintControlInfo(gText_PickOKExit);
    CopyWindowToVram(0, COPYWIN_GFX);
    CopyWindowToVram(1, COPYWIN_GFX);
}

static void Task_DexScreen_CharacteristicOrder(u8 taskId)
{
    switch (sPokedexScreenData->state)
    {
    case 0:
        ListMenuLoadStdPalAt(0x10, 0);
        ListMenuLoadStdPalAt(0x20, 1);
        sPokedexScreenData->orderedDexCount = DexScreen_CountMonsInOrderedList(sPokedexScreenData->dexOrderId);
        sPokedexScreenData->state = 2;
        break;
    case 1:
        DexScreen_DestroyDexOrderListMenu(sPokedexScreenData->dexOrderId);
        HideBg(1);
        DexScreen_RemoveWindow(&sPokedexScreenData->numericalOrderWindowId);
        gTasks[taskId].func = Task_PokedexScreen;
        sPokedexScreenData->state = 0;
        break;
    case 2:
        DexScreen_CreateCharacteristicListMenu();
        sPokedexScreenData->state = 3;
        break;
    case 3:
        CopyBgTilemapBufferToVram(3);
        CopyBgTilemapBufferToVram(1);
        sPokedexScreenData->state = 4;
        break;
    case 4:
        ShowBg(1);
        BeginNormalPaletteFade(~0x8000, 0, 16, 0, RGB_WHITEALPHA);
        sPokedexScreenData->state = 5;
        break;
    case 5:
        ListMenuGetScrollAndRow(sPokedexScreenData->modeSelectListMenuId, &sPokedexScreenData->modeSelectCursorPosBak, NULL);
        sPokedexScreenData->scrollArrowsTaskId = DexScreen_CreateDexOrderScrollArrows();
        sPokedexScreenData->state = 6;
        break;
    case 6:
        sPokedexScreenData->characteristicMenuInput = ListMenu_ProcessInput(sPokedexScreenData->orderedListMenuTaskId);
        ListMenuGetScrollAndRow(sPokedexScreenData->modeSelectListMenuId, &sPokedexScreenData->modeSelectCursorPosBak, NULL);
		
        if (JOY_NEW(A_BUTTON))
        {
            if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(sPokedexScreenData->characteristicMenuInput), FLAG_GET_SEEN) && !DexScreen_LookUpCategoryBySpecies(sPokedexScreenData->characteristicMenuInput))
            {
                RemoveScrollIndicatorArrowPair(sPokedexScreenData->scrollArrowsTaskId);
                BeginNormalPaletteFade(~0x8000, 0, 0, 16, RGB_WHITEALPHA);
                sPokedexScreenData->state = 7;
            }
        }
        else if (JOY_NEW(B_BUTTON))
        {
            RemoveScrollIndicatorArrowPair(sPokedexScreenData->scrollArrowsTaskId);
            BeginNormalPaletteFade(~0x8000, 0, 0, 16, RGB_WHITEALPHA);
            sPokedexScreenData->state = 1;
        }
        break;
    case 7:
        DexScreen_DestroyDexOrderListMenu(sPokedexScreenData->dexOrderId);
        FillBgTilemapBufferRect_Palette0(1, 0x000, 0, 0, 32, 20);
        CopyBgTilemapBufferToVram(1);
        DexScreen_RemoveWindow(&sPokedexScreenData->numericalOrderWindowId);
        sPokedexScreenData->parentOfCategoryMenu = 1;
        gTasks[taskId].func = Task_DexScreen_CategorySubmenu;
        sPokedexScreenData->state = 0;
        break;
    }
}

static void DexScreen_CreateCharacteristicListMenu(void)
{
    struct ListMenuTemplate template;
	
    FillBgTilemapBufferRect(3, 0x00E, 0, 0, 30, 20, 0x00);
    FillBgTilemapBufferRect(1, 0x000, 0, 0, 32, 32, 0x11);
	
    sPokedexScreenData->numericalOrderWindowId = AddWindow(&sWindowTemplate_OrderedListMenu);
    template = sListMenuTemplate_OrderedListMenu;
    template.items = sPokedexScreenData->listItems;
    template.windowId = sPokedexScreenData->numericalOrderWindowId;
    template.totalItems = sPokedexScreenData->orderedDexCount;
    DexScreen_InitListMenuForOrderedList(&template, sPokedexScreenData->dexOrderId);
    FillWindowPixelBuffer(0, PIXEL_FILL(15));
    DexScreen_PrintStringWithAlignment(gText_SearchNoColor, TEXT_CENTER);
    FillWindowPixelBuffer(1, PIXEL_FILL(15));
    DexScreen_PrintControlInfo(gText_PickOKExit);
    CopyWindowToVram(0, COPYWIN_GFX);
    CopyWindowToVram(1, COPYWIN_GFX);
}

static u16 SortDex(u8 region, u8 getSetFlag, s8 (*sortFunc)(struct ListMenuItem*, struct ListMenuItem*))
{
	struct SortComparator comparator;
	u16 i, num, dexStart, dexEnd, ret = NATIONAL_DEX_NONE;
	
	if (region == REGIONS_COUNT) // National dex
	{
		dexStart = 0;
		dexEnd = NATIONAL_DEX_END;
	}
	else // Specific region's dex
	{
		dexStart = gRegions[region].dexStart - 1;
		dexEnd = gRegions[region].dexEnd;
	}
	
	if (sortFunc == NULL) // No sort
	{
		for (i = dexStart; i < dexEnd; i++)
		{
			num = i + 1;
			
			if (GetSetPokedexFlag(num, getSetFlag))
			{
				sPokedexScreenData->listItems[i].label = gSpeciesInfo[num].name;
				ret = num; // Returns last index
			}
			else
				sPokedexScreenData->listItems[i].label = gText_5Dashes;
			
			sPokedexScreenData->listItems[i].index = num;
		}
	}
	else
	{
		for (i = dexStart; i < dexEnd; i++)
		{
			num = i + 1;
			
			if (GetSetPokedexFlag(num, getSetFlag))
			{
				sPokedexScreenData->listItems[ret].label = gSpeciesInfo[num].name;
				sPokedexScreenData->listItems[ret].index = num;
				++ret; // Returns N. of pokes in list
			}
		}
		
		if (ret > 1)
		{
			comparator.kind = SORT_LIST_MENU_ITEMS;
			comparator.sortUnion.listMenuItemSort.array = sPokedexScreenData->listItems;
			comparator.sortUnion.listMenuItemSort.func = sortFunc;
			MergeSortArray(&comparator, ret);
		}
	}
	return ret;
}

static u16 DexScreen_CountMonsInOrderedList(u8 orderIdx)
{
    u16 ret;

    switch (orderIdx)
    {
    default:
    case DEX_ORDER_NUMERICAL_KANTO:
	    ret = SortDex(REGION_KANTO, FLAG_GET_SEEN, NULL);
        break;
    case DEX_ORDER_ATOZ:
	    ret = SortDex(REGIONS_COUNT, FLAG_GET_SEEN, CompareSpeciesAlphabetically);
        break;
    case DEX_ORDER_TYPE:
	    ret = SortDex(REGIONS_COUNT, FLAG_GET_CAUGHT, CompareSpeciesByType);
        break;
    case DEX_ORDER_LIGHTEST:
	    ret = SortDex(REGIONS_COUNT, FLAG_GET_CAUGHT, CompareSpeciesByWeight); // Lightest to heaviest
        break;
    case DEX_ORDER_SMALLEST:
	    ret = SortDex(REGIONS_COUNT, FLAG_GET_CAUGHT, CompareSpeciesByHeight); // Smallest to largest
        break;
    case DEX_ORDER_NUMERICAL_NATIONAL:
	    ret = SortDex(REGIONS_COUNT, FLAG_GET_SEEN, NULL);
        break;
    }
    return ret;
}

static s8 CompareSpeciesAlphabetically(struct ListMenuItem *dexSlot1, struct ListMenuItem *dexSlot2)
{
	return CompareTextAlphabetically(dexSlot1->label, dexSlot2->label);
}

static s8 CompareSpeciesByType(struct ListMenuItem *dexSlot1, struct ListMenuItem *dexSlot2)
{
	u8 slot1Types[2] = {gSpeciesInfo[dexSlot1->index].types[0], gSpeciesInfo[dexSlot1->index].types[1]};
	u8 slot2Types[2] = {gSpeciesInfo[dexSlot2->index].types[0], gSpeciesInfo[dexSlot2->index].types[1]};
	bool8 slot1HasTwoTypes = (slot1Types[0] != slot1Types[1]);
	bool8 slot2HasTwoTypes = (slot2Types[0] != slot2Types[1]);
	
	if (!slot1HasTwoTypes && slot2HasTwoTypes) // Slot 2 has two types, Slot 1 has only one so put it first
		return -1;
	else if (slot1HasTwoTypes && !slot2HasTwoTypes) // slot 1 has two types, Slot 2 has only one so put it first
		return 1;
	else if (slot1Types[0] < slot2Types[0]) // Check first type
	    return -1;
	else if (slot1Types[0] > slot2Types[0])
		return 1;
	else if (slot1HasTwoTypes && slot2HasTwoTypes)
	{
		// Check second type
		if (slot1Types[1] < slot2Types[1])
			return -1;
		else if (slot1Types[1] > slot2Types[1])
			return 1;
	}
	return CompareSpeciesAlphabetically(dexSlot1, dexSlot2); // Species have same type(s) so sort alphabetically
}

static s8 CompareSpeciesByWeight(struct ListMenuItem *dexSlot1, struct ListMenuItem *dexSlot2)
{
	u16 weight1 = gSpeciesInfo[dexSlot1->index].weight, weight2 = gSpeciesInfo[dexSlot2->index].weight;
	
	if (weight1 < weight2)
		return -1;
	else if (weight1 > weight2)
		return 1;
	
	return CompareSpeciesAlphabetically(dexSlot1, dexSlot2); // Species have same weights so sort alphabetically
}

static s8 CompareSpeciesByHeight(struct ListMenuItem *dexSlot1, struct ListMenuItem *dexSlot2)
{
	u16 height1 = gSpeciesInfo[dexSlot1->index].height, height2 = gSpeciesInfo[dexSlot2->index].height;
	
	if (height1 < height2)
		return -1;
	else if (height1 > height2)
		return 1;
	
	return CompareSpeciesAlphabetically(dexSlot1, dexSlot2); // Species have same heights so sort alphabetically
}

static void DexScreen_InitListMenuForOrderedList(const struct ListMenuTemplate * template, u8 order)
{
    switch (order)
    {
    default:
    case DEX_ORDER_NUMERICAL_KANTO:
        sPokedexScreenData->orderedListMenuTaskId = ListMenuInitInRect(template, sListMenuRects_OrderedList, sPokedexScreenData->kantoOrderMenuCursorPos, sPokedexScreenData->kantoOrderMenuItemsAbove);
        break;
    case DEX_ORDER_ATOZ:
    case DEX_ORDER_TYPE:
    case DEX_ORDER_LIGHTEST:
    case DEX_ORDER_SMALLEST:
        sPokedexScreenData->orderedListMenuTaskId = ListMenuInitInRect(template, sListMenuRects_OrderedList, sPokedexScreenData->characteristicOrderMenuCursorPos, sPokedexScreenData->characteristicOrderMenuItemsAbove);
        break;
    case DEX_ORDER_NUMERICAL_NATIONAL:
        sPokedexScreenData->orderedListMenuTaskId = ListMenuInitInRect(template, sListMenuRects_OrderedList, sPokedexScreenData->nationalOrderMenuCursorPos, sPokedexScreenData->nationalOrderMenuItemsAbove);
        break;
    }
}

static void DexScreen_DestroyDexOrderListMenu(u8 order)
{
    switch (order)
    {
    default:
    case DEX_ORDER_NUMERICAL_KANTO:
        DestroyListMenuTask(sPokedexScreenData->orderedListMenuTaskId, &sPokedexScreenData->kantoOrderMenuCursorPos, &sPokedexScreenData->kantoOrderMenuItemsAbove);
        break;
    case DEX_ORDER_ATOZ:
    case DEX_ORDER_TYPE:
    case DEX_ORDER_LIGHTEST:
    case DEX_ORDER_SMALLEST:
        DestroyListMenuTask(sPokedexScreenData->orderedListMenuTaskId, &sPokedexScreenData->characteristicOrderMenuCursorPos, &sPokedexScreenData->characteristicOrderMenuItemsAbove);
        break;
    case DEX_ORDER_NUMERICAL_NATIONAL:
        DestroyListMenuTask(sPokedexScreenData->orderedListMenuTaskId, &sPokedexScreenData->nationalOrderMenuCursorPos, &sPokedexScreenData->nationalOrderMenuItemsAbove);
        break;
    }
}

static u8 DexScreen_CreateDexOrderScrollArrows(void)
{
    struct ScrollArrowsTemplate template = sDexOrderScrollArrowsTemplate;
	
    if (sPokedexScreenData->orderedDexCount > sListMenuTemplate_OrderedListMenu.maxShowed)
        template.fullyDownThreshold = sPokedexScreenData->orderedDexCount - sListMenuTemplate_OrderedListMenu.maxShowed;
    else
        template.fullyDownThreshold = 0;
	
    return AddScrollIndicatorArrowPair(&template, &sPokedexScreenData->modeSelectCursorPosBak);
}

static void ItemPrintFunc_OrderedListMenu(u8 windowId, u32 species, u8 y)
{
    u8 type1;
	
    DexScreen_PrintMonDexNo(sPokedexScreenData->numericalOrderWindowId, 0, species, 12, y);
	
    if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
    {
        BlitMoveInfoIcon(sPokedexScreenData->numericalOrderWindowId, 0, 0x2A, y);
		
        type1 = gSpeciesInfo[species].types[0];
        BlitMoveInfoIcon(sPokedexScreenData->numericalOrderWindowId, type1 + 1, 0x78, y);
		
        if (type1 != gSpeciesInfo[species].types[1])
            BlitMoveInfoIcon(sPokedexScreenData->numericalOrderWindowId, gSpeciesInfo[species].types[1] + 1, 0x98, y);
    }
}

static void Task_DexScreen_CategorySubmenu(u8 taskId)
{
    u8 pageFlipCmd, *ptr;
	
    switch (sPokedexScreenData->state)
    {
    case 0:
        HideBg(3);
        HideBg(2);
        HideBg(1);
        DexScreen_GetPageLimitsForCategory(sPokedexScreenData->category);
        if (sPokedexScreenData->pageNum < sPokedexScreenData->firstPageInCategory)
            sPokedexScreenData->pageNum = sPokedexScreenData->firstPageInCategory;
        sPokedexScreenData->state = 2;
        break;
    case 1:
        DexScreen_DestroyCategoryPageMonIconAndInfoWindows();
        HideBg(2);
        HideBg(1);
		
        switch (sPokedexScreenData->parentOfCategoryMenu)
        {
        case 0:
        default:
            gTasks[taskId].func = Task_PokedexScreen;
            break;
        case 1:
            gTasks[taskId].func = Task_DexScreen_CharacteristicOrder;
            break;
        }
        sPokedexScreenData->state = 0;
        break;
    case 2:
        DexScreen_CreateCategoryListGfx(FALSE);
        CopyBgTilemapBufferToVram(3);
        CopyBgTilemapBufferToVram(2);
        CopyBgTilemapBufferToVram(1);
        DexScreen_CreateCategoryPageSelectionCursor(0xFF);
        sPokedexScreenData->state = 3;
        break;
    case 3:
        BeginNormalPaletteFade(~0x8000, 0, 16, 0, RGB_WHITEALPHA);
        ShowBg(3);
        ShowBg(2);
        ShowBg(1);
        sPokedexScreenData->state = 4;
        break;
    case 4:
        sPokedexScreenData->scrollArrowsTaskId = DexScreen_CreateCategoryMenuScrollArrows();
        sPokedexScreenData->categoryPageCursorTaskId = ListMenuAddCursorObjectInternal(&sCursorStruct_CategoryPage, 0);
        sPokedexScreenData->state = 5;
        break;
    case 5:
        DexScreen_CreateCategoryPageSelectionCursor(sPokedexScreenData->categoryCursorPosInPage);
        DexScreen_UpdateCategoryPageCursorObject(sPokedexScreenData->categoryPageCursorTaskId, sPokedexScreenData->categoryCursorPosInPage, sPokedexScreenData->numMonsOnPage);
        sPokedexScreenData->modeSelectCursorPosBak = sPokedexScreenData->pageNum;
        pageFlipCmd = 0;
		
        if (JOY_NEW(A_BUTTON) && GetSetPokedexFlag(SpeciesToNationalPokedexNum(sPokedexScreenData->pageSpecies[sPokedexScreenData->categoryCursorPosInPage]), FLAG_GET_SEEN))
        {
            RemoveScrollIndicatorArrowPair(sPokedexScreenData->scrollArrowsTaskId);
            ListMenuRemoveCursorObject(sPokedexScreenData->categoryPageCursorTaskId, 0);
            sPokedexScreenData->state = 12;
            break;
        }
        if (!JOY_HELD(R_BUTTON) && JOY_REPT(DPAD_LEFT))
        {
            if (sPokedexScreenData->categoryCursorPosInPage != 0)
            {
                sPokedexScreenData->categoryCursorPosInPage--;
                PlaySE(SE_SELECT);
                break;
            }
            else
                pageFlipCmd = 1;
        }
        if (!JOY_HELD(R_BUTTON) && JOY_REPT(DPAD_RIGHT))
        {
            if (sPokedexScreenData->categoryCursorPosInPage < sPokedexScreenData->numMonsOnPage - 1)
            {
                sPokedexScreenData->categoryCursorPosInPage++;
                PlaySE(SE_SELECT);
                break;
            }
            else
                pageFlipCmd = 2;
        }
        if (pageFlipCmd == 0)
            pageFlipCmd = DexScreen_InputHandler_GetShoulderInput();
		
        switch (pageFlipCmd)
        {
        case 0: // No action
            break;
        case 1: // Left
            while (sPokedexScreenData->pageNum > sPokedexScreenData->firstPageInCategory)
            {
                sPokedexScreenData->pageNum--;
                if (DexScreen_IsPageUnlocked(sPokedexScreenData->category, sPokedexScreenData->pageNum))
                {
                    sPokedexScreenData->state = 8;
                    break;
                }
            }
            if (sPokedexScreenData->state != 8)
                sPokedexScreenData->state = 6;
            break;
        case 2: // Right
            while (sPokedexScreenData->pageNum < sPokedexScreenData->lastPageInCategory - 1)
            {
                sPokedexScreenData->pageNum++;
                if (DexScreen_IsPageUnlocked(sPokedexScreenData->category, sPokedexScreenData->pageNum))
                {
                    sPokedexScreenData->state = 10;
                    break;
                }
            }
            if (sPokedexScreenData->state != 10)
                sPokedexScreenData->state = 6;
            break;
        }
        if (JOY_NEW(B_BUTTON))
            sPokedexScreenData->state = 6;
        break;
    case 6:
    case 7:
        RemoveScrollIndicatorArrowPair(sPokedexScreenData->scrollArrowsTaskId);
        ListMenuRemoveCursorObject(sPokedexScreenData->categoryPageCursorTaskId, 0);
        BeginNormalPaletteFade(~0x8000, 0, 0, 16, RGB_WHITEALPHA);
        sPokedexScreenData->state = 1;
        break;
    case 8:
    case 10:
        DexScreen_DestroyCategoryPageMonIconAndInfoWindows();
        DexScreen_CreateCategoryPageSelectionCursor(0xFF);
        ListMenuUpdateCursorObject(sPokedexScreenData->categoryPageCursorTaskId, 0, 0xA0, 0);
        sPokedexScreenData->categoryPageSelectionCursorTimer = 0;
        sPokedexScreenData->data[0] = 0;
        sPokedexScreenData->state++;
        break;
    case 9:
        if (DexScreen_FlipCategoryPageInDirection(0))
        {
            sPokedexScreenData->categoryCursorPosInPage = sPokedexScreenData->numMonsOnPage - 1;
            sPokedexScreenData->state = 5;
        }
        break;
    case 11:
        if (DexScreen_FlipCategoryPageInDirection(1))
        {
            sPokedexScreenData->categoryCursorPosInPage = 0;
            sPokedexScreenData->state = 5;
        }
        break;
    case 12:
        sPokedexScreenData->dexSpecies = sPokedexScreenData->pageSpecies[sPokedexScreenData->categoryCursorPosInPage];
        PlaySE(SE_SELECT);
        sPokedexScreenData->state = 14;
        break;
    case 13:
        RemoveDexPageWindows();
        sPokedexScreenData->state = 4;
        break;
    case 14:
	    sPokedexScreenData->forms[1] = SPECIES_NONE; // No forms display
        DexScreen_DrawMonDexPage(FALSE);
        sPokedexScreenData->state = 15;
        break;
    case 15:
        sPokedexScreenData->data[0] = 0;
        sPokedexScreenData->data[1] = 0;
        sPokedexScreenData->state++;
        // fallthrough
    case 16:
        if (sPokedexScreenData->data[1] < 6)
        {
            if (sPokedexScreenData->data[0])
            {
                DexScreen_DexPageZoomEffectFrame(0, sPokedexScreenData->data[1]);
                CopyBgTilemapBufferToVram(0);
                sPokedexScreenData->data[0] = 4;
                sPokedexScreenData->data[1]++;
            }
            else
                sPokedexScreenData->data[0]--;
        }
        else
        {
            FillBgTilemapBufferRect_Palette0(0, 0x000, 0, 2, 30, 16);
            CopyBgTilemapBufferToVram(3);
            CopyBgTilemapBufferToVram(2);
            CopyBgTilemapBufferToVram(1);
            CopyBgTilemapBufferToVram(0);
            PlayCry_NormalNoDucking(sPokedexScreenData->dexSpecies, 0, 125, CRY_PRIORITY_NORMAL);
            sPokedexScreenData->data[0] = 0;
            sPokedexScreenData->state = 17;
        }
        break;
    case 17:
        if (JOY_NEW(A_BUTTON))
        {
            RemoveDexPageWindows();
            FillBgTilemapBufferRect_Palette0(1, 0x000, 0, 2, 30, 16);
            CopyBgTilemapBufferToVram(1);
            sPokedexScreenData->state = 21;
        }
        else if (JOY_NEW(B_BUTTON))
            sPokedexScreenData->state = 18;
		else
			DexScreen_InputHandler_StartToCry();
        break;
    case 18:
        DexScreen_CreateCategoryListGfx(FALSE);
        DexScreen_DexPageZoomEffectFrame(0, 6);
        CopyBgTilemapBufferToVram(3);
        CopyBgTilemapBufferToVram(2);
        CopyBgTilemapBufferToVram(1);
        CopyBgTilemapBufferToVram(0);
        sPokedexScreenData->state = 19;
        break;
    case 19:
        sPokedexScreenData->data[0] = 0;
        sPokedexScreenData->data[1] = 6;
        sPokedexScreenData->state++;
        // fallthrough
    case 20:
        if (sPokedexScreenData->data[1])
        {
            if (sPokedexScreenData->data[0])
            {
                sPokedexScreenData->data[1]--;
                FillBgTilemapBufferRect_Palette0(0, 0x000, 0, 2, 30, 16);
                DexScreen_DexPageZoomEffectFrame(0, sPokedexScreenData->data[1]);
                CopyBgTilemapBufferToVram(0);
                sPokedexScreenData->data[0] = 1;
            }
            else
                sPokedexScreenData->data[0]--;
        }
        else
        {
            FillBgTilemapBufferRect_Palette0(0, 0x000, 0, 2, 30, 16);
            CopyBgTilemapBufferToVram(0);
            sPokedexScreenData->state = 13;
        }
        break;
    case 21:
        DexScreen_DrawMonAreaPage();
        sPokedexScreenData->state = 22;
        break;
    case 22:
        CopyBgTilemapBufferToVram(3);
        CopyBgTilemapBufferToVram(2);
        CopyBgTilemapBufferToVram(1);
        CopyBgTilemapBufferToVram(0);
        sPokedexScreenData->state = 23;
        break;
    case 23:
        if (JOY_NEW(A_BUTTON))
        {
            FillBgTilemapBufferRect_Palette0(2, 0x000, 0, 2, 30, 16);
            FillBgTilemapBufferRect_Palette0(1, 0x000, 0, 2, 30, 16);
            FillBgTilemapBufferRect_Palette0(0, 0x000, 0, 2, 30, 16);
            CopyBgTilemapBufferToVram(2);
            CopyBgTilemapBufferToVram(1);
            CopyBgTilemapBufferToVram(0);
            sPokedexScreenData->state = 26;
        }
        else if (JOY_NEW(B_BUTTON))
        {
            FillBgTilemapBufferRect_Palette0(2, 0x000, 0, 2, 30, 16);
            FillBgTilemapBufferRect_Palette0(1, 0x000, 0, 2, 30, 16);
            FillBgTilemapBufferRect_Palette0(0, 0x000, 0, 2, 30, 16);
            CopyBgTilemapBufferToVram(2);
            CopyBgTilemapBufferToVram(1);
            CopyBgTilemapBufferToVram(0);
            sPokedexScreenData->state = 24;
        }
        else
            DexScreen_InputHandler_StartToCry();
        break;
    case 24:
        DexScreen_DestroyAreaScreenResources();
        sPokedexScreenData->state = 25;
        break;
    case 25:
        DexScreen_DrawMonDexPage(FALSE);
        CopyBgTilemapBufferToVram(3);
        CopyBgTilemapBufferToVram(2);
        CopyBgTilemapBufferToVram(1);
        CopyBgTilemapBufferToVram(0);
        sPokedexScreenData->state = 17;
        break;
    case 26:
        DexScreen_DestroyAreaScreenResources();
        sPokedexScreenData->state = 18;
        break;
    }
}

static u8 DexScreen_CreateCategoryMenuScrollArrows(void)
{
    struct ScrollArrowsTemplate template = sScrollArrowsTemplate_CategoryMenu;
	
    template.fullyUpThreshold = sPokedexScreenData->firstPageInCategory;
    template.fullyDownThreshold = sPokedexScreenData->lastPageInCategory - 1;
	
    sPokedexScreenData->modeSelectCursorPosBak = sPokedexScreenData->pageNum;
    return AddScrollIndicatorArrowPair(&template, &sPokedexScreenData->modeSelectCursorPosBak);
}

static void DexScreen_GetSpeciesForms(u16 species)
{
	u16 i, numForms = 0, natDex = SpeciesToNationalPokedexNum(species);
	
	sPokedexScreenData->forms[numForms++] = species;
	
	if (GetSetPokedexFlag(natDex, FLAG_GET_CAUGHT))
	{
		// Put in the array all species that has the same national dex num as the current species to be shown
		for (i = FORMS_START + 1; i < NUM_SPECIES; i++)
		{
			if (SpeciesToNationalPokedexNum(i) == natDex)
				sPokedexScreenData->forms[numForms++] = i;
		}
	}
	sPokedexScreenData->forms[numForms] = SPECIES_NONE;
}

/*
 * Returns 1 to flip pages left, 2 to flip pages right, 0 for no action
 */
static u8 DexScreen_InputHandler_GetShoulderInput(void)
{
    if (gMain.newKeys & L_BUTTON)
        return 1;
    else if (gMain.newKeys & R_BUTTON)
        return 2;
    else
        return 0;
}

static void Task_DexScreen_ShowMonPage(u8 taskId)
{
    switch (sPokedexScreenData->state)
    {
    case 0:
        HideBg(3);
        HideBg(2);
        HideBg(1);
        sPokedexScreenData->state = 14;
        break;
    case 1:
        HideBg(2);
        HideBg(1);
        gTasks[taskId].func = Task_DexScreen_NumericalOrder;
		sPokedexScreenData->formIndex = 0;
        sPokedexScreenData->state = 0;
        break;
    case 2:
        sPokedexScreenData->numMonsOnPage = 1;
        DexScreen_DrawMonDexPage(FALSE);
        sPokedexScreenData->state = 3;
        break;
    case 3:
        CopyBgTilemapBufferToVram(3);
        CopyBgTilemapBufferToVram(2);
        CopyBgTilemapBufferToVram(1);
        CopyBgTilemapBufferToVram(0);
        PlayCry_NormalNoDucking(sPokedexScreenData->dexSpecies, 0, 125, CRY_PRIORITY_NORMAL);
        sPokedexScreenData->state = 4;
        break;
    case 4:
        BeginNormalPaletteFade(~0x8000, 0, 16, 0, RGB_WHITEALPHA);
        ShowBg(3);
        ShowBg(2);
        ShowBg(1);
        sPokedexScreenData->state = 5;
        break;
    case 5:
        if (JOY_NEW(A_BUTTON))
        {
            RemoveDexPageWindows();
            FillBgTilemapBufferRect_Palette0(1, 0x000, 0, 2, 30, 16);
            CopyBgTilemapBufferToVram(1);
            sPokedexScreenData->state = 7;
        }
        else if (JOY_NEW(B_BUTTON))
        {
            RemoveDexPageWindows();
            BeginNormalPaletteFade(~0x8000, 0, 0, 16, RGB_WHITEALPHA);
            sPokedexScreenData->state = 1;
        }
        else if (JOY_NEW(DPAD_UP) && DexScreen_TryScrollMonsVertical(1))
        {
            RemoveDexPageWindows();
            BeginNormalPaletteFade(~0x8000, 0, 0, 16, RGB_WHITEALPHA);
            sPokedexScreenData->state = 6;
        }
        else if (JOY_NEW(DPAD_DOWN) && DexScreen_TryScrollMonsVertical(0))
        {
            RemoveDexPageWindows();
            BeginNormalPaletteFade(~0x8000, 0, 0, 16, RGB_WHITEALPHA);
            sPokedexScreenData->state = 6;
        }
		else
		{
			// Has forms to shown
			if (sPokedexScreenData->forms[1] != SPECIES_NONE)
			{
				if (JOY_NEW(DPAD_LEFT) && sPokedexScreenData->formIndex > 0)
				{
					--sPokedexScreenData->formIndex;
					RemoveDexPageWindows();
					BeginNormalPaletteFade(~0x8000, 0, 0, 16, RGB_WHITEALPHA);
					sPokedexScreenData->state = 13;
				}
				else if (JOY_NEW(DPAD_RIGHT) && sPokedexScreenData->forms[sPokedexScreenData->formIndex + 1] != SPECIES_NONE)
				{
					++sPokedexScreenData->formIndex;
					RemoveDexPageWindows();
					BeginNormalPaletteFade(~0x8000, 0, 0, 16, RGB_WHITEALPHA);
					sPokedexScreenData->state = 13;
				}
			}
			DexScreen_InputHandler_StartToCry();
		}
        break;
    case 6:
	    HideBg(2);
        HideBg(1);
	    sPokedexScreenData->formIndex = 0;
	    sPokedexScreenData->dexSpecies = sPokedexScreenData->characteristicMenuInput;
		sPokedexScreenData->state = 14;
        break;
    case 7:
        DexScreen_DrawMonAreaPage();
        sPokedexScreenData->state = 8;
        break;
    case 8:
        CopyBgTilemapBufferToVram(3);
        CopyBgTilemapBufferToVram(2);
        CopyBgTilemapBufferToVram(1);
        CopyBgTilemapBufferToVram(0);
        sPokedexScreenData->state = 9;
        break;
    case 9:
        if (JOY_NEW(A_BUTTON))
        {
            BeginNormalPaletteFade(~0x8000, 0, 0, 16, RGB_WHITEALPHA);
            sPokedexScreenData->state = 12;
        }
        else if (JOY_NEW(B_BUTTON))
        {
            FillBgTilemapBufferRect_Palette0(2, 0x000, 0, 2, 30, 16);
            FillBgTilemapBufferRect_Palette0(1, 0x000, 0, 2, 30, 16);
            FillBgTilemapBufferRect_Palette0(0, 0x000, 0, 2, 30, 16);
            CopyBgTilemapBufferToVram(2);
            CopyBgTilemapBufferToVram(1);
            CopyBgTilemapBufferToVram(0);
            sPokedexScreenData->state = 10;
        }
        else
            DexScreen_InputHandler_StartToCry();
        break;
    case 10:
        DexScreen_DestroyAreaScreenResources();
        sPokedexScreenData->state = 11;
        break;
    case 11:
        DexScreen_DrawMonDexPage(FALSE);
        CopyBgTilemapBufferToVram(3);
        CopyBgTilemapBufferToVram(2);
        CopyBgTilemapBufferToVram(1);
        CopyBgTilemapBufferToVram(0);
        sPokedexScreenData->state = 5;
        break;
    case 12:
        DexScreen_DestroyAreaScreenResources();
        FillBgTilemapBufferRect_Palette0(0, 0x000, 0, 2, 30, 16);
        CopyBgTilemapBufferToVram(0);
        sPokedexScreenData->state = 1;
        break;
	case 13:
	    HideBg(2);
        HideBg(1);
		sPokedexScreenData->dexSpecies = sPokedexScreenData->forms[sPokedexScreenData->formIndex];
        sPokedexScreenData->state = 2;
        break;
	case 14:
	    DexScreen_GetSpeciesForms(sPokedexScreenData->dexSpecies);
		sPokedexScreenData->state = 2;
        break;
    }
}

static bool32 DexScreen_TryScrollMonsVertical(u8 direction)
{
    int selectedIndex;
    u16 *itemsAbove_p, *cursorPos_p;

    switch (sPokedexScreenData->dexOrderId)
    {
    default:
    case DEX_ORDER_NUMERICAL_KANTO:
        cursorPos_p = &sPokedexScreenData->kantoOrderMenuCursorPos;
        itemsAbove_p = &sPokedexScreenData->kantoOrderMenuItemsAbove;
        break;
    case DEX_ORDER_ATOZ:
    case DEX_ORDER_TYPE:
    case DEX_ORDER_LIGHTEST:
    case DEX_ORDER_SMALLEST:
        cursorPos_p = &sPokedexScreenData->characteristicOrderMenuCursorPos;
        itemsAbove_p = &sPokedexScreenData->characteristicOrderMenuItemsAbove;
        break;
    case DEX_ORDER_NUMERICAL_NATIONAL:
        cursorPos_p = &sPokedexScreenData->nationalOrderMenuCursorPos;
        itemsAbove_p = &sPokedexScreenData->nationalOrderMenuItemsAbove;
        break;
    }
    selectedIndex = *cursorPos_p + *itemsAbove_p;
	
    if (direction) // Seek up
    {
        if (selectedIndex == 0)
            return FALSE;
		
		--selectedIndex;
        while (selectedIndex >= 0)
        {
            if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(sPokedexScreenData->listItems[selectedIndex].index), FLAG_GET_SEEN))
                break;
            selectedIndex--;
        }
        if (selectedIndex < 0)
            return FALSE;
    }
    else // Seek down
    {
        if (selectedIndex == sPokedexScreenData->orderedDexCount - 1)
            return FALSE;

        ++selectedIndex;
        while (selectedIndex < sPokedexScreenData->orderedDexCount) //Should be while (++selectedIndex < sPokedexScreenData->orderedDexCount) without the selectedIndex++ in the body or before the while at all, but this is needed to match.
        {
            if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(sPokedexScreenData->listItems[selectedIndex].index), FLAG_GET_SEEN))
                break;
            selectedIndex++;
        }
        if (selectedIndex >= sPokedexScreenData->orderedDexCount)
            return FALSE;
    }
    sPokedexScreenData->characteristicMenuInput = sPokedexScreenData->listItems[selectedIndex].index;

    if (sPokedexScreenData->orderedDexCount > 9)
    {
        if (selectedIndex < 4)
        {
            *cursorPos_p = 0;
            *itemsAbove_p = selectedIndex;
        }
        else if (selectedIndex >= (sPokedexScreenData->orderedDexCount - 4))
        {
            *cursorPos_p = (sPokedexScreenData->orderedDexCount - 9);
            *itemsAbove_p = selectedIndex + 9 - (sPokedexScreenData->orderedDexCount);
        }
        else
        {
            *cursorPos_p = selectedIndex - 4;
            *itemsAbove_p = 4;
        }
    }
    else
    {
        *cursorPos_p = 0;
        *itemsAbove_p = selectedIndex;
    }
    return TRUE;
}

static void DexScreen_RemoveWindow(u8 *windowId_p)
{
    if (*windowId_p != 0xFF)
    {
        RemoveWindow(*windowId_p);
        *windowId_p = 0xFF;
    }
}

static void DexScreen_AddTextPrinterParameterized(u8 windowId, u8 fontId, const u8 *str, u8 x, u8 y, u8 colorIdx)
{
    u8 textColor[3];
	
    switch (colorIdx)
    {
    case 0:
        textColor[0] = 0;
        textColor[1] = 1;
        textColor[2] = 3;
        break;
    case 1:
        textColor[0] = 0;
        textColor[1] = 5;
        textColor[2] = 1;
        break;
    case 2:
        textColor[0] = 0;
        textColor[1] = 15;
        textColor[2] = 14;
        break;
    case 3:
        textColor[0] = 0;
        textColor[1] = 11;
        textColor[2] = 1;
        break;
    case 4:
        textColor[0] = 0;
        textColor[1] = 1;
        textColor[2] = 2;
        break;
    }
    AddTextPrinterParameterized4(windowId, fontId, x, y, fontId == 0 ? 0 : 1, 0, textColor, -1, str);
}

void DexScreen_PrintNum4LeadingZeroes(u8 windowId, u8 fontId, u16 num, u8 x, u8 y, u8 colorIdx)
{
    u8 buff[5];
    buff[0] = (num / 1000) + CHAR_0;
    buff[1] = ((num %= 1000) / 100) + CHAR_0;
    buff[2] = ((num %= 100) / 10) + CHAR_0;
    buff[3] = (num % 10) + CHAR_0;
    buff[4] = EOS;
    DexScreen_AddTextPrinterParameterized(windowId, fontId, buff, x, y, colorIdx);
}

static void DexScreen_PrintNum3RightAlign(u8 windowId, u8 fontId, u16 num, u8 x, u8 y, u8 colorIdx)
{
    u8 i, buff[4];
	
    buff[0] = (num / 100) + CHAR_0;
    buff[1] = ((num %= 100) / 10) + CHAR_0;
    buff[2] = (num % 10) + CHAR_0;
    buff[3] = EOS;
	
    for (i = 0; i < 3; i++)
    {
        if (buff[i] != CHAR_0)
            break;
        buff[i] = CHAR_SPACE;
    }
    DexScreen_AddTextPrinterParameterized(windowId, fontId, buff, x, y, colorIdx);
}

static void DexScreen_PrintNum4RightAlign(u8 windowId, u8 fontId, u16 num, u8 x, u8 y, u8 colorIdx)
{
    u8 i, buff[5];

    buff[0] = (num / 1000) + CHAR_0;
    buff[1] = ((num %= 1000) / 100) + CHAR_0;
    buff[2] = ((num %= 100) / 10) + CHAR_0;
    buff[3] = (num % 10) + CHAR_0;
    buff[4] = EOS;
	
    for (i = 0; i < 4; i++)
    {
        if (buff[i] != CHAR_0)
            break;
        buff[i] = CHAR_SPACE;
    }
    DexScreen_AddTextPrinterParameterized(windowId, fontId, buff, x, y, colorIdx);
}

static u32 DexScreen_GetDefaultPersonality(u16 species)
{
    switch (species)
    {
    case SPECIES_SPINDA:
        return gSaveBlock2Ptr->pokedex.spindaPersonality;
    default:
        return 0;
    }
}

static void DexScreen_LoadMonPicInWindow(u8 windowId, u16 species, u16 paletteOffset)
{
    LoadMonPicInWindow(species, FALSE, DexScreen_GetDefaultPersonality(species), TRUE, paletteOffset >> 4, windowId);
}

static void DexScreen_PrintMonDexNo(u8 windowId, u8 fontId, u16 species, u8 x, u8 y)
{
    DexScreen_AddTextPrinterParameterized(windowId, fontId, gText_PokedexNo, x, y, 0);
    DexScreen_PrintNum4LeadingZeroes(windowId, fontId, SpeciesToNationalPokedexNum(species), x + 9, y, 0);
}

static u16 DexScreen_GetDexCount(u8 caseId, u8 dexMode)
{
	return dexMode == DEX_MODE_KANTO ? GetRegionPokedexCount(REGION_KANTO, caseId) : GetNationalPokedexCount(caseId);
}

static void DexScreen_PrintControlInfo(const u8 *src)
{
    DexScreen_AddTextPrinterParameterized(1, 0, src, 236 - GetStringWidth(0, src, 0), 2, 4);
}

static bool8 DexScreen_DrawMonPicInCategoryPage(u16 species, u8 slot, u8 numSlots)
{
    struct WindowTemplate template;
	
    numSlots--;
	
    CopyToBgTilemapBufferRect_ChangePalette(3, sCategoryPageIconWindowBg, sCategoryPageIconCoords[numSlots][slot][0], sCategoryPageIconCoords[numSlots][slot][1], 8, 8, slot + 5);
    
	if (sPokedexScreenData->categoryMonWindowIds[slot] == 0xFF)
    {
        template = sWindowTemplate_CategoryMonIcon;
        template.tilemapLeft = sCategoryPageIconCoords[numSlots][slot][0];
        template.tilemapTop = sCategoryPageIconCoords[numSlots][slot][1];
        template.paletteNum = slot + 1;
        template.baseBlock = slot * 64 + 8;
		
        sPokedexScreenData->categoryMonWindowIds[slot] = AddWindow(&template);
        FillWindowPixelBuffer(sPokedexScreenData->categoryMonWindowIds[slot], PIXEL_FILL(0));
        DexScreen_LoadMonPicInWindow(sPokedexScreenData->categoryMonWindowIds[slot], species, slot * 16 + 16);
        PutWindowTilemap(sPokedexScreenData->categoryMonWindowIds[slot]);
        CopyWindowToVram(sPokedexScreenData->categoryMonWindowIds[slot], COPYWIN_GFX);
    }
    else
        PutWindowTilemap(sPokedexScreenData->categoryMonWindowIds[slot]);

    if (sPokedexScreenData->categoryMonInfoWindowIds[slot] == 0xFF)
    {
        if (species != SPECIES_NONE)
        {
            template = sWindowTemplate_CategoryMonInfo;
            template.tilemapLeft = sCategoryPageIconCoords[numSlots][slot][2];
            template.tilemapTop = sCategoryPageIconCoords[numSlots][slot][3];
            template.baseBlock = slot * 40 + 0x108;
			
            sPokedexScreenData->categoryMonInfoWindowIds[slot] = AddWindow(&template);
            CopyToWindowPixelBuffer(sPokedexScreenData->categoryMonInfoWindowIds[slot], sCategoryMonInfoBgTiles, 0, 0);
            DexScreen_PrintMonDexNo(sPokedexScreenData->categoryMonInfoWindowIds[slot], 0, species, 12, 0);
            DexScreen_AddTextPrinterParameterized(sPokedexScreenData->categoryMonInfoWindowIds[slot], 2, gSpeciesInfo[species].name, 2, 13, 0);
			
            if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
                BlitBitmapRectToWindow(sPokedexScreenData->categoryMonInfoWindowIds[slot], sDexScreen_CaughtIcon, 0, 0, 8, 8, 2, 3, 8, 8);
			
            PutWindowTilemap(sPokedexScreenData->categoryMonInfoWindowIds[slot]);
            CopyWindowToVram(sPokedexScreenData->categoryMonInfoWindowIds[slot], COPYWIN_GFX);
        }
    }
    else
        PutWindowTilemap(sPokedexScreenData->categoryMonInfoWindowIds[slot]);

    return TRUE;
}

static void DexScreen_DestroyCategoryPageMonIconAndInfoWindows(void)
{
    u8 i;
	
    for (i = 0; i < 4; i++)
    {
        DexScreen_RemoveWindow(&sPokedexScreenData->categoryMonWindowIds[i]);
        DexScreen_RemoveWindow(&sPokedexScreenData->categoryMonInfoWindowIds[i]);
    }
}

static void DexScreen_PrintCategoryPageNumbers(u8 windowId, u16 currentPage, u16 totalPages, u16 x, u16 y)
{
    u8 buffer[30];
    u8 *ptr = StringCopy(buffer, gText_Page);
	
    ptr = ConvertIntToDecimalStringN(ptr, currentPage, STR_CONV_MODE_RIGHT_ALIGN, 2);
    *ptr++ = CHAR_SLASH;
    ptr = ConvertIntToDecimalStringN(ptr, totalPages, STR_CONV_MODE_RIGHT_ALIGN, 2);
	
    DexScreen_PrintStringWithAlignment(buffer, TEXT_RIGHT);
}

static bool8 DexScreen_CreateCategoryListGfx(bool8 justRegistered)
{
    FillBgTilemapBufferRect_Palette0(3, 2, 0, 0, 30, 20);
    FillBgTilemapBufferRect_Palette0(2, 0, 0, 0, 32, 20);
    FillBgTilemapBufferRect_Palette0(1, 0, 0, 0, 32, 20);
	
    DexScreen_CreateCategoryPageSpeciesList(sPokedexScreenData->category, sPokedexScreenData->pageNum);
    FillWindowPixelBuffer(0, PIXEL_FILL(15));
	
    if (justRegistered)
        DexScreen_PrintStringWithAlignment(sDexCategoryNamePtrs[sPokedexScreenData->category], TEXT_CENTER);
    else
    {
        DexScreen_PrintStringWithAlignment(sDexCategoryNamePtrs[sPokedexScreenData->category], TEXT_LEFT);
        DexScreen_PrintCategoryPageNumbers(0, DexScreen_PageNumberToRenderablePages(sPokedexScreenData->pageNum), DexScreen_PageNumberToRenderablePages(sPokedexScreenData->lastPageInCategory - 1), 160, 2);
    }
    CopyWindowToVram(0, COPYWIN_GFX);
    FillWindowPixelBuffer(1, PIXEL_FILL(15));
	
    if (!justRegistered)
        DexScreen_PrintControlInfo(gText_PickFlipPageCheckCancel);
	
    CopyWindowToVram(1, COPYWIN_GFX);
	
    if (sPokedexScreenData->pageSpecies[0] != 0xFFFF)
        DexScreen_DrawMonPicInCategoryPage(sPokedexScreenData->pageSpecies[0], 0, sPokedexScreenData->numMonsOnPage);
	
    if (sPokedexScreenData->pageSpecies[1] != 0xFFFF)
        DexScreen_DrawMonPicInCategoryPage(sPokedexScreenData->pageSpecies[1], 1, sPokedexScreenData->numMonsOnPage);
	
    if (sPokedexScreenData->pageSpecies[2] != 0xFFFF)
        DexScreen_DrawMonPicInCategoryPage(sPokedexScreenData->pageSpecies[2], 2, sPokedexScreenData->numMonsOnPage);
	
    if (sPokedexScreenData->pageSpecies[3] != 0xFFFF)
        DexScreen_DrawMonPicInCategoryPage(sPokedexScreenData->pageSpecies[3], 3, sPokedexScreenData->numMonsOnPage);
	
    return FALSE;
}

static void DexScreen_CreateCategoryPageSelectionCursor(u8 cursorPos)
{
    u8 i;
    u32 palIdx;

    if (cursorPos == 0xFF)
    {
        for (i = 0; i < 4; i++)
        {
            LoadPalette(&sDexScreen_CategoryCursorPals[0], 0x52 + 0x10 * i, 2);
            LoadPalette(&sDexScreen_CategoryCursorPals[1], 0x58 + 0x10 * i, 2);
        }
        LoadPalette(&sDexScreen_CategoryCursorPals[0], 0x141, 2);
        sPokedexScreenData->categoryPageSelectionCursorTimer = 0;
    }
    else
    {
        if (++sPokedexScreenData->categoryPageSelectionCursorTimer == 16)
            sPokedexScreenData->categoryPageSelectionCursorTimer = 0;
		
        palIdx = sPokedexScreenData->categoryPageSelectionCursorTimer >> 2;
		
        for (i = 0; i < 4; i++)
        {
            if (i == cursorPos)
            {
                LoadPalette(&sDexScreen_CategoryCursorPals[2 * palIdx + 2], 0x52 + 0x10 * i, 2);
                LoadPalette(&sDexScreen_CategoryCursorPals[2 * palIdx + 3], 0x58 + 0x10 * i, 2);
            }
            else
            {
                LoadPalette(&sDexScreen_CategoryCursorPals[0], 0x52 + 0x10 * i, 2);
                LoadPalette(&sDexScreen_CategoryCursorPals[1], 0x58 + 0x10 * i, 2);
            }
        }
        LoadPalette(&sDexScreen_CategoryCursorPals[2 * palIdx + 2], 0x141, 2);
    }
}

static void DexScreen_UpdateCategoryPageCursorObject(u8 taskId, u8 cursorPos, u8 numMonsInPage)
{
    numMonsInPage--;
    ListMenuUpdateCursorObject(taskId, sCategoryPageIconCoords[numMonsInPage][cursorPos][2] * 8, sCategoryPageIconCoords[numMonsInPage][cursorPos][3] * 8, 0);
}

static void DexPage_TileBuffer_CopyCol(const u16 *srcBuf, u8 srcCol, u16 *dstBuf, u8 dstCol)
{
    u8 i;
    const u16 *src = &srcBuf[srcCol];
    u16 *dst = &dstBuf[dstCol];
	
    for (i = 0; i < 20; i++)
    {
        *dst = *src;
        dst += 32;
        src += 32;
    }
}

static void DexPage_TileBuffer_FillCol(u16 tileNo, u16 *tileBuf, u8 x)
{
    u8 i;
    u16 *dst = &tileBuf[x];
	
    for (i = 0; i < 20; i++)
    {
        *dst = tileNo;
        dst += 32;
    }
}

static void DexScreen_TurnCategoryPage_BgEffect(u8 page)
{
    int dstCol;
    int srcCol;
    u16 *bg1buff = GetBgTilemapBuffer(1);
    u16 *bg2buff = GetBgTilemapBuffer(2);
    u16 *bg3buff = GetBgTilemapBuffer(3);
    u16 *bg1mem = sPokedexScreenData->bgBufsMem + 0x800;
    u16 *bg2mem = sPokedexScreenData->bgBufsMem + 0x400;
    u16 *bg3mem = sPokedexScreenData->bgBufsMem + 0x000;
	
    for (dstCol = 0; dstCol < 30; dstCol++)
    {
        srcCol = sDexScreenPageTurnColumns[page][dstCol];
		
        if (srcCol == 30)
        {
            DexPage_TileBuffer_FillCol(0x000, bg1buff, dstCol);
            DexPage_TileBuffer_FillCol(0x000, bg2buff, dstCol);
            DexPage_TileBuffer_FillCol(0x00C, bg3buff, dstCol);
        }
        else
        {
            DexPage_TileBuffer_CopyCol(bg1mem, srcCol, bg1buff, dstCol);
            DexPage_TileBuffer_CopyCol(bg2mem, srcCol, bg2buff, dstCol);
            DexPage_TileBuffer_CopyCol(bg3mem, srcCol, bg3buff, dstCol);
        }
    }
    CopyBgTilemapBufferToVram(1);
    CopyBgTilemapBufferToVram(2);
    CopyBgTilemapBufferToVram(3);
}

/*
 * Direction = 0: Left; 1: Right
 */
static bool8 DexScreen_FlipCategoryPageInDirection(u8 direction)
{
    u16 color = sNationalDexPalette[7];
	
    switch (sPokedexScreenData->data[0])
    {
    case 0:
        sPokedexScreenData->bgBufsMem = Alloc(3 * BG_SCREEN_SIZE);
		sPokedexScreenData->data[0] = direction == 0 ? 2 : 6;
        break;
    case 1:
        Free(sPokedexScreenData->bgBufsMem);
        return TRUE;
        // Go left
    case 2:
        BeginNormalPaletteFade(0x00007FFF, 0, 0, 16, color);
        sPokedexScreenData->data[0]++;
        break;
    case 3:
        FillBgTilemapBufferRect_Palette0(3, 0x00C, 0, 0, 30, 20);
        FillBgTilemapBufferRect_Palette0(2, 0x000, 0, 0, 32, 20);
        FillBgTilemapBufferRect_Palette0(1, 0x000, 0, 0, 32, 20);
        CopyBgTilemapBufferToVram(1);
        CopyBgTilemapBufferToVram(2);
        CopyBgTilemapBufferToVram(3);
        sPokedexScreenData->data[0]++;
        break;
    case 4:
        BeginNormalPaletteFade(0x00007FFF, 0, 0, 0, color);
        DexScreen_CreateCategoryListGfx(FALSE);
        CpuFastCopy(GetBgTilemapBuffer(3), &sPokedexScreenData->bgBufsMem[0 * BG_SCREEN_SIZE / 2], BG_SCREEN_SIZE);
        CpuFastCopy(GetBgTilemapBuffer(2), &sPokedexScreenData->bgBufsMem[1 * BG_SCREEN_SIZE / 2], BG_SCREEN_SIZE);
        CpuFastCopy(GetBgTilemapBuffer(1), &sPokedexScreenData->bgBufsMem[2 * BG_SCREEN_SIZE / 2], BG_SCREEN_SIZE);
        FillBgTilemapBufferRect_Palette0(3, 0x00C, 0, 0, 30, 20);
        FillBgTilemapBufferRect_Palette0(2, 0x000, 0, 0, 32, 20);
        FillBgTilemapBufferRect_Palette0(1, 0x000, 0, 0, 32, 20);

        sPokedexScreenData->data[1] = 0;
        sPokedexScreenData->data[0]++;
        PlaySE(SE_BALL_TRAY_ENTER);
        break;
    case 5:
        if (sPokedexScreenData->data[1] < 10)
        {
            DexScreen_TurnCategoryPage_BgEffect(sPokedexScreenData->data[1]);
            sPokedexScreenData->data[1]++;
        }
        else
            sPokedexScreenData->data[0] = 1;
        break;
        // Go right
    case 6:
        CpuFastCopy(GetBgTilemapBuffer(3), &sPokedexScreenData->bgBufsMem[0 * BG_SCREEN_SIZE / 2], BG_SCREEN_SIZE);
        CpuFastCopy(GetBgTilemapBuffer(2), &sPokedexScreenData->bgBufsMem[1 * BG_SCREEN_SIZE / 2], BG_SCREEN_SIZE);
        CpuFastCopy(GetBgTilemapBuffer(1), &sPokedexScreenData->bgBufsMem[2 * BG_SCREEN_SIZE / 2], BG_SCREEN_SIZE);

        sPokedexScreenData->data[1] = 9;
        sPokedexScreenData->data[0]++;
        PlaySE(SE_BALL_TRAY_ENTER);
        break;
    case 7:
        if (sPokedexScreenData->data[1] != 0)
        {
            DexScreen_TurnCategoryPage_BgEffect(sPokedexScreenData->data[1]);
            sPokedexScreenData->data[1]--;
        }
        else
        {
            DexScreen_TurnCategoryPage_BgEffect(0);
            BeginNormalPaletteFade(0x00007FFF, 0, 16, 16, color);
            sPokedexScreenData->data[0]++;
        }
        break;
    case 8:
        gPaletteFade.bufferTransferDisabled = TRUE;
        DexScreen_CreateCategoryListGfx(FALSE);
        CopyBgTilemapBufferToVram(1);
        CopyBgTilemapBufferToVram(2);
        CopyBgTilemapBufferToVram(3);
        sPokedexScreenData->data[0]++;
        break;
    case 9:
        gPaletteFade.bufferTransferDisabled = FALSE;
        BeginNormalPaletteFade(0x00007FFF, 0, 16, 0, color);
        sPokedexScreenData->data[0] = 1;
        break;
    }
    return FALSE;
}

// Scale from 0 to 6
static void DexScreen_DexPageZoomEffectFrame(u8 bg, u8 scale)
{
    u8 tileLeft, tileTop, width, height;
    s16 left, top, divY;

    if (!sPokedexScreenData->numMonsOnPage)
    {
        tileLeft = sCategoryPageIconCoords[0][0][2];
        tileTop = sCategoryPageIconCoords[0][0][3];
    }
    else
    {
        tileLeft = sCategoryPageIconCoords[sPokedexScreenData->numMonsOnPage - 1][sPokedexScreenData->categoryCursorPosInPage][2];
        tileTop = sCategoryPageIconCoords[sPokedexScreenData->numMonsOnPage - 1][sPokedexScreenData->categoryCursorPosInPage][3];
    }

    width = 6 + (scale * 4);
    height = 3 + (scale * 2);
    if (width >= 28) // Make sure it's not wider than the screen
        width = 28;
    if (height >= 14) // Make sure it's not taller than the screen
        height = 14;

    left = tileLeft - ((scale * 4) / 2);
    top = tileTop - ((scale * 2) / 2);
    if ((left + width + 2) >= 30) // Don't wrap right...
        left -= ((left + width + 2) - 30);
    else if (left < 0) // ... left ...
        left = 0;
    if ((top + height + 2) >= 18) // ... down ...
        top -= ((top + height + 2) - 18);
    else if (top < 2) // or up.
        top = 2;

    divY = (top + 1) + ((height / 2) + 1); // The horizontal divider

    // Top edge
    FillBgTilemapBufferRect_Palette0(bg, 4, left, top, 1, 1);
    FillBgTilemapBufferRect_Palette0(bg, 5, left + 1, top, width, 1);
    FillBgTilemapBufferRect_Palette0(bg, BG_TILE_H_FLIP(4), left + 1 + width, top, 1, 1);

    // Bottom edge
    FillBgTilemapBufferRect_Palette0(bg, 10, left, top + 1 + height, 1, 1);
    FillBgTilemapBufferRect_Palette0(bg, 11, left + 1, top + 1 + height, width, 1);
    FillBgTilemapBufferRect_Palette0(bg, BG_TILE_H_FLIP(10), left + 1 + width, top + 1 + height, 1, 1);

    // Left edge
    FillBgTilemapBufferRect_Palette0(bg, 6, left, top + 1, 1, divY - top - 1);
    FillBgTilemapBufferRect_Palette0(bg, 7, left, divY, 1, 1);
    FillBgTilemapBufferRect_Palette0(bg, 9, left, divY + 1, 1, top + height - divY);

    // Right edge
    FillBgTilemapBufferRect_Palette0(bg, BG_TILE_H_FLIP(6), left + 1 + width, top + 1, 1, divY - top - 1);
    FillBgTilemapBufferRect_Palette0(bg, BG_TILE_H_FLIP(7), left + 1 + width, divY, 1, 1);
    FillBgTilemapBufferRect_Palette0(bg, BG_TILE_H_FLIP(9), left + 1 + width, divY + 1, 1, top + height - divY);

    // Interior
    FillBgTilemapBufferRect_Palette0(bg, 1, left + 1, top + 1, width, divY - top - 1);
    FillBgTilemapBufferRect_Palette0(bg, 8, left + 1, divY, width, 1);
    FillBgTilemapBufferRect_Palette0(bg, 2, left + 1, divY + 1, width, top + height - divY);
}

static void DexScreen_PrintMonCategory(u8 windowId, u16 species, u8 x, u8 y)
{
    u8 categoryStr[DEX_CATEGORY_NAME_LENGTH + 1];
	
	if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
		StringCopy(categoryStr, gSpeciesInfo[species].categoryName);
	else
		StringFill(categoryStr, CHAR_QUESTION_MARK, DEX_CATEGORY_NAME_LENGTH);
	
    DexScreen_AddTextPrinterParameterized(windowId, 0, categoryStr, x, y, 0);
    x += GetStringWidth(0, categoryStr, 0);
    DexScreen_AddTextPrinterParameterized(windowId, 0, gText_PokedexPokemon, x, y, 0);
}

static void DexScreen_PrintMonHeight(u8 windowId, u16 species, u8 x, u8 y)
{
    u16 height = gSpeciesInfo[species].height;
	u8 *labelText;
	u8 i, text[19];
	
	labelText = StringCopy(text, gText_HT);
	*labelText++ = EXT_CTRL_CODE_BEGIN;
	*labelText++ = EXT_CTRL_CODE_MIN_LETTER_SPACING;
	*labelText++ = 5;
	for (i = 0; i < 6; i++)
		*labelText++ = CHAR_SPACE;
	
	if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
    {
		bool8 isGigantamax = (gSpeciesInfo[species].flags & SPECIES_FLAG_GIGANTAMAX);
		
		if (isGigantamax)
			--labelText;
		
		labelText = ConvertIntToDecimalStringN(labelText, height / 10, STR_CONV_MODE_RIGHT_ALIGN, 2);
		*labelText++ = CHAR_PERIOD;
		*labelText++ = height % 10 + CHAR_0;
		
		if (isGigantamax)
			*labelText++ = CHAR_PLUS;
	}
	else
	{
		*labelText++ = CHAR_QUESTION_MARK;
		*labelText++ = CHAR_QUESTION_MARK;
		*labelText++ = CHAR_PERIOD;
		*labelText++ = CHAR_QUESTION_MARK;
	}
	*labelText++ = CHAR_SPACE;
	*labelText++ = CHAR_m;
	*labelText++ = EOS;
	
    DexScreen_AddTextPrinterParameterized(windowId, 0, text, x, y, 0);
}

static void DexScreen_PrintMonWeight(u8 windowId, u16 species, u8 x, u8 y)
{
    u16 weight = gSpeciesInfo[species].weight;
    u8 *labelText;
    u8 i, text[19];

    labelText = StringCopy(text, gText_WT);
    *labelText++ = EXT_CTRL_CODE_BEGIN;
    *labelText++ = EXT_CTRL_CODE_MIN_LETTER_SPACING;
    *labelText++ = 5;
	for (i = 0; i < 4; i++)
		*labelText++ = CHAR_SPACE;

    if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT) && weight)
    {
        labelText = ConvertIntToDecimalStringN(labelText, weight / 10, STR_CONV_MODE_RIGHT_ALIGN, 4);
		*labelText++ = CHAR_PERIOD;
		*labelText++ = weight % 10 + CHAR_0;
    }
    else
    {
		for (i = 0; i < 4; i++)
			*labelText++ = CHAR_QUESTION_MARK;
		*labelText++ = CHAR_PERIOD;
		*labelText++ = CHAR_QUESTION_MARK;
    }
    *labelText++ = CHAR_SPACE;
    *labelText++ = CHAR_K;
	*labelText++ = CHAR_g;
	*labelText++ = EOS;
	
    DexScreen_AddTextPrinterParameterized(windowId, 0, text, x, y, 0);
}

static void DexScreen_PrintMonFlavorText(u8 windowId, u16 species, u8 x, u8 y)
{
    struct TextPrinterTemplate printerTemplate;
    u16 length;
    s32 xCenter;

    if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
    {
        printerTemplate.currentChar = gSpeciesInfo[species].description;
        printerTemplate.windowId = windowId;
        printerTemplate.fontId = 2;
        printerTemplate.letterSpacing = 1;
        printerTemplate.lineSpacing = 0;
        printerTemplate.unk = 0;
        printerTemplate.fgColor = 1;
        printerTemplate.bgColor = 0;
        printerTemplate.shadowColor = 2;

        length = GetStringWidth(2, gSpeciesInfo[species].description, 0);
        xCenter = x + (240 - length) / 2;

        if (xCenter > 0)
            x = xCenter;
        else
            x = 0;

        printerTemplate.x = x;
        printerTemplate.y = y;
        printerTemplate.currentX = x;
        printerTemplate.currentY = y;

        AddTextPrinter(&printerTemplate, TEXT_SPEED_FF, NULL);
    }
}

static void DexScreen_DrawMonDexPage(bool8 justRegistered)
{
    DexScreen_DexPageZoomEffectFrame(3, 6);
    FillBgTilemapBufferRect_Palette0(2, 0, 0, 0, 30, 20);
    FillBgTilemapBufferRect_Palette0(1, 0, 0, 0, 30, 20);
    FillBgTilemapBufferRect_Palette0(0, 0, 0, 2, 30, 16);

    sPokedexScreenData->windowIds[0] = AddWindow(&sWindowTemplate_DexEntry_MonPic);
    sPokedexScreenData->windowIds[1] = AddWindow(&sWindowTemplate_DexEntry_SpeciesStats);
    sPokedexScreenData->windowIds[2] = AddWindow(&sWindowTemplate_DexEntry_FlavorText);

    // Mon pic
    FillWindowPixelBuffer(sPokedexScreenData->windowIds[0], PIXEL_FILL(0));
    DexScreen_LoadMonPicInWindow(sPokedexScreenData->windowIds[0], sPokedexScreenData->dexSpecies, 144);
    PutWindowTilemap(sPokedexScreenData->windowIds[0]);
    CopyWindowToVram(sPokedexScreenData->windowIds[0], COPYWIN_GFX);

    // Species stats
    FillWindowPixelBuffer(sPokedexScreenData->windowIds[1], PIXEL_FILL(0));
    DexScreen_PrintMonDexNo(sPokedexScreenData->windowIds[1], 0, sPokedexScreenData->dexSpecies, 0, 8);
    DexScreen_AddTextPrinterParameterized(sPokedexScreenData->windowIds[1], 2, gSpeciesInfo[sPokedexScreenData->dexSpecies].name, 33, 8, 0);
    DexScreen_PrintMonCategory(sPokedexScreenData->windowIds[1], sPokedexScreenData->dexSpecies, 0, 24);
    DexScreen_PrintMonHeight(sPokedexScreenData->windowIds[1], sPokedexScreenData->dexSpecies, 0, 36);
    DexScreen_PrintMonWeight(sPokedexScreenData->windowIds[1], sPokedexScreenData->dexSpecies, 0, 48);
	
	if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(sPokedexScreenData->dexSpecies), FLAG_GET_CAUGHT))
		DrawSpeciesFootprint(sPokedexScreenData->windowIds[1], sPokedexScreenData->dexSpecies, 88, 40);
	
    PutWindowTilemap(sPokedexScreenData->windowIds[1]);
    CopyWindowToVram(sPokedexScreenData->windowIds[1], COPYWIN_GFX);

    // Dex entry
    FillWindowPixelBuffer(sPokedexScreenData->windowIds[2], PIXEL_FILL(0));
    DexScreen_PrintMonFlavorText(sPokedexScreenData->windowIds[2], sPokedexScreenData->dexSpecies, 0, 8);
    PutWindowTilemap(sPokedexScreenData->windowIds[2]);
    CopyWindowToVram(sPokedexScreenData->windowIds[2], COPYWIN_GFX);

    // Control info
    FillWindowPixelBuffer(1, PIXEL_FILL(15));
	
    if (justRegistered == FALSE)
    {
        DexScreen_AddTextPrinterParameterized(1, 0, gText_Cry, 8, 2, 4);
        DexScreen_PrintControlInfo(sPokedexScreenData->forms[1] == SPECIES_NONE ? gText_NextDataCancel : gText_FormsNextDataCancel);
    }
    else
        // Just registered
        DexScreen_PrintControlInfo(gText_Next);

    PutWindowTilemap(1);
    CopyWindowToVram(1, COPYWIN_GFX);
}

static void RemoveDexPageWindows(void)
{
    DexScreen_RemoveWindow(&sPokedexScreenData->windowIds[0]);
    DexScreen_RemoveWindow(&sPokedexScreenData->windowIds[1]);
    DexScreen_RemoveWindow(&sPokedexScreenData->windowIds[2]);
}

static const u8 *const sStatsTextPointers[] =
{
    gText_Hp,
    gText_Atk,
    gText_Def,
    gText_Speed,
    gText_SpAtk,
    gText_SpDef,
};

enum
{
	STAT_PRINT_HP,
	STAT_PRINT_ATK,
	STAT_PRINT_DEF,
	STAT_PRINT_SPEED,
	STAT_PRINT_SPATK,
	STAT_PRINT_SPDEF,
};

static void PrepareStatToPrint(u16 species, u8 statId)
{
    switch (statId)
    {
    case STAT_PRINT_HP:
            ConvertIntToDecimalStringN(gStringVar1, gSpeciesInfo[species].baseHP, STR_CONV_MODE_LEADING_ZEROS, 3);
            break;
    case STAT_PRINT_ATK:
            ConvertIntToDecimalStringN(gStringVar1, gSpeciesInfo[species].baseAttack, STR_CONV_MODE_LEADING_ZEROS, 3);
            break;
    case STAT_PRINT_DEF:
            ConvertIntToDecimalStringN(gStringVar1, gSpeciesInfo[species].baseDefense, STR_CONV_MODE_LEADING_ZEROS, 3);
            break;
    case STAT_PRINT_SPEED:
            ConvertIntToDecimalStringN(gStringVar1, gSpeciesInfo[species].baseSpeed, STR_CONV_MODE_LEADING_ZEROS, 3);
            break;
    case STAT_PRINT_SPATK:
            ConvertIntToDecimalStringN(gStringVar1, gSpeciesInfo[species].baseSpAttack, STR_CONV_MODE_LEADING_ZEROS, 3);
            break;
    case STAT_PRINT_SPDEF:
            ConvertIntToDecimalStringN(gStringVar1, gSpeciesInfo[species].baseSpDefense, STR_CONV_MODE_LEADING_ZEROS, 3);
            break;
    }
    StringExpandPlaceholders(gStringVar4, sStatsTextPointers[statId]);
}   

static void DexScreen_DrawMonAreaPage(void)
{
    int i;
    u8 width, height;
    s16 left, top;
    u16 kantoMapVoff, species = sPokedexScreenData->dexSpecies;
	bool8 monIsCaught = GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT);

    width = 28;
    height = 14;
    left = 0;
    top = 2;

    FillBgTilemapBufferRect_Palette0(3, 4, left, top, 1, 1);
    FillBgTilemapBufferRect_Palette0(3, BG_TILE_H_FLIP(4), left + 1 + width, top, 1, 1);
    FillBgTilemapBufferRect_Palette0(3, BG_TILE_V_FLIP(4), left, top + 1 + height, 1, 1);
    FillBgTilemapBufferRect_Palette0(3, BG_TILE_H_V_FLIP(4), left + 1 + width, top + 1 + height, 1, 1);
    FillBgTilemapBufferRect_Palette0(3, 5, left + 1, top, width, 1);
    FillBgTilemapBufferRect_Palette0(3, BG_TILE_V_FLIP(5), left + 1, top + 1 + height, width, 1);
    FillBgTilemapBufferRect_Palette0(3, 6, left, top + 1, 1, height);
    FillBgTilemapBufferRect_Palette0(3, BG_TILE_H_FLIP(6), left + 1 + width, top + 1, 1, height);
    FillBgTilemapBufferRect_Palette0(3, 1, left + 1, top + 1, width, height);
    FillBgTilemapBufferRect_Palette0(0, 0, 0, 2, 30, 16);
    FillBgTilemapBufferRect_Palette0(2, 0, 0, 0, 30, 20);
    FillBgTilemapBufferRect_Palette0(1, 0, 0, 0, 30, 20);

    sPokedexScreenData->unlockedSeviiAreas = GetUnlockedSeviiAreas();
    kantoMapVoff = 4;
	
    // If any of the postgame islands are unlocked, Kanto map needs to be flush with the
    // top of the screen.
    for (i = 3; i < 7; i++)
	{
        if ((sPokedexScreenData->unlockedSeviiAreas >> i) & 1)
            kantoMapVoff = 0;
	}
    sPokedexScreenData->windowIds[0] = AddWindow(&sWindowTemplate_AreaMap_Kanto);
    CopyToWindowPixelBuffer(sPokedexScreenData->windowIds[0], (void *)sTilemap_AreaMap_Kanto, 0, 0);
    SetWindowAttribute(sPokedexScreenData->windowIds[0], WINDOW_TILEMAP_TOP, GetWindowAttribute(sPokedexScreenData->windowIds[0], WINDOW_TILEMAP_TOP) + kantoMapVoff);
    PutWindowTilemap(sPokedexScreenData->windowIds[0]);
	
    for (i = 0; i < 7; i++)
	{
        if ((sPokedexScreenData->unlockedSeviiAreas >> i) & 1)
        {
            sPokedexScreenData->windowIds[i + 1] = AddWindow(sAreaMapStructs_SeviiIslands[i].window);
            CopyToWindowPixelBuffer(sPokedexScreenData->windowIds[i + 1], sAreaMapStructs_SeviiIslands[i].tiles, 0, 0);
            SetWindowAttribute(sPokedexScreenData->windowIds[i + 1], WINDOW_TILEMAP_TOP, GetWindowAttribute(sPokedexScreenData->windowIds[i + 1], WINDOW_TILEMAP_TOP) + kantoMapVoff);
            PutWindowTilemap(sPokedexScreenData->windowIds[i + 1]);
            CopyWindowToVram(sPokedexScreenData->windowIds[i + 1], COPYWIN_GFX);
        }
	}
    sPokedexScreenData->windowIds[8] = AddWindow(&sWindowTemplate_AreaMap_SpeciesName);
    sPokedexScreenData->windowIds[9] = AddWindow(&sWindowTemplate_AreaMap_Size);
    sPokedexScreenData->windowIds[10] = AddWindow(&sWindowTemplate_AreaMap_Area);
    sPokedexScreenData->windowIds[11] = AddWindow(&sWindowTemplate_AreaMap_MonIcon);
    sPokedexScreenData->windowIds[12] = AddWindow(&sWindowTemplate_AreaMap_MonTypes);

    // Draw the mon icon
    FillWindowPixelBuffer(sPokedexScreenData->windowIds[11], PIXEL_FILL(0));
	LoadMonIconPaletteIdxAt(GetValidMonIconPalIndex(species), 160);
	LoadMonIconGraphicsInWindow(sPokedexScreenData->windowIds[11], species);
    PutWindowTilemap(sPokedexScreenData->windowIds[11]);
    CopyWindowToVram(sPokedexScreenData->windowIds[11], COPYWIN_GFX);

    // Print mon stats or "Capture to more info"
    FillWindowPixelBuffer(sPokedexScreenData->windowIds[9], PIXEL_FILL(0));
	
	if (monIsCaught)
	{
		PrepareStatToPrint(species, STAT_PRINT_HP);
		DexScreen_AddTextPrinterParameterized(sPokedexScreenData->windowIds[9], 0, gStringVar4, 0, 4, 0);
		PrepareStatToPrint(species, STAT_PRINT_ATK);
		DexScreen_AddTextPrinterParameterized(sPokedexScreenData->windowIds[9], 0, gStringVar4, 44, 4, 0);
		PrepareStatToPrint(species, STAT_PRINT_DEF);
		DexScreen_AddTextPrinterParameterized(sPokedexScreenData->windowIds[9], 0, gStringVar4, 0, 17, 0);
		PrepareStatToPrint(species, STAT_PRINT_SPATK);
		DexScreen_AddTextPrinterParameterized(sPokedexScreenData->windowIds[9], 0, gStringVar4, 44, 17, 0);
		PrepareStatToPrint(species, STAT_PRINT_SPDEF);
		DexScreen_AddTextPrinterParameterized(sPokedexScreenData->windowIds[9], 0, gStringVar4, 0, 30, 0);
		PrepareStatToPrint(species, STAT_PRINT_SPEED);
		DexScreen_AddTextPrinterParameterized(sPokedexScreenData->windowIds[9], 0, gStringVar4, 44, 30, 0);
		DexScreen_AddTextPrinterParameterized(sPokedexScreenData->windowIds[9], 0, gAbilities[gSpeciesInfo[species].abilities[0]].name, 0, 46, 0);
		
		if (gSpeciesInfo[species].abilities[0] != gSpeciesInfo[species].abilities[1])
			DexScreen_AddTextPrinterParameterized(sPokedexScreenData->windowIds[9], 0, gAbilities[gSpeciesInfo[species].abilities[1]].name, 0, 57, 0);
		
		if (gSpeciesInfo[species].hiddenAbility && gSpeciesInfo[species].hiddenAbility != gSpeciesInfo[species].abilities[0])
			DexScreen_AddTextPrinterParameterized(sPokedexScreenData->windowIds[9], 0, gAbilities[gSpeciesInfo[species].hiddenAbility].name, 0, 68, 0);
	}
	else
		DexScreen_AddTextPrinterParameterized(sPokedexScreenData->windowIds[9], 0, gText_CaptureToMoreInfo, 0, 4, 0);

    PutWindowTilemap(sPokedexScreenData->windowIds[9]);
    CopyWindowToVram(sPokedexScreenData->windowIds[9], COPYWIN_GFX);

    // Print "Area"
    FillWindowPixelBuffer(sPokedexScreenData->windowIds[10], PIXEL_FILL(0));
	DexScreen_AddTextPrinterParameterized(sPokedexScreenData->windowIds[10], 0, gText_Area, (sWindowTemplate_AreaMap_Area.width * 8 - GetStringWidth(0, gText_Area, 0)) / 2, 4, 0);
    SetWindowAttribute(sPokedexScreenData->windowIds[10], WINDOW_TILEMAP_TOP, GetWindowAttribute(sPokedexScreenData->windowIds[10], WINDOW_TILEMAP_TOP) + kantoMapVoff);
    PutWindowTilemap(sPokedexScreenData->windowIds[10]);
    CopyWindowToVram(sPokedexScreenData->windowIds[10], COPYWIN_GFX);

    // Print species name
    FillWindowPixelBuffer(sPokedexScreenData->windowIds[8], PIXEL_FILL(0));
    DexScreen_PrintMonDexNo(sPokedexScreenData->windowIds[8], 0, species, 0, 0);
    DexScreen_AddTextPrinterParameterized(sPokedexScreenData->windowIds[8], 2, gSpeciesInfo[species].name, 3, 12, 0);
    PutWindowTilemap(sPokedexScreenData->windowIds[8]);
    CopyWindowToVram(sPokedexScreenData->windowIds[8], COPYWIN_GFX);

    // Type icons
    FillWindowPixelBuffer(sPokedexScreenData->windowIds[12], PIXEL_FILL(0));
    ListMenuLoadStdPalAt(176, 1);

    if (monIsCaught)
    {
        BlitMoveInfoIcon(sPokedexScreenData->windowIds[12], 1 + gSpeciesInfo[species].types[0], 0, 1);
		
        if (gSpeciesInfo[species].types[0] != gSpeciesInfo[species].types[1])
            BlitMoveInfoIcon(sPokedexScreenData->windowIds[12], 1 + gSpeciesInfo[species].types[1], 32, 1);
    }
    PutWindowTilemap(sPokedexScreenData->windowIds[12]);
    CopyWindowToVram(sPokedexScreenData->windowIds[12], COPYWIN_GFX);

    // Show size comparison
    ResetAllPicSprites();
    LoadPalette(sPalette_Silhouette, 288, 32);
    sPokedexScreenData->windowIds[14] = 0xff;
    sPokedexScreenData->windowIds[15] = 0xff;

    // Create the area markers
    sPokedexScreenData->data[2] = Ctor_PokedexAreaMarkers(species, 2001, 3, kantoMapVoff * 8);
	
    if (!(PokedexAreaMarkers_Any(sPokedexScreenData->data[2])))
    {
        BlitBitmapRectToWindow(sPokedexScreenData->windowIds[0], (void *)sBlitTiles_WideEllipse, 0, 0, 88, 16, 4, 28, 88, 16);
		DexScreen_AddTextPrinterParameterized(sPokedexScreenData->windowIds[0], 0, gText_AreaUnknown, (96 - GetStringWidth(0, gText_AreaUnknown, 0)) / 2, 29, 0);
    }
    CopyWindowToVram(sPokedexScreenData->windowIds[0], COPYWIN_GFX);

    // Draw the control info
    FillWindowPixelBuffer(1, PIXEL_FILL(15));
    DexScreen_AddTextPrinterParameterized(1, 0, gText_Cry, 8, 2, 4);
    DexScreen_PrintControlInfo(gText_CancelPreviousData);
    PutWindowTilemap(1);
    CopyWindowToVram(1, COPYWIN_GFX);
}


static void DexScreen_DestroyAreaScreenResources(void)
{
    int i;

    Dtor_PokedexAreaMarkers(sPokedexScreenData->data[2]);

    for (i = 0; i < 13; i++)
        DexScreen_RemoveWindow(&sPokedexScreenData->windowIds[i]);
	
    if (sPokedexScreenData->windowIds[15] != 0xff)
        FreeAndDestroyTrainerPicSprite(sPokedexScreenData->windowIds[15]);
	
    if (sPokedexScreenData->windowIds[14] != 0xff)
        FreeAndDestroyMonPicSprite(sPokedexScreenData->windowIds[14]);
}

static bool8 DexScreen_IsPageUnlocked(u8 categoryNum, u8 pageNum)
{
    int i, count = gDexCategories[categoryNum].page[pageNum].count;

    for (i = 0; i < 4; i++)
    {
        if (i < count)
        {
            if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(gDexCategories[categoryNum].page[pageNum].species[i]), FLAG_GET_SEEN))
                return TRUE;
        }
    }
    return FALSE;
}

static bool8 DexScreen_IsCategoryUnlocked(u8 categoryNum)
{
    int i;
    u8 count = gDexCategories[categoryNum].count;

    for (i = 0; i < count; i++)
	{
        if (DexScreen_IsPageUnlocked(categoryNum, i))
            return TRUE;
	}
    return FALSE;
}

static void DexScreen_CreateCategoryPageSpeciesList(u8 categoryNum, u8 pageNum)
{
    int i, count = gDexCategories[categoryNum].page[pageNum].count;
    u16 species;

    sPokedexScreenData->numMonsOnPage = 0;

    for (i = 0; i < 4; i++)
        sPokedexScreenData->pageSpecies[i] = 0xFFFF;
	
    for (i = 0; i < count; i++)
    {
        species = gDexCategories[categoryNum].page[pageNum].species[i];
		
        if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_SEEN))
        {
            sPokedexScreenData->pageSpecies[sPokedexScreenData->numMonsOnPage] = species;
            sPokedexScreenData->numMonsOnPage++;
        }
    }
}

static void DexScreen_GetPageLimitsForCategory(u8 category)
{
    int i;
    u8 firstPage = 0xFF, lastPage = 0xFF, count = gDexCategories[category].count;

    for (i = 0; i < count; i++)
	{
        if (DexScreen_IsPageUnlocked(category, i))
        {
            if (firstPage == 0xFF)
                firstPage = i;
            lastPage = i;
        }
	}
    if (lastPage != 0xFF)
    {
        sPokedexScreenData->firstPageInCategory = firstPage;
        sPokedexScreenData->lastPageInCategory = lastPage + 1;
    }
    else
    {
        sPokedexScreenData->firstPageInCategory = 0;
        sPokedexScreenData->lastPageInCategory = 0;
    }
}

static bool8 DexScreen_LookUpCategoryBySpecies(u16 species)
{
    int i, j, k, categoryCount, categoryPageCount, posInPage;
    u16 dexSpecies;

    for (i = 0; i < ARRAY_COUNT(gDexCategories); i++)
    {
        categoryCount = gDexCategories[i].count;
		
        for (j = 0; j < categoryCount; j++)
        {
            categoryPageCount = gDexCategories[i].page[j].count;
			
            for (k = 0, posInPage = 0; k < categoryPageCount; k++)
            {
                dexSpecies = gDexCategories[i].page[j].species[k];
				
                if (species == dexSpecies)
                {
                    sPokedexScreenData->category = i;
                    sPokedexScreenData->pageNum = j;
                    sPokedexScreenData->categoryCursorPosInPage = posInPage;
                    return FALSE;
                }
                if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(dexSpecies), FLAG_GET_SEEN))
                    posInPage++;
            }
        }
    }
    return TRUE;
}

static u8 DexScreen_PageNumberToRenderablePages(u16 page)
{
    int i, count;

    for (i = 0, count = 0; i < page; i++)
	{
        if (DexScreen_IsPageUnlocked(sPokedexScreenData->category, i))
            count++;
	}
    return count + 1;
}

static void DexScreen_InputHandler_StartToCry(void)
{
    if (JOY_NEW(START_BUTTON))
        PlayCry_NormalNoDucking(sPokedexScreenData->dexSpecies, 0, 125, 10);
}

u8 DexScreen_RegisterMonToPokedex(u16 species)
{
	u16 nationalDexNum = SpeciesToNationalPokedexNum(species);
	
    GetSetPokedexFlag(nationalDexNum, FLAG_SET_SEEN);
    GetSetPokedexFlag(nationalDexNum, FLAG_SET_CAUGHT);

    DexScreen_LoadResources();
    gTasks[sPokedexScreenData->taskId].func = Task_DexScreen_RegisterMonToPokedex;
    DexScreen_LookUpCategoryBySpecies(species);

    return sPokedexScreenData->taskId;
}

static void Task_DexScreen_RegisterMonToPokedex(u8 taskId)
{
    switch (sPokedexScreenData->state)
    {
    case 0:
        DexScreen_GetPageLimitsForCategory(sPokedexScreenData->category);
        if (sPokedexScreenData->pageNum < sPokedexScreenData->firstPageInCategory)
            sPokedexScreenData->pageNum = sPokedexScreenData->firstPageInCategory;
        sPokedexScreenData->state = 3;
        break;
    case 1:
        RemoveDexPageWindows();
        DexScreen_DestroyCategoryPageMonIconAndInfoWindows();
        gMain.state = 0;
        sPokedexScreenData->state = 2;
        break;
    case 2:
        if (DoClosePokedex())
            DestroyTask(taskId);
        break;
    case 3:
        DexScreen_CreateCategoryListGfx(TRUE);
        PutWindowTilemap(0);
        PutWindowTilemap(1);

        CopyBgTilemapBufferToVram(3);
        CopyBgTilemapBufferToVram(2);
        CopyBgTilemapBufferToVram(1);
        CopyBgTilemapBufferToVram(0);

        DexScreen_CreateCategoryPageSelectionCursor(0xff);

        sPokedexScreenData->state = 4;
        break;
    case 4:
        gPaletteFade.bufferTransferDisabled = 0;
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, 0xffff);
        ShowBg(3);
        ShowBg(2);
        ShowBg(1);
        ShowBg(0);

        sPokedexScreenData->state = 5;
        break;
    case 5:
        gTasks[taskId].data[0] = 30;
        sPokedexScreenData->categoryPageCursorTaskId = ListMenuAddCursorObjectInternal(&sCursorStruct_CategoryPage, 0);
        sPokedexScreenData->state = 6;
        break;
    case 6:
        DexScreen_CreateCategoryPageSelectionCursor(sPokedexScreenData->categoryCursorPosInPage);
        DexScreen_UpdateCategoryPageCursorObject(sPokedexScreenData->categoryPageCursorTaskId, sPokedexScreenData->categoryCursorPosInPage, sPokedexScreenData->numMonsOnPage);

        if (gTasks[taskId].data[0])
            gTasks[taskId].data[0]--;
        else
        {
            ListMenuRemoveCursorObject(sPokedexScreenData->categoryPageCursorTaskId, 0);
            sPokedexScreenData->state = 7;
        }
        break;
    case 7:
        sPokedexScreenData->dexSpecies = sPokedexScreenData->pageSpecies[sPokedexScreenData->categoryCursorPosInPage];
        sPokedexScreenData->state = 8;
        break;
    case 8:
        DexScreen_DrawMonDexPage(TRUE);
        sPokedexScreenData->state = 9;
        break;
    case 9:
        sPokedexScreenData->data[0] = 0;
        sPokedexScreenData->data[1] = 0;
        sPokedexScreenData->state++;
    case 10:
        if (sPokedexScreenData->data[1] < 6)
        {
            if (sPokedexScreenData->data[0])
            {
                DexScreen_DexPageZoomEffectFrame(0, sPokedexScreenData->data[1]);
                CopyBgTilemapBufferToVram(0);
                sPokedexScreenData->data[0] = 4;
                sPokedexScreenData->data[1]++;
            }
            else
                sPokedexScreenData->data[0]--;
        }
        else
        {
            FillBgTilemapBufferRect_Palette0(0, 0, 0, 2, 30, 16);
            CopyBgTilemapBufferToVram(3);
            CopyBgTilemapBufferToVram(2);
            CopyBgTilemapBufferToVram(1);
            CopyBgTilemapBufferToVram(0);

            PlayCry_NormalNoDucking(sPokedexScreenData->dexSpecies, 0, 125, CRY_PRIORITY_NORMAL);
            sPokedexScreenData->data[0] = 0;
            sPokedexScreenData->state = 11;
        }
        break;
    case 11:
        if (JOY_NEW(A_BUTTON | B_BUTTON))
            sPokedexScreenData->state = 2;
        break;
    }
}

static void DexScreen_PrintStringWithAlignment(const u8 * str, s32 mode)
{
    u32 x;

    switch (mode)
    {
    case TEXT_LEFT:
        x = 8;
        break;
    case TEXT_CENTER:
        x = (u32)(240 - GetStringWidth(2, str, 0)) / 2;
        break;
    case TEXT_RIGHT:
    default:
        x = 232 - GetStringWidth(2, str, 0);
        break;
    }
    DexScreen_AddTextPrinterParameterized(0, 2, str, x, 2, 4);
}
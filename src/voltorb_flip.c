// Credits: Pokabbie & Blackuser
#include "coins.h"
#include "event_data.h"
#include "field_specials.h"
#include "global.h"
#include "gflib.h"
#include "m4a.h"
#include "menu.h"
#include "menu_helpers.h"
#include "new_menu_helpers.h"
#include "overworld.h"
#include "random.h"
#include "scanline_effect.h"
#include "script.h"
#include "task.h"
#include "text_window.h"
#include "constants/songs.h"

enum
{
    GAME_STATE_IN_PROGRESS,
    GAME_STATE_MAX_COINS,
    GAME_STATE_WIN,
    GAME_STATE_LOSE
};

enum
{
    WIN_INFO_MESSAGE,
    WIN_INFO_RED_X,
    WIN_INFO_GREEN_X,
    WIN_INFO_YELLOW_X,
    WIN_INFO_BLUE_X,
    WIN_INFO_PURPLE_X,
    WIN_INFO_RED_Y,
    WIN_INFO_GREEN_Y,
    WIN_INFO_YELLOW_Y,
    WIN_INFO_BLUE_Y,
    WIN_INFO_PURPLE_Y,
    WIN_INFO_LEVEL,
    WIN_INFO_LR_BUTTONS,
    WIN_INFO_NUM_COINS,
    WIN_INFO_COLLECTED_COINS,
    WIN_INFO_QUIT,
    WIN_INFO_BLACK_BACKGROUND,
    WIN_COUNT
};

enum
{
    CARD_GFX_HIDDEN,
    CARD_GFX_SHOW_1,
    CARD_GFX_SHOW_2,
    CARD_GFX_SHOW_3,
    CARD_GFX_SHOW_VOLTORB,
    CARD_GFX_COUNT,
    // For the flip anim stages
    CARD_GFX_MID_HIDDEN = 0xFE,
    CARD_GFX_SHOW_FROM_CARD = 0xFF
};

enum
{
    CARD_VALUE_1,
    CARD_VALUE_2,
    CARD_VALUE_3,
    CARD_VALUE_VOLTORB,
    CARD_VALUE_COUNT
};

#define CARD_TO_MULTIPLIER(cardValue) (cardValue + 1)

#define BOARD_WIDTH  5
#define BOARD_HEIGHT 5
#define TOTAL_CARD_COUNT (BOARD_WIDTH * BOARD_HEIGHT)

#define CARD_INITIAL_X_OFFSET 3
#define CARD_INITIAL_Y_OFFSET 1

#define COORDS_TO_CARD(x, y) (y * BOARD_WIDTH + x)

#define MAX_VOLTORB_FLIP_LEVEL     8
#define MAX_VOLTORB_LEVEL_VARIANTS 5
#define MAX_COLLECTED_COINS        50000

#define SPRITE_SHEET_TILE_TAG 5525
#define SPRITE_SHEET_PAL_TAG  5526

struct VoltorbSpawnCounts
{
    u8 x2Count;
    u8 x3Count;
    u8 voltorbCount;
};

struct VoltorbCardState
{
    u8 value:2;
    u8 isShown:1;
    u8 note1:1;
    u8 note2:1;
    u8 note3:1;
    u8 noteVoltorb:1;
    u8 unused:1;
};

struct VoltorbFlipState
{
    u16 tilemapBuffer[BG_SCREEN_SIZE];
    u16 numCoins;
    u16 numCollectedCoins;
    struct VoltorbCardState cardStates[TOTAL_CARD_COUNT];
    u8 cursorX:3;
    u8 cursorY:3;
    u8 flippingCard:1;
    u8 explodingVoltorb:1;
    u8 pointerSpriteId;
    u8 pointerOutlineSpriteId;
    u8 cursorWriteValue:3;
    u8 numCardsFlipped:5;
    u8 level:4;
    u8 isCursorOnQuitButton:1;
    u8 loseRound:1;
    u8 unused:2;
};

static void CB2_ShowVoltorbFlip(void);
static void InitVoltorbFlipWindows(void);
static void InitVoltorbFlipSprites(void);
static void SetPointerSpriteVisibility(bool32 invisible);
static void UpdateVoltorbFlipSprites(void);
static void PrintPlayerAndCollectedCoinsText(void);
static void PrintInstructionsTexts(void);
static void ResetVoltorbFlipCards(void);
static void PrintVoltorbFlipLevel(void);
static void InitVoltorbFlipRound(void);
static void StartCardFlipSequence(bool32 show, bool32 allCards);
static void CreateGameClearSequenceTask(u32 mainTaskId);
static void CreateGameLoseSequenceTask(u32 mainTaskId);
static void CreateGameQuitSequenceTask(u32 mainTaskId);
static void CreateEarnedMaxCoinsSequenceTask(u32 mainTaskId);
static void SpriteCB_VoltorbExplosion(struct Sprite *sprite);
static void Task_VoltorbFlip_HandleInput(u32 taskId);
// Game sequence funcs
static bool32 VFSequence_FadeOutBgPalettes(s16 *data);
static bool32 VFSequence_FadeOutExceptionPalettes(s16 *data);
static bool32 VFSequence_WaitPaletteFade(s16 *data);
static bool32 VFSequence_PrintGameClearText(s16 *data);
static bool32 VFSequence_WaitText_CloseBox(s16 *data);
static bool32 VFSequence_FadeInBgPalettes(s16 *data);
static bool32 VFSequence_GiveCollectedCoinsToPlayer(s16 *data);
static bool32 VFSequence_PrintReceivedCoinsText(s16 *data);
static bool32 VFSequence_ShowAllCards(s16 *data);
static bool32 VFSequence_WaitCardFlipAnim(s16 *data);
static bool32 VFSequence_WaitAOrBButtonPress(s16 *data);
static bool32 VFSequence_HideAllCards(s16 *data);
static bool32 VFSequence_AdvanceLevel(s16 *data);
static bool32 VFSequence_HidePointerSprite(s16 *data);
static bool32 VFSequence_PrintCurrentLevel(s16 *data);
static bool32 VFSequence_PrintAdvancedLevelText(s16 *data);
static bool32 VFSequence_PrintAskPlayLevelText(s16 *data);
static bool32 VFSequence_WaitText_DontCloseBox(s16 *data);
static bool32 VFSequence_CreateYesNoMenu(s16 *data);
static bool32 VFSequence_HandleYesNoInput(s16 *data);
static bool32 VFSequence_CloseMessageBox(s16 *data);
static bool32 VFSequence_StartRound(s16 *data);
static bool32 VFSequence_ShowPointerSprite(s16 *data);
static bool32 VFSequence_FadeToExit(s16 *data);
static bool32 VFSequence_ExitFromGame(s16 *data);
static bool32 VFSequence_WaitVoltorbExplosionAnim(s16 *data);
static bool32 VFSequence_LoseCollectedCoins(s16 *data);
static bool32 VFSequence_PrintGet0CoinsText(s16 *data);
static bool32 VFSequence_RegressLevel(s16 *data);
static bool32 VFSequence_PrintAskQuitGameText(s16 *data);
static bool32 VFSequence_TryPrintCoinMultiplierText(s16 *data);
static bool32 VFSequence_UpdateCollectedCoins(s16 *data);
static bool32 VFSequence_TryPrintWhatExpectingText(s16 *data);
static bool32 VFSequence_WaitFanfare(s16 *data);
static bool32 VFSequence_FlipCardAnim(s16 *data);
static bool32 VFSequence_PrintDroppedLevelText(s16 *data);
static bool32 VFSequence_PrintGatheredMaxCoinsText(s16 *data);
static bool32 VFSequence_EnableMainTaskInput(s16 *data);
static bool32 VFSequence_End(s16 *data);

static EWRAM_DATA struct VoltorbFlipState *sVoltorbFlipState = NULL;

static const u32 sVoltorbFlipTiles[] = INCBIN_U32("graphics/voltorb_flip/gameboard.4bpp.lz");
static const u32 sVoltorbFlipTilemap[] = INCBIN_U32("graphics/voltorb_flip/gameboard.bin.lz");
static const u16 sVoltorbFlipPalette[] = INCBIN_U16("graphics/voltorb_flip/gameboard.gbapal");
static const u8 sVoltorbFlipSpriteSheetData[] = INCBIN_U8("graphics/voltorb_flip/sprites.4bpp");
static const u16 sVoltorbFlipSpritePaletteData[] = INCBIN_U16("graphics/voltorb_flip/sprites.gbapal");
static const u8 sExplosionSpriteSheetData[] = INCBIN_U8("graphics/voltorb_flip/explosion.4bpp");
static const u16 sExplosionSpritePaletteData[] = INCBIN_U16("graphics/voltorb_flip/explosion.gbapal");

static const u8 sTextColors[][3] =
{
    {0, 2, 3},
    {0, 1, 14},
    {0, 1, 2}
};

static const u8 sDigitsToCoinChangeValue[] = {1, 1, 10, 40};

static const struct BgTemplate sVoltorbFlipBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
    {
        .bg = 1,
        .charBaseIndex = 0,
        .mapBaseIndex = 6,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0,
    },
    {
        .bg = 2,
        .charBaseIndex = 1,
        .mapBaseIndex = 28,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0,
    },
    {
        .bg = 3,
        .charBaseIndex = 1,
        .mapBaseIndex = 29,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0,
    }
};

static const struct WindowTemplate sVoltorbFlipYesNoWindow =
{
    .bg = 0,
    .tilemapLeft = 21,
    .tilemapTop = 9,
    .width = 6,
    .height = 4,
    .paletteNum = 14,
    .baseBlock = 112,
};

static const struct WindowTemplate sVoltorbFlipWinTemplates[WIN_COUNT + 1] =
{
    // Bg 0 windows
    [WIN_INFO_MESSAGE] =
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = 28,
        .height = 4,
        .paletteNum = 14,
        .baseBlock = 160,
    },
    
    // Bg 2 windows
    [WIN_INFO_RED_X] =
    {
        .bg = 2,
        .tilemapLeft = 18,
        .tilemapTop = 1,
        .width = 3,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 1,
    },
    [WIN_INFO_GREEN_X] =
    {
        .bg = 2,
        .tilemapLeft = 18,
        .tilemapTop = 1 + 3 * 1,
        .width = 3,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 10,
    },
    [WIN_INFO_YELLOW_X] =
    {
        .bg = 2,
        .tilemapLeft = 18,
        .tilemapTop = 1 + 3 * 2,
        .width = 3,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 19,
    },
    [WIN_INFO_BLUE_X] =
    {
        .bg = 2,
        .tilemapLeft = 18,
        .tilemapTop = 1 + 3 * 3,
        .width = 3,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 28,
    },
    [WIN_INFO_PURPLE_X] =
    {
        .bg = 2,
        .tilemapLeft = 18,
        .tilemapTop = 1 + 3 * 4,
        .width = 3,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 37,
    },
    
    [WIN_INFO_RED_Y] =
    {
        .bg = 2,
        .tilemapLeft = 3,
        .tilemapTop = 16,
        .width = 3,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 46,
    },
    [WIN_INFO_GREEN_Y] =
    {
        .bg = 2,
        .tilemapLeft = 3 + 3 * 1,
        .tilemapTop = 16,
        .width = 3,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 55,
    },
    [WIN_INFO_YELLOW_Y] =
    {
        .bg = 2,
        .tilemapLeft = 3 + 3 * 2,
        .tilemapTop = 16,
        .width = 3,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 64,
    },
    [WIN_INFO_BLUE_Y] =
    {
        .bg = 2,
        .tilemapLeft = 3 + 3 * 3,
        .tilemapTop = 16,
        .width = 3,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 73,
    },
    [WIN_INFO_PURPLE_Y] =
    {
        .bg = 2,
        .tilemapLeft = 3 + 3 * 4,
        .tilemapTop = 16,
        .width = 3,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 82,
    },
    
    [WIN_INFO_LEVEL] =
    {
        .bg = 2,
        .tilemapLeft = 23,
        .tilemapTop = 0,
        .width = 3,
        .height = 2,
        .paletteNum = 0,
        .baseBlock = 91,
    },
    
    [WIN_INFO_LR_BUTTONS] =
    {
        .bg = 2,
        .tilemapLeft = 22,
        .tilemapTop = 4,
        .width = 5,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 97,
    },
    
    [WIN_INFO_NUM_COINS] =
    {
        .bg = 2,
        .tilemapLeft = 22,
        .tilemapTop = 6,
        .width = 5,
        .height = 5,
        .paletteNum = 15,
        .baseBlock = 107,
    },
    [WIN_INFO_COLLECTED_COINS] =
    {
        .bg = 2,
        .tilemapLeft = 22,
        .tilemapTop = 11,
        .width = 5,
        .height = 5,
        .paletteNum = 15,
        .baseBlock = 132,
    },
    
    [WIN_INFO_QUIT] =
    {
        .bg = 2,
        .tilemapLeft = 22,
        .tilemapTop = 17,
        .width = 5,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 157,
    },
    
    // Bg 3 windows
    [WIN_INFO_BLACK_BACKGROUND] =
    {
        .bg = 3,
        .tilemapLeft = 2,
        .tilemapTop = 1,
        .width = 17,
        .height = 15,
        .paletteNum = 14,
        .baseBlock = 167,
    },
    [WIN_COUNT] = DUMMY_WIN_TEMPLATE
};

static const struct SpriteSheet sVoltorbFlipSpriteSheet =
{
    .data = sVoltorbFlipSpriteSheetData,
    .size = 2048,
    .tag = SPRITE_SHEET_TILE_TAG
};

static const struct SpritePalette sVoltorbFlipSpritePalette =
{
    .data = sVoltorbFlipSpritePaletteData,
    .tag = SPRITE_SHEET_PAL_TAG
};

static const struct SpriteSheet sExplosionSpriteSheet =
{
    .data = sExplosionSpriteSheetData,
    .size = sizeof(sExplosionSpriteSheetData),
    .tag = SPRITE_SHEET_TILE_TAG + 1
};

static const struct SpritePalette sExplosionSpritePalette =
{
    .data = sExplosionSpritePaletteData,
    .tag = SPRITE_SHEET_PAL_TAG + 1
};

static const struct OamData sPointerSpriteOamData =
{
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 1
};

static const union AnimCmd sPointerSprite_PointerFrame[] =
{
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_END
};

static const union AnimCmd sPointerSprite_PencilFrame[] =
{
    ANIMCMD_FRAME(16, 5),
    ANIMCMD_END
};

static const union AnimCmd sPointerSprite_PointerOutlineFrame[] =
{
    ANIMCMD_FRAME(32, 5),
    ANIMCMD_END
};

static const union AnimCmd sPointerSprite_PencilOutlineFrame[] =
{
    ANIMCMD_FRAME(48, 5),
    ANIMCMD_END
};

static const union AnimCmd *const sPencilAnimationCommands[] =
{
    sPointerSprite_PointerFrame,
    sPointerSprite_PencilFrame,
    sPointerSprite_PointerOutlineFrame,
    sPointerSprite_PencilOutlineFrame,
};

static const struct SpriteTemplate sPointerSpriteTemplate =
{
    .tileTag = SPRITE_SHEET_TILE_TAG,
    .paletteTag = SPRITE_SHEET_PAL_TAG,
    .oam = &sPointerSpriteOamData,
    .anims = sPencilAnimationCommands,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

static const struct OamData sExplosionSpriteOamData =
{
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .size = SPRITE_SIZE(16x16),
    .priority = 1
};

static const union AnimCmd sExplosionAnimCmds[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(4, 4),
    ANIMCMD_FRAME(8, 4),
    ANIMCMD_FRAME(12, 4),
    ANIMCMD_FRAME(16, 4),
    ANIMCMD_END
};

static const union AnimCmd *const sExplosionAnimTable[] =
{
    sExplosionAnimCmds
};

static const union AffineAnimCmd sExplosionAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(256, 256, 0, 1), // Double sprite size
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const sExplosionAffineAnimTable[] =
{
    sExplosionAffineAnimCmds
};

static const struct SpriteTemplate sVoltorbExplosionSpriteTemplate =
{
    .tileTag = SPRITE_SHEET_TILE_TAG + 1,
    .paletteTag = SPRITE_SHEET_PAL_TAG + 1,
    .oam = &sExplosionSpriteOamData,
    .anims = sExplosionAnimTable,
    .images = NULL,
    .affineAnims = sExplosionAffineAnimTable,
    .callback = SpriteCB_VoltorbExplosion
};

static bool32 (*const sCardFlipSequenceFuncs[])(s16 *) =
{
    VFSequence_TryPrintWhatExpectingText,
    VFSequence_WaitText_DontCloseBox,
    VFSequence_WaitFanfare,
    VFSequence_CloseMessageBox,
    VFSequence_FlipCardAnim,
    VFSequence_TryPrintCoinMultiplierText,
    VFSequence_WaitText_CloseBox,
    VFSequence_UpdateCollectedCoins,
    VFSequence_End
};

static bool32 (*const sGameClearSequenceFuncs[])(s16 *) =
{
    VFSequence_FadeOutBgPalettes,
    VFSequence_WaitPaletteFade,
    VFSequence_PrintGameClearText,
    VFSequence_WaitText_CloseBox,
    VFSequence_FadeInBgPalettes,
    VFSequence_WaitPaletteFade,
    VFSequence_GiveCollectedCoinsToPlayer,
    VFSequence_FadeOutBgPalettes,
    VFSequence_WaitPaletteFade,
    VFSequence_PrintReceivedCoinsText,
    VFSequence_WaitText_CloseBox,
    VFSequence_FadeInBgPalettes,
    VFSequence_WaitPaletteFade,
    VFSequence_ShowAllCards,
    VFSequence_WaitCardFlipAnim,
    VFSequence_WaitAOrBButtonPress,
    VFSequence_HideAllCards,
    VFSequence_WaitCardFlipAnim,
    VFSequence_AdvanceLevel,
    VFSequence_HidePointerSprite,
    VFSequence_PrintCurrentLevel,
    VFSequence_FadeOutBgPalettes,
    VFSequence_WaitPaletteFade,
    VFSequence_PrintAdvancedLevelText,
    VFSequence_WaitText_DontCloseBox,
    VFSequence_PrintAskPlayLevelText,
    VFSequence_WaitText_DontCloseBox,
    VFSequence_CreateYesNoMenu,
    VFSequence_HandleYesNoInput,
    // No
    VFSequence_FadeOutExceptionPalettes,
    VFSequence_WaitPaletteFade,
    VFSequence_FadeToExit,
    VFSequence_WaitPaletteFade,
    VFSequence_ExitFromGame,
    // Yes
    VFSequence_CloseMessageBox,
    VFSequence_FadeInBgPalettes,
    VFSequence_WaitPaletteFade,
    VFSequence_StartRound,
    VFSequence_ShowPointerSprite,
    VFSequence_EnableMainTaskInput,
    VFSequence_End
};

static bool32 (*const sGameLoseSequenceFuncs[])(s16 *) =
{
    VFSequence_WaitVoltorbExplosionAnim,
    VFSequence_LoseCollectedCoins,
    VFSequence_PrintGet0CoinsText,
    VFSequence_WaitText_CloseBox,
    VFSequence_ShowAllCards,
    VFSequence_WaitCardFlipAnim,
    VFSequence_WaitAOrBButtonPress,
    VFSequence_HideAllCards,
    VFSequence_WaitCardFlipAnim,
    VFSequence_RegressLevel,
    VFSequence_HidePointerSprite,
    VFSequence_PrintCurrentLevel,
    VFSequence_FadeOutBgPalettes,
    VFSequence_WaitPaletteFade,
    VFSequence_PrintDroppedLevelText,
    VFSequence_WaitText_CloseBox,
    VFSequence_FadeInBgPalettes,
    VFSequence_WaitPaletteFade,
    VFSequence_StartRound,
    VFSequence_ShowPointerSprite,
    VFSequence_EnableMainTaskInput,
    VFSequence_End
};

static bool32 (*const sGameQuitSequenceFuncs[])(s16 *) =
{
    VFSequence_FadeOutBgPalettes,
    VFSequence_WaitPaletteFade,
    VFSequence_PrintAskQuitGameText,
    VFSequence_WaitText_DontCloseBox,
    VFSequence_CreateYesNoMenu,
    VFSequence_HandleYesNoInput,
    // No
    VFSequence_CloseMessageBox,
    VFSequence_FadeInBgPalettes,
    VFSequence_WaitPaletteFade,
    VFSequence_EnableMainTaskInput,
    VFSequence_End,
    // Yes
    VFSequence_CloseMessageBox,
    VFSequence_FadeInBgPalettes,
    VFSequence_WaitPaletteFade,
    VFSequence_GiveCollectedCoinsToPlayer,
    VFSequence_ShowAllCards,
    VFSequence_WaitCardFlipAnim,
    VFSequence_WaitAOrBButtonPress,
    VFSequence_HideAllCards,
    VFSequence_WaitCardFlipAnim,
    VFSequence_RegressLevel,
    VFSequence_HidePointerSprite,
    VFSequence_PrintCurrentLevel,
    VFSequence_FadeOutBgPalettes,
    VFSequence_WaitPaletteFade,
    VFSequence_PrintDroppedLevelText,
    VFSequence_WaitText_DontCloseBox,
    VFSequence_PrintAskPlayLevelText,
    VFSequence_WaitText_DontCloseBox,
    VFSequence_CreateYesNoMenu,
    VFSequence_HandleYesNoInput,
    // No
    VFSequence_FadeOutExceptionPalettes,
    VFSequence_WaitPaletteFade,
    VFSequence_FadeToExit,
    VFSequence_WaitPaletteFade,
    VFSequence_ExitFromGame,
    // Yes
    VFSequence_CloseMessageBox,
    VFSequence_FadeInBgPalettes,
    VFSequence_WaitPaletteFade,
    VFSequence_StartRound,
    VFSequence_ShowPointerSprite,
    VFSequence_EnableMainTaskInput,
    VFSequence_End
};

static bool32 (*const sEarnedMaxCoinsSequenceFuncs[])(s16 *) =
{
    VFSequence_FadeOutBgPalettes,
    VFSequence_WaitPaletteFade,
    VFSequence_PrintGatheredMaxCoinsText,
    VFSequence_WaitText_DontCloseBox,
    VFSequence_FadeOutExceptionPalettes,
    VFSequence_WaitPaletteFade,
    VFSequence_FadeToExit,
    VFSequence_WaitPaletteFade,
    VFSequence_ExitFromGame
};

static const struct VoltorbSpawnCounts sVoltorbSpawnCounts[MAX_VOLTORB_FLIP_LEVEL][MAX_VOLTORB_LEVEL_VARIANTS] = 
{
    {
        { .x2Count = 3, .x3Count = 1, .voltorbCount = 6 },
        { .x2Count = 0, .x3Count = 3, .voltorbCount = 6 },
        { .x2Count = 5, .x3Count = 0, .voltorbCount = 6 },
        { .x2Count = 2, .x3Count = 2, .voltorbCount = 6 },
        { .x2Count = 4, .x3Count = 1, .voltorbCount = 6 },
    },
    {
        { .x2Count = 1, .x3Count = 3, .voltorbCount = 7 },
        { .x2Count = 6, .x3Count = 0, .voltorbCount = 7 },
        { .x2Count = 3, .x3Count = 2, .voltorbCount = 7 },
        { .x2Count = 0, .x3Count = 4, .voltorbCount = 7 },
        { .x2Count = 5, .x3Count = 1, .voltorbCount = 7 },
    },
    {
        { .x2Count = 2, .x3Count = 3, .voltorbCount = 8 },
        { .x2Count = 7, .x3Count = 0, .voltorbCount = 8 },
        { .x2Count = 4, .x3Count = 2, .voltorbCount = 8 },
        { .x2Count = 1, .x3Count = 4, .voltorbCount = 8 },
        { .x2Count = 6, .x3Count = 1, .voltorbCount = 8 },
    },
    {
        { .x2Count = 3, .x3Count = 3, .voltorbCount = 8 },
        { .x2Count = 0, .x3Count = 5, .voltorbCount = 8 },
        { .x2Count = 8, .x3Count = 0, .voltorbCount = 10 },
        { .x2Count = 5, .x3Count = 2, .voltorbCount = 10 },
        { .x2Count = 2, .x3Count = 4, .voltorbCount = 10 },
    },
    {
        { .x2Count = 7, .x3Count = 1, .voltorbCount = 10 },
        { .x2Count = 4, .x3Count = 3, .voltorbCount = 10 },
        { .x2Count = 1, .x3Count = 5, .voltorbCount = 10 },
        { .x2Count = 9, .x3Count = 0, .voltorbCount = 10 },
        { .x2Count = 6, .x3Count = 2, .voltorbCount = 10 },
    },
    {
        { .x2Count = 3, .x3Count = 4, .voltorbCount = 10 },
        { .x2Count = 0, .x3Count = 6, .voltorbCount = 10 },
        { .x2Count = 8, .x3Count = 1, .voltorbCount = 10 },
        { .x2Count = 5, .x3Count = 3, .voltorbCount = 10 },
        { .x2Count = 2, .x3Count = 5, .voltorbCount = 10 },
    },
    {
        { .x2Count = 7, .x3Count = 2, .voltorbCount = 10 },
        { .x2Count = 4, .x3Count = 4, .voltorbCount = 10 },
        { .x2Count = 1, .x3Count = 6, .voltorbCount = 13 },
        { .x2Count = 9, .x3Count = 1, .voltorbCount = 13 },
        { .x2Count = 6, .x3Count = 3, .voltorbCount = 10 },
    },
    {
        { .x2Count = 0, .x3Count = 7, .voltorbCount = 10 },
        { .x2Count = 8, .x3Count = 2, .voltorbCount = 10 },
        { .x2Count = 5, .x3Count = 4, .voltorbCount = 10 },
        { .x2Count = 2, .x3Count = 6, .voltorbCount = 10 },
        { .x2Count = 7, .x3Count = 3, .voltorbCount = 10 },
    },
};

#define FLIP_VERTICAL   (0x08 << 8)
#define FLIP_HORIZONTAL (0x04 << 8)

static const u16 sQuitButtonGfxs[2][10] =
{
    // Deselected
    [FALSE] =
    {
        0x20, 0x21, 0x21, 0x21, 0x22,
        0x20 | FLIP_VERTICAL, 0x21 | FLIP_VERTICAL, 0x21 | FLIP_VERTICAL, 0x21 | FLIP_VERTICAL, 0x22 | FLIP_VERTICAL,
    },
    // Selected
    [TRUE] =
    {
        0x12, 0x13, 0x13, 0x13, 0x14,
        0x12 | FLIP_VERTICAL, 0x13 | FLIP_VERTICAL, 0x13 | FLIP_VERTICAL, 0x13 | FLIP_VERTICAL, 0x14 | FLIP_VERTICAL,
    }
};

static const u16 sCardGfxTiles[CARD_GFX_COUNT][2][9] = 
{
    [CARD_GFX_HIDDEN] = 
    {
        // Full hidden
        {
            0x06, 0x07, 0x06 | FLIP_HORIZONTAL,
            0x10, 0x11, 0x10 | FLIP_HORIZONTAL,
            0x06 | FLIP_VERTICAL, 0x07 | FLIP_VERTICAL, 0x06 | FLIP_HORIZONTAL | FLIP_VERTICAL,
        },
        // 1/3 hidden
        {
            0x63, 0x64, 0x65,
            0x66, 0x67, 0x68,
            0x69, 0x6A, 0x63 | FLIP_HORIZONTAL | FLIP_VERTICAL,
        }
    },
    [CARD_GFX_SHOW_1] = 
    {
        // 1/3 unhidden
        {
            0x6F, 0x70, 0x71,
            0x72, 0x73, 0x72 | FLIP_HORIZONTAL,
            0x71 | FLIP_HORIZONTAL | FLIP_VERTICAL, 0x74, 0x75,
        },
        // Full unhidden
        {
            0x25, 0x26, 0x25 | FLIP_HORIZONTAL,
            0x30, 0x47, 0x30 | FLIP_HORIZONTAL,
            0x25 | FLIP_VERTICAL, 0x26 | FLIP_VERTICAL, 0x25 | FLIP_HORIZONTAL | FLIP_VERTICAL,
        }
    },
    [CARD_GFX_SHOW_2] = 
    {
        // 1/3 unhidden
        {
            0x76, 0x77, 0x78,
            0x79, 0x7A, 0x79 | FLIP_HORIZONTAL,
            0x78 | FLIP_HORIZONTAL | FLIP_VERTICAL, 0x7B, 0x7C,
        },
        // Full unhidden
        {
            0x27, 0x28, 0x27 | FLIP_HORIZONTAL,
            0x32, 0x48, 0x32 | FLIP_HORIZONTAL,
            0x27 | FLIP_VERTICAL, 0x28 | FLIP_VERTICAL, 0x27 | FLIP_HORIZONTAL | FLIP_VERTICAL,
        }
    },
    [CARD_GFX_SHOW_3] = 
    {
        // 1/3 unhidden
        {
            0x76, 0x77, 0x78,
            0x79, 0x7D, 0x79 | FLIP_HORIZONTAL,
            0x78 | FLIP_HORIZONTAL | FLIP_VERTICAL, 0x7E, 0x7C,
        },
        // Full unhidden
        {
            0x27, 0x28, 0x27 | FLIP_HORIZONTAL,
            0x32, 0x49, 0x32 | FLIP_HORIZONTAL,
            0x27 | FLIP_VERTICAL, 0x28 | FLIP_VERTICAL, 0x27 | FLIP_HORIZONTAL | FLIP_VERTICAL,
        }
    },
    [CARD_GFX_SHOW_VOLTORB] = 
    {
        // 1/3 unhidden
        {
            0x7F, 0x80, 0x81,
            0x82, 0x83, 0x84,
            0x81 | FLIP_HORIZONTAL | FLIP_VERTICAL, 0x85, 0x86,
        },
        // Full unhidden
        {
            0x43, 0x44, 0x43 | FLIP_HORIZONTAL,
            0x4A, 0x4B, 0x4A | FLIP_HORIZONTAL,
            0x4F, 0x50, 0x4F | FLIP_HORIZONTAL,
        }
    },
};

// Card is in the middle of hidden/unhidden flip anim
static const u16 sCardGfxMidHiddenTiles[1][9] =
{
    {
        0x6B, 0x6C, 0x6B | FLIP_HORIZONTAL,
        0x6D, 0x6E, 0x6D | FLIP_HORIZONTAL,
        0x6B | FLIP_VERTICAL, 0x6C | FLIP_VERTICAL, 0x6B | FLIP_HORIZONTAL | FLIP_VERTICAL,
    }
};

// For the flip anim
static const u8 sCardFlipGfxStages[2][3] =
{
    // Hide
    [FALSE] =
    {
        CARD_GFX_SHOW_FROM_CARD,
        CARD_GFX_MID_HIDDEN,
        CARD_GFX_HIDDEN
    },
    // Show
    [TRUE] =
    {
        CARD_GFX_HIDDEN,
        CARD_GFX_MID_HIDDEN,
        CARD_GFX_SHOW_FROM_CARD
    }
};

//////////////////////////////////////////
// INITIALIZATION AND PRINTER FUNCTIONS //
//////////////////////////////////////////

void ShowVoltorbFlip(void)
{
    gMain.savedCallback = CB2_ReturnToField;
    SetMainCallback2(CB2_ShowVoltorbFlip);
    ScriptContext2_Enable();
}

static void VBlankCB_VoltorbFlip(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void MainCB2_VoltorbFlip(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void Task_VoltorbFlip_WaitFadeIn(u32 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_VoltorbFlip_HandleInput;
}

static void CB2_ShowVoltorbFlip(void)
{
    switch (gMain.state)
    {
        case 0:
            sVoltorbFlipState = AllocZeroed(sizeof(struct VoltorbFlipState));
            sVoltorbFlipState->numCoins = GetCoins();
            sVoltorbFlipState->level = 1;
            break;
        case 1:
            SetVBlankCallback(NULL);
            break;
        case 2:
            ClearScheduledBgCopiesToVram();
            break;
        case 3:
            ScanlineEffect_Stop();
            ResetTasks();
            ResetSpriteData();
            ResetPaletteFade();
            FreeAllSpritePalettes();
            break;
        case 4:
            ResetVramOamAndBgCntRegs();
            ResetAllBgsPos();
            ResetBgsAndClearDma3BusyFlags(FALSE);
            InitBgsFromTemplates(0, sVoltorbFlipBgTemplates, ARRAY_COUNT(sVoltorbFlipBgTemplates));
            SetBgTilemapBuffer(1, sVoltorbFlipState->tilemapBuffer);
            CopyBgTilemapBufferToVram(1);
            break;
        case 5:
            SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
            SetGpuReg(REG_OFFSET_BLDCNT, 0);
            break;
        case 6:
            ShowBg(0);
            ShowBg(1);
            ShowBg(2);
            ShowBg(3);
            break;
        case 7:
            ResetTempTileDataBuffers();
            DecompressAndCopyTileDataToVram(1, sVoltorbFlipTiles, 0, 0, 0);
            break;
        case 8:
            if (FreeTempTileDataBuffersIfPossible())
                return;
            break;
        case 9:
            LZDecompressWram(sVoltorbFlipTilemap, sVoltorbFlipState->tilemapBuffer);
            break;
        case 10:
            LoadPalette(sVoltorbFlipPalette, 0, 0x20);
            break;
        case 11:
            InitVoltorbFlipWindows();
            break;
        case 12:
            InitVoltorbFlipSprites();
            PrintPlayerAndCollectedCoinsText();
            PrintVoltorbFlipLevel();
            ResetVoltorbFlipCards();
            InitVoltorbFlipRound();
            break;
        case 13:
            BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
            break;
        case 14:
            BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
            break;
        case 15:
            SetVBlankCallback(VBlankCB_VoltorbFlip);
            SetMainCallback2(MainCB2_VoltorbFlip);
            CreateTask(Task_VoltorbFlip_WaitFadeIn, 0);
            return;
    }
    gMain.state++;
}

static void InitVoltorbFlipWindows(void)
{
    InitWindows(sVoltorbFlipWinTemplates);
    DeactivateAllTextPrinters();
    LoadStdWindowFrameGfx();
    FillWindowPixelBuffer(WIN_INFO_RED_X, PIXEL_FILL(0));
    PutWindowTilemap(WIN_INFO_RED_X);
    FillWindowPixelBuffer(WIN_INFO_BLACK_BACKGROUND, PIXEL_FILL(15));
    PutWindowTilemap(WIN_INFO_BLACK_BACKGROUND);
    CopyWindowToVram(WIN_INFO_BLACK_BACKGROUND, COPYWIN_BOTH);
    PrintInstructionsTexts();
}

static void PrintVoltorbFlipText(u32 windowId, u32 fontId, const u8 *text, u32 x, u32 y, u32 lineSpacing, bool32 fillWindow, u32 whichColor)
{
    if (fillWindow)
        FillWindowPixelBuffer(windowId, PIXEL_FILL(0));
    
    AddTextPrinterParameterized4(windowId, fontId, x, y, 0, lineSpacing, sTextColors[whichColor], 0xFF, text);
    PutWindowTilemap(windowId);
    CopyWindowToVram(windowId, COPYWIN_BOTH);
}

static u32 CountRow(u32 column, bool32 countValue)
{
    u32 row, total = 0;
    
    for (row = 0; row < BOARD_WIDTH; row++)
    {
        u32 value = sVoltorbFlipState->cardStates[COORDS_TO_CARD(row, column)].value;
        
        if (countValue)
        {
            if (value != CARD_VALUE_VOLTORB)
                total += CARD_TO_MULTIPLIER(value);
        }
        else
        {
            if (value == CARD_VALUE_VOLTORB)
                total++;
        }
    }
    return total;
}

static u32 CountColumn(u32 row, bool32 countValue)
{
    u32 col, total = 0;
    
    for (col = 0; col < BOARD_HEIGHT; col++)
    {
        u32 value = sVoltorbFlipState->cardStates[COORDS_TO_CARD(row, col)].value;
        
        if (countValue)
        {
            if (value != CARD_VALUE_VOLTORB)
                total += CARD_TO_MULTIPLIER(value);
        }
        else
        {
            if (value == CARD_VALUE_VOLTORB)
                total++;
        }
    }
    return total;
}

static u32 GetNumCardsFlippedAt(u32 pos, bool32 countRow)
{
    u32 i, total = 0;
    
    if (!countRow)
    {
        for (i = 0; i < BOARD_HEIGHT; i++)
        {
            if (sVoltorbFlipState->cardStates[COORDS_TO_CARD(pos, i)].isShown)
                total++;
        }
    }
    else
    {
        for (i = 0; i < BOARD_WIDTH; i++)
        {
            if (sVoltorbFlipState->cardStates[COORDS_TO_CARD(i, pos)].isShown)
                total++;
        }
    }
    return total;
}

#define PLACE_VOLTORB_FLIP_CARDS(count, card)                         \
    i = 0;                                                            \
                                                                      \
    do                                                                \
    {                                                                 \
        u32 idx = RandomMax(TOTAL_CARD_COUNT);                        \
                                                                      \
        if (sVoltorbFlipState->cardStates[idx].value == CARD_VALUE_1) \
        {                                                             \
            sVoltorbFlipState->cardStates[idx].value = card;          \
            i++;                                                      \
        }                                                             \
    } while (i < sVoltorbSpawnCounts[level - 1][variant].count)

static void ResetVoltorbFlipCards(void)
{
    u32 i;
    u32 level = sVoltorbFlipState->level;
    u32 variant = RandomMax(MAX_VOLTORB_LEVEL_VARIANTS);
    
    // Reset cards
    for (i = 0; i < TOTAL_CARD_COUNT; ++i)
    {
        memset(&sVoltorbFlipState->cardStates[i], 0, sizeof(struct VoltorbCardState));
        sVoltorbFlipState->cardStates[i].value = CARD_VALUE_1;
    }
    
    // Place cards
    PLACE_VOLTORB_FLIP_CARDS(voltorbCount, CARD_VALUE_VOLTORB); // Place Voltorbs
    PLACE_VOLTORB_FLIP_CARDS(x2Count, CARD_VALUE_2); // Place x2
    PLACE_VOLTORB_FLIP_CARDS(x3Count, CARD_VALUE_3); // Place x3
}

static void PrintRowInfo(u32 windowId, u32 totalValue, u32 totalVoltorbs)
{
    // Top number
    ConvertIntToDecimalStringN(gStringVar1, totalValue, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("{COLOR WHITE}{SHADOW DARK_GRAY}{STR_VAR_1}"));
    PrintVoltorbFlipText(windowId, FONT_SMALL_NARROWER, gStringVar4, 11, 0, 0, TRUE, 0);
    
    // Bottom number
    ConvertIntToDecimalStringN(gStringVar1, totalVoltorbs, STR_CONV_MODE_RIGHT_ALIGN, 1);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("{COLOR LIGHT_GRAY}{SHADOW DARK_GRAY}{STR_VAR_1}"));
    PrintVoltorbFlipText(windowId, FONT_SMALL_NARROWER, gStringVar4, 15, 8, 0, FALSE, 0);
}

static void DisplayNumCardsAndNumVoltorbs(void)
{
    u32 i;
    
    for (i = 0; i < BOARD_HEIGHT; i++)
        PrintRowInfo(WIN_INFO_RED_X + i, CountRow(i, TRUE), CountRow(i, FALSE));
    
    for (i = 0; i < BOARD_WIDTH; i++)
        PrintRowInfo(WIN_INFO_RED_Y + i, CountColumn(i, TRUE), CountColumn(i, FALSE));
}

static void ZeroNumCardsAndNumVoltorbs(void)
{
    u32 i;
    
    for (i = 0; i < BOARD_HEIGHT; i++)
        PrintRowInfo(WIN_INFO_RED_X + i, 0, 0);
    
    for (i = 0; i < BOARD_WIDTH; i++)
        PrintRowInfo(WIN_INFO_RED_Y + i, 0, 0);
}

static void UpdateCoinsDisplay(u32 windowId, u32 value)
{
    FillWindowPixelRect(windowId, PIXEL_FILL(0), 0, 24, 5 * 8, 2 * 8);
    ConvertIntToDecimalStringN(gStringVar4, value, STR_CONV_MODE_LEADING_ZEROS, 5);
    PrintVoltorbFlipText(windowId, FONT_NORMAL, gStringVar4, 8, 24, 0, FALSE, 0);
}

static void PrintPlayerAndCollectedCoinsText(void)
{
    // Player coins
    StringCopy(gStringVar1, gSaveBlock2Ptr->playerName);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("{STR_VAR_1}'s\nCoins:"));
    PrintVoltorbFlipText(WIN_INFO_NUM_COINS, FONT_SMALL_NARROWER, gStringVar4, 4, 1, 2, TRUE, 0);
    UpdateCoinsDisplay(WIN_INFO_NUM_COINS, sVoltorbFlipState->numCoins);

    // Collected coins
    PrintVoltorbFlipText(WIN_INFO_COLLECTED_COINS, FONT_SMALL_NARROWER, COMPOUND_STRING("Collected\nCoins:"), 4, 1, 2, TRUE, 0);
    UpdateCoinsDisplay(WIN_INFO_COLLECTED_COINS, sVoltorbFlipState->numCollectedCoins);
}

static void PrintInstructionsTexts(void)
{
    PrintVoltorbFlipText(WIN_INFO_LR_BUTTONS, FONT_SMALL_NARROWER, COMPOUND_STRING("{L_BUTTON}{R_BUTTON}"), 5, 3, 0, TRUE, 0);
    PrintVoltorbFlipText(WIN_INFO_QUIT, FONT_NORMAL, COMPOUND_STRING("Quit"), 10, 1, 0, TRUE, 2);
}

static void PrintVoltorbFlipLevel(void)
{
    ConvertIntToDecimalStringN(gStringVar1, sVoltorbFlipState->level, STR_CONV_MODE_LEFT_ALIGN, 1);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("Lv. {STR_VAR_1}"));
    PrintVoltorbFlipText(WIN_INFO_LEVEL, FONT_SMALL_NARROWER, gStringVar4, 4, 4, 0, TRUE, 1);
}

static void PrintVoltorbFlipTextInMessageBox(bool32 useMiniBox)
{
    if (useMiniBox)
    {
        SetWindowAttribute(WIN_INFO_MESSAGE, WINDOW_TILEMAP_TOP, 17);
        SetWindowAttribute(WIN_INFO_MESSAGE, WINDOW_HEIGHT, 2);
    }
    else
    {
        SetWindowAttribute(WIN_INFO_MESSAGE, WINDOW_TILEMAP_TOP, 15);
        SetWindowAttribute(WIN_INFO_MESSAGE, WINDOW_HEIGHT, 4);
    }
    DrawStdWindowFrame(WIN_INFO_MESSAGE, TRUE);
    AddTextPrinterForMessage(TRUE);
}

static inline void CloseVoltorbFlipMessageBox(void)
{
    ClearStdWindowAndFrame(WIN_INFO_MESSAGE, TRUE);
}

static bool32 IsVoltorbFlipTextPrinterActive(bool32 closeBox)
{
    if (!RunTextPrinters_CheckPrinter0Active())
    {
        if (closeBox)
            CloseVoltorbFlipMessageBox();
        
        return FALSE;
    }
    return TRUE;
}

static void DrawNoteTiles(void)
{
    u32 x = sVoltorbFlipState->cursorX, y = sVoltorbFlipState->cursorY;
    struct VoltorbCardState *state = &sVoltorbFlipState->cardStates[COORDS_TO_CARD(x, y)];
    
    if (!state->isShown)
    {
        const u16 *gfxs = sCardGfxTiles[CARD_GFX_HIDDEN][0];
        
        FillBgTilemapBufferRect_Palette0(1, state->noteVoltorb ? 0x23 : gfxs[0], CARD_INITIAL_X_OFFSET + x * 3 + 0, CARD_INITIAL_Y_OFFSET + y * 3 + 0, 1, 1);
        FillBgTilemapBufferRect_Palette0(1, state->note1 ? 0x24 : gfxs[2], CARD_INITIAL_X_OFFSET + x * 3 + 2, CARD_INITIAL_Y_OFFSET + y * 3 + 0, 1, 1);
        FillBgTilemapBufferRect_Palette0(1, state->note2 ? 0x3A : gfxs[6], CARD_INITIAL_X_OFFSET + x * 3 + 0, CARD_INITIAL_Y_OFFSET + y * 3 + 2, 1, 1);
        FillBgTilemapBufferRect_Palette0(1, state->note3 ? 0x3B : gfxs[8], CARD_INITIAL_X_OFFSET + x * 3 + 2, CARD_INITIAL_Y_OFFSET + y * 3 + 2, 1, 1);
        
        ScheduleBgCopyTilemapToVram(1);
    }
}

static void ChangeNoteTilesSelectionCursor(void)
{
    u32 writeValue = sVoltorbFlipState->cursorWriteValue;
    FillBgTilemapBufferRect_Palette0(1, (writeValue == 0) ? 0x38 : 0x39, 22 + 0, 4, 1, 1);
    FillBgTilemapBufferRect_Palette0(1, (writeValue == 1 + CARD_VALUE_1) ? 0x37 : 0x39, 22 + 1, 4, 1, 1);
    FillBgTilemapBufferRect_Palette0(1, (writeValue == 1 + CARD_VALUE_2) ? 0x37 : 0x39, 22 + 2, 4, 1, 1);
    FillBgTilemapBufferRect_Palette0(1, (writeValue == 1 + CARD_VALUE_3) ? 0x37 : 0x39, 22 + 3, 4, 1, 1);
    FillBgTilemapBufferRect_Palette0(1, (writeValue == 1 + CARD_VALUE_VOLTORB) ? 0x37 : 0x39, 22 + 4, 4, 1, 1);
    ScheduleBgCopyTilemapToVram(1);
}

static void ChangeQuitButtonFrame(bool32 selected)
{
    u32 i;
    
    sVoltorbFlipState->isCursorOnQuitButton = selected;
    
    for (i = 0; i < 10; i++)
        FillBgTilemapBufferRect_Palette0(1, sQuitButtonGfxs[selected][i], 22 + (i % 5), 17 + (i / 5), 1, 1);
    
    ScheduleBgCopyTilemapToVram(1);
}

static void ChangePointerFromOrToQuitButton(bool32 moveToButton)
{
    SetPointerSpriteVisibility(moveToButton);
    ChangeQuitButtonFrame(moveToButton);
}

static void InitVoltorbFlipRound(void)
{
    sVoltorbFlipState->cursorX = 0;
    sVoltorbFlipState->cursorY = 0;
    sVoltorbFlipState->cursorWriteValue = 0;
    sVoltorbFlipState->numCardsFlipped = 0;
    sVoltorbFlipState->loseRound = FALSE;
    ChangeQuitButtonFrame(FALSE);
    DisplayNumCardsAndNumVoltorbs();
    ChangeNoteTilesSelectionCursor();
}

static void PlaySE1(u32 se)
{
    m4aMPlayStop(&gMPlayInfo_SE1);
    PlaySE1WithPanning(se, 0);
}

static void PlaySE2(u32 se)
{
    m4aMPlayStop(&gMPlayInfo_SE2);
    PlaySE2WithPanning(se, 0);
}

/////////////
// SPRITES //
/////////////

static void InitVoltorbFlipSprites(void)
{
    LoadSpriteSheet(&sVoltorbFlipSpriteSheet);
    LoadSpritePalette(&sVoltorbFlipSpritePalette);
    
    LoadSpriteSheet(&sExplosionSpriteSheet);
    LoadSpritePalette(&sExplosionSpritePalette);
    
    sVoltorbFlipState->pointerSpriteId = CreateSprite(&sPointerSpriteTemplate, 24 + 12, 8 + 0, 0);
    sVoltorbFlipState->pointerOutlineSpriteId = CreateSprite(&sPointerSpriteTemplate, 0, 0, 1);
    
    UpdateVoltorbFlipSprites();
}

static void UpdateVoltorbFlipSprites(void)
{
    gSprites[sVoltorbFlipState->pointerSpriteId].x = 24 + 12 + 24 * sVoltorbFlipState->cursorX;
    gSprites[sVoltorbFlipState->pointerSpriteId].y = 8 + 0 + 24 * sVoltorbFlipState->cursorY;
    
    gSprites[sVoltorbFlipState->pointerOutlineSpriteId].x = 24 + 16 + 24 * sVoltorbFlipState->cursorX;
    gSprites[sVoltorbFlipState->pointerOutlineSpriteId].y = 8 + 16 + 24 * sVoltorbFlipState->cursorY;
    
    if (gSprites[sVoltorbFlipState->pointerSpriteId].y2 > 0)
    {
        if (gSprites[sVoltorbFlipState->pointerSpriteId].y2 < 2)
            gSprites[sVoltorbFlipState->pointerSpriteId].y2 = 0;
        else
            gSprites[sVoltorbFlipState->pointerSpriteId].y2 -= 2;
    }
    
    if (sVoltorbFlipState->cursorWriteValue == 0)
    {
        StartSpriteAnimIfDifferent(&gSprites[sVoltorbFlipState->pointerSpriteId], 0);
        StartSpriteAnimIfDifferent(&gSprites[sVoltorbFlipState->pointerOutlineSpriteId], 2);
    }
    else
    {
        StartSpriteAnimIfDifferent(&gSprites[sVoltorbFlipState->pointerSpriteId], 1);
        StartSpriteAnimIfDifferent(&gSprites[sVoltorbFlipState->pointerOutlineSpriteId], 3);
    }
}

static void SetPointerSpriteVisibility(bool32 invisible)
{
    gSprites[sVoltorbFlipState->pointerSpriteId].invisible = gSprites[sVoltorbFlipState->pointerOutlineSpriteId].invisible = invisible;
}

static void CreateExplosionSpriteAt(u32 x, u32 y)
{
    u32 spriteId = CreateSprite(&sVoltorbExplosionSpriteTemplate, 36 + 24 * x, 20 + 24 * y, 0);
    gSprites[spriteId].invisible = TRUE;
    gSprites[spriteId].animPaused = TRUE;
    sVoltorbFlipState->explodingVoltorb = TRUE;
}

static void SpriteCB_VoltorbExplosion(struct Sprite *sprite)
{
    switch (sprite->data[0])
    {
        case 0:
            if (!sVoltorbFlipState->flippingCard) // Wait card flip
                sprite->data[0]++;
            break;
        case 1:
            PlaySE1(SE_M_EXPLOSION);
            sprite->invisible = FALSE;
            sprite->animPaused = FALSE;
            sprite->data[0]++;
            break;
        case 2:
            if (sprite->animEnded)
            {
                sprite->invisible = TRUE;
                sprite->data[0]++;
            }
            break;
        case 3:
            if (!IsSEPlaying())
            {
                DestroySprite(sprite);
                sVoltorbFlipState->explodingVoltorb = FALSE;
            }
            break;
    }
}

////////////////////////////
// LEVEL CHANGE FUNCTIONS //
////////////////////////////

static void AdvanceVoltorbFlipLevel(void)
{
    sVoltorbFlipState->level = min(sVoltorbFlipState->level + 1, MAX_VOLTORB_FLIP_LEVEL);
}

static void TryRegressVoltorbFlipLevel(void)
{
    if (sVoltorbFlipState->level > 1)
    {
        u32 cardsFlipped = sVoltorbFlipState->numCardsFlipped;
        
        if (sVoltorbFlipState->loseRound)
            cardsFlipped--; // Lose due to flipping a Voltorb, ignores this card
        
        if (cardsFlipped < sVoltorbFlipState->level)
            sVoltorbFlipState->level = max(1, cardsFlipped);
    }
}

///////////
// INPUT //
///////////

#define tInputDisabled data[0]

static void DisableMainTaskInput(u32 mainTaskId)
{
    gTasks[mainTaskId].tInputDisabled = TRUE;
}

static void EnableMainTaskInput(u32 mainTaskId)
{
    gTasks[mainTaskId].tInputDisabled = FALSE;
}

static inline bool32 IsMainTaskInputDisabled(u32 mainTaskId)
{
    return gTasks[mainTaskId].tInputDisabled;
}

#undef tInputDisabled

static u32 CalculateBoardState(void)
{
    u32 i;
    bool32 inProgress = FALSE;
    
    for (i = 0; i < TOTAL_CARD_COUNT; i++)
    {
        u32 value = sVoltorbFlipState->cardStates[i].value;
        
        if (sVoltorbFlipState->cardStates[i].isShown)
        {
            if (value == CARD_VALUE_VOLTORB)
            {
                sVoltorbFlipState->loseRound = TRUE;
                return GAME_STATE_LOSE;
            }
        }
        else
        {
            if (value == CARD_VALUE_2 || value == CARD_VALUE_3)
                inProgress = TRUE;
        }
    }
    
    if (!inProgress)
        return GAME_STATE_WIN;
    
    if (sVoltorbFlipState->numCoins == MAX_COLLECTED_COINS)
        return GAME_STATE_MAX_COINS;
    
    return GAME_STATE_IN_PROGRESS;
}

static void Task_VoltorbFlip_HandleInput(u32 taskId)
{
    if (!IsMainTaskInputDisabled(taskId) && !sVoltorbFlipState->flippingCard)
    {
        switch (CalculateBoardState())
        {
            case GAME_STATE_WIN:
                CreateGameClearSequenceTask(taskId);
                return;
            case GAME_STATE_LOSE:
                CreateGameLoseSequenceTask(taskId);
                return;
            case GAME_STATE_MAX_COINS:
                CreateEarnedMaxCoinsSequenceTask(taskId);
                return;
            case GAME_STATE_IN_PROGRESS:
                if (!sVoltorbFlipState->isCursorOnQuitButton) // Normal input
                {
                    if (JOY_NEW(A_BUTTON))
                    {
                        struct VoltorbCardState *state = &sVoltorbFlipState->cardStates[COORDS_TO_CARD(sVoltorbFlipState->cursorX, sVoltorbFlipState->cursorY)];
                        
                        if (state->isShown)
                            PlaySE1(SE_FAILURE);
                        else
                        {
                            switch (sVoltorbFlipState->cursorWriteValue)
                            {
                                case 0:
                                    if (sVoltorbFlipState->numCardsFlipped < TOTAL_CARD_COUNT)
                                        sVoltorbFlipState->numCardsFlipped++;
                                    
                                    StartCardFlipSequence(TRUE, FALSE);
                                    break;
                                case 1 + CARD_VALUE_1:
                                    PlaySE1(SE_BALL);
                                    state->note1 ^= TRUE;
                                    DrawNoteTiles();
                                    break;
                                case 1 + CARD_VALUE_2:
                                    PlaySE1(SE_BALL);
                                    state->note2 ^= TRUE;
                                    DrawNoteTiles();
                                    break;
                                case 1 + CARD_VALUE_3:
                                    PlaySE1(SE_BALL);
                                    state->note3 ^= TRUE;
                                    DrawNoteTiles();
                                    break;
                                case 1 + CARD_VALUE_VOLTORB:
                                    PlaySE1(SE_BALL);
                                    state->noteVoltorb ^= TRUE;
                                    DrawNoteTiles();
                                    break;
                            }
                            gSprites[sVoltorbFlipState->pointerSpriteId].y2 = 12;
                        }
                    }
                    else if (JOY_NEW(B_BUTTON))
                    {
                        PlaySE2(SE_SELECT);
                        ChangePointerFromOrToQuitButton(TRUE);
                        CreateGameQuitSequenceTask(taskId);
                        return;
                    }
                    else if (JOY_NEW(DPAD_LEFT))
                    {
                        if (sVoltorbFlipState->cursorX > 0)
                        {
                            --sVoltorbFlipState->cursorX;
                            PlaySE2(SE_DEX_SCROLL);
                        }
                    }
                    else if (JOY_NEW(DPAD_RIGHT))
                    {
                        PlaySE2(SE_DEX_SCROLL);
                        
                        if (sVoltorbFlipState->cursorX == BOARD_WIDTH - 1)
                            ChangePointerFromOrToQuitButton(TRUE);
                        else
                            ++sVoltorbFlipState->cursorX;
                    }
                    else if (JOY_NEW(DPAD_UP))
                    {
                        if (sVoltorbFlipState->cursorY > 0)
                        {
                            --sVoltorbFlipState->cursorY;
                            PlaySE2(SE_DEX_SCROLL);
                        }
                    }
                    else if (JOY_NEW(DPAD_DOWN))
                    {
                        PlaySE2(SE_DEX_SCROLL);
                        
                        if (sVoltorbFlipState->cursorY == BOARD_HEIGHT - 1)
                            ChangePointerFromOrToQuitButton(TRUE);
                        else
                            ++sVoltorbFlipState->cursorY;
                    }
                    else if (JOY_NEW(L_BUTTON))
                    {
                        if (sVoltorbFlipState->cursorWriteValue > 0)
                        {
                            --sVoltorbFlipState->cursorWriteValue;
                            PlaySE2(SE_SELECT);
                            ChangeNoteTilesSelectionCursor();
                        }
                    }
                    else if (JOY_NEW(R_BUTTON))
                    {
                        if (sVoltorbFlipState->cursorWriteValue < CARD_VALUE_COUNT)
                        {
                            ++sVoltorbFlipState->cursorWriteValue;
                            PlaySE2(SE_SELECT);
                            ChangeNoteTilesSelectionCursor();
                        }
                    }
                }
                else // Quit button input
                {
                    if (JOY_NEW(A_BUTTON))
                    {
                        PlaySE2(SE_SELECT);
                        CreateGameQuitSequenceTask(taskId);
                        return;
                    }
                    else if (JOY_NEW(B_BUTTON))
                    {
                        PlaySE2(SE_SELECT);
                        ChangePointerFromOrToQuitButton(FALSE);
                    }
                    else if (JOY_NEW(DPAD_UP | DPAD_LEFT))
                    {
                        PlaySE2(SE_DEX_SCROLL);
                        ChangePointerFromOrToQuitButton(FALSE);
                    }
                }
                break;
        }
    }
    UpdateVoltorbFlipSprites();
}

///////////////////////////
// COIN CHANGE ANIMATION //
///////////////////////////

#define COINS_UPDATE_SPEED 3 // Update coin value every 3 milliseconds

// Change coins display depending on the N. of digits, how higher the value more fast it changes
static u32 GetCoinsChangeValue(u32 value)
{
    return sDigitsToCoinChangeValue[CountDigits(value) - 1];
}

static bool32 ChangeCoins(u32 windowId, u16 *dest, u32 targetValue, s32 coinChangeVal)
{
    bool32 decrease = (coinChangeVal < 0), inProgress = (*dest != targetValue);
    
    if (inProgress)
    {
        u32 prevVal = *dest;
        s16 newVal = *dest + coinChangeVal;
        
        if (decrease)
        {
            if (newVal < targetValue)
                newVal = targetValue;
        }
        else
        {
            if (newVal > targetValue)
                newVal = targetValue;
        }
        *dest = newVal;
        
        UpdateCoinsDisplay(windowId, *dest);
        
        if (!decrease)
        {
            u32 soundChangeVal = coinChangeVal * 10;
            
            if (prevVal < ((prevVal / soundChangeVal + 1) * soundChangeVal) && *dest >= ((prevVal / soundChangeVal + 1) * soundChangeVal))
                PlaySE(SE_RS_SHOP);
        }
    }
    else
    {
        if (!decrease)
        {
            if (targetValue <= coinChangeVal * 10)
                PlaySE(SE_RS_SHOP);
        }
    }
    return inProgress;
}

////////////////////
// GAME SEQUENCES //
////////////////////

#define tState       data[0]
#define tTargetValue data[1]
#define tTimerDelay  data[2]
#define tDelay       data[3]
#define tMainTaskId  data[4]
#define tShowCard    data[5]
#define tCardGfxId   data[6]
#define tCardGfxId2  data[7]
#define tAllCards    data[8]
#define tStartX      data[9]
#define tStartY      data[10]
#define tFlipStage   data[11]

static void Task_RunCardFlipSequence(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    
    if (sCardFlipSequenceFuncs[tState](data))
    {
        sVoltorbFlipState->flippingCard = FALSE;
        DestroyTask(taskId);
    }
}

#define CARD_FLIP_DELAY 4 // Change card frame every 4 milliseconds

static void StartCardFlipSequence(bool32 show, bool32 allCards)
{
    s16 *data = gTasks[CreateTask(Task_RunCardFlipSequence, 0)].data;
    
    tShowCard = show;
    tCardGfxId = tCardGfxId2 = show ? 1 : 0;
    tAllCards = allCards;
    
    if (allCards)
    {
        tTimerDelay = CARD_FLIP_DELAY / 2; // Flip fast if flipping multiple cards
        tStartX = 0;
        tStartY = 0;
    }
    else
    {
        tTimerDelay = CARD_FLIP_DELAY;
        tStartX = sVoltorbFlipState->cursorX;
        tStartY = sVoltorbFlipState->cursorY;
    }
    sVoltorbFlipState->flippingCard = TRUE;
}

static void Task_VoltorbFlip_RunGameClearSequence(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    
    if (sGameClearSequenceFuncs[tState](data))
        DestroyTask(taskId);
}

static void CreateGameClearSequenceTask(u32 mainTaskId)
{
    s16 *data = gTasks[CreateTask(Task_VoltorbFlip_RunGameClearSequence, 0)].data;
    tTargetValue = min(sVoltorbFlipState->numCoins + sVoltorbFlipState->numCollectedCoins, MAX_COLLECTED_COINS);
    tMainTaskId = mainTaskId;
    DisableMainTaskInput(mainTaskId);
}

static void Task_VoltorbFlip_RunGameLoseSequence(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    
    if (sGameLoseSequenceFuncs[tState](data))
        DestroyTask(taskId);
}

static void CreateGameLoseSequenceTask(u32 mainTaskId)
{
    gTasks[CreateTask(Task_VoltorbFlip_RunGameLoseSequence, 0)].tMainTaskId = mainTaskId;
    DisableMainTaskInput(mainTaskId);
}

static void Task_VoltorbFlip_RunGameQuitSequence(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    
    if (sGameQuitSequenceFuncs[tState](data))
        DestroyTask(taskId);
}

static void CreateGameQuitSequenceTask(u32 mainTaskId)
{
    s16 *data = gTasks[CreateTask(Task_VoltorbFlip_RunGameQuitSequence, 0)].data;
    tTargetValue = min(sVoltorbFlipState->numCoins + sVoltorbFlipState->numCollectedCoins, MAX_COLLECTED_COINS);
    tMainTaskId = mainTaskId;
    DisableMainTaskInput(mainTaskId);
}

static void Task_VoltorbFlip_RunEarnedMaxCoinsSequence(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    
    if (sEarnedMaxCoinsSequenceFuncs[tState](data))
        DestroyTask(taskId);
}

static void CreateEarnedMaxCoinsSequenceTask(u32 mainTaskId)
{
    gTasks[CreateTask(Task_VoltorbFlip_RunEarnedMaxCoinsSequence, 0)].tMainTaskId = mainTaskId;
    DisableMainTaskInput(mainTaskId);
}

#define VOLTORB_FLIP_BLEND_PALETTES ~(Bit(14)) // All palettes, except slot 14

static bool32 VFSequence_FadeOutBgPalettes(s16 *data)
{
    BeginNormalPaletteFade(VOLTORB_FLIP_BLEND_PALETTES, 4, 0, 6, RGB_BLACK);
    ++tState;
    return FALSE;
}

static bool32 VFSequence_FadeInBgPalettes(s16 *data)
{
    BeginNormalPaletteFade(VOLTORB_FLIP_BLEND_PALETTES, 4, 6, 0, RGB_BLACK);
    tState++;
    return FALSE;
}

static bool32 VFSequence_FadeOutExceptionPalettes(s16 *data)
{
    BeginNormalPaletteFade(~(VOLTORB_FLIP_BLEND_PALETTES), 0, 0, 6, RGB_BLACK);
    tState++;
    return FALSE;
}

static bool32 VFSequence_FadeToExit(s16 *data)
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 6, 16, RGB_BLACK);
    tState++;
    return FALSE;
}

static bool32 VFSequence_WaitPaletteFade(s16 *data)
{
    if (!gPaletteFade.active)
        tState++;
    return FALSE;
}

static bool32 VFSequence_PrintGameClearText(s16 *data)
{
    PlayFanfare(MUS_OBTAIN_ITEM);
    StringCopy(gStringVar4, COMPOUND_STRING("Game clear!\p"
                                            "You've found all of the hidden x2 and\nx3 cards.\p"
                                            "This means you've found all the Coins\nin this game, so the game is now over.\p"));
    PrintVoltorbFlipTextInMessageBox(FALSE);
    tState++;
    return FALSE;
}

static bool32 VFSequence_PrintReceivedCoinsText(s16 *data)
{
    StringCopy(gStringVar1, gSaveBlock2Ptr->playerName);
    ConvertIntToDecimalStringN(gStringVar2, tTargetValue, STR_CONV_MODE_LEFT_ALIGN, 5);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("{STR_VAR_1} received {STR_VAR_2} Coin(s)!"));
    PrintVoltorbFlipTextInMessageBox(FALSE);
    tState++;
    return FALSE;
}

static bool32 VFSequence_PrintAdvancedLevelText(s16 *data)
{
    PlayFanfare(MUS_LEVEL_UP);
    ConvertIntToDecimalStringN(gStringVar1, sVoltorbFlipState->level, STR_CONV_MODE_LEFT_ALIGN, 1);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("Advanced to Game Lv. {STR_VAR_1}!\l"
                                                          "Congratulations!\p"
                                                          "You can receive even more Coins\nin the next game!\p"));
    PrintVoltorbFlipTextInMessageBox(FALSE);
    tState++;
    return FALSE;
}

static bool32 VFSequence_PrintAskPlayLevelText(s16 *data)
{
    ConvertIntToDecimalStringN(gStringVar1, sVoltorbFlipState->level, STR_CONV_MODE_LEFT_ALIGN, 1);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("Play Voltorb Flip Lv. {STR_VAR_1}?"));
    PrintVoltorbFlipTextInMessageBox(FALSE);
    tState++;
    return FALSE;
}

static bool32 VFSequence_PrintGet0CoinsText(s16 *data)
{
    PlayFanfare(MUS_TOO_BAD);
    StringCopy(gStringVar4, COMPOUND_STRING("Oh no! You get 0 Coins!"));
    PrintVoltorbFlipTextInMessageBox(TRUE);
    tState++;
    return FALSE;
}

static bool32 VFSequence_PrintAskQuitGameText(s16 *data)
{
    if (sVoltorbFlipState->numCollectedCoins == 0)
        StringCopy(gStringVar4, COMPOUND_STRING("You haven't found any Coins!\nAre you sure you want to quit?"));
    else
    {
        ConvertIntToDecimalStringN(gStringVar1, sVoltorbFlipState->numCollectedCoins, STR_CONV_MODE_LEFT_ALIGN, 5);
        StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("If you quit now, you will\nreceive {STR_VAR_1} Coin(s).\pWill you quit?"));
    }
    PrintVoltorbFlipTextInMessageBox(FALSE);
    tState++;
    return FALSE;
}

static bool32 VFSequence_TryPrintCoinMultiplierText(s16 *data)
{
    if (!tAllCards)
    {
        u32 cardValue = sVoltorbFlipState->cardStates[COORDS_TO_CARD(tStartX, tStartY)].value;
        
        if (cardValue != CARD_VALUE_VOLTORB)
        {
            u32 multiplier = CARD_TO_MULTIPLIER(cardValue);
            bool32 firstCard = (sVoltorbFlipState->numCardsFlipped == 1);
            bool32 updateDisplay = FALSE;
        
            // First card flipped, set coins to multiplier's value
            if (firstCard)
            {
                updateDisplay = TRUE;
                tTargetValue = multiplier;
            }
            else
            {
                // Only update display if its a x2 or x3 card
                if (multiplier > CARD_TO_MULTIPLIER(CARD_VALUE_1))
                {
                    updateDisplay = TRUE;
                    tTargetValue = multiplier * sVoltorbFlipState->numCollectedCoins;
                }
            }
            
            if (updateDisplay)
            {
                ConvertIntToDecimalStringN(gStringVar1, multiplier, STR_CONV_MODE_LEFT_ALIGN, 1);
                
                if (firstCard)
                    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("{STR_VAR_1}! Received {STR_VAR_1} Coin(s)!"));
                else
                {
                    ConvertIntToDecimalStringN(gStringVar2, tTargetValue, STR_CONV_MODE_LEFT_ALIGN, 5);
                    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("x{STR_VAR_1}! Received {STR_VAR_2} Coins!"));
                }
                PrintVoltorbFlipTextInMessageBox(TRUE);
                tState++;
                return FALSE;
            }
        }
    }
    tState += 3;
    return FALSE;
}

static bool32 VFSequence_TryPrintWhatExpectingText(s16 *data)
{
    if (!tAllCards)
    {
        u32 voltorbsInRow = CountRow(tStartY, FALSE);
        u32 voltorbsInCol = CountColumn(tStartX, FALSE);
        
        if (voltorbsInRow > 0 && voltorbsInCol > 0)
        {
            u32 flippedCardsInRow = GetNumCardsFlippedAt(tStartY, TRUE);
            u32 flippedCardsInCol = GetNumCardsFlippedAt(tStartX, FALSE);
            
            if ((100 * voltorbsInRow / (BOARD_WIDTH - flippedCardsInRow)) >= 75 || (100 * voltorbsInCol / (BOARD_HEIGHT - flippedCardsInCol)) >= 75)
            {
                PlayFanfare(MUS_OBTAIN_BADGE);
                StringCopy(gStringVar4, COMPOUND_STRING("Is this what you're expecting?!"));
                PrintVoltorbFlipTextInMessageBox(TRUE);
                tState++;
                return FALSE;
            }
        }
    }
    tState = 2;
    return FALSE;
}

static bool32 VFSequence_PrintDroppedLevelText(s16 *data)
{
    ConvertIntToDecimalStringN(gStringVar1, sVoltorbFlipState->level, STR_CONV_MODE_LEFT_ALIGN, 1);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("Dropped to Game Lv. {STR_VAR_1}.\p"));
    PrintVoltorbFlipTextInMessageBox(FALSE);
    tState++;
    return FALSE;
}

static bool32 VFSequence_PrintGatheredMaxCoinsText(s16 *data)
{
    ConvertIntToDecimalStringN(gStringVar1, MAX_COLLECTED_COINS, STR_CONV_MODE_LEFT_ALIGN, 5);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("You've gathered {STR_VAR_1} Coins.\nYou cannot gather any more.\p"));
    PrintVoltorbFlipTextInMessageBox(FALSE);
    tState++;
    return FALSE;
}

static bool32 VFSequence_WaitText_CloseBox(s16 *data)
{
    if (!IsVoltorbFlipTextPrinterActive(TRUE))
        tState++;
    return FALSE;
}

static bool32 VFSequence_WaitText_DontCloseBox(s16 *data)
{
    if (!IsVoltorbFlipTextPrinterActive(FALSE))
        tState++;
    return FALSE;
}

static bool32 VFSequence_CloseMessageBox(s16 *data)
{
    CloseVoltorbFlipMessageBox();
    tState++;
    return FALSE;
}

static bool32 VFSequence_GiveCollectedCoinsToPlayer(s16 *data)
{
    u32 coinChangeVal;
    
    if (++tDelay >= COINS_UPDATE_SPEED)
    {
        tDelay = 0;
        coinChangeVal = GetCoinsChangeValue(sVoltorbFlipState->numCollectedCoins);
        
        ChangeCoins(WIN_INFO_COLLECTED_COINS, &sVoltorbFlipState->numCollectedCoins, 0, -coinChangeVal);
        
        if (!ChangeCoins(WIN_INFO_NUM_COINS, &sVoltorbFlipState->numCoins, tTargetValue, coinChangeVal))
        {
            SetCoins(tTargetValue);
            tState++;
        }
    }
    return FALSE;
}

static bool32 VFSequence_LoseCollectedCoins(s16 *data)
{
    sVoltorbFlipState->numCollectedCoins = 0;
    UpdateCoinsDisplay(WIN_INFO_COLLECTED_COINS, sVoltorbFlipState->numCollectedCoins);
    tState++;
    return FALSE;
}

static bool32 VFSequence_UpdateCollectedCoins(s16 *data)
{
    if (++tDelay >= COINS_UPDATE_SPEED)
    {
        tDelay = 0;
        
        if (!ChangeCoins(WIN_INFO_COLLECTED_COINS, &sVoltorbFlipState->numCollectedCoins, tTargetValue, GetCoinsChangeValue(sVoltorbFlipState->numCollectedCoins)))
            tState++;
    }
    return FALSE;
}

static bool32 VFSequence_FlipCardAnim(s16 *data)
{
    struct VoltorbCardState *state;
    bool32 nextCard;
    
    if (--tDelay <= 0)
    {
        tDelay = tTimerDelay;
        state = &sVoltorbFlipState->cardStates[COORDS_TO_CARD(tStartX, tStartY)];
        
        if (state->isShown != tShowCard)
        {
            const u16 *gfxs;
            u32 i, currGfx = sCardFlipGfxStages[tShowCard][tFlipStage];
            
            switch (currGfx)
            {
                case CARD_GFX_MID_HIDDEN:
                    gfxs = sCardGfxMidHiddenTiles[0];
                    break;
                case CARD_GFX_SHOW_FROM_CARD:
                    currGfx = CARD_GFX_SHOW_1 + state->value;
                    // Fallthrough
                default:
                    gfxs = sCardGfxTiles[currGfx][tCardGfxId];
                    break;
            }
            
            for (i = 0; i < 9; i++)
                FillBgTilemapBufferRect_Palette0(1, gfxs[i], CARD_INITIAL_X_OFFSET + tStartX * 3 + (i % 3), CARD_INITIAL_Y_OFFSET + tStartY * 3 + (i / 3), 1, 1);
            
            ScheduleBgCopyTilemapToVram(1);
            
            if (tFlipStage == 0)
            {
                PlaySE1(SE_CARD_FLIP);
                m4aMPlayVolumeControl(&gMPlayInfo_SE1, 0xFFFF, 125);
            }
            
            tCardGfxId += (tShowCard ? 1 : -1);
            if (tCardGfxId == 2 || tCardGfxId == -1)
            {
                if (currGfx == CARD_GFX_MID_HIDDEN)
                    tCardGfxId2 ^= 1;
                
                tCardGfxId = tCardGfxId2;
                tFlipStage++;
            }
            
            if (tFlipStage == 3)
            {
                tFlipStage = 0;
                tCardGfxId = tCardGfxId2 = tShowCard ? 1 : 0;
                nextCard = TRUE;
            }
            else
                nextCard = FALSE;
        }
        else
            nextCard = TRUE;
        
        if (nextCard)
        {
            tDelay = 0;
            state->isShown = tShowCard;
            
            if (state->value == CARD_VALUE_VOLTORB && !tAllCards)
                CreateExplosionSpriteAt(tStartX, tStartY);
            
            if (!tAllCards)
                tState++;
            else
            {
                if (++tStartX == BOARD_WIDTH)
                {
                    tStartX = 0;
                    
                    if (++tStartY == BOARD_HEIGHT)
                        tState++;
                }
            }
        }
    }
    return FALSE;
}

static bool32 VFSequence_ShowAllCards(s16 *data)
{
    StartCardFlipSequence(TRUE, TRUE);
    tState++;
    return FALSE;
}

static bool32 VFSequence_HideAllCards(s16 *data)
{
    StartCardFlipSequence(FALSE, TRUE);
    tState++;
    return FALSE;
}

static bool32 VFSequence_WaitCardFlipAnim(s16 *data)
{
    if (!sVoltorbFlipState->flippingCard)
        tState++;
    return FALSE;
}

static bool32 VFSequence_WaitAOrBButtonPress(s16 *data)
{
    if (JOY_NEW(A_BUTTON | B_BUTTON))
        tState++;
    return FALSE;
}

static bool32 VFSequence_WaitVoltorbExplosionAnim(s16 *data)
{
    if (!sVoltorbFlipState->explodingVoltorb)
        tState++;
    return FALSE;
}

static bool32 VFSequence_AdvanceLevel(s16 *data)
{
    ZeroNumCardsAndNumVoltorbs();
    AdvanceVoltorbFlipLevel();
    ResetVoltorbFlipCards();
    tState++;
    return FALSE;
}

static bool32 VFSequence_RegressLevel(s16 *data)
{
    TryRegressVoltorbFlipLevel();
    ResetVoltorbFlipCards();
    tState++;
    return FALSE;
}

static bool32 VFSequence_PrintCurrentLevel(s16 *data)
{
    PrintVoltorbFlipLevel();
    tState++;
    return FALSE;
}

static bool32 VFSequence_StartRound(s16 *data)
{
    InitVoltorbFlipRound();
    tState++;
    return FALSE;
}

static bool32 VFSequence_HidePointerSprite(s16 *data)
{
    SetPointerSpriteVisibility(TRUE);
    tState++;
    return FALSE;
}

static bool32 VFSequence_ShowPointerSprite(s16 *data)
{
    SetPointerSpriteVisibility(FALSE);
    tState++;
    return FALSE;
}

static bool32 VFSequence_WaitFanfare(s16 *data)
{
    if (IsFanfareTaskInactive())
        tState++;
    return FALSE;
}

static bool32 VFSequence_CreateYesNoMenu(s16 *data)
{
    CreateYesNoMenu(&sVoltorbFlipYesNoWindow, 2, 0, 2, 532, 14, 0);
    tState++;
    return FALSE;
}

static bool32 VFSequence_HandleYesNoInput(s16 *data)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
        case MENU_NOTHING_CHOSEN:
            break;
        case 0: // Choose Yes
            tState += 6;
            break;
        default: // Choose No or pressed B
            tState++;
            break;
    }
    return FALSE;
}

static bool32 VFSequence_ExitFromGame(s16 *data)
{
    FREE_AND_SET_NULL(sVoltorbFlipState);
    FreeAllWindowBuffers();
    SetMainCallback2(CB2_ReturnToFieldContinueScript);
    return TRUE;
}

static bool32 VFSequence_EnableMainTaskInput(s16 *data)
{
    EnableMainTaskInput(tMainTaskId);
    tState++;
    return FALSE;
}

static bool32 VFSequence_End(s16 *data)
{
    return TRUE;
}

#undef tState
#undef tTargetValue
#undef tTimerDelay
#undef tDelay
#undef tMainTaskId
#undef tShowCard
#undef tCardGfxId
#undef tCardGfxId2
#undef tAllCards
#undef tStartX
#undef tStartY
#undef tFlipStage

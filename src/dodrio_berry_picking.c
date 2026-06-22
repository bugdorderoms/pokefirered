#include "global.h"
#include "dodrio_berry_picking.h"
#include "gflib.h"
#include "item.h"
#include "link.h"
#include "link_rfu.h"
#include "minigame_countdown.h"
#include "m4a.h"
#include "new_menu_helpers.h"
#include "random.h"
#include "save.h"
#include "script.h"
#include "strings.h"
#include "text_window.h"
#include "text_window_graphics.h"
#include "constants/songs.h"
#include "constants/sound.h"

#define MAX_SCORE 999990
#define MAX_BERRIES 9999

// The minimum score needed to receive a prize
#define PRIZE_SCORE 3000

// Difficulty increases as berries are eaten. The rate of new berries increases and the types of berries changes
// When the max difficulty is reached it starts again from the beginning
#define NUM_DIFFICULTIES 7

#define NUM_RECORD_TYPES 3

#define MAX_FALL_DIST 10 // The number of times a berry needs to fall before hitting the ground
#define EAT_FALL_DIST 7 // The number of times a berry needs to fall to be available to eat

enum
{
    PRIZE_RECEIVED,
    PRIZE_FILLED_BAG,
    PRIZE_NO_ROOM,
    NO_PRIZE
};

enum
{
    BG_INTERFACE,
    BG_TREE_LEFT,
    BG_TREE_RIGHT,
    BG_SCENERY
};

enum
{
    WIN_PLAY_AGAIN,
    WIN_YES_NO
};

enum
{
    COLORID_GRAY,
    COLORID_RED,
    COLORID_BLUE
};

enum
{
    BERRY_BLUE,
    BERRY_GREEN,
    BERRY_GOLD,
    BERRY_MISSED,
    BERRY_PRIZE,
    BERRY_IN_ROW,
    NUM_BERRY_IDS
};

#define NUM_BERRY_TYPES 4 // Blue, Green, Gold, and 'missed'

// Eaten anim comes after the normal & missed versions of other berries
#define ANIM_EATEN (BERRY_MISSED * 2)

enum
{
    BERRYSTATE_NONE,
    BERRYSTATE_PICKED,   // Berry has been picked by a Dodrio, replaced with blue hit sprite (still falling)
    BERRYSTATE_EATEN,    // Berry has been eaten (after being picked), berry is gone now
    BERRYSTATE_SQUISHED  // Berry has hit the ground
};

enum
{
    PICK_NONE,     // Dodrio standing still
    PICK_RIGHT,    // Dodrio reaching right
    PICK_MIDDLE,   // Dodrio reaching up
    PICK_LEFT,     // Dodrio reaching left
    PICK_DISABLED  // Dodrio down after game over
};

enum
{
    INPUTSTATE_NONE,
    INPUTSTATE_TRY_PICK,
    INPUTSTATE_PICKED,
    INPUTSTATE_ATE_BERRY,
    INPUTSTATE_BAD_MISS
};

enum
{
    FUNC_INTRO,
    FUNC_INIT_COUNTDOWN,
    FUNC_COUNTDOWN,
    FUNC_WAIT_START,
    FUNC_PLAY_GAME,
    FUNC_INIT_RESULTS,
    FUNC_RESULTS,
    FUNC_ASK_PLAY_AGAIN,
    FUNC_END_LINK,
    FUNC_EXIT,
    FUNC_RESET_GAME,
    FUNC_WAIT_END_GAME
};

enum
{
    GFXFUNC_LOAD,
    GFXFUNC_SHOW_NAMES,
    GFXFUNC_SHOW_RESULTS,
    GFXFUNC_MSG_PLAY_AGAIN,
    GFXFUNC_MSG_SAVING,
    GFXFUNC_MSG_COMM_STANDBY,
    GFXFUNC_ERASE_MSG,
    GFXFUNC_MSG_PLAYER_DROPPED,
    GFXFUNC_STOP,
    GFXFUNC_IDLE
};

enum
{
    PACKET_READY_START = 1,
    PACKET_GAME_STATE,
    PACKET_PICK_STATE,
    PACKET_READY_END
};

// Colors for status bar squares
// Colored gray when a berry is missed
// Flash red when few yellow squares remain
enum
{
    STATUS_YELLOW,
    STATUS_GRAY,
    STATUS_RED
};

enum
{
    PLAY_AGAIN_NONE,
    PLAY_AGAIN_YES,
    PLAY_AGAIN_NO,
    PLAY_AGAIN_DROPPED = 5
};

#define PLAYER_NONE 0xFF

#define NUM_STATUS_SQUARES 10

#define GFXTAG_DODRIO    0
#define GFXTAG_STATUS    1
#define GFXTAG_BERRIES   2
#define GFXTAG_CLOUD     5
#define GFXTAG_COUNTDOWN 7

#define PALTAG_DODRIO_NORMAL 0
#define PALTAG_DODRIO_SHINY  1
#define PALTAG_STATUS        2
#define PALTAG_BERRIES       3
#define PALTAG_CLOUD         6
#define PALTAG_COUNTDOWN     8

#define NUM_CLOUDS 2

// Berries fall in predefined columns.
// A total of 10 are available, though fewer will be used with < 5 players
// The 11th column is a repeat of the 1st column wrapped around, so only
// the values 0-9 are unique 'valid' columns
#define NUM_BERRY_COLUMNS 11

struct DodrioGame_Berries
{
    u8 id;
    u8 fallDist;
};

struct DodrioGame_PlayerCommData
{
    u8 pickState;
    bool8 ateBerry:1;
    bool8 missedBerry:1;
    bool8 unused:6;
};

struct DodrioGame_Player
{
    u8 name[16];
    struct DodrioGame_Berries berries[NUM_BERRY_COLUMNS];
    struct DodrioGame_PlayerCommData comm;
};

struct DodrioGame_MonInfo
{
    bool8 isShiny;
};

struct DodrioGame_ScoreResults
{
    u8 ranking;
    u32 score;
};

struct DodrioGame_Gfx
{
    u16 tilemapBuffers[3][BG_SCREEN_SIZE];
    MainCallback func;
    u8 taskId;
    u8 state;
    u8 loadState;
    u8 cursorSelection;
    u8 playAgainState;
    bool8 finished;
    u16 timer;
    u8 windowIds[MAX_RFU_PLAYERS];
};

struct DodrioGame
{
    void (*exitCallback)(void);
    u8 multiplayerId;
    u8 startState;
    u8 state;
    u8 timer;
    u8 playersReceived;
    u8 clearRecvCmdTimer;
    u8 taskId;
    u8 funcId;
    u16 berriesPickedInRow;
    u16 maxBerriesPickedInRow;
    u8 numPlayers;
    u8 exitingGame:1;
    u8 startGame:1;
    u8 berriesFalling:1;
    u8 numGraySquares:4;
    u8 allReadyToEnd:1;
    u8 isLeader:1;
    u8 clearRecvCmds:1;
    u8 endSoundState:2;
    u8 playingPickSound:1;
    u8 unused:3;
    u8 pickStateQueue[4];
    u8 berryColStart;
    u8 berryColEnd;
    u8 cloudSpriteIds[NUM_CLOUDS];
    u8 berryIconSpriteIds[NUM_BERRY_TYPES];
    u16 berryResults[MAX_RFU_PLAYERS][NUM_BERRY_IDS];
    struct
    {
        u8 spriteIds[NUM_STATUS_SQUARES];
        bool8 entered[NUM_STATUS_SQUARES];
        s16 yChange[NUM_STATUS_SQUARES];
        u16 flashTimer;
    } statusBar;
    struct
    {
        u8 inputState:3;
        u8 playAgainState:3;
        u8 readyToStart:1;
        u8 readyToEnd:1;
        u8 inputDelay;
        u8 posToPlayerId;
        u8 dodrioSpriteId;
        u8 difficulty;
        u16 berriesEaten;
        struct DodrioGame_Player player;
        struct DodrioGame_MonInfo monInfo;
        struct DodrioGame_ScoreResults scoreResults;
    } members[MAX_RFU_PLAYERS];
    struct
    {
        u8 fallTimer;
        u8 newBerryTimer;
        u8 playersAttemptingPick[2];
        u8 berryState:2;
        u8 playingSquishSound:1;
        u8 berryEatenBy:3;
        u8 unused:2;
        u8 berrySpriteId;
        u8 prevBerryId;
        u8 eatTimer;
    } berryColumns[NUM_BERRY_COLUMNS];
    struct DodrioGame_Gfx gfx;
    struct DodrioGame_Player player;
};

struct PickStatePacket
{
    u8 id;
    u8 ALIGNED(4) pickState;
};

struct ReadyToStartPacket
{
    u8 id;
    bool8 ALIGNED(4) ready;
};

struct ReadyToEndPacket
{
    u8 id;
    bool32 ready;
};

struct GameStatePacket
{
    u8 id;
    u8 fallDist_Col0:4;
    u8 fallDist_Col1:4;
    u16 fallDist_Col2:4;
    u16 fallDist_Col3:4;
    u16 fallDist_Col4:4;
    u16 fallDist_Col5:4;
    u16 fallDist_Col6:4;
    u16 fallDist_Col7:4;
    u16 fallDist_Col8:4;
    u16 fallDist_Col9:4;
    u16 berryId_Col0:2;
    u16 berryId_Col1:2;
    u16 berryId_Col2:2;
    u16 berryId_Col3:2;
    u16 berryId_Col4:2;
    u16 berryId_Col5:2;
    u16 berryId_Col6:2;
    u16 berryId_Col7:2;
    u8 berryId_Col8:2;
    u8 berryId_Col9:2;
    u8 pickState_Player1:2;
    u8 pickState_Player2:2;
    u8 pickState_Player3:2;
    u8 pickState_Player4:2;
    u8 pickState_Player5:2;
    bool8 ateBerry_Player1:1;
    bool8 ateBerry_Player2:1;
    bool8 ateBerry_Player3:1;
    bool8 ateBerry_Player4:1;
    bool8 ateBerry_Player5:1;
    u8 numGraySquares:5;
    bool8 allReadyToEnd:1;
    bool8 berriesFalling:1;
    bool8 missedBerry_Player1:1;
    bool8 missedBerry_Player2:1;
    bool8 missedBerry_Player3:1;
    bool8 missedBerry_Player4:1;
    bool8 missedBerry_Player5:1;
};

struct WinCoords
{
    u8 left;
    u8 top;
};

static void Task_StartDodrioGame(u32 taskId);
static void Task_CommunicateMonInfo(u32 taskId);
static void Task_DodrioGame_Leader(u32 taskId);
static void Task_DodrioGame_Member(u32 taskId);
static void Task_NewGameIntro(u32 taskId);
static void LoadDodrioGfx(void);
static void CreateDodrioSprite(struct DodrioGame_MonInfo * monInfo, u32 playerId, u32 id, u32 numPlayers);
static void SpriteCB_Dodrio(struct Sprite *sprite);
static void SetAllDodrioDisabled(void);
static void UpdateAllDodrioAnims(void);
static void StartDodrioIntroAnim(void);
static void StartDodrioMissedAnim(void);
static void DestroyDodrioSprites(u32 numPlayers);
static void LoadBerryGfx(void);
static void CreateBerrySprites(void);
static void SetBerryYPos(u32 id, u32 y);
static void SetBerryInvisibility(u32 id, bool32 invisible);
static void SetBerryIconsInvisibility(bool32 invisible);
static void UpdateBerrySprites(void);
static void SetMaxBerriesPickedInRow(void);
static void InitFirstWaveOfBerries(void);
static void UpdateFallingBerries(void);
static void DestroyBerrySprites(void);
static void CreateCloudSprites(void);
static void SpriteCB_Cloud(struct Sprite *sprite);
static void ResetCloudPos(void);
static void SetCloudInvisibility(bool32 invisible);
static void StartCloudMovement(void);
static void DestroyCloudSprites(void);
static void CreateStatusBarSprites(void);
static void InitStatusBarPos(void);
static bool32 DoStatusBarIntro(void);
static void UpdateStatusBarAnim(u32 numEmpty);
static void SetStatusBarInvisibility(bool32 invisible);
static void DestroyStatusBar(void);
static void TryUpdateRecords(void);
static void SetRandomPrize(void);
static u32 TryGivePrize(u32 prizeItem);
static u32 GetPrizeItemId(void);
static void SetScoreResults(void);
static u32 GetHighestScore(void);
static void PrintRankedScores(void);
static u32 GetHighestBerryResult(u32 berryId);
static void InitGameGfx(void);
static void ResetGfxState(void);
static void SetGfxFuncById(u32 id);
static bool32 IsGfxFuncActive(void);
static void LoadGfx(void);
static void ShowNames(void);
static void ShowResults(void);
static void Msg_WantToPlayAgain(void);
static void Msg_SavingDontTurnOff(void);
static void Msg_CommunicationStandby(void);
static void EraseMessage(void);
static void Msg_SomeoneDroppedOut(void);
static void StopGfxFuncs(void);
static void GfxIdle(void);
static void DoGameIntro(void);
static void InitCountdown(void);
static void DoCountdown(void);
static void WaitGameStart(void);
static void PlayGame_Leader(void);
static void PlayGame_Member(void);
static void InitResults_Leader(void);
static void InitResults_Member(void);
static void DoResults(void);
static void AskPlayAgain(void);
static void EndLink(void);
static void ExitGame(void);
static void ResetGame(void);
static void WaitEndGame_Leader(void);
static void WaitEndGame_Member(void);

static EWRAM_DATA struct DodrioGame * sGame = NULL;

static const u32 sBg_Gfx[]                  = INCBIN_U32("graphics/link_games/dodrioberry_bg1.4bpp.lz");
static const u32 sBg_Tilemap[]              = INCBIN_U32("graphics/link_games/dodrioberry_bg1.bin.lz");
static const u16 sBg_Pal[]                  = INCBIN_U16("graphics/link_games/dodrioberry_bg1.gbapal",
                                                         "graphics/link_games/dodrioberry_bg2.gbapal");

static const u32 sTreeBorder_Gfx[]          = INCBIN_U32("graphics/link_games/dodrioberry_bg2.4bpp.lz");
static const u32 sTreeBorderRight_Tilemap[] = INCBIN_U32("graphics/link_games/dodrioberry_bg2right.bin.lz");
static const u32 sTreeBorderLeft_Tilemap[]  = INCBIN_U32("graphics/link_games/dodrioberry_bg2left.bin.lz");

static const u32 sDodrio_Gfx[]              = INCBIN_U32("graphics/link_games/dodrioberry_pkmn.4bpp.lz");
static const u16 sDodrioNormal_Pal[]        = INCBIN_U16("graphics/link_games/dodrioberry_pkmn.gbapal");
static const u16 sDodrioShiny_Pal[]         = INCBIN_U16("graphics/link_games/dodrioberry_shiny.gbapal");

static const u32 sBerries_Gfx[]             = INCBIN_U32("graphics/link_games/dodrioberry_berrysprites.4bpp.lz");
static const u16 sBerries_Pal[]             = INCBIN_U16("graphics/link_games/dodrioberry_berrysprites.gbapal");

static const u32 sCloud_Gfx[]               = INCBIN_U32("graphics/link_games/dodrioberry_platform.4bpp.lz");
static const u16 sCloud_Pal[]               = INCBIN_U16("graphics/link_games/dodrioberry_platform.gbapal");

static const u32 sStatus_Gfx[]              = INCBIN_U32("graphics/link_games/dodrioberry_status.4bpp.lz");
static const u16 sStatus_Pal[]              = INCBIN_U16("graphics/link_games/dodrioberry_status.gbapal");

static const u8 sText_BerryPickingResults[] = _("Announcing Berry-Picking results!");
static const u8 sText_AnnouncingRankings[] = _("Announcing rankings!");
static const u8 sText_AnnouncingPrizes[] = _("Announcing prizes!");
static const u8 sText_SpacePoints[] = _(" points");

static const u8 sTextColorTable[][3] =
{
    [COLORID_GRAY]  = {TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY},
    [COLORID_RED]   = {TEXT_COLOR_WHITE, TEXT_COLOR_RED,       TEXT_COLOR_LIGHT_RED},
    [COLORID_BLUE]  = {TEXT_COLOR_WHITE, TEXT_COLOR_BLUE,      TEXT_COLOR_LIGHT_BLUE},
};

static const struct BgTemplate sBgTemplates[] =
{
    {
        .bg = BG_INTERFACE,
        .charBaseIndex = 0,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = BG_TREE_LEFT,
        .charBaseIndex = 2,
        .mapBaseIndex = 12,
        .screenSize = 1,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    },
    {
        .bg = BG_TREE_RIGHT,
        .charBaseIndex = 2,
        .mapBaseIndex = 14,
        .screenSize = 1,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    },
    {
        .bg = BG_SCENERY,
        .charBaseIndex = 3,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0
    },
};

static const struct WindowTemplate sWindowTemplates_Results[] =
{
    {
        .bg = BG_INTERFACE,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 28,
        .height = 3,
        .paletteNum = 13,
        .baseBlock = 0x13,
    },
    {
        .bg = BG_INTERFACE,
        .tilemapLeft = 1,
        .tilemapTop = 6,
        .width = 28,
        .height = 13,
        .paletteNum = 13,
        .baseBlock = 0x67,
    }
};

static const struct WindowTemplate sWindowTemplates_Records =
{
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = 28,
    .height = 11,
    .paletteNum = 15,
    .baseBlock = 1,
};

static const struct WindowTemplate sWindowTemplate_Prize =
{
    .bg = BG_INTERFACE,
    .tilemapLeft = 1,
    .tilemapTop = 6,
    .width = 28,
    .height = 7,
    .paletteNum = 13,
    .baseBlock = 0x67,
};

static const struct WindowTemplate sWindowTemplates_PlayAgain[] =
{
    [WIN_PLAY_AGAIN] = {
        .bg = BG_INTERFACE,
        .tilemapLeft = 1,
        .tilemapTop = 8,
        .width = 19,
        .height = 3,
        .paletteNum = 13,
        .baseBlock = 0x13,
    },
    [WIN_YES_NO] = {
        .bg = BG_INTERFACE,
        .tilemapLeft = 22,
        .tilemapTop = 7,
        .width = 6,
        .height = 4,
        .paletteNum = 13,
        .baseBlock = 0x4C,
    }
};

static const struct WindowTemplate sWindowTemplate_CommStandby =
{
    .bg = BG_INTERFACE,
    .tilemapLeft = 5,
    .tilemapTop = 8,
    .width = 19,
    .height = 3,
    .paletteNum = 13,
    .baseBlock = 0x13,
};

static const struct WindowTemplate sWindowTemplate_DroppedOut =
{
    .bg = BG_INTERFACE,
    .tilemapLeft = 4,
    .tilemapTop = 6,
    .width = 22,
    .height = 5,
    .paletteNum = 13,
    .baseBlock = 0x13,
};

static const struct OamData sOamData_Dodrio =
{
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64),
    .priority = 2,
};

static const union AnimCmd sAnim_Dodrio_Normal[] =
{
    ANIMCMD_FRAME(0, 20),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sAnim_Dodrio_PickRight[] =
{
    ANIMCMD_FRAME(64, 20),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sAnim_Dodrio_PickMiddle[] =
{
    ANIMCMD_FRAME(128, 20),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sAnim_Dodrio_PickLeft[] =
{
    ANIMCMD_FRAME(192, 20),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sAnim_Dodrio_Down[] =
{
    ANIMCMD_FRAME(256, 20),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd *const sAnims_Dodrio[] =
{
    [PICK_NONE]     = sAnim_Dodrio_Normal,
    [PICK_RIGHT]    = sAnim_Dodrio_PickRight,
    [PICK_MIDDLE]   = sAnim_Dodrio_PickMiddle,
    [PICK_LEFT]     = sAnim_Dodrio_PickLeft,
    [PICK_DISABLED] = sAnim_Dodrio_Down
    // There is an unused 6th frame of Dodrio's graphic
};

static const struct OamData sOamData_Berry =
{
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .size = SPRITE_SIZE(16x16),
    .priority = 2,
};

static const union AnimCmd sAnim_Berry_Blue[] =
{
    ANIMCMD_FRAME(0, 20),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sAnim_Berry_Green[] =
{
    ANIMCMD_FRAME(4, 20),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sAnim_Berry_Gold[] =
{
    ANIMCMD_FRAME(8, 20),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sAnim_Berry_BlueSquished[] =
{
    ANIMCMD_FRAME(12, 20),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sAnim_Berry_GreenSquished[] =
{
    ANIMCMD_FRAME(16, 20),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sAnim_Berry_GoldSquished[] =
{
    ANIMCMD_FRAME(20, 20),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sAnim_Berry_Eaten[] =
{
    ANIMCMD_FRAME(24, 20),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sAnim_Berry_Empty1[] =
{
    ANIMCMD_FRAME(28, 20),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sAnim_Berry_Empty2[] =
{
    ANIMCMD_FRAME(32, 20),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd *const sAnims_Berry[] =
{
    [BERRY_BLUE]  = sAnim_Berry_Blue,
    [BERRY_GREEN] = sAnim_Berry_Green,
    [BERRY_GOLD]  = sAnim_Berry_Gold,

    [BERRY_BLUE + BERRY_MISSED]  = sAnim_Berry_BlueSquished,
    [BERRY_GREEN + BERRY_MISSED] = sAnim_Berry_GreenSquished,
    [BERRY_GOLD + BERRY_MISSED]  = sAnim_Berry_GoldSquished,

    [ANIM_EATEN] = sAnim_Berry_Eaten,

    sAnim_Berry_Empty1,
    sAnim_Berry_Empty2
};

static const struct SpriteTemplate sBerrySpriteTemplate =
{
    .tileTag = GFXTAG_BERRIES,
    .paletteTag = PALTAG_BERRIES,
    .oam = &sOamData_Berry,
    .anims = sAnims_Berry,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

// Used by the status bar and the results screen berry icons
static const struct OamData sOamData_16x16_Priority0 =
{
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .size = SPRITE_SIZE(16x16),
    .priority = 0
};

static const struct SpriteTemplate sBerryIconSpriteTemplate =
{
    .tileTag = GFXTAG_BERRIES,
    .paletteTag = PALTAG_BERRIES,
    .oam = &sOamData_16x16_Priority0,
    .anims = sAnims_Berry,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const union AnimCmd sAnims_StatusBar_Yellow[] =
{
    ANIMCMD_FRAME(0, 20),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sAnims_StatusBar_Gray[] =
{
    ANIMCMD_FRAME(4, 20),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sAnims_StatusBar_Red[] =
{
    ANIMCMD_FRAME(8, 20),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd *const sAnims_StatusBar[] =
{
    [STATUS_YELLOW] = sAnims_StatusBar_Yellow,
    [STATUS_GRAY]   = sAnims_StatusBar_Gray,
    [STATUS_RED]    = sAnims_StatusBar_Red
};

static const struct SpriteTemplate sStatusBarSpriteTemplate =
{
    .tileTag = GFXTAG_STATUS,
    .paletteTag = PALTAG_STATUS,
    .oam = &sOamData_16x16_Priority0,
    .anims = sAnims_StatusBar,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct OamData sOamData_Cloud =
{
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .size = SPRITE_SIZE(64x32),
    .priority = 3
};

static const union AnimCmd sAnim_Cloud[] =
{
    ANIMCMD_FRAME(0, 20),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd *const sAnims_Cloud[] =
{
    sAnim_Cloud
};

static const struct SpriteTemplate sCloudSpriteTemplate =
{
    .tileTag = GFXTAG_CLOUD,
    .paletteTag = PALTAG_CLOUD,
    .oam = &sOamData_Cloud,
    .anims = sAnims_Cloud,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Cloud,
};

struct
{
    u8 id;
    MainCallback func;
} static const sGfxFuncs[] =
{
    {GFXFUNC_LOAD,               LoadGfx},
    {GFXFUNC_SHOW_NAMES,         ShowNames},
    {GFXFUNC_SHOW_RESULTS,       ShowResults},
    {GFXFUNC_MSG_PLAY_AGAIN,     Msg_WantToPlayAgain},
    {GFXFUNC_MSG_SAVING,         Msg_SavingDontTurnOff},
    {GFXFUNC_MSG_COMM_STANDBY,   Msg_CommunicationStandby},
    {GFXFUNC_ERASE_MSG,          EraseMessage},
    {GFXFUNC_MSG_PLAYER_DROPPED, Msg_SomeoneDroppedOut},
    {GFXFUNC_STOP,               StopGfxFuncs},
    {GFXFUNC_IDLE,               GfxIdle},
};

static void (*const sLeaderFuncs[])(void) =
{
    [FUNC_INTRO]          = DoGameIntro,
    [FUNC_INIT_COUNTDOWN] = InitCountdown,
    [FUNC_COUNTDOWN]      = DoCountdown,
    [FUNC_WAIT_START]     = WaitGameStart,
    [FUNC_PLAY_GAME]      = PlayGame_Leader,
    [FUNC_INIT_RESULTS]   = InitResults_Leader,
    [FUNC_RESULTS]        = DoResults,
    [FUNC_ASK_PLAY_AGAIN] = AskPlayAgain,
    [FUNC_END_LINK]       = EndLink,
    [FUNC_EXIT]           = ExitGame,
    [FUNC_RESET_GAME]     = ResetGame,
    [FUNC_WAIT_END_GAME]  = WaitEndGame_Leader
};

static void (*const sMemberFuncs[])(void) =
{
    [FUNC_INTRO]          = DoGameIntro,
    [FUNC_INIT_COUNTDOWN] = InitCountdown,
    [FUNC_COUNTDOWN]      = DoCountdown,
    [FUNC_WAIT_START]     = WaitGameStart,
    [FUNC_PLAY_GAME]      = PlayGame_Member,
    [FUNC_INIT_RESULTS]   = InitResults_Member,
    [FUNC_RESULTS]        = DoResults,
    [FUNC_ASK_PLAY_AGAIN] = AskPlayAgain,
    [FUNC_END_LINK]       = EndLink,
    [FUNC_EXIT]           = ExitGame,
    [FUNC_RESET_GAME]     = ResetGame,
    [FUNC_WAIT_END_GAME]  = WaitEndGame_Member
};

// How far the outer tree borders should slide to reveal the game screen.
// Dependent on how many players are playing.
// Curiously the 2-player screen is narrower than the 1-player, though neither
// gets used as there's a 3 player minimum
static const u8 sTreeBorderXPos[MAX_RFU_PLAYERS] = {8, 5, 8, 11, 15};

static const u8 sBerryIconXCoords[] = {88, 128, 168, 208};

static const u16 sPrizeBerryIds[][10] =
{
    { // Possible prizes with 3 players
        ITEM_TO_BERRY(ITEM_RAZZ_BERRY),
        ITEM_TO_BERRY(ITEM_BLUK_BERRY),
        ITEM_TO_BERRY(ITEM_NANAB_BERRY),
        ITEM_TO_BERRY(ITEM_WEPEAR_BERRY),
        ITEM_TO_BERRY(ITEM_PINAP_BERRY),
        ITEM_TO_BERRY(ITEM_PINAP_BERRY),
        ITEM_TO_BERRY(ITEM_WEPEAR_BERRY),
        ITEM_TO_BERRY(ITEM_NANAB_BERRY),
        ITEM_TO_BERRY(ITEM_BLUK_BERRY),
        ITEM_TO_BERRY(ITEM_RAZZ_BERRY)
    },
    { // Possible prizes with 4 players
        ITEM_TO_BERRY(ITEM_POMEG_BERRY),
        ITEM_TO_BERRY(ITEM_KELPSY_BERRY),
        ITEM_TO_BERRY(ITEM_QUALOT_BERRY),
        ITEM_TO_BERRY(ITEM_HONDEW_BERRY),
        ITEM_TO_BERRY(ITEM_GREPA_BERRY),
        ITEM_TO_BERRY(ITEM_TAMATO_BERRY),
        ITEM_TO_BERRY(ITEM_CORNN_BERRY),
        ITEM_TO_BERRY(ITEM_MAGOST_BERRY),
        ITEM_TO_BERRY(ITEM_RABUTA_BERRY),
        ITEM_TO_BERRY(ITEM_NOMEL_BERRY)
    },
    { // Possible prizes with 5 players
        ITEM_TO_BERRY(ITEM_SPELON_BERRY),
        ITEM_TO_BERRY(ITEM_PAMTRE_BERRY),
        ITEM_TO_BERRY(ITEM_WATMEL_BERRY),
        ITEM_TO_BERRY(ITEM_DURIN_BERRY),
        ITEM_TO_BERRY(ITEM_BELUE_BERRY),
        ITEM_TO_BERRY(ITEM_BELUE_BERRY),
        ITEM_TO_BERRY(ITEM_DURIN_BERRY),
        ITEM_TO_BERRY(ITEM_WATMEL_BERRY),
        ITEM_TO_BERRY(ITEM_PAMTRE_BERRY),
        ITEM_TO_BERRY(ITEM_SPELON_BERRY)
    }
};

static const u8 sBerryScoreMultipliers[] =
{
    [BERRY_BLUE]   = 10,
    [BERRY_GREEN]  = 30,
    [BERRY_GOLD]   = 50,
    [BERRY_MISSED] = 50 // Subtracted
};

static const u8 sBerryFallDelays[][3] =
{
    { [BERRY_BLUE] = 40, [BERRY_GREEN] = 24, [BERRY_GOLD] = 13 },
    { [BERRY_BLUE] = 32, [BERRY_GREEN] = 19, [BERRY_GOLD] = 10 },
    { [BERRY_BLUE] = 22, [BERRY_GREEN] = 13, [BERRY_GOLD] =  7 }
};

static const u8 *const sRankingTexts[MAX_RFU_PLAYERS] =
{
    COMPOUND_STRING("1:"),
    COMPOUND_STRING("2:"),
    COMPOUND_STRING("3:"),
    COMPOUND_STRING("4:"),
    COMPOUND_STRING("5:")
};

struct
{
    const u8 *text;
    u8 maxDigits;
    u8 yCoords[2]; // Text and num coords
} static const sRecordsData[] =
{
    {
        .text = COMPOUND_STRING("Berries picked:"),
        .maxDigits = 4,
        .yCoords = {24, 24},
    },
    {
        .text = COMPOUND_STRING("Best score:"),
        .maxDigits = 7,
        .yCoords = {40, 40},
    },
    {
        .text = COMPOUND_STRING("Berries picked in a row with\nfive players:"),
        .maxDigits = 4,
        .yCoords = {56, 70},
    }
};

static const u8 sResultsXCoords[] = {92, 132, 172, 212};
static const u8 sResultsYCoords[] = {30, 45, 60, 75, 90};
static const u8 sRankingYCoords[] = {12, 28, 44, 60, 76};

static const struct UCoords8 sCloudStartCoords[NUM_CLOUDS] =
{
    {.x = 230, .y = 55},
    {.x =  30, .y = 74}
};

// The number of berries eaten needed to progress to the next difficulty
static const u8 sDifficultyThresholds[NUM_DIFFICULTIES] = {5, 10, 20, 30, 50, 70, 100};

#define x 9 // No player at this column. This may go out of bounds if this is returned

// Takes the number of players and a column and returns the player id at that column.
// Note that the assignment is somewhat arbitrary as players share neighboring columns.
static const u8 sPlayerIdAtColumn[MAX_RFU_PLAYERS][NUM_BERRY_COLUMNS] =
{
    {x, x, x, x, 1, 1, 1, x, x, x, x}, // 1 player
    {x, x, x, 0, 0, 1, 1, 0, x, x, x}, // 2 players
    {x, x, 2, 2, 0, 0, 1, 1, 1, x, x}, // 3 players
    {x, 3, 3, 0, 0, 1, 1, 2, 2, 3, x}, // 4 players
    {3, 3, 4, 4, 0, 0, 1, 1, 2, 2, 3}, // 5 players
};

#undef x

// For each player, the array is a list of all the columns starting with the column to their left
// Only the range of active columns is read from the array (dependent on the number of players),
// so the arrays are spaced such that the numbers in the center are where the data that's read starts and end.
static const u8 sActiveColumnMap[MAX_RFU_PLAYERS][MAX_RFU_PLAYERS][NUM_BERRY_COLUMNS] =
{
    { // 1 player (never used), columns 4-6.
      // Sometimes read to get default order regardless of the current number of players
        {0, 1, 2, 3,     4, 5, 6,     7, 8, 9, 0},
    },
    { // 2 players (never used), columns 3-6
        {0, 1, 2,     3, 4, 5, 6, 3,     8, 9, 0},
        {0, 1, 2,     5, 6, 3, 4, 5,     8, 9, 0},
    },
    { // 3 players, columns 2-7
        {0, 1,     2, 3, 4, 5, 6, 7, 2,     9, 0},
        {0, 1,     4, 5, 6, 7, 2, 3, 4,     9, 0},
        {0, 1,     6, 7, 2, 3, 4, 5, 6,     9, 0},
    },
    { // 4 players, columns 1-8
        {0,     1, 2, 3, 4, 5, 6, 7, 8, 1,     0},
        {0,     3, 4, 5, 6, 7, 8, 1, 2, 3,     0},
        {0,     5, 6, 7, 8, 1, 2, 3, 4, 5,     0},
        {0,     7, 8, 1, 2, 3, 4, 5, 6, 7,     0},
    },
    { // 5 players, all columns (0-9)
        {    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0    },
        {    2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2    },
        {    4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4    },
        {    6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6    },
        {    8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8    },
    },
};

// Each array contains the columns that belong solely to one player, dependent on the number of players
// When determing how difficult the berries in a column should be, the highest
// difficulty of the players sharing that column is used.
// This table is used to skip that check, and instead automatically use the
// difficulty of the only player who can use the column.
static const u8 sUnsharedColumns[MAX_RFU_PLAYERS][MAX_RFU_PLAYERS] =
{
    {5},
    {4, 6},
    {3, 5, 7},
    {2, 4, 6, 8},
    {1, 3, 5, 7, 9},
};

// A table for which falling berry column corresponds to which Dodrio head for each player
// The numbers in each array are the column number for each head, {left, middle, right}
// Dependent on the number of players
static const u8 sDodrioHeadToColumnMap[MAX_RFU_PLAYERS][MAX_RFU_PLAYERS][3] =
{
    { // 1 player (never used)
        {4, 5, 6},
    },
    { // 2 players (never used)
        {3, 4, 5},
        {5, 6, 3},
    },
    { // 3 players
        {4, 5, 6},
        {6, 7, 2},
        {2, 3, 4},
    },
    { // 4 players
        {3, 4, 5},
        {5, 6, 7},
        {7, 8, 1},
        {1, 2, 3},
    },
    { // 5 players
        {4, 5, 6},
        {6, 7, 8},
        {8, 9, 0},
        {0, 1, 2},
        {2, 3, 4},
    },
};

// A table of player ids and their neighbor, dependent on the total number of players
// {L, M, R}, where M is the player in question, L is their neighbor to the left, and R is their neighbor to the right
static const u8 sDodrioNeighborMap[MAX_RFU_PLAYERS][MAX_RFU_PLAYERS][3] =
{
    { // 1 player (never used)
        {1, 0, 1},
    },
    { // 2 players (never used)
        {1, 0, 1},
        {0, 1, 0},
    },
    { // 3 players
        {2, 0, 1},
        {0, 1, 2},
        {1, 2, 0},
    },
    { // 4 players
        {3, 0, 1},
        {0, 1, 2},
        {1, 2, 3},
        {2, 3, 0},
    },
    { // 5 players
        {4, 0, 1},
        {0, 1, 2},
        {1, 2, 3},
        {2, 3, 4},
        {3, 4, 0},
    }
};

static const struct WinCoords sNameWindowCoords_1Player[] = {{12, 6}};
static const struct WinCoords sNameWindowCoords_2Players[] = {{9, 10}, {15, 6}};
static const struct WinCoords sNameWindowCoords_3Players[] = {{12, 6}, {18, 10}, {6, 10}};
static const struct WinCoords sNameWindowCoords_4Players[] = {{9, 10}, {15, 6}, {21, 10}, {3, 6}};
static const struct WinCoords sNameWindowCoords_5Players[] = {{12, 6}, {18, 10}, {23, 6}, {1, 6}, {6, 10}};

static const struct WinCoords * const sNameWindowCoords[MAX_RFU_PLAYERS] =
{
    sNameWindowCoords_1Player,
    sNameWindowCoords_2Players,
    sNameWindowCoords_3Players,
    sNameWindowCoords_4Players,
    sNameWindowCoords_5Players
};

////////////////////
// INITIALIZATION //
////////////////////

static void CB2_DodrioGame(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB_DodrioGame(void)
{
    TransferPlttBuffer();
    LoadOam();
    ProcessSpriteCopyRequests();
}

static inline void InitMonInfo(struct DodrioGame_MonInfo * monInfo, struct Pokemon * mon)
{
    monInfo->isShiny = GetMonData(mon, MON_DATA_IS_SHINY);
}

static void InitDodrioGame(void)
{
    u32 i;
    
    sGame->numPlayers = GetLinkPlayerCount();
    sGame->multiplayerId = GetMultiplayerId();
    sGame->isLeader = (sGame->multiplayerId == 0);
    sGame->startState = 0;
    sGame->state = 0;
    sGame->timer = 0;
    sGame->funcId = FUNC_INTRO;
    sGame->startGame = FALSE;
    sGame->berriesFalling = FALSE;
    sGame->numGraySquares = 0;
    sGame->allReadyToEnd = FALSE;

    for (i = 0; i < ARRAY_COUNT(sGame->pickStateQueue); i++)
        sGame->pickStateQueue[i] = PICK_NONE;

    for (i = 0; i < MAX_RFU_PLAYERS; i++)
    {
        sGame->berryResults[i][BERRY_BLUE] = 0;
        sGame->berryResults[i][BERRY_GREEN] = 0;
        sGame->berryResults[i][BERRY_GOLD] = 0;
        sGame->berryResults[i][BERRY_MISSED] = 0;
        sGame->berryResults[i][BERRY_IN_ROW] = 0;
        sGame->members[i].inputState = INPUTSTATE_NONE;
        sGame->members[i].inputDelay = 0;
        sGame->members[i].playAgainState = PLAY_AGAIN_NONE;
        sGame->members[i].readyToEnd = FALSE;
    }

    for (i = 0; i < NUM_BERRY_COLUMNS; i++)
    {
        sGame->berryColumns[i].fallTimer = 0;
        sGame->berryColumns[i].newBerryTimer = 0;
        sGame->berryColumns[i].berryState = BERRYSTATE_NONE;
        sGame->berryColumns[i].playersAttemptingPick[0] = PLAYER_NONE;
        sGame->berryColumns[i].playersAttemptingPick[1] = PLAYER_NONE;
    }
    sGame->members[0].posToPlayerId = sGame->multiplayerId;
    
    for (i = 1; i < sGame->numPlayers; i++)
    {
        sGame->members[i].posToPlayerId = sGame->members[i - 1].posToPlayerId + 1;
        if (sGame->members[i].posToPlayerId > sGame->numPlayers - 1)
            sGame->members[i].posToPlayerId %= sGame->numPlayers;
    }
}

static void GetActiveBerryColumns(u32 numPlayers, u8 *start, u8 *end)
{
    switch (numPlayers)
    {
        case 1:
            *start = 4, *end = 7;
            break;
        case 2:
            *start = 3, *end = 8;
            break;
        case 3:
            *start = 2, *end = 9;
            break;
        case 4:
            *start = 1, *end = 10;
            break;
        case 5:
            *start = 0, *end = 11;
            break;
    }
}

void StartDodrioBerryPicking(u32 partyId, MainCallback exitCallback)
{
    sGame = AllocZeroed(sizeof(*sGame));
    
    if (sGame != NULL && gReceivedRemoteLinkPlayers)
    {
        ResetTasks();
        ResetSpriteData();
        FreeAllSpritePalettes();
        
        InitDodrioGame();
        sGame->exitCallback = exitCallback;
        sGame->player = sGame->members[sGame->multiplayerId].player;
        
        InitMonInfo(&sGame->members[sGame->multiplayerId].monInfo, &gPlayerParty[partyId]);
        CreateTask(Task_StartDodrioGame, 1);
        SetMainCallback2(CB2_DodrioGame);
        SetRandomPrize();
        GetActiveBerryColumns(sGame->numPlayers, &sGame->berryColStart, &sGame->berryColEnd);
        StopMapMusic();
        PlayNewMapMusic(MUS_BERRY_PICK);
        return;
    }
    // Exit - Alloc failed, or players not connected
    SetMainCallback2(exitCallback);
}

static void CreateDodrioGameTask(TaskFunc func)
{
    sGame->taskId = CreateTask(func, 1);
    sGame->state = 0;
    sGame->startState = 0;
    sGame->timer = 0;
}

static void Task_StartDodrioGame(u32 taskId)
{
    u32 i, numPlayers;

    switch (sGame->startState)
    {
        case 0:
            SetVBlankCallback(NULL);
            CreateTask(Task_CommunicateMonInfo, 4);
            sGame->startState++;
            break;
        case 1:
            if (!FuncIsActiveTask(Task_CommunicateMonInfo))
            {
                InitGameGfx();
                sGame->startState++;
            }
            break;
        case 2:
            if (!IsGfxFuncActive())
            {
                Rfu_SetLinkStandbyCallback();
                sGame->startState++;
            }
            break;
        case 3:
            if (IsLinkTaskFinished())
            {
                if (gReceivedRemoteLinkPlayers)
                {
                    LoadWirelessStatusIndicatorSpriteGfx();
                    CreateWirelessStatusIndicatorSprite(0, 0);
                }
                sGame->startState++;
            }
            break;
        case 4:
            numPlayers = sGame->numPlayers;
            
            LoadDodrioGfx();
            
            for (i = 0; i < numPlayers; i++)
                CreateDodrioSprite(&sGame->members[sGame->members[i].posToPlayerId].monInfo, i, sGame->members[i].posToPlayerId, sGame->numPlayers);

            sGame->startState++;
            break;
        case 5:
            LoadBerryGfx();
            CreateBerrySprites();
            CreateCloudSprites();
            CreateStatusBarSprites();
            sGame->startState++;
            break;
        case 6:
            BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
            BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
            SetVBlankCallback(VBlankCB_DodrioGame);
            sGame->startState++;
            break;
        case 7:
            UpdatePaletteFade();
            
            if (!gPaletteFade.active)
                sGame->startState++;
            
            break;
        default:
            DestroyTask(taskId);
            CreateDodrioGameTask(Task_NewGameIntro);
            break;
    }
}

static bool32 SlideTreeBordersOut(void)
{
    u8 x = sGame->timer / 4;
    
    sGame->timer++;
    
    if (x != 0 && sGame->timer % 4 == 0)
    {
        if (x < sTreeBorderXPos[sGame->numPlayers - 1])
        {
            // Update position
            SetGpuReg(REG_OFFSET_BG1HOFS,  (x * 8)); // BG_TREE_LEFT
            SetGpuReg(REG_OFFSET_BG2HOFS, -(x * 8)); // BG_TREE_RIGHT
        }
        else // Animation finished
            return TRUE;
    }
    return FALSE;
}

static void Task_NewGameIntro(u32 taskId)
{
    switch (sGame->state)
    {
        case 0:
            if (SlideTreeBordersOut())
                sGame->state++;
            break;
        case 1:
            InitStatusBarPos();
            sGame->state++;
            break;
        case 2:
            if (DoStatusBarIntro())
                sGame->state++;
            break;
        default:
            CreateDodrioGameTask(sGame->isLeader ? Task_DodrioGame_Leader : Task_DodrioGame_Member);
            DestroyTask(taskId);
            break;
    }
}

////////////////////////////////
// SEND/RECV PACKET FUNCTIONS //
////////////////////////////////

static void SendPacket_GameState(struct DodrioGame_Player *player, struct DodrioGame_PlayerCommData *player1, struct DodrioGame_PlayerCommData *player2,
                                 struct DodrioGame_PlayerCommData *player3, struct DodrioGame_PlayerCommData *player4, struct DodrioGame_PlayerCommData *player5,
                                 u32 numGraySquares, bool32 berriesFalling, bool32 allReadyToEnd)
{
    struct GameStatePacket packet;

    packet.id = PACKET_GAME_STATE;
    packet.fallDist_Col0 = player->berries[0].fallDist;
    packet.fallDist_Col1 = player->berries[1].fallDist;
    packet.fallDist_Col2 = player->berries[2].fallDist;
    packet.fallDist_Col3 = player->berries[3].fallDist;
    packet.fallDist_Col4 = player->berries[4].fallDist;
    packet.fallDist_Col5 = player->berries[5].fallDist;
    packet.fallDist_Col6 = player->berries[6].fallDist;
    packet.fallDist_Col7 = player->berries[7].fallDist;
    packet.fallDist_Col8 = player->berries[8].fallDist;
    packet.fallDist_Col9 = player->berries[9].fallDist;

    packet.berryId_Col0 = player->berries[0].id;
    packet.berryId_Col1 = player->berries[1].id;
    packet.berryId_Col2 = player->berries[2].id;
    packet.berryId_Col3 = player->berries[3].id;
    packet.berryId_Col4 = player->berries[4].id;
    packet.berryId_Col5 = player->berries[5].id;
    packet.berryId_Col6 = player->berries[6].id;
    packet.berryId_Col7 = player->berries[7].id;
    packet.berryId_Col8 = player->berries[8].id;
    packet.berryId_Col9 = player->berries[9].id;

    packet.pickState_Player1 = player1->pickState;
    packet.pickState_Player2 = player2->pickState;
    packet.pickState_Player3 = player3->pickState;
    packet.pickState_Player4 = player4->pickState;
    packet.pickState_Player5 = player5->pickState;

    packet.ateBerry_Player1 = player1->ateBerry;
    packet.ateBerry_Player2 = player2->ateBerry;
    packet.ateBerry_Player3 = player3->ateBerry;
    packet.ateBerry_Player4 = player4->ateBerry;
    packet.ateBerry_Player5 = player5->ateBerry;

    packet.missedBerry_Player1 = player1->missedBerry;
    packet.missedBerry_Player2 = player2->missedBerry;
    packet.missedBerry_Player3 = player3->missedBerry;
    packet.missedBerry_Player4 = player4->missedBerry;
    packet.missedBerry_Player5 = player5->missedBerry;

    packet.numGraySquares = numGraySquares;
    packet.berriesFalling = berriesFalling;
    packet.allReadyToEnd = allReadyToEnd;
    Rfu_SendPacket(&packet);
}

static void RecvPacket_GameState(struct DodrioGame_Player *player, struct DodrioGame_PlayerCommData *player1, struct DodrioGame_PlayerCommData *player2,
                                 struct DodrioGame_PlayerCommData *player3, struct DodrioGame_PlayerCommData *player4, struct DodrioGame_PlayerCommData *player5)
{
    struct GameStatePacket *packet;

    if ((gRecvCmds[0][0] & RFUCMD_MASK) != RFUCMD_SEND_PACKET)
        return;

    packet = (void *)&gRecvCmds[0][1];
    if (packet->id == PACKET_GAME_STATE)
    {
        player->berries[0].fallDist = packet->fallDist_Col0;
        player->berries[1].fallDist = packet->fallDist_Col1;
        player->berries[2].fallDist = packet->fallDist_Col2;
        player->berries[3].fallDist = packet->fallDist_Col3;
        player->berries[4].fallDist = packet->fallDist_Col4;
        player->berries[5].fallDist = packet->fallDist_Col5;
        player->berries[6].fallDist = packet->fallDist_Col6;
        player->berries[7].fallDist = packet->fallDist_Col7;
        player->berries[8].fallDist = packet->fallDist_Col8;
        player->berries[9].fallDist = packet->fallDist_Col9;
        player->berries[10].fallDist = packet->fallDist_Col0;

        player->berries[0].id = packet->berryId_Col0;
        player->berries[1].id = packet->berryId_Col1;
        player->berries[2].id = packet->berryId_Col2;
        player->berries[3].id = packet->berryId_Col3;
        player->berries[4].id = packet->berryId_Col4;
        player->berries[5].id = packet->berryId_Col5;
        player->berries[6].id = packet->berryId_Col6;
        player->berries[7].id = packet->berryId_Col7;
        player->berries[8].id = packet->berryId_Col8;
        player->berries[9].id = packet->berryId_Col9;
        player->berries[10].id = packet->berryId_Col0;

        player1->pickState = packet->pickState_Player1;
        player1->ateBerry = packet->ateBerry_Player1;
        player1->missedBerry = packet->missedBerry_Player1;

        player2->pickState = packet->pickState_Player2;
        player2->ateBerry = packet->ateBerry_Player2;
        player2->missedBerry = packet->missedBerry_Player2;

        player3->pickState = packet->pickState_Player3;
        player3->ateBerry = packet->ateBerry_Player3;
        player3->missedBerry = packet->missedBerry_Player3;

        player4->pickState = packet->pickState_Player4;
        player4->ateBerry = packet->ateBerry_Player4;
        player4->missedBerry = packet->missedBerry_Player4;

        player5->pickState = packet->pickState_Player5;
        player5->ateBerry = packet->ateBerry_Player5;
        player5->missedBerry = packet->missedBerry_Player5;

        sGame->numGraySquares = packet->numGraySquares;
        sGame->berriesFalling = packet->berriesFalling;
        sGame->allReadyToEnd = packet->allReadyToEnd;
    }
}

static void SendPacket_ReadyToStart(bool32 ready)
{
    struct ReadyToStartPacket packet;
    packet.id = PACKET_READY_START;
    packet.ready = ready;
    Rfu_SendPacket(&packet);
}

static bool32 RecvPacket_ReadyToStart(u32 playerId)
{
    struct ReadyToStartPacket *packet;

    if ((gRecvCmds[0][0] & RFUCMD_MASK) != RFUCMD_SEND_PACKET)
        return FALSE;

    packet = (void *)&gRecvCmds[playerId][1];
    if (packet->id == PACKET_READY_START)
        return packet->ready;

    return FALSE;
}

static void SendPacket_ReadyToEnd(bool32 ready)
{
    struct ReadyToEndPacket packet;
    packet.id = PACKET_READY_END;
    packet.ready = ready;
    Rfu_SendPacket(&packet);
}

static bool32 RecvPacket_ReadyToEnd(u32 playerId)
{
    struct ReadyToEndPacket * packet;

    if ((gRecvCmds[0][0] & RFUCMD_MASK) != RFUCMD_SEND_PACKET)
        return FALSE;

    packet = (void *)&gRecvCmds[playerId][1];
    if (packet->id == PACKET_READY_END)
        return packet->ready;

    return FALSE;
}

static void SendPacket_PickState(u32 pickState)
{
    struct PickStatePacket packet;
    packet.id = PACKET_PICK_STATE;
    packet.pickState = pickState;
    Rfu_SendPacket(&packet);
}

static bool32 RecvPacket_PickState(u32 playerId, u8 *pickState)
{
    struct PickStatePacket * packet;

    if ((gRecvCmds[0][0] & RFUCMD_MASK) != RFUCMD_SEND_PACKET)
        return FALSE;

    packet = (void *)&gRecvCmds[playerId][1];
    if (packet->id == PACKET_PICK_STATE)
    {
        *pickState = packet->pickState;
        return TRUE;
    }
    return FALSE;
}

/////////////////////////////
// COMMUNICATION FUNCTIONS //
/////////////////////////////

#define tState data[0]

static void Task_CommunicateMonInfo(u32 taskId)
{
    u32 i;
    s16 * data = gTasks[taskId].data;
    u8 blockReceivedStatus;

    switch (tState)
    {
        case 0:
            SendBlock(&sGame->members[sGame->multiplayerId].monInfo.isShiny, sizeof(sGame->members[sGame->multiplayerId].monInfo.isShiny));
            sGame->playersReceived = 0;
            tState++;
            break;
        case 1:
            if (IsLinkTaskFinished())
                tState++;
            break;
        case 2:
            blockReceivedStatus = GetBlockReceivedStatus();
            
            for (i = 0; i < sGame->numPlayers; blockReceivedStatus >>= 1, i++)
            {
                if (blockReceivedStatus & 1)
                {
                    *(u8 *)&sGame->members[i].monInfo = *(u8 *)gBlockRecvBuffer[i];
                    ResetBlockReceivedFlag(i);
                    sGame->playersReceived++;
                }
            }
            
            if (sGame->playersReceived >= sGame->numPlayers)
            {
                DestroyTask(taskId);
                SetGfxFuncById(GFXFUNC_ERASE_MSG);
                sGame->state++;
            }
            break;
    }
}

#undef tState

static void SendLinkData_Leader(void)
{
    switch (sGame->funcId)
    {
        case FUNC_PLAY_GAME:
        case FUNC_WAIT_END_GAME:
            SendPacket_GameState(&sGame->player, &sGame->members[0].player.comm, &sGame->members[1].player.comm, &sGame->members[2].player.comm, &sGame->members[3].player.comm,
                                 &sGame->members[4].player.comm, sGame->numGraySquares, sGame->berriesFalling, sGame->allReadyToEnd);
            break;
    }
}

static bool32 AllPlayersReadyToStart(void)
{
    u32 i, numPlayers = sGame->numPlayers;

    for (i = 1; i < numPlayers; i++)
    {
        if (!sGame->members[i].readyToStart)
            sGame->members[i].readyToStart = RecvPacket_ReadyToStart(i);
    }

    for (i = 1; i < numPlayers; i++)
    {
        if (!sGame->members[i].readyToStart)
            return FALSE;
    }
    return TRUE;
}

static void ResetReadyToStart(void)
{
    u32 i;

    for (i = 0; i < MAX_RFU_PLAYERS; i++)
        sGame->members[i].readyToStart = FALSE;
}

static void RecvLinkData_Gameplay(void)
{
    u32 i, numPlayers = sGame->numPlayers;

    RecvPacket_GameState(&sGame->members[0].player, &sGame->members[0].player.comm, &sGame->members[1].player.comm, &sGame->members[2].player.comm,
                         &sGame->members[3].player.comm, &sGame->members[4].player.comm);
    sGame->clearRecvCmds = TRUE;

    for (i = 1; i < numPlayers; i++)
    {
        if (sGame->members[i].inputState == INPUTSTATE_NONE && !RecvPacket_PickState(i, &sGame->members[i].player.comm.pickState))
        {
            sGame->members[i].player.comm.pickState = PICK_NONE;
            sGame->clearRecvCmds = FALSE;
        }
    }
    
    if (++sGame->clearRecvCmdTimer >= 60)
    {
        if (sGame->clearRecvCmds)
        {
            ClearRecvCommands();
            sGame->clearRecvCmdTimer = 0;
        }
        else if (sGame->clearRecvCmdTimer > 70)
        {
            ClearRecvCommands();
            sGame->clearRecvCmdTimer = 0;
        }
    }

    for (i = 0; i < numPlayers; i++)
    {
        if (sGame->members[i].player.comm.pickState != PICK_NONE && sGame->members[i].inputState == INPUTSTATE_NONE)
            sGame->members[i].inputState = INPUTSTATE_TRY_PICK;

        switch (sGame->members[i].inputState)
        {
            case INPUTSTATE_NONE:
            default:
                break;
            case INPUTSTATE_TRY_PICK:
            case INPUTSTATE_PICKED:
            case INPUTSTATE_ATE_BERRY:
                if (++sGame->members[i].inputDelay >= 6)
                {
                    sGame->members[i].inputDelay = 0;
                    sGame->members[i].inputState = INPUTSTATE_NONE;
                    sGame->members[i].player.comm.pickState = PICK_NONE;
                    sGame->members[i].player.comm.ateBerry = FALSE;
                    sGame->members[i].player.comm.missedBerry = FALSE;
                }
                break;
            case INPUTSTATE_BAD_MISS:
                // Tried to pick with no berry in range, long delay until next input
                if (++sGame->members[i].inputDelay >= 40)
                {
                    sGame->members[i].inputDelay = 0;
                    sGame->members[i].inputState = INPUTSTATE_NONE;
                    sGame->members[i].player.comm.pickState = PICK_NONE;
                    sGame->members[i].player.comm.ateBerry = FALSE;
                    sGame->members[i].player.comm.missedBerry = FALSE;
                }
                break;
        }
    }
}

static void RecvLinkData_ReadyToEnd(void)
{
    u32 i, numPlayers = sGame->numPlayers;

    RecvPacket_GameState(&sGame->members[0].player, &sGame->members[0].player.comm, &sGame->members[1].player.comm, &sGame->members[2].player.comm,
                         &sGame->members[3].player.comm, &sGame->members[4].player.comm);
    sGame->clearRecvCmds = TRUE;

    for (i = 1; i < numPlayers; i++)
    {
        if (RecvPacket_ReadyToEnd(i))
        {
            sGame->members[i].readyToEnd = TRUE;
            sGame->clearRecvCmds = FALSE;
        }
    }
    
    if (++sGame->clearRecvCmdTimer >= 60)
    {
        if (sGame->clearRecvCmds)
        {
            ClearRecvCommands();
            sGame->clearRecvCmdTimer = 0;
        }
        else if (sGame->clearRecvCmdTimer > 70)
        {
            ClearRecvCommands();
            sGame->clearRecvCmdTimer = 0;
        }
    }
}

static void RecvLinkData_Leader(void)
{
    switch (sGame->funcId)
    {
        case FUNC_WAIT_START:
            if (AllPlayersReadyToStart())
            {
                ResetReadyToStart();
                sGame->startGame = TRUE;
            }
            break;
        case FUNC_PLAY_GAME:
            RecvLinkData_Gameplay();
            break;
        case FUNC_WAIT_END_GAME:
            RecvLinkData_ReadyToEnd();
            break;
    }
}

static void UpdateGame(void)
{
    UpdateBerrySprites();
    
    if (sGame->numGraySquares >= NUM_STATUS_SQUARES)
        SetAllDodrioDisabled();
    else
        UpdateAllDodrioAnims();

    UpdateStatusBarAnim(sGame->numGraySquares);
}

static void Task_DodrioGame_Leader(u32 taskId)
{
    RecvLinkData_Leader();
    
    sLeaderFuncs[sGame->funcId]();
    
    if (!sGame->exitingGame)
        UpdateGame();
    
    SendLinkData_Leader();
}

static void SendLinkData_Member(void)
{
    switch (sGame->funcId)
    {
        case FUNC_WAIT_START:
            SendPacket_ReadyToStart(TRUE);
            sGame->startGame = TRUE;
            break;
        case FUNC_PLAY_GAME:
            if (sGame->player.comm.pickState != PICK_NONE)
                SendPacket_PickState(sGame->player.comm.pickState);
            break;
        case FUNC_WAIT_END_GAME:
            if (!sGame->berriesFalling && !sGame->allReadyToEnd)
                SendPacket_ReadyToEnd(TRUE);
            break;
    }
}

static void RecvLinkData_Member(void)
{
    switch (sGame->funcId)
    {
        case FUNC_PLAY_GAME:
        case FUNC_WAIT_END_GAME:
            RecvPacket_GameState(&sGame->members[sGame->multiplayerId].player, &sGame->members[0].player.comm, &sGame->members[1].player.comm, &sGame->members[2].player.comm,
                                 &sGame->members[3].player.comm, &sGame->members[4].player.comm);
            break;
    }
}

static void Task_DodrioGame_Member(u32 taskId)
{
    RecvLinkData_Member();
    
    sMemberFuncs[sGame->funcId]();
    
    if (!sGame->exitingGame)
        UpdateGame();
    
    SendLinkData_Member();
}

static void SetGameFunc(u32 funcId)
{
    sGame->funcId = funcId;
    sGame->state = 0;
    sGame->timer = 0;
}

static void DoGameIntro(void)
{
    switch (sGame->state)
    {
        case 0:
            StartDodrioIntroAnim();
            SetGfxFuncById(GFXFUNC_SHOW_NAMES);
            sGame->state++;
            break;
        case 1:
            if (!IsGfxFuncActive())
                SetGameFunc(FUNC_INIT_COUNTDOWN);
            break;
    }
}

static void InitCountdown(void)
{
    switch (sGame->state)
    {
        case 0:
            InitFirstWaveOfBerries();
            sGame->state++;
            break;
        default:
            SetGameFunc(FUNC_COUNTDOWN);
            break;
    }
}

static void DoCountdown(void)
{
    switch (sGame->state)
    {
        case 0:
            StartMinigameCountdown(GFXTAG_COUNTDOWN, PALTAG_COUNTDOWN, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, 0);
            sGame->state++;
            break;
        case 1:
            Rfu_SetLinkStandbyCallback();
            sGame->state++;
            break;
        case 2:
            if (IsLinkTaskFinished())
                sGame->state++;
            break;
        case 3:
            if (!IsMinigameCountdownRunning())
            {
                Rfu_SetLinkStandbyCallback();
                sGame->state++;
            }
            break;
        case 4:
            if (IsLinkTaskFinished())
                SetGameFunc(FUNC_WAIT_START);
            break;
    }
}

static void WaitGameStart(void)
{
    switch (sGame->state)
    {
        case 0:
            if (sGame->startGame)
                SetGameFunc(FUNC_PLAY_GAME);
            break;
    }
}

// Enqueue the given state, and dequeue and return the state that should be used next
static u32 UpdatePickStateQueue(u32 pickState)
{
    u32 i, nextState = sGame->pickStateQueue[ARRAY_COUNT(sGame->pickStateQueue) - 1];

    for (i = ARRAY_COUNT(sGame->pickStateQueue) - 1; i != 0; i--)
        sGame->pickStateQueue[i] = sGame->pickStateQueue[i - 1];
    
    sGame->pickStateQueue[0] = pickState;
    
    return nextState;
}

static void HandleSound_Leader(void)
{
    if (sGame->members[sGame->multiplayerId].player.comm.pickState == PICK_NONE)
    {
        if (!IsSEPlaying())
            sGame->playingPickSound = FALSE;
    }
    else if (sGame->members[sGame->multiplayerId].player.comm.ateBerry == TRUE)
    {
        if (!sGame->playingPickSound)
        {
            m4aSongNumStop(SE_SUCCESS);
            PlaySE(SE_SUCCESS);
            sGame->playingPickSound = TRUE;
        }
    }
    else if (sGame->members[sGame->multiplayerId].player.comm.missedBerry == TRUE)
    {
        if (!sGame->playingPickSound && !IsSEPlaying())
        {
            PlaySE(SE_BOO);
            StartDodrioMissedAnim();
            sGame->playingPickSound = TRUE;
        }
    }

    if (sGame->endSoundState == 0 && sGame->numGraySquares >= NUM_STATUS_SQUARES)
    {
        // Ready to play game over sound
        StopMapMusic();
        sGame->endSoundState = 1;
    }
    else if (sGame->endSoundState == 1)
    {
        // Play game over sound
        PlayFanfareByFanfareNum(FANFARE_TOO_BAD);
        sGame->endSoundState = 2;
    }
}

static void PlayGame_Leader(void)
{
    switch (sGame->state)
    {
        case 0:
            if (sGame->numGraySquares < NUM_STATUS_SQUARES)
            {
                if (sGame->members[0].inputState == INPUTSTATE_NONE)
                {
                    if (JOY_NEW(DPAD_UP))
                    {
                        if (sGame->members[0].player.comm.pickState == PICK_NONE)
                        {
                            sGame->members[0].player.comm.ateBerry = FALSE;
                            sGame->members[0].player.comm.pickState = UpdatePickStateQueue(PICK_MIDDLE);
                        }
                    }
                    else if (JOY_NEW(DPAD_RIGHT))
                    {
                        if (sGame->members[0].player.comm.pickState == PICK_NONE)
                        {
                            sGame->members[0].player.comm.ateBerry = FALSE;
                            sGame->members[0].player.comm.pickState = UpdatePickStateQueue(PICK_RIGHT);
                        }
                    }
                    else if (JOY_NEW(DPAD_LEFT))
                    {
                        if (sGame->members[0].player.comm.pickState == PICK_NONE)
                        {
                            sGame->members[0].player.comm.ateBerry = FALSE;
                            sGame->members[0].player.comm.pickState = UpdatePickStateQueue(PICK_LEFT);
                        }
                    }
                    else
                        sGame->members[0].player.comm.pickState = UpdatePickStateQueue(PICK_NONE);
                }
            }
            else
                SetGameFunc(FUNC_WAIT_END_GAME);

            UpdateFallingBerries();
            HandleSound_Leader();
            break;
    }
}

static void HandleSound_Member(void)
{
    u32 i;
    
    if (sGame->members[sGame->multiplayerId].player.comm.pickState == PICK_NONE)
    {
        if (sGame->members[sGame->multiplayerId].player.comm.ateBerry != TRUE && sGame->members[sGame->multiplayerId].player.comm.missedBerry != TRUE)
            sGame->playingPickSound = FALSE;
    }
    else if (sGame->members[sGame->multiplayerId].player.comm.ateBerry == TRUE)
    {
        if (!sGame->playingPickSound)
        {
            m4aSongNumStop(SE_SUCCESS);
            PlaySE(SE_SUCCESS);
            sGame->playingPickSound = TRUE;
        }
    }
    else if (sGame->members[sGame->multiplayerId].player.comm.missedBerry == TRUE)
    {
        if (!sGame->playingPickSound && !IsSEPlaying())
        {
            PlaySE(SE_BOO);
            StartDodrioMissedAnim();
            sGame->playingPickSound = TRUE;
        }
    }
    
    for (i = sGame->berryColStart; i < sGame->berryColEnd; i++)
    {
        if (sGame->members[sGame->multiplayerId].player.berries[i].fallDist >= MAX_FALL_DIST)
        {
            if (!sGame->berryColumns[i].playingSquishSound)
            {
                sGame->berryColumns[i].playingSquishSound = TRUE;
                PlaySE(SE_BALLOON_RED + sGame->members[sGame->multiplayerId].player.berries[i].id);
            }
        }
        else
            sGame->berryColumns[i].playingSquishSound = FALSE;
    }
    
    if (sGame->endSoundState == 0 && sGame->numGraySquares >= NUM_STATUS_SQUARES)
    {
        // Ready to play game over sound
        StopMapMusic();
        sGame->endSoundState = 1;
    }
    else if (sGame->endSoundState == 1)
    {
        // Play game over sound
        PlayFanfareByFanfareNum(FANFARE_TOO_BAD);
        sGame->endSoundState = 2;
    }
}

static void PlayGame_Member(void)
{
    if (sGame->numGraySquares < NUM_STATUS_SQUARES)
    {
        if (JOY_NEW(DPAD_UP))
        {
            if (sGame->members[sGame->multiplayerId].player.comm.pickState == PICK_NONE)
                sGame->player.comm.pickState = PICK_MIDDLE;
        }
        else if (JOY_NEW(DPAD_RIGHT))
        {
            if (sGame->members[sGame->multiplayerId].player.comm.pickState == PICK_NONE)
                sGame->player.comm.pickState = PICK_RIGHT;
        }
        else if (JOY_NEW(DPAD_LEFT))
        {
            if (sGame->members[sGame->multiplayerId].player.comm.pickState == PICK_NONE)
                sGame->player.comm.pickState = PICK_LEFT;
        }
        else
            sGame->player.comm.pickState = PICK_NONE;
    }
    else
        SetGameFunc(FUNC_WAIT_END_GAME);
    
    HandleSound_Member();
}

static void InitResults_Leader(void)
{
    u32 i;
    u8 blockReceivedStatus;
    
    switch (sGame->state)
    {
        case 0:
            SendBlock(sGame->berryResults, sizeof(sGame->berryResults));
            sGame->playersReceived = 0;
            sGame->state++;
            break;
        case 1:
            if (IsLinkTaskFinished())
                sGame->state++;
            break;
        case 2:
            blockReceivedStatus = GetBlockReceivedStatus();
            
            for (i = 0; i < sGame->numPlayers; blockReceivedStatus >>= 1, i++)
            {
                if (blockReceivedStatus & 1)
                {
                    ResetBlockReceivedFlag(i);
                    sGame->playersReceived++;
                }
            }
            
            if (sGame->playersReceived >= sGame->numPlayers)
            {
                sGame->timer++;
                sGame->state++;
            }
            break;
        default:
            if (WaitFanfare(TRUE))
            {
                SetGameFunc(FUNC_RESULTS);
                FadeOutAndPlayNewMapMusic(MUS_VICTORY_WILD, 4);
            }
            break;
    }
}

static void InitResults_Member(void)
{
    u32 i;
    u8 blockReceivedStatus;

    switch (sGame->state)
    {
        case 0:
            SendBlock(sGame->berryResults[sGame->timer], sizeof(sGame->berryResults));
            sGame->playersReceived = 0;
            sGame->state++;
            break;
        case 1:
            if (IsLinkTaskFinished())
                sGame->state++;
            break;
        case 2:
            blockReceivedStatus = GetBlockReceivedStatus();
            
            for (i = 0; i < sGame->numPlayers; blockReceivedStatus >>= 1, i++)
            {
                if (blockReceivedStatus & 1)
                {
                    memcpy(sGame->berryResults, gBlockRecvBuffer, sizeof(sGame->berryResults));
                    ResetBlockReceivedFlag(i);
                    sGame->playersReceived++;
                }
            }
            
            if (sGame->playersReceived >= sGame->numPlayers)
            {
                sGame->timer++;
                sGame->state++;
            }
            break;
        default:
            if (WaitFanfare(TRUE))
            {
                sGame->maxBerriesPickedInRow = sGame->berryResults[sGame->multiplayerId][BERRY_IN_ROW];
                SetGameFunc(FUNC_RESULTS);
                FadeOutAndPlayNewMapMusic(MUS_VICTORY_WILD, 4);
            }
            break;
    }
}

static void DoResults(void)
{
    u32 i;
    u8 playAgainState, blockReceivedStatus;

    switch (sGame->state)
    {
        case 0:
            TryUpdateRecords();
            SetStatusBarInvisibility(TRUE);
            ResetCloudPos();
            SetCloudInvisibility(TRUE);
            SetGfxFuncById(GFXFUNC_SHOW_RESULTS);
            sGame->state++;
            break;
        case 1:
            if (!IsGfxFuncActive())
            {
                playAgainState = PLAY_AGAIN_YES;
                SetGfxFuncById(GFXFUNC_MSG_COMM_STANDBY);
                playAgainState = sGame->gfx.playAgainState;
                SendBlock(&playAgainState, sizeof(playAgainState));
                sGame->state++;
            }
            break;
        case 2:
            if (IsLinkTaskFinished())
            {
                sGame->state++;
                sGame->playersReceived = 0;
            }
            break;
        case 3:
            blockReceivedStatus = GetBlockReceivedStatus();
            
            for (i = 0; i < sGame->numPlayers; blockReceivedStatus >>= 1, i++)
            {
                if (blockReceivedStatus & 1)
                {
                    sGame->members[i].playAgainState = *(u8 *)gBlockRecvBuffer[i];
                    ResetBlockReceivedFlag(i);
                    sGame->playersReceived++;
                }
            }
            
            if (sGame->playersReceived >= sGame->numPlayers)
            {
                if (++sGame->timer >= 120)
                {
                    SetGfxFuncById(GFXFUNC_ERASE_MSG);
                    sGame->state++;
                }
            }
            break;
        default:
            if (!IsGfxFuncActive())
                SetGameFunc(FUNC_ASK_PLAY_AGAIN);
            break;
    }
}

static void ResetBerryAndStatusBarSprites(void)
{
    u32 i;
    
    for (i = 0; i < NUM_BERRY_COLUMNS; i++)
    {
        SetBerryInvisibility(i, TRUE);
        SetBerryYPos(i, 1);
    }
    SetStatusBarInvisibility(FALSE);
}

static void ResetForPlayAgainPrompt(void)
{
    u32 i, j;

    for (i = 0; i < MAX_RFU_PLAYERS; i++)
    {
        for (j = 0; j < NUM_BERRY_COLUMNS; j++)
            sGame->members[i].player.berries[j].fallDist = 0;
        
        sGame->members[i].player.comm.pickState = PICK_NONE;
        sGame->members[i].player.comm.ateBerry = FALSE;
        sGame->members[i].difficulty = 0;
        sGame->members[i].berriesEaten = 0;
        sGame->members[i].scoreResults.ranking = 0;
        sGame->members[i].scoreResults.score = 0;
        sGame->berryResults[i][BERRY_BLUE] = 0;
        sGame->berryResults[i][BERRY_GREEN] = 0;
        sGame->berryResults[i][BERRY_GOLD] = 0;
        sGame->berryResults[i][BERRY_MISSED] = 0;
        sGame->berryResults[i][BERRY_PRIZE] = 0;
        sGame->berryResults[i][BERRY_IN_ROW] = 0;
    }
    sGame->endSoundState = 0;
    sGame->berriesPickedInRow = 0;
    sGame->numGraySquares = 0;
    
    UpdateAllDodrioAnims();
    UpdateBerrySprites();
}

// The player may extend their Dodrio's heads while they wait for
// other players to respond to the "Play again?" prompt
static void HandleWaitPlayAgainInput(void)
{
    if (sGame->members[sGame->multiplayerId].inputDelay == 0)
    {
        if (JOY_NEW(DPAD_UP))
        {
            sGame->members[sGame->multiplayerId].player.comm.pickState = PICK_MIDDLE;
            sGame->members[sGame->multiplayerId].inputDelay = 6;
            PlaySE(SE_M_CHARM);
        }
        else if (JOY_NEW(DPAD_LEFT))
        {
            sGame->members[sGame->multiplayerId].player.comm.pickState = PICK_LEFT;
            sGame->members[sGame->multiplayerId].inputDelay = 6;
            PlaySE(SE_M_CHARM);
        }
        else if (JOY_NEW(DPAD_RIGHT))
        {
            sGame->members[sGame->multiplayerId].player.comm.pickState = PICK_RIGHT;
            sGame->members[sGame->multiplayerId].inputDelay = 6;
            PlaySE(SE_M_CHARM);
        }
        else
            sGame->members[sGame->multiplayerId].player.comm.pickState = PICK_NONE;
    }
    else
        sGame->members[sGame->multiplayerId].inputDelay--;
}

static void AskPlayAgain(void)
{
    u32 i;
    u8 playAgainState, blockReceivedStatus;

    switch (sGame->state)
    {
        case 0:
            if (GetHighestScore() >= PRIZE_SCORE)
                SetGfxFuncById(GFXFUNC_MSG_SAVING);
            sGame->state++;
            break;
        case 1:
            if (!IsGfxFuncActive())
            {
                SetGfxFuncById(GFXFUNC_MSG_PLAY_AGAIN);
                sGame->state++;
            }
            break;
        case 2:
            ResetBerryAndStatusBarSprites();
            ResetForPlayAgainPrompt();
            sGame->state++;
            break;
        case 3:
            if ((playAgainState = sGame->gfx.playAgainState) != PLAY_AGAIN_NONE)
                sGame->state++;
            break;
        case 4:
            if (!IsGfxFuncActive())
            {
                SetGfxFuncById(GFXFUNC_MSG_COMM_STANDBY);
                playAgainState = sGame->gfx.playAgainState;
                SendBlock(&playAgainState, sizeof(playAgainState));
                sGame->state++;
            }
            break;
        case 5:
            if (IsLinkTaskFinished())
            {
                sGame->state++;
                sGame->playersReceived = 0;
            }
            break;
        case 6:
            blockReceivedStatus = GetBlockReceivedStatus();
            
            for (i = 0; i < sGame->numPlayers; blockReceivedStatus >>= 1, i++)
            {
                if (blockReceivedStatus & 1)
                {
                    sGame->members[i].playAgainState = *(u8 *)gBlockRecvBuffer[i];
                    ResetBlockReceivedFlag(i);
                    sGame->playersReceived++;
                }
            }
            
            if (sGame->playersReceived >= sGame->numPlayers)
            {
                if (++sGame->timer >= 120)
                {
                    sGame->members[sGame->multiplayerId].player.comm.pickState = PICK_NONE;
                    SetGfxFuncById(GFXFUNC_ERASE_MSG);
                    sGame->state++;
                }
            }
            else
                HandleWaitPlayAgainInput();
            break;
        default:
            if (!IsGfxFuncActive())
            {
                for (i = 0; i < sGame->numPlayers; i++)
                {
                    if (sGame->members[i].playAgainState == PLAY_AGAIN_NO)
                    {
                        SetGameFunc(FUNC_END_LINK);
                        return;
                    }
                }
                SetGameFunc(FUNC_RESET_GAME);
            }
            break;
    }
}

static void EndLink(void)
{
    switch (sGame->state)
    {
        case 0:
            SetCloseLinkCallback();
            SetGfxFuncById(GFXFUNC_MSG_PLAYER_DROPPED);
            sGame->state++;
            break;
        case 1:
            if (!IsGfxFuncActive())
                sGame->state++;
            break;
        case 2:
            if (sGame->gfx.playAgainState == PLAY_AGAIN_DROPPED)
                sGame->state++;
            break;
        default:
            if (!gReceivedRemoteLinkPlayers)
                SetGameFunc(FUNC_EXIT);
            break;
    }
}

static void ExitGame(void)
{
    switch (sGame->state)
    {
        case 0:
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            sGame->state++;
            break;
        case 1:
            UpdatePaletteFade();
            
            if (!gPaletteFade.active)
                sGame->state++;
            break;
        case 2:
            DestroyBerrySprites();
            DestroyStatusBar();
            DestroyDodrioSprites(sGame->numPlayers);
            DestroyCloudSprites();
            sGame->exitingGame = TRUE;
            SetGfxFuncById(GFXFUNC_STOP);
            sGame->state++;
            break;
        default:
            if (!IsGfxFuncActive())
            {
                SetMainCallback2(sGame->exitCallback);
                DestroyTask(sGame->taskId);
                Free(sGame);
                FreeAllWindowBuffers();
            }
            break;
    }
}

static void ResetGame(void)
{
    switch (sGame->state)
    {
        case 0:
            SetGfxFuncById(GFXFUNC_IDLE);
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            sGame->state++;
            break;
        case 1:
        case 6:
            UpdatePaletteFade();
            
            if (!gPaletteFade.active)
                sGame->state++;
            break;
        case 2:
            ResetAllBgsPos();
            sGame->state++;
            break;
        case 3:
            StopMapMusic();
            sGame->state++;
            break;
        case 4:
            PlayNewMapMusic(MUS_BERRY_PICK);
            StartCloudMovement();
            sGame->state++;
            break;
        case 5:
            BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
            BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
            sGame->state++;
            break;
        default:
            DestroyTask(sGame->taskId);
            CreateDodrioGameTask(Task_NewGameIntro);
            ResetGfxState();
            InitDodrioGame();
            
            if (!gReceivedRemoteLinkPlayers)
                sGame->numPlayers = 1;
            
            SetRandomPrize();
            SetCloudInvisibility(FALSE);
            break;
    }
}

static bool32 ReadyToEndGame_Leader(void)
{
    if (sGame->numGraySquares >= NUM_STATUS_SQUARES && !sGame->berriesFalling)
    {
        sGame->numGraySquares = NUM_STATUS_SQUARES;
        
        if (sGame->allReadyToEnd)
            return TRUE;
    }
    return FALSE;
}

static void WaitEndGame_Leader(void)
{
    u32 i;

    UpdateFallingBerries();
    HandleSound_Leader();
    
    if (ReadyToEndGame_Leader())
    {
        SetMaxBerriesPickedInRow();
        SetGameFunc(FUNC_INIT_RESULTS);
    }
    else
    {
        sGame->allReadyToEnd = TRUE;
        
        for (i = 1; i < sGame->numPlayers; i++)
        {
            if (!sGame->members[i].readyToEnd)
            {
                sGame->allReadyToEnd = FALSE;
                break;
            }
        }
    }
}

static bool32 ReadyToEndGame_Member(void)
{
    u32 i;

    if (sGame->numGraySquares >= NUM_STATUS_SQUARES)
    {
        sGame->numGraySquares = NUM_STATUS_SQUARES;
        
        if (sGame->allReadyToEnd)
        {
            for (i = sGame->berryColStart; i < sGame->berryColEnd; i++)
            {
                struct DodrioGame_Player * player = &sGame->members[sGame->multiplayerId].player;
                u32 column = sActiveColumnMap[sGame->numPlayers - 1][sGame->multiplayerId][i];

                if (player->berries[column].fallDist != MAX_FALL_DIST)
                    return FALSE;
            }
            return TRUE;
        }
    }
    return FALSE;
}

static void WaitEndGame_Member(void)
{
    HandleSound_Member();
    
    if (ReadyToEndGame_Member())
        SetGameFunc(FUNC_INIT_RESULTS);
}

///////////////////
// GFX FUNCTIONS //
///////////////////

static void Task_TryRunGfxFunc(u32 taskId)
{
    // Continue calling function until it
    // has reached its finished state.
    // Another will not be called until
    // readied by SetGfxFuncById
    if (!sGame->gfx.finished)
        sGame->gfx.func();
}

static void InitGameGfx(void)
{
    sGame->gfx.playAgainState = PLAY_AGAIN_NONE;
    sGame->gfx.taskId = CreateTask(Task_TryRunGfxFunc, 3);
    SetGfxFuncById(GFXFUNC_LOAD);
}

static void ResetGfxState(void)
{
    sGame->gfx.finished = FALSE;
    sGame->gfx.state = 0;
    sGame->gfx.loadState = 0;
    sGame->gfx.cursorSelection = 0;
    sGame->gfx.playAgainState = PLAY_AGAIN_NONE;
}

static void SetGfxFuncById(u32 id)
{
    u32 i;
    
    for (i = 0; i < ARRAY_COUNT(sGfxFuncs); i++)
    {
        if (sGfxFuncs[i].id == id)
        {
            sGame->gfx.state = 0;
            sGame->gfx.finished = FALSE;
            sGame->gfx.func = sGfxFuncs[i].func;
            break;
        }
    }
}

static bool32 IsGfxFuncActive(void)
{
    return !sGame->gfx.finished;
}

static void InitBgs(void)
{
    DmaClearLarge16(3, (void *)VRAM, VRAM_SIZE, 0x1000);
    DmaClear32(3,(void *)OAM, OAM_SIZE);
    DmaClear16(3, (void *)PLTT, PLTT_SIZE);
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    ResetBgsAndClearDma3BusyFlags(FALSE);
    InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
    ChangeBgX(BG_INTERFACE, 0, 0);
    ChangeBgY(BG_INTERFACE, 0, 0);
    ChangeBgX(BG_TREE_LEFT, 0, 0);
    ChangeBgY(BG_TREE_LEFT, 0, 0);
    ChangeBgX(BG_TREE_RIGHT, 0, 0);
    ChangeBgY(BG_TREE_RIGHT, 0, 0);
    ChangeBgX(BG_SCENERY, 0, 0);
    ChangeBgY(BG_SCENERY, 0, 0);
    InitStandardTextBoxWindows();
    InitTextBoxGfxAndPrinters();
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    SetBgTilemapBuffer(BG_SCENERY, sGame->gfx.tilemapBuffers[0]);
    SetBgTilemapBuffer(BG_TREE_LEFT, sGame->gfx.tilemapBuffers[1]);
    SetBgTilemapBuffer(BG_TREE_RIGHT, sGame->gfx.tilemapBuffers[2]);
}

static bool32 LoadBgGfx(void)
{
    switch (sGame->gfx.loadState)
    {
        case 0:
            LoadPalette(sBg_Pal, 0, sizeof(sBg_Pal));
            break;
        case 1:
            ResetTempTileDataBuffers();
            break;
        case 2:
            DecompressAndCopyTileDataToVram(BG_SCENERY, sBg_Gfx, 0, 0, 0);
            break;
        case 3:
            DecompressAndCopyTileDataToVram(BG_TREE_LEFT, sTreeBorder_Gfx, 0, 0, 0);
            break;
        case 4:
            if (FreeTempTileDataBuffersIfPossible())
                return FALSE;
            break;
        case 5:
            LoadPalette(stdpal_get(3), 0xD0, 0x20);
            break;
        default:
            sGame->gfx.loadState = 0;
            return TRUE;
    }
    sGame->gfx.loadState++;
    return FALSE;
}

static void LoadGfx(void)
{
    switch (sGame->gfx.state)
    {
        case 0:
            InitBgs();
            sGame->gfx.state++;
            break;
        case 1:
            if (LoadBgGfx())
                sGame->gfx.state++;
            break;
        case 2:
            CopyToBgTilemapBuffer(BG_SCENERY, sBg_Tilemap, 0, 0);
            CopyToBgTilemapBuffer(BG_TREE_LEFT, sTreeBorderLeft_Tilemap, 0, 0);
            CopyToBgTilemapBuffer(BG_TREE_RIGHT, sTreeBorderRight_Tilemap, 0, 0);
            CopyBgTilemapBufferToVram(BG_SCENERY);
            CopyBgTilemapBufferToVram(BG_TREE_LEFT);
            CopyBgTilemapBufferToVram(BG_TREE_RIGHT);
            sGame->gfx.state++;
            break;
        case 3:
            ShowBg(BG_INTERFACE);
            ShowBg(BG_SCENERY);
            ShowBg(BG_TREE_LEFT);
            ShowBg(BG_TREE_RIGHT);
            sGame->gfx.state++;
            break;
        case 4:
            DrawUserFrameGraphicsToBg0(1, 0xA0);
            TextWindow_SetStdFrame0_WithPal(0, 0xA, 0xB0);
            sGame->gfx.state++;
            break;
        default:
            sGame->gfx.finished = TRUE;
            break;
    }
}

static u8 *GetPlayerName(u32 id)
{
    if (gReceivedRemoteLinkPlayers)
        return gLinkPlayers[id].name;
    else
        return sGame->members[id].player.name;
}

static u32 GetPlayerTextColorsId(u32 playerId)
{
    return playerId == GetMultiplayerId() ? COLORID_BLUE : COLORID_GRAY;
}

static inline void DrawMessageWindow(u32 windowId)
{
    DrawTextBorderOuter(windowId, 10, 11);
}

static void ShowNames(void)
{
    u32 i, numPlayers;
    u8 *name;
    struct WindowTemplate window;
    const struct WinCoords * coords;

    switch (sGame->gfx.state)
    {
        case 0:
            numPlayers = sGame->numPlayers;
            coords = sNameWindowCoords[numPlayers - 1];
            
            window.bg = BG_INTERFACE;
            window.width = 7;
            window.height = 2;
            window.paletteNum = 13;
            window.baseBlock = 0x13;
            
            for (i = 0; i < numPlayers; coords++, i++)
            {
                u32 left, playerId = sGame->members[i].posToPlayerId;
                
                window.tilemapLeft = coords->left;
                window.tilemapTop = coords->top;
                
                sGame->gfx.windowIds[i] = AddWindow(&window);
                ClearWindowTilemap(sGame->gfx.windowIds[i]);
                FillWindowPixelBuffer(sGame->gfx.windowIds[i], PIXEL_FILL(1));
                
                name = GetPlayerName(playerId);
                left = (56 - GetStringWidth(FONT_SMALL, name, -1)) / 2u;
                AddTextPrinterParameterized3(sGame->gfx.windowIds[i], FONT_SMALL, left, 1, sTextColorTable[GetPlayerTextColorsId(playerId)], 0xFF, name);
                CopyWindowToVram(sGame->gfx.windowIds[i], COPYWIN_GFX);
                
                window.baseBlock += 0xE;
                
                DrawMessageWindow(sGame->gfx.windowIds[i]);
            }
            sGame->gfx.state++;
            break;
        case 1:
            if (!IsDma3ManagerBusyWithBgCopy())
            {
                numPlayers = sGame->numPlayers;
                
                for (i = 0; i < numPlayers; i++)
                    PutWindowTilemap(sGame->gfx.windowIds[i]);
                
                CopyBgTilemapBufferToVram(BG_INTERFACE);
                sGame->gfx.state++;
            }
            break;
        default:
            if (++sGame->gfx.state > 180)
            {
                numPlayers = sGame->numPlayers;
                
                for (i = 0; i < numPlayers; i++)
                {
                    ClearWindowTilemap(sGame->gfx.windowIds[i]);
                    RemoveWindow(sGame->gfx.windowIds[i]);
                }
                FillBgTilemapBufferRect_Palette0(BG_INTERFACE, 0, 0, 0, 30, 20);
                CopyBgTilemapBufferToVram(BG_INTERFACE);
                sGame->gfx.finished = TRUE;
            }
            break;
    }
}

static void ShowResults(void)
{
    u32 i, j, prizeState, prizeItem;
    u8 *name;
    u32 strWidth, x;

    switch (sGame->gfx.state)
    {
        case 0:
            SetScoreResults();
            sGame->gfx.timer = 0;
            sGame->gfx.state++;
            break;
        case 1:
            sGame->gfx.windowIds[0] = AddWindow(&sWindowTemplates_Results[0]);
            sGame->gfx.windowIds[1] = AddWindow(&sWindowTemplates_Results[1]);
            ClearWindowTilemap(sGame->gfx.windowIds[0]);
            ClearWindowTilemap(sGame->gfx.windowIds[1]);
            DrawMessageWindow(sGame->gfx.windowIds[0]);
            DrawMessageWindow(sGame->gfx.windowIds[1]);
            sGame->gfx.state++;
            break;
        case 2:
            FillWindowPixelBuffer(sGame->gfx.windowIds[0], PIXEL_FILL(1));
            FillWindowPixelBuffer(sGame->gfx.windowIds[1], PIXEL_FILL(1));
            
            strWidth = GetStringWidth(FONT_SMALL, sText_BerryPickingResults, -1);
            x = (DISPLAY_WIDTH - 16 - strWidth) / 2;
            AddTextPrinterParameterized(sGame->gfx.windowIds[0], FONT_SMALL, sText_BerryPickingResults, x, 2, 0xFF, NULL);
            AddTextPrinterParameterized(sGame->gfx.windowIds[1], FONT_SMALL, COMPOUND_STRING("10P     30P     50P     {EXTRA 0xDD}50P"), 68, 16, 0xFF, NULL);
            
            for (i = 0; i < sGame->numPlayers; i++)
            {
                u32 colorsId = GetPlayerTextColorsId(i);
    
                name = GetPlayerName(i);
                AddTextPrinterParameterized3(sGame->gfx.windowIds[1], FONT_SMALL, 2, sResultsYCoords[i], sTextColorTable[colorsId], 0xFF, name);
                
                for (j = 0; j < 4; j++)
                {
                    u32 width;
                    u32 berriesPicked = min(sGame->berryResults[i][j], MAX_BERRIES);
                    u32 maxBerriesPicked = min(GetHighestBerryResult(j), MAX_BERRIES);
    
                    ConvertIntToDecimalStringN(gStringVar1, berriesPicked, STR_CONV_MODE_LEFT_ALIGN, 4);
                    width = GetStringWidth(FONT_SMALL, gStringVar1, -1);
                    
                    // If player got the most of a berry type, highlight their number in red
                    if (maxBerriesPicked == berriesPicked && maxBerriesPicked != 0)
                        AddTextPrinterParameterized3(sGame->gfx.windowIds[1], FONT_SMALL, sResultsXCoords[j] - width, sResultsYCoords[i], sTextColorTable[COLORID_RED], 0xFF, gStringVar1);
                    else
                        AddTextPrinterParameterized(sGame->gfx.windowIds[1], FONT_SMALL, gStringVar1, sResultsXCoords[j] - width, sResultsYCoords[i], 0xFF, NULL);
                }
            }
            CopyWindowToVram(sGame->gfx.windowIds[0], COPYWIN_GFX);
            CopyWindowToVram(sGame->gfx.windowIds[1], COPYWIN_GFX);
            sGame->gfx.state++;
            break;
        case 3:
            if (!IsDma3ManagerBusyWithBgCopy())
            {
                PutWindowTilemap(sGame->gfx.windowIds[0]);
                PutWindowTilemap(sGame->gfx.windowIds[1]);
            }
            CopyBgTilemapBufferToVram(BG_INTERFACE);
            SetBerryIconsInvisibility(FALSE);
            sGame->gfx.state++;
            break;
        case 4:
            if (++sGame->gfx.timer >= 30 && JOY_NEW(A_BUTTON))
            {
                sGame->gfx.timer = 0;
                PlaySE(SE_SELECT);
                SetBerryIconsInvisibility(TRUE);
                sGame->gfx.state++;
            }
            break;
        case 5:
            FillWindowPixelBuffer(sGame->gfx.windowIds[0], PIXEL_FILL(1));
            FillWindowPixelBuffer(sGame->gfx.windowIds[1], PIXEL_FILL(1));
            
            strWidth = GetStringWidth(FONT_SMALL, sText_AnnouncingRankings, -1);
            x = (DISPLAY_WIDTH - 16 - strWidth) / 2;
            AddTextPrinterParameterized(sGame->gfx.windowIds[0], FONT_SMALL, sText_AnnouncingRankings, x, 2, 0xFF, NULL);
            sGame->gfx.state++;
            break;
        case 6:
            PrintRankedScores();
            CopyWindowToVram(sGame->gfx.windowIds[0], COPYWIN_GFX);
            CopyWindowToVram(sGame->gfx.windowIds[1], COPYWIN_GFX);
            sGame->gfx.state++;
            break;
        case 7:
            if (!IsDma3ManagerBusyWithBgCopy())
            {
                PutWindowTilemap(sGame->gfx.windowIds[0]);
                PutWindowTilemap(sGame->gfx.windowIds[1]);
            }
            CopyBgTilemapBufferToVram(0);
            sGame->gfx.state++;
            break;
        case 8:
            if (++sGame->gfx.timer >= 30 && JOY_NEW(A_BUTTON))
            {
                sGame->gfx.timer = 0;
                
                PlaySE(SE_SELECT);
                
                if (GetHighestScore() < PRIZE_SCORE)
                    sGame->gfx.state = 127; // Skip to end, past giving prize
                else
                {
                    StopMapMusic();
                    sGame->gfx.state++;
                }
                FillBgTilemapBufferRect_Palette0(BG_INTERFACE, 0, 0, 5, 30, 15);
                RemoveWindow(sGame->gfx.windowIds[1]);
                sGame->gfx.windowIds[1] = AddWindow(&sWindowTemplate_Prize);
                ClearWindowTilemap(sGame->gfx.windowIds[1]);
                DrawMessageWindow(sGame->gfx.windowIds[1]);
            }
            break;
        case 9:
            PlayNewMapMusic(MUS_LEVEL_UP);
            
            FillWindowPixelBuffer(sGame->gfx.windowIds[0], PIXEL_FILL(1));
            FillWindowPixelBuffer(sGame->gfx.windowIds[1], PIXEL_FILL(1));
            
            strWidth = GetStringWidth(FONT_SMALL, sText_AnnouncingPrizes, -1);
            x = (DISPLAY_WIDTH - 16 - strWidth) / 2;
            AddTextPrinterParameterized(sGame->gfx.windowIds[0], FONT_SMALL, sText_AnnouncingPrizes, x, 2, 0xFF, NULL);
            
            prizeItem = GetPrizeItemId();
            CopyItemName(prizeItem, gStringVar1);
            StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("The first-place winner gets\nthis {STR_VAR_1}!"));
            AddTextPrinterParameterized(sGame->gfx.windowIds[1], FONT_SMALL, gStringVar4, 8, 2, 0xFF, NULL);
            
            switch (TryGivePrize(prizeItem))
            {
                case PRIZE_NO_ROOM:
                    AddTextPrinterParameterized(sGame->gfx.windowIds[1], FONT_SMALL, COMPOUND_STRING("You can't hold any more..."), 8, 40, 0xFF, NULL);
                    break;
                case PRIZE_FILLED_BAG:
                    AddTextPrinterParameterized(sGame->gfx.windowIds[1], FONT_SMALL, COMPOUND_STRING("It filled its storage space."), 8, 40, 0xFF, NULL);
                    break;
            }
            CopyWindowToVram(sGame->gfx.windowIds[0], COPYWIN_GFX);
            CopyWindowToVram(sGame->gfx.windowIds[1], COPYWIN_GFX);
            sGame->gfx.state++;
            break;
        case 10:
            if (!IsDma3ManagerBusyWithBgCopy())
            {
                PutWindowTilemap(sGame->gfx.windowIds[0]);
                PutWindowTilemap(sGame->gfx.windowIds[1]);
            }
            CopyBgTilemapBufferToVram(BG_INTERFACE);
            FadeOutAndFadeInNewMapMusic(MUS_VICTORY_WILD, 20, 10);
            sGame->gfx.state++;
            break;
        case 11:
            if (++sGame->gfx.timer >= 30 && JOY_NEW(A_BUTTON))
            {
                sGame->gfx.timer = 0;
                PlaySE(SE_SELECT);
                sGame->gfx.state++;
            }
            break;
        default:
            ClearWindowTilemap(sGame->gfx.windowIds[0]);
            ClearWindowTilemap(sGame->gfx.windowIds[1]);
            
            RemoveWindow(sGame->gfx.windowIds[0]);
            RemoveWindow(sGame->gfx.windowIds[1]);
            
            FillBgTilemapBufferRect_Palette0(BG_INTERFACE, 0, 0, 0, 30, 20);
            CopyBgTilemapBufferToVram(BG_INTERFACE);
            sGame->gfx.finished = TRUE;
            break;
    }
}

static void Msg_WantToPlayAgain(void)
{
    u32 y;

    switch (sGame->gfx.state)
    {
        case 0:
            // Create windows
            sGame->gfx.windowIds[WIN_PLAY_AGAIN] = AddWindow(&sWindowTemplates_PlayAgain[WIN_PLAY_AGAIN]);
            sGame->gfx.windowIds[WIN_YES_NO] = AddWindow(&sWindowTemplates_PlayAgain[WIN_YES_NO]);
            
            ClearWindowTilemap(sGame->gfx.windowIds[WIN_PLAY_AGAIN]);
            ClearWindowTilemap(sGame->gfx.windowIds[WIN_YES_NO]);
            
            DrawMessageWindow(sGame->gfx.windowIds[WIN_PLAY_AGAIN]);
            DrawTextBorderOuter(sGame->gfx.windowIds[WIN_YES_NO], 1, 10);
            
            sGame->gfx.state++;
            sGame->gfx.cursorSelection = PLAY_AGAIN_NONE;
            sGame->gfx.playAgainState = PLAY_AGAIN_NONE;
            break;
        case 1:
            // Print text
            FillWindowPixelBuffer(sGame->gfx.windowIds[WIN_PLAY_AGAIN], PIXEL_FILL(1));
            FillWindowPixelBuffer(sGame->gfx.windowIds[WIN_YES_NO], PIXEL_FILL(1));
            
            AddTextPrinterParameterized(sGame->gfx.windowIds[WIN_PLAY_AGAIN], FONT_NORMAL, gText_WantToPlayAgain, 0, 6, 0xFF, NULL);
            AddTextPrinterParameterized(sGame->gfx.windowIds[WIN_YES_NO], FONT_NORMAL, gText_Yes, 8, 2, 0xFF, NULL);
            AddTextPrinterParameterized(sGame->gfx.windowIds[WIN_YES_NO], FONT_NORMAL, gText_No, 8, 16, 0xFF, NULL);
            AddTextPrinterParameterized(sGame->gfx.windowIds[WIN_YES_NO], FONT_NORMAL, gText_SelectorArrow2, 0, 2, 0xFF, NULL);
            
            CopyWindowToVram(sGame->gfx.windowIds[WIN_PLAY_AGAIN], COPYWIN_GFX);
            CopyWindowToVram(sGame->gfx.windowIds[WIN_YES_NO], COPYWIN_GFX);
            sGame->gfx.state++;
            break;
        case 2:
            // Draw windows
            if (!IsDma3ManagerBusyWithBgCopy())
            {
                PutWindowTilemap(sGame->gfx.windowIds[WIN_PLAY_AGAIN]);
                PutWindowTilemap(sGame->gfx.windowIds[WIN_YES_NO]);
            }
            CopyBgTilemapBufferToVram(BG_INTERFACE);
            sGame->gfx.state++;
            break;
        case 3:
            // Handle input
            y = sGame->gfx.cursorSelection;
            if (y == PLAY_AGAIN_NONE)
                y = PLAY_AGAIN_YES;
            
            FillWindowPixelBuffer(sGame->gfx.windowIds[WIN_YES_NO], PIXEL_FILL(1));
            AddTextPrinterParameterized(sGame->gfx.windowIds[WIN_YES_NO], FONT_NORMAL, gText_Yes, 8, 2, 0xFF, NULL);
            AddTextPrinterParameterized(sGame->gfx.windowIds[WIN_YES_NO], FONT_NORMAL, gText_No, 8, 16, 0xFF, NULL);
            AddTextPrinterParameterized(sGame->gfx.windowIds[WIN_YES_NO], FONT_NORMAL, gText_SelectorArrow2, 0, y == PLAY_AGAIN_YES ? 2 : 16, 0xFF, NULL);
            CopyWindowToVram(sGame->gfx.windowIds[WIN_YES_NO], COPYWIN_BOTH);
            
            // Increment state only if A or B button have been pressed.
            if (JOY_NEW(A_BUTTON))
            {
                PlaySE(SE_SELECT);
                
                if (sGame->gfx.cursorSelection == PLAY_AGAIN_NONE)
                    sGame->gfx.cursorSelection = PLAY_AGAIN_YES;
                
                sGame->gfx.state++;
            }
            else if (JOY_NEW(DPAD_UP | DPAD_DOWN))
            {
                PlaySE(SE_SELECT);
                
                switch (sGame->gfx.cursorSelection)
                {
                    case PLAY_AGAIN_NONE:
                        sGame->gfx.cursorSelection = PLAY_AGAIN_NO;
                        break;
                    case PLAY_AGAIN_YES:
                        sGame->gfx.cursorSelection = PLAY_AGAIN_NO;
                        break;
                    case PLAY_AGAIN_NO:
                        sGame->gfx.cursorSelection = PLAY_AGAIN_YES;
                        break;
                }
            }
            else if (JOY_NEW(B_BUTTON))
            {
                PlaySE(SE_SELECT);
                sGame->gfx.cursorSelection = PLAY_AGAIN_NO;
                sGame->gfx.state++;
            }
            break;
        default:
            sGame->gfx.playAgainState = sGame->gfx.cursorSelection;
            
            ClearWindowTilemap(sGame->gfx.windowIds[WIN_PLAY_AGAIN]);
            ClearWindowTilemap(sGame->gfx.windowIds[WIN_YES_NO]);
            
            RemoveWindow(sGame->gfx.windowIds[WIN_PLAY_AGAIN]);
            RemoveWindow(sGame->gfx.windowIds[WIN_YES_NO]);
            
            FillBgTilemapBufferRect_Palette0(BG_INTERFACE, 0, 0, 0, 30, 20);
            CopyBgTilemapBufferToVram(BG_INTERFACE);
            sGame->gfx.finished = TRUE;
            break;
    }
}

static void Msg_SavingDontTurnOff(void)
{
    switch (sGame->gfx.state)
    {
        case 0:
            DrawDialogueFrame(0, FALSE);
            AddTextPrinterParameterized2(0, FONT_NORMAL, gText_SavingDontTurnOffThePower, 0, NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
            sGame->gfx.state++;
            break;
        case 1:
            CopyWindowToVram(0, COPYWIN_BOTH);
            sGame->gfx.state++;
            break;
        case 2:
            if (!IsDma3ManagerBusyWithBgCopy())
            {
                CreateTask(Task_LinkSave, 0);
                sGame->gfx.state++;
            }
            break;
        case 3:
            if (!FuncIsActiveTask(Task_LinkSave))
                sGame->gfx.state++;
            break;
        default:
            FillBgTilemapBufferRect_Palette0(BG_INTERFACE, 0, 0, 0, 30, 20);
            CopyBgTilemapBufferToVram(BG_INTERFACE);
            sGame->gfx.finished = TRUE;
            break;
    }
}

static void Msg_CommunicationStandby(void)
{
    switch (sGame->gfx.state)
    {
        case 0:
            sGame->gfx.windowIds[0] = AddWindow(&sWindowTemplate_CommStandby);
            ClearWindowTilemap(sGame->gfx.windowIds[0]);
            DrawMessageWindow(sGame->gfx.windowIds[0]);
            sGame->gfx.state++;
            break;
        case 1:
            FillWindowPixelBuffer(sGame->gfx.windowIds[0], PIXEL_FILL(1));
            AddTextPrinterParameterized(sGame->gfx.windowIds[0], FONT_NORMAL, gText_CommunicationStandby, 0, 6, 0xFF, NULL);
            CopyWindowToVram(sGame->gfx.windowIds[0], COPYWIN_GFX);
            sGame->gfx.state++;
            break;
        case 2:
            if (!IsDma3ManagerBusyWithBgCopy())
                PutWindowTilemap(sGame->gfx.windowIds[0]);
            
            CopyBgTilemapBufferToVram(BG_INTERFACE);
            sGame->gfx.state++;
            break;
        default:
            sGame->gfx.finished = TRUE;
            break;
    }
}

static void EraseMessage(void)
{
    ClearWindowTilemap(sGame->gfx.windowIds[0]);
    RemoveWindow(sGame->gfx.windowIds[0]);
    FillBgTilemapBufferRect_Palette0(BG_INTERFACE, 0, 0, 0, 30, 20);
    CopyBgTilemapBufferToVram(BG_INTERFACE);
    sGame->gfx.finished = TRUE;
}

static void Msg_SomeoneDroppedOut(void)
{
    switch (sGame->gfx.state)
    {
        case 0:
            sGame->gfx.windowIds[0] = AddWindow(&sWindowTemplate_DroppedOut);
            ClearWindowTilemap(sGame->gfx.windowIds[0]);
            DrawMessageWindow(sGame->gfx.windowIds[0]);
            
            sGame->gfx.state++;
            sGame->gfx.timer = 0;
            sGame->gfx.cursorSelection = 0;
            sGame->gfx.playAgainState = PLAY_AGAIN_NONE;
            break;
        case 1:
            FillWindowPixelBuffer(sGame->gfx.windowIds[0], PIXEL_FILL(1));
            AddTextPrinterParameterized(sGame->gfx.windowIds[0], FONT_NORMAL, gText_SomeoneDroppedOut, 0, 6, 0xFF, NULL);
            CopyWindowToVram(sGame->gfx.windowIds[0], COPYWIN_GFX);
            sGame->gfx.state++;
            break;
        case 2:
            if (!IsDma3ManagerBusyWithBgCopy())
                PutWindowTilemap(sGame->gfx.windowIds[0]);
            
            CopyBgTilemapBufferToVram(BG_INTERFACE);
            sGame->gfx.state++;
            break;
        case 3:
            if (++sGame->gfx.timer >= 120)
                sGame->gfx.state++;
            break;
        default:
            sGame->gfx.playAgainState = PLAY_AGAIN_DROPPED;
            ClearWindowTilemap(sGame->gfx.windowIds[0]);
            RemoveWindow(sGame->gfx.windowIds[0]);
            FillBgTilemapBufferRect_Palette0(BG_INTERFACE, 0, 0, 0, 30, 20);
            CopyBgTilemapBufferToVram(BG_INTERFACE);
            sGame->gfx.finished = TRUE;
            break;
    }
}

static void StopGfxFuncs(void)
{
    DestroyTask(sGame->gfx.taskId);
    sGame->gfx.finished = TRUE;
}

static void GfxIdle(void) { }

/////////////
// SPRITES //
/////////////

static void LoadDodrioGfx(void)
{
    void *ptr = AllocZeroed(0x3000);
    struct SpritePalette normal = {sDodrioNormal_Pal, PALTAG_DODRIO_NORMAL};
    struct SpritePalette shiny = {sDodrioShiny_Pal, PALTAG_DODRIO_SHINY};

    LZDecompressWram(sDodrio_Gfx, ptr);
    
    if (ptr)
    {
        struct SpriteSheet sheet = {ptr, 0x3000, GFXTAG_DODRIO};
        LoadSpriteSheet(&sheet);
        Free(ptr);
    }
    LoadSpritePalette(&normal);
    LoadSpritePalette(&shiny);
}

static s16 GetDodrioXPos(u32 playerId, u32 numPlayers)
{
    s16 x = 0;
    
    switch (numPlayers)
    {
        case 1:
            x = 15;
            break;
        case 2:
            switch (playerId)
            {
                case 0:
                    x = 12;
                    break;
                case 1:
                    x = 18;
                    break;
            }
            break;
        case 3:
            switch (playerId)
            {
                case 0:
                    x = 15;
                    break;
                case 1:
                    x = 21;
                    break;
                case 2:
                    x = 9;
                    break;
            }
            break;
        case 4:
            switch (playerId)
            {
                case 0:
                    x = 12;
                    break;
                case 1:
                    x = 18;
                    break;
                case 2:
                    x = 24;
                    break;
                case 3:
                    x = 6;
                    break;
            }
            break;
        case 5:
            switch (playerId)
            {
                case 0:
                    x = 15;
                    break;
                case 1:
                    x = 21;
                    break;
                case 2:
                    x = 27;
                    break;
                case 3:
                    x = 3;
                    break;
                case 4:
                    x = 9;
                    break;
            }
            break;
    }
    return x * 8;
}

static void SetDodrioInvisibility(bool32 invisible, u32 id)
{
    gSprites[sGame->members[id].dodrioSpriteId].invisible = invisible;
}

static void CreateDodrioSprite(struct DodrioGame_MonInfo * monInfo, u32 playerId, u32 id, u32 numPlayers)
{
    struct SpriteTemplate template =
    {
        .tileTag = GFXTAG_DODRIO,
        .paletteTag = PALTAG_DODRIO_NORMAL + monInfo->isShiny,
        .oam = &sOamData_Dodrio,
        .anims = sAnims_Dodrio,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCB_Dodrio,
    };
    sGame->members[id].dodrioSpriteId = CreateSprite(&template, GetDodrioXPos(playerId, numPlayers), 136, 3);
}

static void SetDodrioAnim(u32 id, u32 pickState)
{
    StartSpriteAnim(&gSprites[sGame->members[id].dodrioSpriteId], pickState);
}

static void SetAllDodrioDisabled(void)
{
    u32 i, numPlayers = sGame->numPlayers;

    for (i = 0; i < numPlayers; i++)
        SetDodrioAnim(i, PICK_DISABLED);
}

static void UpdateAllDodrioAnims(void)
{
    u32 i, numPlayers = sGame->numPlayers;

    for (i = 0; i < numPlayers; i++)
        SetDodrioAnim(i, sGame->members[i].player.comm.pickState);
}

#define sState data[0]
#define sTimer data[1]

static void StartDodrioIntroAnim(void)
{
    struct Sprite *sprite = &gSprites[sGame->members[GetMultiplayerId()].dodrioSpriteId];
    sprite->sState = 2;
    sprite->sTimer = 0;
}

static void StartDodrioMissedAnim(void)
{
    struct Sprite *sprite = &gSprites[sGame->members[GetMultiplayerId()].dodrioSpriteId];
    sprite->sState = 1;
    sprite->sTimer = 0;
}

// Does the intro animation where the player's Dodrio
// cycles through extending each head twice
#define FRAMES_PER_STATE  13
#define NUM_INTRO_PICK_STATES PICK_DISABLED // Cycle through 'Normal' and each head, but exclude the Disabled state

static void DoDodrioIntroAnim(struct Sprite *sprite)
{
    u32 pickState = (++sprite->sTimer / FRAMES_PER_STATE) % NUM_INTRO_PICK_STATES;

    // Play a sound effect at the start of each head extension
    if (sprite->sTimer % FRAMES_PER_STATE == 0 && pickState != PICK_NONE)
        PlaySE(SE_M_CHARM);

    if (sprite->sTimer >= FRAMES_PER_STATE * NUM_INTRO_PICK_STATES * 2)
    {
        // End animation
        sprite->sState = 0;
        pickState = PICK_NONE;
    }
    SetDodrioAnim(GetMultiplayerId(), pickState);
}

// Do animation where Dodrio shakes horizontally after reaching for a berry and missing
static void DoDodrioMissedAnim(struct Sprite *sprite)
{
    s8 x;
    u32 state = (++sprite->sTimer / 2) % 4;

    if (sprite->sTimer >= 3)
    {
        switch (state)
        {
            case 1:
            case 2:
                x = -1;
                break;
            default:
                x = 1;
                break;
        }
        sprite->x += x;
        
        if (++sprite->sTimer >= 40)
        {
            sprite->sState = 0;
            sprite->x = GetDodrioXPos(0, sGame->numPlayers);
        }
    }
}

static void SpriteCB_Dodrio(struct Sprite *sprite)
{
    switch (sprite->sState)
    {
        case 0:
            break;
        case 1:
            DoDodrioMissedAnim(sprite);
            break;
        case 2:
            DoDodrioIntroAnim(sprite);
            break;
    }
}

#undef sState
#undef sTimer

static void DestroyDodrioSprites(u32 numPlayers)
{
    u32 i;
    
    for (i = 0; i < numPlayers; i++)
    {
        struct Sprite *sprite = &gSprites[sGame->members[i].dodrioSpriteId];
        if (sprite)
            DestroySpriteAndFreeResources(sprite);
    }
}

static void LoadBerryGfx(void)
{
    void *ptr = AllocZeroed(0x480);
    struct SpritePalette pal = {sBerries_Pal, PALTAG_BERRIES};

    LZDecompressWram(sBerries_Gfx, ptr);
    
    if (ptr)
    {
        struct SpriteSheet sheet = {ptr, 0x480, GFXTAG_BERRIES};
        LoadSpriteSheet(&sheet);
    }
    LoadSpritePalette(&pal);
    Free(ptr);
}

static void SetBerryInvisibility(u32 id, bool32 invisible)
{
    gSprites[sGame->berryColumns[id].berrySpriteId].invisible = invisible;
}

static void SetBerryIconsInvisibility(bool32 invisible)
{
    u32 i;
    
    for (i = 0; i < NUM_BERRY_TYPES; i++)
        gSprites[sGame->berryIconSpriteIds[i]].invisible = invisible;
}

static void CreateBerrySprites(void)
{
    u32 i;

    // Create berry sprites that fall during gameplay
    for (i = 0; i < NUM_BERRY_COLUMNS; i++)
    {
        sGame->berryColumns[i].berrySpriteId = CreateSprite(&sBerrySpriteTemplate, (i * 16) + (i * 8), 8, 1);
        SetBerryInvisibility(i, TRUE);
    }

    // Create berry icon sprites for results screen
    for (i = 0; i < NUM_BERRY_TYPES; i++)
    {
        sGame->berryIconSpriteIds[i] = CreateSprite(&sBerryIconSpriteTemplate, sBerryIconXCoords[i], i == BERRY_MISSED ? 57 : 60, 0);
        StartSpriteAnim(&gSprites[sGame->berryIconSpriteIds[i]], i);
    }
    SetBerryIconsInvisibility(TRUE);
}

static void SetBerryAnim(u32 id, u32 animNum)
{
    StartSpriteAnim(&gSprites[sGame->berryColumns[id].berrySpriteId], animNum);
}

static void SetBerryYPos(u32 id, u32 y)
{
    gSprites[sGame->berryColumns[id].berrySpriteId].y = y * 8;
}

static void UpdateBerrySprites(void)
{
    u32 i;

    for (i = sGame->berryColStart; i < sGame->berryColEnd; i++)
    {
        struct DodrioGame_Player * player = &sGame->members[sGame->multiplayerId].player;
        u32 column = sActiveColumnMap[sGame->numPlayers - 1][sGame->multiplayerId][i];
        
        SetBerryInvisibility(i, player->berries[column].fallDist != 0 ? FALSE : TRUE);

        if (player->berries[column].fallDist >= MAX_FALL_DIST)
        {
            // Berry was missed, set squished anim
            SetBerryAnim(i, player->berries[column].id + BERRY_MISSED);
            SetBerryYPos(i, player->berries[column].fallDist * 2 - 1);
        }
        else if (player->berries[column].id == 3)
        {
            // Berry was picked, set eaten anim
            player->berries[column].fallDist = EAT_FALL_DIST;
            SetBerryAnim(i, ANIM_EATEN);
            SetBerryYPos(i, player->berries[column].fallDist * 2 - 1);
        }
        else
        {
            // Berry is still falling
            SetBerryAnim(i, player->berries[column].id);
            SetBerryYPos(i, player->berries[column].fallDist * 2);
        }
    }
}

static void InitFirstWaveOfBerries(void)
{
    u32 i;

    for (i = sGame->berryColStart; i < sGame->berryColEnd; i++)
    {
        sGame->player.berries[i].fallDist = (i % 2 == 0) ? 1 : 0;
        sGame->player.berries[i].id = BERRY_BLUE;
    }
}

// Despite being set up to take a berry id as an argument, this
// function is only ever given BERRY_BLUE or BERRY_MISSED.
// It reads the actual berry id (if necessary) from ids
static void IncrementBerryResult(u32 berryIdArg, u32 column, u32 playerId)
{
    u32 berryId;
    
    switch (berryIdArg)
    {
        case BERRY_BLUE:
        case BERRY_GREEN:
        case BERRY_GOLD:
            berryId = sGame->members[0].player.berries[column].id;
            
            if (sGame->berryResults[playerId][berryId] < 20000)
                sGame->berryResults[playerId][berryId]++;
            break;
        case BERRY_MISSED:
            switch (sGame->numPlayers)
            {
                case 5:
                    switch (column)
                    {
                        case 0:
                            sGame->berryResults[2][BERRY_MISSED]++;
                            sGame->berryResults[3][BERRY_MISSED]++;
                            break;
                        case 1:
                            sGame->berryResults[3][BERRY_MISSED]++;
                            break;
                        case 2:
                            sGame->berryResults[3][BERRY_MISSED]++;
                            sGame->berryResults[4][BERRY_MISSED]++;
                            break;
                        case 3:
                            sGame->berryResults[4][BERRY_MISSED]++;
                            break;
                        case 4:
                            sGame->berryResults[4][BERRY_MISSED]++;
                            sGame->berryResults[0][BERRY_MISSED]++;
                            break;
                        case 5:
                            sGame->berryResults[0][BERRY_MISSED]++;
                            break;
                        case 6:
                            sGame->berryResults[0][BERRY_MISSED]++;
                            sGame->berryResults[1][BERRY_MISSED]++;
                            break;
                        case 7:
                            sGame->berryResults[1][BERRY_MISSED]++;
                            break;
                        case 8:
                            sGame->berryResults[1][BERRY_MISSED]++;
                            sGame->berryResults[2][BERRY_MISSED]++;
                            break;
                        case 9:
                            sGame->berryResults[2][BERRY_MISSED]++;
                            break;
                    }
                    break;
                case 4:
                    switch (column)
                    {
                        case 1:
                            sGame->berryResults[2][BERRY_MISSED]++;
                            sGame->berryResults[3][BERRY_MISSED]++;
                            break;
                        case 2:
                            sGame->berryResults[3][BERRY_MISSED]++;
                            break;
                        case 3:
                            sGame->berryResults[3][BERRY_MISSED]++;
                            sGame->berryResults[0][BERRY_MISSED]++;
                            break;
                        case 4:
                            sGame->berryResults[0][BERRY_MISSED]++;
                            break;
                        case 5:
                            sGame->berryResults[0][BERRY_MISSED]++;
                            sGame->berryResults[1][BERRY_MISSED]++;
                            break;
                        case 6:
                            sGame->berryResults[1][BERRY_MISSED]++;
                            break;
                        case 7:
                            sGame->berryResults[1][BERRY_MISSED]++;
                            sGame->berryResults[2][BERRY_MISSED]++;
                            break;
                        case 8:
                            sGame->berryResults[2][BERRY_MISSED]++;
                            break;
                    }
                    break;
                case 3:
                    switch (column)
                    {
                        case 2:
                            sGame->berryResults[1][BERRY_MISSED]++;
                            sGame->berryResults[2][BERRY_MISSED]++;
                            break;
                        case 3:
                            sGame->berryResults[2][BERRY_MISSED]++;
                            break;
                        case 4:
                            sGame->berryResults[2][BERRY_MISSED]++;
                            sGame->berryResults[0][BERRY_MISSED]++;
                            break;
                        case 5:
                            sGame->berryResults[0][BERRY_MISSED]++;
                            break;
                        case 6:
                            sGame->berryResults[0][BERRY_MISSED]++;
                            sGame->berryResults[1][BERRY_MISSED]++;
                            break;
                        case 7:
                            sGame->berryResults[1][BERRY_MISSED]++;
                            break;
                    }
                    break;
                case 2:
                    switch (column)
                    {
                        case 3:
                            sGame->berryResults[0][BERRY_MISSED]++;
                            sGame->berryResults[1][BERRY_MISSED]++;
                            break;
                        case 4:
                            sGame->berryResults[0][BERRY_MISSED]++;
                            break;
                        case 5:
                            sGame->berryResults[0][BERRY_MISSED]++;
                            sGame->berryResults[1][BERRY_MISSED]++;
                            break;
                        case 6:
                            sGame->berryResults[1][BERRY_MISSED]++;
                            break;
                    }
                    break;
                }
                break;
    }
}

static u32 GetBerriesPicked(u32 playerId)
{
    u32 sum = sGame->berryResults[playerId][BERRY_BLUE]
            + sGame->berryResults[playerId][BERRY_GREEN]
            + sGame->berryResults[playerId][BERRY_GOLD];
    return min(sum, MAX_BERRIES);
}

static void UpdateBerriesPickedInRow(bool32 picked)
{
    // The 'berries picked in row' stat is only
    // counted for games with all 5 players
    if (sGame->numPlayers != MAX_RFU_PLAYERS)
        return;

    if (picked)
    {
        if (++sGame->berriesPickedInRow > sGame->maxBerriesPickedInRow)
            sGame->maxBerriesPickedInRow = sGame->berriesPickedInRow;
        
        if (sGame->berriesPickedInRow > MAX_BERRIES)
            sGame->berriesPickedInRow = MAX_BERRIES;
    }
    else // missed
    {
        if (sGame->berriesPickedInRow > sGame->maxBerriesPickedInRow)
            sGame->maxBerriesPickedInRow = sGame->berriesPickedInRow;
        
        sGame->berriesPickedInRow = 0;
    }
}

static void SetMaxBerriesPickedInRow(void)
{
    u32 i;
    
    for (i = 0; i < sGame->numPlayers; i++)
        sGame->berryResults[i][BERRY_IN_ROW] = sGame->maxBerriesPickedInRow;
}

static u32 GetPlayerIdAtColumn(u32 column)
{
    return sPlayerIdAtColumn[sGame->numPlayers - 1][column];
}

static bool32 TryPickBerry(u32 playerId, u32 pickState, u32 column)
{
    s32 pick = 0;
    u32 numPlayersIdx = sGame->numPlayers - 1;

    switch (pickState)
    {
        case PICK_LEFT:
        default:
            pick = 0;
            break;
        case PICK_MIDDLE:
            pick = 1;
            break;
        case PICK_RIGHT:
            pick = 2;
            break;
    }

    // Check if berry is within range to be picked
    if (sGame->player.berries[column].fallDist == EAT_FALL_DIST - 1 || sGame->player.berries[column].fallDist == EAT_FALL_DIST)
    {
        // Check if this berry is the one the player is trying to pick
        if (column == sDodrioHeadToColumnMap[numPlayersIdx][playerId][pick])
        {
            // Check if berry has been picked/eaten by another player
            if (sGame->berryColumns[column].berryState == BERRYSTATE_PICKED || sGame->berryColumns[column].berryState == BERRYSTATE_EATEN)
            {
                // Missed berry, picked by someone else
                sGame->members[playerId].player.comm.missedBerry = TRUE;
                return FALSE;
            }
            else // Successfully picked berry
                return TRUE;
        }
    }
    else
    {
        // Check if this berry is the one the player is trying to pick
        if (column == sDodrioHeadToColumnMap[numPlayersIdx][playerId][pick])
        {
            // Missed berry, out of range
            sGame->members[playerId].inputState = INPUTSTATE_BAD_MISS;
            sGame->members[playerId].player.comm.missedBerry = TRUE;
        }
    }
    return FALSE;
}

static void TryIncrementDifficulty(u32 playerId)
{
    u32 threshold = sDifficultyThresholds[sGame->members[playerId].difficulty % NUM_DIFFICULTIES] + (sGame->members[playerId].difficulty / NUM_DIFFICULTIES) * 100;
    if (sGame->members[playerId].berriesEaten >= threshold)
        sGame->members[playerId].difficulty++;
}

// This function checks every berry and resolves if it should be eaten or not.
// It's run in a loop that handles moving each individual berry, which means
// that every time any berry moves, every single berry is checked.
static void HandlePickBerries(void)
{
    u32 i, j, k, column;
    u32 berryStart, berryEnd, numPlayers;

    // Game is already over
    if (sGame->numGraySquares >= NUM_STATUS_SQUARES)
        return;
    
    berryStart = sGame->berryColStart;
    berryEnd = sGame->berryColEnd;
    numPlayers = sGame->numPlayers;
    
    for (i = 0; i < numPlayers; i++)
    {
        u8 *pickState = &sGame->members[i].player.comm.pickState;
        
        if (*pickState != PICK_NONE && sGame->members[i].inputState == INPUTSTATE_TRY_PICK)
        {
            // Player is attempting to pick a berry
            for (j = berryStart; j < berryEnd; j++)
            {
                column = sActiveColumnMap[0][0][j];

                // Attempt has already been checked
                if (sGame->berryColumns[column].playersAttemptingPick[0] == i || sGame->berryColumns[column].playersAttemptingPick[1] == i)
                    break;

                // Check berry pick attempt
                if (TryPickBerry(i, *pickState, column))
                {
                    for (k = 0; k < ARRAY_COUNT(sGame->berryColumns[0].playersAttemptingPick); k++)
                    {
                        if (sGame->berryColumns[column].playersAttemptingPick[k] == PLAYER_NONE)
                        {
                            sGame->berryColumns[column].playersAttemptingPick[k] = i;
                            sGame->members[i].inputState = INPUTSTATE_PICKED;
                            sGame->berryColumns[column].berryState = BERRYSTATE_PICKED;
                            break;
                        }
                    }
                    break;
                }
                
                if (sGame->members[i].player.comm.missedBerry)
                    break;
            }
        }
    }

    for (j = berryStart; j < berryEnd; j++)
    {
        u32 playerIdMissed = PLAYER_NONE;
        
        column = sActiveColumnMap[0][0][j];
        
        if (sGame->berryColumns[column].berryState == BERRYSTATE_PICKED)
        {
            s32 delayRemaining;
            u32 playerIdPicked, delayStage = sGame->members[GetPlayerIdAtColumn(column)].difficulty / NUM_DIFFICULTIES;
            
            if (delayStage >= ARRAY_COUNT(sBerryFallDelays) - 1)
                delayStage = ARRAY_COUNT(sBerryFallDelays) - 1;

            delayRemaining = sBerryFallDelays[delayStage][sGame->members[0].player.berries[column].id] - sGame->berryColumns[column].fallTimer;
            if (delayRemaining < 6)
                sGame->berryColumns[column].eatTimer += delayRemaining;

            if (++sGame->berryColumns[column].eatTimer >= 6)
            {
                sGame->berryColumns[column].eatTimer = 0;
                
                // No players attempting to pick this berry
                if (sGame->berryColumns[column].playersAttemptingPick[0] == PLAYER_NONE && sGame->berryColumns[column].playersAttemptingPick[1] == PLAYER_NONE)
                    continue;
                // One player attempting to pick this berry
                else if (sGame->berryColumns[column].playersAttemptingPick[0] != PLAYER_NONE && sGame->berryColumns[column].playersAttemptingPick[1] == PLAYER_NONE)
                    playerIdPicked = sGame->berryColumns[column].playersAttemptingPick[0];
                else
                {
                    // Two players attempting to pick this berry
                    // Randomly give it to one of them
                    u32 playerId1 = sGame->berryColumns[column].playersAttemptingPick[0];
                    
                    i = sGame->berryColumns[column].playersAttemptingPick[1]; // playerId2. Have to re-use the variable to match.
                    
                    if (!(Random() & 1))
                    {
                        playerIdPicked = playerId1;
                        playerIdMissed = i;
                    }
                    else
                    {
                        playerIdPicked = i;
                        playerIdMissed = playerId1;
                    }
                }
                // Eat berry
                sGame->player.berries[column].fallDist = EAT_FALL_DIST;
                sGame->berryColumns[column].berryState = BERRYSTATE_EATEN;
                sGame->members[playerIdPicked].inputState = INPUTSTATE_ATE_BERRY;
                sGame->berryColumns[column].berryEatenBy = playerIdPicked;
                sGame->members[playerIdPicked].player.comm.ateBerry = TRUE;

                if (playerIdMissed != PLAYER_NONE)
                    sGame->members[playerIdPicked].player.comm.missedBerry = TRUE; // UB: playerIdMissed can be PLAYER_NONE here, which is out of bounds

                sGame->members[playerIdPicked].berriesEaten++;
                IncrementBerryResult(0, column, playerIdPicked);
                UpdateBerriesPickedInRow(TRUE);
                TryIncrementDifficulty(playerIdPicked);
                sGame->berryColumns[column].prevBerryId = sGame->player.berries[column].id;
                sGame->player.berries[column].id = BERRY_MISSED; // Just to clear berry id, wasn't actually missed
                sGame->berryColumns[column].playersAttemptingPick[0] = PLAYER_NONE;
                sGame->berryColumns[column].playersAttemptingPick[1] = PLAYER_NONE;
            }
        }
    }
}

// The berry types cycle through different distributions depending on the difficulty
static u32 GetNewBerryIdByDifficulty(u32 difficulty, u32 column)
{
    u32 prevBerryId = sGame->berryColumns[column].prevBerryId;
    
    switch (difficulty % NUM_DIFFICULTIES)
    {
        default: return BERRY_BLUE;
        case 0:  return BERRY_BLUE;
        case 1:  return BERRY_GREEN;
        case 2:  return BERRY_GOLD;
        case 3:
            if (prevBerryId == BERRY_BLUE)
                return BERRY_GREEN;
            else
                return BERRY_BLUE;
        case 4:
            if (prevBerryId == BERRY_BLUE)
                return BERRY_GOLD;
            else
                return BERRY_BLUE;
        case 5:
            if (prevBerryId == BERRY_GOLD)
                return BERRY_GREEN;
            else
                return BERRY_GOLD;
        case 6:
            if (prevBerryId == BERRY_BLUE)
                return BERRY_GREEN;
            else if (prevBerryId == BERRY_GREEN)
                return BERRY_GOLD;
            else
                return BERRY_BLUE;
    }
}

// Get a berry id for when a new falling berry is created.
// What type of berry it is depends on the current difficulty
// level of players who can pick berries from that column.
static u32 GetNewBerryId(u32 playerId, u32 column)
{
    u32 i, highestDifficulty;
    u32 numPlayersIdx = sGame->numPlayers - 1;
    u32 leftPlayer = sDodrioNeighborMap[numPlayersIdx][playerId][0];
    u32 middlePlayer = sDodrioNeighborMap[numPlayersIdx][playerId][1];
    u32 rightPlayer = sDodrioNeighborMap[numPlayersIdx][playerId][2];

    for (i = 0; sUnsharedColumns[numPlayersIdx][i] != 0; i++)
    {
        // If only one player can use this column, just use their difficulty
        if (column == sUnsharedColumns[numPlayersIdx][i])
            return GetNewBerryIdByDifficulty(sGame->members[middlePlayer].difficulty, column);
    }

    // This column is shared, get the highest difficulty of adjacent players
    if (sGame->members[leftPlayer].difficulty > sGame->members[middlePlayer].difficulty)
        highestDifficulty = sGame->members[leftPlayer].difficulty;
    else
        highestDifficulty = sGame->members[middlePlayer].difficulty;

    if (sGame->members[rightPlayer].difficulty > highestDifficulty)
        highestDifficulty = sGame->members[rightPlayer].difficulty;

    return GetNewBerryIdByDifficulty(highestDifficulty, column);
}

static void UpdateFallingBerries(void)
{
    u32 i, delayStage = 0;

    sGame->berriesFalling = FALSE;

    for (i = sGame->berryColStart; i < sGame->berryColEnd - 1; i++)
    {
        if (sGame->berryColumns[i].berryState == BERRYSTATE_NONE || sGame->berryColumns[i].berryState == BERRYSTATE_PICKED)
        {
            sGame->berriesFalling = TRUE;

            if (sGame->player.berries[i].fallDist >= MAX_FALL_DIST)
            {
                // Berry hit the ground
                sGame->player.berries[i].fallDist = MAX_FALL_DIST;
                sGame->berryColumns[i].berryState = BERRYSTATE_SQUISHED;
                
                if (!sGame->berryColumns[i].playingSquishSound)
                {
                    sGame->berryColumns[i].playingSquishSound = TRUE;
                    PlaySE(SE_BALLOON_RED + sGame->player.berries[i].id);
                }
                
                if (sGame->numGraySquares < NUM_STATUS_SQUARES)
                {
                    if (sGame->numGraySquares < NUM_STATUS_SQUARES)
                        sGame->numGraySquares++;
                    
                    sGame->berryColumns[i].playingSquishSound = FALSE;
                    
                    IncrementBerryResult(BERRY_MISSED, i, 0);
                    UpdateBerriesPickedInRow(FALSE);
                }
            }
            else
            {
                // Berry is still falling
                delayStage = sGame->members[GetPlayerIdAtColumn(i)].difficulty / NUM_DIFFICULTIES;
                if (delayStage >= ARRAY_COUNT(sBerryFallDelays) - 1)
                    delayStage = ARRAY_COUNT(sBerryFallDelays) - 1;

                if (++sGame->berryColumns[i].fallTimer >= sBerryFallDelays[delayStage][sGame->player.berries[i].id])
                {
                    sGame->berryColumns[i].fallTimer = 0;
                    sGame->player.berries[i].fallDist++;
                }
                HandlePickBerries();
            }
        }
        else if (sGame->berryColumns[i].berryState == BERRYSTATE_EATEN)
        {
            // Berry has been eaten, wait and create a new berry
            if (++sGame->berryColumns[i].newBerryTimer >= 20)
            {
                sGame->berryColumns[i].newBerryTimer = 0;
                sGame->berryColumns[i].fallTimer = 0;
                sGame->berryColumns[i].berryState = BERRYSTATE_NONE;
                sGame->members[sGame->berryColumns[i].berryEatenBy].player.comm.ateBerry = FALSE;
                sGame->player.berries[i].fallDist = 1;
                sGame->player.berries[i].id = GetNewBerryId(GetPlayerIdAtColumn(i), i);
            }
        }
        else if (sGame->berryColumns[i].berryState == BERRYSTATE_SQUISHED)
        {
            // Berry has already hit the ground, wait and create a new berry
            if (++sGame->berryColumns[i].newBerryTimer >= 20)
            {
                if (sGame->numGraySquares < NUM_STATUS_SQUARES)
                {
                    sGame->berryColumns[i].newBerryTimer = 0;
                    sGame->berryColumns[i].fallTimer = 0;
                    sGame->berryColumns[i].berryState = BERRYSTATE_NONE;
                    sGame->berryColumns[i].prevBerryId = sGame->player.berries[i].id;
                    sGame->player.berries[i].fallDist = 1;
                    sGame->player.berries[i].id = GetNewBerryId(GetPlayerIdAtColumn(i), i);
                }
            }
        }
    }
}

static void DestroyBerrySprites(void)
{
    u32 i;
    struct Sprite *sprite;

    for (i = 0; i < NUM_BERRY_COLUMNS; i++)
    {
        sprite = &gSprites[sGame->berryColumns[i].berrySpriteId];
        if (sprite)
            DestroySprite(sprite);
    }
    
    for (i = 0; i < NUM_BERRY_TYPES; i++)
    {
        sprite = &gSprites[sGame->berryIconSpriteIds[i]];
        if (sprite)
            DestroySprite(sprite);
    }
}

static void CreateCloudSprites(void)
{
    u32 i;
    void *ptr = AllocZeroed(0x400);
    struct SpritePalette pal = {sCloud_Pal, PALTAG_CLOUD};

    LZDecompressWram(sCloud_Gfx, ptr);
    
    if (ptr)
    {
        struct SpriteSheet sheet = {ptr, 0x400, GFXTAG_CLOUD};

        LoadSpriteSheet(&sheet);
        LoadSpritePalette(&pal);
        
        for (i = 0; i < NUM_CLOUDS; i++)
            sGame->cloudSpriteIds[i] = CreateSprite(&sCloudSpriteTemplate, sCloudStartCoords[i].x, sCloudStartCoords[i].y, 4);
    }
    Free(ptr);
}

static void SetCloudInvisibility(bool32 invisible)
{
    u32 i;
    
    for (i = 0; i < NUM_CLOUDS; i++)
        gSprites[sGame->cloudSpriteIds[i]].invisible = invisible;
}

#define sDelay  data[0]
#define sFrozen data[1]

static void ResetCloudPos(void)
{
    u32 i;
    
    for (i = 0; i < NUM_CLOUDS; i++)
    {
        struct Sprite *sprite = &gSprites[sGame->cloudSpriteIds[i]];
        sprite->sFrozen = TRUE;
        sprite->x = sCloudStartCoords[i].x;
        sprite->y = sCloudStartCoords[i].y;
    }
}

static void StartCloudMovement(void)
{
    u32 i;
    
    for (i = 0; i < NUM_CLOUDS; i++)
        gSprites[sGame->cloudSpriteIds[i]].sFrozen = FALSE;
}

static void SpriteCB_Cloud(struct Sprite *sprite)
{
    u32 i;
    static const u8 moveDelays[] = {30, 20};

    if (!sprite->sFrozen)
    {
        for (i = 0; i < NUM_CLOUDS; i++)
        {
            if (++gSprites[sGame->cloudSpriteIds[i]].sDelay > moveDelays[i])
            {
                gSprites[sGame->cloudSpriteIds[i]].sDelay = 0;
                sprite->x--;
            }
        }
    }
}

#undef sDelay
#undef sFrozen

static void DestroyCloudSprites(void)
{
    u32 i;
    
    for (i = 0; i < NUM_CLOUDS; i++)
    {
        struct Sprite *sprite = &gSprites[sGame->cloudSpriteIds[i]];
        if (sprite)
            DestroySprite(sprite);
    }
}

static void CreateStatusBarSprites(void)
{
    u32 i;
    void *ptr = AllocZeroed(0x180);
    struct SpritePalette pal = {sStatus_Pal, PALTAG_STATUS};

    LZDecompressWram(sStatus_Gfx, ptr);
    // This check should be one line up.
    if (ptr)
    {
        struct SpriteSheet sheet = {ptr, 0x180, GFXTAG_STATUS};

        LoadSpriteSheet(&sheet);
        LoadSpritePalette(&pal);
        
        for (i = 0; i < NUM_STATUS_SQUARES; i++)
            sGame->statusBar.spriteIds[i] = CreateSprite(&sStatusBarSpriteTemplate, (i * 16) + 48, -8 - (i * 8), 0);
    }
    Free(ptr);
}

static void InitStatusBarPos(void)
{
    u32 i;
    
    for (i = 0; i < NUM_STATUS_SQUARES; i++)
    {
        struct Sprite *sprite = &gSprites[sGame->statusBar.spriteIds[i]];
        sprite->x = (i * 16) + 48;
        sprite->y = -8 - (i * 8);
        sGame->statusBar.entered[i] = FALSE;
    }
}

// Progress an animation where each square of the
// status bar drops down into view, bounces up,
// then settles into position.
// Returns TRUE if the animation is complete
static bool32 DoStatusBarIntro(void)
{
    u32 i;
    bool32 animActive = FALSE;
    
    for (i = 0; i < NUM_STATUS_SQUARES; i++)
    {
        struct Sprite *sprite = &gSprites[sGame->statusBar.spriteIds[i]];
        
        sGame->statusBar.yChange[i] = 2;
        
        if (sGame->statusBar.entered[i] && sprite->y == 8)
            continue;

        animActive = TRUE;
        
        if (sprite->y == 8)
        {
            if (sGame->statusBar.entered[i])
                continue;

            // Square has entered screen, play click
            // sound and reverse direction
            sGame->statusBar.entered[i] = TRUE;
            sGame->statusBar.yChange[i] = -16;
            PlaySE(SE_CLICK);
        }
        sprite->y += sGame->statusBar.yChange[i];
    }
    return !animActive;
}

// The status bar at the top changes color depending on the game performance.
// The squares start out yellow. For every berry missed, a square is colored gray.
// If there are 4 or fewer yellow squares left they also flash red
static void UpdateStatusBarAnim(u32 numEmpty)
{
    u32 i;

    if (numEmpty > NUM_STATUS_SQUARES)
    {
        // All squares gray
        for (i = 0; i < NUM_STATUS_SQUARES; i++)
            StartSpriteAnim(&gSprites[sGame->statusBar.spriteIds[i]], STATUS_GRAY);
    }
    else
    {
        // At least 1 square is yellow
        for (i = 0; i < NUM_STATUS_SQUARES - numEmpty; i++)
        {
            if (numEmpty > 6)
            {
                // Flash the yellow squares red
                // The flash cycles faster the fewer yellow squares remain
                sGame->statusBar.flashTimer += numEmpty - 6;
                
                if (sGame->statusBar.flashTimer > 30)
                    sGame->statusBar.flashTimer = 0;
                else if (sGame->statusBar.flashTimer > 10)
                    StartSpriteAnim(&gSprites[sGame->statusBar.spriteIds[i]], STATUS_RED);
                else
                    StartSpriteAnim(&gSprites[sGame->statusBar.spriteIds[i]], STATUS_YELLOW);
            }
            else
                StartSpriteAnim(&gSprites[sGame->statusBar.spriteIds[i]], STATUS_YELLOW);
        }

        // Set remaining squares gray
        for (; i < NUM_STATUS_SQUARES; i++)
            StartSpriteAnim(&gSprites[sGame->statusBar.spriteIds[i]], STATUS_GRAY);
    }
}

static void SetStatusBarInvisibility(bool32 invisible)
{
    u32 i;
    
    for (i = 0; i < NUM_STATUS_SQUARES; i++)
        gSprites[sGame->statusBar.spriteIds[i]].invisible = invisible;
}

static void DestroyStatusBar(void)
{
    u32 i;
    
    for (i = 0; i < NUM_STATUS_SQUARES; i++)
    {
        struct Sprite *sprite = &gSprites[sGame->statusBar.spriteIds[i]];
        if (sprite)
            DestroySpriteAndFreeResources(sprite);
    }
}

///////////////////////////////
// PRIZE AND SCORE FUNCTIONS //
///////////////////////////////

static void PrintRecordsText(u32 windowId)
{
    s32 i, x, numWidth;
    s32 recordNums[NUM_RECORD_TYPES];
    u8 strbuf[20];
    
    recordNums[0] = gSaveBlock2Ptr->berryPick.berriesPicked;
    recordNums[1] = gSaveBlock2Ptr->berryPick.bestScore;
    recordNums[2] = gSaveBlock2Ptr->berryPick.berriesPickedInRow;

    TextWindow_SetStdFrame0_WithPal(windowId, 0x21D, 0xD0);
    DrawTextBorderOuter(windowId, 0x21D, 13);
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    AddTextPrinterParameterized(windowId, FONT_NORMAL, COMPOUND_STRING("Dodrio Berry-Picking Records"), 1, 1, 0xFF, NULL);
    
    for (i = 0; i < NUM_RECORD_TYPES; i++)
    {
        ConvertIntToDecimalStringN(strbuf, recordNums[i], STR_CONV_MODE_LEFT_ALIGN, sRecordsData[i].maxDigits);
        numWidth = GetStringWidth(FONT_NORMAL, strbuf, -1);
        AddTextPrinterParameterized(windowId, FONT_NORMAL, sRecordsData[i].text, 1, sRecordsData[i].yCoords[0], 0xFF, NULL);
        x = DISPLAY_WIDTH - 16 - numWidth;
        AddTextPrinterParameterized(windowId, FONT_NORMAL, strbuf, x, sRecordsData[i].yCoords[1], 0xFF, NULL);
    }
    PutWindowTilemap(windowId);
}

#define tState    data[0]
#define tWindowId data[1]

static void Task_ShowDodrioBerryPickingRecords(u32 taskId)
{
    s16 *data = gTasks[taskId].data;

    switch (tState)
    {
        case 0:
            tWindowId = AddWindow(&sWindowTemplates_Records);
            PrintRecordsText(tWindowId);
            CopyWindowToVram(tWindowId, COPYWIN_BOTH);
            tState++;
            break;
        case 1:
            if (!IsDma3ManagerBusyWithBgCopy())
                tState++;
            break;
        case 2:
            if (JOY_NEW(A_BUTTON | B_BUTTON))
            {
                rbox_fill_rectangle(tWindowId);
                CopyWindowToVram(tWindowId, COPYWIN_MAP);
                tState++;
            }
            break;
        case 3:
            if (!IsDma3ManagerBusyWithBgCopy())
            {
                RemoveWindow(tWindowId);
                DestroyTask(taskId);
                EnableBothScriptContexts();
            }
            break;
    }
}

#undef tState
#undef tWindowId

void ShowDodrioBerryPickingRecords(void)
{
    Task_ShowDodrioBerryPickingRecords(CreateTask(Task_ShowDodrioBerryPickingRecords, 0));
}

static u32 GetScore(u32 playerId)
{
    u32 i, scoreLost, score = 0;

    // Sum up points for berries picked
    for (i = 0; i < BERRY_MISSED; i++)
        score += sGame->berryResults[playerId][i] * sBerryScoreMultipliers[i];

    // Get points lost for berries missed
    scoreLost = sGame->berryResults[playerId][BERRY_MISSED] * sBerryScoreMultipliers[BERRY_MISSED];
    
    if (score <= scoreLost)
        return 0;
    else
        return score - scoreLost;
}

static u32 GetHighestScore(void)
{
    u32 i, numPlayers = sGame->numPlayers;
    u32 highestScore = GetScore(0);

    for (i = 1; i < numPlayers; i++)
    {
        u32 score = GetScore(i);
        if (score > highestScore)
            highestScore = score;
    }
    return min(highestScore, MAX_SCORE);
}

static u32 GetScoreByRanking(u32 ranking)
{
    u32 scores[MAX_RFU_PLAYERS], temp;
    bool32 unsorted = TRUE;
    u32 i, numPlayers = sGame->numPlayers;

    for (i = 0; i < numPlayers; i++)
        scores[i] = temp = GetScore(i);

    // Sort the scores in the array highest to lowest
    while (unsorted)
    {
        unsorted = FALSE;
        
        for (i = 0; i < numPlayers - 1; i++)
        {
            if (scores[i] < scores[i + 1])
            {
                SWAP(scores[i], scores[i + 1], temp);
                unsorted = TRUE;
            }
        }
    }
    return scores[ranking];
}

static void SetScoreResults(void)
{
    u32 i, ranking = 0, nextRanking = 0, playersRanked = 0;
    u32 numPlayers = sGame->numPlayers;

    if (GetHighestScore() == 0)
    {
        // No one scored any points, put everyone in last place with a score of 0.
        // Presumably this was supposed to then return, as the assignments in this
        // loop are then overwritten by the rest of the function
        for (i = 0; i < numPlayers; i++)
        {
            sGame->members[i].scoreResults.ranking = MAX_RFU_PLAYERS - 1;;
            sGame->members[i].scoreResults.score = 0;
        }
    }

    // Set scores
    for (i = 0; i < numPlayers; i++)
        sGame->members[i].scoreResults.score = min(GetScore(i), MAX_SCORE);

    // Set rankings
    do
    {
        u32 score = GetScoreByRanking(ranking);
        u32 curRanking = nextRanking;

        // Find all players with the score for this ranking.
        // Increment nextRanking but not curRanking to allow
        // for ties
        for (i = 0; i < numPlayers; i++)
        {
            if (score == sGame->members[i].scoreResults.score)
            {
                sGame->members[i].scoreResults.ranking = curRanking;
                nextRanking++;
                playersRanked++;
            }
        }
        ranking = nextRanking;
    } while (playersRanked < numPlayers);
}

static inline void GetScoreResults(struct DodrioGame_ScoreResults * dst, u32 playerId)
{
    *dst = sGame->members[playerId].scoreResults;
}

static void PrintRankedScores(void)
{
    u32 i, ranking = 0, rankedPlayers = 0;
    u32 numPlayers = sGame->numPlayers;
    u32 x, numWidth;
    u8 numString[32];
    u8 playersByRanking[MAX_RFU_PLAYERS] = {0, 1, 2, 3, 4};
    struct DodrioGame_ScoreResults temp, scoreResults[MAX_RFU_PLAYERS];

    // Get all players scores and rankings
    for (i = 0; i < numPlayers; i++)
    {
        playersByRanking[i] = i;
        GetScoreResults(&temp, i);
        scoreResults[i] = temp;
    }

    // Sort player ids by ranking
    if (GetHighestScore() != 0)
    {
        do
        {
            for (i = 0; i < numPlayers; i++)
            {
                if (scoreResults[i].ranking == ranking)
                {
                    playersByRanking[rankedPlayers] = i;
                    rankedPlayers++;
                }
            }
            ranking = rankedPlayers;
        } while (rankedPlayers < numPlayers);
    }

    // Put any player with a score of 0 at lowest ranking
    for (i = 0; i < numPlayers; i++)
    {
        if (scoreResults[i].score == 0)
            scoreResults[i].ranking = numPlayers - 1;
    }

    // Print text
    x = 216 - GetStringWidth(FONT_SMALL, sText_SpacePoints, 0);
    for (i = 0; i < numPlayers; i++)
    {
        u32 playerId = playersByRanking[i];

        AddTextPrinterParameterized(sGame->gfx.windowIds[1], FONT_SMALL, sRankingTexts[scoreResults[playerId].ranking], 8, sRankingYCoords[i], 0xFF, NULL);
        AddTextPrinterParameterized3(sGame->gfx.windowIds[1], FONT_SMALL, 28, sRankingYCoords[i], sTextColorTable[GetPlayerTextColorsId(playerId)], 0xFF, GetPlayerName(playerId));
        
        ConvertIntToDecimalStringN(numString, scoreResults[playerId].score, STR_CONV_MODE_RIGHT_ALIGN, 7);
        numWidth = GetStringWidth(FONT_SMALL, numString, -1);
        AddTextPrinterParameterized(sGame->gfx.windowIds[1], FONT_SMALL, numString, x - 35, sRankingYCoords[i], 0xFF, NULL);
        AddTextPrinterParameterized(sGame->gfx.windowIds[1], FONT_SMALL, sText_SpacePoints, x, sRankingYCoords[i], 0xFF, NULL);
    }
}

static void TryUpdateRecords(void)
{
    u32 berriesPicked = GetBerriesPicked(sGame->multiplayerId);
    u32 score = min(GetScore(sGame->multiplayerId), MAX_SCORE);

    if (gSaveBlock2Ptr->berryPick.bestScore < score)
        gSaveBlock2Ptr->berryPick.bestScore = score;
    
    if (gSaveBlock2Ptr->berryPick.berriesPicked < berriesPicked)
        gSaveBlock2Ptr->berryPick.berriesPicked = berriesPicked;
    
    if (gSaveBlock2Ptr->berryPick.berriesPickedInRow < sGame->maxBerriesPickedInRow)
        gSaveBlock2Ptr->berryPick.berriesPickedInRow = sGame->maxBerriesPickedInRow;
}

static u32 GetHighestBerryResult(u32 berryId)
{
    u32 i, numPlayers = sGame->numPlayers;
    u32 highestResult = sGame->berryResults[0][berryId];

    for (i = 0; i < numPlayers; i++)
    {
        u32 result = sGame->berryResults[i][berryId];
        if (result > highestResult)
            highestResult = result;
    }
    return highestResult;
}

static void SetRandomPrize(void)
{
    u32 i, prizeSet = 0, prizeIdx = 0;

    switch (sGame->numPlayers)
    {
        case 4:
            prizeSet = 1;
            break;
        case 5:
            prizeSet = 2;
            break;
    }
    prizeIdx = Random() % 10;
    
    for (i = 0; i < MAX_RFU_PLAYERS; i++)
        sGame->berryResults[i][BERRY_PRIZE] = sPrizeBerryIds[prizeSet][prizeIdx];
}

static u32 GetPrizeItemId(void)
{
    return sGame->berryResults[sGame->multiplayerId][BERRY_PRIZE] + FIRST_BERRY_INDEX;
}

static u32 TryGivePrize(u32 prizeItem)
{
    u32 multiplayerId = sGame->multiplayerId;

    if (GetScore(multiplayerId) != GetHighestScore())
        return NO_PRIZE;
    else if (!CheckBagHasSpace(prizeItem, 1))
        return PRIZE_NO_ROOM;

    AddBagItem(prizeItem, 1);
    
    if (!CheckBagHasSpace(prizeItem, 1))
        return PRIZE_FILLED_BAG;
    
    return PRIZE_RECEIVED;
}

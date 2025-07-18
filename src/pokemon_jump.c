#include "decompress.h"
#include "digit_obj_util.h"
#include "event_data.h"
#include "gflib.h"
#include "global.h"
#include "item.h"
#include "link.h"
#include "link_rfu.h"
#include "menu.h"
#include "minigame_countdown.h"
#include "new_menu_helpers.h"
#include "pokemon_jump.h"
#include "random.h"
#include "save.h"
#include "script.h"
#include "sound.h"
#include "strings.h"
#include "text_window.h"
#include "trig.h"
#include "constants/songs.h"

// Max height a mon is allowed in the pokemon jump
#define MAX_POKEMON_JUMP_HEIGHT 10 // 1.0 m

#define MAX_JUMP_SCORE 99990
#define MAX_JUMPS 9999

#define JUMP_PEAK (-30)

enum
{
    JUMP_TYPE_SLOW,
    JUMP_TYPE_NORMAL,
    JUMP_TYPE_FAST
};

enum
{
    FUNC_GAME_INTRO,
    FUNC_WAIT_ROUND,
    FUNC_GAME_ROUND,
    FUNC_GAME_OVER,
    FUNC_ASK_PLAY_AGAIN,
    FUNC_RESET_GAME,
    FUNC_EXIT,
    FUNC_GIVE_PRIZE,
    FUNC_SAVE,
    FUNC_NONE
};

enum
{
    GFXFUNC_LOAD,
    GFXFUNC_SHOW_NAMES,
    GFXFUNC_SHOW_NAMES_HIGHLIGHT,
    GFXFUNC_ERASE_NAMES,
    GFXFUNC_MSG_PLAY_AGAIN,
    GFXFUNC_MSG_SAVING,
    GFXFUNC_ERASE_MSG,
    GFXFUNC_MSG_PLAYER_DROPPED,
    GFXFUNC_MSG_COMM_STANDBY,
    GFXFUNC_COUNTDOWN
};

// The venusaurs in the background are actually an empty 256x512 bg with 3 pairs of venusaurs on it.
// The below array is used to get values for where to set the bg Y to show the corresponding
// venusaur pair in their state of swinging the vine rope
// NEUTRAL/DOWN/UP refers to which direction the Venusaur is facing as it swings the vine
enum
{
    VENUSAUR_NEUTRAL,
    VENUSAUR_DOWN,
    VENUSAUR_UP
};

enum
{
    VINE_HIGHEST,
    VINE_DOWNSWING_HIGHER,
    VINE_DOWNSWING_HIGH,
    VINE_DOWNSWING_LOW,
    VINE_DOWNSWING_LOWER,
    VINE_LOWEST,
    VINE_UPSWING_LOWER,
    VINE_UPSWING_LOW,
    VINE_UPSWING_HIGH,
    VINE_UPSWING_HIGHER,
    NUM_VINESTATES
};

// Used to compare limits for vineStateTimer
// The upper 8 bits of vineStateTimer are the vine state,
// the lower 8 bits are a timer to the next state.
// When the timer is incremented above 255, it increments
// the vine state and the timer is reset.
#define VINE_STATE_TIMER(vineState) (((vineState) << 8) | 0xFF)

#define VINE_SPRITES_PER_SIDE 4 // Vine rope is divided into 8 sprites, 4 per side copied and flipped horizontally

#define GFXTAG_VINE1     5
#define GFXTAG_VINE2     6
#define GFXTAG_VINE3     7
#define GFXTAG_VINE4     8
#define GFXTAG_COUNTDOWN 9
#define GFXTAG_STAR      10

#define PALTAG_1         5
#define PALTAG_2         6
#define PALTAG_COUNTDOWN 7

#define TAG_DIGITS 800

#define TAG_MON1 0
#define TAG_MON2 1 // MON2-5 used implicitly by adding multiplayer id to tag
#define TAG_MON3 2
#define TAG_MON4 3
#define TAG_MON5 4

enum
{
    MONSTATE_NORMAL, // Pokémon is either on the ground or in the middle of a jump
    MONSTATE_JUMP,   // Pokémon has begun a jump
    MONSTATE_HIT     // Pokémon got hit by the vine
};

enum
{
    JUMPSTATE_NONE,
    JUMPSTATE_SUCCESS, // Cleared vine
    JUMPSTATE_FAILURE  // Hit vine
};

enum
{
    BG_INTERFACE,
    BG_BONUSES,
    BG_VENUSAUR,
    BG_SCENERY
};

// IDs for persistent windows
enum
{
    WIN_POINTS,
    WIN_TIMES,
    NUM_WINDOWS
};

enum
{
    PACKET_MON_INFO = 1,
    PACKET_UNUSED,
    PACKET_LEADER_STATE,
    PACKET_MEMBER_STATE
};

struct PokemonJump_CommData
{
    u8 funcId;
    u8 receivedBonusFlags;
    u16 data; // Multi-use
    u32 jumpScore;
    u16 jumpsInRow;
};

struct PokemonJumpGfx
{
    u16 mainState;
    u8 taskId;
    u8 resetVineTimer:4;
    u8 vineState:4;
    u8 vinePalNumDownswing:4;
    u8 vinePalNumUpswing:4;
    u8 resetVineState:2;
    u8 bonusTimer:6;
    u8 msgWindowState:2;
    u8 funcFinished:1;
    u8 unused:5;
    u8 msgWindowId;
    u16 fanfare;
    u8 nameWindowIds[MAX_RFU_PLAYERS];
    u16 tilemapBuffer[BG_SCREEN_SIZE];
    struct Sprite *monSprites[MAX_RFU_PLAYERS];
    struct Sprite *starSprites[MAX_RFU_PLAYERS];
    struct Sprite *vineSprites[VINE_SPRITES_PER_SIDE * 2];
    u8 monSpriteSubpriorities[MAX_RFU_PLAYERS];
};

struct PokemonJump_MonInfo
{
    u16 species;
    u32 otId;
    u32 personality;
};

struct PokemonJump_Player
{
    int jumpOffset;
    int jumpOffsetIdx;
    u16 monJumpType;
    u16 jumpTimeStart;
    u16 monState;
    u16 prevMonState;
    int jumpState;
    bool8 funcFinished;
    u8 name[11];
};

struct PokemonJump
{
    MainCallback exitCallback;
    u8 taskId;
    u8 multiplayerId;
    u8 numPlayers;
    u8 vineState:4;
    u8 prevVineState:4;
    u16 vineTimer;
    u16 vineStateTimer;
    int vineSpeed;
    u8 vineBaseSpeedIdx;
    u8 vineSpeedStage;
    u16 vineSpeedDelay;
    u32 vineSpeedAccel;
    u32 nextVineSpeed;
    u16 mainState;
    u16 helperState;
    u8 nextFuncId;
    u8 atMaxSpeedStage:1;
    u8 initScoreUpdate:1;
    u8 updateScore:1;
    u8 isLeader:1;
    u8 allPlayersReady:1;
    u8 allowVineUpdates:1;
    u8 funcActive:1;
    u8 showBonus:1;
    u16 giveBonus:1;
    u16 startDelayTimer:5;
    u16 ignoreJumpInput:1;
    u16 gameOver:1;
    u16 numPlayersAtPeak:3;
    u16 playAgainState:2;
    u16 playAgainComm:2;
    u16 unused:1;
    u16 excellentsInRow;
    u16 excellentsInRowRecord;
    int linkTimer;
    u32 linkTimerLimit;
    u32 rngSeed;
    u16 prizeItemId;
    u16 prizeItemQuantity;
    u16 timer;
    struct
    {
        u8 funcId;
        u8 atJumpPeak:1;
        u8 atJumpPeak2:1;
        u8 atJumpPeak3:1;
        u8 playAgainState:2;
        u8 unused:3;
        u16 jumpTimeStart;
        struct PokemonJump_MonInfo monInfo;
    } members[MAX_RFU_PLAYERS];
    struct PokemonJump_Player players[MAX_RFU_PLAYERS];
    struct PokemonJump_Player *player;
    struct PokemonJumpGfx jumpGfx;
    struct PokemonJump_CommData comm;
};

struct MonInfoPacket
{
    u8 id;
    u16 species;
    u32 personality;
    u32 otId;
};

struct LeaderStatePacket
{
    u8 id;
    u8 funcId;
    u8 monState;
    u8 receivedBonusFlags:5; // 1 bit for each player (MAX_RFU_PLAYERS)
    u8 jumpState:3;
    u16 jumpTimeStart;
    u16 vineTimer;
    u32 jumpsInRow:15;
    u32 jumpScore:17;
};

struct MemberStatePacket
{
    u8 id;
    u8 monState;
    u8 jumpState;
    bool8 funcFinished;
    u16 jumpTimeStart;
    u8 funcId;
    u16 playAgainState;
};

// Used by SetLinkTimeInterval to get a bit mask for capping
// a timer that controls how frequently link data is sent.
#define LINK_INTERVAL_NONE   0
#define LINK_INTERVAL_SHORT  3 // 3 frame interval
#define LINK_INTERVAL_MEDIUM 4 // 7 frame interval
#define LINK_INTERVAL_LONG   5 // 15 frame interval

#define LINK_TIMER_STOPPED 0x1111

static void Task_StartPokemonJump(u32 taskId);
static void Task_CommunicateMonInfo(u32 taskId);
static void Task_PokemonJump_Leader(u32 taskId);
static void Task_PokemonJump_Member(u32 taskId);
static void SetLinkTimeInterval(u32 intervalId);
static void StartPokeJumpGfx(struct PokemonJumpGfx *jumpGfx);
static void HandleMonState(void);
static bool32 IsPokeJumpGfxFuncFinished(void);
static bool32 IsMonIntroBounceActive(void);
static void DoStarAnim(struct PokemonJumpGfx *jumpGfx, u32 multiplayerId);
static void CreateVineSprites(struct PokemonJumpGfx *jumpGfx);
static void InitVineState(void);
static void UpdateVineAnim(struct PokemonJumpGfx *jumpGfx, u32 vineState);
static bool32 UpdateVineHitStates(void);
static void SetUpResetVineGfx(void);
static void ResetVineState(void);
static bool32 HandleSwingRound(void);
static void ResetVineAfterHit(void);
static void TryUpdateVineSwing(void);
static void UpdateVineSwing(u32 vineState);
static bool32 AllPlayersJumpedOrHit(void);
static bool32 DoVineHitEffect(void);
static bool32 DidAllPlayersClearVine(void);
static u32 GetPlayersAtJumpPeak(void);
static void SpriteCB_Star(struct Sprite *sprite);
static void SpriteCB_MonIntroBounce(struct Sprite *sprite);
static void SpriteCB_MonHitShake(struct Sprite *sprite);
static void SpriteCB_MonHitFlash(struct Sprite *sprite);
static bool32 GameIntro_Leader(void);
static bool32 GameIntro_Member(void);
static bool32 WaitRound_Leader(void);
static bool32 WaitRound_Member(void);
static bool32 GameRound_Leader(void);
static bool32 GameRound_Member(void);
static bool32 GameOver_Leader(void);
static bool32 GameOver_Member(void);
static bool32 AskPlayAgain_Leader(void);
static bool32 AskPlayAgain_Member(void);
static bool32 ResetGame_Leader(void);
static bool32 ResetGame_Member(void);
static bool32 ExitGame(void);
static bool32 GivePrize_Leader(void);
static bool32 GivePrize_Member(void);
static bool32 SavePokeJump(void);
static void SetUpPokeJumpGfxFuncById(u32 id);
static void LoadPokeJumpGfx(void);
static void PrintPlayerNamesNoHighlight(void);
static void PrintPlayerNamesWithHighlight(void);
static void ErasePlayerNames(void);
static void Msg_WantToPlayAgain(void);
static void Msg_SavingDontTurnOff(void);
static void EraseMessage(void);
static void Msg_SomeoneDroppedOut(void);
static void DoPokeJumpCountdown(void);
static void Msg_CommunicationStandby(void);
static void CreateJumpMonSprites(void);
static u32 AddMessageWindow(u32 left, u32 top, u32 width, u32 height);
static void ResetMonSpriteSubpriorities(void);
static void StartMonIntroBounce(u32 multiplayerId);
static void StartMonHitShake(u32 multiplayerId);
static bool32 IsMonHitShakeActive(u32 multiplayerId);
static void StartMonHitFlash(u32 multiplayerId);
static void StopMonHitFlash(void);

EWRAM_DATA static struct PokemonJump *sPokemonJump = NULL;
EWRAM_DATA static struct PokemonJumpGfx *sPokemonJumpGfx = NULL;

static const u16 sInterface_Pal[] = INCBIN_U16("graphics/link_games/pkmnjump_pal3.gbapal");

static const u16 sBg_Pal[] = INCBIN_U16("graphics/link_games/pkmnjump_bg.gbapal");
static const u32 sBg_Gfx[] = INCBIN_U32("graphics/link_games/pkmnjump_bg.4bpp.lz");
static const u32 sBg_Tilemap[] = INCBIN_U32("graphics/link_games/pkmnjump_bg.bin.lz");

static const u16 sVenusaur_Pal[] = INCBIN_U16("graphics/link_games/pkmnjump_venusaur.gbapal");
static const u32 sVenusaur_Gfx[] = INCBIN_U32("graphics/link_games/pkmnjump_venusaur.4bpp.lz");
static const u32 sVenusaur_Tilemap[] = INCBIN_U32("graphics/link_games/pkmnjump_venusaur.bin.lz");

static const u16 sBonuses_Pal[] = INCBIN_U16("graphics/link_games/pkmnjump_results.gbapal");
static const u32 sBonuses_Gfx[] = INCBIN_U32("graphics/link_games/pkmnjump_results.4bpp.lz");
static const u32 sBonuses_Tilemap[] = INCBIN_U32("graphics/link_games/pkmnjump_results.bin.lz");

static const u16 sPokeJumpPal1[] = INCBIN_U16("graphics/link_games/pkmnjump_pal1.gbapal");
static const u16 sPokeJumpPal2[] = INCBIN_U16("graphics/link_games/pkmnjump_pal2.gbapal");

static const u32 sVine1_Gfx[] = INCBIN_U32("graphics/link_games/pkmnjump_rope1.4bpp.lz");
static const u32 sVine2_Gfx[] = INCBIN_U32("graphics/link_games/pkmnjump_rope2.4bpp.lz");
static const u32 sVine3_Gfx[] = INCBIN_U32("graphics/link_games/pkmnjump_rope3.4bpp.lz");
static const u32 sVine4_Gfx[] = INCBIN_U32("graphics/link_games/pkmnjump_rope4.4bpp.lz");

static const u32 sStar_Gfx[] = INCBIN_U32("graphics/link_games/pkmnjump_star.4bpp.lz");

static const struct CompressedSpriteSheet sCompressedSpriteSheets[] =
{
    {sVine1_Gfx, 0x600, GFXTAG_VINE1},
    {sVine2_Gfx, 0xC00, GFXTAG_VINE2},
    {sVine3_Gfx, 0x600, GFXTAG_VINE3},
    {sVine4_Gfx, 0x600, GFXTAG_VINE4},
    {sStar_Gfx,  0x200, GFXTAG_STAR},
};

static const struct SpritePalette sSpritePalettes[] =
{
    {sPokeJumpPal1, PALTAG_1},
    {sPokeJumpPal2, PALTAG_2},
};

static const struct CompressedSpriteSheet sSpriteSheet_Digits = {gUnknown_8479688, 0, TAG_DIGITS};
static const struct SpritePalette sSpritePalette_Digits = {gUnknown_8479668, TAG_DIGITS};

static const struct BgTemplate sBgTemplates[] =
{
    {
        .bg = BG_INTERFACE,
        .charBaseIndex = 0,
        .mapBaseIndex = 27,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = BG_VENUSAUR,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 2,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0
    },
    {
        .bg = BG_BONUSES,
        .charBaseIndex = 2,
        .mapBaseIndex = 12,
        .screenSize = 3,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    },
    {
        .bg = BG_SCENERY,
        .charBaseIndex = 3,
        .mapBaseIndex = 29,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0
    },
};

static const struct WindowTemplate sWindowTemplates[] =
{
    [WIN_POINTS] =
    {
        .bg = BG_INTERFACE,
        .tilemapLeft = 19,
        .tilemapTop = 0,
        .width = 6,
        .height = 2,
        .paletteNum = 2,
        .baseBlock = 0x13,
    },
    [WIN_TIMES] =
    {
        .bg = BG_INTERFACE,
        .tilemapLeft = 8,
        .tilemapTop = 0,
        .width = 6,
        .height = 2,
        .paletteNum = 2,
        .baseBlock = 0x1F,
    },
    DUMMY_WIN_TEMPLATE
};

static const struct WindowTemplate sWindowTemplate_Records =
{
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = 28,
    .height = 9,
    .paletteNum = 15,
    .baseBlock = 0x1,
};

static const u8 sVenusaurStates[] =
{
    [VINE_HIGHEST]          = VENUSAUR_UP,
    [VINE_DOWNSWING_HIGHER] = VENUSAUR_UP,
    [VINE_DOWNSWING_HIGH]   = VENUSAUR_NEUTRAL,
    [VINE_DOWNSWING_LOW]    = VENUSAUR_NEUTRAL,
    [VINE_DOWNSWING_LOWER]  = VENUSAUR_DOWN,
    [VINE_LOWEST]           = VENUSAUR_DOWN,
    [VINE_UPSWING_LOWER]    = VENUSAUR_DOWN,
    [VINE_UPSWING_LOW]      = VENUSAUR_NEUTRAL,
    [VINE_UPSWING_HIGH]     = VENUSAUR_NEUTRAL,
    [VINE_UPSWING_HIGHER]   = VENUSAUR_UP,
};

struct
{
    int id;
    void (*func)(void);
} static const sPokeJumpGfxFuncs[] =
{
    {GFXFUNC_LOAD,                 LoadPokeJumpGfx},
    {GFXFUNC_SHOW_NAMES,           PrintPlayerNamesNoHighlight},
    {GFXFUNC_SHOW_NAMES_HIGHLIGHT, PrintPlayerNamesWithHighlight},
    {GFXFUNC_ERASE_NAMES,          ErasePlayerNames},
    {GFXFUNC_MSG_PLAY_AGAIN,       Msg_WantToPlayAgain},
    {GFXFUNC_MSG_SAVING,           Msg_SavingDontTurnOff},
    {GFXFUNC_ERASE_MSG,            EraseMessage},
    {GFXFUNC_MSG_PLAYER_DROPPED,   Msg_SomeoneDroppedOut},
    {GFXFUNC_COUNTDOWN,            DoPokeJumpCountdown},
    {GFXFUNC_MSG_COMM_STANDBY,     Msg_CommunicationStandby},
};

static bool32 (* const sPokeJumpLeaderFuncs[])(void) =
{
    [FUNC_GAME_INTRO]     = GameIntro_Leader,
    [FUNC_WAIT_ROUND]     = WaitRound_Leader,
    [FUNC_GAME_ROUND]     = GameRound_Leader,
    [FUNC_GAME_OVER]      = GameOver_Leader,
    [FUNC_ASK_PLAY_AGAIN] = AskPlayAgain_Leader,
    [FUNC_RESET_GAME]     = ResetGame_Leader,
    [FUNC_EXIT]           = ExitGame,
    [FUNC_GIVE_PRIZE]     = GivePrize_Leader,
    [FUNC_SAVE]           = SavePokeJump,
};

static bool32 (* const sPokeJumpMemberFuncs[])(void) =
{
    [FUNC_GAME_INTRO]     = GameIntro_Member,
    [FUNC_WAIT_ROUND]     = WaitRound_Member,
    [FUNC_GAME_ROUND]     = GameRound_Member,
    [FUNC_GAME_OVER]      = GameOver_Member,
    [FUNC_ASK_PLAY_AGAIN] = AskPlayAgain_Member,
    [FUNC_RESET_GAME]     = ResetGame_Member,
    [FUNC_EXIT]           = ExitGame,
    [FUNC_GIVE_PRIZE]     = GivePrize_Member,
    [FUNC_SAVE]           = SavePokeJump,
};

// Bonuses given depend on the number of
// players that jumped at the same time
static const u16 sScoreBonuses[MAX_RFU_PLAYERS + 1] = {0, 0, 50, 100, 200, 500};

static const u16 sSoundEffects[MAX_RFU_PLAYERS - 1] = {SE_RS_SHOP, SE_SHINY, SE_M_MORNING_SUN, SE_POKE_JUMP_SUCCESS};

static const u8 *const sRecordsTexts[] =
{
    COMPOUND_STRING("Jumps in a row:"),
    COMPOUND_STRING("Best score:"),
    COMPOUND_STRING("Excellents in a row:")
};

static const struct OamData sOamData_Vine16x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(16x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
    .affineParam = 0
};

static const struct OamData sOamData_Vine32x32 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
    .affineParam = 0
};

static const struct OamData sOamData_Vine32x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
    .affineParam = 0
};

static const union AnimCmd sAnims_Vine_Highest[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END
};

static const union AnimCmd sAnims_Vine_Higher[] =
{
    ANIMCMD_FRAME(8, 1),
    ANIMCMD_END
};

static const union AnimCmd sAnims_Vine_High[] =
{
    ANIMCMD_FRAME(16, 1),
    ANIMCMD_END
};

static const union AnimCmd sAnims_Vine_Low[] =
{
    ANIMCMD_FRAME(24, 1),
    ANIMCMD_END
};

static const union AnimCmd sAnims_Vine_Lower[] =
{
    ANIMCMD_FRAME(32, 1),
    ANIMCMD_END
};

static const union AnimCmd sAnims_Vine_Lowest[] =
{
    ANIMCMD_FRAME(40, 1),
    ANIMCMD_END
};

static const union AnimCmd sAnims_VineTall_Highest[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END
};

static const union AnimCmd sAnims_VineTall_Higher[] =
{
    ANIMCMD_FRAME(16, 1),
    ANIMCMD_END
};

static const union AnimCmd sAnims_VineTall_High[] =
{
    ANIMCMD_FRAME(32, 1),
    ANIMCMD_END
};

static const union AnimCmd sAnims_VineTall_Low[] =
{
    ANIMCMD_FRAME(48, 1),
    ANIMCMD_END
};

static const union AnimCmd sAnims_VineTall_Lower[] =
{
    ANIMCMD_FRAME(64, 1),
    ANIMCMD_END
};

static const union AnimCmd sAnims_VineTall_Lowest[] =
{
    ANIMCMD_FRAME(80, 1),
    ANIMCMD_END
};

static const union AnimCmd *const sAnims_Vine[] =
{
    sAnims_Vine_Highest,
    sAnims_Vine_Higher,
    sAnims_Vine_High,
    sAnims_Vine_Low,
    sAnims_Vine_Lower,
    sAnims_Vine_Lowest
};

// Vine 2 needs its own set of anims because the graphic is twice as large
static const union AnimCmd *const sAnims_VineTall[] =
{
    sAnims_VineTall_Highest,
    sAnims_VineTall_Higher,
    sAnims_VineTall_High,
    sAnims_VineTall_Low,
    sAnims_VineTall_Lower,
    sAnims_VineTall_Lowest
};

static const struct SpriteTemplate sSpriteTemplate_Vine1 =
{
    .tileTag = GFXTAG_VINE1,
    .paletteTag = PALTAG_1,
    .oam = &sOamData_Vine16x32,
    .anims = sAnims_Vine,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sSpriteTemplate_Vine2 =
{
    .tileTag = GFXTAG_VINE2,
    .paletteTag = PALTAG_1,
    .oam = &sOamData_Vine32x32,
    .anims = sAnims_VineTall,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sSpriteTemplate_Vine3 =
{
    .tileTag = GFXTAG_VINE3,
    .paletteTag = PALTAG_1,
    .oam = &sOamData_Vine32x16,
    .anims = sAnims_Vine,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sSpriteTemplate_Vine4 =
{
    .tileTag = GFXTAG_VINE4,
    .paletteTag = PALTAG_1,
    .oam = &sOamData_Vine32x16,
    .anims = sAnims_Vine,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate *const sSpriteTemplates_Vine[VINE_SPRITES_PER_SIDE] =
{
    &sSpriteTemplate_Vine1,
    &sSpriteTemplate_Vine2,
    &sSpriteTemplate_Vine3,
    &sSpriteTemplate_Vine4,
};

static const u8 sVineXCoords[VINE_SPRITES_PER_SIDE * 2] = {16, 40, 72, 104, 136, 168, 200, 224};

static const u8 sVineYCoords[VINE_SPRITES_PER_SIDE][NUM_VINESTATES] =
{
    {96, 96, 96, 114, 120, 120, 120, 114, 96, 96},
    {70, 80, 96, 114, 120, 128, 120, 114, 96, 80},
    {50, 72, 96, 114, 128, 136, 128, 114, 96, 72},
    {42, 72, 96, 114, 128, 136, 128, 114, 96, 72},
};

static const u8 sVineBaseSpeeds[] = {26, 31, 36, 41, 46, 51, 56, 61};
static const u8 sVineSpeedDelays[] = {0, 1, 1, 2};

static const u8 sMonXCoords_2Players[] = {88, 152};
static const u8 sMonXCoords_3Players[] = {88, 120, 152};
static const u8 sMonXCoords_4Players[] = {56, 88, 152, 184};
static const u8 sMonXCoords_5Players[] = {56, 88, 120, 152, 184};

static const u8 *const sMonXCoords[MAX_RFU_PLAYERS - 1] =
{
    sMonXCoords_2Players,
    sMonXCoords_3Players,
    sMonXCoords_4Players,
    sMonXCoords_5Players,
};

static const u8 sPlayerNameWindowCoords_2Players[] =
{
     6, 8,
    16, 8
};

static const u8 sPlayerNameWindowCoords_3Players[] =
{
     6, 8,
    11, 6,
    16, 8
};

static const u8 sPlayerNameWindowCoords_4Players[] =
{
     2, 6,
     6, 8,
    16, 8,
    20, 6
};

static const u8 sPlayerNameWindowCoords_5Players[] =
{
     2, 6,
     6, 8,
    11, 6,
    16, 8,
    20, 6
};

static const u8 *const sPlayerNameWindowCoords[MAX_RFU_PLAYERS - 1] =
{
    sPlayerNameWindowCoords_2Players,
    sPlayerNameWindowCoords_3Players,
    sPlayerNameWindowCoords_4Players,
    sPlayerNameWindowCoords_5Players,
};

static const s8 sJumpOffsets[][48] =
{
    [JUMP_TYPE_NORMAL] = { -3,  -6,  -8, -10, -13, -15, -17, -19,
                          -21, -23, -25, -27, -28, -29,
                          JUMP_PEAK, JUMP_PEAK, JUMP_PEAK,
                          -28, -27, -26, -25, -23, -22, -20, -18,
                          -17, -15, -13, -11,  -8,  -6,  -4,  -1},

    [JUMP_TYPE_FAST]  = { -3,  -6,  -9, -11, -14, -16, -18, -20,
                         -22, -24, -26, -28, -29,
                         JUMP_PEAK, JUMP_PEAK,
                         -28, -26, -24, -22, -20, -18, -16, -14,
                         -11, -9,  -6,  -4,  -1},

    [JUMP_TYPE_SLOW]  = { -3,  -6,  -9, -11, -13, -15, -17, -19,
                         -21, -23, -25, -27, -28, -29,
                         JUMP_PEAK, JUMP_PEAK, JUMP_PEAK, JUMP_PEAK,
                         -29, -29, -28, -28, -27, -27, -26, -25,
                         -24, -22, -20, -18, -16, -14, -12, -11,
                          -9,  -6,  -4,  -1},
};

static const struct OamData sOamData_JumpMon =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
    .affineParam = 0
};

static const struct SpriteTemplate sSpriteTemplate_JumpMon =
{
    .tileTag = TAG_MON1,
    .paletteTag = TAG_MON1,
    .oam = &sOamData_JumpMon,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct OamData sOamData_Star =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0
};

static const union AnimCmd sAnim_Star_Still[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd sAnim_Star_Spinning[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(4, 4),
    ANIMCMD_FRAME(8, 4),
    ANIMCMD_FRAME(12, 4),
    ANIMCMD_LOOP(1),
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_END
};

static const union AnimCmd *const sAnims_Star[] =
{
    sAnim_Star_Still,
    sAnim_Star_Spinning
};

static const struct SpriteTemplate sSpriteTemplate_Star =
{
    .tileTag = GFXTAG_STAR,
    .paletteTag = PALTAG_1,
    .oam = &sOamData_Star,
    .anims = sAnims_Star,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

struct {
    u32 score;
    u32 quantity;
} static const sPrizeQuantityData[] =
{
    { .score =  5000, .quantity = 1},
    { .score =  8000, .quantity = 2},
    { .score = 12000, .quantity = 3},
    { .score = 16000, .quantity = 4},
    { .score = 20000, .quantity = 5},
};

////////////////////////////
// VALID FOR POKEMON JUMP //
////////////////////////////

void IsPokemonJumpSpeciesInParty(void)
{
    u32 i;
    
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (IsMonValidSpecies(&gPlayerParty[i]) && IsSpeciesAllowedInPokemonJump(GetMonData(&gPlayerParty[i], MON_DATA_SPECIES)))
        {
            gSpecialVar_Result = TRUE;
            return;
        }
    }
    gSpecialVar_Result = FALSE;
}

bool32 IsSpeciesAllowedInPokemonJump(u32 species)
{
    if (gSpeciesInfo[species].pokemonJumpDisallowed)
        return FALSE;
    
    return (gSpeciesInfo[species].height <= MAX_POKEMON_JUMP_HEIGHT);
}

void FormatPokemonJumpHeightString(void)
{
    u8 *str = ConvertIntToDecimalStringN(gStringVar1, MAX_POKEMON_JUMP_HEIGHT / 10, STR_CONV_MODE_LEFT_ALIGN, 2);
    *str++ = CHAR_PERIOD;
    *str++ = MAX_POKEMON_JUMP_HEIGHT % 10 + CHAR_0;
    *str = EOS;
}

////////////////////
// INITIALIZATION //
////////////////////

static u32 GetSpeciesJumpType(u32 species)
{
    u32 weight = gSpeciesInfo[species].weight;
    
    if (weight <= 185)
        return JUMP_TYPE_FAST;
    else if (weight <= 335)
        return JUMP_TYPE_NORMAL;
    else
        return JUMP_TYPE_SLOW;
}

static void InitPlayerAndJumpTypes(void)
{
    u32 i;

    for (i = 0; i < MAX_RFU_PLAYERS; i++)
        sPokemonJump->players[i].monJumpType = GetSpeciesJumpType(sPokemonJump->members[i].monInfo.species);

    sPokemonJump->player = &sPokemonJump->players[sPokemonJump->multiplayerId];
}

void ResetPokeJumpResults(void)
{
    gSaveBlock2Ptr->pokeJump.jumpsInRow = 0;
    gSaveBlock2Ptr->pokeJump.bestJumpScore = 0;
    gSaveBlock2Ptr->pokeJump.excellentsInRow = 0;
    gSaveBlock2Ptr->pokeJump.gamesWithMaxPlayers = 0;
}

static void IncrementGamesWithMaxPlayers(void)
{
    if (gSaveBlock2Ptr->pokeJump.gamesWithMaxPlayers < 9999)
        gSaveBlock2Ptr->pokeJump.gamesWithMaxPlayers++;
}

static void ResetPlayersForNewGame(void)
{
    u32 i;

    for (i = 0; i < MAX_RFU_PLAYERS; i++)
    {
        sPokemonJump->players[i].jumpTimeStart = 0;
        sPokemonJump->players[i].monState = MONSTATE_NORMAL;
        sPokemonJump->players[i].prevMonState = MONSTATE_NORMAL;
        sPokemonJump->players[i].jumpOffset = 0;
        sPokemonJump->players[i].jumpOffsetIdx = INT_MAX;
        sPokemonJump->players[i].jumpState = JUMPSTATE_NONE;
        sPokemonJump->members[i].funcId = FUNC_NONE;
    }
}

static void ResetPlayersJumpStates(void)
{
    u32 i;
    
    for (i = 0; i < MAX_RFU_PLAYERS; i++)
        sPokemonJump->players[i].jumpState = JUMPSTATE_NONE;
}

static void ResetForNewGame(void)
{
    u32 i;
    
    sPokemonJump->vineState = VINE_UPSWING_LOWER;
    sPokemonJump->prevVineState = VINE_UPSWING_LOWER;
    sPokemonJump->vineTimer = 0;
    sPokemonJump->vineSpeed = 0;
    sPokemonJump->updateScore = FALSE;
    sPokemonJump->isLeader = GetMultiplayerId() == 0;
    sPokemonJump->mainState = 0;
    sPokemonJump->helperState = 0;
    sPokemonJump->excellentsInRow = 0;
    sPokemonJump->excellentsInRowRecord = 0;
    sPokemonJump->initScoreUpdate = FALSE;
    sPokemonJump->numPlayersAtPeak = 0;
    sPokemonJump->allowVineUpdates = FALSE;
    sPokemonJump->allPlayersReady = FALSE;
    sPokemonJump->funcActive = TRUE;
    sPokemonJump->comm.jumpScore = 0;
    sPokemonJump->comm.receivedBonusFlags = 0;
    sPokemonJump->comm.jumpsInRow = 0;
    sPokemonJump->showBonus = FALSE;
    sPokemonJump->giveBonus = FALSE;
    sPokemonJump->linkTimer = 0;
    sPokemonJump->linkTimerLimit = 0;
    
    ResetPlayersForNewGame();

    for (i = 0; i < MAX_RFU_PLAYERS; i++)
    {
        sPokemonJump->members[i].atJumpPeak = FALSE;
        sPokemonJump->members[i].jumpTimeStart = 0;
    }
}

static void InitJumpMonInfo(struct PokemonJump_MonInfo *monInfo, struct Pokemon *mon)
{
    monInfo->species = GetMonData(mon, MON_DATA_SPECIES);
    monInfo->otId = GetMonData(mon, MON_DATA_OT_ID);
    monInfo->personality = GetMonData(mon, MON_DATA_PERSONALITY);
}

static void CB2_PokemonJump(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB_PokemonJump(void)
{
    TransferPlttBuffer();
    LoadOam();
    ProcessSpriteCopyRequests();
}

void StartPokemonJump(u32 partyId, MainCallback exitCallback)
{
    if (gReceivedRemoteLinkPlayers)
    {
        sPokemonJump = Alloc(sizeof(*sPokemonJump));
        
        if (sPokemonJump != NULL)
        {
            ResetTasks();
            
            sPokemonJump->taskId = CreateTask(Task_StartPokemonJump, 1);
            sPokemonJump->exitCallback = exitCallback;
            sPokemonJump->multiplayerId = GetMultiplayerId();
            sPokemonJump->numPlayers = GetLinkPlayerCount();
            sPokemonJump->comm.funcId = FUNC_RESET_GAME;
            sPokemonJump->comm.data = 0;
            
            InitJumpMonInfo(&sPokemonJump->members[sPokemonJump->multiplayerId].monInfo, &gPlayerParty[partyId]);
            InitPlayerAndJumpTypes();
            ResetForNewGame();
            
            if (sPokemonJump->numPlayers == MAX_RFU_PLAYERS)
                IncrementGamesWithMaxPlayers();
            
            SetWordTaskArg(sPokemonJump->taskId, 2, (uintptr_t)sPokemonJump);
            SetMainCallback2(CB2_PokemonJump);
            return;
        }
    }
    // Exit - Players not connected, or alloc failed
    SetMainCallback2(exitCallback);
}

static void FreePokemonJump(void)
{
    FreeAllWindowBuffers();
    DigitObjUtil_Free();
    Free(sPokemonJump);
}

#define tState                    data[0]
#define tNumReceived              data[1]
#define tReceivedPacket(playerId) data[(playerId) + 2]
#define DATAIDX_GAME_STRUCT       14

static inline void SetPokeJumpTask(TaskFunc func)
{
    sPokemonJump->taskId = CreateTask(func, 1);
    sPokemonJump->mainState = 0;
}

static void Task_StartPokemonJump(u32 taskId)
{
    switch (sPokemonJump->mainState)
    {
        case 0:
            SetVBlankCallback(NULL);
            ResetSpriteData();
            FreeAllSpritePalettes();
            SetWordTaskArg(CreateTask(Task_CommunicateMonInfo, 5), DATAIDX_GAME_STRUCT, (uintptr_t)sPokemonJump);
            FadeOutMapMusic(4);
            sPokemonJump->mainState++;
            break;
        case 1:
            if (!FuncIsActiveTask(Task_CommunicateMonInfo))
            {
                StartPokeJumpGfx(&sPokemonJump->jumpGfx);
                LoadWirelessStatusIndicatorSpriteGfx();
                CreateWirelessStatusIndicatorSprite(0, 0);
                sPokemonJump->mainState++;
            }
            break;
        case 2:
            if (!IsPokeJumpGfxFuncFinished() && IsNotWaitingForBGMStop())
            {
                FadeOutAndPlayNewMapMusic(MUS_POKE_JUMP, 8);
                sPokemonJump->mainState++;
            }
            break;
        case 3:
            if (IsLinkTaskFinished())
            {
                BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
                BeginNormalPaletteFade(PALETTES_ALL, -1, 16, 0, RGB_BLACK);
                SetVBlankCallback(VBlankCB_PokemonJump);
                sPokemonJump->mainState++;
            }
            break;
        case 4:
            UpdatePaletteFade();
            
            if (!gPaletteFade.active)
            {
                sPokemonJump->startDelayTimer = 0;
                sPokemonJump->mainState++;
            }
            break;
        case 5:
            if (++sPokemonJump->startDelayTimer >= 20)
            {
                SetPokeJumpTask(sPokemonJump->isLeader ? Task_PokemonJump_Leader : Task_PokemonJump_Member);
                InitVineState();
                DestroyTask(taskId);
            }
            break;
    }
}

static int PokeJumpRandom(void)
{
    sPokemonJump->rngSeed = ISO_RANDOMIZE(sPokemonJump->rngSeed);
    return sPokemonJump->rngSeed >> 16;
}

///////////////////////////////
// PRIZE AND SCORE FUNCTIONS //
///////////////////////////////

static inline bool32 HasEnoughScoreForPrize(void)
{
    if (sPokemonJump->comm.jumpScore >= sPrizeQuantityData[0].score)
        return TRUE;
    else
        return FALSE;
}

static u32 GetPrizeQuantity(void)
{
    u32 i, quantity = 0;

    for (i = 0; i < ARRAY_COUNT(sPrizeQuantityData); i++)
    {
        if (sPokemonJump->comm.jumpScore >= sPrizeQuantityData[i].score)
            quantity = sPrizeQuantityData[i].quantity;
        else
            break;
    }
    return quantity;
}

// Choose a random berry
static inline u32 GetPrizeItemId(void)
{
    return RandomUniform(RNG_NONE, FIRST_BERRY_INDEX, BERRY_ITEMS_END);
}

static u16 GetPrizeData(void)
{
    return (GetPrizeQuantity() << 12) | (GetPrizeItemId() & 0xFFF);
}

static void UnpackPrizeData(u16 data, u16 *itemId, u16 *quantity)
{
    *quantity = data >> 12;
    *itemId = data & 0xFFF;
}

static u32 GetQuantityLimitedByBag(u32 item, u32 quantity)
{
    while (quantity && !CheckBagHasSpace(item, quantity))
        quantity--;

    return quantity;
}

static void PrintPrizeMessage(u32 itemId, u32 quantity)
{
    CopyItemNameHandlePlural(itemId, quantity, gStringVar1);
    ConvertIntToDecimalStringN(gStringVar2, quantity, STR_CONV_MODE_LEFT_ALIGN, 1);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("Awesome score! You've\nwon {STR_VAR_1} {STR_VAR_2}!"));
    sPokemonJumpGfx->msgWindowId = AddMessageWindow(4, 8, 22, 4);
    AddTextPrinterParameterized(sPokemonJumpGfx->msgWindowId, FONT_NORMAL, gStringVar4, 0, 2, 0xFF, NULL);
    CopyWindowToVram(sPokemonJumpGfx->msgWindowId, COPYWIN_GFX);
    sPokemonJumpGfx->fanfare = MUS_LEVEL_UP;
    sPokemonJumpGfx->msgWindowState = 0;
}

static void PrintPrizeFilledBagMessage(void)
{
    sPokemonJumpGfx->msgWindowId = AddMessageWindow(4, 8, 22, 4);
    AddTextPrinterParameterized(sPokemonJumpGfx->msgWindowId, FONT_NORMAL, COMPOUND_STRING("There's no room left now."), 0, 2, 0xFF, NULL);
    CopyWindowToVram(sPokemonJumpGfx->msgWindowId, COPYWIN_GFX);
    sPokemonJumpGfx->fanfare = MUS_DUMMY;
    sPokemonJumpGfx->msgWindowState = 0;
}

static void PrintNoRoomForPrizeMessage(void)
{
    sPokemonJumpGfx->msgWindowId = AddMessageWindow(4, 9, 22, 2);
    AddTextPrinterParameterized(sPokemonJumpGfx->msgWindowId, FONT_NORMAL, COMPOUND_STRING("You can't hold any more!"), 0, 2, 0xFF, NULL);
    CopyWindowToVram(sPokemonJumpGfx->msgWindowId, COPYWIN_GFX);
    sPokemonJumpGfx->fanfare = MUS_DUMMY;
    sPokemonJumpGfx->msgWindowState = 0;
}

static void TryUpdateExcellentsRecord(u32 excellentsInRow)
{
    if (excellentsInRow > sPokemonJump->excellentsInRowRecord)
        sPokemonJump->excellentsInRowRecord = excellentsInRow;
}

static void AddJumpScore(u32 score)
{
    sPokemonJump->updateScore = TRUE;
    sPokemonJump->comm.jumpScore += score;
    if (sPokemonJump->comm.jumpScore >= MAX_JUMP_SCORE)
        sPokemonJump->comm.jumpScore = MAX_JUMP_SCORE;
}

static u32 GetNumPlayersForBonus(void)
{
    u32 i, flags = 0, count = 0;

    for (i = 0; i < MAX_RFU_PLAYERS; i++)
    {
        if (sPokemonJump->members[i].atJumpPeak3)
        {
            flags |= Bit(i);
            count++;
        }
    }

    sPokemonJump->comm.receivedBonusFlags = flags;
    if (flags)
        sPokemonJump->showBonus = TRUE;

    return count;
}

static void TryUpdateScore(void)
{
    u32 i;
    
    if (sPokemonJump->vineState == VINE_UPSWING_HIGH && sPokemonJump->prevVineState == VINE_UPSWING_LOW)
    {
        // Vine has passed through the point where it would hit the players, allow score to update
        if (!sPokemonJump->initScoreUpdate)
        {
            sPokemonJump->numPlayersAtPeak = 0;
            sPokemonJump->initScoreUpdate = TRUE;
            sPokemonJump->comm.receivedBonusFlags = 0;
        }
        else
        {
            if (sPokemonJump->numPlayersAtPeak == MAX_RFU_PLAYERS)
            {
                // An 'excellent' is the max 5 players all jumping synchronously
                sPokemonJump->excellentsInRow++;
                TryUpdateExcellentsRecord(sPokemonJump->excellentsInRow);
            }
            else
                sPokemonJump->excellentsInRow = 0;
            
            if (sPokemonJump->numPlayersAtPeak > 1)
            {
                sPokemonJump->giveBonus = TRUE;
                // Unclear why atJumpPeak needed to be copied over twice
                for (i = 0; i < MAX_RFU_PLAYERS; i++)
                    sPokemonJump->members[i].atJumpPeak3 = sPokemonJump->members[i].atJumpPeak2;
            }
            sPokemonJump->numPlayersAtPeak = 0;
            sPokemonJump->initScoreUpdate = TRUE;
            sPokemonJump->comm.receivedBonusFlags = 0;
            if (sPokemonJump->comm.jumpsInRow < MAX_JUMPS)
                sPokemonJump->comm.jumpsInRow++;

            AddJumpScore(10);
            SetLinkTimeInterval(LINK_INTERVAL_SHORT);
        }
    }

    if (sPokemonJump->giveBonus && (DidAllPlayersClearVine() || sPokemonJump->vineState == VINE_HIGHEST))
    {
        AddJumpScore(sScoreBonuses[GetNumPlayersForBonus()]);
        SetLinkTimeInterval(LINK_INTERVAL_SHORT);
        sPokemonJump->giveBonus = FALSE;
    }

    if (sPokemonJump->initScoreUpdate)
    {
        u32 numAtPeak = GetPlayersAtJumpPeak();
        if (numAtPeak > sPokemonJump->numPlayersAtPeak)
        {
            sPokemonJump->numPlayersAtPeak = numAtPeak;
            
            for (i = 0; i < MAX_RFU_PLAYERS; i++)
                sPokemonJump->members[i].atJumpPeak2 = sPokemonJump->members[i].atJumpPeak;
        }
    }
}

// "Points" for jump score and "times" for number of jumps in a row
static void PrintScoreSuffixes(void)
{
    u8 color[] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY};

    PutWindowTilemap(WIN_POINTS);
    PutWindowTilemap(WIN_TIMES);
    FillWindowPixelBuffer(WIN_POINTS, PIXEL_FILL(0));
    FillWindowPixelBuffer(WIN_TIMES, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WIN_POINTS, FONT_SMALL, 0, 2, color, 0, COMPOUND_STRING(" points"));
    AddTextPrinterParameterized3(WIN_TIMES, FONT_SMALL, 0, 2, color, 0, COMPOUND_STRING(" time(s)"));
}

static void PrintScore(u32 num)
{
    DigitObjUtil_PrintNumOn(WIN_POINTS, num);
}

static void PrintJumpsInRow(u32 num)
{
    DigitObjUtil_PrintNumOn(WIN_TIMES, num);
}

static bool32 UpdateBonus(void)
{
    if (sPokemonJumpGfx->bonusTimer >= 32)
        return FALSE;
    else
    {
        ChangeBgY(BG_BONUSES, 128, 1);
        
        if (++sPokemonJumpGfx->bonusTimer >= 32)
            HideBg(BG_BONUSES);
        
        return TRUE;
    }
}

static void Task_UpdateBonus(u32 taskId)
{
    if (!UpdateBonus())
        DestroyTask(taskId);
}

static void ShowBonus(u32 bonusId)
{
    sPokemonJumpGfx->bonusTimer = 0;
    ChangeBgX(BG_BONUSES, (bonusId / 2) * 256 * 256, 0);
    ChangeBgY(BG_BONUSES, (((bonusId % 2) * 256) - 40) * 256, 0);
    ShowBg(BG_BONUSES);
    CreateTask(Task_UpdateBonus, 4);
}

static u32 DoSameJumpTimeBonus(u8 flags)
{
    u32 i, numPlayers = 0;

    for (i = 0; i < MAX_RFU_PLAYERS; i++)
    {
        if (flags & 1)
        {
            // Player was part of a synchronous jump
            // Give a bonus to them
            DoStarAnim(sPokemonJumpGfx, i);
            numPlayers++;
        }
        flags >>= 1;
    }
    ShowBonus(numPlayers - 2);
    
    return numPlayers;
}

static void UpdateGame(void)
{
    if (sPokemonJump->updateScore)
    {
        sPokemonJump->updateScore = FALSE;
        
        PrintScore(sPokemonJump->comm.jumpScore);
        
        if (sPokemonJump->showBonus)
        {
            sPokemonJump->showBonus = FALSE;
            PlaySE(sSoundEffects[DoSameJumpTimeBonus(sPokemonJump->comm.receivedBonusFlags) - 2]);
        }
    }
    PrintJumpsInRow(sPokemonJump->comm.jumpsInRow);
    HandleMonState();
    TryUpdateVineSwing();
}

static void TryUpdateRecords(u32 jumpScore, u32 jumpsInRow, u32 excellentsInRow)
{
    struct PokemonJumpRecords *records = &gSaveBlock2Ptr->pokeJump;

    if (records->bestJumpScore < jumpScore && jumpScore <= MAX_JUMP_SCORE)
        records->bestJumpScore = jumpScore;
    
    if (records->jumpsInRow < jumpsInRow && jumpsInRow <= MAX_JUMPS)
        records->jumpsInRow = jumpsInRow;
    
    if (records->excellentsInRow < excellentsInRow && excellentsInRow <= MAX_JUMPS)
        records->excellentsInRow = excellentsInRow;
}

static void TruncateToFirstWordOnly(u8 *str)
{
    for (; *str != EOS; str++)
    {
        if (*str == CHAR_SPACE)
        {
            *str = EOS;
            break;
        }
    }
}

static void PrintRecordsText(u32 windowId)
{
    u32 i;
    u8 strbuf[8];
    struct PokemonJumpRecords *records = &gSaveBlock2Ptr->pokeJump;
    int x, recordNums[3];
    
    recordNums[0] = records->jumpsInRow;
    recordNums[1] = records->bestJumpScore;
    recordNums[2] = records->excellentsInRow;
    
    TextWindow_SetStdFrame0_WithPal(windowId, 0x21D, 0xD0);
    DrawTextBorderOuter(windowId, 0x21D, 13);
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    AddTextPrinterParameterized5(windowId, FONT_NORMAL, COMPOUND_STRING("Pokémon Jump Records"), 0, 0, 0xFF, NULL, 1, 0);
    
    for (i = 0; i < ARRAY_COUNT(sRecordsTexts); i++)
    {
        AddTextPrinterParameterized5(windowId, FONT_NORMAL, sRecordsTexts[i], 0, 20 + (i * 14), 0xFF, NULL, 1, 0);
        ConvertIntToDecimalStringN(strbuf, recordNums[i], STR_CONV_MODE_LEFT_ALIGN, 5);
        TruncateToFirstWordOnly(strbuf);
        x = 0xDE - GetStringWidth(FONT_NORMAL, strbuf, 0);
        AddTextPrinterParameterized5(windowId, FONT_NORMAL, strbuf, x, 20 + (i * 14), 0xFF, NULL, 0, 0);
    }
    PutWindowTilemap(windowId);
}

#define tStep     data[0]
#define tWindowId data[1]

static void Task_ShowPokemonJumpRecords(u32 taskId)
{
    s16 *data = gTasks[taskId].data;

    switch (tStep)
    {
        case 0:
            tWindowId = AddWindow(&sWindowTemplate_Records);
            PrintRecordsText(tWindowId);
            CopyWindowToVram(tWindowId, COPYWIN_BOTH);
            tStep++;
            break;
        case 1:
            if (!IsDma3ManagerBusyWithBgCopy())
                tStep++;
            break;
        case 2:
            if (JOY_NEW(A_BUTTON | B_BUTTON))
            {
                rbox_fill_rectangle(tWindowId);
                CopyWindowToVram(tWindowId, COPYWIN_MAP);
                tStep++;
            }
            break;
        case 3:
            if (!IsDma3ManagerBusyWithBgCopy())
            {
                RemoveWindow(tWindowId);
                EnableBothScriptContexts();
                DestroyTask(taskId);
            }
            break;
    }
}

#undef tStep
#undef tWindowId

void ShowPokemonJumpRecords(void)
{
    Task_ShowPokemonJumpRecords(CreateTask(Task_ShowPokemonJumpRecords, 0));
}

/////////////
// WINDOWS //
/////////////

static void AddPlayerNameWindows(void)
{
    struct WindowTemplate window;
    u32 i, playersCount = GetLinkPlayerCount();
    const u8 *winCoords = sPlayerNameWindowCoords[playersCount - 2];

    window.bg = BG_INTERFACE;
    window.width = 8;
    window.height = 2;
    window.paletteNum = 2;
    window.baseBlock = 0x2B;

    for (i = 0; i < playersCount; i++)
    {
        window.tilemapLeft = winCoords[0];
        window.tilemapTop = winCoords[1];
        sPokemonJumpGfx->nameWindowIds[i] = AddWindow(&window);
        ClearWindowTilemap(sPokemonJumpGfx->nameWindowIds[i]);
        window.baseBlock += 0x10;
        winCoords += 2;
    }
    CopyBgTilemapBufferToVram(BG_INTERFACE);
}

static void PrintPokeJumpPlayerName(u32 multiplayerId, u32 bgColor, u32 fgColor, u32 shadow)
{
    u32 x;
    u8 colors[3] = {bgColor, fgColor, shadow};

    FillWindowPixelBuffer(sPokemonJumpGfx->nameWindowIds[multiplayerId], PIXEL_FILL(0));
    
    x = (64 - GetStringWidth(FONT_SMALL, sPokemonJump->players[multiplayerId].name, -1)) / 2;
    AddTextPrinterParameterized3(sPokemonJumpGfx->nameWindowIds[multiplayerId], FONT_SMALL, x, 2, colors, 0xFF, sPokemonJump->players[multiplayerId].name);
    CopyWindowToVram(sPokemonJumpGfx->nameWindowIds[multiplayerId], COPYWIN_GFX);
}

static void PrintPokeJumpPlayerNames(bool32 highlightSelf)
{
    u32 i, multiplayerId, playersCount = GetLinkPlayerCount();

    if (!highlightSelf)
    {
        for (i = 0; i < playersCount; i++)
            PrintPokeJumpPlayerName(i, TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY);
    }
    else
    {
        // Highlight own name
        multiplayerId = sPokemonJump->multiplayerId;
        
        for (i = 0; i < playersCount; i++)
        {
            if (multiplayerId != i)
                PrintPokeJumpPlayerName(i, TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY);
            else
                PrintPokeJumpPlayerName(i, TEXT_COLOR_TRANSPARENT, TEXT_COLOR_RED, TEXT_COLOR_LIGHT_RED);
        }
    }
}

static void DrawPlayerNameWindows(void)
{
    u32 i, playersCount = GetLinkPlayerCount();

    for (i = 0; i < playersCount; i++)
        PutWindowTilemap(sPokemonJumpGfx->nameWindowIds[i]);
    
    CopyBgTilemapBufferToVram(BG_INTERFACE);
}

static void ErasePlayerNames(void)
{
    u32 i, numPlayers = GetLinkPlayerCount();

    switch (sPokemonJumpGfx->mainState)
    {
        case 0:
            for (i = 0; i < numPlayers; i++)
                ClearWindowTilemap(sPokemonJumpGfx->nameWindowIds[i]);
    
            CopyBgTilemapBufferToVram(BG_INTERFACE);
            sPokemonJumpGfx->mainState++;
            break;
        case 1:
            if (!IsDma3ManagerBusyWithBgCopy())
            {
                for (i = 0; i < numPlayers; i++)
                    RemoveWindow(sPokemonJumpGfx->nameWindowIds[i]);
    
                sPokemonJumpGfx->funcFinished = TRUE;
            }
            break;
    }
}

static u32 AddMessageWindow(u32 left, u32 top, u32 width, u32 height)
{
    u32 windowId;
    struct WindowTemplate window;

    window.bg = BG_INTERFACE;
    window.tilemapLeft = left;
    window.tilemapTop = top;
    window.width = width;
    window.height = height;
    window.paletteNum = 15;
    window.baseBlock = 0x43;

    windowId = AddWindow(&window);
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    return windowId;
}

static void ClearMessageWindow(void)
{
    if (sPokemonJumpGfx->msgWindowId != 0xFF)
    {
        rbox_fill_rectangle(sPokemonJumpGfx->msgWindowId);
        CopyWindowToVram(sPokemonJumpGfx->msgWindowId, COPYWIN_MAP);
        sPokemonJumpGfx->msgWindowState = 0;
    }
}

static bool32 RemoveMessageWindow(void)
{
    if (sPokemonJumpGfx->msgWindowId == 0xFF)
        return FALSE;

    switch (sPokemonJumpGfx->msgWindowState)
    {
        case 0:
            if (!IsDma3ManagerBusyWithBgCopy())
            {
                RemoveWindow(sPokemonJumpGfx->msgWindowId);
                sPokemonJumpGfx->msgWindowId = 0xFF;
                sPokemonJumpGfx->msgWindowState++;
            }
            else
                break;
        case 1:
            return FALSE;
    }
    return TRUE;
}

static void CreatePokeJumpYesNoMenu(u32 left, u32 top, u32 cursorPos)
{
    struct WindowTemplate window;
    
    window.bg = BG_INTERFACE;
    window.tilemapLeft = left;
    window.tilemapTop = top;
    window.width = 6;
    window.height = 4;
    window.paletteNum = 2;
    window.baseBlock = 0x2B;

    CreateYesNoMenu(&window, FONT_NORMAL, 0, 2, 0x00a, 13, cursorPos);
}

////////////////////////////////////////
// COMMUNICATION AND SEND/RECV PACKET //
////////////////////////////////////////

static void SetLinkTimeInterval(u32 intervalId)
{
    if (intervalId == LINK_INTERVAL_NONE)
    {
        // Link data is sent when timer reaches 0.
        // Set timer to 1 and set limit to special
        // 'stopped' value so timer won't change        
        sPokemonJump->linkTimerLimit = LINK_TIMER_STOPPED;
        sPokemonJump->linkTimer = 1;
    }
    else
    {
        sPokemonJump->linkTimerLimit = (1 << (intervalId - 1)) - 1;
        sPokemonJump->linkTimer = 0;
    }
}

static bool32 AreLinkQueuesEmpty(void)
{
    return (Rfu.recvQueue.count == 0 && Rfu.sendQueue.count == 0);
}

static void SendPacket_MonInfo(struct PokemonJump_MonInfo *monInfo)
{
    struct MonInfoPacket packet;
    packet.id = PACKET_MON_INFO;
    packet.species = monInfo->species;
    packet.otId = monInfo->otId;
    packet.personality = monInfo->personality;
    Rfu_SendPacket(&packet);
}

static bool32 RecvPacket_MonInfo(u32 multiplayerId, struct PokemonJump_MonInfo *monInfo)
{
    struct MonInfoPacket packet;

    if ((gRecvCmds[multiplayerId][0] & RFUCMD_MASK) != RFUCMD_SEND_PACKET)
        return FALSE;

    memcpy(&packet, &gRecvCmds[multiplayerId][1], sizeof(packet));
    if (packet.id == PACKET_MON_INFO)
    {
        monInfo->species = packet.species;
        monInfo->otId = packet.otId;
        monInfo->personality = packet.personality;
        return TRUE;
    }
    return FALSE;
}

static void Task_CommunicateMonInfo(u32 taskId)
{
    u32 i;
    s16 *data = gTasks[taskId].data;
    struct PokemonJump *jump = (struct PokemonJump *)GetWordTaskArg(taskId, DATAIDX_GAME_STRUCT);

    switch (tState)
    {
        case 0:
            for (i = 0; i < MAX_RFU_PLAYERS; i++)
                tReceivedPacket(i) = FALSE;
    
            tState++;
            // fall through
        case 1:
            SendPacket_MonInfo(&jump->members[jump->multiplayerId].monInfo);
            
            for (i = 0; i < MAX_RFU_PLAYERS; i++)
            {
                if (!tReceivedPacket(i) && RecvPacket_MonInfo(i, &jump->members[i].monInfo))
                {
                    StringCopy(jump->players[i].name, gLinkPlayers[i].name);
                    tReceivedPacket(i) = TRUE;
    
                    if (++tNumReceived == jump->numPlayers)
                    {
                        InitPlayerAndJumpTypes();
                        DestroyTask(taskId);
                        break;
                    }
                }
            }
            break;
    }
}

#undef tState
#undef tNumReceived
#undef tReceivedPacket
#undef DATAIDX_GAME_STRUCT

static void SendPacket_LeaderState(struct PokemonJump_Player *player, struct PokemonJump_CommData *comm)
{
    struct LeaderStatePacket packet;
    packet.id = PACKET_LEADER_STATE;
    packet.jumpScore = comm->jumpScore;
    packet.receivedBonusFlags = comm->receivedBonusFlags;
    packet.funcId = comm->funcId;
    packet.vineTimer = comm->data;
    packet.jumpsInRow = comm->jumpsInRow;
    packet.monState = player->monState;
    packet.jumpState = player->jumpState;
    packet.jumpTimeStart = player->jumpTimeStart;
    Rfu_SendPacket(&packet);
}

static void SendLinkData_Leader(void)
{
    if (!sPokemonJump->linkTimer)
        SendPacket_LeaderState(sPokemonJump->players, &sPokemonJump->comm);

    if (sPokemonJump->linkTimerLimit != LINK_TIMER_STOPPED)
    {
        sPokemonJump->linkTimer++;
        sPokemonJump->linkTimer &= sPokemonJump->linkTimerLimit;
    }
}

static void SendPacket_MemberState(struct PokemonJump_Player *player, u32 funcId, u32 playAgainState)
{
    struct MemberStatePacket packet;
    packet.id = PACKET_MEMBER_STATE;
    packet.monState = player->monState;
    packet.jumpState = player->jumpState;
    packet.funcFinished = player->funcFinished;
    packet.jumpTimeStart = player->jumpTimeStart;
    packet.funcId = funcId;
    packet.playAgainState = playAgainState;
    Rfu_SendPacket(&packet);
}

static void SendLinkData_Member(void)
{
    if (!sPokemonJump->linkTimer)
        SendPacket_MemberState(&sPokemonJump->players[sPokemonJump->multiplayerId], sPokemonJump->comm.funcId, sPokemonJump->playAgainComm);

    if (sPokemonJump->linkTimerLimit != LINK_TIMER_STOPPED)
    {
        sPokemonJump->linkTimer++;
        sPokemonJump->linkTimer &= sPokemonJump->linkTimerLimit;
    }
}

// Used by the group leader to read the state of group members
static bool32 RecvPacket_MemberStateToLeader(struct PokemonJump_Player *player, u32 multiplayerId, u8 *funcId, u32 *playAgainState)
{
    struct MemberStatePacket packet;

    if ((gRecvCmds[multiplayerId][0] & RFUCMD_MASK) != RFUCMD_SEND_PACKET)
        return FALSE;

    memcpy(&packet, &gRecvCmds[multiplayerId][1], sizeof(packet));
    if (packet.id != PACKET_MEMBER_STATE)
        return FALSE;

    player->monState = packet.monState;
    player->jumpState = packet.jumpState;
    player->funcFinished = packet.funcFinished;
    player->jumpTimeStart = packet.jumpTimeStart;
    *funcId = packet.funcId;
    *playAgainState = packet.playAgainState;
    return TRUE;
}

// Used by group members to read the state of other group members
static bool32 RecvPacket_MemberStateToMember(struct PokemonJump_Player *player, u32 multiplayerId)
{
    struct MemberStatePacket packet;

    if ((gRecvCmds[multiplayerId][0] & RFUCMD_MASK) != RFUCMD_SEND_PACKET)
        return FALSE;

    memcpy(&packet, &gRecvCmds[multiplayerId][1], sizeof(packet));
    if (packet.id != PACKET_MEMBER_STATE)
        return FALSE;

    player->monState = packet.monState;
    player->jumpState = packet.jumpState;
    player->funcFinished = packet.funcFinished;
    player->jumpTimeStart = packet.jumpTimeStart;
    return TRUE;
}

// Used by group members to read the state of the group leader
static bool32 RecvPacket_LeaderState(struct PokemonJump_Player *player, struct PokemonJump_CommData *comm)
{
    struct LeaderStatePacket packet;

    if ((gRecvCmds[0][0] & RFUCMD_MASK) != RFUCMD_SEND_PACKET)
        return FALSE;

    memcpy(&packet, &gRecvCmds[0][1], sizeof(packet));
    if (packet.id != PACKET_LEADER_STATE)
        return FALSE;

    comm->jumpScore = packet.jumpScore;
    comm->receivedBonusFlags = packet.receivedBonusFlags;
    comm->funcId = packet.funcId;
    comm->data = packet.vineTimer;
    comm->jumpsInRow = packet.jumpsInRow;
    player->monState = packet.monState;
    player->jumpState = packet.jumpState;
    player->jumpTimeStart = packet.jumpTimeStart;
    return TRUE;
}

static void RecvLinkData_Leader(void)
{
    u32 i, numReady, monState, playAgainState;
    u8 funcId;

    for (i = 1, numReady = 0; i < sPokemonJump->numPlayers; i++)
    {
        monState = sPokemonJump->players[i].monState;
        
        if (RecvPacket_MemberStateToLeader(&sPokemonJump->players[i], i, &funcId, &playAgainState))
        {
            sPokemonJump->members[i].playAgainState = playAgainState;
            sPokemonJump->members[i].funcId = funcId;
            sPokemonJump->players[i].prevMonState = monState;
        }

        if (sPokemonJump->players[i].funcFinished && sPokemonJump->members[i].funcId == sPokemonJump->comm.funcId)
            numReady++;
    }

    if (numReady == sPokemonJump->numPlayers - 1)
        sPokemonJump->allPlayersReady = TRUE;
}

static void SetFunc_Leader(u32 funcId)
{
    u32 i;

    sPokemonJump->comm.funcId = funcId;
    sPokemonJump->mainState = 0;
    sPokemonJump->helperState = 0;
    sPokemonJump->funcActive = TRUE;
    sPokemonJump->allPlayersReady = FALSE;
    
    for (i = 1; i < sPokemonJump->numPlayers; i++)
        sPokemonJump->players[i].funcFinished = FALSE;
}

static void SetFunc_Member(u32 funcId)
{
    sPokemonJump->comm.funcId = funcId;
    sPokemonJump->mainState = 0;
    sPokemonJump->helperState = 0;
    sPokemonJump->funcActive = TRUE;
    sPokemonJump->players[sPokemonJump->multiplayerId].funcFinished = FALSE;
}

static void Task_PokemonJump_Leader(u32 taskId)
{
    RecvLinkData_Leader();
    TryUpdateScore();
    
    if (!sPokemonJump->funcActive && sPokemonJump->allPlayersReady)
    {
        SetFunc_Leader(sPokemonJump->nextFuncId);
        SetLinkTimeInterval(LINK_INTERVAL_SHORT);
    }

    if (sPokemonJump->funcActive)
    {
        if (!sPokeJumpLeaderFuncs[sPokemonJump->comm.funcId]())
        {
            sPokemonJump->funcActive = FALSE;
            sPokemonJump->players[sPokemonJump->multiplayerId].funcFinished = TRUE;
        }
    }
    UpdateGame();
    SendLinkData_Leader();
}

static void RecvLinkData_Member(void)
{
    u32 i, monState = sPokemonJump->players[0].monState;
    struct PokemonJump_CommData leaderData;

    if (RecvPacket_LeaderState(sPokemonJump->players, &leaderData))
    {
        if (sPokemonJump->players[sPokemonJump->multiplayerId].funcFinished && leaderData.funcId != sPokemonJump->comm.funcId)
            SetFunc_Member(leaderData.funcId);

        if (sPokemonJump->comm.jumpScore != leaderData.jumpScore)
        {
            sPokemonJump->comm.jumpScore = leaderData.jumpScore;
            sPokemonJump->updateScore = TRUE;
            sPokemonJump->comm.receivedBonusFlags = leaderData.receivedBonusFlags;
            if (sPokemonJump->comm.receivedBonusFlags)
                sPokemonJump->showBonus = TRUE;
            else
                sPokemonJump->showBonus = FALSE;
        }
        sPokemonJump->comm.data = leaderData.data;
        sPokemonJump->comm.jumpsInRow = leaderData.jumpsInRow;
        sPokemonJump->players[0].prevMonState = monState;
    }

    for (i = 1; i < sPokemonJump->numPlayers; i++)
    {
        if (i != sPokemonJump->multiplayerId)
        {
            monState = sPokemonJump->players[i].monState;
            
            if (RecvPacket_MemberStateToMember(&sPokemonJump->players[i], i))
                sPokemonJump->players[i].prevMonState = monState;
        }
    }
}

static void Task_PokemonJump_Member(u32 taskId)
{
    RecvLinkData_Member();
    
    if (sPokemonJump->funcActive)
    {
        if (!sPokeJumpMemberFuncs[sPokemonJump->comm.funcId]())
        {
            sPokemonJump->funcActive = FALSE;
            sPokemonJump->players[sPokemonJump->multiplayerId].funcFinished = TRUE;
            SetLinkTimeInterval(LINK_INTERVAL_SHORT);
        }
    }
    UpdateGame();
    SendLinkData_Member();
}

static bool32 ResetVineGfx(void)
{
    switch (sPokemonJumpGfx->resetVineState)
    {
        case 0:
            if (++sPokemonJumpGfx->resetVineTimer > 10)
            {
                sPokemonJumpGfx->resetVineTimer = 0;
                
                if (++sPokemonJumpGfx->vineState >= NUM_VINESTATES)
                {
                    sPokemonJumpGfx->vineState = VINE_HIGHEST;
                    sPokemonJumpGfx->resetVineState++;
                }
            }
            UpdateVineSwing(sPokemonJumpGfx->vineState);
            
            if (sPokemonJumpGfx->vineState != VINE_UPSWING_LOW)
                break;
        case 1:
            return FALSE;
    }
    return TRUE;
}

static bool32 DoGameIntro(void)
{
    switch (sPokemonJump->helperState)
    {
        case 0:
            SetUpPokeJumpGfxFuncById(GFXFUNC_SHOW_NAMES_HIGHLIGHT);
            ResetMonSpriteSubpriorities();
            sPokemonJump->helperState++;
            break;
        case 1:
            if (!IsPokeJumpGfxFuncFinished())
            {
                StartMonIntroBounce(sPokemonJump->multiplayerId);
                sPokemonJump->timer = 0;
                sPokemonJump->helperState++;
            }
            break;
        case 2:
            if (++sPokemonJump->timer > 120)
            {
                SetUpPokeJumpGfxFuncById(GFXFUNC_ERASE_NAMES);
                sPokemonJump->helperState++;
            }
            break;
        case 3:
            if (!IsPokeJumpGfxFuncFinished() && !IsMonIntroBounceActive())
                sPokemonJump->helperState++;
            break;
        case 4:
            SetUpPokeJumpGfxFuncById(GFXFUNC_COUNTDOWN);
            sPokemonJump->helperState++;
            break;
        case 5:
            if (!IsPokeJumpGfxFuncFinished())
            {
                sPokemonJump->allowVineUpdates = FALSE;
                SetUpResetVineGfx();
                sPokemonJump->helperState++;
            }
            break;
        case 6:
            if (!ResetVineGfx())
            {
                sPokemonJump->allowVineUpdates = TRUE;
                ResetVineState();
                sPokemonJump->helperState++;
                return FALSE;
            }
            break;
        case 7:
            return FALSE;
    }
    return TRUE;
}

static bool32 GameIntro_Leader(void)
{
    switch (sPokemonJump->mainState)
    {
        case 0:
            SetLinkTimeInterval(LINK_INTERVAL_SHORT);
            sPokemonJump->mainState++;
            // fall through
        case 1:
            if (!DoGameIntro())
            {
                sPokemonJump->comm.data = sPokemonJump->vineTimer;
                sPokemonJump->nextFuncId = FUNC_WAIT_ROUND;
                return FALSE;
            }
            break;
    }
    return TRUE;
}

static bool32 GameIntro_Member(void)
{
    switch (sPokemonJump->mainState)
    {
        case 0:
            SetLinkTimeInterval(LINK_INTERVAL_NONE);
            sPokemonJump->rngSeed = sPokemonJump->comm.data;
            sPokemonJump->mainState++;
            // fall through
        case 1:
            return DoGameIntro();
    }
    return TRUE;
}

static bool32 WaitRound_Leader(void)
{
    switch (sPokemonJump->mainState)
    {
        case 0:
            ResetPlayersJumpStates();
            SetLinkTimeInterval(LINK_INTERVAL_LONG);
            sPokemonJump->mainState++;
            break;
        case 1:
            if (sPokemonJump->allPlayersReady)
            {
                sPokemonJump->nextFuncId = FUNC_GAME_ROUND;
                return FALSE;
            }
            break;
    }
    return TRUE;
}

static bool32 WaitRound_Member(void)
{
    switch (sPokemonJump->mainState)
    {
        case 0:
            ResetPlayersJumpStates();
            SetLinkTimeInterval(LINK_INTERVAL_NONE);
            sPokemonJump->vineTimer = sPokemonJump->comm.data;
            sPokemonJump->mainState++;
            // fall through
        case 1:
            if (AreLinkQueuesEmpty())
                return FALSE;
            break;
    }
    return TRUE;
}

static bool32 GameRound_Leader(void)
{
    if (!HandleSwingRound())
    {
        sPokemonJump->comm.data = sPokemonJump->vineTimer;
        sPokemonJump->nextFuncId = FUNC_WAIT_ROUND;
    }
    else if (UpdateVineHitStates())
        return TRUE;
    else
    {
        // Someone hit the vine
        ResetVineAfterHit();
        sPokemonJump->nextFuncId = FUNC_GAME_OVER;
    }
    return FALSE;
}

static bool32 GameRound_Member(void)
{
    if (!HandleSwingRound())
        ;
    else if (UpdateVineHitStates())
        return TRUE;
    else // Someone hit the vine
        ResetVineAfterHit();

    return FALSE;
}

static bool32 GameOver_Leader(void)
{
    switch (sPokemonJump->mainState)
    {
        case 0:
            UpdateVineHitStates();
            
            if (AllPlayersJumpedOrHit())
                sPokemonJump->mainState++;
            break;
        case 1:
            if (!DoVineHitEffect())
            {
                if (HasEnoughScoreForPrize())
                {
                    sPokemonJump->comm.data = GetPrizeData();
                    sPokemonJump->nextFuncId = FUNC_GIVE_PRIZE;
                }
                else if (sPokemonJump->comm.jumpsInRow >= 200)
                {
                    sPokemonJump->comm.data = sPokemonJump->excellentsInRowRecord;
                    sPokemonJump->nextFuncId = FUNC_SAVE;
                }
                else
                {
                    sPokemonJump->comm.data = sPokemonJump->excellentsInRowRecord;
                    sPokemonJump->nextFuncId = FUNC_ASK_PLAY_AGAIN;
                }
                sPokemonJump->mainState++;
                return FALSE;
            }
            break;
        case 2:
            return FALSE;
    }
    return TRUE;
}

static bool32 GameOver_Member(void)
{
    switch (sPokemonJump->mainState)
    {
        case 0:
            if (!UpdateVineHitStates())
                ResetVineAfterHit();
            if (AllPlayersJumpedOrHit())
                sPokemonJump->mainState++;
            break;
        case 1:
            if (!DoVineHitEffect())
            {
                sPokemonJump->mainState++;
                return FALSE;
            }
            break;
        case 2:
            return FALSE;
    }
    return TRUE;
}

static bool32 ShouldPlayAgain(void)
{
    u32 i;

    if (sPokemonJump->playAgainState == 1)
        return FALSE;

    for (i = 1; i < sPokemonJump->numPlayers; i++)
    {
        if (sPokemonJump->members[i].playAgainState == 1)
            return FALSE;
    }
    return TRUE;
}

static bool32 DoPlayAgainPrompt(void)
{
    switch (sPokemonJump->helperState)
    {
        case 0:
            SetUpPokeJumpGfxFuncById(GFXFUNC_MSG_PLAY_AGAIN);
            sPokemonJump->helperState++;
            break;
        case 1:
            if (!IsPokeJumpGfxFuncFinished())
                sPokemonJump->helperState++;
            break;
        case 2:
            switch (Menu_ProcessInputNoWrapClearOnChoose())
            {
                case MENU_B_PRESSED:
                case 1: // No
                    sPokemonJump->playAgainState = 1;
                    SetUpPokeJumpGfxFuncById(GFXFUNC_ERASE_MSG);
                    sPokemonJump->helperState++;
                    break;
                case 0: // Yes
                    sPokemonJump->playAgainState = 2;
                    SetUpPokeJumpGfxFuncById(GFXFUNC_ERASE_MSG);
                    sPokemonJump->helperState++;
                    break;
            }
            break;
        case 3:
            if (!IsPokeJumpGfxFuncFinished())
                sPokemonJump->helperState++;
            break;
        case 4:
            SetUpPokeJumpGfxFuncById(GFXFUNC_MSG_COMM_STANDBY);
            sPokemonJump->helperState++;
            break;
        case 5:
            if (!IsPokeJumpGfxFuncFinished())
            {
                sPokemonJump->helperState++;
                return FALSE;
            }
            break;
        case 6:
            return FALSE;
    }
    return TRUE;
}

static bool32 AskPlayAgain_Leader(void)
{
    switch (sPokemonJump->mainState)
    {
        case 0:
            SetLinkTimeInterval(LINK_INTERVAL_MEDIUM);
            sPokemonJump->mainState++;
            // fall through
        case 1:
            if (!DoPlayAgainPrompt())
            {
                TryUpdateRecords(sPokemonJump->comm.jumpScore, sPokemonJump->comm.jumpsInRow, sPokemonJump->comm.data);
                sPokemonJump->mainState++;
            }
            break;
        case 2:
            if (sPokemonJump->allPlayersReady)
            {
                if (ShouldPlayAgain())
                    sPokemonJump->nextFuncId = FUNC_RESET_GAME;
                else
                    sPokemonJump->nextFuncId = FUNC_EXIT;
    
                sPokemonJump->mainState++;
                return FALSE;
            }
            break;
        case 3:
            return FALSE;
    }
    return TRUE;
}

static bool32 AskPlayAgain_Member(void)
{
    switch (sPokemonJump->mainState)
    {
        case 0:
            SetLinkTimeInterval(LINK_INTERVAL_NONE);
            sPokemonJump->mainState++;
            // fall through
        case 1:
            if (!DoPlayAgainPrompt())
            {
                TryUpdateRecords(sPokemonJump->comm.jumpScore, sPokemonJump->comm.jumpsInRow, sPokemonJump->comm.data);
                sPokemonJump->playAgainComm = sPokemonJump->playAgainState;
                return FALSE;
            }
            break;
    }
    return TRUE;
}

static bool32 CloseMessageAndResetScore(void)
{
    switch (sPokemonJump->helperState)
    {
        case 0:
            ClearMessageWindow();
            PrintScore(0);
            sPokemonJump->helperState++;
            break;
        case 1:
            if (!RemoveMessageWindow())
            {
                sPokemonJump->helperState++;
                return FALSE;
            }
            break;
        case 2:
            return FALSE;
    }
    return TRUE;
}

static bool32 ResetGame_Leader(void)
{
    switch (sPokemonJump->mainState)
    {
        case 0:
            if (!CloseMessageAndResetScore())
                sPokemonJump->mainState++;
            break;
        case 1:
            if (sPokemonJump->allPlayersReady)
            {
                ResetForNewGame();
                sPokemonJump->rngSeed = Random();
                sPokemonJump->comm.data = sPokemonJump->rngSeed;
                sPokemonJump->nextFuncId = FUNC_GAME_INTRO;
                return FALSE;
            }
            break;
    }
    return TRUE;
}

static bool32 ResetGame_Member(void)
{
    switch (sPokemonJump->mainState)
    {
        case 0:
            if (!CloseMessageAndResetScore())
            {
                ResetForNewGame();
                sPokemonJump->mainState++;
                return FALSE;
            }
            break;
        case 1:
            return FALSE;
    }
    return TRUE;
}

static bool32 ClosePokeJumpLink(void)
{
    switch (sPokemonJump->helperState)
    {
        case 0:
            ClearMessageWindow();
            sPokemonJump->helperState++;
            break;
        case 1:
            if (!RemoveMessageWindow())
            {
                SetUpPokeJumpGfxFuncById(GFXFUNC_MSG_PLAYER_DROPPED);
                sPokemonJump->helperState++;
            }
            break;
        case 2:
            if (!IsPokeJumpGfxFuncFinished())
            {
                sPokemonJump->timer = 0;
                sPokemonJump->helperState++;
            }
            break;
        case 3:
            if (++sPokemonJump->timer > 120)
            {
                BeginNormalPaletteFade(PALETTES_ALL, -1, 0, 16, RGB_BLACK);
                sPokemonJump->helperState++;
            }
            break;
        case 4:
            if (!gPaletteFade.active)
            {
                SetCloseLinkCallback();
                sPokemonJump->helperState++;
            }
            break;
        case 5:
            if (!gReceivedRemoteLinkPlayers)
                return FALSE;
            break;
    }
    return TRUE;
}

static bool32 ExitGame(void)
{
    switch (sPokemonJump->mainState)
    {
        case 0:
            sPokemonJump->mainState = 1;
            break;
        case 1:
            SetLinkTimeInterval(LINK_INTERVAL_NONE);
            sPokemonJump->mainState++;
            break;
        case 2:
            if (!ClosePokeJumpLink())
            {
                SetMainCallback2(sPokemonJump->exitCallback);
                FreePokemonJump();
            }
            break;
    }
    return TRUE;
}

static bool32 DoPrizeMessageAndFanfare(void)
{
    switch (sPokemonJumpGfx->msgWindowState)
    {
        case 0:
            if (!IsDma3ManagerBusyWithBgCopy())
            {
                PutWindowTilemap(sPokemonJumpGfx->msgWindowId);
                DrawTextBorderOuter(sPokemonJumpGfx->msgWindowId, 1, 14);
                CopyBgTilemapBufferToVram(BG_INTERFACE);
                sPokemonJumpGfx->msgWindowState++;
            }
            break;
        case 1:
            if (IsDma3ManagerBusyWithBgCopy())
                break;
            
            if (sPokemonJumpGfx->fanfare == MUS_DUMMY)
            {
                sPokemonJumpGfx->msgWindowState += 2;
                return FALSE;
            }
            PlayFanfare(sPokemonJumpGfx->fanfare);
            sPokemonJumpGfx->msgWindowState++;
        case 2:
            if (!IsFanfareTaskInactive())
                break;
            sPokemonJumpGfx->msgWindowState++;
        case 3:
            return FALSE;
    }
    return TRUE;
}

static bool32 TryGivePrize(void)
{
    switch (sPokemonJump->helperState)
    {
        case 0:
            UnpackPrizeData(sPokemonJump->comm.data, &sPokemonJump->prizeItemId, &sPokemonJump->prizeItemQuantity);
            PrintPrizeMessage(sPokemonJump->prizeItemId, sPokemonJump->prizeItemQuantity);
            sPokemonJump->helperState++;
            break;
        case 1:
        case 4:
            if (!DoPrizeMessageAndFanfare())
            {
                sPokemonJump->timer = 0;
                sPokemonJump->helperState++;
            }
            break;
        case 2:
        case 5:
            // Wait to continue after message
            sPokemonJump->timer++;
            
            if (JOY_NEW(A_BUTTON | B_BUTTON) || sPokemonJump->timer > 180)
            {
                ClearMessageWindow();
                sPokemonJump->helperState++;
            }
            break;
        case 3:
            if (!RemoveMessageWindow())
            {
                sPokemonJump->prizeItemQuantity = GetQuantityLimitedByBag(sPokemonJump->prizeItemId, sPokemonJump->prizeItemQuantity);
                
                if (sPokemonJump->prizeItemQuantity && AddBagItem(sPokemonJump->prizeItemId, sPokemonJump->prizeItemQuantity))
                {
                    if (!CheckBagHasSpace(sPokemonJump->prizeItemId, 1))
                    {
                        // An item was given successfully, but no room for any more.
                        // It's possible the full prize quantity had to be limited
                        PrintPrizeFilledBagMessage();
                        sPokemonJump->helperState = 4; // Do message
                    }
                    else
                    {
                        sPokemonJump->helperState = 6; // Exit
                        break;
                    }
                }
                else
                {
                    PrintNoRoomForPrizeMessage();
                    sPokemonJump->helperState = 4; // Do message
                }
            }
            break;
        case 6:
            if (!RemoveMessageWindow())
                return FALSE;
            break;
    }
    return TRUE;
}

static bool32 GivePrize_Leader(void)
{
    switch (sPokemonJump->mainState)
    {
        case 0:
            SetLinkTimeInterval(LINK_INTERVAL_MEDIUM);
            sPokemonJump->mainState++;
            break;
        case 1:
            if (!TryGivePrize())
            {
                sPokemonJump->comm.data = sPokemonJump->excellentsInRowRecord;
                sPokemonJump->nextFuncId = FUNC_SAVE;
                return FALSE;
            }
            break;
    }
    return TRUE;
}

static bool32 GivePrize_Member(void)
{
    SetLinkTimeInterval(LINK_INTERVAL_NONE);
    
    if (!TryGivePrize())
        return FALSE;
    else
        return TRUE;
}

static bool32 SavePokeJump(void)
{
    switch (sPokemonJump->mainState)
    {
        case 0:
            TryUpdateRecords(sPokemonJump->comm.jumpScore, sPokemonJump->comm.jumpsInRow, sPokemonJump->comm.data);
            SetUpPokeJumpGfxFuncById(GFXFUNC_MSG_SAVING);
            sPokemonJump->mainState++;
            break;
        case 1:
            if (!IsPokeJumpGfxFuncFinished())
            {
                SetLinkTimeInterval(LINK_INTERVAL_NONE);
                sPokemonJump->mainState++;
            }
            break;
        case 2:
            if (AreLinkQueuesEmpty())
            {
                CreateTask(Task_LinkSave, 6);
                sPokemonJump->mainState++;
            }
            break;
        case 3:
            if (!FuncIsActiveTask(Task_LinkSave))
            {
                ClearMessageWindow();
                sPokemonJump->mainState++;
            }
            break;
        case 4:
            if (!RemoveMessageWindow())
            {
                sPokemonJump->nextFuncId = FUNC_ASK_PLAY_AGAIN;
                return FALSE;
            }
            break;
    }
    return TRUE;
}

///////////////////
// GFX FUNCTIONS //
///////////////////

static void SetUpPokeJumpGfxFuncById(u32 id)
{
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sPokeJumpGfxFuncs); i++)
    {
        if (sPokeJumpGfxFuncs[i].id == id)
        {
            SetWordTaskArg(sPokemonJumpGfx->taskId, 0, (uintptr_t)sPokeJumpGfxFuncs[i].func);
            sPokemonJumpGfx->mainState = 0;
            sPokemonJumpGfx->funcFinished = FALSE;
        }
    }
}

static void Task_RunPokeJumpGfxFunc(u32 taskId)
{
    if (!sPokemonJumpGfx->funcFinished)
    {
        // Read the function set in the data by SetUpPokeJumpGfxFuncById
        void (*func)(void) = (void *)(GetWordTaskArg(taskId, 0));
        func();
    }
}

static void StartPokeJumpGfx(struct PokemonJumpGfx *jumpGfx)
{
    sPokemonJumpGfx = jumpGfx;
    sPokemonJumpGfx->msgWindowId = 0xFF;
    sPokemonJumpGfx->taskId = CreateTask(Task_RunPokeJumpGfxFunc, 3);
    SetWordTaskArg(sPokemonJumpGfx->taskId, 2, (uintptr_t)sPokemonJumpGfx);
    SetUpPokeJumpGfxFuncById(GFXFUNC_LOAD);
}

static bool32 IsPokeJumpGfxFuncFinished(void)
{
    return (sPokemonJumpGfx->funcFinished != TRUE);
}

static void LoadSpriteSheetsAndPalettes(struct PokemonJumpGfx *jumpGfx)
{
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sCompressedSpriteSheets); i++)
        LoadCompressedSpriteSheet(&sCompressedSpriteSheets[i]);

    for (i = 0; i < ARRAY_COUNT(sSpritePalettes); i++)
        LoadSpritePalette(&sSpritePalettes[i]);

    jumpGfx->vinePalNumDownswing = IndexOfSpritePaletteTag(PALTAG_1);
    jumpGfx->vinePalNumUpswing = IndexOfSpritePaletteTag(PALTAG_2);
}

static void InitDigitPrinters(void)
{
    struct DigitObjUtilTemplate template = {
        .shape = SPRITE_SHAPE(8x8),
        .size = SPRITE_SIZE(8x8),
        .strConvMode = 0,
        .priority = 1,
        .oamCount = 5,
        .xDelta = 8,
        .x = 108,
        .y = 6,
        .spriteSheet = {.compressed = &sSpriteSheet_Digits},
        .spritePal = &sSpritePalette_Digits,
    };

    DigitObjUtil_Init(NUM_WINDOWS);
    DigitObjUtil_CreatePrinter(WIN_POINTS, 0, &template);

    template.oamCount = 4;
    template.x = 30;
    template.y = 6;
    DigitObjUtil_CreatePrinter(WIN_TIMES, 0, &template);
}

static void LoadPokeJumpGfx(void)
{
    switch (sPokemonJumpGfx->mainState)
    {
        case 0:
            ResetBgsAndClearDma3BusyFlags(FALSE);
            InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
            InitWindows(sWindowTemplates);
            ResetAllBgsPos();
            ResetTempTileDataBuffers();
            LoadSpriteSheetsAndPalettes(sPokemonJumpGfx);
            InitDigitPrinters();
            LoadPalette(sBg_Pal, 0, sizeof(sBg_Pal));
            DecompressAndCopyTileDataToVram(BG_SCENERY, sBg_Gfx, 0, 0, 0);
            DecompressAndCopyTileDataToVram(BG_SCENERY, sBg_Tilemap, 0, 0, 1);
            LoadPalette(sVenusaur_Pal, 0x30, sizeof(sVenusaur_Pal));
            DecompressAndCopyTileDataToVram(BG_VENUSAUR, sVenusaur_Gfx, 0, 0, 0);
            DecompressAndCopyTileDataToVram(BG_VENUSAUR, sVenusaur_Tilemap, 0, 0, 1);
            LoadPalette(sBonuses_Pal, 0x10, sizeof(sBonuses_Pal));
            DecompressAndCopyTileDataToVram(BG_BONUSES, sBonuses_Gfx, 0, 0, 0);
            DecompressAndCopyTileDataToVram(BG_BONUSES, sBonuses_Tilemap, 0, 0, 1);
            LoadPalette(sInterface_Pal, 0x20, sizeof(sInterface_Pal));
            SetBgTilemapBuffer(BG_INTERFACE, sPokemonJumpGfx->tilemapBuffer);
            FillBgTilemapBufferRect_Palette0(BG_INTERFACE, 0, 0, 0, 0x20, 0x20);
            PrintScoreSuffixes();
            PrintScore(0);
            DrawWindowBorderWithStdpal3(0, 1, 0xE0);
            LoadUserWindowBorderGfx(0, 0x00A, 0xD0);
            CopyBgTilemapBufferToVram(BG_INTERFACE);
            CopyBgTilemapBufferToVram(BG_VENUSAUR);
            CopyBgTilemapBufferToVram(BG_BONUSES);
            sPokemonJumpGfx->mainState++;
            break;
        case 1:
            if (!FreeTempTileDataBuffersIfPossible())
            {
                CreateJumpMonSprites();
                CreateVineSprites(sPokemonJumpGfx);
                UpdateVineAnim(sPokemonJumpGfx, VINE_UPSWING_LOWER);
                ShowBg(BG_SCENERY);
                ShowBg(BG_INTERFACE);
                ShowBg(BG_VENUSAUR);
                HideBg(BG_BONUSES);
                sPokemonJumpGfx->mainState++;
            }
            break;
        case 2:
            sPokemonJumpGfx->funcFinished = TRUE;
            break;
    }
}

static void PrintPlayerNamesNoHighlight(void)
{
    switch (sPokemonJumpGfx->mainState)
    {
        case 0:
            AddPlayerNameWindows();
            sPokemonJumpGfx->mainState++;
            break;
        case 1:
            if (!IsDma3ManagerBusyWithBgCopy())
            {
                PrintPokeJumpPlayerNames(FALSE);
                sPokemonJumpGfx->mainState++;
            }
            break;
        case 2:
            if (!IsDma3ManagerBusyWithBgCopy())
            {
                DrawPlayerNameWindows();
                sPokemonJumpGfx->mainState++;
            }
            break;
        case 3:
            if (!IsDma3ManagerBusyWithBgCopy())
                sPokemonJumpGfx->funcFinished = TRUE;
            break;
    }
}

static void PrintPlayerNamesWithHighlight(void)
{
    switch (sPokemonJumpGfx->mainState)
    {
        case 0:
            AddPlayerNameWindows();
            sPokemonJumpGfx->mainState++;
            break;
        case 1:
            if (!IsDma3ManagerBusyWithBgCopy())
            {
                PrintPokeJumpPlayerNames(TRUE);
                sPokemonJumpGfx->mainState++;
            }
            break;
        case 2:
            if (!IsDma3ManagerBusyWithBgCopy())
            {
                DrawPlayerNameWindows();
                sPokemonJumpGfx->mainState++;
            }
            break;
        case 3:
            if (!IsDma3ManagerBusyWithBgCopy())
                sPokemonJumpGfx->funcFinished = TRUE;
            break;
    }
}

static void Msg_WantToPlayAgain(void)
{
    switch (sPokemonJumpGfx->mainState)
    {
        case 0:
            sPokemonJumpGfx->msgWindowId = AddMessageWindow(1, 8, 20, 2);
            AddTextPrinterParameterized(sPokemonJumpGfx->msgWindowId, FONT_NORMAL, gText_WantToPlayAgain, 0, 2, 0xFF, NULL);
            CopyWindowToVram(sPokemonJumpGfx->msgWindowId, COPYWIN_GFX);
            sPokemonJumpGfx->mainState++;
            break;
        case 1:
            if (!IsDma3ManagerBusyWithBgCopy())
            {
                PutWindowTilemap(sPokemonJumpGfx->msgWindowId);
                DrawTextBorderOuter(sPokemonJumpGfx->msgWindowId, 1, 14);
                CreatePokeJumpYesNoMenu(23, 7, 0);
                CopyBgTilemapBufferToVram(BG_INTERFACE);
                sPokemonJumpGfx->mainState++;
            }
            break;
        case 2:
            if (!IsDma3ManagerBusyWithBgCopy())
                sPokemonJumpGfx->funcFinished = TRUE;
            break;
    }
}

static void Msg_SavingDontTurnOff(void)
{
    switch (sPokemonJumpGfx->mainState)
    {
        case 0:
            sPokemonJumpGfx->msgWindowId = AddMessageWindow(2, 7, 26, 4);
            AddTextPrinterParameterized(sPokemonJumpGfx->msgWindowId, FONT_NORMAL, gText_SavingDontTurnOffThePower, 0, 2, 0xFF, NULL);
            CopyWindowToVram(sPokemonJumpGfx->msgWindowId, COPYWIN_GFX);
            sPokemonJumpGfx->mainState++;
            break;
        case 1:
            if (!IsDma3ManagerBusyWithBgCopy())
            {
                PutWindowTilemap(sPokemonJumpGfx->msgWindowId);
                DrawTextBorderOuter(sPokemonJumpGfx->msgWindowId, 1, 14);
                CopyBgTilemapBufferToVram(BG_INTERFACE);
                sPokemonJumpGfx->mainState++;
            }
            break;
        case 2:
            if (!IsDma3ManagerBusyWithBgCopy())
                sPokemonJumpGfx->funcFinished = TRUE;
            break;
    }
}

static void EraseMessage(void)
{
    switch (sPokemonJumpGfx->mainState)
    {
        case 0:
            ClearMessageWindow();
            DestroyYesNoMenu();
            CopyBgTilemapBufferToVram(BG_INTERFACE);
            sPokemonJumpGfx->mainState++;
            break;
        case 1:
            if (!RemoveMessageWindow() && !IsDma3ManagerBusyWithBgCopy())
                sPokemonJumpGfx->funcFinished = TRUE;
            break;
    }
}

static void Msg_SomeoneDroppedOut(void)
{
    switch (sPokemonJumpGfx->mainState)
    {
        case 0:
            sPokemonJumpGfx->msgWindowId = AddMessageWindow(2, 8, 22, 4);
            AddTextPrinterParameterized(sPokemonJumpGfx->msgWindowId, FONT_NORMAL, gText_SomeoneDroppedOut, 0, 2, 0xFF, NULL);
            CopyWindowToVram(sPokemonJumpGfx->msgWindowId, COPYWIN_GFX);
            sPokemonJumpGfx->mainState++;
            break;
        case 1:
            if (!IsDma3ManagerBusyWithBgCopy())
            {
                PutWindowTilemap(sPokemonJumpGfx->msgWindowId);
                DrawTextBorderOuter(sPokemonJumpGfx->msgWindowId, 1, 14);
                CopyBgTilemapBufferToVram(BG_INTERFACE);
                sPokemonJumpGfx->mainState++;
            }
            break;
        case 2:
            if (!IsDma3ManagerBusyWithBgCopy())
                sPokemonJumpGfx->funcFinished = TRUE;
            break;
    }
}

static void StartPokeJumpCountdown(void)
{
    StartMinigameCountdown(GFXTAG_COUNTDOWN, PALTAG_COUNTDOWN, 120, 80, 0);
    ResetMonSpriteSubpriorities();
}

static void DoPokeJumpCountdown(void)
{
    switch (sPokemonJumpGfx->mainState)
    {
        case 0:
            StartPokeJumpCountdown();
            sPokemonJumpGfx->mainState++;
            break;
        case 1:
            if (!IsMinigameCountdownRunning())
                sPokemonJumpGfx->funcFinished = TRUE;
            break;
    }
}

static void Msg_CommunicationStandby(void)
{
    switch (sPokemonJumpGfx->mainState)
    {
        case 0:
            sPokemonJumpGfx->msgWindowId = AddMessageWindow(7, 10, 16, 2);
            AddTextPrinterParameterized(sPokemonJumpGfx->msgWindowId, FONT_NORMAL, gText_CommunicationStandby, 0, 2, 0xFF, NULL);
            CopyWindowToVram(sPokemonJumpGfx->msgWindowId, COPYWIN_GFX);
            sPokemonJumpGfx->mainState++;
            break;
        case 1:
            if (!IsDma3ManagerBusyWithBgCopy())
            {
                PutWindowTilemap(sPokemonJumpGfx->msgWindowId);
                DrawTextBorderOuter(sPokemonJumpGfx->msgWindowId, 1, 14);
                CopyBgTilemapBufferToVram(BG_INTERFACE);
                sPokemonJumpGfx->mainState++;
            }
            break;
        case 2:
            if (!IsDma3ManagerBusyWithBgCopy())
                sPokemonJumpGfx->funcFinished = TRUE;
            break;
    }
}

////////////////////
// VINE FUNCTIONS //
////////////////////

static void CreateVineSprites(struct PokemonJumpGfx *jumpGfx)
{
    s32 i;
    u32 spriteId, count = 0;

    for (i = 0; i < VINE_SPRITES_PER_SIDE; i++)
    {
        spriteId = CreateSprite(sSpriteTemplates_Vine[i], sVineXCoords[count], sVineYCoords[i][0], 2);
        jumpGfx->vineSprites[count] = &gSprites[spriteId];
        count++;
    }

    for (i = VINE_SPRITES_PER_SIDE - 1; i >= 0; i--)
    {
        spriteId = CreateSprite(sSpriteTemplates_Vine[i], sVineXCoords[count], sVineYCoords[i][0], 2);
        jumpGfx->vineSprites[count] = &gSprites[spriteId];
        jumpGfx->vineSprites[count]->hFlip = TRUE;
        count++;
    }
}

static void UpdateVineAnim(struct PokemonJumpGfx *jumpGfx, u32 vineState)
{
    s32 i;
    u32 count, palNum, priority;

    if (vineState > VINE_LOWEST)
    {
        // animNums for vine on upswing are same as on downswing but in reverse
        vineState = NUM_VINESTATES - vineState;
        priority = 3; // Set vine behind Pokémon
        palNum = jumpGfx->vinePalNumUpswing;
    }
    else
    {
        priority = 2; // Set vine in front of Pokémon
        palNum = jumpGfx->vinePalNumDownswing;
    }

    for (i = 0, count = 0; i < VINE_SPRITES_PER_SIDE; i++)
    {
        jumpGfx->vineSprites[count]->y = sVineYCoords[i][vineState];
        jumpGfx->vineSprites[count]->oam.priority = priority;
        jumpGfx->vineSprites[count]->oam.paletteNum = palNum;
        StartSpriteAnim(jumpGfx->vineSprites[count], vineState);
        count++;
    }

    for (i = VINE_SPRITES_PER_SIDE - 1; i >= 0; i--)
    {
        jumpGfx->vineSprites[count]->y = sVineYCoords[i][vineState];
        jumpGfx->vineSprites[count]->oam.priority = priority;
        jumpGfx->vineSprites[count]->oam.paletteNum = palNum;
        StartSpriteAnim(jumpGfx->vineSprites[count], vineState);
        count++;
    }
}

static void UpdateVineSpeed(void)
{
    u32 baseSpeed;

    sPokemonJump->vineSpeedAccel = 0;
    
    if (sPokemonJump->vineSpeedDelay)
    {
        sPokemonJump->vineSpeedDelay--;
        
        if (sPokemonJump->atMaxSpeedStage)
        {
            if (PokeJumpRandom() % 4)
                sPokemonJump->vineSpeed = sPokemonJump->nextVineSpeed;
            else
            {
                if (sPokemonJump->nextVineSpeed > 54)
                    sPokemonJump->vineSpeed = 30;
                else
                    sPokemonJump->vineSpeed = 82;
            }
        }
    }
    else
    {
        if (!(sPokemonJump->vineBaseSpeedIdx & ARRAY_COUNT(sVineBaseSpeeds)))
        {
            sPokemonJump->nextVineSpeed = sVineBaseSpeeds[sPokemonJump->vineBaseSpeedIdx] + (sPokemonJump->vineSpeedStage * 7);
            sPokemonJump->vineSpeedDelay = sVineSpeedDelays[PokeJumpRandom() % ARRAY_COUNT(sVineSpeedDelays)] + 2;
            sPokemonJump->vineBaseSpeedIdx++;
        }
        else
        {
            if (sPokemonJump->vineBaseSpeedIdx == ARRAY_COUNT(sVineBaseSpeeds))
            {
                if (sPokemonJump->vineSpeedStage < 3)
                    sPokemonJump->vineSpeedStage++;
                else
                    sPokemonJump->atMaxSpeedStage = TRUE;
            }

            baseSpeed = sVineBaseSpeeds[15 - sPokemonJump->vineBaseSpeedIdx];
            sPokemonJump->nextVineSpeed = baseSpeed + (sPokemonJump->vineSpeedStage * 7);
            if (++sPokemonJump->vineBaseSpeedIdx > 15)
            {
                if (PokeJumpRandom() % 4 == 0)
                    sPokemonJump->nextVineSpeed -= 5;

                sPokemonJump->vineBaseSpeedIdx = 0;
            }
        }
        sPokemonJump->vineSpeed = sPokemonJump->nextVineSpeed;
    }
}

static void UpdateVineSwing(u32 vineState)
{
    UpdateVineAnim(sPokemonJumpGfx, vineState);
    ChangeBgY(BG_VENUSAUR, (sVenusaurStates[vineState] * 5) << 13, 0);
}

static void TryUpdateVineSwing(void)
{
    if (sPokemonJump->allowVineUpdates)
        UpdateVineSwing(sPokemonJump->vineState);
}

static void SetUpResetVineGfx(void)
{
    sPokemonJumpGfx->resetVineState = 0;
    sPokemonJumpGfx->resetVineTimer = 0;
    sPokemonJumpGfx->vineState = VINE_UPSWING_LOWER;
    UpdateVineSwing(sPokemonJumpGfx->vineState);
}

static void InitVineState(void)
{
    sPokemonJump->vineTimer = 0;
    sPokemonJump->vineState = VINE_UPSWING_LOWER;
    sPokemonJump->vineStateTimer = 0;
    sPokemonJump->vineSpeed = 0;
    sPokemonJump->ignoreJumpInput = FALSE;
    sPokemonJump->gameOver = FALSE;
}

static int GetVineSpeed(void)
{
    if (sPokemonJump->gameOver)
        return 0;
    else
    {
        int speed = sPokemonJump->vineSpeed;
        
        if (sPokemonJump->vineStateTimer <= VINE_STATE_TIMER(VINE_LOWEST))
        {
            // If at or below lowest, then vine is in downswing
            // Increase speed in downswing
            sPokemonJump->vineSpeedAccel += 80;
            speed += sPokemonJump->vineSpeedAccel / 256;
        }
        return speed;
    }
}

static void UpdateVineState(void)
{
    if (sPokemonJump->allowVineUpdates)
    {
        sPokemonJump->vineTimer++;
        sPokemonJump->vineStateTimer += GetVineSpeed();
        if (sPokemonJump->vineStateTimer >= VINE_STATE_TIMER(NUM_VINESTATES - 1))
            sPokemonJump->vineStateTimer -= VINE_STATE_TIMER(NUM_VINESTATES - 1);

        sPokemonJump->prevVineState = sPokemonJump->vineState;
        sPokemonJump->vineState = sPokemonJump->vineStateTimer >> 8;

         // If beginning upswing
        if (sPokemonJump->vineState > VINE_UPSWING_LOWER && sPokemonJump->prevVineState < VINE_UPSWING_LOW)
        {
            sPokemonJump->ignoreJumpInput++;
            UpdateVineSpeed();
        }
    }
}

static void ResetVineState(void)
{
    sPokemonJump->vineTimer = 0;
    sPokemonJump->vineStateTimer = VINE_STATE_TIMER(VINE_UPSWING_LOWER);
    sPokemonJump->vineState = VINE_UPSWING_LOW;
    sPokemonJump->ignoreJumpInput = FALSE;
    sPokemonJump->gameOver = FALSE;
    sPokemonJump->vineSpeedStage = 0;
    sPokemonJump->vineBaseSpeedIdx = 0;
    sPokemonJump->vineSpeedAccel = 0;
    sPokemonJump->vineSpeedDelay = 0;
    sPokemonJump->atMaxSpeedStage = FALSE;
    UpdateVineSpeed();
}

static bool32 DidAllPlayersClearVine(void)
{
    u32 i;
    
    for (i = 0; i < sPokemonJump->numPlayers; i++)
    {
        if (sPokemonJump->players[i].jumpState != JUMPSTATE_SUCCESS)
            return FALSE;
    }
    return TRUE;
}

// Has everyone either jumped or been hit by the vine
static bool32 AllPlayersJumpedOrHit(void)
{
    u32 i, numJumpedOrHit = 0, numPlayers = sPokemonJump->numPlayers;
    
    for (i = 0; i < numPlayers; i++)
    {
        if (sPokemonJump->players[i].jumpState != JUMPSTATE_NONE)
            numJumpedOrHit++;
    }
    return (numJumpedOrHit == numPlayers);
}

static void SetMonStateHit(void)
{
    sPokemonJump->player->prevMonState = sPokemonJump->player->monState;
    sPokemonJump->player->monState = MONSTATE_HIT;
    sPokemonJump->player->jumpTimeStart = sPokemonJump->vineTimer;
    sPokemonJump->player->jumpState = JUMPSTATE_FAILURE;
}

// Returns FALSE if any player was hit by vine
static bool32 UpdateVineHitStates(void)
{
    u32 i;

    if (sPokemonJump->vineState == VINE_UPSWING_LOWER && sPokemonJump->player->jumpOffset == 0)
    {
        // Vine is in position to hit the player and jump offset is 0.
        // Unless the player had just jumped and has been forced to the ground
        // by someone else getting hit, the player has been hit
        if (sPokemonJump->player->prevMonState == MONSTATE_JUMP && sPokemonJump->gameOver)
            sPokemonJump->player->jumpState = JUMPSTATE_SUCCESS;
        else
        {
            // Hit vine
            SetMonStateHit();
            SetLinkTimeInterval(LINK_INTERVAL_SHORT);
        }
    }

    if (sPokemonJump->vineState == VINE_UPSWING_LOW && sPokemonJump->prevVineState == VINE_UPSWING_LOWER && sPokemonJump->player->monState != MONSTATE_HIT)
    {
        sPokemonJump->player->jumpState = JUMPSTATE_SUCCESS;
        SetLinkTimeInterval(LINK_INTERVAL_SHORT);
    }

    for (i = 0; i < sPokemonJump->numPlayers; i++)
    {
        if (sPokemonJump->players[i].monState == MONSTATE_HIT)
            return FALSE;
    }
    return TRUE;
}

static bool32 IsPlayersMonState(u32 monState)
{
    if (sPokemonJump->players[sPokemonJump->multiplayerId].monState == monState)
        return TRUE;
    else
        return FALSE;
}

static void SetMonStateJump(void)
{
    sPokemonJump->player->jumpTimeStart = sPokemonJump->vineTimer;
    sPokemonJump->player->prevMonState = sPokemonJump->player->monState;
    sPokemonJump->player->monState = MONSTATE_JUMP;
}

// Update the vine and wait for player to input a jump
// Returns false when vine reaches the 'hit' point, after
// which input is ignored
static bool32 HandleSwingRound(void)
{
    UpdateVineState();
    
    if (sPokemonJump->ignoreJumpInput)
    {
        sPokemonJump->ignoreJumpInput = FALSE;
        return FALSE;
    }

    switch (sPokemonJump->helperState)
    {
        case 0:
            if (IsPlayersMonState(MONSTATE_NORMAL))
                sPokemonJump->helperState++;
            else
                break;
            // fall through
        case 1:
            if (JOY_NEW(A_BUTTON))
            {
                SetMonStateJump();
                SetLinkTimeInterval(LINK_INTERVAL_SHORT);
                sPokemonJump->helperState++;
            }
            break;
        case 2:
            if (IsPlayersMonState(MONSTATE_JUMP))
                sPokemonJump->helperState++;
            break;
        case 3:
            if (IsPlayersMonState(MONSTATE_NORMAL))
                sPokemonJump->helperState = 0;
            break;
    }
    return TRUE;
}

static void ResetVineAfterHit(void)
{
    sPokemonJump->gameOver = TRUE;
    sPokemonJump->vineState = VINE_UPSWING_LOWER;
    sPokemonJump->vineStateTimer = VINE_STATE_TIMER(VINE_LOWEST);
    sPokemonJump->allowVineUpdates = TRUE;
}

static void ResetPlayersMonState(void)
{
    sPokemonJump->player->monState = MONSTATE_NORMAL;
    sPokemonJump->player->prevMonState = MONSTATE_NORMAL;
}

static bool32 DoVineHitEffect(void)
{
    u32 i;

    switch (sPokemonJump->helperState)
    {
        case 0:
            for (i = 0; i < sPokemonJump->numPlayers; i++)
            {
                if (IsMonHitShakeActive(i))
                    return TRUE;
            }
            sPokemonJump->helperState++;
            break;
        case 1:
            for (i = 0; i < sPokemonJump->numPlayers; i++)
            {
                if (sPokemonJump->players[i].monState == MONSTATE_HIT)
                    StartMonHitFlash(i);
            }
            SetUpPokeJumpGfxFuncById(GFXFUNC_SHOW_NAMES);
            sPokemonJump->timer = 0;
            sPokemonJump->helperState++;
            break;
        case 2:
            if (++sPokemonJump->timer > 100)
            {
                SetUpPokeJumpGfxFuncById(GFXFUNC_ERASE_NAMES);
                sPokemonJump->timer = 0;
                sPokemonJump->helperState++;
            }
            break;
        case 3:
            if (!IsPokeJumpGfxFuncFinished())
            {
                StopMonHitFlash();
                sPokemonJump->comm.receivedBonusFlags = 0;
                ResetPlayersMonState();
                sPokemonJump->helperState++;
                return FALSE;
            }
            break;
        case 4:
            return FALSE;
    }
    return TRUE;
}

static u32 GetPlayersAtJumpPeak(void)
{
    u32 i, numAtPeak = 0;

    for (i = 0; i < sPokemonJump->numPlayers; i++)
    {
        if (sPokemonJump->players[i].jumpOffset == JUMP_PEAK)
        {
            sPokemonJump->members[i].atJumpPeak = TRUE;
            numAtPeak++;
        }
        else
            sPokemonJump->members[i].atJumpPeak = FALSE;
    }
    return numAtPeak;
}

/////////////
// SPRITES //
/////////////

static void ResetPokeJumpSpriteData(struct Sprite *sprite)
{
    u32 i;
    
    for (i = 0; i < ARRAY_COUNT(sprite->data); i++)
        sprite->data[i] = 0;
}

static void CreateStarSprite(struct PokemonJumpGfx *jumpGfx, s16 x, s16 y, u32 multiplayerId)
{
    u32 spriteId = CreateSprite(&sSpriteTemplate_Star, x, y, 1);
    if (spriteId != MAX_SPRITES)
    {
        gSprites[spriteId].invisible = TRUE;
        jumpGfx->starSprites[multiplayerId] = &gSprites[spriteId];
    }
}

static void DoStarAnim(struct PokemonJumpGfx *jumpGfx, u32 multiplayerId)
{
    ResetPokeJumpSpriteData(jumpGfx->starSprites[multiplayerId]);
    jumpGfx->starSprites[multiplayerId]->invisible = FALSE;
    jumpGfx->starSprites[multiplayerId]->y = 96;
    jumpGfx->starSprites[multiplayerId]->callback = SpriteCB_Star;
    StartSpriteAnim(jumpGfx->starSprites[multiplayerId], 1);
}

#define sState data[0]
#define sTimer data[1]

static void SpriteCB_Star(struct Sprite *sprite)
{
    switch (sprite->sState)
    {
    case 0:
        if (sprite->animEnded)
        {
            sprite->invisible = TRUE;
            sprite->callback = SpriteCallbackDummy;
        }
        break;
    case 1:
        sprite->y--;
        sprite->sTimer++;
        
        if (sprite->y <= 72)
        {
            sprite->y = 72;
            sprite->sState++;
        }
        break;
    case 2:
        if (++sprite->sTimer >= 48)
        {
            sprite->invisible = TRUE;
            sprite->callback = SpriteCallbackDummy;
        }
        break;
    }
}

#undef sState
#undef sTimer

static void CreateJumpMonSprite(struct PokemonJumpGfx *jumpGfx, struct PokemonJump_MonInfo *monInfo, s16 x, s16 y, u32 multiplayerId)
{
    struct SpriteTemplate spriteTemplate = sSpriteTemplate_JumpMon;
    struct SpriteSheet spriteSheet;
    struct CompressedSpritePalette spritePalette;
    u8 *buffer;
    u32 spriteId, subpriority;
    
    buffer = Alloc(0x2000);
    if (buffer)
    {
        LoadSpecialPokePic(monInfo->species, monInfo->personality, TRUE, buffer);

        spriteSheet.data = buffer;
        spriteSheet.tag = multiplayerId;
        spriteSheet.size = 0x800;
        LoadSpriteSheet(&spriteSheet);

        spritePalette.data = GetMonSpritePalFromSpecies(monInfo->species, FALSE);
        spritePalette.tag = multiplayerId;
        LoadCompressedSpritePalette(&spritePalette);

        Free(buffer);
        
        if (multiplayerId == sPokemonJump->multiplayerId)
            subpriority = 3;
        else
            subpriority = multiplayerId + 4;
        
        spriteTemplate.tileTag += multiplayerId;
        spriteTemplate.paletteTag += multiplayerId;
        spriteId = CreateSprite(&spriteTemplate, x, y, subpriority);
        if (spriteId != MAX_SPRITES)
        {
            jumpGfx->monSprites[multiplayerId] = &gSprites[spriteId];
            jumpGfx->monSpriteSubpriorities[multiplayerId] = subpriority;
            return;
        }
    }
    jumpGfx->monSprites[multiplayerId] = NULL;
}

static void CreateJumpMonSprites(void)
{
    u32 i, playersCount = GetLinkPlayerCount();
    const u8 *xCoords = sMonXCoords[playersCount - 2];

    for (i = 0; i < playersCount; i++)
    {
        struct PokemonJump_MonInfo *info = &sPokemonJump->members[i].monInfo;

        CreateJumpMonSprite(sPokemonJumpGfx, info, *xCoords, gSpeciesInfo[info->species].frontPicYOffset + 112, i);
        CreateStarSprite(sPokemonJumpGfx, *xCoords, 112, i);
        xCoords++;
    }
}

static void SetMonSpriteY(u32 id, s16 y)
{
    sPokemonJumpGfx->monSprites[id]->y2 = y;
}

static void SetMonStateNormal(void)
{
    sPokemonJump->player->prevMonState = sPokemonJump->player->monState;
    sPokemonJump->player->monState = MONSTATE_NORMAL;
}

static void UpdateJump(u32 multiplayerId)
{
    int jumpOffsetIdx;
    int jumpOffset;
    struct PokemonJump_Player *player = &sPokemonJump->players[multiplayerId];

    if (player->jumpOffsetIdx != INT_MAX)
    {
        player->jumpOffsetIdx++;
        jumpOffsetIdx = player->jumpOffsetIdx;
    }
    else
    {
        jumpOffsetIdx = sPokemonJump->vineTimer - player->jumpTimeStart;
        if (jumpOffsetIdx >= 65000)
        {
            jumpOffsetIdx -= 65000;
            jumpOffsetIdx += sPokemonJump->vineTimer;
        }
        player->jumpOffsetIdx = jumpOffsetIdx;
    }

    if (jumpOffsetIdx < 4)
        return;

    jumpOffsetIdx -= 4;
    if (jumpOffsetIdx < (int)ARRAY_COUNT(sJumpOffsets[0]))
        jumpOffset = sJumpOffsets[player->monJumpType][jumpOffsetIdx];
    else
        jumpOffset = 0;

    SetMonSpriteY(multiplayerId, jumpOffset);
    if (jumpOffset == 0 && multiplayerId == sPokemonJump->multiplayerId)
        SetMonStateNormal();

    player->jumpOffset = jumpOffset;
}

#define F_SE_JUMP Bit(0)
#define F_SE_FAIL Bit(1)

static void HandleMonState(void)
{
    u32 i, soundFlags = 0;

    for (i = 0; i < sPokemonJump->numPlayers; i++)
    {
        switch (sPokemonJump->players[i].monState)
        {
            case MONSTATE_NORMAL:
                SetMonSpriteY(i, 0);
                break;
            case MONSTATE_JUMP:
                if (sPokemonJump->players[i].prevMonState != MONSTATE_JUMP || sPokemonJump->players[i].jumpTimeStart != sPokemonJump->members[i].jumpTimeStart)
                {
                    // This is a new jump, play SE and init fields for jump handling
                    if (i == sPokemonJump->multiplayerId)
                        sPokemonJump->players[i].prevMonState = MONSTATE_JUMP;
    
                    soundFlags |= F_SE_JUMP;
                    sPokemonJump->players[i].jumpOffsetIdx = INT_MAX;
                    sPokemonJump->members[i].jumpTimeStart = sPokemonJump->players[i].jumpTimeStart;
                }
                UpdateJump(i);
                break;
            case MONSTATE_HIT:
                if (sPokemonJump->players[i].prevMonState != MONSTATE_HIT)
                {
                    if (i == sPokemonJump->multiplayerId)
                        sPokemonJump->players[i].prevMonState = MONSTATE_HIT;
    
                    soundFlags |= F_SE_FAIL;
                    StartMonHitShake(i);
                }
                break;
        }
    }

    if (soundFlags & F_SE_FAIL)
        PlaySE(SE_POKE_JUMP_FAILURE);
    else if (soundFlags & F_SE_JUMP)
        PlaySE(SE_LEDGE);
}

static void ResetMonSpriteSubpriorities(void)
{
    u32 i, numPlayers = GetLinkPlayerCount();

    for (i = 0; i < numPlayers; i++)
        sPokemonJumpGfx->monSprites[i]->subpriority = sPokemonJumpGfx->monSpriteSubpriorities[i];
}

static void StartMonIntroBounce(u32 multiplayerId)
{
    ResetPokeJumpSpriteData(sPokemonJumpGfx->monSprites[multiplayerId]);
    sPokemonJumpGfx->monSprites[multiplayerId]->callback = SpriteCB_MonIntroBounce;
}

static bool32 IsMonIntroBounceActive(void)
{
    u32 i, numPlayers = GetLinkPlayerCount();

    for (i = 0; i < numPlayers; i++)
    {
        if (sPokemonJumpGfx->monSprites[i]->callback == SpriteCB_MonIntroBounce)
            return TRUE;
    }
    return FALSE;
}

#define sState   data[0]
#define sHopPos  data[1]
#define sNumHops data[2]

static void SpriteCB_MonIntroBounce(struct Sprite *sprite)
{
    switch (sprite->sState)
    {
        case 0:
            PlaySE(SE_BIKE_HOP);
            sprite->sHopPos = 0;
            sprite->sState++;
            // fall through
        case 1:
            sprite->sHopPos += 4;
            if (sprite->sHopPos > 127)
                sprite->sHopPos = 0;
    
            sprite->y2 = -(gSineTable[sprite->sHopPos] >> 3);
            if (sprite->sHopPos == 0)
            {
                if (++sprite->sNumHops < 2)
                    sprite->sState = 0;
                else
                    sprite->callback = SpriteCallbackDummy;
            }
            break;
    }
}

#undef sState
#undef sHopPos
#undef sNumHops

static void StartMonHitShake(u32 multiplayerId)
{
    sPokemonJumpGfx->monSprites[multiplayerId]->callback = SpriteCB_MonHitShake;
    sPokemonJumpGfx->monSprites[multiplayerId]->y2 = 0;
    ResetPokeJumpSpriteData(sPokemonJumpGfx->monSprites[multiplayerId]);
}

static bool32 IsMonHitShakeActive(u32 multiplayerId)
{
    return (sPokemonJumpGfx->monSprites[multiplayerId]->callback == SpriteCB_MonHitShake);
}

#define sTimer     data[1]
#define sNumShakes data[2]

static void SpriteCB_MonHitShake(struct Sprite *sprite)
{
    if (++sprite->sTimer > 1)
    {
        if (++sprite->sNumShakes & 1)
            sprite->y2 = 2;
        else
            sprite->y2 = -2;

        sprite->sTimer = 0;
    }

    if (sprite->sNumShakes > 12)
    {
        sprite->y2 = 0;
        sprite->callback = SpriteCallbackDummy;
    }
}

#undef sTimer
#undef sNumShakes

static void StartMonHitFlash(u32 multiplayerId)
{
    ResetPokeJumpSpriteData(sPokemonJumpGfx->monSprites[multiplayerId]);
    sPokemonJumpGfx->monSprites[multiplayerId]->callback = SpriteCB_MonHitFlash;
}

static void StopMonHitFlash(void)
{
    u32 i, numPlayers = GetLinkPlayerCount();

    for (i = 0; i < numPlayers; i++)
    {
        if (sPokemonJumpGfx->monSprites[i]->callback == SpriteCB_MonHitFlash)
        {
            sPokemonJumpGfx->monSprites[i]->invisible = FALSE;
            sPokemonJumpGfx->monSprites[i]->callback = SpriteCallbackDummy;
            sPokemonJumpGfx->monSprites[i]->subpriority = 10;
        }
    }
}

#define sTimer data[0]

static void SpriteCB_MonHitFlash(struct Sprite *sprite)
{
    if (++sprite->sTimer > 3)
    {
        sprite->sTimer = 0;
        sprite->invisible ^= TRUE;
    }
}

#undef sTimer

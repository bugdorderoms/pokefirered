#include "global.h"
#include "gflib.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_ai_script_commands.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "battle_main.h"
#include "battle_message.h"
#include "battle_scripts.h"
#include "battle_setup.h"
#include "battle_string_ids.h"
#include "data.h"
#include "dns.h"
#include "dexnav.h"
#include "decompress.h"
#include "event_data.h"
#include "evolution_scene.h"
#include "graphics.h"
#include "item.h"
#include "link.h"
#include "link_rfu.h"
#include "load_save.h"
#include "m4a.h"
#include "party_menu.h"
#include "pokeball.h"
#include "pokedex.h"
#include "form_change.h"
#include "quest_log.h"
#include "random.h"
#include "roamer.h"
#include "wild_encounter.h"
#include "safari_zone.h"
#include "scanline_effect.h"
#include "task.h"
#include "trig.h"
#include "vs_seeker.h"
#include "util.h"
#include "trainer_slide.h"
#include "constants/abilities.h"
#include "constants/battle_move_effects.h"
#include "constants/battle_script_commands.h"
#include "constants/battle_setup.h"
#include "constants/form_change.h"
#include "constants/hold_effects.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/songs.h"

static void HandleAction_UseMove(void);
static void HandleAction_Switch(void);
static void HandleAction_UseItem(void);
static void HandleAction_Run(void);
static void HandleAction_WatchesCarefully(void);
static void HandleAction_SafariZoneBallThrow(void);
static void HandleAction_ThrowBait(void);
static void HandleAction_ThrowRock(void);
static void HandleAction_SafariZoneRun(void);
static void HandleAction_OldManBallThrow(void);
static void HandleAction_TryFinish(void);
static void HandleAction_NothingIsFainted(void);
static void HandleAction_ActionFinished(void);
static void HandleEndTurn_ContinueBattle(void);
static void HandleEndTurn_BattleWon(void);
static void HandleEndTurn_BattleLost(void);
static void HandleEndTurn_RanFromBattle(void);
static void HandleEndTurn_MonFled(void);
static void HandleEndTurn_FinishBattle(void);
static void CB2_InitBattleInternal(void);
static void CB2_PreInitMultiBattle(void);
static void CB2_HandleStartMultiBattle(void);
static u8 CreateNPCTrainerParty(struct Pokemon *party, u16 trainerNum);
static void CB2_HandleStartBattle(void);
static void BattleMainCB1(void);
static void CB2_QuitPokedudeBattle(void);
static void CB2_EndLinkBattle(void);
static void EndLinkBattleInSteps(void);
static void SpriteCB_MoveWildMonToRight(struct Sprite *sprite);
static void SpriteCB_WildMonShowHealthbox(struct Sprite *sprite);
static void SpriteCB_AnimFaintOpponent(struct Sprite *sprite);
static void SpriteCb_BlinkVisible(struct Sprite *sprite);
static void oac_poke_ally_(struct Sprite *sprite);
static void SpriteCallbackDummy3(struct Sprite *sprite);
static void SpriteCB_BounceEffect(struct Sprite *sprite);
static void SpriteCB_PlayerThrowUpdate(struct Sprite *sprite);
static void BattleStartClearSetData(void);
static void BattleIntroGetMonsData(void);
static void TurnValuesCleanUp(bool8 var0);
static void BattleIntroPrepareBackgroundSlide(void);
static void BattleIntroDrawTrainersOrMonsSprites(void);
static void BattleIntroDrawPartySummaryScreens(void);
static void BattleIntroPrintTrainerWantsToBattle(void);
static void BattleIntroPrintWildMonAttacked(void);
static void BattleIntroPrintOpponentSendsOut(void);
static void BattleIntroPrintPlayerSendsOut(void);
static void BattleIntroRecordMonsToDex(void);
static void BattleIntroOpponentSendsOutMonAnimation(void);
static void BattleIntroPlayerSendsOutMonAnimation(void);
static void TryDoEventsBeforeFirstTurn(void);
static void HandleTurnActionSelectionState(void);
static void RunTurnActionsFunctions(void);
static void SetActionsAndBattlersTurnOrder(void);
static void CheckFocusPunch_ClearVarsBeforeTurnStarts(void);
static void HandleEndTurn_FinishBattle(void);
static void FreeResetData_ReturnToOvOrDoEvolutions(void);
static void ReturnFromBattleToOverworld(void);
static void TryEvolvePokemon(void);
static void TrySpecialEvolution(void);
static void WaitForEvoSceneToFinish(void);
static void SetTypeBeforeUsingMove(u16 move, u8 battler);

EWRAM_DATA u16 gBattle_BG0_X = 0;
EWRAM_DATA u16 gBattle_BG0_Y = 0;
EWRAM_DATA u16 gBattle_BG1_X = 0;
EWRAM_DATA u16 gBattle_BG1_Y = 0;
EWRAM_DATA u16 gBattle_BG2_X = 0;
EWRAM_DATA u16 gBattle_BG2_Y = 0;
EWRAM_DATA u16 gBattle_BG3_X = 0;
EWRAM_DATA u16 gBattle_BG3_Y = 0;
EWRAM_DATA u16 gBattle_WIN0H = 0;
EWRAM_DATA u16 gBattle_WIN0V = 0;
EWRAM_DATA u16 gBattle_WIN1H = 0;
EWRAM_DATA u16 gBattle_WIN1V = 0;
EWRAM_DATA u8 gDisplayedStringBattle[300] = {0};
EWRAM_DATA u8 gBattleTextBuff1[TEXT_BUFF_ARRAY_COUNT] = {0};
EWRAM_DATA u8 gBattleTextBuff2[TEXT_BUFF_ARRAY_COUNT] = {0};
EWRAM_DATA u8 gBattleTextBuff3[TEXT_BUFF_ARRAY_COUNT] = {0};
EWRAM_DATA u32 gBattleTypeFlags = 0;
EWRAM_DATA u8 gBattleTerrain = 0;
EWRAM_DATA struct MultiBattlePokemonTx gMultiPartnerParty[3] = {0};
EWRAM_DATA u8 *gBattleAnimMons_BgTilesBuffer = NULL;
EWRAM_DATA u8 *gBattleAnimMons_BgTilemapBuffer = NULL;
EWRAM_DATA u8 gBattleBufferA[MAX_BATTLERS_COUNT][0x200] = {0};
EWRAM_DATA u8 gBattleBufferB[MAX_BATTLERS_COUNT][0x200] = {0};
EWRAM_DATA u8 gActiveBattler = 0;
EWRAM_DATA u32 gBattleControllerExecFlags = 0;
EWRAM_DATA u8 gBattlersCount = 0;
EWRAM_DATA u16 gBattlerPartyIndexes[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gBattlerPositions[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gActionsByTurnOrder[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gBattlerByTurnOrder[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gCurrentTurnActionNumber = 0;
EWRAM_DATA u8 gCurrentActionFuncId = 0;
EWRAM_DATA struct BattlePokemon gBattleMons[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gBattlerSpriteIds[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gCurrMovePos = 0;
EWRAM_DATA u8 gChosenMovePos = 0;
EWRAM_DATA u16 gCurrentMove = 0;
EWRAM_DATA u16 gChosenMove = 0;
EWRAM_DATA u16 gCalledMove = 0;
EWRAM_DATA s32 gBattleMoveDamage = 0;
EWRAM_DATA s32 gHpDealt = 0;
EWRAM_DATA s32 gTakenDmg[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gLastUsedItem = 0;
EWRAM_DATA u16 gLastUsedAbility = 0;
EWRAM_DATA u8 gBattlerAttacker = 0;
EWRAM_DATA u8 gBattlerTarget = 0;
EWRAM_DATA u8 gBattlerFainted = 0;
EWRAM_DATA u8 gEffectBattler = 0;
EWRAM_DATA u8 gAbsentBattlerFlags = 0;
EWRAM_DATA bool8 gIsCriticalHit = 0;
EWRAM_DATA u8 gMultiHitCounter = 0;
EWRAM_DATA const u8 *gBattlescriptCurrInstr = NULL;
EWRAM_DATA u8 gChosenActionByBattler[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA const u8 *gSelectionBattleScripts[MAX_BATTLERS_COUNT] = {NULL};
EWRAM_DATA u16 gLastPrintedMoves[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gLastMoves[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gLastLandedMoves[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gLastHitByType[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gLastResultingMoves[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gLockedMoves[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gLastHitBy[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gChosenMoveByBattler[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gMoveResultFlags = 0;
EWRAM_DATA u32 gHitMarker = 0;
EWRAM_DATA u8 gTakenDmgByBattler[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gSideStatuses[B_SIDE_COUNT] = {0};
EWRAM_DATA struct SideTimer gSideTimers[B_SIDE_COUNT] = {0};
EWRAM_DATA u32 gStatuses3[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA struct DisableStruct gDisableStructs[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gPauseCounterBattle = 0;
EWRAM_DATA u16 gPaydayMoney = 0;
EWRAM_DATA u16 gRandomTurnNumber = 0;
EWRAM_DATA u8 gBattleCommunication[BATTLE_COMMUNICATION_ENTRIES_COUNT] = {0};
EWRAM_DATA u8 gBattleOutcome = 0;
EWRAM_DATA struct ProtectStruct gProtectStructs[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA struct SpecialStatus gSpecialStatuses[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gBattleWeather = 0;
EWRAM_DATA struct WishFutureKnock gWishFutureKnock = {0};
EWRAM_DATA u16 gIntroSlideFlags = 0;
EWRAM_DATA u8 gSentPokesToOpponent[2] = {0};
EWRAM_DATA u16 gExpShareExp = 0;
EWRAM_DATA struct BattleScripting gBattleScripting = {0};
EWRAM_DATA struct BattleStruct *gBattleStruct = NULL;
EWRAM_DATA u8 *gLinkBattleSendBuffer = NULL;
EWRAM_DATA u8 *gLinkBattleRecvBuffer = NULL;
EWRAM_DATA struct BattleResources *gBattleResources = NULL;
EWRAM_DATA u8 gActionSelectionCursor[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gMoveSelectionCursor[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gBattlerStatusSummaryTaskId[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gBattlerInMenuId = 0;
EWRAM_DATA bool8 gDoingBattleAnim = FALSE;
EWRAM_DATA u32 gTransformedPersonalities[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA struct BattleSpriteData *gBattleSpritesDataPtr = NULL;
EWRAM_DATA struct MonSpritesGfx *gMonSpritesGfxPtr = NULL;
EWRAM_DATA u16 gBattleMovePower = 0;
EWRAM_DATA u16 gMoveToLearn = 0;
EWRAM_DATA u16 gBattleMonForms[PARTY_SIZE][B_SIDE_COUNT] = {0};
EWRAM_DATA u8 gPartyCriticalHits[PARTY_SIZE] = {0};
EWRAM_DATA static u8 sTriedEvolving = 0;

void (*gPreBattleCallback1)(void);
void (*gBattleMainFunc)(void);
u8 gLeveledUpInBattle;
void (*gBattlerControllerFuncs[MAX_BATTLERS_COUNT])(void);
u8 gHealthboxSpriteIds[MAX_BATTLERS_COUNT];
u8 gMultiUsePlayerCursor;
u8 gNumberOfMovesToChoose;
u8 gBattleControllerData[MAX_BATTLERS_COUNT];

static const struct ScanlineEffectParams sIntroScanlineParams16Bit =
{
    &REG_BG3HOFS, SCANLINE_EFFECT_DMACNT_16BIT, 1
};

const struct OamData gOamData_BattlerOpponent =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
    .affineParam = 0,
};

const struct OamData gOamData_BattlerPlayer =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 2,
    .affineParam = 0,
};

static const s8 sPlayerThrowXTranslation[] = { -32, -16, -16, -32, -32, 0, 0, 0 };

// same format as bulbapedia type Effectiveness. See https://bulbapedia.bulbagarden.net/wiki/Type#Type_chart
// TYPE_MUL_SUPER_EFFECTIVE = ×2.0
// TYPE_MUL_NORMAL          = ×1.0
// TYPE_MUL_NOT_EFFECTIVE   = ×0.5
// TYPE_MUL_NO_EFFECT       = ×0.0
const u8 gTypeEffectiveness[NUMBER_OF_MON_TYPES][NUMBER_OF_MON_TYPES] =
{
    {TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NO_EFFECT, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL},
    {TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NO_EFFECT, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE},
    {TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL},
    {TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NO_EFFECT, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE},
    {TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NO_EFFECT, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL},
    {TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL},
    {TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE},
    {TYPE_MUL_NO_EFFECT, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL},
    {TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE},
    {TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL},
    {TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL},
    {TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL},
    {TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL},
    {TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NO_EFFECT, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL},
    {TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NO_EFFECT, TYPE_MUL_NORMAL},
    {TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL},
    {TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NO_EFFECT},
    {TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE},
	{TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NOT_EFFECTIVE, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_NORMAL, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL},
};

const u8 gTypeNames[NUMBER_OF_MON_TYPES][TYPE_NAME_LENGTH + 1] =
{
    [TYPE_NORMAL]   = _("Normal"),
    [TYPE_FIGHTING] = _("Fight"),
    [TYPE_FLYING]   = _("Flying"),
    [TYPE_POISON]   = _("Poison"),
    [TYPE_GROUND]   = _("Ground"),
    [TYPE_ROCK]     = _("Rock"),
    [TYPE_BUG]      = _("Bug"),
    [TYPE_GHOST]    = _("Ghost"),
    [TYPE_STEEL]    = _("Steel"),
    [TYPE_MYSTERY]  = _("???"),
    [TYPE_FIRE]     = _("Fire"),
    [TYPE_WATER]    = _("Water"),
    [TYPE_GRASS]    = _("Grass"),
    [TYPE_ELECTRIC] = _("Electr"),
    [TYPE_PSYCHIC]  = _("Psychc"),
    [TYPE_ICE]      = _("Ice"),
    [TYPE_DRAGON]   = _("Dragon"),
    [TYPE_DARK]     = _("Dark"),
	[TYPE_FAIRY]    = _("Fairy"),
};

const u8 gCategoryNames[NUM_MOVE_SPLITS][8] =
{
	[SPLIT_PHYSICAL] = _("Physic"),
	[SPLIT_SPECIAL]  = _("Special"),
	[SPLIT_STATUS]   = _("Status"),
};

// This is a factor in how much money you get for beating a trainer and the poke ball for it.
const struct TrainerMoneyBall gTrainerMoneyAndBallTable[] =
{
    { TRAINER_CLASS_LEADER, 25, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_ELITE_FOUR, 25, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_PKMN_PROF, 25, ITEM_POKE_BALL },
    { TRAINER_CLASS_RIVAL_EARLY, 4, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_RIVAL_LATE, 9, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_CHAMPION, 25, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_YOUNGSTER, 4, ITEM_POKE_BALL },
    { TRAINER_CLASS_BUG_CATCHER, 3, ITEM_NET_BALL },
    { TRAINER_CLASS_HIKER, 9, ITEM_GREAT_BALL },
    { TRAINER_CLASS_BIRD_KEEPER, 6, ITEM_GREAT_BALL },
    { TRAINER_CLASS_PICNICKER, 5, ITEM_POKE_BALL },
    { TRAINER_CLASS_SUPER_NERD, 6, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_FISHERMAN, 9, ITEM_NET_BALL },
    { TRAINER_CLASS_TEAM_ROCKET, 8, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_LASS, 4, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_BEAUTY, 18, ITEM_LUXURY_BALL },
    { TRAINER_CLASS_BLACK_BELT, 6, ITEM_GREAT_BALL },
    { TRAINER_CLASS_CUE_BALL, 6, ITEM_GREAT_BALL },
    { TRAINER_CLASS_CHANNELER, 8, ITEM_GREAT_BALL },
    { TRAINER_CLASS_ROCKER, 6, ITEM_GREAT_BALL },
    { TRAINER_CLASS_GENTLEMAN, 18, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_BURGLAR, 22, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_SWIMMER_M, 1, ITEM_DIVE_BALL },
    { TRAINER_CLASS_ENGINEER, 12, ITEM_GREAT_BALL },
    { TRAINER_CLASS_JUGGLER, 10, ITEM_POKE_BALL },
    { TRAINER_CLASS_SAILOR, 8, ITEM_NET_BALL },
    { TRAINER_CLASS_COOLTRAINER, 9, ITEM_POKE_BALL },
    { TRAINER_CLASS_POKEMANIAC, 12, ITEM_POKE_BALL },
    { TRAINER_CLASS_TAMER, 10, ITEM_POKE_BALL },
    { TRAINER_CLASS_CAMPER, 5, ITEM_POKE_BALL },
    { TRAINER_CLASS_PSYCHIC, 5, ITEM_GREAT_BALL },
    { TRAINER_CLASS_BIKER, 5, ITEM_TIMER_BALL },
    { TRAINER_CLASS_GAMER, 18, ITEM_POKE_BALL },
    { TRAINER_CLASS_SCIENTIST, 12, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_CRUSH_GIRL, 6, ITEM_GREAT_BALL },
    { TRAINER_CLASS_TUBER, 1, ITEM_NET_BALL },
    { TRAINER_CLASS_PKMN_BREEDER, 7, ITEM_POKE_BALL },
    { TRAINER_CLASS_PKMN_RANGER, 9, ITEM_SAFARI_BALL },
    { TRAINER_CLASS_AROMA_LADY, 7, ITEM_LUXURY_BALL },
    { TRAINER_CLASS_RUIN_MANIAC, 12, ITEM_PREMIER_BALL },
    { TRAINER_CLASS_LADY, 50, ITEM_LUXURY_BALL },
    { TRAINER_CLASS_PAINTER, 4, ITEM_GREAT_BALL },
    { TRAINER_CLASS_TWINS, 3, ITEM_POKE_BALL },
    { TRAINER_CLASS_YOUNG_COUPLE, 7, ITEM_POKE_BALL },
    { TRAINER_CLASS_SIS_AND_BRO, 1, ITEM_POKE_BALL },
    { TRAINER_CLASS_COOL_COUPLE, 6, ITEM_POKE_BALL },
    { TRAINER_CLASS_CRUSH_KIN, 6, ITEM_GREAT_BALL },
    { TRAINER_CLASS_SWIMMER_F, 1, ITEM_DIVE_BALL },
    { TRAINER_CLASS_PLAYER, 1, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_RS_LEADER, 25, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_RS_ELITE_FOUR, 25, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_RS_LASS, 4, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_RS_YOUNGSTER, 4, ITEM_POKE_BALL },
    { TRAINER_CLASS_PKMN_TRAINER, 15, ITEM_PREMIER_BALL },
    { TRAINER_CLASS_RS_HIKER, 10, ITEM_GREAT_BALL },
    { TRAINER_CLASS_RS_BEAUTY, 20, ITEM_LUXURY_BALL },
    { TRAINER_CLASS_RS_FISHERMAN, 10, ITEM_NET_BALL },
    { TRAINER_CLASS_RS_LADY, 50, ITEM_LUXURY_BALL },
    { TRAINER_CLASS_TRIATHLETE, 10, ITEM_GREAT_BALL },
    { TRAINER_CLASS_TEAM_AQUA, 5, ITEM_NET_BALL },
    { TRAINER_CLASS_RS_TWINS, 3, ITEM_POKE_BALL },
    { TRAINER_CLASS_RS_SWIMMER_F, 2, ITEM_DIVE_BALL },
    { TRAINER_CLASS_RS_BUG_CATCHER, 4, ITEM_NET_BALL },
    { TRAINER_CLASS_SCHOOL_KID, 5, ITEM_POKE_BALL },
    { TRAINER_CLASS_RICH_BOY, 50, ITEM_LUXURY_BALL },
    { TRAINER_CLASS_SR_AND_JR, 4, ITEM_POKE_BALL },
    { TRAINER_CLASS_RS_BLACK_BELT, 8, ITEM_GREAT_BALL },
    { TRAINER_CLASS_RS_TUBER_F, 1, ITEM_NET_BALL },
    { TRAINER_CLASS_HEX_MANIAC, 6, ITEM_POKE_BALL },
    { TRAINER_CLASS_RS_PKMN_BREEDER, 10, ITEM_POKE_BALL },
    { TRAINER_CLASS_TEAM_MAGMA, 5, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_INTERVIEWER, 12, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_RS_TUBER_M, 1, ITEM_NET_BALL },
    { TRAINER_CLASS_RS_YOUNG_COUPLE, 8, ITEM_POKE_BALL },
    { TRAINER_CLASS_GUITARIST, 8, ITEM_GREAT_BALL },
    { TRAINER_CLASS_RS_GENTLEMAN, 20, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_RS_CHAMPION, 50, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_MAGMA_LEADER, 20, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_BATTLE_GIRL, 6, ITEM_POKE_BALL },
    { TRAINER_CLASS_RS_SWIMMER_M, 2, ITEM_DIVE_BALL },
    { TRAINER_CLASS_POKEFAN, 20, ITEM_POKE_BALL },
    { TRAINER_CLASS_EXPERT, 10, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_DRAGON_TAMER, 12, ITEM_GREAT_BALL },
    { TRAINER_CLASS_RS_BIRD_KEEPER, 8, ITEM_GREAT_BALL },
    { TRAINER_CLASS_NINJA_BOY, 3, ITEM_POKE_BALL },
    { TRAINER_CLASS_PARASOL_LADY, 10, ITEM_LUXURY_BALL },
    { TRAINER_CLASS_BUG_MANIAC, 15, ITEM_NET_BALL },
    { TRAINER_CLASS_RS_SAILOR, 8, ITEM_NET_BALL },
    { TRAINER_CLASS_COLLECTOR, 15, ITEM_GREAT_BALL },
    { TRAINER_CLASS_RS_PKMN_RANGER, 12, ITEM_SAFARI_BALL },
    { TRAINER_CLASS_MAGMA_ADMIN, 10, ITEM_ULTRA_BALL },
    { TRAINER_CLASS_RS_AROMA_LADY, 10, ITEM_LUXURY_BALL },
    { TRAINER_CLASS_RS_RUIN_MANIAC, 15, ITEM_PREMIER_BALL },
    { TRAINER_CLASS_RS_COOLTRAINER, 12, ITEM_POKE_BALL },
    { TRAINER_CLASS_RS_POKEMANIAC, 15, ITEM_POKE_BALL },
    { TRAINER_CLASS_KINDLER, 8, ITEM_POKE_BALL },
    { TRAINER_CLASS_RS_CAMPER, 4, ITEM_POKE_BALL },
    { TRAINER_CLASS_RS_PICNICKER, 4, ITEM_POKE_BALL },
    { TRAINER_CLASS_RS_PSYCHIC, 6, ITEM_GREAT_BALL },
    { TRAINER_CLASS_RS_SIS_AND_BRO, 3, ITEM_POKE_BALL },
    { TRAINER_CLASS_OLD_COUPLE, 10, ITEM_GREAT_BALL },
    { TRAINER_CLASS_AQUA_ADMIN, 10, ITEM_NET_BALL },
    { TRAINER_CLASS_AQUA_LEADER, 20, ITEM_NET_BALL },
    { TRAINER_CLASS_BOSS, 25, ITEM_ULTRA_BALL },
    { 0xFF, 5, ITEM_NONE },
};

#include "data/pokemon/abilities.h"

static void (*const sTurnActionsFuncsTable[])(void) =
{
    [B_ACTION_USE_MOVE] = HandleAction_UseMove,
    [B_ACTION_USE_ITEM] = HandleAction_UseItem,
    [B_ACTION_SWITCH] = HandleAction_Switch,
    [B_ACTION_RUN] = HandleAction_Run,
    [B_ACTION_SAFARI_WATCH_CAREFULLY] = HandleAction_WatchesCarefully,
    [B_ACTION_SAFARI_BALL] = HandleAction_SafariZoneBallThrow,
    [B_ACTION_SAFARI_BAIT] = HandleAction_ThrowBait,
    [B_ACTION_SAFARI_GO_NEAR] = HandleAction_ThrowRock,
    [B_ACTION_SAFARI_RUN] = HandleAction_SafariZoneRun,
    [B_ACTION_OLDMAN_THROW] = HandleAction_OldManBallThrow,
    [B_ACTION_EXEC_SCRIPT] = HandleAction_RunBattleScript,
    [B_ACTION_TRY_FINISH] = HandleAction_TryFinish,
    [B_ACTION_FINISHED] = HandleAction_ActionFinished,
    [B_ACTION_NOTHING_FAINTED] = HandleAction_NothingIsFainted,
};

static void (*const sEndTurnFuncsTable[])(void) =
{
    [0] = HandleEndTurn_ContinueBattle, //B_OUTCOME_NONE?
    [B_OUTCOME_WON] = HandleEndTurn_BattleWon,
    [B_OUTCOME_LOST] = HandleEndTurn_BattleLost,
    [B_OUTCOME_DREW] = HandleEndTurn_BattleLost,
    [B_OUTCOME_RAN] = HandleEndTurn_RanFromBattle,
    [B_OUTCOME_PLAYER_TELEPORTED] = HandleEndTurn_FinishBattle,
    [B_OUTCOME_MON_FLED] = HandleEndTurn_MonFled,
    [B_OUTCOME_CAUGHT] = HandleEndTurn_FinishBattle,
    [B_OUTCOME_NO_SAFARI_BALLS] = HandleEndTurn_FinishBattle,
	[B_OUTCOME_MON_TELEPORTED] = HandleEndTurn_FinishBattle,
};

const u8 gStatusConditionString_PoisonJpn[8] = _("どく$$$$$");
const u8 gStatusConditionString_SleepJpn[8] = _("ねむり$$$$");
const u8 gStatusConditionString_ParalysisJpn[8] = _("まひ$$$$$");
const u8 gStatusConditionString_BurnJpn[8] = _("やけど$$$$");
const u8 gStatusConditionString_IceJpn[8] = _("こおり$$$$");
const u8 gStatusConditionString_ConfusionJpn[8] = _("こんらん$$$");
const u8 gStatusConditionString_LoveJpn[8] = _("メロメロ$$$");

const u8 *const gStatusConditionStringsTable[7][2] =
{
    { gStatusConditionString_PoisonJpn, gText_Poison },
    { gStatusConditionString_SleepJpn, gText_Sleep },
    { gStatusConditionString_ParalysisJpn, gText_Paralysis },
    { gStatusConditionString_BurnJpn, gText_Burn },
    { gStatusConditionString_IceJpn, gText_Ice },
    { gStatusConditionString_ConfusionJpn, gText_Confusion },
    { gStatusConditionString_LoveJpn, gText_Love }
};

void CB2_InitBattle(void)
{
    MoveSaveBlocks_ResetHeap();
    AllocateBattleResources();
    AllocateBattleSpritesData();
    AllocateMonSpritesGfx();
    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        HandleLinkBattleSetup();
        SetMainCallback2(CB2_PreInitMultiBattle);
        gBattleCommunication[MULTIUSE_STATE] = 0;
    }
    else
    {
        CB2_InitBattleInternal();
    }
}

static void CB2_InitBattleInternal(void)
{
    s32 i;

    SetHBlankCallback(NULL);
    SetVBlankCallback(NULL);
    CpuFill32(0, (void *)VRAM, VRAM_SIZE);
    SetGpuReg(REG_OFFSET_MOSAIC, 0);
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(0, 0xF0));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(0x50, 0x51));
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);
    gBattle_WIN0H = WIN_RANGE(0, 0xF0);
    gBattle_WIN0V = WIN_RANGE(0x50, 0x51);
    ScanlineEffect_Clear();
    for (i = 0; i < 80; ++i)
    {
        gScanlineEffectRegBuffers[0][i] = 0xF0;
        gScanlineEffectRegBuffers[1][i] = 0xF0;
    }
    for (; i < 160; ++i)
    {
        gScanlineEffectRegBuffers[0][i] = 0xFF10;
        gScanlineEffectRegBuffers[1][i] = 0xFF10;
    }
    ScanlineEffect_SetParams(sIntroScanlineParams16Bit);
    ResetPaletteFade();
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    gBattle_BG1_X = 0;
    gBattle_BG1_Y = 0;
    gBattle_BG2_X = 0;
    gBattle_BG2_Y = 0;
    gBattle_BG3_X = 0;
    gBattle_BG3_Y = 0;
    gBattleTerrain = BattleSetup_GetTerrainId();
    InitBattleBgsVideo();
    LoadBattleTextboxAndBackground();
    ResetSpriteData();
    ResetTasks();
    DrawBattleEntryBackground();
    FreeAllSpritePalettes();
    gReservedSpritePaletteCount = 4;
    SetVBlankCallback(VBlankCB_Battle);
    SetUpBattleVars();
	SetMainCallback2((gBattleTypeFlags & BATTLE_TYPE_MULTI) ? CB2_HandleStartMultiBattle : CB2_HandleStartBattle);
	
    if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
    {
        CreateNPCTrainerParty(&gEnemyParty[0], gTrainerBattleOpponent_A);
        SetWildMonHeldItem(&gEnemyParty[0]);
		
		if (IS_DOUBLE_WILD_BATTLE())
			SetWildMonHeldItem(&gEnemyParty[1]);
		
		CalculateEnemyPartyCount();
    }
    gMain.inBattle = TRUE;
    for (i = 0; i < PARTY_SIZE; ++i)
        AdjustFriendship(&gPlayerParty[i], FRIENDSHIP_EVENT_LEAGUE_BATTLE);
    gBattleCommunication[MULTIUSE_STATE] = 0;
}

#define BUFFER_PARTY_VS_SCREEN_STATUS(party, flags, i)              \
    for ((i) = 0; (i) < PARTY_SIZE; (i)++)                          \
    {                                                               \
        u16 species = GetMonData(&(party)[(i)], MON_DATA_SPECIES2); \
        u16 hp = GetMonData(&(party)[(i)], MON_DATA_HP);            \
        u32 status = GetMonData(&(party)[(i)], MON_DATA_STATUS);    \
                                                                    \
        if (species == SPECIES_NONE)                                \
            continue;                                               \
                                                                    \
        /* Is healthy mon? */                                       \
        if (species != SPECIES_EGG && hp != 0 && status == 0)       \
            (flags) |= 1 << (i) * 2;                                \
                                                                    \
        /* Is Egg or statused? */                                   \
        if (hp != 0 && (species == SPECIES_EGG || status != 0))     \
            (flags) |= 2 << (i) * 2;                                \
                                                                    \
        /* Is fainted? */                                           \
        if (species != SPECIES_EGG && hp == 0)                      \
            (flags) |= 3 << (i) * 2;                                \
    }

static void BufferPartyVsScreenHealth_AtStart(void)
{
    u16 flags = 0;
    s32 i;

    BUFFER_PARTY_VS_SCREEN_STATUS(gPlayerParty, flags, i);
    gBattleStruct->multiBuffer.linkPartnerHeader.vsScreenHealthFlagsLo = flags;
    *(&gBattleStruct->multiBuffer.linkPartnerHeader.vsScreenHealthFlagsHi) = flags >> 8;
}

static void LinkBattleComputeBattleTypeFlags(u8 numPlayers, u8 multiPlayerId)
{
    u8 found = 0;

    // If player 1 is playing the minimum version, player 1 is master.
    if (gBlockRecvBuffer[0][0] == 0x100)
    {
        if (multiPlayerId == 0)
            gBattleTypeFlags |= BATTLE_TYPE_IS_MASTER | BATTLE_TYPE_TRAINER;
        else
            gBattleTypeFlags |= BATTLE_TYPE_TRAINER;
        ++found;
    }
    if (found == 0)
    {
        // If multiple different versions are being used, player 1 is master.
        s32 i;

        for (i = 0; i < numPlayers; ++i)
            if (gBlockRecvBuffer[0][0] != gBlockRecvBuffer[i][0])
                break;
        if (i == numPlayers)
        {
            if (multiPlayerId == 0)
                gBattleTypeFlags |= BATTLE_TYPE_IS_MASTER | BATTLE_TYPE_TRAINER;
            else
                gBattleTypeFlags |= BATTLE_TYPE_TRAINER;
            ++found;
        }
        if (found == 0)
        {
            // Lowest index player with the highest game version is master.
            for (i = 0; i < numPlayers; ++i)
            {
                if (gBlockRecvBuffer[i][0] == 0x201 && i != multiPlayerId)
                    if (i < multiPlayerId)
                        break;
                if (gBlockRecvBuffer[i][0] > 0x201 && i != multiPlayerId)
                    break;
            }
            if (i == numPlayers)
                gBattleTypeFlags |= BATTLE_TYPE_IS_MASTER | BATTLE_TYPE_TRAINER;
            else
                gBattleTypeFlags |= BATTLE_TYPE_TRAINER;
        }
    }
}

static void CB2_HandleStartBattle(void)
{
    u8 i, playerMultiplayerId, enemyMultiplayerId;

    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    playerMultiplayerId = GetMultiplayerId();
    gBattleStruct->multiplayerId = playerMultiplayerId;
    enemyMultiplayerId = BATTLE_OPPOSITE(playerMultiplayerId);
    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            ShowBg(0);
            ShowBg(1);
            ShowBg(2);
            ShowBg(3);
            BattleInterfaceSetWindowPals();
            gBattleCommunication[MULTIUSE_STATE] = 1;
        }
        if (gWirelessCommType)
            LoadWirelessStatusIndicatorSpriteGfx();
        break;
    case 1:
        if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        {
            if (gReceivedRemoteLinkPlayers)
            {
                if (IsLinkTaskFinished())
                {
                    // 0x201
                    *(&gBattleStruct->multiBuffer.linkPartnerHeader.versionSignatureLo) = 1;
                    *(&gBattleStruct->multiBuffer.linkPartnerHeader.versionSignatureHi) = 2;
                    BufferPartyVsScreenHealth_AtStart();
                    SendBlock(bitmask_all_link_players_but_self(), &gBattleStruct->multiBuffer.linkPartnerHeader, sizeof(gBattleStruct->multiBuffer.linkPartnerHeader));
                    gBattleCommunication[MULTIUSE_STATE] = 2;
                }
                if (gWirelessCommType != 0)
                    CreateWirelessStatusIndicatorSprite(0, 0);
            }
        }
        else
        {
            gBattleTypeFlags |= BATTLE_TYPE_IS_MASTER;
            gBattleCommunication[MULTIUSE_STATE] = 15;
        }
        break;
    case 2:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            u8 taskId;

            ResetBlockReceivedFlags();
            LinkBattleComputeBattleTypeFlags(2, playerMultiplayerId);
            taskId = CreateTask(InitLinkBattleVsScreen, 0);
            gTasks[taskId].data[1] = 270;
            gTasks[taskId].data[2] = 90;
            gTasks[taskId].data[5] = 0;
            gTasks[taskId].data[3] = gBattleStruct->multiBuffer.linkPartnerHeader.vsScreenHealthFlagsLo | (gBattleStruct->multiBuffer.linkPartnerHeader.vsScreenHealthFlagsHi << 8);
            gTasks[taskId].data[4] = gBlockRecvBuffer[enemyMultiplayerId][1];
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 3:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gPlayerParty, sizeof(struct Pokemon) * 2);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 4:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            ResetBlockReceivedFlags();
            memcpy(gEnemyParty, gBlockRecvBuffer[enemyMultiplayerId], sizeof(struct Pokemon) * 2);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 7:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gPlayerParty + 2, sizeof(struct Pokemon) * 2);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 8:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            ResetBlockReceivedFlags();
            memcpy(gEnemyParty + 2, gBlockRecvBuffer[enemyMultiplayerId], sizeof(struct Pokemon) * 2);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 11:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gPlayerParty + 4, sizeof(struct Pokemon) * 2);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 12:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            ResetBlockReceivedFlags();
            memcpy(gEnemyParty + 4, gBlockRecvBuffer[enemyMultiplayerId], sizeof(struct Pokemon) * 2);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 15:
        InitBtlControllers();
        ++gBattleCommunication[MULTIUSE_STATE];
        gBattleCommunication[SPRITES_INIT_STATE1] = 0;
        gBattleCommunication[SPRITES_INIT_STATE2] = 0;
        break;
    case 16:
        if (BattleInitAllSprites(&gBattleCommunication[SPRITES_INIT_STATE1], &gBattleCommunication[SPRITES_INIT_STATE2]))
        {
            gPreBattleCallback1 = gMain.callback1;
            gMain.callback1 = BattleMainCB1;
            SetMainCallback2(BattleMainCB2);
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                gBattleTypeFlags |= BATTLE_TYPE_LINK_ESTABLISHED;
            }
        }
        break;
    case 5:
    case 9:
    case 13:
        ++gBattleCommunication[MULTIUSE_STATE];
        gBattleCommunication[SPRITES_INIT_STATE1] = 1;
    case 6:
    case 10:
    case 14:
        if (--gBattleCommunication[SPRITES_INIT_STATE1] == 0)
            ++gBattleCommunication[MULTIUSE_STATE];
        break;
    }
}

static void PrepareOwnMultiPartnerBuffer(void)
{
    s32 i, j;
    u8 *nick, *cur;

    for (i = 0; i < 3; ++i)
    {
        gMultiPartnerParty[i].species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES);
        gMultiPartnerParty[i].heldItem = GetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM);
        nick = gMultiPartnerParty[i].nickname;
        GetMonData(&gPlayerParty[i], MON_DATA_NICKNAME, nick);
        gMultiPartnerParty[i].level = GetMonData(&gPlayerParty[i], MON_DATA_LEVEL);
        gMultiPartnerParty[i].hp = GetMonData(&gPlayerParty[i], MON_DATA_HP);
        gMultiPartnerParty[i].maxhp = GetMonData(&gPlayerParty[i], MON_DATA_MAX_HP);
        gMultiPartnerParty[i].status = GetMonData(&gPlayerParty[i], MON_DATA_STATUS);
        gMultiPartnerParty[i].personality = GetMonData(&gPlayerParty[i], MON_DATA_PERSONALITY);
        gMultiPartnerParty[i].gender = GetMonGender(&gPlayerParty[i]);
        StripExtCtrlCodes(nick);
        if (GetMonData(&gPlayerParty[i], MON_DATA_LANGUAGE) != LANGUAGE_JAPANESE)
        {
            for (cur = nick, j = 0; cur[j] != EOS; ++j)
                ;
            while (j < 6)
                cur[j++] = 0;
            cur[j] = EOS;
        }
    }
    memcpy(gBattleStruct->multiBuffer.multiBattleMons, gMultiPartnerParty, sizeof(gMultiPartnerParty));
}

static void CB2_PreInitMultiBattle(void)
{
    s32 i;
    u8 playerMultiplierId;
    u8 r4 = 0xF;
    u16 *savedBattleTypeFlags;
    void (**savedCallback)(void);

    playerMultiplierId = GetMultiplayerId();
    gBattleStruct->multiplayerId = playerMultiplierId;
    savedCallback = &gBattleStruct->savedCallback;
    savedBattleTypeFlags = &gBattleStruct->savedBattleTypeFlags;

    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        if (gReceivedRemoteLinkPlayers && IsLinkTaskFinished())
        {
            PrepareOwnMultiPartnerBuffer();
            SendBlock(bitmask_all_link_players_but_self(), gBattleStruct->multiBuffer.multiBattleMons, sizeof(gBattleStruct->multiBuffer.multiBattleMons));
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 1:
        if ((GetBlockReceivedStatus() & r4) == r4)
        {
            ResetBlockReceivedFlags();
            for (i = 0; i < 4; ++i)
            {
                if (i == playerMultiplierId)
                    continue;
                if ((!(gLinkPlayers[i].id & 1) && !(gLinkPlayers[playerMultiplierId].id & 1))
                 || (gLinkPlayers[i].id & 1 && gLinkPlayers[playerMultiplierId].id & 1))
                    memcpy(gMultiPartnerParty, gBlockRecvBuffer[i], sizeof(gMultiPartnerParty));
            }
            ++gBattleCommunication[MULTIUSE_STATE];
            *savedCallback = gMain.savedCallback;
            *savedBattleTypeFlags = gBattleTypeFlags;
            gMain.savedCallback = CB2_PreInitMultiBattle;
            ShowPartyMenuToShowcaseMultiBattleParty();
        }
        break;
    case 2:
        if (!gPaletteFade.active)
        {
            ++gBattleCommunication[MULTIUSE_STATE];
            if (gWirelessCommType)
                SetLinkStandbyCallback();
            else
                SetCloseLinkCallback();
        }
        break;
    case 3:
        if (gWirelessCommType)
        {
            if (IsLinkRfuTaskFinished())
            {
                gBattleTypeFlags = *savedBattleTypeFlags;
                gMain.savedCallback = *savedCallback;
                SetMainCallback2(CB2_InitBattleInternal);
            }
        }
        else if (!gReceivedRemoteLinkPlayers)
        {
            gBattleTypeFlags = *savedBattleTypeFlags;
            gMain.savedCallback = *savedCallback;
            SetMainCallback2(CB2_InitBattleInternal);
        }
        break;
    }
}

static void CB2_HandleStartMultiBattle(void)
{
    u8 i, playerMultiplayerId, taskId;
    s32 id;

    playerMultiplayerId = GetMultiplayerId();
    gBattleStruct->multiplayerId = playerMultiplayerId;
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            ShowBg(0);
            ShowBg(1);
            ShowBg(2);
            ShowBg(3);
            BattleInterfaceSetWindowPals();
            gBattleCommunication[MULTIUSE_STATE] = 1;
        }
        if (gWirelessCommType)
            LoadWirelessStatusIndicatorSpriteGfx();
        break;
    case 1:
        if (gReceivedRemoteLinkPlayers)
        {
            if (IsLinkTaskFinished())
            {
                // 0x201
                *(&gBattleStruct->multiBuffer.linkPartnerHeader.versionSignatureLo) = 1;
                *(&gBattleStruct->multiBuffer.linkPartnerHeader.versionSignatureHi) = 2;
                BufferPartyVsScreenHealth_AtStart();
                SendBlock(bitmask_all_link_players_but_self(), &gBattleStruct->multiBuffer.linkPartnerHeader, sizeof(gBattleStruct->multiBuffer.linkPartnerHeader));
                ++gBattleCommunication[MULTIUSE_STATE];
            }
            if (gWirelessCommType)
                CreateWirelessStatusIndicatorSprite(0, 0);
        }
        break;
    case 2:
        if ((GetBlockReceivedStatus() & 0xF) == 0xF)
        {
            ResetBlockReceivedFlags();
            LinkBattleComputeBattleTypeFlags(4, playerMultiplayerId);
            memcpy(gDecompressionBuffer, gPlayerParty, sizeof(struct Pokemon) * 3);
            taskId = CreateTask(InitLinkBattleVsScreen, 0);
            gTasks[taskId].data[1] = 270;
            gTasks[taskId].data[2] = 90;
            gTasks[taskId].data[5] = 0;
            gTasks[taskId].data[3] = 0;
            gTasks[taskId].data[4] = 0;
            for (id = 0; id < MAX_LINK_PLAYERS; ++id)
            {
                switch (gLinkPlayers[id].id)
                {
                case 0:
                    gTasks[taskId].data[3] |= gBlockRecvBuffer[id][1] & 0x3F;
                    break;
                case 1:
                    gTasks[taskId].data[4] |= gBlockRecvBuffer[id][1] & 0x3F;
                    break;
                case 2:
                    gTasks[taskId].data[3] |= (gBlockRecvBuffer[id][1] & 0x3F) << 6;
                    break;
                case 3:
                    gTasks[taskId].data[4] |= (gBlockRecvBuffer[id][1] & 0x3F) << 6;
                    break;
                }
            }
            ZeroPlayerPartyMons();
            ZeroEnemyPartyMons();
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        else
        {
            break;
        }
        // fall through
    case 3:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gDecompressionBuffer, sizeof(struct Pokemon) * 2);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 4:
        if ((GetBlockReceivedStatus() & 0xF) == 0xF)
        {
            ResetBlockReceivedFlags();
            for (id = 0; id < MAX_LINK_PLAYERS; ++id)
            {
                if (id == playerMultiplayerId)
                {
                    switch (gLinkPlayers[id].id)
                    {
                    case 0:
                    case 3:
                        memcpy(gPlayerParty, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                        break;
                    case 1:
                    case 2:
                        memcpy(gPlayerParty + 3, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                        break;
                    }
                }
                else
                {
                    if ((!(gLinkPlayers[id].id & 1) && !(gLinkPlayers[playerMultiplayerId].id & 1))
                     || ((gLinkPlayers[id].id & 1) && (gLinkPlayers[playerMultiplayerId].id & 1)))
                    {
                        switch (gLinkPlayers[id].id)
                        {
                        case 0:
                        case 3:
                            memcpy(gPlayerParty, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                            break;
                        case 1:
                        case 2:
                            memcpy(gPlayerParty + 3, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                            break;
                        }
                    }
                    else
                    {
                        switch (gLinkPlayers[id].id)
                        {
                        case 0:
                        case 3:
                            memcpy(gEnemyParty, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                            break;
                        case 1:
                        case 2:
                            memcpy(gEnemyParty + 3, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                            break;
                        }
                    }
                }
            }
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 7:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gDecompressionBuffer + sizeof(struct Pokemon) * 2, sizeof(struct Pokemon));
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 8:
        if ((GetBlockReceivedStatus() & 0xF) == 0xF)
        {
            ResetBlockReceivedFlags();
            for (id = 0; id < MAX_LINK_PLAYERS; ++id)
            {
                if (id == playerMultiplayerId)
                {
                    switch (gLinkPlayers[id].id)
                    {
                    case 0:
                    case 3:
                        memcpy(gPlayerParty + 2, gBlockRecvBuffer[id], sizeof(struct Pokemon));
                        break;
                    case 1:
                    case 2:
                        memcpy(gPlayerParty + 5, gBlockRecvBuffer[id], sizeof(struct Pokemon));
                        break;
                    }
                }
                else
                {
                    if ((!(gLinkPlayers[id].id & 1) && !(gLinkPlayers[playerMultiplayerId].id & 1))
                     || ((gLinkPlayers[id].id & 1) && (gLinkPlayers[playerMultiplayerId].id & 1)))
                    {
                        switch (gLinkPlayers[id].id)
                        {
                        case 0:
                        case 3:
                            memcpy(gPlayerParty + 2, gBlockRecvBuffer[id], sizeof(struct Pokemon));
                            break;
                        case 1:
                        case 2:
                            memcpy(gPlayerParty + 5, gBlockRecvBuffer[id], sizeof(struct Pokemon));
                            break;
                        }
                    }
                    else
                    {
                        switch (gLinkPlayers[id].id)
                        {
                        case 0:
                        case 3:
                            memcpy(gEnemyParty + 2, gBlockRecvBuffer[id], sizeof(struct Pokemon));
                            break;
                        case 1:
                        case 2:
                            memcpy(gEnemyParty + 5, gBlockRecvBuffer[id], sizeof(struct Pokemon));
                            break;
                        }
                    }
                }
            }
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 11:
        InitBtlControllers();
        ++gBattleCommunication[MULTIUSE_STATE];
        gBattleCommunication[SPRITES_INIT_STATE1] = 0;
        gBattleCommunication[SPRITES_INIT_STATE2] = 0;
        break;
    case 12:
        if (BattleInitAllSprites(&gBattleCommunication[SPRITES_INIT_STATE1], &gBattleCommunication[SPRITES_INIT_STATE2]))
        {
            gPreBattleCallback1 = gMain.callback1;
            gMain.callback1 = BattleMainCB1;
            SetMainCallback2(BattleMainCB2);
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
                gBattleTypeFlags |= BATTLE_TYPE_LINK_ESTABLISHED;
        }
        break;
    case 5:
    case 9:
        ++gBattleCommunication[MULTIUSE_STATE];
        gBattleCommunication[SPRITES_INIT_STATE1] = 1;
        // fall through
    case 6:
    case 10:
        if (--gBattleCommunication[SPRITES_INIT_STATE1] == 0)
            ++gBattleCommunication[MULTIUSE_STATE];
        break;
    }
}

void BattleMainCB2(void)
{
    AnimateSprites();
    BuildOamBuffer();
    RunTextPrinters();
    UpdatePaletteFade();
    RunTasks();
#if USE_DNS_IN_BATTLE
    DNSApplyFilters();
#endif
    
    if (JOY_HELD(B_BUTTON) && gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
    {
        gSpecialVar_Result = gBattleOutcome = B_OUTCOME_DREW;
        ResetPaletteFadeControl();
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 0x10, RGB_BLACK);
        SetMainCallback2(CB2_QuitPokedudeBattle);
    }
}

void FreeRestoreBattleData(void)
{
    gMain.callback1 = gPreBattleCallback1;
    gScanlineEffect.state = 3;
    gMain.inBattle = FALSE;
    ZeroEnemyPartyMons();
    m4aSongNumStop(SE_LOW_HEALTH);
    FreeMonSpritesGfx();
    FreeBattleSpritesData();
    FreeBattleResources();
}

static void CB2_QuitPokedudeBattle(void)
{
    UpdatePaletteFade();
	
    if (!gPaletteFade.active)
    {
        FreeRestoreBattleData();
        FreeAllWindowBuffers();
        SetMainCallback2(gMain.savedCallback);
    }
}

static u16 GetTrainerClassBallId(u8 trainerClass)
{
	u32 i;
	
	for (i = 0; gTrainerMoneyAndBallTable[i].classId != 0xFF; i++)
	{
		if (gTrainerMoneyAndBallTable[i].classId == trainerClass)
			break;
	}
	return gTrainerMoneyAndBallTable[i].ballId;
}

static u8 CreateNPCTrainerParty(struct Pokemon *party, u16 trainerNum)
{
    u32 i, j;
	const struct TrainerMon *partyData;
	struct PokemonGenerator generator;
	
    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER && !(gBattleTypeFlags & (BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_TRAINER_TOWER)))
	{
		ZeroEnemyPartyMons();
		
		partyData = gTrainers[trainerNum].party;
		
		for (i = 0; i < gTrainers[trainerNum].partySize; i++)
		{
			generator.species = partyData[i].species;
			generator.level = GetTrainerPartyMonLevel(partyData[i]);
			
			if (partyData[i].gender)
			{
				switch (partyData[i].gender)
				{
					case TRAINER_MON_MALE:
						generator.forcedGender = MON_MALE;
						break;
					case TRAINER_MON_FEMALE:
						generator.forcedGender = MON_FEMALE;
						break;
					case TRAINER_MON_GENDERLESS:
						generator.forcedGender = MON_GENDERLESS;
						break;
				}
				generator.forceGender = TRUE;
			}
			else
			{
				generator.forceGender = FALSE;
				generator.forcedGender = MON_MALE;
			}
			
			generator.shinyType = partyData[i].isShiny ? GENERATE_SHINY_FORCED : GENERATE_SHINY_LOCKED;
			generator.otIdType = OT_ID_RANDOM;
			generator.hasFixedPersonality = FALSE;
			generator.fixedPersonality = 0;
			
			if (partyData[i].nature)
			{
				generator.forceNature = TRUE;
				generator.forcedNature = partyData[i].nature - 1;
			}
			else
			{
				generator.forceNature = FALSE;
				generator.forcedNature = NUM_NATURES;
			}
			generator.pokemon = &party[i];
			
			// Create mon
			CreateMon(generator);
			
			// Update form based on gender
			DoOverworldFormChange(&party[i], FORM_CHANGE_GENDER);
			
			// Set held item
			SetMonData(&party[i], MON_DATA_HELD_ITEM, &partyData[i].heldItem);
			
			// Give moves
			for (j = 0; j < MAX_MON_MOVES; j++)
			{
				if (partyData[i].moves[j])
					SetMonMoveSlot(&party[i], partyData[i].moves[j], j);
			}
			// Give nickname
			if (partyData[i].nickname != NULL)
				SetMonData(&party[i], MON_DATA_NICKNAME, partyData[i].nickname);
			
			// Set ivs
			if (partyData[i].iv)
				SetMonData(&party[i], MON_DATA_IVS, &partyData[i].iv);
			
			// Set evs
			if (partyData[i].ev != NULL)
			{
				for (j = 0; j < NUM_STATS; j++)
					SetMonData(&party[i], MON_DATA_HP_EV + j, &partyData[i].ev[j]);
			}
			// Set ability num or hidden
			if (partyData[i].abilityNum)
			{
				// Set ability hidden
				if (partyData[i].abilityNum == 3)
				{
					j = TRUE;
					SetMonData(&party[i], MON_DATA_ABILITY_HIDDEN, &j);
				}
				else // Set ability num
				{
					j = partyData[i].abilityNum - 1;
					SetMonData(&party[i], MON_DATA_ABILITY_NUM, &j);
				}
			}
			// Calculate stats
			CalculateMonStats(&party[i]);
			
			// Give the pokeball Id
#if CLASS_BASED_BALLS
            j = GetTrainerClassBallId(gTrainers[trainerNum].trainerClass);
#else
			if (partyData[i].ballId)
				j = partyData[i].ballId;
			else
				j = GetTrainerClassBallId(gTrainers[trainerNum].trainerClass);
#endif

			SetMonData(&party[i], MON_DATA_POKEBALL, &j);
		}
		if (gTrainers[trainerNum].doubleBattle)
			gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;
	}
	return gTrainers[trainerNum].partySize;
}

void VBlankCB_Battle(void)
{
    // Change gRngSeed every vblank.
    Random();
    SetGpuReg(REG_OFFSET_BG0HOFS, gBattle_BG0_X);
    SetGpuReg(REG_OFFSET_BG0VOFS, gBattle_BG0_Y);
    SetGpuReg(REG_OFFSET_BG1HOFS, gBattle_BG1_X);
    SetGpuReg(REG_OFFSET_BG1VOFS, gBattle_BG1_Y);
    SetGpuReg(REG_OFFSET_BG2HOFS, gBattle_BG2_X);
    SetGpuReg(REG_OFFSET_BG2VOFS, gBattle_BG2_Y);
    SetGpuReg(REG_OFFSET_BG3HOFS, gBattle_BG3_X);
    SetGpuReg(REG_OFFSET_BG3VOFS, gBattle_BG3_Y);
    SetGpuReg(REG_OFFSET_WIN0H, gBattle_WIN0H);
    SetGpuReg(REG_OFFSET_WIN0V, gBattle_WIN0V);
    SetGpuReg(REG_OFFSET_WIN1H, gBattle_WIN1H);
    SetGpuReg(REG_OFFSET_WIN1V, gBattle_WIN1V);
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
    ScanlineEffect_InitHBlankDmaTransfer();
}

void SpriteCB_VsLetterDummy(struct Sprite *sprite)
{
}

static void SpriteCB_VsLetter(struct Sprite *sprite)
{
    if (sprite->data[0] != 0)
        sprite->x = sprite->data[1] + ((sprite->data[2] & 0xFF00) >> 8);
    else
        sprite->x = sprite->data[1] - ((sprite->data[2] & 0xFF00) >> 8);
    sprite->data[2] += 0x180;
    if (sprite->affineAnimEnded)
    {
        FreeSpriteTilesByTag(ANIM_SPRITES_START);
        FreeSpritePaletteByTag(ANIM_SPRITES_START);
        FreeSpriteOamMatrix(sprite);
        DestroySprite(sprite);
    }
}

void SpriteCB_VsLetterInit(struct Sprite *sprite)
{
    StartSpriteAffineAnim(sprite, 1);
    sprite->callback = SpriteCB_VsLetter;
    PlaySE(SE_MUGSHOT);
}

static void BufferPartyVsScreenHealth_AtEnd(u8 taskId)
{
    struct Pokemon *party1;
    struct Pokemon *party2;
    u32 r7;
    s32 i;

    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        switch (gLinkPlayers[gBattleStruct->multiplayerId].id)
        {
        case 0:
        case 2:
            party1 = gPlayerParty;
            party2 = gEnemyParty;
            break;
        case 1:
        case 3:
            party1 = gEnemyParty;
            party2 = gPlayerParty;
            break;
        }
    }
    else
    {
        party1 = gPlayerParty;
        party2 = gEnemyParty;
    }
    r7 = 0;
    BUFFER_PARTY_VS_SCREEN_STATUS(party1, r7, i);
    gTasks[taskId].data[3] = r7;
    r7 = 0;
    BUFFER_PARTY_VS_SCREEN_STATUS(party2, r7, i);
    gTasks[taskId].data[4] = r7;
}

void CB2_InitEndLinkBattle(void)
{
    s32 i;
    u8 taskId;

    SetHBlankCallback(NULL);
    SetVBlankCallback(NULL);
    CpuFill32(0, (void *)VRAM, VRAM_SIZE);
    SetGpuReg(REG_OFFSET_MOSAIC, 0);
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(0, 0xF0));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(0x50, 0x51));
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);
    gBattle_WIN0H = WIN_RANGE(0, 0xF0);
    gBattle_WIN0V = WIN_RANGE(0x50, 0x51);
    ScanlineEffect_Clear();
    for (i = 0; i < 80; ++i)
    {
        gScanlineEffectRegBuffers[0][i] = 0xF0;
        gScanlineEffectRegBuffers[1][i] = 0xF0;
    }

    for (; i < 160; ++i)
    {
        gScanlineEffectRegBuffers[0][i] = 0xFF10;
        gScanlineEffectRegBuffers[1][i] = 0xFF10;
    }
    ResetPaletteFade();
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    gBattle_BG1_X = 0;
    gBattle_BG1_Y = 0;
    gBattle_BG2_X = 0;
    gBattle_BG2_Y = 0;
    gBattle_BG3_X = 0;
    gBattle_BG3_Y = 0;
    InitBattleBgsVideo();
    LoadCompressedPalette(gBattleTextboxPalette, 0, 64);
    LoadBattleMenuWindowGfx();
    ResetSpriteData();
    ResetTasks();
    DrawBattleEntryBackground();
    SetGpuReg(REG_OFFSET_WINOUT, WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 | WININ_WIN0_OBJ | WININ_WIN0_CLR);
    FreeAllSpritePalettes();
    gReservedSpritePaletteCount = 4;
    SetVBlankCallback(VBlankCB_Battle);
    taskId = CreateTask(InitLinkBattleVsScreen, 0);
    gTasks[taskId].data[1] = 270;
    gTasks[taskId].data[2] = 90;
    gTasks[taskId].data[5] = 1;
    BufferPartyVsScreenHealth_AtEnd(taskId);
    SetMainCallback2(CB2_EndLinkBattle);
    gBattleCommunication[MULTIUSE_STATE] = 0;
}

static void CB2_EndLinkBattle(void)
{
    EndLinkBattleInSteps();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
    RunTasks();
}

static void EndLinkBattleInSteps(void)
{
    s32 i;

    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        ShowBg(0);
        ShowBg(1);
        ShowBg(2);
        gBattleCommunication[SPRITES_INIT_STATE1] = 0xFF;
        ++gBattleCommunication[MULTIUSE_STATE];
        break;
    case 1:
        if (--gBattleCommunication[SPRITES_INIT_STATE1] == 0)
        {
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 0x10, RGB_BLACK);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 2:
        if (!gPaletteFade.active)
        {
            SetMainCallback2(gMain.savedCallback);
            TrySetQuestLogLinkBattleEvent();
            FreeMonSpritesGfx();
            FreeBattleSpritesData();
            FreeBattleResources();
        }
        break;
    }
}

u32 GetBattleBgAttribute(u8 arrayId, u8 caseId)
{
    u32 ret = 0;

    switch (caseId)
    {
    case 0:
        ret = gBattleBgTemplates[arrayId].bg;
        break;
    case 1:
        ret = gBattleBgTemplates[arrayId].charBaseIndex;
        break;
    case 2:
        ret = gBattleBgTemplates[arrayId].mapBaseIndex;
        break;
    case 3:
        ret = gBattleBgTemplates[arrayId].screenSize;
        break;
    case 4:
        ret = gBattleBgTemplates[arrayId].paletteMode;
        break;
    case 5:
        ret = gBattleBgTemplates[arrayId].priority;
        break;
    case 6:
        ret = gBattleBgTemplates[arrayId].baseTile;
        break;
    }
    return ret;
}

#define sBattler            data[0]
#define sSpeciesId          data[2]

void SpriteCB_EnemyMon(struct Sprite *sprite)
{
    sprite->callback = SpriteCB_MoveWildMonToRight;
    StartSpriteAnimIfDifferent(sprite, 0);
	if (IS_DOUBLE_WILD_BATTLE())
		BeginNormalPaletteFade((0x10000 << sprite->sBattler) | (0x10000 << BATTLE_PARTNER(sprite->sBattler)), 0, 10, 10, RGB(8, 8, 8));
	else
		BeginNormalPaletteFade((0x10000 << sprite->sBattler), 0, 10, 10, RGB(8, 8, 8));
}

static void SpriteCB_WildMonWaitCry(struct Sprite *sprite)
{
	if (IsCryFinished())
	{
		PlayCry1(sprite->data[2], 25);
		sprite->callback = SpriteCB_WildMonShowHealthbox;
	}
}

static void SpriteCB_MoveWildMonToRight(struct Sprite *sprite)
{
    if ((gIntroSlideFlags & 1) == 0)
    {
        sprite->x2 += 2;
        if (sprite->x2 == 0)
            sprite->callback = SpriteCB_WildMonWaitCry;
    }
}

static void SpriteCB_WildMonShowHealthbox(struct Sprite *sprite)
{
    if (sprite->animEnded)
    {
        StartHealthboxSlideIn(sprite->sBattler);
        SetHealthboxSpriteVisible(gHealthboxSpriteIds[sprite->sBattler]);
        sprite->callback = SpriteCallbackDummy2;
        StartSpriteAnimIfDifferent(sprite, 0);
		if (IS_DOUBLE_WILD_BATTLE())
			BeginNormalPaletteFade((0x10000 << sprite->sBattler) | (0x10000 << BATTLE_PARTNER(sprite->sBattler)), 0, 10, 0, RGB(8, 8, 8));
		else
			BeginNormalPaletteFade((0x10000 << sprite->sBattler), 0, 10, 0, RGB(8, 8, 8));
    }
}

void SpriteCallbackDummy2(struct Sprite *sprite)
{
}

void SpriteCB_FaintOpponentMon(struct Sprite *sprite)
{
    u8 battler = sprite->sBattler;
    u16 species;
    u8 yOffset;

    if (gBattleSpritesDataPtr->battlerData[battler].transformSpecies != 0)
        species = gBattleSpritesDataPtr->battlerData[battler].transformSpecies;
    else
        species = sprite->sSpeciesId;
	
    sprite->data[3] = 8 - gMonFrontPicCoords[SanitizeSpeciesId(species)].y_offset / 8;
    sprite->data[4] = 1;
    sprite->callback = SpriteCB_AnimFaintOpponent;
}

static void SpriteCB_AnimFaintOpponent(struct Sprite *sprite)
{
    s32 i;

    if (--sprite->data[4] == 0)
    {
        sprite->data[4] = 2;
        sprite->y2 += 8; // Move the sprite down.
        if (--sprite->data[3] < 0)
        {
            FreeSpriteOamMatrix(sprite);
            DestroySprite(sprite);
        }
        else // Erase bottom part of the sprite to create a smooth illusion of mon falling down.
        {
            u8 *dst = (u8 *)gMonSpritesGfxPtr->sprites[GetBattlerPosition(sprite->sBattler)] + (sprite->data[3] << 8);

            for (i = 0; i < 0x100; ++i)
                *(dst++) = 0;
            StartSpriteAnim(sprite, 0);
        }
    }
}

void SpriteCb_ShowAsMoveTarget(struct Sprite *sprite)
{
    sprite->data[3] = 8;
    sprite->data[4] = sprite->invisible;
    sprite->callback = SpriteCb_BlinkVisible;
}

static void SpriteCb_BlinkVisible(struct Sprite *sprite)
{
    --sprite->data[3];
    if (sprite->data[3] == 0)
    {
        sprite->invisible ^= 1;
        sprite->data[3] = 8;
    }
}

void SpriteCb_HideAsMoveTarget(struct Sprite *sprite)
{
    sprite->invisible = sprite->data[4];
    sprite->data[4] = FALSE;
    sprite->callback = SpriteCallbackDummy2;
}

void SpriteCB_AllyMon(struct Sprite *sprite)
{
    sprite->callback = oac_poke_ally_;
}

static void oac_poke_ally_(struct Sprite *sprite)
{
    if (!(gIntroSlideFlags & 1))
    {
        sprite->x2 -= 2;
        if (sprite->x2 == 0)
        {
            sprite->callback = SpriteCallbackDummy3;
            sprite->data[1] = 0;
        }
    }
}

void SpriteCB_SetToDummy3(struct Sprite *sprite)
{
    sprite->callback = SpriteCallbackDummy3;
}

static void SpriteCallbackDummy3(struct Sprite *sprite)
{
}

void SpriteCB_FaintSlideAnim(struct Sprite *sprite)
{
    if (!(gIntroSlideFlags & 1))
    {
        sprite->x2 += sprite->data[1];
        sprite->y2 += sprite->data[2];
    }
}

#define sSinIndex           data[0]
#define sDelta              data[1]
#define sAmplitude          data[2]
#define sBouncerSpriteId    data[3]
#define sWhich              data[4]

void DoBounceEffect(u8 battler, u8 which, s8 delta, s8 amplitude)
{
    u8 invisibleSpriteId;
    u8 bouncerSpriteId;

    switch (which)
    {
    case BOUNCE_HEALTHBOX:
    default:
        if (gBattleSpritesDataPtr->healthBoxesData[battler].healthboxIsBouncing)
            return;
        break;
    case BOUNCE_MON:
        if (gBattleSpritesDataPtr->healthBoxesData[battler].battlerIsBouncing)
            return;
        break;
    }
    invisibleSpriteId = CreateInvisibleSpriteWithCallback(SpriteCB_BounceEffect);
    if (which == BOUNCE_HEALTHBOX)
    {
        bouncerSpriteId = gHealthboxSpriteIds[battler];
        gBattleSpritesDataPtr->healthBoxesData[battler].healthboxBounceSpriteId = invisibleSpriteId;
        gBattleSpritesDataPtr->healthBoxesData[battler].healthboxIsBouncing = 1;
        gSprites[invisibleSpriteId].sSinIndex = 128; // 0
    }
    else
    {
        bouncerSpriteId = gBattlerSpriteIds[battler];
        gBattleSpritesDataPtr->healthBoxesData[battler].battlerBounceSpriteId = invisibleSpriteId;
        gBattleSpritesDataPtr->healthBoxesData[battler].battlerIsBouncing = 1;
        gSprites[invisibleSpriteId].sSinIndex = 192; // -1
    }
    gSprites[invisibleSpriteId].sDelta = delta;
    gSprites[invisibleSpriteId].sAmplitude = amplitude;
    gSprites[invisibleSpriteId].sBouncerSpriteId = bouncerSpriteId;
    gSprites[invisibleSpriteId].sWhich = which;
    gSprites[bouncerSpriteId].x2 = 0;
    gSprites[bouncerSpriteId].y2 = 0;
}

void EndBounceEffect(u8 battler, u8 which)
{
    u8 bouncerSpriteId;

    if (which == BOUNCE_HEALTHBOX)
    {
        if (!gBattleSpritesDataPtr->healthBoxesData[battler].healthboxIsBouncing)
            return;

        bouncerSpriteId = gSprites[gBattleSpritesDataPtr->healthBoxesData[battler].healthboxBounceSpriteId].sBouncerSpriteId;
        DestroySprite(&gSprites[gBattleSpritesDataPtr->healthBoxesData[battler].healthboxBounceSpriteId]);
        gBattleSpritesDataPtr->healthBoxesData[battler].healthboxIsBouncing = 0;
    }
    else
    {
        if (!gBattleSpritesDataPtr->healthBoxesData[battler].battlerIsBouncing)
            return;

        bouncerSpriteId = gSprites[gBattleSpritesDataPtr->healthBoxesData[battler].battlerBounceSpriteId].sBouncerSpriteId;
        DestroySprite(&gSprites[gBattleSpritesDataPtr->healthBoxesData[battler].battlerBounceSpriteId]);
        gBattleSpritesDataPtr->healthBoxesData[battler].battlerIsBouncing = 0;
    }
    gSprites[bouncerSpriteId].x2 = 0;
    gSprites[bouncerSpriteId].y2 = 0;
}

static void SpriteCB_BounceEffect(struct Sprite *sprite)
{
    gSprites[sprite->sBouncerSpriteId].y2 = Sin(sprite->sSinIndex, sprite->sAmplitude) + sprite->sAmplitude;
    sprite->sSinIndex = (sprite->sSinIndex + sprite->sDelta) & 0xFF;
}

void SpriteCB_PlayerThrowInit(struct Sprite *sprite)
{
    StartSpriteAnim(sprite, 1);
    sprite->callback = SpriteCB_PlayerThrowUpdate;
}

void UpdatePlayerPosInThrowAnim(struct Sprite *sprite)
{
    if (sprite->animDelayCounter == 0)
        sprite->centerToCornerVecX = sPlayerThrowXTranslation[sprite->animCmdIndex];
}

static void SpriteCB_PlayerThrowUpdate(struct Sprite *sprite)
{
    UpdatePlayerPosInThrowAnim(sprite);
    if (sprite->animEnded)
        sprite->callback = SpriteCallbackDummy3;
}

void BattleDummy(void)
{
}

void BeginBattleIntro(void)
{
    BattleStartClearSetData();
    gBattleCommunication[SPRITES_INIT_STATE1] = 0;
    gBattleMainFunc = BattleIntroGetMonsData;
}

static void BattleMainCB1(void)
{
    gBattleMainFunc();
    for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
        gBattlerControllerFuncs[gActiveBattler]();
}

static void BattleStartClearSetData(void)
{
    s32 i, j;

    TurnValuesCleanUp(FALSE);
    memset(&gSpecialStatuses, 0, sizeof(gSpecialStatuses));
	memset(&gDisableStructs, 0, sizeof(gDisableStructs));
	memset(&gSideStatuses, 0, sizeof(gSideStatuses));
	memset(&gSideTimers, 0, sizeof(gSideTimers));
	memset(&gWishFutureKnock, 0, sizeof(gWishFutureKnock));
	
    for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
    {
        gStatuses3[i] = 0;
        gDisableStructs[i].isFirstTurn = 2;
        gLastMoves[i] = MOVE_NONE;
        gLastLandedMoves[i] = MOVE_NONE;
        gLastHitByType[i] = 0;
        gLastResultingMoves[i] = MOVE_NONE;
        gLastHitBy[i] = 0xFF;
        gLockedMoves[i] = MOVE_NONE;
        gLastPrintedMoves[i] = MOVE_NONE;
        gBattleResources->flags->flags[i] = 0;
		for (j = 0; j < MAX_BATTLERS_COUNT; j++)
			gBattleStruct->lastTakenMoveFrom[i][j] = MOVE_NONE;
		gBattleStruct->lastTakenMove[i] = MOVE_NONE;
		gBattleStruct->choicedMove[i] = MOVE_NONE;
		gBattleStruct->changedItems[i] = ITEM_NONE;
		gBattleStruct->targetsDone[i] = 0;
		gBattleStruct->supremeOverlordBoosts[i] = 0;
		gBattleStruct->abilityOverride[i] = ABILITY_NONE;
		gBattleStruct->pickupStack[i] = 0xFF;
		ClearIllusionMon(i);
    }
	for (i = 0; i < B_SIDE_COUNT; i++)
	{
		gBattleStruct->intrepidSwordActivated[i] = 0;
		gBattleStruct->dauntlessShieldActivated[i] = 0;
		gBattleStruct->zeroToHeroActivated[i] = 0;
		gBattleStruct->allowedToChangeFormInWeather[i] = 0;
	}
    gBattlerAttacker = 0;
    gBattlerTarget = 0;
    gBattleWeather = 0;
    gHitMarker = 0;
	
    gBattleScripting.battleStyle = gSaveBlock2Ptr->optionsBattleStyle;
    gMultiHitCounter = 0;
	gBattleScripting.savedDmg = 0;
    gBattleOutcome = 0;
    gBattleControllerExecFlags = 0;
    gPaydayMoney = 0;
    gBattleResources->battleScriptsStack->size = 0;
    gBattleResources->battleCallbackStack->size = 0;
	
    for (i = 0; i < BATTLE_COMMUNICATION_ENTRIES_COUNT; ++i)
        gBattleCommunication[i] = 0;
	
    gPauseCounterBattle = 0;
    gBattleMoveDamage = 0;
    gIntroSlideFlags = 0;
    gBattleScripting.animTurn = 0;
    gBattleScripting.animTargetsHit = 0;
    gLeveledUpInBattle = 0;
    gAbsentBattlerFlags = 0;
    gBattleStruct->runTries = 0;
    gBattleStruct->safariGoNearCounter = 0;
    gBattleStruct->safariPkblThrowCounter = 0;
	// safari battles can't be double battles, so this no need to be changed
    gBattleStruct->safariCatchFactor = gBaseStats[GetMonData(&gEnemyParty[0], MON_DATA_SPECIES)].catchRate * 100 / 1275;
	
    gBattleStruct->safariEscapeFactor = gBaseStats[GetMonData(&gEnemyParty[0], MON_DATA_SPECIES)].safariZoneFleeRate * 100 / 1275;
	if (gBattleStruct->safariEscapeFactor <= 1)
        gBattleStruct->safariEscapeFactor = 2;
	
    gBattleStruct->wildVictorySong = FALSE;
    gBattleStruct->moneyMultiplier = 1;
	gBattleStruct->appearedInBattle = 0;
	gBattleStruct->firstSuperEffectiveHitTakenMsgState = 0;
	gBattleStruct->firstCritcalHitTakenMsgState = 0;
	gBattleStruct->firstMonSendOutMsgDone = FALSE;
	gBattleStruct->firstMonDownMsgDone = FALSE;
	gBattleStruct->lastMonSendOutMsgDone = FALSE;
	gBattleStruct->lastMonLowHPMsgDone = FALSE;
	gBattleStruct->megaEvolutionMsgDone = FALSE;
	gBattleStruct->zMoveMsgDone = FALSE;
	gBattleStruct->dynamaxMsgDone = FALSE;
	gBattleStruct->terastalMsgDone = FALSE;
	gBattleStruct->soulHeartBattlerId = 0;
	gBattleStruct->throwingPokeBall = FALSE;
	
	for (i = 0; i < PARTY_SIZE; i++)
	{
		gBattleStruct->usedHeldItems[i][B_SIDE_PLAYER] = ITEM_NONE;
		gBattleStruct->usedHeldItems[i][B_SIDE_OPPONENT] = ITEM_NONE;
		gPartyCriticalHits[i] = 0;
	}
    *(gBattleStruct->AI_monToSwitchIntoId + 0) = PARTY_SIZE;
    *(gBattleStruct->AI_monToSwitchIntoId + 1) = PARTY_SIZE;
    gBattleStruct->givenExpMons = 0;
}

void SwitchInClearSetData(void)
{
    struct DisableStruct disableStructCopy = gDisableStructs[gActiveBattler];
    s32 i;

    ClearIllusionMon(gActiveBattler);

    if (gBattleMoves[gCurrentMove].effect != EFFECT_BATON_PASS)
    {
		TryResetBattlerStatChanges(gActiveBattler);
        for (i = 0; i < gBattlersCount; ++i)
        {
            if ((gBattleMons[i].status2 & STATUS2_ESCAPE_PREVENTION) && gDisableStructs[i].battlerPreventingEscape == gActiveBattler)
                gBattleMons[i].status2 &= ~STATUS2_ESCAPE_PREVENTION;
            if ((gStatuses3[i] & STATUS3_ALWAYS_HITS) && gDisableStructs[i].battlerWithSureHit == gActiveBattler)
            {
                gStatuses3[i] &= ~STATUS3_ALWAYS_HITS;
                gDisableStructs[i].battlerWithSureHit = 0;
            }
        }
    }
    if (gBattleMoves[gCurrentMove].effect == EFFECT_BATON_PASS)
    {
        gBattleMons[gActiveBattler].status2 &= (STATUS2_CONFUSION | STATUS2_FOCUS_ENERGY | STATUS2_SUBSTITUTE | STATUS2_ESCAPE_PREVENTION | STATUS2_CURSED);
        gStatuses3[gActiveBattler] &= (STATUS3_LEECHSEED_BATTLER | STATUS3_LEECHSEED | STATUS3_ALWAYS_HITS | STATUS3_PERISH_SONG | STATUS3_ROOTED | STATUS3_MUDSPORT | STATUS3_WATERSPORT);
        for (i = 0; i < gBattlersCount; ++i)
        {
            if (GetBattlerSide(gActiveBattler) != GetBattlerSide(i)
             && (gStatuses3[i] & STATUS3_ALWAYS_HITS) != 0
             && (gDisableStructs[i].battlerWithSureHit == gActiveBattler))
            {
                gStatuses3[i] &= ~(STATUS3_ALWAYS_HITS);
                gStatuses3[i] |= 0x10;
            }
        }
    }
    else
    {
        gBattleMons[gActiveBattler].status2 = 0;
        gStatuses3[gActiveBattler] = 0;
    }
    for (i = 0; i < gBattlersCount; ++i)
    {
        if (gBattleMons[i].status2 & STATUS2_INFATUATED_WITH(gActiveBattler))
            gBattleMons[i].status2 &= ~(STATUS2_INFATUATED_WITH(gActiveBattler));
        if ((gBattleMons[i].status2 & STATUS2_WRAPPED) && gBattleStruct->wrappedBy[i] == gActiveBattler)
            gBattleMons[i].status2 &= ~(STATUS2_WRAPPED);
    }
    gActionSelectionCursor[gActiveBattler] = 0;
    gMoveSelectionCursor[gActiveBattler] = 0;
    memset(&gDisableStructs[gActiveBattler], 0, sizeof(struct DisableStruct));
    if (gBattleMoves[gCurrentMove].effect == EFFECT_BATON_PASS)
    {
        gDisableStructs[gActiveBattler].substituteHP = disableStructCopy.substituteHP;
        gDisableStructs[gActiveBattler].battlerWithSureHit = disableStructCopy.battlerWithSureHit;
        gDisableStructs[gActiveBattler].perishSongTimer = disableStructCopy.perishSongTimer;
        gDisableStructs[gActiveBattler].battlerPreventingEscape = disableStructCopy.battlerPreventingEscape;
    }
    gMoveResultFlags = 0;
    gDisableStructs[gActiveBattler].isFirstTurn = 2;
	gDisableStructs[gActiveBattler].canProteanActivate = TRUE;
    gLastMoves[gActiveBattler] = MOVE_NONE;
    gLastLandedMoves[gActiveBattler] = MOVE_NONE;
    gLastHitByType[gActiveBattler] = 0;
    gLastResultingMoves[gActiveBattler] = MOVE_NONE;
    gLastPrintedMoves[gActiveBattler] = MOVE_NONE;
    gLastHitBy[gActiveBattler] = 0xFF;
	gBattleStruct->choicedMove[gActiveBattler] = MOVE_NONE;
    gBattleStruct->lastTakenMove[gActiveBattler] = MOVE_NONE;
	gBattleStruct->supremeOverlordBoosts[gActiveBattler] = 0;
	
	for (i = 0; i < MAX_BATTLERS_COUNT; i++)
		gBattleStruct->lastTakenMoveFrom[gActiveBattler][i] = MOVE_NONE;
	
    for (i = 0; i < gBattlersCount; ++i)
    {
        if (i != gActiveBattler)
            gBattleStruct->lastTakenMove[i] = MOVE_NONE;
        
		gBattleStruct->lastTakenMoveFrom[i][gActiveBattler] = MOVE_NONE;
    }
    gBattleResources->flags->flags[gActiveBattler] = 0;
    gCurrentMove = MOVE_NONE;
}

void FaintClearSetData(void)
{
    s32 i;
	
	TryResetBattlerStatChanges(gActiveBattler);
	
    gBattleMons[gActiveBattler].status2 = 0;
    gStatuses3[gActiveBattler] = 0;
	
    for (i = 0; i < gBattlersCount; ++i)
    {
        if ((gBattleMons[i].status2 & STATUS2_ESCAPE_PREVENTION) && gDisableStructs[i].battlerPreventingEscape == gActiveBattler)
            gBattleMons[i].status2 &= ~STATUS2_ESCAPE_PREVENTION;
        if (gBattleMons[i].status2 & STATUS2_INFATUATED_WITH(gActiveBattler))
            gBattleMons[i].status2 &= ~(STATUS2_INFATUATED_WITH(gActiveBattler));
        if ((gBattleMons[i].status2 & STATUS2_WRAPPED) && gBattleStruct->wrappedBy[i] == gActiveBattler)
            gBattleMons[i].status2 &= ~(STATUS2_WRAPPED);
    }
    gActionSelectionCursor[gActiveBattler] = 0;
    gMoveSelectionCursor[gActiveBattler] = 0;
	
    memset(&gDisableStructs[gActiveBattler], 0, sizeof(struct DisableStruct));
	gDisableStructs[gActiveBattler].isFirstTurn = 2;
	
    gProtectStructs[gActiveBattler].protected = FALSE;
    gProtectStructs[gActiveBattler].endured = FALSE;
    gProtectStructs[gActiveBattler].noValidMoves = FALSE;
    gProtectStructs[gActiveBattler].helpingHand = FALSE;
    gProtectStructs[gActiveBattler].bounceMove = FALSE;
    gProtectStructs[gActiveBattler].stealMove = FALSE;
    gProtectStructs[gActiveBattler].prlzImmobility = FALSE;
    gProtectStructs[gActiveBattler].confusionSelfDmg = FALSE;
    gProtectStructs[gActiveBattler].targetNotAffected = FALSE;
    gProtectStructs[gActiveBattler].chargingTurn = FALSE;
    gProtectStructs[gActiveBattler].fleeFlag = 0;
    gProtectStructs[gActiveBattler].usedImprisonedMove = FALSE;
    gProtectStructs[gActiveBattler].loveImmobility = FALSE;
    gProtectStructs[gActiveBattler].usedDisabledMove = FALSE;
    gProtectStructs[gActiveBattler].usedTauntedMove = FALSE;
    gProtectStructs[gActiveBattler].flinchImmobility = FALSE;
    gProtectStructs[gActiveBattler].notFirstStrike = FALSE;
	gProtectStructs[gActiveBattler].usesBouncedMove = FALSE;
	gProtectStructs[gActiveBattler].myceliumMightElevated = FALSE;
	gProtectStructs[gActiveBattler].pranksterElevated = FALSE;
	
    gLastMoves[gActiveBattler] = MOVE_NONE;
    gLastLandedMoves[gActiveBattler] = MOVE_NONE;
    gLastHitByType[gActiveBattler] = MOVE_NONE;
    gLastResultingMoves[gActiveBattler] = MOVE_NONE;
    gLastPrintedMoves[gActiveBattler] = MOVE_NONE;
    gLastHitBy[gActiveBattler] = 0xFF;
	
    gBattleStruct->choicedMove[gActiveBattler] = MOVE_NONE;
	gBattleStruct->lastTakenMove[gActiveBattler] = MOVE_NONE;
	gBattleStruct->supremeOverlordBoosts[gActiveBattler] = 0;
	
	for (i = 0; i < MAX_BATTLERS_COUNT; i++)
		gBattleStruct->lastTakenMoveFrom[gActiveBattler][i] = MOVE_NONE;
	
    for (i = 0; i < gBattlersCount; ++i)
    {
        if (i != gActiveBattler)
			gBattleStruct->lastTakenMove[i] = MOVE_NONE;

        gBattleStruct->lastTakenMoveFrom[i][gActiveBattler] = 0;
    }
    gBattleResources->flags->flags[gActiveBattler] = 0;
	
    gBattleMons[gActiveBattler].type1 = gBaseStats[gBattleMons[gActiveBattler].species].type1;
    gBattleMons[gActiveBattler].type2 = gBaseStats[gBattleMons[gActiveBattler].species].type2;
}

static void BattleIntroGetMonsData(void)
{
    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        gActiveBattler = gBattleCommunication[SPRITES_INIT_STATE1];
        BtlController_EmitGetMonData(0, REQUEST_ALL_BATTLE, 0);
        MarkBattlerForControllerExec(gActiveBattler);
        ++gBattleCommunication[MULTIUSE_STATE];
        break;
    case 1:
        if (gBattleControllerExecFlags == 0)
        {
            ++gBattleCommunication[SPRITES_INIT_STATE1];
            if (gBattleCommunication[SPRITES_INIT_STATE1] == gBattlersCount)
                gBattleMainFunc = BattleIntroPrepareBackgroundSlide;
            else
                gBattleCommunication[MULTIUSE_STATE] = 0;
        }
        break;
    }
}

static void BattleIntroPrepareBackgroundSlide(void)
{
    if (gBattleControllerExecFlags == 0)
    {
        gActiveBattler = GetBattlerAtPosition(0);
        BtlController_EmitIntroSlide(0, gBattleTerrain);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattleMainFunc = BattleIntroDrawTrainersOrMonsSprites;
        gBattleCommunication[MULTIUSE_STATE] = 0;
        gBattleCommunication[SPRITES_INIT_STATE1] = 0;
    }
}

static void BattleIntroDrawTrainersOrMonsSprites(void)
{
    s32 i;

    if (!gBattleControllerExecFlags)
    {
        for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
        {
            if ((gBattleTypeFlags & BATTLE_TYPE_SAFARI) && GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
				memset(&gBattleMons[gActiveBattler], 0, sizeof(struct BattlePokemon));
            else
            {
                u16 *hpOnSwitchout;
				
				memcpy(&gBattleMons[gActiveBattler], &gBattleBufferB[gActiveBattler][4], sizeof(struct BattlePokemon));
                gBattleMons[gActiveBattler].type1 = gBaseStats[gBattleMons[gActiveBattler].species].type1;
                gBattleMons[gActiveBattler].type2 = gBaseStats[gBattleMons[gActiveBattler].species].type2;
                gBattleMons[gActiveBattler].ability = GetAbilityBySpecies(gBattleMons[gActiveBattler].species, gBattleMons[gActiveBattler].abilityNum, gBattleMons[gActiveBattler].abilityHidden);
                hpOnSwitchout = &gBattleStruct->hpOnSwitchout[GetBattlerSide(gActiveBattler)];
                *hpOnSwitchout = gBattleMons[gActiveBattler].hp;
				TryResetBattlerStatChanges(gActiveBattler);
                gBattleMons[gActiveBattler].status2 = 0;
            }
			switch (GetBattlerPosition(gActiveBattler))
			{
				case B_POSITION_PLAYER_LEFT:
				    BtlController_EmitDrawTrainerPic(0);
					MarkBattlerForControllerExec(gActiveBattler);
					break;
				case B_POSITION_OPPONENT_LEFT:
				    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
					{
						BtlController_EmitDrawTrainerPic(0);
						MarkBattlerForControllerExec(gActiveBattler);
					}
					else
					{
						BtlController_EmitLoadMonSprite(0);
						MarkBattlerForControllerExec(gActiveBattler);
					}
					break;
				case B_POSITION_PLAYER_RIGHT:
				    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
					{
						BtlController_EmitDrawTrainerPic(0);
						MarkBattlerForControllerExec(gActiveBattler);
					}
					break;
				case B_POSITION_OPPONENT_RIGHT:
				    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
					{
						if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
						{
							BtlController_EmitDrawTrainerPic(0);
							MarkBattlerForControllerExec(gActiveBattler);
						}
					}
					else if (IsBattlerAlive(gActiveBattler))
					{
						BtlController_EmitLoadMonSprite(0);
						MarkBattlerForControllerExec(gActiveBattler);
					}
					break;
			}
			if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT)
			{
				if (gBattleTypeFlags & (BATTLE_TYPE_GHOST | BATTLE_TYPE_GHOST_UNVEILED))
				{
					if (!IS_BATTLE_TYPE_GHOST_WITHOUT_SCOPE())
						HandleSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[gActiveBattler].species), FLAG_SET_SEEN, gBattleMons[gActiveBattler].personality);
				}
				else if (!(gBattleTypeFlags & (BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_POKEDUDE | BATTLE_TYPE_LINK | BATTLE_TYPE_OLD_MAN_TUTORIAL | BATTLE_TYPE_LEGENDARY)))
					    HandleSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[gActiveBattler].species), FLAG_SET_SEEN, gBattleMons[gActiveBattler].personality);
			}
            if (GetBattlerPosition(gActiveBattler) == B_POSITION_PLAYER_LEFT)
            {
                BtlController_EmitDrawTrainerPic(0);
                MarkBattlerForControllerExec(gActiveBattler);
            }
        }
		for (i = 0; i < PARTY_SIZE; i++)
		{
			// try revert shaymin
			DoOverworldFormChange(&gPlayerParty[i], FORM_CHANGE_TIME);
			DoOverworldFormChange(&gEnemyParty[i], FORM_CHANGE_TIME);
			// save original species for battle form changes
			gBattleMonForms[i][B_SIDE_PLAYER] = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES);
			gBattleMonForms[i][B_SIDE_OPPONENT] = GetMonData(&gEnemyParty[i], MON_DATA_SPECIES);
			// transform xerneas into active form
			DoOverworldFormChange(&gPlayerParty[i], FORM_CHANGE_START_BATTLE);
			DoOverworldFormChange(&gEnemyParty[i], FORM_CHANGE_START_BATTLE);
		}
        gBattleMainFunc = BattleIntroDrawPartySummaryScreens;
    }
}

static void BattleIntroDrawPartySummaryScreens(void)
{
    s32 i;
    struct HpAndStatus hpStatus[PARTY_SIZE];

    if (!gBattleControllerExecFlags)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        {
            for (i = 0; i < PARTY_SIZE; ++i)
            {
                if (GetMonData(&gEnemyParty[i], MON_DATA_SPECIES2) == SPECIES_NONE
                 || GetMonData(&gEnemyParty[i], MON_DATA_SPECIES2) == SPECIES_EGG)
                {
                    hpStatus[i].hp = 0xFFFF;
                    hpStatus[i].status = 0;
                }
                else
                {
                    hpStatus[i].hp = GetMonData(&gEnemyParty[i], MON_DATA_HP);
                    hpStatus[i].status = GetMonData(&gEnemyParty[i], MON_DATA_STATUS);
                }
            }
            gActiveBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
            BtlController_EmitDrawPartyStatusSummary(0, hpStatus, 0x80);
            MarkBattlerForControllerExec(gActiveBattler);
            for (i = 0; i < PARTY_SIZE; ++i)
            {
                if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2) == SPECIES_NONE
                 || GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2) == SPECIES_EGG)
                {
                    hpStatus[i].hp = 0xFFFF;
                    hpStatus[i].status = 0;
                }
                else
                {
                    hpStatus[i].hp = GetMonData(&gPlayerParty[i], MON_DATA_HP);
                    hpStatus[i].status = GetMonData(&gPlayerParty[i], MON_DATA_STATUS);
                }
            }
            gActiveBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
            BtlController_EmitDrawPartyStatusSummary(0, hpStatus, 0x80);
            MarkBattlerForControllerExec(gActiveBattler);

            gBattleMainFunc = BattleIntroPrintTrainerWantsToBattle;
        }
        else
        {
            // The struct gets set here, but nothing is ever done with it since
            // wild battles don't show the party summary.
            // Still, there's no point in having dead code.
            for (i = 0; i < PARTY_SIZE; ++i)
            {
                if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2) == SPECIES_NONE
                 || GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2) == SPECIES_EGG)
                {
                    hpStatus[i].hp = 0xFFFF;
                    hpStatus[i].status = 0;
                }
                else
                {
                    hpStatus[i].hp = GetMonData(&gPlayerParty[i], MON_DATA_HP);
                    hpStatus[i].status = GetMonData(&gPlayerParty[i], MON_DATA_STATUS);
                }
            }
            gBattleMainFunc = BattleIntroPrintWildMonAttacked;
        }
    }
}

static void BattleIntroPrintTrainerWantsToBattle(void)
{
    if (!gBattleControllerExecFlags)
    {
        gActiveBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
        PrepareStringBattle(STRINGID_INTROMSG, gActiveBattler);
        gBattleMainFunc = BattleIntroPrintOpponentSendsOut;
    }
}

static void BattleIntroPrintWildMonAttacked(void)
{
    if (!gBattleControllerExecFlags)
    {
        gBattleMainFunc = BattleIntroPrintPlayerSendsOut;
        PrepareStringBattle(STRINGID_INTROMSG, 0);
        if (IS_BATTLE_TYPE_GHOST_WITH_SCOPE())
        {
            gBattleScripting.battler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
            BattleScriptExecute(BattleScript_SilphScopeUnveiled);
        }
    }
}

static void BattleIntroPrintOpponentSendsOut(void)
{
    if (!gBattleControllerExecFlags)
    {
        PrepareStringBattle(STRINGID_INTROSENDOUT, GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT));
        gBattleMainFunc = BattleIntroOpponentSendsOutMonAnimation;
    }
}

static void BattleIntroOpponentSendsOutMonAnimation(void)
{
    if (!gBattleControllerExecFlags)
    {
        for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
        {
            if (GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_LEFT)
            {
                BtlController_EmitIntroTrainerBallThrow(0);
                MarkBattlerForControllerExec(gActiveBattler);
            }
            if (gBattleTypeFlags & BATTLE_TYPE_MULTI && GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_RIGHT)
            {
                BtlController_EmitIntroTrainerBallThrow(0);
                MarkBattlerForControllerExec(gActiveBattler);
            }
        }
        gBattleMainFunc = BattleIntroRecordMonsToDex;
    }
}

static void BattleIntroRecordMonsToDex(void)
{
    if (!gBattleControllerExecFlags)
    {
        for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
		{
            if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT && !(gBattleTypeFlags & (BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_POKEDUDE | BATTLE_TYPE_LINK
			| BATTLE_TYPE_GHOST | BATTLE_TYPE_OLD_MAN_TUTORIAL | BATTLE_TYPE_LEGENDARY)))
                HandleSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[gActiveBattler].species), FLAG_SET_SEEN, gBattleMons[gActiveBattler].personality);
        }
		gBattleMainFunc = BattleIntroPrintPlayerSendsOut;
    }
}

void BattleIntroPrintPlayerSendsOut(void)
{
    if (!gBattleControllerExecFlags)
    {
        if (!(gBattleTypeFlags & BATTLE_TYPE_SAFARI))
            PrepareStringBattle(STRINGID_INTROSENDOUT, GetBattlerAtPosition(B_POSITION_PLAYER_LEFT));
        gBattleMainFunc = BattleIntroPlayerSendsOutMonAnimation;
    }
}

static void BattleIntroPlayerSendsOutMonAnimation(void)
{
    if (!gBattleControllerExecFlags)
    {
        for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
        {
            if (GetBattlerPosition(gActiveBattler) == B_POSITION_PLAYER_LEFT)
            {
                BtlController_EmitIntroTrainerBallThrow(0);
                MarkBattlerForControllerExec(gActiveBattler);
            }
            if (gBattleTypeFlags & BATTLE_TYPE_MULTI && GetBattlerPosition(gActiveBattler) == B_POSITION_PLAYER_RIGHT)
            {
                BtlController_EmitIntroTrainerBallThrow(0);
                MarkBattlerForControllerExec(gActiveBattler);
            }
        }
        gBattleStruct->switchInAbilitiesCounter = 0;
        gBattleStruct->switchInItemsCounter = 0;
        gBattleStruct->overworldWeatherDone = FALSE;
        gBattleMainFunc = TryDoEventsBeforeFirstTurn;
    }
}

static void TryDoEventsBeforeFirstTurn(void)
{
    s32 i, j;

    if (!gBattleControllerExecFlags)
    {
        if (gBattleStruct->switchInAbilitiesCounter == 0)
        {
            for (i = 0; i < gBattlersCount; ++i)
                gBattlerByTurnOrder[i] = i;
            for (i = 0; i < gBattlersCount - 1; ++i)
                for (j = i + 1; j < gBattlersCount; ++j)
                    if (GetWhoStrikesFirst(gBattlerByTurnOrder[i], gBattlerByTurnOrder[j], TRUE) != ATTACKER_STRIKES_FIRST)
                        SwapTurnOrder(i, j);
        }
        if (!gBattleStruct->overworldWeatherDone && AbilityBattleEffects(ABILITYEFFECT_SWITCH_IN_WEATHER, 0, MOVE_NONE))
        {
            gBattleStruct->overworldWeatherDone = TRUE;
            return;
        }
		// check Neutralizing Gas
		if (AbilityBattleEffects(ABILITYEFFECT_NEUTRALIZING_GAS, 0, MOVE_NONE))
			return;
		// check Unnerve
		if (AbilityBattleEffects(ABILITYEFFECT_UNNERVE, 0, MOVE_NONE))
			return;
        // Check all switch in abilities happening from the fastest mon to slowest.
        while (gBattleStruct->switchInAbilitiesCounter < gBattlersCount)
        {
            if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, gBattlerByTurnOrder[gBattleStruct->switchInAbilitiesCounter++], MOVE_NONE))
                return;
        }
        if (AbilityBattleEffects(ABILITYEFFECT_TRACE1, 0, MOVE_NONE))
            return;
        // Check all switch in items having effect from the fastest mon to slowest.
        while (gBattleStruct->switchInItemsCounter < gBattlersCount)
        {
            if (ItemBattleEffects(ITEMEFFECT_ON_SWITCH_IN, gBattlerByTurnOrder[gBattleStruct->switchInItemsCounter++], FALSE))
                return;
        }
        for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
        {
            gBattleStruct->monToSwitchIntoId[i] = PARTY_SIZE;
            gChosenActionByBattler[i] = B_ACTION_NONE;
            gChosenMoveByBattler[i] = MOVE_NONE;
			if (IsBattlerAlive(i) && GetBattlerSide(i) == B_SIDE_PLAYER)
				gBattleStruct->appearedInBattle |= gBitTable[gBattlerPartyIndexes[i]];
        }
        TurnValuesCleanUp(FALSE);
        memset(&gSpecialStatuses, 0, sizeof(gSpecialStatuses));
        gBattleStruct->absentBattlerFlags = gAbsentBattlerFlags;
        gBattleMainFunc = HandleTurnActionSelectionState;
        ResetSentPokesToOpponentValue();
        for (i = 0; i < BATTLE_COMMUNICATION_ENTRIES_COUNT; ++i)
            gBattleCommunication[i] = 0;
        for (i = 0; i < gBattlersCount; ++i)
            gBattleMons[i].status2 &= ~(STATUS2_FLINCHED);
        gBattleStruct->turnEffectsTracker = 0;
        gBattleStruct->turnEffectsBattlerId = 0;
        gBattleStruct->wishPerishSongState = 0;
        gBattleStruct->wishPerishSongBattlerId = 0;
        gBattleScripting.atk49_state = 0;
        gBattleStruct->faintedActionsState = 0;
        gBattleStruct->turnCountersTracker = 0;
        gMoveResultFlags = 0;
        gRandomTurnNumber = Random();
		
		if (ShouldDoTrainerSlide(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), TRAINER_SLIDE_FIRST_MON_SEND_OUT))
			BattleScriptExecute(BattleScript_TrainerSlideMsgEnd2);
    }
}

static void HandleEndTurn_ContinueBattle(void)
{
    s32 i;

    if (!gBattleControllerExecFlags)
    {
        gBattleMainFunc = BattleTurnPassed;
        for (i = 0; i < BATTLE_COMMUNICATION_ENTRIES_COUNT; ++i)
            gBattleCommunication[i] = 0;
        for (i = 0; i < gBattlersCount; ++i)
        {
            gBattleMons[i].status2 &= ~(STATUS2_FLINCHED);
            if ((gBattleMons[i].status1 & STATUS1_SLEEP) && (gBattleMons[i].status2 & STATUS2_MULTIPLETURNS))
                CancelMultiTurnMoves(i);
        }
        gBattleStruct->turnEffectsTracker = 0;
        gBattleStruct->turnEffectsBattlerId = 0;
        gBattleStruct->wishPerishSongState = 0;
        gBattleStruct->wishPerishSongBattlerId = 0;
        gBattleStruct->turnCountersTracker = 0;
        gMoveResultFlags = 0;
    }
}

void BattleTurnPassed(void)
{
    s32 i;

    TurnValuesCleanUp(TRUE);
    if (gBattleOutcome == 0)
    {
        if (DoFieldEndTurnEffects() || DoBattlerEndTurnEffects())
            return;
    }
    if (HandleFaintedMonActions())
        return;
    gBattleStruct->faintedActionsState = 0;
    if (HandleWishPerishSongOnTurnEnd())
        return;
    TurnValuesCleanUp(FALSE);
    gHitMarker &= ~(HITMARKER_NO_ATTACKSTRING | HITMARKER_UNABLE_TO_USE_MOVE | HITMARKER_PLAYER_FAINTED | HITMARKER_PASSIVE_DAMAGE);
    gBattleScripting.animTurn = 0;
    gBattleScripting.animTargetsHit = 0;
    gBattleScripting.atk49_state = 0;
    gBattleMoveDamage = 0;
    gMoveResultFlags = 0;
    for (i = 0; i < 5; ++i)
        gBattleCommunication[i] = 0;
    if (gBattleOutcome != 0)
    {
        gCurrentActionFuncId = B_ACTION_FINISHED;
        gBattleMainFunc = RunTurnActionsFunctions;
        return;
    }
    if (gBattleStruct->battleTurnCounter < 0xFF)
        ++gBattleStruct->battleTurnCounter;
    for (i = 0; i < gBattlersCount; ++i)
    {
        gChosenActionByBattler[i] = B_ACTION_NONE;
        gChosenMoveByBattler[i] = MOVE_NONE;
		gBattleStruct->pickupStack[i] = 0xFF;
    }
    for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
	{
        gBattleStruct->monToSwitchIntoId[i] = PARTY_SIZE;
		gBattleStruct->targetsDone[i] = 0;
	}
    gBattleStruct->absentBattlerFlags = gAbsentBattlerFlags;
	gBattleStruct->throwingPokeBall = FALSE;
    gBattleMainFunc = HandleTurnActionSelectionState;
    gRandomTurnNumber = Random();
	
    if (ShouldDoTrainerSlide(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), TRAINER_SLIDE_LAST_MON_LOW_HP))
	    BattleScriptExecute(BattleScript_TrainerSlideMsgEnd2);
	else if (ShouldDoTrainerSlide(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), TRAINER_SLIDE_FIRST_SUPER_EFFECTIVE_HIT_TAKEN))
		BattleScriptExecute(BattleScript_TrainerSlideMsgEnd2);
	else if (ShouldDoTrainerSlide(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), TRAINER_SLIDE_FIRST_CRITICAL_TAKEN))
		BattleScriptExecute(BattleScript_TrainerSlideMsgEnd2);
}

u8 IsRunningFromBattleImpossible(void)
{
    u8 ret, holdEffect = GetBattlerItemHoldEffect(gActiveBattler, TRUE);
	
	if (GetBattlerPosition(gActiveBattler) == B_POSITION_PLAYER_RIGHT && IS_DOUBLE_WILD_BATTLE() && IsBattlerAlive(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)))
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
		return BATTLE_RUN_FAILURE;
	}
    if (holdEffect == HOLD_EFFECT_CAN_ALWAYS_RUN || (gBattleTypeFlags & BATTLE_TYPE_LINK) || GetBattlerAbility(gActiveBattler) == ABILITY_RUN_AWAY)
        return BATTLE_RUN_SUCCESS;
	
	ret = IsAbilityPreventingSwitchOut(gActiveBattler);
	
	if (ret)
	{
		gBattleScripting.battler = ret;
		gBattleCommunication[MULTISTRING_CHOOSER] = 2;
		return BATTLE_RUN_FAILURE;
	}
    if (!CanBattlerEscape(gActiveBattler))
    {
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        return BATTLE_RUN_FORBIDDEN;
    }
    if (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE)
    {
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
        return BATTLE_RUN_FORBIDDEN;
    }
    return BATTLE_RUN_SUCCESS;
}

void UpdatePartyOwnerOnSwitch_NonMulti(u8 battler)
{
    s32 i;
    u8 r4, r1;

    for (i = 0; i < 3; ++i)
        gBattlePartyCurrentOrder[i] = *(battler * 3 + i + (u8 *)(gBattleStruct->battlerPartyOrders));
    r4 = GetPartyIdFromBattlePartyId(gBattlerPartyIndexes[battler]);
    r1 = GetPartyIdFromBattlePartyId(gBattleStruct->monToSwitchIntoId[battler]);
    SwitchPartyMonSlots(r4, r1);
    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        for (i = 0; i < 3; ++i)
        {
            *(battler * 3 + i + (u8 *)(gBattleStruct->battlerPartyOrders)) = gBattlePartyCurrentOrder[i];
            *(BATTLE_PARTNER(battler) * 3 + i + (u8 *)(gBattleStruct->battlerPartyOrders)) = gBattlePartyCurrentOrder[i];
        }
    }
    else
    {
        for (i = 0; i < 3; ++i)
            *(battler * 3 + i + (u8 *)(gBattleStruct->battlerPartyOrders)) = gBattlePartyCurrentOrder[i];
    }
}

enum
{
    STATE_BEFORE_ACTION_CHOSEN,
    STATE_WAIT_ACTION_CHOSEN,
    STATE_WAIT_ACTION_CASE_CHOSEN,
    STATE_WAIT_ACTION_CONFIRMED_STANDBY,
    STATE_WAIT_ACTION_CONFIRMED,
    STATE_SELECTION_SCRIPT,
    STATE_WAIT_SET_BEFORE_ACTION,
};

static void HandleTurnActionSelectionState(void)
{
    s32 i;

    gBattleCommunication[ACTIONS_CONFIRMED_COUNT] = 0;
    for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
    {
        u8 position = GetBattlerPosition(gActiveBattler);

        switch (gBattleCommunication[gActiveBattler])
        {
        case STATE_BEFORE_ACTION_CHOSEN: // Choose an action.
            gBattleStruct->monToSwitchIntoId[gActiveBattler] = PARTY_SIZE;
            if (gBattleTypeFlags & BATTLE_TYPE_MULTI || (position & BIT_FLANK) == B_FLANK_LEFT
             || gBattleStruct->absentBattlerFlags & gBitTable[GetBattlerAtPosition(BATTLE_PARTNER(position))]
             || gBattleCommunication[GetBattlerAtPosition(BATTLE_PARTNER(position))] == STATE_WAIT_ACTION_CONFIRMED)
            {
                if (gBattleStruct->absentBattlerFlags & gBitTable[gActiveBattler])
                {
                    gChosenActionByBattler[gActiveBattler] = B_ACTION_NOTHING_FAINTED;
					gBattleCommunication[gActiveBattler] = (gBattleTypeFlags & BATTLE_TYPE_MULTI) ? STATE_WAIT_ACTION_CONFIRMED_STANDBY : STATE_WAIT_ACTION_CONFIRMED;
                }
                else
                {
                    if (gBattleMons[gActiveBattler].status2 & (STATUS2_MULTIPLETURNS | STATUS2_RECHARGE))
                    {
                        gChosenActionByBattler[gActiveBattler] = B_ACTION_USE_MOVE;
                        gBattleCommunication[gActiveBattler] = STATE_WAIT_ACTION_CONFIRMED_STANDBY;
                    }
					else if (IS_DOUBLE_WILD_BATTLE() && position == B_POSITION_PLAYER_RIGHT && (gBattleStruct->throwingPokeBall || gChosenActionByBattler[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)] == B_ACTION_RUN)
					&& gChosenActionByBattler[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)] != B_ACTION_NOTHING_FAINTED)
					{
						gBattleStruct->throwingPokeBall = FALSE;
						gChosenActionByBattler[gActiveBattler] = B_ACTION_NOTHING_FAINTED;
						gBattleCommunication[gActiveBattler] = STATE_WAIT_ACTION_CONFIRMED_STANDBY;
					}
                    else
                    {
						gBattleStruct->itemPartyIndex[gActiveBattler] = PARTY_SIZE;
                        BtlController_EmitChooseAction(0, gChosenActionByBattler[0], gBattleBufferB[0][1] | (gBattleBufferB[0][2] << 8));
                        MarkBattlerForControllerExec(gActiveBattler);
                        ++gBattleCommunication[gActiveBattler];
                    }
                }
            }
            break;
        case STATE_WAIT_ACTION_CHOSEN: // Try to perform an action.
            if (!(gBattleControllerExecFlags & ((gBitTable[gActiveBattler]) | (0xF0000000) | (gBitTable[gActiveBattler] << 4) | (gBitTable[gActiveBattler] << 8) | (gBitTable[gActiveBattler] << 0xC))))
            {
                gChosenActionByBattler[gActiveBattler] = gBattleBufferB[gActiveBattler][1];
                switch (gBattleBufferB[gActiveBattler][1])
                {
                case B_ACTION_USE_MOVE:
                    if (AreAllMovesUnusable())
                    {
                        gBattleCommunication[gActiveBattler] = STATE_SELECTION_SCRIPT;
                        gBattleStruct->selectionScriptFinished &= ~(gBitTable[gActiveBattler]);
                        *(gBattleStruct->stateIdAfterSelScript + gActiveBattler) = STATE_WAIT_ACTION_CONFIRMED_STANDBY;
                        *(gBattleStruct->moveTarget + gActiveBattler) = gBattleBufferB[gActiveBattler][3];
                        return;
                    }
                    else if (gDisableStructs[gActiveBattler].encoredMove != MOVE_NONE)
                    {
                        gChosenMoveByBattler[gActiveBattler] = gDisableStructs[gActiveBattler].encoredMove;
                        *(gBattleStruct->chosenMovePositions + gActiveBattler) = gDisableStructs[gActiveBattler].encoredMovePos;
                        gBattleCommunication[gActiveBattler] = STATE_WAIT_ACTION_CONFIRMED_STANDBY;
                        return;
                    }
                    else
                    {
                        struct ChooseMoveStruct moveInfo;

                        moveInfo.species = gBattleMons[gActiveBattler].species;
                        moveInfo.monType1 = gBattleMons[gActiveBattler].type1;
                        moveInfo.monType2 = gBattleMons[gActiveBattler].type2;
                        for (i = 0; i < MAX_MON_MOVES; ++i)
                        {
                            moveInfo.moves[i] = gBattleMons[gActiveBattler].moves[i];
                            moveInfo.currentPp[i] = gBattleMons[gActiveBattler].pp[i];
                            moveInfo.maxPp[i] = CalculatePPWithBonus(gBattleMons[gActiveBattler].moves[i], gBattleMons[gActiveBattler].ppBonuses, i);
                        }
                        BtlController_EmitChooseMove(0, (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) != 0, FALSE, &moveInfo);
                        MarkBattlerForControllerExec(gActiveBattler);
                    }
                    break;
                case B_ACTION_USE_ITEM:
                    if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_EREADER_TRAINER))
                    {
                        gSelectionBattleScripts[gActiveBattler] = BattleScript_ActionSelectionItemsCantBeUsed;
                        gBattleCommunication[gActiveBattler] = STATE_SELECTION_SCRIPT;
                        gBattleStruct->selectionScriptFinished &= ~(gBitTable[gActiveBattler]);
                        *(gBattleStruct->stateIdAfterSelScript + gActiveBattler) = STATE_BEFORE_ACTION_CHOSEN;
                        return;
                    }
                    else
                    {
                        BtlController_EmitChooseItem(0, gBattleStruct->battlerPartyOrders[gActiveBattler]);
                        MarkBattlerForControllerExec(gActiveBattler);
                    }
                    break;
                case B_ACTION_SWITCH:
                    *(gBattleStruct->battlerPartyIndexes + gActiveBattler) = gBattlerPartyIndexes[gActiveBattler];
                    if (!CanBattlerEscape(gActiveBattler))
                    {
                        BtlController_EmitChoosePokemon(0, PARTY_ACTION_CANT_SWITCH, 6, ABILITY_NONE, gBattleStruct->battlerPartyOrders[gActiveBattler]);
                    }
					else if (IsAbilityPreventingSwitchOut(gActiveBattler))
					{
						BtlController_EmitChoosePokemon(0, ((IsAbilityPreventingSwitchOut(gActiveBattler) - 1) << 4) | PARTY_ACTION_ABILITY_PREVENTS, 6, gLastUsedAbility, gBattleStruct->battlerPartyOrders[gActiveBattler]);
					}
                    else
                    {
                        if (gActiveBattler == 2 && gChosenActionByBattler[0] == B_ACTION_SWITCH)
                            BtlController_EmitChoosePokemon(0, PARTY_ACTION_CHOOSE_MON, gBattleStruct->monToSwitchIntoId[0], ABILITY_NONE, gBattleStruct->battlerPartyOrders[gActiveBattler]);
                        else if (gActiveBattler == 3 && gChosenActionByBattler[1] == B_ACTION_SWITCH)
                            BtlController_EmitChoosePokemon(0, PARTY_ACTION_CHOOSE_MON, gBattleStruct->monToSwitchIntoId[1], ABILITY_NONE, gBattleStruct->battlerPartyOrders[gActiveBattler]);
                        else
                            BtlController_EmitChoosePokemon(0, PARTY_ACTION_CHOOSE_MON, 6, ABILITY_NONE, gBattleStruct->battlerPartyOrders[gActiveBattler]);
                    }
                    MarkBattlerForControllerExec(gActiveBattler);
                    break;
                case B_ACTION_SAFARI_BALL:
                    if (IsPlayerPartyAndPokemonStorageFull())
                    {
                        gSelectionBattleScripts[gActiveBattler] = BattleScript_PrintFullBox;
                        gBattleCommunication[gActiveBattler] = STATE_SELECTION_SCRIPT;
                        gBattleStruct->selectionScriptFinished &= ~(gBitTable[gActiveBattler]);
                        *(gBattleStruct->stateIdAfterSelScript + gActiveBattler) = STATE_BEFORE_ACTION_CHOSEN;
                        return;
                    }
                    break;
                case B_ACTION_CANCEL_PARTNER:
                    gBattleCommunication[gActiveBattler] = STATE_WAIT_SET_BEFORE_ACTION;
                    gBattleCommunication[GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(gActiveBattler)))] = STATE_BEFORE_ACTION_CHOSEN;
                    BtlController_EmitEndBounceEffect(0);
                    MarkBattlerForControllerExec(gActiveBattler);
                    return;
                }
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER && !(gBattleTypeFlags & BATTLE_TYPE_LINK) && gBattleBufferB[gActiveBattler][1] == B_ACTION_RUN)
                {
                    BattleScriptExecute(BattleScript_PrintCantRunFromTrainer);
                    gBattleCommunication[gActiveBattler] = STATE_BEFORE_ACTION_CHOSEN;
                }
                else if (IsRunningFromBattleImpossible() != BATTLE_RUN_SUCCESS && gBattleBufferB[gActiveBattler][1] == B_ACTION_RUN)
                {
					gSelectionBattleScripts[gActiveBattler] = BattleScript_PrintCantEscapeFromBattle;
                    gBattleCommunication[gActiveBattler] = STATE_SELECTION_SCRIPT;
                    gBattleStruct->selectionScriptFinished &= ~(gBitTable[gActiveBattler]);
                    *(gBattleStruct->stateIdAfterSelScript + gActiveBattler) = STATE_BEFORE_ACTION_CHOSEN;
                    return;
                }
                else
                    ++gBattleCommunication[gActiveBattler];
            }
            break;
        case STATE_WAIT_ACTION_CASE_CHOSEN:
            if (!(gBattleControllerExecFlags & ((gBitTable[gActiveBattler]) | (0xF0000000) | (gBitTable[gActiveBattler] << 4) | (gBitTable[gActiveBattler] << 8) | (gBitTable[gActiveBattler] << 0xC))))
            {
                switch (gChosenActionByBattler[gActiveBattler])
                {
                case B_ACTION_USE_MOVE:
                    switch (gBattleBufferB[gActiveBattler][1])
                    {
                    case 3 ... 9:
                        gChosenActionByBattler[gActiveBattler] = gBattleBufferB[gActiveBattler][1];
                        return;
                    default:
                        if ((gBattleBufferB[gActiveBattler][2] | (gBattleBufferB[gActiveBattler][3] << 8)) == 0xFFFF)
                        {
                            gBattleCommunication[gActiveBattler] = STATE_BEFORE_ACTION_CHOSEN;
                        }
                        else if (TrySetCantSelectMoveBattleScript())
                        {
                            gBattleCommunication[gActiveBattler] = STATE_SELECTION_SCRIPT;
                            gBattleStruct->selectionScriptFinished &= ~(gBitTable[gActiveBattler]);
                            gBattleBufferB[gActiveBattler][1] = 0;
                            *(gBattleStruct->stateIdAfterSelScript + gActiveBattler) = STATE_WAIT_ACTION_CHOSEN;
                            return;
                        }
                        else
                        {
                            *(gBattleStruct->chosenMovePositions + gActiveBattler) = gBattleBufferB[gActiveBattler][2];
                            gChosenMoveByBattler[gActiveBattler] = gBattleMons[gActiveBattler].moves[*(gBattleStruct->chosenMovePositions + gActiveBattler)];
                            *(gBattleStruct->moveTarget + gActiveBattler) = gBattleBufferB[gActiveBattler][3];
                            ++gBattleCommunication[gActiveBattler];
                        }
                        break;
                    }
                    break;
                case B_ACTION_USE_ITEM:
                    if ((gBattleBufferB[gActiveBattler][1] | (gBattleBufferB[gActiveBattler][2] << 8)) == 0)
                    {
                        gBattleCommunication[gActiveBattler] = STATE_BEFORE_ACTION_CHOSEN;
                    }
                    else
                    {
                        gLastUsedItem = (gBattleBufferB[gActiveBattler][1] | (gBattleBufferB[gActiveBattler][2] << 8));
						if (ItemId_GetPocket(gLastUsedItem) == POCKET_POKE_BALLS)
                            gBattleStruct->throwingPokeBall = TRUE;
                        ++gBattleCommunication[gActiveBattler];
                    }
                    break;
                case B_ACTION_SWITCH:
                    if (gBattleBufferB[gActiveBattler][1] == PARTY_SIZE)
                    {
                        gBattleCommunication[gActiveBattler] = STATE_BEFORE_ACTION_CHOSEN;
                    }
                    else
                    {
                        gBattleStruct->monToSwitchIntoId[gActiveBattler] = gBattleBufferB[gActiveBattler][1];
                        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                        {
                            *(gActiveBattler * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) &= 0xF;
                            *(gActiveBattler * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) |= (gBattleBufferB[gActiveBattler][2] & 0xF0);
                            *(gActiveBattler * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 1) = gBattleBufferB[gActiveBattler][3];
                            *((BATTLE_PARTNER(gActiveBattler)) * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) &= (0xF0);
                            *((BATTLE_PARTNER(gActiveBattler)) * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) |= (gBattleBufferB[gActiveBattler][2] & 0xF0) >> 4;
                            *((BATTLE_PARTNER(gActiveBattler)) * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 2) = gBattleBufferB[gActiveBattler][3];
                        }
                        ++gBattleCommunication[gActiveBattler];
                    }
                    break;
                case B_ACTION_RUN:
                    gHitMarker |= HITMARKER_RUN;
                    ++gBattleCommunication[gActiveBattler];
                    break;
                case B_ACTION_SAFARI_WATCH_CAREFULLY:
                    ++gBattleCommunication[gActiveBattler];
                    break;
                case B_ACTION_SAFARI_BALL:
                    ++gBattleCommunication[gActiveBattler];
                    break;
                case B_ACTION_SAFARI_BAIT:
                case B_ACTION_SAFARI_GO_NEAR:
                    ++gBattleCommunication[gActiveBattler];
                    break;
                case B_ACTION_SAFARI_RUN:
                    gHitMarker |= HITMARKER_RUN;
                    ++gBattleCommunication[gActiveBattler];
                    break;
                case B_ACTION_OLDMAN_THROW:
                    ++gBattleCommunication[gActiveBattler];
                    break;
                }
            }
            break;
        case STATE_WAIT_ACTION_CONFIRMED_STANDBY:
            if (!(gBattleControllerExecFlags & ((gBitTable[gActiveBattler]) | (0xF0000000) | (gBitTable[gActiveBattler] << 4) | (gBitTable[gActiveBattler] << 8) | (gBitTable[gActiveBattler] << 0xC))))
            {
                if (((gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_DOUBLE)) != BATTLE_TYPE_DOUBLE) || (position & BIT_FLANK) != B_FLANK_LEFT
                 || (gBattleStruct->absentBattlerFlags & gBitTable[GetBattlerAtPosition(BATTLE_PARTNER(position))]))
                    BtlController_EmitLinkStandbyMsg(0, 0);
                else
                    BtlController_EmitLinkStandbyMsg(0, 1);
                MarkBattlerForControllerExec(gActiveBattler);
                ++gBattleCommunication[gActiveBattler];
            }
            break;
        case STATE_WAIT_ACTION_CONFIRMED:
            if (!(gBattleControllerExecFlags & ((gBitTable[gActiveBattler]) | (0xF0000000) | (gBitTable[gActiveBattler] << 4) | (gBitTable[gActiveBattler] << 8) | (gBitTable[gActiveBattler] << 0xC))))
                ++gBattleCommunication[ACTIONS_CONFIRMED_COUNT];
            break;
        case STATE_SELECTION_SCRIPT:
            if (gBattleStruct->selectionScriptFinished & gBitTable[gActiveBattler])
            {
                gBattleCommunication[gActiveBattler] = *(gBattleStruct->stateIdAfterSelScript + gActiveBattler);
            }
            else
            {
                gBattlerAttacker = gActiveBattler;
                gBattlescriptCurrInstr = gSelectionBattleScripts[gActiveBattler];
                if (!(gBattleControllerExecFlags & ((gBitTable[gActiveBattler]) | (0xF0000000) | (gBitTable[gActiveBattler] << 4) | (gBitTable[gActiveBattler] << 8) | (gBitTable[gActiveBattler] << 0xC))))
                    gBattleScriptingCommandsTable[gBattlescriptCurrInstr[0]]();
                gSelectionBattleScripts[gActiveBattler] = gBattlescriptCurrInstr;
            }
            break;
        case STATE_WAIT_SET_BEFORE_ACTION:
            if (!(gBattleControllerExecFlags & ((gBitTable[gActiveBattler]) | (0xF0000000) | (gBitTable[gActiveBattler] << 4) | (gBitTable[gActiveBattler] << 8) | (gBitTable[gActiveBattler] << 0xC))))
                gBattleCommunication[gActiveBattler] = STATE_BEFORE_ACTION_CHOSEN;
            break;
        }
    }
    // Check if everyone chose actions.
    if (gBattleCommunication[ACTIONS_CONFIRMED_COUNT] == gBattlersCount)
	{
		if (IS_DOUBLE_WILD_BATTLE() && gBattleStruct->throwingPokeBall && gChosenActionByBattler[GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)] != B_ACTION_NOTHING_FAINTED)
			gChosenActionByBattler[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)] = B_ACTION_NOTHING_FAINTED;
		
        gBattleMainFunc = SetActionsAndBattlersTurnOrder;
	}
}

void SwapTurnOrder(u8 id1, u8 id2)
{
    u32 temp;

    SWAP(gActionsByTurnOrder[id1], gActionsByTurnOrder[id2], temp);
    SWAP(gBattlerByTurnOrder[id1], gBattlerByTurnOrder[id2], temp);
}

u8 GetWhoStrikesFirst(u8 battler1, u8 battler2, bool8 ignoreChosenMoves)
{
    s8 battler1Priority, battler2Priority;
    s32 battler1Bracket, battler2Bracket;
    u32 battler1Speed, battler2Speed;
  
    // priority check
    if (!ignoreChosenMoves) 
    {
		battler1Priority = battler2Priority = 0;
		
		if (gChosenActionByBattler[battler1] == B_ACTION_USE_MOVE)
			battler1Priority = GetChosenMovePriority(battler1);
		
		if (gChosenActionByBattler[battler2] == B_ACTION_USE_MOVE)
			battler2Priority = GetChosenMovePriority(battler2);
        
        if (battler1Priority > battler2Priority) 
            return ATTACKER_STRIKES_FIRST;
        else if (battler1Priority < battler2Priority)
            return DEFENDER_STRIKES_FIRST;
    }
    
    // bracket check
    battler1Bracket = GetBattlerBracket(battler1);
    battler2Bracket = GetBattlerBracket(battler2);
    
    if (battler1Bracket > battler2Bracket) 
        return ATTACKER_STRIKES_FIRST;
    else if (battler1Bracket < battler2Bracket)
        return DEFENDER_STRIKES_FIRST;
    
    // speed check
    battler1Speed = GetBattlerTotalSpeed(battler1);
    battler2Speed = GetBattlerTotalSpeed(battler2);
    
    if (battler1Speed > battler2Speed)
        return ATTACKER_STRIKES_FIRST;
    else if (battler1Speed < battler2Speed)
        return DEFENDER_STRIKES_FIRST;
    else
    {
        if ((Random() & 1))
            return ATTACKER_STRIKES_FIRST;
        else
            return SPEED_TIE;
    }
}

s8 GetChosenMovePriority(u8 battler)
{
	gProtectStructs[battler].pranksterElevated = FALSE;
	
	return GetMovePriority(battler, gProtectStructs[battler].noValidMoves ? MOVE_STRUGGLE : gBattleMons[battler].moves[*(gBattleStruct->chosenMovePositions + battler)]);
}

s8 GetMovePriority(u8 battler, u16 move)
{
    s8 priority = gBattleMoves[move].priority;
	
	switch (GetBattlerAbility(battler))
	{
		case ABILITY_PRANKSTER:
		    if (IS_MOVE_STATUS(move))
			{
				gProtectStructs[battler].pranksterElevated = TRUE;
				++priority;
			}
			break;
		case ABILITY_GALE_WINGS:
		    if (gBattleMoves[move].type == TYPE_FLYING && gBattleMons[battler].hp == gBattleMons[battler].maxHP)
				++priority;
			break;
		case ABILITY_TRIAGE:
		    if (MoveHasHealingEffect(move))
				priority += 3;
			break;
	}
    return priority;
}

s32 GetBattlerBracket(u8 battler)
{
    u8 holdEffect = GetBattlerItemHoldEffect(battler, TRUE), holdEffectParam = ItemId_GetHoldEffectParam(gBattleMons[battler].item);
    s32 bracket = 0;
	
	gProtectStructs[battler].myceliumMightElevated = FALSE;
	
    if (holdEffect == HOLD_EFFECT_QUICK_CLAW && gRandomTurnNumber < (0xFFFF * holdEffectParam) / 100)
        bracket = 1;
    else if (GetBattlerAbility(battler) == ABILITY_STALL)
        bracket = -1;
	else if (GetBattlerAbility(battler) == ABILITY_MYCELIUM_MIGHT && IS_MOVE_STATUS(gCurrentMove))
	{
		gProtectStructs[battler].myceliumMightElevated = TRUE;
		bracket = -1;
	}
    return bracket;
}

u32 GetBattlerTotalSpeed(u8 battler)
{
    u8 holdEffect, holdEffectParam;
    u32 monSpeed;
	
	APPLY_STAT_MOD(monSpeed, &gBattleMons[battler], gBattleMons[battler].speed, STAT_SPEED);

	switch (GetBattlerAbility(battler))
	{
		case ABILITY_SWIFT_SWIM:
		    if (IsBattlerWeatherAffected(battler, WEATHER_RAIN_ANY))
				monSpeed *= 2;
			break;
		case ABILITY_CHLOROPHYLL:
		    if (IsBattlerWeatherAffected(battler, WEATHER_SUN_ANY))
				monSpeed *= 2;
			break;
		case ABILITY_SAND_RUSH:
		    if (IsBattlerWeatherAffected(battler, WEATHER_SANDSTORM_ANY))
				monSpeed *= 2;
			break;
		case ABILITY_SLUSH_RUSH:
		    if (IsBattlerWeatherAffected(battler, WEATHER_HAIL_ANY))
				monSpeed *= 2;
			break;
		case ABILITY_QUICK_FEET:
		    if (gBattleMons[battler].status1 & STATUS1_ANY)
				monSpeed *= 2;
			break;
		case ABILITY_SLOW_START:
		    if (gDisableStructs[battler].slowStartTimer)
				monSpeed /= 2;
			break;
		case ABILITY_UNBURDEN: // Don't gain boost if ability was lost
		    if (gBattleResources->flags->flags[battler] & RESOURCE_FLAG_UNBURDEN_BOOST)
				monSpeed *= 2;
			break;
	}
	
#if BADGE_BOOST
    // badge stat boost
    if (!(gBattleTypeFlags & BATTLE_TYPE_LINK) && FlagGet(FLAG_BADGE03_GET) && GetBattlerSide(battler) == B_SIDE_PLAYER)
        monSpeed = (monSpeed * 110) / 100;
#endif

#if SUN_BOOST_SPEED
    if (IsBattlerWeatherAffected(battler, WEATHER_SUN_ANY) && IS_BATTLER_OF_TYPE(battler, TYPE_GRASS))
		monSpeed += (monSpeed / 3);
#endif

#if HAIL_BOOST_SPEED
	if (IsBattlerWeatherAffected(battler, WEATHER_HAIL_ANY) && IS_BATTLER_OF_TYPE(battler, TYPE_ICE))
		monSpeed += (monSpeed / 3);
#endif

    holdEffect = GetBattlerItemHoldEffect(battler, TRUE);
	holdEffectParam = ItemId_GetHoldEffectParam(gBattleMons[battler].item);
    
	switch (holdEffect)
	{
		case HOLD_EFFECT_METAL_POWDER:
		    if (gBattleMons[battler].species == SPECIES_DITTO && !(gBattleMons[battler].status2 & STATUS2_TRANSFORMED) && holdEffectParam == STAT_SPEED)
				monSpeed *= 2;
			break;
		case HOLD_EFFECT_CHOICE_ITEM:
		    if (holdEffectParam == STAT_SPEED)
				monSpeed = (15 * monSpeed) / 10;
			break;
	}
    if (GetBattlerItemHoldEffect(battler, FALSE) == HOLD_EFFECT_MACHO_BRACE)
        monSpeed /= 2;
    
    if (gBattleMons[battler].status1 & STATUS1_PARALYSIS && GetBattlerAbility(battler) != ABILITY_QUICK_FEET)
#if PARALYSIS_UPDATE
        monSpeed /= 2;
#else
        monSpeed /= 4;
#endif

    return monSpeed;
}

static void SetActionsAndBattlersTurnOrder(void)
{
    s32 turnOrderId = 0;
    s32 i, j;

    if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
    {
        for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
        {
            gActionsByTurnOrder[turnOrderId] = gChosenActionByBattler[gActiveBattler];
            gBattlerByTurnOrder[turnOrderId] = gActiveBattler;
            ++turnOrderId;
        }
    }
    else
    {
        if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        {
            for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
            {
                if (gChosenActionByBattler[gActiveBattler] == B_ACTION_RUN)
                {
                    turnOrderId = 5;
                    break;
                }
            }
        }
        else if (gChosenActionByBattler[0] == B_ACTION_RUN)
        {
            gActiveBattler = 0;
            turnOrderId = 5;
        }
        if (turnOrderId == 5) // One of battlers wants to run.
        {
            gActionsByTurnOrder[0] = gChosenActionByBattler[gActiveBattler];
            gBattlerByTurnOrder[0] = gActiveBattler;
            turnOrderId = 1;
            for (i = 0; i < gBattlersCount; ++i)
            {
                if (i != gActiveBattler)
                {
                    gActionsByTurnOrder[turnOrderId] = gChosenActionByBattler[i];
                    gBattlerByTurnOrder[turnOrderId] = i;
                    ++turnOrderId;
                }
            }
            gBattleMainFunc = CheckFocusPunch_ClearVarsBeforeTurnStarts;
            gBattleStruct->focusPunchBattlerId = 0;
            return;
        }
        else
        {
            for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
            {
                if (gChosenActionByBattler[gActiveBattler] == B_ACTION_USE_ITEM || gChosenActionByBattler[gActiveBattler] == B_ACTION_SWITCH)
                {
                    gActionsByTurnOrder[turnOrderId] = gChosenActionByBattler[gActiveBattler];
                    gBattlerByTurnOrder[turnOrderId] = gActiveBattler;
                    ++turnOrderId;
                }
            }
            for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
            {
                if (gChosenActionByBattler[gActiveBattler] != B_ACTION_USE_ITEM && gChosenActionByBattler[gActiveBattler] != B_ACTION_SWITCH)
                {
                    gActionsByTurnOrder[turnOrderId] = gChosenActionByBattler[gActiveBattler];
                    gBattlerByTurnOrder[turnOrderId] = gActiveBattler;
                    ++turnOrderId;
                }
            }
            for (i = 0; i < gBattlersCount - 1; ++i)
            {
                for (j = i + 1; j < gBattlersCount; ++j)
                {
                    u8 battler1 = gBattlerByTurnOrder[i];
                    u8 battler2 = gBattlerByTurnOrder[j];

                    if (gActionsByTurnOrder[i] != B_ACTION_USE_ITEM && gActionsByTurnOrder[j] != B_ACTION_USE_ITEM
					&& gActionsByTurnOrder[i] != B_ACTION_SWITCH && gActionsByTurnOrder[j] != B_ACTION_SWITCH)
                        if (GetWhoStrikesFirst(battler1, battler2, FALSE) != ATTACKER_STRIKES_FIRST)
                            SwapTurnOrder(i, j);
                }
            }
        }
    }
    gBattleMainFunc = CheckFocusPunch_ClearVarsBeforeTurnStarts;
    gBattleStruct->focusPunchBattlerId = 0;
}

static void TurnValuesCleanUp(bool8 var0)
{
    for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
    {
        if (var0)
        {
            gProtectStructs[gActiveBattler].protected = FALSE;
            gProtectStructs[gActiveBattler].endured = FALSE;
        }
        else
        {
			memset(&gProtectStructs[gActiveBattler], 0, sizeof(struct ProtectStruct));
			
            if (gDisableStructs[gActiveBattler].isFirstTurn)
                --gDisableStructs[gActiveBattler].isFirstTurn;
            if (gDisableStructs[gActiveBattler].rechargeTimer)
            {
                --gDisableStructs[gActiveBattler].rechargeTimer;
                if (gDisableStructs[gActiveBattler].rechargeTimer == 0)
                    gBattleMons[gActiveBattler].status2 &= ~(STATUS2_RECHARGE);
            }
        }

        if (gDisableStructs[gActiveBattler].substituteHP == 0)
            gBattleMons[gActiveBattler].status2 &= ~(STATUS2_SUBSTITUTE);
		
		gSpecialStatuses[gActiveBattler].parentalBondState = PARENTAL_BOND_OFF;
    }
    gSideTimers[B_SIDE_PLAYER].followmeTimer = 0;
    gSideTimers[B_SIDE_OPPONENT].followmeTimer = 0;
}

static void CheckFocusPunch_ClearVarsBeforeTurnStarts(void)
{
    if (!(gHitMarker & HITMARKER_RUN))
    {
        while (gBattleStruct->focusPunchBattlerId < gBattlersCount)
        {
            gActiveBattler = gBattlerAttacker = gBattleStruct->focusPunchBattlerId++;
			
            if (gChosenMoveByBattler[gActiveBattler] == MOVE_FOCUS_PUNCH && !(gBattleMons[gActiveBattler].status1 & STATUS1_SLEEP)
				&& !(gDisableStructs[gBattlerAttacker].truantCounter) && !(gProtectStructs[gActiveBattler].noValidMoves))
            {
                BattleScriptExecute(BattleScript_FocusPunchSetUp);
                return;
            }
        }
    }
    TryClearRageStatuses();
    gCurrentTurnActionNumber = 0;
	gCurrentActionFuncId = gActionsByTurnOrder[0];
    gBattleMainFunc = RunTurnActionsFunctions;
    gBattleStruct->moveEffectByte = 0;
    gBattleCommunication[ACTIONS_CONFIRMED_COUNT] = 0;
    gBattleResources->battleScriptsStack->size = 0;
}

static void RunTurnActionsFunctions(void)
{
    if (gBattleOutcome != 0)
        gCurrentActionFuncId = B_ACTION_FINISHED;
    *(&gBattleStruct->savedTurnActionNumber) = gCurrentTurnActionNumber;
    sTurnActionsFuncsTable[gCurrentActionFuncId]();

    if (gCurrentTurnActionNumber >= gBattlersCount) // everyone did their actions, turn finished
    {
        gHitMarker &= ~(HITMARKER_PASSIVE_DAMAGE);
        gBattleMainFunc = sEndTurnFuncsTable[gBattleOutcome & 0x7F];
    }
    else
    {
        if (gBattleStruct->savedTurnActionNumber != gCurrentTurnActionNumber) // action turn has been done, clear hitmarker bits for another battlerId
        {
            gHitMarker &= ~(HITMARKER_NO_ATTACKSTRING);
            gHitMarker &= ~(HITMARKER_UNABLE_TO_USE_MOVE);
        }
    }
}

static void HandleEndTurn_BattleWon(void)
{
    gCurrentActionFuncId = 0;
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        gBattleTextBuff1[0] = gBattleOutcome;
        gBattlerAttacker = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        gBattlescriptCurrInstr = BattleScript_LinkBattleWonOrLost;
        gBattleOutcome &= ~(B_OUTCOME_LINK_BATTLE_RAN);
    }
    else if (gBattleTypeFlags & (BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_BATTLE_TOWER))
    {
        BattleStopLowHpSound();
        PlayBGM(MUS_VICTORY_TRAINER);
        gBattlescriptCurrInstr = BattleScript_BattleTowerTrainerBattleWon;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER && !(gBattleTypeFlags & BATTLE_TYPE_LINK))
    {
        BattleStopLowHpSound();
        gBattlescriptCurrInstr = BattleScript_LocalTrainerBattleWon;
        switch (gTrainers[gTrainerBattleOpponent_A].trainerClass)
        {
        case TRAINER_CLASS_LEADER:
        case TRAINER_CLASS_CHAMPION:
            PlayBGM(MUS_VICTORY_GYM_LEADER);
            break;
        default:
            PlayBGM(MUS_VICTORY_TRAINER);
            break;
        }
    }
    else
    {
        gBattlescriptCurrInstr = BattleScript_PayDayMoneyAndPickUpItems;
    }
    gBattleMainFunc = HandleEndTurn_FinishBattle;
}

static void HandleEndTurn_BattleLost(void)
{
    gCurrentActionFuncId = 0;
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        gBattleTextBuff1[0] = gBattleOutcome;
        gBattlerAttacker = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        gBattlescriptCurrInstr = BattleScript_LinkBattleWonOrLost;
        gBattleOutcome &= ~(B_OUTCOME_LINK_BATTLE_RAN);
    }
    else
    {
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER && GetTrainerBattleMode() == TRAINER_BATTLE_EARLY_RIVAL)
        {
			gBattleCommunication[MULTISTRING_CHOOSER] = (GetRivalBattleFlags() & RIVAL_BATTLE_HEAL_AFTER) ? 1 : 2; // Do or not white out text
            gBattlerAttacker = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
        }
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
		
        gBattlescriptCurrInstr = BattleScript_LocalBattleLost;
    }
    gBattleMainFunc = HandleEndTurn_FinishBattle;
}

static void HandleEndTurn_RanFromBattle(void)
{
    gCurrentActionFuncId = 0;
    switch (gProtectStructs[gBattlerAttacker].fleeFlag)
    {
    default:
        gBattlescriptCurrInstr = BattleScript_GotAwaySafely;
        break;
    case 1:
        gBattlescriptCurrInstr = BattleScript_SmokeBallEscape;
        break;
    case 2:
	    PlaySE(SE_FLEE); // play sound here bc in gen 5 onwards only the ability pop up is displayed with no msg
        gBattlescriptCurrInstr = BattleScript_RanAwayUsingMonAbility;
        break;
    }
    gBattleMainFunc = HandleEndTurn_FinishBattle;
}

static void HandleEndTurn_MonFled(void)
{
    gCurrentActionFuncId = 0;
    PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, gBattlerAttacker, gBattlerPartyIndexes[gBattlerAttacker]);
    gBattlescriptCurrInstr = BattleScript_WildMonFled;
    gBattleMainFunc = HandleEndTurn_FinishBattle;
}

static void HandleEndTurn_FinishBattle(void)
{
	u32 status;
	
    if (gCurrentActionFuncId == B_ACTION_TRY_FINISH || gCurrentActionFuncId == B_ACTION_FINISHED)
    {
		// In Gen4 onwards Natural Cure heals any status condition on completing a battle
		status = 0;
		
		for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
		{
			if (GetBattlerAbility(gActiveBattler) == ABILITY_NATURAL_CURE && gBattleMons[gActiveBattler].status1)
				SetMonData(GetBattlerPartyIndexPtr(gActiveBattler), MON_DATA_STATUS, &status);
		}
        TrySetQuestLogBattleEvent();
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
            ClearRematchStateByTrainerId();
        BeginFastPaletteFade(3);
        FadeOutMapMusic(5);
		DoPlayerPartyEndBattleFormChange();
		memset(&gBattleMonForms, 0, sizeof(gBattleMonForms));
        gBattleMainFunc = FreeResetData_ReturnToOvOrDoEvolutions;
        gCB2_AfterEvolution = BattleMainCB2;
    }
    else if (!gBattleControllerExecFlags)
    {
        gBattleScriptingCommandsTable[gBattlescriptCurrInstr[0]]();
    }
}

static void FreeResetData_ReturnToOvOrDoEvolutions(void)
{
    if (!gPaletteFade.active)
    {
        gIsFishingEncounter = FALSE;
		gIsSurfingEncounter = FALSE;
        ResetSpriteData();
		gBattleMainFunc = gBattleOutcome != B_OUTCOME_WON ? ReturnFromBattleToOverworld : TrySpecialEvolution;
        FreeAllWindowBuffers();
        if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
        {
            FreeMonSpritesGfx();
            FreeBattleSpritesData();
            FreeBattleResources();
        }
		IncrementOrResetDexNavChain((gDexnavBattle && (gBattleOutcome == B_OUTCOME_WON || gBattleOutcome == B_OUTCOME_CAUGHT)));
    }
}

static void TryEvolvePokemon(void)
{
    s32 i;

    while (gLeveledUpInBattle != 0)
    {
        for (i = 0; i < PARTY_SIZE; ++i)
        {
            if (gLeveledUpInBattle & gBitTable[i])
            {
                u16 species;
                u8 levelUpBits = gLeveledUpInBattle;

                levelUpBits &= ~(gBitTable[i]);
                gLeveledUpInBattle = levelUpBits;
                species = GetEvolutionTargetSpecies(&gPlayerParty[i], EVO_MODE_NORMAL, levelUpBits, NULL);
                if (species != SPECIES_NONE)
                {
                    gBattleMainFunc = WaitForEvoSceneToFinish;
                    EvolutionScene(&gPlayerParty[i], species, (TASK_BIT_CAN_STOP | TASK_BIT_LEARN_MOVE), i);
                    return;
                }
            }
        }
    }
    gBattleMainFunc = ReturnFromBattleToOverworld;
}

static void TrySpecialEvolution(void) // Attempts to perform non-level related battle evolutions
{
	s32 i;
	
	for (i = 0; i < PARTY_SIZE; i++)
	{
		u16 species = GetEvolutionTargetSpecies(&gPlayerParty[i], EVO_MODE_BATTLE_SPECIAL, i, NULL);
		
		if (species != SPECIES_NONE && !(sTriedEvolving & gBitTable[i]))
		{
			sTriedEvolving |= gBitTable[i];
			gBattleMainFunc = WaitForEvoSceneToFinish;
			EvolutionScene(&gPlayerParty[i], species, (TASK_BIT_CAN_STOP | TASK_BIT_LEARN_MOVE), i);
			return;
		}
	}
	sTriedEvolving = 0;
	gBattleMainFunc = TryEvolvePokemon;
}

static void WaitForEvoSceneToFinish(void)
{
    if (gMain.callback2 == BattleMainCB2)
        gBattleMainFunc = TrySpecialEvolution;
}

static void ReturnFromBattleToOverworld(void)
{
    if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
    {
        RandomlyGivePartyPokerus(gPlayerParty);
        PartySpreadPokerus(gPlayerParty);
    }
    if (!(gBattleTypeFlags & BATTLE_TYPE_LINK) || !gReceivedRemoteLinkPlayers)
    {
        gSpecialVar_Result = gBattleOutcome;
        gMain.inBattle = FALSE;
        gMain.callback1 = gPreBattleCallback1;
        if (gBattleTypeFlags & BATTLE_TYPE_ROAMER)
        {
            UpdateRoamerHPStatus(&gEnemyParty[0]);
		
	    if ((gBattleOutcome == B_OUTCOME_WON) || gBattleOutcome == B_OUTCOME_CAUGHT)
		    SetRoamerInactive();
        }
        m4aSongNumStop(SE_LOW_HEALTH);
        SetMainCallback2(gMain.savedCallback);
    }
}

void RunBattleScriptCommands_PopCallbacksStack(void)
{
    if (gCurrentActionFuncId == B_ACTION_TRY_FINISH || gCurrentActionFuncId == B_ACTION_FINISHED)
    {
        if (gBattleResources->battleCallbackStack->size != 0)
            --gBattleResources->battleCallbackStack->size;
        gBattleMainFunc = gBattleResources->battleCallbackStack->function[gBattleResources->battleCallbackStack->size];
    }
    else
    {
        if (!gBattleControllerExecFlags)
            gBattleScriptingCommandsTable[gBattlescriptCurrInstr[0]]();
    }
}

void RunBattleScriptCommands(void)
{
    if (!gBattleControllerExecFlags)
        gBattleScriptingCommandsTable[gBattlescriptCurrInstr[0]]();
}

static void SetTypeBeforeUsingMove(u16 move, u8 battler)
{
	u16 moveEffect;
	
	gBattleStruct->dynamicMoveType = gBattleMoves[move].type;
	
	if (move == MOVE_STRUGGLE)
		return;
	
	moveEffect = gBattleMoves[move].effect;
	
	switch (moveEffect)
	{
		case EFFECT_WEATHER_BALL:
		    if (IsBattlerWeatherAffected(battler, WEATHER_RAIN_ANY))
				gBattleStruct->dynamicMoveType = TYPE_WATER;
			else if (IsBattlerWeatherAffected(battler, WEATHER_SANDSTORM_ANY))
				gBattleStruct->dynamicMoveType = TYPE_ROCK;
			else if (IsBattlerWeatherAffected(battler, WEATHER_SUN_ANY))
				gBattleStruct->dynamicMoveType = TYPE_FIRE;
			else if (IsBattlerWeatherAffected(battler, WEATHER_HAIL_ANY))
				gBattleStruct->dynamicMoveType = TYPE_ICE;
			break;
		case EFFECT_HIDDEN_POWER:
			gBattleStruct->dynamicMoveType = GetHiddenPowerType(GetBattlerPartyIndexPtr(battler));
			break;
	}
	if (moveEffect != EFFECT_WEATHER_BALL && moveEffect != EFFECT_HIDDEN_POWER && moveEffect != EFFECT_NATURAL_GIFT && moveEffect != EFFECT_CHANGE_TYPE_ON_ITEM
	&& moveEffect != EFFECT_TERRAIN_PULSE)
	{
		switch (GetBattlerAbility(battler))
		{
			case ABILITY_NORMALIZE:
			    if (gBattleStruct->dynamicMoveType != TYPE_NORMAL)
					gBattleStruct->dynamicMoveType = TYPE_NORMAL;
				break;
			case ABILITY_REFRIGERATE:
			    if (gBattleStruct->dynamicMoveType == TYPE_NORMAL)
					gBattleStruct->dynamicMoveType = TYPE_ICE;
				break;
			case ABILITY_PIXILATE:
			    if (gBattleStruct->dynamicMoveType == TYPE_NORMAL)
					gBattleStruct->dynamicMoveType = TYPE_FAIRY;
				break;
			case ABILITY_AERILATE:
			    if (gBattleStruct->dynamicMoveType == TYPE_NORMAL)
					gBattleStruct->dynamicMoveType = TYPE_FLYING;
				break;
			case ABILITY_GALVANIZE:
			    if (gBattleStruct->dynamicMoveType == TYPE_NORMAL)
					gBattleStruct->dynamicMoveType = TYPE_ELECTRIC;
				break;
		}
	}
	else if (gBattleMoves[move].flags.soundMove && GetBattlerAbility(battler) == ABILITY_LIQUID_VOICE)
		gBattleStruct->dynamicMoveType = TYPE_WATER;
}

static void HandleAction_UseMove(void)
{
    u8 side, i, var = 4, moveTarget, moveType;
    
    gBattlerAttacker = gBattlerByTurnOrder[gCurrentTurnActionNumber];
	
    if (gBattleStruct->absentBattlerFlags & gBitTable[gBattlerAttacker] || !IsBattlerAlive(gBattlerAttacker))
    {
        gCurrentActionFuncId = B_ACTION_FINISHED;
        return;
    }
    gIsCriticalHit = FALSE;
    gBattleScripting.dmgMultiplier = 1;
    gBattleStruct->atkCancellerTracker = 0;
    gMoveResultFlags = 0;
    gMultiHitCounter = 0;
	gBattleScripting.savedDmg = 0;
    gBattleCommunication[MISS_TYPE] = 0;
    gCurrMovePos = gChosenMovePos = *(gBattleStruct->chosenMovePositions + gBattlerAttacker);
	
    // choose move
    if (gProtectStructs[gBattlerAttacker].noValidMoves)
    {
        gProtectStructs[gBattlerAttacker].noValidMoves = 0;
        gCurrentMove = gChosenMove = MOVE_STRUGGLE;
        gHitMarker |= HITMARKER_NO_PPDEDUCT;
        gBattleStruct->moveTarget[gBattlerAttacker] = GetMoveTarget(MOVE_STRUGGLE, 0);
    }
    else if (gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS || gBattleMons[gBattlerAttacker].status2 & STATUS2_RECHARGE)
    {
        gCurrentMove = gChosenMove = gLockedMoves[gBattlerAttacker];
    }
    // encore forces you to use the same move
    else if (gDisableStructs[gBattlerAttacker].encoredMove != MOVE_NONE
          && gDisableStructs[gBattlerAttacker].encoredMove == gBattleMons[gBattlerAttacker].moves[gDisableStructs[gBattlerAttacker].encoredMovePos])
    {
        gCurrentMove = gChosenMove = gDisableStructs[gBattlerAttacker].encoredMove;
        gCurrMovePos = gChosenMovePos = gDisableStructs[gBattlerAttacker].encoredMovePos;
        gBattleStruct->moveTarget[gBattlerAttacker] = GetMoveTarget(gCurrentMove, 0);
    }
    // check if the encored move wasn't overwritten
    else if (gDisableStructs[gBattlerAttacker].encoredMove != MOVE_NONE
          && gDisableStructs[gBattlerAttacker].encoredMove != gBattleMons[gBattlerAttacker].moves[gDisableStructs[gBattlerAttacker].encoredMovePos])
    {
        gCurrMovePos = gChosenMovePos = gDisableStructs[gBattlerAttacker].encoredMovePos;
        gCurrentMove = gChosenMove = gBattleMons[gBattlerAttacker].moves[gCurrMovePos];
        gDisableStructs[gBattlerAttacker].encoredMove = MOVE_NONE;
        gDisableStructs[gBattlerAttacker].encoredMovePos = 0;
        gDisableStructs[gBattlerAttacker].encoreTimer = 0;
        gBattleStruct->moveTarget[gBattlerAttacker] = GetMoveTarget(gCurrentMove, 0);
    }
    else if (gBattleMons[gBattlerAttacker].moves[gCurrMovePos] != gChosenMoveByBattler[gBattlerAttacker])
    {
        gCurrentMove = gChosenMove = gBattleMons[gBattlerAttacker].moves[gCurrMovePos];
        gBattleStruct->moveTarget[gBattlerAttacker] = GetMoveTarget(gCurrentMove, 0);
    }
    else
        gCurrentMove = gChosenMove = gBattleMons[gBattlerAttacker].moves[gCurrMovePos];
	
	// Set dynamic move type
	SetTypeBeforeUsingMove(gCurrentMove, gBattlerAttacker);
	moveType = gBattleStruct->dynamicMoveType;
	
	moveTarget = GetBattlerMoveTargetType(gBattlerAttacker, gCurrentMove);
	
    // Choose target
    side = GetBattlerSide(BATTLE_OPPOSITE(gBattlerAttacker));
	
	if (IsBattlerAffectedByFollowMe(gBattlerAttacker, side, gCurrentMove) && GetBattlerSide(gBattlerAttacker) != GetBattlerSide(gSideTimers[side].followmeTarget)
		&& moveTarget == MOVE_TARGET_SELECTED)
	    gBattlerTarget = gSideTimers[side].followmeTarget;
    else if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && !gSideTimers[side].followmeTimer && (!IS_MOVE_STATUS(gCurrentMove) || moveTarget & MOVE_TARGET_USER)
		&& ((GetBattlerAbility(gBattleStruct->moveTarget[gBattlerAttacker]) != ABILITY_LIGHTNING_ROD && moveType == TYPE_ELECTRIC)
	    || (GetBattlerAbility(gBattleStruct->moveTarget[gBattlerAttacker]) != ABILITY_STORM_DRAIN && moveType == TYPE_WATER)))
    {
        for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
		{
            if (GetBattlerSide(gBattlerAttacker) != GetBattlerSide(gActiveBattler) && gBattleStruct->moveTarget[gBattlerAttacker] != gActiveBattler
				&& ((GetBattlerAbility(gActiveBattler) == ABILITY_LIGHTNING_ROD && moveType == TYPE_ELECTRIC)
				|| (GetBattlerAbility(gActiveBattler) == ABILITY_STORM_DRAIN && moveType == TYPE_WATER)) && GetBattlerTurnOrderNum(gActiveBattler) < var
				&& GetBattlerAbility(gBattlerAttacker) != ABILITY_PROPELLER_TAIL && GetBattlerAbility(gBattlerAttacker) != ABILITY_STALWART)
				var = GetBattlerTurnOrderNum(gActiveBattler);
		}
        if (var == 4)
        {
            if (moveTarget == MOVE_TARGET_RANDOM)
				gBattlerTarget = GetRandomTarget(gBattlerAttacker);
            else if (moveTarget == MOVE_TARGET_FOES_AND_ALLY)
			{
				for (gBattlerTarget = 0; gBattlerTarget < gBattlersCount; gBattlerTarget++)
				{
					if (gBattlerTarget != gBattlerAttacker && IsBattlerAlive(gBattlerTarget))
						break;
				}
			}
			else
                gBattlerTarget = gBattleStruct->moveTarget[gBattlerAttacker];
            
			if (!IsBattlerAlive(gBattlerTarget))
            {
                if (GetBattlerSide(gBattlerAttacker) != GetBattlerSide(gBattlerTarget))
                    gBattlerTarget = BATTLE_PARTNER(gBattlerTarget);
                else
                {
                    gBattlerTarget = BATTLE_OPPOSITE(gBattlerAttacker);
					
                    if (!IsBattlerAlive(gBattlerTarget))
                        gBattlerTarget = BATTLE_PARTNER(gBattlerTarget);
                }
            }
        }
        else
        {
            gActiveBattler = gBattlerByTurnOrder[var];
            RecordAbilityBattle(gActiveBattler, gBattleMons[gActiveBattler].ability);
			
			if (GetBattlerAbility(gActiveBattler) == ABILITY_LIGHTNING_ROD)
				gSpecialStatuses[gActiveBattler].lightningRodRedirected = 1;
			else if (GetBattlerAbility(gActiveBattler) == ABILITY_STORM_DRAIN)
				gSpecialStatuses[gActiveBattler].stormDrainRedirected = 1;
			
            gBattlerTarget = gActiveBattler;
        }
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && moveTarget == MOVE_TARGET_RANDOM)
	{
		gBattlerTarget = GetRandomTarget(gBattlerAttacker);
		
        if (gAbsentBattlerFlags & gBitTable[gBattlerTarget] && GetBattlerSide(gBattlerAttacker) != GetBattlerSide(gBattlerTarget))
            gBattlerTarget = BATTLE_PARTNER(gBattlerTarget);
    }
	else if (moveTarget == MOVE_TARGET_ALLY)
	{
		gBattlerTarget = BATTLE_PARTNER(gBattlerAttacker);
		
		if (!IsBattlerAlive(gBattlerTarget))
			gBattlerTarget = gBattlerAttacker;
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && moveTarget == MOVE_TARGET_FOES_AND_ALLY)
	{
		for (gBattlerTarget = 0; gBattlerTarget < gBattlersCount; gBattlerTarget++)
		{
			if (gBattlerTarget != gBattlerAttacker && IsBattlerAlive(gBattlerTarget))
				break;
		}
	}
    else
    {
        gBattlerTarget = gBattleStruct->moveTarget[gBattlerAttacker];
		
        if (!IsBattlerAlive(gBattlerTarget))
        {
            if (GetBattlerSide(gBattlerAttacker) != GetBattlerSide(gBattlerTarget))
                gBattlerTarget = BATTLE_PARTNER(gBattlerTarget);
            else
            {
                gBattlerTarget = BATTLE_OPPOSITE(gBattlerAttacker);
				
                if (!IsBattlerAlive(gBattlerTarget))
                    gBattlerTarget = BATTLE_PARTNER(gBattlerTarget);
            }
        }
    }
	for (i = 0; i < MAX_BATTLERS_COUNT; i++)
		gBattleStruct->hpBefore[i] = gBattleMons[i].hp;
	
    gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

static void HandleAction_Switch(void)
{
    gBattlerAttacker = gBattlerByTurnOrder[gCurrentTurnActionNumber];
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    gActionSelectionCursor[gBattlerAttacker] = 0;
    gMoveSelectionCursor[gBattlerAttacker] = 0;
    PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, gBattlerAttacker, *(gBattleStruct->battlerPartyIndexes + gBattlerAttacker));
    gBattleScripting.battler = gBattlerAttacker;
    gBattlescriptCurrInstr = BattleScript_ActionSwitch;
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

static void HandleAction_UseItem(void)
{
	gBattlerAttacker = gActiveBattler = gBattlerByTurnOrder[gCurrentTurnActionNumber];
	gBattleScripting.battler = GetItemUseBattler(gBattlerAttacker);
    gBattle_BG0_X = 0;
	gBattle_BG0_Y = 0;
    ClearFuryCutterDestinyBondGrudge(gBattlerAttacker);
    gLastUsedItem = gBattleBufferB[gBattlerAttacker][1] | (gBattleBufferB[gBattlerAttacker][2] << 8);
	
	if (gBattleScripting.battler == MAX_BATTLERS_COUNT) // if the item ins't used on a battler only plays the message and sound.
		gBattlescriptCurrInstr = BattleScript_ItemUseMessageEnd;
	else // otherwise execute the item's script, wich plays the message and sound too.
		gBattlescriptCurrInstr = gBattlescriptsForUsingItem[ItemId_GetBattleUsage(gLastUsedItem) - 1];
	
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

bool8 TryRunFromBattle(u8 battler)
{
    bool8 effect = FALSE;
    u8 holdEffect = GetBattlerItemHoldEffect(battler, TRUE);
    u8 speedVar, battlerEscapeFrom;

    if (holdEffect == HOLD_EFFECT_CAN_ALWAYS_RUN)
    {
        gLastUsedItem = gBattleMons[battler].item;
        gProtectStructs[battler].fleeFlag = 1;
        ++effect;
    }
	else if (IS_BATTLER_OF_TYPE(battler, TYPE_GHOST))
	{
		++effect;
	}
    else if (GetBattlerAbility(battler) == ABILITY_RUN_AWAY)
    {
        gLastUsedAbility = ABILITY_RUN_AWAY;
        gProtectStructs[battler].fleeFlag = 2;
        ++effect;
    }
    else if (IS_BATTLE_TYPE_GHOST_WITHOUT_SCOPE())
    {
        if (GetBattlerSide(battler) == B_SIDE_PLAYER)
            ++effect;
    }
    else
    {
        battlerEscapeFrom = BATTLE_OPPOSITE(battler);
		
		if (!IsBattlerAlive(battlerEscapeFrom))
			battlerEscapeFrom = BATTLE_PARTNER(battlerEscapeFrom);
		
		if (gBattleMons[battler].speed < gBattleMons[battlerEscapeFrom].speed)
		{
			speedVar = (gBattleMons[battler].speed * 128) / (gBattleMons[battlerEscapeFrom].speed) + (gBattleStruct->runTries * 30);
			if (speedVar > (Random() & 0xFF))
				++effect;
		}
		else // same speed or faster
		{
			++effect;
		}

        ++gBattleStruct->runTries;
    }
    if (effect)
    {
        gCurrentTurnActionNumber = gBattlersCount;
        gBattleOutcome = B_OUTCOME_RAN;
    }
    return effect;
}

static void HandleAction_Run(void)
{
    gBattlerAttacker = gBattlerByTurnOrder[gCurrentTurnActionNumber];

    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        gCurrentTurnActionNumber = gBattlersCount;
        for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
        {
			if (gChosenActionByBattler[gActiveBattler] == B_ACTION_RUN)
			{
				if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
					gBattleOutcome |= B_OUTCOME_LOST;
				else
					gBattleOutcome |= B_OUTCOME_WON;
			}
        }
        gBattleOutcome |= B_OUTCOME_LINK_BATTLE_RAN;
    }
    else
    {
        if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
        {
            if (!TryRunFromBattle(gBattlerAttacker)) // failed to run away
            {
                ClearFuryCutterDestinyBondGrudge(gBattlerAttacker);
                gBattleCommunication[MULTISTRING_CHOOSER] = 3;
                gBattlescriptCurrInstr = BattleScript_PrintFailedToRunString;
                gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
            }
        }
        else
        {
            if (!CanBattlerEscape(gBattlerAttacker))
            {
                gBattleCommunication[MULTISTRING_CHOOSER] = 4;
                gBattlescriptCurrInstr = BattleScript_PrintFailedToRunString;
                gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
            }
            else
            {
                gCurrentTurnActionNumber = gBattlersCount;
                gBattleOutcome = B_OUTCOME_MON_FLED;
            }
        }
    }
}

static void HandleAction_WatchesCarefully(void)
{
    gBattlerAttacker = gBattlerByTurnOrder[gCurrentTurnActionNumber];
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    if (gBattleStruct->safariGoNearCounter != 0)
    {
        --gBattleStruct->safariGoNearCounter;
        if (gBattleStruct->safariGoNearCounter == 0)
        {
            *(&gBattleStruct->safariCatchFactor) = gBaseStats[GetMonData(gEnemyParty, MON_DATA_SPECIES)].catchRate * 100 / 1275;
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        }
        else
        {
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
        }
    }
    else
    {
        if (gBattleStruct->safariPkblThrowCounter != 0)
        {
            --gBattleStruct->safariPkblThrowCounter;
            if (gBattleStruct->safariPkblThrowCounter == 0)
                gBattleCommunication[MULTISTRING_CHOOSER] = 0;
            else
                gBattleCommunication[MULTISTRING_CHOOSER] = 2;
        }
        else
        {
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        }
    }
    gBattlescriptCurrInstr = gBattlescriptsForSafariActions[0];
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

static void HandleAction_SafariZoneBallThrow(void)
{
    gBattlerAttacker = gBattlerByTurnOrder[gCurrentTurnActionNumber];
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    --gNumSafariBalls;
    gLastUsedItem = ITEM_SAFARI_BALL;
    gBattlescriptCurrInstr = BattleScript_ThrowSafariBall;
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

static void HandleAction_ThrowBait(void)
{
    gBattlerAttacker = gBattlerByTurnOrder[gCurrentTurnActionNumber];
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    gBattleStruct->safariPkblThrowCounter += Random() % 5 + 2;
    if (gBattleStruct->safariPkblThrowCounter > 6)
        gBattleStruct->safariPkblThrowCounter = 6;
    gBattleStruct->safariGoNearCounter = 0;
    gBattleStruct->safariCatchFactor >>= 1;
    if (gBattleStruct->safariCatchFactor <= 2)
        gBattleStruct->safariCatchFactor = 3;
    gBattlescriptCurrInstr = gBattlescriptsForSafariActions[2];
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

static void HandleAction_ThrowRock(void)
{
    gBattlerAttacker = gBattlerByTurnOrder[gCurrentTurnActionNumber];
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    gBattleStruct->safariGoNearCounter += Random() % 5 + 2;
    if (gBattleStruct->safariGoNearCounter > 6)
        gBattleStruct->safariGoNearCounter = 6;
    gBattleStruct->safariPkblThrowCounter = 0;
    gBattleStruct->safariCatchFactor <<= 1;
    if (gBattleStruct->safariCatchFactor > 20)
        gBattleStruct->safariCatchFactor = 20;
    gBattlescriptCurrInstr = gBattlescriptsForSafariActions[1];
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

static void HandleAction_SafariZoneRun(void)
{
    gBattlerAttacker = gBattlerByTurnOrder[gCurrentTurnActionNumber];
    PlaySE(SE_FLEE);
    gCurrentTurnActionNumber = gBattlersCount;
    gBattleOutcome = B_OUTCOME_RAN;
}

static void HandleAction_OldManBallThrow(void)
{
    gBattlerAttacker = gBattlerByTurnOrder[gCurrentTurnActionNumber];
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, gBattlerAttacker, gBattlerPartyIndexes[gBattlerAttacker])
    gBattlescriptCurrInstr = gBattlescriptsForSafariActions[3];
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
    gActionsByTurnOrder[1] = B_ACTION_FINISHED;
}

static void HandleAction_TryFinish(void)
{
    if (!HandleFaintedMonActions())
    {
        gBattleStruct->faintedActionsState = 0;
        gCurrentActionFuncId = B_ACTION_FINISHED;
    }
}

static void HandleAction_NothingIsFainted(void)
{
    ++gCurrentTurnActionNumber;
    gCurrentActionFuncId = gActionsByTurnOrder[gCurrentTurnActionNumber];
	gHitMarker &= ~(HITMARKER_DESTINYBOND | HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_ATTACKSTRING_PRINTED | HITMARKER_NO_PPDEDUCT | HITMARKER_IGNORE_SAFEGUARD
	| HITMARKER_PASSIVE_DAMAGE | HITMARKER_OBEYS | HITMARKER_SYNCHRONISE_EFFECT | HITMARKER_CHARGING | HITMARKER_IGNORE_DISGUISE);
}

static void HandleAction_ActionFinished(void)
{
	gSelectedMonPartyId = PARTY_SIZE;
	HandleAction_NothingIsFainted();
	memset(&gSpecialStatuses, 0, sizeof(gSpecialStatuses));
    gCurrentMove = MOVE_NONE;
    gBattleMoveDamage = 0;
    gMoveResultFlags = 0;
    gBattleScripting.animTurn = 0;
    gBattleScripting.animTargetsHit = 0;
    gLastLandedMoves[gBattlerAttacker] = 0;
    gLastHitByType[gBattlerAttacker] = 0;
    gBattleStruct->dynamicMoveType = 0;
    gBattleScripting.atk49_state = 0;
    gBattleStruct->moveEffectByte = 0;
    gBattleCommunication[ACTIONS_CONFIRMED_COUNT] = 0;
    gBattleResources->battleScriptsStack->size = 0;
}

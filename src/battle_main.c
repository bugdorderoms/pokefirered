#include "global.h"
#include "gflib.h"
#include "battle.h"
#include "battle_ai_util.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "battle_gfx_sfx_util.h"
#include "battle_main.h"
#include "battle_message.h"
#include "battle_raid.h"
#include "battle_damage_calc.h"
#include "battle_scripts.h"
#include "battle_move_effects.h"
#include "battle_setup.h"
#include "data.h"
#include "dns.h"
#include "menu_helpers.h"
#include "battle_queued_effects.h"
#include "dexnav.h"
#include "decompress.h"
#include "event_data.h"
#include "evolution_scene.h"
#include "field_weather.h"
#include "graphics.h"
#include "item.h"
#include "link.h"
#include "evolution.h"
#include "link_rfu.h"
#include "load_save.h"
#include "m4a.h"
#include "party_menu.h"
#include "pokeball.h"
#include "pokedex.h"
#include "form_change.h"
#include "reshow_battle_screen.h"
#include "random.h"
#include "roamer.h"
#include "raid_intro.h"
#include "wild_encounter.h"
#include "safari_zone.h"
#include "test_runner.h"
#include "scanline_effect.h"
#include "recorded_battle.h"
#include "script_pokemon_util.h"
#include "task.h"
#include "trig.h"
#include "vs_seeker.h"
#include "util.h"
#include "trainer_slide.h"
#include "constants/abilities.h"
#include "constants/battle_script_commands.h"
#include "constants/battle_setup.h"
#include "constants/form_change.h"
#include "constants/hold_effects.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/trainer_slides.h"
#include "constants/songs.h"
#include "constants/battle_string_ids.h"
#include "constants/weather.h"

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
static void HandleAction_LastUsedBallThrow(void);
static void HandleAction_Cheer(void);
static void HandleAction_TryFinish(void);
static void HandleAction_NothingIsFainted(void);
static void HandleAction_ActionFinished(void);
static void HandleEndTurn_ContinueBattle(void);
static void HandleEndTurn_BattleWon(void);
static void HandleEndTurn_BattleLost(void);
static void HandleEndTurn_RanFromBattle(void);
static void HandleEndTurn_MonFled(void);
static void HandleEndTurn_FinishBattle(void);
static void ClearActionsAndMovesForNextTurn(void);
static void TryCallSosAlly(void);
static void CB2_InitBattleInternal(void);
static void CB2_PreInitMultiBattle(void);
static void CB2_PreInitIngamePlayerPartnerBattle(void);
static void CB2_HandleStartMultiBattle(void);
static void CB2_HandleStartMultiRaidBattle(void);
static void CB2_HandleStartMultiPartnerBattle(void);
static void CreateNPCTrainerParty(u32 trainerNum);
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
static void DoBattleIntro(void);
static void TurnValuesCleanUp(bool32 var0);
static void TryDoEventsBeforeFirstTurn(void);
static void HandleAssignUsableGimmicks(void);
static void HandleTurnActionSelectionState(void);
static void RunTurnActionsFunctions(void);
static void SetActionsAndBattlersTurnOrder(void);
static void CheckChangingTurnOrderEffects(void);
static void FreeResetData_ReturnToOvOrDoEvolutions(void);
static void ReturnFromBattleToOverworld(void);
static void TryEvolvePokemon(void);
static void WaitForEvoSceneToFinish(void);
static void BattleEndClearFlags(void);

// Bg rams
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
EWRAM_DATA u8 *gBattleAnimMons_BgTilesBuffer = NULL;
EWRAM_DATA u8 *gBattleAnimMons_BgTilemapBuffer = NULL;
// String rams
EWRAM_DATA u8 gDisplayedStringBattle[300] = {0};
EWRAM_DATA u8 gBattleTextBuff1[TEXT_BUFF_ARRAY_COUNT] = {0};
EWRAM_DATA u8 gBattleTextBuff2[TEXT_BUFF_ARRAY_COUNT] = {0};
EWRAM_DATA u8 gBattleTextBuff3[TEXT_BUFF_ARRAY_COUNT] = {0};
// Battle basic rams
EWRAM_DATA u8 gBattleTerrain = 0;
EWRAM_DATA u8 gAbsentBattlerFlags = 0;
EWRAM_DATA u32 gBattleTypeFlags = 0;
EWRAM_DATA u8 gBattlerSpriteIds[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA const u8 *gBattlescriptCurrInstr = NULL;
EWRAM_DATA u16 gBattlerPartyIndexes[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gBattlerPositions[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gBattleCommunication[BATTLE_COMMUNICATION_ENTRIES_COUNT] = {0};
EWRAM_DATA u16 gPauseCounterBattle = 0;
EWRAM_DATA struct QuickClawRandom gQuickClawTurnRandom[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gBattlersCount = 0;
EWRAM_DATA u8 gBattleOutcome = 0;
EWRAM_DATA u16 gBattleWeather = 0;
EWRAM_DATA u16 gIntroSlideFlags = 0;
EWRAM_DATA u8 gSentPokesToOpponent[2] = {0};
EWRAM_DATA u16 gLastUsedItem = 0;
EWRAM_DATA u16 gBallToDisplay = 0;
EWRAM_DATA u16 gLastThrownBall = 0;
EWRAM_DATA u8 gBattlerStatusSummaryTaskId[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gExpShareExp = 0;
EWRAM_DATA u8 gBattlerInMenuId = 0;
EWRAM_DATA bool8 gDoingBattleAnim = FALSE;
EWRAM_DATA u16 gBattleMonForms[B_SIDE_COUNT][PARTY_SIZE] = {0};
EWRAM_DATA u16 gMoveToLearn = 0;
EWRAM_DATA u32 gTransformedPersonalities[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA bool8 gTransformedShinies[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gPartyCriticalHits[PARTY_SIZE] = {0};
EWRAM_DATA u8 gNumRaidBattleStars = 0;
EWRAM_DATA bool8 gCapturedWildMon = FALSE;
EWRAM_DATA struct BattleFormChange gBattleFormChangeData = {0};
// Controller rams
EWRAM_DATA u8 *gLinkBattleSendBuffer = NULL;
EWRAM_DATA u8 *gLinkBattleRecvBuffer = NULL;
EWRAM_DATA u8 gBattleBufferA[MAX_BATTLERS_COUNT][0x200] = {0};
EWRAM_DATA u8 gBattleBufferB[MAX_BATTLERS_COUNT][0x200] = {0};
EWRAM_DATA u32 gBattleControllerExecFlags = 0;
// Turn and actions rams
EWRAM_DATA u8 gActionsByTurnOrder[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gBattlerByTurnOrder[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gActionSelectionCursor[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u8 gCurrentTurnActionNumber = 0;
EWRAM_DATA u8 gCurrentActionFuncId = 0;
EWRAM_DATA u8 gCurrentTurnActionBattlerId = 0;
// Move vars
EWRAM_DATA u8 gCurrMovePos = 0;
EWRAM_DATA u8 gChosenMovePos = 0;
EWRAM_DATA u16 gLastUsedMove = 0;
EWRAM_DATA u16 gLastUsedBaseMove = 0;
EWRAM_DATA u16 gCurrentMove = 0;
EWRAM_DATA u16 gChosenMove = 0;
EWRAM_DATA u16 gCalledMove = 0;
EWRAM_DATA bool8 gIsCriticalHit = FALSE;
EWRAM_DATA u8 gMultiHitCounter = 0;
// Damage vars
EWRAM_DATA s32 gBattleMoveDamage = 0;
EWRAM_DATA s32 gHpDealt = 0;
EWRAM_DATA u32 gHitMarker = 0;
EWRAM_DATA u16 gBattleMovePower = 0;
EWRAM_DATA u16 gMoveResultFlags = 0;
// Battlers vars
EWRAM_DATA u8 gBattlerAttacker = 0;
EWRAM_DATA u8 gBattlerTarget = 0;
EWRAM_DATA u8 gBattlerFainted = 0;
EWRAM_DATA u8 gEffectBattler = 0;
// Structs and timers
EWRAM_DATA struct MultiBattlePokemonTx gMultiPartnerParty[MULTI_PARTY_SIZE] = {0};
EWRAM_DATA struct BattleSpriteData *gBattleSpritesDataPtr = NULL;
EWRAM_DATA struct MonSpritesGfx *gMonSpritesGfxPtr = NULL;
EWRAM_DATA struct BattlePokemon gBattleMons[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA u16 gSideStatuses[B_SIDE_COUNT] = {0};
EWRAM_DATA struct SideTimer gSideTimers[B_SIDE_COUNT] = {0};
EWRAM_DATA u32 gStatuses3[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA struct DisableStruct gDisableStructs[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA struct ProtectStruct gProtectStructs[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA struct SpecialStatus gSpecialStatuses[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA struct BattleScripting gBattleScripting = {0};
EWRAM_DATA struct BattleStruct *gBattleStruct = NULL;
EWRAM_DATA struct BattleResources *gBattleResources = NULL;
EWRAM_DATA u32 gFieldStatus = 0;
EWRAM_DATA struct FieldTimer gFieldTimers = {0};
EWRAM_DATA struct QueuedStatBoost gQueuedStatBoosts[MAX_BATTLERS_COUNT] = {0};

// IWRAM vars
void (*gPreBattleCallback1)(void);
void (*gBattleMainFunc)(void);
struct BattlerControllerData gBattlerControllersData[MAX_BATTLERS_COUNT];
u8 gHealthboxSpriteIds[MAX_BATTLERS_COUNT];
u8 gMultiUsePlayerCursor;
u8 gNumberOfMovesToChoose;
u8 gLeveledUpInBattle;

static const struct ScanlineEffectParams sIntroScanlineParams16Bit =
{
    .dmaDest = &REG_BG3HOFS,
    .dmaControl = SCANLINE_EFFECT_DMACNT_16BIT,
    .initState = 1
};

#if USE_DNS_IN_BATTLE
static const struct DNSPalExceptions sCombatPalExceptions =  
{
    .pal = {
        DNS_PAL_EXCEPTION,  // 0
        DNS_PAL_EXCEPTION,  // 1
        DNS_PAL_ACTIVE,     // 2
        DNS_PAL_ACTIVE,     // 3
        DNS_PAL_ACTIVE,     // 4
        DNS_PAL_EXCEPTION,  // 5
        DNS_PAL_EXCEPTION,  // 6
        DNS_PAL_ACTIVE,     // 7
        DNS_PAL_EXCEPTION,  // 8
        DNS_PAL_EXCEPTION,  // 9
        DNS_PAL_ACTIVE,     // 10
        DNS_PAL_ACTIVE,     // 11
        DNS_PAL_ACTIVE,     // 12
        DNS_PAL_ACTIVE,     // 13
        DNS_PAL_ACTIVE,     // 14
        DNS_PAL_ACTIVE,     // 15
        DNS_PAL_EXCEPTION,  // 16
        DNS_PAL_EXCEPTION,  // 17
        DNS_PAL_EXCEPTION,  // 18
        DNS_PAL_EXCEPTION,  // 19
        DNS_PAL_EXCEPTION,  // 20
        DNS_PAL_EXCEPTION,  // 21
        DNS_PAL_EXCEPTION,  // 22
        DNS_PAL_EXCEPTION,  // 23
        DNS_PAL_EXCEPTION,  // 24
        DNS_PAL_EXCEPTION,  // 25
        DNS_PAL_EXCEPTION,  // 26
        DNS_PAL_EXCEPTION,  // 27
        DNS_PAL_EXCEPTION,  // 28
        DNS_PAL_EXCEPTION,  // 29
        DNS_PAL_EXCEPTION,  // 30
        DNS_PAL_EXCEPTION,  // 31
    }
};
#endif

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

const struct NonVolatileStatus gNonVolatileStatusConditions[] =
{
    [STATUS1_SLEEP - 1] =
    {
        .placeholder = COMPOUND_STRING("sleep"),
        .ailmentId = AILMENT_SLP,
        .animationId = B_ANIM_STATUS_SLP,
        .healthboxStatusGfx = HEALTHBOX_GFX_STATUS_SLP_BATTLER0,
        .healthboxStatusPal = RGB(20, 20, 17),
        .statusCuredByItemMsgId = ITEMUSE_STRING_WOKE_UP,
        .statusCuredByMoveMultistringId = B_MSG_WOKEUP_EFFECT,
    },
    [STATUS1_POISON - 1 ... STATUS1_TOXIC_POISON - 1] =
    {
        .placeholder = COMPOUND_STRING("poison"),
        .ailmentId = AILMENT_PSN,
        .animationId = B_ANIM_STATUS_PSN,
        .healthboxStatusGfx = HEALTHBOX_GFX_STATUS_PSN_BATTLER0,
        .healthboxStatusPal = RGB(24, 12, 24),
        .statusCuredByItemMsgId = ITEMUSE_STRING_POISON_CURED,
        .statusCuredByMoveMultistringId = B_MSG_CURED_BUFF1, // Never used
    },
    [STATUS1_BURN - 1] =
    {
        .placeholder = COMPOUND_STRING("burn"),
        .ailmentId = AILMENT_BRN,
        .animationId = B_ANIM_STATUS_BRN,
        .healthboxStatusGfx = HEALTHBOX_GFX_STATUS_BRN_BATTLER0,
        .healthboxStatusPal = RGB(28, 14, 10),
        .statusCuredByItemMsgId = ITEMUSE_STRING_BURN_HEALED,
        .statusCuredByMoveMultistringId = B_MSG_CURED_BUFF1, // Never used
    },
    [STATUS1_FREEZE - 1] =
    {
        .placeholder = COMPOUND_STRING("freeze"),
        .ailmentId = AILMENT_FRZ,
        .animationId = B_ANIM_STATUS_FRZ,
        .healthboxStatusGfx = HEALTHBOX_GFX_STATUS_FRZ_BATTLER0,
        .healthboxStatusPal = RGB(17, 22, 28),
        .statusCuredByItemMsgId = ITEMUSE_STRING_THAWED,
        .statusCuredByMoveMultistringId = B_MSG_CURED_BUFF1, // Never used
    },
    [STATUS1_PARALYSIS - 1] =
    {
        .placeholder = COMPOUND_STRING("paralysis"),
        .ailmentId = AILMENT_PRZ,
        .animationId = B_ANIM_STATUS_PRZ,
        .healthboxStatusGfx = HEALTHBOX_GFX_STATUS_PRZ_BATTLER0,
        .healthboxStatusPal = RGB(23, 23, 3),
        .statusCuredByItemMsgId = ITEMUSE_STRING_PARALYSIS_CURED,
        .statusCuredByMoveMultistringId = B_MSG_CURED_BUFF1,
    },
};

const u8 gCategoryNames[NUM_MOVE_SPLITS][CATEGORY_NAME_LENGTH + 1] =
{
    [SPLIT_PHYSICAL] = _("Physical"),
    [SPLIT_SPECIAL]  = _("Special"),
    [SPLIT_STATUS]   = _("Status"),
};

const struct NaturalGift gNaturalGiftTable[ITEM_TO_BERRY(BERRY_ITEMS_END) + 1] =
{
    [ITEM_TO_BERRY(ITEM_CHERI_BERRY)] = { .type = TYPE_FIRE, .power = 80 },
    [ITEM_TO_BERRY(ITEM_CHESTO_BERRY)] = { .type = TYPE_WATER, .power = 80 },
    [ITEM_TO_BERRY(ITEM_PECHA_BERRY)] = { .type = TYPE_ELECTRIC, .power = 80 },
    [ITEM_TO_BERRY(ITEM_RAWST_BERRY)] = { .type = TYPE_GRASS, .power = 80 },
    [ITEM_TO_BERRY(ITEM_ASPEAR_BERRY)] = { .type = TYPE_ICE, .power = 80 },
    [ITEM_TO_BERRY(ITEM_LEPPA_BERRY)] = { .type = TYPE_FIGHTING, .power = 80 },
    [ITEM_TO_BERRY(ITEM_ORAN_BERRY)] = { .type = TYPE_POISON, .power = 80 },
    [ITEM_TO_BERRY(ITEM_PERSIM_BERRY)] = { .type = TYPE_GROUND, .power = 80 },
    [ITEM_TO_BERRY(ITEM_LUM_BERRY)] = { .type = TYPE_FLYING, .power = 80 },
    [ITEM_TO_BERRY(ITEM_SITRUS_BERRY)] = { .type = TYPE_PSYCHIC, .power = 80 },
    [ITEM_TO_BERRY(ITEM_FIGY_BERRY)] = { .type = TYPE_BUG, .power = 80 },
    [ITEM_TO_BERRY(ITEM_WIKI_BERRY)] = { .type = TYPE_ROCK, .power = 80 },
    [ITEM_TO_BERRY(ITEM_MAGO_BERRY)] = { .type = TYPE_GHOST, .power = 80 },
    [ITEM_TO_BERRY(ITEM_AGUAV_BERRY)] = { .type = TYPE_DRAGON, .power = 80 },
    [ITEM_TO_BERRY(ITEM_IAPAPA_BERRY)] = { .type = TYPE_DARK, .power = 80 },
    [ITEM_TO_BERRY(ITEM_RAZZ_BERRY)] = { .type = TYPE_STEEL, .power = 80 },
    [ITEM_TO_BERRY(ITEM_BLUK_BERRY)] = { .type = TYPE_FIRE, .power = 90 },
    [ITEM_TO_BERRY(ITEM_NANAB_BERRY)] = { .type = TYPE_WATER, .power = 90 },
    [ITEM_TO_BERRY(ITEM_WEPEAR_BERRY)] = { .type = TYPE_ELECTRIC, .power = 90 },
    [ITEM_TO_BERRY(ITEM_PINAP_BERRY)] = { .type = TYPE_GRASS, .power = 90 },
    [ITEM_TO_BERRY(ITEM_POMEG_BERRY)] = { .type = TYPE_ICE, .power = 90 },
    [ITEM_TO_BERRY(ITEM_KELPSY_BERRY)] = { .type = TYPE_FIGHTING, .power = 90 },
    [ITEM_TO_BERRY(ITEM_QUALOT_BERRY)] = { .type = TYPE_POISON, .power = 90 },
    [ITEM_TO_BERRY(ITEM_HONDEW_BERRY)] = { .type = TYPE_GROUND, .power = 90 },
    [ITEM_TO_BERRY(ITEM_GREPA_BERRY)] = { .type = TYPE_FLYING, .power = 90 },
    [ITEM_TO_BERRY(ITEM_TAMATO_BERRY)] = { .type = TYPE_PSYCHIC, .power = 90 },
    [ITEM_TO_BERRY(ITEM_CORNN_BERRY)] = { .type = TYPE_BUG, .power = 90 },
    [ITEM_TO_BERRY(ITEM_MAGOST_BERRY)] = { .type = TYPE_ROCK, .power = 90 },
    [ITEM_TO_BERRY(ITEM_RABUTA_BERRY)] = { .type = TYPE_GHOST, .power = 90 },
    [ITEM_TO_BERRY(ITEM_NOMEL_BERRY)] = { .type = TYPE_DRAGON, .power = 90 },
    [ITEM_TO_BERRY(ITEM_SPELON_BERRY)] = { .type = TYPE_DARK, .power = 90 },
    [ITEM_TO_BERRY(ITEM_PAMTRE_BERRY)] = { .type = TYPE_STEEL, .power = 90 },
    [ITEM_TO_BERRY(ITEM_WATMEL_BERRY)] = { .type = TYPE_FIRE, .power = 100 },
    [ITEM_TO_BERRY(ITEM_DURIN_BERRY)] = { .type = TYPE_WATER, .power = 100 },
    [ITEM_TO_BERRY(ITEM_BELUE_BERRY)] = { .type = TYPE_ELECTRIC, .power = 100 },
    [ITEM_TO_BERRY(ITEM_LIECHI_BERRY)] = { .type = TYPE_GRASS, .power = 100 },
    [ITEM_TO_BERRY(ITEM_GANLON_BERRY)] = { .type = TYPE_ICE, .power = 100 },
    [ITEM_TO_BERRY(ITEM_SALAC_BERRY)] = { .type = TYPE_FIGHTING, .power = 100 },
    [ITEM_TO_BERRY(ITEM_PETAYA_BERRY)] = { .type = TYPE_POISON, .power = 100 },
    [ITEM_TO_BERRY(ITEM_APICOT_BERRY)] = { .type = TYPE_GROUND, .power = 100 },
    [ITEM_TO_BERRY(ITEM_LANSAT_BERRY)] = { .type = TYPE_FLYING, .power = 100 },
    [ITEM_TO_BERRY(ITEM_STARF_BERRY)] = { .type = TYPE_PSYCHIC, .power = 100 },
    [ITEM_TO_BERRY(ITEM_OCCA_BERRY)] = { .type = TYPE_FIRE, .power = 80 },
    [ITEM_TO_BERRY(ITEM_PASSHO_BERRY)] = { .type = TYPE_WATER, .power = 80 },
    [ITEM_TO_BERRY(ITEM_WACAN_BERRY)] = { .type = TYPE_ELECTRIC, .power = 80 },
    [ITEM_TO_BERRY(ITEM_RINDO_BERRY)] = { .type = TYPE_GRASS, .power = 80 },
    [ITEM_TO_BERRY(ITEM_YACHE_BERRY)] = { .type = TYPE_ICE, .power = 80 },
    [ITEM_TO_BERRY(ITEM_CHOPLE_BERRY)] = { .type = TYPE_FIGHTING, .power = 80 },
    [ITEM_TO_BERRY(ITEM_KEBIA_BERRY)] = { .type = TYPE_POISON, .power = 80 },
    [ITEM_TO_BERRY(ITEM_SHUCA_BERRY)] = { .type = TYPE_GROUND, .power = 80 },
    [ITEM_TO_BERRY(ITEM_COBA_BERRY)] = { .type = TYPE_FLYING, .power = 80 },
    [ITEM_TO_BERRY(ITEM_PAYAPA_BERRY)] = { .type = TYPE_PSYCHIC, .power = 80 },
    [ITEM_TO_BERRY(ITEM_TANGA_BERRY)] = { .type = TYPE_BUG, .power = 80 },
    [ITEM_TO_BERRY(ITEM_CHARTI_BERRY)] = { .type = TYPE_ROCK, .power = 80 },
    [ITEM_TO_BERRY(ITEM_KASIB_BERRY)] = { .type = TYPE_GHOST, .power = 80 },
    [ITEM_TO_BERRY(ITEM_HABAN_BERRY)] = { .type = TYPE_DRAGON, .power = 80 },
    [ITEM_TO_BERRY(ITEM_COLBUR_BERRY)] = { .type = TYPE_DARK, .power = 80 },
    [ITEM_TO_BERRY(ITEM_BABIRI_BERRY)] = { .type = TYPE_STEEL, .power = 80 },
    [ITEM_TO_BERRY(ITEM_CHILAN_BERRY)] = { .type = TYPE_NORMAL, .power = 80 },
    [ITEM_TO_BERRY(ITEM_MICLE_BERRY)] = { .type = TYPE_ROCK, .power = 100 },
    [ITEM_TO_BERRY(ITEM_CUSTAP_BERRY)] = { .type = TYPE_GHOST, .power = 100 },
    [ITEM_TO_BERRY(ITEM_JABOCA_BERRY)] = { .type = TYPE_DRAGON, .power = 100 },
    [ITEM_TO_BERRY(ITEM_ROWAP_BERRY)] = { .type = TYPE_DARK, .power = 100 },
    [ITEM_TO_BERRY(ITEM_ROSELI_BERRY)] = { .type = TYPE_FAIRY, .power = 80 },
    [ITEM_TO_BERRY(ITEM_KEE_BERRY)] = { .type = TYPE_FAIRY, .power = 100 },
    [ITEM_TO_BERRY(ITEM_MARANGA_BERRY)] = { .type = TYPE_DARK, .power = 100 },
    [ITEM_TO_BERRY(ITEM_ENIGMA_BERRY)] = { .type = TYPE_BUG, .power = 100 },
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
    [B_ACTION_EXEC_SCRIPT] = RunBattleScriptCommands,
    [B_ACTION_TRY_FINISH] = HandleAction_TryFinish,
    [B_ACTION_FINISHED] = HandleAction_ActionFinished,
    [B_ACTION_NOTHING_FAINTED] = HandleAction_NothingIsFainted,
    [B_ACTION_THROW_BALL] = HandleAction_LastUsedBallThrow,
    [B_ACTION_CHEER] = HandleAction_Cheer,
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

void CB2_InitBattle(void)
{
    if (!gTestRunnerEnabled)
        MoveSaveBlocks_ResetHeap();
    
    AllocateBattleResources();
    AllocateBattleSpritesData();
    AllocateMonSpritesGfx();
    
    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
            SetMainCallback2(CB2_PreInitIngamePlayerPartnerBattle);
        else
        {
            HandleLinkBattleSetup();
            SetMainCallback2(CB2_PreInitMultiBattle);
        }
        gBattleCommunication[MULTIUSE_STATE] = 0;
    }
    else
        CB2_InitBattleInternal();
}

static void CB2_InitBattleInternal(void)
{
    u32 i;

    SetHBlankCallback(NULL);
    SetVBlankCallback(NULL);
    
    CpuFill32(0, (void *)VRAM, VRAM_SIZE);
    
    SetGpuReg(REG_OFFSET_MOSAIC, 0);
    SetGpuReg(REG_OFFSET_WIN0H, DISPLAY_WIDTH);
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(DISPLAY_HEIGHT / 2, DISPLAY_HEIGHT / 2 + 1));
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);
    
    gBattle_WIN0H = DISPLAY_WIDTH;
    gBattle_WIN0V = WIN_RANGE(DISPLAY_HEIGHT / 2, DISPLAY_HEIGHT / 2 + 1);
    
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
    
    gAbsentBattlerFlags = 0;
    gCapturedWildMon = FALSE;
    gBattleTerrain = (gBattleTypeFlags & BATTLE_TYPE_RECORDED) ? BATTLE_TERRAIN_BUILDING : BattleSetup_GetTerrainId();
    InitBattleBgsVideo();
    LoadBattleTextboxAndBackground();
    ResetSpriteData();
    ResetTasks();
    DrawBattleEntryBackground();
    FreeAllSpritePalettes();
    gReservedSpritePaletteCount = MAX_BATTLERS_COUNT;
    SetVBlankCallback(VBlankCB_Battle);
    SetUpBattleVars();
    
    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
            SetMainCallback2(CB2_HandleStartMultiPartnerBattle);
        else if (gBattleTypeFlags & BATTLE_TYPE_RAID)
            SetMainCallback2(CB2_HandleStartMultiRaidBattle);
        else
            SetMainCallback2(CB2_HandleStartMultiBattle);
    }
    else
        SetMainCallback2(CB2_HandleStartBattle);
        
    if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED)))
    {
        CreateNPCTrainerParty(gTrainerBattleOpponent_A);
        CalculateEnemyPartyCount();
        SetWildMonsHeldItem();
    }
    gMain.inBattle = TRUE;
    
    for (i = 0; i < PARTY_SIZE; ++i)
        AdjustFriendship(&gPlayerParty[i], FRIENDSHIP_EVENT_LEAGUE_BATTLE);
    
    gBattleCommunication[MULTIUSE_STATE] = 0;
}

#define BUFFER_PARTY_VS_SCREEN_STATUS(party, flags, i)              \
    for ((i) = 0; (i) < PARTY_SIZE; (i)++)                          \
    {                                                               \
        u32 species = GetMonData(&(party)[(i)], MON_DATA_SPECIES2); \
        u32 hp = GetMonData(&(party)[(i)], MON_DATA_HP);            \
        u32 status = GetMonData(&(party)[(i)], MON_DATA_STATUS_ID); \
                                                                    \
        if (!species)                                               \
            continue;                                               \
                                                                    \
        /* Is healthy mon? */                                       \
        if (species != SPECIES_EGG && hp && !status)                \
            (flags) |= 1 << (i) * 2;                                \
                                                                    \
        /* Is Egg or statused? */                                   \
        if (hp && (species == SPECIES_EGG || status))               \
            (flags) |= 2 << (i) * 2;                                \
                                                                    \
        /* Is fainted? */                                           \
        if (species != SPECIES_EGG && !hp)                          \
            (flags) |= 3 << (i) * 2;                                \
    }

static void BufferPartyVsScreenHealth_AtStart(void)
{
    u16 flags = 0;
    u32 i;

    BUFFER_PARTY_VS_SCREEN_STATUS(gPlayerParty, flags, i);
    gBattleStruct->multiBuffer.linkPartnerHeader.vsScreenHealthFlagsLo = flags;
    *(&gBattleStruct->multiBuffer.linkPartnerHeader.vsScreenHealthFlagsHi) = flags >> 8;
}

static void LinkBattleComputeBattleTypeFlags(u32 numPlayers, u32 multiPlayerId)
{
    u32 found = 0;

    // If player 1 is playing the minimum version, player 1 is master.
    if (gBlockRecvBuffer[0][0] == 0x100)
    {
        if (multiPlayerId == 0)
            gBattleTypeFlags |= BATTLE_TYPE_IS_MASTER;
        
        if (!(gBattleTypeFlags & BATTLE_TYPE_RAID))
            gBattleTypeFlags |= BATTLE_TYPE_TRAINER;
        
        ++found;
    }
    
    if (found == 0)
    {
        // If multiple different versions are being used, player 1 is master.
        u32 i;

        for (i = 0; i < numPlayers; ++i)
        {
            if (gBlockRecvBuffer[0][0] != gBlockRecvBuffer[i][0])
                break;
        }
            
        if (i == numPlayers)
        {
            if (multiPlayerId == 0)
                gBattleTypeFlags |= BATTLE_TYPE_IS_MASTER;
            
            if (!(gBattleTypeFlags & BATTLE_TYPE_RAID))
                gBattleTypeFlags |= BATTLE_TYPE_TRAINER;
            
            ++found;
        }
        
        if (found == 0)
        {
            // Lowest index player with the highest game version is master.
            for (i = 0; i < numPlayers; ++i)
            {
                if (gBlockRecvBuffer[i][0] == 0x201 && i != multiPlayerId)
                {
                    if (i < multiPlayerId)
                        break;
                }
                
                if (gBlockRecvBuffer[i][0] > 0x201 && i != multiPlayerId)
                    break;
            }
            
            if (i == numPlayers)
                gBattleTypeFlags |= BATTLE_TYPE_IS_MASTER;
            
            if (!(gBattleTypeFlags & BATTLE_TYPE_RAID))
                gBattleTypeFlags |= BATTLE_TYPE_TRAINER;
        }
    }
}

static void CB2_HandleStartBattle(void)
{
    u32 i, playerMultiplayerId, enemyMultiplayerId;

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
            if (!(gBattleTypeFlags & BATTLE_TYPE_RECORDED))
                gBattleTypeFlags |= BATTLE_TYPE_IS_MASTER;
            
            gBattleCommunication[MULTIUSE_STATE] = 15;
        }
        break;
    case 2:
        if ((GetBlockReceivedStatus() & 3) == 3)
        {
            u32 taskId;

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
                gBattleTypeFlags |= BATTLE_TYPE_LINK_ESTABLISHED;
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

static void PrepareOwnMultiPartnerBuffer(u32 offset)
{
    s32 i, j;
    u8 *nick, *cur;

    for (i = 0; i < (PARTY_SIZE / 2); ++i)
    {
        gMultiPartnerParty[i].species = GetMonData(&gPlayerParty[offset + i], MON_DATA_SPECIES);
        gMultiPartnerParty[i].heldItem = GetMonData(&gPlayerParty[offset + i], MON_DATA_HELD_ITEM);
        nick = gMultiPartnerParty[i].nickname;
        GetMonData(&gPlayerParty[offset + i], MON_DATA_NICKNAME, nick);
        gMultiPartnerParty[i].level = GetMonData(&gPlayerParty[offset + i], MON_DATA_LEVEL);
        gMultiPartnerParty[i].hp = GetMonData(&gPlayerParty[offset + i], MON_DATA_HP);
        gMultiPartnerParty[i].maxhp = GetMonData(&gPlayerParty[offset + i], MON_DATA_MAX_HP);
        gMultiPartnerParty[i].status.id = GetMonData(&gPlayerParty[offset + i], MON_DATA_STATUS_ID);
        gMultiPartnerParty[i].status.counter = GetMonData(&gPlayerParty[offset + i], MON_DATA_STATUS_COUNTER);
        gMultiPartnerParty[i].personality = GetMonData(&gPlayerParty[offset + i], MON_DATA_PERSONALITY);
        gMultiPartnerParty[i].gender = GetMonGender(&gPlayerParty[offset + i]);
        StripExtCtrlCodes(nick);
        if (GetMonData(&gPlayerParty[offset + i], MON_DATA_LANGUAGE) != LANGUAGE_JAPANESE)
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
    s32 i, battlersCount;
    u32 playerMultiplierId, r4;
    u32 *savedBattleTypeFlags;
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
            PrepareOwnMultiPartnerBuffer(0);
            SendBlock(bitmask_all_link_players_but_self(), gBattleStruct->multiBuffer.multiBattleMons, sizeof(gBattleStruct->multiBuffer.multiBattleMons));
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 1:
        r4 = (gBattleTypeFlags & BATTLE_TYPE_RAID) ? 0x3 : 0xF;

        if ((GetBlockReceivedStatus() & r4) == r4)
        {
            ResetBlockReceivedFlags();
            
            battlersCount = (gBattleTypeFlags & BATTLE_TYPE_RAID) ? MAX_BATTLERS_COUNT / 2 : MAX_BATTLERS_COUNT;
            
            for (i = 0; i < battlersCount; ++i)
            {
                if (i == playerMultiplierId)
                    continue;
                
                if ((!(gLinkPlayers[i].id & 1) && !(gLinkPlayers[playerMultiplierId].id & 1))
                 || ((gLinkPlayers[i].id & 1) && (gLinkPlayers[playerMultiplierId].id & 1)))
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

static void CB2_PreInitIngamePlayerPartnerBattle(void)
{
    u32 *savedBattleTypeFlags;
    void (**savedCallback)(void);

    savedCallback = &gBattleStruct->savedCallback;
    savedBattleTypeFlags = &gBattleStruct->savedBattleTypeFlags;

    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    
    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        PrepareOwnMultiPartnerBuffer(MULTI_PARTY_SIZE);
        gBattleCommunication[MULTIUSE_STATE]++;
        *savedCallback = gMain.savedCallback;
        *savedBattleTypeFlags = gBattleTypeFlags;
        gMain.savedCallback = CB2_PreInitIngamePlayerPartnerBattle;
        ShowPartyMenuToShowcaseMultiBattleParty();
        break;
    case 1:
        if (!gPaletteFade.active)
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
    u32 i, playerMultiplayerId, taskId;
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
            break;

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

static void CB2_HandleStartMultiRaidBattle(void)
{
    u32 i, playerMultiplayerId, taskId;
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
        if ((GetBlockReceivedStatus() & 0x3) == 0x3)
        {
            ResetBlockReceivedFlags();
            LinkBattleComputeBattleTypeFlags(2, playerMultiplayerId);
            memcpy(gDecompressionBuffer, gPlayerParty, sizeof(struct Pokemon) * MULTI_PARTY_SIZE);
            ZeroPlayerPartyMons();
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        else
            break;
        // fall through
    case 3:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gDecompressionBuffer, sizeof(struct Pokemon) * 2);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 4:
        if ((GetBlockReceivedStatus() & 0x3) == 0x3)
        {
            ResetBlockReceivedFlags();
            
            for (id = 0; id < MAX_LINK_PLAYERS / 2; id++)
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
                        memcpy(gPlayerParty + MULTI_PARTY_SIZE, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                        break;
                    }
                }
                else
                {
                    switch (gLinkPlayers[id].id)
                    {
                    case 0:
                    case 3:
                        memcpy(gPlayerParty, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                        break;
                    case 1:
                    case 2:
                        memcpy(gPlayerParty + MULTI_PARTY_SIZE, gBlockRecvBuffer[id], sizeof(struct Pokemon) * 2);
                        break;
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
        if ((GetBlockReceivedStatus() & 0x3) == 0x3)
        {
            ResetBlockReceivedFlags();
            
            for (id = 0; id < MAX_LINK_PLAYERS / 2; ++id)
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
                }
            }
            memcpy(gDecompressionBuffer, gEnemyParty, sizeof(struct Pokemon));
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 9:
        if (IsLinkTaskFinished())
        {
            SendBlock(bitmask_all_link_players_but_self(), gDecompressionBuffer, sizeof(struct Pokemon));
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 10:
        if ((GetBlockReceivedStatus() & 0x3) == 0x3)
        {
            ResetBlockReceivedFlags();
            
            if (!(gBattleTypeFlags & BATTLE_TYPE_IS_MASTER))
            {
                for (id = 0; id < MAX_LINK_PLAYERS / 2; id++)
                {
                    if (id != playerMultiplayerId)
                        memcpy(gEnemyParty, gBlockRecvBuffer[id], sizeof(struct Pokemon));
                }
            }
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 13:
        InitBtlControllers();
        ++gBattleCommunication[MULTIUSE_STATE];
        gBattleCommunication[SPRITES_INIT_STATE1] = 0;
        gBattleCommunication[SPRITES_INIT_STATE2] = 0;
        break;
    case 14:
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
    case 11:
        ++gBattleCommunication[MULTIUSE_STATE];
        gBattleCommunication[SPRITES_INIT_STATE1] = 1;
        // fall through
    case 6:
    case 12:
        if (--gBattleCommunication[SPRITES_INIT_STATE1] == 0)
            ++gBattleCommunication[MULTIUSE_STATE];
        break;
    }
}

static void CB2_HandleStartMultiPartnerBattle(void)
{
    u32 playerMultiplayerId, partnerMultiplayerId;

    RunTasks();
    AnimateSprites();
    BuildOamBuffer();

    playerMultiplayerId = GetMultiplayerId();
    gBattleStruct->multiplayerId = playerMultiplayerId;
    partnerMultiplayerId = BATTLE_OPPOSITE(playerMultiplayerId);

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
            
            if (!(gBattleTypeFlags & BATTLE_TYPE_RECORDED))
                gBattleTypeFlags |= BATTLE_TYPE_IS_MASTER;
            
            gBattleCommunication[MULTIUSE_STATE] = 1;
        }
        // fallthrough
    case 1:
        InitBtlControllers();
        gBattleCommunication[SPRITES_INIT_STATE1] = 0;
        gBattleCommunication[SPRITES_INIT_STATE2] = 0;
        gBattleCommunication[MULTIUSE_STATE] = 2;
        break;
    case 2:
        // Finish, start battle
        if (BattleInitAllSprites(&gBattleCommunication[SPRITES_INIT_STATE1], &gBattleCommunication[SPRITES_INIT_STATE2]))
        {
            gPreBattleCallback1 = gMain.callback1;
            gMain.callback1 = BattleMainCB1;
            SetMainCallback2(BattleMainCB2);
        }
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
    DNSApplyFilters(sCombatPalExceptions, NULL, 0); // Don't need define battle palette tag since none battle sprite was affected by dns.
#endif
    
    if (JOY_HELD(B_BUTTON))
    {
        if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
        {
            gSpecialVar_Result = gBattleOutcome = B_OUTCOME_DREW;
            ResetPaletteFade();
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            SetMainCallback2(CB2_QuitPokedudeBattle);
        }
        else if ((gBattleTypeFlags & BATTLE_TYPE_RECORDED) && RecordedBattle_CanStopPlayback())
        {
            // Player pressed B during recorded battle playback, end battle
            gSpecialVar_Result = gBattleOutcome = B_OUTCOME_PLAYER_TELEPORTED;
            ResetPaletteFade();
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            SetMainCallback2(CB2_QuitRecordedBattle);
        }
    }
}

void CB2_SosCall(void)
{
    switch (gMain.state)
    {
        case 0:
            SetVBlankCallback(NULL);
            gMain.state++;
            break;
        case 1:
            ResetVramOamAndBgCntRegs();
            SetGpuReg(REG_OFFSET_DISPCNT, 0);
            ResetBgsAndClearDma3BusyFlags(FALSE);
            ResetAllBgsPos();
            FreeAllWindowBuffers();
            DeactivateAllTextPrinters();
            SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
            gMain.state++;
            break;
        case 2:
            ResetPaletteFade();
            ScanlineEffect_Stop();
            ResetTasks();
            ResetSpriteData();
            gMain.state++;
            break;
        case 3:
            BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
            SetVBlankCallback(VBlankCB_Battle);
            SetMainCallback2(ReshowBattleScreenAfterMenu);
            return;
    }
}

void FreeRestoreBattleData(void)
{
    gMain.callback1 = gPreBattleCallback1;
    gScanlineEffect.state = 3;
    gMain.inBattle = FALSE;
    gBattleTypeFlags = 0;
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

void CB2_QuitRecordedBattle(void)
{
    UpdatePaletteFade();
    
    if (!gPaletteFade.active)
    {
        m4aMPlayStop(&gMPlayInfo_SE1);
        m4aMPlayStop(&gMPlayInfo_SE2);
        
        if (gTestRunnerEnabled)
            TestRunner_Battle_AfterLastTurn();
        
        FreeRestoreBattleData();
        FreeAllWindowBuffers();
        SetMainCallback2(gMain.savedCallback);
    }
}

static u32 GenerateOtIdForNPCTrainer(const u8 *trainerName)
{
    u32 i;
    u32 firstIdPart = 0, secondIdPart = 0, thirdIdPart = 0;
    
    for (i = 0; trainerName[i] != EOS && i < 3; i++)
    {
        switch (i)
        {
            case 0:
                firstIdPart = trainerName[i];
                // fallthrough
            case 1:
                secondIdPart = trainerName[i];
                // fallthrough
            case 2:
                thirdIdPart = trainerName[i];
                break;
        }
    }
    return ((firstIdPart % 72) * 1000) + ((secondIdPart % 23) * 10) + (thirdIdPart % 37) % 65536;
}

static void CreateTrainerMon(struct Pokemon *mon, const struct TrainerMon *partyData, u32 trainerClass, u32 fixedOtId)
{
    u32 i;
    struct PokemonGenerator generator =
    {
        .species = partyData->species,
        .level = (gBattleTypeFlags & BATTLE_TYPE_RAID) ? GetRaidRecommendedLevel(gNumRaidBattleStars) : GetTrainerPartyMonLevel(partyData),
        .otIdType = OT_ID_PRESET,
        .fixedOtId = fixedOtId,
        .shinyType = partyData->isShiny ? GENERATE_SHINY_FORCED : GENERATE_SHINY_LOCKED,
        .shinyRollType = SHINY_ROLL_NORMAL,
        .forcedGender = MON_GENDERLESS,
        .hasFixedPersonality = FALSE,
        .fixedPersonality = 0,
        .forcedNature = partyData->nature ? partyData->nature - 1 : NUM_NATURES,
        .formChanges = GENERATOR_FORMS(FORM_CHANGE_GENDER),
        .nPerfectIvs = 0,
    };
    
    if (partyData->gender)
    {
        switch (partyData->gender)
        {
            case TRAINER_MON_MALE:
                generator.forcedGender = MON_MALE;
                break;
            case TRAINER_MON_FEMALE:
                generator.forcedGender = MON_FEMALE;
                break;
        }
    }
    memcpy(generator.moves, partyData->moves, sizeof(generator.moves));
    
    // Create mon
    CreateMon(mon, generator);
    
    // Set held item
    SetMonData(mon, MON_DATA_HELD_ITEM, &partyData->heldItem);
    
    // Give nickname
    if (partyData->nickname != NULL)
        SetMonData(mon, MON_DATA_NICKNAME, partyData->nickname);
    
    // Set ivs
    if (partyData->iv)
        SetMonData(mon, MON_DATA_IVS, &partyData->iv);
    
    // Set evs
    if (partyData->ev != NULL)
    {
        for (i = 0; i < NUM_STATS; i++)
            SetMonData(mon, MON_DATA_HP_EV + i, &partyData->ev[i]);
    }
    
    // Set ability num or hidden
    if (partyData->abilityNum)
    {
        // Set ability hidden
        if (partyData->abilityNum == 3)
        {
            i = TRUE;
            SetMonData(mon, MON_DATA_ABILITY_HIDDEN, &i);
        }
        else // Set ability num
        {
            i = partyData->abilityNum - 1;
            SetMonData(mon, MON_DATA_ABILITY_NUM, &i);
        }
    }
    // Calculate stats
    CalculateMonStats(mon);
    
    // Give the pokeball Id
#if CLASS_BASED_BALLS
    i = ITEM_TO_BALL(gTrainerClassTable[trainerClass].pokeball);
#else
    if (partyData->ballId)
        i = ITEM_TO_BALL(partyData->ballId);
    else
        i = ITEM_TO_BALL(gTrainerClassTable[trainerClass].pokeball);
#endif

    SetMonData(mon, MON_DATA_POKEBALL, &i);
}

static void CreateNPCTrainerParty(u32 trainerNum)
{
    u32 i, otId;
    
    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
    {
        ZeroEnemyPartyMons();
        
        switch (gTrainers[trainerNum].battleType)
        {
            case TRAINER_BATTLE_TYPE_DOUBLE:
                gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;
                break;
            case TRAINER_BATTLE_TYPE_TWO_VS_ONE:
                gBattleTypeFlags |= (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_VS_ONE);
                break;
        }
        otId = GenerateOtIdForNPCTrainer(gTrainers[trainerNum].trainerName);

        for (i = 0; i < gTrainers[trainerNum].partySize; i++)
            CreateTrainerMon(&gEnemyParty[i], &gTrainers[trainerNum].party[i], gTrainers[trainerNum].trainerClass, otId);
        
        gBattleStruct->battleChallenge = gTrainers[trainerNum].battleChallenge;
    }
}

void FillPartnerParty(u32 trainerId)
{
    u32 i, otId;
    const u8 *partnerName = gBattlePartners[trainerId].trainerName;
    
    // Zero player party for partner
    for (i = 0; i < MULTI_PARTY_SIZE; i++)
        ZeroMonData(&gPlayerParty[MULTI_PARTY_SIZE + i]);
    
    otId = GenerateOtIdForNPCTrainer(partnerName);

    for (i = 0; i < MULTI_PARTY_SIZE && i < gBattlePartners[trainerId].partySize; i++)
    {
        CreateTrainerMon(&gPlayerParty[MULTI_PARTY_SIZE + i], &gBattlePartners[trainerId].party[i], gBattlePartners[trainerId].trainerClass, otId);
        SetMonData(&gPlayerParty[MULTI_PARTY_SIZE + i], MON_DATA_OT_NAME, partnerName);
    }
}

void VBlankCB_Battle(void)
{
#if NO_SAVE_STATE_RNG_MANIPULATION == FALSE
    // Change gRngSeed every vblank.
    if (!(gBattleTypeFlags & BATTLE_TYPE_RECORDED))
        Random();
#endif

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
        DestroySpriteAndFreeMatrix(sprite);
    }
}

void SpriteCB_VsLetterInit(struct Sprite *sprite)
{
    StartSpriteAffineAnim(sprite, 1);
    sprite->callback = SpriteCB_VsLetter;
    PlaySE(SE_MUGSHOT);
}

static void BufferPartyVsScreenHealth_AtEnd(u32 taskId)
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
    u32 i;
    bool32 showResults = (!(gBattleTypeFlags & BATTLE_TYPE_RAID));

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
    
    if (showResults)
    {
        LoadCompressedPalette(gBattleTextboxPalette, 0, 64);
        LoadBattleMenuWindowGfx();
        DrawBattleEntryBackground();
    }
    ResetSpriteData();
    ResetTasks();
    SetGpuReg(REG_OFFSET_WINOUT, WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 | WININ_WIN0_OBJ | WININ_WIN0_CLR);
    FreeAllSpritePalettes();
    gReservedSpritePaletteCount = MAX_BATTLERS_COUNT;
    SetVBlankCallback(VBlankCB_Battle);
    
    if (showResults)
    {
        u32 taskId = CreateTask(InitLinkBattleVsScreen, 0);
        gTasks[taskId].data[1] = 270;
        gTasks[taskId].data[2] = 90;
        gTasks[taskId].data[5] = 1;
        BufferPartyVsScreenHealth_AtEnd(taskId);
    }
    SetMainCallback2(CB2_EndLinkBattle);
    
    gBattleCommunication[SPRITES_INIT_STATE1] = 0xFF;
    gBattleCommunication[MULTIUSE_STATE] = showResults ? 0 : 1;
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
    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        ShowBg(0);
        ShowBg(1);
        ShowBg(2);
        ++gBattleCommunication[MULTIUSE_STATE];
        break;
    case 1:
        if (--gBattleCommunication[SPRITES_INIT_STATE1] == 0)
        {
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 2:
        if (!gPaletteFade.active)
        {
            FreeMonSpritesGfx();
            FreeBattleSpritesData();
            FreeBattleResources();
            BattleEndClearFlags();
        }
        break;
    }
}

u32 GetBattleBgAttribute(u32 arrayId, u32 caseId)
{
    u32 ret;

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
    default:
        ret = 0;
        break;
    }
    return ret;
}

#define sBattler            data[0]

void SpriteCB_EnemyMon(struct Sprite *sprite)
{
    StartSpriteAnimIfDifferent(sprite, 0);
    
    if (IsDoubleBattleForBattler(sprite->sBattler))
        BeginNormalPaletteFade((0x10000 << sprite->sBattler) | (0x10000 << BATTLE_PARTNER(sprite->sBattler)), 0, 10, 10, RGB(8, 8, 8));
    else
        BeginNormalPaletteFade((0x10000 << sprite->sBattler), 0, 10, 10, RGB(8, 8, 8));
    
    sprite->callback = SpriteCB_MoveWildMonToRight;
}

static void SpriteCB_WildMonWaitCry(struct Sprite *sprite)
{
    if (IsCryFinished())
    {
        PlayCry_Normal(GetBattlerVisualSpecies(sprite->sBattler), 25);
        sprite->callback = SpriteCB_WildMonShowHealthbox;
    }
}

static void SpriteCB_MoveWildMonToRight(struct Sprite *sprite)
{
    if (!(gIntroSlideFlags & INTRO_SLIDE_FLAG_SLIDE_IN_COUNTER))
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
        SetHealthboxSpriteVisibility(gHealthboxSpriteIds[sprite->sBattler], FALSE);
        StartSpriteAnimIfDifferent(sprite, 0);
        
        if (IsDoubleBattleForBattler(sprite->sBattler))
            BeginNormalPaletteFade((0x10000 << sprite->sBattler) | (0x10000 << BATTLE_PARTNER(sprite->sBattler)), 0, 10, 0, RGB(8, 8, 8));
        else
            BeginNormalPaletteFade((0x10000 << sprite->sBattler), 0, 10, 0, RGB(8, 8, 8));
        
        gIntroSlideFlags &= ~(INTRO_SLIDE_FLAG_SLIDE_IN_STARTED);
        sprite->callback = SpriteCallbackDummy2;
    }
}

void SpriteCallbackDummy2(struct Sprite *sprite)
{
}

void SpriteCB_FaintOpponentMon(struct Sprite *sprite)
{
    sprite->data[3] = 8 - gSpeciesInfo[GetBattlerVisualSpecies(sprite->sBattler)].frontPicYOffset / 8;
    sprite->data[4] = 1;
    sprite->callback = SpriteCB_AnimFaintOpponent;
}

static void SpriteCB_AnimFaintOpponent(struct Sprite *sprite)
{
    u32 i;

    if (--sprite->data[4] == 0)
    {
        sprite->data[4] = 2;
        sprite->y2 += 8; // Move the sprite down.
        
        if (--sprite->data[3] < 0)
            DestroySpriteAndFreeMatrix(sprite);
        else // Erase bottom part of the sprite to create a smooth illusion of mon falling down.
        {
            u8 *dst = (u8 *)gMonSpritesGfxPtr->battlers[GetBattlerPosition(sprite->sBattler)].sprite + (sprite->data[3] << 8);

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
        sprite->invisible ^= TRUE;
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
    if (!(gIntroSlideFlags & INTRO_SLIDE_FLAG_SLIDE_IN_COUNTER))
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
    if (!(gIntroSlideFlags & INTRO_SLIDE_FLAG_SLIDE_IN_COUNTER))
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

void DoBounceEffect(u32 battler, u32 which, s8 delta, s8 amplitude)
{
    u32 invisibleSpriteId, bouncerSpriteId;

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

void EndBounceEffect(u32 battler, u32 which)
{
    u32 bouncerSpriteId;

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
    gBattleCommunication[SPRITES_INIT_STATE2] = 0;
    gBattleMainFunc = DoBattleIntro;
}

static void BattleMainCB1(void)
{
    u32 i;
    
    gBattleMainFunc();
    
    for (i = 0; i < gBattlersCount; ++i)
        gBattlerControllersData[i].func(i);
}

static void ClearBattlerEffectsOnFaintOrSwitch(u32 battlerId)
{
    u32 i;
    
    for (i = 0; i < gBattlersCount; i++)
    {
        // Free all mons trapped by the user
        if ((gBattleMons[i].status2 & STATUS2_ESCAPE_PREVENTION) && gDisableStructs[i].battlerPreventingEscape == battlerId)
            gBattleMons[i].status2 &= ~(STATUS2_ESCAPE_PREVENTION);
        
        // Free all mons infatuated by the user
        if ((gBattleMons[i].status2 & STATUS2_INFATUATION) && gDisableStructs[i].infatuatedWith == battlerId)
            gBattleMons[i].status2 &= ~(STATUS2_INFATUATION);
        
        // Free all mons wrapped by the user
        if (gDisableStructs[i].wrapTurns && gDisableStructs[i].wrappedBy == battlerId && !gDisableStructs[i].wrappedByGMaxEffect)
            gDisableStructs[i].wrapTurns = 0;
        
        // Reset all last taken moves
        if (i != battlerId)
            gBattleStruct->battlers[i].lastTakenMove = MOVE_NONE;
        
        gBattleStruct->battlers[i].lastTakenMoveFrom[battlerId] = MOVE_NONE;
    }
    // Clear menu selections
    gActionSelectionCursor[battlerId] = 0;
    gBattleStruct->battlers[battlerId].moveSelectionCursor = 0;
    gBattleStruct->battlers[battlerId].gMaxChiStrikeBoost = 0;
    gBattleStruct->battlers[battlerId].cheerCritBoost = FALSE;
    
    // Clear move vars
    gBattleStruct->battlers[battlerId].lastHitBattler = 0xFF;
    gBattleStruct->battlers[battlerId].lastHitMoveType = 0;
    gBattleStruct->battlers[battlerId].lastResultingMove = MOVE_NONE;
    gBattleStruct->battlers[battlerId].lastLandedMove = MOVE_NONE;
    gBattleStruct->battlers[battlerId].lastMove = MOVE_NONE;
    gBattleStruct->battlers[battlerId].lastPrintedMove = MOVE_NONE;
    gBattleStruct->battlers[battlerId].lastTakenMove = MOVE_NONE;
    gBattleStruct->battlers[battlerId].choicedMove = MOVE_NONE;
    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
        gBattleStruct->battlers[battlerId].lastTakenMoveFrom[i] = MOVE_NONE;
    
    AI_ClearRecordedMovesByBattler(battlerId);
}

static void BattleStartClearSetData(void)
{
    u32 i, catchRate;
    
    // Clear ram locs
    TurnValuesCleanUp(FALSE);
    memset(&gSpecialStatuses, 0, sizeof(gSpecialStatuses));
    memset(&gSideStatuses, 0, sizeof(gSideStatuses));
    memset(&gSideTimers, 0, sizeof(gSideTimers));
    memset(&gStatuses3, 0, sizeof(gStatuses3));
    memset(&gDisableStructs, 0, sizeof(gDisableStructs));
    memset(&gFieldStatus, 0, sizeof(gFieldStatus));
    
    // Reset datas by battler
    for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
    {
        gDisableStructs[i].isFirstTurn = 2;
        
        // Clear Illusion
        ClearIllusionMon(i);
        
        // Clear battler effects
        ClearBattlerEffectsOnFaintOrSwitch(i);
        
        gBattleStruct->pickupStack[i] = 0xFF;
        gBattleStruct->battlers[i].AI_monToSwitchIntoId = PARTY_SIZE;
    }
    gBattlerAttacker = 0;
    gBattlerTarget = 0;
    gBattleWeather = 0;
    gHitMarker = 0;
    
    gBattleScripting.battleStyle = gSaveBlock2Ptr->optionsBattleStyle;
    gBattleScripting.expOnCatch = EXP_ON_CAUGHT;
    gBattleScripting.savedDmg = 0;
    gBattleScripting.animTurn = 0;
    gBattleScripting.animTargetsHit = 0;
    
    gMultiHitCounter = 0;
    gBattleOutcome = 0;
    gBattleControllerExecFlags = 0;
    
    for (i = 0; i < BATTLE_COMMUNICATION_ENTRIES_COUNT; ++i)
        gBattleCommunication[i] = 0;
    
    gLastUsedMove = MOVE_NONE;
    gLastUsedBaseMove = MOVE_NONE;
    gPauseCounterBattle = 0;
    gBattleMoveDamage = 0;
    gLeveledUpInBattle = 0;
    
    // Safari battles can't be double battles, so this don't need to be changed
    catchRate = gSpeciesInfo[GetMonData(&gEnemyParty[0], MON_DATA_SPECIES)].catchRate;
    
    gBattleStruct->safariCatchFactor = catchRate * 100 / 1275;
    
    gBattleStruct->safariEscapeFactor = catchRate / 2 / gBattleStruct->safariCatchFactor;
    if (gBattleStruct->safariEscapeFactor <= 1)
        gBattleStruct->safariEscapeFactor = 2;
    
    // Clear datas by party slot
    for (i = 0; i < PARTY_SIZE; i++)
        gPartyCriticalHits[i] = 0;
}

void SwitchInClearSetData(u32 battlerId)
{
    u32 i;
    struct DisableStruct disableStructCopy = gDisableStructs[battlerId];
    
    // Clear Illusion
    ClearIllusionMon(battlerId);
    
    // Clear battler effects
    ClearBattlerEffectsOnFaintOrSwitch(battlerId);

    // Clear disable struct
    memset(&gDisableStructs[battlerId], 0, sizeof(struct DisableStruct));
    gDisableStructs[battlerId].isFirstTurn = 2;
    gDisableStructs[battlerId].canProteanActivate = TRUE;
    
    gStatuses3[battlerId] &= ~(STATUS3_ALWAYS_HITS);
    
    for (i = 0; i < gBattlersCount; ++i)
    {
        // Reset all mons with always hit flag
        if ((gStatuses3[i] & STATUS3_ALWAYS_HITS) && gDisableStructs[i].battlerWithSureHit == battlerId)
            gStatuses3[i] &= ~(STATUS3_ALWAYS_HITS);
    }
    
    if (gBattleMoves[gCurrentMove].effect == EFFECT_BATON_PASS)
    {
        // Status passed by Baton Pass
        gBattleMons[battlerId].status2 &= (STATUS2_CONFUSION | STATUS2_FOCUS_ENERGY | STATUS2_SUBSTITUTE | STATUS2_ESCAPE_PREVENTION | STATUS2_CURSED | STATUS2_AQUA_RING);
        gStatuses3[battlerId] &= (STATUS3_LEECHSEED | STATUS3_PERISH_SONG | STATUS3_POWER_TRICK | STATUS3_HEAL_BLOCK | STATUS3_EMBARGO | STATUS3_ROOTED | STATUS3_GASTRO_ACID
                                 | STATUS3_MAGNET_RISE | STATUS3_TELEKINESIS);
        
        HANDLE_POWER_TRICK_SWAP(battlerId)
        
        if (gAbilities[gBattleMons[battlerId].ability].cantBeSuppressed)
            gStatuses3[battlerId] &= ~(STATUS3_GASTRO_ACID);
        
        // Only Mega Evolved pokémons remove this effect on Baton Pass's switch in
        if (GetActiveGimmick(battlerId) == GIMMICK_MEGA && gSpeciesInfo[GetMonData(GetBattlerPartyIndexPtr(battlerId), MON_DATA_SPECIES)].suppressEnemyShadow)
            gStatuses3[battlerId] &= ~(STATUS3_TELEKINESIS);
        
        gDisableStructs[battlerId].substituteHP = disableStructCopy.substituteHP;
        gDisableStructs[battlerId].perishSongTimer = disableStructCopy.perishSongTimer;
        gDisableStructs[battlerId].battlerPreventingEscape = disableStructCopy.battlerPreventingEscape;
        gDisableStructs[battlerId].embargoTimer = disableStructCopy.embargoTimer;
        gDisableStructs[battlerId].healBlockTimer = disableStructCopy.healBlockTimer;
        gDisableStructs[battlerId].leechSeedBattler = disableStructCopy.leechSeedBattler;
        gDisableStructs[battlerId].magnetRiseTimer = disableStructCopy.magnetRiseTimer;
        gDisableStructs[battlerId].telekinesisTimer = disableStructCopy.telekinesisTimer;
    }
    else
    {
        TryResetBattlerStatChanges(battlerId);
        
        // Clear battler status
        gBattleMons[battlerId].status2 = 0;
        gStatuses3[battlerId] = 0;
    }
    // Reset general datas
    gMoveResultFlags = 0;
    gCurrentMove = MOVE_NONE;
}

void FaintClearSetData(u32 battlerId)
{
    struct ProtectStruct savedProtectStruct;
    
    // Reset battler stats
    TryResetBattlerStatChanges(battlerId);
    
    // Clear battler status
    gBattleMons[battlerId].status2 = 0;
    gStatuses3[battlerId] = 0;
    
    // Clear battler effects
    ClearBattlerEffectsOnFaintOrSwitch(battlerId);

    // Clear disable struct
    memset(&gDisableStructs[battlerId], 0, sizeof(struct DisableStruct));
    gDisableStructs[battlerId].isFirstTurn = 2;
    
    // Clear protect data, except for Physical/Special dmg vars
    savedProtectStruct = gProtectStructs[battlerId];
    memset(&gProtectStructs[battlerId], 0, sizeof(struct ProtectStruct));
    
    gProtectStructs[battlerId].physicalBattlerId = savedProtectStruct.physicalBattlerId;
    gProtectStructs[battlerId].physicalDmg = savedProtectStruct.physicalDmg;
    
    gProtectStructs[battlerId].specialBattlerId = savedProtectStruct.specialBattlerId;
    gProtectStructs[battlerId].specialDmg = savedProtectStruct.specialDmg;
    
    // Set battler initial types
    SetBattlerInitialTypes(battlerId);
    
    if (IsBattlerTotemPokemon(battlerId))
        gBattleStruct->sos.totemBattlerId = MAX_BATTLERS_COUNT;
}

enum
{
    BATTLE_INTRO_STATE_REVERT_FORMS,
    BATTLE_INTRO_STATE_FORMS_REVERT_LOOP,
    BATTLE_INTRO_STATE_RAID_BOSS_FORM,
    BATTLE_INTRO_STATE_GET_MON_DATA,
    BATTLE_INTRO_STATE_LOOP_BATTLER_DATA,
    BATTLE_INTRO_STATE_RAID_BOSS_GIMMICK,
    BATTLE_INTRO_STATE_RAID_BOSS_GIMMICK_INDICATOR,
    BATTLE_INTRO_STATE_PREPARE_BG_SLIDE,
    BATTLE_INTRO_STATE_DRAW_SPRITES,
    BATTLE_INTRO_STATE_DRAW_PARTY_SUMMARY,
    BATTLE_INTRO_STATE_TRAINER_TEXT,
    BATTLE_INTRO_STATE_OPPONENT_SEND_OUT_TEXT,
    BATTLE_INTRO_STATE_OPPONENT_SEND_OUT_ANIM,
    BATTLE_INTRO_STATE_WILD_TEXT,
    BATTLE_INTRO_STATE_REVEAL_GHOST,
    BATTLE_INTRO_STATE_PLAYER_SEND_OUT_TEXT,
    BATTLE_INTRO_STATE_PLAYER_SEND_OUT_ANIM,
    BATTLE_INTRO_STATE_END,
};

static void DoBattleIntro(void)
{
    u32 battlerId;
    
    switch (gBattleCommunication[MULTIUSE_STATE])
    {
        case BATTLE_INTRO_STATE_REVERT_FORMS:
            if (!gBattleControllerExecFlags)
            {
                u32 side = gBattleCommunication[SPRITES_INIT_STATE2];
                u32 partyId = gBattleCommunication[SPRITES_INIT_STATE1];
                struct Pokemon *mon = &GetSideParty(side)[partyId];
                
                if (IsMonValidSpecies(mon))
                {
                    gBattleStruct->sides[side].party[partyId].itemEffects.savedItem = GetMonData(mon, MON_DATA_HELD_ITEM);
                    
                    if (side == B_SIDE_PLAYER)
                        battlerId = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
                    else
                        battlerId = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
                    
                    BtlController_EmitPartyFormChange(battlerId, BUFFER_A, partyId, FORMCHANGE_SAVE_ORIGINAL_FORM);
                    MarkBattlerForControllerExec(battlerId);
                }
                gBattleCommunication[MULTIUSE_STATE]++;
            }
            break;
        case BATTLE_INTRO_STATE_FORMS_REVERT_LOOP:
            if (++gBattleCommunication[SPRITES_INIT_STATE1] == PARTY_SIZE)
            {
                gBattleCommunication[SPRITES_INIT_STATE1] = 0;
                
                if (++gBattleCommunication[SPRITES_INIT_STATE2] == B_SIDE_COUNT)
                    gBattleCommunication[MULTIUSE_STATE]++;
                else
                    gBattleCommunication[MULTIUSE_STATE]--;
            }
            else
                gBattleCommunication[MULTIUSE_STATE]--;
            break;
        case BATTLE_INTRO_STATE_RAID_BOSS_FORM:
            if (!gBattleControllerExecFlags)
            {
                battlerId = gBattleStruct->sos.totemBattlerId;
                
                if ((gBattleTypeFlags & BATTLE_TYPE_RAID) && GetMonData(GetBattlerPartyIndexPtr(battlerId), MON_DATA_GIGANTAMAX_FACTOR))
                {
                    BtlController_EmitPartyFormChange(battlerId, BUFFER_A, gBattlerPartyIndexes[battlerId], FORMCHANGE_CHANGE_RAID_BOSS_FORM);
                    MarkBattlerForControllerExec(battlerId);
                }
            }
            gBattleCommunication[MULTIUSE_STATE]++;
            break;
        case BATTLE_INTRO_STATE_GET_MON_DATA:
            battlerId = gBattleCommunication[SPRITES_INIT_STATE1];
            BtlController_EmitGetMonData(battlerId, BUFFER_A, REQUEST_ALL_BATTLE, 0);
            MarkBattlerForControllerExec(battlerId);
            gBattleCommunication[MULTIUSE_STATE]++;
            break;
        case BATTLE_INTRO_STATE_LOOP_BATTLER_DATA:
            if (!gBattleControllerExecFlags)
            {
                if (++gBattleCommunication[SPRITES_INIT_STATE1] == gBattlersCount)
                {
                    gBattleCommunication[SPRITES_INIT_STATE1] = 0;
                    gBattleCommunication[MULTIUSE_STATE]++;
                }
                else
                    gBattleCommunication[MULTIUSE_STATE]--;
            }
            break;
        case BATTLE_INTRO_STATE_RAID_BOSS_GIMMICK:
            if (!gBattleControllerExecFlags)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_RAID)
                {
                    BtlController_EmitGimmickState(gBattleStruct->sos.totemBattlerId, BUFFER_A, STATE_ACTIVE_GIMMICK, GIMMICK_DYNAMAX);
                    MarkBattlerForControllerExec(gBattleStruct->sos.totemBattlerId);
                    gBattleCommunication[MULTIUSE_STATE]++;
                }
                else
                    gBattleCommunication[MULTIUSE_STATE] = BATTLE_INTRO_STATE_PREPARE_BG_SLIDE;
            }
            break;
        case BATTLE_INTRO_STATE_RAID_BOSS_GIMMICK_INDICATOR:
            if (!gBattleControllerExecFlags)
            {
                BtlController_EmitHealthBoxUpdate(gBattleStruct->sos.totemBattlerId, BUFFER_A, HEALTHBOX_LEVEL);
                MarkBattlerForControllerExec(gBattleStruct->sos.totemBattlerId);
                gBattleCommunication[MULTIUSE_STATE]++;
            }
            break;
        case BATTLE_INTRO_STATE_PREPARE_BG_SLIDE:
            if (!gBattleControllerExecFlags)
            {
                battlerId = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
                BtlController_EmitIntroSlide(battlerId, BUFFER_A, gBattleTerrain);
                MarkBattlerForControllerExec(battlerId);
                gBattleCommunication[MULTIUSE_STATE]++;
            }
            break;
        case BATTLE_INTRO_STATE_DRAW_SPRITES:
            if (!gBattleControllerExecFlags)
            {
                for (battlerId = 0; battlerId < gBattlersCount; battlerId++)
                {
                    if ((gBattleTypeFlags & BATTLE_TYPE_SAFARI) && GetBattlerSide(battlerId) == B_SIDE_PLAYER)
                        memset(&gBattleMons[battlerId], 0, sizeof(struct BattlePokemon));
                    else
                    {
                        memcpy(&gBattleMons[battlerId], &gBattleBufferB[battlerId][4], sizeof(struct BattlePokemon));
                        SetBattlerInitialTypes(battlerId);
                        gBattleMons[battlerId].ability = GetAbilityBySpecies(gBattleMons[battlerId].species, gBattleMons[battlerId].abilityNum, gBattleMons[battlerId].abilityHidden);
                        gBattleStruct->sides[GetBattlerSide(battlerId)].hpOnSwitchout = gBattleMons[battlerId].hp;
                        TryResetBattlerStatChanges(battlerId);
                        gBattleMons[battlerId].status2 = 0;
                    }

                    switch (GetBattlerPosition(battlerId))
                    {
                        case B_POSITION_PLAYER_LEFT:
                            BtlController_EmitDrawTrainerPic(battlerId, BUFFER_A);
                            MarkBattlerForControllerExec(battlerId);
                            break;
                        case B_POSITION_OPPONENT_LEFT:
                            if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                                BtlController_EmitDrawTrainerPic(battlerId, BUFFER_A);
                            else
                                BtlController_EmitLoadMonSprite(battlerId, BUFFER_A);
                            
                            MarkBattlerForControllerExec(battlerId);
                            break;
                        case B_POSITION_PLAYER_RIGHT:
                            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                            {
                                BtlController_EmitDrawTrainerPic(battlerId, BUFFER_A);
                                MarkBattlerForControllerExec(battlerId);
                            }
                            break;
                        case B_POSITION_OPPONENT_RIGHT:
                            if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                            {
                                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                                {
                                    BtlController_EmitDrawTrainerPic(battlerId, BUFFER_A);
                                    MarkBattlerForControllerExec(battlerId);
                                }
                            }
                            else if (IsBattlerAlive(battlerId))
                            {
                                BtlController_EmitLoadMonSprite(battlerId, BUFFER_A);
                                MarkBattlerForControllerExec(battlerId);
                            }
                            break;
                    }
                    
                    if (GetBattlerSide(battlerId) == B_SIDE_OPPONENT)
                    {
                        if (gBattleTypeFlags & (BATTLE_TYPE_GHOST | BATTLE_TYPE_GHOST_UNVEILED))
                        {
                            if (!IS_BATTLE_TYPE_GHOST_WITHOUT_SCOPE)
                                HandleSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[battlerId].species), FLAG_SET_SEEN, gBattleMons[battlerId].personality);
                        }
                        else if (!(gBattleTypeFlags & (BATTLE_TYPE_POKEDUDE | BATTLE_TYPE_LINK | BATTLE_TYPE_OLD_MAN_TUTORIAL | BATTLE_TYPE_RECORDED)))
                            HandleSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[battlerId].species), FLAG_SET_SEEN, gBattleMons[battlerId].personality);
                    }
                }
                
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                    gBattleCommunication[MULTIUSE_STATE]++;
                else // Skip party summary since it is a wild battle
                    gBattleCommunication[MULTIUSE_STATE] = BATTLE_INTRO_STATE_WILD_TEXT;
            }
            break;
        case BATTLE_INTRO_STATE_DRAW_PARTY_SUMMARY:
            if (!gBattleControllerExecFlags)
            {
                u32 i, j;
                struct Pokemon *party;
                struct HpAndStatus hpStatus[PARTY_SIZE];
                
                for (i = 0; i < B_SIDE_COUNT; i++)
                {
                    party = GetSideParty(i);
                    
                    for (j = 0; j < PARTY_SIZE; ++j)
                    {
                        if (!IsMonValidSpecies(&party[j]))
                        {
                            hpStatus[j].hp = 0xFFFF;
                            hpStatus[j].status.id = 0;
                            hpStatus[j].status.counter = 0;
                        }
                        else
                        {
                            hpStatus[j].hp = GetMonData(&party[j], MON_DATA_HP);
                            hpStatus[j].status.id = GetMonData(&party[j], MON_DATA_STATUS_ID);
                            hpStatus[j].status.counter = GetMonData(&party[j], MON_DATA_STATUS_COUNTER);
                        }
                    }
                    
                    if (i == B_SIDE_PLAYER)
                        battlerId = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
                    else
                        battlerId = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
                    
                    BtlController_EmitDrawPartyStatusSummary(battlerId, BUFFER_A, hpStatus, PARTY_SUMM_SKIP_DRAW_DELAY);
                    MarkBattlerForControllerExec(battlerId);
                }
                gBattleCommunication[MULTIUSE_STATE]++;
            }
            break;
        case BATTLE_INTRO_STATE_TRAINER_TEXT:
            if (!gBattleControllerExecFlags)
            {
                PrepareStringBattle(STRINGID_INTROMSG, GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT));
                gBattleCommunication[MULTIUSE_STATE]++;
            }
            break;
        case BATTLE_INTRO_STATE_OPPONENT_SEND_OUT_TEXT:
            if (!gBattleControllerExecFlags)
            {
                PrepareStringBattle(STRINGID_INTROSENDOUT, GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT));
                gBattleCommunication[MULTIUSE_STATE]++;
            }
            break;
        case BATTLE_INTRO_STATE_OPPONENT_SEND_OUT_ANIM:
            if (!gBattleControllerExecFlags)
            {
                for (battlerId = 0; battlerId < gBattlersCount; battlerId++)
                {
                    if (IsBattlerAlive(battlerId))
                    {
                        switch (GetBattlerPosition(battlerId))
                        {
                            case B_POSITION_OPPONENT_LEFT:
                                BtlController_EmitIntroTrainerBallThrow(battlerId, BUFFER_A);
                                MarkBattlerForControllerExec(battlerId);
                                break;
                            case B_POSITION_OPPONENT_RIGHT:
                                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                                {
                                    BtlController_EmitIntroTrainerBallThrow(battlerId, BUFFER_A);
                                    MarkBattlerForControllerExec(battlerId);
                                }
                                break;
                        }
                        
                        if (GetBattlerSide(battlerId) == B_SIDE_OPPONENT && !(gBattleTypeFlags & (BATTLE_TYPE_POKEDUDE | BATTLE_TYPE_LINK | BATTLE_TYPE_GHOST | BATTLE_TYPE_OLD_MAN_TUTORIAL)))
                            HandleSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[battlerId].species), FLAG_SET_SEEN, gBattleMons[battlerId].personality);
                    }
                }
                gBattleCommunication[MULTIUSE_STATE] = BATTLE_INTRO_STATE_PLAYER_SEND_OUT_TEXT;
            }
            break;
        case BATTLE_INTRO_STATE_WILD_TEXT:
            if (!gBattleControllerExecFlags)
            {
                PrepareStringBattle(STRINGID_INTROMSG, GetBattlerAtPosition(B_POSITION_PLAYER_LEFT));
                gBattleCommunication[MULTIUSE_STATE]++;
            }
            break;
        case BATTLE_INTRO_STATE_REVEAL_GHOST:
            if (!gBattleControllerExecFlags)
            {
                if (IS_BATTLE_TYPE_GHOST_WITH_SCOPE)
                {
                    gLastUsedItem = ITEM_SILPH_SCOPE;
                    gBattlerTarget = gBattleStruct->sos.totemBattlerId;
                    BattleScriptExecute(BattleScript_ItemUnveiledGhost);
                }
                gBattleCommunication[MULTIUSE_STATE]++;
            }
            break;
        case BATTLE_INTRO_STATE_PLAYER_SEND_OUT_TEXT:
            if (!gBattleControllerExecFlags)
            {
                if (!(gBattleTypeFlags & BATTLE_TYPE_SAFARI))
                    PrepareStringBattle(STRINGID_INTROSENDOUT, GetBattlerAtPosition(B_POSITION_PLAYER_LEFT));
                
                gBattleCommunication[MULTIUSE_STATE]++;
            }
            break;
        case BATTLE_INTRO_STATE_PLAYER_SEND_OUT_ANIM:
            if (!gBattleControllerExecFlags)
            {
                for (battlerId = 0; battlerId < gBattlersCount; battlerId++)
                {
                    switch (GetBattlerPosition(battlerId))
                    {
                        case B_POSITION_PLAYER_LEFT:
                            BtlController_EmitIntroTrainerBallThrow(battlerId, BUFFER_A);
                            MarkBattlerForControllerExec(battlerId);
                            break;
                        case B_POSITION_PLAYER_RIGHT:
                            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                            {
                                BtlController_EmitIntroTrainerBallThrow(battlerId, BUFFER_A);
                                MarkBattlerForControllerExec(battlerId);
                            }
                            break;
                    }
                }
                gBattleCommunication[MULTIUSE_STATE]++;
            }
            break;
        case BATTLE_INTRO_STATE_END:
            gBattleStruct->firstTurnEventsState = 0;
            gBattleStruct->switchInByTurnOrderCounter = 0;
            gBattleMainFunc = TryDoEventsBeforeFirstTurn;
            break;
    }
}

static void UpdateQuickClawRandomNumber(void)
{
    u32 i;
    
    for (i = 0; i < gBattlersCount; i++)
    {
        gQuickClawTurnRandom[i].quickClawActivates = RandomPercentage(RNG_QUICK_CLAW, ItemId_GetHoldEffectParam(gBattleMons[i].item));
        gQuickClawTurnRandom[i].quickDrawActivates = RandomPercentage(RNG_QUICK_DRAW, 30);
        
        if (IsRaidBoss(i))
            gQuickClawTurnRandom[i].raidBossUsingRegularMove = RandomPercentage(RNG_RAID_REGULAR_MOVE, gNumRaidBattleStars < FOUR_STAR_RAID ? 25 : 10);
    }
}

static bool32 TryStartOverworldWeather(void)
{
    bool32 effect = FALSE;
    
    switch (GetCurrentWeather())
    {
        case WEATHER_RAIN:
        case WEATHER_RAIN_THUNDERSTORM:
        case WEATHER_DOWNPOUR:
            if (!(gBattleWeather & B_WEATHER_RAIN_ANY))
            {
                gBattleWeather = B_WEATHER_RAIN_NORMAL;
                gBattleStruct->weatherDuration = 0;
                gBattleScripting.animArg1 = B_ANIM_RAIN_CONTINUES;
                gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_STARTED_RAIN;
                effect = TRUE;
            }
            break;
        case WEATHER_SANDSTORM:
            if (!(gBattleWeather & B_WEATHER_SANDSTORM))
            {
                gBattleWeather = B_WEATHER_SANDSTORM;
                gBattleStruct->weatherDuration = 0;
                gBattleScripting.animArg1 = B_ANIM_SANDSTORM_CONTINUES;
                gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SANDSTORM_UP;
                effect = TRUE;
            }
            break;
        case WEATHER_SNOW:
        case WEATHER_SNOWSTORM:
            if (!(gBattleWeather & B_WEATHER_HAIL))
            {
                gBattleWeather = B_WEATHER_HAIL;
                gBattleStruct->weatherDuration = 0;
                gBattleScripting.animArg1 = B_ANIM_HAIL_CONTINUES;
                gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_STARTED_HAIL;
                effect = TRUE;
            }
            break;
        case WEATHER_DROUGHT:
            if (!(gBattleWeather & B_WEATHER_SUN_ANY))
            {
                gBattleWeather = B_WEATHER_SUN_NORMAL;
                gBattleStruct->weatherDuration = 0;
                gBattleScripting.animArg1 = B_ANIM_SUN_CONTINUES;
                gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SUN_TURN_HARSH;
                effect = TRUE;
            }
            break;
#if FOG_IN_BATTLE
        case WEATHER_FOG_HORIZONTAL:
        case WEATHER_FOG_DIAGONAL:
            if (!(gBattleWeather & B_WEATHER_FOG))
            {
                gBattleWeather = B_WEATHER_FOG;
                gBattleStruct->weatherDuration = 0;
                gBattleScripting.animArg1 = B_ANIM_FOG_CONTINUES;
                gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_DEEP_FOG;
                effect = TRUE;
            }
            break;
#endif
    }
    return effect;
}

enum
{
    FIRST_TURN_EVENT_ORDER,
    FIRST_TURN_EVENT_OVERWORLD_WEATHER,
    FIRST_TURN_EVENT_OVERWORLD_TERRAIN,
    FIRST_TURN_EVENT_BATTLE_CHALLENGE,
    FIRST_TURN_EVENT_RAID_BATTLE_REVEAL,
    FIRST_TURN_EVENT_DYNAMAX_SWIRL,
    FIRST_TURN_EVENT_TOTEM_BOOST,
    FIRST_TURN_EVENT_UNNERVE, // Also Neutralizing Gas
    FIRST_TURN_EVENT_SWITCHIN_ABILITIES,
    FIRST_TURN_EVENT_OPPORTUNIST_1,
    FIRST_TURN_EVENT_SWITCHIN_ITEMS,
    FIRST_TURN_EVENT_OPPORTUNIST_2,
    FIRST_TURN_EVENT_AIR_BALLOON,
    FIRST_TURN_EVENT_TRAINER_SLIDE,
    FIRST_TURN_EVENT_END,
};

static void TryDoEventsBeforeFirstTurn(void)
{
    u32 i, j;
    
    if (!gBattleControllerExecFlags)
    {
        switch (gBattleStruct->firstTurnEventsState)
        {
            case FIRST_TURN_EVENT_ORDER:
                for (i = 0; i < gBattlersCount; ++i)
                    gBattlerByTurnOrder[i] = i;
                
                for (i = 0; i < gBattlersCount - 1; ++i)
                {
                    for (j = i + 1; j < gBattlersCount; ++j)
                    {
                        if (GetWhoStrikesFirst(gBattlerByTurnOrder[i], gBattlerByTurnOrder[j], TRUE) != BATTLER1_STRIKES_FIRST)
                            SwapTurnOrder(i, j);
                    }
                }
                ++gBattleStruct->firstTurnEventsState;
                break;
            case FIRST_TURN_EVENT_OVERWORLD_WEATHER:
                if (!(gBattleTypeFlags & BATTLE_TYPE_RECORDED) && TryStartOverworldWeather())
                    BattleScriptPushCursorAndCallback(BattleScript_OverworldWeatherStarts);

                ++gBattleStruct->firstTurnEventsState;
                break;
            case FIRST_TURN_EVENT_OVERWORLD_TERRAIN:
                ++gBattleStruct->firstTurnEventsState;
                break;
            case FIRST_TURN_EVENT_BATTLE_CHALLENGE:
                if (TryBattleChallengeStartingStatus())
                    BattleScriptPushCursorAndCallback(BattleScript_BattleChallengeStartingStatus);
                    
                ++gBattleStruct->firstTurnEventsState;
                break;
            case FIRST_TURN_EVENT_RAID_BATTLE_REVEAL:
                if (gBattleTypeFlags & BATTLE_TYPE_RAID)
                {
                    StartRaidDynamaxEnergyPosition();
                    gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_RAID_STORM_STARTED;
                    
                    if (ShouldRaidStartWithShields(gBattleStruct->sos.totemBattlerId))
                    {
                        gBattleScripting.animArg1 = GetNumRaidShieldsToCreate(gBattleStruct->sos.totemBattlerId);
                        gBattleCommunication[MULTIUSE_STATE] = TRUE;
                    }
                    else
                        gBattleCommunication[MULTIUSE_STATE] = FALSE;
                    
                    gBattleScripting.battler = gBattleStruct->sos.totemBattlerId;
                    BattleScriptPushCursorAndCallback(BattleScript_RaidIntro);
                }
                ++gBattleStruct->firstTurnEventsState;
                break;
            case FIRST_TURN_EVENT_DYNAMAX_SWIRL:
                if (FlagGet(FLAG_DYNAMAX_ENABLED))
                    BattleScriptPushCursorAndCallback(BattleScript_DynamaxEnergySwirl);

                ++gBattleStruct->firstTurnEventsState;
                break;
            case FIRST_TURN_EVENT_TOTEM_BOOST:
                if ((gBattleTypeFlags & BATTLE_TYPE_TOTEM) && gQueuedStatBoosts[gBattleStruct->sos.totemBattlerId].stats)
                {
                    SaveAttackerToStack(gBattleStruct->sos.totemBattlerId);
                    BattleScriptPushCursorAndCallback(BattleScript_TotemBoost);
                    return;
                }
                memset(gQueuedStatBoosts, 0, sizeof(gQueuedStatBoosts));
                ++gBattleStruct->firstTurnEventsState;
                break;
            case FIRST_TURN_EVENT_UNNERVE:
                if (!AbilityBattleEffects(ABILITYEFFECT_NEUTRALIZING_GAS, 0) && !AbilityBattleEffects(ABILITYEFFECT_UNNERVE, 0))
                    ++gBattleStruct->firstTurnEventsState; // Incremment when all battler have been checked
                break;
            case FIRST_TURN_EVENT_SWITCHIN_ABILITIES: // From the fastest mon to slowest
                while (gBattleStruct->switchInByTurnOrderCounter < gBattlersCount)
                {
                    u32 battler = gBattlerByTurnOrder[gBattleStruct->switchInByTurnOrderCounter];
                    
                    if (IsBattlerAlive(battler))
                    {
                        if (TryPrimalReversion(battler) || AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, battler))
                            return;
                    }
                    gBattleStruct->switchInByTurnOrderCounter++;
                }
                gBattleStruct->switchInByTurnOrderCounter = 0;
                ++gBattleStruct->firstTurnEventsState;
                break;
            case FIRST_TURN_EVENT_OPPORTUNIST_1:
            case FIRST_TURN_EVENT_OPPORTUNIST_2:
                if (!AbilityBattleEffects(ABILITYEFFECT_OPPORTUNIST, 0))
                    ++gBattleStruct->firstTurnEventsState; // Incremment when all battler have been checked
                break;
            case FIRST_TURN_EVENT_SWITCHIN_ITEMS: // From the fastest mon to slowest
                while (gBattleStruct->switchInByTurnOrderCounter < gBattlersCount)
                {
                    u32 battler = gBattlerByTurnOrder[gBattleStruct->switchInByTurnOrderCounter];
                    
                    if (IsBattlerAlive(battler))
                    {
                        if (ItemBattleEffects(ITEMEFFECT_ON_SWITCH_IN, battler, FALSE))
                            return;
                    }
                    gBattleStruct->switchInByTurnOrderCounter++;
                }
                gBattleStruct->switchInByTurnOrderCounter = 0;
                ++gBattleStruct->firstTurnEventsState;
                break;
            case FIRST_TURN_EVENT_AIR_BALLOON:
                ++gBattleStruct->firstTurnEventsState;
                break;
            case FIRST_TURN_EVENT_TRAINER_SLIDE:
                if (ShouldDoTrainerSlide(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), TRAINER_SLIDE_FIRST_MON_SEND_OUT))
                    BattleScriptPushCursorAndCallback(BattleScript_TrainerSlideMsgEnd3);
                    
                ++gBattleStruct->firstTurnEventsState;
                break;
            case FIRST_TURN_EVENT_END:
                for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
                {
                    gBattleStruct->battlers[i].monToSwitchIntoId = PARTY_SIZE;
                    gBattleStruct->battlers[i].chosenMove = MOVE_NONE;
                    gBattleStruct->battlers[i].chosenAction = B_ACTION_NONE;
                    
                    if (IsBattlerAlive(i))
                        gBattleStruct->sides[GetBattlerSide(i)].party[gBattlerPartyIndexes[i]].appearedInBattle = TRUE;

                    gBattleMons[i].status2 &= ~(STATUS2_FLINCHED);
                }
                TurnValuesCleanUp(FALSE);
                
                memset(&gSpecialStatuses, 0, sizeof(gSpecialStatuses));
                memset(&gQueuedStatBoosts, 0, sizeof(gQueuedStatBoosts));
                
                gBattleStruct->absentBattlerFlags = gAbsentBattlerFlags;
                
                ResetSentPokesToOpponentValue();
                
                for (i = 0; i < BATTLE_COMMUNICATION_ENTRIES_COUNT; ++i)
                    gBattleCommunication[i] = 0;
                
                gBattleScripting.atk48_state = 0;
                gBattleScripting.atk49_state = 0;
                
                gMoveResultFlags = 0;
                UpdateQuickClawRandomNumber();
                
                gBattleStruct->turnEffectsTracker = 0;
                gBattleStruct->turnEffectsBattlerId = 0;
                gBattleStruct->turnSideTracker = 0;
                gBattleStruct->faintedActionsState = 0;
                
                gBattleMainFunc = HandleAssignUsableGimmicks;
                break;
        }
    }
}

enum
{
    GIMMICK_STATE_CHECK_USABLE,
    GIMMICK_STATE_CHECK_KEY_ITEM,
    GIMMICK_STATE_LOOP_KEY_ITEM_CHECK,
    GIMMICK_STATE_SAVE_KEY_ITEMS,
    GIMMICK_STATE_ASSIGN_GIMMICKS,
    GIMMICK_STATE_LOOP_USABLE_GIMMICK,
    GIMMICK_STATE_WAIT_AND_END,
};

static void HandleAssignUsableGimmicks(void)
{
    u32 battlerId;
    
    switch (gBattleCommunication[MULTIUSE_STATE])
    {
        case GIMMICK_STATE_CHECK_USABLE:
            if (gBattleTypeFlags & BATTLE_TYPE_SAFARI) // Not usable in safari zone
                gBattleCommunication[MULTIUSE_STATE] = GIMMICK_STATE_WAIT_AND_END;
            else
                gBattleCommunication[MULTIUSE_STATE]++;
            break;
        case GIMMICK_STATE_CHECK_KEY_ITEM:
            if (!gBattleControllerExecFlags)
            {
                battlerId = gBattleCommunication[SPRITES_INIT_STATE1];
                BtlController_EmitGimmickState(battlerId, BUFFER_A, STATE_CHECK_GIMMICK_KEY_ITEMS, 0);
                MarkBattlerForControllerExec(battlerId);
                gBattleCommunication[MULTIUSE_STATE]++;
            }
            break;
        case GIMMICK_STATE_LOOP_KEY_ITEM_CHECK:
        case GIMMICK_STATE_LOOP_USABLE_GIMMICK:
            if (++gBattleCommunication[SPRITES_INIT_STATE1] == gBattlersCount)
            {
                gBattleCommunication[SPRITES_INIT_STATE1] = 0;
                gBattleCommunication[MULTIUSE_STATE]++;
            }
            else
                gBattleCommunication[MULTIUSE_STATE]--;
            break;
        case GIMMICK_STATE_SAVE_KEY_ITEMS:
            if (!gBattleControllerExecFlags)
            {
                for (battlerId = 0; battlerId < gBattlersCount; battlerId++)
                    memcpy(gBattleStruct->battlers[battlerId].hasGimmickKeyItem, &gBattleBufferB[battlerId][4], sizeof(gBattleStruct->battlers[battlerId].hasGimmickKeyItem));

                gBattleCommunication[MULTIUSE_STATE]++;
            }
            break;
        case GIMMICK_STATE_ASSIGN_GIMMICKS:
            if (!gBattleControllerExecFlags)
            {
                AssignUsableGimmick(gBattleCommunication[SPRITES_INIT_STATE1]);
                gBattleCommunication[MULTIUSE_STATE]++;
            }
            break;
        case GIMMICK_STATE_WAIT_AND_END:
            gBattleCommunication[MULTIUSE_STATE] = 0;
            gBattleMainFunc = HandleTurnActionSelectionState;
            break;
    }
}

static void HandleEndTurn_ContinueBattle(void)
{
    u32 i;

    if (!gBattleControllerExecFlags)
    {
        gBattleMainFunc = BattleTurnPassed;
        
        for (i = 0; i < BATTLE_COMMUNICATION_ENTRIES_COUNT; ++i)
            gBattleCommunication[i] = 0;
        
        for (i = 0; i < gBattlersCount; ++i)
        {
            gBattleMons[i].status2 &= ~(STATUS2_FLINCHED);
            
            if (gBattleMons[i].status1.id == STATUS1_SLEEP && (gBattleMons[i].status2 & STATUS2_MULTIPLETURNS))
                CancelMultiTurnMoves(i);
        }
        
        gBattleStruct->turnEffectsTracker = 0;
        gBattleStruct->turnEffectsBattlerId = 0;
        gBattleStruct->turnSideTracker = 0;
        
        gMoveResultFlags = 0;
        SaveBattlersHps(); // For Emergency Exit
    }
}

void BattleTurnPassed(void)
{
    u32 i;

    TurnValuesCleanUp(TRUE);
    
    if (gBattleOutcome == 0 && DoEndTurnEffects())
        return;
    
    if (HandleFaintedMonActions())
        return;
    
    gBattleStruct->faintedActionsState = 0;
    
    TurnValuesCleanUp(FALSE);
    gHitMarker &= ~(HITMARKER_NO_ATTACKSTRING | HITMARKER_UNABLE_TO_USE_MOVE | HITMARKER_PLAYER_FAINTED | HITMARKER_PASSIVE_DAMAGE);
    gBattleScripting.animTurn = 0;
    gBattleScripting.animTargetsHit = 0;
    gBattleScripting.atk48_state = 0;
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

    if (gBattleTypeFlags & BATTLE_TYPE_SOS)
    {
        gBattleMainFunc = TryCallSosAlly;
        return;
    }
    ClearActionsAndMovesForNextTurn();
    
    if (gBattleTypeFlags & BATTLE_TYPE_RAID)
    {
        ChangeRaidDynamaxEnergyPosition();
        
        if (ShouldRaidKickPlayer())
            gBattleMainFunc = HandleEndTurn_FinishBattle;
    }
    BattleAI_SetAILogicDataForTurn();
    
    if (ShouldDoTrainerSlide(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), TRAINER_SLIDE_LAST_MON_LOW_HP))
        BattleScriptExecute(BattleScript_TrainerSlideMsgEnd2);
    else if (ShouldDoTrainerSlide(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), TRAINER_SLIDE_FIRST_SUPER_EFFECTIVE_HIT_TAKEN))
        BattleScriptExecute(BattleScript_TrainerSlideMsgEnd2);
    else if (ShouldDoTrainerSlide(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), TRAINER_SLIDE_FIRST_CRITICAL_TAKEN))
        BattleScriptExecute(BattleScript_TrainerSlideMsgEnd2);
}

static void TryCallSosAlly(void)
{
    if (!gBattleStruct->sos.triedToCallAlly)
    {
        gBattleStruct->sos.triedToCallAlly = TRUE;
        
        if (gBattleOutcome == 0 && TryInitSosCall())
            return;
    }
    gBattleStruct->sos.triedToCallAlly = FALSE;
    ClearActionsAndMovesForNextTurn();
    BattleAI_SetupAILogicData();
}

static void ClearActionsAndMovesForNextTurn(void)
{
    u32 i;
    
    if (gBattleStruct->battleTurnCounter < 0xFF)
        ++gBattleStruct->battleTurnCounter;
    
    for (i = 0; i < 5; ++i)
        gBattleCommunication[i] = 0;
    
    for (i = 0; i < gBattlersCount; ++i)
    {
        gBattleStruct->battlers[i].chosenAction = B_ACTION_NONE;
        gBattleStruct->battlers[i].chosenMove = MOVE_NONE;
        gBattleStruct->pickupStack[i] = 0xFF;
    }
    
    for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
    {
        gBattleStruct->battlers[i].monToSwitchIntoId = PARTY_SIZE;
        gBattleStruct->battlers[i].targetsDone = 0;
    }
    gBattleStruct->absentBattlerFlags = gAbsentBattlerFlags;
    gBattleStruct->throwingPokeBall = FALSE;
    
    UpdateQuickClawRandomNumber();

    gBattleCommunication[MULTIUSE_STATE] = GIMMICK_STATE_ASSIGN_GIMMICKS;
    gBattleMainFunc = HandleAssignUsableGimmicks;
}

u32 IsRunningFromBattleImpossible(u32 battlerId, bool32 checkIngrain)
{
    u32 ret, holdEffect = GetBattlerItemHoldEffect(battlerId, TRUE);
    
    if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && GetBattlerPosition(battlerId) == B_POSITION_PLAYER_RIGHT && IsBattlerAlive(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT))
    && IsDoubleBattleForBattler(battlerId))
    {
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CANT_ESCAPE;
        return BATTLE_RUN_FAILURE;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_RAID)
    {
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CANT_ESCAPE;
        return BATTLE_RUN_FAILURE;
    }
    else if (holdEffect == HOLD_EFFECT_CAN_ALWAYS_RUN || (gBattleTypeFlags & BATTLE_TYPE_LINK) || GetBattlerAbility(battlerId) == ABILITY_RUN_AWAY)
        return BATTLE_RUN_SUCCESS;
    else if ((ret = IsAbilityPreventingSwitchOut(battlerId)))
    {
        gBattleScripting.battler = ret - 1;
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_ABILITY_PREVENT_ESCAPE;
        return BATTLE_RUN_FAILURE;
    }
    else if (!CanBattlerEscape(battlerId, checkIngrain))
    {
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CANT_ESCAPE;
        return BATTLE_RUN_FORBIDDEN;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE)
    {
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_DONT_LEAVE_BIRTH;
        return BATTLE_RUN_FORBIDDEN;
    }
    return BATTLE_RUN_SUCCESS;
}

void UpdatePartyOwnerOnSwitch_NonMulti(u32 battler)
{
    u32 i;

    for (i = 0; i < 3; ++i)
        gBattlePartyCurrentOrder[i] = *(battler * 3 + i + (u8 *)(gBattleStruct->battlerPartyOrders));
    
    SwitchPartyMonSlots(GetPartyIdFromBattlePartyId(gBattlerPartyIndexes[battler]), GetPartyIdFromBattlePartyId(gBattleStruct->battlers[battler].monToSwitchIntoId));
    
    if (IsDoubleBattleForBattler(battler))
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

void SwitchPartyOrderInGameMulti(u32 battler, u32 monToSwitchIntoId)
{
    if (GetBattlerSide(battler) == B_SIDE_PLAYER)
    {
        u32 i;
        
        for (i = 0; i < ARRAY_COUNT(gBattlePartyCurrentOrder); i++)
            gBattlePartyCurrentOrder[i] = *(i + (u8 *)(gBattleStruct->battlerPartyOrders));

        SwitchPartyMonSlots(GetPartyIdFromBattlePartyId(gBattlerPartyIndexes[battler]), GetPartyIdFromBattlePartyId(monToSwitchIntoId));

        for (i = 0; i < ARRAY_COUNT(gBattlePartyCurrentOrder); i++)
            *(i + (u8 *)(gBattleStruct->battlerPartyOrders)) = gBattlePartyCurrentOrder[i];
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

#define CONTROLLER_ALL_BATTLERS_FLAGS(battlerId) ((Bit(battlerId)) | (0xF0000000) | (Bit(battlerId) << 4) | (Bit(battlerId) << 8) | (Bit(battlerId) << 0xC))

static void HandleTurnActionSelectionState(void)
{
    u32 i, battlerId;

    gBattleCommunication[ACTIONS_CONFIRMED_COUNT] = 0;
    
    for (battlerId = 0; battlerId < gBattlersCount; ++battlerId)
    {
        u32 position = GetBattlerPosition(battlerId);

        switch (gBattleCommunication[battlerId])
        {
        case STATE_BEFORE_ACTION_CHOSEN: // Choose an action.
            gBattleStruct->battlers[battlerId].monToSwitchIntoId = PARTY_SIZE;
            
            if ((gBattleTypeFlags & BATTLE_TYPE_MULTI) || (position & BIT_FLANK) == B_FLANK_LEFT
             || gBattleStruct->absentBattlerFlags & Bit(GetBattlerAtPosition(BATTLE_PARTNER(position)))
             || gBattleCommunication[GetBattlerAtPosition(BATTLE_PARTNER(position))] == STATE_WAIT_ACTION_CONFIRMED)
            {
                if ((gBattleStruct->absentBattlerFlags & Bit(battlerId)) || (gStatuses3[battlerId] & STATUS3_COMMANDING))
                {
                    gBattleStruct->battlers[battlerId].chosenAction = B_ACTION_NOTHING_FAINTED;
                    gBattleCommunication[battlerId] = (gBattleTypeFlags & BATTLE_TYPE_MULTI) ? STATE_WAIT_ACTION_CONFIRMED_STANDBY : STATE_WAIT_ACTION_CONFIRMED;
                }
                else
                {
                    if ((gBattleMons[battlerId].status2 & (STATUS2_MULTIPLETURNS | STATUS2_RECHARGE)))
                    {
                        gBattleStruct->battlers[battlerId].chosenAction = B_ACTION_USE_MOVE;
                        gBattleCommunication[battlerId] = STATE_WAIT_ACTION_CONFIRMED_STANDBY;
                    }
                    else if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && position == B_POSITION_PLAYER_RIGHT && IsDoubleBattleForBattler(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT))
                    && (gBattleStruct->throwingPokeBall || gBattleStruct->battlers[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)].chosenAction == B_ACTION_RUN)
                    && gBattleStruct->battlers[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)].chosenAction != B_ACTION_NOTHING_FAINTED)
                    {
                        gBattleStruct->throwingPokeBall = FALSE;
                        gBattleStruct->battlers[battlerId].chosenAction = B_ACTION_NOTHING_FAINTED;
                        gBattleCommunication[battlerId] = STATE_WAIT_ACTION_CONFIRMED_STANDBY;
                    }
                    else
                    {
                        gBattleStruct->battlers[battlerId].itemPartyIndex = PARTY_SIZE;
                        BtlController_EmitChooseAction(battlerId, BUFFER_A, gBattleStruct->battlers[0].chosenAction, gBattleBufferB[0][1] | (gBattleBufferB[0][2] << 8));
                        MarkBattlerForControllerExec(battlerId);
                        ++gBattleCommunication[battlerId];
                    }
                }
            }
            break;
        case STATE_WAIT_ACTION_CHOSEN: // Try to perform an action.
            if (!(gBattleControllerExecFlags & CONTROLLER_ALL_BATTLERS_FLAGS(battlerId)))
            {
                gBattleStruct->battlers[battlerId].chosenAction = gBattleBufferB[battlerId][1];
                
                switch (gBattleBufferB[battlerId][1])
                {
                case B_ACTION_USE_MOVE:
                    if (AreAllMovesUnusable(battlerId))
                    {
                        gBattleCommunication[battlerId] = STATE_SELECTION_SCRIPT;
                        gBattleStruct->battlers[battlerId].selectionScriptFinished = FALSE;
                        gBattleStruct->battlers[battlerId].stateIdAfterSelScript = STATE_WAIT_ACTION_CONFIRMED_STANDBY;
                        gBattleStruct->battlers[battlerId].moveTarget = gBattleBufferB[battlerId][3];
                        return;
                    }
                    else
                    {
                        u32 j, move, types[3];
                        struct ChooseMoveStruct moveInfo = {0};
                        
                        moveInfo.species = gBattleMons[battlerId].species;
                        
                        GetBattlerTypes(battlerId, FALSE, types);
                        
                        for (i = 0; i < 3; i++)
                            moveInfo.monTypes[i] = types[i];
                        
                        for (i = 0; i < MAX_BATTLERS_COUNT; i++)
                            moveInfo.isHealBlocked[i] = (gStatuses3[i] & STATUS3_HEAL_BLOCK);
                        
                        for (i = 0; i < MAX_MON_MOVES; ++i)
                        {
                            moveInfo.moves[i].move = move = gBattleMons[battlerId].moves[i];
                            moveInfo.moves[i].currentPp = gBattleMons[battlerId].pp[i];
                            moveInfo.moves[i].maxPp = CalculatePPWithBonus(move, gBattleMons[battlerId].ppBonuses, i);
                            moveInfo.moves[i].split = GetBattleMoveSplit(move);
                            moveInfo.moves[i].target = GetBattlerMoveTargetType(battlerId, move);
                            moveInfo.moves[i].type = GetBattlerMoveType(battlerId, move);
                            moveInfo.moves[i].power = gBattleMoves[move].power;
                            
                            for (j = 0; j < MAX_BATTLERS_COUNT; j++)
                                TypeCalc(move, moveInfo.moves[i].type, battlerId, j, FALSE, FALSE, &moveInfo.moves[i].effectivenessFlags[j]);
                            
                            if (IsTypeStellarBoosted(battlerId, moveInfo.moves[i].type))
                                moveInfo.moves[i].isStellarBoosted = TRUE;
                        }
                        BtlController_EmitChooseMove(battlerId, BUFFER_A, (gBattleTypeFlags & BATTLE_TYPE_DOUBLE), FALSE, &moveInfo);
                        MarkBattlerForControllerExec(battlerId);
                    }
                    break;
                case B_ACTION_USE_ITEM:
                    if (IsPlayerBagDisabled())
                    {
                        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CANT_USE_ITEM;
                        gBattlerControllersData[battlerId].selectionScript = BattleScript_ActionSelectionItemsCantBeUsed;
                        gBattleCommunication[battlerId] = STATE_SELECTION_SCRIPT;
                        gBattleStruct->battlers[battlerId].selectionScriptFinished = FALSE;
                        gBattleStruct->battlers[battlerId].stateIdAfterSelScript = STATE_BEFORE_ACTION_CHOSEN;
                        RecordedBattle_ClearBattlerAction(battlerId, 1);
                        return;
                    }
                    else
                    {
                        BtlController_EmitChooseItem(battlerId, BUFFER_A, gBattleStruct->battlerPartyOrders[battlerId]);
                        MarkBattlerForControllerExec(battlerId);
                    }
                    break;
                case B_ACTION_SWITCH:
                    gBattleStruct->battlers[battlerId].partyIndex = gBattlerPartyIndexes[battlerId];
                    
                    if (IsBattlerBeingCommanded(battlerId) || !CanBattlerEscape(battlerId, TRUE))
                        BtlController_EmitChoosePokemon(battlerId, BUFFER_A, PARTY_ACTION_CANT_SWITCH, PARTY_SIZE, gBattleStruct->battlerPartyOrders[battlerId]);
                    else
                    {
                        i = IsAbilityPreventingSwitchOut(battlerId);
                        
                        if (i)
                            BtlController_EmitChoosePokemon(battlerId, BUFFER_A, ((i - 1) << 4) | PARTY_ACTION_ABILITY_PREVENTS, PARTY_SIZE, gBattleStruct->battlerPartyOrders[battlerId]);
                        else if ((position & BIT_FLANK) != B_FLANK_LEFT && gBattleStruct->battlers[BATTLE_PARTNER(battlerId)].chosenAction == B_ACTION_SWITCH)
                            BtlController_EmitChoosePokemon(battlerId, BUFFER_A, PARTY_ACTION_CHOOSE_MON, gBattleStruct->battlers[BATTLE_PARTNER(battlerId)].monToSwitchIntoId, gBattleStruct->battlerPartyOrders[battlerId]);
                        else
                            BtlController_EmitChoosePokemon(battlerId, BUFFER_A, PARTY_ACTION_CHOOSE_MON, PARTY_SIZE, gBattleStruct->battlerPartyOrders[battlerId]);
                    }
                    MarkBattlerForControllerExec(battlerId);
                    break;
                case B_ACTION_SAFARI_BALL:
                    if (IsPlayerPartyAndPokemonStorageFull())
                    {
                        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_BOX_FULL;
                        gBattlerControllersData[battlerId].selectionScript = BattleScript_ActionSelectionItemsCantBeUsed;
                        gBattleCommunication[battlerId] = STATE_SELECTION_SCRIPT;
                        gBattleStruct->battlers[battlerId].selectionScriptFinished = FALSE;
                        gBattleStruct->battlers[battlerId].stateIdAfterSelScript = STATE_BEFORE_ACTION_CHOSEN;
                        return;
                    }
                    break;
                case B_ACTION_CANCEL_PARTNER:
                    gBattleCommunication[battlerId] = STATE_WAIT_SET_BEFORE_ACTION;
                    gBattleCommunication[GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battlerId)))] = STATE_BEFORE_ACTION_CHOSEN;
                    gBattleStruct->battlers[BATTLE_PARTNER(GetBattlerPosition(battlerId))].toActivateGimmick = FALSE;
                    BtlController_EmitEndBounceEffect(battlerId, BUFFER_A);
                    MarkBattlerForControllerExec(battlerId);
                    return;
                case B_ACTION_CHEER:
                    break;
                }
                
                if (gBattleBufferB[battlerId][1] == B_ACTION_RUN && (gBattleTypeFlags & BATTLE_TYPE_TRAINER) && !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED)))
                {
                    BattleScriptExecute(BattleScript_PrintCantRunFromTrainer);
                    gBattleCommunication[battlerId] = STATE_BEFORE_ACTION_CHOSEN;
                }
                else if (gBattleBufferB[battlerId][1] == B_ACTION_RUN && IsRunningFromBattleImpossible(battlerId, TRUE) != BATTLE_RUN_SUCCESS)
                {
                    gBattlerControllersData[battlerId].selectionScript = BattleScript_PrintCantEscapeFromBattle;
                    gBattleCommunication[battlerId] = STATE_SELECTION_SCRIPT;
                    gBattleStruct->battlers[battlerId].selectionScriptFinished = FALSE;
                    gBattleStruct->battlers[battlerId].stateIdAfterSelScript = STATE_BEFORE_ACTION_CHOSEN;
                    return;
                }
                else
                    ++gBattleCommunication[battlerId];
            }
            break;
        case STATE_WAIT_ACTION_CASE_CHOSEN:
            if (!(gBattleControllerExecFlags & CONTROLLER_ALL_BATTLERS_FLAGS(battlerId)))
            {
                switch (gBattleStruct->battlers[battlerId].chosenAction)
                {
                case B_ACTION_USE_MOVE:
                    switch (gBattleBufferB[battlerId][1])
                    {
                    case 3 ... 9:
                        gBattleStruct->battlers[battlerId].chosenAction = gBattleBufferB[battlerId][1];
                        return;
                    default:
                        if ((gBattleBufferB[battlerId][2] | (gBattleBufferB[battlerId][3] << 8)) == 0xFFFF)
                        {
                            gBattleCommunication[battlerId] = STATE_BEFORE_ACTION_CHOSEN;
                        }
                        else if (TrySetCantSelectMoveBattleScript(battlerId, gBattleBufferB[battlerId][2] & ~(RET_GIMMICK)))
                        {
                            gBattleCommunication[battlerId] = STATE_SELECTION_SCRIPT;
                            gBattleBufferB[battlerId][1] = 0;
                            gBattleStruct->battlers[battlerId].selectionScriptFinished = FALSE;
                            gBattleStruct->battlers[battlerId].stateIdAfterSelScript = STATE_WAIT_ACTION_CHOSEN;
                            return;
                        }
                        else
                        {
                            gBattleStruct->battlers[battlerId].chosenMovePosition = gBattleBufferB[battlerId][2] & ~(RET_GIMMICK);
                            gBattleStruct->battlers[battlerId].chosenMove = gBattleMons[battlerId].moves[gBattleStruct->battlers[battlerId].chosenMovePosition];
                            gBattleStruct->battlers[battlerId].moveTarget = gBattleBufferB[battlerId][3];
                            
                            // Check usable gimmick so tests don't stop running when unable to activate it
                            if ((gBattleBufferB[battlerId][2] & RET_GIMMICK) && gBattleStruct->battlers[battlerId].usableGimmick != GIMMICK_NONE)
                                gBattleStruct->battlers[battlerId].toActivateGimmick = TRUE;
                            
                            ++gBattleCommunication[battlerId];
                        }
                        break;
                    }
                    break;
                case B_ACTION_USE_ITEM:
                    if ((gBattleBufferB[battlerId][1] | (gBattleBufferB[battlerId][2] << 8)) == 0)
                        gBattleCommunication[battlerId] = STATE_BEFORE_ACTION_CHOSEN;
                    else
                    {
                        gLastUsedItem = (gBattleBufferB[battlerId][1] | (gBattleBufferB[battlerId][2] << 8));
                        if (ItemId_GetPocket(gLastUsedItem) == POCKET_POKE_BALLS)
                            gBattleStruct->throwingPokeBall = TRUE;
                        
                        ++gBattleCommunication[battlerId];
                    }
                    break;
                case B_ACTION_SWITCH:
                    if (gBattleBufferB[battlerId][1] == PARTY_SIZE)
                        gBattleCommunication[battlerId] = STATE_BEFORE_ACTION_CHOSEN;
                    else
                    {
                        gBattleStruct->battlers[battlerId].monToSwitchIntoId = gBattleBufferB[battlerId][1];
                        
                        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                        {
                            *(battlerId * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) &= 0xF;
                            *(battlerId * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) |= (gBattleBufferB[battlerId][2] & 0xF0);
                            *(battlerId * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 1) = gBattleBufferB[battlerId][3];
                            *((BATTLE_PARTNER(battlerId)) * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) &= (0xF0);
                            *((BATTLE_PARTNER(battlerId)) * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) |= (gBattleBufferB[battlerId][2] & 0xF0) >> 4;
                            *((BATTLE_PARTNER(battlerId)) * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 2) = gBattleBufferB[battlerId][3];
                        }
                        ++gBattleCommunication[battlerId];
                    }
                    break;
                case B_ACTION_RUN:
                case B_ACTION_SAFARI_RUN:
                    gHitMarker |= HITMARKER_RUN;
                    ++gBattleCommunication[battlerId];
                    break;
                case B_ACTION_SAFARI_WATCH_CAREFULLY:
                case B_ACTION_SAFARI_BALL:
                case B_ACTION_SAFARI_BAIT:
                case B_ACTION_SAFARI_GO_NEAR:
                case B_ACTION_OLDMAN_THROW:
                case B_ACTION_CHEER:
                    ++gBattleCommunication[battlerId];
                    break;
                case B_ACTION_THROW_BALL:
                    gBattleStruct->throwingPokeBall = TRUE;
                    ++gBattleCommunication[battlerId];
                    break;
                }
            }
            break;
        case STATE_WAIT_ACTION_CONFIRMED_STANDBY:
            if (!(gBattleControllerExecFlags & CONTROLLER_ALL_BATTLERS_FLAGS(battlerId)))
            {
                if (((gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_DOUBLE)) != BATTLE_TYPE_DOUBLE) || (position & BIT_FLANK) != B_FLANK_LEFT
                 || (gBattleStruct->absentBattlerFlags & Bit(GetBattlerAtPosition(BATTLE_PARTNER(position)))))
                    BtlController_EmitLinkStandbyMsg(battlerId, BUFFER_A, 0);
                else
                    BtlController_EmitLinkStandbyMsg(battlerId, BUFFER_A, 1);
                MarkBattlerForControllerExec(battlerId);
                ++gBattleCommunication[battlerId];
            }
            break;
        case STATE_WAIT_ACTION_CONFIRMED:
            if (!(gBattleControllerExecFlags & CONTROLLER_ALL_BATTLERS_FLAGS(battlerId)))
                ++gBattleCommunication[ACTIONS_CONFIRMED_COUNT];
            break;
        case STATE_SELECTION_SCRIPT:
            if (gBattleStruct->battlers[battlerId].selectionScriptFinished)
                gBattleCommunication[battlerId] = gBattleStruct->battlers[battlerId].stateIdAfterSelScript;
            else
            {
                gBattlerAttacker = battlerId;

                gBattlescriptCurrInstr = gBattlerControllersData[battlerId].selectionScript;
                
                if (!(gBattleControllerExecFlags & CONTROLLER_ALL_BATTLERS_FLAGS(battlerId)))
                    gBattleScriptingCommandsTable[gBattlescriptCurrInstr[0]]();
                
                gBattlerControllersData[battlerId].selectionScript = gBattlescriptCurrInstr;
            }
            break;
        case STATE_WAIT_SET_BEFORE_ACTION:
            if (!(gBattleControllerExecFlags & CONTROLLER_ALL_BATTLERS_FLAGS(battlerId)))
                gBattleCommunication[battlerId] = STATE_BEFORE_ACTION_CHOSEN;
            break;
        }
    }
    // Check if everyone chose actions.
    if (gBattleCommunication[ACTIONS_CONFIRMED_COUNT] == gBattlersCount)
    {
        gBattleMainFunc = SetActionsAndBattlersTurnOrder;
        
        if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
        {
            for (i = 0; i < gBattlersCount; i++)
            {
                if (gBattleStruct->battlers[i].chosenAction == B_ACTION_SWITCH)
                    SwitchPartyOrderInGameMulti(i, gBattleStruct->battlers[i].monToSwitchIntoId);
            }
        }
        
        if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && gBattleStruct->battlers[GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)].chosenAction != B_ACTION_NOTHING_FAINTED
        && gBattleStruct->throwingPokeBall && IsDoubleBattleForBattler(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)))
            gBattleStruct->battlers[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)].chosenAction = B_ACTION_NOTHING_FAINTED;
    }
}

void SwapTurnOrder(u32 id1, u32 id2)
{
    u32 temp;

    SWAP(gActionsByTurnOrder[id1], gActionsByTurnOrder[id2], temp);
    SWAP(gBattlerByTurnOrder[id1], gBattlerByTurnOrder[id2], temp);
}

static s32 GetBattlerBracket(u32 battler, u32 action, u32 move)
{
    u32 holdEffect = GetBattlerItemHoldEffect(battler, TRUE);
    u32 holdEffectParam = ItemId_GetHoldEffectParam(gBattleMons[battler].item);
    u32 ability = GetBattlerAbility(battler);
    
    gSpecialStatuses[battler].quickClawActivated = FALSE;
    gSpecialStatuses[battler].quickDrawActivated = FALSE;
    
    if (ability == ABILITY_QUICK_DRAW && gQuickClawTurnRandom[battler].quickDrawActivates && action == B_ACTION_USE_MOVE && GetBattleMoveSplit(move) != SPLIT_STATUS)
    {
        gSpecialStatuses[battler].quickDrawActivated = TRUE;
        return 1;
    }
    else if (holdEffect == HOLD_EFFECT_QUICK_CLAW && gQuickClawTurnRandom[battler].quickClawActivates)
    {
        gSpecialStatuses[battler].quickClawActivated = TRUE;
        return 1;
    }
    else if (ability == ABILITY_STALL || (ability == ABILITY_MYCELIUM_MIGHT && action == B_ACTION_USE_MOVE && GetBattleMoveSplit(move) == SPLIT_STATUS))
        return -1;
    
    return 0;
}

u32 GetWhoStrikesFirst(u32 battler1, u32 battler2, bool32 ignoreChosenMoves)
{
    s8 battler1Priority, battler2Priority;
    s32 battler1Bracket, battler2Bracket;
    u32 battler1Speed, battler2Speed, temp;
  
    if (!ignoreChosenMoves) 
    {
        u32 battler1ChosenAction = gBattleStruct->battlers[battler1].chosenAction;
        u32 battler2ChosenAction = gBattleStruct->battlers[battler2].chosenAction;
        u32 battler1ChosenMove = gBattleStruct->battlers[battler1].chosenMove;
        u32 battler2ChosenMove = gBattleStruct->battlers[battler2].chosenMove;
        
        // priority check
        battler1Priority = battler2Priority = 0;
        
        if (battler1ChosenAction == B_ACTION_USE_MOVE)
            battler1Priority = GetMovePriority(battler1, battler1ChosenMove);
        
        if (battler2ChosenAction == B_ACTION_USE_MOVE)
            battler2Priority = GetMovePriority(battler2, battler2ChosenMove);
        
        if (battler1Priority > battler2Priority) 
            return BATTLER1_STRIKES_FIRST;
        else if (battler1Priority < battler2Priority)
            return BATTLER2_STRIKES_FIRST;
        
        battler1Bracket = GetBattlerBracket(battler1, battler1ChosenAction, battler1ChosenMove);
        battler2Bracket = GetBattlerBracket(battler2, battler2ChosenAction, battler2ChosenMove);
    }
    else
    {
        battler1Bracket = GetBattlerBracket(battler1, B_ACTION_NONE, MOVE_NONE);
        battler2Bracket = GetBattlerBracket(battler2, B_ACTION_NONE, MOVE_NONE);
    }
    
    // bracket check
    if (battler1Bracket > battler2Bracket) 
        return BATTLER1_STRIKES_FIRST;
    else if (battler1Bracket < battler2Bracket)
        return BATTLER2_STRIKES_FIRST;
    
    // speed check
    battler1Speed = GetBattlerTotalSpeed(battler1);
    battler2Speed = GetBattlerTotalSpeed(battler2);
    
    if (battler1Speed == battler2Speed) // Same speeds
        return RandomPercentage(RNG_SPEED_TIE, 50) ? BATTLER1_STRIKES_FIRST : SPEED_TIE;
    else
    {
        if (gFieldStatus & STATUS_FIELD_TRICK_ROOM)
            SWAP(battler1Speed, battler2Speed, temp);
        
        if (battler1Speed > battler2Speed)
            return BATTLER1_STRIKES_FIRST;
        else
            return BATTLER2_STRIKES_FIRST;
    }
}

s8 GetMovePriority(u32 battler, u32 move)
{
    s8 priority;
    
    if (gProtectStructs[battler].noValidMoves)
        move = MOVE_STRUGGLE;
    
    if (GetActiveGimmick(battler) == GIMMICK_DYNAMAX && GetBattleMoveSplit(move) == SPLIT_STATUS && !IsRaidBossUsingRegularMove(battler, move))
        move = MOVE_MAX_GUARD;

    priority = gBattleMoves[move].priority;
    
    switch (GetBattlerAbility(battler))
    {
        case ABILITY_PRANKSTER:
            if (GetBattleMoveSplit(move) == SPLIT_STATUS)
                ++priority;
            break;
        case ABILITY_GALE_WINGS:
            if (gBattleMoves[move].type == TYPE_FLYING && BATTLER_MAX_HP(battler))
                ++priority;
            break;
        case ABILITY_TRIAGE:
            if (GET_MOVE_MOVEEFFECT_TABLE(move).healingEffect)
                priority += 3;
            break;
    }
    return priority;
}

u32 GetBattlerTotalSpeed(u32 battler)
{
    u32 holdEffectParam, monSpeed;
    
    APPLY_MON_STAT_MOD(monSpeed, &gBattleMons[battler], gBattleMons[battler].speed, STAT_SPEED);

    switch (GetBattlerAbility(battler))
    {
        case ABILITY_SWIFT_SWIM:
            if (IsBattlerWeatherAffected(battler, B_WEATHER_RAIN_ANY))
                monSpeed *= 2;
            break;
        case ABILITY_CHLOROPHYLL:
            if (IsBattlerWeatherAffected(battler, B_WEATHER_SUN_ANY))
                monSpeed *= 2;
            break;
        case ABILITY_SAND_RUSH:
            if (IsBattlerWeatherAffected(battler, B_WEATHER_SANDSTORM))
                monSpeed *= 2;
            break;
        case ABILITY_SLUSH_RUSH:
            if (IsBattlerWeatherAffected(battler, B_WEATHER_HAIL))
                monSpeed *= 2;
            break;
        case ABILITY_QUICK_FEET:
            if (gBattleMons[battler].status1.id)
                monSpeed = (monSpeed * 15) / 10;
            break;
        case ABILITY_SLOW_START:
            if (gDisableStructs[battler].slowStartTimer)
                monSpeed /= 2;
            break;
        case ABILITY_UNBURDEN: // Don't gain boost if ability was lost
            if (gDisableStructs[battler].unburdenBoost)
                monSpeed *= 2;
            break;
    }
    
    if (gSideStatuses[GetBattlerSide(battler)] & SIDE_STATUS_TAILWIND)
        monSpeed *= 2;
    
    // badge stat boost
    if (CanReceiveBadgeBoost(battler, FLAG_BADGE03_GET))
        monSpeed = (monSpeed * 110) / 100;

#if SUN_BOOST_SPEED
    if (IsBattlerWeatherAffected(battler, B_WEATHER_SUN_ANY) && IsBattlerOfType(battler, TYPE_GRASS))
        monSpeed += (monSpeed / 3);
#endif

#if HAIL_BOOST_SPEED
    if (IsBattlerWeatherAffected(battler, B_WEATHER_HAIL) && IsBattlerOfType(battler, TYPE_ICE))
        monSpeed += (monSpeed / 3);
#endif

    holdEffectParam = ItemId_GetHoldEffectParam(gBattleMons[battler].item);
    
    switch (GetBattlerItemHoldEffect(battler, TRUE))
    {
        case HOLD_EFFECT_METAL_POWDER:
            if (gBattleMons[battler].species == SPECIES_DITTO && !(gBattleMons[battler].status2 & STATUS2_TRANSFORMED) && holdEffectParam == STAT_SPEED)
                monSpeed *= 2;
            break;
        case HOLD_EFFECT_CHOICE_ITEM:
            if (GetActiveGimmick(battler) != GIMMICK_DYNAMAX && holdEffectParam == STAT_SPEED)
                monSpeed = (15 * monSpeed) / 10;
            break;
    }
    if (GetBattlerItemHoldEffect(battler, FALSE) == HOLD_EFFECT_MACHO_BRACE)
        monSpeed /= 2;
    
    if (gBattleMons[battler].status1.id == STATUS1_PARALYSIS && GetBattlerAbility(battler) != ABILITY_QUICK_FEET)
#if PARALYSIS_UPDATE
        monSpeed /= 2;
#else
        monSpeed /= 4;
#endif

    return monSpeed;
}

static void SetActionsAndBattlersTurnOrder(void)
{
    u32 i, j, battlerId, turnOrderId = 0;

    if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
    {
        for (battlerId = 0; battlerId < gBattlersCount; ++battlerId)
        {
            gActionsByTurnOrder[turnOrderId] = gBattleStruct->battlers[battlerId].chosenAction;
            gBattlerByTurnOrder[turnOrderId] = battlerId;
            ++turnOrderId;
        }
    }
    else
    {
        if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        {
            for (battlerId = 0; battlerId < gBattlersCount; ++battlerId)
            {
                if (gBattleStruct->battlers[battlerId].chosenAction == B_ACTION_RUN)
                {
                    turnOrderId = 5;
                    break;
                }
            }
        }
        else if (gBattleStruct->battlers[0].chosenAction == B_ACTION_RUN)
        {
            battlerId = 0;
            turnOrderId = 5;
        }
        
        if (turnOrderId == 5) // One of battlers wants to run.
        {
            gActionsByTurnOrder[0] = gBattleStruct->battlers[battlerId].chosenAction;
            gBattlerByTurnOrder[0] = battlerId;
            
            turnOrderId = 1;
            
            for (i = 0; i < gBattlersCount; ++i)
            {
                if (i != battlerId)
                {
                    gActionsByTurnOrder[turnOrderId] = gBattleStruct->battlers[i].chosenAction;
                    gBattlerByTurnOrder[turnOrderId] = i;
                    ++turnOrderId;
                }
            }
        }
        else
        {
            for (battlerId = 0; battlerId < gBattlersCount; ++battlerId)
            {
                if (gBattleStruct->battlers[battlerId].chosenAction == B_ACTION_USE_ITEM || gBattleStruct->battlers[battlerId].chosenAction == B_ACTION_SWITCH
                || gBattleStruct->battlers[battlerId].chosenAction == B_ACTION_THROW_BALL || gBattleStruct->battlers[battlerId].chosenAction == B_ACTION_CHEER)
                {
                    gActionsByTurnOrder[turnOrderId] = gBattleStruct->battlers[battlerId].chosenAction;
                    gBattlerByTurnOrder[turnOrderId] = battlerId;
                    ++turnOrderId;
                }
            }
            
            for (battlerId = 0; battlerId < gBattlersCount; ++battlerId)
            {
                if (gBattleStruct->battlers[battlerId].chosenAction != B_ACTION_USE_ITEM && gBattleStruct->battlers[battlerId].chosenAction != B_ACTION_SWITCH
                && gBattleStruct->battlers[battlerId].chosenAction != B_ACTION_THROW_BALL && gBattleStruct->battlers[battlerId].chosenAction != B_ACTION_CHEER)
                {
                    gActionsByTurnOrder[turnOrderId] = gBattleStruct->battlers[battlerId].chosenAction;
                    gBattlerByTurnOrder[turnOrderId] = battlerId;
                    ++turnOrderId;
                }
            }
            
            for (i = 0; i < gBattlersCount - 1; ++i)
            {
                for (j = i + 1; j < gBattlersCount; ++j)
                {
                    u32 battler1 = gBattlerByTurnOrder[i];
                    u32 battler2 = gBattlerByTurnOrder[j];

                    if (gActionsByTurnOrder[i] != B_ACTION_USE_ITEM && gActionsByTurnOrder[j] != B_ACTION_USE_ITEM
                    && gActionsByTurnOrder[i] != B_ACTION_SWITCH && gActionsByTurnOrder[j] != B_ACTION_SWITCH
                    && gActionsByTurnOrder[i] != B_ACTION_THROW_BALL && gActionsByTurnOrder[j] != B_ACTION_THROW_BALL
                    && gActionsByTurnOrder[i] != B_ACTION_CHEER && gActionsByTurnOrder[j] != B_ACTION_CHEER)
                    {
                        if (GetWhoStrikesFirst(battler1, battler2, FALSE) != BATTLER1_STRIKES_FIRST)
                            SwapTurnOrder(i, j);
                    }
                }
            }
        }
    }
    gBattleStruct->quickClawBattlerId = 0;
    gBattleMainFunc = CheckChangingTurnOrderEffects;
}

static void TurnValuesCleanUp(bool32 var0)
{
    u32 battlerId, side;
    
    gBattleStruct->dancer.inProgress = FALSE;
    
    for (battlerId = 0; battlerId < gBattlersCount; ++battlerId)
    {
        if (var0)
        {
            gProtectStructs[battlerId].protected = FALSE;
            gProtectStructs[battlerId].maxGuarded = FALSE;
            gSideStatuses[GetBattlerSide(battlerId)] &= ~(SIDE_STATUS_PROTECTIONS_ANY);
            memset(&gQueuedStatBoosts[battlerId], 0, sizeof(struct QueuedStatBoost));
        }
        else
        {
            memset(&gProtectStructs[battlerId], 0, sizeof(struct ProtectStruct));
            
            if (gDisableStructs[battlerId].isFirstTurn)
                --gDisableStructs[battlerId].isFirstTurn;
            
            if (gDisableStructs[battlerId].rechargeTimer && --gDisableStructs[battlerId].rechargeTimer == 0)
                gBattleMons[battlerId].status2 &= ~(STATUS2_RECHARGE);
        }

        if (gDisableStructs[battlerId].substituteHP == 0)
            gBattleMons[battlerId].status2 &= ~(STATUS2_SUBSTITUTE);
        
        gBattleStruct->battlers[battlerId].noMoreMovingThisTurn = FALSE;
        gSpecialStatuses[battlerId].parentalBondState = PARENTAL_BOND_OFF;
    }
    
    for (side = 0; side < B_SIDE_COUNT; side++)
        gSideTimers[side].followmeSet = FALSE;
}

static void CheckChangingTurnOrderEffects(void)
{
    u32 i, battler;
    
    if (!(gHitMarker & HITMARKER_RUN))
    {
        while (gBattleStruct->quickClawBattlerId < gBattlersCount)
        {
            battler = gBattleStruct->quickClawBattlerId++;
            
            if (gBattleStruct->battlers[battler].chosenAction == B_ACTION_USE_MOVE && gBattleMons[battler].status1.id != STATUS1_SLEEP
            && (gBattleMoves[gBattleStruct->battlers[battler].chosenMove].effect != EFFECT_FOCUS_PUNCH || GetActiveGimmick(battler) == GIMMICK_DYNAMAX)
            && !gDisableStructs[battler].truantCounter && !gProtectStructs[battler].noValidMoves)
            {
                if (gSpecialStatuses[battler].quickDrawActivated)
                {
                    gSpecialStatuses[battler].quickDrawActivated = FALSE;
                    gBattleScripting.battler = battler;
                    BattleScriptExecute(BattleScript_QuickDrawActivation);
                    return;
                }
                else if (gSpecialStatuses[battler].quickClawActivated)
                {
                    gSpecialStatuses[battler].quickClawActivated = FALSE;
                    gBattleScripting.battler = battler;
                    gLastUsedItem = gBattleMons[battler].item;
                    BattleScriptExecute(BattleScript_QuickClawActivation);
                    return;
                }
            }
        }
    }
    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
        gBattleStruct->battlers[i].focusPunchDone = FALSE;
    
    TryClearRageStatuses();
    gCurrentTurnActionNumber = 0;
    gCurrentTurnActionBattlerId = gBattlerByTurnOrder[0];
    gCurrentActionFuncId = gActionsByTurnOrder[0];
    gBattleStruct->effectsBeforeUsingMoveDone = FALSE;
    gBattleMainFunc = RunTurnActionsFunctions;
    gBattleStruct->moveEffect.moveEffectByte = MOVE_EFFECT_NONE;
    gBattleCommunication[ACTIONS_CONFIRMED_COUNT] = 0;
    gBattleResources->battleScriptsStack->size = 0;
}

static bool32 TryActivateGimmick(u32 battler)
{
    u32 gimmick;
    
    if (!gProtectStructs[battler].noValidMoves && gBattleStruct->battlers[battler].toActivateGimmick)
    {
        gBattleStruct->battlers[battler].toActivateGimmick = FALSE;
        gBattleStruct->raid.gimmickActivated = TRUE;
        
        gimmick = gBattleStruct->battlers[battler].usableGimmick;
        ActivateGimmick(battler, gimmick);
        
        gBattlerAttacker = battler;
        BattleScriptExecute(GetGimmickActivationScript(gimmick));
        return TRUE;
    }
    return FALSE;
}

static bool32 TryDoGimmicksBeforeMoves(void)
{
    u32 i;
    
    if (!(gHitMarker & HITMARKER_RUN))
    {
        u8 battlers[MAX_BATTLERS_COUNT];
        
        SortBattlersBySpeed(battlers, FALSE);
        
        for (i = 0; i < gBattlersCount; i++)
        {
            if (TryActivateGimmick(battlers[i]))
                return TRUE;
        }
    }
    return FALSE;
}

static bool32 TryDoMoveEffectsBeforeMoves(void)
{
    u32 i;
    
    if (!(gHitMarker & HITMARKER_RUN))
    {
        u8 battlers[MAX_BATTLERS_COUNT];
        
        SortBattlersBySpeed(battlers, FALSE);
        
        for (i = 0; i < gBattlersCount; i++)
        {
            gBattlerAttacker = battlers[i];
            
            if (gBattleMons[gBattlerAttacker].status1.id != STATUS1_SLEEP && !gDisableStructs[gBattlerAttacker].truantCounter && !gProtectStructs[gBattlerAttacker].noValidMoves
            && !gBattleStruct->battlers[gBattlerAttacker].focusPunchDone && GetActiveGimmick(gBattlerAttacker) != GIMMICK_DYNAMAX)
            {
                gBattleStruct->battlers[gBattlerAttacker].focusPunchDone = TRUE;
                
                switch (gBattleMoves[gBattleStruct->battlers[gBattlerAttacker].chosenMove].effect)
                {
                    case EFFECT_FOCUS_PUNCH:
                        BattleScriptExecute(BattleScript_FocusPunchSetUp);
                        return TRUE;
                }
            }
        }
    }
    return FALSE;
}

static void RunTurnActionsFunctions(void)
{
    if (gBattleOutcome != 0)
        gCurrentActionFuncId = B_ACTION_FINISHED;

    // Mega Evolve / Focus Punch-like moves after switching, items, running, but before using a move.
    if (gCurrentActionFuncId == B_ACTION_USE_MOVE && !gBattleStruct->effectsBeforeUsingMoveDone)
    {
        if (!gBattleStruct->pursuitSwitchDmg)
        {
            if (TryDoGimmicksBeforeMoves())
                return;
            else if (TryDoMoveEffectsBeforeMoves())
                return;
            
            gBattleStruct->effectsBeforeUsingMoveDone = TRUE;
        }
        else
        {
            if (TryActivateGimmick(gBattlerByTurnOrder[gCurrentTurnActionNumber]))
                return;
        }
    }
    gBattleStruct->savedTurnActionNumber = gCurrentTurnActionNumber;
    sTurnActionsFuncsTable[gCurrentActionFuncId]();

    if (gCurrentTurnActionNumber >= gBattlersCount) // everyone did their actions, turn finished
    {
        gHitMarker &= ~(HITMARKER_PASSIVE_DAMAGE);
        gBattleMainFunc = sEndTurnFuncsTable[gBattleOutcome & 0x7F];
    }
    else
    {
        if (gBattleStruct->savedTurnActionNumber != gCurrentTurnActionNumber) // action turn has been done, clear hitmarker bits for another battlerId
            gHitMarker &= ~(HITMARKER_NO_ATTACKSTRING | HITMARKER_UNABLE_TO_USE_MOVE);
    }
}

static void HandleEndTurn_BattleWon(void)
{
    gCurrentActionFuncId = 0;
    
    if (!(gBattleTypeFlags & BATTLE_TYPE_RAID))
    {
        if ((gBattleTypeFlags & BATTLE_TYPE_LINK) || ((gBattleTypeFlags & BATTLE_TYPE_RECORDED) && (gBattleTypeFlags & BATTLE_TYPE_TRAINER)))
        {
            gBattleTextBuff1[0] = gBattleOutcome;
            gBattlerAttacker = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
            gBattlescriptCurrInstr = BattleScript_LinkBattleWonOrLost;
            gBattleOutcome &= ~(B_OUTCOME_LINK_BATTLE_RAN);
        }
        else if ((gBattleTypeFlags & BATTLE_TYPE_TRAINER) && !(gBattleTypeFlags & BATTLE_TYPE_LINK))
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
            gBattlescriptCurrInstr = BattleScript_PayDayMoneyAndPickUpItems;
    }
    else
        gBattlescriptCurrInstr = BattleScript_PayDayMoneyAndPickUpItems;
    
    gBattleMainFunc = HandleEndTurn_FinishBattle;
}

static void HandleEndTurn_BattleLost(void)
{
    gCurrentActionFuncId = 0;
    
    if ((gBattleTypeFlags & BATTLE_TYPE_LINK) || ((gBattleTypeFlags & BATTLE_TYPE_RECORDED) && (gBattleTypeFlags & BATTLE_TYPE_TRAINER)))
    {
        gBattleTextBuff1[0] = gBattleOutcome;
        gBattlerAttacker = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        gBattlescriptCurrInstr = BattleScript_LinkBattleWonOrLost;
        gBattleOutcome &= ~(B_OUTCOME_LINK_BATTLE_RAN);
    }
    else
    {
        if ((gBattleTypeFlags & BATTLE_TYPE_TRAINER) && GetTrainerBattleMode() == TRAINER_BATTLE_EARLY_RIVAL)
        {
            gBattleCommunication[MULTIUSE_STATE] = (GetRivalBattleFlags() & RIVAL_BATTLE_HEAL_AFTER) ? 1 : 2; // Do or not white out text
            gBattlerAttacker = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
        }
        else
            gBattleCommunication[MULTIUSE_STATE] = 0;
        
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
    PrepareMonNickBuffer(gBattleTextBuff1, gBattlerAttacker, gBattlerPartyIndexes[gBattlerAttacker]);
    gBattlescriptCurrInstr = BattleScript_WildMonFled;
    gBattleMainFunc = HandleEndTurn_FinishBattle;
}

static void HandleEndTurn_ActualFinishBattle(void)
{
    if (gCurrentActionFuncId == B_ACTION_TRY_FINISH || gCurrentActionFuncId == B_ACTION_FINISHED)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
            ClearRematchStateByTrainerId();
        
        if (gTestRunnerEnabled)
            TestRunner_Battle_AfterLastTurn();
    
        BeginFastPaletteFade(FAST_FADE_OUT_TO_BLACK);
        FadeOutMapMusic(5);
    
        gBattleMainFunc = FreeResetData_ReturnToOvOrDoEvolutions;
        gCB2_AfterEvolution = BattleMainCB2;
    }
    else if (!gBattleControllerExecFlags)
        gBattleScriptingCommandsTable[gBattlescriptCurrInstr[0]]();
}

static void HandleEndTurn_FinishBattle(void)
{
    if (gCurrentActionFuncId == B_ACTION_TRY_FINISH || gCurrentActionFuncId == B_ACTION_FINISHED)
    {
        gCurrentActionFuncId = 0;
        gBattlescriptCurrInstr = BattleScript_RunBattleEndFunctions;
        gBattleMainFunc = HandleEndTurn_ActualFinishBattle;
    }
    else if (!gBattleControllerExecFlags)
        gBattleScriptingCommandsTable[gBattlescriptCurrInstr[0]]();
}

static void FreeResetData_ReturnToOvOrDoEvolutions(void)
{
    if (!gPaletteFade.active)
    {
        gIsFishingEncounter = FALSE;
        gIsSurfingEncounter = FALSE;
        gPartnerTrainerId = PARTNER_NONE;
        
        ResetSpriteData();
        FreeAllWindowBuffers();
        
        if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
        {
            FreeMonSpritesGfx();
            FreeBattleSpritesData();
            FreeBattleResources();
        }
        IncrementOrResetDexNavChain((gDexnavBattle && (gBattleOutcome == B_OUTCOME_WON || gBattleOutcome == B_OUTCOME_CAUGHT)));
        
        gBattleMainFunc = TryEvolvePokemon;
    }
}

static void TryEvolvePokemon(void)
{
    u32 i, species;
    
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (!(gTriedEvolving & Bit(i)))
        {
            gTriedEvolving |= Bit(i);
            
            species = GetEvolutionTargetSpecies(i, EVO_MODE_BATTLE_SPECIAL, ITEM_NONE, NULL, FALSE);
            
            if (!species && (gLeveledUpInBattle & Bit(i)))
            {
                gLeveledUpInBattle &= ~(Bit(i));
                species = GetEvolutionTargetSpecies(i, EVO_MODE_NORMAL, ITEM_NONE, NULL, FALSE);
            }
            
            if (species)
            {
                gBattleMainFunc = WaitForEvoSceneToFinish;
                EvolutionScene(&gPlayerParty[i], species, (TASK_BIT_CAN_STOP | TASK_BIT_LEARN_MOVE), i);
                return;
            }
        }
    }
    gTriedEvolving = 0;
    gLeveledUpInBattle = 0;
    gBattleMainFunc = ReturnFromBattleToOverworld;
}

static void WaitForEvoSceneToFinish(void)
{
    if (gMain.callback2 == BattleMainCB2)
        gBattleMainFunc = TryEvolvePokemon;
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
            
            if (gBattleOutcome == B_OUTCOME_WON || gBattleOutcome == B_OUTCOME_CAUGHT)
                SetRoamerInactive();
        }
        m4aSongNumStop(SE_LOW_HEALTH);
        BattleEndClearFlags();
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

static void TryChangeTurnOrder(void)
{
    u32 i, j;
    
    for (i = gCurrentTurnActionNumber; i < gBattlersCount - 1; i++)
    {
        for (j = i + 1; j < gBattlersCount; j++)
        {
            if (gActionsByTurnOrder[i] != B_ACTION_USE_ITEM && gActionsByTurnOrder[j] != B_ACTION_USE_ITEM
            && gActionsByTurnOrder[i] != B_ACTION_SWITCH && gActionsByTurnOrder[j] != B_ACTION_SWITCH
            && gActionsByTurnOrder[i] != B_ACTION_THROW_BALL && gActionsByTurnOrder[j] != B_ACTION_THROW_BALL
            && gActionsByTurnOrder[i] != B_ACTION_FINISHED && gActionsByTurnOrder[j] != B_ACTION_FINISHED
            && gActionsByTurnOrder[i] != B_ACTION_CHEER && gActionsByTurnOrder[j] != B_ACTION_CHEER
            && !(gBattleMons[gBattlerByTurnOrder[i]].status2 & STATUS2_TURN_ORDER_LOCKED) && !(gBattleMons[gBattlerByTurnOrder[j]].status2 & STATUS2_TURN_ORDER_LOCKED))
            {
                if (GetWhoStrikesFirst(gBattlerByTurnOrder[i], gBattlerByTurnOrder[j], FALSE) != BATTLER1_STRIKES_FIRST)
                    SwapTurnOrder(i, j);
            }
        }
    }
}

static inline void UseMoveAction_ChooseMove(u32 attacker)
{
    u32 move;
    
    gCurrMovePos = gChosenMovePos = gBattleStruct->battlers[attacker].chosenMovePosition;
    
    if (gProtectStructs[attacker].noValidMoves) // No moves available
    {
        gProtectStructs[gBattlerAttacker].noValidMoves = FALSE;
        gHitMarker |= HITMARKER_NO_PPDEDUCT;
        move = MOVE_STRUGGLE;
        gBattleStruct->battlers[attacker].moveTarget = GetMoveTarget(move, 0, TRUE);
    }
    else if ((gBattleMons[attacker].status2 & (STATUS2_MULTIPLETURNS | STATUS2_RECHARGE))) // Move locked
        move = gBattleStruct->battlers[attacker].lockedMove;
    else if (gDisableStructs[attacker].encoredMove) // Encore forces you to use the same move
    {
        gCurrMovePos = gChosenMovePos = gDisableStructs[attacker].encoredMovePos;
        
        // Check if the encored move wasn't overwritten
        if (gDisableStructs[attacker].encoredMove == gBattleMons[attacker].moves[gDisableStructs[attacker].encoredMovePos])
            move = gDisableStructs[attacker].encoredMove;
        else
        {
            gDisableStructs[attacker].encoredMove = MOVE_NONE;
            gDisableStructs[attacker].encoreTimer = 0;
            move = gBattleMons[attacker].moves[gCurrMovePos];
        }
        gBattleStruct->battlers[attacker].moveTarget = GetMoveTarget(move, 0, TRUE);
    }
    else
    {
        move = gBattleMons[attacker].moves[gCurrMovePos];
        
        if (move != gBattleStruct->battlers[attacker].chosenMove)
            gBattleStruct->battlers[attacker].moveTarget = GetMoveTarget(move, 0, TRUE);
    }
    gBattleStruct->dynamicMoveType = GetBattlerMoveType(attacker, move); // Set dynamic move type

    // Try turn move into Max Move
    if (GetActiveGimmick(attacker) == GIMMICK_DYNAMAX)
    {
        if (!IsRaidBossUsingRegularMove(attacker, move))
        {
            u32 maxMove = GetMaxMove(attacker, gBattleMons[attacker].species, move, gBattleStruct->dynamicMoveType, gBattleMoves[move].split);
            
            if (maxMove != move)
            {
                gBattleStruct->battlers[attacker].baseUsedMove = move;
                gBattleStruct->categoryOverride = gBattleMoves[move].split;
                move = maxMove;
            }
        }
    }
    gCurrentMove = gChosenMove = move;
}

static inline u32 UseMoveAction_ChooseTarget(u32 moveTarget)
{
    u32 opposingBattler = BATTLE_OPPOSITE(gBattlerAttacker);
    u32 newTarget, opposingSide = GetBattlerSide(opposingBattler);
    
    // Check Follow Me
    if (IsBattlerAffectedByFollowMe(gBattlerAttacker, opposingSide, gCurrentMove) && (moveTarget == MOVE_TARGET_SELECTED || moveTarget == MOVE_TARGET_SELECTED_OPPONENT
    || moveTarget == MOVE_TARGET_RANDOM) && !IsBattlerAlly(gBattlerAttacker, gSideTimers[opposingSide].followmeTarget))
        return gSideTimers[opposingSide].followmeTarget;
    
    // Check Lightning Rod and Storm Drain redirection
    if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && !gSideTimers[opposingSide].followmeSet && IsMoveAffectedByRedirectionEffects(gBattlerAttacker, gCurrentMove)
    && (GetBattleMoveSplit(gCurrentMove) != SPLIT_STATUS || (moveTarget != MOVE_TARGET_USER && moveTarget != MOVE_TARGET_ALL_BATTLERS))
    && moveTarget != MOVE_TARGET_FOES_AND_ALLY && moveTarget != MOVE_TARGET_BOTH)
    {
        switch (gBattleStruct->dynamicMoveType)
        {
            case TYPE_ELECTRIC:
                if (GetBattlerAbility(gBattleStruct->battlers[gBattlerAttacker].moveTarget) != ABILITY_LIGHTNING_ROD)
                {
                    if ((newTarget = ABILITY_ON_FIELD_EXCEPT_BATTLER(gBattlerAttacker, ABILITY_LIGHTNING_ROD)))
                    {
                        newTarget--;
                        gSpecialStatuses[newTarget].abilityRedirected = TRUE;
                        return newTarget;
                    }
                }
                break;
            case TYPE_WATER:
                if (GetBattlerAbility(gBattleStruct->battlers[gBattlerAttacker].moveTarget) != ABILITY_STORM_DRAIN)
                {
                    if ((newTarget = ABILITY_ON_FIELD_EXCEPT_BATTLER(gBattlerAttacker, ABILITY_STORM_DRAIN)))
                    {
                        newTarget--;
                        gSpecialStatuses[newTarget].abilityRedirected = TRUE;
                        return newTarget;
                    }
                }
                break;
        }
    }
    
    // Specific battler targeting
    switch (moveTarget)
    {
        case MOVE_TARGET_USER:
            return gBattlerAttacker;
        case MOVE_TARGET_ALLY:
            if (IsBattlerAlive(BATTLE_PARTNER(gBattlerAttacker)) && !gProtectStructs[BATTLE_PARTNER(gBattlerAttacker)].usedAllySwitch)
                return BATTLE_PARTNER(gBattlerAttacker);
            else
                return gBattlerAttacker;
    }
    
    // Moves hitting multiples Pokémons in doubles
    if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && moveTarget == MOVE_TARGET_FOES_AND_ALLY)
    {
        for (newTarget = 0; newTarget < gBattlersCount; newTarget++)
        {
            if (newTarget != gBattlerAttacker && IsBattlerAlive(newTarget))
                break;
        }
        return newTarget;
    }
    else if (IsDoubleBattleForBattler(opposingBattler) && moveTarget == MOVE_TARGET_RANDOM)
        newTarget = GetRandomTarget(gBattlerAttacker);
    else
        newTarget = gBattleStruct->battlers[gBattlerAttacker].moveTarget;
        
    if (!IsBattlerAlive(newTarget))
    {
        if (!IsBattlerAlly(gBattlerAttacker, newTarget))
            newTarget = BATTLE_PARTNER(newTarget);
        else
        {
            newTarget = opposingBattler;
            
            if (!IsBattlerAlive(newTarget))
                newTarget = BATTLE_PARTNER(newTarget);
        }
    }
    return newTarget;
}

static void HandleAction_UseMove(void)
{
    u32 moveTarget;
    
    // Recalc current turn order
    TryChangeTurnOrder();
    gBattlerAttacker = gCurrentTurnActionBattlerId = gBattlerByTurnOrder[gCurrentTurnActionNumber];
    
    if ((gBattleStruct->absentBattlerFlags & Bit(gBattlerAttacker)) || !IsBattlerAlive(gBattlerAttacker))
    {
        gCurrentActionFuncId = B_ACTION_FINISHED;
        return;
    }
    gIsCriticalHit = FALSE;
    gBattleStruct->atkCancellerTracker = 0;
    gBattleStruct->magnitudeBasePower = 0;
    gBattleStruct->strongWindsMessageState = 0;
    gBattleStruct->dancer.inProgress = FALSE;
    gMoveResultFlags = 0;
    gMultiHitCounter = 0;
    gBattleScripting.savedDmg = 0;
    gBattleCommunication[MISS_TYPE] = B_MSG_MISSED;
    
    UseMoveAction_ChooseMove(gBattlerAttacker); // Choose move

    moveTarget = GetBattlerMoveTargetType(gBattlerAttacker, gCurrentMove);
    
    gBattlerTarget = UseMoveAction_ChooseTarget(moveTarget); // Choose target

    SaveBattlersHps(); // For abilities
    
    if (gBattleTypeFlags & BATTLE_TYPE_RAID)
        gBattleStruct->raid.turnStartHP = gBattleMons[gBattleStruct->sos.totemBattlerId].hp;
    
    // Choose script to execute
    if (IsBattlerAlly(gBattlerAttacker, gBattlerTarget) && !IsBattlerAlive(gBattlerTarget))
        gBattlescriptCurrInstr = BattleScript_ButItFailedAtkCanceler;
    else if ((moveTarget == MOVE_TARGET_ALLY || moveTarget == MOVE_TARGET_USER_OR_ALLY) && gBattlerAttacker == gBattlerTarget && gProtectStructs[BATTLE_PARTNER(gBattlerAttacker)].usedAllySwitch)
        gBattlescriptCurrInstr = BattleScript_ButItFailedAtkCanceler;
    else
        gBattlescriptCurrInstr = GET_MOVE_BATTLESCRIPT(gCurrentMove);
    
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

static void HandleAction_Switch(void)
{
    gBattlerAttacker = gCurrentTurnActionBattlerId;
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    gActionSelectionCursor[gBattlerAttacker] = 0;
    gBattleStruct->battlers[gBattlerAttacker].moveSelectionCursor = 0;
    PrepareMonNickBuffer(gBattleTextBuff1, gBattlerAttacker, gBattleStruct->battlers[gBattlerAttacker].partyIndex);
    gBattleScripting.battler = gBattlerAttacker;
    gBattlescriptCurrInstr = BattleScript_ActionSwitch;
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

static void HandleAction_UseItem(void)
{
    gBattlerAttacker = gCurrentTurnActionBattlerId;
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

static u32 GetSideAverageSpeed(u32 battler)
{
    u32 i, numBattlers = 0;
    u32 speed = 0;
    
    for (i = battler; i < NUM_BATTLERS_PER_SIDE; i = BATTLE_PARTNER(i))
    {
        if (IsBattlerAlive(i))
        {
            speed += GetBattlerTotalSpeed(i);
            numBattlers++;
        }
    }
    
    if (numBattlers)
        speed /= numBattlers;
    
    return speed;
}

bool32 TryRunFromBattle(u32 battler)
{
    bool32 effect = FALSE;
    u32 holdEffect = GetBattlerItemHoldEffect(battler, TRUE);
    u32 speedVar, battlerSpeed, escapeFromSpeed;

    if (holdEffect == HOLD_EFFECT_CAN_ALWAYS_RUN)
    {
        gLastUsedItem = gBattleMons[battler].item;
        gProtectStructs[battler].fleeFlag = 1;
        effect = TRUE;
    }
    else if (IsBattlerOfType(battler, TYPE_GHOST))
        effect = TRUE;
    else if (GetBattlerAbility(battler) == ABILITY_RUN_AWAY)
    {
        gProtectStructs[battler].fleeFlag = 2;
        effect = TRUE;
    }
    else if (IS_BATTLE_TYPE_GHOST_WITHOUT_SCOPE)
    {
        if (GetBattlerSide(battler) == B_SIDE_PLAYER)
            effect = TRUE;
    }
    else
    {
        battlerSpeed = GetSideAverageSpeed(battler);
        escapeFromSpeed = GetSideAverageSpeed(BATTLE_OPPOSITE(battler));
        
        if (battlerSpeed < escapeFromSpeed)
        {
            speedVar = (battlerSpeed * 128) / (escapeFromSpeed) + (gBattleStruct->runTries * 30);
            if (speedVar > (Random() & 0xFF))
                effect = TRUE;
        }
        else // same speed or faster
            effect = TRUE;

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
    u32 battlerId;
    
    gBattlerAttacker = gCurrentTurnActionBattlerId;

    if ((gBattleTypeFlags & BATTLE_TYPE_LINK) || ((gBattleTypeFlags & BATTLE_TYPE_RECORDED) && (gBattleTypeFlags & BATTLE_TYPE_TRAINER)))
    {
        gCurrentTurnActionNumber = gBattlersCount;
        
        for (battlerId = 0; battlerId < gBattlersCount; ++battlerId)
        {
            if (gBattleStruct->battlers[battlerId].chosenAction == B_ACTION_RUN)
            {
                if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
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
                gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CANT_ESCAPE;
                gBattlescriptCurrInstr = BattleScript_PrintFailedToRunString;
                gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
            }
        }
        else
        {
            if (!CanBattlerEscape(gBattlerAttacker, TRUE))
            {
                gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_ATK_CANT_ESCAPE;
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
    gBattlerAttacker = gCurrentTurnActionBattlerId;
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    
    if (gBattleStruct->safariGoNearCounter != 0)
    {
        if (--gBattleStruct->safariGoNearCounter == 0)
        {
            *(&gBattleStruct->safariCatchFactor) = gSpeciesInfo[GetMonData(&gEnemyParty[0], MON_DATA_SPECIES)].catchRate * 100 / 1275;
            gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_WATCHING_CAREFULLY;
        }
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_ANGRY;
    }
    else
    {
        if (gBattleStruct->safariPkblThrowCounter != 0)
        {
            if (--gBattleStruct->safariPkblThrowCounter == 0)
                gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_WATCHING_CAREFULLY;
            else
                gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_EATING;
        }
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_WATCHING_CAREFULLY;
    }
    gBattlescriptCurrInstr = BattleScript_WatchesCarefully;
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

static void HandleAction_SafariZoneBallThrow(void)
{
    gBattlerAttacker = gCurrentTurnActionBattlerId;
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    --gNumSafariBalls;
    gLastUsedItem = ITEM_SAFARI_BALL;
    gBattlescriptCurrInstr = BattleScript_ThrowBall;
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

static void HandleAction_ThrowBait(void)
{
    gBattlerAttacker = gCurrentTurnActionBattlerId;
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    
    gBattleStruct->safariPkblThrowCounter += RandomUniform(RNG_SAFARI_BAIT_THROW, 2, 6);
    if (gBattleStruct->safariPkblThrowCounter > 6)
        gBattleStruct->safariPkblThrowCounter = 6;
    
    gBattleStruct->safariGoNearCounter = 0;
    
    gBattleStruct->safariCatchFactor >>= 1;
    if (gBattleStruct->safariCatchFactor <= 2)
        gBattleStruct->safariCatchFactor = 3;
    
    gBattlescriptCurrInstr = BattleScript_ThrowBait;
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

static void HandleAction_ThrowRock(void)
{
    gBattlerAttacker = gCurrentTurnActionBattlerId;
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    
    gBattleStruct->safariGoNearCounter += RandomUniform(RNG_SAFARI_ROCK_THROW, 2, 6);
    if (gBattleStruct->safariGoNearCounter > 6)
        gBattleStruct->safariGoNearCounter = 6;
    
    gBattleStruct->safariPkblThrowCounter = 0;
    
    gBattleStruct->safariCatchFactor <<= 1;
    if (gBattleStruct->safariCatchFactor > 20)
        gBattleStruct->safariCatchFactor = 20;
    
    gBattlescriptCurrInstr = BattleScript_ThrowRock;
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

static void HandleAction_SafariZoneRun(void)
{
    gBattlerAttacker = gCurrentTurnActionBattlerId;
    PlaySE(SE_FLEE);
    gCurrentTurnActionNumber = gBattlersCount;
    gBattleOutcome = B_OUTCOME_RAN;
}

static void HandleAction_OldManBallThrow(void)
{
    gBattlerAttacker = gCurrentTurnActionBattlerId;
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    PrepareMonNickBuffer(gBattleTextBuff1, gBattlerAttacker, gBattlerPartyIndexes[gBattlerAttacker]);
    gBattlescriptCurrInstr = BattleScript_LeftoverWallyPrepToThrow;
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
    gCurrentTurnActionBattlerId = gBattlerByTurnOrder[gCurrentTurnActionNumber];
    gCurrentActionFuncId = gActionsByTurnOrder[gCurrentTurnActionNumber];
    gHitMarker &= ~(HITMARKER_DESTINYBOND | HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_ATTACKSTRING_PRINTED | HITMARKER_NO_PPDEDUCT | HITMARKER_PASSIVE_DAMAGE
                  | HITMARKER_OBEYS | HITMARKER_SYNCHRONISE_EFFECT | HITMARKER_CHARGING | HITMARKER_IGNORE_DISGUISE);
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
    gBattleStruct->battlers[gBattlerAttacker].lastLandedMove = 0;
    gBattleStruct->battlers[gBattlerAttacker].lastHitMoveType = 0;
    gBattleStruct->dynamicMoveType = 0;
    gBattleScripting.atk48_state = 0;
    gBattleScripting.atk49_state = 0;
    gBattleStruct->moveEffect.moveEffectByte = MOVE_EFFECT_NONE;
    gBattleCommunication[ACTIONS_CONFIRMED_COUNT] = 0;
    gBattleResources->battleScriptsStack->size = 0;
}

static void HandleAction_LastUsedBallThrow(void)
{
    gBattlerAttacker = gCurrentTurnActionBattlerId;
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    gLastUsedItem = gBallToDisplay;
    RemoveBagItem(gLastUsedItem, 1);
    gBattlescriptCurrInstr = BattleScript_ThrowBall;
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

static void HandleAction_Cheer(void)
{
    gBattlerAttacker = gBattleScripting.battler = gCurrentTurnActionBattlerId;
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    gBattlescriptCurrInstr = gRaidCheerEffectsTable[GetRaidSheerEffect()];
    BattleScriptCall(BattleScript_RaidCheerMessage);
    gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

static void BattleEndClearFlags(void)
{
    memset(&gBattleMonForms, 0, sizeof(gBattleMonForms));

    FlagClear(FLAG_DYNAMAX_ENABLED);
    FlagClear(FLAG_START_RAID_SHIELDS);
    
    SetMainCallback2(gMain.savedCallback);
}

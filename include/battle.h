#ifndef GUARD_BATTLE_H
#define GUARD_BATTLE_H

#include <limits.h>
#include "global.h"
#include "constants/battle.h"
#include "battle_util.h"
#include "battle_queued_effects.h"
#include "battle_script_commands.h"
#include "battle_main.h"
#include "battle_ai_switch_items.h"
#include "battle_gfx_sfx_util.h"
#include "battle_util2.h"
#include "battle_bg.h"
#include "constants/battle_script_commands.h"

/*
    Banks are a name given to what could be called a 'battlerId' or 'monControllerId'.
    Each bank has a value consisting of two bits.
    0x1 bit is responsible for the side, 0 = player's side, 1 = opponent's side.
    0x2 bit is responsible for the id of sent out pokemon. 0 means it's the first sent out pokemon, 1 it's the second one. (Triple battle didn't exist at the time yet.)
*/

#define B_ACTION_USE_MOVE                  0
#define B_ACTION_USE_ITEM                  1
#define B_ACTION_SWITCH                    2
#define B_ACTION_RUN                       3
#define B_ACTION_SAFARI_WATCH_CAREFULLY    4
#define B_ACTION_SAFARI_BALL               5
#define B_ACTION_SAFARI_BAIT               6
#define B_ACTION_SAFARI_GO_NEAR            7
#define B_ACTION_SAFARI_RUN                8
#define B_ACTION_OLDMAN_THROW              9
#define B_ACTION_EXEC_SCRIPT               10
#define B_ACTION_TRY_FINISH                11
#define B_ACTION_FINISHED                  12
#define B_ACTION_CANCEL_PARTNER            12 // when choosing an action
#define B_ACTION_NOTHING_FAINTED           13 // when choosing an action
#define B_ACTION_NONE                      0xFF

#define MOVE_TARGET_SELECTED          0
#define MOVE_TARGET_DEPENDS           (1 << 0)
#define MOVE_TARGET_USER_OR_SELECTED  (1 << 1) // unused
#define MOVE_TARGET_RANDOM            (1 << 2)
#define MOVE_TARGET_OPPONENTS_FIELD   (1 << 3)
#define MOVE_TARGET_BOTH              (1 << 4)
#define MOVE_TARGET_USER              (1 << 5)
#define MOVE_TARGET_ALLY              (1 << 6)
#define MOVE_TARGET_USER_OR_ALLY      (MOVE_TARGET_USER | MOVE_TARGET_ALLY)
#define MOVE_TARGET_FOES_AND_ALLY     (MOVE_TARGET_BOTH | MOVE_TARGET_ALLY)
#define MOVE_TARGET_ALL_BATTLERS      (MOVE_TARGET_FOES_AND_ALLY | MOVE_TARGET_USER)

#define MAX_TRAINER_ITEMS 4

#define TRAINER_MON_MALE       1
#define TRAINER_MON_FEMALE     2
#define TRAINER_MON_GENDERLESS 3

struct TrainerMon
{
	/*0x00*/ const u8 *nickname;
	/*0x04*/ const u8 *ev;
	/*0x08*/ u16 species;
	/*0x0A*/ u16 heldItem;
	/*0x0C*/ u16 moves[MAX_MON_MOVES];
	/*0x14*/ u32 iv;
	/*0x18*/ u8 lvl;
	/*0x19*/ u8 abilityNum:2; // 0 = based on personality, 1 = first, 2 = second, 3 = hidden
	         u8 nature:5; // 0 = based on personality. Up to 31 natures
	         u8 isShiny:1;
	/*0x1A*/ u8 ballId:6; // 0 = poke ball based on trainer's class. Up to 63 poke balls
			 u8 gender:2; // 0 = based on personality, 1 = male, 2 = female, 3 = genderless
};

struct Trainer
{
	/*0x00*/ u8 trainerClass;
    /*0x01*/ u8 encounterMusic_gender; // last bit is gender
	/*0x02*/ u8 partySize;
	/*0x03*/ u8 trainerPic;
	/*0x04*/ u8 trainerName[12];
    /*0x10*/ u16 items[MAX_TRAINER_ITEMS];
    /*0x18*/ u32 aiFlags;
	/*0x1C*/ const struct TrainerMon *party;
	/*0x20*/ bool8 doubleBattle;
};

struct TrainerSlide
{
	/*0x00*/ const u8 *firstMonMsg;
	/*0x04*/ const u8 *firstMonDownMsg;
	/*0x08*/ const u8 *lastMonMsg;
	/*0x0C*/ const u8 *lastMonLowHPMsg;
	/*0x10*/ const u8 *firstSuperEffectiveHitTakenMsg;
	/*0x14*/ const u8 *firstCriticalTakenMsg;
	/*0x18*/ const u8 *megaEvoMsg;
	/*0x1C*/ const u8 *zMoveMsg;
	/*0x20*/ const u8 *dynamaxMsg;
	/*0x24*/ const u8 *terastalMsg;
};

extern const struct Trainer gTrainers[];
extern const struct TrainerSlide gTrainerSlides[];

#define RESOURCE_FLAG_FLASH_FIRE       (1 << 0)
#define RESOURCE_FLAG_TRACED           (1 << 1)
#define RESOURCE_FLAG_UNBURDEN_BOOST   (1 << 2)
#define RESOURCE_FLAG_NEUTRALIZING_GAS (1 << 3)
#define RESOURCE_FLAG_EMERGENCY_EXIT   (1 << 4)

struct ResourceFlags
{
    u32 flags[MAX_BATTLERS_COUNT];
};

struct DisableStruct
{
    /*0x00*/ u32 transformedMonPersonality;
	/*0x04*/ u16 disabledMove;
    /*0x06*/ u16 encoredMove;
    /*0x08*/ u8 protectUses;
    /*0x09*/ u8 stockpileCounter;
    /*0x0A*/ u8 substituteHP;
    /*0x0B*/ u8 truantCounter:1;
    /*0x0B*/ u8 disableTimer:3;
    /*0x0B*/ u8 encoreTimer:2;
    /*0x0B*/ u8 chargeTimer:2;
    /*0x0C*/ u16 perishSongTimer:2;
	/*0x0C*/ u16 isFirstTurn:2;
    /*0x0C*/ u16 tauntTimer:3;
    /*0x0C*/ u16 furyCutterCounter:2;
    /*0x0C*/ u16 rolloutTimer:3;
    /*0x0C*/ u16 destinyBondCounter:2;
	/*0x0C*/ u16 enduredHit:1;
	/*0x0C*/ u16 transformedMonShynies:1;
    /*0x0E*/ u8 encoredMovePos;
    /*0x0F*/ u8 mimickedMoves:4;
	/*0x0F*/ u8 canProteanActivate:1;
    /*0x0F*/ u8 slowStartTimer:3;
    /*0x10*/ u8 battlerPreventingEscape;
    /*0x11*/ u8 battlerWithSureHit;
    /*0x12*/ u8 rechargeTimer;
	/*0x13*/ u8 imposterActivated:1; // only activate when switched in, not when gained
	/*0x13*/ u8 infatuatedWith:2;
	/*0x13*/ u8 unused:5;
};

extern struct DisableStruct gDisableStructs[MAX_BATTLERS_COUNT];

struct ProtectStruct
{
    /* field_0 */
    u32 protected:1;
    u32 endured:1;
    u32 noValidMoves:1;
    u32 pranksterElevated:1;
    u32 bounceMove:1;
    u32 stealMove:1;
    u32 notFirstStrike:1;
    u32 prlzImmobility:1;
    /* field_1 */
    u32 confusionSelfDmg:1;
    u32 targetNotAffected:1;
    u32 chargingTurn:1;
    u32 fleeFlag:2; // for RunAway and Smoke Ball
    u32 usedImprisonedMove:1;
    u32 loveImmobility:1;
    u32 usedDisabledMove:1;
    /* field_2 */
    u32 usedTauntedMove:1;
    u32 flinchImmobility:1;
    u32 usesBouncedMove:1;
    u32 myceliumMightElevated:1;
	u32 helpingHandUses:3;
    u32 flag_x80:1;
	/* field_3 */
    u32 field3:8;

    u32 physicalDmg;
    u32 specialDmg;
    u8 physicalBattlerId;
    u8 specialBattlerId;
};

extern struct ProtectStruct gProtectStructs[MAX_BATTLERS_COUNT];

struct SpecialStatus
{
	s32 dmg;
    s32 physicalDmg;
    s32 specialDmg;
    u32 statLowered:1;
    u32 lightningRodRedirected:1;
	u32 stormDrainRedirected:1;
    u32 restoredBattlerSprite:1;
    u32 removedWeatherChangeAbility:1;
    u32 faintedHasReplacement:1;
    u32 focusBanded:1;
    u32 sturdied:1;
	// end of byte
	u32 traced:1;
	u32 switchInAbilityDone:1;
	u32 weatherAbilityDone:1;
	u32 terrainAbilityDone:1;
	u32 announceNeutralizingGas:1;
	u32 removedNeutralizingGas:1;
	u32 parentalBondState:2;
	// end of byte
	u32 multiHitOn:1;
	u32 announceUnnerve:1;
	u32 unused:6;
	// end of byte
	u32 unused2:8;
    u8 physicalBattlerId;
    u8 specialBattlerId;
	u8 changedStatsBattlerId;
};

extern struct SpecialStatus gSpecialStatuses[MAX_BATTLERS_COUNT];

struct SideTimer
{
    /*0x00*/ u32 reflectTimer:4;
    /*0x00*/ u32 reflectBattlerId:2;
	/*0x00*/ u32 lightscreenTimer:4;
	/*0x00*/ u32 lightscreenBattlerId:2;
	/*0x00*/ u32 mistTimer:3;
	/*0x00*/ u32 mistBattlerId:2;
	/*0x00*/ u32 safeguardTimer:3;
	/*0x00*/ u32 safeguardBattlerId:2;
	/*0x00*/ u32 followmeSet:1;
	/*0x00*/ u32 followmeTarget:2;
	/*0x00*/ u32 spikesAmount:2;
	/*0x00*/ u32 unused:5;
	// end of word
};

extern struct SideTimer gSideTimers[B_SIDE_COUNT];

struct FieldTimer
{
	u8 waterSportTimer:3;
	u8 mudSportTimer:3;
};

struct WishFutureKnock
{
    u8 futureSightCounter[MAX_BATTLERS_COUNT];
    u8 futureSightAttacker[MAX_BATTLERS_COUNT];
    u16 futureSightMove[MAX_BATTLERS_COUNT];
    u8 wishCounter[MAX_BATTLERS_COUNT];
    u8 wishMonId[MAX_BATTLERS_COUNT];
    u8 weatherDuration;
    u8 knockedOffMons[B_SIDE_COUNT]; // as bit using gBitTable
};

extern struct WishFutureKnock gWishFutureKnock;

struct AI_ThinkingStruct
{
    u8 aiState;
    u8 movesetIndex;
    u16 moveConsidered;
    s8 score[4];
    u32 funcResult;
    u32 aiFlags;
    u8 simulatedRNG[4];
    u8 aiAction;
    u8 aiLogicId;
};

extern struct BattlePokemon gBattleMons[MAX_BATTLERS_COUNT];

struct BattleHistory
{
    /*0x00*/ u16 usedMoves[2][8]; // 0xFFFF means move not used (confuse self hit, etc)
    /*0x20*/ u16 abilities[MAX_BATTLERS_COUNT / 2];
    /*0x24*/ u8 itemEffects[MAX_BATTLERS_COUNT / 2];
    /*0x26*/ u16 trainerItems[MAX_BATTLERS_COUNT];
    /*0x2E*/ u8 itemsNo;
};

struct BattleScriptsStack
{
    const u8 *ptr[8];
    u8 size;
};

struct BattleCallbacksStack
{
    void (*function[8])(void);
    u8 size;
};

struct StatsArray
{
    u16 stats[NUM_STATS];
};

struct BattleResources
{
    struct ResourceFlags *flags;
    struct BattleScriptsStack *battleScriptsStack;
    struct BattleCallbacksStack *battleCallbackStack;
    struct StatsArray *beforeLvlUp;
    struct AI_ThinkingStruct *ai;
    struct BattleHistory *battleHistory;
    struct BattleScriptsStack *AI_ScriptsStack;
};

extern struct BattleResources *gBattleResources;

struct LinkPartnerHeader
{
    u8 versionSignatureLo;
    u8 versionSignatureHi;
    u8 vsScreenHealthFlagsLo;
    u8 vsScreenHealthFlagsHi;
};

struct Illusion
{
	u8 partyId;
	u8 on;
	u8 set;
	u8 broken;
	struct Pokemon *mon;
};

struct MoveInfo
{
	u8 windowId; // Move's description window Id
	u8 arrowTaskId; // Submenu arrow task id
	u16 submenuState; // Determine which string will be show on the submenu info
};

struct MoveEffect
{
	u8 moveEffectByte;
	bool8 affectsUser:1;
	bool8 certain:1;
	bool8 unused:6;
};

struct QueuedEffect
{
	u8 id;
	u8 battler;
	bool8 done;
};

struct BattleStruct
{
    /*0x000*/ u8 turnEffectsTracker;
    /*0x001*/ u8 queuedEffectsCount;
	/*0x002*/ u8 atkCancellerTracker;
	/*0x003*/ u8 faintedActionsState;
	/*0x004*/ u8 focusPunchBattlerId;
	/*0x005*/ u8 turnEffectsBattlerId;
	/*0x006*/ u8 faintedActionsBattlerId;
	/*0x007*/ u8 switchInAbilitiesCounter;
    /*0x008*/ u16 wrappedMove[MAX_BATTLERS_COUNT];
	/*0x010*/ u8 wrappedBy[MAX_BATTLERS_COUNT];
    /*0x014*/ u8 moveTarget[MAX_BATTLERS_COUNT];
	/*0x018*/ u8 expGetterMonId;
    /*0x019*/ u8 wildVictorySong:1;
	/*0x019*/ u8 firstCritcalHitTakenMsgState:2;
	/*0x019*/ u8 firstMonSendOutMsgDone:1;
	/*0x019*/ u8 firstMonDownMsgDone:1;
	/*0x019*/ u8 lastMonSendOutMsgDone:1;
	/*0x019*/ u8 lastMonLowHPMsgDone:1;
	/*0x019*/ u8 megaEvolutionMsgDone:1;
	/*0x01A*/ u8 dynamicMoveType;
	/*0x01B*/ u8 battlerPreventingSwitchout;
    /*0x01C*/ u8 moneyMultiplier;
    /*0x01D*/ u8 savedTurnActionNumber;
	/*0x01E*/ u8 filler3; // Unused
	/*0x01F*/ u8 runTries;
	/*0x020*/ u16 expValue;
	/*0x022*/ u8 sentInPokes;
	/*0x023*/ u8 expGetterBattlerId;
	/*0x024*/ u8 battlerPartyIndexes[MAX_BATTLERS_COUNT];
	/*0x028*/ u8 monToSwitchIntoId[MAX_BATTLERS_COUNT];
	/*0x02C*/ u8 battlerPartyOrders[MAX_BATTLERS_COUNT][3];
	/*0x038*/ u8 selectionScriptFinished:4; // as flag using gBitTable
    /*0x038*/ u8 switchInAbilityPostponed:4; // for switch in abilities, as flag using gBitTable
	/*0x039*/ u8 appearedInBattle:6; // for Burmy form change, as flag using gBitTable
	/*0x039*/ u8 spriteIgnore0Hp:1; // for Illusion
	/*0x039*/ u8 overworldWeatherDone:1;
	/*0x03A*/ u8 stringMoveType;
	/*0x03B*/ u8 absentBattlerFlags:4;
	/*0x03B*/ u8 zMoveMsgDone:1;
	/*0x03B*/ u8 dynamaxMsgDone:1;
	/*0x03B*/ u8 firstSuperEffectiveHitTakenMsgState:2;
    /*0x03C*/ u8 caughtMonNick[POKEMON_NAME_LENGTH + 1];
    /*0x047*/ u8 safariGoNearCounter;
    /*0x048*/ u8 safariPkblThrowCounter;
    /*0x049*/ u8 safariEscapeFactor;
    /*0x04A*/ u8 safariCatchFactor;
    /*0x04B*/ u8 linkBattleVsSpriteId_V;
    /*0x04C*/ u8 linkBattleVsSpriteId_S;
	/*0x04D*/ u8 chosenMovePositions[MAX_BATTLERS_COUNT];
    /*0x051*/ u8 stateIdAfterSelScript[MAX_BATTLERS_COUNT];
    /*0x055*/ u8 playerPartyIdx;
	/*0x056*/ u16 hpOnSwitchout[2];
	/*0x05A*/ u8 hpScale;
	/*0x05B*/ u8 multiplayerId;
    /*0x05C*/ u16 abilityPreventingSwitchout;
	/*0x05E*/ u8 simulatedInputState[4];  // used by Oak/Old Man/Pokedude controllers
	/*0x062*/ u8 filler; // Unused
	/*0x063*/ u8 soulHeartBattlerId;
	/*0x064*/ u16 savedBattleTypeFlags;
	/*0x066*/ u8 synchronizeMoveEffect;
	/*0x067*/ u8 switchInItemsCounter;
	/*0x068*/ u8 givenExpMons;
	/*0x069*/ u8 payDayLevels[MAX_BATTLERS_COUNT / 2]; // To store player mon's levels when using pay day, 0 = left, 1 = right
	/*0x06B*/ u8 usedReviveItemBattler; // for revive battle usage, as flag using gBitTable
	/*0x06C*/ u16 lastTakenMove[MAX_BATTLERS_COUNT];
	/*0x074*/ u16 lastTakenMoveFrom[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT];
	/*0x094*/ u16 choicedMove[MAX_BATTLERS_COUNT];
	/*0x09C*/ u16 changedItems[MAX_BATTLERS_COUNT];
	/*0x0A4*/ u8 magnitudeBasePower;
	/*0x0A5*/ u8 presentBasePower;
	/*0x0A6*/ u8 weatherIconSpriteId;
	/*0x0A7*/ u8 field_DA; // battle tower related
	/*0x0A8*/ void (*savedCallback)(void);
	/*0x0AC*/ u16 usedHeldItems[PARTY_SIZE][B_SIDE_COUNT];
	/*0x0C4*/ u16 hpBefore[MAX_BATTLERS_COUNT]; // hp before use move, for Berserk and Emergency Exit
	/*0x0CC*/ u8 zeroToHeroActivated[B_SIDE_COUNT]; // as flag using gBitTable
	/*0x0CE*/ u8 allowedToChangeFormInWeather[B_SIDE_COUNT]; // for Ice Face, as flag using gBitTable
	/*0x0D0*/ u16 chosenItem[MAX_BATTLERS_COUNT]; // AI related
	/*0x0D8*/ const u8 *trainerSlideMsg;
	/*0x0DC*/ u8 AI_monToSwitchIntoId[2]; // AI related
	/*0x0DE*/ u8 itemPartyIndex[MAX_BATTLERS_COUNT]; // for item use
	/*0x0E2*/ u8 targetsDone[MAX_BATTLERS_COUNT]; // for moves hiting multiples pokemon, as flag using gBitTable
	/*0x0E6*/ u8 battleTurnCounter;
	/*0x0E7*/ u8 terastalMsgDone:1;
	/*0x0E7*/ u8 throwingPokeBall:1;
	/*0x0E7*/ u8 turnSideTracker:3;
	/*0x0E7*/ u8 filler2:3; // Unused
	/*0x0E8*/ u8 intrepidSwordActivated[B_SIDE_COUNT]; // as flag using gBitTable
	/*0x0EA*/ u8 dauntlessShieldActivated[B_SIDE_COUNT]; // as flag using gBitTable
	/*0x0EC*/ u16 abilityOverride[MAX_BATTLERS_COUNT]; // Used to override the ability on pop up by this value
	/*0x0F4*/ u8 faintCounter[B_SIDE_COUNT]; // for Supreme Overlord, caps at 100 faints per side
	/*0x0F6*/ u8 supremeOverlordBoosts[MAX_BATTLERS_COUNT];
	/*0x0FA*/ u8 pickupStack[MAX_BATTLERS_COUNT]; // for Pickup gen5 effect
	/*0x0FE*/ struct QueuedEffect queuedEffectsList[B_QUEUED_COUNT + 1];
	          struct MoveEffect moveEffect;
	          struct Illusion illusion[MAX_BATTLERS_COUNT];
	          struct MoveInfo moveInfo;
    union {
        struct LinkPartnerHeader linkPartnerHeader;
        struct MultiBattlePokemonTx multiBattleMons[3];
    } multiBuffer;
};

extern struct BattleStruct *gBattleStruct;

#define APPLY_STAT_MOD(var, mon, stat, statIndex)                                   \
{                                                                                   \
    (var) = (stat) * (gStatStageRatios)[(mon)->statStages[(statIndex)]][0];         \
    (var) /= (gStatStageRatios)[(mon)->statStages[(statIndex)]][1];                 \
}

#define IS_MOVE_PHYSICAL(move)((GetMoveSplit(move) == SPLIT_PHYSICAL))
#define IS_MOVE_SPECIAL(move)((GetMoveSplit(move) == SPLIT_SPECIAL))
#define IS_MOVE_STATUS(move)((GetMoveSplit(move) == SPLIT_STATUS))

#define BATTLER_DAMAGED(battlerId) ((gSpecialStatuses[battlerId].physicalDmg != 0 || gSpecialStatuses[battlerId].specialDmg != 0))
#define BATTLER_TURN_DAMAGED(battlerId) ((!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(battlerId) \
                                         && !gProtectStructs[gBattlerAttacker].confusionSelfDmg))

#define IS_BATTLER_OF_TYPE(battlerId, type)((gBattleMons[battlerId].type1 == type || gBattleMons[battlerId].type2 == type || (gBattleMons[battlerId].type3 != TYPE_MYSTERY && gBattleMons[battlerId].type3 == type)))

#define GET_STAT_BUFF_ID(n)((n & 0xF))              // first four bits 0x1, 0x2, 0x4, 0x8
#define GET_STAT_BUFF_VALUE2(n)((n & 0xF0))
#define GET_STAT_BUFF_VALUE(n)(((n >> 4) & 7))      // 0x10, 0x20, 0x40
#define STAT_BUFF_NEGATIVE 0x80                     // 0x80, the sign bit
#define SET_STAT_BUFF_VALUE(n)(((s8)(((s8)(n) << 4)) & 0xF0))
#define SET_STATCHANGER(statId, stage, goesDown)(gBattleScripting.statChanger = (statId) + (stage << 4) + (goesDown << 7))

#define HANDLE_POWER_TRICK_SWAP(battlerId)                                         \
{                                                                                  \
	u16 temp;                                                                      \
	                                                                               \
	if (gStatuses3[battlerId] & STATUS3_POWER_TRICK)                               \
		SWAP(gBattleMons[battlerId].attack, gBattleMons[battlerId].defense, temp); \
}

#define ARE_BATTLERS_OF_SAME_GENDER(battler1, battler2) ((GetBattlerGender(battler1) == GetBattlerGender(battler2)))

#define BATTLER_MAX_HP(battlerId) ((gBattleMons[battlerId].hp == gBattleMons[battlerId].maxHP))

struct BattleScripting
{
    /*0x00*/ s32 painSplitHp;
    /*0x04*/ s32 bideDmg;
	/*0x08*/ s32 savedDmg;
    /*0x0C*/ u8 multihitString[6]; // for the "Hit X time(s)!" string
    /*0x12*/ u8 dmgMultiplier;
    /*0x27*/ bool8 bypassAbilityPopUp; // don't show ability pop up
    /*0x14*/ u8 animArg1;
    /*0x15*/ u8 animArg2;
    /*0x16*/ u8 tripleKickPower;
    /*0x17*/ u8 atk49_state;
    /*0x18*/ u8 battlerWithAbility;
    /*0x19*/ u8 battler;
    /*0x1A*/ u8 animTurn;
    /*0x1B*/ u8 animTargetsHit;
    /*0x1C*/ u8 statChanger;
    /*0x1D*/ bool8 statAnimPlayed;
    /*0x1E*/ u8 atk23_state; // give exp state
    /*0x1F*/ u8 battleStyle;
    /*0x20*/ u8 atk6C_state; // lvl up box state
    /*0x21*/ u8 learnMoveState;
    /*0x22*/ u8 reshowMainState; // for reshow battle screen after menu
    /*0x23*/ u8 reshowHelperState; // for reshow battle screen after menu
    /*0x24*/ u8 field_23; // does something with hp calc
    /*0x25*/ bool8 illusionNickHack;
	/*0x26*/ bool8 fixedAbilityPopUp;
	/*0x27*/ u8 savedBattler; // Multiuse
};

enum
{
    BACK_PIC_RED,
    BACK_PIC_LEAF,
    BACK_PIC_RS_BRENDAN,
    BACK_PIC_RS_MAY,
    BACK_PIC_POKEDUDE,
    BACK_PIC_OLDMAN
};

struct BattleSpriteInfo
{
    /*0x0*/ u16 invisible:1;
            u16 lowHpSong:1;
            u16 behindSubstitute:1;
            u16 flag_x8:1;
            u16 hpNumbersNoBars:1;
			u16 unused:3;
    /*0x2*/ u16 transformSpecies;
};

struct BattleAnimationInfo
{
    /*0x0*/ u16 animArg; // to fill up later
    /*0x2*/ u8 ballThrowCaseId;
	/*0x3*/ u8 healthboxSlideInStarted:1;
			u8 battlerSpriteVisibility:1;
			u8 unused:6;
	/*0x4*/ s16 ballShakeThing;
    /*0x6*/ u8 particleCounter;
};

struct BattleHealthboxInfo
{
    /*0x0*/ u8 partyStatusSummaryShown:1;
			u8 healthboxIsBouncing:1;
			u8 battlerIsBouncing:1;
			u8 ballAnimActive:1;
			u8 statusAnimActive:1;
			u8 animFromTableActive:1;
			u8 specialAnimActive:1;
			u8 triedShinyMonAnim:1;
    /*0x1*/ u8 finishedShinyMonAnim:1;
			u8 opponentDrawPartyStatusSummaryDelay:5;
			u8 animationState:2;
	/*0x2*/ u8 healthboxBounceSpriteId;
    /*0x3*/ u8 battlerBounceSpriteId;
    /*0x4*/ u8 partyStatusDelayTimer;
    /*0x5*/ u8 matrixNum;
    /*0x6*/ u8 shadowSpriteId;
    /*0x7*/ u8 soundTimer;
    /*0x8*/ u8 introEndDelay;
};

struct BattleBarInfo
{
    u8 healthboxSpriteId;
    s32 maxValue;
    s32 oldValue;
    s32 receivedValue;
    s32 currValue;
};

struct BattleSpriteData
{
    struct BattleSpriteInfo *battlerData;
    struct BattleHealthboxInfo *healthBoxesData;
    struct BattleAnimationInfo *animationData;
    struct BattleBarInfo *battleBars;
};

extern struct BattleSpriteData *gBattleSpritesDataPtr;

#define BATTLE_BUFFER_LINK_SIZE 0x1000

extern u8 *gLinkBattleSendBuffer;
extern u8 *gLinkBattleRecvBuffer;

// Move this somewhere else

#include "sprite.h"

struct MonSpritesGfx
{
    void* firstDecompressed; // ptr to the decompressed sprite of the first pokemon
    void* sprites[MAX_BATTLERS_COUNT];
    struct SpriteTemplate templates[MAX_BATTLERS_COUNT];
    struct SpriteFrameImage images[MAX_BATTLERS_COUNT][4];
    u8 *barFontGfx;
    u16 *multiUseBuffer;
};

struct PokedudeBattlerState
{
    u8 action_idx;
    u8 move_idx;
    u8 timer;
    u8 msg_idx;
    u8 saved_bg0y;
};

extern u16 gBattle_BG0_X;
extern u16 gBattle_BG0_Y;
extern u16 gBattle_BG1_X;
extern u16 gBattle_BG1_Y;
extern u16 gBattle_BG2_X;
extern u16 gBattle_BG2_Y;
extern u16 gBattle_BG3_X;
extern u16 gBattle_BG3_Y;
extern u16 gBattle_WIN0H;
extern u16 gBattle_WIN0V;
extern u16 gBattle_WIN1H;
extern u16 gBattle_WIN1V;
extern struct BattleSpritesGfx *gMonSpritesGfx;
extern u8 gBattleOutcome;
extern u16 gLastUsedItem;
extern u32 gBattleTypeFlags;
extern struct MonSpritesGfx *gMonSpritesGfxPtr;
extern u16 gTrainerBattleOpponent_A;
extern u16 gMoveToLearn;
extern u16 gBattleMovePower;
extern u16 gCurrentMove;
extern u16 gChosenMove;
extern u16 gCalledMove;
extern bool8 gIsCriticalHit;
extern u16 gBattleWeather;
extern u16 gLastUsedAbility;
extern u8 gBattlerInMenuId;
extern u8 gBattlersCount;
extern u16 gBattlerPartyIndexes[MAX_BATTLERS_COUNT];
extern s32 gBattleMoveDamage;
extern u16 gIntroSlideFlags;
extern u32 gTransformedPersonalities[MAX_BATTLERS_COUNT];
extern bool8 gTransformedShinies[MAX_BATTLERS_COUNT];
extern u8 gBattlerPositions[MAX_BATTLERS_COUNT];
extern u8 gHealthboxSpriteIds[MAX_BATTLERS_COUNT];
extern u8 gBattleOutcome;
extern u16 gBattleMonForms[B_SIDE_COUNT][PARTY_SIZE];
extern void (*gBattlerControllerFuncs[MAX_BATTLERS_COUNT])(u8);
extern void (*gBattlerControllerEndFuncs[MAX_BATTLERS_COUNT])(u8);
extern u32 gBattleControllerExecFlags;
extern u8 gBattleBufferA[MAX_BATTLERS_COUNT][0x200];
extern u8 gBattleBufferB[MAX_BATTLERS_COUNT][0x200];
extern u8 gActionSelectionCursor[MAX_BATTLERS_COUNT];
extern void (*gPreBattleCallback1)(void);
extern bool8 gDoingBattleAnim;
extern struct PokedudeBattlerState *gPokedudeBattlerStates[MAX_BATTLERS_COUNT];
extern u8 *gBattleAnimMons_BgTilesBuffer;
extern u8 *gBattleAnimMons_BgTilemapBuffer;
extern void (*gBattleMainFunc)(void);
extern u8 gMoveSelectionCursor[MAX_BATTLERS_COUNT];
extern u8 gBattlerAttacker;
extern u8 gEffectBattler;
extern u8 gMultiHitCounter;
extern struct BattleScripting gBattleScripting;
extern u8 gBattlerFainted;
extern u32 gStatuses3[MAX_BATTLERS_COUNT];
extern u8 gSentPokesToOpponent[2];
extern const u8 *gBattlescriptCurrInstr;
extern const u8 *gSelectionBattleScripts[MAX_BATTLERS_COUNT];
extern u16 gLastMoves[MAX_BATTLERS_COUNT];
extern u8 gLastUsedMovesTypes[MAX_BATTLERS_COUNT];
extern u8 gBattlerByTurnOrder[MAX_BATTLERS_COUNT];
extern u8 gBattleCommunication[BATTLE_COMMUNICATION_ENTRIES_COUNT];
extern u16 gSideStatuses[B_SIDE_COUNT];
extern u32 gHitMarker;
extern u16 gChosenMoveByBattler[MAX_BATTLERS_COUNT];
extern u16 gMoveResultFlags;
extern s32 gTakenDmg[MAX_BATTLERS_COUNT];
extern u8 gTakenDmgByBattler[MAX_BATTLERS_COUNT];
extern u8 gCurrentActionFuncId;
extern u8 gCurrMovePos;
extern u8 gChosenMovePos;
extern u8 gBattleControllerData[MAX_BATTLERS_COUNT];
extern u8 gBattlerStatusSummaryTaskId[MAX_BATTLERS_COUNT];
extern u16 gLastLandedMoves[MAX_BATTLERS_COUNT];
extern u8 gLastHitBy[MAX_BATTLERS_COUNT];
extern u8 gMultiUsePlayerCursor;
extern u8 gNumberOfMovesToChoose;
extern u8 gLastHitByType[MAX_BATTLERS_COUNT];
extern s32 gHpDealt;
extern u16 gPauseCounterBattle;
extern u16 gPaydayMoney;
extern u16 gLockedMoves[MAX_BATTLERS_COUNT];
extern u8 gCurrentTurnActionNumber;
extern u16 gExpShareExp;
extern u8 gLeveledUpInBattle;
extern u16 gLastResultingMoves[MAX_BATTLERS_COUNT];
extern u16 gLastPrintedMoves[MAX_BATTLERS_COUNT];
extern u8 gActionsByTurnOrder[MAX_BATTLERS_COUNT];
extern u8 gChosenActionByBattler[MAX_BATTLERS_COUNT];
extern u8 gBattleTerrain;
extern struct MultiBattlePokemonTx gMultiPartnerParty[3];
extern u16 gRandomTurnNumber;
extern u8 gPartyCriticalHits[PARTY_SIZE];
extern u8 gBattlerTarget;
extern u8 gAbsentBattlerFlags;
extern u32 gFieldStatus;
extern struct FieldTimer gFieldTimers;

static inline u8 GetBattlerPosition(u8 battlerId)
{
    return gBattlerPositions[battlerId];
}

static inline u8 GetBattlerSide(u8 battlerId)
{
    return GetBattlerPosition(battlerId) & BIT_SIDE;
}

#endif // GUARD_BATTLE_H

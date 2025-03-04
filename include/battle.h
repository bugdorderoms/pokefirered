#ifndef GUARD_BATTLE_H
#define GUARD_BATTLE_H

#include <limits.h>
#include "global.h"
#include "constants/battle.h"
#include "battle_util.h"
#include "battle_queued_effects.h"
#include "battle_script_commands.h"
#include "battle_main.h"
#include "battle_ai.h"
#include "battle_util2.h"
#include "battle_bg.h"
#include "constants/battle_script_commands.h"

/*
    Banks are a name given to what could be called a 'battlerId' or 'monControllerId'.
    Each bank has a value consisting of two bits.
    0x1 bit is responsible for the side, 0 = player's side, 1 = opponent's side.
    0x2 bit is responsible for the id of sent out pokemon. 0 means it's the first sent out pokemon, 1 it's the second one. (Triple battle didn't exist at the time yet.)
*/

#define GetBattlerPosition(battlerId) ((gBattlerPositions[battlerId]))
#define GetBattlerSide(battlerId) ((GetBattlerPosition(battlerId) & BIT_SIDE))

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
#define MOVE_TARGET_OPPONENTS         (1 << 1) // Same as Both foes, but with no script re-execution
#define MOVE_TARGET_RANDOM            (1 << 2)
#define MOVE_TARGET_OPPONENTS_FIELD   (1 << 3)
#define MOVE_TARGET_BOTH              (1 << 4)
#define MOVE_TARGET_USER              (1 << 5)
#define MOVE_TARGET_ALLY              (1 << 6)
#define MOVE_TARGET_SELECTED_OPPONENT (1 << 7) // Same as selected, but can't select an ally
#define MOVE_TARGET_USER_OR_ALLY      (MOVE_TARGET_USER | MOVE_TARGET_ALLY)
#define MOVE_TARGET_FOES_AND_ALLY     (MOVE_TARGET_BOTH | MOVE_TARGET_ALLY)
#define MOVE_TARGET_ALL_BATTLERS      (MOVE_TARGET_FOES_AND_ALLY | MOVE_TARGET_USER)

#define MAX_TRAINER_ITEMS 4

#define TRAINER_MON_MALE   1
#define TRAINER_MON_FEMALE 2

#define TRAINER_CHALLENGE_INVERSE_BATTLE    1
#define TRAINER_CHALLENGE_INFINITE_TAILWIND 2

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
			 u8 gender:2; // 0 = based on personality, 1 = male, 2 = female
};

struct Trainer
{
	/*0x00*/ u8 trainerName[12];
	/*0x0C*/ u8 trainerClass;
	/*0x0D*/ u8 trainerPic;
	/*0x0E*/ u8 encounterMusic;
	/*0x0F*/ u8 doubleBattle:1;
			 u8 battleChallenge:7; // 0 means no challenge, max 126 battle challenges
    /*0x10*/ u16 items[MAX_TRAINER_ITEMS];
    /*0x18*/ u32 aiFlags;
	/*0x1C*/ const struct TrainerMon *party;
	/*0x20*/ u8 partySize;
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

// Cleared each time a mon leaves the field, either by switching out or fainting.
struct DisableStruct
{
    /*0x00*/ u32 transformedMonPersonality;
	/*0x04*/ u32 transformedMonShynies:1;
			 u32 isFirstTurn:2;
			 u32 tauntTimer:3;
			 u32 canProteanActivate:1;
	         u32 slowStartTimer:3;
			 u32 stockpileCounter:2;
			 u32 stockpiledDef:2;
			 u32 stockpiledSpDef:2;
			 u32 furyCutterCounter:2;
             u32 rolloutTimer:3;
             u32 destinyBondCounter:2;
	         u32 enduredHit:1;
			 u32 perishSongTimer:2;
			 u32 truantCounter:1;
			 u32 mimickedMoves:4;
			 u32 imposterActivated:1; // Only activate when switched in, not when gained
	/*0x08*/ u16 flashFireBoost:1;
			 u16 healBlockTimer:3;
			 u16 rechargeTimer:2;
			 u16 embargoTimer:3;
			 u16 disableTimer:3;
			 u16 encoreTimer:2;
	         u16 encoredMovePos:2;
	/*0x0A*/ u16 disabledMove;
	/*0x0C*/ u16 encoredMove;
	/*0x0E*/ u8 protectUses;
	/*0x0F*/ u8 substituteHP;
	/*0x10*/ u8 infatuatedWith;
    /*0x11*/ u8 battlerWithSureHit;
    /*0x12*/ u8 battlerPreventingEscape;
	/*0x13*/ u8 leechSeedBattler;
	/*0x14*/ u8 unburdenBoost:1;
			 u8 startedNeutralizingGas:1;
			 u8 supremeOverlordBoost:3;
			 u8 wrapTurns:3;
	/*0x15*/ u8 wrappedBy;
	/*0x16*/ u16 wrappedMove;
	/*0x18*/ u8 roostActive:1;
			 u8 usedMoveIndices:4; // As flag using gBitTable
			 u8 magnetRiseTimer:3;
	/*0x19*/ u8 commanderActivated:1;
			 u8 unused:7;
};

extern struct DisableStruct gDisableStructs[MAX_BATTLERS_COUNT];

// Fully Cleared each turn after end turn effects are done. A few things are cleared before end turn effects. Or when the battler faints.
struct ProtectStruct
{
	/*0x00*/ u32 physicalDmg;
    /*0x04*/ u32 specialDmg;
    /*0x08*/ u8 physicalBattlerId;
    /*0x09*/ u8 specialBattlerId;
    /*0x0A*/ u8 protected:1;
             u8 endured:1;
             u8 noValidMoves:1;
             u8 usedHealBlockedMove:1;
             u8 bounceMove:1;
             u8 stealMove:1;
             u8 notFirstStrike:1;
             u8 prlzImmobility:1;
    /*0x0B*/ u8 confusionSelfDmg:1;
			 u8 targetNotAffected:1;
			 u8 chargingTurn:1;
			 u8 fleeFlag:2; // for RunAway and Smoke Ball
			 u8 usedImprisonedMove:1;
			 u8 loveImmobility:1;
			 u8 usedDisabledMove:1;
    /*0x0C*/ u8 usedTauntedMove:1;
			 u8 flinchImmobility:1;
			 u8 usesBouncedMove:1;
			 u8 usedGravityBannedMove:1;
			 u8 unused:4;
	/*0x0D*/ u8 helpingHandUses;
};

extern struct ProtectStruct gProtectStructs[MAX_BATTLERS_COUNT];

// Cleared each time a battler does their action, at the start of HandleAction_ActionFinished.
struct SpecialStatus
{
	/*0x00*/ s32 dmg;
    /*0x04*/ s32 physicalDmg;
    /*0x08*/ s32 specialDmg;
	/*0x0C*/ u8 physicalBattlerId;
    /*0x0D*/ u8 specialBattlerId;
	/*0x0E*/ u8 removedNeutralizingGas:1;
			 u8 abilityRedirected:1;
			 u8 emergencyExited:1;
			 u8 restoredBattlerSprite:1;
			 u8 removedWeatherChangeAbility:1;
			 u8 faintedHasReplacement:1;
			 u8 focusBanded:1;
			 u8 terrainAbilityDone:1;
	/*0x0F*/ u8 switchInAbilityDone:1;
			 u8 weatherAbilityDone:1;
			 u8 multiHitOn:1;
			 u8 parentalBondState:2;
			 u8 hitBySuperEffective:1; // For sos battles
			 u8 unused:2;
	/*0x10*/ u8 changedStatsBattlerId;
};

extern struct SpecialStatus gSpecialStatuses[MAX_BATTLERS_COUNT];

struct SideTimer
{
    /*0x00*/ u8 reflectTimer:4;
			 u8 lightscreenTimer:4;
	/*0x01*/ u8 reflectBattlerId;
	/*0x02*/ u8 lightscreenBattlerId;
	/*0x03*/ u8 mistTimer:3;
			 u8 safeguardTimer:3;
			 u8 spikesAmount:2;
	/*0x04*/ u8 mistBattlerId;
	/*0x05*/ u8 safeguardBattlerId;
	/*0x06*/ u8 luckyChantTimer:3;
	         u8 tailwindTimer:3;
			 u8 toxicSpikesAmount:2;
	/*0x07*/ u8 luckyChantBattlerId;
	/*0x08*/ u8 tailwindBattlerId;
	/*0x09*/ u8 followmeSet:1;
			 u8 unused:7;
	/*0x0A*/ u8 followmeTarget;
};

extern struct SideTimer gSideTimers[B_SIDE_COUNT];

struct FieldTimer
{
	/*0x00*/ u16 waterSportTimer:3;
			 u16 mudSportTimer:3;
			 u16 gravityTimer:3;
			 u16 trickRoomTimer:3;
			 u16 unused:4;
};

extern struct BattlePokemon gBattleMons[MAX_BATTLERS_COUNT];

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
    struct BattleScriptsStack *battleScriptsStack;
    struct BattleCallbacksStack *battleCallbackStack;
    struct StatsArray *beforeLvlUp;
	struct AiData *aiData;
	struct AiThinking *aiThinking;
};

extern struct BattleResources *gBattleResources;

struct LinkPartnerHeader
{
    u8 versionSignatureLo;
    u8 versionSignatureHi;
    u8 vsScreenHealthFlagsLo;
    u8 vsScreenHealthFlagsHi;
};

struct MoveEffect
{
	u8 moveEffectByte;
	bool8 affectsUser:1;
	bool8 certain:1;
	bool8 continueAdditionalEffectsLoop:1; // Used by setadditionaleffects
	u8 additionalEffectsCounter:2; // Used by setadditionaleffects
	u8 unused:3;
};

struct StatChange
{
	u8 statId;
	s8 buff;
	u8 flags;
	u8 result;
	const u8* str; // Jump str if fail
	u8 mirrorArmorState:2; // 1 - reflected, 2 - pop up displayed
	bool8 maxOut:1;
	bool8 statAnimPlayed:1;
	u8 unused:4;
};

struct QueuedStatBoost
{
	u8 stats; // bitfield for each battle stat that is set if the stat changes
	s8 statChanges[NUM_BATTLE_STATS];
};

struct BattleItemEffect
{
	u16 savedItems[B_SIDE_COUNT][PARTY_SIZE]; // Items of the party members, they are restored at the end of trainer battles
};

struct SosCall
{
	u16 chainLength:5;
	u16 nPerfectIvs:3;
	u16 hiddenAbilityChance:4;
	u16 shinyRolls:4;
};

struct QueuedEffect
{
	u8 id:7; // Max 127 queued effects.
	u8 done:1;
};

struct BattlerState
{
	/*0x00*/ u8 moveTarget;
	/*0x01*/ u8 stateIdAfterSelScript;
	/*0x02*/ u8 partyIndex;
	/*0x03*/ u8 chosenMovePosition;
	/*0x04*/ u8 monToSwitchIntoId;
	/*0x05*/ u8 itemPartyIndex; // For item use
	/*0x06*/ u8 targetsDone; // For moves hiting multiples pokemon, as flag using gBitTable
	/*0x07*/ u8 payDayLevel; // To store player mon's levels when using pay day, this is unused for opponents
	/*0x08*/ u8 aiMoveOrAction;
	/*0x09*/ u8 aiChosenTarget;
	/*0x0A*/ u16 lastTakenMove;
	/*0x0C*/ u16 lastTakenMoveFrom[MAX_BATTLERS_COUNT];
	/*0x14*/ u16 choicedMove;
	/*0x16*/ u16 hpBefore; // Hp before use move, for Berserk and Emergency Exit
	/*0x18*/ u16 abilityOverride; // Used to override the ability on pop up by this value
	/*0x1A*/ u16 chosenItem; // AI related
	/*0x1C*/ u8 selectionScriptFinished:1;
			 u8 usedReviveItem:1; // For revive battle usage
			 u8 storedHealingWish:1;
			 u8 storedLunarDance:1;
			 u8 wishCounter:2;
			 u8 futureSightCounter:2;
	/*0x1D*/ u8 wishMonId;
	/*0x1E*/ u16 futureSightMove;
	/*0x20*/ u8 futureSightAttacker;
	/*0x21*/ u8 savedSpriteId; // Used for trainer slides
	/*0x22*/ u16 chosenMove;
	/*0x24*/ u16 lockedMove;
	/*0x26*/ u8 queuedEffectsCount:7;
			 u8 unused:1;
	/*0x27*/ u8 moveSelectionCursor;
	/*0x28*/ s32 bideTakenDamage;
	/*0x2C*/ u16 lastPrintedMove;
	/*0x2E*/ u16 lastMove;
	/*0x30*/ u16 lastLandedMove;
	/*0x32*/ u16 lastResultingMove;
	/*0x34*/ u8 lastHitMoveType;
	/*0x35*/ u8 chosenAction;
	/*0x36*/ u8 lastHitBattler;
	/*0x37*/ u8 bideTakenDamageBattler;
	/*0x38*/ u8 lastUsedMoveType;
			 struct QueuedEffect queuedEffectsList[B_BATTLER_QUEUED_COUNT + 1];
			 struct {
				 u8 partyId;
				 bool8 on;
				 bool8 set;
				 bool8 broken;
				 struct Pokemon *mon;
			 } illusion;
};

struct PartyState
{
	/*0x00*/ u16 usedHeldItem;
	/*0x02*/ u8 intrepidSwordActivated:1; // Each party member can activate only once
		     u8 dauntlessShieldActivated:1; // Each party member can activate only once
		     u8 zeroToHeroActivated:1;
		     u8 battleBondActivated:1;
		     u8 appearedInBattle:1; // For Burmy form change
		     u8 allowedToChangeFormInWeather:1; // For Ice Face
		     u8 unused:2;
};

struct SideState
{
	/*0x00*/ u16 hpOnSwitchout;
	/*0x02*/ u16 faintCounter:7; // Caps at 100 faints per side
			 u16 queuedEffectsCount:7;
			 u16 unused:2;
			 struct QueuedEffect queuedEffectsList[B_SIDE_QUEUED_COUNT + 1];
			 struct PartyState party[PARTY_SIZE];
};

// Cleared at the beginning of the battle. Fields need to be cleared when needed manually otherwise.
struct BattleStruct
{
	/*0x000*/ u8 pickupStack[MAX_BATTLERS_COUNT]; // For Pickup gen5 effect
	/*0x004*/ u8 focusPunchBattlerId;
	/*0x005*/ u8 turnEffectsBattlerId;
	/*0x006*/ u8 faintedActionsBattlerId;
	/*0x007*/ u8 switchInByTurnOrderCounter;
	/*0x008*/ u8 expGetterMonId;
	/*0x009*/ u8 wildVictorySong:1;
	/*0x009*/ u8 firstCritcalHitTakenMsgState:2;
	/*0x009*/ u8 firstMonSendOutMsgDone:1;
	/*0x009*/ u8 firstMonDownMsgDone:1;
	/*0x009*/ u8 lastMonSendOutMsgDone:1;
	/*0x009*/ u8 lastMonLowHPMsgDone:1;
	/*0x009*/ u8 megaEvolutionMsgDone:1;
	/*0x00A*/ u8 dynamicMoveType;
	/*0x00B*/ u8 battlerPreventingSwitchout;
	/*0x00C*/ u8 moneyMultiplier:1;
	/*0x00C*/ u8 terastalMsgDone:1;
	/*0x00C*/ u8 throwingPokeBall:1;
	/*0x00C*/ u8 turnSideTracker:3;
	/*0x00C*/ u8 meFirstBoost:1;
	/*0x00C*/ u8 spriteIgnore0Hp:1; // For Illusion
	/*0x00D*/ u8 savedTurnActionNumber;
	/*0x00E*/ u8 runTries;
	/*0x00F*/ u8 sentInPokes;
	/*0x010*/ u16 expValue;
	/*0x012*/ u8 expGetterBattlerId;
	/*0x013*/ u8 battleTurnCounter;
	/*0x014*/ u8 weatherDuration:4;
	/*0x014*/ u8 zMoveMsgDone:1;
	/*0x014*/ u8 dynamaxMsgDone:1;
	/*0x014*/ u8 firstSuperEffectiveHitTakenMsgState:2;
	/*0x015*/ u8 switchInAbilityPostponed; // For switch in abilities, as flag using gBitTable
	/*0x016*/ u8 safariEscapeFactor;
    /*0x017*/ u8 safariCatchFactor;
	/*0x018*/ u8 safariGoNearCounter;
    /*0x019*/ u8 safariPkblThrowCounter;
	/*0x01A*/ u8 caughtMonNick[POKEMON_NAME_LENGTH + 1];
	/*0x027*/ u8 battlerPartyOrders[MAX_BATTLERS_COUNT][3];
	/*0x033*/ u8 playerPartyIdx;
	/*0x034*/ u8 hpScale;
	/*0x035*/ u8 multiplayerId;
    /*0x036*/ u8 soulHeartBattlerId;
	/*0x037*/ u8 simulatedInputState[4]; // Used by Oak/Old Man/Pokedude controllers
	/*0x03B*/ u8 synchronizeMoveEffect;
	/*0x03C*/ u8 firstTurnEventsState;
	/*0x03D*/ u8 givenExpMons;
	/*0x03E*/ u8 magnitudeBasePower;
	/*0x03F*/ u8 presentBasePower;
	/*0x040*/ u16 savedBattleTypeFlags;
	/*0x042*/ u16 poisonPuppeteerConfusion:1;
	/*0x042*/ u16 strongWindsMessageState:2;
	/*0x042*/ u16 pursuitSwitchDmg:1;
	/*0x042*/ u16 hasFetchedBall:1; // For Ball Fetch
	/*0x042*/ u16 attackAnimPlayed:1; // For Dancer
	/*0x042*/ u16 unused:2; // unused
	/*0x042*/ u16 savedAttackerStackCount:4;
	/*0x042*/ u16 savedTargetStackCount:4;
	/*0x044*/ u8 savedAttackerStack[10];
	/*0x054*/ u8 savedTargetStack[10];
	/*0x064*/ u8 weatherIconSpriteId;
	/*0x065*/ u8 field_DA; // battle tower related
	/*0x066*/ u8 AI_monToSwitchIntoId[2]; // AI related
	/*0x068*/ void (*savedCallback)(void);
	/*0x06C*/ const u8 *trainerSlideMsg;
	/*0x070*/ u8 turnEffectsTracker;
	/*0x071*/ u8 atkCancellerTracker;
	/*0x072*/ u8 faintedActionsState;
	/*0x073*/ u8 absentBattlerFlags;
	/*0x074*/ u8 linkBattleVsSpriteId_V;
    /*0x075*/ u8 linkBattleVsSpriteId_S;
	/*0x076*/ u16 lastFailedBallThrow; // For Ball Fetch
	/*0x078*/ u8 battleChallenge;
	/*0x079*/ struct {
				  u8 calls:5;
				  u8 usedAdrenalineOrb:1;
				  u8 lastCallFailed:1;
				  u8 triedToCallAlly:1;
				  u8 lastCallBattler;
				  u8 totemBattlerId; // If the Totem slot was swaped, e.g by Ally Switch
			  } sos;
			  struct {
				  u8 triggerSpriteId;
				  u8 windowIds[2]; // 0 - move's name, 1 - move's desc
				  u8 arrowTaskId; // Submenu arrow task id
				  u16 submenuState; // Determine which string will be show on the submenu info
			  } moveInfo;
			  struct Dancer
			  {
				  bool8 inProgress;
			      u8 battlersLoopCounter;
				  u8 savedAttacker;
				  u8 savedTarget;
			      u8 turnOrder[MAX_BATTLERS_COUNT];
			  } dancer;
			  struct BattlerState battlers[MAX_BATTLERS_COUNT];
			  struct SideState sides[B_SIDE_COUNT];
	          struct MoveEffect moveEffect;
			  struct MoveEffect moveEffect2;
			  struct StatChange statChange;
			  struct BattleItemEffect itemEffects;
    union {
        struct LinkPartnerHeader linkPartnerHeader;
        struct MultiBattlePokemonTx multiBattleMons[PARTY_SIZE / 2];
    } multiBuffer;
};

extern struct BattleStruct *gBattleStruct;

#define APPLY_STAT_MOD(var, mon, stat, statIndex)                           \
{                                                                           \
    (var) = (stat) * (gStatStageRatios)[(mon)->statStages[(statIndex)]][0]; \
    (var) /= (gStatStageRatios)[(mon)->statStages[(statIndex)]][1];         \
}

#define IS_MOVE_PHYSICAL(move)((GetMoveSplit(move) == SPLIT_PHYSICAL))
#define IS_MOVE_SPECIAL(move)((GetMoveSplit(move) == SPLIT_SPECIAL))
#define IS_MOVE_STATUS(move)((GetMoveSplit(move) == SPLIT_STATUS))

#define BATTLER_DAMAGED(battlerId) ((gSpecialStatuses[battlerId].physicalDmg != 0 || gSpecialStatuses[battlerId].specialDmg != 0))
#define BATTLER_TURN_DAMAGED(battlerId) ((!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(battlerId) && !gProtectStructs[gBattlerAttacker].confusionSelfDmg))

#define HANDLE_POWER_TRICK_SWAP(battlerId)                                         \
{                                                                                  \
	u16 temp;                                                                      \
	                                                                               \
	if (gStatuses3[battlerId] & STATUS3_POWER_TRICK)                               \
		SWAP(gBattleMons[battlerId].attack, gBattleMons[battlerId].defense, temp); \
}

#define BATTLER_MAX_HP(battlerId) ((gBattleMons[battlerId].hp == gBattleMons[battlerId].maxHP))

/* Checks if 'battlerId' is any of the types.
 * Passing multiple types is more efficient than calling this multiple
 * times with one type because it shares the 'GetBattlerTypes' result. */
 
#define IS_BATTLER_ANY_TYPE_HELPER(type) (types[0] == type) || (types[1] == type) || (types[2] == type) ||

#define IsBattlerAnyType(battlerId, ...)                                        \
	({                                                                          \
        u8 types[3];                                                            \
        GetBattlerTypes(battlerId, types);                                      \
        RECURSIVELY(R_FOR_EACH(IS_BATTLER_ANY_TYPE_HELPER, __VA_ARGS__)) FALSE; \
    })

#define IsBattlerOfType IsBattlerAnyType

#define IsBattlerTypeless(battlerId)                                                      \
    ({                                                                                    \
        u8 types[3];                                                                      \
        GetBattlerTypes(battlerId, types);                                                \
        types[0] == TYPE_MYSTERY && types[1] == TYPE_MYSTERY && types[2] == TYPE_MYSTERY; \
    })

// Used to exclude moves learned temporarily by Transform or Mimic
#define MOVE_IS_PERMANENT(battler, moveSlot) (!(gBattleMons[battler].status2 & STATUS2_TRANSFORMED) && !(gDisableStructs[battler].mimickedMoves & gBitTable[moveSlot]))

struct BattleScripting
{
    /*0x00*/ s32 painSplitHp;
    /*0x04*/ s32 bideDmg;
	/*0x08*/ s32 savedDmg;
    /*0x0C*/ u8 multihitString[6]; // for the "Hit X time(s)!" string
    /*0x12*/ u8 atk48_state;
    /*0x27*/ bool8 bypassAbilityPopUp; // don't show ability pop up
    /*0x14*/ u8 animArg1;
    /*0x15*/ u8 animArg2;
    /*0x16*/ u8 tripleKickPower;
    /*0x17*/ u8 atk49_state;
    /*0x18*/ u8 battlerWithAbility; // To save the battle that has an side protect ability
    /*0x19*/ u8 battler;
    /*0x1A*/ u8 animTurn;
    /*0x1B*/ u8 animTargetsHit;
    /*0x1C*/ bool8 fixedAbilityPopUp;
    /*0x1D*/ bool8 illusionNickHack;
    /*0x1E*/ u8 atk23_state; // give exp state
    /*0x1F*/ u8 battleStyle;
    /*0x20*/ u8 atk5C_state; // lvl up box state
    /*0x21*/ u8 learnMoveState;
    /*0x22*/ u8 reshowMainState; // for reshow battle screen after menu
    /*0x23*/ u8 reshowHelperState; // for reshow battle screen after menu
    /*0x24*/ u8 field_23; // does something with hp calc
	/*0x25*/ u8 switchinEffectState;
	/*0x26*/ bool8 expOnCatch;
};

struct BattleSpriteInfo
{
    /*0x0*/ u16 transformSpecies;
	/*0x2*/ u16 formChangeSpecies;
	/*0x4*/ u8 invisible:1;
            u8 lowHpSong:1;
            u8 behindSubstitute:1;
            u8 flag_x8:1;
            u8 hpNumbersNoBars:1;
			u8 unused:3;
};

struct BattleAnimationInfo
{
    /*0x0*/ u16 animArg; // to fill up later
	/*0x2*/ s16 ballShakeThing;
	/*0x4*/ u8 healthboxSlideInStarted:1;
			u8 battlerSpriteVisibility:1;
			u8 ballThrowCaseId:3;
			u8 isCriticalCapture:1;
			u8 criticalCaptureSuccess:1;
			u8 unused:1;
    /*0x5*/ u8 particleCounter;
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
    /*0x6*/ u8 shadowSpriteIdPrimary;
	/*0x7*/ u8 shadowSpriteIdSecondary;
    /*0x8*/ u8 soundTimer;
    /*0x9*/ u8 introEndDelay;
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
extern u8 gBattlerInMenuId;
extern u8 gBattlersCount;
extern u16 gBattlerPartyIndexes[MAX_BATTLERS_COUNT];
extern s32 gBattleMoveDamage;
extern u16 gIntroSlideFlags;
extern u32 gTransformedPersonalities[MAX_BATTLERS_COUNT];
extern bool8 gTransformedShinies[MAX_BATTLERS_COUNT];
extern u8 gBattlerPositions[MAX_BATTLERS_COUNT];
extern u8 gHealthboxSpriteIds[MAX_BATTLERS_COUNT];
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
extern u8 gBattlerAttacker;
extern u8 gEffectBattler;
extern u8 gMultiHitCounter;
extern struct BattleScripting gBattleScripting;
extern u8 gBattlerFainted;
extern u32 gStatuses3[MAX_BATTLERS_COUNT];
extern u8 gSentPokesToOpponent[2];
extern const u8 *gBattlescriptCurrInstr;
extern const u8 *gSelectionBattleScripts[MAX_BATTLERS_COUNT];
extern u16 gLastUsedMove;
extern u8 gBattlerByTurnOrder[MAX_BATTLERS_COUNT];
extern u8 gBattleCommunication[BATTLE_COMMUNICATION_ENTRIES_COUNT];
extern u16 gSideStatuses[B_SIDE_COUNT];
extern u32 gHitMarker;
extern u16 gMoveResultFlags;
extern u8 gCurrentActionFuncId;
extern u8 gCurrMovePos;
extern u8 gChosenMovePos;
extern u8 gBattleControllerData[MAX_BATTLERS_COUNT];
extern u8 gBattlerStatusSummaryTaskId[MAX_BATTLERS_COUNT];
extern u8 gMultiUsePlayerCursor;
extern u8 gNumberOfMovesToChoose;
extern s32 gHpDealt;
extern u16 gPauseCounterBattle;
extern u16 gPaydayMoney;
extern u8 gCurrentTurnActionNumber;
extern u8 gCurrentTurnActionBattlerId;
extern u16 gExpShareExp;
extern u8 gLeveledUpInBattle;
extern u8 gActionsByTurnOrder[MAX_BATTLERS_COUNT];
extern u8 gBattleTerrain;
extern struct MultiBattlePokemonTx gMultiPartnerParty[PARTY_SIZE / 2];
extern u8 gPartyCriticalHits[PARTY_SIZE];
extern u8 gBattlerTarget;
extern u8 gAbsentBattlerFlags;
extern u32 gFieldStatus;
extern struct FieldTimer gFieldTimers;
extern struct QueuedStatBoost gQueuedStatBoosts[MAX_BATTLERS_COUNT];

#endif // GUARD_BATTLE_H

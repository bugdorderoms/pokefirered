#ifndef GUARD_BATTLE_UTIL_H
#define GUARD_BATTLE_UTIL_H

#include "global.h"

// Return values for the CanBe* statused functions
enum
{
    STATUS_CHANGE_WORKED,
    STATUS_CHANGE_FAIL_ALREADY_STATUSED,
    STATUS_CHANGE_FAIL_SPECIFIC_STATUSED,
    STATUS_CHANGE_FAIL_TYPE_NOT_AFFECTED,
    STATUS_CHANGE_FAIL_WEATHER_PREVENTED,
    STATUS_CHANGE_FAIL_ABILITY_PREVENTED,
    STATUS_CHANGE_FAIL_SAFEGUARD_PROTECTED,
    STATUS_CHANGE_FAIL_UPROAR,
    STATUS_CHANGE_FAIL_SLEEP_CLAUSE,
    STATUS_CHANGE_FAIL_SWEET_VEIL_ON_SIDE,
    STATUS_CHANGE_FAIL_FLOWER_VEIL_ON_SIDE,
    STATUS_CHANGE_FAIL_PASTEL_VEIL_ON_SIDE,
    STATUS_CHANGE_FAIL_AROMA_VEIL_ON_SIDE,
};

// Result values for ChangeStatBuffs
enum
{
    STAT_CHANGE_WORKED,
    STAT_CHANGE_FAIL_WONT_CHANGE,
    STAT_CHANGE_FAIL_MIST,
    STAT_CHANGE_FAIL_PROTECTED,
    STAT_CHANGE_FAIL_ABILITY_PREVENTED,
    STAT_CHANGE_FAIL_ABILITY_PREVENT_SPECIFIC_STAT,
    STAT_CHANGE_FAIL_FLOWER_VEIL,
};

struct BattleWeatherInfo
{
    const struct SpriteSheet iconGfx;
    const struct SpritePalette iconPalette;
    u16 flag;
    u8 continueMessage;
    u8 endMessage;
    u8 animation;
};

// Flags for CheckMoveLimitations
#define MOVE_LIMITATION_IGNORE_NO_PP    Bit(0)
#define MOVE_LIMITATION_IGNORE_IMPRISON Bit(1)
#define MOVE_LIMITATION_ALL_MOVES_MASK  (Bit(MAX_MON_MOVES) - 1) // Mask when all moves are unusable

// Cases for AbilityBattleEffects
#define ABILITYEFFECT_ON_SWITCHIN         0
#define ABILITYEFFECT_ENDTURN             1
#define ABILITYEFFECT_NEUTRALIZING_GAS    2
#define ABILITYEFFECT_UNNERVE             3
#define ABILITYEFFECT_MOVE_END_ATTACKER   4
#define ABILITYEFFECT_MOVE_END_TARGET     5
#define ABILITYEFFECT_IMMUNITY            6
#define ABILITYEFFECT_ON_WEATHER          7
#define ABILITYEFFECT_SYNCHRONIZE         8
#define ABILITYEFFECT_ON_TERRAIN          9
#define ABILITYEFFECT_OPPORTUNIST         10
#define ABILITYEFFECT_ON_TERASTALLIZATION 11

// Cases for CheckAbilityInBattle
#define CHECK_ABILITY_ON_FIELD                0
#define CHECK_ABILITY_ON_FIELD_EXCEPT_BATTLER 1
#define CHECK_ABILITY_ON_SIDE                 2

// CheckAbilityInBattle expanded to more readable macros
#define ABILITY_ON_SIDE(battlerId, abilityId) ((CheckAbilityInBattle(CHECK_ABILITY_ON_SIDE, battlerId, abilityId)))
#define ABILITY_ON_OPPOSING_SIDE(battlerId, abilityId) ABILITY_ON_SIDE(BATTLE_OPPOSITE(battlerId), abilityId)
#define ABILITY_ON_FIELD(abilityId) ((CheckAbilityInBattle(CHECK_ABILITY_ON_FIELD, 0, abilityId)))
#define ABILITY_ON_FIELD_EXCEPT_BATTLER(battlerId, abilityId) ((CheckAbilityInBattle(CHECK_ABILITY_ON_FIELD_EXCEPT_BATTLER, battlerId, abilityId)))

// Cases for ItemBattleEffects
#define ITEMEFFECT_ON_SWITCH_IN                 0x0
#define ITEMEFFECT_MOVE_END                     0x3
#define ITEMEFFECT_KINGSROCK_SHELLBELL          0x4

#define BATTLE_ALIVE_SIDE           0
#define BATTLE_ALIVE_EXCEPT_BATTLER 1
#define BATTLE_ALIVE_FIELD          2

#define IS_WHOLE_SIDE_ALIVE(battler) ((CountAliveMonsInBattle(battler, BATTLE_ALIVE_SIDE) >= NUM_BATTLERS_PER_SIDE))

#define IS_MULTIHIT_FINAL_STRIKE ((gBattleStruct->pursuitSwitchDmg || gMultiHitCounter <= 1))

u32 GetBattlerForBattleScript(u32 caseId);
void MarkBattlerForControllerExec(u32 battlerId);
void MarkBattlerReceivedLinkData(u32 battlerId);
void CancelMultiTurnMoves(u32 battler);
void BringDownInAirBattler(u32 battlerId);
bool32 WasUnableToUseMove(u32 battler);
void PrepareStringBattle(u32 stringId, u32 battler);
void ResetSentPokesToOpponentValue(void);
void OpponentSwitchInResetSentPokesToOpponentValue(u32 battler);
void UpdateSentPokesToOpponentValue(u32 battler);
void BattleScriptPush(const u8 *bsPtr);
void BattleScriptPushCursor(void);
void BattleScriptPop(void);
void BattleScriptCall(const u8 *bsPtr);
bool32 TrySetCantSelectMoveBattleScript(u32 battlerId, u32 movePos);
u32 CheckMoveLimitations(u32 battlerId, u32 flags);
bool32 AreAllMovesUnusable(u32 battlerId);
bool32 DoEndTurnEffects(void);
bool32 HandleFaintedMonActions(void);
void TryClearRageStatuses(void);
u32 AtkCanceller_UnableToUseMove(void);
bool32 HasNoMonsToSwitch(u32 battler, u32 partyIdBattlerOn1, u32 partyIdBattlerOn2);
u32 AbilityBattleEffects(u32 caseId, u32 battler);
u32 CheckAbilityInBattle(u32 mode, u32 battlerId, u32 abilityId);
bool32 DoSwitchInAbilitiesItems(u32 battlerId);
void BattleScriptExecute(const u8 *BS_ptr);
void BattleScriptPushCursorAndCallback(const u8 *BS_ptr);
u8 ItemBattleEffects(u8 caseID, u8 battlerId, bool8 moveTurn);
void ClearFuryCutterDestinyBondGrudge(u32 battlerId);
u32 GetBattleMoveSplit(u32 move);
u32 GetDefaultMoveTarget(u32 battlerId);
u32 SetRandomTarget(u32 battlerId);
u32 GetRandomTarget(u32 battlerId);
u32 GetBattlerMoveTargetType(u32 battlerId, u32 move);
u32 GetMoveTarget(u32 move, u32 setTarget, bool32 useBaseType);
void CopyMoveTargetName(u32 moveTarget, u8 *dest);
u32 IsMonDisobedient(void);
bool32 SubsBlockMove(u32 attacker, u32 defender, u32 move);
u32 GetHiddenPowerType(struct Pokemon *mon);
bool32 CheckPinchBerryActivate(u32 battler, u32 item);
void CopyStatusStringToBattleBuffer1(u32 statusId);
void ClearBattlerStatus(u32 battler);
u32 CanBecameConfused(u32 attacker, u32 defender, u32 flags);
u32 CanBePutToSleep(u32 attacker, u32 defender, u32 flags);
u32 CanBePoisoned(u32 attacker, u32 defender, u32 flags);
u32 CanBeBurned(u32 attacker, u32 defender, u32 flags);
bool32 CanBeFrozen(u32 attacker, u32 defender, u32 flags);
u32 CanBeParalyzed(u32 attacker, u32 defender, u32 flags);
u32 GetBattlerItemHoldEffect(u32 battler, bool32 checkNegating);
u32 GetBattlerAbility(u32 battler);
u32 SetBattlerAbility(u32 battlerId, u32 newAbility);
u32 SuppressBattlerAbility(u32 battlerId);
void SetIllusionMon(u32 battler, bool32 canDisguiseAsAlly);
void ClearIllusionMon(u32 battler);
bool32 TryRemoveIllusion(u32 battler);
u32 GetPartyMonIdForIllusion(u32 battler, struct Pokemon *party, u32 partyCount, struct Pokemon *illusionMon, bool32 canDisguiseAsAlly);
bool32 MoveIsAffectedBySheerForce(u32 move);
bool32 ReceiveSheerForceBoost(u32 battler, u32 move);
bool32 CompareStat(u32 battlerId, u32 statId, s8 cmpTo, u32 cmpKind);
bool32 IsUnnerveOnOpposingField(u32 battler);
u16 *GetUsedHeldItemPtr(u32 battler);
bool32 NoAliveMonsForParty(struct Pokemon *party);
bool32 NoAliveMonsForEitherParty(void);
bool32 IsBattlerAlive(u32 battlerId);
bool32 IsBattlerWeatherAffected(u32 battlerId, u32 weatherFlags);
bool32 TryChangeBattleWeather(u32 battlerId, u32 weatherEnumId);
u32 GetCurrentWeatherEnumId(u32 weatherFlags);
bool32 CalcMoveIsCritical(u32 battlerAtk, u32 battlerDef, u32 move);
bool32 IsMoveMakingContact(u32 battler, u32 move);
bool32 IsBattlerProtected(u32 attacker, u32 defender, u32 move, bool32 ignoreZMoveCheck);
u32 CountBattlerStatIncreases(u32 battlerId, bool32 countEvasionAccuracy);
bool32 IsBattlerGrounded(u32 battlerId);
bool32 IsBattlerGroundedInternal(u32 battlerId, bool32 ignoreGravity, bool32 checkInverseBattle);
bool32 CanBattlerGetOrLoseItem(u32 battlerId, u32 itemId);
bool32 CanStealItem(u32 battlerAtk, u32 battlerDef, u32 itemId);
void RemoveBattlerItem(u32 battlerId);
void GiveItemToBattler(u32 battlerId, u32 itemId);
void SortBattlersBySpeed(u8 *battlers, bool32 slowToFast);
u32 CountUsablePartyMons(u32 battlerId, u8 *viableMons, bool32(*excludes)(u32, u32, u32));
bool32 CanBattlerFlee(u32 battlerId);
bool32 CanBattlerEscape(u32 battlerId, bool32 checkIngrain);
bool32 CanBattlerSwitchOut(u32 battlerId, bool32 checkEscapePrevention);
u32 IsAbilityPreventingSwitchOut(u32 battlerId);
u32 GetCatchingBattler(void);
u32 GetBattlerTurnOrderNum(u32 battlerId);
u32 CalcSecondaryEffectChance(u32 battlerId, u32 moveEffect, u32 chance);
bool32 TryResetBattlerStatChanges(u32 battlerId);
void CopyBattlerStatChanges(u32 battler1, u32 battler2);
u32 GetBattlerGender(u32 battlerId);
u32 CanBeInfatuatedBy(u32 battlerIdAtk, u32 battlerIdDef);
u32 GetBattlerWeight(u32 battlerId);
bool32 IsBattlerAffectedBySpore(u32 battlerId);
void RemoveOrAddBattlerOnPickupStack(u32 battlerId, bool32 addToStack);
u32 GetBattlerOnTopOfPickupStack(u32 battlerId);
bool32 TryRecycleBattlerItem(u32 battlerRecycler, u32 itemBattler);
u32 GetBattlerHighestStatId(u32 battlerId, bool32 checkWonderRoom);
bool32 IsBattlerAffectedByFollowMe(u32 battlerId, u32 opposingSide, u32 move);
bool32 IsMoveAffectedByRedirectionEffects(u32 battlerId, u32 move);
u32 GetItemUseBattler(u32 battlerId);
u32 GetBattleMonForItemUse(u32 battlerId, u32 partyIndex);
bool32 IsItemUseBlockedByBattleEffect(u32 battlerId);
bool32 CanThrowBall(void);
const u8 *PokemonUseItemEffectsBattle(u32 battlerId, u32 itemId, bool32 *canUse);
bool32 IsBattleAnimationsOn(void);
struct Pokemon *GetBattlerParty(u32 battlerId);
struct Pokemon *GetBattlerPartyIndexPtr(u32 battlerId);
struct Pokemon *GetBattlerIllusionPartyIndexPtr(u32 battlerId);
u32 CountAliveMonsInBattle(u32 battlerId, u32 caseId);
u32 CalculatePayDayMoney(void);
s32 GetDrainedBigRootHp(u32 battlerId, s32 hp);
u32 GetBattlerMoveType(u32 battlerId, u32 move);
bool32 TryTransformIntoBattler(u32 battler1, u32 battler2);
bool32 CanSafeguardProtectBattler(u32 attacker, u32 defender);
bool32 IsBattlerProtectedByFlowerVeil(u32 battlerId);
void SaveBattlersHps(void);
void GetBattlerTypes(u32 battlerId, bool32 ignoreTera, u32 *types);
void SetBattlerType(u32 battlerId, u32 type);
void SetBattlerInitialTypes(u32 battlerId);
bool32 DoBattlersShareType(u32 battler1, u32 battler2);
bool32 TryRemoveScreens(u32 battler, bool32 clear, bool32 fromBothSides);
bool32 DoesSpreadMoveStrikesOnlyOnce(u32 attacker, u32 defender, u32 move, bool32 checkTargetsDone);
bool32 TryRemoveEntryHazards(u32 battler, bool32 clear, bool32 fromBothSides);
bool32 TryActivateEmergencyExit(u32 battler);
bool32 LiftProtectionEffects(u32 battlerId);
bool32 IsBattlerBeingCommanded(u32 battlerId);
void SaveAttackerToStack(u32 battlerId);
void RestoreAttackerFromStack(void);
void SaveTargetToStack(u32 battlerId);
void RestoreTargetFromStack(void);
u32 FindMoveSlotInBattlerMoveset(u32 battlerId, u32 move);
bool32 CanUseLastResort(u32 battlerId);
void TryUpdateEvolutionTracker(u32 evoMode, u32 upAmount, u32 data);
bool32 IsDoubleBattleForBattler(u32 battler);
bool32 IsPlayerBagDisabled(void);
u32 GetNumBeatUpHits(u32 battler);
s8 GetItemStatChangeStages(u32 item);
bool32 CanReceiveBadgeBoost(u32 battlerId, u32 flagId);
bool32 MoveHasMoveEffect(u32 move, u32 moveEffect, bool32 self);
bool32 MoveHasMoveEffectWithChance(u32 move, u32 moveEffect, u32 chance);
bool32 MoveHasChargeTurnMoveEffect(u32 move);
bool32 TryInitSosCall(void);
const struct SosCall GetSosCallRateTable(void);
bool32 IsBattlerTotemPokemon(u32 battlerId);
void SwapBattlersPositions(u32 battler1, u32 battler2);
bool32 TryBattleChallengeStartingStatus(void);
bool32 IsPartnerMonFromSameTrainer(u32 battler);
bool32 TryPrimalReversion(u32 battler);
u32 GetBattlerIdFromPartySlot(u32 slot, u32 partyId);
u32 CanAbilityAbsorbMove(u32 ability, u32 move, u32 moveType, u32 attacker, u32 target, bool32 onlyChecking);
bool32 CanAbilityBlockMove(u32 move, u32 attacker, u32 target, bool32 onlyChecking);
bool32 BattlerTurnDamaged(u32 battlerId);

static inline bool32 CanBattlerSwitch(u32 battlerId)
{
    u8 viableMons[PARTY_SIZE];
    return (CountUsablePartyMons(battlerId, viableMons, NULL) > 0);
}

extern const struct BattleWeatherInfo gBattleWeatherInfo[];

#endif // GUARD_BATTLE_UTIL_H

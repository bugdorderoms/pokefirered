#ifndef GUARD_BATTLE_UTIL_H
#define GUARD_BATTLE_UTIL_H

#include "global.h"

// Return values for the CanBe* statused function
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

#define MOVE_LIMITATION_ALL_MOVES_MASK ((1 << MAX_MON_MOVES) - 1)

#define MOVE_LIMITATION_IGNORE_NO_PP             (1 << 0)
#define MOVE_LIMITATION_IGNORE_IMPRISON          (1 << 1)

#define ABILITYEFFECT_ON_SWITCHIN                0
#define ABILITYEFFECT_ENDTURN                    1
#define ABILITYEFFECT_TRACE                      2
#define ABILITYEFFECT_NEUTRALIZING_GAS           3
#define ABILITYEFFECT_UNNERVE                    4
#define ABILITYEFFECT_MOVES_BLOCK                5
#define ABILITYEFFECT_ABSORBING                  6
#define ABILITYEFFECT_MOVE_END_ATTACKER          7
#define ABILITYEFFECT_MOVE_END_TARGET            8
#define ABILITYEFFECT_IMMUNITY                   9
#define ABILITYEFFECT_ON_WEATHER                 10
#define ABILITYEFFECT_SYNCHRONIZE                11
#define ABILITYEFFECT_ON_TERRAIN                 12

#define CHECK_ABILITY_ON_FIELD                   0
#define CHECK_ABILITY_ON_FIELD_EXCEPT_BATTLER    1
#define CHECK_ABILITY_ON_SIDE                    2

#define ABILITY_ON_OPPOSING_SIDE(battlerId, abilityId) ((CheckAbilityInBattle(CHECK_ABILITY_ON_SIDE, BATTLE_OPPOSITE(battlerId), abilityId)))
#define ABILITY_ON_FIELD(abilityId) ((CheckAbilityInBattle(CHECK_ABILITY_ON_FIELD, 0, abilityId)))
#define ABILITY_ON_FIELD_EXCPET_BATTLER(battlerId, abilityId) ((CheckAbilityInBattle(CHECK_ABILITY_ON_FIELD_EXCEPT_BATTLER, battlerId, abilityId)))

#define ITEMEFFECT_ON_SWITCH_IN                 0x0
#define ITEMEFFECT_MOVE_END                     0x3
#define ITEMEFFECT_KINGSROCK_SHELLBELL          0x4

#define WEATHER_HAS_EFFECT ((!ABILITY_ON_FIELD(ABILITY_CLOUD_NINE) && !ABILITY_ON_FIELD(ABILITY_AIR_LOCK)))

#define BS_GET_TARGET                   0
#define BS_GET_ATTACKER                 1
#define BS_GET_EFFECT_BANK              2
#define BS_GET_SCRIPTING_BANK           10
#define BS_GET_PLAYER1                  11
#define BS_GET_OPPONENT1                12
#define BS_GET_PLAYER2                  13
#define BS_GET_OPPONENT2                14

#define BATTLE_ALIVE_SIDE            0
#define BATTLE_ALIVE_EXCEPT_BATTLER  1

#define IS_WHOLE_SIDE_ALIVE(battler) ((CountAliveMonsInBattle(battler, BATTLE_ALIVE_SIDE) >= 2))

u8 GetBattlerForBattleScript(u8 caseId);
void MarkBattlerForControllerExec(u8 battlerId);
void MarkBattlerReceivedLinkData(u8 battlerId);
void CancelMultiTurnMoves(u8 battler);
bool8 WasUnableToUseMove(u8 battler);
void PrepareStringBattle(u16 stringId, u8 battler);
void ResetSentPokesToOpponentValue(void);
void OpponentSwitchInResetSentPokesToOpponentValue(u8 battler);
void UpdateSentPokesToOpponentValue(u8 battler);
void BattleScriptPush(const u8 *bsPtr);
void BattleScriptPushCursor(void);
void BattleScriptPop(void);
void BattleScriptCall(const u8 *bsPtr);
bool8 TrySetCantSelectMoveBattleScript(u8 battlerId);
u8 CheckMoveLimitations(u8 battlerId, u32 flags);
bool8 AreAllMovesUnusable(u8 battlerId);
bool8 DoEndTurnEffects(void);
bool8 HandleFaintedMonActions(void);
void TryClearRageStatuses(void);
u8 AtkCanceller_UnableToUseMove(void);
bool8 HasNoMonsToSwitch(u8 battler, u8 partyIdBattlerOn1, u8 partyIdBattlerOn2);
u8 AbilityBattleEffects(u8 caseId, u8 battler);
u8 CheckAbilityInBattle(u8 mode, u8 battlerId, u16 abilityId);
bool8 DoSwitchInAbilitiesItems(u8 battlerId);
void BattleScriptExecute(const u8 *BS_ptr);
void BattleScriptPushCursorAndCallback(const u8 *BS_ptr);
u8 ItemBattleEffects(u8 caseID, u8 battlerId, bool8 moveTurn);
void ClearFuryCutterDestinyBondGrudge(u8 battlerId);
void HandleAction_RunBattleScript(void);
u8 GetMoveSplit(u16 move);
u8 GetDefaultMoveTarget(u8 battlerId);
u8 GetRandomTarget(u8 battlerId);
u8 GetBattlerMoveTargetType(u8 battlerId, u16 move);
u8 GetMoveTarget(u16 move, u8 setTarget);
void CopyMoveTargetName(u8 battlerId, u16 move, u8 *dest);
u8 IsMonDisobedient(void);
bool8 SubsBlockMove(u8 attacker, u8 defender, u16 move);
u8 GetHiddenPowerType(struct Pokemon *mon);
bool8 CheckPinchBerryActivate(u8 battler, u16 item);
void CopyStatusStringToBattleBuffer1(u32 status);
void ClearBattlerStatus(u8 battler);
u8 CanBecameConfused(u8 attacker, u8 defender, u32 flags);
u8 CanBePutToSleep(u8 attacker, u8 defender, u32 flags);
u8 CanBePoisoned(u8 attacker, u8 defender, u32 flags);
u8 CanBeBurned(u8 attacker, u8 defender, u32 flags);
bool8 CanBeFrozen(u8 attacker, u8 defender, u32 flags);
u8 CanBeParalyzed(u8 attacker, u8 defender, u32 flags);
u8 GetBattlerItemHoldEffect(u8 battler, bool8 checkNegating);
u16 GetBattlerAbility(u8 battler);
u16 SetBattlerAbility(u8 battlerId, u16 newAbility);
u16 SuppressBattlerAbility(u8 battlerId);
void ClearIllusionMon(u8 battler);
bool8 TryRemoveIllusion(u8 battler);
u8 GetPartyMonIdForIllusion(u8 battler, struct Pokemon *party, u8 partyCount, struct Pokemon *illusionMon);
bool8 ReceiveSheerForceBoost(u8 battler, u16 move);
bool8 CompareStat(u8 battlerId, u8 statId, s8 cmpTo, u8 cmpKind);
bool8 IsUnnerveOnOpposingField(u8 battler);
u16 *GetUsedHeldItemPtr(u8 battler);
bool8 NoAliveMonsForParty(struct Pokemon *party);
bool8 NoAliveMonsForEitherParty(void);
bool8 IsBattlerAlive(u8 battlerId);
bool8 IsBattlerWeatherAffected(u8 battlerId, u16 weatherFlags);
bool8 TryChangeBattleWeather(u8 battlerId, u8 weatherEnumId);
s16 CalcMoveCritChance(u8 battlerAtk, u8 battlerDef, u16 move);
bool8 IsMoveMakingContact(u8 battler, u16 move);
bool8 IsBattlerProtected(u8 attacker, u8 defender, u16 move);
u8 CountBattlerStatIncreases(u8 battlerId, bool8 countEvasionAccuracy);
bool8 IsBattlerGrounded(u8 battlerId);
bool8 CanStealItem(u8 battlerAtk, u8 battlerDef, u16 itemId);
void StealTargetItem(u8 battlerAtk, u8 battlerDef);
void SortBattlersBySpeed(u8 *battlers, bool8 slowToFast);
u8 CountUsablePartyMons(u8 battlerId);
bool8 CanBattlerEscape(u8 battlerId, bool8 checkIngrain);
bool8 CanBattlerSwitch(u8 battlerId);
u8 IsAbilityPreventingSwitchOut(u8 battlerId);
u8 GetCatchingBattler(void);
u8 GetBattlerTurnOrderNum(u8 battlerId);
void CheckSetBattlerUnburden(u8 battler);
u32 GetEffectChanceIncreases(u8 battlerId, u32 secondaryEffectChance);
bool8 TryResetBattlerStatChanges(u8 battlerId);
void CopyBattlerStatChanges(u8 battler1, u8 battler2);
u8 GetBattlerGender(u8 battlerId);
u8 CanBeInfatuatedBy(u8 battlerIdAtk, u8 battlerIdDef);
u32 GetBattlerWeight(u8 battlerId);
bool8 IsBattlerAffectedBySpore(u8 battlerId);
void RemoveOrAddBattlerOnPickupStack(u8 battlerId, bool8 addToStack);
u8 GetBattlerOnTopOfPickupStack(u8 battlerId);
bool8 TryRecycleBattlerItem(u8 battlerRecycler, u8 battlerItem);
u8 GetBattlerHighestStatId(u8 battlerId);
bool8 IsBattlerAffectedByFollowMe(u8 battlerId, u8 opposingSide, u16 move);
u8 GetItemUseBattler(u8 battlerId);
u8 GetBattleMonForItemUse(u8 battlerId, u8 partyIndex);
bool8 IsItemUseBlockedByBattleEffect(u8 battlerId);
const u8 *PokemonUseItemEffectsBattle(u8 battlerId, u16 itemId, bool8 *canUse);
bool8 IsBattleAnimationsOn(void);
struct Pokemon *GetBattlerParty(u8 battlerId);
struct Pokemon *GetBattlerPartyIndexPtr(u8 battlerId);
struct Pokemon *GetBattlerIllusionPartyIndexPtr(u8 battlerId);
u8 CountAliveMonsInBattle(u8 battlerId, u8 caseId);
void CalculatePayDayMoney(void);
s32 GetDrainedBigRootHp(u8 battlerId, s32 hp);
void SetTypeBeforeUsingMove(u16 move, u8 battler);
bool8 TryTransformIntoBattler(u8 battler1, u8 battler2);
bool8 TryDisableMove(u8 battlerId, u8 movePos, u16 move);
bool8 CanSafeguardProtectBattler(u8 attacker, u8 defender);
bool8 IsBattlerProtectedByFlowerVeil(u8 battlerId);
u16 GetFutureAttackData(u8 caseId, u16 move);
void SaveBattlersHps(void);
bool8 IsBattlerOfType(u8 battlerId, u8 type);
u8 GetBattlerType(u8 battlerId, u8 index);
void SetBattlerType(u8 battlerId, u8 type);
void SetBattlerInitialTypes(u8 battlerId);
bool8 CopyBattlerCritModifier(u8 attacker, u8 defender);
bool8 TryRemoveScreens(u8 battler, bool8 fromBothSides);
bool8 DoesSpreadMoveStrikesOnlyOnce(u8 attacker, u8 defender, u16 move, bool8 checkTargetsDone);
u8 GetTypeChangingMoveType(struct Pokemon *mon, u16 move);
u8 GetSecretPowerEffect(void);
bool8 TryRemoveEntryHazards(u8 battlerId);
bool8 AttacksThisTurn(u8 battlerId, u16 move);
bool8 TryActivateEmergencyExit(u8 battler);
bool8 LiftProtectionEffects(u8 battlerId);
bool8 IsBattlerBeingCommanded(u8 battlerId);
void SaveAttackerToStack(u8 battlerId);
void RestoreAttackerFromStack(void);
void SaveTargetToStack(u8 battlerId);
void RestoreTargetFromStack(void);
u8 FindMoveSlotInBattlerMoveset(u8 battlerId, u16 move);
bool8 CanUseLastResort(u8 battlerId);
bool8 TrySetToxicSpikesOnBattlerSide(u8 battlerId);
void TryUpdateEvolutionTracker(u16 evoMode, u32 upAmount, u16 data);
bool8 IsMultiBattle(void);

#endif // GUARD_BATTLE_UTIL_H

#ifndef GUARD_BATTLE_UTIL_H
#define GUARD_BATTLE_UTIL_H

#include "global.h"

#define MOVE_LIMITATION_ZEROMOVE                (1 << 0)
#define MOVE_LIMITATION_PP                      (1 << 1)
#define MOVE_LIMITATION_DISABLED                (1 << 2)
#define MOVE_LIMITATION_TORMENTED               (1 << 3)
#define MOVE_LIMITATION_TAUNT                   (1 << 4)
#define MOVE_LIMITATION_IMPRISON                (1 << 5)

#define ABILITYEFFECT_SWITCH_IN_WEATHER          0
#define ABILITYEFFECT_ON_SWITCHIN                1
#define ABILITYEFFECT_ENDTURN                    2
#define ABILITYEFFECT_TRACE1                     3
#define ABILITYEFFECT_TRACE2                     4
#define ABILITYEFFECT_NEUTRALIZING_GAS           5
#define ABILITYEFFECT_UNNERVE                    6
#define ABILITYEFFECT_MOVES_BLOCK                7
#define ABILITYEFFECT_ABSORBING                  8
#define ABILITYEFFECT_MOVE_END_ATTACKER          9
#define ABILITYEFFECT_MOVE_END_TARGET            10
#define ABILITYEFFECT_IMMUNITY                   11
#define ABILITYEFFECT_ON_WEATHER                 12
#define ABILITYEFFECT_SYNCHRONIZE                13
#define ABILITYEFFECT_ON_TERRAIN                 14

#define CHECK_ABILITY_ON_FIELD                   0
#define CHECK_ABILITY_ON_FIELD_EXCEPT_BATTLER    1
#define CHECK_ABILITY_ON_SIDE                    2

#define ABILITY_ON_OPPOSING_SIDE(battlerId, abilityId)(CheckAbilityInBattle(CHECK_ABILITY_ON_SIDE, BATTLE_OPPOSITE(battlerId), abilityId))
#define ABILITY_ON_FIELD(abilityId)(CheckAbilityInBattle(CHECK_ABILITY_ON_FIELD, 0, abilityId))
#define ABILITY_ON_FIELD_EXCPET_BATTLER(battlerId, abilityId)(CheckAbilityInBattle(CHECK_ABILITY_ON_FIELD_EXCEPT_BATTLER, battlerId, abilityId))

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

// For ChangeStatBuffs
#define STAT_CHANGE_WORKED      0
#define STAT_CHANGE_DIDNT_WORK  1

#define BATTLE_ALIVE_EXCEPT_ACTIVE   0
#define BATTLE_ALIVE_ATK_SIDE        1
#define BATTLE_ALIVE_DEF_SIDE        2
#define BATTLE_ALIVE_EXCEPT_ATTACKER 3

u8 GetBattlerForBattleScript(u8 caseId);
void PressurePPLose(u8 target, u8 attacker, u16 move);
void PressurePPLoseOnUsingImprison(u8 attacker);
void PressurePPLoseOnUsingPerishSong(u8 attacker);
void MarkAllBattlersForControllerExec(void);
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
u8 TrySetCantSelectMoveBattleScript(void);
u8 CheckMoveLimitations(u8 battlerId, u8 unusableMoves, u8 check);
bool8 AreAllMovesUnusable(void);
u8 GetImprisonedMovesCount(u8 battlerId, u16 move);
u8 DoFieldEndTurnEffects(void);
u8 DoBattlerEndTurnEffects(void);
bool8 HandleWishPerishSongOnTurnEnd(void);
bool8 HandleFaintedMonActions(void);
void TryClearRageStatuses(void);
u8 AtkCanceller_UnableToUseMove(void);
bool8 HasNoMonsToSwitch(u8 battler, u8 partyIdBattlerOn1, u8 partyIdBattlerOn2);
u8 AbilityBattleEffects(u8 caseId, u8 battler, u16 moveArg);
u8 CheckAbilityInBattle(u8 mode, u8 battlerId, u16 abilityId);
bool8 DoSwitchInAbilitiesItems(u8 battlerId);
void BattleScriptExecute(const u8 *BS_ptr);
void BattleScriptPushCursorAndCallback(const u8 *BS_ptr);
u8 ItemBattleEffects(u8 caseID, u8 battlerId, bool8 moveTurn);
void ClearFuryCutterDestinyBondGrudge(u8 battlerId);
void HandleAction_RunBattleScript(void);
u8 GetDefaultMoveTarget(u8 battlerId);
u8 GetRandomTarget(u8 battlerId);
u8 GetBattlerMoveTargetType(u8 battlerId, u16 move);
u8 GetMoveTarget(u16 move, u8 setTarget);
void CopyMoveTargetName(u8 *dest, u16 move);
u8 IsMonDisobedient(void);
bool8 SubsBlockMove(u8 attacker, u8 defender, u16 move);
u8 GetHiddenPowerType(struct Pokemon *mon);
bool8 CheckPinchBerryActivate(u8 battler, u16 item);
void ClearBattlerStatus(u8 battler);
bool8 CanBePutToSleep(u8 bank, bool8 checkFlowerVeil);
bool8 CanBePoisoned(u8 bankDef, u8 bankAtk, bool8 checkFlowerVeil);
bool8 CanPoisonType(u8 bankAtk, u8 bankDef);
bool8 CanBeBurned(u8 bank, bool8 checkFlowerVeil);
bool8 CanBeFrozen(u8 bank, bool8 checkFlowerVeil);
bool8 CanBeParalyzed(u8 bank, bool8 checkFlowerVeil);
u8 GetBattlerItemHoldEffect(u8 battler, bool8 checkNegating);
u16 GetBattlerAbility(u8 battler);
u16 SetBattlerAbility(u8 battlerId, u16 newAbility);
u16 SuppressBattlerAbility(u8 battlerId);
void ClearIllusionMon(u8 battler);
bool8 TryRemoveIllusion(u8 battler);
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
bool8 IsBattlerProtected(u8 battlerId, u16 move);
u8 CountBattlerStatIncreases(u8 battlerId, bool8 countEvasionAccuracy);
bool8 IsBattlerGrounded(u8 battlerId);
bool8 CanStealItem(u8 battlerAtk, u8 battlerDef, u16 itemId);
void StealTargetItem(u8 battlerAtk, u8 battlerDef);
void SortBattlersBySpeed(u8 *battlers, bool8 slowToFast);
u8 CountUsablePartyMons(u8 battlerId);
bool8 CanBattlerEscape(u8 battlerId);
bool8 CanBattlerSwitch(u8 battlerId);
u8 IsAbilityPreventingSwitchOut(u8 battlerId);
u8 GetCatchingBattler(void);
u8 GetBattlerTurnOrderNum(u8 battlerId);
void CheckSetBattlerUnburden(u8 battler);
u32 GetEffectChanceIncreases(u8 battlerId, u32 secondaryEffectChance);
u8 ChangeStatBuffs(s8 statValue, u8 statId, u8, const u8 *BS_ptr);
bool8 TryResetBattlerStatChanges(u8 battlerId);
void CopyBattlerStatChanges(u8 battler1, u8 battler2);
u8 GetBattlerGender(u8 battlerId);
bool8 CanBeInfatuatedBy(u8 battlerIdAtk, u8 battlerIdDef);
u32 GetBattlerWeight(u8 battlerId);
bool8 IsBattlerAffectedBySpore(u8 battlerId);
void RemoveOrAddBattlerOnPickupStack(u8 battlerId, bool8 addToStack);
u8 GetBattlerOnTopOfPickupStack(u8 battlerId);
bool8 TryRecycleBattlerItem(u8 battlerRecycler, u8 battlerItem);
u8 GetBattlerHighestStatId(u8 battlerId);
bool8 MoveHasHealingEffect(u16 move);
bool8 IsBattlerAffectedByFollowMe(u8 battlerId, u8 opposingSide, u16 move);
bool8 IsAbilityBlockedByNeutralizingGas(u16 ability);
u8 GetItemUseBattler(u8 battlerId);
u8 GetBattleMonForItemUse(u8 partyIndex);
bool8 IsItemUseBlockedByBattleEffect(void);
const u8 *PokemonUseItemEffectsBattle(u8 battlerId, u16 itemId, bool8 *canUse);
bool8 IsBattleAnimationsOn(void);
struct Pokemon *GetBattlerParty(u8 battlerId);
struct Pokemon *GetBattlerPartyIndexPtr(u8 battlerId);
struct Pokemon *GetBattlerIllusionPartyIndexPtr(u8 battlerId);
u8 CountAliveMonsInBattle(u8 caseId);

#endif // GUARD_BATTLE_UTIL_H

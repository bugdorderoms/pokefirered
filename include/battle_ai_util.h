#ifndef GUARD_BATTLE_AI_UTIL_H
#define GUARD_BATTLE_AI_UTIL_H

#include "battle_ai.h"

void AI_RecordLastUsedMoveByBattler(u32 battlerId, u32 move);
void AI_ClearRecordedMovesByBattler(u32 battlerId);
bool32 AI_IsTargetFullyImmuneToMove(struct AIScript *data);
u32 AI_GetNoOfHitsToKOTarget(struct AIScript *data);
u32 AI_GetNoOfHitsToKOAttacker(struct AIScript *data);
u32 AI_GetNoOfHitsToKOBattlerHigherDamage(u32 attacker, u32 defender);
u32 AI_GetNumMovesWithSplitInBattlerMoveset(u32 battlerId, u32 split);
bool32 AI_IsFaster(u32 battler1, u32 battler2);
bool32 AI_HasDamagingMoveWithTypeInMoveset(u32 battlerId, u32 type);
bool32 AI_BattlerHasMoveEffectInMoveset(u32 battlerId, u32 moveEffect);
bool32 AI_BattlerHasMoveWithAdditionalEffectInMoveset(u32 battlerId, u32 moveEffect);
bool32 AI_BattlerHasMoveEffectInMovesetThatAffectsTarget(u32 attacker, u32 target, u32 moveEffect);
bool32 AI_SideHasMoveEffectInMovesetThatAffectsTarget(u32 attacker, u32 target, u32 moveEffect);
bool32 AI_BattlerHasThawingUserMoveInMoveset(u32 battlerId);
bool32 AI_BattlerHasProtectionMoveInMoveset(u32 battlerId);
s8 AI_GetScoreForSpeedControl(struct AIScript *data);
bool32 AI_BadIdeaToBurn(struct AIScript *data);
bool32 AI_GoodIdeaToBurnSelf(struct AIScript *data);
s8 AI_GetScoreForInflictBurn(struct AIScript *data);
bool32 AI_BadIdeaToFreeze(struct AIScript *data);
s8 AI_GetScoreForInflictFreeze(struct AIScript *data);
bool32 AI_BadIdeaToParalyze(struct AIScript *data);
bool32 AI_GoodIdeaToParalyzeSelf(struct AIScript *data);
s8 AI_GetScoreForInflictParalysis(struct AIScript *data);
bool32 AI_GoodIdeaToRaiseAttackAgainstTarget(struct AIScript *data, u32 stages);
bool32 AI_BadIdeaToRaiseAttackAgainst(u32 attacker, u32 target, u32 stages, bool32 checkPartner);
bool32 AI_WillMoveBeLockedWhenUsed(struct AIScript *data);
bool32 AI_PartnerMoveEffectIsSameNoTarget(struct AIScript *data);
bool32 AI_PartnerMoveEffectIsSame(struct AIScript *data);

#define AI_BattlerHasPhysicalMove(battlerId) ((AI_GetNumMovesWithSplitInBattlerMoveset(battlerId, SPLIT_PHYSICAL) > 0))
#define AI_BattlerHasSpecialMove(battlerId) ((AI_GetNumMovesWithSplitInBattlerMoveset(battlerId, SPLIT_SPECIAL) > 0))





















u32 AI_GetStatChangeScore(u32 attacker, u32 defender, u32 statId, s32 stages, bool32 toUp, bool32 checkEffectsBlock);

#define GetStatUpScore(attacker, defender, statId, stages, checkEffectsBlock) AI_GetStatChangeScore(attacker, defender, statId, stages, TRUE, checkEffectsBlock)
#define GetStatDownScore(attacker, defender, statId, stages, checkEffectsBlock) AI_GetStatChangeScore(attacker, defender, statId, stages, FALSE, checkEffectsBlock)

#endif // GUARD_BATTLE_AI_UTIL_H

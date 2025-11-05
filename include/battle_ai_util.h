#ifndef GUARD_BATTLE_AI_UTIL_H
#define GUARD_BATTLE_AI_UTIL_H

#include "battle_ai.h"

bool32 AI_IsTargetFullyImmuneToMove(struct AIScript *data);
u32 AI_GetNoOfHitsToKOBattler(struct AIScript *data);
u32 AI_GetNoOfHitsToKOBattlerHigherDamage(u32 attacker, u32 defender);
u32 AI_GetNumMovesWithSplitInBattlerMoveset(u32 battlerId, u32 split);
bool32 AI_IsFaster(u32 battler1, u32 battler2);

#define AI_BattlerHasPhysicalMove(battlerId) ((AI_GetNumMovesWithSplitInBattlerMoveset(battlerId, SPLIT_PHYSICAL) > 0))
#define AI_BattlerHasSpecialMove(battlerId) ((AI_GetNumMovesWithSplitInBattlerMoveset(battlerId, SPLIT_SPECIAL) > 0))

#define AI_BattlerHasMoveWithFlagInMoveset(var, battlerId, flag) \
{                                                                \
    u32 i;                                                       \
                                                                 \
    var = FALSE;                                                 \
                                                                 \
    for (i = 0; i < MAX_MON_MOVES; i++)                          \
    {                                                            \
        u32 move = gAIData->thinking[battlerId].moves[i];        \
                                                                 \
        if (move && gBattleMoves[move].flags.flag)               \
        {                                                        \
            var = TRUE;                                          \
            break;                                               \
        }                                                        \
    }                                                            \
}





















u32 AI_GetStatChangeScore(u32 attacker, u32 defender, u32 statId, s32 stages, bool32 toUp, bool32 checkEffectsBlock);
bool32 ShouldAIIncreaseCriticalChance(u32 attacker, u32 defender);
bool32 HasMoveEffectInMoveset(u16 *moveset, u32 moveEffect);
bool32 SideHasMoveEffectInMovesetThatAffectsTarget(u32 attacker, u32 target, u32 moveEffect);
bool32 BadIdeaToBurn(u32 attacker, u32 defender);
u32 GetScoreForInflictBurn(u32 attacker, u32 defender);
bool32 BadIdeaToFreeze(u32 attacker, u32 defender);
u32 GetScoreForFreezeTarget(u32 attacker, u32 defender);
bool32 BadIdeaToParalyze(u32 attacker, u32 defender);
u32 GetScoreForInflictParalyze(u32 attacker, u32 defender);

#define GetStatUpScore(attacker, defender, statId, stages, checkEffectsBlock) AI_GetStatChangeScore(attacker, defender, statId, stages, TRUE, checkEffectsBlock)
#define GetStatDownScore(attacker, defender, statId, stages, checkEffectsBlock) AI_GetStatChangeScore(attacker, defender, statId, stages, FALSE, checkEffectsBlock)

#define AI_BattlerHasMoveEffectInMoveset(battler, moveEffect) HasMoveEffectInMoveset(gAIData->thinking[battler].moves, moveEffect)

#endif // GUARD_BATTLE_AI_UTIL_H

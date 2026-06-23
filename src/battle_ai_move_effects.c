#include "global.h"
#include "battle.h"
#include "battle_ai.h"
#include "battle_ai_move_effects.h"
#include "battle_ai_util.h"
#include "battle_util.h"
#include "item.h"
#include "constants/hold_effects.h"
#include "constants/moves.h"

s8 BattleAIFunc_EffectOHKO(struct AIScript *data, s8 score, u32 caseId)
{
    switch (caseId)
    {
        case AI_CHECK_BAD_MOVE_EFFECT:
            if (data->defAbility == ABILITY_STURDY || !CanOHKOBattler(data->attacker, data->target, data->move))
                CHANGE_SCORE(-10);
            break;
    }
    return score;
}

s8 BattleAIFunc_EffectTwoTurnsAttack(struct AIScript *data, s8 score, u32 caseId)
{
    switch (caseId)
    {
        case AI_CHECK_BAD_MOVE_EFFECT:
            if (data->atkHoldEffect != HOLD_EFFECT_POWER_HERB && !CheckIfCanFireTwoTurnMoveNow(data->attacker, data->move, TRUE)) // Will not attack immediately
            {
                if (gBattleMons[data->attacker].hp <= (gBattleMons[data->attacker].maxHP / 8))
                    CHANGE_SCORE(-10); // Can faint from secondary damage while charging, so don't risk it
                
                if (gBattleMons[data->target].status1.id != STATUS1_PARALYSIS && gBattleMons[data->target].status1.id != STATUS1_FREEZE // Target won't randomly not be able to attack
                && !(gBattleMons[data->target].status1.id == STATUS1_SLEEP && gBattleMons[data->target].status1.counter > 1) // Target is awake (not that they could wake up and protect for the second turn but that's fair and not AI abuse)
                && (gBattleMons[data->target].status2 & STATUS2_CONFUSION) < STATUS2_CONFUSION_TURN(3) // Target wouldn't be confused when the attack would hit
                && !(gBattleMons[data->target].status2 & STATUS2_INFATUATION)) // Target wouldn't miss the attack since they'll never be immobilized by love
                {
                    // Target could protect before the attack hits and the protecting is reasonable
                    if (AI_BattlerHasProtectionMoveInMoveset(data->target) && !(gBattleMons[data->target].hp <= (gBattleMons[data->target].maxHP / 8)))
                        CHANGE_SCORE(-8); // Better not to use this attack, but still can if need be
                    else if (gAIData->thinking[data->attacker].predictedMove)
                    {
                        // Check if attacker can be knocked out before it can attack
                        if (AI_IsFaster(data->attacker, data->target)) // Charge -> Target Attack -> Charge Release
                        {
                            if (AI_GetNoOfHitsToKOBattlerHigherDamage(data->attacker, data->target) < 2)
                                CHANGE_SCORE(-4);
                        }
                        else // Target Attack -> Charge -> Target Attack -> Charge Release
                        {
                            if (AI_GetNoOfHitsToKOBattlerHigherDamage(data->attacker, data->target) <= 2)
                                CHANGE_SCORE(-8); // You're slower so probably not a good idea
                        }
                    }
                }
            }
            break;
    }
    return score;
}

s8 BattleAIFunc_EffectUserAttackUp2(struct AIScript *data, s8 score, u32 caseId)
{
    switch (caseId)
    {
        case AI_CHECK_BAD_MOVE_EFFECT:
            if (AI_WillMoveBeLockedWhenUsed(data))
                CHANGE_SCORE(-10); // Don't set up when potential to be choice locked
            else if (data->atkAbility == ABILITY_CONTRARY || CompareStat(data->attacker, STAT_ATK, MAX_STAT_STAGES, CMP_EQUAL) || !AI_BattlerHasPhysicalMove(data->attacker))
                CHANGE_SCORE(-10);
            break;
        case AI_CHECK_GOOD_MOVE_EFFECT:
            if (data->atkAbility != ABILITY_CONTRARY && AI_GoodIdeaToRaiseAttackAgainstTarget(data, 2))
                CHANGE_SCORE(5);
            break;
    }
    return score;
}

s8 BattleAIFunc_EffectRandomSwitch(struct AIScript *data, s8 score, u32 caseId)
{
    switch (caseId)
    {
        case AI_CHECK_BAD_MOVE_EFFECT:
            /*if (AI_PartnerMoveEffectIsSame(data))
                CHANGE_SCORE(-10); // Don't blow out the same Pokemon twice
            else if (gBattleMons[data->target].hp <= (gBattleMons[data->target].maxHP / 8) || (gBattleMons[data->target].status2 & (STATUS2_NIGHTMARE | STATUS2_CURSED))
                || (gStatuses3[data->target] & (STATUS3_LEECHSEED | STATUS3_ROOTED)) || gBattleMons[data->target].status1.id == STATUS1_TOXIC_POISON
                || gBattleMons[data->target].status1.id == STATUS1_POISON || gDisableStructs[data->target].wrapTurns > 0)
                CHANGE_SCORE(-10); // Don't blow out a Pokemon that HP is low, or is taking bad secondary damage
            else if (data->defAbility == ABILITY_SUCTION_CUPS || data->defAbility == ABILITY_GUARD_DOG || !CanBattlerSwitch(data->target))
                CHANGE_SCORE(-10);*/
            break;
        case AI_CHECK_GOOD_MOVE_EFFECT:
            /*if (!SubstituteBlocksMove(data->attacker, data->target, data->move) && AI_ShouldPhaze())*/
            break;
    }
    return score;
}

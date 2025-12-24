#include "global.h"
#include "battle.h"
#include "battle_ai.h"
#include "battle_ai_move_effects.h"
#include "battle_ai_util.h"
#include "battle_move_effects.h"
#include "battle_secondary_effect.h"
#include "battle_util.h"
#include "item.h"
#include "random.h"

static s8 BattleAI_CheckTargetContactAbility(struct AIScript *data, s8 score);
static s8 BattleAIFunc_Partner(struct AIScript *data, s8 score);
static s8 BattleAI_DamageMoveScoreIncrease(struct AIScript *data, s8 score);

s8 ChangeScore(s8 score, s8 val)
{
    // Double decreases in doubles
    if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && val < 0)
        val *= 2;
    
    return clamp(score + val, 0, 127);
}

////////////////////////////
// AI_FLAG_CHECK_BAD_MOVE //
////////////////////////////

s8 BattleAIFunc_CheckBadMove(struct AIScript *data, s8 score)
{
    if (data->moveTarget != MOVE_TARGET_USER)
    {
        // Powder move check
        if (gBattleMoves[data->move].flags.powderMove && !IsBattlerAffectedBySpore(data->target))
            CHANGE_SCORE(-10);
        
        if (AI_IsTargetFullyImmuneToMove(data))
            RETURN_SCORE(-10);
        
        // Target ability checks
        if (data->defAbility == ABILITY_MAGIC_BOUNCE && gBattleMoves[data->move].flags.magicCoatAffected)
            CHANGE_SCORE(-20);
        
        if (!data->targetingPartner)
        {
            score = BattleAI_CheckTargetContactAbility(data, score);
            
            // Make sure partner isn't going to steal move
            if (data->moveTarget != MOVE_TARGET_ALL_BATTLERS && data->moveTarget != MOVE_TARGET_FOES_AND_ALLY && data->moveTarget != MOVE_TARGET_BOTH
            && IsMoveAffectedByRedirectionEffects(data->attacker, data->move) && CanAbilityAbsorbMove(data->defAbility, data->move, data->moveType, data->attacker, data->target, TRUE))
                RETURN_SCORE(-10); // Only 10 because wouldn't be so bad to hit partner
        }
    }
    
    if (data->moveSplit != SPLIT_STATUS)
    {
        // Primal weather check
        switch (data->moveType)
        {
            case TYPE_FIRE:
                if (IsBattlerWeatherAffected(data->attacker, B_WEATHER_RAIN_PRIMAL))
                    RETURN_SCORE(-20);
                break;
            case TYPE_WATER:
                if (IsBattlerWeatherAffected(data->attacker, B_WEATHER_SUN_PRIMAL))
                    RETURN_SCORE(-20);
                break;
        }
    }
    else
    {
        if (gBattleMoves[gAIData->thinking[data->attacker].partnerMove].effect == EFFECT_HELPING_HAND)
            CHANGE_SCORE(-10); // Don't use a status move if partner wants to help
    }

    // Check move effects
    if (GET_MOVE_MOVEEFFECT_TABLE(data->move).aiFunc != NULL)
        score = GET_MOVE_MOVEEFFECT_TABLE(data->move).aiFunc(data, score, AI_CHECK_BAD_MOVE_EFFECT);

    return score;
}

static s8 BattleAI_CheckTargetContactAbility(struct AIScript *data, s8 score)
{
    switch (data->defAbility)
    {
        case ABILITY_DEFIANT:
            if (data->moveSplit != SPLIT_STATUS && data->moveType == TYPE_DARK && CompareStat(data->target, STAT_ATK, MAX_STAT_STAGES, CMP_NOT_EQUAL)
            && AI_GetNoOfHitsToKOTarget(data) > 2 && AI_BattlerHasPhysicalMove(data->target))
            {
                if (data->targetingPartner)
                    CHANGE_SCORE(6);
                else
                    CHANGE_SCORE(-4);
            }
            break;
        case ABILITY_RATTLED:
            if (data->moveSplit != SPLIT_STATUS && (data->moveType == TYPE_DARK || data->moveType == TYPE_GHOST || data->moveType == TYPE_BUG)
            && CompareStat(data->target, STAT_SPEED, MAX_STAT_STAGES, CMP_NOT_EQUAL) && AI_GetNoOfHitsToKOTarget(data) > 1 && AI_IsFaster(data->attacker, data->target))
            {
                if (data->targetingPartner)
                    CHANGE_SCORE(6);
                else
                {
                    if (AI_GetNoOfHitsToKOTarget(data) <= 2)
                        CHANGE_SCORE(-1); // Risk it, but not best choice because foe might outspeed and strike back harder
                    else
                        CHANGE_SCORE(-9); // Don't risk raising enemy stats
                }
            }
            break;
        case ABILITY_STEAM_ENGINE:
            if (data->moveSplit != SPLIT_STATUS && (data->moveType == TYPE_WATER || data->moveType == TYPE_FIRE)
            && CompareStat(data->target, STAT_SPEED, MAX_STAT_STAGES, CMP_NOT_EQUAL) && AI_GetNoOfHitsToKOTarget(data) > 1 && AI_IsFaster(data->attacker, data->target))
            {
                if (data->targetingPartner)
                    CHANGE_SCORE(6);
                else
                {
                    if (AI_GetNoOfHitsToKOTarget(data) <= 2)
                        CHANGE_SCORE(-5); // Not best choice because foe might outspeed and strike back harder
                    else
                        CHANGE_SCORE(-9); // Don't risk raising enemy stats
                }
            }
            break;
        case ABILITY_ANGER_SHELL:
            if (data->moveSplit != SPLIT_STATUS && AI_GetNoOfHitsToKOTarget(data) <= 2)
            {
                if (data->targetingPartner)
                    CHANGE_SCORE(8);
                else
                    CHANGE_SCORE(-11); // Don't risk raising enemy stats
            }
            break;
    }
    return score;
}

/////////////////////////////
// AI_FLAG_CHECK_GOOD_MOVE //
/////////////////////////////

// Extra args are primary, affectsUser and flags, see DoMoveEffect for more info
#define CHECK_EFFECT_BLOCK(moveEffect, ...) \
    ((CheckSecondaryEffectsBlockers(data->attacker, data->target, data->move, moveEffect, DEFAULT_2(FALSE, __VA_ARGS__), DEFAULT(FALSE, __VA_ARGS__), DEFAULT_3(0, __VA_ARGS__))))

s8 BattleAIFunc_CheckGoodMove(struct AIScript *data, s8 score)
{
    if (IsDoubleBattleForBattler(data->attacker) && data->targetingPartner)
        return BattleAIFunc_Partner(data, score); // Check good move on partner
    
    // Check move effects
    if (GET_MOVE_MOVEEFFECT_TABLE(data->move).aiFunc != NULL)
        score = GET_MOVE_MOVEEFFECT_TABLE(data->move).aiFunc(data, score, AI_CHECK_GOOD_MOVE_EFFECT);
    
    // Check move secondary effects
    if (!ReceiveSheerForceBoost(data->attacker, data->move))
    {
        u32 i;
        
        for (i = 0; i < gBattleMoves[data->move].numAdditionalEffects; i++)
        {
            const struct AdditionalEffect *additionalEffect = &gBattleMoves[data->move].additionalEffects[i];
            u32 effectChance = additionalEffect->chance == 0 ? 100 : CalcSecondaryEffectChance(data->attacker, additionalEffect->moveEffect, additionalEffect->chance);
            
            switch (additionalEffect->moveEffect)
            {
                case MOVE_EFFECT_BURN:
                    if (effectChance >= 75 && !CHECK_EFFECT_BLOCK(additionalEffect->moveEffect, FALSE, additionalEffect->self) && !AI_BadIdeaToBurn(data))
                        CHANGE_SCORE(AI_GetScoreForInflictBurn(data));
                    break;
                case MOVE_EFFECT_FREEZE:
                    if (effectChance >= 75 && !CHECK_EFFECT_BLOCK(additionalEffect->moveEffect, FALSE, additionalEffect->self) && !AI_BadIdeaToFreeze(data))
                        CHANGE_SCORE(AI_GetScoreForInflictFreeze(data));
                    break;
                case MOVE_EFFECT_PARALYSIS:
                    if (effectChance >= 75 && !CHECK_EFFECT_BLOCK(additionalEffect->moveEffect, FALSE, additionalEffect->self) && !AI_BadIdeaToParalyze(data))
                        CHANGE_SCORE(AI_GetScoreForInflictParalysis(data));
                    break;
            }
        }
    }
    
    if (data->moveSplit != SPLIT_STATUS)
        score = BattleAI_DamageMoveScoreIncrease(data, score);
    
    if (gBattleMons[data->attacker].status1.id == STATUS1_FREEZE && gBattleMoves[data->move].flags.thawUser)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            CHANGE_SCORE(20);
        else
            CHANGE_SCORE(10);
    }
    return score;
}

static s8 BattleAI_DamageMoveScoreIncrease(struct AIScript *data, s8 score)
{
    return score;
}

static s8 BattleAIFunc_Partner(struct AIScript *data, s8 score)
{
    if ((!gBattleMoves[gAIData->thinking[data->attacker].partnerMove].flags.usesProtectCounter || gBattleMoves[data->move].flags.forbiddenProtect)
    && data->moveTarget != MOVE_TARGET_BOTH && data->moveTarget != MOVE_TARGET_SELECTED_OPPONENT)
    {
        // Check target ability absorbs move
        switch (CanAbilityAbsorbMove(data->defAbility, data->move, data->moveType, data->attacker, data->target, TRUE))
        {
            case 1:
                if (gBattleMons[data->target].hp <= ((gBattleMons[data->target].maxHP * 2) / 3)) // Only try heal if 2/3 or less HP
                {
                    if (gBattleStruct->battlers[data->target].chosenAction == B_ACTION_USE_ITEM)
                    {
                        u32 item = gBattleBufferB[data->target][1] | (gBattleBufferB[data->target][2] << 8);
                        
                        // Don't heal with move when item is already being used to heal
                        if (ItemId_GetBattleUsage(item) == EFFECT_ITEM_RESTORE_HP)
                            break;
                    }
                    CHANGE_SCORE(15);
                }
                break;
            case 2:
                CHANGE_SCORE(6);
                break;
            case 3:
                if (!gDisableStructs[data->target].flashFireBoost && !AI_HasDamagingMoveWithTypeInMoveset(data->target, data->moveType))
                    CHANGE_SCORE(6);
                break;
        }
        score = BattleAI_CheckTargetContactAbility(data, score);
        
        // Check move effects
        if (GET_MOVE_MOVEEFFECT_TABLE(data->move).aiFunc != NULL)
            score = GET_MOVE_MOVEEFFECT_TABLE(data->move).aiFunc(data, score, AI_CHECK_GOOD_MOVE_EFFECT_ON_PARTNER);
    }
    return score;
}

////////////////////
// AI_FLAG_SAFARI //
////////////////////

s8 BattleAIFunc_Safari(struct AIScript *data, s8 score)
{
    u32 safariFleeRate;

    if (gBattleStruct->safariGoNearCounter)
    {
        safariFleeRate = gBattleStruct->safariEscapeFactor * 2;
        if (safariFleeRate > 20)
            safariFleeRate = 20;
    }
    else if (gBattleStruct->safariPkblThrowCounter)
    {
        safariFleeRate = gBattleStruct->safariEscapeFactor / 4;
        if (safariFleeRate == 0)
            safariFleeRate = 1;
    }
    else
        safariFleeRate = gBattleStruct->safariEscapeFactor;
    
    safariFleeRate *= 5;
    
    if (RandomPercentage(RNG_SAFARI_FLEE_RATE, safariFleeRate))
        gAIData->thinking[data->attacker].action |= (AI_ACTION_DONT_ATTACK | AI_ACTION_FLEE);
    else
        gAIData->thinking[data->attacker].action |= (AI_ACTION_DONT_ATTACK | AI_ACTION_WATCH);
    
    return score;
}

////////////////////
// AI_FLAG_ROAMER //
////////////////////

s8 BattleAIFunc_Roamer(struct AIScript *data, s8 score)
{
    if (CanBattlerFlee(data->attacker))
        gAIData->thinking[data->attacker].action |= (AI_ACTION_DONT_ATTACK | AI_ACTION_FLEE);
    
    return score;
}

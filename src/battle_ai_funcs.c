#include "global.h"
#include "battle.h"
#include "battle_ai.h"
#include "battle_ai_util.h"
#include "battle_move_effects.h"
#include "battle_util.h"
#include "battle_secondary_effect.h"
#include "random.h"
#include "constants/hold_effects.h"
#include "constants/moves.h"

static s8 BattleAIFunc_Partner(struct AIScript *data, s8 score);
static s8 DamageMoveScoreIncrease(struct AIScript *data, s8 score);

////////////////////////////
// SCORE CHANGE FUNCTIONS //
////////////////////////////

static inline s8 ChangeScore(s8 score, s8 val)
{
    return clamp(score + val, 0, 127);
}

#define CHANGE_SCORE(val) score = ChangeScore(score, val)
#define RETURN_SCORE(val) return ChangeScore(score, val)

////////////////////////////
// AI_FLAG_CHECK_BAD_MOVE //
////////////////////////////

// Extra args are primary, affectsUser and flags, see DoMoveEffect for more info
#define CHECK_EFFECT_BLOCK(moveEffect, ...) \
    ((CheckSecondaryEffectsBlockers(data->attacker, data->target, data->move, moveEffect, DEFAULT_2(FALSE, __VA_ARGS__), DEFAULT(FALSE, __VA_ARGS__), DEFAULT_3(0, __VA_ARGS__))))

// set burn:
//  + if def has physical move
//  + if user has a protection move
//  + if user has a move effect that benefit from that, like Hex
// 
// set freeze:
//  + if user has a move effect that benefit from that, like Hex
// 
// set paralyze:
//  + if user is lower than the def
//  + if user has a protection move
//  + if user has a move effect that benefit from that, like Hex

// Increase score based on power, speed, effectiveness, accuracy, crit stages and maybe num of hits to ko the target
// Check substitute
// Make ai recognize Protean and Libero, maybe on the damage calc
// Make ai try predict an protection move

s8 BattleAIFunc_CheckBadMove(struct AIScript *data, s8 score)
{
    if (data->moveTarget != MOVE_TARGET_USER)
    {
        // Powder move check
        if (gBattleMoves[data->move].flags.powderMove && !IsBattlerAffectedBySpore(data->target))
            CHANGE_SCORE(-10);
        
        if (AI_IsTargetFullyImmuneToMove(data))
            RETURN_SCORE(-10);
        
        // Target ability check
        switch (data->defAbility)
        {
            case ABILITY_MAGIC_BOUNCE:
                if (gBattleMoves[data->move].flags.magicCoatAffected)
                    CHANGE_SCORE(-20);
                break;
            case ABILITY_DEFIANT:
                if (data->moveSplit != SPLIT_STATUS && !data->targetingPartner && data->moveType == TYPE_DARK && CompareStat(data->target, STAT_ATK, MAX_STAT_STAGES, CMP_NOT_EQUAL)
                && AI_GetNoOfHitsToKOBattler(data) > 2 && AI_BattlerHasPhysicalMove(data->target))
                    CHANGE_SCORE(-4);
                break;
            case ABILITY_RATTLED:
                if (data->moveSplit != SPLIT_STATUS && !data->targetingPartner && (data->moveType == TYPE_DARK || data->moveType == TYPE_GHOST || data->moveType == TYPE_BUG)
                && CompareStat(data->target, STAT_SPEED, MAX_STAT_STAGES, CMP_NOT_EQUAL) && AI_GetNoOfHitsToKOBattler(data) > 1 && AI_IsFaster(data->attacker, data->target))
                {
                    if (AI_GetNoOfHitsToKOBattler(data) <= 2)
                        CHANGE_SCORE(-1); // Risk it, but not best choice because foe might outspeed and strike back harder
                    else
                        CHANGE_SCORE(-9); // Don't risk raising enemy stats
                }
                break;
            case ABILITY_STEAM_ENGINE:
                if (data->moveSplit != SPLIT_STATUS && !data->targetingPartner && (data->moveType == TYPE_WATER || data->moveType == TYPE_FIRE)
                && CompareStat(data->target, STAT_SPEED, MAX_STAT_STAGES, CMP_NOT_EQUAL) && AI_GetNoOfHitsToKOBattler(data) > 1 && AI_IsFaster(data->attacker, data->target))
                {
                    if (AI_GetNoOfHitsToKOBattler(data) <= 2)
                        CHANGE_SCORE(-5); // Not best choice because foe might outspeed and strike back harder
                    else
                        CHANGE_SCORE(-9); // Don't risk raising enemy stats
                }
                break;
            case ABILITY_ANGER_SHELL:
                if (data->moveSplit != SPLIT_STATUS && !data->targetingPartner && AI_GetNoOfHitsToKOBattler(data) <= 2)
                    CHANGE_SCORE(-11); // Don't risk raising enemy stats
                break;
        }
        
        // Make sure partner isn't going to steal move
        if (!data->targetingPartner && data->moveTarget != MOVE_TARGET_ALL_BATTLERS && data->moveTarget != MOVE_TARGET_FOES_AND_ALLY && data->moveTarget != MOVE_TARGET_BOTH
        && IsMoveAffectedByRedirectionEffects(data->attacker, data->move) && CanAbilityAbsorbMove(data->defAbility, data->move, data->moveType, data->attacker, data->target, TRUE))
            RETURN_SCORE(-10); // Only 10 because wouldn't be so bad to hit partner
    }
    
    // Primal weather check
    if (data->moveSplit != SPLIT_STATUS)
    {
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
    
    // Check move effects
    switch (gBattleMoves[data->move].effect)
    {
        case EFFECT_OHKO:
            if ((GetActiveGimmick(data->target) == GIMMICK_DYNAMAX && !HasRaidShields(data->target)) || data->defAbility == ABILITY_STURDY
            || !KanOHKOBattler(data->attacker, data->target, data->move, FALSE))
                CHANGE_SCORE(-10);
            break;
    }
    
    if (gBattleMoves[gAIData->thinking[data->attacker].partnerMove].effect == EFFECT_HELPING_HAND && data->moveSplit != SPLIT_STATUS)
        CHANGE_SCORE(-10); // Don't use a status move if partner wants to help
    
    return score;
}

/////////////////////////////
// AI_FLAG_CHECK_GOOD_MOVE //
/////////////////////////////

s8 BattleAIFunc_CheckGoodMove(struct AIScript *data, s8 score)
{
    if (IsDoubleBattleForBattler(data->attacker) && data->targetingPartner)
        return BattleAIFunc_Partner(data, score);
    
    switch (gBattleMoves[data->move].effect)
    {
        case EFFECT_HIT:
            break;
    }
    
    if (data->moveSplit != SPLIT_STATUS)
        score = DamageMoveScoreIncrease(data, score);
    
    if (gBattleMons[data->attacker].status1.id == STATUS1_FREEZE && gBattleMoves[data->move].flags.thawUser)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            CHANGE_SCORE(20);
        else
            CHANGE_SCORE(10);
    }
    return score;
}

static s8 DamageMoveScoreIncrease(struct AIScript *data, s8 score)
{
    return score;
}

static s8 BattleAIFunc_Partner(struct AIScript *data, s8 score)
{
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

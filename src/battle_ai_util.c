#include "global.h"
#include "battle.h"
#include "battle_ai_util.h"
#include "battle_move_effects.h"
#include "battle_secondary_effect.h"
#include "battle_util.h"
#include "item.h"
#include "random.h"
#include "util.h"
#include "constants/abilities.h"
#include "constants/hold_effects.h"
#include "constants/moves.h"

void AI_RecordLastUsedMoveByBattler(u32 battlerId, u32 move)
{
    u32 i;
    
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (gAIData->thinking[battlerId].usedMoves[i] == move) // Move already recorded, don't record again
            break;
            
        if (gAIData->thinking[battlerId].usedMoves[i] == MOVE_NONE) // Move hasn't been used yet, record move
        {
            gAIData->thinking[battlerId].usedMoves[i] = move;
            return;
        }
    }
}

void AI_ClearRecordedMovesByBattler(u32 battlerId)
{
    u32 i;
    
    for (i = 0; i < MAX_MON_MOVES; i++)
        gAIData->thinking[battlerId].usedMoves[i] = MOVE_NONE;
}

bool32 AI_IsTargetFullyImmuneToMove(struct AIScript *data)
{
    if (gAIData->thinking[data->attacker].targets[data->target].effectiveness[data->moveSlot] == TYPE_MUL_NO_EFFECT)
        return TRUE;
    else if (CanAbilityBlockMove(data->move, data->attacker, data->target, TRUE))
        return TRUE;
    else if (!data->targetingPartner && CanAbilityAbsorbMove(data->defAbility, data->move, data->moveType, data->attacker, data->target, TRUE))
        return TRUE;
    else
        return FALSE;
}

static u32 AI_GetNoOfHitsToKOBattlerDmg(s32 dmg, u32 battler)
{
    if (!dmg)
        return 0xFFFFFFFF;
    
    return gBattleMons[battler].hp / (dmg + 1) + 1;
}

u32 AI_GetNoOfHitsToKOTarget(struct AIScript *data)
{
    return AI_GetNoOfHitsToKOBattlerDmg(gAIData->thinking[data->attacker].targets[data->target].simulatedDmg[data->moveSlot], data->target);
}

u32 AI_GetNoOfHitsToKOAttacker(struct AIScript *data)
{
    u32 moveSlot = FindMoveSlotInBattlerMoveset(data->target, gAIData->thinking[data->target].predictedMove);
    
    if (moveSlot == MAX_MON_MOVES)
        return 0xFFFFFFFF;
    
    return AI_GetNoOfHitsToKOBattlerDmg(gAIData->thinking[data->target].targets[data->attacker].simulatedDmg[moveSlot], data->target);
}

u32 AI_GetNoOfHitsToKOBattlerHigherDamage(u32 attacker, u32 defender)
{
    return AI_GetNoOfHitsToKOBattlerDmg(gAIData->thinking[attacker].targets[defender].higherDamage, defender);
}

bool32 AI_IsFaster(u32 battler1, u32 battler2)
{
    return (gAIData->thinking[battler1].totalSpeed > gAIData->thinking[battler2].totalSpeed);
}

u32 AI_GetNumMovesWithSplitInBattlerMoveset(u32 battlerId, u32 split)
{
    u32 i, count = 0;

    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (gAIData->thinking[battlerId].moves[i] && GetBattleMoveSplit(gAIData->thinking[battlerId].moves[i]) == split)
            ++count;
    }
    return count;
}

bool32 AI_HasDamagingMoveWithTypeInMoveset(u32 battlerId, u32 type)
{
    u32 i;

    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (gAIData->thinking[battlerId].moves[i] && GetBattleMoveSplit(gAIData->thinking[battlerId].moves[i]) != SPLIT_STATUS
        && gAIData->thinking[battlerId].moveTypes[i] == type)
            return TRUE;
    }
    return FALSE;
}

bool32 AI_BattlerHasMoveWithAdditionalEffectInMoveset(u32 battlerId, u32 moveEffect)
{
    u32 i;
    
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (gAIData->thinking[battlerId].moves[i] && MoveHasMoveEffect(gAIData->thinking[battlerId].moves[i], moveEffect, FALSE))
            return TRUE;
    }
    return FALSE;
}

bool32 AI_BattlerHasMoveEffectInMoveset(u32 battlerId, u32 moveEffect)
{
    u32 i;
    
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (gAIData->thinking[battlerId].moves[i] && gBattleMoves[gAIData->thinking[battlerId].moves[i]].effect == moveEffect)
            return TRUE;
    }
    return FALSE;
}

bool32 AI_BattlerHasMoveEffectInMovesetThatAffectsTarget(u32 attacker, u32 target, u32 moveEffect)
{
    u32 i;
    
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (gAIData->thinking[attacker].moves[i] && gBattleMoves[gAIData->thinking[attacker].moves[i]].effect == moveEffect
        && gAIData->thinking[attacker].targets[target].effectiveness[i] != TYPE_MUL_NO_EFFECT)
            return TRUE;
    }
    return FALSE;
}

bool32 AI_SideHasMoveEffectInMovesetThatAffectsTarget(u32 attacker, u32 target, u32 moveEffect)
{
    u32 i;
    
    for (i = 0; i < NUM_BATTLERS_PER_SIDE; i++, attacker = BATTLE_PARTNER(attacker))
    {
        if (IsBattlerAlive(attacker) && AI_BattlerHasMoveEffectInMovesetThatAffectsTarget(attacker, target, moveEffect))
            return TRUE;
    }
    return FALSE;
}

bool32 AI_WillMoveBeLockedWhenUsed(struct AIScript *data)
{
    return (data->atkHoldEffect == HOLD_EFFECT_CHOICE_ITEM || data->atkAbility == ABILITY_GORILLA_TACTICS || gDisableStructs[data->attacker].encoreTimer > 0);
}

bool32 AI_BattlerHasThawingUserMoveInMoveset(u32 battlerId)
{
    u32 i;
    
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (gAIData->thinking[battlerId].moves[i] && gBattleMoves[gAIData->thinking[battlerId].moves[i]].flags.thawUser)
            return TRUE;
    }
    return FALSE;
}

bool32 AI_BattlerHasProtectionMoveInMoveset(u32 battlerId)
{
    u32 i;
    
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (gAIData->thinking[battlerId].moves[i] && gBattleMoves[gAIData->thinking[battlerId].moves[i]].flags.usesProtectCounter)
            return TRUE;
    }
    return FALSE;
}

static bool32 AI_IsMovePredictionPhazingMove(u32 attacker, u32 target)
{
    if (GetBattlerSide(attacker) == B_SIDE_PLAYER && (AI_BattlerHasMoveEffectInMoveset(attacker, EFFECT_NORMALISE_FIELD_BUFFS)
    || AI_BattlerHasMoveWithAdditionalEffectInMoveset(attacker, MOVE_EFFECT_CLEAR_SMOG)))
        return TRUE; // Assume the player knows the foe has a setup move and will try to cheese them with Hazing
    
    switch (gBattleMoves[gAIData->thinking[attacker].predictedMove].effect)
    {
        case EFFECT_NORMALISE_FIELD_BUFFS:
            return TRUE;
        case EFFECT_RANDOM_SWITCH:
        case EFFECT_HIT_SWITCH_TARGET:
            return (GetActiveGimmick(target) != GIMMICK_DYNAMAX);
    }
    
    if (MoveHasMoveEffect(gAIData->thinking[attacker].predictedMove, MOVE_EFFECT_CLEAR_SMOG, FALSE))
        return TRUE;
    
    return FALSE;
}

static bool32 AI_IsMovePredictionHighAccSleepingMove(u32 attacker, u32 target)
{
    u32 predictedMove = gAIData->thinking[attacker].predictedMove;
    
    if (gBattleMoves[predictedMove].effect == EFFECT_SLEEP || gBattleMoves[predictedMove].effect == EFFECT_YAWN)
        return (AccuracyCalcHelper(attacker, target, predictedMove, NULL, NULL) == ACCURACY_STATE_HITS || CalcMoveTotalAccuracy(attacker, target, predictedMove) >= 80);
    
    return FALSE;
}

static bool32 AI_HasUsedMoveWithEffect(u32 battlerId, u32 moveEffect)
{
    u32 i;
    
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (!gAIData->thinking[battlerId].usedMoves[i])
            break;
        
        if (gBattleMoves[gAIData->thinking[battlerId].usedMoves[i]].effect == moveEffect)
            return TRUE;
    }
    return FALSE;
}

static bool32 AI_HasUsedMoveWithAdditionalEffectHigherThan(u32 battlerId, u32 moveEffect, u32 chance)
{
    u32 i, j;
    
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        u32 move = gAIData->thinking[battlerId].usedMoves[i];
        if (!move)
            break;
        
        for (j = 0; i < gBattleMoves[move].numAdditionalEffects; j++)
        {
            const struct AdditionalEffect *additionalEffect = &gBattleMoves[move].additionalEffects[i];
            
            if (additionalEffect->moveEffect == moveEffect)
            {
                u32 effectChance = additionalEffect->chance == 0 ? 100 : CalcSecondaryEffectChance(battlerId, additionalEffect->moveEffect, additionalEffect->chance);
                
                if (effectChance >= chance)
                    return TRUE;
            }
        }
    }
    return FALSE;
}

bool32 AI_PartnerMoveEffectIsSameNoTarget(struct AIScript *data)
{
    if (IsBattlerAlive(BATTLE_PARTNER(data->attacker)) && gBattleMoves[data->move].effect == gBattleMoves[gAIData->thinking[data->attacker].partnerMove].effect
    && gBattleStruct->battlers[BATTLE_PARTNER(data->attacker)].chosenMove != MOVE_NONE)
        return TRUE;
    
    return FALSE;
}

bool32 AI_PartnerMoveEffectIsSame(struct AIScript *data)
{
    return (AI_PartnerMoveEffectIsSameNoTarget(data) && gBattleStruct->battlers[BATTLE_PARTNER(data->attacker)].moveTarget == data->target);
}

bool32 AI_BadIdeaToBurn(struct AIScript *data)
{
    if (CanBeBurned(data->attacker, data->target, 0) != STATUS_CHANGE_WORKED)
        return TRUE;
    
    switch (data->defAbility)
    {
        case ABILITY_SHED_SKIN:
        case ABILITY_QUICK_FEET:
            return TRUE;
        case ABILITY_MAGIC_GUARD:
            if (!AI_BattlerHasMoveEffectInMovesetThatAffectsTarget(data->attacker, data->target, EFFECT_HEX))
                return TRUE;
            break;
        case ABILITY_SYNCHRONIZE:
            if (CanBeBurned(data->target, data->attacker, 0) == STATUS_CHANGE_WORKED && !AI_GoodIdeaToBurnSelf(data))
                return TRUE;
            break;
        case ABILITY_MARVEL_SCALE:
            if (AI_BattlerHasPhysicalMove(data->attacker))
                return TRUE;
            break;
        case ABILITY_NATURAL_CURE:
            if (CanBattlerSwitchOut(data->target, TRUE))
                return TRUE;
            break;
        case ABILITY_FLARE_BOOST:
            if (AI_BattlerHasSpecialMove(data->target))
                return TRUE;
            break;
        case ABILITY_GUTS:
            if (AI_BattlerHasPhysicalMove(data->target))
                return TRUE;
            break;
        case ABILITY_HYDRATION:
            if ((GetBattlerWeatherFlags(data->target) & B_WEATHER_RAIN_ANY) && gBattleStruct->weatherDuration != 1) // Not expire this turn
                return TRUE;
            break;
    }
    
    if (IsBattlerAlive(BATTLE_PARTNER(data->target)))
    {
        switch (GetBattlerAbility(BATTLE_PARTNER(data->target)))
        {
            case ABILITY_HEALER:
                return TRUE;
        }
    }
    
    if (AI_BattlerHasMoveEffectInMoveset(data->target, EFFECT_FACADE) || AI_BattlerHasMoveEffectInMoveset(data->target, EFFECT_PSYCHO_SHIFT))
        return TRUE;

    return FALSE;
}

bool32 AI_GoodIdeaToBurnSelf(struct AIScript *data)
{
    if (CanBeBurned(data->attacker, data->attacker, 0) == STATUS_CHANGE_WORKED)
    {
        switch (data->atkAbility)
        {
            case ABILITY_QUICK_FEET:
            case ABILITY_HEATPROOF:
            case ABILITY_MAGIC_GUARD:
                return TRUE;
            case ABILITY_FLARE_BOOST:
                if (AI_BattlerHasSpecialMove(data->attacker))
                    return TRUE;
                break;
            case ABILITY_GUTS:
                if (AI_BattlerHasPhysicalMove(data->attacker))
                    return TRUE;
                break;
        }
        
        if (AI_BattlerHasMoveEffectInMoveset(data->attacker, EFFECT_FACADE) || AI_BattlerHasMoveEffectInMoveset(data->attacker, EFFECT_PSYCHO_SHIFT))
            return TRUE;
    }
    return FALSE;
}

s8 AI_GetScoreForInflictBurn(struct AIScript *data)
{
    if (AI_BattlerHasPhysicalMove(data->target))
        return 11;
    else if (GetBattleMoveSplit(gAIData->thinking[data->target].predictedMove) == SPLIT_PHYSICAL && AI_GetNoOfHitsToKOAttacker(data) <= 1)
        return 7; // If the enemy can kill with a physical move, try burning them so they can't anymore
    else if (AI_SideHasMoveEffectInMovesetThatAffectsTarget(data->attacker, data->target, EFFECT_HEX) || AI_BattlerHasPhysicalMove(data->target)
        || AI_BattlerHasMoveEffectInMoveset(data->attacker, EFFECT_INFERNAL_PARADE))
        return 6;
    else
        return 5;
}

bool32 AI_BadIdeaToFreeze(struct AIScript *data)
{
    if (!CanBeFrozen(data->attacker, data->target, 0))
        return TRUE;
    
    switch (data->defAbility)
    {
        case ABILITY_SYNCHRONIZE:
            if (CanBeFrozen(data->target, data->attacker, 0))
                return TRUE;
            break;
        case ABILITY_NATURAL_CURE:
            if (CanBattlerSwitchOut(data->target, TRUE))
                return TRUE;
            break;
    }
    
    switch (data->defHoldEffect)
    {
        case HOLD_EFFECT_CURE_FRZ:
        case HOLD_EFFECT_CURE_STATUS:
            return TRUE;
    }
    
    if (AI_BattlerHasThawingUserMoveInMoveset(data->target))
        return TRUE;

    return FALSE;
}

s8 AI_GetScoreForInflictFreeze(struct AIScript *data)
{
    return 8;
}

bool32 AI_BadIdeaToParalyze(struct AIScript *data)
{
    if (CanBeParalyzed(data->attacker, data->target, 0) != STATUS_CHANGE_WORKED)
        return TRUE;
    
    switch (data->defAbility)
    {
        case ABILITY_SHED_SKIN:
        case ABILITY_QUICK_FEET:
            return TRUE;
        case ABILITY_SYNCHRONIZE:
            if (CanBeParalyzed(data->target, data->attacker, 0) == STATUS_CHANGE_WORKED && !AI_GoodIdeaToParalyzeSelf(data))
                return TRUE;
            break;
        case ABILITY_MARVEL_SCALE:
            if (AI_BattlerHasPhysicalMove(data->attacker))
                return TRUE;
            break;
        case ABILITY_NATURAL_CURE:
            if (CanBattlerSwitchOut(data->target, TRUE))
                return TRUE;
            break;
        case ABILITY_GUTS:
            if (AI_BattlerHasPhysicalMove(data->target))
                return TRUE;
            break;
        case ABILITY_HYDRATION:
            if ((GetBattlerWeatherFlags(data->target) & B_WEATHER_RAIN_ANY) && gBattleStruct->weatherDuration != 1) // Not expire this turn
                return TRUE;
            break;
    }
    
    if (IsBattlerAlive(BATTLE_PARTNER(data->target)))
    {
        switch (GetBattlerAbility(BATTLE_PARTNER(data->target)))
        {
            case ABILITY_HEALER:
                return TRUE;
        }
    }
    
    if (AI_BattlerHasMoveEffectInMoveset(data->target, EFFECT_FACADE) || AI_BattlerHasMoveEffectInMoveset(data->target, EFFECT_PSYCHO_SHIFT)
    || AI_BattlerHasMoveEffectInMoveset(data->target, EFFECT_REST))
        return TRUE;
    
    return FALSE;
}

bool32 AI_GoodIdeaToParalyzeSelf(struct AIScript *data)
{
    if (CanBeParalyzed(data->attacker, data->attacker, 0) == STATUS_CHANGE_WORKED)
    {
        switch (data->atkAbility)
        {
            case ABILITY_MARVEL_SCALE:
            case ABILITY_QUICK_FEET:
                return TRUE;
            case ABILITY_GUTS:
                if (AI_BattlerHasPhysicalMove(data->attacker))
                    return TRUE;
                break;
        }
        
        if (AI_BattlerHasMoveEffectInMoveset(data->attacker, EFFECT_FACADE) || AI_BattlerHasMoveEffectInMoveset(data->attacker, EFFECT_PSYCHO_SHIFT))
            return TRUE;
    }
    return FALSE;
}

s8 AI_GetScoreForInflictParalysis(struct AIScript *data)
{
    s8 score = AI_GetScoreForSpeedControl(data);
    
    if (score == 0)
    {
        if ((!AI_IsFaster(data->attacker, data->target) && gAIData->thinking[data->target].totalSpeed / 2 < gAIData->thinking[data->attacker].totalSpeed) // You'll go first after paralyzing foe
        || AI_SideHasMoveEffectInMovesetThatAffectsTarget(data->attacker, data->target, EFFECT_HEX) || (gBattleMons[data->target].status2 & (STATUS2_CONFUSION | STATUS2_INFATUATION))
        || AI_BattlerHasMoveEffectInMoveset(data->attacker, EFFECT_FAKE_OUT) || AI_BattlerHasMoveWithAdditionalEffectInMoveset(data->attacker, MOVE_EFFECT_FLINCH))
            score = 6;
        else
            score = 5;
    }
    return score;
}

s8 AI_GetScoreForSpeedControl(struct AIScript *data)
{
    if (!(gFieldStatus & STATUS_FIELD_TRICK_ROOM) && (!AI_IsFaster(data->attacker, data->target) || (IsBattlerAlive(BATTLE_PARTNER(data->attacker))
    && !AI_IsFaster(BATTLE_PARTNER(data->attacker), data->target))))
        return 11;
    else
        return 0;
}

static bool32 AI_BadIdeaToRaiseStatAgainst(u32 attacker, u32 target, bool32 checkDefAbility)
{
    if (AI_IsMovePredictionPhazingMove(target, attacker))
        return TRUE;
    else if (AI_IsMovePredictionHighAccSleepingMove(target, attacker) && AI_IsFaster(target, attacker)) // Will put AI to sleep after it sets up
        return TRUE;
    else if (checkDefAbility && !(gBattleMons[target].hp <= (gBattleMons[target].maxHP / 8)) && GetBattlerAbility(target) == ABILITY_UNAWARE) // Don't set up if the boosts will just be ignored
        return TRUE;
    else
        return FALSE;
}

bool32 AI_GoodIdeaToRaiseAttackAgainstTarget(struct AIScript *data, u32 stages)
{
    return (!AI_BadIdeaToRaiseAttackAgainst(data->attacker, data->target, stages, TRUE) && AI_BattlerHasPhysicalMove(data->attacker));
}

bool32 AI_BadIdeaToRaiseAttackAgainst(u32 attacker, u32 target, u32 stages, bool32 checkPartner)
{
    if (AI_BadIdeaToRaiseStatAgainst(attacker, target, checkPartner) || AI_HasUsedMoveWithEffect(target, EFFECT_ATTACK_DOWN_2))
        return TRUE;
    else if (checkPartner && AI_BattlerHasMoveEffectInMoveset(target, EFFECT_KINGS_SHIELD) && IsMoveMakingContact(attacker, gAIData->thinking[attacker].targets[target].higherDamageMove))
        return TRUE;
    
    if (stages <= 1)
    {
        if (AI_HasUsedMoveWithEffect(target, EFFECT_ATTACK_DOWN) || AI_HasUsedMoveWithEffect(target, EFFECT_TICKLE))
            return TRUE;
        else if (AI_HasUsedMoveWithEffect(target, EFFECT_VENOM_DRENCH) && (gBattleMons[attacker].status1.id == STATUS1_POISON || gBattleMons[attacker].status1.id == STATUS1_TOXIC_POISON))
            return TRUE;
        else if (AI_HasUsedMoveWithAdditionalEffectHigherThan(target, MOVE_EFFECT_DEF_MINUS_1, 75))
            return TRUE;
    }
    
    // Check partner too so the AI doesn't raise stats against one foe because it's okay
    if (checkPartner && IsBattlerAlive(BATTLE_PARTNER(target)))
        return AI_BadIdeaToRaiseAttackAgainst(attacker, BATTLE_PARTNER(target), stages, FALSE);
    
    return FALSE;
}

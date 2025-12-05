#include "global.h"
#include "battle.h"
#include "battle_ai.h"
#include "battle_ai_util.h"
#include "battle_controllers.h"
#include "battle_damage_calc.h"
#include "form_change.h"
#include "item.h"
#include "util.h"

////////////
// SWITCH //
////////////

static u32 FindMonThatAbsorbsOpponentsMove(u32 battlerId, u8 *viableMons, u32 availableToSwitch, u32 opposingBattler);
static u32 TryPassOnWish(u32 battlerId, u8 *viableMons, u32 availableToSwitch, u32 opposingBattler);
static u32 ShouldSwitchIfPerishSong(u32 battlerId, u8 *viableMons, u32 availableToSwitch, u32 opposingBattler);
static u32 ShouldSwitchIfWonderGuard(u32 battlerId, u8 *viableMons, u32 availableToSwitch, u32 opposingBattler);
static u32 ShouldSwitchIfOnlyBadMovesLeft(u32 battlerId, u8 *viableMons, u32 availableToSwitch, u32 opposingBattler);
static u32 ShouldSwitchIfNaturalCureOrRegenerator(u32 battlerId, u8 *viableMons, u32 availableToSwitch, u32 opposingBattler);
static u32 ShouldSwitchWhenYawned(u32 battlerId, u8 *viableMons, u32 availableToSwitch, u32 opposingBattler);

static u32 (*const sAIShouldSwitchFuncs[])(u32, u8*, u32, u32) =
{
    FindMonThatAbsorbsOpponentsMove,
    TryPassOnWish,
    ShouldSwitchIfPerishSong,
    ShouldSwitchIfWonderGuard,
    ShouldSwitchIfOnlyBadMovesLeft,
    ShouldSwitchIfNaturalCureOrRegenerator,
    ShouldSwitchWhenYawned,
};

static bool32 CheckMonAlreadyInBankForSwitching(u32 partyId, u32 battlerIn1, u32 battlerIn2)
{
    if (partyId != gBattleStruct->battlers[battlerIn1].monToSwitchIntoId && partyId != gBattleStruct->battlers[battlerIn2].monToSwitchIntoId)
        return FALSE;
    
    return TRUE;
}

u32 GetViableMonsToSwitchInto(u32 battlerId, u8 *viableMons)
{
    return CountUsablePartyMons(battlerId, viableMons, CheckMonAlreadyInBankForSwitching);
}

#define NO_SWITCH PARTY_SIZE + 1

bool32 BattleAI_ShouldSwitch(u32 battlerId)
{
    if (CanBattlerFlee(battlerId))
    {
        u8 viableMons[PARTY_SIZE];
        u32 i, availableToSwitch = GetViableMonsToSwitchInto(battlerId, viableMons);
        
        if (availableToSwitch > 0)
        {
            u32 opposingBattler = BATTLE_OPPOSITE(battlerId);
            
            for (i = 0; i < ARRAY_COUNT(sAIShouldSwitchFuncs); i++)
            {
                u32 id = sAIShouldSwitchFuncs[i](battlerId, viableMons, availableToSwitch, opposingBattler);
                
                if (id != NO_SWITCH)
                {
                    gBattleStruct->battlers[battlerId].AI_monToSwitchIntoId = id;
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

static u32 GetMostSuitableMonWithTypeMatchup(u32 battlerId, u32 opposingBattler)
{
    u8 viableMons[PARTY_SIZE];
    u32 i, j, id, move, dmg, bestDmg, bestMonId, viableMonsCount = GetViableMonsToSwitchInto(battlerId, viableMons);
    struct Pokemon *party;
    
    bestDmg = 0;
    bestMonId = PARTY_SIZE;
    
    // Find mon with a good type matchup and moves
    party = GetBattlerParty(battlerId);
    for (i = 0; i < viableMonsCount; i++)
    {
        id = viableMons[i];
        dmg = AI_GetSwitchInTypeMatchup(&party[id], opposingBattler);
        
        // If no mon has good types and moves, switch to the best one found
        if (bestDmg < dmg)
        {
            bestDmg = dmg;
            bestMonId = id;
        }
        
        if (dmg <= TYPE_MUL_NORMAL)
        {
            for (j = 0; j < MAX_MON_MOVES; j++)
            {
                move = GetMonData(&party[id], MON_DATA_MOVE1 + j);
                
                if (move && AI_TypeCalc(&party[id], move, opposingBattler) >= TYPE_MUL_SUPER_EFFECTIVE)
                    return id;
            }
        }
    }
    return bestMonId;
}

u32 GetMostSuitableMonToSwitchInto(u32 battlerId)
{
    if (gBattleStruct->battlers[battlerId].monToSwitchIntoId != PARTY_SIZE)
        return gBattleStruct->battlers[battlerId].monToSwitchIntoId;
    else
    {
        u32 opposingBattler = BATTLE_OPPOSITE(battlerId);
        
        if (IsDoubleBattleForBattler(opposingBattler))
        {
            if (!IsBattlerAlive(opposingBattler))
                opposingBattler = BATTLE_PARTNER(opposingBattler);
        }
        return GetMostSuitableMonWithTypeMatchup(battlerId, opposingBattler);
    }
}

static u32 FindMonThatAbsorbsOpponentsMove(u32 battlerId, u8 *viableMons, u32 availableToSwitch, u32 opposingBattler)
{
    u32 i, predictedMove;
    struct Pokemon *party;
    
    if (gBattleMons[battlerId].statStages[STAT_EVASION] >= (DEFAULT_STAT_STAGES + (DEFAULT_STAT_STAGES / 2)))
        return NO_SWITCH;
    
    predictedMove = gAIData->thinking[opposingBattler].predictedMove;
    
    if (!predictedMove || GetBattleMoveSplit(predictedMove) == SPLIT_STATUS)
        return NO_SWITCH;
    
    party = GetBattlerParty(battlerId);
    for (i = 0; i < availableToSwitch; i++)
    {
        if (CanAbilityAbsorbMove(GetMonAbility(&party[viableMons[i]]), predictedMove, gBattleMoves[predictedMove].type, opposingBattler, battlerId, TRUE))
            return viableMons[i];
    }
    return NO_SWITCH;
}

static u32 TryPassOnWish(u32 battlerId, u8 *viableMons, u32 availableToSwitch, u32 opposingBattler)
{
    if (gBattleStruct->battlers[battlerId].wishCounter)
    {
        u32 i;
        struct Pokemon *party;
        
        if (gBattleMons[battlerId].hp < gBattleMons[battlerId].maxHP / 2)
        {
            if (AI_GetNoOfHitsToKOBattlerHigherDamage(opposingBattler, battlerId) > 1 || AI_BattlerHasProtectionMoveInMoveset(battlerId))
                return NO_SWITCH;
        }
        
        party = GetBattlerParty(battlerId);
        for (i = 0; i < availableToSwitch; i++)
        {
            u32 id = viableMons[i];
            
            if (GetMonData(&party[id], MON_DATA_HP) < GetMonData(&party[id], MON_DATA_MAX_HP) / 2)
                return id;
        }
    }
    return NO_SWITCH;
}

static u32 ShouldSwitchIfPerishSong(u32 battlerId, u8 *viableMons, u32 availableToSwitch, u32 opposingBattler)
{
    if ((gStatuses3[battlerId] & STATUS3_PERISH_SONG) && gDisableStructs[battlerId].perishSongTimer == 0)
        return PARTY_SIZE;
    return NO_SWITCH;
}

static u32 ShouldSwitchIfWonderGuard(u32 battlerId, u8 *viableMons, u32 availableToSwitch, u32 opposingBattler)
{
    struct Pokemon *party;
    u32 i, j, id, move;
    u16 flags;
    
    if (IsDoubleBattleForBattler(battlerId) || GetBattlerAbility(battlerId) != ABILITY_WONDER_GUARD)
        return NO_SWITCH;
    
    // Check battler has a super effective move
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        move = gBattleMons[battlerId].moves[i];
        
        if (!move || GetBattleMoveSplit(move) == SPLIT_STATUS)
            continue;
        
        if (TypeCalc(move, GetBattlerMoveType(battlerId, move), battlerId, opposingBattler, FALSE, FALSE, &flags) >= TYPE_MUL_SUPER_EFFECTIVE)
            return NO_SWITCH;
    }
    
    // Check party mon has a super effective move
    party = GetBattlerParty(battlerId);
    for (i = 0; i < availableToSwitch; i++)
    {
        id = viableMons[i];
        
        for (j = 0; j < MAX_MON_MOVES; j++)
        {
            move = GetMonData(&party[id], MON_DATA_MOVE1 + j);
            
            if (move && GetBattleMoveSplit(move) != SPLIT_STATUS && AI_TypeCalc(&party[id], move, opposingBattler) >= TYPE_MUL_SUPER_EFFECTIVE)
                return id;
        }
    }
    return NO_SWITCH;
}

static u32 ShouldSwitchIfOnlyBadMovesLeft(u32 battlerId, u8 *viableMons, u32 availableToSwitch, u32 opposingBattler)
{
    u32 i, move;
    u16 flags;
    
    if ((gBattleMons[battlerId].status2 & STATUS2_DESTINY_BOND) && AI_GetNoOfHitsToKOBattlerHigherDamage(opposingBattler, battlerId) < 2)
        return NO_SWITCH;
    
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        move = gBattleMons[battlerId].moves[i];
        
        if (!move || GetBattleMoveSplit(move) == SPLIT_STATUS)
            continue;
        
        if (TypeCalc(move, GetBattlerMoveType(battlerId, move), battlerId, opposingBattler, FALSE, FALSE, &flags) != TYPE_MUL_NO_EFFECT)
            return NO_SWITCH;
    }
    return PARTY_SIZE;
}

static u32 ShouldSwitchIfNaturalCureOrRegenerator(u32 battlerId, u8 *viableMons, u32 availableToSwitch, u32 opposingBattler)
{
    if (FindQueuedEffectsInSideList(GetBattlerSide(battlerId), gEntryHazardsQueuedEffectIds) == B_SIDE_QUEUED_COUNT)
    {
        switch (GetBattlerAbility(battlerId))
        {
            case ABILITY_NATURAL_CURE:
                if (gBattleMons[battlerId].status1.id)
                    return PARTY_SIZE;
                break;
            case ABILITY_REGENERATOR:
                if (gBattleMons[battlerId].hp < gBattleMons[battlerId].maxHP / 2)
                    return PARTY_SIZE;
                break;
            case ABILITY_ZERO_TO_HERO:
                if (TryDoBattleFormChange(battlerId, FORM_CHANGE_SWITCH_OUT))
                    return PARTY_SIZE;
                break;
        }
    }
    return NO_SWITCH;
}

static u32 ShouldSwitchWhenYawned(u32 battlerId, u8 *viableMons, u32 availableToSwitch, u32 opposingBattler)
{
    if ((gStatuses3[battlerId] & STATUS3_YAWN) && GetBattlerAbility(battlerId) != ABILITY_NATURAL_CURE && gBattleMons[battlerId].hp > gBattleMons[battlerId].maxHP / 4
    && CanBePutToSleep(battlerId, battlerId, 0) == STATUS_CHANGE_WORKED)
    {
        // TODO
        return PARTY_SIZE;
    }
    return NO_SWITCH;
}

///////////
// ITEMS //
///////////

bool32 BattleAI_ShouldUseItem(u32 battlerId)
{
    return FALSE;
}

// #define MAX_ITEM_PRIORITY 10
// 
// // How higher more priority takes over other item effects
// bool32 BattleAI_ShouldUseItem(u32 battlerId)
// {
//     u32 i, j, side, partyId, itemPriority, chosenItemIndex, numUsableItems, itemPriorities[MAX_TRAINER_ITEMS];
//     u32 holdEffectParam, item, usableItems[MAX_TRAINER_ITEMS];
//     
//     // Item effects are'nt blocked
//     if (!IsItemUseBlockedByBattleEffect(battlerId) && !(gBattleTypeFlags & BATTLE_TYPE_RECORDED))
//     {
//         side = GetBattlerSide(battlerId);
//         
//         gBattleStruct->battlers[battlerId].itemPartyIndex = PARTY_SIZE;
//         
//         itemPriority = 0;
//         numUsableItems = 0;
//         
//         for (i = 0; i < MAX_TRAINER_ITEMS; i++, itemPriority = 0)
//         {
//             item = gAIData->logic[side].items[i];
//             usableItems[numUsableItems] = item;
//             
//             // Ignore items that was already used or that dont exist
//             if (item && item < ITEMS_COUNT)
//             {
//                 holdEffectParam = ItemId_GetHoldEffectParam(item);
//                 
//                 switch (ItemId_GetBattleUsage(item))
//                 {
//                     case EFFECT_ITEM_REVIVE:
//                         for (partyId = 0; partyId < PARTY_SIZE; partyId++)
//                         {
//                             // Loop through fainted mons
//                             if (partyId != gBattlerPartyIndexes[battlerId] && IsMonValidSpecies(&GetSideParty(side)[partyId]) && !GetMonData(&GetSideParty(side)[partyId], MON_DATA_HP)
//                                 /*&& (IsMonGoodChoiceToSwitchInto(&GetSideParty(side)[partyId]) || AI_GetNoOfHitsToKOBattlerHigherDamage(defender, attacker) < 3)*/)
//                                 break;
//                         }
//                         
//                         if (partyId != PARTY_SIZE)
//                         {
//                             gBattleStruct->battlers[battlerId].itemPartyIndex = partyId;
//                             itemPriority = MAX_ITEM_PRIORITY / holdEffectParam; // Increase based on how much HP the mon will be revived with
//                         }
//                         break;
//                         // Need a party member selector
//                     case EFFECT_ITEM_RESTORE_HP:
//                     case EFFECT_ITEM_CURE_PRIMARY_STATUS:
//                         break;
//                     case EFFECT_ITEM_INCREASE_STAT:
//                         if (gBattleMons[battlerId].hp >= gBattleMons[battlerId].maxHP / 2 && CompareStat(battlerId, holdEffectParam, MAX_STAT_STAGES, CMP_NOT_EQUAL))
//                             itemPriority = GetStatUpScore(battlerId, MAX_BATTLERS_COUNT, holdEffectParam, GetItemStatChangeStages(item), FALSE); // Increase based on the number of stages
//                         break;
//                     case EFFECT_ITEM_INCREASE_ALL_STATS:
//                         if (gBattleMons[battlerId].hp >= gBattleMons[battlerId].maxHP / 2)
//                         {
//                             u32 count = 0;
//                             
//                             for (j = STAT_ATK; j < NUM_STATS; j++)
//                             {
//                                 // Increases how much as has valid stats to increase
//                                 if (CompareStat(battlerId, j, MAX_STAT_STAGES, CMP_NOT_EQUAL))
//                                 {
//                                     itemPriority += GetStatUpScore(battlerId, MAX_BATTLERS_COUNT, j, +1, FALSE);
//                                     ++count;
//                                 }
//                             }
//                             itemPriority /= (count / 2); // Get average score
//                         }
//                         break;
//                     /*case EFFECT_ITEM_SET_FOCUS_ENERGY:
//                         if (ShouldAIIncreaseCriticalChance(attacker, defender))
//                             itemPriority = 2; // Since it increases two stages, consider 2x a stat up
//                         break;*/
//                     // The other items cant be used
//                     default:
//                         break;
//                 }
//                 
//                 if (itemPriority)
//                 {
//                     if (itemPriority > MAX_ITEM_PRIORITY)
//                         itemPriority = MAX_ITEM_PRIORITY;
//                     
//                     itemPriorities[numUsableItems] = itemPriority;
//                     ++numUsableItems;
//                 }
//             }
//         }
//         
//         // Choose item to use
//         if (numUsableItems)
//         {
//             chosenItemIndex = 0;
//             
//             for (i = 0; i < numUsableItems; i++)
//             {
//                 if (itemPriorities[i] > itemPriorities[chosenItemIndex])
//                     chosenItemIndex = i;
//             }
//             
//             gAIData->logic[side].items[chosenItemIndex] = ITEM_NONE;
//             gBattleStruct->battlers[battlerId].chosenItem = usableItems[chosenItemIndex];
//             
//             // Set selected party ID to current battler if none chosen.
//             if (gBattleStruct->battlers[battlerId].itemPartyIndex == PARTY_SIZE)
//                 gBattleStruct->battlers[battlerId].itemPartyIndex = gBattlerPartyIndexes[battlerId];
//             
//             BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_USE_ITEM, 0);
//             
//             return TRUE;
//         }
//     }
//     return FALSE;
// }

#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_util.h"
#include "battle_controllers.h"
#include "random.h"
#include "party_menu.h"
#include "item.h"
#include "util.h"
#include "constants/abilities.h"
#include "constants/item_effects.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"

static bool8 HasSuperEffectiveMoveAgainstOpponents(u8 battlerId, bool8 noRng);
static bool8 FindMonWithFlagsAndSuperEffective(u8 battlerId, u8 flags, u8 moduloPercent);
static bool8 ShouldUseItem(u8 battlerId);

static bool8 ShouldSwitchIfPerishSong(u8 battlerId)
{
    if (gStatuses3[battlerId] & STATUS3_PERISH_SONG
     && gDisableStructs[battlerId].perishSongTimer == 0)
    {
        *(gBattleStruct->AI_monToSwitchIntoId + (GetBattlerPosition(battlerId) >> 1)) = PARTY_SIZE;
        BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_SWITCH, 0);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static bool8 ShouldSwitchIfWonderGuard(u8 battlerId)
{
    u8 opposingBattler;
    s32 i, j;
    u16 move, moveFlags;

    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
        return FALSE;
    if (GetBattlerAbility(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)) == ABILITY_WONDER_GUARD)
    {
        // Check if Pokemon has a super effective move.
        for (opposingBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), i = 0; i < MAX_MON_MOVES; ++i)
        {
            move = gBattleMons[battlerId].moves[i];
            if (move == MOVE_NONE)
                continue;
            moveFlags = AI_TypeCalc(move, gBattleMons[opposingBattler].species, GetBattlerAbility(opposingBattler));
            if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE)
                return FALSE;
        }
        // Find a Pokemon in the party that has a super effective move.
        for (i = 0; i < PARTY_SIZE; ++i)
        {
            if (!MonCanBattle(&gEnemyParty[i]) || i == gBattlerPartyIndexes[battlerId])
                continue;
            GetMonData(&gEnemyParty[i], MON_DATA_SPECIES); // Unused return value.
            GetMonData(&gEnemyParty[i], MON_DATA_ABILITY_NUM); // Unused return value.        
            for (opposingBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), j = 0; j < MAX_MON_MOVES; ++j)
            {
                move = GetMonData(&gEnemyParty[i], MON_DATA_MOVE1 + j);
                if (move == MOVE_NONE)
                    continue;
                moveFlags = AI_TypeCalc(move, gBattleMons[opposingBattler].species, GetBattlerAbility(opposingBattler));
                if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE && Random() % 3 < 2)
                {
                    // We found a mon.
                    *(gBattleStruct->AI_monToSwitchIntoId + (GetBattlerPosition(battlerId) >> 1)) = i;
                    BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_SWITCH, 0);
                    return TRUE;
                }
            }
        }
    }
    return FALSE; // There is not a single Pokemon in the party that has a super effective move against a mon with Wonder Guard.
}

static bool8 FindMonThatAbsorbsOpponentsMove(u8 battlerId)
{
    u8 battlerIn1, battlerIn2;
    u16 absorbingTypeAbility;
    s32 i;

    if ((HasSuperEffectiveMoveAgainstOpponents(battlerId, TRUE) && Random() % 3) 
    || (gLastLandedMoves[battlerId] == MOVE_NONE))
        return FALSE;
    if (gLastLandedMoves[battlerId] == 0xFFFF
     || gBattleMoves[gLastLandedMoves[battlerId]].power == 0)
        return FALSE;
    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        battlerIn1 = battlerId;
        if (gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battlerId)))])
            battlerIn2 = battlerId;
        else
            battlerIn2 = GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battlerId)));
    }
    else
    {
        battlerIn1 = battlerId;
        battlerIn2 = battlerId;
    }
    if (gBattleMoves[gLastLandedMoves[battlerId]].type == TYPE_FIRE)
        absorbingTypeAbility = ABILITY_FLASH_FIRE;
    else if (gBattleMoves[gLastLandedMoves[battlerId]].type == TYPE_WATER)
        absorbingTypeAbility = ABILITY_WATER_ABSORB;
    else if (gBattleMoves[gLastLandedMoves[battlerId]].type == TYPE_ELECTRIC)
        absorbingTypeAbility = ABILITY_VOLT_ABSORB;
    else
        return FALSE;
    if (GetBattlerAbility(battlerId) == absorbingTypeAbility)
        return FALSE;
    for (i = 0; i < PARTY_SIZE; ++i)
    {
        u16 monAbility;

        if (!MonCanBattle(&gEnemyParty[i]) || i == gBattlerPartyIndexes[battlerIn1] || i == gBattlerPartyIndexes[battlerIn2]
		|| i == gBattleStruct->monToSwitchIntoId[battlerIn1] || i == gBattleStruct->monToSwitchIntoId[battlerIn2])
            continue;
            monAbility = GetMonAbility(&gEnemyParty[i]);
        if (absorbingTypeAbility == monAbility && Random() & 1)
        {
            // we found a mon
            *(gBattleStruct->AI_monToSwitchIntoId + (GetBattlerPosition(battlerId) >> 1)) = i;
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_SWITCH, 0);
            return TRUE;
        }
    }
    return FALSE;
}

static bool8 ShouldSwitchIfNaturalCure(u8 battlerId)
{
    if (!(gBattleMons[battlerId].status1 & STATUS1_SLEEP)
     || (GetBattlerAbility(battlerId) != ABILITY_NATURAL_CURE)
     || (gBattleMons[battlerId].hp < gBattleMons[battlerId].maxHP / 2))
        return FALSE;
    if ((gLastLandedMoves[battlerId] == MOVE_NONE || gLastLandedMoves[battlerId] == 0xFFFF) && Random() & 1)
    {
        *(gBattleStruct->AI_monToSwitchIntoId + (GetBattlerPosition(battlerId) >> 1)) = PARTY_SIZE;
        BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_SWITCH, 0);
        return TRUE;
    }
    else if (gBattleMoves[gLastLandedMoves[battlerId]].power == 0 && Random() & 1)
    {
        *(gBattleStruct->AI_monToSwitchIntoId + (GetBattlerPosition(battlerId) >> 1)) = PARTY_SIZE;
        BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_SWITCH, 0);
        return TRUE;
    }
    if (FindMonWithFlagsAndSuperEffective(battlerId, MOVE_RESULT_DOESNT_AFFECT_FOE, 1)
     || FindMonWithFlagsAndSuperEffective(battlerId, MOVE_RESULT_NOT_VERY_EFFECTIVE, 1))
        return TRUE;
    if (Random() & 1)
    {
        *(gBattleStruct->AI_monToSwitchIntoId + (GetBattlerPosition(battlerId) >> 1)) = PARTY_SIZE;
        BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_SWITCH, 0);
        return TRUE;
    }
    return FALSE;
}

static bool8 HasSuperEffectiveMoveAgainstOpponents(u8 battlerId, bool8 noRng)
{
    u8 opposingBattler;
    s32 i;
    u16 move, moveFlags;

    opposingBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
    if (!(gAbsentBattlerFlags & gBitTable[opposingBattler]))
    {
        for (i = 0; i < MAX_MON_MOVES; ++i)
        {
            move = gBattleMons[battlerId].moves[i];
            if (move == MOVE_NONE)
                continue;

            moveFlags = AI_TypeCalc(move, gBattleMons[opposingBattler].species, GetBattlerAbility(opposingBattler));
            if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE)
            {
                if (noRng || (Random() % 10))
                    return TRUE;
            }
        }
    }
    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
        return FALSE;
    opposingBattler = GetBattlerAtPosition(BATTLE_PARTNER(B_POSITION_PLAYER_LEFT));
    if (!(gAbsentBattlerFlags & gBitTable[opposingBattler]))
    {
        for (i = 0; i < MAX_MON_MOVES; ++i)
        {
            move = gBattleMons[battlerId].moves[i];
            if (move == MOVE_NONE)
                continue;
            moveFlags = AI_TypeCalc(move, gBattleMons[opposingBattler].species, GetBattlerAbility(opposingBattler));
            if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE)
            {
                if (noRng)
                    return TRUE;
                if (Random() % 10 != 0)
                    return TRUE;
            }
        }
    }
    return FALSE;
}

static bool8 AreStatsRaised(u8 battlerId)
{
    u8 buffedStatsValue = 0;
    s32 i;

    for (i = 0; i < NUM_BATTLE_STATS; ++i)
    {
        if (gBattleMons[battlerId].statStages[i] > 6)
            buffedStatsValue += gBattleMons[battlerId].statStages[i] - 6;
    }
    return (buffedStatsValue > 3);
}

static bool8 FindMonWithFlagsAndSuperEffective(u8 battlerId, u8 flags, u8 moduloPercent)
{
    u8 battlerIn1, battlerIn2;
    s32 i, j;
    u16 move, moveFlags;

    if (gLastLandedMoves[battlerId] == 0)
        return FALSE;
    if ((gLastLandedMoves[battlerId] == 0xFFFF)
     || (gLastHitBy[battlerId] == 0xFF)
     || (gBattleMoves[gLastLandedMoves[battlerId]].power == 0))
        return FALSE;
    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        battlerIn1 = battlerId;
        if (gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battlerId)))])
            battlerIn2 = battlerId;
        else
            battlerIn2 = GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battlerId)));
    }
    else
    {
        battlerIn1 = battlerId;
        battlerIn2 = battlerId;
    }
    for (i = 0; i < PARTY_SIZE; ++i)
    {
        u16 monAbility;

        if (!MonCanBattle(&gEnemyParty[i]) || (i == gBattlerPartyIndexes[battlerIn1]) || (i == gBattlerPartyIndexes[battlerIn2])
         || (i == gBattleStruct->monToSwitchIntoId[battlerIn1]) || (i == gBattleStruct->monToSwitchIntoId[battlerIn2]))
            continue;
            monAbility = GetMonAbility(&gEnemyParty[i]);
        moveFlags = AI_TypeCalc(gLastLandedMoves[battlerId], GetMonData(&gEnemyParty[i], MON_DATA_SPECIES), monAbility);
        if (moveFlags & flags)
        {
            battlerIn1 = gLastHitBy[battlerId];
            for (j = 0; j < MAX_MON_MOVES; ++j)
            {
                move = GetMonData(&gEnemyParty[i], MON_DATA_MOVE1 + j);
                if (move == MOVE_NONE)
                    continue;
                moveFlags = AI_TypeCalc(move, gBattleMons[battlerIn1].species, GetBattlerAbility(battlerIn1));
                if (moveFlags & MOVE_RESULT_SUPER_EFFECTIVE && Random() % moduloPercent == 0)
                {
                    *(gBattleStruct->AI_monToSwitchIntoId + (GetBattlerPosition(battlerId) >> 1)) = i;
                    BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_SWITCH, 0);
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

static bool8 ShouldSwitch(u8 battlerId)
{
    u8 battlerIn1, battlerIn2;
    s32 i;
    s32 availableToSwitch;

    if ((gBattleMons[battlerId].status2 & (STATUS2_WRAPPED | STATUS2_ESCAPE_PREVENTION))
     || (gStatuses3[battlerId] & STATUS3_ROOTED)
     || ABILITY_ON_OPPOSING_SIDE(battlerId, ABILITY_SHADOW_TAG)
     || ABILITY_ON_OPPOSING_SIDE(battlerId, ABILITY_ARENA_TRAP))
        return FALSE; // misses the flying or levitate check
    if (ABILITY_ON_FIELD(ABILITY_MAGNET_PULL))
        if ((gBattleMons[battlerId].type1 == TYPE_STEEL) || (gBattleMons[battlerId].type2 == TYPE_STEEL))
            return FALSE;
    availableToSwitch = 0;
    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        battlerIn1 = battlerId;
        if (gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battlerId)))])
            battlerIn2 = battlerId;
        else
            battlerIn2 = GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battlerId)));
    }
    else
    {
        battlerIn2 = battlerId;
        battlerIn1 = battlerId;
    }
    for (i = 0; i < PARTY_SIZE; ++i)
    {
        if (!MonCanBattle(&gEnemyParty[i]) || (i == gBattlerPartyIndexes[battlerIn1]) || (i == gBattlerPartyIndexes[battlerIn2])
         || (i == gBattleStruct->monToSwitchIntoId[battlerIn1]) || (i == gBattleStruct->monToSwitchIntoId[battlerIn2]))
            continue;
        ++availableToSwitch;
    }
    if (!availableToSwitch)
        return FALSE;
    if (ShouldSwitchIfPerishSong(battlerId)
     || ShouldSwitchIfWonderGuard(battlerId)
     || FindMonThatAbsorbsOpponentsMove(battlerId)
     || ShouldSwitchIfNaturalCure(battlerId))
        return TRUE;
    if (HasSuperEffectiveMoveAgainstOpponents(battlerId, FALSE)
     || AreStatsRaised(battlerId))
        return FALSE;
    if (FindMonWithFlagsAndSuperEffective(battlerId, MOVE_RESULT_DOESNT_AFFECT_FOE, 2)
     || FindMonWithFlagsAndSuperEffective(battlerId, MOVE_RESULT_NOT_VERY_EFFECTIVE, 3))
        return TRUE;
    return FALSE;
}

void AI_TrySwitchOrUseItem(u8 battlerId)
{
    u8 battlerIn1, battlerIn2;

    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
    {
        if (ShouldSwitch(battlerId))
        {
            if (*(gBattleStruct->AI_monToSwitchIntoId + (GetBattlerPosition(battlerId) >> 1)) == 6)
            {
                s32 monToSwitchId = GetMostSuitableMonToSwitchInto(battlerId);
                if (monToSwitchId == 6)
                {
                    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
                    {
                        battlerIn1 = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
                        battlerIn2 = battlerIn1;
                    }
                    else
                    {
                        battlerIn1 = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
                        battlerIn2 = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
                    }
                    for (monToSwitchId = 0; monToSwitchId < PARTY_SIZE; ++monToSwitchId)
                    {
                        if ((!GetMonData(&gEnemyParty[monToSwitchId], MON_DATA_HP) == 0)
                         && (monToSwitchId != gBattlerPartyIndexes[battlerIn1])
                         && (monToSwitchId != gBattlerPartyIndexes[battlerIn2])
                         && (monToSwitchId != gBattleStruct->monToSwitchIntoId[battlerIn1])
                         && (monToSwitchId != gBattleStruct->monToSwitchIntoId[battlerIn2]))
                            break;
                    }
                }
                *(gBattleStruct->AI_monToSwitchIntoId + (GetBattlerPosition(battlerId) >> 1)) = monToSwitchId;
            }
            gBattleStruct->monToSwitchIntoId[battlerId] = *(gBattleStruct->AI_monToSwitchIntoId + (GetBattlerPosition(battlerId) >> 1));
            return;
        }
        else if (ShouldUseItem(battlerId))
        {
            return;
        }
    }
    BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_USE_MOVE, (BATTLE_OPPOSITE(battlerId)) << 8);
}

static void ModulateByTypeEffectiveness(u8 atkType, u8 defType1, u8 defType2, u8 *var)
{
    *var *= GetTypeModifier(atkType, defType1);
	
	if (defType1 != defType2)
		*var *= GetTypeModifier(atkType, defType2);
}

u8 GetMostSuitableMonToSwitchInto(u8 battlerId)
{
    u8 opposingBattler;
    u8 bestDmg; // Note : should be changed to u32 for obvious reasons.
    u8 bestMonId;
    u8 battlerIn1, battlerIn2;
    s32 i, j;
    u8 invalidMons;
    u16 move;

    if (gBattleStruct->monToSwitchIntoId[battlerId] != PARTY_SIZE)
        return gBattleStruct->monToSwitchIntoId[battlerId];
    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        battlerIn1 = battlerId;
        if (gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battlerId)))])
            battlerIn2 = battlerId;
        else
            battlerIn2 = GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battlerId)));
        // UB: It considers the opponent only player's side even though it can battle alongside player.
        opposingBattler = Random() & BIT_FLANK;
        if (gAbsentBattlerFlags & gBitTable[opposingBattler])
            opposingBattler = BATTLE_PARTNER(opposingBattler);
    }
    else
    {
        opposingBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        battlerIn1 = battlerId;
        battlerIn2 = battlerId;
    }
    invalidMons = 0;
    while (invalidMons != 0x3F) // All mons are invalid.
    {
        bestDmg = 0;
        bestMonId = 6;
        // Find the mon whose type is the most suitable offensively.
        for (i = 0; i < PARTY_SIZE; ++i)
        {
            u16 species = GetMonData(&gEnemyParty[i], MON_DATA_SPECIES);
            if (species != SPECIES_NONE
                && GetMonData(&gEnemyParty[i], MON_DATA_HP) != 0
                && !(gBitTable[i] & invalidMons)
                && gBattlerPartyIndexes[battlerIn1] != i
                && gBattlerPartyIndexes[battlerIn2] != i
                && i != gBattleStruct->monToSwitchIntoId[battlerIn1]
                && i != gBattleStruct->monToSwitchIntoId[battlerIn2])
            {
                u8 type1 = gBaseStats[species].type1;
                u8 type2 = gBaseStats[species].type2;
                u8 typeDmg = 10;
                ModulateByTypeEffectiveness(gBattleMons[opposingBattler].type1, type1, type2, &typeDmg);
                ModulateByTypeEffectiveness(gBattleMons[opposingBattler].type2, type1, type2, &typeDmg);
                if (bestDmg < typeDmg)
                {
                    bestDmg = typeDmg;
                    bestMonId = i;
                }
            }
            else
            {
                invalidMons |= gBitTable[i];
            }
        }
        // Ok, we know the mon has the right typing but does it have at least one super effective move?
        if (bestMonId != PARTY_SIZE)
        {
            for (i = 0; i < MAX_MON_MOVES; ++i)
            {
				u16 flags;
				move = GetMonData(&gEnemyParty[bestMonId], MON_DATA_MOVE1 + i);
				
				if (move != MOVE_NONE)
				{
					if (CalcTypeEffectivenessMultiplier(move, gBattleMoves[move].type, battlerId, opposingBattler, FALSE, &flags) == TYPE_MUL_SUPER_EFFECTIVE)
						break;
				}
            }
            if (i != MAX_MON_MOVES)
                return bestMonId; // Has both the typing and at least one super effective move.

            invalidMons |= gBitTable[bestMonId]; // Sorry buddy, we want something better.
        }
        else
        {
            invalidMons = 0x3F; // No viable mon to switch.
        }
    }
    bestDmg = 0;
    bestMonId = 6;
    // If we couldn't find the best mon in terms of typing, find the one that deals most damage.
    for (i = 0; i < PARTY_SIZE; ++i)
    {
        if (((u16)(GetMonData(&gEnemyParty[i], MON_DATA_SPECIES)) == SPECIES_NONE)
         || (GetMonData(&gEnemyParty[i], MON_DATA_HP) == 0)
         || (gBattlerPartyIndexes[battlerIn1] == i)
         || (gBattlerPartyIndexes[battlerIn2] == i)
         || (i == gBattleStruct->monToSwitchIntoId[battlerIn1])
         || (i == gBattleStruct->monToSwitchIntoId[battlerIn2]))
            continue;
        for (j = 0; j < MAX_MON_MOVES; ++j)
        {
            move = GetMonData(&gEnemyParty[i], MON_DATA_MOVE1 + j);
			
            if (move != MOVE_NONE && gBattleMoves[move].power != 1)
                AI_CalcDmg(battlerId, opposingBattler, move);
			
            if (bestDmg < gBattleMoveDamage)
            {
                bestDmg = gBattleMoveDamage;
                bestMonId = i;
            }
        }
    }
    return bestMonId;
}

static bool8 ShouldUseItem(u8 battlerId)
{
    s32 i;
    u8 validMons = 0;
    bool8 shouldUse = FALSE;

    for (i = 0; i < PARTY_SIZE; ++i)
        if (MonCanBattle(&gEnemyParty[i]))
            ++validMons;
    for (i = 0; i < MAX_TRAINER_ITEMS; ++i)
    {
        u16 item;
        const u8 *itemEffects;
        u8 paramOffset;
        u8 battlerSide;

        item = gBattleResources->battleHistory->trainerItems[i];
        if (item == ITEM_NONE)
            continue;
		itemEffects = GetItemEffect(item);
        if (itemEffects == NULL)
			continue;
        switch (ItemId_GetBattleUsage(item))
        {
			default: // TODO: add ai logic
			    shouldUse = FALSE;
        }
        if (shouldUse)
        {
			// Set selected party ID to current battler if none chosen.
			if (gBattleStruct->itemPartyIndex[battlerId] == PARTY_SIZE)
				gBattleStruct->itemPartyIndex[battlerId] = gBattlerPartyIndexes[battlerId];
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_USE_ITEM, 0);
            gBattleStruct->chosenItem[battlerId] = item;
            gBattleResources->battleHistory->trainerItems[i] = 0;
            return shouldUse;
        }
    }
    return FALSE;
}

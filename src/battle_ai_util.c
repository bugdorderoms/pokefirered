#include "global.h"
#include "battle.h"
#include "battle_ai.h"
#include "battle_ai_util.h"
#include "battle_move_effects.h"
#include "battle_util.h"
#include "item.h"
#include "random.h"
#include "set_effect.h"
#include "util.h"
#include "constants/abilities.h"
#include "constants/hold_effects.h"
#include "constants/moves.h"

u32 GetAllyChosenMove(u32 battlerId)
{
	u32 ally = BATTLE_PARTNER(battlerId);
	
	if (!IsBattlerAlive(ally))
		return MOVE_NONE;
	else if (ally > battlerId)
		return gBattleStruct->battlers[ally].lastMove;
	else
		return gBattleMons[ally].moves[gBattleStruct->battlers[ally].chosenMovePosition];
}

bool32 AIShouldConsiderMoveForBattler(u32 attacker, u32 defender, u32 move)
{
	if (attacker == BATTLE_PARTNER(defender))
	{
		u32 moveTarget = GetBattlerMoveTargetType(attacker, move);
		
		if (moveTarget == MOVE_TARGET_BOTH || moveTarget == MOVE_TARGET_OPPONENTS_FIELD)
			return FALSE;
	}
	return TRUE;
}

/////////////////////////
// AI RECORD FUNCTIONS //
/////////////////////////
bool32 BattleAI_KnowsBattlerItem(u32 battlerId)
{
	if (GetBattlerSide(battlerId) == B_SIDE_PLAYER && !(AI_DATA->knownPlayerItems & Bit(gBattlerPartyIndexes[battlerId])))
		return FALSE;
	return TRUE;
}

void BattleAI_RecordHoldEffect(u32 battlerId)
{
	if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
		AI_DATA->knownPlayerItems |= Bit(gBattlerPartyIndexes[battlerId]);
}

bool32 BattleAI_KnowsBattlerMoveIndex(u32 battlerId, u32 moveIndex)
{
	if (GetBattlerSide(battlerId) == B_SIDE_PLAYER && !(AI_DATA->knownPlayerMoves[gBattlerPartyIndexes[battlerId]] & Bit(moveIndex)))
		return FALSE;
	return TRUE;
}

void BattleAI_RecordMoveUsed(u32 battlerId, u32 moveSlot)
{
	if (GetBattlerSide(battlerId) == B_SIDE_PLAYER && moveSlot != MAX_MON_MOVES)
		AI_DATA->knownPlayerMoves[gBattlerPartyIndexes[battlerId]] |= Bit(moveSlot);
}

bool32 BattleAI_KnowsBattlerPartyIndex(u32 battlerId)
{
	if (GetBattlerSide(battlerId) == B_SIDE_PLAYER && !(AI_DATA->knownPartyIndices & Bit(gBattlerPartyIndexes[battlerId])))
		return FALSE;
	return TRUE;
}

void BattleAI_RecordPartyIndex(u32 battlerId)
{
	AI_DATA->knownPartyIndices |= Bit(gBattlerPartyIndexes[battlerId]);
}

bool32 BattleAI_KnowsBattlerAbility(u32 battlerId)
{
	if (GetBattlerSide(battlerId) == B_SIDE_PLAYER && !(AI_DATA->knownPlayerAbilities & Bit(gBattlerPartyIndexes[battlerId])))
		return FALSE;
	return TRUE;
}

void BattleAI_RecordAbility(u32 battlerId)
{
	if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
		AI_DATA->knownPlayerAbilities |= Bit(gBattlerPartyIndexes[battlerId]);
}

////////////////////////////////////////
// DAMAGE AND STATS UTILITY FUNCTIONS //
////////////////////////////////////////
u32 GetNoOfHitsToKOBattler(u32 attacker, u32 defender, u32 moveIndex)
{
	return GetNoOfHitsToKOBattlerDmg(AI_THINKING->simulatedDmg[attacker][defender][moveIndex], defender);
}

u32 GetNoOfHitsToKOBattlerHigherDamage(u32 attacker, u32 defender)
{
	return GetNoOfHitsToKOBattlerDmg(AI_THINKING->higherDamage[attacker][defender], defender);
}

u32 GetNoOfHitsToKOBattlerDmg(s32 dmg, u32 battler)
{
	if (!dmg)
		return 0xFFFFFFFF;
	
	return gBattleMons[battler].hp / (dmg + 1) + 1;
}

u32 GetNumMovesWithSplitInBattlerMoveset(u32 battlerId, u32 split)
{
	u32 i, count = 0;

	for (i = 0; i < MAX_MON_MOVES; i++)
	{
		if (AI_THINKING->moves[battlerId][i] && GetMoveSplit(AI_THINKING->moves[battlerId][i]) == split)
			++count;
	}
	return count;
}

static bool32 IsBattlerPhysicalAttacker(u32 battlerId)
{
	if (gBattleMons[battlerId].attack >= gBattleMons[battlerId].spAttack || GetNumMovesWithSplitInBattlerMoveset(battlerId, SPLIT_PHYSICAL) >= (MAX_MON_MOVES / 2))
		return TRUE;
	
	return FALSE;
}

static bool32 IsBattlerSpecialAttacker(u32 battlerId)
{
	if (gBattleMons[battlerId].spAttack >= gBattleMons[battlerId].attack || GetNumMovesWithSplitInBattlerMoveset(battlerId, SPLIT_SPECIAL) >= (MAX_MON_MOVES / 2))
		return TRUE;
	
	return FALSE;
}

static bool32 IsBattlerPhysicalDefender(u32 battlerId)
{
	return (gBattleMons[battlerId].defense >= gBattleMons[battlerId].spDefense);
}

static bool32 IsBattlerSpecialDefender(u32 battlerId)
{
	return (gBattleMons[battlerId].spDefense >= gBattleMons[battlerId].defense);
}

u32 AI_GetStatChangeScore(u32 attacker, u32 defender, u32 statId, s32 stages, bool32 toUp, bool32 checkEffectsBlock)
{
	bool32 getScore = FALSE;
	
	if (GetBattlerAbility(attacker) == ABILITY_CONTRARY)
		toUp ^= TRUE;
	
	if (toUp)
	{
		switch (statId)
		{
			case STAT_ATK: // Increase Attack if is a physical attacker mon or the target is a special defender mon
				if (IsBattlerPhysicalAttacker(attacker) || (defender != MAX_BATTLERS_COUNT && IsBattlerSpecialDefender(defender)))
					getScore = TRUE;
				break;
			case STAT_SPATK: // Increase Sp. Attack if is a special attacker mon or the target is a physical defender mon
				if (IsBattlerSpecialAttacker(attacker) || (defender != MAX_BATTLERS_COUNT && IsBattlerPhysicalDefender(defender)))
					getScore = TRUE;
				break;
			case STAT_SPEED: // Increase Speed if is lower than the target, except in Trick Room
				if ((defender == MAX_BATTLERS_COUNT || !AIIsFaster(attacker, defender)) && !(gFieldStatus & STATUS_FIELD_TRICK_ROOM))
					getScore = TRUE;
				break;
			case STAT_DEF: // Increase Defense if is a physical defender
				if (IsBattlerPhysicalDefender(attacker))
					getScore = TRUE;
				break;
			case STAT_SPDEF: // Increase Sp. Defense if is a special defender
				if (IsBattlerSpecialDefender(attacker))
					getScore = TRUE;
				break;
			default: // Always benefit from the others stat ups
				getScore = TRUE;
				break;
		}
	}
	else
	{
		if (!checkEffectsBlock || defender == MAX_BATTLERS_COUNT || CheckStatDecreaseBlockEffects(attacker, defender, statId, STAT_CHANGE_FLAG_IGNORE_PROTECT) == STAT_CHANGE_WORKED)
		{
			switch (statId)
			{
				case STAT_ATK: // Decrease Attack only if it is't a physical attacker
					if (!IsBattlerPhysicalAttacker(attacker))
						getScore = TRUE;
					break;
				case STAT_SPATK: // Decrease Sp. Attack only if it is't a special attacker
					if (!IsBattlerSpecialAttacker(attacker))
						getScore = TRUE;
					break;
				case STAT_DEF: // Decrease Defense only if it is't a physical defender
					if (!IsBattlerPhysicalDefender(attacker))
						getScore = TRUE;
					break;
				case STAT_SPDEF: // Decrease Sp. Defense only if it is't a special defender
					if (!IsBattlerSpecialDefender(attacker))
						getScore = TRUE;
					break;
				default: // Never benefit from the others stat down
					break;
			}
		}
	}
	
	if (getScore)
	{
		if (GetBattlerAbility(attacker) == ABILITY_SIMPLE)
			stages *= 2; // Double stages if has Simple
		
		if (stages < 0) // Turns into a positive number
			stages *= -1;
		
		if (stages > +6) // Clamp to max +6 stages
			stages = +6;
		
		return stages;
	}
	return 0;
}

// Battler1 is the ai battler
bool32 AIIsFaster(u32 battler1, u32 battler2)
{
	return (AI_THINKING->totalSpeeds[battler1] > AI_THINKING->totalSpeeds[battler2]);
}

bool32 ShouldAIIncreaseCriticalChance(u32 attacker, u32 defender)
{
	if (!(gBattleMons[attacker].status2 & STATUS2_FOCUS_ENERGY))
	{
		// Prefer copy critical chance using Psych Up
		if ((gBattleMons[defender].status2 & STATUS2_FOCUS_ENERGY) && AI_BattlerHasMoveEffectInMoveset(attacker, EFFECT_PSYCH_UP))
			return FALSE;
		
		return RandomPercent(30);
	}
	return FALSE;
}

///////////////////////////////////
// MOVE EFFECTS HELPER FUNCTIONS //
///////////////////////////////////
bool32 TargetImuneToMove(u32 attacker, u32 defender, u32 moveSlot)
{
	if (AI_THINKING->effectiveness[attacker][defender][moveSlot] == TYPE_MUL_NO_EFFECT || AbilityBattleEffects(ABILITYEFFECT_WOULD_BLOCK_MOVE, defender)
	|| (!IsBattlerAlly(attacker, defender) && AbilityBattleEffects(ABILITYEFFECT_WOULD_ABSORB_MOVE, defender)))
		return TRUE;
	
	return FALSE;
}

static bool32 BattlerHasMoveEffectInMovesetThatAffectsTarget(u32 attacker, u32 target, u32 moveEffect)
{
	u32 i;

	for (i = 0; i < MAX_MON_MOVES; i++)
	{
		if (AI_THINKING->moves[attacker][i] && gBattleMoves[AI_THINKING->moves[attacker][i]].effect == moveEffect && !TargetImuneToMove(attacker, target, i))
			return TRUE;
	}
	return FALSE;
}

bool32 HasMoveEffectInMoveset(u16 *moveset, u32 moveEffect)
{
	u32 i;

	for (i = 0; i < MAX_MON_MOVES; i++)
	{
		if (moveset[i] && gBattleMoves[moveset[i]].effect == moveEffect)
			return TRUE;
	}
	return FALSE;
}

bool32 SideHasMoveEffectInMovesetThatAffectsTarget(u32 attacker, u32 target, u32 moveEffect)
{
	u32 i;
	
	for (i = 0; i < NUM_BATTLERS_PER_SIDE; i++, attacker = BATTLE_PARTNER(attacker))
	{
		if (IsBattlerAlive(attacker) && BattlerHasMoveEffectInMovesetThatAffectsTarget(attacker, target, moveEffect))
			return TRUE;
	}
	return FALSE;
}

bool32 BadIdeaToBurn(u32 attacker, u32 defender)
{
	u32 holdEffect, ability;
	
	if (CanBeBurned(attacker, defender, STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD) != STATUS_CHANGE_WORKED)
		return TRUE;
	
	holdEffect = GetBattlerItemHoldEffect(defender, TRUE);
	
	if (holdEffect == HOLD_EFFECT_CURE_BRN || holdEffect == HOLD_EFFECT_CURE_STATUS)
		return TRUE;
	
	ability = GetBattlerAbility(defender);
	
	if (ability == ABILITY_SHED_SKIN
	|| ability == ABILITY_MAGIC_GUARD
	|| ability == ABILITY_QUICK_FEET
	|| (ability == ABILITY_SYNCHRONIZE && CanBeBurned(defender, attacker, 0) != STATUS_CHANGE_WORKED)
	|| (ability == ABILITY_MARVEL_SCALE && BattlerHasPhysicalMove(attacker))
	|| (ability == ABILITY_NATURAL_CURE && CanBattlerSwitchOut(defender, TRUE))
	|| (ability == ABILITY_FLARE_BOOST && BattlerHasSpecialMove(defender))
	|| (ability == ABILITY_GUTS && BattlerHasPhysicalMove(defender))
	|| (ability == ABILITY_HYDRATION && IsBattlerWeatherAffected(defender, B_WEATHER_RAIN_ANY) && gBattleStruct->weatherDuration > 1)
	|| (IsBattlerAlive(BATTLE_PARTNER(defender)) && GetBattlerAbility(BATTLE_PARTNER(defender)) == ABILITY_HEALER)
	|| AI_BattlerHasMoveEffectInMoveset(defender, EFFECT_FACADE)
	|| AI_BattlerHasMoveEffectInMoveset(defender, EFFECT_PSYCHO_SHIFT))
		return TRUE;
		
	return FALSE;
}

u32 GetScoreForInflictBurn(u32 attacker, u32 defender)
{
	// Higher score
	if (BattlerHasPhysicalMove(defender)
	|| SideHasMoveEffectInMovesetThatAffectsTarget(attacker, defender, EFFECT_HEX)
	|| SideHasMoveEffectInMovesetThatAffectsTarget(attacker, defender, EFFECT_INFERNAL_PARADE))
		return +11;
	
	// Medium score
	if (SideHasMoveEffectInMovesetThatAffectsTarget(attacker, defender, EFFECT_SPARKLING_ARIA))
		return +8;
	
	// Low score
	return +5;
}

bool32 BadIdeaToFreeze(u32 attacker, u32 defender)
{
	u32 var, holdEffect, ability;
	
	if (!CanBeFrozen(attacker, defender, STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD))
		return TRUE;
	
	holdEffect = GetBattlerItemHoldEffect(defender, TRUE);
	
	if (holdEffect == HOLD_EFFECT_CURE_FRZ || holdEffect == HOLD_EFFECT_CURE_STATUS)
		return TRUE;
	
	ability = GetBattlerAbility(defender);
	
	if ((ability == ABILITY_SYNCHRONIZE && CanBeFrozen(defender, attacker, 0))
	|| (ability == ABILITY_NATURAL_CURE && CanBattlerSwitchOut(defender, TRUE)))
		return TRUE;
	
	BattlerHasMoveWithFlagInMoveset(var, defender, thawUser)

	return var;
}

u32 GetScoreForFreezeTarget(u32 attacker, u32 defender)
{
	// Higher score
	if (SideHasMoveEffectInMovesetThatAffectsTarget(attacker, defender, EFFECT_HEX))
		return +11;
	
	// Medium score
	return +8;
}

bool32 BadIdeaToParalyze(u32 attacker, u32 defender)
{
	u32 holdEffect, ability;
	
	if (CanBeParalyzed(attacker, defender, STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD) != STATUS_CHANGE_WORKED)
		return TRUE;
	
	holdEffect = GetBattlerItemHoldEffect(defender, TRUE);
	
	if (holdEffect == HOLD_EFFECT_CURE_PAR || holdEffect == HOLD_EFFECT_CURE_STATUS)
		return TRUE;
	
	if (!AIIsFaster(attacker, defender) && (gFieldStatus & STATUS_FIELD_TRICK_ROOM))
		return TRUE;
	
	ability = GetBattlerAbility(defender);
	
	if (ability == ABILITY_SHED_SKIN
	|| ability == ABILITY_QUICK_FEET
	|| (ability == ABILITY_SYNCHRONIZE && CanBeParalyzed(defender, attacker, 0) != STATUS_CHANGE_WORKED)
	|| (ability == ABILITY_MARVEL_SCALE && BattlerHasPhysicalMove(attacker))
	|| (ability == ABILITY_NATURAL_CURE && CanBattlerSwitchOut(defender, TRUE))
	|| (ability == ABILITY_GUTS && BattlerHasPhysicalMove(defender))
	|| (ability == ABILITY_HYDRATION && IsBattlerWeatherAffected(defender, B_WEATHER_RAIN_ANY) && gBattleStruct->weatherDuration > 1)
	|| (IsBattlerAlive(BATTLE_PARTNER(defender)) && GetBattlerAbility(BATTLE_PARTNER(defender)) == ABILITY_HEALER)
	|| AI_BattlerHasMoveEffectInMoveset(defender, EFFECT_FACADE)
	|| AI_BattlerHasMoveEffectInMoveset(defender, EFFECT_PSYCHO_SHIFT))
		return TRUE;
	
	return FALSE;
}

// TODO
u32 GetScoreForInflictParalyze(u32 attacker, u32 defender)
{
	// Higher score
	if (!AIIsFaster(attacker, defender)
	|| SideHasMoveEffectInMovesetThatAffectsTarget(attacker, defender, EFFECT_HEX))
		return +11;
	
	/*// Medium score
	if (SideHasMoveEffectInMovesetThatAffectsTarget(attacker, defender, EFFECT_SMELLING_SALT))
		return +8;*/
	
	// Low score
	return +5;
}

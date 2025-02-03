#include "global.h"
#include "battle.h"
#include "battle_ai.h"
#include "battle_ai_util.h"
#include "battle_move_effects.h"
#include "battle_util.h"
#include "set_effect.h"
#include "constants/hold_effects.h"
#include "constants/moves.h"

#define CHANGE_SCORE(val) score += val

/*
#define RET_SCORE(val) \
{                      \
	CHANGE_SCORE(val); \
	return score;      \
}*/

// Extra args are primary, affectsUser and flags, see DoMoveEffect for more info
#define CHECK_EFFECT_BLOCK(moveEffect, ...) ((CheckSecondaryEffectsBlockers(attacker, defender, move, moveEffect, DEFAULT_2(FALSE, __VA_ARGS__), DEFAULT(FALSE, __VA_ARGS__), DEFAULT_3(0, __VA_ARGS__))))

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

// Decrease score if move will fail
s8 BattleAIFunc_CheckBadMove(u8 attacker, u8 defender, u8 moveSlot, u16 move, s8 score)
{
	if (TargetImuneToMove(attacker, defender, moveSlot))
		CHANGE_SCORE(-10);
	
	switch (gBattleMoves[move].effect)
	{
		case EFFECT_OHKO:
			if (!KanOHKOBattler(attacker, defender, move, FALSE))
				CHANGE_SCORE(-10);
			break;
		case EFFECT_TWO_TURNS_ATTACK:
			if (GetBattlerItemHoldEffect(attacker, TRUE) != HOLD_EFFECT_POWER_HERB && GetNoOfHitsToKOBattlerHigherDamage(defender, attacker) == 1)
				CHANGE_SCORE(-10);
			break;
		
		/*case EFFECT_FREEZE_HIT:
			if (CHECK_EFFECT_BLOCK(MOVE_EFFECT_FREEZE, FALSE, FALSE, STATUS_CHANGE_FLAG_IGNORE_SUBSTITUTE))
				CHANGE_SCORE(-10);
			break;
		case EFFECT_PARALYZE_HIT:
			if (CHECK_EFFECT_BLOCK(MOVE_EFFECT_PARALYSIS, FALSE, FALSE, STATUS_CHANGE_FLAG_IGNORE_SUBSTITUTE))
				CHANGE_SCORE(-10);
			break;
		case EFFECT_USER_ATTACK_UP_2:
			if (!CompareStat(attacker, STAT_ATK, MAX_STAT_STAGES, CMP_LESS_THAN))
				CHANGE_SCORE(-10);
			break;
		case EFFECT_ROAR:
			if (defAbility == ABILITY_GUARD_DOG || defAbility == ABILITY_SUCTION_CUPS || (gStatus3[defender] & STATUS3_ROOTED)
			|| DoesRandomSwitchoutFails(attacker, target))*/
		case EFFECT_WILL_O_WISP:
			if (CHECK_EFFECT_BLOCK(MOVE_EFFECT_BURN, TRUE))
				CHANGE_SCORE(-10);
			break;
		case EFFECT_SET_PARALYZE:
			if (CHECK_EFFECT_BLOCK(MOVE_EFFECT_PARALYSIS, TRUE))
				CHANGE_SCORE(-10);
			break;
	}
	return score;
}

s8 BattleAIFunc_CheckViability(u8 attacker, u8 defender, u8 moveSlot, u16 move, s8 score)
{
	switch (gBattleMoves[move].effect)
	{
		/*case EFFECT_BURN_HIT:
		BURN_CHANCE_CHECK:
			if (GetMoveEffectChanceIncreases(attacker, move) >= 75 && !CHECK_EFFECT_BLOCK(MOVE_EFFECT_BURN))
				goto AI_BURN_CHECKS;
			break;
		case EFFECT_FREEZE_HIT:
			if (GetMoveEffectChanceIncreases(attacker, move) >= 75 && !CHECK_EFFECT_BLOCK(MOVE_EFFECT_FREEZE))
			{
			AI_FREEZE_CHECKS:
				if (!BadIdeaToFreeze(attacker, defender))
					CHANGE_SCORE(GetScoreForFreezeTarget(attacker, defender));
			}
			break;
		case EFFECT_PARALYZE_HIT:
		PARALYZE_CHANCE_CHECK:
			if (GetMoveEffectChanceIncreases(attacker, move) >= 75 && !CHECK_EFFECT_BLOCK(MOVE_EFFECT_PARALYSIS))
				goto AI_PARALYZE_CHECKS;
			break;
		case EFFECT_OHKO:
			if (gBattleMons[defender].hp > gBattleMons[defender].maxHP * 2 / 3 && GetOHKOChance(attacker, defender, move) >= 70
			&& GetHitDamageResult(defender, move, TRUE) == 0)
				CHANGE_SCORE(+11);
			else
				CHANGE_SCORE(-10);
			break;
		case EFFECT_TWO_TURNS_ATTACK:
			switch (gBattleMoves[move].argument)
			{
				case MOVE_EFFECT_FLINCH:
					goto FLINCH_CHANCE_CHECK;
				case MOVE_EFFECT_PARALYSIS:
					goto PARALYZE_CHANCE_CHECK;
				case MOVE_EFFECT_BURN:
					goto BURN_CHANCE_CHECK;
			}
			break;
		case EFFECT_FLINCH_HIT:
		FLINCH_CHANCE_CHECK:
			if (GetMoveEffectChanceIncreases(attacker, move) >= 75 && !CHECK_EFFECT_BLOCK(MOVE_EFFECT_FLINCH))
				goto AI_FLINCH_CHECKS;
			break;
		case EFFECT_USER_ATTACK_UP_2:
		{
			u8 ret = GetStatUpScore(attacker, defender, STAT_ATK, +2, FALSE);
			
			if (ret)
				CHANGE_SCORE((MAX_STAT_STAGES - ret) * 2);
			else
				CHANGE_SCORE(-10);
			
			break;
		}
			
			
			
			
		case EFFECT_WILL_O_WISP:
		AI_BURN_CHECKS:
			if (!BadIdeaToBurn(attacker, defender))
				CHANGE_SCORE(GetScoreForInflictBurn(attacker, defender));
			break;
		case EFFECT_SET_PARALYZE:
		AI_PARALYZE_CHECKS:
			if (!BadIdeaToParalyze(attacker, defender))
				CHANGE_SCORE(GetScoreForInflictParalyze(attacker, defender));
			break;
		case EFFECT_FAKE_OUT:
		AI_FLINCH_CHECKS:
			break;*/
	}
	return score;
}

s8 BattleAIFunc_Safari(u8 attacker, u8 defender, u8 moveSlot, u16 move, s8 score)
{
	return score;
}

s8 BattleAIFunc_Roamer(u8 attacker, u8 defender, u8 moveSlot, u16 move, s8 score)
{
	return score;
}

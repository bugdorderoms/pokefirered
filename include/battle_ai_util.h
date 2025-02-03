#ifndef GUARD_BATTLE_AI_UTIL_H
#define GUARD_BATTLE_AI_UTIL_H

u16 GetAllyChosenMove(u8 battlerId);
bool8 AIShouldConsiderMoveForBattler(u8 attacker, u8 defender, u16 move);
u8 AI_GetStatChangeScore(u8 attacker, u8 defender, u8 statId, s8 stages, bool8 toUp, bool8 checkEffectsBlock);
bool8 ShouldAIIncreaseCriticalChance(u8 attacker, u8 defender);
u32 GetNoOfHitsToKOBattler(u8 attacker, u8 defender, u8 moveIndex);
u32 GetNoOfHitsToKOBattlerDmg(s32 dmg, u8 battler);
u32 GetNoOfHitsToKOBattlerHigherDamage(u8 attacker, u8 defender);
u8 GetNumMovesWithSplitInBattlerMoveset(u8 battlerId, u8 split);
bool8 BattlerHasMoveEffectInMoveset(u8 battler, u16 moveEffect);
bool8 SideHasMoveEffectInMovesetThatAffectsTarget(u8 attacker, u8 target, u16 moveEffect);
bool8 TargetImuneToMove(u8 attacker, u8 defender, u8 moveSlot);
bool8 AIIsFaster(u8 battler1, u8 battler2);
bool8 BadIdeaToBurn(u8 attacker, u8 defender);
u8 GetScoreForInflictBurn(u8 attacker, u8 defender);
bool8 BadIdeaToFreeze(u8 attacker, u8 defender);
u8 GetScoreForFreezeTarget(u8 attacker, u8 defender);
bool8 BadIdeaToParalyze(u8 attacker, u8 defender);
u8 GetScoreForInflictParalyze(u8 attacker, u8 defender);

#define GetStatUpScore(attacker, defender, statId, stages, checkEffectsBlock) AI_GetStatChangeScore(attacker, defender, statId, stages, TRUE, checkEffectsBlock)
#define GetStatDownScore(attacker, defender, statId, stages, checkEffectsBlock) AI_GetStatChangeScore(attacker, defender, statId, stages, FALSE, checkEffectsBlock)

#define BattlerHasPhysicalMove(battlerId) ((GetNumMovesWithSplitInBattlerMoveset(battlerId, SPLIT_PHYSICAL) > 0))
#define BattlerHasSpecialMove(battlerId) ((GetNumMovesWithSplitInBattlerMoveset(battlerId, SPLIT_SPECIAL) > 0))

#define BattlerHasMoveWithFlagInMoveset(var, battlerId, flag)                                              \
{                                                                                                          \
	u8 i;                                                                                                  \
	                                                                                                       \
	var = FALSE;                                                                                           \
                                                                                                           \
	for (i = 0; i < MAX_MON_MOVES; i++)                                                                    \
	{                                                                                                      \
		if (AI_THINKING->moves[battlerId][i] && gBattleMoves[AI_THINKING->moves[battlerId][i]].flags.flag) \
		{                                                                                                  \
			var = TRUE;                                                                                    \
			break;                                                                                         \
		}                                                                                                  \
	}                                                                                                      \
}

bool8 BattleAI_KnowsBattlerItem(u8 battlerId);
bool8 BattleAI_KnowsBattlerMoveIndex(u8 battlerId, u8 moveIndex);
bool8 BattleAI_KnowsBattlerPartyIndex(u8 battlerId);
bool8 BattleAI_KnowsBattlerAbility(u8 battlerId);
void BattleAI_RecordAbility(u8 battlerId);
void BattleAI_RecordHoldEffect(u8 battlerId);
void BattleAI_RecordMoveUsed(u8 battlerId, u8 moveSlot);
void BattleAI_RecordPartyIndex(u8 battlerId);

#endif // GUARD_BATTLE_AI_UTIL_H

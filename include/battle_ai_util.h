#ifndef GUARD_BATTLE_AI_UTIL_H
#define GUARD_BATTLE_AI_UTIL_H

u32 GetAllyChosenMove(u32 battlerId);
bool32 AIShouldConsiderMoveForBattler(u32 attacker, u32 defender, u32 move);
u32 AI_GetStatChangeScore(u32 attacker, u32 defender, u32 statId, s32 stages, bool32 toUp, bool32 checkEffectsBlock);
bool32 ShouldAIIncreaseCriticalChance(u32 attacker, u32 defender);
u32 GetNoOfHitsToKOBattler(u32 attacker, u32 defender, u32 moveIndex);
u32 GetNoOfHitsToKOBattlerDmg(s32 dmg, u32 battler);
u32 GetNoOfHitsToKOBattlerHigherDamage(u32 attacker, u32 defender);
u32 GetNumMovesWithSplitInBattlerMoveset(u32 battlerId, u32 split);
bool32 BattlerHasMoveEffectInMoveset(u32 battler, u32 moveEffect);
bool32 SideHasMoveEffectInMovesetThatAffectsTarget(u32 attacker, u32 target, u32 moveEffect);
bool32 TargetImuneToMove(u32 attacker, u32 defender, u32 moveSlot);
bool32 AIIsFaster(u32 battler1, u32 battler2);
bool32 BadIdeaToBurn(u32 attacker, u32 defender);
u32 GetScoreForInflictBurn(u32 attacker, u32 defender);
bool32 BadIdeaToFreeze(u32 attacker, u32 defender);
u32 GetScoreForFreezeTarget(u32 attacker, u32 defender);
bool32 BadIdeaToParalyze(u32 attacker, u32 defender);
u32 GetScoreForInflictParalyze(u32 attacker, u32 defender);

#define GetStatUpScore(attacker, defender, statId, stages, checkEffectsBlock) AI_GetStatChangeScore(attacker, defender, statId, stages, TRUE, checkEffectsBlock)
#define GetStatDownScore(attacker, defender, statId, stages, checkEffectsBlock) AI_GetStatChangeScore(attacker, defender, statId, stages, FALSE, checkEffectsBlock)

#define BattlerHasPhysicalMove(battlerId) ((GetNumMovesWithSplitInBattlerMoveset(battlerId, SPLIT_PHYSICAL) > 0))
#define BattlerHasSpecialMove(battlerId) ((GetNumMovesWithSplitInBattlerMoveset(battlerId, SPLIT_SPECIAL) > 0))

#define BattlerHasMoveWithFlagInMoveset(var, battlerId, flag)                                              \
{                                                                                                          \
	u32 i;                                                                                                 \
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

bool32 BattleAI_KnowsBattlerItem(u32 battlerId);
bool32 BattleAI_KnowsBattlerMoveIndex(u32 battlerId, u32 moveIndex);
bool32 BattleAI_KnowsBattlerPartyIndex(u32 battlerId);
bool32 BattleAI_KnowsBattlerAbility(u32 battlerId);
void BattleAI_RecordAbility(u32 battlerId);
void BattleAI_RecordHoldEffect(u32 battlerId);
void BattleAI_RecordMoveUsed(u32 battlerId, u32 moveSlot);
void BattleAI_RecordPartyIndex(u32 battlerId);

#endif // GUARD_BATTLE_AI_UTIL_H

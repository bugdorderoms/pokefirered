#include "global.h"
#include "battle.h"
#include "battle_ai.h"
#include "battle_ai_util.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "battle_util.h"
#include "calculate_base_damage.h"
#include "random.h"
#include "util.h"
#include "constants/battle.h"
#include "constants/moves.h"

#define AI_ACTION_DONT_ATTACK (1 << 0)
#define AI_ACTION_FLEE        (1 << 1)
#define AI_ACTION_WATCH       (1 << 2)
#define AI_ACTION_DONE        (1 << 3)

static u8 BattleAI_ChooseMoveOrAction_Singles(u8 battlerId);
static u8 BattleAI_ChooseMoveOrAction_Doubles(u8 battlerId);
static void BattleAI_DoAiProcessing(u8 attacker, u8 logicId, u8 target);

static EWRAM_DATA bool8 sBattleAI_IsRunnig = FALSE;

static s8 (*const sBattleAiFuncsTable[])(u8, u8, u8, u16, s8) =
{
	[0] = BattleAIFunc_CheckBadMove,     // AI_FLAG_CHECK_BAD_MOVE
	[1] = BattleAIFunc_CheckViability,   // AI_FLAG_CHECK_VIABILITY
	/*[2] = BattleAIFunc_TryToFaint,       // AI_FLAG_TRY_TO_FAINT*/
	[3] = NULL,                          // AI_FLAG_OMNISCIENT
	[4] = BattleAIFunc_Safari,           // AI_FLAG_SAFARI
	[5] = BattleAIFunc_Roamer,           // AI_FLAG_ROAMER
	/*[6] = BattleAIFunc_PreferStrongMove, // AI_FLAG_PREFER_STRONGEST_MOVE
	[7] = BattleAIFunc_PredictSwitch,    // AI_FLAG_PREDICT_SWITCH
	[8] = BattleAIFunc_DoubleBattle,     // AI_FLAG_DOUBLE_BATTLE
	[9] = BattleAIFunc_HPAware,          // AI_FLAG_HP_AWARE*/
	[10 ... 31] = NULL,
};

void BattleAI_SetupAILogicData(void)
{
	u8 i, count;
	u32 averageLevel;
	
	if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
	{
		if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
			AI_DATA->aiFlags = AI_FLAG_SAFARI;
		else if (gBattleTypeFlags & BATTLE_TYPE_ROAMER)
			AI_DATA->aiFlags = AI_FLAG_ROAMER;
		else
		{
			if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER)) // Wild
			{
				averageLevel = 0;
				count = 0;
				
				for (i = 0; i < PARTY_SIZE; i++)
				{
					if (MonCanBattle(&gEnemyParty[i]))
					{
						averageLevel += GetMonData(&gEnemyParty[i], MON_DATA_LEVEL);
						count++;
					}
				}
				averageLevel /= count;

				AI_DATA->aiFlags = AI_FLAG_CHECK_BAD_MOVE;
				if (averageLevel >= 20)
					AI_DATA->aiFlags |= AI_FLAG_CHECK_VIABILITY;
				if (averageLevel >= 60)
					AI_DATA->aiFlags |= AI_FLAG_PREFER_STRONGEST_MOVE;
				if (averageLevel >= 80)
					AI_DATA->aiFlags |= AI_FLAG_HP_AWARE;
			}
			else // Trainer
			{
				AI_DATA->aiFlags = AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_TRY_TO_FAINT | AI_FLAG_CHECK_VIABILITY;
				AI_DATA->aiFlags |= gTrainers[gTrainerBattleOpponent_A].aiFlags;
				
				// Set trainer knowing all player's data if AI_FLAG_OMNISCIENT is set
				if (AI_DATA->aiFlags & AI_FLAG_OMNISCIENT)
				{
					AI_DATA->knownPartyIndices = 0xFF;
					AI_DATA->knownPlayerAbilities = 0xFF;
					AI_DATA->knownPlayerItems = 0xFF;
					
					for (i = 0; i < PARTY_SIZE; i++)
						AI_DATA->knownPlayerMoves[i] = 0xFF;
				}
			}
		}
	}
	BattleAI_SetAILogicDataForTurn();
}

static void SetBattlerData(u8 attacker)
{
	u8 i, defender;
	
	AI_THINKING->predictedMoves[attacker] = gBattleStruct->battlers[attacker].lastMove;
	AI_THINKING->moveLimitations[attacker] = CheckMoveLimitations(attacker, 0);
	AI_THINKING->totalSpeeds[attacker] = GetBattlerTotalSpeed(attacker);
	
	GetBattlerMovesArray(attacker, &AI_THINKING->moves[attacker][0]);
	
	// Don't consider moves that can't be used
	for (i = 0; i < MAX_MON_MOVES; i++)
	{
		if (AI_THINKING->moveLimitations[attacker] & gBitTable[i])
			AI_THINKING->moves[attacker][i] = MOVE_NONE;
	}
	
	for (defender = 0; defender < gBattlersCount; defender++)
	{
		s32 maxDmg = 0;
		
		if (attacker != defender && IsBattlerAlive(defender))
		{
			for (i = 0; i < MAX_MON_MOVES; i++)
			{
				s32 dmg = 0;
				u16 flags, move = AI_THINKING->moves[attacker][i];
				
				if (move)
				{
					u8 moveType = AI_THINKING->moveTypes[attacker][i] = GetBattlerMoveType(attacker, move);
					
					AI_THINKING->totalAccuracy[attacker][defender][i] = CalcMoveTotalAccuracy(move, attacker, defender);
					AI_THINKING->effectiveness[attacker][defender][i] = TypeCalc(move, moveType, attacker, defender, FALSE, &flags);
					dmg = AI_CalcMoveDamage(move, attacker, defender, moveType, AI_THINKING->effectiveness[attacker][defender][i]);
					
					if (dmg > maxDmg)
						maxDmg = dmg;
				}
				AI_THINKING->simulatedDmg[attacker][defender][i] = dmg;
			}
		}
		AI_THINKING->higherDamage[attacker][defender] = maxDmg;
	}
}

void BattleAI_SetAILogicDataForTurn(void)
{
	if (BATTLE_TYPE_HAS_AI)
	{
		u8 attacker;
		
		sBattleAI_IsRunnig = TRUE;
		
		for (attacker = 0; attacker < gBattlersCount; attacker++)
		{
			if (IsBattlerAlive(attacker))
				SetBattlerData(attacker);
		}
		sBattleAI_IsRunnig = FALSE;
	}
}

void BattleAI_ChooseAction(u8 battlerId)
{
	if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
	{
		if (BattleAI_ShouldSwitch(battlerId))
		{
			u8 battlerIn1, battlerIn2;
			
			if (gBattleStruct->AI_monToSwitchIntoId[GetBattlerPosition(battlerId) >> 1] == PARTY_SIZE)
			{
				u8 id = GetMostSuitableMonToSwitchInto(battlerId);
				
				if (id == PARTY_SIZE)
				{
					if (!IsDoubleBattleOnSide(B_SIDE_OPPONENT))
					{
						battlerIn1 = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
                        battlerIn2 = battlerIn1;
					}
					else
					{
						battlerIn1 = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
                        battlerIn2 = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
					}
					
					for (id = 0; id < PARTY_SIZE; id++)
					{
						if (GetMonData(&gEnemyParty[id], MON_DATA_HP) && id != gBattlerPartyIndexes[battlerIn1] && id != gBattlerPartyIndexes[battlerIn2]
						&& id != gBattleStruct->battlers[battlerIn1].monToSwitchIntoId && id != gBattleStruct->battlers[battlerIn2].monToSwitchIntoId)
						break;
					}
				}
				gBattleStruct->AI_monToSwitchIntoId[GetBattlerPosition(battlerId) >> 1] = id;
			}
			gBattleStruct->battlers[battlerId].monToSwitchIntoId = gBattleStruct->AI_monToSwitchIntoId[GetBattlerPosition(battlerId) >> 1];
			BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_SWITCH, 0);
			return;
		}
		else if (BattleAI_ShouldUseItem(battlerId))
			return;
	}
	BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_USE_MOVE, (BATTLE_OPPOSITE(battlerId) << 8));
}

bool8 BattleAI_IsRunning(void)
{
	return sBattleAI_IsRunnig;
}

void BattleAI_ComputeMovesScore(u8 battlerId)
{
	u8 i;
	
	sBattleAI_IsRunnig = TRUE;
	
	if (!IsDoubleBattleForBattler(battlerId))
		gBattleStruct->battlers[battlerId].aiMoveOrAction = BattleAI_ChooseMoveOrAction_Singles(battlerId);
	else
		gBattleStruct->battlers[battlerId].aiMoveOrAction = BattleAI_ChooseMoveOrAction_Doubles(battlerId);
	
	sBattleAI_IsRunnig = FALSE;
}

static void BattleAI_InitMovesScore(u8 battlerId)
{
	u8 i, j;
	
	for (i = 0; i < MAX_MON_MOVES; i++)
	{
		AI_THINKING->score[battlerId][i] = (AI_THINKING->moveLimitations[battlerId] & gBitTable[i]) ? 0 : 100;
		AI_THINKING->simulatedRNG[battlerId][i] = 100 - (Random() % 16);
	}
	AI_THINKING->action[battlerId] = 0;
	gBattleStruct->battlers[battlerId].aiChosenTarget = gBattlerTarget = SetRandomTarget(battlerId);
}

static u8 BattleAI_ChooseMoveOrAction_Singles(u8 battlerId)
{
	u8 currentMoveArray[MAX_MON_MOVES];
    u8 consideredMoveArray[MAX_MON_MOVES];
	u8 i, numOfBestMoves, logicId = 0;
	u32 flags = AI_DATA->aiFlags;
	
	BattleAI_InitMovesScore(battlerId);
	AI_THINKING->partnerMove = MOVE_NONE; // No ally
	
	while (flags)
	{
		if (flags & 1)
			BattleAI_DoAiProcessing(battlerId, logicId, gBattlerTarget);
		
		flags >>= 1;
		logicId++;
	}
	
	if (AI_THINKING->action[battlerId] & AI_ACTION_FLEE)
		return AI_CHOICE_FLEE;
	else if (AI_THINKING->action[battlerId] & AI_ACTION_WATCH)
		return AI_CHOICE_WATCH;
	else if (BattleAI_SwitchIfBadMoves(battlerId, FALSE))
		return AI_CHOICE_SWITCH;
	
	numOfBestMoves = 1;
	currentMoveArray[0] = AI_THINKING->score[battlerId][0];
	consideredMoveArray[0] = 0;
	
	for (i = 1; i < MAX_MON_MOVES; i++)
	{
		if (!gBattleMons[battlerId].moves[i])
			continue;
		
		if (currentMoveArray[0] < AI_THINKING->score[battlerId][i])
		{
			currentMoveArray[0] = AI_THINKING->score[battlerId][i];
			consideredMoveArray[0] = i;
			numOfBestMoves = 1;
		}
		
		if (currentMoveArray[0] == AI_THINKING->score[battlerId][i])
		{
			currentMoveArray[numOfBestMoves] = AI_THINKING->score[battlerId][i];
			consideredMoveArray[numOfBestMoves] = i;
			++numOfBestMoves;
		}
	}
	return consideredMoveArray[RandomMax(numOfBestMoves)];
}

static u8 BattleAI_ChooseMoveOrAction_Doubles(u8 battlerId)
{
	u8 actionOrMoveIndex[MAX_BATTLERS_COUNT];
	s8 bestMovePointsForTarget[MAX_BATTLERS_COUNT];
	u8 mostViableTargetsArray[MAX_BATTLERS_COUNT];
	s8 mostViableMovesScores[MAX_MON_MOVES];
    u8 mostViableMovesIndices[MAX_MON_MOVES];
    u8 mostViableMovesNo;
	u8 mostViableTargetsNo;
	s8 mostMovePoints;
	u8 i, j, logicId;
	u32 flags;
	
	for (i = 0; i < MAX_BATTLERS_COUNT; i++)
	{
		if (i == battlerId || !IsBattlerAlive(i))
		{
			actionOrMoveIndex[i] = 0xFF;
			bestMovePointsForTarget[i] = -1;
		}
		else
		{
			BattleAI_InitMovesScore(battlerId);
			AI_THINKING->partnerMove = GetAllyChosenMove(battlerId);
			AI_THINKING->movesetMoveId[battlerId] = 0;
			flags = AI_DATA->aiFlags;
			logicId = 0;
			
			gBattlerTarget = i;
			
			while (flags)
			{
				if (flags & 1)
					BattleAI_DoAiProcessing(battlerId, logicId, i);
				
				flags >>= 1;
				logicId++;
			}
			
			if (AI_THINKING->action[battlerId] & AI_ACTION_FLEE)
				actionOrMoveIndex[i] = AI_CHOICE_FLEE;
			else if (AI_THINKING->action[battlerId] & AI_ACTION_WATCH)
				actionOrMoveIndex[i] = AI_CHOICE_WATCH;
			else
			{
				mostViableMovesScores[0] = AI_THINKING->score[battlerId][0];
				mostViableMovesIndices[0] = 0;
				mostViableMovesNo = 1;
				
				for (j = 1; j < MAX_MON_MOVES; j++)
				{
					u16 move = gBattleMons[battlerId].moves[j];
					
					if (!move || !CanTargetBattler(battlerId, i, move, GetBattlerMoveTargetType(battlerId, move)))
						continue;
					
					if (mostViableMovesScores[0] < AI_THINKING->score[battlerId][j])
					{
						mostViableMovesScores[0] = AI_THINKING->score[battlerId][j];
						mostViableMovesIndices[0] = j;
						mostViableMovesNo = 1;
					}
					
					if (mostViableMovesScores[0] == AI_THINKING->score[battlerId][j])
					{
						mostViableMovesScores[mostViableMovesNo] = AI_THINKING->score[battlerId][j];
						mostViableMovesIndices[mostViableMovesNo] = j;
						++mostViableMovesNo;
					}
				}
				actionOrMoveIndex[i] = mostViableMovesIndices[RandomMax(mostViableMovesNo)];
				bestMovePointsForTarget[i] = mostViableMovesScores[0];
				
				if (i == BATTLE_PARTNER(battlerId) && bestMovePointsForTarget[i] < 100)
					bestMovePointsForTarget[i] = -1;
			}
		}
	}
	
	if (BattleAI_SwitchIfBadMoves(battlerId, TRUE))
		return AI_CHOICE_SWITCH;
	
	mostMovePoints = bestMovePointsForTarget[0];
	mostViableTargetsArray[0] = 0;
	mostViableTargetsNo = 1;
	
	for (i = 1; i < MAX_BATTLERS_COUNT; i++)
	{
		if (mostMovePoints < bestMovePointsForTarget[i])
		{
			mostMovePoints = bestMovePointsForTarget[i];
			mostViableTargetsArray[0] = i;
			mostViableTargetsNo = 1;
		}
		
		if (mostMovePoints == bestMovePointsForTarget[i])
		{
			mostViableTargetsArray[mostViableTargetsNo] = i;
			++mostViableTargetsNo;
		}
	}
	gBattleStruct->battlers[battlerId].aiChosenTarget = gBattlerTarget = mostViableTargetsArray[RandomMax(mostViableTargetsNo)];
	
	return actionOrMoveIndex[gBattlerTarget];
}

static void BattleAI_DoAiProcessing(u8 attacker, u8 logicId, u8 target)
{
	u8 moveIndex;
	u16 move;
	
	do
	{
		moveIndex = AI_THINKING->movesetMoveId[attacker];
		
		if (!gBattleMons[attacker].pp[moveIndex])
			move = MOVE_NONE;
		else
			move = gBattleMons[attacker].moves[moveIndex];
		
		if (move && AI_THINKING->score[attacker][moveIndex] > 0 && AIShouldConsiderMoveForBattler(attacker, target, move))
		{
			if (logicId < ARRAY_COUNT(sBattleAiFuncsTable) && sBattleAiFuncsTable[logicId] != NULL)
			{
				gCurrentTurnActionBattlerId = gBattlerAttacker = attacker;
				gBattlerTarget = target;
				gCurrentMove = move;
				gBattleStruct->dynamicMoveType = AI_THINKING->moveTypes[attacker][moveIndex];
				
				AI_THINKING->score[attacker][moveIndex] = sBattleAiFuncsTable[logicId](attacker, target, moveIndex, move, AI_THINKING->score[attacker][moveIndex]);
			}
		}
		else
			AI_THINKING->score[attacker][moveIndex] = 0;
		
		AI_THINKING->movesetMoveId[attacker]++;
		
	} while (AI_THINKING->movesetMoveId[attacker] < MAX_MON_MOVES && !(AI_THINKING->action[attacker] & AI_ACTION_DONT_ATTACK));
	
	AI_THINKING->movesetMoveId[attacker] = 0;
}

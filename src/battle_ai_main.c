#include "global.h"
#include "battle.h"
#include "battle_ai.h"
#include "battle_ai_util.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "battle_util.h"
#include "battle_damage_calc.h"
#include "malloc.h"
#include "random.h"
#include "recorded_battle.h"
#include "util.h"
#include "constants/battle.h"
#include "constants/moves.h"

static u32 BattleAI_ChooseMoveOrAction_Singles(u32 battlerId, u32 aiFlags);
static u32 BattleAI_ChooseMoveOrAction_Doubles(u32 battlerId, u32 aiFlags);
static void BattleAI_DoAIProcessing(u32 attacker, u32 logicId, u32 target);

EWRAM_DATA struct AIData *gAIData = NULL;
static EWRAM_DATA bool8 sBattleAI_IsRunnig = FALSE;

static s8 (*const sBattleAiFuncsTable[])(struct AIScript*, s8) =
{
    [0] = BattleAIFunc_CheckBadMove,     // AI_FLAG_CHECK_BAD_MOVE
    [1] = BattleAIFunc_CheckGoodMove,    // AI_FLAG_CHECK_GOOD_MOVE
    [2] = BattleAIFunc_Safari,           // AI_FLAG_SAFARI
    [3] = BattleAIFunc_Roamer,           // AI_FLAG_ROAMER
    // [4] = BattleAIFunc_PredictSwitch,    // AI_FLAG_PREDICT_SWITCH
    [5 ... 31] = NULL,
};

static void BattleAI_SetupAILogicDataForSide(u32 side)
{
    u32 i;
    
    // Basic AI flags
    if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
        gAIData->logic[side].aiFlags = GetAiScriptsInRecordedBattle();
    else if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
    {
        gAIData->logic[side].aiFlags = AI_FLAG_SAFARI;
        return;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_ROAMER)
    {
        gAIData->logic[side].aiFlags = AI_FLAG_ROAMER;
        return;
    }
    else if (side == B_SIDE_PLAYER && (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER))
    {
        gAIData->logic[side].aiFlags = gBattlePartners[gPartnerTrainerId].aiFlags;
        
        for (i = 0; i < MAX_TRAINER_ITEMS; i++)
            gAIData->logic[side].items[i] = gBattlePartners[gPartnerTrainerId].items[i];
    }
    else if ((gBattleTypeFlags & BATTLE_TYPE_TRAINER) && !(gBattleTypeFlags & BATTLE_TYPE_LINK))
    {
        gAIData->logic[side].aiFlags = gTrainers[gTrainerBattleOpponent_A].aiFlags;
        
        for (i = 0; i < MAX_TRAINER_ITEMS; i++)
            gAIData->logic[side].items[i] = gTrainers[gTrainerBattleOpponent_A].items[i];
    }
    else // Wild
    {
        gAIData->logic[side].aiFlags = 0;
        return;
    }
    gAIData->logic[side].aiFlags |= (AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_GOOD_MOVE);
}

void BattleAI_SetupAILogicData(void)
{
    u32 i;
    
    for (i = 0; i < B_SIDE_COUNT; i++)
        BattleAI_SetupAILogicDataForSide(i);
    
    BattleAI_SetAILogicDataForTurn();
}

static void SetBattlerData(u32 attacker)
{
    u32 i, defender;
    
    gAIData->thinking[attacker].predictedMove = gBattleStruct->battlers[attacker].lastMove == MOVE_UNAVAILABLE ? MOVE_NONE : gBattleStruct->battlers[attacker].lastMove;
    gAIData->thinking[attacker].moveLimitations = CheckMoveLimitations(attacker, 0);
    gAIData->thinking[attacker].totalSpeed = GetBattlerTotalSpeed(attacker);

    // Don't consider moves that can't be used
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        gAIData->thinking[attacker].moves[i] = gBattleMons[attacker].moves[i];
        
        if (gAIData->thinking[attacker].moveLimitations & Bit(i))
            gAIData->thinking[attacker].moves[i] = MOVE_NONE;
    }
    
    for (defender = 0; defender < gBattlersCount; defender++)
    {
        s32 maxDmg = 0;
        u32 maxDmgMove = MOVE_NONE;
        
        if (attacker != defender && IsBattlerAlive(defender))
        {
            for (i = 0; i < MAX_MON_MOVES; i++)
            {
                s32 dmg = 0;
                u32 move = gAIData->thinking[attacker].moves[i];
                u16 flags;
                
                if (move)
                {
                    u32 moveType = gAIData->thinking[attacker].moveTypes[i] = GetBattlerMoveType(attacker, move);
                    
                    gAIData->thinking[attacker].targets[defender].totalAccuracy[i] = CalcMoveTotalAccuracy(move, attacker, defender);
                    gAIData->thinking[attacker].targets[defender].effectiveness[i] = TypeCalc(move, moveType, attacker, defender, FALSE, FALSE, &flags);
                    dmg = AI_CalcMoveDamage(move, i, attacker, defender, moveType, gAIData->thinking[attacker].targets[defender].effectiveness[i]);
                    
                    if (dmg > maxDmg)
                    {
                        maxDmg = dmg;
                        maxDmgMove = move;
                    }
                }
                gAIData->thinking[attacker].targets[defender].simulatedDmg[i] = dmg;
            }
        }
        gAIData->thinking[attacker].targets[defender].higherDamageMove = maxDmgMove;
        gAIData->thinking[attacker].targets[defender].higherDamage = maxDmg;
    }
}

void BattleAI_SetAILogicDataForTurn(void)
{
    // Only set if battle type has AI
    if ((gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_SAFARI | BATTLE_TYPE_ROAMER | BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_RAID)))
    {
        u32 attacker;
        
        sBattleAI_IsRunnig = TRUE;
        
        for (attacker = 0; attacker < gBattlersCount; attacker++)
        {
            if (IsBattlerAlive(attacker))
                SetBattlerData(attacker);
        }
        sBattleAI_IsRunnig = FALSE;
    }
}

void BattleAI_ChooseAction(u32 battlerId)
{
    if (BattleAI_ShouldSwitch(battlerId))
    {
        if (gBattleStruct->battlers[battlerId].AI_monToSwitchIntoId == PARTY_SIZE)
        {
            u32 id = GetMostSuitableMonToSwitchInto(battlerId);
            
            if (id == PARTY_SIZE)
            {
                u8 viableMons[PARTY_SIZE];
                GetViableMonsToSwitchInto(battlerId, viableMons);
                id = viableMons[0];
            }
            gBattleStruct->battlers[battlerId].AI_monToSwitchIntoId = id;
        }
        gBattleStruct->battlers[battlerId].monToSwitchIntoId = gBattleStruct->battlers[battlerId].AI_monToSwitchIntoId;
        BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_SWITCH, 0);
        return;
    }
    else if (BattleAI_ShouldUseItem(battlerId))
        return;
    
    BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_USE_MOVE, (BATTLE_OPPOSITE(battlerId) << 8));
}

bool32 BattleAI_IsRunning(void)
{
    return sBattleAI_IsRunnig;
}

void BattleAI_ComputeMovesScore(u32 battlerId)
{
    u32 aiFlags = gAIData->logic[GetBattlerSide(battlerId)].aiFlags;
    
    sBattleAI_IsRunnig = TRUE;
    
    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
        gBattleStruct->battlers[battlerId].aiMoveOrAction = BattleAI_ChooseMoveOrAction_Singles(battlerId, aiFlags);
    else
        gBattleStruct->battlers[battlerId].aiMoveOrAction = BattleAI_ChooseMoveOrAction_Doubles(battlerId, aiFlags);
    
    sBattleAI_IsRunnig = FALSE;
}

#define DEFAULT_MOVE_SCORE 60

static void BattleAI_InitMovesScore(u32 battlerId)
{
    u32 i;
    
    for (i = 0; i < MAX_MON_MOVES; i++)
        gAIData->thinking[battlerId].score[i] = (gAIData->thinking[battlerId].moveLimitations & Bit(i)) ? 0 : DEFAULT_MOVE_SCORE;

    gAIData->thinking[battlerId].action = 0;
    gBattleStruct->battlers[battlerId].aiChosenTarget = gBattlerTarget = SetRandomTarget(battlerId);
}

static u32 GetAllyChosenMove(u32 battlerId)
{
    u32 ally = BATTLE_PARTNER(battlerId);
    
    if (!IsBattlerAlive(ally))
        return MOVE_NONE;
    else if (ally > battlerId)
        return gBattleStruct->battlers[ally].lastMove;
    else
        return gBattleMons[ally].moves[gBattleStruct->battlers[ally].chosenMovePosition];
}

static u32 BattleAI_ChooseMoveOrAction_Singles(u32 battlerId, u32 aiFlags)
{
    s8 currentMoveArray[MAX_MON_MOVES];
    u32 consideredMoveArray[MAX_MON_MOVES];
    u32 i, numOfBestMoves, logicId = 0;
    
    BattleAI_InitMovesScore(battlerId);
    gAIData->thinking[battlerId].partnerMove = GetAllyChosenMove(battlerId);
    
    while (aiFlags)
    {
        if (aiFlags & 1)
            BattleAI_DoAIProcessing(battlerId, logicId, gBattlerTarget);
        
        aiFlags >>= 1;
        logicId++;
    }
    
    if (gAIData->thinking[battlerId].action & AI_ACTION_FLEE)
        return AI_CHOICE_FLEE;
    else if (gAIData->thinking[battlerId].action & AI_ACTION_WATCH)
        return AI_CHOICE_WATCH;
    
    numOfBestMoves = 1;
    currentMoveArray[0] = gAIData->thinking[battlerId].score[0];
    consideredMoveArray[0] = 0;
    
    for (i = 1; i < MAX_MON_MOVES; i++)
    {
        if (!gBattleMons[battlerId].moves[i])
            continue;
        
        if (currentMoveArray[0] < gAIData->thinking[battlerId].score[i])
        {
            currentMoveArray[0] = gAIData->thinking[battlerId].score[i];
            consideredMoveArray[0] = i;
            numOfBestMoves = 1;
        }
        
        if (currentMoveArray[0] == gAIData->thinking[battlerId].score[i])
        {
            currentMoveArray[numOfBestMoves] = gAIData->thinking[battlerId].score[i];
            consideredMoveArray[numOfBestMoves] = i;
            ++numOfBestMoves;
        }
    }
    return consideredMoveArray[RandomUniform(RNG_AI_CHOSEN_MOVE, 0, numOfBestMoves - 1)];
}

static u32 BattleAI_ChooseMoveOrAction_Doubles(u32 battlerId, u32 aiFlags)
{
    u32 actionOrMoveIndex[MAX_BATTLERS_COUNT];
    s8 bestMovePointsForTarget[MAX_BATTLERS_COUNT];
    u32 mostViableTargetsArray[MAX_BATTLERS_COUNT];
    s8 mostViableMovesScores[MAX_MON_MOVES];
    u32 mostViableMovesIndices[MAX_MON_MOVES];
    u32 mostViableMovesNo;
    u32 mostViableTargetsNo;
    s8 mostMovePoints;
    u32 i, j, logicId, flags;
    
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
            
            gAIData->thinking[battlerId].partnerMove = GetAllyChosenMove(battlerId);
            gAIData->thinking[battlerId].movesetMoveId = 0;
            
            logicId = 0;
            flags = aiFlags;
            
            gBattlerTarget = i;
            
            while (flags)
            {
                if (flags & 1)
                    BattleAI_DoAIProcessing(battlerId, logicId, i);
                
                flags >>= 1;
                logicId++;
            }
            
            if (gAIData->thinking[battlerId].action & AI_ACTION_FLEE)
                actionOrMoveIndex[i] = AI_CHOICE_FLEE;
            else if (gAIData->thinking[battlerId].action & AI_ACTION_WATCH)
                actionOrMoveIndex[i] = AI_CHOICE_WATCH;
            else
            {
                mostViableMovesScores[0] = gAIData->thinking[battlerId].score[0];
                mostViableMovesIndices[0] = 0;
                mostViableMovesNo = 1;
                
                for (j = 1; j < MAX_MON_MOVES; j++)
                {
                    u32 move = gBattleMons[battlerId].moves[j];
                    
                    if (!move || !CanTargetBattler(battlerId, i, move, GetBattlerMoveTargetType(battlerId, move), (gStatuses3[i] & STATUS3_HEAL_BLOCK)))
                        continue;
                    
                    if (mostViableMovesScores[0] < gAIData->thinking[battlerId].score[j])
                    {
                        mostViableMovesScores[0] = gAIData->thinking[battlerId].score[j];
                        mostViableMovesIndices[0] = j;
                        mostViableMovesNo = 1;
                    }
                    
                    if (mostViableMovesScores[0] == gAIData->thinking[battlerId].score[j])
                    {
                        mostViableMovesScores[mostViableMovesNo] = gAIData->thinking[battlerId].score[j];
                        mostViableMovesIndices[mostViableMovesNo] = j;
                        ++mostViableMovesNo;
                    }
                }
                actionOrMoveIndex[i] = mostViableMovesIndices[RandomUniform(RNG_AI_CHOSEN_MOVE, 0, mostViableMovesNo - 1)];
                bestMovePointsForTarget[i] = mostViableMovesScores[0];
                
                if (IsBattlerAlly(battlerId, i) && bestMovePointsForTarget[i] < DEFAULT_MOVE_SCORE)
                    bestMovePointsForTarget[i] = -1;
            }
        }
    }
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
    gBattleStruct->battlers[battlerId].aiChosenTarget = gBattlerTarget = mostViableTargetsArray[RandomUniform(RNG_AI_CHOSEN_TARGET, 0, mostViableTargetsNo - 1)];
    
    return actionOrMoveIndex[gBattlerTarget];
}

static bool32 AI_ShouldConsiderMoveForBattler(bool32 targetingPartner, u32 moveTarget)
{
    if (targetingPartner)
    {
        if (moveTarget == MOVE_TARGET_BOTH || moveTarget == MOVE_TARGET_OPPONENTS_FIELD || moveTarget == MOVE_TARGET_OPPONENTS)
            return FALSE;
    }
    return TRUE;
}

static void BattleAI_DoAIProcessing(u32 attacker, u32 logicId, u32 target)
{
    u32 moveIndex, move, moveTarget;
    struct AIScript *data = Alloc(sizeof(struct AIScript));
    
    data->attacker = gCurrentTurnActionBattlerId = gBattlerAttacker = attacker;
    data->target = gBattlerTarget = target;
    data->targetingPartner = IsBattlerAlly(attacker, target);
    data->atkAbility = GetBattlerAbility(attacker);
    data->defAbility = GetBattlerAbility(target);
    data->atkHoldEffect = GetBattlerItemHoldEffect(attacker, TRUE);
    data->defHoldEffect = GetBattlerItemHoldEffect(target, TRUE);
    
    do
    {
        moveIndex = gAIData->thinking[attacker].movesetMoveId;
        
        if (!gBattleMons[attacker].pp[moveIndex])
            move = MOVE_NONE;
        else
            move = gBattleMons[attacker].moves[moveIndex];
        
        moveTarget = GetBattlerMoveTargetType(attacker, move);
        
        if (move && gAIData->thinking[attacker].score[moveIndex] > 0 && AI_ShouldConsiderMoveForBattler(data->targetingPartner, moveTarget))
        {
            if (logicId < ARRAY_COUNT(sBattleAiFuncsTable) && sBattleAiFuncsTable[logicId] != NULL)
            {
                data->move = gCurrentMove = move;
                data->moveSlot = gCurrMovePos = moveIndex;
                data->moveTarget = moveTarget;
                data->moveSplit = GetBattleMoveSplit(move);
                data->moveType = gBattleStruct->dynamicMoveType = gAIData->thinking[attacker].moveTypes[moveIndex];

                gAIData->thinking[attacker].score[moveIndex] = sBattleAiFuncsTable[logicId](data, gAIData->thinking[attacker].score[moveIndex]);
            }
        }
        else
            gAIData->thinking[attacker].score[moveIndex] = 0;
        
        gAIData->thinking[attacker].movesetMoveId++;
        
    } while (gAIData->thinking[attacker].movesetMoveId < MAX_MON_MOVES && !(gAIData->thinking[attacker].action & AI_ACTION_DONT_ATTACK));
    
    gAIData->thinking[attacker].movesetMoveId = 0;
    Free(data);
}

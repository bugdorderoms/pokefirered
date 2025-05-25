#ifndef GUARD_BATTLE_AI_H
#define GUARD_BATTLE_AI_H

#include "constants/battle.h"

// Values for gBattleStruct->aiMoveOrAction
// values from 0 to AI_CHOICE_FLEE - 1 are move idx
#define AI_CHOICE_FLEE   (MAX_MON_MOVES)
#define AI_CHOICE_WATCH  (MAX_MON_MOVES + 1)
#define AI_CHOICE_SWITCH (MAX_MON_MOVES + 2)

// Basic ai basic flags, they are always set for all trainers
#define AI_FLAG_CHECK_BAD_MOVE        Bit(0) // Decrease score based if move will fail
#define AI_FLAG_CHECK_VIABILITY       Bit(1) // Increase score based on how much the move will be effective
#define AI_FLAG_TRY_TO_FAINT          Bit(2)
// Others ai flags
#define AI_FLAG_OMNISCIENT            Bit(3) // Causes the ai to always know the user's items, moves, etc.
#define AI_FLAG_SAFARI                Bit(4) // Safari mon thinking
#define AI_FLAG_ROAMER                Bit(5) // Roamer mon try flee
#define AI_FLAG_PREFER_STRONGEST_MOVE Bit(6)
#define AI_FLAG_PREDICT_SWITCH        Bit(7)
#define AI_FLAG_DOUBLE_BATTLE         Bit(8)
#define AI_FLAG_HP_AWARE              Bit(9)

struct AiData
{
    u32 aiFlags;
    u8 usedItemsIndices; // Indices of the used items by the trainer
    u8 knownPartyIndices; // Player party indices the ai knows
    u8 knownPlayerAbilities; // Player party indices the ai knows its abilities
    u8 knownPlayerItems; // Player party indices the ai knows its items
    u8 knownPlayerMoves[PARTY_SIZE]; // Player party's move slots indices the ai knowns
};

struct AiThinking
{
    s8 score[MAX_BATTLERS_COUNT][MAX_MON_MOVES];
    u8 movesetMoveId[MAX_BATTLERS_COUNT];
    u8 action[MAX_BATTLERS_COUNT];
    u8 moveLimitations[MAX_BATTLERS_COUNT];
    s32 simulatedDmg[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT][MAX_MON_MOVES]; // attacker, target, moveIndex
    u32 totalSpeeds[MAX_BATTLERS_COUNT];
    u32 totalAccuracy[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT][MAX_MON_MOVES]; // attacker, target, moveIndex
    u16 moves[MAX_BATTLERS_COUNT][MAX_MON_MOVES];
    u8 moveTypes[MAX_BATTLERS_COUNT][MAX_MON_MOVES];
    u8 effectiveness[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT][MAX_MON_MOVES]; // attacker, target, moveIndex
    u16 partnerMove;
    s32 higherDamage[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; // Stores the higher damage a pokemon can receive from the battler
    
    // TODO
    u8 simulatedRNG[MAX_BATTLERS_COUNT][MAX_MON_MOVES];
    u16 predictedMoves[MAX_BATTLERS_COUNT];
};

#define AI_DATA (gBattleResources->aiData)
#define AI_THINKING (gBattleResources->aiThinking)

// battle_ai_main
bool32 BattleAI_IsRunning(void);
void BattleAI_SetupAILogicData(void);
void BattleAI_ChooseAction(u32 battlerId);
void BattleAI_ComputeMovesScore(u32 battlerId);
void BattleAI_SetAILogicDataForTurn(void);

// battle_ai_switch_items
bool32 BattleAI_ShouldSwitch(u32 battlerId);
u32 GetMostSuitableMonToSwitchInto(u32 battlerId);
bool32 BattleAI_SwitchIfBadMoves(u32 battlerId, bool32 doubles);
bool32 BattleAI_ShouldUseItem(u32 battlerId);

// battle_ai_funcs
s32 BattleAIFunc_CheckBadMove(u32 attacker, u32 defender, u32 moveSlot, u32 move, s32 score);
s32 BattleAIFunc_CheckViability(u32 attacker, u32 defender, u32 moveSlot, u32 move, s32 score);
s32 BattleAIFunc_Safari(u32 attacker, u32 defender, u32 moveSlot, u32 move, s32 score);
s32 BattleAIFunc_Roamer(u32 attacker, u32 defender, u32 moveSlot, u32 move, s32 score);

#endif // GUARD_BATTLE_AI_H

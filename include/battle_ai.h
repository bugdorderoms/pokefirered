#ifndef GUARD_BATTLE_AI_H
#define GUARD_BATTLE_AI_H

#include "battle.h"

// Chosen AI actions. If none set, the AI will attack.
#define AI_ACTION_DONT_ATTACK Bit(0)
#define AI_ACTION_FLEE        Bit(1)
#define AI_ACTION_WATCH       Bit(2)

// Values for gBattleStruct->aiMoveOrAction
// values from 0 to AI_CHOICE_FLEE - 1 are move idx
#define AI_CHOICE_FLEE   (MAX_MON_MOVES)
#define AI_CHOICE_WATCH  (MAX_MON_MOVES + 1)

// AI flags
#define AI_FLAG_CHECK_BAD_MOVE  Bit(0) // Basic AI flag. Decrease score based on viability
#define AI_FLAG_CHECK_GOOD_MOVE Bit(1) // Basic AI flag. Increase score based on viability
#define AI_FLAG_SAFARI          Bit(2) // Safari mon thinking
#define AI_FLAG_ROAMER          Bit(3) // Roamer mon try flee
#define AI_FLAG_PREDICT_SWITCH  Bit(4)

struct AITarget
{
    u32 totalAccuracy[MAX_MON_MOVES];
    u32 effectiveness[MAX_MON_MOVES];
    s32 simulatedDmg[MAX_MON_MOVES];
    s32 higherDamage; // Stores the higher damage a pokemon can receive from the battler
    u16 higherDamageMove; // The move that deals the higher damage
};

struct AIThinking
{
    /*0x00*/ u8 movesetMoveId;
    /*0x01*/ u8 moveLimitations;
    /*0x02*/ u16 predictedMove;
    /*0x04*/ u32 totalSpeed;
    /*0x08*/ u16 moves[MAX_MON_MOVES];
    /*0x10*/ u16 usedMoves[MAX_MON_MOVES];
    /*0x18*/ u16 partnerMove;
    /*0x1A*/ u8 action;
    /*0x1B*/ s8 score[MAX_MON_MOVES];
    /*0x1F*/ u8 moveTypes[MAX_MON_MOVES];
    /*0x23*/ struct AITarget targets[MAX_BATTLERS_COUNT];
};

struct AIData
{
    struct
    {
        u32 aiFlags;
        u16 items[MAX_TRAINER_ITEMS];
    } logic[B_SIDE_COUNT];
    struct AIThinking thinking[MAX_BATTLERS_COUNT];
};

extern struct AIData *gAIData;

struct AIScript
{
    u8 attacker;
    u8 target;
    u16 atkAbility;
    u16 defAbility;
    u16 move;
    u8 moveSlot;
    u8 moveTarget;
    u8 moveType;
    u8 moveSplit;
    u8 atkHoldEffect;
    u8 defHoldEffect;
    bool8 targetingPartner;
};

// battle_ai_main
bool32 BattleAI_IsRunning(void);
void BattleAI_SetupAILogicData(void);
void BattleAI_ChooseAction(u32 battlerId);
void BattleAI_ComputeMovesScore(u32 battlerId);
void BattleAI_SetAILogicDataForTurn(void);

// battle_ai_switch_items
bool32 BattleAI_ShouldSwitch(u32 battlerId);
u32 GetViableMonsToSwitchInto(u32 battlerId, u8 *viableMons);
u32 GetMostSuitableMonToSwitchInto(u32 battlerId);
bool32 BattleAI_ShouldUseItem(u32 battlerId);

// battle_ai_funcs
s8 BattleAIFunc_CheckBadMove(struct AIScript *data, s8 score);
s8 BattleAIFunc_CheckGoodMove(struct AIScript *data, s8 score);
s8 BattleAIFunc_Safari(struct AIScript *data, s8 score);
s8 BattleAIFunc_Roamer(struct AIScript *data, s8 score);

#endif // GUARD_BATTLE_AI_H

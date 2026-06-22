#ifndef GUARD_BATTLE_SCRIPT_COMMANDS_H
#define GUARD_BATTLE_SCRIPT_COMMANDS_H

#include "global.h"

// For AccuracyCalcHelper
enum
{
    ACCURACY_STATE_NORMAL,
    ACCURACY_STATE_MISSES,
    ACCURACY_STATE_HITS
};

enum
{
    CANCELLER_FLAGS,
    CANCELLER_RAID_BATTLE_NULLIFICATION,
    CANCELLER_ASLEEP,
    CANCELLER_FROZEN,
    CANCELLER_TRUANT,
    CANCELLER_RECHARGE,
    CANCELLER_FLINCH,
    CANCELLER_SIGNATURE_MOVE,
    CANCELLER_GRAVITY,
    CANCELLER_DISABLED,
    CANCELLER_HEAL_BLOCKED,
    // CANCELLER_THROAT_CHOP, // TODO:
    CANCELLER_TAUNTED,
    CANCELLER_IMPRISONED,
    CANCELLER_CONFUSED,
    CANCELLER_PARALYSED,
    CANCELLER_GHOST,
    CANCELLER_IN_LOVE,
    CANCELLER_STANCE_CHANGE,
    CANCELLER_BIDE,
    CANCELLER_THAW,
    // CANCELLER_Z_MOVE, // TODO:
    // CANCELLER_GRAVITY_Z_MOVE, // TODO:
    // from here to down are all recalled when a move calls another. like Metronome calls a multi-hit move, it will need pass by CANCELLER_MULTIHIT_MOVE
    // for attack multiples times.
    // CANCELLER_SIGNATURE_MOVE_2, // TODO:
    CANCELLER_GRAVITY_2,
    CANCELLER_HEAL_BLOCKED_2,
    // CANCELLER_THROAT_CHOP_2, // TODO:
    CANCELLER_DYNAMAX_MOVES,
    CANCELLER_NATURAL_GIFT,
    CANCELLER_DANCER,
    // CANCELLER_POWDER, // TODO:
    CANCELLER_PRIMAL_WEATHER,
    // CANCELLER_PSYCHIC_TERRAIN, // TODO:
    CANCELLER_PRANKSTER,
    CANCELLER_SPORE_MOVE,
    CANCELLER_DAMP,
    CANCELLER_MULTIHIT_MOVE,
    CANCELLER_END,
};

#define CANCELLER_RECALL_CASEID CANCELLER_GRAVITY_2

bool32 CanOHKOBattler(u32 attacker, u32 target, u32 move);
bool32 CheckIfCanFireTwoTurnMoveNow(u32 battlerId, u32 move, bool32 checkChargeTurnEffects);
bool32 JumpIfMoveAffectedByProtect(const u8 *nextInstr, const u8 *jumpStr);
bool32 IsMonGettingExpSentOut(void);
void BufferMoveToLearnIntoBattleTextBuff2(void);
u32 GetCurrentLevelCapLevel(void);
u32 AccuracyCalcHelper(u32 battlerIdAtk, u32 battlerIdDef, u32 move, const u8 *nextInstr, const u8 *jumpStr);
u32 CalcMoveTotalAccuracy(u32 move, u32 attacker, u32 defender);

extern void (* const gBattleScriptingCommandsTable[])(void);

#endif // GUARD_BATTLE_SCRIPT_COMMANDS_H

#ifndef GUARD_BATTLE_SCRIPT_COMMANDS_H
#define GUARD_BATTLE_SCRIPT_COMMANDS_H

#include "global.h"

enum
{
    CANCELLER_FLAGS,
    CANCELLER_ASLEEP,
    CANCELLER_FROZEN,
    CANCELLER_TRUANT,
    CANCELLER_RECHARGE,
    CANCELLER_FLINCH,
	// CANCELLER_SIGNATURE_MOVE, // TODO:
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
	// CANCELLER_DYNAMAX_MOVES, // TODO:
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

#define WINDOW_CLEAR            0x1
#define WINDOW_x80              0x80

bool8 UproarWakeUpCheck(void);
u8 IsUproarActive(void);
u8 GetHitDamageResult(u8 battlerId, u16 move, bool8 checkSturdy);
u16 GetOHKOChance(u8 attacker, u8 target, u16 move);
bool8 KanOHKOBattler(u8 attacker, u8 target, u16 move, bool8 checkKOAcc);
bool8 JumpIfMoveAffectedByProtect(u8 addr, const u8 *jumpStr);
bool32 IsMonGettingExpSentOut(void);
void BattleCreateYesNoCursorAt(void);
void BattleDestroyYesNoCursorAt(void);
void HandleBattleWindow(u8 xStart, u8 yStart, u8 xEnd, u8 yEnd, u8 flags);
void BufferMoveToLearnIntoBattleTextBuff2(void);
u8 GetCurrentLevelCapLevel(void);
u32 CalcMoveTotalAccuracy(u16 move, u8 attacker, u8 defender);

extern void (* const gBattleScriptingCommandsTable[])(void);

#endif // GUARD_BATTLE_SCRIPT_COMMANDS_H

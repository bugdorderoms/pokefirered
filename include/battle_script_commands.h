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
    CANCELLER_DISABLED,
    CANCELLER_TAUNTED,
    CANCELLER_IMPRISONED,
    CANCELLER_CONFUSED,
    CANCELLER_PARALYSED,
    CANCELLER_GHOST,
    CANCELLER_IN_LOVE,
    CANCELLER_BIDE,
    CANCELLER_THAW,
	// from here to down are all recalled when a move calls another. like Metronome calls a multi-hit move, it will need pass by CANCELLER_MULTIHIT_MOVE
	// for attack multiples times.
	CANCELLER_PRIMAL_WEATHER,
    CANCELLER_SPORE_MOVE,
	CANCELLER_DAMP,
	CANCELLER_MULTIHIT_MOVE,
    CANCELLER_END,
};

#define CANCELLER_RECALL_CASEID CANCELLER_PRIMAL_WEATHER

#define STAT_CHANGE_WORKED      0
#define STAT_CHANGE_DIDNT_WORK  1

#define WINDOW_CLEAR            0x1
#define WINDOW_x80              0x80

bool8 UproarWakeUpCheck(void);
u8 IsUproarActive(void);
bool8 JumpIfMoveAffectedByProtect(u8 addr, const u8 *jumpStr);
u8 GetTypeModifier(u8 atkType, u8 defType);
u16 AI_TypeCalc(u16 move, u16 targetSpecies, u16 targetAbility);
u8 CalcTypeEffectivenessMultiplier(u16 move, u8 moveType, u8 attacker, u8 defender, bool8 recordAbilities, u16 *flags);
void AI_CalcDmg(u8 attacker, u8 defender, u16 move);
bool32 IsMonGettingExpSentOut(void);
void BattleCreateYesNoCursorAt(void);
void BattleDestroyYesNoCursorAt(void);
void HandleBattleWindow(u8 xStart, u8 yStart, u8 xEnd, u8 yEnd, u8 flags);
void BufferMoveToLearnIntoBattleTextBuff2(void);
u8 GetCurrentLevelCapLevel(void);

extern void (* const gBattleScriptingCommandsTable[])(void);

#endif // GUARD_BATTLE_SCRIPT_COMMANDS_H

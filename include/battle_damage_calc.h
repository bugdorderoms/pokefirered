#ifndef GUARD_BATTLE_DAMAGE_CALC_H
#define GUARD_BATTLE_DAMAGE_CALC_H

#include "global.h"

// Damage calc
s32 CalculateConfusionDamage(void);
s32 CalculateMoveDamage(u32 move, u32 moveType, u32 attacker, u32 defender, bool32 isCrit);
s32 AI_CalcMoveDamage(u32 move, u32 moveSlot, u32 attacker, u32 defender, u32 moveType, u32 effectiveness);

// Type calc
u32 GetTypeModifier(u32 atkType, u32 defType);
u32 TypeCalc(u32 move, u32 moveType, u32 attacker, u32 defender, bool32 setAbilityFlags, bool32 forAnticipation, u16 *flags);
u32 AI_TypeCalc(struct Pokemon *mon, u32 move, u32 defender);
u32 AI_GetSwitchInTypeMatchup(struct Pokemon *mon, u32 opposingBattler);

#endif // GUARD_BATTLE_DAMAGE_CALC_H

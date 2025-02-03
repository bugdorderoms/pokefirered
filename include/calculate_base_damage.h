#ifndef GUARD_CALCULATE_BASE_DAMAGE_H
#define GUARD_CALCULATE_BASE_DAMAGE_H

#include "global.h"

// Damage calc
s32 CalculateConfusionDamage(void);
s32 CalculateMoveDamage(u16 move, u8 moveType, u8 attacker, u8 defender, bool8 isCrit);
s32 AI_CalcMoveDamage(u16 move, u8 attacker, u8 defender, u8 moveType, u8 effectiveness);

// Type calc
u8 GetTypeModifier(u8 atkType, u8 defType);
u8 TypeCalc(u16 move, u8 moveType, u8 attacker, u8 defender, bool8 setAbilityFlags, u16 *flags);
u8 AI_TypeCalc(struct Pokemon *mon, u16 move, u8 defender);

#endif // GUARD_CALCULATE_BASE_DAMAGE_H

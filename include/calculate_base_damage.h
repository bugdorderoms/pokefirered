#ifndef GUARD_CALCULATE_BASE_DAMAGE_H
#define GUARD_CALCULATE_BASE_DAMAGE_H

#include "global.h"

s32 CalculateConfusionDamage(void);
s32 CalculateMoveDamage(u16 move, u8 moveType, u8 attacker, u8 defender, bool8 isCrit, bool8 randomFactor);

#endif // GUARD_CALCULATE_BASE_DAMAGE_H

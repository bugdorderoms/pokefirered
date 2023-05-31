#ifndef GUARD_CALCULATE_BASE_DAMAGE_H
#define GUARD_CALCULATE_BASE_DAMAGE_H

#include "global.h"

s32 CalculateBaseDamage(u16 move, u8 type, u8 battlerIdAtk, u8 battlerIdDef, bool8 isCrit, bool8 randomFactor);

#endif // GUARD_CALCULATE_BASE_DAMAGE_H

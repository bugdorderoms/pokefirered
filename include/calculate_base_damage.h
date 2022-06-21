#ifndef GUARD_CALCULATE_BASE_DAMAGE_H
#define GUARD_CALCULATE_BASE_DAMAGE_H

#include "global.h"

s32 CalculateBaseDamage(struct BattlePokemon *attacker, struct BattlePokemon *defender, u16 move, u16 sideStatus, bool8 isConfusionDmg, u8 battlerIdAtk, u8 battlerIdDef);

#endif // GUARD_CALCULATE_BASE_DAMAGE_H

#ifndef GUARD_CALCULATE_BASE_DAMAGE_H
#define GUARD_CALCULATE_BASE_DAMAGE_H

#include "global.h"

u16 GetModifiedMovePower(u8 battlerIdAtk, battlerIdDef, u16 move, s32 *returnSomething);
s32 CalculateBaseDamage(struct BattlePokemon *attacker, struct BattlePokemon *defender, u16 move, u16 sideStatus, bool8 isConfusionDmg, u8 battlerIdAtk, u8 battlerIdDef);

#endif // GUARD_CALCULATE_BASE_DAMAGE_H

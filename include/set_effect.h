#ifndef GUARD_SET_EFFECT_H
#define GUARD_SET_EFFECT_H

/////////////////
// MOVE EFFECT //
/////////////////

void SetMoveEffect(u8 moveEffect, bool8 affectsUser, bool8 certain);
bool8 DoMoveEffect(bool8 primary, const u8 *scriptStr, u32 flags); // Move effect
void DoZMoveEffect(void); // Z-Move effect
void DoGMaxMoveEffect(void); // G-Max move effect
bool8 CheckSecondaryEffectsBlockers(u8 attacker, u8 defender, u16 move, u8 moveEffect, bool8 affectsUser, bool8 primary, u32 flags);

/////////////////
// STAT CHANGE //
/////////////////

void SetStatChanger(u8 statId, s8 buff);
bool8 ChangeStatBuffs(u8 flags, bool8 onlyChecks);
u8 CheckStatDecreaseBlockEffects(u8 attacker, u8 target, u8 statId, u8 flags);

#endif // GUARD_SET_EFFECT_H

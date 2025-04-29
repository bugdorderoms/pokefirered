#ifndef GUARD_SET_EFFECT_H
#define GUARD_SET_EFFECT_H

/////////////////
// MOVE EFFECT //
/////////////////

void SetMoveEffect(u32 moveEffect, bool32 affectsUser, bool32 certain);
bool32 DoMoveEffect(bool32 primary, const u8 *scriptStr, u32 flags); // Move effect
void DoZMoveEffect(void); // Z-Move effect
void DoGMaxMoveEffect(void); // G-Max move effect
bool32 CheckSecondaryEffectsBlockers(u32 attacker, u32 defender, u32 move, u32 moveEffect, bool32 affectsUser, bool32 primary, u32 flags);

/////////////////
// STAT CHANGE //
/////////////////

void SetStatChanger(u32 statId, s8 buff);
bool32 ChangeStatBuffs(u32 flags, bool32 onlyChecks);
u32 CheckStatDecreaseBlockEffects(u32 attacker, u32 target, u32 statId, u32 flags);

#endif // GUARD_SET_EFFECT_H

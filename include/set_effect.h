#ifndef GUARD_SET_EFFECT_H
#define GUARD_SET_EFFECT_H

/////////////////
// MOVE EFFECT //
/////////////////

void SetMoveEffect(u8 moveEffect, bool8 affectsUser, bool8 certain);
bool8 DoMoveEffect(bool8 primary, bool8 jumpToScript, u32 flags); // Move effect
void DoZMoveEffect(void); // Z-Move effect
void DoGMaxMoveEffect(void); // G-Max move effect

/////////////////
// STAT CHANGE //
/////////////////

void SetStatChanger(u8 statId, s8 buff);
bool8 ChangeStatBuffs(u8 flags, bool8 onlyChecks);

#endif // GUARD_SET_EFFECT_H

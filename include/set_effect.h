#ifndef GUARD_SET_EFFECT_H
#define GUARD_SET_EFFECT_H

void SetMoveEffect(u8 moveEffect, bool8 affectsUser, bool8 certain);

// Move effect
void DoMoveEffect(bool8 primary);
// Z-Move effect
void DoZMoveEffect(void);
// G-Max move effect
void DoGMaxMoveEffect(void);

extern const u32 gStatusFlagsForMoveEffects[];

#endif // GUARD_SET_EFFECT_H

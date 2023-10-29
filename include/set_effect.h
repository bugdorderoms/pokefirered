#ifndef GUARD_SET_EFFECT_H
#define GUARD_SET_EFFECT_H

void SetMoveEffect(u8 moveEffect, bool8 affectsUser, bool8 certain);

// Move effect
bool8 DoMoveEffect(bool8 primary, bool8 jumpToScript, u32 flags);

// Z-Move effect
void DoZMoveEffect(void);

// G-Max move effect
void DoGMaxMoveEffect(void);

#endif // GUARD_SET_EFFECT_H

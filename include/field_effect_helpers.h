#ifndef GUARD_FIELD_EFFECT_HELPERS_H
#define GUARD_FIELD_EFFECT_HELPERS_H

#include "global.h"

void StartEscalatorWarp(u32 metatileBehavior, u32 priority);
u32 CreateWarpArrowSprite(void);
void ShowWarpArrowSprite(u32 spriteId, u32 direction, s16 x, s16 y);
void CreateReflectionEffectSprites(void);
void SetUpReflection(struct ObjectEvent * objectEvent, struct Sprite * sprite, bool32 stillReflection);
void CreateTeleportFieldEffectTask(void);
void StartEscapeRopeFieldEffect(void);

#endif //GUARD_FIELD_EFFECT_HELPERS_H

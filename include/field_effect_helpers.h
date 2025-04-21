//
// Created by scott on 9/7/2017.
//

#ifndef GUARD_FIELD_EFFECT_HELPERS_H
#define GUARD_FIELD_EFFECT_HELPERS_H

#include "global.h"

// Exported type declarations

// Exported RAM declarations

// Exported ROM declarations

u32 CreateWarpArrowSprite(void);
void SetSurfBlob_BobState(u32, u8);
void SetSurfBlob_DontSyncAnim(u32, u8);
void StartAshFieldEffect(s16, s16, u32, s16);
void StartRevealDisguise(struct ObjectEvent *);
bool32 UpdateRevealDisguise(struct ObjectEvent *);
void SetUpReflection(struct ObjectEvent*, struct Sprite*, bool32);
u32 StartFieldEffectForObjectEvent(u32, struct ObjectEvent*);
u32 FindTallGrassFieldEffectSpriteId(u32 localId, u32 mapNum, u32 mapGroup, s16 x, s16 y);
void ShowWarpArrowSprite(u32 spriteId, u32 direction, s16 x, s16 y);
void SetSurfBlob_PlayerOffset(u32 spriteId, u8 hasOffset, s16 offset);
void UpdateJumpImpactEffect(struct Sprite * sprite);
void UpdateShadowFieldEffect(struct Sprite * sprite);
void UpdateBubblesFieldEffect(struct Sprite * sprite);
void UpdateSparkleFieldEffect(struct Sprite * sprite);
void UpdateTallGrassFieldEffect(struct Sprite * sprite);
void WaitFieldEffectSpriteAnim(struct Sprite * sprite);
void UpdateAshFieldEffect(struct Sprite * sprite);
void UpdateSurfBlobFieldEffect(struct Sprite * sprite);
void UpdateFootprintsTireTracksFieldEffect(struct Sprite * sprite);
void UpdateSplashFieldEffect(struct Sprite * sprite);
void UpdateShortGrassFieldEffect(struct Sprite * sprite);
void UpdateLongGrassFieldEffect(struct Sprite * sprite);
void UpdateSandPileFieldEffect(struct Sprite * sprite);
void UpdateDisguiseFieldEffect(struct Sprite * sprite);
void UpdateHotSpringsWaterFieldEffect(struct Sprite * sprite);
void LoadFieldEffectPalette(u32 fieldEffect);
void FieldEffectFreePaletteIfUnused(u32 paletteNum);
void SetUpShadow(struct ObjectEvent *objectEvent);

#endif //GUARD_FIELD_EFFECT_HELPERS_H

#ifndef GUARD_FIELD_EFFECTS_H
#define GUARD_FIELD_EFFECTS_H

#include "global.h"
#include "constants/field_effects.h"

#define SHOW_MON_CRY_NO_DUCKING Bit(31)

extern u32 gFieldEffectArguments[8];
extern void (*gPostMenuFieldCallback)(void);
extern bool32 (*gFieldCallback2)(void);

u32 FieldEffectStart(u32);
bool32 FieldEffectActiveListContains(u32 id);
void CreateTeleportFieldEffectTask(void);
void FieldEffectActiveListRemove(u32 id);
void StartEscapeRopeFieldEffect(void);
void FieldEffectStop(struct Sprite *sprite, u32 id);
u32 CreateTrainerSprite(u32 trainerSpriteID, s16 x, s16 y, u32 subpriority);
void FieldCB_FallWarpExit(void);
void StartEscalatorWarp(u32 metatileBehavior, u32 priority);
void StartLavaridgeGymB1FWarp(u32 a0);
void StartLavaridgeGym1FWarp(u32 a0);
void ApplyGlobalFieldPaletteTint(u32 paletteIdx);
void FreeResourcesAndDestroySprite(struct Sprite * sprite, u32 spriteId);
void ReturnToFieldFromFlyMapSelect(void);
void MultiplyInvertedPaletteRGBComponents(u16 i, u8 r, u8 g, u8 b);
void SpriteCB_PopOutOfAsh(struct Sprite * sprite);
void SpriteCB_AshLaunch(struct Sprite * sprite);
void FieldEffectActiveListClear(void);
void FieldEff_CaveDust(void);

#endif //GUARD_FIELD_EFFECTS_H

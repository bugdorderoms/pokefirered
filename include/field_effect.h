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
void FieldEffectActiveListRemove(u32 id);
void FieldEffectActiveListClear(void);
void FieldEffectStop(struct Sprite *sprite, u32 id);
void FieldEffectFreePaletteIfUnused(u32 paletteNum);
void FreeResourcesAndDestroySprite(struct Sprite * sprite, u32 spriteId);
void ApplyGlobalFieldPaletteTint(u32 paletteIdx);
void LoadFieldEffectPalette(const struct SpriteTemplate *spriteTemplate);
void FieldEffect_LoadFadedPal(const struct SpritePalette *spritePalette);
void SetUpShadow(struct ObjectEvent *objectEvent);
u32 FindTallGrassFieldEffectSpriteId(u32 localId, u32 mapNum, u32 mapGroup, s16 x, s16 y);
void SetSurfBlob_BobState(u32, u8);
void SetSurfBlob_DontSyncAnim(u32, u8);
void SetSurfBlob_PlayerOffset(u32 spriteId, u8 hasOffset, s16 offset);
void StartRevealDisguise(struct ObjectEvent *);
bool32 UpdateRevealDisguise(struct ObjectEvent *);
void MultiplyInvertedPaletteRGBComponents(u16 i, u8 r, u8 g, u8 b);
void ReturnToFieldFromFlyMapSelect(void);
void StartLavaridgeGymB1FWarp(u32 priority);
void StartLavaridgeGym1FWarp(u32 priority);
void FieldCB_FallWarpExit(void);
void StartAshFieldEffect(s16, s16, u32, s16);

extern u16 gReflectionPaletteBuffer[16];
extern const struct SpriteTemplate gSpriteTemplate_TallGrass;
extern const u16 gSlidingPokeballBigPokeballPalette[];

#endif //GUARD_FIELD_EFFECTS_H

#ifndef GUARD_PALETTE_H
#define GUARD_PALETTE_H

#include "global.h"

#define PLTT_BUFFER_SIZE 0x200
#define PLTT_DECOMP_BUFFER_SIZE (PLTT_BUFFER_SIZE * 2)

#define PALETTE_FADE_STATUS_DONE    0x0
#define PALETTE_FADE_STATUS_DELAY   0x2
#define PALETTE_FADE_STATUS_ACTIVE  0x1
#define PALETTE_FADE_STATUS_LOADING 0xFF

#define PALETTES_BG      0x0000FFFF
#define PALETTES_OBJECTS 0xFFFF0000
#define PALETTES_ALL     (PALETTES_BG | PALETTES_OBJECTS)

enum
{
    FAST_FADE_IN_FROM_WHITE,
    FAST_FADE_OUT_TO_WHITE,
    FAST_FADE_IN_FROM_BLACK,
    FAST_FADE_OUT_TO_BLACK,
};

struct PaletteFadeControl
{
    u32 multipurpose1;
    u8 delayCounter:6;
    u16 y:5; // blend coefficient
    u16 targetY:5; // target blend coefficient
    u16 blendColor:15;
    u16 active:1;
    u16 multipurpose2:6;
    u16 yDec:1; // whether blend coefficient is decreasing
    u16 bufferTransferDisabled:1;
    u16 mode:2;
    u16 shouldResetBlendRegisters:1;
    u16 hardwareFadeFinishing:1;
    u16 softwareFadeFinishingCounter:5;
    u16 softwareFadeFinishing:1;
    u16 objPaletteToggle:1;
    u8 deltaY:4; // rate of change of blend coefficient
    u32 unused;
};

extern struct PaletteFadeControl gPaletteFade;
extern u16 gPlttBufferUnfaded[PLTT_BUFFER_SIZE];
extern u16 gPlttBufferFaded[PLTT_BUFFER_SIZE];

void BlendPalette(u32, u32, u32, u32);
void BlendPalettesAt(u16 * palbuff, u32 blend_pal, u32 coefficient, s32 size);
void LoadCompressedPalette(const u32 *src, u32 offset, u32 size);
void LoadPalette(const void *src, u32 offset, u32 size);
void FillPalette(u32 value, u32 offset, u32 size);
void TransferPlttBuffer(void);
u32 UpdatePaletteFade(void);
void ResetPaletteFade(void);
bool32 BeginNormalPaletteFade(u32 selectedPalettes, s8 delay, u32 startY, u32 targetY, u32 blendColor);
void InvertPlttBuffer(u32 selectedPalettes);
void BeginFastPaletteFade(u32 submode);
void BeginHardwarePaletteFade(u8 blendCnt, u32 delay, u32 y, u32 targetY, bool32 shouldResetBlendRegisters);
void BlendPalettes(u32 selectedPalettes, u32 coeff, u32 color);
void BlendPalettesUnfaded(u32 selectedPalettes, u32 coeff, u32 color);
void TintPalette_GrayScale(u16 *palette, u32 count);
void TintPalette_GrayScale2(u16 *palette, u32 count);
void TintPalette_SepiaTone(u16 *palette, u32 count);
void TintPalette_CustomTone(u16 *palette, u32 count, u16 rTone, u16 gTone, u16 bTone);
void BlendPalettesGradually(u32 selectedPalettes, s8 delay, u32 coeff, u32 coeffTarget, u32 color, u32 priority, u32 id);
bool32 IsBlendPalettesGraduallyTaskActive(u32 id);
void DestroyBlendPalettesGraduallyTask(void);

#endif // GUARD_PALETTE_H

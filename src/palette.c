#include "global.h"
#include "gflib.h"
#include "util.h"
#include "math_util.h"
#include "decompress.h"
#include "task.h"
#include "dns.h"

enum
{
    NORMAL_FADE,
    FAST_FADE,
    HARDWARE_FADE,
};

#define gPaletteFade_selectedPalettes (gPaletteFade.multipurpose1) // normal and fast fade
#define gPaletteFade_blendCnt         (gPaletteFade.multipurpose1) // hardware fade
#define gPaletteFade_delay            (gPaletteFade.multipurpose2) // normal and hardware fade
#define gPaletteFade_submode          (gPaletteFade.multipurpose2) // fast fade

static u32 UpdateNormalPaletteFade(void);
static u32 UpdateFastPaletteFade(void);
static u32 UpdateHardwarePaletteFade(void);
static void UpdateBlendRegisters(void);
static bool32 IsSoftwarePaletteFadeFinishing(void);
static void Task_BlendPalettesGradually(u32 taskId);

EWRAM_DATA struct PaletteFadeControl gPaletteFade = {0};
ALIGNED(4) EWRAM_DATA u16 gPlttBufferUnfaded[PLTT_BUFFER_SIZE] = {0};
ALIGNED(4) EWRAM_DATA u16 gPlttBufferFaded[PLTT_BUFFER_SIZE] = {0};
static EWRAM_DATA u32 sPlttBufferTransferPending = 0;
EWRAM_DATA u8 gPaletteDecompressionBuffer[PLTT_DECOMP_BUFFER_SIZE] = {0};

static const u8 sRoundedDownGrayscaleMap[] =
{
     0,  0,  0,  0,  0,
     5,  5,  5,  5,  5,
    11, 11, 11, 11, 11,
    16, 16, 16, 16, 16,
    21, 21, 21, 21, 21,
    27, 27, 27, 27, 27,
    31, 31
};

void LoadCompressedPalette(const u32 *src, u32 offset, u32 size)
{
    LZDecompressWram(src, gPaletteDecompressionBuffer);
    CpuCopy16(gPaletteDecompressionBuffer, gPlttBufferUnfaded + offset, size);
    CpuCopy16(gPaletteDecompressionBuffer, gPlttBufferFaded + offset, size);
}

void LoadPalette(const void *src, u32 offset, u32 size)
{
    CpuCopy16(src, gPlttBufferUnfaded + offset, size);
    CpuCopy16(src, gPlttBufferFaded + offset, size);
}

void FillPalette(u32 value, u32 offset, u32 size)
{
    CpuFill16(value, gPlttBufferUnfaded + offset, size);
    CpuFill16(value, gPlttBufferFaded + offset, size);
}

void TransferPlttBuffer(void)
{    
    if (!gPaletteFade.bufferTransferDisabled)
    {
        void *src = gPlttBufferFaded;
        void *dest = (void *)PLTT;
        DNSTransferPlttBuffer(src, dest);
        
        sPlttBufferTransferPending = FALSE;
        
        if (gPaletteFade.mode == HARDWARE_FADE && gPaletteFade.active)
            UpdateBlendRegisters();
    }
}

u32 UpdatePaletteFade(void)
{
    u32 result;

    if (sPlttBufferTransferPending)
        return PALETTE_FADE_STATUS_LOADING;
    else if (gPaletteFade.mode == NORMAL_FADE)
        result = UpdateNormalPaletteFade();
    else if (gPaletteFade.mode == FAST_FADE)
        result = UpdateFastPaletteFade();
    else
        result = UpdateHardwarePaletteFade();
    
    sPlttBufferTransferPending = gPaletteFade.multipurpose1;
    
    return result;
}

void ResetPaletteFade(void)
{
    gPaletteFade.multipurpose1 = 0;
    gPaletteFade.multipurpose2 = 0;
    gPaletteFade.delayCounter = 0;
    gPaletteFade.y = 0;
    gPaletteFade.targetY = 0;
    gPaletteFade.blendColor = 0;
    gPaletteFade.active = FALSE;
    gPaletteFade.yDec = FALSE;
    gPaletteFade.bufferTransferDisabled = FALSE;
    gPaletteFade.shouldResetBlendRegisters = FALSE;
    gPaletteFade.hardwareFadeFinishing = FALSE;
    gPaletteFade.softwareFadeFinishing = FALSE;
    gPaletteFade.softwareFadeFinishingCounter = 0;
    gPaletteFade.objPaletteToggle = FALSE;
    gPaletteFade.deltaY = 2;
}

bool32 BeginNormalPaletteFade(u32 selectedPalettes, s8 delay, u32 startY, u32 targetY, u32 blendColor)
{
    u32 temp;

    if (!gPaletteFade.active)
    {
        gPaletteFade.deltaY = 2;
        if (delay < 0)
        {
            gPaletteFade.deltaY += (delay * -1);
            delay = 0;
        }
        gPaletteFade_selectedPalettes = selectedPalettes;
        gPaletteFade.delayCounter = delay;
        gPaletteFade_delay = delay;
        gPaletteFade.y = startY;
        gPaletteFade.targetY = targetY;
        gPaletteFade.blendColor = blendColor;
        gPaletteFade.active = TRUE;
        gPaletteFade.mode = NORMAL_FADE;
        gPaletteFade.yDec = startY < targetY ? FALSE : TRUE;

        UpdatePaletteFade();
        
        temp = gPaletteFade.bufferTransferDisabled;
        gPaletteFade.bufferTransferDisabled = FALSE;
        
        TransferPlttBuffer();
        
        gPaletteFade.bufferTransferDisabled = temp;
        
        return TRUE;
    }
    return FALSE;
}

void BlendPalette(u32 palOffset, u32 numEntries, u32 coeff, u32 blendColor)
{
    u32 i;
    u32 rBlend = GET_R(blendColor);
    u32 gBlend = GET_G(blendColor);
    u32 bBlend = GET_B(blendColor);
    
    for (i = 0; i < numEntries; i++)
    {
        u32 index = i + palOffset;
        u32 color = gPlttBufferUnfaded[index];

        s8 r = GET_R(color);
        s8 g = GET_G(color);
        s8 b = GET_B(color);
        
        gPlttBufferFaded[index] = RGBA((r + (((rBlend - r) * coeff) >> 4)),
                                       (g + (((gBlend - g) * coeff) >> 4)),
                                       (b + (((bBlend - b) * coeff) >> 4)),
                                       IS_ALPHA(color));
    }
}

static u32 UpdateNormalPaletteFade(void)
{
    u16 paletteOffset, selectedPalettes;

    if (!gPaletteFade.active)
        return PALETTE_FADE_STATUS_DONE;
    else if (IsSoftwarePaletteFadeFinishing())
        return gPaletteFade.active ? PALETTE_FADE_STATUS_ACTIVE : PALETTE_FADE_STATUS_DONE;
    else
    {
        if (!gPaletteFade.objPaletteToggle)
        {
            if (gPaletteFade.delayCounter < gPaletteFade_delay)
            {
                ++gPaletteFade.delayCounter;
                return 2;
            }
            gPaletteFade.delayCounter = 0;
        }

        if (!gPaletteFade.objPaletteToggle)
        {
            selectedPalettes = gPaletteFade_selectedPalettes;
            paletteOffset = 0;
        }
        else
        {
            selectedPalettes = gPaletteFade_selectedPalettes >> 16;
            paletteOffset = 256;
        }
        
        while (selectedPalettes)
        {
            if (selectedPalettes & 1)
                BlendPalette(paletteOffset, 16, gPaletteFade.y, gPaletteFade.blendColor);
            
            selectedPalettes >>= 1;
            paletteOffset += 16;
        }
        gPaletteFade.objPaletteToggle ^= TRUE;
        
        if (!gPaletteFade.objPaletteToggle)
        {
            if (gPaletteFade.y == gPaletteFade.targetY)
            {
                gPaletteFade_selectedPalettes = 0;
                gPaletteFade.softwareFadeFinishing = TRUE;
            }
            else
            {
                s8 val;

                if (!gPaletteFade.yDec)
                {
                    val = gPaletteFade.y;
                    val += gPaletteFade.deltaY;
                    if (val > gPaletteFade.targetY)
                        val = gPaletteFade.targetY;
                    gPaletteFade.y = val;
                }
                else
                {
                    val = gPaletteFade.y;
                    val -= gPaletteFade.deltaY;
                    if (val < gPaletteFade.targetY)
                        val = gPaletteFade.targetY;
                    gPaletteFade.y = val;
                }
            }
        }
        return PALETTE_FADE_STATUS_ACTIVE;
    }
}

void InvertPlttBuffer(u32 selectedPalettes)
{
    u32 i;
    u16 paletteOffset = 0;

    while (selectedPalettes)
    {
        if (selectedPalettes & 1)
        {
            for (i = 0; i < 16; ++i)
                gPlttBufferFaded[paletteOffset + i] = ~(gPlttBufferFaded[paletteOffset + i]);
        }
        selectedPalettes >>= 1;
        paletteOffset += 16;
    }
}

void BeginFastPaletteFade(u32 submode)
{
    gPaletteFade.deltaY = 2;
    gPaletteFade.y = 31;
    gPaletteFade_submode = submode & 0x3F;
    gPaletteFade.active = TRUE;
    gPaletteFade.mode = FAST_FADE;
    
    if (submode == FAST_FADE_IN_FROM_BLACK)
        CpuFill16(RGB_BLACK, gPlttBufferFaded, PLTT_SIZE);
    if (submode == FAST_FADE_IN_FROM_WHITE)
        CpuFill16(RGB_WHITE, gPlttBufferFaded, PLTT_SIZE);
    
    UpdatePaletteFade();
}

static u32 UpdateFastPaletteFade(void)
{
    u32 i, paletteOffsetStart, paletteOffsetEnd;
    s8 r0, g0, b0, r, g, b;

    if (!gPaletteFade.active)
        return PALETTE_FADE_STATUS_DONE;
    else if (IsSoftwarePaletteFadeFinishing())
        return gPaletteFade.active ? PALETTE_FADE_STATUS_ACTIVE : PALETTE_FADE_STATUS_DONE;
    
    if (gPaletteFade.objPaletteToggle)
    {
        paletteOffsetStart = 256;
        paletteOffsetEnd = 512;
    }
    else
    {
        paletteOffsetStart = 0;
        paletteOffsetEnd = 256;
    }
    
    switch (gPaletteFade_submode)
    {
    case FAST_FADE_IN_FROM_WHITE:
        for (i = paletteOffsetStart; i < paletteOffsetEnd; ++i)
        {
            r0 = GET_R(gPlttBufferUnfaded[i]);
            g0 = GET_G(gPlttBufferUnfaded[i]);
            b0 = GET_B(gPlttBufferUnfaded[i]);
            
            r = GET_R(gPlttBufferFaded[i]) - 2;
            g = GET_G(gPlttBufferFaded[i]) - 2;
            b = GET_B(gPlttBufferFaded[i]) - 2;
            if (r < r0)
                r = r0;
            if (g < g0)
                g = g0;
            if (b < b0)
                b = b0;
            gPlttBufferFaded[i] = RGBA(r, g, b, IS_ALPHA(gPlttBufferUnfaded[i]));
        }
        break;
    case FAST_FADE_OUT_TO_WHITE:
        for (i = paletteOffsetStart; i < paletteOffsetEnd; ++i)
        {
            r = GET_R(gPlttBufferFaded[i]) + 2;
            g = GET_G(gPlttBufferFaded[i]) + 2;
            b = GET_B(gPlttBufferFaded[i]) + 2;
            if (r > 31)
                r = 31;
            if (g > 31)
                g = 31;
            if (b > 31)
                b = 31;
            gPlttBufferFaded[i] = RGBA(r, g, b, IS_ALPHA(gPlttBufferFaded[i]));
        }
        break;
    case FAST_FADE_IN_FROM_BLACK:
        for (i = paletteOffsetStart; i < paletteOffsetEnd; ++i)
        {
            r0 = GET_R(gPlttBufferUnfaded[i]);
            g0 = GET_G(gPlttBufferUnfaded[i]);
            b0 = GET_B(gPlttBufferUnfaded[i]);
            
            r = GET_R(gPlttBufferFaded[i]) + 2;
            g = GET_G(gPlttBufferFaded[i]) + 2;
            b = GET_B(gPlttBufferFaded[i]) + 2;
            if (r > r0)
                r = r0;
            if (g > g0)
                g = g0;
            if (b > b0)
                b = b0;
            gPlttBufferFaded[i] = RGBA(r, g, b, IS_ALPHA(gPlttBufferUnfaded[i]));
        }
        break;
    case FAST_FADE_OUT_TO_BLACK:
        for (i = paletteOffsetStart; i < paletteOffsetEnd; ++i)
        {
            r = GET_R(gPlttBufferFaded[i]) - 2;
            g = GET_G(gPlttBufferFaded[i]) - 2;
            b = GET_B(gPlttBufferFaded[i]) - 2;
            if (r < 0)
                r = 0;
            if (g < 0)
                g = 0;
            if (b < 0)
                b = 0;
            gPlttBufferFaded[i] = RGBA(r, g, b, IS_ALPHA(gPlttBufferFaded[i]));
        }
    }
    gPaletteFade.objPaletteToggle ^= TRUE;
    
    if (gPaletteFade.objPaletteToggle)
        return PALETTE_FADE_STATUS_ACTIVE;
    
    if (gPaletteFade.y - gPaletteFade.deltaY < 0)
        gPaletteFade.y = 0;
    else
        gPaletteFade.y -= gPaletteFade.deltaY;
    
    if (gPaletteFade.y == 0)
    {
        switch (gPaletteFade_submode)
        {
        case FAST_FADE_IN_FROM_WHITE:
        case FAST_FADE_IN_FROM_BLACK:
            CpuCopy32(gPlttBufferUnfaded, gPlttBufferFaded, PLTT_SIZE);
            break;
        case FAST_FADE_OUT_TO_WHITE:
            CpuFill32(RGB_WHITE, gPlttBufferFaded, PLTT_SIZE);
            break;
        case FAST_FADE_OUT_TO_BLACK:
            CpuFill32(RGB_BLACK, gPlttBufferFaded, PLTT_SIZE);
            break;
        }
        gPaletteFade.mode = NORMAL_FADE;
        gPaletteFade.softwareFadeFinishing = TRUE;
    }
    return PALETTE_FADE_STATUS_ACTIVE;
}

void BeginHardwarePaletteFade(u8 blendCnt, u32 delay, u32 y, u32 targetY, bool32 shouldResetBlendRegisters)
{
    gPaletteFade_blendCnt = blendCnt;
    gPaletteFade.delayCounter = delay;
    gPaletteFade_delay = delay;
    gPaletteFade.y = y;
    gPaletteFade.targetY = targetY;
    gPaletteFade.active = TRUE;
    gPaletteFade.mode = HARDWARE_FADE;
    gPaletteFade.shouldResetBlendRegisters = (shouldResetBlendRegisters & 1);
    gPaletteFade.hardwareFadeFinishing = FALSE;
    gPaletteFade.yDec = y < targetY ? FALSE : TRUE;
}

static u32 UpdateHardwarePaletteFade(void)
{
    if (!gPaletteFade.active)
        return PALETTE_FADE_STATUS_DONE;
    else if (gPaletteFade.delayCounter < gPaletteFade_delay)
    {
        ++gPaletteFade.delayCounter;
        return PALETTE_FADE_STATUS_DELAY;
    }
    gPaletteFade.delayCounter = 0;
    
    if (!gPaletteFade.yDec)
    {
        if (++gPaletteFade.y > gPaletteFade.targetY)
        {
            ++gPaletteFade.hardwareFadeFinishing;
            --gPaletteFade.y;
        }
    }
    else
    {
        if (gPaletteFade.y-- - 1 < gPaletteFade.targetY)
        {
            ++gPaletteFade.hardwareFadeFinishing;
            ++gPaletteFade.y;
        }
    }

    if (gPaletteFade.hardwareFadeFinishing)
    {
        if (gPaletteFade.shouldResetBlendRegisters)
        {
            gPaletteFade_blendCnt = 0;
            gPaletteFade.y = 0;
        }
        gPaletteFade.shouldResetBlendRegisters = FALSE;
    }
    return PALETTE_FADE_STATUS_ACTIVE;
}

static void UpdateBlendRegisters(void)
{
    SetGpuReg(REG_OFFSET_BLDCNT, (u16)gPaletteFade_blendCnt);
    SetGpuReg(REG_OFFSET_BLDY, gPaletteFade.y);
    
    if (gPaletteFade.hardwareFadeFinishing)
    {
        gPaletteFade.hardwareFadeFinishing = FALSE;
        gPaletteFade.mode = 0;
        gPaletteFade_blendCnt = 0;
        gPaletteFade.y = 0;
        gPaletteFade.active = FALSE;
    }
}

static bool32 IsSoftwarePaletteFadeFinishing(void)
{
    if (gPaletteFade.softwareFadeFinishing)
    {
        if (gPaletteFade.softwareFadeFinishingCounter == 4)
        {
            gPaletteFade.active = FALSE;
            gPaletteFade.softwareFadeFinishing = FALSE;
            gPaletteFade.softwareFadeFinishingCounter = 0;
        }
        else
            ++gPaletteFade.softwareFadeFinishingCounter;

        return TRUE;
    }
    return FALSE;
}

void BlendPalettes(u32 selectedPalettes, u32 coeff, u32 color)
{
    u32 paletteOffset;

    for (paletteOffset = 0; selectedPalettes; paletteOffset += 16)
    {
        if (selectedPalettes & 1)
            BlendPalette(paletteOffset, 16, coeff, color);
        
        selectedPalettes >>= 1;
    }
}

void BlendPalettesUnfaded(u32 selectedPalettes, u32 coeff, u32 color)
{
    // This copy is done via DMA in both RUBY and EMERALD
    CpuFastCopy(gPlttBufferUnfaded, gPlttBufferFaded, 0x400);
    BlendPalettes(selectedPalettes, coeff, color);
}

void TintPalette_GrayScale(u16 *palette, u32 count)
{
    s32 r, g, b;
    u32 i, gray;

    for (i = 0; i < count; ++i)
    {
        r = GET_R(*palette);
        g = GET_G(*palette);
        b = GET_B(*palette);
        
        gray = (r * Q_8_8(0.3) + g * Q_8_8(0.59) + b * Q_8_8(0.1133)) >> 8;
        
        *palette++ = RGBA(gray, gray, gray, IS_ALPHA(*palette));
    }
}

void TintPalette_GrayScale2(u16 *palette, u32 count)
{
    s32 r, g, b;
    u32 i, gray;

    for (i = 0; i < count; ++i)
    {
        r = GET_R(*palette);
        g = GET_G(*palette);
        b = GET_B(*palette);
        
        gray = (r * Q_8_8(0.3) + g * Q_8_8(0.59) + b * Q_8_8(0.1133)) >> 8;
        if (gray > 0x1F)
            gray = 0x1F;
        
        gray = sRoundedDownGrayscaleMap[gray];
        
        *palette++ = RGBA(gray, gray, gray, IS_ALPHA(*palette));
    }
}

void TintPalette_SepiaTone(u16 *palette, u32 count)
{
    s32 r, g, b;
    u32 i, gray;

    for (i = 0; i < count; ++i)
    {
        r = GET_R(*palette);
        g = GET_G(*palette);
        b = GET_B(*palette);
        
        gray = (r * Q_8_8(0.3) + g * Q_8_8(0.59) + b * Q_8_8(0.1133)) >> 8;
        
        r = (u16)((Q_8_8(1.2) * gray)) >> 8;
        g = (u16)((Q_8_8(1.0) * gray)) >> 8;
        b = (u16)((Q_8_8(0.94) * gray)) >> 8;
        
        if (r > 31)
            r = 31;
        
        *palette++ = RGBA(r, g, b, IS_ALPHA(*palette));
    }
}

void TintPalette_CustomTone(u16 *palette, u32 count, u16 rTone, u16 gTone, u16 bTone)
{
    s32 r, g, b;
    u32 i, gray;

    for (i = 0; i < count; ++i)
    {
        r = GET_R(*palette);
        g = GET_G(*palette);
        b = GET_B(*palette);
        
        gray = (r * Q_8_8(0.3) + g * Q_8_8(0.59) + b * Q_8_8(0.1133)) >> 8;
        
        r = (u16)((rTone * gray)) >> 8;
        g = (u16)((gTone * gray)) >> 8;
        b = (u16)((bTone * gray)) >> 8;
        
        if (r > 31)
            r = 31;
        if (g > 31)
            g = 31;
        if (b > 31)
            b = 31;
        
        *palette++ = RGBA(r, g, b, IS_ALPHA(*palette));
    }
}

void BlendPalettesGradually(u32 selectedPalettes, s8 delay, u32 coeff, u32 coeffTarget, u32 color, u32 priority, u32 id)
{
    u32 taskId = CreateTask(Task_BlendPalettesGradually, priority);

    gTasks[taskId].data[0] = coeff;
    gTasks[taskId].data[1] = coeffTarget;
    gTasks[taskId].data[7] = color;
    gTasks[taskId].data[8] = id;
    
    if (delay >= 0)
    {
        gTasks[taskId].data[3] = delay;
        gTasks[taskId].data[2] = 1;
    }
    else
    {
        gTasks[taskId].data[3] = 0;
        gTasks[taskId].data[2] = -delay + 1;
    }
    if (coeffTarget < coeff)
        gTasks[taskId].data[2] *= -1;
    
    SetWordTaskArg(taskId, 5, selectedPalettes);
    
    gTasks[taskId].func(taskId);
}

bool32 IsBlendPalettesGraduallyTaskActive(u32 id)
{
    s32 i;

    for (i = 0; i < NUM_TASKS; ++i)
    {
        if (gTasks[i].isActive && gTasks[i].func == Task_BlendPalettesGradually && gTasks[i].data[8] == id)
            return TRUE;
    }
    return FALSE;
}

void DestroyBlendPalettesGraduallyTask(void)
{
    u32 taskId;

    while (TRUE)
    {
        taskId = FindTaskIdByFunc(Task_BlendPalettesGradually);
        
        if (taskId == TAIL_SENTINEL)
            break;
        
        DestroyTask(taskId);
    }
}

static void Task_BlendPalettesGradually(u32 taskId)
{
    s16 temp, *data = gTasks[taskId].data;
    u32 wordVar = GetWordTaskArg(taskId, 5);

    if (++data[4] > data[3])
    {
        data[4] = 0;
        BlendPalettes(wordVar, data[0], data[7]);
        temp = data[1];
        
        if (data[0] == temp)
            DestroyTask(taskId);
        else
        {
            data[0] += data[2];
            
            if (data[2] >= 0)
            {
                if (data[0] < temp)
                    return;
            }
            else if (data[0] > temp)
                return;

            data[0] = temp;
        }
    }
}

#include "global.h"
#include "sprite.h"
#include "task.h"
#include "overworld.h"
#include "malloc.h"
#include "data.h"
#include "battle.h"
#include "palette.h"
#include "trig.h"
#include "random.h"
#include "sound.h"
#include "decompress.h"
#include "gpu_regs.h"
#include "battle_transition.h"
#include "field_effect.h"
#include "field_weather.h"
#include "field_camera.h"
#include "trainer_pokemon_sprites.h"
#include "scanline_effect.h"
#include "event_object_movement.h"
#include "constants/songs.h"

typedef bool32 (*TransitionStateFunc)(struct Task *task);
typedef bool32 (*TransitionSpriteCallback)(struct Sprite *sprite);

#define B_TRANS_DMA_FLAGS (((DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_16BIT | DMA_SRC_INC | DMA_DEST_FIXED) << 16) | 1)

struct TransitionData
{
    vu8 vblankDma;
    u16 winIn;
    u16 winOut;
    u16 win0H;
    u16 win0V;
    u16 win1V;
    u16 bldCnt;
    u16 bldAlpha;
    u16 bldY;
    s16 bg123HOfs;
    s16 bg123VOfs;
    s16 bg0HOfsOpponent;
    s16 bg0HOfsPlayer;
    s16 bg0VOfs;
    s16 counter;
    s16 data[11]; // for multiple purposes
};

struct RectangularSpiralLine
{
    u8 state;
    s16 position;
    u8 moveIndex;
    s16 reboundPosition;
    bool8 outward;
};

static EWRAM_DATA struct TransitionData *sTransitionStructPtr = NULL;
static EWRAM_DATA struct RectangularSpiralLine sRectangularSpiralLines[4] = {0};

static void BT_LaunchTask(u32 transitionId);
static void BT_TaskMain(u32 taskId);
static bool32 BT_Phase1Blink(struct Task *task);
static bool32 BT_WaitForPhase1(struct Task *task);
static bool32 BT_Phase2LaunchAnimTask(struct Task *task);
static bool32 BT_WaitForPhase2(struct Task *task);
static void BT_Phase1Task(u32 taskId);
static void BT_CreatePhase1SubTask(s16 fadeOutDelay, s16 fadeInDelay, s16 blinkTimes, s16 fadeOutSpeed, s16 fadeInSpeed);
static void BT_Phase1SubTask(u32 taskId);
static bool32 BT_Phase1_FadeOut(struct Task *task);
static bool32 BT_Phase1_FadeIn(struct Task *task);
static void BT_InitCtrlBlk(void);
static void BT_VBSyncOamAndPltt(void);
static void BT_BlendPalettesToBlack(void);
static void BT_DestroyPhase2AnimTask(struct Task *task);
static bool32 BT_EndScanlinePhase2Transition(struct Task *task);
static void HBCB_BT_Phase2UpdateBgsHorizontalOffsets(void);
static void HBCB_BT_Phase2UpdateBgsVerticalOffsets(void);
static void BT_GetBg0TilesetBase(u16 **tilesetPtr);
static void BT_GetBg0TilemapBase(u16 **tilemapPtr);
static void BT_GetBg0TilemapAndTilesetBase(u16 **tilemapPtr, u16 **tilesetPtr);
static void BT_LoadWaveIntoBuffer(s16 *buffer, s16 offset, s16 theta, s16 frequency, s16 amplitude, s16 bufSize);
static void BT_GenerateCircle(s16 *buffer, s16 x, s16 y, s16 radius);
static void BT_DiagonalSegment_InitParams(s16 *data, s16 startPtX, s16 startPtY, s16 endPtX, s16 endPtY, s16 stepX, s16 stepY);
static bool32 BT_DiagonalSegment_ComputePointOnSegment(s16 *data, bool32 checkBoundary1, bool32 checkBoundary2);
static void BT_Phase2Blur(u32 taskId);
static void BT_Phase2DistortedWave(u32 taskId);
static void BT_Phase2HorizontalCorrugate(u32 taskId);
static void BT_Phase2BigPokeball(u32 taskId);
static void BT_Phase2Regice(u32 taskId);
static void BT_Phase2Registeel(u32 taskId);
static void BT_Phase2Regirock(u32 taskId);
static void BT_Phase2SlidingPokeballs(u32 taskId);
static void BT_Phase2ClockwiseBlackFade(u32 taskId);
static void BT_Phase2FullScreenWave(u32 taskId);
static void BT_Phase2BlackWaveToRight(u32 taskId);
static void BT_Phase2SlicedScreen(u32 taskId);
static void BT_Phase2WhiteFadeInStripes(u32 taskId);
static void BT_Phase2GridSquares(u32 taskId);
static void BT_Phase2BlackDoodles(u32 taskId);
static void BT_Phase2Mugshot(u32 taskId);
static void BT_Phase2AntiClockwiseSpiral(u32 taskId);
static void BT_Phase2ShredSplit(u32 taskId);
static void BT_Phase2Kyogre(u32 taskId);
static void BT_Phase2Groudon(u32 taskId);
static void BT_Phase2BlackHole(u32 taskId);
static void BT_Phase2BlackHolePulsate(u32 taskId);
static void BT_Phase2Rayquaza(u32 taskId);
static void BT_Phase2RectangularSpiral(u32 taskId);

static const TransitionStateFunc sBT_MainPhases[] =
{
    BT_Phase1Blink,
    BT_WaitForPhase1,
    BT_Phase2LaunchAnimTask,
    BT_WaitForPhase2,
};

static const TransitionStateFunc sBT_Phase1FadeFuncs[] =
{
    BT_Phase1_FadeOut,
    BT_Phase1_FadeIn,
};

static const TaskFunc sBT_Phase2Tasks[] =
{
    [B_TRANSITION_BLUR]                  = BT_Phase2Blur,
    [B_TRANSITION_DISTORTED_WAVE]        = BT_Phase2DistortedWave,
    [B_TRANSITION_HORIZONTAL_CORRUGATE]  = BT_Phase2HorizontalCorrugate,
    [B_TRANSITION_BIG_POKEBALL]          = BT_Phase2BigPokeball,
    [B_TRANSITION_REGICE]                = BT_Phase2Regice,
    [B_TRANSITION_REGISTEEL]             = BT_Phase2Registeel,
    [B_TRANSITION_REGIROCK]              = BT_Phase2Regirock,
    [B_TRANSITION_SLIDING_POKEBALLS]     = BT_Phase2SlidingPokeballs,
    [B_TRANSITION_CLOCKWISE_BLACKFADE]   = BT_Phase2ClockwiseBlackFade,
    [B_TRANSITION_FULLSCREEN_WAVE]       = BT_Phase2FullScreenWave,
    [B_TRANSITION_BLACK_WAVE_TO_RIGHT]   = BT_Phase2BlackWaveToRight,
    [B_TRANSITION_SLICED_SCREEN]         = BT_Phase2SlicedScreen,
    [B_TRANSITION_WHITEFADE_IN_STRIPES]  = BT_Phase2WhiteFadeInStripes,
    [B_TRANSITION_GRID_SQUARES]          = BT_Phase2GridSquares,
    [B_TRANSITION_BLACK_DOODLES]         = BT_Phase2BlackDoodles,
    [B_TRANSITION_MUGSHOT]               = BT_Phase2Mugshot,
    [B_TRANSITION_ANTI_CLOCKWISE_SPIRAL] = BT_Phase2AntiClockwiseSpiral,
    [B_TRANSITION_SHRED_SPLIT]           = BT_Phase2ShredSplit,
    [B_TRANSITION_KYOGRE]                = BT_Phase2Kyogre,
    [B_TRANSITION_GROUDON]               = BT_Phase2Groudon,
    [B_TRANSITION_BLACKHOLE]             = BT_Phase2BlackHole,
    [B_TRANSITION_BLACKHOLE_PULSATE]     = BT_Phase2BlackHolePulsate,
    [B_TRANSITION_RAYQUAZA]              = BT_Phase2Rayquaza,
    [B_TRANSITION_RECTANGULAR_SPIRAL]    = BT_Phase2RectangularSpiral,
};

////////////////////////
// TRANSITION PHASE 1 //
////////////////////////

#define tState          data[0]
#define tTransitionId   data[1]
#define tTransitionDone data[15]

void BT_StartOnField(u32 transitionId)
{
    sTransitionStructPtr = AllocZeroed(sizeof(struct TransitionData));
    gMain.callback2 = CB2_OverworldBasic;
    BT_LaunchTask(transitionId);
}

static void BT_LaunchTask(u32 transitionId)
{
    gTasks[CreateTask(BT_TaskMain, 2)].tTransitionId = transitionId;
}

static void BT_TaskMain(u32 taskId)
{
    while (sBT_MainPhases[gTasks[taskId].tState](&gTasks[taskId]));
}

bool32 BT_IsDone(void)
{
    u32 taskId = FindTaskIdByFunc(BT_TaskMain);

    if (gTasks[taskId].tTransitionDone)
    {
        BT_InitCtrlBlk();
        FREE_AND_SET_NULL(sTransitionStructPtr);
        DestroyTask(taskId);
        return TRUE;
    }
    else
        return FALSE;
}

static bool32 BT_Phase1Blink(struct Task *task)
{
    SetWeatherScreenFadeOut();
    CpuCopy32(gPlttBufferFaded, gPlttBufferUnfaded, 0x400);
    CreateTask(BT_Phase1Task, 4);
    ++task->tState;
    return FALSE;
}

static bool32 BT_WaitForPhase1(struct Task *task)
{
    if (FindTaskIdByFunc(BT_Phase1Task) == TAIL_SENTINEL)
    {
        ++task->tState;
        return TRUE;
    }
    else
        return FALSE;
}

static bool32 BT_Phase2LaunchAnimTask(struct Task *task)
{
    CreateTask(sBT_Phase2Tasks[task->tTransitionId], 0);
    ++task->tState;
    return FALSE;
}

static bool32 BT_WaitForPhase2(struct Task *task)
{
    task->tTransitionDone = (FindTaskIdByFunc(sBT_Phase2Tasks[task->tTransitionId]) == TAIL_SENTINEL);
    return FALSE;
}

static bool32 BT_IsPhase1Done(void)
{
    return (FindTaskIdByFunc(BT_Phase1SubTask) == TAIL_SENTINEL);
}

static void BT_Phase1Task(u32 taskId)
{
    if (!gTasks[taskId].tState)
    {
        ++gTasks[taskId].tState;
        BT_CreatePhase1SubTask(0, 0, 2, 2, 2);
    }
    else if (BT_IsPhase1Done())
        DestroyTask(taskId);
}

#define tFadeOutDelay data[1]
#define tFadeInDelay  data[2]
#define tBlinkTimes   data[3]
#define tFadeOutSpeed data[4]
#define tFadeInSpeed  data[5]
#define tDelayCounter data[6]
#define tCoeff        data[7]
#define tBldCntSaved  data[8]
#define tShadowColor  data[9]

static void BT_CreatePhase1SubTask(s16 fadeOutDelay, s16 fadeInDelay, s16 blinkTimes, s16 fadeOutSpeed, s16 fadeInSpeed)
{
    u32 taskId = CreateTask(BT_Phase1SubTask, 3);
    gTasks[taskId].tFadeOutDelay = fadeOutDelay;
    gTasks[taskId].tFadeInDelay = fadeInDelay;
    gTasks[taskId].tBlinkTimes = blinkTimes;
    gTasks[taskId].tFadeOutSpeed = fadeOutSpeed;
    gTasks[taskId].tFadeInSpeed = fadeInSpeed;
    gTasks[taskId].tDelayCounter = fadeOutDelay;
}

static void BT_Phase1SubTask(u32 taskId)
{
    while (sBT_Phase1FadeFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase1_FadeOut(struct Task *task)
{
    if (task->tDelayCounter == 0 || --task->tDelayCounter == 0)
    {
        task->tDelayCounter = task->tFadeOutDelay;
        
        task->tCoeff += task->tFadeOutSpeed;
        if (task->tCoeff > 16)
            task->tCoeff = 16;
        
        task->tShadowColor = GetShadowColor();
        
        BlendPalettes(PALETTES_ALL, task->tCoeff, RGB(11, 11, 11));
        
        UpdateShadowColor(task->tShadowColor);
    }
    
    if (task->tCoeff > 15)
    {
        // Save BLDCNT and turn off targets temporarily
        task->tBldCntSaved = GetGpuReg(REG_OFFSET_BLDCNT);
        SetGpuReg(REG_OFFSET_BLDCNT, task->tBldCntSaved & ~(BLDCNT_TGT2_BG_ALL));
        UpdateShadowColor(RGB(11, 11, 11));
        task->tDelayCounter = task->tFadeInDelay;
        ++task->tState;
    }
    return FALSE;
}

static bool32 BT_Phase1_FadeIn(struct Task *task)
{
    if (task->tDelayCounter == 0 || --task->tDelayCounter == 0)
    {
        task->tDelayCounter = task->tFadeInDelay;
        
        task->tCoeff -= task->tFadeInSpeed;
        if (task->tCoeff < 0)
            task->tCoeff = 0;
        
        BlendPalettes(PALETTES_ALL, task->tCoeff, RGB(11, 11, 11));
        
        // Restore BLDCNT
        SetGpuReg(REG_OFFSET_BLDCNT, task->tBldCntSaved);
        UpdateShadowColor(task->tShadowColor);
    }
    
    if (task->tCoeff == 0)
    {
        if (--task->tBlinkTimes == 0)
            DestroyTask(FindTaskIdByFunc(BT_Phase1SubTask));
        else
        {
            task->tDelayCounter = task->tFadeOutDelay;
            task->tState = 0;
        }
    }
    return FALSE;
}

#undef tFadeOutDelay
#undef tFadeInDelay
#undef tBlinkTimes
#undef tFadeOutSpeed
#undef tFadeInSpeed
#undef tDelayCounter
#undef tCoeff

///////////////////////
// UTILITY FUNCTIONS //
///////////////////////

static void BT_InitCtrlBlk(void)
{
    memset(sTransitionStructPtr, 0, sizeof(*sTransitionStructPtr));
    FieldCameraGetPixelOffsetAtGround(&sTransitionStructPtr->bg123HOfs, &sTransitionStructPtr->bg123VOfs);
}

static void BT_VBSyncOamAndPltt(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void BT_BlendPalettesToBlack(void)
{
    BlendPalettes(PALETTES_ALL, 0x10, RGB_BLACK);
}

static void HBCB_BT_Phase2UpdateBgsHorizontalOffsets(void)
{
    s16 offset = gScanlineEffectRegBuffers[1][REG_VCOUNT];
    REG_BG1HOFS = offset;
    REG_BG2HOFS = offset;
    REG_BG3HOFS = offset;
}

static void HBCB_BT_Phase2UpdateBgsVerticalOffsets(void)
{
    s16 offset = gScanlineEffectRegBuffers[1][REG_VCOUNT];
    REG_BG1VOFS = offset;
    REG_BG2VOFS = offset;
    REG_BG3VOFS = offset;
}

static void BT_GetBg0TilesetBase(u16 **tilesetPtr)
{
    u16 charBase = GetGpuReg(REG_OFFSET_BG0CNT) >> 2;
    charBase <<= 14;
    *tilesetPtr = (u16 *)(VRAM + charBase);
}

static void BT_GetBg0TilemapBase(u16 **tilemapPtr)
{
    u16 screenBase = (GetGpuReg(REG_OFFSET_BG0CNT) >> 8) & 0x1F;
    screenBase <<= 11;
    *tilemapPtr = (u16 *)(VRAM + screenBase);
}

static void BT_GetBg0TilemapAndTilesetBase(u16 **tilemapPtr, u16 **tilesetPtr)
{
    BT_GetBg0TilemapBase(tilemapPtr);
    BT_GetBg0TilesetBase(tilesetPtr);
}

static void BT_DestroyPhase2AnimTask(struct Task *task)
{
    DestroyTask(FindTaskIdByFunc(task->func));
}

static bool32 BT_EndScanlinePhase2Transition(struct Task *task)
{
    DmaStop(0);
    BT_BlendPalettesToBlack();
    BT_DestroyPhase2AnimTask(task);
    return FALSE;
}

static void BT_LoadWaveIntoBuffer(s16 *buffer, s16 offset, s16 theta, s16 frequency, s16 amplitude, s16 bufSize)
{
    u32 i;

    for (i = 0; bufSize > 0; --bufSize, ++i, theta += frequency)
        buffer[i] = offset + Sin(0xFF & theta, amplitude);
}

static void BT_GenerateCircle(s16 *buffer, s16 x, s16 y, s16 radius)
{
    s16 i;

    memset(buffer, 0xA, DISPLAY_HEIGHT * sizeof(u16));
    
    // 64 iterations because we only want to cover [0, π/2) discretely.
    for (i = 0; i < 64; ++i)
    {
        s16 sinResult, cosResult, leftX, topY, bottomY, nextTopY, nextBottomY, winVal;

        // The loop variable i here does not stand for rotation angle, 
        // but is the angle between segment (center, pointOnCircle) 
        // and vertical line.   
        sinResult = Sin(i, radius);
        cosResult = Cos(i, radius);
        
        leftX = x - sinResult;
        winVal = x + sinResult;
        topY = y - cosResult;
        bottomY = y + cosResult;
        
        if (leftX < 0)
            leftX = 0;
        
        if (winVal > DISPLAY_WIDTH)
            winVal = DISPLAY_WIDTH;
        
        if (topY < 0)
            topY = 0;
        
        if (bottomY > DISPLAY_HEIGHT - 1)
            bottomY = DISPLAY_HEIGHT - 1;
        
        winVal |= (leftX << 8);
        buffer[topY] = winVal;
        buffer[bottomY] = winVal;
        
        cosResult = Cos(i + 1, radius);
        nextTopY = y - cosResult;
        nextBottomY = y + cosResult;
        
        if (nextTopY < 0)
            nextTopY = 0;
        
        if (nextBottomY > DISPLAY_HEIGHT - 1)
            nextBottomY = DISPLAY_HEIGHT - 1;
        
        // fill everything in between with the same WIN0H value
        while (topY > nextTopY)
            buffer[--topY] = winVal;
        
        while (topY < nextTopY)
            buffer[++topY] = winVal;
        
        while (bottomY > nextBottomY)
            buffer[--bottomY] = winVal;
        
        while (bottomY < nextBottomY)
            buffer[++bottomY] = winVal;
    }
}

#define trStartPtX   data[0]
#define trStartPtY   data[1]
#define trCurrentPtX data[2]
#define trCurrentPtY data[3]
#define trEndPtX     data[4]
#define trEndPtY     data[5]
#define trStepX      data[6]
#define trStepY      data[7]
#define trAbsDeltaX  data[8]
#define trAbsDeltaY  data[9]
#define trAccum      data[10] // track one dimension based on slope

static void BT_DiagonalSegment_InitParams(s16 *data, s16 startPtX, s16 startPtY, s16 endPtX, s16 endPtY, s16 stepX, s16 stepY)
{
    trStartPtX = startPtX;
    trStartPtY = startPtY;
    
    trCurrentPtX = startPtX;
    trCurrentPtY = startPtY;
    
    trEndPtX = endPtX;
    trEndPtY = endPtY;
    
    trStepX = stepX;
    trStepY = stepY;
    trAbsDeltaX = endPtX - startPtX;
    
    if (trAbsDeltaX < 0)
    {
        trAbsDeltaX = -trAbsDeltaX;
        trStepX = -stepX;
    }
    trAbsDeltaY = endPtY - startPtY;
    
    if (trAbsDeltaY < 0)
    {
        trAbsDeltaY = -trAbsDeltaY;
        trStepY = -stepY;
    }
    trAccum = 0;
}

static bool32 BT_DiagonalSegment_ComputePointOnSegment(s16 *data, bool32 checkBoundary1, bool32 checkBoundary2)
{
    u32 finish;

    if (trAbsDeltaX > trAbsDeltaY)
    {
        trCurrentPtX += trStepX;
        trAccum += trAbsDeltaY;
        
        if (trAccum > trAbsDeltaX)
        {
            trCurrentPtY += trStepY;
            trAccum -= trAbsDeltaX;
        }
    }
    else
    {
        trCurrentPtY += trStepY;
        trAccum += trAbsDeltaX;
        
        if (trAccum > trAbsDeltaY)
        {
            trCurrentPtX += trStepX;
            trAccum -= trAbsDeltaY;
        }
    }
    finish = 0;
    
    if ((trStepX > 0 && trCurrentPtX >= trEndPtX) || (trStepX < 0 && trCurrentPtX <= trEndPtX))
    {
        ++finish;
        
        if (checkBoundary1)
            trCurrentPtX = trEndPtX;
    }
    if ((trStepY > 0 && trCurrentPtY >= trEndPtY) || (trStepY < 0 && trCurrentPtY <= trEndPtY))
    {
        ++finish;
        
        if (checkBoundary2)
            trCurrentPtY = trEndPtY;
    }
    
    if (finish == 2)
        return TRUE;
    else
        return FALSE;
}

#undef trStartPtX
#undef trStartPtY
#undef trCurrentPtX
#undef trCurrentPtY
#undef trEndPtX
#undef trEndPtY
#undef trStepX
#undef trStepY
#undef trAbsDeltaX
#undef trAbsDeltaY
#undef trAccum

//-------------------
// B_TRANSITION_BLUR
//-------------------

static bool32 BT_Phase2Blur_InitBgMosaic(struct Task *task);
static bool32 BT_Phase2Blur_Anim(struct Task *task);
static bool32 BT_Phase2Blur_IsDone(struct Task *task);

static const TransitionStateFunc sBT_Phase2BlurFuncs[] =
{
    BT_Phase2Blur_InitBgMosaic,
    BT_Phase2Blur_Anim,
    BT_Phase2Blur_IsDone,
};

#define tInterval   data[1]
#define tMosaicSize data[2]

static void BT_Phase2Blur(u32 taskId)
{
    while (sBT_Phase2BlurFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2Blur_InitBgMosaic(struct Task *task)
{
    SetGpuReg(REG_OFFSET_MOSAIC, 0);
    SetGpuRegBits(REG_OFFSET_BG1CNT, BGCNT_MOSAIC);
    SetGpuRegBits(REG_OFFSET_BG2CNT, BGCNT_MOSAIC);
    SetGpuRegBits(REG_OFFSET_BG3CNT, BGCNT_MOSAIC);
    ++task->tState;
    return TRUE;
}

static bool32 BT_Phase2Blur_Anim(struct Task *task)
{
    if (task->tInterval)
        --task->tInterval;
    else
    {
        task->tInterval = 2;
        
        if (++task->tMosaicSize == 10)
            BeginNormalPaletteFade(PALETTES_ALL, -1, 0, 0x10, RGB_BLACK);
        
        // The mosaic size argument is shared by HSIZE and VSIZE
        SetGpuReg(REG_OFFSET_MOSAIC, (task->tMosaicSize & 0xF) + ((task->tMosaicSize & 0xF) << 4));
        if (task->tMosaicSize > 14)
            ++task->tState;
    }
    return FALSE;
}

static bool32 BT_Phase2Blur_IsDone(struct Task *task)
{
    if (!gPaletteFade.active)
        BT_DestroyPhase2AnimTask(task);

    return FALSE;
}

#undef tInterval
#undef tMosaicSize

//-----------------------------
// B_TRANSITION_DISTORTED_WAVE
//-----------------------------

static bool32 BT_Phase2DistortedWave_InitWave(struct Task *task);
static bool32 BT_Phase2DistortedWave_UpdateWave(struct Task *task);
static void VBCB_BT_Phase2DistortedWave(void);

static const TransitionStateFunc sBT_Phase2DistortedWaveFuncs[] =
{
    BT_Phase2DistortedWave_InitWave,
    BT_Phase2DistortedWave_UpdateWave,
};

#define tTheta     data[1]
#define tAmplitude data[2]

static void BT_Phase2DistortedWave(u32 taskId)
{
    while (sBT_Phase2DistortedWaveFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2DistortedWave_InitWave(struct Task *task)
{
    BT_InitCtrlBlk();
    ScanlineEffect_Clear();
    BeginNormalPaletteFade(PALETTES_ALL, 4, 0, 0x10, RGB_BLACK);
    BT_LoadWaveIntoBuffer(gScanlineEffectRegBuffers[1], sTransitionStructPtr->bg123HOfs, 0, 2, 0, DISPLAY_HEIGHT);
    SetVBlankCallback(VBCB_BT_Phase2DistortedWave);
    SetHBlankCallback(HBCB_BT_Phase2UpdateBgsHorizontalOffsets);
    EnableInterrupts(INTR_FLAG_VBLANK | INTR_FLAG_HBLANK);
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2DistortedWave_UpdateWave(struct Task *task)
{
    sTransitionStructPtr->vblankDma = FALSE;
    
    task->tTheta += 4;
    task->tAmplitude += 8;
    
    BT_LoadWaveIntoBuffer(gScanlineEffectRegBuffers[0], sTransitionStructPtr->bg123HOfs, task->tTheta, 2, task->tAmplitude, DISPLAY_HEIGHT);
    
    if (!gPaletteFade.active)
        BT_DestroyPhase2AnimTask(task);
    
    ++sTransitionStructPtr->vblankDma;
    return FALSE;
}

static void VBCB_BT_Phase2DistortedWave(void)
{
    BT_VBSyncOamAndPltt();
    
    if (sTransitionStructPtr->vblankDma)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], DISPLAY_HEIGHT * 2);
}

#undef tTheta
#undef tAmplitude

//-----------------------------------
// B_TRANSITION_HORIZONTAL_CORRUGATE
//-----------------------------------

static bool32 BT_Phase2HorizontalCorrugate_Init(struct Task *task);
static bool32 BT_Phase2HorizontalCorrugate_UpdateWave(struct Task *task);

static const TransitionStateFunc sBT_Phase2HorizontalCorrugateFuncs[] =
{
    BT_Phase2HorizontalCorrugate_Init,
    BT_Phase2HorizontalCorrugate_UpdateWave,
};

#define tTheta     data[1]
#define tAmplitude data[2]

static void BT_Phase2HorizontalCorrugate(u32 taskId)
{
    while (sBT_Phase2HorizontalCorrugateFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2HorizontalCorrugate_Init(struct Task *task)
{
    BT_InitCtrlBlk();
    ScanlineEffect_Clear();
    BeginNormalPaletteFade(PALETTES_ALL, 4, 0, 0x10, RGB_BLACK);
    memset(gScanlineEffectRegBuffers[1], sTransitionStructPtr->bg123VOfs, DISPLAY_HEIGHT * 2);
    SetVBlankCallback(VBCB_BT_Phase2DistortedWave);
    SetHBlankCallback(HBCB_BT_Phase2UpdateBgsVerticalOffsets);
    EnableInterrupts(INTR_FLAG_VBLANK | INTR_FLAG_HBLANK);
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2HorizontalCorrugate_UpdateWave(struct Task *task)
{
    u32 i;
    u16 theta, amplitude;

    sTransitionStructPtr->vblankDma = FALSE;
    
    theta = task->tTheta;
    amplitude = task->tAmplitude >> 8;
    
    task->tTheta += 4224;
    task->tAmplitude += 384;
    
    for (i = 0; i < DISPLAY_HEIGHT; ++i, theta += 4224)
        gScanlineEffectRegBuffers[0][i] = sTransitionStructPtr->bg123VOfs + Sin(theta / 256, amplitude);
    
    if (!gPaletteFade.active)
        BT_DestroyPhase2AnimTask(task);
    
    ++sTransitionStructPtr->vblankDma;
    
    return FALSE;
}

#undef tTheta
#undef tAmplitude

//---------------------------
// B_TRANSITION_BIG_POKEBALL
//---------------------------

static bool32 BT_Phase2BigPokeball_Init(struct Task *task);
static bool32 BT_Phase2BigPokeball_LoadTilemapAndWave(struct Task *task);
static bool32 BT_Phase2BigPokeball_UpdateWave1IncEva(struct Task *task);
static bool32 BT_Phase2BigPokeball_UpdateWave2DecEvb(struct Task *task);
static bool32 BT_Phase2BigPokeball_UpdateWave3(struct Task *task);
static bool32 BT_Phase2BigPokeball_CircleEffect(struct Task *task);
static void BT_Phase2_InitPatternWaveTransition(struct Task *task, const u32 *tileset, u32 tilesetSize, const u16 *palette, bool32 fastTransition);
static void VBCB_BT_Phase2BigPokeball1(void);
static void VBCB_BT_Phase2BigPokeball2(void);

static const u32 sBigPokeballTileset[] = INCBIN_U32("graphics/battle_transitions/big_pokeball_tileset.4bpp");
static const u16 sBigPokeballTilemap[] = INCBIN_U16("graphics/battle_transitions/big_pokeball_tilemap.bin");

static const TransitionStateFunc sBT_Phase2BigPokeballFuncs[] =
{
    BT_Phase2BigPokeball_Init,
    BT_Phase2BigPokeball_LoadTilemapAndWave,
    BT_Phase2BigPokeball_UpdateWave1IncEva,
    BT_Phase2BigPokeball_UpdateWave2DecEvb,
    BT_Phase2BigPokeball_UpdateWave3,
    BT_Phase2BigPokeball_CircleEffect,
};

#define tEvb       data[1]
#define tEva       data[2]
#define tInterval  data[3]
#define tTheta     data[4]
#define tAmplitude data[5]
#define tFastAnim  data[6]

static void BT_Phase2BigPokeball(u32 taskId)
{
    while (sBT_Phase2BigPokeballFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2BigPokeball_Init(struct Task *task)
{
    BT_Phase2_InitPatternWaveTransition(task, sBigPokeballTileset, sizeof(sBigPokeballTileset), gSlidingPokeballBigPokeballPalette, TRUE);
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2BigPokeball_LoadTilemapAndWave(struct Task *task)
{
    u32 i, j;
    u16 *tilemapAddr;
    const u16 *tilemap = sBigPokeballTilemap;

    BT_GetBg0TilemapBase(&tilemapAddr);
    
    for (i = 0; i < 20; ++i)
    {
        for (j = 0; j < 30; ++j, ++tilemap)
            tilemapAddr[i * 32 + j] = *tilemap | 0xF000; // use palette #15
    }
    BT_LoadWaveIntoBuffer(gScanlineEffectRegBuffers[0], 0, task->tTheta, 132, task->tAmplitude, DISPLAY_HEIGHT);

    ++task->tState;
    return TRUE;
}

static bool32 BT_Phase2BigPokeball_UpdateWave1IncEva(struct Task *task)
{
    sTransitionStructPtr->vblankDma = FALSE;
    
    if (task->tInterval == 0 || --task->tInterval == 0)
    {
        ++task->tEva;
        task->tInterval = 1; // Broken logic. This makes the condition always TRUE. 
    }
    sTransitionStructPtr->bldAlpha = BLDALPHA_BLEND(task->tEva, task->tEvb);
    
    // Increment eva until it reaches 50% coeff
    if (task->tEva > 15)
        ++task->tState;
    
    if (task->tFastAnim)
    {
        task->tTheta += 12;
        task->tAmplitude -= 384;
    }
    else
    {
        task->tTheta += 8;
        task->tAmplitude -= 256;
    }

    // Assign a very high frequency value so that 2 adjacent values in gScanlineEffectRegBuffers[0] will have different sign. 
    BT_LoadWaveIntoBuffer(gScanlineEffectRegBuffers[0], 0, task->tTheta, 132, task->tAmplitude >> 8, DISPLAY_HEIGHT);
    ++sTransitionStructPtr->vblankDma;
    
    return FALSE;
}

static bool32 BT_Phase2BigPokeball_UpdateWave2DecEvb(struct Task *task)
{
    sTransitionStructPtr->vblankDma = FALSE;
    
    if (task->tInterval == 0 || --task->tInterval == 0)
    {
        --task->tEvb;
        task->tInterval = 2;
    }
    sTransitionStructPtr->bldAlpha = (task->tEvb << 8) | task->tEva;
    
    if (!task->tEvb)
        ++task->tState;
    
    if (task->tAmplitude > 0)
    {
        if (task->tFastAnim)
        {
            task->tTheta += 12;
            task->tAmplitude -= 384;
        }
        else
        {
            task->tTheta += 8;
            task->tAmplitude -= 256;
        }
    }
    else
        task->tAmplitude = 0;

    BT_LoadWaveIntoBuffer(gScanlineEffectRegBuffers[0], 0, task->tTheta, 132, task->tAmplitude >> 8, DISPLAY_HEIGHT);
    ++sTransitionStructPtr->vblankDma;
    return FALSE;
}

#define tRadius      data[1]
#define tDeltaRadius data[2]
#define tKeepVBCB    data[3]

static bool32 BT_Phase2BigPokeball_UpdateWave3(struct Task *task)
{
    sTransitionStructPtr->vblankDma = FALSE;
    
    if (task->tAmplitude > 0)
    {
        if (task->tFastAnim)
        {
            task->tTheta += 12;
            task->tAmplitude -= 384;
        }
        else
        {
            task->tTheta += 8;
            task->tAmplitude -= 256;
        }
    }
    else
        task->tAmplitude = 0;

    BT_LoadWaveIntoBuffer(gScanlineEffectRegBuffers[0], 0, task->tTheta, 132, task->tAmplitude >> 8, DISPLAY_HEIGHT);
    
    if (task->tAmplitude <= 0)
    {
        ++task->tState;
        task->tRadius = DISPLAY_HEIGHT;
        task->tDeltaRadius = 256;
        task->tKeepVBCB = 0;
    }
    ++sTransitionStructPtr->vblankDma;
    return FALSE;
}

static bool32 BT_Phase2BigPokeball_CircleEffect(struct Task *task)
{
    sTransitionStructPtr->vblankDma = FALSE;
    
    if (task->tFastAnim)
    {
        if (task->tDeltaRadius < 2048)
            task->tDeltaRadius += 256;
    }
    else
    {
        if (task->tDeltaRadius < 1024)
            task->tDeltaRadius += 128;
    }
    
    if (task->tRadius)
    {
        task->tRadius -= (task->tDeltaRadius >> 8);
        if (task->tRadius < 0)
            task->tRadius = 0;
    }
    BT_GenerateCircle(gScanlineEffectRegBuffers[0], DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, task->tRadius);
    
    if (task->tRadius == 0)
        BT_EndScanlinePhase2Transition(task);

    if (task->tKeepVBCB == 0)
    {
        ++task->tKeepVBCB;
        SetVBlankCallback(VBCB_BT_Phase2BigPokeball2);
    }
    ++sTransitionStructPtr->vblankDma;
    return FALSE;
}

static void BT_Phase2_InitPatternWaveTransition(struct Task *task, const u32 *tileset, u32 tilesetSize, const u16 *palette, bool32 fastTransition)
{
    u32 i;
    u16 *tilemapAddr, *tilesetAddr;

    BT_InitCtrlBlk();
    ScanlineEffect_Clear();
    
    task->tEvb = 16;
    task->tEva = 0;
    task->tTheta = 0;
    task->tAmplitude = 0x4000;
    task->tFastAnim = fastTransition;
    
    sTransitionStructPtr->winIn = WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR;
    sTransitionStructPtr->winOut = 0;
    sTransitionStructPtr->win0H = WIN_RANGE(0, DISPLAY_WIDTH);
    sTransitionStructPtr->win0V = WIN_RANGE(0, DISPLAY_HEIGHT);
    sTransitionStructPtr->bldCnt = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 | BLDCNT_TGT2_OBJ | BLDCNT_TGT2_BD;
    sTransitionStructPtr->bldAlpha = (task->tEvb << 8) | task->tEva;
    
    UpdateShadowColor(RGB_GRAY);
    
    for (i = 0; i < DISPLAY_HEIGHT; ++i)
        gScanlineEffectRegBuffers[1][i] = DISPLAY_WIDTH;
    
    SetVBlankCallback(VBCB_BT_Phase2BigPokeball1);
    BT_GetBg0TilemapAndTilesetBase(&tilemapAddr, &tilesetAddr);
    
    CpuFill16(0, tilemapAddr, 0x800);
    CpuCopy16(tileset, tilesetAddr, tilesetSize);
    
    LoadPalette(palette, 0xF0, 0x20);
}

#undef tRadius
#undef tDeltaRadius
#undef tKeepVBCB

static void BT_VBStopDma0SyncSrcBufferSetLcdRegs(void)
{
    DmaStop(0);
    BT_VBSyncOamAndPltt();
    
    if (sTransitionStructPtr->vblankDma)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], DISPLAY_HEIGHT * 2);
    
    SetGpuReg(REG_OFFSET_WININ, sTransitionStructPtr->winIn);
    SetGpuReg(REG_OFFSET_WINOUT, sTransitionStructPtr->winOut);
    SetGpuReg(REG_OFFSET_WIN0V, sTransitionStructPtr->win0V);
    SetGpuReg(REG_OFFSET_BLDCNT, sTransitionStructPtr->bldCnt);
    SetGpuReg(REG_OFFSET_BLDALPHA, sTransitionStructPtr->bldAlpha);
}

static void VBCB_BT_Phase2BigPokeball1(void)
{
    BT_VBStopDma0SyncSrcBufferSetLcdRegs();
    DmaSet(0, gScanlineEffectRegBuffers[1], &REG_BG0HOFS, B_TRANS_DMA_FLAGS);
}

static void VBCB_BT_Phase2BigPokeball2(void)
{
    BT_VBStopDma0SyncSrcBufferSetLcdRegs();
    DmaSet(0, gScanlineEffectRegBuffers[1], &REG_WIN0H, B_TRANS_DMA_FLAGS);
}

//---------------------
// B_TRANSITION_REGICE
//---------------------

static bool32 BT_Phase2Regice_Init(struct Task *task);
static bool32 BT_Phase2Regice_LoadTilemapAndWave(struct Task *task);

static const u32 sRegisTileset[] = INCBIN_U32("graphics/battle_transitions/regis.4bpp");

static const u32 sRegiceTilemap[] = INCBIN_U32("graphics/battle_transitions/regice.bin");
static const u16 sRegicePalette[] = INCBIN_U16("graphics/battle_transitions/regice.gbapal");

static const TransitionStateFunc sBT_Phase2RegiceFuncs[] =
{
    BT_Phase2Regice_Init,
    BT_Phase2Regice_LoadTilemapAndWave,
    BT_Phase2BigPokeball_UpdateWave1IncEva,
    BT_Phase2BigPokeball_UpdateWave2DecEvb,
    BT_Phase2BigPokeball_UpdateWave3,
    BT_Phase2BigPokeball_CircleEffect,
};

static void BT_Phase2Regice(u32 taskId)
{
    while (sBT_Phase2RegiceFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2Regice_Init(struct Task *task)
{
    BT_Phase2_InitPatternWaveTransition(task, sRegisTileset, sizeof(sRegisTileset), sRegicePalette, FALSE);
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2Regice_LoadTilemapAndWave(struct Task *task)
{
    u16 *tilemapAddr;

    BT_GetBg0TilemapBase(&tilemapAddr);
    CpuCopy16(sRegiceTilemap, tilemapAddr, 0x500);
    
    BT_LoadWaveIntoBuffer(gScanlineEffectRegBuffers[0], 0, task->tTheta, 132, task->tAmplitude, DISPLAY_HEIGHT);

    ++task->tState;
    return TRUE;
}

//------------------------
// B_TRANSITION_REGISTEEL
//------------------------

static bool32 BT_Phase2Registeel_Init(struct Task *task);
static bool32 BT_Phase2Registeel_LoadTilemapAndWave(struct Task *task);

static const u32 sRegisteelTilemap[] = INCBIN_U32("graphics/battle_transitions/registeel.bin");
static const u16 sRegisteelPalette[] = INCBIN_U16("graphics/battle_transitions/registeel.gbapal");

static const TransitionStateFunc sBT_Phase2RegisteelFuncs[] =
{
    BT_Phase2Registeel_Init,
    BT_Phase2Registeel_LoadTilemapAndWave,
    BT_Phase2BigPokeball_UpdateWave1IncEva,
    BT_Phase2BigPokeball_UpdateWave2DecEvb,
    BT_Phase2BigPokeball_UpdateWave3,
    BT_Phase2BigPokeball_CircleEffect,
};

static void BT_Phase2Registeel(u32 taskId)
{
    while (sBT_Phase2RegisteelFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2Registeel_Init(struct Task *task)
{
    BT_Phase2_InitPatternWaveTransition(task, sRegisTileset, sizeof(sRegisTileset), sRegisteelPalette, FALSE);
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2Registeel_LoadTilemapAndWave(struct Task *task)
{
    u16 *tilemapAddr;

    BT_GetBg0TilemapBase(&tilemapAddr);
    CpuCopy16(sRegisteelTilemap, tilemapAddr, 0x500);
    
    BT_LoadWaveIntoBuffer(gScanlineEffectRegBuffers[0], 0, task->tTheta, 132, task->tAmplitude, DISPLAY_HEIGHT);

    ++task->tState;
    return TRUE;
}

//-----------------------
// B_TRANSITION_REGIROCK
//-----------------------

static bool32 BT_Phase2Regirock_Init(struct Task *task);
static bool32 BT_Phase2Regirock_LoadTilemapAndWave(struct Task *task);

static const u32 sRegirockTilemap[] = INCBIN_U32("graphics/battle_transitions/regirock.bin");
static const u16 sRegirockPalette[] = INCBIN_U16("graphics/battle_transitions/regirock.gbapal");

static const TransitionStateFunc sBT_Phase2RegirockFuncs[] =
{
    BT_Phase2Regirock_Init,
    BT_Phase2Regirock_LoadTilemapAndWave,
    BT_Phase2BigPokeball_UpdateWave1IncEva,
    BT_Phase2BigPokeball_UpdateWave2DecEvb,
    BT_Phase2BigPokeball_UpdateWave3,
    BT_Phase2BigPokeball_CircleEffect,
};

static void BT_Phase2Regirock(u32 taskId)
{
    while (sBT_Phase2RegirockFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2Regirock_Init(struct Task *task)
{
    BT_Phase2_InitPatternWaveTransition(task, sRegisTileset, sizeof(sRegisTileset), sRegirockPalette, FALSE);
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2Regirock_LoadTilemapAndWave(struct Task *task)
{
    u16 *tilemapAddr;

    BT_GetBg0TilemapBase(&tilemapAddr);
    CpuCopy16(sRegirockTilemap, tilemapAddr, 0x500);
    
    BT_LoadWaveIntoBuffer(gScanlineEffectRegBuffers[0], 0, task->tTheta, 132, task->tAmplitude, DISPLAY_HEIGHT);

    ++task->tState;
    return TRUE;
}

#undef tEvb
#undef tEva
#undef tInterval
#undef tTheta
#undef tAmplitude
#undef tFastAnim

//--------------------------------
// B_TRANSITION_SLIDING_POKEBALLS
//--------------------------------

static bool32 BT_Phase2SlidingPokeballs_LoadBgGfx(struct Task *task);
static bool32 BT_Phase2SlidingPokeballs_SetupFldeffArgs(struct Task *task);
static bool32 BT_Phase2SlidingPokeballs_IsDone(struct Task *task);

static const u32 sSlidingPokeballTilemap[] = INCBIN_U32("graphics/battle_transitions/sliding_pokeball_tilemap.bin");

static const s16 gUnknown_83FA400[] = { -16, 256 };
static const s16 gUnknown_83FA404[] = { 0, 16, 32, 8, 24 };

static const TransitionStateFunc sBT_Phase2SlidingPokeballsFuncs[] =
{
    BT_Phase2SlidingPokeballs_LoadBgGfx,
    BT_Phase2SlidingPokeballs_SetupFldeffArgs,
    BT_Phase2SlidingPokeballs_IsDone,
};

static void BT_Phase2SlidingPokeballs(u32 taskId)
{
    while (sBT_Phase2SlidingPokeballsFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2SlidingPokeballs_LoadBgGfx(struct Task *task)
{
    u16 *tilemapAddr, *tilesetAddr;

    BT_GetBg0TilemapAndTilesetBase(&tilemapAddr, &tilesetAddr);
    CpuCopy16(sSlidingPokeballTilemap, tilesetAddr, 0x40);
    CpuFill32(0, tilemapAddr, 0x800);
    LoadPalette(gSlidingPokeballBigPokeballPalette, 0xF0, 0x20);
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2SlidingPokeballs_SetupFldeffArgs(struct Task *task)
{
    u32 i;
    s16 rand;
    s16 arr0[ARRAY_COUNT(gUnknown_83FA400)];
    s16 arr1[ARRAY_COUNT(gUnknown_83FA404)];

    memcpy(arr0, gUnknown_83FA400, sizeof(gUnknown_83FA400));
    memcpy(arr1, gUnknown_83FA404, sizeof(gUnknown_83FA404));
    
    rand = Random() % 2;
    
    for (i = 0; i <= 4; ++i, rand ^= 1)
    {
        gFieldEffectArguments[0] = arr0[rand];      // x
        gFieldEffectArguments[1] = (i * 32) + 16;   // y
        gFieldEffectArguments[2] = rand;
        gFieldEffectArguments[3] = arr1[i];
        FieldEffectStart(FLDEFF_POKEBALL);
    }
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2SlidingPokeballs_IsDone(struct Task *task)
{
    if (!FieldEffectActiveListContains(FLDEFF_POKEBALL))
    {
        BT_BlendPalettesToBlack();
        BT_DestroyPhase2AnimTask(task);
    }
    return FALSE;
}

//----------------------------------
// B_TRANSITION_CLOCKWISE_BLACKFADE
//----------------------------------

static bool32 BT_Phase2ClockwiseBlackFade_Init(struct Task *task);
static bool32 BT_Phase2ClockwiseBlackFade_Step1(struct Task *task);
static bool32 BT_Phase2ClockwiseBlackFade_Step2(struct Task *task);
static bool32 BT_Phase2ClockwiseBlackFade_Step3(struct Task *task);
static bool32 BT_Phase2ClockwiseBlackFade_Step4(struct Task *task);
static bool32 BT_Phase2ClockwiseBlackFade_Step5(struct Task *task);
static void VBCB_BT_Phase2ClockwiseBlackFade(void);

static const TransitionStateFunc sBT_Phase2ClockwiseBlackFadeFuncs[] =
{
    BT_Phase2ClockwiseBlackFade_Init,
    BT_Phase2ClockwiseBlackFade_Step1,
    BT_Phase2ClockwiseBlackFade_Step2,
    BT_Phase2ClockwiseBlackFade_Step3,
    BT_Phase2ClockwiseBlackFade_Step4,
    BT_Phase2ClockwiseBlackFade_Step5,
    BT_EndScanlinePhase2Transition,
};

#define trStartPtX   data[0]
#define trStartPtY   data[1]
#define trCurrentPtX data[2]
#define trCurrentPtY data[3]
#define trEndPtX     data[4]
#define trEndPtY     data[5]

static void BT_Phase2ClockwiseBlackFade(u32 taskId)
{
    while (sBT_Phase2ClockwiseBlackFadeFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2ClockwiseBlackFade_Init(struct Task *task)
{
    u32 i;

    BT_InitCtrlBlk();
    ScanlineEffect_Clear();
    
    sTransitionStructPtr->winIn = 0;
    sTransitionStructPtr->winOut = WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR;
    sTransitionStructPtr->win0H = WIN_RANGE(DISPLAY_WIDTH, DISPLAY_WIDTH + 1);
    sTransitionStructPtr->win0V = DISPLAY_HEIGHT;
    
    for (i = 0; i < DISPLAY_HEIGHT; ++i)
        gScanlineEffectRegBuffers[1][i] = ((DISPLAY_WIDTH + 3) << 8) | (DISPLAY_WIDTH + 4);
    
    SetVBlankCallback(VBCB_BT_Phase2ClockwiseBlackFade);
    sTransitionStructPtr->trEndPtX = DISPLAY_WIDTH / 2;
    ++task->tState;
    return TRUE;
}

static bool32 BT_Phase2ClockwiseBlackFade_Step1(struct Task *task)
{
    sTransitionStructPtr->vblankDma = FALSE;
    BT_DiagonalSegment_InitParams(sTransitionStructPtr->data, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, sTransitionStructPtr->trEndPtX, 0, 1, 1);
    
    do
    {
        gScanlineEffectRegBuffers[0][sTransitionStructPtr->trCurrentPtY] = WIN_RANGE(DISPLAY_WIDTH / 2, sTransitionStructPtr->trCurrentPtX + 1);
    }
    while (!BT_DiagonalSegment_ComputePointOnSegment(sTransitionStructPtr->data, TRUE, TRUE));

    sTransitionStructPtr->trEndPtX += 32;
    if (sTransitionStructPtr->trEndPtX >= DISPLAY_WIDTH)
    {
        sTransitionStructPtr->trEndPtY = 0;
        ++task->tState;
    }
    ++sTransitionStructPtr->vblankDma;
    return FALSE;
}

static bool32 BT_Phase2ClockwiseBlackFade_Step2(struct Task *task)
{
    s16 left, right;
    vu8 finished = FALSE;

    sTransitionStructPtr->vblankDma = FALSE;
    BT_DiagonalSegment_InitParams(sTransitionStructPtr->data, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, DISPLAY_WIDTH, sTransitionStructPtr->trEndPtY, 1, 1);
    
    while (TRUE)
    {
        left = DISPLAY_WIDTH / 2;
        right = sTransitionStructPtr->trCurrentPtX + 1;
        
        if (sTransitionStructPtr->trEndPtY >= DISPLAY_HEIGHT / 2)
        {
            left = sTransitionStructPtr->trCurrentPtX;
            right = DISPLAY_WIDTH;
        }
        gScanlineEffectRegBuffers[0][sTransitionStructPtr->trCurrentPtY] = WIN_RANGE2(left, right);
        
        if (finished)
            break;
        
        finished = BT_DiagonalSegment_ComputePointOnSegment(sTransitionStructPtr->data, TRUE, TRUE);
    }
    
    sTransitionStructPtr->trEndPtY += 16;
    if (sTransitionStructPtr->trEndPtY >= DISPLAY_HEIGHT)
    {
        sTransitionStructPtr->trEndPtX = DISPLAY_WIDTH;
        ++task->tState;
    }
    else
    {
        while (sTransitionStructPtr->trCurrentPtY < sTransitionStructPtr->trEndPtY)
            gScanlineEffectRegBuffers[0][++sTransitionStructPtr->trCurrentPtY] = WIN_RANGE2(left, right);
    }
    ++sTransitionStructPtr->vblankDma;
    return FALSE;
}

static bool32 BT_Phase2ClockwiseBlackFade_Step3(struct Task *task)
{
    sTransitionStructPtr->vblankDma = FALSE;
    BT_DiagonalSegment_InitParams(sTransitionStructPtr->data, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, sTransitionStructPtr->trEndPtX, DISPLAY_HEIGHT, 1, 1);
    
    do
    {
        gScanlineEffectRegBuffers[0][sTransitionStructPtr->trCurrentPtY] = (sTransitionStructPtr->trCurrentPtX << 8) | DISPLAY_WIDTH;
    }
    while (!BT_DiagonalSegment_ComputePointOnSegment(sTransitionStructPtr->data, TRUE, TRUE));
    
    sTransitionStructPtr->trEndPtX -= 32;
    if (sTransitionStructPtr->trEndPtX <= 0)
    {
        sTransitionStructPtr->trEndPtY = DISPLAY_HEIGHT;
        ++task->tState;
    }
    ++sTransitionStructPtr->vblankDma;
    return FALSE;
}

/*
 * BUG: The following 2 functions are incorrect. The animation after 
 * the rotation angle reaches 1.5π will not be displayed. 
 *
 * There're 2 problems which need to be solved in order to correct the logic. 
 * 1. With current setup, nothing is displayed inside WIN0 and everything
 * is displayed outside WIN0. Thus, if the rotation angle is > 1.5π, it
 * won't be able to handle the situation. 
 * 2. The programmer sometimes swapped the place of left and right boundary
 * of WIN0 (see variables left and right), which will sometimes cause right
 * to be smaller than left. In this way, garbage data will be written to WIN0H. 
 */
static bool32 BT_Phase2ClockwiseBlackFade_Step4(struct Task *task)
{
    s16 right, left;
    u16 win0H;
    vu8 finished = FALSE;

    sTransitionStructPtr->vblankDma = FALSE;
    BT_DiagonalSegment_InitParams(sTransitionStructPtr->data, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, 0, sTransitionStructPtr->trEndPtY, 1, 1);
    
    while (TRUE)
    {
        right = (gScanlineEffectRegBuffers[0][sTransitionStructPtr->trCurrentPtY]) & 0xFF;
        left = sTransitionStructPtr->trCurrentPtX;
        
        if (sTransitionStructPtr->trEndPtY <= DISPLAY_HEIGHT / 2)
        {
            left = DISPLAY_WIDTH / 2;
            right = sTransitionStructPtr->trCurrentPtX;
        }
        win0H = WIN_RANGE2(left, right);
        
        gScanlineEffectRegBuffers[0][sTransitionStructPtr->trCurrentPtY] = win0H;
        if (finished)
            break;
        
        finished = BT_DiagonalSegment_ComputePointOnSegment(sTransitionStructPtr->data, TRUE, TRUE);
    }
    
    sTransitionStructPtr->trEndPtY -= 16;
    if (sTransitionStructPtr->trEndPtY <= 0)
    {
        sTransitionStructPtr->trEndPtX = 0;
        ++task->tState;
    }
    else
    {
        while (sTransitionStructPtr->trCurrentPtY > sTransitionStructPtr->trEndPtY)
            gScanlineEffectRegBuffers[0][--sTransitionStructPtr->trCurrentPtY] = WIN_RANGE2(left, right);
    }
    ++sTransitionStructPtr->vblankDma;
    return FALSE;
}

static bool32 BT_Phase2ClockwiseBlackFade_Step5(struct Task *task)
{
    s16 left, right;

    sTransitionStructPtr->vblankDma = FALSE;
    BT_DiagonalSegment_InitParams(sTransitionStructPtr->data, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, sTransitionStructPtr->trEndPtX, 0, 1, 1);
    
    do
    {
        left = DISPLAY_WIDTH / 2;
        right = sTransitionStructPtr->trCurrentPtX;
        
        if (sTransitionStructPtr->trCurrentPtX >= DISPLAY_WIDTH / 2)
        {
            left = 0;
            right = DISPLAY_WIDTH;
        }
        gScanlineEffectRegBuffers[0][sTransitionStructPtr->trCurrentPtY] = WIN_RANGE2(left, right);
    }
    while (!BT_DiagonalSegment_ComputePointOnSegment(sTransitionStructPtr->data, TRUE, TRUE));
    
    sTransitionStructPtr->trEndPtX += 32;
    if (sTransitionStructPtr->trCurrentPtX > DISPLAY_WIDTH / 2)
        ++task->tState;
    
    ++sTransitionStructPtr->vblankDma;
    return FALSE;
}

static void VBCB_BT_Phase2ClockwiseBlackFade(void)
{
    DmaStop(0);
    BT_VBSyncOamAndPltt();
    
    if (sTransitionStructPtr->vblankDma)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], DISPLAY_HEIGHT * 2);
    
    SetGpuReg(REG_OFFSET_WININ, sTransitionStructPtr->winIn);
    SetGpuReg(REG_OFFSET_WINOUT, sTransitionStructPtr->winOut);
    SetGpuReg(REG_OFFSET_WIN0V, sTransitionStructPtr->win0V);
    SetGpuReg(REG_OFFSET_WIN0H, gScanlineEffectRegBuffers[1][0]);
    
    DmaSet(0, gScanlineEffectRegBuffers[1], &REG_WIN0H, B_TRANS_DMA_FLAGS);
}

#undef trStartPtX
#undef trStartPtY
#undef trCurrentPtX
#undef trCurrentPtY
#undef trEndPtX
#undef trEndPtY

//------------------------------
// B_TRANSITION_FULLSCREEN_WAVE
//------------------------------

static bool32 BT_Phase2FullScreenWave_Init(struct Task *task);
static bool32 BT_Phase2FullScreenWave_UpdateWave(struct Task *task);

static const TransitionStateFunc sBT_Phase2FullScreenWaveFuncs[] =
{
    BT_Phase2FullScreenWave_Init,
    BT_Phase2FullScreenWave_UpdateWave,
};

#define tTheta        data[1]
#define tAmplitude    data[2]
#define tDelayForFade data[3]
#define tStartFade    data[4]

static void BT_Phase2FullScreenWave(u32 taskId)
{
    while (sBT_Phase2FullScreenWaveFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2FullScreenWave_Init(struct Task *task)
{
    u32 i;

    BT_InitCtrlBlk();
    ScanlineEffect_Clear();
    
    for (i = 0; i < DISPLAY_HEIGHT; ++i)
        gScanlineEffectRegBuffers[1][i] = sTransitionStructPtr->bg123VOfs;
    
    SetVBlankCallback(VBCB_BT_Phase2DistortedWave);
    SetHBlankCallback(HBCB_BT_Phase2UpdateBgsVerticalOffsets);
    EnableInterrupts(INTR_FLAG_HBLANK);
    
    ++task->tState;
    return TRUE;
}

static bool32 BT_Phase2FullScreenWave_UpdateWave(struct Task *task)
{
    u32 i;
    s16 amplitude;
    u16 theta, frequency;

    sTransitionStructPtr->vblankDma = FALSE;
    
    amplitude = task->tAmplitude >> 8;
    theta = task->tTheta;
    frequency = 384;
    task->tTheta += 0x400;
    
    if (task->tAmplitude <= 0x1FFF)
        task->tAmplitude += 384;
    
    for (i = 0; i < DISPLAY_HEIGHT; ++i, theta += frequency)
        gScanlineEffectRegBuffers[0][i] = sTransitionStructPtr->bg123VOfs + Sin(theta >> 8, amplitude);
    
    if (++task->tDelayForFade == 41)
    {
        ++task->tStartFade;
        BeginNormalPaletteFade(PALETTES_ALL, -8, 0, 0x10, RGB_BLACK);
    }
    
    if (task->tStartFade && !gPaletteFade.active)
        BT_DestroyPhase2AnimTask(task);
    
    ++sTransitionStructPtr->vblankDma;
    return FALSE;
}

#undef tTheta
#undef tAmplitude
#undef tDelayForFade
#undef tStartFade

//----------------------------------
// B_TRANSITION_BLACK_WAVE_TO_RIGHT
//----------------------------------

static bool32 BT_Phase2BlackWaveToRight_Init(struct Task *task);
static bool32 BT_Phase2BlackWaveToRight_UpdateWave(struct Task *task);
static void VBCB_BT_Phase2BlackWaveToRight(void);

static const TransitionStateFunc sBT_Phase2BlackWaveToRightFuncs[] =
{
    BT_Phase2BlackWaveToRight_Init,
    BT_Phase2BlackWaveToRight_UpdateWave,
    BT_EndScanlinePhase2Transition,
};

#define tOffset data[1]
#define tTheta  data[2]

static void BT_Phase2BlackWaveToRight(u32 taskId)
{
    while (sBT_Phase2BlackWaveToRightFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2BlackWaveToRight_Init(struct Task *task)
{
    u32 i;

    BT_InitCtrlBlk();
    ScanlineEffect_Clear();
    
    sTransitionStructPtr->winIn = WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR;
    sTransitionStructPtr->winOut = 0;
    sTransitionStructPtr->win0H = DISPLAY_WIDTH;
    sTransitionStructPtr->win0V = DISPLAY_HEIGHT;
    
    for (i = 0; i < DISPLAY_HEIGHT; ++i)
        gScanlineEffectRegBuffers[1][i] = DISPLAY_WIDTH + 2;
    
    SetVBlankCallback(VBCB_BT_Phase2BlackWaveToRight);
    ++task->tState;
    return TRUE;
}

static bool32 BT_Phase2BlackWaveToRight_UpdateWave(struct Task *task)
{
    u32 i;
    u8 theta;
    u16 *winVal;
    bool32 nextFunc;

    sTransitionStructPtr->vblankDma = FALSE;
    
    winVal = gScanlineEffectRegBuffers[0];
    theta = task->tTheta;
    task->tTheta += 16;
    task->tOffset += 8;
    
    for (i = 0, nextFunc = TRUE; i < DISPLAY_HEIGHT; ++i, theta += 4, ++winVal)
    {
        s16 left = task->tOffset + Sin(theta, 40);
        if (left < 0)
            left = 0;
        if (left > DISPLAY_WIDTH)
            left = DISPLAY_WIDTH;
        
        *winVal = WIN_RANGE(left, DISPLAY_WIDTH + 1);
        if (left < DISPLAY_WIDTH)
            nextFunc = FALSE;
    }
    
    if (nextFunc)
        ++task->tState;
    
    ++sTransitionStructPtr->vblankDma;
    return FALSE;
}

static void VBCB_BT_Phase2BlackWaveToRight(void)
{
    DmaStop(0);
    BT_VBSyncOamAndPltt();
    
    if (sTransitionStructPtr->vblankDma)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], DISPLAY_HEIGHT * 2);
    
    SetGpuReg(REG_OFFSET_WININ, sTransitionStructPtr->winIn);
    SetGpuReg(REG_OFFSET_WINOUT, sTransitionStructPtr->winOut);
    SetGpuReg(REG_OFFSET_WIN0V, sTransitionStructPtr->win0V);
    
    DmaSet(0, gScanlineEffectRegBuffers[1], &REG_WIN0H, B_TRANS_DMA_FLAGS & ~(DMA_16BIT << 16));
}

#undef tOffset
#undef tTheta

//----------------------------
// B_TRANSITION_SLICED_SCREEN
//----------------------------

static bool32 BT_Phase2SlicedScreen_Init(struct Task *task);
static bool32 BT_Phase2SlicedScreen_UpdateOffsets(struct Task *task);
static void VBCB_BT_Phase2SlicedScreen(void);

static const TransitionStateFunc sBT_Phase2SlicedScreenFuncs[] =
{
    BT_Phase2SlicedScreen_Init,
    BT_Phase2SlicedScreen_UpdateOffsets,
    BT_EndScanlinePhase2Transition,
};

#define tSpeed data[1]
#define tAcc   data[2]
#define tJerk  data[3]

static void BT_Phase2SlicedScreen(u32 taskId)
{
    while (sBT_Phase2SlicedScreenFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2SlicedScreen_Init(struct Task *task)
{
    u32 i;

    BT_InitCtrlBlk();
    ScanlineEffect_Clear();
    
    task->tAcc = 256;
    task->tJerk = 1;
    
    sTransitionStructPtr->winIn = WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR;
    sTransitionStructPtr->winOut = 0;
    sTransitionStructPtr->win0V = DISPLAY_HEIGHT;
    
    for (i = 0; i < DISPLAY_HEIGHT; ++i)
    {
        gScanlineEffectRegBuffers[1][i] = sTransitionStructPtr->bg123HOfs;
        gScanlineEffectRegBuffers[1][DISPLAY_HEIGHT + i] = DISPLAY_WIDTH;
    }
    EnableInterrupts(INTR_FLAG_HBLANK);
    SetVBlankCallback(VBCB_BT_Phase2SlicedScreen);
    SetHBlankCallback(HBCB_BT_Phase2UpdateBgsHorizontalOffsets);
    
    ++task->tState;
    return TRUE;
}

static bool32 BT_Phase2SlicedScreen_UpdateOffsets(struct Task *task)
{
    u32 i;

    sTransitionStructPtr->vblankDma = FALSE;
    
    task->tSpeed += (task->tAcc >> 8);
    if (task->tSpeed > DISPLAY_WIDTH)
        task->tSpeed = DISPLAY_WIDTH;
    
    if (task->tAcc <= 0xFFF)
        task->tAcc += task->tJerk;
    
    if (task->tJerk < 128)
        task->tJerk <<= 1;
    
    for (i = 0; i < DISPLAY_HEIGHT; ++i)
    {
        u16 *ofsBuffer = &gScanlineEffectRegBuffers[0][i];
        u16 *win0HBuffer = &gScanlineEffectRegBuffers[0][i + DISPLAY_HEIGHT];
        
        if (i & 1)
        {
            *ofsBuffer = sTransitionStructPtr->bg123HOfs + task->tSpeed;
            *win0HBuffer = DISPLAY_WIDTH - task->tSpeed;
        }
        else
        {
            *ofsBuffer = sTransitionStructPtr->bg123HOfs - task->tSpeed;
            *win0HBuffer = WIN_RANGE(task->tSpeed, DISPLAY_WIDTH + 1);
        }
    }
    
    if (task->tSpeed >= DISPLAY_WIDTH)
        ++task->tState;
    
    ++sTransitionStructPtr->vblankDma;
    return FALSE;
}

static void VBCB_BT_Phase2SlicedScreen(void)
{
    DmaStop(0);
    BT_VBSyncOamAndPltt();
    
    SetGpuReg(REG_OFFSET_WININ, sTransitionStructPtr->winIn);
    SetGpuReg(REG_OFFSET_WINOUT, sTransitionStructPtr->winOut);
    SetGpuReg(REG_OFFSET_WIN0V, sTransitionStructPtr->win0V);
    
    if (sTransitionStructPtr->vblankDma)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], DISPLAY_HEIGHT * 4);
    
    DmaSet(0, &gScanlineEffectRegBuffers[1][DISPLAY_HEIGHT], &REG_WIN0H, B_TRANS_DMA_FLAGS);
}

#undef tSpeed
#undef tAcc
#undef tJerk

//-----------------------------------
// B_TRANSITION_WHITEFADE_IN_STRIPES
//-----------------------------------

static bool32 BT_Phase2WhiteFadeInStripes_Init(struct Task *task);
static bool32 BT_Phase2WhiteFadeInStripes_SetupSprites(struct Task *task);
static bool32 BT_Phase2WhiteFadeInStripes_IsWhiteFadeDone(struct Task *task);
static bool32 BT_Phase2WhiteFadeInStripes_Stop(struct Task *task);
static bool32 BT_Phase2WhiteFadeInStripes_IsDone(struct Task *task);
static void VBCB_BT_Phase2WhiteFadeInStripes1(void);
static void VBCB_BT_Phase2WhiteFadeInStripes2(void);
static void HBCB_BT_Phase2WhiteFadeInStripes(void);
static void SpriteCB_BT_Phase2WhiteFadeInStripes(struct Sprite *sprite);

#define NUM_WHITE_BARS 6

static const u16 sWhiteStripeDelay[NUM_WHITE_BARS] = { 0, 9, 15, 6, 12, 3 };

static const TransitionStateFunc sBT_Phase2WhiteFadeInStripesFuncs[] =
{
    BT_Phase2WhiteFadeInStripes_Init,
    BT_Phase2WhiteFadeInStripes_SetupSprites,
    BT_Phase2WhiteFadeInStripes_IsWhiteFadeDone,
    BT_Phase2WhiteFadeInStripes_Stop,
    BT_Phase2WhiteFadeInStripes_IsDone,
};

#define spBldyCounter data[0]
#define spFinished    data[1]
#define spAltDelay    data[2]
#define spDelay       data[5]
#define spLastSprite  data[6]

static void BT_Phase2WhiteFadeInStripes(u32 taskId)
{
    while (sBT_Phase2WhiteFadeInStripesFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2WhiteFadeInStripes_Init(struct Task *task)
{
    u32 i;

    BT_InitCtrlBlk();
    ScanlineEffect_Clear();
    
    sTransitionStructPtr->bldCnt = BLDCNT_TGT1_BG0 | BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ | BLDCNT_TGT1_BD | BLDCNT_EFFECT_LIGHTEN;
    sTransitionStructPtr->bldY = 0;
    sTransitionStructPtr->winIn = WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 | WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ;
    sTransitionStructPtr->winOut = WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ | WININ_WIN0_CLR;
    sTransitionStructPtr->win0V = DISPLAY_HEIGHT;
    
    for (i = 0; i < DISPLAY_HEIGHT; ++i)
    {
        gScanlineEffectRegBuffers[1][i] = 0;
        gScanlineEffectRegBuffers[1][i + DISPLAY_HEIGHT] = DISPLAY_WIDTH;
    }
    EnableInterrupts(INTR_FLAG_HBLANK);
    SetHBlankCallback(HBCB_BT_Phase2WhiteFadeInStripes);
    SetVBlankCallback(VBCB_BT_Phase2WhiteFadeInStripes1);
    
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2WhiteFadeInStripes_SetupSprites(struct Task *task)
{
    u32 i;
    s16 posY;
    s16 buffer[ARRAY_COUNT(sWhiteStripeDelay)];
    struct Sprite *sprite;

    memcpy(buffer, sWhiteStripeDelay, sizeof(sWhiteStripeDelay));
    
    for (i = 0, posY = 0; i < NUM_WHITE_BARS; ++i, posY += DISPLAY_HEIGHT / NUM_WHITE_BARS)
    {
        sprite = &gSprites[CreateInvisibleSprite(SpriteCB_BT_Phase2WhiteFadeInStripes)];
        sprite->x = DISPLAY_WIDTH;
        sprite->y = posY;
        sprite->spDelay = buffer[i];
    }
    ++sprite->spLastSprite;
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2WhiteFadeInStripes_IsWhiteFadeDone(struct Task *task)
{
    sTransitionStructPtr->vblankDma = FALSE;
    
    if (sTransitionStructPtr->counter >= NUM_WHITE_BARS)
    {
        BlendPalettes(PALETTES_ALL, 0x10, RGB_WHITE);
        ++task->tState;
    }
    return FALSE;
}

static bool32 BT_Phase2WhiteFadeInStripes_Stop(struct Task *task)
{
    sTransitionStructPtr->vblankDma = FALSE;
    
    DmaStop(0);
    SetVBlankCallback(NULL);
    SetHBlankCallback(NULL);
    
    sTransitionStructPtr->win0H = WIN_RANGE(0, DISPLAY_WIDTH);
    sTransitionStructPtr->bldY = 0;
    sTransitionStructPtr->bldCnt = BLDCNT_TGT1_BG0 | BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ | BLDCNT_TGT1_BD | BLDCNT_EFFECT_DARKEN;
    sTransitionStructPtr->winIn = WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ | WININ_WIN0_CLR;
    sTransitionStructPtr->counter = 0;
    
    SetVBlankCallback(VBCB_BT_Phase2WhiteFadeInStripes2);
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2WhiteFadeInStripes_IsDone(struct Task *task)
{
   sTransitionStructPtr->counter += 480;
   sTransitionStructPtr->bldY = sTransitionStructPtr->counter >> 8;
   
   if (sTransitionStructPtr->bldY > 16)
   {
       BT_BlendPalettesToBlack();
       BT_DestroyPhase2AnimTask(task);
   }
   return FALSE;
}

static void VBCB_BT_Phase2WhiteFadeInStripes1(void)
{
    DmaStop(0);
    BT_VBSyncOamAndPltt();
    
    SetGpuReg(REG_OFFSET_BLDCNT, sTransitionStructPtr->bldCnt);
    SetGpuReg(REG_OFFSET_WININ, sTransitionStructPtr->winIn);
    SetGpuReg(REG_OFFSET_WINOUT, sTransitionStructPtr->winOut);
    SetGpuReg(REG_OFFSET_WIN0V, sTransitionStructPtr->win0V);
    
    if (sTransitionStructPtr->vblankDma)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], DISPLAY_HEIGHT * 4);
    
    DmaSet(0, &gScanlineEffectRegBuffers[1][160], &REG_WIN0H, B_TRANS_DMA_FLAGS);
}

static void VBCB_BT_Phase2WhiteFadeInStripes2(void)
{
    BT_VBSyncOamAndPltt();
    SetGpuReg(REG_OFFSET_BLDY, sTransitionStructPtr->bldY);
    SetGpuReg(REG_OFFSET_BLDCNT, sTransitionStructPtr->bldCnt);
    SetGpuReg(REG_OFFSET_WININ, sTransitionStructPtr->winIn);
    SetGpuReg(REG_OFFSET_WINOUT, sTransitionStructPtr->winOut);
    SetGpuReg(REG_OFFSET_WIN0H, sTransitionStructPtr->win0H);
    SetGpuReg(REG_OFFSET_WIN0V, sTransitionStructPtr->win0V);
}

static void HBCB_BT_Phase2WhiteFadeInStripes(void)
{
    vu16 index = REG_VCOUNT;
    if (index == 227)
        index = 0;
    
    REG_BLDY = gScanlineEffectRegBuffers[1][index];
}

static void SpriteCB_BT_Phase2WhiteFadeInStripes(struct Sprite *sprite)
{
    if (sprite->spDelay)
    {
        --sprite->spDelay;
        
        if (sprite->spLastSprite)
            sTransitionStructPtr->vblankDma = TRUE;
    }
    else
    {
        u32 i;
        u16 *bldY = &gScanlineEffectRegBuffers[0][sprite->y];
        u16 *win0H = &gScanlineEffectRegBuffers[0][sprite->y + DISPLAY_HEIGHT];
        u32 stripeWidth = sprite->spLastSprite ? 0x19 : 0x1B;
        
        for (i = 0; i < stripeWidth; ++i)
        {
            bldY[i] = sprite->spBldyCounter >> 8;
            win0H[i] = (u8)(sprite->x);
        }
        
        if (sprite->x == 0 && sprite->spBldyCounter == 0x1000)
            sprite->spFinished = TRUE;
        
        sprite->x -= 24;
        if (sprite->x < 0)
            sprite->x = 0;
        
        sprite->spBldyCounter += 192;
        if (sprite->spBldyCounter > 0x1000)
            sprite->spBldyCounter = 0x1000;
        
        if (sprite->spLastSprite)
            sTransitionStructPtr->vblankDma = TRUE;
        
        if (sprite->spFinished)
        {
            if (sprite->spLastSprite == FALSE || (sTransitionStructPtr->counter > 4))
            {
                ++sTransitionStructPtr->counter;
                DestroySprite(sprite);
            }
        }
    }
}

#undef spBldyCounter
#undef spFinished
#undef spAltDelay
#undef spDelay
#undef spLastSprite

//---------------------------
// B_TRANSITION_GRID_SQUARES
//---------------------------

static bool32 BT_Phase2GridSquares_LoadGfx(struct Task *task);
static bool32 BT_Phase2GridSquares_UpdateTileset(struct Task *task);
static bool32 BT_Phase2GridSquares_IsDone(struct Task *task);

static const u32 sGridSquareTileset[] = INCBIN_U32("graphics/battle_transitions/grid_square_tileset.4bpp");

static const TransitionStateFunc sBT_Phase2GridSquaresFuncs[] =
{
    BT_Phase2GridSquares_LoadGfx,
    BT_Phase2GridSquares_UpdateTileset,
    BT_Phase2GridSquares_IsDone,
};

#define tDelay     data[1]
#define tWhichGrid data[2]

static void BT_Phase2GridSquares(u32 taskId)
{
    while (sBT_Phase2GridSquaresFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2GridSquares_LoadGfx(struct Task *task)
{
    u16 *tilemapAddr, *tilesetAddr;

    BT_GetBg0TilemapAndTilesetBase(&tilemapAddr, &tilesetAddr);
    
    CpuCopy16(sGridSquareTileset, tilesetAddr, 0x20);
    CpuFill16(0xF000, tilemapAddr, 0x800);
    
    LoadPalette(gSlidingPokeballBigPokeballPalette, 0xF0, 0x20);
    
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2GridSquares_UpdateTileset(struct Task *task)
{
    u16 *tilesetAddr;

    if (task->tDelay == 0)
    {
        BT_GetBg0TilesetBase(&tilesetAddr);
        
        task->tDelay = 3;
        ++task->tWhichGrid;
        
        CpuCopy16(sGridSquareTileset + (task->tWhichGrid * 8), tilesetAddr, 0x20);
        
        if (task->tWhichGrid > 0xD)
        {
            ++task->tState;
            task->tDelay = 16;
        }
    }
    --task->tDelay;
    return FALSE;
}

static bool32 BT_Phase2GridSquares_IsDone(struct Task *task)
{
    if (--task->tDelay == 0)
    {
        BT_BlendPalettesToBlack();
        BT_DestroyPhase2AnimTask(task);
    }
    return FALSE;
}

#undef tDelay
#undef tWhichGrid

//----------------------------
// B_TRANSITION_BLACK_DOODLES
//----------------------------

static bool32 BT_Phase2BlackDoodles_Init(struct Task *task);
static bool32 BT_Phase2BlackDoodles_InitSingleBrush(struct Task *task);
static bool32 BT_Phase2BlackDoodles_DrawSingleBrush(struct Task *task);
static bool32 BT_Phase2BlackDoodles_IsDone(struct Task *task);
static bool32 BT_Phase2BlackDoodles_NextBrush(struct Task *task);
static void VBCB_BT_Phase2BlackDoodles(void);

static const s16 sBlackDoodlesSegments[][5] =
{
    { 0x38, 0x00, 0x00, 0xA0, 0 },
    { 0x68, 0xA0, 0xF0, 0x58, 1 },
    { 0xF0, 0x48, 0x38, 0x00, 1 },
    { 0x00, 0x20, 0x90, 0xA0, 0 },
    { 0x90, 0xA0, 0xB8, 0x00, 1 },
    { 0x38, 0x00, 0xA8, 0xA0, 0 },
    { 0xA8, 0xA0, 0x30, 0x00, 1 },
};

static const s16 sBlackDoodlesDelay[] = { 1, 1, 1, 1, 1, 1, 0 };

static const TransitionStateFunc sBT_Phase2BlackDoodlesFuncs[] =
{
    BT_Phase2BlackDoodles_Init,
    BT_Phase2BlackDoodles_InitSingleBrush,
    BT_Phase2BlackDoodles_DrawSingleBrush,
    BT_Phase2BlackDoodles_IsDone,
    BT_Phase2BlackDoodles_NextBrush,
};

#define tWhichBrush data[1]
#define tWhichSide  data[2]
#define tDelay      data[3]

#define trCurrentPtX data[2]
#define trCurrentPtY data[3]

static void BT_Phase2BlackDoodles(u32 taskId)
{
    while (sBT_Phase2BlackDoodlesFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2BlackDoodles_Init(struct Task *task)
{
    u32 i;

    BT_InitCtrlBlk();
    ScanlineEffect_Clear();
    
    sTransitionStructPtr->winIn = WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR;
    sTransitionStructPtr->winOut = 0;
    sTransitionStructPtr->win0V = WIN_RANGE(0, DISPLAY_HEIGHT);
    
    for (i = 0; i < DISPLAY_HEIGHT; ++i)
        gScanlineEffectRegBuffers[0][i] = WIN_RANGE(0, DISPLAY_WIDTH);
    
    CpuCopy16(gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], DISPLAY_HEIGHT * 2);
    SetVBlankCallback(VBCB_BT_Phase2BlackDoodles);
    
    ++task->tState;
    return TRUE;
}

static bool32 BT_Phase2BlackDoodles_InitSingleBrush(struct Task *task)
{
    BT_DiagonalSegment_InitParams(sTransitionStructPtr->data, sBlackDoodlesSegments[task->tWhichBrush][0], sBlackDoodlesSegments[task->tWhichBrush][1], sBlackDoodlesSegments[task->tWhichBrush][2], sBlackDoodlesSegments[task->tWhichBrush][3], 1, 1);
    task->tWhichSide = sBlackDoodlesSegments[task->tWhichBrush][4];
    ++task->tState;
    return TRUE;
}

static bool32 BT_Phase2BlackDoodles_DrawSingleBrush(struct Task *task)
{
    u32 i;
    bool32 nextFunc;

    sTransitionStructPtr->vblankDma = FALSE;
    
    for (i = 0, nextFunc = FALSE; i < 16; ++i)
    {
        s16 left = gScanlineEffectRegBuffers[0][sTransitionStructPtr->trCurrentPtY] >> 8;
        s16 right = gScanlineEffectRegBuffers[0][sTransitionStructPtr->trCurrentPtY] & 0xFF;
        
        if (task->tWhichSide == 0)
        {
            if (left < sTransitionStructPtr->trCurrentPtX)
                left = sTransitionStructPtr->trCurrentPtX;
            if (left > right)
                left = right;
        }
        else
        {
            if (right > sTransitionStructPtr->trCurrentPtX)
                right = sTransitionStructPtr->trCurrentPtX;
            if (right <= left)
                right = left;
        }
        gScanlineEffectRegBuffers[0][sTransitionStructPtr->trCurrentPtY] = WIN_RANGE2(left, right);
        
        if (nextFunc)
        {
            ++task->tState;
            break;
        }
        else
            nextFunc = BT_DiagonalSegment_ComputePointOnSegment(sTransitionStructPtr->data, TRUE, TRUE);
    }
    ++sTransitionStructPtr->vblankDma;
    return FALSE;
}

static bool32 BT_Phase2BlackDoodles_IsDone(struct Task *task)
{
    if (++task->tWhichBrush < 7)
    {
        ++task->tState;
        task->tDelay = sBlackDoodlesDelay[task->tWhichBrush - 1];
        return TRUE;
    }
    else
    {
        BT_EndScanlinePhase2Transition(task);
        return FALSE;
    }
}

static bool32 BT_Phase2BlackDoodles_NextBrush(struct Task *task)
{
    if (--task->tDelay == 0)
    {
        task->tState = 1;
        return TRUE;
    }
    else
        return FALSE;
}

static void VBCB_BT_Phase2BlackDoodles(void)
{
    DmaStop(0);
    BT_VBSyncOamAndPltt();
    
    if (sTransitionStructPtr->vblankDma)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], DISPLAY_HEIGHT * 2);
    
    SetGpuReg(REG_OFFSET_WININ, sTransitionStructPtr->winIn);
    SetGpuReg(REG_OFFSET_WINOUT, sTransitionStructPtr->winOut);
    SetGpuReg(REG_OFFSET_WIN0V, sTransitionStructPtr->win0V);
    SetGpuReg(REG_OFFSET_WIN0H, gScanlineEffectRegBuffers[1][0]);
    
    DmaSet(0, gScanlineEffectRegBuffers[1], &REG_WIN0H, B_TRANS_DMA_FLAGS);
}

#undef tWhichBrush
#undef tWhichSide
#undef tDelay

#undef trCurrentPtX
#undef trCurrentPtY

//----------------------
// B_TRANSITION_MUGSHOT
//----------------------

static bool32 BT_Phase2Mugshot_Init(struct Task *task);
static bool32 BT_Phase2Mugshot_LoadGfx(struct Task *task);
static bool32 BT_Phase2Mugshot_VsBarsSlideIn(struct Task *task);
static bool32 BT_Phase2Mugshot_StartSpriteSlide(struct Task *task);
static bool32 BT_Phase2Mugshot_WaitForOpponentInPlace(struct Task *task);
static bool32 BT_Phase2Mugshot_WaitForPlayerInPlace(struct Task *task);
static bool32 BT_Phase2Mugshot_ExpandWhiteBand(struct Task *task);
static bool32 BT_Phase2Mugshot_StartBlackFade(struct Task *task);
static bool32 BT_Phase2Mugshot_WaitForBlackFade(struct Task *task);
static void VBCB_BT_Phase2Mugshot1_Slide(void);
static void VBCB_BT_Phase2Mugshot2_WhiteFade(void);
static void HBCB_BT_Phase2Mugshot(void);
static void BT_Phase2Mugshots_CreateSprites(struct Task *task);
static void SpriteCB_BT_Phase2Mugshots(struct Sprite *sprite);
static void BT_StartSpriteSlide(u32 spriteId);
static bool32 BT_IsSpriteSlideFinished(u32 spriteId);
static bool32 BT_Phase2MugshotsSpriteFuncs_Wait(struct Sprite *sprite);
static bool32 BT_Phase2MugshotsSpriteFuncs_InitParams(struct Sprite *sprite);
static bool32 BT_Phase2MugshotsSpriteFuncs_SlideSpriteIn(struct Sprite *sprite);
static bool32 BT_Phase2MugshotsSpriteFuncs_DecelerateSprite(struct Sprite *sprite);
static bool32 BT_Phase2MugshotsSpriteFuncs_DecelerateSprite2(struct Sprite *sprite);

static const u32 sVsBarTileset[] = INCBIN_U32("graphics/battle_transitions/vsbar_tileset.4bpp");
static const u16 sVsBarTilemap[] = INCBIN_U16("graphics/battle_transitions/vsbar_tilemap.bin");

static const u16 sVsBarPurplePalette[] = INCBIN_U16("graphics/battle_transitions/purple_bg.gbapal");
static const u16 sVsBarGreenPalette[] = INCBIN_U16("graphics/battle_transitions/green_bg.gbapal");
static const u16 sVsBarPinkPalette[] = INCBIN_U16("graphics/battle_transitions/pink_bg.gbapal");
static const u16 sVsBarBluePalette[] = INCBIN_U16("graphics/battle_transitions/blue_bg.gbapal");
static const u16 sVsBarYellowPalette[] = INCBIN_U16("graphics/battle_transitions/yellow_bg.gbapal");
static const u16 sVsBarMalePlayerPalette[] = INCBIN_U16("graphics/battle_transitions/male_player_bg.gbapal");
static const u16 sVsBarFemalePlayerPalette[] = INCBIN_U16("graphics/battle_transitions/female_player_bg.gbapal");

static const s16 sMugShotSlideVelocity[] = { 12, -12 };
static const s16 sMugShotSlideDeceleration[] = { -1, 1 };

static const struct OamData sMugShotOamAttributes = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0x000,
    .priority = 0,
    .paletteNum = 0x0,
    .affineParam = 0
};

static const u16 *const sVsBarOpponentPalettes[MUGSHOT_COLOR_COUNT] =
{
    [MUGSHOT_COLOR_PURPLE] = sVsBarPurplePalette,
    [MUGSHOT_COLOR_GREEN]  = sVsBarGreenPalette,
    [MUGSHOT_COLOR_PINK]   = sVsBarPinkPalette,
    [MUGSHOT_COLOR_BUE]    = sVsBarBluePalette,
    [MUGSHOT_COLOR_YELLOW] = sVsBarYellowPalette,
};

static const u16 *const sVsBarPlayerPalettes[] =
{
    [MALE]   = sVsBarMalePlayerPalette,
    [FEMALE] = sVsBarFemalePlayerPalette,
};

static const TransitionStateFunc sBT_Phase2MugshotFuncs[] =
{
    BT_Phase2Mugshot_Init,
    BT_Phase2Mugshot_LoadGfx,
    BT_Phase2Mugshot_VsBarsSlideIn,
    BT_Phase2Mugshot_StartSpriteSlide,
    BT_Phase2Mugshot_WaitForOpponentInPlace,
    BT_Phase2Mugshot_WaitForPlayerInPlace,
    BT_Phase2Mugshot_ExpandWhiteBand,
    BT_Phase2Mugshot_StartBlackFade,
    BT_Phase2Mugshot_WaitForBlackFade,
    BT_EndScanlinePhase2Transition,
};

static const TransitionSpriteCallback sBT_Phase2MugshotSpriteFuncs[] =
{
    BT_Phase2MugshotsSpriteFuncs_Wait,
    BT_Phase2MugshotsSpriteFuncs_InitParams,
    BT_Phase2MugshotsSpriteFuncs_SlideSpriteIn,
    BT_Phase2MugshotsSpriteFuncs_DecelerateSprite,
    BT_Phase2MugshotsSpriteFuncs_Wait,
    BT_Phase2MugshotsSpriteFuncs_DecelerateSprite2, // not used
    BT_Phase2MugshotsSpriteFuncs_Wait,
};

#define tTheta            data[1]
#define tbg0HOfsOpponent  data[2]
#define tbg0HOfsPlayer    data[3]
#define tCounter          data[3]
#define tHalfBandwidth    data[4]
#define tOpponentSpriteId data[13]
#define tPlayerSpriteId   data[14]

#define spState            data[0]
#define spSpeed            data[1]
#define spAbsAcc           data[2]
#define sphasSlideFinished data[6]
#define spOpponentOrPlayer data[7]

static void BT_Phase2Mugshot(u32 taskId)
{
    while (sBT_Phase2MugshotFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2Mugshot_Init(struct Task *task)
{
    u32 i;

    BT_InitCtrlBlk();
    ScanlineEffect_Clear();
    BT_Phase2Mugshots_CreateSprites(task);
    
    task->tTheta = 0;
    task->tbg0HOfsOpponent = 1;
    task->tbg0HOfsPlayer = 239;
    
    sTransitionStructPtr->winIn = WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR;
    sTransitionStructPtr->winOut = WININ_WIN0_BG1 | WININ_WIN0_BG2 | WININ_WIN0_BG3 | WININ_WIN0_OBJ | WININ_WIN0_CLR;
    sTransitionStructPtr->win0V = WIN_RANGE(0, DISPLAY_HEIGHT);
    
    for (i = 0; i < DISPLAY_HEIGHT; ++i)
        gScanlineEffectRegBuffers[1][i] = WIN_RANGE(DISPLAY_WIDTH, DISPLAY_WIDTH + 1);
    
    SetVBlankCallback(VBCB_BT_Phase2Mugshot1_Slide);
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2Mugshot_LoadGfx(struct Task *task)
{
    u32 i, j, mugshotColor;
    u16 *tilemapAddr, *tilesetAddr;
    const u16 *mugshotsMap = sVsBarTilemap;
    
    BT_GetBg0TilemapAndTilesetBase(&tilemapAddr, &tilesetAddr);
    CpuCopy16(sVsBarTileset, tilesetAddr, 0x1E0);
    
    mugshotColor = gTrainers[gTrainerBattleOpponent_A].mugshotColor;
    if (mugshotColor >= MUGSHOT_COLOR_COUNT)
        mugshotColor = MUGSHOT_COLOR_PURPLE;
    
    LoadPalette(sVsBarOpponentPalettes[mugshotColor], 0xF0, 0x20);
    LoadPalette(sVsBarPlayerPalettes[gSaveBlock2Ptr->playerGender], 0xFA, 0xC);
    
    for (i = 0; i < 20; ++i)
    {
        for (j = 0; j < 32; ++j, ++mugshotsMap)
            tilemapAddr[i * 32 + j] = *mugshotsMap | 0xF000; // use palette #15
    }
    EnableInterrupts(INTR_FLAG_HBLANK);
    SetHBlankCallback(HBCB_BT_Phase2Mugshot);
    
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2Mugshot_VsBarsSlideIn(struct Task *task)
{
    u32 i;
    u8 theta;
    u16 *winVal;
    s16 value;
    s32 mergedBg0hOfs;

    sTransitionStructPtr->vblankDma = FALSE;

    winVal = gScanlineEffectRegBuffers[0];
    theta = task->tTheta;
    task->tTheta += 0x10;
    
    for (i = 0; i < DISPLAY_HEIGHT / 2; ++i, ++winVal, theta += 0x10)
    {
        value = task->tbg0HOfsOpponent + Sin(theta, 0x10);
        if (value < 0)
            value = 1;
        if (value > DISPLAY_WIDTH)
            value = DISPLAY_WIDTH;
        *winVal = value;
    }
    
    for (; i < DISPLAY_HEIGHT; ++i, ++winVal, theta += 0x10)
    {
        value = task->tCounter - Sin(theta, 0x10);
        if (value < 0)
            value = 0;
        if (value > DISPLAY_WIDTH - 1)
            value = DISPLAY_WIDTH - 1;
        *winVal = (value << 8) | (DISPLAY_WIDTH);
    }
    
    task->tbg0HOfsOpponent += 8;
    if (task->tbg0HOfsOpponent > DISPLAY_WIDTH)
        task->tbg0HOfsOpponent = DISPLAY_WIDTH;
    
    task->tCounter -= 8;
    if (task->tCounter < 0)
        task->tCounter = 0;
    
    if (task->tbg0HOfsOpponent == DISPLAY_WIDTH && task->tCounter == 0)
        ++task->tState;
    
    sTransitionStructPtr->bg0HOfsOpponent -= 8;
    sTransitionStructPtr->bg0HOfsPlayer += 8;
    ++sTransitionStructPtr->vblankDma;
    
    return FALSE;
}

static bool32 BT_Phase2Mugshot_StartSpriteSlide(struct Task *task)
{
    u32 i;
    u16 *winVal;

    sTransitionStructPtr->vblankDma = FALSE;
    
    for (i = 0, winVal = gScanlineEffectRegBuffers[0]; i < DISPLAY_HEIGHT; ++i, ++winVal)
        *winVal = DISPLAY_WIDTH;
    
    ++task->tState;
    task->tTheta = 0;
    task->tbg0HOfsOpponent = 0;
    task->tbg0HOfsPlayer = 0;
    
    sTransitionStructPtr->bg0HOfsOpponent -= 8;
    sTransitionStructPtr->bg0HOfsPlayer += 8;
    
    gSprites[task->tOpponentSpriteId].spOpponentOrPlayer = FALSE;
    gSprites[task->tPlayerSpriteId].spOpponentOrPlayer = TRUE;

    BT_StartSpriteSlide(task->tOpponentSpriteId);
    
    PlaySE(SE_MUGSHOT);
    
    ++sTransitionStructPtr->vblankDma;
    return FALSE;
}

static bool32 BT_Phase2Mugshot_WaitForOpponentInPlace(struct Task *task)
{
    sTransitionStructPtr->bg0HOfsOpponent -= 8;
    sTransitionStructPtr->bg0HOfsPlayer += 8;
    
    if (BT_IsSpriteSlideFinished(task->tOpponentSpriteId))
    {
        ++task->tState;
        BT_StartSpriteSlide(task->tPlayerSpriteId);
    }
    return FALSE;
}

static bool32 BT_Phase2Mugshot_WaitForPlayerInPlace(struct Task *task)
{
    sTransitionStructPtr->bg0HOfsOpponent -= 8;
    sTransitionStructPtr->bg0HOfsPlayer += 8;
    
    if (BT_IsSpriteSlideFinished(task->tPlayerSpriteId))
    {
        sTransitionStructPtr->vblankDma = FALSE;
        
        SetVBlankCallback(NULL);
        DmaStop(0);
        
        memset(gScanlineEffectRegBuffers[0], 0, DISPLAY_HEIGHT * 2);
        memset(gScanlineEffectRegBuffers[1], 0, DISPLAY_HEIGHT * 2);
        
        SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(0, DISPLAY_WIDTH));
        SetGpuReg(REG_OFFSET_BLDY, 0);
        
        ++task->tState;
        task->tCounter = 0;
        task->tHalfBandwidth = 0;
        
        sTransitionStructPtr->bldCnt = BLDCNT_TGT1_BG0 | BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ | BLDCNT_TGT1_BD | BLDCNT_EFFECT_LIGHTEN;
        SetVBlankCallback(VBCB_BT_Phase2Mugshot2_WhiteFade);
    }
    return FALSE;
}

static bool32 BT_Phase2Mugshot_ExpandWhiteBand(struct Task *task)
{
    bool32 nextFunc = TRUE;

    sTransitionStructPtr->vblankDma = FALSE;
    sTransitionStructPtr->bg0HOfsOpponent -= 8;
    sTransitionStructPtr->bg0HOfsPlayer += 8;
    
    if (task->tHalfBandwidth < DISPLAY_HEIGHT / 2)
        task->tHalfBandwidth += 2;
    
    if (task->tHalfBandwidth > DISPLAY_HEIGHT / 2)
        task->tHalfBandwidth = DISPLAY_HEIGHT / 2;
    
    if (++task->tCounter & 1)
    {
        s16 i;

        for (i = 0, nextFunc = FALSE; i <= task->tHalfBandwidth; ++i)
        {
            s16 y1 = DISPLAY_HEIGHT / 2 - i;
            s16 y2 = DISPLAY_HEIGHT / 2 + i;

            if (gScanlineEffectRegBuffers[0][y1] <= 15)
            {
                nextFunc = TRUE;
                ++gScanlineEffectRegBuffers[0][y1];
            }
            if (gScanlineEffectRegBuffers[0][y2] <= 15)
            {
                nextFunc = TRUE;
                ++gScanlineEffectRegBuffers[0][y2];
            }
        }
    }
    
    if (task->tHalfBandwidth == DISPLAY_HEIGHT / 2 && !nextFunc)
        ++task->tState;
    
    ++sTransitionStructPtr->vblankDma;
    return FALSE;
}

static bool32 BT_Phase2Mugshot_StartBlackFade(struct Task *task)
{
    sTransitionStructPtr->vblankDma = FALSE;
    BlendPalettes(PALETTES_ALL, 0x10, RGB_WHITE);
    sTransitionStructPtr->bldCnt = BLDCNT_TGT1_BG0 | BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ | BLDCNT_TGT1_BD | BLDCNT_EFFECT_DARKEN;
    task->tCounter = 0;
    ++task->tState;
    return TRUE;
}

static bool32 BT_Phase2Mugshot_WaitForBlackFade(struct Task *task)
{
    sTransitionStructPtr->vblankDma = FALSE;
    ++task->tCounter;
    
    memset(gScanlineEffectRegBuffers[0], task->tCounter, DISPLAY_HEIGHT * 2);
    
    if (task->tCounter > 15)
        ++task->tState;
    
    ++sTransitionStructPtr->vblankDma;
    return FALSE;
}

static u32 CreateTrainerSprite(u32 trainerSpriteID, s16 x, s16 y, u32 subpriority)
{
    struct SpriteTemplate spriteTemplate;
    
    LoadCompressedSpritePalette(&gTrainerFrontPicTable[trainerSpriteID].palette);
    LoadCompressedSpriteSheet(&gTrainerFrontPicTable[trainerSpriteID].pic);
    
    spriteTemplate.tileTag = gTrainerFrontPicTable[trainerSpriteID].pic.tag;
    spriteTemplate.paletteTag = gTrainerFrontPicTable[trainerSpriteID].palette.tag;
    spriteTemplate.oam = &sMugShotOamAttributes;
    spriteTemplate.anims = gDummySpriteAnimTable;
    spriteTemplate.images = NULL;
    spriteTemplate.affineAnims = gDummySpriteAffineAnimTable;
    spriteTemplate.callback = SpriteCallbackDummy;
    
    return CreateSprite(&spriteTemplate, x, y, subpriority);
}

static void BT_Phase2Mugshots_CreateSprites(struct Task *task)
{
    struct Sprite *opponentSprite, *playerSprite;
    s16 opponentRotationScales;
    u32 trainerPicId = gTrainers[gTrainerBattleOpponent_A].trainerPic;

    gReservedSpritePaletteCount = 10;
    task->tOpponentSpriteId = CreateTrainerSprite(trainerPicId, gTrainerFrontPicTable[trainerPicId].mugshotCoords.x - 32, gTrainerFrontPicTable[trainerPicId].mugshotCoords.y + 42, 0);
    task->tPlayerSpriteId = CreateTrainerSprite(PlayerGenderToFrontTrainerPicId_Debug(gSaveBlock2Ptr->playerGender, TRUE), 272, 106, 0);
    gReservedSpritePaletteCount = 12;
    
    opponentSprite = &gSprites[task->tOpponentSpriteId];
    playerSprite = &gSprites[task->tPlayerSpriteId];
    opponentSprite->callback = SpriteCB_BT_Phase2Mugshots;
    playerSprite->callback = SpriteCB_BT_Phase2Mugshots;
    opponentSprite->oam.affineMode = 3;
    playerSprite->oam.affineMode = 3;
    opponentSprite->oam.matrixNum = AllocOamMatrix();
    playerSprite->oam.matrixNum = AllocOamMatrix();
    opponentSprite->oam.shape = 1;
    playerSprite->oam.shape = 1;
    opponentSprite->oam.size = 3;
    playerSprite->oam.size = 3;
    CalcCenterToCornerVec(opponentSprite, 1, 3, 3);
    CalcCenterToCornerVec(playerSprite, 1, 3, 3);
    
    opponentRotationScales = gTrainerFrontPicTable[trainerPicId].mugshotRotation;
    SetOamMatrixRotationScaling(opponentSprite->oam.matrixNum, opponentRotationScales, opponentRotationScales, 0);
    SetOamMatrixRotationScaling(playerSprite->oam.matrixNum, -512, 512, 0);
}

static void BT_StartSpriteSlide(u32 spriteId)
{
    ++gSprites[spriteId].spState;
}

static bool32 BT_IsSpriteSlideFinished(u32 spriteId)
{
    return gSprites[spriteId].sphasSlideFinished;
}

static void SpriteCB_BT_Phase2Mugshots(struct Sprite *sprite)
{
    while (sBT_Phase2MugshotSpriteFuncs[sprite->spState](sprite));
}

static bool32 BT_Phase2MugshotsSpriteFuncs_Wait(struct Sprite *sprite)
{
    return FALSE;
}

static bool32 BT_Phase2MugshotsSpriteFuncs_InitParams(struct Sprite *sprite)
{
    s16 arr0[2];
    s16 arr1[2];

    memcpy(arr0, sMugShotSlideVelocity, sizeof(sMugShotSlideVelocity));
    memcpy(arr1, sMugShotSlideDeceleration, sizeof(sMugShotSlideDeceleration));
    
    ++sprite->spState;
    sprite->spSpeed = arr0[sprite->spOpponentOrPlayer];
    sprite->spAbsAcc = arr1[sprite->spOpponentOrPlayer];
    
    return TRUE;
}

static bool32 BT_Phase2MugshotsSpriteFuncs_SlideSpriteIn(struct Sprite *sprite)
{
    sprite->x += sprite->spSpeed;
    
    if (sprite->spOpponentOrPlayer && sprite->x < 133)
        ++sprite->spState;
    else if (!sprite->spOpponentOrPlayer && sprite->x > 103)
        ++sprite->spState;
    
    return FALSE;
}

static bool32 BT_Phase2MugshotsSpriteFuncs_DecelerateSprite(struct Sprite *sprite)
{
    sprite->spSpeed += sprite->spAbsAcc;
    sprite->x += sprite->spSpeed;
    
    if (sprite->spSpeed == 0)
    {
        ++sprite->spState;
        sprite->spAbsAcc = -sprite->spAbsAcc;
        sprite->sphasSlideFinished = TRUE;
    }
    return FALSE;
}

// not used
static bool32 BT_Phase2MugshotsSpriteFuncs_DecelerateSprite2(struct Sprite *sprite)
{
    sprite->spSpeed += sprite->spAbsAcc;
    sprite->x += sprite->spSpeed;
    
    if (sprite->x < -31 || sprite->x > 271)
        ++sprite->spState;
    
    return FALSE;
}

static void VBCB_BT_Phase2Mugshot1_Slide(void)
{
    DmaStop(0);
    BT_VBSyncOamAndPltt();
    
    if (sTransitionStructPtr->vblankDma)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], DISPLAY_HEIGHT * 2);
    
    SetGpuReg(REG_OFFSET_BG0VOFS, sTransitionStructPtr->bg0VOfs);
    SetGpuReg(REG_OFFSET_WININ, sTransitionStructPtr->winIn);
    SetGpuReg(REG_OFFSET_WINOUT, sTransitionStructPtr->winOut);
    SetGpuReg(REG_OFFSET_WIN0V, sTransitionStructPtr->win0V);
    
    DmaSet(0, gScanlineEffectRegBuffers[1], &REG_WIN0H, B_TRANS_DMA_FLAGS & ~(DMA_16BIT << 16));
}

static void VBCB_BT_Phase2Mugshot2_WhiteFade(void)
{
    DmaStop(0);
    BT_VBSyncOamAndPltt();
    
    if (sTransitionStructPtr->vblankDma)
        DmaCopy16(3, gScanlineEffectRegBuffers[0], gScanlineEffectRegBuffers[1], DISPLAY_HEIGHT * 2);
    
    SetGpuReg(REG_OFFSET_BLDCNT, sTransitionStructPtr->bldCnt);
    DmaSet(0, gScanlineEffectRegBuffers[1], &REG_BLDY, B_TRANS_DMA_FLAGS & ~(DMA_16BIT << 16));
}

static void HBCB_BT_Phase2Mugshot(void)
{
    REG_BG0HOFS = REG_VCOUNT < DISPLAY_HEIGHT / 2 ? sTransitionStructPtr->bg0HOfsOpponent : sTransitionStructPtr->bg0HOfsPlayer;
}

#undef tTheta
#undef tbg0HOfsOpponent
#undef tbg0HOfsPlayer
#undef tCounter
#undef tHalfBandwidth
#undef tOpponentSpriteId
#undef tPlayerSpriteId

#undef spState
#undef spSpeed
#undef spAbsAcc
#undef sphasSlideFinished
#undef spOpponentOrPlayer

//------------------------------------
// B_TRANSITION_ANTI_CLOCKWISE_SPIRAL
//------------------------------------

static bool32 BT_Phase2AntiClockwiseSpiral_Init(struct Task *task);
static bool32 BT_Phase2AntiClockwiseSpiral_Update(struct Task *task);
static void VBCB_BT_Phase2AntiClockwiseBlackFade(void);
static void BT_AntiClockwiseSpiral_DoUpdateFrame(s16 initRadius, s16 deltaAngleMax, u32 offsetMaybe);

static const s16 gUnknown_83FA444[] =
{
    0x0, 0x26E,
    0x100, 0x69,
    0x0, -0x69,
    -0x100, -0x266E,
    0x0, 0x26E,
    0x100, 0x69,
    0x0, -0x69,
    -0x100, -0x266E,
};

static const TransitionStateFunc sBT_Phase2AntiClockwiseSpiralFuncs[] =
{
    BT_Phase2AntiClockwiseSpiral_Init,
    BT_Phase2AntiClockwiseSpiral_Update,
};

static void BT_Phase2AntiClockwiseSpiral(u32 taskId)
{
    while (sBT_Phase2AntiClockwiseSpiralFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2AntiClockwiseSpiral_Init(struct Task *task)
{
    BT_InitCtrlBlk();
    ScanlineEffect_Clear();
    
    sTransitionStructPtr->winIn = 0;
    sTransitionStructPtr->winOut = WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR;
    sTransitionStructPtr->win0H = WIN_RANGE(DISPLAY_WIDTH / 2, DISPLAY_WIDTH / 2);
    sTransitionStructPtr->win0V = WIN_RANGE(48, DISPLAY_HEIGHT - 48);
    sTransitionStructPtr->win1V = WIN_RANGE(16, DISPLAY_HEIGHT - 16);
    sTransitionStructPtr->counter = 0;
    
    BT_AntiClockwiseSpiral_DoUpdateFrame(0, 0, 0);
    BT_AntiClockwiseSpiral_DoUpdateFrame(0, 0, 1);
    
    DmaCopy16(3, gScanlineEffectRegBuffers[1], gScanlineEffectRegBuffers[0], DISPLAY_HEIGHT * 4);
    SetVBlankCallback(VBCB_BT_Phase2AntiClockwiseBlackFade);
    
    ++task->tState;
    task->data[1] = 0;
    task->data[2] = 0;
    
    return FALSE;
}

static bool32 BT_Phase2AntiClockwiseSpiral_Update(struct Task *task)
{
    s16 win_top, win_bottom;

    BT_AntiClockwiseSpiral_DoUpdateFrame(task->data[2], task->data[1], 1);
    sTransitionStructPtr->vblankDma |= TRUE;
    
    if (++task->data[1] == ARRAY_COUNT(gUnknown_83FA444) + 1)
    {
        BT_AntiClockwiseSpiral_DoUpdateFrame(task->data[2], 16, 0);
        
        win_top = 48 - task->data[2];
        if (win_top < 0)
            win_top = 0;
        
        win_bottom = task->data[2] + 112;
        if (win_bottom > 255)
            win_bottom = 255;
        
        sTransitionStructPtr->win0V = win_top | win_bottom;
        task->data[2] += 32;
        task->data[1] = 0;
        
        BT_AntiClockwiseSpiral_DoUpdateFrame(task->data[2], 0, 1);
        
        win_top = 48 - task->data[2];
        if (win_top < 0)
            win_top = 0;
        
        win_bottom = task->data[2] + 112;
        if (win_bottom > 255)
            win_bottom = 255;
        
        sTransitionStructPtr->win1V = win_top | win_bottom;
        sTransitionStructPtr->vblankDma |= TRUE;
        
        if (task->data[2] >= DISPLAY_HEIGHT)
        {
            sTransitionStructPtr->counter = TRUE;
            BT_BlendPalettesToBlack();
            BT_DestroyPhase2AnimTask(task);
        }
    }
    return FALSE;
}

static void BT_AntiClockwiseSpiral_DoUpdateFrame(s16 initRadius, s16 deltaAngleMax, u32 offsetMaybe)
{
    u8 theta = 0;
    s16 i, amplitude1, amplitude2;
    s16 y1, x1, y2, x2;

    for (i = DISPLAY_HEIGHT * 2; i < DISPLAY_HEIGHT * 6; ++i)
        gScanlineEffectRegBuffers[1][i] = DISPLAY_WIDTH / 2;

    for (i = 0; i < (deltaAngleMax * 16); ++i, ++theta)
    {
        amplitude1 = initRadius + (theta >> 3);
        
        if ((theta >> 3) != ((theta + 1) >> 3))
            amplitude2 = amplitude1 + 1;
        else
            amplitude2 = amplitude1;

        y1 = (DISPLAY_HEIGHT / 2) - Sin(theta, amplitude1);
        x1 = Cos(theta, amplitude1) + (DISPLAY_WIDTH / 2);
        y2 = (DISPLAY_HEIGHT / 2) - Sin(theta + 1, amplitude2);
        x2 = Cos(theta + 1, amplitude2) + (DISPLAY_WIDTH / 2);

        if (y1 < 0 && y2 < 0)
            continue;
        if (y1 > DISPLAY_HEIGHT - 1 && y2 > DISPLAY_HEIGHT - 1)
            continue;

        if (y1 < 0)
            y1 = 0;
        if (y1 > DISPLAY_HEIGHT - 1)
            y1 = DISPLAY_HEIGHT - 1;
        if (x1 < 0)
            x1 = 0;
        if (x1 > 255)
            x1 = 255;
        if (y2 < 0)
            y2 = 0;
        if (y2 > DISPLAY_HEIGHT - 1)
            y2 = DISPLAY_HEIGHT - 1;
        if (x2 < 0)
            x2 = 0;
        if (x2 > 255)
            x2 = 255;

        y2 -= y1;

        if (theta >= 64 && theta < 64 * 3)
        {
            gScanlineEffectRegBuffers[1][y1 + (DISPLAY_HEIGHT * 2)] = x1;

            if (y2 == 0)
                continue;

            x2 -= x1;
            if (x2 < -1 && x1 > 1)
                --x1;
            else if (x2 > 1 && x1 < 255)
                ++x1;

            if (y2 < 0)
            {
                for (; y2 < 0; y2++)
                    gScanlineEffectRegBuffers[1][y1 + y2 + (DISPLAY_HEIGHT * 2)] = x1;
            }
            else
            {
                for (; y2 > 0; y2--)
                    gScanlineEffectRegBuffers[1][y1 + y2 + (DISPLAY_HEIGHT * 2)] = x1;
            }
        }
        else
        {
            gScanlineEffectRegBuffers[1][y1 + (DISPLAY_HEIGHT * 3)] = x1;

            if (y2 == 0)
                continue;

            x2 -= x1;
            if (x2 < -1 && x1 > 1)
                --x1;
            else if (x2 > 1 && x1 < 255)
                ++x1;

            if (y2 < 0)
            {
                for (; y2 < 0; y2++)
                    gScanlineEffectRegBuffers[1][y1 + y2 + (DISPLAY_HEIGHT * 3)] = x1;
            }
            else
            {
                for (; y2 > 0; y2--)
                    gScanlineEffectRegBuffers[1][y1 + y2 + (DISPLAY_HEIGHT * 3)] = x1;
            }
        }
    }

    if (offsetMaybe == 0 || deltaAngleMax % 4 == 0)
    {
        for (i = 0; i < DISPLAY_HEIGHT; i++)
            gScanlineEffectRegBuffers[1][i * 2 + offsetMaybe] = gScanlineEffectRegBuffers[1][i + (DISPLAY_HEIGHT * 2)] << 8 | gScanlineEffectRegBuffers[1][i + (DISPLAY_HEIGHT * 3)];

        return;
    }

    y1 = Sin(deltaAngleMax * 16, initRadius + (deltaAngleMax << 1));

    switch (deltaAngleMax / 4)
    {
    case 0:
        if (y1 > DISPLAY_HEIGHT / 2)
            y1 = DISPLAY_HEIGHT / 2;
        
        for (i = y1; i > 0; i--)
        {
            sTransitionStructPtr->data[2] = x1 = ((i * gUnknown_83FA444[deltaAngleMax]) >> 8) + (DISPLAY_WIDTH / 2);
            
            if (x1 < 0 || x1 > 255)
                continue;
            
            sTransitionStructPtr->bg123HOfs = 400 - i;
            sTransitionStructPtr->data[10] = gScanlineEffectRegBuffers[1][400 - i];
            
            if (gScanlineEffectRegBuffers[1][560 - i] < x1)
                gScanlineEffectRegBuffers[1][560 - i] = DISPLAY_WIDTH / 2;
            else if (gScanlineEffectRegBuffers[1][400 - i] < x1)
                gScanlineEffectRegBuffers[1][400 - i] = x1;
        }
        break;
    case 1:
        if (y1 > DISPLAY_HEIGHT / 2)
            y1 = DISPLAY_HEIGHT / 2;
        
        for (i = y1; i > 0; i--)
        {
            sTransitionStructPtr->data[2] = x1 = ((i * gUnknown_83FA444[deltaAngleMax]) >> 8) + (DISPLAY_WIDTH / 2);
            
            if (x1 < 0 || x1 > 255)
                continue;
            
            sTransitionStructPtr->bg123HOfs = 400 - i;
            sTransitionStructPtr->data[10] = gScanlineEffectRegBuffers[1][400 - i];
            
            if (gScanlineEffectRegBuffers[1][400 - i] < x1)
                gScanlineEffectRegBuffers[1][400 - i] = x1;
        }
        break;
    case 2:
        if (y1 < -(DISPLAY_HEIGHT / 2 - 1))
            y1 = -(DISPLAY_HEIGHT / 2 - 1);
        
        for (i = y1; i <= 0; i++)
        {
            sTransitionStructPtr->data[2] = x1 = ((i * gUnknown_83FA444[deltaAngleMax]) >> 8) + (DISPLAY_WIDTH / 2);
            
            if (x1 < 0 || x1 > 255)
                continue;
            
            sTransitionStructPtr->bg123HOfs = 560 - i;
            sTransitionStructPtr->data[10] = gScanlineEffectRegBuffers[1][560 - i];
            
            if (gScanlineEffectRegBuffers[1][400 - i] >= x1)
                gScanlineEffectRegBuffers[1][400 - i] = DISPLAY_WIDTH / 2;
            else if (gScanlineEffectRegBuffers[1][560 - i] > x1)
                gScanlineEffectRegBuffers[1][560 - i] = x1;
        }
        break;
    case 3:
        if (y1 < -(DISPLAY_HEIGHT / 2 - 1))
            y1 = -(DISPLAY_HEIGHT / 2 - 1);
        
        for (i = y1; i <= 0; i++)
        {
            sTransitionStructPtr->data[2] = x1 = ((i * gUnknown_83FA444[deltaAngleMax]) >> 8) + (DISPLAY_WIDTH / 2);
            
            if (x1 < 0 || x1 > 255)
                continue;
            
            sTransitionStructPtr->bg123HOfs = 560 - i;
            sTransitionStructPtr->data[10] = gScanlineEffectRegBuffers[1][560 - i];
            
            if (gScanlineEffectRegBuffers[1][560 - i] > x1)
                gScanlineEffectRegBuffers[1][560 - i] = x1;
        }
        break;
    default:
        break;
    }

    for (i = 0; i < DISPLAY_HEIGHT; i++)
        gScanlineEffectRegBuffers[1][i * 2 + offsetMaybe] = (gScanlineEffectRegBuffers[1][i + (DISPLAY_HEIGHT * 2)] << 8) | gScanlineEffectRegBuffers[1][i + (DISPLAY_HEIGHT * 3)];
}

static void VBCB_BT_Phase2AntiClockwiseBlackFade(void)
{
    DmaStop(0);
    BT_VBSyncOamAndPltt();
    
    if (!sTransitionStructPtr->counter)
    {
        if (sTransitionStructPtr->vblankDma)
        {
            DmaCopy16(3, gScanlineEffectRegBuffers[1], gScanlineEffectRegBuffers[0], DISPLAY_HEIGHT * 4);
            sTransitionStructPtr->vblankDma = FALSE;
        }
        SetGpuReg(REG_OFFSET_WININ, sTransitionStructPtr->winIn);
        SetGpuReg(REG_OFFSET_WINOUT, sTransitionStructPtr->winOut);
        SetGpuReg(REG_OFFSET_WIN0V, sTransitionStructPtr->win0V);
        SetGpuReg(REG_OFFSET_WIN1V, sTransitionStructPtr->win1V);
        SetGpuReg(REG_OFFSET_WIN0H, gScanlineEffectRegBuffers[0][0]);
        SetGpuReg(REG_OFFSET_WIN1H, gScanlineEffectRegBuffers[0][1]);
        
        DmaSet(0, gScanlineEffectRegBuffers[0], &REG_WIN0H, B_TRANS_DMA_FLAGS | (DMA_32BIT << 16));
    }
}

//--------------------------
// B_TRANSITION_SHRED_SPLIT
//--------------------------

static bool32 BT_Phase2ShredSplit_Init(struct Task *task);
static bool32 BT_Phase2ShredSplit_Main(struct Task *task);
static bool32 BT_Phase2ShredSplit_Next(struct Task *task);
static void HBCB_BT_Phase2ShredSplit(void);

static const u8 sShredSplit_SectionYCoords[] = {39, DISPLAY_HEIGHT - 41};
static const s16 sShredSplit_SectionMoveDirs[] = {1, -1};

static const TransitionStateFunc sBT_Phase2ShredSplitFuncs[] =
{
    BT_Phase2ShredSplit_Init,
    BT_Phase2ShredSplit_Main,
    BT_Phase2ShredSplit_Next,
    BT_EndScanlinePhase2Transition,
};

#define tDelayTimer data[1]
#define tExtent     data[2]
#define tDelay      data[3]

static void BT_Phase2ShredSplit(u32 taskId)
{
    while (sBT_Phase2ShredSplitFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2ShredSplit_Init(struct Task *task)
{
    u32 i;

    BT_InitCtrlBlk();
    ScanlineEffect_Clear();

    sTransitionStructPtr->winIn = WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR;
    sTransitionStructPtr->winOut = 0;
    sTransitionStructPtr->win0V = DISPLAY_HEIGHT;

    for (i = 0; i < DISPLAY_HEIGHT; i++)
    {
        gScanlineEffectRegBuffers[1][i] = sTransitionStructPtr->bg123HOfs;
        gScanlineEffectRegBuffers[1][DISPLAY_HEIGHT + i] = DISPLAY_WIDTH;
        gScanlineEffectRegBuffers[0][i] = sTransitionStructPtr->bg123HOfs;
        gScanlineEffectRegBuffers[0][DISPLAY_HEIGHT + i] = DISPLAY_WIDTH;
        gScanlineEffectRegBuffers[0][DISPLAY_HEIGHT * 2 + i] = 0;
        gScanlineEffectRegBuffers[0][DISPLAY_HEIGHT * 3 + i] = 256;
        gScanlineEffectRegBuffers[0][DISPLAY_HEIGHT * 4 + i] = 1;
    }
    task->tDelayTimer = 0;
    task->tExtent = 0;
    task->tDelay = 7;

    EnableInterrupts(INTR_FLAG_HBLANK);

    SetVBlankCallback(VBCB_BT_Phase2SlicedScreen);
    SetHBlankCallback(HBCB_BT_Phase2ShredSplit);

    task->tState++;
    return TRUE;
}

static bool32 BT_Phase2ShredSplit_Main(struct Task *task)
{
    u32 i, j, k;
    u8 baseY[ARRAY_COUNT(sShredSplit_SectionYCoords)];
    s16 moveDirs[ARRAY_COUNT(sShredSplit_SectionMoveDirs)];
    u32 linesFinished;
    u16 *ptr4, *ptr3, *ptr1, *ptr2;
    s16 y;

    memcpy(baseY, sShredSplit_SectionYCoords, sizeof(baseY));
    memcpy(moveDirs, sShredSplit_SectionMoveDirs, sizeof(moveDirs));

    sTransitionStructPtr->vblankDma = FALSE;
    linesFinished = 0;

    for (i = 0; i <= task->tExtent; i++)
    {
        // Slide half of the pixel rows (alternating) right
        for (j = 0; j < 2; j++)
        {
            for (k = 0; k < 2; k++)
            {
                y = baseY[j] + (moveDirs[k] * -i * 2);
                
                if (y >= 0 && (y != DISPLAY_HEIGHT / 2 - 1 || j != 1))
                {
                    ptr4 = &gScanlineEffectRegBuffers[0][y + DISPLAY_HEIGHT * 2];
                    ptr3 = &gScanlineEffectRegBuffers[0][y + DISPLAY_HEIGHT * 3];
                    ptr1 = &gScanlineEffectRegBuffers[0][y + DISPLAY_HEIGHT * 4];
                    
                    if (*ptr4 >= DISPLAY_WIDTH)
                    {
                        *ptr4 = DISPLAY_WIDTH;
                        linesFinished++;
                    }
                    else
                    {
                        *ptr4 += (*ptr3 >> 8);
                        if (*ptr1 <= 0x7F)
                            *ptr1 *= 2;
                        if (*ptr3 <= 0xFFF)
                            *ptr3 += *ptr1;
                    }
                    ptr2 = &gScanlineEffectRegBuffers[0][y];
                    ptr3 = &gScanlineEffectRegBuffers[0][y + DISPLAY_HEIGHT];
                    *ptr2 = sTransitionStructPtr->bg123HOfs + *ptr4;
                    *ptr3 = DISPLAY_WIDTH - *ptr4;

                    if (i == 0)
                        break;
                }
            }
        }

        // Slide the other half of the rows left
        for (j = 0; j < 2; j++)
        {
            for (k = 0; k < 2; k++)
            {
                y = baseY[j] + 1 + (moveDirs[k] * -i * 2);
                
                if (y <= DISPLAY_HEIGHT && (y != DISPLAY_HEIGHT / 2 || j != 1))
                {
                    ptr4 = &gScanlineEffectRegBuffers[0][y + DISPLAY_HEIGHT * 2];
                    ptr3 = &gScanlineEffectRegBuffers[0][y + DISPLAY_HEIGHT * 3];
                    ptr1 = &gScanlineEffectRegBuffers[0][y + DISPLAY_HEIGHT * 4];
                    
                    if (*ptr4 >= DISPLAY_WIDTH)
                    {
                        *ptr4 = DISPLAY_WIDTH;
                        linesFinished++;
                    }
                    else
                    {
                        *ptr4 += (*ptr3 >> 8);
                        if (*ptr1 <= 0x7F)
                            *ptr1 *= 2;
                        if (*ptr3 <= 0xFFF)
                            *ptr3 += *ptr1;
                    }
                    ptr2 = &gScanlineEffectRegBuffers[0][y];
                    ptr3 = &gScanlineEffectRegBuffers[0][y + DISPLAY_HEIGHT];
                    *ptr2 = sTransitionStructPtr->bg123HOfs - *ptr4;
                    *ptr3 = (*ptr4 << 8) | (DISPLAY_WIDTH + 1);

                    if (i == 0)
                        break;
                }
            }
        }
    }

    // Count down to next move
    if (--task->tDelayTimer < 0)
        task->tDelayTimer = 0;

    // Try increase effect's extent
    if (task->tDelayTimer <= 0 && task->tExtent + 1 <= DISPLAY_HEIGHT / 8)
    {
        task->tDelayTimer = task->tDelay;
        task->tExtent++;
    }

    // All lines have reached screen width, move on.
    if (linesFinished >= DISPLAY_HEIGHT)
        task->tState++;

    ++sTransitionStructPtr->vblankDma;
    return FALSE;
}

static bool32 BT_Phase2ShredSplit_Next(struct Task *task)
{
    task->tState++;
    return FALSE;
}

static void HBCB_BT_Phase2ShredSplit(void)
{
    if (REG_VCOUNT < DISPLAY_HEIGHT)
        HBCB_BT_Phase2UpdateBgsHorizontalOffsets();
}

#undef tDelayTimer
#undef tExtent
#undef tDelay

//---------------------
// B_TRANSITION_KYOGRE
//---------------------

static bool32 BT_Phase2Kyogre_BgFadeBlack(struct Task *task);
static bool32 BT_Phase2Kyogre_WaitPaletteFade(struct Task *task);
static bool32 BT_Phase2Kyogre_LoadGfx(struct Task *task);
static bool32 BT_Phase2Kyogre_PaletteFlash(struct Task *task);
static bool32 BT_Phase2Kyogre_PaletteBrighten(struct Task *task);
static bool32 BT_Phase2Kyogre_WaitEndTimer(struct Task *task);
static bool32 BT_Phase2Kyogre_FadeOut(struct Task *task);
static bool32 BT_Phase2Kyogre_End(struct Task *task);

static const u32 sKyogreTileset[] = INCBIN_U32("graphics/battle_transitions/kyogre.4bpp.lz");
static const u32 sKyogreTilemap[] = INCBIN_U32("graphics/battle_transitions/kyogre.bin.lz");

static const u16 sKyogre1_Palette[] = INCBIN_U16("graphics/battle_transitions/kyogre_pt1.gbapal");
static const u16 sKyogre2_Palette[] = INCBIN_U16("graphics/battle_transitions/kyogre_pt2.gbapal");

static const TransitionStateFunc sBT_Phase2KyogreFuncs[] =
{
    BT_Phase2Kyogre_BgFadeBlack,
    BT_Phase2Kyogre_WaitPaletteFade,
    BT_Phase2Kyogre_LoadGfx,
    BT_Phase2Kyogre_PaletteFlash,
    BT_Phase2Kyogre_PaletteBrighten,
    BT_Phase2Kyogre_WaitEndTimer,
    BT_Phase2Kyogre_FadeOut,
    BT_Phase2Kyogre_End,
};

#define tTimer    data[1]
#define tEndDelay data[2]

static void BT_Phase2Kyogre(u32 taskId)
{
    while (sBT_Phase2KyogreFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2Kyogre_BgFadeBlack(struct Task *task)
{
    BeginNormalPaletteFade(PALETTES_BG, 1, 0, 16, RGB_BLACK);
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2Kyogre_WaitPaletteFade(struct Task *task)
{
    if (!gPaletteFade.active)
        ++task->tState;
    
    return FALSE;
}

static bool32 BT_Phase2Kyogre_LoadGfx(struct Task *task)
{
    u16 *tilemapAddr, *tilesetAddr;

    BT_GetBg0TilemapAndTilesetBase(&tilemapAddr, &tilesetAddr);
    CpuFill16(0, tilemapAddr, BG_SCREEN_SIZE);
    LZDecompressVram(sKyogreTileset, tilesetAddr);
    LZDecompressVram(sKyogreTilemap, tilemapAddr);
    
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2Kyogre_PaletteFlash(struct Task *task)
{
    if (task->tTimer % 3 == 0)
    {
        u16 offset = (task->tTimer % 30) / 3;
        LoadPalette(&sKyogre1_Palette[offset * 16], 0xF0, 0x20);
    }
    
    if (++task->tTimer > 58)
    {
        task->tTimer = 0;
        ++task->tState;
    }
    return FALSE;
}

static bool32 BT_Phase2Kyogre_PaletteBrighten(struct Task *task)
{
    if (task->tTimer % 5 == 0)
    {
        s16 offset = task->tTimer / 5;
        LoadPalette(&sKyogre2_Palette[offset * 16], 0xF0, 0x20);
    }
    
    if (++task->tTimer > 68)
    {
        task->tTimer = 0;
        task->tEndDelay = 30;
        ++task->tState;
    }
    return FALSE;
}

static bool32 BT_Phase2Kyogre_WaitEndTimer(struct Task *task)
{
    if (--task->tEndDelay == 0)
        ++task->tState;
    
    return FALSE;
}

static bool32 BT_Phase2Kyogre_FadeOut(struct Task *task)
{
    BeginNormalPaletteFade(PALETTES_OBJECTS | (1 << 15), 1, 0, 16, RGB_BLACK);
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2Kyogre_End(struct Task *task)
{
    if (!gPaletteFade.active)
        BT_EndScanlinePhase2Transition(task);

    return FALSE;
}

//----------------------
// B_TRANSITION_GROUDON
//----------------------

static bool32 BT_Phase2Groudon_LoadGfx(struct Task *task);
static bool32 BT_Phase2Groudon_PaletteFlash(struct Task *task);
static bool32 BT_Phase2Groudon_PaletteBrighten(struct Task *task);

static const u32 sGroudonTileset[] = INCBIN_U32("graphics/battle_transitions/groudon.4bpp.lz");
static const u32 sGroudonTilemap[] = INCBIN_U32("graphics/battle_transitions/groudon.bin.lz");

static const u16 sGroudon1_Palette[] = INCBIN_U16("graphics/battle_transitions/groudon_pt1.gbapal");
static const u16 sGroudon2_Palette[] = INCBIN_U16("graphics/battle_transitions/groudon_pt2.gbapal");

static const TransitionStateFunc sBT_Phase2GroudonFuncs[] =
{
    BT_Phase2Kyogre_BgFadeBlack,
    BT_Phase2Kyogre_WaitPaletteFade,
    BT_Phase2Groudon_LoadGfx,
    BT_Phase2Groudon_PaletteFlash,
    BT_Phase2Groudon_PaletteBrighten,
    BT_Phase2Kyogre_WaitEndTimer,
    BT_Phase2Kyogre_FadeOut,
    BT_Phase2Kyogre_End,
};

static void BT_Phase2Groudon(u32 taskId)
{
    while (sBT_Phase2GroudonFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2Groudon_LoadGfx(struct Task *task)
{
    u16 *tilemapAddr, *tilesetAddr;

    BT_GetBg0TilemapAndTilesetBase(&tilemapAddr, &tilesetAddr);
    CpuFill16(0, tilemapAddr, BG_SCREEN_SIZE);
    LZDecompressVram(sGroudonTileset, tilesetAddr);
    LZDecompressVram(sGroudonTilemap, tilemapAddr);
    
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2Groudon_PaletteFlash(struct Task *task)
{
    if (task->tTimer % 3 == 0)
    {
        u16 offset = (task->tTimer % 30) / 3;
        LoadPalette(&sGroudon1_Palette[offset * 16], 0xF0, 0x20);
    }
    
    if (++task->tTimer > 58)
    {
        task->tTimer = 0;
        ++task->tState;
    }
    return FALSE;
}

static bool32 BT_Phase2Groudon_PaletteBrighten(struct Task *task)
{
    if (task->tTimer % 5 == 0)
    {
        s16 offset = task->tTimer / 5;
        LoadPalette(&sGroudon2_Palette[offset * 16], 0xF0, 0x20);
    }
    
    if (++task->tTimer > 68)
    {
        task->tTimer = 0;
        task->tEndDelay = 30;
        ++task->tState;
    }
    return FALSE;
}

#undef tTimer
#undef tEndDelay

//------------------------
// B_TRANSITION_BLACKHOLE
//------------------------

static bool32 BT_Phase2BlackHole_Init(struct Task *task);
static bool32 BT_Phase2BlackHole_Vibrate(struct Task *task);
static bool32 BT_Phase2BlackHole_Grow(struct Task *task);

// Blackhole rapidly alternates adding these values to the radius,
// resulting in a vibrating shrink/grow effect.
static const s16 sBlackhole_Vibrations[] = {-6, 4};

static const TransitionStateFunc sBT_Phase2BlackHoleFuncs[] =
{
    BT_Phase2BlackHole_Init,
    BT_Phase2BlackHole_Vibrate,
    BT_Phase2BlackHole_Grow,
};

#define tRadius    data[1]
#define tGrowSpeed data[2]
#define tVibrateId data[6]
#define tFlag      data[7] // Used generally to indicate an action has taken place.

static void BT_Phase2BlackHole(u32 taskId)
{
    while (sBT_Phase2BlackHoleFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2BlackHole_Init(struct Task *task)
{
    u32 i;
    
    BT_InitCtrlBlk();
    ScanlineEffect_Clear();

    sTransitionStructPtr->winIn = 0;
    sTransitionStructPtr->winOut = WINOUT_WIN01_ALL;
    sTransitionStructPtr->win0H = DISPLAY_WIDTH;
    sTransitionStructPtr->win0V = DISPLAY_HEIGHT;
    
    for (i = 0; i < DISPLAY_HEIGHT; i++)
        gScanlineEffectRegBuffers[1][i] = 0;
    
    SetVBlankCallback(VBCB_BT_Phase2BigPokeball2);
    
    task->tRadius = 1;
    task->tGrowSpeed = 1 << 8;
    task->tFlag = FALSE;
    
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2BlackHole_Vibrate(struct Task *task)
{
    sTransitionStructPtr->vblankDma = FALSE;
    
    if (task->tFlag == FALSE)
    {
        task->tFlag++;
        task->tRadius = 48;
        task->tVibrateId = 0;
    }
    
    task->tRadius += sBlackhole_Vibrations[task->tVibrateId];
    task->tVibrateId = (task->tVibrateId + 1) % ARRAY_COUNT(sBlackhole_Vibrations);
    
    BT_GenerateCircle(gScanlineEffectRegBuffers[0], DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, task->tRadius);
    
    if (task->tRadius < 9)
    {
        ++task->tState;
        task->tFlag = FALSE;
    }
    
    ++sTransitionStructPtr->vblankDma;
    return FALSE;
}

static bool32 BT_Phase2BlackHole_Grow(struct Task *task)
{
    if (task->tFlag)
    {
        DmaStop(0);
        SetVBlankCallback(NULL);
        BT_DestroyPhase2AnimTask(task);
    }
    else
    {
        sTransitionStructPtr->vblankDma = FALSE;
        
        if (task->tGrowSpeed < 1024)
            task->tGrowSpeed += 128;
        
        if (task->tRadius < DISPLAY_HEIGHT)
            task->tRadius += task->tGrowSpeed >> 8;
        
        if (task->tRadius > DISPLAY_HEIGHT)
            task->tRadius = DISPLAY_HEIGHT;
        
        BT_GenerateCircle(gScanlineEffectRegBuffers[0], DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, task->tRadius);
        
        if (task->tRadius == DISPLAY_HEIGHT)
        {
            task->tFlag = TRUE;
            BT_BlendPalettesToBlack();
        }
        else
            ++sTransitionStructPtr->vblankDma;
    }
    return FALSE;
}

//--------------------------------
// B_TRANSITION_BLACKHOLE_PULSATE
//--------------------------------

static bool32 BT_Phase2BlackHolePulsate_Main(struct Task *task);

static const TransitionStateFunc sBT_Phase2BlackHolePulsateFuncs[] =
{
    BT_Phase2BlackHole_Init,
    BT_Phase2BlackHolePulsate_Main,
};

#define tSinIndex  data[5]
#define tAmplitude data[6]

static void BT_Phase2BlackHolePulsate(u32 taskId)
{
    while (sBT_Phase2BlackHolePulsateFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2BlackHolePulsate_Main(struct Task *task)
{
    s16 index, amplitude;
    
    sTransitionStructPtr->vblankDma = FALSE;
    
    if (task->tFlag == FALSE)
    {
        task->tFlag++;
        task->tSinIndex = 2;
        task->tAmplitude = 2;
    }
    
    if (task->tRadius > DISPLAY_HEIGHT)
        task->tRadius = DISPLAY_HEIGHT;
    
    BT_GenerateCircle(gScanlineEffectRegBuffers[0], DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, task->tRadius);
    
    if (task->tRadius == DISPLAY_HEIGHT)
        BT_EndScanlinePhase2Transition(task);
    
    index = task->tSinIndex;
    if ((task->tSinIndex & 0xFF) <= 128)
    {
        amplitude = task->tAmplitude;
        task->tSinIndex += 8;
    }
    else
    {
        amplitude = task->tAmplitude - 1;
        task->tSinIndex += 16;
    }
    task->tRadius += Sin(index & 0xFF, amplitude);

    if (task->tRadius <= 0)
        task->tRadius = 1;

    if (task->tSinIndex >= 0xFF)
    {
        task->tSinIndex >>= 8;
        task->tAmplitude++;
    }
    ++sTransitionStructPtr->vblankDma;
    return FALSE;
}

#undef tSinIndex
#undef tAmplitude

//-----------------------
// B_TRANSITION_RAYQUAZA
//-----------------------

static bool32 BT_Phase2Rayquaza_Init(struct Task *task);
static bool32 BT_Phase2Rayquaza_PaletteFlash(struct Task *task);
static bool32 BT_Phase2Rayquaza_FadeToBlack(struct Task *task);
static bool32 BT_Phase2Rayquaza_WaitPaletteFade(struct Task *task);
static bool32 BT_Phase2Rayquaza_SetBlack(struct Task *task);
static bool32 BT_Phase2Rayquaza_TriRing(struct Task *task);
static void VBCB_BT_Phase2Rayquaza(void);

static const u32 sRayquazaTileset[] = INCBIN_U32("graphics/battle_transitions/rayquaza.4bpp");
static const u32 sRayquazaTilemap[] = INCBIN_U32("graphics/battle_transitions/rayquaza.bin");
static const u16 sRayquazaPalette[] = INCBIN_U16("graphics/battle_transitions/rayquaza.gbapal");

static const TransitionStateFunc sBT_Phase2RayquazaFuncs[] =
{
    BT_Phase2Kyogre_BgFadeBlack,
    BT_Phase2Kyogre_WaitPaletteFade,
    BT_Phase2Rayquaza_Init,
    BT_Phase2Rayquaza_PaletteFlash,
    BT_Phase2Rayquaza_FadeToBlack,
    BT_Phase2Rayquaza_WaitPaletteFade,
    BT_Phase2Rayquaza_SetBlack,
    BT_Phase2Rayquaza_TriRing,
    BT_Phase2BlackHole_Vibrate,
    BT_Phase2BlackHole_Grow,
};

#define tTimer data[1]

static void BT_Phase2Rayquaza(u32 taskId)
{
    while (sBT_Phase2RayquazaFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2Rayquaza_Init(struct Task *task)
{
    u32 i;
    u16 *tilemapAddr, *tilesetAddr;
    
    BT_InitCtrlBlk();
    ScanlineEffect_Clear();
    
    SetGpuReg(REG_OFFSET_BG0CNT, BGCNT_CHARBASE(2) | BGCNT_SCREENBASE(26) | BGCNT_TXT256x512);

    BT_GetBg0TilemapAndTilesetBase(&tilemapAddr, &tilesetAddr);
    CpuFill16(0, tilemapAddr, BG_SCREEN_SIZE);
    CpuCopy16(sRayquazaTilemap, tilemapAddr, sizeof(sRayquazaTilemap));
    CpuCopy16(sRayquazaTileset, tilesetAddr, 0x2000);
    
    LoadPalette(&sRayquazaPalette[80], 0xF0, 0x20);
    
    sTransitionStructPtr->counter = 0;
    
    for (i = 0; i < DISPLAY_HEIGHT; i++)
    {
        gScanlineEffectRegBuffers[0][i] = 0;
        gScanlineEffectRegBuffers[1][i] = 0x100;
    }
    SetVBlankCallback(VBCB_BT_Phase2Rayquaza);
    
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2Rayquaza_PaletteFlash(struct Task *task)
{
    if (task->tTimer % 4 == 0)
    {
        u16 value = task->tTimer / 4;
        LoadPalette(&sRayquazaPalette[(value + 5) * 16], 0xF0, 0x20);
    }
    
    if (++task->tTimer > 40)
    {
        task->tTimer = 0;
        ++task->tState;
    }
    return FALSE;
}

static bool32 BT_Phase2Rayquaza_FadeToBlack(struct Task *task)
{
    if (++task->tTimer > 20)
    {
        task->tTimer = 0;
        ++task->tState;
        BeginNormalPaletteFade(PALETTES_OBJECTS | (1 << 15), 2, 0, 16, RGB_BLACK);
    }
    return FALSE;
}

static bool32 BT_Phase2Rayquaza_WaitPaletteFade(struct Task *task)
{
    if (!gPaletteFade.active)
    {
        sTransitionStructPtr->counter = 1;
        ++task->tState;
    }
    return FALSE;
}

static bool32 BT_Phase2Rayquaza_SetBlack(struct Task *task)
{
    BlendPalettes(PALETTES_BG & ~(1 << 15), 8, RGB_BLACK);
    BlendPalettes(PALETTES_OBJECTS | (1 << 15), 0, RGB_BLACK);
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2Rayquaza_TriRing(struct Task *task)
{
    if (task->tTimer % 3 == 0)
    {
        u16 value = task->tTimer / 3;
        LoadPalette(&sRayquazaPalette[(value + 0) * 16], 0xF0, 0x20);
    }
    
    if (++task->tTimer >= 40)
    {
        u32 i;
        
        sTransitionStructPtr->winIn = 0;
        sTransitionStructPtr->winOut = WINOUT_WIN01_ALL;
        sTransitionStructPtr->win0H = DISPLAY_WIDTH;
        sTransitionStructPtr->win0V = DISPLAY_HEIGHT;
        
        for (i = 0; i < DISPLAY_HEIGHT; i++)
            gScanlineEffectRegBuffers[1][i] = 0;
        
        SetVBlankCallback(VBCB_BT_Phase2BigPokeball2);
        
        task->tGrowSpeed = 1 << 8;
        task->tFlag = FALSE;
        ++task->tState;

        ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_BG0_ON);
    }
    return FALSE;
}

static void VBCB_BT_Phase2Rayquaza(void)
{
    void *dmaSrc;

    DmaStop(0);
    BT_VBSyncOamAndPltt();
    
    if (sTransitionStructPtr->counter == 1)
        dmaSrc = gScanlineEffectRegBuffers[1];
    else
        dmaSrc = gScanlineEffectRegBuffers[0];

    DmaSet(0, dmaSrc, &REG_BG0VOFS, B_TRANS_DMA_FLAGS);
}

#undef tTimer

#undef tRadius
#undef tGrowSpeed
#undef tVibrateId
#undef tFlag

//---------------------------------
// B_TRANSITION_RECTANGULAR_SPIRAL
//---------------------------------

static bool32 BT_Phase2RectangularSpiral_Init(struct Task *task);
static bool32 BT_Phase2RectangularSpiral_Main(struct Task *task);
static bool32 UpdateRectangularSpiralLine(const s16 *const *moveDataTable, struct RectangularSpiralLine *line);

// Note that the directions are inverted for the lines originating at the bottom.
// i.e. MOVE_RIGHT is a right move for the top lines and a left move for the inverted bottom lines.
enum {
    MOVE_RIGHT = 1,
    MOVE_LEFT,
    MOVE_UP,
    MOVE_DOWN,
};

// Offsets of the movement data for spiraling in either direction.
#define SPIRAL_INWARD_START  0
#define SPIRAL_INWARD_END    3
#define SPIRAL_OUTWARD_START 4
#define SPIRAL_OUTWARD_END   7

#define SPIRAL_END     -1
#define SPIRAL_REBOUND -2

static const s16 sRectangularSpiral_Major_InwardRight[]  = {MOVE_RIGHT,  27, 275, SPIRAL_END};
static const s16 sRectangularSpiral_Major_InwardLeft[]   = {MOVE_LEFT,  486, SPIRAL_END};
static const s16 sRectangularSpiral_Major_InwardUp[]     = {MOVE_UP,    262, SPIRAL_END};
static const s16 sRectangularSpiral_Major_InwardDown[]   = {MOVE_DOWN,  507, SPIRAL_REBOUND};

static const s16 sRectangularSpiral_Minor_InwardRight[]  = {MOVE_RIGHT, 213, SPIRAL_END};
static const s16 sRectangularSpiral_Minor_InwardLeft[]   = {MOVE_LEFT,  548, SPIRAL_REBOUND};
static const s16 sRectangularSpiral_Minor_InwardUp[]     = {MOVE_UP,    196, SPIRAL_END};
static const s16 sRectangularSpiral_Minor_InwardDown[]   = {MOVE_DOWN,  573, 309, SPIRAL_END};

static const s16 sRectangularSpiral_Minor_OutwardRight[] = {MOVE_RIGHT, 474, SPIRAL_END};
static const s16 sRectangularSpiral_Minor_OutwardLeft[]  = {MOVE_LEFT,  295, 32, SPIRAL_END};
static const s16 sRectangularSpiral_Minor_OutwardUp[]    = {MOVE_UP,     58, SPIRAL_END};
static const s16 sRectangularSpiral_Minor_OutwardDown[]  = {MOVE_DOWN,  455, SPIRAL_END};

static const s16 sRectangularSpiral_Major_OutwardRight[] = {MOVE_RIGHT, 540, SPIRAL_END};
static const s16 sRectangularSpiral_Major_OutwardLeft[]  = {MOVE_LEFT,  229, SPIRAL_END};
static const s16 sRectangularSpiral_Major_OutwardUp[]    = {MOVE_UP,    244, 28, SPIRAL_END};
static const s16 sRectangularSpiral_Major_OutwardDown[]  = {MOVE_DOWN,  517, SPIRAL_END};

// Move data for spiral lines starting in the top left / bottom right
static const s16 *const sRectangularSpiral_MoveDataTable_MajorDiagonal[] =
{
    [SPIRAL_INWARD_START] =
    sRectangularSpiral_Major_InwardRight,
    sRectangularSpiral_Major_InwardDown,
    sRectangularSpiral_Major_InwardLeft,
    sRectangularSpiral_Major_InwardUp,

    [SPIRAL_OUTWARD_START] =
    sRectangularSpiral_Major_OutwardUp,
    sRectangularSpiral_Major_OutwardLeft,
    sRectangularSpiral_Major_OutwardDown,
    sRectangularSpiral_Major_OutwardRight
};

// Move data for spiral lines starting in the top right / bottom left
static const s16 *const sRectangularSpiral_MoveDataTable_MinorDiagonal[] =
{
    [SPIRAL_INWARD_START] =
    sRectangularSpiral_Minor_InwardDown,
    sRectangularSpiral_Minor_InwardLeft,
    sRectangularSpiral_Minor_InwardUp,
    sRectangularSpiral_Minor_InwardRight,

    [SPIRAL_OUTWARD_START] =
    sRectangularSpiral_Minor_OutwardLeft,
    sRectangularSpiral_Minor_OutwardDown,
    sRectangularSpiral_Minor_OutwardRight,
    sRectangularSpiral_Minor_OutwardUp
};

static const s16 *const *const sRectangularSpiral_MoveDataTables[] =
{
    sRectangularSpiral_MoveDataTable_MajorDiagonal,
    sRectangularSpiral_MoveDataTable_MinorDiagonal
};

static const TransitionStateFunc sBT_Phase2RectangularSpiralFuncs[] =
{
    BT_Phase2RectangularSpiral_Init,
    BT_Phase2RectangularSpiral_Main,
    BT_EndScanlinePhase2Transition,
};

static void BT_Phase2RectangularSpiral(u32 taskId)
{
    while (sBT_Phase2RectangularSpiralFuncs[gTasks[taskId].tState](&gTasks[taskId]));
}

static bool32 BT_Phase2RectangularSpiral_Init(struct Task *task)
{
    u16 *tilemapAddr, *tilesetAddr;

    BT_GetBg0TilemapAndTilesetBase(&tilemapAddr, &tilesetAddr);
    
    CpuFill16(0xF000, tilemapAddr, 0x800);
    CpuCopy16(sGridSquareTileset, tilesetAddr, 0x20);
    CpuCopy16(&sGridSquareTileset[0x70], &tilesetAddr[0x20], 0x20);
    
    LoadPalette(gSlidingPokeballBigPokeballPalette, 0xF0, 0x20);
    
    task->data[3] = 1;
    
    // Line starting in top left
    sRectangularSpiralLines[0].state = SPIRAL_INWARD_START;
    sRectangularSpiralLines[0].position = -1;
    sRectangularSpiralLines[0].moveIndex = 1;
    sRectangularSpiralLines[0].reboundPosition = 308;
    sRectangularSpiralLines[0].outward = FALSE;

    // Line starting in bottom right
    sRectangularSpiralLines[1].state = SPIRAL_INWARD_START;
    sRectangularSpiralLines[1].position = -1;
    sRectangularSpiralLines[1].moveIndex = 1;
    sRectangularSpiralLines[1].reboundPosition = 308;
    sRectangularSpiralLines[1].outward = FALSE;

    // Line starting in top right
    sRectangularSpiralLines[2].state = SPIRAL_INWARD_START;
    sRectangularSpiralLines[2].position = -3;
    sRectangularSpiralLines[2].moveIndex = 1;
    sRectangularSpiralLines[2].reboundPosition = 307;
    sRectangularSpiralLines[2].outward = FALSE;

    // Line starting in bottom left
    sRectangularSpiralLines[3].state = SPIRAL_INWARD_START;
    sRectangularSpiralLines[3].position = -3;
    sRectangularSpiralLines[3].moveIndex = 1;
    sRectangularSpiralLines[3].reboundPosition = 307;
    sRectangularSpiralLines[3].outward = FALSE;
    
    ++task->tState;
    return FALSE;
}

static bool32 BT_Phase2RectangularSpiral_Main(struct Task *task)
{
    u32 i, j;
    u16 *tilemapAddr;
    bool32 done = TRUE;

    BT_GetBg0TilemapBase(&tilemapAddr);
    
    // Draw 2 tiles at a time for each spiral line
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < ARRAY_COUNT(sRectangularSpiralLines); j++)
        {
            if (UpdateRectangularSpiralLine(sRectangularSpiral_MoveDataTables[j / 2], &sRectangularSpiralLines[j]))
            {
                s16 x, y;
                s16 position = sRectangularSpiralLines[j].position;
                
                done = FALSE; // The line moved to a new position, draw the tile.

                // Invert position for the two lines that start at the bottom.
                if ((j % 2) == 1)
                    position = 637 - position;

                x = position % 32;
                y = position / 32;
                
                tilemapAddr[y * 32 + x] = 2 | 0xF000;
            }
        }
    }

    if (done)
        ++task->tState;
    
    return FALSE;
}

// Returns TRUE if a tile should be drawn, FALSE otherwise
static bool32 UpdateRectangularSpiralLine(const s16 *const *moveDataTable, struct RectangularSpiralLine *line)
{
    const s16 *moveData = moveDataTable[line->state];

    // Has spiral finished?
    // Note that most move data arrays endsin SPIRAL_END but it is
    // only ever reached on the final array of spiraling outward.
    if (moveData[line->moveIndex] == SPIRAL_END)
        return FALSE;

    // Note that for the two lines originating at the bottom the
    // position is inverted, so the directions are flipped.
    // i.e. position += 1 is right for the top lines and left
    // for their inverted partners on the bottom.
    switch (moveData[0])
    {
        case MOVE_RIGHT:
            line->position += 1;
            break;
        case MOVE_LEFT:
            line->position -= 1;
            break;
        case MOVE_UP:
            line->position -= 32;
            break;
        case MOVE_DOWN:
            line->position += 32;
            break;
    }

    if (!line->outward && moveData[line->moveIndex] == SPIRAL_REBOUND)
    {
        // Line has reached the final point of spiraling inward.
        // Time to flip and start spiraling outward.
        line->outward = TRUE;
        line->moveIndex = 1;
        line->position = line->reboundPosition;
        line->state = SPIRAL_OUTWARD_START;
    }

    // Reached move target, advance to next movement.
    if (line->position == moveData[line->moveIndex])
    {
        line->state++;
        
        if (line->outward)
        {
            if (line->state > SPIRAL_OUTWARD_END)
            {
                // Still spiraling outward, loop back to the first state
                // but use the second set of move targets.
                // For example, the 28 in sRectangularSpiral_Major_OutwardUp
                line->moveIndex++;
                line->state = SPIRAL_OUTWARD_START;
            }
        }
        else
        {
            if (line->state > SPIRAL_INWARD_END)
            {
                // Still spiraling inward, loop back to the first state
                // but use the second set of move targets.
                // For example, the 275 in sRectangularSpiral_Major_InwardRight
                line->moveIndex++;
                line->state = SPIRAL_INWARD_START;
            }
        }
    }
    return TRUE;
}

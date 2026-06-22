#include "global.h"
#include "event_object_movement.h"
#include "fieldmap.h"
#include "field_effect.h"
#include "field_fadetransition.h"
#include "field_player_avatar.h"
#include "field_weather.h"
#include "gflib.h"
#include "math_util.h"
#include "metatile_behavior.h"
#include "object_event_graphics.h"
#include "overworld.h"
#include "script.h"
#include "special_field_anim.h"
#include "task.h"
#include "trig.h"
#include "constants/event_objects.h"
#include "constants/metatile_behaviors.h"
#include "constants/songs.h"

////////////////////
// ESCALATOR WARP //
////////////////////

static void Task_EscalatorWarpFieldEffect(u32 taskId);
static bool32 EscalatorWarpEffect_1(struct Task * task);
static bool32 EscalatorWarpEffect_2(struct Task * task);
static bool32 EscalatorWarpEffect_3(struct Task * task);
static bool32 EscalatorWarpEffect_4(struct Task * task);
static bool32 EscalatorWarpEffect_5(struct Task * task);
static bool32 EscalatorWarpEffect_6(struct Task * task);
static void Escalator_AnimatePlayerGoingDown(struct Task * task);
static void Escalator_AnimatePlayerGoingUp(struct Task * task);
static void Escalator_TransitionToWarpInEffect(void);
static void FieldCB_EscalatorWarpIn(void);
static void Task_EscalatorWarpInFieldEffect(u32 taskId);
static bool32 EscalatorWarpInEffect_1(struct Task * task);
static bool32 EscalatorWarpInEffect_2(struct Task * task);
static bool32 EscalatorWarpInEffect_3(struct Task * task);
static bool32 EscalatorWarpInEffect_4(struct Task * task);
static bool32 EscalatorWarpInEffect_5(struct Task * task);
static bool32 EscalatorWarpInEffect_6(struct Task * task);
static bool32 EscalatorWarpInEffect_7(struct Task * task);

static bool32 (*const sEscalatorWarpFieldEffectFuncs[])(struct Task * task) = {
    EscalatorWarpEffect_1,
    EscalatorWarpEffect_2,
    EscalatorWarpEffect_3,
    EscalatorWarpEffect_4,
    EscalatorWarpEffect_5,
    EscalatorWarpEffect_6
};

static bool32 (*const sEscalatorWarpInFieldEffectFuncs[])(struct Task * task) = {
    EscalatorWarpInEffect_1,
    EscalatorWarpInEffect_2,
    EscalatorWarpInEffect_3,
    EscalatorWarpInEffect_4,
    EscalatorWarpInEffect_5,
    EscalatorWarpInEffect_6,
    EscalatorWarpInEffect_7
};

void StartEscalatorWarp(u32 metatileBehavior, u32 priority)
{
    gTasks[CreateTask(Task_EscalatorWarpFieldEffect, priority)].data[1] = (metatileBehavior == MB_UP_ESCALATOR);
}

static void Task_EscalatorWarpFieldEffect(u32 taskId)
{
    while (sEscalatorWarpFieldEffectFuncs[gTasks[taskId].data[0]](&gTasks[taskId]));
}

static bool32 EscalatorWarpEffect_1(struct Task * task)
{
    FreezeObjectEvents();
    CameraObjectReset2();
    StartEscalator(task->data[1]);
    task->data[0]++;
    return FALSE;
}

static bool32 EscalatorWarpEffect_2(struct Task * task)
{
    struct ObjectEvent * objectEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    if (!ObjectEventIsMovementOverridden(objectEvent) || ObjectEventClearHeldMovementIfFinished(objectEvent))
    {
        ObjectEventSetHeldMovement(objectEvent, GetFaceDirectionMovementAction(GetPlayerFacingDirection()));
        
        objectEvent->noShadow = TRUE; // hide shadow for cleaner movement
        
        task->data[0]++;
        task->data[2] = 0;
        task->data[3] = 0;
        
        if ((u8)task->data[1] == 0)
            task->data[0] = 4;

        PlaySE(SE_ESCALATOR);
    }
    return FALSE;
}

static void Escalator_BeginFadeOutToNewMap(void)
{
    TryFadeOutOldMapMusic();
    WarpFadeOutScreen();
}

static bool32 EscalatorWarpEffect_3(struct Task * task)
{
    Escalator_AnimatePlayerGoingDown(task);
    
    if (task->data[2] > 3)
    {
        Escalator_BeginFadeOutToNewMap();
        task->data[0]++;
    }
    return FALSE;
}

static bool32 EscalatorWarpEffect_4(struct Task * task)
{
    Escalator_AnimatePlayerGoingDown(task);
    Escalator_TransitionToWarpInEffect();
    return FALSE;
}

static bool32 EscalatorWarpEffect_5(struct Task * task)
{
    Escalator_AnimatePlayerGoingUp(task);
    
    if (task->data[2] > 3)
    {
        Escalator_BeginFadeOutToNewMap();
        task->data[0]++;
    }
    return FALSE;
}

static bool32 EscalatorWarpEffect_6(struct Task * task)
{
    Escalator_AnimatePlayerGoingUp(task);
    Escalator_TransitionToWarpInEffect();
    return FALSE;
}

static void Escalator_AnimatePlayerGoingDown(struct Task * task)
{
    struct Sprite * sprite = &gSprites[gPlayerAvatar.spriteId];

    sprite->x2 = Cos(0x84, task->data[2]);
    sprite->y2 = Sin(0x94, task->data[2]);
    
    task->data[3]++;
    if (task->data[3] & 1)
        task->data[2]++;
}

static void Escalator_AnimatePlayerGoingUp(struct Task * task)
{
    struct Sprite * sprite = &gSprites[gPlayerAvatar.spriteId];

    sprite->x2 = Cos(0x7c, task->data[2]);
    sprite->y2 = Sin(0x76, task->data[2]);
    
    task->data[3]++;
    if (task->data[3] & 1)
        task->data[2]++;
}

static void Escalator_TransitionToWarpInEffect(void)
{
    if (!gPaletteFade.active && IsNotWaitingForBGMStop())
    {
        StopEscalator();
        WarpIntoMap();
        gFieldCallback = FieldCB_EscalatorWarpIn;
        SetMainCallback2(CB2_LoadMap);
        DestroyTask(FindTaskIdByFunc(Task_EscalatorWarpFieldEffect));
    }
}

static void FieldCB_EscalatorWarpIn(void)
{
    Overworld_PlaySpecialMapMusic();
    WarpFadeInScreen();
    ScriptContext2_Enable();
    FreezeObjectEvents();
    CreateTask(Task_EscalatorWarpInFieldEffect, 0);
    gFieldCallback = NULL;
}

static void Task_EscalatorWarpInFieldEffect(u32 taskId)
{
    while (sEscalatorWarpInFieldEffectFuncs[gTasks[taskId].data[0]](&gTasks[taskId]));
}

static bool32 EscalatorWarpInEffect_1(struct Task * task)
{
    struct ObjectEvent * objectEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
    s16 x, y;
    bool32 goingUp;
    
    CameraObjectReset2();
    objectEvent->noShadow = TRUE;
    ObjectEventSetHeldMovement(objectEvent, GetFaceDirectionMovementAction(DIR_EAST));
    PlayerGetDestCoords(&x, &y);
    
    task->data[0]++;
    task->data[1] = 16;
    
    if (MapGridGetMetatileBehaviorAt(x, y) == MB_DOWN_ESCALATOR)
    {
        goingUp = TRUE;
        task->data[0] = 3;
    }
    else
        goingUp = FALSE;
    
    StartEscalator(goingUp);
    return TRUE;
}

static bool32 EscalatorWarpInEffect_2(struct Task * task)
{
    struct Sprite * sprite = &gSprites[gPlayerAvatar.spriteId];

    sprite->x2 = Cos(0x84, task->data[1]);
    sprite->y2 = Sin(0x94, task->data[1]);
    task->data[0]++;
    return FALSE;
}

static bool32 EscalatorWarpInEffect_3(struct Task * task)
{
    struct Sprite * sprite = &gSprites[gPlayerAvatar.spriteId];

    sprite->x2 = Cos(0x84, task->data[1]);
    sprite->y2 = Sin(0x94, task->data[1]);
    
    if (++task->data[2] & 1)
        task->data[1]--;

    if (task->data[1] == 0)
    {
        sprite->x2 = 0;
        sprite->y2 = 0;
        task->data[0] = 5;
    }
    return FALSE;
}

static bool32 EscalatorWarpInEffect_4(struct Task * task)
{
    struct Sprite * sprite = &gSprites[gPlayerAvatar.spriteId];

    sprite->x2 = Cos(0x7c, task->data[1]);
    sprite->y2 = Sin(0x76, task->data[1]);
    task->data[0]++;
    return FALSE;
}

static bool32 EscalatorWarpInEffect_5(struct Task * task)
{
    struct Sprite * sprite = &gSprites[gPlayerAvatar.spriteId];

    sprite->x2 = Cos(0x7c, task->data[1]);
    sprite->y2 = Sin(0x76, task->data[1]);
    
    if (++task->data[2] & 1)
        task->data[1]--;

    if (task->data[1] == 0)
    {
        sprite->x2 = 0;
        sprite->y2 = 0;
        task->data[0]++;
    }
    return FALSE;
}

static bool32 EscalatorWarpInEffect_6(struct Task * task)
{
    if (IsEscalatorMoving())
        return FALSE;

    StopEscalator();
    task->data[0]++;
    return TRUE;
}

static bool32 EscalatorWarpInEffect_7(struct Task * task)
{
    struct ObjectEvent * objectEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
    
    objectEvent->noShadow = FALSE;
    
    if (ObjectEventClearHeldMovementIfFinished(objectEvent))
    {
        CameraObjectReset1();
        ScriptContext2_Disable();
        UnfreezeObjectEvents();
        ObjectEventSetHeldMovement(objectEvent, GetWalkNormalMovementAction(DIR_EAST));
        DestroyTask(FindTaskIdByFunc(Task_EscalatorWarpInFieldEffect));
    }
    return FALSE;
}

////////////////
// WARP ARROW //
////////////////

static const struct SpriteFrameImage sPicTable_Arrow[] =
{
    overworld_frame(gFieldEffectObjectPic_Arrow, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_Arrow, 2, 2, 1),
    overworld_frame(gFieldEffectObjectPic_Arrow, 2, 2, 2),
    overworld_frame(gFieldEffectObjectPic_Arrow, 2, 2, 3),
    overworld_frame(gFieldEffectObjectPic_Arrow, 2, 2, 4),
    overworld_frame(gFieldEffectObjectPic_Arrow, 2, 2, 5),
    overworld_frame(gFieldEffectObjectPic_Arrow, 2, 2, 6),
    overworld_frame(gFieldEffectObjectPic_Arrow, 2, 2, 7),
};

static const union AnimCmd sArrowAnim_South[] =
{
    ANIMCMD_FRAME(2, 32),
    ANIMCMD_FRAME(3, 32),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sArrowAnim_North[] =
{
    ANIMCMD_FRAME(0, 32),
    ANIMCMD_FRAME(1, 32),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sArrowAnim_West[] =
{
    ANIMCMD_FRAME(4, 32),
    ANIMCMD_FRAME(5, 32),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sArrowAnim_East[] =
{
    ANIMCMD_FRAME(6, 32),
    ANIMCMD_FRAME(7, 32),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnimTable_Arrow[] =
{
    [DIR_SOUTH - 1] = sArrowAnim_South,
    [DIR_NORTH - 1] = sArrowAnim_North,
    [DIR_WEST  - 1] = sArrowAnim_West,
    [DIR_EAST  - 1] = sArrowAnim_East,
};

static const struct SpriteTemplate sSpriteTemplate_Arrow =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = OBJ_EVENT_PAL_TAG_PLAYER_RED,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_Arrow,
    .images = sPicTable_Arrow,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

u32 CreateWarpArrowSprite(void)
{
    u32 spriteId;

    LoadFieldEffectPalette(&sSpriteTemplate_Arrow);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_Arrow, 0, 0, 0x52);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        sprite->oam.priority = 1;
        sprite->coordOffsetEnabled = TRUE;
        sprite->invisible = TRUE;
    }
    return spriteId;
}

void ShowWarpArrowSprite(u32 spriteId, u32 direction, s16 x, s16 y)
{
    s16 x2, y2;
    struct Sprite * sprite = &gSprites[spriteId];

    if (sprite->invisible || sprite->data[0] != x || sprite->data[1] != y)
    {
        SetSpritePosToMapCoords(x, y, &x2, &y2);
        sprite = &gSprites[spriteId];
        sprite->x = x2 + 8;
        sprite->y = y2 + 8;
        sprite->invisible = FALSE;
        sprite->data[0] = x;
        sprite->data[1] = y;
        StartSpriteAnim(sprite, direction - 1);
    }
}

//////////////////////
// WATER REFLECTION //
//////////////////////

static void SpriteCB_UpdateObjectReflectionSprite(struct Sprite * sprite);
static void LoadObjectReflectionPalette(struct ObjectEvent * objectEvent, struct Sprite * sprite);
static void LoadSpecialReflectionPalette(struct Sprite *sprite);

static const union AffineAnimCmd sAffineAnim_ReflectionDistortion_0[] =
{
    AFFINEANIMCMD_FRAME(-256, 256, 128, 0),
    AFFINEANIMCMD_FRAME(1, 0, 0, 4),
    AFFINEANIMCMD_FRAME(0, 0, 0, 8),
    AFFINEANIMCMD_FRAME(-1, 0, 0, 4),
    AFFINEANIMCMD_FRAME(0, 0, 0, 8),
    AFFINEANIMCMD_FRAME(-1, 0, 0, 4),
    AFFINEANIMCMD_FRAME(0, 0, 0, 8),
    AFFINEANIMCMD_FRAME(1, 0, 0, 4),
    AFFINEANIMCMD_FRAME(0, 0, 0, 8),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd sAffineAnim_ReflectionDistortion_1[] =
{
    AFFINEANIMCMD_FRAME(256, 256, 128, 0),
    AFFINEANIMCMD_FRAME(-1, 0, 0, 4),
    AFFINEANIMCMD_FRAME(0, 0, 0, 8),
    AFFINEANIMCMD_FRAME(1, 0, 0, 4),
    AFFINEANIMCMD_FRAME(0, 0, 0, 8),
    AFFINEANIMCMD_FRAME(1, 0, 0, 4),
    AFFINEANIMCMD_FRAME(0, 0, 0, 8),
    AFFINEANIMCMD_FRAME(-1, 0, 0, 4),
    AFFINEANIMCMD_FRAME(0, 0, 0, 8),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd *const sAffineAnims_ReflectionDistortion[] =
{
    sAffineAnim_ReflectionDistortion_0,
    sAffineAnim_ReflectionDistortion_1,
};

static const struct SpriteTemplate sSpriteTemplate_ReflectionDistortion =
{
    .tileTag = 0x0,
    .paletteTag = SPRITE_INVALID_TAG,
    .oam = &gDummyOamData,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_ReflectionDistortion,
    .callback = SpriteCallbackDummy,
};

static const u8 sBridgeReflectionVerticalOffsets[] = {
    [BRIDGE_TYPE_POND_LOW - 1] = 12,
    [BRIDGE_TYPE_POND_MED - 1] = 28,
    [BRIDGE_TYPE_POND_HIGH - 1] = 44
};

static void CreateReflectionSprite(u32 affineAnimNum)
{
    u32 spriteId = CreateSpriteAtEnd(&sSpriteTemplate_ReflectionDistortion, 0, 0, 31);
    gSprites[spriteId].oam.affineMode = ST_OAM_AFFINE_NORMAL;
    InitSpriteAffineAnim(&gSprites[spriteId]);
    StartSpriteAffineAnim(&gSprites[spriteId], affineAnimNum);
    gSprites[spriteId].invisible = TRUE;
}

void CreateReflectionEffectSprites(void)
{
    CreateReflectionSprite(0);
    CreateReflectionSprite(1);
}

#define sReflectionObjEventId      data[0]
#define sReflectionObjEventLocalId data[1]
#define sReflectionVerticalOffset  data[2]
#define sIsStillReflection         data[7]

void SetUpReflection(struct ObjectEvent * objectEvent, struct Sprite * sprite, bool32 stillReflection)
{
    struct Sprite * reflectionSprite = &gSprites[CreateCopySpriteAt(sprite, sprite->x, sprite->y, 0x98)];

    reflectionSprite->callback = SpriteCB_UpdateObjectReflectionSprite;
    reflectionSprite->oam.priority = 3;
    reflectionSprite->usingSheet = TRUE;
    reflectionSprite->anims = gDummySpriteAnimTable;
    StartSpriteAnim(reflectionSprite, 0);
    reflectionSprite->affineAnims = gDummySpriteAffineAnimTable;
    reflectionSprite->affineAnimBeginning = TRUE;
    reflectionSprite->subspriteMode = SUBSPRITES_OFF;
    reflectionSprite->sReflectionObjEventId = sprite->data[0];
    reflectionSprite->sReflectionObjEventLocalId = objectEvent->localId;
    reflectionSprite->sIsStillReflection = stillReflection;
    LoadObjectReflectionPalette(objectEvent, reflectionSprite);

    if (!stillReflection)
        reflectionSprite->oam.affineMode = ST_OAM_AFFINE_NORMAL;
}

static void LoadObjectReflectionPalette(struct ObjectEvent * objectEvent, struct Sprite * sprite)
{
    u32 bridgeType;
    
    if (!GetObjectEventGraphicsInfo(objectEvent->graphicsId)->disableReflectionPaletteLoad && ((bridgeType = MetatileBehavior_GetBridgeType(objectEvent->previousMetatileBehavior))
    || (bridgeType = MetatileBehavior_GetBridgeType(objectEvent->currentMetatileBehavior))))
    {
        // When walking on a bridge high above water (Route 120), the reflection is a solid dark blue color.
        // This is so the sprite blends in with the dark water metatile underneath the bridge.
        sprite->sReflectionVerticalOffset = sBridgeReflectionVerticalOffsets[bridgeType - 1];
        LoadObjectEventPalette(OBJ_EVENT_PAL_TAG_BRIDGE_REFLECTION);
        sprite->oam.paletteNum = IndexOfSpritePaletteTag(OBJ_EVENT_PAL_TAG_BRIDGE_REFLECTION);
        UpdateSpritePaletteWithWeather(sprite->oam.paletteNum);
    }
    else
    {
        sprite->sReflectionVerticalOffset = 0;
        LoadSpecialReflectionPalette(sprite);
    }
}

static void LoadSpecialReflectionPalette(struct Sprite *sprite)
{
    struct SpritePalette reflectionPalette;
    
    CpuCopy16(&gPlttBufferUnfaded[0x100 + sprite->oam.paletteNum * 16], gReflectionPaletteBuffer, 32);
    TintPalette_CustomTone(gReflectionPaletteBuffer, 16, Q_8_8(1.0), Q_8_8(1.0), Q_8_8(3.5));
    
    reflectionPalette.data = gReflectionPaletteBuffer;
    reflectionPalette.tag = GetSpritePaletteTagByPaletteNum(sprite->oam.paletteNum) + 0x1000;
    LoadSpritePalette(&reflectionPalette);
    
    sprite->oam.paletteNum = IndexOfSpritePaletteTag(reflectionPalette.tag);
    UpdateSpritePaletteWithWeather(sprite->oam.paletteNum);
}

static void SpriteCB_UpdateObjectReflectionSprite(struct Sprite * reflectionSprite)
{
    struct ObjectEvent * objectEvent = &gObjectEvents[reflectionSprite->sReflectionObjEventId];

    if (!objectEvent->active || !objectEvent->hasReflection || objectEvent->localId != reflectionSprite->sReflectionObjEventLocalId)
        reflectionSprite->inUse = FALSE;
    else
    {
        struct Sprite * mainSprite = &gSprites[objectEvent->spriteId];
        
        reflectionSprite->oam.shape = mainSprite->oam.shape;
        reflectionSprite->oam.size = mainSprite->oam.size;
        reflectionSprite->oam.matrixNum = mainSprite->oam.matrixNum | ST_OAM_VFLIP;
        reflectionSprite->oam.tileNum = mainSprite->oam.tileNum;
        reflectionSprite->subspriteTables = mainSprite->subspriteTables;
        reflectionSprite->subspriteTableNum = mainSprite->subspriteTableNum;
        reflectionSprite->invisible = mainSprite->invisible;
        reflectionSprite->x = mainSprite->x;
        reflectionSprite->y = mainSprite->y + (GetObjectEventGraphicsInfo(objectEvent->graphicsId)->height - 2) + reflectionSprite->sReflectionVerticalOffset;
        reflectionSprite->centerToCornerVecX = mainSprite->centerToCornerVecX;
        reflectionSprite->centerToCornerVecY = mainSprite->centerToCornerVecY;
        reflectionSprite->x2 = mainSprite->x2;
        reflectionSprite->y2 = -mainSprite->y2;
        reflectionSprite->coordOffsetEnabled = mainSprite->coordOffsetEnabled;

        // Check if the reflection is not still.
        if (!reflectionSprite->sIsStillReflection)
        {
            // Sets the reflection sprite's rot/scale matrix to the appropriate
            // matrix based on whether or not the main sprite is horizontally flipped.
            // If the sprite is facing to the east, then it is flipped, and its matrixNum is 8.
            reflectionSprite->oam.matrixNum = 0;
            if (mainSprite->oam.matrixNum & ST_OAM_HFLIP)
                reflectionSprite->oam.matrixNum = 1;
        }
    }
}

#undef sReflectionObjEventId
#undef sReflectionObjEventLocalId
#undef sReflectionVerticalOffset
#undef sIsStillReflection

///////////////////////////////
// TELEPORT/ESCAPE ROPE SPIN //
///////////////////////////////

static void Task_DoTeleportFieldEffect(u32 taskId);
static void TeleportFieldEffectTask1(struct Task * task);
static void TeleportFieldEffectTask2(struct Task * task);
static void TeleportFieldEffectTask3(struct Task * task);
static void TeleportFieldEffectTask4(struct Task * task);
static void FieldCallback_TeleportIn(void);
static void Task_DoTeleportInFieldEffect(u32 taskId);
static void TeleportInFieldEffectTask1(struct Task * task);
static void TeleportInFieldEffectTask2(struct Task * task);
static void TeleportInFieldEffectTask3(struct Task * task);
static void Task_DoEscapeRopeFieldEffect(u32 taskId);
static void EscapeRopeFieldEffect_Step0(struct Task * task);
static void EscapeRopeFieldEffect_Step1(struct Task * task);
static void FieldCallback_EscapeRopeExit(void);
static void Task_DoEscapeRopeExitFieldEffect(u32 taskId);
static void EscapeRopeExitFieldEffect_Step0(struct Task * task);
static void EscapeRopeExitFieldEffect_Step1(struct Task * task);

static void (*const sTeleportEffectFuncs[])(struct Task * ) = {
    TeleportFieldEffectTask1,
    TeleportFieldEffectTask2,
    TeleportFieldEffectTask3,
    TeleportFieldEffectTask4
};

static void (*const sTeleportInEffectFuncs[])(struct Task * ) = {
    TeleportInFieldEffectTask1,
    TeleportInFieldEffectTask2,
    TeleportInFieldEffectTask3
};

static void (*const sEscapeRopeFieldEffectFuncs[])(struct Task * task) = {
    EscapeRopeFieldEffect_Step0,
    EscapeRopeFieldEffect_Step1
};

static void (*const sEscapeRopeExitEffectFuncs[])(struct Task * task) = {
    EscapeRopeExitFieldEffect_Step0,
    EscapeRopeExitFieldEffect_Step1
};

static const u8 sSpinDirections[] = {
    [DIR_NONE]  = DIR_SOUTH,
    [DIR_SOUTH] = DIR_WEST,
    [DIR_WEST]  = DIR_NORTH,
    [DIR_NORTH] = DIR_EAST,
    [DIR_EAST]  = DIR_SOUTH,
};

void CreateTeleportFieldEffectTask(void)
{
    CreateTask(Task_DoTeleportFieldEffect, 0);
}

static void Task_DoTeleportFieldEffect(u32 taskId)
{
    sTeleportEffectFuncs[gTasks[taskId].data[0]](&gTasks[taskId]);
}

static void TeleportFieldEffectTask1(struct Task * task)
{
    ScriptContext2_Enable();
    FreezeObjectEvents();
    CameraObjectReset2();
    task->data[15] = GetPlayerFacingDirection();
    task->data[0]++;
}

static void TeleportFieldEffectTask2(struct Task * task)
{
    struct ObjectEvent * objectEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
    
    if (task->data[1] == 0 || (--task->data[1]) == 0)
    {
        ObjectEventTurn(objectEvent, sSpinDirections[objectEvent->facingDirection]);
        task->data[1] = 8;
        task->data[2]++;
    }
    
    if (task->data[2] > 7 && task->data[15] == objectEvent->facingDirection)
    {
        task->data[0]++;
        task->data[1] = 4;
        task->data[2] = 8;
        task->data[3] = 1;
        PlaySE(SE_WARP_IN);
    }
}

static void TeleportFieldEffectTask3(struct Task * task)
{
    struct ObjectEvent * objectEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
    struct Sprite * sprite = &gSprites[gPlayerAvatar.spriteId];
    
    if ((--task->data[1]) <= 0)
    {
        task->data[1] = 4;
        ObjectEventTurn(objectEvent, sSpinDirections[objectEvent->facingDirection]);
    }
    sprite->y -= task->data[3];
    task->data[4] += task->data[3];
    
    if ((--task->data[2]) <= 0 && (task->data[2] = 4, task->data[3] < 8))
        task->data[3] <<= 1;
    
    if (task->data[4] > 8 && (sprite->oam.priority = 1, sprite->subspriteMode != SUBSPRITES_OFF))
        sprite->subspriteMode = SUBSPRITES_IGNORE_PRIORITY;
    
    if (task->data[4] >= 0xa8)
    {
        task->data[0]++;
        TryFadeOutOldMapMusic();
        WarpFadeOutScreen();
    }
}

static void TeleportFieldEffectTask4(struct Task * task)
{
    if (!gPaletteFade.active && IsNotWaitingForBGMStop())
    {
        SetWarpDestinationToLastHealLocation();
        WarpIntoMap();
        SetMainCallback2(CB2_LoadMap);
        gFieldCallback = FieldCallback_TeleportIn;
        DestroyTask(FindTaskIdByFunc(Task_DoTeleportFieldEffect));
    }
}

static void FieldCallback_TeleportIn(void)
{
    Overworld_PlaySpecialMapMusic();
    WarpFadeInScreen();
    ScriptContext2_Enable();
    FreezeObjectEvents();
    gFieldCallback = NULL;
    gObjectEvents[gPlayerAvatar.objectEventId].invisible = TRUE;
    CameraObjectReset2();
    CreateTask(Task_DoTeleportInFieldEffect, 0);
}

static void Task_DoTeleportInFieldEffect(u32 taskId)
{
    sTeleportInEffectFuncs[gTasks[taskId].data[0]](&gTasks[taskId]);
}

static void TeleportInFieldEffectTask1(struct Task * task)
{
    struct Sprite * sprite;
    s16 centerToCornerVecY;
    
    if (IsWeatherNotFadingIn())
    {
        sprite = &gSprites[gPlayerAvatar.spriteId];
        centerToCornerVecY = -(sprite->centerToCornerVecY << 1);
        sprite->y2 = -(sprite->y + sprite->centerToCornerVecY + gSpriteCoordOffsetY + centerToCornerVecY);
        gObjectEvents[gPlayerAvatar.objectEventId].invisible = FALSE;
        task->data[0]++;
        task->data[1] = 8;
        task->data[2] = 1;
        task->data[14] = sprite->subspriteMode;
        task->data[15] = GetPlayerFacingDirection();
        PlaySE(SE_WARP_IN);
    }
}

static void TeleportInFieldEffectTask2(struct Task * task)
{
    struct ObjectEvent * objectEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
    struct Sprite * sprite = &gSprites[gPlayerAvatar.spriteId];
    
    if ((sprite->y2 += task->data[1]) >= -8)
    {
        if (task->data[13] == 0)
        {
            task->data[13]++;
            objectEvent->triggerGroundEffectsOnMove = TRUE;
            sprite->subspriteMode = task->data[14];
        }
    }
    else
    {
        sprite->oam.priority = 1;
        
        if (sprite->subspriteMode != SUBSPRITES_OFF)
            sprite->subspriteMode = SUBSPRITES_IGNORE_PRIORITY;
    }
    
    if (sprite->y2 >= -0x30 && task->data[1] > 1 && !(sprite->y2 & 1))
        task->data[1]--;
    
    if ((--task->data[2]) == 0)
    {
        task->data[2] = 4;
        ObjectEventTurn(objectEvent, sSpinDirections[objectEvent->facingDirection]);
    }
    
    if (sprite->y2 >= 0)
    {
        sprite->y2 = 0;
        task->data[0]++;
        task->data[1] = 1;
        task->data[2] = 0;
    }
}

static void TeleportInFieldEffectTask3(struct Task * task)
{
    struct ObjectEvent * objectEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
    
    if ((--task->data[1]) == 0)
    {
        ObjectEventTurn(objectEvent, sSpinDirections[objectEvent->facingDirection]);
        task->data[1] = 8;
        
        if ((++task->data[2]) > 4 && task->data[14] == objectEvent->facingDirection)
        {
            ScriptContext2_Disable();
            CameraObjectReset1();
            UnfreezeObjectEvents();
            DestroyTask(FindTaskIdByFunc(Task_DoTeleportInFieldEffect));
        }
    }
}

void StartEscapeRopeFieldEffect(void)
{
    ScriptContext2_Enable();
    FreezeObjectEvents();
    CreateTask(Task_DoEscapeRopeFieldEffect, 80);
}

static void Task_DoEscapeRopeFieldEffect(u32 taskId)
{
    sEscapeRopeFieldEffectFuncs[gTasks[taskId].data[0]](&gTasks[taskId]);
}

static void EscapeRopeFieldEffect_Step0(struct Task * task)
{
    task->data[0]++;
    task->data[13] = 64;
    task->data[14] = GetPlayerFacingDirection();
    task->data[15] = 0;
}

static u32 sub_808576C(struct ObjectEvent * playerObj, s16 *delay_p, s16 *stage_p)
{
    if (!ObjectEventIsMovementOverridden(playerObj) || ObjectEventClearHeldMovementIfFinished(playerObj))
    {
        if (*delay_p != 0 && --(*delay_p) != 0)
            return playerObj->facingDirection;
        
        ObjectEventSetHeldMovement(playerObj, GetFaceDirectionMovementAction(sSpinDirections[playerObj->facingDirection]));
        
        if (*stage_p < 12)
            (*stage_p)++;
        *delay_p = 12 >> (*stage_p); // 12 >> 4 = 0
        
        return sSpinDirections[playerObj->facingDirection];
    }
    return playerObj->facingDirection;
}

static bool32 sub_80857F0(struct ObjectEvent * playerObj, s16 *state_p, s16 *y_p)
{
    struct Sprite * sprite = &gSprites[playerObj->spriteId];
    
    switch (*state_p)
    {
    case 0:
        CameraObjectReset2();
        (*state_p)++;
        // fallthrough
    case 1:
        sprite->y2 -= 8;
        (*y_p) -= 8;
        if (*y_p <= -16)
        {
            playerObj->fixedPriority = TRUE;
            sprite->oam.priority = 1;
            sprite->subpriority = 0;
            sprite->subspriteMode = SUBSPRITES_OFF;
            (*state_p)++;
        }
        break;
    case 2:
        sprite->y2 -= 8;
        (*y_p) -= 8;
        if (*y_p <= -88)
        {
            (*state_p)++;
            return FALSE;
        }
        break;
    case 3:
        return FALSE;
    }
    return TRUE;
}

static void EscapeRopeFieldEffect_Step1(struct Task * task)
{
    struct ObjectEvent * playerObj = &gObjectEvents[gPlayerAvatar.objectEventId];
    s16 *data = task->data;
    
    sub_808576C(playerObj, &task->data[1], &task->data[2]);
    
    if (data[3] < 60)
    {
        if (++data[3] == 20)
            PlaySE(SE_WARP_IN);
    }
    else if (data[4] == 0 && !sub_80857F0(playerObj, &task->data[5], &task->data[6]))
    {
        TryFadeOutOldMapMusic();
        WarpFadeOutScreen();
        data[4] = 1;
    }
    
    if (data[4] == 1 && !gPaletteFade.active && IsNotWaitingForBGMStop())
    {
        SetObjectEventDirection(playerObj, task->data[15]);
        SetWarpDestinationToEscapeWarp();
        WarpIntoMap();
        gFieldCallback = FieldCallback_EscapeRopeExit;
        SetMainCallback2(CB2_LoadMap);
        DestroyTask(FindTaskIdByFunc(Task_DoEscapeRopeFieldEffect));
    }
}

static void FieldCallback_EscapeRopeExit(void)
{
    Overworld_PlaySpecialMapMusic();
    WarpFadeInScreen();
    ScriptContext2_Enable();
    FreezeObjectEvents();
    gFieldCallback = NULL;
    gObjectEvents[gPlayerAvatar.objectEventId].invisible = TRUE;
    CreateTask(Task_DoEscapeRopeExitFieldEffect, 0);
}

static void Task_DoEscapeRopeExitFieldEffect(u32 taskId)
{
    sEscapeRopeExitEffectFuncs[gTasks[taskId].data[0]](&gTasks[taskId]);
}

static void EscapeRopeExitFieldEffect_Step0(struct Task * task)
{
    if (IsWeatherNotFadingIn())
    {
        PlaySE(SE_WARP_OUT);
        task->data[15] = GetPlayerFacingDirection();
        task->data[0]++;
    }
}

static bool32 sub_80858A4(struct ObjectEvent * playerObj, s16 *state_p, s16 *y_p, s16 *priority_p, s16 *subpriority_p, s16 *subspriteMode_p)
{
    struct Sprite * sprite = &gSprites[playerObj->spriteId];
    
    switch (*state_p)
    {
    case 0:
        CameraObjectReset2();
        *y_p = -88;
        sprite->y2 -= 88;
        *priority_p = sprite->oam.priority;
        *subpriority_p = sprite->subpriority;
        *subspriteMode_p = sprite->subspriteMode;
        playerObj->fixedPriority = TRUE;
        sprite->oam.priority = 1;
        sprite->subpriority = 0;
        sprite->subspriteMode = SUBSPRITES_OFF;
        (*state_p)++;
        // fallthrough
    case 1:
        sprite->y2 += 4;
        (*y_p) += 4;
        if (*y_p >= -16)
        {
            sprite->oam.priority = *priority_p;
            sprite->subpriority = *subpriority_p;
            sprite->subspriteMode = *subspriteMode_p;
            (*state_p)++;
        }
        break;
    case 2:
        sprite->y2 += 4;
        (*y_p) += 4;
        if (*y_p >= 0)
        {
            PlaySE(SE_CLICK);
            CameraObjectReset1();
            (*state_p)++;
            return FALSE;
        }
        break;
    case 3:
        return FALSE;
    }
    return TRUE;
}

static void EscapeRopeExitFieldEffect_Step1(struct Task * task)
{
    s16 *data = task->data;
    struct ObjectEvent * playerObj = &gObjectEvents[gPlayerAvatar.objectEventId];
    bool32 finished = sub_80858A4(playerObj, &data[1], &data[2], &data[3], &data[4], &data[5]);
    
    playerObj->invisible = FALSE;
    
    if (data[6] < 8)
        data[6]++;
    else if (data[7] == 0)
    {
        data[6]++;
        data[8] = sub_808576C(playerObj, &data[9], &data[10]);
        
        if (data[6] >= 50 && data[8] == data[15])
            data[7] = 1;
    }
    
    if (!finished && data[8] == data[15] && ObjectEventCheckHeldMovementStatus(playerObj))
    {
        playerObj->invisible = FALSE;
        playerObj->fixedPriority = FALSE;
        ScriptContext2_Disable();
        UnfreezeObjectEvents();
        DestroyTask(FindTaskIdByFunc(Task_DoEscapeRopeExitFieldEffect));
    }
}

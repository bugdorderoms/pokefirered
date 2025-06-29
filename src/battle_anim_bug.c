#include "battle_anim.h"
#include "global.h"
#include "gpu_regs.h"
#include "scanline_effect.h"
#include "trig.h"

static void AnimMegahornHorn(struct Sprite *sprite);
static void AnimTranslateWebThread(struct Sprite *sprite);
static void AnimTranslateWebThread_Step(struct Sprite *sprite);
static void AnimStringWrap_Step(struct Sprite *sprite);
static void AnimSpiderWeb(struct Sprite *sprite);
static void AnimSpiderWeb_Step(struct Sprite *sprite);
static void AnimTranslateStinger(struct Sprite *sprite);
static void AnimQuiverDanceOrb(struct Sprite *sprite);
static void AnimQuiverDanceOrb_Step(struct Sprite *sprite);
static void AnimTask_QuiverDanceWaver_Step(u32 taskId);
static void UpdateQuiverDanceScanlineEffect(struct Task *task);

static const union AffineAnimCmd sAffineAnim_MegahornHorn_0[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 30, 0),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_MegahornHorn_1[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, -99, 0),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_MegahornHorn_2[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 94, 0),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims_MegahornHorn[] =
{
    sAffineAnim_MegahornHorn_0,
    sAffineAnim_MegahornHorn_1,
    sAffineAnim_MegahornHorn_2,
};

const struct SpriteTemplate gMegahornHornSpriteTemplate =
{
    .tileTag = ANIM_TAG_HORN_HIT_2,
    .paletteTag = ANIM_TAG_HORN_HIT_2,
    .oam = &gOamData_AffineDouble_ObjNormal_32x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_MegahornHorn,
    .callback = AnimMegahornHorn,
};

const struct SpriteTemplate gWebThreadSpriteTemplate =
{
    .tileTag = ANIM_TAG_WEB_THREAD,
    .paletteTag = ANIM_TAG_WEB_THREAD,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimTranslateWebThread,
};

const struct SpriteTemplate gStringWrapSpriteTemplate =
{
    .tileTag = ANIM_TAG_STRING,
    .paletteTag = ANIM_TAG_STRING,
    .oam = &gOamData_AffineOff_ObjNormal_64x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimStringWrap,
};

static const union AffineAnimCmd sAffineAnim_SpiderWeb[] =
{
    AFFINEANIMCMD_FRAME(0x10, 0x10, 0, 0),
    AFFINEANIMCMD_FRAME(0x6, 0x6, 0, 1),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd *const sAffineAnims_SpiderWeb[] =
{
    sAffineAnim_SpiderWeb,
};

const struct SpriteTemplate gSpiderWebSpriteTemplate =
{
    .tileTag = ANIM_TAG_SPIDER_WEB,
    .paletteTag = ANIM_TAG_SPIDER_WEB,
    .oam = &gOamData_AffineDouble_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_SpiderWeb,
    .callback = AnimSpiderWeb,
};

const struct SpriteTemplate gLinearStingerSpriteTemplate =
{
    .tileTag = ANIM_TAG_NEEDLE,
    .paletteTag = ANIM_TAG_NEEDLE,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimTranslateStinger,
};

const struct SpriteTemplate gPinMissileSpriteTemplate =
{
    .tileTag = ANIM_TAG_NEEDLE,
    .paletteTag = ANIM_TAG_NEEDLE,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimMissileArc,
};

static const union AffineAnimCmd sSilverWindBigSparkAffineAnimCmds[] = 
{
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_FRAME(0, 0, -10, 1),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd sSilverWindMediumSparkAffineAnimCmds[] = 
{
    AFFINEANIMCMD_FRAME(192, 192, 0, 0),
    AFFINEANIMCMD_FRAME(0, 0, -12, 1),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd sSilverWindSmallSparkAffineAnimCmds[] = 
{
    AFFINEANIMCMD_FRAME(143, 143, 0, 0),
    AFFINEANIMCMD_FRAME(0, 0, -15, 1),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd *const sSilverWindBigSparkAffineAnimTable[] = 
{
    sSilverWindBigSparkAffineAnimCmds,
};

static const union AffineAnimCmd *const sSilverWindMediumSparkAffineAnimTable[] = 
{
    sSilverWindMediumSparkAffineAnimCmds,
};

static const union AffineAnimCmd *const sSilverWindSmallSparkAffineAnimTable[] = 
{
    sSilverWindSmallSparkAffineAnimCmds,
};

const struct SpriteTemplate gSilverWindBigSparkSpriteTemplate =    
{
    .tileTag = ANIM_TAG_SPARKLE_6,
    .paletteTag = ANIM_TAG_SPARKLE_6,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sSilverWindBigSparkAffineAnimTable,
    .callback = AnimFlyingParticle,
};

const struct SpriteTemplate gSilverWindMediumSparkSpriteTemplate =    
{
    .tileTag = ANIM_TAG_SPARKLE_6,
    .paletteTag = ANIM_TAG_SPARKLE_6,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sSilverWindMediumSparkAffineAnimTable,
    .callback = AnimFlyingParticle,
};

const struct SpriteTemplate gSilverWindSmallSparkSpriteTemplate =
{
    .tileTag = ANIM_TAG_SPARKLE_6,
    .paletteTag = ANIM_TAG_SPARKLE_6,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sSilverWindSmallSparkAffineAnimTable,
    .callback = AnimFlyingParticle,
};

const struct SpriteTemplate gSignalBeamRedOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_GLOWY_RED_ORB,
    .paletteTag = ANIM_TAG_GLOWY_RED_ORB,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimToTargetInSinWave,
};

const struct SpriteTemplate gSignalBeamGreenOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_GLOWY_GREEN_ORB,
    .paletteTag = ANIM_TAG_GLOWY_GREEN_ORB,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimToTargetInSinWave,
};

const struct SpriteTemplate gUTurnBallInSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_BUBBLES,
    .paletteTag = ANIM_TAG_RAZOR_LEAF,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimShadowBall,
};

const struct SpriteTemplate gUTurnBallOutSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_BUBBLES,
    .paletteTag = ANIM_TAG_RAZOR_LEAF,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimAbsorptionOrb,
};

const struct SpriteTemplate gQuiverDanceOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_HOLLOW_ORB,
    .paletteTag = ANIM_TAG_CIRCLE_OF_LIGHT,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimQuiverDanceOrb,
};

// Animates the horn sprite in MOVE_MEGAHORN's anim.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: final x pixel offset
// arg 3: final y pixel offset
// arg 4: speed
static void AnimMegahornHorn(struct Sprite *sprite)
{
    if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_PLAYER)
    {
        StartSpriteAffineAnim(sprite, 1);
        
        gBattleAnimArgs[0] = -gBattleAnimArgs[0];
        gBattleAnimArgs[1] = -gBattleAnimArgs[1];
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];
        gBattleAnimArgs[3] = -gBattleAnimArgs[3];
    }
    sprite->x = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X) + gBattleAnimArgs[0];
    sprite->y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[1];
    
    sprite->data[0] = gBattleAnimArgs[4];
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X) + gBattleAnimArgs[2];
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3];
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Creates a single web thread that travels from attacker to target.
// Used by MOVE_STRING_SHOT and MOVE_SPIDER_WEB in their first move phase.
// arg 0: x
// arg 1: y
// arg 2: speed
// arg 3: amplitude
// arg 4: if targets both opponents
static void AnimTranslateWebThread(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, TRUE);
    
    sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[1] = sprite->x;
    sprite->data[3] = sprite->y;
    
    if (!gBattleAnimArgs[4])
    {
        sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
        sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
    }
    else
        SetAverageBattlerPositions(gBattleAnimTarget, TRUE, &sprite->data[2], &sprite->data[4]);

    BattleAnim_InitLinearTranslationWithDuration(sprite);
    sprite->data[5] = gBattleAnimArgs[3];
    sprite->callback = AnimTranslateWebThread_Step;
}

static void AnimTranslateWebThread_Step(struct Sprite *sprite)
{
    if (AnimTranslateLinear(sprite))
        DestroyAnimSprite(sprite);
    else
    {
        sprite->x2 += Sin(sprite->data[6], sprite->data[5]);
        sprite->data[6] = (sprite->data[6] + 13) & 0xFF;
    }
}

// Animates the string wraps in the second phase of MOVE_STRING_SHOT and MOVE_SPIDER_WEB.
// arg 0: x
// arg 1: y
// arg 2: if targets both foes
void AnimStringWrap(struct Sprite *sprite)
{
    if (gBattleAnimArgs[2])
        SetAverageBattlerPositions(gBattleAnimTarget, FALSE, &sprite->x, &sprite->y);
    
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        sprite->x -= gBattleAnimArgs[0];
    else
        sprite->x += gBattleAnimArgs[0];
    
    sprite->y += gBattleAnimArgs[1];
    
    if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_PLAYER)
        sprite->y += 8;
    
    sprite->callback = AnimStringWrap_Step;
}

static void AnimStringWrap_Step(struct Sprite *sprite)
{
    if (++sprite->data[0] == 3)
    {
        sprite->data[0] = 0;
        sprite->invisible ^= TRUE;
    }
    
    if (++sprite->data[1] == 51)
        DestroyAnimSprite(sprite);
}

// Animates second part of MOVE_SPIDER_WEB's anim.
// No args.
static void AnimSpiderWeb(struct Sprite *sprite)
{
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16, 0));
    sprite->data[0] = 16;
    sprite->callback = AnimSpiderWeb_Step;
}

static void AnimSpiderWeb_Step(struct Sprite *sprite)
{
    if (sprite->data[2] < 20)
        ++sprite->data[2];
    else if (sprite->data[1]++ & 1)
    {
        --sprite->data[0];
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sprite->data[0], 16 - sprite->data[0]));

        if (sprite->data[0] == 0)
        {
            sprite->invisible = TRUE;
            sprite->callback = DestroyAnimSpriteAndDisableBlend;
        }
    }
}

// Translates a stinger sprite linearly to a destination location. The sprite is
// initially rotated so that it appears to be traveling in a straight line.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: target x pixel offset
// arg 3: target y pixel offset
// arg 4: duration
static void AnimTranslateStinger(struct Sprite *sprite)
{
    s16 lVarX, lVarY;
    u16 rot;

    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
    {
        gBattleAnimArgs[1] = -gBattleAnimArgs[1];
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];
        gBattleAnimArgs[3] = -gBattleAnimArgs[3];
    }
    
    if (IsBattlerAlly(gBattleAnimAttacker, gBattleAnimTarget))
    {
        if ((GetBattlerPosition(gBattleAnimTarget) & BIT_FLANK) == B_FLANK_LEFT)
        {
            gBattleAnimArgs[0] = -gBattleAnimArgs[0];
            gBattleAnimArgs[2] = -gBattleAnimArgs[2];
        }
    }
    InitSpritePosToAnimAttacker(sprite, TRUE);
    
    lVarX = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X) + gBattleAnimArgs[2];
    lVarY = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3];
    
    rot = ArcTan2Neg(lVarX - sprite->x, lVarY - sprite->y);
    rot += 0xC000;
    TrySetSpriteRotScale(sprite, FALSE, 0x100, 0x100, rot);
    
    sprite->data[0] = gBattleAnimArgs[4];
    sprite->data[2] = lVarX;
    sprite->data[4] = lVarY;
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Rotates sprite and moves it in an arc, so that it appears like a missle or arrow traveling.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: target x pixel offset
// arg 3: target y pixel offset
// arg 4: duration
// arg 5: wave amplitude
void AnimMissileArc(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, TRUE);
    
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];
    
    sprite->data[0] = gBattleAnimArgs[4];
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X) + gBattleAnimArgs[2];
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3];
    sprite->data[5] = gBattleAnimArgs[5];
    InitAnimArcTranslation(sprite);
    sprite->callback = AnimMissileArcStep;
    sprite->invisible = TRUE;
}

void AnimMissileArcStep(struct Sprite *sprite)
{
    sprite->invisible = FALSE;

    if (TranslateAnimHorizontalArc(sprite))
        DestroyAnimSprite(sprite);
    else
    {
        u32 i;
        s16 tempData[8];
        u16 *data = sprite->data;
        u16 x1 = sprite->x;
        s16 x2 = sprite->x2;
        u16 y1 = sprite->y;
        s16 y2 = sprite->y2;

        for (i = 0; i < 8; ++i)
            tempData[i] = data[i];
        
        x2 += x1;
        y2 += y1;
        
        if (!TranslateAnimHorizontalArc(sprite))
        {
            u16 rotation = ArcTan2Neg(sprite->x + sprite->x2 - x2, sprite->y + sprite->y2 - y2);

            rotation += 0xC000;
            TrySetSpriteRotScale(sprite, FALSE, 0x100, 0x100, rotation);
            
            for (i = 0; i < 8; ++i)
                data[i] = tempData[i];
        }
    }
}

// Returns where Fury Cutter hits hight.
// No args.
void AnimTask_IsFuryCutterHitRight(u32 taskId)
{
    gBattleAnimArgs[ARG_RET_ID] = (gAnimDisableStructPtr->furyCutterCounter & 1);
    DestroyAnimVisualTask(taskId);
}

// Returns the Fury Cutter counter.
// No args.
void AnimTask_GetFuryCutterHitCount(u32 taskId)
{
    gBattleAnimArgs[ARG_RET_ID] = gAnimDisableStructPtr->furyCutterCounter;
    DestroyAnimVisualTask(taskId);
}

// Wavers the attacker back and forth. Progressing vertical wave of scanline shifts. Used by Quiver Dance.
// No args.
void AnimTask_QuiverDanceWaver(u32 taskId)
{
    struct ScanlineEffectParams sp;
    struct Task *task = &gTasks[taskId];
    u32 i, r1;

    if (GetBattlerSpriteBGPriorityRank(gBattleAnimAttacker) == 1)
    {
        sp.dmaDest = &REG_BG1HOFS;
        task->data[2] = gBattle_BG1_X;
    }
    else
    {
        sp.dmaDest = &REG_BG2HOFS;
        task->data[2] = gBattle_BG2_X;
    }
    sp.dmaControl = SCANLINE_EFFECT_DMACNT_16BIT;
    sp.initState = 1;

    r1 = GetBattlerYCoordWithElevation(gBattleAnimAttacker);
    
    task->data[3] = r1 - 32;
    task->data[4] = r1 + 32;
    
    if (task->data[3] < 0)
        task->data[3] = 0;
    
    for (i = task->data[3]; i <= task->data[4]; ++i)
    {
        gScanlineEffectRegBuffers[0][i] = task->data[2];
        gScanlineEffectRegBuffers[1][i] = task->data[2];
    }
    ScanlineEffect_SetParams(sp);
    task->func = AnimTask_QuiverDanceWaver_Step;
}

static void AnimTask_QuiverDanceWaver_Step(u32 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
        case 0:
            if (++task->data[7] > 1)
            {
                task->data[7] = 0;
                
                if (++task->data[6] == 3)
                    ++task->data[0];
            }
            UpdateQuiverDanceScanlineEffect(task);
            break;
        case 1:
            if (++task->data[1] > 0x3C)
                ++task->data[0];
            
            UpdateQuiverDanceScanlineEffect(task);
            break;
        case 2:
            if (++task->data[7] > 1)
            {
                task->data[7] = 0;
                
                if (--task->data[6] == 0)
                    ++task->data[0];
            }
            UpdateQuiverDanceScanlineEffect(task);
            break;
        case 3:
            gScanlineEffect.state = 3;
            ++task->data[0];
            break;
        case 4:
            DestroyAnimVisualTask(taskId);
            break;
    }
}

static void UpdateQuiverDanceScanlineEffect(struct Task *task)
{
    u32 i, r3 = task->data[5];

    for (i = task->data[3]; i <= task->data[4]; ++i)
    {
        gScanlineEffectRegBuffers[gScanlineEffect.srcBuffer][i] = ((gSineTable[r3] * task->data[6]) >> 7) + task->data[2];
        r3 = (r3 + 8) & 0xFF;
    }
    task->data[5] = (task->data[5] + 9) & 0xFF;
}

// Animates the circular orbs in Quiver Dance anim.
// arg 0: wave index
static void AnimQuiverDanceOrb(struct Sprite *sprite)
{
    u32 height, width;

    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
    
    sprite->data[5] = 1;
    sprite->data[6] = gBattleAnimArgs[0];
    
    height = GetBattlerSpriteCoordAttr(gBattleAnimAttacker, BATTLER_COORD_ATTR_HEIGHT);
    width = GetBattlerSpriteCoordAttr(gBattleAnimAttacker, BATTLER_COORD_ATTR_WIDTH);
    
    if (height > width)
        sprite->data[7] = height / 2;
    else
        sprite->data[7] = width / 2;
    
    sprite->x2 = Cos(sprite->data[6], sprite->data[7]);
    sprite->y2 = Sin(sprite->data[6], sprite->data[7]);
    
    sprite->callback = AnimQuiverDanceOrb_Step;
}

static void AnimQuiverDanceOrb_Step(struct Sprite *sprite)
{
    switch (sprite->data[0])
    {
        case 0:
            sprite->data[6] = (sprite->data[6] - sprite->data[5]) & 0xFF;
            
            sprite->x2 = Cos(sprite->data[6], sprite->data[7]);
            sprite->y2 = Sin(sprite->data[6], sprite->data[7]);
            
            if (++sprite->data[4] > 5)
            {
                sprite->data[4] = 0;
                
                if (sprite->data[5] <= 15 && ++sprite->data[5] > 15)
                    sprite->data[5] = 16;
            }
            
            if (++sprite->data[3] > 0x3C)
            {
                sprite->data[3] = 0;
                ++sprite->data[0];
            }
            break;
        case 1:
            sprite->data[6] = (sprite->data[6] - sprite->data[5]) & 0xFF;
            
            if (sprite->data[7] <= 0x95 && (sprite->data[7] += 8) > 0x95)
                sprite->data[7] = 0x96;
            
            sprite->x2 = Cos(sprite->data[6], sprite->data[7]);
            sprite->y2 = Sin(sprite->data[6], sprite->data[7]);
            
            if (++sprite->data[4] > 5)
            {
                sprite->data[4] = 0;
                
                if (sprite->data[5] <= 15 && ++sprite->data[5] > 15)
                    sprite->data[5] = 16;
            }
            
            if (++sprite->data[3] > 20)
                DestroyAnimSprite(sprite);
            break;
    }
}

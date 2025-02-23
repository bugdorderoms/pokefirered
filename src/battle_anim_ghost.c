#include "global.h"
#include "gflib.h"
#include "battle_anim.h"
#include "scanline_effect.h"
#include "graphics.h"
#include "trig.h"
#include "util.h"
#include "decompress.h"
#include "constants/songs.h"

static void AnimConfuseRayBallBounce(struct Sprite *sprite);
static void AnimConfuseRayBallBounce_Step(struct Sprite *sprite);
static void AnimConfuseRayBallBounce_Step2(struct Sprite *sprite);
static void UpdateConfuseRayBallBlend(struct Sprite *sprite);
static void AnimConfuseRayBallSpiral(struct Sprite *sprite);
static void AnimConfuseRayBallSpiral_Step(struct Sprite *sprite);
static void AnimTask_NightShadeClone_Step(u8 taskId);
static void AnimTask_NightShadeClone_Step2(u8 taskId);
static void AnimShadowBallStep(struct Sprite *sprite);
static void AnimLick(struct Sprite *sprite);
static void AnimLickStep(struct Sprite *sprite);
static void AnimTask_NightmareClone_Step(u8 taskId);
static void AnimTask_SpiteTargetShadow_Step(u8 taskId);
static void AnimTask_SpiteTargetShadow_Step2(u8 taskId);
static void AnimTask_SpiteTargetShadow_Step3(u8 taskId);
static void AnimDestinyBondWhiteShadow(struct Sprite *sprite);
static void AnimTask_DestinyBondWhiteShadow_Step(u8 taskId);
static void AnimTask_CurseStretchingBlackBg_Step(u8 taskId);
static void AnimTask_CurseStretchingBlackBg_Step2(u8 taskId);
static void AnimCurseNail(struct Sprite *sprite);
static void AnimCurseNail_Step(struct Sprite *sprite);
static void AnimCurseNail_Step2(struct Sprite *sprite);
static void AnimCurseNail_Step3(struct Sprite *sprite);
static void AnimGhostStatusSprite(struct Sprite *sprite);
static void AnimTask_GrudgeFlames_Step(u8 taskId);
static void AnimGrudgeFlame(struct Sprite *sprite);
static void AnimTask_GhostGetOutAttackerEffect_Step(u8 taskId);
static void AnimTask_GhostGetOutAttackerEffect_Step2(u8 taskId);
static void AnimTask_GhostGetOutAttackerEffect_Step3(u8 taskId);
static void AnimTask_DrawFallingWhiteLinesOnAttacker_Step(u8 taskId);
static void AnimTask_NightmareBgWaveEffectStep(u8 taskId);

static const u16 sRgbWhite[] = { RGB_WHITE };

static const union AffineAnimCmd sAffineAnim_ConfuseRayBallBounce[] =
{
    AFFINEANIMCMD_FRAME(0x1E, 0x1E, 10, 5),
    AFFINEANIMCMD_FRAME(-0x1E, -0x1E, 10, 5),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd *const sAffineAnims_ConfuseRayBallBounce[] =
{
    sAffineAnim_ConfuseRayBallBounce,
};

const struct SpriteTemplate gConfuseRayBallBounceSpriteTemplate =
{
    .tileTag = ANIM_TAG_YELLOW_BALL,
    .paletteTag = ANIM_TAG_YELLOW_BALL,
    .oam = &gOamData_AffineDouble_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_ConfuseRayBallBounce,
    .callback = AnimConfuseRayBallBounce,
};

const struct SpriteTemplate gConfuseRayBallSpiralSpriteTemplate =
{
    .tileTag = ANIM_TAG_YELLOW_BALL,
    .paletteTag = ANIM_TAG_YELLOW_BALL,
    .oam = &gOamData_AffineOff_ObjBlend_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimConfuseRayBallSpiral,
};

static const union AffineAnimCmd sAffineAnim_ShadowBall[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, 10, 1),
    AFFINEANIMCMD_JUMP(0),
};

const union AffineAnimCmd *const gAffineAnims_ShadowBall[] =
{
    sAffineAnim_ShadowBall,
};

const struct SpriteTemplate gShadowBallSpriteTemplate =
{
    .tileTag = ANIM_TAG_SHADOW_BALL,
    .paletteTag = ANIM_TAG_SHADOW_BALL,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gAffineAnims_ShadowBall,
    .callback = AnimShadowBall,
};

const union AnimCmd sAnim_Lick[] =
{
    ANIMCMD_FRAME(0, 2),
    ANIMCMD_FRAME(8, 2),
    ANIMCMD_FRAME(16, 2),
    ANIMCMD_FRAME(24, 2),
    ANIMCMD_FRAME(32, 2),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_Lick[] =
{
    sAnim_Lick,
};

const struct SpriteTemplate gLickSpriteTemplate =
{
    .tileTag = ANIM_TAG_LICK,
    .paletteTag = ANIM_TAG_LICK,
    .oam = &gOamData_AffineOff_ObjNormal_16x32,
    .anims = sAnims_Lick,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimLick,
};

const struct SpriteTemplate gDestinyBondWhiteShadowSpriteTemplate =
{
    .tileTag = ANIM_TAG_WHITE_SHADOW,
    .paletteTag = ANIM_TAG_WHITE_SHADOW,
    .oam = &gOamData_AffineOff_ObjBlend_64x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimDestinyBondWhiteShadow,
};

const struct SpriteTemplate gDestinyBondBlackShadowSpriteTemplate =
{
    .tileTag = ANIM_TAG_WHITE_SHADOW,
    .paletteTag = ANIM_TAG_QUICK_GUARD,
    .oam = &gOamData_AffineOff_ObjBlend_64x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimDestinyBondWhiteShadow,
};

const struct SpriteTemplate gCurseNailSpriteTemplate =
{
    .tileTag = ANIM_TAG_NAIL,
    .paletteTag = ANIM_TAG_NAIL,
    .oam = &gOamData_AffineOff_ObjBlend_32x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimCurseNail,
};

const struct SpriteTemplate gCurseGhostSpriteTemplate =
{
    .tileTag = ANIM_TAG_GHOSTLY_SPIRIT,
    .paletteTag = ANIM_TAG_GHOSTLY_SPIRIT,
    .oam = &gOamData_AffineOff_ObjBlend_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimGhostStatusSprite,
};

const struct SpriteTemplate gNightmareDevilSpriteTemplate =
{
    .tileTag = ANIM_TAG_DEVIL,
    .paletteTag = ANIM_TAG_DEVIL,
    .oam = &gOamData_AffineOff_ObjBlend_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimGhostStatusSprite,
};

static const union AnimCmd sAnim_GrudgeFlame[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(8, 4),
    ANIMCMD_FRAME(16, 4),
    ANIMCMD_FRAME(24, 4),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnims_GrudgeFlame[] =
{
    sAnim_GrudgeFlame,
};

const struct SpriteTemplate gGrudgeFlameSpriteTemplate =
{
    .tileTag = ANIM_TAG_PURPLE_FLAME,
    .paletteTag = ANIM_TAG_PURPLE_FLAME,
    .oam = &gOamData_AffineOff_ObjBlend_16x32,
    .anims = sAnims_GrudgeFlame,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimGrudgeFlame,
};

// Animates the Confuse Ray ball bouncing from the attacker's position to the target's position.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: duration
static void AnimConfuseRayBallBounce(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, TRUE);
	
    sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[1] = sprite->x;
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    sprite->data[3] = sprite->y;
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
    BattleAnim_InitLinearTranslationWithDuration(sprite);
	
	sprite->data[6] = 16;
    
    SetGpuReg(REG_OFFSET_BLDCNT, (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL));
    SetGpuReg(REG_OFFSET_BLDALPHA, sprite->data[6]);
	
	sprite->callback = AnimConfuseRayBallBounce_Step;
}

static void AnimConfuseRayBallBounce_Step(struct Sprite *sprite)
{
    s16 r2;

    UpdateConfuseRayBallBlend(sprite);
	
    if (AnimTranslateLinear(sprite))
    {
        sprite->callback = AnimConfuseRayBallBounce_Step2;
        return;
    }
    sprite->x2 += Sin(sprite->data[5], 10);
    sprite->y2 += Cos(sprite->data[5], 15);
	
    r2 = sprite->data[5];
    sprite->data[5] = (sprite->data[5] + 5) & 0xFF;
	
    if ((r2 != 0 && r2 <= 196) || sprite->data[5] <= 0)
		return;
	
    PlaySE12WithPanning(SE_M_CONFUSE_RAY, gAnimCustomPanning);
}

static void AnimConfuseRayBallBounce_Step2(struct Sprite *sprite)
{
    s16 r2;

    sprite->data[0] = 1;
	
    AnimTranslateLinear(sprite);
	
    sprite->x2 += Sin(sprite->data[5], 10);
    sprite->y2 += Cos(sprite->data[5], 15);
	
    r2 = sprite->data[5];
    sprite->data[5] = (sprite->data[5] + 5) & 0xFF;
	
    if ((r2 == 0 || r2 > 196) && sprite->data[5] > 0)
        PlaySE(SE_M_CONFUSE_RAY);
	
    if (sprite->data[6] == 0)
    {
        sprite->invisible = TRUE;
        sprite->callback = DestroyAnimSpriteAndDisableBlend;
    }
    else
        UpdateConfuseRayBallBlend(sprite);
}

static void UpdateConfuseRayBallBlend(struct Sprite *sprite)
{
    s16 r0;

    if (sprite->data[6] > 0xFF)
    {
        if (++sprite->data[6] == 0x10d)
            sprite->data[6] = 0;
		
        return;
    }
    r0 = sprite->data[7]++;

    if ((r0 & 0xFF) == 0)
    {
        sprite->data[7] &= 0xFF00;
		
        if (sprite->data[7] & 0x100)
            ++sprite->data[6];
        else
            --sprite->data[6];
		
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sprite->data[6], 16 - sprite->data[6]));
		
        if (sprite->data[6] == 0 || sprite->data[6] == 16)
            sprite->data[7] ^= 0x100;
		
        if (sprite->data[6] == 0)
            sprite->data[6] = 0x100;
    }
}

// Animates the Confuse Ray ball moving in spiral movement above the target.
// arg 0: x pixel offset
// arg 1: y pixel offset
static void AnimConfuseRayBallSpiral(struct Sprite *sprite)
{
    InitSpritePosToAnimTarget(sprite, TRUE);
    sprite->callback = AnimConfuseRayBallSpiral_Step;
    sprite->callback(sprite);
}

static void AnimConfuseRayBallSpiral_Step(struct Sprite *sprite)
{
    u16 temp1;

    sprite->x2 = Sin(sprite->data[0], 32);
    sprite->y2 = Cos(sprite->data[0], 8);
	
    temp1 = sprite->data[0] - 65;
    if (temp1 <= 130)
        sprite->oam.priority = 2;
    else
        sprite->oam.priority = 1;
	
    sprite->data[0] = (sprite->data[0] + 19) & 0xFF;
    sprite->data[2] += 80;
	
    sprite->y2 += sprite->data[2] >> 8;
	
    if (++sprite->data[7] == 61)
        DestroyAnimSprite(sprite);
}

// Creates a large transparent clone of the attacker centered on their position which shrinks to original size.
// arg 0: duration (?)
void AnimTask_NightShadeClone(u8 taskId)
{
    u8 spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);

    SetGpuReg(REG_OFFSET_BLDCNT, (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL));
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 0x10));
	
    PrepareBattlerSpriteForRotScale(spriteId, ST_OAM_OBJ_BLEND);
    SetSpriteRotScale(spriteId, 128, 128, 0);
	
    gSprites[spriteId].invisible = FALSE;
	
    gTasks[taskId].data[0] = 128;
    gTasks[taskId].data[1] = gBattleAnimArgs[0];
    gTasks[taskId].data[2] = 0;
    gTasks[taskId].data[3] = 16;
    gTasks[taskId].func = AnimTask_NightShadeClone_Step;
}

static void AnimTask_NightShadeClone_Step(u8 taskId)
{
    if (++gTasks[taskId].data[10] == 3)
    {
        gTasks[taskId].data[10] = 0;
		
        ++gTasks[taskId].data[2];
        --gTasks[taskId].data[3];
		
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[2], gTasks[taskId].data[3]));
		
        if (gTasks[taskId].data[2] == 9)
			gTasks[taskId].func = AnimTask_NightShadeClone_Step2;
    }
}

static void AnimTask_NightShadeClone_Step2(u8 taskId)
{
    u8 spriteId;

    if (gTasks[taskId].data[1] > 0)
        --gTasks[taskId].data[1];
	else
	{
		spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);
		
		gTasks[taskId].data[0] += 8;
		
		if (gTasks[taskId].data[0] <= 0xFF)
			SetSpriteRotScale(spriteId, gTasks[taskId].data[0], gTasks[taskId].data[0], 0);
		else
		{
			ResetSpriteRotScale(spriteId);
			DestroyAnimVisualTaskAndDisableBlend(taskId);
		}
	}
}

// Spins a sprite towards the target, pausing in the middle. Used in Shadow Ball.
// arg 0: duration step 1 (attacker -> center)
// arg 1: duration step 2 (spin center)
// arg 2: duration step 3 (center -> target)
void AnimShadowBall(struct Sprite *sprite)
{
    s16 oldPosX = sprite->x;
    s16 oldPosY = sprite->y;

    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
	
    sprite->data[0] = 0;
    sprite->data[1] = gBattleAnimArgs[0];
    sprite->data[2] = gBattleAnimArgs[1];
    sprite->data[3] = gBattleAnimArgs[2];
    sprite->data[4] = sprite->x << 4;
    sprite->data[5] = sprite->y << 4;
    sprite->data[6] = ((oldPosX - sprite->x) << 4) / (gBattleAnimArgs[0] << 1);
    sprite->data[7] = ((oldPosY - sprite->y) << 4) / (gBattleAnimArgs[0] << 1);
    sprite->callback = AnimShadowBallStep;
}

static void AnimShadowBallStep(struct Sprite *sprite)
{
    switch (sprite->data[0])
    {
    case 0:
        sprite->data[4] += sprite->data[6];
        sprite->data[5] += sprite->data[7];
		
        sprite->x = sprite->data[4] >> 4;
        sprite->y = sprite->data[5] >> 4;
		
        if (--sprite->data[1] > 0)
            break;
		
        ++sprite->data[0];
        break;
    case 1:
        if (--sprite->data[2] > 0)
            break;
		
        sprite->data[1] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
        sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
        sprite->data[4] = sprite->x << 4;
        sprite->data[5] = sprite->y << 4;
        sprite->data[6] = ((sprite->data[1] - sprite->x) << 4) / sprite->data[3];
        sprite->data[7] = ((sprite->data[2] - sprite->y) << 4) / sprite->data[3];
		
        ++sprite->data[0];
        break;
    case 2:
        sprite->data[4] += sprite->data[6];
        sprite->data[5] += sprite->data[7];
		
        sprite->x = sprite->data[4] >> 4;
        sprite->y = sprite->data[5] >> 4;
		
        if (--sprite->data[3] > 0)
            break;
		
        sprite->x = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
        sprite->y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
		
        ++sprite->data[0];
        break;
    case 3:
        DestroyAnimSprite(sprite);
        break;
    }
}

// Animates a lick sprite.
// arg 0: x offset
// arg 1: y offset
static void AnimLick(struct Sprite *sprite)
{
    InitSpritePosToAnimTarget(sprite, TRUE);
    sprite->callback = AnimLickStep;
}

static void AnimLickStep(struct Sprite *sprite)
{
    bool8 blinkVisibility = FALSE, endAnim = FALSE;

    if (sprite->animEnded)
    {
        if (!sprite->invisible)
            sprite->invisible = TRUE;

        switch (sprite->data[0])
        {
        default:
            endAnim = TRUE;
            break;
        case 0:
            if (sprite->data[1] == 2)
                blinkVisibility = TRUE;
            break;
        case 1:
            if (sprite->data[1] == 4)
                blinkVisibility = TRUE;
            break;
        }
		
        if (blinkVisibility)
        {
            sprite->invisible ^= TRUE;
			
            sprite->data[1] = 0;
			
            if (++sprite->data[2] == 5)
            {
                sprite->data[2] = 0;
                ++sprite->data[0];
            }
        }
        else if (endAnim)
            DestroyAnimSprite(sprite);
        else
            ++sprite->data[1];
    }
}

// Animates the target's clone sprite in MOVE_NIGHTMARE.
// No args.
void AnimTask_NightmareClone(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    task->data[0] = CloneBattlerSpriteWithBlend(ANIM_TARGET);
    if (task->data[0] < 0)
    {
        DestroyAnimVisualTask(taskId);
        return;
    }
    task->data[1] = 0;
    task->data[2] = 15;
    task->data[3] = 2;
    task->data[4] = 0;
    SetGpuReg(REG_OFFSET_BLDCNT, (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL));
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(task->data[2], task->data[3]));
	
    gSprites[task->data[0]].data[0] = 80;
	gSprites[task->data[0]].data[3] = 0;
    gSprites[task->data[0]].data[4] = 0;
	
    if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_PLAYER)
    {
        gSprites[task->data[0]].data[1] = -144;
        gSprites[task->data[0]].data[2] = 112;
    }
    else
    {
        gSprites[task->data[0]].data[1] = 144;
        gSprites[task->data[0]].data[2] = -112;
    }
    StoreSpriteCallbackInData6(&gSprites[task->data[0]], SpriteCallbackDummy);
	gSprites[task->data[0]].callback = TranslateSpriteLinearFixedPoint;
    task->func = AnimTask_NightmareClone_Step;
}

static void AnimTask_NightmareClone_Step(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->data[4])
    {
    case 0:
        ++task->data[1];
		
        task->data[5] = task->data[1] & 3;
		
        if (task->data[5] == 1)
		{
            if (task->data[2] > 0)
                --task->data[2];
		}
		
        if (task->data[5] == 3)
		{
            if (task->data[3] <= 15)
                ++task->data[3];
		}
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(task->data[2], task->data[3]));
		
        if (task->data[3] != 16 || task->data[2] != 0 || task->data[1] <= 80)
            break;
		
        DestroySpriteWithActiveSheet(&gSprites[task->data[0]]);
        task->data[4] = 1;
        break;
    case 1:
        if (++task->data[6] <= 1)
            break;
		
        DestroyAnimVisualTaskAndDisableBlend(taskId);
        break;
    }
}

// Animates the wave effect on the nightmare's bg.
// No args.
void AnimTask_NightmareBgWaveEffect(u8 taskId)
{
	CreateTask(AnimTask_NightmareBgWaveEffectStep, 5);
	DestroyAnimVisualTask(taskId);
}

static void AnimTask_NightmareBgWaveEffectStep(u8 taskId)
{
	if (!gTasks[taskId].data[0])
	{
		++gTasks[taskId].data[0];
		ScanlineEffect_InitWave(0, 240, 2, 6, 0, 12, TRUE);
	}
	
	// Signal to stop wave effect
	if (gBattleAnimArgs[ARG_RET_ID] == -1)
	{
		gScanlineEffect.state = 3;
		DestroyTask(taskId);
	}
}

// Animates the target's mon shadow in MOVE_SPITE's anim.
// No args.
void AnimTask_SpiteTargetShadow(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    task->data[15] = 0;
    task->func = AnimTask_SpiteTargetShadow_Step;
    task->func(taskId);
}

static void AnimTask_SpiteTargetShadow_Step(u8 taskId)
{
    s16 startLine;
    struct Task *task = &gTasks[taskId];
    u8 position = GetBattlerSpriteBGPriorityRank(gBattleAnimTarget);

    switch (task->data[15])
    {
    case 0:
        task->data[14] = AllocSpritePalette(ANIM_TAG_BENT_SPOON);
		
        if (task->data[14] == 0xFF || task->data[14] == 0xF)
            DestroyAnimVisualTask(taskId);
        else
        {
            task->data[0] = CloneBattlerSpriteWithBlend(ANIM_TARGET);
			
            if (task->data[0] < 0)
            {
                FreeSpritePaletteByTag(ANIM_TAG_BENT_SPOON);
                DestroyAnimVisualTask(taskId);
            }
            else
            {
                gSprites[task->data[0]].oam.paletteNum = task->data[14];
                gSprites[task->data[0]].oam.objMode = ST_OAM_OBJ_NORMAL;
                gSprites[task->data[0]].oam.priority = 3;
                gSprites[task->data[0]].invisible = gBattleSpritesDataPtr->battlerData[gBattleAnimTarget].invisible;
				
                task->data[1] = 0;
                task->data[2] = 0;
                task->data[3] = 16;
                task->data[13] = GetAnimBattlerSpriteId(ANIM_TARGET);
                task->data[4] = (gSprites[task->data[13]].oam.paletteNum + 16) * 16;
				
                ClearGpuRegBits(REG_OFFSET_DISPCNT, position == 1 ? DISPCNT_BG1_ON : DISPCNT_BG2_ON);
				
                ++task->data[15];
            }
        }
        break;
    case 1:
        task->data[14] = (task->data[14] + 16) * 16;
		
        CpuCopy32(&gPlttBufferUnfaded[task->data[4]], &gPlttBufferFaded[task->data[14]], 0x20);
        BlendPalette(task->data[4], 16, 10, RGB(13, 0, 15));
		
        ++task->data[15];
        break;
    case 2:
        startLine = gSprites[task->data[13]].y + gSprites[task->data[13]].y2 - 32;
        if (startLine < 0)
            startLine = 0;
		
        task->data[10] = ScanlineEffect_InitWave(startLine, startLine + 64, 2, 6, 0, position == 1 ? 4 : 8, TRUE);
		
        ++task->data[15];
        break;
    case 3:
        if (position == 1)
            SetGpuReg(REG_OFFSET_BLDCNT, (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL | BLDCNT_TGT1_BG1));
        else
            SetGpuReg(REG_OFFSET_BLDCNT, (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL | BLDCNT_TGT1_BG2));
		
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 0x10));
		
        ++task->data[15];
        break;
    case 4:
        SetGpuRegBits(REG_OFFSET_DISPCNT, position == 1 ? DISPCNT_BG1_ON : DISPCNT_BG2_ON);
        task->func = AnimTask_SpiteTargetShadow_Step2;
        break;
    }
}

static void AnimTask_SpiteTargetShadow_Step2(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    ++task->data[1];
	
    task->data[5] = task->data[1] & 1;
	
    if (task->data[5] == 0)
        task->data[2] = gSineTable[task->data[1]] / 18;
    else if (task->data[5] == 1)
        task->data[3] = 16 - (gSineTable[task->data[1]] / 18);
	
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(task->data[2], task->data[3]));
	
    if (task->data[1] == 128)
    {
        task->data[15] = 0;
        task->func = AnimTask_SpiteTargetShadow_Step3;
        task->func(taskId);
    }
}

static void AnimTask_SpiteTargetShadow_Step3(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    u8 rank = GetBattlerSpriteBGPriorityRank(gBattleAnimTarget);

    switch (task->data[15])
    {
    case 0:
        gScanlineEffect.state = 3;
        task->data[14] = GetAnimBattlerSpriteId(ANIM_TARGET);
        ClearGpuRegBits(REG_OFFSET_DISPCNT, rank == 1 ? DISPCNT_BG1_ON : DISPCNT_BG2_ON);
        break;
    case 1:
        BlendPalette(task->data[4], 16, 0, RGB(13, 0, 15));
        break;
    case 2:
        gSprites[task->data[14]].invisible = TRUE;
        DestroySpriteWithActiveSheet(&gSprites[task->data[0]]);
        FreeSpritePaletteByTag(ANIM_TAG_BENT_SPOON);
		
        SetGpuRegBits(REG_OFFSET_DISPCNT, rank == 1 ? DISPCNT_BG1_ON : DISPCNT_BG2_ON);
		
        DestroyAnimVisualTaskAndDisableBlend(taskId);
        break;
    }
    ++task->data[15];
}

static void AnimDestinyBondWhiteShadow(struct Sprite *sprite)
{
    if (sprite->data[4])
    {
        sprite->data[0] += sprite->data[2];
        sprite->data[1] += sprite->data[3];
		
        sprite->x = sprite->data[0] >> 4;
        sprite->y = sprite->data[1] >> 4;
		
        if (--sprite->data[4] == 0)
            sprite->data[0] = 0;
    }
}

// Animates Destiny Bond's shadow.
// arg 0: duration (?)
// arg 1: hit both (boolean) (if set, creates an extra shadow in double battles)
// arg 2: black shadow (boolean) (if set, creates an black shadow instead of white)
void AnimTask_DestinyBondWhiteShadow(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    u8 battler, spriteId;
    s16 baseX, baseY;
    s16 x, y;

    SetGpuReg(REG_OFFSET_BLDCNT, (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL));
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 0x10));
	
    task->data[5] = 0;
    task->data[6] = 0;
    task->data[7] = 0;
    task->data[8] = 0;
    task->data[9] = 16;
	
    baseX = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    baseY = GetBattlerSpriteCoordAttr(gBattleAnimAttacker, BATTLER_COORD_ATTR_BOTTOM);
    
    for (battler = 0; battler < MAX_BATTLERS_COUNT; ++battler)
    {
        if (battler != gBattleAnimAttacker && battler != BATTLE_PARTNER(gBattleAnimAttacker))
        {
			if (!gBattleAnimArgs[1] && battler == BATTLE_PARTNER(gBattleAnimTarget))
				continue;
			
			if (!IsBattlerSpriteVisible(battler))
				continue;
			
			if (!gBattleAnimArgs[2])
				spriteId = CreateSprite(&gDestinyBondWhiteShadowSpriteTemplate, baseX, baseY, 55);
			else
				spriteId = CreateSprite(&gDestinyBondBlackShadowSpriteTemplate, baseX, baseY, 55);
            
            if (spriteId != MAX_SPRITES)
            {
                x = GetBattlerSpriteCoord(battler, BATTLER_COORD_X);
                y = GetBattlerSpriteCoordAttr(battler, BATTLER_COORD_ATTR_BOTTOM);
                
                gSprites[spriteId].data[0] = baseX << 4;
                gSprites[spriteId].data[1] = baseY << 4;
                gSprites[spriteId].data[2] = ((x - baseX) << 4) / gBattleAnimArgs[0];
                gSprites[spriteId].data[3] = ((y - baseY) << 4) / gBattleAnimArgs[0];
                gSprites[spriteId].data[4] = gBattleAnimArgs[0];
                gSprites[spriteId].data[5] = x;
                gSprites[spriteId].data[6] = y;
                task->data[task->data[12] + 13] = spriteId;
                ++task->data[12];
            }
        }
    }
    task->func = AnimTask_DestinyBondWhiteShadow_Step;
}

static void AnimTask_DestinyBondWhiteShadow_Step(u8 taskId)
{
    u16 i;
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        if (task->data[6] == 0)
        {
            if (++task->data[5] > 1)
            {
                task->data[5] = 0;

                if (++task->data[7] & 1)
                {
                    if (task->data[8] < 16)
                        ++task->data[8];
                }
                else
                {
                    if (task->data[9])
                        --task->data[9];
                }
                SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(task->data[8], task->data[9]));
				
                if (task->data[7] >= 24)
                {
                    task->data[7] = 0;
                    task->data[6] = 1;
                }
            }
        }
		
        if (task->data[6])
            ++task->data[0];
        break;
    case 1:
        if (++task->data[5] > 1)
        {
            task->data[5] = 0;

            if (++task->data[7] & 1)
            {
                if (task->data[8])
                    --task->data[8];
            }
            else if (task->data[9] < 16)
				++task->data[9];
			
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(task->data[8], task->data[9]));
			
            if (task->data[8] == 0 && task->data[9] == 16)
            {
                for (i = 0; i < task->data[12]; ++i)
                    DestroySprite(&gSprites[task->data[i + 13]]);
				
                ++task->data[0];
            }
        }
        break;
    case 2:
        if (++task->data[5] > 0)
            ++task->data[0];
        break;
    case 3:
        DestroyAnimVisualTaskAndDisableBlend(taskId);
        break;
    }
}

// Animates ghost Curse's background effect.
// No args.
void AnimTask_CurseStretchingBlackBg(u8 taskId)
{
    s16 startX = GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER ? 40 : 200, startY = 40;
    s16 leftDistance, topDistance, bottomDistance, rightDistance;

    gBattle_WIN0H = WIN_RANGE(startX, startX);
    gBattle_WIN0V = WIN_RANGE(startY, startY);
	
    SetGpuReg(REG_OFFSET_WININ, ((WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR) | (WININ_WIN1_BG_ALL | WININ_WIN1_OBJ | WININ_WIN1_CLR)));
    SetGpuReg(REG_OFFSET_WINOUT, ((WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ) | (WINOUT_WINOBJ_BG_ALL | WINOUT_WINOBJ_OBJ | WINOUT_WINOBJ_CLR)));
    SetGpuReg(REG_OFFSET_BLDCNT, (BLDCNT_TGT1_BG3 | BLDCNT_EFFECT_DARKEN));
    SetGpuReg(REG_OFFSET_BLDY, 0x10);
	
    leftDistance = startX;
    rightDistance = 240 - startX;
	
    topDistance = startY;
    bottomDistance = 72;
	
    gTasks[taskId].data[1] = leftDistance;
    gTasks[taskId].data[2] = rightDistance;
    gTasks[taskId].data[3] = topDistance;
    gTasks[taskId].data[4] = bottomDistance;
    gTasks[taskId].data[5] = startX;
    gTasks[taskId].data[6] = startY;
    gTasks[taskId].func = AnimTask_CurseStretchingBlackBg_Step;
}

static void AnimTask_CurseStretchingBlackBg_Step(u8 taskId)
{
    s16 step, leftDistance, rightDistance, topDistance, bottomDistance, startX, startY;
    u16 left, right, top, bottom;

    step = gTasks[taskId].data[0]++;

    leftDistance = gTasks[taskId].data[1];
    rightDistance = gTasks[taskId].data[2];
    topDistance = gTasks[taskId].data[3];
    bottomDistance = gTasks[taskId].data[4];
	
    startX = gTasks[taskId].data[5];
    startY = gTasks[taskId].data[6];
	
    if (step < 16)
    {
        left   = startX - (leftDistance   * 0.0625) * step;
        right  = startX + (rightDistance  * 0.0625) * step;
        top    = startY - (topDistance    * 0.0625) * step;
        bottom = startY + (bottomDistance * 0.0625) * step;
    }
    else
    {
        left = 0;
        right = 240;
        top = 0;
        bottom = 112;
		
        BeginNormalPaletteFade(SelectBattleAnimSpriteAndBgPalettes(TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE), 0, 16, 16, RGB_BLACK);
		
        gTasks[taskId].func = AnimTask_CurseStretchingBlackBg_Step2;
    }
    gBattle_WIN0H = WIN_RANGE(left, right);
    gBattle_WIN0V = WIN_RANGE(top, bottom);
}

static void AnimTask_CurseStretchingBlackBg_Step2(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        gBattle_WIN0H = 0;
        gBattle_WIN0V = 0;
		
        SetGpuReg(REG_OFFSET_WININ, ((WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR) | (WININ_WIN1_BG_ALL | WININ_WIN1_OBJ | WININ_WIN1_CLR)));
        SetGpuReg(REG_OFFSET_WINOUT, ((WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR) | (WINOUT_WINOBJ_BG_ALL | WINOUT_WINOBJ_OBJ | WINOUT_WINOBJ_CLR)));
		
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        DestroyAnimVisualTask(taskId);
    }
}

// Animates ghost Curse's nail sprite.
// arg 0: x pixel offset
// arg 1: y pixel offset
static void AnimCurseNail(struct Sprite *sprite)
{
    s16 xDelta, xDelta2;

    InitSpritePosToAnimAttacker(sprite, TRUE);
	
    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
    {
        xDelta = 24;
        xDelta2 = -2;
        sprite->oam.matrixNum = ST_OAM_HFLIP;
    }
    else
    {
        xDelta = -24;
        xDelta2 = 2;
    }
    sprite->x += xDelta;
    sprite->data[1] = xDelta2;
    sprite->data[0] = 60;
    sprite->callback = AnimCurseNail_Step;
}

static void AnimCurseNail_Step(struct Sprite *sprite)
{
    if (sprite->data[0] > 0)
        --sprite->data[0];
    else
    {
        sprite->x2 += sprite->data[1];

        if ((u16)sprite->x2 + 7 > 14)
        {
            sprite->x += sprite->x2;
            sprite->x2 = 0;
			
            sprite->oam.tileNum += 8;
			
            if (++sprite->data[2] == 3)
            {
                sprite->data[0] = 30;
                sprite->callback = WaitAnimForDuration;
                StoreSpriteCallbackInData6(sprite, AnimCurseNail_Step2);
            }
            else
                sprite->data[0] = 40;
        }
    }
}

static void AnimCurseNail_Step2(struct Sprite *sprite)
{
    if (sprite->data[0] == 0)
    {
		++sprite->data[0];
		
        SetGpuReg(REG_OFFSET_BLDCNT, (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL));
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16, 0));
        
        sprite->data[1] = 0;
        sprite->data[2] = 0;
    }
    else if (sprite->data[1] < 2)
        ++sprite->data[1];
    else
    {
        sprite->data[1] = 0;
		
        ++sprite->data[2];
		
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND2(16 - sprite->data[2], sprite->data[2]));
		
        if (sprite->data[2] == 16)
        {
            sprite->invisible = TRUE;
            sprite->callback = AnimCurseNail_Step3;
        }
    }
}

static void AnimCurseNail_Step3(struct Sprite *sprite)
{
    gBattle_WIN0H = 0;
    gBattle_WIN0V = 0;
    DestroyAnimSpriteAndDisableBlend(sprite);
}

// Animates the ghost spirit above the mon's head.
// No args.
static void AnimGhostStatusSprite(struct Sprite *sprite)
{
    u16 coeffB, coeffA;

    sprite->x2 = Sin(sprite->data[0], 12);
	
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        sprite->x2 = -sprite->x2;
	
    sprite->data[0] = (sprite->data[0] + 6) & 0xFF;
	
    sprite->data[1] += 0x100;
    sprite->y2 = -(sprite->data[1] >> 8);

    if (++sprite->data[7] == 1)
    {
        sprite->data[6] = 0x050B;
        SetGpuReg(REG_OFFSET_BLDCNT, (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL));
        SetGpuReg(REG_OFFSET_BLDALPHA, sprite->data[6]);
    }
    else if (sprite->data[7] > 30)
    {
        ++sprite->data[2];
		
        coeffB = sprite->data[6] >> 8;
        coeffA = sprite->data[6] & 0xFF;
		
        if (++coeffB > 16)
            coeffB = 16;
		
        --coeffA;
        if ((s16)coeffA < 0)
            coeffA = 0;
		
		sprite->data[6] = BLDALPHA_BLEND(coeffA, coeffB);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(coeffA, coeffB));
		
        if (coeffB == 16 && coeffA == 0)
        {
            sprite->invisible = TRUE;
            sprite->callback = DestroyAnimSpriteAndDisableBlend;
        }
    }
}

// Animates Grudge flames on the given battler.
// arg 0: anim battler
void AnimTask_GrudgeFlames(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
	u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);

    task->data[0] = 0;
    task->data[1] = 16;
	task->data[2] = battler;
    task->data[9] = GetBattlerSpriteCoord(battler, BATTLER_COORD_X);
    task->data[10] = GetBattlerYCoordWithElevation(battler);
    task->data[11] = (GetBattlerSpriteCoordAttr(battler, BATTLER_COORD_ATTR_WIDTH) / 2) + 8;
    task->data[7] = 0;
    task->data[5] = GetBattlerSpriteBGPriority(battler);
    task->data[6] = GetBattlerSpriteSubpriority(battler) - 2;
    task->data[3] = 0;
    task->data[4] = 16;
	
    SetGpuReg(REG_OFFSET_BLDCNT, (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL));
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 0x10));
	
    task->data[8] = 0;
    task->func = AnimTask_GrudgeFlames_Step;
}

static void AnimTask_GrudgeFlames_Step(u8 taskId)
{
    u16 i;
    u8 spriteId;
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        for (i = 0; i < 6; ++i)
        {
            spriteId = CreateSprite(&gGrudgeFlameSpriteTemplate, task->data[9], task->data[10], task->data[6]);
			
            if (spriteId != MAX_SPRITES)
            {
                gSprites[spriteId].data[0] = taskId;
                gSprites[spriteId].data[1] = (GetBattlerSide(task->data[2]) == B_SIDE_PLAYER);
                gSprites[spriteId].data[2] = (i * 42) & 0xFF;
                gSprites[spriteId].data[3] = task->data[11];
                gSprites[spriteId].data[5] = i * 6;
                ++task->data[7];
            }
        }
        ++task->data[0];
        break;
    case 1:
        if (++task->data[1] & 1)
        {
            if (task->data[3] < 14)
                ++task->data[3];
        }
        else if (task->data[4] > 4)
            --task->data[4];

        if (task->data[3] == 14 && task->data[4] == 4)
        {
            task->data[1] = 0;
            ++task->data[0];
        }
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(task->data[3], task->data[4]));
        break;
    case 2:
        if (++task->data[1] > 30)
        {
            task->data[1] = 0;
            ++task->data[0];
        }
        break;
    case 3:
        if (++task->data[1] & 1)
        {
            if (task->data[3] > 0)
                --task->data[3];
        }
        else if (task->data[4] < 16)
            ++task->data[4];

        if (task->data[3] == 0 && task->data[4] == 16)
        {
            task->data[8] = 1;
            ++task->data[0];
        }
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(task->data[3], task->data[4]));
        break;
    case 4:
        if (task->data[7] == 0)
            ++task->data[0];
        break;
    case 5:
        DestroyAnimVisualTaskAndDisableBlend(taskId);
        break;
    }
}

static void AnimGrudgeFlame(struct Sprite *sprite)
{
    u16 index;

    if (!sprite->data[1])
        sprite->data[2] += 2;
    else
        sprite->data[2] -= 2;
	
    sprite->data[2] &= 0xFF;
    sprite->x2 = Sin(sprite->data[2], sprite->data[3]);
	
    index = sprite->data[2] - 65;
    if (index < 127)
        sprite->oam.priority = gTasks[sprite->data[0]].data[5] + 1;
    else
        sprite->oam.priority = gTasks[sprite->data[0]].data[5];
	
    ++sprite->data[5];
	
    sprite->data[6] = (sprite->data[5] * 8) & 0xFF;
    sprite->y2 = Sin(sprite->data[6], 7);
	
    if (gTasks[sprite->data[0]].data[8])
    {
        --gTasks[sprite->data[0]].data[7];
        DestroySprite(sprite);
    }
}

// Animates the attacker's effect on ghost get out animation.
// No args.
void AnimTask_GhostGetOutAttackerEffect(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    task->data[15] = 0;
    task->func = AnimTask_GhostGetOutAttackerEffect_Step;
    AnimTask_GhostGetOutAttackerEffect_Step(taskId);
}

static void AnimTask_GhostGetOutAttackerEffect_Step(u8 taskId)
{
    s16 y;
    struct BattleAnimBgData animBgData;
    struct Task *task = &gTasks[taskId];

    switch (task->data[15])
    {
    case 0:
        SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 2);
        SetAnimBgAttribute(2, BG_ANIM_PRIORITY, 1);
		
        task->data[1] = 0;
        task->data[2] = 0;
        task->data[3] = 16;
        task->data[4] = GetAnimBattlerSpriteId(ANIM_ATTACKER);
        task->data[5] = gSprites[task->data[4]].oam.priority;
        task->data[6] = (gSprites[task->data[4]].oam.paletteNum + 16) << 4;
		
        gSprites[task->data[4]].oam.objMode = ST_OAM_OBJ_BLEND;
        gSprites[task->data[4]].oam.priority = 3;
		
        task->data[7] = 128;
        break;
    case 1:
        if (++task->data[1] & 1)
            return;
		
        BlendPalette(task->data[6], 0x10, task->data[2], RGB(0, 23, 25));
        BlendPalette(task->data[7], 0x10, task->data[2], RGB(0, 23, 25));
		
        if (task->data[2] <= 11)
        {
            ++task->data[2];
            return;
        }
        task->data[1] = 0;
        task->data[2] = 0;
		
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 0x10));
        break;
    case 2:
        SetAnimBgAttribute(2, BG_ANIM_CHAR_BASE_BLOCK, 1);
        SetAnimBgAttribute(2, BG_ANIM_SCREEN_SIZE, 0);
		
        gBattle_BG2_X = 0;
        gBattle_BG2_Y = 0;
		
        SetGpuReg(REG_OFFSET_BG2HOFS, gBattle_BG2_X);
        SetGpuReg(REG_OFFSET_BG2VOFS, gBattle_BG2_Y);
		
        GetBattleAnimBgData(&animBgData, 2);
        AnimLoadCompressedBgGfx(animBgData.bgId, gBattleAnim_ScaryFaceGfx, animBgData.tilesOffset);
        LoadCompressedPalette(gBattleAnim_ScaryFacePal, 16 * animBgData.paletteId, 0x20);
        break;
    case 3:
        GetBattleAnimBgData(&animBgData, 2);
        gMonSpritesGfxPtr->multiUseBuffer = AllocZeroed(0x2000);
        LZDecompressWram(gBattleAnimBgTilemap_ScaryFacePlayer, gMonSpritesGfxPtr->multiUseBuffer);
        RelocateBattleBgPal(animBgData.paletteId, gMonSpritesGfxPtr->multiUseBuffer, 256, 0);
        CopyToBgTilemapBufferRect_ChangePalette(animBgData.bgId, gMonSpritesGfxPtr->multiUseBuffer, 0, 0, 0x20, 0x20, 0x11);
        CopyBgTilemapBufferToVram(2);
        FREE_AND_SET_NULL(gMonSpritesGfxPtr->multiUseBuffer);
        break;
    case 4:
        if (++task->data[1] & 1)
            return;
		
        ++task->data[2];
        --task->data[3];
		
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(task->data[2], task->data[3]));
		
        if (task->data[3])
            return;
		
        task->data[1] = 0;
        task->data[2] = 0;
        task->data[3] = 16;
		
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 0x10));
		
        SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
        SetAnimBgAttribute(2, BG_ANIM_PRIORITY, 2);
        break;
    case 5:
        ClearGpuRegBits(REG_OFFSET_DISPCNT, GetBattlerSpriteBGPriorityRank(gBattleAnimAttacker) == 1 ? DISPCNT_BG1_ON : DISPCNT_BG2_ON);
        break;
    case 6:
        y = gSprites[task->data[4]].y + gSprites[task->data[4]].y2 - 0x20;
        if (y < 0)
            y = 0;
		
        task->data[10] = ScanlineEffect_InitWave(y, y + 0x40, 4, 8, 0, GetBattlerSpriteBGPriorityRank(gBattleAnimAttacker) == 1 ? 4 : 8, TRUE);
        break;
    case 7:
        BlendPalette(task->data[7], 0x10, 0xC, RGB(31, 31, 29));
        SetGpuRegBits(REG_OFFSET_DISPCNT, GetBattlerSpriteBGPriorityRank(gBattleAnimAttacker) == 1 ? DISPCNT_BG1_ON : DISPCNT_BG2_ON);
        task->func = AnimTask_GhostGetOutAttackerEffect_Step2;
        task->data[15] = 0;
        break;
    }
    ++task->data[15];
}

static void AnimTask_GhostGetOutAttackerEffect_Step2(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    ++task->data[1];
	
    task->data[8] = task->data[1] & 1;
	
    if (!task->data[8])
        task->data[2] = gSineTable[task->data[1]] / 18;
    else if (task->data[8] == 1)
        task->data[3] = 16 - gSineTable[task->data[1]] / 18;
	
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(task->data[2], task->data[3]));
	
    if (task->data[1] == 128)
    {
        task->data[15] = 0;
        task->func = AnimTask_GhostGetOutAttackerEffect_Step3;
        AnimTask_GhostGetOutAttackerEffect_Step3(taskId);
    }
}

static void AnimTask_GhostGetOutAttackerEffect_Step3(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->data[15])
    {
    case 0:
        gScanlineEffect.state = 3;
        BlendPalette(task->data[7], 0x10, 0xC, RGB(0, 23, 25));
        break;
    case 1:
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0x10, 0));
		
        task->data[2] = 16;
        task->data[3] = 0;
        break;
    case 2:
        --task->data[2];
        ++task->data[3];
		
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(task->data[2], task->data[3]));
		
        if (task->data[3] <= 15)
            return;
		
        SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 2);
        SetAnimBgAttribute(2, BG_ANIM_PRIORITY, 2);
        break;
    case 3:
        InitBattleAnimBg(2);
        FillPalette(0, 0x90, 0x20);
        SetAnimBgAttribute(2, BG_ANIM_CHAR_BASE_BLOCK, 0);
        task->data[1] = 12;
        break;
    case 4:
        BlendPalette(task->data[6], 0x10, task->data[1], RGB(0, 23, 25));
        BlendPalette(task->data[7], 0x10, task->data[1], RGB(0, 23, 25));
		
        if (task->data[1])
        {
            --task->data[1];
            return;
        }
        task->data[1] = 0;
		
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 0x10));
        break;
    case 5:
        gSprites[task->data[4]].oam.priority = task->data[5];
        gSprites[task->data[4]].oam.objMode = ST_OAM_OBJ_NORMAL;
		
        SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
        SetAnimBgAttribute(2, BG_ANIM_PRIORITY, 1);
		
        DestroyAnimVisualTaskAndDisableBlend(taskId);
        break;
    }
    ++task->data[15];
}

// Creates non-ghost Curse bg's effect.
// No args.
void AnimTask_SetUpCurseBackground(u8 taskId)
{
    s32 newSpriteId;
    u16 bg1Cnt;
    u8 spriteId;
    struct BattleAnimBgData animBgData;
	bool8 changedPriority;

    gBattle_WIN0H = 0;
    gBattle_WIN0V = 0;
	
    SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG_ALL | WININ_WIN1_OBJ | WININ_WIN1_CLR);
    SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG2 | WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR | WINOUT_WINOBJ_BG_ALL | WINOUT_WINOBJ_OBJ | WINOUT_WINOBJ_CLR);
    SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJWIN_ON);
	
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG1 | BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(8, 12));
	
    bg1Cnt = GetGpuReg(REG_OFFSET_BG1CNT);
    ((struct BgCnt *)&bg1Cnt)->priority = 0;
    ((struct BgCnt *)&bg1Cnt)->screenSize = 0;
    SetGpuReg(REG_OFFSET_BG1CNT, bg1Cnt);
	
    ((struct BgCnt *)&bg1Cnt)->charBaseBlock = 1;
    SetGpuReg(REG_OFFSET_BG1CNT, bg1Cnt);
    
	changedPriority = FALSE;
	
    if (IsDoubleBattleForBattler(gBattleAnimAttacker))
    {
        if (GetBattlerPosition(gBattleAnimAttacker) == B_POSITION_OPPONENT_RIGHT || GetBattlerPosition(gBattleAnimAttacker) == B_POSITION_PLAYER_LEFT)
        {
            if (IsBattlerSpriteVisible(BATTLE_PARTNER(gBattleAnimAttacker)))
            {
                gSprites[gBattlerSpriteIds[BATTLE_PARTNER(gBattleAnimAttacker)]].oam.priority -= 1;
                ((struct BgCnt *)&bg1Cnt)->priority = 1;
                SetGpuReg(REG_OFFSET_BG1CNT, bg1Cnt);
                changedPriority = TRUE;
            }
        }
    }
    spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);
    newSpriteId = CreateCloneOfSpriteInWindowMode(spriteId, GetMonData(GetBattlerPartyIndexPtr(gBattleAnimAttacker), MON_DATA_SPECIES));
	
    GetBattleAnimBgData(&animBgData, 1);
    AnimLoadCompressedBgTilemap(animBgData.bgId, gFile_graphics_battle_anims_masks_curse_tilemap);
    AnimLoadCompressedBgGfx(animBgData.bgId, gFile_graphics_battle_anims_masks_curse_sheet, animBgData.tilesOffset);
    LoadPalette(sRgbWhite, animBgData.paletteId * 16 + 1, 2);
	
    gBattle_BG1_X = -gSprites[spriteId].x + 32;
    gBattle_BG1_Y = -gSprites[spriteId].y + 32;
	
    gTasks[taskId].data[0] = newSpriteId;
    gTasks[taskId].data[6] = changedPriority;
    gTasks[taskId].func = AnimTask_DrawFallingWhiteLinesOnAttacker_Step;
}

static void AnimTask_DrawFallingWhiteLinesOnAttacker_Step(u8 taskId)
{
    struct BattleAnimBgData animBgData;
    u16 bg1Cnt;

    gTasks[taskId].data[10] += 4;
	
    gBattle_BG1_Y -= 4;
	
    if (gTasks[taskId].data[10] == 64)
    {
        gTasks[taskId].data[10] = 0;
		
        gBattle_BG1_Y += 64;
		
        if (++gTasks[taskId].data[11] == 4)
        {
            ResetBattleAnimBg(0);
			
            gBattle_WIN0H = 0;
            gBattle_WIN0V = 0;
			
            SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG_ALL | WININ_WIN1_OBJ | WININ_WIN1_CLR);
            SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG_ALL  | WINOUT_WIN01_OBJ  | WINOUT_WIN01_CLR | WINOUT_WINOBJ_BG_ALL | WINOUT_WINOBJ_OBJ | WINOUT_WINOBJ_CLR);
			
            bg1Cnt = GetGpuReg(REG_OFFSET_BG1CNT);
            ((struct BgCnt *)&bg1Cnt)->charBaseBlock = 0;
            SetGpuReg(REG_OFFSET_BG1CNT, bg1Cnt);
			
            SetGpuReg(REG_OFFSET_DISPCNT, GetGpuReg(REG_OFFSET_DISPCNT) ^ DISPCNT_OBJWIN_ON);
			
            DestroySprite(&gSprites[gTasks[taskId].data[0]]);
			
            GetBattleAnimBgData(&animBgData, 1);
            InitBattleAnimBg(animBgData.bgId);
			
            if (gTasks[taskId].data[6])
                ++gSprites[gBattlerSpriteIds[BATTLE_PARTNER(gBattleAnimAttacker)]].oam.priority;
			
            gBattle_BG1_Y = 0;
			
            DestroyAnimVisualTaskAndDisableBlend(taskId);
        }
    }
}

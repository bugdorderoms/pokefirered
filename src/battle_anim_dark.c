#include "global.h"
#include "gflib.h"
#include "battle_anim.h"
#include "graphics.h"
#include "scanline_effect.h"
#include "trig.h"
#include "util.h"
#include "constants/songs.h"

static void AnimTearDrop(struct Sprite *sprite);
static void AnimClawSlash(struct Sprite *sprite);
static void AnimTask_AttackerFadeToInvisible_Step(u8 taskId);
static void AnimTask_AttackerFadeFromInvisible_Step(u8 taskId);
static void AnimBite_Step(struct Sprite *sprite);
static void SetPriorityForAllVisibleBattlers(u8 priority);
static void AnimTask_MoveAttackerMementoShadow_Step(u8 taskId);
static void DoMementoShadowEffect(struct Task *task);
static void AnimTask_MoveTargetMementoShadow_Step(u8 taskId);
static void TormentAttacker_Step(u8);
static void TormentAttacker_Callback(struct Sprite *);
static void AnimTauntFinger(struct Sprite* sprite);
static void AnimTauntFingerStep1(struct Sprite *sprite);
static void AnimTauntFingerStep2(struct Sprite *sprite);

static const union AffineAnimCmd sAffineAnim_Bite_0[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, 0, 1),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Bite_1[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, 32, 1),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Bite_2[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, 64, 1),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Bite_3[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, 96, 1),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Bite_4[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, -128, 1),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Bite_5[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, -96, 1),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Bite_6[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, -64, 1),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Bite_7[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, -32, 1),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd *const gAffineAnims_Bite[] =
{
    sAffineAnim_Bite_0,
    sAffineAnim_Bite_1,
    sAffineAnim_Bite_2,
    sAffineAnim_Bite_3,
    sAffineAnim_Bite_4,
    sAffineAnim_Bite_5,
    sAffineAnim_Bite_6,
    sAffineAnim_Bite_7,
};

const struct SpriteTemplate gSharpTeethSpriteTemplate =
{
    .tileTag = ANIM_TAG_SHARP_TEETH,
    .paletteTag = ANIM_TAG_SHARP_TEETH,
    .oam = &gOamData_AffineNormal_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gAffineAnims_Bite,
    .callback = AnimBite,
};

static const union AffineAnimCmd sAffineAnim_TearDrop_0[] =
{
    AFFINEANIMCMD_FRAME(0xC0, 0xC0, 80, 0),
    AFFINEANIMCMD_FRAME(0x0, 0x0, -2, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_TearDrop_1[] =
{
    AFFINEANIMCMD_FRAME(0xC0, 0xC0, -80, 0),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 2, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims_TearDrop[] =
{
    sAffineAnim_TearDrop_0,
    sAffineAnim_TearDrop_1,
};

const struct SpriteTemplate gTearDropSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_BUBBLES,
    .paletteTag = ANIM_TAG_SMALL_BUBBLES,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_TearDrop,
    .callback = AnimTearDrop,
};

static const union AnimCmd sAnim_ClawSlash_0[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(16, 4),
    ANIMCMD_FRAME(32, 4),
    ANIMCMD_FRAME(48, 4),
    ANIMCMD_FRAME(64, 4),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_ClawSlash_1[] =
{
    ANIMCMD_FRAME(0, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(48, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(64, 4, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_ClawSlash[] =
{
    sAnim_ClawSlash_0,
    sAnim_ClawSlash_1,
};

const struct SpriteTemplate gClawSlashSpriteTemplate =
{
    .tileTag = ANIM_TAG_CLAW_SLASH,
    .paletteTag = ANIM_TAG_CLAW_SLASH,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sAnims_ClawSlash,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimClawSlash,
};

static const union AffineAnimCmd sTormentAttackerAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(-12, 8, 0, 4),
    AFFINEANIMCMD_FRAME(20, -20, 0, 4),
    AFFINEANIMCMD_FRAME(-8, 12, 0, 4),
    AFFINEANIMCMD_END,
};

static const union AnimCmd sTauntFingerAnimCmds1[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

static const union AnimCmd sTauntFingerAnimCmds2[] =
{
    ANIMCMD_FRAME(0, 1, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd sTauntFingerAnimCmds3[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(16, 4),
    ANIMCMD_FRAME(32, 4),
    ANIMCMD_FRAME(16, 4),
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(16, 4),
    ANIMCMD_FRAME(32, 4),
    ANIMCMD_END,
};

static const union AnimCmd sTauntFingerAnimCmds4[] =
{
    ANIMCMD_FRAME(0, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(0, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 4, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd *const sTauntFingerAnimTable[] =
{
    sTauntFingerAnimCmds1,
    sTauntFingerAnimCmds2,
    sTauntFingerAnimCmds3,
    sTauntFingerAnimCmds4,
};

const struct SpriteTemplate gTauntFingerSpriteTemplate =
{
    .tileTag = ANIM_TAG_FINGER_2,
    .paletteTag = ANIM_TAG_FINGER_2,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sTauntFingerAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimTauntFinger,
};

const struct SpriteTemplate gDropRockSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_ROCK,
    .paletteTag = ANIM_TAG_SMALL_ROCK,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_TearDrop,
    .callback = AnimTearDrop,
};

const struct SpriteTemplate gChainLinkSpriteTemplate =
{
    .tileTag = ANIM_TAG_CHAIN_LINK,
    .paletteTag = ANIM_TAG_CHAIN_LINK,
    .oam = &gOamData_AffineOff_ObjNormal_64x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimStringWrap,
};

const struct SpriteTemplate gFlingItemSpriteTemplate =
{
    .tileTag = ANIM_TAG_ITEM_BAG,
    .paletteTag = ANIM_TAG_ITEM_BAG,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimMissileArc,
};

const struct SpriteTemplate gDarkPulseRingSpriteTemplate =
{
    .tileTag = ANIM_TAG_PURPLE_RING,
    .paletteTag = ANIM_TAG_PURPLE_RING,
    .oam = &gOamData_AffineDouble_ObjNormal_16x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gAffineAnims_ShadowBall,
    .callback = TranslateAnimSpriteToTargetMonLocation,
};

const struct SpriteTemplate gShadowClawSpriteTemplate =
{
    .tileTag = ANIM_TAG_CLAW_SLASH,
    .paletteTag = ANIM_TAG_HANDS_AND_FEET,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sAnims_ClawSlash,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimClawSlash,
};

// Fade the attacker's sprite from visible to invisible.
// arg 0: fade delay
void AnimTask_AttackerFadeToInvisible(u8 taskId)
{
    gTasks[taskId].data[0] = gBattleAnimArgs[0];
    gTasks[taskId].data[1] = 16;
	
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16, 0));
	
    if (GetBattlerSpriteBGPriorityRank(gBattleAnimAttacker) == 1)
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_BG1);
    else
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_BG2);
	
    gTasks[taskId].func = AnimTask_AttackerFadeToInvisible_Step;
}

static void AnimTask_AttackerFadeToInvisible_Step(u8 taskId)
{
    u8 blendA, blendB;

    if (gTasks[taskId].data[2] == (u8)gTasks[taskId].data[0])
    {
		gTasks[taskId].data[2] = 0;
		
		blendA = gTasks[taskId].data[1] >> 8;
		blendB = gTasks[taskId].data[1];
		
        ++blendA;
        --blendB;
		
        gTasks[taskId].data[1] = BLDALPHA_BLEND(blendB, blendA);
		
        SetGpuReg(REG_OFFSET_BLDALPHA, gTasks[taskId].data[1]);
		
        if (blendA == 16)
        {
            gSprites[gBattlerSpriteIds[gBattleAnimAttacker]].invisible = TRUE;
            DestroyAnimVisualTask(taskId);
        }
    }
    else
        ++gTasks[taskId].data[2];
}

// Fade the attacker's sprite from invisible to visible.
// arg 0: fade delay
void AnimTask_AttackerFadeFromInvisible(u8 taskId)
{
    gTasks[taskId].data[0] = gBattleAnimArgs[0];
    gTasks[taskId].data[1] = BLDALPHA_BLEND(0, 16);
	
	SetGpuReg(REG_OFFSET_BLDALPHA, gTasks[taskId].data[1]);
	
    gTasks[taskId].func = AnimTask_AttackerFadeFromInvisible_Step;
}

static void AnimTask_AttackerFadeFromInvisible_Step(u8 taskId)
{
    u8 blendA, blendB;

    if (gTasks[taskId].data[2] == (u8)gTasks[taskId].data[0])
    {
		gTasks[taskId].data[2] = 0;
		
		blendA = gTasks[taskId].data[1] >> 8;
		blendB = gTasks[taskId].data[1];
		
        --blendA;
        ++blendB;
		
        gTasks[taskId].data[1] = (blendA << 8) | blendB;
        SetGpuReg(REG_OFFSET_BLDALPHA, gTasks[taskId].data[1]);
        
        if (blendA == 0)
			DestroyAnimVisualTaskAndDisableBlend(taskId);
    }
    else
        ++gTasks[taskId].data[2];
}

// Inits MOVE_FEINT_ATTACK attacker's sprite fading from invisible.
// No args.
void AnimTask_InitAttackerFadeFromInvisible(u8 taskId)
{
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 16));
	
    if (GetBattlerSpriteBGPriorityRank(gBattleAnimAttacker) == 1)
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_BG1);
    else
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_BG2);
	
    DestroyAnimVisualTask(taskId);
}

// Move sprite inward for Bite/Crunch and Clamp.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: affine anim num
// arg 3: x movement value
// arg 4: y movement value
// arg 5: duration
void AnimBite(struct Sprite *sprite)
{
    sprite->x += gBattleAnimArgs[0];
    sprite->y += gBattleAnimArgs[1];
	
    StartSpriteAffineAnim(sprite, gBattleAnimArgs[2]);
	
    sprite->data[0] = gBattleAnimArgs[3];
    sprite->data[1] = gBattleAnimArgs[4];
    sprite->data[2] = gBattleAnimArgs[5];
	StoreSpriteCallbackInData6(sprite, AnimBite_Step);
    sprite->callback = AnimMoveSpriteOverDuration;
}

static void AnimBite_Step(struct Sprite *sprite)
{
	sprite->data[0] = -sprite->data[0];
	sprite->data[1] = -sprite->data[1];
	sprite->data[3] = 0;
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
	sprite->callback = AnimMoveSpriteOverDuration;
}

// Launches a tear drop away from the battler. Used by Fake Tears.
// arg 0: anim battler
// arg 1: which position
// arg 2: which sprite
static void AnimTearDrop(struct Sprite *sprite)
{
    u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);
    s8 xOffset = 20;
	
    sprite->oam.tileNum += gBattleAnimArgs[2] * 4;
	
    switch (gBattleAnimArgs[1])
    {
    case 0:
        sprite->x = GetBattlerSpriteCoordAttr(battler, BATTLER_COORD_ATTR_RIGHT) - 8;
        sprite->y = GetBattlerSpriteCoordAttr(battler, BATTLER_COORD_ATTR_TOP) + 8;
        break;
    case 1:
        sprite->x = GetBattlerSpriteCoordAttr(battler, BATTLER_COORD_ATTR_RIGHT) - 14;
        sprite->y = GetBattlerSpriteCoordAttr(battler, BATTLER_COORD_ATTR_TOP) + 16;
        break;
    case 2:
        sprite->x = GetBattlerSpriteCoordAttr(battler, BATTLER_COORD_ATTR_LEFT) + 8;
        sprite->y = GetBattlerSpriteCoordAttr(battler, BATTLER_COORD_ATTR_TOP) + 8;
        StartSpriteAffineAnim(sprite, 1);
        xOffset = -xOffset;
        break;
    case 3:
        sprite->x = GetBattlerSpriteCoordAttr(battler, BATTLER_COORD_ATTR_LEFT) + 14;
        sprite->y = GetBattlerSpriteCoordAttr(battler, BATTLER_COORD_ATTR_TOP) + 16;
        StartSpriteAffineAnim(sprite, 1);
        xOffset = -xOffset;
        break;
    }
    sprite->data[0] = 32;
    sprite->data[2] = sprite->x + xOffset;
    sprite->data[4] = sprite->y + 12;
    sprite->data[5] = -12;
    InitAnimArcTranslation(sprite);
    sprite->callback = DestroyAnimSpriteAfterHorizontalTranslation;
}

// Animates the attacker's shadow in Memento's anim.
// No args.
void AnimTask_MoveAttackerMementoShadow(u8 taskId)
{
    struct ScanlineEffectParams scanlineParams;
    struct BattleAnimBgData animBg;
    u16 i;
    u8 pos;
    s32 var0;
    struct Task *task = &gTasks[taskId];

    task->data[7] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y) + 31;
    task->data[6] = GetBattlerSpriteCoordAttr(gBattleAnimAttacker, BATTLER_COORD_ATTR_TOP) - 7;
    task->data[5] = task->data[7];
    task->data[4] = task->data[6];
    task->data[13] = (task->data[7] - task->data[6]) << 8;
	
    pos = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    task->data[14] = pos - 32;
    task->data[15] = pos + 32;
	task->data[8] = GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER ? -12 : -64;
    task->data[3] = GetBattlerSpriteBGPriorityRank(gBattleAnimAttacker);
	
    if (task->data[3] == 1)
    {
        GetBattleAnimBgData(&animBg, 1);
		
        task->data[10] = gBattle_BG1_Y;
		
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_BG1);
		
        FillPalette(0, animBg.paletteId * 16, 32);
		
        scanlineParams.dmaDest = &REG_BG1VOFS;
		
        var0 = WINOUT_WIN01_BG1;
		
        gBattle_BG2_X += 240;
    }
    else
    {
        task->data[10] = gBattle_BG2_Y;
		
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_BG2);
		
        FillPalette(0, 144, 32);
		
        scanlineParams.dmaDest = &REG_BG2VOFS;
		
        var0 = WINOUT_WIN01_BG2;
		
        gBattle_BG1_X += 240;
    }
    scanlineParams.dmaControl = SCANLINE_EFFECT_DMACNT_16BIT;
    scanlineParams.initState = 1;

    task->data[11] = 0;
    task->data[12] = 16;
    task->data[0] = 0;
    task->data[1] = 0;
    task->data[2] = 0;
    SetPriorityForAllVisibleBattlers(3);
	
    for (i = 0; i < 112; ++i)
    {
        gScanlineEffectRegBuffers[0][i] = task->data[10];
        gScanlineEffectRegBuffers[1][i] = task->data[10];
    }
    ScanlineEffect_SetParams(scanlineParams);
	
    SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WINOBJ_BG_ALL | WINOUT_WINOBJ_OBJ | WINOUT_WINOBJ_CLR | (var0 ^ (WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR)));
    SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG_ALL | WININ_WIN1_OBJ | WININ_WIN1_CLR);
	
    gBattle_WIN0H = (task->data[14] << 8) | task->data[15];
    gBattle_WIN0V = 160;

    task->func = AnimTask_MoveAttackerMementoShadow_Step;
}

static void AnimTask_MoveAttackerMementoShadow_Step(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        if (++task->data[1] > 1)
        {
            task->data[1] = 0;
			
            if (++task->data[2] & 1)
            {
                if (task->data[11] != 12)
                    ++task->data[11];
            }
            else if (task->data[12] != 8)
                --task->data[12];

            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(task->data[11], task->data[12]));
			
            if (task->data[11] == 12 && task->data[12] == 8)
                ++task->data[0];
        }
        break;
    case 1:
        task->data[4] -= 8;
		
        DoMementoShadowEffect(task);
		
        if (task->data[4] < task->data[8])
            ++task->data[0];
        break;
    case 2:
        task->data[4] -= 8;
		
        DoMementoShadowEffect(task);
		
        task->data[14] += 4;
        task->data[15] -= 4;
		
        if (task->data[14] >= task->data[15])
            task->data[14] = task->data[15];
		
        gBattle_WIN0H = (task->data[14] << 8) | task->data[15];
		
        if (task->data[14] == task->data[15])
            ++task->data[0];
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

// Animates the target's shadow in Memento's anim.
// No args.
void AnimTask_MoveTargetMementoShadow(u8 taskId)
{
    struct BattleAnimBgData animBg;
    struct ScanlineEffectParams scanlineParams;
    u8 x;
    u16 i;
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        task->data[3] = GetBattlerSpriteBGPriorityRank(gBattleAnimTarget);
		
        if (task->data[3] == 1)
        {
            SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_BG1);
            gBattle_BG2_X += 240;
        }
        else
        {
            SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_BG2);
            gBattle_BG1_X += 240;
        }
        ++task->data[0];
        break;
    case 1:
        if (task->data[3] == 1)
        {
            GetBattleAnimBgData(&animBg, 1);
            task->data[10] = gBattle_BG1_Y;
            FillPalette(0, animBg.paletteId * 16, 32);
        }
        else
        {
            task->data[10] = gBattle_BG2_Y;
            FillPalette(0, 9 * 16, 32);
        }
        SetPriorityForAllVisibleBattlers(3);
        ++task->data[0];
        break;
    case 2:
        task->data[7] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y) + 31;
        task->data[6] = GetBattlerSpriteCoordAttr(gBattleAnimTarget, BATTLER_COORD_ATTR_TOP) - 7;
        task->data[13] = (task->data[7] - task->data[6]) << 8;
		
        x = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
		
        task->data[14] = x - 4;
        task->data[15] = x + 4;
		task->data[8] = GetBattlerSide(gBattleAnimTarget) == B_SIDE_PLAYER ? -12 : -64;
        task->data[4] = task->data[8];
        task->data[5] = task->data[8];
        task->data[11] = 12;
        task->data[12] = 8;
        ++task->data[0];
        break;
    case 3:
		scanlineParams.dmaDest = task->data[3] == 1 ? &REG_BG1VOFS : &REG_BG2VOFS;
		
        for (i = 0; i < 112; ++i)
        {
            gScanlineEffectRegBuffers[0][i] = task->data[10] + (159 - i);
            gScanlineEffectRegBuffers[1][i] = task->data[10] + (159 - i);
        }
        scanlineParams.dmaControl = SCANLINE_EFFECT_DMACNT_16BIT;
        scanlineParams.initState = 1;
        ScanlineEffect_SetParams(scanlineParams);
        ++task->data[0];
        break;
    case 4:
        if (task->data[3] == 1)
            SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WINOBJ_BG_ALL | WINOUT_WINOBJ_OBJ | WINOUT_WINOBJ_CLR | WINOUT_WIN01_BG0 | WINOUT_WIN01_BG2 | WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
        else
            SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WINOBJ_BG_ALL | WINOUT_WINOBJ_OBJ | WINOUT_WINOBJ_CLR | WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
        
		SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG_ALL | WININ_WIN1_OBJ | WININ_WIN1_CLR);
		
        gBattle_WIN0H = (task->data[14] << 8) | task->data[15];
        gBattle_WIN0V = 160;
		
        task->data[0] = 0;
        task->data[1] = 0;
        task->data[2] = 0;
		
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(12, 8));
		
        task->func = AnimTask_MoveTargetMementoShadow_Step;
        break;
    }
}

static void AnimTask_MoveTargetMementoShadow_Step(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        task->data[5] += 8;
		
        if (task->data[5] >= task->data[7])
            task->data[5] = task->data[7];
		
        DoMementoShadowEffect(task);
		
        if (task->data[5] == task->data[7])
            ++task->data[0];
        break;
    case 1:
        if (task->data[15] - task->data[14] < 0x40)
        {
            task->data[14] -= 4;
            task->data[15] += 4;
        }
        else
            task->data[1] = 1;

        gBattle_WIN0H = (task->data[14] << 8) | task->data[15];
		
        task->data[4] += 8;
		
        if (task->data[4] >= task->data[6])
            task->data[4] = task->data[6];
		
        DoMementoShadowEffect(task);
		
        if (task->data[4] == task->data[6] && task->data[1])
        {
            task->data[1] = 0;
            ++task->data[0];
        }
        break;
    case 2:
        if (++task->data[1] > 1)
        {
            task->data[1] = 0;
			
            if (++task->data[2] & 1)
            {
                if (task->data[11])
                    --task->data[11];
            }
            else if (task->data[12] < 16)
                ++task->data[12];

            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(task->data[11], task->data[12]));
			
            if (task->data[11] == 0 && task->data[12] == 16)
                ++task->data[0];
        }
        break;
    case 3:
        gScanlineEffect.state = 3;
        ++task->data[0];
        break;
    case 4:
        gBattle_WIN0H = 0;
        gBattle_WIN0V = 0;
		
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG_ALL | WININ_WIN1_OBJ | WININ_WIN1_CLR);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WINOBJ_BG_ALL | WINOUT_WINOBJ_OBJ | WINOUT_WINOBJ_CLR | WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
		
        DestroyAnimVisualTask(taskId);
        break;
    }
}

static void DoMementoShadowEffect(struct Task *task)
{
    s32 var0, var1, var4;
    s16 i, var2 = task->data[5] - task->data[4];

    if (var2 != 0)
    {
        var0 = task->data[13] / var2;
        var1 = task->data[6] << 8;
		
        for (i = 0; i < task->data[4]; ++i)
            gScanlineEffectRegBuffers[gScanlineEffect.srcBuffer][i] = task->data[10] - (i - 159);
		
        for (i = task->data[4]; i <= task->data[5]; ++i)
        {
            if (i >= 0)
            {
                s16 var3 = (var1 >> 8) - i;

                gScanlineEffectRegBuffers[gScanlineEffect.srcBuffer][i] = var3 + task->data[10];
            }
            var1 += var0;
        }
        var4 = task->data[10] - (i - 159);
		
        for (; i < task->data[7]; ++i)
		{
            if (i >= 0)
                gScanlineEffectRegBuffers[gScanlineEffect.srcBuffer][i] = var4--;
		}
    }
    else
    {
        var4 = task->data[10] + 159;
		
        for (i = 0; i < 112; ++i)
        {
            gScanlineEffectRegBuffers[0][i] = var4;
            gScanlineEffectRegBuffers[1][i] = var4;
            --var4;
        }
    }
}

static void SetPriorityForAllVisibleBattlers(u8 priority)
{
    u8 i;

    for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
    {
        u8 spriteId = GetAnimBattlerSpriteId(i);

        if (spriteId != 0xFF)
            gSprites[spriteId].oam.priority = priority;
    }
}

// Init attacker's sprite for Memento shadow effect.
// No args.
void AnimTask_InitMementoShadow(u8 taskId)
{
    bool8 toBG2 = (GetBattlerSpriteBGPriorityRank(gBattleAnimAttacker) ^ 1);

    MoveBattlerSpriteToBG(gBattleAnimAttacker, toBG2);
    gSprites[gBattlerSpriteIds[gBattleAnimAttacker]].invisible = FALSE;
	
    if (IsBattlerSpriteVisible(BATTLE_PARTNER(gBattleAnimAttacker)))
    {
        MoveBattlerSpriteToBG(BATTLE_PARTNER(gBattleAnimAttacker), toBG2 ^ TRUE);
        gSprites[gBattlerSpriteIds[BATTLE_PARTNER(gBattleAnimAttacker)]].invisible = FALSE;
    }
    DestroyAnimVisualTask(taskId);
}

// Ends attacker's sprite for Memento shadow effect.
// No args.
void AnimTask_EndMementoShadow(u8 taskId)
{
    bool8 toBG2 = (GetBattlerSpriteBGPriorityRank(gBattleAnimAttacker) ^ 1);
    
    ResetBattleAnimBg(toBG2);
	
    if (IsBattlerSpriteVisible(BATTLE_PARTNER(gBattleAnimAttacker)))
        ResetBattleAnimBg(toBG2 ^ TRUE);
	
    DestroyAnimVisualTask(taskId);
}

// Animates the claw slash sprite. Used by moves like Metal and Dragon claw.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: sprite anim num
static void AnimClawSlash(struct Sprite *sprite)
{
	StartSpriteAnim(sprite, gBattleAnimArgs[2]);
	
    sprite->x += gBattleAnimArgs[0];
    sprite->y += gBattleAnimArgs[1];
	
    sprite->callback = RunStoredCallbackWhenAnimEnds;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Changes battler's palette to either greyscale or original.
// arg 0: anim battler
// arg 1: set original (boolean)
void AnimTask_SetGreyscaleOrOriginalPal(u8 taskId)
{
    u8 spriteId, position, battler;
    bool8 calcSpriteId = FALSE;

    switch (gBattleAnimArgs[0])
    {
    case 0 ... MAX_BATTLERS_COUNT - 1:
        spriteId = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);
        break;
    case MAX_BATTLERS_COUNT:
        position = B_POSITION_PLAYER_LEFT;
        calcSpriteId = TRUE;
        break;
    case MAX_BATTLERS_COUNT + 1:
        position = B_POSITION_PLAYER_RIGHT;
        calcSpriteId = TRUE;
        break;
    case MAX_BATTLERS_COUNT + 2:
        position = B_POSITION_OPPONENT_LEFT;
        calcSpriteId = TRUE;
        break;
    case MAX_BATTLERS_COUNT + 3:
        position = B_POSITION_OPPONENT_RIGHT;
        calcSpriteId = TRUE;
        break;
    default:
        spriteId = 0xFF;
        break;
    }
	
    if (calcSpriteId)
    {
        battler = GetBattlerAtPosition(position);
		spriteId = IsBattlerSpriteVisible(battler) ? gBattlerSpriteIds[battler] : 0xFF;
    }
	
    if (spriteId != 0xFF)
        SetGreyscaleOrOriginalPalette(gSprites[spriteId].oam.paletteNum + 16, gBattleAnimArgs[1]);
	
    DestroyAnimVisualTask(taskId);
}

// Changes the backgound's palette to either greyscale or original.
// arg 0: set original (boolean)
void AnimTask_GreyscaleBackgound(u8 taskId)
{
	SetGreyscaleOrOriginalPalette(1, gBattleAnimArgs[0]);
	SetGreyscaleOrOriginalPalette(2, gBattleAnimArgs[0]);
	SetGreyscaleOrOriginalPalette(3, gBattleAnimArgs[0]);
	DestroyAnimVisualTask(taskId);
}

// Animates MOVE_TORMENT's thought bubbles on the attacker.
// No args.
void AnimTask_TormentAttacker(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    task->data[0] = 0;
    task->data[1] = 0;
    task->data[2] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    task->data[3] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
    task->data[4] = 32;
    task->data[5] = -20;
    task->data[6] = 0;
    task->data[15] = GetAnimBattlerSpriteId(ANIM_ATTACKER);
    task->func = TormentAttacker_Step;
}

static void TormentAttacker_Step(u8 taskId)
{
    int var0, var1;
    s16 x, y;
    u16 i, j;
    u8 spriteId;
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        var0 = task->data[2];
		
        if (task->data[1] & 1)
        {
            var1 = task->data[4];
            x = var0 - var1;
        }
        else
        {
            var1 = task->data[4];
            x = var0 + var1;
        }
        y = task->data[3] + task->data[5];
		
        spriteId = CreateSprite(&gThoughtBubbleSpriteTemplate, x, y, 6 - task->data[1]);
        PlaySE12WithPanning(SE_M_METRONOME, BattleAnimAdjustPanning(SOUND_PAN_ATTACKER));

        if (spriteId != MAX_SPRITES)
        {
            gSprites[spriteId].hFlip = task->data[1] & 1;
            gSprites[spriteId].callback = SpriteCallbackDummy;
        }

        if (task->data[1] & 1)
        {
            task->data[4] -= 6;
            task->data[5] -= 6;
        }
        PrepareAffineAnimInTaskData(task, task->data[15], sTormentAttackerAffineAnimCmds);
		
        task->data[1]++;
        task->data[0] = 1;
        break;
    case 1:
        if (!RunAffineAnimFromTaskData(task))
        {
            if (task->data[1] == 6)
            {
                task->data[6] = 8;
                task->data[0] = 3;
            }
            else
            {
                if (task->data[1] <= 2)
                    task->data[6] = 10;
                else
                    task->data[6] = 0;

                task->data[0] = 2;
            }
        }
        break;
    case 2:
        if (task->data[6] != 0)
            task->data[6]--;
        else
            task->data[0] = 0;
        break;
    case 3:
        if (task->data[6] != 0)
            task->data[6]--;
        else
            task->data[0] = 4;
        break;
    case 4:
        for (i = 0, j = 0; i < MAX_SPRITES; i++)
        {
            if (gSprites[i].template == &gThoughtBubbleSpriteTemplate)
            {
                gSprites[i].data[0] = taskId;
                gSprites[i].data[1] = 6;
				gSprites[i].callback = TormentAttacker_Callback;
				
                StartSpriteAnim(&gSprites[i], 2);
                
                if (++j == 6)
                    break;
            }
        }
        task->data[6] = j;
        task->data[0] = 5;
        break;
    case 5:
        if (task->data[6] == 0)
            DestroyAnimVisualTask(taskId);
        break;
    }
}

static void TormentAttacker_Callback(struct Sprite *sprite)
{
    if (sprite->animEnded)
    {
        gTasks[sprite->data[0]].data[sprite->data[1]]--;
        DestroySprite(sprite);
    }
}

// Animates Taunt's finger sprite.
// arg 0: anim battler
static void AnimTauntFinger(struct Sprite* sprite)
{
    u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);
    
    SetSpriteNextToMonHead(battler, sprite);
	
    if (GetBattlerSide(battler) == B_SIDE_PLAYER)
    {
        StartSpriteAnim(sprite, 0);
        sprite->data[0] = 2;
    }
    else
    {
        StartSpriteAnim(sprite, 1);
        sprite->data[0] = 3;
    }
    sprite->callback = AnimTauntFingerStep1;
}

static void AnimTauntFingerStep1(struct Sprite* sprite)
{
    if (++sprite->data[1] > 10)
    {
        sprite->data[1] = 0;
        StartSpriteAnim(sprite, sprite->data[0]);
        StoreSpriteCallbackInData6(sprite, AnimTauntFingerStep2);
        sprite->callback = RunStoredCallbackWhenAnimEnds;
    }
}

static void AnimTauntFingerStep2(struct Sprite* sprite)
{
    if (++sprite->data[1] > 5)
        DestroyAnimSprite(sprite);
}

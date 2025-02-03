#include "global.h"
#include "gflib.h"
#include "battle_anim.h"
#include "random.h"
#include "task.h"
#include "trig.h"

static void AnimConfusionDuck(struct Sprite *sprite);
static void AnimSimplePaletteBlend(struct Sprite *sprite);
static void AnimComplexPaletteBlend(struct Sprite *sprite);
static void AnimShakeMonOrBattleTerrain(struct Sprite *sprite);
static void AnimShakeMonOrBattleTerrain_UpdateCoordOffsetEnabled(u8 dontShakeBattler);
static void AnimShakeMonOrBattleTerrain_Step(struct Sprite *sprite);
static void AnimHitSplatBasic(struct Sprite *sprite);
static void AnimHitSplatHandleInvert(struct Sprite *sprite);
static void AnimHitSplatRandom(struct Sprite *sprite);
static void AnimHitSplatOnMonEdge(struct Sprite *sprite);
static void AnimCrossImpact(struct Sprite *sprite);
static void AnimFlashingHitSplat(struct Sprite *sprite);
static void AnimFlashingHitSplat_Step(struct Sprite *sprite);
static void AnimHitSplatPersistent(struct Sprite *sprite);
static void AnimConfusionDuckStep(struct Sprite *sprite);
static void AnimSimplePaletteBlendStep(struct Sprite *sprite);
static void AnimComplexPaletteBlendStep(struct Sprite *sprite);
static void AnimComplexPaletteBlendStep2(struct Sprite *sprite);
static void AnimShellSmashShell(struct Sprite *sprite);
static void AnimShellSmashShell_Step(struct Sprite *sprite);
static void AnimClampShell(struct Sprite *sprite);
static void BlendColorCycle(u8 taskId, u8 initialBlendAmount, u8 targetBlendAmount);
static void AnimTask_BlendColorCycleLoop(u8 taskId);
static void BlendColorCycleExclude(u8 taskId, u8 initialBlendAmount, u8 targetBlendAmount);
static void AnimTask_BlendColorCycleExcludeLoop(u8 taskId);
static void BlendColorCycleByTag(u8 taskId, u8 initialBlendAmount, u8 targetBlendAmount);
static void AnimTask_BlendColorCycleByTagLoop(u8 taskId);
static void sub_80B9FD8(u8 taskId);
static void sub_80BA090(u8 taskId);
static void AnimTask_ShakeBattleTerrain_Step(u8 taskId);
static void AnimTask_FadeOutParticles_Step(u8 taskId);
static void SpriteCB_WeatherBallUp_Step(struct Sprite *sprite);

static const union AnimCmd sAnim_ConfusionDuck_0[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(4, 8),
    ANIMCMD_FRAME(0, 8, .hFlip = TRUE),
    ANIMCMD_FRAME(8, 8),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_ConfusionDuck_1[] =
{
    ANIMCMD_FRAME(0, 8, .hFlip = TRUE),
    ANIMCMD_FRAME(4, 8),
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(8, 8),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnims_ConfusionDuck[] =
{
    sAnim_ConfusionDuck_0,
    sAnim_ConfusionDuck_1,
};

const struct SpriteTemplate gConfusionDuckSpriteTemplate =
{
    .tileTag = ANIM_TAG_DUCK,
    .paletteTag = ANIM_TAG_DUCK,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = sAnims_ConfusionDuck,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimConfusionDuck,
};

const struct SpriteTemplate gSimplePaletteBlendSpriteTemplate =
{
    .tileTag = 0,
    .paletteTag = 0,
    .oam = &gDummyOamData,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSimplePaletteBlend,
};

const struct SpriteTemplate gComplexPaletteBlendSpriteTemplate =
{
    .tileTag = 0,
    .paletteTag = 0,
    .oam = &gDummyOamData,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimComplexPaletteBlend,
};

const struct SpriteTemplate gShakeMonOrTerrainSpriteTemplate =
{
    .tileTag = 0,
    .paletteTag = 0,
    .oam = &gDummyOamData,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimShakeMonOrBattleTerrain,
};

static const union AffineAnimCmd sAffineAnim_HitSplat_0[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, 0, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_HitSplat_1[] =
{
    AFFINEANIMCMD_FRAME(0xD8, 0xD8, 0, 0),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 0, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_HitSplat_2[] =
{
    AFFINEANIMCMD_FRAME(0xB0, 0xB0, 0, 0),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 0, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_HitSplat_3[] =
{
    AFFINEANIMCMD_FRAME(0x80, 0x80, 0, 0),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 0, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims_HitSplat[] =
{
    sAffineAnim_HitSplat_0,
    sAffineAnim_HitSplat_1,
    sAffineAnim_HitSplat_2,
    sAffineAnim_HitSplat_3,
};

const struct SpriteTemplate gBasicHitSplatSpriteTemplate =
{
    .tileTag = ANIM_TAG_IMPACT,
    .paletteTag = ANIM_TAG_IMPACT,
    .oam = &gOamData_AffineNormal_ObjBlend_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_HitSplat,
    .callback = AnimHitSplatBasic,
};

const struct SpriteTemplate gHandleInvertHitSplatSpriteTemplate =
{
    .tileTag = ANIM_TAG_IMPACT,
    .paletteTag = ANIM_TAG_IMPACT,
    .oam = &gOamData_AffineNormal_ObjBlend_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_HitSplat,
    .callback = AnimHitSplatHandleInvert,
};

const struct SpriteTemplate gWaterHitSplatSpriteTemplate =
{
    .tileTag = ANIM_TAG_WATER_IMPACT,
    .paletteTag = ANIM_TAG_WATER_IMPACT,
    .oam = &gOamData_AffineNormal_ObjBlend_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_HitSplat,
    .callback = AnimHitSplatBasic,
};

const struct SpriteTemplate gRandomPosHitSplatSpriteTemplate =
{
    .tileTag = ANIM_TAG_IMPACT,
    .paletteTag = ANIM_TAG_IMPACT,
    .oam = &gOamData_AffineNormal_ObjBlend_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_HitSplat,
    .callback = AnimHitSplatRandom,
};

const struct SpriteTemplate gMonEdgeHitSplatSpriteTemplate =
{
    .tileTag = ANIM_TAG_IMPACT,
    .paletteTag = ANIM_TAG_IMPACT,
    .oam = &gOamData_AffineNormal_ObjBlend_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_HitSplat,
    .callback = AnimHitSplatOnMonEdge,
};

const struct SpriteTemplate gCrossImpactSpriteTemplate =
{
    .tileTag = ANIM_TAG_CROSS_IMPACT,
    .paletteTag = ANIM_TAG_CROSS_IMPACT,
    .oam = &gOamData_AffineOff_ObjBlend_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimCrossImpact,
};

const struct SpriteTemplate gFlashingHitSplatSpriteTemplate =
{
    .tileTag = ANIM_TAG_IMPACT,
    .paletteTag = ANIM_TAG_IMPACT,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_HitSplat,
    .callback = AnimFlashingHitSplat,
};

const struct SpriteTemplate gPersistHitSplatSpriteTemplate =
{
    .tileTag = ANIM_TAG_IMPACT,
    .paletteTag = ANIM_TAG_IMPACT,
    .oam = &gOamData_AffineNormal_ObjBlend_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_HitSplat,
    .callback = AnimHitSplatPersistent,
};

static const union AnimCmd sAnim_LeftShell[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_RightShell[] =
{
    ANIMCMD_FRAME(64, 0),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_LeftRightShell[] =
{
    sAnim_LeftShell,
    sAnim_RightShell,
};

static const union AffineAnimCmd sAffineAnim_LeftShell[] =
{
    AFFINEANIMCMD_FRAME(0, 0, 64, 1),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_RightShell[] =
{
    AFFINEANIMCMD_FRAME(0, 0, -64, 1),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims_LeftRightShell[] =
{
    sAffineAnim_LeftShell,
    sAffineAnim_RightShell,
};

const struct SpriteTemplate gLeftRightShellSpriteTemplate =
{
	.tileTag = ANIM_TAG_SHELL,
	.paletteTag = ANIM_TAG_SHELL,
	.oam = &gOamData_AffineNormal_ObjBlend_64x64,
	.anims = sAnims_LeftRightShell,
	.images = NULL,
	.affineAnims = sAffineAnims_LeftRightShell,
	.callback = AnimShellSmashShell,
};

const struct SpriteTemplate gClampShellSpriteTemplate =
{
    .tileTag = ANIM_TAG_SHELL,
    .paletteTag = ANIM_TAG_SHELL,
    .oam = &gOamData_AffineNormal_ObjBlend_64x64,
    .anims = sAnims_LeftRightShell,
    .images = NULL,
    .affineAnims = gAffineAnims_Bite,
    .callback = AnimClampShell,
};

static const union AffineAnimCmd sAffineAnim_ShellSmashStretchAttacker[] =
{
    AFFINEANIMCMD_FRAME(64, 64, 0, 16),
	AFFINEANIMCMD_FRAME(0, 0, 0, 48),
	AFFINEANIMCMD_FRAME(-64, -64, 0, 16),
	AFFINEANIMCMD_END,
};

static const union AnimCmd sSpriteAnimCmds_WeatherBallUp[] =
{
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd *const sSpriteAnimTable_WeatherBallUp[] =
{
    sSpriteAnimCmds_WeatherBallUp
};

const struct SpriteTemplate gWeatherBallUpSpriteTemplate =
{
    .tileTag = ANIM_TAG_WEATHER_BALL,
    .paletteTag = ANIM_TAG_WEATHER_BALL,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sSpriteAnimTable_WeatherBallUp,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_WeatherBallUp,
};

const struct SpriteTemplate gWeatherBallNormalDownSpriteTemplate =
{
    .tileTag = ANIM_TAG_WEATHER_BALL,
    .paletteTag = ANIM_TAG_WEATHER_BALL,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sSpriteAnimTable_WeatherBallUp,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimWeatherBallDown,
};

const struct SpriteTemplate gGyroBallCircleHitSpriteTemplate =    
{
    .tileTag = ANIM_TAG_YELLOW_BALL,
    .paletteTag = ANIM_TAG_GUST,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_HitSplat,
    .callback = AnimHitSplatBasic,
};

const struct SpriteTemplate gAcupressureFingerSpriteTemplate =    
{
    .tileTag = ANIM_TAG_ACUPRESSURE_FINGER,
    .paletteTag = ANIM_TAG_ACUPRESSURE_FINGER,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimStompFoot,
};

const struct SpriteTemplate gAssuranceHandSpriteTemplate =
{
    .tileTag = ANIM_TAG_ASSURANCE_HAND,
    .paletteTag = ANIM_TAG_ASSURANCE_HAND,
    .oam = &gOamData_AffineNormal_ObjBlend_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_HitSplat,
    .callback = AnimHitSplatBasic,
};

const struct SpriteTemplate gPunishmentPurpleHitSpriteTemplate =
{
    .tileTag = ANIM_TAG_IMPACT,
    .paletteTag = ANIM_TAG_POISON_BUBBLE,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_HitSplat,
    .callback = AnimHitSplatBasic,
};

// Moves a spinning sprite around the mon's head.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: initial wave offset
// arg 3: wave period (higher means faster wave)
// arg 4: duration
static void AnimConfusionDuck(struct Sprite *sprite)
{
    sprite->x += gBattleAnimArgs[0];
    sprite->y += gBattleAnimArgs[1];
    sprite->data[0] = gBattleAnimArgs[2];
	
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        sprite->data[1] = -gBattleAnimArgs[3];
    else
    {
        sprite->data[1] = gBattleAnimArgs[3];
        StartSpriteAnim(sprite, 1);
    }
    sprite->data[3] = gBattleAnimArgs[4];
    sprite->callback = AnimConfusionDuckStep;
    sprite->callback(sprite);
}

static void AnimConfusionDuckStep(struct Sprite *sprite)
{
    sprite->x2 = Cos(sprite->data[0], 30);
    sprite->y2 = Sin(sprite->data[0], 10);
	
    if ((u16)sprite->data[0] < 128)
        sprite->oam.priority = 1;
    else
        sprite->oam.priority = 3;
	
    sprite->data[0] = (sprite->data[0] + sprite->data[1]) & 0xFF;
	
    if (++sprite->data[2] == sprite->data[3])
        DestroyAnimSprite(sprite);
}

// Performs a simple color blend on a specified sprite.
// arg 0: palette selector
// arg 1: delay
// arg 2: start blend amount
// arg 3: end blend amount
// arg 4: blend color
static void AnimSimplePaletteBlend(struct Sprite *sprite)
{
    BeginNormalPaletteFade(UnpackSelectedBattleAnimPalettes(gBattleAnimArgs[0]), gBattleAnimArgs[1], gBattleAnimArgs[2], gBattleAnimArgs[3], gBattleAnimArgs[4]);
    sprite->invisible = TRUE;
    sprite->callback = AnimSimplePaletteBlendStep;
}

// Unpacks a bitfield and returns a bitmask of its selected palettes.
// Bits 0-6 of the selector parameter result in the following palettes being selected:
//   0: F_PAL_BG, battle background palettes (BG palettes 1, 2, and 3)
//   1: F_PAL_ATTACKER, gBattleAnimAttacker OBJ palette
//   2: F_PAL_TARGET, gBattleAnimTarget OBJ palette
//   3: F_PAL_ATK_PARTNER, gBattleAnimAttacker partner OBJ palette
//   4: F_PAL_DEF_PARTNER, gBattleAnimTarget partner OBJ palette
//   5: F_PAL_ANIM_1, BG palette 8
//   6: F_PAL_ANIM_2, BG palette 9
u32 UnpackSelectedBattleAnimPalettes(s16 selector)
{
    u8 battleBackground = selector & 1;
    u8 attacker = (selector >> 1) & 1;
    u8 target = (selector >> 2) & 1;
    u8 attackerPartner = (selector >> 3) & 1;
    u8 targetPartner = (selector >> 4) & 1;
    u8 arg5 = (selector >> 5) & 1;
    u8 arg6 = (selector >> 6) & 1;

    return SelectBattleAnimSpriteAndBgPalettes(battleBackground, attacker, target, attackerPartner, targetPartner, arg5, arg6);
}

static void AnimSimplePaletteBlendStep(struct Sprite *sprite)
{
    if (!gPaletteFade.active)
        DestroyAnimSprite(sprite);
}

static void AnimComplexPaletteBlend(struct Sprite *sprite)
{
    sprite->data[0] = gBattleAnimArgs[1];
    sprite->data[1] = gBattleAnimArgs[1];
    sprite->data[2] = gBattleAnimArgs[2];
    sprite->data[3] = gBattleAnimArgs[3];
    sprite->data[4] = gBattleAnimArgs[4];
    sprite->data[5] = gBattleAnimArgs[5];
    sprite->data[6] = gBattleAnimArgs[6];
    sprite->data[7] = gBattleAnimArgs[0];
    BlendPalettes(UnpackSelectedBattleAnimPalettes(sprite->data[7]), gBattleAnimArgs[4], gBattleAnimArgs[3]);
    sprite->invisible = TRUE;
    sprite->callback = AnimComplexPaletteBlendStep;
}

static void AnimComplexPaletteBlendStep(struct Sprite *sprite)
{
    u32 selectedPalettes;

    if (sprite->data[0] > 0)
    {
        --sprite->data[0];
        return;
    }
	
    if (gPaletteFade.active)
        return;
	
    if (sprite->data[2] == 0)
    {
        sprite->callback = AnimComplexPaletteBlendStep2;
        return;
    }
    selectedPalettes = UnpackSelectedBattleAnimPalettes(sprite->data[7]);
    if (sprite->data[1] & 0x100)
        BlendPalettes(selectedPalettes, sprite->data[4], sprite->data[3]);
    else
        BlendPalettes(selectedPalettes, sprite->data[6], sprite->data[5]);
	
    sprite->data[1] ^= 0x100;
    sprite->data[0] = sprite->data[1] & 0xFF;
    --sprite->data[2];
}

static void AnimComplexPaletteBlendStep2(struct Sprite *sprite)
{
    if (!gPaletteFade.active)
    {
        BlendPalettes(UnpackSelectedBattleAnimPalettes(sprite->data[7]), 0, 0);
        DestroyAnimSprite(sprite);
    }
}

// Blends mon/screen to designated color or back alternately by arg 2' times
// Many uses of this task only set a arg 2 as 2, which has the effect of blending to a color and back once.
// arg 0: see values for UnpackSelectedBattleAnimPalettes
// arg 1: delay
// arg 2: duration
// arg 3: initial blend amount
// arg 4: target blend amount
// arg 5: blend color
void AnimTask_BlendColorCycle(u8 taskId)
{
    gTasks[taskId].data[0] = gBattleAnimArgs[0];
    gTasks[taskId].data[1] = gBattleAnimArgs[1];
    gTasks[taskId].data[2] = gBattleAnimArgs[2];
    gTasks[taskId].data[3] = gBattleAnimArgs[3];
    gTasks[taskId].data[4] = gBattleAnimArgs[4];
    gTasks[taskId].data[5] = gBattleAnimArgs[5];
    gTasks[taskId].data[8] = FALSE;
    BlendColorCycle(taskId, 0, gTasks[taskId].data[4]);
    gTasks[taskId].func = AnimTask_BlendColorCycleLoop;
}

static void BlendColorCycle(u8 taskId, u8 initialBlendAmount, u8 targetBlendAmount)
{
    BeginNormalPaletteFade(UnpackSelectedBattleAnimPalettes(gTasks[taskId].data[0]), gTasks[taskId].data[1], initialBlendAmount, targetBlendAmount, gTasks[taskId].data[5]);
    --gTasks[taskId].data[2];
    gTasks[taskId].data[8] ^= TRUE;
}

static void AnimTask_BlendColorCycleLoop(u8 taskId)
{
    u8 initialBlendAmount, targetBlendAmount;

    if (!gPaletteFade.active)
    {
        if (gTasks[taskId].data[2] > 0)
        {
            if (!gTasks[taskId].data[8])
            {
                initialBlendAmount = gTasks[taskId].data[3];
                targetBlendAmount = gTasks[taskId].data[4];
            }
            else
            {
                initialBlendAmount = gTasks[taskId].data[4];
                targetBlendAmount = gTasks[taskId].data[3];
            }
			
            if (gTasks[taskId].data[2] == 1)
                targetBlendAmount = 0;
			
            BlendColorCycle(taskId, initialBlendAmount, targetBlendAmount);
        }
        else
            DestroyAnimVisualTask(taskId);
    }
}

// See AnimTask_BlendColorCycle. Same, but excludes Attacker and Target.
void AnimTask_BlendColorCycleExclude(u8 taskId)
{
    u8 battler;
    u32 selectedPalettes = 0xE;
	
	for (battler = 0; battler < gBattlersCount; ++battler)
	{
        if (battler != gBattleAnimAttacker && battler != gBattleAnimTarget)
            selectedPalettes |= 1 << (battler + 16);
	}
    gTasks[taskId].data[0] = gBattleAnimArgs[0];
    gTasks[taskId].data[1] = gBattleAnimArgs[1];
    gTasks[taskId].data[2] = gBattleAnimArgs[2];
    gTasks[taskId].data[3] = gBattleAnimArgs[3];
    gTasks[taskId].data[4] = gBattleAnimArgs[4];
    gTasks[taskId].data[5] = gBattleAnimArgs[5];
    gTasks[taskId].data[8] = 0;
    gTasks[taskId].data[9] = selectedPalettes >> 16;
    gTasks[taskId].data[10] = selectedPalettes & 0xFF;
	
    BlendColorCycleExclude(taskId, 0, gTasks[taskId].data[4]);
	
    gTasks[taskId].func = AnimTask_BlendColorCycleExcludeLoop;
}

static void BlendColorCycleExclude(u8 taskId, u8 initialBlendAmount, u8 targetBlendAmount)
{
    BeginNormalPaletteFade(((u16)gTasks[taskId].data[9] << 16) | (u16)gTasks[taskId].data[10], gTasks[taskId].data[1], initialBlendAmount, targetBlendAmount, gTasks[taskId].data[5]);
    --gTasks[taskId].data[2];
    gTasks[taskId].data[8] ^= TRUE;
}

static void AnimTask_BlendColorCycleExcludeLoop(u8 taskId)
{
    u8 initialBlendAmount, targetBlendAmount;

    if (!gPaletteFade.active)
    {
        if (gTasks[taskId].data[2] > 0)
        {
            if (!gTasks[taskId].data[8])
            {
                initialBlendAmount = gTasks[taskId].data[3];
                targetBlendAmount = gTasks[taskId].data[4];
            }
            else
            {
                initialBlendAmount = gTasks[taskId].data[4];
                targetBlendAmount = gTasks[taskId].data[3];
            }

            if (gTasks[taskId].data[2] == 1)
                targetBlendAmount = 0;
			
            BlendColorCycleExclude(taskId, initialBlendAmount, targetBlendAmount);
        }
        else
            DestroyAnimVisualTask(taskId);
    }
}

// See AnimTask_BlendColorCycle. Same, but selects palette by ANIM_TAG_*.
// arg 0: anim tag
// arg 1: delay
// arg 2: duration
// arg 3: initial blend amount
// arg 4: target blend amount
// arg 5: blend color
void AnimTask_BlendColorCycleByTag(u8 taskId)
{
    gTasks[taskId].data[0] = gBattleAnimArgs[0];
    gTasks[taskId].data[1] = gBattleAnimArgs[1];
    gTasks[taskId].data[2] = gBattleAnimArgs[2];
    gTasks[taskId].data[3] = gBattleAnimArgs[3];
    gTasks[taskId].data[4] = gBattleAnimArgs[4];
    gTasks[taskId].data[5] = gBattleAnimArgs[5];
    gTasks[taskId].data[8] = FALSE;
    BlendColorCycleByTag(taskId, 0, gTasks[taskId].data[4]);
    gTasks[taskId].func = AnimTask_BlendColorCycleByTagLoop;
}

static void BlendColorCycleByTag(u8 taskId, u8 initialBlendAmount, u8 targetBlendAmount)
{
    BeginNormalPaletteFade(1 << (IndexOfSpritePaletteTag(gTasks[taskId].data[0]) + 16), gTasks[taskId].data[1], initialBlendAmount, targetBlendAmount, gTasks[taskId].data[5]);
    --gTasks[taskId].data[2];
    gTasks[taskId].data[8] ^= TRUE;
}

static void AnimTask_BlendColorCycleByTagLoop(u8 taskId)
{
    u8 initialBlendAmount, targetBlendAmount;

    if (!gPaletteFade.active)
    {
        if (gTasks[taskId].data[2] > 0)
        {
            if (!gTasks[taskId].data[8])
            {
                initialBlendAmount = gTasks[taskId].data[3];
                targetBlendAmount = gTasks[taskId].data[4];
            }
            else
            {
                initialBlendAmount = gTasks[taskId].data[4];
                targetBlendAmount = gTasks[taskId].data[3];
            }

            if (gTasks[taskId].data[2] == 1)
                targetBlendAmount = 0;
			
            BlendColorCycleByTag(taskId, initialBlendAmount, targetBlendAmount);
        }
        else
            DestroyAnimVisualTask(taskId);
    }
}

// Flashes the specified anim tag with given color. Used e.g. to flash the particles red in Hyper Beam.
// arg 0: anim tag
// arg 1: (?)
// arg 2: duration
// arg 3: blend color
// arg 4: initial blend amount
// arg 5: second blend color (?)
// arg 6: target blend color
void AnimTask_FlashAnimTagWithColor(u8 taskId)
{
    gTasks[taskId].data[0] = gBattleAnimArgs[1];
    gTasks[taskId].data[1] = gBattleAnimArgs[1];
    gTasks[taskId].data[2] = gBattleAnimArgs[2];
    gTasks[taskId].data[3] = gBattleAnimArgs[3];
    gTasks[taskId].data[4] = gBattleAnimArgs[4];
    gTasks[taskId].data[5] = gBattleAnimArgs[5];
    gTasks[taskId].data[6] = gBattleAnimArgs[6];
    gTasks[taskId].data[7] = gBattleAnimArgs[0];
    BeginNormalPaletteFade(1 << (IndexOfSpritePaletteTag(gBattleAnimArgs[0]) + 16), 0, gBattleAnimArgs[4], gBattleAnimArgs[4], gBattleAnimArgs[3]);
    gTasks[taskId].func = sub_80B9FD8;
}

static void sub_80B9FD8(u8 taskId)
{
    u32 selectedPalettes;

    if (gTasks[taskId].data[0] > 0)
    {
        --gTasks[taskId].data[0];
        return;
    }
	
    if (gPaletteFade.active)
        return;
	
    if (gTasks[taskId].data[2] == 0)
    {
        gTasks[taskId].func = sub_80BA090;
        return;
    }
    selectedPalettes = 1 << (IndexOfSpritePaletteTag(gTasks[taskId].data[7]) + 16);
	
    if (gTasks[taskId].data[1] & 0x100)
        BeginNormalPaletteFade(selectedPalettes, 0, gTasks[taskId].data[4], gTasks[taskId].data[4], gTasks[taskId].data[3]);
    else
        BeginNormalPaletteFade(selectedPalettes, 0, gTasks[taskId].data[6], gTasks[taskId].data[6], gTasks[taskId].data[5]);
	
    gTasks[taskId].data[1] ^= 0x100;
    gTasks[taskId].data[0] = gTasks[taskId].data[1] & 0xFF;
	
    --gTasks[taskId].data[2];
}

static void sub_80BA090(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        BeginNormalPaletteFade(1 << (IndexOfSpritePaletteTag(gTasks[taskId].data[7]) + 16), 0, 0, 0, RGB_BLACK);
        DestroyAnimVisualTask(taskId);
    }
}

// Simple inverts the screen color.
void AnimTask_InvertScreenColor(u8 taskId)
{
    u32 selectedPalettes = 0;
	
    if (gBattleAnimArgs[0] & 0x100)
        selectedPalettes = SelectBattleAnimSpriteAndBgPalettes(TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE);
    if (gBattleAnimArgs[1] & 0x100)
        selectedPalettes |= (0x10000 << gBattleAnimAttacker);
    if (gBattleAnimArgs[2] & 0x100)
        selectedPalettes |= (0x10000 << gBattleAnimTarget);
	
    InvertPlttBuffer(selectedPalettes);
    DestroyAnimVisualTask(taskId);
}

// Fades out all particles by altering the blend alpha.
// arg 0: fade delay
void AnimTask_FadeOutParticles(u8 taskId)
{
	SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL); // Blend sprites out
	SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16, 0));
	
	gTasks[taskId].data[1] = 16;
	gTasks[taskId].data[2] = 0;
	gTasks[taskId].data[3] = gBattleAnimArgs[0];
	gTasks[taskId].data[4] = 0;
	gTasks[taskId].func = AnimTask_FadeOutParticles_Step;
}

static void AnimTask_FadeOutParticles_Step(u8 taskId)
{
	if (gTasks[taskId].data[1])
	{
		if (gTasks[taskId].data[4]++ >= gTasks[taskId].data[3])
		{
			gTasks[taskId].data[4] = 0;
			
			--gTasks[taskId].data[1];
			++gTasks[taskId].data[2];
			
			SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[1], gTasks[taskId].data[2]));
		}
	}
	else
		DestroyAnimVisualTaskAndDisableBlend(taskId);
}

// Shakes the battle terrain or the given mon sprite.
// arg 0: slide amound
// arg 1: shake delay
// arg 2: shake duration
// arg 3: which shake
// 0 - shake bg x pos
// 1 - shake bg y pos
// 2 - shake sprite x pos
// 3 - shake sprite y pos
// arg 4: battlers to not shake (0-1 are the respective battler, 2 - both attacker and target)
static void AnimShakeMonOrBattleTerrain(struct Sprite *sprite)
{
    u16 var0;

    sprite->invisible = TRUE;
	
    sprite->data[0] = -gBattleAnimArgs[0];
    sprite->data[1] = gBattleAnimArgs[1];
    sprite->data[2] = gBattleAnimArgs[1];
    sprite->data[3] = gBattleAnimArgs[2];

    switch (gBattleAnimArgs[3])
    {
    case 0:
        StoreSpriteCallbackInData6(sprite, (void *)&gBattle_BG3_X);
        break;
    case 1:
        StoreSpriteCallbackInData6(sprite, (void *)&gBattle_BG3_Y);
        break;
    case 2:
        StoreSpriteCallbackInData6(sprite, (void *)&gSpriteCoordOffsetX);
        break;
    default:
        StoreSpriteCallbackInData6(sprite, (void *)&gSpriteCoordOffsetY);
        break;
    }
    sprite->data[4] = *(u16 *)(sprite->data[6] | (sprite->data[7] << 16));
    sprite->data[5] = gBattleAnimArgs[3];
	
    var0 = sprite->data[5] - 2;
    if (var0 < 2)
        AnimShakeMonOrBattleTerrain_UpdateCoordOffsetEnabled(gBattleAnimArgs[4]);
	
    sprite->callback = AnimShakeMonOrBattleTerrain_Step;
}

static void AnimShakeMonOrBattleTerrain_Step(struct Sprite *sprite)
{
    u8 i;
    u16 var0;

    if (sprite->data[3] > 0)
    {
        --sprite->data[3];
		
        if (sprite->data[1] > 0)
            --sprite->data[1];
        else
        {
            sprite->data[1] = sprite->data[2];
            *(u16 *)(sprite->data[6] | (sprite->data[7] << 16)) += sprite->data[0];
            sprite->data[0] = -sprite->data[0];
        }
    }
    else
    {
        *(u16 *)(sprite->data[6] | (sprite->data[7] << 16)) = sprite->data[4];
        var0 = sprite->data[5] - 2;
		
        if (var0 < 2)
		{
            for (i = 0; i < gBattlersCount; ++i)
                gSprites[gBattlerSpriteIds[i]].coordOffsetEnabled = FALSE;
		}
        DestroyAnimSprite(sprite);
    }
}

static void AnimShakeMonOrBattleTerrain_UpdateCoordOffsetEnabled(u8 dontShakeBattler)
{
	// Shake both
    gSprites[gBattlerSpriteIds[gBattleAnimAttacker]].coordOffsetEnabled = FALSE;
    gSprites[gBattlerSpriteIds[gBattleAnimTarget]].coordOffsetEnabled = FALSE;
	
    if (dontShakeBattler > ANIM_TARGET) // Shake no one
    {
        gSprites[gBattlerSpriteIds[gBattleAnimAttacker]].coordOffsetEnabled = TRUE;
        gSprites[gBattlerSpriteIds[gBattleAnimTarget]].coordOffsetEnabled = TRUE;
    }
    else // Coose one to don't shake
		gSprites[gBattlerSpriteIds[dontShakeBattler == ANIM_ATTACKER ? gBattleAnimAttacker : gBattleAnimTarget]].coordOffsetEnabled = TRUE;
}

// Can shake battle terrain back and forth on the X or down and back to original pos on Y (cant shake up from orig pos)
// arg 0: x offset of shake
// arg 1: y offset of shake
// arg 2: number of shakes
// arg 3: time between shakes
void AnimTask_ShakeBattleTerrain(u8 taskId)
{
    gTasks[taskId].data[0] = gBattleAnimArgs[0];
    gTasks[taskId].data[1] = gBattleAnimArgs[1];
    gTasks[taskId].data[2] = gBattleAnimArgs[2];
    gTasks[taskId].data[3] = gBattleAnimArgs[3];
    gTasks[taskId].data[8] = gBattleAnimArgs[3];
	
    gBattle_BG3_X = gBattleAnimArgs[0];
    gBattle_BG3_Y = gBattleAnimArgs[1];
	
    gTasks[taskId].func = AnimTask_ShakeBattleTerrain_Step;
    gTasks[taskId].func(taskId);
}

static void AnimTask_ShakeBattleTerrain_Step(u8 taskId)
{
    if (gTasks[taskId].data[3] == 0)
    {
        if (gBattle_BG3_X == gTasks[taskId].data[0])
            gBattle_BG3_X = -gTasks[taskId].data[0];
        else
            gBattle_BG3_X = gTasks[taskId].data[0];

        if (gBattle_BG3_Y == -gTasks[taskId].data[1])
            gBattle_BG3_Y = 0;
        else
            gBattle_BG3_Y = -gTasks[taskId].data[1];

        gTasks[taskId].data[3] = gTasks[taskId].data[8];
		
        if (--gTasks[taskId].data[2] == 0)
        {
            gBattle_BG3_X = 0;
            gBattle_BG3_Y = 0;
            DestroyAnimVisualTask(taskId);
        }
    }
    else
        --gTasks[taskId].data[3];
}

// Shrinks the attacker, pauses, then scales up again.
// No args.
void AnimTask_ShellSmashShrinkAttacker(u8 taskId)
{
	PrepareAffineAnimInTaskData(&gTasks[taskId], GetAnimBattlerSpriteId(ANIM_ATTACKER), sAffineAnim_ShellSmashStretchAttacker);
    gTasks[taskId].func = AnimTask_DestroyTaskAfterAffineAnimFromTaskDataEnds;
}

// Creates a basic hit splat sprite on given battler.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: anim battler
// arg 3: sprite affine anim id
static void AnimHitSplatBasic(struct Sprite *sprite)
{
    StartSpriteAffineAnim(sprite, gBattleAnimArgs[3]);
	InitSpritePosToAnimBattler(sprite, gBattleAnimArgs[2], TRUE);
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
    sprite->callback = RunStoredCallbackWhenAffineAnimEnds;
}

// Creates a basic hit splat sprite on given battler that persist for a given duration when the affine anim ends.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: anim battler
// arg 3: sprite affine anim id
// arg 4: duration
static void AnimHitSplatPersistent(struct Sprite *sprite)
{
    StartSpriteAffineAnim(sprite, gBattleAnimArgs[3]);
	InitSpritePosToAnimBattler(sprite, gBattleAnimArgs[2], TRUE);
    sprite->data[0] = gBattleAnimArgs[4];
    sprite->callback = RunStoredCallbackWhenAffineAnimEnds;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSpriteAfterTimer);
}

// Creates a basic hit splat sprite on given battler. But changes y offset depending on the side.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: anim battler
// arg 3: sprite affine anim id
static void AnimHitSplatHandleInvert(struct Sprite *sprite)
{
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        gBattleAnimArgs[1] = -gBattleAnimArgs[1];
	
    AnimHitSplatBasic(sprite);
}

// Creates a hit splat sprite on given battler, with random positions.
// arg 0: anim battler
// arg 1: afine anim num (if -1 then choose at random)
static void AnimHitSplatRandom(struct Sprite *sprite)
{
    if (gBattleAnimArgs[1] == -1)
        gBattleAnimArgs[1] = RandomMax(4);
	
    StartSpriteAffineAnim(sprite, gBattleAnimArgs[1]);
	InitSpritePosToAnimBattler(sprite, gBattleAnimArgs[0], FALSE);
    
    sprite->x2 += RandomMax(48) - 24;
    sprite->y2 += RandomMax(24) - 12;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
    sprite->callback = RunStoredCallbackWhenAffineAnimEnds;
}

// Creates a basic hit splat sprite on the edge of the given battler.
// arg 0: anim battler
// arg 1: x pixel offset
// arg 2: y pixel offset
// arg 3: sprite affine anim num
static void AnimHitSplatOnMonEdge(struct Sprite *sprite)
{
    sprite->data[0] = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);
	
    sprite->x = gSprites[sprite->data[0]].x + gSprites[sprite->data[0]].x2;
    sprite->y = gSprites[sprite->data[0]].y + gSprites[sprite->data[0]].y2;
	
    sprite->x2 = gBattleAnimArgs[1];
    sprite->y2 = gBattleAnimArgs[2];
	
    StartSpriteAffineAnim(sprite, gBattleAnimArgs[3]);
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
    sprite->callback = RunStoredCallbackWhenAffineAnimEnds;
}

// Animates the cross impact sprite on the given battler. Used in moves like Cross Chop and Leaf Blade.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: anim battler
// arg 3: duration
static void AnimCrossImpact(struct Sprite *sprite)
{
	InitSpritePosToAnimBattler(sprite, gBattleAnimArgs[2], TRUE);
	
    sprite->data[0] = gBattleAnimArgs[3];
    sprite->callback = WaitAnimForDuration;
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Creates a basic hit splat sprite on given battler that switches its visibility each frame.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: anim battler
// arg 3: sprite affine anim id
static void AnimFlashingHitSplat(struct Sprite *sprite)
{
	InitSpritePosToAnimBattler(sprite, gBattleAnimArgs[2], TRUE);
    StartSpriteAffineAnim(sprite, gBattleAnimArgs[3]);
    sprite->callback = AnimFlashingHitSplat_Step;
}

static void AnimFlashingHitSplat_Step(struct Sprite *sprite)
{
    sprite->invisible ^= TRUE;
	
    if (sprite->data[0]++ > 12)
        DestroyAnimSprite(sprite);
}

// Moves the shells towards the attacker and leaves them there until they fade out.
// arg 0: initial x pixel offset
// arg 1: final x pixel offset
// arg 2: duration
// arg 3: anim num (0 = left shell, 1 = right shell)
static void AnimShellSmashShell(struct Sprite *sprite)
{
	StartSpriteAnim(sprite, gBattleAnimArgs[3]);
	StartSpriteAffineAnim(sprite, gBattleAnimArgs[3]);
	
	sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X) + gBattleAnimArgs[0];
	sprite->y = GetBattlerSpriteCoord2(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET) + 2; // Slightly encompass the entire sprite
	
	sprite->data[0] = gBattleAnimArgs[2];
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X) + gBattleAnimArgs[1];
	sprite->data[4] = sprite->y;
	sprite->callback = StartAnimLinearTranslation;
	StoreSpriteCallbackInData6(sprite, AnimShellSmashShell_Step);
}

static void AnimShellSmashShell_Step(struct Sprite *sprite)
{
	if (GetGpuReg(REG_OFFSET_BLDALPHA) >= BLDALPHA_BLEND(0, 8)) // Wait until reach 1/2 fade out
		DestroyAnimSprite(sprite);
}

// Move sprite inward for Clamp.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: affine anim num
// arg 3: x movement value
// arg 4: y movement value
// arg 5: duration
// arg 6: anim num (0 = left shell, 1 = right shell)
static void AnimClampShell(struct Sprite *sprite)
{
	StartSpriteAnim(sprite, gBattleAnimArgs[6]);
	AnimBite(sprite);
}

// Animates the Weather Ball up sprite.
// No args.
void SpriteCB_WeatherBallUp(struct Sprite *sprite)
{
    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
	
    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
        sprite->data[0] = 5;
    else
        sprite->data[0] = -10;
	
    sprite->data[1] = -40;
    sprite->callback = SpriteCB_WeatherBallUp_Step;
}

static void SpriteCB_WeatherBallUp_Step(struct Sprite *sprite)
{
    sprite->data[2] += sprite->data[0];
    sprite->data[3] += sprite->data[1];
	
    sprite->x2 = sprite->data[2] / 10;
    sprite->y2 = sprite->data[3] / 10;
	
    if (sprite->data[1] < -20)
        ++sprite->data[1];
	
    if (sprite->y + sprite->y2 < -32)
        DestroyAnimSprite(sprite);
}

// Animates Weather ball particles down.
// arg 0: speed
// arg 1: final x position
// arg 2: final y position
void AnimWeatherBallDown(struct Sprite *sprite)
{
    s32 x;

    sprite->data[0] = gBattleAnimArgs[0];
    sprite->data[2] = sprite->x + gBattleAnimArgs[1];
    sprite->data[4] = sprite->y + gBattleAnimArgs[2];
	
    if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_PLAYER)
    {
		sprite->y = gBattleAnimArgs[2] - 20;
        x = (u16)gBattleAnimArgs[1] + 30;
    }
    else
    {
		sprite->y = gBattleAnimArgs[2] - 80;
        x = (u16)gBattleAnimArgs[1] - 30;
    }
	sprite->x += x;
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

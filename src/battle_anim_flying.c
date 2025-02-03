#include "global.h"
#include "battle_anim.h"
#include "gflib.h"
#include "math_util.h"
#include "random.h"
#include "trig.h"
#include "constants/battle_anim.h"
#include "constants/songs.h"

static void AnimEllipticalGust(struct Sprite *sprite);
static void AnimEllipticalGust_Step(struct Sprite *sprite);
static void AnimGustToTarget(struct Sprite *sprite);
static void AnimAirWaveCrescent(struct Sprite *sprite);
static void AnimFlyBallUp(struct Sprite *sprite);
static void AnimFlyBallUp_Step(struct Sprite *sprite);
static void AnimFlyBallAttack(struct Sprite *sprite);
static void AnimFlyBallAttack_Step(struct Sprite *sprite);
static void AnimFallingFeather(struct Sprite *sprite);
static void AnimWhirlwindLine(struct Sprite *sprite);
static void AnimWhirlwindLine_Step(struct Sprite *sprite);
static void AnimBounceBallShrink(struct Sprite *sprite);
static void AnimBounceBallLand(struct Sprite *sprite);
static void AnimSkyAttackBird(struct Sprite *sprite);
static void AnimSkyAttackBird_Step(struct Sprite *sprite);
static void AnimTask_AnimateGustTornadoPalette_Step(u8 taskId);
static void AnimFallingFeather_Step(struct Sprite *sprite);
static void AirCutterProjectileStep1(u8 taskId);
static void AirCutterProjectileStep2(u8 taskId);
static void AnimAirWaveProjectile(struct Sprite *sprite);
static void AnimAirWaveProjectile_Step1(struct Sprite *sprite);
static void AnimAirWaveProjectile_Step2(struct Sprite *sprite);

const struct SpriteTemplate gEllipticalGustSpriteTemplate =
{
    .tileTag = ANIM_TAG_GUST,
    .paletteTag = ANIM_TAG_GUST,
    .oam = &gOamData_AffineOff_ObjNormal_32x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimEllipticalGust,
};

static const union AffineAnimCmd sAffineAnim_GustToTarget[] =
{
    AFFINEANIMCMD_FRAME(0x10, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME(0xA, 0x0, 0, 24),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims_GustToTarget[] =
{
    sAffineAnim_GustToTarget,
};

const struct SpriteTemplate gGustToTargetSpriteTemplate =
{
    .tileTag = ANIM_TAG_GUST,
    .paletteTag = ANIM_TAG_GUST,
    .oam = &gOamData_AffineNormal_ObjNormal_32x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_GustToTarget,
    .callback = AnimGustToTarget,
};

static const union AnimCmd sAnim_AirWaveCrescent[] =
{
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_FRAME(0, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(0, 3, .vFlip = TRUE),
    ANIMCMD_FRAME(0, 3, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnims_AirWaveCrescent[] =
{
    sAnim_AirWaveCrescent,
};

const struct SpriteTemplate gAirWaveCrescentSpriteTemplate =
{
    .tileTag = ANIM_TAG_AIR_WAVE_2,
    .paletteTag = ANIM_TAG_AIR_WAVE_2,
    .oam = &gOamData_AffineOff_ObjNormal_32x16,
    .anims = sAnims_AirWaveCrescent,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimAirWaveCrescent,
};

static const union AffineAnimCmd sAffineAnim_FlyBallUp[] =
{
    AFFINEANIMCMD_FRAME(0x10, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME(0x28, 0x0, 0, 6),
    AFFINEANIMCMD_FRAME(0x0, -0x20, 0, 5),
    AFFINEANIMCMD_FRAME(-0x10, 0x20, 0, 10),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims_FlyBallUp[] =
{
    sAffineAnim_FlyBallUp,
};

static const union AffineAnimCmd sAffineAnim_FlyBallAttack_0[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, 50, 1),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_FlyBallAttack_1[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, -40, 1),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims_FlyBallAttack[] =
{
    sAffineAnim_FlyBallAttack_0,
    sAffineAnim_FlyBallAttack_1,
};

const struct SpriteTemplate gFlyBallUpSpriteTemplate =
{
    .tileTag = ANIM_TAG_ROUND_SHADOW,
    .paletteTag = ANIM_TAG_ROUND_SHADOW,
    .oam = &gOamData_AffineDouble_ObjNormal_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_FlyBallUp,
    .callback = AnimFlyBallUp,
};

const struct SpriteTemplate gFlyBallAttackSpriteTemplate =
{
    .tileTag = ANIM_TAG_ROUND_SHADOW,
    .paletteTag = ANIM_TAG_ROUND_SHADOW,
    .oam = &gOamData_AffineNormal_ObjNormal_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_FlyBallAttack,
    .callback = AnimFlyBallAttack,
};

static const union AnimCmd sAnim_FallingFeather_0[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_FallingFeather_1[] =
{
    ANIMCMD_FRAME(16, 0, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_FallingFeather[] =
{
    sAnim_FallingFeather_0,
    sAnim_FallingFeather_1,
};

const struct SpriteTemplate gFallingFeatherSpriteTemplate =
{
    .tileTag = ANIM_TAG_WHITE_FEATHER,
    .paletteTag = ANIM_TAG_WHITE_FEATHER,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = sAnims_FallingFeather,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimFallingFeather,
};

static const union AnimCmd sAnim_WhirlwindLines[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_FRAME(8, 1),
    ANIMCMD_FRAME(16, 1),
    ANIMCMD_FRAME(8, 1, .hFlip = TRUE),
    ANIMCMD_FRAME(0, 1, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_WhirlwindLines[] =
{
    sAnim_WhirlwindLines,
};

const struct SpriteTemplate gWhirlwindLineSpriteTemplate =
{
    .tileTag = ANIM_TAG_WHIRLWIND_LINES,
    .paletteTag = ANIM_TAG_WHIRLWIND_LINES,
    .oam = &gOamData_AffineOff_ObjNormal_32x16,
    .anims = sAnims_WhirlwindLines,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimWhirlwindLine,
};

static const union AffineAnimCmd sAffineAnim_BounceBallShrink[] =
{
    AFFINEANIMCMD_FRAME(0x10, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME(0x28, 0x0, 0, 6),
    AFFINEANIMCMD_FRAME(0x0, -0x20, 0, 5),
    AFFINEANIMCMD_FRAME(-0x14, 0x0, 0, 7),
    AFFINEANIMCMD_FRAME(-0x14, -0x14, 0, 5),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims_BounceBallShrink[] =
{
    sAffineAnim_BounceBallShrink,
};

const struct SpriteTemplate gBounceBallShrinkSpriteTemplate =
{
    .tileTag = ANIM_TAG_ROUND_SHADOW,
    .paletteTag = ANIM_TAG_ROUND_SHADOW,
    .oam = &gOamData_AffineDouble_ObjNormal_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_BounceBallShrink,
    .callback = AnimBounceBallShrink,
};

static const union AffineAnimCmd sAffineAnim_BounceBallLand[] =
{
    AFFINEANIMCMD_FRAME(0xA0, 0x100, 0, 0),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims_BounceBallLand[] =
{
    sAffineAnim_BounceBallLand,
};

const struct SpriteTemplate gBounceBallLandSpriteTemplate =
{
    .tileTag = ANIM_TAG_ROUND_SHADOW,
    .paletteTag = ANIM_TAG_ROUND_SHADOW,
    .oam = &gOamData_AffineDouble_ObjNormal_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_BounceBallLand,
    .callback = AnimBounceBallLand,
};

const struct SpriteTemplate gSkyAttackBirdSpriteTemplate =
{
    .tileTag = ANIM_TAG_BIRD,
    .paletteTag = ANIM_TAG_BIRD,
    .oam = &gOamData_AffineDouble_ObjNormal_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSkyAttackBird,
};

static const union AffineAnimCmd sAffineAnim_ScatterFeather[] =
{
    AFFINEANIMCMD_FRAME(0, 0, -1, 14), // Rotate a little right
	AFFINEANIMCMD_FRAME(0, 0, 1, 28), // Rotate a little left
	AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims_ScatterFeather[] =
{
    sAffineAnim_ScatterFeather,
};

const struct SpriteTemplate gScatterFeatherSpriteTemplate =
{
	.tileTag = ANIM_TAG_WHITE_FEATHER,
    .paletteTag = ANIM_TAG_WHITE_FEATHER,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = sAnims_FallingFeather,
    .images = NULL,
    .affineAnims = sAffineAnims_ScatterFeather,
    .callback = AnimDizzyPunchDuck,
};

static const union AffineAnimCmd sAffineAnim_WhirlwindStretchAndRotateTargetToRight[] =
{
	AFFINEANIMCMD_FRAME(0, 8, 0, 8),
	AFFINEANIMCMD_FRAME(0, -10, 0, 6),
	AFFINEANIMCMD_FRAME(0, 0, 0, 5),
	AFFINEANIMCMD_FRAME(0, 0, -2, 6),
	AFFINEANIMCMD_FRAME(0, 0, -1, 20),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_WhirlwindStretchAndRotateTargetToLeft[] =
{
    AFFINEANIMCMD_FRAME(0, 8, 0, 8),
	AFFINEANIMCMD_FRAME(0, -10, 0, 6),
	AFFINEANIMCMD_FRAME(0, 0, 0, 5),
	AFFINEANIMCMD_FRAME(0, 0, 2, 6),
	AFFINEANIMCMD_FRAME(0, 0, 1, 20),
    AFFINEANIMCMD_END,
};

const struct SpriteTemplate gSkyAttackCloakEnergySpriteTemplate =
{
    .tileTag = ANIM_TAG_FOCUS_ENERGY,
    .paletteTag = ANIM_TAG_WATER_ORB,
    .oam = &gOamData_AffineOff_ObjNormal_16x32,
    .anims = gEndureEnergyAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimEndureEnergy,
};

const struct SpriteTemplate gAirWaveProjectileSpriteTemplate =
{
    .tileTag = ANIM_TAG_AIR_WAVE,
    .paletteTag = ANIM_TAG_AIR_WAVE,
    .oam = &gOamData_AffineOff_ObjBlend_32x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimAirWaveProjectile,
};

// Animates the elliptical gust sprite on the target. Used by MOVE_GUST.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
static void AnimEllipticalGust(struct Sprite *sprite)
{
    InitSpritePosToAnimTarget(sprite, FALSE);
    sprite->y += 20;
    sprite->data[1] = 191;
    sprite->callback = AnimEllipticalGust_Step;
    sprite->callback(sprite);
}

static void AnimEllipticalGust_Step(struct Sprite *sprite)
{
    sprite->x2 = Sin(sprite->data[1], 32);
    sprite->y2 = Cos(sprite->data[1], 8);
    sprite->data[1] += 5;
    sprite->data[1] &= 0xFF;
    if (++sprite->data[0] == 71)
        DestroyAnimSprite(sprite);
}

// Animates the palette on the gust tornado to make it look like its spinning.
// arg 0: update delay
// arg 1: duration
void AnimTask_AnimateGustTornadoPalette(u8 taskId)
{
    gTasks[taskId].data[0] = gBattleAnimArgs[1];
    gTasks[taskId].data[1] = gBattleAnimArgs[0];
    gTasks[taskId].data[2] = IndexOfSpritePaletteTag(ANIM_TAG_GUST);
    gTasks[taskId].func = AnimTask_AnimateGustTornadoPalette_Step;
}

static void AnimTask_AnimateGustTornadoPalette_Step(u8 taskId)
{
    u8 data2;
    u16 temp;
    s32 i, base;

    if (gTasks[taskId].data[10]++ == gTasks[taskId].data[1])
    {
        gTasks[taskId].data[10] = 0;
        data2 = gTasks[taskId].data[2];
        temp = gPlttBufferFaded[16 * data2 + 0x108];
        i = 7;
        base = data2 * 16;
        
		do
        {
            gPlttBufferFaded[base + 0x101 + i] = gPlttBufferFaded[base + 0x100 + i];
        } while (--i > 0);

        gPlttBufferFaded[base + 0x101] = temp;
    }
	
    if (--gTasks[taskId].data[0] == 0)
        DestroyAnimVisualTask(taskId);
}

static void AnimGustToTarget(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, TRUE);
	
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];
	
    sprite->data[0] = gBattleAnimArgs[4];
    sprite->data[1] = sprite->x;
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X) + gBattleAnimArgs[2];
    sprite->data[3] = sprite->y;
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3];
    InitAnimLinearTranslation(sprite);
    sprite->callback = RunStoredCallbackWhenAffineAnimEnds;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSpriteWhenLinearTranslationEnds);
}

// Animates the air wave from the attacker into the target in the MOVE_RAZOR_WIND's anim.
// arg 0: initial x offset
// arg 1: initial y offset
// arg 2: final x offset
// arg 3: final y offset
// arg 4: duration
// arg 5: seek anim id
// arg 6: average positions
static void AnimAirWaveCrescent(struct Sprite *sprite)
{
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
    {
        gBattleAnimArgs[0] = -gBattleAnimArgs[0];
        gBattleAnimArgs[1] = -gBattleAnimArgs[1];
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];
        gBattleAnimArgs[3] = -gBattleAnimArgs[3];
    }
    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X) + gBattleAnimArgs[0];
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[1];
    sprite->data[0] = gBattleAnimArgs[4];
	
    if (!gBattleAnimArgs[6])
    {
        sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
        sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
    }
    else
        SetAverageBattlerPositions(gBattleAnimTarget, TRUE, &sprite->data[2], &sprite->data[4]);

    sprite->data[2] += gBattleAnimArgs[2];
    sprite->data[4] += gBattleAnimArgs[3];
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
    SeekSpriteAnim(sprite, gBattleAnimArgs[5]);
}

// Animates the Fly shadow up.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: initial delay
// arg 3: movement speed
static void AnimFlyBallUp(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, TRUE);
    sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[1] = gBattleAnimArgs[3];
	ChangeAnimBattlerSpriteVisibility(ANIM_ATTACKER, TRUE);
    sprite->callback = AnimFlyBallUp_Step;
}

static void AnimFlyBallUp_Step(struct Sprite *sprite)
{
    if (sprite->data[0] > 0)
        --sprite->data[0];
    else
    {
        sprite->data[2] += sprite->data[1];
        sprite->y2 -= (sprite->data[2] >> 8);
    }
	
    if (sprite->y + sprite->y2 < -32)
        DestroyAnimSprite(sprite);
}

// Animates the Fly shadow moving into the target.
// arg 0: duration
static void AnimFlyBallAttack(struct Sprite *sprite)
{
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
    {
        sprite->x = 272;
        StartSpriteAffineAnim(sprite, 1);
    }
    else
        sprite->x = -32;
	
	sprite->y = -32;
	
    sprite->data[0] = gBattleAnimArgs[0];
    sprite->data[1] = sprite->x;
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    sprite->data[3] = sprite->y;
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
    InitAnimLinearTranslation(sprite);
    sprite->callback = AnimFlyBallAttack_Step;
}

static void AnimFlyBallAttack_Step(struct Sprite *sprite)
{
    sprite->data[0] = 1;
    AnimTranslateLinear(sprite);
	
    if (((u16)sprite->data[3] >> 8) > 200)
    {
        sprite->x += sprite->x2;
        sprite->x2 = 0;
        sprite->data[3] &= 0xFF;
    }
	
    if (sprite->x + sprite->x2 < -32 || sprite->x + sprite->x2 > 272 || sprite->y + sprite->y2 > 160)
	{
		ChangeAnimBattlerSpriteVisibility(ANIM_ATTACKER, FALSE);
        DestroyAnimSprite(sprite);
	}
}

void DestroyAnimSpriteAfterTimer(struct Sprite *sprite)
{
    if (sprite->data[0]-- <= 0)
		DestroyAnimSprite(sprite);
}

struct FeatherDanceData
{
    u16 unk0_0a:1;
    u16 unk0_0b:1;
    u16 unk0_0c:1;
    u16 unk0_0d:1;
    u16 unk0_1:4;
    u16 unk1:8;
    u16 unk2;
    s16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u8 unkC[2];
    u16 unkE_0:1;
    u16 unkE_1:15;
};

// Animates an falling feather sprite on the given battler.
// arg 0: x position
// arg 1: initial y position
// arg 2: (?)
// arg 3: (?)
// arg 4: falling speed
// arg 5: (?)
// arg 6: final y position
// arg 7: anim battler
static void AnimFallingFeather(struct Sprite *sprite)
{
    u8 matrixNum, sinIndex, battler = GetBattlerForAnimScript(gBattleAnimArgs[7]);
    s16 spriteCoord, sinVal;
    struct FeatherDanceData *data = (struct FeatherDanceData *)sprite->data;
	
	if (GetBattlerSide(battler) == B_SIDE_PLAYER)
        gBattleAnimArgs[0] = -gBattleAnimArgs[0];
	
    sprite->x = GetBattlerSpriteCoord(battler, BATTLER_COORD_ATTR_HEIGHT) + gBattleAnimArgs[0];
    spriteCoord = GetBattlerSpriteCoord(battler, BATTLER_COORD_ATTR_WIDTH);
    sprite->y = spriteCoord + gBattleAnimArgs[1];
	
    data->unk8 = sprite->y << 8;
    data->unkE_1 = spriteCoord + gBattleAnimArgs[6];
    data->unk0_0c = 1;
    data->unk2 = gBattleAnimArgs[2] & 0xFF;
    data->unkA = (gBattleAnimArgs[2] >> 8) & 0xFF;
    data->unk4 = gBattleAnimArgs[3];
    data->unk6 = gBattleAnimArgs[4];
    *(u16 *)(data->unkC) = gBattleAnimArgs[5];
	
    if (data->unk2 >= 64 && data->unk2 <= 191)
    {
        sprite->oam.priority = GetBattlerSpriteBGPriority(battler) + 1;
        data->unkE_0 = 0;
		
        if (!(data->unk4 & 0x8000))
        {
            sprite->hFlip ^= TRUE;
            sprite->animNum = sprite->hFlip;
            sprite->animBeginning = TRUE;
            sprite->animEnded = FALSE;
        }
    }
    else
    {
        sprite->oam.priority = GetBattlerSpriteBGPriority(battler);
        data->unkE_0 = 1;
		
        if (data->unk4 & 0x8000)
        {
            sprite->hFlip ^= TRUE;
            sprite->animNum = sprite->hFlip;
            sprite->animBeginning = TRUE;
            sprite->animEnded = FALSE;
        }
    }
    data->unk0_1 = data->unk2 >> 6;
	
    sprite->x2 = (gSineTable[data->unk2] * data->unkC[0]) >> 8;
	
    matrixNum = sprite->oam.matrixNum;
	
    sinIndex = (-sprite->x2 >> 1) + data->unkA;
	
    sinVal = gSineTable[sinIndex];
	
    gOamMatrices[matrixNum].a = gOamMatrices[matrixNum].d = gSineTable[sinIndex + 64];
	gOamMatrices[matrixNum].b = sinVal;
	gOamMatrices[matrixNum].c = -sinVal;
	
    sprite->callback = AnimFallingFeather_Step;
}

static void AnimFallingFeather_Step(struct Sprite *sprite)
{
    u8 matrixNum, sinIndex;
    s16 sinVal;
    struct FeatherDanceData *data = (struct FeatherDanceData *)sprite->data;
	
    if (data->unk0_0a)
    {
        if (data->unk1-- % 256 == 0)
        {
            data->unk0_0a = 0;
            data->unk1 = 0;
        }
    }
    else
    {
        switch (data->unk2 / 64)
        {
        case 0: 
            if ((u8)data->unk0_1 == 1) //casts to u8 here are necessary for matching
            {
                data->unk0_0d = 1;
                data->unk0_0a = 1;
                data->unk1 = 0;
            }
            else if ((u8)data->unk0_1 == 3)
            {
                data->unk0_0b ^= 1;
                data->unk0_0a = 1;
                data->unk1 = 0;
            }
            else if (data->unk0_0d)
            {
                sprite->hFlip ^= TRUE;
                sprite->animNum = sprite->hFlip;
                sprite->animBeginning = TRUE;
                sprite->animEnded = FALSE;
				
                if (data->unk0_0c)
                {
                    if (!data->unkE_0)
                        --sprite->oam.priority;
                    else
                        ++sprite->oam.priority;
					
					data->unkE_0 ^= 1;
                }
                data->unk0_0d = 0;
                data->unk2;
            }
            data->unk0_1 = 0;
            break;
        case 1:
            if ((u8)data->unk0_1 == 0)
            {
                data->unk0_0d = 1;
                data->unk0_0a = 1;
                data->unk1 = 0;
            }
            else if ((u8)data->unk0_1 == 2)
            {
                data->unk0_0a = 1;
                data->unk1 = 0;
            }
            else if (data->unk0_0d)
            {
                sprite->hFlip ^= TRUE;
                sprite->animNum = sprite->hFlip;
                sprite->animBeginning = TRUE;
                sprite->animEnded = FALSE;
				
                if (data->unk0_0c)
                {
                    if (!data->unkE_0)
                        --sprite->oam.priority;
                    else
                        ++sprite->oam.priority;

					data->unkE_0 ^= 1;
                }
                data->unk0_0d = 0;
            }
            data->unk0_1 = 1;
            break;
        case 2:
            if ((u8)data->unk0_1 == 3)
            {
                data->unk0_0d = 1;
                data->unk0_0a = 1;
                data->unk1 = 0;
            }
            else if ((u8)data->unk0_1 == 1)
            {
                data->unk0_0a = 1;
                data->unk1 = 0;
            }
            else if (data->unk0_0d)
            {
                sprite->hFlip ^= TRUE;
                sprite->animNum = sprite->hFlip;
                sprite->animBeginning = TRUE;
                sprite->animEnded = FALSE;
				
                if (data->unk0_0c)
                {
                    if (!data->unkE_0)
                        --sprite->oam.priority;
                    else
                        ++sprite->oam.priority;
					
					data->unkE_0 ^= 1;
                }
                data->unk0_0d = 0;
            }
            data->unk0_1 = 2;
            break;
        case 3:
            if ((u8)data->unk0_1 == 2)
            {
                data->unk0_0d = 1;
            }
            else if ((u8)data->unk0_1 == 0)
            {
                data->unk0_0b ^= 1;
                data->unk0_0a = 1;
                data->unk1 = 0;
            }
            else if (data->unk0_0d)
            {
                sprite->hFlip ^= TRUE;
                sprite->animNum = sprite->hFlip;
                sprite->animBeginning = TRUE;
                sprite->animEnded = FALSE;
				
                if (data->unk0_0c)
                {
                    if (!data->unkE_0)
                        --sprite->oam.priority;
                    else
                        ++sprite->oam.priority;

					data->unkE_0 ^= 1;
                }
                data->unk0_0d = 0;
            }
            data->unk0_1 = 3;
            break;
        }

        sprite->x2 = ((s32)data->unkC[data->unk0_0b] * gSineTable[data->unk2]) >> 8;
        matrixNum = sprite->oam.matrixNum;

        sinIndex = (-sprite->x2 >> 1) + data->unkA;
        sinVal = gSineTable[sinIndex];

        gOamMatrices[matrixNum].a = gOamMatrices[matrixNum].d = gSineTable[sinIndex + 64];
        gOamMatrices[matrixNum].b = sinVal;
        gOamMatrices[matrixNum].c = -sinVal;

        data->unk8 += data->unk6;
        sprite->y = data->unk8 >> 8;
		
        if (data->unk4 & 0x8000)
            data->unk2 = (data->unk2 - (data->unk4 & 0x7FFF)) & 0xFF;
        else
            data->unk2 = (data->unk2 + (data->unk4 & 0x7FFF)) & 0xFF;

        if (sprite->y + sprite->y2 >= data->unkE_1)
        {
            sprite->data[0] = 0;
            sprite->callback = DestroyAnimSpriteAfterTimer;
        }
    }
}

// Animates the Whirlwind's lines into the target.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: anim battler
// arg 3: duration
// arg 4: anim id
static void AnimWhirlwindLine(struct Sprite *sprite)
{
    u16 arg;
	
	InitSpritePosToAnimBattler(sprite, gBattleAnimArgs[2], FALSE);
	
	if (GetBattlerSide(GetBattlerForAnimScript(gBattleAnimArgs[2])) == B_SIDE_PLAYER)
		sprite->x += 8;
	
	sprite->x -= 32;
	
    SeekSpriteAnim(sprite, gBattleAnimArgs[4]);
    sprite->data[1] = 3276;
    arg = gBattleAnimArgs[4];
    sprite->x2 += 12 * arg;
    sprite->data[0] = arg;
    sprite->data[7] = gBattleAnimArgs[3];
    sprite->callback = AnimWhirlwindLine_Step;
}

static void AnimWhirlwindLine_Step(struct Sprite *sprite)
{
    sprite->x2 += sprite->data[1] >> 8;
	
    if (++sprite->data[0] == 6)
    {
        sprite->data[0] = 0;
        sprite->x2 = 0;
        StartSpriteAnim(sprite, 0);
    }

    if (--sprite->data[7] == -1)
        DestroyAnimSprite(sprite);
}

// Animates the mon sprite rotation in Whirlwind switch out.
// No args.
void AnimTask_WhirlwindStretchAndRotateTarget(u8 taskId)
{
    PrepareAffineAnimInTaskData(&gTasks[taskId], GetAnimBattlerSpriteId(ANIM_TARGET), GetBattlerSide(gBattleAnimTarget) == B_SIDE_PLAYER ? sAffineAnim_WhirlwindStretchAndRotateTargetToLeft : sAffineAnim_WhirlwindStretchAndRotateTargetToRight);
    gTasks[taskId].func = AnimTask_DestroyTaskAfterAffineAnimFromTaskDataEnds;
}

// Animates the Drill Peck circular impacts.
// No args.
void AnimTask_DrillPeckHitSplats(u8 taskId)
{
    if (!(gTasks[taskId].data[0] % 32))
    {
        ++gAnimVisualTaskCount;
        gBattleAnimArgs[0] = Sin(gTasks[taskId].data[0], -13);
        gBattleAnimArgs[1] = Cos(gTasks[taskId].data[0], -13);
        gBattleAnimArgs[2] = ANIM_TARGET;
        gBattleAnimArgs[3] = 3;
        CreateSpriteAndAnimate(&gFlashingHitSplatSpriteTemplate, GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X), GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET), 3);
    }
    gTasks[taskId].data[0] += 8;
	
    if (gTasks[taskId].data[0] > 255)
        DestroyAnimVisualTask(taskId);
}

// Animates MOVE_BOUNCE's ball on the attacker.
// arg 0: x pixel offset
// arg 1: y pixel offset
static void AnimBounceBallShrink(struct Sprite *sprite)
{
	InitSpritePosToAnimAttacker(sprite, TRUE);
	gSprites[GetAnimBattlerSpriteId(ANIM_ATTACKER)].invisible = TRUE;
	sprite->callback = DestroyAnimSpriteWhenAffineAnimEnds;
}

// Animates MOVE_BOUNCE's ball landing on the target.
// No args.
static void AnimBounceBallLand(struct Sprite *sprite)
{
    switch (sprite->data[0])
    {
    case 0:
        sprite->y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y);
        sprite->y2 = -sprite->y - 32;
        ++sprite->data[0];
        break;
    case 1:
        sprite->y2 += 10;
		
        if (sprite->y2 >= 0)
            ++sprite->data[0];
        break;
    case 2:
        sprite->y2 -= 10;
		
        if (sprite->y + sprite->y2 < -32)
        {
            gSprites[GetAnimBattlerSpriteId(ANIM_ATTACKER)].invisible = FALSE;
            DestroyAnimSprite(sprite);
        }
        break;
    }
}

// Animates the Sky Attacker bird moving from the attacker's pos to the target's pos and then off screen.
static void AnimSkyAttackBird(struct Sprite *sprite)
{
    u16 rotation;
    s16 posx = sprite->x;
    s16 posy = sprite->y;

    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
	
    sprite->data[4] = sprite->x << 4;
    sprite->data[5] = sprite->y << 4;
    sprite->data[6] = ((posx - sprite->x) << 4) / 12;
    sprite->data[7] = ((posy - sprite->y) << 4) / 12;
	
    rotation = ArcTan2Neg(posx - sprite->x, posy - sprite->y);
    rotation += 0xC000;
    TrySetSpriteRotScale(sprite, TRUE, 0x100, 0x100, rotation);
	
    sprite->callback = AnimSkyAttackBird_Step;
}

static void AnimSkyAttackBird_Step(struct Sprite *sprite)
{
    sprite->data[4] += sprite->data[6];
    sprite->data[5] += sprite->data[7];
	
    sprite->x = sprite->data[4] >> 4;
    sprite->y = sprite->data[5] >> 4;
	
    if (sprite->x > 285 || sprite->x < -45 || sprite->y > 157 || sprite->y < -45)
        DestroyAnimSprite(sprite);
}

// Animates MOVE_AIR_CUTTER's air projectile sprites.
// arg 0: target x pixel offset
// arg 1: target y pixel offset
// arg 2: (?)
// arg 3: air wave creation delay
// arg 4: air wave subpriority
void AnimTask_AirCutterProjectile(u8 taskId)
{
    s16 attackerX, attackerY;
    s16 targetX, targetY;
    s16 xDiff, yDiff;

    if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_PLAYER)
    {
        gTasks[taskId].data[4] = 1;
		
        gBattleAnimArgs[0] = -gBattleAnimArgs[0];
        gBattleAnimArgs[1] = -gBattleAnimArgs[1];
		
        if (gBattleAnimArgs[2] & 1)
            gBattleAnimArgs[2] &= ~(1);
        else
            gBattleAnimArgs[2] |= 1;
    }
    attackerX = gTasks[taskId].data[9] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    attackerY = gTasks[taskId].data[10] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y);
	
    if (IsDoubleBattleForBattler(gBattleAnimTarget) && IsBattlerSpriteVisible(BATTLE_PARTNER(gBattleAnimTarget)))
        SetAverageBattlerPositions(gBattleAnimTarget, FALSE, &targetX, &targetY);
    else
    {
        targetX = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
        targetY = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y);
    }
    targetX = gTasks[taskId].data[11] = targetX + gBattleAnimArgs[0];
    targetY = gTasks[taskId].data[12] = targetY + gBattleAnimArgs[1];
	
    if (targetX >= attackerX)
        xDiff = targetX - attackerX;
    else
        xDiff = attackerX - targetX;

    gTasks[taskId].data[5] = Q_8_8_mul(xDiff, Q_8_8_inv(gBattleAnimArgs[2] & ~1));
    gTasks[taskId].data[6] = Q_8_8_mul(gTasks[taskId].data[5], 0x80);
	gTasks[taskId].data[7] = gBattleAnimArgs[2];
	
    if (targetY >= attackerY)
    {
        yDiff = targetY - attackerY;
        gTasks[taskId].data[8] = Q_8_8_mul(yDiff, Q_8_8_inv(gTasks[taskId].data[5])) & ~1;
    }
    else
    {
        yDiff = attackerY - targetY;
        gTasks[taskId].data[8] = Q_8_8_mul(yDiff, Q_8_8_inv(gTasks[taskId].data[5])) | 1;
    }
    gTasks[taskId].data[3] = gBattleAnimArgs[3];
	
    if (gBattleAnimArgs[4] & 0x80)
		gBattleAnimArgs[4] ^= 0x80;
	
    if (gBattleAnimArgs[4] >= 64)
    {
        u16 var = GetBattlerSpriteSubpriority(gBattleAnimTarget) + (gBattleAnimArgs[4] - 64);
        gTasks[taskId].data[2] = var;
    }
    else
    {
        u16 var = GetBattlerSpriteSubpriority(gBattleAnimTarget) - gBattleAnimArgs[4];
        gTasks[taskId].data[2] = var;
    }

    if (gTasks[taskId].data[2] < 3)
        gTasks[taskId].data[2] = 3;

    gTasks[taskId].func = AirCutterProjectileStep1;
}

static void AirCutterProjectileStep1(u8 taskId)
{
    if (gTasks[taskId].data[0]-- <= 0)
    {
        u8 spriteId = CreateSprite(&gAirWaveProjectileSpriteTemplate, gTasks[taskId].data[9], gTasks[taskId].data[10], gTasks[taskId].data[2] - gTasks[taskId].data[1]);
        struct Sprite *sprite = &gSprites[spriteId];

        switch (gTasks[taskId].data[4])
        {
        case 1:
            sprite->oam.matrixNum |= (ST_OAM_HFLIP | ST_OAM_VFLIP);
            break;
        case 2:
            sprite->oam.matrixNum = ST_OAM_HFLIP;
            break;
        }
        sprite->data[0] = gTasks[taskId].data[5] - gTasks[taskId].data[6];
        sprite->data[7] = taskId;
		
        gTasks[taskId].data[gTasks[taskId].data[1] + 13] = spriteId;
        gTasks[taskId].data[0] = gTasks[taskId].data[3];

        PlaySE12WithPanning(SE_M_BLIZZARD2, BattleAnimAdjustPanning(-SOUND_PAN_TARGET));
		
        if (++gTasks[taskId].data[1] > 2)
            gTasks[taskId].func = AirCutterProjectileStep2;
    }
}

static void AirCutterProjectileStep2(u8 taskId)
{
    if (gTasks[taskId].data[1] == 0)
        DestroyAnimVisualTask(taskId);
}

static void AnimAirWaveProjectile(struct Sprite *sprite)
{
    s16 a, b, c;
    struct Task* task = &gTasks[sprite->data[7]];
    
    sprite->data[1] += (-2 & task->data[7]);
    sprite->data[2] += (-2 & task->data[8]);
	
    if (1 & task->data[7])
        sprite->x2 = ((u16)sprite->data[1] >> 8) * -1;
    else
        sprite->x2 = (u16)sprite->data[1] >> 8;

    if (1 & task->data[8])
        sprite->y2 = ((u16)sprite->data[2] / 256u) * -1;
    else
        sprite->y2 = (u16)sprite->data[2] / 256u;

    if (sprite->data[0]-- <= 0)
    {
        sprite->data[0] = 8;
        task->data[5] = 4;
		
        SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
		
		a = Q_8_8_inv(0x1000);
		
        if (task->data[11] >= sprite->x)
            b = (task->data[11] - sprite->x) << 8;
        else
            b = (sprite->x - task->data[11]) << 8;

        if (task->data[12] >= sprite->y)
            c = (task->data[12] - sprite->y) << 8;
        else
            c = (sprite->y - task->data[12]) << 8;

        sprite->data[2] = 0;
        sprite->data[1] = 0;
        sprite->data[6] = 0;
        sprite->data[5] = 0;
        sprite->data[3] = Q_8_8_mul(Q_8_8_mul(b, a), Q_8_8_inv(0x1C0));
        sprite->data[4] = Q_8_8_mul(Q_8_8_mul(c, a), Q_8_8_inv(0x1C0));
        sprite->callback = AnimAirWaveProjectile_Step1;
    }
}

static void AnimAirWaveProjectile_Step1(struct Sprite *sprite)
{
    struct Task* task = &gTasks[sprite->data[7]];
    
    if (sprite->data[0] > task->data[5])
    {
        sprite->data[5] += sprite->data[3];
        sprite->data[6] += sprite->data[4];
    }
    else
    {
        sprite->data[5] -= sprite->data[3];
        sprite->data[6] -= sprite->data[4];
    }
    sprite->data[1] += sprite->data[5];
    sprite->data[2] += sprite->data[6];
	
    if (1 & task->data[7])
        sprite->x2 = ((u16)sprite->data[1] >> 8) * -1;
    else
        sprite->x2 = (u16)sprite->data[1] >> 8;

    if (1 & task->data[8])
        sprite->y2 = ((u16)sprite->data[2] / 256u) * -1;
    else
        sprite->y2 = (u16)sprite->data[2] / 256u;

    if (sprite->data[0]-- <= 0)
    {
        sprite->data[0] = 30;
        sprite->callback = AnimAirWaveProjectile_Step2;
    }
}

static void AnimAirWaveProjectile_Step2(struct Sprite *sprite)
{
    if (sprite->data[0]-- <= 0)
    {
        gTasks[sprite->data[7]].data[1]--;
        DestroySprite(sprite);
    }
}

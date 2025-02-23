#include "global.h"
#include "gflib.h"
#include "battle_anim.h"
#include "scanline_effect.h"
#include "trig.h"

static void AnimSludgeProjectile(struct Sprite *sprite);
static void AnimAcidPoisonBubble(struct Sprite *sprite);
static void AnimSludgeBombHitParticle(struct Sprite *sprite);
static void AnimSludgeBombHitParticle_Step(struct Sprite *sprite);
static void AnimAcidPoisonDroplet(struct Sprite *sprite);
static void AnimTask_AcidArmorStep(u8);

static const union AnimCmd sAnim_ToxicBubble[] =
{
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_FRAME(8, 5),
    ANIMCMD_FRAME(16, 5),
    ANIMCMD_FRAME(24, 5),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_ToxicBubble[] =
{
    sAnim_ToxicBubble,
};

const struct SpriteTemplate gToxicBubbleSpriteTemplate =
{
    .tileTag = ANIM_TAG_TOXIC_BUBBLE,
    .paletteTag = ANIM_TAG_TOXIC_BUBBLE,
    .oam = &gOamData_AffineOff_ObjNormal_16x32,
    .anims = sAnims_ToxicBubble,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSpriteOnMonPos,
};

static const union AnimCmd sAnim_PoisonProjectile[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_AcidPoisonDroplet[] =
{
    ANIMCMD_FRAME(4, 1),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_SludgeBombHit[] =
{
    ANIMCMD_FRAME(8, 1),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_PoisonProjectile[] =
{
    sAnim_PoisonProjectile,
};

static const union AnimCmd *const sAnims_AcidPoisonDroplet[] =
{
    sAnim_AcidPoisonDroplet,
};

static const union AnimCmd *const sAnims_SludgeBombHit[] =
{
    sAnim_SludgeBombHit,
};

static const union AffineAnimCmd sAffineAnim_PoisonProjectile[] =
{
    AFFINEANIMCMD_FRAME(0x160, 0x160, 0, 0),
    AFFINEANIMCMD_FRAME(-0xA, -0xA, 0, 10),
    AFFINEANIMCMD_FRAME(0xA, 0xA, 0, 10),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sAffineAnim_SludgeBombHit[] =
{
    AFFINEANIMCMD_FRAME(0xEC, 0xEC, 0, 0),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims_PoisonProjectile[] =
{
    sAffineAnim_PoisonProjectile,
};

static const union AffineAnimCmd *const sAffineAnims_SludgeBombHit[] =
{
    sAffineAnim_SludgeBombHit,
};

const struct SpriteTemplate gSludgeProjectileSpriteTemplate =
{
    .tileTag = ANIM_TAG_POISON_BUBBLE,
    .paletteTag = ANIM_TAG_POISON_BUBBLE,
    .oam = &gOamData_AffineDouble_ObjNormal_16x16,
    .anims = sAnims_PoisonProjectile,
    .images = NULL,
    .affineAnims = sAffineAnims_PoisonProjectile,
    .callback = AnimSludgeProjectile,
};

const struct SpriteTemplate gAcidPoisonBubbleSpriteTemplate =
{
    .tileTag = ANIM_TAG_POISON_BUBBLE,
    .paletteTag = ANIM_TAG_POISON_BUBBLE,
    .oam = &gOamData_AffineDouble_ObjNormal_16x16,
    .anims = sAnims_PoisonProjectile,
    .images = NULL,
    .affineAnims = sAffineAnims_PoisonProjectile,
    .callback = AnimAcidPoisonBubble,
};

const struct SpriteTemplate gSludgeBombHitParticleSpriteTemplate =
{
    .tileTag = ANIM_TAG_POISON_BUBBLE,
    .paletteTag = ANIM_TAG_POISON_BUBBLE,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = sAnims_SludgeBombHit,
    .images = NULL,
    .affineAnims = sAffineAnims_SludgeBombHit,
    .callback = AnimSludgeBombHitParticle,
};

static const union AffineAnimCmd sAffineAnim_AcidPoisonDroplet[] =
{
    AFFINEANIMCMD_FRAME(-0x10, 0x10, 0, 6),
    AFFINEANIMCMD_FRAME(0x10, -0x10, 0, 6),
    AFFINEANIMCMD_JUMP(0),
};

const union AffineAnimCmd *const gAffineAnims_Droplet[] =
{
    sAffineAnim_AcidPoisonDroplet,
};

const struct SpriteTemplate gAcidPoisonDropletSpriteTemplate =
{
    .tileTag = ANIM_TAG_POISON_BUBBLE,
    .paletteTag = ANIM_TAG_POISON_BUBBLE,
    .oam = &gOamData_AffineDouble_ObjNormal_16x16,
    .anims = sAnims_AcidPoisonDroplet,
    .images = NULL,
    .affineAnims = gAffineAnims_Droplet,
    .callback = AnimAcidPoisonDroplet,
};

const struct SpriteTemplate gPoisonBubbleSpriteTemplate =
{
    .tileTag = ANIM_TAG_POISON_BUBBLE,
    .paletteTag = ANIM_TAG_POISON_BUBBLE,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = sAnims_PoisonProjectile,
    .images = NULL,
    .affineAnims = gAffineAnims_Bubble,
    .callback = AnimBubbleEffect,
};

const struct SpriteTemplate gPoisonGasCloudSpriteTemplate =
{
    .tileTag = ANIM_TAG_PURPLE_GAS_CLOUD,
    .paletteTag = ANIM_TAG_PURPLE_GAS_CLOUD,
    .oam = &gOamData_AffineOff_ObjBlend_32x16,
    .anims = gAnims_Cloud,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = InitSwirlingFogAnim,
};

const struct SpriteTemplate gGastroAcidGreenPoisonBubbleSpriteTemplate =
{
    .tileTag = ANIM_TAG_POISON_BUBBLE,
    .paletteTag = ANIM_TAG_RAZOR_LEAF,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = sAnims_PoisonProjectile,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimFallingRock,
};

const struct SpriteTemplate gToxicSpikesSpriteTemplate =    
{
    .tileTag = ANIM_TAG_SPIKES,
    .paletteTag = ANIM_TAG_SHADOW_BALL,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSpikes,
};

const struct SpriteTemplate gPoisonJabSpriteTemplate =    
{
    .tileTag = ANIM_TAG_POISON_JAB,
    .paletteTag = ANIM_TAG_POISON_JAB,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimNeedleArmSpike,
};

static const union AffineAnimCmd sAffineAnim_MudBomb[] =
{
    AFFINEANIMCMD_FRAME(0, -8, -4, 4), // Compress Vertically
	AFFINEANIMCMD_FRAME(-8, 8, -4, 4), // Compress Horizontally, Normalize Vertically
	AFFINEANIMCMD_FRAME(8, -8, -4, 4), // Normalize Horizontally, Compress Vertically
	AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd *const sAffineAnims_MudBomb[] =
{
    sAffineAnim_MudBomb,
};

const struct SpriteTemplate gMudBombBallSpriteTemplate =
{
    .tileTag = ANIM_TAG_MUD_BOMB,
    .paletteTag = ANIM_TAG_MUD_BOMB,
    .oam = &gOamData_AffineNormal_ObjNormal_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_MudBomb,
    .callback = AnimAcidPoisonBubble,
};

const struct SpriteTemplate gMudBombDropletSpriteTemplate =
{
    .tileTag = ANIM_TAG_POISON_BUBBLE,
    .paletteTag = ANIM_TAG_BROWN_ORB,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = sAnims_AcidPoisonDroplet,
    .images = NULL,
    .affineAnims = gAffineAnims_Droplet,
    .callback = AnimAcidPoisonDroplet,
};

const struct SpriteTemplate gMudBombSludgeSpriteTemplate =
{
    .tileTag = ANIM_TAG_POISON_BUBBLE,
    .paletteTag = ANIM_TAG_BROWN_ORB,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = sAnims_PoisonProjectile,
    .images = NULL,
    .affineAnims = sAffineAnims_PoisonProjectile,
    .callback = AnimRockFragment,
};

// Animates a sludge project.
// arg 0: initial x offset
// arg 1: initial y offset
// arg 2: target x offset
// arg 3: target y offset
// arg 4: speed
// arg 5: wave amplitude
// arg 6: use current image frame (boolean)
static void AnimSludgeProjectile(struct Sprite *sprite)
{
    if (!gBattleAnimArgs[6])
        StartSpriteAnim(sprite, 2);
	
	AnimThrowProjectile(sprite);
}

// Animates the acid projectile in MOVE_ACID's anim.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: speed
// arg 3: use current image frame
// arg 4: final x pixel offset
// arg 5: final y pixel offset
// arg 6: if hit both foes (boolean)
static void AnimAcidPoisonBubble(struct Sprite *sprite)
{
    s16 x, y;

    if (!gBattleAnimArgs[3])
        StartSpriteAnim(sprite, 2);
	
	if (gBattleAnimArgs[6])
		SetAverageBattlerPositions(gBattleAnimTarget, TRUE, &x, &y);
	else
	{
		InitSpritePosToAnimTarget(sprite, TRUE);
		x = sprite->x;
		y = sprite->y;
	}
    InitSpritePosToAnimAttacker(sprite, TRUE);
	
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        gBattleAnimArgs[4] = -gBattleAnimArgs[4];
	
    sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[2] = x + gBattleAnimArgs[4];
    sprite->data[4] = y + gBattleAnimArgs[5];
    sprite->data[5] = -30;
    InitAnimArcTranslation(sprite);
    sprite->callback = DestroyAnimSpriteAfterHorizontalTranslation;
}

// Animates Sludge Bomb's poison hit particles sprite on target.
// arg 0: final x offset
// arg 1: final y offset
// arg 2: duration
static void AnimSludgeBombHitParticle(struct Sprite *sprite)
{
    sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[1] = sprite->x;
    sprite->data[2] = sprite->x + gBattleAnimArgs[0];
    sprite->data[3] = sprite->y;
    sprite->data[4] = sprite->y + gBattleAnimArgs[1];
    InitSpriteDataForLinearTranslation(sprite);
    sprite->data[5] = sprite->data[1] / gBattleAnimArgs[2];
    sprite->data[6] = sprite->data[2] / gBattleAnimArgs[2];
    sprite->callback = AnimSludgeBombHitParticle_Step;
}

static void AnimSludgeBombHitParticle_Step(struct Sprite *sprite)
{
    TranslateSpriteLinearFixedPoint(sprite);
	
    sprite->data[1] -= sprite->data[5];
    sprite->data[2] -= sprite->data[6];
	
    if (!sprite->data[0])
        DestroyAnimSprite(sprite);
}

// Animates the poison droplets in MOVE_ACID's anim.
// arg 0: initial x offset
// arg 1: initial y offset
// arg 2: final x offset
// arg 3: duration
// arg 4: if hit both foes (boolean)
static void AnimAcidPoisonDroplet(struct Sprite *sprite)
{
	if (gBattleAnimArgs[4])
		SetAverageBattlerPositions(gBattleAnimTarget, TRUE, &sprite->x, &sprite->y);
	
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        gBattleAnimArgs[0] = -gBattleAnimArgs[0];
	
    sprite->x += gBattleAnimArgs[0];
    sprite->y += gBattleAnimArgs[1];
    sprite->data[0] = gBattleAnimArgs[3];
    sprite->data[2] = sprite->x + gBattleAnimArgs[2];
    sprite->data[4] = sprite->y + sprite->data[0];
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Performs a wavy transformation on the mon's sprite, and fades out.
// arg 0: which battler
void AnimTask_AcidArmor(u8 taskId)
{
    u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);
    u16 bgX, bgY;
    s16 y, i;
    struct ScanlineEffectParams scanlineParams;
    struct Task *task = &gTasks[taskId];

    task->data[0] = 0;
    task->data[1] = 0;
    task->data[2] = 0;
    task->data[3] = 16;
    task->data[4] = 0;
    task->data[5] = battler;
    task->data[6] = 32;
    task->data[7] = 0;
    task->data[8] = 24;

    if (GetBattlerSide(battler) == B_SIDE_OPPONENT)
        task->data[8] *= -task->data[8];

    task->data[13] = GetBattlerYCoordWithElevation(battler) - 34;
    if (task->data[13] < 0)
        task->data[13] = 0;

    task->data[14] = task->data[13] + 66;
    task->data[15] = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);
	
    if (GetBattlerSpriteBGPriorityRank(battler) == 1)
    {
        scanlineParams.dmaDest = &REG_BG1HOFS;
		
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_BG1);
		
        bgX = gBattle_BG1_X;
        bgY = gBattle_BG1_Y;
    }
    else
    {
        scanlineParams.dmaDest = &REG_BG2HOFS;
		
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_BG2);
		
        bgX = gBattle_BG2_X;
        bgY = gBattle_BG2_Y;
    }

    for (y = 0, i = 0; y < 160; y++, i += 2)
    {
        gScanlineEffectRegBuffers[0][i] = bgX;
        gScanlineEffectRegBuffers[1][i] = bgX;
        gScanlineEffectRegBuffers[0][i + 1] = bgY;
        gScanlineEffectRegBuffers[1][i + 1] = bgY;
    }
    scanlineParams.dmaControl = SCANLINE_EFFECT_DMACNT_32BIT;
    scanlineParams.initState = 1;
    ScanlineEffect_SetParams(scanlineParams);
	
    task->func = AnimTask_AcidArmorStep;
}

static void AnimTask_AcidArmorStep(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    s16 var1;
    s16 var2;
    s16 bgX, bgY;
    s16 offset;
    s16 var0;
    s16 i;
    s16 sineIndex;
    s16 var3;

    if (GetBattlerSpriteBGPriorityRank(task->data[5]) == 1)
    {
        bgX = gBattle_BG1_X;
        bgY = gBattle_BG1_Y;
    }
    else
    {
        bgX = gBattle_BG2_X;
        bgY = gBattle_BG2_Y;
    }

    switch (task->data[0])
    {
    case 0:
        offset = task->data[14] * 2;
		
        var1 = 0;
        var2 = 0;
		
        i = 0;
		
        task->data[1] = (task->data[1] + 2) & 0xFF;
        sineIndex = task->data[1];
		
        task->data[9] = 0x7E0 / task->data[6];
        task->data[10] = -((task->data[7] * 2) / task->data[9]);
        task->data[11] = task->data[7];
		
        var3 = task->data[11] >> 5;
        task->data[12] = var3;
        var0 = task->data[14];
		
        while (var0 > task->data[13])
        {
            gScanlineEffectRegBuffers[gScanlineEffect.srcBuffer][offset + 1] = (i - var2) + bgY;
            gScanlineEffectRegBuffers[gScanlineEffect.srcBuffer][offset] = bgX + var3 + (gSineTable[sineIndex] >> 5);
			
            sineIndex = (sineIndex + 10) & 0xFF;
            task->data[11] += task->data[10];
            var3 = task->data[11] >> 5;
            task->data[12] = var3;

            i++;
            offset -= 2;
            var1 += task->data[6];
            var2 = var1 >> 5;
            var0--;
        }
        var0 *= 2;
		
        while (var0 >= 0)
        {
            gScanlineEffectRegBuffers[0][var0] = bgX + 240;
            gScanlineEffectRegBuffers[1][var0] = bgX + 240;
            var0 -= 2;
        }

        if (++task->data[6] > 63)
        {
            task->data[6] = 64;
			
            if (++task->data[2] & 1)
                task->data[3]--;
            else
                task->data[4]++;

            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(task->data[3], task->data[4]));
			
            if (task->data[3] == 0 && task->data[4] == 16)
            {
                task->data[2] = 0;
                task->data[3] = 0;
                task->data[0]++;
            }
        }
        else
            task->data[7] += task->data[8];
        break;
    case 1:
        if (++task->data[2] > 12)
        {
			task->data[2] = 0;
            gScanlineEffect.state = 3;
            task->data[0]++;
        }
        break;
    case 2:
        if (++task->data[2] & 1)
            task->data[3]++;
        else
            task->data[4]--;

        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(task->data[3], task->data[4]));
		
        if (task->data[3] == 16 && task->data[4] == 0)
        {
            task->data[2] = 0;
            task->data[3] = 0;
            task->data[0]++;
        }
        break;
    case 3:
        DestroyAnimVisualTask(taskId);
        break;
    }
}

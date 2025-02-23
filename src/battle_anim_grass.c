#include "global.h"
#include "battle_anim.h"
#include "gpu_regs.h"
#include "palette.h"
#include "random.h"
#include "sound.h"
#include "trig.h"
#include "constants/songs.h"

static void AnimRazorLeafParticleStep1(struct Sprite *);
static void AnimRazorLeafParticleStep2(struct Sprite *);
static void AnimMoveTwisterParticleStep(struct Sprite *);
static void AnimBulletSeed_Step1(struct Sprite *);
static void AnimBulletSeed_Step2(struct Sprite *);
static void AnimPetalDanceBigFlowerStep(struct Sprite *);
static void AnimPetalDanceSmallFlowerStep(struct Sprite *);
static void AnimTask_LeafBladeStep(u8 taskId);
static void AnimTask_LeafBladeStep2(u8 taskId);
static void AnimTask_LeafBladeStep2_Callback(struct Sprite *);
static void AnimSporeParticle(struct Sprite *);
static void AnimSporeParticleStep(struct Sprite *);
static void AnimIngrainRoot(struct Sprite* sprite);
static void AnimRootFlickerOut(struct Sprite *);
static void AnimIngrainOrb(struct Sprite* sprite);
static void AnimFrenzyPlantRoot(struct Sprite *sprite);

static const u16 sMagicalLeafBlendColors[] =
{
    RGB(31, 0, 0),
    RGB(31, 19, 0),
    RGB(31, 31, 0),
    RGB(0, 31, 0),
    RGB(5, 14, 31),
    RGB(22, 10, 31),
    RGB(22, 21, 31),
};

static const union AnimCmd sRazorLeafParticleAnimCmds1[] =
{
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_FRAME(4, 5),
    ANIMCMD_FRAME(8, 5),
    ANIMCMD_FRAME(12, 5),
    ANIMCMD_FRAME(16, 5),
    ANIMCMD_FRAME(20, 5),
    ANIMCMD_FRAME(16, 5),
    ANIMCMD_FRAME(12, 5),
    ANIMCMD_FRAME(8, 5),
    ANIMCMD_FRAME(4, 5),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sRazorLeafParticleAnimCmds2[] =
{
    ANIMCMD_FRAME(24, 5),
    ANIMCMD_FRAME(28, 5),
    ANIMCMD_FRAME(32, 5),
    ANIMCMD_END,
};

static const union AnimCmd *const sRazorLeafParticleAnimTable[] =
{
    sRazorLeafParticleAnimCmds1,
    sRazorLeafParticleAnimCmds2,
};

const struct SpriteTemplate gRazorLeafParticleSpriteTemplate =
{
    .tileTag = ANIM_TAG_LEAF,
    .paletteTag = ANIM_TAG_LEAF,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = sRazorLeafParticleAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimRazorLeafParticle,
};

const struct SpriteTemplate gTwisterLeafParticleSpriteTemplate =
{
    .tileTag = ANIM_TAG_LEAF,
    .paletteTag = ANIM_TAG_LEAF,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = sRazorLeafParticleAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimMoveTwisterParticle,
};

static const union AnimCmd sLeafBladeAnimCmds1[] =
{
    ANIMCMD_FRAME(28, 1),
    ANIMCMD_END,
};

static const union AnimCmd sLeafBladeAnimCmds2[] =
{
    ANIMCMD_FRAME(32, 1),
    ANIMCMD_END,
};

static const union AnimCmd sLeafBladeAnimCmds3[] =
{
    ANIMCMD_FRAME(20, 1),
    ANIMCMD_END,
};

static const union AnimCmd sLeafBladeAnimCmds4[] =
{
    ANIMCMD_FRAME(28, 1, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd sLeafBladeAnimCmds5[] =    
{
    ANIMCMD_FRAME(16, 1),
    ANIMCMD_END,
};

static const union AnimCmd sLeafBladeAnimCmds6[] =    
{
    ANIMCMD_FRAME(16, 1, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd sLeafBladeAnimCmds7[] =    
{
    ANIMCMD_FRAME(28, 1),
    ANIMCMD_END,
};

static const union AnimCmd *const sLeafBladeAnimTable[] =
{
    sLeafBladeAnimCmds1,
    sLeafBladeAnimCmds2,
    sLeafBladeAnimCmds3,
    sLeafBladeAnimCmds4,
    sLeafBladeAnimCmds5,
    sLeafBladeAnimCmds6,
    sLeafBladeAnimCmds7,
};

const struct SpriteTemplate gLeafBladeSpriteTemplate =    
{
    .tileTag = ANIM_TAG_LEAF,
    .paletteTag = ANIM_TAG_LEAF,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = sLeafBladeAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const union AnimCmd sRazorLeafCutterAnimCmds[] =
{
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_FRAME(0, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(0, 3, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_FRAME(0, 3, .vFlip = TRUE),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sRazorLeafCutterAnimTable[] =
{
    sRazorLeafCutterAnimCmds,
};

const struct SpriteTemplate gRazorLeafCutterSpriteTemplate =
{
    .tileTag = ANIM_TAG_RAZOR_LEAF,
    .paletteTag = ANIM_TAG_RAZOR_LEAF,
    .oam = &gOamData_AffineOff_ObjNormal_32x16,
    .anims = sRazorLeafCutterAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimTranslateLinearSingleSineWave,
};

const struct SpriteTemplate gScatterLeafSpriteTemplate =
{
	.tileTag = ANIM_TAG_LEAF,
    .paletteTag = ANIM_TAG_LEAF,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = sRazorLeafParticleAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimDizzyPunchDuck,
};

static const union AffineAnimCmd sBulletSeedAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(0, 0, 20, 1),
    AFFINEANIMCMD_JUMP(0),
};

const union AffineAnimCmd *const gBulletSeedAffineAnimTable[] =
{
    sBulletSeedAffineAnimCmds,
};

const struct SpriteTemplate gBulletSeedSpriteTemplate =    
{
    .tileTag = ANIM_TAG_SEED,
    .paletteTag = ANIM_TAG_SEED,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gBulletSeedAffineAnimTable,
    .callback = AnimBulletSeed,
};

static const union AnimCmd sPetalDanceBigFlowerAnimCmds[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

static const union AnimCmd sPetalDanceSmallFlowerAnimCmds[] =
{
    ANIMCMD_FRAME(4, 1),
    ANIMCMD_END,
};

static const union AnimCmd *const sPetalDanceBigFlowerAnimTable[] =
{
    sPetalDanceBigFlowerAnimCmds,
};

static const union AnimCmd *const sPetalDanceSmallFlowerAnimTable[] =
{
    sPetalDanceSmallFlowerAnimCmds,
};

const struct SpriteTemplate gPetalDanceBigFlowerSpriteTemplate =
{
    .tileTag = ANIM_TAG_FLOWER,
    .paletteTag = ANIM_TAG_FLOWER,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = sPetalDanceBigFlowerAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimPetalDanceFlower,
};

const struct SpriteTemplate gPetalDanceSmallFlowerSpriteTemplate =
{
    .tileTag = ANIM_TAG_FLOWER,
    .paletteTag = ANIM_TAG_FLOWER,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = sPetalDanceSmallFlowerAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimPetalDanceFlower,
};

static const union AffineAnimCmd sAromatherapyBigFlowerAffineAnimCmds[] = 
{
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_FRAME(0, 0, 4, 1),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd *const sAromatherapyBigFlowerAffineAnimTable[] = 
{
    sAromatherapyBigFlowerAffineAnimCmds,
};

const struct SpriteTemplate gAromatherapySmallFlowerSpriteTemplate = 
{
    .tileTag = ANIM_TAG_FLOWER,
    .paletteTag = ANIM_TAG_FLOWER,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = sPetalDanceSmallFlowerAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimFlyingParticle,
};

const struct SpriteTemplate gAromatherapyBigFlowerSpriteTemplate = 
{
    .tileTag = ANIM_TAG_FLOWER,
    .paletteTag = ANIM_TAG_FLOWER,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = sPetalDanceBigFlowerAnimTable,
    .images = NULL,
    .affineAnims = sAromatherapyBigFlowerAffineAnimTable,
    .callback = AnimFlyingParticle,
};

static const union AnimCmd sSporeParticleAnimCmds1[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

static const union AnimCmd sSporeParticleAnimCmds2[] =
{
    ANIMCMD_FRAME(4, 7),
    ANIMCMD_END,
};

static const union AnimCmd *const sSporeParticleAnimTable[] =
{
    sSporeParticleAnimCmds1,
    sSporeParticleAnimCmds2,
};

const struct SpriteTemplate gSporeParticleSpriteTemplate =
{
    .tileTag = ANIM_TAG_SPORE,
    .paletteTag = ANIM_TAG_SPORE,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = sSporeParticleAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSporeParticle,
};

static const union AnimCmd sIngrainRootAnimCmds1[] =
{
    ANIMCMD_FRAME(0, 7),
    ANIMCMD_FRAME(16, 7),
    ANIMCMD_FRAME(32, 7),
    ANIMCMD_FRAME(48, 7),
    ANIMCMD_END,
};

static const union AnimCmd sIngrainRootAnimCmds2[] =
{
    ANIMCMD_FRAME(0, 7, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 7, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 7, .hFlip = TRUE),
    ANIMCMD_FRAME(48, 7, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd sIngrainRootAnimCmds3[] =
{
    ANIMCMD_FRAME(0, 7),
    ANIMCMD_FRAME(16, 7),
    ANIMCMD_FRAME(32, 7),
    ANIMCMD_END,
};

static const union AnimCmd sIngrainRootAnimCmds4[] =
{
    ANIMCMD_FRAME(0, 7, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 7, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 7, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd *const sIngrainRootAnimTable[] =
{
    sIngrainRootAnimCmds1,
    sIngrainRootAnimCmds2,
    sIngrainRootAnimCmds3,
    sIngrainRootAnimCmds4,
};

const struct SpriteTemplate gIngrainRootSpriteTemplate =
{
    .tileTag = ANIM_TAG_ROOTS,
    .paletteTag = ANIM_TAG_ROOTS,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sIngrainRootAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimIngrainRoot,
};

static const union AnimCmd sIngrainOrbAnimCmds[] =
{
    ANIMCMD_FRAME(3, 3),
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sIngrainOrbAnimTable[] =
{
    sIngrainOrbAnimCmds,
};

const struct SpriteTemplate gIngrainOrbSpriteTemplate =    
{
    .tileTag = ANIM_TAG_ORBS,
    .paletteTag = ANIM_TAG_ORBS,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = sIngrainOrbAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimIngrainOrb,
};

const struct SpriteTemplate gFrenzyPlantRootSpriteTemplate =
{
    .tileTag = ANIM_TAG_ROOTS,
    .paletteTag = ANIM_TAG_ROOTS,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sIngrainRootAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimFrenzyPlantRoot,
};

const struct SpriteTemplate gNeedleArmSpikeSpriteTemplate =    
{
    .tileTag = ANIM_TAG_GREEN_SPIKE,
    .paletteTag = ANIM_TAG_GREEN_SPIKE,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimNeedleArmSpike,
};

const struct SpriteTemplate gZygardeHexSpriteTemplate =
{
    .tileTag = ANIM_TAG_ZYGARDE_HEXES,
    .paletteTag = ANIM_TAG_ZYGARDE_HEXES,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = sRazorLeafParticleAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimMoveTwisterParticle,
};

const struct SpriteTemplate gSuckerPunchSpriteTemplate =
{
    .tileTag = ANIM_TAG_POISON_JAB,
    .paletteTag = ANIM_TAG_POISON_JAB,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = sRazorLeafCutterAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSlidingKick,
};

const struct SpriteTemplate gSeedBombSeedSpriteTemplate =
{
    .tileTag = ANIM_TAG_SEED,
    .paletteTag = ANIM_TAG_SEED,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gBulletSeedAffineAnimTable,
    .callback = AnimMissileArc,
};

const struct SpriteTemplate gEnergyBallSpriteTemplate =
{
    .tileTag = ANIM_TAG_ENERGY_BALL,
    .paletteTag = ANIM_TAG_ENERGY_BALL,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gAffineAnims_ShadowBall,
    .callback = AnimShadowBall,
};

// Shoots a leaf upward, then floats it downward while swaying back and forth.
// arg 0: upward x delta per frame
// arg 1: upward y delta per frame
// arg 2: upward duration
void AnimRazorLeafParticle(struct Sprite* sprite)
{
    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
	
    sprite->data[0] = gBattleAnimArgs[0];
    sprite->data[1] = gBattleAnimArgs[1];
    sprite->data[2] = gBattleAnimArgs[2];
    sprite->callback = AnimRazorLeafParticleStep1;
}

static void AnimRazorLeafParticleStep1(struct Sprite* sprite)
{
    if (sprite->data[2] == 0)
    {
        if (sprite->data[1] & 1)
            sprite->data[0] = 0x80;
        else
            sprite->data[0] = 0;
            
		sprite->data[1] = 0;
		sprite->data[2] = 0;
        sprite->callback = AnimRazorLeafParticleStep2;
    }
    else
    {
        sprite->data[2]--;
        sprite->x += sprite->data[0];
        sprite->y += sprite->data[1];
    }
}

static void AnimRazorLeafParticleStep2(struct Sprite* sprite)
{
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        sprite->x2 = -Sin(sprite->data[0], 25);
    else
        sprite->x2 = Sin(sprite->data[0], 25);

    sprite->data[0] += 2;
    sprite->data[0] &= 0xFF;
    ++sprite->data[1];
	
    if (!(sprite->data[1] & 1))
        sprite->y2++;

    if (sprite->data[1] > 80)
        DestroyAnimSprite(sprite);
}

// Animates particles in the Twister move animation.
// arg 0: duration
// arg 1: total y delta (the particles rise upward)
// arg 2: wave period (higher means faster wave)
// arg 3: wave amplitude
// arg 4: speedup frame (particles move faster at the end of the animation)
void AnimMoveTwisterParticle(struct Sprite* sprite)
{
    if (IsDoubleBattleForBattler(gBattleAnimTarget))
        SetAverageBattlerPositions(gBattleAnimTarget, TRUE, &sprite->x, &sprite->y);

    sprite->y += 32;
    sprite->data[0] = gBattleAnimArgs[0];
    sprite->data[1] = gBattleAnimArgs[1];
    sprite->data[2] = gBattleAnimArgs[2];
    sprite->data[3] = gBattleAnimArgs[3];
    sprite->data[4] = gBattleAnimArgs[4];
    sprite->callback = AnimMoveTwisterParticleStep;
}

static void AnimMoveTwisterParticleStep(struct Sprite* sprite)
{
    if (sprite->data[1] == 0xFF)
        sprite->y -= 2;
    else if (sprite->data[1] > 0)
    {
        sprite->y -= 2;
        sprite->data[1] -= 2;
    }
    sprite->data[5] += sprite->data[2];
	
    if (sprite->data[0] < sprite->data[4])
        sprite->data[5] += sprite->data[2];

    sprite->data[5] &= 0xFF;
	
    sprite->x2 = Cos(sprite->data[5], sprite->data[3]);
    sprite->y2 = Sin(sprite->data[5], 5);
	
    if (sprite->data[5] < 0x80)
        sprite->oam.priority = GetBattlerSpriteBGPriority(gBattleAnimTarget) - 1;
    else
        sprite->oam.priority = GetBattlerSpriteBGPriority(gBattleAnimTarget) + 1;

    if (--sprite->data[0] == 0)
        DestroyAnimSprite(sprite);
}

static s16 LeafBladeGetPosFactor(struct Sprite* sprite)
{
    s16 var = 8;
    
    if (sprite->data[4] < sprite->y)
        var = -var;

    return var;
}

// Animates Leaf Blade's cross leafs.
// No args.
void AnimTask_LeafBlade(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    task->data[4] = GetBattlerSpriteSubpriority(gBattleAnimTarget) - 1;
	
    task->data[6] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    task->data[7] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
	
    task->data[10] = GetBattlerSpriteCoordAttr(gBattleAnimTarget, BATTLER_COORD_ATTR_WIDTH);
    task->data[11] = GetBattlerSpriteCoordAttr(gBattleAnimTarget, BATTLER_COORD_ATTR_HEIGHT);
	
    task->data[5] = (GetBattlerSide(gBattleAnimTarget) == B_SIDE_OPPONENT) ? 1 : -1;
    task->data[9] = 56 - (task->data[5] * 64);
    task->data[8] = task->data[7] - task->data[9] + task->data[6];
    task->data[2] = CreateSprite(&gLeafBladeSpriteTemplate, task->data[8], task->data[9], task->data[4]);
	
    if (task->data[2] == MAX_SPRITES)
        DestroyAnimVisualTask(taskId);

    gSprites[task->data[2]].data[0] = 10;
    gSprites[task->data[2]].data[1] = task->data[8];
    gSprites[task->data[2]].data[2] = task->data[6] - (task->data[10] / 2 + 10) * task->data[5];
    gSprites[task->data[2]].data[3] = task->data[9];
    gSprites[task->data[2]].data[4] = task->data[7] + (task->data[11] / 2 + 10) * task->data[5];
    gSprites[task->data[2]].data[5] = LeafBladeGetPosFactor(&gSprites[task->data[2]]);
    InitAnimArcTranslation(&gSprites[task->data[2]]);
    task->func = AnimTask_LeafBladeStep;
}

static void AnimTask_LeafBladeStep(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    struct Sprite* sprite = &gSprites[task->data[2]];
    int a = task->data[0];
    
    switch (a)
    {
    case 4:
        AnimTask_LeafBladeStep2(taskId);
		
        if (TranslateAnimHorizontalArc(sprite))
        {
            task->data[15] = 5;
            task->data[0] = 0xFF;
        }
        break;
    case 8:
        AnimTask_LeafBladeStep2(taskId);
		
        if (TranslateAnimHorizontalArc(sprite))
        {
            task->data[15] = 9;
            task->data[0] = 0xFF;
        }
        break;
    case 0:
        AnimTask_LeafBladeStep2(taskId);
		
        if (TranslateAnimHorizontalArc(sprite))
        {
            task->data[15] = 1;
            task->data[0] = 0xFF;
        }
        break;
    case 1:
        SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
		
        sprite->data[0] = 10;
        sprite->data[1] = sprite->x;
        sprite->data[2] = task->data[6];
        sprite->data[3] = sprite->y;
        sprite->data[4] = task->data[7];
        sprite->data[5] = LeafBladeGetPosFactor(sprite);
		
        task->data[4] += 2;
        task->data[3] = a;
		
        sprite->subpriority = task->data[4];
        StartSpriteAnim(sprite, task->data[3]);
        InitAnimArcTranslation(sprite);
        task->data[0]++;
        break;
    case 2:
        AnimTask_LeafBladeStep2(taskId);
		
        if (TranslateAnimHorizontalArc(sprite))
        {
            task->data[15] = 3;
            task->data[0] = 0xFF;
        }
        break;
    case 3:
        SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
		
        sprite->data[0] = 10;
        sprite->data[1] = sprite->x;
        sprite->data[2] = task->data[6] - ((task->data[10] / 2) + 10) * task->data[5];
        sprite->data[3] = sprite->y;
        sprite->data[4] = task->data[7] - ((task->data[11] / 2) + 10) * task->data[5];
        sprite->data[5] = LeafBladeGetPosFactor(sprite);
		
        task->data[3] = 2;
		
        sprite->subpriority = task->data[4];
        StartSpriteAnim(sprite, task->data[3]);
        InitAnimArcTranslation(sprite);
        task->data[0]++;
        break;
    case 5:
        SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
		
        sprite->data[0] = 10;
        sprite->data[1] = sprite->x;
        sprite->data[2] = task->data[6] + ((task->data[10] / 2) + 10) * task->data[5];
        sprite->data[3] = sprite->y;
        sprite->data[4] = task->data[7] + ((task->data[11] / 2) + 10) * task->data[5];
        sprite->data[5] = LeafBladeGetPosFactor(sprite);
		
        task->data[4] -= 2;
        task->data[3] = 3;
		
        sprite->subpriority = task->data[4];
        StartSpriteAnim(sprite, task->data[3]);
        InitAnimArcTranslation(sprite);
        task->data[0]++;
        break;
    case 6:
        AnimTask_LeafBladeStep2(taskId);
		
        if (TranslateAnimHorizontalArc(sprite))
        {
            task->data[15] = 7;
            task->data[0] = 0xFF;
        }
        break;
    case 7:
        SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
		
        sprite->data[0] = 10;
        sprite->data[1] = sprite->x;
        sprite->data[2] = task->data[6];
        sprite->data[3] = sprite->y;
        sprite->data[4] = task->data[7];
        sprite->data[5] = LeafBladeGetPosFactor(sprite);
		
        task->data[4] += 2;
        task->data[3] = 4;
		
        sprite->subpriority = task->data[4];
        StartSpriteAnim(sprite, task->data[3]);
        InitAnimArcTranslation(sprite);
        task->data[0]++;
        break;
    case 9:
        SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
		
        sprite->data[0] = 10;
        sprite->data[1] = sprite->x;
        sprite->data[2] = task->data[6] - ((task->data[10] / 2) + 10) * task->data[5];
        sprite->data[3] = sprite->y;
        sprite->data[4] = task->data[7] + ((task->data[11] / 2) + 10) * task->data[5];
        sprite->data[5] = LeafBladeGetPosFactor(sprite);
		
        task->data[3] = 5;
		
        sprite->subpriority = task->data[4];
        StartSpriteAnim(sprite, task->data[3]);
        InitAnimArcTranslation(sprite);
        task->data[0]++;
        break;
    case 10:
        AnimTask_LeafBladeStep2(taskId);
		
        if (TranslateAnimHorizontalArc(sprite))
        {
            task->data[15] = 11;
            task->data[0] = 0xFF;
        }
        break;
    case 11:
        SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
		
        sprite->data[0] = 10;
        sprite->data[1] = sprite->x;
        sprite->data[2] = task->data[8];
        sprite->data[3] = sprite->y;
        sprite->data[4] = task->data[9];
        sprite->data[5] = LeafBladeGetPosFactor(sprite);
		
        task->data[4] -= 2;
        task->data[3] = 6;
		
        sprite->subpriority = task->data[4];
        StartSpriteAnim(sprite, task->data[3]);
        InitAnimArcTranslation(sprite);
        task->data[0]++;
        break;
    case 12:
        AnimTask_LeafBladeStep2(taskId);
		
        if (TranslateAnimHorizontalArc(sprite))
        {
            DestroySprite(sprite);
            task->data[0]++;
        }
        break;
    case 13:
        if (task->data[12] == 0)
            DestroyAnimVisualTask(taskId);
        break;
    case 0xFF:
        if (++task->data[1] > 5)
        {
            task->data[1] = 0;
            task->data[0] = task->data[15];
        }
        break;
    }
}

static void AnimTask_LeafBladeStep2(u8 taskId)
{
	u8 spriteId;
    s16 spriteX;
    s16 spriteY;
	struct Task *task = &gTasks[taskId];
	
    if (++task->data[14] > 0)
    {
        task->data[14] = 0;
		
        spriteX = gSprites[task->data[2]].x + gSprites[task->data[2]].x2;
        spriteY = gSprites[task->data[2]].y + gSprites[task->data[2]].y2;
		
        spriteId = CreateSprite(&gLeafBladeSpriteTemplate, spriteX, spriteY, task->data[4]);
		
        if (spriteId != MAX_SPRITES)
        {
            gSprites[spriteId].data[6] = taskId;
            gSprites[spriteId].data[7] = 12;
			
            gTasks[taskId].data[12]++;
			
            gSprites[spriteId].data[0] = task->data[13] & 1;
			
            gTasks[taskId].data[13]++;
			
            StartSpriteAnim(&gSprites[spriteId], task->data[3]);
            gSprites[spriteId].subpriority = task->data[4];
            gSprites[spriteId].callback = AnimTask_LeafBladeStep2_Callback;
        }
    }
}

static void AnimTask_LeafBladeStep2_Callback(struct Sprite* sprite)
{
    if (++sprite->data[0] > 1)
    {
        sprite->data[0] = 0;
		
        sprite->invisible ^= TRUE;

        if (++sprite->data[1] > 8)
        {
            gTasks[sprite->data[6]].data[sprite->data[7]]--;
            DestroySprite(sprite);
        }
    }
}

// Animates the palette cycle in Magical Leaf's anim.
// No args.
void AnimTask_CycleMagicalLeafPal(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    
    switch (task->data[0])
    {
    case 0:
        task->data[8] = IndexOfSpritePaletteTag(ANIM_TAG_LEAF) * 16 + 256;
        task->data[12] = IndexOfSpritePaletteTag(ANIM_TAG_RAZOR_LEAF) * 16 + 256;
        task->data[0]++;
        break;
    case 1:
        if (++task->data[9] >= 0)
        {
            task->data[9] = 0;
			
            BlendPalette(task->data[8], 16, task->data[10], sMagicalLeafBlendColors[task->data[11]]);
            BlendPalette(task->data[12], 16, task->data[10], sMagicalLeafBlendColors[task->data[11]]);
			
            if (++task->data[10] == 17)
            {
                task->data[10] = 0;
				
                if (++task->data[11] == 7)
                    task->data[11] = 0;
            }
        }
        break;
    }
	
	// Signal to end the cycle effect
    if (gBattleAnimArgs[ARG_RET_ID] == -1)
        DestroyAnimVisualTask(taskId);
}

// Animates the seeds in MOVE_BULLET_SEED moving to the target and then falling around.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: se to play (play specified se if set)
void AnimBulletSeed(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, TRUE);
    sprite->data[0] = 20;
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
	sprite->data[5] = gBattleAnimArgs[2];
	sprite->affineAnimPaused = TRUE;
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, AnimBulletSeed_Step1);
}

static void AnimBulletSeed_Step1(struct Sprite *sprite)
{
    int i;
    s16* ptr;
    
	if (sprite->data[5] > MUS_DUMMY)
		PlaySE12WithPanning(sprite->data[5], BattleAnimAdjustPanning(SOUND_PAN_TARGET));
	
    SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
	
    ptr = &sprite->data[7];
    for (i = 0; i < 8; i++)
        ptr[i - 7] = 0;

    sprite->data[6] = 0xFFF4 - RandomMax(8);
    sprite->data[7] = RandomMax(0xA0) + 0xA0;
	sprite->affineAnimPaused = FALSE;
    sprite->callback = AnimBulletSeed_Step2;
}

static void AnimBulletSeed_Step2(struct Sprite *sprite)
{
    sprite->data[0] += sprite->data[7];
	
    sprite->x2 = sprite->data[0] >> 8;
    if (sprite->data[7] & 1)
        sprite->x2 = -sprite->x2;

    sprite->y2 = Sin(sprite->data[1], sprite->data[6]);
	
    sprite->data[1] += 8;
	
    if (sprite->data[1] > 126)
    {
        sprite->data[1] = 0;
        sprite->data[2] /= 2;
		
        if (++sprite->data[3] == 1)
            DestroyAnimSprite(sprite);
    }
}

// Rotates a big or small flower around the attacking mon, and slowly floats downward.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: target y pixel offset
// arg 3: duration
// arg 4: big flower (boolean)
void AnimPetalDanceFlower(struct Sprite* sprite)
{
    InitSpritePosToAnimAttacker(sprite, FALSE);
    sprite->data[0] = gBattleAnimArgs[3];
    sprite->data[1] = sprite->x;
    sprite->data[2] = sprite->x;
    sprite->data[3] = sprite->y;
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[2];
    InitAnimLinearTranslation(sprite);
    sprite->data[5] = 0x40;
    sprite->callback = gBattleAnimArgs[4] ? AnimPetalDanceBigFlowerStep : AnimPetalDanceSmallFlowerStep;
    sprite->callback(sprite);
}

static void AnimPetalDanceBigFlowerStep(struct Sprite* sprite)
{
    if (!AnimTranslateLinear(sprite))
    {
        sprite->x2 += Sin(sprite->data[5], 32);
        sprite->y2 += Cos(sprite->data[5], -5);
		
        if ((u16)(sprite->data[5] - 0x40) < 0x80)
            sprite->subpriority = GetBattlerSpriteSubpriority(gBattleAnimAttacker) - 1;
        else
            sprite->subpriority = GetBattlerSpriteSubpriority(gBattleAnimAttacker) + 1;

        sprite->data[5] = (sprite->data[5] + 5) & 0xFF;
    }
    else
        DestroyAnimSprite(sprite);
}

static void AnimPetalDanceSmallFlowerStep(struct Sprite* sprite)
{
    if (!AnimTranslateLinear(sprite))
    {
        sprite->x2 += Sin(sprite->data[5], 8);
		
        if ((u16)(sprite->data[5] - 59) < 5 || (u16)(sprite->data[5] - 187) < 5)
            sprite->oam.matrixNum ^= ST_OAM_HFLIP;

        sprite->data[5] += 5;
        sprite->data[5] &= 0xFF;
    }
    else
       DestroyAnimSprite(sprite);
}

// Moves a spore particle in a halo around the target mon.
// The sprite's priority is updated to give the effect of going
// behind the mon's sprite.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: initial wave offset
// arg 3: duration
// arg 4: blend on (boolean)
// arg 5: anim battler
void AnimSporeParticle(struct Sprite* sprite)
{
    InitSpritePosToAnimBattler(sprite, gBattleAnimArgs[5], TRUE);
	
    StartSpriteAnim(sprite, gBattleAnimArgs[4]);
	
    if (gBattleAnimArgs[4])
        sprite->oam.objMode = ST_OAM_OBJ_BLEND;

    sprite->data[0] = gBattleAnimArgs[3];
    sprite->data[1] = gBattleAnimArgs[2];
	sprite->data[3] = GetBattlerForAnimScript(gBattleAnimArgs[5]);
    sprite->callback = AnimSporeParticleStep;
    sprite->callback(sprite);
}

static void AnimSporeParticleStep(struct Sprite* sprite)
{
    sprite->x2 = Sin(sprite->data[1], 32);
    sprite->y2 = Cos(sprite->data[1], -3) + ((sprite->data[2] += 24) >> 8);
	
    if ((u16)(sprite->data[1] - 0x40) < 0x80)
        sprite->oam.priority = GetBattlerSpriteBGPriority(sprite->data[3]);
    else
    {
        u8 priority = GetBattlerSpriteBGPriority(sprite->data[3]) + 1;
        if (priority > 3)
            priority = 3;

        sprite->oam.priority = priority;
    }
    sprite->data[1] += 2;
    sprite->data[1] &= 0xFF;
	
    if (--sprite->data[0] == -1)
        DestroyAnimSprite(sprite);
}

static void CreateCottonSporeSporeSprite(u8 animBattler, s16 y, s16 waveOffset)
{
	u8 battlerId = GetBattlerForAnimScript(animBattler);
	
	gBattleAnimArgs[0] = 0;
	gBattleAnimArgs[1] = y;
	gBattleAnimArgs[2] = waveOffset;
	gBattleAnimArgs[3] = 80;
	gBattleAnimArgs[4] = FALSE;
	gBattleAnimArgs[5] = animBattler;
	CreateSpriteAndAnimate(&gSporeParticleSpriteTemplate, GetBattlerSpriteCoord(battlerId, BATTLER_COORD_X), GetBattlerSpriteCoord(battlerId, BATTLER_COORD_Y_PIC_OFFSET), GetBattlerSpriteSubpriority(gBattleAnimAttacker) + 2);
	gAnimVisualTaskCount++;
}

// Creates Cotton Spore's spores. Same as above, but creating on both Pokémon if aplicable.
// args are the initial y pixel offset and the initial wave offset of each spore created.
// Are created three spores three times, with a delay of 12 frames betwen each spore creation.
// So it creates 3 * 3 spores, with a delay of 12 betwhen each spore creation.
void AnimTask_CreateCottonSporeSpores(u8 taskId)
{
	u8 i, argId;
	s16 y, waveOffset;
	struct Task *task = &gTasks[taskId];

	switch (task->data[0])
	{
		case 0:
			task->data[3] = (IsDoubleBattleForBattler(gBattleAnimTarget) && IsBattlerSpriteVisible(BATTLE_PARTNER(gBattleAnimTarget))); // If hit ally
			
			// Save spores's positions
			for (i = 0; i < 3; i++)
			{
				argId = i * 2;
				task->data[5 + argId] = gBattleAnimArgs[argId];
				task->data[6 + argId] = gBattleAnimArgs[1 + argId];
			}
			++task->data[0];
			// fallthrough
		case 1:
			if (--task->data[2] <= 0) // Wait delay until create a spore sprite
			{
				task->data[2] = 12;
				
				argId = task->data[1] * 2;
				
				y = task->data[5 + argId];
				waveOffset = task->data[6 + argId];
				
				CreateCottonSporeSporeSprite(ANIM_TARGET, y, waveOffset);
				
				if (task->data[3]) // If creates on ally
					CreateCottonSporeSporeSprite(ANIM_DEF_PARTNER, y, waveOffset);
				
				if (++task->data[1] == 3) // Creates three spores at a time
				{
					task->data[1] = 0;
					
					if (++task->data[4] == 3) // Num times spore are created
						DestroyAnimVisualTask(taskId);
				}
			}
			break;
	}
}

// In a double battle where spore only affects one battler, Updates the mon sprite background priorities to allow the circling effect controlled by AnimSporeParticle.
// Used AnimTask_CreateCottonSporeSpores to create spores on both battlers.
// No args.
void AnimTask_SporeDoubleBattle(u8 taskId)
{
    if (IsDoubleBattleForBattler(gBattleAnimTarget))
    {
        if (GetBattlerSpriteBGPriorityRank(gBattleAnimTarget) == 1)
            SetAnimBgAttribute(2, BG_ANIM_PRIORITY, 3);
        else
            SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
    }
	DestroyAnimVisualTask(taskId);
}

// Animates a root that flickers away after some time.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: sprite subpriority offset
// arg 3: sprite anim num
// arg 4: duration
static void AnimIngrainRoot(struct Sprite* sprite)
{
    if (!sprite->data[0])
    {
		sprite->data[0]++;
		
        sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
        sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y);
		
        sprite->x2 = gBattleAnimArgs[0];
        sprite->y2 = gBattleAnimArgs[1];
		
        sprite->subpriority = gBattleAnimArgs[2] + 30;
        StartSpriteAnim(sprite, gBattleAnimArgs[3]);
		
        sprite->data[2] = gBattleAnimArgs[4];
		
        if (sprite->y + sprite->y2 > 120)
            sprite->y += sprite->y2 + sprite->y - 120;
    }
    sprite->callback = AnimRootFlickerOut;
}

static void AnimRootFlickerOut(struct Sprite* sprite)
{
    if (++sprite->data[0] > (sprite->data[2] - 10))
        sprite->invisible = sprite->data[0] % 2;

    if (sprite->data[0] > sprite->data[2])
        DestroyAnimSprite(sprite);
}

// Moves an orb in a fast wavy path.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: horizontal velocity
// arg 3: wave amplitude
// arg 4: duration
static void AnimIngrainOrb(struct Sprite* sprite)
{
    if (!sprite->data[0])
    {
        sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X) + gBattleAnimArgs[0];
        sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y) + gBattleAnimArgs[1];
		
        sprite->data[1] = gBattleAnimArgs[2];
        sprite->data[2] = gBattleAnimArgs[3];
        sprite->data[3] = gBattleAnimArgs[4];
    }
    sprite->data[0]++;
	
    sprite->x2 = sprite->data[1] * sprite->data[0];
    sprite->y2 = Sin((sprite->data[0] * 20) & 0xFF, sprite->data[2]);
	
    if (sprite->data[0] > sprite->data[3])
        DestroyAnimSprite(sprite);
}

// Places a root on the path to the target mon that flickers away after some time.
// arg 0: percent along the path to the target mon
// arg 1: x pixel offset
// arg 2: y pixel offset
// arg 3: sprite subpriority offset
// arg 4: sprite anim num
// arg 5: duration
static void AnimFrenzyPlantRoot(struct Sprite *sprite)
{
    s16 attackerX = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    s16 attackerY = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
    s16 targetX = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    s16 targetY = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);

    targetX -= attackerX;
    targetY -= attackerY;
	
    sprite->x = attackerX + targetX * gBattleAnimArgs[0] / 100;
    sprite->y = attackerY + targetY * gBattleAnimArgs[0] / 100;
	
    sprite->x2 = gBattleAnimArgs[1];
    sprite->y2 = gBattleAnimArgs[2];
	
    sprite->subpriority = gBattleAnimArgs[3] + 30;
	
    StartSpriteAnim(sprite, gBattleAnimArgs[4]);
	
    sprite->data[2] = gBattleAnimArgs[5];
    sprite->callback = AnimRootFlickerOut;
}

// Animates MOVE_NEEDLE_ARM's spikes.
// arg 0: anim battler
// arg 1: from center to around (boolean)
// arg 2: x slide
// arg 3: y slide
// arg 4: duration
void AnimNeedleArmSpike(struct Sprite* sprite)
{
    u8 a, b;
    u16 c, x, y;
	s16 targetX, targetY;

    if (gBattleAnimArgs[4] == 0)
        DestroyAnimSprite(sprite);
    else
    {
		u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);
		
        a = GetBattlerSpriteCoord(battler, BATTLER_COORD_X);
        b = GetBattlerSpriteCoord(battler, BATTLER_COORD_Y_PIC_OFFSET);
        
        sprite->data[2] = gBattleAnimArgs[4];
		
        if (!gBattleAnimArgs[1])
        {
            sprite->x = gBattleAnimArgs[2] + a;
            sprite->y = gBattleAnimArgs[3] + b;
			
            targetX = a;
            targetY = b;
        }
        else
        {
            sprite->x = a;
            sprite->y = b;
			
            targetX = gBattleAnimArgs[2] + a;
            targetY = gBattleAnimArgs[3] + b;
        }
        x = sprite->x;
        sprite->data[4] = x * 16;
		
        y = sprite->y;
        sprite->data[5] = y * 16;
		
        sprite->data[0] = (targetX - sprite->x) * 16 / sprite->data[2];
        sprite->data[1] = (targetY - sprite->y) * 16 / sprite->data[2];
		
        c = ArcTan2Neg(targetX - x, targetY - y);
        TrySetSpriteRotScale(sprite, 0, 0x100, 0x100, c);
		
		StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
        sprite->callback = AnimMoveSpriteOverDurationFast;
    }
}

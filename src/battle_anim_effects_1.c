#include "global.h"
#include "gflib.h"
#include "battle_anim.h"
#include "battle_gfx_sfx_util.h"
#include "battle_interface.h"
#include "random.h"
#include "trig.h"
#include "util.h"
#include "constants/songs.h"

// Function Declarations
static void AnimMovePowderParticleStep(struct Sprite *);
static void AnimSolarbeamSmallOrbStep(struct Sprite *);
static void AnimHyperBeamOrbStep(struct Sprite *);
static void AnimLeechSeedStep(struct Sprite *);
static void AnimLeechSeedSprouts(struct Sprite *);
static void AnimTranslateLinearSingleSineWaveStep(struct Sprite *);
static void AnimConstrictBindingStep1(struct Sprite *);
static void AnimConstrictBindingStep2(struct Sprite *);
static void AnimTask_DuplicateAndShrinkToPosStep1(u8);
static void AnimTask_DuplicateAndShrinkToPosStep2(u8);
static void AnimItemStealStep3(struct Sprite *);
static void AnimTrickBagStep1(struct Sprite *);
static void AnimTrickBagStep2(struct Sprite *);
static void AnimTrickBagStep3(struct Sprite *);
static void AnimFlyingParticleStep(struct Sprite *);
static void AnimSliceStep(struct Sprite *);
static void AnimProtectStep(struct Sprite *);
static void AnimMilkBottleStep1(struct Sprite *);
static void AnimMilkBottleStep2(struct Sprite *, int, int);
static void AnimSleepLetterZ(struct Sprite* sprite);
static void AnimSleepLetterZStep(struct Sprite *);
static void AnimLockOnTargetStep1(struct Sprite *);
static void AnimLockOnTargetStep2(struct Sprite *);
static void AnimLockOnTargetStep3(struct Sprite *);
static void AnimLockOnTargetStep4(struct Sprite *);
static void AnimLockOnTargetStep5(struct Sprite *);
static void AnimLockOnTargetStep6(struct Sprite *);
static void AnimBowMonStep1(struct Sprite *);
static void AnimBowMonStep1_Callback(struct Sprite *);
static void AnimBowMonStep2(struct Sprite *);
static void AnimBowMonStep3(struct Sprite *);
static void AnimBowMonStep3_Callback(struct Sprite *);
static void AnimTask_SkullBashPositionSet(u8);
static void AnimTask_SkullBashPositionReset(u8);
static void AnimFalseSwipeSliceStep1(struct Sprite *);
static void AnimFalseSwipeSliceStep2(struct Sprite *);
static void AnimFalseSwipeSliceStep3(struct Sprite *);
static void AnimEndureEnergyStep(struct Sprite *);
static void AnimSharpenSphereStep(struct Sprite *);
static void AnimConversion2Step(struct Sprite *);
static void AnimHornHitStep(struct Sprite *);
static void AnimTask_DoubleTeamStep(u8);
static void AnimTask_DoubleTeamCallback(struct Sprite *);
static void AnimWavyMusicNotesGetNextPos(s16, s16, s16 *, s16 *, s8);
static void AnimWavyMusicNotesStep(struct Sprite *);
static void AnimFlyingMusicNotesStep(struct Sprite *);
static void AnimSlowFlyingMusicNotesStep(struct Sprite *);
static void AnimThoughtBubbleStep(struct Sprite *);
static void AnimMetronomeFingerStep(struct Sprite *);
static void AnimFollowMeFingerStep1(struct Sprite *);
static void AnimFollowMeFingerStep2(struct Sprite *);

static const union AnimCmd sPowderParticlesAnimCmds[] =
{
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_FRAME(2, 5),
    ANIMCMD_FRAME(4, 5),
    ANIMCMD_FRAME(6, 5),
    ANIMCMD_FRAME(8, 5),
    ANIMCMD_FRAME(10, 5),
    ANIMCMD_FRAME(12, 5),
    ANIMCMD_FRAME(14, 5),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sPowderParticlesAnimTable[] =
{
    sPowderParticlesAnimCmds,
};

const struct SpriteTemplate gSleepPowderParticleSpriteTemplate =
{
    .tileTag = ANIM_TAG_SLEEP_POWDER,
    .paletteTag = ANIM_TAG_SLEEP_POWDER,
    .oam = &gOamData_AffineOff_ObjNormal_8x16,
    .anims = sPowderParticlesAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimMovePowderParticle,
};

const struct SpriteTemplate gStunSporeParticleSpriteTemplate =
{
    .tileTag = ANIM_TAG_STUN_SPORE,
    .paletteTag = ANIM_TAG_STUN_SPORE,
    .oam = &gOamData_AffineOff_ObjNormal_8x16,
    .anims = sPowderParticlesAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimMovePowderParticle,
};

const struct SpriteTemplate gPoisonPowderParticleSpriteTemplate =
{
    .tileTag = ANIM_TAG_POISON_POWDER,
    .paletteTag = ANIM_TAG_POISON_POWDER,
    .oam = &gOamData_AffineOff_ObjNormal_8x16,
    .anims = sPowderParticlesAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimMovePowderParticle,
};

static const union AnimCmd sSolarbeamBigOrbAnimCmds1[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

static const union AnimCmd sSolarbeamBigOrbAnimCmds2[] =
{
    ANIMCMD_FRAME(1, 1),
    ANIMCMD_END,
};

static const union AnimCmd sSolarbeamBigOrbAnimCmds3[] =
{
    ANIMCMD_FRAME(2, 1),
    ANIMCMD_END,
};

static const union AnimCmd sSolarbeamBigOrbAnimCmds4[] =
{
    ANIMCMD_FRAME(3, 1),
    ANIMCMD_END,
};

static const union AnimCmd sSolarbeamBigOrbAnimCmds5[] =
{
    ANIMCMD_FRAME(4, 1),
    ANIMCMD_END,
};

static const union AnimCmd sSolarbeamBigOrbAnimCmds6[] =
{
    ANIMCMD_FRAME(5, 1),
    ANIMCMD_END,
};

static const union AnimCmd sSolarbeamBigOrbAnimCmds7[] =
{
    ANIMCMD_FRAME(6, 1),
    ANIMCMD_END,
};

static const union AnimCmd sSolarbeamSmallOrbAnimCms[] =
{
    ANIMCMD_FRAME(7, 1),
    ANIMCMD_END,
};

static const union AnimCmd sPowerAbsorptionOrbAnimCmds[] =
{
    ANIMCMD_FRAME(8, 1),
    ANIMCMD_END,
};

static const union AnimCmd *const sSolarbeamBigOrbAnimTable[] =
{
    sSolarbeamBigOrbAnimCmds1,
    sSolarbeamBigOrbAnimCmds2,
    sSolarbeamBigOrbAnimCmds3,
    sSolarbeamBigOrbAnimCmds4,
    sSolarbeamBigOrbAnimCmds5,
    sSolarbeamBigOrbAnimCmds6,
    sSolarbeamBigOrbAnimCmds7,
};

static const union AnimCmd *const sSolarbeamSmallOrbAnimTable[] =
{
    sSolarbeamSmallOrbAnimCms,
};

static const union AnimCmd *const sPowerAbsorptionOrbAnimTable[] =
{
    sPowerAbsorptionOrbAnimCmds,
};

static const union AffineAnimCmd sPowerAbsorptionOrbAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(-5, -5, 0, 1),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd *const sPowerAbsorptionOrbAffineAnimTable[] =
{
    sPowerAbsorptionOrbAffineAnimCmds,
};

const struct SpriteTemplate gPowerAbsorptionOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_ORBS,
    .paletteTag = ANIM_TAG_ORBS,
    .oam = &gOamData_AffineNormal_ObjBlend_16x16,
    .anims = sPowerAbsorptionOrbAnimTable,
    .images = NULL,
    .affineAnims = sPowerAbsorptionOrbAffineAnimTable,
    .callback = AnimPowerAbsorptionOrb,
};

const struct SpriteTemplate gSolarbeamBigOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_ORBS,
    .paletteTag = ANIM_TAG_ORBS,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = sSolarbeamBigOrbAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSolarbeamBigOrb,
};

const struct SpriteTemplate gSolarbeamSmallOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_ORBS,
    .paletteTag = ANIM_TAG_ORBS,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = sSolarbeamSmallOrbAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSolarbeamSmallOrb,
};

static const union AffineAnimCmd sStockpileAbsorptionOrbAffineCmds[] =
{
    AFFINEANIMCMD_FRAME(320, 320, 0, 0),
    AFFINEANIMCMD_FRAME(-14, -14, 0, 1),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd *const sStockpileAbsorptionOrbAffineAnimTable[] =
{
    sStockpileAbsorptionOrbAffineCmds,
};

const struct SpriteTemplate gStockpileAbsorptionOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_GRAY_ORB,
    .paletteTag = ANIM_TAG_GRAY_ORB,
    .oam = &gOamData_AffineDouble_ObjNormal_8x8,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sStockpileAbsorptionOrbAffineAnimTable,
    .callback = AnimPowerAbsorptionOrb,
};

static const union AffineAnimCmd sAbsorptionOrbAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(-5, -5, 0, 1),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd *const sAbsorptionOrbAffineAnimTable[] =
{
    sAbsorptionOrbAffineAnimCmds,
};

const struct SpriteTemplate gAbsorptionOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_ORBS,
    .paletteTag = ANIM_TAG_ORBS,
    .oam = &gOamData_AffineNormal_ObjBlend_16x16,
    .anims = sPowerAbsorptionOrbAnimTable,
    .images = NULL,
    .affineAnims = sAbsorptionOrbAffineAnimTable,
    .callback = AnimAbsorptionOrb,
};

const struct SpriteTemplate gHyperBeamOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_ORBS,
    .paletteTag = ANIM_TAG_ORBS,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = sSolarbeamBigOrbAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimHyperBeamOrb,
};

static const union AnimCmd sLeechSeedAnimCmds1[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

static const union AnimCmd sLeechSeedAnimCmds2[] =
{
    ANIMCMD_FRAME(4, 7),
    ANIMCMD_FRAME(8, 7),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sLeechSeedAnimTable[] =
{
    sLeechSeedAnimCmds1,
    sLeechSeedAnimCmds2,
};

const struct SpriteTemplate gLeechSeedSpriteTemplate =
{
    .tileTag = ANIM_TAG_SEED,
    .paletteTag = ANIM_TAG_SEED,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = sLeechSeedAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimLeechSeed,
};

static const union AffineAnimCmd sSwiftStarAffineAnimCmds[] = 
{
    AFFINEANIMCMD_FRAME(0, 0, 0, 1),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd *const sSwiftStarAffineAnimTable[] = 
{
    sSwiftStarAffineAnimCmds,
};

const struct SpriteTemplate gSwiftStarSpriteTemplate =
{
    .tileTag = ANIM_TAG_YELLOW_STAR,
    .paletteTag = ANIM_TAG_YELLOW_STAR,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sSwiftStarAffineAnimTable,
    .callback = AnimTranslateLinearSingleSineWave,
};

static const union AnimCmd sConstrictBindingAnimCmds1[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(32, 4),
    ANIMCMD_FRAME(64, 4),
    ANIMCMD_FRAME(96, 4),
    ANIMCMD_END,
};

static const union AnimCmd sConstrictBindingAnimCmds2[] =
{
    ANIMCMD_FRAME(0,  4, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(64, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(96, 4, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd *const sConstrictBindingAnimTable[] =
{
    sConstrictBindingAnimCmds1,
    sConstrictBindingAnimCmds2,
};

static const union AffineAnimCmd sConstrictBindingAffineAnimCmds1[] = 
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME(-11, 0, 0, 6),
    AFFINEANIMCMD_FRAME(11, 0, 0, 6),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sConstrictBindingAffineAnimCmds2[] = 
{
    AFFINEANIMCMD_FRAME(-0x100, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME(11, 0, 0, 6),
    AFFINEANIMCMD_FRAME(-11, 0, 0, 6),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sConstrictBindingAffineAnimTable[] = 
{
    sConstrictBindingAffineAnimCmds1,
    sConstrictBindingAffineAnimCmds2,
};

const struct SpriteTemplate gConstrictBindingSpriteTemplate =
{
    .tileTag = ANIM_TAG_TENDRILS,
    .paletteTag = ANIM_TAG_TENDRILS,
    .oam = &gOamData_AffineNormal_ObjNormal_64x32,
    .anims = sConstrictBindingAnimTable,
    .images = NULL,
    .affineAnims = sConstrictBindingAffineAnimTable,
    .callback = AnimConstrictBinding,
};

static const union AffineAnimCmd sMimicOrbAffineAnimCmds1[] = 
{
    AFFINEANIMCMD_FRAME(0, 0, 0, 0),
    AFFINEANIMCMD_FRAME(48, 48, 0, 14),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sMimicOrbAffineAnimCmds2[] = 
{
    AFFINEANIMCMD_FRAME(-16, -16, 0, 1),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd *const sMimicOrbAffineAnimTable[] = 
{
    sMimicOrbAffineAnimCmds1,
    sMimicOrbAffineAnimCmds2,
};

const struct SpriteTemplate gMimicOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_ORBS,
    .paletteTag = ANIM_TAG_ORBS,
    .oam = &gOamData_AffineDouble_ObjNormal_16x16,
    .anims = sPowerAbsorptionOrbAnimTable,
    .images = NULL,
    .affineAnims = sMimicOrbAffineAnimTable,
    .callback = AnimMimicOrb,
};

static const union AnimCmd sFallingBagAnimCmds[] =
{
    ANIMCMD_FRAME(0, 30),
    ANIMCMD_END,
};

static const union AnimCmd *const sFallingBagAnimTable[] =
{
    sFallingBagAnimCmds,
};

static const union AffineAnimCmd sFallingBagAffineAnimCmds1[] = 
{
    AFFINEANIMCMD_FRAME(0, 0, -4, 10),
    AFFINEANIMCMD_FRAME(0, 0, 4, 20),
    AFFINEANIMCMD_FRAME(0, 0, -4, 10),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sFallingBagAffineAnimCmds2[] = 
{
    AFFINEANIMCMD_FRAME(0, 0, -1, 2),
    AFFINEANIMCMD_FRAME(0, 0, 1, 4),
    AFFINEANIMCMD_FRAME(0, 0, -1, 4),
    AFFINEANIMCMD_FRAME(0, 0, 1, 4),
    AFFINEANIMCMD_FRAME(0, 0, -1, 4),
    AFFINEANIMCMD_FRAME(0, 0, 1, 2),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sFallingBagAffineAnimTable[] = 
{
    sFallingBagAffineAnimCmds1,
    sFallingBagAffineAnimCmds2,
};

const struct SpriteTemplate gPresentSpriteTemplate =
{
    .tileTag = ANIM_TAG_ITEM_BAG_2,
    .paletteTag = ANIM_TAG_ITEM_BAG_2,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = sFallingBagAnimTable,
    .images = NULL,
    .affineAnims = sFallingBagAffineAnimTable,
    .callback = AnimPresent,
};

const struct SpriteTemplate gKnockOffItemSpriteTemplate =
{
    .tileTag = ANIM_TAG_ITEM_BAG,
    .paletteTag = ANIM_TAG_ITEM_BAG,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = sFallingBagAnimTable,
    .images = NULL,
    .affineAnims = sFallingBagAffineAnimTable,
    .callback = AnimKnockOffItem,
};

static const union AnimCmd sPresentHealParticleAnimCmds[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(4, 4),
    ANIMCMD_FRAME(8, 4),
    ANIMCMD_FRAME(12, 4),
    ANIMCMD_END,
};

static const union AnimCmd *const sPresentHealParticleAnimTable[] =
{
    sPresentHealParticleAnimCmds,
};

const struct SpriteTemplate gPresentHealParticleSpriteTemplate =
{
    .tileTag = ANIM_TAG_GREEN_SPARKLE,
    .paletteTag = ANIM_TAG_GREEN_SPARKLE,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = sPresentHealParticleAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimPresentHealParticle,
};

const struct SpriteTemplate gItemStealSpriteTemplate =
{
    .tileTag = ANIM_TAG_ITEM_BAG,
    .paletteTag = ANIM_TAG_ITEM_BAG,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = sFallingBagAnimTable,
    .images = NULL,
    .affineAnims = sFallingBagAffineAnimTable,
    .callback = AnimItemSteal,
};

static const union AffineAnimCmd sTrickBagAffineAnimCmds1[] = 
{
    AFFINEANIMCMD_FRAME(0, 0, 0, 3),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sTrickBagAffineAnimCmds2[] = 
{
    AFFINEANIMCMD_FRAME(0, -10, 0, 3),
    AFFINEANIMCMD_FRAME(0, -6, 0, 3),
    AFFINEANIMCMD_FRAME(0, -2, 0, 3),
    AFFINEANIMCMD_FRAME(0, 0, 0, 3),
    AFFINEANIMCMD_FRAME(0, 2, 0, 3),
    AFFINEANIMCMD_FRAME(0, 6, 0, 3),
    AFFINEANIMCMD_FRAME(0, 10, 0, 3),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sTrickBagAffineAnimTable[] = 
{
    sTrickBagAffineAnimCmds1,
    sTrickBagAffineAnimCmds2,
    sFallingBagAffineAnimCmds1,
    sFallingBagAffineAnimCmds2,
};

const struct SpriteTemplate gTrickBagSpriteTemplate =
{
    .tileTag = ANIM_TAG_ITEM_BAG_2,
    .paletteTag = ANIM_TAG_ITEM_BAG_2,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = sFallingBagAnimTable,
    .images = NULL,
    .affineAnims = sTrickBagAffineAnimTable,
    .callback = AnimTrickBag,
};

static const s8 gTrickBagCoordinates[][3] =
{
    {5, 24,   1},
    {0,  4,   0},
    {8, 16,  -1},
    {0,  2,   0},
    {8, 16,   1},
    {0,  2,   0},
    {8, 16,   1},
    {0,  2,   0},
    {8, 16,   1},
    {0, 16,   0},
    {0,  0, 127},
};

static const union AnimCmd sWhipAnimCmds1[] =    
{
    ANIMCMD_FRAME(64, 3),
    ANIMCMD_FRAME(80, 3),
    ANIMCMD_FRAME(96, 3),
    ANIMCMD_FRAME(112, 6),
    ANIMCMD_END,
};

static const union AnimCmd sWhipAnimCmds2[] =    
{
    ANIMCMD_FRAME(64, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(80, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(96, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(112, 6, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd *const sWhipAnimTable[] =    
{
    sWhipAnimCmds1,
    sWhipAnimCmds2,
};

const struct SpriteTemplate gSlamHitSpriteTemplate =
{
    .tileTag = ANIM_TAG_SLAM_HIT,
    .paletteTag = ANIM_TAG_SLAM_HIT,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sWhipAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimWhipHit,
};

const struct SpriteTemplate gVineWhipSpriteTemplate =
{
    .tileTag = ANIM_TAG_WHIP_HIT,
    .paletteTag = ANIM_TAG_WHIP_HIT,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sWhipAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimWhipHit,
};

static const union AnimCmd sCuttingSliceAnimCmds[] =
{
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_FRAME(16, 5),
    ANIMCMD_FRAME(32, 5),
    ANIMCMD_FRAME(48, 5),
    ANIMCMD_END,
};

static const union AnimCmd *const sCuttingSliceAnimTable[] =
{
    sCuttingSliceAnimCmds,
};

const struct SpriteTemplate gCuttingSliceSpriteTemplate =
{
    .tileTag = ANIM_TAG_CUT,
    .paletteTag = ANIM_TAG_CUT,
    .oam = &gOamData_AffineOff_ObjBlend_32x32,
    .anims = sCuttingSliceAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimCuttingSlice,
};

const struct SpriteTemplate gProtectWallSpriteTemplate =
{
    .tileTag = ANIM_TAG_PROTECT,
    .paletteTag = ANIM_TAG_PROTECT,
    .oam = &gOamData_AffineOff_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimProtect,
};

static const union AffineAnimCmd sMilkBottleAffineAnimCmds1[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sMilkBottleAffineAnimCmds2[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, 2, 12),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 0, 6),
    AFFINEANIMCMD_FRAME(0x0, 0x0, -2, 24),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 0, 6),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 2, 12),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd *const sMilkBottleAffineAnimTable[] =
{
    sMilkBottleAffineAnimCmds1,
    sMilkBottleAffineAnimCmds2,
};

const struct SpriteTemplate gMilkBottleSpriteTemplate =
{
    .tileTag = ANIM_TAG_MILK_BOTTLE,
    .paletteTag = ANIM_TAG_MILK_BOTTLE,
    .oam = &gOamData_AffineNormal_ObjBlend_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sMilkBottleAffineAnimTable,
    .callback = AnimMilkBottle,
};

static const union AnimCmd sGrantingStarsAnimCmds[] =
{
    ANIMCMD_FRAME(0, 7),
    ANIMCMD_FRAME(16, 7),
    ANIMCMD_FRAME(32, 7),
    ANIMCMD_FRAME(48, 7),
    ANIMCMD_FRAME(64, 7),
    ANIMCMD_FRAME(80, 7),
    ANIMCMD_FRAME(96, 7),
    ANIMCMD_FRAME(112, 7),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sGrantingStarsAnimTable[] =
{
    sGrantingStarsAnimCmds,
};

const struct SpriteTemplate gGrantingStarsSpriteTemplate =    
{
    .tileTag = ANIM_TAG_SPARKLE_2,
    .paletteTag = ANIM_TAG_SPARKLE_2,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sGrantingStarsAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimGrantingStars,
};

const struct SpriteTemplate gSparklingStarsSpriteTemplate =    
{
    .tileTag = ANIM_TAG_SPARKLE_2,
    .paletteTag = ANIM_TAG_SPARKLE_2,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sGrantingStarsAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSparkingStars,
};

static const union AnimCmd sSleepLetterZAnimCmds[] =
{
    ANIMCMD_FRAME(0, 40),
    ANIMCMD_END,
};

static const union AnimCmd *const sSleepLetterZAnimTable[] =
{
    sSleepLetterZAnimCmds,
};

static const union AffineAnimCmd sSleepLetterZAffineAnimCmds1[] =
{
    AFFINEANIMCMD_FRAME(0x14, 0x14, -30, 0),
    AFFINEANIMCMD_FRAME(0x8, 0x8, 1, 24),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSleepLetterZAffineAnimCmds1_2[] =
{
    AFFINEANIMCMD_LOOP(0),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 1, 24),
    AFFINEANIMCMD_LOOP(10),
};

static const union AffineAnimCmd sSleepLetterZAffineAnimCmds2[] =
{
    AFFINEANIMCMD_FRAME(0x14, 0x14, 30, 0),
    AFFINEANIMCMD_FRAME(0x8, 0x8, -1, 24),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSleepLetterZAffineAnimCmds2_2[] =
{
    AFFINEANIMCMD_LOOP(0),
    AFFINEANIMCMD_FRAME(0x0, 0x0, -1, 24),
    AFFINEANIMCMD_LOOP(10),
};

static const union AffineAnimCmd *const sSleepLetterZAffineAnimTable[] =
{
    sSleepLetterZAffineAnimCmds1,
    sSleepLetterZAffineAnimCmds2,
};

const struct SpriteTemplate gSleepLetterZSpriteTemplate =
{
    .tileTag = ANIM_TAG_LETTER_Z,
    .paletteTag = ANIM_TAG_LETTER_Z,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = sSleepLetterZAnimTable,
    .images = NULL,
    .affineAnims = sSleepLetterZAffineAnimTable,
    .callback = AnimSleepLetterZ,
};

const struct SpriteTemplate gLockOnTargetSpriteTemplate =
{
    .tileTag = ANIM_TAG_LOCK_ON,
    .paletteTag = ANIM_TAG_LOCK_ON,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimLockOnTarget,
};

const struct SpriteTemplate gLockOnMoveTargetSpriteTemplate =
{
    .tileTag = ANIM_TAG_LOCK_ON,
    .paletteTag = ANIM_TAG_LOCK_ON,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimLockOnMoveTarget,
};

static const s8 sInclineMonCoordTable[][2] =
{
    { 64,  64},
    {  0, -64},
    {-64,  64},
    { 32, -32},
};

const struct SpriteTemplate gBowMonSpriteTemplate =    
{
    .tileTag = 0,
    .paletteTag = 0,
    .oam = &gDummyOamData,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimBowMon,
};

static const union AnimCmd sSlashSliceAnimCmds1[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(16, 4),
    ANIMCMD_FRAME(32, 4),
    ANIMCMD_FRAME(48, 4),
    ANIMCMD_END,
};

static const union AnimCmd sSlashSliceAnimCmds2[] =
{
    ANIMCMD_FRAME(48, 4),
    ANIMCMD_END,
};

static const union AnimCmd *const sSlashSliceAnimTable[] =
{
    sSlashSliceAnimCmds1,
    sSlashSliceAnimCmds2,
};

const struct SpriteTemplate gSlashSliceSpriteTemplate =    
{
    .tileTag = ANIM_TAG_SLASH,
    .paletteTag = ANIM_TAG_SLASH,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sSlashSliceAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSlashSlice,
};

const struct SpriteTemplate gFalseSwipeSliceSpriteTemplate =
{
    .tileTag = ANIM_TAG_SLASH_2,
    .paletteTag = ANIM_TAG_SLASH_2,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sSlashSliceAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimFalseSwipeSlice,
};

const struct SpriteTemplate gFalseSwipePositionedSliceSpriteTemplate =
{
    .tileTag = ANIM_TAG_SLASH_2,
    .paletteTag = ANIM_TAG_SLASH_2,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sSlashSliceAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimFalseSwipePositionedSlice,
};

static const union AnimCmd sEndureEnergyAnimCmds[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(8, 12),
    ANIMCMD_FRAME(16, 4),
    ANIMCMD_FRAME(24, 4),
    ANIMCMD_END,
};

const union AnimCmd *const gEndureEnergyAnimTable[] =
{
    sEndureEnergyAnimCmds,
};

const struct SpriteTemplate gEndureEnergySpriteTemplate =
{
    .tileTag = ANIM_TAG_FOCUS_ENERGY,
    .paletteTag = ANIM_TAG_FOCUS_ENERGY,
    .oam = &gOamData_AffineOff_ObjNormal_16x32,
    .anims = gEndureEnergyAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimEndureEnergy,
};

const struct SpriteTemplate gEndureBlueEnergySpriteTemplate =
{
    .tileTag = ANIM_TAG_FOCUS_ENERGY,
    .paletteTag = ANIM_TAG_SWEAT_BEAD,
    .oam = &gOamData_AffineOff_ObjNormal_16x32,
    .anims = gEndureEnergyAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimEndureEnergy,
};

const struct SpriteTemplate gEndureGreenEnergySpriteTemplate =
{
    .tileTag = ANIM_TAG_FOCUS_ENERGY,
    .paletteTag = ANIM_TAG_WHIP_HIT,
    .oam = &gOamData_AffineOff_ObjNormal_16x32,
    .anims = gEndureEnergyAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimEndureEnergy,
};

const struct SpriteTemplate gEndureYellowEnergySpriteTemplate =
{
    .tileTag = ANIM_TAG_FOCUS_ENERGY,
    .paletteTag = ANIM_TAG_PAW_PRINT,
    .oam = &gOamData_AffineOff_ObjNormal_16x32,
    .anims = gEndureEnergyAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimEndureEnergy,
};

static const union AnimCmd sSharpenSphereAnimCmds[] =
{
    ANIMCMD_FRAME(0, 18),
    ANIMCMD_FRAME(0, 6),
    ANIMCMD_FRAME(16, 18),
    ANIMCMD_FRAME(0, 6),
    ANIMCMD_FRAME(16, 6),
    ANIMCMD_FRAME(32, 18),
    ANIMCMD_FRAME(16, 6),
    ANIMCMD_FRAME(32, 6),
    ANIMCMD_FRAME(48, 18),
    ANIMCMD_FRAME(32, 6),
    ANIMCMD_FRAME(48, 6),
    ANIMCMD_FRAME(64, 18),
    ANIMCMD_FRAME(48, 6),
    ANIMCMD_FRAME(64, 54),
    ANIMCMD_END,
};

static const union AnimCmd *const sSharpenSphereAnimTable[] =
{
    sSharpenSphereAnimCmds,
};

const struct SpriteTemplate gSharpenSphereSpriteTemplate =
{
    .tileTag = ANIM_TAG_SPHERE_TO_CUBE,
    .paletteTag = ANIM_TAG_SPHERE_TO_CUBE,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sSharpenSphereAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSharpenSphere,
};

static const union AnimCmd sConversionAnimCmds[] =
{
    ANIMCMD_FRAME(3, 5),
    ANIMCMD_FRAME(2, 5),
    ANIMCMD_FRAME(1, 5),
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_END,
};

static const union AnimCmd *const sConversionAnimTable[] =
{
    sConversionAnimCmds,
};

static const union AffineAnimCmd sConversionAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(0x200, 0x200, 0, 0),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sConversionAffineAnimTable[] =
{
    sConversionAffineAnimCmds,
};

const struct SpriteTemplate gConversionSpriteTemplate =
{
    .tileTag = ANIM_TAG_CONVERSION,
    .paletteTag = ANIM_TAG_CONVERSION,
    .oam = &gOamData_AffineDouble_ObjBlend_8x8,
    .anims = sConversionAnimTable,
    .images = NULL,
    .affineAnims = sConversionAffineAnimTable,
    .callback = AnimConversion,
};

static const union AnimCmd sConversion2AnimCmds[] =
{
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_FRAME(1, 5),
    ANIMCMD_FRAME(2, 5),
    ANIMCMD_FRAME(3, 5),
    ANIMCMD_END,
};

static const union AnimCmd *const sConversion2AnimTable[] =
{
    sConversion2AnimCmds,
};

const struct SpriteTemplate gConversion2SpriteTemplate =
{
    .tileTag = ANIM_TAG_CONVERSION,
    .paletteTag = ANIM_TAG_CONVERSION,
    .oam = &gOamData_AffineDouble_ObjBlend_8x8,
    .anims = sConversion2AnimTable,
    .images = NULL,
    .affineAnims = sConversionAffineAnimTable,
    .callback = AnimConversion2,
};

static const union AnimCmd sHealingBlueStarAnimCmds[] =
{
    ANIMCMD_FRAME(0, 2),
    ANIMCMD_FRAME(16, 2),
    ANIMCMD_FRAME(32, 2),
    ANIMCMD_FRAME(48, 3),
    ANIMCMD_FRAME(64, 5),
    ANIMCMD_FRAME(80, 3),
    ANIMCMD_FRAME(96, 2),
    ANIMCMD_FRAME(0, 2),
    ANIMCMD_END,
};

static const union AnimCmd *const sHealingBlueStarAnimTable[] =
{
    sHealingBlueStarAnimCmds,
};

const struct SpriteTemplate gHealingBlueStarSpriteTemplate =
{
    .tileTag = ANIM_TAG_BLUE_STAR,
    .paletteTag = ANIM_TAG_BLUE_STAR,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sHealingBlueStarAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSpriteOnMonPos,
};

const struct SpriteTemplate gHornHitSpriteTemplate =
{
    .tileTag = ANIM_TAG_HORN_HIT,
    .paletteTag = ANIM_TAG_HORN_HIT,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimHornHit,
};

static const union AnimCmd sSuperFangAnimCmds[] =
{
    ANIMCMD_FRAME(0, 2),
    ANIMCMD_FRAME(16, 2),
    ANIMCMD_FRAME(32, 2),
    ANIMCMD_FRAME(48, 2),
    ANIMCMD_END,
};

static const union AnimCmd *const sSuperFangAnimTable[] =
{
    sSuperFangAnimCmds,
};

const struct SpriteTemplate gSuperFangSpriteTemplate =
{
    .tileTag = ANIM_TAG_FANG_ATTACK,
    .paletteTag = ANIM_TAG_FANG_ATTACK,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sSuperFangAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = DestroyAnimSpriteWhenAnimEnds,
};

static const union AnimCmd sWavyMusicNotesAnimCmds1[] =
{
    ANIMCMD_FRAME(0, 10),
    ANIMCMD_END,
};

static const union AnimCmd sWavyMusicNotesAnimCmds2[] =
{
    ANIMCMD_FRAME(4, 10),
    ANIMCMD_END,
};

static const union AnimCmd sWavyMusicNotesAnimCmds3[] =
{
    ANIMCMD_FRAME(8, 41),
    ANIMCMD_END,
};

static const union AnimCmd sWavyMusicNotesAnimCmds4[] =
{
    ANIMCMD_FRAME(12, 10),
    ANIMCMD_END,
};

static const union AnimCmd sWavyMusicNotesAnimCmds5[] =
{
    ANIMCMD_FRAME(16, 10),
    ANIMCMD_END,
};

static const union AnimCmd sWavyMusicNotesAnimCmds6[] =
{
    ANIMCMD_FRAME(20, 10),
    ANIMCMD_END,
};

static const union AnimCmd sWavyMusicNotesAnimCmds7[] =
{
    ANIMCMD_FRAME(0, 10, .vFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd sWavyMusicNotesAnimCmds8[] =
{
    ANIMCMD_FRAME(4, 10, .vFlip = TRUE),
    ANIMCMD_END,
};

const union AnimCmd *const gMusicNotesAnimTable[] =    
{
    sWavyMusicNotesAnimCmds1,
    sWavyMusicNotesAnimCmds2,
    sWavyMusicNotesAnimCmds3,
    sWavyMusicNotesAnimCmds4,
    sWavyMusicNotesAnimCmds5,
    sWavyMusicNotesAnimCmds6,
    sWavyMusicNotesAnimCmds7,
    sWavyMusicNotesAnimCmds8,
};

static const union AffineAnimCmd sWavyMusicNotesAffineAnimCmds[] =    
{
    AFFINEANIMCMD_FRAME(0xC, 0xC, 0, 16),
    AFFINEANIMCMD_FRAME(0xFFF4, 0xFFF4, 0, 16),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd *const sMusicNotesAffineAnimTable[] =    
{
    sWavyMusicNotesAffineAnimCmds,
};

const struct SpriteTemplate gWavyMusicNotesSpriteTemplate =    
{
    .tileTag = ANIM_TAG_MUSIC_NOTES,
    .paletteTag = ANIM_TAG_MUSIC_NOTES,
    .oam = &gOamData_AffineDouble_ObjNormal_16x16,
    .anims = gMusicNotesAnimTable,
    .images = NULL,
    .affineAnims = sMusicNotesAffineAnimTable,
    .callback = AnimWavyMusicNotes,
};

static const u16 sParticlesColorBlendTable[][6] =    
{
    {ANIM_TAG_MUSIC_NOTES,     RGB_WHITE, RGB(31, 26, 28), RGB(31, 22, 26), RGB(31, 17, 24), RGB(31, 13, 22)},
    {ANIM_TAG_BENT_SPOON,      RGB_WHITE, RGB(25, 31, 26), RGB(20, 31, 21), RGB(15, 31, 16), RGB(10, 31, 12)},
    {ANIM_TAG_SPHERE_TO_CUBE,  RGB_WHITE, RGB(31, 31, 24), RGB(31, 31, 17), RGB(31, 31, 10), RGB(31, 31, 3)},
    {ANIM_TAG_LARGE_FRESH_EGG, RGB_WHITE, RGB(26, 28, 31), RGB(21, 26, 31), RGB(16, 24, 31), RGB(12, 22, 31)},
};

const struct SpriteTemplate gFastFlyingMusicNotesSpriteTemplate =
{
    .tileTag = ANIM_TAG_MUSIC_NOTES,
    .paletteTag = ANIM_TAG_MUSIC_NOTES,
    .oam = &gOamData_AffineDouble_ObjNormal_16x16,
    .anims = gMusicNotesAnimTable,
    .images = NULL,
    .affineAnims = sMusicNotesAffineAnimTable,
    .callback = AnimFlyingMusicNotes,
};

const struct SpriteTemplate gBellyDrumHandSpriteTemplate =
{
    .tileTag = ANIM_TAG_PURPLE_HAND_OUTLINE,
    .paletteTag = ANIM_TAG_PURPLE_HAND_OUTLINE,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimBellyDrumHand,
};

static const union AffineAnimCmd sSlowFlyingMusicNotesAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(0xA0, 0xA0, 0, 0),
    AFFINEANIMCMD_FRAME(0x4, 0x4, 0, 1),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd *const sSlowFlyinsMusicNotesAffineAnimTable[] =
{
    sSlowFlyingMusicNotesAffineAnimCmds,
};

const struct SpriteTemplate gSlowFlyingMusicNotesSpriteTemplate =
{
    .tileTag = ANIM_TAG_MUSIC_NOTES,
    .paletteTag = ANIM_TAG_MUSIC_NOTES,
    .oam = &gOamData_AffineDouble_ObjNormal_16x16,
    .anims = gMusicNotesAnimTable,
    .images = NULL,
    .affineAnims = sSlowFlyinsMusicNotesAffineAnimTable,
    .callback = AnimSlowFlyingMusicNotes,
};

static const union AnimCmd sMetronomeThroughtBubbleAnimCmds1[] =
{
    ANIMCMD_FRAME(0, 2, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 2, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 2, .hFlip = TRUE),
    ANIMCMD_FRAME(48, 2, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd sMetronomeThroughtBubbleAnimCmds3[] =
{
    ANIMCMD_FRAME(48, 2, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 2, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 2, .hFlip = TRUE),
    ANIMCMD_FRAME(0, 2, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd sMetronomeThroughtBubbleAnimCmds2[] =
{
    ANIMCMD_FRAME(0, 2),
    ANIMCMD_FRAME(16, 2),
    ANIMCMD_FRAME(32, 2),
    ANIMCMD_FRAME(48, 2),
    ANIMCMD_END,
};

static const union AnimCmd sMetronomeThroughtBubbleAnimCmds4[] =
{
    ANIMCMD_FRAME(48, 2),
    ANIMCMD_FRAME(32, 2),
    ANIMCMD_FRAME(16, 2),
    ANIMCMD_FRAME(0, 2),
    ANIMCMD_END,
};

static const union AnimCmd *const sMetronomeThroughtBubbleAnimTable[] =
{
    sMetronomeThroughtBubbleAnimCmds1,
    sMetronomeThroughtBubbleAnimCmds2,
    sMetronomeThroughtBubbleAnimCmds3,
    sMetronomeThroughtBubbleAnimCmds4,
};

const struct SpriteTemplate gThoughtBubbleSpriteTemplate =
{
    .tileTag = ANIM_TAG_THOUGHT_BUBBLE,
    .paletteTag = ANIM_TAG_THOUGHT_BUBBLE,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sMetronomeThroughtBubbleAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimThoughtBubble,
};

static const union AffineAnimCmd sMetronomeFingerAffineAnimCmds1[] =
{
    AFFINEANIMCMD_FRAME(0x10, 0x10, 0, 0),
    AFFINEANIMCMD_FRAME(0x1E, 0x1E, 0, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sMetronomeFingerAffineAnimCmds2[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, 4, 11),
    AFFINEANIMCMD_FRAME(0x0, 0x0, -4, 11),
    AFFINEANIMCMD_LOOP(2),
    AFFINEANIMCMD_FRAME(-0x1E, -0x1E, 0, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sMetronomeFingerAffineAnimCmds2_2[] =
{
    AFFINEANIMCMD_FRAME(16, 16, 0, 0),
    AFFINEANIMCMD_FRAME(30, 30, 0, 8),
    AFFINEANIMCMD_FRAME(0, 0, 0, 16),
    AFFINEANIMCMD_LOOP(0),
    AFFINEANIMCMD_FRAME(0, 0, 4, 11),
    AFFINEANIMCMD_FRAME(0, 0, -4, 11),
    AFFINEANIMCMD_LOOP(2),
    AFFINEANIMCMD_FRAME(-30, -30, 0, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sMetronomeFingerAffineAnimTable[] =
{
    sMetronomeFingerAffineAnimCmds1,
    sMetronomeFingerAffineAnimCmds2,
};

const struct SpriteTemplate gMetronomeFingerSpriteTemplate =
{
    .tileTag = ANIM_TAG_FINGER,
    .paletteTag = ANIM_TAG_FINGER,
    .oam = &gOamData_AffineDouble_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sMetronomeFingerAffineAnimTable,
    .callback = AnimMetronomeFinger,
};

const struct SpriteTemplate gFollowMeFingerSpriteTemplate =
{
    .tileTag = ANIM_TAG_FINGER,
    .paletteTag = ANIM_TAG_FINGER,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sMetronomeFingerAffineAnimTable,
    .callback = AnimFollowMeFinger,
};

const struct SpriteTemplate gPaybackAbsorptionOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_ORBS,
    .paletteTag = ANIM_TAG_POISON_JAB,
    .oam = &gOamData_AffineNormal_ObjBlend_16x16,
    .anims = sPowerAbsorptionOrbAnimTable,
    .images = NULL,
    .affineAnims = sPowerAbsorptionOrbAffineAnimTable,
    .callback = AnimPowerAbsorptionOrb,
};

const struct SpriteTemplate gLuckyChantPinkStarsSpriteTemplate =
{
    .tileTag = ANIM_TAG_SPARKLE_2,
    .paletteTag = ANIM_TAG_PINK_PETAL,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sGrantingStarsAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimPetalDanceFlower,
};

const struct SpriteTemplate gMegaEvolutionStoneSpriteTemplate =
{
    .tileTag = ANIM_TAG_MEGA_EVOLUTION_STONE,
    .paletteTag = ANIM_TAG_MEGA_EVOLUTION_STONE,
    .oam = &gOamData_AffineDouble_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gAffineAnims_LusterPurgeCircle,
    .callback = AnimSpriteOnMonPos,
};

const struct SpriteTemplate gMegaEvolutionOrbsSpriteTemplate =
{
    .tileTag = ANIM_TAG_MEGA_EVOLUTION_ORBS,
    .paletteTag = ANIM_TAG_MEGA_EVOLUTION_ORBS,
    .oam = &gOamData_AffineNormal_ObjBlend_8x8,
    .anims = sSolarbeamBigOrbAnimTable,
    .images = NULL,
    .affineAnims = sPowerAbsorptionOrbAffineAnimTable,
    .callback = AnimSolarbeamBigOrb,
};

static const union AffineAnimCmd sMegaEvolutionSymbolAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(16, 16, 0, 0), // Start small
	AFFINEANIMCMD_FRAME(32, 32, 0, 15), // Grow sprite
	AFFINEANIMCMD_FRAME(0, 0, 0, 2), // Pause for 2 frames
	// Pulsate sprite
	AFFINEANIMCMD_LOOP(0),
	AFFINEANIMCMD_FRAME(-16, -16, 0, 4),
	AFFINEANIMCMD_FRAME(16, 16, 0, 4),
	AFFINEANIMCMD_LOOP(2), // Repeat pulse 2 more times
	AFFINEANIMCMD_FRAME(-32, -32, 0, 15), // Shrink down again
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sMegaEvolutionSymbolAffineAnimTable[] =
{
    sMegaEvolutionSymbolAffineAnimCmds,
};

const struct SpriteTemplate gMegaEvolutionSymbolSpriteTemplate =
{
    .tileTag = ANIM_TAG_MEGA_SYMBOL,
    .paletteTag = ANIM_TAG_MEGA_SYMBOL,
    .oam = &gOamData_AffineDouble_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sMegaEvolutionSymbolAffineAnimTable,
    .callback = AnimSpriteOnMonPos,
};

const struct SpriteTemplate gPreySpitOutSpriteTemplate =
{
    .tileTag = ANIM_TAG_ORBS,
    .paletteTag = ANIM_TAG_ORBS,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = sPowerAbsorptionOrbAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimShadowBall,
};

const struct SpriteTemplate gLastResortSparklesSpriteTemplate =
{
    .tileTag = ANIM_TAG_SPARKLE_2,
    .paletteTag = ANIM_TAG_PAIN_SPLIT,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sGrantingStarsAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimGrantingStars,
};

// Animates the falling particles that horizontally wave back and forth. Used by Sleep Powder, Stun Spore, and Poison Powder.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: total duration in frames
// arg 3: vertical movement speed (sub-pixel value)
// arg 4: wave amplitude
// arg 5: wave speed
void AnimMovePowderParticle(struct Sprite* sprite)
{
    sprite->x += gBattleAnimArgs[0];
    sprite->y += gBattleAnimArgs[1];
	
	if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_OPPONENT)
        gBattleAnimArgs[4] = -gBattleAnimArgs[4];
	
    sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[1] = gBattleAnimArgs[3];
	sprite->data[3] = gBattleAnimArgs[4];
    sprite->data[4] = gBattleAnimArgs[5];
    sprite->callback = AnimMovePowderParticleStep;
}

static void AnimMovePowderParticleStep(struct Sprite* sprite)
{
    if (sprite->data[0] > 0)
    {
        sprite->data[0]--;
		
        sprite->y2 = sprite->data[2] >> 8;
        sprite->data[2] += sprite->data[1];
		
        sprite->x2 = Sin(sprite->data[5], sprite->data[3]);
        sprite->data[5] = (sprite->data[5] + sprite->data[4]) & 0xFF;
    }
    else
        DestroyAnimSprite(sprite);
}

// Moves an energy orb towards the center of the mon.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: duration
void AnimPowerAbsorptionOrb(struct Sprite* sprite)
{
    InitSpritePosToAnimAttacker(sprite, TRUE);
    sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Moves an orb in a straight line towards the target mon.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: duration
// arg 3: sprite anim number
void AnimSolarbeamBigOrb(struct Sprite* sprite)
{
    InitSpritePosToAnimAttacker(sprite, TRUE);
    StartSpriteAnim(sprite, gBattleAnimArgs[3]);
    sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Moves a small orb in a wavy pattern towards the target mon. The small orb "circles" the big orbs in AnimSolarbeamBigOrb.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: duration
// arg 3: initial wave offset
void AnimSolarbeamSmallOrb(struct Sprite* sprite)
{
    InitSpritePosToAnimAttacker(sprite, TRUE);
    sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[1] = sprite->x;
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    sprite->data[3] = sprite->y;
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
    InitAnimLinearTranslation(sprite);
    sprite->data[5] = gBattleAnimArgs[3];
    sprite->callback = AnimSolarbeamSmallOrbStep;
    sprite->callback(sprite);
}

static void AnimSolarbeamSmallOrbStep(struct Sprite* sprite)
{
    if (AnimTranslateLinear(sprite))
        DestroySprite(sprite);
    else
    {
        if (sprite->data[5] > 0x7F)
            sprite->subpriority = GetBattlerSpriteSubpriority(gBattleAnimTarget) + 1;
        else
            sprite->subpriority = GetBattlerSpriteSubpriority(gBattleAnimTarget) + 6;

        sprite->x2 += Sin(sprite->data[5], 5);
        sprite->y2 += Cos(sprite->data[5], 14);
        sprite->data[5] = (sprite->data[5] + 15) & 0xFF;
    }
}

// Creates 15 small secondary orbs used in the solarbeam anim effect.
// There is a 7-frame delay between each of them.
// No args.
void AnimTask_CreateSmallSolarbeamOrbs(u8 taskId)
{
    if (--gTasks[taskId].data[0] == -1)
    {
        gTasks[taskId].data[0] = 6;
		gTasks[taskId].data[1]++;
        gBattleAnimArgs[0] = 15;
        gBattleAnimArgs[1] = 0;
        gBattleAnimArgs[2] = 80;
        gBattleAnimArgs[3] = 0;
        CreateSpriteAndAnimate(&gSolarbeamSmallOrbSpriteTemplate, 0, 0, GetBattlerSpriteSubpriority(gBattleAnimTarget) + 1);
    }

    if (gTasks[taskId].data[1] == 15)
        DestroyAnimVisualTask(taskId);
}

// Moves an orb from the target mon to the attacking mon in an arc-like fashion.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: wave amplitude
// arg 3: wave period (lower means faster wave)
void AnimAbsorptionOrb(struct Sprite* sprite)
{
    InitSpritePosToAnimTarget(sprite, TRUE);
    sprite->data[0] = gBattleAnimArgs[3];
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
    sprite->data[5] = gBattleAnimArgs[2];
    InitAnimArcTranslation(sprite);
    sprite->callback = DestroyAnimSpriteAfterHorizontalTranslation;
}

// Moves an orb in a wave-like fashion towards the target mon. The wave's
// properties and the sprite anim are randomly determined.
// No args.
void AnimHyperBeamOrb(struct Sprite* sprite)
{
    StartSpriteAnim(sprite, RandomMax(8));
	
    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
	
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        sprite->x -= 20;
    else
        sprite->x += 20;

    sprite->data[0] = (Random() & 31) + 64;
    sprite->data[1] = sprite->x;
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    sprite->data[3] = sprite->y;
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
    InitAnimFastLinearTranslationWithSpeed(sprite);
	
    sprite->data[5] = Random() & 0xFF;
    sprite->data[6] = sprite->subpriority;
	
    sprite->callback = AnimHyperBeamOrbStep;
    sprite->callback(sprite);
}

static void AnimHyperBeamOrbStep(struct Sprite* sprite)
{
    if (AnimFastTranslateLinear(sprite))
        DestroyAnimSprite(sprite);
    else
    {
        sprite->y2 += Cos(sprite->data[5], 12);
		
        if (sprite->data[5] < 0x7F)
            sprite->subpriority = sprite->data[6];
        else
            sprite->subpriority = sprite->data[6] + 1;

        sprite->data[5] += 24;
        sprite->data[5] &= 0xFF;
    }
}

// seed (sprouts a sapling from a seed.) Used by Leech Seed.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: target x pixel offset
// arg 3: target y pixel offset
// arg 4: duration
// arg 5: wave amplitude
void AnimLeechSeed(struct Sprite* sprite)
{
    InitSpritePosToAnimAttacker(sprite, TRUE);
	
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];

    sprite->data[0] = gBattleAnimArgs[4];
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X) + gBattleAnimArgs[2];
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y) + gBattleAnimArgs[3];
    sprite->data[5] = gBattleAnimArgs[5];
    InitAnimArcTranslation(sprite);
    sprite->callback = AnimLeechSeedStep;
}

static void AnimLeechSeedStep(struct Sprite* sprite)
{
    if (TranslateAnimHorizontalArc(sprite))
    {
        sprite->invisible = TRUE;
        sprite->data[0] = 10;
        sprite->callback = WaitAnimForDuration;
        StoreSpriteCallbackInData6(sprite, AnimLeechSeedSprouts);
    }
}

static void AnimLeechSeedSprouts(struct Sprite* sprite)
{
    sprite->invisible = FALSE;
    StartSpriteAnim(sprite, 1);
    sprite->data[0] = 60;
    sprite->callback = WaitAnimForDuration;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Animates a sprite that moves linearly from one location to another, with a
// single-cycle sine wave added to the y position along the way.
// Used by Razor Leaf and Magical Leaf.
// arg 0: initial x offset
// arg 1: initial y offset
// arg 2: target x offset
// arg 3: target y offset
// arg 4: translation duration
// arg 5: wave amplitude
// arg 6: target between double battle opponents (boolean)
void AnimTranslateLinearSingleSineWave(struct Sprite* sprite)
{
    InitSpritePosToAnimAttacker(sprite, TRUE);
	
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];

    sprite->data[0] = gBattleAnimArgs[4];
	
    if (!gBattleAnimArgs[6])
    {
        sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X) + gBattleAnimArgs[2];
        sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3];
    }
    else
    {
        SetAverageBattlerPositions(gBattleAnimTarget, TRUE, &sprite->data[2], &sprite->data[4]);
        sprite->data[2] += gBattleAnimArgs[2];
        sprite->data[4] += gBattleAnimArgs[3];
    }
    sprite->data[5] = gBattleAnimArgs[5];
    InitAnimArcTranslation(sprite);
	
    if (GetBattlerSide(gBattleAnimAttacker) == GetBattlerSide(gBattleAnimTarget))
        sprite->data[0] = 1;
    else
        sprite->data[0] = 0;

    sprite->callback = AnimTranslateLinearSingleSineWaveStep;
}

static void AnimTranslateLinearSingleSineWaveStep(struct Sprite* sprite)
{
    bool8 destroy = FALSE;
    s16 a = sprite->data[0];
    s16 b = sprite->data[7];

    sprite->data[0] = 1;
	
    TranslateAnimHorizontalArc(sprite);
	
	sprite->data[0] = a;
	
    if (b > 200 && sprite->data[7] < 56 && sprite->oam.affineParam == 0)
        sprite->oam.affineParam++;

    if (sprite->oam.affineParam && sprite->data[0])
    {
        sprite->invisible ^= TRUE;

        if (++sprite->oam.affineParam == 30)
            destroy = TRUE;
    }

    if (sprite->x + sprite->x2 > 256 || sprite->x + sprite->x2 < -16 || sprite->y + sprite->y2 > 160 || sprite->y + sprite->y2 < -16)
        destroy = TRUE;

    if (destroy)
        DestroyAnimSprite(sprite);
}

// Squeezes a constricting "rope" several times via affine animations.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: affine anim num
// arg 3: num squeezes
void AnimConstrictBinding(struct Sprite* sprite)
{
    InitSpritePosToAnimTarget(sprite, FALSE);
    sprite->affineAnimPaused = TRUE;
    StartSpriteAffineAnim(sprite, gBattleAnimArgs[2]);
    sprite->data[6] = gBattleAnimArgs[2];
    sprite->data[7] = gBattleAnimArgs[3];
    sprite->callback = AnimConstrictBindingStep1;
}

static void AnimConstrictBindingStep1(struct Sprite* sprite)
{
	// Signal to go to next step
    if (gBattleAnimArgs[ARG_RET_ID] == -1)
    {
        sprite->affineAnimPaused = FALSE;
        sprite->data[0] = 0x100;
        sprite->callback = AnimConstrictBindingStep2;
    }
}

static void AnimConstrictBindingStep2(struct Sprite* sprite)
{
    if (!sprite->data[2])
        sprite->data[0] += 11;
    else
        sprite->data[0] -= 11;

    if (++sprite->data[1] == 6)
    {
        sprite->data[1] = 0;
        sprite->data[2] ^= TRUE;
    }

    if (sprite->affineAnimEnded)
    {
        if (--sprite->data[7] > 0)
            StartSpriteAffineAnim(sprite, sprite->data[6]);
        else
            DestroyAnimSprite(sprite);
    }
}

// Creates a copy of the battler's sprite and shink it, used by MOVE_MIMIC.
// arg 0: x slide
// arg 1: shink duration
// arg 2: anim battler
void AnimTask_ShrinkBattlerCopy(u8 taskId)
{
    u8 spriteId = GetAnimBattlerSpriteId(gBattleAnimArgs[2]);
    
    if (gSprites[spriteId].invisible)
        DestroyAnimVisualTask(taskId);
    else
    {
        PrepareBattlerSpriteForRotScale(spriteId, ST_OAM_OBJ_BLEND);
        gTasks[taskId].data[0] = gBattleAnimArgs[0];
        gTasks[taskId].data[1] = gBattleAnimArgs[1];
		gTasks[taskId].data[2] = spriteId;
		gTasks[taskId].data[3] = GetBattlerForAnimScript(gBattleAnimArgs[2]);
        gTasks[taskId].data[11] = 0x100;
        gTasks[taskId].func = AnimTask_DuplicateAndShrinkToPosStep1;
    }
}

static void AnimTask_DuplicateAndShrinkToPosStep1(u8 taskId)
{
    u8 spriteId = gTasks[taskId].data[2];
    
    gTasks[taskId].data[10] += gTasks[taskId].data[0];
    gSprites[spriteId].x2 = gTasks[taskId].data[10] >> 8;
	
    if (GetBattlerSide(gTasks[taskId].data[3]) != B_SIDE_PLAYER)
        gSprites[spriteId].x2 = -gSprites[spriteId].x2;

    gTasks[taskId].data[11] += 16;
    SetSpriteRotScale(spriteId, gTasks[taskId].data[11], gTasks[taskId].data[11], 0);
	
    SetBattlerSpriteYOffsetFromYScale(spriteId);
	
    if (--gTasks[taskId].data[1] == 0)
    {
        gTasks[taskId].data[0] = 0;
        gTasks[taskId].func = AnimTask_DuplicateAndShrinkToPosStep2;
    }
}

static void AnimTask_DuplicateAndShrinkToPosStep2(u8 taskId)
{
    if (gBattleAnimArgs[ARG_RET_ID] == -1) // Signal to end anim
    {
        if (gTasks[taskId].data[0] == 0)
        {
            u8 spriteId = gTasks[taskId].data[2];
			
			gSprites[spriteId].x2 = 0;
            gSprites[spriteId].y2 = 0;
            ResetSpriteRotScale(spriteId);
            gTasks[taskId].data[0]++;
            return;
        }
    }
    else
    {
        if (gTasks[taskId].data[0] == 0)
            return;
    }

    if (++gTasks[taskId].data[0] == 3)
        DestroyAnimVisualTask(taskId);
}

// Moves an orb from the target mon to the attacking mon.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
void AnimMimicOrb(struct Sprite* sprite)
{
    switch (sprite->data[0])
    {
    case 0:
        if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_PLAYER)
            gBattleAnimArgs[0] = -gBattleAnimArgs[0];

        sprite->x = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X) + gBattleAnimArgs[0];
        sprite->y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y) + gBattleAnimArgs[1];
        sprite->invisible = TRUE;
        sprite->data[0]++;
        break;
    case 1:
        sprite->invisible = FALSE;
		
        if (sprite->affineAnimEnded)
        {
            ChangeSpriteAffineAnim(sprite, 1);
            sprite->data[0] = 25;
            sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
            sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
            sprite->callback = InitAndRunAnimFastLinearTranslation;
            StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
            break;
        }
    }
}

static void InitItemBagData(struct Sprite* sprite, s16 c)
{
    int a = (sprite->x << 8) | sprite->y;
    int b = (sprite->data[6] << 8) | sprite->data[7];
    c <<= 8;
    sprite->data[5] = a;
    sprite->data[6] = b;
    sprite->data[7] = c;
}

bool8 MoveAlongLinearPath(struct Sprite* sprite)
{
    u16 xStartPos = (u8)(sprite->data[5] >> 8);
    u16 yStartPos = (u8)sprite->data[5];
    s32 xEndPos = (u8)(sprite->data[6] >> 8);
    s32 yEndPos = (u8)sprite->data[6];
    s16 totalTime = sprite->data[7] >> 8;
    s16 currentTime = sprite->data[7] & 0xFF;
    s16 yEndPos_2;
    s16 r0;
    s32 var1;
    s32 vaxEndPos;

    if (xEndPos == 0)
        xEndPos = -32;
    else if (xEndPos == 255)
        xEndPos = 272;

    yEndPos_2 = yEndPos - yStartPos;
    r0 = xEndPos - xStartPos;
    var1 = r0 * currentTime / totalTime;
    vaxEndPos = yEndPos_2 * currentTime / totalTime;
    sprite->x = var1 + xStartPos;
    sprite->y = vaxEndPos + yStartPos;
	
    if (++currentTime == totalTime)
        return TRUE;

    sprite->data[7] = (totalTime << 8) | currentTime;
	
    return FALSE;
}

void AnimItemStealStep2(struct Sprite* sprite)
{
    if (sprite->data[0] == 10)
        StartSpriteAffineAnim(sprite, 1);

    if (++sprite->data[0] > 50)
        DestroyAnimSprite(sprite);
}

static void AnimItemStealStep1(struct Sprite* sprite)
{
    sprite->data[0] += sprite->data[3] * 128 / sprite->data[4];
	
    if (sprite->data[0] >= 128)
    {
		sprite->data[0] = 0;
        sprite->data[1]++;
    }
    sprite->y2 = Sin(sprite->data[0] + 128, 30 - sprite->data[1] * 8);
	
    if (MoveAlongLinearPath(sprite))
    {
        sprite->y2 = 0;
        sprite->data[0] = 0;
        sprite->callback = AnimItemStealStep2;
    }
}

// Animates MOVE_PRESENT's item bag jumping to the target's position.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
void AnimPresent(struct Sprite* sprite)
{
    InitSpritePosToAnimAttacker(sprite, FALSE);
	
	sprite->data[3] = GetBattlerSide(gBattleAnimAttacker) == GetBattlerSide(gBattleAnimTarget) ? 1 : 3;
	sprite->data[6] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
	sprite->data[7] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y) + 10;
	
	InitItemBagData(sprite, 60);

    sprite->data[4] = 60;
    sprite->callback = AnimItemStealStep1;
}

static void AnimItemStealStep4(struct Sprite* sprite)
{
    sprite->data[0] += ((sprite->data[3] * 128) / sprite->data[4]);

    if (sprite->data[0] > 0x7F)
    {
        sprite->data[1]++;
        sprite->data[0] = 0;
    }
    sprite->y2 = Sin(sprite->data[0] + 0x80, 30 - sprite->data[1] * 8);
	
    if (MoveAlongLinearPath(sprite))
    {
        sprite->y2 = 0;
        sprite->data[0] = 0;
        DestroyAnimSprite(sprite);
    }
}

// Animates Knock Off's item removal anim.
// No args.
void AnimKnockOffItem(struct Sprite* sprite)
{
	sprite->data[3] = 3;
	sprite->data[4] = 60;
	sprite->data[7] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y) + 10;
	
    if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_PLAYER)
    {
        sprite->data[6] = 0;
        sprite->callback = AnimItemStealStep1;
    }
    else
    {
        sprite->data[6] = 255;
        sprite->callback = AnimItemStealStep4;
    }
	InitItemBagData(sprite, 40);
}

// Animates a heal particle upward.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: vertical velocity
void AnimPresentHealParticle(struct Sprite* sprite)
{
    if (!sprite->data[0])
    {
        InitSpritePosToAnimTarget(sprite, FALSE);
        sprite->data[1] = gBattleAnimArgs[2];
    }
    sprite->data[0]++;
	
    sprite->y2 = sprite->data[1] * sprite->data[0];
	
    if (sprite->animEnded)
        DestroyAnimSprite(sprite);
}

// Animates the item steal animation, moving from the target's position to the attacker's position.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
void AnimItemSteal(struct Sprite* sprite)
{
    InitSpritePosToAnimTarget(sprite, FALSE);
	
    if (GetBattlerSide(gBattleAnimTarget) == GetBattlerSide(gBattleAnimAttacker))
        sprite->data[3] = 1;
    else
        sprite->data[3] = 3;
	
	sprite->data[6] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
	sprite->data[7] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y) + 10;
	InitItemBagData(sprite, 60);
    sprite->data[4] = 60;
    sprite->callback = AnimItemStealStep3;
}

static void AnimItemStealStep3(struct Sprite* sprite)
{
    sprite->data[0] += ((sprite->data[3] * 128) / sprite->data[4]);

    if (sprite->data[0] > 127)
    {
        sprite->data[0] = 0;
		sprite->data[1]++;
    }

    sprite->y2 = Sin(sprite->data[0] + 0x80, 30 - sprite->data[1] * 8);
    if (sprite->y2 == 0)
        PlaySE12WithPanning(SE_M_BUBBLE2, BattleAnimAdjustPanning(SOUND_PAN_TARGET));

    if (MoveAlongLinearPath(sprite))
    {
		PlaySE12WithPanning(SE_M_BUBBLE2, BattleAnimAdjustPanning(SOUND_PAN_ATTACKER));
		
        sprite->y2 = 0;
        sprite->data[0] = 0;
        sprite->callback = AnimItemStealStep2;
    }
}

// Moves a bag in a circular motion. Used by MOVE_TRICK.
// arg 0: y position
// arg 1: initial wave offset
void AnimTrickBag(struct Sprite* sprite)
{
    int a;
    int b;

    if (!sprite->data[0])
    {
        sprite->x = 120;
        sprite->y = gBattleAnimArgs[0];
		
		sprite->data[1] = gBattleAnimArgs[1];
        sprite->data[2] = gBattleAnimArgs[0];
        sprite->data[4] = 20;
		
        sprite->x2 = Cos(sprite->data[1], 60);
        sprite->y2 = Sin(sprite->data[1], 20);
        sprite->callback = AnimTrickBagStep1;
		
        if (sprite->data[1] > 0 && sprite->data[1] < 192)
            sprite->subpriority = 31;
        else
            sprite->subpriority = 29;
    }
}

static void AnimTrickBagStep1(struct Sprite* sprite)
{
    switch (sprite->data[3])
    {
    case 0:
        if (sprite->data[2] > 78)
        {
            sprite->data[3] = 1;
            StartSpriteAffineAnim(sprite, 1);
        }
        else
        {
            sprite->data[2] += sprite->data[4] / 10;
            sprite->data[4] += 3;
            sprite->y = sprite->data[2];
        }
        break;
    case 1:
        if (sprite->data[3] && sprite->affineAnimEnded)
        {
            sprite->data[0] = 0;
            sprite->data[2] = 0;
            sprite->callback = AnimTrickBagStep2;
        }
        break;
    }
}

static void AnimTrickBagStep2(struct Sprite* sprite)
{
    if (sprite->data[2] == gTrickBagCoordinates[sprite->data[0]][1])
    {
        if (gTrickBagCoordinates[sprite->data[0]][2] == 127)
        {
            sprite->data[0] = 0;
            sprite->callback = AnimTrickBagStep3;
        }
        sprite->data[2] = 0;
        sprite->data[0]++;
    }
    else
    {
        sprite->data[2]++;
        sprite->data[1] = (gTrickBagCoordinates[sprite->data[0]][0] * gTrickBagCoordinates[sprite->data[0]][2] + sprite->data[1]) & 0xFF;
		
        if ((u16)(sprite->data[1] - 1) < 191)
            sprite->subpriority = 31;
        else
            sprite->subpriority = 29;
		
        sprite->x2 = Cos(sprite->data[1], 60);
        sprite->y2 = Sin(sprite->data[1], 20);
    }
}

static void AnimTrickBagStep3(struct Sprite* sprite)
{
    if (sprite->data[0] > 20)
        DestroyAnimSprite(sprite);

    sprite->invisible = sprite->data[0] % 2;
    sprite->data[0]++;
}

// Animates a flying particle across the screen. Used by moves like Aromatherapy and Silver Wind.
// arg 0: y pixel offset
// arg 1: y movement speed
// arg 2: sin index
// arg 3: x movement speed
// arg 4: sin adder
// arg 5: y pos and priority case id
// arg 6: anim battler
void AnimFlyingParticle(struct Sprite* sprite)
{
    u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[6]);
 

    if (GetBattlerSide(battler) != B_SIDE_PLAYER)
    {
        sprite->data[4] = FALSE;
        sprite->x = -16;
    }
    else
    {
		gBattleAnimArgs[3] = -gBattleAnimArgs[3];
		
        sprite->data[4] = TRUE;
        sprite->x = 256;
    }
    sprite->data[0] = gBattleAnimArgs[2];
	sprite->data[1] = gBattleAnimArgs[1];
	sprite->data[2] = gBattleAnimArgs[3];
    sprite->data[3] = gBattleAnimArgs[4];
	
    switch (gBattleAnimArgs[5])
    {
    case 0:
        sprite->y = gBattleAnimArgs[0];
        sprite->oam.priority = GetBattlerSpriteBGPriority(battler);
        break;
    case 1:
        sprite->y = gBattleAnimArgs[0];
        sprite->oam.priority = GetBattlerSpriteBGPriority(battler) + 1;
        break;
    case 2:
        sprite->y = GetBattlerSpriteCoord(battler, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[0];
        sprite->oam.priority = GetBattlerSpriteBGPriority(battler);
        break;
    case 3:
        sprite->y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[0];
        sprite->oam.priority = GetBattlerSpriteBGPriority(battler) + 1;
        break;
    }
    sprite->callback = AnimFlyingParticleStep;
}

static void AnimFlyingParticleStep(struct Sprite* sprite)
{
    int a = sprite->data[7];
    
    sprite->data[7]++;
	
    sprite->y2 = (sprite->data[1] * gSineTable[sprite->data[0]]) >> 8;
    sprite->x2 = sprite->data[2] * a;
	
    sprite->data[0] = (sprite->data[3] * a) & 0xFF;
	
    if (!sprite->data[4])
    {
        if (sprite->x2 + sprite->x <= 0xF7)
            return;
    }
    else
    {
        if (sprite->x2 + sprite->x > -16)
            return;
    }
    DestroyAnimSprite(sprite);
}

// Animates the Slam hit and the Vine Whip on the target.
// arg 0: x offset
// arg 1: y offset
void AnimWhipHit(struct Sprite* sprite)
{
    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
        StartSpriteAnim(sprite, 1);

    sprite->callback = DestroyAnimSpriteWhenAnimEnds;
    SetAnimSpriteInitialXOffset(sprite, gBattleAnimArgs[0]);
    sprite->y += gBattleAnimArgs[1];
}

// Moves the sprite in a diagonally slashing motion across the target mon. Used by moves such as MOVE_CUT and MOVE_AERIAL_ACE.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: slice direction; 0 = right-to-left, 1 = left-to-right
// arg 3: which coord to use
void AnimCuttingSlice(struct Sprite* sprite)
{
	u8 x, y;
    
    switch (gBattleAnimArgs[3])
    {
    case 1:
        x = GetBattlerSpriteCoord(BATTLE_PARTNER(gBattleAnimTarget), BATTLER_COORD_X);
        y = GetBattlerSpriteCoord(BATTLE_PARTNER(gBattleAnimTarget), BATTLER_COORD_Y);
        break;
    case 2:
        x = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
        y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y);
		
        if (IsBattlerSpriteVisible(BATTLE_PARTNER(gBattleAnimTarget)))
        {
            x = (GetBattlerSpriteCoord(BATTLE_PARTNER(gBattleAnimTarget), BATTLER_COORD_X) + x) / 2;
            y = (GetBattlerSpriteCoord(BATTLE_PARTNER(gBattleAnimTarget), BATTLER_COORD_Y) + y) / 2;
        }
        break;
    default:
        x = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
        y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y);
        break;
    }
	
    sprite->x = x;
    sprite->y = y;
	
    if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_PLAYER)
        sprite->y += 8;
	
    if (gBattleAnimArgs[2] == 0)
        sprite->x += gBattleAnimArgs[0];
    else
    {
        sprite->x -= gBattleAnimArgs[0];
        sprite->hFlip = TRUE;
    }
    sprite->y += gBattleAnimArgs[1];
	
    sprite->data[1] -= 0x400;
    sprite->data[2] += 0x400;
    sprite->data[5] = gBattleAnimArgs[2];
    if (sprite->data[5] == 1)
        sprite->data[1] = -sprite->data[1];
	
	sprite->callback = AnimSliceStep;
}

static void AnimSliceStep(struct Sprite* sprite)
{
    sprite->data[3] += sprite->data[1];
    sprite->data[4] += sprite->data[2];
	
    if (sprite->data[5] == 0)
        sprite->data[1] += 0x18;
    else
        sprite->data[1] -= 0x18;

    sprite->data[2] -= 0x18;
    sprite->x2 = sprite->data[3] >> 8;
    sprite->y2 = sprite->data[4] >> 8;

    if (++sprite->data[0] == 20)
    {
        StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
        sprite->data[0] = 3;
        sprite->callback = WaitAnimForDuration;
    }
}

// Animates the protect's wall on the attacker.
// arg 0: x offset
// arg 1: y offset
// arg 2: duration (?)
void AnimProtect(struct Sprite* sprite)
{
    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X) + gBattleAnimArgs[0];
    sprite->y = GetBattlerSpriteCoord2(gBattleAnimAttacker, BATTLER_COORD_Y) + gBattleAnimArgs[1];
	
    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
        sprite->oam.priority = GetBattlerSpriteBGPriority(gBattleAnimAttacker) + 1;
    else
        sprite->oam.priority = GetBattlerSpriteBGPriority(gBattleAnimAttacker);

    sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[2] = (IndexOfSpritePaletteTag(ANIM_TAG_PROTECT) << 4) + 0x100;
    sprite->data[7] = 16;
	
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16 - sprite->data[7], sprite->data[7]));
	
    sprite->callback = AnimProtectStep;
}

static void AnimProtectStep(struct Sprite *sprite)
{
    int i, id, savedPal;
	
    sprite->data[5] += 96;
    sprite->x2 = -(sprite->data[5] >> 8);
	
    if (++sprite->data[1] > 1)
    {
        sprite->data[1] = 0;
		
        savedPal = gPlttBufferFaded[sprite->data[2] + 1];
		
        i = 0;
        while (i < 6)
        {
            id = sprite->data[2] + ++i;
            gPlttBufferFaded[id] = gPlttBufferFaded[id + 1];
        }
        gPlttBufferFaded[sprite->data[2] + 7] = savedPal;
    }

    if (sprite->data[7] > 6 && sprite->data[0] > 0 && ++sprite->data[6] > 1)
    {
        sprite->data[6] = 0;
		
        sprite->data[7]--;
		
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16 - sprite->data[7], sprite->data[7]));
    }

    if (sprite->data[0] > 0)
        sprite->data[0]--;
    else if (++sprite->data[6] > 1)
    {
        sprite->data[6] = 0;
		
        sprite->data[7]++;
		
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16 - sprite->data[7], sprite->data[7]));
		
        if (sprite->data[7] == 16)
        {
            sprite->invisible = TRUE;
            sprite->callback = DestroyAnimSpriteAndDisableBlend;
        }
    }
}

// Animates MOVE_MILK_DRINK's milk bottle sprite.
// No args.
void AnimMilkBottle(struct Sprite* sprite)
{
    sprite->x = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    sprite->y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + 0xFFE8;
	
    sprite->data[0] = 0;
    sprite->data[1] = 0;
    sprite->data[2] = 0;
    sprite->data[3] = 0;
    sprite->data[4] = 0;
    sprite->data[6] = 0;
    sprite->data[7] = 16;
	
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sprite->data[6], sprite->data[7]));
	
    sprite->callback = AnimMilkBottleStep1;
}

static void AnimMilkBottleStep1(struct Sprite* sprite)
{
    switch (sprite->data[0])
    {
    case 0:
        if (++sprite->data[2] > 0)
        {
            sprite->data[2] = 0;
			
            if (((++sprite->data[1]) & 1))
            {
                if (sprite->data[6] <= 15)
                    sprite->data[6]++;
            }
            else if (sprite->data[7] > 0)
                sprite->data[7]--;

            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sprite->data[6], sprite->data[7]));
			
            if (sprite->data[6] == 16 && sprite->data[7] == 0)
            {
                sprite->data[1] = 0;
                sprite->data[0]++;
            }
        }
        break;
    case 1:
        if (++sprite->data[1] > 8)
        {
            sprite->data[1] = 0;
			
            StartSpriteAffineAnim(sprite, 1);
			
            sprite->data[0]++;
        }
        break;
    case 2:
        AnimMilkBottleStep2(sprite, 16, 4);
		
        if (++sprite->data[1] > 2)
        {
            sprite->data[1] = 0;
            sprite->y++;
        }

        if (++sprite->data[2] <= 29)
            break;

        if (sprite->data[2] & 1)
        {
            if (sprite->data[6] > 0)
                sprite->data[6]--;
        }
        else if (sprite->data[7] <= 15)
            sprite->data[7]++;

        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sprite->data[6], sprite->data[7]));
		
        if (sprite->data[6] == 0 && sprite->data[7] == 16)
        {
            sprite->data[1] = 0;
            sprite->data[2] = 0;
            sprite->data[0]++;
        }
        break;
    case 3:
        sprite->invisible = TRUE;
        sprite->data[0]++;
        break;
    case 4:
        DestroyAnimSpriteAndDisableBlend(sprite);
        break;
    }
}

static void AnimMilkBottleStep2(struct Sprite* sprite, int unk1, int unk2)
{
    if (sprite->data[3] <= 11)
        sprite->data[4] += 2;

    if ((u16)(sprite->data[3] - 0x12) <= 0x17)
        sprite->data[4] -= 2;

    if ((sprite->data[3]) > 0x2F)
        sprite->data[4] += 2;

    sprite->x2 = sprite->data[4] / 9;
	
    sprite->y2 = sprite->data[4] / 14;
    if (sprite->y2 < 0)
        sprite->y2 *= -1;

    if (++sprite->data[3] > 0x3B)
        sprite->data[3] = 0;
}

// Animates small yellow stars on the given battler. Used by moves like Synthesis and Wish.
// arg 0: x offset
// arg 1: y offset
// arg 2: anim battler
// arg 3: x movement speed
// arg 4: y movement speed
// arg 5: duration
void AnimGrantingStars(struct Sprite* sprite)
{
    if (gBattleAnimArgs[2] == ANIM_ATTACKER)
        SetSpriteCoordsToAnimAttackerCoords(sprite);

    SetAnimSpriteInitialXOffset(sprite, gBattleAnimArgs[0]);
	
    sprite->y += gBattleAnimArgs[1];
	
    sprite->data[0] = gBattleAnimArgs[5];
    sprite->data[1] = gBattleAnimArgs[3];
    sprite->data[2] = gBattleAnimArgs[4];
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
    sprite->callback = TranslateSpriteLinearFixedPoint;
}

// Animates little sparkling yellow stars on the given battler.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: anim battler
// arg 3: x movement speed
// arg 4: y movement speed
// arg 5: duration
// arg 6: respectMonPicOffsets (boolean)
void AnimSparkingStars(struct Sprite* sprite)
{
    u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[2]);

    if (IsDoubleBattleForBattler(battler) && IsBattlerSpriteVisible(BATTLE_PARTNER(battler)))
    {
        SetAverageBattlerPositions(battler, gBattleAnimArgs[6], &sprite->x, &sprite->y);
		
        SetAnimSpriteInitialXOffset(sprite, gBattleAnimArgs[0]);
        sprite->y += gBattleAnimArgs[1];
    }
    else
    {
		sprite->x = GetBattlerSpriteCoord(battler, BATTLER_COORD_X);
		sprite->y = GetBattlerSpriteCoord(battler, gBattleAnimArgs[6] ? BATTLER_COORD_Y_PIC_OFFSET : BATTLER_COORD_Y) + gBattleAnimArgs[1];
		
        SetAnimSpriteInitialXOffset(sprite, gBattleAnimArgs[0]);
    }
    sprite->data[0] = gBattleAnimArgs[5];
    sprite->data[1] = gBattleAnimArgs[3];
    sprite->data[2] = gBattleAnimArgs[4];
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
    sprite->callback = TranslateSpriteLinearFixedPoint;
}

// Animates the Z letter on the sleep animation.
// arg 0: x pixel offset
// arg 1: y pixel offset
static void AnimSleepLetterZ(struct Sprite* sprite)
{
    SetSpriteCoordsToAnimAttackerCoords(sprite);
	
    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
    {
        sprite->x += gBattleAnimArgs[0];
        sprite->data[3] = 1;
    }
    else
    {
        sprite->x -= gBattleAnimArgs[0];
        sprite->data[3] = -1;
        StartSpriteAffineAnim(sprite, 1);
    }
	sprite->y += gBattleAnimArgs[1];
    sprite->callback = AnimSleepLetterZStep;
}

static void AnimSleepLetterZStep(struct Sprite* sprite)
{
    sprite->y2 = -(sprite->data[0] / 0x28);
    sprite->x2 = sprite->data[4] / 10;
	
    sprite->data[4] += sprite->data[3] * 2;
    sprite->data[0] += sprite->data[1];
	
    if (++sprite->data[1] > 60)
        DestroyAnimSprite(sprite);
}

// Animates Lock-On sprite.
// No args.
void AnimLockOnTarget(struct Sprite* sprite)
{
    sprite->x -= 32;
    sprite->y -= 32;
    sprite->data[0] = 20;
    sprite->callback = WaitAnimForDuration;
    StoreSpriteCallbackInData6(sprite, AnimLockOnTargetStep1);
}

static void AnimLockOnTargetStep1(struct Sprite* sprite)
{
    switch (sprite->data[5] & 1)
    {
    case 0:
        sprite->data[0] = 1;
        sprite->callback = WaitAnimForDuration;
        StoreSpriteCallbackInData6(sprite, AnimLockOnTargetStep1);
        break;
    case 1:
        SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
		
        sprite->data[0] = 8;
        sprite->data[2] = sprite->x + sInclineMonCoordTable[sprite->data[5] >> 8][0];
        sprite->data[4] = sprite->y + sInclineMonCoordTable[sprite->data[5] >> 8][1];
        sprite->callback = StartAnimLinearTranslation;
        StoreSpriteCallbackInData6(sprite, AnimLockOnTargetStep2);
		
        sprite->data[5] += 0x100;
		
        PlaySE12WithPanning(SE_M_LOCK_ON, BattleAnimAdjustPanning(SOUND_PAN_TARGET));
        break;
    }
    sprite->data[5] ^= 1;
}

static void AnimLockOnTargetStep2(struct Sprite* sprite)
{
    if ((sprite->data[5] >> 8) == 4)
    {
        sprite->data[0] = 10;
        sprite->callback = WaitAnimForDuration;
        StoreSpriteCallbackInData6(sprite, AnimLockOnTargetStep3);
    }
    else
        sprite->callback = AnimLockOnTargetStep1;
}

static void AnimLockOnTargetStep3(struct Sprite* sprite)
{
    s16 a, b;
    
    if (sprite->oam.affineParam == 0)
    {
        sprite->data[0] = 3;
        sprite->data[1] = 0;
        sprite->data[2] = 0;
        sprite->callback = WaitAnimForDuration;
        StoreSpriteCallbackInData6(sprite, AnimLockOnTargetStep4);
    }
    else
    {
        switch (sprite->oam.affineParam)
        {
        case 1:
            a = -8;
            b = -8;
            break;
        case 2:
            a = -8;
            b = 8;
            break;
        case 3:
            a = 8;
            b = -8;
            break;
        default:
            a = 8;
            b = 8;
            break;
        }
        SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
		
        sprite->data[0] = 6;
        sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X) + a;
        sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + b;
        sprite->callback = StartAnimLinearTranslation;
        StoreSpriteCallbackInData6(sprite, AnimLockOnTargetStep5);
    }
}

static void AnimLockOnTargetStep4(struct Sprite* sprite)
{
    if (sprite->data[2] == 0)
    {
		sprite->data[1] += 3;
		
        if (sprite->data[1] > 16)
            sprite->data[1] = 16;
    }
    else
	{
		sprite->data[1] -= 3;
		
		if (sprite->data[1] < 0)
			sprite->data[1] = 0;
    }
    BlendPalettes(SelectBattleAnimSpriteAndBgPalettes(TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE), sprite->data[1], RGB_WHITE);
	
    if (sprite->data[1] == 16)
    {
        int pal;
		
        sprite->data[2]++;
		
        pal = sprite->oam.paletteNum;
        LoadPalette(&gPlttBufferUnfaded[0x108 + pal * 16], pal * 16 | 0x101, 4);
		
        PlaySE12WithPanning(SE_M_LEER, BattleAnimAdjustPanning(SOUND_PAN_TARGET));
    }
    else if (sprite->data[1] == 0)
        sprite->callback = AnimLockOnTargetStep5;
}

static void AnimLockOnTargetStep5(struct Sprite* sprite)
{
    if (gBattleAnimArgs[ARG_RET_ID] == -1) // Signal to start blink effect
    {
        sprite->data[1] = 0;
        sprite->data[0] = 0;
        sprite->callback = AnimLockOnTargetStep6;
    }
}

static void AnimLockOnTargetStep6(struct Sprite* sprite)
{
    if (sprite->data[0] % 3 == 0)
    {
        sprite->data[1]++;
        sprite->invisible ^= TRUE;
    }
    sprite->data[0]++;
	
    if (sprite->data[1] == 8)
        DestroyAnimSprite(sprite);
}

// Part of Lock-On sprite animation. Used to determine the sprite frame image and position.
// arg 0: sprite frame id
void AnimLockOnMoveTarget(struct Sprite* sprite)
{
    sprite->oam.affineParam = gBattleAnimArgs[0];
	
    if ((s16)sprite->oam.affineParam == 1)
    {
        sprite->x -= 0x18;
        sprite->y -= 0x18;
    }
    else if ((s16)sprite->oam.affineParam == 2)
    {
        sprite->x -= 0x18;
        sprite->y += 0x18;
        sprite->oam.matrixNum = ST_OAM_VFLIP;
    }
    else if ((s16)sprite->oam.affineParam == 3)
    {
        sprite->x += 0x18;
        sprite->y -= 0x18;
        sprite->oam.matrixNum = ST_OAM_HFLIP;
    }
    else
    {
        sprite->x += 0x18;
        sprite->y += 0x18;
        sprite->oam.matrixNum = ST_OAM_HFLIP | ST_OAM_VFLIP;
    }
    sprite->oam.tileNum = (sprite->oam.tileNum + 16);
    sprite->callback = AnimLockOnTarget;
    sprite->callback(sprite);
}

// Animates the mon lowering it's head in MOVE_HEADBUTT anim.
// arg 0: state
void AnimBowMon(struct Sprite* sprite)
{
    sprite->invisible = TRUE;
    sprite->data[0] = 0;
	
    switch (gBattleAnimArgs[0])
    {
    case 0:
        sprite->callback = AnimBowMonStep1;
        break;
    case 1:
        sprite->callback = AnimBowMonStep2;
        break;
    case 2:
        sprite->callback = AnimBowMonStep3;
        break;
    default:
        sprite->callback = DestroyAnimSprite;
        break;
    }
}

static void AnimBowMonStep1(struct Sprite* sprite)
{
    sprite->data[0] = 6;
    sprite->data[1] = GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER ? 2 : -2;
    sprite->data[2] = 0;
    sprite->data[3] = gBattlerSpriteIds[gBattleAnimAttacker];
    StoreSpriteCallbackInData6(sprite, AnimBowMonStep1_Callback);
    sprite->callback = TranslateMonSpriteLinear;
}

static void AnimBowMonStep1_Callback(struct Sprite* sprite)
{
    if (sprite->data[0] == 0)
    {
        sprite->data[3] = gBattlerSpriteIds[gBattleAnimAttacker];
        PrepareBattlerSpriteForRotScale(sprite->data[3], ST_OAM_OBJ_NORMAL);
		sprite->data[6] = GetBattlerSide(gBattleAnimAttacker);
        sprite->data[4] = sprite->data[6] == B_SIDE_OPPONENT ? 0x300 : 0xFFFFFD00;
        sprite->data[5] = 0;
    }
    sprite->data[5] += sprite->data[4];
	
    SetSpriteRotScale(sprite->data[3], 0x100, 0x100, sprite->data[5]);
    SetBattlerSpriteYOffsetFromRotation(sprite->data[3]);
	
    if (++sprite->data[0] > 3)
    {
        sprite->data[0] = 0;
        sprite->callback = DestroyAnimSprite;
    }
}

static void AnimBowMonStep2(struct Sprite* sprite)
{
    sprite->data[0] = 4;
    sprite->data[1] = GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER ? -3 : 3;
    sprite->data[2] = 0;
    sprite->data[3] = gBattlerSpriteIds[gBattleAnimAttacker];
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
    sprite->callback = TranslateMonSpriteLinear;
}

static void AnimBowMonStep3(struct Sprite* sprite)
{
    if (++sprite->data[0] > 8)
    {
        sprite->data[0] = 0;
        sprite->callback = AnimBowMonStep3_Callback;
    }
}

static void AnimBowMonStep3_Callback(struct Sprite* sprite)
{
    if (sprite->data[0] == 0)
    {
        sprite->data[3] = gBattlerSpriteIds[gBattleAnimAttacker];
        sprite->data[6] = GetBattlerSide(gBattleAnimAttacker);
		
        if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        {
            sprite->data[4] = 0xFC00;
            sprite->data[5] = 0xC00;
        }
        else
        {
            sprite->data[4] = 0x400;
            sprite->data[5] = 0xF400;
        }
    }
    sprite->data[5] += sprite->data[4];
	
    SetSpriteRotScale(sprite->data[3], 0x100, 0x100, sprite->data[5]);
    SetBattlerSpriteYOffsetFromRotation(sprite->data[3]);
	
    if (++sprite->data[0] > 2)
    {
        ResetSpriteRotScale(sprite->data[3]);
        sprite->callback = DestroyAnimSprite;
    }
}

// Animates the Skull Bash second turn attack animation.
// arg 0: anim case
void AnimTask_SkullBashPosition(u8 taskId)
{
    u8 side = GetBattlerSide(gBattleAnimAttacker);

    gTasks[taskId].data[0] = gBattlerSpriteIds[gBattleAnimAttacker];
    gTasks[taskId].data[1] = side;
    gTasks[taskId].data[2] = 0;
	
    switch (gBattleAnimArgs[0])
    {
    case 0:
        gTasks[taskId].data[2] = 0;
        gTasks[taskId].data[3] = 8;
        gTasks[taskId].data[4] = 0;
        gTasks[taskId].data[5] = 3;
		
        if (side == B_SIDE_PLAYER)
            gTasks[taskId].data[5] *= -1;

        gTasks[taskId].func = AnimTask_SkullBashPositionSet;
        break;
    case 1:
        gTasks[taskId].data[3] = 8;
        gTasks[taskId].data[4] = 0x600;
        gTasks[taskId].data[5] = 0xC0;
		
        if (side == B_SIDE_PLAYER)
        {
            gTasks[taskId].data[4] = -gTasks[taskId].data[4];
            gTasks[taskId].data[5] = -gTasks[taskId].data[5];
        }
        gTasks[taskId].func = AnimTask_SkullBashPositionReset;
        break;
	default:
        DestroyAnimVisualTask(taskId);
        break;
    }
}

static void AnimTask_SkullBashPositionSet(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    
    switch (task->data[2])
    {
    case 0:
        if (task->data[3])
        {
			task->data[3]--;
            task->data[4] += task->data[5];
            gSprites[task->data[0]].x2 = task->data[4];
        }
        else
        {
            task->data[3] = 8;
            task->data[4] = 0;
            task->data[5] = (task->data[1] == B_SIDE_PLAYER) ? -0xC0 : 0xC0;
            PrepareBattlerSpriteForRotScale(task->data[0], ST_OAM_OBJ_NORMAL);
            task->data[2]++;
        }
        break;
    case 1:
        if (task->data[3])
        {
			task->data[3]--;
            task->data[4] += task->data[5];
            SetSpriteRotScale(task->data[0], 0x100, 0x100, task->data[4]);
            SetBattlerSpriteYOffsetFromRotation(task->data[0]);
        }
        else
        {
            task->data[3] = 8;
            task->data[4] = gSprites[task->data[0]].x2;
            task->data[5] = (task->data[1] == B_SIDE_PLAYER) ? 0x2 : -0x2;
            task->data[6] = 1;
            task->data[2]++;
        }
        break;
    case 2:
        if (task->data[3])
        {
            if (task->data[6])
                task->data[6]--;
            else
            {
                if (task->data[3] & 1)
                    gSprites[task->data[0]].x2 = task->data[4] + task->data[5];
                else
                    gSprites[task->data[0]].x2 = task->data[4] - task->data[5];

                task->data[6] = 1;
                task->data[3]--;
            }
        }
        else
        {
            gSprites[task->data[0]].x2 = task->data[4];
            task->data[3] = 12;
            task->data[2]++;
        }
        break;
    case 3:
        if (task->data[3])
            task->data[3]--;
        else
        {
            task->data[3] = 3;
            task->data[4] = gSprites[task->data[0]].x2;
            task->data[5] = (task->data[1] == B_SIDE_PLAYER) ? 8 : -8;
            task->data[2]++;
        }
        break;
    case 4:
        if (task->data[3])
        {
            task->data[4] += task->data[5];
            gSprites[task->data[0]].x2 = task->data[4];
            task->data[3]--;
        }
        else
            DestroyAnimVisualTask(taskId);
        break;
    }
}

static void AnimTask_SkullBashPositionReset(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    
    if (task->data[3])
    {
		task->data[3]--;
        task->data[4] -= task->data[5];
        SetSpriteRotScale(task->data[0], 0x100, 0x100, task->data[4]);
        SetBattlerSpriteYOffsetFromRotation(task->data[0]);
    }
    else
    {
        ResetSpriteRotScale(task->data[0]);
        DestroyAnimVisualTask(taskId);
    }
}

// Animates the Slash claw slices.
// arg 0: anim battler
// arg 1: x pixel offset
// arg 2: y pixel offset
void AnimSlashSlice(struct Sprite* sprite)
{
	u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);
	
	sprite->x = GetBattlerSpriteCoord(battler, BATTLER_COORD_X) + gBattleAnimArgs[1];
	sprite->y = GetBattlerSpriteCoord(battler, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[2];

    sprite->data[0] = 0;
    sprite->data[1] = 0;
    StoreSpriteCallbackInData6(sprite, AnimFalseSwipeSliceStep3);
    sprite->callback = RunStoredCallbackWhenAnimEnds;
}

// Animates the first False Swipe's slice. Which does their anim, waits some time, and then is destroyed.
// No args.
void AnimFalseSwipeSlice(struct Sprite* sprite)
{
    sprite->x = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X) + 0xFFD0;
    sprite->y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
	
	sprite->callback = RunStoredCallbackWhenAnimEnds;
    StoreSpriteCallbackInData6(sprite, AnimFalseSwipeSliceStep1);
}

// Animates an positioned False Swipe's slice.
// arg 0: x pixel offset
void AnimFalseSwipePositionedSlice(struct Sprite* sprite)
{
    sprite->x = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X) + 0xFFD0 + gBattleAnimArgs[0];
    sprite->y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
	
    StartSpriteAnim(sprite, 1);
	
    AnimFalseSwipeSliceStep2(sprite);
}

static void AnimFalseSwipeSliceStep1(struct Sprite* sprite)
{
    if (++sprite->data[0] > 8)
    {
        sprite->data[0] = 12;
        sprite->data[1] = 8;
        sprite->data[2] = 0;
		sprite->callback = TranslateSpriteLinear;
        StoreSpriteCallbackInData6(sprite, AnimFalseSwipeSliceStep2);
    }
}

static void AnimFalseSwipeSliceStep2(struct Sprite* sprite)
{
    sprite->data[0] = 0;
    sprite->data[1] = 0;
    sprite->callback = AnimFalseSwipeSliceStep3;
}

static void AnimFalseSwipeSliceStep3(struct Sprite* sprite)
{
    if (++sprite->data[0] > 1)
    {
        sprite->data[0] = 0;
		
        sprite->invisible ^= TRUE;
		
        if (++sprite->data[1] > 8)
            DestroyAnimSprite(sprite);
    }
}

// Animates the Focus Energy's particles moving upward in the attacker.
// arg 0: anim battler
// arg 1: x offset
// arg 2: y offset
// arg 3: movement speed
void AnimEndureEnergy(struct Sprite* sprite)
{
	u8 battlerId = GetBattlerForAnimScript(gBattleAnimArgs[0]);
	
    sprite->x = GetBattlerSpriteCoord(battlerId, BATTLER_COORD_X) + gBattleAnimArgs[1];
    sprite->y = GetBattlerSpriteCoord(battlerId, BATTLER_COORD_Y) + gBattleAnimArgs[2];
    
    sprite->data[0] = 0;
    sprite->data[1] = gBattleAnimArgs[3];
    sprite->callback = AnimEndureEnergyStep;
}

static void AnimEndureEnergyStep(struct Sprite* sprite)
{
    if (++sprite->data[0] > sprite->data[1])
    {
        sprite->data[0] = 0;
        sprite->y--;
    }
    sprite->y -= sprite->data[0];
	
    if (sprite->animEnded)
        DestroyAnimSprite(sprite);
}

// Animates the Sharpen's sphere to cube anim.
// No args.
void AnimSharpenSphere(struct Sprite* sprite)
{
    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET) - 12;
    sprite->data[0] = 0;
    sprite->data[1] = 2;
    sprite->data[2] = 0;
    sprite->data[3] = 0;
    sprite->data[4] = 0;
    sprite->data[5] = BattleAnimAdjustPanning(SOUND_PAN_ATTACKER);
    sprite->callback = AnimSharpenSphereStep;
}

static void AnimSharpenSphereStep(struct Sprite* sprite)
{
    if (++sprite->data[0] >= sprite->data[1])
    {
		sprite->data[0] = 0;
		
        sprite->invisible ^= TRUE;
		
        if (!sprite->invisible)
        {
            if (!(++sprite->data[4] & 1))
                PlaySE12WithPanning(SE_M_SWAGGER2, sprite->data[5]);
        }

        if (++sprite->data[2] > 1)
        {
            sprite->data[2] = 0;
            sprite->data[1]++;
        }
    }

    if (sprite->animEnded && sprite->data[1] > 16 && sprite->invisible)
        DestroyAnimSprite(sprite);
}

// Animates the Conversion's sprites on the attacker, waiting a signal to destroy it.
// arg 0: x pixel offset
// arg 1: y pixel offset
void AnimConversion(struct Sprite* sprite)
{
    if (sprite->data[0] == 0)
    {
		sprite->data[0]++;
		
        sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X) + gBattleAnimArgs[0];
        sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y) + gBattleAnimArgs[1];
    }
	
    if (gBattleAnimArgs[ARG_RET_ID] == -1) // Signal to destroy sprite
        DestroyAnimSprite(sprite);
}

// Animates Conversion's blend effect.
// No args.
void AnimTask_ConversionAlphaBlend(u8 taskId)
{
	switch (gTasks[taskId].data[2])
	{
		case 0:
			if (++gTasks[taskId].data[0] == 4)
			{
				gTasks[taskId].data[0] = 0;
				
				gTasks[taskId].data[1]++;
				
				SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16 - gTasks[taskId].data[1], gTasks[taskId].data[1]));
				
				if (gTasks[taskId].data[1] == 16)
					gTasks[taskId].data[2]++;
			}
			break;
		case 1:
			gBattleAnimArgs[ARG_RET_ID] = -1;
			gTasks[taskId].data[2]++;
			break;
		case 2:
			DestroyAnimVisualTask(taskId);
			break;
	}
}

// Animates MOVE_CONVERSION_2's sprites.
// arg 0: x pixel offset
// arg 1: y pixel offset
void AnimConversion2(struct Sprite* sprite)
{
    InitSpritePosToAnimTarget(sprite, FALSE);
    sprite->animPaused = TRUE;
    sprite->data[0] = gBattleAnimArgs[2];
    sprite->callback = AnimConversion2Step;
}

static void AnimConversion2Step(struct Sprite* sprite)
{
    if (sprite->data[0])
        sprite->data[0]--;
    else
    {
        sprite->animPaused = FALSE;
        sprite->data[0] = 30;
        sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
        sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
        sprite->callback = StartAnimLinearTranslation;
        StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
    }
}

// Animates MOVE_CONVERSION_2's sprites blend effect.
// No args.
void AnimTask_Conversion2AlphaBlend(u8 taskId)
{
    if (++gTasks[taskId].data[0] == 4)
    {
        gTasks[taskId].data[0] = 0;
		
        gTasks[taskId].data[1]++;
		
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[1], 16 - gTasks[taskId].data[1]));
		
        if (gTasks[taskId].data[1] == 16)
            DestroyAnimVisualTask(taskId);
    }
}

// Animates a horn hit attack.
// arg 0: x offset
// arg 1: y offset
// arg 2: duration
void AnimHornHit(struct Sprite* sprite)
{
    if (gBattleAnimArgs[2] < 2)
        gBattleAnimArgs[2] = 2;
    else if (gBattleAnimArgs[2] > 0x7F)
        gBattleAnimArgs[2] = 0x7F;

    sprite->data[0] = 0;
    sprite->data[1] = gBattleAnimArgs[2];
    sprite->x = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X) + gBattleAnimArgs[0];
    sprite->y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[1];
    sprite->data[6] = sprite->x;
    sprite->data[7] = sprite->y;
	
    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
    {
        sprite->x -= 40;
        sprite->y += 20;
        sprite->data[3] = 0x1400 / sprite->data[1];
        sprite->data[5] = -0xA00 / sprite->data[1];
    }
    else
    {
        sprite->x += 40;
        sprite->y -= 20;
        sprite->data[3] = -0x1400 / sprite->data[1];
        sprite->data[5] = 0xA00 / sprite->data[1];
        sprite->oam.matrixNum = (ST_OAM_HFLIP | ST_OAM_VFLIP);
    }
	sprite->data[2] = sprite->x << 7;
	sprite->data[4] = sprite->y << 7;

    sprite->callback = AnimHornHitStep;
}

static void AnimHornHitStep(struct Sprite* sprite)
{
    sprite->data[2] += sprite->data[3];
    sprite->data[4] += sprite->data[5];
	
    sprite->x = sprite->data[2] >> 7;
    sprite->y = sprite->data[4] >> 7;
	
    if (--sprite->data[1] == 1)
    {
        sprite->x = sprite->data[6];
        sprite->y = sprite->data[7];
    }

    if (sprite->data[1] == 0)
        DestroyAnimSprite(sprite);
}

// Animates the Double Team's attacker clones.
// No args.
void AnimTask_DoubleTeam(u8 taskId)
{
    u16 i;
    int obj;
    u16 r3;
    u16 r4;
    struct Task* task = &gTasks[taskId];
    
    task->data[0] = GetAnimBattlerSpriteId(ANIM_ATTACKER);
    task->data[1] = AllocSpritePalette(ANIM_TAG_BENT_SPOON);
	
    r3 = (task->data[1] * 16) + 0x100;
    r4 = (gSprites[task->data[0]].oam.paletteNum + 16) << 4;
	
    for (i = 1; i < 16; i++)
        gPlttBufferUnfaded[r3 + i] = gPlttBufferUnfaded[r4 + i];

    BlendPalette(r3, 16, 11, RGB_BLACK);
	
    task->data[3] = 0;
    i = 0;
	
    while (i < 2 && (obj = CloneBattlerSpriteWithBlend(0)) >= 0)
    {
        gSprites[obj].oam.paletteNum = task->data[1];
        gSprites[obj].data[0] = 0;
        gSprites[obj].data[1] = i << 7;
        gSprites[obj].data[2] = taskId;
        gSprites[obj].callback = AnimTask_DoubleTeamCallback;
        task->data[3]++;
        i++;
    }
	
    if (GetBattlerSpriteBGPriorityRank(gBattleAnimAttacker) == 1)
        ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_BG1_ON);
    else
        ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_BG2_ON);
	
	task->func = AnimTask_DoubleTeamStep;
}

static void AnimTask_DoubleTeamStep(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    
    if (!task->data[3])
    {
        if (GetBattlerSpriteBGPriorityRank(gBattleAnimAttacker) == 1)
            SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_BG1_ON);
        else
            SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_BG2_ON);

        FreeSpritePaletteByTag(ANIM_TAG_BENT_SPOON);
        DestroyAnimVisualTask(taskId);
    }
}

static void AnimTask_DoubleTeamCallback(struct Sprite* sprite)
{
    if (++sprite->data[3] > 1)
    {
        sprite->data[3] = 0;
        sprite->data[0]++;
    }

    if (sprite->data[0] > 64)
    {
        gTasks[sprite->data[2]].data[3]--;
        DestroySpriteWithActiveSheet(sprite);
    }
    else
    {
        sprite->data[4] = gSineTable[sprite->data[0]] / 6;
        sprite->data[5] = gSineTable[sprite->data[0]] / 13;
        sprite->data[1] = (sprite->data[1] + sprite->data[5]) & 0xFF;
        sprite->x2 = Sin(sprite->data[1], sprite->data[4]);
    }
}

// Starts the rainbow effect for musical notes.
// No args.
void AnimTask_MusicNotesRainbowBlend(u8 taskId)
{
    u16 i, j, index = IndexOfSpritePaletteTag(sParticlesColorBlendTable[0][0]);

    if (index != 0xFF)
    {
        index = (index << 4) + 0x100;
		
        for (i = 1; i < ARRAY_COUNT(sParticlesColorBlendTable[0]); i++)
            gPlttBufferFaded[index + i] = sParticlesColorBlendTable[0][i];
    }

    for (j = 1; j < ARRAY_COUNT(sParticlesColorBlendTable); j++)
    {
        index = AllocSpritePalette(sParticlesColorBlendTable[j][0]);
		
        if (index != 0xFF)
        {
            index = (index << 4) + 0x100;
			
            for (i = 1; i < ARRAY_COUNT(sParticlesColorBlendTable[0]); i++)
                gPlttBufferFaded[index + i] = sParticlesColorBlendTable[j][i];
        }
    }
    DestroyAnimVisualTask(taskId);
}

// Clears the rainbow effect for musical notes.
// No args.
void AnimTask_MusicNotesClearRainbowBlend(u8 taskId)
{
    u16 i;
    
    for (i = 1; i < ARRAY_COUNT(sParticlesColorBlendTable); i++)
        FreeSpritePaletteByTag(sParticlesColorBlendTable[i][0]);

    DestroyAnimVisualTask(taskId);
}

// Animates the music notes waving from the user's position to the target's position.
// arg 0: anim num
// arg 1: pal num
// arg 2: duration (?)
void AnimWavyMusicNotes(struct Sprite* sprite)
{
    u8 index, a, b;
    
    SetSpriteCoordsToAnimAttackerCoords(sprite);
    StartSpriteAnim(sprite, gBattleAnimArgs[0]);
	
	index = IndexOfSpritePaletteTag(sParticlesColorBlendTable[gBattleAnimArgs[1]][0]);
    if (index != 0xFF)
        sprite->oam.paletteNum = index;

    sprite->data[1] = gBattleAnimArgs[1];
    sprite->data[2] = 0;
    sprite->data[3] = gBattleAnimArgs[2];
	sprite->data[4] = sprite->x << 4;
    sprite->data[5] = sprite->y << 4;
	
    a = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    b = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
    AnimWavyMusicNotesGetNextPos(a - sprite->x, b - sprite->y, &sprite->data[6], &sprite->data[7], 40);
	
    sprite->callback = AnimWavyMusicNotesStep;
}

static void AnimWavyMusicNotesGetNextPos(s16 a, s16 b, s16* c, s16* d, s8 e)
{
    int f, g;
    
    if (a < 0)
        e = -e;

    f = a << 8;
    g = f / e;
    if (g == 0)
        g = 1;

    *c = f / g;
    *d = (b << 8) / g;
}

static void AnimWavyMusicNotesStep(struct Sprite* sprite)
{
    s16 y, yDelta;
    u8 index;

    sprite->data[0]++;
	
    yDelta = sprite->data[0] * 5 - ((sprite->data[0] * 5 / 256) << 8);
	
    sprite->data[4] += sprite->data[6];
    sprite->data[5] += sprite->data[7];
	
    sprite->x = sprite->data[4] >> 4;
    sprite->y = sprite->data[5] >> 4;
    sprite->y2 = Sin(yDelta, 15);

    y = sprite->y;
    if (sprite->x < -16 || sprite->x > 256 || y < -16 || y > 128)
        DestroyAnimSprite(sprite);
    else
    {
        if (sprite->data[3] && ++sprite->data[2] > sprite->data[3])
        {
            sprite->data[2] = 0;
			
            if (++sprite->data[1] > 3)
                sprite->data[1] = 0;

            index = IndexOfSpritePaletteTag(sParticlesColorBlendTable[sprite->data[1]][0]);
            if (index != 0xFF)
                sprite->oam.paletteNum = index;
        }
    }
}

// Animates MOVE_TEETER_DANCE's flying music notes moving fast.
// arg 0: sprite anim num
// arg 1: x pixel offset
// arg 2: y pixel offset
void AnimFlyingMusicNotes(struct Sprite* sprite)
{
    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_OPPONENT)
        gBattleAnimArgs[1] = -gBattleAnimArgs[1];

    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X) + gBattleAnimArgs[1];
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[2];
	
    StartSpriteAnim(sprite, gBattleAnimArgs[0]);
	
    sprite->data[2] = 0;
    sprite->data[3] = 0;
    sprite->data[4] = sprite->x << 4;
    sprite->data[5] = sprite->y << 4;
    sprite->data[6] = (gBattleAnimArgs[1] << 4) / 5;
    sprite->data[7] = (gBattleAnimArgs[2] << 7) / 5;
    sprite->callback = AnimFlyingMusicNotesStep;
}

static void AnimFlyingMusicNotesStep(struct Sprite* sprite)
{
    sprite->data[4] += sprite->data[6];
    sprite->data[5] += sprite->data[7];
	
    sprite->x = sprite->data[4] >> 4;
    sprite->y = sprite->data[5] >> 4;
	
    if (sprite->data[0] > 5 && sprite->data[3] == 0)
    {
        sprite->data[2] = (sprite->data[2] + 16) & 0xFF;
		
        sprite->x2 = Cos(sprite->data[2], 18);
        sprite->y2 = Sin(sprite->data[2], 18);
		
        if (sprite->data[2] == 0)
            sprite->data[3] = 1;
    }

    if (++sprite->data[0] == 48)
        DestroyAnimSprite(sprite);
}

// Animates MOVE_BELLY_DRUM's hand sprite.
// arg 0: flip horizontally (boolean)
void AnimBellyDrumHand(struct Sprite* sprite)
{
    s16 x;
    
    if (gBattleAnimArgs[0])
    {
        sprite->oam.matrixNum = ST_OAM_HFLIP;
        x = 16;
    }
    else
        x = -16;

    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X) + x;
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET) + 8;
    sprite->data[0] = 8;
    sprite->callback = WaitAnimForDuration;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Animates Belly Drum's music notes.
// arg 0: movement diretion (0 = to left, 1 = t oright)
// arg 1: sprite anim num
// arg 2: pal num
// arg 3: sin value
void AnimSlowFlyingMusicNotes(struct Sprite* sprite)
{
    s16 xDiff;
    u8 index;
    
    SetSpriteCoordsToAnimAttackerCoords(sprite);
    sprite->y += 8;
	
    StartSpriteAnim(sprite, gBattleAnimArgs[1]);
	
    index = IndexOfSpritePaletteTag(sParticlesColorBlendTable[gBattleAnimArgs[2]][0]);
    if (index != 0xFF)
        sprite->oam.paletteNum = index;

    xDiff = gBattleAnimArgs[0] == 0 ? -32 : 32;
	
    sprite->data[0] = 40;
    sprite->data[1] = sprite->x;
    sprite->data[2] = xDiff + sprite->data[1];
    sprite->data[3] = sprite->y;
    sprite->data[4] = sprite->data[3] - 40;
    InitAnimLinearTranslation(sprite);
    sprite->data[5] = gBattleAnimArgs[3];
    sprite->callback = AnimSlowFlyingMusicNotesStep;
}

static void AnimSlowFlyingMusicNotesStep(struct Sprite* sprite)
{
    if (!AnimTranslateLinear(sprite))
    {
        s16 xDiff = Sin(sprite->data[5], 8);
        
        if (sprite->x2 < 0)
            xDiff = -xDiff;

        sprite->x2 += xDiff;
        sprite->y2 += Sin(sprite->data[5], 4);
		
        sprite->data[5] = (sprite->data[5] + 8) & 0xFF;
    }
    else
        DestroyAnimSprite(sprite);
}

void SetSpriteNextToMonHead(u8 battler, struct Sprite* sprite)
{
    if (GetBattlerSide(battler) == B_SIDE_PLAYER)
        sprite->x = GetBattlerSpriteCoordAttr(battler, BATTLER_COORD_ATTR_RIGHT) + 8;
    else
        sprite->x = GetBattlerSpriteCoordAttr(battler, BATTLER_COORD_ATTR_LEFT) - 8;

    sprite->y = GetBattlerSpriteCoord(battler, BATTLER_COORD_Y_PIC_OFFSET) - (s16)GetBattlerSpriteCoordAttr(battler, BATTLER_COORD_ATTR_HEIGHT) / 4;
}

// Animates a thought bubble, used in MOVE_METRONOME and MOVE_TAUNT.
// arg 0: anim battler
// arg 1: duration
void AnimThoughtBubble(struct Sprite* sprite)
{
    u8 a, battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);

    SetSpriteNextToMonHead(battler, sprite);
	
    a = (GetBattlerSide(battler) == B_SIDE_PLAYER) ? 0 : 1;
	
    sprite->data[0] = gBattleAnimArgs[1];
    sprite->data[1] = a + 2;
	
    StartSpriteAnim(sprite, a);
	
    StoreSpriteCallbackInData6(sprite, AnimThoughtBubbleStep);
    sprite->callback = RunStoredCallbackWhenAnimEnds;
}

static void AnimThoughtBubbleStep(struct Sprite* sprite)
{
    if (--sprite->data[0] == 0)
    {
        StartSpriteAnim(sprite, sprite->data[1]);
		StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
        sprite->callback = RunStoredCallbackWhenAnimEnds;
    }
}

// Animates the Metronome's finger sprite.
// arg 0: anim battler
void AnimMetronomeFinger(struct Sprite* sprite)
{
    u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);
    
    SetSpriteNextToMonHead(battler, sprite);
	
    StoreSpriteCallbackInData6(sprite, AnimMetronomeFingerStep);
    sprite->callback = RunStoredCallbackWhenAffineAnimEnds;
}

static void AnimMetronomeFingerStep(struct Sprite* sprite)
{
    if (++sprite->data[0] > 16)
    {
        StartSpriteAffineAnim(sprite, 1);
        StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
        sprite->callback = RunStoredCallbackWhenAffineAnimEnds;
    }
}

// Animates Follow Me finger sprite.
// arg 0: anim battler
void AnimFollowMeFinger(struct Sprite* sprite)
{
    u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);

    sprite->x = GetBattlerSpriteCoord(battler, BATTLER_COORD_X);
    sprite->y = GetBattlerSpriteCoordAttr(battler, BATTLER_COORD_ATTR_TOP);
	
    if (sprite->y <= 9)
        sprite->y = 10;

    sprite->data[0] = 1;
    sprite->data[1] = 0;
    sprite->data[2] = sprite->subpriority;
    sprite->data[3] = sprite->subpriority + 4;
    sprite->data[4] = 0;
    StoreSpriteCallbackInData6(sprite, AnimFollowMeFingerStep1);
    sprite->callback = RunStoredCallbackWhenAffineAnimEnds;
}

static void AnimFollowMeFingerStep1(struct Sprite* sprite)
{
    if (++sprite->data[4] > 12)
        sprite->callback = AnimFollowMeFingerStep2;
}

static void AnimFollowMeFingerStep2(struct Sprite* sprite)
{
    s16 x1, x2;

    sprite->data[1] += 4;
	
    if (sprite->data[1] > 254)
    {
        if (--sprite->data[0] == 0)
        {
            sprite->x2 = 0;
            sprite->callback = AnimMetronomeFingerStep;
            return;
        }
        else
            sprite->data[1] &= 0xFF;
    }

    if (sprite->data[1] > 0x4F)
        sprite->subpriority = sprite->data[3];

    if (sprite->data[1] > 0x9F)
        sprite->subpriority = sprite->data[2];

    x1 = gSineTable[sprite->data[1]];
    x2 = x1 >> 3;
    sprite->x2 = (x1 >> 3) + (x2 >> 1);
}

// Applys an grayscale effect on the given tag.
// arg 0: anim tag
void AnimTask_GrayscaleParticle(u8 taskId)
{
	SetGreyscaleOrOriginalPalette(IndexOfSpritePaletteTag(gBattleAnimArgs[0]) + 16, FALSE);
	DestroyAnimVisualTask(taskId);
}

// Reloads the attacker's sprite after the Mega Evolution's flash ends.
// No args.
void AnimTask_MegaEvolutionUpdateAttackerSprite(u8 taskId)
{
	struct Task *newTask;
	
	switch (gTasks[taskId].data[0])
	{
		case 0:
			gTasks[taskId].data[1] = gSprites[gBattlerSpriteIds[gBattleAnimAttacker]].x;
			gSprites[gBattlerSpriteIds[gBattleAnimAttacker]].x = -64; // Put sprite offscreen
			LoadBattleMonGfxAndAnimate(gBattleAnimAttacker, TRUE, gBattlerSpriteIds[gBattleAnimAttacker]);
			++gTasks[taskId].data[0];
			break;
		case 1:
			gSprites[gBattlerSpriteIds[gBattleAnimAttacker]].invisible = FALSE;
			
			newTask = &gTasks[CreateTask(Task_ClearMonBg, 5)];
			newTask->data[0] = gBattleAnimAttacker;
			newTask->data[2] = gBattlerSpriteIds[gBattleAnimAttacker];
			
			++gTasks[taskId].data[0];
			break;
		case 2:
			if (!FuncIsActiveTask(Task_ClearMonBg))
				++gTasks[taskId].data[0];
			break;
		case 3:
			gSprites[gBattlerSpriteIds[gBattleAnimAttacker]].x = gTasks[taskId].data[1];
			DestroyAnimVisualTask(taskId);
			break;
	}
}

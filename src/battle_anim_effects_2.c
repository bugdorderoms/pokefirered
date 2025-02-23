#include "global.h"
#include "gflib.h"
#include "battle_anim.h"
#include "battle_interface.h"
#include "decompress.h"
#include "graphics.h"
#include "math_util.h"
#include "random.h"
#include "scanline_effect.h"
#include "trig.h"
#include "util.h"
#include "constants/songs.h"

static void AnimSwordsDanceBladeStep(struct Sprite *);
static void AnimSwordsDanceBladeStep2(struct Sprite *);
static void AnimCoinThrow(struct Sprite *sprite);
static void AnimFallingCoin(struct Sprite *sprite);
static void AnimFallingCoin_Step(struct Sprite *);
static void AnimViseGripPincer(struct Sprite *sprite);
static void AnimGuillotinePincer(struct Sprite *sprite);
static void AnimGuillotinePincerStep1(struct Sprite *);
static void AnimGuillotinePincerStep2(struct Sprite *);
static void AnimRazorWindTornado(struct Sprite *sprite);
static void AnimTask_GrowAndGreyscaleStep(u8 taskId);
static void AnimTask_MinimizeStep1(u8);
static void CreateMinimizeSprite(struct Task *, u8);
static void ClonedMinizeSprite_Step(struct Sprite *);
static void AnimTask_SplashStep(u8);
static void ThrashMoveMon(u8);
static void AnimTask_SketchDrawMon_Step(u8);
static void AnimPencil_Step(struct Sprite *);
static void AnimSoftBoiledEgg_Step1(struct Sprite *);
static void AnimSoftBoiledEgg_Step2(struct Sprite *);
static void AnimSoftBoiledEgg_Step3(struct Sprite *);
static void AnimSoftBoiledEgg_Step3_Callback1(struct Sprite *);
static void AnimSoftBoiledEgg_Step3_Callback2(struct Sprite *);
static void AnimSoftBoiledEgg_Step4(struct Sprite *);
static void StretchAttacker_Step(u8);
static void ExtremeSpeedImpact_Step(u8);
static void ExtremeSpeedMonReappear_Step(u8);
static void SpeedDust_Step1(u8);
static void FakeOutStep1(u8);
static void FakeOutStep2(u8);
static void AnimRedHeartProjectile_Step(struct Sprite *);
static void AnimRedHeartRising_Step(struct Sprite *);
static void ScaryFace_Step(u8);
static void AnimOrbitFastStep(struct Sprite *);
static void AnimOrbitScatterStep(struct Sprite *);
static void AnimMovementWaves_Step(struct Sprite *);
static void AnimJaggedMusicNote_Step(struct Sprite *);
static void AnimPerishSongMusicNote_Step(struct Sprite *);
static void AnimMaxFlutterbyStep(struct Sprite *sprite);

// Data
static const union AffineAnimCmd sSwordsDanceBladeAffineAnimCmds[] =
{
	AFFINEANIMCMD_FRAME(0, 16, 0, 0),
    AFFINEANIMCMD_FRAME(21, 16, 0, 12),
	AFFINEANIMCMD_FRAME(1, 16, 0, 4),
    AFFINEANIMCMD_FRAME(0, 0, 0, 28),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSwordsDanceBladeAffineAnimInclineToRightCmds[] =
{
    AFFINEANIMCMD_FRAME(0, 0, -6, 6),
	AFFINEANIMCMD_FRAME(0, 0, 0, 15),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSwordsDanceBladeAffineAnimInclineToLeftCmds[] =
{
    AFFINEANIMCMD_FRAME(0, 0, 6, 6),
	AFFINEANIMCMD_FRAME(0, 0, 0, 15),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSwordsDanceBladeAffineAnimInclineToRightWithReductionCmds[] =
{
    AFFINEANIMCMD_FRAME(-3, -3, -4, 6),
	AFFINEANIMCMD_FRAME(0, 0, 0, 15),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSwordsDanceBladeAffineAnimInclineToLeftWithReductionCmds[] =
{
    AFFINEANIMCMD_FRAME(-3, -3, 4, 6),
	AFFINEANIMCMD_FRAME(0, 0, 0, 15),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSwordsDanceBladeAffineAnimInclineToCenterCmds[] =
{
    AFFINEANIMCMD_FRAME(-3, -3, 0, 6),
	AFFINEANIMCMD_FRAME(0, 0, 0, 15),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sSwordsDanceBladeAffineAnimTable[] =
{
    sSwordsDanceBladeAffineAnimCmds,
	sSwordsDanceBladeAffineAnimInclineToCenterCmds,
	sSwordsDanceBladeAffineAnimInclineToRightCmds,
	sSwordsDanceBladeAffineAnimInclineToLeftCmds,
	sSwordsDanceBladeAffineAnimInclineToRightWithReductionCmds,
	sSwordsDanceBladeAffineAnimInclineToLeftWithReductionCmds,
};

const struct SpriteTemplate gSwordsDanceBladeSpriteTemplate =
{
    .tileTag = ANIM_TAG_SWORD,
    .paletteTag = ANIM_TAG_SWORD,
    .oam = &gOamData_AffineDouble_ObjBlend_32x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sSwordsDanceBladeAffineAnimTable,
    .callback = Anim_SwordsDanceBlade,
};

const struct SpriteTemplate gSonicBoomSpriteTemplate =
{
    .tileTag = ANIM_TAG_AIR_WAVE,
    .paletteTag = ANIM_TAG_AIR_WAVE,
    .oam = &gOamData_AffineDouble_ObjBlend_32x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSonicBoomProjectile,
};

static const union AffineAnimCmd sGrowingRingAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(32, 32, 0, 0),
    AFFINEANIMCMD_FRAME(7, 7, 0, -56),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sWaterPulseRingAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(5, 5, 0, 10),
    AFFINEANIMCMD_FRAME(-10, -10, 0, 10),
    AFFINEANIMCMD_FRAME(10, 10, 0, 10),
    AFFINEANIMCMD_FRAME(-10, -10, 0, 10),
    AFFINEANIMCMD_FRAME(10, 10, 0, 10),
    AFFINEANIMCMD_FRAME(-10, -10, 0, 10),
    AFFINEANIMCMD_FRAME(10, 10, 0, 10),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd *const gGrowingRingAffineAnimTable[] =
{
    sGrowingRingAffineAnimCmds,
};

static const union AffineAnimCmd *const sWaterPulseRingAffineAnimTable[] =    
{
    sWaterPulseRingAffineAnimCmds,
};

const struct SpriteTemplate gSupersonicWaveSpriteTemplate =    
{
    .tileTag = ANIM_TAG_GOLD_RING,
    .paletteTag = ANIM_TAG_GOLD_RING,
    .oam = &gOamData_AffineDouble_ObjNormal_16x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gGrowingRingAffineAnimTable,
    .callback = TranslateAnimSpriteToTargetMonLocation,
};

const struct SpriteTemplate gScreechWaveSpriteTemplate =    
{
    .tileTag = ANIM_TAG_PURPLE_RING,
    .paletteTag = ANIM_TAG_PURPLE_RING,
    .oam = &gOamData_AffineDouble_ObjNormal_16x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gGrowingRingAffineAnimTable,
    .callback = TranslateAnimSpriteToTargetMonLocation,
};

const struct SpriteTemplate gWaterPulseRingSpriteTemplate =    
{
    .tileTag = ANIM_TAG_BLUE_RING_2,
    .paletteTag = ANIM_TAG_BLUE_RING_2,
    .oam = &gOamData_AffineDouble_ObjNormal_16x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sWaterPulseRingAffineAnimTable,
    .callback = AnimWaterPulseRing,
};

const struct SpriteTemplate gEggThrowSpriteTemplate =    
{
    .tileTag = ANIM_TAG_LARGE_FRESH_EGG,
    .paletteTag = ANIM_TAG_LARGE_FRESH_EGG,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimThrowProjectile,
};

static const union AnimCmd sCoinAnimCmds[] =
{
    ANIMCMD_FRAME(8, 1),
    ANIMCMD_END,
};

static const union AnimCmd *const sCoinAnimTable[] =
{
    sCoinAnimCmds,
};

const struct SpriteTemplate gCoinThrowSpriteTemplate =    
{
    .tileTag = ANIM_TAG_COIN,
    .paletteTag = ANIM_TAG_COIN,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = sCoinAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimCoinThrow,
};

const struct SpriteTemplate gFallingCoinSpriteTemplate =
{
    .tileTag = ANIM_TAG_COIN,
    .paletteTag = ANIM_TAG_COIN,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = sCoinAnimTable,
    .images = NULL,
    .affineAnims = gAffineAnims_ShadowBall,
    .callback = AnimFallingCoin,
};

static const union AffineAnimCmd sRazorWindTornadoAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(16, 256, 0, 0),
    AFFINEANIMCMD_FRAME(4, 0, 0, 40),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd *const sRazorWindTornadoAffineAnimTable[] =
{
    sRazorWindTornadoAffineAnimCmds,
};

const struct SpriteTemplate gRazorWindTornadoSpriteTemplate =
{
    .tileTag = ANIM_TAG_GUST,
    .paletteTag = ANIM_TAG_GUST,
    .oam = &gOamData_AffineNormal_ObjNormal_32x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sRazorWindTornadoAffineAnimTable,
    .callback = AnimRazorWindTornado,
};

static const union AnimCmd sViseGripAnimCmds1[] =
{
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_FRAME(16, 3),
    ANIMCMD_FRAME(32, 20),
    ANIMCMD_END,
};

static const union AnimCmd sViseGripAnimCmds2[] =
{
    ANIMCMD_FRAME(0, 3, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 3, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 20, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd *const sViseGripAnimTable[] =
{
    sViseGripAnimCmds1,
    sViseGripAnimCmds2,
};

const struct SpriteTemplate gViseGripSpriteTemplate =
{
    .tileTag = ANIM_TAG_CUT,
    .paletteTag = ANIM_TAG_CUT,
    .oam = &gOamData_AffineOff_ObjBlend_32x32,
    .anims = sViseGripAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimViseGripPincer,
};

static const union AnimCmd sGuillotineAnimCmds1[] =
{
    ANIMCMD_FRAME(0, 2),
    ANIMCMD_FRAME(16, 2),
    ANIMCMD_FRAME(32, 1),
    ANIMCMD_END,
};

static const union AnimCmd sGuillotineAnimCmds2[] =
{
    ANIMCMD_FRAME(0, 2, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 2, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 1, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd *const sGuillotineAnimTable[] =
{
    sGuillotineAnimCmds1,
    sGuillotineAnimCmds2,
};

const struct SpriteTemplate gGuillotineSpriteTemplate =
{
    .tileTag = ANIM_TAG_CUT,
    .paletteTag = ANIM_TAG_CUT,
    .oam = &gOamData_AffineOff_ObjBlend_32x32,
    .anims = sGuillotineAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimGuillotinePincer,
};

static const union AffineAnimCmd sSplashEffectAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(-6, 4, 0, 8),
    AFFINEANIMCMD_FRAME(10, -10, 0, 8),
    AFFINEANIMCMD_FRAME(-4, 6, 0, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sGrowAndShrinkAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(-4, -5, 0, 12),
    AFFINEANIMCMD_FRAME(0, 0, 0, 24),
    AFFINEANIMCMD_FRAME(4, 5, 0, 12),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAngerMarkAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(11, 11, 0, 8),
    AFFINEANIMCMD_FRAME(-11, -11, 0, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAngerMarkAffineAnimTable[] =
{
    sAngerMarkAffineAnimCmds,
};

const struct SpriteTemplate gAngerMarkSpriteTemplate =
{
    .tileTag = ANIM_TAG_ANGER,
    .paletteTag = ANIM_TAG_ANGER,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAngerMarkAffineAnimTable,
    .callback = AnimAngerMark,
};

static const union AffineAnimCmd sThrashMoveMonAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(-10, 9, 0, 7),
    AFFINEANIMCMD_FRAME(20, -20, 0, 7),
    AFFINEANIMCMD_FRAME(-20, 20, 0, 7),
    AFFINEANIMCMD_FRAME(10, -9, 0, 7),
    AFFINEANIMCMD_LOOP(2),
    AFFINEANIMCMD_END,
};

const struct SpriteTemplate gPencilSpriteTemplate =
{
    .tileTag = ANIM_TAG_PENCIL,
    .paletteTag = ANIM_TAG_PENCIL,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimPencil,
};

const struct SpriteTemplate gSnoreZSpriteTemplate =    
{
    .tileTag = ANIM_TAG_SNORE_Z,
    .paletteTag = ANIM_TAG_SNORE_Z,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimTravelDiagonally,
};

static const union AnimCmd sExplosionAnimCmds[] =
{
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_FRAME(16, 5),
    ANIMCMD_FRAME(32, 5),
    ANIMCMD_FRAME(48, 5),
    ANIMCMD_END,
};

static const union AnimCmd *const sExplosionAnimTable[] =
{
    sExplosionAnimCmds,
};

const struct SpriteTemplate gExplosionSpriteTemplate =    
{
    .tileTag = ANIM_TAG_EXPLOSION,
    .paletteTag = ANIM_TAG_EXPLOSION,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sExplosionAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSpriteOnMonPos,
};

static const union AffineAnimCmd sSoftBoiledEggAffineAnimCmds1[] =
{
    AFFINEANIMCMD_FRAME(0, 0, -8, 2),
    AFFINEANIMCMD_FRAME(0, 0, 8, 4),
    AFFINEANIMCMD_FRAME(0, 0, -8, 2),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sSoftBoiledEggAffineAnimCmds2[] =
{
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSoftBoiledEggAffineAnimCmds3[] =
{
    AFFINEANIMCMD_FRAME(-8, 4, 0, 8),
    AFFINEANIMCMD_LOOP(0),
    AFFINEANIMCMD_FRAME(16, -8, 0, 8),
    AFFINEANIMCMD_FRAME(-16, 8, 0, 8),
    AFFINEANIMCMD_LOOP(1),
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_FRAME(0, 0, 0, 15),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sSoftBoiledEggAffineAnimTable[] =
{
    sSoftBoiledEggAffineAnimCmds1,
    sSoftBoiledEggAffineAnimCmds2,
    sSoftBoiledEggAffineAnimCmds3,
};

const struct SpriteTemplate gSoftBoiledEggSpriteTemplate =    
{
    .tileTag = ANIM_TAG_BREAKING_EGG,
    .paletteTag = ANIM_TAG_BREAKING_EGG,
    .oam = &gOamData_AffineDouble_ObjBlend_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sSoftBoiledEggAffineAnimTable,
    .callback = AnimSoftBoiledEgg,
};

static const union AffineAnimCmd sThinRingExpandingAffineAnimCmds1[] =
{
    AFFINEANIMCMD_FRAME(16, 16, 0, 0),
    AFFINEANIMCMD_FRAME(16, 16, 0, 30),
    AFFINEANIMCMD_END_ALT(1),
};

static const union AffineAnimCmd sThinRingExpandingAffineAnimCmds2[] =
{
    AFFINEANIMCMD_FRAME(16, 16, 0, 0),
    AFFINEANIMCMD_FRAME(32, 32, 0, 15),
    AFFINEANIMCMD_END_ALT(1),
};

static const union AffineAnimCmd sHyperVoiceRingAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(16, 16, 0, 0),
    AFFINEANIMCMD_FRAME(11, 11, 0, 45),
    AFFINEANIMCMD_END_ALT(1),
};

static const union AffineAnimCmd *const sThinRingExpandingAffineAnimTable[] =
{
    sThinRingExpandingAffineAnimCmds1,
    sThinRingExpandingAffineAnimCmds2,
};

static const union AffineAnimCmd *const sHyperVoiceRingAffineAnimTable[] =
{
    sHyperVoiceRingAffineAnimCmds,
};

const struct SpriteTemplate gThinRingExpandingSpriteTemplate =
{
    .tileTag = ANIM_TAG_THIN_RING,
    .paletteTag = ANIM_TAG_THIN_RING,
    .oam = &gOamData_AffineDouble_ObjNormal_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sThinRingExpandingAffineAnimTable,
    .callback = AnimSpriteOnMonPos,
};

static const union AffineAnimCmd sThinRingShrinkingAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(512, 512, 0, 0),
    AFFINEANIMCMD_FRAME(-16, -16, 0, 30),
    AFFINEANIMCMD_END_ALT(1),
};

static const union AffineAnimCmd *const sThinRingShrinkingAffineAnimTable[] =
{
    sThinRingShrinkingAffineAnimCmds,
};

const struct SpriteTemplate gThinRingShrinkingSpriteTemplate =
{
    .tileTag = ANIM_TAG_THIN_RING,
    .paletteTag = ANIM_TAG_THIN_RING,
    .oam = &gOamData_AffineDouble_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sThinRingShrinkingAffineAnimTable,
    .callback = AnimSpriteOnMonPos,
};

const struct SpriteTemplate gBlendThinRingExpandingSpriteTemplate =
{
    .tileTag = ANIM_TAG_THIN_RING,
    .paletteTag = ANIM_TAG_THIN_RING,
    .oam = &gOamData_AffineDouble_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sThinRingExpandingAffineAnimTable,
    .callback = AnimBlendThinRing,
};

const struct SpriteTemplate gHyperVoiceRingSpriteTemplate =    
{
    .tileTag = ANIM_TAG_THIN_RING,
    .paletteTag = ANIM_TAG_THIN_RING,
    .oam = &gOamData_AffineDouble_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sHyperVoiceRingAffineAnimTable,
    .callback = AnimHyperVoiceRing,
};

const struct SpriteTemplate gUproarRingSpriteTemplate =    
{
    .tileTag = ANIM_TAG_THIN_RING,
    .paletteTag = ANIM_TAG_THIN_RING,
    .oam = &gOamData_AffineDouble_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sThinRingExpandingAffineAnimTable,
    .callback = AnimUproarRing,
};

const struct SpriteTemplate gSilverThinRingSpriteTemplate =    
{
    .tileTag = ANIM_TAG_THIN_RING,
    .paletteTag = ANIM_TAG_GUST,
    .oam = &gOamData_AffineDouble_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sThinRingExpandingAffineAnimTable,
    .callback = AnimUproarRing,
};

const struct SpriteTemplate gPinkThinRingSpriteTemplate =
{
    .tileTag = ANIM_TAG_THIN_RING,
    .paletteTag = ANIM_TAG_PINK_PETAL,
    .oam = &gOamData_AffineDouble_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sThinRingExpandingAffineAnimTable,
    .callback = AnimUproarRing,
};

static const union AffineAnimCmd sStretchAttackerAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(96, -13, 0, 8),
    AFFINEANIMCMD_END,
};

static const union AnimCmd sSpeedDustAnimCmds[] =
{
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_FRAME(4, 3),
    ANIMCMD_FRAME(8, 3),
    ANIMCMD_FRAME(4, 3),
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_END,
};

static const union AnimCmd *const sSpeedDustAnimTable[] =
{
    sSpeedDustAnimCmds,
};

const struct SpriteTemplate gSpeedDustSpriteTemplate =
{
    .tileTag = ANIM_TAG_SPEED_DUST,
    .paletteTag = ANIM_TAG_SPEED_DUST,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = sSpeedDustAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSpeedDust,
};

static const s8 sSpeedDustPosTable[][2] =
{
    {30, 28},
    {-20, 24},
    {16, 26},
    {-10, 28},
};

static const union AnimCmd sBellAnimCmds[] =
{
    ANIMCMD_FRAME(0, 6),
    ANIMCMD_FRAME(16, 6),
    ANIMCMD_FRAME(32, 15),
    ANIMCMD_FRAME(16, 6),
    ANIMCMD_FRAME(0, 6),
    ANIMCMD_FRAME(16, 6, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 15, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 6, .hFlip = TRUE),
    ANIMCMD_FRAME(0, 6),
    ANIMCMD_FRAME(16, 6),
    ANIMCMD_FRAME(32, 15),
    ANIMCMD_FRAME(16, 6),
    ANIMCMD_FRAME(0, 6),
    ANIMCMD_END,
};

static const union AnimCmd *const sBellAnimTable[] =
{
    sBellAnimCmds,
};

const struct SpriteTemplate gBellSpriteTemplate =
{
    .tileTag = ANIM_TAG_BELL,
    .paletteTag = ANIM_TAG_BELL,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sBellAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSpriteOnMonPos,
};

const u16 gMusicNotePaletteTagsTable[] =
{
    ANIM_TAG_MUSIC_NOTES_2,
    ANIM_SPRITES_START - 1,
    ANIM_SPRITES_START - 2,
};

const struct SpriteTemplate gHealBellMusicNoteSpriteTemplate =
{
    .tileTag = ANIM_TAG_MUSIC_NOTES_2,
    .paletteTag = ANIM_TAG_MUSIC_NOTES_2,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimHealBellMusicNote,
};

static const union AffineAnimCmd sUnknown_83E4200[] =
{
    AFFINEANIMCMD_FRAME(10, -13, 0, 10),
    AFFINEANIMCMD_FRAME(-10, 13, 0, 10),
    AFFINEANIMCMD_END,
};

const struct SpriteTemplate gRedHeartProjectileSpriteTemplate =
{
    .tileTag = ANIM_TAG_RED_HEART,
    .paletteTag = ANIM_TAG_RED_HEART,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimRedHeartProjectile,
};

const struct SpriteTemplate gRedHeartRisingSpriteTemplate =    
{
    .tileTag = ANIM_TAG_RED_HEART,
    .paletteTag = ANIM_TAG_RED_HEART,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimRedHeartRising,
};

static const union AffineAnimCmd sHiddenPowerOrbAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(128, 128, 0, 0),
    AFFINEANIMCMD_FRAME(8, 8, 0, 1),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd *const sHiddenPowerOrbAffineAnimTable[] =
{
    sHiddenPowerOrbAffineAnimCmds,
};

const struct SpriteTemplate gHiddenPowerOrbSpriteTemplate =    
{
    .tileTag = ANIM_TAG_RED_ORB,
    .paletteTag = ANIM_TAG_RED_ORB,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sHiddenPowerOrbAffineAnimTable,
    .callback = AnimOrbitFast,
};

const struct SpriteTemplate gHiddenPowerOrbScatterSpriteTemplate =
{
    .tileTag = ANIM_TAG_RED_ORB,
    .paletteTag = ANIM_TAG_RED_ORB,
    .oam = &gOamData_AffineDouble_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sHiddenPowerOrbAffineAnimTable,
    .callback = AnimOrbitScatter,
};

static const union AffineAnimCmd sSpitUpOrbAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(128, 128, 0, 0),
    AFFINEANIMCMD_FRAME(8, 8, 0, 1),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd *const sSpitUpOrbAffineAnimTable[] =
{
    sSpitUpOrbAffineAnimCmds,
};

const struct SpriteTemplate gSpitUpOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_RED_ORB_2,
    .paletteTag = ANIM_TAG_RED_ORB_2,
    .oam = &gOamData_AffineDouble_ObjNormal_8x8,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sSpitUpOrbAffineAnimTable,
    .callback = AnimOrbitScatter,
};

static const union AnimCmd sEyeSparkleAnimCmds[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(4, 4),
    ANIMCMD_FRAME(8, 4),
    ANIMCMD_FRAME(4, 4),
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_END,
};

static const union AnimCmd *const sEyeSparkleAnimTable[] =
{
    sEyeSparkleAnimCmds,
};

const struct SpriteTemplate gEyeSparkleSpriteTemplate =
{
    .tileTag = ANIM_TAG_EYE_SPARKLE,
    .paletteTag = ANIM_TAG_EYE_SPARKLE,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = sEyeSparkleAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimEyeSparkle,
};

static const union AnimCmd sFurySwipesAnimCmd1[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(16, 4),
    ANIMCMD_FRAME(32, 4),
    ANIMCMD_FRAME(48, 4),
    ANIMCMD_END,
};

static const union AnimCmd sFurySwipesAnimCmd2[] =
{
    ANIMCMD_FRAME(0, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(48, 4, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd *const sFurySwipesAnimTable[] =
{
    sFurySwipesAnimCmd1,
    sFurySwipesAnimCmd2,
};

const struct SpriteTemplate gFurySwipesSpriteTemplate =
{
    .tileTag = ANIM_TAG_SWIPE,
    .paletteTag = ANIM_TAG_SWIPE,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sFurySwipesAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimFurySwipes,
};

static const union AnimCmd sMovementWavesAnimCmds1[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(16, 8),
    ANIMCMD_FRAME(32, 8),
    ANIMCMD_FRAME(16, 8),
    ANIMCMD_END,
};

static const union AnimCmd sMovementWavesAnimCmds2[] =
{
    ANIMCMD_FRAME(16, 8, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 8, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 8, .hFlip = TRUE),
    ANIMCMD_FRAME(0, 8, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd *const sMovementWavesAnimTable[] =
{
    sMovementWavesAnimCmds1,
    sMovementWavesAnimCmds2,
};

const struct SpriteTemplate gMovementWavesSpriteTemplate =
{
    .tileTag = ANIM_TAG_MOVEMENT_WAVES,
    .paletteTag = ANIM_TAG_MOVEMENT_WAVES,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sMovementWavesAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimMovementWaves,
};

static const union AffineAnimCmd sUproarAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(-12, 8, 0, 4),
    AFFINEANIMCMD_FRAME(20, -20, 0, 4),
    AFFINEANIMCMD_FRAME(-8, 12, 0, 4),
    AFFINEANIMCMD_END,
};

const struct SpriteTemplate gJaggedMusicNoteSpriteTemplate =
{
    .tileTag = ANIM_TAG_JAGGED_MUSIC_NOTE,
    .paletteTag = ANIM_TAG_JAGGED_MUSIC_NOTE,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimJaggedMusicNote,
};

static const union AffineAnimCmd sPerishSongMusicNoteAffineAnimCmds1[] =
{
    AFFINEANIMCMD_FRAME(0, 0, 0, 5),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sPerishSongMusicNoteAffineAnimCmds2[] =
{
    AFFINEANIMCMD_FRAME(0, 0, -8, 16),
    AFFINEANIMCMD_END_ALT(1),
};

static const union AffineAnimCmd sPerishSongMusicNoteAffineAnimCmds3[] =
{
    AFFINEANIMCMD_FRAME(0, 0, 8, 16),
    AFFINEANIMCMD_END_ALT(1),
};

static const union AffineAnimCmd *const sPerishSongMusicNoteAffineAnimTable[] =
{
    sPerishSongMusicNoteAffineAnimCmds1,
    sPerishSongMusicNoteAffineAnimCmds2,
    sPerishSongMusicNoteAffineAnimCmds3,
};

const struct SpriteTemplate gPerishSongMusicNoteSpriteTemplate =
{
    .tileTag = ANIM_TAG_MUSIC_NOTES_2,
    .paletteTag = ANIM_TAG_MUSIC_NOTES_2,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gMusicNotesAnimTable,
    .images = NULL,
    .affineAnims = sPerishSongMusicNoteAffineAnimTable,
    .callback = AnimPerishSongMusicNote,
};

const struct SpriteTemplate gPerishSongMusicNote2SpriteTemplate =
{
    .tileTag = ANIM_TAG_MUSIC_NOTES_2,
    .paletteTag = ANIM_TAG_MUSIC_NOTES_2,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gMusicNotesAnimTable,
    .images = NULL,
    .affineAnims = sPerishSongMusicNoteAffineAnimTable,
    .callback = AnimPerishSongMusicNote2,
};

static const union AffineAnimCmd sGuardRingAffineAnimCmds1[] =
{
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sGuardRingAffineAnimCmds2[] =
{
    AFFINEANIMCMD_FRAME(512, 256, 0, 0),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sGuardRingAffineAnimTable[] =
{
    sGuardRingAffineAnimCmds1,
    sGuardRingAffineAnimCmds2,
};

const struct SpriteTemplate gGuardRingSpriteTemplate =
{
    .tileTag = ANIM_TAG_GUARD_RING,
    .paletteTag = ANIM_TAG_GUARD_RING,
    .oam = &gOamData_AffineDouble_ObjBlend_64x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sGuardRingAffineAnimTable,
    .callback = AnimGuardRing,
};

const struct SpriteTemplate gBlueGuardRingSpriteTemplate =
{
    .tileTag = ANIM_TAG_GUARD_RING,
    .paletteTag = ANIM_TAG_WATER_ORB,
    .oam = &gOamData_AffineDouble_ObjBlend_64x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sGuardRingAffineAnimTable,
    .callback = AnimGuardRing,
};

const struct SpriteTemplate gPowerGemGemSpriteTemplate =
{
    .tileTag = ANIM_TAG_POWER_GEM,
    .paletteTag = ANIM_TAG_POWER_GEM,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gAffineAnims_ShadowBall,
    .callback = AnimDirtScatter,
};

static const union AffineAnimCmd sSpriteAffineAnim_MaxFlutterbyPulsate[] =
{
	AFFINEANIMCMD_FRAME(16, 16, 0, 4),
	AFFINEANIMCMD_FRAME(-16, -16, 0, 4),
	AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sSpriteAffineAnim_MaxFlutterbyGrow[] =
{
	AFFINEANIMCMD_FRAME(8, 8, 0, 16), // Double in size
	AFFINEANIMCMD_END,
};

const union AffineAnimCmd* const gAffineAnimTable_MaxFlutterby[] =
{
	sSpriteAffineAnim_MaxFlutterbyPulsate,
	sSpriteAffineAnim_MaxFlutterbyGrow,
};

// Animates a sword that rises into the air after a brief pause. And then do the given affine anim.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: affine anim id
// arg 3: priority mod
void Anim_SwordsDanceBlade(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, FALSE);
	sprite->oam.priority = GetBattlerSpriteBGPriority(gBattleAnimAttacker) + gBattleAnimArgs[3];
	sprite->data[5] = gBattleAnimArgs[2];
    sprite->callback = RunStoredCallbackWhenAffineAnimEnds;
    StoreSpriteCallbackInData6(sprite, AnimSwordsDanceBladeStep);
}

static void AnimSwordsDanceBladeStep(struct Sprite *sprite)
{
    sprite->data[0] = 6;
    sprite->data[2] = sprite->x;
    sprite->data[4] = sprite->y - 32;
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, AnimSwordsDanceBladeStep2);
}

static void AnimSwordsDanceBladeStep2(struct Sprite *sprite)
{
	StartSpriteAffineAnim(sprite, sprite->data[5]);
	sprite->callback = RunStoredCallbackWhenAffineAnimEnds;
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Moves a projectile towards the target mon. The sprite is rotated to be pointing in the same direction it's moving.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: target x pixel offset
// arg 3: target y pixel offset
// arg 4: duration
void AnimSonicBoomProjectile(struct Sprite *sprite)
{
    s16 targetXPos, targetYPos;
    u16 rotation;

    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
    {
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];
        gBattleAnimArgs[1] = -gBattleAnimArgs[1];
        gBattleAnimArgs[3] = -gBattleAnimArgs[3];
    }
    InitSpritePosToAnimAttacker(sprite, TRUE);
	
    targetXPos = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X) + gBattleAnimArgs[2];
    targetYPos = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3];
    
	rotation = ArcTan2Neg(targetXPos - sprite->x, targetYPos - sprite->y);
    rotation += 0xF000;
    TrySetSpriteRotScale(sprite, FALSE, 0x100, 0x100, rotation);
	
    sprite->data[0] = gBattleAnimArgs[4];
    sprite->data[2] = targetXPos;
    sprite->data[4] = targetYPos;
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Animates the coin sprite translating to the target position.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: final x pixel offset
// arg 3: final y pixel offset
// arg 4: duration
static void AnimCoinThrow(struct Sprite *sprite)
{
    s16 r6;
    s16 r7;
    u16 var;

    InitSpritePosToAnimAttacker(sprite, TRUE);
	
    r6 = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    r7 = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3];
	
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];

    r6 += gBattleAnimArgs[2];
    var = ArcTan2Neg(r6 - sprite->x, r7 - sprite->y);
    var += 0xC000;
    TrySetSpriteRotScale(sprite, FALSE, 0x100, 0x100, var);
	
    sprite->data[0] = gBattleAnimArgs[4];
    sprite->data[2] = r6;
    sprite->data[4] = r7;
    sprite->callback = BattleAnim_InitAndRunLinearTranslationWithDuration;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Animates the falling coin sprite on MOVE_PAY_DAY.
// No args.
static void AnimFallingCoin(struct Sprite *sprite)
{
    sprite->data[2] = -16;
    sprite->y += 8;
    sprite->callback = AnimFallingCoin_Step;
}

static void AnimFallingCoin_Step(struct Sprite *sprite)
{
    sprite->data[0] += 0x80;
	
    sprite->x2 = sprite->data[0] >> 8;
	
    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
        sprite->x2 = -sprite->x2;

    sprite->y2 = Sin(sprite->data[1], sprite->data[2]);
	
    sprite->data[1] += 5;
	
    if (sprite->data[1] > 126)
    {
        sprite->data[1] = 0;
        sprite->data[2] /= 2;
		
        if (++sprite->data[3] == 2)
            DestroyAnimSprite(sprite);
    }
}

// Moves a tornado in a circlular motion.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: wave amplitude
// arg 3: initial wave offset
// arg 4: wave period (higher means faster wave)
// arg 5: duration
static void AnimRazorWindTornado(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, FALSE);
	
    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
        sprite->y += 16;

    sprite->data[0] = gBattleAnimArgs[3];
    sprite->data[1] = gBattleAnimArgs[2];
    sprite->data[2] = gBattleAnimArgs[4];
    sprite->data[3] = gBattleAnimArgs[5];
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
    sprite->callback = TranslateSpriteInCircleOverDuration;
    sprite->callback(sprite);
}

// Animates a single pincer line that extends towards the center of the target mon.
// arg 0: invert
static void AnimViseGripPincer(struct Sprite *sprite)
{
    s16 startXOffset = 32;
    s16 startYOffset = -32;
    s16 endXOffset = 16;
    s16 endYOffset = -16;
    
    if (gBattleAnimArgs[0])
    {
        startXOffset = -startXOffset;
        startYOffset = -startYOffset;
        endXOffset = -endXOffset;
        endYOffset = -endYOffset;
        StartSpriteAnim(sprite, 1);
    }
    sprite->x += startXOffset;
    sprite->y += startYOffset;
    sprite->data[0] = 6;
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X) + endXOffset;
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + endYOffset;
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSpriteWhenAnimEnds);
}

// Animates a single pincer line that extends towards the center of the target mon, and then back out.
// arg 0: animation id
static void AnimGuillotinePincer(struct Sprite *sprite)
{
    s16 startXOffset = 32;
    s16 startYOffset = -32;
    s16 endXOffset = 16;
    s16 endYOffset = -16;
    
    if (gBattleAnimArgs[0])
    {
        startXOffset = -startXOffset;
        startYOffset = -startYOffset;
        endXOffset = -endXOffset;
        endYOffset = -endYOffset;
        StartSpriteAnim(sprite, 1);
    }
    sprite->x += startXOffset;
    sprite->y += startYOffset;
    sprite->data[0] = 6;
    sprite->data[1] = sprite->x;
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X) + endXOffset;
    sprite->data[3] = sprite->y;
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + endYOffset;
    InitAnimLinearTranslation(sprite);
    sprite->data[5] = gBattleAnimArgs[0];
    sprite->data[6] = sprite->data[0];
    sprite->callback = AnimGuillotinePincerStep1;
}

static void AnimGuillotinePincerStep1(struct Sprite *sprite)
{
    if (AnimTranslateLinear(sprite) && sprite->animEnded)
    {
        SeekSpriteAnim(sprite, 0);
        sprite->animPaused = TRUE;
		
		SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
        sprite->x2 = 2;
        sprite->y2 = -2;
		
        sprite->data[0] = sprite->data[6];
        sprite->data[1] ^= 1;
        sprite->data[2] ^= 1;
        sprite->data[4] = 0;
        sprite->data[3] = 0;
        sprite->callback = AnimGuillotinePincerStep2;
    }
}

static void AnimGuillotinePincerStep2(struct Sprite *sprite)
{
    if (sprite->data[3])
    {
        sprite->x2 = -sprite->x2;
        sprite->y2 = -sprite->y2;
    }
    sprite->data[3] ^= 1;
	
    if (++sprite->data[4] == 51)
    {
        sprite->y2 = 0;
        sprite->x2 = 0;
        sprite->data[4] = 0;
        sprite->data[3] = 0;
        sprite->animPaused = FALSE;
        StartSpriteAnim(sprite, sprite->data[5] ^ TRUE);
        sprite->callback = DestroyAnimSpriteWhenLinearTranslationEnds;
    }
}

// Scales up the target mon sprite, and sets the palette to greyscale. Used in MOVE_DISABLE.
// arg 0: anim battler
// arg 1: duration
void AnimTask_GrowAndGreyscale(u8 taskId)
{
    u8 spriteId = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);
    
    PrepareBattlerSpriteForRotScale(spriteId, ST_OAM_OBJ_BLEND);
    SetSpriteRotScale(spriteId, 0xD0, 0xD0, 0);
    SetGreyscaleOrOriginalPalette(gSprites[spriteId].oam.paletteNum + 16, FALSE);
	
	gTasks[taskId].data[0] = gBattleAnimArgs[1];
	gTasks[taskId].data[1] = spriteId;
    gTasks[taskId].func = AnimTask_GrowAndGreyscaleStep;
}

static void AnimTask_GrowAndGreyscaleStep(u8 taskId)
{
    if (--gTasks[taskId].data[0] == -1)
    {
        ResetSpriteRotScale(gTasks[taskId].data[1]);
        SetGreyscaleOrOriginalPalette(gSprites[gTasks[taskId].data[1]].oam.paletteNum + 16, TRUE);
        DestroyAnimVisualTask(taskId);
    }
}

// Shrinks and grows the attacking mon several times. Also creates transparent versions of the mon's sprite while it is shrinking.
// No args.
void AnimTask_Minimize(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    u8 spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);
    
    task->data[0] = spriteId;
    PrepareBattlerSpriteForRotScale(spriteId, ST_OAM_OBJ_NORMAL);
    task->data[1] = 0;
    task->data[2] = 0;
    task->data[3] = 0;
    task->data[4] = 0x100;
    task->data[5] = 0;
    task->data[6] = 0;
    task->data[7] = GetBattlerSpriteSubpriority(gBattleAnimAttacker);
    task->func = AnimTask_MinimizeStep1;
}

static void AnimTask_MinimizeStep1(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    
    switch (task->data[1])
    {
    case 0:
        if (task->data[2] == 0 || task->data[2] == 3 || task->data[2] == 6)
            CreateMinimizeSprite(task, taskId);
		
        task->data[2]++;
		
        task->data[4] += 0x28;
        SetSpriteRotScale(task->data[0], task->data[4], task->data[4], 0);
        SetBattlerSpriteYOffsetFromYScale(task->data[0]);
		
        if (task->data[2] == 32)
        {
            task->data[5]++;
            task->data[1]++;
        }
        break;
    case 1:
        if (task->data[6] == 0)
        {
            if (task->data[5] == 3)
            {
                task->data[2] = 0;
                task->data[1] = 3;
            }
            else
            {
                task->data[2] = 0;
                task->data[3] = 0;
                task->data[4] = 0x100;
                SetSpriteRotScale(task->data[0], task->data[4], task->data[4], 0);
                SetBattlerSpriteYOffsetFromYScale(task->data[0]);
				
                task->data[1] = 2;
            }
        }
        break;
    case 2:
        task->data[1] = 0;
        break;
    case 3:
        if (++task->data[2] > 32)
        {
            task->data[2] = 0;
            task->data[1]++;
        }
        break;
    case 4:
        task->data[2] += 2;
        task->data[4] -= 0x50;
        SetSpriteRotScale(task->data[0], task->data[4], task->data[4], 0);
        SetBattlerSpriteYOffsetFromYScale(task->data[0]);
		
        if (task->data[2] == 32)
        {
            task->data[2] = 0;
            task->data[1]++;
        }
        break;
    case 5:
        ResetSpriteRotScale(task->data[0]);
        gSprites[task->data[15]].y2 = 0;
        DestroyAnimVisualTask(taskId);
        break;
    }
}

static void CreateMinimizeSprite(struct Task* task, u8 taskId)
{
    u16 matrixNum;
    s16 spriteId = CloneBattlerSpriteWithBlend(ANIM_ATTACKER);
    
    if (spriteId >= 0)
    {
        if ((matrixNum = AllocOamMatrix()) == 0xFF)
            DestroySpriteWithActiveSheet(&gSprites[spriteId]);
        else
        {
            gSprites[spriteId].oam.objMode = ST_OAM_OBJ_BLEND;
            gSprites[spriteId].oam.affineMode = ST_OAM_AFFINE_DOUBLE;
            gSprites[spriteId].affineAnimPaused = TRUE;
            gSprites[spriteId].oam.matrixNum = matrixNum;
            gSprites[spriteId].subpriority = task->data[7] - task->data[3];
            gSprites[spriteId].data[0] = 16;
            gSprites[spriteId].data[1] = taskId;
            gSprites[spriteId].data[2] = 6;
            gSprites[spriteId].callback = ClonedMinizeSprite_Step;
            SetSpriteRotScale(spriteId, task->data[4], task->data[4], 0);
            gSprites[spriteId].oam.affineMode = ST_OAM_AFFINE_NORMAL;
            CalcCenterToCornerVec(&gSprites[spriteId], gSprites[spriteId].oam.shape, gSprites[spriteId].oam.size, gSprites[spriteId].oam.affineMode);
			
			task->data[3]++;
            task->data[6]++;
        }
    }
}

static void ClonedMinizeSprite_Step(struct Sprite *sprite)
{
    if (--sprite->data[0] == 0)
    {
        gTasks[sprite->data[1]].data[sprite->data[2]]--;
        FreeSpriteOamMatrix(sprite);
        DestroySpriteWithActiveSheet(sprite);
    }
}

// Task to facilitate expanding and hopping effect seen in Splash.
// arg 0: anim battler
// arg 1: num hops
void AnimTask_Splash(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    
    if (gBattleAnimArgs[1] == 0)
        DestroyAnimVisualTask(taskId);
    else
    {
        task->data[2] = gBattleAnimArgs[1];
        PrepareAffineAnimInTaskData(task, GetAnimBattlerSpriteId(gBattleAnimArgs[0]), sSplashEffectAffineAnimCmds);
        task->func = AnimTask_SplashStep;
    }
}

static void AnimTask_SplashStep(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    
    switch (task->data[1])
    {
    case 0:
        RunAffineAnimFromTaskData(task);
		
        task->data[4] += 3;
        gSprites[task->data[15]].y2 += task->data[4];
		
        if (++task->data[3] > 7)
        {
            task->data[3] = 0;
            task->data[1]++;
        }
        break;
    case 1:
        RunAffineAnimFromTaskData(task);
		
        gSprites[task->data[15]].y2 += task->data[4];
		
        if (++task->data[3] > 7)
        {
            task->data[3] = 0;
            task->data[1]++;
        }
        break;
    case 2:
        if (task->data[4] != 0)
        {
            gSprites[task->data[15]].y2 -= 2;
            task->data[4] -= 2;
        }
        else
            task->data[1]++;
        break;
    case 3:
        if (!RunAffineAnimFromTaskData(task))
        {
            if (--task->data[2] == 0)
            {
                gSprites[task->data[15]].y2 = 0;
                DestroyAnimVisualTask(taskId);
            }
            else
            {
                PrepareAffineAnimInTaskData(task, task->data[15], sSplashEffectAffineAnimCmds);
                task->data[1] = 0;
            }
        }
        break;
    }
}

// Grows, pauses, then shrinks the attacking mon. Used by MOVE_SWAGGER and MOVE_BULK_UP.
// No args.
void AnimTask_GrowAndShrink(u8 taskId)
{
    PrepareAffineAnimInTaskData(&gTasks[taskId], GetAnimBattlerSpriteId(ANIM_ATTACKER), sGrowAndShrinkAffineAnimCmds);
    gTasks[taskId].func = AnimTask_DestroyTaskAfterAffineAnimFromTaskDataEnds;
}

// Animates an "angry" mark above a mon's head.
// arg 0: anim battler
// arg 1: x pixel offset
// arg 2: y pixel offset
void AnimAngerMark(struct Sprite *sprite)
{
    u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);
    
    if (GetBattlerSide(battler) == B_SIDE_OPPONENT)
        gBattleAnimArgs[1] = -gBattleAnimArgs[1];

    sprite->x = GetBattlerSpriteCoord(battler, BATTLER_COORD_X) + gBattleAnimArgs[1];
    sprite->y = GetBattlerSpriteCoord(battler, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[2];
	
    if (sprite->y < 8)
        sprite->y = 8;

    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
    sprite->callback = RunStoredCallbackWhenAffineAnimEnds;
}

// Thrash left/right movements
// No args.
void AnimTask_ThrashMoveMonHorizontal(u8 taskId)
{
    PrepareAffineAnimInTaskData(&gTasks[taskId], GetAnimBattlerSpriteId(ANIM_ATTACKER), sThrashMoveMonAffineAnimCmds);
    gTasks[taskId].func = AnimTask_DestroyTaskAfterAffineAnimFromTaskDataEnds;
}

// Thrash up/down movements
// No args.
void AnimTask_ThrashMoveMonVertical(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    
    task->data[0] = GetAnimBattlerSpriteId(ANIM_ATTACKER);
    task->data[1] = 0;
    task->data[2] = 4;
    task->data[3] = 7;
    task->data[4] = 3;
    task->data[5] = gSprites[task->data[0]].x;
    task->data[6] = gSprites[task->data[0]].y;
    task->data[7] = 0;
    task->data[8] = 0;
    task->data[9] = 2;
	
    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_OPPONENT)
        task->data[2] *= -1;

    task->func = ThrashMoveMon;
}

static void ThrashMoveMon(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    
    if (++task->data[7] > 2)
    {
        task->data[7] = 0;
        ++task->data[8];
		
        if (task->data[8] & 1)
            gSprites[task->data[0]].y += task->data[9];
        else
            gSprites[task->data[0]].y -= task->data[9];
    }
	
    switch (task->data[1])
    {
    case 0:
        gSprites[task->data[0]].x += task->data[2];
		
        if (--task->data[3] == 0)
        {
            task->data[3] = 14;
            task->data[1] = 1;
        }
        break;
    case 1:
        gSprites[task->data[0]].x -= task->data[2];
		
        if (--task->data[3] == 0)
        {
            task->data[3] = 7;
            task->data[1] = 2;
        }
        break;
    case 2:
        gSprites[task->data[0]].x += task->data[2];
		
        if (--task->data[3] == 0)
        {
            if (--task->data[4] != 0)
            {
                task->data[3] = 7;
                task->data[1] = 0;
            }
            else
            {
                if (task->data[8] & 1)
                    gSprites[task->data[0]].y -= task->data[9];

                DestroyAnimVisualTask(taskId);
            }
        }
        break;
    }
}

// Animates Sketch's Pokémon draw effect.
// No args.
void AnimTask_SketchDrawMon(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    struct ScanlineEffectParams params;
    s16 i;
    
    task->data[0] = GetBattlerYCoordWithElevation(gBattleAnimTarget) + 32;
    task->data[1] = 4;
    task->data[2] = 0;
    task->data[3] = 0;
    task->data[4] = 0;
    task->data[5] = 0;
    task->data[15] = GetBattlerSpriteCoordAttr(gBattleAnimTarget, BATTLER_COORD_ATTR_HEIGHT);

    if (GetBattlerSpriteBGPriorityRank(gBattleAnimTarget) == 1)
    {
        task->data[6] = gBattle_BG1_X;
        params.dmaDest = (u16 *)REG_ADDR_BG1HOFS;
    }
    else
    {
        task->data[6] = gBattle_BG2_X;
        params.dmaDest = (u16 *)REG_ADDR_BG2HOFS;
    }

    for (i = task->data[0] - 0x40; i <= task->data[0]; i++)
    {
        if (i >= 0)
        {
            gScanlineEffectRegBuffers[0][i] = task->data[6] + 0xF0;
            gScanlineEffectRegBuffers[1][i] = task->data[6] + 0xF0;
        }
    }
    params.dmaControl = SCANLINE_EFFECT_DMACNT_16BIT;
    params.initState = 1;
    ScanlineEffect_SetParams(params);
    task->func = AnimTask_SketchDrawMon_Step;
}

static void AnimTask_SketchDrawMon_Step(u8 taskId)
{
    struct Task* task = &gTasks[taskId];

    switch (task->data[4])
    {
    case 0:
        if (++task->data[5] > 20)
            task->data[4]++;
        break;
    case 1:
        if (++task->data[1] > 3)
        {
            task->data[1] = 0;
            task->data[2] = task->data[3] & 3;
            task->data[5] = task->data[0] - task->data[3];
			
            switch (task->data[2])
            {
            case 0:
                break;
            case 1:
                task->data[5] -= 2;
                break;
            case 2:
                task->data[5] += 1;
                break;
            case 3:
                task->data[5] += 1;
                break;
            }

            if (task->data[5] >= 0)
            {
                gScanlineEffectRegBuffers[0][task->data[5]] = task->data[6];
                gScanlineEffectRegBuffers[1][task->data[5]] = task->data[6];
            }

            if (++task->data[3] >= task->data[15])
            {
                gScanlineEffect.state = 3;
                DestroyAnimVisualTask(taskId);
            }
        }
        break;
    }
}

// Animates Sketch's pencil sprite.
// No args.
void AnimPencil(struct Sprite *sprite)
{
    sprite->x = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X) - 16;
    sprite->y = GetBattlerYCoordWithElevation(gBattleAnimTarget) + 16;
    sprite->data[0] = 0;
    sprite->data[1] = 0;
    sprite->data[2] = 0;
    sprite->data[3] = 16;
    sprite->data[4] = 0;
    sprite->data[5] = GetBattlerSpriteCoordAttr(gBattleAnimTarget, BATTLER_COORD_ATTR_HEIGHT) + 2;
    sprite->data[6] = BattleAnimAdjustPanning(SOUND_PAN_TARGET);
    sprite->callback = AnimPencil_Step;
}

static void AnimPencil_Step(struct Sprite *sprite)
{
    switch (sprite->data[0])
    {
    case 0:
        if (++sprite->data[2] > 1)
        {
            sprite->data[2] = 0;
            sprite->invisible ^= TRUE;
        }
		
        if (++sprite->data[1] > 16)
        {
            sprite->invisible = FALSE;
            sprite->data[0]++;
        }
        break;
    case 1:
        if (++sprite->data[1] > 3 && sprite->data[2] < sprite->data[5])
        {
            sprite->data[1] = 0;
            sprite->y--;
            
            if (++sprite->data[2] % 10 == 0)
                PlaySE12WithPanning(SE_M_SKETCH, sprite->data[6]);
        }
        sprite->data[4] += sprite->data[3];
		
        if (sprite->data[4] > 31)
        {
            sprite->data[4] = 0x40 - sprite->data[4];
            sprite->data[3] *= -1;
        }
        else if (sprite->data[4] <= -32)
        {
            sprite->data[4] = -0x40 - sprite->data[4];
            sprite->data[3] *= -1;
        }
        sprite->x2 = sprite->data[4];
		
        if (sprite->data[5] == sprite->data[2])
        {
            sprite->data[1] = 0;
            sprite->data[2] = 0;
            sprite->data[0]++;
        }
        break;
    case 2:
        if (++sprite->data[2] > 1)
        {
            sprite->data[2] = 0;
            sprite->invisible ^= TRUE;
        }
		
        if (++sprite->data[1] > 16)
        {
            sprite->invisible = FALSE;
            DestroyAnimSprite(sprite);
        }
        break;
    }
}

// Animates an expanding thin ring sprite blended that average its position in doubles.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: anim battler
// arg 3: respectMonPicOffsets (boolean) (This arg is inverted in base game)
void AnimBlendThinRing(struct Sprite *sprite)
{
    u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[2]);
    u8 r4;

    if (IsDoubleBattleForBattler(battler) && IsBattlerSpriteVisible(BATTLE_PARTNER(battler)))
    {
		u16 x, y;
		
        SetAverageBattlerPositions(battler, gBattleAnimArgs[3], &x, &y);
		
        r4 = GetBattlerSpriteCoord(battler, BATTLER_COORD_X);

        if (GetBattlerSide(battler) != B_SIDE_PLAYER)
            gBattleAnimArgs[0] -= (x - r4) - gBattleAnimArgs[0];
        else
            gBattleAnimArgs[0] = x - r4;
    }
    sprite->callback = AnimSpriteOnMonPos;
    sprite->callback(sprite);
}

// Moves the Hyper Voice's expanding ring into the target.
// arg 0: initial x position (also speed ?)
// arg 1: initial y position
// arg 2: respectMonPicOffsets (boolean)
// arg 3: final x pixel offset
// arg 4: final y pixel offset
void AnimHyperVoiceRing(struct Sprite *sprite)
{
    u16 r9 = 0;
    u16 r6 = 0;
    s16 sp0 = 0;
    s16 sp1 = 0;
    u8 sp4 = gBattleAnimArgs[2] ? BATTLER_COORD_Y_PIC_OFFSET : BATTLER_COORD_Y;
    u8 battler1 = GetBattlerForAnimScript(ANIM_ATTACKER);
    u8 battler2 = GetBattlerForAnimScript(ANIM_TARGET);

    if (GetBattlerSide(battler1) != B_SIDE_PLAYER)
    {
        r9 = GetBattlerSpriteCoord(battler1, BATTLER_COORD_X) + gBattleAnimArgs[0];
		
        if (IsBattlerSpriteVisible(BATTLE_PARTNER(battler2)))
            sprite->subpriority = gSprites[gBattlerSpriteIds[BATTLE_PARTNER(battler2)]].subpriority - 1;
        else
            sprite->subpriority = gSprites[gBattlerSpriteIds[battler2]].subpriority - 1;
    }
    else
    {
        r9 = GetBattlerSpriteCoord(battler1, BATTLER_COORD_X) - gBattleAnimArgs[0];
		
        if (IsBattlerSpriteVisible(BATTLE_PARTNER(battler1)))
        {
            if (gSprites[gBattlerSpriteIds[battler1]].x < gSprites[gBattlerSpriteIds[BATTLE_PARTNER(battler1)]].x)
                sprite->subpriority = gSprites[gBattlerSpriteIds[BATTLE_PARTNER(battler1)]].subpriority + 1;
            else
                sprite->subpriority = gSprites[gBattlerSpriteIds[battler1]].subpriority - 1;
        }
        else
            sprite->subpriority = gSprites[gBattlerSpriteIds[battler1]].subpriority - 1;

    }
    r6 = GetBattlerSpriteCoord(battler1, sp4) + gBattleAnimArgs[1];
	
    if (IsBattlerSpriteVisible(BATTLE_PARTNER(battler2)))
        SetAverageBattlerPositions(battler2, gBattleAnimArgs[2], &sp0, &sp1);
    else
    {
        sp0 = GetBattlerSpriteCoord(battler2, BATTLER_COORD_X);
        sp1 = GetBattlerSpriteCoord(battler2, sp4);
    }

    if (GetBattlerSide(battler2) != B_SIDE_PLAYER)
        sp0 += gBattleAnimArgs[3];
    else
        sp0 -= gBattleAnimArgs[3];

    sp1 += gBattleAnimArgs[4];
	
    sprite->x = sprite->data[1] = r9;
    sprite->y = sprite->data[3] = r6;
	
    sprite->data[2] = sp0;
    sprite->data[4] = sp1;
    sprite->data[0] = gBattleAnimArgs[0];
    InitAnimLinearTranslation(sprite);
    sprite->callback = DestroyAnimSpriteWhenLinearTranslationEnds;
    sprite->callback(sprite);
}

// Animates MOVE_UPROAR's ring sprites.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: anim battler
// arg 3: respectMonPicOffsets (boolean) (This arg is inverted in base game)
// arg 4: ring blend color
// arg 5: blend coefficient
void AnimUproarRing(struct Sprite *sprite)
{
    u8 index = IndexOfSpritePaletteTag(ANIM_TAG_THIN_RING);
    
    if (index != 0xFF)
        BlendPalette(((index << 20) + 0x1010000) >> 16, 15, gBattleAnimArgs[5], gBattleAnimArgs[4]);

    StartSpriteAffineAnim(sprite, 1);
    sprite->callback = AnimSpriteOnMonPos;
    sprite->callback(sprite);
}

// Animates the breaking egg in MOVE_SOFT_BOILED.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: cracked egg part (0 = up part, 1 = left part)
void AnimSoftBoiledEgg(struct Sprite *sprite)
{
    s16 r1;
    
    InitSpritePosToAnimAttacker(sprite, FALSE);
	
    sprite->data[0] = 0x380;
    sprite->data[1] = GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER ? -160 : 160;
    sprite->data[7] = gBattleAnimArgs[2];
    sprite->callback = AnimSoftBoiledEgg_Step1;
}

static void AnimSoftBoiledEgg_Step1(struct Sprite *sprite)
{
    s16 add;
    
    sprite->y2 -= (sprite->data[0] >> 8);
    sprite->x2 = sprite->data[1] >> 8;
	
    sprite->data[0] -= 32;
    sprite->data[1] += GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER ? -160 : 160;
	
    if (sprite->y2 > 0)
    {
        SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
		
        StartSpriteAffineAnim(sprite, 1);
		
		sprite->data[0] = 0;
        sprite->callback = AnimSoftBoiledEgg_Step2;
    }
}

static void AnimSoftBoiledEgg_Step2(struct Sprite *sprite)
{
    if (sprite->data[0]++ > 19)
    {
        StartSpriteAffineAnim(sprite, 2);
        sprite->callback = AnimSoftBoiledEgg_Step3;
    }
}

static void AnimSoftBoiledEgg_Step3(struct Sprite *sprite)
{
    if (sprite->affineAnimEnded)
    {
        StartSpriteAffineAnim(sprite, 1);
		
        sprite->data[0] = 0;
		
        if (sprite->data[7] == 0)
        {
            sprite->oam.tileNum += 16;
            sprite->callback = AnimSoftBoiledEgg_Step3_Callback1;
        }
        else
        {
            sprite->oam.tileNum += 32;
            sprite->callback = AnimSoftBoiledEgg_Step4;
        }
    }
}

static void AnimSoftBoiledEgg_Step3_Callback1(struct Sprite *sprite)
{
    sprite->y2 -= 2;
	
    if (++sprite->data[0] == 9)
    {
        sprite->data[0] = 16;
        sprite->data[1] = 0;
		
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND((u16)sprite->data[0], 0));
		
        sprite->callback = AnimSoftBoiledEgg_Step3_Callback2;
    }
}

static void AnimSoftBoiledEgg_Step3_Callback2(struct Sprite *sprite)
{
    if (sprite->data[1]++ % 3 == 0)
    {
        sprite->data[0]--;
		
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sprite->data[0], 16 - sprite->data[0]));
		
        if (sprite->data[0] == 0)
            sprite->callback = AnimSoftBoiledEgg_Step4;
    }
}

static void AnimSoftBoiledEgg_Step4(struct Sprite *sprite)
{
    if (gBattleAnimArgs[ARG_RET_ID] == -1)
    {
        sprite->invisible = TRUE;
		
        if (sprite->data[7] == 0)
            sprite->callback = DestroyAnimSpriteAndDisableBlend;
        else
            sprite->callback = DestroyAnimSprite;
    }
}

// Animates MOVE_EXTREME_SPEED's attacker stretch effect.
// No args.
void AnimTask_StretchAttacker(u8 taskId)
{
    PrepareAffineAnimInTaskData(&gTasks[taskId], GetAnimBattlerSpriteId(ANIM_ATTACKER), sStretchAttackerAffineAnimCmds);
    gTasks[taskId].func = StretchAttacker_Step;
}

static void StretchAttacker_Step(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    
    if (!RunAffineAnimFromTaskData(task))
    {
        gSprites[task->data[15]].invisible = TRUE;
        DestroyAnimVisualTask(taskId);
    }
}

// Animates the target movement in the impacts of MOVE_EXTREME_SPEED's anim.
// No args.
void AnimTask_ExtremeSpeedImpact(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    
    task->data[0] = 0;
    task->data[1] = 0;
    task->data[2] = 0;
    task->data[3] = 0;
    task->data[12] = 3;
	
    if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_PLAYER)
    {
        task->data[13] = -1;
        task->data[14] = 8;
    }
    else
    {
        task->data[13] = 1;
        task->data[14] = -8;
    }
    task->data[15] = GetAnimBattlerSpriteId(ANIM_TARGET);
    task->func = ExtremeSpeedImpact_Step;
}

static void ExtremeSpeedImpact_Step(u8 taskId)
{
    struct Task* task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        gSprites[task->data[15]].x2 += task->data[14];
        task->data[1] = 0;
        task->data[2] = 0;
        task->data[3] = 0;
        task->data[0]++;
        break;
    case 1:
        if (++task->data[1] > 1)
        {
            task->data[1] = 0;
			
            if (++task->data[2] & 1)
                gSprites[task->data[15]].x2 += 6;
            else
                gSprites[task->data[15]].x2 -= 6;

            if (++task->data[3] > 4)
            {
                if (task->data[2] & 1)
                    gSprites[task->data[15]].x2 -= 6;

                task->data[0]++;
            }
        }
        break;
    case 2:
        if (--task->data[12] != 0)
            task->data[0] = 0;
        else
            task->data[0]++;
        break;
    case 3:
        gSprites[task->data[15]].x2 += task->data[13];
		
        if (gSprites[task->data[15]].x2 == 0)
            DestroyAnimVisualTask(taskId);
        break;
    }
}

// Animates MOVE_EXTREME_SPEED's attacker sprite blink in its reappear effect.
// No args.
void AnimTask_ExtremeSpeedMonReappear(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    
    task->data[0] = 0;
    task->data[1] = 0;
    task->data[2] = 0;
    task->data[3] = 0;
    task->data[4] = 1;
    task->data[13] = 14;
    task->data[14] = 2;
    task->data[15] = GetAnimBattlerSpriteId(ANIM_ATTACKER);
    task->func = ExtremeSpeedMonReappear_Step;
}

static void ExtremeSpeedMonReappear_Step(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    
    if (task->data[0] == 0 && ++task->data[1] > task->data[4])
    {
        task->data[1] = 0;
		
        if (++task->data[2] & 1)
            gSprites[task->data[15]].invisible = FALSE;
        else
            gSprites[task->data[15]].invisible = TRUE;

        if (++task->data[3] >= task->data[13])
        {
            if (++task->data[4] < task->data[14])
            {
                task->data[1] = 0;
                task->data[2] = 0;
                task->data[3] = 0;
            }
            else
            {
                gSprites[task->data[15]].invisible = FALSE;
                DestroyAnimVisualTask(taskId);
            }
        }
    }
}

// Animates MOVE_EXTREME_SPEED's dust sprites on the attacker.
// No args.
void AnimTask_SpeedDust(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    
    task->data[0] = 0;
    task->data[1] = 4;
    task->data[2] = 0;
    task->data[3] = 0;
    task->data[4] = 0;
    task->data[5] = 0;
    task->data[6] = 0;
    task->data[7] = 0;
    task->data[8] = 0;
    task->data[13] = 0;
    task->data[14] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    task->data[15] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y);
    task->func = SpeedDust_Step1;
}

static void SpeedDust_Step1(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    
    switch (task->data[8])
    {
    case 0:
        if (++task->data[4] > 1)
        {
            task->data[4] = 0;
			
            task->data[5] = (task->data[5] + 1) & 1;
			
            if (++task->data[6] > 20)
            {
                if (task->data[7] == 0)
                {
                    task->data[6] = 0;
                    task->data[8] = 1;
                }
                else
                    task->data[8] = 2;
            }
        }
        break;
    case 1:
        task->data[5] = 0;
		
        if (++task->data[4] > 20)
        {
            task->data[7] = 1;
            task->data[8] = 0;
        }
        break;
    case 2:
        task->data[5] = 1;
        break;
    }

    switch (task->data[0])
    {
    case 0:
        if (++task->data[1] > 4)
        {
            u8 spriteId;
			
            task->data[1] = 0;
			
            spriteId = CreateSprite(&gSpeedDustSpriteTemplate, task->data[14], task->data[15], 0);
			
            if (spriteId != MAX_SPRITES)
            {
                gSprites[spriteId].data[0] = taskId;
                gSprites[spriteId].data[1] = 13;
                gSprites[spriteId].x2 = sSpeedDustPosTable[task->data[2]][0];
                gSprites[spriteId].y2 = sSpeedDustPosTable[task->data[2]][1];
				
                if (++task->data[2] > 3)
                {
                    task->data[2] = 0;
					
                    if (++task->data[3] > 5)
                        task->data[0]++;
                }
				task->data[13]++;
            }
        }
        break;
    case 1:
        if (task->data[13] == 0)
            DestroyAnimVisualTask(taskId);
        break;
    }
}

void AnimSpeedDust(struct Sprite *sprite)
{
    sprite->invisible = gTasks[sprite->data[0]].data[5];
	
    if (sprite->animEnded)
    {
        gTasks[sprite->data[0]].data[sprite->data[1]]--;
        DestroySprite(sprite);
    }
}

// Load different palettes for the music note sprite. Used by MOVE_HEAL_BELL.
// No args.
void AnimTask_LoadMusicNotesPals(u8 taskId)
{
    u8 i, paletteNums[3];

    paletteNums[0] = IndexOfSpritePaletteTag(ANIM_TAG_MUSIC_NOTES_2);
	
    for (i = 1; i < 3; i++)
        paletteNums[i] = AllocSpritePalette(ANIM_SPRITES_START - i);

    gMonSpritesGfxPtr->multiUseBuffer = AllocZeroed(0x2000);
	
    LZDecompressWram(gBattleAnimSpritePal_MusicNotes2, gMonSpritesGfxPtr->multiUseBuffer);
	
    for (i = 0; i < 3; i++)
        LoadPalette(&gMonSpritesGfxPtr->multiUseBuffer[i * 32], (u16)((paletteNums[i] << 4) + 0x100), 32);

    FREE_AND_SET_NULL(gMonSpritesGfxPtr->multiUseBuffer);
	
    DestroyAnimVisualTask(taskId);
}

// Free the different palettes loaded for the music note sprite.
// No args.
void AnimTask_FreeMusicNotesPals(u8 taskId)
{
    u8 i;
    
    for (i = 0; i < 3; i++)
        FreeSpritePaletteByTag(gMusicNotePaletteTagsTable[i]);

    DestroyAnimVisualTask(taskId);
}

static void SetMusicNotePalette(struct Sprite *sprite, u8 a, u8 b)
{
    u8 tile = (b & 1) ? 32 : 0;
    sprite->oam.tileNum += tile + (a << 2);
    sprite->oam.paletteNum = IndexOfSpritePaletteTag(gMusicNotePaletteTagsTable[b >> 1]);
}

// Animates the MOVE_HEAL_BELL's multi colored note sprites.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: final x pixel offset
// arg 3: final y pixel offset
// arg 4: duration
// arg 5: ? (related to the music note color and frame)
// arg 6: ? (related to the music note color and frame)
void AnimHealBellMusicNote(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, FALSE);
	
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];

    sprite->data[0] = gBattleAnimArgs[4];
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X) + gBattleAnimArgs[2];
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y) + gBattleAnimArgs[3];
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
	
    SetMusicNotePalette(sprite, gBattleAnimArgs[5], gBattleAnimArgs[6]);
}

// Animates Fake Out's effect.
// No args.
void AnimTask_FakeOut(u8 taskId)
{
    u16 win0v = 0;

    gBattle_WIN0H = 0xF0;
    gBattle_WIN0V = 0xA0;
	
    SetGpuReg(REG_OFFSET_WIN0H, gBattle_WIN0H);
    SetGpuReg(REG_OFFSET_WIN0V, gBattle_WIN0V);
    SetGpuReg(REG_OFFSET_WININ,  WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_BG_ALL);
    SetGpuReg(REG_OFFSET_WINOUT, WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG_ALL | WININ_WIN0_CLR | WININ_WIN0_OBJ | WININ_WIN0_BG_ALL);    
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG3 | BLDCNT_EFFECT_DARKEN);
    SetGpuReg(REG_OFFSET_BLDY, BLDCNT_TGT1_OBJ);
	
    gTasks[taskId].data[0] = win0v;
    gTasks[taskId].data[1] = gBattle_WIN0H;
    gTasks[taskId].func = FakeOutStep1;
}

static void FakeOutStep1(u8 taskId)
{
    gTasks[taskId].data[0] += 13;
    gTasks[taskId].data[1] -= 13;
	
    if (gTasks[taskId].data[0] >= gTasks[taskId].data[1])
    {
        gBattle_WIN0H = 0;
        gTasks[taskId].func = FakeOutStep2;
    }
    else
        gBattle_WIN0H = gTasks[taskId].data[1] | (gTasks[taskId].data[0] << 8);
}

static void FakeOutStep2(u8 taskId)
{
    if (++gTasks[taskId].data[10] == 5)
    {
        gTasks[taskId].data[11] = 0x88;
		
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG3 | BLDCNT_EFFECT_LIGHTEN);
		
        BlendPalettes(SelectBattleAnimSpriteAndBgPalettes(TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE), 16, RGB_WHITE);
    }
    else if (gTasks[taskId].data[10] > 4)
    {
        gBattle_WIN0H = 0;
        gBattle_WIN0V = 0;
		
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG_ALL | WININ_WIN1_OBJ | WININ_WIN1_CLR);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR | WINOUT_WINOBJ_BG_ALL | WINOUT_WINOBJ_OBJ | WINOUT_WINOBJ_CLR);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
		
        DestroyAnimVisualTask(taskId);
    }
}

// Animates a stretching up effect on the given battler.
// arg 0: anim battler
void AnimTask_StretchBattlerUp(u8 taskId)
{
	switch (gTasks[taskId].data[0])
	{
		case 0:
			gTasks[taskId].data[1] = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);
			
			PrepareAffineAnimInTaskData(&gTasks[taskId], gTasks[taskId].data[1], sUnknown_83E4200);
			
			gSprites[gTasks[taskId].data[1]].x2 = 4;
			++gTasks[taskId].data[0];
			break;
		case 1:
		{
			u8 spriteId = gTasks[taskId].data[1];
			
			gSprites[spriteId].x2 = -gSprites[spriteId].x2;
			
			if (!RunAffineAnimFromTaskData(&gTasks[taskId]))
			{
				gSprites[spriteId].x2 = 0;
				gSprites[spriteId].y2 = 0;
				DestroyAnimVisualTask(taskId);
			}
			break;
		}
	}
}

// Animates an projectile sprite floating to the mon position in sin wave. Used by MOVE_ATTRACT.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
void AnimRedHeartProjectile(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, TRUE);
	
    sprite->data[0] = 95;
    sprite->data[1] = sprite->x;
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    sprite->data[3] = sprite->y;
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
    InitAnimLinearTranslation(sprite);
    sprite->callback = AnimRedHeartProjectile_Step;
}

static void AnimRedHeartProjectile_Step(struct Sprite *sprite)
{
    if (!AnimTranslateLinear(sprite))
    {
        sprite->y2 += Sin(sprite->data[5], 14);
        sprite->data[5] = (sprite->data[5] + 4) & 0xFF;
    }
    else
        DestroyAnimSprite(sprite);
}

// Animates MOVE_ATTRACT's hearts rising and moving horizontally in sin wave.
// arg 0: x position
// arg 1: y slide amount
// arg 2: initial wait duration
void AnimRedHeartRising(struct Sprite *sprite)
{
    sprite->x = gBattleAnimArgs[0];
    sprite->y = 160;
	
    sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[1] = gBattleAnimArgs[1];
    sprite->callback = WaitAnimForDuration;
    StoreSpriteCallbackInData6(sprite, AnimRedHeartRising_Step);
}

static void AnimRedHeartRising_Step(struct Sprite *sprite)
{
    s16 y;
    
    sprite->data[2] += sprite->data[1];
	
    sprite->y2 = -((u16)sprite->data[2] >> 8);
    sprite->x2 = Sin(sprite->data[3], 4);
	
    sprite->data[3] = (sprite->data[3] + 3) & 0xFF;
	
    y = sprite->y + sprite->y2;
	
    if (y <= 72)
    {
        sprite->invisible = sprite->data[3] % 2;
		
        if (y <= 64)
            DestroyAnimSprite(sprite);
    }
}

// Displays Scary Faces.
// No args.
void AnimTask_ScaryFace(u8 taskId)
{
    struct BattleAnimBgData animBg;

    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 16));
	
    SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
    SetAnimBgAttribute(1, BG_ANIM_SCREEN_SIZE, 0);
    SetAnimBgAttribute(1, BG_ANIM_CHAR_BASE_BLOCK, 1);
	
    gBattle_BG1_X = 0;
    gBattle_BG1_Y = 0;
	
    SetGpuReg(REG_OFFSET_BG1HOFS, gBattle_BG1_X);
    SetGpuReg(REG_OFFSET_BG1VOFS, gBattle_BG1_Y);
	
    GetBattleAnimBgData(&animBg, 1);
    AnimLoadCompressedBgTilemap(animBg.bgId, GetBattlerSide(gBattleAnimTarget) == B_SIDE_OPPONENT ? gBattleAnimBgTilemap_ScaryFacePlayer : gBattleAnimBgTilemap_ScaryFaceOpponent);
    AnimLoadCompressedBgGfx(animBg.bgId, gBattleAnim_ScaryFaceGfx, animBg.tilesOffset);
    LoadCompressedPalette(gBattleAnim_ScaryFacePal, animBg.paletteId * 16, 32);
	
    gTasks[taskId].func = ScaryFace_Step;
}

static void ScaryFace_Step(u8 taskId)
{
    struct BattleAnimBgData animBg;

    switch (gTasks[taskId].data[12])
    {
    case 0:
        if (++gTasks[taskId].data[10] == 2)
        {
            gTasks[taskId].data[10] = 0;
			
            gTasks[taskId].data[11]++;
			
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[11], 16 - gTasks[taskId].data[11]));
			
            if (gTasks[taskId].data[11] == 14)
            {
				gTasks[taskId].data[11] = 0;
                gTasks[taskId].data[12]++;
            }
        }
        break;
    case 1:
        if (++gTasks[taskId].data[11] == 21)
        {
            gTasks[taskId].data[11] = 14;
            gTasks[taskId].data[12]++;
        }
        break;
    case 2:
        if (++gTasks[taskId].data[10] == 2)
        {
            gTasks[taskId].data[10] = 0;
			
            gTasks[taskId].data[11]--;
			
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[11], 16 - gTasks[taskId].data[11]));
			
            if (gTasks[taskId].data[11] == 0)
                gTasks[taskId].data[12]++;
        }
        break;
    case 3:
        GetBattleAnimBgData(&animBg, 1);
		
        InitBattleAnimBg(1);
        InitBattleAnimBg(2);
		
        SetAnimBgAttribute(1, BG_ANIM_CHAR_BASE_BLOCK, 0);
        SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
		
        DestroyAnimVisualTaskAndDisableBlend(taskId);
        break;
    }
}

// Orbits a sphere in an ellipse around the mon. Used by MOVE_HIDDEN_POWER.
// arg 0: duration
// arg 1: initial wave offset
void AnimOrbitFast(struct Sprite *sprite)
{
    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
	
    sprite->affineAnimPaused = TRUE;
	
    sprite->data[0] = gBattleAnimArgs[0];
    sprite->data[1] = gBattleAnimArgs[1];
    sprite->data[7] = GetBattlerSpriteSubpriority(gBattleAnimAttacker);
	
    sprite->callback = AnimOrbitFastStep;
    sprite->callback(sprite);
}

static void AnimOrbitFastStep(struct Sprite *sprite)
{
    if (sprite->data[1] >= 64 && sprite->data[1] <= 191)
        sprite->subpriority = sprite->data[7] + 1;
    else
        sprite->subpriority = sprite->data[7] - 1;

    sprite->x2 = Sin(sprite->data[1], sprite->data[2] >> 8);
    sprite->y2 = Cos(sprite->data[1], sprite->data[3] >> 8);
	
    sprite->data[1] = (sprite->data[1] + 9) & 0xFF;
	
    switch (sprite->data[5])
    {
    case 1:
        sprite->data[2] -= 0x400;
        sprite->data[3] -= 0x100;
		
        if (++sprite->data[4] == sprite->data[0])
        {
            sprite->data[5] = 2;
            return;
        }
        break;
    case 0:
        sprite->data[2] += 0x400;
        sprite->data[3] += 0x100;
		
        if (++sprite->data[4] == sprite->data[0])
        {
            sprite->data[4] = 0;
            sprite->data[5] = 1;
        }
        break;
    }

    if (gBattleAnimArgs[ARG_RET_ID] == -1) // Signal to end anim
        DestroyAnimSprite(sprite);
}

// Moves orbs away from the mon, based on where they are in their orbit. Used in MOVE_HIDDEN_POWER.
// arg 0: initial wave offset
// arg 1: duration (if not set, then the sprite will move until being offscreen)
void AnimOrbitScatter(struct Sprite *sprite)
{
    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
	
    sprite->data[0] = Sin(gBattleAnimArgs[0], 10);
    sprite->data[1] = Cos(gBattleAnimArgs[0], 7);
	sprite->data[2] = gBattleAnimArgs[1];
    sprite->callback = AnimOrbitScatterStep;
}

static void AnimOrbitScatterStep(struct Sprite *sprite)
{
    sprite->x2 += sprite->data[0];
    sprite->y2 += sprite->data[1];
	
	if (sprite->data[2] > 0)
	{
		if (sprite->data[3]++ >= sprite->data[2])
			DestroyAnimSprite(sprite);
	}
	else
	{
		if (sprite->x + sprite->x2 + 16 > 272u || sprite->y + sprite->y2 > 160 || sprite->y + sprite->y2 < -16)
			DestroyAnimSprite(sprite);
	}
}

// Animates small eye sparkles on the attacker, used by moves like Glare and Scary Face.
// arg 0: x pixel offset
// arg 1: y pixel offset
void AnimEyeSparkle(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, TRUE);
    sprite->callback = DestroyAnimSpriteWhenAnimEnds;
}

// Animates Fury Swipes's slashs.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: sprite anim num
void AnimFurySwipes(struct Sprite *sprite)
{
	StartSpriteAnim(sprite, gBattleAnimArgs[2]);
	
    sprite->x += gBattleAnimArgs[0];
    sprite->y += gBattleAnimArgs[1];
	
	sprite->callback = DestroyAnimSpriteWhenAnimEnds;
}

// Animates the Struggle's movement waves.
// arg 0: anim battler
// arg 1: left position (boolean)
// arg 2: second anim delay
void AnimMovementWaves(struct Sprite *sprite)
{
	u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);
	
    sprite->x = GetBattlerSpriteCoord(battler, BATTLER_COORD_X);
    sprite->y = GetBattlerSpriteCoord(battler, BATTLER_COORD_Y_PIC_OFFSET);

    if (!gBattleAnimArgs[1])
        sprite->x += 32;
    else
        sprite->x -= 32;

    sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[1] = gBattleAnimArgs[1];
    StartSpriteAnim(sprite, sprite->data[1]);
    sprite->callback = AnimMovementWaves_Step;
}

static void AnimMovementWaves_Step(struct Sprite *sprite)
{
    if (sprite->animEnded)
    {
        if (--sprite->data[0])
            StartSpriteAnim(sprite, sprite->data[1]);
        else
            DestroyAnimSprite(sprite);
    }
}

// Animates Uproar distortion effect on a given battler.
// arg 0: anim battler
void AnimTask_UproarDistortion(u8 taskId)
{
    PrepareAffineAnimInTaskData(&gTasks[taskId], GetAnimBattlerSpriteId(gBattleAnimArgs[0]), sUproarAffineAnimCmds);
    gTasks[taskId].func = AnimTask_DestroyTaskAfterAffineAnimFromTaskDataEnds;
}

// Animates MOVE_UPROAR's music note sprite.
// arg 0: anim battler
// arg 1: x pixel offset
// arg 2: y pixel offset
// arg 3: music note tile num
void AnimJaggedMusicNote(struct Sprite *sprite)
{
    int var1;
    u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);

    if (GetBattlerSide(battler) == B_SIDE_OPPONENT)
        gBattleAnimArgs[1] = -gBattleAnimArgs[1];

    sprite->x = GetBattlerSpriteCoord(battler, BATTLER_COORD_X)  + gBattleAnimArgs[1];
    sprite->y = GetBattlerSpriteCoord(battler, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[2];
	
    sprite->data[0] = 0;
    sprite->data[1] = (u16)sprite->x << 3;
    sprite->data[2] = (u16)sprite->y << 3;

    var1 = gBattleAnimArgs[1] << 3;
    if (var1 < 0)
        var1 += 7;
    sprite->data[3] = var1 >> 3;

    var1 = gBattleAnimArgs[2] << 3;
    if (var1 < 0)
        var1 += 7;
    sprite->data[4] = var1 >> 3;

    sprite->oam.tileNum += gBattleAnimArgs[3] * 16;
    sprite->callback = AnimJaggedMusicNote_Step;
}

static void AnimJaggedMusicNote_Step(struct Sprite *sprite)
{
    sprite->data[1] += sprite->data[3];
    sprite->data[2] += sprite->data[4];
	
    sprite->x = sprite->data[1] >> 3;
    sprite->y = sprite->data[2] >> 3;
	
    if (++sprite->data[0] > 16)
        DestroyAnimSprite(sprite);
}

// Creates an invisible music note only for the grayscale effect in MOVE_PERISH_SONG's anim.
// arg 0: delay to grayscale sprites
void AnimPerishSongMusicNote2(struct Sprite *sprite)
{
    if (!sprite->data[0])
    {
        sprite->data[1] = 120 - gBattleAnimArgs[0];
        sprite->invisible = TRUE;
    }

    if (++sprite->data[0] == sprite->data[1])
        SetGreyscaleOrOriginalPalette(sprite->oam.paletteNum + 16, FALSE);

    if (sprite->data[0] == sprite->data[1] + 80)
        DestroyAnimSprite(sprite);
}

// Creates the music notes in MOVE_PERISH_SONG's anim.
// arg 0: y offset
// arg 1: sprite anim num
// arg 2: (?)
void AnimPerishSongMusicNote(struct Sprite *sprite)
{
    int index;

    if (!sprite->data[0])
    {
        sprite->x = 120;
        sprite->y = gBattleAnimArgs[0] / 2 - 15;

        StartSpriteAnim(sprite, gBattleAnimArgs[1]);

        sprite->data[5] = 120;
        sprite->data[3] = gBattleAnimArgs[2];
    }
    sprite->data[0]++;

    sprite->data[1] = sprite->data[0] / 2;
	
    index = ((sprite->data[0] * 3) + (u16)sprite->data[3]);
    sprite->data[6] = (sprite->data[6] + 10) & 0xFF;
    index &= 0xFF;
	
    sprite->x2 = Cos(index, 100);
    sprite->y2 = sprite->data[1] + Sin(index, 10) + Cos(sprite->data[6], 4);

    if (sprite->data[0] > sprite->data[5])
    {
		SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
		StartSpriteAffineAnim(sprite, 1);
		
        sprite->data[0] = 10;
        sprite->data[2] = 5;
        sprite->data[4] = 0;
        sprite->data[3] = 0;
		sprite->callback = WaitAnimForDuration;
		StoreSpriteCallbackInData6(sprite, AnimPerishSongMusicNote_Step);
    }
}

static void AnimPerishSongMusicNote_Step(struct Sprite *sprite)
{
    sprite->data[3] += sprite->data[2];
    sprite->y2 = sprite->data[3];

    sprite->data[2]++;

    if (sprite->data[3] > 48 && sprite->data[2] > 0)
    {
        sprite->data[2] = sprite->data[4] - 5;
        sprite->data[4]++;
    }

    if (sprite->data[4] > 3)
    {
        sprite->invisible = sprite->data[2] % 2;
        DestroyAnimSprite(sprite);
    }

    if (sprite->data[4] == 4)
        DestroyAnimSprite(sprite);
}

// Animates MOVE_SAFEGUARD's rings.
// arg 0: both mons (boolean)
void AnimGuardRing(struct Sprite *sprite)
{
    if (gBattleAnimArgs[0] && IsDoubleBattleForBattler(gBattleAnimAttacker) && IsBattlerSpriteVisible(BATTLE_PARTNER(gBattleAnimAttacker)))
    {
        SetAverageBattlerPositions(gBattleAnimAttacker, FALSE, &sprite->x, &sprite->y);
        sprite->y += 40;

        StartSpriteAffineAnim(sprite, 1);
    }
    else
    {
        sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
        sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y) + 40;
    }

    sprite->data[0] = 13;
    sprite->data[2] = sprite->x;
    sprite->data[4] = sprite->y - 72;

    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Animates the butterflies in Max Flutterby.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: wave amplitude
// arg 3: se to play when start moving (play specified se if set)
void AnimMaxFlutterby(struct Sprite *sprite)
{
	InitSpritePosToAnimAttacker(sprite, FALSE);
	
	sprite->data[0] = 16; // speed delay
	sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
	sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
	sprite->data[5] = gBattleAnimArgs[2];
	sprite->data[6] = gBattleAnimArgs[3];
	sprite->callback = AnimMaxFlutterbyStep;
}

static void AnimMaxFlutterbyStep(struct Sprite *sprite)
{
	// Wait until dynamax growth anim ends
	if (!FuncIsActiveTask(AnimTask_DestroyTaskAfterAffineAnimFromTaskDataEnds))
	{
		if (sprite->data[6] > MUS_DUMMY)
			PlaySE(sprite->data[6]);
		
		StartSpriteAffineAnim(sprite, 1);
		InitAnimArcTranslation(sprite);
		sprite->callback = DestroyAnimSpriteAfterHorizontalTranslation;
	}
}

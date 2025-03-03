#include "global.h"
#include "gflib.h"
#include "battle.h"
#include "battle_anim.h"
#include "data.h"
#include "decompress.h"
#include "graphics.h"
#include "battle_gfx_sfx_util.h"
#include "pokemon_icon.h"
#include "random.h"
#include "scanline_effect.h"
#include "task.h"
#include "trig.h"
#include "util.h"
#include "constants/battle_anim.h"
#include "constants/songs.h"
#include "constants/weather.h"

#define TAG_SMOKESCREEN 55019

// Function Declarations
static void AnimBlackSmokeStep(struct Sprite *);
static void AnimWhiteHalo_Step1(struct Sprite *);
static void AnimMeanLookEye_Step1(struct Sprite *);
static void AnimMeanLookEye_Step2(struct Sprite *);
static void AnimMeanLookEye_Step3(struct Sprite *);
static void AnimMeanLookEye_Step4(struct Sprite *);
static void FadeScreenToWhite_Step(u8);
static void AnimSpotlight_Step1(struct Sprite *);
static void AnimSpotlight_Step2(struct Sprite *);
static void AnimClappingHand_Step(struct Sprite *);
static void AnimRapidSpin_Step(struct Sprite *);
static void RapinSpinMonElevation_Step(u8);
static void AnimWishStar_Step(struct Sprite *);
static void AnimMiniTwinklingStar_Step(struct Sprite *);
static void AnimGreenStar_Step1(struct Sprite *);
static void AnimGreenStar_Step2(struct Sprite *);
static void AnimGreenStar_Callback(struct Sprite *);
static void AnimTask_RockMonBackAndForthStep(u8);
static void AnimSweetScentPetalStep(struct Sprite *);
static void AnimTask_FlailMovementStep(u8);
static void AnimFlatterConfettiStep(struct Sprite *);
static void AnimFlatterSpotlightStep(struct Sprite *);
static void AnimReversalOrbStep(struct Sprite *);
static void AnimTask_DeepInhaleStep(u8);
static void AnimYawnCloudStep(struct Sprite *);
static void AnimTask_SquishAndSweatDropletsStep(u8);
static void CreateSweatDroplets(u8, bool8);
static void AnimTask_FacadeColorBlendStep(u8);
static void AnimTask_GlareEyeDotsStep(u8);
static void GetGlareEyeDotCoords(s16, s16, s16, s16, u8, u8, s16 *, s16 *);
static void AnimTask_BarrageBallStep(u8);
static void AnimSmellingSaltsHand_Step(struct Sprite *);
static void AnimTask_SmellingSaltsSquishStep(u8);
static void AnimSmellingSaltExclamationStep(struct Sprite *);
static void AnimHelpingHandClapStep(struct Sprite *);
static void AnimTask_HelpingHandAttackerMovementStep(u8);
static void AnimForesightMagnifyingGlassStep(struct Sprite *);
static void AnimTask_MonToSubstituteDoll(u8);
static void AnimBlockXStep(struct Sprite *);
static void AnimTask_OdorSleuthMovementWaitFinish(u8);
static void MoveOdorSleuthClone(struct Sprite *);
static void AnimTask_TeeterDanceMovementStep(u8);
static void AnimRecycleStep(struct Sprite *);
static void AnimTask_SlackOffSquishStep(u8);
static void SmokescreenImpact_Callback(struct Sprite * sprite);
static void SmokescreenImpact_Callback_DestroySprite(struct Sprite * sprite);
static void AnimBatonPassPokeball(struct Sprite *sprite);
static void AnimTask_SnatchOpposingMonMove(u8 taskId);
static void AnimTask_SnatchPartnerMove(u8 taskId);
static void AnimHealingWishStar(struct Sprite *sprite);
static void AnimHealingWishStar_Step(struct Sprite *sprite);

// Data
static const struct CompressedSpriteSheet sSmokescreenImpactSpriteSheet =
{
    .data = gSmokescreenImpactTiles, .size = 0x180, .tag = TAG_SMOKESCREEN
};

static const struct CompressedSpritePalette sSmokescreenImpactSpritePalette =
{
    .data = gSmokescreenImpactPalette, .tag = TAG_SMOKESCREEN
};

static const struct OamData sOamData_SmokescreenImpact =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0
};

static const union AnimCmd sAnim_SmokescreenImpact_0[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(4, 4),
    ANIMCMD_FRAME(8, 4),
    ANIMCMD_END
};

static const union AnimCmd sAnim_SmokescreenImpact_1[] =
{
    ANIMCMD_FRAME(0, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(4, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(8, 4, .hFlip = TRUE),
    ANIMCMD_END
};

static const union AnimCmd sAnim_SmokescreenImpact_2[] =
{
    ANIMCMD_FRAME(0, 4, .vFlip = TRUE),
    ANIMCMD_FRAME(4, 4, .vFlip = TRUE),
    ANIMCMD_FRAME(8, 4, .vFlip = TRUE),
    ANIMCMD_END
};

static const union AnimCmd sAnim_SmokescreenImpact_3[] =
{
    ANIMCMD_FRAME(0, 4, .hFlip = TRUE, .vFlip = TRUE),
    ANIMCMD_FRAME(4, 4, .hFlip = TRUE, .vFlip = TRUE),
    ANIMCMD_FRAME(8, 4, .hFlip = TRUE, .vFlip = TRUE),
    ANIMCMD_END
};

static const union AnimCmd *const sAnims_SmokescreenImpact[] =
{
    sAnim_SmokescreenImpact_0,
    sAnim_SmokescreenImpact_1,
    sAnim_SmokescreenImpact_2,
    sAnim_SmokescreenImpact_3,
};

static const struct SpriteTemplate sSmokescreenImpactSpriteTemplate =
{
    .tileTag = TAG_SMOKESCREEN,
    .paletteTag = TAG_SMOKESCREEN,
    .oam = &sOamData_SmokescreenImpact,
    .anims = sAnims_SmokescreenImpact,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SmokescreenImpact_Callback_DestroySprite
};

static const union AnimCmd sScratchAnimCmds[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(16, 4),
    ANIMCMD_FRAME(32, 4),
    ANIMCMD_FRAME(48, 4),
    ANIMCMD_FRAME(64, 4),
    ANIMCMD_END,
};

static const union AnimCmd *const sScratchAnimTable[] =
{
    sScratchAnimCmds,
};

const struct SpriteTemplate gScratchSpriteTemplate =
{
    .tileTag = ANIM_TAG_SCRATCH,
    .paletteTag = ANIM_TAG_SCRATCH,
    .oam = &gOamData_AffineOff_ObjBlend_32x32,
    .anims = sScratchAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSpriteOnMonPos,
};

const struct SpriteTemplate gBlackSmokeSpriteTemplate =    
{
    .tileTag = ANIM_TAG_BLACK_SMOKE,
    .paletteTag = ANIM_TAG_BLACK_SMOKE,
    .oam = &gOamData_AffineOff_ObjNormal_32x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimBlackSmoke,
};

const struct SpriteTemplate gBlackBallSpriteTemplate =    
{
    .tileTag = ANIM_TAG_BLACK_BALL,
    .paletteTag = ANIM_TAG_BLACK_BALL,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimThrowProjectile,
};

static const union AnimCmd sOpeningEyeAnimCmds[] =    
{
    ANIMCMD_FRAME(0, 40),
    ANIMCMD_FRAME(16, 8),
    ANIMCMD_FRAME(32, 40),
    ANIMCMD_END,
};

static const union AnimCmd *const sOpeningEyeAnimTable[] =    
{
    sOpeningEyeAnimCmds,
};

const struct SpriteTemplate gOpeningEyeSpriteTemplate =    
{
    .tileTag = ANIM_TAG_OPENING_EYE,
    .paletteTag = ANIM_TAG_OPENING_EYE,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sOpeningEyeAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSpriteOnMonPos,
};

const struct SpriteTemplate gWhiteHaloSpriteTemplate =
{
    .tileTag = ANIM_TAG_ROUND_WHITE_HALO,
    .paletteTag = ANIM_TAG_ROUND_WHITE_HALO,
    .oam = &gOamData_AffineOff_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimWhiteHalo,
};

const struct SpriteTemplate gTealAlertSpriteTemplate =
{
    .tileTag = ANIM_TAG_TEAL_ALERT,
    .paletteTag = ANIM_TAG_TEAL_ALERT,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimTealAlert,
};

static const union AffineAnimCmd sMeanLookEyeAffineAnimCmds1[] =
{
    AFFINEANIMCMD_FRAME(0x180, 0x180, 0, 0),
    AFFINEANIMCMD_FRAME(-0x20, 0x18, 0, 5),
    AFFINEANIMCMD_FRAME(0x18, -0x20, 0, 5),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd sMeanLookEyeAffineAnimCmds2[] =
{
    AFFINEANIMCMD_FRAME(0x30, 0x30, 0, 0),
    AFFINEANIMCMD_FRAME(0x20, 0x20, 0, 6),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sMeanLookEyeAffineAnimTable[] =
{
    sMeanLookEyeAffineAnimCmds1,
    sMeanLookEyeAffineAnimCmds2,
};

const struct SpriteTemplate gMeanLookEyeSpriteTemplate =
{
    .tileTag = ANIM_TAG_EYE,
    .paletteTag = ANIM_TAG_EYE,
    .oam = &gOamData_AffineDouble_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sMeanLookEyeAffineAnimTable,
    .callback = AnimMeanLookEye,
};

static const union AnimCmd sLeerAnimCmds[] =
{
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_FRAME(16, 3),
    ANIMCMD_FRAME(32, 3),
    ANIMCMD_FRAME(48, 3),
    ANIMCMD_FRAME(64, 3),
    ANIMCMD_END,
};

static const union AnimCmd *const sLeerAnimTable[] =
{
    sLeerAnimCmds,
};

const struct SpriteTemplate gLeerSpriteTemplate =
{
    .tileTag = ANIM_TAG_LEER,
    .paletteTag = ANIM_TAG_LEER,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sLeerAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimLeer,
};

static const union AnimCmd sLetterZAnimCmds[] =
{
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_END,
};

static const union AnimCmd *const sLetterZAnimTable[] =
{
    sLetterZAnimCmds,
};

static const union AffineAnimCmd sLetterZAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(-7, -7, -3, 16),
    AFFINEANIMCMD_FRAME(7, 7, 3, 16),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd *const sLetterZAffineAnimTable[] =
{
    sLetterZAffineAnimCmds,
};

const struct SpriteTemplate gLetterZSpriteTemplate =
{
    .tileTag = ANIM_TAG_LETTER_Z,
    .paletteTag = ANIM_TAG_LETTER_Z,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = sLetterZAnimTable,
    .images = NULL,
    .affineAnims = sLetterZAffineAnimTable,
    .callback = AnimLetterZ,
};

static const union AnimCmd sFangAnimCmds[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(16, 16),
    ANIMCMD_FRAME(32, 4),
    ANIMCMD_FRAME(48, 4),
    ANIMCMD_END,
};

static const union AnimCmd *const sFangAnimTable[] =
{
    sFangAnimCmds,
};

static const union AffineAnimCmd sFangAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(0x200, 0x200, 0, 0),
    AFFINEANIMCMD_FRAME(-0x20, -0x20, 0, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sFangAffineAnimTable[] =
{
    sFangAffineAnimCmds,
};

const struct SpriteTemplate gFangSpriteTemplate =
{
    .tileTag = ANIM_TAG_FANG_ATTACK,
    .paletteTag = ANIM_TAG_FANG_ATTACK,
    .oam = &gOamData_AffineDouble_ObjNormal_32x32,
    .anims = sFangAnimTable,
    .images = NULL,
    .affineAnims = sFangAffineAnimTable,
    .callback = DestroyAnimSpriteWhenAnimEnds,
};

static const union AffineAnimCmd sSpotlightAffineAnimCmds1[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x180, 0, 0),
    AFFINEANIMCMD_FRAME(0x10, 0x0, 0, 20),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSpotlightAffineAnimCmds2[] =
{
    AFFINEANIMCMD_FRAME(0x140, 0x180, 0, 0),
    AFFINEANIMCMD_FRAME(-0x10, 0x0, 0, 19),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sSpotlightAffineAnimTable[] =
{
    sSpotlightAffineAnimCmds1,
    sSpotlightAffineAnimCmds2,
};

const struct SpriteTemplate gSpotlightSpriteTemplate =
{
    .tileTag = ANIM_TAG_SPOTLIGHT,
    .paletteTag = ANIM_TAG_SPOTLIGHT,
    .oam = &gOamData_AffineDouble_ObjNormal_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sSpotlightAffineAnimTable,
    .callback = AnimSpotlight,
};

const struct SpriteTemplate gClappingHandSpriteTemplate =
{
    .tileTag = ANIM_TAG_TAG_HAND,
    .paletteTag = ANIM_TAG_TAG_HAND,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimClappingHand,
};

const struct SpriteTemplate gClappingHand2SpriteTemplate =
{
    .tileTag = ANIM_TAG_TAG_HAND,
    .paletteTag = ANIM_TAG_TAG_HAND,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimClappingHand2,
};

static const union AnimCmd sRapidSpinAnimCmds[] =
{
    ANIMCMD_FRAME(0, 2),
    ANIMCMD_FRAME(8, 2),
    ANIMCMD_FRAME(16, 2),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sRapidSpinAnimTable[] =
{
    sRapidSpinAnimCmds,
};

const struct SpriteTemplate gRapidSpinSpriteTemplate =
{
    .tileTag = ANIM_TAG_RAPID_SPIN,
    .paletteTag = ANIM_TAG_RAPID_SPIN,
    .oam = &gOamData_AffineOff_ObjNormal_32x16,
    .anims = sRapidSpinAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimRapidSpin,
};

static const union AnimCmd sTriAttackTriangleAnimCmds[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_END,
};

static const union AnimCmd *const sTriAttackTriangleAnimTable[] =
{
    sTriAttackTriangleAnimCmds,
};

static const union AffineAnimCmd sTriAttackTriangleAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(0, 0, 5, 40),
    AFFINEANIMCMD_FRAME(0, 0, 10, 10),
    AFFINEANIMCMD_FRAME(0, 0, 15, 10),
    AFFINEANIMCMD_FRAME(0, 0, 20, 40),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd *const sTriAttackTriangleAffineAnimTable[] =
{
    sTriAttackTriangleAffineAnimCmds,
};

const struct SpriteTemplate gTriAttackTriangleSpriteTemplate =
{
    .tileTag = ANIM_TAG_TRI_FORCE_TRIANGLE,
    .paletteTag = ANIM_TAG_TRI_FORCE_TRIANGLE,
    .oam = &gOamData_AffineDouble_ObjNormal_64x64,
    .anims = sTriAttackTriangleAnimTable,
    .images = NULL,
    .affineAnims = sTriAttackTriangleAffineAnimTable,
    .callback = AnimTriAttackTriangle,
};

static const union AnimCmd sEclipsingOrbAnimCmds[] =
{
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_FRAME(16, 3),
    ANIMCMD_FRAME(32, 3),
    ANIMCMD_FRAME(48, 3),
    ANIMCMD_FRAME(32, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(0, 3, .hFlip = TRUE),
    ANIMCMD_LOOP(1),
    ANIMCMD_END,
};

static const union AnimCmd *const sEclipsingOrbAnimTable[] =
{
    sEclipsingOrbAnimCmds,
};

const struct SpriteTemplate gEclipsingOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_ECLIPSING_ORB,
    .paletteTag = ANIM_TAG_ECLIPSING_ORB,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sEclipsingOrbAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSpriteOnMonPos,
};

static const union AffineAnimCmd sDefenseCurlDeformMonAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(-12, 20, 0, 8),
    AFFINEANIMCMD_FRAME(12, -20, 0, 8),
    AFFINEANIMCMD_LOOP(2),
    AFFINEANIMCMD_END,
};

const struct SpriteTemplate gWishStarSpriteTemplate =
{
    .tileTag = ANIM_TAG_GOLD_STARS,
    .paletteTag = ANIM_TAG_GOLD_STARS,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimWishStar,
};

const struct SpriteTemplate gMiniTwinklingStarSpriteTemplate =
{
    .tileTag = ANIM_TAG_GOLD_STARS,
    .paletteTag = ANIM_TAG_GOLD_STARS,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimMiniTwinklingStar,
};

static const union AffineAnimCmd sStockpileDeformMonAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(8, -8, 0, 12),
    AFFINEANIMCMD_FRAME(-16, 16, 0, 12),
    AFFINEANIMCMD_FRAME(8, -8, 0, 12),
    AFFINEANIMCMD_LOOP(1),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSpitUpDeformMonAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(0, 6, 0, 20),
    AFFINEANIMCMD_FRAME(0, 0, 0, 20),
    AFFINEANIMCMD_FRAME(0, -18, 0, 6),
    AFFINEANIMCMD_FRAME(-18, -18, 0, 3),
    AFFINEANIMCMD_FRAME(0, 0, 0, 15),
    AFFINEANIMCMD_FRAME(4, 4, 0, 13),
    AFFINEANIMCMD_END,
};

const struct SpriteTemplate gSwallowBlueOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_BLUE_ORB,
    .paletteTag = ANIM_TAG_BLUE_ORB,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSwallowBlueOrb,
};

static const union AffineAnimCmd sSwallowDeformMonAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(0, 6, 0, 20),
    AFFINEANIMCMD_FRAME(0, 0, 0, 20),
    AFFINEANIMCMD_FRAME(7, -30, 0, 6),
    AFFINEANIMCMD_FRAME(0, 0, 0, 20),
    AFFINEANIMCMD_FRAME(-2, 3, 0, 20),
    AFFINEANIMCMD_END,
};

static const s8 sMorningSunLightBeamCoordsTable[] =
{
    0xE8,
    0x18,
    0xFC,
    0x00,
};

static const union AnimCmd sGreenStarAnimCmds1[] =
{
    ANIMCMD_FRAME(0, 6),
    ANIMCMD_FRAME(4, 6),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sGreenStarAnimCmds2[] =
{
    ANIMCMD_FRAME(8, 6),
    ANIMCMD_END,
};

static const union AnimCmd sGreenStarAnimCmds3[] =    
{
    ANIMCMD_FRAME(12, 6),
    ANIMCMD_END,
};

static const union AnimCmd *const sGreenStarAnimTable[] =    
{
    sGreenStarAnimCmds1,
    sGreenStarAnimCmds2,
    sGreenStarAnimCmds3,
};

const struct SpriteTemplate gGreenStarSpriteTemplate =    
{
    .tileTag = ANIM_TAG_GREEN_STAR,
    .paletteTag = ANIM_TAG_GREEN_STAR,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = sGreenStarAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimGreenStar,
};

static const s8 sDoomDesireLightBeamCoordTable[] =    
{
    0x78,
    0x50,
    0x28,
    0x00,
};

static const u8 sDoomDesireLightBeamDelayTable[] =    
{
    0,
    0,
    0,
    0,
    50,
};

static const union AffineAnimCmd sStrongFrustrationAffineAnimCmds[] =    
{
    AFFINEANIMCMD_FRAME(0, -15, 0, 7),
    AFFINEANIMCMD_FRAME(0, 15, 0, 7),
    AFFINEANIMCMD_LOOP(2),
    AFFINEANIMCMD_END,
};

const struct SpriteTemplate gWeakFrustrationAngerMarkSpriteTemplate =
{
    .tileTag = ANIM_TAG_ANGER,
    .paletteTag = ANIM_TAG_ANGER,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimWeakFrustrationAngerMark,
};

static const union AnimCmd sSweetScentPetalAnimCmds1[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(1, 8),
    ANIMCMD_FRAME(2, 8),
    ANIMCMD_FRAME(3, 8),
    ANIMCMD_FRAME(3, 8, .vFlip = TRUE),
    ANIMCMD_FRAME(2, 8, .vFlip = TRUE),
    ANIMCMD_FRAME(0, 8, .vFlip = TRUE),
    ANIMCMD_FRAME(1, 8, .vFlip = TRUE),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sSweetScentPetalAnimCmds2[] =
{
    ANIMCMD_FRAME(0, 8, .hFlip = TRUE),
    ANIMCMD_FRAME(1, 8, .hFlip = TRUE),
    ANIMCMD_FRAME(2, 8, .hFlip = TRUE),
    ANIMCMD_FRAME(3, 8, .hFlip = TRUE),
    ANIMCMD_FRAME(3, 8, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_FRAME(2, 8, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_FRAME(0, 8, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_FRAME(1, 8, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sSweetScentPetalAnimCmds3[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_END,
};

static const union AnimCmd *const sSweetScentPetalAnimCmdTable[] =    
{
    sSweetScentPetalAnimCmds1,
    sSweetScentPetalAnimCmds2,
    sSweetScentPetalAnimCmds3,
};

const struct SpriteTemplate gSweetScentPetalSpriteTemplate =
{
    .tileTag = ANIM_TAG_PINK_PETAL,
    .paletteTag = ANIM_TAG_PINK_PETAL,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = sSweetScentPetalAnimCmdTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSweetScentPetal,
};

static const union AnimCmd sPainSplitAnimCmds[] =
{
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_FRAME(4, 9),
    ANIMCMD_FRAME(8, 5),
    ANIMCMD_END,
};

static const union AnimCmd *const sPainSplitAnimCmdTable[] =
{
    sPainSplitAnimCmds,
};

const struct SpriteTemplate gPainSplitProjectileSpriteTemplate =
{
    .tileTag = ANIM_TAG_PAIN_SPLIT,
    .paletteTag = ANIM_TAG_PAIN_SPLIT,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = sPainSplitAnimCmdTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimPainSplitProjectile,
};

const struct SpriteTemplate gFlatterConfettiSpriteTemplate =
{
    .tileTag = ANIM_TAG_CONFETTI,
    .paletteTag = ANIM_TAG_CONFETTI,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimFlatterConfetti,
};

const struct SpriteTemplate gFlatterSpotlightSpriteTemplate =
{
    .tileTag = ANIM_TAG_SPOTLIGHT,
    .paletteTag = ANIM_TAG_SPOTLIGHT,
    .oam = &gOamData_AffineDouble_ObjNormal_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sSpotlightAffineAnimTable,
    .callback = AnimFlatterSpotlight,
};

const struct SpriteTemplate gReversalOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_BLUE_ORB,
    .paletteTag = ANIM_TAG_BLUE_ORB,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimReversalOrb,
};

static const union AffineAnimCmd sDeepInhaleAffineAnimCmds[] =    
{
    AFFINEANIMCMD_FRAME(16, 0, 0, 4),
    AFFINEANIMCMD_FRAME(0, -3, 0, 16),
    AFFINEANIMCMD_FRAME(4, 0, 0, 4),
    AFFINEANIMCMD_FRAME(0, 0, 0, 24),
    AFFINEANIMCMD_FRAME(-5, 3, 0, 16),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sYawnCloudAffineAnimCmds1[] =    
{
    AFFINEANIMCMD_FRAME(0x80, 0x80, 0, 0),
    AFFINEANIMCMD_FRAME(-8, -8, 0, 8),
    AFFINEANIMCMD_FRAME(8, 8, 0, 8),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sYawnCloudAffineAnimCmds2[] =    
{
    AFFINEANIMCMD_FRAME(0xC0, 0xC0, 0, 0),
    AFFINEANIMCMD_FRAME(8, 8, 0, 8),
    AFFINEANIMCMD_FRAME(-8, -8, 0, 8),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sYawnCloudAffineAnimCmds3[] =    
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME(8, 8, 0, 8),
    AFFINEANIMCMD_FRAME(-8, -8, 0, 8),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd *const sYawnCloudAffineAnimTable[] =    
{
    sYawnCloudAffineAnimCmds1,
    sYawnCloudAffineAnimCmds2,
    sYawnCloudAffineAnimCmds3,
};

const struct SpriteTemplate gYawnCloudSpriteTemplate =
{
    .tileTag = ANIM_TAG_PINK_CLOUD,
    .paletteTag = ANIM_TAG_PINK_CLOUD,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sYawnCloudAffineAnimTable,
    .callback = AnimYawnCloud,
};

static const union AffineAnimCmd sSmokeBallEscapeCloudAffineAnimCmds1[] =    
{
    AFFINEANIMCMD_FRAME(0x80, 0x80, 0, 0),
    AFFINEANIMCMD_FRAME(-4, -6, 0, 16),
    AFFINEANIMCMD_FRAME(4, 6, 0, 16),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sSmokeBallEscapeCloudAffineAnimCmds2[] =    
{
    AFFINEANIMCMD_FRAME(0xC0, 0xC0, 0, 0),
    AFFINEANIMCMD_FRAME(4, 6, 0, 16),
    AFFINEANIMCMD_FRAME(-4, -6, 0, 16),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sSmokeBallEscapeCloudAffineAnimCmds3[] =    
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME(4, 6, 0, 16),
    AFFINEANIMCMD_FRAME(-4, -6, 0, 16),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sSmokeBallEscapeCloudAffineAnimCmds4[] =    
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME(8, 10, 0, 30),
    AFFINEANIMCMD_FRAME(-8, -10, 0, 16),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd *const sSmokeBallEscapeCloudAffineAnimTable[] =    
{
    sSmokeBallEscapeCloudAffineAnimCmds1,
    sSmokeBallEscapeCloudAffineAnimCmds2,
    sSmokeBallEscapeCloudAffineAnimCmds3,
    sSmokeBallEscapeCloudAffineAnimCmds4,
};

const struct SpriteTemplate gSmokeBallEscapeCloudSpriteTemplate =    
{
    .tileTag = ANIM_TAG_PINK_CLOUD,
    .paletteTag = ANIM_TAG_PINK_CLOUD,
    .oam = &gOamData_AffineDouble_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sSmokeBallEscapeCloudAffineAnimTable,
    .callback = AnimSmokeBallEscapeCloud,
};

static const union AffineAnimCmd sFacadeSquishAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(-16, 16, 0, 6),
    AFFINEANIMCMD_FRAME(16, -16, 0, 12),
    AFFINEANIMCMD_FRAME(-16, 16, 0, 6),
    AFFINEANIMCMD_END,
};

const struct SpriteTemplate gFacadeSweatDropSpriteTemplate =
{
    .tileTag = ANIM_TAG_SWEAT_DROP,
    .paletteTag = ANIM_TAG_SWEAT_DROP,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimFacadeSweatDrop,
};

static const u16 sFacadeBlendColors[] = 
{
    RGB(28, 25,  1),
    RGB(28, 21,  5),
    RGB(27, 18,  8),
    RGB(27, 14, 11),
    RGB(26, 10, 15),
    RGB(26,  7, 18),
    RGB(25,  3, 21),
    RGB(25,  0, 25),
    RGB(25,  0, 23),
    RGB(25,  0, 20),
    RGB(25,  0, 16),
    RGB(25,  0, 13),
    RGB(26,  0, 10),
    RGB(26,  0,  6),
    RGB(26,  0,  3),
    RGB(27,  0,  0),
    RGB(27,  1,  0),
    RGB(27,  5,  0),
    RGB(27,  9,  0),
    RGB(27, 12,  0),
    RGB(28, 16,  0),
    RGB(28, 19,  0),
    RGB(28, 23,  0),
    RGB(29, 27,  0),
};

static const union AnimCmd sRoarNoiseLineAnimCmds1[] =
{
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_FRAME(16, 3),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sRoarNoiseLineAnimCmds2[] =    
{
    ANIMCMD_FRAME(32, 3),
    ANIMCMD_FRAME(48, 3),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sRoarNoiseLineAnimTable[] =
{
    sRoarNoiseLineAnimCmds1,
    sRoarNoiseLineAnimCmds2,
};

const struct SpriteTemplate gRoarNoiseLineSpriteTemplate =
{
    .tileTag = ANIM_TAG_NOISE_LINE,
    .paletteTag = ANIM_TAG_NOISE_LINE,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sRoarNoiseLineAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimRoarNoiseLine,
};

const struct SpriteTemplate gGlareEyeDotSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_RED_EYE,
    .paletteTag = ANIM_TAG_SMALL_RED_EYE,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimGlareEyeDot,
};

const struct SpriteTemplate gAssistPawprintSpriteTemplate =    
{
    .tileTag = ANIM_TAG_PAW_PRINT,
    .paletteTag = ANIM_TAG_PAW_PRINT,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimAssistPawprint,
};

static const union AffineAnimCmd sBarrageBallAffineAnimCmds1[] =
{
    AFFINEANIMCMD_FRAME(0, 0, -4, 24),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sBarrageBallAffineAnimCmds2[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, -64, 0),
    AFFINEANIMCMD_FRAME(0, 0, 4, 24),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sBarrageBallAffineAnimTable[] =
{
    sBarrageBallAffineAnimCmds1,
    sBarrageBallAffineAnimCmds2,
};

const struct SpriteTemplate gBarrageBallSpriteTemplate =
{
    .tileTag = ANIM_TAG_RED_BALL,
    .paletteTag = ANIM_TAG_RED_BALL,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sBarrageBallAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gSmellingSaltsHandSpriteTemplate =
{
    .tileTag = ANIM_TAG_TAG_HAND,
    .paletteTag = ANIM_TAG_TAG_HAND,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSmellingSaltsHand,
};

static const union AffineAnimCmd sSmellingSaltsSquishAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(0, -16, 0, 6),
    AFFINEANIMCMD_FRAME(0, 16, 0, 6),
    AFFINEANIMCMD_END,
};

const struct SpriteTemplate gSmellingSaltExclamationSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMELLINGSALT_EFFECT,
    .paletteTag = ANIM_TAG_SMELLINGSALT_EFFECT,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSmellingSaltExclamation,
};

const struct SpriteTemplate gHelpingHandClapSpriteTemplate =
{
    .tileTag = ANIM_TAG_TAG_HAND,
    .paletteTag = ANIM_TAG_TAG_HAND,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimHelpingHandClap,
};

const struct SpriteTemplate gForesightMagnifyingGlassSpriteTemplate =
{
    .tileTag = ANIM_TAG_MAGNIFYING_GLASS,
    .paletteTag = ANIM_TAG_MAGNIFYING_GLASS,
    .oam = &gOamData_AffineOff_ObjBlend_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimForesightMagnifyingGlass,
};

const struct SpriteTemplate gMeteorMashStarSpriteTemplate =    
{
    .tileTag = ANIM_TAG_GOLD_STARS,
    .paletteTag = ANIM_TAG_GOLD_STARS,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimMeteorMashStar,
};

const struct SpriteTemplate gBlockXSpriteTemplate =    
{
    .tileTag = ANIM_TAG_X_SIGN,
    .paletteTag = ANIM_TAG_X_SIGN,
    .oam = &gOamData_AffineOff_ObjNormal_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimBlockX,
};

static const union AnimCmd sKnockOffStrikeAnimCmds[] =    
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(64, 4),
    ANIMCMD_END,
};

static const union AnimCmd *const sKnockOffStrikeAnimTable[] =    
{
    sKnockOffStrikeAnimCmds,
};

static const union AffineAnimCmd sKnockOffStrikeAffineanimCmds1[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME(0, 0, -4, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sKnockOffStrikeAffineanimCmds2[] =
{
    AFFINEANIMCMD_FRAME(-0x100, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME(0, 0, 4, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sKnockOffStrikeAffineAnimTable[] =
{
    sKnockOffStrikeAffineanimCmds1,
    sKnockOffStrikeAffineanimCmds2,
};

const struct SpriteTemplate gKnockOffStrikeSpriteTemplate =    
{
    .tileTag = ANIM_TAG_SLAM_HIT_2,
    .paletteTag = ANIM_TAG_SLAM_HIT_2,
    .oam = &gOamData_AffineNormal_ObjNormal_64x64,
    .anims = sKnockOffStrikeAnimTable,
    .images = NULL,
    .affineAnims = sKnockOffStrikeAffineAnimTable,
    .callback = AnimKnockOffStrike,
};

static const union AffineAnimCmd sRecycleSpriteAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(0, 0, -4, 64),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd *const sRecycleSpriteAffineAnimTable[] =
{
    sRecycleSpriteAffineAnimCmds,
};

const struct SpriteTemplate gRecycleSpriteTemplate =
{
    .tileTag = ANIM_TAG_RECYCLE,
    .paletteTag = ANIM_TAG_RECYCLE,
    .oam = &gOamData_AffineNormal_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sRecycleSpriteAffineAnimTable,
    .callback = AnimRecycle,
};

static const union AffineAnimCmd sSlackOffSquishAffineAnimCmds[] =    
{
    AFFINEANIMCMD_FRAME(0, 16, 0, 4),
    AFFINEANIMCMD_FRAME(-2, 0, 0, 8),
    AFFINEANIMCMD_FRAME(0, 4, 0, 4),
    AFFINEANIMCMD_FRAME(0, 0, 0, 24),
    AFFINEANIMCMD_FRAME(1, -5, 0, 16),
    AFFINEANIMCMD_END,
};

const struct SpriteTemplate gBatonPassPokeballSpriteTemplate =
{
    .tileTag = ANIM_TAG_POKEBALL,
    .paletteTag = ANIM_TAG_POKEBALL,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimBatonPassPokeball,
};

static const union AffineAnimCmd sSquishTargetAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(0, 64, 0, 16), // Flatten
    AFFINEANIMCMD_FRAME(0, 0, 0, 64),
    AFFINEANIMCMD_FRAME(0, -64, 0, 16),
    AFFINEANIMCMD_END,
};

static const union AnimCmd sHealingWishGreenStarAnimCmds[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(4, 4),
    ANIMCMD_FRAME(8, 4),
    ANIMCMD_FRAME(12, 4),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sHealingWishGreenStarAnimTable[] =
{
    sHealingWishGreenStarAnimCmds,
};

static const union AffineAnimCmd sHealingWishStarAffineAnimCmds[] =
{
	AFFINEANIMCMD_FRAME(240, 240, 0, 1),
    AFFINEANIMCMD_FRAME(0, 0, -2, 1),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd *const sHealingWishStarAffineAnimTable[] =
{
    sHealingWishStarAffineAnimCmds,
};

const struct SpriteTemplate gHealingWishSparkleSpriteTemplate =
{
    .tileTag = ANIM_TAG_GREEN_SPARKLE,
    .paletteTag = ANIM_TAG_GREEN_SPARKLE,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = sHealingWishGreenStarAnimTable,
    .images = NULL,
    .affineAnims = sHealingWishStarAffineAnimTable,
    .callback = AnimHealingWishStar,
};

static const union AnimCmd sMiniTwinklingGreenStarAnimCmds[] =
{
	ANIMCMD_FRAME(4, 0),
	ANIMCMD_END,
};

static const union AnimCmd *const sMiniTwinklingGreenStarAnimTable[] =
{
    sMiniTwinklingGreenStarAnimCmds,
};

static const union AffineAnimCmd sMiniTwinklingGreenStarAffineAnimCmds[] =
{
	AFFINEANIMCMD_FRAME(-128, -128, 0, 1),
    AFFINEANIMCMD_FRAME(0, 0, -2, 1),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd *const sMiniTwinklingGreenStarAffineAnimTable[] =
{
    sMiniTwinklingGreenStarAffineAnimCmds,
};

const struct SpriteTemplate gMiniTwinklingGreenStarSpriteTemplate =
{
    .tileTag = ANIM_TAG_GREEN_SPARKLE,
    .paletteTag = ANIM_TAG_GREEN_SPARKLE,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = sMiniTwinklingGreenStarAnimTable,
    .images = NULL,
    .affineAnims = sMiniTwinklingGreenStarAffineAnimTable,
    .callback = AnimMiniTwinklingStar,
};

static const union AffineAnimCmd sAffineAnim_PunishmentBladeCmds1[] =
{
    AFFINEANIMCMD_FRAME(0, 0, 10, 1),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sAffineAnim_PunishmentBladeCmds2[] =
{
    AFFINEANIMCMD_FRAME(0, 0, -10, 1),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd *const sAffineAnims_PunishmentBlade[] =
{
    sAffineAnim_PunishmentBladeCmds1,
	sAffineAnim_PunishmentBladeCmds2,
};

const struct SpriteTemplate gPunishmentBladeSpriteTemplate =
{
    .tileTag = ANIM_TAG_PUNISHMENT_BLADES,
    .paletteTag = ANIM_TAG_PUNISHMENT_BLADES,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = sScratchAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_PunishmentBlade,
    .callback = AnimKnockOffStrike,
};

const struct SpriteTemplate gVacuumWaveWhiteSmokeSpriteTemplate =
{
    .tileTag = ANIM_TAG_BLACK_SMOKE,
    .paletteTag = ANIM_TAG_ICE_CHUNK,
    .oam = &gOamData_AffineOff_ObjNormal_32x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimBlackSmoke,
};

// Animates the Smokescreen's black smokes moving on the target.
// arg 0: x offset
// arg 1: y offset
// arg 2: x slide value
// arg 3: duration
void AnimBlackSmoke(struct Sprite *sprite)
{
    sprite->x += gBattleAnimArgs[0];
    sprite->y += gBattleAnimArgs[1];
	sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[1] = gBattleAnimArgs[3];
    sprite->callback = AnimBlackSmokeStep;
}

static void AnimBlackSmokeStep(struct Sprite *sprite)
{
    if (sprite->data[1] > 0)
    {
		sprite->data[1]--;
		
        sprite->x2 = sprite->data[2] >> 8;
        sprite->data[2] += sprite->data[0];
		
        sprite->invisible ^= TRUE;
    }
    else
        DestroyAnimSprite(sprite);
}

void AnimTask_SmokescreenImpact(u8 taskId)
{
	u8 mainSpriteId, spriteId1, spriteId2, spriteId3, spriteId4;
	struct Sprite *mainSprite;
	s16 x = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X) + 8;
	s16 y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + 8;

    if (GetSpriteTileStartByTag(sSmokescreenImpactSpriteSheet.tag) == 0xFFFF)
    {
        LoadCompressedSpriteSheetUsingHeap(&sSmokescreenImpactSpriteSheet);
        LoadCompressedSpritePaletteUsingHeap(&sSmokescreenImpactSpritePalette);
    }

    mainSpriteId = CreateInvisibleSpriteWithCallback(SmokescreenImpact_Callback);
    mainSprite = &gSprites[mainSpriteId];

    spriteId1 = CreateSprite(&sSmokescreenImpactSpriteTemplate, x - 16, y - 16, 2);
    gSprites[spriteId1].data[0] = mainSpriteId;
    mainSprite->data[0]++;
    AnimateSprite(&gSprites[spriteId1]);

    spriteId2 = CreateSprite(&sSmokescreenImpactSpriteTemplate, x, y - 16, 2);
    gSprites[spriteId2].data[0] = mainSpriteId;
    mainSprite->data[0]++;
    StartSpriteAnim(&gSprites[spriteId2], 1);
    AnimateSprite(&gSprites[spriteId2]);

    spriteId3 = CreateSprite(&sSmokescreenImpactSpriteTemplate, x - 16, y, 2);
    gSprites[spriteId3].data[0] = mainSpriteId;
    mainSprite->data[0]++;
    StartSpriteAnim(&gSprites[spriteId3], 2);
    AnimateSprite(&gSprites[spriteId3]);

    spriteId4 = CreateSprite(&sSmokescreenImpactSpriteTemplate, x, y, 2);
    gSprites[spriteId4].data[0] = mainSpriteId;
    mainSprite->data[0]++;
    StartSpriteAnim(&gSprites[spriteId4], 3);
    AnimateSprite(&gSprites[spriteId4]);

    DestroyAnimVisualTask(taskId);
}

static void SmokescreenImpact_Callback(struct Sprite *sprite)
{
    if (!sprite->data[0])
    {
        FreeSpriteTilesByTag(sSmokescreenImpactSpriteSheet.tag);
        FreeSpritePaletteByTag(sSmokescreenImpactSpritePalette.tag);
		DestroySprite(sprite);
    }
}

static void SmokescreenImpact_Callback_DestroySprite(struct Sprite *sprite)
{
    if (sprite->animEnded)
    {
        gSprites[sprite->data[0]].data[0]--;
        DestroySprite(sprite);
    }
}

// Animates Mind Reader's white halo on the target.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: anim battler
void AnimWhiteHalo(struct Sprite *sprite)
{
	InitSpritePosToAnimBattler(sprite, gBattleAnimArgs[2], TRUE);
	
    sprite->data[0] = 90;
    sprite->data[1] = 7;
	sprite->callback = WaitAnimForDuration;
    StoreSpriteCallbackInData6(sprite, AnimWhiteHalo_Step1);
	
	SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sprite->data[1], 16 - sprite->data[1]));
}

static void AnimWhiteHalo_Step1(struct Sprite *sprite)
{
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sprite->data[1], 16 - sprite->data[1]));
	
    if (--sprite->data[1] < 0)
    {
        sprite->invisible = TRUE;
        sprite->callback = DestroyAnimSpriteAndDisableBlend;
    }
}

// Animates Mind Reader's eyes sparkle.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: movement speed
// arg 3: anim battler
void AnimTealAlert(struct Sprite *sprite)
{
	u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[3]);
    u16 rotation;
    u8 x = GetBattlerSpriteCoord(battler, BATTLER_COORD_X);
    u8 y = GetBattlerSpriteCoord(battler, BATTLER_COORD_Y_PIC_OFFSET);

    InitSpritePosToAnimBattler(sprite, gBattleAnimArgs[3], TRUE);
	
    rotation = ArcTan2Neg(sprite->x - x, sprite->y - y);
    rotation += 0x6000;
    TrySetSpriteRotScale(sprite, FALSE, 0x100, 0x100, rotation);
	
    sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[2] = x;
    sprite->data[4] = y;
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Animates MOVE_MEAN_LOOK's eye sprite.
// No args.
void AnimMeanLookEye(struct Sprite *sprite)
{
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 16));
	
    sprite->data[0] = 4;
    sprite->callback = AnimMeanLookEye_Step1;
}

static void AnimMeanLookEye_Step1(struct Sprite *sprite)
{
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sprite->data[0], 16 - sprite->data[0]));
	
    if (sprite->data[1])
        sprite->data[0]--;
    else
        sprite->data[0]++;

    if (sprite->data[0] == 15 || sprite->data[0] == 4)
        sprite->data[1] ^= 1;

    if (sprite->data[2]++ > 70)
    {
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
		
        StartSpriteAffineAnim(sprite, 1);
		
        sprite->data[2] = 0;
        sprite->invisible = TRUE;
        sprite->affineAnimPaused = TRUE;
        sprite->callback = AnimMeanLookEye_Step2;
    }
}

static void AnimMeanLookEye_Step2(struct Sprite *sprite)
{
    if (sprite->data[2]++ > 9)
    {
        sprite->invisible = FALSE;
        sprite->affineAnimPaused = FALSE;
		
        if (sprite->affineAnimEnded)
            sprite->callback = AnimMeanLookEye_Step3;
    }
}

static void AnimMeanLookEye_Step3(struct Sprite *sprite)
{
    switch (sprite->data[3])
    {
    case 0:
    case 1:
        sprite->x2 = 1;
        sprite->y2 = 0;
        break;
    case 2:
    case 3:
        sprite->x2 = -1;
        sprite->y2 = 0;
        break;
    case 4:
    case 5:
        sprite->x2 = 0;
        sprite->y2 = 1;
        break;
    case 6:
    default:
        sprite->x2 = 0;
        sprite->y2 = -1;
        break;
    }

    if (++sprite->data[3] > 7)
        sprite->data[3] = 0;

    if (sprite->data[4]++ > 15)
    {
        sprite->data[0] = 16;
        sprite->data[1] = 0;
		
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sprite->data[0], 0));
		
        sprite->callback = AnimMeanLookEye_Step4;
    }
}

static void AnimMeanLookEye_Step4(struct Sprite *sprite)
{
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sprite->data[0], 16 - sprite->data[0]));
	
    if (sprite->data[1]++ > 1)
    {
		sprite->data[1] = 0;
        sprite->data[0]--;
    }

    if (sprite->data[0] == 0)
        sprite->invisible = TRUE;

    if (sprite->data[0] < 0)
        DestroyAnimSpriteAndDisableBlend(sprite);
}

// Animates the bg fading to white, used by MOVE_LUSTER_PURGE and MOVE_PSYCHO_BOOST.
// No args.
void AnimTask_FadeScreenToWhite(u8 taskId)
{
    gTasks[taskId].func = FadeScreenToWhite_Step;
    gAnimVisualTaskCount--;
}

static void FadeScreenToWhite_Step(u8 taskId)
{
    int i;
    u16 lastColor;
    u8 paletteIndex = 2; // battle bg palette num

    if (++gTasks[taskId].data[5] == 4)
    {
        lastColor = gPlttBufferFaded[paletteIndex * 16 + 11];
		
        for (i = 10; i > 0; i--)
            gPlttBufferFaded[paletteIndex * 16 + i + 1] = gPlttBufferFaded[paletteIndex * 16 + i];
		
        gPlttBufferFaded[paletteIndex * 16 + 1] = lastColor;

        lastColor = gPlttBufferUnfaded[paletteIndex * 16 + 11];
		
        for (i = 10; i > 0; i--)
            gPlttBufferUnfaded[paletteIndex * 16 + i + 1] = gPlttBufferUnfaded[paletteIndex * 16 + i];
		
        gPlttBufferUnfaded[paletteIndex * 16 + 1] = lastColor;

        gTasks[taskId].data[5] = 0;
    }
	
	// Signal to end effect
    if (gBattleAnimArgs[ARG_RET_ID] == -1)
        DestroyTask(taskId);
}

// Animates the leer sprite on the user.
// arg 0: x offset
// arg 1: y offset
void AnimLeer(struct Sprite *sprite)
{
    SetSpriteCoordsToAnimAttackerCoords(sprite);
    SetAnimSpriteInitialXOffset(sprite, gBattleAnimArgs[0]);
    sprite->y += gBattleAnimArgs[1];
    sprite->callback = RunStoredCallbackWhenAnimEnds;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Animates MOVE_SLEEP_TALK's Z letter sprite.
// arg 0: initial x pixel offset
// arg 1: x movement speed
// arg 2: y movement speed (also controls the sin direction)
void AnimLetterZ(struct Sprite *sprite)
{
    int var0;
    
    if (sprite->data[0] == 0)
    {
        SetSpriteCoordsToAnimAttackerCoords(sprite);
        SetAnimSpriteInitialXOffset(sprite, gBattleAnimArgs[0]);
        
        if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
		{
			gBattleAnimArgs[1] = -gBattleAnimArgs[1];
			gBattleAnimArgs[2] = -gBattleAnimArgs[2];
		}
        sprite->data[1] = gBattleAnimArgs[1];
        sprite->data[2] = gBattleAnimArgs[2];
    }
    sprite->data[0]++;
	
    var0 = (sprite->data[0] * 20) & 0xFF;
	
    sprite->data[3] += sprite->data[1];
    sprite->data[4] += sprite->data[2];
	
    sprite->x2 = sprite->data[3] / 2;
    sprite->y2 = Sin(var0 & 0xFF, 5) + (sprite->data[4] / 2);
	
    if ((u16)(sprite->x + sprite->x2) > 240)
        DestroyAnimSprite(sprite);
}

// Returns where a move should heal instead of cause damage.
// No args.
void AnimTask_IsHealingMove(u8 taskId)
{
	gBattleAnimArgs[ARG_RET_ID] = (gAnimMoveDmg < 0);
    DestroyAnimVisualTask(taskId);
}

static void InitSpotlightEffect(struct Sprite *sprite, u8 animBattler, void(*CB)(struct Sprite *))
{
	InitSpritePosToAnimBattler(sprite, animBattler, FALSE);
	
	SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR | WINOUT_WINOBJ_BG_ALL | WINOUT_WINOBJ_OBJ);
    SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJWIN_ON);
	
    gBattle_WIN0H = 0;
    gBattle_WIN0V = 0;
	
    SetGpuReg(REG_OFFSET_WIN0H, gBattle_WIN0H);
    SetGpuReg(REG_OFFSET_WIN0V, gBattle_WIN0V);
	
	sprite->oam.objMode = ST_OAM_OBJ_WINDOW;
    sprite->invisible = TRUE;
	sprite->callback = CB;
}

// Animates MOVE_ENCORE's spotlight.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: anim battler
void AnimSpotlight(struct Sprite *sprite)
{
    InitSpotlightEffect(sprite, gBattleAnimArgs[2], AnimSpotlight_Step1);
}

static void AnimSpotlight_Step1(struct Sprite *sprite)
{
    switch (sprite->data[0])
    {
    case 0:
        sprite->invisible = FALSE;
		
        if (sprite->affineAnimEnded)
            sprite->data[0]++;
        break;
    case 1:
    case 3:
        sprite->data[1] += 117;
        sprite->x2 = sprite->data[1] >> 8;
		
        if (++sprite->data[2] == 21)
        {
            sprite->data[2] = 0;
            sprite->data[0]++;
        }
        break;
    case 2:
        sprite->data[1] -= 117;
        sprite->x2 = sprite->data[1] >> 8;
		
        if (++sprite->data[2] == 41)
        {
            sprite->data[2] = 0;
            sprite->data[0]++;
        }
        break;
    case 4:
        ChangeSpriteAffineAnim(sprite, 1);
        sprite->data[0]++;
        break;
    case 5:
        if (sprite->affineAnimEnded)
        {
            sprite->invisible = TRUE;
            sprite->callback = AnimSpotlight_Step2;
        }
        break;
    }
}

static void AnimSpotlight_Step2(struct Sprite *sprite)
{
    SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR | WINOUT_WINOBJ_BG_ALL | WINOUT_WINOBJ_OBJ | WINOUT_WINOBJ_CLR);
    SetGpuReg(REG_OFFSET_DISPCNT, GetGpuReg(REG_OFFSET_DISPCNT) ^ DISPCNT_OBJWIN_ON);
    DestroyAnimSprite(sprite);
}

// Animates MOVE_ENCORE's first clapping hand.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: dont flip horizontally (boolean)
// arg 3: duration (?)
void AnimClappingHand(struct Sprite *sprite)
{
    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X) + gBattleAnimArgs[0];
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y) + gBattleAnimArgs[1];

    sprite->oam.tileNum += 16;
	
    if (!gBattleAnimArgs[2])
    {
        sprite->oam.matrixNum = ST_OAM_HFLIP;
        sprite->x2 = -12;
        sprite->data[1] = 2;
    }
    else
    {
        sprite->x2 = 12;
        sprite->data[1] = -2;
    }
    sprite->data[0] = gBattleAnimArgs[3];
	
    if (sprite->data[3] != 255)
        sprite->data[3] = gBattleAnimArgs[2];

    sprite->callback = AnimClappingHand_Step;
}

static void AnimClappingHand_Step(struct Sprite *sprite)
{
    if (sprite->data[2] == 0)
    {
        sprite->x2 += sprite->data[1];
		
        if (sprite->x2 == 0)
        {
            sprite->data[2]++;
			
            if (sprite->data[3] == 0)
                PlaySE1WithPanning(SE_M_ENCORE, BattleAnimAdjustPanning(SOUND_PAN_ATTACKER));
        }
    }
    else
    {
        sprite->x2 -= sprite->data[1];
		
        if (abs(sprite->x2) == 12)
        {
            sprite->data[0]--;
            sprite->data[2]--;
        }

    }

    if (sprite->data[0] == 0)
        DestroyAnimSprite(sprite);
}

// Animates MOVE_ENCORE's second clapping hand. It creates an second set of hands in window mode for the effect.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: dont flip horizontally (boolean)
// arg 3: duration (?)
void AnimClappingHand2(struct Sprite *sprite)
{
    sprite->oam.objMode = ST_OAM_OBJ_WINDOW;
    sprite->data[3] = 255;
    AnimClappingHand(sprite);
}

// Inits the gpu regs for the Spotlight effect.
// No args.
void AnimTask_CreateSpotlight(u8 taskId)
{
    SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG_ALL | WININ_WIN1_OBJ);
	
    gBattle_WIN1H = WIN_RANGE(0, 0xF0);
    gBattle_WIN1V = WININ_WIN0_CLR | WIN_RANGE(0, 0x80) | WININ_WIN1_BG3 | WININ_WIN1_OBJ | WININ_WIN1_CLR | WIN_RANGE(0x40, 0);
	
    SetGpuReg(REG_OFFSET_WIN1H, WIN_RANGE(0, 0xF0));
    SetGpuReg(REG_OFFSET_WIN1V, gBattle_WIN1V);
	
    SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN1_ON);
	
    DestroyAnimVisualTask(taskId);
}

// Resets the gpu regs for the Spotlight effect.
// No args.
void AnimTask_RemoveSpotlight(u8 taskId)
{
    SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG_ALL | WININ_WIN1_OBJ | WININ_WIN1_CLR);
	
    gBattle_WIN1H = 0;
    gBattle_WIN1V = 0;
	
    ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN1_ON);
	
    DestroyAnimVisualTask(taskId);
}

// Animates the Rapid Spin's effect on the given battler.
// arg 0: anim battler
// arg 1: x pos
// arg 2: y pos
// arg 3: spin size (?)
// arg 4: x movement sin adder
// arg 5: y movement speed
void AnimRapidSpin(struct Sprite *sprite)
{
	u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);
	
    sprite->x = GetBattlerSpriteCoord(battler, BATTLER_COORD_X) + gBattleAnimArgs[1];
    sprite->y = GetBattlerSpriteCoord(battler, BATTLER_COORD_Y);

    sprite->y2 = gBattleAnimArgs[2];
	
    sprite->data[0] = (sprite->y2 > gBattleAnimArgs[3]);
    sprite->data[1] = 0;
    sprite->data[2] = gBattleAnimArgs[4];
    sprite->data[3] = gBattleAnimArgs[5];
    sprite->data[4] = gBattleAnimArgs[3];
    sprite->callback = AnimRapidSpin_Step;
}

static void AnimRapidSpin_Step(struct Sprite *sprite)
{
    sprite->data[1] = (sprite->data[1] + sprite->data[2]) & 0xFF;
	
    sprite->x2 = gSineTable[sprite->data[1]] >> 4;
    sprite->y2 += sprite->data[3];
	
    if (sprite->data[0])
    {
        if (sprite->y2 < sprite->data[4])
            DestroyAnimSprite(sprite);
    }
    else
    {
        if (sprite->y2 > sprite->data[4])
            DestroyAnimSprite(sprite);
    }
}

// Animates the MOVE_RAPID_SPIN mon elevating effect.
// arg 0: anim battler
// arg 1: ?
// arg 2: is the return anim (boolean)
void AnimTask_RapinSpinMonElevation(u8 taskId)
{
	u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);
	s16 y = GetBattlerYCoordWithElevation(battler);
    u8 priorityRank = GetBattlerSpriteBGPriorityRank(battler);
    s16 var2;
    int var3;
    int var4;
    s16 i;
    struct ScanlineEffectParams scanlineParams;
    struct Task *task = &gTasks[taskId];

    task->data[0] = y + 36;
    task->data[1] = task->data[0];
    task->data[2] = y - 33;
	
    if (task->data[2] < 0)
        task->data[2] = 0;

    task->data[3] = task->data[0];
    task->data[4] = 8;
    task->data[5] = gBattleAnimArgs[1];
    task->data[6] = 0;
    task->data[7] = 0;
	
	var3 = priorityRank == 1 ? gBattle_BG1_X : gBattle_BG2_X;
	var4 = var3 + DISPLAY_WIDTH;
	
	task->data[8] = var3;
    task->data[9] = var4;
    task->data[10] = gBattleAnimArgs[2];
	
    if (!gBattleAnimArgs[2])
    {
        task->data[11] = var4;
        var2 = task->data[8];
    }
    else
    {
        task->data[11] = var3;
        var2 = task->data[9];
    }
    task->data[15] = 0;
	
    i = task->data[2];
    while (i <= task->data[3])
    {
        gScanlineEffectRegBuffers[0][i] = var2;
        gScanlineEffectRegBuffers[1][i] = var2;
        i++;
    }
	
	scanlineParams.dmaDest = priorityRank == 1 ? &REG_BG1HOFS : &REG_BG2HOFS;
    scanlineParams.dmaControl = SCANLINE_EFFECT_DMACNT_16BIT;
    scanlineParams.initState = 1;
    ScanlineEffect_SetParams(scanlineParams);

    task->func = RapinSpinMonElevation_Step;
}

static void RapinSpinMonElevation_Step(u8 taskId)
{
    s16 i;
    struct Task *task = &gTasks[taskId];

    task->data[0] -= task->data[5];
    if (task->data[0] < task->data[2])
        task->data[0] = task->data[2];

    if (task->data[4] == 0)
    {
        task->data[1] -= task->data[5];
		
        if (task->data[1] < task->data[2])
        {
            task->data[1] = task->data[2];
            task->data[15] = 1;
        }
    }
    else
        task->data[4]--;

    if (++task->data[6] > 1)
    {
        task->data[6] = 0;
        task->data[7] ^= TRUE;

        if (task->data[7])
            task->data[12] = task->data[8];
        else
            task->data[12] = task->data[9];
    }

    i = task->data[0];
    while (i < task->data[1])
    {
        gScanlineEffectRegBuffers[0][i] = task->data[12];
        gScanlineEffectRegBuffers[1][i] = task->data[12];
        i++;
    }

    i = task->data[1];
    while (i <= task->data[3])
    {
        gScanlineEffectRegBuffers[0][i] = task->data[11];
        gScanlineEffectRegBuffers[1][i] = task->data[11];
        i++;
    }

    if (task->data[15])
    {
        if (task->data[10])
            gScanlineEffect.state = 3;

        DestroyAnimVisualTask(taskId);
    }
}

// Animates the Tri Attack triangle.
// arg 0: x pixel offset
// arg 1: y pixel offset
void AnimTriAttackTriangle(struct Sprite *sprite)
{
    if (sprite->data[0] == 0)
        InitSpritePosToAnimAttacker(sprite, FALSE);

    if (++sprite->data[0] < 40)
    {
        if (!((u16)sprite->data[0] & 1))
            sprite->invisible = TRUE;
        else
            sprite->invisible = FALSE;
    }

    if (sprite->data[0] > 30)
        sprite->invisible = FALSE;

    if (sprite->data[0] == 61)
    {
        SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
		
        sprite->data[0] = 20;
        sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
        sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
        sprite->callback = StartAnimLinearTranslation;
		StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
    }
}

void AnimTask_DefenseCurlDeformMon(u8 taskId)
{
	PrepareAffineAnimInTaskData(&gTasks[taskId], GetAnimBattlerSpriteId(ANIM_ATTACKER), sDefenseCurlDeformMonAffineAnimCmds);
	gTasks[taskId].func = AnimTask_DestroyTaskAfterAffineAnimFromTaskDataEnds;
}

// Animates a wish sprite moving across the screen.
// No args.
void AnimWishStar(struct Sprite *sprite)
{
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        sprite->x = -16;
    else
        sprite->x = 256;

    sprite->y = 0;
    sprite->callback = AnimWishStar_Step;
}

static void AnimWishStar_Step(struct Sprite *sprite)
{
    u32 newX;

    sprite->data[0] += 72;
	
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        sprite->x2 = sprite->data[0] >> 4;
    else
        sprite->x2 = -(sprite->data[0] >> 4);

    sprite->data[1] += 16;
    sprite->y2 += sprite->data[1] >> 8;

    if (++sprite->data[2] % 3 == 0)
        CreateSpriteAndAnimate(&gMiniTwinklingStarSpriteTemplate, sprite->x + sprite->x2, sprite->y + sprite->y2, sprite->subpriority + 1);

    newX = sprite->x + sprite->x2 + 32;
    if (newX > DISPLAY_WIDTH + 64)
        DestroyAnimSprite(sprite);
}

// Animates little twinkling stars. Used by moves like Wish and Meteor Mash.
// No args.
void AnimMiniTwinklingStar(struct Sprite *sprite)
{
	s8 y;
    u8 rand = RandomMax(4);

    if (rand == 0)
        sprite->oam.tileNum += 4;
    else
        sprite->oam.tileNum += 5;

    y = RandomMax(8);
    if (y > 3)
        y = -y;

    sprite->y2 = y;
    sprite->callback = AnimMiniTwinklingStar_Step;
}

static void AnimMiniTwinklingStar_Step(struct Sprite *sprite)
{
    if (++sprite->data[0] < 30)
    {
        if (++sprite->data[1] == 2)
        {
            sprite->invisible ^= TRUE;
            sprite->data[1] = 0;
        }
    }
    else
    {
        if (sprite->data[1] == 2)
            sprite->invisible = FALSE;

        if (sprite->data[1] == 3)
        {
            sprite->invisible = TRUE;
            sprite->data[1] = -1;
        }
        sprite->data[1]++;
    }

    if (sprite->data[0] > 60)
        DestroySpriteAndFreeMatrix(sprite);
}

// Animates the attacker deformation in MOVE_STOCKPILE's anim.
// No args.
void AnimTask_StockpileDeformMon(u8 taskId)
{
	PrepareAffineAnimInTaskData(&gTasks[taskId], GetAnimBattlerSpriteId(ANIM_ATTACKER), sStockpileDeformMonAffineAnimCmds);
	gTasks[taskId].func = AnimTask_DestroyTaskAfterAffineAnimFromTaskDataEnds;
}

// Animates the attacker deformation in MOVE_SPIT_UP's anim.
// No args.
void AnimTask_SpitUpDeformMon(u8 taskId)
{
	PrepareAffineAnimInTaskData(&gTasks[taskId], GetAnimBattlerSpriteId(ANIM_ATTACKER), sSpitUpDeformMonAffineAnimCmds);
	gTasks[taskId].func = AnimTask_DestroyTaskAfterAffineAnimFromTaskDataEnds;
}

// Animates the MOVE_SWALLOW's blue orbs raising offscreen and then back to the attacker.
// arg 0: x pixel offset
// arg 1: y pixel offset
void AnimSwallowBlueOrb(struct Sprite *sprite)
{
    switch (sprite->data[0])
    {
    case 0:
        InitSpritePosToAnimAttacker(sprite, FALSE);
		
        sprite->data[1] = 0x900;
        sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
        sprite->data[0]++;
        break;
    case 1:
        sprite->y2 -= sprite->data[1] >> 8;
		
        sprite->data[1] -= 96;
		
        if (sprite->y + sprite->y2 > sprite->data[2])
            DestroyAnimSprite(sprite);
        break;
    }
}

// Animates the attacker deformation in MOVE_SWALLOW's anim.
// No args.
void AnimTask_SwallowDeformMon(u8 taskId)
{
	PrepareAffineAnimInTaskData(&gTasks[taskId], GetAnimBattlerSpriteId(ANIM_ATTACKER), sSwallowDeformMonAffineAnimCmds);
	gTasks[taskId].func = AnimTask_DestroyTaskAfterAffineAnimFromTaskDataEnds;
}

// Animates the sprite mosaic effect on change form.
// arg 0: anim battler to transform into
// arg 1: gfx special flags
void AnimTask_TransformMon(u8 taskId)
{
    struct BattleAnimBgData animBg;
    u16 stretch;

    switch (gTasks[taskId].data[0])
    {
    case 0:
        SetGpuReg(REG_OFFSET_MOSAIC, 0);
		
		SetAnimBgAttribute(GetBattlerSpriteBGPriorityRank(gBattleAnimAttacker), BG_ANIM_MOSAIC, 1);
		
        gTasks[taskId].data[10] = gBattleAnimArgs[0];
		gTasks[taskId].data[11] = gBattleAnimArgs[1];
		
        gTasks[taskId].data[0]++;
        break;
    case 1:
        if (gTasks[taskId].data[2]++ > 1)
        {
            gTasks[taskId].data[2] = 0;
			
            gTasks[taskId].data[1]++;
			
            stretch = gTasks[taskId].data[1];
            SetGpuReg(REG_OFFSET_MOSAIC, (stretch << 4) | stretch);
			
            if (stretch == 15)
                gTasks[taskId].data[0]++;
        }
        break;
    case 2:
        HandleSpeciesGfxDataChange(gBattleAnimAttacker, GetBattlerForAnimScript(gTasks[taskId].data[10]), gTasks[taskId].data[11]);
		
        GetBattleAnimBgDataByPriorityRank(&animBg);
        CpuCopy32(gMonSpritesGfxPtr->sprites[GetBattlerPosition(gBattleAnimAttacker)], animBg.bgTiles, 0x800);
        LoadBgTiles(1, animBg.bgTiles, 0x800, animBg.tilesOffset);
		
        gTasks[taskId].data[0]++;
        break;
    case 3:
        if (gTasks[taskId].data[2]++ > 1)
        {
            gTasks[taskId].data[2] = 0;
			
            gTasks[taskId].data[1]--;
			
            stretch = gTasks[taskId].data[1];
            SetGpuReg(REG_OFFSET_MOSAIC, (stretch << 4) | stretch);

            if (stretch == 0)
                gTasks[taskId].data[0]++;
        }
        break;
    case 4:
        SetGpuReg(REG_OFFSET_MOSAIC, 0);
		SetAnimBgAttribute(GetBattlerSpriteBGPriorityRank(gBattleAnimAttacker), BG_ANIM_MOSAIC, 0);
		SetBattlerShadowSpriteCallback(gBattleAnimAttacker);
        DestroyAnimVisualTask(taskId);
        break;
    }
}

// Animates MOVE_MORNING_SUN's light rays on the attacker.
// No args.
void AnimTask_MorningSunLightBeam(u8 taskId)
{
    struct BattleAnimBgData animBg;

    switch (gTasks[taskId].data[0])
    {
    case 0:
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_BG1);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 16));
		
        SetAnimBgAttribute(1, BG_ANIM_SCREEN_SIZE, 0);
        SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
        SetAnimBgAttribute(1, BG_ANIM_CHAR_BASE_BLOCK, 1);
		
        GetBattleAnimBgData(&animBg, 1);
        AnimLoadCompressedBgTilemap(animBg.bgId, gBattleAnim_MorningSunTilemap);
        AnimLoadCompressedBgGfx(animBg.bgId, gBattleAnim_MorningSunGfx, animBg.tilesOffset);
        LoadCompressedPalette(gBattleAnim_MorningSunPal, animBg.paletteId * 16, 32);
		
		gBattle_BG1_X = (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER) ? -135 : -10;
        gBattle_BG1_Y = 0;
		
        gTasks[taskId].data[10] = gBattle_BG1_X;
        gTasks[taskId].data[11] = gBattle_BG1_Y;
		
        PlaySE12WithPanning(SE_M_MORNING_SUN, BattleAnimAdjustPanning(SOUND_PAN_ATTACKER));
		
		gTasks[taskId].data[0]++;
        break;
    case 1:
        if (gTasks[taskId].data[4]++ > 0)
        {
            gTasks[taskId].data[4] = 0;
			
            if (++gTasks[taskId].data[1] > 12)
                gTasks[taskId].data[1] = 12;

            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[1], 16 - gTasks[taskId].data[1]));
			
            if (gTasks[taskId].data[1] == 12)
                gTasks[taskId].data[0]++;
        }
        break;
    case 2:
        if (--gTasks[taskId].data[1] < 0)
            gTasks[taskId].data[1] = 0;

        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[1], 16 - gTasks[taskId].data[1]));
		
        if (!gTasks[taskId].data[1])
        {
            gBattle_BG1_X = sMorningSunLightBeamCoordsTable[gTasks[taskId].data[2]] + gTasks[taskId].data[10];
			
            if (++gTasks[taskId].data[2] == 4)
                gTasks[taskId].data[0] = 4;
            else
                gTasks[taskId].data[0] = 3;
        }
        break;
    case 3:
        if (++gTasks[taskId].data[3] == 4)
        {
            gTasks[taskId].data[3] = 0;
            gTasks[taskId].data[0] = 1;
			
            PlaySE12WithPanning(SE_M_MORNING_SUN, BattleAnimAdjustPanning(SOUND_PAN_ATTACKER));
        }
        break;
    case 4:
        GetBattleAnimBgData(&animBg, 1);
        InitBattleAnimBg(animBg.bgId);
		
        SetAnimBgAttribute(1, BG_ANIM_CHAR_BASE_BLOCK, 0);
        SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
		
        gBattle_BG1_X = 0;
        gBattle_BG1_Y = 0;

        DestroyAnimVisualTaskAndDisableBlend(taskId);
        break;
    }
}

static u8 CreateMorningSunGreenStar(struct Sprite *sprite, u8 animNum)
{
	u8 spriteId = CreateSprite(&gGreenStarSpriteTemplate, sprite->x, sprite->y, sprite->subpriority + 1);
	
	StartSpriteAnim(&gSprites[spriteId], animNum);
	
	gSprites[spriteId].data[1] = sprite->data[1];
    gSprites[spriteId].data[2] = sprite->data[2];
	gSprites[spriteId].data[7] = -1;
	
	gSprites[spriteId].invisible = TRUE;
    gSprites[spriteId].callback = AnimGreenStar_Callback;
	
	return spriteId;
}

// Animates MOVE_MORNING_SUN's healing stars.
// arg 0: duration
// arg 1: y movement speed
void AnimGreenStar(struct Sprite *sprite)
{
    s16 xOffset = Random() & 0x3F;
	
    if (xOffset > 31)
        xOffset = 32 - xOffset;

    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X) + xOffset;
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y) + 32;
	
    sprite->data[1] = gBattleAnimArgs[0];
    sprite->data[2] = gBattleAnimArgs[1];
	
	sprite->data[6] = CreateMorningSunGreenStar(sprite, 1);
    sprite->data[7] = CreateMorningSunGreenStar(sprite, 2);
    sprite->callback = AnimGreenStar_Step1;
}

static void AnimGreenStar_Step1(struct Sprite *sprite)
{
    s16 delta = sprite->data[3] + sprite->data[2];
    
    sprite->y2 -= delta >> 8;
	
    sprite->data[3] += sprite->data[2];
    sprite->data[3] &= 0xFF;
	
    if (sprite->data[4] == 0 && sprite->y2 < -8)
    {
        gSprites[sprite->data[6]].invisible = FALSE;
        sprite->data[4]++;
    }

    if (sprite->data[4] == 1 && sprite->y2 < -16)
    {
        gSprites[sprite->data[7]].invisible = FALSE;
        sprite->data[4]++;
    }

    if (--sprite->data[1] == -1)
    {
        sprite->invisible = TRUE;
        sprite->callback = AnimGreenStar_Step2;
    }
}

static void AnimGreenStar_Step2(struct Sprite *sprite)
{
    if (gSprites[sprite->data[6]].callback == SpriteCallbackDummy && gSprites[sprite->data[7]].callback == SpriteCallbackDummy)
    {
        DestroySprite(&gSprites[sprite->data[6]]);
        DestroySprite(&gSprites[sprite->data[7]]);
        DestroyAnimSprite(sprite);
    }
}

static void AnimGreenStar_Callback(struct Sprite *sprite)
{
    if (!sprite->invisible)
    {
        s16 delta = sprite->data[3] + sprite->data[2];
		
        sprite->y2 -= delta >> 8;
		
        sprite->data[3] += sprite->data[2];
        sprite->data[3] &= 0xFF;
		
        if (--sprite->data[1] == -1)
        {
            sprite->invisible = TRUE;
            sprite->callback = SpriteCallbackDummy;
        }
    }
}

// Animates Doom Desire's light beam effect.
// No args.
void AnimTask_DoomDesireLightBeam(u8 taskId)
{
    struct BattleAnimBgData animBg;
    u8 position;
    
    switch (gTasks[taskId].data[0])
    {
    case 0:
        position = GetBattlerPosition(gBattleAnimTarget);
		
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_BG1);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(3, 13));
		
        SetAnimBgAttribute(1, BG_ANIM_SCREEN_SIZE, 0);
        SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
        SetAnimBgAttribute(1, BG_ANIM_CHAR_BASE_BLOCK, 1);
		
        GetBattleAnimBgData(&animBg, 1);
        AnimLoadCompressedBgTilemap(animBg.bgId, gBattleAnim_MorningSunTilemap);
        AnimLoadCompressedBgGfx(animBg.bgId, gBattleAnim_MorningSunGfx, animBg.tilesOffset);
        LoadCompressedPalette(gBattleAnim_MorningSunPal, animBg.paletteId * 16, 32);
            
        if (IsDoubleBattleForBattler(gBattleAnimTarget))
        {
			switch (position)
			{
				case B_POSITION_OPPONENT_LEFT:
				    gBattle_BG1_X = -155;
					break;
				case B_POSITION_OPPONENT_RIGHT:
				    gBattle_BG1_X = -115;
					break;
				case B_POSITION_PLAYER_LEFT:
				    gBattle_BG1_X = 14;
					break;
				case B_POSITION_PLAYER_RIGHT:
				    gBattle_BG1_X = -20;
					break;
			}
        }
        else
        {
            if (position == B_POSITION_OPPONENT_LEFT)
                gBattle_BG1_X = -135;
            else if (position == B_POSITION_PLAYER_LEFT)
                gBattle_BG1_X = -10;
        }
        gBattle_BG1_Y = 0;
		
        gTasks[taskId].data[10] = gBattle_BG1_X;
        gTasks[taskId].data[11] = gBattle_BG1_Y;
        gTasks[taskId].data[0]++;
        break;
    case 1:
        gTasks[taskId].data[3] = 0;
		
        if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_OPPONENT)
            gBattle_BG1_X = gTasks[taskId].data[10] + sDoomDesireLightBeamCoordTable[gTasks[taskId].data[2]];
        else
            gBattle_BG1_X = gTasks[taskId].data[10] - sDoomDesireLightBeamCoordTable[gTasks[taskId].data[2]];

        if (++gTasks[taskId].data[2] == 5)
            gTasks[taskId].data[0] = 5;
        else
            gTasks[taskId].data[0]++;
        break;
    case 2:
        if (--gTasks[taskId].data[1] <= 4)
            gTasks[taskId].data[1] = 5;

        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(3, gTasks[taskId].data[1]));
		
        if (gTasks[taskId].data[1] == 5)
            gTasks[taskId].data[0]++;
        break;
    case 3:
        if (++gTasks[taskId].data[3] > sDoomDesireLightBeamDelayTable[gTasks[taskId].data[2]])
            gTasks[taskId].data[0]++;
        break;
    case 4:
        if (++gTasks[taskId].data[1] > 13)
            gTasks[taskId].data[1] = 13;

        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(3, gTasks[taskId].data[1]));
		
        if (gTasks[taskId].data[1] == 13)
            gTasks[taskId].data[0] = 1;
        break;
    case 5:
        GetBattleAnimBgData(&animBg, 1);
        InitBattleAnimBg(animBg.bgId);
		
        SetAnimBgAttribute(1, BG_ANIM_CHAR_BASE_BLOCK, 0);
        SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
		
        gBattle_BG1_X = 0;
        gBattle_BG1_Y = 0;
		
        DestroyAnimVisualTaskAndDisableBlend(taskId);
        break;
    }
}

// Briefly vertically grows and shrinks the attacking mon's sprite.
// No args.
void AnimTask_StrongFrustrationGrowAndShrink(u8 taskId)
{
	PrepareAffineAnimInTaskData(&gTasks[taskId], GetAnimBattlerSpriteId(ANIM_ATTACKER), sStrongFrustrationAffineAnimCmds);
	gTasks[taskId].func = AnimTask_DestroyTaskAfterAffineAnimFromTaskDataEnds;
}

// Animates a falling anger mark near the mon's head. Used by MOVE_FRUSTRATION's weak power level.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
void AnimWeakFrustrationAngerMark(struct Sprite *sprite)
{
    if (!sprite->data[0])
    {
        InitSpritePosToAnimAttacker(sprite, FALSE);
        sprite->data[0]++;
    }
    else if (sprite->data[0]++ > 20)
    {
        sprite->data[1] += 160;
        sprite->data[2] += 128;
		
		sprite->x2 = GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER ? -(sprite->data[1] >> 8) : (sprite->data[1] >> 8);
        sprite->y2 += sprite->data[2] >> 8;
		
        if (sprite->y2 > 64)
            DestroyAnimSprite(sprite);
    }
}

// Rocks the mon back and forth. This is done on a pivot so it is done via rotation.
// arg 0: which battler
// arg 1: number of rocks
// arg 2: rotation speed increase
void AnimTask_RockMonBackAndForth(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    if (!gBattleAnimArgs[1])
    {
        DestroyAnimVisualTask(taskId);
        return;
    }

    if (gBattleAnimArgs[2] < 0)
        gBattleAnimArgs[2] = 0;
    else if (gBattleAnimArgs[2] > 2)
        gBattleAnimArgs[2] = 2;

    task->data[0] = 0;
    task->data[1] = 0;
    task->data[2] = 0;
    task->data[3] = 8 - (2 * gBattleAnimArgs[2]);
    task->data[4] = 0x100 + (gBattleAnimArgs[2] * 128);
    task->data[5] = gBattleAnimArgs[2] + 2;
    task->data[6] = gBattleAnimArgs[1] - 1;
    task->data[15] = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);
	
    if (GetBattlerSide(GetBattlerForAnimScript(gBattleAnimArgs[0])) == B_SIDE_OPPONENT)
    {
        task->data[4] *= -1;
        task->data[5] *= -1;
    }
    PrepareBattlerSpriteForRotScale(task->data[15], ST_OAM_OBJ_NORMAL);
    task->func = AnimTask_RockMonBackAndForthStep;
}

static void AnimTask_RockMonBackAndForthStep(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        gSprites[task->data[15]].x2 += task->data[5];
        task->data[2] -= task->data[4];
		
        SetSpriteRotScale(task->data[15], 0x100, 0x100, task->data[2]);
        SetBattlerSpriteYOffsetFromRotation(task->data[15]);
		
        if (++task->data[1] >= task->data[3])
        {
            task->data[1] = 0;
            task->data[0]++;
        }
        break;
    case 1:
        gSprites[task->data[15]].x2 -= task->data[5];
        task->data[2] += task->data[4];
		
        SetSpriteRotScale(task->data[15], 0x100, 0x100, task->data[2]);
        SetBattlerSpriteYOffsetFromRotation(task->data[15]);
		
        if (++task->data[1] >= task->data[3] * 2)
        {
            task->data[1] = 0;
            task->data[0]++;
        }
        break;
    case 2:
        gSprites[task->data[15]].x2 += task->data[5];
        task->data[2] -= task->data[4];
		
        SetSpriteRotScale(task->data[15], 0x100, 0x100, task->data[2]);
        SetBattlerSpriteYOffsetFromRotation(task->data[15]);
		
        if (++task->data[1] >= task->data[3])
        {
            if (task->data[6])
            {
                task->data[6]--;
                task->data[1] = 0;
                task->data[0] = 0;
            }
            else
                task->data[0]++;
        }
        break;
    case 3:
        ResetSpriteRotScale(task->data[15]);
        DestroyAnimVisualTask(taskId);
        break;
    }
}

// Floats a petal across the screen towards the target mon's side.
// arg 0: initial y pixel offset
// arg 1: sprite anim num
void AnimSweetScentPetal(struct Sprite *sprite)
{
    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
    {
        sprite->x = 0;
        sprite->y = gBattleAnimArgs[0];
    }
    else
    {
        sprite->x = 240;
        sprite->y = gBattleAnimArgs[0] - 30;
    }
    StartSpriteAnim(sprite, gBattleAnimArgs[1]);
    sprite->callback = AnimSweetScentPetalStep;
}

static void AnimSweetScentPetalStep(struct Sprite *sprite)
{
    sprite->data[0] += 3;
	
    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
    {
        sprite->x += 5;
        sprite->y--;

        if (sprite->x > 240)
            DestroyAnimSprite(sprite);

        sprite->y2 = Sin(sprite->data[0] & 0xFF, 16);
    }
    else
    {
        sprite->x -= 5;
        sprite->y++;

        if (sprite->x < 0)
            DestroyAnimSprite(sprite);

        sprite->y2 = Cos(sprite->data[0] & 0xFF, 16);
    }
}

// Moves the mon sprite in a flailing back-and-forth motion.
// arg 0: anim battler
void AnimTask_FlailMovement(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    task->data[0] = 0;
    task->data[1] = 0;
    task->data[2] = 0;
    task->data[3] = 0;
    task->data[12] = 0x20;
    task->data[13] = 0x40;
    task->data[14] = 0x800;
    task->data[15] = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);

    PrepareBattlerSpriteForRotScale(task->data[15], ST_OAM_OBJ_NORMAL);
    task->func = AnimTask_FlailMovementStep;
}

static void AnimTask_FlailMovementStep(u8 taskId)
{
    int temp;
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        task->data[2] += 0x200;
		
        if (task->data[2] >= task->data[14])
        {
            s16 diff = task->data[14] - task->data[2];
            s16 div = diff / (task->data[14] * 2);
            s16 mod = diff % (task->data[14] * 2);

            if (!(div & 1))
            {
                task->data[2] = task->data[14] - mod;
                task->data[0] = 1;
            }
            else
                task->data[2] = mod - task->data[14];
        }
        break;
    case 1:
        task->data[2] -= 0x200;
		
        if (task->data[2] <= -task->data[14])
        {
            s16 diff = task->data[14] - task->data[2];
            s16 div = diff / (task->data[14] * 2);
            s16 mod = diff % (task->data[14] * 2);

            if (!(1 & div))
            {
                task->data[2] = mod - task->data[14];
                task->data[0] = 0;
            }
            else
                task->data[2] = task->data[14] - mod;
        }
        break;
    case 2:
        ResetSpriteRotScale(task->data[15]);
        DestroyAnimVisualTask(taskId);
        return;
    }
    SetSpriteRotScale(task->data[15], 0x100, 0x100, task->data[2]);
    SetBattlerSpriteYOffsetFromRotation(task->data[15]);
	
    gSprites[task->data[15]].x2 = -(((temp = task->data[2]) >= 0 ? task->data[2] : temp + 63) >> 6);
	
    if (++task->data[1] > 8)
    {
        if (task->data[12])
        {
            task->data[12]--;
			
            task->data[14] -= task->data[13];
			
            if (task->data[14] < 16)
                task->data[14] = 16;
        }
        else
            task->data[0] = 2;
    }
}

// Makes a spark-like projectile fall on top of the mon.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: which battler
void AnimPainSplitProjectile(struct Sprite *sprite)
{
    if (!sprite->data[0])
    {
        if (gBattleAnimArgs[2] == ANIM_ATTACKER)
        {
            sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
            sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
        }
        sprite->x += gBattleAnimArgs[0];
        sprite->y += gBattleAnimArgs[1];
		
        sprite->data[1] = 0x80;
        sprite->data[2] = 0x300;
        sprite->data[3] = gBattleAnimArgs[1];
        sprite->data[0]++;
    }
    else
    {
        sprite->x2 = sprite->data[1] >> 8;
        sprite->y2 += sprite->data[2] >> 8;
		
        if (sprite->data[4] == 0 && sprite->y2 > -sprite->data[3])
        {
            sprite->data[4] = 1;
            sprite->data[2] = (-sprite->data[2] / 3) * 2;
        }
        sprite->data[1] += 192;
        sprite->data[2] += 128;
		
        if (sprite->animEnded)
            DestroyAnimSprite(sprite);
    }
}

// Performs one of several affine transformations on the mon sprite.
// arg 0: which battler
// arg 1: which transformation
void AnimTask_PainSplitMovement(u8 taskId)
{
    u8 spriteId;

    if (gTasks[taskId].data[0] == 0)
    {
		gTasks[taskId].data[11] = GetBattlerForAnimScript(gBattleAnimArgs[0]);

        spriteId = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);
        gTasks[taskId].data[10] = spriteId;
        PrepareBattlerSpriteForRotScale(spriteId, ST_OAM_OBJ_NORMAL);

        switch (gBattleAnimArgs[1])
        {
        case 0:
            SetSpriteRotScale(spriteId, 0xE0, 0x140, 0);
            SetBattlerSpriteYOffsetFromYScale(spriteId);
            break;
        case 1:
            SetSpriteRotScale(spriteId, 0xD0, 0x130, 0xF00);
            SetBattlerSpriteYOffsetFromYScale(spriteId);
			
            if (GetBattlerSide(gTasks[taskId].data[11]) == B_SIDE_PLAYER)
                gSprites[spriteId].y2 += 16;
            break;
        case 2:
            SetSpriteRotScale(spriteId, 0xD0, 0x130, 0xF100);
            SetBattlerSpriteYOffsetFromYScale(spriteId);
			
            if (GetBattlerSide(gTasks[taskId].data[11]) == B_SIDE_PLAYER)
                gSprites[spriteId].y2 += 16;
            break;
        }
        gSprites[spriteId].x2 = 2;
        gTasks[taskId].data[0]++;
    }
    else
    {
        spriteId = gTasks[taskId].data[10];
		
        if (++gTasks[taskId].data[2] == 3)
        {
            gTasks[taskId].data[2] = 0;
            gSprites[spriteId].x2 = -gSprites[spriteId].x2;
        }

        if (++gTasks[taskId].data[1] == 13)
        {
            ResetSpriteRotScale(spriteId);
            gSprites[spriteId].x2 = 0;
            gSprites[spriteId].y2 = 0;
            DestroyAnimVisualTask(taskId);
        }
    }
}

// Move a piece of confetti in a slightly-random speed across the screen.
// arg 0: which battler the confetti starts from
void AnimFlatterConfetti(struct Sprite *sprite)
{
    int rand1 = Random() & 0x1FF;
    int rand2 = Random() & 0xFF;

    sprite->oam.tileNum += RandomMax(12);

    if (rand1 & 1)
        sprite->data[0] = 0x5E0 + rand1;
    else
        sprite->data[0] = 0x5E0 - rand1;

    if (rand2 & 1)
        sprite->data[1] = 0x480 + rand2;
    else
        sprite->data[1] = 0x480 - rand2;

    sprite->data[2] = gBattleAnimArgs[0];
	
    if (sprite->data[2] == ANIM_ATTACKER)
        sprite->x = -8;
    else
        sprite->x = 248;

    sprite->y = 104;
    sprite->callback = AnimFlatterConfettiStep;
}

static void AnimFlatterConfettiStep(struct Sprite *sprite)
{
    if (sprite->data[2] == ANIM_ATTACKER)
        sprite->x2 += sprite->data[0] >> 8;
    else
        sprite->x2 -= sprite->data[0] >> 8;
	
	sprite->y2 -= sprite->data[1] >> 8;

    sprite->data[0] -= 22;
    sprite->data[1] -= 48;
	
    if (sprite->data[0] < 0)
        sprite->data[0] = 0;

    if (++sprite->data[3] == 31)
        DestroyAnimSprite(sprite);
}

// Uses a spotlight sprite as a light mask to illuminate the target mon. The spotlight grows and shrinks.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: duration of fully-opened spotlight
void AnimFlatterSpotlight(struct Sprite *sprite)
{
	InitSpotlightEffect(sprite, ANIM_TARGET, AnimFlatterSpotlightStep);
	sprite->data[0] = gBattleAnimArgs[2];
}

static void AnimFlatterSpotlightStep(struct Sprite *sprite)
{
    switch (sprite->data[1])
    {
    case 0:
        sprite->invisible = FALSE;
		
        if (sprite->affineAnimEnded)
            sprite->data[1]++;
        break;
    case 1:
        if (--sprite->data[0] == 0)
        {
            ChangeSpriteAffineAnim(sprite, 1);
            sprite->data[1]++;
        }
        break;
    case 2:
        if (sprite->affineAnimEnded)
        {
            sprite->invisible = TRUE;
            sprite->data[1]++;
        }
        break;
    case 3:
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR | WINOUT_WINOBJ_BG_ALL | WINOUT_WINOBJ_OBJ | WINOUT_WINOBJ_CLR);
        SetGpuReg(REG_OFFSET_DISPCNT, GetGpuReg(REG_OFFSET_DISPCNT) ^ DISPCNT_OBJWIN_ON);
		
        DestroyAnimSprite(sprite);
        break;
    }
}

// Spins an orb around the attacking mon, while its path radius grows and shrinks.
// arg 0: duration
// arg 1: initial wave offset
void AnimReversalOrb(struct Sprite *sprite)
{
    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
	
    sprite->data[0] = gBattleAnimArgs[0];
    sprite->data[1] = gBattleAnimArgs[1];
    sprite->callback = AnimReversalOrbStep;
    sprite->callback(sprite);
}

static void AnimReversalOrbStep(struct Sprite *sprite)
{
    sprite->x2 = Sin(sprite->data[1], sprite->data[2] >> 8);
    sprite->y2 = Cos(sprite->data[1], sprite->data[3] >> 8);
	
    sprite->data[1] = (sprite->data[1] + 9) & 0xFF;

    if ((u16)sprite->data[1] < 64 || sprite->data[1] > 195)
        sprite->subpriority = GetBattlerSpriteSubpriority(gBattleAnimAttacker) - 1;
    else
        sprite->subpriority = GetBattlerSpriteSubpriority(gBattleAnimAttacker) + 1;

    if (!sprite->data[5])
    {
        sprite->data[2] += 0x400;
        sprite->data[3] += 0x100;

        if (++sprite->data[4] == sprite->data[0])
        {
            sprite->data[4] = 0;
            sprite->data[5] = 1;
        }
    }
    else if (sprite->data[5] == 1)
    {
        sprite->data[2] -= 0x400;
        sprite->data[3] -= 0x100;
		
        if (++sprite->data[4] == sprite->data[0])
            DestroyAnimSprite(sprite);
    }
}

// Runs an affine animation that makes it look like the mon is inhaling deeply.
// arg 0: anim battler
void AnimTask_DeepInhale(u8 taskId)
{
    PrepareAffineAnimInTaskData(&gTasks[taskId], GetAnimBattlerSpriteId(gBattleAnimArgs[0]), sDeepInhaleAffineAnimCmds);
    gTasks[taskId].func = AnimTask_DeepInhaleStep;
}

static void AnimTask_DeepInhaleStep(u8 taskId)
{
    u16 var0;
    struct Task *task = &gTasks[taskId];
    
    var0 = task->data[0];
    task->data[0]++;
    var0 -= 20;
	
    if (var0 < 23)
    {
        if (++task->data[1] > 1)
        {
            task->data[1] = 0;

            if (++task->data[2] & 1)
                gSprites[task->data[15]].x2 = 1;
            else
                gSprites[task->data[15]].x2 = -1;
        }
    }
    else
        gSprites[task->data[15]].x2 = 0;

    if (!RunAffineAnimFromTaskData(task))
        DestroyAnimVisualTask(taskId);
}

// Drifts a cloud in a wavy path towards the target mon.
// arg 0: which affine anim
void AnimYawnCloud(struct Sprite *sprite)
{
    s16 destX = sprite->x;
    s16 destY = sprite->y;

    SetSpriteCoordsToAnimAttackerCoords(sprite);
    StartSpriteAffineAnim(sprite, gBattleAnimArgs[0]);
	
    sprite->data[4] = sprite->x << 4;
    sprite->data[5] = sprite->y << 4;
    sprite->data[6] = ((destX - sprite->x) << 4) / 64;
    sprite->data[7] = ((destY - sprite->y) << 4) / 64;
	
    sprite->data[0] = 0;
    sprite->callback = AnimYawnCloudStep;
}

static void AnimYawnCloudStep(struct Sprite *sprite)
{
    int index;

    sprite->data[0]++;
	
    index = (sprite->data[0] * 8) & 0xFF;
	
    sprite->data[4] += sprite->data[6];
    sprite->data[5] += sprite->data[7];
	
    sprite->x = sprite->data[4] >> 4;
    sprite->y = sprite->data[5] >> 4;
	
    sprite->y2 = Sin(index, 8);
	
    if (sprite->data[0] > 58)
    {
        if (++sprite->data[1] > 1)
        {
            sprite->data[1] = 0;
			
            sprite->data[2]++;
			
            sprite->invisible = sprite->data[2] & 1;
			
            if (sprite->data[2] > 3)
                DestroyAnimSprite(sprite);
        }
    }
}

// Animates a cloud coming from the smoke ball.
// arg 0: affine anim num
// arg 1: initial x pixel offset
// arg 2: initial y pixel offset
// arg 3: duration
void AnimSmokeBallEscapeCloud(struct Sprite *sprite)
{
    StartSpriteAffineAnim(sprite, gBattleAnimArgs[0]);
	
    if (GetBattlerSide(gBattleAnimTarget) != B_SIDE_PLAYER)
        gBattleAnimArgs[1] = -gBattleAnimArgs[1];

    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X) + gBattleAnimArgs[1];
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[2];
	
	sprite->data[0] = gBattleAnimArgs[3];
    sprite->callback = DestroyAnimSpriteAfterTimer;
}

static void AnimTask_SlideMonForFocusBand_Step2(u8 taskId)
{
    u16 var0 = 0;
    u16 var1 = 0;

    gTasks[taskId].data[0]--;
	
    if ((gTasks[taskId].data[6] & 0x8000) && (--gTasks[taskId].data[1] == -1))
    {
        if (gTasks[taskId].data[9] == 0)
        {
            gTasks[taskId].data[9] = gTasks[taskId].data[4];
            gTasks[taskId].data[4] = -gTasks[taskId].data[4];
        }
        else
            gTasks[taskId].data[9] = 0;

        if (gTasks[taskId].data[10] == 0)
        {
            gTasks[taskId].data[10] = gTasks[taskId].data[5];
            gTasks[taskId].data[5] = -gTasks[taskId].data[5];
        }
        else
            gTasks[taskId].data[10] = 0;

        gTasks[taskId].data[1] = gTasks[taskId].data[13];
    }
    var0 = gTasks[taskId].data[7];
    var1 = gTasks[taskId].data[8];
	
    if (gTasks[taskId].data[2] & 0x8000)
        gSprites[gTasks[taskId].data[15]].x2 = gTasks[taskId].data[9] - (var0 >> 8);
    else
        gSprites[gTasks[taskId].data[15]].x2 = gTasks[taskId].data[9] + (var0 >> 8);

    if (gTasks[taskId].data[3] & 0x8000)
        gSprites[gTasks[taskId].data[15]].y2 = gTasks[taskId].data[10] - (var1 >> 8);
    else
        gSprites[gTasks[taskId].data[15]].y2 = gTasks[taskId].data[10] + (var1 >> 8);

    if (gTasks[taskId].data[0] < 1)
        DestroyAnimVisualTask(taskId);
}

static void AnimTask_SlideMonForFocusBand_Step(u8 taskId)
{
    u16 var0 = 0;
    u16 var1 = 0;

    gTasks[taskId].data[0]--;
	
    if ((gTasks[taskId].data[6] & 0x8000) && (--gTasks[taskId].data[1] == -1))
    {
        if (gTasks[taskId].data[9] == 0)
        {
            gTasks[taskId].data[9] = gTasks[taskId].data[4];
            gTasks[taskId].data[4] = -gTasks[taskId].data[4];
        }
        else
            gTasks[taskId].data[9] = var0;

        if (gTasks[taskId].data[10] == 0)
        {
            gTasks[taskId].data[10] = gTasks[taskId].data[5];
            gTasks[taskId].data[5] = -gTasks[taskId].data[5];
        }
        else
            gTasks[taskId].data[10] = 0;

        gTasks[taskId].data[1] = gTasks[taskId].data[13];
    }
    var0 = (gTasks[taskId].data[2] & 0x7FFF) + gTasks[taskId].data[7];
    var1 = (gTasks[taskId].data[3] & 0x7FFF) + gTasks[taskId].data[8];
	
    if (gTasks[taskId].data[2] & 0x8000)
        gSprites[gTasks[taskId].data[15]].x2 = gTasks[taskId].data[9] - (var0 >> 8);
    else
        gSprites[gTasks[taskId].data[15]].x2 = gTasks[taskId].data[9] + (var0 >> 8);

    if (gTasks[taskId].data[3] & 0x8000)
        gSprites[gTasks[taskId].data[15]].y2 = gTasks[taskId].data[10] - (var1 >> 8);
    else
        gSprites[gTasks[taskId].data[15]].y2 = gTasks[taskId].data[10] + (var1 >> 8);

    gTasks[taskId].data[7] = var0;
    gTasks[taskId].data[8] = var1;
	
    if (gTasks[taskId].data[0] < 1)
    {
        gTasks[taskId].data[0] = 30;
        gTasks[taskId].data[13] = 0;
        gTasks[taskId].func = AnimTask_SlideMonForFocusBand_Step2;
    }
}

// Animates the attacker's slide movement in Focus Band activation anim.
// arg 0: slide in duration
// arg 1: slide amount
// arg 2: shrink amount
// arg 3: (?) (boolean)
// arg 4: num shinks (?)
// arg 5: (?)
// arg 6: (?)
void AnimTask_SlideMonForFocusBand(u8 taskId)
{
    gTasks[taskId].data[15] = gBattlerSpriteIds[gBattleAnimAttacker];
    gTasks[taskId].data[14] = gBattleAnimArgs[0];
    gTasks[taskId].data[0] = gBattleAnimArgs[0];
    gTasks[taskId].data[13] = gBattleAnimArgs[6];
	
    if (gBattleAnimArgs[3])
        gTasks[taskId].data[6] = gTasks[taskId].data[6] | -0x8000;

    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
    {
        gTasks[taskId].data[2] = gBattleAnimArgs[1];
        gTasks[taskId].data[3] = gBattleAnimArgs[2];
    }
    else
    {
        if (gBattleAnimArgs[1] & 0x8000)
            gTasks[taskId].data[2] = gBattleAnimArgs[1] & 0x7FFF;
        else
            gTasks[taskId].data[2] = gBattleAnimArgs[1] | -0x8000;

        if (gBattleAnimArgs[2] & 0x8000)
            gTasks[taskId].data[3] = gBattleAnimArgs[2] & 0x7FFF;
        else
            gTasks[taskId].data[3] = gBattleAnimArgs[2] | -0x8000;
    }
    gTasks[taskId].data[8] = 0;
    gTasks[taskId].data[7] = 0;
    gTasks[taskId].data[4] = gBattleAnimArgs[4];
    gTasks[taskId].data[5] = gBattleAnimArgs[5];
    gTasks[taskId].func = AnimTask_SlideMonForFocusBand_Step;
}

// Squishes the mon vertically and emits sweat droplets a few times.
// arg 0: anim battler
// arg 1: num squishes
void AnimTask_SquishAndSweatDroplets(u8 taskId)
{
    u8 battler;
    struct Task *task = &gTasks[taskId];

    if (gBattleAnimArgs[1] == 0)
        DestroyAnimVisualTask(taskId);

    task->data[0] = 0;
    task->data[1] = 0;
    task->data[2] = 0;
    task->data[3] = gBattleAnimArgs[1];
	battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);
    
    task->data[4] = GetBattlerSpriteCoord(battler, BATTLER_COORD_X);
    task->data[5] = GetBattlerSpriteCoord(battler, BATTLER_COORD_Y);
    task->data[6] = GetBattlerSpriteSubpriority(battler);
    task->data[15] = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);
	
    PrepareAffineAnimInTaskData(task, task->data[15], sFacadeSquishAffineAnimCmds);
	
    task->func = AnimTask_SquishAndSweatDropletsStep;
}

static void AnimTask_SquishAndSweatDropletsStep(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        task->data[1]++;
		
        if (task->data[1] == 6)
            CreateSweatDroplets(taskId, TRUE);

        if (task->data[1] == 18)
            CreateSweatDroplets(taskId, FALSE);

        if (!RunAffineAnimFromTaskData(task))
        {
            if (--task->data[3] == 0)
                task->data[0]++;
            else
            {
                task->data[1] = 0;
                PrepareAffineAnimInTaskData(task, task->data[15], sFacadeSquishAffineAnimCmds);
            }
        }
        break;
    case 1:
        if (task->data[2] == 0)
            DestroyAnimVisualTask(taskId);
        break;
    }
}

static void CreateSweatDroplets(u8 taskId, bool8 arg1)
{
    u8 i;
    s8 xOffset, yOffset;
    struct Task *task = &gTasks[taskId];
    s16 xCoords[4];
    s16 yCoords[2];
	
    if (!arg1)
    {
        xOffset = 18;
        yOffset = -20;
    }
    else
    {
        xOffset = 30;
        yOffset = 20;
    }
    xCoords[0] = task->data[4] - xOffset;
    xCoords[1] = task->data[4] - xOffset - 4;
    xCoords[2] = task->data[4] + xOffset;
    xCoords[3] = task->data[4] + xOffset + 4;
    yCoords[0] = task->data[5] + yOffset;
    yCoords[1] = task->data[5] + yOffset + 6;

    for (i = 0; i < 4; i++)
    {
        u8 spriteId = CreateSprite(&gFacadeSweatDropSpriteTemplate, xCoords[i], yCoords[i & 1], task->data[6] - 5);
		
        if (spriteId != MAX_SPRITES)
        {
            gSprites[spriteId].data[0] = 0;
            gSprites[spriteId].data[1] = i < 2 ? -2 : 2;
            gSprites[spriteId].data[2] = -1;
            gSprites[spriteId].data[3] = taskId;
            gSprites[spriteId].data[4] = 2;
            task->data[2]++;
        }
    }
}

void AnimFacadeSweatDrop(struct Sprite *sprite)
{
    sprite->x += sprite->data[1];
    sprite->y += sprite->data[2];
	
    if (++sprite->data[0] > 6)
    {
        gTasks[sprite->data[3]].data[sprite->data[4]]--;
        DestroySprite(sprite);
    }
}

// Blends the mon sprite's color with a rotating set of colors.
// arg 0: anim battler
// arg 1: duration
void AnimTask_FacadeColorBlend(u8 taskId)
{
    gTasks[taskId].data[0] = 0;
    gTasks[taskId].data[1] = gBattleAnimArgs[1];
    gTasks[taskId].data[2] = 0x100 + gSprites[GetAnimBattlerSpriteId(gBattleAnimArgs[0])].oam.paletteNum * 16;
    gTasks[taskId].func = AnimTask_FacadeColorBlendStep;
}

static void AnimTask_FacadeColorBlendStep(u8 taskId)
{
    if (gTasks[taskId].data[1])
    {
		gTasks[taskId].data[1]--;
		
        BlendPalette(gTasks[taskId].data[2], 16, 8, sFacadeBlendColors[gTasks[taskId].data[0]]);
		
        if (++gTasks[taskId].data[0] > 23)
            gTasks[taskId].data[0] = 0;
    }
    else
    {
        BlendPalette(gTasks[taskId].data[2], 16, 0, RGB_BLACK);
        DestroyAnimVisualTask(taskId);
    }
}

// Animates Refresh and Aromatherapy status clear effect.
// arg 0: both mons (boolean)
void AnimTask_StatusClearedEffect(u8 taskId)
{
    StartMonScrollingBgMask(taskId, 0x1A0, gBattleAnimAttacker, gBattleAnimArgs[0], 10, 2, 30, gCureBubblesGfx, gCureBubblesTilemap, gCureBubblesPal);
}

// Moves a noise line from the mon.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: which direction (0 = upward, 1 = downward, 2 = horizontal)
void AnimRoarNoiseLine(struct Sprite *sprite)
{
    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_OPPONENT)
        gBattleAnimArgs[0] = -gBattleAnimArgs[0];

    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X) + gBattleAnimArgs[0];
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y) + gBattleAnimArgs[1];
	
	switch (gBattleAnimArgs[2])
	{
		case 0:
			sprite->data[1] = -0x280;
			break;
		case 1:
			sprite->vFlip = TRUE;
			sprite->data[1] = 0x280;
			break;
		default:
			StartSpriteAnim(sprite, 1);
			break;
	}
	sprite->data[0] = 0x280;
	sprite->data[2] = 14;

    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
    {
        sprite->data[0] = -sprite->data[0];
        sprite->hFlip = TRUE;
    }
	StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
    sprite->callback = AnimMoveSpriteOverDuration;
}

// Makes a series of dots in a trail from the attacker to the target.
void AnimTask_GlareEyeDots(u8 taskId)
{
	u8 x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X), height = GetBattlerSpriteCoordAttr(gBattleAnimAttacker, BATTLER_COORD_ATTR_HEIGHT) / 4;
	struct Task *task = &gTasks[taskId];

    task->data[5] = 12;
    task->data[6] = 3;
    task->data[7] = 0;

    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
        task->data[11] = x + height;
    else
        task->data[11] = x - height;

    task->data[12] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET) - height;
    task->data[13] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    task->data[14] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
    task->func = AnimTask_GlareEyeDotsStep;
}

static void AnimTask_GlareEyeDotsStep(u8 taskId)
{
    u8 i;
    s16 x, y;
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        if (++task->data[1] > 3)
        {
            task->data[1] = 0;
			
            GetGlareEyeDotCoords(task->data[11], task->data[12], task->data[13], task->data[14], task->data[5], task->data[2], &x, &y);

            for (i = 0; i < 2; i++)
            {
                u8 spriteId = CreateSprite(&gGlareEyeDotSpriteTemplate, x, y, 35);
				
                if (spriteId != MAX_SPRITES)
                {
                    if (task->data[7] == 0)
                    {
                        if (i == 0)
                            gSprites[spriteId].x2 = gSprites[spriteId].y2 = -task->data[6];
                        else
                            gSprites[spriteId].x2 = gSprites[spriteId].y2 = task->data[6];
                    }
                    else
                    {
                        if (i == 0)
                        {
                            gSprites[spriteId].x2 = -task->data[6];
                            gSprites[spriteId].y2 = task->data[6];
                        }
                        else
                        {
                            gSprites[spriteId].x2 = task->data[6];
                            gSprites[spriteId].y2 = -task->data[6];
                        }
                    }
                    gSprites[spriteId].data[0] = 0;
                    gSprites[spriteId].data[1] = taskId;
                    gSprites[spriteId].data[2] = 10;
					
                    task->data[10]++;
                }
            }

            if (task->data[2] == task->data[5])
                task->data[0]++;

            task->data[2]++;
        }
        break;
    case 1:
        if (task->data[10] == 0)
            DestroyAnimVisualTask(taskId);
        break;
    }
}

static void GetGlareEyeDotCoords(s16 arg0, s16 arg1, s16 arg2, s16 arg3, u8 arg4, u8 arg5, s16 *x, s16 *y)
{
    int x2, y2;

    if (arg5 == 0)
    {
        *x = arg0;
        *y = arg1;
        return;
    }

    if (arg5 >= arg4)
    {
        *x = arg2;
        *y = arg3;
        return;
    }
    arg4--;
	
    x2 = (arg0 << 8) + arg5 * (((arg2 - arg0) << 8) / arg4);
    y2 = (arg1 << 8) + arg5 * (((arg3 - arg1) << 8) / arg4);
	
    *x = x2 >> 8;
    *y = y2 >> 8;
}

// Animates the Glare eye dots moving.
void AnimGlareEyeDot(struct Sprite *sprite)
{
    if (++sprite->data[0] > 36)
    {
        gTasks[sprite->data[1]].data[sprite->data[2]]--;
        DestroySprite(sprite);
    }
}

// Moves a pawprint in a straight line.
// arg 0: initial x position
// arg 1: initial y position
// arg 2: destination x position
// arg 3: destination y position
// arg 4: duration
void AnimAssistPawprint(struct Sprite *sprite)
{
    sprite->x = gBattleAnimArgs[0];
    sprite->y = gBattleAnimArgs[1];
	
    sprite->data[2] = gBattleAnimArgs[2];
    sprite->data[4] = gBattleAnimArgs[3];
    sprite->data[0] = gBattleAnimArgs[4];
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
    sprite->callback = InitAndRunAnimFastLinearTranslation;
}

// Moves a ball in an arc twoards the target, and rotates the ball while arcing.
// No args.
void AnimTask_BarrageBall(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    task->data[11] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    task->data[12] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
	
    task->data[13] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    task->data[14] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + GetBattlerSpriteCoordAttr(gBattleAnimTarget, BATTLER_COORD_ATTR_HEIGHT) / 4;
	
    task->data[15] = CreateSprite(&gBarrageBallSpriteTemplate, task->data[11], task->data[12], GetBattlerSpriteSubpriority(gBattleAnimTarget) - 5);
	
    if (task->data[15] != MAX_SPRITES)
    {
        gSprites[task->data[15]].data[0] = 16;
        gSprites[task->data[15]].data[2] = task->data[13];
        gSprites[task->data[15]].data[4] = task->data[14];
        gSprites[task->data[15]].data[5] = -32;
        InitAnimArcTranslation(&gSprites[task->data[15]]);
		
        if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_OPPONENT)
            StartSpriteAffineAnim(&gSprites[task->data[15]], 1);

        task->func = AnimTask_BarrageBallStep;
    }
    else
        DestroyAnimVisualTask(taskId);
}

static void AnimTask_BarrageBallStep(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        if (++task->data[1] > 1)
        {
            task->data[1] = 0;
			
            TranslateAnimHorizontalArc(&gSprites[task->data[15]]);
			
            if (++task->data[2] > 7)
                task->data[0]++;
        }
        break;
    case 1:
        if (TranslateAnimHorizontalArc(&gSprites[task->data[15]]))
        {
            task->data[1] = 0;
            task->data[2] = 0;
            task->data[0]++;
        }
        break;
    case 2:
        if (++task->data[1] > 1)
        {
            task->data[1] = 0;
			
            task->data[2]++;
			
            gSprites[task->data[15]].invisible = task->data[2] & 1;
			
            if (task->data[2] == 16)
            {
                DestroySpriteAndFreeMatrix(&gSprites[task->data[15]]);
                task->data[0]++;
            }
        }
        break;
    case 3:
        DestroyAnimVisualTask(taskId);
        break;
    }
}

// Moves a hand back and forth in a squishing motion.
// arg 0: which battler
// arg 1: horizontal flip (boolean)
// arg 2: num squishes
void AnimSmellingSaltsHand(struct Sprite *sprite)
{
    u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);

    sprite->oam.tileNum += 16;
	
    sprite->data[6] = gBattleAnimArgs[2];
    sprite->data[7] = gBattleAnimArgs[1] ? 1 : -1;
	
    sprite->y = GetBattlerSpriteCoord(battler, BATTLER_COORD_Y_PIC_OFFSET);
	
    if (!gBattleAnimArgs[1])
    {
        sprite->oam.matrixNum |= ST_OAM_HFLIP;
        sprite->x = GetBattlerSpriteCoordAttr(battler, BATTLER_COORD_ATTR_LEFT) - 8;
    }
    else
        sprite->x = GetBattlerSpriteCoordAttr(battler, BATTLER_COORD_ATTR_RIGHT) + 8;

    sprite->callback = AnimSmellingSaltsHand_Step;
}

static void AnimSmellingSaltsHand_Step(struct Sprite *sprite)
{
    switch (sprite->data[0])
    {
    case 0:
        if (++sprite->data[1] > 1)
        {
            sprite->data[1] = 0;
			
            sprite->x2 += sprite->data[7];
			
            if (++sprite->data[2] == 12)
                sprite->data[0]++;
        }
        break;
    case 1:
        if (++sprite->data[1] == 8)
        {
            sprite->data[1] = 0;
            sprite->data[0]++;
        }
        break;
    case 2:
        sprite->x2 -= sprite->data[7] * 4;
		
        if (++sprite->data[1] == 6)
        {
            sprite->data[1] = 0;
            sprite->data[0]++;
        }
        break;
    case 3:
        sprite->x2 += sprite->data[7] * 3;
		
        if (++sprite->data[1] == 8)
        {
            if (--sprite->data[6])
            {
                sprite->data[1] = 0;
                sprite->data[0]--;
            }
            else
                DestroyAnimSprite(sprite);
        }
        break;
    }
}

// Squishes the target horizontally a few times.
// arg 0: number of squishes
void AnimTask_SmellingSaltsSquish(u8 taskId)
{
    gTasks[taskId].data[0] = gBattleAnimArgs[0];
    PrepareAffineAnimInTaskData(&gTasks[taskId], GetAnimBattlerSpriteId(ANIM_TARGET), sSmellingSaltsSquishAffineAnimCmds);
    gTasks[taskId].func = AnimTask_SmellingSaltsSquishStep;
}

static void AnimTask_SmellingSaltsSquishStep(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    if (++task->data[1] > 1)
    {
        task->data[1] = 0;
		
        if (!(task->data[2] & 1))
            gSprites[task->data[15]].x2 = 2;
        else
            gSprites[task->data[15]].x2 = -2;
    }

    if (!RunAffineAnimFromTaskData(task))
    {
        gSprites[task->data[15]].x2 = 0;
		
        if (--task->data[0])
        {
            PrepareAffineAnimInTaskData(&gTasks[taskId], gTasks[taskId].data[15], sSmellingSaltsSquishAffineAnimCmds);
			
            task->data[1] = 0;
            task->data[2] = 0;
        }
        else
            DestroyAnimVisualTask(taskId);
    }
}

// Blinks an exclamation image over the mon a few times.
// arg 0: which mon
// arg 1: blink delay
// arg 2: number of blinks
void AnimSmellingSaltExclamation(struct Sprite *sprite)
{
	u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);
	
    sprite->x = GetBattlerSpriteCoord(battler, BATTLER_COORD_X);
    sprite->y = GetBattlerSpriteCoordAttr(battler, BATTLER_COORD_ATTR_TOP);

    if (sprite->y < 8)
        sprite->y = 8;

    sprite->data[0] = 0;
    sprite->data[1] = gBattleAnimArgs[1];
    sprite->data[2] = 0;
    sprite->data[3] = gBattleAnimArgs[2];
    sprite->callback = AnimSmellingSaltExclamationStep;
}

static void AnimSmellingSaltExclamationStep(struct Sprite *sprite)
{
    if (++sprite->data[0] >= sprite->data[1])
    {
        sprite->data[0] = 0;
        sprite->data[2] = (sprite->data[2] + 1) & 1;
		
        sprite->invisible = sprite->data[2];
		
        if (sprite->data[2] && --sprite->data[3] == 0)
            DestroyAnimSprite(sprite);
    }
}

// Claps a hand several times. Used by MOVE_HELPING_HAND.
// arg 0: flip horizontally (boolean)
void AnimHelpingHandClap(struct Sprite *sprite)
{
    if (gBattleAnimArgs[0])
    {
        sprite->oam.matrixNum |= ST_OAM_HFLIP;
        sprite->x = 100;
        sprite->data[7] = 1;
    }
    else
    {
        sprite->x = 140;
        sprite->data[7] = -1;
    }
    sprite->y = 56;
    sprite->callback = AnimHelpingHandClapStep;
}

static void AnimHelpingHandClapStep(struct Sprite *sprite)
{
    switch (sprite->data[0])
    {
    case 0:
        sprite->y -= sprite->data[7] * 2;
		
        if (sprite->data[1] & 1)
            sprite->x -= sprite->data[7] * 2;

        if (++sprite->data[1] == 9)
        {
            sprite->data[1] = 0;
            sprite->data[0]++;
        }
        break;
    case 1:
        if (++sprite->data[1] == 4)
        {
            sprite->data[1] = 0;
            sprite->data[0]++;
        }
        break;
    case 2:
        sprite->data[1]++;
		
        sprite->y += sprite->data[7] * 3;
        sprite->x2 = sprite->data[7] * (gSineTable[sprite->data[1] * 10] >> 3);
		
        if (sprite->data[1] == 12)
        {
            sprite->data[1] = 0;
            sprite->data[0]++;
        }
        break;
    case 3:
        if (++sprite->data[1] == 2)
        {
            sprite->data[1] = 0;
            sprite->data[0]++;
        }
        break;
    case 4:
        sprite->data[1]++;
		
        sprite->y -= sprite->data[7] * 3;
        sprite->x2 = sprite->data[7] * (gSineTable[sprite->data[1] * 10] >> 3);
		
        if (sprite->data[1] == 12)
            sprite->data[0]++;
        break;
    case 5:
        sprite->data[1]++;
		
        sprite->y += sprite->data[7] * 3;
        sprite->x2 = sprite->data[7] * (gSineTable[sprite->data[1] * 10] >> 3);
		
        if (sprite->data[1] == 15)
            sprite->oam.tileNum += 16;

        if (sprite->data[1] == 18)
        {
            sprite->data[1] = 0;
            sprite->data[0]++;
        }
        break;
    case 6:
        sprite->x += sprite->data[7] * 6;
		
        if (++sprite->data[1] == 9)
        {
            sprite->data[1] = 0;
            sprite->data[0]++;
        }
        break;
    case 7:
        sprite->x += sprite->data[7] * 2;
		
        if (++sprite->data[1] == 1)
        {
            sprite->data[1] = 0;
            sprite->data[0]++;
        }
        break;
    case 8:
        sprite->x -= sprite->data[7] * 3;
		
        if (++sprite->data[1] == 5)
            DestroyAnimSprite(sprite);
        break;
    }
}

// Repeatedly moves the attacking mon in a horizontal lunging motion.
// No args.
void AnimTask_HelpingHandAttackerMovement(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    task->data[15] = GetAnimBattlerSpriteId(ANIM_ATTACKER);
	
    if (IsDoubleBattleForBattler(gBattleAnimAttacker))
    {
        int x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
        int y = GetBattlerSpriteCoord(BATTLE_PARTNER(gBattleAnimAttacker), BATTLER_COORD_X);
		
        if (x > y)
            task->data[14] = 1;
        else
            task->data[14] = -1;
    }
    else
    {
        if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
            task->data[14] = -1;
        else
            task->data[14] = 1;
    }
    task->func = AnimTask_HelpingHandAttackerMovementStep;
}

static void AnimTask_HelpingHandAttackerMovementStep(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        if (++task->data[1] == 13)
        {
            task->data[1] = 0;
            task->data[0]++;
        }
        break;
    case 1:
        gSprites[task->data[15]].x2 -= task->data[14] * 3;
		
        if (++task->data[1] == 6)
        {
            task->data[1] = 0;
            task->data[0]++;
        }
        break;
    case 2:
        gSprites[task->data[15]].x2 += task->data[14] * 3;
		
        if (++task->data[1] == 6)
        {
            task->data[1] = 0;
            task->data[0]++;
        }
        break;
    case 3:
        if (++task->data[1] == 2)
        {
            task->data[1] = 0;
			
            if (task->data[2] == 0)
            {
                task->data[2]++;
                task->data[0] = 1;
            }
            else
                task->data[0]++;
        }
        break;
    case 4:
        gSprites[task->data[15]].x2 += task->data[14];
		
        if (++task->data[1] == 3)
        {
            task->data[1] = 0;
            task->data[0]++;
        }
        break;
    case 5:
        if (++task->data[1] == 6)
        {
            task->data[1] = 0;
            task->data[0]++;
        }
        break;
    case 6:
        gSprites[task->data[15]].x2 -= task->data[14] * 4;
		
        if (++task->data[1] == 5)
        {
            task->data[1] = 0;
            task->data[0]++;
        }
        break;
    case 7:
        gSprites[task->data[15]].x2 += task->data[14] * 4;
		
        if (++task->data[1] == 5)
        {
            task->data[1] = 0;
            task->data[0]++;
        }
        break;
    case 8:
        gSprites[task->data[15]].x2 = 0;
        DestroyAnimVisualTask(taskId);
        break;
    }
}

// Moves a magnifying glass around the target in straight lines.
// No args.
void AnimForesightMagnifyingGlass(struct Sprite *sprite)
{
	sprite->data[7] = gBattleAnimTarget;

    if (GetBattlerSide(sprite->data[7]) == B_SIDE_OPPONENT)
        sprite->oam.matrixNum = ST_OAM_HFLIP;

    sprite->oam.priority = GetBattlerSpriteBGPriority(sprite->data[7]);
    sprite->callback = AnimForesightMagnifyingGlassStep;
}

static void AnimForesightMagnifyingGlassStep(struct Sprite *sprite)
{
    u16 x, y;

    switch (sprite->data[5])
    {
    case 0:
        switch (sprite->data[6])
        {
        default:
            sprite->data[6] = 0;
        case 0:
        case 4:
            x = GetBattlerSpriteCoordAttr(sprite->data[7], BATTLER_COORD_ATTR_RIGHT) - 4;
            y = GetBattlerSpriteCoordAttr(sprite->data[7], BATTLER_COORD_ATTR_BOTTOM) - 4;
            break;
        case 1:
            x = GetBattlerSpriteCoordAttr(sprite->data[7], BATTLER_COORD_ATTR_RIGHT) - 4;
            y = GetBattlerSpriteCoordAttr(sprite->data[7], BATTLER_COORD_ATTR_TOP) + 4;
            break;
        case 2:
            x = GetBattlerSpriteCoordAttr(sprite->data[7], BATTLER_COORD_ATTR_LEFT) + 4;
            y = GetBattlerSpriteCoordAttr(sprite->data[7], BATTLER_COORD_ATTR_BOTTOM) - 4;
            break;
        case 3:
            x = GetBattlerSpriteCoordAttr(sprite->data[7], BATTLER_COORD_ATTR_LEFT) + 4;
            y = GetBattlerSpriteCoordAttr(sprite->data[7], BATTLER_COORD_ATTR_TOP) - 4;
            break;
        case 5:
            x = GetBattlerSpriteCoord(sprite->data[7], BATTLER_COORD_X);
            y = GetBattlerSpriteCoord(sprite->data[7], BATTLER_COORD_Y_PIC_OFFSET);
            break;
        }

        if (sprite->data[6] == 4)
            sprite->data[0] = 24;
        else if (sprite->data[6] == 5)
            sprite->data[0] = 6;
        else
            sprite->data[0] = 12;

        sprite->data[1] = sprite->x;
        sprite->data[2] = x;
        sprite->data[3] = sprite->y;
        sprite->data[4] = y;
        InitAnimLinearTranslation(sprite);
        sprite->data[5]++;
        break;
    case 1:
        if (AnimTranslateLinear(sprite))
        {
            switch (sprite->data[6])
            {
            default:
                SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
                sprite->data[0] = 0;
                sprite->data[5]++;
                sprite->data[6]++;
                break;
            case 4:
                SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
                sprite->data[5] = 0;
                sprite->data[6]++;
                break;
            case 5:
                sprite->data[0] = 0;
                sprite->data[1] = 16;
                sprite->data[2] = 0;
                sprite->data[5] = 3;
                break;
            }
        }
        break;
    case 2:
        if (++sprite->data[0] == 4)
            sprite->data[5] = 0;
        break;
    case 3:
        if (!(sprite->data[0] & 1))
            sprite->data[1]--;
        else
            sprite->data[2]++;

        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sprite->data[1], sprite->data[2]));
		
        if (++sprite->data[0] == 32)
        {
            sprite->invisible = TRUE;
            sprite->data[5]++;
        }
        break;
    case 4:
        DestroyAnimSprite(sprite);
        break;
    }
}

static void AnimMeteorMashStarStep(struct Sprite *sprite)
{
    sprite->x2 = ((sprite->data[2] - sprite->data[0]) * sprite->data[5]) / sprite->data[4];
    sprite->y2 = ((sprite->data[3] - sprite->data[1]) * sprite->data[5]) / sprite->data[4];
	
    if (!(sprite->data[5] & 1))
        CreateSprite(&gMiniTwinklingStarSpriteTemplate, sprite->x + sprite->x2, sprite->y + sprite->y2, 5);

    if (sprite->data[5] == sprite->data[4])
        DestroyAnimSprite(sprite);

    sprite->data[5]++;
}

// Moves a shooting star across the screen that leaves little twinkling stars behind its path.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: destination x pixel offset
// arg 3: destination y pixel offset
// arg 4: duration
void AnimMeteorMashStar(struct Sprite *sprite)
{
    s16 y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    s16 x = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);

    if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_PLAYER)
    {
        sprite->data[0] = sprite->x - gBattleAnimArgs[0];
        sprite->data[2] = sprite->x - gBattleAnimArgs[2];
    }
    else
    {
        sprite->data[0] = sprite->x + gBattleAnimArgs[0];
        sprite->data[2] = sprite->x + gBattleAnimArgs[2];
    }
    sprite->data[1] = sprite->y + gBattleAnimArgs[1];
    sprite->data[3] = sprite->y + gBattleAnimArgs[3];
    sprite->data[4] = gBattleAnimArgs[4];
	
    sprite->x = sprite->data[0];
    sprite->y = sprite->data[1];
	
    sprite->callback = AnimMeteorMashStarStep;
}

void AnimTask_MonToSubstitute(u8 taskId)
{
    int i;
    u8 spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);
	
	switch (gTasks[taskId].data[0])
	{
		case 0:
			PrepareBattlerSpriteForRotScale(spriteId, FALSE);
			gTasks[taskId].data[1] = 0x100;
			gTasks[taskId].data[2] = 0x100;
			gTasks[taskId].data[0]++;
			break;
		case 1:
			gTasks[taskId].data[1] += 0x60;
			gTasks[taskId].data[2] -= 0xD;
			SetSpriteRotScale(spriteId, gTasks[taskId].data[1], gTasks[taskId].data[2], 0);
			
			if (++gTasks[taskId].data[3] == 9)
			{
				gTasks[taskId].data[3] = 0;
				ResetSpriteRotScale(spriteId);
				gSprites[spriteId].invisible = TRUE;
				gTasks[taskId].data[0]++;
			}
			break;
		case 2:
			LoadBattleMonGfxAndAnimate(gBattleAnimAttacker, FALSE, spriteId);
			for (i = 0; i < 16; i++)
				gTasks[taskId].data[i] = 0;
	
			gTasks[taskId].func = AnimTask_MonToSubstituteDoll;
			break;
	}
}

static void AnimTask_MonToSubstituteDoll(u8 taskId)
{
    u8 spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);

    switch (gTasks[taskId].data[0])
    {
    case 0:
        gSprites[spriteId].y2 = -200;
        gSprites[spriteId].x2 = 200;
        gSprites[spriteId].invisible = FALSE;
        gTasks[taskId].data[10] = 0;
        gTasks[taskId].data[0]++;
        break;
    case 1:
        gTasks[taskId].data[10] += 112;
        gSprites[spriteId].y2 += gTasks[taskId].data[10] >> 8;
        if (gSprites[spriteId].y + gSprites[spriteId].y2 >= -32)
            gSprites[spriteId].x2 = 0;

        if (gSprites[spriteId].y2 > 0)
            gSprites[spriteId].y2 = 0;

        if (gSprites[spriteId].y2 == 0)
        {
            PlaySE12WithPanning(SE_M_BUBBLE2, BattleAnimAdjustPanning(SOUND_PAN_ATTACKER));
            gTasks[taskId].data[10] -= 0x800;
            gTasks[taskId].data[0]++;
        }
        break;
    case 2:
        gTasks[taskId].data[10] -= 112;
        if (gTasks[taskId].data[10] < 0)
            gTasks[taskId].data[10] = 0;

        gSprites[spriteId].y2 -= gTasks[taskId].data[10] >> 8;
        if (gTasks[taskId].data[10] == 0)
            gTasks[taskId].data[0]++;
        break;
    case 3:
        gTasks[taskId].data[10] += 112;
		
        gSprites[spriteId].y2 += gTasks[taskId].data[10] >> 8;
        if (gSprites[spriteId].y2 > 0)
            gSprites[spriteId].y2 = 0;

        if (gSprites[spriteId].y2 == 0)
        {
            PlaySE12WithPanning(SE_M_BUBBLE2, BattleAnimAdjustPanning(SOUND_PAN_ATTACKER));
            DestroyAnimVisualTask(taskId);
        }
        break;
    }
}

// Moves down an X on the target that flickers and disappears. Used by MOVE_BLOCK.
// No args.
void AnimBlockX(struct Sprite *sprite)
{
    s16 y;

    if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_PLAYER)
    {
        sprite->subpriority = GetBattlerSpriteSubpriority(gBattleAnimTarget) - 2;
        y = -144;
    }
    else
    {
        sprite->subpriority = GetBattlerSpriteSubpriority(gBattleAnimTarget) + 2;
        y = -96;
    }
    sprite->y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
    sprite->y2 = y;
    sprite->callback = AnimBlockXStep;
}

static void AnimBlockXStep(struct Sprite *sprite)
{
    switch (sprite->data[0])
    {
    case 0:
        sprite->y2 += 10;
		
        if (sprite->y2 >= 0)
        {
            PlaySE12WithPanning(SE_M_SKETCH, BattleAnimAdjustPanning(SOUND_PAN_TARGET));
			
            sprite->y2 = 0;
            sprite->data[0]++;
        }
        break;
    case 1:
        sprite->data[1] += 4;
        sprite->y2 = -(gSineTable[sprite->data[1]] >> 3);
		
        if (sprite->data[1] > 0x7F)
        {
            PlaySE12WithPanning(SE_M_SKETCH, BattleAnimAdjustPanning(SOUND_PAN_TARGET));
			
			sprite->data[1] = 0;
            sprite->y2 = 0;
            sprite->data[0]++;
        }
        break;
    case 2:
        sprite->data[1] += 6;
        sprite->y2 = -(gSineTable[sprite->data[1]] >> 4);
		
        if (sprite->data[1] > 0x7F)
        {
            sprite->data[1] = 0;
            sprite->y2 = 0;
            sprite->data[0]++;
        }
        break;
    case 3:
        if (++sprite->data[1] > 8)
        {
            PlaySE12WithPanning(SE_M_LEER, BattleAnimAdjustPanning(SOUND_PAN_TARGET));
			
            sprite->data[1] = 0;
            sprite->data[0]++;
        }
        break;
    case 4:
        if (++sprite->data[1] > 8)
        {
            sprite->data[1] = 0;
			
            sprite->data[2]++;
			
            sprite->invisible = sprite->data[2] & 1;
			
            if (sprite->data[2] == 7)
                DestroyAnimSprite(sprite);
        }
        break;
    }
}

// Quickly moves two clones of the target mon back and forth.
// No args.
void AnimTask_OdorSleuthMovement(u8 taskId)
{
    s16 spriteId1, spriteId2;

    spriteId1 = CloneBattlerSpriteWithBlend(ANIM_TARGET);
    if (spriteId1 < 0)
    {
        DestroyAnimVisualTask(taskId);
        return;
    }

    spriteId2 = CloneBattlerSpriteWithBlend(ANIM_TARGET);
    if (spriteId2 < 0)
    {
        DestroySpriteWithActiveSheet(&gSprites[spriteId1]);
        DestroyAnimVisualTask(taskId);
        return;
    }

    gSprites[spriteId2].x2 += 24;
    gSprites[spriteId1].x2 -= 24;
    gSprites[spriteId2].data[0] = 0;
    gSprites[spriteId1].data[0] = 0;
    gSprites[spriteId2].data[1] = 0;
    gSprites[spriteId1].data[1] = 0;
    gSprites[spriteId2].data[2] = 0;
    gSprites[spriteId1].data[2] = 0;
    gSprites[spriteId2].data[3] = 16;
    gSprites[spriteId1].data[3] = -16;
    gSprites[spriteId2].data[4] = 0;
    gSprites[spriteId1].data[4] = 128;
    gSprites[spriteId2].data[5] = 24;
    gSprites[spriteId1].data[5] = 24;
    gSprites[spriteId2].data[6] = taskId;
    gSprites[spriteId1].data[6] = taskId;
    gSprites[spriteId2].data[7] = 0;
    gSprites[spriteId1].data[7] = 0;
    gTasks[taskId].data[0] = 2;

    if (!gBattleSpritesDataPtr->battlerData[gBattleAnimTarget].invisible)
    {
        gSprites[spriteId2].invisible = FALSE;
        gSprites[spriteId1].invisible = TRUE;
    }
    else
    {
        gSprites[spriteId2].invisible = TRUE;
        gSprites[spriteId1].invisible = TRUE;
    }
    gSprites[spriteId2].oam.objMode = ST_OAM_OBJ_NORMAL;
    gSprites[spriteId1].oam.objMode = ST_OAM_OBJ_NORMAL;
    gSprites[spriteId2].callback = MoveOdorSleuthClone;
    gSprites[spriteId1].callback = MoveOdorSleuthClone;
    gTasks[taskId].func = AnimTask_OdorSleuthMovementWaitFinish;
}

static void AnimTask_OdorSleuthMovementWaitFinish(u8 taskId)
{
    if (gTasks[taskId].data[0] == 0)
        DestroyAnimVisualTask(taskId);
}

static void MoveOdorSleuthClone(struct Sprite *sprite)
{
    if (++sprite->data[1] > 1)
    {
        sprite->data[1] = 0;
		
        if (!gBattleSpritesDataPtr->battlerData[gBattleAnimTarget].invisible)
            sprite->invisible ^= TRUE;
    }
    sprite->data[4] = sprite->data[4] + sprite->data[3];
    sprite->data[4] &= 0xFF;
	
    sprite->x2 = Cos(sprite->data[4], sprite->data[5]);
	
    switch (sprite->data[0])
    {
    case 0:
        if (++sprite->data[2] == 60)
        {
            sprite->data[2] = 0;
            sprite->data[0]++;
        }
        break;
    case 1:
        if (++sprite->data[2] > 0)
        {
            sprite->data[2] = 0;
            sprite->data[5] -= 2;
			
            if (sprite->data[5] < 0)
            {
                gTasks[sprite->data[6]].data[sprite->data[7]]--;
                DestroySpriteWithActiveSheet(sprite);
            }
        }
        break;
    }
}

// Gets the MOVE_RETURN's power level.
// No args.
void AnimTask_GetReturnPowerLevel(u8 taskId)
{
    if (gAnimFriendship <= 60)
        gBattleAnimArgs[ARG_RET_ID] = 0;
    else if (gAnimFriendship > 60 && gAnimFriendship < 92)
        gBattleAnimArgs[ARG_RET_ID] = 1;
    else if (gAnimFriendship > 91 && gAnimFriendship < 201)
        gBattleAnimArgs[ARG_RET_ID] = 2;
    else if (gAnimFriendship > 200)
        gBattleAnimArgs[ARG_RET_ID] = 3;

    DestroyAnimVisualTask(taskId);
}

// Gets the MOVE_FRUSTRATION's power level.
// No args.
void AnimTask_GetFrustrationPowerLevel(u8 taskId)
{
    if (gAnimFriendship <= 30)
        gBattleAnimArgs[ARG_RET_ID] = 0;
    else if (gAnimFriendship <= 100)
        gBattleAnimArgs[ARG_RET_ID] = 1;
    else if (gAnimFriendship <= 200)
        gBattleAnimArgs[ARG_RET_ID] = 2;
    else
        gBattleAnimArgs[ARG_RET_ID] = 3;

    DestroyAnimVisualTask(taskId);
}

// Animates Snatch move steal animation.
// No args.
void AnimTask_SnatchMonMove(u8 taskId)
{
	gTasks[taskId].func = IsBattlerAlly(gBattleAnimAttacker, gBattleAnimTarget) ? AnimTask_SnatchPartnerMove : AnimTask_SnatchOpposingMonMove;
	gTasks[taskId].func(taskId);
}

static void AnimTask_SnatchOpposingMonMove(u8 taskId)
{
	struct Pokemon * mon;
    u8 spriteId, spriteId2;
    u32 personality;
    bool8 isShiny;
    u16 species;
    u8 subpriority;
    bool8 isBackPic;
    s16 x;

    switch (gTasks[taskId].data[0])
    {
    case 0:
        spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);
		
        gTasks[taskId].data[1] += 0x800;
		
        if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
            gSprites[spriteId].x2 += (gTasks[taskId].data[1] >> 8);
        else
            gSprites[spriteId].x2 -= (gTasks[taskId].data[1] >> 8);

        gTasks[taskId].data[1] &= 0xFF;
		
        x = gSprites[spriteId].x + gSprites[spriteId].x2;
		
        if ((u16)(x + 32) > 304)
        {
            gTasks[taskId].data[1] = 0;
            gTasks[taskId].data[0]++;
        }
        break;
    case 1:
        {
            s16 x;
			bool8 trasformed;

            mon = GetBattlerPartyIndexPtr(gBattleAnimAttacker);
			personality = GetMonData(mon, MON_DATA_PERSONALITY);
			isShiny = GetMonData(mon, MON_DATA_IS_SHINY);
			
			if (!gBattleSpritesDataPtr->battlerData[gBattleAnimAttacker].transformSpecies)
			{
				trasformed = FALSE;
				species = GetMonData(mon, MON_DATA_SPECIES);
			}
			else
			{
				trasformed = TRUE;
				species = gBattleSpritesDataPtr->battlerData[gBattleAnimAttacker].transformSpecies;
			}
			
            if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
            {
                subpriority = gSprites[GetAnimBattlerSpriteId(ANIM_TARGET)].subpriority + 1;
                isBackPic = FALSE;
                x = 272;
            }
            else
            {
                subpriority = gSprites[GetAnimBattlerSpriteId(ANIM_TARGET)].subpriority - 1;
                isBackPic = TRUE;
                x = -32;
            }
            spriteId2 = CreateAdditionalMonSpriteForMoveAnim(species, isBackPic, x, GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y), subpriority, personality, isShiny, trasformed);
			
            gTasks[taskId].data[15] = spriteId2;
            gTasks[taskId].data[0]++;
            break;
        }
    case 2:
        spriteId2 = gTasks[taskId].data[15];
		
        gTasks[taskId].data[1] += 0x800;
		
        if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
            gSprites[spriteId2].x2 -= (gTasks[taskId].data[1] >> 8);
        else
            gSprites[spriteId2].x2 += (gTasks[taskId].data[1] >> 8);

        gTasks[taskId].data[1] &= 0xFF;
		
        x = gSprites[spriteId2].x + gSprites[spriteId2].x2;
		
        if (gTasks[taskId].data[14] == 0)
        {
            if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
            {
                if (x < GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X))
                {
                    gTasks[taskId].data[14]++;
                    gBattleAnimArgs[7] = 0xFFFF;
                }
            }
            else
            {
                if (x > GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X))
                {
                    gTasks[taskId].data[14]++;
                    gBattleAnimArgs[7] = 0xFFFF;
                }
            }
        }
		
        if ((u16)(x + 32) > 304)
        {
            gTasks[taskId].data[1] = 0;
            gTasks[taskId].data[0]++;
        }
        break;
    case 3:
        spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);
        spriteId2 = gTasks[taskId].data[15];
		
        DestroySpriteAndFreeResources(&gSprites[spriteId2]);
		
        if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
            gSprites[spriteId].x2 = -gSprites[spriteId].x - 32;
        else
            gSprites[spriteId].x2 = 272 - gSprites[spriteId].x;

        gTasks[taskId].data[0]++;
        break;
    case 4:
        spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);
		
        gTasks[taskId].data[1] += 0x800;
		
        if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
        {
            gSprites[spriteId].x2 += (gTasks[taskId].data[1] >> 8);
			
            if (gSprites[spriteId].x2 + gSprites[spriteId].x >= GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X))
                gSprites[spriteId].x2 = 0;
        }
        else
        {
            gSprites[spriteId].x2 -= (gTasks[taskId].data[1] >> 8);
			
            if (gSprites[spriteId].x2 + gSprites[spriteId].x <= GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X))
                gSprites[spriteId].x2 = 0;
        }
        gTasks[taskId].data[1] = (u8)gTasks[taskId].data[1];
		
        if (gSprites[spriteId].x2 == 0)
            DestroyAnimVisualTask(taskId);
        break;
    }
}

static void AnimTask_SnatchPartnerMove(u8 taskId)
{
    s16 attackerX, targetX;
    u8 spriteId;

    switch (gTasks[taskId].data[15])
    {
    case 0:
        attackerX = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
        targetX = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
		
        gTasks[taskId].data[0] = 6;
		
        if (attackerX > targetX)
            gTasks[taskId].data[0] *= -1;

        gTasks[taskId].data[1] = attackerX;
        gTasks[taskId].data[2] = targetX;
        gTasks[taskId].data[15]++;
        break;
    case 1:
        spriteId = gBattlerSpriteIds[gBattleAnimAttacker];
		
        gSprites[spriteId].x2 += gTasks[taskId].data[0];
		
        if (gTasks[taskId].data[0] > 0)
        {
            if (gSprites[spriteId].x + gSprites[spriteId].x2 >= gTasks[taskId].data[2])
                gTasks[taskId].data[15]++;
        }
        else
        {
            if (gSprites[spriteId].x + gSprites[spriteId].x2 <= gTasks[taskId].data[2])
                gTasks[taskId].data[15]++;
        }
        break;
    case 2:
        gTasks[taskId].data[0] *= -1;
        gTasks[taskId].data[15]++;
        break;
    case 3:
        spriteId = gBattlerSpriteIds[gBattleAnimAttacker];
		
        gSprites[spriteId].x2 += gTasks[taskId].data[0];
		
        if (gTasks[taskId].data[0] < 0)
        {
            if (gSprites[spriteId].x + gSprites[spriteId].x2 <= gTasks[taskId].data[1])
                gTasks[taskId].data[15]++;
        }
        else
        {
            if (gSprites[spriteId].x + gSprites[spriteId].x2 >= gTasks[taskId].data[1])
                gTasks[taskId].data[15]++;
        }
        break;
    case 4:
    default:
        gSprites[gBattlerSpriteIds[gBattleAnimAttacker]].x2 = 0;
        DestroyAnimVisualTask(taskId);
        break;
    }
}

// Moves the mon's sprite back and forth in an unpredictable swaying motion.
// No args.
void AnimTask_TeeterDanceMovement(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    
    task->data[3] = GetAnimBattlerSpriteId(ANIM_ATTACKER);
    task->data[4] = GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER ? 1 : -1;
    task->data[6] = gSprites[task->data[3]].y;
    task->data[5] = gSprites[task->data[3]].x;
    task->data[9] = 0;
    task->data[11] = 0;
    task->data[10] = 1;
    task->data[12] = 0;
    task->func = AnimTask_TeeterDanceMovementStep;
}

static void AnimTask_TeeterDanceMovementStep(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    
    switch (task->data[0])
    {
    case 0:
        task->data[11] += 8;
        task->data[11] &= 0xFF;
        gSprites[task->data[3]].x2 = gSineTable[task->data[11]] >> 5;
		
        task->data[9] += 2;
        task->data[9] &= 0xFF;
        gSprites[task->data[3]].x = (gSineTable[task->data[9]] >> 3) * task->data[4] + task->data[5];
		
        if (task->data[9] == 0)
        {
            gSprites[task->data[3]].x = task->data[5];
            task->data[0]++;
        }
        break;
    case 1:
        task->data[11] += 8;
        task->data[11] &= 0xFF;
        gSprites[task->data[3]].x2 = gSineTable[task->data[11]] >> 5;
		
        if (task->data[11] == 0)
        {
            gSprites[task->data[3]].x2 = 0;
            task->data[0]++;
        }
        break;
    case 2:
        DestroyAnimVisualTask(taskId);
        break;
    }
}

static void AnimKnockOffStrikeStep(struct Sprite *sprite)
{
    if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_PLAYER)
    {
        sprite->data[1] += sprite->data[0];
        sprite->data[1] &= 0xFF;
    }
    else
    {
        sprite->data[1] += sprite->data[0];
        sprite->data[1] &= 0xFF;
    }

    sprite->x2 = Cos(sprite->data[1], 20);
    sprite->y2 = Sin(sprite->data[1], 20);
    if (sprite->animEnded)
        DestroyAnimSprite(sprite);

    sprite->data[2]++;
}

// Animates a strike that swipes downard at the target mon.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
void AnimKnockOffStrike(struct Sprite *sprite)
{
    if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_PLAYER)
    {
        sprite->x -= gBattleAnimArgs[0];
        sprite->y += gBattleAnimArgs[1];
        sprite->data[0] = -11;
        sprite->data[1] = 192;
        StartSpriteAffineAnim(sprite, 1);
    }
    else
    {
        sprite->data[0] = 11;
        sprite->data[1] = 192;
        sprite->x += gBattleAnimArgs[0];
        sprite->y += gBattleAnimArgs[1];
    }
    sprite->callback = AnimKnockOffStrikeStep;
}

// Gradually fades a rotating recyle arrow sprite in and back out.
// No args.
void AnimRecycle(struct Sprite *sprite)
{
    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    sprite->y = GetBattlerSpriteCoordAttr(gBattleAnimAttacker, BATTLER_COORD_ATTR_TOP);
	
    if (sprite->y < 16)
        sprite->y = 16;

    sprite->data[6] = 0;
    sprite->data[7] = 16;
    sprite->callback = AnimRecycleStep;
	
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sprite->data[6], sprite->data[7]));
}

static void AnimRecycleStep(struct Sprite *sprite)
{
    switch (sprite->data[2])
    {
    case 0:
        if (++sprite->data[0] > 1)
        {
            sprite->data[0] = 0;
			
            if (!(sprite->data[1] & 1))
            {
                if (sprite->data[6] < 16)
                    sprite->data[6]++;
            }
            else
            {
                if (sprite->data[7] != 0)
                    sprite->data[7]--;
            }
            sprite->data[1]++;
			
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sprite->data[6], sprite->data[7]));
			
            if (sprite->data[7] == 0)
                sprite->data[2]++;
        }
        break;
    case 1:
        if (++sprite->data[0] == 10)
        {
            sprite->data[0] = 0;
            sprite->data[1] = 0;
            sprite->data[2]++;
        }
        break;
    case 2:
        if (++sprite->data[0] > 1)
        {
            sprite->data[0] = 0;
			
            if (!(sprite->data[1] & 1))
            {
                if (sprite->data[6] != 0)
                    sprite->data[6]--;
            }
            else
            {
                if (sprite->data[7] < 16)
                    sprite->data[7]++;
            }
            sprite->data[1]++;
			
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sprite->data[6], sprite->data[7]));
			
            if (sprite->data[7] == 16)
                sprite->data[2]++;
        }
        break;
    case 3:
        DestroyAnimSprite(sprite);
        break;
    }
}

// Gets the current battle weather.
// No args.
void AnimTask_GetWeather(u8 taskId)
{
	gBattleAnimArgs[ARG_RET_ID] = GetCurrentWeatherEnumId(gBattleWeather);
    DestroyAnimVisualTask(taskId);
}

// Squishes the mon sprite vertically, and shakes it back and forth.
// arg 0: which battler
void AnimTask_SlackOffSquish(u8 taskId)
{
    PrepareAffineAnimInTaskData(&gTasks[taskId], GetAnimBattlerSpriteId(gBattleAnimArgs[0]), sSlackOffSquishAffineAnimCmds);
    gTasks[taskId].func = AnimTask_SlackOffSquishStep;
}

static void AnimTask_SlackOffSquishStep(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    gTasks[taskId].data[0]++;
	
    if (gTasks[taskId].data[0] > 16 && gTasks[taskId].data[0] < 40)
    {
        if (++task->data[1] > 2)
        {
            task->data[1] = 0;

            if (!(++task->data[2] & 1))
                gSprites[task->data[15]].x2 = -1;
            else
                gSprites[task->data[15]].x2 = 1;
        }
    }
    else
        gSprites[task->data[15]].x2 = 0;

    if (!RunAffineAnimFromTaskData(&gTasks[taskId]))
        DestroyAnimVisualTask(taskId);
}

// Squishes the target sprite vertically, and then back.
// No args.
void AnimTask_SquishTarget(u8 taskId)
{
    PrepareAffineAnimInTaskData(&gTasks[taskId], GetAnimBattlerSpriteId(ANIM_TARGET), sSquishTargetAffineAnimCmds);
    gTasks[taskId].func = AnimTask_DestroyTaskAfterAffineAnimFromTaskDataEnds;
}

// Animates MOVE_HEALING_WISH's blue star sprite.
// arg 0: move up (boolean)
static void AnimHealingWishStar(struct Sprite *sprite)
{
	u8 y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
	
	sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X) + 8;

	sprite->data[0] = gBattleAnimArgs[0];
	
	if (sprite->data[0])
	{
		sprite->y = y - 8;
		sprite->data[1] = -8;
	}
	else
	{
		sprite->y = -8;
		sprite->data[1] = y + 8;
	}
	
	if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_OPPONENT)
	{
		// Moves more slowly on opponent's side
		sprite->data[5] = 5;
		sprite->data[6] = 6;
	}
	else
	{
		sprite->data[5] = 10;
		sprite->data[6] = 3;
	}
	sprite->callback = AnimHealingWishStar_Step;
}

static void AnimHealingWishStar_Step(struct Sprite *sprite)
{
	u32 newY;
	
	sprite->data[3] += sprite->data[5];
	
	if (sprite->data[0])
		sprite->y2 = -(sprite->data[3] >> 4);
	else
		sprite->y2 = sprite->data[3] >> 4;
	
	sprite->data[4] += 5;
	sprite->data[4] &= 0xFF;
	
	sprite->x2 = Sin(sprite->data[4], 3);
	
	if (++sprite->data[2] % sprite->data[6] == 0)
        CreateSpriteAndAnimate(&gMiniTwinklingGreenStarSpriteTemplate, sprite->x + sprite->x2, sprite->y + sprite->y2, sprite->subpriority + 1);
	
	newY = sprite->y + sprite->y2 + 8;
    if (newY > sprite->data[1])
        DestroyAnimSprite(sprite);
}

// Animates the pokeball sprite on MOVE_BATON_PASS's anim.
// No args.
static void AnimBatonPassPokeball(struct Sprite *sprite)
{
	u8 spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);

    switch (sprite->data[0])
    {
    case 0:
        sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
        sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
		
        PrepareBattlerSpriteForRotScale(spriteId, ST_OAM_OBJ_NORMAL);
		
        sprite->data[1] = 256;
        sprite->data[2] = 256;
        sprite->data[0]++;
        break;
    case 1:
        sprite->data[1] += 96;
        sprite->data[2] -= 26;
        SetSpriteRotScale(spriteId, sprite->data[1], sprite->data[2], 0);

        if (++sprite->data[3] == 5)
            sprite->data[0]++;
        // fall through
    case 2:
        sprite->data[1] += 96;
        sprite->data[2] += 48;
        SetSpriteRotScale(spriteId, sprite->data[1], sprite->data[2], 0);

        if (++sprite->data[3] == 9)
        {
            sprite->data[3] = 0;
            gSprites[spriteId].invisible = TRUE;
            ResetSpriteRotScale(spriteId);
            sprite->data[0]++;
        }
        break;
    case 3:
        sprite->y2 -= 6;
        if (sprite->y + sprite->y2 < -32)
            DestroyAnimSprite(sprite);
        break;
    }
}

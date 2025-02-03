#include "global.h"
#include "gflib.h"
#include "battle_anim.h"
#include "graphics.h"
#include "scanline_effect.h"
#include "trig.h"
#include "constants/songs.h"

static void AnimDefensiveWall(struct Sprite *sprite);
static void AnimDefensiveWall_Step(struct Sprite *sprite);
static void AnimDefensiveWall_Step2(struct Sprite *sprite);
static void AnimDefensiveWall_Step3(struct Sprite *sprite);
static void AnimDefensiveWall_Step4(struct Sprite *sprite);
static void AnimWallSparkle(struct Sprite *sprite);
static void AnimBentSpoon(struct Sprite *sprite);
static void AnimQuestionMark(struct Sprite *sprite);
static void AnimQuestionMarkStep(struct Sprite *sprite);
static void AnimQuestionMarkStep2(struct Sprite *sprite);
static void AnimRedX(struct Sprite *sprite);
static void AnimSkillSwapOrb(struct Sprite *sprite);
static void AnimPsychoBoost(struct Sprite *sprite);
static void AnimTask_Teleport_Step(u8 taskId);
static void AnimTask_ImprisonOrbs_Step(u8 taskId);
static void AnimTask_SkillSwapOrbs(u8 taskId);
static void AnimTask_ExtrasensoryDistortion_Step(u8 taskId);
static void AnimTask_TransparentCloneGrowAndShrink_Step(u8 taskId);
static void SetPsychicBackground_Step(u8 taskId);
static void AnimTask_RolePlaySilhouetteStep1(u8);
static void AnimTask_RolePlaySilhouetteStep2(u8);
static void AnimThrowMistBall(struct Sprite *sprite);
static void AnimTask_OverlayFogTiles(u8 taskId);

static const union AffineAnimCmd sAffineAnim_PsychUpSpiral[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME(-0x2, -0x2, -10, 120),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims_PsychUpSpiral[] =
{
    sAffineAnim_PsychUpSpiral,
};

const struct SpriteTemplate gPsychUpSpiralSpriteTemplate =
{
    .tileTag = ANIM_TAG_SPIRAL,
    .paletteTag = ANIM_TAG_SPIRAL,
    .oam = &gOamData_AffineNormal_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_PsychUpSpiral,
    .callback = AnimSpriteOnMonPos,
};

const struct SpriteTemplate gLightScreenWallSpriteTemplate =
{
    .tileTag = ANIM_TAG_GREEN_LIGHT_WALL,
    .paletteTag = ANIM_TAG_GREEN_LIGHT_WALL,
    .oam = &gOamData_AffineOff_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimDefensiveWall,
};

const struct SpriteTemplate gReflectWallSpriteTemplate =
{
    .tileTag = ANIM_TAG_BLUE_LIGHT_WALL,
    .paletteTag = ANIM_TAG_BLUE_LIGHT_WALL,
    .oam = &gOamData_AffineOff_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimDefensiveWall,
};

const struct SpriteTemplate gMirrorCoatWallSpriteTemplate =
{
    .tileTag = ANIM_TAG_RED_LIGHT_WALL,
    .paletteTag = ANIM_TAG_RED_LIGHT_WALL,
    .oam = &gOamData_AffineOff_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimDefensiveWall,
};

const struct SpriteTemplate gBarrierWallSpriteTemplate =
{
    .tileTag = ANIM_TAG_GRAY_LIGHT_WALL,
    .paletteTag = ANIM_TAG_GRAY_LIGHT_WALL,
    .oam = &gOamData_AffineOff_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimDefensiveWall,
};

const struct SpriteTemplate gMagicCoatWallSpriteTemplate =
{
    .tileTag = ANIM_TAG_ORANGE_LIGHT_WALL,
    .paletteTag = ANIM_TAG_ORANGE_LIGHT_WALL,
    .oam = &gOamData_AffineOff_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimDefensiveWall,
};

static const union AnimCmd sAnim_ReflectSparkle[] =
{
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_FRAME(16, 3),
    ANIMCMD_FRAME(32, 3),
    ANIMCMD_FRAME(48, 3),
    ANIMCMD_FRAME(64, 3),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_ReflectSparkle[] =
{
    sAnim_ReflectSparkle,
};

const struct SpriteTemplate gReflectSparkleSpriteTemplate =
{
    .tileTag = ANIM_TAG_SPARKLE_4,
    .paletteTag = ANIM_TAG_SPARKLE_4,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sAnims_ReflectSparkle,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimWallSparkle,
};

static const union AnimCmd sAnim_SpecialScreenSparkle[] =
{
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_FRAME(4, 5),
    ANIMCMD_FRAME(8, 5),
    ANIMCMD_FRAME(12, 5),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_SpecialScreenSparkle[] =
{
    sAnim_SpecialScreenSparkle,
};

const struct SpriteTemplate gSpecialScreenSparkleSpriteTemplate =
{
    .tileTag = ANIM_TAG_SPARKLE_3,
    .paletteTag = ANIM_TAG_SPARKLE_3,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = sAnims_SpecialScreenSparkle,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimWallSparkle,
};

const struct SpriteTemplate gGoldRingSpriteTemplate =
{
    .tileTag = ANIM_TAG_GOLD_RING,
    .paletteTag = ANIM_TAG_GOLD_RING,
    .oam = &gOamData_AffineOff_ObjNormal_16x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = TranslateAnimSpriteToTargetMonLocation,
};

static const union AnimCmd sAnim_BentSpoon_0[] =
{
    ANIMCMD_FRAME(8, 60, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 5, .hFlip = TRUE),
    ANIMCMD_FRAME(8, 5, .hFlip = TRUE),
    ANIMCMD_FRAME(0, 5, .hFlip = TRUE),
    ANIMCMD_FRAME(8, 22, .hFlip = TRUE),
    ANIMCMD_LOOP(0),
    ANIMCMD_FRAME(16, 5, .hFlip = TRUE),
    ANIMCMD_FRAME(8, 5, .hFlip = TRUE),
    ANIMCMD_FRAME(0, 5, .hFlip = TRUE),
    ANIMCMD_FRAME(8, 5, .hFlip = TRUE),
    ANIMCMD_LOOP(1),
    ANIMCMD_FRAME(8, 22, .hFlip = TRUE),
    ANIMCMD_FRAME(24, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(40, 22, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_BentSpoon_1[] =
{
    ANIMCMD_FRAME(8, 60),
    ANIMCMD_FRAME(16, 5),
    ANIMCMD_FRAME(8, 5),
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_FRAME(8, 22),
    ANIMCMD_LOOP(0),
    ANIMCMD_FRAME(16, 5),
    ANIMCMD_FRAME(8, 5),
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_FRAME(8, 5),
    ANIMCMD_LOOP(1),
    ANIMCMD_FRAME(8, 22),
    ANIMCMD_FRAME(24, 3),
    ANIMCMD_FRAME(32, 3),
    ANIMCMD_FRAME(40, 22),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_BentSpoon[] =
{
    sAnim_BentSpoon_0,
    sAnim_BentSpoon_1,
};

const struct SpriteTemplate gBentSpoonSpriteTemplate =
{
    .tileTag = ANIM_TAG_BENT_SPOON,
    .paletteTag = ANIM_TAG_BENT_SPOON,
    .oam = &gOamData_AffineOff_ObjNormal_16x32,
    .anims = sAnims_BentSpoon,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimBentSpoon,
};

static const union AnimCmd sAnim_QuestionMark[] =
{
    ANIMCMD_FRAME(0, 6),
    ANIMCMD_FRAME(16, 6),
    ANIMCMD_FRAME(32, 6),
    ANIMCMD_FRAME(48, 6),
    ANIMCMD_FRAME(64, 6),
    ANIMCMD_FRAME(80, 6),
    ANIMCMD_FRAME(96, 18),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_QuestionMark[] =
{
    sAnim_QuestionMark,
};

static const union AffineAnimCmd sAffineAnim_QuestionMark[] =
{
    AFFINEANIMCMD_FRAME(0, 0, 4, 4),
    AFFINEANIMCMD_FRAME(0, 0, -4, 8),
    AFFINEANIMCMD_FRAME(0, 0, 4, 4),
    AFFINEANIMCMD_LOOP(2),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims_QuestionMark[] =
{
    sAffineAnim_QuestionMark,
};

const struct SpriteTemplate gQuestionMarkSpriteTemplate =
{
    .tileTag = ANIM_TAG_AMNESIA,
    .paletteTag = ANIM_TAG_AMNESIA,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sAnims_QuestionMark,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimQuestionMark,
};

static const union AffineAnimCmd sAffineAnim_MeditateStretchAttacker[] =
{
    AFFINEANIMCMD_FRAME(-8, 10, 0, 16),
    AFFINEANIMCMD_FRAME(18, -18, 0, 16),
    AFFINEANIMCMD_FRAME(-20, 16, 0, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_Teleport[] =
{
    AFFINEANIMCMD_FRAME(64, -4, 0, 20),
    AFFINEANIMCMD_FRAME(0, 0, 0, -56),
    AFFINEANIMCMD_END,
};

static const struct SpriteTemplate sImprisonOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_HOLLOW_ORB,
    .paletteTag = ANIM_TAG_HOLLOW_ORB,
    .oam = &gOamData_AffineOff_ObjBlend_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gRedXSpriteTemplate =
{
    .tileTag = ANIM_TAG_X_SIGN,
    .paletteTag = ANIM_TAG_X_SIGN,
    .oam = &gOamData_AffineOff_ObjNormal_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimRedX,
};

static const union AffineAnimCmd sAffineAnim_SkillSwapOrb_0[] =
{
    AFFINEANIMCMD_FRAME(-0x8, -0x8, 0, 8),
    AFFINEANIMCMD_FRAME(0x8, 0x8, 0, 8),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sAffineAnim_SkillSwapOrb_1[] =
{
    AFFINEANIMCMD_FRAME(0xF0, 0xF0, 0, 0),
    AFFINEANIMCMD_FRAME(-0x8, -0x8, 0, 6),
    AFFINEANIMCMD_FRAME(0x8, 0x8, 0, 8),
    AFFINEANIMCMD_FRAME(-0x8, -0x8, 0, 2),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd sAffineAnim_SkillSwapOrb_2[] =
{
    AFFINEANIMCMD_FRAME(0xD0, 0xD0, 0, 0),
    AFFINEANIMCMD_FRAME(-0x8, -0x8, 0, 4),
    AFFINEANIMCMD_FRAME(0x8, 0x8, 0, 8),
    AFFINEANIMCMD_FRAME(-0x8, -0x8, 0, 4),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd sAffineAnim_SkillSwapOrb_3[] =
{
    AFFINEANIMCMD_FRAME(0xB0, 0xB0, 0, 0),
    AFFINEANIMCMD_FRAME(-0x8, -0x8, 0, 2),
    AFFINEANIMCMD_FRAME(0x8, 0x8, 0, 8),
    AFFINEANIMCMD_FRAME(-0x8, -0x8, 0, 6),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd *const sAffineAnims_SkillSwapOrb[] =
{
    sAffineAnim_SkillSwapOrb_0,
    sAffineAnim_SkillSwapOrb_1,
    sAffineAnim_SkillSwapOrb_2,
    sAffineAnim_SkillSwapOrb_3,
};

static const struct SpriteTemplate sSkillSwapOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_BLUEGREEN_ORB,
    .paletteTag = ANIM_TAG_ELECTRIC_ORBS,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_SkillSwapOrb,
    .callback = AnimSkillSwapOrb,
};

static const struct SpriteTemplate sPowerSwapOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_BLUEGREEN_ORB,
    .paletteTag = ANIM_TAG_RED_HEART,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_SkillSwapOrb,
    .callback = AnimSkillSwapOrb,
};

static const struct SpriteTemplate sGuardSwapOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_BLUEGREEN_ORB,
    .paletteTag = ANIM_TAG_BLUEGREEN_ORB,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_SkillSwapOrb,
    .callback = AnimSkillSwapOrb,
};

static const union AffineAnimCmd sAffineAnim_LusterPurgeCircle[] =
{
    AFFINEANIMCMD_FRAME(0x20, 0x20, 0, 0),
    AFFINEANIMCMD_FRAME(0x4, 0x4, 0, 120),
    AFFINEANIMCMD_END_ALT(1),
};

const union AffineAnimCmd *const gAffineAnims_LusterPurgeCircle[] =
{
    sAffineAnim_LusterPurgeCircle,
};

const struct SpriteTemplate gLusterPurgeCircleSpriteTemplate =
{
    .tileTag = ANIM_TAG_WHITE_CIRCLE_OF_LIGHT,
    .paletteTag = ANIM_TAG_WHITE_CIRCLE_OF_LIGHT,
    .oam = &gOamData_AffineDouble_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gAffineAnims_LusterPurgeCircle,
    .callback = AnimSpriteOnMonPos,
};

static const union AffineAnimCmd sAffineAnim_PsychoBoostOrb_0[] =
{
    AFFINEANIMCMD_FRAME(0x20, 0x20, 0, 0),
    AFFINEANIMCMD_FRAME(0x10, 0x10, 0, 17),
    AFFINEANIMCMD_LOOP(0),
    AFFINEANIMCMD_FRAME(-0x8, -0x8, 0, 10),
    AFFINEANIMCMD_FRAME(0x8, 0x8, 0, 10),
    AFFINEANIMCMD_LOOP(4),
    AFFINEANIMCMD_LOOP(0),
    AFFINEANIMCMD_FRAME(-0x10, -0x10, 0, 5),
    AFFINEANIMCMD_FRAME(0x10, 0x10, 0, 5),
    AFFINEANIMCMD_LOOP(7),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_PsychoBoostOrb_1[] =
{
    AFFINEANIMCMD_FRAME(-0x14, 0x18, 0, 15),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims_PsychoBoostOrb[] =
{
    sAffineAnim_PsychoBoostOrb_0,
    sAffineAnim_PsychoBoostOrb_1,
};

const struct SpriteTemplate gPsychoBoostOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_CIRCLE_OF_LIGHT,
    .paletteTag = ANIM_TAG_CIRCLE_OF_LIGHT,
    .oam = &gOamData_AffineDouble_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_PsychoBoostOrb,
    .callback = AnimPsychoBoost,
};

static const union AnimCmd sKinesisZapEnergyAnimCmds[] =
{
    ANIMCMD_FRAME(0, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(8, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(24, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(40, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(48, 3, .hFlip = TRUE),
    ANIMCMD_LOOP(1),
    ANIMCMD_END,
};

static const union AnimCmd *const sKinesisZapEnergyAnimTable[] =
{
    sKinesisZapEnergyAnimCmds,
};

const struct SpriteTemplate gKinesisZapEnergySpriteTemplate =
{
    .tileTag = ANIM_TAG_ALERT,
    .paletteTag = ANIM_TAG_ALERT,
    .oam = &gOamData_AffineOff_ObjNormal_32x16,
    .anims = sKinesisZapEnergyAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = Anim_KinesisZapEnergy,
};

const struct SpriteTemplate gPsywaveRingSpriteTemplate =
{
    .tileTag = ANIM_TAG_BLUE_RING,
    .paletteTag = ANIM_TAG_BLUE_RING,
    .oam = &gOamData_AffineDouble_ObjNormal_16x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gGrowingRingAffineAnimTable,
    .callback = AnimToTargetInSinWave,
};

const struct SpriteTemplate gMistBallSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_BUBBLES,
    .paletteTag = ANIM_TAG_SMALL_BUBBLES,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimThrowMistBall,
};

static const u8 sMistBallBlendCoeefs[] =
{
    0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5,
};

static const union AffineAnimCmd sAffineAnim_PowerTrick[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, 20, 1),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd *const sAffineAnims_PowerTrick[] =
{
    sAffineAnim_PowerTrick,
};

const struct SpriteTemplate gPowerTrickSpriteTemplate =    
{
    .tileTag = ANIM_TAG_POWER_TRICK,
    .paletteTag = ANIM_TAG_POWER_TRICK,
    .oam = &gOamData_AffineNormal_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_PowerTrick,
    .callback = AnimSpriteOnMonForDuration,
};

// For the rectangular wall sprite used by Reflect, Mirror Coat, etc.
// arg 0: x offset
// arg 1: y offset
// arg 2: anim tag
static void AnimDefensiveWall(struct Sprite *sprite)
{
    u8 battler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
    bool8 toBG2 = (GetBattlerSpriteBGPriorityRank(battler) ^ 1);
    
    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
    {
        sprite->oam.priority = 2;
        sprite->subpriority = 200;
    }
	
    if (IsBattlerSpriteVisible(battler))
        MoveBattlerSpriteToBG(battler, toBG2);
	
    battler = BATTLE_PARTNER(battler);
	
    if (IsBattlerSpriteVisible(battler))
        MoveBattlerSpriteToBG(battler, toBG2 ^ TRUE);
	
    if (IsDoubleBattleForBattler(gBattleAnimAttacker))
    {
        if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
        {
            sprite->x = 72;
            sprite->y = 80;
        }
        else
        {
            sprite->x = 176;
            sprite->y = 40;
        }
    }
    else
    {
        if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
            gBattleAnimArgs[0] = -gBattleAnimArgs[0];
		
        sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X) + gBattleAnimArgs[0];
        sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y) + gBattleAnimArgs[1];
    }
    sprite->data[0] = 256 + IndexOfSpritePaletteTag(gBattleAnimArgs[2]) * 16;
    sprite->callback = AnimDefensiveWall_Step;
    sprite->callback(sprite);
}

static void AnimDefensiveWall_Step(struct Sprite *sprite)
{
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sprite->data[3], 16 - sprite->data[3]));
	
    if (sprite->data[3] == 13)
        sprite->callback = AnimDefensiveWall_Step2;
    else
        ++sprite->data[3];
}

static void AnimDefensiveWall_Step2(struct Sprite *sprite)
{
	s8 i;
    u16 startOffset, color;

    if (++sprite->data[1] == 2)
    {
        sprite->data[1] = 0;
		
        startOffset = sprite->data[0];
        color = gPlttBufferFaded[startOffset + 8];
		
        for (i = 8; i > 0; --i)
            gPlttBufferFaded[startOffset + i] = gPlttBufferFaded[startOffset + i - 1];
		
        gPlttBufferFaded[startOffset + 1] = color;
		
        if (++sprite->data[2] == 16)
            sprite->callback = AnimDefensiveWall_Step3;
    }
}

static void AnimDefensiveWall_Step3(struct Sprite *sprite)
{
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sprite->data[3], 16 - sprite->data[3]));
	
    if (--sprite->data[3] == -1)
    {
        u8 battler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);

        if (IsBattlerSpriteVisible(battler))
            gSprites[gBattlerSpriteIds[battler]].invisible = FALSE;
		
        battler = BATTLE_PARTNER(battler);
		
        if (IsBattlerSpriteVisible(battler))
            gSprites[gBattlerSpriteIds[battler]].invisible = FALSE;
        
        sprite->invisible = TRUE;
        sprite->callback = AnimDefensiveWall_Step4;
    }
}

static void AnimDefensiveWall_Step4(struct Sprite *sprite)
{
    u8 battler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
    bool8 toBG2 = (GetBattlerSpriteBGPriorityRank(battler) ^ 1);

    if (IsBattlerSpriteVisible(battler))
        ResetBattleAnimBg(toBG2);
	
    battler = BATTLE_PARTNER(battler);
	
    if (IsBattlerSpriteVisible(battler))
        ResetBattleAnimBg(toBG2 ^ TRUE);
	
    sprite->callback = DestroyAnimSprite;
}

// Animates the sparkle that appears during Reflect or Light Screen/Mirror Coat.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: anim battler
static void AnimWallSparkle(struct Sprite *sprite)
{
    if (sprite->data[0] == 0)
    {
        if (IsDoubleBattleForBattler(gBattleAnimAttacker))
        {
            if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
            {
                sprite->x = 72 - gBattleAnimArgs[0];
                sprite->y = gBattleAnimArgs[1] + 80;
            }
            else
            {
                sprite->x = gBattleAnimArgs[0] + 176;
                sprite->y = gBattleAnimArgs[1] + 40;
            }
        }
        else
			InitSpritePosToAnimBattler(sprite, gBattleAnimArgs[2], FALSE);
            
        ++sprite->data[0];
    }
    else if (sprite->animEnded || sprite->affineAnimEnded)
        DestroyAnimSprite(sprite);
}

// Animates the Kinesis bent spoon sprite.
// No args.
static void AnimBentSpoon(struct Sprite *sprite)
{
    SetSpriteCoordsToAnimAttackerCoords(sprite);
	
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
    {
        StartSpriteAnim(sprite, 1);
        sprite->x -= 40;
        sprite->y += 10;
        sprite->data[1] = -1;
    }
    else
    {
        sprite->x += 40;
        sprite->y -= 10;
        sprite->data[1] = 1;
    }
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
    sprite->callback = RunStoredCallbackWhenAnimEnds;
}

// Animates the Amnesia's question mark sprite.
// No args.
static void AnimQuestionMark(struct Sprite *sprite)
{
    s16 x = GetBattlerSpriteCoordAttr(gBattleAnimAttacker, BATTLER_COORD_ATTR_WIDTH) / 2;
    s16 y = GetBattlerSpriteCoordAttr(gBattleAnimAttacker, BATTLER_COORD_ATTR_HEIGHT) / -2;

    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_OPPONENT)
        x = -x;
	
    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X) + x;
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET) + y;
	
    if (sprite->y < 16)
        sprite->y = 16;
	
    StoreSpriteCallbackInData6(sprite, AnimQuestionMarkStep);
    sprite->callback = RunStoredCallbackWhenAnimEnds;
}

static void AnimQuestionMarkStep(struct Sprite *sprite)
{
    sprite->oam.affineMode = ST_OAM_AFFINE_NORMAL;
    sprite->affineAnims = sAffineAnims_QuestionMark;
    sprite->data[0] = 0;
    InitSpriteAffineAnim(sprite);
    sprite->callback = AnimQuestionMarkStep2;
}

static void AnimQuestionMarkStep2(struct Sprite *sprite)
{
    switch (sprite->data[0])
    {
    case 0:
        if (sprite->affineAnimEnded)
        {
            FreeSpriteOamMatrix(sprite);
            sprite->data[1] = 18;
            ++sprite->data[0];
        }
        break;
    case 1:
        if (--sprite->data[1] == -1)
            DestroyAnimSprite(sprite);
        break;
    }
}

// Animates the Meditate attacker's stretch effect.
void AnimTask_MeditateStretchAttacker(u8 taskId)
{
    PrepareAffineAnimInTaskData(&gTasks[taskId], GetAnimBattlerSpriteId(ANIM_ATTACKER), sAffineAnim_MeditateStretchAttacker);
    gTasks[taskId].func = AnimTask_DestroyTaskAfterAffineAnimFromTaskDataEnds;
}

// Animates the teleport effect.
void AnimTask_Teleport(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    task->data[3] = GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER ? 4 : 8;
    PrepareAffineAnimInTaskData(task, GetAnimBattlerSpriteId(ANIM_ATTACKER), sAffineAnim_Teleport);
    task->func = AnimTask_Teleport_Step;
}

static void AnimTask_Teleport_Step(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->data[1])
    {
    case 0:
        RunAffineAnimFromTaskData(task);
		
        if (++task->data[2] > 19)
            ++task->data[1];
        break;
    case 1:
        if (task->data[3] != 0)
        {
            gSprites[task->data[15]].y2 -= 8;
            --task->data[3];
        }
        else
        {
            gSprites[task->data[15]].invisible = TRUE;
            gSprites[task->data[15]].x = 272;
            ResetSpriteRotScale(task->data[15]);
            DestroyAnimVisualTask(taskId);
        }
        break;
    }
}

// Animates MOVE_IMPRISON's transparent orbs.
// No args.
void AnimTask_ImprisonOrbs(u8 taskId)
{
    u16 var0, var1;
    struct Task *task = &gTasks[taskId];

    task->data[3] = 16;
    task->data[4] = 0;
    task->data[13] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    task->data[14] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
	
    var0 = GetBattlerSpriteCoordAttr(gBattleAnimAttacker, BATTLER_COORD_ATTR_WIDTH) / 3;
    var1 = GetBattlerSpriteCoordAttr(gBattleAnimAttacker, BATTLER_COORD_ATTR_HEIGHT) / 3;
    task->data[12] = var0 > var1 ? var0 : var1;
	
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16, 0));
	
    task->func = AnimTask_ImprisonOrbs_Step;
}

static void AnimTask_ImprisonOrbs_Step(u8 taskId)
{
    u16 i;
    u8 spriteId;
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        if (++task->data[1] > 8)
        {
            task->data[1] = 0;
			
            spriteId = CreateSprite(&sImprisonOrbSpriteTemplate, task->data[13], task->data[14], 0);
            task->data[task->data[2] + 8] = spriteId;
			
            if (spriteId != MAX_SPRITES)
            {
                switch (task->data[2])
                {
                case 0:
                    gSprites[spriteId].x2 = task->data[12];
                    gSprites[spriteId].y2 = -task->data[12];
                    break;
                case 1:
                    gSprites[spriteId].x2 = -task->data[12];
                    gSprites[spriteId].y2 = task->data[12];
                    break;
                case 2:
                    gSprites[spriteId].x2 = task->data[12];
                    gSprites[spriteId].y2 = task->data[12];
                    break;
                case 3:
                    gSprites[spriteId].x2 = -task->data[12];
                    gSprites[spriteId].y2 = -task->data[12];
                    break;
                }
            }
            if (++task->data[2] == 5)
                ++task->data[0];
        }
        break;
    case 1:
        if (task->data[1] & 1)
            --task->data[3];
        else
            ++task->data[4];
		
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(task->data[3], task->data[4]));
		
        if (++task->data[1] == 32)
        {
            for (i = 8; i < 13; ++i)
			{
                if (task->data[i] != MAX_SPRITES)
                    DestroySprite(&gSprites[task->data[i]]);
			}
            ++task->data[0];
        }
        break;
    case 2:
        ++task->data[0];
        break;
    case 3:
        DestroyAnimVisualTaskAndDisableBlend(taskId);
        break;
    }
}

static void AnimRedX_Step(struct Sprite *sprite)
{
    if (sprite->data[1] > sprite->data[0] - 10)
        sprite->invisible = sprite->data[1] & 1;
	
    if (sprite->data[1] == sprite->data[0])
        DestroyAnimSprite(sprite);
	
    ++sprite->data[1];
}

// Animates an red x sprite on the given battler. Used by moves like Imprison and Rock Tomb.
// arg 0: anim battler
// arg 1: duration
static void AnimRedX(struct Sprite *sprite)
{
    if (gBattleAnimArgs[0] == ANIM_ATTACKER)
    {
        sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
        sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
    }
    sprite->data[0] = gBattleAnimArgs[1];
    sprite->callback = AnimRedX_Step;
}

// Animates MOVE_SKILL_SWAP's orbs.
// arg 0: from target to attacker (boolean)
// arg 1: wich orb sprite
void AnimTask_SkillSwap(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    
    if (gBattleAnimArgs[0])
    {
        task->data[10] = -10;
        task->data[11] = GetBattlerSpriteCoordAttr(gBattleAnimTarget, BATTLER_COORD_ATTR_LEFT) + 8;
        task->data[12] = GetBattlerSpriteCoordAttr(gBattleAnimTarget, BATTLER_COORD_ATTR_TOP) + 8;
        task->data[13] = GetBattlerSpriteCoordAttr(gBattleAnimAttacker, BATTLER_COORD_ATTR_LEFT) + 8;
        task->data[14] = GetBattlerSpriteCoordAttr(gBattleAnimAttacker, BATTLER_COORD_ATTR_TOP) + 8;
    }
    else
    {
        task->data[10] = 10;
        task->data[11] = GetBattlerSpriteCoordAttr(gBattleAnimAttacker, BATTLER_COORD_ATTR_RIGHT) - 8;
        task->data[12] = GetBattlerSpriteCoordAttr(gBattleAnimAttacker, BATTLER_COORD_ATTR_BOTTOM) - 8;
        task->data[13] = GetBattlerSpriteCoordAttr(gBattleAnimTarget, BATTLER_COORD_ATTR_RIGHT) - 8;
        task->data[14] = GetBattlerSpriteCoordAttr(gBattleAnimTarget, BATTLER_COORD_ATTR_BOTTOM) - 8;
    }
    task->data[1] = 6;
	task->data[3] = gBattleAnimArgs[1];
    task->func = AnimTask_SkillSwapOrbs;
}

static void AnimTask_SkillSwapOrbs(u8 taskId)
{
    u8 spriteId;
	s16 x, y;
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        if (++task->data[1] > 6)
        {
            task->data[1] = 0;
			
			x = task->data[11];
			y = task->data[12];
			
			switch (task->data[3])
			{
				case 0: // Skill Swap orbs
					spriteId = CreateSprite(&sSkillSwapOrbSpriteTemplate, x, y, 0);
					break;
				case 1: // Power Swap orbs
					spriteId = CreateSprite(&sPowerSwapOrbSpriteTemplate, x, y, 0);
					break;
				case 2: // Guard Swap orbs
					spriteId = CreateSprite(&sGuardSwapOrbSpriteTemplate, x, y, 0);
					break;
			}
			
            if (spriteId != MAX_SPRITES)
            {
                gSprites[spriteId].data[0] = 16;
                gSprites[spriteId].data[2] = task->data[13];
                gSprites[spriteId].data[4] = task->data[14];
                gSprites[spriteId].data[5] = task->data[10];
                InitAnimArcTranslation(&gSprites[spriteId]);
                StartSpriteAffineAnim(&gSprites[spriteId], task->data[2] & 3);
            }
			
            if (++task->data[2] == 12)
                ++task->data[0];
        }
        break;
    case 1:
        if (++task->data[1] > 17)
            DestroyAnimVisualTask(taskId);
        break;
    }
}

static void AnimSkillSwapOrb(struct Sprite *sprite)
{
    if (TranslateAnimHorizontalArc(sprite))
		DestroySpriteAndFreeMatrix(sprite);
}

// The scanline effect that distorts the target during Extrasensory by segmenting the mon vertically and shifting the slices
// arg 0: Stage. Stage 0 is a slight right distortion, 1 is a medium left distortion, and 2 is a severe right distortion
void AnimTask_ExtrasensoryDistortion(u8 taskId)
{
    s16 i;
    u8 yOffset = GetBattlerYCoordWithElevation(gBattleAnimTarget);
    struct ScanlineEffectParams scanlineParams;
    struct Task *task = &gTasks[taskId];
    
    task->data[14] = yOffset - 32;
	
    switch (gBattleAnimArgs[0])
    {
    case 0:
        task->data[11] = 2;
        task->data[12] = 5;
        task->data[13] = 64;
        task->data[15] = yOffset + 32;
        break;
    case 1:
        task->data[11] = 2;
        task->data[12] = 5;
        task->data[13] = 192;
        task->data[15] = yOffset + 32;
        break;
    case 2:
        task->data[11] = 4;
        task->data[12] = 4;
        task->data[13] = 0;
        task->data[15] = yOffset + 32;
        break;
    }
    if (task->data[14] < 0)
        task->data[14] = 0;
	
    if (GetBattlerSpriteBGPriorityRank(gBattleAnimTarget) == 1)
    {
        task->data[10] = gBattle_BG1_X;
        scanlineParams.dmaDest = &REG_BG1HOFS;
    }
    else
    {
        task->data[10] = gBattle_BG2_X;
        scanlineParams.dmaDest = &REG_BG2HOFS;
    }
	
    for (i = task->data[14]; i <= task->data[14] + 64; ++i)
    {
        gScanlineEffectRegBuffers[0][i] = task->data[10];
        gScanlineEffectRegBuffers[1][i] = task->data[10];
    }
    scanlineParams.dmaControl = SCANLINE_EFFECT_DMACNT_16BIT;
    scanlineParams.initState = 1;
    ScanlineEffect_SetParams(scanlineParams);
    task->func = AnimTask_ExtrasensoryDistortion_Step;
}

static void AnimTask_ExtrasensoryDistortion_Step(u8 taskId)
{
    s16 sineIndex, i;
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        sineIndex = task->data[13];
		
        for (i = task->data[14]; i <= task->data[15]; ++i)
        {
            s16 var2 = (gSineTable[sineIndex] >> task->data[12]);

            if (var2 > 0)
                var2 += (task->data[1] & 3);
            else if (var2 < 0)
                var2 -= (task->data[1] & 3);
			
            gScanlineEffectRegBuffers[0][i] = task->data[10] + var2;
            gScanlineEffectRegBuffers[1][i] = task->data[10] + var2;
			
            sineIndex += task->data[11];
        }
        if (++task->data[1] > 23)
            ++task->data[0];
        break;
    case 1:
        gScanlineEffect.state = 3;
        ++task->data[0];
        break;
    case 2:
        DestroyAnimVisualTask(taskId);
        break;
    }
}

// Creates a cloned transparent sprite of the battler that grows and then shrinks back to original size. Used by Extrasensory
// arg 0: anim battler
void AnimTask_TransparentCloneGrowAndShrink(u8 taskId)
{
    s16 spriteId;
    s16 matrixNum = AllocOamMatrix();
    struct Task *task = &gTasks[taskId];
    
    if (matrixNum == 0xFF)
    {
        DestroyAnimVisualTask(taskId);
        return;
    }
    spriteId = CloneBattlerSpriteWithBlend(gBattleAnimArgs[0]);
	
    if (spriteId < 0)
    {
        FreeOamMatrix(matrixNum);
        DestroyAnimVisualTask(taskId);
        return;
    }
    gSprites[spriteId].callback = SpriteCallbackDummy;
    gSprites[spriteId].oam.affineMode = ST_OAM_AFFINE_DOUBLE;
    gSprites[spriteId].oam.matrixNum = matrixNum;
    gSprites[spriteId].affineAnimPaused = TRUE;
    gSprites[spriteId].subpriority++;
	
    SetSpriteRotScale(spriteId, 256, 256, 0);
    CalcCenterToCornerVec(&gSprites[spriteId], gSprites[spriteId].oam.shape, gSprites[spriteId].oam.size, gSprites[spriteId].oam.affineMode);
    
	task->data[13] = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);
    task->data[14] = matrixNum;
    task->data[15] = spriteId;
    task->func = AnimTask_TransparentCloneGrowAndShrink_Step;
}

static void AnimTask_TransparentCloneGrowAndShrink_Step(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        task->data[1] += 4;
        task->data[2] = 256 - (gSineTable[task->data[1]] >> 1);
		
        SetSpriteRotScale(task->data[15], task->data[2], task->data[2], 0);
        SetBattlerSpriteYOffsetFromOtherYScale(task->data[15], task->data[13]);
		
        if (task->data[1] == 48)
            ++task->data[0];
        break;
    case 1:
        task->data[1] -= 4;
        task->data[2] = 256 - (gSineTable[task->data[1]] >> 1);
		
        SetSpriteRotScale(task->data[15], task->data[2], task->data[2], 0);
        SetBattlerSpriteYOffsetFromOtherYScale(task->data[15], task->data[13]);
		
        if (task->data[1] == 0)
            ++task->data[0];
        break;
    case 2:
        DestroySpriteWithActiveSheet(&gSprites[task->data[15]]);
        ++task->data[0];
        break;
    case 3:
        FreeOamMatrix(task->data[14]);
        DestroyAnimVisualTask(taskId);
        break;
    }
}

// Animates MOVE_PSYCHO_BOOST's circle of light.
// No args.
static void AnimPsychoBoost(struct Sprite *sprite)
{
    switch (sprite->data[0])
    {
    case 0:
        sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
        sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y);
		
        sprite->data[1] = 8;
		
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sprite->data[1], 16 - sprite->data[1]));
		
        ++sprite->data[0];
        break;
    case 1:
        if (sprite->affineAnimEnded)
        {
            PlaySE12WithPanning(SE_M_TELEPORT, BattleAnimAdjustPanning(SOUND_PAN_ATTACKER));
            ChangeSpriteAffineAnim(sprite, 1);
            ++sprite->data[0];
        }
        break;
    case 2:
        if (sprite->data[2]++ > 1)
        {
            sprite->data[2] = 0;
			
            --sprite->data[1];
			
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sprite->data[1], 16 - sprite->data[1]));
			
            if (sprite->data[1] == 0)
            {
                ++sprite->data[0];
                sprite->invisible = TRUE;
            }
        }
        sprite->data[3] += 0x380;
        sprite->y2 -= sprite->data[3] >> 8;
        sprite->data[3] &= 0xFF;
        break;
    case 3:
        DestroyAnimSpriteAndDisableBlend(sprite);
        break;
    }
}

// Animates the psychic background color effect.
void AnimTask_SetPsychicBackground(u8 taskId)
{
    gTasks[taskId].func = SetPsychicBackground_Step;
    gAnimVisualTaskCount--;
}

static void SetPsychicBackground_Step(u8 taskId)
{
    u8 i, paletteIndex;
    u16 lastColor;

    if (++gTasks[taskId].data[5] == 4)
    {
		gTasks[taskId].data[5] = 0;
		
		paletteIndex = 2; // battle bg palette num
        lastColor = gPlttBufferFaded[paletteIndex * 16 + 11];
		
        for (i = 10; i > 0; i--)
            gPlttBufferFaded[paletteIndex * 16 + i + 1] = gPlttBufferFaded[paletteIndex * 16 + i];

        gPlttBufferFaded[paletteIndex * 16 + 1] = lastColor;
    }

    if (gBattleAnimArgs[ARG_RET_ID] == -1)
        DestroyTask(taskId);
}

// Animates a "zap of energy" used in KINESIS.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: vertical flip (boolean)
void Anim_KinesisZapEnergy(struct Sprite *sprite)
{
    SetSpriteCoordsToAnimAttackerCoords(sprite);
	
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        sprite->x -= gBattleAnimArgs[0];
    else
        sprite->x += gBattleAnimArgs[0];

    sprite->y += gBattleAnimArgs[1];
	
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        sprite->hFlip = TRUE;
    
	if (gBattleAnimArgs[2])
		sprite->vFlip = TRUE;

    sprite->callback = RunStoredCallbackWhenAnimEnds;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Copies the target mon's sprite, and makes a white silhouette that shrinks away.
void AnimTask_RolePlaySilhouette(u8 taskId)
{
	struct Pokemon *targetMon = GetBattlerPartyIndexPtr(gBattleAnimTarget);
    u32 personality = GetMonData(targetMon, MON_DATA_PERSONALITY);
    bool8 isShiny = GetMonData(targetMon, MON_DATA_IS_SHINY);
    u16 species;
    s16 xOffset;
    u32 priority;
    u8 spriteId;
	bool8 isBackPic;
	
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
    {
        isBackPic = FALSE;
        xOffset = 20;
    }
    else
    {
        isBackPic = TRUE;
        xOffset = -20;
    }
	
	if (gBattleSpritesDataPtr->battlerData[gBattleAnimTarget].transformSpecies == SPECIES_NONE)
		species = GetMonData(targetMon, MON_DATA_SPECIES);
	else
		species = gBattleSpritesDataPtr->battlerData[gBattleAnimTarget].transformSpecies;
	
	priority = GetBattlerSpriteBGPriority(gBattleAnimAttacker);
    spriteId = CreateAdditionalMonSpriteForMoveAnim(species, isBackPic, GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X) + xOffset, GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y), 5, personality, isShiny, FALSE);
    
	gSprites[spriteId].oam.priority = priority;
    gSprites[spriteId].oam.objMode = ST_OAM_OBJ_BLEND;
	
    FillPalette(RGB_WHITE, (gSprites[spriteId].oam.paletteNum << 4) + 0x100, 32);
	
    gSprites[spriteId].oam.priority = priority;
	
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[1], 16 - gTasks[taskId].data[1]));
	
    gTasks[taskId].data[0] = spriteId;
    gTasks[taskId].func = AnimTask_RolePlaySilhouetteStep1;
}

static void AnimTask_RolePlaySilhouetteStep1(u8 taskId)
{
    if (gTasks[taskId].data[10]++ > 1)
    {
        gTasks[taskId].data[10] = 0;
        gTasks[taskId].data[1]++;
		
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[1], 16 - gTasks[taskId].data[1]));
		
        if (gTasks[taskId].data[1] == 10)
        {
            gTasks[taskId].data[10] = 256;
            gTasks[taskId].data[11] = 256;
            gTasks[taskId].func = AnimTask_RolePlaySilhouetteStep2;
        }
    }
}

static void AnimTask_RolePlaySilhouetteStep2(u8 taskId)
{
    u8 spriteId = gTasks[taskId].data[0];
    
    gTasks[taskId].data[10] -= 16;
    gTasks[taskId].data[11] += 128;
	
    gSprites[spriteId].oam.affineMode |= ST_OAM_AFFINE_DOUBLE_MASK;
    TrySetSpriteRotScale(&gSprites[spriteId], TRUE, gTasks[taskId].data[10], gTasks[taskId].data[11], 0);
	
    if (++gTasks[taskId].data[12] == 9)
    {
        TryResetSpriteAffineState(&gSprites[spriteId]);
        DestroySpriteAndFreeResources(&gSprites[spriteId]);
		
        gTasks[taskId].func = DestroyAnimVisualTaskAndDisableBlend;
    }
}

// Throws the ball in Mist Ball.
// arg 0: initial x offset
// arg 1: initial y offset
// arg 2: target x offset
// arg 3: target y offset
// arg 4: duration
// arg 5: lower 8 bits = location on attacking mon, upper 8 bits = location on target mon pick to target
static void AnimThrowMistBall(struct Sprite *sprite)
{
    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
    sprite->callback = TranslateAnimSpriteToTargetMonLocation;
	sprite->callback(sprite);
}

// Displays misty background in Mist Ball.
// No args.
void AnimTask_LoadMistTiles(u8 taskId)
{
    struct BattleAnimBgData animBg;

    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 16));
	
    SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
    SetAnimBgAttribute(1, BG_ANIM_SCREEN_SIZE, 0);
    SetAnimBgAttribute(1, BG_ANIM_CHAR_BASE_BLOCK, 1);
	
    gBattle_BG1_X = 0;
    gBattle_BG1_Y = 0;
	
    SetGpuReg(REG_OFFSET_BG1HOFS, gBattle_BG1_X);
    SetGpuReg(REG_OFFSET_BG1VOFS, gBattle_BG1_Y);
	
    GetBattleAnimBgData(&animBg, 1);
    LoadBgTiles(animBg.bgId, gWeatherFogHorizontalTiles, 0x800, animBg.tilesOffset);
    AnimLoadCompressedBgTilemap(animBg.bgId, gBattleAnimFogTilemap);
    LoadPalette(&gDefaultWeatherSpritePalette, animBg.paletteId * 16, 32);
	
    gTasks[taskId].data[15] = -1;
    gTasks[taskId].func = AnimTask_OverlayFogTiles;
}

static void AnimTask_OverlayFogTiles(u8 taskId)
{
    struct BattleAnimBgData animBg;

    gBattle_BG1_X += gTasks[taskId].data[15];
	
    switch (gTasks[taskId].data[12])
    {
    case 0:
        gTasks[taskId].data[9] += 1;
		
        gTasks[taskId].data[11] = sMistBallBlendCoeefs[gTasks[taskId].data[9]];
		
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[11], 17 - gTasks[taskId].data[11]));
		
        if (gTasks[taskId].data[11] == 5)
        {
            gTasks[taskId].data[11] = 0;
			++gTasks[taskId].data[12];
        }
        break;
    case 1:
        if (++gTasks[taskId].data[11] == 0x51)
        {
            gTasks[taskId].data[11] = 5;
            ++gTasks[taskId].data[12];
        }
        break;
    case 2:
        if (++gTasks[taskId].data[10] == 4)
        {
            gTasks[taskId].data[10] = 0;
			
            --gTasks[taskId].data[11];
			
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[11], 16 - gTasks[taskId].data[11]));
			
            if (gTasks[taskId].data[11] == 0)
            {
				gTasks[taskId].data[11] = 0;
                ++gTasks[taskId].data[12];
            }
        }
        break;
    case 3:
        GetBattleAnimBgData(&animBg, 1);
        InitBattleAnimBg(1);
        InitBattleAnimBg(2);
        ++gTasks[taskId].data[12];
        // fall through
    case 4:
        SetAnimBgAttribute(1, BG_ANIM_CHAR_BASE_BLOCK, 0);
		SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
		
        gBattle_BG1_X = 0;
        gBattle_BG1_Y = 0;
		
        DestroyAnimVisualTaskAndDisableBlend(taskId);
        break;
    }
}

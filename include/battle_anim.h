#ifndef GUARD_BATTLE_ANIM_H
#define GUARD_BATTLE_ANIM_H

#include "battle.h"
#include "data.h"
#include "task.h"
#include "constants/battle_anim.h"

enum
{
	ANIM_TYPE_GENERAL,
	ANIM_TYPE_MOVE,
	ANIM_TYPE_STATUS,
	ANIM_TYPE_SPECIAL,
};

enum
{
    BG_ANIM_SCREEN_SIZE,
    BG_ANIM_AREA_OVERFLOW_MODE,
    BG_ANIM_MOSAIC,
    BG_ANIM_CHAR_BASE_BLOCK,
    BG_ANIM_PRIORITY,
    BG_ANIM_PALETTES_MODE,
    BG_ANIM_SCREEN_BASE_BLOCK
};

struct BattleAnimBgData
{
    u16 tilesOffset;
    u8 paletteId;
    u8 bgId;
    u16 *bgTilemap;
    u8 *bgTiles;
};

struct BattleAnimBackground
{
    const u32 *image;
    const u32 *palette;
    const u32 *tilemap;
};

struct BattleAnimTable
{
	const u8 *script;
	bool8 hideHpBoxes:1; // Animation will hide the battler's hp boxes when played
	bool8 shouldBePlayed:1; // Animation will ignore battle scenes off will be played regardless
	bool8 ignoreSemiInvulnerability:1; // Animation should be played regardless battler is semi invulnerable
	bool8 ignoreSubstitute:1; // Animation should be played regardless battler is behind substitute
	bool8 substituteRecede:1; // Substitute will be removed before doing the anim, being restored at the end
	bool8 changeForm:1; // Animation will change the Pokémon's form, necessary for the substitute out and in animation
	bool8 unused:2;
};

#define ANIM_ARGS_COUNT 8

// Linear Translation
#define sTransl_Speed    data[0]
#define sTransl_Duration data[0] // for Fast
#define sTransl_InitX    data[1]
#define sTransl_DestX    data[2]
#define sTransl_InitY    data[3]
#define sTransl_DestY    data[4]
#define sTransl_ArcAmpl  data[5]

extern void (*gAnimScriptCallback)(void);
extern bool8 gAnimScriptActive;
extern u8 gAnimVisualTaskCount;
extern u8 gAnimSoundTaskCount;
extern struct DisableStruct *gAnimDisableStructPtr;
extern u16 gAnimMovePower;
extern u8 gAnimFriendship;
extern u16 gWeatherMoveAnim;
extern s16 gBattleAnimArgs[ANIM_ARGS_COUNT];
extern u8 gAnimMoveTurn;
extern u8 gBattleAnimAttacker;
extern u8 gBattleAnimTarget;
extern u8 gBattlerSpriteIds[MAX_BATTLERS_COUNT];
extern s32 gAnimMoveDmg;
extern u16 gAnimBattlerSpecies[MAX_BATTLERS_COUNT];
extern u8 gAnimCustomPanning;
extern u8 gActiveAbilityPopUps;

// battle_anim.c
extern const struct OamData gOamData_AffineOff_ObjNormal_8x8;
extern const struct OamData gOamData_AffineOff_ObjNormal_16x16;
extern const struct OamData gOamData_AffineOff_ObjNormal_32x32;
extern const struct OamData gOamData_AffineOff_ObjNormal_64x64;
extern const struct OamData gOamData_AffineOff_ObjNormal_16x8;
extern const struct OamData gOamData_AffineOff_ObjNormal_32x8;
extern const struct OamData gOamData_AffineOff_ObjNormal_32x16;
extern const struct OamData gOamData_AffineOff_ObjNormal_64x32;
extern const struct OamData gOamData_AffineOff_ObjNormal_8x16;
extern const struct OamData gOamData_AffineOff_ObjNormal_8x32;
extern const struct OamData gOamData_AffineOff_ObjNormal_16x32;
extern const struct OamData gOamData_AffineOff_ObjNormal_32x64;
extern const struct OamData gOamData_AffineNormal_ObjNormal_8x8;
extern const struct OamData gOamData_AffineNormal_ObjNormal_16x16;
extern const struct OamData gOamData_AffineNormal_ObjNormal_32x32;
extern const struct OamData gOamData_AffineNormal_ObjNormal_64x64;
extern const struct OamData gOamData_AffineNormal_ObjNormal_16x8;
extern const struct OamData gOamData_AffineNormal_ObjNormal_32x8;
extern const struct OamData gOamData_AffineNormal_ObjNormal_32x16;
extern const struct OamData gOamData_AffineNormal_ObjNormal_64x32;
extern const struct OamData gOamData_AffineNormal_ObjNormal_8x16;
extern const struct OamData gOamData_AffineNormal_ObjNormal_8x32;
extern const struct OamData gOamData_AffineNormal_ObjNormal_16x32;
extern const struct OamData gOamData_AffineNormal_ObjNormal_32x64;
extern const struct OamData gOamData_AffineDouble_ObjNormal_8x8;
extern const struct OamData gOamData_AffineDouble_ObjNormal_16x16;
extern const struct OamData gOamData_AffineDouble_ObjNormal_32x32;
extern const struct OamData gOamData_AffineDouble_ObjNormal_64x64;
extern const struct OamData gOamData_AffineDouble_ObjNormal_16x8;
extern const struct OamData gOamData_AffineDouble_ObjNormal_32x8;
extern const struct OamData gOamData_AffineDouble_ObjNormal_32x16;
extern const struct OamData gOamData_AffineDouble_ObjNormal_64x32;
extern const struct OamData gOamData_AffineDouble_ObjNormal_8x16;
extern const struct OamData gOamData_AffineDouble_ObjNormal_8x32;
extern const struct OamData gOamData_AffineDouble_ObjNormal_16x32;
extern const struct OamData gOamData_AffineDouble_ObjNormal_32x64;
extern const struct OamData gOamData_AffineOff_ObjBlend_8x8;
extern const struct OamData gOamData_AffineOff_ObjBlend_16x16;
extern const struct OamData gOamData_AffineOff_ObjBlend_32x32;
extern const struct OamData gOamData_AffineOff_ObjBlend_64x64;
extern const struct OamData gOamData_AffineOff_ObjBlend_16x8;
extern const struct OamData gOamData_AffineOff_ObjBlend_32x8;
extern const struct OamData gOamData_AffineOff_ObjBlend_32x16;
extern const struct OamData gOamData_AffineOff_ObjBlend_64x32;
extern const struct OamData gOamData_AffineOff_ObjBlend_8x16;
extern const struct OamData gOamData_AffineOff_ObjBlend_8x32;
extern const struct OamData gOamData_AffineOff_ObjBlend_16x32;
extern const struct OamData gOamData_AffineOff_ObjBlend_32x64;
extern const struct OamData gOamData_AffineNormal_ObjBlend_8x8;
extern const struct OamData gOamData_AffineNormal_ObjBlend_16x16;
extern const struct OamData gOamData_AffineNormal_ObjBlend_32x32;
extern const struct OamData gOamData_AffineNormal_ObjBlend_64x64;
extern const struct OamData gOamData_AffineNormal_ObjBlend_16x8;
extern const struct OamData gOamData_AffineNormal_ObjBlend_32x8;
extern const struct OamData gOamData_AffineNormal_ObjBlend_32x16;
extern const struct OamData gOamData_AffineNormal_ObjBlend_64x32;
extern const struct OamData gOamData_AffineNormal_ObjBlend_8x16;
extern const struct OamData gOamData_AffineNormal_ObjBlend_8x32;
extern const struct OamData gOamData_AffineNormal_ObjBlend_16x32;
extern const struct OamData gOamData_AffineNormal_ObjBlend_32x64;
extern const struct OamData gOamData_AffineDouble_ObjBlend_8x8;
extern const struct OamData gOamData_AffineDouble_ObjBlend_16x16;
extern const struct OamData gOamData_AffineDouble_ObjBlend_32x32;
extern const struct OamData gOamData_AffineDouble_ObjBlend_64x64;
extern const struct OamData gOamData_AffineDouble_ObjBlend_16x8;
extern const struct OamData gOamData_AffineDouble_ObjBlend_32x8;
extern const struct OamData gOamData_AffineDouble_ObjBlend_32x16;
extern const struct OamData gOamData_AffineDouble_ObjBlend_64x32;
extern const struct OamData gOamData_AffineDouble_ObjBlend_8x16;
extern const struct OamData gOamData_AffineDouble_ObjBlend_8x32;
extern const struct OamData gOamData_AffineDouble_ObjBlend_16x32;
extern const struct OamData gOamData_AffineDouble_ObjBlend_32x64;

extern const struct CompressedSpriteSheet gBattleAnimPicTable[];
extern const struct CompressedSpritePalette gBattleAnimPaletteTable[];

u8 GetBattlerForAnimScript(u8 animBattler);
void ChangeAnimBattlerSpriteVisibility(u8 animBattler, bool8 invisible);
void MoveBattlerSpriteToBG(u8 battlerId, bool8 toBG_2);
void ResetBattleAnimBg(bool8 to_BG2);
void ClearBattleAnimationVars(void);
void DoMoveAnim(u16 move);
void LaunchBattleAnimation(u8 animType, u16 animId);
void DestroyAnimSprite(struct Sprite *sprite);
void DestroyAnimVisualTask(u8 taskId);
void DestroyAnimSoundTask(u8 taskId);
s8 BattleAnimAdjustPanning(s8 pan);
s8 BattleAnimAdjustPanning2(s8 pan);
s16 CalculatePanIncrement(s16 sourcePan, s16 targetPan, s16 incrementPan);
bool8 IsBattlerSpriteVisible(u8 battlerId);
s16 KeepPanInRange(s16 a, s32 oldPan);
void RelocateBattleBgPal(u16 paletteNum, u16 *dest, s32 offset, u8 largeScreen);
void CreateItemBagSprite(const struct SpriteTemplate *template, s16 x, s16 y, u8 subpriority);
void Task_ClearMonBg(u8 taskId);
void LoadMoveBg(u16 bgId);

// battle_intro.c
void SetAnimBgAttribute(u8 bgId, u8 attributeId, u8 value);
s32 GetAnimBgAttribute(u8 bgId, u8 attributeId);
void HandleIntroSlide(u8 terrain);
void BattleIntroSlide1(u8 taskId);
void BattleIntroSlide2(u8 taskId);
void BattleIntroSlide3(u8 taskId);

// battle_anim_effects_1.c
extern const union AnimCmd *const gMusicNotesAnimTable[];
extern const union AnimCmd *const gEndureEnergyAnimTable[];
extern const struct SpriteTemplate gThoughtBubbleSpriteTemplate;

void AnimTask_ShrinkBattlerCopy(u8 taskId);

void AnimMovePowderParticle(struct Sprite *);
void AnimPowerAbsorptionOrb(struct Sprite *);
void AnimSolarbeamBigOrb(struct Sprite *);
void AnimSolarbeamSmallOrb(struct Sprite *);
void AnimAbsorptionOrb(struct Sprite *);
void AnimHyperBeamOrb(struct Sprite *);
void AnimRazorLeafParticle(struct Sprite *);
void AnimLeechSeed(struct Sprite *);
void AnimTranslateLinearSingleSineWave(struct Sprite *);
void AnimConstrictBinding(struct Sprite *);
void AnimMimicOrb(struct Sprite *);
void AnimPresent(struct Sprite *);
void AnimKnockOffItem(struct Sprite *);
void AnimPresentHealParticle(struct Sprite *);
void AnimItemSteal(struct Sprite *);
void AnimTrickBag(struct Sprite *);
void AnimFlyingParticle(struct Sprite *);
void AnimWhipHit(struct Sprite *);
void AnimCuttingSlice(struct Sprite *);
void AnimAirCutterSlice(struct Sprite *);
void AnimProtect(struct Sprite *);
void AnimMilkBottle(struct Sprite *);
void AnimGrantingStars(struct Sprite *);
void AnimSparkingStars(struct Sprite *);
void AnimLockOnTarget(struct Sprite *);
void AnimLockOnMoveTarget(struct Sprite *);
void AnimBowMon(struct Sprite *);
void AnimSlashSlice(struct Sprite *);
void AnimFalseSwipeSlice(struct Sprite *);
void AnimFalseSwipePositionedSlice(struct Sprite *);
void AnimEndureEnergy(struct Sprite *);
void AnimSharpenSphere(struct Sprite *);
void AnimConversion(struct Sprite *);
void AnimConversion2(struct Sprite *);
void AnimHornHit(struct Sprite *);
void AnimWavyMusicNotes(struct Sprite *);
void AnimFlyingMusicNotes(struct Sprite *);
void AnimBellyDrumHand(struct Sprite *);
void AnimSlowFlyingMusicNotes(struct Sprite *);
void AnimThoughtBubble(struct Sprite *);
void AnimMetronomeFinger(struct Sprite *);
void AnimFollowMeFinger(struct Sprite *);
void SetSpriteNextToMonHead(u8 battler, struct Sprite* sprite);

// battle_anim_effects_2.c
extern const union AffineAnimCmd *const gGrowingRingAffineAnimTable[];
extern const union AffineAnimCmd* const gAffineAnimTable_MaxFlutterby[];

void Anim_SwordsDanceBlade(struct Sprite *);
void AnimSonicBoomProjectile(struct Sprite *);
void AnimVoidLines(struct Sprite *);
void AnimAngerMark(struct Sprite *);
void AnimPencil(struct Sprite *);
void AnimBlendThinRing(struct Sprite *);
void AnimHyperVoiceRing(struct Sprite *);
void AnimUproarRing(struct Sprite *);
void AnimSoftBoiledEgg(struct Sprite *);
void AnimSpeedDust(struct Sprite *);
void AnimHealBellMusicNote(struct Sprite *);
void AnimRedHeartProjectile(struct Sprite *);
void AnimRedHeartRising(struct Sprite *);
void AnimOrbitFast(struct Sprite *);
void AnimOrbitScatter(struct Sprite *);
void AnimEyeSparkle(struct Sprite *);
void AnimFurySwipes(struct Sprite *);
void AnimMovementWaves(struct Sprite *);
void AnimJaggedMusicNote(struct Sprite *);
void AnimPerishSongMusicNote2(struct Sprite *);
void AnimPerishSongMusicNote(struct Sprite *);
void AnimGuardRing(struct Sprite *);
void AnimMaxFlutterby(struct Sprite *);

// battle_anim_effects_3.c
extern const struct SpriteTemplate gWishStarSpriteTemplate;
extern const struct SpriteTemplate gMiniTwinklingStarSpriteTemplate;

void AnimBlackSmoke(struct Sprite *);
void AnimWhiteHalo(struct Sprite *);
void AnimTealAlert(struct Sprite *);
void AnimMeanLookEye(struct Sprite *);
void AnimLeer(struct Sprite *);
void AnimLetterZ(struct Sprite *);
void AnimSpotlight(struct Sprite *);
void AnimClappingHand(struct Sprite *);
void AnimClappingHand2(struct Sprite *);
void AnimRapidSpin(struct Sprite *);
void AnimTriAttackTriangle(struct Sprite *);
void AnimWishStar(struct Sprite *);
void AnimMiniTwinklingStar(struct Sprite *);
void AnimSwallowBlueOrb(struct Sprite *);
void AnimGreenStar(struct Sprite *);
void AnimWeakFrustrationAngerMark(struct Sprite *);
void AnimSweetScentPetal(struct Sprite *);
void AnimPainSplitProjectile(struct Sprite *);
void AnimFlatterConfetti(struct Sprite *);
void AnimFlatterSpotlight(struct Sprite *);
void AnimReversalOrb(struct Sprite *);
void AnimYawnCloud(struct Sprite *);
void AnimSmokeBallEscapeCloud(struct Sprite *);
void AnimFacadeSweatDrop(struct Sprite *);
void AnimRoarNoiseLine(struct Sprite *);
void AnimGlareEyeDot(struct Sprite *);
void AnimAssistPawprint(struct Sprite *);
void AnimSmellingSaltsHand(struct Sprite *);
void AnimSmellingSaltExclamation(struct Sprite *);
void AnimHelpingHandClap(struct Sprite *);
void AnimForesightMagnifyingGlass(struct Sprite *);
void AnimMeteorMashStar(struct Sprite *);
void AnimBlockX(struct Sprite *);
void AnimKnockOffStrike(struct Sprite *);
void AnimRecycle(struct Sprite *);

// battle_anim_special.c
void TryShinyAnimation(u8 battler);
void IncrementBattleParticleCounter(void);
void DoLoadHealthboxPalsForLevelUp(u8 *, u8 *, u8 battlerId);
void DoFreeHealthboxPalsForLevelUp(u8 batterId);

enum
{
    BATTLER_COORD_X,
    BATTLER_COORD_Y,
    BATTLER_COORD_Y_PIC_OFFSET,
    BATTLER_COORD_Y_PIC_OFFSET_DEFAULT,
};

enum
{
    BATTLER_COORD_ATTR_HEIGHT,
    BATTLER_COORD_ATTR_WIDTH,
    BATTLER_COORD_ATTR_TOP,
    BATTLER_COORD_ATTR_BOTTOM,
    BATTLER_COORD_ATTR_LEFT,
    BATTLER_COORD_ATTR_RIGHT,
    BATTLER_COORD_ATTR_RAW_BOTTOM,
};

// battle_anim_status_effects.s
void LaunchStatusAnimation(u8 battlerId, u8 statusAnimId);

// battle_anim_mons.c
void AnimTask_BlendMonInAndOut(u8 task);
void AnimTask_BlendPalInAndOutByTag(u8 taskId);
void AnimTask_AttackerPunchWithTrace(u8 taskId);
void AnimTask_DestroyTaskAfterAffineAnimFromTaskDataEnds(u8 taskId);

u8 GetBattlerSpriteCoord(u8 battlerId, u8 coordType);
u8 GetBattlerSpriteCoord2(u8 battlerId, u8 coordType);
u8 GetBattlerSpriteDefault_Y(u8 battlerId);
u8 GetBattlerYCoordWithElevation(u8 battlerId);
u8 GetSubstituteSpriteDefault_Y(u8 battlerId);
u8 GetGhostSpriteDefault_Y(u8 battlerId);
u8 GetAnimBattlerSpriteId(u8 animBattler);
void StoreSpriteCallbackInData6(struct Sprite *sprite, SpriteCallback callback);
void TranslateSpriteInCircleOverDuration(struct Sprite *sprite);
void TranslateSpriteInGrowingCircleOverDuration(struct Sprite *sprite);
void TranslateSpriteInEllipseOverDuration(struct Sprite *sprite);
void WaitAnimForDuration(struct Sprite *sprite);
void SetupLinearTranslationWithFixedDuration(struct Sprite *sprite);
void TranslateSpriteLinear(struct Sprite *sprite);
void TranslateSpriteLinearFixedPoint(struct Sprite *sprite);
void TranslateMonSpriteLinear(struct Sprite *sprite);
void TranslateMonSpriteLinearFixedPoint(struct Sprite *sprite);
void TranslateSpriteLinearAndFlicker(struct Sprite *sprite);
void RunStoredCallbackWhenAffineAnimEnds(struct Sprite *sprite);
void RunStoredCallbackWhenAnimEnds(struct Sprite *sprite);
void DestroyAnimSpriteAndDisableBlend(struct Sprite *sprite);
void DestroyAnimVisualTaskAndDisableBlend(u8 taskId);
void SetSpriteCoordsToAnimAttackerCoords(struct Sprite *sprite);
void SetAnimSpriteInitialXOffset(struct Sprite *sprite, s16 xOffset);
void InitAnimArcTranslation(struct Sprite *sprite);
void DestroyAnimSpriteAfterHorizontalTranslation(struct Sprite *sprite);
bool8 TranslateAnimHorizontalArc(struct Sprite *sprite);
bool8 TranslateAnimVerticalArc(struct Sprite *sprite);
void SetSpritePrimaryCoordsFromSecondaryCoords(struct Sprite *sprite);
void InitSpritePosToAnimTarget(struct Sprite *sprite, bool8 respectMonPicOffsets);
void InitSpritePosToAnimAttacker(struct Sprite *sprite, bool8 respectMonPicOffsets);
void InitSpritePosToAnimBattler(struct Sprite *sprite, u8 battler, bool8 respectMonPicOffsets);
u8 GetBattlerAtPosition(u8 position);
bool8 IsBattlerSpritePresent(u8 battlerId);
void GetBattleAnimBgData(struct BattleAnimBgData *animBgData, u32 bgId);
void GetBattleAnimBgDataByPriorityRank(struct BattleAnimBgData *animBgData);
void InitBattleAnimBg(u32 bgId);
void AnimLoadCompressedBgGfx(u32 bgId, const u32 *src, u32 tilesOffset);
void InitAnimBgTilemapBuffer(u32 bgId, const void *src);
void AnimLoadCompressedBgTilemap(u32 bgId, const u32 *src);
void ToggleBg3Mode(bool8 arg0);
void StartSpriteLinearTranslationFromCurrentPos(struct Sprite *sprite);
void InitSpriteDataForLinearTranslation(struct Sprite *sprite);
void InitAnimLinearTranslation(struct Sprite *sprite);
void StartAnimLinearTranslation(struct Sprite *sprite);
void PlayerThrowBall_StartAnimLinearTranslation(struct Sprite *sprite);
bool8 AnimTranslateLinear(struct Sprite *sprite);
void RunLinearTranslation_ThenceSetCBtoStoredInData6(struct Sprite *sprite);
void BattleAnim_InitLinearTranslationWithDuration(struct Sprite *sprite);
void BattleAnim_InitAndRunLinearTranslationWithDuration(struct Sprite *sprite);
void InitAndRunAnimFastLinearTranslation(struct Sprite *sprite);
bool8 AnimFastTranslateLinear(struct Sprite *sprite);
void InitAnimFastLinearTranslationWithSpeed(struct Sprite *sprite);
void InitAndStartAnimFastLinearTranslationWithSpeed(struct Sprite *sprite);
void DestroyAnimSpriteWhenLinearTranslationEnds(struct Sprite *sprite);
void SetSpriteRotScale(u8 spriteId, s16 xScale, s16 yScale, u16 rotation);
void PrepareBattlerSpriteForRotScale(u8 spriteId, u8 objMode);
void ResetSpriteRotScale(u8 spriteId);
void SetBattlerSpriteYOffsetFromRotation(u8 spriteId);
void TrySetSpriteRotScale(struct Sprite *sprite, bool8 recalcCenterVector, s16 xScale, s16 yScale, u16 rotation);
void TryResetSpriteAffineState(struct Sprite *sprite);
u16 ArcTan2Neg(s16 a, s16 b);
void SetGreyscaleOrOriginalPalette(u16 paletteNum, bool8 restoreOriginalColor);
u32 SelectBattleAnimSpriteAndBgPalettes(bool8 battleBackground, bool8 attacker, bool8 target, bool8 attackerPartner, bool8 targetPartner, bool8 a6, bool8 a7);
u32 SelectBattlerSpritePalettes(bool8 playerLeft, bool8 playerRight, bool8 foeLeft, bool8 foeRight);
void AnimSpriteOnMonPos(struct Sprite *sprite);
void TranslateAnimSpriteToTargetMonLocation(struct Sprite *sprite);
void AnimThrowProjectile(struct Sprite *sprite);
void AnimTravelDiagonally(struct Sprite *sprite);
s16 CloneBattlerSpriteWithBlend(u8 animBattler);
void DestroySpriteWithActiveSheet(struct Sprite *sprite);
void PrepareAffineAnimInTaskData(struct Task *task, u8 spriteId, const union AffineAnimCmd *affineAnimCmds);
bool8 RunAffineAnimFromTaskData(struct Task *task);
void SetBattlerSpriteYOffsetFromYScale(u8 spriteId);
void SetBattlerSpriteYOffsetFromOtherYScale(u8 spriteId, u8 otherSpriteId);
void StorePointerInVars(s16 *lo, s16 *hi, const void *ptr);
void *LoadPointerFromVars(s16 lo, s16 hi);
void BattleAnimHelper_SetSpriteSquashParams(struct Task *task, u8 spriteId, s16 xScale0, s16 yScale0, s16 xScale1, s16 yScale1, u16 duration);
u8 BattleAnimHelper_RunSpriteSquash(struct Task *task);
void ResetSpritePriorityOfAllVisibleBattlers(void);
u8 GetBattlerSpriteSubpriority(u8 battlerId);
u8 GetBattlerSpriteBGPriority(u8 battlerId);
void DestroyAnimSpriteWhenAnimEnds(struct Sprite *sprite);
void DestroyAnimSpriteWhenAffineAnimEnds(struct Sprite *sprite);

// Returns 2 if player left or opp right
// Returns 1 if player right or opp left
u8 GetBattlerSpriteBGPriorityRank(u8 battlerId);
u8 CreateAdditionalMonSpriteForMoveAnim(u16 species, bool8 isBackpic, s16 x, s16 y, u8 subpriority, u32 personality, bool8 isShiny, bool8 transformPalFade);
s16 GetBattlerSpriteCoordAttr(u8 battlerId, u8 attr);
void SetAverageBattlerPositions(u8 battlerId, bool8 respectMonPicOffsets, s16 *x, s16 *y);
u8 CreateCloneOfSpriteInWindowMode(u8 spriteId, s32 species);
void SpriteCB_TrackOffsetFromAttackerAndWaitAnim(struct Sprite *sprite);

// battle_anim_mon_movement.c
void AnimTask_ShakeMon(u8 taskId);
void AnimTask_ShakeMon2(u8 taskId);
void AnimTask_ShakeMonInPlace(u8 taskId);
void AnimTask_ShakeAndSinkMon(u8 taskId);
void AnimTask_TranslateMonElliptical(u8 taskId);
void AnimTask_TranslateMonEllipticalRespectSide(u8 taskId);
void AnimTask_WindUpLunge(u8 taskId);
void AnimTask_SlideOffScreen(u8 taskId);
void AnimTask_SwayMon(u8 taskId);
void AnimTask_ScaleMonAndRestore(u8 taskId);
void AnimTask_RotateMonSpriteToSide(u8 taskId);
void AnimTask_RotateMonToSideAndRestore(u8 taskId);
void AnimTask_ShakeTargetBasedOnMovePowerOrDmg(u8 taskId);

// normal.c
extern const struct SpriteTemplate gFlashingHitSplatSpriteTemplate;
extern const struct SpriteTemplate gBasicHitSplatSpriteTemplate;
extern const struct SpriteTemplate gWaterHitSplatSpriteTemplate;

void AnimTask_BlendColorCycle(u8 taskId);
void AnimTask_BlendColorCycleExclude(u8 taskId);
void AnimTask_BlendColorCycleByTag(u8 taskId);
void AnimTask_FlashAnimTagWithColor(u8 taskId);
void AnimTask_InvertScreenColor(u8 taskId);
void AnimTask_ShakeBattleTerrain(u8 taskId);

u32 UnpackSelectedBattleAnimPalettes(s16 selector);
void SpriteCB_WeatherBallUp(struct Sprite *sprite);
void AnimWeatherBallDown(struct Sprite *sprite);

// ground.c
void AnimTask_DigDownMovement(u8 taskId);
void AnimTask_DigUpMovement(u8 taskId);
void AnimTask_HorizontalShake(u8 taskId);
void AnimTask_IsPowerOver99(u8 taskId);
void AnimTask_PositionFissureBgOnBattler(u8 taskId);

void AnimSpikes(struct Sprite *);
void AnimDirtScatter(struct Sprite *sprite);

// dragon.c
void AnimTask_DragonDanceWaver(u8 taskId);

void AnimOutrageFlame(struct Sprite *sprite);

// ghost.c
extern const union AffineAnimCmd *const gAffineAnims_ShadowBall[];

void AnimTask_NightShadeClone(u8 taskId);
void AnimTask_NightmareClone(u8 taskId);
void AnimTask_SpiteTargetShadow(u8 taskId);
void AnimTask_DestinyBondWhiteShadow(u8 taskId);
void AnimTask_CurseStretchingBlackBg(u8 taskId);
void AnimTask_GrudgeFlames(u8 taskId);
void AnimTask_GhostGetOutAttackerEffect(u8 taskId);

void AnimShadowBall(struct Sprite *sprite);

// rock.c
void AnimTask_LoadSandstormBackground(u8 taskId);
void AnimTask_Rollout(u8 taskId);
void AnimTask_GetSeismicTossDamageLevel(u8 taskId);
void AnimTask_MoveSeismicTossBg(u8 taskId);
void AnimTask_SeismicTossBgAccelerateDownAtEnd(u8 taskId);

void AnimParticleInVortex(struct Sprite *sprite);
void AnimFallingRock(struct Sprite *sprite);
void AnimRockFragment(struct Sprite *sprite);

// psychic.c
extern const union AffineAnimCmd *const gAffineAnims_LusterPurgeCircle[];

void AnimTask_MeditateStretchAttacker(u8 taskId);
void AnimTask_Teleport(u8 taskId);
void AnimTask_ImprisonOrbs(u8 taskId);
void AnimTask_SkillSwap(u8 taskId);
void AnimTask_ExtrasensoryDistortion(u8 taskId);
void AnimTask_TransparentCloneGrowAndShrink(u8 taskId);

void Anim_KinesisZapEnergy(struct Sprite *);

// dark.c
extern const union AffineAnimCmd *const gAffineAnims_Bite[];

void AnimTask_AttackerFadeToInvisible(u8 taskId);
void AnimTask_AttackerFadeFromInvisible(u8 taskId);
void AnimTask_InitAttackerFadeFromInvisible(u8 taskId);
void AnimTask_MoveAttackerMementoShadow(u8 taskId);
void AnimTask_MoveTargetMementoShadow(u8 taskId);
void AnimTask_InitMementoShadow(u8 taskId);
void AnimTask_EndMementoShadow(u8 taskId);
void AnimTask_SetGreyscaleOrOriginalPal(u8 taskId);

void AnimMoveSpriteOverDuration(struct Sprite *sprite);
void AnimMoveSpriteOverDurationFast(struct Sprite *sprite);
void AnimBite(struct Sprite *sprite);

// flying.c
void AnimTask_AnimateGustTornadoPalette(u8 taskId);
void AnimTask_DrillPeckHitSplats(u8 taskId);

void AnimSkyAttackBird(struct Sprite *sprite);
void DestroyAnimSpriteAfterTimer(struct Sprite *sprite);

// steel.c
void AnimTask_MetallicShine(u8 taskId);

// poison.c
extern const union AffineAnimCmd *const gAffineAnims_Droplet[];

// fighting.c
void AnimTask_MoveSkyUppercutBg(u8 taskId);

void AnimDizzyPunchDuck(struct Sprite *sprite);
void AnimSpriteOnMonForDuration(struct Sprite *sprite);
void AnimStompFoot(struct Sprite *sprite);
void AnimSlidingKick(struct Sprite *sprite);
void AnimSuperpowerFireball(struct Sprite *sprite);
bool8 GetSpriteOrientationBasedOnBattlers(void);

// ice.c
extern const union AnimCmd *const gAnims_SmallBubblePair[];
extern const union AnimCmd *const gAnims_Cloud[];

void AnimTask_Haze1(u8 taskId);
void AnimTask_LoadMistTiles(u8 taskId);
void AnimTask_Hail1(u8 taskId);
void AnimTask_GetRolloutCounter(u8 taskId);

void InitSwirlingFogAnim(struct Sprite *sprite);

// bug.c
void AnimStringWrap(struct Sprite *sprite);
void AnimMissileArc(struct Sprite *sprite);
void AnimMissileArcStep(struct Sprite *sprite);

// electric.c
void AnimTask_ElectricBolt(u8 taskId);
void AnimTask_ElectricChargingParticles(u8 taskId);
void AnimTask_VoltTackleAttackerReappear(u8 taskId);
void AnimTask_VoltTackleBolt(u8 taskId);
void AnimTask_ShockWaveProgressingBolt(u8 taskId);
void AnimTask_ShockWaveLightning(u8 taskId);

// fire.c
extern const union AnimCmd *const gAnims_WillOWispOrb[];

void AnimTask_EruptionLaunchRocks(u8 taskId);
void AnimTask_ShakeTargetInPattern(u8 taskId);
void AnimTask_BlendBackground(u8 taskId);
void AnimTask_MoveHeatWaveTargets(u8 taskId);

void AnimFireSpiralInward(struct Sprite *sprite);
void UpdateFireRingCircleOffset(struct Sprite *sprite, s16 addrX, s16 addrY, s8 speed);
void AnimFireSpiralOutward(struct Sprite *sprite);

// water.c
extern const union AffineAnimCmd *const gAffineAnims_Bubble[];

void AnimWaterPulseRing(struct Sprite *sprite);
void AnimToTargetInSinWave(struct Sprite *sprite);
void AnimBubbleEffect(struct Sprite *sprite);

// grass.c
extern const union AffineAnimCmd *const gBulletSeedAffineAnimTable[];

void AnimBulletSeed(struct Sprite *);
void AnimPetalDanceFlower(struct Sprite *);
void AnimMoveTwisterParticle(struct Sprite *);
void AnimNeedleArmSpike(struct Sprite *);

// battle_anim_utility_funcs.c
void AnimTask_BlendSelected(u8 taskId);
void AnimTask_BlendExcept(u8 taskId);
void AnimTask_SetCamouflageBlend(u8 taskId);
void AnimTask_BlendParticle(u8 taskId);
void AnimTask_HardwarePaletteFade(u8 taskId);
void AnimTask_CloneBattlerSpriteWithBlend(u8 taskId);
void InitStatsChangeAnimation(u8 taskId);
void AnimTask_BlendNonAttackerPalettes(u8 taskId);
void AnimTask_StartSlidingBg(u8 taskId);
void AnimTask_GetTargetIsAttackerPartner(u8 taskId);
void AnimTask_SetAllNonAttackersInvisiblity(u8 taskId);
void AnimTask_GetBattleTerrain(u8 taskId);
void AnimTask_AllocBackupPalBuffer(u8 taskId);
void AnimTask_FreeBackupPalBuffer(u8 taskId);
void AnimTask_CopyPalUnfadedToBackup(u8 taskId);
void AnimTask_CopyPalUnfadedFromBackup(u8 taskId);
void AnimTask_CopyPalFadedToUnfaded(u8 taskId);
void AnimTask_SetAnimAttackerAndTargetForEffectTgt(u8 taskId);
void AnimTask_SetAnimTargetFromArg(u8 taskId);
void AnimTask_SetAttackerInvisibleWaitForSignal(u8 taskId);

void StartMonScrollingBgMask(u8 taskId, u16 yMovementSpeed, u8 battler1, bool8 bothMons, u8 targetBlendCoeff, u8 blendDelay, u8 duration, const u32 *gfx, const u32 *tilemap, const u32 *palette);

extern const struct UCoords8 gBattlerCoords[][MAX_BATTLERS_COUNT];

extern const struct BattleAnimTable gBattleAnims_General[];

#endif // GUARD_BATTLE_ANIM_H

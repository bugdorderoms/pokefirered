#include "global.h"
#include "battle_anim.h"
#include "sound.h"
#include "palette.h"
#include "task.h"
#include "trig.h"
#include "constants/songs.h"

static void AnimFireSpread(struct Sprite *sprite);
static void AnimLargeFlame(struct Sprite *sprite);
static void AnimLargeFlame_Step(struct Sprite *sprite);
static void AnimFirePlume(struct Sprite *sprite);
static void AnimSunlightRay(struct Sprite *sprite);
static void AnimEmberFlare(struct Sprite *sprite);
static void AnimFireRing(struct Sprite *sprite);
static void AnimFireCross(struct Sprite *sprite);
static void AnimFireSpiralOutward_Step(struct Sprite *sprite);
static void AnimFireSpiralOutward_Step2(struct Sprite *sprite);
static void AnimEruptionLaunchRock(struct Sprite *sprite);
static void AnimEruptionFallingRock(struct Sprite *sprite);
static void AnimEruptionFallingRock_Step(struct Sprite *sprite);
static void AnimWillOWispOrb(struct Sprite *sprite);
static void AnimWillOWispOrb_Step(struct Sprite *sprite);
static void AnimWillOWispFire(struct Sprite *sprite);
static void AnimFireRing_Step1(struct Sprite *sprite);
static void AnimFireRing_Step2(struct Sprite *sprite);
static void AnimFireRing_Step3(struct Sprite *sprite);
static void AnimTask_EruptionLaunchRocks_Step(u8 taskId);
static void CreateEruptionLaunchRocks(u8 spriteId, u8 taskId, u8 a3);
static u16 GetEruptionLaunchRockInitialYPos(u8 spriteId);
static void InitEruptionLaunchRockCoordData(struct Sprite *sprite, s16 x, s16 y);
static void UpdateEruptionLaunchRockPos(struct Sprite *sprite);
static void AnimTask_MoveHeatWaveTargets_Step(u8 taskId);
static void AnimMoveSpriteUpwardsForDuration(struct Sprite *sprite);
static void AnimMoveSpriteUpwardsForDuration_Step(struct Sprite *sprite);

static const union AnimCmd sAnim_FireSpiralSpread_0[] =
{
    ANIMCMD_FRAME(16, 4),
    ANIMCMD_FRAME(32, 4),
    ANIMCMD_FRAME(48, 4),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_FireSpiralSpread_1[] =
{
    ANIMCMD_FRAME(16, 4, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 4, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_FRAME(48, 4, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnims_FireSpiralSpread[] =
{
    sAnim_FireSpiralSpread_0,
    sAnim_FireSpiralSpread_1,
};

const struct SpriteTemplate gFireSpiralInwardSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_EMBER,
    .paletteTag = ANIM_TAG_SMALL_EMBER,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sAnims_FireSpiralSpread,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimFireSpiralInward,
};

const struct SpriteTemplate gFireSpreadSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_EMBER,
    .paletteTag = ANIM_TAG_SMALL_EMBER,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sAnims_FireSpiralSpread,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimFireSpread,
};

static const union AnimCmd sAnim_LargeFlame[] =
{
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_FRAME(16, 3),
    ANIMCMD_FRAME(32, 3),
    ANIMCMD_FRAME(48, 3),
    ANIMCMD_FRAME(64, 3),
    ANIMCMD_FRAME(80, 3),
    ANIMCMD_FRAME(96, 3),
    ANIMCMD_FRAME(112, 3),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnims_LargeFlame[] =
{
    sAnim_LargeFlame,
};

static const union AnimCmd sAnim_FirePlume[] =
{
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_FRAME(16, 5),
    ANIMCMD_FRAME(32, 5),
    ANIMCMD_FRAME(48, 5),
    ANIMCMD_FRAME(64, 5),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnims_FirePlume[] =
{
    sAnim_FirePlume,
};

static const union AffineAnimCmd sAffineAnim_LargeFlame[] =
{
    AFFINEANIMCMD_FRAME(0x32, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME(0x20, 0x0, 0, 7),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims_LargeFlame[] =
{
    sAffineAnim_LargeFlame,
};

const struct SpriteTemplate gLargeFlameSpriteTemplate =
{
    .tileTag = ANIM_TAG_FIRE,
    .paletteTag = ANIM_TAG_FIRE,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = sAnims_LargeFlame,
    .images = NULL,
    .affineAnims = sAffineAnims_LargeFlame,
    .callback = AnimLargeFlame,
};

const struct SpriteTemplate gLargeFlameScatterSpriteTemplate =
{
    .tileTag = ANIM_TAG_FIRE,
    .paletteTag = ANIM_TAG_FIRE,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sAnims_LargeFlame,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimLargeFlame,
};

const struct SpriteTemplate gFirePlumeSpriteTemplate =
{
    .tileTag = ANIM_TAG_FIRE_PLUME,
    .paletteTag = ANIM_TAG_FIRE_PLUME,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sAnims_FirePlume,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimFirePlume,
};

static const union AffineAnimCmd sAffineAnim_SunlightRay[] =
{
    AFFINEANIMCMD_FRAME(0x50, 0x50, 0, 0),
    AFFINEANIMCMD_FRAME(0x2, 0x2, 10, 1),
    AFFINEANIMCMD_JUMP(1),
};

static const union AffineAnimCmd *const sAffineAnims_SunlightRay[] =
{
    sAffineAnim_SunlightRay,
};

const struct SpriteTemplate gSunlightRaySpriteTemplate =
{
    .tileTag = ANIM_TAG_SUNLIGHT,
    .paletteTag = ANIM_TAG_SUNLIGHT,
    .oam = &gOamData_AffineNormal_ObjBlend_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_SunlightRay,
    .callback = AnimSunlightRay,
};

static const union AnimCmd sAnim_BasicFire[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(16, 4),
    ANIMCMD_FRAME(32, 4),
    ANIMCMD_FRAME(48, 4),
    ANIMCMD_FRAME(64, 4),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnims_BasicFire[] =
{
    sAnim_BasicFire,
};

const struct SpriteTemplate gEmberSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_EMBER,
    .paletteTag = ANIM_TAG_SMALL_EMBER,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = TranslateAnimSpriteToTargetMonLocation,
};

const struct SpriteTemplate gEmberFlareSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_EMBER,
    .paletteTag = ANIM_TAG_SMALL_EMBER,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sAnims_BasicFire,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimEmberFlare,
};

const struct SpriteTemplate gBurnFlameSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_EMBER,
    .paletteTag = ANIM_TAG_SMALL_EMBER,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sAnims_BasicFire,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimTravelDiagonally,
};

const struct SpriteTemplate gFireBlastRingSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_EMBER,
    .paletteTag = ANIM_TAG_SMALL_EMBER,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sAnims_BasicFire,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimFireRing,
};

static const union AnimCmd sAnim_FireBlastCross[] =
{
    ANIMCMD_FRAME(32, 6),
    ANIMCMD_FRAME(48, 6),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnims_FireBlastCross[] =
{
    sAnim_FireBlastCross,
};

const struct SpriteTemplate gFireBlastCrossSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_EMBER,
    .paletteTag = ANIM_TAG_SMALL_EMBER,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sAnims_FireBlastCross,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimFireCross,
};

const struct SpriteTemplate gFireSpiralOutwardSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_EMBER,
    .paletteTag = ANIM_TAG_SMALL_EMBER,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sAnims_BasicFire,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimFireSpiralOutward,
};

const struct SpriteTemplate gWeatherBallFireDownSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_EMBER,
    .paletteTag = ANIM_TAG_SMALL_EMBER,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sAnims_BasicFire,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimWeatherBallDown,
};

static const struct SpriteTemplate gEruptionLaunchRockSpriteTemplate =
{
    .tileTag = ANIM_TAG_WARM_ROCK,
    .paletteTag = ANIM_TAG_WARM_ROCK,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimEruptionLaunchRock,
};

static const s16 sEruptionLaunchRockCoords[][2] =
{
    {-2, -5},
    {-1, -1},
    { 3, -6},
    { 4, -2},
    { 2, -8},
    {-5, -5},
    { 4, -7},
};

const struct SpriteTemplate gEruptionFallingRockSpriteTemplate =
{
    .tileTag = ANIM_TAG_WARM_ROCK,
    .paletteTag = ANIM_TAG_WARM_ROCK,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimEruptionFallingRock,
};

static const union AnimCmd sAnim_WillOWispOrb_0[] =
{
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_FRAME(4, 5),
    ANIMCMD_FRAME(8, 5),
    ANIMCMD_FRAME(12, 5),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_WillOWispOrb_1[] =
{
    ANIMCMD_FRAME(16, 5),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_WillOWispOrb_2[] =
{
    ANIMCMD_FRAME(20, 5),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_WillOWispOrb_3[] =
{
    ANIMCMD_FRAME(20, 5),
    ANIMCMD_END,
};

const union AnimCmd *const gAnims_WillOWispOrb[] =
{
    sAnim_WillOWispOrb_0,
    sAnim_WillOWispOrb_1,
    sAnim_WillOWispOrb_2,
    sAnim_WillOWispOrb_3,
};

const struct SpriteTemplate gWillOWispOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_WISP_ORB,
    .paletteTag = ANIM_TAG_WISP_ORB,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gAnims_WillOWispOrb,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimWillOWispOrb,
};

static const union AnimCmd sAnim_WillOWispFire[] =
{
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_FRAME(16, 5),
    ANIMCMD_FRAME(32, 5),
    ANIMCMD_FRAME(48, 5),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnims_WillOWispFire[] =
{
    sAnim_WillOWispFire,
};

const struct SpriteTemplate gWillOWispFireSpriteTemplate =
{
    .tileTag = ANIM_TAG_WISP_FIRE,
    .paletteTag = ANIM_TAG_WISP_FIRE,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sAnims_WillOWispFire,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimWillOWispFire,
};

const struct SpriteTemplate gFireSpinSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_EMBER,
    .paletteTag = ANIM_TAG_SMALL_EMBER,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sAnims_BasicFire,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimParticleInVortex,
};

const struct SpriteTemplate gFlameBuffSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_EMBER,
    .paletteTag = ANIM_TAG_SMALL_EMBER,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimMoveSpriteUpwardsForDuration,
};

// Directions for shaking up/down or left/right in AnimTask_ShakeTargetInPattern
// First pattern results in larger shakes, second results in faster oscillation
static const s8 sShakeDirsPattern0[10] =
{
    -1, -1, 0, 1, 1, 0, 0, -1, -1, 1,
};

static const s8 sShakeDirsPattern1[10] =
{
    -1, 0, 1, 0, -1, 1, 0, -1, 0, 1,
};

const struct SpriteTemplate gPowerGemBladeSpriteTemplate =
{
    .tileTag = ANIM_TAG_PUNISHMENT_BLADES,
    .paletteTag = ANIM_TAG_AIR_WAVE_2,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = sAnims_FireSpiralSpread,
    .images = NULL,
    .affineAnims = gAffineAnims_ShadowBall,
    .callback = AnimFireSpread,
};

// For the first stage of Fire Punch and Ice Punch
// arg 0: initial position angle around circle (0-256)
void AnimFireSpiralInward(struct Sprite *sprite)
{
    sprite->data[0] = gBattleAnimArgs[0];
    sprite->data[1] = 60;
    sprite->data[2] = 9;
    sprite->data[3] = 30;
    sprite->data[4] = -512;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
    sprite->callback = TranslateSpriteInGrowingCircleOverDuration;
    sprite->callback(sprite);
}

// For the impact spread of fire sprites for moves like Blaze Kick or Fire Punch.
// arg 0: x offset
// arg 1: y offset
// arg 2: x pixel slide offset
// arg 3: y pixel slide offset
// arg 4: duration
static void AnimFireSpread(struct Sprite *sprite)
{
    SetAnimSpriteInitialXOffset(sprite, gBattleAnimArgs[0]);
	
    sprite->y += gBattleAnimArgs[1];
	
    sprite->data[0] = gBattleAnimArgs[4];
    sprite->data[1] = gBattleAnimArgs[2];
    sprite->data[2] = gBattleAnimArgs[3];
    sprite->callback = TranslateSpriteLinearFixedPoint;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Animates a fire plume in the attacker. Used by moves like Sacred Fire.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: duration
// arg 3: movement delay
// arg 4: x slide amount
// arg 5: y slide amount
static void AnimFirePlume(struct Sprite *sprite)
{
    SetSpriteCoordsToAnimAttackerCoords(sprite);
	
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
    {
        sprite->x -= gBattleAnimArgs[0];
        gBattleAnimArgs[4] = -gBattleAnimArgs[4];
    }
    else
        sprite->x += gBattleAnimArgs[0];

	sprite->y += gBattleAnimArgs[1];
	
    sprite->data[1] = gBattleAnimArgs[2];
	sprite->data[2] = gBattleAnimArgs[4];
	sprite->data[3] = gBattleAnimArgs[5];
    sprite->data[4] = gBattleAnimArgs[3];
    sprite->callback = AnimLargeFlame_Step;
}

// Animates the MOVE_SACRED_FIRE large flames in the target.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: duration
// arg 3: movement delay
// arg 4: x slide amount
// arg 5: y slide amount
static void AnimLargeFlame(struct Sprite *sprite)
{
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        sprite->x -= gBattleAnimArgs[0];
    else
    {
        sprite->x += gBattleAnimArgs[0];
        gBattleAnimArgs[4] = -gBattleAnimArgs[4];
    }
	sprite->y += gBattleAnimArgs[1];
	
    sprite->data[1] = gBattleAnimArgs[2];
	sprite->data[2] = gBattleAnimArgs[4];
	sprite->data[3] = gBattleAnimArgs[5];
    sprite->data[4] = gBattleAnimArgs[3];
    sprite->callback = AnimLargeFlame_Step;
}

static void AnimLargeFlame_Step(struct Sprite *sprite)
{
    if (++sprite->data[0] < sprite->data[4])
    {
        sprite->x2 += sprite->data[2];
        sprite->y2 += sprite->data[3];
    }
	
    if (sprite->data[0] == sprite->data[1])
        DestroyAnimSprite(sprite);
}

// Animates MOVE_SUNNY_DAY's sunlight ray sprite.
// No args.
static void AnimSunlightRay(struct Sprite *sprite)
{
    sprite->x = 0;
    sprite->y = 0;
    sprite->data[0] = 60;
    sprite->data[2] = 140;
    sprite->data[4] = 80;
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

//fire 2

// Animates the secondary effect of MOVE_EMBER, where the flames grow and slide horizontally a bit.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: final x pixel offset
// arg 3: final y pixel offset
// arg 4: duration
// arg 5: anim battler
// arg 6: ? (TODO: something related to which mon the pixel offsets are based on) (boolean)
static void AnimEmberFlare(struct Sprite *sprite)
{
    if (IsBattlerAlly(gBattleAnimAttacker, gBattleAnimTarget) && (GetBattlerPosition(gBattleAnimAttacker) & BIT_FLANK) != B_FLANK_RIGHT)
		gBattleAnimArgs[2] = -gBattleAnimArgs[2];
	
    sprite->callback = AnimTravelDiagonally;
    sprite->callback(sprite);
}

// Animates the sprite in the first-half of the MOVE_FIRE_BLAST animation. The fire sprite first moves in a circle around the mon,
// and then it is translated towards the target mon, while still rotating. Lastly, it moves in a circle around the target mon.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: initial wave offset
static void AnimFireRing(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, TRUE);
    sprite->data[7] = gBattleAnimArgs[2];
    sprite->callback = AnimFireRing_Step1;
}

void UpdateFireRingCircleOffset(struct Sprite *sprite, s16 addrX, s16 addrY, s8 speed)
{
    sprite->x2 = addrX + Sin(sprite->data[7], 28);
    sprite->y2 = addrY + Cos(sprite->data[7], 28);
    sprite->data[7] = (sprite->data[7] + speed) & 0xFF;
}

static void AnimFireRing_Step1(struct Sprite *sprite)
{   
    UpdateFireRingCircleOffset(sprite, 0, 0, 20);

    if (++sprite->data[0] == 0x12)
    {
        sprite->data[0] = 0x19;
        sprite->data[1] = sprite->x;
        sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
        sprite->data[3] = sprite->y;
        sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
        InitAnimLinearTranslation(sprite);
        sprite->callback = AnimFireRing_Step2;
    }
}

static void AnimFireRing_Step2(struct Sprite *sprite)
{
    if (AnimTranslateLinear(sprite))
    {
        sprite->data[0] = 0;

        sprite->x = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
        sprite->y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
		
        sprite->x2 = 0;
		sprite->y2 = 0;
		
        sprite->callback = AnimFireRing_Step3;
        sprite->callback(sprite);
    }
    else
		UpdateFireRingCircleOffset(sprite, sprite->x2, sprite->y2, 20);
}

static void AnimFireRing_Step3(struct Sprite *sprite)
{
    UpdateFireRingCircleOffset(sprite, 0, 0, 20);
	
    if (++sprite->data[0] == 0x1F)
        DestroyAnimSprite(sprite);
}

// Animates the crossing flames in end of MOVE_FIRE_BLAST anim.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: duration
// arg 3: x delta
// arg 4: y delta 
static void AnimFireCross(struct Sprite *sprite)
{
    sprite->x += gBattleAnimArgs[0];
    sprite->y += gBattleAnimArgs[1];
	
    sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[1] = gBattleAnimArgs[3];
    sprite->data[2] = gBattleAnimArgs[4];
	sprite->callback = TranslateSpriteLinear;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Animates MOVE_FLAME_WHEEL's fire spiral.
// Arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: duration
// arg 3: initial wait duration
void AnimFireSpiralOutward(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, TRUE);
	
    sprite->data[1] = gBattleAnimArgs[2];
    sprite->data[0] = gBattleAnimArgs[3];
    sprite->invisible = TRUE;
    sprite->callback = WaitAnimForDuration;
    StoreSpriteCallbackInData6(sprite, AnimFireSpiralOutward_Step);
}

static void AnimFireSpiralOutward_Step(struct Sprite *sprite)
{
    sprite->invisible = FALSE;
    sprite->data[0] = sprite->data[1];
    sprite->data[1] = 0;
    sprite->callback = AnimFireSpiralOutward_Step2;
    sprite->callback(sprite);
}

static void AnimFireSpiralOutward_Step2(struct Sprite *sprite)
{
    sprite->x2 = Sin(sprite->data[1], sprite->data[2] >> 8);
    sprite->y2 = Cos(sprite->data[1], sprite->data[2] >> 8);
	
    sprite->data[1] = (sprite->data[1] + 10) & 0xFF;
    sprite->data[2] += 0xD0;
	
    if (--sprite->data[0] == -1)
        DestroyAnimSprite(sprite);
}

// Animates the attacker's launching rocks in the MOVE_ERUPTION's anim.
// No args.
void AnimTask_EruptionLaunchRocks(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    task->data[15] = GetAnimBattlerSpriteId(ANIM_ATTACKER);
    task->data[0] = 0;
    task->data[1] = 0;
    task->data[2] = 0;
    task->data[3] = 0;
    task->data[4] = gSprites[task->data[15]].y;
    task->data[5] = GetBattlerSide(gBattleAnimAttacker);
    task->data[6] = 0;
    PrepareBattlerSpriteForRotScale(task->data[15], ST_OAM_OBJ_NORMAL);
    task->func = AnimTask_EruptionLaunchRocks_Step;
}

static void AnimTask_EruptionLaunchRocks_Step(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        BattleAnimHelper_SetSpriteSquashParams(task, task->data[15], 0x100, 0x100, 0xE0, 0x200, 0x20);
        ++task->data[0];
        // fall through
    case 1:
        if (++task->data[1] > 1)
        {
            task->data[1] = 0;
			
            if (++task->data[2] & 0x1)
                gSprites[task->data[15]].x2 = 3;
            else
                gSprites[task->data[15]].x2 = -3;
        }
		
        if (task->data[5] == B_SIDE_OPPONENT)
        {
            if (++task->data[3] > 4)
            {
                task->data[3] = 0;
                ++gSprites[task->data[15]].y;
            }
        }
		
        if(!BattleAnimHelper_RunSpriteSquash(task))
        {
            SetBattlerSpriteYOffsetFromYScale(task->data[15]);
            gSprites[task->data[15]].x2 = 0;
            task->data[1] = 0;
            task->data[2] = 0;
            task->data[3] = 0;
            ++task->data[0];
        }
        break;
    case 2:
        if (++task->data[1] > 4)
        {
			BattleAnimHelper_SetSpriteSquashParams(task, task->data[15], 0xE0, 0x200, 0x180, task->data[5] == B_SIDE_OPPONENT ? 0xF0 : 0xC0, 0x6);
            task->data[1] = 0;
            ++task->data[0];
        }
        break;
    case 3:
        if (!BattleAnimHelper_RunSpriteSquash(task))
        {
            CreateEruptionLaunchRocks(task->data[15], taskId, 6);
            ++task->data[0];
        }
        break;
    case 4:
        if (++task->data[1] > 1)
        {
            task->data[1] = 0;
			
            if (++task->data[2] & 1)
                gSprites[task->data[15]].y2 += 3;
            else
                gSprites[task->data[15]].y2 -= 3;
        }
		
        if (++task->data[3] > 0x18)
        {
			BattleAnimHelper_SetSpriteSquashParams(task, task->data[15], 0x180, task->data[5] == B_SIDE_OPPONENT ? 0xF0 : 0xC0, 0x100, 0x100, 0x8);
			
            if (task->data[2] & 1)
                gSprites[task->data[15]].y2 -= 3;
			
            task->data[1] = 0;
            task->data[2] = 0;
            task->data[3] = 0;
            ++task->data[0];
        }
        break;
    case 5:
        if (task->data[5] == B_SIDE_OPPONENT)
            --gSprites[task->data[15]].y;
		
        if (!BattleAnimHelper_RunSpriteSquash(task))
        {
            gSprites[task->data[15]].y = task->data[4];
            ResetSpriteRotScale(task->data[15]);

            task->data[2] = 0;
            ++task->data[0];
        }
        break;
    case 6:
        if (!task->data[6])
            DestroyAnimVisualTask(taskId);
        break;
    }
}

static void CreateEruptionLaunchRocks(u8 spriteId, u8 taskId, u8 a3)
{
    u16 i, j;
    s8 sign;
    u16 y = GetEruptionLaunchRockInitialYPos(spriteId);
    u16 x = gSprites[spriteId].x;

    if(GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
    {
        x -= 0xC;
        sign = 1;
    }
    else
    {
        x += 0x10;
        sign = -1;
    }
	
    for (i = 0, j = 0; i <= 6; ++i)
    {
        u8 spriteId = CreateSprite(&gEruptionLaunchRockSpriteTemplate, x, y, 2);

        if (spriteId != MAX_SPRITES)
        {
            gSprites[spriteId].oam.tileNum += j * 4 + 0x40;
			
            if (++j >= 5)
                j = 0;
			
            InitEruptionLaunchRockCoordData(&gSprites[spriteId], sEruptionLaunchRockCoords[i][0] * sign, sEruptionLaunchRockCoords[i][1]);
            gSprites[spriteId].data[6] = taskId;
            gSprites[spriteId].data[7] = a3;
			
            ++gTasks[taskId].data[a3];
        }
    }
}

static void AnimEruptionLaunchRock(struct Sprite *sprite)
{
    UpdateEruptionLaunchRockPos(sprite);
	
    if (sprite->invisible)
    {
        --gTasks[sprite->data[6]].data[sprite->data[7]];
        DestroySprite(sprite);
    }
}

static u16 GetEruptionLaunchRockInitialYPos(u8 spriteId)
{
    u16 var1 = gSprites[spriteId].y + gSprites[spriteId].y2 + gSprites[spriteId].centerToCornerVecY;

    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
        var1 = ((var1 << 16) + 0x4A0000) >> 16;
    else
        var1 = ((var1 << 16) + 0x2C0000) >> 16;
	
    return var1;
}

static void InitEruptionLaunchRockCoordData(struct Sprite *sprite, s16 x, s16 y)
{
    sprite->data[0] = 0;
    sprite->data[1] = 0;
    sprite->data[2] = (u16)sprite->x * 8;
    sprite->data[3] = (u16)sprite->y * 8;
    sprite->data[4] = x * 8;
    sprite->data[5] = y * 8;
}

static void UpdateEruptionLaunchRockPos(struct Sprite *sprite)
{
    s32 var1;

    if (++sprite->data[0] > 2)
    {
        sprite->data[0] = 0;
		
        ++sprite->data[1];
		
        var1 = (u16)sprite->data[1] * (u16)sprite->data[1];
        sprite->data[3] += var1;
    }
    sprite->data[2] += sprite->data[4];
    sprite->x = sprite->data[2] >> 3;
	
    sprite->data[3] += sprite->data[5];
    sprite->y = sprite->data[3] >> 3;
	
    if (sprite->x < -8 || sprite->x > 0xf8 || sprite->y < -8 || sprite->y > 120)
        sprite->invisible = TRUE;
}

// Animates MOVE_ERUPTION's falling rocks on the screen.
// arg 0: x position
// arg 1: initial y position
// arg 2: initial delay amount
// arg 3: final y position
// arg 4: sprite tile num
static void AnimEruptionFallingRock(struct Sprite *sprite)
{
    sprite->x = gBattleAnimArgs[0];
    sprite->y = gBattleAnimArgs[1];
	
    sprite->data[0] = 0;
    sprite->data[1] = 0;
    sprite->data[2] = 0;
    sprite->data[6] = gBattleAnimArgs[2];
    sprite->data[7] = gBattleAnimArgs[3];
    sprite->oam.tileNum += gBattleAnimArgs[4] * 16;
    sprite->callback = AnimEruptionFallingRock_Step;
}

static void AnimEruptionFallingRock_Step(struct Sprite *sprite)
{
    switch (sprite->data[0])
    {
    case 0:
        if (sprite->data[6])
        {
            --sprite->data[6];
            return;
        }
        ++sprite->data[0];
        // fall through
    case 1:
        sprite->y += 8;
		
        if (sprite->y >= sprite->data[7])
        {
            sprite->y = sprite->data[7];
            ++sprite->data[0];
        }
        break;
    case 2:
        if (++sprite->data[1] > 1)
        {
            sprite->data[1] = 0;
			sprite->y2 = (++sprite->data[2] & 1) != 0 ? -3 : 3;
        }
		
        if (++sprite->data[3] > 16)
            DestroyAnimSprite(sprite);
        break;
    }
}

// Animates the MOVE_WILL_O_WISP's orbs.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: sprite anim num
static void AnimWillOWispOrb(struct Sprite *sprite)
{
    switch (sprite->data[0])
    {
    case 0:
        InitSpritePosToAnimAttacker(sprite, FALSE);
        StartSpriteAnim(sprite, gBattleAnimArgs[2]);
		
        sprite->data[7] = gBattleAnimArgs[2];
		sprite->data[4] = GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER ? 4 : -4;
        sprite->oam.priority = GetBattlerSpriteBGPriority(gBattleAnimTarget);
		
        ++sprite->data[0];
        break;
    case 1:
        sprite->data[1] += 192;
		
		sprite->y2 = GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER ? -(sprite->data[1] >> 8) : sprite->data[1] >> 8;
        sprite->x2 = Sin(sprite->data[2], sprite->data[4]);
		
        sprite->data[2] = (sprite->data[2] + 4) & 0xFF;
		
        if (++sprite->data[3] == 1)
        {
            sprite->data[3] = 0;
            ++sprite->data[0];
        }
        break;
    case 2:
        sprite->x2 = Sin(sprite->data[2], sprite->data[4]);
		
        sprite->data[2] = (sprite->data[2] + 4) & 0xFF;
		
        if (++sprite->data[3] == 31)
        {
            SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
			
            sprite->data[0] = 256;
            sprite->data[1] = sprite->x;
            sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
            sprite->data[3] = sprite->y;
            sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
            BattleAnim_InitLinearTranslationWithDuration(sprite);
            sprite->callback = AnimWillOWispOrb_Step;
        }
        break;
    }
}

static void AnimWillOWispOrb_Step(struct Sprite *sprite)
{
    s16 initialData5;

    if (!AnimTranslateLinear(sprite))
    {
        sprite->x2 += Sin(sprite->data[5], 16);
		
        initialData5 = sprite->data[5];
		
        sprite->data[5] = (sprite->data[5] + 4) & 0xFF;
		
        if ((initialData5 == 0 || initialData5 > 196) && sprite->data[5] > 0 && sprite->data[7] == 0)
            PlaySE12WithPanning(SE_M_FLAME_WHEEL, gAnimCustomPanning);
    }
    else
        DestroyAnimSprite(sprite);
}

// Animates MOVE_WILL_O_WISP's fire on the second part of the anim.
// arg 0: initial wave offset
static void AnimWillOWispFire(struct Sprite *sprite)
{
    if (!sprite->data[0])
    {
        sprite->data[1] = gBattleAnimArgs[0];
        ++sprite->data[0];
    }
    sprite->data[3] += 0xC0 * 2;
    sprite->data[4] += 0xA0;
	
    sprite->x2 = Sin(sprite->data[1], sprite->data[3] >> 8);
    sprite->y2 = Cos(sprite->data[1], sprite->data[4] >> 8);
	
    sprite->data[1] = (sprite->data[1] + 7) & 0xFF;
    
    if (sprite->data[1] < 64 || sprite->data[1] > 195)
        sprite->oam.priority = GetBattlerSpriteBGPriority(gBattleAnimTarget);
    else
        sprite->oam.priority = GetBattlerSpriteBGPriority(gBattleAnimTarget) + 1;
   
    if (++sprite->data[2] > 0x14)
        sprite->invisible ^= TRUE;
	
    if (sprite->data[2] == 0x1E)
        DestroyAnimSprite(sprite);
}

// Animates the MOVE_HEAT_WAVE targets movement.
// No args.
void AnimTask_MoveHeatWaveTargets(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    task->data[12] = GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER ? 1 : -1;
    task->data[13] = IsBattlerSpriteVisible(BATTLE_PARTNER(gBattleAnimTarget)) + 1;
    task->data[14] = GetAnimBattlerSpriteId(ANIM_TARGET);
    task->data[15] = GetAnimBattlerSpriteId(ANIM_DEF_PARTNER);
    task->func = AnimTask_MoveHeatWaveTargets_Step;
}

static void AnimTask_MoveHeatWaveTargets_Step(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
        case 0:
            task->data[10] += task->data[12] * 2;
			
            if (++task->data[1] >= 2)
            {
                task->data[1] = 0;
				
                if (++task->data[2] & 1)
                    task->data[11] = 2;
                else
                    task->data[11] = -2;
            }
			
            for (task->data[3] = 0; task->data[3] < task->data[13]; task->data[3]++)
                gSprites[task->data[task->data[3] + 14]].x2 = task->data[10] + task->data[11];
			
            if (++task->data[9] == 16)
            {
                task->data[9] = 0;
                ++task->data[0];
            }
            break;
        case 1:
            if (++task->data[1] >= 5)
            {
                task->data[1] = 0;
                ++task->data[2];
				task->data[11] = (task->data[2] & 1) ? 2 : -2;
            }
			
            for (task->data[3] = 0; task->data[3] < task->data[13]; task->data[3]++)
                gSprites[task->data[task->data[3] + 14]].x2 = task->data[10] + task->data[11];
			
            if (++task->data[9] == 96)
            {
                task->data[9] = 0;
                ++task->data[0];
            }
            break;
        case 2:
            task->data[10] -= task->data[12] * 2;
			
            if (++task->data[1] >= 2)
            {
                task->data[1] = 0;
                ++task->data[2];
				task->data[11] = (task->data[2] & 1) ? 2 : -2;
            }
			
            for (task->data[3] = 0; task->data[3] < task->data[13]; task->data[3]++)
                gSprites[task->data[task->data[3] + 14]].x2 = task->data[10] + task->data[11];
			
            if (++task->data[9] == 16)
                ++task->data[0];
            break;
        case 3:
            for (task->data[3] = 0; task->data[3] < task->data[13]; task->data[3]++)
                gSprites[task->data[task->data[3] + 14]].x2 = 0;
			
            DestroyAnimVisualTask(taskId);
            break;
    }
}

// Used to add a color mask to the battle interface / HUD in Heat Wave.
// arg 0: opacity
// arg 1: blend color
void AnimTask_BlendBackground(u8 taskId)
{
    struct BattleAnimBgData animBg;
    GetBattleAnimBgData(&animBg, 1);
    BlendPalette(animBg.paletteId * 16, 16, gBattleAnimArgs[0], gBattleAnimArgs[1]);
    DestroyAnimVisualTask(taskId);
}

// Shakes target horizontally or vertically x times, following a set pattern of alternations.
// arg 0: num shakes
// arg 1: shake value
// arg 2: 0 = horizontal shake, 1 = vertical shake
// arg 3: shake pattern table id
void AnimTask_ShakeTargetInPattern(u8 taskId)
{
    s8 val;
    u8 spriteId;
	
	switch (gTasks[taskId].data[0])
	{
		case 0:
			gTasks[taskId].data[1] = gBattleAnimArgs[0];
			gTasks[taskId].data[2] = gBattleAnimArgs[1];
			gTasks[taskId].data[3] = gBattleAnimArgs[2];
			gTasks[taskId].data[4] = gBattleAnimArgs[3];
			break;
		default:
			spriteId = gBattlerSpriteIds[gBattleAnimTarget];
			val = gTasks[taskId].data[4] == 0 ? sShakeDirsPattern0[gTasks[taskId].data[0] % 10] : sShakeDirsPattern1[gTasks[taskId].data[0] % 10];
			
			if (gTasks[taskId].data[3] == 1)
				gSprites[spriteId].y2 = gTasks[taskId].data[2] * val < 0 ? -(gTasks[taskId].data[2] * val) : gTasks[taskId].data[2] * val;
			else
				gSprites[spriteId].x2 = gTasks[taskId].data[2] * val;
			break;
	}

    if (++gTasks[taskId].data[0] == gTasks[taskId].data[1])
    {
        gSprites[spriteId].x2 = 0;
        gSprites[spriteId].y2 = 0;
        DestroyAnimVisualTask(taskId);
    }
}

// Moves the particle upwards on the given target, for the specified duration.
// arg 0: anim battler
// arg 1: x pixel offset
// arg 2: y pixel offset
// arg 3: speed
// arg 4: duration
static void AnimMoveSpriteUpwardsForDuration(struct Sprite *sprite)
{
	u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);
	
	sprite->x = GetBattlerSpriteCoord(battler, BATTLER_COORD_X) + gBattleAnimArgs[1];
	sprite->y = GetBattlerSpriteCoord(battler, BATTLER_COORD_Y) + gBattleAnimArgs[2];

	sprite->data[1] = gBattleAnimArgs[3];
	sprite->data[2] = gBattleAnimArgs[4];
	sprite->callback = AnimMoveSpriteUpwardsForDuration_Step;
}

static void AnimMoveSpriteUpwardsForDuration_Step(struct Sprite *sprite)
{
	sprite->y -= sprite->data[1];
	if (sprite->data[0]++ > sprite->data[2])
		DestroyAnimSprite(sprite);
}

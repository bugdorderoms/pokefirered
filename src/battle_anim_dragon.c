#include "global.h"
#include "battle_anim.h"
#include "scanline_effect.h"
#include "task.h"
#include "trig.h"

static void AnimDragonFireToTarget(struct Sprite *sprite);
static void AnimDragonRageFirePlume(struct Sprite *sprite);
static void AnimOverheatFlame(struct Sprite *sprite);
static void AnimOverheatFlame_Step(struct Sprite *sprite);
static void AnimDracoMeteorRock(struct Sprite *sprite);
static void AnimDracoMeteorRock_Step(struct Sprite *sprite);

static const union AnimCmd sOutrageFlameAnimCmds[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(16, 4),
    ANIMCMD_FRAME(32, 4),
    ANIMCMD_FRAME(48, 4),
    ANIMCMD_FRAME(64, 4),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sOutrageFlameAnimTable[] =
{
    sOutrageFlameAnimCmds,
};

const struct SpriteTemplate gOutrageFlameSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_EMBER,
    .paletteTag = ANIM_TAG_SMALL_EMBER,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sOutrageFlameAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimOutrageFlame,
};

static const union AnimCmd sAnim_DragonBreathFire_0[] =
{
    ANIMCMD_FRAME(16, 3),
    ANIMCMD_FRAME(32, 3),
    ANIMCMD_FRAME(48, 3),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_DragonBreathFire_1[] =
{
    ANIMCMD_FRAME(16, 3, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 3, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_FRAME(48, 3, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnims_DragonBreathFire[] =
{
    sAnim_DragonBreathFire_0,
    sAnim_DragonBreathFire_1,
};

static const union AffineAnimCmd sAffineAnim_DragonBreathFire_0[] =
{
    AFFINEANIMCMD_FRAME(0x50, 0x50, 127, 0),
    AFFINEANIMCMD_FRAME(0xD, 0xD, 0, 100),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_DragonBreathFire_1[] =
{
    AFFINEANIMCMD_FRAME(0x50, 0x50, 0, 0),
    AFFINEANIMCMD_FRAME(0xD, 0xD, 0, 100),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims_DragonBreathFire[] =
{
    sAffineAnim_DragonBreathFire_0,
    sAffineAnim_DragonBreathFire_1,
};

const struct SpriteTemplate gDragonBreathFireSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_EMBER,
    .paletteTag = ANIM_TAG_WATER_ORB,
    .oam = &gOamData_AffineDouble_ObjNormal_32x32,
    .anims = sAnims_DragonBreathFire,
    .images = NULL,
    .affineAnims = sAffineAnims_DragonBreathFire,
    .callback = AnimDragonFireToTarget,
};

const struct SpriteTemplate gFlamethrowerFireSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_EMBER,
    .paletteTag = ANIM_TAG_SMALL_EMBER,
    .oam = &gOamData_AffineDouble_ObjNormal_32x32,
    .anims = sAnims_DragonBreathFire,
    .images = NULL,
    .affineAnims = sAffineAnims_DragonBreathFire,
    .callback = AnimDragonFireToTarget,
};

const union AnimCmd sAnim_DragonRageFirePlume[] =
{
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_FRAME(16, 5),
    ANIMCMD_FRAME(32, 5),
    ANIMCMD_FRAME(48, 5),
    ANIMCMD_FRAME(64, 5),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_DragonRageFirePlume[] =
{
    sAnim_DragonRageFirePlume,
};

const struct SpriteTemplate gDragonRageFirePlumeSpriteTemplate =
{
    .tileTag = ANIM_TAG_FIRE_PLUME,
    .paletteTag = ANIM_TAG_PURPLE_FLAME,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sAnims_DragonRageFirePlume,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimDragonRageFirePlume,
};

const struct SpriteTemplate gEarthPowerFirePlumeSpriteTemplate =
{
    .tileTag = ANIM_TAG_FIRE_PLUME,
    .paletteTag = ANIM_TAG_FIRE_PLUME,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sAnims_DragonRageFirePlume,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimDragonRageFirePlume,
};

static const union AnimCmd sAnim_DragonRageFire[] =
{
    ANIMCMD_FRAME(16, 3),
    ANIMCMD_FRAME(32, 3),
    ANIMCMD_FRAME(48, 3),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnims_DragonRageFire[] =
{
    sAnim_DragonRageFire,
    sAnim_DragonRageFire,
};

static const union AffineAnimCmd sAffineAnim_DragonRageFire_0[] =
{
    AFFINEANIMCMD_FRAME(0x64, 0x64, 127, 1),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnim_DragonRageFire_1[] =
{
    AFFINEANIMCMD_FRAME(0x64, 0x64, 0, 1),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims_DragonRageFire[] =
{
    sAffineAnim_DragonRageFire_0,
    sAffineAnim_DragonRageFire_1,
};

const struct SpriteTemplate gDragonRageFireSpitSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_EMBER,
    .paletteTag = ANIM_TAG_PURPLE_FLAME,
    .oam = &gOamData_AffineDouble_ObjNormal_32x32,
    .anims = sAnims_DragonRageFire,
    .images = NULL,
    .affineAnims = sAffineAnims_DragonRageFire,
    .callback = AnimDragonFireToTarget,
};

const struct SpriteTemplate gOverheatFlameSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_EMBER,
    .paletteTag = ANIM_TAG_SMALL_EMBER,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sOutrageFlameAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimOverheatFlame,
};

const struct SpriteTemplate gDragonDanceBlackSmokeSpriteTemplate =
{
    .tileTag = ANIM_TAG_PINK_CLOUD,
    .paletteTag = ANIM_TAG_HANDS_AND_FEET,
    .oam = &gOamData_AffineOff_ObjBlend_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimParticleInVortex,
};

const struct SpriteTemplate gDragonPulseRingSpriteTemplate =
{
    .tileTag = ANIM_TAG_DRAGON_PULSE_RING,
    .paletteTag = ANIM_TAG_DRAGON_PULSE_RING,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = TranslateAnimSpriteToTargetMonLocation,
};

const struct SpriteTemplate gDragonPulseBallSpriteTemplate =
{
    .tileTag = ANIM_TAG_FLASH_CANNON_BALL,
    .paletteTag = ANIM_TAG_FLASH_CANNON_BALL,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = TranslateAnimSpriteToTargetMonLocation,
};

const struct SpriteTemplate gPurpleDrakeSpriteTemplate =
{
    .tileTag = ANIM_TAG_PURPLE_DRAKE,
    .paletteTag = ANIM_TAG_PURPLE_DRAKE,
    .oam = &gOamData_AffineNormal_ObjNormal_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSkyAttackBird,
};

const struct SpriteTemplate gDracoMeteorRockSpriteTemplate =
{
    .tileTag = ANIM_TAG_ROCKS,
    .paletteTag = ANIM_TAG_FAIRY_LOCK_CHAINS,
    .oam = &gOamData_AffineOff_ObjNormal_32x32_HiPrio,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimDracoMeteorRock,
};

static const union AffineAnimCmd sAffineAnim_DracoMeteorRockTail[] =
{
    AFFINEANIMCMD_FRAME(16, 16, 0, 16), // Double in size
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnims_DracoMeteorRockTail[] =
{
    sAffineAnim_DracoMeteorRockTail,
};

const struct SpriteTemplate gDracoMeteorRockTailSpriteTemplate =
{
    .tileTag = ANIM_TAG_WATER_ORB,
    .paletteTag = ANIM_TAG_WATER_ORB,
    .oam = &gOamData_AffineDouble_ObjBlend_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_DracoMeteorRockTail,
    .callback = AnimDracoMeteorRock,
};

const struct SpriteTemplate gMaxFlareFireGeyserSpriteTemplate =
{
    .tileTag = ANIM_TAG_SMALL_EMBER,
    .paletteTag = ANIM_TAG_SMALL_EMBER,
    .oam = &gOamData_AffineOff_ObjNormal_32x32_HiPrio,
    .anims = sAnims_DragonBreathFire,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimGeyserSprite,
};

// Animates MOVE_OUTRAGE's flame sprite.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: duration
// arg 3: x slide
// arg 4: y slide
// arg 5: flicker timer
void AnimOutrageFlame(struct Sprite *sprite)
{
    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
    
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
    {
        sprite->x -= gBattleAnimArgs[0];
        gBattleAnimArgs[3] = -gBattleAnimArgs[3];
        gBattleAnimArgs[4] = -gBattleAnimArgs[4];
    }
    else
        sprite->x += gBattleAnimArgs[0];

    sprite->y += gBattleAnimArgs[1];
    
    sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[1] = gBattleAnimArgs[3];
    sprite->data[3] = gBattleAnimArgs[4];
    sprite->data[5] = gBattleAnimArgs[5];
    sprite->invisible = TRUE;
    sprite->callback = TranslateSpriteLinearAndFlicker;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Animates the fore plume in the second part of MOVE_DRAGON_RAGE's anim.
// arg 0: anim battler
// arg 1: initial x offset
// arg 2: initial y offset
static void AnimDragonRageFirePlume(struct Sprite *sprite)
{
    u32 battlerId = GetBattlerForAnimScript(gBattleAnimArgs[0]);
    
    sprite->x = GetBattlerSpriteCoord(battlerId, BATTLER_COORD_X);
    sprite->y = GetBattlerSpriteCoord(battlerId, BATTLER_COORD_Y);
    
    SetAnimSpriteInitialXOffset(sprite, gBattleAnimArgs[1]);
    sprite->y += gBattleAnimArgs[2];
    
    sprite->callback = RunStoredCallbackWhenAnimEnds;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// For Dragon Breath and Dragon Rage.
// arg 0: player side initial x offset
// arg 1: initial y offset
// arg 2: target x offset
// arg 3: target y offset
// arg 4: duration
static void AnimDragonFireToTarget(struct Sprite *sprite)
{
    SetSpriteCoordsToAnimAttackerCoords(sprite);
    
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3];
    
    sprite->y += gBattleAnimArgs[1];
    
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
    {
        sprite->x -= gBattleAnimArgs[1];
        sprite->data[2] -= gBattleAnimArgs[2];
        StartSpriteAffineAnim(sprite, 1);
    }
    else
    {
        sprite->x += gBattleAnimArgs[0];
        sprite->data[2] += gBattleAnimArgs[2];
        StartSpriteAnim(sprite, 1);
    }
    sprite->data[0] = gBattleAnimArgs[4];
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Animates Overheat flames.
// arg 0: movement distance
// arg 1: wave index
// arg 2: x amplitude
// arg 3: movement duration
// arg 4: y pixel offset
static void AnimOverheatFlame(struct Sprite *sprite)
{
    s32 yAmplitude = (gBattleAnimArgs[2] * 3) / 5;

    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[4];
    
    sprite->data[1] = Cos(gBattleAnimArgs[1], gBattleAnimArgs[2]);
    sprite->data[2] = Sin(gBattleAnimArgs[1], yAmplitude);
    
    sprite->x += sprite->data[1] * gBattleAnimArgs[0];
    sprite->y += sprite->data[2] * gBattleAnimArgs[0];
    
    sprite->data[3] = gBattleAnimArgs[3];
    sprite->callback = AnimOverheatFlame_Step;
}

static void AnimOverheatFlame_Step(struct Sprite *sprite)
{
    sprite->data[4] += sprite->data[1];
    sprite->data[5] += sprite->data[2];
    
    sprite->x2 = sprite->data[4] / 10;
    sprite->y2 = sprite->data[5] / 10;
    
    if (++sprite->data[0] > sprite->data[3])
        DestroyAnimSprite(sprite);
}

// Moves the Draco Meteor rocks across the screen.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: final x pixel offset
// arg 3: final y pixel offset
// arg 4: duration
static void AnimDracoMeteorRock(struct Sprite *sprite)
{
    sprite->data[6] = gBattleAnimArgs[2];
    sprite->data[7] = gBattleAnimArgs[3];
    
    if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_PLAYER)
    {
        gBattleAnimArgs[0] = -gBattleAnimArgs[0];
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];
    }
    sprite->data[0] = sprite->x + gBattleAnimArgs[0];
    sprite->data[1] = sprite->y + gBattleAnimArgs[1];
    
    sprite->data[2] = sprite->x + gBattleAnimArgs[2];
    sprite->data[3] = sprite->y + gBattleAnimArgs[3];
    sprite->data[4] = gBattleAnimArgs[4];
    
    sprite->x = sprite->data[0];
    sprite->y = sprite->data[1];
    sprite->callback = AnimDracoMeteorRock_Step;
}

static void AnimDracoMeteorRock_Step(struct Sprite *sprite)
{
    sprite->x2 = ((sprite->data[2] - sprite->data[0]) * sprite->data[5]) / sprite->data[4];
    sprite->y2 = ((sprite->data[3] - sprite->data[1]) * sprite->data[5]) / sprite->data[4];
    
    if (sprite->data[5] != sprite->data[4])
        sprite->data[5]++;
    else
        DestroyAnimSprite(sprite);
}

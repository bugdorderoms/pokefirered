#include "global.h"
#include "battle_anim.h"
#include "random.h"
#include "scanline_effect.h"
#include "task.h"
#include "trig.h"

static void AnimBonemerangProjectile(struct Sprite *sprite);
static void AnimBonemerangProjectileStep(struct Sprite *sprite);
static void AnimBoneHitProjectile(struct Sprite *sprite);
static void AnimMudSportDirt(struct Sprite *sprite);
static void AnimFissureDirtPlumeParticle(struct Sprite *sprite);
static void AnimDigDirtMound(struct Sprite *sprite);
static void AnimMudSportDirtRising(struct Sprite *sprite);
static void AnimMudSportDirtFalling(struct Sprite *sprite);
static void AnimTask_DigDisappear(u8 taskId);
static void AnimTask_DigBounceMovement(u8 taskId);
static void SetDigScanlineEffect(u8 useBg1, s16 y, s16 endY);
static void AnimTask_DigSetVisibleUnderground(u8 taskId);
static void AnimTask_DigRiseUpFromHole(u8 taskId);
static void AnimTask_HorizontalShakeTerrain(u8 taskId);
static void AnimTask_HorizontalShakeBattlers(u8 taskId);
static void AnimTask_HorizontalShakeBattlersDoShake(struct Task *task);
static void AnimTask_PositionFissureBgOnBattler_Step(u8 taskId);
static void AnimSpikes_Step1(struct Sprite *);
static void AnimSpikes_Step2(struct Sprite *);

static const union AffineAnimCmd sAffineAnim_Bonemerang[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, 15, 1),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sAffineAnim_SpinningBone[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, 20, 1),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd *const sAffineAnims_Bonemerang[] =
{
    sAffineAnim_Bonemerang,
};

static const union AffineAnimCmd *const sAffineAnims_SpinningBone[] =
{
    sAffineAnim_SpinningBone,
};

const struct SpriteTemplate gBonemerangSpriteTemplate =
{
    .tileTag = ANIM_TAG_BONE,
    .paletteTag = ANIM_TAG_BONE,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_Bonemerang,
    .callback = AnimBonemerangProjectile,
};

const struct SpriteTemplate gSpinningBoneSpriteTemplate =
{
    .tileTag = ANIM_TAG_BONE,
    .paletteTag = ANIM_TAG_BONE,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_SpinningBone,
    .callback = AnimBoneHitProjectile,
};

const struct SpriteTemplate gSandAttackDirtSpriteTemplate =
{
    .tileTag = ANIM_TAG_MUD_SAND,
    .paletteTag = ANIM_TAG_MUD_SAND,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimDirtScatter,
};

static const union AnimCmd sAnim_MudSlapMud[] =
{
    ANIMCMD_FRAME(1, 1),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_MudSlapMud[] =
{
    sAnim_MudSlapMud,
};

const struct SpriteTemplate gMudSlapMudSpriteTemplate =
{
    .tileTag = ANIM_TAG_MUD_SAND,
    .paletteTag = ANIM_TAG_MUD_SAND,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = sAnims_MudSlapMud,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimDirtScatter,
};

const struct SpriteTemplate gMudsportMudSpriteTemplate =
{
    .tileTag = ANIM_TAG_MUD_SAND,
    .paletteTag = ANIM_TAG_MUD_SAND,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimMudSportDirt,
};

const struct SpriteTemplate gDirtPlumeSpriteTemplate =
{
    .tileTag = ANIM_TAG_MUD_SAND,
    .paletteTag = ANIM_TAG_MUD_SAND,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimFissureDirtPlumeParticle,
};

const struct SpriteTemplate gDirtMoundSpriteTemplate =
{
    .tileTag = ANIM_TAG_DIRT_MOUND,
    .paletteTag = ANIM_TAG_DIRT_MOUND,
    .oam = &gOamData_AffineOff_ObjNormal_32x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimDigDirtMound,
};

const struct SpriteTemplate gSpikesSpriteTemplate =    
{
    .tileTag = ANIM_TAG_SPIKES,
    .paletteTag = ANIM_TAG_SPIKES,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSpikes,
};

const struct SpriteTemplate gSwirlingDirtSpriteTemplate =
{
    .tileTag = ANIM_TAG_MUD_SAND,
    .paletteTag = ANIM_TAG_MUD_SAND,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimParticleInVortex,
};

const struct SpriteTemplate gMudShotOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_WATER_ORB,
    .paletteTag = ANIM_TAG_WATER_ORB,
    .oam = &gOamData_AffineOff_ObjBlend_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimToTargetInSinWave,
};

// Moves a bone projectile towards the target mon, which moves like a boomerang. After hitting the target mon, it comes back to the user.
// No args.
static void AnimBonemerangProjectile(struct Sprite *sprite)
{
    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
	
    sprite->data[0] = 20;
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
    sprite->data[5] = -40;
    InitAnimArcTranslation(sprite);
    sprite->callback = AnimBonemerangProjectileStep;
}

static void AnimBonemerangProjectileStep(struct Sprite *sprite)
{
    if (TranslateAnimHorizontalArc(sprite))
    {
		SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
		
        sprite->data[0] = 20;
		sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
		sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
        sprite->data[5] = 40;
        InitAnimArcTranslation(sprite);
        sprite->callback = DestroyAnimSpriteAfterHorizontalTranslation;
    }
}

// Moves a bone projectile towards the target mon, starting right next to the target mon.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: target x pixel offset
// arg 3: target y pixel offset
// arg 4: duration
static void AnimBoneHitProjectile(struct Sprite *sprite)
{
    InitSpritePosToAnimTarget(sprite, TRUE);
	
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];
	
    sprite->data[0] = gBattleAnimArgs[4];
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X) + gBattleAnimArgs[2];
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[3];
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Moves a small dirt projectile towards the target mon.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: duration
// arg 3: target x pixel offset
// arg 4: target y pixel offset
void AnimDirtScatter(struct Sprite *sprite)
{
    u8 targetXPos, targetYPos;
    s16 xOffset, yOffset;

    InitSpritePosToAnimAttacker(sprite, TRUE);
	
    targetXPos = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    targetYPos = GetBattlerSpriteCoord2(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
	
    xOffset = RandomMax(32);
	if (xOffset > 16)
        xOffset = 16 - xOffset;
	
    yOffset = RandomMax(32);
    if (yOffset > 16)
        yOffset = 16 - yOffset;
	
    sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[2] = targetXPos + xOffset;
    sprite->data[4] = targetYPos + yOffset;
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Moves a particle of dirt in the Mud Sport animation. The dirt can either be rising upward, or falling down.
// arg 0: FALSE = dirt is rising into the air, TRUE = dirt is falling down
// arg 1: initial x pixel offset
// arg 2: initial y pixel offset
static void AnimMudSportDirt(struct Sprite *sprite)
{
    ++sprite->oam.tileNum;
	
    if (!gBattleAnimArgs[0])
    {
        sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X) + gBattleAnimArgs[1];
        sprite->y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[2];
		
        sprite->data[0] = gBattleAnimArgs[1] > 0 ? 1 : -1;
        sprite->callback = AnimMudSportDirtRising;
    }
    else
    {
        sprite->x = gBattleAnimArgs[1];
        sprite->y = gBattleAnimArgs[2];
        sprite->y2 = -gBattleAnimArgs[2];
        sprite->callback = AnimMudSportDirtFalling;
    }
}

static void AnimMudSportDirtRising(struct Sprite *sprite)
{
    if (++sprite->data[1] > 1)
    {
        sprite->data[1] = 0;
        sprite->x += sprite->data[0];
    }
    sprite->y -= 4;
	
    if (sprite->y < -4)
        DestroyAnimSprite(sprite);
}

static void AnimMudSportDirtFalling(struct Sprite *sprite)
{
    switch (sprite->data[0])
    {
    case 0:
        sprite->y2 += 4;
		
        if (sprite->y2 >= 0)
        {
            sprite->y2 = 0;
            ++sprite->data[0];
        }
        break;
    case 1:
        if (++sprite->data[1] > 0)
        {
            sprite->data[1] = 0;
			
            sprite->invisible ^= TRUE;
			
            if (++sprite->data[2] == 10)
                DestroyAnimSprite(sprite);
        }
        break;
    }
}

// Animates the battler's dig down movement.
// arg 0: dig disappear (boolean)
void AnimTask_DigDownMovement(u8 taskId)
{
    gTasks[taskId].func = gBattleAnimArgs[0] ? AnimTask_DigDisappear : AnimTask_DigBounceMovement;
    gTasks[taskId].func(taskId);
}

static void AnimTask_DigBounceMovement(u8 taskId)
{
    u8 var0;
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        task->data[10] = GetAnimBattlerSpriteId(ANIM_ATTACKER);
        task->data[11] = GetBattlerSpriteBGPriorityRank(gBattleAnimAttacker);
		
        if (task->data[11] == 1)
        {
            task->data[12] = gBattle_BG1_X;
            task->data[13] = gBattle_BG1_Y;
        }
        else
        {
            task->data[12] = gBattle_BG2_X;
            task->data[13] = gBattle_BG2_Y;
        }
        var0 = GetBattlerYCoordWithElevation(gBattleAnimAttacker);
		
        task->data[14] = var0 - 32;
        task->data[15] = var0 + 32;
		
        if (task->data[14] < 0)
            task->data[14] = 0;
		
        gSprites[task->data[10]].invisible = TRUE;
        ++task->data[0];
        break;
    case 1:
        SetDigScanlineEffect(task->data[11], task->data[14], task->data[15]);
        ++task->data[0];
        break;
    case 2:
        task->data[2] = (task->data[2] + 6) & 0x7F;
		
        if (++task->data[4] > 2)
        {
            task->data[4] = 0;
            ++task->data[3];
        }
        task->data[5] = task->data[3] + (gSineTable[task->data[2]] >> 4);
		
        if (task->data[11] == 1)
            gBattle_BG1_Y = task->data[13] - task->data[5];
        else
            gBattle_BG2_Y = task->data[13] - task->data[5];

        if (task->data[5] > 63)
        {
            task->data[5] = 120 - task->data[14];
			
            if (task->data[11] == 1)
                gBattle_BG1_Y = task->data[13] - task->data[5];
            else
                gBattle_BG2_Y = task->data[13] - task->data[5];

            gSprites[task->data[10]].x2 = 272 - gSprites[task->data[10]].x;
            ++task->data[0];
        }
        break;
    case 3:
        gScanlineEffect.state = 3;
        ++task->data[0];
        break;
    case 4:
		gSprites[task->data[10]].invisible = TRUE;
        DestroyAnimVisualTask(taskId);
        break;
    }
}

static void AnimTask_DigDisappear(u8 taskId)
{
    u8 spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);

    gSprites[spriteId].x2 = 0;
    gSprites[spriteId].y2 = 0;
	gSprites[spriteId].invisible = TRUE;
	
    if (GetBattlerSpriteBGPriorityRank(gBattleAnimAttacker) == 1)
        gBattle_BG1_Y = 0;
    else
        gBattle_BG2_Y = 0;
	
    DestroyAnimVisualTask(taskId);
}

// Animates the battler's dig up movement.
// arg 0: dig reappear (boolean)
void AnimTask_DigUpMovement(u8 taskId)
{
    gTasks[taskId].func = gBattleAnimArgs[0] ? AnimTask_DigRiseUpFromHole : AnimTask_DigSetVisibleUnderground;
    gTasks[taskId].func(taskId);
}

static void AnimTask_DigSetVisibleUnderground(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        task->data[10] = GetAnimBattlerSpriteId(ANIM_ATTACKER);
        gSprites[task->data[10]].invisible = FALSE;
        gSprites[task->data[10]].x2 = 0;
        gSprites[task->data[10]].y2 = 160 - gSprites[task->data[10]].y;
        ++task->data[0];
        break;
    case 1:
        DestroyAnimVisualTask(taskId);
		break;
    }
}

static void AnimTask_DigRiseUpFromHole(u8 taskId)
{
    u8 var0;
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        task->data[10] = GetAnimBattlerSpriteId(ANIM_ATTACKER);
        task->data[11] = GetBattlerSpriteBGPriorityRank(gBattleAnimAttacker);
        task->data[12] = task->data[11] == 1 ? gBattle_BG1_X : gBattle_BG2_X;
		
        var0 = GetBattlerYCoordWithElevation(gBattleAnimAttacker);
        task->data[14] = var0 - 32;
        task->data[15] = var0 + 32;
        ++task->data[0];
        break;
    case 1:
        SetDigScanlineEffect(task->data[11], 0, task->data[15]);
        ++task->data[0];
        break;
    case 2:
        gSprites[task->data[10]].y2 = 96;
        ++task->data[0];
        break;
    case 3:
        gSprites[task->data[10]].y2 -= 8;
		
        if (gSprites[task->data[10]].y2 == 0)
        {
            gScanlineEffect.state = 3;
            ++task->data[0];
        }
        break;
    case 4:
        DestroyAnimVisualTask(taskId);
        break;
    }
}

static void SetDigScanlineEffect(bool8 useBG1, s16 y, s16 endY)
{
    s16 bgX;
    struct ScanlineEffectParams scanlineParams;

    if (useBG1 == 1)
    {
        bgX = gBattle_BG1_X;
        scanlineParams.dmaDest = &REG_BG1HOFS;
    }
    else
    {
        bgX = gBattle_BG2_X;
        scanlineParams.dmaDest = &REG_BG2HOFS;
    }
	
    if (y < 0)
        y = 0;
	
    while (y < endY)
    {
        gScanlineEffectRegBuffers[0][y] = bgX;
        gScanlineEffectRegBuffers[1][y] = bgX;
        ++y;
    }
	
    while (y < 160)
    {
        gScanlineEffectRegBuffers[0][y] = bgX + 240;
        gScanlineEffectRegBuffers[1][y] = bgX + 240;
        ++y;
    }
    scanlineParams.dmaControl = SCANLINE_EFFECT_DMACNT_16BIT;
    scanlineParams.initState = 1;
    ScanlineEffect_SetParams(scanlineParams);
}

// Moves a particle of dirt in a plume of dirt. Used in Fissure and Dig.
// arg 0: anim battler
// arg 1: which side of mon (0 = left, 1 = right)
// arg 2: target x offset
// arg 3: target y offset
// arg 4: wave amplitude
// arg 5: duration
static void AnimFissureDirtPlumeParticle(struct Sprite *sprite)
{
    s8 battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);
    s16 xOffset = 24;

    if (gBattleAnimArgs[1] == 1)
    {
        xOffset *= -1;
        gBattleAnimArgs[2] *= -1;
    }
    sprite->x = GetBattlerSpriteCoord(battler, BATTLER_COORD_X) + xOffset;
    sprite->y = GetBattlerYCoordWithElevation(battler) + 30;
	
    sprite->data[0] = gBattleAnimArgs[5];
    sprite->data[2] = sprite->x + gBattleAnimArgs[2];
    sprite->data[4] = sprite->y + gBattleAnimArgs[3];
    sprite->data[5] = gBattleAnimArgs[4];
    InitAnimArcTranslation(sprite);
    sprite->callback = DestroyAnimSpriteAfterHorizontalTranslation;
}

// Displays the dirt mound seen in the move Dig for set duration.
// The dirt mound image is too large for a single sprite, so two sprites are lined up next to each other.
// arg 0: anim battler
// arg 1: oam tile num (0 = left half of image, 1 = right half of image)
// arg 2: duration
static void AnimDigDirtMound(struct Sprite *sprite)
{
    s8 battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);

    sprite->x = GetBattlerSpriteCoord(battler, BATTLER_COORD_X) - 16 + (gBattleAnimArgs[1] * 32);
    sprite->y = GetBattlerYCoordWithElevation(battler) + 32;
	
    sprite->oam.tileNum += gBattleAnimArgs[1] * 8;
	sprite->data[0] = gBattleAnimArgs[2];
	
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
    sprite->callback = WaitAnimForDuration;
}

// Shakes battler(s) or the battle terrain back and forth horizontally. Used by e.g. Earthquake, Eruption
// arg 0: What to shake. 0-3 for any specific battler, MAX_BATTLERS_COUNT for all battlers, MAX_BATTLERS_COUNT + 1 for the terrain
// arg 1: Shake intensity, used to calculate horizontal pixel offset (if 0, use move power instead)
// arg 2: Length of time to shake for
void AnimTask_HorizontalShake(u8 taskId)
{
    u16 i;
    struct Task *task = &gTasks[taskId];

    if (gBattleAnimArgs[1])
        task->data[14] = task->data[15] = gBattleAnimArgs[1] + 3;
    else
        task->data[14] = task->data[15] = (gAnimMovePower / 10) + 3;

    task->data[3] = gBattleAnimArgs[2];
	
    switch (gBattleAnimArgs[0])
    {
    case MAX_BATTLERS_COUNT: // Shake all battlers
        task->data[13] = 0;
		
        for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
        {
            if (IsBattlerSpriteVisible(i))
            {
                task->data[task->data[13] + 9] = gBattlerSpriteIds[i];
                ++task->data[13];
            }
        }
        task->func = AnimTask_HorizontalShakeBattlers;
        break;
	case MAX_BATTLERS_COUNT + 1: // Shake terrain
        task->data[13] = gBattle_BG3_X;
        task->func = AnimTask_HorizontalShakeTerrain;
        break;
    default: // Skane specified battler
        task->data[9] = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);
		
        if (task->data[9] == 0xFF)
            DestroyAnimVisualTask(taskId);
        else
        {
            task->data[13] = 1;
            task->func = AnimTask_HorizontalShakeBattlers;
        }
        break;
    }
}

static void AnimTask_HorizontalShakeTerrain(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        if (++task->data[1] > 1)
        {
            task->data[1] = 0;
			
            if (!(task->data[2] & 1))
                gBattle_BG3_X = task->data[13] + task->data[15];
            else
                gBattle_BG3_X = task->data[13] - task->data[15];

            if (++task->data[2] == task->data[3])
            {
                task->data[2] = 0;
                --task->data[14];
                ++task->data[0];
            }
        }
        break;
    case 1:
        if (++task->data[1] > 1)
        {
            task->data[1] = 0;
			
            if (!(task->data[2] & 1))
                gBattle_BG3_X = task->data[13] + task->data[14];
            else
                gBattle_BG3_X = task->data[13] - task->data[14];

            if (++task->data[2] == 4)
            {
                task->data[2] = 0;
				
                if (--task->data[14] == 0)
                    ++task->data[0];
            }
        }
        break;
    case 2:
        gBattle_BG3_X = task->data[13];
        DestroyAnimVisualTask(taskId);
        break;
    }
}

static void AnimTask_HorizontalShakeBattlers(u8 taskId)
{
    u16 i;
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        if (++task->data[1] > 1)
        {
            task->data[1] = 0;
			
            AnimTask_HorizontalShakeBattlersDoShake(task);
			
            if (++task->data[2] == task->data[3])
            {
                task->data[2] = 0;
                --task->data[14];
                ++task->data[0];
            }
        }
        break;
    case 1:
        if (++task->data[1] > 1)
        {
            task->data[1] = 0;
			
            AnimTask_HorizontalShakeBattlersDoShake(task);
			
            if (++task->data[2] == 4)
            {
                task->data[2] = 0;
				
                if (--task->data[14] == 0)
                    ++task->data[0];
            }
        }
        break;
    case 2:
        for (i = 0; i < task->data[13]; ++i)
            gSprites[task->data[9 + i]].x2 = 0;
		
        DestroyAnimVisualTask(taskId);
        break;
    }
}

static void AnimTask_HorizontalShakeBattlersDoShake(struct Task *task)
{
    u16 i, xOffset;

    if (!(task->data[2] & 1))
        xOffset = (task->data[14] / 2) + (task->data[14] & 1);
    else
        xOffset = -(task->data[14] / 2);
	
    for (i = 0; i < task->data[13]; ++i)
        gSprites[task->data[9 + i]].x2 = xOffset;
}

// Returns where the move's power is over 99. Used by MOVE_MAGNITUDE.
// No args.
void AnimTask_IsPowerOver99(u8 taskId)
{
    gBattleAnimArgs[ARG_RET_ID] = (gAnimMovePower > 99);
    DestroyAnimVisualTask(taskId);
}

// Positiones the fissure bg.
// arg 0: anim battler
// arg 1: task subpriority
void AnimTask_PositionFissureBgOnBattler(u8 taskId)
{
	u8 battler = GetBattlerForAnimScript(gBattleAnimArgs[0]);
    struct Task *newTask = &gTasks[CreateTask(AnimTask_PositionFissureBgOnBattler_Step, gBattleAnimArgs[1])];
    
    newTask->data[1] = (32 - GetBattlerSpriteCoord(battler, BATTLER_COORD_X)) & 0x1FF;
    newTask->data[2] = (64 - GetBattlerSpriteCoord(battler, BATTLER_COORD_Y_PIC_OFFSET)) & 0xFF;
	
    gBattle_BG3_X = newTask->data[1];
    gBattle_BG3_Y = newTask->data[2];
	
    DestroyAnimVisualTask(taskId);
}

static void AnimTask_PositionFissureBgOnBattler_Step(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    if (gBattleAnimArgs[ARG_RET_ID] == -1)
    {
        gBattle_BG3_X = 0;
        gBattle_BG3_Y = 0;
        DestroyTask(taskId);
    }
    else
    {
        gBattle_BG3_X = task->data[1];
        gBattle_BG3_Y = task->data[2];
    }
}

// Throws a projectile to the target's feet.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: final x pixel offset
// arg 3: final y pixel offset
// arg 4: speed
void AnimSpikes(struct Sprite *sprite)
{
    u16 x, y;

    InitSpritePosToAnimAttacker(sprite, TRUE);
    SetAverageBattlerPositions(gBattleAnimTarget, FALSE, &x, &y);
	
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];

    sprite->data[0] = gBattleAnimArgs[4];
    sprite->data[2] = x + gBattleAnimArgs[2];
    sprite->data[4] = y + gBattleAnimArgs[3];
    sprite->data[5] = -50;
    InitAnimArcTranslation(sprite);
    sprite->callback = AnimSpikes_Step1;
}

static void AnimSpikes_Step1(struct Sprite *sprite)
{
    if (TranslateAnimHorizontalArc(sprite))
    {
        sprite->data[0] = 30;
        sprite->data[1] = 0;
        sprite->callback = WaitAnimForDuration;
        StoreSpriteCallbackInData6(sprite, AnimSpikes_Step2);
    }
}

static void AnimSpikes_Step2(struct Sprite *sprite)
{
    if (sprite->data[1] & 1)
        sprite->invisible ^= TRUE;

    if (++sprite->data[1] == 16)
        DestroyAnimSprite(sprite);
}

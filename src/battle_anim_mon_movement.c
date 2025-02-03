#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "task.h"
#include "trig.h"

#undef abs
#define abs(x) ((x) < 0 ? -(x) : (x))

static void AnimTask_ShakeMonStep(u8 taskId);
static void AnimTask_ShakeMon2Step(u8 taskId);
static void AnimTask_ShakeMonInPlaceStep(u8 taskId);
static void AnimTask_ShakeAndSinkMonStep(u8 taskId);
static void AnimTask_ShakeTargetBasedOnMovePowerOrDmgStep(u8 taskId);
static void AnimTask_TranslateMonEllipticalStep(u8 taskId);
static void DoHorizontalLunge(struct Sprite * sprite);
static void ReverseHorizontalLungeDirection(struct Sprite * sprite);
static void DoVerticalDip(struct Sprite * sprite);
static void ReverseVerticalDipDirection(struct Sprite * sprite);
static void SlideMonToOriginalPos(struct Sprite * sprite);
static void SlideMonToOriginalPosStep(struct Sprite * sprite);
static void SlideMonToOffset(struct Sprite * sprite);
static void SlideMonToOffsetAndBack(struct Sprite * sprite);
static void SlideMonToOffsetAndBackStep(struct Sprite * sprite);
static void AnimTask_WindUpLungePart1(u8 taskId);
static void AnimTask_WindUpLungePart2(u8 taskId);
static void AnimTask_SlideOffScreen_Step(u8 taskId);
static void AnimTask_SwayMonStep(u8 taskId);
static void AnimTask_ScaleMonAndRestoreStep(u8 taskId);
static void AnimTask_RotateMonSpriteToSide_Step(u8 taskId);
static void SlideMonToPartnerOffsetAndBack(struct Sprite * sprite);
static void SlideMonToPartnerOffsetAndBackStep(struct Sprite * sprite);

const struct SpriteTemplate gHorizontalLungeSpriteTemplate =
{
    .tileTag = 0,
    .paletteTag = 0,
    .oam = &gDummyOamData,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = DoHorizontalLunge,
};

const struct SpriteTemplate gVerticalDipSpriteTemplate =
{
    .tileTag = 0,
    .paletteTag = 0,
    .oam = &gDummyOamData,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = DoVerticalDip,
};

const struct SpriteTemplate gSlideMonToOriginalPosSpriteTemplate =
{
    .tileTag = 0,
    .paletteTag = 0,
    .oam = &gDummyOamData,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SlideMonToOriginalPos,
};

const struct SpriteTemplate gSlideMonToOffsetSpriteTemplate =
{
    .tileTag = 0,
    .paletteTag = 0,
    .oam = &gDummyOamData,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SlideMonToOffset,
};

const struct SpriteTemplate gSlideMonToPartnerOffsetAndBackSpriteTemplate =
{
    .tileTag = 0,
    .paletteTag = 0,
    .oam = &gDummyOamData,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SlideMonToPartnerOffsetAndBack,
};

const struct SpriteTemplate gSlideMonToOffsetAndBackSpriteTemplate =
{
    .tileTag = 0,
    .paletteTag = 0,
    .oam = &gDummyOamData,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SlideMonToOffsetAndBack,
};

// Task to facilitate simple shaking of a pokemon's picture in battle.
// The shaking alternates between the original position and the target position.
// arg 0: anim battler
// arg 1: x pixel offset
// arg 2: y pixel offset
// arg 3: num times to shake
// arg 4: frame delay
void AnimTask_ShakeMon(u8 taskId)
{
    u8 spriteId = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);

    if (spriteId == 0xFF)
        DestroyAnimVisualTask(taskId);
    else
    {
		gTasks[taskId].data[0] = spriteId;
        gTasks[taskId].data[1] = gBattleAnimArgs[3];
        gTasks[taskId].data[2] = gBattleAnimArgs[4];
        gTasks[taskId].data[3] = gBattleAnimArgs[4];
        gTasks[taskId].data[4] = gSprites[spriteId].x2 = gBattleAnimArgs[1];
        gTasks[taskId].data[5] = gSprites[spriteId].y2 = gBattleAnimArgs[2];
        gTasks[taskId].func = AnimTask_ShakeMonStep;
        gTasks[taskId].func(taskId);
    }
}

static void AnimTask_ShakeMonStep(u8 taskId)
{
	// Wait num of frames
    if (gTasks[taskId].data[3] == 0)
    {
		u8 spriteId = gTasks[taskId].data[0];
		
		gSprites[spriteId].x2 = (gSprites[spriteId].x2 == 0) ? gTasks[taskId].data[4] : 0;
		gSprites[spriteId].y2 = (gSprites[spriteId].y2 == 0) ? gTasks[taskId].data[5] : 0;
        
        gTasks[taskId].data[3] = gTasks[taskId].data[2];
		
        if (--gTasks[taskId].data[1] == 0)
        {
            gSprites[spriteId].x2 = 0;
            gSprites[spriteId].y2 = 0;
			
            DestroyAnimVisualTask(taskId);
        }
    }
    else
        gTasks[taskId].data[3]--;
}

// Task to facilitate simple shaking of a pokemon's picture in battle.
// The shaking alternates between the positive and negative versions of the specified pixel offsets.
// arg 0: anim battler
// arg 1: x pixel offset
// arg 2: y pixel offset
// arg 3: num times to shake
// arg 4: frame delay
void AnimTask_ShakeMon2(u8 taskId)
{
    bool8 abort = FALSE;
    u8 battlerId, spriteId;

    if (gBattleAnimArgs[0] < MAX_BATTLERS_COUNT + 4)
    {
        spriteId = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);
		
        if (spriteId == 0xFF)
            abort = TRUE;
    }
    else
        spriteId = gBattlerSpriteIds[gBattleAnimAttacker]; // Get attacker's sprite id, ignoring visibility
	
    if (abort)
        DestroyAnimVisualTask(taskId);
    else
    {
        gSprites[spriteId].x2 = gBattleAnimArgs[1];
        gSprites[spriteId].y2 = gBattleAnimArgs[2];
		
        gTasks[taskId].data[0] = spriteId;
        gTasks[taskId].data[1] = gBattleAnimArgs[3];
        gTasks[taskId].data[2] = gBattleAnimArgs[4];
        gTasks[taskId].data[3] = gBattleAnimArgs[4];
        gTasks[taskId].data[4] = gBattleAnimArgs[1];
        gTasks[taskId].data[5] = gBattleAnimArgs[2];
        gTasks[taskId].func = AnimTask_ShakeMon2Step;
        gTasks[taskId].func(taskId);
    }
}

static void AnimTask_ShakeMon2Step(u8 taskId)
{
    if (gTasks[taskId].data[3] == 0)
    {
        if (gSprites[gTasks[taskId].data[0]].x2 == gTasks[taskId].data[4])
            gSprites[gTasks[taskId].data[0]].x2 = -gTasks[taskId].data[4];
        else
            gSprites[gTasks[taskId].data[0]].x2 = gTasks[taskId].data[4];
		
        if (gSprites[gTasks[taskId].data[0]].y2 == gTasks[taskId].data[5])
            gSprites[gTasks[taskId].data[0]].y2 = -gTasks[taskId].data[5];
        else
            gSprites[gTasks[taskId].data[0]].y2 = gTasks[taskId].data[5];
		
        gTasks[taskId].data[3] = gTasks[taskId].data[2];
		
        if (--gTasks[taskId].data[1] == 0)
        {
            gSprites[gTasks[taskId].data[0]].x2 = 0;
            gSprites[gTasks[taskId].data[0]].y2 = 0;
			
            DestroyAnimVisualTask(taskId);
        }
    }
    else
        gTasks[taskId].data[3]--;
}

// Task to facilitate simple shaking of a pokemon's picture in battle.
// The shaking alternates between the positive and negative versions of the specified pixel offsets
// with respect to the current location of the mon's picture.
// arg 0: battler
// arg 1: x offset
// arg 2: y offset
// arg 3: num shakes
// arg 4: delay
void AnimTask_ShakeMonInPlace(u8 taskId)
{
    u8 spriteId = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);

    if (spriteId == 0xFF)
        DestroyAnimVisualTask(taskId);
    else
    {
        gSprites[spriteId].x2 += gBattleAnimArgs[1];
        gSprites[spriteId].y2 += gBattleAnimArgs[2];
        gTasks[taskId].data[0] = spriteId;
        gTasks[taskId].data[1] = 0;
        gTasks[taskId].data[2] = gBattleAnimArgs[3];
        gTasks[taskId].data[3] = 0;
        gTasks[taskId].data[4] = gBattleAnimArgs[4];
        gTasks[taskId].data[5] = gBattleAnimArgs[1] * 2;
        gTasks[taskId].data[6] = gBattleAnimArgs[2] * 2;
        gTasks[taskId].func = AnimTask_ShakeMonInPlaceStep;
        gTasks[taskId].func(taskId);
    }
}

static void AnimTask_ShakeMonInPlaceStep(u8 taskId)
{
    if (gTasks[taskId].data[3] == 0)
    {
        if (gTasks[taskId].data[1] & 1)
        {
            gSprites[gTasks[taskId].data[0]].x2 += gTasks[taskId].data[5];
            gSprites[gTasks[taskId].data[0]].y2 += gTasks[taskId].data[6];
        }
        else
        {
            gSprites[gTasks[taskId].data[0]].x2 -= gTasks[taskId].data[5];
            gSprites[gTasks[taskId].data[0]].y2 -= gTasks[taskId].data[6];
        }
        gTasks[taskId].data[3] = gTasks[taskId].data[4];
        if (++gTasks[taskId].data[1] >= gTasks[taskId].data[2])
        {
            if (gTasks[taskId].data[1] & 1)
            {
                gSprites[gTasks[taskId].data[0]].x2 += gTasks[taskId].data[5] / 2;
                gSprites[gTasks[taskId].data[0]].y2 += gTasks[taskId].data[6] / 2;
            }
            else
            {
                gSprites[gTasks[taskId].data[0]].x2 -= gTasks[taskId].data[5] / 2;
                gSprites[gTasks[taskId].data[0]].y2 -= gTasks[taskId].data[6] / 2;
            }
            DestroyAnimVisualTask(taskId);
        }
    }
    else
        gTasks[taskId].data[3]--;
}

// Shakes a mon bg horizontally and moves it downward linearly.
// arg 0: battler
// arg 1: x offset
// arg 2: frame delay between each movement
// arg 3: downward speed (subpixel)
// arg 4: duration
void AnimTask_ShakeAndSinkMon(u8 taskId)
{
	u8 spriteId = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);
	
	if (spriteId == 0xFF)
		DestroyAnimVisualTask(taskId);
	else
	{
		gSprites[spriteId].x2 = gBattleAnimArgs[1];
		
		gTasks[taskId].data[0] = spriteId;
		gTasks[taskId].data[1] = gBattleAnimArgs[1];
		gTasks[taskId].data[2] = gBattleAnimArgs[2];
		gTasks[taskId].data[3] = gBattleAnimArgs[3];
		gTasks[taskId].data[4] = gBattleAnimArgs[4];
		gTasks[taskId].func = AnimTask_ShakeAndSinkMonStep;
		gTasks[taskId].func(taskId);
	}
}

static void AnimTask_ShakeAndSinkMonStep(u8 taskId)
{
    u8 spriteId = gTasks[taskId].data[0];
    s16 data1 = gTasks[taskId].data[1];
	
    if (gTasks[taskId].data[2] == gTasks[taskId].data[8]++)
    {
        gTasks[taskId].data[8] = 0;
		
        if (gSprites[spriteId].x2 == data1)
            data1 = -data1;
		
        gSprites[spriteId].x2 += data1;
    }
    gTasks[taskId].data[1] = data1;
    gTasks[taskId].data[9] += gTasks[taskId].data[3];
	
    gSprites[spriteId].y2 = gTasks[taskId].data[9] >> 8;
	
    if (--gTasks[taskId].data[4] == 0)
        DestroyAnimVisualTask(taskId);
}

// Moves a mon bg picture along an elliptical path that begins and ends at the mon's origin location.
// arg 0: battler
// arg 1: ellipse width
// arg 2: ellipse height
// arg 3: num loops
// arg 4: speed (valid values are 0-5)
void AnimTask_TranslateMonElliptical(u8 taskId)
{
    u8 i, wavePeriod = 1, spriteId = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);;
	
    if (gBattleAnimArgs[4] > 5)
        gBattleAnimArgs[4] = 5;
	
    for (i = 0; i < gBattleAnimArgs[4]; i++)
        wavePeriod *= 2;
	
    gTasks[taskId].data[0] = spriteId;
    gTasks[taskId].data[1] = gBattleAnimArgs[1];
    gTasks[taskId].data[2] = gBattleAnimArgs[2];
    gTasks[taskId].data[3] = gBattleAnimArgs[3];
    gTasks[taskId].data[4] = wavePeriod;
    gTasks[taskId].func = AnimTask_TranslateMonEllipticalStep;
    gTasks[taskId].func(taskId);
}

static void AnimTask_TranslateMonEllipticalStep(u8 taskId)
{
    u8 spriteId = gTasks[taskId].data[0];
	
    gSprites[spriteId].x2 = Sin(gTasks[taskId].data[5], gTasks[taskId].data[1]);
    gSprites[spriteId].y2 = -Cos(gTasks[taskId].data[5], gTasks[taskId].data[2]);
    gSprites[spriteId].y2 += gTasks[taskId].data[2];
	
    gTasks[taskId].data[5] += gTasks[taskId].data[4];
    gTasks[taskId].data[5] &= 0xFF;
    if (gTasks[taskId].data[5] == 0)
        gTasks[taskId].data[3]--;
	
    if (gTasks[taskId].data[3] == 0)
    {
        gSprites[spriteId].x2 = 0;
        gSprites[spriteId].y2 = 0;
        DestroyAnimVisualTask(taskId);
    }
}

// Moves a mon bg picture along an elliptical path that begins
// and ends at the mon's origin location. Reverses the direction
// of the path if it's not on the player's side of the battle.
// arg 0: battler
// arg 1: ellipse width
// arg 2: ellipse height
// arg 3: num loops
// arg 4: speed (valid values are 0-5)
void AnimTask_TranslateMonEllipticalRespectSide(u8 taskId)
{
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        gBattleAnimArgs[1] = -gBattleAnimArgs[1];
	
    AnimTask_TranslateMonElliptical(taskId);
}

// Performs a simple horizontal lunge, where the mon moves horizontally, and then moves back in the opposite direction.
// arg 0: duration of single lunge direction
// arg 1: x pixel delta that is applied each frame
static void DoHorizontalLunge(struct Sprite * sprite)
{
    sprite->invisible = TRUE;
	
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        gBattleAnimArgs[1] = -gBattleAnimArgs[1];
	
    sprite->data[0] = gBattleAnimArgs[0];
	sprite->data[1] = gBattleAnimArgs[1];
    sprite->data[2] = 0;
    sprite->data[3] = gBattlerSpriteIds[gBattleAnimAttacker];
    sprite->data[4] = gBattleAnimArgs[0];
	
    StoreSpriteCallbackInData6(sprite, ReverseHorizontalLungeDirection);
    sprite->callback = TranslateMonSpriteLinear;
}

static void ReverseHorizontalLungeDirection(struct Sprite * sprite)
{
    sprite->data[0] = sprite->data[4];
    sprite->data[1] = -sprite->data[1];
    sprite->callback = TranslateMonSpriteLinear;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Performs a simple horizontal lunge, where the mon moves horizontally to its partner's offset.
// arg 0: x pixel delta that is applied each frame
// arg 1: target x pixel offset
// arg 2: slide in (boolean)
static void SlideMonToPartnerOffsetAndBack(struct Sprite * sprite)
{
	s16 xSlide;
	u8 spriteId = gBattlerSpriteIds[gBattleAnimAttacker];
	
	sprite->invisible = TRUE;
	
	if ((GetBattlerPosition(gBattleAnimAttacker) & BIT_FLANK) != B_FLANK_LEFT)
		gBattleAnimArgs[0] = -gBattleAnimArgs[0];
	
	if (gBattleAnimArgs[2])
		gSprites[spriteId].oam.priority--; // Above partner's sprite
	
	xSlide = gSprites[gBattlerSpriteIds[gBattleAnimTarget]].x - gSprites[spriteId].x;
	if (xSlide < 0)
		xSlide *= -1;
	
	sprite->data[0] = xSlide - 32 + gBattleAnimArgs[1];
	sprite->data[1] = gBattleAnimArgs[0];
    sprite->data[2] = 0;
    sprite->data[3] = spriteId;
	
	StoreSpriteCallbackInData6(sprite, gBattleAnimArgs[2] ? DestroyAnimSprite : SlideMonToPartnerOffsetAndBackStep);
    sprite->callback = TranslateMonSpriteLinear;
}

static void SlideMonToPartnerOffsetAndBackStep(struct Sprite * sprite)
{
	gSprites[sprite->data[3]].oam.priority++; // Restore priority when movement back ends
	DestroyAnimSprite(sprite);
}

// Performs a simple vertical dipping motion, where moves vertically, and then moves back in the opposite direction.
// arg 0: duration of single dip direction
// arg 1: y pixel delta that is applied each frame
// arg 2: anim battler
static void DoVerticalDip(struct Sprite * sprite)
{
    sprite->invisible = TRUE;

    sprite->data[0] = gBattleAnimArgs[0];
    sprite->data[1] = 0;
    sprite->data[2] = gBattleAnimArgs[1];
    sprite->data[3] = GetAnimBattlerSpriteId(gBattleAnimArgs[2]);
    sprite->data[4] = gBattleAnimArgs[0];
	
    StoreSpriteCallbackInData6(sprite, ReverseVerticalDipDirection);
    sprite->callback = TranslateMonSpriteLinear;
}

static void ReverseVerticalDipDirection(struct Sprite * sprite)
{
    sprite->data[0] = sprite->data[4];
    sprite->data[2] = -sprite->data[2];
    sprite->callback = TranslateMonSpriteLinear;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Linearly slides a mon's bg picture back to its original sprite position.
// The sprite parameter is a dummy sprite used for facilitating the movement with its callback.
// arg 0: anim battler
// arg 1: direction (0 = horizontal and vertical, 1 = horizontal only, 2 = vertical only)
// arg 2: duration
static void SlideMonToOriginalPos(struct Sprite * sprite)
{
    u8 spriteId = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);
	
	if (spriteId == 0xFF)
		DestroyAnimSprite(sprite);
	else
	{
		sprite->data[0] = gBattleAnimArgs[2];
		sprite->data[1] = gSprites[spriteId].x + gSprites[spriteId].x2;
		sprite->data[2] = gSprites[spriteId].x;
		sprite->data[3] = gSprites[spriteId].y + gSprites[spriteId].y2;
		sprite->data[4] = gSprites[spriteId].y;
		InitSpriteDataForLinearTranslation(sprite);
		sprite->data[3] = 0;
		sprite->data[4] = 0;
		sprite->data[5] = gSprites[spriteId].x2;
		sprite->data[6] = gSprites[spriteId].y2;
		sprite->invisible = TRUE;
		
		if (gBattleAnimArgs[1] == 1)
			sprite->data[2] = 0;
		else if (gBattleAnimArgs[1] == 2)
			sprite->data[1] = 0;
		
		sprite->data[7] = gBattleAnimArgs[1];
		sprite->data[7] |= spriteId << 8;
		sprite->callback = SlideMonToOriginalPosStep;
	}
}

static void SlideMonToOriginalPosStep(struct Sprite * sprite)
{
    u8 data7 = sprite->data[7];
    struct Sprite *otherSprite = &gSprites[sprite->data[7] >> 8];
	
    if (sprite->data[0] == 0)
    {
        if (data7 == 1 || data7 == 0)
            otherSprite->x2 = 0;
        if (data7 == 2 || data7 == 0)
            otherSprite->y2 = 0;
		
        DestroyAnimSprite(sprite);
    }
    else
    {
        sprite->data[0]--;
		
        sprite->data[3] += sprite->data[1];
        sprite->data[4] += sprite->data[2];
		
        otherSprite->x2 = (sprite->data[3] >> 8) + sprite->data[5];
        otherSprite->y2 = (sprite->data[4] >> 8) + sprite->data[6];
    }
}

// Linearly translates a mon to a target offset. The horizontal offset
// is mirrored for the opponent's pokemon, and the vertical offset
// is only mirrored if arg 3 is set to TRUE.
// arg 0: anim battler
// arg 1: target x pixel offset
// arg 2: target y pixel offset
// arg 3: mirror vertical translation for opposite battle side (boolean)
// arg 4: duration
static void SlideMonToOffset(struct Sprite * sprite)
{
    u8 battlerId = GetBattlerForAnimScript(gBattleAnimArgs[0]);
    u8 spriteId = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);
	
	sprite->invisible = TRUE;
	
    if (GetBattlerSide(battlerId) != B_SIDE_PLAYER)
    {
        gBattleAnimArgs[1] = -gBattleAnimArgs[1];
		
        if (gBattleAnimArgs[3])
            gBattleAnimArgs[2] = -gBattleAnimArgs[2];
    }
    sprite->data[0] = gBattleAnimArgs[4];
    sprite->data[1] = gSprites[spriteId].x;
    sprite->data[2] = gSprites[spriteId].x + gBattleAnimArgs[1];
    sprite->data[3] = gSprites[spriteId].y;
    sprite->data[4] = gSprites[spriteId].y + gBattleAnimArgs[2];
    InitSpriteDataForLinearTranslation(sprite);
	
    sprite->data[3] = 0;
    sprite->data[4] = 0;
    sprite->data[5] = spriteId;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
    sprite->callback = TranslateMonSpriteLinearFixedPoint;
}

// Linearly translates a mon to a target offset and then back. The horizontal offset
// is mirrored for the opponent's pokemon, and the vertical offset
// is only mirrored if arg 3 is set to TRUE.
// arg 0: anim battler
// arg 1: target x pixel offset
// arg 2: target y pixel offset
// arg 3: mirror vertical translation for opposite battle side (boolean)
// arg 4: duration
// arg 5: dont reset position on final anim (boolean)
static void SlideMonToOffsetAndBack(struct Sprite * sprite)
{
    u8 battlerId = GetBattlerForAnimScript(gBattleAnimArgs[0]);
    u8 spriteId = gBattlerSpriteIds[battlerId];
	
    sprite->invisible = TRUE;
    
    if (GetBattlerSide(battlerId) != B_SIDE_PLAYER)
    {
        gBattleAnimArgs[1] = -gBattleAnimArgs[1];
		
        if (gBattleAnimArgs[3])
            gBattleAnimArgs[2] = -gBattleAnimArgs[2];
    }
    sprite->data[0] = gBattleAnimArgs[4];
    sprite->data[1] = gSprites[spriteId].x + gSprites[spriteId].x2;
    sprite->data[2] = sprite->data[1] + gBattleAnimArgs[1];
    sprite->data[3] = gSprites[spriteId].y + gSprites[spriteId].y2;
    sprite->data[4] = sprite->data[3] + gBattleAnimArgs[2];
    InitSpriteDataForLinearTranslation(sprite);
	
    sprite->data[3] = gSprites[spriteId].x2 << 8;
    sprite->data[4] = gSprites[spriteId].y2 << 8;
    sprite->data[5] = spriteId;
    sprite->data[6] = gBattleAnimArgs[5];
	StoreSpriteCallbackInData6(sprite, gBattleAnimArgs[5] ? SlideMonToOffsetAndBackStep : DestroyAnimSprite);
    sprite->callback = TranslateMonSpriteLinearFixedPoint;
}

static void SlideMonToOffsetAndBackStep(struct Sprite * sprite)
{
    gSprites[sprite->data[5]].x2 = 0;
    gSprites[sprite->data[5]].y2 = 0;
    DestroyAnimSprite(sprite);
}

// Task to facilitate a two-part translation animation, in which the sprite
// is first translated in an arc to one position.  Then, it "lunges" to a target
// x offset.  Used in TAKE_DOWN, for example.
// arg 0: anim bank
// arg 1: horizontal speed (subpixel)
// arg 2: wave amplitude
// arg 3: first duration
// arg 4: delay before starting lunge
// arg 5: target x offset for lunge
// arg 6: lunge duration
void AnimTask_WindUpLunge(u8 taskId)
{
    u16 wavePeriod = 0x8000 / gBattleAnimArgs[3];
	
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
    {
        gBattleAnimArgs[1] = -gBattleAnimArgs[1];
        gBattleAnimArgs[5] = -gBattleAnimArgs[5];
    }
    gTasks[taskId].data[0] = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);
    gTasks[taskId].data[1] = gBattleAnimArgs[1] * 256 / gBattleAnimArgs[3];
    gTasks[taskId].data[2] = gBattleAnimArgs[2];
    gTasks[taskId].data[3] = gBattleAnimArgs[3];
    gTasks[taskId].data[4] = gBattleAnimArgs[4];
    gTasks[taskId].data[5] = gBattleAnimArgs[5] * 256 / gBattleAnimArgs[6];
    gTasks[taskId].data[6] = gBattleAnimArgs[6];
    gTasks[taskId].data[7] = wavePeriod;
    gTasks[taskId].func = AnimTask_WindUpLungePart1;
}

static void AnimTask_WindUpLungePart1(u8 taskId)
{
    u8 spriteId = gTasks[taskId].data[0];
    gTasks[taskId].data[11] += gTasks[taskId].data[1];
    gSprites[spriteId].x2 = gTasks[taskId].data[11] >> 8;
    gSprites[spriteId].y2 = Sin((u8)(gTasks[taskId].data[10] >> 8), gTasks[taskId].data[2]);
    gTasks[taskId].data[10] += gTasks[taskId].data[7];
    if (--gTasks[taskId].data[3] == 0)
        gTasks[taskId].func = AnimTask_WindUpLungePart2;
}

static void AnimTask_WindUpLungePart2(u8 taskId)
{
    if (gTasks[taskId].data[4] > 0)
        gTasks[taskId].data[4]--;
    else
    {
        gTasks[taskId].data[12] += gTasks[taskId].data[5];
        gSprites[gTasks[taskId].data[0]].x2 = (gTasks[taskId].data[12] >> 8) + (gTasks[taskId].data[11] >> 8);
		
        if (--gTasks[taskId].data[6] == 0)
            DestroyAnimVisualTask(taskId);
    }
}

// Animates the given battler sliding to offscreen.
// arg 0: anim battler
// arg 1: slide speed
void AnimTask_SlideOffScreen(u8 taskId)
{
    u8 spriteId;
	
    switch (gBattleAnimArgs[0])
    {
	case ANIM_ATK_PARTNER:
	case ANIM_DEF_PARTNER:
		if (!IsBattlerSpriteVisible(GetBattlerForAnimScript(gBattleAnimArgs[0])))
			goto NO_SLIDE;
		
		// Fallthrough
    case ANIM_ATTACKER:
    case ANIM_TARGET:
        spriteId = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);
        break;
    default:
	NO_SLIDE:
        DestroyAnimVisualTask(taskId);
        return;
    }
    gTasks[taskId].data[0] = spriteId;
	gTasks[taskId].data[1] = GetBattlerSide(gBattleAnimTarget) != B_SIDE_PLAYER ? gBattleAnimArgs[1] : -gBattleAnimArgs[1];
    gTasks[taskId].func = AnimTask_SlideOffScreen_Step;
}

static void AnimTask_SlideOffScreen_Step(u8 taskId)
{
    u8 spriteId = gTasks[taskId].data[0];
    gSprites[spriteId].x2 += gTasks[taskId].data[1];
    if (gSprites[spriteId].x2 + gSprites[spriteId].x < -0x20 || gSprites[spriteId].x2 + gSprites[spriteId].x > 0x110)
        DestroyAnimVisualTask(taskId);
}

// Task that facilitates translating the mon bg picture back and forth
// in a swaying motion (uses Sine wave). It can sway either horizontally
// or vertically, but not both.
// arg 0: direction (0 = horizontal, 1 = vertical)
// arg 1: wave amplitude
// arg 2: wave period
// arg 3: num sways
// arg 4: which mon
void AnimTask_SwayMon(u8 taskId)
{
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        gBattleAnimArgs[1] = -gBattleAnimArgs[1];
	
    gTasks[taskId].data[0] = gBattleAnimArgs[0];
    gTasks[taskId].data[1] = gBattleAnimArgs[1];
    gTasks[taskId].data[2] = gBattleAnimArgs[2];
    gTasks[taskId].data[3] = gBattleAnimArgs[3];
    gTasks[taskId].data[4] = GetAnimBattlerSpriteId(gBattleAnimArgs[4]);
	gTasks[taskId].data[5] = GetBattlerForAnimScript(gBattleAnimArgs[4]);
    gTasks[taskId].data[12] = 1;
    gTasks[taskId].func = AnimTask_SwayMonStep;
}

static void AnimTask_SwayMonStep(u8 taskId)
{
    u8 spriteId = gTasks[taskId].data[4];
	u16 sineIndex = gTasks[taskId].data[10] + gTasks[taskId].data[2];
    u32 waveIndex = sineIndex >> 8;
    s16 sineValue = Sin(waveIndex, gTasks[taskId].data[1]);

    gTasks[taskId].data[10] = sineIndex;

    if (gTasks[taskId].data[0] == 0)
        gSprites[spriteId].x2 = sineValue;
    else if (GetBattlerSide(gTasks[taskId].data[5]) == B_SIDE_PLAYER)
        gSprites[spriteId].y2 = abs(sineValue);
    else
        gSprites[spriteId].y2 = -abs(sineValue);
	
    if ((waveIndex > 0x7F && gTasks[taskId].data[11] == 0 && gTasks[taskId].data[12] == 1) || (waveIndex < 0x7F && gTasks[taskId].data[11] == 1 && gTasks[taskId].data[12] == 0))
    {
        gTasks[taskId].data[11] ^= 1;
        gTasks[taskId].data[12] ^= 1;
		
        if (--gTasks[taskId].data[3] == 0)
        {
            gSprites[spriteId].x2 = 0;
            gSprites[spriteId].y2 = 0;
            DestroyAnimVisualTask(taskId);
        }
    }
}

// Scales a mon's sprite, and then scales back to its original dimensions.
// arg 0: x scale delta
// arg 1: y scale delta
// arg 2: duration
// arg 3: anim bank
// arg 4: sprite object mode
void AnimTask_ScaleMonAndRestore(u8 taskId)
{
    u8 spriteId = GetAnimBattlerSpriteId(gBattleAnimArgs[3]);
    PrepareBattlerSpriteForRotScale(spriteId, gBattleAnimArgs[4]);
    gTasks[taskId].data[0] = gBattleAnimArgs[0];
    gTasks[taskId].data[1] = gBattleAnimArgs[1];
    gTasks[taskId].data[2] = gBattleAnimArgs[2];
    gTasks[taskId].data[3] = gBattleAnimArgs[2];
    gTasks[taskId].data[4] = spriteId;
    gTasks[taskId].data[10] = 0x100;
    gTasks[taskId].data[11] = 0x100;
    gTasks[taskId].func = AnimTask_ScaleMonAndRestoreStep;
}

static void AnimTask_ScaleMonAndRestoreStep(u8 taskId)
{
    u8 spriteId;
	
    gTasks[taskId].data[10] += gTasks[taskId].data[0];
    gTasks[taskId].data[11] += gTasks[taskId].data[1];
	
    spriteId = gTasks[taskId].data[4];
    SetSpriteRotScale(spriteId, gTasks[taskId].data[10], gTasks[taskId].data[11], 0);
	
    if (--gTasks[taskId].data[2] == 0)
    {
        if (gTasks[taskId].data[3] > 0)
        {
            gTasks[taskId].data[0] = -gTasks[taskId].data[0];
            gTasks[taskId].data[1] = -gTasks[taskId].data[1];
            gTasks[taskId].data[2] = gTasks[taskId].data[3];
            gTasks[taskId].data[3] = 0;
        }
        else
        {
            ResetSpriteRotScale(spriteId);
            DestroyAnimVisualTask(taskId);
        }
    }
}

// Rotates mon to side for duration.
// arg 0: duration
// arg 1: rotation increment value
// arg 2: anim battler
// arg 3: final state
void AnimTask_RotateMonSpriteToSide(u8 taskId)
{
    u8 spriteId = GetAnimBattlerSpriteId(gBattleAnimArgs[2]);
	
    PrepareBattlerSpriteForRotScale(spriteId, ST_OAM_OBJ_NORMAL);
	
    gTasks[taskId].data[1] = 0;
    gTasks[taskId].data[2] = gBattleAnimArgs[0];
	
	gTasks[taskId].data[3] = gBattleAnimArgs[3] != 1 ? 0 : (gBattleAnimArgs[0] * gBattleAnimArgs[1]);
    gTasks[taskId].data[4] = gBattleAnimArgs[1];
    gTasks[taskId].data[5] = spriteId;
    gTasks[taskId].data[6] = gBattleAnimArgs[3];
	gTasks[taskId].data[7] = GetBattlerSide(GetBattlerForAnimScript(gBattleAnimArgs[2])) == B_SIDE_PLAYER ? 1 : 0;
	
    if (gTasks[taskId].data[7])
    {
        gTasks[taskId].data[3] = -gTasks[taskId].data[3];
        gTasks[taskId].data[4] = -gTasks[taskId].data[4];
    }
    gTasks[taskId].func = AnimTask_RotateMonSpriteToSide_Step;
}

// Rotates mon to side and back to original position. For Peck and when a held item activates.
// arg 0: duration
// arg 1: rotation increment value
// arg 2: anim battler
// arg 3: final state
void AnimTask_RotateMonToSideAndRestore(u8 taskId)
{
    u8 spriteId = GetAnimBattlerSpriteId(gBattleAnimArgs[2]);
	
    PrepareBattlerSpriteForRotScale(spriteId, ST_OAM_OBJ_NORMAL);
	
    gTasks[taskId].data[1] = 0;
    gTasks[taskId].data[2] = gBattleAnimArgs[0];
	
    if (GetBattlerSide(GetBattlerForAnimScript(gBattleAnimArgs[2])) != B_SIDE_PLAYER)
        gBattleAnimArgs[1] = -gBattleAnimArgs[1];
    
	gTasks[taskId].data[3] = gBattleAnimArgs[3] != 1 ? 0 : -(gBattleAnimArgs[0] * gBattleAnimArgs[1]);
    gTasks[taskId].data[4] = -gBattleAnimArgs[1];
    gTasks[taskId].data[5] = spriteId;
    gTasks[taskId].data[6] = gBattleAnimArgs[3];
    gTasks[taskId].data[7] = 1;
	
    gTasks[taskId].func = AnimTask_RotateMonSpriteToSide_Step;
}

static void AnimTask_RotateMonSpriteToSide_Step(u8 taskId)
{
    gTasks[taskId].data[3] += gTasks[taskId].data[4];
	
    SetSpriteRotScale(gTasks[taskId].data[5], 0x100, 0x100, gTasks[taskId].data[3]);
	
    if (gTasks[taskId].data[7])
        SetBattlerSpriteYOffsetFromRotation(gTasks[taskId].data[5]);
	
    if (++gTasks[taskId].data[1] >= gTasks[taskId].data[2])
    {
        switch (gTasks[taskId].data[6])
        {
        case 1:
            ResetSpriteRotScale(gTasks[taskId].data[5]);
            // fallthrough
        case 0:
        default:
            DestroyAnimVisualTask(taskId);
            break;
        case 2:
            gTasks[taskId].data[1] = 0;
            gTasks[taskId].data[4] *= -1;
            gTasks[taskId].data[6] = 1;
            break;
        }
    }
}

// Shakes the battler based on the move's power or damage.
// arg 0: shake based on damage (boolean)
// arg 1: shake delay
// arg 2: num shakes
// arg 3: shake horizontally (boolean)
// arg 4: shake vertically (boolean)
void AnimTask_ShakeTargetBasedOnMovePowerOrDmg(u8 taskId)
{
	gTasks[taskId].data[15] = gBattleAnimArgs[0] ? gAnimMoveDmg / 12 : gAnimMovePower / 12;

	if (gTasks[taskId].data[15] < 1)
		gTasks[taskId].data[15] = 1;
    else if (gTasks[taskId].data[15] > 16)
		gTasks[taskId].data[15] = 16;
		
    gTasks[taskId].data[14] = gTasks[taskId].data[15] / 2;
    gTasks[taskId].data[13] = gTasks[taskId].data[14] + (gTasks[taskId].data[15] & 1);
    gTasks[taskId].data[12] = 0;
    gTasks[taskId].data[10] = gBattleAnimArgs[3];
    gTasks[taskId].data[11] = gBattleAnimArgs[4];
    gTasks[taskId].data[7] = GetAnimBattlerSpriteId(ANIM_TARGET);
    gTasks[taskId].data[8] = gSprites[gTasks[taskId].data[7]].x2;
    gTasks[taskId].data[9] = gSprites[gTasks[taskId].data[7]].y2;
    gTasks[taskId].data[0] = 0;
    gTasks[taskId].data[1] = gBattleAnimArgs[1];
    gTasks[taskId].data[2] = gBattleAnimArgs[2];
    gTasks[taskId].func = AnimTask_ShakeTargetBasedOnMovePowerOrDmgStep;
}

static void AnimTask_ShakeTargetBasedOnMovePowerOrDmgStep(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
	
    if (++task->data[0] > task->data[1])
    {
        task->data[0] = 0;
        task->data[12] = (task->data[12] + 1) & 1;
		
        if (task->data[10])
        {
            if (task->data[12])
                gSprites[task->data[7]].x2 = task->data[8] + task->data[13];
            else
                gSprites[task->data[7]].x2 = task->data[8] - task->data[14];
        }
		
        if (task->data[11])
        {
            if (task->data[12])
                gSprites[task->data[7]].y2 = task->data[15];
            else
                gSprites[task->data[7]].y2 = 0;
        }
		
        if (--task->data[2] == 0)
        {
            gSprites[task->data[7]].x2 = 0;
            gSprites[task->data[7]].y2 = 0;
            DestroyAnimVisualTask(taskId);
            return;
        }
    }
}

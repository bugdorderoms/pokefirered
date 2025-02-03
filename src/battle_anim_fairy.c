#include "global.h"
#include "battle_anim.h"
#include "gflib.h"
#include "graphics.h"
#include "random.h"
#include "trig.h"
#include "util.h"

static void AnimDevil(struct Sprite *sprite);
static void AnimPinkHeart(struct Sprite *sprite);
static void AnimPinkHeart_Step(struct Sprite *sprite);
static void AnimAngel(struct Sprite *sprite);
static void AnimParticuleBurst(struct Sprite *sprite);
static void AnimMagentaHeart(struct Sprite *sprite);
static void HeartsBackground_Step(u8);
static void AnimMoon(struct Sprite* sprite);
static void AnimMoonStep(struct Sprite *);
static void AnimTask_AlphaFadeIn_Step(u8 taskId);
static void AnimMoonlightSparkle(struct Sprite *);
static void AnimMoonlightSparkleStep(struct Sprite *);
static void AnimTask_FadeScreenBlueStep(u8 taskId);

static const union AnimCmd sDevilAnimCmds1[] =
{
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sDevilAnimCmds2[] =
{
    ANIMCMD_FRAME(16, 3),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sDevilAnimTable[] =
{
    sDevilAnimCmds1,
    sDevilAnimCmds2,
};

const struct SpriteTemplate gDevilSpriteTemplate =
{
    .tileTag = ANIM_TAG_DEVIL,
    .paletteTag = ANIM_TAG_DEVIL,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sDevilAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimDevil,
};

const struct SpriteTemplate gPinkHeartSpriteTemplate =    
{
    .tileTag = ANIM_TAG_PINK_HEART,
    .paletteTag = ANIM_TAG_PINK_HEART,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimPinkHeart,
};

static const union AnimCmd sAngelSpriteAnimCmds[] =
{
    ANIMCMD_FRAME(0, 24),
    ANIMCMD_END,
};

static const union AnimCmd *const sAngelSpriteAnimTable[] =
{
    sAngelSpriteAnimCmds,
};

const struct SpriteTemplate gAngelSpriteTemplate =
{
    .tileTag = ANIM_TAG_ANGEL,
    .paletteTag = ANIM_TAG_ANGEL,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sAngelSpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimAngel,
};

const struct SpriteTemplate gRedHeartBurstSpriteTemplate =
{
    .tileTag = ANIM_TAG_RED_HEART,
    .paletteTag = ANIM_TAG_RED_HEART,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimParticuleBurst,
};

const struct SpriteTemplate gMagentaHeartSpriteTemplate =
{
    .tileTag = ANIM_TAG_MAGENTA_HEART,
    .paletteTag = ANIM_TAG_MAGENTA_HEART,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimMagentaHeart,
};

const struct SpriteTemplate gMoonSpriteTemplate =
{
    .tileTag = ANIM_TAG_MOON,
    .paletteTag = ANIM_TAG_MOON,
    .oam = &gOamData_AffineOff_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimMoon,
};

static const union AnimCmd sMoonlightSparkleAnimCmds[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(4, 8),
    ANIMCMD_FRAME(8, 8),
    ANIMCMD_FRAME(12, 8),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sMoonlightSparkleAnimTable[] =
{
    sMoonlightSparkleAnimCmds,
};

const struct SpriteTemplate gMoonlightSparkleSpriteTemplate =
{
    .tileTag = ANIM_TAG_GREEN_SPARKLE,
    .paletteTag = ANIM_TAG_GREEN_SPARKLE,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = sMoonlightSparkleAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimMoonlightSparkle,
};

// Animates the devil sprite fluctuating beyond the target in Lovely Kiss's anim.
// arg 0: x pos
// arg 1: y pos
void AnimDevil(struct Sprite *sprite)
{
	switch (sprite->data[3])
	{
		case 0:
			StartSpriteAnim(sprite, 0);
			
			sprite->x += gBattleAnimArgs[0];
			sprite->y += gBattleAnimArgs[1];
			
			sprite->subpriority = GetBattlerSpriteSubpriority(gBattleAnimTarget) - 1;
			
			sprite->data[2] = 1;
			// Fallthrough
		default:
			sprite->data[0] += sprite->data[2];
			sprite->data[1] = (sprite->data[0] * 4) % 256;
			
			if (sprite->data[1] < 0)
				sprite->data[1] = 0;
			
			sprite->x2 = Cos(sprite->data[1], 30 - sprite->data[0] / 4);
			sprite->y2 = Sin(sprite->data[1], 10 - sprite->data[0] / 8);
			
			if (sprite->data[1] > 128 && sprite->data[2] > 0)
				sprite->data[2] = -1;
			
			if (sprite->data[1] == 0 && sprite->data[2] < 0)
				sprite->data[2] = 1;
			
			sprite->data[3]++;
			
			if (sprite->data[3] < 10 || sprite->data[3] > 80)
				sprite->invisible = sprite->data[0] % 2;
			else
				sprite->invisible = FALSE;
			
			if (sprite->data[3] > 90)
				DestroyAnimSprite(sprite);
			
			break;
	}
}

// Animates the heart sprites on move Lovely Kiss's animation.
// arg 0: x slide amount
// arg 1: amplitude
static void AnimPinkHeart(struct Sprite *sprite)
{
	switch (sprite->data[0])
	{
		case 0:
			sprite->data[1] = gBattleAnimArgs[0];
			sprite->data[2] = gBattleAnimArgs[1];
			sprite->data[0]++;
			break;
		case 1:
			sprite->data[4] += sprite->data[1];
			sprite->x2 = sprite->data[4] >> 8;
			
			sprite->y2 = Sin(sprite->data[3], sprite->data[2]);
			sprite->data[3] = (sprite->data[3] + 3) & 0xFF;
			
			if (sprite->data[3] > 70)
			{
				SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
				
				sprite->data[3] = RandomMax(180);
				sprite->callback = AnimPinkHeart_Step;
			}
	}
}

static void AnimPinkHeart_Step(struct Sprite *sprite)
{
    sprite->data[5]++;
	
    sprite->x2 = Sin(sprite->data[3], 5);
    sprite->y2 = sprite->data[5] / 2;
	
    sprite->data[3] = (sprite->data[3] + 3) & 0xFF;
	
    if (sprite->data[5] > 20)
        sprite->invisible = sprite->data[5] % 2;

    if (sprite->data[5] > 30)
        DestroyAnimSprite(sprite);
}

// Animates MOVE_SWEET_KISS's angel sprite.
// arg 0: x offset
// arg 1: y offset
static void AnimAngel(struct Sprite *sprite)
{
    s16 var0;
    
    if (!sprite->data[0])
    {
        sprite->x += gBattleAnimArgs[0];
        sprite->y += gBattleAnimArgs[1];
    }
    sprite->data[0]++;
	
    var0 = (sprite->data[0] * 10) & 0xFF;
    sprite->x2 = Sin(var0, 80) >> 8;
	
    if (sprite->data[0] < 80)
        sprite->y2 = (sprite->data[0] / 2) + (Cos(var0, 80) >> 8);

    if (sprite->data[0] > 90)
    {
        sprite->data[2]++;
        sprite->x2 -= sprite->data[2] / 2;
    }

    if (sprite->data[0] > 100)
        DestroyAnimSprite(sprite);
}

// Animates MOVE_SWEET_KISS's hearts burst.
// arg 0: x movement speed
// arg 1: wave amplitude
static void AnimParticuleBurst(struct Sprite *sprite)
{
    if (sprite->data[0] == 0)
    {
        sprite->data[1] = gBattleAnimArgs[0];
        sprite->data[2] = gBattleAnimArgs[1];
        sprite->data[0]++;
    }
    else
    {
        sprite->data[4] += sprite->data[1];
		
        sprite->x2 = sprite->data[4] >> 8;
        sprite->y2 = Sin(sprite->data[3], sprite->data[2]);
		
        sprite->data[3] = (sprite->data[3] + 3) & 0xFF;
        if (sprite->data[3] > 100)
            sprite->invisible = sprite->data[3] % 2;

        if (sprite->data[3] > 120)
            DestroyAnimSprite(sprite);
    }
}

// Animates hearts flying above the player's head.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
static void AnimMagentaHeart(struct Sprite *sprite)
{
    if (++sprite->data[0] == 1)
        InitSpritePosToAnimAttacker(sprite, FALSE);

    sprite->x2 = Sin(sprite->data[1], 8);
    sprite->y2 = sprite->data[2] >> 8;
	
    sprite->data[1] = (sprite->data[1] + 7) & 0xFF;
    sprite->data[2] -= 0x80;
	
    if (sprite->data[0] == 60)
        DestroyAnimSprite(sprite);
}

// Fades to the hearts background. Unsed by MOVE_ATTRACT.
// No args.
void AnimTask_HeartsBackground(u8 taskId)
{
    struct BattleAnimBgData animBg;

    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 16));
	
    SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 3);
    SetAnimBgAttribute(1, BG_ANIM_SCREEN_SIZE, 0);
    SetAnimBgAttribute(1, BG_ANIM_CHAR_BASE_BLOCK, 1);
	
    gBattle_BG1_X = 0;
    gBattle_BG1_Y = 0;
	
    SetGpuReg(REG_OFFSET_BG1HOFS, gBattle_BG1_X);
    SetGpuReg(REG_OFFSET_BG1VOFS, gBattle_BG1_Y);
	
    GetBattleAnimBgData(&animBg, 1);
    AnimLoadCompressedBgTilemap(animBg.bgId, gBattleAnimBg_AttractTilemap);
    AnimLoadCompressedBgGfx(animBg.bgId, gBattleAnimBg_AttractGfx, animBg.tilesOffset);
    LoadCompressedPalette(gBattleAnimBg_AttractPal, animBg.paletteId * 16, 32);
	
    gTasks[taskId].func = HeartsBackground_Step;
}

static void HeartsBackground_Step(u8 taskId)
{
    struct BattleAnimBgData animBg;

    switch (gTasks[taskId].data[12])
    {
    case 0:
        if (++gTasks[taskId].data[10] == 4)
        {
            gTasks[taskId].data[10] = 0;
			
            gTasks[taskId].data[11]++;
			
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[11], 16 - gTasks[taskId].data[11]));
			
            if (gTasks[taskId].data[11] == 16)
            {
				gTasks[taskId].data[11] = 0;
                gTasks[taskId].data[12]++;
            }
        }
        break;
    case 1:
        if (++gTasks[taskId].data[11] == 141)
        {
            gTasks[taskId].data[11] = 16;
            gTasks[taskId].data[12]++;
        }
        break;
    case 2:
        if (++gTasks[taskId].data[10] == 4)
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
        InitBattleAnimBg(animBg.bgId);
        gTasks[taskId].data[12]++;
        break;
    case 4:
        SetAnimBgAttribute(1, BG_ANIM_CHAR_BASE_BLOCK, 0);
        SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
		
        DestroyAnimVisualTaskAndDisableBlend(taskId);
        break;
    }
}

// Animates MOVE_MOONLIGHT's moon sprite.
// arg 0: x pos
// arg 1: y pos
static void AnimMoon(struct Sprite* sprite)
{
    sprite->x = gBattleAnimArgs[0];
    sprite->y = gBattleAnimArgs[1];
	
    sprite->oam.shape = SPRITE_SHAPE(8x8);
    sprite->oam.size = SPRITE_SIZE(64x32);
	
    sprite->data[0] = 0;
    sprite->callback = AnimMoonStep;
}

static void AnimMoonStep(struct Sprite* sprite)
{
    if (sprite->data[0])
        DestroyAnimSprite(sprite);
}

// Used to fade Moonlight's moon sprite in.
// arg 0: initial blend A coefficient
// arg 1: initial blend B coefficient
// arg 2: target blend A coefficient
// arg 3: target blend B coefficient
// arg 4: fade delay
void AnimTask_AlphaFadeIn(u8 taskId)
{
    s16 v1 = 0, v2 = 0;

    if (gBattleAnimArgs[2] > gBattleAnimArgs[0])
        v2 = 1;
	
    if (gBattleAnimArgs[2] < gBattleAnimArgs[0])
        v2 = -1;
	
    if (gBattleAnimArgs[3] > gBattleAnimArgs[1])
        v1 = 1;
	
    if (gBattleAnimArgs[3] < gBattleAnimArgs[1])
        v1 = -1;
	
    gTasks[taskId].data[0] = 0;
    gTasks[taskId].data[1] = gBattleAnimArgs[4];
    gTasks[taskId].data[2] = 0;
    gTasks[taskId].data[3] = gBattleAnimArgs[0];
    gTasks[taskId].data[4] = gBattleAnimArgs[1];
    gTasks[taskId].data[5] = v2;
    gTasks[taskId].data[6] = v1;
    gTasks[taskId].data[7] = gBattleAnimArgs[2];
    gTasks[taskId].data[8] = gBattleAnimArgs[3];
	
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gBattleAnimArgs[0], gBattleAnimArgs[1]));
	
    gTasks[taskId].func = AnimTask_AlphaFadeIn_Step;
}

static void AnimTask_AlphaFadeIn_Step(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    if (++task->data[0] > task->data[1])
    {
        task->data[0] = 0;
		
        if (++task->data[2] & 1)
        {
            if (task->data[3] != task->data[7])
                task->data[3] += task->data[5];
        }
        else
        {
            if (task->data[4] != task->data[8])
                task->data[4] += task->data[6];
        }
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(task->data[3], task->data[4]));
		
        if (task->data[3] == task->data[7] && task->data[4] == task->data[8])
            DestroyAnimVisualTask(taskId);
    }
}

// Animates MOVE_MOONLIGHT's sparkles.
// arg 0: x pixel offset
// arg 1: y offset
static void AnimMoonlightSparkle(struct Sprite* sprite)
{
    sprite->x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X) + gBattleAnimArgs[0];
    sprite->y = gBattleAnimArgs[1];
	
    sprite->data[0] = 0;
    sprite->data[1] = 0;
    sprite->data[2] = 0;
    sprite->data[3] = 0;
    sprite->data[4] = 1;
    sprite->callback = AnimMoonlightSparkleStep;
}

static void AnimMoonlightSparkleStep(struct Sprite* sprite)
{
    if (++sprite->data[1] > 1)
    {
        sprite->data[1] = 0;
		
        if (sprite->data[2] < 120)
        {
			sprite->data[2]++;
            sprite->y++;
        }
    }

    if (sprite->data[0])
        DestroyAnimSprite(sprite);
}

// Blends the screen to blue in MOVE_MOONLIGHT's anim.
// No args.
void AnimTask_FadeScreenBlue(u8 taskId)
{
    int a = SelectBattleAnimSpriteAndBgPalettes(TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE) & 0xFFFF;
    int b;
    int c;
    int d;

    gTasks[taskId].data[0] = 0;
    gTasks[taskId].data[1] = 0;
    gTasks[taskId].data[2] = 0;
    gTasks[taskId].data[3] = a;
    gTasks[taskId].data[4] = 0;
    gTasks[taskId].data[5] = 0;
    gTasks[taskId].data[6] = 0;
    gTasks[taskId].data[7] = 13;
    gTasks[taskId].data[8] = 14;
    gTasks[taskId].data[9] = 15;
	
    b = SelectBattlerSpritePalettes(TRUE, TRUE, TRUE, TRUE);
    c = a | b;
	
    StorePointerInVars(&gTasks[taskId].data[14], &gTasks[taskId].data[15], (void*)c);
	
    b = b | (0x10000 << IndexOfSpritePaletteTag(ANIM_TAG_MOON));
    d = IndexOfSpritePaletteTag(ANIM_TAG_GREEN_SPARKLE);
	
    BeginNormalPaletteFade((0x10000 << d) | b, 0, 0, 16, RGB(27, 29, 31));
	
    gTasks[taskId].func = AnimTask_FadeScreenBlueStep;
    gTasks[taskId].func(taskId);
}

static void AnimTask_FadeScreenBlueStep(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    
    switch (task->data[0])
    {
    case 0:
        if (++task->data[1] > 0)
        {
            u16 i, j, color, bitmask, r3;
			
            task->data[1] = 0;
			
            if (++task->data[2] <= 15)
            {
                u16 red, green, blue;

                task->data[4] += task->data[7];
                task->data[5] += task->data[8];
                task->data[6] += task->data[9];
				
                red = task->data[4] >> 3;
                green = task->data[5] >> 3;
                blue = task->data[6] >> 3;
				
                color = RGB(red, green, blue);
            }
            else
            {
                color = RGB(27, 29, 31);
                task->data[0]++;
            }

            bitmask = 1;
            r3 = 0;
            for (i = 0; i <= 15; i++)
            {
                if (task->data[3] & bitmask)
                {
                    for (j = 1; j <= 15; j++)
                        gPlttBufferFaded[r3 + j] = color;
                }
                bitmask <<= 1;
                r3 += 16;
            }
        }
        break;
    case 1:
        if (!gPaletteFade.active)
        {
            u8 spriteId;
			
            for (spriteId = 0; spriteId < MAX_SPRITES; spriteId++)
            {
                if (gSprites[spriteId].template == &gMoonSpriteTemplate || gSprites[spriteId].template == &gMoonlightSparkleSpriteTemplate)
                    gSprites[spriteId].data[0] = 1;
            }
            task->data[1] = 0;
            task->data[0]++;
        }
        break;
    case 2:
        if (++task->data[1] > 30)
        {
            BeginNormalPaletteFade((u32)LoadPointerFromVars(task->data[14], task->data[15]), 0, 16, 0, RGB(27, 29, 31));
            task->data[0]++;
        }
        break;
    case 3:
        if (!gPaletteFade.active)
            DestroyAnimVisualTask(taskId);
        break;
    }
}

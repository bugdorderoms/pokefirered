#include "global.h"
#include "gflib.h"
#include "battle.h"
#include "battle_anim.h"
#include "decompress.h"
#include "task.h"
#include "trig.h"
#include "util.h"
#include "constants/battle_anim.h"
#include "constants/pokemon.h"

// Function Declarations
static u8 sub_8078178(u8 battlerId, bool8 b);
static void sub_80782BC(u8 taskId);
static void AnimTask_FrozenIceCubeStep(u8 taskId);
static void AnimTask_FrozenIceCubeStep2(u8 taskId);
static void AnimTask_FrozenIceCubeStep3(u8 taskId);
static void AnimTask_FrozenIceCubeStep4(u8 taskId);
static void Task_DoStatusAnimation(u8 taskId);
static void sub_807834C(struct Sprite *sprite);
static void sub_8078380(struct Sprite *sprite);

// Data
static const union AnimCmd sSpriteAnim_DisableSparkle[] =
{
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_FRAME(16, 3),
    ANIMCMD_FRAME(32, 3),
    ANIMCMD_FRAME(48, 3),
    ANIMCMD_FRAME(64, 3),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_DisableSparkle[] =
{
    sSpriteAnim_DisableSparkle
};

const struct SpriteTemplate gDisableSparkleSpriteTemplate =
{
    .tileTag = ANIM_TAG_SPARKLE_4,
    .paletteTag = ANIM_TAG_SPARKLE_4,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sSpriteAnimTable_DisableSparkle,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_TrackOffsetFromAttackerAndWaitAnim,
};

static const struct Subsprite sFrozenIceCubeSubsprites[] =
{
    {.x = -16, .y = -16, .shape = SPRITE_SHAPE(8x8),  .size = 3, .tileOffset =   0, .priority = 2},
    {.x = -16, .y =  48, .shape = SPRITE_SHAPE(16x8), .size = 3, .tileOffset =  64, .priority = 2},
    {.x =  48, .y = -16, .shape = SPRITE_SHAPE(8x16), .size = 3, .tileOffset =  96, .priority = 2},
    {.x =  48, .y =  48, .shape = SPRITE_SHAPE(8x8),  .size = 2, .tileOffset = 128, .priority = 2},
};

static const struct SubspriteTable sFrozenIceCubeSubspriteTable[] =
{
    {ARRAY_COUNT(sFrozenIceCubeSubsprites), sFrozenIceCubeSubsprites},
};

static const struct SpriteTemplate sFrozenIceCubeSpriteTemplate =
{
    .tileTag = ANIM_TAG_ICE_CUBE,
    .paletteTag = ANIM_TAG_ICE_CUBE,
    .oam = &gOamData_AffineOff_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sUnknown_83BF574 =
{
    .tileTag = ANIM_TAG_CIRCLE_IMPACT,
    .paletteTag = ANIM_TAG_CIRCLE_IMPACT,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_807834C,
};

// Functions
static u8 sub_8078178(u8 battlerId, bool8 b)
{
    u8 battlerSpriteId = gBattlerSpriteIds[battlerId];
    u8 taskId = CreateTask(sub_80782BC, 10);
    u8 spriteId2;
    u8 i;

    LoadCompressedSpriteSheetUsingHeap(&gBattleAnimPicTable[GET_TRUE_SPRITE_INDEX(ANIM_TAG_CIRCLE_IMPACT)]);
    LoadCompressedSpritePaletteUsingHeap(&gBattleAnimPaletteTable[GET_TRUE_SPRITE_INDEX(ANIM_TAG_CIRCLE_IMPACT)]);
    gTasks[taskId].data[0] = battlerId;
    if (b)
    {
        gTasks[taskId].data[1] = RGB_RED;
        for (i = 0; i < 10; i++)
        {
            spriteId2 = CreateSprite(&sUnknown_83BF574, gSprites[battlerSpriteId].x, gSprites[battlerSpriteId].y + 32, 0);
            gSprites[spriteId2].data[0] = i * 51;
            gSprites[spriteId2].data[1] = -256;
            gSprites[spriteId2].invisible = TRUE;
            if (i > 4)
                gSprites[spriteId2].data[6] = 21;
        }
    }
    else
    {
        gTasks[taskId].data[1] = RGB_BLUE;
        for (i = 0; i < 10; i++)
        {
            spriteId2 = CreateSprite(&sUnknown_83BF574, gSprites[battlerSpriteId].x, gSprites[battlerSpriteId].y - 32, 0);
            gSprites[spriteId2].data[0] = i * 51;
            gSprites[spriteId2].data[1] = 256;
            gSprites[spriteId2].invisible = TRUE;
            if (i > 4)
                gSprites[spriteId2].data[6] = 21;
        }
    }
    gSprites[spriteId2].data[7] = 1;
    return taskId;
}

static void sub_80782BC(u8 taskId)
{
    if (gTasks[taskId].data[2] == 2)
    {
        gTasks[taskId].data[2] = 0;
        BlendPalette(0x100 + gTasks[taskId].data[0] * 16, 16, gTasks[taskId].data[4], gTasks[taskId].data[1]);
        if (gTasks[taskId].data[5] == 0)
        {
            gTasks[taskId].data[4]++;
            if (gTasks[taskId].data[4] > 8)
                gTasks[taskId].data[5] ^= 1;
        }
        else
        {
            u16 var = gTasks[taskId].data[4];

            gTasks[taskId].data[4]--;
            if (gTasks[taskId].data[4] < 0)
            {
                gTasks[taskId].data[4] = var;
                gTasks[taskId].data[5] ^= 1;
                gTasks[taskId].data[3]++;
                if (gTasks[taskId].data[3] == 2)
                    DestroyTask(taskId);
            }
        }
    }
    else
    {
        gTasks[taskId].data[2]++;
    }
}

static void sub_807834C(struct Sprite *sprite)
{
    if (sprite->data[6] == 0)
    {
        sprite->invisible = FALSE;
        sprite->callback = sub_8078380;
        sub_8078380(sprite);
    }
    else
    {
        sprite->data[6]--;
    }
}

static void sub_8078380(struct Sprite *sprite)
{
    sprite->x2 = Cos(sprite->data[0], 32);
    sprite->y2 = Sin(sprite->data[0], 8);
    if (sprite->data[0] < 128)
        sprite->subpriority = 29;
    else
        sprite->subpriority = 31;
    sprite->data[0] = (sprite->data[0] + 8) & 0xFF;
    sprite->data[5] += sprite->data[1];
    sprite->y2 += sprite->data[5] >> 8;
    sprite->data[2]++;
    if (sprite->data[2] == 52)
    {
        if (sprite->data[7])
            DestroySpriteAndFreeResources(sprite);
        else
            DestroySprite(sprite);
    }
}

// Animates the frozen ice cube on the given battler.
// arg 0: anim battler
void AnimTask_FrozenIceCube(u8 taskId)
{
	u8 spriteId, battlerId = GetBattlerForAnimScript(gBattleAnimArgs[0]);
    s16 x = GetBattlerSpriteCoord(battlerId, BATTLER_COORD_X) - 32;
    s16 y = GetBattlerSpriteCoord(battlerId, BATTLER_COORD_Y_PIC_OFFSET) - 36;
    
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 16));
	
    spriteId = CreateSprite(&sFrozenIceCubeSpriteTemplate, x, y, 4);
	
    if (GetSpriteTileStartByTag(ANIM_TAG_ICE_CUBE) == SPRITE_INVALID_TAG)
        gSprites[spriteId].invisible = TRUE;
    
    SetSubspriteTables(&gSprites[spriteId], sFrozenIceCubeSubspriteTable);
	
    gTasks[taskId].data[15] = spriteId;
    gTasks[taskId].func = AnimTask_FrozenIceCubeStep;
}

static void AnimTask_FrozenIceCubeStep(u8 taskId)
{
    if (++gTasks[taskId].data[1] == 10)
    {
        gTasks[taskId].func = AnimTask_FrozenIceCubeStep2;
        gTasks[taskId].data[1] = 0;
    }
    else
    {
        u8 var = gTasks[taskId].data[1];
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(var, 16 - var));
    }
}

static void AnimTask_FrozenIceCubeStep2(u8 taskId)
{
    u8 palIndex = IndexOfSpritePaletteTag(ANIM_TAG_ICE_CUBE);

    if (gTasks[taskId].data[1]++ > 13)
    {
        if (++gTasks[taskId].data[2] == 3)
        {
            u16 temp = gPlttBufferFaded[0x100 + palIndex * 16 + 13];

            gPlttBufferFaded[0x100 + palIndex * 16 + 13] = gPlttBufferFaded[0x100 + palIndex * 16 + 14];
            gPlttBufferFaded[0x100 + palIndex * 16 + 14] = gPlttBufferFaded[0x100 + palIndex * 16 + 15];
            gPlttBufferFaded[0x100 + palIndex * 16 + 15] = temp;

            gTasks[taskId].data[2] = 0;

            if (++gTasks[taskId].data[3] == 3)
            {
                gTasks[taskId].data[3] = 0;
                gTasks[taskId].data[1] = 0;

                if (++gTasks[taskId].data[4] == 2)
                {
                    gTasks[taskId].data[1] = 9;
                    gTasks[taskId].func = AnimTask_FrozenIceCubeStep3;
                }
            }
        }
    }
}

static void AnimTask_FrozenIceCubeStep3(u8 taskId)
{
    if (--gTasks[taskId].data[1] == -1)
    {
        gTasks[taskId].func = AnimTask_FrozenIceCubeStep4;
        gTasks[taskId].data[1] = 0;
    }
    else
    {
        u8 var = gTasks[taskId].data[1];
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(var, 16 - var));
    }
}

static void AnimTask_FrozenIceCubeStep4(u8 taskId)
{
    if (++gTasks[taskId].data[1] == 37)
        DestroySpriteAndFreeMatrix(&gSprites[gTasks[taskId].data[15]]);
    else if (gTasks[taskId].data[1] == 39)
        DestroyAnimVisualTaskAndDisableBlend(taskId);
}

void AnimTask_StatsChange(u8 taskId)
{
    gBattleAnimArgs[0] = (gBattleStruct->statChange.buff < 0);
    gBattleAnimArgs[1] = gBattleSpritesDataPtr->animationData->animArg;
    gBattleAnimArgs[2] = 0;
    gBattleAnimArgs[3] = 0;
    gBattleAnimArgs[4] = 0;
    gTasks[taskId].func = InitStatsChangeAnimation;
    InitStatsChangeAnimation(taskId);
}

void LaunchStatusAnimation(u8 battlerId, u8 statusAnimId)
{
    gBattleAnimAttacker = battlerId;
    gBattleAnimTarget = battlerId;
    LaunchBattleAnimation(ANIM_TYPE_STATUS, statusAnimId);
    gTasks[CreateTask(Task_DoStatusAnimation, 10)].data[0] = battlerId;
}

static void Task_DoStatusAnimation(u8 taskId)
{
    gAnimScriptCallback();
	
    if (!gAnimScriptActive)
    {
        gBattleSpritesDataPtr->healthBoxesData[gTasks[taskId].data[0]].statusAnimActive = FALSE;
        DestroyTask(taskId);
    }
}

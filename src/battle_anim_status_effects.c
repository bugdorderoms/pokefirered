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
static void AnimTask_FrozenIceCubeStep(u8 taskId);
static void AnimTask_FrozenIceCubeStep2(u8 taskId);
static void AnimTask_FrozenIceCubeStep3(u8 taskId);
static void AnimTask_FrozenIceCubeStep4(u8 taskId);
static void Task_DoStatusAnimation(u8 taskId);

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

// Animates the frozen ice cube on the given battler.
// arg 0: anim battler
void AnimTask_FrozenIceCube(u8 taskId)
{
	u32 spriteId, battlerId = GetBattlerForAnimScript(gBattleAnimArgs[0]);

    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 16));
	
    spriteId = CreateSprite(&sFrozenIceCubeSpriteTemplate, GetBattlerSpriteCoord(battlerId, BATTLER_COORD_X) - 32, GetBattlerSpriteCoord(battlerId, BATTLER_COORD_Y_PIC_OFFSET) - 36, 4);
	
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
        u32 var = gTasks[taskId].data[1];
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(var, 16 - var));
    }
}

static void AnimTask_FrozenIceCubeStep2(u8 taskId)
{
    u32 palIndex = IndexOfSpritePaletteTag(ANIM_TAG_ICE_CUBE);

    if (gTasks[taskId].data[1]++ > 13)
    {
        if (++gTasks[taskId].data[2] == 3)
        {
            u32 temp = gPlttBufferFaded[0x100 + palIndex * 16 + 13];

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
        u32 var = gTasks[taskId].data[1];
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

void LaunchStatusAnimation(u32 battlerId, u32 statusAnimId)
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

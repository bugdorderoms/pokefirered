#include "global.h"
#include "gflib.h"
#include "battle_anim.h"
#include "graphics.h"
#include "constants/songs.h"

static void AnimTask_MetallicShine_Step(u32 taskId);
static void AnimTask_MaxSteelspike_Step(u32 taskId);
static void CreateSteelspikeSprite(struct Task *task);
static void CreateSteelspikeCurvedSprite(void);
static void AnimLargeSteelSpike(struct Sprite *sprite);

const struct SpriteTemplate gMetalSoundSpriteTemplate =    
{
    .tileTag = ANIM_TAG_METAL_SOUND_WAVES,
    .paletteTag = ANIM_TAG_METAL_SOUND_WAVES,
    .oam = &gOamData_AffineDouble_ObjNormal_32x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gGrowingRingAffineAnimTable,
    .callback = TranslateAnimSpriteToTargetMonLocation,
};

const struct SpriteTemplate gGyroBallSteelBallSpriteTemplate =    
{
    .tileTag = ANIM_TAG_WISP_ORB,
    .paletteTag = ANIM_TAG_GUST,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gAnims_WillOWispOrb,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimFireSpiralOutward,
};

const struct SpriteTemplate gMetalBurstSpikeSpriteTemplate =
{
    .tileTag = ANIM_TAG_SPIKES,
    .paletteTag = ANIM_TAG_SPIKES,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimOutrageFlame,
};

const struct SpriteTemplate gFlashCannonBallSpriteTemplate =
{
    .tileTag = ANIM_TAG_FLASH_CANNON_BALL,
    .paletteTag = ANIM_TAG_FLASH_CANNON_BALL,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gAffineAnims_ShadowBall,
    .callback = AnimShadowBall,
};

const struct SpriteTemplate gMagnetBombRockBallSpriteTemplate =
{
    .tileTag = ANIM_TAG_BLACK_BALL_2,
    .paletteTag = ANIM_TAG_BLACK_BALL_2,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimNeedleArmSpike,
};

static const union AnimCmd sLargeSpikeAnimCmds[] =
{
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_FRAME(32, 3),
    ANIMCMD_FRAME(64, 3),
    ANIMCMD_FRAME(96, 3),
    ANIMCMD_END
};

static const union AnimCmd *const sLargeSpikeAnimTable[] =
{
    sLargeSpikeAnimCmds,
};

const struct SpriteTemplate gMaxSteelspikeSpriteTemplate =
{
    .tileTag = ANIM_TAG_LARGE_SPIKE,
    .paletteTag = ANIM_TAG_LARGE_SPIKE,
    .oam = &gOamData_AffineOff_ObjNormal_32x64,
    .anims = sLargeSpikeAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimLargeSteelSpike,
};

static const union AffineAnimCmd sLargeSpikeLeftAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(0, 0, 32, 1), // 45 degree turn
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd sLargeSpikeRightAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(0, 0, -32, 1), // 45 degree turn
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const sLargeSpikeAffineAnimTable[] =
{
    sLargeSpikeLeftAffineAnimCmds,
    sLargeSpikeRightAffineAnimCmds
};

const struct SpriteTemplate gMaxSteelspikeCurvedSpriteTemplate =
{
    .tileTag = ANIM_TAG_LARGE_SPIKE,
    .paletteTag = ANIM_TAG_LARGE_SPIKE,
    .oam = &gOamData_AffineDouble_ObjNormal_32x64,
    .anims = sLargeSpikeAnimTable,
    .images = NULL,
    .affineAnims = sLargeSpikeAffineAnimTable,
    .callback = AnimLargeSteelSpike,
};

const struct SpriteTemplate gSteelsurgeSpriteTemplate =    
{
    .tileTag = ANIM_TAG_SPIKES,
    .paletteTag = ANIM_TAG_LARGE_SPIKE,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSpikes,
};

// Makes the attacker metallic and shining.
// Used by MOVE_HARDEN and MOVE_IRON_DEFENSE.
// arg 0: if true won't change battler's palette back
// arg 1: if true, use custom color
// arg 2: custom color
// Custom color argument is used in MOVE_POISON_TAIL to make the mon turn purplish/pinkish as if became cloaked in poison.
void AnimTask_MetallicShine(u32 taskId)
{
    u32 spriteId, newSpriteId;
    u32 paletteNum;
    struct BattleAnimBgData animBg;
    bool32 priorityChanged;

    gBattle_WIN0H = 0;
    gBattle_WIN0V = 0;
    
    SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG_ALL | WININ_WIN1_OBJ | WININ_WIN1_CLR);
    SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WINOBJ_BG_ALL | WINOUT_WINOBJ_OBJ | WINOUT_WINOBJ_CLR | WINOUT_WIN01_BG0 | WINOUT_WIN01_BG2 | WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
    SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJWIN_ON);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_BG1);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(8, 12));
    
    SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 0);
    SetAnimBgAttribute(1, BG_ANIM_SCREEN_SIZE, 0);
    SetAnimBgAttribute(1, BG_ANIM_CHAR_BASE_BLOCK, 1);
    
    priorityChanged = FALSE;
    
    if (IsDoubleBattleForBattler(gBattleAnimAttacker))
    {
        if (GetBattlerPosition(gBattleAnimAttacker) == B_POSITION_OPPONENT_RIGHT || GetBattlerPosition(gBattleAnimAttacker) == B_POSITION_PLAYER_LEFT)
        {
            if (IsBattlerSpriteVisible(BATTLE_PARTNER(gBattleAnimAttacker)))
            {
                gSprites[gBattlerSpriteIds[BATTLE_PARTNER(gBattleAnimAttacker)]].oam.priority--;
                SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
                priorityChanged = TRUE;
            }
        }
    }
    spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);
    newSpriteId = CreateCloneOfSpriteInWindowMode(spriteId);
    
    GetBattleAnimBgData(&animBg, 1);
    AnimLoadCompressedBgTilemap(animBg.bgId, gMetalShineTilemap);
    AnimLoadCompressedBgGfx(animBg.bgId, gMetalShineGfx, animBg.tilesOffset);
    LoadCompressedPalette(gMetalShinePalette, animBg.paletteId * 16, 32);
    
    gBattle_BG1_X = -gSprites[spriteId].x + 96;
    gBattle_BG1_Y = -gSprites[spriteId].y + 32;
    
    paletteNum = 16 + gSprites[spriteId].oam.paletteNum;
    
    if (!gBattleAnimArgs[1])
        SetGreyscaleOrOriginalPalette(paletteNum, FALSE);
    else
        BlendPalette(paletteNum * 16, 16, 11, gBattleAnimArgs[2]);
    
    gTasks[taskId].data[0] = newSpriteId;
    gTasks[taskId].data[1] = gBattleAnimArgs[0];
    gTasks[taskId].data[6] = priorityChanged;
    gTasks[taskId].func = AnimTask_MetallicShine_Step;
}

static void AnimTask_MetallicShine_Step(u32 taskId)
{
    struct BattleAnimBgData animBg;
    
    gBattle_BG1_X -= 4;
    
    gTasks[taskId].data[10] += 4;
    
    if (gTasks[taskId].data[10] == 128)
    {
        gTasks[taskId].data[10] = 0;
        
        gBattle_BG1_X += 128;
        
        gTasks[taskId].data[11]++;
        
        if (gTasks[taskId].data[11] == 2)
        {
            if (!gTasks[taskId].data[1])
                SetGreyscaleOrOriginalPalette(16 + gSprites[GetAnimBattlerSpriteId(ANIM_ATTACKER)].oam.paletteNum, TRUE);
            
            DestroySprite(&gSprites[gTasks[taskId].data[0]]);
            
            GetBattleAnimBgData(&animBg, 1);
            InitBattleAnimBg(animBg.bgId);
            
            if (gTasks[taskId].data[6])
                gSprites[gBattlerSpriteIds[BATTLE_PARTNER(gBattleAnimAttacker)]].oam.priority++;
        }
        else if (gTasks[taskId].data[11] == 3)
        {
            gBattle_WIN0H = 0;
            gBattle_WIN0V = 0;
            
            SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG_ALL | WININ_WIN1_OBJ | WININ_WIN1_CLR);
            SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WINOBJ_BG_ALL | WINOUT_WINOBJ_OBJ | WINOUT_WINOBJ_CLR | WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
            SetGpuReg(REG_OFFSET_DISPCNT, GetGpuReg(REG_OFFSET_DISPCNT) ^ DISPCNT_OBJWIN_ON);
            
            SetAnimBgAttribute(1, BG_ANIM_CHAR_BASE_BLOCK, 0);
            
            DestroyAnimVisualTaskAndDisableBlend(taskId);
        }
    }
}

// Creates Max Steelspike's spikes.
// No args.
void AnimTask_MaxSteelspike(u32 taskId)
{
    u32 var0, var1, var2, var3;
    s32 var4;
    struct Task *task = &gTasks[taskId];

    var0 = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    var1 = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y) + 24;
    var2 = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    var3 = IsBattlerAlly(gBattleAnimAttacker, gBattleAnimTarget) ? var1 : GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y) + 24;
    
    task->data[8] = 48 - (4 * 8);
    task->data[0] = 0;
    task->data[11] = 0;
    task->data[9] = 0;
    task->data[12] = 1;
    
    var4 = task->data[8];
    if (var4 < 0)
        var4 += 7;
    
    task->data[10] = (var4 >> 3) - 1;
    task->data[2] = var0 * 8;
    task->data[3] = var1 * 8;
    task->data[4] = ((var2 - var0) * 8) / task->data[8];
    task->data[5] = ((var3 - var1) * 8) / task->data[8];
    task->func = AnimTask_MaxSteelspike_Step;
}

static void AnimTask_MaxSteelspike_Step(u32 taskId)
{
    struct Task *task = &gTasks[taskId];
    
    switch (task->data[0])
    {
        case 0:
            task->data[2] += task->data[4];
            task->data[3] += task->data[5];
            
            if (++task->data[9] >= task->data[10])
            {
                task->data[9] = 0;
                CreateSteelspikeSprite(task);
                task->data[13] += task->data[14];
            }
            
            if (--task->data[8] == 0)
            {
                task->data[11] = 0;
                task->data[0]++;
            }
            break;
        case 1:
            if (++task->data[11] == 10)
                task->data[0]++;
            break;
        case 2:
            CreateSteelspikeCurvedSprite();
            task->data[11] = 0;
            task->data[0]++;
            break;
        case 3:
            if (++task->data[11] == 7)
                task->data[0]++;
            break;
        case 4:
            DestroyAnimVisualTask(taskId);
            break;
    }
}

static void CreateSteelspikeSprite(struct Task *task)
{
    u32 spriteId;
    u16 x, y;
    
    x = task->data[2] >> 3;
    y = task->data[3] >> 3;
    x += (task->data[12] * 8);
    
    spriteId = CreateSprite(&gMaxSteelspikeSpriteTemplate, x, y, 35);
    
    if (spriteId != MAX_SPRITES)
    {
        gSprites[spriteId].data[0] = task->data[8] + 60;
        PlaySE(SE_M_HARDEN);
        task->data[11]++;
    }
    task->data[12] *= -1;
}

static void CreateSteelspikeCurvedSprite(void)
{
    u32 spriteId1, spriteId2;
    u16 x, y;
    
    x = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    y = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y);
    
    spriteId1 = CreateSprite(&gMaxSteelspikeCurvedSpriteTemplate, x + 5, y, 35);
    spriteId2 = CreateSprite(&gMaxSteelspikeCurvedSpriteTemplate, x - 5, y, 35);
    
    StartSpriteAffineAnim(&gSprites[spriteId1], 0); // Point left
    gSprites[spriteId1].data[0] = 50;
    
    StartSpriteAffineAnim(&gSprites[spriteId2], 1); // Point right
    gSprites[spriteId2].data[0] = 50;
    
    if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_PLAYER)
    {
        gSprites[spriteId1].oam.priority--;
        gSprites[spriteId1].y += 15;
        
        gSprites[spriteId2].oam.priority--;
        gSprites[spriteId2].y2 += 15;
    }
}

static void AnimLargeSteelSpike(struct Sprite *sprite)
{
    if (--sprite->data[0] == 0)
        DestroySpriteAndFreeMatrix(sprite);
}

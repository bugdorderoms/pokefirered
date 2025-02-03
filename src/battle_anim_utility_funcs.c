#include "global.h"
#include "gflib.h"
#include "battle.h"
#include "battle_anim.h"
#include "graphics.h"
#include "task.h"
#include "util.h"
#include "constants/songs.h"

struct AnimStatsChangeData
{
    u8 battler1;
    u8 battler2;
    u8 higherPriority;
    s16 data[5];
    u16 species;
};

static void StartBlendAnimSpriteColor(u8 taskId, u32 selectedPalettes);
static void AnimTask_BlendSpriteColor_Step2(u8 taskId);
static void Task_WaitHardwarePaletteFade(u8 taskId);
static void Task_DoCloneBattlerSpriteWithBlend(u8 taskId);
static void SpriteCB_FinishCloneBattlerSpriteWithBlend(struct Sprite *sprite);
static void StatsChangeAnimation_Step1(u8 taskId);
static void StatsChangeAnimation_Step2(u8 taskId);
static void StatsChangeAnimation_Step3(u8 taskId);
static void AnimTask_Flash_Step(u8 taskId);
static void SetPalettesToColor(u32 selectedPalettes, u16 color);
static void AnimTask_DoBgSliding(u8 taskId);
static void AnimTask_MonScrollingBgMask(u8 taskId);
static void AnimTask_SetAttackerInvisibleWaitForSignal_Step(u8 taskId);

static EWRAM_DATA struct AnimStatsChangeData *sAnimStatsChangeData = NULL;

const u8 gBattleAnimRegOffsBgCnt[] = { REG_OFFSET_BG0CNT, REG_OFFSET_BG1CNT, REG_OFFSET_BG2CNT, REG_OFFSET_BG3CNT };
const u8 gBattleIntroRegOffsBgCnt[] = { REG_OFFSET_BG0CNT, REG_OFFSET_BG1CNT, REG_OFFSET_BG2CNT, REG_OFFSET_BG3CNT };

// arg 0 is a bitfield.
// Bits 0-10 result in the following palettes being selected:
//   0: battle background palettes (BG palettes 1, 2, and 3)
//   1: gBattleAnimAttacker OBJ palette
//   2: gBattleAnimTarget OBJ palette
//   3: gBattleAnimAttacker partner OBJ palette
//   4: gBattleAnimTarget partner OBJ palette
//   5: BG palette 4
//   6: BG palette 5
//   7: Player battler left
//   8: Player battler right
//   9: Enemy battler left
//  10: Enemy battler right
// arg 1: blend delay (how higher more slow)
// arg 2: initial coefficient
// arg 3: target coefficient
// arg 4: blend color
void AnimTask_BlendSelected(u8 taskId)
{
    u32 selectedPalettes = UnpackSelectedBattleAnimPalettes(gBattleAnimArgs[0]);
    selectedPalettes |= SelectBattlerSpritePalettes((gBattleAnimArgs[0] >> 7) & 1, (gBattleAnimArgs[0] >> 8) & 1, (gBattleAnimArgs[0] >> 9) & 1, (gBattleAnimArgs[0] >> 10) & 1);
    StartBlendAnimSpriteColor(taskId, selectedPalettes);
}

// gBattleAnimArgs[0] is a command ID
// This command will blend bg and battlers except as commanded:
// Bits 0-6 of the selector parameter result in the following palettes not being selected:
//   0: F_PAL_EXCEPT_BG, battle background palettes (BG palettes 1, 2, and 3)
//   1: F_PAL_EXCEPT_ATTACKER, gBattleAnimAttacker OBJ palette
//   2: F_PAL_EXCEPT_TARGET, gBattleAnimTarget OBJ palette
//   3: F_PAL_EXCEPT_ATK_PARTNER, gBattleAnimAttacker partner OBJ palette
//   4: F_PAL_EXCEPT_DEF_PARTNER, gBattleAnimTarget partner OBJ palette
// arg 1: blend delay (how higher more slow)
// arg 2: initial coefficient
// arg 3: target coefficient
// arg 4: blend color
void AnimTask_BlendExcept(u8 taskId)
{
    u8 i, j, selectedBattlersCount = 0, animBattlers[MAX_BATTLERS_COUNT] = {0xFF, 0xFF, 0xFF, 0xFF};
    u32 selectedPalettes = (gBattleAnimArgs[0] & F_PAL_EXCEPT_BG) ? 0 : UnpackSelectedBattleAnimPalettes(F_PAL_BG);
	
	if (gBattleAnimArgs[0] & F_PAL_EXCEPT_ATTACKER)
		animBattlers[selectedBattlersCount++] = gBattleAnimAttacker;

	if (gBattleAnimArgs[0] & F_PAL_EXCEPT_TARGET)
		animBattlers[selectedBattlersCount++] = gBattleAnimTarget;
	
	if (gBattleAnimArgs[0] & F_PAL_EXCEPT_ATK_PARTNER)
		animBattlers[selectedBattlersCount++] = BATTLE_PARTNER(gBattleAnimAttacker);
	
	if (gBattleAnimArgs[0] & F_PAL_EXCEPT_DEF_PARTNER)
		animBattlers[selectedBattlersCount++] = BATTLE_PARTNER(gBattleAnimTarget);
	
    for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
    {
		bool8 select = TRUE;
		
		for (j = 0; j < selectedBattlersCount; j++)
		{
			if (i == animBattlers[j])
			{
				select = FALSE;
				break;
			}
		}
		
		if (select && IsBattlerSpriteVisible(i))
			selectedPalettes |= 0x10000 << i;
    }
    StartBlendAnimSpriteColor(taskId, selectedPalettes);
}

// arg 0 is a bitfield.
// Bits 0-10 result in the following palettes being selected:
//   0: battle background palettes (BG palettes 1, 2, and 3)
//   1: gBattleAnimAttacker OBJ palette
//   2: gBattleAnimTarget OBJ palette
//   3: gBattleAnimAttacker partner OBJ palette
//   4: gBattleAnimTarget partner OBJ palette
//   5: BG palette 4
//   6: BG palette 5
// arg 1: blend delay (how higher more slow)
// arg 2: initial coefficient
// arg 3: target coefficient
void AnimTask_SetCamouflageBlend(u8 taskId)
{
	gBattleAnimArgs[4] = gBattleTerrainTable[gBattleTerrain].camouflageBlendColor;
    StartBlendAnimSpriteColor(taskId, UnpackSelectedBattleAnimPalettes(gBattleAnimArgs[0]));
}

// Blends the given particle to the specified color.
// arg 0: palette tag
// arg 1: blend delay (how higher more slow)
// arg 2: initial coefficient
// arg 3: target coefficient
// arg 4: blend color
void AnimTask_BlendParticle(u8 taskId)
{
    u32 selectedPalettes = 1 << (IndexOfSpritePaletteTag(gBattleAnimArgs[0]) + 16);
    StartBlendAnimSpriteColor(taskId, selectedPalettes);
}

static void StartBlendAnimSpriteColor(u8 taskId, u32 selectedPalettes)
{
    gTasks[taskId].data[0] = selectedPalettes;
    gTasks[taskId].data[1] = selectedPalettes >> 16;
    gTasks[taskId].data[2] = gBattleAnimArgs[1];
    gTasks[taskId].data[4] = gBattleAnimArgs[3];
    gTasks[taskId].data[5] = gBattleAnimArgs[4];
    gTasks[taskId].data[10] = gBattleAnimArgs[2];
    gTasks[taskId].func = AnimTask_BlendSpriteColor_Step2;
    gTasks[taskId].func(taskId);
}

static void AnimTask_BlendSpriteColor_Step2(u8 taskId)
{
    u32 selectedPalettes;
    u16 singlePaletteMask;

    if (gTasks[taskId].data[9] == gTasks[taskId].data[2])
    {
        gTasks[taskId].data[9] = 0;
		
        selectedPalettes = gTasks[taskId].data[0] | (gTasks[taskId].data[1] << 16);
		singlePaletteMask = 0;
		
        while (selectedPalettes)
        {
            if (selectedPalettes & 1)
                BlendPalette(singlePaletteMask, 16, gTasks[taskId].data[10], gTasks[taskId].data[5]);
			
            singlePaletteMask += 0x10;
            selectedPalettes >>= 1;
        }
		
        if (gTasks[taskId].data[10] < gTasks[taskId].data[4])
            ++gTasks[taskId].data[10];
        else if (gTasks[taskId].data[10] > gTasks[taskId].data[4])
            --gTasks[taskId].data[10];
        else
            DestroyAnimVisualTask(taskId);
    }
	else
		++gTasks[taskId].data[9];
}

// Starts an hardware palette fade.
// arg 0: blend cnt
// arg 1: fade delay
// arg 2: initial y coefficient
// arg 3: target y coefficient
// arg 4: shouldResetBlendRegisters (boolean)
void AnimTask_HardwarePaletteFade(u8 taskId)
{
    BeginHardwarePaletteFade(gBattleAnimArgs[0], gBattleAnimArgs[1], gBattleAnimArgs[2], gBattleAnimArgs[3], gBattleAnimArgs[4]);
    gTasks[taskId].func = Task_WaitHardwarePaletteFade;
}

static void Task_WaitHardwarePaletteFade(u8 taskId)
{
    if (!gPaletteFade.active)
        DestroyAnimVisualTask(taskId);
}

// Used to leave blended traces of a mon, usually to imply speed as in Agility or Aerial Ace
// arg 0: anim battler
// arg 1: delay (?)
// arg 2: duration (num frames the sprite will be visible)
// arg 3: state (?)
void AnimTask_CloneBattlerSpriteWithBlend(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    task->data[0] = gBattleAnimArgs[0];
    task->data[1] = 0;
    task->data[2] = gBattleAnimArgs[1];
    task->data[3] = gBattleAnimArgs[2];
    task->data[4] = gBattleAnimArgs[3];
    task->data[5] = 0;
    task->func = Task_DoCloneBattlerSpriteWithBlend;
}

static void Task_DoCloneBattlerSpriteWithBlend(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    if (task->data[4])
    {
        if (task->data[1])
            --task->data[1];
        else
        {
            task->data[6] = CloneBattlerSpriteWithBlend(task->data[0]);
			
            if (task->data[6] >= 0)
            {
                gSprites[task->data[6]].oam.priority = task->data[0] != ANIM_ATTACKER ? 1 : 2;
                gSprites[task->data[6]].data[0] = task->data[3];
                gSprites[task->data[6]].data[1] = taskId;
                gSprites[task->data[6]].data[2] = 5;
                gSprites[task->data[6]].callback = SpriteCB_FinishCloneBattlerSpriteWithBlend;
                ++task->data[5];
            }
            --task->data[4];
            task->data[1] = task->data[2];
        }
    }
    else if (task->data[5] == 0)
        DestroyAnimVisualTask(taskId);
}

static void SpriteCB_FinishCloneBattlerSpriteWithBlend(struct Sprite *sprite)
{
    if (sprite->data[0])
        --sprite->data[0];
    else
    {
        --gTasks[sprite->data[1]].data[sprite->data[2]];
        DestroySpriteWithActiveSheet(sprite);
    }
}

void InitStatsChangeAnimation(u8 taskId)
{
    u8 i;

    sAnimStatsChangeData = AllocZeroed(sizeof(struct AnimStatsChangeData));
	
    for (i = 0; i < 5; ++i)
        sAnimStatsChangeData->data[i] = gBattleAnimArgs[i];
	
	sAnimStatsChangeData->battler1 = gBattleAnimAttacker;
	sAnimStatsChangeData->battler2 = BATTLE_PARTNER(sAnimStatsChangeData->battler1);
	
    gTasks[taskId].func = StatsChangeAnimation_Step1;
}

static void StatsChangeAnimation_Step1(u8 taskId)
{
    gBattle_WIN0H = 0;
    gBattle_WIN0V = 0;
	
    SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG_ALL | WININ_WIN1_OBJ | WININ_WIN1_CLR);
    SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG2 | WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ  | WINOUT_WIN01_CLR
                               | WINOUT_WINOBJ_BG_ALL | WINOUT_WINOBJ_OBJ | WINOUT_WINOBJ_CLR);
    SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJWIN_ON);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG1 | BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 16));
	
    SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 0);
    SetAnimBgAttribute(1, BG_ANIM_SCREEN_SIZE, 0);
    SetAnimBgAttribute(1, BG_ANIM_CHAR_BASE_BLOCK, 1);
	
    if (IsDoubleBattleForBattler(sAnimStatsChangeData->battler1))
    {
        if (GetBattlerPosition(sAnimStatsChangeData->battler1) == B_POSITION_OPPONENT_RIGHT || GetBattlerPosition(sAnimStatsChangeData->battler1) == B_POSITION_PLAYER_LEFT)
        {
            if (IsBattlerSpriteVisible(sAnimStatsChangeData->battler2))
            {
                gSprites[gBattlerSpriteIds[sAnimStatsChangeData->battler2]].oam.priority -= 1;
                SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
                sAnimStatsChangeData->higherPriority = 1;
            }
        }
    }
	sAnimStatsChangeData->species = GetMonData(GetBattlerPartyIndexPtr(sAnimStatsChangeData->battler1), MON_DATA_SPECIES);
    gTasks[taskId].func = StatsChangeAnimation_Step2;
}

static void StatsChangeAnimation_Step2(u8 taskId)
{
    struct BattleAnimBgData animBgData;
    u8 spriteId = CreateCloneOfSpriteInWindowMode(gBattlerSpriteIds[sAnimStatsChangeData->battler1], sAnimStatsChangeData->species);
	
    GetBattleAnimBgData(&animBgData, 1);
	AnimLoadCompressedBgTilemap(animBgData.bgId, sAnimStatsChangeData->data[0] == 0 ? gBattleStatMask1_Tilemap : gBattleStatMask2_Tilemap);
    AnimLoadCompressedBgGfx(animBgData.bgId, gBattleStatMask_Gfx, animBgData.tilesOffset);
	
    switch (sAnimStatsChangeData->data[1])
    {
	case 0: // Multiple stats
        LoadCompressedPalette(gBattleStatMask5_Pal, animBgData.paletteId * 16, 32);
        break;
    case STAT_ATK: // Attack
        LoadCompressedPalette(gBattleStatMask2_Pal, animBgData.paletteId * 16, 32);
        break;
    case STAT_DEF: // Defense
        LoadCompressedPalette(gBattleStatMask1_Pal, animBgData.paletteId * 16, 32);
        break;
	case STAT_SPEED: // Speed
        LoadCompressedPalette(gBattleStatMask4_Pal, animBgData.paletteId * 16, 32);
        break;
	case STAT_SPATK: // Sp. Attack
        LoadCompressedPalette(gBattleStatMask7_Pal, animBgData.paletteId * 16, 32);
        break;
	case STAT_SPDEF: // Sp. Defense
        LoadCompressedPalette(gBattleStatMask8_Pal, animBgData.paletteId * 16, 32);
        break;
    case STAT_ACC: // Accuracy
        LoadCompressedPalette(gBattleStatMask3_Pal, animBgData.paletteId * 16, 32);
        break;
    case STAT_EVASION: // Evasion
        LoadCompressedPalette(gBattleStatMask6_Pal, animBgData.paletteId * 16, 32);
        break;
    }
    gBattle_BG1_Y = 0;
	
	// Is negative
    if (sAnimStatsChangeData->data[0])
    {
        gBattle_BG1_X = 64;
        gTasks[taskId].data[1] = -3;
    }
    else
	{
		gBattle_BG1_X = 0;
        gTasks[taskId].data[1] = 3;
	}
    gTasks[taskId].data[0] = spriteId;
	gTasks[taskId].data[4] = 10;
	gTasks[taskId].data[5] = 20;
    gTasks[taskId].data[6] = sAnimStatsChangeData->higherPriority;
    gTasks[taskId].data[7] = gBattlerSpriteIds[sAnimStatsChangeData->battler2];
    gTasks[taskId].func = StatsChangeAnimation_Step3;
	PlaySE12WithPanning(sAnimStatsChangeData->data[0] ? SE_M_STAT_DECREASE : SE_M_STAT_INCREASE, BattleAnimAdjustPanning2(SOUND_PAN_ATTACKER));
}

static void StatsChangeAnimation_Step3(u8 taskId)
{
    gBattle_BG1_Y += gTasks[taskId].data[1];

    switch (gTasks[taskId].data[15])
    {
    case 0:
        if (gTasks[taskId].data[11]++ > 0)
        {
            gTasks[taskId].data[11] = 0;
            ++gTasks[taskId].data[12];
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[12], 16 - gTasks[taskId].data[12]));
            if (gTasks[taskId].data[12] == gTasks[taskId].data[4])
                ++gTasks[taskId].data[15];
        }
        break;
    case 1:
        if (++gTasks[taskId].data[10] == gTasks[taskId].data[5])
            ++gTasks[taskId].data[15];
        break;
    case 2:
        if (gTasks[taskId].data[11]++ > 0)
        {
            gTasks[taskId].data[11] = 0;
            --gTasks[taskId].data[12];
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[12], 16 - gTasks[taskId].data[12]));
            if (gTasks[taskId].data[12] == 0)
            {
                ResetBattleAnimBg(0);
                ++gTasks[taskId].data[15];
            }
        }
        break;
    case 3:
        gBattle_WIN0H = 0;
        gBattle_WIN0V = 0;
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG_ALL | WININ_WIN1_OBJ | WININ_WIN1_CLR);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG_ALL  | WINOUT_WIN01_OBJ  | WINOUT_WIN01_CLR | WINOUT_WINOBJ_BG_ALL | WINOUT_WINOBJ_OBJ | WINOUT_WINOBJ_CLR);
        SetAnimBgAttribute(1, BG_ANIM_CHAR_BASE_BLOCK, 0);
        SetGpuReg(REG_OFFSET_DISPCNT, GetGpuReg(REG_OFFSET_DISPCNT) ^ DISPCNT_OBJWIN_ON);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        DestroySprite(&gSprites[gTasks[taskId].data[0]]);
        if (gTasks[taskId].data[6] == 1)
            ++gSprites[gTasks[taskId].data[7]].oam.priority;
        Free(sAnimStatsChangeData);
        sAnimStatsChangeData = NULL;
        DestroyAnimVisualTask(taskId);
        break;
    }
}

// Animates flash's effect.
// No args.
void AnimTask_Flash(u8 taskId)
{
    u32 selectedPalettes = SelectBattlerSpritePalettes(TRUE, TRUE, TRUE, TRUE);

    SetPalettesToColor(selectedPalettes, RGB_BLACK);
	
    gTasks[taskId].data[14] = selectedPalettes >> 16;
	
    selectedPalettes = SelectBattleAnimSpriteAndBgPalettes(TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE) & 0xFFFF;
    SetPalettesToColor(selectedPalettes, RGB_WHITE);
	
    gTasks[taskId].data[15] = selectedPalettes;
    gTasks[taskId].data[0] = 0;
    gTasks[taskId].data[1] = 0;
    gTasks[taskId].func = AnimTask_Flash_Step;
}

static void AnimTask_Flash_Step(u8 taskId)
{
    u16 i;
    struct Task *task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        if (++task->data[1] > 6)
        {
            task->data[1] = 0;
            task->data[2] = 16;
            ++task->data[0];
        }
        break;
    case 1:
        if (++task->data[1] > 1)
        {
            task->data[1] = 0;
			
            --task->data[2];
			
            for (i = 0; i < 16; ++i)
            {
                if ((task->data[15] >> i) & 1)
                {
                    u16 paletteOffset = i * 16;
                    BlendPalette(paletteOffset, 16, task->data[2], RGB_WHITE);
                }

                if ((task->data[14] >> i) & 1)
                {
                    u16 paletteOffset = i * 16 + 0x100;
                    BlendPalette(paletteOffset, 16, task->data[2], RGB_BLACK);
                }
            }

            if (task->data[2] == 0)
                ++task->data[0];
        }
        break;
    case 2:
        DestroyAnimVisualTask(taskId);
        break;
    }
}

static void SetPalettesToColor(u32 selectedPalettes, u16 color)
{
    u16 i, curOffset, paletteOffset;

    for (i = 0; i < 32; selectedPalettes >>= 1, ++i)
	{
        if (selectedPalettes & 1)
		{
            for (curOffset = i * 16, paletteOffset = curOffset; curOffset < paletteOffset + 16; ++curOffset)
                gPlttBufferFaded[curOffset] = color;
		}
	}
}

// Blends all battler's palettes, except the attacker.
// arg 0: blend delay (how higher more slow)
// arg 1: initial coefficient
// arg 2: target coefficient
// arg 3: blend color
void AnimTask_BlendNonAttackerPalettes(u8 taskId)
{
    s32 j;
    u32 battler, selectedPalettes = 0;

    for (battler = 0; battler < MAX_BATTLERS_COUNT; ++battler)
	{
        if (gBattleAnimAttacker != battler)
            selectedPalettes |= 1 << (battler + 16);
	}
    for (j = 4; j != 0; --j)
        gBattleAnimArgs[j] = gBattleAnimArgs[j - 1];
	
    StartBlendAnimSpriteColor(taskId, selectedPalettes);
}

// Starts the sliding bg movement task.
// arg 0: x movement
// arg 1: y movement
// arg 2: invert direction based on side
void AnimTask_StartSlidingBg(u8 taskId)
{
    u8 newTaskId = CreateTask(AnimTask_DoBgSliding, 5);

    ToggleBg3Mode(FALSE);
	
    if (gBattleAnimArgs[2] && GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
    {
        gBattleAnimArgs[0] = -gBattleAnimArgs[0];
        gBattleAnimArgs[1] = -gBattleAnimArgs[1];
    }
    gTasks[newTaskId].data[1] = gBattleAnimArgs[0];
    gTasks[newTaskId].data[2] = gBattleAnimArgs[1];

    DestroyAnimVisualTask(taskId);
}

static void AnimTask_DoBgSliding(u8 taskId)
{
    gTasks[taskId].data[10] += gTasks[taskId].data[1];
    gTasks[taskId].data[11] += gTasks[taskId].data[2];
	
    gBattle_BG3_X += gTasks[taskId].data[10] >> 8;
    gBattle_BG3_Y += gTasks[taskId].data[11] >> 8;
	
    gTasks[taskId].data[10] &= 0xFF;
    gTasks[taskId].data[11] &= 0xFF;
	
	// Signal to stop
    if (gBattleAnimArgs[ARG_RET_ID] == -1)
    {
        gBattle_BG3_X = 0;
        gBattle_BG3_Y = 0;
        ToggleBg3Mode(TRUE);
        DestroyTask(taskId);
    }
}

// For hiding or subsequently revealing all other battlers.
// arg 0: invisible (boolean)
void AnimTask_SetAllNonAttackersInvisiblity(u8 taskId)
{
    u16 battler;

    for (battler = 0; battler < MAX_BATTLERS_COUNT; ++battler)
	{
        if (battler != gBattleAnimAttacker && IsBattlerSpriteVisible(battler))
            gSprites[gBattlerSpriteIds[battler]].invisible = gBattleAnimArgs[0];
	}
    DestroyAnimVisualTask(taskId);
}

void StartMonScrollingBgMask(u8 taskId, u16 yMovementSpeed, u8 battler1, bool8 bothMons, u8 targetBlendCoeff, u8 blendDelay, u8 duration, const u32 *gfx, const u32 *tilemap, const u32 *palette)
{
    u16 species;
    u8 spriteId, newSpriteId = 0;
    u16 bg1Cnt;
    struct BattleAnimBgData animBgData;
    u8 battler2 = BATTLE_PARTNER(battler1);

    if (bothMons && !IsBattlerSpriteVisible(battler2))
        bothMons = FALSE;
	
    gBattle_WIN0H = 0;
    gBattle_WIN0V = 0;
	
    SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG_ALL | WININ_WIN1_OBJ | WININ_WIN1_CLR);
    SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG2 | WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ  | WINOUT_WIN01_CLR | WINOUT_WINOBJ_BG_ALL | WINOUT_WINOBJ_OBJ | WINOUT_WINOBJ_CLR);
    SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJWIN_ON);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG1 | BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 16));
	
    bg1Cnt = GetGpuReg(REG_OFFSET_BG1CNT);
    ((vBgCnt *)&bg1Cnt)->priority = 0;
    ((vBgCnt *)&bg1Cnt)->screenSize = 0;
    ((vBgCnt *)&bg1Cnt)->areaOverflowMode = 1;
    ((vBgCnt *)&bg1Cnt)->charBaseBlock = 1;
    SetGpuReg(REG_OFFSET_BG1CNT, bg1Cnt);
	
	species = GetMonData(GetBattlerPartyIndexPtr(battler1), MON_DATA_SPECIES);
    spriteId = CreateCloneOfSpriteInWindowMode(gBattlerSpriteIds[battler1], species);
	
    if (bothMons)
        newSpriteId = CreateCloneOfSpriteInWindowMode(gBattlerSpriteIds[battler2], species);
	
    GetBattleAnimBgData(&animBgData, 1);
    AnimLoadCompressedBgTilemap(animBgData.bgId, tilemap);
    AnimLoadCompressedBgGfx(animBgData.bgId, gfx, animBgData.tilesOffset);
    LoadCompressedPalette(palette, animBgData.paletteId * 16, 32);
	
    gBattle_BG1_X = 0;
    gBattle_BG1_Y = 0;
	
    gTasks[taskId].data[1] = yMovementSpeed;
    gTasks[taskId].data[4] = targetBlendCoeff;
    gTasks[taskId].data[5] = duration;
    gTasks[taskId].data[6] = blendDelay;
    gTasks[taskId].data[0] = spriteId;
    gTasks[taskId].data[2] = bothMons;
    gTasks[taskId].data[3] = newSpriteId;
    gTasks[taskId].func = AnimTask_MonScrollingBgMask;
}

static void AnimTask_MonScrollingBgMask(u8 taskId)
{
    gTasks[taskId].data[13] += gTasks[taskId].data[1] < 0 ? -gTasks[taskId].data[1] : gTasks[taskId].data[1];
	
    if (gTasks[taskId].data[1] < 0)
        gBattle_BG1_Y -= gTasks[taskId].data[13] >> 8;
    else
        gBattle_BG1_Y += gTasks[taskId].data[13] >> 8;
	
    gTasks[taskId].data[13] &= 0xFF;
	
    switch (gTasks[taskId].data[15])
    {
    case 0:
        if (gTasks[taskId].data[11]++ >= gTasks[taskId].data[6])
        {
            gTasks[taskId].data[11] = 0;
			
            ++gTasks[taskId].data[12];
			
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[12], 16 - gTasks[taskId].data[12]));
			
            if (gTasks[taskId].data[12] == gTasks[taskId].data[4])
                ++gTasks[taskId].data[15];
        }
        break;
    case 1:
        if (++gTasks[taskId].data[10] == gTasks[taskId].data[5])
            ++gTasks[taskId].data[15];
        break;
    case 2:
        if (gTasks[taskId].data[11]++ >= gTasks[taskId].data[6])
        {
            u16 bg1Cnt;
            
            gTasks[taskId].data[11] = 0;
			
            --gTasks[taskId].data[12];
			
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[12], 16 - gTasks[taskId].data[12]));
			
            if (gTasks[taskId].data[12] == 0)
            {
                ResetBattleAnimBg(0);
				
                gBattle_WIN0H = 0;
                gBattle_WIN0V = 0;
				
                SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG_ALL | WININ_WIN1_OBJ | WININ_WIN1_CLR);
                SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG_ALL  | WINOUT_WIN01_OBJ  | WINOUT_WIN01_CLR | WINOUT_WINOBJ_BG_ALL | WINOUT_WINOBJ_OBJ | WINOUT_WINOBJ_CLR);
				
                bg1Cnt = GetGpuReg(REG_OFFSET_BG1CNT);
                ((vBgCnt *)&bg1Cnt)->charBaseBlock = 0;
                SetGpuReg(REG_OFFSET_BG1CNT, bg1Cnt);
				
                SetGpuReg(REG_OFFSET_DISPCNT, GetGpuReg(REG_OFFSET_DISPCNT) ^ DISPCNT_OBJWIN_ON);
                
                DestroySprite(&gSprites[gTasks[taskId].data[0]]);
				
                if (gTasks[taskId].data[2])
                    DestroySprite(&gSprites[gTasks[taskId].data[3]]);
				
                DestroyAnimVisualTaskAndDisableBlend(taskId);
            }
        }
        break;
    }
}

// Allocate memory for a backup of the palettes. Used by MOVE_OVERHEAT.
// No args.
void AnimTask_AllocBackupPalBuffer(u8 taskId)
{
    gMonSpritesGfxPtr->multiUseBuffer = AllocZeroed(0x2000);
    DestroyAnimVisualTask(taskId);
}

// Frees the palette memory backup.
// No args.
void AnimTask_FreeBackupPalBuffer(u8 taskId)
{
    FREE_AND_SET_NULL(gMonSpritesGfxPtr->multiUseBuffer);
    DestroyAnimVisualTask(taskId);
}

// Copy the specified palette from gPlttBufferUnfaded to the palette backup memory.
// arg 0: case id
// arg 1: multiuse buffer pal index
void AnimTask_CopyPalUnfadedToBackup(u8 taskId)
{
    u32 selectedPalettes;
    s32 paletteIndex = 0;
	
	switch (gBattleAnimArgs[0])
	{
		case 0:
			for (selectedPalettes = SelectBattleAnimSpriteAndBgPalettes(TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE); (selectedPalettes & 1) == 0; ++paletteIndex)
				selectedPalettes >>= 1;
			break;
		case 1:
			paletteIndex = gBattleAnimAttacker + 16;
			break;
		case 2:
			paletteIndex = gBattleAnimTarget + 16;
			break;
	}
    memcpy(&gMonSpritesGfxPtr->multiUseBuffer[gBattleAnimArgs[1] * 16], &gPlttBufferUnfaded[paletteIndex * 16], 32);
    DestroyAnimVisualTask(taskId);
}

// Copy the specified palette from the palette backup memory to the gPlttBufferUnfaded.
// arg 0: case id
// arg 1: multiuse buffer pal index
void AnimTask_CopyPalUnfadedFromBackup(u8 taskId)
{
    u32 selectedPalettes;
    s32 paletteIndex = 0;
	
	switch (gBattleAnimArgs[0])
	{
		case 0:
			for (selectedPalettes = SelectBattleAnimSpriteAndBgPalettes(TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE); (selectedPalettes & 1) == 0; ++paletteIndex)
				selectedPalettes >>= 1;
			break;
		case 1:
			paletteIndex = gBattleAnimAttacker + 16;
			break;
		case 2:
			paletteIndex = gBattleAnimTarget + 16;
			break;
	}
    memcpy(&gPlttBufferUnfaded[paletteIndex * 16], &gMonSpritesGfxPtr->multiUseBuffer[gBattleAnimArgs[1] * 16], 32);
    DestroyAnimVisualTask(taskId);
}

// Copy the specified palette from gPlttBufferFaded to gPlttBufferUnfaded.
// arg 0: case id
void AnimTask_CopyPalFadedToUnfaded(u8 taskId)
{
    u32 selectedPalettes;
    s32 paletteIndex = 0;
	
	switch (gBattleAnimArgs[0])
	{
		case 0:
			for (selectedPalettes = SelectBattleAnimSpriteAndBgPalettes(TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE); (selectedPalettes & 1) == 0; ++paletteIndex)
				selectedPalettes >>= 1;
			break;
		case 1:
			paletteIndex = gBattleAnimAttacker + 16;
			break;
		case 2:
			paletteIndex = gBattleAnimTarget + 16;
			break;
	}
    memcpy(&gPlttBufferUnfaded[paletteIndex * 16], &gPlttBufferFaded[paletteIndex * 16], 32);
    DestroyAnimVisualTask(taskId);
}

void AnimTask_SetAnimAttackerAndTargetForEffectTgt(u8 taskId)
{
    gBattleAnimAttacker = gBattlerTarget;
    gBattleAnimTarget = gEffectBattler;
    DestroyAnimVisualTask(taskId);
}

// Gets the anim target from anim arg 2.
// No args.
void AnimTask_SetAnimTargetFromArg(u8 taskId)
{
    gBattleAnimTarget = gBattleSpritesDataPtr->animationData->animArg >> 8;;
    DestroyAnimVisualTask(taskId);
}

void AnimTask_SetAttackerInvisibleWaitForSignal(u8 taskId)
{
    gTasks[taskId].data[0] = gBattleSpritesDataPtr->battlerData[gBattleAnimAttacker].invisible;
    gBattleSpritesDataPtr->battlerData[gBattleAnimAttacker].invisible = TRUE;
    gTasks[taskId].func = AnimTask_SetAttackerInvisibleWaitForSignal_Step;
    --gAnimVisualTaskCount;
}

static void AnimTask_SetAttackerInvisibleWaitForSignal_Step(u8 taskId)
{
    if (gBattleAnimArgs[ARG_RET_ID] == -2)
    {
        gBattleSpritesDataPtr->battlerData[gBattleAnimAttacker].invisible = (u8)gTasks[taskId].data[0] & 1;
        DestroyTask(taskId);
    }
}

void AnimTask_GetBattleTerrain(u8 taskId)
{
	gBattleAnimArgs[ARG_RET_ID] = gBattleTerrain;
	DestroyAnimVisualTask(taskId);
}

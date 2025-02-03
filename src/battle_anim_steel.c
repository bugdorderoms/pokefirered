#include "global.h"
#include "gflib.h"
#include "battle_anim.h"
#include "graphics.h"

static void AnimTask_MetallicShine_Step(u8 taskId);

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

// Makes the attacker metallic and shining.
// Used by MOVE_HARDEN and MOVE_IRON_DEFENSE.
// arg 0: if true won't change battler's palette back
// arg 1: if true, use custom color
// arg 2: custom color
// Custom color argument is used in MOVE_POISON_TAIL to make the mon turn purplish/pinkish as if became cloaked in poison.
void AnimTask_MetallicShine(u8 taskId)
{
    u8 spriteId, newSpriteId;
    u16 paletteNum;
    struct BattleAnimBgData animBg;
    bool8 priorityChanged;

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
    newSpriteId = CreateCloneOfSpriteInWindowMode(spriteId, GetMonData(GetBattlerPartyIndexPtr(gBattleAnimAttacker), MON_DATA_SPECIES));
	
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

static void AnimTask_MetallicShine_Step(u8 taskId)
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
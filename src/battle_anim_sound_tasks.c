#include "global.h"
#include "gflib.h"
#include "battle.h"
#include "battle_anim.h"
#include "task.h"
#include "constants/battle_anim.h"
#include "constants/sound.h"

static void SoundTask_FireBlastStep(u8 taskId);
static void SoundTask_FireBlastStep2(u8 taskId);
static void SoundTask_LoopSEAdjustPanning_Step(u8 taskId);
static void SoundTask_PlayDoubleCry_Step(u8 taskId);
static void SoundTask_PlayCryWithEcho_Step(u8 taskId);
static void SoundTask_AdjustPanningVar_Step(u8 taskId);

// Loops the specified sound effect and pans from the attacker to the target.
// The second specified sound effect is played at the very end. This task is effectively
// hardcoded to the move FIRE_BLAST due to the baked-in durations.
// arg 0: looped sound effect
// arg 1: ending sound effect
void SoundTask_FireBlast(u8 taskId)
{
    s8 pan1, pan2;

    gTasks[taskId].data[0] = gBattleAnimArgs[0];
    gTasks[taskId].data[1] = gBattleAnimArgs[1];
	
    pan1 = BattleAnimAdjustPanning(SOUND_PAN_ATTACKER);
    pan2 = BattleAnimAdjustPanning(SOUND_PAN_TARGET);
	
    gTasks[taskId].data[2] = pan1;
    gTasks[taskId].data[3] = pan2;
    gTasks[taskId].data[4] = CalculatePanIncrement(pan1, pan2, 2);
    gTasks[taskId].data[10] = 10;
    gTasks[taskId].func = SoundTask_FireBlastStep;
}

static void SoundTask_FireBlastStep(u8 taskId)
{
    s16 pan = gTasks[taskId].data[2];
    s8 panIncrement = gTasks[taskId].data[4];

    if (++gTasks[taskId].data[11] == 111)
    {
		gTasks[taskId].data[11] = 0;
        gTasks[taskId].data[10] = 5;
        gTasks[taskId].func = SoundTask_FireBlastStep2;
    }
    else
    {
        if (++gTasks[taskId].data[10] == 11)
        {
            gTasks[taskId].data[10] = 0;
            PlaySE12WithPanning(gTasks[taskId].data[0], pan);
        }
        pan += panIncrement;
        gTasks[taskId].data[2] = KeepPanInRange(pan, panIncrement);
    }
}

static void SoundTask_FireBlastStep2(u8 taskId)
{
    if (++gTasks[taskId].data[10] == 6)
    {
        gTasks[taskId].data[10] = 0;
		
        PlaySE12WithPanning(gTasks[taskId].data[1], BattleAnimAdjustPanning(SOUND_PAN_TARGET));
		
        if (++gTasks[taskId].data[11] == 2)
            DestroyAnimSoundTask(taskId);
    }
}

// Loop the specified SE, adjusting the pan.
// arg 0: se id
// arg 1: initial pan
// arg 2: target pan
// arg 3: pan increment
// arg 4: duration
// arg 5: pan change delay
// arg 6: next se play delay
void SoundTask_LoopSEAdjustPanning(u8 taskId)
{
    u16 songId = gBattleAnimArgs[0];
    s8 targetPan = gBattleAnimArgs[2];
    s8 panIncrement = gBattleAnimArgs[3];
    u8 r10 = gBattleAnimArgs[4];
    u8 r7 = gBattleAnimArgs[5];
    u8 r9 = gBattleAnimArgs[6];
    s8 sourcePan = BattleAnimAdjustPanning(gBattleAnimArgs[1]);

    targetPan = BattleAnimAdjustPanning(targetPan);
    gTasks[taskId].data[0] = songId;
    gTasks[taskId].data[1] = sourcePan;
    gTasks[taskId].data[2] = targetPan;
    gTasks[taskId].data[3] = CalculatePanIncrement(sourcePan, targetPan, panIncrement);
    gTasks[taskId].data[4] = r10;
    gTasks[taskId].data[5] = r7;
    gTasks[taskId].data[6] = r9;
    gTasks[taskId].data[10] = 0;
    gTasks[taskId].data[11] = sourcePan;
    gTasks[taskId].data[12] = r9;
    gTasks[taskId].func = SoundTask_LoopSEAdjustPanning_Step;
    SoundTask_LoopSEAdjustPanning_Step(taskId);
}

static void SoundTask_LoopSEAdjustPanning_Step(u8 taskId)
{
    if (gTasks[taskId].data[12]++ == gTasks[taskId].data[6])
    {
        gTasks[taskId].data[12] = 0;
		
        PlaySE12WithPanning(gTasks[taskId].data[0], gTasks[taskId].data[11]);
		
        if (--gTasks[taskId].data[4] == 0)
        {
            DestroyAnimSoundTask(taskId);
            return;
        }
    }
	
    if (gTasks[taskId].data[10]++ == gTasks[taskId].data[5])
    {
        u16 dPan, oldPan;

        gTasks[taskId].data[10] = 0;
		
        dPan = gTasks[taskId].data[3];
        oldPan = gTasks[taskId].data[11];
		
        gTasks[taskId].data[11] = dPan + oldPan;
        gTasks[taskId].data[11] = KeepPanInRange(gTasks[taskId].data[11], oldPan);
    }
}

// Plays the given battler's cry with a high pitch.
// arg 0: anim battler
void SoundTask_PlayCryHighPitch(u8 taskId)
{
    u16 species;
    u8 battlerId = GetBattlerForAnimScript(gBattleAnimArgs[0]);

    // Check if battler is visible.
    if ((gBattleAnimArgs[0] == ANIM_TARGET || gBattleAnimArgs[0] == ANIM_DEF_PARTNER) && !IsBattlerSpriteVisible(battlerId))
    {
        DestroyAnimVisualTask(taskId);
        return;
    }
	species = GetMonData(GetBattlerPartyIndexPtr(battlerId), MON_DATA_SPECIES);
	
    if (species)
        PlayCry_ByMode(species, BattleAnimAdjustPanning(SOUND_PAN_ATTACKER), CRY_MODE_HIGH_PITCH);
	
    DestroyAnimVisualTask(taskId);
}

// Plays an Pokémon cry doubled.
// arg 0: anim battler
// arg 1: first cry's mode
// arg 2: second cry's mode
void SoundTask_PlayDoubleCry(u8 taskId)
{
	s8 pan;
    u8 battlerId = GetBattlerForAnimScript(gBattleAnimArgs[0]);
	u16 species;
    
    // Check if battler is visible.
    if ((gBattleAnimArgs[0] == ANIM_TARGET || gBattleAnimArgs[0] == ANIM_DEF_PARTNER) && !IsBattlerSpriteVisible(battlerId))
    {
        DestroyAnimVisualTask(taskId);
        return;
    }
    species = GetMonData(GetBattlerPartyIndexPtr(battlerId), MON_DATA_SPECIES);
	
    if (species)
    {
		pan = BattleAnimAdjustPanning(SOUND_PAN_ATTACKER);
		
		PlayCry_ByMode(species, pan, gBattleAnimArgs[1]);
		
		gTasks[taskId].data[0] = gBattleAnimArgs[2];
		gTasks[taskId].data[1] = species;
		gTasks[taskId].data[2] = pan;
        gTasks[taskId].func = SoundTask_PlayDoubleCry_Step;
    }
    else
        DestroyAnimVisualTask(taskId);
}

static void SoundTask_PlayDoubleCry_Step(u8 taskId)
{
    if (gTasks[taskId].data[9] < 2)
        ++gTasks[taskId].data[9];
	else
	{
		if (!IsCryPlaying())
		{
			PlayCry_ByMode(gTasks[taskId].data[1], gTasks[taskId].data[2], gTasks[taskId].data[0]);
            DestroyAnimVisualTask(taskId);
		}
	}
}

void SoundTask_WaitForCry(u8 taskId)
{
    if (gTasks[taskId].data[9] < 2)
        ++gTasks[taskId].data[9];
    else if (!IsCryPlaying())
        DestroyAnimVisualTask(taskId);
}

// Plays the attacker's mon cry with an echo.
// No args.
void SoundTask_PlayCryWithEcho(u8 taskId)
{
    u16 species = gAnimBattlerSpecies[gBattleAnimAttacker];

    if (species)
    {
		s8 pan = BattleAnimAdjustPanning(SOUND_PAN_ATTACKER);
		
        PlayCry_ByMode(species, pan, CRY_MODE_ECHO_START);
		
		gTasks[taskId].data[1] = species;
		gTasks[taskId].data[2] = pan;
        gTasks[taskId].func = SoundTask_PlayCryWithEcho_Step;
    }
    else
        DestroyAnimVisualTask(taskId);
}

static void SoundTask_PlayCryWithEcho_Step(u8 taskId)
{
    if (gTasks[taskId].data[9] < 2)
        ++gTasks[taskId].data[9];
    else if (!IsCryPlaying())
    {
        PlayCry_ByMode(gTasks[taskId].data[1], gTasks[taskId].data[2], CRY_MODE_ECHO_END);
        DestroyAnimVisualTask(taskId);
    }
}

// Simple, plays an se1 with given panning.
// arg 0: se id
// arg 1: panning
void SoundTask_PlaySE1WithPanning(u8 taskId)
{
    PlaySE1WithPanning(gBattleAnimArgs[0], BattleAnimAdjustPanning(gBattleAnimArgs[1]));
    DestroyAnimVisualTask(taskId);
}

// Simple, plays an se2 with given panning.
// arg 0: se id
// arg 1: panning
void SoundTask_PlaySE2WithPanning(u8 taskId)
{
    PlaySE2WithPanning(gBattleAnimArgs[0], BattleAnimAdjustPanning(gBattleAnimArgs[1]));
    DestroyAnimVisualTask(taskId);
}

// Adjusts panning and assigns it to gAnimCustomPanning. Doesnt play sound.
// Used by Confuse Ray and Will-O-Wisp (see uses of gAnimCustomPanning)
// arg 0: initial pan
// arg 1: target pan
// arg 2: pan increment
// arg 3: (?)
void SoundTask_AdjustPanningVar(u8 taskId)
{
    s8 targetPan = gBattleAnimArgs[1];
    s8 panIncrement = gBattleAnimArgs[2];
    u16 r9 = gBattleAnimArgs[3];
    s8 sourcePan = BattleAnimAdjustPanning(gBattleAnimArgs[0]);

    targetPan = BattleAnimAdjustPanning(targetPan);
    panIncrement = CalculatePanIncrement(sourcePan, targetPan, panIncrement);
	
    gTasks[taskId].data[1] = sourcePan;
    gTasks[taskId].data[2] = targetPan;
    gTasks[taskId].data[3] = panIncrement;
    gTasks[taskId].data[5] = r9;
    gTasks[taskId].data[10] = 0;
    gTasks[taskId].data[11] = sourcePan;
    gTasks[taskId].func = SoundTask_AdjustPanningVar_Step;
    SoundTask_AdjustPanningVar_Step(taskId);
}

static void SoundTask_AdjustPanningVar_Step(u8 taskId)
{
    u16 oldPan, panIncrement;

    if (gTasks[taskId].data[10]++ == gTasks[taskId].data[5])
    {
        gTasks[taskId].data[10] = 0;
		
        oldPan = gTasks[taskId].data[11];
		panIncrement = gTasks[taskId].data[3];
		
        gTasks[taskId].data[11] = panIncrement + oldPan; 
        gTasks[taskId].data[11] = KeepPanInRange(gTasks[taskId].data[11], oldPan);
    }
    gAnimCustomPanning = gTasks[taskId].data[11];
	
    if (gTasks[taskId].data[11] == gTasks[taskId].data[2])
        DestroyAnimVisualTask(taskId);
}

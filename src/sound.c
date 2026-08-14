#include "global.h"
#include "gba/m4a_internal.h"
#include "gflib.h"
#include "battle.h"
#include "m4a.h"
#include "constants/songs.h"
#include "constants/sound.h"
#include "task.h"

// TODO: what are these
extern u8 gDisableMapMusicChangeOnMapLoad;
extern u8 gDisableHelpSystemVolumeReduce;

// ewram
EWRAM_DATA struct MusicPlayerInfo* gMPlay_PokemonCry = NULL;
EWRAM_DATA u8 gPokemonCryBGMDuckingCounter = 0;

// iwram bss
static u16 sCurrentMapMusic;
static u16 sNextMapMusic;
static u8 sMapMusicState;
static u8 sMapMusicFadeInSpeed;
static u16 sFanfareCounter;

// iwram common
bool8 gDisableMusic;

extern u32 gBattleTypeFlags;
extern struct MusicPlayerInfo gMPlayInfo_BGM;
extern struct MusicPlayerInfo gMPlayInfo_SE1;
extern struct MusicPlayerInfo gMPlayInfo_SE2;
extern struct MusicPlayerInfo gMPlayInfo_SE3;
extern struct ToneData gCryTable[];
extern struct ToneData gCryTableReverse[];

static void Task_Fanfare(u32 taskId);
static void CreateFanfareTask(void);
static void Task_DuckBGMForPokemonCry(u32 taskId);
static void RestoreBGMVolumeAfterPokemonCry(void);

#define CRY_VOLUME 120 // was 125 in R/S

struct
{
    u16 songNum;
    u16 duration;
} static const sFanfares[] =
{
    [FANFARE_LEVEL_UP]      = { MUS_LEVEL_UP,         80 },
    [FANFARE_OBTAIN_ITEM]   = { MUS_OBTAIN_ITEM,     160 },
    [FANFARE_EVOLVED]       = { MUS_EVOLVED,         220 },
    [FANFARE_OBTAIN_TMHM]   = { MUS_OBTAIN_TMHM,     220 },
    [FANFARE_HEAL]          = { MUS_HEAL,            160 },
    [FANFARE_OBTAIN_BADGE]  = { MUS_OBTAIN_BADGE,    340 },
    [FANFARE_MOVE_DELETED]  = { MUS_MOVE_DELETED,    180 },
    [FANFARE_OBTAIN_BERRY]  = { MUS_OBTAIN_BERRY,    120 },
    [FANFARE_SLOTS_JACKPOT] = { MUS_SLOTS_JACKPOT,   250 },
    [FANFARE_SLOTS_WIN]     = { MUS_SLOTS_WIN,       150 },
    [FANFARE_TOO_BAD]       = { MUS_TOO_BAD,         160 },
    [FANFARE_POKEFLUTE]     = { MUS_POKE_FLUTE,      450 },
    [FANFARE_KEY_ITEM]      = { MUS_OBTAIN_KEY_ITEM, 170 },
    [FANFARE_DEX_EVAL]      = { MUS_DEX_RATING,      196 }
};

struct
{
    u8 release;
    u8 length;
    u8 chorus;
    bool8 overrideVolume:1;
    bool8 reverse:1;
    u32 pitch;
} static const sCrySoundData[] =
{
    [CRY_MODE_NORMAL] =
    {
        .release = 0,
        .length = 140,
        .pitch = 15360,
        .chorus = 0,
        .reverse = FALSE,
    },
    [CRY_MODE_DOUBLES] =
    {
        .release = 225,
        .length = 20,
        .pitch = 15360,
        .chorus = 0,
        .reverse = FALSE,
    },
    [CRY_MODE_ENCOUNTER] =
    {
        .release = 225,
        .length = 140,
        .pitch = 15600,
        .chorus = 20,
        .overrideVolume = TRUE,
        .reverse = FALSE,
    },
    [CRY_MODE_HIGH_PITCH] =
    {
        .release = 200,
        .length = 50,
        .pitch = 15800,
        .chorus = 20,
        .overrideVolume = TRUE,
        .reverse = FALSE,
    },
    [CRY_MODE_FAINT] =
    {
        .release = 200,
        .length = 140,
        .pitch = 14440,
        .chorus = 0,
        .reverse = FALSE,
    },
    [CRY_MODE_ECHO_START] =
    {
        .release = 100,
        .length = 25,
        .pitch = 15600,
        .chorus = 192,
        .overrideVolume = TRUE,
        .reverse = TRUE,
    },
    [CRY_MODE_ECHO_END] =
    {
        .release = 220,
        .length = 140,
        .pitch = 15555,
        .chorus = 192,
        .overrideVolume = TRUE,
        .reverse = FALSE,
    },
    [CRY_MODE_ROAR_1] =
    {
        .release = 100,
        .length = 10,
        .pitch = 14848,
        .chorus = 0,
        .reverse = FALSE,
    },
    [CRY_MODE_ROAR_2] =
    {
        .release = 225,
        .length = 60,
        .pitch = 15616,
        .chorus = 0,
        .reverse = FALSE,
    },
    [CRY_MODE_GROWL_1] =
    {
        .release = 125,
        .length = 15,
        .pitch = 15200,
        .chorus = 0,
        .reverse = TRUE,
    },
    [CRY_MODE_GROWL_2] =
    {
        .release = 225,
        .length = 100,
        .pitch = 15200,
        .chorus = 0,
        .reverse = FALSE,
    },
    [CRY_MODE_WEAK] =
    {
        .release = 0,
        .length = 140,
        .pitch = 15000,
        .chorus = 0,
        .reverse = FALSE,
    },
    [CRY_MODE_WEAK_DOUBLES] =
    {
        .release = 225,
        .length = 20,
        .pitch = 15360,
        .chorus = 0,
        .reverse = FALSE,
    },
    [CRY_MODE_DYNAMAX] =
    {
        .release = 255,
        .length = 255,
        .pitch = 12150,
        .chorus = 200,
        .reverse = FALSE,
    },
};

void InitMapMusic(void)
{
    gDisableMusic = FALSE;
    ResetMapMusic();
}

void MapMusicMain(void)
{
    switch (sMapMusicState)
    {
    case 0:
        break;
    case 1:
        sMapMusicState = 2;
        PlayBGM(sCurrentMapMusic);
        break;
    case 2:
    case 3:
    case 4:
        break;
    case 5:
        if (IsBGMStopped())
        {
            sNextMapMusic = 0;
            sMapMusicState = 0;
        }
        break;
    case 6:
        if (IsBGMStopped() && IsFanfareTaskInactive())
        {
            sCurrentMapMusic = sNextMapMusic;
            sNextMapMusic = 0;
            sMapMusicState = 2;
            PlayBGM(sCurrentMapMusic);
        }
        break;
    case 7:
        if (IsBGMStopped() && IsFanfareTaskInactive())
        {
            FadeInNewBGM(sNextMapMusic, sMapMusicFadeInSpeed);
            sCurrentMapMusic = sNextMapMusic;
            sNextMapMusic = 0;
            sMapMusicState = 2;
            sMapMusicFadeInSpeed = 0;
        }
        break;
    }
}

void ResetMapMusic(void)
{
    sCurrentMapMusic = 0;
    sNextMapMusic = 0;
    sMapMusicState = 0;
    sMapMusicFadeInSpeed = 0;
}

u32 GetCurrentMapMusic(void)
{
    return sCurrentMapMusic;
}

void PlayNewMapMusic(u32 songNum)
{
    sCurrentMapMusic = songNum;
    sNextMapMusic = 0;
    sMapMusicState = 1;
}

void StopMapMusic(void)
{
    sCurrentMapMusic = 0;
    sNextMapMusic = 0;
    sMapMusicState = 1;
}

void FadeOutMapMusic(u32 speed)
{
    if (IsNotWaitingForBGMStop())
        FadeOutBGM(speed);
    
    sCurrentMapMusic = 0;
    sNextMapMusic = 0;
    sMapMusicState = 5;
}

void FadeOutAndPlayNewMapMusic(u32 songNum, u32 speed)
{
    FadeOutMapMusic(speed);
    sCurrentMapMusic = 0;
    sNextMapMusic = songNum;
    sMapMusicState = 6;
}

void FadeOutAndFadeInNewMapMusic(u32 songNum, u32 fadeOutSpeed, u32 fadeInSpeed)
{
    FadeOutMapMusic(fadeOutSpeed);
    sCurrentMapMusic = 0;
    sNextMapMusic = songNum;
    sMapMusicState = 7;
    sMapMusicFadeInSpeed = fadeInSpeed;
}

static void FadeInNewMapMusic(u32 songNum, u32 speed)
{
    FadeInNewBGM(songNum, speed);
    sCurrentMapMusic = songNum;
    sNextMapMusic = 0;
    sMapMusicState = 2;
    sMapMusicFadeInSpeed = 0;
}

bool32 IsNotWaitingForBGMStop(void)
{
    if (sMapMusicState == 6)
        return FALSE;
    if (sMapMusicState == 5)
        return FALSE;
    if (sMapMusicState == 7)
        return FALSE;
    return TRUE;
}

void PlayFanfareByFanfareNum(u32 fanfareNum)
{
    m4aMPlayStop(&gMPlayInfo_BGM);
    sFanfareCounter = sFanfares[fanfareNum].duration;
    m4aSongNumStart(sFanfares[fanfareNum].songNum);
}

void PlayFanfare(u32 songNum)
{
    u32 i;
    
    for (i = 0; i < ARRAY_COUNT(sFanfares); i++)
    {
        if (sFanfares[i].songNum == songNum)
        {
            PlayFanfareByFanfareNum(i);
            CreateFanfareTask();
            return;
        }
    }
    PlayFanfareByFanfareNum(FANFARE_LEVEL_UP);
    CreateFanfareTask();
}

bool32 WaitFanfare(bool32 stop)
{
    if (sFanfareCounter)
    {
        sFanfareCounter--;
        return FALSE;
    }
    else
    {
        if (!stop)
            m4aMPlayContinue(&gMPlayInfo_BGM);
        else
            m4aSongNumStart(MUS_DUMMY);

        return TRUE;
    }
}

void StopFanfareByFanfareNum(u32 fanfareNum)
{
    m4aSongNumStop(sFanfares[fanfareNum].songNum);
}

bool32 IsFanfareTaskInactive(void)
{
    if (FuncIsActiveTask(Task_Fanfare))
        return FALSE;
    
    return TRUE;
}

static void Task_Fanfare(u32 taskId)
{
    if (sFanfareCounter)
        sFanfareCounter--;
    else
    {
        m4aMPlayContinue(&gMPlayInfo_BGM);
        DestroyTask(taskId);
    }
}

static void CreateFanfareTask(void)
{
    if (!FuncIsActiveTask(Task_Fanfare))
        CreateTask(Task_Fanfare, 80);
}

void FadeInNewBGM(u32 songNum, u32 speed)
{
    if (gDisableMusic)
        songNum = 0;
    
    if (songNum == MUS_NONE)
        songNum = 0;
    
    m4aSongNumStart(songNum);
    m4aMPlayImmInit(&gMPlayInfo_BGM);
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 0);
    m4aSongNumStop(songNum);
    m4aMPlayFadeIn(&gMPlayInfo_BGM, speed);
}

void FadeOutBGMTemporarily(u32 speed)
{
    m4aMPlayFadeOutTemporarily(&gMPlayInfo_BGM, speed);
}

bool32 IsBGMPausedOrStopped(void)
{
    if (gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_PAUSE)
        return TRUE;
    else if (!(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK))
        return TRUE;
    else
        return FALSE;
}

void FadeInBGM(u32 speed)
{
    m4aMPlayFadeIn(&gMPlayInfo_BGM, speed);
}

void FadeOutBGM(u32 speed)
{
    m4aMPlayFadeOut(&gMPlayInfo_BGM, speed);
}

bool32 IsBGMStopped(void)
{
    if (!(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK))
        return TRUE;
    return FALSE;
}

static void PlayCry_WithDucking(u32 species, s8 pan, u32 mode)
{
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 85);
    PlayCryInternal(species, pan, CRY_VOLUME, CRY_PRIORITY_NORMAL, mode);
    gPokemonCryBGMDuckingCounter = 2;
    RestoreBGMVolumeAfterPokemonCry();
}

void PlayCry_Normal(u32 species, s8 pan)
{
    PlayCry_WithDucking(species, pan, CRY_MODE_NORMAL);
}

void PlayCry_NormalNoDucking(u32 species, s8 pan, s8 volume, u32 priority)
{
    PlayCryInternal(species, pan, volume, priority, CRY_MODE_NORMAL);
}

void PlayCry_ByMode(u32 species, s8 pan, u32 mode)
{
    if (mode == CRY_MODE_DOUBLES)
        PlayCryInternal(species, pan, CRY_VOLUME, CRY_PRIORITY_NORMAL, mode);
    else
        PlayCry_WithDucking(species, pan, mode);
}

void PlayCry_ReleaseDouble(u32 species, s8 pan, u32 mode)
{
    if (mode != CRY_MODE_DOUBLES && !(gBattleTypeFlags & BATTLE_TYPE_MULTI))
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 85);
    
    PlayCryInternal(species, pan, CRY_VOLUME, CRY_PRIORITY_NORMAL, mode);
}

void PlayCryInternal(u32 species, s8 pan, s8 volume, u32 priority, u32 mode)
{
    u32 cryId;
    
    if ((mode == CRY_MODE_NORMAL || mode == CRY_MODE_DOUBLES) && (gSpeciesInfo[species].flags & SPECIES_FLAG_HIGH_PITCH_CRY))
        mode = CRY_MODE_HIGH_PITCH;
    
    if (sCrySoundData[mode].overrideVolume)
        volume = 90;

    SetPokemonCryVolume(volume);
    SetPokemonCryPanpot(pan);
    SetPokemonCryPitch(sCrySoundData[mode].pitch);
    SetPokemonCryLength(sCrySoundData[mode].length);
    SetPokemonCryProgress(0);
    SetPokemonCryRelease(sCrySoundData[mode].release);
    SetPokemonCryChorus(sCrySoundData[mode].chorus);
    SetPokemonCryPriority(priority);
    
    cryId = gSpeciesInfo[species].cryId;
    gMPlay_PokemonCry = SetPokemonCryTone(sCrySoundData[mode].reverse ? &gCryTableReverse[cryId] : &gCryTable[cryId]);
}

bool32 IsCryFinished(void)
{
    if (!FuncIsActiveTask(Task_DuckBGMForPokemonCry))
    {
        ClearPokemonCrySongs();
        return TRUE;
    }
    return FALSE;
}

void StopCryAndClearCrySongs(void)
{
    m4aMPlayStop(gMPlay_PokemonCry);
    ClearPokemonCrySongs();
}

void StopCry(void)
{
    m4aMPlayStop(gMPlay_PokemonCry);
}

bool32 IsCryPlayingOrClearCrySongs(void)
{
    if (IsPokemonCryPlaying(gMPlay_PokemonCry))
        return TRUE;
    else
    {
        ClearPokemonCrySongs();
        return FALSE;
    }
}

bool32 IsCryPlaying(void)
{
    return IsPokemonCryPlaying(gMPlay_PokemonCry);
}

static void Task_DuckBGMForPokemonCry(u32 taskId)
{
    if (gPokemonCryBGMDuckingCounter)
    {
        gPokemonCryBGMDuckingCounter--;
        return;
    }

    if (!IsPokemonCryPlaying(gMPlay_PokemonCry))
    {
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 256);
        DestroyTask(taskId);
    }
}

static void RestoreBGMVolumeAfterPokemonCry(void)
{
    if (!FuncIsActiveTask(Task_DuckBGMForPokemonCry))
        CreateTask(Task_DuckBGMForPokemonCry, 80);
}

void PlayBGM(u32 songNum)
{
    if (gDisableMusic || songNum == MUS_NONE)
        songNum = 0;
    
    m4aSongNumStart(songNum);
}

void PlaySE(u32 songNum)
{
    if (gDisableMapMusicChangeOnMapLoad == 0)
        m4aSongNumStart(songNum);
}

void PlaySE12WithPanning(u32 songNum, s8 pan)
{
    m4aSongNumStart(songNum);
    m4aMPlayImmInit(&gMPlayInfo_SE1);
    m4aMPlayImmInit(&gMPlayInfo_SE2);
    m4aMPlayPanpotControl(&gMPlayInfo_SE1, 0xFFFF, pan);
    m4aMPlayPanpotControl(&gMPlayInfo_SE2, 0xFFFF, pan);
}

void PlaySE1WithPanning(u32 songNum, s8 pan)
{
    m4aSongNumStart(songNum);
    m4aMPlayImmInit(&gMPlayInfo_SE1);
    m4aMPlayPanpotControl(&gMPlayInfo_SE1, 0xFFFF, pan);
}

void PlaySE2WithPanning(u32 songNum, s8 pan)
{
    m4aSongNumStart(songNum);
    m4aMPlayImmInit(&gMPlayInfo_SE2);
    m4aMPlayPanpotControl(&gMPlayInfo_SE2, 0xFFFF, pan);
}

void SE12PanpotControl(s8 pan)
{
    m4aMPlayPanpotControl(&gMPlayInfo_SE1, 0xFFFF, pan);
    m4aMPlayPanpotControl(&gMPlayInfo_SE2, 0xFFFF, pan);
}

bool32 IsSEPlaying(void)
{
    if ((gMPlayInfo_SE1.status & MUSICPLAYER_STATUS_PAUSE) && (gMPlayInfo_SE2.status & MUSICPLAYER_STATUS_PAUSE))
        return FALSE;
    if (!(gMPlayInfo_SE1.status & MUSICPLAYER_STATUS_TRACK) && !(gMPlayInfo_SE2.status & MUSICPLAYER_STATUS_TRACK))
        return FALSE;
    return TRUE;
}

bool32 IsBGMPlaying(void)
{
    if (gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_PAUSE)
        return FALSE;
    if (!(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK))
        return FALSE;
    return TRUE;
}

bool32 IsSpecialSEPlaying(void)
{
    if (gMPlayInfo_SE3.status & MUSICPLAYER_STATUS_PAUSE)
        return FALSE;
    if (!(gMPlayInfo_SE3.status & MUSICPLAYER_STATUS_TRACK))
        return FALSE;
    return TRUE;
}

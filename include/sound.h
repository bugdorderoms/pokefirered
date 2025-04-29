#ifndef GUARD_SOUND_H
#define GUARD_SOUND_H

#include "global.h"

void InitMapMusic(void);
void MapMusicMain(void);
void ResetMapMusic(void);
u32 GetCurrentMapMusic(void);
void PlayNewMapMusic(u32 songNum);
void StopMapMusic(void);
void FadeOutMapMusic(u32 speed);
void FadeOutAndPlayNewMapMusic(u32 songNum, u32 speed);
void FadeOutAndFadeInNewMapMusic(u32 songNum, u32 fadeOutSpeed, u32 fadeInSpeed);
bool32 IsNotWaitingForBGMStop(void);
bool32 WaitFanfare(bool32 stop);
void PlayFanfareByFanfareNum(u32 fanfareNum);
void StopFanfareByFanfareNum(u32 fanfareNum);
void PlayFanfare(u32 songNum);
bool32 IsFanfareTaskInactive(void);
void FadeInNewBGM(u32 songNum, u32 speed);
void FadeOutBGMTemporarily(u32 speed);
bool32 IsBGMPausedOrStopped(void);
void FadeInBGM(u32 speed);
void FadeOutBGM(u32 speed);
bool32 IsBGMStopped(void);
void PlayCry_Normal(u32 species, s8 pan);
void PlayCry_NormalNoDucking(u32 species, s8 pan, s8 volume, u32 priority);
void PlayCry_ByMode(u32 species, s8 pan, u32 mode);
void PlayCry_ReleaseDouble(u32 species, s8 pan, u32 mode);
void PlayCryInternal(u32 species, s8 pan, s8 volume, u32 priority, u32 mode);
bool32 IsCryFinished(void);
void StopCryAndClearCrySongs(void);
void StopCry(void);
bool32 IsCryPlayingOrClearCrySongs(void);
bool32 IsCryPlaying(void);
void PlayBGM(u32 songNum);
void PlaySE(u32 songNum);
void PlaySE12WithPanning(u32 songNum, s8 pan);
void PlaySE1WithPanning(u32 songNum, s8 pan);
void PlaySE2WithPanning(u32 songNum, s8 pan);
void SE12PanpotControl(s8 pan);
bool32 IsSEPlaying(void);
bool32 IsBGMPlaying(void);
bool32 IsSpecialSEPlaying(void);

#endif // GUARD_SOUND_H

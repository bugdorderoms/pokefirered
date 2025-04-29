#ifndef GUARD_SAVE_FAILED_SCREEN_H
#define GUARD_SAVE_FAILED_SCREEN_H

void DoSaveFailedScreen(u32 saveType);
void SetNotInSaveFailedScreen(void);
bool32 RunSaveFailedScreen(void);
void CB2_FlashNotDetectedScreen(void);

#endif //GUARD_SAVE_FAILED_SCREEN_H

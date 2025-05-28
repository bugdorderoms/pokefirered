#ifndef GUARD_VS_SEEKER_H
#define GUARD_VS_SEEKER_H

#include "global.h"

void Task_VsSeeker_0(u32 taskId);
void ClearRematchStateByTrainerId(void);
void ClearRematchStateOfLastTalked(void);
u32 GetRematchTrainerId(u32 trainerId);
bool32 UpdateVsSeekerStepCounter(void);
void MapResetTrainerRematches(void);

#endif //GUARD_VS_SEEKER_H

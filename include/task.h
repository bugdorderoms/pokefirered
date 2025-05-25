#ifndef GUARD_TASK_H
#define GUARD_TASK_H

#include "global.h"

#define HEAD_SENTINEL 0xFE
#define TAIL_SENTINEL 0xFF

#define NUM_TASKS 16
#define NUM_TASK_DATA 16

typedef void (*TaskFunc)(u32 taskId);

struct Task
{
    TaskFunc func;
    bool8 isActive;
    u8 prev;
    u8 next;
    u8 priority;
    s16 data[NUM_TASK_DATA];
};

extern struct Task gTasks[];

void ResetTasks(void);
u32 CreateTask(TaskFunc func, u32 priority);
void DestroyTask(u32 taskId);
void RunTasks(void);
void TaskDummy(u32 taskId);
void SetTaskFuncWithFollowupFunc(u32 taskId, TaskFunc func, TaskFunc followupFunc);
void SwitchTaskToFollowupFunc(u32 taskId);
bool32 FuncIsActiveTask(TaskFunc func);
u32 FindTaskIdByFunc(TaskFunc func);
u32 GetTaskCount(void);
void SetWordTaskArg(u32 taskId, u32 dataElem, unsigned long value);
u32 GetWordTaskArg(u32 taskId, u32 dataElem);

#endif // GUARD_TASK_H

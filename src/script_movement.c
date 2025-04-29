#include "global.h"
#include "task.h"
#include "util.h"
#include "event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/event_object_movement.h"

static EWRAM_DATA const u8 (*sMovementScripts[OBJECT_EVENTS_COUNT]) = {};

static void ScriptMovement_StartMoveObjects(u32 priority);
static u32 GetMoveObjectsTaskId(void);
static bool32 ScriptMovement_TryAddNewMovement(u32 taskId, u32 objEventId, const u8 *movementScript);
static u32 GetMovementScriptIdFromObjectEventId(u32 taskId, u32 objEventId);
static bool32 IsMovementScriptFinished(u32 taskId, u32 moveScrId);
static void ScriptMovement_MoveObjects(u8 taskId);
static void ScriptMovement_AddNewMovement(u32 taskId, u32 moveScrId, u32 objEventId, const u8 *movementScript);
static void ScriptMovement_UnfreezeActiveObjects(u32 taskId);
static void ScriptMovement_TakeStep(u32 taskId, u32 moveScrId, u32 objEventId, const u8 *movementScript);

bool32 ScriptMovement_StartObjectMovementScript(u32 localId, u32 mapNum, u32 mapGroup, const u8 *movementScript)
{
    u8 objEventId;
	
    if (TryGetObjectEventIdByLocalIdAndMap(localId, mapNum, mapGroup, &objEventId))
        return TRUE;

    if (!FuncIsActiveTask(ScriptMovement_MoveObjects))
        ScriptMovement_StartMoveObjects(50);
	
    return ScriptMovement_TryAddNewMovement(GetMoveObjectsTaskId(), objEventId, movementScript);
}

bool32 ScriptMovement_IsObjectMovementFinished(u32 localId, u32 mapNum, u32 mapGroup)
{
    u8 objEventId;
    u32 taskId;
    u32 moveScrId;
	
    if (TryGetObjectEventIdByLocalIdAndMap(localId, mapNum, mapGroup, &objEventId))
        return TRUE;
	
    taskId = GetMoveObjectsTaskId();
    moveScrId = GetMovementScriptIdFromObjectEventId(taskId, objEventId);
    if (moveScrId == OBJECT_EVENTS_COUNT)
        return TRUE;
	
    return IsMovementScriptFinished(taskId, moveScrId);
}

void ScriptMovement_UnfreezeObjectEvents(void)
{
    u32 taskId = GetMoveObjectsTaskId();
    if (taskId != TAIL_SENTINEL)
    {
        ScriptMovement_UnfreezeActiveObjects(taskId);
        DestroyTask(taskId);
    }
}

void ScriptMovement_StartMoveObjects(u32 priority)
{
    u32 i, taskId = CreateTask(ScriptMovement_MoveObjects, priority);
	
    for (i = 1; i < NUM_TASK_DATA; i++)
        gTasks[taskId].data[i] = -1;
}

static u32 GetMoveObjectsTaskId(void)
{
    return FindTaskIdByFunc(ScriptMovement_MoveObjects);
}

static bool32 ScriptMovement_TryAddNewMovement(u32 taskId, u32 objEventId, const u8 *movementScript)
{
    u32 moveScrId;

    moveScrId = GetMovementScriptIdFromObjectEventId(taskId, objEventId);
    if (moveScrId != OBJECT_EVENTS_COUNT)
    {
        if (!IsMovementScriptFinished(taskId, moveScrId))
            return TRUE;
        else
        {
            ScriptMovement_AddNewMovement(taskId, moveScrId, objEventId, movementScript);
            return FALSE;
        }
    }
	
    moveScrId = GetMovementScriptIdFromObjectEventId(taskId, OBJ_EVENT_ID_PLAYER);
    if (moveScrId == OBJECT_EVENTS_COUNT)
        return TRUE;
    else
    {
        ScriptMovement_AddNewMovement(taskId, moveScrId, objEventId, movementScript);
        return FALSE;
    }
}

static u32 GetMovementScriptIdFromObjectEventId(u32 taskId, u32 objEventId)
{
    u32 i;
    u8 *moveScriptId = (u8 *)&gTasks[taskId].data[1];
	
    for (i = 0; i < OBJECT_EVENTS_COUNT; i++, moveScriptId++)
    {
        if (*moveScriptId == objEventId)
            return i;
    }
    return OBJECT_EVENTS_COUNT;
}

static void LoadObjectEventIdPtrFromMovementScript(u32 taskId, u32 moveScrId, u8 **pObjEventId)
{
    u32 i;
    *pObjEventId = (u8 *)&gTasks[taskId].data[1];
    for (i = 0; i < moveScrId; i++, (*pObjEventId)++);
}

static void SetObjectEventIdAtMovementScript(u32 taskId, u32 moveScrId, u8 objEventId)
{
    u8 *ptr;
    LoadObjectEventIdPtrFromMovementScript(taskId, moveScrId, &ptr);
    *ptr = objEventId;
}

static void LoadObjectEventIdFromMovementScript(u32 taskId, u32 moveScrId, u8 *objEventId)
{
    u8 *ptr;
    LoadObjectEventIdPtrFromMovementScript(taskId, moveScrId, &ptr);
    *objEventId = *ptr;
}

static void ClearMovementScriptFinished(u32 taskId, u32 moveScrId)
{
    u16 mask = ~(Bit(moveScrId));
    gTasks[taskId].data[0] &= mask;
}

static void SetMovementScriptFinished(u32 taskId, u32 moveScrId)
{
    gTasks[taskId].data[0] |= Bit(moveScrId);
}

static bool32 IsMovementScriptFinished(u32 taskId, u32 moveScrId)
{
    u16 moveScriptFinished = (u16)gTasks[taskId].data[0] & Bit(moveScrId);

    if (moveScriptFinished != 0)
        return TRUE;
    else
        return FALSE;
}

static void SetMovementScript(u32 moveScrId, const u8 *movementScript)
{
    sMovementScripts[moveScrId] = movementScript;
}

static const u8 *GetMovementScript(u32 moveScrId)
{
    return sMovementScripts[moveScrId];
}

static void ScriptMovement_AddNewMovement(u32 taskId, u32 moveScrId, u32 objEventId, const u8 *movementScript)
{
    ClearMovementScriptFinished(taskId, moveScrId);
    SetMovementScript(moveScrId, movementScript);
    SetObjectEventIdAtMovementScript(taskId, moveScrId, objEventId);
}

static void ScriptMovement_UnfreezeActiveObjects(u32 taskId)
{
	u32 i;
    u8 *pObjEventId = (u8 *)&gTasks[taskId].data[1];

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++, pObjEventId++)
    {
        if (*pObjEventId != 0xFF)
            UnfreezeObjectEvent(&gObjectEvents[*pObjEventId]);
    }
}

static void ScriptMovement_MoveObjects(u8 taskId)
{
    u32 i;
    u8 objEventId;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        LoadObjectEventIdFromMovementScript(taskId, i, &objEventId);
        if (objEventId != 0xFF)
            ScriptMovement_TakeStep(taskId, i, objEventId, GetMovementScript(i));
    }
}

static void ScriptMovement_TakeStep(u32 taskId, u32 moveScrId, u32 objEventId, const u8 *movementScript)
{
    u32 nextMoveActionId;

    if (IsMovementScriptFinished(taskId, moveScrId))
        return;
	
    if (ObjectEventIsHeldMovementActive(&gObjectEvents[objEventId])
        && !ObjectEventClearHeldMovementIfFinished(&gObjectEvents[objEventId]))
        return;

    nextMoveActionId = *movementScript;
    if (nextMoveActionId == MOVEMENT_ACTION_STEP_END)
    {
        SetMovementScriptFinished(taskId, moveScrId);
        FreezeObjectEvent(&gObjectEvents[objEventId]);
    }
    else
    {
        if (!ObjectEventSetHeldMovement(&gObjectEvents[objEventId], nextMoveActionId))
        {
            movementScript++;
            SetMovementScript(moveScrId, movementScript);
        }
    }
}

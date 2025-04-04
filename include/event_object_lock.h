#ifndef GUARD_EVENT_OBJECT_LOCK_H
#define GUARD_EVENT_OBJECT_LOCK_H

#include "global.h"

bool32 NativeScript_WaitPlayerStopMoving(void);
void ScriptFreezeObjectEvents(void);
bool32 NativeScript_WaitPlayerAndTargetNPCStopMoving(void);
void LockSelectedObjectEvent(void);
void ClearPlayerHeldMovementAndUnfreezeObjectEvents(void);
bool32 walkrun_is_standing_still(void);
void UnionRoom_UnlockPlayerAndChatPartner(void);

#endif // GUARD_EVENT_OBJECT_LOCK_H

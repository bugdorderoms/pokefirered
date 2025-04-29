#ifndef GUARD_SCRIPT_MOVEMENT_H
#define GUARD_SCRIPT_MOVEMENT_H

#include "global.h"

bool32 ScriptMovement_StartObjectMovementScript(u32, u32, u32, const u8 *);
bool32 ScriptMovement_IsObjectMovementFinished(u32, u32, u32);
void ScriptMovement_UnfreezeObjectEvents(void);

#endif // GUARD_SCRIPT_MOVEMENT_H

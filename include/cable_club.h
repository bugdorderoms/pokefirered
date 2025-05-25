#ifndef GUARD_CABLE_CLUB_H
#define GUARD_CABLE_CLUB_H

#include "task.h"

// Exported type declarations

// Exported RAM declarations

// Exported ROM declarations
void Task_WaitForReceivedRemoteLinkPlayers5SecondTimeout(u32 taskId);
u32 CreateTask_ReestablishLinkInCableClubRoom(void);
void CB2_ReturnFromCableClubBattle(void);
bool32 GetSeeingLinkPlayerCardMsg(u32 who);

#endif //GUARD_CABLE_CLUB_H

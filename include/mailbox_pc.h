#ifndef GUARD_MAILBOX_PC_H
#define GUARD_MAILBOX_PC_H

#include "player_pc.h"

bool32 MailboxPC_InitBuffers(u32 num);
u32 MailboxPC_GetAddWindow(u32 winIdx);
void MailboxPC_RemoveWindow(u32 winIdx);
u32 MailboxPC_InitListMenu(struct PlayerPCItemPageStruct * playerPcStruct);
void MailboxPC_AddScrollIndicatorArrows(struct PlayerPCItemPageStruct * playerPcStruct);
void MailboxPC_DestroyListMenuBuffer(void);

#endif //GUARD_MAILBOX_PC_H

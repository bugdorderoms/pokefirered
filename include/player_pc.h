#ifndef GUARD_PLAYER_PC_H
#define GUARD_PLAYER_PC_H

#include "global.h"
#include "list_menu.h"

struct PlayerPCItemPageStruct
{
    u16 itemsAbove;
    u16 cursorPos;
    u8 pageItems;
    u8 count;
    bool8 notInRoom;
    u8 scrollIndicatorId;
};

extern struct PlayerPCItemPageStruct gPlayerPcMenuManager;

void NewGameInitPCItems(void);
void Mailbox_ReturnToMailListAfterDeposit(void);

#endif //GUARD_PLAYER_PC_H

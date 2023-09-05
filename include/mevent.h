#ifndef GUARD_MEVENT_H
#define GUARD_MEVENT_H

#include "global.h"

void MEvent_RecordIdOfWonderCardSenderByEventType(u32 eventId, u32 trainerId);
u16 *GetMEventProfileECWordsMaybe(void);
void ResetReceivedWonderCardFlag(void);
bool32 MEventHandleReceivedWonderCard(u16 flagId);
u16 GetWonderCardFlagId(void);

#endif //GUARD_MEVENT_H

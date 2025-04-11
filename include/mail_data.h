#ifndef GUARD_MAIL_DATA_H
#define GUARD_MAIL_DATA_H

#include "global.h"

void ClearMailData(void);
void ClearMailStruct(struct Mail *mail);
bool32 MonHasMail(struct Pokemon *mon);
u32 GiveMailToMon(struct Pokemon *mon, u32 itemId);
u32 GiveMailToMon2(struct Pokemon *mon, struct Mail *mail);
void TakeMailFromMon(struct Pokemon *mon);
u32 TakeMailFromMon2(struct Pokemon *mon);
bool32 ItemIsMail(u32 itemId);

#endif //GUARD_MAIL_DATA_H

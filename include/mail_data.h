#ifndef GUARD_MAIL_DATA_H
#define GUARD_MAIL_DATA_H

#include "global.h"

void ClearMailData(void);
void ClearMailStruct(struct Mail *mail);
bool8 MonHasMail(struct Pokemon *mon);
u8 GiveMailToMon(struct Pokemon *mon, u16 itemId);
u8 GiveMailToMon2(struct Pokemon *mon, struct Mail *mail);
void TakeMailFromMon(struct Pokemon *mon);
u8 TakeMailFromMon2(struct Pokemon *mon);
bool8 ItemIsMail(u16 itemId);

#endif //GUARD_MAIL_DATA_H

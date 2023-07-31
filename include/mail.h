#ifndef GUARD_MAIL_H
#define GUARD_MAIL_H

#include "global.h"

#define MAIL_WORDS_COUNT 9

#define ITEM_TO_MAIL(itemId) (itemId - ITEM_ORANGE_MAIL)

void ReadMail(struct Mail * mail, void (*savedCallback)(void), bool8 messageExists);

#endif // GUARD_MAIL_H

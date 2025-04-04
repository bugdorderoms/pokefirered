#ifndef GUARD_FIELD_MESSAGE_BOX_H
#define GUARD_FIELD_MESSAGE_BOX_H

#include "global.h"

bool32 ShowFieldMessage(const u8 *message);
bool32 ShowFieldAutoScrollMessage(const u8 *message);
void HideFieldMessageBox(void);
bool32 IsFieldMessageBoxHidden(void);
bool32 textbox_any_visible(void);
void InitFieldMessageBox(void);

#endif // GUARD_FIELD_MESSAGE_BOX_H

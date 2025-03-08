#ifndef GUARD_BAG_H
#define GUARD_BAG_H

#include "menu_helpers.h"

void InitBagWindows(void);
void BagPrintTextOnWindow(u32 windowId, u32 fontId, const u8 * str, u32 x, u32 y, u32 letterSpacing, u32 lineSpacing, u32 speed, u32 colorIdx);
void BagPrintTextOnWin1CenteredColor0(const u8 * str);
u32 ShowBagWindow(u32 whichWindow, u32 nItems);
void HideBagWindow(u32 whichWindow);
u32 OpenBagWindow(u32 whichWindow);
void CloseBagWindow(u32 whichWindow);
u32 GetBagWindow(u32 whichWindow);
void BagCreateYesNoMenuRight(u32 taskId, const struct YesNoFuncTable * ptrs, bool32 isTop);

#endif //GUARD_BAG_H

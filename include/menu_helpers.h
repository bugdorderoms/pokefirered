#ifndef GUARD_MENU_HELPERS_H
#define GUARD_MENU_HELPERS_H

#include "global.h"
#include "task.h"
#include "window.h"

struct YesNoFuncTable
{
    TaskFunc yesFunc;
    TaskFunc noFunc;
};

bool16 RunTextPrinters_CheckActive(u8 textPrinterId);
bool8 MenuHelpers_CallLinkSomething(void);
bool8 sub_80BF748(void);
bool8 MenuHelpers_LinkSomething(void);
void SetVBlankHBlankCallbacksToNull(void);
void ResetAllBgsCoordinatesAndBgCntRegs(void);
u8 GetDialogBoxFontId(void);
bool8 AdjustQuantityAccordingToDPadInput(s16 *quantity_p, u16 qmax);
void DisplayMessageAndContinueTask(u8 taskId, u8 windowId, u16 tileNum, u8 paletteNum, u8 fontId, u8 textSpeed, const u8 *string, void *taskFunc);
void CreateYesNoMenuWithCallbacks(u8 taskId, const struct WindowTemplate *template, u8 fontId, u8 left, u8 top, u16 tileStart, u8 palette, const struct YesNoFuncTable *yesNo);
bool8 CanWriteMailHere(u16 itemId);
void ResetVramOamAndBgCntRegs(void);

#endif //GUARD_MENU_HELPERS_H

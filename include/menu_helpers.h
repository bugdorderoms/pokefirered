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

bool32 RunTextPrinters_CheckActive(u32 textPrinterId);
bool32 MenuHelpers_CallLinkSomething(void);
bool32 sub_80BF748(void);
bool32 MenuHelpers_LinkSomething(void);
void SetVBlankHBlankCallbacksToNull(void);
void ResetAllBgsCoordinatesAndBgCntRegs(void);
u32 GetDialogBoxFontId(void);
bool32 AdjustQuantityAccordingToDPadInput(s16 *quantity_p, u32 qmax);
void DisplayMessageAndContinueTask(u32 taskId, u32 windowId, u32 tileNum, u32 paletteNum, u32 fontId, u32 textSpeed, const u8 *string, void *taskFunc);
void CreateYesNoMenuWithCallbacks(u32 taskId, const struct WindowTemplate *template, u32 fontId, u32 left, u32 top, u32 tileStart, u32 palette, const struct YesNoFuncTable *yesNo);
bool32 CanWriteMailHere(u32 itemId);
void ResetVramOamAndBgCntRegs(void);

#endif //GUARD_MENU_HELPERS_H

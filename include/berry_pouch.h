#ifndef GUARD_BERRY_POUCH_H
#define GUARD_BERRY_POUCH_H

#include "task.h"

enum BerryPouchType
{
    BERRYPOUCH_FROMFIELD,
    BERRYPOUCH_FROMPARTYGIVE,
    BERRYPOUCH_FROMMARTSELL,
    BERRYPOUCH_FROMPOKEMONSTORAGEPC,
    BERRYPOUCH_FROMBATTLE,
    BERRYPOUCH_NA,
    BERRYPOUCH_CHOOSE_ITEM,
};

void BerryPouch_StartFadeToExitCallback(u32 taskId);
void BerryPouch_SetExitCallback(void (*exitCallback)(void));
void InitBerryPouch(u32 type, void (*savedCallback)(void), u32 allowSelect);
void DisplayItemMessageInBerryPouch(u32 taskId, u32 fontId, const u8 * str, TaskFunc followUpFunc);
void Task_BerryPouch_DestroyDialogueWindowAndRefreshListMenu(u32 taskId);
void BerryPouch_CursorResetToTop(void);

#endif //GUARD_BERRY_POUCH_H

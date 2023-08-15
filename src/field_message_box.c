#include "global.h"
#include "gflib.h"
#include "new_menu_helpers.h"
#include "quest_log.h"
#include "script.h"
#include "text_window.h"

static EWRAM_DATA u8 sMessageBoxType = 0;

void InitFieldMessageBox(void)
{
    sMessageBoxType = 0;
    gTextFlags.canABSpeedUpPrint = FALSE;
    gTextFlags.useAlternateDownArrow = FALSE;
    gTextFlags.autoScroll = FALSE;
}

static void Task_RunFieldMessageBoxPrinter(u8 taskId)
{
    struct Task * task = &gTasks[taskId];
    switch (task->data[0])
    {
    case 0:
        if (gQuestLogState == QL_STATE_PLAYBACK)
        {
            gTextFlags.autoScroll = TRUE;
            TextWindow_LoadTilesStdFrame1(0, 0x200);
        }
        else if (!IsMsgSignPost())
        {
            LoadStdWindowFrameGfx();
        }
        else
        {
            LoadSignPostWindowFrameGfx();
        }
        task->data[0]++;
        break;
    case 1:
        DrawDialogueFrame(0, TRUE);
        task->data[0]++;
        break;
    case 2:
        if (RunTextPrinters_CheckPrinter0Active() != TRUE)
        {
            sMessageBoxType = 0;
            DestroyTask(taskId);
        }
        break;
    }
}

static void textbox_fdecode_auto_and_task_add(const u8 *str)
{
    StringExpandPlaceholders(gStringVar4, str);
    AddTextPrinterDiffStyle(TRUE);
    CreateTask(Task_RunFieldMessageBoxPrinter, 80);
}

bool8 ShowFieldMessage(const u8 *str)
{
    if (sMessageBoxType != 0)
        return FALSE;
    textbox_fdecode_auto_and_task_add(str);
    sMessageBoxType = 2;
    return TRUE;
}

bool8 ShowFieldAutoScrollMessage(const u8 *str)
{
    if (sMessageBoxType != 0)
        return FALSE;
    sMessageBoxType = 3;
    textbox_fdecode_auto_and_task_add(str);
    return TRUE;
}

void HideFieldMessageBox(void)
{
	u8 taskId = FindTaskIdByFunc(Task_RunFieldMessageBoxPrinter);
    if (taskId != 0xFF)
        DestroyTask(taskId);
    ClearDialogWindowAndFrame(0, TRUE);
    sMessageBoxType = 0;
}

u8 textbox_any_visible(void)
{
    return sMessageBoxType;
}

bool8 IsFieldMessageBoxHidden(void)
{
    if (sMessageBoxType == 0)
        return TRUE;
    else
        return FALSE;
}

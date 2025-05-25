#include "global.h"
#include "gflib.h"
#include "task.h"
#include "menu.h"
#include "menu_helpers.h"
#include "new_menu_helpers.h"
#include "link.h"
#include "overworld.h"
#include "mail_data.h"
#include "field_specials.h"
#include "constants/songs.h"
#include "constants/items.h"
#include "constants/maps.h"

static EWRAM_DATA const struct YesNoFuncTable *gUnknown_20399C8 = NULL;
static EWRAM_DATA TaskFunc gUnknown_20399CC = NULL;
static EWRAM_DATA u8 gUnknown_20399D0 = {0};

static void Task_ContinueTaskAfterMessagePrints(u32 taskId);

void DisplayMessageAndContinueTask(u32 taskId, u32 windowId, u32 tileNum, u32 paletteNum, u32 fontId, u32 textSpeed, const u8 *string, void *taskFunc)
{
    gUnknown_20399D0 = windowId;
    DrawDialogFrameWithCustomTileAndPalette(windowId, TRUE, tileNum, paletteNum);

    if (string != gStringVar4)
        StringExpandPlaceholders(gStringVar4, string);

    gTextFlags.canABSpeedUpPrint = 1;
    AddTextPrinterParameterized2(windowId, fontId, gStringVar4, textSpeed, NULL, 2, 1, 3);
    gUnknown_20399CC = taskFunc;
    gTasks[taskId].func = Task_ContinueTaskAfterMessagePrints;
}

bool32 RunTextPrinters_CheckActive(u32 textPrinterId)
{
    RunTextPrinters();
    return IsTextPrinterActive(textPrinterId);
}

static void Task_ContinueTaskAfterMessagePrints(u32 taskId)
{
    if (!RunTextPrinters_CheckActive(gUnknown_20399D0))
        gUnknown_20399CC(taskId);
}

static void Task_CallYesOrNoCallback(u32 taskId)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0:
        PlaySE(SE_SELECT);
        gTasks[taskId].func = gUnknown_20399C8->yesFunc;
        break;
    case 1:
    case MENU_B_PRESSED:
        PlaySE(SE_SELECT);
        gTasks[taskId].func = gUnknown_20399C8->noFunc;
        break;
    }
}

void CreateYesNoMenuWithCallbacks(u32 taskId, const struct WindowTemplate *template, u32 fontId, u32 left, u32 top, u32 tileStart, u32 palette, const struct YesNoFuncTable *yesNo)
{
    CreateYesNoMenu(template, fontId, left, top, tileStart, palette, 0);
    gUnknown_20399C8 = yesNo;
    gTasks[taskId].func = Task_CallYesOrNoCallback;
}

bool32 CanWriteMailHere(u32 itemId)
{
    if (!IsUpdateLinkStateCBActive() && !InUnionRoom())
        return TRUE;
    else if (!ItemIsMail(itemId))
        return TRUE;
    else
        return FALSE;
}

bool32 MenuHelpers_LinkSomething(void)
{
    if (IsUpdateLinkStateCBActive() || gReceivedRemoteLinkPlayers == 1)
        return TRUE;
    else
        return FALSE;
}

bool32 MenuHelpers_CallLinkSomething(void)
{
    if (!MenuHelpers_LinkSomething())
        return FALSE;
    else
        return Overworld_LinkRecvQueueLengthMoreThan2();
}

bool32 sub_80BF748(void)
{
    if (MenuHelpers_CallLinkSomething())
        return TRUE;
    else if (!LinkRecvQueueLengthMoreThan2())
        return FALSE;
    else
        return TRUE;
}

void SetVBlankHBlankCallbacksToNull(void)
{
    SetVBlankCallback(NULL);
    SetHBlankCallback(NULL);
}

void ResetVramOamAndBgCntRegs(void)
{
    ResetAllBgsCoordinatesAndBgCntRegs();
    CpuFill16(0, (void*) VRAM, VRAM_SIZE);
    CpuFill32(0, (void*) OAM, OAM_SIZE);
    CpuFill16(0, (void*) PLTT, PLTT_SIZE);
}

void ResetAllBgsCoordinatesAndBgCntRegs(void)
{
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_BG3CNT, 0);
    SetGpuReg(REG_OFFSET_BG2CNT, 0);
    SetGpuReg(REG_OFFSET_BG1CNT, 0);
    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    ResetAllBgsPos();
}

bool32 AdjustQuantityAccordingToDPadInput(s16 *quantity_p, u32 qmax)
{
    s16 valBefore = (*quantity_p);

    if (JOY_REPT(DPAD_ANY) == DPAD_UP)
    {
        (*quantity_p)++;
        if ((*quantity_p) > qmax)
            (*quantity_p) = 1;

        if ((*quantity_p) == valBefore)
            return FALSE;
        else
        {
            PlaySE(SE_SELECT);
            return TRUE;
        }
    }
    else if (JOY_REPT(DPAD_ANY) == DPAD_DOWN)
    {
        (*quantity_p)--;
        if ((*quantity_p) <= 0)
            (*quantity_p) = qmax;
        
        if ((*quantity_p) == valBefore)
            return FALSE;
        else
        {
            PlaySE(SE_SELECT);
            return TRUE;
        }
    }
    else if (JOY_REPT(DPAD_ANY) == DPAD_RIGHT)
    {
        (*quantity_p) += 10;
        if ((*quantity_p) > qmax)
            (*quantity_p) = qmax;
        
        if ((*quantity_p) == valBefore)
            return FALSE;
        else
        {
            PlaySE(SE_SELECT);
            return TRUE;
        }
    }
    else if (JOY_REPT(DPAD_ANY) == DPAD_LEFT)
    {
        (*quantity_p) -= 10;
        if ((*quantity_p) <= 0)
            (*quantity_p) = 1;
        
        if ((*quantity_p) == valBefore)
            return FALSE;
        else
        {
            PlaySE(SE_SELECT);
            return TRUE;
        }
    }
    return FALSE;
}

u32 GetDialogBoxFontId(void)
{
    if (!ContextNpcGetTextColor())
        return 4;
    else
        return 5;
}

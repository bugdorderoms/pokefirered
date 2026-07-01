#include "global.h"
#include "gflib.h"
#include "party_menu.h"
#include "menu.h"
#include "field_fadetransition.h"
#include "new_menu_helpers.h"
#include "constants/songs.h"

static void sub_80E57E8(u32 taskId);
static void sub_80E583C(u32 taskId);
static void sub_80E58A0(u32 taskId);
static void sub_80E5934(u32 taskId);

bool32 SetUpFieldMove_SoftBoiled(void)
{
    u32 partyId = GetCursorSelectionMonId();
    return (GetMonData(&gPlayerParty[partyId], MON_DATA_HP) > GetMonData(&gPlayerParty[partyId], MON_DATA_MAX_HP) / 5);
}

void ChooseMonForSoftboiled(u32 taskId)
{
    gPartyMenu.action = 10;
    gPartyMenu.slotId2 = gPartyMenu.slotId;
    AnimatePartySlot(GetCursorSelectionMonId(), 1);
    DisplayPartyMenuStdMessage(5);
    gTasks[taskId].func = Task_HandleChooseMonInput;
}

void Task_TryUseSoftboiledOnPartyMon(u32 taskId)
{
    u32 r8 = gPartyMenu.slotId;
    u32 r5 = gPartyMenu.slotId2;
    u32 curHp;

    if (r5 > 6)
    {
        gPartyMenu.action = 0;
        DisplayPartyMenuStdMessage(0);
        gTasks[taskId].func = Task_HandleChooseMonInput;
    }
    else
    {
        curHp = GetMonData(&gPlayerParty[r5], MON_DATA_HP);
        
        if (curHp == 0 || r8 == r5 || GetMonData(&gPlayerParty[r5], MON_DATA_MAX_HP) == curHp)
            sub_80E5934(taskId);
        else
        {
            PlaySE(SE_USE_ITEM);
            PartyMenuModifyHP(taskId, r8, -1, GetMonData(&gPlayerParty[r8], MON_DATA_MAX_HP) / 5, sub_80E57E8);
        }
    }
}

static void sub_80E57E8(u32 taskId)
{
    PlaySE(SE_USE_ITEM);
    PartyMenuModifyHP(taskId, gPartyMenu.slotId2, 1, GetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_MAX_HP) / 5, sub_80E583C);
}

static void sub_80E583C(u32 taskId)
{
    GetMonNickname(&gPlayerParty[gPartyMenu.slotId2], gStringVar1);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("{STR_VAR_1}'s HP was restored\nby {STR_VAR_2} point(s).{PAUSE_UNTIL_PRESS}"));
    DisplayPartyMenuMessage(gStringVar4, 0);
    ScheduleBgCopyTilemapToVram(2);
    gTasks[taskId].func = sub_80E58A0;
}

static void sub_80E58A0(u32 taskId)
{
    if (IsPartyMenuTextPrinterActive() != TRUE)
    {
        gPartyMenu.action = 0;
        AnimatePartySlot(gPartyMenu.slotId, 0);
        gPartyMenu.slotId = gPartyMenu.slotId2;
        AnimatePartySlot(gPartyMenu.slotId2, 1);
        ClearStdWindowAndFrameToTransparent(6, 0);
        ClearWindowTilemap(6);
        DisplayPartyMenuStdMessage(0);
        gTasks[taskId].func = Task_HandleChooseMonInput;
    }
}

static void sub_80E5900(u32 taskId)
{
    if (IsPartyMenuTextPrinterActive() != TRUE)
    {
        DisplayPartyMenuStdMessage(5);
        gTasks[taskId].func = Task_HandleChooseMonInput;
    }
}

static void sub_80E5934(u32 taskId)
{
    PlaySE(SE_SELECT);
    DisplayPartyMenuMessage(COMPOUND_STRING("This can't be used on\nthat Pokémon.{PAUSE_UNTIL_PRESS}"), 0);
    ScheduleBgCopyTilemapToVram(2);
    gTasks[taskId].func = sub_80E5900;
}

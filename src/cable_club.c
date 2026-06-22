#include "global.h"
#include "gflib.h"
#include "battle.h"
#include "battle_records.h"
#include "cable_club.h"
#include "event_data.h"
#include "event_scripts.h"
#include "field_message_box.h"
#include "field_specials.h"
#include "field_weather.h"
#include "link.h"
#include "load_save.h"
#include "m4a.h"
#include "new_menu_helpers.h"
#include "overworld.h"
#include "script.h"
#include "script_pokemon_util.h"
#include "start_menu.h"
#include "strings.h"
#include "task.h"
#include "trade.h"
#include "trainer_card.h"
#include "union_room.h"
#include "constants/songs.h"
#include "constants/cable_club.h"
#include "constants/field_weather.h"

static void Task_Linkup0(u32 taskId);
static void Task_Linkup1(u32 taskId);
static void Task_LinkupMaster_2(u32 taskId);
static void Task_LinkupMaster_3(u32 taskId);
static void Task_LinkupMaster_4(u32 taskId);
static void Task_LinkupMaster_5(u32 taskId);
static void Task_LinkupSlave_2(u32 taskId);
static void Task_LinkupMaster_6(u32 taskId);
static void Task_Linkup_6a(u32 taskId);
static void Task_Linkup_7(u32 taskId);
static void Task_Linkup_Canceled(u32 taskId);
static void Task_Linkup_ErroredOut(u32 taskId);
static bool32 Task_Linkup_TimedOut(u32 taskId);
static void Task_ReestablishLinkInCableClubRoom_0(u32 taskId);
static void Task_ReestablishLinkInCableClubRoom_1(u32 taskId);
static void Task_ReestablishLinkInCableClubRoom_Master(u32 taskId);
static void Task_ReestablishLinkInCableClubRoom_2(u32 taskId);

static const struct WindowTemplate gUnknown_83C6AB0 = {
    .bg = 0,
    .tilemapLeft = 16,
    .tilemapTop = 11,
    .width = 11,
    .height = 2,
    .paletteNum = 15,
    .baseBlock = 0x125
};

static const u8 *const sStarsMessagePtrs[] = {
    gUnknown_841DF8B,
    gUnknown_841DF92,
    gUnknown_841DF99,
    gUnknown_841DFA0
};

static void CreateLinkupTask(u32 lower, u32 higher)
{
    u32 taskId;
    
    if (FindTaskIdByFunc(Task_Linkup0) == 0xFF)
    {
        taskId = CreateTask(Task_Linkup0, 80);
        gTasks[taskId].data[1] = lower;
        gTasks[taskId].data[2] = higher;
    }
}

static void PrintNewCountOnLinkPlayerCountDisplayWindow(u32 windowId, s32 num)
{
    ConvertIntToDecimalStringN(gStringVar1, num, STR_CONV_MODE_LEFT_ALIGN, 1);
    SetStdWindowBorderStyle(windowId, FALSE);
    StringExpandPlaceholders(gStringVar4, gUnknown_841DF82);
    AddTextPrinterParameterized(windowId, 2, gStringVar4, 0, 0, TEXT_SPEED_FF, NULL);
    CopyWindowToVram(windowId, COPYWIN_BOTH);
}

static void DestroyLinkPlayerCountDisplayWindow(u32 windowId)
{
    ClearStdWindowAndFrame(windowId, FALSE);
    CopyWindowToVram(windowId, COPYWIN_BOTH);
}

static void UpdateLinkPlayerCountDisplay(u32 taskId, u32 num)
{
    s16 *data = gTasks[taskId].data;
    
    if (num != data[3])
    {
        if (num < 2)
            DestroyLinkPlayerCountDisplayWindow(data[5]);
        else
            PrintNewCountOnLinkPlayerCountDisplayWindow(data[5], num);
        
        data[3] = num;
    }
}

u32 sub_8080844(u32 lower, u32 higher)
{
    switch (GetLinkPlayerDataExchangeStatusTimed(lower, higher))
    {
    case EXCHANGE_COMPLETE:
        return 1;
    case EXCHANGE_IN_PROGRESS:
        return 3;
    case EXCHANGE_STAT_4:
        return 7;
    case EXCHANGE_STAT_5:
        return 9;
    case EXCHANGE_STAT_6:
        ConvertIntToDecimalStringN(gStringVar1, GetLinkPlayerCount_2(), STR_CONV_MODE_LEFT_ALIGN, 1);
        return 4;
    default:
        return 0;
    }
}

static bool32 sub_80808BC(u32 taskId)
{
    if (HasLinkErrorOccurred())
    {
        gTasks[taskId].func = Task_Linkup_ErroredOut;
        return TRUE;
    }
    return FALSE;
}

static bool32 sub_80808F0(u32 taskId)
{
    if (JOY_NEW(B_BUTTON) && !IsLinkConnectionEstablished())
    {
        gLinkType = 0;
        gTasks[taskId].func = Task_Linkup_Canceled;
        return TRUE;
    }
    return FALSE;
}

static bool32 sub_808093C(u32 taskId)
{
    if (IsLinkConnectionEstablished())
        SetSuppressLinkErrorMessage(TRUE);

    if (JOY_NEW(B_BUTTON))
    {
        gLinkType = 0;
        gTasks[taskId].func = Task_Linkup_Canceled;
        return TRUE;
    }
    return FALSE;
}

static bool32 sub_8080990(u32 taskId)
{
    if (GetSioMultiSI())
    {
        gTasks[taskId].func = Task_Linkup_ErroredOut;
        return TRUE;
    }
    return FALSE;
}

static void Task_Linkup0(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    
    if (data[0] == 0)
    {
        OpenLinkTimed();
        sub_800AA24();
        ResetLinkPlayers();
        data[5] = AddWindow(&gUnknown_83C6AB0);
    }
    else if (data[0] > 9)
        gTasks[taskId].func = Task_Linkup1;

    data[0]++;
}

static void Task_Linkup1(u32 taskId)
{
    u32 linkPlayerCount = GetLinkPlayerCount_2();
    
    if (sub_80808F0(taskId) != TRUE && sub_808093C(taskId) != TRUE && linkPlayerCount >= 2)
    {
        SetSuppressLinkErrorMessage(TRUE);
        gTasks[taskId].data[3] = 0;
        
        if (IsLinkMaster())
        {
            PlaySE(SE_PIN);
            ShowFieldAutoScrollMessage(CableClub_Text_WhenAllPlayersReadyAConfirmBCancel);
            gTasks[taskId].func = Task_LinkupMaster_2;
        }
        else
        {
            PlaySE(SE_BOO);
            ShowFieldAutoScrollMessage(CableClub_Text_AwaitingLinkupBCancel);
            gTasks[taskId].func = Task_LinkupSlave_2;
        }
    }
}

static void Task_LinkupMaster_2(u32 taskId)
{
    if (sub_80808F0(taskId) != TRUE && sub_8080990(taskId) != TRUE && sub_80808BC(taskId) != TRUE && !textbox_any_visible())
    {
        gTasks[taskId].data[3] = 0;
        gTasks[taskId].func = Task_LinkupMaster_3;
    }
}

static void Task_LinkupMaster_3(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    s32 linkPlayerCount = GetLinkPlayerCount_2();
    
    if (sub_80808F0(taskId) != TRUE && sub_8080990(taskId) != TRUE && sub_80808BC(taskId) != TRUE)
    {
        UpdateLinkPlayerCountDisplay(taskId, linkPlayerCount);
        
        if (JOY_NEW(A_BUTTON) && linkPlayerCount >= data[1])
        {
            sub_800A900(linkPlayerCount);
            DestroyLinkPlayerCountDisplayWindow(data[5]);
            ConvertIntToDecimalStringN(gStringVar1, linkPlayerCount, STR_CONV_MODE_LEFT_ALIGN, 1);
            ShowFieldAutoScrollMessage(CableClub_Text_StartLinkWithXPlayersAConfirmBCancel);
            gTasks[taskId].func = Task_LinkupMaster_4;
        }
    }
}

static void Task_LinkupMaster_4(u32 taskId)
{
    if (sub_80808F0(taskId) != TRUE && sub_8080990(taskId) != TRUE && sub_80808BC(taskId) != TRUE && !textbox_any_visible())
    {
        if (GetSavedPlayerCount() != GetLinkPlayerCount_2())
        {
            ShowFieldAutoScrollMessage(CableClub_Text_WhenAllPlayersReadyAConfirmBCancel);
            gTasks[taskId].func = Task_LinkupMaster_2;
        }
        else if (JOY_HELD(B_BUTTON))
        {
            ShowFieldAutoScrollMessage(CableClub_Text_WhenAllPlayersReadyAConfirmBCancel);
            gTasks[taskId].func = Task_LinkupMaster_2;
        }
        else if (JOY_HELD(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            CheckShouldAdvanceLinkState();
            gTasks[taskId].func = Task_LinkupMaster_5;
        }
    }
}

static void Task_LinkupMaster_5(u32 taskId)
{
    u32 lower = gTasks[taskId].data[1];
    u32 higher = gTasks[taskId].data[2];

    if (sub_80808BC(taskId) != TRUE && Task_Linkup_TimedOut(taskId) != TRUE)
    {
        if (GetLinkPlayerCount_2() != GetSavedPlayerCount())
            gTasks[taskId].func = Task_Linkup_ErroredOut;
        else
        {
            gSpecialVar_Result = sub_8080844(lower, higher);
            if (gSpecialVar_Result)
                gTasks[taskId].func = Task_LinkupMaster_6;
        }
    }
}

static void Task_LinkupSlave_2(u32 taskId)
{
    u32 res;
    u32 lower = gTasks[taskId].data[1];
    u32 higher = gTasks[taskId].data[2];
    
    if (sub_80808F0(taskId) != TRUE && sub_80808BC(taskId) != TRUE)
    {
        gSpecialVar_Result = res = sub_8080844(lower, higher);
        
        if (res)
        {
            if (res == 3 || res == 4)
            {
                SetCloseLinkCallback();
                HideFieldMessageBox();
                gTasks[taskId].func = Task_Linkup_7;
            }
            else if (res == 7 || res == 9)
            {
                CloseLink();
                HideFieldMessageBox();
                gTasks[taskId].func = Task_Linkup_7;
            }
            else
            {
                gFieldLinkPlayerCount = GetLinkPlayerCount_2();
                gLocalLinkPlayerId = GetMultiplayerId();
                sub_800A900(gFieldLinkPlayerCount);
                TrainerCard_GenerateCardForLinkPlayer((void*)gBlockSendBuffer);
                gTasks[taskId].func = Task_Linkup_6a;
            }
        }
    }
}

bool32 AnyConnectedPartnersPlayingRS(void)
{
    u32 i, version;

    for (i = 0; i < GetLinkPlayerCount(); i++)
    {
        version = gLinkPlayers[i].version & 0xFF;
        if (version == VERSION_RUBY || version == VERSION_SAPPHIRE)
            return TRUE;
    }
    return FALSE;
}

static void Task_LinkupMaster_6(u32 taskId)
{
    if (sub_80808BC(taskId) != TRUE)
    {
        if (gSpecialVar_Result == 4)
        {
            if (AnyConnectedPartnersPlayingRS())
                CloseLink();
            else
                SetCloseLinkCallback();
            
            HideFieldMessageBox();
            gTasks[taskId].func = Task_Linkup_7;
        }
        else if (gSpecialVar_Result == 3)
        {
            SetCloseLinkCallback();
            HideFieldMessageBox();
            gTasks[taskId].func = Task_Linkup_7;
        }
        else if (gSpecialVar_Result == 7 || gSpecialVar_Result == 9)
        {
            CloseLink();
            HideFieldMessageBox();
            gTasks[taskId].func = Task_Linkup_7;
        }
        else
        {
            gFieldLinkPlayerCount = GetLinkPlayerCount_2();
            gLocalLinkPlayerId = GetMultiplayerId();
            sub_800A900(gFieldLinkPlayerCount);
            TrainerCard_GenerateCardForLinkPlayer((void*)gBlockSendBuffer);
            gTasks[taskId].func = Task_Linkup_6a;
            Link_PrepareCmd0xCCCC_Rfu0xA100(2);
        }
    }
}

static void Task_Linkup_6a(u32 taskId)
{
    u32 i, version;
    
    if (sub_80808BC(taskId) != TRUE && GetBlockReceivedStatus() == sub_800A8A4())
    {
        for(i = 0; i < GetLinkPlayerCount(); i++)
        {
            version = gLinkPlayers[i].version & 0xFF;
            
            if (version != VERSION_FIRE_RED && version != VERSION_LEAF_GREEN)
            {
                const struct TrainerCardRSE * src = (const struct TrainerCardRSE *)gBlockRecvBuffer[i];
                gTrainerCards[i].rse = *src;
                gTrainerCards[i].version = gLinkPlayers[i].version;
            }
            else
            {
                const struct TrainerCard * src = (const struct TrainerCard *)gBlockRecvBuffer[i];
                gTrainerCards[i] = *src;
            }
        }
        SetSuppressLinkErrorMessage(FALSE);
        ResetBlockReceivedFlags();
        HideFieldMessageBox();
        
        if (gSpecialVar_Result == 1)
        {
            DestroyLinkPlayerCountDisplayWindow(gTasks[taskId].data[5]);
            EnableBothScriptContexts();
            DestroyTask(taskId);
        }
        else
        {
            SetCloseLinkCallback();
            gTasks[taskId].func = Task_Linkup_7;
        }
    }
}

static void Task_Linkup_7(u32 taskId)
{
    if (!gReceivedRemoteLinkPlayers)
    {
        DestroyLinkPlayerCountDisplayWindow(gTasks[taskId].data[5]);
        EnableBothScriptContexts();
        RemoveWindow(gTasks[taskId].data[5]);
        DestroyTask(taskId);
    }
}

static void Task_Linkup_Canceled(u32 taskId)
{
    gSpecialVar_Result = 5;
    DestroyLinkPlayerCountDisplayWindow(gTasks[taskId].data[5]);
    HideFieldMessageBox();
    EnableBothScriptContexts();
    DestroyTask(taskId);
}

static void Task_Linkup_ErroredOut(u32 taskId)
{
    gSpecialVar_Result = 6;
    DestroyLinkPlayerCountDisplayWindow(gTasks[taskId].data[5]);
    HideFieldMessageBox();
    EnableBothScriptContexts();
    DestroyTask(taskId);
}

static bool32 Task_Linkup_TimedOut(u32 taskId)
{
    if (++gTasks[taskId].data[4] > 600)
    {
        gTasks[taskId].func = Task_Linkup_ErroredOut;
        return TRUE;
    }
    return FALSE;
}

void TryBattleLinkup(void)
{
    u32 lower, higher;
    
    higher = lower = 2;
    
    switch (gSpecialVar_0x8004)
    {
    case USING_SINGLE_BATTLE:
        gLinkType = LINKTYPE_SINGLE_BATTLE;
        break;
    case USING_DOUBLE_BATTLE:
        gLinkType = LINKTYPE_DOUBLE_BATTLE;
        break;
    case USING_MULTI_BATTLE:
        higher = lower = 4;
        gLinkType = LINKTYPE_MULTI_BATTLE;
        break;
    }
    CreateLinkupTask(lower, higher);
}

void TryTradeLinkup(void)
{
    gLinkType = LINKTYPE_TRADE_SETUP;
    gBattleTypeFlags = 0;
    CreateLinkupTask(2, 2);
}

u32 CreateTask_ReestablishLinkInCableClubRoom(void)
{
    if (FuncIsActiveTask(Task_ReestablishLinkInCableClubRoom_0))
        return 0xFF;
    
    switch (gSpecialVar_0x8004)
    {
    case USING_SINGLE_BATTLE:
        gLinkType = LINKTYPE_SINGLE_BATTLE;
        break;
    case USING_DOUBLE_BATTLE:
        gLinkType = LINKTYPE_DOUBLE_BATTLE;
        break;
    case USING_MULTI_BATTLE:
        gLinkType = LINKTYPE_MULTI_BATTLE;
        break;
    case USING_TRADE_CENTER:
        gLinkType = LINKTYPE_TRADE;
        break;
    case USING_RECORD_CORNER:
        gLinkType = LINKTYPE_0x3322;
        break;
    }
    return CreateTask(Task_ReestablishLinkInCableClubRoom_0, 80);
}

static void Task_ReestablishLinkInCableClubRoom_0(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    
    if (data[0] == 0)
    {
        OpenLink();
        ResetLinkPlayers();
        CreateTask(Task_WaitForReceivedRemoteLinkPlayers5SecondTimeout, 80);
    }
    else if (data[0] > 9)
        gTasks[taskId].func = Task_ReestablishLinkInCableClubRoom_1;
    
    data[0]++;
}

static void Task_ReestablishLinkInCableClubRoom_1(u32 taskId)
{
    if (GetLinkPlayerCount_2() >= 2)
        gTasks[taskId].func = IsLinkMaster() ? Task_ReestablishLinkInCableClubRoom_Master : Task_ReestablishLinkInCableClubRoom_2;
}

static void Task_ReestablishLinkInCableClubRoom_Master(u32 taskId)
{
    if (GetSavedPlayerCount() == GetLinkPlayerCount_2())
    {
        CheckShouldAdvanceLinkState();
        gTasks[taskId].func = Task_ReestablishLinkInCableClubRoom_2;
    }
}

static void Task_ReestablishLinkInCableClubRoom_2(u32 taskId)
{
    if (gReceivedRemoteLinkPlayers && IsLinkPlayerDataExchangeComplete())
    {
        sub_800A9A4();
        StartSendingKeysToLink();
        DestroyTask(taskId);
    }
}

static void Task_StartWiredCableClubBattle(u32 taskId)
{
    struct Task * task = &gTasks[taskId];
    
    switch (task->data[0])
    {
    case 0:
        FadeScreen(FADE_TO_BLACK, 0);
        gLinkType = LINKTYPE_BATTLE;
        ClearLinkCallback_2();
        task->data[0]++;
        break;
    case 1:
        if (!gPaletteFade.active)
            task->data[0]++;
        break;
    case 2:
        if (++task->data[1] > 20)
            task->data[0]++;
        break;
    case 3:
        SetCloseLinkCallback();
        task->data[0]++;
        break;
    case 4:
        if (!gReceivedRemoteLinkPlayers)
            task->data[0]++;
        break;
    case 5:
        if (gLinkPlayers[0].trainerId & 1)
            PlayMapChosenOrBattleBGM(MUS_RS_VS_GYM_LEADER);
        else
            PlayMapChosenOrBattleBGM(MUS_RS_VS_TRAINER);
        
        switch (gSpecialVar_0x8004)
        {
        case USING_SINGLE_BATTLE:
            gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_LINK;
            break;
        case USING_DOUBLE_BATTLE:
            gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_LINK | BATTLE_TYPE_DOUBLE;
            break;
        case USING_MULTI_BATTLE:
            ReducePlayerPartyToThree();
            gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_LINK | BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI;
            break;
        }
        CleanupOverworldWindowsAndTilemaps();
        gTrainerBattleOpponent_A = TRAINER_LINK_OPPONENT;
        SetMainCallback2(CB2_InitBattle);
        gMain.savedCallback = CB2_ReturnFromCableClubBattle;
        DestroyTask(taskId);
        break;
    }
}

static void Task_StartWirelessCableClubBattle(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    u32 i;
    
    switch (data[0])
    {
    case 0:
        FadeScreen(FADE_TO_BLACK, 0);
        gLinkType = LINKTYPE_BATTLE;
        ClearLinkCallback_2();
        data[0] = 1;
        break;
    case 1:
        if (!gPaletteFade.active)
            data[0] = 2;
        break;
    case 2:
        SendBlock(0, &gLocalLinkPlayer, sizeof(struct LinkPlayer));
        data[0] = 3;
        break;
    case 3:
        if (GetBlockReceivedStatus() == sub_800A8D4())
        {
            for (i = 0; i < GetLinkPlayerCount(); i++)
            {
                gLinkPlayers[i] = *(struct LinkPlayer *)gBlockRecvBuffer[i];
                ConvertLinkPlayerName(&gLinkPlayers[i]);
                ResetBlockReceivedFlag(i);
            }
            data[0] = 4;
        }
        break;
    case 4:
        if (++data[1] > 20)
            data[0] = 5;
        break;
    case 5:
        SetLinkStandbyCallback();
        data[0] = 6;
        break;
    case 6:
        if (IsLinkTaskFinished())
            data[0] = 7;
        break;
    case 7:
        if (gLinkPlayers[0].trainerId & 1)
            PlayMapChosenOrBattleBGM(MUS_RS_VS_GYM_LEADER);
        else
            PlayMapChosenOrBattleBGM(MUS_RS_VS_TRAINER);
        
        gLinkPlayers[0].linkType = LINKTYPE_BATTLE;
        
        switch (gSpecialVar_0x8004)
        {
        case USING_SINGLE_BATTLE:
            gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_LINK;
            break;
        case USING_DOUBLE_BATTLE:
            gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_LINK | BATTLE_TYPE_DOUBLE;
            break;
        case USING_MULTI_BATTLE:
            ReducePlayerPartyToThree();
            gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_LINK | BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI;
            break;
        }
        CleanupOverworldWindowsAndTilemaps();
        gTrainerBattleOpponent_A = TRAINER_LINK_OPPONENT;
        SetMainCallback2(CB2_InitBattle);
        gMain.savedCallback = CB2_ReturnFromCableClubBattle;
        DestroyTask(taskId);
        break;
    }
}

static void sub_8081624(void)
{
    switch (gMain.state)
    {
    case 0:
        SetCloseLinkCallback();
        gMain.state++;
        break;
    case 1:
        if (IsLinkTaskFinished())
            SetMainCallback2(CB2_ReturnToField);
        break;
    }
}

void CB2_ReturnFromCableClubBattle(void)
{
    gBattleTypeFlags &= ~(BATTLE_TYPE_LINK_ESTABLISHED);
    ResetMapMusic();
    LoadPlayerParty();
    SavePlayerBag();
    UpdateTrainerFansAfterLinkBattle();
    
    if (gSpecialVar_0x8004 != USING_MULTI_BATTLE)
        TryRecordLinkBattleOutcome(gLocalLinkPlayerId ^ 1);

    if (InUnionRoom())
        gMain.savedCallback = sub_8081624;
    else
        gMain.savedCallback = CB2_ReturnToFieldFromMultiplayer;

    SetMainCallback2(CB2_SetUpSaveAfterLinkBattle);
}

void CleanupLinkRoomState(void)
{
    if (gSpecialVar_0x8004 == USING_SINGLE_BATTLE || gSpecialVar_0x8004 == USING_DOUBLE_BATTLE || gSpecialVar_0x8004 == USING_MULTI_BATTLE)
    {
        LoadPlayerParty();
        SavePlayerBag();
    }
    SetWarpDestinationToDynamicWarp();
}

void ExitLinkRoom(void)
{
    QueueExitLinkRoomKey();
}

static void Task_EnterCableClubSeat(u32 taskId)
{
    struct Task * task = &gTasks[taskId];
    
    switch (task->data[0])
    {
    case 0:
        ShowFieldMessage(CableClub_Text_PleaseWaitBCancel);
        task->data[0] = 1;
        break;
    case 1:
        if (IsFieldMessageBoxHidden())
        {
            sub_8057F34();
            SetLocalLinkPlayerId(gSpecialVar_0x8005);
            task->data[0] = 2;
        }
        break;
    case 2:
        switch (sub_8057EC0())
        {
        case 0:
            break;
        case 1:
            HideFieldMessageBox();
            task->data[0] = 0;
            sub_8057F70();
            SwitchTaskToFollowupFunc(taskId);
            break;
        case 2:
            task->data[0] = 3;
            break;
        }
        break;
    case 3:
        sub_8057F48();
        EraseFieldMessageBox(TRUE);
        DestroyTask(taskId);
        EnableBothScriptContexts();
        break;
    }
}

static void CreateEnterCableClubSeatTaskWithFollowupFunc(TaskFunc followUpFunc)
{
    SetTaskFuncWithFollowupFunc(CreateTask(Task_EnterCableClubSeat, 80), Task_EnterCableClubSeat, followUpFunc);
    ScriptContext1_Stop();
}

static void Task_StartWiredCableClubTrade(u32 taskId)
{
    struct Task * task = &gTasks[taskId];
    
    switch (task->data[0])
    {
    case 0:
        ScriptContext2_Enable();
        FadeScreen(FADE_TO_BLACK, 0);
        ClearLinkCallback_2();
        task->data[0]++;
        break;
    case 1:
        if (!gPaletteFade.active)
            task->data[0]++;
        break;
    case 2:
        gSelectedTradeMonPositions[0] = 0;
        gSelectedTradeMonPositions[1] = 0;
        m4aMPlayAllStop();
        SetCloseLinkCallback();
        task->data[0]++;
        break;
    case 3:
        if (!gReceivedRemoteLinkPlayers)
        {
            SetMainCallback2(CB2_ReturnFromLinkTrade);
            DestroyTask(taskId);
        }
        break;
    }
}

static void Task_StartWirelessCableClubTrade(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    
    switch (data[0])
    {
    case 0:
        ScriptContext2_Enable();
        FadeScreen(FADE_TO_BLACK, 0);
        ClearLinkRfuCallback();
        data[0]++;
        break;
    case 1:
        if (!gPaletteFade.active)
            data[0]++;
        break;
    case 2:
        gSelectedTradeMonPositions[0] = 0;
        gSelectedTradeMonPositions[1] = 0;
        m4aMPlayAllStop();
        SetLinkStandbyCallback();
        data[0]++;
        break;
    case 3:
        if (IsLinkTaskFinished())
        {
            UnionRoom_CreateTask_CallCB2ReturnFromLinkTrade();
            DestroyTask(taskId);
        }
        break;
    }
}

void EnterTradeSeat(void)
{
    if (gWirelessCommType)
        CreateEnterCableClubSeatTaskWithFollowupFunc(Task_StartWirelessCableClubTrade);
    else
        CreateEnterCableClubSeatTaskWithFollowupFunc(Task_StartWiredCableClubTrade);
}

void EnterColosseumPlayerSpot(void)
{
    gLinkType = LINKTYPE_BATTLE;
    
    if (gWirelessCommType)
        CreateEnterCableClubSeatTaskWithFollowupFunc(Task_StartWirelessCableClubBattle);
    else
        CreateEnterCableClubSeatTaskWithFollowupFunc(Task_StartWiredCableClubBattle);
}

void Script_ShowLinkTrainerCard(void)
{
    ShowTrainerCardInLink(gSpecialVar_0x8006, CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

bool32 GetSeeingLinkPlayerCardMsg(u32 who)
{
    u32 stars;
    
    gSpecialVar_0x8006 = who;
    
    StringCopy(gStringVar1, gLinkPlayers[who].name);
    
    stars = GetTrainerCardStars(who);
    if (stars == 0)
        return FALSE;
    
    StringCopy(gStringVar2, sStarsMessagePtrs[stars - 1]);
    return TRUE;
}

void Task_WaitForReceivedRemoteLinkPlayers5SecondTimeout(u32 taskId)
{
    struct Task * task = &gTasks[taskId];

    if (++task->data[0] > 300)
    {
        CloseLink();
        SetMainCallback2(CB2_LinkError);
        DestroyTask(taskId);
    }
    if (gReceivedRemoteLinkPlayers)
        DestroyTask(taskId);
}

#include "global.h"
#include "gflib.h"
#include "decompress.h"
#include "graphics.h"
#include "task.h"
#include "text_window.h"
#include "menu.h"
#include "menu_helpers.h"
#include "new_menu_helpers.h"
#include "list_menu.h"
#include "tm_case.h"
#include "item.h"
#include "pokemon_icon.h"
#include "item_menu.h"
#include "link.h"
#include "money.h"
#include "shop.h"
#include "teachy_tv.h"
#include "pokemon_storage_system.h"
#include "party_menu.h"
#include "data.h"
#include "scanline_effect.h"
#include "strings.h"
#include "menu_indicators.h"
#include "constants/items.h"
#include "constants/songs.h"

struct UnkStruct_203B10C
{
    void (* savedCallback)(void);
    u8 tmCaseMenuType;
    u8 unk_05;
    u16 selectedRow;
    u16 scrollOffset;
};

struct UnkStruct_203B118
{
    void (* savedCallback)(void);
    u8 maxTMsShown;
    u8 numTMs;
    u16 currItem;
    const u8 * menuActionIndices;
    u8 contextMenuWindowId;
    u8 scrollIndicatorArrowPairId;
    s16 seqId;
    u8 numMenuActions;
    u8 partyIconSpriteIds[PARTY_SIZE];
};

struct UnkStruct_203B11C
{
    struct ItemSlot bagPocket_TMHM[BAG_TMHM_COUNT];
    struct ItemSlot bagPocket_KeyItems[BAG_KEYITEMS_COUNT];
    u16 unk_160;
    u16 unk_162;
};

static EWRAM_DATA struct UnkStruct_203B10C sTMCaseStaticResources = {};
static EWRAM_DATA struct UnkStruct_203B118 * sTMCaseDynamicResources = NULL;
static EWRAM_DATA struct UnkStruct_203B11C * sPokedudePackBackup = NULL;
static EWRAM_DATA void * sTilemapBuffer = NULL; // tilemap buffer
static EWRAM_DATA struct ListMenuItem * sListMenuItemsBuffer = NULL;
static EWRAM_DATA u8 (* sListMenuStringsBuffer)[29] = NULL;

static void CB2_SetUpTMCaseUI_Blocking(void);
static bool32 DoSetUpTMCaseUI(void);
static void ResetBufferPointers_NoFree(void);
static void LoadBGTemplates(void);
static bool32 HandleLoadTMCaseGraphicsAndPalettes(void);
static void CreateTMCaseListMenuBuffers(void);
static void InitTMCaseListMenuItems(void);
static void GetTMNumberAndMoveString(u8 * dest, u32 itemId);
static void TMCase_MoveCursorFunc(s32 itemIndex, bool32 onInit, struct ListMenu *list);
static void TMCase_MoveCursor_UpdatePrintedDescription(s32 itemIndex);
static void PrintListMenuCursorAt_WithColorIdx(u32 a0, u32 a1);
static void CreateTMCaseScrollIndicatorArrowPair_Main(void);
static void TMCaseSetup_GetTMCount(void);
static void TMCaseSetup_InitListMenuPositions(void);
static void TMCaseSetup_UpdateVisualMenuOffset(void);
static void Task_FadeOutAndCloseTMCase(u32 taskId);
static void Task_TMCaseMain(u32 taskId);
static void Task_SelectTMAction_FromFieldBag(u32 taskId);
static void Task_TMContextMenu_HandleInput(u32 taskId);
static void TMHMContextMenuAction_Use(u32 taskId);
static void TMHMContextMenuAction_Give(u32 taskId);
static void PrintError_ThereIsNoPokemon(u32 taskId);
static void Task_WaitButtonAfterErrorPrint(u32 taskId);
static void Subtask_CloseContextMenuAndReturnToMain(u32 taskId);
static void TMHMContextMenuAction_Exit(u32 taskId);
static void Task_SelectTMAction_Type1(u32 taskId);
static void Task_SelectTMAction_Type3(u32 taskId);
static void Task_BeginFadeOutFromTMCase(u32 taskId);
static void Task_SelectTMAction_FromSellMenu(u32 taskId);
static void Task_AskConfirmSaleWithAmount(u32 taskId);
static void Task_PlaceYesNoBox(u32 taskId);
static void Task_SaleOfTMsCanceled(u32 taskId);
static void Task_InitQuantitySelectUI(u32 taskId);
static void SellTM_PrintQuantityAndSalePrice(s16 quantity, s32 value);
static void Task_QuantitySelect_HandleInput(u32 taskId);
static void Task_PrintSaleConfirmedText(u32 taskId);
static void Task_DoSaleOfTMs(u32 taskId);
static void Task_AfterSale_ReturnToList(u32 taskId);
static void Task_TMCaseDude1(u32 taskId);
static void Task_TMCaseDude_Playback(u32 taskId);
static void InitWindowTemplatesAndPals(void);
static void AddTextPrinterParameterized_ColorByIndex(u32 windowId, u32 fontId, const u8 * str, u32 x, u32 y, u32 letterSpacing, u32 lineSpacing, u32 speed, u32 colorIdx);
static void TMCase_SetWindowBorder1(u32 windowId);
static void TMCase_SetWindowBorder2(u32 windowId);
static void TMCase_PrintMessageWithFollowupTask(u32 taskId, u32 windowId, const u8 * str, TaskFunc func);
static void PrintStringTMCaseOnWindow3(void);
static void DrawMoveInfoUIMarkers(void);
static void TMCase_MoveCursor_UpdatePrintedTMInfo(u32 itemId);
static void HandlePrintMoneyOnHand(void);
static void HandleCreateYesNoMenu(u32 taskId, const struct YesNoFuncTable * ptrs);
static u32 AddTMContextMenu(u8 * a0, u32 a1);
static void RemoveTMContextMenu(u8 * a0);
static bool32 IsTMCaseWithPartyIcons(void);
static void CreateTMCasePartyIcons(u32 itemId);
static void UpdateTMCasePartyIcons(u32 itemId);

static const struct BgTemplate sBGTemplates[] = {
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0x000
    }, {
        .bg = 1,
        .charBaseIndex = 0,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0x000
    }, {
        .bg = 2,
        .charBaseIndex = 0,
        .mapBaseIndex = 29,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0x000
    }
};

static const TaskFunc sSelectTMActionTasks[] = {
    [TMCASE_FROMFIELD] = Task_SelectTMAction_FromFieldBag,
    [TMCASE_FROMPARTYGIVE] = Task_SelectTMAction_Type1,
    [TMCASE_FROMMARTSELL] = Task_SelectTMAction_FromSellMenu,
    [TMCASE_FROMPOKEMONSTORAGEPC] = Task_SelectTMAction_Type3,
    [TMCASE_FROMBATTLE] = NULL,
    [TMCASE_NA] = NULL,
    [TMCASE_CHOOSE_ITEM] = Task_BeginFadeOutFromTMCase,
};

static const struct MenuAction sMenuActions_UseGiveExit[] = {
    {gOtherText_Use,  TMHMContextMenuAction_Use },
    {gOtherText_Give, TMHMContextMenuAction_Give},
    {gOtherText_Exit, TMHMContextMenuAction_Exit},
};

static const u8 sMenuActionIndices_Field[] = {0, 1, 2};
static const u8 sMenuActionIndices_UnionRoom[] = {1, 2};
static const struct YesNoFuncTable sYesNoFuncTable = {Task_PrintSaleConfirmedText, Task_SaleOfTMsCanceled};

static const u8 sText_ClearTo18[] = _("{CLEAR_TO 18}");
static const u8 sText_SingleSpace[] = _(" ");

static ALIGNED(4) const u16 sPal3Override[] = {RGB(8, 8, 8), RGB(30, 16, 6)};

static const u8 sTextColors[][3] = {
    {0, 1, 2},
    {0, 2, 3},
    {0, 3, 6},
    {0, 14, 10}
};

static const struct WindowTemplate sWindowTemplates[] = {
    {
        .bg = 0,
        .tilemapLeft = 14,
        .tilemapTop = 1,
        .width = 15,
        .height = 10,
        .paletteNum = 15,
        .baseBlock = 0x081
    },
    {
        .bg = 0,
        .tilemapLeft = 12,
        .tilemapTop = 12,
        .width = 18,
        .height = 8,
        .paletteNum = 10,
        .baseBlock = 0x13f
    },
    {
        .bg = 1,
        .tilemapLeft = 5,
        .tilemapTop = 15,
        .width = 15,
        .height = 4,
        .paletteNum = 13,
        .baseBlock = 0x1f9
    },
    {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 1,
        .width = 10,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 0x235
    },
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 13,
        .width = 5,
        .height = 6,
        .paletteNum = 12,
        .baseBlock = 0x249
    },
    {
        .bg = 0,
        .tilemapLeft = 7,
        .tilemapTop = 13,
        .width = 5,
        .height = 6,
        .paletteNum = 12,
        .baseBlock = 0x267
    },
    {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 11,
        .baseBlock = 0x285
    },
    {
        .bg = 1,
        .tilemapLeft = 17,
        .tilemapTop = 9,
        .width = 12,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 0x2ed
    },
    {
        .bg = 1,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 8,
        .height = 3,
        .paletteNum = 13,
        .baseBlock = 0x31d
    },
    DUMMY_WIN_TEMPLATE
};

static const struct WindowTemplate sYesNoWindowTemplate = {0x01, 0x15, 0x09, 0x06, 0x04, 0x0f, 0x0335};

static const struct WindowTemplate sTMContextWindowTemplates[] = {
    {0x01, 0x16, 0x0d, 0x07, 0x06, 0x0f, 0x01cf},
    {0x01, 0x16, 0x0f, 0x07, 0x04, 0x0f, 0x01cf}
};

void InitTMCase(u32 type, void (* callback)(void), u32 a2)
{
    ResetBufferPointers_NoFree();

    sTMCaseDynamicResources = Alloc(sizeof(struct UnkStruct_203B118));
    sTMCaseDynamicResources->savedCallback = 0;
    sTMCaseDynamicResources->scrollIndicatorArrowPairId = 0xFF;
    sTMCaseDynamicResources->contextMenuWindowId = 0xFF;

    if (type != TMCASE_NA)
        sTMCaseStaticResources.tmCaseMenuType = type;

    if (callback != NULL)
        sTMCaseStaticResources.savedCallback = callback;

    if (a2 != 0xFF)
        sTMCaseStaticResources.unk_05 = a2;

    gTextFlags.autoScroll = FALSE;
    SetMainCallback2(CB2_SetUpTMCaseUI_Blocking);
}

static void CB2_Idle(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB_Idle(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void CB2_SetUpTMCaseUI_Blocking(void)
{
    while (TRUE)
    {
        if (MenuHelpers_CallLinkSomething())
            break;
        if (DoSetUpTMCaseUI())
            break;
        if (MenuHelpers_LinkSomething())
            break;
    }
}

static bool32 DoSetUpTMCaseUI(void)
{
    u32 taskId;

    switch (gMain.state)
    {
    case 0:
        SetVBlankHBlankCallbacksToNull();
        ClearScheduledBgCopiesToVram();
        gMain.state++;
        break;
    case 1:
        ScanlineEffect_Stop();
        gMain.state++;
        break;
    case 2:
        FreeAllSpritePalettes();
        gMain.state++;
        break;
    case 3:
        ResetPaletteFade();
        gMain.state++;
        break;
    case 4:
        ResetSpriteData();
        gMain.state++;
        break;
    case 5:
        ResetTasks();
        gMain.state++;
        break;
    case 6:
        LoadBGTemplates();
        sTMCaseDynamicResources->seqId = 0;
        gMain.state++;
        break;
    case 7:
        InitWindowTemplatesAndPals();
        gMain.state++;
        break;
    case 8:
        if (HandleLoadTMCaseGraphicsAndPalettes())
            gMain.state++;
        break;
    case 9:
        SortPocketAndPlaceHMsFirst(&gBagPockets[POCKET_TM_CASE - 1]);
        gMain.state++;
        break;
    case 10:
        TMCaseSetup_GetTMCount();
        TMCaseSetup_InitListMenuPositions();
        TMCaseSetup_UpdateVisualMenuOffset();
        gMain.state++;
        break;
    case 11:
        DrawMoveInfoUIMarkers();
        gMain.state++;
        break;
    case 12:
        CreateTMCaseListMenuBuffers();
        InitTMCaseListMenuItems();
        gMain.state++;
        break;
    case 13:
        PrintStringTMCaseOnWindow3();
        gMain.state++;
        break;
    case 14:
        if (sTMCaseStaticResources.tmCaseMenuType == TMCASE_FROMBATTLE)
            taskId = CreateTask(Task_TMCaseDude1, 0);
        else
            taskId = CreateTask(Task_TMCaseMain, 0);

        gTasks[taskId].data[0] = ListMenuInit(&gMultiuseListMenuTemplate, &sTMCaseStaticResources.scrollOffset, &sTMCaseStaticResources.selectedRow);
        gMain.state++;
        break;
    case 15:
        CreateTMCaseScrollIndicatorArrowPair_Main();
        gMain.state++;
        break;
    case 16:
        if (IsTMCaseWithPartyIcons())
            CreateTMCasePartyIcons(BagGetItemIdByPocketPosition(POCKET_TM_CASE, sTMCaseStaticResources.scrollOffset + sTMCaseStaticResources.selectedRow));
        gMain.state++;
        break;
    case 17:
        BlendPalettes(PALETTES_ALL, 16, 0);
        gMain.state++;
        break;
    case 18:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        gMain.state++;
        break;
    default:
        SetVBlankCallback(VBlankCB_Idle);
        SetMainCallback2(CB2_Idle);
        return TRUE;
    }

    return FALSE;
}

static void ResetBufferPointers_NoFree(void)
{
    sTMCaseDynamicResources = NULL;
    sTilemapBuffer = NULL;
    sListMenuItemsBuffer = NULL;
    sListMenuStringsBuffer = NULL;
}

static void LoadBGTemplates(void)
{
    void ** ptr;

    ResetAllBgsCoordinatesAndBgCntRegs();

    ptr = &sTilemapBuffer;
    *ptr = AllocZeroed(0x800);

    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sBGTemplates, ARRAY_COUNT(sBGTemplates));
    SetBgTilemapBuffer(2, *ptr);
    ScheduleBgCopyTilemapToVram(1);
    ScheduleBgCopyTilemapToVram(2);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG2);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(4, 3));
    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
}

static bool32 HandleLoadTMCaseGraphicsAndPalettes(void)
{
    switch (sTMCaseDynamicResources->seqId)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, gUnknown_8E845D8, 0, 0, 0);
        sTMCaseDynamicResources->seqId++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            LZDecompressWram(gUnknown_8E84A24, sTilemapBuffer);
            sTMCaseDynamicResources->seqId++;
        }
        break;
    case 2:
        sTMCaseDynamicResources->seqId++;
        break;
    case 3:
        if (gSaveBlock2Ptr->playerGender == MALE)
            LoadCompressedPalette(gUnknown_8E84CB0, 0, 0x80);
        else
            LoadCompressedPalette(gUnknown_8E84D20, 0, 0x80);
        sTMCaseDynamicResources->seqId++;
        break;
    case 4:
        LoadMonIconPalettes();
        sTMCaseDynamicResources->seqId++;
        break;
    default:
        sTMCaseDynamicResources->seqId = 0;
        return TRUE;
    }

    return FALSE;
}

static void CreateTMCaseListMenuBuffers(void)
{
    sListMenuItemsBuffer = Alloc((gBagPockets[POCKET_TM_CASE - 1].capacity + 1) * sizeof(struct ListMenuItem));
    sListMenuStringsBuffer = Alloc(sTMCaseDynamicResources->numTMs * 29);
}

static void InitTMCaseListMenuItems(void)
{
    struct BagPocket * pocket = &gBagPockets[POCKET_TM_CASE - 1];
    u32 i;

    for (i = 0; i < sTMCaseDynamicResources->numTMs; i++)
    {
        GetTMNumberAndMoveString(sListMenuStringsBuffer[i], pocket->itemSlots[i].itemId);
        sListMenuItemsBuffer[i].label = sListMenuStringsBuffer[i];
        sListMenuItemsBuffer[i].index = i;
    }
    sListMenuItemsBuffer[i].label = gText_Close;
    sListMenuItemsBuffer[i].index = -2;

    gMultiuseListMenuTemplate.items = sListMenuItemsBuffer;
    gMultiuseListMenuTemplate.totalItems = sTMCaseDynamicResources->numTMs + 1;
    gMultiuseListMenuTemplate.windowId = 0;
    gMultiuseListMenuTemplate.header_X = 0;
    gMultiuseListMenuTemplate.item_X = 8;
    gMultiuseListMenuTemplate.cursor_X = 0;
    gMultiuseListMenuTemplate.lettersSpacing = 0;
    gMultiuseListMenuTemplate.itemVerticalPadding = 2;
    gMultiuseListMenuTemplate.upText_Y = 2;
    gMultiuseListMenuTemplate.maxShowed = sTMCaseDynamicResources->maxTMsShown;
    gMultiuseListMenuTemplate.fontId = 2;
    gMultiuseListMenuTemplate.cursorPal = 2;
    gMultiuseListMenuTemplate.fillValue = 0;
    gMultiuseListMenuTemplate.cursorShadowPal = 3;
    gMultiuseListMenuTemplate.moveCursorFunc = TMCase_MoveCursorFunc;
    gMultiuseListMenuTemplate.itemPrintFunc = NULL;
    gMultiuseListMenuTemplate.cursorKind = 0;
    gMultiuseListMenuTemplate.scrollMultiple = 0;
}

static void GetTMNumberAndMoveString(u8 * dest, u32 itemId)
{
    StringCopy(gStringVar4, gText_FontSize0);
    StringAppend(gStringVar4, gOtherText_UnkF9_08_Clear_01);
    ConvertIntToDecimalStringN(gStringVar1, (itemId - FIRST_TM_INDEX) + 1, STR_CONV_MODE_LEADING_ZEROS, 3);
    StringAppend(gStringVar4, gStringVar1);
    StringAppend(gStringVar4, sText_SingleSpace);
    StringAppend(gStringVar4, gText_FontSize2);
    StringAppend(gStringVar4, gBattleMoves[ItemId_GetHoldEffectParam(itemId)].name);
    StringCopy(dest, gStringVar4);
}

static void TMCase_MoveCursorFunc(s32 itemIndex, bool32 onInit, struct ListMenu *list)
{
    u32 itemId;

    if (itemIndex == -2)
        itemId = 0;
    else
        itemId = BagGetItemIdByPocketPosition(POCKET_TM_CASE, itemIndex);

    if (!onInit)
    {
        PlaySE(SE_SELECT);
        
        if (IsTMCaseWithPartyIcons())
            UpdateTMCasePartyIcons(itemId);
    }
    TMCase_MoveCursor_UpdatePrintedDescription(itemIndex);
    TMCase_MoveCursor_UpdatePrintedTMInfo(itemId);
}

static void TMCase_MoveCursor_UpdatePrintedDescription(s32 itemIndex)
{
    const u8 * str;

    if (itemIndex != -2)
        str = ItemId_GetDescription(BagGetItemIdByPocketPosition(POCKET_TM_CASE, itemIndex));
    else
        str = gText_TMCaseWillBePutAway;
    
    FillWindowPixelBuffer(1, 0);
    AddTextPrinterParameterized_ColorByIndex(1, 2, str, 2, 3, 1, 0, 0, 0);
}

static void FillBG2RowWithPalette_2timesNplus1(s32 a0)
{
    SetBgTilemapPalette(2, 0, 12, 30, 8, 2 * a0 + 1);
    ScheduleBgCopyTilemapToVram(2);
}

static void PrintListMenuCursorByID_WithColorIdx(u32 a0, u32 a1)
{
    PrintListMenuCursorAt_WithColorIdx(ListMenuGetYCoordForPrintingArrowCursor(a0), a1);
}

static void PrintListMenuCursorAt_WithColorIdx(u32 a0, u32 a1)
{
    if (a1 == 0xFF)
    {
        FillWindowPixelRect(0, 0, 0, a0, GetFontAttribute(2, 0), GetFontAttribute(2, 1));
        CopyWindowToVram(0, COPYWIN_GFX);
    }
    else
        AddTextPrinterParameterized_ColorByIndex(0, 2, gText_SelectorArrow2, 0, a0, 0, 0, 0, a1);
}

static void CreateTMCaseScrollIndicatorArrowPair_Main(void)
{
    sTMCaseDynamicResources->scrollIndicatorArrowPairId = AddScrollIndicatorArrowPairParameterized(2, 0xA0, 0x08, 0x58, sTMCaseDynamicResources->numTMs - sTMCaseDynamicResources->maxTMsShown + 1, 0x6E, 0x6E, &sTMCaseStaticResources.scrollOffset);
}

static void CreateTMCaseScrollIndicatorArrowPair_SellQuantitySelect(void)
{
    sTMCaseDynamicResources->currItem = 1;
    sTMCaseDynamicResources->scrollIndicatorArrowPairId = AddScrollIndicatorArrowPairParameterized(2, 0x98, 0x48, 0x68, 2, 0x6E, 0x6E, &sTMCaseDynamicResources->currItem);
}

static void RemoveTMCaseScrollIndicatorArrowPair(void)
{
    if (sTMCaseDynamicResources->scrollIndicatorArrowPairId != 0xFF)
    {
        RemoveScrollIndicatorArrowPair(sTMCaseDynamicResources->scrollIndicatorArrowPairId);
        sTMCaseDynamicResources->scrollIndicatorArrowPairId = 0xFF;
    }
}

void ResetTMCaseCursorPos(void)
{
    sTMCaseStaticResources.selectedRow = 0;
    sTMCaseStaticResources.scrollOffset = 0;
}

static void TMCaseSetup_GetTMCount(void)
{
    struct BagPocket * pocket = &gBagPockets[POCKET_TM_CASE - 1];
    u32 i;

    BagPocketCompaction(pocket);
    
    sTMCaseDynamicResources->numTMs = 0;
    
    for (i = 0; i < pocket->capacity; i++)
    {
        if (pocket->itemSlots[i].itemId == ITEM_NONE)
            break;
        sTMCaseDynamicResources->numTMs++;
    }
    sTMCaseDynamicResources->maxTMsShown = min(sTMCaseDynamicResources->numTMs + 1, 5);
}

static void TMCaseSetup_InitListMenuPositions(void)
{
    if (sTMCaseStaticResources.scrollOffset != 0)
    {
        if (sTMCaseStaticResources.scrollOffset + sTMCaseDynamicResources->maxTMsShown > sTMCaseDynamicResources->numTMs + 1)
            sTMCaseStaticResources.scrollOffset = sTMCaseDynamicResources->numTMs + 1 - sTMCaseDynamicResources->maxTMsShown;
    }
    if (sTMCaseStaticResources.scrollOffset + sTMCaseStaticResources.selectedRow >= sTMCaseDynamicResources->numTMs + 1)
    {
        if (sTMCaseDynamicResources->numTMs + 1 < 2)
            sTMCaseStaticResources.selectedRow = 0;
        else
            sTMCaseStaticResources.selectedRow = sTMCaseDynamicResources->numTMs;
    }
}

static void TMCaseSetup_UpdateVisualMenuOffset(void)
{
    u8 i;

    if (sTMCaseStaticResources.selectedRow > 3)
    {
        for (i = 0; i <= sTMCaseStaticResources.selectedRow - 3 && sTMCaseStaticResources.scrollOffset + sTMCaseDynamicResources->maxTMsShown != sTMCaseDynamicResources->numTMs + 1; i++)
        {
            do {} while (0);
            sTMCaseStaticResources.selectedRow--;
            sTMCaseStaticResources.scrollOffset++;
        }
    }
}

static void DestroyTMCaseBuffers(void)
{
    if (sTMCaseDynamicResources != NULL)
        Free(sTMCaseDynamicResources);
    if (sTilemapBuffer != NULL)
        Free(sTilemapBuffer);
    if (sListMenuItemsBuffer != NULL)
        Free(sListMenuItemsBuffer);
    if (sListMenuStringsBuffer != NULL)
        Free(sListMenuStringsBuffer);
    FreeAllWindowBuffers();
}

static void Task_BeginFadeOutFromTMCase(u32 taskId)
{
    BeginNormalPaletteFade(PALETTES_ALL, -2, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_FadeOutAndCloseTMCase;
}

static void Task_FadeOutAndCloseTMCase(u32 taskId)
{
    s16 * data = gTasks[taskId].data;

    if (!gPaletteFade.active)
    {
        DestroyListMenuTask(data[0], &sTMCaseStaticResources.scrollOffset, &sTMCaseStaticResources.selectedRow);

        if (sTMCaseDynamicResources->savedCallback != NULL)
            SetMainCallback2(sTMCaseDynamicResources->savedCallback);
        else
            SetMainCallback2(sTMCaseStaticResources.savedCallback);

        RemoveTMCaseScrollIndicatorArrowPair();
        DestroyTMCaseBuffers();
        DestroyTask(taskId);
    }
}

static void Task_TMCaseMain(u32 taskId)
{
    s16 * data = gTasks[taskId].data;
    s32 input;

    if (!gPaletteFade.active)
    {
        if (MenuHelpers_CallLinkSomething() != TRUE)
        {
            input = ListMenu_ProcessInput(data[0]);

            ListMenuGetScrollAndRow(data[0], &sTMCaseStaticResources.scrollOffset, &sTMCaseStaticResources.selectedRow);

            if (JOY_NEW(SELECT_BUTTON) && sTMCaseStaticResources.unk_05 == 1)
            {
                PlaySE(SE_SELECT);
                gSpecialVar_ItemId = ITEM_NONE;
                Task_BeginFadeOutFromTMCase(taskId);
            }
            else
            {
                switch (input)
                {
                case -1:
                    break;
                case -2:
                    PlaySE(SE_SELECT);
                    gSpecialVar_ItemId = 0;
                    Task_BeginFadeOutFromTMCase(taskId);
                    break;
                default:
                    PlaySE(SE_SELECT);
                    FillBG2RowWithPalette_2timesNplus1(1);
                    RemoveTMCaseScrollIndicatorArrowPair();
                    PrintListMenuCursorByID_WithColorIdx(data[0], 2);
                    data[1] = input;
                    data[2] = BagGetQuantityByPocketPosition(POCKET_TM_CASE, input);
                    gSpecialVar_ItemId = BagGetItemIdByPocketPosition(POCKET_TM_CASE, input);
                    gTasks[taskId].func = sSelectTMActionTasks[sTMCaseStaticResources.tmCaseMenuType];
                    break;
                }
            }
        }
    }
}

static void Subtask_ReturnToTMCaseMain(u32 taskId)
{
    FillBG2RowWithPalette_2timesNplus1(0);
    CreateTMCaseScrollIndicatorArrowPair_Main();
    gTasks[taskId].func = Task_TMCaseMain;
}

static void Task_SelectTMAction_FromFieldBag(u32 taskId)
{
    u8 * strbuf;

    TMCase_SetWindowBorder2(2);

    if (!MenuHelpers_LinkSomething() && !InUnionRoom())
    {
        AddTMContextMenu(&sTMCaseDynamicResources->contextMenuWindowId, 0);
        sTMCaseDynamicResources->menuActionIndices = sMenuActionIndices_Field;
        sTMCaseDynamicResources->numMenuActions = ARRAY_COUNT(sMenuActionIndices_Field);
    }
    else
    {
        AddTMContextMenu(&sTMCaseDynamicResources->contextMenuWindowId, 1);
        sTMCaseDynamicResources->menuActionIndices = sMenuActionIndices_UnionRoom;
        sTMCaseDynamicResources->numMenuActions = ARRAY_COUNT(sMenuActionIndices_UnionRoom);
    }
    AddItemMenuActionTextPrinters(sTMCaseDynamicResources->contextMenuWindowId, 2, GetMenuCursorDimensionByFont(2, 0), 2, 0, GetFontAttribute(2, 1) + 2, sTMCaseDynamicResources->numMenuActions, sMenuActions_UseGiveExit, sTMCaseDynamicResources->menuActionIndices);
    Menu_InitCursor(sTMCaseDynamicResources->contextMenuWindowId, 2, 0, 2, GetFontAttribute(2, 1) + 2, sTMCaseDynamicResources->numMenuActions, 0);
    strbuf = Alloc(256);
    GetTMNumberAndMoveString(strbuf, gSpecialVar_ItemId);
    StringAppend(strbuf, gText_Var1IsSelected + 2); // +2 skips over the stringvar
    AddTextPrinterParameterized_ColorByIndex(2, 2, strbuf, 0, 2, 1, 0, 0, 1);
    Free(strbuf);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(1);
    gTasks[taskId].func = Task_TMContextMenu_HandleInput;
}

static void Task_TMContextMenu_HandleInput(u32 taskId)
{
    s8 input;

    if (!MenuHelpers_CallLinkSomething())
    {
        input = Menu_ProcessInputNoWrapAround();

        switch (input)
        {
        case -1:
            PlaySE(SE_SELECT);
            sMenuActions_UseGiveExit[sTMCaseDynamicResources->menuActionIndices[sTMCaseDynamicResources->numMenuActions - 1]].func.void_u32(taskId);
            break;
        case -2:
            break;
        default:
            PlaySE(SE_SELECT);
            sMenuActions_UseGiveExit[sTMCaseDynamicResources->menuActionIndices[input]].func.void_u32(taskId);
            break;
        }
    }
}

static void TMHMContextMenuAction_Use(u32 taskId)
{
    RemoveTMContextMenu(&sTMCaseDynamicResources->contextMenuWindowId);
    ClearStdWindowAndFrameToTransparent(2, 0);
    ClearWindowTilemap(2);
    PutWindowTilemap(0);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(1);

    if (CalculatePlayerPartyCount() == 0)
        PrintError_ThereIsNoPokemon(taskId);
    else
    {
        gItemUseCB = ItemUseCB_TM;
        sTMCaseDynamicResources->savedCallback = CB2_ShowPartyMenuForItemUse;
        Task_BeginFadeOutFromTMCase(taskId);
    }
}

static void TMHMContextMenuAction_Give(u32 taskId)
{
    s16 * data = gTasks[taskId].data;
    u32 itemId = BagGetItemIdByPocketPosition(POCKET_TM_CASE, data[1]);

    RemoveTMContextMenu(&sTMCaseDynamicResources->contextMenuWindowId);
    ClearStdWindowAndFrameToTransparent(2, 0);
    ClearWindowTilemap(2);
    PutWindowTilemap(1);
    PutWindowTilemap(4);
    PutWindowTilemap(5);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(1);
    
    if (CalculatePlayerPartyCount() == 0)
        PrintError_ThereIsNoPokemon(taskId);
    else
    {
        sTMCaseDynamicResources->savedCallback = CB2_ChooseMonToGiveItem;
        Task_BeginFadeOutFromTMCase(taskId);
    }
}

static void PrintError_ThereIsNoPokemon(u32 taskId)
{
    TMCase_PrintMessageWithFollowupTask(taskId, 2, gText_ThereIsNoPokemon, Task_WaitButtonAfterErrorPrint);
}

static void Task_WaitButtonAfterErrorPrint(u32 taskId)
{
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        Subtask_CloseContextMenuAndReturnToMain(taskId);
    }
}

static void Subtask_CloseContextMenuAndReturnToMain(u32 taskId)
{
    s16 * data = gTasks[taskId].data;

    DestroyListMenuTask(data[0], &sTMCaseStaticResources.scrollOffset, &sTMCaseStaticResources.selectedRow);
    data[0] = ListMenuInit(&gMultiuseListMenuTemplate, &sTMCaseStaticResources.scrollOffset, &sTMCaseStaticResources.selectedRow);
    PrintListMenuCursorByID_WithColorIdx(data[0], 1);
    ClearDialogWindowAndFrameToTransparent(6, 0);
    ClearWindowTilemap(6);
    PutWindowTilemap(1);
    PutWindowTilemap(4);
    PutWindowTilemap(5);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(1);
    Subtask_ReturnToTMCaseMain(taskId);
}

static void TMHMContextMenuAction_Exit(u32 taskId)
{
    s16 * data = gTasks[taskId].data;

    RemoveTMContextMenu(&sTMCaseDynamicResources->contextMenuWindowId);
    ClearStdWindowAndFrameToTransparent(2, 0);
    ClearWindowTilemap(2);
    PutWindowTilemap(0);
    PrintListMenuCursorByID_WithColorIdx(data[0], 1);
    PutWindowTilemap(1);
    PutWindowTilemap(4);
    PutWindowTilemap(5);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(1);
    Subtask_ReturnToTMCaseMain(taskId);
}

static void Task_SelectTMAction_Type1(u32 taskId)
{
    sTMCaseDynamicResources->savedCallback = CB2_GiveHoldItem;
    Task_BeginFadeOutFromTMCase(taskId);
}

static void Task_SelectTMAction_Type3(u32 taskId)
{
    sTMCaseDynamicResources->savedCallback = Cb2_ReturnToPSS;
    Task_BeginFadeOutFromTMCase(taskId);
}

static void Task_SelectTMAction_FromSellMenu(u32 taskId)
{
    s16 * data = gTasks[taskId].data;

    if (ItemId_GetPrice(gSpecialVar_ItemId) == 0)
    {
        CopyItemName(gSpecialVar_ItemId, gStringVar1);
        StringExpandPlaceholders(gStringVar4, gText_OhNoICantBuyThat);
        TMCase_PrintMessageWithFollowupTask(taskId, GetDialogBoxFontId(), gStringVar4, Subtask_CloseContextMenuAndReturnToMain);
    }
    else
    {
        data[8] = 1;

        if (data[2] == 1)
        {
            HandlePrintMoneyOnHand();
            Task_AskConfirmSaleWithAmount(taskId);
        }
        else
        {
            if (data[2] > 99)
                data[2] = 99;

            CopyItemName(gSpecialVar_ItemId, gStringVar1);
            StringExpandPlaceholders(gStringVar4, gText_HowManyWouldYouLikeToSell);
            TMCase_PrintMessageWithFollowupTask(taskId, GetDialogBoxFontId(), gStringVar4, Task_InitQuantitySelectUI);
        }
    }
}

static void Task_AskConfirmSaleWithAmount(u32 taskId)
{
    s16 * data = gTasks[taskId].data;

    ConvertIntToDecimalStringN(gStringVar3, ItemId_GetPrice(BagGetItemIdByPocketPosition(POCKET_TM_CASE, data[1])) / ITEM_SELL_DIVISOR * data[8], STR_CONV_MODE_LEFT_ALIGN, 6);
    StringExpandPlaceholders(gStringVar4, gText_ICanPayThisMuch_WouldThatBeOkay);
    TMCase_PrintMessageWithFollowupTask(taskId, GetDialogBoxFontId(), gStringVar4, Task_PlaceYesNoBox);
}

static void Task_PlaceYesNoBox(u32 taskId)
{
    HandleCreateYesNoMenu(taskId, &sYesNoFuncTable);
}

static void Task_SaleOfTMsCanceled(u32 taskId)
{
    s16 * data = gTasks[taskId].data;

    ClearStdWindowAndFrameToTransparent(8, 0);
    ClearDialogWindowAndFrameToTransparent(6, 0);
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    PutWindowTilemap(3);
    PutWindowTilemap(4);
    PutWindowTilemap(5);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(1);
    PrintListMenuCursorByID_WithColorIdx(data[0], 1);
    Subtask_ReturnToTMCaseMain(taskId);
}

static void Task_InitQuantitySelectUI(u32 taskId)
{
    s16 * data = gTasks[taskId].data;

    TMCase_SetWindowBorder1(7);
    ConvertIntToDecimalStringN(gStringVar1, 1, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringExpandPlaceholders(gStringVar4, gText_TimesStrVar1);
    AddTextPrinterParameterized_ColorByIndex(7, 0, gStringVar4, 4, 10, 1, 0, 0, 1);
    SellTM_PrintQuantityAndSalePrice(1, ItemId_GetPrice(BagGetItemIdByPocketPosition(POCKET_TM_CASE, data[1])) / ITEM_SELL_DIVISOR * data[8]);
    HandlePrintMoneyOnHand();
    CreateTMCaseScrollIndicatorArrowPair_SellQuantitySelect();
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(1);
    gTasks[taskId].func = Task_QuantitySelect_HandleInput;
}

static void SellTM_PrintQuantityAndSalePrice(s16 quantity, s32 amount)
{
    FillWindowPixelBuffer(7, 0x11);
    ConvertIntToDecimalStringN(gStringVar1, quantity, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringExpandPlaceholders(gStringVar4, gText_TimesStrVar1);
    AddTextPrinterParameterized_ColorByIndex(7, 0, gStringVar4, 4, 10, 1, 0, 0, 1);
    PrintMoneyAmount(7, 0x38, 0x0A, amount, 0);
}

static void Task_QuantitySelect_HandleInput(u32 taskId)
{
    s16 * data = gTasks[taskId].data;

    if (AdjustQuantityAccordingToDPadInput(&data[8], data[2]) == 1)
        SellTM_PrintQuantityAndSalePrice(data[8], ItemId_GetPrice(BagGetItemIdByPocketPosition(POCKET_TM_CASE, data[1])) / ITEM_SELL_DIVISOR * data[8]);
    else if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        ClearStdWindowAndFrameToTransparent(7, 0);
        ScheduleBgCopyTilemapToVram(0);
        ScheduleBgCopyTilemapToVram(1);
        RemoveTMCaseScrollIndicatorArrowPair();
        Task_AskConfirmSaleWithAmount(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        ClearStdWindowAndFrameToTransparent(7, 0);
        ClearStdWindowAndFrameToTransparent(8, 0);
        ClearDialogWindowAndFrameToTransparent(6, 0);
        PutWindowTilemap(3);
        PutWindowTilemap(0);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        ScheduleBgCopyTilemapToVram(1);
        RemoveTMCaseScrollIndicatorArrowPair();
        PrintListMenuCursorByID_WithColorIdx(data[0], 1);
        Subtask_ReturnToTMCaseMain(taskId);
    }
}

static void Task_PrintSaleConfirmedText(u32 taskId)
{
    s16 * data = gTasks[taskId].data;

    PutWindowTilemap(0);
    ScheduleBgCopyTilemapToVram(0);
    CopyItemName(gSpecialVar_ItemId, gStringVar1);
    ConvertIntToDecimalStringN(gStringVar3, ItemId_GetPrice(BagGetItemIdByPocketPosition(POCKET_TM_CASE, data[1])) / ITEM_SELL_DIVISOR * data[8], STR_CONV_MODE_LEFT_ALIGN, 6);
    StringExpandPlaceholders(gStringVar4, gText_TurnedOverItemsWorthYen);
    TMCase_PrintMessageWithFollowupTask(taskId, 2, gStringVar4, Task_DoSaleOfTMs);
}

static void Task_DoSaleOfTMs(u32 taskId)
{
    s16 * data = gTasks[taskId].data;

    PlaySE(SE_SHOP);
    RemoveBagItem(gSpecialVar_ItemId, data[8]);
    AddMoney(&gSaveBlock1Ptr->money, ItemId_GetPrice(gSpecialVar_ItemId) / ITEM_SELL_DIVISOR * data[8]);
    DestroyListMenuTask(data[0], &sTMCaseStaticResources.scrollOffset, &sTMCaseStaticResources.selectedRow);
    TMCaseSetup_GetTMCount();
    TMCaseSetup_InitListMenuPositions();
    InitTMCaseListMenuItems();
    data[0] = ListMenuInit(&gMultiuseListMenuTemplate, &sTMCaseStaticResources.scrollOffset, &sTMCaseStaticResources.selectedRow);
    PrintListMenuCursorByID_WithColorIdx(data[0], 2);
    PrintMoneyAmountInMoneyBox(8, GetMoney(&gSaveBlock1Ptr->money), 0);
    gTasks[taskId].func = Task_AfterSale_ReturnToList;
}

static void Task_AfterSale_ReturnToList(u32 taskId)
{
    if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        ClearStdWindowAndFrameToTransparent(8, 0);
        ClearDialogWindowAndFrameToTransparent(6, 0);
        PutWindowTilemap(1);
        PutWindowTilemap(3);
        PutWindowTilemap(4);
        PutWindowTilemap(5);
        Subtask_CloseContextMenuAndReturnToMain(taskId);
    }
}

void Pokedude_InitTMCase(void)
{
    sPokedudePackBackup = AllocZeroed(sizeof(*sPokedudePackBackup));

    memcpy(sPokedudePackBackup->bagPocket_TMHM, gSaveBlock1Ptr->bagPocket_TMHM, sizeof(gSaveBlock1Ptr->bagPocket_TMHM));
    memcpy(sPokedudePackBackup->bagPocket_KeyItems, gSaveBlock1Ptr->bagPocket_KeyItems, sizeof(gSaveBlock1Ptr->bagPocket_KeyItems));
    
    sPokedudePackBackup->unk_160 = sTMCaseStaticResources.selectedRow;
    sPokedudePackBackup->unk_162 = sTMCaseStaticResources.scrollOffset;

    ClearItemSlots(gSaveBlock1Ptr->bagPocket_TMHM, ARRAY_COUNT(gSaveBlock1Ptr->bagPocket_TMHM));
    ClearItemSlots(gSaveBlock1Ptr->bagPocket_KeyItems, ARRAY_COUNT(gSaveBlock1Ptr->bagPocket_KeyItems));

    ResetTMCaseCursorPos();

    AddBagItem(ITEM_TM_TAKE_DOWN, 1);
    AddBagItem(ITEM_TM_FAKE_TEARS, 1);
    AddBagItem(ITEM_TM_HYPER_BEAM, 1);
    AddBagItem(ITEM_TM_SUNNY_DAY, 1);

    InitTMCase(TMCASE_FROMBATTLE, CB2_ReturnToTeachyTV, 0);
}

static void Task_TMCaseDude1(u32 taskId)
{
    s16 * data = gTasks[taskId].data;

    if (!gPaletteFade.active)
    {
        data[8] = 0;
        data[9] = 0;
        gTasks[taskId].func = Task_TMCaseDude_Playback;
    }
}

static void Task_TMCaseDude_Playback(u32 taskId)
{
    s16 * data = gTasks[taskId].data;

    if (JOY_NEW(B_BUTTON))
    {
        if (data[8] < 21)
        {
            data[8] = 21;
            SetTeachyTvControllerModeToResume();
        }
    }

    switch (data[8])
    {
    case 0:
        BeginNormalPaletteFade(0xFFFF8405, 4, 0, 6, 0);
        FillBG2RowWithPalette_2timesNplus1(1);
        data[8]++;
        break;
    case 1:
    case 11:
        if (!gPaletteFade.active)
        {
            data[9]++;
            if (data[9] > 0x65)
            {
                data[9] = 0;
                data[8]++;
            }
        }
        break;
    case 2:
    case 3:
    case 4:
    case 12:
    case 13:
    case 14:
        if (data[9] == 0)
        {
            gMain.newKeys = 0;
            gMain.newAndRepeatedKeys = DPAD_DOWN;
            ListMenu_ProcessInput(data[0]);
        }
        data[9]++;
        if (data[9] > 0x65)
        {
            data[9] = 0;
            data[8]++;
        }
        break;
    case 5:
    case 6:
    case 7:
    case 15:
    case 16:
    case 17:
        if (data[9] == 0)
        {
            gMain.newKeys = 0;
            gMain.newAndRepeatedKeys = DPAD_UP;
            ListMenu_ProcessInput(data[0]);
        }

        if (++data[9] > 0x65)
        {
            data[9] = 0;
            data[8]++;
        }
        break;
    case 8:
        FillBG2RowWithPalette_2timesNplus1(1);
        TMCase_PrintMessageWithFollowupTask(taskId, 4, gPokedudeText_TMTypes, 0);
        gTasks[taskId].func = Task_TMCaseDude_Playback;
        data[8]++;
        break;
    case 9:
    case 19:
        RunTextPrinters();

        if (!IsTextPrinterActive(6))
            data[8]++;
        break;
    case 10:
        if (JOY_NEW(A_BUTTON | B_BUTTON))
        {
            FillBG2RowWithPalette_2timesNplus1(0);
            BeginNormalPaletteFade(0x00000400, 0, 6, 0, 0);
            ClearDialogWindowAndFrameToTransparent(6, 0);
            ScheduleBgCopyTilemapToVram(1);
            data[8]++;
        }
        break;
    case 18:
        FillBG2RowWithPalette_2timesNplus1(1);
        TMCase_PrintMessageWithFollowupTask(taskId, 4, gPokedudeText_ReadTMDescription, NULL);
        gTasks[taskId].func = Task_TMCaseDude_Playback; // this function
        data[8]++;
        break;
    case 20:
        if (JOY_NEW(A_BUTTON | B_BUTTON))
            data[8]++;
        break;
    case 21:
        if (!gPaletteFade.active)
        {
            memcpy(gSaveBlock1Ptr->bagPocket_TMHM, sPokedudePackBackup->bagPocket_TMHM, sizeof(gSaveBlock1Ptr->bagPocket_TMHM));
            memcpy(gSaveBlock1Ptr->bagPocket_KeyItems, sPokedudePackBackup->bagPocket_KeyItems, sizeof(gSaveBlock1Ptr->bagPocket_KeyItems));
            DestroyListMenuTask(data[0], NULL, NULL);
            sTMCaseStaticResources.selectedRow = sPokedudePackBackup->unk_160;
            sTMCaseStaticResources.scrollOffset = sPokedudePackBackup->unk_162;
            Free(sPokedudePackBackup);
            CpuFastCopy(gPlttBufferFaded, gPlttBufferUnfaded, 0x400);
            CB2_SetUpReshowBattleScreenAfterMenu();
            BeginNormalPaletteFade(PALETTES_ALL, -2, 0, 16, 0);
            data[8]++;
        }
        break;
    default:
        if (!gPaletteFade.active)
        {
            SetMainCallback2(sTMCaseStaticResources.savedCallback);
            RemoveTMCaseScrollIndicatorArrowPair();
            DestroyTMCaseBuffers();
            DestroyTask(taskId);
        }
        break;
    }
}

static void InitWindowTemplatesAndPals(void)
{
    u32 i;

    InitWindows(sWindowTemplates);
    DeactivateAllTextPrinters();

    TextWindow_SetUserSelectedFrame(0, 0x5B, 0xE0);
    TextWindow_LoadResourcesStdFrame0(0, 0x64, 0xB0);
    TextWindow_SetStdFrame0_WithPal(0, 0x78, 0xD0);

    Menu_LoadStdPalAt(0xF0, 0x20);
    Menu_LoadStdPalAt(0xA0, 0x20);

    LoadPalette(sPal3Override, 0xF6, 0x04);
    LoadPalette(sPal3Override, 0xD6, 0x04);

    ListMenuLoadStdPalAt(0xc0, 0x01);

    for (i = 0; i < 9; i++)
        FillWindowPixelBuffer(i, 0x00);

    PutWindowTilemap(0);
    PutWindowTilemap(1);
    PutWindowTilemap(3);
    PutWindowTilemap(4);
    PutWindowTilemap(5);
    
    ScheduleBgCopyTilemapToVram(0);
}

static void AddTextPrinterParameterized_ColorByIndex(u32 windowId, u32 fontId, const u8 * str, u32 x, u32 y, u32 letterSpacing, u32 lineSpacing, u32 speed, u32 colorIdx)
{
    AddTextPrinterParameterized4(windowId, fontId, x, y, letterSpacing, lineSpacing, sTextColors[colorIdx], speed, str);
}

static void TMCase_SetWindowBorder1(u32 windowId)
{
    DrawStdFrameWithCustomTileAndPalette(windowId, FALSE, 0x5B, 0x0E);
}

static void TMCase_SetWindowBorder2(u32 windowId)
{
    DrawStdFrameWithCustomTileAndPalette(windowId, FALSE, 0x78, 0x0D);
}

static void TMCase_PrintMessageWithFollowupTask(u32 taskId, u32 windowId, const u8 * str, TaskFunc func)
{
    DisplayMessageAndContinueTask(taskId, 6, 0x64, 0x0B, windowId, GetTextSpeedSetting(), str, func);
    ScheduleBgCopyTilemapToVram(1);
}

static void PrintStringTMCaseOnWindow3(void)
{
    u32 distance = 104 - GetStringWidth(1, gText_TMCase, 0);
    AddTextPrinterParameterized3(3, 1, distance / 2, 1, sTextColors[0], 0, gText_TMCase);
}

static void DrawMoveInfoUIMarkers(void)
{
    BlitMoveInfoIcon(4, MOVE_INFO_TYPE_TEXT_ICON, 0, 0);
    BlitMoveInfoIcon(4, MOVE_INFO_POWER_TEXT_ICON, 0, 12);
    BlitMoveInfoIcon(4, MOVE_INFO_ACCURACY_TEXT_ICON, 0, 24);
    BlitMoveInfoIcon(4, MOVE_INFO_PP_TEXT_ICON, 0, 36);
    CopyWindowToVram(4, COPYWIN_GFX);
}

static void TMCase_MoveCursor_UpdatePrintedTMInfo(u32 itemId)
{
    u32 i, move;
    const u8 * str;

    FillWindowPixelRect(5, 0, 0, 0, 40, 48);

    if (itemId == ITEM_NONE)
    {
        for (i = 0; i < 4; i++)
            AddTextPrinterParameterized_ColorByIndex(5, 3, gText_ThreeHyphens, 7, 12 * i, 0, 0, 0xFF, 3);

        CopyWindowToVram(5, COPYWIN_GFX);
    }
    else
    {
        move = ItemId_GetHoldEffectParam(itemId);

        BlitMoveInfoIcon(5, MOVE_INFO_MOVE_TYPE_ICON + gBattleMoves[move].type, 0, 0);

        if (gBattleMoves[move].power < 2)
            str = gText_ThreeHyphens;
        else
        {
            ConvertIntToDecimalStringN(gStringVar1, gBattleMoves[move].power, STR_CONV_MODE_RIGHT_ALIGN, 3);
            str = gStringVar1;
        }
        AddTextPrinterParameterized_ColorByIndex(5, 3, str, 7, 12, 0, 0, 0xFF, 3);

        if (gBattleMoves[move].accuracy == 0)
            str = gText_ThreeHyphens;
        else
        {
            ConvertIntToDecimalStringN(gStringVar1, gBattleMoves[move].accuracy, STR_CONV_MODE_RIGHT_ALIGN, 3);
            str = gStringVar1;
        }
        AddTextPrinterParameterized_ColorByIndex(5, 3, str, 7, 24, 0, 0, 0xFF, 3);
        ConvertIntToDecimalStringN(gStringVar1, gBattleMoves[move].pp, STR_CONV_MODE_RIGHT_ALIGN, 3);
        AddTextPrinterParameterized_ColorByIndex(5, 3, gStringVar1, 7, 36, 0, 0, 0xFF, 3);
        CopyWindowToVram(5, COPYWIN_GFX);
    }
}

static void HandlePrintMoneyOnHand(void)
{
    PrintMoneyAmountInMoneyBoxWithBorder(8, 0x78, 0xD, GetMoney(&gSaveBlock1Ptr->money));
}

static void HandleCreateYesNoMenu(u32 taskId, const struct YesNoFuncTable *ptrs)
{
    CreateYesNoMenuWithCallbacks(taskId, &sYesNoWindowTemplate, 2, 0, 2, 0x5B, 0x0E, ptrs);
}

static u32 AddTMContextMenu(u8 * a0, u32 a1)
{
    if (*a0 == 0xFF)
    {
        *a0 = AddWindow(&sTMContextWindowTemplates[a1]);
        TMCase_SetWindowBorder1(*a0);
        ScheduleBgCopyTilemapToVram(0);
    }
    return *a0;
}

static void RemoveTMContextMenu(u8 * a0)
{
    ClearStdWindowAndFrameToTransparent(*a0, FALSE);
    ClearWindowTilemap(*a0);
    RemoveWindow(*a0);
    ScheduleBgCopyTilemapToVram(0);
    *a0 = 0xFF;
}

static bool32 IsTMCaseWithPartyIcons(void)
{
    if (sTMCaseStaticResources.tmCaseMenuType == TMCASE_FROMBATTLE || sTMCaseStaticResources.tmCaseMenuType == TMCASE_CHOOSE_ITEM)
        return FALSE;
    return TRUE;
}

static void CreateTMCasePartyIcons(u32 itemId)
{
    u32 i, species, spriteId;
    
    for (i = 0; i < PARTY_SIZE; i++)
    {
        species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2);
        
        if (species)
        {
            spriteId = CreateMonIcon(species, SpriteCallbackDummy, 15 + 32 * (i % (PARTY_SIZE / 2)), 44 + 32 * (i / (PARTY_SIZE / 2)), 0);
            gSprites[spriteId].oam.priority = 2;
            gSprites[spriteId].data[0] = i;
        }
        else
            spriteId = MAX_SPRITES;
        
        sTMCaseDynamicResources->partyIconSpriteIds[i] = spriteId;
    }
    UpdateTMCasePartyIcons(itemId);
}

static void UpdateTMCasePartyIcons(u32 itemId)
{
    u32 i, move = ItemId_GetHoldEffectParam(itemId);
    struct Sprite *sprite;
    
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (sTMCaseDynamicResources->partyIconSpriteIds[i] != MAX_SPRITES)
        {
            sprite = &gSprites[sTMCaseDynamicResources->partyIconSpriteIds[i]];
            
            if (!CanMonLearnTM(&gPlayerParty[sprite->data[0]], move))
            {
                sprite->callback = SpriteCallbackDummy;
                sprite->oam.objMode = ST_OAM_OBJ_BLEND;
            }
            else
            {
                if (FindMoveSlotInMoveset(&gPlayerParty[sprite->data[0]], move) != MAX_MON_MOVES)
                    sprite->callback = SpriteCallbackDummy;
                else
                    sprite->callback = SpriteCB_MonIcon;
                
                sprite->oam.objMode = ST_OAM_OBJ_NORMAL;
            }
        }
    }
}

#include "global.h"
#include "battle_script_commands.h"
#include "dexnav.h"
#include "event_data.h"
#include "event_object_lock.h"
#include "event_object_movement.h"
#include "event_scripts.h"
#include "fieldmap.h"
#include "field_fadetransition.h"
#include "field_player_avatar.h"
#include "field_specials.h"
#include "field_weather.h"
#include "gpu_regs.h"
#include "item_menu.h"
#include "link.h"
#include "link_rfu.h"
#include "list_menu.h"
#include "load_save.h"
#include "malloc.h"
#include "menu.h"
#include "menu_helpers.h"
#include "menu_indicators.h"
#include "new_game.h"
#include "new_menu_helpers.h"
#include "option_menu.h"
#include "overworld.h"
#include "party_menu.h"
#include "pokedex.h"
#include "pokedex_screen.h"
#include "region_map.h"
#include "ride_pager.h"
#include "rtc.h"
#include "safari_zone.h"
#include "save.h"
#include "scanline_effect.h"
#include "script.h"
#include "sound.h"
#include "start_menu.h"
#include "strings.h"
#include "string_util.h"
#include "task.h"
#include "text_window.h"
#include "trainer_card.h"
#include "window.h"
#include "constants/flags.h"
#include "constants/songs.h"
#include "constants/trainer_card.h"

enum
{
	START_MENU_DEXNAV,
	START_MENU_POKEDEX,
	START_MENU_POKEMON,
	START_MENU_BAG,
	START_MENU_PLAYER,
	START_MENU_PLAYER_LINK,
	START_MENU_SAVE,
	START_MENU_OPTIONS,
	START_MENU_EXIT,
	START_MENU_RETIRE,
	START_MENU_RIDE_PAGER,
	NUM_START_MENU_OPTIONS,
};

struct StartMenu
{
	u8 drawState;
	u8 windowId;
	u8 clockTaskId;
	u8 listMenuTaskId;
	u16 cursorPos;
	u16 itemsAbove;
	struct ListMenuItem *itemsList;
	u8 (*saveDialogCB)(void);
	u8 selectedIndex;
	u8 scrollArrowTaskId;
	bool8 saveDialogIsPrinting;
	u8 saveDialogDelay;
	u8 saveStatsWindowId;
};

struct StartMenuOption
{
	u16 index;
	u16 flagId;
};

static void Task_DoLoadStartMenu(u8 taskId);
static void CreateStartMenuWindowAndList(void);
static void RemoveStartMenuWindow(void);
static u8 SetUpStartMenuItems(void);
static u8 DoLoadStartMenuItems(const struct StartMenuOption *options, u8 maxOptions);
static void StartMenu_MoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list);
static bool8 StartMenu_CanStartSelectedIndexAction(u8 index);
static void StartMenu_FadeScreenIfLeavingOverworld(u8 index);
static void DrawClockBox(void);
static void RemoveClockBox(void);
static u8 StartMenuDexnavCallback(void);
static u8 StartMenuPokedexCallback(void);
static u8 StartMenuPokemonCallback(void);
static u8 StartMenuBagCallback(void);
static u8 StartMenuPlayerCallback(void);
static u8 StartMenuPlayerLinkCallback(void);
static u8 StartMenuSaveCallback(void);
static u8 StartMenuOptionCallback(void);
static u8 StartMenuExitCallback(void);
static u8 StartMenuRetireCallback(void);
static u8 StartMenuRidePagerCallback(void);
static void StartMenu_PrepareForSave(void);
static u8 RunSaveDialogCB(void);
static u8 SaveDialogCB_PrintAskSaveText(void);
static u8 SaveDialogCB_AskSavePrintYesNoMenu(void);
static u8 SaveDialogCB_AskSaveHandleInput(void);
static u8 SaveDialogCB_PrintAskOverwriteText(void);
static u8 SaveDialogCB_AskReplacePreviousFilePrintYesNoMenu(void);
static u8 SaveDialogCB_AskOverwriteOrReplacePreviousFileHandleInput(void);
static u8 SaveDialogCB_AskOverwritePrintYesNoMenu(void);
static u8 SaveDialogCB_PrintSavingDontTurnOffPower(void);
static u8 SaveDialogCB_DoSave(void);
static u8 SaveDialogCB_PrintSaveResult(void);
static u8 SaveDialogCB_WaitPrintSuccessAndPlaySE(void);
static u8 SaveDialogCB_WaitPrintErrorAndPlaySE(void);
static u8 SaveDialogCB_ReturnSuccess(void);
static u8 SaveDialogCB_ReturnError(void);

static EWRAM_DATA struct StartMenu sStartMenu = {0};

#include "data/start_menu.h"

////////////////
// START MENU //
////////////////

#define tInputState data[0]

void ShowStartMenu(void)
{
	u8 taskId;
	
	if (!IsUpdateLinkStateCBActive())
    {
        FreezeObjectEvents();
        HandleEnforcedLookDirectionOnPlayerStopMoving();
        StopPlayerAvatar();
    }
	sStartMenu.drawState = 0;
	
	SetTaskFuncWithFollowupFunc(CreateTask(Task_DoLoadStartMenu, 80), Task_DoLoadStartMenu, Task_StartMenuHandleInput);

    ScriptContext2_Enable();
}

static void CloseStartMenu(void)
{
	RemoveClockBox();
    RemoveStartMenuWindow();
    ClearPlayerHeldMovementAndUnfreezeObjectEvents();
    ScriptContext2_Disable();
}

static bool8 DoDrawStartMenu(void)
{
	switch (sStartMenu.drawState)
    {
    case 0:
        sStartMenu.drawState++;
        break;
    case 1:
		CreateStartMenuWindowAndList();
        sStartMenu.drawState++;
        break;
    case 2:
		DrawClockBox();
		sStartMenu.drawState++;
		break;
	case 3:
		CopyWindowToVram(sStartMenu.windowId, COPYWIN_MAP);
        return TRUE;
    }
    return FALSE;
}

static void Task_DoLoadStartMenu(u8 taskId)
{
	if (DoDrawStartMenu())
		SwitchTaskToFollowupFunc(taskId);
}

static void DrawStartMenuInOneGo(void)
{
    sStartMenu.drawState = 0;
    while (!DoDrawStartMenu());
}

static bool8 FieldCB2_DrawStartMenu(void)
{
    if (!DoDrawStartMenu())
        return FALSE;
	
    FadeTransition_FadeInOnReturnToStartMenu();
	
    return TRUE;
}

void SetUpReturnToStartMenu(void)
{
    sStartMenu.drawState = 0;
    gFieldCallback2 = FieldCB2_DrawStartMenu;
}

static u8 CreateStartMenuWindow(u8 numItems)
{
	struct WindowTemplate template;
	u8 windowId;
	
	LoadStdWindowFrameGfx();
    template = SetWindowTemplateFields(0, 0x16, 1, 7, numItems * 2, DLG_WINDOW_PALETTE_NUM, 0x150);
	windowId = AddWindow(&template);
	PutWindowTilemap(windowId);
	DrawStdWindowFrame(windowId, FALSE);
	
    return windowId;
}

static void RemoveStartMenuWindow(void)
{
	ClearStdWindowAndFrame(sStartMenu.windowId, TRUE);
    RemoveWindow(sStartMenu.windowId);
	DestroyListMenuTask(sStartMenu.listMenuTaskId, NULL, NULL);
	RemoveScrollIndicatorArrowPair(sStartMenu.scrollArrowTaskId);
	FREE_AND_SET_NULL(sStartMenu.itemsList);
}

static void CreateStartMenuWindowAndList(void)
{
	u8 numItems = SetUpStartMenuItems(), maxShowed = numItems > 9 ? 9 : numItems;
	
	// Create window
	sStartMenu.windowId = CreateStartMenuWindow(maxShowed);
	
	// Create list menu
	gMultiuseListMenuTemplate.items = sStartMenu.itemsList;
	gMultiuseListMenuTemplate.totalItems = numItems;
	gMultiuseListMenuTemplate.windowId = sStartMenu.windowId;
    gMultiuseListMenuTemplate.header_X = 0;
    gMultiuseListMenuTemplate.item_X = 8;
    gMultiuseListMenuTemplate.cursor_X = 0;
    gMultiuseListMenuTemplate.lettersSpacing = 0;
    gMultiuseListMenuTemplate.itemVerticalPadding = 3;
    gMultiuseListMenuTemplate.upText_Y = 0;
    gMultiuseListMenuTemplate.maxShowed = maxShowed;
    gMultiuseListMenuTemplate.fontId = 0;
    gMultiuseListMenuTemplate.cursorPal = 2;
    gMultiuseListMenuTemplate.fillValue = 1;
    gMultiuseListMenuTemplate.cursorShadowPal = 3;
    gMultiuseListMenuTemplate.moveCursorFunc = StartMenu_MoveCursorFunc;
    gMultiuseListMenuTemplate.itemPrintFunc = NULL;
    gMultiuseListMenuTemplate.scrollMultiple = 0;
    gMultiuseListMenuTemplate.cursorKind = 0;
	
	sStartMenu.listMenuTaskId = ListMenuInit(&gMultiuseListMenuTemplate, sStartMenu.cursorPos, sStartMenu.itemsAbove);
	
	// Create scroll arrow
	sStartMenu.scrollArrowTaskId = AddScrollIndicatorArrowPairParameterized(SCROLL_ARROW_UP, 205, 5, 155, numItems - maxShowed, 110, 110, &sStartMenu.cursorPos);
}

static u8 SetUpStartMenuItems(void)
{
	if (IsUpdateLinkStateCBActive())
		return DoLoadStartMenuItems(sStartMenuOptionsLink, ARRAY_COUNT(sStartMenuOptionsLink));
	else if (InUnionRoom())
		return DoLoadStartMenuItems(sStartMenuOptionsUnionRoom, ARRAY_COUNT(sStartMenuOptionsUnionRoom));
	else if (GetSafariZoneFlag())
		return DoLoadStartMenuItems(sStartMenuOptionsSafari, ARRAY_COUNT(sStartMenuOptionsSafari));
	else
		return DoLoadStartMenuItems(sStartMenuOptionsNormal, ARRAY_COUNT(sStartMenuOptionsNormal));
}

static u8 DoLoadStartMenuItems(const struct StartMenuOption *options, u8 maxOptions)
{
	u8 i, index, numOptions = 0;
	struct ListMenuItem items[maxOptions];
	
	for (i = 0; i < maxOptions; i++)
	{
		if (!options[i].flagId || FlagGet(options[i].flagId))
		{
			index = options[i].index;
			
			if (sStartMenuOptions[index].text != NULL)
				items[numOptions].label = sStartMenuOptions[index].text;
			else
			{
				switch (index)
				{
					case START_MENU_PLAYER:
					case START_MENU_PLAYER_LINK:
						items[numOptions].label = gSaveBlock2Ptr->playerName;
						break;
				}
			}
			items[numOptions].index = index;
			++numOptions;
		}
	}
	sStartMenu.itemsList = Alloc(sizeof(struct ListMenuItem) * numOptions);
	
	for (i = 0; i < numOptions; i++)
		sStartMenu.itemsList[i] = items[i];
	
	return numOptions;
}

static u8 StartMenu_HandleInput(void)
{
	s32 input = ListMenu_ProcessInput(sStartMenu.listMenuTaskId);
	
	ListMenuGetScrollAndRow(sStartMenu.listMenuTaskId, &sStartMenu.cursorPos, &sStartMenu.itemsAbove);
	
	switch (input)
	{
		case LIST_CANCEL:
		    return 2;
		case LIST_NOTHING_CHOSEN:
		    if (JOY_NEW(START_BUTTON))
				return 2;
			break;
		default:
		    return 1;
	}
	return 0;
}

void Task_StartMenuHandleInput(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	
    switch (tInputState)
    {
    case INPUT_STATE_SET_IN_UNION_ROOM:
		TrySetUsingUnionRoomStartMenu();
        tInputState = INPUT_STATE_READ_KEYS;
        break;
    case INPUT_STATE_READ_KEYS:
		switch (StartMenu_HandleInput())
		{
			case 0: // Does nothing
			    break;
			case 1: // Use selected option
			    if (StartMenu_CanStartSelectedIndexAction(sStartMenu.selectedIndex))
				{
					PlaySE(SE_SELECT);
					StartMenu_FadeScreenIfLeavingOverworld(sStartMenu.selectedIndex);
					tInputState = INPUT_STATE_RUN_OPTION_CALLBACK;
				}
				else
					PlaySE(SE_FAILURE);
				break;
			case 2: // Close start menu
			    PlaySE(SE_WIN_OPEN);
			    CloseStartMenu();
				DestroyTask(taskId);
				break;
		}
        break;
	case INPUT_STATE_RUN_OPTION_CALLBACK:
		switch (sStartMenuOptions[sStartMenu.selectedIndex].func.u8_void())
		{
			case CALLBACK_STATE_WAIT_FADE_OUT:
			    break;
			case CALLBACK_STATE_CLOSE_MENU:
			    tInputState = INPUT_STATE_CLOSE_MENU;
				break;
			case CALLBACK_STATE_SAVE:
				tInputState = INPUT_STATE_PREPARE_FOR_SAVE;
				break;
		}
		break;
	case INPUT_STATE_PREPARE_FOR_SAVE:
		StartMenu_PrepareForSave();
		tInputState = INPUT_STATE_RUN_SAVE_CALLBACK;
		break;
	case INPUT_STATE_RUN_SAVE_CALLBACK:
		switch (RunSaveDialogCB())
		{
			case SAVECB_RETURN_CONTINUE:
				break;
			case SAVECB_RETURN_OKAY:
			case SAVECB_RETURN_ERROR:
				ClearDialogWindowAndFrameToTransparent(0, TRUE);
				ClearPlayerHeldMovementAndUnfreezeObjectEvents();
				ScriptContext2_Disable();
				tInputState = INPUT_STATE_CLOSE_MENU;
				break;
			case SAVECB_RETURN_CANCEL:
				ClearDialogWindowAndFrameToTransparent(0, FALSE);
				DrawStartMenuInOneGo();
				tInputState = INPUT_STATE_SET_IN_UNION_ROOM;
				break;
		}
		break;
	case INPUT_STATE_CLOSE_MENU:
		FREE_IF_NOT_NULL(sStartMenu.itemsList);
		DestroyTask(taskId);
		break;
    }
}

static void StartMenu_MoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list)
{
	if (!onInit)
		PlaySE(SE_SELECT);
	
	sStartMenu.selectedIndex = itemIndex;
}

static bool8 StartMenu_CanStartSelectedIndexAction(u8 index)
{
	switch (index)
	{
		case START_MENU_POKEDEX:
			if (!GetNationalPokedexCount(FLAG_GET_SEEN))
				return FALSE;
			break;
		case START_MENU_RIDE_PAGER:
		    if (!CountObtainedPokeRides())
				return FALSE;
			break;
	}
	return TRUE;
}

static void StartMenu_FadeScreenIfLeavingOverworld(u8 index)
{
    if (index != START_MENU_SAVE && index != START_MENU_EXIT && index != START_MENU_RETIRE && index != START_MENU_RIDE_PAGER)
    {
        StopPokemonLeagueLightingEffectTask();
        FadeScreen(FADE_TO_BLACK, 0);
    }
}

#undef tInputState

///////////////
// CALLBACKS //
///////////////

static u8 StartMenuDexnavCallback(void)
{
	if (!gPaletteFade.active)
	{
		PlayRainStoppingSoundEffect();
		RemoveClockBox();
		CleanupOverworldWindowsAndTilemaps();
		DexNavGuiInit(CB2_ReturnToFieldWithOpenMenu);
		return CALLBACK_STATE_CLOSE_MENU;
	}
	return CALLBACK_STATE_WAIT_FADE_OUT;
}

static u8 StartMenuPokedexCallback(void)
{
	if (!gPaletteFade.active)
    {
        IncrementGameStat(GAME_STAT_CHECKED_POKEDEX);
        PlayRainStoppingSoundEffect();
        RemoveClockBox();
        CleanupOverworldWindowsAndTilemaps();
        SetMainCallback2(CB2_OpenPokedexFromStartMenu);
        return CALLBACK_STATE_CLOSE_MENU;
    }
    return CALLBACK_STATE_WAIT_FADE_OUT;
}

static u8 StartMenuPokemonCallback(void)
{
	if (!gPaletteFade.active)
    {
        PlayRainStoppingSoundEffect();
        RemoveClockBox();
        CleanupOverworldWindowsAndTilemaps();
        SetMainCallback2(CB2_PartyMenuFromStartMenu);
        return CALLBACK_STATE_CLOSE_MENU;
    }
    return CALLBACK_STATE_WAIT_FADE_OUT;
}

static u8 StartMenuBagCallback(void)
{
	if (!gPaletteFade.active)
    {
        PlayRainStoppingSoundEffect();
        RemoveClockBox();
        CleanupOverworldWindowsAndTilemaps();
        SetMainCallback2(CB2_BagMenuFromStartMenu);
        return CALLBACK_STATE_CLOSE_MENU;
    }
    return CALLBACK_STATE_WAIT_FADE_OUT;
}

static u8 StartMenuPlayerCallback(void)
{
    if (!gPaletteFade.active)
    {
#if MON_ICON_ON_TRAINER_CARD
		gSpecialVar_0x8004 = MON_ICON_TINT_NORMAL;
	
		UpdateTrainerCardPhotoIcons();
#endif
        PlayRainStoppingSoundEffect();
        RemoveClockBox();
        CleanupOverworldWindowsAndTilemaps();
        ShowPlayerTrainerCard(CB2_ReturnToFieldWithOpenMenu);
        return CALLBACK_STATE_CLOSE_MENU;
    }
    return CALLBACK_STATE_WAIT_FADE_OUT;
}

static u8 StartMenuPlayerLinkCallback(void)
{
	if (!gPaletteFade.active)
    {
        PlayRainStoppingSoundEffect();
		RemoveClockBox();
        CleanupOverworldWindowsAndTilemaps();
        ShowTrainerCardInLink(gLocalLinkPlayerId, CB2_ReturnToFieldWithOpenMenu);
        return CALLBACK_STATE_CLOSE_MENU;
    }
    return CALLBACK_STATE_WAIT_FADE_OUT;
}

static u8 StartMenuSaveCallback(void)
{
	RemoveClockBox();
    RemoveStartMenuWindow();
	return CALLBACK_STATE_SAVE;
}

static u8 StartMenuOptionCallback(void)
{
	if (!gPaletteFade.active)
    {
        PlayRainStoppingSoundEffect();
        RemoveClockBox();
        CleanupOverworldWindowsAndTilemaps();
        SetMainCallback2(CB2_InitOptionsMenu);
        gMain.savedCallback = CB2_ReturnToFieldWithOpenMenu;
        return CALLBACK_STATE_CLOSE_MENU;
    }
    return CALLBACK_STATE_WAIT_FADE_OUT;
}

static u8 StartMenuExitCallback(void)
{
    CloseStartMenu();
	return CALLBACK_STATE_CLOSE_MENU;
}

static u8 StartMenuRetireCallback(void)
{
	CloseStartMenu();
    SafariZoneRetirePrompt();
    return CALLBACK_STATE_CLOSE_MENU;
}

static u8 StartMenuRidePagerCallback(void)
{
	CloseStartMenu();
	InitRidePager();
	return CALLBACK_STATE_CLOSE_MENU;
}

///////////
// CLOCK //
///////////

static inline bool8 CanShowCurrentLevelCapOnClockBox(u8 levelCap)
{
	if (levelCap != MAX_LEVEL && !MenuHelpers_LinkSomething() && !InUnionRoom() && FlagGet(FLAG_SYS_POKEMON_GET))
		return TRUE;
	
	return FALSE;
}

#define tTimer            data[0]
#define tShowSecondsColon data[1]
#define tWindowId         data[2]

static void Task_UpdateTimeInClockBox(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	
	StringCopy(gStringVar4, gStartMenu_TimeBoxClock);
	ConvertIntToDecimalStringN(gStringVar1, gRtcLocation.hour, STR_CONV_MODE_LEADING_ZEROS, 2);
	StringAppend(gStringVar4, gStringVar1);
	StringAppend(gStringVar4, gText_Font2Colon);
	ConvertIntToDecimalStringN(gStringVar1, gRtcLocation.minute, STR_CONV_MODE_LEADING_ZEROS, 2);
	StringAppend(gStringVar4, gStringVar1);
	StringAppend(gStringVar4, tShowSecondsColon ? gText_Font2Colon : gText_Space);
	ConvertIntToDecimalStringN(gStringVar1, gRtcLocation.second, STR_CONV_MODE_LEADING_ZEROS, 2);
	StringAppend(gStringVar4, gStringVar1);
	AddTextPrinterParameterized(tWindowId, 0, gStringVar4, 4, 0, 0xFF, NULL);
	CopyWindowToVram(tWindowId, COPYWIN_GFX);
	
	if (++tTimer >= 30) // Show colon every half second
	{
		tTimer = 0; // Reset Timer for next update
		tShowSecondsColon ^= TRUE; // Invert it. If the current Timer show it, the next don't
	}
}

static void DrawClockBox(void)
{
	u8 windowId, height, levelCap = GetCurrentLevelCapLevel();
	bool8 inSafari = GetSafariZoneFlag(), canShowLevelCap = CanShowCurrentLevelCapOnClockBox(levelCap);
	struct WindowTemplate template;
	
	// Create task
	sStartMenu.clockTaskId = CreateTask(Task_UpdateTimeInClockBox, 90);
	gTasks[sStartMenu.clockTaskId].tShowSecondsColon = TRUE;
	
	// Create window
	if (inSafari)
		height = 5;
	else if (canShowLevelCap)
		height = 3;
	else
		height = 2;
	
	template = SetWindowTemplateFields(0, 1, 1, 10, height, 15, 0x1CE);
    gTasks[sStartMenu.clockTaskId].tWindowId = windowId = AddWindow(&template);
	PutWindowTilemap(windowId);
    DrawStdWindowFrame(windowId, FALSE);
	
    if (inSafari) // Display safari texts
    {
        ConvertIntToDecimalStringN(gStringVar1, gSafariZoneStepCounter, STR_CONV_MODE_LEFT_ALIGN, 3);
        ConvertIntToDecimalStringN(gStringVar2, MAX_SAFARI_STEPS, STR_CONV_MODE_LEFT_ALIGN, 3);
        ConvertIntToDecimalStringN(gStringVar3, gNumSafariBalls, STR_CONV_MODE_LEFT_ALIGN, 2);
        StringExpandPlaceholders(gStringVar4, gUnknown_84162A9);
        AddTextPrinterParameterized(windowId, 0, gStringVar4, 4, 12, 0xFF, NULL);
    }
	else if (canShowLevelCap) // Display level cap
	{
		ConvertIntToDecimalStringN(gStringVar1, levelCap, STR_CONV_MODE_LEFT_ALIGN, 3);
		StringExpandPlaceholders(gStringVar4, gText_CurrentLevelCap);
		AddTextPrinterParameterized(windowId, 0, gStringVar4, 4, 12, 0xFF, NULL);
	}
	CopyWindowToVram(windowId, COPYWIN_GFX);
}

static void RemoveClockBox(void)
{
	s16 *data = gTasks[sStartMenu.clockTaskId].data;
	ClearStdWindowAndFrameToTransparent(tWindowId, FALSE);
	CopyWindowToVram(tWindowId, COPYWIN_GFX);
	RemoveWindow(tWindowId);
	DestroyTask(sStartMenu.clockTaskId);
}

#undef tTimer
#undef tShowSecondsColon
#undef tWindowId

//////////
// SAVE //
//////////

static void SaveStatToString(u8 caseId, u8 *dest, u8 color)
{
	*dest++ = EXT_CTRL_CODE_BEGIN;
    *dest++ = EXT_CTRL_CODE_COLOR;
    *dest++ = color;
    *dest++ = EXT_CTRL_CODE_BEGIN;
    *dest++ = EXT_CTRL_CODE_SHADOW;
    *dest++ = color + 1;
	
	switch (caseId)
	{
		case SAVE_STAT_LOCATION:
			GetMapName(dest, GetCurrentRegionMapSectionId());
			break;
		case SAVE_STAT_NAME:
			StringCopy(dest, gSaveBlock2Ptr->playerName);
			break;
		case SAVE_STAT_BADGES:
			ConvertIntToDecimalStringN(dest, GetNumOfBadges(), STR_CONV_MODE_LEFT_ALIGN, 2);
			break;
		case SAVE_STAT_SEEN:
			ConvertIntToDecimalStringN(dest, GetNationalPokedexCount(FLAG_GET_SEEN), STR_CONV_MODE_LEFT_ALIGN, 4);
			break;
		case SAVE_STAT_CAUGHT:
			ConvertIntToDecimalStringN(dest, GetNationalPokedexCount(FLAG_GET_CAUGHT), STR_CONV_MODE_LEFT_ALIGN, 4);
			break;
		case SAVE_STAT_DATE:
			dest = ConvertIntToDecimalStringN(dest, gRtcLocation.day, STR_CONV_MODE_LEADING_ZEROS, 2);
			*dest++ = CHAR_SLASH;
			dest = ConvertIntToDecimalStringN(dest, gRtcLocation.month, STR_CONV_MODE_LEADING_ZEROS, 2);
			*dest++ = CHAR_SLASH;
			ConvertIntToDecimalStringN(dest, gRtcLocation.year, STR_CONV_MODE_LEFT_ALIGN, 4);
			break;
	}
}

static void PrintSaveStats(void)
{
    u8 x, y, windowId;
	
	sStartMenu.saveStatsWindowId = windowId = AddWindow(&sSaveStatsWindowTemplate);
	TextWindow_SetStdFrame0_WithPal(windowId, 0x21D, 0xD0);
	DrawStdFrameWithCustomTileAndPalette(windowId, FALSE, 0x21D, 0x0D);
	
	// Print Location
	SaveStatToString(SAVE_STAT_LOCATION, gStringVar4, 8);
    x = (u32)(112 - GetStringWidth(2, gStringVar4, -1)) / 2;
    AddTextPrinterParameterized3(windowId, 2, x, 0, sTextColor_LocationHeader, -1, gStringVar4);
	
	// Print name
    AddTextPrinterParameterized3(windowId, 0, 2, 14, sTextColor_StatName, -1, gSaveStatName_Player);
    SaveStatToString(SAVE_STAT_NAME, gStringVar4, 2);
    Menu_PrintFormatIntlPlayerName(windowId, gStringVar4, 60, 14);
	
	// Print badges
    AddTextPrinterParameterized3(windowId, 0, 2, 28, sTextColor_StatName, -1, gSaveStatName_Badges);
    SaveStatToString(SAVE_STAT_BADGES, gStringVar4, 2);
    AddTextPrinterParameterized3(windowId, 0, 60, 28, sTextColor_StatValue, -1, gStringVar4);
	
	// Print pokedex
    if (FlagGet(FLAG_SYS_POKEDEX_GET))
    {
		// Seen
        AddTextPrinterParameterized3(windowId, 0, 2, 42, sTextColor_StatName, -1, gSaveStatName_Seen);
        SaveStatToString(SAVE_STAT_SEEN, gStringVar4, 2);
        AddTextPrinterParameterized3(windowId, 0, 60, 42, sTextColor_StatValue, -1, gStringVar4);
		
		// Caught
		AddTextPrinterParameterized3(windowId, 0, 2, 56, sTextColor_StatName, -1, gSaveStatName_Caught);
        SaveStatToString(SAVE_STAT_CAUGHT, gStringVar4, 2);
        AddTextPrinterParameterized3(windowId, 0, 60, 56, sTextColor_StatValue, -1, gStringVar4);
        
		y = 70;
    }
	else
		y = 42;
	
	// Print date
    AddTextPrinterParameterized3(windowId, 0, 2, y, sTextColor_StatName, -1, gSaveStatName_Date);
    SaveStatToString(SAVE_STAT_DATE, gStringVar4, 2);
    AddTextPrinterParameterized3(windowId, 0, 60, y, sTextColor_StatValue, -1, gStringVar4);
    
	CopyWindowToVram(windowId, COPYWIN_GFX);
}

static void PrintSaveTextWithFollowupFunc(const u8 *str, u8 (*saveDialogCB)(void))
{
    StringExpandPlaceholders(gStringVar4, str);
    LoadMessageBoxAndFrameGfx(0, TRUE);
    AddTextPrinterForMessage(TRUE);
    sStartMenu.saveDialogIsPrinting = TRUE;
    sStartMenu.saveDialogCB = saveDialogCB;
}

static void CloseSaveStatsWindow(void)
{
    ClearStdWindowAndFrame(sStartMenu.saveStatsWindowId, TRUE);
    RemoveWindow(sStartMenu.saveStatsWindowId);
}

static void CloseSaveMessageWindow(void)
{
    ClearDialogWindowAndFrame(0, TRUE);
}

static void StartMenu_PrepareForSave(void)
{
    SaveMapView();
	sStartMenu.saveDialogCB = SaveDialogCB_PrintAskSaveText;
	sStartMenu.saveDialogIsPrinting = FALSE;
}

static u8 RunSaveDialogCB(void)
{
	if (RunTextPrinters_CheckPrinter0Active())
        return SAVECB_RETURN_CONTINUE;
	else
	{
		sStartMenu.saveDialogIsPrinting = FALSE;
		return sStartMenu.saveDialogCB();
	}
}

static u8 SaveDialogCB_PrintAskSaveText(void)
{
    PrintSaveStats();
    PrintSaveTextWithFollowupFunc(gText_WouldYouLikeToSaveTheGame, SaveDialogCB_AskSavePrintYesNoMenu);
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_AskSavePrintYesNoMenu(void)
{
    DisplayYesNoMenuDefaultYes();
    sStartMenu.saveDialogCB = SaveDialogCB_AskSaveHandleInput;
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_AskSaveHandleInput(void)
{
	switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0:
        if ((gSaveFileStatus != SAVE_STATUS_EMPTY && gSaveFileStatus != SAVE_STATUS_INVALID) || !gDifferentSaveFile)
            sStartMenu.saveDialogCB = SaveDialogCB_PrintAskOverwriteText;
        else
            sStartMenu.saveDialogCB = SaveDialogCB_PrintSavingDontTurnOffPower;
        break;
    case 1:
    case -1:
        CloseSaveStatsWindow();
        CloseSaveMessageWindow();
        return SAVECB_RETURN_CANCEL;
    }
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_PrintAskOverwriteText(void)
{
	if (gDifferentSaveFile)
        PrintSaveTextWithFollowupFunc(gText_DifferentGameFile, SaveDialogCB_AskReplacePreviousFilePrintYesNoMenu);
    else
        PrintSaveTextWithFollowupFunc(gText_AlreadySaveFile_WouldLikeToOverwrite, SaveDialogCB_AskOverwritePrintYesNoMenu);
	
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_AskReplacePreviousFilePrintYesNoMenu(void)
{
	DisplayYesNoMenuDefaultNo();
    sStartMenu.saveDialogCB = SaveDialogCB_AskOverwriteOrReplacePreviousFileHandleInput;
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_AskOverwritePrintYesNoMenu(void)
{
	DisplayYesNoMenuDefaultYes();
    sStartMenu.saveDialogCB = SaveDialogCB_AskOverwriteOrReplacePreviousFileHandleInput;
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_AskOverwriteOrReplacePreviousFileHandleInput(void)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0:
        sStartMenu.saveDialogCB = SaveDialogCB_PrintSavingDontTurnOffPower;
        break;
    case 1:
    case -1:
        CloseSaveStatsWindow();
        CloseSaveMessageWindow();
        return SAVECB_RETURN_CANCEL;
    }
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_PrintSavingDontTurnOffPower(void)
{
	PrintSaveTextWithFollowupFunc(gText_SavingDontTurnOffThePower, SaveDialogCB_DoSave);
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_DoSave(void)
{
	IncrementGameStat(GAME_STAT_SAVED_GAME);
	
    if (gDifferentSaveFile)
    {
        TrySavingData(SAVE_OVERWRITE_DIFFERENT_FILE);
        gDifferentSaveFile = FALSE;
    }
    else
        TrySavingData(SAVE_NORMAL);
	
    sStartMenu.saveDialogCB = SaveDialogCB_PrintSaveResult;
	
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_PrintSaveResult(void)
{
	if (gSaveSucceeded)
        PrintSaveTextWithFollowupFunc(gText_PlayerSavedTheGame, SaveDialogCB_WaitPrintSuccessAndPlaySE);
    else
        PrintSaveTextWithFollowupFunc(gText_SaveError_PleaseExchangeBackupMemory, SaveDialogCB_WaitPrintErrorAndPlaySE);
	
    sStartMenu.saveDialogDelay = 60;
	
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_WaitPrintSuccessAndPlaySE(void)
{
	if (!RunTextPrinters_CheckPrinter0Active())
    {
        PlaySE(SE_SAVE);
        sStartMenu.saveDialogCB = SaveDialogCB_ReturnSuccess;
    }
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_WaitPrintErrorAndPlaySE(void)
{
    if (!RunTextPrinters_CheckPrinter0Active())
    {
        PlaySE(SE_BOO);
        sStartMenu.saveDialogCB = SaveDialogCB_ReturnError;
    }
    return SAVECB_RETURN_CONTINUE;
}

static bool8 SaveDialog_Wait60FramesOrAButtonHeld(void)
{
    sStartMenu.saveDialogDelay--;
	
    if (JOY_HELD(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        return TRUE;
    }
    else if (!sStartMenu.saveDialogDelay)
        return TRUE;
    else
        return FALSE;
}

static u8 SaveDialogCB_ReturnSuccess(void)
{
    if (!IsSEPlaying() && SaveDialog_Wait60FramesOrAButtonHeld())
    {
        CloseSaveStatsWindow();
        return SAVECB_RETURN_OKAY;
    }
    return SAVECB_RETURN_CONTINUE;
}

static bool8 SaveDialog_Wait60FramesThenCheckAButtonHeld(void)
{
    if (!sStartMenu.saveDialogDelay)
    {
        if (JOY_HELD(A_BUTTON))
            return TRUE;
        else
            return FALSE;
    }
    else
    {
        sStartMenu.saveDialogDelay--;
        return FALSE;
    }
}

static u8 SaveDialogCB_ReturnError(void)
{
    if (!SaveDialog_Wait60FramesThenCheckAButtonHeld())
        return SAVECB_RETURN_CONTINUE;
	else
	{
		CloseSaveStatsWindow();
		return SAVECB_RETURN_ERROR;
	}
}

static void Task_SaveGame(u8 taskId)
{
	switch (RunSaveDialogCB())
    {
    case SAVECB_RETURN_CONTINUE:
        return;
    case SAVECB_RETURN_CANCEL:
    case SAVECB_RETURN_ERROR:
        gSpecialVar_Result = FALSE;
        break;
    case SAVECB_RETURN_OKAY:
        gSpecialVar_Result = TRUE;
        break;
    }
    DestroyTask(taskId);
    EnableBothScriptContexts();
}

void Field_AskSaveTheGame(void)
{
    StartMenu_PrepareForSave();
    CreateTask(Task_SaveGame, 80);
}

static void VBlankCB_WhileSavingAfterLinkBattle(void)
{
    TransferPlttBuffer();
}

static bool8 DoSetUpSaveAfterLinkBattle(u8 *state)
{
    switch (*state)
    {
    case 0:
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        SetVBlankCallback(NULL);
        ScanlineEffect_Stop();
        DmaFill16Defvars(3, 0, (void *)PLTT, PLTT_SIZE);
        DmaFillLarge16(3, 0, (void *)VRAM, VRAM_SIZE, 0x1000);
        break;
    case 1:
        ResetSpriteData();
        ResetTasks();
        ResetPaletteFade();
        ScanlineEffect_Clear();
        break;
    case 2:
        ResetBgsAndClearDma3BusyFlags(FALSE);
        InitBgsFromTemplates(0, sBGTemplates_AfterLinkSaveMessage, ARRAY_COUNT(sBGTemplates_AfterLinkSaveMessage));
        InitWindows(sWindowTemplates_AfterLinkSaveMessage);
        TextWindow_SetStdFrame0_WithPal(0, 0x008, 0xF0);
        break;
    case 3:
        ShowBg(0);
        BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
        SetVBlankCallback(VBlankCB_WhileSavingAfterLinkBattle);
        EnableInterrupts(INTR_FLAG_VBLANK);
        break;
    case 4:
        return TRUE;
    }
    (*state)++;
    return FALSE;
}

#define tState data[0]

static void Task_SaveGameAfterLinkBattle(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	
    if (!gPaletteFade.active)
    {
        switch (tState)
        {
        case LINK_SAVE_STATE_PRINT_SAVING_TEXT:
            FillWindowPixelBuffer(0, PIXEL_FILL(1));
            AddTextPrinterParameterized2(0, 2, gText_SavingDontTurnOffThePower2, 0xFF, NULL, 2, 1, 3);
            DrawTextBorderOuter(0, 0x008, 0x0F);
			
            PutWindowTilemap(0);
            CopyWindowToVram(0, COPYWIN_BOTH);
			
            BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
			
            if (gWirelessCommType && InUnionRoom())
                tState = LINK_SAVE_STATE_CREATE_LINK_SAVE_TASK;
            else
                tState = LINK_SAVE_STATE_WARP_PLAYER;
            break;
        case LINK_SAVE_STATE_WARP_PLAYER:
            SetContinueGameWarpStatusToDynamicWarp();
            WriteSaveBlock2();
            tState = LINK_SAVE_STATE_WAIT_WRITE_SAVEBLOCK1;
            break;
        case LINK_SAVE_STATE_WAIT_WRITE_SAVEBLOCK1:
            if (WriteSaveBlock1Sector())
            {
                ClearContinueGameWarpStatus2();
                tState = LINK_SAVE_STATE_START_FADE_OUT;
            }
            break;
        case LINK_SAVE_STATE_START_FADE_OUT:
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            tState = LINK_SAVE_STATE_RETURN_TO_FIELD;
            break;
        case LINK_SAVE_STATE_CREATE_LINK_SAVE_TASK:
            CreateTask(Task_LinkSave, 5);
            tState = LINK_SAVE_STATE_WAIT_LINK_SAVE_TASK;
            break;
        case LINK_SAVE_STATE_WAIT_LINK_SAVE_TASK:
            if (!FuncIsActiveTask(Task_LinkSave))
                tState = LINK_SAVE_STATE_START_FADE_OUT;
            break;
		case LINK_SAVE_STATE_RETURN_TO_FIELD:
            FreeAllWindowBuffers();
            SetMainCallback2(gMain.savedCallback);
            DestroyTask(taskId);
            break;
        }
    }
}

static void CB2_WhileSavingAfterLinkBattle(void)
{
    RunTasks();
    UpdatePaletteFade();
}

void CB2_SetUpSaveAfterLinkBattle(void)
{
    if (DoSetUpSaveAfterLinkBattle(&gMain.state))
    {
		CreateTask(Task_SaveGameAfterLinkBattle, 80);
        SetMainCallback2(CB2_WhileSavingAfterLinkBattle);
    }
}

#undef tState

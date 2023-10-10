#include "global.h"
#include "gflib.h"
#include "event_data.h"
#include "link.h"
#include "main_menu.h"
#include "menu.h"
#include "naming_screen.h"
#include "oak_speech.h"
#include "option_menu.h"
#include "overworld.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "save.h"
#include "scanline_effect.h"
#include "strings.h"
#include "task.h"
#include "text_window.h"
#include "text_window_graphics.h"
#include "title_screen.h"
#include "constants/songs.h"

// Main menu types
enum
{
	MAIN_MENU_NEWGAME,
	MAIN_MENU_CONTINUE,
	MAIN_MENU_ERROR,
	MAIN_MENU_COUNT
};

// Window Ids
enum
{
	// New Game windows
	NEWGAME_WINDOW_NEWGAME,
	NEWGAME_WINDOW_OPTIONS,
	
	// Continue windows
	CONTINUE_WINDOW_CONTINUE,
	CONTINUE_WINDOW_NEWGAME,
	CONTINUE_WINDOW_OPTIONS,
	CONTINUE_WINDOW_MYSTERYGIFT,
	
	// Error window
	ERROR_WINDOW_MESSAGE,
	
	MAIN_MENU_WINDOWS_COUNT
};

// Main menu actions
enum
{
	MAIN_MENU_ACTION_NEWGAME,
	MAIN_MENU_ACTION_CONTINUE,
	MAIN_MENU_ACTION_OPTIONS,
	MAIN_MENU_ACTION_MYSTERYGIFT,
};

struct MainMenuBgScroll
{
	u8 menuType;
	u8 cursorPos; // starts from id 0
	bool8 scrollDown;
	u8 endPosition;
};

static void Task_SetWin0BldRegsAndCheckSaveFile(u8 taskId);
static void Task_SetWin0BldRegsAndInitMainMenu(u8 taskId);
static void Task_WaitFadeAndPrintMainMenuText(u8 taskId);
static void Task_PrintMainMenuText(u8 taskId);
static void Task_WaitDma3AndFadeIn(u8 taskId);
static void Task_UpdateVisualSelection(u8 taskId);
static void Task_HandleMenuInput(u8 taskId);
static void Task_ExecuteMainMenuSelection(u8 taskId);
static void Task_ReturnToTileScreen(u8 taskId);
static void Task_SaveErrorStatus_RunPrinterThenWaitButton(u8 taskId);
static void PrintSaveErrorStatus(u8 taskId, const u8 *str);
static void LoadUserFrameToBg0(void);
static void SetStdFrame0OnBg0(void);
static void MainMenu_DrawWindow(const struct WindowTemplate * windowTemplate);
static void MainMenu_EraseErrorWindow(void);
static bool8 HandleMenuInput(u8 taskId);
static void PrintContinueStats(void);
static void MoveWindowByMenuTypeAndCursorPos(u8 menuType, u8 cursorPos);

static const u16 sBgPal00[] = INCBIN_U16("graphics/main_menu/unk_8234648.gbapal");
static const u16 sBgPal15[] = INCBIN_U16("graphics/main_menu/unk_8234668.gbapal");

static const u8 sTextColor1[] = {10, 11, 12};
static const u8 sTextColor2[] = {10, 1, 12};

static const struct BgTemplate sMainMenuBgTemplate[] =
{
    { // Text
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 30,
        .priority = 1,
		.screenSize = 2,
    },
	{ // Sprites
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 28,
        .priority = 0,
		.screenSize = 2,
    },
};

// Data for bg scroll
static const struct MainMenuBgScroll sMainMenuBgScrollData[] =
{
	{
		.menuType = MAIN_MENU_CONTINUE, // In menu continue
		.cursorPos = 1, // On selection 2
		.scrollDown = TRUE, // Press down
		.endPosition = 152, // Go to position
	},
	{
		.menuType = MAIN_MENU_CONTINUE, // In menu continue
		.cursorPos = 2, // On selection 3
		.scrollDown = FALSE, // Press up
		.endPosition = 0, // Go to position
	},
};

static const struct WindowTemplate sMainMenuWinTemplates[MAIN_MENU_WINDOWS_COUNT + 1] =
{
	// Windows for MAIN_MENU_NEWGAME
	// New Game
	[NEWGAME_WINDOW_NEWGAME] = {
		.bg = 0,
		.tilemapLeft = 3,
		.tilemapTop = 1,
		.width = 24,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 0x001
	},
	// Options
	[NEWGAME_WINDOW_OPTIONS] = {
		.bg = 0,
		.tilemapLeft = 3,
		.tilemapTop = 5,
		.width = 24,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 0x031
	},
	
	// Windows for MAIN_MENU_CONTINUE
	// Continue
	[CONTINUE_WINDOW_CONTINUE] = {
		.bg = 0,
		.tilemapLeft = 3,
		.tilemapTop = 1,
		.width = 24,
		.height = 13,
		.paletteNum = 15,
		.baseBlock = 0x001
	},
	// New Game
	[CONTINUE_WINDOW_NEWGAME] = {
		.bg = 0,
		.tilemapLeft = 3,
		.tilemapTop = 16,
		.width = 24,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 0x139
	},
	// Options
	[CONTINUE_WINDOW_OPTIONS] = {
		.bg = 0,
		.tilemapLeft = 3,
		.tilemapTop = 20,
		.width = 24,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 0x169
	},
	// Mystery Gift
	[CONTINUE_WINDOW_MYSTERYGIFT] = {
		.bg = 0,
		.tilemapLeft = 3,
		.tilemapTop = 24,
		.width = 24,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 0x199
	},
	
	// Windows for MAIN_MENU_ERROR
	// Message
	[ERROR_WINDOW_MESSAGE] = {
		.bg = 0,
		.tilemapLeft = 3,
		.tilemapTop = 15,
		.width = 24,
		.height = 4,
		.paletteNum = 15,
		.baseBlock = 0x001
	},

	[MAIN_MENU_WINDOWS_COUNT] = DUMMY_WIN_TEMPLATE
};

static void CB2_MainMenu(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB_MainMenu(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

#define tCursorPos       data[0]
#define tNumOptions      data[1]
#define tMenuType        data[2]
#define tScrollDown      data[3]
#define tEndPosition     data[4]

void CB2_InitMainMenu(void)
{
	SetVBlankCallback(NULL);
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_BG2CNT, 0);
    SetGpuReg(REG_OFFSET_BG1CNT, 0);
    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    SetGpuReg(REG_OFFSET_BG2HOFS, 0);
    SetGpuReg(REG_OFFSET_BG2VOFS, 0);
    SetGpuReg(REG_OFFSET_BG1HOFS, 0);
    SetGpuReg(REG_OFFSET_BG1VOFS, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    DmaFill16(3, 0, (void *)VRAM, VRAM_SIZE);
    DmaFill32(3, 0, (void *)OAM, OAM_SIZE);
    DmaFill16(3, 0, (void *)(PLTT + 2), PLTT_SIZE - 2);
    ScanlineEffect_Stop();
    ResetTasks();
    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetPaletteFade();
    ResetBgsAndClearDma3BusyFlags(FALSE);
    InitBgsFromTemplates(0, sMainMenuBgTemplate, ARRAY_COUNT(sMainMenuBgTemplate));
	ChangeBgX(0, 0, 0);
    ChangeBgY(0, 0, 0);
    ChangeBgX(1, 0, 0);
    ChangeBgY(1, 0, 0);
    ChangeBgX(2, 0, 0);
    ChangeBgY(2, 0, 0);
	InitWindows(sMainMenuWinTemplates);
	DeactivateAllTextPrinters();
	LoadPalette(sBgPal00, 0x00, 0x20);
	LoadPalette(sBgPal15, 0xF0, 0x20);
	SetGpuReg(REG_OFFSET_WIN0H, 0);
    SetGpuReg(REG_OFFSET_WIN0V, 0);
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
	SetMainCallback2(CB2_MainMenu);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
	gTasks[CreateTask(Task_SetWin0BldRegsAndCheckSaveFile, 0)].tCursorPos = 0;
}

/*
 * The entire screen is darkened slightly except at WIN0 to indicate
 * the player cursor position.
 */
static void MainMenu_SetWin0BldRegs(void)
{
	SetGpuReg(REG_OFFSET_WIN0H, 0);
	SetGpuReg(REG_OFFSET_WIN0V, 0);
	SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_OBJ | WININ_WIN0_BG0 | WININ_WIN0_BG1);
	SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_CLR | WINOUT_WIN01_OBJ | WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1);
	SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ | BLDCNT_TGT1_BD | BLDCNT_EFFECT_DARKEN);
	SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 0));
	SetGpuReg(REG_OFFSET_BLDY, 7);
}

static void Task_SetWin0BldRegsAndCheckSaveFile(u8 taskId)
{
	if (!gPaletteFade.active)
	{
		MainMenu_SetWin0BldRegs();
		
		switch (gSaveFileStatus)
        {
		default: // Save empty
            LoadUserFrameToBg0();
            gTasks[taskId].tMenuType = MAIN_MENU_NEWGAME;
            gTasks[taskId].func = Task_SetWin0BldRegsAndInitMainMenu;
            break;
        case SAVE_STATUS_OK:
            LoadUserFrameToBg0();
			gTasks[taskId].tMenuType = MAIN_MENU_CONTINUE;
            gTasks[taskId].func = Task_SetWin0BldRegsAndInitMainMenu;
            break;
        case SAVE_STATUS_INVALID:
            SetStdFrame0OnBg0();
            gTasks[taskId].tMenuType = MAIN_MENU_NEWGAME;
            PrintSaveErrorStatus(taskId, gText_SaveFileHasBeenDeleted);
            break;
        case SAVE_STATUS_ERROR:
            SetStdFrame0OnBg0();
            gTasks[taskId].tMenuType = MAIN_MENU_CONTINUE;
            PrintSaveErrorStatus(taskId, gText_SaveFileCorruptedPrevWillBeLoaded);   
            break;
        case SAVE_STATUS_NO_FLASH:
            SetStdFrame0OnBg0();
            gTasks[taskId].tMenuType = MAIN_MENU_NEWGAME;
            PrintSaveErrorStatus(taskId, gText_1MSubCircuitBoardNotInstalled);
            break;
		}
	}
}

static void Task_SetWin0BldRegsAndInitMainMenu(u8 taskId)
{
	if (!gPaletteFade.active)
	{
		MainMenu_SetWin0BldRegs();
		
#if AUTO_INIT_NEW_GAME
		if (gTasks[taskId].tMenuType == MAIN_MENU_NEWGAME)
			gTasks[taskId].func = Task_ExecuteMainMenuSelection;
		else
#endif
		gTasks[taskId].func = Task_WaitFadeAndPrintMainMenuText;
	}
}

static void Task_WaitFadeAndPrintMainMenuText(u8 taskId)
{
	if (!gPaletteFade.active)
		Task_PrintMainMenuText(taskId);
}

static void PrintMainMenuHeaderTextCentered(u8 windowId, const u8 *str)
{
	u32 x = 192 - GetStringWidth(2, str, -1);
	AddTextPrinterParameterized3(windowId, 2, x / 2, 2, sTextColor1, -1, str);
}

static void Task_PrintMainMenuText(u8 taskId)
{
	u16 pal = gSaveBlock2Ptr->playerGender == MALE ? RGB(4, 16, 31) : RGB(31, 3, 21);
	
	MainMenu_SetWin0BldRegs();
	LoadPalette(&pal, 0xF1, 2);
	
	switch (gTasks[taskId].tMenuType)
	{
		case MAIN_MENU_NEWGAME:
			// New Game
			FillWindowPixelBuffer(NEWGAME_WINDOW_NEWGAME, PIXEL_FILL(10));
			PrintMainMenuHeaderTextCentered(NEWGAME_WINDOW_NEWGAME, gText_NewGame);
			MainMenu_DrawWindow(&sMainMenuWinTemplates[NEWGAME_WINDOW_NEWGAME]);
			PutWindowTilemap(NEWGAME_WINDOW_NEWGAME);
			CopyWindowToVram(NEWGAME_WINDOW_NEWGAME, COPYWIN_GFX);
			// Option
			FillWindowPixelBuffer(NEWGAME_WINDOW_OPTIONS, PIXEL_FILL(10));
			PrintMainMenuHeaderTextCentered(NEWGAME_WINDOW_OPTIONS, gStartMenuText_Option);
			MainMenu_DrawWindow(&sMainMenuWinTemplates[NEWGAME_WINDOW_OPTIONS]);
			PutWindowTilemap(NEWGAME_WINDOW_OPTIONS);
			CopyWindowToVram(NEWGAME_WINDOW_OPTIONS, COPYWIN_BOTH);
			
			gTasks[taskId].tNumOptions = 2;
			break;
		case MAIN_MENU_CONTINUE:
			// Continue
			FillWindowPixelBuffer(CONTINUE_WINDOW_CONTINUE, PIXEL_FILL(10));
			PrintMainMenuHeaderTextCentered(CONTINUE_WINDOW_CONTINUE, gText_Continue);
			MainMenu_DrawWindow(&sMainMenuWinTemplates[CONTINUE_WINDOW_CONTINUE]);
			PrintContinueStats();
			PutWindowTilemap(CONTINUE_WINDOW_CONTINUE);
			CopyWindowToVram(CONTINUE_WINDOW_CONTINUE, COPYWIN_GFX);
			// New Game
			FillWindowPixelBuffer(CONTINUE_WINDOW_NEWGAME, PIXEL_FILL(10));
			PrintMainMenuHeaderTextCentered(CONTINUE_WINDOW_NEWGAME, gText_NewGame);
			MainMenu_DrawWindow(&sMainMenuWinTemplates[CONTINUE_WINDOW_NEWGAME]);
			PutWindowTilemap(CONTINUE_WINDOW_NEWGAME);
			CopyWindowToVram(CONTINUE_WINDOW_NEWGAME, COPYWIN_GFX);
			// Option
			FillWindowPixelBuffer(CONTINUE_WINDOW_OPTIONS, PIXEL_FILL(10));
			PrintMainMenuHeaderTextCentered(CONTINUE_WINDOW_OPTIONS, gStartMenuText_Option);
			MainMenu_DrawWindow(&sMainMenuWinTemplates[CONTINUE_WINDOW_OPTIONS]);
			PutWindowTilemap(CONTINUE_WINDOW_OPTIONS);
			CopyWindowToVram(CONTINUE_WINDOW_OPTIONS, COPYWIN_GFX);
			// Mystery Gift
			FillWindowPixelBuffer(CONTINUE_WINDOW_MYSTERYGIFT, PIXEL_FILL(10));
			PrintMainMenuHeaderTextCentered(CONTINUE_WINDOW_MYSTERYGIFT, gText_MysteryGift);
			MainMenu_DrawWindow(&sMainMenuWinTemplates[CONTINUE_WINDOW_MYSTERYGIFT]);
			PutWindowTilemap(CONTINUE_WINDOW_MYSTERYGIFT);
			CopyWindowToVram(CONTINUE_WINDOW_MYSTERYGIFT, COPYWIN_BOTH);
			
			gTasks[taskId].tNumOptions = 4;
			break;
	}
	gTasks[taskId].func = Task_WaitDma3AndFadeIn;
}

static void MainMenu_InitFadeToDisplayBgs(void)
{
	BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, 0xFFFF);
	ShowBg(0);
	ShowBg(1);
	SetVBlankCallback(VBlankCB_MainMenu);
}

static void Task_WaitDma3AndFadeIn(u8 taskId)
{
	if (WaitDma3Request(-1) != -1)
	{
		MainMenu_InitFadeToDisplayBgs();
		gTasks[taskId].func = Task_UpdateVisualSelection;
	}
}

static void Task_UpdateVisualSelection(u8 taskId)
{
	MoveWindowByMenuTypeAndCursorPos(gTasks[taskId].tMenuType, gTasks[taskId].tCursorPos);
	gTasks[taskId].func = Task_HandleMenuInput;
}

static void Task_HandleMenuInput(u8 taskId)
{
	if (!gPaletteFade.active && HandleMenuInput(taskId))
		gTasks[taskId].func = Task_UpdateVisualSelection;
}

static void MainMenu_ScrollBgs(u32 value, u8 op)
{
	ChangeBgY(0, value, op);
	ChangeBgY(1, value, op);
}

#define MAIN_MENU_BG_SCROLL_ADD 1500 // Value for bg scroll, how higher more fast

static void Task_MainMenuScrollBg(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	s32 endPos = tEndPosition << 8, bgPos = GetBgY(0); // Same for bg1
	
	if (tScrollDown)
	{
		if (bgPos + MAIN_MENU_BG_SCROLL_ADD > endPos)
		{
			MainMenu_ScrollBgs(endPos - bgPos, 1);
			gTasks[taskId].func = Task_UpdateVisualSelection;
		}
		else
			MainMenu_ScrollBgs(MAIN_MENU_BG_SCROLL_ADD, 1);
	}
	else
	{
		if (bgPos - MAIN_MENU_BG_SCROLL_ADD < endPos)
		{
			MainMenu_ScrollBgs(endPos == 0 ? bgPos : endPos - bgPos, 2);
			gTasks[taskId].func = Task_UpdateVisualSelection;
		}
		else
			MainMenu_ScrollBgs(MAIN_MENU_BG_SCROLL_ADD, 2);
	}
}

static bool8 MainMenu_CanScrollBg(u8 taskId, bool8 scrollDown)
{
	u8 i;
	s16 *data = gTasks[taskId].data;
	
	for (i = 0; i < ARRAY_COUNT(sMainMenuBgScrollData); i++)
	{
		if (sMainMenuBgScrollData[i].menuType == tMenuType && sMainMenuBgScrollData[i].cursorPos == tCursorPos && sMainMenuBgScrollData[i].scrollDown == scrollDown)
		{
			// Remove selection
			SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(0, 0));
			SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(0, 0));
			
			tScrollDown = scrollDown;
			tEndPosition = sMainMenuBgScrollData[i].endPosition;
			gTasks[taskId].func = Task_MainMenuScrollBg;
			return TRUE;
		}
	}
	return FALSE;
}

static bool8 HandleMenuInput(u8 taskId)
{
	if (JOY_NEW(A_BUTTON))
	{
		PlaySE(SE_SELECT);
        IsWirelessAdapterConnected(); // called for its side effects only
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_ExecuteMainMenuSelection;
	}
	else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(0, 240));
        SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(0, 160));
        gTasks[taskId].func = Task_ReturnToTileScreen;
    }
	else if (JOY_NEW(DPAD_UP))
	{
		if (MainMenu_CanScrollBg(taskId, FALSE))
			gTasks[taskId].tCursorPos--;
		else if (gTasks[taskId].tCursorPos > 0)
		{
			gTasks[taskId].tCursorPos--;
			return TRUE; // Update selection
		}
	}
	else if (JOY_NEW(DPAD_DOWN))
	{
		if (MainMenu_CanScrollBg(taskId, TRUE))
			gTasks[taskId].tCursorPos++;
		else if (gTasks[taskId].tCursorPos + 1 < gTasks[taskId].tNumOptions)
		{
			gTasks[taskId].tCursorPos++;
			return TRUE; // Update selection
		}
	}
	return FALSE;
}

// Update selection window
static void MoveWindowByMenuTypeAndCursorPos(u8 menuType, u8 cursorPos)
{
	u16 win0vTop, win0vBot;
	
	SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(18, 222));
	
	switch (menuType)
	{
		case MAIN_MENU_NEWGAME:
			switch (cursorPos)
			{
				case 0: // New Game
					win0vTop = 0x00 << 8;
					win0vBot = 0x20;
					break;
				case 1: // Options
					win0vTop = 0x20 << 8;
					win0vBot = 0x40;
					break;
			}
			break;
		case MAIN_MENU_CONTINUE:
			switch (cursorPos)
			{
				case 0: // Continue
					win0vTop = 0x00 << 8;
					win0vBot = 0x78;
					break;
				case 1: // New Game
					win0vTop = 0x78 << 8;
					win0vBot = 0x98;
					break;
				case 2: // Option
					win0vTop = 0x00 << 8;
					win0vBot = 0x20;
					break;
				case 3: // Mystery Gift
					win0vTop = 0x20 << 8;
					win0vBot = 0x40;
					break;
			}
			break;
	}
	SetGpuReg(REG_OFFSET_WIN0V, (win0vTop + (2 << 8)) | (win0vBot - 2));
}

static void Task_ExecuteMainMenuSelection(u8 taskId)
{
	u8 action, cursorPos = gTasks[taskId].tCursorPos;
	
	if (!gPaletteFade.active)
	{
		// Get action
		switch (gTasks[taskId].tMenuType)
		{
			case MAIN_MENU_NEWGAME:
				switch (cursorPos)
				{
					case 0:
						action = MAIN_MENU_ACTION_NEWGAME;
						break;
					case 1:
						action = MAIN_MENU_ACTION_OPTIONS;
						break;
				}
				break;
			case MAIN_MENU_CONTINUE:
				switch (cursorPos)
				{
					case 0:
						action = MAIN_MENU_ACTION_CONTINUE;
						break;
					case 1:
						action = MAIN_MENU_ACTION_NEWGAME;
						break;
					case 2:
						action = MAIN_MENU_ACTION_OPTIONS;
						break;
					case 3:
						action = MAIN_MENU_ACTION_MYSTERYGIFT;
						break;
				}
				break;
		}
		// Do action
		switch (action)
		{
			case MAIN_MENU_ACTION_NEWGAME:
				gUnknown_2031DE0 = 0;
				FreeAllWindowBuffers();
				DestroyTask(taskId);
				StartNewGameScene();
				break;
			case MAIN_MENU_ACTION_CONTINUE:
				gPlttBufferUnfaded[0] = RGB_BLACK;
				gPlttBufferFaded[0] = RGB_BLACK;
				gUnknown_2031DE0 = 0;
				FreeAllWindowBuffers();
				SetMainCallback2(CB2_ContinueSavedGame);
				DestroyTask(taskId);
				break;
			case MAIN_MENU_ACTION_OPTIONS:
				FreeAllWindowBuffers();
				SetMainCallback2(CB2_OptionsMenuFromStartMenu);
				gMain.savedCallback = CB2_InitMainMenu;
				DestroyTask(taskId);
				break;
			case MAIN_MENU_ACTION_MYSTERYGIFT:
				FreeAllWindowBuffers();
				DestroyTask(taskId);
				DoNamingScreen(NAMING_SCREEN_MYSTERY_GIFT, gStringVar1, 0, 0, CB2_InitTitleScreen);
				break;
		}
	}
}

static void Task_ReturnToTileScreen(u8 taskId)
{
	if (!gPaletteFade.active)
	{
		SetMainCallback2(CB2_InitTitleScreen);
        DestroyTask(taskId);
	}
}

static void PrintPlayerName(void)
{
    u8 i, name[PLAYER_NAME_LENGTH + 1], *ptr = name;

    AddTextPrinterParameterized3(CONTINUE_WINDOW_CONTINUE, 2, 2, 18, sTextColor2, -1, gText_Player);

    for (i = 0; i < PLAYER_NAME_LENGTH; i++)
        *ptr++ = gSaveBlock2Ptr->playerName[i];
    *ptr = EOS;
	
    AddTextPrinterParameterized3(CONTINUE_WINDOW_CONTINUE, 2, 62, 18, sTextColor2, -1, name);
}

static void PrintDexCount(void)
{
    u8 strbuf[30];
	
    if (FlagGet(FLAG_SYS_POKEDEX_GET))
    {
        AddTextPrinterParameterized3(CONTINUE_WINDOW_CONTINUE, 2, 2, 50, sTextColor2, -1, gText_Pokedex);
        ConvertIntToDecimalStringN(strbuf, GetNationalPokedexCount(FLAG_GET_CAUGHT), STR_CONV_MODE_LEFT_ALIGN, 4);
        AddTextPrinterParameterized3(CONTINUE_WINDOW_CONTINUE, 2, 62, 50, sTextColor2, -1, strbuf);
    }
}

static void PrintPlayTime(void)
{
    u8 strbuf[30];
    u8 *ptr;

    AddTextPrinterParameterized3(CONTINUE_WINDOW_CONTINUE, 2, 2, 34, sTextColor2, -1, gText_Time);
    ptr = ConvertIntToDecimalStringN(strbuf, gSaveBlock2Ptr->playTimeHours, STR_CONV_MODE_LEFT_ALIGN, 3);
    *ptr++ = CHAR_COLON;
    ConvertIntToDecimalStringN(ptr, gSaveBlock2Ptr->playTimeMinutes, STR_CONV_MODE_LEADING_ZEROS, 2);
    AddTextPrinterParameterized3(CONTINUE_WINDOW_CONTINUE, 2, 62, 34, sTextColor2, -1, strbuf);
}

static void PrintBadgeCount(void)
{
    u8 strbuf[30];
    AddTextPrinterParameterized3(CONTINUE_WINDOW_CONTINUE, 2, 2, 66, sTextColor2, -1, gText_Badges);
    ConvertIntToDecimalStringN(strbuf, GetNumOfBadges(), STR_CONV_MODE_LEADING_ZEROS, 1);
    AddTextPrinterParameterized3(CONTINUE_WINDOW_CONTINUE, 2, 62, 66, sTextColor2, -1, strbuf);
}

// Create different windows on bg1 for each pokemon icon
static void DrawPartyMonIcons(void)
{
	u8 i, windowId;
	u16 species;
	struct WindowTemplate template;

	LoadMonIconPalettesAt(0x30);
	
	for (i = 0; i < gPlayerPartyCount; i++)
	{
		species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2);
		
		template = SetWindowTemplateFields(1, 3 + (i * 4), 10, 4, 4, GetValidMonIconPalIndex(species) + 3, 0x001 + (i * 4 * 4));
		windowId = AddWindow(&template);
		FillWindowPixelBuffer(windowId, PIXEL_FILL(0));
		LoadMonIconGraphicsInWindow(windowId, species);
		PutWindowTilemap(windowId);
		CopyWindowToVram(windowId, COPYWIN_BOTH);
	}
}

static void PrintContinueStats(void)
{
	PrintPlayerName();
    PrintDexCount();
    PrintPlayTime();
    PrintBadgeCount();
    DrawPartyMonIcons();
}

static void PrintMessageOnErrorWindow(const u8 *str)
{
	FillWindowPixelBuffer(ERROR_WINDOW_MESSAGE, PIXEL_FILL(10));
    MainMenu_DrawWindow(&sMainMenuWinTemplates[ERROR_WINDOW_MESSAGE]);
    AddTextPrinterParameterized3(ERROR_WINDOW_MESSAGE, 2, 0, 2, sTextColor1, 2, str);
    PutWindowTilemap(ERROR_WINDOW_MESSAGE);
    CopyWindowToVram(ERROR_WINDOW_MESSAGE, COPYWIN_GFX);
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(19, 221));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(115, 157));
}

static void PrintSaveErrorStatus(u8 taskId, const u8 *str)
{
	PrintMessageOnErrorWindow(str);
    MainMenu_InitFadeToDisplayBgs();
	gTasks[taskId].func = Task_SaveErrorStatus_RunPrinterThenWaitButton;
}

static void Task_SaveErrorStatus_RunPrinterThenWaitButton(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        RunTextPrinters();
		
        if (!IsTextPrinterActive(ERROR_WINDOW_MESSAGE) && JOY_NEW(A_BUTTON))
        {
            ClearWindowTilemap(ERROR_WINDOW_MESSAGE);
            MainMenu_EraseErrorWindow();
            LoadUserFrameToBg0();
			gTasks[taskId].func = gTasks[taskId].tMenuType == MAIN_MENU_NEWGAME ? Task_SetWin0BldRegsAndInitMainMenu : Task_PrintMainMenuText;
        }
    }
}

static void LoadUserFrameToBg0(void)
{
	const struct TextWindowGraphics * frame = GetUserFrameGraphicsInfo(gSaveBlock2Ptr->optionsWindowFrameType);
	
    LoadBgTiles(0, frame->tiles, 0x120, 0x1C9);
    LoadPalette(frame->palette, 0x20, 0x20);
    MainMenu_EraseErrorWindow();
}

static void SetStdFrame0OnBg0(void)
{
	TextWindow_SetStdFrame0_WithPal(NEWGAME_WINDOW_NEWGAME, 0x1C9, 0x20);
    MainMenu_EraseErrorWindow();
}

static void MainMenu_DrawWindow(const struct WindowTemplate * windowTemplate)
{
    FillBgTilemapBufferRect(windowTemplate->bg, 0x1C9, windowTemplate->tilemapLeft - 1, windowTemplate->tilemapTop - 1, 1, 1, 2);
    FillBgTilemapBufferRect(windowTemplate->bg, 0x1CA, windowTemplate->tilemapLeft, windowTemplate->tilemapTop - 1, windowTemplate->width, windowTemplate->height, 2);
    FillBgTilemapBufferRect(windowTemplate->bg, 0x1CB, windowTemplate->tilemapLeft + windowTemplate->width, windowTemplate->tilemapTop - 1, 1, 1, 2);
    FillBgTilemapBufferRect(windowTemplate->bg, 0x1CC, windowTemplate->tilemapLeft - 1, windowTemplate->tilemapTop, 1, windowTemplate->height, 2);
    FillBgTilemapBufferRect(windowTemplate->bg, 0x1CE, windowTemplate->tilemapLeft + windowTemplate->width, windowTemplate->tilemapTop, 1, windowTemplate->height, 2);
    FillBgTilemapBufferRect(windowTemplate->bg, 0x1CF, windowTemplate->tilemapLeft - 1, windowTemplate->tilemapTop + windowTemplate->height, 1, 1, 2);
    FillBgTilemapBufferRect(windowTemplate->bg, 0x1D0, windowTemplate->tilemapLeft, windowTemplate->tilemapTop + windowTemplate->height, windowTemplate->width, 1, 2);
    FillBgTilemapBufferRect(windowTemplate->bg, 0x1D1, windowTemplate->tilemapLeft + windowTemplate->width, windowTemplate->tilemapTop + windowTemplate->height, 1, 1, 2);
    CopyBgTilemapBufferToVram(windowTemplate->bg);
}

static void MainMenu_EraseErrorWindow(void)
{
	const struct WindowTemplate * windowTemplate = &sMainMenuWinTemplates[ERROR_WINDOW_MESSAGE];
	
	FillBgTilemapBufferRect(windowTemplate->bg, 0x000, windowTemplate->tilemapLeft - 1, windowTemplate->tilemapTop - 1,
		windowTemplate->tilemapLeft + windowTemplate->width + 1, windowTemplate->tilemapTop + windowTemplate->height + 1, 2);
    CopyBgTilemapBufferToVram(windowTemplate->bg);
}

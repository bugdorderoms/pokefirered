#include "global.h"
#include "option_menu.h"
#include "gflib.h"
#include "scanline_effect.h"
#include "text_window_graphics.h"
#include "menu.h"
#include "list_menu.h"
#include "task.h"
#include "overworld.h"
#include "text_window.h"
#include "menu_helpers.h"
#include "menu_indicators.h"
#include "field_fadetransition.h"
#include "gba/m4a_internal.h"

// Menu items
enum
{
    MENUITEM_TEXTSPEED = 0,
    MENUITEM_BATTLESCENE,
    MENUITEM_BATTLESTYLE,
    MENUITEM_SOUND,
    MENUITEM_FRAMETYPE,
	MENUITEM_PKMNNICKNAME,
	MENUITEM_DEXNAVSEARCH,
	MENUITEM_CANCEL,
    MENUITEM_COUNT
};

// Window Ids
enum
{
    WIN_TEXT_OPTION,
	WIN_OPTIONS,
	WIN_INSTRUCTIONS,
	WIN_COUNT
};

struct Option
{
	const u8 * const *options;
	u8 name[14];
	u8 numOptions;
};

struct OptionMenu
{
    u16 option[MENUITEM_COUNT];
    u16 cursorPos;
	u16 itemsAbove;
    u8 state;
	struct ListMenuItem *itemsList;
};

static void CB2_DoLoadOptionMenu(void);
static void InitOptionMenuBg(void);
static void DrawOptionMenuBg(void);
static void PrintOptionMenuHeader(void);
static u8 CreateOptionsListMenu(void);
static void OptionMenu_PickSwitchCancel(void);
static void Options_MoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list);
static void Options_ItemPrintFunc(u8 windowId, u32 itemId, u8 y);
static void Task_OptionMenu(u8 taskId);
static bool8 OptionMenu_ProcessInput(u8 taskId);
static void CloseAndSaveOptionMenu(u8 taskId);
static void UpdateOptionsText(u8 windowId, u32 itemId, u8 y);

static EWRAM_DATA struct OptionMenu *sOptionMenuPtr = NULL;

static const u16 sOptionMenuPalette[] = INCBIN_U16("graphics/misc/unk_83cc2e4.gbapal");

static const u8 sText_PickSwitchCancel[] = _("{DPAD_UPDOWN}Pick {DPAD_LEFTRIGHT}Switch {A_BUTTON}{B_BUTTON}Cancel");

static const struct BgTemplate sOptionMenuBgTemplates[] =
{
   {
       .bg = 1,
       .charBaseIndex = 1,
       .mapBaseIndex = 30,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 0,
       .baseTile = 0
   },
   {
       .bg = 0,
       .charBaseIndex = 1,
       .mapBaseIndex = 31,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 1,
       .baseTile = 0
   },
   {
       .bg = 2,
       .charBaseIndex = 1,
       .mapBaseIndex = 29,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 2,
       .baseTile = 0
   },
};

static const struct WindowTemplate sOptionMenuWinTemplates[WIN_COUNT + 1] =
{
    [WIN_TEXT_OPTION] = {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 3,
        .width = 26,
        .height = 2,
        .paletteNum = 1,
        .baseBlock = 2
    },
	[WIN_OPTIONS] = {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 7,
        .width = 26,
        .height = 12,
        .paletteNum = 1,
        .baseBlock = 0x36
    },
    [WIN_INSTRUCTIONS] = {
        .bg = 2,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 30,
        .height = 2,
        .paletteNum = 0xF,
        .baseBlock = 0x16e
    },
    DUMMY_WIN_TEMPLATE
};

static const u8 *const sTextSpeedOptions[] =
{
    COMPOUND_STRING("Slow"), 
    COMPOUND_STRING("Mid"), 
    COMPOUND_STRING("Fast")
};

static const u8 *const sBattleSceneOptions[] =
{
    COMPOUND_STRING("On"), 
    COMPOUND_STRING("Off")
};

static const u8 *const sBattleStyleOptions[] =
{
    COMPOUND_STRING("Shift"),
    COMPOUND_STRING("Set")
};

static const u8 *const sSoundOptions[] =
{
    COMPOUND_STRING("Mono"), 
    COMPOUND_STRING("Stereo")
};

static const u8 *const sFrameOptions[] =
{
	COMPOUND_STRING("Type 1"),
	COMPOUND_STRING("Type 2"),
	COMPOUND_STRING("Type 3"),
	COMPOUND_STRING("Type 4"),
	COMPOUND_STRING("Type 5"),
	COMPOUND_STRING("Type 6"),
	COMPOUND_STRING("Type 7"),
	COMPOUND_STRING("Type 8"),
	COMPOUND_STRING("Type 9"),
	COMPOUND_STRING("Type 10")
};

static const u8 *const sNicknameOptions[] =
{
	COMPOUND_STRING("Give"),
	COMPOUND_STRING("Skip")
};

static const u8 *const sDexnavSearchOptions[] =
{
	COMPOUND_STRING("L Button"),
	COMPOUND_STRING("R Button")
};

#define OPTION(_name, _options)          \
{                                        \
	.name = _(_name),                    \
	.options = _options,                 \
	.numOptions = ARRAY_COUNT(_options), \
}

static const struct Option sOptionMenuOptions[MENUITEM_COUNT] =
{
	[MENUITEM_TEXTSPEED]      = OPTION("Text Speed",    sTextSpeedOptions),
	[MENUITEM_BATTLESCENE]    = OPTION("Battle Scene",  sBattleSceneOptions),
	[MENUITEM_BATTLESTYLE]    = OPTION("Battle Style",  sBattleStyleOptions),
    [MENUITEM_SOUND]          = OPTION("Sound",         sSoundOptions),
    [MENUITEM_FRAMETYPE]      = OPTION("Frame",         sFrameOptions),
	[MENUITEM_PKMNNICKNAME]   = OPTION("Pokémon Nick",  sNicknameOptions),
	[MENUITEM_DEXNAVSEARCH]   = OPTION("Dexnav Search", sDexnavSearchOptions),
	[MENUITEM_CANCEL]         = OPTION("Cancel",        NULL)
};

static void LoadOrSaveOptions(bool8 load)
{
	if (load)
	{
		sOptionMenuPtr->option[MENUITEM_TEXTSPEED]    = gSaveBlock2Ptr->optionsTextSpeed;
        sOptionMenuPtr->option[MENUITEM_BATTLESCENE]  = gSaveBlock2Ptr->optionsBattleSceneOff;
        sOptionMenuPtr->option[MENUITEM_BATTLESTYLE]  = gSaveBlock2Ptr->optionsBattleStyle;
        sOptionMenuPtr->option[MENUITEM_SOUND]        = gSaveBlock2Ptr->optionsSound;
        sOptionMenuPtr->option[MENUITEM_FRAMETYPE]    = gSaveBlock2Ptr->optionsWindowFrameType;
	    sOptionMenuPtr->option[MENUITEM_PKMNNICKNAME] = gSaveBlock2Ptr->optionsSkipPkmnNickname;
		sOptionMenuPtr->option[MENUITEM_DEXNAVSEARCH] = gSaveBlock2Ptr->optionsDexnavSearchOnR;
	}
	else
	{
		gSaveBlock2Ptr->optionsTextSpeed        = sOptionMenuPtr->option[MENUITEM_TEXTSPEED];
		gSaveBlock2Ptr->optionsBattleSceneOff   = sOptionMenuPtr->option[MENUITEM_BATTLESCENE];
		gSaveBlock2Ptr->optionsBattleStyle      = sOptionMenuPtr->option[MENUITEM_BATTLESTYLE];
		gSaveBlock2Ptr->optionsSound            = sOptionMenuPtr->option[MENUITEM_SOUND];
		gSaveBlock2Ptr->optionsWindowFrameType  = sOptionMenuPtr->option[MENUITEM_FRAMETYPE];
		gSaveBlock2Ptr->optionsSkipPkmnNickname = sOptionMenuPtr->option[MENUITEM_PKMNNICKNAME];
		gSaveBlock2Ptr->optionsDexnavSearchOnR  = sOptionMenuPtr->option[MENUITEM_DEXNAVSEARCH];
		SetPokemonCryStereo(gSaveBlock2Ptr->optionsSound);
	}
}

void CB2_InitOptionsMenu(void)
{
    sOptionMenuPtr = AllocZeroed(sizeof(struct OptionMenu));
    sOptionMenuPtr->state = 0;
    sOptionMenuPtr->cursorPos = 0;
	sOptionMenuPtr->itemsAbove = 0;
	
	LoadOrSaveOptions(TRUE); // Load options
	
    SetMainCallback2(CB2_DoLoadOptionMenu);
}

static void CB2_OptionMenu(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB_OptionMenu(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void OptionsMenu_LoadWindowGraphics(void)
{
	LoadBgTiles(1, GetUserFrameGraphicsInfo(sOptionMenuPtr->option[MENUITEM_FRAMETYPE])->tiles, 0x120, 0x1AA);
	LoadPalette(GetUserFrameGraphicsInfo(sOptionMenuPtr->option[MENUITEM_FRAMETYPE])->palette, 0x20, 0x20);
}

static void CB2_DoLoadOptionMenu(void)
{
    switch (sOptionMenuPtr->state)
    {
    case 0:
        SetVBlankHBlankCallbacksToNull();
        break;
    case 1:
        InitOptionMenuBg();
        break;
    case 2:
	    ResetSpriteData();
		ResetPaletteFade();
		FreeAllSpritePalettes();
		ResetTasks();
		ScanlineEffect_Stop();
        break;
    case 3:
	    OptionsMenu_LoadWindowGraphics();
        LoadPalette(sOptionMenuPalette, 0x10, 0x20);
        LoadPalette(stdpal_get(2), 0xF0, 0x20);
        DrawWindowBorderWithStdpal3(1, 0x1B3, 0x30);
        break;
    case 4:
        PrintOptionMenuHeader();
        break;
    case 5:
        DrawOptionMenuBg();
        break;
    case 6:
	    OptionMenu_PickSwitchCancel();
		break;
	case 7:
	    gTasks[CreateTask(Task_OptionMenu, 0)].data[0] = CreateOptionsListMenu();
		SetMainCallback2(CB2_OptionMenu);
		sOptionMenuPtr->state = 0;
		return;
    }
    sOptionMenuPtr->state++;
}

static void InitOptionMenuBg(void)
{
    void * dest = (void *)VRAM;
    DmaClearLarge16(3, dest, VRAM_SIZE, 0x1000);    
    DmaClear32(3, (void *)OAM, OAM_SIZE);
    DmaClear16(3, (void *)PLTT, PLTT_SIZE);    
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0);
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sOptionMenuBgTemplates, ARRAY_COUNT(sOptionMenuBgTemplates));
    ResetAllBgsPos();
    InitWindows(sOptionMenuWinTemplates);
    DeactivateAllTextPrinters();
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_EFFECT_LIGHTEN);
    SetGpuReg(REG_OFFSET_BLDY, BLDCNT_TGT1_BG1);
    SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG0 | WININ_WIN0_OBJ);
    SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
};

static void DrawOptionMenuBg(void)
{
    u8 h = 2;
    
    FillBgTilemapBufferRect(1, 0x1B3, 1, 2, 1, 1, 3);
    FillBgTilemapBufferRect(1, 0x1B4, 2, 2, 0x1B, 1, 3);
    FillBgTilemapBufferRect(1, 0x1B5, 0x1C, 2, 1, 1, 3);
    FillBgTilemapBufferRect(1, 0x1B6, 1, 3, 1, h, 3);
    FillBgTilemapBufferRect(1, 0x1B8, 0x1C, 3, 1, h, 3);
    FillBgTilemapBufferRect(1, 0x1B9, 1, 5, 1, 1, 3);
    FillBgTilemapBufferRect(1, 0x1BA, 2, 5, 0x1B, 1, 3);
    FillBgTilemapBufferRect(1, 0x1BB, 0x1C, 5, 1, 1, 3);
    FillBgTilemapBufferRect(1, 0x1AA, 1, 6, 1, 1, h);
    FillBgTilemapBufferRect(1, 0x1AB, 2, 6, 0x1A, 1, h);
    FillBgTilemapBufferRect(1, 0x1AC, 0x1C, 6, 1, 1, h);
    FillBgTilemapBufferRect(1, 0x1AD, 1, 7, 1, 0x10, h);
    FillBgTilemapBufferRect(1, 0x1AF, 0x1C, 7, 1, 0x10, h);
    FillBgTilemapBufferRect(1, 0x1B0, 1, 0x13, 1, 1, h);
    FillBgTilemapBufferRect(1, 0x1B1, 2, 0x13, 0x1A, 1, h);
    FillBgTilemapBufferRect(1, 0x1B2, 0x1C, 0x13, 1, 1, h);
	
    CopyBgTilemapBufferToVram(1);
}

static void PrintOptionMenuHeader(void)
{
    FillWindowPixelBuffer(WIN_TEXT_OPTION, PIXEL_FILL(1));
    AddTextPrinterParameterized(WIN_TEXT_OPTION, 2, COMPOUND_STRING("Option"), 8, 1, TEXT_SPEED_FF, NULL);
    PutWindowTilemap(WIN_TEXT_OPTION);
    CopyWindowToVram(WIN_TEXT_OPTION, COPYWIN_BOTH);
}

static void OptionMenu_PickSwitchCancel(void)
{
	u8 color[3] = {TEXT_DYNAMIC_COLOR_6, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY};
    FillWindowPixelBuffer(WIN_INSTRUCTIONS, PIXEL_FILL(15)); 
    AddTextPrinterParameterized3(WIN_INSTRUCTIONS, 0, 0xE4 - GetStringWidth(0, sText_PickSwitchCancel, 0), 0, color, 0, sText_PickSwitchCancel);
    PutWindowTilemap(WIN_INSTRUCTIONS);
    CopyWindowToVram(WIN_INSTRUCTIONS, COPYWIN_BOTH);
}

static u8 CreateOptionsListMenu(void)
{
	u8 i;
	
	sOptionMenuPtr->itemsList = AllocZeroed(sizeof(struct ListMenuItem) * MENUITEM_COUNT);
	
	for (i = 0; i < MENUITEM_COUNT; i++)
    {
        sOptionMenuPtr->itemsList[i].label = sOptionMenuOptions[i].name;
		sOptionMenuPtr->itemsList[i].index = i;
    }
	gMultiuseListMenuTemplate.items = sOptionMenuPtr->itemsList;
    gMultiuseListMenuTemplate.totalItems = MENUITEM_COUNT;
    gMultiuseListMenuTemplate.windowId = WIN_OPTIONS;
    gMultiuseListMenuTemplate.header_X = 0;
    gMultiuseListMenuTemplate.item_X = 8;
    gMultiuseListMenuTemplate.cursor_X = 0;
    gMultiuseListMenuTemplate.lettersSpacing = 1;
    gMultiuseListMenuTemplate.itemVerticalPadding = 0;
    gMultiuseListMenuTemplate.upText_Y = 1;
    gMultiuseListMenuTemplate.maxShowed = 7;
    gMultiuseListMenuTemplate.fontId = 0;
    gMultiuseListMenuTemplate.cursorPal = 2;
    gMultiuseListMenuTemplate.fillValue = 1;
    gMultiuseListMenuTemplate.cursorShadowPal = 3;
    gMultiuseListMenuTemplate.moveCursorFunc = Options_MoveCursorFunc;
    gMultiuseListMenuTemplate.itemPrintFunc = Options_ItemPrintFunc;
    gMultiuseListMenuTemplate.scrollMultiple = 0;
    gMultiuseListMenuTemplate.cursorKind = 1;
	
	return ListMenuInit(&gMultiuseListMenuTemplate, sOptionMenuPtr->cursorPos, sOptionMenuPtr->itemsAbove);
}

static void Task_OptionMenu(u8 taskId)
{
	switch (sOptionMenuPtr->state)
    {
	case 0:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB_OptionMenu);
        sOptionMenuPtr->state++;
        break;
    case 1:
        if (gPaletteFade.active)
            return;
		AddScrollIndicatorArrowPairParameterized(SCROLL_ARROW_UP, DISPLAY_WIDTH / 2, 54, 154, MENUITEM_COUNT - 7, 110, 110, &sOptionMenuPtr->cursorPos);
        sOptionMenuPtr->state++;
        break;
    case 2:
        if (MenuHelpers_CallLinkSomething())
            break;
		
		if (OptionMenu_ProcessInput(taskId))
			sOptionMenuPtr->state++;
        break;
    case 3:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
        sOptionMenuPtr->state++;
        break;
    case 4:
        if (gPaletteFade.active)
            return;
        sOptionMenuPtr->state++;
        break;
    case 5:
        CloseAndSaveOptionMenu(taskId);
        break;
    }
}

static bool8 OptionMenu_ProcessInput(u8 taskId)
{
	s8 rightLeftMove = 0;
	u8 itemId, listMenuTaskId = gTasks[taskId].data[0];
	
	ListMenu_ProcessInput(listMenuTaskId);
	ListMenuGetScrollAndRow(listMenuTaskId, &sOptionMenuPtr->cursorPos, &sOptionMenuPtr->itemsAbove);
	
	if (JOY_REPT(DPAD_RIGHT))
		rightLeftMove = +1; // Move to right
	else if (JOY_REPT(DPAD_LEFT))
		rightLeftMove = -1; // Move to left
	
	itemId = sOptionMenuPtr->cursorPos + sOptionMenuPtr->itemsAbove;
	
	if (rightLeftMove != 0 && itemId != MENUITEM_CANCEL)
	{
		if (rightLeftMove == +1)
		{
			if (sOptionMenuPtr->option[itemId] + 1 == sOptionMenuOptions[itemId].numOptions)
				sOptionMenuPtr->option[itemId] = 0;
			else
				sOptionMenuPtr->option[itemId]++;
			
			UpdateOptionsText(WIN_OPTIONS, itemId, ListMenuGetYCoordForPrintingArrowCursor(listMenuTaskId));
		}
		else
		{
			if (sOptionMenuPtr->option[itemId] == 0)
				sOptionMenuPtr->option[itemId] = (sOptionMenuOptions[itemId].numOptions - 1);
			else
				sOptionMenuPtr->option[itemId]--;
			
			UpdateOptionsText(WIN_OPTIONS, itemId, ListMenuGetYCoordForPrintingArrowCursor(listMenuTaskId));
		}   
	}
	
	if (JOY_NEW(A_BUTTON | B_BUTTON))
		return TRUE; // Close the menu
	
	return FALSE;
}

static void UpdateOptionsText(u8 windowId, u32 itemId, u8 y)
{
	// Update text
	if (sOptionMenuOptions[itemId].options != NULL)
	{
		u8 x = 0x82, color[3] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_LIGHT_RED, TEXT_COLOR_RED};
		
		FillWindowPixelRect(windowId, PIXEL_FILL(1), x, y, 0x46, GetFontAttribute(0, FONTATTR_MAX_LETTER_HEIGHT));
		AddTextPrinterParameterized3(windowId, 0, x, y, color, -1, sOptionMenuOptions[itemId].options[sOptionMenuPtr->option[itemId]]);
	}
	
	// Special updates
	switch (itemId)
	{
		case MENUITEM_FRAMETYPE:
		    OptionsMenu_LoadWindowGraphics();
			break;
	}
	PutWindowTilemap(windowId);
    CopyWindowToVram(windowId, COPYWIN_BOTH);
}

static void Options_MoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list)
{
	u16 maxLetterHeight = GetFontAttribute(0, FONTATTR_MAX_LETTER_HEIGHT), y = (itemIndex - list->cursorPos) * maxLetterHeight + 0x3A;
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(y, y + maxLetterHeight));
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(0x10, 0xE0));
}

static void Options_ItemPrintFunc(u8 windowId, u32 itemId, u8 y)
{
	UpdateOptionsText(windowId, itemId, y);
}

static void CloseAndSaveOptionMenu(u8 taskId)
{
    gFieldCallback = FieldCB_DefaultWarpExit;
    SetMainCallback2(gMain.savedCallback);
    FreeAllWindowBuffers();
	LoadOrSaveOptions(FALSE); // Save options
	FREE_AND_SET_NULL(sOptionMenuPtr->itemsList);
    FREE_AND_SET_NULL(sOptionMenuPtr);
    DestroyTask(taskId);
}

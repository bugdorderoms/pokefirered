// Credits: BSBob, psf, ravepossum
#include "global.h"
#include "gflib.h"
#include "task.h"
#include "menu.h"
#include "rtc.h"
#include "dns.h"
#include "event_data.h"
#include "new_menu_helpers.h"
#include "text_window.h"
#include "region_map.h"
#include "strings.h"

// States and data defines for Task_MapNamePopUpWindow
enum
{
	STATE_PRINT,
    STATE_SLIDE_IN,
    STATE_WAIT,
    STATE_SLIDE_OUT,
    STATE_ERASE,
    STATE_END
};

static const u8 sMapPopUpPrimary[] = INCBIN_U8("graphics/map_popup/primary.4bpp");
static const u8 sMapPopUpSecondary[] = INCBIN_U8("graphics/map_popup/secondary.4bpp");
static const u16 sMapPopUpPalette[] = INCBIN_U16("graphics/map_popup/palette.gbapal");

static void Task_MapNamePopup(u8 taskId);
static void ShowMapNamePopUpWindow(struct Task *task);
static void HideMapNamePopUpWindow(void);
static void MapNamePopupPrintTextOnWindows(u8 primaryWindowId, u8 secondaryWindowId);

static EWRAM_DATA u8 sMapPopUpTaskId = 0;

static const struct WindowTemplate sMapPopUpWindows[2] =
{
	// Top popup
	{
		.bg = 0,
		.tilemapLeft = 0,
		.tilemapTop = 0,
		.width = 30,
		.height = 3,
		.paletteNum = 13,
		.baseBlock = 0x107,
	},
	// Down popup
	{
		.bg = 0,
		.tilemapLeft = 0,
		.tilemapTop = 17,
		.width = 30,
		.height = 3,
		.paletteNum = 13,
		.baseBlock = 0x161,
	}
};

#define tState             data[0]
#define tTimer             data[1]
#define tPos               data[2]
#define tReshow            data[3]
#define tPrintTimer        data[4]
#define tPrimaryWindowId   data[5]
#define tSecondaryWindowId data[6]

#define MAP_POPUP_SLIDE_SPEED 2
#define MAP_POPUP_INITIAL_Y   24

void ShowMapNamePopup(void)
{
    if (!FlagGet(FLAG_DONT_SHOW_MAP_NAME_POPUP))
    {
		u8 taskId = FindTaskIdByFunc(Task_MapNamePopup);
		
        if (taskId == 0xFF)
        {
            sMapPopUpTaskId = CreateTask(Task_MapNamePopup, 90);
            gTasks[sMapPopUpTaskId].tState = STATE_PRINT;
            gTasks[sMapPopUpTaskId].tPos = MAP_POPUP_INITIAL_Y;
        }
        else
        {
            if (gTasks[taskId].tState != STATE_SLIDE_OUT)
                gTasks[taskId].tState = STATE_SLIDE_OUT;
			
            gTasks[taskId].tReshow = TRUE;
        }
    }
}

bool32 IsMapNamePopupTaskActive(void)
{
    return FuncIsActiveTask(Task_MapNamePopup);
}

void DismissMapNamePopup(void)
{
    u8 taskId = FindTaskIdByFunc(Task_MapNamePopup);
	
    if (taskId != 0xFF)
    {
        if (gTasks[taskId].tState < STATE_ERASE)
            gTasks[taskId].tState = STATE_ERASE;
    }
}

static void HBlankCB_DoublePopupWindow(void)
{
    u16 offset = gTasks[sMapPopUpTaskId].tPos;
    u16 scanline = REG_VCOUNT;

    if (scanline < 80 || scanline > 160)
        REG_BG0VOFS = offset;
    else
        REG_BG0VOFS = 512 - offset;
}

static void Task_MapNamePopup(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
	
    switch (task->tState)
    {
		case STATE_PRINT:
			if (++task->tPrintTimer > 30)
			{
				task->tPrintTimer = 0;
				ShowMapNamePopUpWindow(task);
				EnableInterrupts(INTR_FLAG_HBLANK);
				SetHBlankCallback(HBlankCB_DoublePopupWindow);
				task->tState = STATE_SLIDE_IN;
			}
			break;
		case STATE_SLIDE_IN:
			task->tPos -= MAP_POPUP_SLIDE_SPEED;
			
			if (task->tPos <= 0)
			{
				task->tPos = 0;
				task->tTimer = 0;
				task->tState = STATE_WAIT;
			}
			break;
		case STATE_WAIT:
			if (++task->tTimer > 120)
			{
				task->tTimer = 0;
				task->tState = STATE_SLIDE_OUT;
			}
			break;
		case STATE_SLIDE_OUT:
			task->tPos += MAP_POPUP_SLIDE_SPEED;
			
			if (task->tPos >= MAP_POPUP_INITIAL_Y)
			{
				task->tPos = MAP_POPUP_INITIAL_Y;
				
				if (task->tReshow)
				{
					task->tReshow = FALSE;
					task->tTimer = 0;
					task->tState = STATE_PRINT;
				}
				else
					task->tState = STATE_ERASE;
			}
			break;
		case STATE_ERASE:
			ClearStdWindowAndFrame(task->tPrimaryWindowId, TRUE);
			RemoveWindow(task->tPrimaryWindowId);
			
			ClearStdWindowAndFrame(task->tSecondaryWindowId, TRUE);
			RemoveWindow(task->tSecondaryWindowId);
			
			task->tState = STATE_END;
			break;
		case STATE_END:
			HideMapNamePopUpWindow();
			DestroyTask(taskId);
			break;
	}
}

static void ShowMapNamePopUpWindow(struct Task *task)
{
	LoadPalette(sMapPopUpPalette, 0xd0, 0x20);
	
	task->tPrimaryWindowId = AddWindow(&sMapPopUpWindows[0]);
	task->tSecondaryWindowId = AddWindow(&sMapPopUpWindows[1]);
	
	CopyToWindowPixelBuffer(task->tPrimaryWindowId, sMapPopUpPrimary, sizeof(sMapPopUpPrimary), 0);
	CopyToWindowPixelBuffer(task->tSecondaryWindowId, sMapPopUpSecondary, sizeof(sMapPopUpSecondary), 0);
	
	PutWindowTilemap(task->tPrimaryWindowId);
	PutWindowTilemap(task->tSecondaryWindowId);
	
	MapNamePopupPrintTextOnWindows(task->tPrimaryWindowId, task->tSecondaryWindowId);
}

static void HideMapNamePopUpWindow(void)
{
	DisableInterrupts(INTR_FLAG_HBLANK);
	SetHBlankCallback(NULL);
	SetGpuReg_ForcedBlank(REG_OFFSET_BG0VOFS, 0);
}

static u8 *MapNamePopupAppendFloorNum(u8 *dest, s8 floorNum)
{
    if (floorNum == 0)
        return dest;
	
    *dest++ = CHAR_SPACE;
	
    if (floorNum == 0x7F)
        return StringCopy(dest, gText_Rooftop2);
	
    if (floorNum < 0)
    {
        *dest++ = CHAR_B;
        floorNum *= -1;
    }
    dest = ConvertIntToDecimalStringN(dest, floorNum, STR_CONV_MODE_LEFT_ALIGN, 2);
    *dest++ = CHAR_F;
    *dest = EOS;
    return dest;
}

static u8 *FormatDecimalTimeToMeridiemSystem(u8 *txtPtr)
{
	u8 hour = gRtcLocation.hour;
	
	if (hour == 0)
		txtPtr = ConvertIntToDecimalStringN(txtPtr, 12, STR_CONV_MODE_LEADING_ZEROS, 2);
	else if (hour < 13)
		txtPtr = ConvertIntToDecimalStringN(txtPtr, hour, STR_CONV_MODE_LEADING_ZEROS, 2);
	else
		txtPtr = ConvertIntToDecimalStringN(txtPtr, hour - 12, STR_CONV_MODE_LEADING_ZEROS, 2);
	
	*txtPtr++ = CHAR_COLON;
	txtPtr = ConvertIntToDecimalStringN(txtPtr, gRtcLocation.minute, STR_CONV_MODE_LEADING_ZEROS, 2);
	*txtPtr++ = CHAR_SPACE;
	txtPtr = StringCopy(txtPtr, hour < 12 ? COMPOUND_STRING("AM") : COMPOUND_STRING("PM"));
	
	return txtPtr;
}

static s32 GetStringRightAlignXOffset(u8 fontId, const u8 *str, s32 totalWidth, u8 letterSpacing)
{
	s32 stringWidth = GetStringWidth(fontId, str, letterSpacing);
	
    if (totalWidth > stringWidth)
        return totalWidth - stringWidth;
    else
        return 0;
}

static void MapNamePopupPrintTextOnWindows(u8 primaryWindowId, u8 secondaryWindowId)
{
    u8 mapDisplayHeader[25];
	u8 *ptr, *withoutPrefixPtr;
	
	mapDisplayHeader[0] = EXT_CTRL_CODE_BEGIN;
    mapDisplayHeader[1] = EXT_CTRL_CODE_HIGHLIGHT;
    mapDisplayHeader[2] = TEXT_COLOR_TRANSPARENT;
	
	withoutPrefixPtr = &(mapDisplayHeader[3]);
	
	// Print map name
	ptr = GetMapName(withoutPrefixPtr, gMapHeader.regionMapSectionId);
	
	if (gMapHeader.floorNum != 0)
        ptr = MapNamePopupAppendFloorNum(ptr, gMapHeader.floorNum);
	
	AddTextPrinterParameterized(primaryWindowId, 2, mapDisplayHeader, 8, 2, TEXT_SPEED_FF, NULL);
	
	// Print time and season
	ptr = FormatDecimalTimeToMeridiemSystem(withoutPrefixPtr);
	*ptr++ = CHAR_SPACE;
	*ptr++ = CHAR_HYPHEN;
	*ptr++ = CHAR_SPACE;
	ptr = DNSCopyCurrentSeasonName(ptr);
	
	AddTextPrinterParameterized(secondaryWindowId, 0, mapDisplayHeader, GetStringRightAlignXOffset(0, mapDisplayHeader, DISPLAY_WIDTH, 0) - 5, 8, TEXT_SPEED_FF, NULL);
	
	CopyWindowToVram(primaryWindowId, COPYWIN_BOTH);
	CopyWindowToVram(secondaryWindowId, COPYWIN_BOTH);
}

#undef tState
#undef tTimer
#undef tPos
#undef tReshow
#undef tPrintTimer
#undef tPrimaryWindowId
#undef tSecondaryWindowId

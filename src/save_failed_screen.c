#include "global.h"
#include "gflib.h"
#include "save_failed_screen.h"
#include "decompress.h"
#include "gba/flash_internal.h"
#include "menu.h"
#include "m4a.h"
#include "task.h"
#include "text_window.h"
#include "save.h"
#include "strings.h"

bool32 sIsInSaveFailedScreen;

static EWRAM_DATA u16 sSaveType = SAVE_NORMAL;
static EWRAM_DATA u8 sSaveFailedScreenState = 0;

static void BlankPalettes(void);
static void UpdateMapBufferWithText(void);
static void ClearMapBuffer(void);
static void PrintTextOnSaveFailedScreen(const u8 *a0);
static bool32 TryWipeDamagedSectors(void);
static bool32 WipeDamagedSectors(u32 damagedSectors);

static const u16 sSaveFailedScreenPals[] = INCBIN_U16("graphics/interface/save_failed_screen.gbapal");

static const u8 sText_FlashNotDetected[] = _("{COLOR RED}ERROR!\n{COLOR DARK_GRAY}Flash memory not detected.\n\nSet your emulator's save type\nsetting to Flash 1Mb / 128K\nand reload the rom.");

static const struct BgTemplate sFlashNotDetectedBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    }
};

static const struct WindowTemplate sFlashNotDetectedWinTemplates[] =
{
	{
		.bg = 0,
		.tilemapLeft = 3,
		.tilemapTop = 2,
		.width = 24,
		.height = 16,
		.paletteNum = 15,
		.baseBlock = 1,
	},
	DUMMY_WIN_TEMPLATE,
};

void SetNotInSaveFailedScreen(void)
{
    sIsInSaveFailedScreen = FALSE;
}

void DoSaveFailedScreen(u8 saveType)
{
    sSaveType = saveType;
    sIsInSaveFailedScreen = TRUE;
}

bool32 RunSaveFailedScreen(void)
{
    switch (sSaveFailedScreenState)
    {
    case 0:
        if (!sIsInSaveFailedScreen)
            return FALSE;
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 128);
        sSaveFailedScreenState = 1;
        break;
    case 1:
        BlankPalettes();
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        sSaveFailedScreenState = 2;
        break;
    case 2:
        RequestDma3Fill(0, (void *)BG_CHAR_ADDR(3), BG_CHAR_SIZE, DMA3_16BIT);
        RequestDma3Copy(sSaveFailedScreenPals, (void *)PLTT, 0x20, DMA3_16BIT);
        sSaveFailedScreenState = 3;
        break;
    case 3:
        ClearMapBuffer();
        PrintTextOnSaveFailedScreen(gText_SaveFailedScreen_CheckingBackupMemory);
        UpdateMapBufferWithText();
        sSaveFailedScreenState = 4;
        break;
    case 4:
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BG0HOFS, 0);
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
        SetGpuReg(REG_OFFSET_BG0CNT, BGCNT_PRIORITY(0) | BGCNT_CHARBASE(3) | BGCNT_SCREENBASE(31));
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_BG0_ON);
        sSaveFailedScreenState = 5;
        break;
    case 5:
        if (TryWipeDamagedSectors() == TRUE)
        {
            gSaveSucceeded = SAVE_STATUS_OK;
            PrintTextOnSaveFailedScreen(gText_SaveFailedScreen_SaveCompleted);
        }
        else
        {
            gSaveSucceeded = SAVE_STATUS_ERROR;
            PrintTextOnSaveFailedScreen(gText_SaveFailedScreen_BackupMemoryDamaged);
        }
        sSaveFailedScreenState = 6;
        break;
    case 6:
        if (JOY_NEW(A_BUTTON))
            sSaveFailedScreenState = 7;
        break;
    case 7:
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        BlankPalettes();
        sSaveFailedScreenState = 8;
        break;
    case 8:
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 256);
        sIsInSaveFailedScreen = FALSE;
        sSaveFailedScreenState = 0;
        break;
    }
    return TRUE;
}

static void BlankPalettes(void)
{
    int i;
    for (i = 0; i < BG_PLTT_SIZE; i += sizeof(u16))
    {
        *((u16 *)(BG_PLTT + i)) = RGB_BLACK;
        *((u16 *)(OBJ_PLTT + i)) = RGB_BLACK;
    }
}

static void RequestDmaCopyFromScreenBuffer(void)
{
    RequestDma3Copy(gDecompressionBuffer + 0x3800, (void *)BG_SCREEN_ADDR(31), 0x500, DMA3_16BIT);
}

static void RequestDmaCopyFromCharBuffer(void)
{
    RequestDma3Copy(gDecompressionBuffer + 0x020, (void *)BG_CHAR_ADDR(3) + 0x20, 0x2300, DMA3_16BIT);
}

static void FillBgMapBufferRect(u16 baseBlock, u8 left, u8 top, u8 width, u8 height, u16 blockOffset)
{
    u16 i, j;

    for (i = top; i < top + height; i++)
    {
        for (j = left; j < left + width; j++)
        {
            *((u16 *)(gDecompressionBuffer + 0x3800 + 64 * i + 2 * j)) = baseBlock;
            baseBlock += blockOffset;
        }
    }
    RequestDmaCopyFromScreenBuffer();
}

static void UpdateMapBufferWithText(void)
{
    FillBgMapBufferRect(0x001, 1, 5, 28, 10, 0x001);
}

static void ClearMapBuffer(void)
{
    FillBgMapBufferRect(0x000, 0, 0, 30, 20, 0x000);
}

static void PrintTextOnSaveFailedScreen(const u8 *str)
{
    GenerateFontHalfRowLookupTable(TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
    CpuFill16(PIXEL_FILL(1) | (PIXEL_FILL(1) << 8), gDecompressionBuffer + 0x20, 0x2300);
    RenderSaveFailedScreenText(2, gDecompressionBuffer + 0x20, str, 2, 2, 28, 10);
    RequestDmaCopyFromCharBuffer();
}

static bool32 TryWipeDamagedSectors(void)
{
    int i = 0;
    for (i = 0; gDamagedSaveSectors != 0 && i < 3; i++)
    {
        if (WipeDamagedSectors(gDamagedSaveSectors))
            return FALSE;
        HandleSavingData(sSaveType);
    }
    if (gDamagedSaveSectors != 0)
        return FALSE;
    return TRUE;
}

static bool16 VerifySectorWipe(u32 sector)
{
    u16 sector0 = sector;
    u16 i;
    u32 *saveDataBuffer = (void *)&gSaveDataBuffer;
    ReadFlash(sector0, 0, saveDataBuffer, 0x1000);
    for (i = 0; i < 0x1000 / sizeof(u32); i++, saveDataBuffer++)
    {
        if (*saveDataBuffer != 0)
            return TRUE;
    }
    return FALSE;
}

static bool32 WipeSector(u32 sector)
{
    bool32 result;
    u16 i, j;

    i = 0;
    while (i < 130)
    {
        for (j = 0; j < 0x1000; j++)
        {
            ProgramFlashByte(sector, j, 0);
        }
        result = VerifySectorWipe(sector);
        i++;
        if (!result)
            break;
    }

    return result;
}

static bool32 WipeDamagedSectors(u32 damagedSectors)
{
    int i;
    for (i = 0; i < 32; i++)
    {
        if (damagedSectors & (1 << i))
        {
            if (!WipeSector(i))
            {
                damagedSectors &= ~(1 << i);
            }
        }
    }
    if (damagedSectors == 0)
        return FALSE;
    else
        return TRUE;
}

void CB2_FlashNotDetectedScreen(void)
{
	switch (gMain.state)
	{
		case 0: // Reset gpu regs
			SetGpuReg(REG_OFFSET_DISPCNT, 0);
			SetGpuReg(REG_OFFSET_BLDCNT, 0);
			SetGpuReg(REG_OFFSET_BG0CNT, 0);
			SetGpuReg(REG_OFFSET_BG0HOFS, 0);
			SetGpuReg(REG_OFFSET_BG0VOFS, 0);
			break;
		case 1: // Clear graphical rams
			DmaFill16(3, 0, VRAM, VRAM_SIZE);
			DmaFill32(3, 0, OAM, OAM_SIZE);
			DmaFill16(3, 0, PLTT, PLTT_SIZE);
			break;
		case 2: // Reset graphical structs
			DeactivateAllTextPrinters();
			ResetTasks();
			ResetPaletteFade();
			ResetBgsAndClearDma3BusyFlags(0);
			break;
		case 3: // Init bgs
			InitBgsFromTemplates(0, sFlashNotDetectedBgTemplates, ARRAY_COUNT(sFlashNotDetectedBgTemplates));
			break;
		case 4: // Init windows
			InitWindows(sFlashNotDetectedWinTemplates);
			TextWindow_SetStdFrame0_WithPal(0, 0x214, 0xF0);
			DrawStdFrameWithCustomTileAndPalette(0, TRUE, 0x214, 15);
			AddTextPrinterParameterized(0, 2, sText_FlashNotDetected, 8, 1, 0, NULL); // Print text
			break;
		case 5: // Update palette
			TransferPlttBuffer();
			*(u16*)PLTT = RGB(17, 18, 31); // Set first color to blue for the background
			// Show bg 0
			ShowBg(0);
			break;
		case 6:
			return;
	}
	++gMain.state;
}

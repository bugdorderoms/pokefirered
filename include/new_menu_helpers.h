#ifndef GUARD_NEW_MENU_HELPERS_H
#define GUARD_NEW_MENU_HELPERS_H

#include "global.h"
#include "graphics.h"
#include "palette.h"
#include "text.h"
#include "task.h"

#define DLG_WINDOW_PALETTE_NUM 15
#define DLG_WINDOW_BASE_TILE_NUM 0x200
#define STD_WINDOW_PALETTE_NUM 14
#define STD_WINDOW_BASE_TILE_NUM 0x214

void ClearScheduledBgCopiesToVram(void);
void ScheduleBgCopyTilemapToVram(u32 bgId);
void DoScheduledBgTilemapCopiesToVram(void);
void ResetTempTileDataBuffers(void);
bool32 FreeTempTileDataBuffersIfPossible(void);
void *DecompressAndCopyTileDataToVram(u32 bgId, const void *src, u32 size, u16 offset, u32 mode);
void *DecompressAndCopyTileDataToVram2(u32 bgId, const void *src, u32 size, u16 offset, u32 mode);
void DecompressAndLoadBgGfxUsingHeap(u32 bgId, const void *src, u32 size, u16 offset, u32 mode);
void DecompressAndLoadBgGfxUsingHeap2(u32 bgId, const void *src, u32 size, u16 offset, u32 mode);
void * MallocAndDecompress(const void * src, u32 * size);
void SetBgTilemapPalette(u32 bgId, u32 left, u32 top, u32 width, u32 height, u32 palette);
void CopyToBufferFromBgTilemap(u32 bgId, u16 *dest, u32 left, u32 top, u32 width, u32 height);
void InitStandardTextBoxWindows(void);
void InitTextBoxGfxAndPrinters(void);
bool32 RunTextPrinters_CheckPrinter0Active(void);
void AddTextPrinterDiffStyle(bool32 allowSkippingDelayWithButtonPress);
void AddTextPrinterForMessage(bool32 allowSkippingDelayWithButtonPress);
void AddTextPrinterWithCustomSpeedForMessage(bool32 allowSkippingDelayWithButtonPress, u32 speed);
void LoadStdWindowFrameGfx(void);
void DrawDialogueFrame(u32 windowId, bool32 copyToVram);
void DrawStdWindowFrame(u32 windowId, bool32 copyToVram);
void ClearDialogWindowAndFrame(u32 windowId, bool32 copyToVram);
void ClearStdWindowAndFrame(u32 windowId, bool32 copyToVram);
void EraseFieldMessageBox(bool32 copyToVram);
void SetStdWindowBorderStyle(u32 windowId, bool32 copyToVram);
void LoadMessageBoxAndFrameGfx(u32 windowId, bool32 copyToVram);
void DisplayItemMessageOnField(u32 taskId, u32 fontId, const u8 *src, TaskFunc callback);
void DisplayYesNoMenuDefaultYes(void);
void DisplayYesNoMenuDefaultNo(void);
u32 GetTextSpeedSetting(void);
void LoadSignPostWindowFrameGfx(void);
void SetDefaultFontsPointer(void);

static inline void Menu_LoadStdPalAt(u32 offset, u32 size)
{
    LoadPalette(gTMCaseMainWindowPalette, offset, size);
}

#endif // GUARD_NEW_MENU_HELPERS_H

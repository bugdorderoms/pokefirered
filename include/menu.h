#ifndef GUARD_MENU_H
#define GUARD_MENU_H

#include "global.h"
#include "text.h"
#include "window.h"

#define MENU_NOTHING_CHOSEN -2
#define MENU_B_PRESSED -1

struct MenuAction
{
    const u8 *text;
    union {
        void (*void_u32)(u32);
        u32 (*u32_void)(void);
    } func;
};

// list_menu
void ListMenuLoadStdPalAt(u32 palOffset, u32 palId);
void BlitMoveInfoIcon(u32 windowId, u32 iconId, u32 x, u32 y);

// menu
s8 Menu_ProcessInputGridLayout(void);
u32 MultichoiceGrid_InitCursor(u32 windowId, u32 fontId, u32 left, u32 top, u32 optionWidth, u32 cols, u32 rows, u32 cursorPos);
void MultichoiceGrid_PrintItems(u32 windowId, u32 fontId, u32 itemWidth, u32 itemHeight, u32 cols, u32 rows, const struct MenuAction *strs);
void DestroyYesNoMenu(void);
s8 Menu_ProcessInputNoWrapClearOnChoose(void);
void CreateYesNoMenu(const struct WindowTemplate *window, u32 fontId, u32 left, u32 top, u32 baseTileNum, u32 paletteNum, u32 initialCursorPos);
void AddItemMenuActionTextPrinters(u32 windowId, u32 fontId, u32 left, u32 top, u32 letterSpacing, u32 lineHeight, u32 itemCount, const struct MenuAction *strs, const u8 *orderArray);
void UnionRoomAndTradeMenuPrintOptions(u32 windowId, u32 fontId, u32 lineHeight, u32 itemCount, const struct MenuAction *strs);

void PrintTextArray(u32 windowId, u32 fontId, u32 left, u32 top, u32 lineHeight, u32 itemCount, const struct MenuAction *strs);
s8 Menu_ProcessInputNoWrapAround_other(void);
s8 Menu_ProcessInput_other(void);
s8 Menu_ProcessInputNoWrapAround(void);
s8 Menu_ProcessInput(void);
u32 Menu_GetCursorPos(void);
u32 Menu_MoveCursorNoWrapAround(s8 cursorDelta);
u32 Menu_MoveCursor(s8 cursorDelta);
u32 Menu_InitCursor(u32 windowId, u32 fontId, u32 left, u32 top, u32 cursorHeight, u32 numChoices, u32 initialCursorPos);
u32 Menu_InitCursorInternal(u32 windowId, u32 fontId, u32 left, u32 top, u32 cursorHeight, u32 numChoices, u32 initialCursorPos, bool32 APressMuted);
void TopBarWindowPrintTwoStrings(const u8 *string, const u8 *string2, bool32 fgColorChooser, bool32 copyToVram);
void TopBarWindowPrintString(const u8 *string, bool32 copyToVram);
void ClearTopBarWindow(void);
void DestroyTopBarWindow(void);
u32 CreateTopBarWindowLoadPalette(u32 bg, u32 width, u32 yPos, u32 palette, u32 baseTile);
void ClearStdWindowAndFrameToTransparent(u32 windowId, bool32 copyToVram);
void DrawStdFrameWithCustomTileAndPalette(u32 windowId, bool32 copyToVram, u32 baseTileNum, u32 paletteNum);
void ClearDialogWindowAndFrameToTransparent(u32 windowId, bool32 copyToVram);
void DrawDialogFrameWithCustomTileAndPalette(u32 windowId, bool32 copyToVram, u32 tileNum, u32 paletteNum);
struct WindowTemplate SetWindowTemplateFields(u32 bg, u32 left, u32 top, u32 width, u32 height, u32 paletteNum, u32 baseBlock);
void Menu_PrintFormatIntlPlayerName(u32 windowId, const u8 * src, u32 x, u32 y);

#endif // GUARD_MENU_H

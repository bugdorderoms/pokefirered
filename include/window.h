#ifndef GUARD_WINDOW_H
#define GUARD_WINDOW_H

#include "global.h"

#define PIXEL_FILL(num) ((num) | ((num) << 4))

enum
{
    WINDOW_BG,
    WINDOW_TILEMAP_LEFT,
    WINDOW_TILEMAP_TOP,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    WINDOW_PALETTE_NUM,
    WINDOW_BASE_BLOCK,
    WINDOW_TILE_DATA
};

struct WindowTemplate
{
    u8 bg;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 paletteNum;
    u16 baseBlock;
};

#define WINDOWS_MAX 32

#define DUMMY_WIN_TEMPLATE          \
{                                   \
    0xFF,                           \
    0,                              \
    0,                              \
    0,                              \
    0,                              \
    0,                              \
    0,                              \
}

struct Window
{
    struct WindowTemplate window;
    u8 *tileData;
};

typedef void (*WindowFunc)(u32 bg, u32 tilemapLeft, u32 tilemapTop, u32 width, u32 height, u32 paletteNum);

u32 GetNumActiveWindowsOnBg(u32 bgId);
bool32 InitWindows(const struct WindowTemplate *templates);
u32 AddWindow(const struct WindowTemplate *template);
void RemoveWindow(u32 windowId);
void FreeAllWindowBuffers(void);
void FillHelpMessageWindow(u32 windowId);

// Mode parameter for funcs below
#define COPYWIN_MAP  1
#define COPYWIN_GFX  2
#define COPYWIN_BOTH 3

void CopyWindowToVram(u32 windowId, u32 mode);
void CopyWindowToVram8Bit(u32 windowId, u32 mode);

void PutWindowTilemap(u32 windowId);
void PutWindowRectTilemapOverridePalette(u32 windowId, u8 x, u8 y, u8 width, u8 height, u32 palette);
void ClearWindowTilemap(u32 windowId);
void PutWindowRectTilemap(u32 windowId, u8 x, u8 y, u8 width, u8 height);
void BlitBitmapToWindow(u32 windowId, const u8 *pixels, u16 x, u16 y, u16 width, u16 height);
void BlitBitmapRectToWindow(u32 windowId, const u8 *pixels, u16 srcX, u16 srcY, u16 srcWidth, int srcHeight, u16 destX, u16 destY, u16 rectWidth, u16 rectHeight);
void FillWindowPixelRect(u32 windowId, u32 fillValue, u16 x, u16 y, u16 width, u16 height);
void CopyToWindowPixelBuffer(u32 windowId, const void *src, u16 size, u16 tileOffset);
void FillWindowPixelBuffer(u32 windowId, u32 fillValue);
void ScrollWindow(u32 windowId, u32 direction, u8 distance, u8 fillValue);
void CallWindowFunction(u32 windowId, WindowFunc func);
void SetWindowAttribute(u32 windowId, u32 attributeId, u32 value);
u32 GetWindowAttribute(u32 windowId, u32 attributeId);
u32 AddWindow8Bit(const struct WindowTemplate *template);
void FillWindowPixelBuffer8Bit(u32 windowId, u32 fillValue);
void FillWindowPixelRect8Bit(u32 windowId, u32 fillValue, u16 x, u16 y, u16 width, u16 height);
void BlitBitmapRectToWindow4BitTo8Bit(u32 windowId, const u8 *pixels, u16 srcX, u16 srcY, u16 srcWidth, int srcHeight, u16 destX, u16 destY, u16 rectWidth, u16 rectHeight, u32 paletteNum);
u32 WindowWidthPx(u32 windowId);
u32 WindowHeightPx(u32 windowId);

extern const u8 gUnknown_84566A8[];
extern void *gWindowBgTilemapBuffers[];
extern struct Window gWindows[];

#endif // GUARD_WINDOW_H

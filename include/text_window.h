#ifndef GUARD_TEXT_WINDOW_H
#define GUARD_TEXT_WINDOW_H

#include "global.h"

struct TilesPal
{
    u32* tiles;
    u16* pal;
};

void rbox_fill_rectangle(u32 windowId);
const u16* stdpal_get(u32 id);
const u16* GetOverworldTextboxPalettePtr(void);
void TextWindow_LoadResourcesStdFrame0(u32 windowId, u16 tileStart, u32 palette);
void TextWindow_SetStdFrame0_WithPal(u32 windowId, u16 tileStart, u32 palette);
void TextWindow_SetUserSelectedFrame(u32 windowId, u16 tileStart, u32 palette);
void LoadUserWindowBorderGfx(u32 windowId, u16 tileStart, u32 palette);
void DrawWindowBorderWithStdpal3(u32 bgId, u16 tileStart, u32 palette);
void DrawTextBorderOuter(u32 windowId, u16 tileStart, u32 palette);
void DrawTextBorderInner(u32 windowId, u16 tileNum, u32 palNum);
void TextWindow_LoadTilesStdFrame1(u32 windowId, u16 destOffset);
void LoadSignpostWindowGfx(u32 windowId, u16 destOffset, u32 palIdx);
void LoadDexNavWindowGfx(u32 windowId, u16 destOffset, u32 palOffset);

#endif // GUARD_TEXT_WINDOW_H

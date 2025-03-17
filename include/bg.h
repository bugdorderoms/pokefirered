#ifndef GUARD_BG_H
#define GUARD_BG_H

#include "global.h"

struct BGCntrlBitfield // for the I/O registers
{
    volatile u16 priority:2;
    volatile u16 charBaseBlock:2;
    volatile u16 field_0_2:4;
    volatile u16 field_1_0:5;
    volatile u16 areaOverflowMode:1;
    volatile u16 screenSize:2;
};

enum
{
    BG_CTRL_ATTR_VISIBLE = 1,
    BG_CTRL_ATTR_CHARBASEINDEX,
    BG_CTRL_ATTR_MAPBASEINDEX,
    BG_CTRL_ATTR_SCREENSIZE,
    BG_CTRL_ATTR_PALETTEMODE,
    BG_CTRL_ATTR_PRIORITY,
    BG_CTRL_ATTR_MOSAIC,
    BG_CTRL_ATTR_WRAPAROUND,
};

enum
{
    BG_ATTR_CHARBASEINDEX = 1,
    BG_ATTR_MAPBASEINDEX,
    BG_ATTR_SCREENSIZE,
    BG_ATTR_PALETTEMODE,
    BG_ATTR_MOSAIC,
    BG_ATTR_WRAPAROUND,
    BG_ATTR_PRIORITY,
    BG_ATTR_MAPSIZE,
    BG_ATTR_BGTYPE,
    BG_ATTR_BASETILE
};

enum AdjustBgMosaicMode
{
    BG_MOSAIC_SET,
    BG_MOSAIC_SET_H,
    BG_MOSAIC_INC_H,
    BG_MOSAIC_DEC_H,
    BG_MOSAIC_SET_V,
    BG_MOSAIC_INC_V,
    BG_MOSAIC_DEC_V,
};

enum BgTileAllocMode
{
    BG_TILE_FIND_FREE_SPACE,
    BG_TILE_ALLOC,
    BG_TILE_FREE,
};

struct BgTemplate
{
    u16 bg:2;                   // 0x1, 0x2 -> 0x3
    u16 charBaseIndex:2;        // 0x4, 0x8 -> 0xC
    u16 mapBaseIndex:5;         // 0x10, 0x20, 0x40, 0x80, 0x100 -> 0x1F0
    u16 screenSize:2;           // 0x200, 0x400 -> 0x600
    u16 paletteMode:1;          // 0x800
    u16 priority:2;             // 0x1000, 0x2000 > 0x3000
    u16 baseTile:10;
};

void ResetBgs(void);
int BgTileAllocOp(int bg, int offset, int count, int mode);
void ResetBgsAndClearDma3BusyFlags(bool32 enableWindowTileAutoAlloc);
void InitBgsFromTemplates(u32 bgMode, const struct BgTemplate *templates, u32 numTemplates);
u16 LoadBgTiles(u32 bg, const void* src, u16 size, u16 destOffset);
u16 LoadBgTilemap(u32 bg, const void *src, u16 size, u16 destOffset);
bool32 IsDma3ManagerBusyWithBgCopy(void);
void ShowBg(u32 bg);
void HideBg(u32 bg);
void SetBgAttribute(u32 bg, u32 attributeId, u32 value);
u16 GetBgAttribute(u32 bg, u32 attributeId);
u32 ChangeBgX(u32 bg, u32 value, u32 op);
u32 GetBgX(u32 bg);
u32 ChangeBgY(u32 bg, u32 value, u32 op);
u32 ChangeBgY_ScreenOff(u32 bg, u32 value, u32 op);
u32 GetBgY(u32 bg);
void ResetAllBgsPos(void);
void SetBgAffine(u32 bg, u32 srcCenterX, u32 srcCenterY, s16 dispCenterX, s16 dispCenterY, s16 scaleX, s16 scaleY, u16 rotationAngle);
u8 AdjustBgMosaic(u8 value, u32 mode);
void SetBgTilemapBuffer(u32 bg, void *tilemap);
void UnsetBgTilemapBuffer(u32 bg);
void* GetBgTilemapBuffer(u32 bg);
void CopyToBgTilemapBuffer(u32 bg, const void *src, u16 mode, u16 destOffset);
void CopyBgTilemapBufferToVram(u32 bg);
void CopyToBgTilemapBufferRect(u32 bg, const void* src, u8 destX, u8 destY, u8 width, u8 height);
void CopyToBgTilemapBufferRect_ChangePalette(u32 bg, const void *src, u8 destX, u8 destY, u8 rectWidth, u8 rectHeight, u32 palette);
void CopyRectToBgTilemapBufferRect(u32 bg, const void* src, u8 srcX, u8 srcY, u8 srcWidth, u8 srcHeight, u8 destX, u8 destY, u8 rectWidth, u8 rectHeight, u32 palette1, s16 tileOffset, s16 palette2);
void FillBgTilemapBufferRect_Palette0(u32 bg, u16 tileNum, u8 x, u8 y, u8 width, u8 height);
void FillBgTilemapBufferRect(u32 bg, u16 tileNum, u8 x, u8 y, u8 width, u8 height, u8 palette);
void WriteSequenceToBgTilemapBuffer(u32 bg, u16 firstTileNum, u8 x, u8 y, u8 width, u8 height, u32 paletteSlot, s16 tileNumDelta);

extern bool8 gWindowTileAutoAllocEnabled;

#endif // GUARD_BG_H

#ifndef GUARD_TEXT_H
#define GUARD_TEXT_H

#include "global.h"
#include "characters.h"

#define NUM_TEXT_PRINTERS 32

#define TEXT_SPEED_INSTANT 0
// Given as a text speed when all the text should be
// loaded at once but not copied to vram yet.
#define TEXT_SPEED_FF 0xFF

enum
{
    FONT_SMALL,
    FONT_NORMAL_COPY_1,
    FONT_NORMAL,
    FONT_NORMAL_COPY_2,
    FONT_MALE,
    FONT_FEMALE,
    FONT_BRAILLE,
	FONT_BOLD,
	FONT_NARROWER,
	FONT_SMALL_NARROWER,
};

enum
{
    FONTATTR_MAX_LETTER_WIDTH,
    FONTATTR_MAX_LETTER_HEIGHT,
    FONTATTR_LETTER_SPACING,
    FONTATTR_LINE_SPACING,
    FONTATTR_UNKNOWN,   // dunno what this is yet
    FONTATTR_COLOR_FOREGROUND,
    FONTATTR_COLOR_BACKGROUND,
    FONTATTR_COLOR_SHADOW
};

struct GlyphInfo
{
    u8 pixels[0x80];
    u8 width;
    u8 height;
};

extern struct GlyphInfo gGlyphInfo;

struct TextPrinterSubStruct
{
    u8 glyphId:4;  // 0x14
    bool8 hasPrintBeenSpedUp:1;
    u8 font_type_5:3;
    u8 downArrowDelay:5;
    u8 downArrowYPosIdx:2;
    u8 hasGlyphIdBeenSet:1;
    u8 autoScrollDelay;
};

struct TextPrinterTemplate // TODO: Better name
{
    const u8* currentChar;
    u8 windowId;
    u8 fontId;
    u8 x;
    u8 y;
    u8 currentX;        // 0x8
    u8 currentY;
    u8 letterSpacing;
    u8 lineSpacing;
    u8 unk:4;   // 0xC
    u8 fgColor:4;
    u8 bgColor:4;
    u8 shadowColor:4;
};

struct TextPrinter
{
    struct TextPrinterTemplate printerTemplate;
    void (*callback)(struct TextPrinterTemplate *, u16); // 0x10
    union __attribute__((packed)) {
        struct TextPrinterSubStruct sub;
        u8 fields[7];
    } subUnion;
    u8 active;
    u8 state;       // 0x1C
    u8 textSpeed;
    u8 delayCounter;
    u8 scrollDistance;
    u8 minLetterSpacing;  // 0x20
    u8 japanese;
};

struct FontInfo
{
    u16 (*fontFunction)(struct TextPrinter *x);
    u8 maxLetterWidth;
    u8 maxLetterHeight;
    u8 letterSpacing;
    u8 lineSpacing;
    u8 unk:4;
    u8 fgColor:4;
    u8 bgColor:4;
    u8 shadowColor:4;
};

extern const struct FontInfo *gFonts;

struct GlyphWidthFunc
{
    u32 fontId;
    s32 (*func)(u16 glyphId, bool32 isJapanese);
};

struct KeypadIcon
{
    u16 tileOffset;
    u8 width;
    u8 height;
};

typedef struct {
    u8 canABSpeedUpPrint:1;
    u8 useAlternateDownArrow:1;
    u8 autoScroll:1;
    u8 forceMidTextSpeed:1;
} TextFlags;

extern TextFlags gTextFlags;

extern u8 gStringVar1[];
extern u8 gStringVar2[];
extern u8 gStringVar3[];
extern u8 gStringVar4[];

extern const u8 gKeypadIconTiles[];

void SetFontsPointer(const struct FontInfo *fonts);
void DeactivateAllTextPrinters(void);
u16 AddTextPrinterParameterized(u8 windowId, u8 fontId, const u8 *str, u8 x, u8 y, u8 speed, void (*callback)(struct TextPrinterTemplate *, u16));
u16 AddTextPrinterParameterized2(u8 windowId, u8 fontId, const u8 *str, u8 speed, void (*callback)(struct TextPrinterTemplate *, u16), u8 fgColor, u8 bgColor, u8 shadowColor);
void AddTextPrinterParameterized3(u8 windowId, u8 fontId, u8 x, u8 y, const u8 * color, s8 speed, const u8 * str);
void AddTextPrinterParameterized4(u8 windowId, u8 fontId, u8 x, u8 y, u8 letterSpacing, u8 lineSpacing, const u8 *color, s8 speed, const u8 *str);
void AddTextPrinterParameterized5(u8 windowId, u8 fontId, const u8 *str, u8 x, u8 y, u8 speed, void (*callback)(struct TextPrinterTemplate *, u16), u8 letterSpacing, u8 lineSpacing);
bool16 AddTextPrinter(struct TextPrinterTemplate *textSubPrinter, u8 speed, void (*callback)(struct TextPrinterTemplate *, u16));
void RunTextPrinters(void);
bool16 IsTextPrinterActive(u8 id);
u32 RenderFont(struct TextPrinter *textPrinter);
void GenerateFontHalfRowLookupTable(u8 fgColor, u8 bgColor, u8 shadowColor);
void SaveTextColors(u8 *fgColor, u8 *bgColor, u8 *shadowColor);
void RestoreTextColors(u8 *fgColor, u8 *bgColor, u8 *shadowColor);
void DecompressGlyphTile(const u16 *src, u16 *dest);
u8 GetLastTextColor(u8 colorType);
void CopyGlyphToWindow(struct TextPrinter *x);
void ClearTextSpan(struct TextPrinter *textPrinter, u32 width);
void TextPrinterInitDownArrowCounters(struct TextPrinter *textPrinter);
void TextPrinterDrawDownArrow(struct TextPrinter *textPrinter);
void TextPrinterClearDownArrow(struct TextPrinter *textPrinter);
bool8 TextPrinterWaitAutoMode(struct TextPrinter *textPrinter);
bool16 TextPrinterWaitWithDownArrow(struct TextPrinter *textPrinter);
bool16 TextPrinterWait(struct TextPrinter *textPrinter);
void DrawDownArrow(u8 windowId, u16 x, u16 y, u8 bgColor, bool8 drawArrow, u8 *counter, u8 *yCoordIndex);
u16 RenderText(struct TextPrinter *textPrinter);
void RenderSaveFailedScreenText(u8 fontId, u8 * dest, const u8 * src, u8 x, u8 y, u8 width, u8 height);
s32 (*GetFontWidthFunc(u8 glyphId))(u16, bool32);
s32 GetStringWidth(u8 fontId, const u8 *str, s16 letterSpacing);
u8 RenderTextFont9(u8 *pixels, u8 fontId, u8 *str, int a3, int a4, int a5, int a6, int a7);
u8 DrawKeypadIcon(u8 windowId, u8 keypadIconId, u16 x, u16 y);
u8 GetKeypadIconTileOffset(u8 keypadIconId);
u8 GetKeypadIconWidth(u8 keypadIconId);
u8 GetKeypadIconHeight(u8 keypadIconId);
u8 GetFontAttribute(u8 fontId, u8 attributeId);
u8 GetMenuCursorDimensionByFont(u8 fontId, u8 whichDimension);
u16 FontFunc_Small(struct TextPrinter *textPrinter);
u16 FontFunc_NormalCopy1(struct TextPrinter *textPrinter);
u16 FontFunc_Normal(struct TextPrinter *textPrinter);
u16 FontFunc_NormalCopy2(struct TextPrinter *textPrinter);
u16 FontFunc_Male(struct TextPrinter *textPrinter);
u16 FontFunc_Female(struct TextPrinter *textPrinter);
u16 FontFunc_Braille(struct TextPrinter *textPrinter);
u16 FontFunc_Narrower(struct TextPrinter *);
u16 FontFunc_SmallNarrower(struct TextPrinter *);
s32 GetGlyphWidth_Braille(u16 font_type, bool32 isJapanese);
void sub_80062B0(struct Sprite *sprite);
u8 CreateTextCursorSpriteForOakSpeech(u8 sheetId, u16 x, u16 y, u8 priority, u8 subpriority);
void DestroyTextCursorSprite(u8 spriteId);
u8 GetFontIdToFit(const u8 *string, u8 fontId, s16 letterSpacing, u32 widthPx);
u8 *PrependFontIdToFit(u8 *start, u8 *end, u8 fontId, u32 width);
u8 *WrapFontIdToFit(u8 *start, u8 *end, u8 fontId, u32 width);

#endif // GUARD_TEXT_H

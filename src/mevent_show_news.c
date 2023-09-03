#include "global.h"
#include "gflib.h"
#include "menu_indicators.h"
#include "new_menu_helpers.h"
#include "mevent.h"
#include "mystery_gift_menu.h"
#include "menu.h"
#include "link_rfu.h"

struct UnkStruct_203F3CC
{
    /*0000*/ struct WonderNews wonderNews;
    /*01bc*/ const struct UnkStruct_8467FB8 * bgSpec;
    /*01c0*/ u8 verticalScrollDisabled:1;
    u8 state:7;
    /*01c1*/ u8 menuIndicatorsId;
    /*01c2*/ u8 unk_01C2_0:1;
    u8 unk_01C2_1:7;
    /*01c3*/ u8 scrollDirection:1;
    u8 unk_01C3_1:7;
    /*01c4*/ u16 numMails;
    /*01c6*/ u16 scrollOffset;
    /*01c8*/ u16 windowIds[2];
    /*01cc*/ u8 filler_01CC[2];
    /*01ce*/ u8 title[41];
    /*01f7*/ u8 messages[10][41];
    /*0394*/ struct ScrollArrowsTemplate scrollArrowsTemplate;
    /*03a4*/ u8 buffer_03A4[0x1000];
};

static EWRAM_DATA struct UnkStruct_203F3CC * sWork = NULL;

static void sub_8146980(void);
static void sub_8146A30(void);
static void sub_8146B58(void);

static const u8 sTextPals[][3] = {
    {0, 2, 3},
    {0, 1, 2}
};
static const struct WindowTemplate gUnknown_8468040[] = {
    {0, 1, 0, 28,  3, 15, 0x000},
    {2, 1, 3, 28, 20, 15, 0x000}
};
static const struct ScrollArrowsTemplate sScrollArrowsTemplate = {
    0x02, 0xe8, 0x18, 0x03, 0xe8, 0x98,
    0x0000, 0x0002, 0x1000, 0x1000, 0x0,
};

static const u16 sNews1Pal[] = INCBIN_U16("graphics/mevent/pal_468060.gbapal");
static const u16 sNews6Pal[] = INCBIN_U16("graphics/mevent/pal_468080.gbapal");
static const u16 sNews7Pal[] = INCBIN_U16("graphics/mevent/pal_4680A0.gbapal");
static const u8 sNews0Gfx[] = INCBIN_U8("graphics/mevent/gfx_4680C0.4bpp.lz");
static const u8 sNews0Map[] = INCBIN_U8("graphics/mevent/tilemap_468140.bin.lz");
static const u8 sNews1Gfx[] = INCBIN_U8("graphics/mevent/gfx_46821C.4bpp.lz");
static const u8 sNews1Map[] = INCBIN_U8("graphics/mevent/tilemap_46824C.bin.lz");
static const u8 sNews2Gfx[] = INCBIN_U8("graphics/mevent/gfx_46830C.4bpp.lz");
static const u8 sNews2Map[] = INCBIN_U8("graphics/mevent/tilemap_46837C.bin.lz");
static const u8 sNews6Gfx[] = INCBIN_U8("graphics/mevent/gfx_468448.4bpp.lz");
static const u8 sNews6Map[] = INCBIN_U8("graphics/mevent/tilemap_4684D8.bin.lz");
static const u8 sNews7Gfx[] = INCBIN_U8("graphics/mevent/gfx_4685B4.4bpp.lz");
static const u8 sNews7Map[] = INCBIN_U8("graphics/mevent/tilemap_468644.bin.lz");

static const struct UnkStruct_8467FB8 sBgSpecs[] = {
    {1, 0, 0, 0, sNews0Gfx, sNews0Map, sNews1Pal},
    {1, 0, 0, 0, sNews1Gfx, sNews1Map, gCard1Pal},
    {1, 0, 0, 0, sNews2Gfx, sNews2Map, gCard2Pal},
    {1, 0, 0, 0, sNews2Gfx, sNews2Map, gCard3Pal},
    {1, 0, 0, 0, sNews2Gfx, sNews2Map, gCard4Pal},
    {1, 0, 0, 0, sNews2Gfx, sNews2Map, gCard5Pal},
    {1, 0, 0, 0, sNews6Gfx, sNews6Map, sNews6Pal},
    {1, 0, 0, 0, sNews7Gfx, sNews7Map, sNews7Pal}
};

static void sub_8146980(void)
{
    u8 i = 0;
    memcpy(sWork->title, sWork->wonderNews.titleText, 40);
    sWork->title[40] = EOS;
    for (; i < 10; ++i)
    {
        memcpy(sWork->messages[i], sWork->wonderNews.bodyText[i], 40);
        sWork->messages[i][40] = EOS;
        if (i > 7 && sWork->messages[i][0] != EOS)
            ++sWork->numMails;
    }
    sWork->scrollArrowsTemplate = sScrollArrowsTemplate;
    sWork->scrollArrowsTemplate.fullyDownThreshold = sWork->numMails;
}

static void sub_8146A30(void)
{
    u8 i = 0;
    s32 x;
    PutWindowTilemap(sWork->windowIds[0]);
    PutWindowTilemap(sWork->windowIds[1]);
    FillWindowPixelBuffer(sWork->windowIds[0], 0);
    FillWindowPixelBuffer(sWork->windowIds[1], 0);
    x = (0xe0 - GetStringWidth(3, sWork->title, GetFontAttribute(3, 2))) / 2;
    if (x < 0)
        x = 0;
    AddTextPrinterParameterized3(sWork->windowIds[0], 3, x, 6, sTextPals[sWork->bgSpec->textPal1], 0, sWork->title);
    for (; i < 10; ++i)
    {
        AddTextPrinterParameterized3(sWork->windowIds[1], 3, 0, 16 * i + 2, sTextPals[sWork->bgSpec->textPal2], 0, sWork->messages[i]);
    }
    CopyWindowToVram(sWork->windowIds[0], COPYWIN_BOTH);
    CopyWindowToVram(sWork->windowIds[1], COPYWIN_BOTH);
}

static void sub_8146B58(void)
{
    u16 r4 = sWork->unk_01C2_1;
    r4 <<= 8;
    if (sWork->scrollDirection)
    {
        ChangeBgY(2, r4, 1);
        ChangeBgY(3, r4, 1);
    }
    else
    {
        ChangeBgY(2, r4, 2);
        ChangeBgY(3, r4, 2);
    }
    sWork->unk_01C3_1 += sWork->unk_01C2_1;
    if (sWork->unk_01C3_1 > 15)
    {
        if (sWork->scrollDirection)
            ++sWork->scrollOffset;
        else
            --sWork->scrollOffset;
        sWork->unk_01C2_0 = FALSE;
        sWork->unk_01C3_1 = 0;
    }
}

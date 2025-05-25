#include "global.h"
#include "gflib.h"
#include "menu.h"
#include "list_menu.h"
#include "menu_indicators.h"
#include "new_menu_helpers.h"
#include "text_window.h"
#include "task.h"
#include "graphics.h"
#include "strings.h"
#include "pokemon_icon.h"
#include "constants/songs.h"

struct ListMenuOverride
{
    u8 cursorPal:4;
    u8 fillValue:4;
    u8 fontId:7;
    u8 enabled:1;
    u16 cursorShadowPal:4;
    u16 lettersSpacing:6;
    u16 unused:6;
};

struct MoveMenuInfoIcon
{
    u8 width;
    u8 height;
    u16 offset;
};

struct ListMenuOverride gListMenuOverride;
struct ListMenuTemplate gMultiuseListMenuTemplate;

static u32 ListMenuInitInternal(const struct ListMenuTemplate *listMenuTemplate, u16 cursorPos, u16 itemsAbove);
static void ListMenuChangeSelection(struct ListMenu *list, bool32 updateCursorAndCallCallback, u32 count, bool32 movingDown);
static void ListMenuPrintEntries(struct ListMenu *list, u16 startIndex, u16 yOffset, u16 count);
static void ListMenuDrawCursor(struct ListMenu *list);
static void ListMenuCallSelectionChangedCallback(struct ListMenu *list, bool32 onInit);
static u32 ListMenuAddCursorObject(struct ListMenu *list, u32 cursorKind);

const struct MoveMenuInfoIcon gMoveMenuInfoIcons[] =
{
    { 12, 12, 0x00 },       // Unused
    { 32, 12, 0x20 },       // Normal icon
    { 32, 12, 0x64 },       // Fight icon
    { 32, 12, 0x60 },       // Flying icon
    { 32, 12, 0x80 },       // Poison icon
    { 32, 12, 0x48 },       // Ground icon
    { 32, 12, 0x44 },       // Rock icon
    { 32, 12, 0x6C },       // Bug icon
    { 32, 12, 0x68 },       // Ghost icon
    { 32, 12, 0x88 },       // Steel icon
    { 32, 12, 0xA4 },       // ??? (Mystery) icon
    { 32, 12, 0x24 },       // Fire icon
    { 32, 12, 0x28 },       // Water icon
    { 32, 12, 0x2C },       // Grass icon
    { 32, 12, 0x40 },       // Electric icon
    { 32, 12, 0x84 },       // Psychic icon
    { 32, 12, 0x4C },       // Ice icon
    { 32, 12, 0xA0 },       // Dragon icon
    { 32, 12, 0x8C },       // Dark icon
    { 32, 12, 0xA8 },       // Fairy icon
    { 40, 12, 0xC0 },       // -Power- icon
    { 40, 12, 0xC8 },       // -Accuracy- icon
    { 40, 12, 0xE0 },       // -PP- icon
    { 40, 12, 0xE8 },       // -Effect- icon
    { 40, 12, 0x100 },      // -Type- icon
    { 40, 12, 0x108 },      // -Category- icon
    { 32, 13, 0x4 },        // Physical icon
    { 32, 13, 0x8 },        // Special icon
    { 32, 13, 0xC },        // Status icon
    { 16, 8, 0x120 },       // E- IV ranking icon
    { 16, 8, 0x122 },       // E IV ranking icon
    { 16, 8, 0x124 },       // E+ IV ranking icon
    { 16, 8, 0x126 },       // D- IV ranking icon
    { 16, 8, 0x128 },       // D IV ranking icon
    { 16, 8, 0x12A },       // D+ IV ranking icon
    { 16, 8, 0x12C },       // C- IV ranking icon
    { 16, 8, 0x12E },       // C IV ranking icon
    { 16, 8, 0x130 },       // C+ IV ranking icon
    { 16, 8, 0x132 },       // B- IV ranking icon
    { 16, 8, 0x134 },       // B IV ranking icon
    { 16, 8, 0x136 },       // B+ IV ranking icon
    { 16, 8, 0x138 },       // A- IV ranking icon
    { 16, 8, 0x13A },       // A IV ranking icon
    { 16, 8, 0x13C },       // A+ IV ranking icon
    { 16, 8, 0x13E },       // S IV ranking icon
};

static void ListMenuDummyTask(u32 taskId)
{
}

u32 ListMenuInit(const struct ListMenuTemplate *listMenuTemplate, u16 cursorPos, u16 itemsAbove)
{
    u32 taskId = ListMenuInitInternal(listMenuTemplate, cursorPos, itemsAbove);
    PutWindowTilemap(listMenuTemplate->windowId);
    CopyWindowToVram(listMenuTemplate->windowId, COPYWIN_GFX);
    return taskId;
}

u32 ListMenuInitInRect(const struct ListMenuTemplate *listMenuTemplate, const struct ListMenuWindowRect *rect, u16 cursorPos, u16 itemsAbove)
{
    u32 i, taskId = ListMenuInitInternal(listMenuTemplate, cursorPos, itemsAbove);
    
    for (i = 0; rect[i].palNum != 0xFF; i++)
        PutWindowRectTilemapOverridePalette(listMenuTemplate->windowId, rect[i].x, rect[i].y, rect[i].width, rect[i].height, rect[i].palNum);
    
    CopyWindowToVram(listMenuTemplate->windowId, COPYWIN_GFX);
    return taskId;
}

s32 ListMenu_ProcessInput(u32 listTaskId)
{
    struct ListMenu *list = (struct ListMenu *)gTasks[listTaskId].data;

    if (JOY_NEW(A_BUTTON))
        return list->template.items[list->cursorPos + list->itemsAbove].index;
    else if (JOY_NEW(B_BUTTON))
        return LIST_CANCEL;
    else if (gMain.newAndRepeatedKeys & DPAD_UP)
    {
        ListMenuChangeSelection(list, TRUE, 1, FALSE);
        return LIST_NOTHING_CHOSEN;
    }
    else if (gMain.newAndRepeatedKeys & DPAD_DOWN)
    {
        ListMenuChangeSelection(list, TRUE, 1, TRUE);
        return LIST_NOTHING_CHOSEN;
    }
    else // try to move by one window scroll
    {
        bool32 rightButton, leftButton;
        
        switch (list->template.scrollMultiple)
        {
        case LIST_NO_MULTIPLE_SCROLL:
        default:
            leftButton = FALSE;
            rightButton = FALSE;
            break;
        case LIST_MULTIPLE_SCROLL_DPAD:
            leftButton = (gMain.newAndRepeatedKeys & DPAD_LEFT);
            rightButton = (gMain.newAndRepeatedKeys & DPAD_RIGHT);
            break;
        case LIST_MULTIPLE_SCROLL_L_R:
            leftButton = (gMain.newAndRepeatedKeys & L_BUTTON);
            rightButton = (gMain.newAndRepeatedKeys & R_BUTTON);
            break;
        }
        
        if (leftButton)
        {
            ListMenuChangeSelection(list, TRUE, list->template.maxShowed, FALSE);
            return LIST_NOTHING_CHOSEN;
        }
        else if (rightButton)
        {
            ListMenuChangeSelection(list, TRUE, list->template.maxShowed, TRUE);
            return LIST_NOTHING_CHOSEN;
        }
        else
            return LIST_NOTHING_CHOSEN;
    }
}

void DestroyListMenuTask(u32 listTaskId, u16 *cursorPos, u16 *itemsAbove)
{
    struct ListMenu *list = (struct ListMenu *)gTasks[listTaskId].data;

    if (cursorPos != NULL)
        *cursorPos = list->cursorPos;
    if (itemsAbove != NULL)
        *itemsAbove = list->itemsAbove;

    if (list->taskId != TAIL_SENTINEL)
        ListMenuRemoveCursorObject(list->taskId, list->template.cursorKind - 2);

    DestroyTask(listTaskId);
}

void RedrawListMenu(u32 listTaskId)
{
    struct ListMenu *list = (struct ListMenu *)gTasks[listTaskId].data;

    FillWindowPixelBuffer(list->template.windowId, PIXEL_FILL(list->template.fillValue));
    ListMenuPrintEntries(list, list->cursorPos, 0, list->template.maxShowed);
    ListMenuDrawCursor(list);
    CopyWindowToVram(list->template.windowId, COPYWIN_GFX);
}

static void ListMenuGetCurrentItemArrayId(u32 listTaskId, u16 *arrayId)
{
    struct ListMenu *list = (struct ListMenu *)gTasks[listTaskId].data;

    if (arrayId != NULL)
        *arrayId = list->cursorPos + list->itemsAbove;
}

void ListMenuGetScrollAndRow(u32 listTaskId, u16 *cursorPos, u16 *itemsAbove)
{
    struct ListMenu *list = (struct ListMenu *)gTasks[listTaskId].data;

    if (cursorPos != NULL)
        *cursorPos = list->cursorPos;
    if (itemsAbove != NULL)
        *itemsAbove = list->itemsAbove;
}

u16 ListMenuGetYCoordForPrintingArrowCursor(u32 listTaskId)
{
    struct ListMenu *list = (struct ListMenu *)gTasks[listTaskId].data;
    u8 yMultiplier = GetFontAttribute(list->template.fontId, FONTATTR_MAX_LETTER_HEIGHT) + list->template.itemVerticalPadding;

    return list->itemsAbove * yMultiplier + list->template.upText_Y;
}

static u32 ListMenuInitInternal(const struct ListMenuTemplate *listMenuTemplate, u16 cursorPos, u16 itemsAbove)
{
    u32 listTaskId = CreateTask(ListMenuDummyTask, 0);
    struct ListMenu *list = (struct ListMenu *)gTasks[listTaskId].data;

    list->template = *listMenuTemplate;
    list->cursorPos = cursorPos;
    list->itemsAbove = itemsAbove;
    list->taskId = TAIL_SENTINEL;
    
    gListMenuOverride.cursorPal = list->template.cursorPal;
    gListMenuOverride.fillValue = list->template.fillValue;
    gListMenuOverride.cursorShadowPal = list->template.cursorShadowPal;
    gListMenuOverride.lettersSpacing = list->template.lettersSpacing;
    gListMenuOverride.fontId = list->template.fontId;
    gListMenuOverride.enabled = FALSE;
    
    if (list->template.totalItems < list->template.maxShowed)
        list->template.maxShowed = list->template.totalItems;
    
    FillWindowPixelBuffer(list->template.windowId, PIXEL_FILL(list->template.fillValue));
    ListMenuPrintEntries(list, list->cursorPos, 0, list->template.maxShowed);
    ListMenuDrawCursor(list);
    ListMenuCallSelectionChangedCallback(list, TRUE);

    return listTaskId;
}

static void ListMenuPrint(struct ListMenu *list, const u8 *str, u32 x, u32 y)
{
    u8 colors[3];
    
    if (gListMenuOverride.enabled)
    {
        colors[0] = gListMenuOverride.fillValue;
        colors[1] = gListMenuOverride.cursorPal;
        colors[2] = gListMenuOverride.cursorShadowPal;
        AddTextPrinterParameterized4(list->template.windowId, gListMenuOverride.fontId, x, y, gListMenuOverride.lettersSpacing, 0, colors, TEXT_SPEED_FF, str);
        gListMenuOverride.enabled = FALSE;
    }
    else
    {
        colors[0] = list->template.fillValue;
        colors[1] = list->template.cursorPal;
        colors[2] = list->template.cursorShadowPal;
        AddTextPrinterParameterized4(list->template.windowId, list->template.fontId, x, y, list->template.lettersSpacing, 0, colors, TEXT_SPEED_FF, str);
    }
}

static void ListMenuPrintEntries(struct ListMenu *list, u16 startIndex, u16 yOffset, u16 count)
{
    u32 i;
    u8 x, y;
    u8 yMultiplier = GetFontAttribute(list->template.fontId, FONTATTR_MAX_LETTER_HEIGHT) + list->template.itemVerticalPadding;

    for (i = 0; i < count; i++)
    {
        if (list->template.items[startIndex].index != LIST_HEADER)
            x = list->template.item_X;
        else
            x = list->template.header_X;
        
        y = (yOffset + i) * yMultiplier + list->template.upText_Y;
        
        if (list->template.itemPrintFunc != NULL)
            list->template.itemPrintFunc(list->template.windowId, list->template.items[startIndex].index, y);
        
        ListMenuPrint(list, list->template.items[startIndex].label, x, y);
        startIndex++;
    }
}

static void ListMenuDrawCursor(struct ListMenu *list)
{
    u8 yMultiplier = GetFontAttribute(list->template.fontId, FONTATTR_MAX_LETTER_HEIGHT) + list->template.itemVerticalPadding;
    u8 x = list->template.cursor_X;
    u8 y = list->itemsAbove * yMultiplier + list->template.upText_Y;
    
    switch (list->template.cursorKind)
    {
    case 0:
        ListMenuPrint(list, gText_SelectorArrow2, x, y);
        break;
    case 1:
        break;
    case 2:
        if (list->taskId == TAIL_SENTINEL)
            list->taskId = ListMenuAddCursorObject(list, 0);
        
        ListMenuUpdateCursorObject(list->taskId, GetWindowAttribute(list->template.windowId, WINDOW_TILEMAP_LEFT) * 8 - 1, GetWindowAttribute(list->template.windowId, WINDOW_TILEMAP_TOP) * 8 + y - 1, 0);
        break;
    case 3:
        if (list->taskId == TAIL_SENTINEL)
            list->taskId = ListMenuAddCursorObject(list, 1);
        
        ListMenuUpdateCursorObject(list->taskId, GetWindowAttribute(list->template.windowId, WINDOW_TILEMAP_LEFT) * 8 + x, GetWindowAttribute(list->template.windowId, WINDOW_TILEMAP_TOP) * 8 + y, 1);
        break;
    }
}

static u32 ListMenuAddCursorObject(struct ListMenu *list, u32 cursorKind)
{
    struct CursorStruct cursor;

    cursor.left = 0;
    cursor.top = 160;
    cursor.rowWidth = GetWindowAttribute(list->template.windowId, WINDOW_WIDTH) * 8 + 2;
    cursor.rowHeight = GetFontAttribute(list->template.fontId, FONTATTR_MAX_LETTER_HEIGHT) + 2;
    cursor.tileTag = 0x4000;
    cursor.palTag = SPRITE_INVALID_TAG;
    cursor.palNum = 15;
    return ListMenuAddCursorObjectInternal(&cursor, cursorKind);
}

static void ListMenuErasePrintedCursor(struct ListMenu *list, u32 itemsAbove)
{
    if (list->template.cursorKind == 0)
    {
        u8 yMultiplier = GetFontAttribute(list->template.fontId, FONTATTR_MAX_LETTER_HEIGHT) + list->template.itemVerticalPadding;
        
        FillWindowPixelRect(list->template.windowId, PIXEL_FILL(list->template.fillValue), list->template.cursor_X, itemsAbove * yMultiplier + list->template.upText_Y,
                            GetMenuCursorDimensionByFont(list->template.fontId, 0), GetMenuCursorDimensionByFont(list->template.fontId, 1));
    }
}

static u32 ListMenuUpdateSelectedRowIndexAndScrollOffset(struct ListMenu *list, bool32 movingDown)
{
    u16 itemsAbove = list->itemsAbove;
    u16 cursorPos = list->cursorPos;
    u16 newRow;
    u32 newScroll;

    if (!movingDown)
    {
        if (list->template.maxShowed == 1)
            newRow = 0;
        else
            newRow = list->template.maxShowed - ((list->template.maxShowed / 2) + (list->template.maxShowed % 2)) - 1;

        if (cursorPos == 0)
        {
            while (itemsAbove != 0)
            {
                itemsAbove--;
                
                if (list->template.items[cursorPos + itemsAbove].index != LIST_HEADER)
                {
                    list->itemsAbove = itemsAbove;
                    return 1;
                }
            }
            return 0;
        }
        else
        {
            while (itemsAbove > newRow)
            {
                itemsAbove--;
                if (list->template.items[cursorPos + itemsAbove].index != LIST_HEADER)
                {
                    list->itemsAbove = itemsAbove;
                    return 1;
                }
            }
            newScroll = cursorPos - 1;
        }
    }
    else
    {
        if (list->template.maxShowed == 1)
            newRow = 0;
        else
            newRow = ((list->template.maxShowed / 2) + (list->template.maxShowed % 2));

        if (cursorPos == list->template.totalItems - list->template.maxShowed)
        {
            while (itemsAbove < list->template.maxShowed - 1)
            {
                itemsAbove++;
                
                if (list->template.items[cursorPos + itemsAbove].index != LIST_HEADER)
                {
                    list->itemsAbove = itemsAbove;
                    return 1;
                }
            }
            return 0;
        }
        else
        {
            while (itemsAbove < newRow)
            {
                itemsAbove++;
                
                if (list->template.items[cursorPos + itemsAbove].index != LIST_HEADER)
                {
                    list->itemsAbove = itemsAbove;
                    return 1;
                }
            }
            newScroll = cursorPos + 1;
        }
    }
    list->itemsAbove = newRow;
    list->cursorPos = newScroll;
    
    return 2;
}

static void ListMenuScroll(struct ListMenu *list, u32 count, bool32 movingDown)
{
    if (count >= list->template.maxShowed)
    {
        FillWindowPixelBuffer(list->template.windowId, PIXEL_FILL(list->template.fillValue));
        ListMenuPrintEntries(list, list->cursorPos, 0, list->template.maxShowed);
    }
    else
    {
        u8 yMultiplier = GetFontAttribute(list->template.fontId, FONTATTR_MAX_LETTER_HEIGHT) + list->template.itemVerticalPadding;
        
        ScrollWindow(list->template.windowId, movingDown ^ TRUE, count * yMultiplier, PIXEL_FILL(list->template.fillValue));
        
        if (!movingDown)
        {
            u16 y, width, height;

            ListMenuPrintEntries(list, list->cursorPos, 0, count);

            y = (list->template.maxShowed * yMultiplier) + list->template.upText_Y;
            width = GetWindowAttribute(list->template.windowId, WINDOW_WIDTH) * 8;
            height = (GetWindowAttribute(list->template.windowId, WINDOW_HEIGHT) * 8) - y;
            FillWindowPixelRect(list->template.windowId, PIXEL_FILL(list->template.fillValue), 0, y, width, height);
        }
        else
        {
            ListMenuPrintEntries(list, list->cursorPos + (list->template.maxShowed - count), list->template.maxShowed - count, count);
            FillWindowPixelRect(list->template.windowId, PIXEL_FILL(list->template.fillValue), 0, 0, GetWindowAttribute(list->template.windowId, WINDOW_WIDTH) * 8, list->template.upText_Y);
        }
    }
}

static void ListMenuChangeSelection(struct ListMenu *list, bool32 updateCursorAndCallCallback, u32 count, bool32 movingDown)
{
    u32 oldSelectedRow = list->itemsAbove;
    u32 i, cursorCount = 0, selectionChange = 0;

    for (i = 0; i < count; i++)
    {
        do
        {
            u32 ret = ListMenuUpdateSelectedRowIndexAndScrollOffset(list, movingDown);
            
            selectionChange |= ret;
            if (ret != 2)
                break;
            
            cursorCount++;
        }
        while (list->template.items[list->cursorPos + list->itemsAbove].index == LIST_HEADER);
    }

    if (updateCursorAndCallCallback)
    {
        switch (selectionChange)
        {
        case 1:
            ListMenuErasePrintedCursor(list, oldSelectedRow);
            ListMenuDrawCursor(list);
            ListMenuCallSelectionChangedCallback(list, FALSE);
            CopyWindowToVram(list->template.windowId, COPYWIN_GFX);
            break;
        case 2:
        case 3:
            ListMenuErasePrintedCursor(list, oldSelectedRow);
            ListMenuScroll(list, cursorCount, movingDown);
            ListMenuDrawCursor(list);
            ListMenuCallSelectionChangedCallback(list, FALSE);
            CopyWindowToVram(list->template.windowId, COPYWIN_GFX);
            break;
        }
    }
}

static void ListMenuCallSelectionChangedCallback(struct ListMenu *list, bool32 onInit)
{
    if (list->template.moveCursorFunc != NULL)
        list->template.moveCursorFunc(list->template.items[list->cursorPos + list->itemsAbove].index, onInit, list);
}

void ListMenuOverrideSetColors(u32 cursorPal, u32 fillValue, u32 cursorShadowPal)
{
    gListMenuOverride.cursorPal = cursorPal;
    gListMenuOverride.fillValue = fillValue;
    gListMenuOverride.cursorShadowPal = cursorShadowPal;
    gListMenuOverride.enabled = TRUE;
}

void ListMenuDefaultCursorMoveFunc(s32 itemIndex, bool32 onInit, struct ListMenu *list)
{
    if (!onInit)
        PlaySE(SE_SELECT);
}

void ListMenuSetTemplateField(u32 taskId, u32 field, s32 value)
{
    struct ListMenu *data = (struct ListMenu *)gTasks[taskId].data;

    switch (field)
    {
    case LISTFIELD_MOVECURSORFUNC:
    case LISTFIELD_MOVECURSORFUNC2:
        data->template.moveCursorFunc = (void *)value;
        break;
    case LISTFIELD_TOTALITEMS:
        data->template.totalItems = value;
        break;
    case LISTFIELD_MAXSHOWED:
        data->template.maxShowed = value;
        break;
    case LISTFIELD_WINDOWID:
        data->template.windowId = value;
        break;
    case LISTFIELD_HEADERX:
        data->template.header_X = value;
        break;
    case LISTFIELD_ITEMX:
        data->template.item_X = value;
        break;
    case LISTFIELD_CURSORX:
        data->template.cursor_X = value;
        break;
    case LISTFIELD_UPTEXTY:
        data->template.upText_Y = value;
        break;
    case LISTFIELD_CURSORPAL:
        data->template.cursorPal = value;
        break;
    case LISTFIELD_FILLVALUE:
        data->template.fillValue = value;
        break;
    case LISTFIELD_CURSORSHADOWPAL:
        data->template.cursorShadowPal = value;
        break;
    case LISTFIELD_LETTERSPACING:
        data->template.lettersSpacing = value;
        break;
    case LISTFIELD_ITEMVERTICALPADDING:
        data->template.itemVerticalPadding = value;
        break;
    case LISTFIELD_SCROLLMULTIPLE:
        data->template.scrollMultiple = value;
        break;
    case LISTFIELD_FONTID:
        data->template.fontId = value;
        break;
    case LISTFIELD_CURSORKIND:
        data->template.cursorKind = value;
        break;
    }
}

void ListMenuLoadStdPalAt(u32 palOffset, u32 palId)
{
    const u16 *palette;

    switch (palId)
    {
    case 0:
    default:
        palette = gFireRedMenuElements1_Pal;
        break;
    case 1:
        palette = gFireRedMenuElements2_Pal;
        break;
    }
    LoadPalette(palette, palOffset, 0x20);
}

void BlitMoveInfoIcon(u32 windowId, u32 iconId, u32 x, u32 y)
{
    BlitBitmapRectToWindow(windowId, gFireRedMenuElements_Gfx + gMoveMenuInfoIcons[iconId].offset * 32, 0, 0, 128, 128, x, y, gMoveMenuInfoIcons[iconId].width, gMoveMenuInfoIcons[iconId].height);
}

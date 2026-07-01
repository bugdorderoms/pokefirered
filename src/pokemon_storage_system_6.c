#include "global.h"
#include "gflib.h"
#include "menu.h"
#include "new_menu_helpers.h"
#include "pokemon_storage_system_internal.h"
#include "constants/songs.h"

void InitMenu(void)
{
    gPSSData->menuItemsCount = 0;
    gPSSData->menuWidth = 0;
    gPSSData->menuWindow.bg = 0;
    gPSSData->menuWindow.paletteNum = 15;
    gPSSData->menuWindow.baseBlock = 92;
}

static const u8 *const sMenuTexts[] = {
    [PC_TEXT_CANCEL]     = COMPOUND_STRING("Cancel"),
    [PC_TEXT_STORE]      = COMPOUND_STRING("Store"),
    [PC_TEXT_WITHDRAW]   = COMPOUND_STRING("Withdraw"),
    [PC_TEXT_MOVE]       = COMPOUND_STRING("Move"),
    [PC_TEXT_SHIFT]      = COMPOUND_STRING("Shift"),
    [PC_TEXT_PLACE]      = COMPOUND_STRING("Place"),
    [PC_TEXT_SUMMARY]    = COMPOUND_STRING("Summary"),
    [PC_TEXT_RELEASE]    = COMPOUND_STRING("Release"),
    [PC_TEXT_MARK]       = COMPOUND_STRING("Mark"),
    [PC_TEXT_JUMP]       = COMPOUND_STRING("Jump"),
    [PC_TEXT_WALLPAPER]  = COMPOUND_STRING("Wallpaper"),
    [PC_TEXT_NAME]       = COMPOUND_STRING("Name"),
    [PC_TEXT_TAKE]       = COMPOUND_STRING("Take"),
    [PC_TEXT_GIVE]       = COMPOUND_STRING("Give"),
    [PC_TEXT_GIVE2]      = COMPOUND_STRING("Give"),
    [PC_TEXT_SWITCH]     = COMPOUND_STRING("Switch"),
    [PC_TEXT_BAG]        = COMPOUND_STRING("Bag"),
    [PC_TEXT_INFO]       = COMPOUND_STRING("Info"),
    [PC_TEXT_SCENERY1]   = COMPOUND_STRING("Scenery 1"),
    [PC_TEXT_SCENERY2]   = COMPOUND_STRING("Scenery 2"),
    [PC_TEXT_SCENERY3]   = COMPOUND_STRING("Scenery 3"),
    [PC_TEXT_ETCETERA]   = COMPOUND_STRING("Etcetera"),
    [PC_TEXT_FOREST]     = COMPOUND_STRING("Forest"),
    [PC_TEXT_CITY]       = COMPOUND_STRING("City"),
    [PC_TEXT_DESERT]     = COMPOUND_STRING("Desert"),
    [PC_TEXT_SAVANNA]    = COMPOUND_STRING("Savanna"),
    [PC_TEXT_CRAG]       = COMPOUND_STRING("Crag"),
    [PC_TEXT_VOLCANO]    = COMPOUND_STRING("Volcano"),
    [PC_TEXT_SNOW]       = COMPOUND_STRING("Snow"),
    [PC_TEXT_CAVE]       = COMPOUND_STRING("Cave"),
    [PC_TEXT_BEACH]      = COMPOUND_STRING("Beach"),
    [PC_TEXT_SEAFLOOR]   = COMPOUND_STRING("Seafloor"),
    [PC_TEXT_RIVER]      = COMPOUND_STRING("River"),
    [PC_TEXT_SKY]        = COMPOUND_STRING("Sky"),
    [PC_TEXT_POLKADOT]   = COMPOUND_STRING("Stars"),
    [PC_TEXT_POKECENTER] = COMPOUND_STRING("Pokécenter"),
    [PC_TEXT_MACHINE]    = COMPOUND_STRING("Tiles"),
    [PC_TEXT_SIMPLE]     = COMPOUND_STRING("Simple"),
};

void SetMenuText(u32 textId)
{
    if (gPSSData->menuItemsCount < MAX_MENU_ITEMS)
    {
        u8 len;
        struct StorageMenu *menu = &gPSSData->menuItems[gPSSData->menuItemsCount];

        menu->text = sMenuTexts[textId];
        menu->textId = textId;
        len = StringLength(menu->text);
        if (len > gPSSData->menuWidth)
            gPSSData->menuWidth = len;

        gPSSData->menuItemsCount++;
    }
}

s8 sub_8094E50(u32 arg0)
{
    if (arg0 >= gPSSData->menuItemsCount)
        return -1;
    else
        return gPSSData->menuItems[arg0].textId;
}

void AddMenu(void)
{
    gPSSData->menuWindow.width = gPSSData->menuWidth + 2;
    gPSSData->menuWindow.height = 2 * gPSSData->menuItemsCount;
    gPSSData->menuWindow.tilemapLeft = 29 - gPSSData->menuWindow.width;
    gPSSData->menuWindow.tilemapTop = 15 - gPSSData->menuWindow.height;
    gPSSData->field_CB0 = AddWindow(&gPSSData->menuWindow);
    ClearWindowTilemap(gPSSData->field_CB0);
    DrawStdFrameWithCustomTileAndPalette(gPSSData->field_CB0, FALSE, 0x00b, 14);
    PrintTextArray(gPSSData->field_CB0, 1, 8, 2, 16, gPSSData->menuItemsCount, (void*)gPSSData->menuItems);
    Menu_InitCursor(gPSSData->field_CB0, 1, 0, 2, 16, gPSSData->menuItemsCount, 0);
    ScheduleBgCopyTilemapToVram(0);
    gPSSData->field_CAE = 0;
}

s16 sub_8094F94(void)
{
    s32 textId = -2;

    do
    {
        if (JOY_NEW(A_BUTTON))
        {
            textId = Menu_GetCursorPos();
            break;
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            textId = -1;
        }

        if (JOY_NEW(DPAD_UP))
        {
            PlaySE(SE_SELECT);
            Menu_MoveCursor(-1);
        }
        else if (JOY_NEW(DPAD_DOWN))
        {
            PlaySE(SE_SELECT);
            Menu_MoveCursor(1);
        }
    } while (0);

    if (textId != -2)
        sub_8095024();

    if (textId >= 0)
        textId = gPSSData->menuItems[textId].textId;

    return textId;
}

void sub_8095024(void)
{
    ClearStdWindowAndFrameToTransparent(gPSSData->field_CB0, TRUE);
    RemoveWindow(gPSSData->field_CB0);
}

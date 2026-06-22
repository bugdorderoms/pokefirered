#include "battle.h"
#include "field_specials.h"
#include "gflib.h"
#include "global.h"
#include "item.h"
#include "list_menu.h"
#include "menu.h"
#include "menu_helpers.h"
#include "menu_indicators.h"
#include "new_menu_helpers.h"
#include "overworld.h"
#include "pokemon.h"
#include "raid_intro.h"
#include "scanline_effect.h"
#include "strings.h"
#include "task.h"
#include "trainer_pokemon_sprites.h"
#include "constants/songs.h"

enum
{
    WIN_ITEM_DESCRIPTION,
    WIN_DEFEATED,
    WIN_INSTRUCTIONS,
    WIN_DROP_LIST
};

struct RaidClearScreen
{
    u32 *tilemapBuffer;
    struct ListMenuItem *dropsList;
    u16 species;
    u8 numStars;
    u8 listTaskId;
    u16 cursorPos;
    u16 itemsAbove;
    bool8 bossWasCaught;
};

static void CB2_InitRaidClearScreen(void);
static void CreateRaidClearScreenMonSprite(void);
static void CreateRaidClearScreenStarSprites(void);
static void PrintRaidClearInstructionsText(void);
static void PrintRaidClearDefeatedOrCaughtText(void);
static void CreateRaidClearDropsList(void);
static void RaidClearDisplayDropItemDescription(u32 index);
static void Task_RaidClearScreenWaitFadeIn(u32 taskId);
static void Task_RaidClearScreenWaitFanfare(u32 taskId);
static void Task_RaidClearScreen_HandleInput(u32 taskId);
static void Task_RaidClearScreen_ItemDescriptionInput(u32 taskId);

static EWRAM_DATA struct RaidClearScreen *sRaidClearScreenData = NULL;

static const u8 sTextColors[][3] =
{
    {0, 1, 2},
    {0, 2, 3},
    {0, 3, 2}
};

static const struct BgTemplate sRaidClearScreenBgTemplates[] =
{
    { // Text
        .bg = 0,
        .charBaseIndex = 1,
        .mapBaseIndex = 28,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0,
    },
    { // Background
        .bg = 1,
        .charBaseIndex = 0,
        .mapBaseIndex = 6,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0,
    },
    { // Item description
        .bg = 2,
        .charBaseIndex = 2,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    }
};

static const struct WindowTemplate sRaidBattleClearWinTemplates[] =
{
    [WIN_ITEM_DESCRIPTION] =
    {
        .bg = 2,
        .tilemapLeft = 1,
        .tilemapTop = 14,
        .width = 28,
        .height = 5,
        .paletteNum = 14,
        .baseBlock = 1,
    },
    [WIN_DEFEATED] =
    {
        .bg = 0,
        .tilemapLeft = 12,
        .tilemapTop = 0,
        .width = 18,
        .height = 2,
        .paletteNum = 14,
        .baseBlock = 141,
    },
    [WIN_INSTRUCTIONS] =
    {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 18,
        .width = 15,
        .height = 2,
        .paletteNum = 14,
        .baseBlock = 177,
    },
    [WIN_DROP_LIST] =
    {
        .bg = 0,
        .tilemapLeft = 14,
        .tilemapTop = 3,
        .width = 16,
        .height = 16,
        .paletteNum = 14,
        .baseBlock = 207,
    },
    DUMMY_WIN_TEMPLATE
};

////////////////////
// INITIALIZATION //
////////////////////

static void VBlankCB_RaidClearScreen(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void CB2_RaidClearScreen(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

void InitRaidClearScreen(u32 species, u32 numStars, bool32 bossCaptured)
{
    sRaidClearScreenData = AllocZeroed(sizeof(*sRaidClearScreenData));
    sRaidClearScreenData->tilemapBuffer = AllocZeroed(BG_SCREEN_SIZE);
    sRaidClearScreenData->species = species;
    sRaidClearScreenData->numStars = numStars;
    sRaidClearScreenData->bossWasCaught = bossCaptured;
    sRaidClearScreenData->listTaskId = 0xFF;
    
    SetMainCallback2(CB2_InitRaidClearScreen);
}

static void CB2_InitRaidClearScreen(void)
{
    switch (gMain.state)
    {
        case 0:
            SetVBlankCallback(NULL);
            ResetVramOamAndBgCntRegs();
            SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
            break;
        case 1:
            ScanlineEffect_Stop();
            ResetTasks();
            ResetSpriteData();
            ResetTempTileDataBuffers();
            ResetPaletteFade();
            FreeAllSpritePalettes();
            break;
        case 2:
            ResetBgsAndClearDma3BusyFlags(FALSE);
            InitBgsFromTemplates(0, sRaidClearScreenBgTemplates, ARRAY_COUNT(sRaidClearScreenBgTemplates));
            SetBgTilemapBuffer(1, sRaidClearScreenData->tilemapBuffer);
            break;
        case 3:
            DecompressAndCopyTileDataToVram(1, &gRaidIntroBgGfx, 0, 0, 0);
            LZDecompressWram(gRaidClearBgMap, sRaidClearScreenData->tilemapBuffer);
            LoadCompressedPalette(gRaidIntroBgPal, 0, 0x20);
            break;
        case 4:
            ListMenuLoadStdPalAt(0xC0, 1);
            break;
        case 5:
            if (IsDma3ManagerBusyWithBgCopy())
                return;
            break;
        case 6:
            ShowBg(0);
            ShowBg(1);
            CopyBgTilemapBufferToVram(1);
            break;
        case 7:
            InitWindows(sRaidBattleClearWinTemplates);
            DeactivateAllTextPrinters();
            break;
        case 8:
            PrintRaidClearInstructionsText();
            PrintRaidClearDefeatedOrCaughtText();
            break;
        case 9:
            LoadStdWindowFrameGfx();
            DrawStdWindowFrame(WIN_ITEM_DESCRIPTION, TRUE);
            break;
        case 10:
            BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
            break;
        case 11:
            CreateRaidClearScreenMonSprite();
            CreateRaidClearScreenStarSprites();
            break;
        case 12:
            CreateRaidClearDropsList();
            break;
        case 13:
            SetVBlankCallback(VBlankCB_RaidClearScreen);
            SetMainCallback2(CB2_RaidClearScreen);

            CreateTask(Task_RaidClearScreenWaitFadeIn, 0);
            return;
    }
    gMain.state++;
}

static void Task_RaidClearScreenWaitFadeIn(u32 taskId)
{
    if (!gPaletteFade.active)
    {
        Overworld_PlaySpecialMapMusic();
        PlayFanfare(MUS_EVOLVED);
        gTasks[taskId].func = Task_RaidClearScreenWaitFanfare;
    }
}

static void Task_RaidClearScreenWaitFanfare(u32 taskId)
{
    if (IsFanfareTaskInactive())
        gTasks[taskId].func = Task_RaidClearScreen_HandleInput;
}

static void Task_RaidClearScreenWaitFadeOut(u32 taskId)
{
    if (!gPaletteFade.active)
    {
        FREE_IF_NOT_NULL(sRaidClearScreenData->tilemapBuffer);
        FREE_IF_NOT_NULL(sRaidClearScreenData->dropsList);
        FREE_IF_NOT_NULL(sRaidClearScreenData);

        SetMainCallback2(CB2_ReturnToFieldContinueScript);
        
        FreeAllWindowBuffers();
        DestroyTask(taskId);
    }
}

/////////////
// SPRITES //
/////////////

static void CreateRaidClearScreenMonSprite(void)
{
    u32 species = sRaidClearScreenData->species;
    gSprites[CreateMonPicSprite(species, FALSE, 0xFFFFFFFF, TRUE, 50, 77, 0, species)].oam.priority = 1;
}

static void CreateRaidClearScreenStarSprites(void)
{
    u32 i;
    
    LoadSymbolsIconGraphics();
    
    for (i = 0; i < sRaidClearScreenData->numStars; i++)
        Create8x8SymbolSprite(9 + (9 * i), 7, 0, SYMBOL_WHITESTAR);
}

//////////
// TEXT //
//////////

static void RaidClearPrintInfoText(u32 windowId, u32 x, u32 y, const u8 *str)
{
    FillWindowPixelBuffer(windowId, PIXEL_FILL(0));
    AddTextPrinterParameterized3(windowId, 0, x, y, sTextColors[0], 0, str);
    PutWindowTilemap(windowId);
    CopyWindowToVram(windowId, COPYWIN_BOTH);
}

static void PrintRaidClearInstructionsText(void)
{
    RaidClearPrintInfoText(WIN_INSTRUCTIONS, 0, 4, COMPOUND_STRING("{DPAD_UPDOWN}Move {SELECT_BUTTON}Info {A_BUTTON}{B_BUTTON}Quit"));
}

static inline s32 GetDefeatedStringRightAlignXOffset(const u8 *str, s32 totalWidth)
{
    s32 stringWidth = GetStringWidth(0, str, 0);
    
    if (totalWidth > stringWidth)
        return totalWidth - stringWidth;
    else
        return 0;
}

static void PrintRaidClearDefeatedOrCaughtText(void)
{
    GetSpeciesName(gStringVar1, sRaidClearScreenData->species);
    StringExpandPlaceholders(gStringVar4, sRaidClearScreenData->bossWasCaught ? COMPOUND_STRING("You caught {STR_VAR_1}!") : COMPOUND_STRING("You defeated {STR_VAR_1}!"));
    RaidClearPrintInfoText(WIN_DEFEATED, GetDefeatedStringRightAlignXOffset(gStringVar4, 144), 0, gStringVar4);
}

static void RaidClearDisplayDropItemDescription(u32 index)
{
    ReformatStringToMaxChars(ItemId_GetDescription(gRaidBattleDrops[index].itemId), 0, 43, FALSE);
    FillWindowPixelBuffer(WIN_ITEM_DESCRIPTION, PIXEL_FILL(1));
    AddTextPrinterParameterized3(WIN_ITEM_DESCRIPTION, 0, 1, 0, sTextColors[1], 0, gStringVar4);
}

////////////////
// DROPS LIST //
////////////////

static void RaidDropsListMenuItemPrintFunc(u32 windowId, u32 index, u32 y)
{
    ConvertIntToDecimalStringN(gStringVar1, gRaidBattleDrops[index].quantity, STR_CONV_MODE_RIGHT_ALIGN, 2);
    StringExpandPlaceholders(gStringVar4, gText_TimesStrVar1);
    AddTextPrinterParameterized3(windowId, 0, 108, y, sTextColors[1], 0, gStringVar4);
}

static void CreateRaidClearDropsList(void)
{
    u32 i, count = 0;
    struct ListMenuItem drops[MAX_RAID_DROPS];
    
    for (i = 0; i < MAX_RAID_DROPS; i++)
    {
        if (!gRaidBattleDrops[i].itemId)
            break;
        
        drops[count].label = ItemId_GetName(gRaidBattleDrops[i].itemId);
        drops[count].index = i;
        count++;
    }

    if (count > 0)
    {
        u32 arrowTaskId, maxShowed = count > 8 ? 8 : count;
        
        sRaidClearScreenData->dropsList = Alloc(sizeof(struct ListMenuItem) * count);
        memcpy(sRaidClearScreenData->dropsList, drops, sizeof(struct ListMenuItem) * count);

        gMultiuseListMenuTemplate.items = sRaidClearScreenData->dropsList;
        gMultiuseListMenuTemplate.totalItems = count;
        gMultiuseListMenuTemplate.windowId = WIN_DROP_LIST;
        gMultiuseListMenuTemplate.header_X = 0;
        gMultiuseListMenuTemplate.item_X = 8;
        gMultiuseListMenuTemplate.cursor_X = 0;
        gMultiuseListMenuTemplate.lettersSpacing = 0;
        gMultiuseListMenuTemplate.itemVerticalPadding = 3;
        gMultiuseListMenuTemplate.upText_Y = 0;
        gMultiuseListMenuTemplate.maxShowed = maxShowed;
        gMultiuseListMenuTemplate.fontId = 0;
        gMultiuseListMenuTemplate.cursorPal = 2;
        gMultiuseListMenuTemplate.fillValue = 0;
        gMultiuseListMenuTemplate.cursorShadowPal = 3;
        gMultiuseListMenuTemplate.moveCursorFunc = ListMenuDefaultCursorMoveFunc;
        gMultiuseListMenuTemplate.itemPrintFunc = RaidDropsListMenuItemPrintFunc;
        gMultiuseListMenuTemplate.cursorKind = 0;
        gMultiuseListMenuTemplate.scrollMultiple = 0;
        
        sRaidClearScreenData->listTaskId = ListMenuInit(&gMultiuseListMenuTemplate, NULL, NULL);
        
        arrowTaskId = AddScrollIndicatorArrowPairParameterized(SCROLL_ARROW_UP, 176, 22, 150, count - maxShowed, 110, 110, &sRaidClearScreenData->cursorPos);
        SetScrollIndicatorArrowPairSpritePriority(arrowTaskId, 1);
        
        CopyWindowToVram(WIN_DROP_LIST, COPYWIN_BOTH);
    }
}

static void ChangeSelectionArrowCursor(bool32 selectedItem)
{
    AddTextPrinterParameterized3(WIN_DROP_LIST, 0, 0, ListMenuGetYCoordForPrintingArrowCursor(sRaidClearScreenData->listTaskId), sTextColors[selectedItem ? 2 : 1], 0, gText_SelectorArrow2);
}

///////////
// INPUT //
///////////

static void Task_RaidClearScreen_HandleInput(u32 taskId)
{
    if (JOY_NEW(A_BUTTON | B_BUTTON))
    {
        PlaySE(SE_SELECT);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_RaidClearScreenWaitFadeOut;
    }
    else
    {
        if (sRaidClearScreenData->listTaskId != 0xFF)
        {
            ListMenu_ProcessInput(sRaidClearScreenData->listTaskId);
            ListMenuGetScrollAndRow(sRaidClearScreenData->listTaskId, &sRaidClearScreenData->cursorPos, &sRaidClearScreenData->itemsAbove);
            
            if (JOY_NEW(SELECT_BUTTON))
            {
                PlaySE(SE_WIN_OPEN);
                ChangeSelectionArrowCursor(TRUE);
                RaidClearDisplayDropItemDescription(sRaidClearScreenData->dropsList[sRaidClearScreenData->cursorPos + sRaidClearScreenData->itemsAbove].index);
                ShowBg(2);
                gTasks[taskId].func = Task_RaidClearScreen_ItemDescriptionInput;
            }
        }
    }
}

static void Task_RaidClearScreen_ItemDescriptionInput(u32 taskId)
{
    if (JOY_NEW(A_BUTTON | B_BUTTON | SELECT_BUTTON))
    {
        PlaySE(SE_WIN_OPEN);
        ChangeSelectionArrowCursor(FALSE);
        HideBg(2);
        gTasks[taskId].func = Task_RaidClearScreen_HandleInput;
    }
}

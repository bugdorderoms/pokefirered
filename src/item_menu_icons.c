#include "global.h"
#include "gflib.h"
#include "menu.h"
#include "item.h"
#include "decompress.h"
#include "graphics.h"
#include "item_menu_icons.h"
#include "event_data.h"
#include "new_menu_helpers.h"
#include "overworld.h"
#include "constants/items.h"

static EWRAM_DATA u8 sItemMenuIconSpriteIds[12] = {0};
static EWRAM_DATA void * sItemIconTilesBuffer = NULL;
static EWRAM_DATA void * sItemIconTilesBufferPadded = NULL;

static void SpriteCB_BagVisualSwitchingPockets(struct Sprite * sprite);
static void SpriteCB_ShakeBagSprite(struct Sprite * sprite);

static const struct OamData sOamData_BagOrSatchel = {
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .shape = ST_OAM_SQUARE,
    .size = 3,
    .priority = 1,
    .paletteNum = 0
};

static const union AnimCmd sAnim_BagOrSatchel_OpenItemPocket[] = {
    ANIMCMD_FRAME(   0, 5),
    ANIMCMD_FRAME(0x40, 0),
    ANIMCMD_END
};

static const union AnimCmd sAnim_BagOrSatchel_OpenKeyItemPocket[] = {
    ANIMCMD_FRAME(   0, 5),
    ANIMCMD_FRAME(0x80, 0),
    ANIMCMD_END
};

static const union AnimCmd sAnim_BagOrSatchel_OpenPokeBallsPocket[] = {
    ANIMCMD_FRAME(   0, 5),
    ANIMCMD_FRAME(0xc0, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sAnimTable_BagOrSatchel[] = {
    sAnim_BagOrSatchel_OpenKeyItemPocket,
    sAnim_BagOrSatchel_OpenPokeBallsPocket,
    sAnim_BagOrSatchel_OpenItemPocket
};

static const union AffineAnimCmd sAffineAnim_Idle[] = {
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd sAffineAnim_Wobble[] = {
    AFFINEANIMCMD_FRAME(0, 0, -2, 2),
    AFFINEANIMCMD_FRAME(0, 0,  2, 4),
    AFFINEANIMCMD_FRAME(0, 0, -2, 4),
    AFFINEANIMCMD_FRAME(0, 0,  2, 2),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const sAffineAnimTable_BagOrSatchel[] = {
    sAffineAnim_Idle,
    sAffineAnim_Wobble
};

const struct CompressedSpriteSheet gSpriteSheet_Backpack = {
    gUnknown_8E8362C,
    0x2000,
    100
};

const struct CompressedSpriteSheet gSpriteSheet_Satchel = {
    gUnknown_8E83DBC,
    0x2000,
    100
};

const struct CompressedSpritePalette gSpritePalette_BagOrSatchel = {
    gUnknown_8E84560,
    100
};

static const struct SpriteTemplate sSpriteTemplate_BagOrSatchel = {
    100,
    100,
    &sOamData_BagOrSatchel,
    sAnimTable_BagOrSatchel,
    NULL,
    sAffineAnimTable_BagOrSatchel,
    SpriteCallbackDummy
};

static const struct OamData gUnknown_83D4214 = {
    .affineMode = ST_OAM_AFFINE_OFF,
    .shape = ST_OAM_SQUARE,
    .size = 1,
    .priority = 1,
    .paletteNum = 1
};

static const union AnimCmd gUnknown_83D421C[] = {
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd gUnknown_83D4224[] = {
    ANIMCMD_FRAME(4, 0),
    ANIMCMD_END
};

static const union AnimCmd gUnknown_83D422C[] = {
    ANIMCMD_FRAME(0, 0, .hFlip = TRUE),
    ANIMCMD_END
};

static const union AnimCmd *const gUnknown_83D4234[] = {
    gUnknown_83D421C,
    gUnknown_83D4224,
    gUnknown_83D422C
};

const struct CompressedSpriteSheet gBagSwapSpriteSheet = {
    gFile_graphics_interface_bag_swap_sheet,
    0x100,
    101
};

const struct CompressedSpritePalette gBagSwapSpritePalette = {
    gFile_graphics_interface_bag_swap_palette,
    101
};

static const struct SpriteTemplate gUnknown_83D4250 = {
    101,
    101,
    &gUnknown_83D4214,
    gUnknown_83D4234,
    NULL,
    gDummySpriteAffineAnimTable,
    SpriteCallbackDummy
};

static const struct OamData sOamData_ItemIcon = {
    .affineMode = ST_OAM_AFFINE_OFF,
    .shape = ST_OAM_SQUARE,
    .size = 2,
    .priority = 1,
    .paletteNum = 2
};

static const union AnimCmd sAnim_ItemIcon_0[] = {
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sAnimTable_ItemIcon[] = {
    sAnim_ItemIcon_0
};

static const struct SpriteTemplate sSpriteTemplate_ItemIcon = {
    ITEMICON_TAG,
    ITEMICON_TAG,
    &sOamData_ItemIcon,
    sAnimTable_ItemIcon,
    NULL,
    gDummySpriteAffineAnimTable,
    SpriteCallbackDummy
};

static const union AffineAnimCmd sSpriteAffineAnim_KeyItemTM[] =
{
    AFFINEANIMCMD_FRAME(0, 0, 128, 1), // Start rotated left
    AFFINEANIMCMD_FRAME(16, 16, -8, 16), // Double sprite size + rotate right
    AFFINEANIMCMD_FRAME(0, 0, -3, 8), // End at right 24
    AFFINEANIMCMD_FRAME(0, 0, 3, 16), // End at left 24
    AFFINEANIMCMD_FRAME(0, 0, -3, 16), // End at right 24
    AFFINEANIMCMD_FRAME(0, 0, 3, 16), // End at left 24
    AFFINEANIMCMD_FRAME(0, 0, -3, 8), // End at 0
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd* const sSpriteAffineAnimTable_KeyItemTM[] =
{
    sSpriteAffineAnim_KeyItemTM,
};

void ResetItemMenuIconState(void)
{
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sItemMenuIconSpriteIds); i++)
        sItemMenuIconSpriteIds[i] = 0xFF;
}

void CreateBagOrSatchelSprite(u32 animNum)
{
    sItemMenuIconSpriteIds[0] = CreateSprite(&sSpriteTemplate_BagOrSatchel, 40, 68, 0);
    SetBagVisualPocketId(animNum);
}

void SetBagVisualPocketId(u32 animNum)
{
    struct Sprite * sprite = &gSprites[sItemMenuIconSpriteIds[0]];
    sprite->y2 = -5;
    sprite->callback = SpriteCB_BagVisualSwitchingPockets;
    StartSpriteAnim(sprite, animNum);
}

static void SpriteCB_BagVisualSwitchingPockets(struct Sprite * sprite)
{
    if (sprite->y2 != 0)
        sprite->y2++;
    else
        sprite->callback = SpriteCallbackDummy;
}

void ShakeBagSprite(void)
{
    struct Sprite * sprite = &gSprites[sItemMenuIconSpriteIds[0]];
    
    if (sprite->affineAnimEnded)
    {
        StartSpriteAffineAnim(sprite, 1);
        sprite->callback = SpriteCB_ShakeBagSprite;
    }
}

static void SpriteCB_ShakeBagSprite(struct Sprite * sprite)
{
    if (sprite->affineAnimEnded)
    {
        StartSpriteAffineAnim(sprite, 0);
        sprite->callback = SpriteCallbackDummy;
    }
}

void ItemMenuIcons_CreateInsertIndicatorBarHidden(void)
{
    u32 i;
    u8 * ptr = &sItemMenuIconSpriteIds[1];

    for (i = 0; i < 9; i++)
    {
        ptr[i] = CreateSprite(&gUnknown_83D4250, i * 16 + 0x60, 7, 0);
        
        switch (i)
        {
        case 0:
            break;
        case 8:
            StartSpriteAnim(&gSprites[ptr[i]], 2);
            break;
        default:
            StartSpriteAnim(&gSprites[ptr[i]], 1);
            break;
        }
        gSprites[ptr[i]].invisible = TRUE;
    }
}

void ItemMenuIcons_ToggleInsertIndicatorBarVisibility(bool32 invisible)
{
    u32 i;
    u8 * ptr = &sItemMenuIconSpriteIds[1];

    for (i = 0; i < 9; i++)
        gSprites[ptr[i]].invisible = invisible;
}

void ItemMenuIcons_MoveInsertIndicatorBar(s16 x, u32 y)
{
    u32 i;
    u8 * ptr = &sItemMenuIconSpriteIds[1];

    for (i = 0; i < 9; i++)
    {
        gSprites[ptr[i]].x2 = x;
        gSprites[ptr[i]].y = y + 7;
    }
}

static bool32 TryAllocItemIconTilesBuffers(void)
{
    void ** ptr1, ** ptr2;

    ptr1 = &sItemIconTilesBuffer;
    *ptr1 = Alloc(0x120);
    if (*ptr1 == NULL)
        return FALSE;
    ptr2 = &sItemIconTilesBufferPadded;
    *ptr2 = AllocZeroed(0x200);
    if (*ptr2 == NULL)
    {
        Free(*ptr1);
        return FALSE;
    }
    return TRUE;
}

static void CopyItemIconPicTo4x4Buffer(const u32 * src, u32 * dest)
{
    u32 tileX, tileY, row;
    u32 destOffset1, destOffset2;

    for (tileY = 0; tileY < 3; tileY++)
    {
        for (tileX = 0; tileX < 3; tileX++)
        {
            const u32 *srcTile = &src[(tileY * 3 + tileX) * 8];

            for (row = 0; row < 8; row++)
            {
                u32 pixelRow = srcTile[row];
                u32 destY = (tileY * 8) + row + 4;
                
                u32 destTileY = destY >> 3;
                u32 destRow = destY & 7;

                destOffset1 = (destTileY * 4 + tileX) * 8 + destRow;
                dest[destOffset1] |= (pixelRow << 16);

                destOffset2 = (destTileY * 4 + (tileX + 1)) * 8 + destRow;
                dest[destOffset2] |= (pixelRow >> 16);
            }
        }
    }
}

u32 AddItemIconObject(u32 tilesTag, u32 paletteTag, u32 itemId)
{
    return AddItemIconObjectWithCustomObjectTemplate(&sSpriteTemplate_ItemIcon, tilesTag, paletteTag, itemId);
}

u32 AddItemIconObjectWithCustomObjectTemplate(const struct SpriteTemplate * origTemplate, u32 tilesTag, u32 paletteTag, u32 itemId)
{
    struct SpriteTemplate template;
    struct SpriteSheet spriteSheet;
    struct CompressedSpritePalette spritePalette;
    u32 spriteId;

    if (!TryAllocItemIconTilesBuffers())
        return MAX_SPRITES;

    LZDecompressWram(GetItemIconPic(itemId), sItemIconTilesBuffer);
    CopyItemIconPicTo4x4Buffer(sItemIconTilesBuffer, sItemIconTilesBufferPadded);
    spriteSheet.data = sItemIconTilesBufferPadded;
    spriteSheet.size = 0x200;
    spriteSheet.tag = tilesTag;
    LoadSpriteSheet(&spriteSheet);

    spritePalette.data = GetItemIconPalette(itemId);
    spritePalette.tag = paletteTag;
    LoadCompressedSpritePalette(&spritePalette);

    CpuCopy16(origTemplate, &template, sizeof(struct SpriteTemplate));
    template.tileTag = tilesTag;
    template.paletteTag = paletteTag;
    spriteId = CreateSprite(&template, 0, 0, 0);

    Free(sItemIconTilesBuffer);
    Free(sItemIconTilesBufferPadded);
    return spriteId;
}

void DestroyItemIconObj(struct Sprite *sprite, u32 tilesTag, u32 paletteTag)
{
    FreeSpriteTilesByTag(tilesTag);
    FreeSpritePaletteByTag(paletteTag);
    DestroySpriteAndFreeMatrix(sprite);
}

void CreateItemMenuIcon(u32 itemId, u32 idx)
{
    u8 * ptr = &sItemMenuIconSpriteIds[10];
    u32 spriteId;

    if (ptr[idx] == 0xFF)
    {
        FreeSpriteTilesByTag(ITEMICON_TAG + idx);
        FreeSpritePaletteByTag(ITEMICON_TAG + idx);
        
        spriteId = AddItemIconObject(ITEMICON_TAG + idx, ITEMICON_TAG + idx, itemId);
        if (spriteId != MAX_SPRITES)
        {
            ptr[idx] = spriteId;
            gSprites[spriteId].x2 = 20;
            gSprites[spriteId].y2 = 136;
        }
    }
}

void DestroyItemMenuIcon(u32 idx)
{
    u8 * ptr = &sItemMenuIconSpriteIds[10];

    if (ptr[idx] != 0xFF)
    {
        DestroySpriteAndFreeResources(&gSprites[ptr[idx]]);
        ptr[idx] = 0xFF;
    }
}

const u32 *GetItemIconPic(u32 itemId)
{
    if (itemId == ITEMS_COUNT)
        return gItemIcon_ReturnToFieldArrow;
    else
    {
        if (itemId > ITEMS_COUNT)
            itemId = ITEM_NONE;
        
        return gItems[itemId].iconPic;
    }
}

const u32 *GetItemIconPalette(u32 itemId)
{
    if (itemId == ITEMS_COUNT)
        return gItemIconPalette_ReturnToFieldArrow;
    else
    {
        if (itemId > ITEMS_COUNT)
            itemId = ITEM_NONE;
        
        if (ItemId_GetPocket(itemId) == POCKET_TM_CASE)
            return gTypesInfo[gBattleMoves[ItemId_GetHoldEffectParam(itemId)].type].tmPalette;
        else
            return gItems[itemId].iconPalette;
    }
}

void sub_80989A0(u32 itemId, u32 idx)
{
    u8 * ptr = &sItemMenuIconSpriteIds[10];
    u32 spriteId;

    if (ptr[idx] == 0xFF)
    {
        FreeSpriteTilesByTag(ITEMICON_TAG + idx);
        FreeSpritePaletteByTag(ITEMICON_TAG + idx);
        
        spriteId = AddItemIconObject(ITEMICON_TAG + idx, ITEMICON_TAG + idx, itemId);
        if (spriteId != MAX_SPRITES)
        {
            ptr[idx] = spriteId;
            gSprites[spriteId].x2 = 20;
            gSprites[spriteId].y2 = 143;
        }
    }
}

#define IS_KEY_ITEM_TM(pocket) ((pocket == POCKET_KEY_ITEMS || pocket == POCKET_TM_CASE))

static u32 ShowObtainedItemDescription(u32 item)
{
    struct WindowTemplate template;
    u32 textX, textY, windowId, maxChars, windowHeight, numLines;
    
    if (IS_KEY_ITEM_TM(ItemId_GetPocket(item)))
    {
        textX = 1;
        maxChars = 43;
    }
    else
    {
        textX = 28;
        maxChars = 39;
    }
    numLines = ReformatStringToMaxChars(ItemId_GetDescription(item), 0, maxChars, FALSE);
    
    if (numLines == 1)
    {
        textY = 4;
        windowHeight = 3;
    }
    else if (numLines >= 3)
    {
        textY = 0;
        windowHeight = 5;
    }
    else
    {
        textY = 0;
        windowHeight = 4;
    }
    template = SetWindowTemplateFields(0, 1, 1, 28, windowHeight, 14, 0x20);
    windowId = AddWindow(&template);
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    PutWindowTilemap(windowId);
    DrawStdFrameWithCustomTileAndPalette(windowId, FALSE, 0x214, 14);
    AddTextPrinterParameterized(windowId, 0, gStringVar4, textX, textY, 0, NULL);
    GetSetItemObtained(item, FLAG_SET_OBTAINED);
    
    return windowId;
}

void CreateItemIconOnFindMessage(void)
{
    u32 itemId = gSpecialVar_0x8009;
    u32 spriteId = AddItemIconObject(ITEMICON_TAG, ITEMICON_TAG, itemId);
    
    if (spriteId != MAX_SPRITES)
    {
        s16 x, y;
        bool32 itemObtained;
        u32 windowId = 0xFF;
        struct Sprite *sprite = &gSprites[spriteId];
        
        // Handle flash
        if (Overworld_GetFlashLevel() > 0)
            sprite->copyToObjWin = TRUE;
        
        itemObtained = GetSetItemObtained(itemId, FLAG_GET_OBTAINED);

        if (IS_KEY_ITEM_TM(ItemId_GetPocket(itemId)))
        {
            x = 100;
            y = 52;
            
            sprite->oam.affineMode = ST_OAM_AFFINE_DOUBLE;
            sprite->oam.matrixNum = AllocOamMatrix();
            sprite->affineAnims = sSpriteAffineAnimTable_KeyItemTM;
            
            StartSpriteAffineAnim(sprite, 0);

            if (!itemObtained)
                windowId = ShowObtainedItemDescription(itemId);
        }
        else
        {
            if (itemObtained)
            {
                x = 209;
                y = 136;
            }
            else
            {
                x = 22;
                y = 20;
                
                windowId = ShowObtainedItemDescription(itemId);
            }
        }
        sprite->x2 = x;
        sprite->y2 = y;
        sprite->oam.priority = 0;
        sprite->data[0] = windowId;
    }
    gSpecialVar_0x8009 = spriteId; // save sprite id for use later
}

void DestroyItemIconOnFindMessage(void)
{
    struct Sprite *sprite = &gSprites[gSpecialVar_0x8009];
    u32 windowId = sprite->data[0];
    
    DestroyItemIconObj(sprite, ITEMICON_TAG, ITEMICON_TAG);

    if (windowId != 0xFF)
    {
        ClearDialogWindowAndFrame(windowId, TRUE);
        RemoveWindow(windowId);
    }
}

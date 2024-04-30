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

// item graphics
#include "data/item/icons.h"
#include "data/item/icons_table.h"

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

static const u32 sHeldItemGfx[] = INCBIN_U32("graphics/interface/hold_icons.4bpp");
static const u16 sHeldItemPalette[] = INCBIN_U16("graphics/interface/hold_icons.gbapal");

static const struct OamData sOamData_HeldItem =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = 0,
    .bpp = 0,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sSpriteAnim_HeldItem[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_HeldMail[] =
{
    ANIMCMD_FRAME(1, 1),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_HeldItem[] =
{
    sSpriteAnim_HeldItem,
    sSpriteAnim_HeldMail,
};

static const struct SpriteSheet sSpriteSheet_HeldItem =
{
    sHeldItemGfx, sizeof(sHeldItemGfx), ITEMICON_TAG
};

static const struct SpritePalette sSpritePalette_HeldItem =
{
    sHeldItemPalette, ITEMICON_TAG
};

static const struct SpriteTemplate sSpriteTemplate_HeldItem =
{
	.tileTag = ITEMICON_TAG,
    .paletteTag = ITEMICON_TAG,
    .oam = &sOamData_HeldItem,
    .anims = sSpriteAnimTable_HeldItem,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
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
    u16 i;

    for (i = 0; i < ARRAY_COUNT(sItemMenuIconSpriteIds); i++)
        sItemMenuIconSpriteIds[i] = 0xFF;
}

void CreateBagOrSatchelSprite(u8 animNum)
{
    sItemMenuIconSpriteIds[0] = CreateSprite(&sSpriteTemplate_BagOrSatchel, 40, 68, 0);
    SetBagVisualPocketId(animNum);
}

void SetBagVisualPocketId(u8 animNum)
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
    u8 i;
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

void ItemMenuIcons_ToggleInsertIndicatorBarVisibility(bool8 invisible)
{
    u8 i;
    u8 * ptr = &sItemMenuIconSpriteIds[1];

    for (i = 0; i < 9; i++)
    {
        gSprites[ptr[i]].invisible = invisible;
    }
}

void ItemMenuIcons_MoveInsertIndicatorBar(s16 x, u16 y)
{
    u8 i;
    u8 * ptr = &sItemMenuIconSpriteIds[1];

    for (i = 0; i < 9; i++)
    {
        gSprites[ptr[i]].x2 = x;
        gSprites[ptr[i]].y = y + 7;
    }
}

static bool8 TryAllocItemIconTilesBuffers(void)
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

void CopyItemIconPicTo4x4Buffer(const void * src, void * dest)
{
    u8 i;

    for (i = 0; i < 3; i++)
        CpuCopy16(src + 0x60 * i, dest + 0xA0 + 0x80 * i, 0x60);
}

u8 AddItemIconObject(u16 tilesTag, u16 paletteTag, u16 itemId)
{
    return AddItemIconObjectWithCustomObjectTemplate(&sSpriteTemplate_ItemIcon, tilesTag, paletteTag, itemId);
}

u8 AddItemIconObjectWithCustomObjectTemplate(const struct SpriteTemplate * origTemplate, u16 tilesTag, u16 paletteTag, u16 itemId)
{
    struct SpriteTemplate template;
    struct SpriteSheet spriteSheet;
    struct CompressedSpritePalette spritePalette;
    u8 spriteId;

    if (!TryAllocItemIconTilesBuffers())
        return MAX_SPRITES;

    LZDecompressWram(GetItemIconGfxPtr(itemId, 0), sItemIconTilesBuffer);
    CopyItemIconPicTo4x4Buffer(sItemIconTilesBuffer, sItemIconTilesBufferPadded);
    spriteSheet.data = sItemIconTilesBufferPadded;
    spriteSheet.size = 0x200;
    spriteSheet.tag = tilesTag;
    LoadSpriteSheet(&spriteSheet);

    spritePalette.data = GetItemIconGfxPtr(itemId, 1);
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

void CreateItemMenuIcon(u16 itemId, u8 idx)
{
    u8 * ptr = &sItemMenuIconSpriteIds[10];
    u8 spriteId;

    if (ptr[idx] == 0xFF)
    {
        FreeSpriteTilesByTag(ITEMICON_TAG + idx);
        FreeSpritePaletteByTag(ITEMICON_TAG + idx);
        spriteId = AddItemIconObject(ITEMICON_TAG + idx, ITEMICON_TAG + idx, itemId);
        if (spriteId != MAX_SPRITES)
        {
            ptr[idx] = spriteId;
            gSprites[spriteId].x2 = 16;
            gSprites[spriteId].y2 = 132;
        }
    }
}

void DestroyItemMenuIcon(u8 idx)
{
    u8 * ptr = &sItemMenuIconSpriteIds[10];

    if (ptr[idx] != 0xFF)
    {
        DestroySpriteAndFreeResources(&gSprites[ptr[idx]]);
        ptr[idx] = 0xFF;
    }
}

const void * GetItemIconGfxPtr(u16 itemId, u8 attrId)
{
    return sItemIconGfxPtrs[itemId > ITEMS_COUNT ? ITEM_NONE : itemId][attrId];
}

void sub_80989A0(u16 itemId, u8 idx)
{
    u8 * ptr = &sItemMenuIconSpriteIds[10];
    u8 spriteId;

    if (ptr[idx] == 0xFF)
    {
        FreeSpriteTilesByTag(ITEMICON_TAG + idx);
        FreeSpritePaletteByTag(ITEMICON_TAG + idx);
        spriteId = AddItemIconObject(ITEMICON_TAG + idx, ITEMICON_TAG + idx, itemId);
        if (spriteId != MAX_SPRITES)
        {
            ptr[idx] = spriteId;
            gSprites[spriteId].x2 = 16;
            gSprites[spriteId].y2 = 139;
        }
    }
}

void LoadHeldItemIcons(void)
{
    LoadSpriteSheet(&sSpriteSheet_HeldItem);
    LoadSpritePalette(&sSpritePalette_HeldItem);
}

u8 CreateHeldItemSprite(s16 x, s16 y, u8 subpriority, bool8 isMail)
{
	u8 spriteId = CreateSprite(&sSpriteTemplate_HeldItem, x, y, subpriority);
	
	StartSpriteAnim(&gSprites[spriteId], isMail);
	
	return spriteId;
}

#define ITEM_ICON_X 18
#define ITEM_ICON_Y 16

#define IS_KEY_ITEM_TM(pocket) ((pocket == POCKET_KEY_ITEMS || pocket == POCKET_TM_CASE))

static u8 ShowObtainedItemDescription(u16 item)
{
	struct WindowTemplate template;
	s16 textX, textY, maxChars, windowHeight, numLines;
	u8 windowId;
	
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
	struct Sprite *sprite1, *sprite2;
	u16 reg1 = GetGpuReg(REG_OFFSET_DISPCNT), reg2 = GetGpuReg(REG_OFFSET_WINOUT), itemId = gSpecialVar_0x8009;
	s16 x, y;
	u8 spriteId = AddItemIconObject(ITEMICON_TAG, ITEMICON_TAG, itemId), spriteId2, windowId = 0xFF;
	
	// Handle flash
	if (Overworld_GetFlashLevel() > 0)
	{
		SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJWIN_ON);
		SetGpuRegBits(REG_OFFSET_WINOUT, WINOUT_WINOBJ_OBJ);
		
		spriteId2 = AddItemIconObject(ITEMICON_TAG, ITEMICON_TAG, itemId);
	}
	else
		spriteId2 = MAX_SPRITES;
	
	if (spriteId != MAX_SPRITES)
	{
		sprite1 = &gSprites[spriteId];
		
		if (spriteId2 != MAX_SPRITES)
			sprite2 = &gSprites[spriteId2];
		
		if (IS_KEY_ITEM_TM(ItemId_GetPocket(itemId)))
		{
			x = 96;
			y = 48;
			
			sprite1->oam.affineMode = ST_OAM_AFFINE_DOUBLE;
			sprite1->oam.matrixNum = AllocOamMatrix();
			sprite1->affineAnims = sSpriteAffineAnimTable_KeyItemTM;
			
			StartSpriteAffineAnim(sprite1, 0);
			
			if (spriteId2 != MAX_SPRITES)
			{
				sprite2->oam.affineMode = ST_OAM_AFFINE_DOUBLE;
				sprite2->oam.matrixNum = AllocOamMatrix();
				sprite2->affineAnims = sSpriteAffineAnimTable_KeyItemTM;
				
				StartSpriteAffineAnim(sprite2, 0);
			}
			if (!GetSetItemObtained(itemId, FLAG_GET_OBTAINED))
				windowId = ShowObtainedItemDescription(itemId);
		}
		else
		{
			if (GetSetItemObtained(itemId, FLAG_GET_OBTAINED))
			{
				x = 205;
				y = 132;
			}
			else
			{
				x = ITEM_ICON_X;
				y = ITEM_ICON_Y;
				
				windowId = ShowObtainedItemDescription(itemId);
			}
		}
		sprite1->x2 = x;
		sprite1->y2 = y;
		sprite1->oam.priority = 0;
		sprite1->data[0] = windowId;
		sprite1->data[1] = reg1;
		sprite1->data[2] = reg2;
		sprite1->data[3] = spriteId2;
		
		if (spriteId2 != MAX_SPRITES)
		{
			sprite2->x2 = x;
			sprite2->y2 = y;
			sprite2->oam.priority = 0;
			sprite2->oam.objMode = ST_OAM_OBJ_WINDOW;
		}
	}
	gSpecialVar_0x8009 = spriteId; // save sprite id for use later
}

void DestroyItemIconOnFindMessage(void)
{
	u16 reg1, reg2;
	u8 windowId, spriteId2, spriteId = gSpecialVar_0x8009;
	struct Sprite * sprite = &gSprites[spriteId];
	
	windowId = sprite->data[0];
	reg1 = sprite->data[1];
	reg2 = sprite->data[2];
	spriteId2 = sprite->data[3];
	
	FreeSpriteTilesByTag(ITEMICON_TAG);
	FreeSpritePaletteByTag(ITEMICON_TAG);
	FreeSpriteOamMatrix(sprite);
	DestroySprite(sprite);
	
	if (spriteId2 != MAX_SPRITES)
	{
		SetGpuReg(REG_OFFSET_DISPCNT, reg1);
		SetGpuReg(REG_OFFSET_WINOUT, reg2);
		
		FreeSpriteTilesByTag(ITEMICON_TAG);
		FreeSpritePaletteByTag(ITEMICON_TAG);
		FreeSpriteOamMatrix(&gSprites[spriteId2]);
		DestroySprite(&gSprites[spriteId2]);
	}
	if (windowId != 0xFF)
	{
		ClearDialogWindowAndFrame(windowId, TRUE);
		RemoveWindow(windowId);
	}
}

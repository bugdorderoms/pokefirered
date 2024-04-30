#include "global.h"
#include "event_data.h"
#include "event_object_lock.h"
#include "event_object_movement.h"
#include "graphics.h"
#include "item.h"
#include "item_menu.h"
#include "item_menu_icons.h"
#include "list_menu.h"
#include "menu_indicators.h"
#include "overworld.h"
#include "registered_item.h"
#include "sprite.h"
#include "script.h"
#include "sound.h"
#include "task.h"
#include "constants/items.h"
#include "constants/songs.h"

/* cursor directions */
enum
{
	CURSORDIRECTION_UP,
	CURSORDIRECTION_RIGT,
	CURSORDIRECTION_DOWN,
	CURSORDIRECTION_LEFT,
	NUM_CURSORDIRECTIONS
};

/* Actions */
enum
{
	REGISTERITEM_ACTION_PRESSA,
	REGISTERITEM_ACTION_PRESSB,
	REGISTERITEM_ACTION_EXIT,
	REGISTERITEM_NUM_ACTIONS
};

// number of times that is added X and Y to the box and item positions when starting slot swapping
#define SLIDE_FRAMES_COUNT 10

// Direction that the cursor will init when the menu was open
#define CURSOR_INITIAL_DIRECTION CURSORDIRECTION_UP

// Distance of the box from the center of the screen
#define BOX_CENTER_DISTANCE 50

// this file's functions
static void SpriteCB_WaitAffinAnimsAndInitCursor(struct Sprite *sprite);
static void SpriteCB_WaitAffinAnimsAndSwitchItems(struct Sprite *sprite);
static void SpriteCB_WaitAffinAnimsAndReturnToInput(struct Sprite *sprite);
static void SpriteCB_WaitSlideAnimAndReturnToInput(struct Sprite *sprite);
static void SpriteCB_SelectItem_HandleInput(struct Sprite *sprite);
static void SpriteCB_MoveItem_HandleInput(struct Sprite *sprite);
static void SpriteCB_HandleItemSelection(struct Sprite *sprite);
static void SpriteCB_MoveSpriteToSwitchMode(struct Sprite *sprite);
static void SpriteCB_SelectItem_Use(struct Sprite *sprite);
static void SpriteCB_SelectItem_Close(struct Sprite *sprite);
static void SpriteCB_SelectItem_ReturnToOverworld(struct Sprite *sprite);
static void SpriteCB_SelectItem_Register(struct Sprite *sprite);
static void SpriteCB_SelectItem_ReturnToBag(struct Sprite *sprite);
static u8 DirectionToAnimNum(u8 direction);
static u8 GetItemSpriteIdByDirection(struct Sprite *sprite, u8 direction);
static void SetDataToSlideAnim(struct Sprite *sprite, u8 direction);
static void GeneralCursorMovement(struct Sprite *sprite);
static void HandleSelectItemAction(struct Sprite *sprite, u8 direction);
static void InitAffinAnimsAndHandleExitAction(struct Sprite *sprite);

// Do different actions depending on the menu location
static const SpriteCallback sRegisterItemActionsByMenuType[][REGISTERITEM_NUM_ACTIONS] =
{
	[REGISTERITEM_LOCATION_OVERWORLD] =
	{
		[REGISTERITEM_ACTION_PRESSA] = SpriteCB_SelectItem_Use,
		[REGISTERITEM_ACTION_PRESSB] = SpriteCB_SelectItem_Close,
		[REGISTERITEM_ACTION_EXIT]   = SpriteCB_SelectItem_ReturnToOverworld,
	},
	[REGISTERITEM_LOCATION_BAG] =
	{
		[REGISTERITEM_ACTION_PRESSA] = SpriteCB_SelectItem_Register,
		[REGISTERITEM_ACTION_PRESSB] = SpriteCB_SelectItem_Close,
		[REGISTERITEM_ACTION_EXIT]   = SpriteCB_SelectItem_ReturnToBag,
	}
};

// X and Y positions of each box
static const struct UCoords16 sRegisteredBoxesIconsPositions[REGISTERED_ITEMS_COUNT] =
{
	{ // up box
		.x = DISPLAY_WIDTH / 2,
		.y = DISPLAY_HEIGHT / 2 - BOX_CENTER_DISTANCE,
	},
	{ // right box
		.x = DISPLAY_WIDTH / 2 + BOX_CENTER_DISTANCE, 
		.y = DISPLAY_HEIGHT / 2,
	},
	{ // down box
		.x = DISPLAY_WIDTH / 2,
		.y = DISPLAY_HEIGHT / 2 + BOX_CENTER_DISTANCE,
	},
	{ // left box
		.x = DISPLAY_WIDTH / 2 - BOX_CENTER_DISTANCE,
		.y = DISPLAY_HEIGHT / 2,
	},
};

// X And Y Positions of each cursor direction
static const struct UCoords16 sCursorPositions[NUM_CURSORDIRECTIONS] =
{
	[CURSORDIRECTION_UP] = {
		.x = 120, 
		.y = 52,
	},
	[CURSORDIRECTION_RIGT] = {
		.x = 147,
		.y = 80,
	},
	[CURSORDIRECTION_DOWN] = {
		.x = 119,
		.y = 107,
	},
	[CURSORDIRECTION_LEFT] = {
		.x = 92,
		.y = 79,
	},
};

static const struct SpriteSheet sBoxSpriteSheet = 
{
	.data = gRegisteredItemBoxSpriteGfx,
	.size = 0x200,
	.tag = BOX_TAG,
};

static const struct SpritePalette sBoxAndCursorSpritePalette = 
{
	.data = gRegisteredItemBoxSpritePal,
	.tag = BOX_TAG,
};

static const struct SpriteSheet sCursorSpriteSheet = 
{
	.data = gRegisteredItemCursorSpriteGfx,
	.size = 0x100,
	.tag = CURSOR_TAG,
};

static const union AffineAnimCmd sAffineAnim_RegisteredItemLoad[] = {
	AFFINEANIMCMD_FRAME(16, 16, 0, 0),
	AFFINEANIMCMD_FRAME(16, 16, 0, 15),
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sAffineAnim_RegisteredItemUnload[] = {
	AFFINEANIMCMD_FRAME(-16, -16, 0, 15),
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sAffineAnim_RegisteredItemCursorUp[] = {
	AFFINEANIMCMD_FRAME(0, 0, 0x00, 1),
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sAffineAnim_RegisteredItemCursorDown[] = {
	AFFINEANIMCMD_FRAME(0, 0, 0x80, 1),
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sAffineAnim_RegisteredItemCursorRight[] = {
	AFFINEANIMCMD_FRAME(0, 0, 0xC0, 1),
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd sAffineAnim_RegisteredItemCursorLeft[] = {
	AFFINEANIMCMD_FRAME(0, 0, 0x40, 1),
	AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const sAffineAnimTable_RegisteredItem[] = {
	sAffineAnim_RegisteredItemLoad,
	sAffineAnim_RegisteredItemUnload
};

static const union AffineAnimCmd *const sAffineAnimTable_RegisteredItemCursor[] = {
	sAffineAnim_RegisteredItemCursorUp,
	sAffineAnim_RegisteredItemCursorDown,
	sAffineAnim_RegisteredItemCursorRight,
	sAffineAnim_RegisteredItemCursorLeft
};

static const struct OamData sOamData_RegisteredItem =
{
	.affineMode = ST_OAM_AFFINE_NORMAL,
	.objMode = ST_OAM_OBJ_NORMAL,
	.bpp = ST_OAM_4BPP,
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 0
};

static const struct OamData sOamData_RegisteredItemCursor =
{
	.affineMode = ST_OAM_AFFINE_NORMAL,
	.objMode = ST_OAM_OBJ_NORMAL,
	.bpp = ST_OAM_4BPP,
	.shape = SPRITE_SHAPE(16x16),
	.size = SPRITE_SIZE(16x16),
	.priority = 0
};

static const struct SpriteTemplate sSelectItem_Cursor =
{
	.tileTag = CURSOR_TAG,
	.paletteTag = BOX_TAG,
	.oam = &sOamData_RegisteredItemCursor,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = sAffineAnimTable_RegisteredItemCursor,
	.callback = SpriteCB_WaitAffinAnimsAndInitCursor
};
	
static const struct SpriteTemplate sBoxTemplate = 
{
	.tileTag = BOX_TAG,
	.paletteTag = BOX_TAG,
	.oam = &sOamData_RegisteredItem,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = sAffineAnimTable_RegisteredItem,
	.callback = SpriteCallbackDummy
};

static const struct SpriteTemplate sItemTemplate =
{
	.tileTag = ITEMICON_INITIAL_TAG,
	.paletteTag = ITEMICON_INITIAL_TAG,
	.oam = &sOamData_RegisteredItem,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = sAffineAnimTable_RegisteredItem,
	.callback = SpriteCallbackDummy
};

// functions that makes the game load more than 1 registered item
bool8 IsAllRegisteredItemSlotsFree(void)
{
	u8 i;

	TryRemoveRegisteredItems();
	
	for (i = 0; i < REGISTERED_ITEMS_COUNT; i++)
	{
		if (gSaveBlock1Ptr->registeredItem[i])
			return FALSE;
	}
	return TRUE;
}

u8 FindRegisteredItemSlot(u16 itemId)
{
	u8 i;
	
	for (i = 0; i < REGISTERED_ITEMS_COUNT; i++)
	{
		if (gSaveBlock1Ptr->registeredItem[i] == itemId)
			return i;
	}
	return REGISTERED_ITEMS_COUNT;
}

void TryRemoveRegisteredItems(void)
{
	u8 i;
	
	for (i = 0; i < REGISTERED_ITEMS_COUNT; i++)
	{
		if (gSaveBlock1Ptr->registeredItem[i] && !CheckBagHasItem(gSaveBlock1Ptr->registeredItem[i], 1))
			gSaveBlock1Ptr->registeredItem[i] = ITEM_NONE;
	}
}

// box and item defines
#define sAddToX                  data[0]
#define sAddToY                  data[1]
#define sNumFramesToAdd          data[2]
// useds only by the boxes
#define sItemSpriteId            data[3]

// cursor defines
#define sCursorDirection         data[0]
#define sCursorToSwitch          data[1]
#define sMenuLocation            data[2]
#define sBoxSpriteIds(direction) data[direction + 3]

static void CreateItemSpriteAndLinkToBox(struct Sprite *cursorSprite, u8 direction)
{
	u8 spriteId, boxSpriteId = cursorSprite->sBoxSpriteIds(direction);
	u16 tag;

	if (gSaveBlock1Ptr->registeredItem[direction])
	{
		tag = ITEMICON_INITIAL_TAG + direction;
		
		spriteId = AddItemIconObjectWithCustomObjectTemplate(&sItemTemplate, tag, tag, gSaveBlock1Ptr->registeredItem[direction]);
		
		// icon X and Y positions = box position - 4 (center of the box image)
		gSprites[spriteId].x = gSprites[boxSpriteId].x - 4;
		gSprites[spriteId].y = gSprites[boxSpriteId].y - 4;
	}
	else
		spriteId = MAX_SPRITES;
	
	gSprites[boxSpriteId].sItemSpriteId = spriteId;
}

static void DestroyItemIconSprite(struct Sprite *cursorSprite, u8 direction)
{
	u8 spriteId = GetItemSpriteIdByDirection(cursorSprite, direction);
	
	if (spriteId != MAX_SPRITES)
	{
		FreeSpriteOamMatrix(&gSprites[spriteId]);
	    DestroySprite(&gSprites[spriteId]);
	    FreeSpriteTilesByTag(ITEMICON_INITIAL_TAG + direction);
	    FreeSpritePaletteByTag(ITEMICON_INITIAL_TAG + direction);
	}
}

static void DestroyCursorSprite(struct Sprite *sprite)
{
	FreeSpriteOamMatrix(sprite);
	DestroySprite(sprite);
	FreeSpriteTilesByTag(CURSOR_TAG);
	FreeSpritePaletteByTag(BOX_TAG);
}

void InitRegisteredItemsToChoose(u8 menuLocation)
{
	u8 i;
	struct Sprite *cursorSprite;
	
	LoadSpriteSheet(&sBoxSpriteSheet);
	LoadSpritePalette(&sBoxAndCursorSpritePalette);
	
	LoadSpriteSheet(&sCursorSpriteSheet);
	
	cursorSprite = &gSprites[CreateSprite(&sSelectItem_Cursor, sCursorPositions[CURSOR_INITIAL_DIRECTION].x, sCursorPositions[CURSOR_INITIAL_DIRECTION].y, 0)];
	
	cursorSprite->invisible = TRUE;
	cursorSprite->sCursorDirection = CURSOR_INITIAL_DIRECTION;
	cursorSprite->sMenuLocation = menuLocation;
	
	for (i = 0; i < REGISTERED_ITEMS_COUNT; i++)
	{
		cursorSprite->sBoxSpriteIds(i) = CreateSprite(&sBoxTemplate, sRegisteredBoxesIconsPositions[i].x, sRegisteredBoxesIconsPositions[i].y, 0xFF);
		CreateItemSpriteAndLinkToBox(cursorSprite, i);
	}
	PlaySE(SE_EXIT);
}

static void SpriteCB_WaitAffinAnimsAndInitCursor(struct Sprite *sprite)
{
	if (gSprites[sprite->sBoxSpriteIds(0)].affineAnimEnded)
	{
		sprite->invisible = FALSE;
		sprite->callback = SpriteCB_SelectItem_HandleInput;
	}
}

static void DoActionByMenuType(struct Sprite *sprite, u8 action)
{
	sRegisterItemActionsByMenuType[sprite->sMenuLocation][action](sprite);
}

static void SpriteCB_SelectItem_HandleInput(struct Sprite *sprite)
{
	u8 direction;
	
	if (JOY_NEW(A_BUTTON)) // Press A
	    DoActionByMenuType(sprite, REGISTERITEM_ACTION_PRESSA);
	else if (JOY_NEW(B_BUTTON | SELECT_BUTTON)) // Press B
		DoActionByMenuType(sprite, REGISTERITEM_ACTION_PRESSB);
	else if (JOY_NEW(START_BUTTON))
	{
		// when press START init slot swap input
		direction = sprite->sCursorToSwitch = sprite->sCursorDirection;
		
		SetDataToSlideAnim(&gSprites[sprite->sBoxSpriteIds(direction)], direction);
		HandleSelectItemAction(sprite, direction ^ 2); // ^ 2 inverts the cursor direction for quickly acess the opposite slot
		sprite->callback = SpriteCB_MoveItem_HandleInput;
	}
	else // general cursor movement
		GeneralCursorMovement(sprite);
}

static void SpriteCB_SelectItem_Use(struct Sprite *sprite)
{
	u8 direction = sprite->sCursorDirection;
	
	if (gSaveBlock1Ptr->registeredItem[direction])
	{
		PlaySE(SE_SELECT);
		gSpecialVar_ItemId = gSaveBlock1Ptr->registeredItem[direction];
		InitAffinAnimsAndHandleExitAction(sprite);
	}
	else
		PlaySE(SE_FAILURE);
}

static void SpriteCB_SelectItem_Close(struct Sprite *sprite)
{
	PlaySE(SE_EXIT);
	gSpecialVar_ItemId = ITEM_NONE;
	InitAffinAnimsAndHandleExitAction(sprite);
}

static void SpriteCB_SelectItem_ReturnToOverworld(struct Sprite *sprite)
{
	DestroyCursorSprite(sprite);
	
	if (!gSpecialVar_ItemId) // Return to overworld
	{
		ClearPlayerHeldMovementAndUnfreezeObjectEvents();
		ScriptContext2_Disable();
	}
	else // Use the selected item
	{
		ScriptContext2_Enable();
		gTasks[CreateTask(ItemId_GetFieldFunc(gSpecialVar_ItemId), 8)].data[3] = 1;
	}
}

static void SpriteCB_SelectItem_Register(struct Sprite *sprite)
{
	u8 direction = sprite->sCursorDirection;
	u16 oldItem = gSaveBlock1Ptr->registeredItem[direction];
	
	PlaySE(oldItem ? SE_SWITCH : SE_SELECT);
	gSaveBlock1Ptr->registeredItem[direction] = gSpecialVar_ItemId;
	
	if (oldItem)
		DestroyItemIconSprite(sprite, direction);
	
	CreateItemSpriteAndLinkToBox(sprite, direction);
	
	InitAffinAnimsAndHandleExitAction(sprite);
}

static void SpriteCB_SelectItem_ReturnToBag(struct Sprite *sprite)
{
	gSpecialVar_0x8000 = TRUE; // Signal that the menu was closed
	DestroyCursorSprite(sprite);
}

static void SpriteCB_MoveItem_HandleInput(struct Sprite *sprite)
{
	u8 firstItem, secondItem, direction = sprite->sCursorToSwitch;
	
	if (!gSprites[sprite->sBoxSpriteIds(direction)].sNumFramesToAdd)
	{
		firstItem = GetItemSpriteIdByDirection(sprite, direction);
		secondItem = GetItemSpriteIdByDirection(sprite, sprite->sCursorDirection);
		
		if (JOY_NEW(A_BUTTON))
		{
			// when press A init the swap action
			SetDataToSlideAnim(&gSprites[sprite->sBoxSpriteIds(direction)], direction ^ 2); // inverts the cursor direction for the box moving anim
			
			// fails if choose the same slot or two free slots
			if (firstItem == secondItem || (firstItem == MAX_SPRITES && secondItem == MAX_SPRITES))
			{
				PlaySE(SE_FAILURE);
				sprite->callback = SpriteCB_WaitSlideAnimAndReturnToInput;
			}
			else
			{
				if (firstItem != MAX_SPRITES)
					StartSpriteAffineAnim(&gSprites[firstItem], 1);
				
				if (secondItem != MAX_SPRITES)
					StartSpriteAffineAnim(&gSprites[secondItem], 1);
				
				sprite->callback = SpriteCB_WaitAffinAnimsAndSwitchItems;
			}
		}
		else if (JOY_NEW(B_BUTTON))
		{
			// when press B calcel slot swap action
			SetDataToSlideAnim(&gSprites[sprite->sBoxSpriteIds(direction)], direction ^ 2); // inverts the cursor direction for the box return for it's original position
			HandleSelectItemAction(sprite, sprite->sCursorToSwitch);
			sprite->callback = SpriteCB_WaitSlideAnimAndReturnToInput;
		}
		else // general cursor movement
		    GeneralCursorMovement(sprite);
	}
}

static void SpriteCB_HandleItemSelection(struct Sprite *sprite)
{
	u8 i;
	
	if (gSprites[sprite->sBoxSpriteIds(0)].affineAnimEnded)
	{
		for (i = 0; i < REGISTERED_ITEMS_COUNT; i++)
		{
			DestroyItemIconSprite(sprite, i);
			
			FreeSpriteOamMatrix(&gSprites[sprite->sBoxSpriteIds(i)]);
			DestroySprite(&gSprites[sprite->sBoxSpriteIds(i)]);
			FreeSpriteTilesByTag(BOX_TAG);
			FreeSpritePaletteByTag(BOX_TAG);
		}
		DoActionByMenuType(sprite, REGISTERITEM_ACTION_EXIT);
	}
}

static void SpriteCB_HandleSwitchItems(struct Sprite *sprite)
{
	u16 temp;
	u8 firstDirection = sprite->sCursorDirection, secondDirection = sprite->sCursorToSwitch;
	
	SWAP(gSaveBlock1Ptr->registeredItem[firstDirection], gSaveBlock1Ptr->registeredItem[secondDirection], temp);
	
	CreateItemSpriteAndLinkToBox(sprite, firstDirection);
	CreateItemSpriteAndLinkToBox(sprite, secondDirection);
	
	sprite->callback = SpriteCB_WaitAffinAnimsAndReturnToInput;
}

static void SpriteCB_WaitAffinAnimsAndSwitchItems(struct Sprite *sprite)
{
	u8 firstDirection = sprite->sCursorDirection, secondDirection = sprite->sCursorToSwitch;
	u8 firstItem = GetItemSpriteIdByDirection(sprite, firstDirection), secondItem = GetItemSpriteIdByDirection(sprite, secondDirection);
	
	if ((firstItem != MAX_SPRITES && gSprites[firstItem].affineAnimEnded) || (secondItem != MAX_SPRITES && gSprites[secondItem].affineAnimEnded)) // wait anim ends to continue
	{
		PlaySE(SE_SWITCH);
		
		DestroyItemIconSprite(sprite, firstDirection);
		DestroyItemIconSprite(sprite, secondDirection);
		
		sprite->callback = SpriteCB_HandleSwitchItems;
	}
}

static void SpriteCB_WaitAffinAnimsAndReturnToInput(struct Sprite *sprite)
{
	u8 firstItem = GetItemSpriteIdByDirection(sprite, sprite->sCursorDirection), secondItem = GetItemSpriteIdByDirection(sprite, sprite->sCursorToSwitch);
	
	if ((firstItem != MAX_SPRITES && gSprites[firstItem].affineAnimEnded) || (secondItem != MAX_SPRITES && gSprites[secondItem].affineAnimEnded)) // wait anim ends to continue
		sprite->callback = SpriteCB_SelectItem_HandleInput;
}

static void SpriteCB_WaitSlideAnimAndReturnToInput(struct Sprite *sprite)
{
	if (!gSprites[sprite->sBoxSpriteIds(sprite->sCursorToSwitch)].sNumFramesToAdd)
		sprite->callback = SpriteCB_SelectItem_HandleInput;
}

static void SpriteCB_MoveSpriteToSwitchMode(struct Sprite *sprite)
{
	if (--sprite->sNumFramesToAdd != 0)
	{
		sprite->x += sprite->sAddToX;
		sprite->y += sprite->sAddToY;
		return;
	}
	sprite->callback = SpriteCallbackDummy;
}

static void SetDataToSlideAnim(struct Sprite *sprite, u8 direction)
{
	u8 itemSpriteId;
	s16 addX = 0, addY = 0;
	
	switch (direction)
	{
		case CURSORDIRECTION_UP:
		    addY = -1;
			break;
		case CURSORDIRECTION_RIGT:
		    addX = 1;
		    break;
		case CURSORDIRECTION_DOWN:
		    addY = 1;
		    break;
		case CURSORDIRECTION_LEFT:
		    addX = -1;
		    break;
	}
	
	sprite->sAddToX = addX;
	sprite->sAddToY = addY;
	sprite->sNumFramesToAdd = SLIDE_FRAMES_COUNT;
	sprite->callback = SpriteCB_MoveSpriteToSwitchMode;
	
	itemSpriteId = sprite->sItemSpriteId;
	
	if (itemSpriteId != MAX_SPRITES)
	{
		gSprites[itemSpriteId].sAddToX = addX;
		gSprites[itemSpriteId].sAddToY = addY;
		gSprites[itemSpriteId].sNumFramesToAdd = SLIDE_FRAMES_COUNT;
		gSprites[itemSpriteId].callback = SpriteCB_MoveSpriteToSwitchMode;
	}
}

static void InitAffinAnimsAndHandleExitAction(struct Sprite *sprite)
{
	u8 i, spriteId;
	
	for (i = 0; i < REGISTERED_ITEMS_COUNT; i++)
	{
		spriteId = GetItemSpriteIdByDirection(sprite, i);
		
		if (spriteId != MAX_SPRITES)
			StartSpriteAffineAnim(&gSprites[spriteId], 1);
		
		StartSpriteAffineAnim(&gSprites[sprite->sBoxSpriteIds(i)], 1);
	}
	sprite->invisible = TRUE;
	sprite->callback = SpriteCB_HandleItemSelection;
}

static void GeneralCursorMovement(struct Sprite *sprite)
{
	if (JOY_NEW(DPAD_UP))
		HandleSelectItemAction(sprite, CURSORDIRECTION_UP);
	else if (JOY_NEW(DPAD_DOWN))
		HandleSelectItemAction(sprite, CURSORDIRECTION_DOWN);
	else if (JOY_NEW(DPAD_RIGHT))
		HandleSelectItemAction(sprite, CURSORDIRECTION_RIGT);
	else if (JOY_NEW(DPAD_LEFT))
		HandleSelectItemAction(sprite, CURSORDIRECTION_LEFT);
}

static void HandleSelectItemAction(struct Sprite *sprite, u8 direction)
{
	PlaySE(SE_SELECT);
	sprite->sCursorDirection = direction;
	sprite->x = sCursorPositions[direction].x;
	sprite->y = sCursorPositions[direction].y;
	StartSpriteAffineAnim(sprite, DirectionToAnimNum(direction));
}

static u8 GetItemSpriteIdByDirection(struct Sprite *sprite, u8 direction)
{
	return gSprites[sprite->sBoxSpriteIds(direction)].sItemSpriteId;
}

static u8 DirectionToAnimNum(u8 direction)
{
	if (direction == CURSORDIRECTION_RIGT || direction == CURSORDIRECTION_DOWN)
		direction ^= 3;
	
	return direction;
}

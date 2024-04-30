#ifndef GUARD_REGISTERED_ITEM_H
#define GUARD_REGISTERED_ITEM_H

enum
{
	REGISTERITEM_LOCATION_OVERWORLD,
	REGISTERITEM_LOCATION_BAG
};

#define REGISTERED_ITEMS_COUNT 4

#define CURSOR_TAG 0x1075
#define BOX_TAG 0x1078
#define ITEMICON_INITIAL_TAG 0x1088

bool8 IsAllRegisteredItemSlotsFree(void);
u8 FindRegisteredItemSlot(u16 itemId);
void TryRemoveRegisteredItems(void);
void InitRegisteredItemsToChoose(u8 menuLocation);

#endif // GUARD_REGISTERED_ITEM_H
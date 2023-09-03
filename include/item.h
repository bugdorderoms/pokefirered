#ifndef GUARD_ITEM_H
#define GUARD_ITEM_H

#include "global.h"
#include "battle.h"

typedef void (*ItemUseFunc)(u8);

// Item type IDs (used to determine the exit callback)
#define ITEM_TYPE_NONE             0
#define ITEM_TYPE_PARTY_MENU       1
#define ITEM_TYPE_FIELD            2
#define ITEM_TYPE_BAG_MENU         3 // No exit callback, stays in bag menu
#define ITEM_TYPE_PARTY_MENU_MOVES 4

// Item battle script IDs (need to be non-zero)
#define EFFECT_ITEM_RESTORE_HP           1 // also revive
#define EFFECT_ITEM_CURE_STATUS          2
#define EFFECT_ITEM_INCREASE_STAT        3
#define EFFECT_ITEM_SET_MIST             4
#define EFFECT_ITEM_SET_FOCUS_ENERGY     5
#define EFFECT_ITEM_ESCAPE               6
#define EFFECT_ITEM_THROW_BALL           7
#define EFFECT_ITEM_RESTORE_PP           8
#define EFFECT_ITEM_INCREASE_ALL_STATS   9
#define EFFECT_ITEM_POKE_FLUTE           10

enum
{
	FLAG_GET_OBTAINED,
	FLAG_SET_OBTAINED,
};

enum
{
	ITEM_TYPE_FIELD_USE,
	ITEM_TYPE_HEALTH_RECOVERY,
	ITEM_TYPE_STATUS_RECOVERY,
	ITEM_TYPE_PP_RECOVERY,
	ITEM_TYPE_STAT_BOOST_DRINK,
	ITEM_TYPE_STAT_BOOST_WING,
	ITEM_TYPE_EVOLUTION_ITEM,
	ITEM_TYPE_BATTLE_ITEM,
	ITEM_TYPE_FLUTE,
	ITEM_TYPE_HELD_ITEM,
	ITEM_TYPE_GEM,
	ITEM_TYPE_PLATE,
	ITEM_TYPE_MEMORY,
	ITEM_TYPE_DRIVE,
	ITEM_TYPE_INCENSE,
	ITEM_TYPE_MEGA_STONE,
	ITEM_TYPE_Z_CRYSTAL,
	ITEM_TYPE_NECTAR,
	ITEM_TYPE_SELLABLE,
	ITEM_TYPE_FOSSIL,
	ITEM_TYPE_MAIL,
	ITEM_TYPE_MINT,
	ITEM_TYPE_TERA_SHARD,
	ITEM_TYPE_UNRECOGNIZED = 0xFF,
};

struct Item
{
    /*0x00*/ u8 name[ITEM_NAME_LENGTH];
	/*0x0E*/ u8 holdEffect;
    /*0x0F*/ u8 holdEffectParam;
    /*0x10*/ const u8 *description;
	/*0x14*/ u16 price;
	/*0x16*/ u8 pocket;
    /*0x17*/ u8 type; // determine the exit CB
    /*0x18*/ ItemUseFunc fieldUseFunc;
	/*0x1C*/ u8 usageType; // used by bag sorting
    /*0x1D*/ u8 battleUsage; // determine battle script id
	/*0x1E*/ u8 flingPower;
};

struct BagPocket
{
    struct ItemSlot *itemSlots;
    u8 capacity;
};

extern const struct Item gItems[];
extern struct BagPocket gBagPockets[];

u16 GetBagItemQuantity(u16 *quantity);
void CopyItemName(u16 itemId, u8 *dest);
void CopyItemNameHandlePlural(u16 itemId, u16 quantity, u8 *dest);
bool8 IsBagPocketNonEmpty(u8 pocket);
bool8 CheckBagHasItem(u16 itemId, u16 count);
bool8 CheckBagHasSpace(u16 itemId, u16 count);
bool8 RemoveBagItem(u16 itemId, u16 count);
void ClearItemSlots(struct ItemSlot *itemSlots, u8 b);
u8 CountUsedPCItemSlots(void);
bool8 CheckPCHasItem(u16 itemId, u16 count);
bool8 AddPCItem(u16 itemId, u16 count);
const u8 *ItemId_GetName(u16 itemId);
u16 ItemId_GetPrice(u16 itemId);
u8 ItemId_GetHoldEffect(u16 itemId);
u8 ItemId_GetHoldEffectParam(u16 itemId);
const u8 *ItemId_GetDescription(u16 itemId);
u8 ItemId_GetPocket(u16 itemId);
u8 ItemId_GetType(u16 itemId);
ItemUseFunc ItemId_GetFieldFunc(u16 itemId);
u8 ItemId_GetBattleUsage(u16 itemId);
u8 ItemId_GetFlingPower(u16 itemId);
u8 ItemId_GetUsageType(u16 itemId);
u8 ItemId_GetBattleEffectUsageType(u16 itemId);
u16 itemid_get_market_price(u16 itemId);
void ClearBag(void);
void ClearPCItemSlots(void);
void TrySetObtainedItemQuestLogEvent(u16 itemId);
bool8 AddBagItem(u16 itemId, u16 amount);

void SortPocketAndPlaceHMsFirst(struct BagPocket * pocket);
u16 BagGetItemIdByPocketPosition(u8 pocketId, u16 itemId);
u16 BagGetQuantityByPocketPosition(u8 pocketId, u16 itemId);
u16 BagGetQuantityByItemId(u16 item);
void BagPocketCompaction(struct ItemSlot * slots, u8 capacity);
u16 GetPcItemQuantity(u16 *);
void SetBagPocketsPointers(void);

void ItemPcCompaction(void);
void RemovePCItem(u16 itemId, u16 quantity);
void SortAndCompactBagPocket(struct BagPocket * pocket);
u8 CountItemsInPC(void);
void ApplyNewEncryptionKeyToBagItems(u32 key);

void ResetItemFlags(void);
bool8 GetSetItemObtained(u16 item, u8 caseId);

#endif // GUARD_ITEM_H

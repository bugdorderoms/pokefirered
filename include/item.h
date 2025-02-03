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
#define EFFECT_ITEM_RESTORE_HP            1
#define EFFECT_ITEM_CURE_PRIMARY_STATUS   2
#define EFFECT_ITEM_CURE_SECONDARY_STATUS 3
#define EFFECT_ITEM_INCREASE_STAT         4
#define EFFECT_ITEM_SET_MIST              5
#define EFFECT_ITEM_SET_FOCUS_ENERGY      6
#define EFFECT_ITEM_ESCAPE                7
#define EFFECT_ITEM_THROW_BALL            8
#define EFFECT_ITEM_RESTORE_PP            9
#define EFFECT_ITEM_INCREASE_ALL_STATS    10
#define EFFECT_ITEM_POKE_FLUTE            11
#define EFFECT_ITEM_REVIVE                12
#define EFFECT_ITEM_ADRENALINE_ORB        13

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
	ITEM_TYPE_MASK,
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
	/*0x14*/ const u8 *description;
	/*0x18*/ u8 pocket;
	/*0x19*/ u8 holdEffect;
	/*0x1A*/ u16 holdEffectParam;
	/*0x1C*/ u16 price;
	/*0x1E*/ u8 type; // determine the exit CB
	/*0x1F*/ u8 usageType; // used by bag sorting
	/*0x20*/ ItemUseFunc fieldUseFunc;
	/*0x24*/ const u32 *iconPic;
	/*0x28*/ const u32 *iconPalette;
	/*0x2C*/ const u8 *itemEffect; // effect on using it on a pokemon
    /*0x30*/ u8 battleUsage; // determine battle script id
	/*0x31*/ u8 flingPower;
};

// For strings printed on item use
enum
{
	ITEMUSE_STRING_NOTHING, // Print fail message
	ITEMUSE_STRING_PP_RESTORED,
	ITEMUSE_STRING_POISON_CURED,
	ITEMUSE_STRING_BURN_HEALED,
	ITEMUSE_STRING_THAWED,
	ITEMUSE_STRING_WOKE_UP,
	ITEMUSE_STRING_SNAPPED_CONFUSION,
	ITEMUSE_STRING_OVER_INFATUATION,
	ITEMUSE_STRING_BECAME_HEALTHY,
	ITEMUSE_STRING_RAISE_DYNAMAX_LEVEL,
	ITEMUSE_STRING_STAT_CHANGED,
	ITEMUSE_STRING_PARALYSIS_CURED,
	ITEMUSE_STRING_LEVELED_UP,
	ITEMUSE_STRING_GAINED_EXP,
	ITEMUSE_STRING_GAINED_EXP_LEVELED_UP,
	ITEMUSE_STRING_CANT_BYPASS_LEVEL_CAP,
	ITEMUSE_STRING_PP_INCREASED,
	ITEMUSE_STRING_CHANGE_GMAX_FACTOR,
	ITEMUSE_STRING_CHANGED_TERA_TYPE,
	ITEMUSE_STRING_FRIENDSHIP_CHANGED
};

enum
{
	ITEMUSE_COPY_NOTHING, // Don't copy anything
	ITEMUSE_COPY_STAT_NAME,
	ITEMUSE_COPY_EXP_AND_LEVEL,
	ITEMUSE_COPY_GAINED_OR_LOSES,
	ITEMUSE_COPY_TYPE_NAME,
	ITEMUSE_COPY_INCREASED_OR_DECREASED
};

struct BagPocket
{
    struct ItemSlot *itemSlots;
    u8 capacity;
};

extern const struct Item gItems[];
extern struct BagPocket gBagPockets[];

u16 GetBagItemQuantity(u16 *quantity);
u8 *CopyItemName(u16 itemId, u8 *dest);
u8 *CopyItemNameHandlePlural(u16 itemId, u16 quantity, u8 *dest);
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
u16 ItemId_GetHoldEffectParam(u16 itemId);
const u8 *ItemId_GetDescription(u16 itemId);
u8 ItemId_GetPocket(u16 itemId);
u8 ItemId_GetType(u16 itemId);
ItemUseFunc ItemId_GetFieldFunc(u16 itemId);
u8 ItemId_GetBattleUsage(u16 itemId);
u8 ItemId_GetFlingPower(u16 itemId);
u8 ItemId_GetUsageType(u16 itemId);
u8 ItemId_GetBattleEffectUsageType(u16 itemId);
const u8 *ItemId_GetItemEffect(u16 itemId);
void ClearBag(void);
void ClearPCItemSlots(void);
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

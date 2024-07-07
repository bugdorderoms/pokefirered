#ifndef GUARD_UTIL_H
#define GUARD_UTIL_H

#include "global.h"
#include "sprite.h"

enum
{
	SORT_BAG_ITEMS,
	SORT_LIST_MENU_ITEMS,
};

struct SortComparator
{
	union
	{
		struct
		{
			struct ItemSlot *array;
			struct ItemSlot *aux;
			s8 (*func)(struct ItemSlot*, struct ItemSlot*);
		} bagItemSort;
		
		struct
		{
			struct ListMenuItem *array;
			struct ListMenuItem *aux;
			s8 (*func)(struct ListMenuItem*, struct ListMenuItem*);
		} listMenuItemSort;
	} sortUnion;
	u8 kind; // This determine how to acess the union above
};

extern const u8 gMiscBlank_Gfx[];
extern const u32 gBitTable[];

u8 CreateInvisibleSpriteWithCallback(void (*)(struct Sprite *));
void StoreWordInTwoHalfwords(u16 *, unsigned);
void LoadWordFromTwoHalfwords(u16 *, unsigned *);
u16 CalcCRC16WithTable(const u8 *data, u32 length);
void DoBgAffineSet(struct BgAffineDstData * dest, u32 texX, u32 texY, s16 srcX, s16 srcY, s16 sx, s16 sy, u16 alpha);
bool8 JumpBasedOnKind(u32 value, u8 cmpKind, u32 cmpTo);
s8 CompareTextAlphabetically(const u8 *text1, const u8 *text2);
void MergeSort(struct SortComparator *comparator, u32 low, u32 high);

#define MergeSortArray(comparator, nItems) MergeSort(comparator, 0, nItems - 1)

#endif // GUARD_UTIL_H

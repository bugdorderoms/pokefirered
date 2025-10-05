#include "global.h"
#include "util.h"
#include "text.h"
#include "list_menu.h"
#include "malloc.h"
#include "global.h"
#include "constants/battle_script_commands.h"

static const struct SpriteTemplate gInvisibleSpriteTemplate =
{
    .tileTag = 0,
    .paletteTag = 0,
    .oam = &gDummyOamData,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const u8 gMiscBlank_Gfx[] = INCBIN_U8("graphics/interface/blank.4bpp");

u32 CreateInvisibleSpriteWithCallback(void (*callback)(struct Sprite *))
{
    u32 spriteId = CreateSprite(&gInvisibleSpriteTemplate, 248, 168, 14);
    gSprites[spriteId].invisible = TRUE;
    gSprites[spriteId].callback = callback;
    return spriteId;
}

void StoreWordInTwoHalfwords(u16 *h, unsigned w)
{
    h[0] = (u16)(w);
    h[1] = (u16)(w >> 16);
}

void LoadWordFromTwoHalfwords(u16 *h, unsigned *w)
{
    *w = h[0] | (s16)h[1] << 16;
}

static void SetBgAffineStruct(struct BgAffineSrcData *src, u32 texX, u32 texY, s16 scrX, s16 scrY, s16 sx, s16 sy, u16 alpha)
{
    src->texX = texX;
    src->texY = texY;
    src->scrX = scrX;
    src->scrY = scrY;
    src->sx = sx;
    src->sy = sy;
    src->alpha = alpha;
}

void DoBgAffineSet(struct BgAffineDstData *dest, u32 texX, u32 texY, s16 scrX, s16 scrY, s16 sx, s16 sy, u16 alpha)
{
    struct BgAffineSrcData src;
    SetBgAffineStruct(&src, texX, texY, scrX, scrY, sx, sy, alpha);
    BgAffineSet(&src, dest, 1);
}

bool32 JumpBasedOnKind(u32 value, u32 cmpKind, u32 cmpTo)
{
    bool32 ret = FALSE;
    
    switch (cmpKind)
    {
        case CMP_EQUAL:
            if (value == cmpTo)
                ret = TRUE;
            break;
        case CMP_NOT_EQUAL:
            if (value != cmpTo)
                ret = TRUE;
            break;
        case CMP_GREATER_THAN:
            if (value > cmpTo)
                ret = TRUE;
            break;
        case CMP_LESS_THAN:
            if (value < cmpTo)
                ret = TRUE;
            break;
        case CMP_COMMON_BITS:
            if (value & cmpTo)
                ret = TRUE;
            break;
        case CMP_NO_COMMON_BITS:
            if (!(value & cmpTo))
                ret = TRUE;
            break;
    }
    return ret;
}

#define SORT(unionVar)                                                      \
    if (aux == NULL)                                                        \
        return;                                                             \
                                                                            \
    for (k = low; k <= high; ++k)                                           \
        aux[k] = comparator->sortUnion.unionVar.array[k];                   \
                                                                            \
    /* Merge back to a[low..high] */                                        \
    for (k = low; k <= high; ++k)                                           \
    {                                                                       \
        if (i > mid)                                                        \
            comparator->sortUnion.unionVar.array[k] = aux[j++];             \
        else if (j > high)                                                  \
            comparator->sortUnion.unionVar.array[k] = aux[i++];             \
        else if (comparator->sortUnion.unionVar.func(&aux[j], &aux[i]) < 0) \
            comparator->sortUnion.unionVar.array[k] = aux[j++];             \
        else                                                                \
            comparator->sortUnion.unionVar.array[k] = aux[i++];             \
    }                                                                       \
    Free(aux)

void MergeSort(struct SortComparator *comparator, u32 low, u32 high)
{
    u32 mid, i, j, k;

    if (high <= low)
        return;

    mid = low + (high - low) / 2;
    MergeSort(comparator, low, mid); // Sort left half.
    MergeSort(comparator, mid + 1, high); // Sort right half.
    
    // Merge results.
    i = low;
    j = mid + 1;
    
    switch (comparator->kind)
    {
        case SORT_BAG_ITEMS:
        {
            struct ItemSlot *aux = Alloc(sizeof(struct ItemSlot) * (high + 1));
            
            SORT(bagItemSort);
            return;
        }
        case SORT_LIST_MENU_ITEMS:
        {
            struct ListMenuItem *aux = Alloc(sizeof(struct ListMenuItem) * (high + 1));
            
            SORT(listMenuItemSort);
            return;
        }
    }
}

// Returns -1 if text1 goes before text2 alphabetically, otherwise returns 1
// Returns 0 if the two texts are identical
s8 CompareTextAlphabetically(const u8 *text1, const u8 *text2)
{
    u32 i;
    
    for (i = 0; ; ++i)
    {
        if (text1[i] == EOS && text2[i] != EOS)
            return -1;
        else if (text1[i] != EOS && text2[i] == EOS)
            return 1;
        else if (text1[i] == EOS && text2[i] == EOS)
            return 0;

        if (text1[i] < text2[i])
            return -1;
        else if (text1[i] > text2[i])
            return 1;
    }
    return 0;
}

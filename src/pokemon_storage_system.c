#include "global.h"
#include "form_change.h"
#include "gflib.h"
#include "pokemon_storage_system_internal.h"
#include "constants/form_change.h"

// Functions here are general utility functions.
u32 StorageGetCurrentBox(void)
{
    return gPokemonStoragePtr->currentBox;
}

void SetCurrentBox(u32 boxId)
{
    if (boxId < TOTAL_BOXES_COUNT)
        gPokemonStoragePtr->currentBox = boxId;
}

static inline bool32 IsValidBoxIdAndPosition(u32 boxId, u32 boxPosition)
{
    return (boxId < TOTAL_BOXES_COUNT && boxPosition < IN_BOX_COUNT);
}

u32 GetAndCopyBoxMonDataAt(u32 boxId, u32 boxPosition, s32 request, void *dst)
{
    return IsValidBoxIdAndPosition(boxId, boxPosition) ? GetBoxMonData(&gPokemonStoragePtr->boxes[boxId][boxPosition], request, dst) : 0;
}

u32 GetBoxMonDataAt(u32 boxId, u32 boxPosition, s32 request)
{
    return GetAndCopyBoxMonDataAt(boxId, boxPosition, request, NULL);
}

static void SetBoxMonDataAt(u32 boxId, u32 boxPosition, s32 request, const void *value)
{
    if (IsValidBoxIdAndPosition(boxId, boxPosition))
        SetBoxMonData(&gPokemonStoragePtr->boxes[boxId][boxPosition], request, value);
}

u32 GetCurrentBoxMonData(u32 boxPosition, s32 request)
{
    return GetBoxMonDataAt(gPokemonStoragePtr->currentBox, boxPosition, request);
}

void SetCurrentBoxMonData(u32 boxPosition, s32 request, const void *value)
{
    SetBoxMonDataAt(gPokemonStoragePtr->currentBox, boxPosition, request, value);
}

void SetBoxMonNickAt(u32 boxId, u32 boxPosition, const u8 *nick)
{
    if (IsValidBoxIdAndPosition(boxId, boxPosition))
        SetBoxMonData(&gPokemonStoragePtr->boxes[boxId][boxPosition], MON_DATA_NICKNAME, nick);
}

void SetBoxMonAt(u32 boxId, u32 boxPosition, struct BoxPokemon * src)
{
    if (IsValidBoxIdAndPosition(boxId, boxPosition))
    {
        DoOverworldFormChange((struct Pokemon*)src, FORM_CHANGE_COUNTDOWN);
        gPokemonStoragePtr->boxes[boxId][boxPosition] = *src;
    }
}

void ZeroBoxMonAt(u32 boxId, u32 boxPosition)
{
    if (IsValidBoxIdAndPosition(boxId, boxPosition))
        ZeroBoxMonData(&gPokemonStoragePtr->boxes[boxId][boxPosition]);
}

void BoxMonAtToMon(u32 boxId, u32 boxPosition, struct Pokemon * dst)
{
    if (IsValidBoxIdAndPosition(boxId, boxPosition))
        BoxMonToMon(&gPokemonStoragePtr->boxes[boxId][boxPosition], dst);
}

struct BoxPokemon * GetBoxedMonPtr(u32 boxId, u32 boxPosition)
{
    return IsValidBoxIdAndPosition(boxId, boxPosition) ? &gPokemonStoragePtr->boxes[boxId][boxPosition] : NULL;
}

u8 *GetBoxNamePtr(u32 boxId)
{
    return (boxId < TOTAL_BOXES_COUNT) ? gPokemonStoragePtr->boxNames[boxId] : NULL;
}

u32 GetBoxWallpaper(u32 boxId)
{
    return (boxId < TOTAL_BOXES_COUNT) ? gPokemonStoragePtr->boxWallpapers[boxId] : 0;
}

void SetBoxWallpaper(u32 boxId, u32 wallpaperId)
{
    if (boxId < TOTAL_BOXES_COUNT && wallpaperId < WALLPAPER_COUNT)
        gPokemonStoragePtr->boxWallpapers[boxId] = wallpaperId;
}

s16 SeekToNextMonInBox(struct BoxPokemon * boxMons, s8 curIndex, u32 maxIndex, u32 flags)
{
    // flags:
    // bit 0: Allow eggs
    // bit 1: Search backwards
    s16 i;
    s16 adder;
    
    if (flags == 0 || flags == 1)
        adder = 1;
    else
        adder = -1;

    if (flags == 1 || flags == 3)
    {
        for (i = curIndex + adder; i >= 0 && i <= maxIndex; i += adder)
        {
            if (GetBoxMonData(&boxMons[i], MON_DATA_SPECIES))
                return i;
        }
    }
    else
    {
        for (i = curIndex + adder; i >= 0 && i <= maxIndex; i += adder)
        {
            if (GetBoxMonData(&boxMons[i], MON_DATA_SPECIES) && !GetBoxMonData(&boxMons[i], MON_DATA_IS_EGG))
                return i;
        }
    }
    return -1;
}

#ifndef GUARD_POKEMON_STORAGE_SYSTEM_H
#define GUARD_POKEMON_STORAGE_SYSTEM_H

#include "global.h"

#define TOTAL_BOXES_COUNT       14
#define IN_BOX_COUNT            30

enum
{
    WALLPAPER_FOREST,
    WALLPAPER_CITY,
    WALLPAPER_DESERT,
    WALLPAPER_SAVANNA,
    WALLPAPER_CRAG,
    WALLPAPER_VOLCANO,
    WALLPAPER_SNOW,
    WALLPAPER_CAVE,
    WALLPAPER_BEACH,
    WALLPAPER_SEAFLOOR,
    WALLPAPER_RIVER,
    WALLPAPER_SKY,
    WALLPAPER_POLKADOT,
    WALLPAPER_POKECENTER,
    WALLPAPER_MACHINE,
    WALLPAPER_PLAIN,
    WALLPAPER_COUNT
};

u8 *GetBoxNamePtr(u32 boxNumber);
struct BoxPokemon *GetBoxedMonPtr(u32 boxId, u32 monPosition);
void SetBoxMonNickAt(u32 boxId, u32 monPosition, const u8 *newNick);
s16 CompactPartySlots(void);
u32 GetBoxMonDataAt(u32 boxId, u32 monPosition, s32 request);
void ZeroBoxMonAt(u32 boxId, u32 monPosition);
void Cb2_ReturnToPSS(void);
void ResetPokemonStorageSystem(void);
u32 StorageGetCurrentBox(void);
void DrawTextWindowAndBufferTiles(const u8 *string, void *dst, u32 zero1, u32 zero2, u8 *buffer, s32 bytesToBuffer);
u32 CountPartyNonEggMons(void);
void UpdatePcMonIconSpecies(void);
s16 SeekToNextMonInBox(struct BoxPokemon * boxMons, s8 curIndex, u32 maxIndex, u32 flags);

extern const u8 gString_Bill[];
extern const u8 gString_Someone[];
extern const u8 gText_SomeoneSPc[];
extern const u8 gText_BillSPc[];
extern const u8 gText_SPc[];
extern const u8 gText_ProfOakSPc[];

#endif // GUARD_POKEMON_STORAGE_SYSTEM_H

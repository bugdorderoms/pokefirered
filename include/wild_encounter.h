#ifndef GUARD_WILD_ENCOUNTER_H
#define GUARD_WILD_ENCOUNTER_H

#include "global.h"

#define LAND_WILD_COUNT     12
#define WATER_WILD_COUNT    5
#define ROCK_WILD_COUNT     5
#define FISH_WILD_COUNT     10

#define WILD_HEADER_LAND       0
#define WILD_HEADER_WATER      1
#define WILD_HEADER_ROCK_SMASH 2
#define WILD_HEADER_FISH       3

struct WildPokemon
{
    u8 minLevel;
    u8 maxLevel;
    u16 species;
};

struct WildPokemonInfo
{
    u8 encounterRate;
    const struct WildPokemon *wildPokemon;
};

struct WildPokemonHeader
{
    u8 mapGroup;
    u8 mapNum;
    const struct WildPokemonInfo *landMonsInfo;
    const struct WildPokemonInfo *waterMonsInfo;
    const struct WildPokemonInfo *rockSmashMonsInfo;
    const struct WildPokemonInfo *fishingMonsInfo;
    const struct WildPokemonInfo *landMonsInfoNight;
    const struct WildPokemonInfo *waterMonsInfoNight;
    const struct WildPokemonInfo *rockSmashMonsInfoNight;
    const struct WildPokemonInfo *fishingMonsInfoNight;
};

extern const struct WildPokemonHeader gWildMonHeaders[];
extern u8 gChainFishingStreak;
extern bool8 gIsFishingEncounter;
extern bool8 gIsSurfingEncounter;

void SeedWildEncounterRng(u16 randVal);
void ResetEncounterRateModifiers(void);
void ToggleWildEncountersState(bool32 disabled);
u32 GetCurrentMapWildMonHeaderId(void);
const struct WildPokemonInfo *GetWildPokemonInfoByHeaderType(u16 headerId, u32 type);
u32 GenerateWildMon(u32 species, u32 level, bool32 checkWildInfluence);
bool32 TryStandardWildEncounter(u32 currMetatileAttrs);
void FishingWildEncounter(u32 rod);
bool32 DoesCurrentMapHaveFishingMons(void);
bool32 TrySweetScentWildEncounter(void);
u32 GetWildMonForAmbientCry(bool8 *isWaterMon);
bool32 UpdateRepelCounter(void);
u32 TryGetForcedWildMonNature(struct Pokemon *mon, u32 ability);

#endif // GUARD_WILD_ENCOUNTER_H

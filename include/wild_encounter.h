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

void DisableWildEncounters(bool8 disabled);
bool8 StandardWildEncounter(u32 currMetatileAttrs, u16 previousMetaTileBehavior);
bool8 SweetScentWildEncounter(void);
bool8 DoesCurrentMapHaveFishingMons(void);
void FishingWildEncounter(u8 rod);
u16 GetLocalWildMon(bool8 *isWaterMon);
u16 GetLocalWaterMon(void);
bool8 UpdateRepelCounter(void);
void DisableWildEncounters(bool8 state);
u8 GetUnownLetterByPersonalityLoByte(u32 personality);
bool8 SweetScentWildEncounter(void);
void SeedWildEncounterRng(u16 randVal);
void ResetEncounterRateModifiers(void);
bool8 TryStandardWildEncounter(u32 currMetatileAttrs);
const struct WildPokemonInfo *GetWildPokemonInfoByHeaderType(u16 headerId, u8 type);
void GenerateWildMon(u16 species, u8 level, u8 slot);
u16 GetCurrentMapWildMonHeaderId(void);

#endif // GUARD_WILD_ENCOUNTER_H

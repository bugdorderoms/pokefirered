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
void ToggleWildEncountersState(bool8 disabled);
u16 GetCurrentMapWildMonHeaderId(void);
const struct WildPokemonInfo *GetWildPokemonInfoByHeaderType(u16 headerId, u8 type);
u16 GenerateWildMon(u16 species, u8 level, bool8 checkWildInfluence);
bool8 TryStandardWildEncounter(u32 currMetatileAttrs);
void FishingWildEncounter(u8 rod);
bool8 DoesCurrentMapHaveFishingMons(void);
bool8 TrySweetScentWildEncounter(void);
u16 GetWildMonForAmbientCry(bool8 *isWaterMon);
bool8 UpdateRepelCounter(void);

#endif // GUARD_WILD_ENCOUNTER_H

#include "global.h"
#include "dns.h"
#include "random.h"
#include "wild_encounter.h"
#include "event_data.h"
#include "fieldmap.h"
#include "field_weather.h"
#include "form_change.h"
#include "roamer.h"
#include "field_player_avatar.h"
#include "battle_setup.h"
#include "overworld.h"
#include "ride_pager.h"
#include "safari_zone.h"
#include "metatile_behavior.h"
#include "event_scripts.h"
#include "script.h"
#include "link.h"
#include "constants/maps.h"
#include "constants/abilities.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/weather.h"

struct WildEncounterData
{
    u32 rngState;
    u16 prevMetatileBehavior;
    u16 encounterRateBuff;
	u8 stepsSinceLastEncounter;
	u8 leadMonAbilityEffectId;
	u16 leadMonHeldItem;
};

static EWRAM_DATA struct WildEncounterData sWildEncounterData = {0};
static EWRAM_DATA bool8 sWildEncountersDisabled = FALSE;
static EWRAM_DATA u16 sLastFishingSpecies = 0;
EWRAM_DATA u8 gChainFishingStreak = 0;
EWRAM_DATA bool8 gIsFishingEncounter = FALSE;
EWRAM_DATA bool8 gIsSurfingEncounter = FALSE;

static void AddToWildEncounterRateBuff(u8 encouterRate);
static bool8 HandleWildEncounterCooldown(u32 currMetatileAttrs);
static bool8 DoWildEncounterRateTest(u32 encounterRate, bool8 ignoreAbility);
static u8 ChooseWildMonLevel(const struct WildPokemon * info);

#include "data/wild_encounters.h"

void ToggleWildEncountersState(bool8 disabled)
{
    sWildEncountersDisabled = disabled;
}

bool8 UpdateRepelCounter(void)
{
    u16 steps;

    if (InUnionRoom())
        return FALSE;

    steps = VarGet(VAR_REPEL_STEP_COUNT);

    if (steps != 0)
    {
        steps--;
        VarSet(VAR_REPEL_STEP_COUNT, steps);
		
        if (steps == 0)
        {
            ScriptContext1_SetupScript(EventScript_RepelWoreOff);
            return TRUE;
        }
    }
    return FALSE;
}

static bool8 IsWildLevelAllowedByRepel(u8 level)
{
    u8 i;

    if (!VarGet(VAR_REPEL_STEP_COUNT))
        return TRUE;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (MonCanBattle(&gPlayerParty[i]))
            return (level >= GetMonData(&gPlayerParty[i], MON_DATA_LEVEL));
    }
    return FALSE;
}

static bool8 IsAbilityAllowingEncounter(u8 level)
{
	if (IsMonValidSpecies(&gPlayerParty[0]))
	{
		u8 leadMonLevel = GetMonData(&gPlayerParty[0], MON_DATA_LEVEL);
		
		switch (GetMonAbility(&gPlayerParty[0]))
		{
			case ABILITY_KEEN_EYE:
			case ABILITY_INTIMIDATE:
				if (RandomPercent(50) && leadMonLevel > 5 && level <= leadMonLevel - 5)
					return FALSE;
				break;
		}
	}
	return TRUE;
}

void SeedWildEncounterRng(u16 seed)
{
    sWildEncounterData.rngState = seed;
    ResetEncounterRateModifiers();
}

void ResetEncounterRateModifiers(void)
{
    sWildEncounterData.encounterRateBuff = 0;
    sWildEncounterData.stepsSinceLastEncounter = 0;
}

static void AddToWildEncounterRateBuff(u8 encounterRate)
{
    if (VarGet(VAR_REPEL_STEP_COUNT) == 0)
        sWildEncounterData.encounterRateBuff += encounterRate;
    else
        sWildEncounterData.encounterRateBuff = 0;
}

static u16 WildEncounterRandom(void)
{
    sWildEncounterData.rngState = ISO_RANDOMIZE2(sWildEncounterData.rngState);
    return sWildEncounterData.rngState >> 16;
}

static bool8 DoWildEncounterRateDiceRoll(u16 rate)
{
    if (WildEncounterRandom() % 1600 < rate)
        return TRUE;
    return FALSE;
}

static inline bool8 DoGlobalWildEncounterDiceRoll(void)
{
	return RandomPercent(60);
}

static bool8 UnlockedTanobyOrAreNotInTanoby(void)
{
    if (FlagGet(FLAG_SYS_UNLOCKED_TANOBY_RUINS) || gSaveBlock1Ptr->location.mapGroup != MAP_GROUP(SEVEN_ISLAND_TANOBY_RUINS_DILFORD_CHAMBER))
        return TRUE;

    if (!(gSaveBlock1Ptr->location.mapNum == MAP_NUM(SEVEN_ISLAND_TANOBY_RUINS_MONEAN_CHAMBER)
		|| gSaveBlock1Ptr->location.mapNum == MAP_NUM(SEVEN_ISLAND_TANOBY_RUINS_LIPTOO_CHAMBER)
		|| gSaveBlock1Ptr->location.mapNum == MAP_NUM(SEVEN_ISLAND_TANOBY_RUINS_WEEPTH_CHAMBER)
		|| gSaveBlock1Ptr->location.mapNum == MAP_NUM(SEVEN_ISLAND_TANOBY_RUINS_DILFORD_CHAMBER)
		|| gSaveBlock1Ptr->location.mapNum == MAP_NUM(SEVEN_ISLAND_TANOBY_RUINS_SCUFIB_CHAMBER)
		|| gSaveBlock1Ptr->location.mapNum == MAP_NUM(SEVEN_ISLAND_TANOBY_RUINS_RIXY_CHAMBER)
		|| gSaveBlock1Ptr->location.mapNum == MAP_NUM(SEVEN_ISLAND_TANOBY_RUINS_VIAPOIS_CHAMBER)))
        return TRUE;
		
    return FALSE;
}

u16 GetCurrentMapWildMonHeaderId(void)
{
    u16 i;

    for (i = 0; ; i++)
    {
        const struct WildPokemonHeader * wildHeader = &gWildMonHeaders[i];
        if (wildHeader->mapGroup == MAP_GROUP(UNDEFINED))
            break;

        if (gWildMonHeaders[i].mapGroup == gSaveBlock1Ptr->location.mapGroup && gWildMonHeaders[i].mapNum == gSaveBlock1Ptr->location.mapNum)
        {
            if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(SIX_ISLAND_ALTERING_CAVE) && gSaveBlock1Ptr->location.mapNum == MAP_NUM(SIX_ISLAND_ALTERING_CAVE))
            {
                u16 alteringCaveId = VarGet(VAR_ALTERING_CAVE_WILD_SET);
                if (alteringCaveId > 8)
                    alteringCaveId = 0;

                i += alteringCaveId;
            }

            if (!UnlockedTanobyOrAreNotInTanoby())
                break;
			
            return i;
        }
    }
    return -1;
}

#define GET_WILD_INFO(monsInfo, dayOrNight)                  \
    if (dayOrNight == TIME_NIGHT && monsInfo##Night != NULL) \
		return monsInfo##Night;                              \
	else                                                     \
		return monsInfo

const struct WildPokemonInfo *GetWildPokemonInfoByHeaderType(u16 headerId, u8 type)
{
	u8 dayOrNight = GetDNSTimeLapseDayOrNight();
	
	switch (type)
	{
		case WILD_HEADER_LAND:
			GET_WILD_INFO(gWildMonHeaders[headerId].landMonsInfo, dayOrNight);
		case WILD_HEADER_WATER:
		    GET_WILD_INFO(gWildMonHeaders[headerId].waterMonsInfo, dayOrNight);
		case WILD_HEADER_ROCK_SMASH:
		    GET_WILD_INFO(gWildMonHeaders[headerId].rockSmashMonsInfo, dayOrNight);
		case WILD_HEADER_FISH:
		    GET_WILD_INFO(gWildMonHeaders[headerId].fishingMonsInfo, dayOrNight);
	}
}

/////////////////////////////////////
// CHOOSE WILD ENCOUNTER FUNCTIONS //
/////////////////////////////////////

static bool8 TryGetAbilityInfluencedWildMonIndex(const struct WildPokemon *wildMon, u8 count, u8 *slot)
{
	u8 type;
	
	if (RandomPercent(50) && IsMonValidSpecies(&gPlayerParty[0]))
	{
		switch (GetMonAbility(&gPlayerParty[0]))
		{
			case ABILITY_FLASH_FIRE:
				type = TYPE_FIRE;
				break;
			case ABILITY_HARVEST:
				type = TYPE_GRASS;
				break;
			case ABILITY_LIGHTNING_ROD:
			case ABILITY_STATIC:
				type = TYPE_ELECTRIC;
				break;
			case ABILITY_MAGNET_PULL:
				type = TYPE_STEEL;
				break;
			case ABILITY_STORM_DRAIN:
				type = TYPE_WATER;
				break;
			default:
				type = NUMBER_OF_MON_TYPES;
				break;
		}
		
		if (type != NUMBER_OF_MON_TYPES)
		{
			u8 i, validIndexes[count], numValidMons = 0;
			
			for (i = 0; i < count; i++)
			{
				if (gSpeciesInfo[wildMon[i].species].types[0] == type || gSpeciesInfo[wildMon[i].species].types[1] == type)
					validIndexes[numValidMons++] = i;
			}
			
			if (numValidMons && numValidMons != count)
			{
				*slot = validIndexes[RandomMax(numValidMons)];
				return TRUE;
			}
		}
	}
	return FALSE;
}

static u8 ChooseWildMonIndex_Land(const struct WildPokemon *wildPokemon)
{
    u8 rand;
	
	if (wildPokemon != NULL && TryGetAbilityInfluencedWildMonIndex(wildPokemon, LAND_WILD_COUNT, &rand))
		return rand;
	
	rand = RandomMax(ENCOUNTER_CHANCE_LAND_MONS_TOTAL);
	
    if (rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_0)
        return 0;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_0 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_1)
        return 1;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_1 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_2)
        return 2;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_2 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_3)
        return 3;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_3 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_4)
        return 4;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_4 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_5)
        return 5;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_5 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_6)
        return 6;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_6 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_7)
        return 7;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_7 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_8)
        return 8;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_8 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_9)
        return 9;
    else if (rand >= ENCOUNTER_CHANCE_LAND_MONS_SLOT_9 && rand < ENCOUNTER_CHANCE_LAND_MONS_SLOT_10)
        return 10;
    else
        return 11;
}

static u8 ChooseWildMonIndex_WaterRock(const struct WildPokemon *wildPokemon)
{
    u8 rand;
	
	if (wildPokemon != NULL && TryGetAbilityInfluencedWildMonIndex(wildPokemon, WATER_WILD_COUNT, &rand))
		return rand;
	
	rand = RandomMax(ENCOUNTER_CHANCE_WATER_MONS_TOTAL);
	
    if (rand < ENCOUNTER_CHANCE_WATER_MONS_SLOT_0)
        return 0;
    else if (rand >= ENCOUNTER_CHANCE_WATER_MONS_SLOT_0 && rand < ENCOUNTER_CHANCE_WATER_MONS_SLOT_1)
        return 1;
    else if (rand >= ENCOUNTER_CHANCE_WATER_MONS_SLOT_1 && rand < ENCOUNTER_CHANCE_WATER_MONS_SLOT_2)
        return 2;
    else if (rand >= ENCOUNTER_CHANCE_WATER_MONS_SLOT_2 && rand < ENCOUNTER_CHANCE_WATER_MONS_SLOT_3)
        return 3;
    else
        return 4;
}

// Always have ability influence
static u8 ChooseWildMonIndex_Fishing(const struct WildPokemon *wildPokemon, u8 rod)
{
    u8 rand, wildMonIndex;
							 
	if (TryGetAbilityInfluencedWildMonIndex(wildPokemon, FISH_WILD_COUNT, &wildMonIndex))
		return wildMonIndex;
	
	wildMonIndex = 0;
	rand = RandomMax(max(max(ENCOUNTER_CHANCE_FISHING_MONS_OLD_ROD_TOTAL, ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_TOTAL), ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_TOTAL));
	
    switch (rod)
    {
    case 0:
        if (rand < ENCOUNTER_CHANCE_FISHING_MONS_OLD_ROD_SLOT_0)
            wildMonIndex = 0;
        else
            wildMonIndex = 1;
        break;
    case 1:
        if (rand < ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_SLOT_2)
            wildMonIndex = 2;
        else if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_SLOT_2 && rand < ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_SLOT_3)
            wildMonIndex = 3;
        else if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_SLOT_3 && rand < ENCOUNTER_CHANCE_FISHING_MONS_GOOD_ROD_SLOT_4)
            wildMonIndex = 4;
        break;
    case 2:
        if (rand < ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_5)
            wildMonIndex = 5;
        else if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_5 && rand < ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_6)
            wildMonIndex = 6;
        else if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_6 && rand < ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_7)
            wildMonIndex = 7;
        else if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_7 && rand < ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_8)
            wildMonIndex = 8;
        else if (rand >= ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_8 && rand < ENCOUNTER_CHANCE_FISHING_MONS_SUPER_ROD_SLOT_9)
            wildMonIndex = 9;
        break;
    }
    return wildMonIndex;
}

/////////////////////////
// WILD MON GENERATION //
/////////////////////////

static u8 TryGetForcedWildMonGender(u16 species)
{
	switch (gSpeciesInfo[species].genderRatio)
	{
		case MON_MALE:
		case MON_FEMALE:
		case MON_GENDERLESS:
			return MON_GENDERLESS; // No forced gender
	}
	
	if (IsMonValidSpecies(&gPlayerParty[0]))
	{
		switch (GetMonAbility(&gPlayerParty[0]))
		{
			case ABILITY_CUTE_CHARM:
				if (RandomMax(3))
				{
					switch (GetMonGender(&gPlayerParty[0]))
					{
						case MON_MALE:
							return MON_FEMALE;
						case MON_FEMALE:
							return MON_MALE;
					}
				}
				break;
		}
	}
	return MON_GENDERLESS; // No forced gender
}

static u8 TryGetForcedWildMonNature(void)
{
	if (IsMonValidSpecies(&gPlayerParty[0]))
	{
		switch (GetMonAbility(&gPlayerParty[0]))
		{
			case ABILITY_SYNCHRONIZE:
				if (RandomPercent(50))
					return GetNatureFromPersonality(GetMonData(&gPlayerParty[0], MON_DATA_PERSONALITY)); // Ignore Mints
				break;
		}
	}
	return NUM_NATURES; // No forced nature
}

u16 GenerateWildMon(u16 species, u8 level, bool8 checkWildInfluence)
{
	struct PokemonGenerator generator =
	{
		.species = species,
		.level = level,
		.forcedGender = checkWildInfluence ? TryGetForcedWildMonGender(species) : MON_GENDERLESS,
		.otIdType = OT_ID_PLAYER_ID,
		.hasFixedPersonality = FALSE,
		.fixedPersonality = 0,
		.shinyType = GENERATE_SHINY_NORMAL,
		.forcedNature = checkWildInfluence ? TryGetForcedWildMonNature() : NUM_NATURES,
		.formChanges = gDeafultGeneratorFormChanges,
		.moves = {MOVE_NONE, MOVE_NONE, MOVE_NONE, MOVE_NONE},
	};
	
    ZeroEnemyPartyMons();
	CreateMon(&gEnemyParty[0], generator);
	
	return GetMonData(&gEnemyParty[0], MON_DATA_SPECIES);
}

static bool8 TryGenerateWildMon(const struct WildPokemonInfo * info, u8 headerType, bool8 checkLevelAllowed)
{
    u8 level, slot = 0;

    switch (headerType)
    {
		case WILD_HEADER_LAND:
			slot = ChooseWildMonIndex_Land(info->wildPokemon);
			break;
		case WILD_HEADER_WATER:
		case WILD_HEADER_ROCK_SMASH:
			slot = ChooseWildMonIndex_WaterRock(info->wildPokemon);
			break;
    }
    level = ChooseWildMonLevel(&info->wildPokemon[slot]);
	
    if (checkLevelAllowed && (!IsWildLevelAllowedByRepel(level) || !IsAbilityAllowingEncounter(level)))
        return FALSE;

    GenerateWildMon(info->wildPokemon[slot].species, level, TRUE);
	
    return TRUE;
}

static bool8 TryDoDoubleWildBattle(void)
{
	if (GetSafariZoneFlag() || GetMonsStateToDoubles() != PLAYER_HAS_TWO_USABLE_MONS)
		return FALSE;
	else if (FlagGet(FLAG_DOUBLE_WILD_BATTLE))
	{
		FlagClear(FLAG_DOUBLE_WILD_BATTLE);
		return TRUE;
	}
	else
	{
#if DOUBLE_WILD_BATTLE_CHANCE != 0
        if (RandomPercent(DOUBLE_WILD_BATTLE_CHANCE))
			return TRUE;
#endif
	    return FALSE;
    }
}

static bool8 TryStartStandardWildEncounter(u32 currMetatileAttrs, u16 previousMetatileBehavior)
{
	u8 headerType;
    u16 headerId;
    struct Roamer * roamer;
    const struct WildPokemonInfo * monInfo;

    if (sWildEncountersDisabled)
        return FALSE;

    headerId = GetCurrentMapWildMonHeaderId();
	
    if (headerId != 0xFFFF)
    {
		u8 encounterType = ExtractMetatileAttribute(currMetatileAttrs, METATILE_ATTRIBUTE_ENCOUNTER_TYPE);
		
		if (encounterType == TILE_ENCOUNTER_LAND)
			headerType = WILD_HEADER_LAND;
		else if (encounterType == TILE_ENCOUNTER_WATER || (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING) && MetatileBehavior_IsBridge(ExtractMetatileAttribute(currMetatileAttrs, METATILE_ATTRIBUTE_BEHAVIOR))))
			headerType = WILD_HEADER_WATER;
		
		monInfo = GetWildPokemonInfoByHeaderType(headerId, headerType);
		
		if (monInfo == NULL || (previousMetatileBehavior != ExtractMetatileAttribute(currMetatileAttrs, METATILE_ATTRIBUTE_BEHAVIOR)
			&& !DoGlobalWildEncounterDiceRoll()))
			return FALSE;

		if (!DoWildEncounterRateTest(monInfo->encounterRate, FALSE))
		{
			AddToWildEncounterRateBuff(monInfo->encounterRate);
			return FALSE;
		}
		else if (TryStartRoamerEncounter())
		{
			roamer = &gSaveBlock1Ptr->roamer;
			
			if (IsWildLevelAllowedByRepel(roamer->level))
			{
				StartRoamerBattle();
                return TRUE;
			}
		}
		else
		{
			if (TryGenerateWildMon(monInfo, headerType, TRUE))
			{
				if (headerType == WILD_HEADER_WATER)
					gIsSurfingEncounter = TRUE;
				
				if (TryDoDoubleWildBattle())
				{
					struct Pokemon mon1;
					CopyMon(&mon1, &gEnemyParty[0], sizeof(struct Pokemon));
					TryGenerateWildMon(monInfo, headerType, FALSE);
					CopyMon(&gEnemyParty[1], &mon1, sizeof(struct Pokemon));
					StartDoubleWildBattle();
				}
				else
					StartWildBattle();
				
				return TRUE;
			}
			else
				AddToWildEncounterRateBuff(monInfo->encounterRate);
		}
    }
    return FALSE;
}

bool8 TryStandardWildEncounter(u32 currMetatileAttrs)
{
    if (!HandleWildEncounterCooldown(currMetatileAttrs))
    {
        sWildEncounterData.prevMetatileBehavior = ExtractMetatileAttribute(currMetatileAttrs, METATILE_ATTRIBUTE_BEHAVIOR);
        return FALSE;
    }
    else if (TryStartStandardWildEncounter(currMetatileAttrs, sWildEncounterData.prevMetatileBehavior))
    {
        sWildEncounterData.encounterRateBuff = 0;
        sWildEncounterData.stepsSinceLastEncounter = 0;
        sWildEncounterData.prevMetatileBehavior = ExtractMetatileAttribute(currMetatileAttrs, METATILE_ATTRIBUTE_BEHAVIOR);
        return TRUE;
    }
    else
    {
        sWildEncounterData.prevMetatileBehavior = ExtractMetatileAttribute(currMetatileAttrs, METATILE_ATTRIBUTE_BEHAVIOR);
        return FALSE;
    }
}

static u16 GenerateFishingEncounter(const struct WildPokemonInfo * info, u8 rod)
{
    u8 slot = ChooseWildMonIndex_Fishing(info->wildPokemon, rod);
    return GenerateWildMon(info->wildPokemon[slot].species, ChooseWildMonLevel(&info->wildPokemon[slot]), TRUE);
}

void FishingWildEncounter(u8 rod)
{
    u16 species;
    
    gIsFishingEncounter = TRUE;
    species = GenerateFishingEncounter(GetWildPokemonInfoByHeaderType(GetCurrentMapWildMonHeaderId(), WILD_HEADER_FISH), rod);
    
    if (species == sLastFishingSpecies)
    {
        if (gChainFishingStreak < 20)
            gChainFishingStreak++;
    }
    else
        sLastFishingSpecies = species;
	
    IncrementGameStat(GAME_STAT_FISHING_CAPTURES);
    StartWildBattle();
}

bool8 DoesCurrentMapHaveFishingMons(void)
{
    u16 headerIdx = GetCurrentMapWildMonHeaderId();
	
    if (headerIdx == 0xFFFF || GetWildPokemonInfoByHeaderType(headerIdx, WILD_HEADER_FISH) == NULL)
        return FALSE;

    return TRUE;
}

bool8 TrySweetScentWildEncounter(void)
{
	u8 headerType;
    s16 x, y;
	const struct WildPokemonInfo * monInfo;
	u16 headerId = GetCurrentMapWildMonHeaderId();
	
    if (headerId != 0xFFFF)
    {
		PlayerGetDestCoords(&x, &y);
		
		switch (MapGridGetMetatileAttributeAt(x, y, METATILE_ATTRIBUTE_ENCOUNTER_TYPE))
		{
			case TILE_ENCOUNTER_LAND:
				headerType = WILD_HEADER_LAND;
				break;
			case TILE_ENCOUNTER_WATER:
				headerType = WILD_HEADER_WATER;
				break;
			case TILE_ENCOUNTER_NONE:
				return FALSE;
		}
		
		if (TryStartRoamerEncounter())
		{
			StartRoamerBattle();
			return TRUE;
		}
		else
		{
			monInfo = GetWildPokemonInfoByHeaderType(headerId, headerType);
	
			if (monInfo == NULL)
				return FALSE;
	
			TryGenerateWildMon(monInfo, headerType, FALSE);
			StartWildBattle();
			
			return TRUE;
		}
    }
    return FALSE;
}

void RockSmashWildEncounter(void)
{
    u16 headerIdx = GetCurrentMapWildMonHeaderId();
	const struct WildPokemonInfo * monInfo;
	
    if (headerIdx == 0xFFFF)
        gSpecialVar_Result = FALSE;
	else
	{
		monInfo = GetWildPokemonInfoByHeaderType(headerIdx, WILD_HEADER_ROCK_SMASH);
		
		if (monInfo == NULL || !DoWildEncounterRateTest(monInfo->encounterRate, TRUE))
			gSpecialVar_Result = FALSE;
		else if (TryGenerateWildMon(monInfo, WILD_HEADER_ROCK_SMASH, TRUE))
		{
			StartWildBattle();
			gSpecialVar_Result = TRUE;
		}
		else
			gSpecialVar_Result = FALSE;
	}
}

u16 GetWildMonForAmbientCry(bool8 *isWaterMon)
{
    u16 headerId;
    const struct WildPokemonInfo * landMonsInfo;
    const struct WildPokemonInfo * waterMonsInfo;

    *isWaterMon = FALSE;
	
    headerId = GetCurrentMapWildMonHeaderId();
    if (headerId == 0xFFFF)
        return SPECIES_NONE;
	
    landMonsInfo = GetWildPokemonInfoByHeaderType(headerId, WILD_HEADER_LAND);
    waterMonsInfo = GetWildPokemonInfoByHeaderType(headerId, WILD_HEADER_WATER);
	
    // Neither
    if (landMonsInfo == NULL && waterMonsInfo == NULL)
        return SPECIES_NONE;
    else if (landMonsInfo != NULL && waterMonsInfo == NULL) // Land Pokemon
        return landMonsInfo->wildPokemon[ChooseWildMonIndex_Land(NULL)].species;
    else if (landMonsInfo == NULL && waterMonsInfo != NULL) // Water Pokemon
    {
        *isWaterMon = TRUE;
        return waterMonsInfo->wildPokemon[ChooseWildMonIndex_WaterRock(NULL)].species;
    }
	
    // Either land or water Pokemon
    if (RandomPercent(80))
        return landMonsInfo->wildPokemon[ChooseWildMonIndex_Land(NULL)].species;
    else
    {
        *isWaterMon = TRUE;
        return waterMonsInfo->wildPokemon[ChooseWildMonIndex_WaterRock(NULL)].species;
    }
}

static u8 ChooseWildMonLevel(const struct WildPokemon * info)
{
    u8 lo, hi, res;
	
    if (info->maxLevel >= info->minLevel)
    {
        lo = info->minLevel;
        hi = info->maxLevel;
    }
    else
    {
        lo = info->maxLevel;
        hi = info->minLevel;
    }
    res = Random() % (hi - lo + 1);
	
	if (IsMonValidSpecies(&gPlayerParty[0]))
	{
		switch (GetMonAbility(&gPlayerParty[0]))
		{
			case ABILITY_HUSTLE:
			case ABILITY_VITAL_SPIRIT:
			case ABILITY_PRESSURE:
				if (RandomPercent(50))
					return hi;
				else if (res)
					--res;
				break;
		}
	}
	res += lo;
	
	if (FlagGet(FLAG_SYS_BLACK_FLUTE_ACTIVE))
		res += RandomRange(1, 4);
	else if (FlagGet(FLAG_SYS_WHITE_FLUTE_ACTIVE))
		res -= RandomRange(1, 4);
	
	if (res == 0)
		res = 1;
	
    return min(MAX_LEVEL, res);
}

////////////////////////
// ENCOUNTER COOLDOWN //
////////////////////////

static u8 GetMapBaseEncounterCooldown(u8 encounterType)
{
    u16 headerIdx = GetCurrentMapWildMonHeaderId();
	const struct WildPokemonInfo * monInfo;
	
    if (headerIdx == 0xFFFF)
        return 0xFF;
	
    if (encounterType == TILE_ENCOUNTER_LAND)
		monInfo = GetWildPokemonInfoByHeaderType(headerIdx, WILD_HEADER_LAND);
	else if (encounterType == TILE_ENCOUNTER_WATER)
		monInfo = GetWildPokemonInfoByHeaderType(headerIdx, WILD_HEADER_WATER);
	else
		return 0xFF;
	
	if (monInfo == NULL)
		return 0xFF;
	if (monInfo->encounterRate >= 80)
		return 0;
	if (monInfo->encounterRate < 10)
		return 8;
	
	return 8 - (monInfo->encounterRate / 10);
}

static bool8 IsLeadMonHoldingEncounterRateModDownItem(void)
{
    if (sWildEncounterData.leadMonHeldItem == ITEM_CLEANSE_TAG)
        return TRUE;
    else
        return FALSE;
}

static u8 GetLeadMonAbilityEncounterRateModType(void)
{
    u8 effectId = 0;
	
	switch (GetMonAbility(&gPlayerParty[0]))
	{
		case ABILITY_STENCH:
		case ABILITY_WHITE_SMOKE:
		case ABILITY_INFILTRATOR:
		case ABILITY_QUICK_FEET:
			effectId = 1;
			break;
		case ABILITY_SAND_VEIL:
			if (GetCurrentWeather() == WEATHER_SANDSTORM)
				effectId = 1;
			break;
		case ABILITY_SNOW_CLOAK:
			if (GetCurrentWeather() == WEATHER_SNOW)
				effectId = 1;
			break;
		case ABILITY_ILLUMINATE:
		case ABILITY_ARENA_TRAP:
		case ABILITY_NO_GUARD:
			effectId = 2;
			break;
	}
    return effectId;
}

static bool8 HandleWildEncounterCooldown(u32 currMetatileAttrs)
{
    u8 encounterType = ExtractMetatileAttribute(currMetatileAttrs, METATILE_ATTRIBUTE_ENCOUNTER_TYPE);
    u32 minSteps;
    u32 encRate;
	
    if (encounterType == TILE_ENCOUNTER_NONE)
        return FALSE;
	
    minSteps = GetMapBaseEncounterCooldown(encounterType);
    if (minSteps == 0xFF)
        return FALSE;
	
    minSteps *= 256;
    encRate = 5 * 256;
	
	if (!IsMonValidSpecies(&gPlayerParty[0]))
	{
		sWildEncounterData.leadMonHeldItem = ITEM_NONE;
		sWildEncounterData.leadMonAbilityEffectId = 0;
	}
	else
	{
		sWildEncounterData.leadMonHeldItem = GetMonData(&gPlayerParty[0], MON_DATA_HELD_ITEM);
		sWildEncounterData.leadMonAbilityEffectId = GetLeadMonAbilityEncounterRateModType();
	}
	
	if (IsLeadMonHoldingEncounterRateModDownItem())
	{
		minSteps += minSteps / 3;
        encRate -= encRate / 3;
	}
	
	switch (sWildEncounterData.leadMonAbilityEffectId)
	{
		case 1:
			minSteps *= 2;
			encRate /= 2;
			break;
		case 2:
			minSteps /= 2;
			encRate *= 2;
			break;
	}
	
    minSteps /= 256;
    encRate /= 256;
	
    if (sWildEncounterData.stepsSinceLastEncounter >= minSteps)
        return TRUE;
	
    sWildEncounterData.stepsSinceLastEncounter++;
	
    return RandomPercent(encRate);
}

static bool8 DoWildEncounterRateTest(u32 encounterRate, bool8 ignoreAbility)
{
    encounterRate *= 16;
	
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE | PLAYER_AVATAR_FLAG_ACRO_BIKE))
        encounterRate = encounterRate * 80 / 100;
	
	encounterRate = ApplyRideEncounterRateMod(encounterRate);
	
    encounterRate += sWildEncounterData.encounterRateBuff * 16 / 200;
	
	if (IsLeadMonHoldingEncounterRateModDownItem())
		encounterRate = encounterRate * 2 / 3;
	
    if (!ignoreAbility)
    {
        switch (sWildEncounterData.leadMonAbilityEffectId)
        {
        case 1:
            encounterRate /= 2;
            break;
        case 2:
            encounterRate *= 2;
            break;
        }
    }
	
    if (encounterRate > 1600)
        encounterRate = 1600;
	
    return DoWildEncounterRateDiceRoll(encounterRate);
}

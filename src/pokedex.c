#include "global.h"
#include "pokedex.h"
#include "pokedex_screen.h"
#include "region_map.h"
#include "overworld.h"
#include "constants/species.h"
#include "constants/pokedex.h"
#include "constants/regions.h"

#define REGION(region, regionName)      \
    [REGION_##region] =                 \
	{                                   \
		.name = _(regionName),          \
		.dexStart = DEX_START_##region, \
		.dexEnd = DEX_END_##region,     \
	}

const struct Region gRegions[REGIONS_COUNT] =
{
	REGION(KANTO, "Kanto"),
	REGION(JOHTO, "Johto"),
	REGION(HOENN, "Hoenn"),
	REGION(SINNOH, "Sinnoh"),
	REGION(UNOVA, "Unova"),
	REGION(KALOS, "Kalos"),
	REGION(ALOLA, "Alola"),
	REGION(GALAR, "Galar"),
	REGION(HISUI, "Hisui"),
	REGION(PALDEA, "Paldea"),
};

bool32 GetSetPokedexFlag(u32 nationalDexNo, u32 caseId)
{
	u32 index, mask;
    bool32 retVal = FALSE;

    nationalDexNo--;
	
    index = nationalDexNo / 8;
    mask = Bit(nationalDexNo % 8);
	
    switch (caseId)
    {
    case FLAG_GET_SEEN:
        if (gSaveBlock2Ptr->pokedex.seen[index] & mask)
			retVal = TRUE;
        break;
    case FLAG_GET_CAUGHT:
        if (gSaveBlock2Ptr->pokedex.owned[index] & mask)
            retVal = TRUE;
        break;
    case FLAG_SET_SEEN:
        gSaveBlock2Ptr->pokedex.seen[index] |= mask;
        break;
    case FLAG_SET_CAUGHT:
        gSaveBlock2Ptr->pokedex.owned[index] |= mask;
        break;
    }
    return retVal;
}

void HandleSetPokedexFlag(u32 nationalNum, u32 caseId, u32 personality)
{
    if (!GetSetPokedexFlag(nationalNum, caseId == FLAG_SET_SEEN ? FLAG_GET_SEEN : FLAG_GET_CAUGHT))
    {
        GetSetPokedexFlag(nationalNum, caseId);
		
        if (nationalNum == NATIONAL_DEX_SPINDA)
            gSaveBlock2Ptr->pokedex.spindaPersonality = personality;
    }
}

u32 GetNationalPokedexCount(u32 caseId)
{
    u32 i, count;

    for (i = 0, count = 0; i < NATIONAL_DEX_END; i++)
    {
		if (GetSetPokedexFlag(i + 1, caseId))
			++count;
    }
    return count;
}

u32 GetRegionPokedexCount(u32 region, u32 caseId)
{
	u32 i, count = 0;
	
	for (i = gRegions[region].dexStart; i < gRegions[region].dexEnd + 1; i++)
	{
		if (GetSetPokedexFlag(i, caseId))
			count++;
	}
	return count;
}

u32 GetCurrentRegionPokedexCount(u32 caseId)
{
	return GetRegionPokedexCount(gMapSectionsInfo[GetCurrentRegionMapSectionId()].region, caseId);
}

bool32 HasAllMons(void)
{
    u32 i;

    for (i = 0; i < NATIONAL_DEX_END; i++)
    {
        if (!GetSetPokedexFlag(i + 1, FLAG_GET_CAUGHT))
            return FALSE;
    }
    return TRUE;
}

bool32 HasAllRegionMons(u32 region)
{
	u32 i;
	
	for (i = gRegions[region].dexStart; i < gRegions[region].dexEnd + 1; i++)
	{
		if (!GetSetPokedexFlag(i, FLAG_GET_CAUGHT))
            return FALSE;
	}
	return TRUE;
}

bool32 HasAllCurrentRegionMons(void)
{
	return HasAllRegionMons(gMapSectionsInfo[GetCurrentRegionMapSectionId()].region);
}

u32 GetNumMonsInRegionPokedex(u32 region)
{
	return gRegions[region].dexEnd - gRegions[region].dexStart + 1;
}

u32 GetNumMonsInCurrentRegionPokedex(void)
{
	return GetNumMonsInRegionPokedex(gMapSectionsInfo[GetCurrentRegionMapSectionId()].region);
}

const u8 *GetRegionName(u32 region)
{
	return gRegions[region].name;
}

const u8 *GetCurrentRegionName(void)
{
	return GetRegionName(gMapSectionsInfo[GetCurrentRegionMapSectionId()].region);
}

u32 SpeciesToNationalPokedexNum(u32 species)
{
	return gSpeciesInfo[species].natDexNum;
}

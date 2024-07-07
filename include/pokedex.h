#ifndef GUARD_POKEDEX_H
#define GUARD_POKEDEX_H

struct Region
{
	u8 name[8];
	u16 dexStart;
	u16 dexEnd;
};

enum
{
    DEX_MODE_KANTO,
    DEX_MODE_NATIONAL
};

enum
{
    FLAG_GET_SEEN,
    FLAG_GET_CAUGHT,
    FLAG_SET_SEEN,
    FLAG_SET_CAUGHT
};

u16 GetNationalPokedexCount(u8 caseId);
u16 GetRegionPokedexCount(u8 region, u8 caseId);
u16 GetCurrentRegionPokedexCount(u8 caseId);
bool16 HasAllMons(void);
bool16 HasAllRegionMons(u8 region);
bool16 HasAllCurrentRegionMons(void);
u16 GetNumMonsInRegionPokedex(u8 region);
u16 GetNumMonsInCurrentRegionPokedex(void);
const u8 *GetRegionName(u8 region);
const u8 *GetCurrentRegionName(void);
bool8 GetSetPokedexFlag(u16 nationalNum, u8 caseId);
void HandleSetPokedexFlag(u16 nationalNum, u8 caseId, u32 personality);
u16 SpeciesToNationalPokedexNum(u16 species);
u8 DexScreen_RegisterMonToPokedex(u16 species);

extern const struct Region gRegions[];

#endif // GUARD_POKEDEX_H

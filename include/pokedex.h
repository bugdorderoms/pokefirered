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

u32 GetNationalPokedexCount(u32 caseId);
u32 GetRegionPokedexCount(u32 region, u32 caseId);
u32 GetCurrentRegionPokedexCount(u32 caseId);
bool32 HasAllMons(void);
bool32 HasAllRegionMons(u32 region);
bool32 HasAllCurrentRegionMons(void);
u32 GetNumMonsInRegionPokedex(u32 region);
u32 GetNumMonsInCurrentRegionPokedex(void);
const u8 *GetRegionName(u32 region);
const u8 *GetCurrentRegionName(void);
bool32 GetSetPokedexFlag(u32 nationalNum, u32 caseId);
void HandleSetPokedexFlag(u32 nationalNum, u32 caseId, u32 personality);
u32 SpeciesToNationalPokedexNum(u32 species);
u32 DexScreen_RegisterMonToPokedex(u32 species);

extern const struct Region gRegions[];

#endif // GUARD_POKEDEX_H

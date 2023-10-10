#ifndef GUARD_POKEDEX_H
#define GUARD_POKEDEX_H

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

struct PokedexEntry
{
    /*0x00*/ u8 categoryName[13];
    /*0x0C*/ u16 height; //in decimeters
    /*0x0E*/ u16 weight; //in hectograms
    /*0x10*/ const u8 *description;
};  /*size = 0x14*/

u16 GetPokedexHeightWeight(u16 dexNum, u8 data);
u16 GetNationalPokedexCount(u8 caseId);
u16 GetKantoPokedexCount(u8 caseId);
bool8 GetSetPokedexFlag(u16 nationalNum, u8 caseId);
void HandleSetPokedexFlag(u16 nationalNum, u8 caseId, u32 personality);
bool16 HasAllKantoMons(void);
bool16 HasAllMons(void);
u16 SpeciesToNationalPokedexNum(u16 species);
u16 NationalPokedexNumToSpecies(u16 nationalNum);

u8 DexScreen_RegisterMonToPokedex(u16 species);

#endif // GUARD_POKEDEX_H

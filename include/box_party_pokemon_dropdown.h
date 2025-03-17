#ifndef GUARD_BOX_PARTY_POKEMON_DROPDOWN_H
#define GUARD_BOX_PARTY_POKEMON_DROPDOWN_H

enum
{
    BPPD_MOVE_INNER_LEFT = 0,
    BPPD_MOVE_OUTER_LEFT,
    BPPD_MOVE_INNER_TOP,
    BPPD_MOVE_OUTER_TOP,
    BPPD_MOVE_INNER_X,
    BPPD_MOVE_INNER_Y
};

void AllocBoxPartyPokemonDropdowns(u32 num);
void FreeBoxPartyPokemonDropdowns(void);
void SetBoxPartyPokemonDropdownMap2(u32 idx, u32 bgId, const void * src, u16 width, u16 height);
void SetBoxPartyPokemonDropdownMap2Pos(u32 idx, u16 x, u16 y);
void SetBoxPartyPokemonDropdownMap2Rect(u32 idx, u16 x, u16 y, u16 width, u16 height);
void AdjustBoxPartyPokemonDropdownPos(u32 a0, u32 a1, s8 a2);
void CopyBoxPartyPokemonDropdownToBgTilemapBuffer(u32 idx);

#endif //GUARD_BOX_PARTY_POKEMON_DROPDOWN_H

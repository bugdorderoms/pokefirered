#ifndef GUARD_POKEMON_JUMP_H
#define GUARD_POKEMON_JUMP_H

#include "main.h"

bool32 IsSpeciesAllowedInPokemonJump(u32 species);
void StartPokemonJump(u32 partyId, MainCallback exitCallback);
void ResetPokeJumpResults(void);

#endif // GUARD_POKEMON_JUMP_H

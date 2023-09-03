#ifndef GUARD_SCRIPT_POKEMON_UTIL_H
#define GUARD_SCRIPT_POKEMON_UTIL_H

bool8 ScriptGiveMon(u16 species, u8 level, u16 item, u8 *ivs, u16 pokeBall, u8 shinyType, bool8 hiddenAbility, u8 nature, u8 gender);
bool8 ScriptGiveEgg(u16 species, u8 *ivs, u8 shinyType, bool8 hiddenAbility, u8 nature);
void ScriptSetMonMoveSlot(u8 partyIdx, u16 move, u8 slot);
void HealPlayerParty(void);
void ReducePlayerPartyToThree(void);
void CreateScriptedWildMon(u16 species, u8 level, u16 item, u16 species2, u8 level2, u16 item2);

#endif //GUARD_SCRIPT_POKEMON_UTIL_H

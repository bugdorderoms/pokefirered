#ifndef GUARD_SCRIPT_POKEMON_UTIL_H
#define GUARD_SCRIPT_POKEMON_UTIL_H

u32 ScriptGiveMon(u32 species, u32 level, u32 item, u8 *ivs, u32 pokeBall, u32 shinyType, bool32 hiddenAbility, u32 nature, u32 gender, u16 *moves);
u32 ScriptGiveEgg(u32 species, u8 *ivs, u32 shinyType, bool32 hiddenAbility, u32 nature, u16 *moves);
u32 GiveScriptCreatedMonToPlayer(struct Pokemon *mon, bool32 setPokedexFlags);
void ScriptSetMonMoveSlot(u32 partyIdx, u32 move, u32 slot);
void HealPlayerParty(void);
void ReducePlayerPartyToThree(void);
void CreateScriptedWildMon(u32 species, u32 level, u32 item, u32 species2, u32 level2, u32 item2);

#endif //GUARD_SCRIPT_POKEMON_UTIL_H

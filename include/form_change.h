#ifndef GUARD_FORM_CHANGE_H
#define GUARD_FORM_CHANGE_H

// overworld form change types
#define OVERWORLD_FORM_CHANGE_END_BATTLE  (1 << 0) // not only on overworld, but handled here (revert mon form to normal)
#define OVERWORLD_FORM_CHANGE_GENDER      (1 << 1) // form change based on gender (Pyroar, Meowstic, etc)
#define OVERWORLD_FORM_CHANGE_PERSONALITY (1 << 2) // form change based on personality (Vivillon, Unown, etc)
#define OVERWORLD_FORM_CHANGE_SEASON      (1 << 3) // form change based on season (Deerling, Sawsbuck, etc)
#define OVERWORLD_FORM_CHANGE_HOLD_ITEM   (1 << 4) // form change based on item hold (Arceus, Genesect, etc)
#define OVERWORLD_FORM_CHANGE_TERRAIN     (1 << 5) // form change based on terrain (Burmy)

#define OVERWORLD_FORM_CHANGE_WILD_ENCOUNTER (OVERWORLD_FORM_CHANGE_GENDER | OVERWORLD_FORM_CHANGE_PERSONALITY | OVERWORLD_FORM_CHANGE_SEASON)

// overworld form change
u16 DoOverworldFormChange(struct Pokemon *mon, u32 formChangeType);
u16 DoWildEncounterFormChange(struct Pokemon *mon);
void DoPlayerPartyEndBattleFormChange(void);

// battle form change types
enum
{
	BATTLE_FORM_CHANGE_SWITCH_OUT = 1,
	BATTLE_FORM_CHANGE_LOW_HP,
	BATTLE_FORM_CHANGE_FAINT,
};

// battle form change
u16 TryDoBattleFormChange(u8 battlerId, u8 formChangeType);
void DoBattleFormChange(u8 battlerId, u16 newSpecies, bool8 reloadTypes, bool8 reloadStats, bool8 reloadAbility);

#endif // GUARD_FORM_CHANGE_H

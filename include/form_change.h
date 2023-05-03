#ifndef GUARD_FORM_CHANGE_H
#define GUARD_FORM_CHANGE_H

struct FormChange
{
	u16 method;
	u16 targetSpecies;
	u16 param;
	u16 param2;
};

u16 GetSpeciesFormChange(u16 formChangeType, u16 species, u32 personality, u16 ability, u16 itemId, u8 battlerId, bool8 knowsMove);

// overworld form change
u16 DoOverworldFormChange(struct Pokemon *mon, u16 formChangeType);
u16 DoWildEncounterFormChange(struct Pokemon *mon);
void DoPlayerPartyEndBattleFormChange(void);
u16 GetMonFormChangeSpecies(struct Pokemon *mon, u16 formChangeType);

// battle form change
u16 TryDoBattleFormChange(u8 battlerId, u16 formChangeType);
void DoBattleFormChange(u8 battlerId, u16 newSpecies, bool8 reloadTypes, bool8 reloadAbility);
void DoSpecialFormChange(u8 battlerId, u8 partyId, u16 formChangeType);

extern const struct FormChange *const gFormChangeTable[];

#endif // GUARD_FORM_CHANGE_H

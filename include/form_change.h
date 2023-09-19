#ifndef GUARD_FORM_CHANGE_H
#define GUARD_FORM_CHANGE_H

struct FormChange
{
	u16 method;
	u16 targetSpecies;
	u16 param;
	u16 param2;
};

enum
{
	FORM_CHANGE_ANIM_MOSAIC,
	FORM_CHANGE_ANIM_GRACIDEA_FLOWERS,
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

// form change animations
void DoFormChangeAnim(u8 animId, u16 species, bool8 isFusion, struct Sprite *icon1, struct Sprite *icon2);
bool8 IsFormChangeAnimFinished(void);

#endif // GUARD_FORM_CHANGE_H

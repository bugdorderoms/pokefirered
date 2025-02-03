#ifndef GUARD_FORM_CHANGE_H
#define GUARD_FORM_CHANGE_H

struct Fusion
{
	u16 masterSpecies;
	u16 otherSpecies;
	u16 item;
	u16 targetSpecies;
	u16 id;
};

enum
{
	FORM_CHANGE_ANIM_MOSAIC,
	FORM_CHANGE_ANIM_GRACIDEA_FLOWERS,
	FORM_CHANGE_ANIM_PURPLE_CLOUD,
	FORM_CHANGE_ANIM_LIGHT_ABSORPTION,
	FORM_CHANGE_ANIM_CLOSING_SCREEN,
	FORM_CHANGE_ANIM_FALLING_FEATHERS,
};

// overworld form change
u16 GetMonFormChangeSpecies(struct Pokemon *mon, u16 species, u16 formChangeType);
u16 DoOverworldFormChange(struct Pokemon *mon, u16 formChangeType);
void DoPlayerPartyEndBattleFormChange(void);
bool8 SpeciesHasFormChangeType(u16 species, u16 formChangeType);
void TrySetMonFormChangeCountdown(struct Pokemon *mon);

// battle form change
u16 GetBattlerFormChangeSpecies(u8 battlerId, u16 species, u16 itemId, u16 formChangeType);
u16 TryDoBattleFormChange(u8 battlerId, u16 formChangeType);
void DoBattleFormChange(u8 battlerId, u16 newSpecies, bool8 reloadTypes, bool8 reloadStats, bool8 reloadAbility);
bool8 DoSpecialFormChange(u8 battlerId, u8 partyId, u16 formChangeType);

extern const u16 gDefaultGeneratorFormChanges[];

extern const struct Fusion gFusionsTable[];

// form change animations
void DoFormChangeAnim(u8 animId, u16 species, bool8 isFusion, struct Sprite *icon1, struct Sprite *icon2);
bool8 IsFormChangeAnimFinished(void);

#endif // GUARD_FORM_CHANGE_H

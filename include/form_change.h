#ifndef GUARD_FORM_CHANGE_H
#define GUARD_FORM_CHANGE_H

#define DEFAULT_GENERATOR_FORMS \
    FORM_CHANGE_GENDER,         \
    FORM_CHANGE_PERSONALITY,    \
    FORM_CHANGE_SEASON,         \
    FORM_CHANGE_NATURE

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
u32 GetMonFormChangeSpecies(struct Pokemon *mon, u32 species, u32 formChangeType);
u32 DoOverworldFormChange(struct Pokemon *mon, u32 formChangeType);
void DoPlayerPartyEndBattleFormChange(void);
bool32 SpeciesHasFormChangeType(u32 species, u32 formChangeType);
void TrySetMonFormChangeCountdown(struct Pokemon *mon);

// battle form change
u32 GetBattlerFormChangeSpecies(u32 battlerId, u32 species, u32 itemId, u32 formChangeType);
u32 TryDoBattleFormChange(u32 battlerId, u32 formChangeType);
void DoBattleFormChange(u32 battlerId, u32 newSpecies, bool32 reloadTypes, bool32 reloadStats, bool32 reloadAbility);
bool32 DoSpecialFormChange(u32 battlerId, u32 partyId, u32 formChangeType);

extern const struct Fusion gFusionsTable[];

// form change animations
void DoFormChangeAnim(u32 animId, u32 species, bool32 isFusion, struct Sprite *icon1, struct Sprite *icon2);
bool32 IsFormChangeAnimFinished(void);

#endif // GUARD_FORM_CHANGE_H

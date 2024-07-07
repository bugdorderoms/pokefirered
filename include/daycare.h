#ifndef GUARD_DAYCARE_H
#define GUARD_DAYCARE_H

#include "global.h"

void StoreSelectedPokemonInDaycare(void);
u16 TakePokemonFromDaycare(void);
u8 GetNumLevelsGainedFromDaycare(void);
void ShowDaycareLevelMenu(void);
void GetDaycareCost(void);
u8 GetDaycareState(void);
void ChooseSendDaycareMon(void);
void CreateEgg(struct Pokemon *mon, struct PokemonGenerator generator);
void GiveEggFromDaycare(void);
void RejectEggFromDayCare(void);
void SetDaycareCompatibilityString(void);
bool8 ShouldEggHatch(void);
void EggHatch(void);
bool8 NameHasGenderSymbol(const u8 *name, u8 genderRatio);
void GetDaycareMonNicknames(void);
u8 GetEggMoves(struct Pokemon *pokemon, u16 *eggMoves);

#endif // GUARD_DAYCARE_H

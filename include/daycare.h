#ifndef GUARD_DAYCARE_H
#define GUARD_DAYCARE_H

#include "global.h"

void StoreSelectedPokemonInDaycare(void);
u32 TakePokemonFromDaycare(void);
u32 GetNumLevelsGainedFromDaycare(void);
void ShowDaycareLevelMenu(void);
void GetDaycareCost(void);
u32 GetDaycareState(void);
void ChooseSendDaycareMon(void);
void CreateEgg(struct Pokemon *mon, struct PokemonGenerator generator);
void GiveEggFromDaycare(void);
void RejectEggFromDayCare(void);
void SetDaycareCompatibilityString(void);
bool32 ShouldEggHatch(void);
void EggHatch(void);
bool32 NameHasGenderSymbol(const u8 *name, u32 genderRatio);
void GetDaycareMonNicknames(void);
u32 GetEggMoves(struct Pokemon *pokemon, u16 *eggMoves);

#endif // GUARD_DAYCARE_H

#ifndef GUARD_TRAINER_POKEMON_SPRITES_H
#define GUARD_TRAINER_POKEMON_SPRITES_H

#include "global.h"

u32 CreateTrainerCardTrainerPicSprite(u32 species, bool32 isFrontPic, u16 destX, u16 destY, u32 paletteSlot, u32 windowId);
void ResetAllPicSprites(void);
u32 CreateTrainerPicSprite(u32 species, bool32 isFrontPic, s16 x, s16 y, u32 paletteSlot, u32 paletteTag);
u32 FreeAndDestroyMonPicSprite(u32);
u32 FreeAndDestroyTrainerPicSprite(u32);
u32 PlayerGenderToFrontTrainerPicId_Debug(u32 gender, bool32 getClass);
u32 LoadMonPicInWindow(u32 species, bool32 isShiny, u32 personality, bool32 isFrontPic, u32 paletteSlot, u32 windowId);
u32 CreateMonPicSprite(u32 species, bool32 isShiny, u32 personality, bool32 isFrontPic, s16 x, s16 y, u32 paletteSlot, u32 paletteTag);

#endif // GUARD_TRAINER_POKEMON_SPRITES_H

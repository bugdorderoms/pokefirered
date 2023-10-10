#ifndef GUARD_POKEMON_ICON_H
#define GUARD_POKEMON_ICON_H

#include "global.h"

#define POKE_ICON_BASE_PAL_TAG 56000

extern const u8 *const gMonIconTable[];
extern const struct SpritePalette gMonIconPaletteTable[];
extern const u16 gMonIconPalettes[][16];
extern const u8 gMonIconPaletteIndices[];

const u8 *GetMonIconPtr(u16 speciesId);
const u8 *GetMonIconTiles(u16 iconSpecies);
const u16 *GetValidMonIconPalettePtr(u16 speciesId);
void LoadMonIconPalettes(void);
void FreeMonIconPalettes(void);
void DestroyMonIcon(struct Sprite *);
void SpriteCB_MonIcon(struct Sprite *);
u8 CreateMonIcon(u16 species, SpriteCallback callback, s16 x, s16 y, u8 subpriority);
u8 UpdateMonIconFrame(struct Sprite * sprite);
void LoadMonIconPalette(u16 iconId);
void FreeMonIconPalette(u16 iconId);
void SetPartyHPBarSprite(struct Sprite * sprite, u8 animNum);
u8 GetMonIconPaletteIndexFromSpecies(u16 species);
void LoadMonIconGraphicsInWindow(u8 windowId, u16 species);
void LoadMonIconPaletteIdxAt(u8 palIdx, u16 offset);
void LoadMonIconPalettesAt(u16 offset);
u8 GetValidMonIconPalIndex(u16 species);
void SafeLoadMonIconPalette(u16 species);
void SafeFreeMonIconPalette(u16 species);

#endif // GUARD_POKEMON_ICON_H

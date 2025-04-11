#ifndef GUARD_POKEMON_ICON_H
#define GUARD_POKEMON_ICON_H

#include "global.h"

#define POKE_ICON_BASE_PAL_TAG 56000

extern const struct SpritePalette gMonIconPaletteTable[];
extern const u16 gMonIconPalettes[][16];

u32 CreateMonIcon(u32 species, SpriteCallback callback, s16 x, s16 y, u32 subpriority);
void DestroyMonIcon(struct Sprite *);
void SpriteCB_MonIcon(struct Sprite *);
u32 UpdateMonIconFrame(struct Sprite * sprite);
void SetPartyHPBarSprite(struct Sprite * sprite, u32 animNum);
const u8 *GetMonIconPtr(u32 species);
void LoadMonIconPalettes(void);
void FreeMonIconPalettes(void);
void LoadMonIconPalette(u32 iconId);
void FreeMonIconPalette(u32 iconId);
void LoadMonIconPaletteIdxAt(u32 palIdx, u32 offset);
void LoadMonIconPalettesAt(u32 offset);
u32 GetMonIconPalIndex(u32 species);
const u16 *GetMonIconPalettePtr(u32 speciesId);
void LoadMonIconGraphicsInWindow(u32 windowId, u32 species);

#endif // GUARD_POKEMON_ICON_H

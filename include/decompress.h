#ifndef GUARD_DECOMPRESS_H
#define GUARD_DECOMPRESS_H

#include "global.h"
#include "sprite.h"

extern u8 gDecompressionBuffer[0x4000];

u32 LoadCompressedSpriteSheet(const struct CompressedSpriteSheet *src);
bool32 LoadCompressedSpriteSheetUsingHeap(const struct CompressedSpriteSheet* src);

void LoadCompressedSpritePalette(const struct CompressedSpritePalette *src);
bool32 LoadCompressedSpritePaletteUsingHeap(const struct CompressedSpritePalette *src);

void LoadSpecialPokePic(u32 species, u32 personality, bool32 isFrontPic, void *dest);

void LoadMonPalette(struct Pokemon *mon);
void LoadMonPaletteFromSpecies(u32 species, bool32 isShiny, u32 paletteTag);

#endif // GUARD_DECOMPRESS_H

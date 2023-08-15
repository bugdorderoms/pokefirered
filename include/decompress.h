#ifndef GUARD_DECOMPRESS_H
#define GUARD_DECOMPRESS_H

#include "global.h"
#include "sprite.h"

extern u8 gDecompressionBuffer[0x4000];

u16 LoadCompressedSpriteSheet(const struct CompressedSpriteSheet *src);
bool8 LoadCompressedSpriteSheetUsingHeap(const struct CompressedSpriteSheet* src);

void LoadCompressedSpritePalette(const struct CompressedSpritePalette *src);
bool8 LoadCompressedSpritePaletteUsingHeap(const struct CompressedSpritePalette *src);

void DecompressPicFromTable(const struct CompressedSpriteSheet *src, void* buffer, s32 species);

void HandleLoadSpecialPokePic(const struct CompressedSpriteSheet *src, void *dest, s32 species, u32 personality);
void LoadSpecialPokePic(const struct CompressedSpriteSheet *src, void *dest, s32 species, u32 personality, bool8 isFrontPic);

#endif // GUARD_DECOMPRESS_H

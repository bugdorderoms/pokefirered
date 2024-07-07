#include "global.h"
#include "gflib.h"
#include "decompress.h"
#include "pokemon.h"

u16 LoadCompressedSpriteSheet(const struct CompressedSpriteSheet *src)
{
    struct SpriteSheet dest;
    LZDecompressWram(src->data, gDecompressionBuffer);
    dest.data = gDecompressionBuffer;
    dest.size = src->size;
    dest.tag = src->tag;
    return LoadSpriteSheet(&dest);
}

bool8 LoadCompressedSpriteSheetUsingHeap(const struct CompressedSpriteSheet* src)
{
    struct SpriteSheet dest;
    void* buffer;

    buffer = AllocZeroed(*((u32*)src->data) >> 8);
    if (!buffer)
        return TRUE;
    LZDecompressWram(src->data, buffer);
    dest.data = buffer;
    dest.size = src->size;
    dest.tag = src->tag;
    LoadSpriteSheet(&dest);
    Free(buffer);
    return FALSE;
}

void LoadCompressedSpritePalette(const struct CompressedSpritePalette *src)
{
    struct SpritePalette dest;

    LZDecompressWram(src->data, gDecompressionBuffer);
    dest.data = (void*) gDecompressionBuffer;
    dest.tag = src->tag;
    LoadSpritePalette(&dest);
}

bool8 LoadCompressedSpritePaletteUsingHeap(const struct CompressedSpritePalette *src)
{
    struct SpritePalette dest;
    void* buffer;

    buffer = AllocZeroed(*((u32*)src->data) >> 8);
    if (!buffer)
        return TRUE;
    LZDecompressWram(src->data, buffer);
    dest.data = buffer;
    dest.tag = src->tag;
    LoadSpritePalette(&dest);
    Free(buffer);
    return FALSE;
}

void LoadMonPalette(struct Pokemon *mon)
{
	LoadMonPaletteFromSpecies(GetMonData(mon, MON_DATA_SPECIES2), GetMonData(mon, MON_DATA_IS_SHINY));
}

void LoadMonPaletteFromSpecies(u16 species, bool8 isShiny)
{
	struct SpritePalette dest;

    LZDecompressWram(GetMonSpritePalFromSpecies(species, isShiny), gDecompressionBuffer);
    dest.data = (void*)gDecompressionBuffer;
    dest.tag = species;
    LoadSpritePalette(&dest);
}

void LoadSpecialPokePic(u16 species, u32 personality, bool8 isFrontPic, void *dest)
{
	species = SanitizeSpeciesId(species);
	LZDecompressWram(isFrontPic ? gSpeciesInfo[species].frontPic : gSpeciesInfo[species].backPic, dest);
    DrawSpindaSpots(species, personality, dest, isFrontPic);
}

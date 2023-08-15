#include "global.h"
#include "gflib.h"
#include "decompress.h"
#include "pokemon.h"

extern const struct CompressedSpriteSheet gMonFrontPicTable[];
extern const struct CompressedSpriteSheet gMonBackPicTable[];

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

void DecompressPicFromTable(const struct CompressedSpriteSheet *src, void* buffer, s32 species)
{
	LZDecompressWram(species > NUM_SPECIES ? gMonFrontPicTable[SPECIES_NONE].data : src->data, buffer);
}

void HandleLoadSpecialPokePic(const struct CompressedSpriteSheet *src, void *dest, s32 species, u32 personality)
{
    LoadSpecialPokePic(src, dest, species, personality, (src == &gMonFrontPicTable[species]));
}

void LoadSpecialPokePic(const struct CompressedSpriteSheet *src, void *dest, s32 species, u32 personality, bool8 isFrontPic)
{
	LZDecompressWram(species > NUM_SPECIES ? gMonFrontPicTable[SPECIES_NONE].data : src->data, dest);
    DrawSpindaSpots(species, personality, dest, isFrontPic);
}

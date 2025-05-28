#include "global.h"
#include "gflib.h"
#include "data.h"
#include "decompress.h"

extern const union AnimCmd *const gSpriteAnimTable_82349BC[];

// Static type declarations

struct PicData
{
    u8 *frames;
    struct SpriteFrameImage *images;
    u16 paletteTag;
    u8 spriteId;
    u8 active;
};

// Static RAM declarations
#define PICS_COUNT 8

static EWRAM_DATA struct SpriteTemplate sCreatingSpriteTemplate = {};
static EWRAM_DATA struct PicData sSpritePics[PICS_COUNT] = {};

// Static ROM declarations

// .rodata

static const struct PicData gUnknown_8453178 = {};

static const struct OamData gUnknown_8453184 =
{
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64)
};

// .text

void DummyPicSpriteCallback(struct Sprite *sprite)
{

}

void ResetAllPicSprites(void)
{
    u32 i;

    for (i = 0; i < PICS_COUNT; i ++)
        sSpritePics[i] = gUnknown_8453178;
}

static void DecompressPic(u32 species, u32 personality, bool32 isFrontPic, u8 *dest, bool32 isTrainer)
{
    if (!isTrainer)
        LoadSpecialPokePic(species, personality, isFrontPic, dest);
    else
        LZDecompressWram(isFrontPic ? gTrainerFrontPicTable[species].pic.data : gTrainerBackPicTable[species].pic.data, dest);
}

static void AssignSpriteAnimsTable(u32 species, bool32 isTrainer)
{
    if (!isTrainer)
        sCreatingSpriteTemplate.anims = gSpriteAnimTable_82349BC;
    else
        sCreatingSpriteTemplate.anims = gTrainerFrontPicTable[species].anims;
}

static void LoadPicPaletteByTagOrSlot(u32 species, bool32 isShiny, u32 paletteSlot, u32 paletteTag, bool32 isTrainer)
{
    if (!isTrainer)
    {
        if (paletteTag == 0xFFFF)
        {
            sCreatingSpriteTemplate.paletteTag = 0xFFFF;
            LoadCompressedPalette(GetMonSpritePalFromSpecies(species, isShiny), 0x100 + paletteSlot * 0x10, 0x20);
        }
        else
        {
            sCreatingSpriteTemplate.paletteTag = paletteTag;
            LoadMonPaletteFromSpecies(species, isShiny);
        }
    }
    else
    {
        if (paletteTag == 0xFFFF)
        {
            sCreatingSpriteTemplate.paletteTag = 0xFFFF;
            LoadCompressedPalette(gTrainerFrontPicTable[species].palette.data, 0x100 + paletteSlot * 0x10, 0x20);
        }
        else
        {
            sCreatingSpriteTemplate.paletteTag = paletteTag;
            LoadCompressedSpritePalette(&gTrainerFrontPicTable[species].palette);
        }
    }
}

static u32 CreatePicSprite(u32 species, bool32 isShiny, u32 personality, bool32 isFrontPic, s16 x, s16 y, u32 paletteSlot, u32 paletteTag, bool32 isTrainer)
{
    u32 i, j;
    u8 *framePics;
    struct SpriteFrameImage *images;
    u32 spriteId;

    for (i = 0; i < PICS_COUNT; i ++)
    {
        if (!sSpritePics[i].active)
            break;
    }
    if (i == PICS_COUNT)
        return 0xFFFF;

    framePics = Alloc(4 * 0x800);
    if (!framePics)
        return 0xFFFF;

    images = Alloc(4 * sizeof(struct SpriteFrameImage));
    if (!images)
    {
        Free(framePics);
        return 0xFFFF;
    }
    DecompressPic(species, personality, isFrontPic, framePics, isTrainer);

    for (j = 0; j < 4; j ++)
    {
        images[j].data = framePics + 0x800 * j;
        images[j].size = 0x800;
    }
    sCreatingSpriteTemplate.tileTag = 0xFFFF;
    sCreatingSpriteTemplate.oam = &gUnknown_8453184;
    AssignSpriteAnimsTable(species, isTrainer);
    sCreatingSpriteTemplate.images = images;
    sCreatingSpriteTemplate.affineAnims = gDummySpriteAffineAnimTable;
    sCreatingSpriteTemplate.callback = DummyPicSpriteCallback;
    LoadPicPaletteByTagOrSlot(species, isShiny, paletteSlot, paletteTag, isTrainer);

    spriteId = CreateSprite(&sCreatingSpriteTemplate, x, y, 0);
    if (paletteTag == 0xFFFF)
        gSprites[spriteId].oam.paletteNum = paletteSlot;

    sSpritePics[i].frames = framePics;
    sSpritePics[i].images = images;
    sSpritePics[i].paletteTag = paletteTag;
    sSpritePics[i].spriteId = spriteId;
    sSpritePics[i].active = TRUE;
    
    return spriteId;
}

static u32 FreeAndDestroyPicSpriteInternal(u32 spriteId)
{
    u32 i;
    u8 *framePics;
    struct SpriteFrameImage *images;

    for (i = 0; i < PICS_COUNT; i ++)
    {
        if (sSpritePics[i].spriteId == spriteId)
            break;
    }
    if (i == PICS_COUNT)
        return 0xFFFF;

    framePics = sSpritePics[i].frames;
    images = sSpritePics[i].images;
    if (sSpritePics[i].paletteTag != 0xFFFF)
        FreeSpritePaletteByTag(GetSpritePaletteTagByPaletteNum(gSprites[spriteId].oam.paletteNum));

    DestroySprite(&gSprites[spriteId]);
    Free(framePics);
    Free(images);
    sSpritePics[i] = gUnknown_8453178;
    return 0;
}

static void LoadPicPaletteBySlot(u32 species, bool32 isShiny, u32 paletteSlot, bool32 isTrainer)
{
    if (!isTrainer)
        LoadCompressedPalette(GetMonSpritePalFromSpecies(species, isShiny), paletteSlot * 0x10, 0x20);
    else
        LoadCompressedPalette(gTrainerFrontPicTable[species].palette.data, paletteSlot * 0x10, 0x20);
}

static u32 CreateTrainerCardSprite(u32 species, bool32 isShiny, u32 personality, bool32 isFrontPic, u16 destX, u16 destY, u32 paletteSlot, u32 windowId, bool32 isTrainer)
{
    u8 *framePics = Alloc(4 * 0x800);

    if (framePics)
    {
        DecompressPic(species, personality, isFrontPic, framePics, isTrainer);
        BlitBitmapRectToWindow(windowId, framePics, 0, 0, 0x40, 0x40, destX, destY, 0x40, 0x40);
        LoadPicPaletteBySlot(species, isShiny, paletteSlot, isTrainer);
        Free(framePics);
        return 0;
    }
    return 0xFFFF;
}

u32 CreateMonPicSprite(u32 species, bool32 isShiny, u32 personality, bool32 isFrontPic, s16 x, s16 y, u32 paletteSlot, u32 paletteTag)
{
    return CreatePicSprite(species, isShiny, personality, isFrontPic, x, y, paletteSlot, paletteTag, FALSE);
}

u32 FreeAndDestroyMonPicSprite(u32 spriteId)
{
    return FreeAndDestroyPicSpriteInternal(spriteId);
}

u32 LoadMonPicInWindow(u32 species, bool32 isShiny, u32 personality, bool32 isFrontPic, u32 paletteSlot, u32 windowId)
{
    return CreateTrainerCardSprite(species, isShiny, personality, isFrontPic, 0, 0, paletteSlot, windowId, FALSE);
}

u32 CreateTrainerPicSprite(u32 species, bool32 isFrontPic, s16 x, s16 y, u32 paletteSlot, u32 paletteTag)
{
    return CreatePicSprite(species, FALSE, 0, isFrontPic, x, y, paletteSlot, paletteTag, TRUE);
}

u32 FreeAndDestroyTrainerPicSprite(u32 spriteId)
{
    return FreeAndDestroyPicSpriteInternal(spriteId);
}

u32 CreateTrainerCardTrainerPicSprite(u32 species, bool32 isFrontPic, u16 destX, u16 destY, u32 paletteSlot, u32 windowId)
{
    return CreateTrainerCardSprite(species, FALSE, 0, isFrontPic, destX, destY, paletteSlot, windowId, TRUE);
}

u32 PlayerGenderToFrontTrainerPicId_Debug(u32 gender, bool32 getClass)
{
    if (getClass)
    {
        switch (gender)
        {
        default:
            return FacilityClassToPicIndex(FACILITY_CLASS_LEAF);
        case MALE:
            return FacilityClassToPicIndex(FACILITY_CLASS_RED);
        }
    }
    return gender;
}

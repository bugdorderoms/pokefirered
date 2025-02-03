#ifndef GUARD_DATA_H
#define GUARD_DATA_H

#include "global.h"

struct MonCoords
{
    // This would use a bitfield, but some function
    // uses it as a u8 and casting won't match.
    u8 size; // u8 width:4, height:4;
    u8 y_offset;
};

struct TrainerPic
{
	const struct CompressedSpriteSheet pic;
	const struct CompressedSpritePalette palette;
	struct MonCoords coords;
	const union AnimCmd *const *anims;
};

#define MON_COORDS_SIZE(width, height)(DIV_ROUND_UP(width, 8) << 4 | DIV_ROUND_UP(height, 8))
#define GET_MON_COORDS_WIDTH(size)((size >> 4) * 8)
#define GET_MON_COORDS_HEIGHT(size)((size & 0xF) * 8)

extern const u16 gTrainerEncounterMusicIdsTable[];
extern const struct TrainerPic gTrainerFrontPicTable[];
extern const struct TrainerPic gTrainerBackPicTable[];

extern const struct CompressedSpriteSheet gSpriteSheet_EnemyShadowsSized;
extern const struct SpriteTemplate gSpriteTemplate_EnemyShadow;

extern const struct SpritePalette gSpritePalettes_HealthBoxHealthBar[2];

extern const union AnimCmd *const gSpriteAnimTable_82349BC[];
extern const union AffineAnimCmd *const gSpriteAffineAnimTable_BattlerPlayer[];
extern const union AffineAnimCmd *const gSpriteAffineAnimTable_BattlerOpponent[];
extern const struct SpriteFrameImage gSpriteImages_BattlerPlayerLeft[];
extern const struct SpriteFrameImage gSpriteImages_BattlerOpponentLeft[];
extern const struct SpriteFrameImage gSpriteImages_BattlerPlayerRight[];
extern const struct SpriteFrameImage gSpriteImages_BattlerOpponentRight[];
extern const struct SpriteFrameImage gTrainerBackPicTable_Red[];
extern const struct SpriteFrameImage gTrainerBackPicTable_Leaf[];
extern const struct SpriteFrameImage gTrainerBackPicTable_Pokedude[];
extern const struct SpriteFrameImage gTrainerBackPicTable_OldMan[];
extern const struct SpriteFrameImage gTrainerBackPicTable_RSBrendan[];
extern const struct SpriteFrameImage gTrainerBackPicTable_RSMay[];

#endif // GUARD_DATA_H

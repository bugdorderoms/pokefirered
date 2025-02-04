static const u32 sKabutoPuzzle_Gfx[] = INCBIN_U32("graphics/sliding_puzzle/puzzles/kabuto.4bpp.lz");
static const u32 sOmanytePuzzle_Gfx[] = INCBIN_U32("graphics/sliding_puzzle/puzzles/omanyte.4bpp.lz");
static const u32 sAerodactylPuzzle_Gfx[] = INCBIN_U32("graphics/sliding_puzzle/puzzles/aerodactyl.4bpp.lz");
static const u32 sHoOhPuzzle_Gfx[] = INCBIN_U32("graphics/sliding_puzzle/puzzles/ho_oh.4bpp.lz");

#define PUZZLE(name) {s##name##Puzzle_Gfx, 0x400 * 16, GFXTAG_TILES}

static const struct CompressedSpriteSheet sSpriteSheet_Tiles[PUZZLE_COUNT] =
{
    [PUZZLE_KABUTO]     = PUZZLE(Kabuto),
    [PUZZLE_OMANYTE]    = PUZZLE(Omanyte),
    [PUZZLE_AERODACTYL] = PUZZLE(Aerodactyl),
    [PUZZLE_HO_OH]      = PUZZLE(HoOh),
    [PUZZLE_SOLVED]     = {},
};

#undef PUZZLE

#define __ 0

static const u8 sPuzzleLayouts[PUZZLE_COUNT][NUM_ROWS][NUM_COLS] =
{
    [PUZZLE_KABUTO] =
    {
        { 6,  1, __,  3,  4, __},
        {__,  5, __,  7,  8, 11},
        {__, __, 10, __, 12, __},
        { 2, 13, 14, 15, 16,  9},
    },
    [PUZZLE_OMANYTE] =
    {
        { 9, __,  2,  3,  4, 11},
        {__,  5,  6, 12, __, 14},
        { 8, __, __, __, __, 13},
        { 1,  7, __, 15, 16, 10},
    },
    [PUZZLE_AERODACTYL] =
    {
        {14, __, __,  3,  4, __},
        {__, __, 10,  7,  8, 16},
        { 2,  9, __, 11, 12,  5},
        { 1, 13, __, 15, __,  6},
    },
    [PUZZLE_HO_OH] =
    {
        {15, __, 14,  3,  4,  1},
        {13, __,  6,  7, __, 10},
        { 2, __, __, __, 12,  9},
        { 8, __,  5, __, 16, 11},
    },
    [PUZZLE_SOLVED] =
    {
        {__,  1,  2,  3,  4, __},
        {__,  5,  6,  7,  8, __},
        {__,  9, 10, 11, 12, __},
        {__, 13, 14, 15, 16, __},
    },
};

#undef __

static const u8 sTileOrientations[PUZZLE_COUNT][NUM_ROWS][NUM_COLS] =
{
    [PUZZLE_KABUTO] =
    {
        {ORIENTATION_90 , IMMOVABLE_TILE, ORIENTATION_0 , IMMOVABLE_TILE, IMMOVABLE_TILE, ORIENTATION_0  },
        {ORIENTATION_0  , IMMOVABLE_TILE, ORIENTATION_0 , IMMOVABLE_TILE, IMMOVABLE_TILE, ORIENTATION_180},
        {ORIENTATION_0  , ORIENTATION_0 , IMMOVABLE_TILE, ORIENTATION_0 , IMMOVABLE_TILE, ORIENTATION_0  },
        {ORIENTATION_270, IMMOVABLE_TILE, IMMOVABLE_TILE, IMMOVABLE_TILE, IMMOVABLE_TILE, ORIENTATION_90 },
    },
    [PUZZLE_OMANYTE] =
    {
        {ORIENTATION_0  , ORIENTATION_0  , IMMOVABLE_TILE, IMMOVABLE_TILE, IMMOVABLE_TILE, ORIENTATION_90 },
        {ORIENTATION_0  , IMMOVABLE_TILE , IMMOVABLE_TILE, ORIENTATION_90, ORIENTATION_0 , ORIENTATION_180},
        {ORIENTATION_0  , ORIENTATION_0  , ORIENTATION_0 , ORIENTATION_0 , ORIENTATION_0 , ORIENTATION_270},
        {ORIENTATION_270, ORIENTATION_180, ORIENTATION_0 , IMMOVABLE_TILE, IMMOVABLE_TILE, ORIENTATION_0  },
    },
    [PUZZLE_AERODACTYL] =
    {
        {ORIENTATION_180, ORIENTATION_0 , ORIENTATION_0  , IMMOVABLE_TILE, IMMOVABLE_TILE, ORIENTATION_0  },
        {ORIENTATION_0  , ORIENTATION_0 , ORIENTATION_270, IMMOVABLE_TILE, IMMOVABLE_TILE, ORIENTATION_270},
        {ORIENTATION_270, IMMOVABLE_TILE, ORIENTATION_0  , IMMOVABLE_TILE, IMMOVABLE_TILE, ORIENTATION_90 },
        {ORIENTATION_180, IMMOVABLE_TILE, ORIENTATION_0  , IMMOVABLE_TILE, ORIENTATION_0 , ORIENTATION_180},
    },
    [PUZZLE_HO_OH] =
    {
        {ORIENTATION_180, ORIENTATION_180, ORIENTATION_90 , IMMOVABLE_TILE, IMMOVABLE_TILE, ORIENTATION_270},
        {ORIENTATION_180, ORIENTATION_0  , IMMOVABLE_TILE , IMMOVABLE_TILE, ORIENTATION_0 , ORIENTATION_270},
        {ORIENTATION_270, ORIENTATION_0  , ORIENTATION_0  , ORIENTATION_0 , IMMOVABLE_TILE, ORIENTATION_0  },
        {ORIENTATION_90 , ORIENTATION_0  , ORIENTATION_180, ORIENTATION_0 , IMMOVABLE_TILE, ORIENTATION_270},
    },
};

static const struct OamData sOamData_Cursor =
{
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 2,
};

static const struct SpriteTemplate sSpriteTemplate_Cursor =
{
    .tileTag = TAG_CURSOR,
    .paletteTag = TAG_CURSOR,
    .oam = &sOamData_Cursor,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Cursor,
};

static const struct OamData sOamData_Tiles =
{
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 2,
};

static const union AnimCmd sAnim_Tile0[] =
{
    ANIMCMD_FRAME(0 * 16, 30),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Tile1[] =
{
    ANIMCMD_FRAME(1 * 16, 30),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Tile2[] =
{
    ANIMCMD_FRAME(2 * 16, 30),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Tile3[] =
{
    ANIMCMD_FRAME(3 * 16, 30),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Tile4[] =
{
    ANIMCMD_FRAME(4 * 16, 30),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Tile5[] =
{
    ANIMCMD_FRAME(5 * 16, 30),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Tile6[] =
{
    ANIMCMD_FRAME(6 * 16, 30),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Tile7[] =
{
    ANIMCMD_FRAME(7 * 16, 30),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Tile8[] =
{
    ANIMCMD_FRAME(8 * 16, 30),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Tile9[] =
{
    ANIMCMD_FRAME(9 * 16, 30),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Tile10[] =
{
    ANIMCMD_FRAME(10 * 16, 30),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Tile11[] =
{
    ANIMCMD_FRAME(11 * 16, 30),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Tile12[] =
{
    ANIMCMD_FRAME(12 * 16, 30),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Tile13[] =
{
    ANIMCMD_FRAME(13 * 16, 30),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Tile14[] =
{
    ANIMCMD_FRAME(14 * 16, 30),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Tile15[] =
{
    ANIMCMD_FRAME(15 * 16, 30),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_Tiles[] =
{
    sAnim_Tile0,
    sAnim_Tile1,
    sAnim_Tile2,
    sAnim_Tile3,
    sAnim_Tile4,
    sAnim_Tile5,
    sAnim_Tile6,
    sAnim_Tile7,
    sAnim_Tile8,
    sAnim_Tile9,
    sAnim_Tile10,
    sAnim_Tile11,
    sAnim_Tile12,
    sAnim_Tile13,
    sAnim_Tile14,
    sAnim_Tile15,
};

static const union AffineAnimCmd sSpriteAffineAnim_Rotated0[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sSpriteAffineAnim_Rotated90[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, -64, 0),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sSpriteAffineAnim_Rotated180[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, -128, 0),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sSpriteAffineAnim_Rotated270[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 64, 0),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sSpriteAffineAnim_RotatingClockwise0to90[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME(0x0, 0x0, -8, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSpriteAffineAnim_RotatingClockwise90to180[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, -64, 0),
    AFFINEANIMCMD_FRAME(0x0, 0x0, -8, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSpriteAffineAnim_RotatingClockwise180to270[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, -128, 0),
    AFFINEANIMCMD_FRAME(0x0, 0x0, -8, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSpriteAffineAnim_RotatingClockwise270to360[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 64, 0),
    AFFINEANIMCMD_FRAME(0x0, 0x0, -8, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSpriteAffineAnim_RotatingAnticlockwise360to270[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 8, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSpriteAffineAnim_RotatingAnticlockwise270to180[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 64, 0),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 8, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSpriteAffineAnim_RotatingAnticlockwise180to90[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, -128, 0),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 8, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sSpriteAffineAnim_RotatingAnticlockwise90to0[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, -64, 0),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 8, 8),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sSpriteAffineAnimTable_RotateTile[] =
{
    sSpriteAffineAnim_Rotated0,
    sSpriteAffineAnim_Rotated90,
    sSpriteAffineAnim_Rotated180,
    sSpriteAffineAnim_Rotated270,
    sSpriteAffineAnim_RotatingAnticlockwise360to270,
    sSpriteAffineAnim_RotatingAnticlockwise90to0,
    sSpriteAffineAnim_RotatingAnticlockwise180to90,
    sSpriteAffineAnim_RotatingAnticlockwise270to180,
    sSpriteAffineAnim_RotatingClockwise0to90,
    sSpriteAffineAnim_RotatingClockwise90to180,
    sSpriteAffineAnim_RotatingClockwise180to270,
    sSpriteAffineAnim_RotatingClockwise270to360,
};

static const struct SpriteTemplate sSpriteTemplate_Tiles =
{
    .tileTag = GFXTAG_TILES,
    .paletteTag = TAG_CURSOR,
    .oam = &sOamData_Tiles,
    .anims = sAnims_Tiles,
    .images = NULL,
    .affineAnims = sSpriteAffineAnimTable_RotateTile,
    .callback = SpriteCallbackDummy,
};

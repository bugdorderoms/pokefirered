static u32 FldEff_ExclamationMarkIcon(void);
static u32 FldEff_Shadow(void);
static u32 FldEff_TallGrass(void);
static u32 FldEff_Ripple(void);
static u32 FldEff_FieldMoveShowMon(void);
static u32 FldEff_Ash(void);
static u32 FldEff_SurfBlob(void);
static u32 FldEff_UseSurf(void);
static u32 FldEff_Dust(void);
static u32 FldEff_DoubleExclMarkIcon(void);
static u32 FldEff_JumpTallGrass(void);
static u32 FldEff_SandFootprints(void);
static u32 FldEff_JumpBigSplash(void);
static u32 FldEff_Splash(void);
static u32 FldEff_JumpSmallSplash(void);
static u32 FldEff_LongGrass(void);
static u32 FldEff_JumpLongGrass(void);
static u32 FldEff_ShakingGrass(void);
static u32 FldEff_ShakingLongGrass(void);
static u32 FldEff_SandHole(void);
static u32 FldEff_WaterSurfacing(void);
static u32 FldEff_BerryTreeGrowthSparkle(void);
static u32 FldEff_DeepSandFootprints(void);
static u32 FldEff_PokecenterHeal(void);
static u32 FldEff_MoveDeoxysRock(void);
static u32 FldEff_DestroyDeoxysRock(void);
static u32 FldEff_TreeDisguise(void);
static u32 FldEff_MountainDisguise(void);
static u32 FldEff_NpcFlyOut(void);
static u32 FldEff_UseFly(void);
static u32 FldEff_FlyIn(void);
static u32 FldEff_QuestionMarkIcon(void);
static u32 FldEff_FeetInFlowingWater(void);
static u32 FldEff_BikeTireTracks(void);
static u32 FldEff_SandDisguise(void);
static u32 FldEff_SmileyFaceIcon(void);
static u32 FldEff_SandPile(void);
static u32 FldEff_UseVsSeeker(void);
static u32 FldEff_ShortGrass(void);
static u32 FldEff_HotSpringsWater(void);
static u32 FldEff_UseWaterfall(void);
static u32 FldEff_UseDive(void);
static u32 FldEff_Unk45(void);
static u32 FldEff_XIcon(void);
static u32 FldEff_CaveDust(void);
static u32 FldEff_Pokeball(void);
static u32 FldEff_PopOutOfAsh(void);
static u32 FldEff_LavaridgeGymWarp(void);
static u32 FldEff_Bubbles(void);
static u32 FldEff_Sparkle(void);
static u32 FldEff_FieldMoveShowMonInit(void);
static u32 FldEff_HallOfFameRecord(void);
static u32 FldEff_SnowFootprints(void);

static u32 (*const sFldEffectsTable[FLDEFF_COUNT])(void) = {
    [FLDEFF_EXCLAMATION_MARK_ICON]     = FldEff_ExclamationMarkIcon,
    [FLDEFF_USE_CUT_ON_GRASS]          = FldEff_UseCutOnGrass,
    [FLDEFF_USE_CUT_ON_TREE]           = FldEff_UseCutOnTree,
    [FLDEFF_SHADOW]                    = FldEff_Shadow,
    [FLDEFF_TALL_GRASS]                = FldEff_TallGrass,
    [FLDEFF_RIPPLE]                    = FldEff_Ripple,
    [FLDEFF_FIELD_MOVE_SHOW_MON]       = FldEff_FieldMoveShowMon,
    [FLDEFF_ASH]                       = FldEff_Ash,
    [FLDEFF_SURF_BLOB]                 = FldEff_SurfBlob,
    [FLDEFF_USE_SURF]                  = FldEff_UseSurf,
    [FLDEFF_DUST]                      = FldEff_Dust,
    [FLDEFF_DOUBLE_EXCL_MARK_ICON]     = FldEff_DoubleExclMarkIcon,
    [FLDEFF_JUMP_TALL_GRASS]           = FldEff_JumpTallGrass,
    [FLDEFF_SAND_FOOTPRINTS]           = FldEff_SandFootprints,
    [FLDEFF_JUMP_BIG_SPLASH]           = FldEff_JumpBigSplash,
    [FLDEFF_SPLASH]                    = FldEff_Splash,
    [FLDEFF_JUMP_SMALL_SPLASH]         = FldEff_JumpSmallSplash,
    [FLDEFF_LONG_GRASS]                = FldEff_LongGrass,
    [FLDEFF_JUMP_LONG_GRASS]           = FldEff_JumpLongGrass,
    [FLDEFF_SHAKING_GRASS]             = FldEff_ShakingGrass,
    [FLDEFF_SHAKING_LONG_GRASS]        = FldEff_ShakingLongGrass,
    [FLDEFF_SAND_HOLE]                 = FldEff_SandHole,
    [FLDEFF_WATER_SURFACING]           = FldEff_WaterSurfacing,
    [FLDEFF_BERRY_TREE_GROWTH_SPARKLE] = FldEff_BerryTreeGrowthSparkle,
    [FLDEFF_DEEP_SAND_FOOTPRINTS]      = FldEff_DeepSandFootprints,
    [FLDEFF_POKECENTER_HEAL]           = FldEff_PokecenterHeal,
    [FLDEFF_MOVE_DEOXYS_ROCK]          = FldEff_MoveDeoxysRock,
    [FLDEFF_DESTROY_DEOXYS_ROCK]       = FldEff_DestroyDeoxysRock,
    [FLDEFF_TREE_DISGUISE]             = FldEff_TreeDisguise,
    [FLDEFF_MOUNTAIN_DISGUISE]         = FldEff_MountainDisguise,
    [FLDEFF_NPCFLY_OUT]                = FldEff_NpcFlyOut,
    [FLDEFF_USE_FLY]                   = FldEff_UseFly,
    [FLDEFF_FLY_IN]                    = FldEff_FlyIn,
    [FLDEFF_QUESTION_MARK_ICON]        = FldEff_QuestionMarkIcon,
    [FLDEFF_FEET_IN_FLOWING_WATER]     = FldEff_FeetInFlowingWater,
    [FLDEFF_BIKE_TIRE_TRACKS]          = FldEff_BikeTireTracks,
    [FLDEFF_SAND_DISGUISE]             = FldEff_SandDisguise,
    [FLDEFF_SMILEY_FACE_ICON]          = FldEff_SmileyFaceIcon,
    [FLDEFF_USE_DIG]                   = FldEff_UseDig,
    [FLDEFF_SAND_PILE]                 = FldEff_SandPile,
    [FLDEFF_USE_VS_SEEKER]             = FldEff_UseVsSeeker,
    [FLDEFF_SHORT_GRASS]               = FldEff_ShortGrass,
    [FLDEFF_HOT_SPRINGS_WATER]         = FldEff_HotSpringsWater,
    [FLDEFF_USE_WATERFALL]             = FldEff_UseWaterfall,
    [FLDEFF_USE_DIVE]                  = FldEff_UseDive,
    [FLDEFF_UNK_45]                    = FldEff_Unk45,
    [FLDEFF_X_ICON]                    = FldEff_XIcon,
    [FLDEFF_CAVE_DUST]                 = FldEff_CaveDust,
    [FLDEFF_POKEBALL]                  = FldEff_Pokeball,
    [FLDEFF_POP_OUT_OF_ASH]            = FldEff_PopOutOfAsh,
    [FLDEFF_LAVARIDGE_GYM_WARP]        = FldEff_LavaridgeGymWarp,
    [FLDEFF_SWEET_SCENT]               = FldEff_SweetScent,
    [FLDEFF_USE_TELEPORT]              = FldEff_UseTeleport,
    [FLDEFF_BUBBLES]                   = FldEff_Bubbles,
    [FLDEFF_SPARKLE]                   = FldEff_Sparkle,
    [FLDEFF_CUT_GRASS]                 = FldEff_CutGrass,
    [FLDEFF_FIELD_MOVE_SHOW_MON_INIT]  = FldEff_FieldMoveShowMonInit,
    [FLDEFF_HALL_OF_FAME_RECORD]       = FldEff_HallOfFameRecord,
    [FLDEFF_SNOW_FOOTPRINTS]           = FldEff_SnowFootprints,
};

static const u16 sGfx_Emoticons[] = INCBIN_U16("graphics/misc/emoticons.4bpp");
static const u16 sFieldMoveStreaksTiles[] = INCBIN_U16("graphics/field_effects/unk_83CB5F0.4bpp");
static const u16 sFieldMoveStreaksPalette[] = INCBIN_U16("graphics/field_effects/unk_83CB7F0.gbapal");
static const u16 sFieldMoveStreaksTilemap[] = INCBIN_U16("graphics/field_effects/unk_83CB810.bin");
static const u16 sDarknessFieldMoveStreaksTiles[] = INCBIN_U16("graphics/field_effects/unk_83CBA90.4bpp");
static const u16 sDarknessFieldMoveStreaksPalette[] = INCBIN_U16("graphics/field_effects/unk_83CBB10.gbapal");
static const u16 sDarknessFieldMoveStreaksTilemap[] = INCBIN_U16("graphics/field_effects/unk_83CBB30.bin");
static const u16 sUnknown_83CAFD0[] = INCBIN_U16("graphics/field_effects/unk_83CAFD0.4bpp");
static const u16 sPokeballGfx[] = INCBIN_U16("graphics/field_effects/pokeball.4bpp");
static const u16 sPokeballPal[] = INCBIN_U16("graphics/field_effects/pokeball.gbapal");
static const u16 sFldEffUnk44_Tiles[] = INCBIN_U16("graphics/field_effects/unk_83CBDB0.4bpp");
static const u8 sSpriteImage_SlidingPokeball[] = INCBIN_U8("graphics/battle_transitions/sliding_pokeball.4bpp");
const u16 gSlidingPokeballBigPokeballPalette[] = INCBIN_U16("graphics/battle_transitions/sliding_pokeball.gbapal"); // Shared by big pokeball and sliding pokeball
static const u16 sUnknown_83CB3F0[] = INCBIN_U16("graphics/field_effects/unk_83CB3F0.4bpp");
static const u16 sUnknown_83CB3D0[] = INCBIN_U16("graphics/field_effects/unk_83CB3D0.gbapal");

const struct SpritePalette gSpritePalette_GeneralFieldEffect0 = { .data = gFieldEffectObjectPalette0, .tag = FLDEFF_PAL_TAG_GENERAL_0 };
const struct SpritePalette gSpritePalette_GeneralFieldEffect1 = { .data = gFieldEffectObjectPalette1, .tag = FLDEFF_PAL_TAG_GENERAL_1 };
const struct SpritePalette gSpritePalette_GeneralFieldEffect2 = { .data = gFieldEffectObjectPalette2, .tag = FLDEFF_PAL_TAG_GENERAL_2 };
const struct SpritePalette gSpritePalette_SlidingPokeball = { .data = gSlidingPokeballBigPokeballPalette, .tag = FLDEFF_PAL_TAG_SLIDING_POKEBALL };
const struct SpritePalette gSpritePalette_Ash = { .data = gFieldEffectPal_Ash, .tag = FLDEFF_PAL_TAG_ASH };

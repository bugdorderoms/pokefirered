#include "global.h"
#include "gflib.h"
#include "menu.h"
#include "item.h"
#include "decompress.h"
#include "graphics.h"
#include "item_menu_icons.h"
#include "event_data.h"
#include "new_menu_helpers.h"
#include "overworld.h"
#include "constants/items.h"

// item graphics
#include "data/graphics/items.h"

static EWRAM_DATA u8 sItemMenuIconSpriteIds[12] = {0};
static EWRAM_DATA void * sItemIconTilesBuffer = NULL;
static EWRAM_DATA void * sItemIconTilesBufferPadded = NULL;

static void SpriteCB_BagVisualSwitchingPockets(struct Sprite * sprite);
static void SpriteCB_ShakeBagSprite(struct Sprite * sprite);

static const struct OamData sOamData_BagOrSatchel = {
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .shape = ST_OAM_SQUARE,
    .size = 3,
    .priority = 1,
    .paletteNum = 0
};

static const union AnimCmd sAnim_BagOrSatchel_OpenItemPocket[] = {
    ANIMCMD_FRAME(   0, 5),
    ANIMCMD_FRAME(0x40, 0),
    ANIMCMD_END
};

static const union AnimCmd sAnim_BagOrSatchel_OpenKeyItemPocket[] = {
    ANIMCMD_FRAME(   0, 5),
    ANIMCMD_FRAME(0x80, 0),
    ANIMCMD_END
};

static const union AnimCmd sAnim_BagOrSatchel_OpenPokeBallsPocket[] = {
    ANIMCMD_FRAME(   0, 5),
    ANIMCMD_FRAME(0xc0, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sAnimTable_BagOrSatchel[] = {
    sAnim_BagOrSatchel_OpenKeyItemPocket,
    sAnim_BagOrSatchel_OpenPokeBallsPocket,
    sAnim_BagOrSatchel_OpenItemPocket
};

static const union AffineAnimCmd sAffineAnim_Idle[] = {
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd sAffineAnim_Wobble[] = {
    AFFINEANIMCMD_FRAME(0, 0, -2, 2),
    AFFINEANIMCMD_FRAME(0, 0,  2, 4),
    AFFINEANIMCMD_FRAME(0, 0, -2, 4),
    AFFINEANIMCMD_FRAME(0, 0,  2, 2),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const sAffineAnimTable_BagOrSatchel[] = {
    sAffineAnim_Idle,
    sAffineAnim_Wobble
};

const struct CompressedSpriteSheet gSpriteSheet_Backpack = {
    gUnknown_8E8362C,
    0x2000,
    100
};

const struct CompressedSpriteSheet gSpriteSheet_Satchel = {
    gUnknown_8E83DBC,
    0x2000,
    100
};

const struct CompressedSpritePalette gSpritePalette_BagOrSatchel = {
    gUnknown_8E84560,
    100
};

static const struct SpriteTemplate sSpriteTemplate_BagOrSatchel = {
    100,
    100,
    &sOamData_BagOrSatchel,
    sAnimTable_BagOrSatchel,
    NULL,
    sAffineAnimTable_BagOrSatchel,
    SpriteCallbackDummy
};

static const struct OamData gUnknown_83D4214 = {
    .affineMode = ST_OAM_AFFINE_OFF,
    .shape = ST_OAM_SQUARE,
    .size = 1,
    .priority = 1,
    .paletteNum = 1
};

static const union AnimCmd gUnknown_83D421C[] = {
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd gUnknown_83D4224[] = {
    ANIMCMD_FRAME(4, 0),
    ANIMCMD_END
};

static const union AnimCmd gUnknown_83D422C[] = {
    ANIMCMD_FRAME(0, 0, .hFlip = TRUE),
    ANIMCMD_END
};

static const union AnimCmd *const gUnknown_83D4234[] = {
    gUnknown_83D421C,
    gUnknown_83D4224,
    gUnknown_83D422C
};

const struct CompressedSpriteSheet gBagSwapSpriteSheet = {
    gFile_graphics_interface_bag_swap_sheet,
    0x100,
    101
};

const struct CompressedSpritePalette gBagSwapSpritePalette = {
    gFile_graphics_interface_bag_swap_palette,
    101
};

static const struct SpriteTemplate gUnknown_83D4250 = {
    101,
    101,
    &gUnknown_83D4214,
    gUnknown_83D4234,
    NULL,
    gDummySpriteAffineAnimTable,
    SpriteCallbackDummy
};

static const struct OamData sOamData_ItemIcon = {
    .affineMode = ST_OAM_AFFINE_OFF,
    .shape = ST_OAM_SQUARE,
    .size = 2,
    .priority = 1,
    .paletteNum = 2
};

static const union AnimCmd sAnim_ItemIcon_0[] = {
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sAnimTable_ItemIcon[] = {
    sAnim_ItemIcon_0
};

static const struct SpriteTemplate sSpriteTemplate_ItemIcon = {
    ITEMICON_TAG,
    ITEMICON_TAG,
    &sOamData_ItemIcon,
    sAnimTable_ItemIcon,
    NULL,
    gDummySpriteAffineAnimTable,
    SpriteCallbackDummy
};

static const union AffineAnimCmd sSpriteAffineAnim_KeyItemTM[] =
{
	AFFINEANIMCMD_FRAME(0, 0, 128, 1), //Start rotated left
	AFFINEANIMCMD_FRAME(16, 16, -8, 16), //Double sprite size + rotate right
	AFFINEANIMCMD_FRAME(0, 0, -3, 8), //End at right 24
	AFFINEANIMCMD_FRAME(0, 0, 3, 16), //End at left 24
	AFFINEANIMCMD_FRAME(0, 0, -3, 16), //End at right 24
	AFFINEANIMCMD_FRAME(0, 0, 3, 16), //End at left 24
	AFFINEANIMCMD_FRAME(0, 0, -3, 8), //End at 0
	AFFINEANIMCMD_END,
};

static const union AffineAnimCmd* const sSpriteAffineAnimTable_KeyItemTM[] =
{
	sSpriteAffineAnim_KeyItemTM,
};

static const void *const sItemIconGfxPtrs[][2] = {
    [ITEM_NONE] = {sItemIcon_QuestionMark, sItemIconPalette_QuestionMark},
    [ITEM_MASTER_BALL] = {sItemIcon_MasterBall, sItemIconPalette_MasterBall},
    [ITEM_ULTRA_BALL] = {sItemIcon_UltraBall, sItemIconPalette_UltraBall},
    [ITEM_GREAT_BALL] = {sItemIcon_GreatBall, sItemIconPalette_GreatBall},
    [ITEM_POKE_BALL] = {sItemIcon_PokeBall, sItemIconPalette_PokeBall},
    [ITEM_SAFARI_BALL] = {sItemIcon_SafariBall, sItemIconPalette_SafariBall},
    [ITEM_NET_BALL] = {sItemIcon_NetBall, sItemIconPalette_NetBall},
    [ITEM_DIVE_BALL] = {sItemIcon_DiveBall, sItemIconPalette_DiveBall},
    [ITEM_NEST_BALL] = {sItemIcon_NestBall, sItemIconPalette_NestBall},
    [ITEM_REPEAT_BALL] = {sItemIcon_RepeatBall, sItemIconPalette_RepeatBall},
    [ITEM_TIMER_BALL] = {sItemIcon_TimerBall, sItemIconPalette_RepeatBall},
    [ITEM_LUXURY_BALL] = {sItemIcon_LuxuryBall, sItemIconPalette_LuxuryBall},
    [ITEM_PREMIER_BALL] = {sItemIcon_PremierBall, sItemIconPalette_LuxuryBall},
    [ITEM_LEVEL_BALL] = {sItemIcon_LevelBall, sItemIconPalette_LevelBall},
    [ITEM_LURE_BALL] = {sItemIcon_LureBall, sItemIconPalette_LureBall},
    [ITEM_MOON_BALL] = {sItemIcon_MoonBall, sItemIconPalette_MoonBall},
    [ITEM_FRIEND_BALL] = {sItemIcon_FriendBall, sItemIconPalette_FriendBall},
    [ITEM_LOVE_BALL] = {sItemIcon_LoveBall, sItemIconPalette_LoveBall},
    [ITEM_HEAVY_BALL] = {sItemIcon_HeavyBall, sItemIconPalette_HeavyBall},
    [ITEM_FAST_BALL] = {sItemIcon_FastBall, sItemIconPalette_FastBall},
    [ITEM_SPORT_BALL] = {sItemIcon_SportBall, sItemIconPalette_SportBall},
    [ITEM_DUSK_BALL] = {sItemIcon_DuskBall, sItemIconPalette_DuskBall},
    [ITEM_QUICK_BALL] = {sItemIcon_QuickBall, sItemIconPalette_QuickBall},
    [ITEM_HEAL_BALL] = {sItemIcon_HealBall, sItemIconPalette_HealBall},
    [ITEM_CHERISH_BALL] = {sItemIcon_CherishBall, sItemIconPalette_CherishBall},
    [ITEM_PARK_BALL] = {sItemIcon_ParkBall, sItemIconPalette_ParkBall},
    [ITEM_BEAST_BALL] = {sItemIcon_BeastBall, sItemIconPalette_BeastBall},
	[ITEM_DREAM_BALL] = {sItemIcon_DreamBall, sItemIconPalette_DreamBall},
    [ITEM_POTION] = {sItemIcon_Potion, sItemIconPalette_Potion},
    [ITEM_ANTIDOTE] = {sItemIcon_Antidote, sItemIconPalette_Antidote},
    [ITEM_BURN_HEAL] = {sItemIcon_StatusHeal, sItemIconPalette_BurnHeal},
    [ITEM_ICE_HEAL] = {sItemIcon_StatusHeal, sItemIconPalette_IceHeal},
    [ITEM_AWAKENING] = {sItemIcon_StatusHeal, sItemIconPalette_Awakening},
    [ITEM_PARALYZE_HEAL] = {sItemIcon_StatusHeal, sItemIconPalette_ParalyzeHeal},
    [ITEM_FULL_RESTORE] = {sItemIcon_LargePotion, sItemIconPalette_FullRestore},
    [ITEM_MAX_POTION] = {sItemIcon_LargePotion, sItemIconPalette_MaxPotion},
    [ITEM_HYPER_POTION] = {sItemIcon_Potion, sItemIconPalette_HyperPotion},
    [ITEM_SUPER_POTION] = {sItemIcon_Potion, sItemIconPalette_SuperPotion},
    [ITEM_FULL_HEAL] = {sItemIcon_FullHeal, sItemIconPalette_FullHeal},
    [ITEM_REVIVE] = {sItemIcon_Revive, sItemIconPalette_Revive},
    [ITEM_MAX_REVIVE] = {sItemIcon_MaxRevive, sItemIconPalette_Revive},
    [ITEM_FRESH_WATER] = {sItemIcon_FreshWater, sItemIconPalette_FreshWater},
    [ITEM_SODA_POP] = {sItemIcon_SodaPop, sItemIconPalette_SodaPop},
    [ITEM_LEMONADE] = {sItemIcon_Lemonade, sItemIconPalette_Lemonade},
    [ITEM_MOOMOO_MILK] = {sItemIcon_MoomooMilk, sItemIconPalette_MoomooMilk},
    [ITEM_ENERGY_POWDER] = {sItemIcon_Powder, sItemIconPalette_EnergyPowder},
    [ITEM_ENERGY_ROOT] = {sItemIcon_EnergyRoot, sItemIconPalette_EnergyRoot},
    [ITEM_HEAL_POWDER] = {sItemIcon_Powder, sItemIconPalette_HealPowder},
    [ITEM_REVIVAL_HERB] = {sItemIcon_RevivalHerb, sItemIconPalette_RevivalHerb},
    [ITEM_ETHER] = {sItemIcon_Ether, sItemIconPalette_Ether},
    [ITEM_MAX_ETHER] = {sItemIcon_Ether, sItemIconPalette_MaxEther},
    [ITEM_ELIXIR] = {sItemIcon_Ether, sItemIconPalette_Elixir},
    [ITEM_MAX_ELIXIR] = {sItemIcon_Ether, sItemIconPalette_MaxElixir},
    [ITEM_LAVA_COOKIE] = {sItemIcon_LavaCookie, sItemIconPalette_LavaCookieAndLetter},
    [ITEM_BLUE_FLUTE] = {sItemIcon_Flute, sItemIconPalette_BlueFlute},
    [ITEM_YELLOW_FLUTE] = {sItemIcon_Flute, sItemIconPalette_YellowFlute},
    [ITEM_RED_FLUTE] = {sItemIcon_Flute, sItemIconPalette_RedFlute},
    [ITEM_BLACK_FLUTE] = {sItemIcon_Flute, sItemIconPalette_BlackFlute},
    [ITEM_WHITE_FLUTE] = {sItemIcon_Flute, sItemIconPalette_WhiteFlute},
    [ITEM_BERRY_JUICE] = {sItemIcon_BerryJuice, sItemIconPalette_BerryJuice},
    [ITEM_SACRED_ASH] = {sItemIcon_SacredAsh, sItemIconPalette_SacredAsh},
	[ITEM_SWEET_HEART] = {sItemIcon_SweetHeart, sItemIconPalette_SweetHeart},
    [ITEM_BIG_MALASADA] = {sItemIcon_BigMalasada, sItemIconPalette_BigMalasada},
    [ITEM_CASTELIACONE] = {sItemIcon_Casteliacone, sItemIconPalette_Casteliacone},
    [ITEM_LUMIOSE_GALETTE] = {sItemIcon_LumioseGalette, sItemIconPalette_LumioseGalette},
    [ITEM_RAGE_CANDY_BAR] = {sItemIcon_RageCandyBar, sItemIconPalette_RageCandyBar},
    [ITEM_SHALOUR_SABLE] = {sItemIcon_ShalourSable, sItemIconPalette_ShalourSable},
    [ITEM_OLD_GATEAU] = {sItemIcon_OldGateau, sItemIconPalette_OldGateau},
	[ITEM_DYNAMAX_CANDY] = {sItemIcon_DynamaxCandy, sItemIconPalette_DynamaxCandy},
	[ITEM_ABILITY_CAPSULE] = {sItemIcon_AbilityCapsule, sItemIconPalette_AbilityCapsule},
	[ITEM_ABILITY_PATCH] = {sItemIcon_AbilityPatch, sItemIconPalette_AbilityPatch},
	[ITEM_PEWTER_CRUNCHIES] = {sItemIcon_PewterCrunchies, sItemIconPalette_PewterCrunchies},
	[ITEM_HP_UP] = {sItemIcon_HPUp, sItemIconPalette_HPUp},
    [ITEM_PROTEIN] = {sItemIcon_Vitamin, sItemIconPalette_Protein},
    [ITEM_IRON] = {sItemIcon_Vitamin, sItemIconPalette_Iron},
    [ITEM_CARBOS] = {sItemIcon_Vitamin, sItemIconPalette_Carbos},
    [ITEM_CALCIUM] = {sItemIcon_Vitamin, sItemIconPalette_Calcium},
    [ITEM_RARE_CANDY] = {sItemIcon_RareCandy, sItemIconPalette_RareCandy},
    [ITEM_PP_UP] = {sItemIcon_PPUp, sItemIconPalette_PPUp},
    [ITEM_ZINC] = {sItemIcon_Vitamin, sItemIconPalette_Zinc},
    [ITEM_PP_MAX] = {sItemIcon_PPMax, sItemIconPalette_PPMax},
	[ITEM_MAX_MUSHROOMS] = {sItemIcon_MaxMushrooms, sItemIconPalette_MaxMushrooms},
	[ITEM_SUN_STONE] = {sItemIcon_SunStone, sItemIconPalette_SunStone},
    [ITEM_MOON_STONE] = {sItemIcon_MoonStone, sItemIconPalette_MoonStone},
    [ITEM_FIRE_STONE] = {sItemIcon_FireStone, sItemIconPalette_FireStone},
    [ITEM_THUNDER_STONE] = {sItemIcon_ThunderStone, sItemIconPalette_ThunderStone},
    [ITEM_WATER_STONE] = {sItemIcon_WaterStone, sItemIconPalette_WaterStone},
    [ITEM_LEAF_STONE] = {sItemIcon_LeafStone, sItemIconPalette_LeafStone},
    [ITEM_ICE_STONE] = {sItemIcon_IceStone, sItemIconPalette_IceStone},
	[ITEM_DUSK_STONE] = {sItemIcon_DuskStone, sItemIconPalette_DuskStone},
	[ITEM_DAWN_STONE] = {sItemIcon_DawnStone, sItemIconPalette_DawnStone},
	[ITEM_SHINY_STONE] = {sItemIcon_ShinyStone, sItemIconPalette_ShinyStone},
    [ITEM_PROTECTOR] = {sItemIcon_Protector, sItemIconPalette_Protector},
    [ITEM_MAGMARIZER] = {sItemIcon_Magmarizer, sItemIconPalette_Magmarizer},
	[ITEM_SACHET] = {sItemIcon_Sachet, sItemIconPalette_Sachet},
	[ITEM_WHIPPED_DREAM] = {sItemIcon_WhippedDream, sItemIconPalette_WhippedDream},
    [ITEM_REAPER_CLOTH] = {sItemIcon_ReaperCloth, sItemIconPalette_ReaperCloth},
    [ITEM_DUBIOUS_DISC] = {sItemIcon_DubiousDisc, sItemIconPalette_DubiousDisc},
    [ITEM_ELECTIRIZER] = {sItemIcon_Electirizer, sItemIconPalette_Electirizer},
	[ITEM_CRACKED_POT] = {sItemIcon_CrackedPot, sItemIconPalette_CrackedPot},
	[ITEM_SWEET_APPLE] = {sItemIcon_SweetApple, sItemIconPalette_SweetApple},
	[ITEM_TART_APPLE] = {sItemIcon_TartApple, sItemIconPalette_TartApple},
	[ITEM_BLACK_AUGURITE] = {sItemIcon_BlackAugurite, sItemIconPalette_BlackAugurite},
	[ITEM_GALARICA_CUFF] = {sItemIcon_GalaricaCuff, sItemIconPalette_GalaricaItem},
	[ITEM_GALARICA_WREATH] = {sItemIcon_GalaricaWreath, sItemIconPalette_GalaricaItem},
	[ITEM_LINKING_CORD] = {sItemIcon_LinkingCord, sItemIconPalette_LinkingCord},
	[ITEM_SCROLL_OF_DARKNESS] = {sItemIcon_Scroll, sItemIconPalette_ScrollOfDarkness},
	[ITEM_SCROLL_OF_WATERS] = {sItemIcon_Scroll, sItemIconPalette_ScrollOfWaters},
	[ITEM_AUSPICIOUS_ARMOR] = {sItemIcon_AuspiciousArmor, sItemIconPalette_AuspiciousArmor},
	[ITEM_MALICIOUS_ARMOR] = {sItemIcon_MaliciousArmor, sItemIconPalette_MaliciousArmor},
	[ITEM_HEALTH_WING] = {sItemIcon_Wing, sItemIconPalette_HealthWing},
    [ITEM_MUSCLE_WING] = {sItemIcon_Wing, sItemIconPalette_MuscleWing},
    [ITEM_RESIST_WING] = {sItemIcon_Wing, sItemIconPalette_ResistWing},
    [ITEM_GENIUS_WING] = {sItemIcon_Wing, sItemIconPalette_GeniusWing},
    [ITEM_CLEVER_WING] = {sItemIcon_Wing, sItemIconPalette_CleverWing},
    [ITEM_SWIFT_WING] = {sItemIcon_Wing, sItemIconPalette_SwiftWing},
    [ITEM_PRETTY_WING] = {sItemIcon_PrettyWing, sItemIconPalette_PrettyWing},
	[ITEM_ADAMANT_MINT] = {sItemIcon_Mint, sItemIconPalette_RedMint},
    [ITEM_BOLD_MINT] = {sItemIcon_Mint, sItemIconPalette_BlueMint},
    [ITEM_BRAVE_MINT] = {sItemIcon_Mint, sItemIconPalette_RedMint},
    [ITEM_CALM_MINT] = {sItemIcon_Mint, sItemIconPalette_PinkMint},
    [ITEM_CAREFUL_MINT] = {sItemIcon_Mint, sItemIconPalette_PinkMint},
    [ITEM_GENTLE_MINT] = {sItemIcon_Mint, sItemIconPalette_PinkMint},
    [ITEM_HASTY_MINT] = {sItemIcon_Mint, sItemIconPalette_GreenMint},
    [ITEM_IMPISH_MINT] = {sItemIcon_Mint, sItemIconPalette_BlueMint},
    [ITEM_JOLLY_MINT] = {sItemIcon_Mint, sItemIconPalette_GreenMint},
    [ITEM_LAX_MINT] = {sItemIcon_Mint, sItemIconPalette_BlueMint},
    [ITEM_LONELY_MINT] = {sItemIcon_Mint, sItemIconPalette_RedMint},
    [ITEM_MILD_MINT] = {sItemIcon_Mint, sItemIconPalette_LightBlueMint},
    [ITEM_MODEST_MINT] = {sItemIcon_Mint, sItemIconPalette_LightBlueMint},
    [ITEM_NAIVE_MINT] = {sItemIcon_Mint, sItemIconPalette_GreenMint},
    [ITEM_NAUGHTY_MINT] = {sItemIcon_Mint, sItemIconPalette_RedMint},
    [ITEM_QUIET_MINT] = {sItemIcon_Mint, sItemIconPalette_LightBlueMint},
    [ITEM_RASH_MINT] = {sItemIcon_Mint, sItemIconPalette_LightBlueMint},
    [ITEM_RELAXED_MINT] = {sItemIcon_Mint, sItemIconPalette_BlueMint},
    [ITEM_SASSY_MINT] = {sItemIcon_Mint, sItemIconPalette_PinkMint},
    [ITEM_SERIOUS_MINT] = {sItemIcon_Mint, sItemIconPalette_YellowMint},
    [ITEM_TIMID_MINT] = {sItemIcon_Mint, sItemIconPalette_GreenMint},
	[ITEM_EXP_CANDY_XS] = {sItemIcon_ExpCandyXs, sItemIconPalette_ExpCandy},
	[ITEM_EXP_CANDY_S] = {sItemIcon_ExpCandyS, sItemIconPalette_ExpCandy},
	[ITEM_EXP_CANDY_M] = {sItemIcon_ExpCandyM, sItemIconPalette_ExpCandy},
	[ITEM_EXP_CANDY_L] = {sItemIcon_ExpCandyL, sItemIconPalette_ExpCandy},
	[ITEM_EXP_CANDY_XL] = {sItemIcon_ExpCandyXl, sItemIconPalette_ExpCandy},
	/*[ITEM_NORMAL_TERA_SHARD] = {sItemIcon_TeraShard, sItemIconPalette_NormalTeraShard},
	[ITEM_FIRE_TERA_SHARD] = {sItemIcon_TeraShard, sItemIconPalette_FireTeraShard},
	[ITEM_WATER_TERA_SHARD] = {sItemIcon_TeraShard, sItemIconPalette_WaterTeraShard},
	[ITEM_ELECTRIC_TERA_SHARD] = {sItemIcon_TeraShard, sItemIconPalette_ElectricTeraShard},
	[ITEM_GRASS_TERA_SHARD] = {sItemIcon_TeraShard, sItemIconPalette_GrassTeraShard},
	[ITEM_ICE_TERA_SHARD] = {sItemIcon_TeraShard, sItemIconPalette_IceTeraShard},
	[ITEM_FIGHTING_TERA_SHARD] = {sItemIcon_TeraShard, sItemIconPalette_FightingTeraShard},
	[ITEM_POISON_TERA_SHARD] = {sItemIcon_TeraShard, sItemIconPalette_PoisonTeraShard},
	[ITEM_GROUND_TERA_SHARD] = {sItemIcon_TeraShard, sItemIconPalette_GroundTeraShard},
	[ITEM_FLYING_TERA_SHARD] = {sItemIcon_TeraShard, sItemIconPalette_FlyingTeraShard},
	[ITEM_PSYCHIC_TERA_SHARD] = {sItemIcon_TeraShard, sItemIconPalette_PsychicTeraShard},
	[ITEM_BUG_TERA_SHARD] = {sItemIcon_TeraShard, sItemIconPalette_BugTeraShard},
	[ITEM_ROCK_TERA_SHARD] = {sItemIcon_TeraShard, sItemIconPalette_RockTeraShard},
	[ITEM_GHOST_TERA_SHARD] = {sItemIcon_TeraShard, sItemIconPalette_GhostTeraShard},
	[ITEM_DRAGON_TERA_SHARD] = {sItemIcon_TeraShard, sItemIconPalette_DragonTeraShard},
	[ITEM_DARK_TERA_SHARD] = {sItemIcon_TeraShard, sItemIconPalette_DarkTeraShard},
	[ITEM_STEEL_TERA_SHARD] = {sItemIcon_TeraShard, sItemIconPalette_SteelTeraShard},
	[ITEM_FAIRY_TERA_SHARD] = {sItemIcon_TeraShard, sItemIconPalette_FairyTeraShard},*/
	[ITEM_CHERI_BERRY] = {sItemIcon_CheriBerry, sItemIconPalette_CheriBerry},
    [ITEM_CHESTO_BERRY] = {sItemIcon_ChestoBerry, sItemIconPalette_ChestoBerry},
    [ITEM_PECHA_BERRY] = {sItemIcon_PechaBerry, sItemIconPalette_PechaBerry},
    [ITEM_RAWST_BERRY] = {sItemIcon_RawstBerry, sItemIconPalette_RawstBerry},
    [ITEM_ASPEAR_BERRY] = {sItemIcon_AspearBerry, sItemIconPalette_AspearBerry},
    [ITEM_LEPPA_BERRY] = {sItemIcon_LeppaBerry, sItemIconPalette_LeppaBerry},
    [ITEM_ORAN_BERRY] = {sItemIcon_OranBerry, sItemIconPalette_OranBerry},
    [ITEM_PERSIM_BERRY] = {sItemIcon_PersimBerry, sItemIconPalette_PersimBerry},
    [ITEM_LUM_BERRY] = {sItemIcon_LumBerry, sItemIconPalette_LumBerry},
    [ITEM_SITRUS_BERRY] = {sItemIcon_SitrusBerry, sItemIconPalette_SitrusBerry},
    [ITEM_FIGY_BERRY] = {sItemIcon_FigyBerry, sItemIconPalette_FigyBerry},
    [ITEM_WIKI_BERRY] = {sItemIcon_WikiBerry, sItemIconPalette_WikiBerry},
    [ITEM_MAGO_BERRY] = {sItemIcon_MagoBerry, sItemIconPalette_MagoBerry},
    [ITEM_AGUAV_BERRY] = {sItemIcon_AguavBerry, sItemIconPalette_AguavBerry},
    [ITEM_IAPAPA_BERRY] = {sItemIcon_IapapaBerry, sItemIconPalette_IapapaBerry},
    [ITEM_RAZZ_BERRY] = {sItemIcon_RazzBerry, sItemIconPalette_RazzBerry},
    [ITEM_BLUK_BERRY] = {sItemIcon_BlukBerry, sItemIconPalette_BlukBerry},
    [ITEM_NANAB_BERRY] = {sItemIcon_NanabBerry, sItemIconPalette_NanabBerry},
    [ITEM_WEPEAR_BERRY] = {sItemIcon_WepearBerry, sItemIconPalette_WepearBerry},
    [ITEM_PINAP_BERRY] = {sItemIcon_PinapBerry, sItemIconPalette_PinapBerry},
    [ITEM_POMEG_BERRY] = {sItemIcon_PomegBerry, sItemIconPalette_PomegBerry},
    [ITEM_KELPSY_BERRY] = {sItemIcon_KelpsyBerry, sItemIconPalette_KelpsyBerry},
    [ITEM_QUALOT_BERRY] = {sItemIcon_QualotBerry, sItemIconPalette_QualotBerry},
    [ITEM_HONDEW_BERRY] = {sItemIcon_HondewBerry, sItemIconPalette_HondewBerry},
    [ITEM_GREPA_BERRY] = {sItemIcon_GrepaBerry, sItemIconPalette_GrepaBerry},
    [ITEM_TAMATO_BERRY] = {sItemIcon_TamatoBerry, sItemIconPalette_TamatoBerry},
    [ITEM_CORNN_BERRY] = {sItemIcon_CornnBerry, sItemIconPalette_CornnBerry},
    [ITEM_MAGOST_BERRY] = {sItemIcon_MagostBerry, sItemIconPalette_MagostBerry},
    [ITEM_RABUTA_BERRY] = {sItemIcon_RabutaBerry, sItemIconPalette_RabutaBerry},
    [ITEM_NOMEL_BERRY] = {sItemIcon_NomelBerry, sItemIconPalette_NomelBerry},
    [ITEM_SPELON_BERRY] = {sItemIcon_SpelonBerry, sItemIconPalette_SpelonBerry},
    [ITEM_PAMTRE_BERRY] = {sItemIcon_PamtreBerry, sItemIconPalette_PamtreBerry},
    [ITEM_WATMEL_BERRY] = {sItemIcon_WatmelBerry, sItemIconPalette_WatmelBerry},
    [ITEM_DURIN_BERRY] = {sItemIcon_DurinBerry, sItemIconPalette_DurinBerry},
    [ITEM_BELUE_BERRY] = {sItemIcon_BelueBerry, sItemIconPalette_BelueBerry},
    [ITEM_LIECHI_BERRY] = {sItemIcon_LiechiBerry, sItemIconPalette_LiechiBerry},
    [ITEM_GANLON_BERRY] = {sItemIcon_GanlonBerry, sItemIconPalette_GanlonBerry},
    [ITEM_SALAC_BERRY] = {sItemIcon_SalacBerry, sItemIconPalette_SalacBerry},
    [ITEM_PETAYA_BERRY] = {sItemIcon_PetayaBerry, sItemIconPalette_PetayaBerry},
    [ITEM_APICOT_BERRY] = {sItemIcon_ApicotBerry, sItemIconPalette_ApicotBerry},
    [ITEM_LANSAT_BERRY] = {sItemIcon_LansatBerry, sItemIconPalette_LansatBerry},
    [ITEM_STARF_BERRY] = {sItemIcon_StarfBerry, sItemIconPalette_StarfBerry},
    [ITEM_OCCA_BERRY] = {sItemIcon_OccaBerry, sItemIconPalette_OccaBerry},
    [ITEM_PASSHO_BERRY] = {sItemIcon_PasshoBerry, sItemIconPalette_PasshoBerry},
    [ITEM_WACAN_BERRY] = {sItemIcon_WacanBerry, sItemIconPalette_WacanBerry},
    [ITEM_RINDO_BERRY] = {sItemIcon_RindoBerry, sItemIconPalette_RindoBerry},
    [ITEM_YACHE_BERRY] = {sItemIcon_YacheBerry, sItemIconPalette_YacheBerry},
    [ITEM_CHOPLE_BERRY] = {sItemIcon_ChopleBerry, sItemIconPalette_ChopleBerry},
    [ITEM_KEBIA_BERRY] = {sItemIcon_KebiaBerry, sItemIconPalette_KebiaBerry},
    [ITEM_SHUCA_BERRY] = {sItemIcon_ShucaBerry, sItemIconPalette_ShucaBerry},
    [ITEM_COBA_BERRY] = {sItemIcon_CobaBerry, sItemIconPalette_CobaBerry},
    [ITEM_PAYAPA_BERRY] = {sItemIcon_PayapaBerry, sItemIconPalette_PayapaBerry},
    [ITEM_TANGA_BERRY] = {sItemIcon_TangaBerry, sItemIconPalette_TangaBerry},
    [ITEM_CHARTI_BERRY] = {sItemIcon_ChartiBerry, sItemIconPalette_ChartiBerry},
    [ITEM_KASIB_BERRY] = {sItemIcon_KasibBerry, sItemIconPalette_KasibBerry},
    [ITEM_HABAN_BERRY] = {sItemIcon_HabanBerry, sItemIconPalette_HabanBerry},
    [ITEM_COLBUR_BERRY] = {sItemIcon_ColburBerry, sItemIconPalette_ColburBerry},
    [ITEM_BABIRI_BERRY] = {sItemIcon_BabiriBerry, sItemIconPalette_BabiriBerry},
    [ITEM_CHILAN_BERRY] = {sItemIcon_ChilanBerry, sItemIconPalette_ChilanBerry},
    [ITEM_MICLE_BERRY] = {sItemIcon_MicleBerry, sItemIconPalette_MicleBerry},
    [ITEM_CUSTAP_BERRY] = {sItemIcon_CustapBerry, sItemIconPalette_CustapBerry},
    [ITEM_JABOCA_BERRY] = {sItemIcon_JabocaBerry, sItemIconPalette_JabocaBerry},
    [ITEM_ROWAP_BERRY] = {sItemIcon_RowapBerry, sItemIconPalette_RowapBerry},
    [ITEM_ROSELI_BERRY] = {sItemIcon_RoseliBerry, sItemIconPalette_RoseliBerry},
    [ITEM_KEE_BERRY] = {sItemIcon_KeeBerry, sItemIconPalette_KeeBerry},
    [ITEM_MARANGA_BERRY] = {sItemIcon_MarangaBerry, sItemIconPalette_MarangaBerry},
	// [ITEM_HOPO_BERRY] = {sItemIcon_HopoBerry, sItemIconPalette_HopoBerry},
    [ITEM_ENIGMA_BERRY] = {sItemIcon_EnigmaBerry, sItemIconPalette_EnigmaBerry},
	[ITEM_POKE_DOLL] = {sItemIcon_PokeDoll, sItemIconPalette_PokeDoll},
    [ITEM_FLUFFY_TAIL] = {sItemIcon_FluffyTail, sItemIconPalette_FluffyTail},
	[ITEM_BRIGHT_POWDER] = {sItemIcon_BrightPowder, sItemIconPalette_BrightPowder},
    [ITEM_WHITE_HERB] = {sItemIcon_InBattleHerb, sItemIconPalette_WhiteHerb},
    [ITEM_MACHO_BRACE] = {sItemIcon_MachoBrace, sItemIconPalette_MachoBrace},
	[ITEM_RAZOR_FANG] = {sItemIcon_RazorFang, sItemIconPalette_RazorFang},
    [ITEM_QUICK_CLAW] = {sItemIcon_QuickClaw, sItemIconPalette_QuickClaw},
    [ITEM_SOOTHE_BELL] = {sItemIcon_SootheBell, sItemIconPalette_SootheBell},
    [ITEM_MENTAL_HERB] = {sItemIcon_InBattleHerb, sItemIconPalette_MentalHerb},
    [ITEM_CHOICE_BAND] = {sItemIcon_ChoiceBand, sItemIconPalette_ChoiceBand},
    [ITEM_KINGS_ROCK] = {sItemIcon_KingsRock, sItemIconPalette_KingsRock},
    [ITEM_SILVER_POWDER] = {sItemIcon_SilverPowder, sItemIconPalette_SilverPowder},
    [ITEM_AMULET_COIN] = {sItemIcon_AmuletCoin, sItemIconPalette_AmuletCoin},
    [ITEM_CLEANSE_TAG] = {sItemIcon_CleanseTag, sItemIconPalette_CleanseTag},
    [ITEM_SOUL_DEW] = {sItemIcon_SoulDew, sItemIconPalette_SoulDew},
    [ITEM_DEEP_SEA_TOOTH] = {sItemIcon_DeepSeaTooth, sItemIconPalette_DeepSeaTooth},
    [ITEM_DEEP_SEA_SCALE] = {sItemIcon_DeepSeaScale, sItemIconPalette_DeepSeaScale},
    [ITEM_SMOKE_BALL] = {sItemIcon_SmokeBall, sItemIconPalette_SmokeBall},
    [ITEM_EVERSTONE] = {sItemIcon_Everstone, sItemIconPalette_Everstone},
    [ITEM_FOCUS_BAND] = {sItemIcon_FocusBand, sItemIconPalette_FocusBand},
    [ITEM_LUCKY_EGG] = {sItemIcon_LuckyEgg, sItemIconPalette_LuckyEgg},
    [ITEM_SCOPE_LENS] = {sItemIcon_ScopeLens, sItemIconPalette_ScopeLens},
    [ITEM_METAL_COAT] = {sItemIcon_MetalCoat, sItemIconPalette_MetalCoat},
    [ITEM_LEFTOVERS] = {sItemIcon_Leftovers, sItemIconPalette_Leftovers},
    [ITEM_DRAGON_SCALE] = {sItemIcon_DragonScale, sItemIconPalette_DragonScale},
    [ITEM_LIGHT_BALL] = {sItemIcon_LightBall, sItemIconPalette_LightBall},
    [ITEM_SOFT_SAND] = {sItemIcon_SoftSand, sItemIconPalette_SoftSand},
    [ITEM_HARD_STONE] = {sItemIcon_HardStone, sItemIconPalette_HardStone},
    [ITEM_MIRACLE_SEED] = {sItemIcon_MiracleSeed, sItemIconPalette_MiracleSeed},
    [ITEM_BLACK_GLASSES] = {sItemIcon_BlackGlasses, sItemIconPalette_BlackTypeEnhancingItem},
    [ITEM_BLACK_BELT] = {sItemIcon_BlackBelt, sItemIconPalette_BlackTypeEnhancingItem},
    [ITEM_MAGNET] = {sItemIcon_Magnet, sItemIconPalette_Magnet},
    [ITEM_MYSTIC_WATER] = {sItemIcon_MysticWater, sItemIconPalette_MysticWater},
    [ITEM_SHARP_BEAK] = {sItemIcon_SharpBeak, sItemIconPalette_SharpBeak},
    [ITEM_POISON_BARB] = {sItemIcon_PoisonBarb, sItemIconPalette_PoisonBarb},
    [ITEM_NEVER_MELT_ICE] = {sItemIcon_NeverMeltIce, sItemIconPalette_NeverMeltIce},
    [ITEM_SPELL_TAG] = {sItemIcon_SpellTag, sItemIconPalette_SpellTag},
    [ITEM_TWISTED_SPOON] = {sItemIcon_TwistedSpoon, sItemIconPalette_TwistedSpoon},
    [ITEM_CHARCOAL] = {sItemIcon_Charcoal, sItemIconPalette_Charcoal},
    [ITEM_DRAGON_FANG] = {sItemIcon_DragonFang, sItemIconPalette_DragonFang},
    [ITEM_SILK_SCARF] = {sItemIcon_SilkScarf, sItemIconPalette_SilkScarf},
    [ITEM_UP_GRADE] = {sItemIcon_UpGrade, sItemIconPalette_UpGrade},
    [ITEM_SHELL_BELL] = {sItemIcon_ShellBell, sItemIconPalette_Shell},
    [ITEM_LUCKY_PUNCH] = {sItemIcon_LuckyPunch, sItemIconPalette_LuckyPunch},
    [ITEM_METAL_POWDER] = {sItemIcon_MetalPowder, sItemIconPalette_MetalPowder},
    [ITEM_THICK_CLUB] = {sItemIcon_ThickClub, sItemIconPalette_ThickClub},
    [ITEM_LEEK] = {sItemIcon_Stick, sItemIconPalette_Stick},
    [ITEM_ADAMANT_ORB] = {sItemIcon_AdamantOrb, sItemIconPalette_AdamantOrb},
    [ITEM_LUSTROUS_ORB] = {sItemIcon_LustrousOrb, sItemIconPalette_LustrousOrb},
    [ITEM_GRISEOUS_ORB] = {sItemIcon_GriseousOrb, sItemIconPalette_GriseousOrb},
    [ITEM_EXPERT_BELT] = {sItemIcon_ExpertBelt, sItemIconPalette_ExpertBelt},
    [ITEM_POWER_HERB] = {sItemIcon_PowerHerb, sItemIconPalette_PowerHerb},
    [ITEM_WIDE_LENS] = {sItemIcon_WideLens, sItemIconPalette_WideLens},
    [ITEM_ZOOM_LENS] = {sItemIcon_ZoomLens, sItemIconPalette_ZoomLens},
    [ITEM_DESTINY_KNOT] = {sItemIcon_DestinyKnot, sItemIconPalette_DestinyKnot},
    [ITEM_SMOOTH_ROCK] = {sItemIcon_SmoothRock, sItemIconPalette_SmoothRock},
    [ITEM_DAMP_ROCK] = {sItemIcon_DampRock, sItemIconPalette_DampRock},
    [ITEM_HEAT_ROCK] = {sItemIcon_HeatRock, sItemIconPalette_HeatRock},
    [ITEM_ICY_ROCK] = {sItemIcon_IcyRock, sItemIconPalette_IcyRock},
    [ITEM_BIG_ROOT] = {sItemIcon_BigRoot, sItemIconPalette_EnergyRoot},
    [ITEM_LIGHT_CLAY] = {sItemIcon_LightClay, sItemIconPalette_LightClay},
    [ITEM_SAFETY_GOGGLES] = {sItemIcon_SafetyGoggles, sItemIconPalette_SafetyGoggles},
    [ITEM_ROCKY_HELMET] = {sItemIcon_RockyHelmet, sItemIconPalette_RockyHelmet},
    [ITEM_WEAKNESS_POLICY] = {sItemIcon_WeaknessPolicy, sItemIconPalette_WeaknessPolicy},
    [ITEM_ASSAULT_VEST] = {sItemIcon_AssaultVest, sItemIconPalette_AssaultVest},
    [ITEM_EVIOLITE] = {sItemIcon_Eviolite, sItemIconPalette_Eviolite},
    [ITEM_ABSORB_BULB] = {sItemIcon_AbsorbBulb, sItemIconPalette_AbsorbBulb},
    [ITEM_AIR_BALLOON] = {sItemIcon_AirBalloon, sItemIconPalette_AirBalloon},
    [ITEM_ADRENALINE_ORB] = {sItemIcon_AdrenalineOrb, sItemIconPalette_AdrenalineOrb},
    [ITEM_BINDING_BAND] = {sItemIcon_BindingBand, sItemIconPalette_BindingBand},
    [ITEM_CELL_BATTERY] = {sItemIcon_CellBattery, sItemIconPalette_CellBattery},
    [ITEM_EJECT_BUTTON] = {sItemIcon_EjectButton, sItemIconPalette_EjectButton},
    [ITEM_FLOAT_STONE] = {sItemIcon_FloatStone, sItemIconPalette_FloatStone},
    [ITEM_FOCUS_SASH] = {sItemIcon_FocusSash, sItemIconPalette_FocusSash},
    [ITEM_GRIP_CLAW] = {sItemIcon_GripClaw, sItemIconPalette_GripClaw},
    [ITEM_IRON_BALL] = {sItemIcon_IronBall, sItemIconPalette_IronBall},
    [ITEM_LAGGING_TAIL] = {sItemIcon_LaggingTail, sItemIconPalette_LaggingTail},
    [ITEM_LUMINOUS_MOSS] = {sItemIcon_LuminousMoss, sItemIconPalette_LuminousMoss},
    [ITEM_QUICK_POWDER] = {sItemIcon_SacredAsh, sItemIconPalette_QuickPowder},
    [ITEM_METRONOME] = {sItemIcon_Metronome, sItemIconPalette_Metronome},
    [ITEM_MUSCLE_BAND] = {sItemIcon_MuscleBand, sItemIconPalette_MuscleBand},
    [ITEM_PROTECTIVE_PADS] = {sItemIcon_ProtectivePads, sItemIconPalette_ProtectivePads},
    [ITEM_RED_CARD] = {sItemIcon_RedCard, sItemIconPalette_RedCard},
    [ITEM_RING_TARGET] = {sItemIcon_RingTarget, sItemIconPalette_RingTarget},
    [ITEM_SHED_SHELL] = {sItemIcon_ShedShell, sItemIconPalette_ShedShell},
    [ITEM_SNOWBALL] = {sItemIcon_Snowball, sItemIconPalette_Snowball},
    [ITEM_STICKY_BARB] = {sItemIcon_StickyBarb, sItemIconPalette_StickyBarb},
    [ITEM_TERRAIN_EXTENDER] = {sItemIcon_TerrainExtender, sItemIconPalette_TerrainExtender},
    [ITEM_WISE_GLASSES] = {sItemIcon_WiseGlasses, sItemIconPalette_WiseGlasses},
    [ITEM_ELECTRIC_SEED] = {sItemIcon_TerrainSeeds, sItemIconPalette_ElectricSeed},
    [ITEM_GRASSY_SEED] = {sItemIcon_TerrainSeeds, sItemIconPalette_GrassySeed},
    [ITEM_MISTY_SEED] = {sItemIcon_TerrainSeeds, sItemIconPalette_MistySeed},
    [ITEM_PSYCHIC_SEED] = {sItemIcon_TerrainSeeds, sItemIconPalette_PsychicSeed},
    [ITEM_LIFE_ORB] = {sItemIcon_LifeOrb, sItemIconPalette_LifeOrb},
    [ITEM_TOXIC_ORB] = {sItemIcon_ToxicOrb, sItemIconPalette_ToxicOrb},
    [ITEM_FLAME_ORB] = {sItemIcon_FlameOrb, sItemIconPalette_FlameOrb},
    [ITEM_BLACK_SLUDGE] = {sItemIcon_BlackSludge, sItemIconPalette_BlackSludge},
    [ITEM_CHOICE_SPECS] = {sItemIcon_ChoiceSpecs, sItemIconPalette_ChoiceSpecs},
    [ITEM_CHOICE_SCARF] = {sItemIcon_ChoiceScarf, sItemIconPalette_ChoiceScarf},
    [ITEM_EJECT_PACK] = {sItemIcon_EjectPack, sItemIconPalette_EjectPack},
    [ITEM_ROOM_SERVICE] = {sItemIcon_RoomService, sItemIconPalette_RoomService},
    [ITEM_BLUNDER_POLICY] = {sItemIcon_WeaknessPolicy, sItemIconPalette_BlunderPolicy},
    [ITEM_HEAVY_DUTY_BOOTS] = {sItemIcon_HeavyDutyBoots, sItemIconPalette_HeavyDutyBoots},
    [ITEM_UTILITY_UMBRELLA] = {sItemIcon_UtilityUmbrella, sItemIconPalette_UtilityUmbrella},
    [ITEM_THROAT_SPRAY] = {sItemIcon_ThroatSpray, sItemIconPalette_ThroatSpray},
	[ITEM_BOOSTER_ENERGY] = {sItemIcon_BoosterEnergy, sItemIconPalette_BoosterEnergy},
	[ITEM_ABILITY_SHIELD] = {sItemIcon_AbilityShield, sItemIconPalette_AbilityShield},
	// [ITEM_CLEAR_AMULET] = {sItemIcon_ClearAmulet, sItemIconPalette_ClearAmulet},
	[ITEM_MIRROR_HERB] = {sItemIcon_MirrorHerb, sItemIconPalette_MirrorHerb},
	[ITEM_PUNCHING_GLOVE] = {sItemIcon_PunchingGlove, sItemIconPalette_PunchingGlove},
	[ITEM_COVERT_CLOAK] = {sItemIcon_CovertCloak, sItemIconPalette_CovertCloak},
    [ITEM_LOADED_DICE] = {sItemIcon_LoadedDice, sItemIconPalette_LoadedDice},
	[ITEM_RUSTED_SWORD] = {sItemIcon_RustedSword, sItemIconPalette_RustedSword},
	[ITEM_RUSTED_SHIELD] = {sItemIcon_RustedShield, sItemIconPalette_RustedSword},
	[ITEM_RED_ORB] = {sItemIcon_RedOrb, sItemIconPalette_RedOrb},
    [ITEM_BLUE_ORB] = {sItemIcon_BlueOrb, sItemIconPalette_BlueOrb},
	[ITEM_RAZOR_CLAW] = {sItemIcon_RazorClaw, sItemIconPalette_RazorClaw},
	[ITEM_OVAL_STONE] = {sItemIcon_OvalStone, sItemIconPalette_OvalStone},
	[ITEM_GUARD_SPEC] = {sItemIcon_BattleStatItem, sItemIconPalette_GuardSpec},
    [ITEM_DIRE_HIT] = {sItemIcon_BattleStatItem, sItemIconPalette_DireHit},
    [ITEM_X_ATTACK] = {sItemIcon_BattleStatItem, sItemIconPalette_XAttack},
    [ITEM_X_DEFEND] = {sItemIcon_BattleStatItem, sItemIconPalette_XDefend},
    [ITEM_X_SPEED] = {sItemIcon_BattleStatItem, sItemIconPalette_XSpeed},
    [ITEM_X_ACCURACY] = {sItemIcon_BattleStatItem, sItemIconPalette_XAccuracy},
    [ITEM_X_SPECIAL] = {sItemIcon_BattleStatItem, sItemIconPalette_XSpecial},
    [ITEM_X_SPECIAL_DEFENSE] = {sItemIcon_BattleStatItem, sItemIconPalette_XSpecialDefense},
	[ITEM_POWER_BRACER] = {sItemIcon_PowerBracer, sItemIconPalette_PowerBracer},
    [ITEM_POWER_BELT] = {sItemIcon_PowerBelt, sItemIconPalette_PowerBelt},
    [ITEM_POWER_LENS] = {sItemIcon_PowerLens, sItemIconPalette_PowerLens},
    [ITEM_POWER_BAND] = {sItemIcon_PowerBand, sItemIconPalette_PowerBand},
    [ITEM_POWER_ANKLET] = {sItemIcon_PowerAnklet, sItemIconPalette_PowerAnklet},
    [ITEM_POWER_WEIGHT] = {sItemIcon_PowerWeight, sItemIconPalette_PowerWeight},
	[ITEM_SEA_INCENSE] = {sItemIcon_SeaIncense, sItemIconPalette_SeaIncense},
    [ITEM_LAX_INCENSE] = {sItemIcon_LaxIncense, sItemIconPalette_LaxIncense},
	[ITEM_LUCK_INCENSE] = {sItemIcon_LuckIncense, sItemIconPalette_LuckIncense},
    [ITEM_FULL_INCENSE] = {sItemIcon_FullIncense, sItemIconPalette_FullIncense},
    [ITEM_ODD_INCENSE] = {sItemIcon_OddIncense, sItemIconPalette_OddIncense},
    [ITEM_ROCK_INCENSE] = {sItemIcon_RockIncense, sItemIconPalette_RockIncense},
    [ITEM_ROSE_INCENSE] = {sItemIcon_RoseIncense, sItemIconPalette_RoseIncense},
    [ITEM_WAVE_INCENSE] = {sItemIcon_WaveIncense, sItemIconPalette_WaveIncense},
    [ITEM_VENUSAURITE] = {sItemIcon_Venusaurite, sItemIconPalette_Venusaurite},
    [ITEM_CHARIZARDITE_X] = {sItemIcon_CharizarditeX, sItemIconPalette_CharizarditeX},
    [ITEM_CHARIZARDITE_Y] = {sItemIcon_CharizarditeY, sItemIconPalette_CharizarditeY},
    [ITEM_BLASTOISINITE] = {sItemIcon_Blastoisinite, sItemIconPalette_Blastoisinite},
    [ITEM_BEEDRILLITE] = {sItemIcon_Beedrillite, sItemIconPalette_Beedrillite},
    [ITEM_PIDGEOTITE] = {sItemIcon_Pidgeotite, sItemIconPalette_Pidgeotite},
    [ITEM_ALAKAZITE] = {sItemIcon_Alakazite, sItemIconPalette_Alakazite},
    [ITEM_SLOWBRONITE] = {sItemIcon_Slowbronite, sItemIconPalette_Slowbronite},
    [ITEM_GENGARITE] = {sItemIcon_Gengarite, sItemIconPalette_Gengarite},
    [ITEM_KANGASKHANITE] = {sItemIcon_Kangaskhanite, sItemIconPalette_Kangaskhanite},
    [ITEM_PINSIRITE] = {sItemIcon_Pinsirite, sItemIconPalette_Pinsirite},
    [ITEM_GYARADOSITE] = {sItemIcon_Gyaradosite, sItemIconPalette_Gyaradosite},
    [ITEM_AERODACTYLITE] = {sItemIcon_Aerodactylite, sItemIconPalette_Aerodactylite},
    [ITEM_MEWTWONITE_X] = {sItemIcon_MewtwoniteX, sItemIconPalette_MewtwoniteX},
    [ITEM_MEWTWONITE_Y] = {sItemIcon_MewtwoniteY, sItemIconPalette_MewtwoniteY},
    [ITEM_AMPHAROSITE] = {sItemIcon_Ampharosite, sItemIconPalette_Ampharosite},
    [ITEM_STEELIXITE] = {sItemIcon_Steelixite, sItemIconPalette_Steelixite},
    [ITEM_SCIZORITE] = {sItemIcon_Scizorite, sItemIconPalette_Scizorite},
    [ITEM_HERACRONITE] = {sItemIcon_Heracronite, sItemIconPalette_Heracronite},
    [ITEM_HOUNDOOMINITE] = {sItemIcon_Houndoominite, sItemIconPalette_Houndoominite},
    [ITEM_TYRANITARITE] = {sItemIcon_Tyranitarite, sItemIconPalette_Tyranitarite},
    [ITEM_SCEPTILITE] = {sItemIcon_Tyranitarite, sItemIconPalette_Tyranitarite}, // Same as tyranitarite
    [ITEM_BLAZIKENITE] = {sItemIcon_Blazikenite, sItemIconPalette_Blazikenite},
    [ITEM_SWAMPERTITE] = {sItemIcon_Swampertite, sItemIconPalette_Swampertite},
    [ITEM_GARDEVOIRITE] = {sItemIcon_Gardevoirite, sItemIconPalette_Gardevoirite},
    [ITEM_SABLENITE] = {sItemIcon_Sablenite, sItemIconPalette_Sablenite},
    [ITEM_MAWILITE] = {sItemIcon_Mawilite, sItemIconPalette_Mawilite},
    [ITEM_AGGRONITE] = {sItemIcon_Aggronite, sItemIconPalette_Aggronite},
    [ITEM_MEDICHAMITE] = {sItemIcon_Medichamite, sItemIconPalette_Medichamite},
    [ITEM_MANECTITE] = {sItemIcon_Manectite, sItemIconPalette_Manectite},
    [ITEM_SHARPEDONITE] = {sItemIcon_Sharpedonite, sItemIconPalette_Sharpedonite},
    [ITEM_CAMERUPTITE] = {sItemIcon_Cameruptite, sItemIconPalette_Cameruptite},
    [ITEM_ALTARIANITE] = {sItemIcon_Altarianite, sItemIconPalette_Altarianite},
    [ITEM_BANETTITE] = {sItemIcon_Banettite, sItemIconPalette_Banettite},
    [ITEM_ABSOLITE] = {sItemIcon_Absolite, sItemIconPalette_Absolite},
    [ITEM_GLALITITE] = {sItemIcon_Glalitite, sItemIconPalette_Glalitite},
    [ITEM_SALAMENCITE] = {sItemIcon_Salamencite, sItemIconPalette_Salamencite},
    [ITEM_METAGROSSITE] = {sItemIcon_Metagrossite, sItemIconPalette_Metagrossite},
    [ITEM_LATIASITE] = {sItemIcon_Latiasite, sItemIconPalette_Latiasite},
    [ITEM_LATIOSITE] = {sItemIcon_Latiosite, sItemIconPalette_Latiosite},
    [ITEM_LOPUNNITE] = {sItemIcon_Lopunnite, sItemIconPalette_Lopunnite},
    [ITEM_GARCHOMPITE] = {sItemIcon_Garchompite, sItemIconPalette_Garchompite},
    [ITEM_LUCARIONITE] = {sItemIcon_Lucarionite, sItemIconPalette_Lucarionite},
    [ITEM_ABOMASITE] = {sItemIcon_Abomasite, sItemIconPalette_Abomasite},
    [ITEM_GALLADITE] = {sItemIcon_Galladite, sItemIconPalette_Galladite},
    [ITEM_AUDINITE] = {sItemIcon_Audinite, sItemIconPalette_Audinite},
    [ITEM_DIANCITE] = {sItemIcon_Diancite, sItemIconPalette_Diancite},
    [ITEM_NORMAL_GEM] = {sItemIcon_Gem, sItemIconPalette_NormalGem},
    [ITEM_FIGHTING_GEM] = {sItemIcon_Gem, sItemIconPalette_FightingGem},
    [ITEM_FLYING_GEM] = {sItemIcon_Gem, sItemIconPalette_FlyingGem},
    [ITEM_POISON_GEM] = {sItemIcon_Gem, sItemIconPalette_PoisonGem},
    [ITEM_GROUND_GEM] = {sItemIcon_Gem, sItemIconPalette_GroundGem},
    [ITEM_ROCK_GEM] = {sItemIcon_Gem, sItemIconPalette_RockGem},
    [ITEM_BUG_GEM] = {sItemIcon_Gem, sItemIconPalette_BugGem},
    [ITEM_GHOST_GEM] = {sItemIcon_Gem, sItemIconPalette_GhostGem},
    [ITEM_STEEL_GEM] = {sItemIcon_Gem, sItemIconPalette_SteelGem},
    [ITEM_FIRE_GEM] = {sItemIcon_Gem, sItemIconPalette_FireGem},
    [ITEM_WATER_GEM] = {sItemIcon_Gem, sItemIconPalette_WaterGem},
    [ITEM_GRASS_GEM] = {sItemIcon_Gem, sItemIconPalette_GrassGem},
    [ITEM_ELECTRIC_GEM] = {sItemIcon_Gem, sItemIconPalette_ElectricGem},
    [ITEM_PSYCHIC_GEM] = {sItemIcon_Gem, sItemIconPalette_PsychicGem},
    [ITEM_ICE_GEM] = {sItemIcon_Gem, sItemIconPalette_IceGem},
    [ITEM_DRAGON_GEM] = {sItemIcon_Gem, sItemIconPalette_DragonGem},
    [ITEM_DARK_GEM] = {sItemIcon_Gem, sItemIconPalette_DarkGem},
    [ITEM_FAIRY_GEM] = {sItemIcon_Gem, sItemIconPalette_FairyGem},
    [ITEM_FIST_PLATE] = {sItemIcon_Plate, sItemIconPalette_FistPlate},
    [ITEM_SKY_PLATE] = {sItemIcon_Plate, sItemIconPalette_SkyPlate},
    [ITEM_TOXIC_PLATE] = {sItemIcon_Plate, sItemIconPalette_ToxicPlate},
    [ITEM_EARTH_PLATE] = {sItemIcon_Plate, sItemIconPalette_EarthPlate},
    [ITEM_STONE_PLATE] = {sItemIcon_Plate, sItemIconPalette_StonePlate},
    [ITEM_INSECT_PLATE] = {sItemIcon_Plate, sItemIconPalette_InsectPlate},
    [ITEM_SPOOKY_PLATE] = {sItemIcon_Plate, sItemIconPalette_SpookyPlate},
    [ITEM_IRON_PLATE] = {sItemIcon_Plate, sItemIconPalette_IronPlate},
    [ITEM_FLAME_PLATE] = {sItemIcon_Plate, sItemIconPalette_FlamePlate},
    [ITEM_SPLASH_PLATE] = {sItemIcon_Plate, sItemIconPalette_SplashPlate},
    [ITEM_MEADOW_PLATE] = {sItemIcon_Plate, sItemIconPalette_MeadowPlate},
    [ITEM_ZAP_PLATE] = {sItemIcon_Plate, sItemIconPalette_ZapPlate},
    [ITEM_MIND_PLATE] = {sItemIcon_Plate, sItemIconPalette_MindPlate},
    [ITEM_ICICLE_PLATE] = {sItemIcon_Plate, sItemIconPalette_IciclePlate},
    [ITEM_DRACO_PLATE] = {sItemIcon_Plate, sItemIconPalette_DracoPlate},
    [ITEM_DREAD_PLATE] = {sItemIcon_Plate, sItemIconPalette_DreadPlate},
    [ITEM_PIXIE_PLATE] = {sItemIcon_Plate, sItemIconPalette_PixiePlate},
    [ITEM_BURN_DRIVE] = {sItemIcon_Drive, sItemIconPalette_BurnDrive},
    [ITEM_DOUSE_DRIVE] = {sItemIcon_Drive, sItemIconPalette_DouseDrive},
    [ITEM_SHOCK_DRIVE] = {sItemIcon_Drive, sItemIconPalette_ShockDrive},
    [ITEM_CHILL_DRIVE] = {sItemIcon_Drive, sItemIconPalette_ChillDrive},
    [ITEM_FIRE_MEMORY] = {sItemIcon_FireMemory, sItemIconPalette_FireMemory},
    [ITEM_WATER_MEMORY] = {sItemIcon_WaterMemory, sItemIconPalette_WaterMemory},
    [ITEM_ELECTRIC_MEMORY] = {sItemIcon_ElectricMemory, sItemIconPalette_ElectricMemory},
    [ITEM_GRASS_MEMORY] = {sItemIcon_GrassMemory, sItemIconPalette_GrassMemory},
    [ITEM_ICE_MEMORY] = {sItemIcon_IceMemory, sItemIconPalette_IceMemory},
    [ITEM_FIGHTING_MEMORY] = {sItemIcon_FightingMemory, sItemIconPalette_FightingMemory},
    [ITEM_POISON_MEMORY] = {sItemIcon_PoisonMemory, sItemIconPalette_PoisonMemory},
    [ITEM_GROUND_MEMORY] = {sItemIcon_GroundMemory, sItemIconPalette_GroundMemory},
    [ITEM_FLYING_MEMORY] = {sItemIcon_FlyingMemory, sItemIconPalette_FlyingMemory},
    [ITEM_PSYCHIC_MEMORY] = {sItemIcon_PsychicMemory, sItemIconPalette_PsychicMemory},
    [ITEM_BUG_MEMORY] = {sItemIcon_BugMemory, sItemIconPalette_BugMemory},
    [ITEM_ROCK_MEMORY] = {sItemIcon_RockMemory, sItemIconPalette_RockMemory},
    [ITEM_GHOST_MEMORY] = {sItemIcon_GhostMemory, sItemIconPalette_GhostMemory},
    [ITEM_DRAGON_MEMORY] = {sItemIcon_DragonMemory, sItemIconPalette_DragonMemory},
    [ITEM_DARK_MEMORY] = {sItemIcon_DarkMemory, sItemIconPalette_DarkMemory},
    [ITEM_STEEL_MEMORY] = {sItemIcon_SteelMemory, sItemIconPalette_SteelMemory},
    [ITEM_FAIRY_MEMORY] = {sItemIcon_FairyMemory, sItemIconPalette_FairyMemory},
    [ITEM_NORMALIUM_Z] = {sItemIcon_NormaliumZ, sItemIconPalette_NormaliumZ},
    [ITEM_FIGHTINIUM_Z] = {sItemIcon_FightiniumZ, sItemIconPalette_FightiniumZ},
    [ITEM_FLYINIUM_Z] = {sItemIcon_FlyiniumZ, sItemIconPalette_FlyiniumZ},
    [ITEM_POISONIUM_Z] = {sItemIcon_PoisoniumZ, sItemIconPalette_PoisoniumZ},
    [ITEM_GROUNDIUM_Z] = {sItemIcon_GroundiumZ, sItemIconPalette_GroundiumZ},
    [ITEM_ROCKIUM_Z] = {sItemIcon_RockiumZ, sItemIconPalette_RockiumZ},
    [ITEM_BUGINIUM_Z] = {sItemIcon_BuginiumZ, sItemIconPalette_BuginiumZ},
    [ITEM_GHOSTIUM_Z] = {sItemIcon_GhostiumZ, sItemIconPalette_GhostiumZ},
    [ITEM_STEELIUM_Z] = {sItemIcon_SteeliumZ, sItemIconPalette_SteeliumZ},
    [ITEM_FIRIUM_Z] = {sItemIcon_FiriumZ, sItemIconPalette_FiriumZ},
    [ITEM_WATERIUM_Z] = {sItemIcon_WateriumZ, sItemIconPalette_WateriumZ},
    [ITEM_GRASSIUM_Z] = {sItemIcon_GrassiumZ, sItemIconPalette_GrassiumZ},
    [ITEM_ELECTRIUM_Z] = {sItemIcon_ElectriumZ, sItemIconPalette_ElectriumZ},
    [ITEM_PSYCHIUM_Z] = {sItemIcon_PsychiumZ, sItemIconPalette_PsychiumZ},
    [ITEM_ICIUM_Z] = {sItemIcon_IciumZ, sItemIconPalette_IciumZ},
    [ITEM_DRAGONIUM_Z] = {sItemIcon_DragoniumZ, sItemIconPalette_DragoniumZ},
    [ITEM_DARKINIUM_Z] = {sItemIcon_DarkiniumZ, sItemIconPalette_DarkiniumZ},
    [ITEM_FAIRIUM_Z] = {sItemIcon_FairiumZ, sItemIconPalette_FairiumZ},
    [ITEM_ALORAICHIUM_Z] = {sItemIcon_AloraichiumZ, sItemIconPalette_AloraichiumZ},
    [ITEM_DECIDIUM_Z] = {sItemIcon_DecidiumZ, sItemIconPalette_DecidiumZ},
    [ITEM_EEVIUM_Z] = {sItemIcon_EeviumZ, sItemIconPalette_EeviumZ},
    [ITEM_INCINIUM_Z] = {sItemIcon_InciniumZ, sItemIconPalette_InciniumZ},
    [ITEM_KOMMONIUM_Z] = {sItemIcon_KommoniumZ, sItemIconPalette_KommoniumZ},
    [ITEM_LUNALIUM_Z] = {sItemIcon_LunaliumZ, sItemIconPalette_LunaliumZ},
    [ITEM_LYCANIUM_Z] = {sItemIcon_LycaniumZ, sItemIconPalette_LycaniumZ},
    [ITEM_MARSHADIUM_Z] = {sItemIcon_MarshadiumZ, sItemIconPalette_MarshadiumZ},
    [ITEM_MEWNIUM_Z] = {sItemIcon_MewniumZ, sItemIconPalette_MewniumZ},
    [ITEM_MIMIKIUM_Z] = {sItemIcon_MimikiumZ, sItemIconPalette_MimikiumZ},
    [ITEM_PIKANIUM_Z] = {sItemIcon_PikaniumZ, sItemIconPalette_PikaniumZ},
    [ITEM_PIKASHUNIUM_Z] = {sItemIcon_PikashuniumZ, sItemIconPalette_PikashuniumZ},
    [ITEM_PRIMARIUM_Z] = {sItemIcon_PrimariumZ, sItemIconPalette_PrimariumZ},
    [ITEM_SNORLIUM_Z] = {sItemIcon_SnorliumZ, sItemIconPalette_SnorliumZ},
    [ITEM_SOLGANIUM_Z] = {sItemIcon_SolganiumZ, sItemIconPalette_SolganiumZ},
    [ITEM_TAPUNIUM_Z] = {sItemIcon_TapuniumZ, sItemIconPalette_TapuniumZ},
	[ITEM_ULTRANECROZIUM_Z] = {sItemIcon_UltranecroziumZ, sItemIconPalette_UltranecroziumZ},
    [ITEM_SHOAL_SHELL] = {sItemIcon_ShoalShell, sItemIconPalette_Shell},
	[ITEM_TINY_MUSHROOM] = {sItemIcon_TinyMushroom, sItemIconPalette_Mushroom},
    [ITEM_BIG_MUSHROOM] = {sItemIcon_BigMushroom, sItemIconPalette_Mushroom},
    [ITEM_PEARL] = {sItemIcon_Pearl, sItemIconPalette_Pearl},
    [ITEM_BIG_PEARL] = {sItemIcon_BigPearl, sItemIconPalette_Pearl},
    [ITEM_STARDUST] = {sItemIcon_Stardust, sItemIconPalette_Star},
    [ITEM_STAR_PIECE] = {sItemIcon_StarPiece, sItemIconPalette_Star},
    [ITEM_NUGGET] = {sItemIcon_Nugget, sItemIconPalette_Nugget},
    [ITEM_HEART_SCALE] = {sItemIcon_HeartScale, sItemIconPalette_HeartScale},
    [ITEM_RARE_BONE] = {sItemIcon_RareBone, sItemIconPalette_RareBone},
    [ITEM_PEARL_STRING] = {sItemIcon_PearlString, sItemIconPalette_Pearl},
    [ITEM_BIG_NUGGET] = {sItemIcon_BigNugget, sItemIconPalette_Nugget},
    [ITEM_COMET_SHARD] = {sItemIcon_StarPiece, sItemIconPalette_CometShard},
    [ITEM_BALM_MUSHROOM] = {sItemIcon_BalmMushroom, sItemIconPalette_BalmMushroom},
	[ITEM_BOTTLE_CAP] = {sItemIcon_BottleCap, sItemIconPalette_BottleCap},
    [ITEM_GOLD_BOTTLE_CAP] = {sItemIcon_GoldBottleCap, sItemIconPalette_GoldBottleCap},
	[ITEM_HELIX_FOSSIL] = {sItemIcon_HelixFossil, sItemIconPalette_KantoFossil},
    [ITEM_DOME_FOSSIL] = {sItemIcon_DomeFossil, sItemIconPalette_KantoFossil},
	[ITEM_OLD_AMBER] = {sItemIcon_OldAmber, sItemIconPalette_OldAmber},
	[ITEM_ROOT_FOSSIL] = {sItemIcon_RootFossil, sItemIconPalette_HoennFossil},
    [ITEM_CLAW_FOSSIL] = {sItemIcon_ClawFossil, sItemIconPalette_HoennFossil},
	[ITEM_SKULL_FOSSIL] = {sItemIcon_SkullFossil, sItemIconPalette_SinnohFossil},
    [ITEM_ARMOR_FOSSIL] = {sItemIcon_ArmorFossil, sItemIconPalette_SinnohFossil},
    [ITEM_COVER_FOSSIL] = {sItemIcon_CoverFossil, sItemIconPalette_UnovaFossil},
    [ITEM_PLUME_FOSSIL] = {sItemIcon_PlumeFossil, sItemIconPalette_UnovaFossil},
    [ITEM_JAW_FOSSIL] = {sItemIcon_JawFossil, sItemIconPalette_JawFossil},
    [ITEM_SAIL_FOSSIL] = {sItemIcon_SailFossil, sItemIconPalette_SailFossil},
	[ITEM_BIRD_FOSSIL] = {sItemIcon_BirdFossil, sItemIconPalette_BirdFossil},
    [ITEM_FISH_FOSSIL] = {sItemIcon_FishFossil, sItemIconPalette_FishFossil},
    [ITEM_DRAKE_FOSSIL] = {sItemIcon_DrakeFossil, sItemIconPalette_DrakeFossil},
    [ITEM_DINO_FOSSIL] = {sItemIcon_DinoFossil, sItemIconPalette_DinoFossil},
	[ITEM_ORANGE_MAIL] = {sItemIcon_OrangeMail, sItemIconPalette_OrangeMail},
    [ITEM_HARBOR_MAIL] = {sItemIcon_HarborMail, sItemIconPalette_HarborMail},
    [ITEM_GLITTER_MAIL] = {sItemIcon_GlitterMail, sItemIconPalette_GlitterMail},
    [ITEM_MECH_MAIL] = {sItemIcon_MechMail, sItemIconPalette_MechMail},
    [ITEM_WOOD_MAIL] = {sItemIcon_WoodMail, sItemIconPalette_WoodMail},
    [ITEM_WAVE_MAIL] = {sItemIcon_WaveMail, sItemIconPalette_WaveMail},
    [ITEM_BEAD_MAIL] = {sItemIcon_BeadMail, sItemIconPalette_BeadMail},
    [ITEM_SHADOW_MAIL] = {sItemIcon_ShadowMail, sItemIconPalette_ShadowMail},
    [ITEM_TROPIC_MAIL] = {sItemIcon_TropicMail, sItemIconPalette_TropicMail},
    [ITEM_DREAM_MAIL] = {sItemIcon_DreamMail, sItemIconPalette_DreamMail},
    [ITEM_FAB_MAIL] = {sItemIcon_FabMail, sItemIconPalette_FabMail},
    [ITEM_RETRO_MAIL] = {sItemIcon_RetroMail, sItemIconPalette_RetroMail}, 
	[ITEM_RED_SHARD] = {sItemIcon_Shard, sItemIconPalette_RedShard},
    [ITEM_BLUE_SHARD] = {sItemIcon_Shard, sItemIconPalette_BlueShard},
    [ITEM_YELLOW_SHARD] = {sItemIcon_Shard, sItemIconPalette_YellowShard},
    [ITEM_GREEN_SHARD] = {sItemIcon_Shard, sItemIconPalette_GreenShard},
	[ITEM_WISHING_PIECE] = {sItemIcon_WishingPiece, sItemIconPalette_WishingPiece},
    [ITEM_HONEY] = {sItemIcon_Honey, sItemIconPalette_Honey},
	[ITEM_REPEL] = {sItemIcon_Repel, sItemIconPalette_Repel},
    [ITEM_SUPER_REPEL] = {sItemIcon_Repel, sItemIconPalette_SuperRepel},
    [ITEM_MAX_REPEL] = {sItemIcon_Repel, sItemIconPalette_MaxRepel},
    [ITEM_ESCAPE_ROPE] = {sItemIcon_EscapeRope, sItemIconPalette_EscapeRope},
    [ITEM_RED_NECTAR] = {sItemIcon_Nectar, sItemIconPalette_RedNectar},
    [ITEM_YELLOW_NECTAR] = {sItemIcon_Nectar, sItemIconPalette_YellowNectar},
    [ITEM_PINK_NECTAR] = {sItemIcon_Nectar, sItemIconPalette_PinkNectar},
    [ITEM_PURPLE_NECTAR] = {sItemIcon_Nectar, sItemIconPalette_PurpleNectar},
	[ITEM_EXPLORER_KIT] = {sItemIcon_ExplorerKit, sItemIconPalette_ExplorerKit},
	[ITEM_LUNAR_WING] = {sItemIcon_LunarWing, sItemIconPalette_LunarWing},
	[ITEM_ZYGARDE_CUBE] = {sItemIcon_ZygardeCube, sItemIconPalette_ZygardeCube},
	[ITEM_CATCHING_CHARM] = {sItemIcon_CatchingCharm, sItemIconPalette_CatchingCharm},
	[ITEM_EXP_CHARM] = {sItemIcon_ExpCharm, sItemIconPalette_ExpCharm},
	[ITEM_REINS_OF_UNITY] = {sItemIcon_ReinsOfUnity, sItemIconPalette_ReinsOfUnity},
	[ITEM_TERA_ORB] = {sItemIcon_TeraOrb, sItemIconPalette_TeraOrb},
	[ITEM_Z_RING] = {sItemIcon_ZRing, sItemIconPalette_ZRing},
    [ITEM_DYNAMAX_BAND] = {sItemIcon_DynamaxBand, sItemIconPalette_DynamaxBand},
	[ITEM_EXP_SHARE] = {sItemIcon_ExpShare, sItemIconPalette_ExpShare},
	[ITEM_REVEAL_GLASS] = {sItemIcon_RevealGlass, sItemIconPalette_RevealGlass},
    [ITEM_PRISON_BOTTLE] = {sItemIcon_PrisonBottle, sItemIconPalette_PrisonBottle},
	[ITEM_GRACIDEA] = {sItemIcon_Gracidea, sItemIconPalette_Gracidea},
	[ITEM_COIN_CASE] = {sItemIcon_CoinCase, sItemIconPalette_CoinCase},
    [ITEM_ITEMFINDER] = {sItemIcon_Itemfinder, sItemIconPalette_Itemfinder},
	[ITEM_OLD_ROD] = {sItemIcon_OldRod, sItemIconPalette_OldRod},
    [ITEM_GOOD_ROD] = {sItemIcon_GoodRod, sItemIconPalette_GoodRod},
    [ITEM_SUPER_ROD] = {sItemIcon_SuperRod, sItemIconPalette_SuperRod},
	[ITEM_SS_TICKET] = {sItemIcon_SSTicket, sItemIconPalette_SSTicket},
	[ITEM_N_SOLARIZER] = {sItemIcon_NSolarizer, sItemIconPalette_NSolarizer},
    [ITEM_N_LUNARIZER] = {sItemIcon_NSolarizer, sItemIconPalette_NLunarizer},
	[ITEM_RIDE_PAGER] = {sItemIcon_RidePager, sItemIconPalette_RidePager},
	[ITEM_MEGA_BRACELET] = {sItemIcon_MegaBracelet, sItemIconPalette_MegaBracelet},
	[ITEM_ROTOM_CATALOG] = {sItemIcon_RotomCatalog, sItemIconPalette_RotomCatalog},
	[ITEM_DNA_SPLICERS] = {sItemIcon_DnaSplicers, sItemIconPalette_DnaSplicers},
	[ITEM_METEORITE] = {sItemIcon_Meteorite, sItemIconPalette_Meteorite},
	[ITEM_OAKS_PARCEL] = {sItemIcon_OaksParcel, sItemIconPalette_OaksParcel},
    [ITEM_POKE_FLUTE] = {sItemIcon_PokeFlute, sItemIconPalette_PokeFlute},
    [ITEM_SECRET_KEY] = {sItemIcon_SecretKey, sItemIconPalette_SecretKey},
    [ITEM_BIKE_VOUCHER] = {sItemIcon_BikeVoucher, sItemIconPalette_BikeVoucher},
    [ITEM_GOLD_TEETH] = {sItemIcon_GoldTeeth, sItemIconPalette_GoldTeeth},
    [ITEM_CARD_KEY] = {sItemIcon_CardKey, sItemIconPalette_CardKey},
    [ITEM_LIFT_KEY] = {sItemIcon_LiftKey, sItemIconPalette_Key},
    [ITEM_SILPH_SCOPE] = {sItemIcon_SilphScope, sItemIconPalette_SilphScope},
    [ITEM_BICYCLE] = {sItemIcon_Bicycle, sItemIconPalette_Bicycle},
    [ITEM_TOWN_MAP] = {sItemIcon_TownMap, sItemIconPalette_TownMap},
    [ITEM_VS_SEEKER] = {sItemIcon_VSSeeker, sItemIconPalette_VSSeeker},
    [ITEM_FAME_CHECKER] = {sItemIcon_FameChecker, sItemIconPalette_FameChecker},
    [ITEM_TM_CASE] = {sItemIcon_TMCase, sItemIconPalette_TMCase},
    [ITEM_BERRY_POUCH] = {sItemIcon_BerryPouch, sItemIconPalette_BerryPouch},
    [ITEM_TEACHY_TV] = {sItemIcon_TeachyTV, sItemIconPalette_TeachyTV},
    [ITEM_TRI_PASS] = {sItemIcon_TriPass, sItemIconPalette_TriPass},
    [ITEM_RAINBOW_PASS] = {sItemIcon_RainbowPass, sItemIconPalette_RainbowPass},
    [ITEM_TEA] = {sItemIcon_Tea, sItemIconPalette_Tea},
    [ITEM_MYSTIC_TICKET] = {sItemIcon_MysticTicket, sItemIconPalette_MysticTicket},
    [ITEM_AURORA_TICKET] = {sItemIcon_AuroraTicket, sItemIconPalette_AuroraTicket},
    [ITEM_POWDER_JAR] = {sItemIcon_PowderJar, sItemIconPalette_PowderJar},
    [ITEM_RUBY] = {sItemIcon_Ruby, sItemIconPalette_Ruby},
    [ITEM_SAPPHIRE] = {sItemIcon_Ruby, sItemIconPalette_Sapphire},
	[ITEM_EV_IV_DISPLAY] = {sItemIcon_EvIvDisplay, sItemIconPalette_EvIvDisplay},
    [ITEM_SHINY_CHARM] = {sItemIcon_ShinyCharm, sItemIconPalette_ShinyCharm},
    [ITEM_RAINBOW_WING] = {sItemIcon_RainbowWing, sItemIconPalette_RainbowWing},
    [ITEM_SILVER_WING] = {sItemIcon_RainbowWing, sItemIconPalette_SilverWing},
	[ITEM_TM01] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM02] = {sItemIcon_TM, sItemIconPalette_DragonTMHM},
    [ITEM_TM03] = {sItemIcon_TM, sItemIconPalette_PsychicTMHM},
    [ITEM_TM04] = {sItemIcon_TM, sItemIconPalette_PsychicTMHM},
    [ITEM_TM05] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM06] = {sItemIcon_TM, sItemIconPalette_PoisonTMHM},
    [ITEM_TM07] = {sItemIcon_TM, sItemIconPalette_IceTMHM},
    [ITEM_TM08] = {sItemIcon_TM, sItemIconPalette_FightingTMHM},
    [ITEM_TM09] = {sItemIcon_TM, sItemIconPalette_PoisonTMHM},
    [ITEM_TM10] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM11] = {sItemIcon_TM, sItemIconPalette_FireTMHM},
    [ITEM_TM12] = {sItemIcon_TM, sItemIconPalette_DarkTMHM},
    [ITEM_TM13] = {sItemIcon_TM, sItemIconPalette_IceTMHM},
    [ITEM_TM14] = {sItemIcon_TM, sItemIconPalette_IceTMHM},
    [ITEM_TM15] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM16] = {sItemIcon_TM, sItemIconPalette_PsychicTMHM},
    [ITEM_TM17] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM18] = {sItemIcon_TM, sItemIconPalette_WaterTMHM},
    [ITEM_TM19] = {sItemIcon_TM, sItemIconPalette_FlyingTMHM},
    [ITEM_TM20] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM21] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM22] = {sItemIcon_TM, sItemIconPalette_GrassTMHM},
    [ITEM_TM23] = {sItemIcon_TM, sItemIconPalette_RockTMHM},
    [ITEM_TM24] = {sItemIcon_TM, sItemIconPalette_ElectricTMHM},
    [ITEM_TM25] = {sItemIcon_TM, sItemIconPalette_ElectricTMHM},
    [ITEM_TM26] = {sItemIcon_TM, sItemIconPalette_GroundTMHM},
    [ITEM_TM27] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM28] = {sItemIcon_TM, sItemIconPalette_BugTMHM},
    [ITEM_TM29] = {sItemIcon_TM, sItemIconPalette_PsychicTMHM},
    [ITEM_TM30] = {sItemIcon_TM, sItemIconPalette_GhostTMHM},
    [ITEM_TM31] = {sItemIcon_TM, sItemIconPalette_FightingTMHM},
    [ITEM_TM32] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM33] = {sItemIcon_TM, sItemIconPalette_PsychicTMHM},
    [ITEM_TM34] = {sItemIcon_TM, sItemIconPalette_PoisonTMHM},
    [ITEM_TM35] = {sItemIcon_TM, sItemIconPalette_FireTMHM},
    [ITEM_TM36] = {sItemIcon_TM, sItemIconPalette_PoisonTMHM},
    [ITEM_TM37] = {sItemIcon_TM, sItemIconPalette_RockTMHM},
    [ITEM_TM38] = {sItemIcon_TM, sItemIconPalette_FireTMHM},
    [ITEM_TM39] = {sItemIcon_TM, sItemIconPalette_RockTMHM},
    [ITEM_TM40] = {sItemIcon_TM, sItemIconPalette_FlyingTMHM},
    [ITEM_TM41] = {sItemIcon_TM, sItemIconPalette_DarkTMHM},
    [ITEM_TM42] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM43] = {sItemIcon_TM, sItemIconPalette_FireTMHM},
    [ITEM_TM44] = {sItemIcon_TM, sItemIconPalette_PsychicTMHM},
    [ITEM_TM45] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM46] = {sItemIcon_TM, sItemIconPalette_DarkTMHM},
    [ITEM_TM47] = {sItemIcon_TM, sItemIconPalette_FightingTMHM},
    [ITEM_TM48] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM49] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM50] = {sItemIcon_TM, sItemIconPalette_FireTMHM},
    [ITEM_TM51] = {sItemIcon_TM, sItemIconPalette_SteelTMHM},
    [ITEM_TM52] = {sItemIcon_TM, sItemIconPalette_FightingTMHM},
    [ITEM_TM53] = {sItemIcon_TM, sItemIconPalette_GrassTMHM},
    [ITEM_TM54] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM55] = {sItemIcon_TM, sItemIconPalette_WaterTMHM},
    [ITEM_TM56] = {sItemIcon_TM, sItemIconPalette_DarkTMHM},
    [ITEM_TM57] = {sItemIcon_TM, sItemIconPalette_ElectricTMHM},
    [ITEM_TM58] = {sItemIcon_TM, sItemIconPalette_FlyingTMHM},
    [ITEM_TM59] = {sItemIcon_TM, sItemIconPalette_DarkTMHM},
    [ITEM_TM60] = {sItemIcon_TM, sItemIconPalette_DarkTMHM},
    [ITEM_TM61] = {sItemIcon_TM, sItemIconPalette_FireTMHM},
    [ITEM_TM62] = {sItemIcon_TM, sItemIconPalette_FlyingTMHM},
    [ITEM_TM63] = {sItemIcon_TM, sItemIconPalette_DarkTMHM},
    [ITEM_TM64] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM65] = {sItemIcon_TM, sItemIconPalette_GhostTMHM},
    [ITEM_TM66] = {sItemIcon_TM, sItemIconPalette_DarkTMHM},
    [ITEM_TM67] = {sItemIcon_TM, sItemIconPalette_SteelTMHM},
    [ITEM_TM68] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM69] = {sItemIcon_TM, sItemIconPalette_RockTMHM},
    [ITEM_TM70] = {sItemIcon_TM, sItemIconPalette_IceTMHM},
    [ITEM_TM71] = {sItemIcon_TM, sItemIconPalette_RockTMHM},
    [ITEM_TM72] = {sItemIcon_TM, sItemIconPalette_ElectricTMHM},
    [ITEM_TM73] = {sItemIcon_TM, sItemIconPalette_ElectricTMHM},
    [ITEM_TM74] = {sItemIcon_TM, sItemIconPalette_SteelTMHM},
    [ITEM_TM75] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM76] = {sItemIcon_TM, sItemIconPalette_FlyingTMHM},
    [ITEM_TM77] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM78] = {sItemIcon_TM, sItemIconPalette_GroundTMHM},
    [ITEM_TM79] = {sItemIcon_TM, sItemIconPalette_IceTMHM},
    [ITEM_TM80] = {sItemIcon_TM, sItemIconPalette_RockTMHM},
    [ITEM_TM81] = {sItemIcon_TM, sItemIconPalette_BugTMHM},
    [ITEM_TM82] = {sItemIcon_TM, sItemIconPalette_DragonTMHM},
    [ITEM_TM83] = {sItemIcon_TM, sItemIconPalette_BugTMHM},
    [ITEM_TM84] = {sItemIcon_TM, sItemIconPalette_PoisonTMHM},
    [ITEM_TM85] = {sItemIcon_TM, sItemIconPalette_PsychicTMHM},
    [ITEM_TM86] = {sItemIcon_TM, sItemIconPalette_GrassTMHM},
    [ITEM_TM87] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM88] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM89] = {sItemIcon_TM, sItemIconPalette_BugTMHM},
    [ITEM_TM90] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM91] = {sItemIcon_TM, sItemIconPalette_SteelTMHM},
    [ITEM_TM92] = {sItemIcon_TM, sItemIconPalette_PsychicTMHM},
    [ITEM_TM93] = {sItemIcon_TM, sItemIconPalette_ElectricTMHM},
    [ITEM_TM94] = {sItemIcon_TM, sItemIconPalette_WaterTMHM},
    [ITEM_TM95] = {sItemIcon_TM, sItemIconPalette_DarkTMHM},
    [ITEM_TM96] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEM_TM97] = {sItemIcon_TM, sItemIconPalette_DarkTMHM},
    [ITEM_TM98] = {sItemIcon_TM, sItemIconPalette_WaterTMHM},
    [ITEM_TM99] = {sItemIcon_TM, sItemIconPalette_FairyTMHM},
    [ITEM_TM100] = {sItemIcon_TM, sItemIconPalette_NormalTMHM},
    [ITEMS_COUNT] = {sItemIcon_ReturnToFieldArrow, sItemIconPalette_ReturnToFieldArrow},
};

void ResetItemMenuIconState(void)
{
    u16 i;

    for (i = 0; i < NELEMS(sItemMenuIconSpriteIds); i++)
        sItemMenuIconSpriteIds[i] = 0xFF;
}

void CreateBagOrSatchelSprite(u8 animNum)
{
    sItemMenuIconSpriteIds[0] = CreateSprite(&sSpriteTemplate_BagOrSatchel, 40, 68, 0);
    SetBagVisualPocketId(animNum);
}

void SetBagVisualPocketId(u8 animNum)
{
    struct Sprite * sprite = &gSprites[sItemMenuIconSpriteIds[0]];
    sprite->y2 = -5;
    sprite->callback = SpriteCB_BagVisualSwitchingPockets;
    StartSpriteAnim(sprite, animNum);
}

static void SpriteCB_BagVisualSwitchingPockets(struct Sprite * sprite)
{
    if (sprite->y2 != 0)
        sprite->y2++;
    else
        sprite->callback = SpriteCallbackDummy;
}

void ShakeBagSprite(void)
{
    struct Sprite * sprite = &gSprites[sItemMenuIconSpriteIds[0]];
    if (sprite->affineAnimEnded)
    {
        StartSpriteAffineAnim(sprite, 1);
        sprite->callback = SpriteCB_ShakeBagSprite;
    }
}

static void SpriteCB_ShakeBagSprite(struct Sprite * sprite)
{
    if (sprite->affineAnimEnded)
    {
        StartSpriteAffineAnim(sprite, 0);
        sprite->callback = SpriteCallbackDummy;
    }
}

void ItemMenuIcons_CreateInsertIndicatorBarHidden(void)
{
    u8 i;
    u8 * ptr = &sItemMenuIconSpriteIds[1];

    for (i = 0; i < 9; i++)
    {
        ptr[i] = CreateSprite(&gUnknown_83D4250, i * 16 + 0x60, 7, 0);
        switch (i)
        {
        case 0:
            break;
        case 8:
            StartSpriteAnim(&gSprites[ptr[i]], 2);
            break;
        default:
            StartSpriteAnim(&gSprites[ptr[i]], 1);
            break;
        }
        gSprites[ptr[i]].invisible = TRUE;
    }
}

void ItemMenuIcons_ToggleInsertIndicatorBarVisibility(bool8 invisible)
{
    u8 i;
    u8 * ptr = &sItemMenuIconSpriteIds[1];

    for (i = 0; i < 9; i++)
    {
        gSprites[ptr[i]].invisible = invisible;
    }
}

void ItemMenuIcons_MoveInsertIndicatorBar(s16 x, u16 y)
{
    u8 i;
    u8 * ptr = &sItemMenuIconSpriteIds[1];

    for (i = 0; i < 9; i++)
    {
        gSprites[ptr[i]].x2 = x;
        gSprites[ptr[i]].y = y + 7;
    }
}

static bool8 TryAllocItemIconTilesBuffers(void)
{
    void ** ptr1, ** ptr2;

    ptr1 = &sItemIconTilesBuffer;
    *ptr1 = Alloc(0x120);
    if (*ptr1 == NULL)
        return FALSE;
    ptr2 = &sItemIconTilesBufferPadded;
    *ptr2 = AllocZeroed(0x200);
    if (*ptr2 == NULL)
    {
        Free(*ptr1);
        return FALSE;
    }
    return TRUE;
}

void CopyItemIconPicTo4x4Buffer(const void * src, void * dest)
{
    u8 i;

    for (i = 0; i < 3; i++)
    {
        CpuCopy16(src + 0x60 * i, dest + 0x80 * i, 0x60);
    }
}

u8 AddItemIconObject(u16 tilesTag, u16 paletteTag, u16 itemId)
{
    struct SpriteTemplate template;
    struct SpriteSheet spriteSheet;
    struct CompressedSpritePalette spritePalette;
    u8 spriteId;

    if (!TryAllocItemIconTilesBuffers())
        return MAX_SPRITES;

    LZDecompressWram(GetItemIconGfxPtr(itemId, 0), sItemIconTilesBuffer);
    CopyItemIconPicTo4x4Buffer(sItemIconTilesBuffer, sItemIconTilesBufferPadded);
    spriteSheet.data = sItemIconTilesBufferPadded;
    spriteSheet.size = 0x200;
    spriteSheet.tag = tilesTag;
    LoadSpriteSheet(&spriteSheet);

    spritePalette.data = GetItemIconGfxPtr(itemId, 1);
    spritePalette.tag = paletteTag;
    LoadCompressedSpritePalette(&spritePalette);

    CpuCopy16(&sSpriteTemplate_ItemIcon, &template, sizeof(struct SpriteTemplate));
    template.tileTag = tilesTag;
    template.paletteTag = paletteTag;
    spriteId = CreateSprite(&template, 0, 0, 0);

    Free(sItemIconTilesBuffer);
    Free(sItemIconTilesBufferPadded);
    return spriteId;
}

u8 AddItemIconObjectWithCustomObjectTemplate(const struct SpriteTemplate * origTemplate, u16 tilesTag, u16 paletteTag, u16 itemId)
{
    struct SpriteTemplate template;
    struct SpriteSheet spriteSheet;
    struct CompressedSpritePalette spritePalette;
    u8 spriteId;

    if (!TryAllocItemIconTilesBuffers())
        return MAX_SPRITES;

    LZDecompressWram(GetItemIconGfxPtr(itemId, 0), sItemIconTilesBuffer);
    CopyItemIconPicTo4x4Buffer(sItemIconTilesBuffer, sItemIconTilesBufferPadded);
    spriteSheet.data = sItemIconTilesBufferPadded;
    spriteSheet.size = 0x200;
    spriteSheet.tag = tilesTag;
    LoadSpriteSheet(&spriteSheet);

    spritePalette.data = GetItemIconGfxPtr(itemId, 1);
    spritePalette.tag = paletteTag;
    LoadCompressedSpritePalette(&spritePalette);

    CpuCopy16(origTemplate, &template, sizeof(struct SpriteTemplate));
    template.tileTag = tilesTag;
    template.paletteTag = paletteTag;
    spriteId = CreateSprite(&template, 0, 0, 0);

    Free(sItemIconTilesBuffer);
    Free(sItemIconTilesBufferPadded);
    return spriteId;
}

void CreateItemMenuIcon(u16 itemId, u8 idx)
{
    u8 * ptr = &sItemMenuIconSpriteIds[10];
    u8 spriteId;

    if (ptr[idx] == 0xFF)
    {
        FreeSpriteTilesByTag(ITEMICON_TAG + idx);
        FreeSpritePaletteByTag(ITEMICON_TAG + idx);
        spriteId = AddItemIconObject(ITEMICON_TAG + idx, ITEMICON_TAG + idx, itemId);
        if (spriteId != MAX_SPRITES)
        {
            ptr[idx] = spriteId;
            gSprites[spriteId].x2 = 24;
            gSprites[spriteId].y2 = 140;
        }
    }
}

void DestroyItemMenuIcon(u8 idx)
{
    u8 * ptr = &sItemMenuIconSpriteIds[10];

    if (ptr[idx] != 0xFF)
    {
        DestroySpriteAndFreeResources(&gSprites[ptr[idx]]);
        ptr[idx] = 0xFF;
    }
}

const void * GetItemIconGfxPtr(u16 itemId, u8 attrId)
{
    if (itemId > ITEMS_COUNT)
        itemId = ITEM_NONE;
    return sItemIconGfxPtrs[itemId][attrId];
}

void sub_80989A0(u16 itemId, u8 idx)
{
    u8 * ptr = &sItemMenuIconSpriteIds[10];
    u8 spriteId;

    if (ptr[idx] == 0xFF)
    {
        FreeSpriteTilesByTag(ITEMICON_TAG + idx);
        FreeSpritePaletteByTag(ITEMICON_TAG + idx);
        spriteId = AddItemIconObject(ITEMICON_TAG + idx, ITEMICON_TAG + idx, itemId);
        if (spriteId != MAX_SPRITES)
        {
            ptr[idx] = spriteId;
            gSprites[spriteId].x2 = 24;
            gSprites[spriteId].y2 = 147;
        }
    }
}

static u8 ReformatItemDescription(u16 item, u8* dest, u8 maxChars)
{
	u8 count = 0, numLines = 1, k = 0;
	u8 buffer[150];
	u8 *desc = (u8 *)ItemId_GetDescription(item);
	u8* lineStart;

	memset(dest, 0xFF, 500);

	lineStart = dest;
	
	StringExpandPlaceholders(buffer, desc);
	
	while (buffer[k] != EOS)
	{
		if (GetStringWidth(0, lineStart, 0) >= maxChars)
		{
			do
			{
				dest--;
				k--;
			} while (buffer[k] != CHAR_SPACE && buffer[k] != CHAR_NEWLINE);
			
			if (buffer[k + 1] != EOS)
			{
				*dest = CHAR_NEWLINE;
				numLines++;
			}
			count = 0;
			dest++;
			k++;
			lineStart = dest;
			continue;
		}
		*dest = buffer[k];
		
		if (buffer[k] == CHAR_NEWLINE)
		{
			if (buffer[k - 1] != CHAR_SPACE)
				*dest = CHAR_SPACE;
			else
				dest--;
		}
		dest++;
		k++;
		count++;
	}
	*dest = EOS;

	return numLines;
}

#define ITEM_ICON_X 26
#define ITEM_ICON_Y 24

#define IS_KEY_ITEM_TM(pocket) ((pocket == POCKET_KEY_ITEMS || pocket == POCKET_TM_CASE))

static u8 ShowObtainedItemDescription(u16 item)
{
	struct WindowTemplate template;
	s16 textX, textY, maxWidth, windowHeight, numLines;
	u8 buffer[500], windowId;
	
	if (IS_KEY_ITEM_TM(ItemId_GetPocket(item)))
	{
		textX = 1;
		maxWidth = 222;
	}
	else
	{
		textX = ITEM_ICON_X + 2;
		maxWidth = 195;
	}
	numLines = ReformatItemDescription(item, buffer, maxWidth);
	
	if (numLines == 1)
	{
		textY = 4;
		windowHeight = 3;
	}
	else if (numLines >= 3)
	{
		textY = 0;
		windowHeight = 5;
	}
	else
	{
		textY = 0;
		windowHeight = 4;
	}
	template = SetWindowTemplateFields(0, 1, 1, 28, windowHeight, 14, 0x20);
	windowId = AddWindow(&template);
	FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
	DrawStdFrameWithCustomTileAndPalette(windowId, FALSE, 0x214, 14);
	PutWindowTilemap(windowId);
	CopyWindowToVram(windowId, COPYWIN_BOTH);
	AddTextPrinterParameterized(windowId, 0, buffer, textX, textY, 0, NULL);
	GetSetItemObtained(item, FLAG_SET_OBTAINED);
	
	return windowId;
}

void CreateItemIconOnFindMessage(void)
{
	struct Sprite * sprite;
	u16 reg1 = GetGpuReg(REG_OFFSET_DISPCNT), reg2 = GetGpuReg(REG_OFFSET_WINOUT), itemId = gSpecialVar_0x8009;
	s16 x, y;
	u8 spriteId, spriteId2 = MAX_SPRITES, windowId = 0xFF;
	
	spriteId = AddItemIconObject(ITEMICON_TAG, ITEMICON_TAG, itemId);
	
	if (Overworld_GetFlashLevel())
	{
		SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJWIN_ON);
		SetGpuRegBits(REG_OFFSET_WINOUT, WINOUT_WINOBJ_OBJ);
		
		spriteId2 = AddItemIconObject(ITEMICON_TAG, ITEMICON_TAG, itemId);
	}
	if (spriteId != MAX_SPRITES)
	{
		sprite = &gSprites[spriteId];
		
		if (IS_KEY_ITEM_TM(ItemId_GetPocket(itemId)))
		{
			x = 112;
			y = 64;
			
			sprite->oam.affineMode = ST_OAM_AFFINE_DOUBLE;
			sprite->oam.matrixNum = AllocOamMatrix();
			sprite->affineAnims = sSpriteAffineAnimTable_KeyItemTM;
			
			StartSpriteAffineAnim(sprite, 0);
			
			if (!GetSetItemObtained(itemId, FLAG_GET_OBTAINED))
				windowId = ShowObtainedItemDescription(itemId);
		}
		else
		{
			if (GetSetItemObtained(itemId, FLAG_GET_OBTAINED))
			{
				x = 213;
				y = 140;
			}
			else
			{
				x = ITEM_ICON_X;
				y = ITEM_ICON_Y;
				
				windowId = ShowObtainedItemDescription(itemId);
			}
		}
		sprite->x2 = x;
		sprite->y2 = y;
		sprite->oam.priority = 0;
		sprite->data[0] = windowId;
		sprite->data[1] = reg1;
		sprite->data[2] = reg2;
		sprite->data[3] = spriteId2;
	}
	if (spriteId2 != MAX_SPRITES)
	{
		sprite = &gSprites[spriteId2];
		
		sprite->x2 = x;
		sprite->y2 = y;
		sprite->oam.priority = 0;
		sprite->oam.objMode = ST_OAM_OBJ_WINDOW;
	}
	gSpecialVar_0x8009 = spriteId; // save sprite id for use later
}

void DestroyItemIconOnFindMessage(void)
{
	u8 windowId, spriteId2, spriteId = gSpecialVar_0x8009;
	u16 reg1, reg2;
	struct Sprite * sprite = &gSprites[spriteId];
	
	windowId = sprite->data[0];
	reg1 = sprite->data[1];
	reg2 = sprite->data[2];
	spriteId2 = sprite->data[3];
	
	FreeSpriteTilesByTag(ITEMICON_TAG);
	FreeSpritePaletteByTag(ITEMICON_TAG);
	FreeSpriteOamMatrix(sprite);
	DestroySprite(sprite);
	
	if (spriteId2 != MAX_SPRITES)
	{
		SetGpuReg(REG_OFFSET_DISPCNT, reg1);
		SetGpuReg(REG_OFFSET_WINOUT, reg2);
		
		FreeSpriteTilesByTag(ITEMICON_TAG);
		FreeSpritePaletteByTag(ITEMICON_TAG);
		DestroySprite(&gSprites[spriteId2]);
	}
	if (windowId != 0xFF)
	{
		ClearDialogWindowAndFrame(windowId, TRUE);
		RemoveWindow(windowId);
	}
}

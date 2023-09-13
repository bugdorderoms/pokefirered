#define VITAMIN_FRIENDSHIP_CHANGE(id)     \
	[id] = ITEMEFFECT_CHANGE_FRIENDSHIP,  \
	[id + 1] = ITEMEFFECT_FRIENDSHIP_ALL, \
    [id + 2] = 5,                         \
    [id + 3] = 3,                         \
    [id + 4] = 2

#define STORE_EXPERIENCE_AMOUNT(amount) \
    [0] = ITEMEFFECT_GIVE_EXPERIENCE,   \
	[1] = (u8)amount,                   \
	[2] = (amount & 0xFF00) >> 8

static const u8 sItemEffect_HealHPAmount[] = {
	[0] = ITEMEFFECT_HEAL_HP,
	[1] = ITEMEFFECT_END
};

static const u8 sItemEffect_CurePoison[] = {
    [0] = ITEMEFFECT_CURE_STATUS,
	[1] = ITEMEFFECT_STATUS_POISON,
	[2] = ITEMEFFECT_END
};

static const u8 sItemEffect_CureBurn[] = {
    [0] = ITEMEFFECT_CURE_STATUS,
	[1] = ITEMEFFECT_STATUS_BURN,
	[2] = ITEMEFFECT_END
};

static const u8 sItemEffect_CureFreeze[] = {
    [0] = ITEMEFFECT_CURE_STATUS,
	[1] = ITEMEFFECT_STATUS_FREEZE,
	[2] = ITEMEFFECT_END
};

static const u8 sItemEffect_CureSleep[] = {
    [0] = ITEMEFFECT_CURE_STATUS,
	[1] = ITEMEFFECT_STATUS_SLEEP,
	[2] = ITEMEFFECT_END
};

static const u8 sItemEffect_CureParalyze[] = {
    [0] = ITEMEFFECT_CURE_STATUS,
	[1] = ITEMEFFECT_STATUS_PARALYSIS,
	[2] = ITEMEFFECT_END
};

static const u8 sItemEffect_HealHPCureAllStatus[] = {
    [0] = ITEMEFFECT_HEAL_HP,
	[1] = ITEMEFFECT_CURE_STATUS,
	[2] = ITEMEFFECT_STATUS_ALL,
	[3] = ITEMEFFECT_END
};

static const u8 sItemEffect_CureAllStatus[] = {
    [0] = ITEMEFFECT_CURE_STATUS,
	[1] = ITEMEFFECT_STATUS_ALL,
	[2] = ITEMEFFECT_END
};

static const u8 sItemEffect_Revive[] = {
    [0] = ITEMEFFECT_REVIVE,
	[1] = ITEMEFFECT_END
};

static const u8 sItemEffect_EnergyPowder[] = {
    [0] = ITEMEFFECT_HEAL_HP,
	[1] = ITEMEFFECT_CHANGE_FRIENDSHIP,
	[2] = ITEMEFFECT_FRIENDSHIP_ALL,
	[3] = -5,
	[4] = -5,
	[5] = -10,
	[6] = ITEMEFFECT_END
};

static const u8 sItemEffect_EnergyRoot[] = {
	[0] = ITEMEFFECT_HEAL_HP,
	[1] = ITEMEFFECT_CHANGE_FRIENDSHIP,
	[2] = ITEMEFFECT_FRIENDSHIP_ALL,
	[3] = -10,
	[4] = -10,
	[5] = -15,
	[6] = ITEMEFFECT_END
};

static const u8 sItemEffect_HealPowder[] = {
    [0] = ITEMEFFECT_CURE_STATUS,
	[1] = ITEMEFFECT_STATUS_ALL,
	[2] = ITEMEFFECT_CHANGE_FRIENDSHIP,
	[3] = ITEMEFFECT_FRIENDSHIP_ALL,
	[4] = -5,
	[5] = -5,
	[6] = -10,
	[7] = ITEMEFFECT_END
};

static const u8 sItemEffect_RevivalHerb[] = {
    [0] = ITEMEFFECT_REVIVE,
	[1] = ITEMEFFECT_CHANGE_FRIENDSHIP,
	[2] = ITEMEFFECT_FRIENDSHIP_ALL,
	[3] = -15,
	[4] = -15,
	[5] = -20,
    [6] = ITEMEFFECT_END
};

static const u8 sItemEffect_RestoreOneMovePP[] = {
	[0] = ITEMEFFECT_RESTORE_PP,
	[1] = FALSE,
	[2] = ITEMEFFECT_END
};

static const u8 sItemEffect_RestoreAllMovesPP[] = {
	[0] = ITEMEFFECT_RESTORE_PP,
	[1] = TRUE,
	[2] = ITEMEFFECT_END
};

static const u8 sItemEffect_CureConfusion[] = {
    [0] = ITEMEFFECT_CURE_STATUS,
	[1] = ITEMEFFECT_STATUS_CONFUSION,
	[2] = ITEMEFFECT_END
};

static const u8 sItemEffect_CureInfatuation[] = {
    [0] = ITEMEFFECT_CURE_STATUS,
	[1] = ITEMEFFECT_STATUS_LOVE,
	[2] = ITEMEFFECT_END
};

static const u8 sItemEffect_IncreaseDynamaxLevel[] = {
    [0] = ITEMEFFECT_UP_DYNAMAX_LEVEL,
    [1] = ITEMEFFECT_END,
};

static const u8 sItemEffect_Vitamins[] = {
    [0] = ITEMEFFECT_CHANGE_EV,
	[1] = 10,
    VITAMIN_FRIENDSHIP_CHANGE(2),
	[7] = ITEMEFFECT_END,
};

static const u8 sItemEffect_RareCandy[] = {
	STORE_EXPERIENCE_AMOUNT(0), // level up
	VITAMIN_FRIENDSHIP_CHANGE(3),
	[8] = ITEMEFFECT_END,
};

static const u8 sItemEffect_PPUp[] = {
	[0] = ITEMEFFECT_INCREASE_PP,
	[1] = FALSE,
	VITAMIN_FRIENDSHIP_CHANGE(2),
	[7] = ITEMEFFECT_END,
};

static const u8 sItemEffect_PPMax[] = {
    [0] = ITEMEFFECT_INCREASE_PP,
	[1] = TRUE,
	VITAMIN_FRIENDSHIP_CHANGE(2),
	[7] = ITEMEFFECT_END,
};

static const u8 sItemEffect_GiveGMaxFactor[] = {
    [0] = ITEMEFFECT_GIVE_GMAX_FACTOR,
	[1] = ITEMEFFECT_END,
};

static const u8 sItemEffect_EvolutionItem[] = {
	[0] = ITEMEFFECT_EVOLUTION,
	[1] = ITEMEFFECT_END,
};

static const u8 sItemEffect_Wings[] = {
	[0] = ITEMEFFECT_CHANGE_EV,
	[1] = 1,
	[2] = ITEMEFFECT_CHANGE_FRIENDSHIP,
	[3] = ITEMEFFECT_FRIENDSHIP_ALL,
	[4] = 3,
	[5] = 2,
	[6] = 1,
	[7] = ITEMEFFECT_END,
};

static const u8 sItemEffect_ExpCandyXS[] = {
	STORE_EXPERIENCE_AMOUNT(100),
	[3] = ITEMEFFECT_END,
};

static const u8 sItemEffect_ExpCandyS[] = {
	STORE_EXPERIENCE_AMOUNT(800),
	[3] = ITEMEFFECT_END,
};

static const u8 sItemEffect_ExpCandyM[] = {
	STORE_EXPERIENCE_AMOUNT(3000),
	[3] = ITEMEFFECT_END,
};

static const u8 sItemEffect_ExpCandyL[] = {
	STORE_EXPERIENCE_AMOUNT(10000),
	[3] = ITEMEFFECT_END,
};

static const u8 sItemEffect_ExpCandyXL[] = {
	STORE_EXPERIENCE_AMOUNT(30000),
	[3] = ITEMEFFECT_END,
};

static const u8 sItemEffect_SetTeraType[] = {
	[0] = ITEMEFFECT_SET_TERA_TYPE,
	[1] = ITEMEFFECT_END,
};

static const u8 sItemEffect_HealHPPercent[] = {
    [0] = ITEMEFFECT_HEAL_HP_PERCENT,
	[1] = ITEMEFFECT_END,
};

static const u8 sItemEffect_FriendshipBerry[] = {
    [0] = ITEMEFFECT_CHANGE_FRIENDSHIP,
	[1] = ITEMEFFECT_FRIENDSHIP_ALL | ITEMEFFECT_FRIENDSHIP_MAIN,
	[2] = 10,
	[3] = 5,
	[4] = 2,
	[5] = ITEMEFFECT_CHANGE_EV,
	[6] = -10,
	[7] = ITEMEFFECT_END,
};

const u8 *const gItemEffectTable[] =
{
	[ITEM_POTION - ITEM_POTION] = sItemEffect_HealHPAmount,
    [ITEM_ANTIDOTE - ITEM_POTION] = sItemEffect_CurePoison,
    [ITEM_BURN_HEAL - ITEM_POTION] = sItemEffect_CureBurn,
    [ITEM_ICE_HEAL - ITEM_POTION] = sItemEffect_CureFreeze,
    [ITEM_AWAKENING - ITEM_POTION] = sItemEffect_CureSleep,
    [ITEM_PARALYZE_HEAL - ITEM_POTION] = sItemEffect_CureParalyze,
    [ITEM_FULL_RESTORE - ITEM_POTION] = sItemEffect_HealHPCureAllStatus,
    [ITEM_MAX_POTION - ITEM_POTION] = sItemEffect_HealHPAmount,
    [ITEM_HYPER_POTION - ITEM_POTION] = sItemEffect_HealHPAmount,
    [ITEM_SUPER_POTION - ITEM_POTION] = sItemEffect_HealHPAmount,
    [ITEM_FULL_HEAL - ITEM_POTION] = sItemEffect_CureAllStatus,
    [ITEM_REVIVE - ITEM_POTION] = sItemEffect_Revive,
    [ITEM_MAX_REVIVE - ITEM_POTION] = sItemEffect_Revive,
    [ITEM_FRESH_WATER - ITEM_POTION] = sItemEffect_HealHPAmount,
    [ITEM_SODA_POP - ITEM_POTION] = sItemEffect_HealHPAmount,
    [ITEM_LEMONADE - ITEM_POTION] = sItemEffect_HealHPAmount,
    [ITEM_MOOMOO_MILK - ITEM_POTION] = sItemEffect_HealHPAmount,
    [ITEM_ENERGY_POWDER - ITEM_POTION] = sItemEffect_EnergyPowder,
    [ITEM_ENERGY_ROOT - ITEM_POTION] = sItemEffect_EnergyRoot,
    [ITEM_HEAL_POWDER - ITEM_POTION] = sItemEffect_HealPowder,
    [ITEM_REVIVAL_HERB - ITEM_POTION] = sItemEffect_RevivalHerb,
    [ITEM_ETHER - ITEM_POTION] = sItemEffect_RestoreOneMovePP,
    [ITEM_MAX_ETHER - ITEM_POTION] = sItemEffect_RestoreOneMovePP,
    [ITEM_ELIXIR - ITEM_POTION] = sItemEffect_RestoreAllMovesPP,
    [ITEM_MAX_ELIXIR - ITEM_POTION] = sItemEffect_RestoreAllMovesPP,
    [ITEM_LAVA_COOKIE - ITEM_POTION] = sItemEffect_CureAllStatus,
    [ITEM_BLUE_FLUTE - ITEM_POTION] = sItemEffect_CureSleep,
    [ITEM_YELLOW_FLUTE - ITEM_POTION] = sItemEffect_CureConfusion,
    [ITEM_RED_FLUTE - ITEM_POTION] = sItemEffect_CureInfatuation,
    [ITEM_BERRY_JUICE - ITEM_POTION] = sItemEffect_HealHPAmount,
    [ITEM_SACRED_ASH - ITEM_POTION] = sItemEffect_Revive,
	[ITEM_SWEET_HEART - ITEM_POTION] = sItemEffect_HealHPAmount,
	[ITEM_BIG_MALASADA - ITEM_POTION] = sItemEffect_CureAllStatus,
	[ITEM_CASTELIACONE - ITEM_POTION] = sItemEffect_CureAllStatus,
	[ITEM_LUMIOSE_GALETTE - ITEM_POTION] = sItemEffect_CureAllStatus,
	[ITEM_RAGE_CANDY_BAR - ITEM_POTION] = sItemEffect_CureAllStatus,
	[ITEM_SHALOUR_SABLE - ITEM_POTION] = sItemEffect_CureAllStatus,
	[ITEM_OLD_GATEAU - ITEM_POTION] = sItemEffect_CureAllStatus,
	[ITEM_DYNAMAX_CANDY - ITEM_POTION] = sItemEffect_IncreaseDynamaxLevel,
	[ITEM_PEWTER_CRUNCHIES - ITEM_POTION] = sItemEffect_CureAllStatus,
    [ITEM_HP_UP - ITEM_POTION] = sItemEffect_Vitamins,
    [ITEM_PROTEIN - ITEM_POTION] = sItemEffect_Vitamins,
    [ITEM_IRON - ITEM_POTION] = sItemEffect_Vitamins,
    [ITEM_CARBOS - ITEM_POTION] = sItemEffect_Vitamins,
    [ITEM_CALCIUM - ITEM_POTION] = sItemEffect_Vitamins,
    [ITEM_RARE_CANDY - ITEM_POTION] = sItemEffect_RareCandy,
    [ITEM_PP_UP - ITEM_POTION] = sItemEffect_PPUp,
    [ITEM_ZINC - ITEM_POTION] = sItemEffect_Vitamins,
    [ITEM_PP_MAX - ITEM_POTION] = sItemEffect_PPMax,
	[ITEM_MAX_MUSHROOMS - ITEM_POTION] = sItemEffect_GiveGMaxFactor,
    [ITEM_SUN_STONE - ITEM_POTION] = sItemEffect_EvolutionItem,
    [ITEM_MOON_STONE - ITEM_POTION] = sItemEffect_EvolutionItem,
    [ITEM_FIRE_STONE - ITEM_POTION] = sItemEffect_EvolutionItem,
    [ITEM_THUNDER_STONE - ITEM_POTION] = sItemEffect_EvolutionItem,
    [ITEM_WATER_STONE - ITEM_POTION] = sItemEffect_EvolutionItem,
    [ITEM_LEAF_STONE - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_ICE_STONE - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_DUSK_STONE - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_DAWN_STONE - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_SHINY_STONE - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_PROTECTOR - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_MAGMARIZER - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_SACHET - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_WHIPPED_DREAM - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_REAPER_CLOTH - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_DUBIOUS_DISC - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_ELECTIRIZER - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_CRACKED_POT - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_SWEET_APPLE - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_TART_APPLE - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_BLACK_AUGURITE - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_GALARICA_CUFF - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_GALARICA_WREATH - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_LINKING_CORD - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_SCROLL_OF_DARKNESS - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_SCROLL_OF_WATERS - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_AUSPICIOUS_ARMOR - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_MALICIOUS_ARMOR - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_KINGS_ROCK - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_METAL_COAT - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_DRAGON_SCALE - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_UP_GRADE - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_DEEP_SEA_TOOTH - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_DEEP_SEA_SCALE - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_PRISM_SCALE - ITEM_POTION] = sItemEffect_EvolutionItem,
	[ITEM_HEALTH_WING - ITEM_POTION] = sItemEffect_Wings,
	[ITEM_MUSCLE_WING - ITEM_POTION] = sItemEffect_Wings,
	[ITEM_RESIST_WING - ITEM_POTION] = sItemEffect_Wings,
	[ITEM_GENIUS_WING - ITEM_POTION] = sItemEffect_Wings,
	[ITEM_CLEVER_WING - ITEM_POTION] = sItemEffect_Wings,
	[ITEM_SWIFT_WING - ITEM_POTION] = sItemEffect_Wings,
	[ITEM_EXP_CANDY_XS - ITEM_POTION] = sItemEffect_ExpCandyXS,
	[ITEM_EXP_CANDY_S - ITEM_POTION] = sItemEffect_ExpCandyS,
	[ITEM_EXP_CANDY_M - ITEM_POTION] = sItemEffect_ExpCandyM,
	[ITEM_EXP_CANDY_L - ITEM_POTION] = sItemEffect_ExpCandyL,
	[ITEM_EXP_CANDY_XL - ITEM_POTION] = sItemEffect_ExpCandyXL,
	[ITEM_NORMAL_TERA_SHARD - ITEM_POTION] = sItemEffect_SetTeraType,
	[ITEM_FIRE_TERA_SHARD - ITEM_POTION] = sItemEffect_SetTeraType,
	[ITEM_WATER_TERA_SHARD - ITEM_POTION] = sItemEffect_SetTeraType,
	[ITEM_ELECTRIC_TERA_SHARD - ITEM_POTION] = sItemEffect_SetTeraType,
	[ITEM_GRASS_TERA_SHARD - ITEM_POTION] = sItemEffect_SetTeraType,
	[ITEM_ICE_TERA_SHARD - ITEM_POTION] = sItemEffect_SetTeraType,
	[ITEM_FIGHTING_TERA_SHARD - ITEM_POTION] = sItemEffect_SetTeraType,
	[ITEM_POISON_TERA_SHARD - ITEM_POTION] = sItemEffect_SetTeraType,
	[ITEM_GROUND_TERA_SHARD - ITEM_POTION] = sItemEffect_SetTeraType,
	[ITEM_FLYING_TERA_SHARD - ITEM_POTION] = sItemEffect_SetTeraType,
	[ITEM_PSYCHIC_TERA_SHARD - ITEM_POTION] = sItemEffect_SetTeraType,
	[ITEM_BUG_TERA_SHARD - ITEM_POTION] = sItemEffect_SetTeraType,
	[ITEM_ROCK_TERA_SHARD - ITEM_POTION] = sItemEffect_SetTeraType,
	[ITEM_GHOST_TERA_SHARD - ITEM_POTION] = sItemEffect_SetTeraType,
	[ITEM_DRAGON_TERA_SHARD - ITEM_POTION] = sItemEffect_SetTeraType,
	[ITEM_DARK_TERA_SHARD - ITEM_POTION] = sItemEffect_SetTeraType,
	[ITEM_STEEL_TERA_SHARD - ITEM_POTION] = sItemEffect_SetTeraType,
	[ITEM_FAIRY_TERA_SHARD - ITEM_POTION] = sItemEffect_SetTeraType,
	[ITEM_CHERI_BERRY - ITEM_POTION] = sItemEffect_CureParalyze,
	[ITEM_CHESTO_BERRY - ITEM_POTION] = sItemEffect_CureSleep,
	[ITEM_PECHA_BERRY - ITEM_POTION] = sItemEffect_CurePoison,
	[ITEM_RAWST_BERRY - ITEM_POTION] = sItemEffect_CureBurn,
	[ITEM_ASPEAR_BERRY - ITEM_POTION] = sItemEffect_CureFreeze,
	[ITEM_LEPPA_BERRY - ITEM_POTION] = sItemEffect_RestoreOneMovePP,
	[ITEM_ORAN_BERRY - ITEM_POTION] = sItemEffect_HealHPAmount,
    [ITEM_PERSIM_BERRY - ITEM_POTION] = sItemEffect_CureConfusion,
	[ITEM_LUM_BERRY - ITEM_POTION] = sItemEffect_CureAllStatus,
    [ITEM_SITRUS_BERRY - ITEM_POTION] = sItemEffect_HealHPPercent,
	[ITEM_POMEG_BERRY - ITEM_POTION] = sItemEffect_FriendshipBerry,
	[ITEM_KELPSY_BERRY - ITEM_POTION] = sItemEffect_FriendshipBerry,
	[ITEM_QUALOT_BERRY - ITEM_POTION] = sItemEffect_FriendshipBerry,
	[ITEM_HONDEW_BERRY - ITEM_POTION] = sItemEffect_FriendshipBerry,
	[ITEM_GREPA_BERRY - ITEM_POTION] = sItemEffect_FriendshipBerry,
	[ITEM_TAMATO_BERRY - ITEM_POTION] = sItemEffect_FriendshipBerry,
	[ITEM_HOPO_BERRY - ITEM_POTION] = sItemEffect_RestoreOneMovePP,
};

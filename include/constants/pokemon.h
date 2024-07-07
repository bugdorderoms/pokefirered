#ifndef GUARD_CONSTANTS_POKEMON_H
#define GUARD_CONSTANTS_POKEMON_H

// For (Set|Get)(Box)?MonData
#define MON_DATA_PERSONALITY        0
#define MON_DATA_OT_ID              1
#define MON_DATA_NICKNAME           2
#define MON_DATA_LANGUAGE           3
#define MON_DATA_SANITY_HAS_SPECIES 4
#define MON_DATA_IS_EGG             5
#define MON_DATA_OT_NAME            6
#define MON_DATA_MARKINGS           7
#define MON_DATA_SPECIES            8
#define MON_DATA_HELD_ITEM          9
#define MON_DATA_MOVE1              10
#define MON_DATA_MOVE2              11
#define MON_DATA_MOVE3              12
#define MON_DATA_MOVE4              13
#define MON_DATA_PP1                14
#define MON_DATA_PP2                15
#define MON_DATA_PP3                16
#define MON_DATA_PP4                17
#define MON_DATA_PP_BONUSES         18
#define MON_DATA_EXP                19
#define MON_DATA_HP_EV              20
#define MON_DATA_ATK_EV             21
#define MON_DATA_DEF_EV             22
#define MON_DATA_SPEED_EV           23
#define MON_DATA_SPATK_EV           24
#define MON_DATA_SPDEF_EV           25
#define MON_DATA_FRIENDSHIP         26
#define MON_DATA_POKERUS            27
#define MON_DATA_MET_LOCATION       28
#define MON_DATA_MET_LEVEL          29
#define MON_DATA_MET_GAME           30
#define MON_DATA_POKEBALL           31
#define MON_DATA_HP_IV              32
#define MON_DATA_ATK_IV             33
#define MON_DATA_DEF_IV             34
#define MON_DATA_SPEED_IV           35
#define MON_DATA_SPATK_IV           36
#define MON_DATA_SPDEF_IV           37
#define MON_DATA_ABILITY_NUM        38
#define MON_DATA_OT_GENDER          39
#define MON_DATA_MAIL               40
#define MON_DATA_SPECIES2           41
#define MON_DATA_IVS                42
#define MON_DATA_KNOWN_MOVES        43
#define MON_DATA_IS_SHINY           44
#define MON_DATA_ABILITY_HIDDEN     45
#define MON_DATA_NATURE             46
#define MON_DATA_DYNAMAX_LEVEL      47
#define MON_DATA_HAS_GMAX_FACTOR    48
#define MON_DATA_TERA_TYPE          49
#define MON_DATA_STATUS             50
#define MON_DATA_LEVEL              51
#define MON_DATA_MAX_HP             52
#define MON_DATA_HP                 53
#define MON_DATA_ATK                54
#define MON_DATA_DEF                55
#define MON_DATA_SPEED              56
#define MON_DATA_SPATK              57
#define MON_DATA_SPDEF              58
#define MON_DATA_EVOLUTION_TRACKER  59

#define SPLIT_PHYSICAL               0x0
#define SPLIT_SPECIAL                0x1
#define SPLIT_STATUS                 0x2
#define NUM_MOVE_SPLITS              0x3

// Pokemon types
#define TYPE_NORMAL         0x00
#define TYPE_FIGHTING       0x01
#define TYPE_FLYING         0x02
#define TYPE_POISON         0x03
#define TYPE_GROUND         0x04
#define TYPE_ROCK           0x05
#define TYPE_BUG            0x06
#define TYPE_GHOST          0x07
#define TYPE_STEEL          0x08
#define TYPE_MYSTERY        0x09
#define TYPE_FIRE           0x0a
#define TYPE_WATER          0x0b
#define TYPE_GRASS          0x0c
#define TYPE_ELECTRIC       0x0d
#define TYPE_PSYCHIC        0x0e
#define TYPE_ICE            0x0f
#define TYPE_DRAGON         0x10
#define TYPE_DARK           0x11
#define TYPE_FAIRY          0x12
#define TYPE_STELLAR        0x13
#define NUMBER_OF_MON_TYPES 0x14

// Pokemon egg groups
#define EGG_GROUP_NONE 0
#define EGG_GROUP_MONSTER 1
#define EGG_GROUP_WATER_1 2
#define EGG_GROUP_BUG 3
#define EGG_GROUP_FLYING 4
#define EGG_GROUP_FIELD 5
#define EGG_GROUP_FAIRY 6
#define EGG_GROUP_GRASS 7
#define EGG_GROUP_HUMAN_LIKE 8
#define EGG_GROUP_WATER_3 9
#define EGG_GROUP_MINERAL 10
#define EGG_GROUP_AMORPHOUS 11
#define EGG_GROUP_WATER_2 12
#define EGG_GROUP_DITTO 13
#define EGG_GROUP_DRAGON 14
#define EGG_GROUP_UNDISCOVERED 15

#define EGG_GROUPS_PER_MON      2

// Pokemon natures
#define NATURE_HARDY 0
#define NATURE_LONELY 1
#define NATURE_BRAVE 2
#define NATURE_ADAMANT 3
#define NATURE_NAUGHTY 4
#define NATURE_BOLD 5
#define NATURE_DOCILE 6
#define NATURE_RELAXED 7
#define NATURE_IMPISH 8
#define NATURE_LAX 9
#define NATURE_TIMID 10
#define NATURE_HASTY 11
#define NATURE_SERIOUS 12
#define NATURE_JOLLY 13
#define NATURE_NAIVE 14
#define NATURE_MODEST 15
#define NATURE_MILD 16
#define NATURE_QUIET 17
#define NATURE_BASHFUL 18
#define NATURE_RASH 19
#define NATURE_CALM 20
#define NATURE_GENTLE 21
#define NATURE_SASSY 22
#define NATURE_CAREFUL 23
#define NATURE_QUIRKY 24
#define NUM_NATURES 25

// Pokemon Stats
#define STAT_HP 0
#define STAT_ATK 1
#define STAT_DEF 2
#define STAT_SPEED 3
#define STAT_SPATK 4
#define STAT_SPDEF 5
#define STAT_ACC 6 // only in battles
#define STAT_EVASION 7 // only in battles

#define NUM_STATS 6
#define NUM_BATTLE_STATS 8

#define DEFAULT_STAT_STAGES 6
#define MIN_STAT_STAGES     0
#define MAX_STAT_STAGES     12

// Species flags
#define SPECIES_FLAG_LEGENDARY    (1 << 0)
#define SPECIES_FLAG_MYTHICAL     (1 << 1)
#define SPECIES_FLAG_ULTRA_BEAST  (1 << 2)
#define SPECIES_FLAG_MEGA         (1 << 3)
#define SPECIES_FLAG_PRIMAL       (1 << 4)
#define SPECIES_FLAG_ULTRA_BURST  (1 << 5)
#define SPECIES_FLAG_GIGANTAMAX   (1 << 6)
#define SPECIES_FLAG_PARADOX      (1 << 7)
#define SPECIES_FLAG_TERASTAL     (1 << 8)
#define SPECIES_FLAG_CANT_TRADE   (1 << 9)
// Those species flags cause the pokemon to be created with 3 perfect ivs
#define SPECIES_PERFECT_IVS_FLAGS (SPECIES_FLAG_LEGENDARY | SPECIES_FLAG_MYTHICAL | SPECIES_FLAG_ULTRA_BEAST | SPECIES_FLAG_PARADOX)

// Shiny odds
#define SHINY_ODDS 8 // Actual probability is SHINY_ODDS/65536

// Learning moves
#define MON_DONT_FIND_MOVE_TO_LEARN 0
#define MON_HAS_MAX_MOVES           1
#define MON_ALREADY_KNOWS_MOVE      2
#define MON_LEARNED_MOVE            3

#define MAX_LV_UP_MOVES              30

#define PLAYER_HAS_TWO_USABLE_MONS              0x0
#define PLAYER_HAS_ONE_MON                      0x1
#define PLAYER_HAS_ONE_USABLE_MON               0x2

#define MAX_LEVEL 100

#define OT_ID_PLAYER_ID 0
#define OT_ID_RANDOM    1
#define OT_ID_PRESET    2

#define MON_GIVEN_TO_PARTY      0x0
#define MON_GIVEN_TO_PC         0x1
#define MON_CANT_GIVE           0x2

#define MON_MALE       0x00
#define MON_FEMALE     0xFE
#define MON_GENDERLESS 0xFF

#define FRIENDSHIP_EVENT_GROW_LEVEL           0
#define FRIENDSHIP_EVENT_VITAMIN              1
#define FRIENDSHIP_EVENT_BATTLE_ITEM          2
#define FRIENDSHIP_EVENT_LEAGUE_BATTLE        3
#define FRIENDSHIP_EVENT_LEARN_TMHM           4
#define FRIENDSHIP_EVENT_WALKING              5
#define FRIENDSHIP_EVENT_MASSAGE              6
#define FRIENDSHIP_EVENT_FAINT_SMALL          7
#define FRIENDSHIP_EVENT_FAINT_OUTSIDE_BATTLE 8
#define FRIENDSHIP_EVENT_FAINT_LARGE          9

#define MAX_PER_STAT_IVS 31
#define USE_RANDOM_IVS (MAX_PER_STAT_IVS + 1)
#define MAX_PER_STAT_EVS 252
#define MAX_PER_VITAMIN_EVS MAX_PER_STAT_EVS
#define MAX_TOTAL_EVS 504
#define MAX_DYNAMAX_LEVEL 10 // can suport up to 15 level

#define PARTY_SIZE 6
#define BOX_NAME_LENGTH 8

#endif // GUARD_CONSTANTS_POKEMON_H

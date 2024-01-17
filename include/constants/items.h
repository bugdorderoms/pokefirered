#ifndef GUARD_CONSTANTS_ITEMS_H
#define GUARD_CONSTANTS_ITEMS_H

#define ITEM_NONE                     0
// Poke balls
#define ITEM_MASTER_BALL              1
#define ITEM_ULTRA_BALL               2
#define ITEM_GREAT_BALL               3
#define ITEM_POKE_BALL                4
#define ITEM_SAFARI_BALL              5
#define ITEM_NET_BALL                 6
#define ITEM_DIVE_BALL                7
#define ITEM_NEST_BALL                8
#define ITEM_REPEAT_BALL              9
#define ITEM_TIMER_BALL               10
#define ITEM_LUXURY_BALL              11
#define ITEM_PREMIER_BALL             12
#define ITEM_PARK_BALL                13
#define ITEM_CHERISH_BALL             14
#define ITEM_DUSK_BALL                15
#define ITEM_HEAL_BALL                16
#define ITEM_QUICK_BALL               17
#define ITEM_FAST_BALL                18
#define ITEM_LEVEL_BALL               19
#define ITEM_LURE_BALL                20
#define ITEM_HEAVY_BALL               21
#define ITEM_LOVE_BALL                22
#define ITEM_FRIEND_BALL              23
#define ITEM_MOON_BALL                24
#define ITEM_SPORT_BALL               25
#define ITEM_BEAST_BALL               26
#define ITEM_DREAM_BALL               27

// In case you want to add new poke ball items, all the indexes below this will be updated with the corrected index
#define POKE_BALL_ITEMS_END           ITEM_DREAM_BALL

// Medicine
#define ITEM_POTION                   (POKE_BALL_ITEMS_END + 1)
#define ITEM_ANTIDOTE                 (POKE_BALL_ITEMS_END + 2)
#define ITEM_BURN_HEAL                (POKE_BALL_ITEMS_END + 3)
#define ITEM_ICE_HEAL                 (POKE_BALL_ITEMS_END + 4)
#define ITEM_AWAKENING                (POKE_BALL_ITEMS_END + 5)
#define ITEM_PARALYZE_HEAL            (POKE_BALL_ITEMS_END + 6)
#define ITEM_FULL_RESTORE             (POKE_BALL_ITEMS_END + 7)
#define ITEM_MAX_POTION               (POKE_BALL_ITEMS_END + 8)
#define ITEM_HYPER_POTION             (POKE_BALL_ITEMS_END + 9)
#define ITEM_SUPER_POTION             (POKE_BALL_ITEMS_END + 10)
#define ITEM_FULL_HEAL                (POKE_BALL_ITEMS_END + 11)
#define ITEM_REVIVE                   (POKE_BALL_ITEMS_END + 12)
#define ITEM_MAX_REVIVE               (POKE_BALL_ITEMS_END + 13)
#define ITEM_FRESH_WATER              (POKE_BALL_ITEMS_END + 14)
#define ITEM_SODA_POP                 (POKE_BALL_ITEMS_END + 15)
#define ITEM_LEMONADE                 (POKE_BALL_ITEMS_END + 16)
#define ITEM_MOOMOO_MILK              (POKE_BALL_ITEMS_END + 17)
#define ITEM_ENERGY_POWDER            (POKE_BALL_ITEMS_END + 18)
#define ITEM_ENERGY_ROOT              (POKE_BALL_ITEMS_END + 19)
#define ITEM_HEAL_POWDER              (POKE_BALL_ITEMS_END + 20)
#define ITEM_REVIVAL_HERB             (POKE_BALL_ITEMS_END + 21)
#define ITEM_ETHER                    (POKE_BALL_ITEMS_END + 22)
#define ITEM_MAX_ETHER                (POKE_BALL_ITEMS_END + 23)
#define ITEM_ELIXIR                   (POKE_BALL_ITEMS_END + 24)
#define ITEM_MAX_ELIXIR               (POKE_BALL_ITEMS_END + 25)
#define ITEM_LAVA_COOKIE              (POKE_BALL_ITEMS_END + 26)
#define ITEM_BLUE_FLUTE               (POKE_BALL_ITEMS_END + 27)
#define ITEM_YELLOW_FLUTE             (POKE_BALL_ITEMS_END + 28)
#define ITEM_RED_FLUTE                (POKE_BALL_ITEMS_END + 29)
#define ITEM_BLACK_FLUTE              (POKE_BALL_ITEMS_END + 30)
#define ITEM_WHITE_FLUTE              (POKE_BALL_ITEMS_END + 31)
#define ITEM_BERRY_JUICE              (POKE_BALL_ITEMS_END + 32)
#define ITEM_SACRED_ASH               (POKE_BALL_ITEMS_END + 33)
#define ITEM_SWEET_HEART              (POKE_BALL_ITEMS_END + 34)
#define ITEM_BIG_MALASADA             (POKE_BALL_ITEMS_END + 35)
#define ITEM_CASTELIACONE             (POKE_BALL_ITEMS_END + 36)
#define ITEM_LUMIOSE_GALETTE          (POKE_BALL_ITEMS_END + 37)
#define ITEM_RAGE_CANDY_BAR           (POKE_BALL_ITEMS_END + 38)
#define ITEM_SHALOUR_SABLE            (POKE_BALL_ITEMS_END + 39)
#define ITEM_OLD_GATEAU               (POKE_BALL_ITEMS_END + 40)
#define ITEM_DYNAMAX_CANDY            (POKE_BALL_ITEMS_END + 41)
#define ITEM_ABILITY_CAPSULE          (POKE_BALL_ITEMS_END + 42)
#define ITEM_ABILITY_PATCH            (POKE_BALL_ITEMS_END + 43)
#define ITEM_PEWTER_CRUNCHIES         (POKE_BALL_ITEMS_END + 44)

// In case you want to add new medicine items, all the indexes below this will be updated with the corrected index
#define MEDICINE_ITEMS_END            ITEM_PEWTER_CRUNCHIES

// Protein
#define ITEM_HP_UP                    (MEDICINE_ITEMS_END + 1)
#define ITEM_PROTEIN                  (MEDICINE_ITEMS_END + 2)
#define ITEM_IRON                     (MEDICINE_ITEMS_END + 3)
#define ITEM_CARBOS                   (MEDICINE_ITEMS_END + 4)
#define ITEM_CALCIUM                  (MEDICINE_ITEMS_END + 5)
#define ITEM_RARE_CANDY               (MEDICINE_ITEMS_END + 6)
#define ITEM_PP_UP                    (MEDICINE_ITEMS_END + 7)
#define ITEM_ZINC                     (MEDICINE_ITEMS_END + 8)
#define ITEM_PP_MAX                   (MEDICINE_ITEMS_END + 9)
#define ITEM_MAX_MUSHROOMS            (MEDICINE_ITEMS_END + 10)

// In case you want to add new protein items, all the indexes below this will be updated with the corrected index
#define PROTEIN_ITEMS_END             ITEM_MAX_MUSHROOMS

// Evolution Items
#define ITEM_SUN_STONE                (PROTEIN_ITEMS_END + 1)
#define ITEM_MOON_STONE               (PROTEIN_ITEMS_END + 2)
#define ITEM_FIRE_STONE               (PROTEIN_ITEMS_END + 3)
#define ITEM_THUNDER_STONE            (PROTEIN_ITEMS_END + 4)
#define ITEM_WATER_STONE              (PROTEIN_ITEMS_END + 5)
#define ITEM_LEAF_STONE               (PROTEIN_ITEMS_END + 6)
#define ITEM_ICE_STONE                (PROTEIN_ITEMS_END + 7)
#define ITEM_DUSK_STONE               (PROTEIN_ITEMS_END + 8)
#define ITEM_DAWN_STONE               (PROTEIN_ITEMS_END + 9)
#define ITEM_SHINY_STONE              (PROTEIN_ITEMS_END + 10)
#define ITEM_PROTECTOR                (PROTEIN_ITEMS_END + 11)
#define ITEM_MAGMARIZER               (PROTEIN_ITEMS_END + 12)
#define ITEM_SACHET                   (PROTEIN_ITEMS_END + 13)
#define ITEM_WHIPPED_DREAM            (PROTEIN_ITEMS_END + 14)
#define ITEM_REAPER_CLOTH             (PROTEIN_ITEMS_END + 15)
#define ITEM_DUBIOUS_DISC             (PROTEIN_ITEMS_END + 16)
#define ITEM_ELECTIRIZER              (PROTEIN_ITEMS_END + 17)
#define ITEM_CRACKED_POT              (PROTEIN_ITEMS_END + 18)
#define ITEM_SWEET_APPLE              (PROTEIN_ITEMS_END + 19)
#define ITEM_TART_APPLE               (PROTEIN_ITEMS_END + 20)
#define ITEM_BLACK_AUGURITE           (PROTEIN_ITEMS_END + 21)
#define ITEM_GALARICA_CUFF            (PROTEIN_ITEMS_END + 22)
#define ITEM_GALARICA_WREATH          (PROTEIN_ITEMS_END + 23)
#define ITEM_LINKING_CORD             (PROTEIN_ITEMS_END + 24)
#define ITEM_SCROLL_OF_DARKNESS       (PROTEIN_ITEMS_END + 25)
#define ITEM_SCROLL_OF_WATERS         (PROTEIN_ITEMS_END + 26)
#define ITEM_AUSPICIOUS_ARMOR         (PROTEIN_ITEMS_END + 27)
#define ITEM_MALICIOUS_ARMOR          (PROTEIN_ITEMS_END + 28)
#define ITEM_KINGS_ROCK               (PROTEIN_ITEMS_END + 29)
#define ITEM_METAL_COAT               (PROTEIN_ITEMS_END + 30)
#define ITEM_DRAGON_SCALE             (PROTEIN_ITEMS_END + 31)
#define ITEM_UP_GRADE                 (PROTEIN_ITEMS_END + 32)
#define ITEM_DEEP_SEA_TOOTH           (PROTEIN_ITEMS_END + 33)
#define ITEM_DEEP_SEA_SCALE           (PROTEIN_ITEMS_END + 34)
#define ITEM_PRISM_SCALE              (PROTEIN_ITEMS_END + 35)

// In case you want to add new evolution items, all the indexes below this will be updated with the corrected index
#define EVOLUTION_ITEMS_END           ITEM_PRISM_SCALE

// Wings
#define ITEM_HEALTH_WING              (EVOLUTION_ITEMS_END + 1)
#define ITEM_MUSCLE_WING              (EVOLUTION_ITEMS_END + 2)
#define ITEM_RESIST_WING              (EVOLUTION_ITEMS_END + 3)
#define ITEM_GENIUS_WING              (EVOLUTION_ITEMS_END + 4)
#define ITEM_CLEVER_WING              (EVOLUTION_ITEMS_END + 5)
#define ITEM_SWIFT_WING               (EVOLUTION_ITEMS_END + 6)
#define ITEM_PRETTY_WING              (EVOLUTION_ITEMS_END + 7)

// Mints
#define ITEM_ADAMANT_MINT             (EVOLUTION_ITEMS_END + 8)
#define ITEM_BOLD_MINT                (EVOLUTION_ITEMS_END + 9)
#define ITEM_BRAVE_MINT               (EVOLUTION_ITEMS_END + 10)
#define ITEM_CALM_MINT                (EVOLUTION_ITEMS_END + 11)
#define ITEM_CAREFUL_MINT             (EVOLUTION_ITEMS_END + 12)
#define ITEM_GENTLE_MINT              (EVOLUTION_ITEMS_END + 13)
#define ITEM_HASTY_MINT               (EVOLUTION_ITEMS_END + 14)
#define ITEM_IMPISH_MINT              (EVOLUTION_ITEMS_END + 15)
#define ITEM_JOLLY_MINT               (EVOLUTION_ITEMS_END + 16)
#define ITEM_LAX_MINT                 (EVOLUTION_ITEMS_END + 17)
#define ITEM_LONELY_MINT              (EVOLUTION_ITEMS_END + 18)
#define ITEM_MILD_MINT                (EVOLUTION_ITEMS_END + 19)
#define ITEM_MODEST_MINT              (EVOLUTION_ITEMS_END + 20)
#define ITEM_NAIVE_MINT               (EVOLUTION_ITEMS_END + 21)
#define ITEM_NAUGHTY_MINT             (EVOLUTION_ITEMS_END + 22)
#define ITEM_QUIET_MINT               (EVOLUTION_ITEMS_END + 23)
#define ITEM_RASH_MINT                (EVOLUTION_ITEMS_END + 24)
#define ITEM_RELAXED_MINT             (EVOLUTION_ITEMS_END + 25)
#define ITEM_SASSY_MINT               (EVOLUTION_ITEMS_END + 26)
#define ITEM_SERIOUS_MINT             (EVOLUTION_ITEMS_END + 27)
#define ITEM_TIMID_MINT               (EVOLUTION_ITEMS_END + 28)

// In case you want to add new natures and consequently new mints, all the indexes below this will be updated with the corrected index
#define MINT_ITEMS_END                ITEM_TIMID_MINT

// Exp. Candies
#define ITEM_EXP_CANDY_XS             (MINT_ITEMS_END + 1)
#define ITEM_EXP_CANDY_S              (MINT_ITEMS_END + 2)
#define ITEM_EXP_CANDY_M              (MINT_ITEMS_END + 3)
#define ITEM_EXP_CANDY_L              (MINT_ITEMS_END + 4)
#define ITEM_EXP_CANDY_XL             (MINT_ITEMS_END + 5)

// In case you want to add new exp. candies, all the indexes below this will be updated with the corrected index
#define EXP_CANDY_ITEMS_END           ITEM_EXP_CANDY_XL

// Tera shards
#define ITEM_NORMAL_TERA_SHARD        (EXP_CANDY_ITEMS_END + 1)
#define ITEM_FIRE_TERA_SHARD          (EXP_CANDY_ITEMS_END + 2)
#define ITEM_WATER_TERA_SHARD         (EXP_CANDY_ITEMS_END + 3)
#define ITEM_ELECTRIC_TERA_SHARD      (EXP_CANDY_ITEMS_END + 4)
#define ITEM_GRASS_TERA_SHARD         (EXP_CANDY_ITEMS_END + 5)
#define ITEM_ICE_TERA_SHARD           (EXP_CANDY_ITEMS_END + 6)
#define ITEM_FIGHTING_TERA_SHARD      (EXP_CANDY_ITEMS_END + 7)
#define ITEM_POISON_TERA_SHARD        (EXP_CANDY_ITEMS_END + 8)
#define ITEM_GROUND_TERA_SHARD        (EXP_CANDY_ITEMS_END + 9)
#define ITEM_FLYING_TERA_SHARD        (EXP_CANDY_ITEMS_END + 10)
#define ITEM_PSYCHIC_TERA_SHARD       (EXP_CANDY_ITEMS_END + 11)
#define ITEM_BUG_TERA_SHARD           (EXP_CANDY_ITEMS_END + 12)
#define ITEM_ROCK_TERA_SHARD          (EXP_CANDY_ITEMS_END + 13)
#define ITEM_GHOST_TERA_SHARD         (EXP_CANDY_ITEMS_END + 14)
#define ITEM_DRAGON_TERA_SHARD        (EXP_CANDY_ITEMS_END + 15)
#define ITEM_DARK_TERA_SHARD          (EXP_CANDY_ITEMS_END + 16)
#define ITEM_STEEL_TERA_SHARD         (EXP_CANDY_ITEMS_END + 17)
#define ITEM_FAIRY_TERA_SHARD         (EXP_CANDY_ITEMS_END + 18)

// In case you want to add new types and consequently new tera shards, all the indexes below this will be updated with the corrected index
#define TERA_SHARD_ITEMS_END          ITEM_FAIRY_TERA_SHARD

// Berries
#define ITEM_CHERI_BERRY              (TERA_SHARD_ITEMS_END + 1)
#define ITEM_CHESTO_BERRY             (TERA_SHARD_ITEMS_END + 2)
#define ITEM_PECHA_BERRY              (TERA_SHARD_ITEMS_END + 3)
#define ITEM_RAWST_BERRY              (TERA_SHARD_ITEMS_END + 4)
#define ITEM_ASPEAR_BERRY             (TERA_SHARD_ITEMS_END + 5)
#define ITEM_LEPPA_BERRY              (TERA_SHARD_ITEMS_END + 6)
#define ITEM_ORAN_BERRY               (TERA_SHARD_ITEMS_END + 7)
#define ITEM_PERSIM_BERRY             (TERA_SHARD_ITEMS_END + 8)
#define ITEM_LUM_BERRY                (TERA_SHARD_ITEMS_END + 9)
#define ITEM_SITRUS_BERRY             (TERA_SHARD_ITEMS_END + 10)
#define ITEM_FIGY_BERRY               (TERA_SHARD_ITEMS_END + 11)
#define ITEM_WIKI_BERRY               (TERA_SHARD_ITEMS_END + 12)
#define ITEM_MAGO_BERRY               (TERA_SHARD_ITEMS_END + 13)
#define ITEM_AGUAV_BERRY              (TERA_SHARD_ITEMS_END + 14)
#define ITEM_IAPAPA_BERRY             (TERA_SHARD_ITEMS_END + 15)
#define ITEM_RAZZ_BERRY               (TERA_SHARD_ITEMS_END + 16)
#define ITEM_BLUK_BERRY               (TERA_SHARD_ITEMS_END + 17)
#define ITEM_NANAB_BERRY              (TERA_SHARD_ITEMS_END + 18)
#define ITEM_WEPEAR_BERRY             (TERA_SHARD_ITEMS_END + 19)
#define ITEM_PINAP_BERRY              (TERA_SHARD_ITEMS_END + 20)
#define ITEM_POMEG_BERRY              (TERA_SHARD_ITEMS_END + 21)
#define ITEM_KELPSY_BERRY             (TERA_SHARD_ITEMS_END + 22)
#define ITEM_QUALOT_BERRY             (TERA_SHARD_ITEMS_END + 23)
#define ITEM_HONDEW_BERRY             (TERA_SHARD_ITEMS_END + 24)
#define ITEM_GREPA_BERRY              (TERA_SHARD_ITEMS_END + 25)
#define ITEM_TAMATO_BERRY             (TERA_SHARD_ITEMS_END + 26)
#define ITEM_CORNN_BERRY              (TERA_SHARD_ITEMS_END + 27)
#define ITEM_MAGOST_BERRY             (TERA_SHARD_ITEMS_END + 28)
#define ITEM_RABUTA_BERRY             (TERA_SHARD_ITEMS_END + 29)
#define ITEM_NOMEL_BERRY              (TERA_SHARD_ITEMS_END + 30)
#define ITEM_SPELON_BERRY             (TERA_SHARD_ITEMS_END + 31)
#define ITEM_PAMTRE_BERRY             (TERA_SHARD_ITEMS_END + 32)
#define ITEM_WATMEL_BERRY             (TERA_SHARD_ITEMS_END + 33)
#define ITEM_DURIN_BERRY              (TERA_SHARD_ITEMS_END + 34)
#define ITEM_BELUE_BERRY              (TERA_SHARD_ITEMS_END + 35)
#define ITEM_LIECHI_BERRY             (TERA_SHARD_ITEMS_END + 36)
#define ITEM_GANLON_BERRY             (TERA_SHARD_ITEMS_END + 37)
#define ITEM_SALAC_BERRY              (TERA_SHARD_ITEMS_END + 38)
#define ITEM_PETAYA_BERRY             (TERA_SHARD_ITEMS_END + 39)
#define ITEM_APICOT_BERRY             (TERA_SHARD_ITEMS_END + 40)
#define ITEM_LANSAT_BERRY             (TERA_SHARD_ITEMS_END + 41)
#define ITEM_STARF_BERRY              (TERA_SHARD_ITEMS_END + 42)
#define ITEM_OCCA_BERRY               (TERA_SHARD_ITEMS_END + 43)
#define ITEM_PASSHO_BERRY             (TERA_SHARD_ITEMS_END + 44)
#define ITEM_WACAN_BERRY              (TERA_SHARD_ITEMS_END + 45)
#define ITEM_RINDO_BERRY              (TERA_SHARD_ITEMS_END + 46)
#define ITEM_YACHE_BERRY              (TERA_SHARD_ITEMS_END + 47)
#define ITEM_CHOPLE_BERRY             (TERA_SHARD_ITEMS_END + 48)
#define ITEM_KEBIA_BERRY              (TERA_SHARD_ITEMS_END + 49)
#define ITEM_SHUCA_BERRY              (TERA_SHARD_ITEMS_END + 50)
#define ITEM_COBA_BERRY               (TERA_SHARD_ITEMS_END + 51)
#define ITEM_PAYAPA_BERRY             (TERA_SHARD_ITEMS_END + 52)
#define ITEM_TANGA_BERRY              (TERA_SHARD_ITEMS_END + 53)
#define ITEM_CHARTI_BERRY             (TERA_SHARD_ITEMS_END + 54)
#define ITEM_KASIB_BERRY              (TERA_SHARD_ITEMS_END + 55)
#define ITEM_HABAN_BERRY              (TERA_SHARD_ITEMS_END + 56)
#define ITEM_COLBUR_BERRY             (TERA_SHARD_ITEMS_END + 57)
#define ITEM_BABIRI_BERRY             (TERA_SHARD_ITEMS_END + 58)
#define ITEM_CHILAN_BERRY             (TERA_SHARD_ITEMS_END + 59)
#define ITEM_MICLE_BERRY              (TERA_SHARD_ITEMS_END + 60)
#define ITEM_CUSTAP_BERRY             (TERA_SHARD_ITEMS_END + 61)
#define ITEM_JABOCA_BERRY             (TERA_SHARD_ITEMS_END + 62)
#define ITEM_ROWAP_BERRY              (TERA_SHARD_ITEMS_END + 63)
#define ITEM_ROSELI_BERRY             (TERA_SHARD_ITEMS_END + 64)
#define ITEM_KEE_BERRY                (TERA_SHARD_ITEMS_END + 65)
#define ITEM_MARANGA_BERRY            (TERA_SHARD_ITEMS_END + 66)
#define ITEM_HOPO_BERRY               (TERA_SHARD_ITEMS_END + 67)
#define ITEM_ENIGMA_BERRY             (TERA_SHARD_ITEMS_END + 68)

// In case you want to add new berries, all the indexes below this will be updated with the corrected index
#define BERRY_ITEMS_END               ITEM_ENIGMA_BERRY

// Battle
#define ITEM_POKE_DOLL                (BERRY_ITEMS_END + 1)
#define ITEM_FLUFFY_TAIL              (BERRY_ITEMS_END + 2)
#define ITEM_BRIGHT_POWDER            (BERRY_ITEMS_END + 3)
#define ITEM_WHITE_HERB               (BERRY_ITEMS_END + 4)
#define ITEM_MACHO_BRACE              (BERRY_ITEMS_END + 5)
#define ITEM_RAZOR_FANG               (BERRY_ITEMS_END + 6)
#define ITEM_QUICK_CLAW               (BERRY_ITEMS_END + 7)
#define ITEM_SOOTHE_BELL              (BERRY_ITEMS_END + 8)
#define ITEM_MENTAL_HERB              (BERRY_ITEMS_END + 9)
#define ITEM_CHOICE_BAND              (BERRY_ITEMS_END + 10)
#define ITEM_OVAL_STONE               (BERRY_ITEMS_END + 11)
#define ITEM_SILVER_POWDER            (BERRY_ITEMS_END + 12)
#define ITEM_AMULET_COIN              (BERRY_ITEMS_END + 13)
#define ITEM_CLEANSE_TAG              (BERRY_ITEMS_END + 14)
#define ITEM_SOUL_DEW                 (BERRY_ITEMS_END + 15)
#define ITEM_RED_ORB                  (BERRY_ITEMS_END + 16)
#define ITEM_BLUE_ORB                 (BERRY_ITEMS_END + 17)
#define ITEM_SMOKE_BALL               (BERRY_ITEMS_END + 18)
#define ITEM_EVERSTONE                (BERRY_ITEMS_END + 19)
#define ITEM_FOCUS_BAND               (BERRY_ITEMS_END + 20)
#define ITEM_LUCKY_EGG                (BERRY_ITEMS_END + 21)
#define ITEM_SCOPE_LENS               (BERRY_ITEMS_END + 22)
#define ITEM_RAZOR_CLAW               (BERRY_ITEMS_END + 23)
#define ITEM_LEFTOVERS                (BERRY_ITEMS_END + 24)
#define ITEM_DIRE_HIT                 (BERRY_ITEMS_END + 25)
#define ITEM_LIGHT_BALL               (BERRY_ITEMS_END + 26)
#define ITEM_SOFT_SAND                (BERRY_ITEMS_END + 27)
#define ITEM_HARD_STONE               (BERRY_ITEMS_END + 28)
#define ITEM_MIRACLE_SEED             (BERRY_ITEMS_END + 29)
#define ITEM_BLACK_GLASSES            (BERRY_ITEMS_END + 30)
#define ITEM_BLACK_BELT               (BERRY_ITEMS_END + 31)
#define ITEM_MAGNET                   (BERRY_ITEMS_END + 32)
#define ITEM_MYSTIC_WATER             (BERRY_ITEMS_END + 33)
#define ITEM_SHARP_BEAK               (BERRY_ITEMS_END + 34)
#define ITEM_POISON_BARB              (BERRY_ITEMS_END + 35)
#define ITEM_NEVER_MELT_ICE           (BERRY_ITEMS_END + 36)
#define ITEM_SPELL_TAG                (BERRY_ITEMS_END + 37)
#define ITEM_TWISTED_SPOON            (BERRY_ITEMS_END + 38)
#define ITEM_CHARCOAL                 (BERRY_ITEMS_END + 39)
#define ITEM_DRAGON_FANG              (BERRY_ITEMS_END + 40)
#define ITEM_SILK_SCARF               (BERRY_ITEMS_END + 41)
#define ITEM_GUARD_SPEC               (BERRY_ITEMS_END + 42)
#define ITEM_SHELL_BELL               (BERRY_ITEMS_END + 43)
#define ITEM_LUCKY_PUNCH              (BERRY_ITEMS_END + 44)
#define ITEM_METAL_POWDER             (BERRY_ITEMS_END + 45)
#define ITEM_THICK_CLUB               (BERRY_ITEMS_END + 46)
#define ITEM_LEEK                     (BERRY_ITEMS_END + 47)
#define ITEM_ADAMANT_ORB              (BERRY_ITEMS_END + 48)
#define ITEM_LUSTROUS_ORB             (BERRY_ITEMS_END + 49)
#define ITEM_GRISEOUS_ORB             (BERRY_ITEMS_END + 50)
#define ITEM_EXPERT_BELT              (BERRY_ITEMS_END + 51)
#define ITEM_POWER_HERB               (BERRY_ITEMS_END + 52)
#define ITEM_WIDE_LENS                (BERRY_ITEMS_END + 53)
#define ITEM_ZOOM_LENS                (BERRY_ITEMS_END + 54)
#define ITEM_DESTINY_KNOT             (BERRY_ITEMS_END + 55)
#define ITEM_SMOOTH_ROCK              (BERRY_ITEMS_END + 56)
#define ITEM_DAMP_ROCK                (BERRY_ITEMS_END + 57)
#define ITEM_HEAT_ROCK                (BERRY_ITEMS_END + 58)
#define ITEM_ICY_ROCK                 (BERRY_ITEMS_END + 59)
#define ITEM_BIG_ROOT                 (BERRY_ITEMS_END + 60)
#define ITEM_LIGHT_CLAY               (BERRY_ITEMS_END + 61)
#define ITEM_SAFETY_GOGGLES           (BERRY_ITEMS_END + 62)
#define ITEM_ROCKY_HELMET             (BERRY_ITEMS_END + 63)
#define ITEM_WEAKNESS_POLICY          (BERRY_ITEMS_END + 64)
#define ITEM_ASSAULT_VEST             (BERRY_ITEMS_END + 65)
#define ITEM_EVIOLITE                 (BERRY_ITEMS_END + 66)
#define ITEM_ABSORB_BULB              (BERRY_ITEMS_END + 67)
#define ITEM_AIR_BALLOON              (BERRY_ITEMS_END + 68)
#define ITEM_ADRENALINE_ORB           (BERRY_ITEMS_END + 69)
#define ITEM_BINDING_BAND             (BERRY_ITEMS_END + 70)
#define ITEM_CELL_BATTERY             (BERRY_ITEMS_END + 71)
#define ITEM_EJECT_BUTTON             (BERRY_ITEMS_END + 72)
#define ITEM_FLOAT_STONE              (BERRY_ITEMS_END + 73)
#define ITEM_FOCUS_SASH               (BERRY_ITEMS_END + 74)
#define ITEM_GRIP_CLAW                (BERRY_ITEMS_END + 75)
#define ITEM_IRON_BALL                (BERRY_ITEMS_END + 76)
#define ITEM_LAGGING_TAIL             (BERRY_ITEMS_END + 77)
#define ITEM_LUMINOUS_MOSS            (BERRY_ITEMS_END + 78)
#define ITEM_QUICK_POWDER             (BERRY_ITEMS_END + 79)
#define ITEM_METRONOME                (BERRY_ITEMS_END + 80)
#define ITEM_MUSCLE_BAND              (BERRY_ITEMS_END + 81)
#define ITEM_PROTECTIVE_PADS          (BERRY_ITEMS_END + 82)
#define ITEM_RED_CARD                 (BERRY_ITEMS_END + 83)
#define ITEM_RING_TARGET              (BERRY_ITEMS_END + 84)
#define ITEM_SHED_SHELL               (BERRY_ITEMS_END + 85)
#define ITEM_SNOWBALL                 (BERRY_ITEMS_END + 86)
#define ITEM_STICKY_BARB              (BERRY_ITEMS_END + 87)
#define ITEM_TERRAIN_EXTENDER         (BERRY_ITEMS_END + 88)
#define ITEM_WISE_GLASSES             (BERRY_ITEMS_END + 89)
#define ITEM_ELECTRIC_SEED            (BERRY_ITEMS_END + 90)
#define ITEM_GRASSY_SEED              (BERRY_ITEMS_END + 91)
#define ITEM_MISTY_SEED               (BERRY_ITEMS_END + 92)
#define ITEM_PSYCHIC_SEED             (BERRY_ITEMS_END + 93)
#define ITEM_LIFE_ORB                 (BERRY_ITEMS_END + 94)
#define ITEM_TOXIC_ORB                (BERRY_ITEMS_END + 95)
#define ITEM_FLAME_ORB                (BERRY_ITEMS_END + 96)
#define ITEM_BLACK_SLUDGE             (BERRY_ITEMS_END + 97)
#define ITEM_CHOICE_SPECS             (BERRY_ITEMS_END + 98)
#define ITEM_CHOICE_SCARF             (BERRY_ITEMS_END + 99)
#define ITEM_EJECT_PACK               (BERRY_ITEMS_END + 100)
#define ITEM_ROOM_SERVICE             (BERRY_ITEMS_END + 101)
#define ITEM_BLUNDER_POLICY           (BERRY_ITEMS_END + 102)
#define ITEM_HEAVY_DUTY_BOOTS         (BERRY_ITEMS_END + 103)
#define ITEM_UTILITY_UMBRELLA         (BERRY_ITEMS_END + 104)
#define ITEM_THROAT_SPRAY             (BERRY_ITEMS_END + 105)
#define ITEM_BOOSTER_ENERGY           (BERRY_ITEMS_END + 106)
#define ITEM_ABILITY_SHIELD           (BERRY_ITEMS_END + 107)
#define ITEM_CLEAR_AMULET             (BERRY_ITEMS_END + 108)
#define ITEM_MIRROR_HERB              (BERRY_ITEMS_END + 109)
#define ITEM_PUNCHING_GLOVE           (BERRY_ITEMS_END + 110)
#define ITEM_COVERT_CLOAK             (BERRY_ITEMS_END + 111)
#define ITEM_LOADED_DICE              (BERRY_ITEMS_END + 112)
#define ITEM_RUSTED_SWORD             (BERRY_ITEMS_END + 113)
#define ITEM_RUSTED_SHIELD            (BERRY_ITEMS_END + 114)
#define ITEM_X_ATTACK                 (BERRY_ITEMS_END + 115)
#define ITEM_X_DEFEND                 (BERRY_ITEMS_END + 116)
#define ITEM_X_SPEED                  (BERRY_ITEMS_END + 117)
#define ITEM_X_ACCURACY               (BERRY_ITEMS_END + 118)
#define ITEM_X_SPECIAL                (BERRY_ITEMS_END + 119)
#define ITEM_X_SPECIAL_DEFENSE        (BERRY_ITEMS_END + 120)

// In case you want to add new battle items, all the indexes below this will be updated with the corrected index
#define BATTLE_ITEMS_END              ITEM_X_SPECIAL_DEFENSE

// Power Items
#define ITEM_POWER_BRACER             (BATTLE_ITEMS_END + 1)
#define ITEM_POWER_BELT               (BATTLE_ITEMS_END + 2)
#define ITEM_POWER_LENS               (BATTLE_ITEMS_END + 3)
#define ITEM_POWER_BAND               (BATTLE_ITEMS_END + 4)
#define ITEM_POWER_ANKLET             (BATTLE_ITEMS_END + 5)
#define ITEM_POWER_WEIGHT             (BATTLE_ITEMS_END + 6)

// Incenses
#define ITEM_SEA_INCENSE              (BATTLE_ITEMS_END + 7)
#define ITEM_LAX_INCENSE              (BATTLE_ITEMS_END + 8)
#define ITEM_LUCK_INCENSE             (BATTLE_ITEMS_END + 9)
#define ITEM_FULL_INCENSE             (BATTLE_ITEMS_END + 10)
#define ITEM_ODD_INCENSE              (BATTLE_ITEMS_END + 11)
#define ITEM_ROCK_INCENSE             (BATTLE_ITEMS_END + 12)
#define ITEM_ROSE_INCENSE             (BATTLE_ITEMS_END + 13)
#define ITEM_WAVE_INCENSE             (BATTLE_ITEMS_END + 14)

// In case you want to add new incense items, all the indexes below this will be updated with the corrected index
#define INCENSE_ITEMS_END             ITEM_WAVE_INCENSE

// Mega
#define ITEM_VENUSAURITE              (INCENSE_ITEMS_END + 1)
#define ITEM_CHARIZARDITE_X           (INCENSE_ITEMS_END + 2)
#define ITEM_CHARIZARDITE_Y           (INCENSE_ITEMS_END + 3)
#define ITEM_BLASTOISINITE            (INCENSE_ITEMS_END + 4)
#define ITEM_BEEDRILLITE              (INCENSE_ITEMS_END + 5)
#define ITEM_PIDGEOTITE               (INCENSE_ITEMS_END + 6)
#define ITEM_ALAKAZITE                (INCENSE_ITEMS_END + 7)
#define ITEM_SLOWBRONITE              (INCENSE_ITEMS_END + 8)
#define ITEM_GENGARITE                (INCENSE_ITEMS_END + 9)
#define ITEM_KANGASKHANITE            (INCENSE_ITEMS_END + 10)
#define ITEM_PINSIRITE                (INCENSE_ITEMS_END + 11)
#define ITEM_GYARADOSITE              (INCENSE_ITEMS_END + 12)
#define ITEM_AERODACTYLITE            (INCENSE_ITEMS_END + 13)
#define ITEM_MEWTWONITE_X             (INCENSE_ITEMS_END + 14)
#define ITEM_MEWTWONITE_Y             (INCENSE_ITEMS_END + 15)
#define ITEM_AMPHAROSITE              (INCENSE_ITEMS_END + 16)
#define ITEM_STEELIXITE               (INCENSE_ITEMS_END + 17)
#define ITEM_SCIZORITE                (INCENSE_ITEMS_END + 18)
#define ITEM_HERACRONITE              (INCENSE_ITEMS_END + 19)
#define ITEM_HOUNDOOMINITE            (INCENSE_ITEMS_END + 20)
#define ITEM_TYRANITARITE             (INCENSE_ITEMS_END + 21)
#define ITEM_SCEPTILITE               (INCENSE_ITEMS_END + 22)
#define ITEM_BLAZIKENITE              (INCENSE_ITEMS_END + 23)
#define ITEM_SWAMPERTITE              (INCENSE_ITEMS_END + 24)
#define ITEM_GARDEVOIRITE             (INCENSE_ITEMS_END + 25)
#define ITEM_SABLENITE                (INCENSE_ITEMS_END + 26)
#define ITEM_MAWILITE                 (INCENSE_ITEMS_END + 27)
#define ITEM_AGGRONITE                (INCENSE_ITEMS_END + 28)
#define ITEM_MEDICHAMITE              (INCENSE_ITEMS_END + 29)
#define ITEM_MANECTITE                (INCENSE_ITEMS_END + 30)
#define ITEM_SHARPEDONITE             (INCENSE_ITEMS_END + 31)
#define ITEM_CAMERUPTITE              (INCENSE_ITEMS_END + 32)
#define ITEM_ALTARIANITE              (INCENSE_ITEMS_END + 33)
#define ITEM_BANETTITE                (INCENSE_ITEMS_END + 34)
#define ITEM_ABSOLITE                 (INCENSE_ITEMS_END + 35)
#define ITEM_GLALITITE                (INCENSE_ITEMS_END + 36)
#define ITEM_SALAMENCITE              (INCENSE_ITEMS_END + 37)
#define ITEM_METAGROSSITE             (INCENSE_ITEMS_END + 38)
#define ITEM_LATIASITE                (INCENSE_ITEMS_END + 39)
#define ITEM_LATIOSITE                (INCENSE_ITEMS_END + 40)
#define ITEM_LOPUNNITE                (INCENSE_ITEMS_END + 41)
#define ITEM_GARCHOMPITE              (INCENSE_ITEMS_END + 42)
#define ITEM_LUCARIONITE              (INCENSE_ITEMS_END + 43)
#define ITEM_ABOMASITE                (INCENSE_ITEMS_END + 44)
#define ITEM_GALLADITE                (INCENSE_ITEMS_END + 45)
#define ITEM_AUDINITE                 (INCENSE_ITEMS_END + 46)
#define ITEM_DIANCITE                 (INCENSE_ITEMS_END + 47)
#define ITEM_ULTRANECROZIUM_Z         (INCENSE_ITEMS_END + 48)

// In case you want to add new mega stones, all the indexes below this will be updated with the corrected index
#define MEGA_STONE_ITEMS_END          ITEM_ULTRANECROZIUM_Z

// Gems
#define ITEM_NORMAL_GEM               (MEGA_STONE_ITEMS_END + 1)
#define ITEM_FIGHTING_GEM             (MEGA_STONE_ITEMS_END + 2)
#define ITEM_FLYING_GEM               (MEGA_STONE_ITEMS_END + 3)
#define ITEM_POISON_GEM               (MEGA_STONE_ITEMS_END + 4)
#define ITEM_GROUND_GEM               (MEGA_STONE_ITEMS_END + 5)
#define ITEM_ROCK_GEM                 (MEGA_STONE_ITEMS_END + 6)
#define ITEM_BUG_GEM                  (MEGA_STONE_ITEMS_END + 7)
#define ITEM_GHOST_GEM                (MEGA_STONE_ITEMS_END + 8)
#define ITEM_STEEL_GEM                (MEGA_STONE_ITEMS_END + 9)
#define ITEM_FIRE_GEM                 (MEGA_STONE_ITEMS_END + 10)
#define ITEM_WATER_GEM                (MEGA_STONE_ITEMS_END + 11)
#define ITEM_GRASS_GEM                (MEGA_STONE_ITEMS_END + 12)
#define ITEM_ELECTRIC_GEM             (MEGA_STONE_ITEMS_END + 13)
#define ITEM_PSYCHIC_GEM              (MEGA_STONE_ITEMS_END + 14)
#define ITEM_ICE_GEM                  (MEGA_STONE_ITEMS_END + 15)
#define ITEM_DRAGON_GEM               (MEGA_STONE_ITEMS_END + 16)
#define ITEM_DARK_GEM                 (MEGA_STONE_ITEMS_END + 17)
#define ITEM_FAIRY_GEM                (MEGA_STONE_ITEMS_END + 18)

// In case you want to add new types and consequently new gems, all the indexes below this will be updated with the corrected index
#define GEM_ITEMS_END                 ITEM_FAIRY_GEM

// Plates
#define ITEM_FIST_PLATE               (GEM_ITEMS_END + 1)
#define ITEM_SKY_PLATE                (GEM_ITEMS_END + 2)
#define ITEM_TOXIC_PLATE              (GEM_ITEMS_END + 3)
#define ITEM_EARTH_PLATE              (GEM_ITEMS_END + 4)
#define ITEM_STONE_PLATE              (GEM_ITEMS_END + 5)
#define ITEM_INSECT_PLATE             (GEM_ITEMS_END + 6)
#define ITEM_SPOOKY_PLATE             (GEM_ITEMS_END + 7)
#define ITEM_IRON_PLATE               (GEM_ITEMS_END + 8)
#define ITEM_FLAME_PLATE              (GEM_ITEMS_END + 9)
#define ITEM_SPLASH_PLATE             (GEM_ITEMS_END + 10)
#define ITEM_MEADOW_PLATE             (GEM_ITEMS_END + 11)
#define ITEM_ZAP_PLATE                (GEM_ITEMS_END + 12)
#define ITEM_MIND_PLATE               (GEM_ITEMS_END + 13)
#define ITEM_ICICLE_PLATE             (GEM_ITEMS_END + 14)
#define ITEM_DRACO_PLATE              (GEM_ITEMS_END + 15)
#define ITEM_DREAD_PLATE              (GEM_ITEMS_END + 16)
#define ITEM_PIXIE_PLATE              (GEM_ITEMS_END + 17)

// In case you want to add new types and consequently new plates, all the indexes below this will be updated with the corrected index
#define PLATE_ITEMS_END               ITEM_PIXIE_PLATE

// Drives
#define ITEM_BURN_DRIVE               (PLATE_ITEMS_END + 1)
#define ITEM_DOUSE_DRIVE              (PLATE_ITEMS_END + 2)
#define ITEM_SHOCK_DRIVE              (PLATE_ITEMS_END + 3)
#define ITEM_CHILL_DRIVE              (PLATE_ITEMS_END + 4)

// Memories
#define ITEM_FIGHTING_MEMORY          (PLATE_ITEMS_END + 5)
#define ITEM_FLYING_MEMORY            (PLATE_ITEMS_END + 6)
#define ITEM_POISON_MEMORY            (PLATE_ITEMS_END + 7)
#define ITEM_GROUND_MEMORY            (PLATE_ITEMS_END + 8)
#define ITEM_ROCK_MEMORY              (PLATE_ITEMS_END + 9)
#define ITEM_BUG_MEMORY               (PLATE_ITEMS_END + 10)
#define ITEM_GHOST_MEMORY             (PLATE_ITEMS_END + 11)
#define ITEM_STEEL_MEMORY             (PLATE_ITEMS_END + 12)
#define ITEM_FIRE_MEMORY              (PLATE_ITEMS_END + 13)
#define ITEM_WATER_MEMORY             (PLATE_ITEMS_END + 14)
#define ITEM_GRASS_MEMORY             (PLATE_ITEMS_END + 15)
#define ITEM_ELECTRIC_MEMORY          (PLATE_ITEMS_END + 16)
#define ITEM_PSYCHIC_MEMORY           (PLATE_ITEMS_END + 17)
#define ITEM_ICE_MEMORY               (PLATE_ITEMS_END + 18)
#define ITEM_DRAGON_MEMORY            (PLATE_ITEMS_END + 19)
#define ITEM_DARK_MEMORY              (PLATE_ITEMS_END + 20)
#define ITEM_FAIRY_MEMORY             (PLATE_ITEMS_END + 21)

// In case you want to add new types and consequently new memories, all the indexes below this will be updated with the corrected index
#define MEMORY_ITEMS_END              ITEM_FAIRY_MEMORY

// Z Crystals
#define ITEM_NORMALIUM_Z              (MEMORY_ITEMS_END + 1)
#define ITEM_FIGHTINIUM_Z             (MEMORY_ITEMS_END + 2)
#define ITEM_FLYINIUM_Z               (MEMORY_ITEMS_END + 3)
#define ITEM_POISONIUM_Z              (MEMORY_ITEMS_END + 4)
#define ITEM_GROUNDIUM_Z              (MEMORY_ITEMS_END + 5)
#define ITEM_ROCKIUM_Z                (MEMORY_ITEMS_END + 6)
#define ITEM_BUGINIUM_Z               (MEMORY_ITEMS_END + 7)
#define ITEM_GHOSTIUM_Z               (MEMORY_ITEMS_END + 8)
#define ITEM_STEELIUM_Z               (MEMORY_ITEMS_END + 9)
#define ITEM_FIRIUM_Z                 (MEMORY_ITEMS_END + 10)
#define ITEM_WATERIUM_Z               (MEMORY_ITEMS_END + 11)
#define ITEM_GRASSIUM_Z               (MEMORY_ITEMS_END + 12)
#define ITEM_ELECTRIUM_Z              (MEMORY_ITEMS_END + 13)
#define ITEM_PSYCHIUM_Z               (MEMORY_ITEMS_END + 14)
#define ITEM_ICIUM_Z                  (MEMORY_ITEMS_END + 15)
#define ITEM_DRAGONIUM_Z              (MEMORY_ITEMS_END + 16)
#define ITEM_DARKINIUM_Z              (MEMORY_ITEMS_END + 17)
#define ITEM_FAIRIUM_Z                (MEMORY_ITEMS_END + 18)
#define ITEM_ALORAICHIUM_Z            (MEMORY_ITEMS_END + 19)
#define ITEM_DECIDIUM_Z               (MEMORY_ITEMS_END + 20)
#define ITEM_EEVIUM_Z                 (MEMORY_ITEMS_END + 21)
#define ITEM_INCINIUM_Z               (MEMORY_ITEMS_END + 22)
#define ITEM_KOMMONIUM_Z              (MEMORY_ITEMS_END + 23)
#define ITEM_LUNALIUM_Z               (MEMORY_ITEMS_END + 24)
#define ITEM_LYCANIUM_Z               (MEMORY_ITEMS_END + 25)
#define ITEM_MARSHADIUM_Z             (MEMORY_ITEMS_END + 26)
#define ITEM_MEWNIUM_Z                (MEMORY_ITEMS_END + 27)
#define ITEM_MIMIKIUM_Z               (MEMORY_ITEMS_END + 28)
#define ITEM_PIKANIUM_Z               (MEMORY_ITEMS_END + 29)
#define ITEM_PIKASHUNIUM_Z            (MEMORY_ITEMS_END + 30)
#define ITEM_PRIMARIUM_Z              (MEMORY_ITEMS_END + 31)
#define ITEM_SNORLIUM_Z               (MEMORY_ITEMS_END + 32)
#define ITEM_SOLGANIUM_Z              (MEMORY_ITEMS_END + 33)
#define ITEM_TAPUNIUM_Z               (MEMORY_ITEMS_END + 34)

// In case you want to add new z crystals, all the indexes below this will be updated with the corrected index
#define Z_CRYSTAL_ITEMS_END           ITEM_TAPUNIUM_Z

// Valuable
#define ITEM_SHOAL_SHELL              (Z_CRYSTAL_ITEMS_END + 1)
#define ITEM_TINY_MUSHROOM            (Z_CRYSTAL_ITEMS_END + 2)
#define ITEM_BIG_MUSHROOM             (Z_CRYSTAL_ITEMS_END + 3)
#define ITEM_PEARL                    (Z_CRYSTAL_ITEMS_END + 4)
#define ITEM_BIG_PEARL                (Z_CRYSTAL_ITEMS_END + 5)
#define ITEM_STARDUST                 (Z_CRYSTAL_ITEMS_END + 6)
#define ITEM_STAR_PIECE               (Z_CRYSTAL_ITEMS_END + 7)
#define ITEM_NUGGET                   (Z_CRYSTAL_ITEMS_END + 8)
#define ITEM_HEART_SCALE              (Z_CRYSTAL_ITEMS_END + 9)
#define ITEM_RARE_BONE                (Z_CRYSTAL_ITEMS_END + 10)
#define ITEM_PEARL_STRING             (Z_CRYSTAL_ITEMS_END + 11)
#define ITEM_BIG_NUGGET               (Z_CRYSTAL_ITEMS_END + 12)
#define ITEM_COMET_SHARD              (Z_CRYSTAL_ITEMS_END + 13)
#define ITEM_BALM_MUSHROOM            (Z_CRYSTAL_ITEMS_END + 14)
#define ITEM_BOTTLE_CAP               (Z_CRYSTAL_ITEMS_END + 15)
#define ITEM_GOLD_BOTTLE_CAP          (Z_CRYSTAL_ITEMS_END + 16)

// In case you want to add new items valuable, all the indexes below this will be updated with the corrected index
#define VALUABLE_ITEMS_END            ITEM_GOLD_BOTTLE_CAP

// Fossils
#define ITEM_HELIX_FOSSIL             (VALUABLE_ITEMS_END + 1)
#define ITEM_DOME_FOSSIL              (VALUABLE_ITEMS_END + 2)
#define ITEM_OLD_AMBER                (VALUABLE_ITEMS_END + 3)
#define ITEM_ROOT_FOSSIL              (VALUABLE_ITEMS_END + 4)
#define ITEM_CLAW_FOSSIL              (VALUABLE_ITEMS_END + 5)
#define ITEM_SKULL_FOSSIL             (VALUABLE_ITEMS_END + 6)
#define ITEM_ARMOR_FOSSIL             (VALUABLE_ITEMS_END + 7)
#define ITEM_COVER_FOSSIL             (VALUABLE_ITEMS_END + 8)
#define ITEM_PLUME_FOSSIL             (VALUABLE_ITEMS_END + 9)
#define ITEM_JAW_FOSSIL               (VALUABLE_ITEMS_END + 10)
#define ITEM_SAIL_FOSSIL              (VALUABLE_ITEMS_END + 11)
#define ITEM_BIRD_FOSSIL              (VALUABLE_ITEMS_END + 12)
#define ITEM_FISH_FOSSIL              (VALUABLE_ITEMS_END + 13)
#define ITEM_DRAKE_FOSSIL             (VALUABLE_ITEMS_END + 14)
#define ITEM_DINO_FOSSIL              (VALUABLE_ITEMS_END + 15)

// In case you want to add new fossils, all the indexes below this will be updated with the corrected index
#define FOSSIL_ITEMS_END              ITEM_DINO_FOSSIL

// Mail
#define ITEM_ORANGE_MAIL              (FOSSIL_ITEMS_END + 1)
#define ITEM_HARBOR_MAIL              (FOSSIL_ITEMS_END + 2)
#define ITEM_GLITTER_MAIL             (FOSSIL_ITEMS_END + 3)
#define ITEM_MECH_MAIL                (FOSSIL_ITEMS_END + 4)
#define ITEM_WOOD_MAIL                (FOSSIL_ITEMS_END + 5)
#define ITEM_WAVE_MAIL                (FOSSIL_ITEMS_END + 6)
#define ITEM_BEAD_MAIL                (FOSSIL_ITEMS_END + 7)
#define ITEM_SHADOW_MAIL              (FOSSIL_ITEMS_END + 8)
#define ITEM_TROPIC_MAIL              (FOSSIL_ITEMS_END + 9)
#define ITEM_DREAM_MAIL               (FOSSIL_ITEMS_END + 10)
#define ITEM_FAB_MAIL                 (FOSSIL_ITEMS_END + 11)
#define ITEM_RETRO_MAIL               (FOSSIL_ITEMS_END + 12)

// Shards
#define ITEM_RED_SHARD                (FOSSIL_ITEMS_END + 13)
#define ITEM_BLUE_SHARD               (FOSSIL_ITEMS_END + 14)
#define ITEM_YELLOW_SHARD             (FOSSIL_ITEMS_END + 15)
#define ITEM_GREEN_SHARD              (FOSSIL_ITEMS_END + 16)

// Others
#define ITEM_WISHING_PIECE            (FOSSIL_ITEMS_END + 17)
#define ITEM_HONEY                    (FOSSIL_ITEMS_END + 18)
#define ITEM_REPEL                    (FOSSIL_ITEMS_END + 19)
#define ITEM_SUPER_REPEL              (FOSSIL_ITEMS_END + 20)
#define ITEM_MAX_REPEL                (FOSSIL_ITEMS_END + 21)
#define ITEM_ESCAPE_ROPE              (FOSSIL_ITEMS_END + 22)

// In case you want to add new items of custom usage, all the indexes below this will be updated with the corrected index
#define GENERAL_ITEMS_END             ITEM_ESCAPE_ROPE

// Nectars
#define ITEM_RED_NECTAR               (GENERAL_ITEMS_END + 1)
#define ITEM_YELLOW_NECTAR            (GENERAL_ITEMS_END + 2)
#define ITEM_PINK_NECTAR              (GENERAL_ITEMS_END + 3)
#define ITEM_PURPLE_NECTAR            (GENERAL_ITEMS_END + 4)

// Key Items
#define ITEM_EXPLORER_KIT             (GENERAL_ITEMS_END + 5)
#define ITEM_LUNAR_WING               (GENERAL_ITEMS_END + 6)
#define ITEM_ZYGARDE_CUBE             (GENERAL_ITEMS_END + 7)
#define ITEM_CATCHING_CHARM           (GENERAL_ITEMS_END + 8)
#define ITEM_EXP_CHARM                (GENERAL_ITEMS_END + 9)
#define ITEM_REINS_OF_UNITY           (GENERAL_ITEMS_END + 10)
#define ITEM_TERA_ORB                 (GENERAL_ITEMS_END + 11)
#define ITEM_Z_RING                   (GENERAL_ITEMS_END + 12)
#define ITEM_DYNAMAX_BAND             (GENERAL_ITEMS_END + 13)
#define ITEM_EXP_SHARE                (GENERAL_ITEMS_END + 14)
#define ITEM_REVEAL_GLASS             (GENERAL_ITEMS_END + 15)
#define ITEM_PRISON_BOTTLE            (GENERAL_ITEMS_END + 16)
#define ITEM_GRACIDEA                 (GENERAL_ITEMS_END + 17)
#define ITEM_COIN_CASE                (GENERAL_ITEMS_END + 18)
#define ITEM_ITEMFINDER               (GENERAL_ITEMS_END + 19)
#define ITEM_OLD_ROD                  (GENERAL_ITEMS_END + 20)
#define ITEM_GOOD_ROD                 (GENERAL_ITEMS_END + 21)
#define ITEM_SUPER_ROD                (GENERAL_ITEMS_END + 22)
#define ITEM_SS_TICKET                (GENERAL_ITEMS_END + 23)
#define ITEM_N_SOLARIZER              (GENERAL_ITEMS_END + 24)
#define ITEM_N_LUNARIZER              (GENERAL_ITEMS_END + 25)
#define ITEM_RIDE_PAGER               (GENERAL_ITEMS_END + 26)
#define ITEM_MEGA_BRACELET            (GENERAL_ITEMS_END + 27)
#define ITEM_ROTOM_CATALOG            (GENERAL_ITEMS_END + 28)
#define ITEM_DNA_SPLICERS             (GENERAL_ITEMS_END + 29)
#define ITEM_METEORITE                (GENERAL_ITEMS_END + 30)
#define ITEM_OAKS_PARCEL              (GENERAL_ITEMS_END + 31)
#define ITEM_POKE_FLUTE               (GENERAL_ITEMS_END + 32)
#define ITEM_SECRET_KEY               (GENERAL_ITEMS_END + 33)
#define ITEM_BIKE_VOUCHER             (GENERAL_ITEMS_END + 34)
#define ITEM_GOLD_TEETH               (GENERAL_ITEMS_END + 35)
#define ITEM_CARD_KEY                 (GENERAL_ITEMS_END + 36)
#define ITEM_LIFT_KEY                 (GENERAL_ITEMS_END + 37)
#define ITEM_SILPH_SCOPE              (GENERAL_ITEMS_END + 38)
#define ITEM_BICYCLE                  (GENERAL_ITEMS_END + 39)
#define ITEM_TOWN_MAP                 (GENERAL_ITEMS_END + 40)
#define ITEM_VS_SEEKER                (GENERAL_ITEMS_END + 41)
#define ITEM_FAME_CHECKER             (GENERAL_ITEMS_END + 42)
#define ITEM_TM_CASE                  (GENERAL_ITEMS_END + 43)
#define ITEM_BERRY_POUCH              (GENERAL_ITEMS_END + 44)
#define ITEM_TEACHY_TV                (GENERAL_ITEMS_END + 45)
#define ITEM_TRI_PASS                 (GENERAL_ITEMS_END + 46)
#define ITEM_RAINBOW_PASS             (GENERAL_ITEMS_END + 47)
#define ITEM_TEA                      (GENERAL_ITEMS_END + 48)
#define ITEM_MYSTIC_TICKET            (GENERAL_ITEMS_END + 49)
#define ITEM_AURORA_TICKET            (GENERAL_ITEMS_END + 50)
#define ITEM_RUBY                     (GENERAL_ITEMS_END + 51)
#define ITEM_SAPPHIRE                 (GENERAL_ITEMS_END + 52)
#define ITEM_EV_IV_DISPLAY            (GENERAL_ITEMS_END + 53)
#define ITEM_SHINY_CHARM              (GENERAL_ITEMS_END + 54)
#define ITEM_RAINBOW_WING             (GENERAL_ITEMS_END + 55)
#define ITEM_SILVER_WING              (GENERAL_ITEMS_END + 56)

#define NUM_KEY_ITEMS                 52

// In case you want to add new key items, all the indexes below this will be updated with the corrected index
#define KEY_ITEMS_END                 ITEM_SILVER_WING

// Tm
#define ITEM_TM01                     (KEY_ITEMS_END + 1)
#define ITEM_TM02                     (KEY_ITEMS_END + 2)
#define ITEM_TM03                     (KEY_ITEMS_END + 3)
#define ITEM_TM04                     (KEY_ITEMS_END + 4)
#define ITEM_TM05                     (KEY_ITEMS_END + 5)
#define ITEM_TM06                     (KEY_ITEMS_END + 6)
#define ITEM_TM07                     (KEY_ITEMS_END + 7)
#define ITEM_TM08                     (KEY_ITEMS_END + 8)
#define ITEM_TM09                     (KEY_ITEMS_END + 9)
#define ITEM_TM10                     (KEY_ITEMS_END + 10)
#define ITEM_TM11                     (KEY_ITEMS_END + 11)
#define ITEM_TM12                     (KEY_ITEMS_END + 12)
#define ITEM_TM13                     (KEY_ITEMS_END + 13)
#define ITEM_TM14                     (KEY_ITEMS_END + 14)
#define ITEM_TM15                     (KEY_ITEMS_END + 15)
#define ITEM_TM16                     (KEY_ITEMS_END + 16)
#define ITEM_TM17                     (KEY_ITEMS_END + 17)
#define ITEM_TM18                     (KEY_ITEMS_END + 18)
#define ITEM_TM19                     (KEY_ITEMS_END + 19)
#define ITEM_TM20                     (KEY_ITEMS_END + 20)
#define ITEM_TM21                     (KEY_ITEMS_END + 21)
#define ITEM_TM22                     (KEY_ITEMS_END + 22)
#define ITEM_TM23                     (KEY_ITEMS_END + 23)
#define ITEM_TM24                     (KEY_ITEMS_END + 24)
#define ITEM_TM25                     (KEY_ITEMS_END + 25)
#define ITEM_TM26                     (KEY_ITEMS_END + 26)
#define ITEM_TM27                     (KEY_ITEMS_END + 27)
#define ITEM_TM28                     (KEY_ITEMS_END + 28)
#define ITEM_TM29                     (KEY_ITEMS_END + 29)
#define ITEM_TM30                     (KEY_ITEMS_END + 30)
#define ITEM_TM31                     (KEY_ITEMS_END + 31)
#define ITEM_TM32                     (KEY_ITEMS_END + 32)
#define ITEM_TM33                     (KEY_ITEMS_END + 33)
#define ITEM_TM34                     (KEY_ITEMS_END + 34)
#define ITEM_TM35                     (KEY_ITEMS_END + 35)
#define ITEM_TM36                     (KEY_ITEMS_END + 36)
#define ITEM_TM37                     (KEY_ITEMS_END + 37)
#define ITEM_TM38                     (KEY_ITEMS_END + 38)
#define ITEM_TM39                     (KEY_ITEMS_END + 39)
#define ITEM_TM40                     (KEY_ITEMS_END + 40)
#define ITEM_TM41                     (KEY_ITEMS_END + 41)
#define ITEM_TM42                     (KEY_ITEMS_END + 42)
#define ITEM_TM43                     (KEY_ITEMS_END + 43)
#define ITEM_TM44                     (KEY_ITEMS_END + 44)
#define ITEM_TM45                     (KEY_ITEMS_END + 45)
#define ITEM_TM46                     (KEY_ITEMS_END + 46)
#define ITEM_TM47                     (KEY_ITEMS_END + 47)
#define ITEM_TM48                     (KEY_ITEMS_END + 48)
#define ITEM_TM49                     (KEY_ITEMS_END + 49)
#define ITEM_TM50                     (KEY_ITEMS_END + 50)
#define ITEM_TM51                     (KEY_ITEMS_END + 51)
#define ITEM_TM52                     (KEY_ITEMS_END + 52)
#define ITEM_TM53                     (KEY_ITEMS_END + 53)
#define ITEM_TM54                     (KEY_ITEMS_END + 54)
#define ITEM_TM55                     (KEY_ITEMS_END + 55)
#define ITEM_TM56                     (KEY_ITEMS_END + 56)
#define ITEM_TM57                     (KEY_ITEMS_END + 57)
#define ITEM_TM58                     (KEY_ITEMS_END + 58)
#define ITEM_TM59                     (KEY_ITEMS_END + 59)
#define ITEM_TM60                     (KEY_ITEMS_END + 60)
#define ITEM_TM61                     (KEY_ITEMS_END + 61)
#define ITEM_TM62                     (KEY_ITEMS_END + 62)
#define ITEM_TM63                     (KEY_ITEMS_END + 63)
#define ITEM_TM64                     (KEY_ITEMS_END + 64)
#define ITEM_TM65                     (KEY_ITEMS_END + 65)
#define ITEM_TM66                     (KEY_ITEMS_END + 66)
#define ITEM_TM67                     (KEY_ITEMS_END + 67)
#define ITEM_TM68                     (KEY_ITEMS_END + 68)
#define ITEM_TM69                     (KEY_ITEMS_END + 69)
#define ITEM_TM70                     (KEY_ITEMS_END + 70)
#define ITEM_TM71                     (KEY_ITEMS_END + 71)
#define ITEM_TM72                     (KEY_ITEMS_END + 72)
#define ITEM_TM73                     (KEY_ITEMS_END + 73)
#define ITEM_TM74                     (KEY_ITEMS_END + 74)
#define ITEM_TM75                     (KEY_ITEMS_END + 75)
#define ITEM_TM76                     (KEY_ITEMS_END + 76)
#define ITEM_TM77                     (KEY_ITEMS_END + 77)
#define ITEM_TM78                     (KEY_ITEMS_END + 78)
#define ITEM_TM79                     (KEY_ITEMS_END + 79)
#define ITEM_TM80                     (KEY_ITEMS_END + 80)
#define ITEM_TM81                     (KEY_ITEMS_END + 81)
#define ITEM_TM82                     (KEY_ITEMS_END + 82)
#define ITEM_TM83                     (KEY_ITEMS_END + 83)
#define ITEM_TM84                     (KEY_ITEMS_END + 84)
#define ITEM_TM85                     (KEY_ITEMS_END + 85)
#define ITEM_TM86                     (KEY_ITEMS_END + 86)
#define ITEM_TM87                     (KEY_ITEMS_END + 87)
#define ITEM_TM88                     (KEY_ITEMS_END + 88)
#define ITEM_TM89                     (KEY_ITEMS_END + 89)
#define ITEM_TM90                     (KEY_ITEMS_END + 90)
#define ITEM_TM91                     (KEY_ITEMS_END + 91)
#define ITEM_TM92                     (KEY_ITEMS_END + 92)
#define ITEM_TM93                     (KEY_ITEMS_END + 93)
#define ITEM_TM94                     (KEY_ITEMS_END + 94)
#define ITEM_TM95                     (KEY_ITEMS_END + 95)
#define ITEM_TM96                     (KEY_ITEMS_END + 96)
#define ITEM_TM97                     (KEY_ITEMS_END + 97)
#define ITEM_TM98                     (KEY_ITEMS_END + 98)
#define ITEM_TM99                     (KEY_ITEMS_END + 99)
#define ITEM_TM100                    (KEY_ITEMS_END + 100)

#define ITEMS_COUNT                   (ITEM_TM100 + 1)

#define ITEM_TM01_WORK_UP             ITEM_TM01
#define ITEM_TM02_DRAGON_CLAW         ITEM_TM02
#define ITEM_TM03_PSYSHOCK            ITEM_TM03
#define ITEM_TM04_CALM_MIND           ITEM_TM04
#define ITEM_TM05_ROAR                ITEM_TM05
#define ITEM_TM06_TOXIC               ITEM_TM06
#define ITEM_TM07_HAIL                ITEM_TM07
#define ITEM_TM08_BULK_UP             ITEM_TM08
#define ITEM_TM09_VENOSHOCK           ITEM_TM09
#define ITEM_TM10_HIDDEN_POWER        ITEM_TM10
#define ITEM_TM11_SUNNY_DAY           ITEM_TM11
#define ITEM_TM12_TAUNT               ITEM_TM12
#define ITEM_TM13_ICE_BEAM            ITEM_TM13
#define ITEM_TM14_BLIZZARD            ITEM_TM14
#define ITEM_TM15_HYPER_BEAM          ITEM_TM15
#define ITEM_TM16_LIGHT_SCREEN        ITEM_TM16
#define ITEM_TM17_PROTECT             ITEM_TM17
#define ITEM_TM18_RAIN_DANCE          ITEM_TM18
#define ITEM_TM19_ROOST               ITEM_TM19
#define ITEM_TM20_SAFEGUARD           ITEM_TM20
#define ITEM_TM21_FRUSTRATION         ITEM_TM21
#define ITEM_TM22_SOLAR_BEAM          ITEM_TM22
#define ITEM_TM23_SMACK_DOWN          ITEM_TM23
#define ITEM_TM24_THUNDERBOLT         ITEM_TM24
#define ITEM_TM25_THUNDER             ITEM_TM25
#define ITEM_TM26_EARTHQUAKE          ITEM_TM26
#define ITEM_TM27_RETURN              ITEM_TM27
#define ITEM_TM28_LEECH_LIFE          ITEM_TM28
#define ITEM_TM29_PSYCHIC             ITEM_TM29
#define ITEM_TM30_SHADOW_BALL         ITEM_TM30
#define ITEM_TM31_BRICK_BREAK         ITEM_TM31
#define ITEM_TM32_DOUBLE_TEAM         ITEM_TM32
#define ITEM_TM33_REFLECT             ITEM_TM33
#define ITEM_TM34_SLUDGE_WAVE         ITEM_TM34
#define ITEM_TM35_FLAMETHROWER        ITEM_TM35
#define ITEM_TM36_SLUDGE_BOMB         ITEM_TM36
#define ITEM_TM37_SANDSTORM           ITEM_TM37
#define ITEM_TM38_FIRE_BLAST          ITEM_TM38
#define ITEM_TM39_ROCK_TOMB           ITEM_TM39
#define ITEM_TM40_AERIAL_ACE          ITEM_TM40
#define ITEM_TM41_TORMENT             ITEM_TM41
#define ITEM_TM42_FACADE              ITEM_TM42
#define ITEM_TM43_FLAME_CHARGE        ITEM_TM43
#define ITEM_TM44_REST                ITEM_TM44
#define ITEM_TM45_ATTRACT             ITEM_TM45
#define ITEM_TM46_THIEF               ITEM_TM46
#define ITEM_TM47_LOW_SWEEP           ITEM_TM47
#define ITEM_TM48_ROUND               ITEM_TM48
#define ITEM_TM49_ECHOED_VOICE        ITEM_TM49
#define ITEM_TM50_OVERHEAT            ITEM_TM50
#define ITEM_TM51_STEEL_WING          ITEM_TM51
#define ITEM_TM52_FOCUS_BLAST         ITEM_TM52
#define ITEM_TM53_ENERGY_BALL         ITEM_TM53
#define ITEM_TM54_FALSE_SWIPE         ITEM_TM54
#define ITEM_TM55_SCALD               ITEM_TM55
#define ITEM_TM56_FLING               ITEM_TM56
#define ITEM_TM57_CHARGE_BEAM         ITEM_TM57
#define ITEM_TM58_SKY_DROP            ITEM_TM58
#define ITEM_TM59_BRUTAL_SWING        ITEM_TM59
#define ITEM_TM60_QUASH               ITEM_TM60
#define ITEM_TM61_WILL_O_WISP         ITEM_TM61
#define ITEM_TM62_ACROBATICS          ITEM_TM62
#define ITEM_TM63_EMBARGO             ITEM_TM63
#define ITEM_TM64_EXPLOSION           ITEM_TM64
#define ITEM_TM65_SHADOW_CLAW         ITEM_TM65
#define ITEM_TM66_PAYBACK             ITEM_TM66
#define ITEM_TM67_SMART_STRIKE        ITEM_TM67
#define ITEM_TM68_GIGA_IMPACT         ITEM_TM68
#define ITEM_TM69_ROCK_POLISH         ITEM_TM69
#define ITEM_TM70_AURORA_VEIL         ITEM_TM70
#define ITEM_TM71_STONE_EDGE          ITEM_TM71
#define ITEM_TM72_VOLT_SWITCH         ITEM_TM72
#define ITEM_TM73_THUNDER_WAVE        ITEM_TM73
#define ITEM_TM74_GYRO_BALL           ITEM_TM74
#define ITEM_TM75_SWORDS_DANCE        ITEM_TM75
#define ITEM_TM76_FLY                 ITEM_TM76
#define ITEM_TM77_PSYCH_UP            ITEM_TM77
#define ITEM_TM78_BULLDOZE            ITEM_TM78
#define ITEM_TM79_FROST_BREATH        ITEM_TM79
#define ITEM_TM80_ROCK_SLIDE          ITEM_TM80
#define ITEM_TM81_X_SCISSOR           ITEM_TM81
#define ITEM_TM82_DRAGON_TAIL         ITEM_TM82
#define ITEM_TM83_INFESTATION         ITEM_TM83
#define ITEM_TM84_POISON_JAB          ITEM_TM84
#define ITEM_TM85_DREAM_EATER         ITEM_TM85
#define ITEM_TM86_GRASS_KNOT          ITEM_TM86
#define ITEM_TM87_SWAGGER             ITEM_TM87
#define ITEM_TM88_SLEEP_TALK          ITEM_TM88
#define ITEM_TM89_U_TURN              ITEM_TM89
#define ITEM_TM90_SUBSTITUTE          ITEM_TM90
#define ITEM_TM91_FLASH_CANNON        ITEM_TM91
#define ITEM_TM92_TRICK_ROOM          ITEM_TM92
#define ITEM_TM93_WILD_CHARGE         ITEM_TM93
#define ITEM_TM94_SURF                ITEM_TM94
#define ITEM_TM95_SNARL               ITEM_TM95
#define ITEM_TM96_NATURE_POWER        ITEM_TM96
#define ITEM_TM97_DARK_PULSE          ITEM_TM97
#define ITEM_TM98_WATERFALL           ITEM_TM98
#define ITEM_TM99_DAZZLING_GLEAM      ITEM_TM99
#define ITEM_TM100_CONFIDE            ITEM_TM100

#define NUM_TECHNICAL_MACHINES        100

#define FIRST_BERRY_INDEX             ITEM_CHERI_BERRY
#define ITEM_TO_BERRY(itemId)         (itemId - FIRST_BERRY_INDEX)

#endif  // GUARD_CONSTANTS_ITEMS_H

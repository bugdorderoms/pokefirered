#ifndef GUARD_GLOBAL_H
#define GUARD_GLOBAL_H

#include "config.h"
#include "gba/gba.h"
#include <string.h>
#include "registered_item.h"
#include "constants/global.h"
#include "constants/flags.h"
#include "constants/vars.h"
#include "constants/species.h"
#include "constants/easy_chat.h"
#include "constants/items.h"
#include "constants/rgb.h"

// Prevent cross-jump optimization.
#define BLOCK_CROSS_JUMP asm("");

// to help in decompiling
#define asm_comment(x) asm volatile("@ -- " x " -- ")
#define asm_unified(x) asm(".syntax unified\n" x "\n.syntax divided")

// IDE support
#if defined(__APPLE__) || defined(__CYGWIN__) || defined(__INTELLISENSE__)
// We define these when using certain IDEs to fool preproc
#define _(x)        (x)
#define __(x)       (x)
#define INCBIN(...) {0}
#define INCBIN_U8   INCBIN
#define INCBIN_U16  INCBIN
#define INCBIN_U32  INCBIN
#define INCBIN_S8   INCBIN
#define INCBIN_S16  INCBIN
#define INCBIN_S32  INCBIN
#endif // IDE support

#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

#define SWAP(a, b, temp)    \
{                           \
    temp = a;               \
    a = b;                  \
    b = temp;               \
}

// useful math macros

// Converts a number to Q8.8 fixed-point format
#define Q_8_8(n) ((s16)((n) * 256))

// Converts a number from Q8.8 fixed-point format
#define Q_8_8_TO_INT(n) ((s16)((n) >> 8))

// Converts a number to Q4.12 fixed-point format
#define Q_4_12(n)  ((s16)((n) * 4096))

// Converts a number from Q4.12 fixed-point format
#define Q_4_12_TO_INT(n) ((s16)((n) >> 12))

// Converts a number to QN.S fixed-point format (16-bits)
#define Q_N_S(s, n) ((s16)((n) * (1 << (s))))

// converts a number from QN.S fixed-point format (16-bits)
#define Q_N_S_TO_INT(s, n) ((s16)((n) >> (s)))

// Converts a number to Q24.8 fixed-point format
#define Q_24_8(n) ((s32)((n) << 8))

// Converts a number from Q24.8 fixed-point format
#define Q_24_8_TO_INT(n) ((s32)((n) >> 8))

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) >= (b) ? (a) : (b))

#if MODERN
#define abs(x) (((x) < 0) ? -(x) : (x))
#endif

#define SAFE_DIV(a, b) ((b) ? (a) / (b) : 0)

// Extracts the upper 16 bits of a 32-bit number
#define HIHALF(n) (((n) & 0xFFFF0000) >> 16)

// Extracts the lower 16 bits of a 32-bit number
#define LOHALF(n) ((n) & 0xFFFF)

#define READ_16(ptr)   ((ptr)[0] | ((ptr)[1] << 8))
#define READ_32(ptr)   ((ptr)[0] | ((ptr)[1] << 8) | ((ptr)[2] << 16) | ((ptr)[3] << 24))
#define READ_PTR(ptr)  (u8*) READ_32(ptr)
#define READ_PTR2(ptr) (void*) READ_32(ptr)

/* Expands to the first/second/third/fourth argument. */
#define FIRST(a, ...) a
#define SECOND(a, ...) __VA_OPT__(FIRST(__VA_ARGS__))
#define THIRD(a, ...) __VA_OPT__(SECOND(__VA_ARGS__))
#define FOURTH(a, ...) __VA_OPT__(THIRD(__VA_ARGS__))

/* Picks the xth VA_ARG if it exists, otherwise returns a default value */
#define DEFAULT(_default, ...) FIRST(__VA_OPT__(__VA_ARGS__, ) _default)
#define DEFAULT_2(_default, ...) DEFAULT(_default __VA_OPT__(, SECOND(__VA_ARGS__)))
#define DEFAULT_3(_default, ...) DEFAULT(_default __VA_OPT__(, THIRD(__VA_ARGS__)))
#define DEFAULT_4(_default, ...) DEFAULT(_default __VA_OPT__(, FOURTH(__VA_ARGS__)))

/* Recursive macros.
 * Based on https://www.scs.stanford.edu/~dm/blog/va-opt.html
 *
 * Macros prefixed with R_ are recursive, to correctly expand them the
 * top-level macro which references them should use 'RECURSIVELY' around
 * them. 'RECURSIVELY' cannot be nested, hence the top-level macro must
 * use it so that a recursive macro is able to reference another
 * recursive macro. */

#define RECURSIVELY(...) RECURSIVELY_4(RECURSIVELY_4(RECURSIVELY_4(RECURSIVELY_4(__VA_ARGS__))))
#define RECURSIVELY_4(...) RECURSIVELY_3(RECURSIVELY_3(RECURSIVELY_3(RECURSIVELY_3(__VA_ARGS__))))
#define RECURSIVELY_3(...) RECURSIVELY_2(RECURSIVELY_2(RECURSIVELY_2(RECURSIVELY_2(__VA_ARGS__))))
#define RECURSIVELY_2(...) RECURSIVELY_1(RECURSIVELY_1(RECURSIVELY_1(RECURSIVELY_1(__VA_ARGS__))))
#define RECURSIVELY_1(...) __VA_ARGS__

/* You'll never guess what this one does */
#define APPEND_SEMICOLON(a) a;

/* Concatenates the xth VA_ARG at the end of the string */
#define CONCAT_HELPER(x, y) x ## y
#define CONCAT_(x, y) CONCAT_HELPER(x, y)
#define CONCAT(prefix, name, ...) CONCAT_(CONCAT_(prefix, name), __VA_OPT__(__VA_ARGS__))

/* Useful for deferring expansion until the second scan. See
 * https://www.scs.stanford.edu/~dm/blog/va-opt.html for more info. */
#define PARENS ()

/* Expands to 'macro(a)' for each 'a' in '...' */
#define R_FOR_EACH(macro, ...) __VA_OPT__(R_FOR_EACH_(macro, __VA_ARGS__))
#define R_FOR_EACH_(macro, a, ...) macro(a) __VA_OPT__(R_FOR_EACH_P PARENS (macro, __VA_ARGS__))
#define R_FOR_EACH_P() R_FOR_EACH_

/* (Credit to MGriffin) A rather monstrous way of finding the set bit in a word.
Invalid input causes a compiler error. Sample: https://cexplore.karathan.at/z/x1hm7B */
#define BIT_INDEX(n) \
    (n) == (1 << 0) ? 0 : \
    (n) == (1 << 1) ? 1 : \
    (n) == (1 << 2) ? 2 : \
    (n) == (1 << 3) ? 3 : \
    (n) == (1 << 4) ? 4 : \
    (n) == (1 << 5) ? 5 : \
    (n) == (1 << 6) ? 6 : \
    (n) == (1 << 7) ? 7 : \
    (n) == (1 << 8) ? 8 : \
    (n) == (1 << 9) ? 9 : \
    (n) == (1 << 10) ? 10 : \
    (n) == (1 << 11) ? 11 : \
    (n) == (1 << 12) ? 12 : \
    (n) == (1 << 13) ? 13 : \
    (n) == (1 << 14) ? 14 : \
    (n) == (1 << 15) ? 15 : \
    (n) == (1 << 16) ? 16 : \
    (n) == (1 << 17) ? 17 : \
    (n) == (1 << 18) ? 18 : \
    (n) == (1 << 19) ? 19 : \
    (n) == (1 << 20) ? 20 : \
    (n) == (1 << 21) ? 21 : \
    (n) == (1 << 22) ? 22 : \
    (n) == (1 << 23) ? 23 : \
    (n) == (1 << 24) ? 24 : \
    (n) == (1 << 25) ? 25 : \
    (n) == (1 << 26) ? 26 : \
    (n) == (1 << 27) ? 27 : \
    (n) == (1 << 28) ? 28 : \
    (n) == (1 << 29) ? 29 : \
    (n) == (1 << 30) ? 30 : \
    (n) == (1 << 31) ? 31 : \
    *(u32 *)NULL

#define COMPRESS_BITS_0 0, 1
#define COMPRESS_BITS_1 1, 1
#define COMPRESS_BITS_2 2, 1
#define COMPRESS_BITS_3 3, 1
#define COMPRESS_BITS_4 4, 1
#define COMPRESS_BITS_5 5, 1
#define COMPRESS_BITS_6 6, 1
#define COMPRESS_BITS_7 7, 1

/* Will try and compress a set bit (or up to three sequential bits) into a single byte
Input must be of the form (upper << lower) where upper can be up to 3, lower up to 31 */
#define COMPRESS_BITS(_val) COMPRESS_BITS_STEP_2 _val
#define COMPRESS_BITS_STEP_2(_unpacked) COMPRESS_BITS_STEP_3(COMPRESS_BITS_## _unpacked)
#define COMPRESS_BITS_STEP_3(...) COMPRESS_BITS_STEP_4(__VA_ARGS__)
#define COMPRESS_BITS_STEP_4(upper, lower) (((upper % 8) << 5) + (BIT_INDEX(lower)))

/* Will read a compressed bit stored by COMPRESS_BIT into a single byte */
#define UNCOMPRESS_BITS(compressed) ((compressed >> 5) << (compressed & 0x1F))

// This macro is required to prevent the compiler from optimizing
// a dpad up/down check in sub_812CAD8 (fame_checker.c).
#define TEST_BUTTON(field, button) ({(field) & (button);})
#define JOY_NEW(button)      TEST_BUTTON(gMain.newKeys,  button)
#define JOY_HELD(button)     TEST_BUTTON(gMain.heldKeys, button)
#define JOY_HELD_RAW(button) TEST_BUTTON(gMain.heldKeysRaw, button)
#define JOY_REPT(button)     TEST_BUTTON(gMain.newAndRepeatedKeys, button)

extern u8 gStringVar1[];
extern u8 gStringVar2[];
extern u8 gStringVar3[];
extern u8 gStringVar4[];

#define DIV_ROUND_UP(val, roundBy)(((val) / (roundBy)) + (((val) % (roundBy)) ? 1 : 0))

#define ROUND_BITS_TO_BYTES(numBits) DIV_ROUND_UP(numBits, 8)

#define DEX_FLAGS_NO ROUND_BITS_TO_BYTES(NUM_SPECIES)

// Converts a string to a compound literal, essentially making it a pointer to const u8
#define COMPOUND_STRING(str) (const u8[]) _(str)

// This produces an error at compile-time if expr is zero.
// It looks like file.c:line: size of array `id' is negative
#define STATIC_ASSERT(expr, id) typedef char id[(expr) ? 1 : -1];

struct Coords8
{
    s8 x;
    s8 y;
};

struct UCoords8
{
    u8 x;
    u8 y;
};

struct Coords16
{
    s16 x;
    s16 y;
};

struct UCoords16
{
    u16 x;
    u16 y;
};

struct Coords32
{
    s32 x;
    s32 y;
};

struct UCoords32
{
    u32 x;
    u32 y;
};

struct Pokedex
{
	u32 spindaPersonality; // set when you first see Spinda
    u8 order;
    u8 mode;
    u8 owned[DEX_FLAGS_NO];
    u8 seen[DEX_FLAGS_NO];
};

struct PokemonJumpRecords
{
    u16 jumpsInRow;
    u16 excellentsInRow;
	u32 bestJumpScore;
    u16 gamesWithMaxPlayers;
};

struct BerryPickingResults // possibly used in the game itself? Size may be wrong as well
{
    u32 bestScore;
    u16 berriesPicked;
    u16 berriesPickedInRow;
    u8 field_8;
    u8 field_9;
    u8 field_A;
    u8 field_B;
    u8 field_C;
    u8 field_D;
    u8 field_E;
    u8 field_F;
};

#define LINK_B_RECORDS_COUNT 5

struct LinkBattleRecord
{
    u8 name[PLAYER_NAME_LENGTH + 1];
    u16 trainerId;
    u16 wins;
    u16 losses;
    u16 draws;
};

struct LinkBattleRecords
{
    struct LinkBattleRecord entries[LINK_B_RECORDS_COUNT];
    u8 languages[LINK_B_RECORDS_COUNT];
};

struct DailyEvent
{
	u32 minute:6;
	u32 hour:5;
	u32 day:5;
	u32 month:4;
	u32 year:7;
	u32 century:5;
};

#include "constants/game_stat.h"
#include "global.fieldmap.h"
#include "pokemon.h"

struct SaveBlock2
{
    /*0x000*/ u8 playerName[PLAYER_NAME_LENGTH + 1];
    /*0x008*/ u8 playerGender; // MALE, FEMALE
    /*0x009*/ u8 specialSaveWarpFlags;
    /*0x00A*/ u8 playerTrainerId[TRAINER_ID_LENGTH];
    /*0x00E*/ u16 playTimeHours;
	/*0x010*/ u32 optionsTextSpeed:3; // OPTIONS_TEXT_SPEED_[SLOW/MID/FAST]
	/*0x010*/ u32 optionsWindowFrameType:5; // Specifies one of the 20 decorative borders for text boxes
	/*0x010*/ u32 optionsSound:1; // OPTIONS_SOUND_[MONO/STEREO]
	/*0x010*/ u32 optionsBattleStyle:1; // OPTIONS_BATTLE_STYLE_[SHIFT/SET]
	/*0x010*/ u32 optionsBattleSceneOff:1; // whether battle animations are disabled
	/*0x010*/ u32 optionsSkipPkmnNickname:1; // whether pokemon nickname prompt are disabled
	/*0x010*/ u32 optionsDexnavSearchOnR:1; // swap dexnav search and auto run button functions
	/*0x010*/ u32 expShare:1; // whether exp share is on
	/*0x010*/ u32 autoRun:1; // whether auto run is on
	/*0x010*/ u32 waitingTaurosChargeStamina:1;
	/*0x010*/ u32 unused:16;
	/*0x014*/ u32 gcnLinkFlags; // Read by Pokemon Colosseum/XD
	/*0x018*/ u32 encryptionKey;
	/*0x01C*/ u16 mapView[0x100];
	/*0x21C*/ u8 playTimeMinutes;
	/*0x21D*/ u8 playTimeSeconds;
	/*0x21E*/ u8 playTimeVBlanks;
	/*0x21F*/ u8 taurosChargeStamina;
	          // from here to down are the structs and data that can be vary in size
	          u8 itemFlags[ROUND_BITS_TO_BYTES(ITEMS_COUNT)];
              struct Pokedex pokedex;
              struct LinkBattleRecords linkBattleRecords;
              struct PokemonJumpRecords pokeJump;
              struct BerryPickingResults berryPick;
			  struct DailyEvent dailyEvent;
};

extern struct SaveBlock2 *gSaveBlock2Ptr;

struct WarpData
{
    s8 mapGroup;
    s8 mapNum;
    s8 warpId;
    s16 x, y;
};

struct ItemSlot
{
    u16 itemId;
    u16 quantity;
};

struct Roamer
{
    /*0x00*/ u32 ivs;
    /*0x04*/ u32 personality;
    /*0x08*/ u16 species;
    /*0x0A*/ u16 hp;
    /*0x0C*/ u8 level;
    /*0x0D*/ struct Status1 status;
    /*0x0E*/ bool8 active;
};

struct RamScriptData
{
    u8 magic;
    u8 mapGroup;
    u8 mapNum;
    u8 objectId;
    u8 script[995];
};

struct RamScript
{
    u32 checksum;
    struct RamScriptData data;
};

struct Mail
{
    /*0x00*/ u16 words[MAIL_WORDS_COUNT];
    /*0x12*/ u8 playerName[PLAYER_NAME_LENGTH + 1];
    /*0x1A*/ u8 trainerId[TRAINER_ID_LENGTH];
    /*0x1E*/ u16 species;
    /*0x20*/ u16 itemId;
};

struct DayCareMail
{
    struct Mail message;
    u8 OT_name[PLAYER_NAME_LENGTH + 1];
    u8 monName[POKEMON_NAME_LENGTH + 1];
    u8 gameLanguage:4;
    u8 monLanguage:4;
};

struct DaycareMon
{
    struct BoxPokemon mon;
    struct DayCareMail mail;
    u32 steps;
};

struct DayCare
{
    struct DaycareMon mons[DAYCARE_MON_COUNT];
    u32 offspringPersonality;
    u8 stepCounter;
	bool8 pending;
};

#include "fame_checker.h"

struct FameCheckerSaveData
{
    /*3a54*/ u16 pickState:2;
    u16 flavorTextFlags:12;
    u16 unk_0_E:2;
};

struct TrainerNameRecord
{
    u32 trainerId;
    u8 trainerName[PLAYER_NAME_LENGTH + 1];
};

enum
{
	FUSION_RESHIRAN,
	FUSION_ZEKROM,
	FUSION_SOLGALEO,
	FUSION_LUNALA,
	FUSION_GLASTRIER,
	FUSION_SPECTRIER,
	FUSIONS_COUNT,
};

struct SaveBlock1
{
    /*0x0000*/ struct Coords16 pos;
    /*0x0004*/ struct WarpData location;
    /*0x000C*/ struct WarpData continueGameWarp;
    /*0x0014*/ struct WarpData dynamicWarp;
    /*0x001C*/ struct WarpData lastHealLocation;
    /*0x0024*/ struct WarpData escapeWarp;
    /*0x002C*/ u16 savedMusic;
	/*0x002E*/ u8 weatherDelay;
    /*0x002F*/ u8 weather;
    /*0x0030*/ u16 mapLayoutId;
	/*0x0032*/ u16 coins;
	/*0x0034*/ u32 money;
	/*0x0038*/ u16 registeredItem[REGISTERED_ITEMS_COUNT]; // registered for use with SELECT button
	/*0x0040*/ u32 gameStats[NUM_GAME_STATS];
	/*0x0080*/ u16 trainerRematchStepCounter;
	/*0x0082*/ u16 easyChatProfile[EASY_CHAT_BATTLE_WORDS_COUNT];
	/*0x008E*/ u8 trainerRematches[MAX_REMATCH_ENTRIES];
	/*0x00F2*/ u8 additionalPhrases[ROUND_BITS_TO_BYTES(NUM_ADDITIONAL_PHRASES)];
	/*0x00F7*/ u8 playerPartyCount;
	/*0x00F8*/ u8 rivalName[PLAYER_NAME_LENGTH + 1];
	/*0x0100*/ u8 registeredTexts[UNION_ROOM_KB_ROW_COUNT][21];
	/*0x01D2*/ u8 flashLevel;
	           u8 flags[ROUND_BITS_TO_BYTES(FLAGS_COUNT)]; // size of 0x120 bytes
			   u16 vars[VARS_COUNT]; // size of 0x200 bytes
			   struct Pokemon playerParty[PARTY_SIZE];
			   struct ItemSlot pcItems[PC_ITEMS_COUNT];
			   struct ItemSlot bagPocket_Items[BAG_ITEMS_COUNT];
			   struct ItemSlot bagPocket_KeyItems[BAG_KEYITEMS_COUNT];
			   struct ItemSlot bagPocket_PokeBalls[BAG_POKEBALLS_COUNT];
			   struct ItemSlot bagPocket_TMHM[BAG_TMHM_COUNT];
			   struct ItemSlot bagPocket_Berries[BAG_BERRIES_COUNT];
			   struct ObjectEvent objectEvents[OBJECT_EVENTS_COUNT];
			   struct ObjectEventTemplate objectEventTemplates[OBJECT_EVENT_TEMPLATES_COUNT];
			   struct Mail mail[MAIL_COUNT];
			   struct DayCare daycare;
			   struct Roamer roamer;
			   struct RamScript ramScript;
			   struct FameCheckerSaveData fameChecker[NUM_FAMECHECKER_PERSONS];
			   struct TrainerNameRecord trainerNameRecords[20];
			   struct DaycareMon route5DayCareMon;
			   struct Pokemon fusions[FUSIONS_COUNT];
};

struct MapPosition
{
    s16 x;
    s16 y;
    s8 height;
};

extern struct SaveBlock1* gSaveBlock1Ptr;
extern u8 gReservedSpritePaletteCount;

#endif // GUARD_GLOBAL_H

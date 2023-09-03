#ifndef GUARD_GLOBAL_H
#define GUARD_GLOBAL_H

#include "config.h"
#include "gba/gba.h"
#include <string.h>
#include "registered_item.h"
#include "constants/global.h"
#include "constants/flags.h"
#include "constants/vars.h"
#include "constants/pokedex.h"
#include "constants/species.h"
#include "constants/easy_chat.h"
#include "constants/items.h"

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

// Alias of ARRAY_COUNT using GameFreak's name from AgbAssert calls.
#define NELEMS(array) ARRAY_COUNT(array)

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
#define NUM_FLAG_BYTES ROUND_BITS_TO_BYTES(FLAGS_COUNT)

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

struct Time
{
    /*0x00*/ s16 days;
    /*0x02*/ s8 hours;
    /*0x03*/ s8 minutes;
    /*0x04*/ s8 seconds;
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

#include "constants/game_stat.h"
#include "global.fieldmap.h"
#include "pokemon.h"

struct BattleTowerRecord // record mixing
{
    /*0x00*/ u8 battleTowerLevelType; // 0 = level 50, 1 = level 100
    /*0x01*/ u8 trainerClass;
    /*0x02*/ u16 winStreak;
    /*0x04*/ u8 name[PLAYER_NAME_LENGTH + 1];
    /*0x0C*/ u8 trainerId[TRAINER_ID_LENGTH];
    /*0x10*/ u16 greeting[EASY_CHAT_BATTLE_WORDS_COUNT];
    /*0x1C*/ struct BattleTowerPokemon party[3];
    /*0xA0*/ u32 checksum;
};

struct BattleTowerEReaderTrainer
{
    /*0x4A0 0x3F0 0x00*/ u8 unk0;
    /*0x4A1 0x3F1 0x01*/ u8 trainerClass;
    /*0x4A2 0x3F2 0x02*/ u16 winStreak;
    /*0x4A4 0x3F4 0x04*/ u8 name[8];
    /*0x4AC 0x3FC 0x0C*/ u8 trainerId[4];
    /*0x4B0 0x400 0x10*/ u16 greeting[6];
    /*0x4BC 0x40C 0x1C*/ u16 farewellPlayerLost[6];
    /*0x4C8 0x418 0x28*/ u16 farewellPlayerWon[6];
    /*0x4D4 0x424 0x34*/ struct BattleTowerPokemon party[3];
    /*0x558 0x4A8 0xB8*/ u32 checksum;
};

struct BattleTowerData // Leftover from R/S
{
    /*0x0000, 0x00B0*/ struct BattleTowerRecord playerRecord;
    /*0x00A4, 0x0154*/ struct BattleTowerRecord records[5]; // from record mixing
    /*0x03D8, 0x0488*/ u16 firstMonSpecies; // species of the first pokemon in the player's battle tower party
    /*0x03DA, 0x048A*/ u16 defeatedBySpecies; // species of the pokemon that defated the player
    /*0x03DC, 0x048C*/ u8 defeatedByTrainerName[8];
    /*0x03E4, 0x0494*/ u8 firstMonNickname[POKEMON_NAME_LENGTH]; // nickname of the first pokemon in the player's battle tower party
    /*0x03F0, 0x04A0*/ struct BattleTowerEReaderTrainer ereaderTrainer;
    /*0x04AC, 0x055C*/ u8 battleTowerLevelType:1; // 0 = level 50; 1 = level 100
    /*0x04AC, 0x055C*/ u8 lastStreakLevelType:1; // 0 = level 50, 1 = level 100.  level type of the last streak. Used by tv to report the level mode.
    /*0x04AC, 0x055C*/ u8 unk_554:1;
    /*0x04AD, 0x055D*/ u8 battleOutcome;
    /*0x04AE, 0x055E*/ u8 var_4AE[2];
    /*0x04B0, 0x0560*/ u16 curChallengeBattleNum[2]; // 1-based index of battle in the current challenge. (challenges consist of 7 battles)
    /*0x04B4, 0x0564*/ u16 curStreakChallengesNum[2]; // 1-based index of the current challenge in the current streak.
    /*0x04B8, 0x0568*/ u16 recordWinStreaks[2];
    /*0x04BC, 0x056C*/ u8 battleTowerTrainerId; // index for gBattleTowerTrainers table
    /*0x04BD, 0x056D*/ u8 selectedPartyMons[0x3]; // indices of the 3 selected player party mons.
    /*0x04C0, 0x0570*/ u16 prizeItem;
    /*0x04C2, 0x0572*/ u8 battledTrainerIds[6];
    /*0x04C8, 0x0578*/ u16 totalBattleTowerWins;
    /*0x04CA, 0x057A*/ u16 bestBattleTowerWinStreak;
    /*0x04CC, 0x057C*/ u16 currentWinStreaks[2];
}; /* size = 0x7E8 */

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
	/*0x010*/ u32 expShare:1; // whether exp share is on
	/*0x010*/ u32 autoRun:1; // whether auto run is on
	/*0x010*/ u32 unused:19;
	/*0x014*/ u32 gcnLinkFlags; // Read by Pokemon Colosseum/XD
	/*0x018*/ u32 encryptionKey;
	/*0x01C*/ u16 mapView[0x100];
	/*0x21C*/ u8 playTimeMinutes;
	/*0x21D*/ u8 playTimeSeconds;
	/*0x21E*/ u8 playTimeVBlanks;
	          // from here to down are the structs and data that can be vary in size
	          u8 itemFlags[ROUND_BITS_TO_BYTES(ITEMS_COUNT)];
              struct Pokedex pokedex;
              struct Time localTimeOffset;
              struct Time lastBerryTreeUpdate;
              struct BattleTowerData battleTower;
              struct LinkBattleRecords linkBattleRecords;
              struct PokemonJumpRecords pokeJump;
              struct BerryPickingResults berryPick;
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
    /*0x0D*/ u8 status;
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
    u16 offspringPersonality;
    u8 stepCounter;
};

struct QuestLogNPCData
{
    u32 x:8;
    u32 negx:1;
    u32 y:8;
    u32 negy:1;
    u32 elevation:6;
    u32 movementType:8;
};

struct QuestLogObjectEvent
{
    /*0x00*/ u8 active:1;
    /*0x00*/ u8 triggerGroundEffectsOnStop:1;
    /*0x00*/ u8 disableCoveringGroundEffects:1;
    /*0x00*/ u8 landingJump:1;
    /*0x00*/ u8 frozen:1;
    /*0x00*/ u8 facingDirectionLocked:1;
    /*0x00*/ u8 disableAnim:1;
    /*0x00*/ u8 enableAnim:1;
    /*0x01*/ u8 inanimate:1;
    /*0x01*/ u8 invisible:1;
    /*0x01*/ u8 offScreen:1;
    /*0x01*/ u8 trackedByCamera:1;
    /*0x01*/ u8 isPlayer:1;
    /*0x01*/ u8 spriteAnimPausedBackup:1;
    /*0x01*/ u8 spriteAffineAnimPausedBackup:1;
    /*0x01*/ u8 disableJumpLandingGroundEffect:1;
    /*0x02*/ u8 fixedPriority:1;
    /*0x02*/ u8 mapobj_unk_18:4;
    /*0x02*/ u8 unused_02_5:3;
    /*0x03*/ u8 mapobj_unk_0B_0:4;
    /*0x03*/ u8 elevation:4;
    /*0x04*/ u8 graphicsId;
    /*0x05*/ u8 animPattern;
    /*0x06*/ u8 trainerType;
    /*0x07*/ u8 localId;
    /*0x08*/ u8 mapNum;
    /*0x09*/ u8 mapGroup;
    /*0x0a*/ s16 x;
    /*0x0c*/ s16 y;
    /*0x0e*/ u8 trainerRange_berryTreeId;
    /*0x0f*/ u8 previousMetatileBehavior;
    /*0x10*/ u8 directionSequenceIndex;
    /*0x11*/ u8 animId;
};

struct QuestLog
{
    /*0x0000*/ u8 startType;
    /*0x0001*/ u8 mapGroup;
    /*0x0002*/ u8 mapNum;
    /*0x0003*/ u8 warpId;
    /*0x0004*/ s16 x;
    /*0x0006*/ s16 y;
    /*0x0008*/ struct QuestLogObjectEvent unk_008[OBJECT_EVENTS_COUNT];

    // These arrays hold the game state for
    // playing back the quest log
    /*0x0148*/ u8 flags[NUM_FLAG_BYTES];
    /*0x02c8*/ u16 vars[VARS_COUNT];
    /*0x0468*/ struct QuestLogNPCData npcData[64];
    /*0x0568*/ u16 script[128];
    /*0x0668*/ u16 end[0];
};

#include "fame_checker.h"

struct FameCheckerSaveData
{
    /*3a54*/ u16 pickState:2;
    u16 flavorTextFlags:12;
    u16 unk_0_E:2;
};

struct WonderNewsMetadata
{
    u8 unk_0_0:2;
    u8 unk_0_2:3;
    u8 unk_0_5:3;
    u8 berry;
};

struct WonderNews
{
    u16 newsId;
    u8 sendType; // SEND_TYPE_*
    u8 bgType;
    u8 titleText[WONDER_NEWS_TEXT_LENGTH];
    u8 bodyText[WONDER_NEWS_BODY_TEXT_LINES][WONDER_NEWS_TEXT_LENGTH];
};

struct WonderCard
{
    u16 flagId; // Event flag (sReceivedGiftFlags) + WONDER_CARD_FLAG_OFFSET
    u16 iconSpecies;
    u32 idNumber;
    u8 type:2; // CARD_TYPE_*
    u8 bgType:4;
    u8 sendType:2; // SEND_TYPE_*
    u8 maxStamps;
    u8 titleText[WONDER_CARD_TEXT_LENGTH];
    u8 subtitleText[WONDER_CARD_TEXT_LENGTH];
    u8 bodyText[WONDER_CARD_BODY_TEXT_LINES][WONDER_CARD_TEXT_LENGTH];
    u8 footerLine1Text[WONDER_CARD_TEXT_LENGTH];
    u8 footerLine2Text[WONDER_CARD_TEXT_LENGTH];
};

struct WonderCardMetadata
{
    u16 battlesWon;
    u16 battlesLost;
    u16 numTrades;
    u16 iconSpecies;
    u16 stampData[2][MAX_STAMP_CARD_STAMPS]; // First element is STAMP_SPECIES, second is STAMP_ID
};

struct MysteryGiftSave
{
    u32 newsCrc;
    struct WonderNews news;
    u32 cardCrc;
    struct WonderCard card;
    u32 cardMetadataCrc;
    struct WonderCardMetadata cardMetadata;
    u16 questionnaireWords[NUM_QUESTIONNAIRE_WORDS];
    struct WonderNewsMetadata newsMetadata;
    u32 trainerIds[2][5]; // Saved ids for 10 trainers, 5 each for battles and trades
}; // 0x36C 0x348C

struct TrainerTower
{
    u32 timer;
    u32 bestTime;
    u8 floorsCleared;
    u8 unk9;
    bool8 receivedPrize:1;
    bool8 checkedFinalTime:1;
    bool8 spokeToOwner:1;
    bool8 hasLost:1;
    bool8 unkA_4:1;
    bool8 validated:1;
};

struct TrainerNameRecord
{
    u32 trainerId;
    u8 trainerName[PLAYER_NAME_LENGTH + 1];
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
    /*0x002E*/ u8 weather;
    /*0x002F*/ u8 flashLevel;
    /*0x0030*/ u16 mapLayoutId;
	/*0x0032*/ u16 coins;
	/*0x0034*/ u32 money;
	/*0x0038*/ u16 registeredItem[REGISTERED_ITEMS_COUNT]; // registered for use with SELECT button
	/*0x0040*/ u32 gameStats[NUM_GAME_STATS];
	/*0x0080*/ u16 trainerRematchStepCounter;
	/*0x0082*/ u16 easyChatProfile[EASY_CHAT_BATTLE_WORDS_COUNT];
	/*0x008E*/ u16 easyChatBattleStart[EASY_CHAT_BATTLE_WORDS_COUNT];
	/*0x009A*/ u16 easyChatBattleWon[EASY_CHAT_BATTLE_WORDS_COUNT];
	/*0x00A6*/ u16 easyChatBattleLost[EASY_CHAT_BATTLE_WORDS_COUNT];
	/*0x00B2*/ u8 trainerRematches[MAX_REMATCH_ENTRIES];
	/*0x0116*/ u8 additionalPhrases[ROUND_BITS_TO_BYTES(NUM_ADDITIONAL_PHRASES)];
	/*0x011B*/ u8 playerPartyCount;
	/*0x011C*/ u32 towerChallengeId;
	/*0x0120*/ u8 rivalName[PLAYER_NAME_LENGTH + 1];
	/*0x0128*/ u8 registeredTexts[UNION_ROOM_KB_ROW_COUNT][21];
	           u8 flags[NUM_FLAG_BYTES]; // size of 0x120 bytes
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
			   struct QuestLog questLog[QUEST_LOG_SCENE_COUNT];
			   struct Mail mail[MAIL_COUNT];
			   struct DayCare daycare;
			   struct Roamer roamer;
			   struct MysteryGiftSave mysteryGift;
			   struct RamScript ramScript;
			   struct FameCheckerSaveData fameChecker[NUM_FAMECHECKER_PERSONS];
			   struct TrainerNameRecord trainerNameRecords[20];
			   struct DaycareMon route5DayCareMon;
			   struct TrainerTower trainerTower[NUM_TOWER_CHALLENGE_TYPES];
			   struct Pokemon fusedReshiram;
			   struct Pokemon fusedZekrom;
			   struct Pokemon fusedSolgaleo;
			   struct Pokemon fusedLunala;
			   struct Pokemon fusedGlastrier;
			   struct Pokemon fusedSpectrier;
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

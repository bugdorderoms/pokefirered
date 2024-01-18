#ifndef GUARD_POKEMON_H
#define GUARD_POKEMON_H

#include "global.h"
#include "sprite.h"
#include "battle_main.h"
#include "constants/pokemon.h"
#include "pokemon_storage_system.h"

struct BoxPokemon
{
    /*0x00*/ u32 personality;
    /*0x04*/ u32 otId;
    /*0x08*/ u8 nickname[POKEMON_NAME_LENGTH];
    /*0x12*/ u8 otName[PLAYER_NAME_LENGTH];
	/*0x19*/ u8 friendship;
	/*0x1A*/ u8 metLocation;
	/*0x1B*/ u8 ppBonuses;
	/*0x1C*/ u32 species:11; // Up to 2047 species
			 u32 metLevel:7;
			 u32 metGame:4;
			 u32 otGender:1;
			 u32 language:3;
			 u32 isEgg:1;
			 u32 nature:5; // 25 natures
	/*0x20*/ u32 experience:21;
			 u32 heldItem:11; // Up to 2047 items
	/*0x24*/ u32 hpIV:5;
			 u32 attackIV:5;
			 u32 defenseIV:5;
			 u32 speedIV:5;
			 u32 spAttackIV:5;
			 u32 spDefenseIV:5;
			 u32 abilityNum:1;
			 u32 abilityHidden:1;
	/*0x28*/ u32 pp1:7; // Max 127 pp
			 u32 pp2:7; // Max 127 pp
			 u32 pp3:7; // Max 127 pp
			 u32 pp4:7; // Max 127 pp
			 u32 markings:4;
	/*0x2C*/ u32 pokerus:4;
			 u32 pokeball:6; // Up to 63 balls
			 u32 move1:11; // Up to 2047 moves
			 u32 move2:11; // Up to 2047 moves
	/*0x30*/ u32 move3:11; // Up to 2047 moves
			 u32 move4:11; // Up to 2047 moves
			 u32 dynamaxLevel:4;
			 u32 gMaxFactor:1;
			 u32 teraType:5; // 19 types
	/*0x34*/ u8 hpEV;
	/*0x35*/ u8 attackEV;
	/*0x36*/ u8 defenseEV;
	/*0x37*/ u8 speedEV;
	/*0x38*/ u8 spAttackEV;
	/*0x39*/ u8 spDefenseEV;
	/*0x3A*/ u8 shiny:1;
			 u8 unused:7;
};

struct Pokemon
{
    struct BoxPokemon box;
    u32 status;
    u8 level;
    u8 mail;
    u16 hp;
    u16 maxHP;
    u16 attack;
    u16 defense;
    u16 speed;
    u16 spAttack;
    u16 spDefense;
};

struct PokemonStorage
{
    u8 currentBox;
    struct BoxPokemon boxes[TOTAL_BOXES_COUNT][IN_BOX_COUNT];
    u8 boxNames[TOTAL_BOXES_COUNT][BOX_NAME_LENGTH + 1];
    u8 boxWallpapers[TOTAL_BOXES_COUNT];
};

struct BattleTowerPokemon
{
    /*0x00*/ u16 species;
    /*0x02*/ u16 heldItem;
    /*0x04*/ u16 moves[4];
    /*0x0C*/ u8 level;
    /*0x0D*/ u8 ppBonuses;
    /*0x0E*/ u8 hpEV;
    /*0x0F*/ u8 attackEV;
    /*0x10*/ u8 defenseEV;
    /*0x11*/ u8 speedEV;
    /*0x12*/ u8 spAttackEV;
    /*0x13*/ u8 spDefenseEV;
    /*0x14*/ u32 otId;
    /*0x18*/ u32 hpIV:5;
             u32 attackIV:5;
             u32 defenseIV:5;
             u32 speedIV:5;
             u32 spAttackIV:5;
             u32 spDefenseIV:5;
             u32 abilityHidden:1;
             u32 abilityNum:1;
    /*0x1C*/ u32 personality;
    /*0x20*/ u8 nickname[11];
    /*0x2B*/ u8 friendship;
};

struct BattlePokemon
{
    /*0x00*/ u16 species;
    /*0x02*/ u16 attack;
    /*0x04*/ u16 defense;
    /*0x06*/ u16 speed;
    /*0x08*/ u16 spAttack;
    /*0x0A*/ u16 spDefense;
    /*0x0C*/ u16 moves[MAX_MON_MOVES];
    /*0x14*/ u32 hpIV:5;
    /*0x14*/ u32 attackIV:5;
    /*0x15*/ u32 defenseIV:5;
    /*0x15*/ u32 speedIV:5;
    /*0x16*/ u32 spAttackIV:5;
    /*0x17*/ u32 spDefenseIV:5;
    /*0x17*/ u32 abilityHidden:1;
    /*0x17*/ u32 abilityNum:1;
    /*0x18*/ s8 statStages[NUM_BATTLE_STATS];
    /*0x20*/ u16 ability;
    /*0x22*/ u8 type1;
    /*0x23*/ u8 type2;
    /*0x24*/ u8 type3; // For Trick-or-Treat and Forest's Curse
	/*0x25*/ u8 pp[MAX_MON_MOVES]; // From here to up are all copied by transform
    /*0x29*/ u8 level;
    /*0x2A*/ u16 item;
    /*0x2C*/ u16 hp;
    /*0x2E*/ u16 maxHP;
    /*0x30*/ u8 nickname[POKEMON_NAME_LENGTH + 1];
    /*0x3B*/ u8 ppBonuses;
    /*0x3C*/ u8 otName[8];
    /*0x44*/ u32 experience;
    /*0x48*/ u32 personality;
    /*0x4C*/ u32 status1;
    /*0x50*/ u32 status2;
    /*0x54*/ u32 otId;
    /*0x58*/ u8 friendship;
    /*0x59*/ u8 unknown;
};

struct BaseStats
{
 /* 0x00 */ u8 baseHP;
 /* 0x01 */ u8 baseAttack;
 /* 0x02 */ u8 baseDefense;
 /* 0x03 */ u8 baseSpeed;
 /* 0x04 */ u8 baseSpAttack;
 /* 0x05 */ u8 baseSpDefense;
 /* 0x06 */ u8 type1;
 /* 0x07 */ u8 type2;
 /* 0x08 */ u8 catchRate;
 /* 0x09 */ u16 expYield;
 /* 0x0A */ u16 evYield_HP:2;
 /* 0x0A */ u16 evYield_Attack:2;
 /* 0x0A */ u16 evYield_Defense:2;
 /* 0x0A */ u16 evYield_Speed:2;
 /* 0x0B */ u16 evYield_SpAttack:2;
 /* 0x0B */ u16 evYield_SpDefense:2;
 /* 0x0C */ u16 item1;
 /* 0x0E */ u16 item2;
 /* 0x10 */ u8 genderRatio;
 /* 0x11 */ u8 eggCycles;
 /* 0x12 */ u8 friendship;
 /* 0x13 */ u8 growthRate;
 /* 0x14 */ u8 eggGroup1;
 /* 0x15 */ u8 eggGroup2;
 /* 0x16 */ u16 abilities[2];
 /* 0x1A */ u8 safariZoneFleeRate;
 /* 0x1B */ u8 bodyColor:7;
            u8 noFlip:1;
 /* 0x1C */ u16 hiddenAbility;
};

struct MoveFlags
{
	u32 makesContact:1; // A move with this flag will makes contact.
	u32 danceMove:1; // A move with this flag can be affected by Dancer.
	u32 magicCoatAffected:1; // A move with this flag can be reflected by Magic Coat.
	u32 snatchAffected:1; // A move with this flag can be stealed by Snatch.
	u32 thawUser:1; // A move with this flag will thaw the user when used.
	u32 kingsRockAffected:1; // A move with this flag can be affected by Kings Rock.
	u32 highCritChance:1; // A move with this flag have a high chance to be critical.
	u32 forbiddenProtect:1; // A move with this flag will ignore the target's protect effects.
	// end of byte
	u32 punchMove:1; // A move with this flag can be affected by Iron Fist.
	u32 twoTurnsMove:1; // A move with this flag will be considered a Two turns move.
	u32 bitingMove:1; // A move with this flag can be affected by Strong Jaw.
	u32 pulseMove:1; // A move with this flag can be affected by Mega Launcher.
	u32 targetStatStagesIgnored:1; // A move with this flag will ignore the target Defense and Evasion stat stages.
	u32 forbiddenMimic:1; // A move with this flag can't be copied by Mimic.
	u32 hitUnderground:1; // If target is undergrounded, can hit and deal double damage.
	u32 hitUnderwater:1; // If target is in underwater, can hit and deal double damage.
	// end of byte
	u32 soundMove:1; // A move with this flag will makes sound.
	u32 ballisticMove:1; // A move with this flag will be blocked by Bulletproof.
	u32 protectionMove:1; // A move with this flag can set a protect like effect.
	u32 powderMove:1; // A move with this flag can be blocked by Overcoat, etc.
	u32 ignoreAbilities:1; // A move with this flag will ignore all ignoreable abilities.
	u32 dmgMinimize:1; // A move with this flag will deal double damage and aways hit the target if it was used Minimize.
	u32 hitInAir:1; // If target is in the air, can hit.
	u32 hitInAirDoubleDmg:1; // If target is in the air, can hit and deal double damage.
	// end of byte
	u32 makeGrounded:1; // A move with this flag makes Ground type moves do 1x damage to flying and levitating targets.
	u32 hitSubstitute:1; // A move with this flag will ignore the target's Substitute.
	u32 slicingMove:1; // A move with this flag can be affected by Sharpness.
	u32 windMove:1; // A move with this flag can be affected by Wind Rider, etc.
	u32 forbiddenParentalBond:1; // A move with this flag can't become a multi-hit move due to Parental Bond.
	u32 forbiddenMirrorMove:1; // A move with this flag can't be affected by Mirror Move.
	u32 forbiddenMetronome:1; // A move with this flag can't be called by Metronome.
	u32 forbiddenAssist:1; // A move with this flag can't be executed by Assist.
	// end of byte
	u32 forbiddenSleepTalk:1; // A move with this flag can't be executed by Sleep Talk.
	u32 forbiddenCopycat:1; // A move with this flag can't be copied by Copycat.
	u32 forbiddenInstruct:1; // A move with this flag can't be executed by Instruct.
	u32 affectsUserSide:1; // Tipycally used to show the entire side on choose the move, but it's still used to determine protect like effects.
	u32 strikeCount:4; // Max 15 hits. Defaults to 1 if not set. May apply its effect on each hit.
	u32 callOtherMove:1; // A move with this flag will be considered a "Move that call other moves".
	u32 noEffectiveness:1; // A move with this flag will not display the "Its super effective" and "Its not very effective" message.
	u32 recoilDivisor:3; // Used by moves that have a recoil damage, also if set, its affected by Reckless.
	u32 forcePressure:1; // A move with this flag will force the extra PP consuption from Pressure.
	u32 gravityBanned:1; // A move with this flag can't be selected while gravity is in effect.
	u32 unused:17;
};

struct BattleMove
{
	/*0x00*/ u8 name[MOVE_NAME_LENGTH + 1];
    /*0x14*/ u16 effect;
    /*0x16*/ u8 power;
    /*0x17*/ u8 type;
    /*0x18*/ u8 accuracy;
    /*0x19*/ u8 pp;
    /*0x1A*/ u8 secondaryEffectChance; // If diffent than 0 it will be affected by Sheer Force
    /*0x1B*/ u8 target;
    /*0x1C*/ s8 priority;
    /*0x1D*/ u8 split;
    /*0x1E*/ u8 argument;
	/*0x1F*/ u8 zMoveEffect;
    /*0x20*/ struct MoveFlags flags;
};

extern const struct BattleMove gBattleMoves[];

struct Ability
{
	u8 name[ABILITY_NAME_LENGTH + 1];
	u8 cantBeCopied:1; // Can't be copied by Role Play.
	u8 cantBeSwapped:1; // Can't be swapped by Skill Swap or Wandering Spirit.
	u8 cantBeTraced:1; // Can't be copied by Trace
	u8 cantBeSuppressed:1; // Can't be suppressed by Gastro Acid or Neutralizing Gas.
	u8 cantBeOverwritten:1; // Can't be replaced by Entrainment, Worry Seed or Simple Beam (but can be by Mummy).
	u8 breakable:1; // Can be bypassed by Mold Breaker.
	u8 unused:2;
	u16 unused2;
	const u8 *description;
};

struct __attribute__((packed)) LevelUpMove
{
    u16 move;
    u8 level;
};

enum
{
    FLAVOR_SPICY,
    FLAVOR_DRY,
    FLAVOR_SWEET,
    FLAVOR_BITTER,
    FLAVOR_SOUR,
    FLAVOR_COUNT
};

enum
{
    GROWTH_MEDIUM_FAST,
    GROWTH_ERRATIC,
    GROWTH_FLUCTUATING,
    GROWTH_MEDIUM_SLOW,
    GROWTH_FAST,
    GROWTH_SLOW
};

enum
{
    BODY_COLOR_RED,
    BODY_COLOR_BLUE,
    BODY_COLOR_YELLOW,
    BODY_COLOR_GREEN,
    BODY_COLOR_BLACK,
    BODY_COLOR_BROWN,
    BODY_COLOR_PURPLE,
    BODY_COLOR_GRAY,
    BODY_COLOR_WHITE,
    BODY_COLOR_PINK
};

#define EVO_FRIENDSHIP                    0x0001 // Pokémon levels up with friendship ≥ 220
#define EVO_FRIENDSHIP_DAY                0x0002 // Pokémon levels up during the day with friendship ≥ 220
#define EVO_FRIENDSHIP_NIGHT              0x0003 // Pokémon levels up at night with friendship ≥ 220
#define EVO_LEVEL                         0x0004 // Pokémon reaches the specified level
#define EVO_ITEM                          0x0005 // specified item is used on Pokémon
#define EVO_LEVEL_ATK_GT_DEF              0x0006 // Pokémon reaches the specified level with attack > defense
#define EVO_LEVEL_ATK_EQ_DEF              0x0007 // Pokémon reaches the specified level with attack = defense
#define EVO_LEVEL_ATK_LT_DEF              0x0008 // Pokémon reaches the specified level with attack < defense
#define EVO_LEVEL_NINJASK                 0x0009 // Pokémon reaches the specified level (special value for Ninjask)
#define EVO_LEVEL_SHEDINJA                0x000A // Pokémon reaches the specified level (special value for Shedinja)
#define EVO_LEVEL_FEMALE                  0x000B // Pokémon reaches the specified level, is female
#define EVO_LEVEL_MALE                    0x000C // Pokémon reaches the specified level, is male
#define EVO_LEVEL_NIGHT                   0x000D // Pokémon reaches the specified level, is night
#define EVO_LEVEL_DAY                     0x000E // Pokémon reaches the specified level, is day
#define EVO_LEVEL_DUSK                    0x000F // Pokémon reaches the specified level, is dusk (5-6 P.M)
#define EVO_ITEM_HOLD_DAY                 0x0010 // Pokémon levels up, holds specified item at day
#define EVO_ITEM_HOLD_NIGHT               0x0011 // Pokémon levels up, holds specified item at night
#define EVO_MOVE                          0x0012 // Pokémon levels up, knows specified move
#define EVO_MOVE_TYPE                     0x0013 // Pokémon levels up, knows move with specified type
#define EVO_MAPSEC                        0x0014 // Pokémon levels up on specified mapsec
#define EVO_ITEM_MALE                     0x0015 // specified item is used on a male Pokémon
#define EVO_ITEM_FEMALE                   0x0016 // specified item is used on a female Pokémon
#define EVO_LEVEL_RAIN_OR_FOG             0x0017 // Pokémon reaches the specified level while it's raining or fog
#define EVO_SPECIFIC_MON_IN_PARTY         0x0018 // Pokémon levels up with a specified Pokémon in party
#define EVO_LEVEL_DARK_TYPE_MON_IN_PARTY  0x0019 // Pokémon reaches the specified level with a Dark Type Pokémon in party
#define EVO_TRADE_SPECIFIC_MON            0x001A // Pokémon is traded for a specified Pokémon
#define EVO_LEVEL_NATURE                  0x001B // Pokémon reaches the specified level, nature forms are handled by form change
#define EVO_CRITICAL_HITS                 0x001C // Pokémon performs specified number of critical hits in one battle
#define EVO_SCRIPT_TRIGGER_DMG            0x001D // Pokémon has specified HP below max, then player interacts trigger
#define EVO_LEVEL_PERSONALITY             0x001E // Pokémon reaches the specified level, personality forms are handled by form change

struct Evolution
{
    u16 method;
    u16 param;
    u16 targetSpecies;
};

#define EVOS_PER_MON 10

enum
{
    GENERATE_SHINY_NORMAL,
    GENERATE_SHINY_LOCKED,
    GENERATE_SHINY_FORCED
};

struct PokemonGenerator
{
	u16 species;
	u8 level;
	u8 otIdType;
	u32 fixedOtId;
	u8 shinyType;
	bool8 forceGender;
	u8 forcedGender;
	bool8 hasFixedPersonality;
	u32 fixedPersonality;
	bool8 forceNature;
	u8 forcedNature;
	u16 unused;
	struct Pokemon *pokemon;
};

extern u8 gPlayerPartyCount;
extern struct Pokemon gPlayerParty[PARTY_SIZE];
extern u8 gEnemyPartyCount;
extern struct Pokemon gEnemyParty[PARTY_SIZE];
extern const struct BaseStats gBaseStats[];
extern const u8 *const gItemEffectTable[];
extern const u8 gStatStageRatios[][2];
extern struct SpriteTemplate gMultiuseSpriteTemplate;
extern struct PokemonStorage* gPokemonStoragePtr;
extern const u32 gExperienceTables[][MAX_LEVEL + 1];
extern const struct LevelUpMove *const gLevelUpLearnsets[];
extern const u8 gFacilityClassToPicIndex[];
extern const u8 gFacilityClassToTrainerClass[];
extern const struct SpriteTemplate gSpriteTemplates_Battlers[];
extern const u8 gPPUpGetMask[];
extern const s8 gNatureStatTable[NUM_NATURES][NUM_STATS - 1];
extern const struct Evolution gEvolutionTable[][EVOS_PER_MON];
extern const u8 gPPUpGetMask[];
extern const u8 gPPUpSetMask[];
extern const u8 gPPUpAddMask[];

void ZeroBoxMonData(struct BoxPokemon *boxMon);
void ZeroMonData(struct Pokemon *mon);
void ZeroPlayerPartyMons(void);
void ZeroEnemyPartyMons(void);
void CreateMon(struct PokemonGenerator generator);
void CreateBattleTowerMon(struct Pokemon *mon, struct BattleTowerPokemon *src);
void GiveMonEvSpread(struct Pokemon *mon, u8 evSpread);
void CalculateMonStats(struct Pokemon *mon);
void BoxMonToMon(struct BoxPokemon *src, struct Pokemon *dest);
u8 GetLevelFromBoxMonExp(struct BoxPokemon *boxMon);
u16 GiveMoveToMon(struct Pokemon *mon, u16 move);
void SetMonMoveSlot(struct Pokemon *mon, u16 move, u8 slot);
u16 MonTryLearningNewMove(struct Pokemon *mon, bool8 firstMove);
u16 MonTryLearningNewMoveAfterEvolution(struct Pokemon *mon, bool8 firstMove);
void GiveMonInitialMoveset(struct Pokemon *mon);
void DeleteFirstMoveAndGiveMoveToMon(struct Pokemon *mon, u16 move);
u32 GetShinyRollsIncrease(void);

u8 GetMonGender(struct Pokemon *mon);
u8 GetBoxMonGender(struct BoxPokemon *boxMon);
u8 GetGenderFromSpeciesAndPersonality(u16 species, u32 personality);
void SetMultiuseSpriteTemplateToPokemon(u16 speciesTag, u8 battlerPosition);
void SetMultiuseSpriteTemplateToTrainerBack(u16 trainerSpriteId, u8 battlerPosition);

// These are full type signatures for GetMonData() and GetBoxMonData(),
// but they are not used since some code erroneously omits the third arg.
// u32 GetMonData(struct Pokemon *mon, s32 field, u8 *data);
// u32 GetBoxMonData(struct BoxPokemon *boxMon, s32 field, u8 *data);

#ifdef IS_POKEMON_C
u32 GetMonData(struct Pokemon *, s32, u8 *);
u32 GetBoxMonData(struct BoxPokemon *, s32, u8 *);
#else
u32 GetMonData();
u32 GetBoxMonData();
#endif // IS_POKEMON_C

void SetMonData(struct Pokemon *mon, s32 field, const void *dataArg);
void SetBoxMonData(struct BoxPokemon *boxMon, s32 field, const void *dataArg);
void CopyMon(void *dest, void *src, size_t size);
u8 GiveMonToPlayer(struct Pokemon *mon);
u8 CalculatePlayerPartyCount(void);
u8 CalculateEnemyPartyCount(void);
u8 GetMonsStateToDoubles(void);
u16 GetAbilityBySpecies(u16 species, bool8 abilityNum, bool8 abilityHidden);
u16 GetMonAbility(struct Pokemon *mon);
bool8 IsPlayerPartyAndPokemonStorageFull(void);
void GetSpeciesName(u8 *name, u16 species);
u8 CalculatePPWithBonus(u16 move, u8 ppBonuses, u8 moveIndex);
void RemoveMonPPBonus(struct Pokemon *mon, u8 moveIndex);
bool8 PokemonUseItemEffects(struct Pokemon *mon, u16 item, u8 partyIndex, u8 moveIndex, u8 battleMonId);
u16 GetEvolutionTargetSpecies(struct Pokemon *mon, u8 type, u16 evolutionItem, struct Pokemon *tradePartner);
void DrawSpindaSpots(u16 species, u32 personality, u8 *dest, bool8 isFrontPic);
void EvolutionRenameMon(struct Pokemon *mon, u16 oldSpecies, u16 newSpecies);
u8 GetPlayerFlankId(void);
u8 GetLinkTrainerFlankId(u8 linkPlayerId);
s32 GetBattlerMultiplayerId(u16 a1);
void AdjustFriendship(struct Pokemon *mon, u8 event);
bool8 ModifyMonFriendship(struct Pokemon *mon, s8 friendshipDelta);
void MonGainEVs(struct Pokemon *mon, u16 defeatedSpecies);
u16 GetMonEVCount(struct Pokemon *mon);
void RandomlyGivePartyPokerus(struct Pokemon *party);
u8 CheckPartyPokerus(struct Pokemon *party, u8 party_bm);
u8 CheckPartyHasHadPokerus(struct Pokemon *party, u8 selection);
void UpdatePartyPokerusTime(void);
void PartySpreadPokerus(struct Pokemon *party);
bool8 TryIncrementMonLevel(struct Pokemon *mon);
bool8 CanMonLearnTMHM(struct Pokemon *mon, u8 tm);
u8 GetMoveRelearnerMoves(struct Pokemon *mon, u16 *moves);
u8 GetLevelUpMovesBySpecies(u16 species, u16 *moves);
u8 GetNumberOfRelearnableMoves(struct Pokemon *mon);
void PlayBattleBGM(void);
void PlayMapChosenOrBattleBGM(u16 songId);
const u32 *GetMonFrontSpritePal(struct Pokemon *mon);
const u32 *GetMonSpritePalFromSpecies(u16 species, bool8 isShiny);
const struct CompressedSpritePalette *GetMonSpritePalStruct(struct Pokemon *mon);
const struct CompressedSpritePalette *GetMonSpritePalStructFromSpecies(u16 species, bool8 isShiny);
u16 ItemIdToBattleMoveId(u16 item);
s8 GetMonFlavorRelation(struct Pokemon *mon, u8 flavor);
bool8 IsTradedMon(struct Pokemon *mon);
bool8 IsOtherTrainer(u32 otId, u8 *otName);
void MonRestorePP(struct Pokemon *mon);
void BoxMonRestorePP(struct BoxPokemon *boxMon);
void SetMonPreventsSwitchingString(void);
void SetWildMonHeldItem(struct Pokemon *mon);
u8 *GetTrainerPartnerName(void);
u8 GetPlayerPartyHighestLevel(void);
void SetDeoxysStats(void);
u16 GetUnionRoomTrainerPic(void);
u16 GetUnionRoomTrainerClass(void);
u8 GetNumOfBadges(void);
void DeleteMonMove(struct Pokemon *mon, u8 movePos);
void ClearAllFusedMonSpecies(void);
bool8 HealStatusConditions(struct Pokemon *mon, u32 healMask, u8 battleId);
const u8* GetItemEffect(u16 item);
bool8 MonCanBattle(struct Pokemon *mon);
bool8 IsMonValidSpecies(struct Pokemon *mon);

static inline u8 GetNatureFromPersonality(u32 personality)
{
    return personality % NUM_NATURES;
}

static inline u16 FacilityClassToPicIndex(u16 facilityClass)
{
    return gFacilityClassToPicIndex[facilityClass];
}

static inline u16 SanitizeSpeciesId(u16 species)
{
	return species > NUM_SPECIES ? SPECIES_NONE : species;
}

#endif // GUARD_POKEMON_H

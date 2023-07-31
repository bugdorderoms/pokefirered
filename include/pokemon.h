#ifndef GUARD_POKEMON_H
#define GUARD_POKEMON_H

#include "global.h"
#include "sprite.h"
#include "constants/pokemon.h"
#include "pokemon_storage_system.h"

struct PokemonSubstruct0
{
    u16 species;
    u16 heldItem;
    u32 experience;
    u8 ppBonuses;
    u8 friendship;
    u8 nature;
	u8 pokeball;
};

struct PokemonSubstruct1
{
    u16 moves[4];
    u8 pp[4];
};

struct PokemonSubstruct2
{
    u8 hpEV;
    u8 attackEV;
    u8 defenseEV;
    u8 speedEV;
    u8 spAttackEV;
    u8 spDefenseEV;
    u8 cool;
    u8 beauty;
    u8 cute;
    u8 smart;
    u8 tough;
    u8 sheen;
};

struct PokemonSubstruct3
{
 /* 0x00 */ u8 pokerus;
 /* 0x01 */ u8 metLocation;

 /* 0x02 */ u16 metLevel:7;
 /* 0x02 */ u16 metGame:4;
 /* 0x03 */ u16 dynamaxLevel:4;
 /* 0x03 */ u16 otGender:1;

 /* 0x04 */ u32 hpIV:5;
 /* 0x04 */ u32 attackIV:5;
 /* 0x05 */ u32 defenseIV:5;
 /* 0x05 */ u32 speedIV:5;
 /* 0x05 */ u32 spAttackIV:5;
 /* 0x06 */ u32 spDefenseIV:5;
 /* 0x07 */ u32 isEgg:1;
 /* 0x07 */ u32 abilityNum:1;

 /* 0x08 */ u32 coolRibbon:3;
 /* 0x08 */ u32 beautyRibbon:3;
 /* 0x08 */ u32 cuteRibbon:3;
 /* 0x09 */ u32 smartRibbon:3;
 /* 0x09 */ u32 toughRibbon:3;
 /* 0x09 */ u32 championRibbon:1;
 /* 0x0A */ u32 winningRibbon:1;
 /* 0x0A */ u32 victoryRibbon:1;
 /* 0x0A */ u32 artistRibbon:1;
 /* 0x0A */ u32 effortRibbon:1;
 /* 0x0A */ u32 marineRibbon:1; // never distributed
 /* 0x0A */ u32 landRibbon:1; // never distributed
 /* 0x0A */ u32 skyRibbon:1; // never distributed
 /* 0x0A */ u32 countryRibbon:1; // distributed during Pokémon Festa '04 and '05 to tournament winners
 /* 0x0B */ u32 nationalRibbon:1;
 /* 0x0B */ u32 earthRibbon:1;
 /* 0x0B */ u32 worldRibbon:1; // distributed during Pokémon Festa '04 and '05 to tournament winners
 /* 0x0B */ u32 abilityHidden:1;
 /* 0x0B */ u32 eventLegal:1; // controls Mew & Deoxys obedience; if set, Pokémon is a fateful encounter in FRLG & Gen 4+ summary screens; set for in-game event island legendaries, some distributed events, and Pokémon from XD: Gale of Darkness.
 /* 0x0B */ u32 gMaxFactor:1;
 /* 0x0B */ u32 filler:2;
};

union PokemonSubstruct
{
    struct PokemonSubstruct0 type0;
    struct PokemonSubstruct1 type1;
    struct PokemonSubstruct2 type2;
    struct PokemonSubstruct3 type3;
    u16 raw[6];
};

struct BoxPokemon
{
    u32 personality;
    u32 otId;
    u8 nickname[POKEMON_NAME_LENGTH];
    u8 language;
    u8 isBadEgg:1;
    u8 hasSpecies:1;
    u8 isEgg:1;
    u8 unused:5;
    u8 otName[PLAYER_NAME_LENGTH];
    u8 markings;
    u16 checksum;
    u16 unknown;

    union
    {
        u32 raw[12];
        union PokemonSubstruct substructs[4];
    } secure;
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
	u8 teraType;
};

struct PokemonStorage
{
    /*0x0000*/ u8 currentBox;
    /*0x0001*/ struct BoxPokemon boxes[TOTAL_BOXES_COUNT][IN_BOX_COUNT];
    /*0x8344*/ u8 boxNames[TOTAL_BOXES_COUNT][BOX_NAME_LENGTH + 1];
    /*0x83C2*/ u8 boxWallpapers[TOTAL_BOXES_COUNT];
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
    /*0x0C*/ u16 moves[4];
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
    /*0x24*/ u8 type3; // for Trick-or-Treat and Forest's Curse
    /*0x25*/ u8 level;
    /*0x26*/ u16 item;
    /*0x28*/ u16 hp;
    /*0x2A*/ u16 maxHP;
    /*0x2C*/ u8 nickname[POKEMON_NAME_LENGTH + 1];
    /*0x37*/ u8 ppBonuses;
    /*0x38*/ u8 otName[8];
    /*0x40*/ u8 pp[4];
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
	u32 secondaryEffectMove:1; // A move with this flag can be affected by Sheer Force.
	u32 bitingMove:1; // A move with this flag can be affected by Strong Jaw.
	u32 pulseMove:1; // A move with this flag can be affected by Mega Launcher.
	u32 targetStatStagesIgnored:1; // A move with this flag will ignore the target Defense and Evasion stat stages.
	u32 forbiddenMimic:1; // A move with this flag can't be copied by Mimic.
	u32 hitUnderground:1; // If target is undergrounded, can hit.
	u32 hitUnderwater:1; // If target is in underwater, can hit.
	// end of byte
	u32 soundMove:1; // A move with this flag will makes sound.
	u32 ballisticMove:1; // A move with this flag will be blocked by Bulletproof.
	u32 protectionMove:1; // A move with this flag can set a protect like effect.
	u32 powderMove:1; // A move with this flag can be blocked by Overcoat, etc.
	u32 ignoreAbilities:1; // A move with this flag will ignore all ignoreable abilities.
	u32 twoStrikes:1; // A move with this flag will strike twice, and may apply its effect on each hit.
	u32 hitInAir:1; // If target is in the air, can hit.
	u32 hitInAirDoubleDmg:1; // If target is in the air, can hit and deal double damage.
	// end of byte
	u32 makeGrounded:1; // A move with this flag makes Ground type moves do 1x damage to flying and levitating targets.
	u32 hitSubstitute:1; // A move with this flag will ignore the target's Substitute.
	u32 slicingMove:1; // A move with this flag can be affected by Sharpness.
	u32 windMove:1; // A move with this flag can be affected by Wind Rider, etc.
	u32 threeStrikes:1; // A move with this flag will strike three times, and may apply its effect on each hit.
	u32 forbiddenMirrorMove:1; // A move with this flag can't be affected by Mirror Move.
	u32 forbiddenMetronome:1; // A move with this flag can't be called by Metronome.
	u32 forbiddenAssist:1; // A move with this flag can't be executed by Assist.
	// end of byte
	u32 forbiddenSleepTalk:1; // A move with this flag can't be executed by Sleep Talk.
	u32 forbiddenCopycat:1; // A move with this flag can't be copied by Copycat.
	u32 forbiddenInstruct:1; // A move with this flag can't be executed by Instruct.
	u32 forbiddenParentalBond:1; // A move with this flag can't become a multi-hit move due to Parental Bond.
	u32 affectsUserSide:1; // Tipycally used to show the entire side on choose the move, but it's still used to determine protect like effects.
	u32 dmgMinimize:1; // A move with this flag will deal double damage and aways hit the target if it was used Minimize.
	u32 unused:26;
};

struct BattleMove
{
    /*0x00*/ u16 effect;
    /*0x02*/ u8 power;
    /*0x03*/ u8 type;
    /*0x04*/ u8 accuracy;
    /*0x05*/ u8 pp;
    /*0x06*/ u8 secondaryEffectChance;
    /*0x07*/ u8 target;
    /*0x08*/ s8 priority;
    /*0x09*/ u8 split;
    /*0x0A*/ u8 argument;
	/*0x0B*/ u8 zMoveEffect;
    /*0x0C*/ struct MoveFlags flags;
};

extern const struct BattleMove gBattleMoves[];

struct SpindaSpot
{
    u8 x, y;
    u16 image[16];
};

struct __attribute__((packed)) LevelUpMove
{
    u16 move;
    u8 level;
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
#define EVO_LEVEL_SILCOON                 0x0009 // Pokémon reaches the specified level with a Silcoon personality value
#define EVO_LEVEL_CASCOON                 0x000A // Pokémon reaches the specified level with a Cascoon personality value
#define EVO_LEVEL_NINJASK                 0x000B // Pokémon reaches the specified level (special value for Ninjask)
#define EVO_LEVEL_SHEDINJA                0x000C // Pokémon reaches the specified level (special value for Shedinja)
#define EVO_BEAUTY                        0x000D // Pokémon levels up with beauty ≥ specified value
#define EVO_LEVEL_FEMALE                  0x000E // Pokémon reaches the specified level, is female
#define EVO_LEVEL_MALE                    0x000F // Pokémon reaches the specified level, is male
#define EVO_LEVEL_NIGHT                   0x0010 // Pokémon reaches the specified level, is night
#define EVO_LEVEL_DAY                     0x0011 // Pokémon reaches the specified level, is day
#define EVO_LEVEL_DUSK                    0x0012 // Pokémon reaches the specified level, is dusk (5-6 P.M)
#define EVO_ITEM_HOLD_DAY                 0x0013 // Pokémon levels up, holds specified item at day
#define EVO_ITEM_HOLD_NIGHT               0x0014 // Pokémon levels up, holds specified item at night
#define EVO_MOVE                          0x0015 // Pokémon levels up, knows specified move
#define EVO_MOVE_TYPE                     0x0016 // Pokémon levels up, knows move with specified type
#define EVO_MAPSEC                        0x0017 // Pokémon levels up on specified mapsec
#define EVO_ITEM_MALE                     0x0018 // specified item is used on a male Pokémon
#define EVO_ITEM_FEMALE                   0x0019 // specified item is used on a female Pokémon
#define EVO_LEVEL_RAIN_OR_FOG             0x001A // Pokémon reaches the specified level while it's raining or fog
#define EVO_SPECIFIC_MON_IN_PARTY         0x001B // Pokémon levels up with a specified Pokémon in party
#define EVO_LEVEL_DARK_TYPE_MON_IN_PARTY  0x001C // Pokémon reaches the specified level with a Dark Type Pokémon in party
#define EVO_TRADE_SPECIFIC_MON            0x001D // Pokémon is traded for a specified Pokémon
#define EVO_LEVEL_NATURE                  0x001E // Pokémon reaches the specified level, nature forms are handled by form change
#define EVO_CRITICAL_HITS                 0x001F // Pokémon performs specified number of critical hits in one battle
#define EVO_SCRIPT_TRIGGER_DMG            0x0020 // Pokémon has specified HP below max, then player interacts trigger
#define EVO_LEVEL_PERSONALITY             0x0021 // Pokémon reaches the specified level, personality forms are handled by form change

struct Evolution
{
    u16 method;
    u16 param;
    u16 targetSpecies;
};

#define EVOS_PER_MON 10

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
extern const s8 gNatureStatTable[][5];
extern const struct Evolution gEvolutionTable[][EVOS_PER_MON];
extern const u8 gPPUpGetMask[];
extern const u8 gPPUpSetMask[];
extern const u8 gPPUpAddMask[];

void ZeroBoxMonData(struct BoxPokemon *boxMon);
void ZeroMonData(struct Pokemon *mon);
void ZeroPlayerPartyMons(void);
void ZeroEnemyPartyMons(void);
void CreateMon(struct Pokemon *mon, u16 species, u8 level, u8 fixedIV, u8 hasFixedPersonality, u32 fixedPersonality, u8 otIdType, u32 fixedOtId);
void CreateBoxMon(struct BoxPokemon *boxMon, u16 species, u8 level, u8 fixedIV, u8 hasFixedPersonality, u32 fixedPersonality, u8 otIdType, u32 fixedOtId);
void CreateMonWithNature(struct Pokemon *mon, u16 species, u8 level, u8 fixedIV, u8 nature);
void CreateMonWithGenderNatureLetter(struct Pokemon *mon, u16 species, u8 level, u8 fixedIV, u8 gender, u8 nature, u8 unownLetter);
void CreateMaleMon(struct Pokemon *mon, u16 species, u8 level);
void CreateMonWithIVsPersonality(struct Pokemon *mon, u16 species, u8 level, u32 ivs, u32 personality);
void CreateMonWithEVSpread(struct Pokemon *mon, u16 species, u8 level, u8 fixedIV, u8 evSpread);
void CreateBattleTowerMon(struct Pokemon *mon, struct BattleTowerPokemon *src);
void ConvertPokemonToBattleTowerPokemon(struct Pokemon *mon, struct BattleTowerPokemon *dest);
void CalculateMonStats(struct Pokemon *mon);
void BoxMonToMon(struct BoxPokemon *src, struct Pokemon *dest);
u8 GetLevelFromBoxMonExp(struct BoxPokemon *boxMon);
u16 GiveMoveToMon(struct Pokemon *mon, u16 move);
u16 GiveMoveToBattleMon(struct BattlePokemon *mon, u16 move);
void SetMonMoveSlot(struct Pokemon *mon, u16 move, u8 slot);
void SetBattleMonMoveSlot(struct BattlePokemon *mon, u16 move, u8 slot);
u16 MonTryLearningNewMove(struct Pokemon *mon, bool8 firstMove);
u16 MonTryLearningNewMoveAfterEvolution(struct Pokemon *mon, bool8 firstMove);
void GiveMonInitialMoveset(struct Pokemon *mon);
void DeleteFirstMoveAndGiveMoveToMon(struct Pokemon *mon, u16 move);

#define BATTLE_ALIVE_EXCEPT_ACTIVE   0
#define BATTLE_ALIVE_ATK_SIDE        1
#define BATTLE_ALIVE_DEF_SIDE        2
#define BATTLE_ALIVE_EXCEPT_ATTACKER 3

u8 CountAliveMonsInBattle(u8 caseId);

u8 GetDefaultMoveTarget(u8 battlerId);
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
u8 GetNatureFromPersonality(u32 personality);
u16 GetAbilityBySpecies(u16 species, bool8 abilityNum, bool8 abilityHidden);
u16 GetMonAbility(struct Pokemon *mon);
u8 GetSecretBaseTrainerPicIndex(void);
u8 GetSecretBaseTrainerNameIndex(void);
bool8 IsPlayerPartyAndPokemonStorageFull(void);
void GetSpeciesName(u8 *name, u16 species);
u8 CalculatePPWithBonus(u16 move, u8 ppBonuses, u8 moveIndex);
void RemoveMonPPBonus(struct Pokemon *mon, u8 moveIndex);
void RemoveBattleMonPPBonus(struct BattlePokemon *mon, u8 moveIndex);
bool8 PokemonUseItemEffects(struct Pokemon *mon, u16 item, u8 partyIndex, u8 moveIndex, u8 battleMonId);
u16 GetEvolutionTargetSpecies(struct Pokemon *mon, u8 type, u16 evolutionItem, struct Pokemon *tradePartner);
u16 NationalPokedexNumToSpecies(u16 nationalNum);
u16 SpeciesToNationalPokedexNum(u16 species);
void DrawSpindaSpots(u16 species, u32 personality, u8 *dest, bool8 isFrontPic);
void EvolutionRenameMon(struct Pokemon *mon, u16 oldSpecies, u16 newSpecies);
bool8 GetPlayerFlankId(void);
bool16 GetLinkTrainerFlankId(u8 linkPlayerId);
s32 GetBattlerMultiplayerId(u16 a1);
u8 GetTrainerEncounterMusicId(u16 trainer);
void AdjustFriendship(struct Pokemon *mon, u8 event);
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
u16 SpeciesToPokedexNum(u16 species);
void PlayBattleBGM(void);
void PlayMapChosenOrBattleBGM(u16 songId);
const u32 *GetMonFrontSpritePal(struct Pokemon *mon);
const u32 *GetMonSpritePalFromSpeciesAndPersonality(u16 species, u32 otId, u32 personality);
const struct CompressedSpritePalette *GetMonSpritePalStruct(struct Pokemon *mon);
const struct CompressedSpritePalette *GetMonSpritePalStructFromOtIdPersonality(u16 species, u32 otId , u32 personality);
bool8 IsPokeSpriteNotFlipped(u16 species);
s8 GetFlavorRelation(u8 battlerId, u8 flavor);
bool8 IsTradedMon(struct Pokemon *mon);
bool8 IsOtherTrainer(u32 otId, u8 *otName);
void MonRestorePP(struct Pokemon *mon);
void BoxMonRestorePP(struct BoxPokemon *boxMon);
void SetMonPreventsSwitchingString(void);
void SetWildMonHeldItem(struct Pokemon *mon);
bool8 IsMonShiny(struct Pokemon *mon);
u8 *GetTrainerPartnerName(void);
u8 GetPlayerPartyHighestLevel(void);
u16 FacilityClassToPicIndex(u16 facilityClass);
void SetDeoxysStats(void);
u16 GetUnionRoomTrainerPic(void);
u16 GetUnionRoomTrainerClass(void);
void CreateEventLegalEnemyMon(void);
void HandleSetPokedexFlag(u16 nationalNum, u8 caseId, u32 personality);
bool8 CheckBattleTypeGhost(struct Pokemon *mon, u8 bank);
struct OakSpeechNidoranFStruct *OakSpeechNidoranFSetup(u8 battlePosition, bool8 enable);
void OakSpeechNidoranFFreeResources(void);
void *OakSpeechNidoranFGetBuffer(u8 bufferId);
u8 GetNumOfBadges(void);
void DeleteMonMove(struct Pokemon *mon, u8 movePos);
void ClearAllFusedMonSpecies(void);
bool8 HealStatusConditions(struct Pokemon *mon, u32 healMask, u8 battleId);
const u8* GetItemEffect(u16 item);

#endif // GUARD_POKEMON_H

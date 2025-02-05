#ifndef GUARD_POKEMON_H
#define GUARD_POKEMON_H

#include "global.h"
#include "sprite.h"
#include "constants/form_change.h"
#include "constants/pokemon.h"
#include "pokemon_storage_system.h"

struct BoxPokemon
{
    /*0x00*/ u32 personality;
    /*0x04*/ u32 otId;
    /*0x08*/ u8 nickname[POKEMON_NAME_LENGTH];
    /*0x14*/ u8 otName[PLAYER_NAME_LENGTH];
	/*0x1B*/ u8 friendship;
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
	/*0x3A*/ u8 metLocation;
	/*0x3B*/ u8 ppBonuses;
	/*0x3C*/ u16 evolutionTracker:10; // Values up to 1023
	         u16 shiny:1;
			 u16 formCountdown:3;
			 u16 unused:2;
};

struct Status1
{
	u8 id:4;
	u8 counter:4;
};

struct Pokemon
{
    struct BoxPokemon box;
    struct Status1 status;
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
    /*0x04*/ u16 moves[MAX_MON_MOVES];
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
    /*0x20*/ u8 nickname[POKEMON_NAME_LENGTH + 1];
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
    /*0x3D*/ u8 ppBonuses;
    /*0x3E*/ u8 otName[8];
	/*0x46*/ u8 friendship;
	/*0x47*/ struct Status1 status1;
    /*0x48*/ u32 personality;
    /*0x4C*/ u32 status2;
    /*0x50*/ u32 otId;
	/*0x54*/ u32 experience;
};

struct SpeciesInfo
{
	        // Base stats
 /* 0x00 */ u8 name[POKEMON_NAME_LENGTH + 1];
 /* 0x0D */ u8 baseHP;
 /* 0x0E */ u8 baseAttack;
 /* 0x0F */ u8 baseDefense;
 /* 0x10 */ u8 baseSpeed;
 /* 0x11 */ u8 baseSpAttack;
 /* 0x12 */ u8 baseSpDefense;
 /* 0x13 */ u8 types[2];
 /* 0x15 */ u8 catchRate;
 /* 0x16 */ u16 expYield;
 /* 0x18 */ u8 evYield_HP:2;
            u8 evYield_Attack:2;
            u8 evYield_Defense:2;
            u8 evYield_Speed:2;
 /* 0x19 */ u8 evYield_SpAttack:2;
            u8 evYield_SpDefense:2;
			u8 growthRate:4; // last bit was unused
 /* 0x1A */ u8 eggGroup1:4;
			u8 eggGroup2:4;
 /* 0x1B */ u8 friendship;
 /* 0x1C */ u16 itemCommon;
 /* 0x1E */ u16 itemRare;
 /* 0x20 */ u16 abilities[2];
 /* 0x24 */ u16 hiddenAbility;
 /* 0x26 */ u8 eggCycles;
 /* 0x27 */ u8 genderRatio;
 /* 0x28 */ u16 cryId;
 /* 0x2A */ u16 flags;
            // Pokedex
 /* 0x2C */ const u8 *description;
 /* 0x30 */ u16 natDexNum;
 /* 0x32 */ u16 height; // In decimeters
 /* 0x34 */ u16 weight; // In hectograms
 /* 0x36 */ u8 categoryName[DEX_CATEGORY_NAME_LENGTH + 1];
			// Graphics
 /* 0x44 */ const u8 *footprint;
 /* 0x48 */ const u8 *icon;
 /* 0x4C */ const u32 *frontPic; // LZ77 compressed pixel data.
 /* 0x50 */ const u32 *backPic; // LZ77 compressed pixel data.
 /* 0x54 */ const u32 *palette; // LZ77 compressed palette data.
 /* 0x58 */ const u32 *shinyPalette; // LZ77 compressed palette data.
 /* 0x5C */ u16 iconPaletteIndex:2;
			// All Pokémon pics are 64x64, but this data table defines where in this 64x64 frame the sprite's non-transparent pixels actually are.
			u16 frontPicYOffset:5; // The number of pixels between the drawn pixel area and the bottom edge.
			u16 backPicYOffset:5; // The number of pixels between the drawn pixel area and the bottom edge.
			u16 suppressEnemyShadow:1; // If true, it won't create a shadow on the enemy front sprite. Also, it is unaffected by Telekinesis.
			u16 shadowSize:3;
 /* 0x5E */ s8 shadowXOffset; // The relative x pixel offset of the shadow sprite.
 /* 0x5F */ s8 frontPicElevation; // The elevation of the pokemon front pic.
 /* 0x60 */ u8 frontPicSize; // The dimensions of this drawn pixel area.
 /* 0x61 */ u8 backPicSize; // The dimensions of this drawn pixel area.
            // Sos battles calls
 /* 0x62 */ u16 sosCallAllies[3]; // Possible allies a Pokémon can call for help in a SOS Battle. 3 in total.
 /* 0x68 */ u8 sosCallRate:4; // The max value for a Pokémon call rate is 15%.
			u8 unused:4;
            // Evolutions and forms
 /* 0x6C */ const u8 *evolutions;
 /* 0x70 */ const struct FormChange *formChangeTable;
			// Learnable moves
 /* 0x74 */ const u16 *tmLearnsets;
 /* 0x78 */ const u16 *tutorLearnsets;
};

struct MoveFlags
{
	/*0x00*/ u8 makesContact:1; // A move with this flag will makes contact.
	/*0x00*/ u8 danceMove:1; // A move with this flag can be affected by Dancer.
	/*0x00*/ u8 magicCoatAffected:1; // A move with this flag can be reflected by Magic Coat.
	/*0x00*/ u8 snatchAffected:1; // A move with this flag can be stealed by Snatch.
	/*0x00*/ u8 thawUser:1; // A move with this flag will thaw the user when used.
	/*0x00*/ u8 kingsRockAffected:1; // A move with this flag can be affected by Kings Rock.
	/*0x00*/ u8 gravityBanned:1; // A move with this flag can't be selected while gravity is in effect.
	/*0x00*/ u8 forbiddenProtect:1; // A move with this flag will ignore the target's protect effects.
	// end of byte
	/*0x01*/ u8 punchMove:1; // A move with this flag can be affected by Iron Fist.
	/*0x01*/ u8 forbiddenMeFirst:1; // A move with this flag can't be executed by Me First.
	/*0x01*/ u8 bitingMove:1; // A move with this flag can be affected by Strong Jaw.
	/*0x01*/ u8 pulseMove:1; // A move with this flag can be affected by Mega Launcher.
	/*0x01*/ u8 targetStatStagesIgnored:1; // A move with this flag will ignore the target Defense and Evasion stat stages.
	/*0x01*/ u8 forbiddenMimic:1; // A move with this flag can't be copied by Mimic.
	/*0x01*/ u8 hitUnderground:1; // If target is undergrounded, can hit and deal double damage.
	/*0x01*/ u8 hitUnderwater:1; // If target is in underwater, can hit and deal double damage.
	// end of byte
	/*0x02*/ u8 soundMove:1; // A move with this flag will makes sound.
	/*0x02*/ u8 ballisticMove:1; // A move with this flag will be blocked by Bulletproof.
	/*0x02*/ u8 protectionMove:1; // A move with this flag can set a protect like effect.
	/*0x02*/ u8 powderMove:1; // A move with this flag can be blocked by Overcoat, etc.
	/*0x02*/ u8 ignoreAbilities:1; // A move with this flag will ignore all ignoreable abilities.
	/*0x02*/ u8 dmgMinimize:1; // A move with this flag will deal double damage and aways hit the target if it was used Minimize.
	/*0x02*/ u8 hitInAir:1; // If target is in the air, can hit.
	/*0x02*/ u8 hitInAirDoubleDmg:1; // If target is in the air, can hit and deal double damage.
	// end of byte
	/*0x03*/ u8 makeGrounded:1; // A move with this flag makes Ground type moves do 1x damage to flying and levitating targets.
	/*0x03*/ u8 hitSubstitute:1; // A move with this flag will ignore the target's Substitute.
	/*0x03*/ u8 slicingMove:1; // A move with this flag can be affected by Sharpness.
	/*0x03*/ u8 windMove:1; // A move with this flag can be affected by Wind Rider, etc.
	/*0x03*/ u8 forbiddenParentalBond:1; // A move with this flag can't become a multi-hit move due to Parental Bond.
	/*0x03*/ u8 forbiddenMirrorMove:1; // A move with this flag can't be affected by Mirror Move.
	/*0x03*/ u8 forbiddenMetronome:1; // A move with this flag can't be called by Metronome.
	/*0x03*/ u8 forbiddenAssist:1; // A move with this flag can't be executed by Assist.
	// end of byte
	/*0x04*/ u8 forbiddenSleepTalk:1; // A move with this flag can't be executed by Sleep Talk.
	/*0x04*/ u8 forbiddenCopycat:1; // A move with this flag can't be copied by Copycat.
	/*0x04*/ u8 forbiddenInstruct:1; // A move with this flag can't be executed by Instruct.
	/*0x04*/ u8 hasQuietBGM:1; // A move with this flag will have a quiet BGM on its battle anim.
	/*0x04*/ u8 strikeCount:4; // Num hits the move will do, max 15 hits. Defaults to 1 if not set. May apply its effect on each hit.
	// end of byte
	/*0x05*/ u8 recoilDivisor:3; // Used by moves that have a recoil damage, also if set, its affected by Reckless.
	/*0x05*/ u8 forcePressure:1; // A move with this flag will force the extra PP consuption from Pressure.
	/*0x05*/ u8 critStage:2; // Max +3 crit stages. Used by moves with a high chance to be critical.
	/*0x05*/ u8 unused:2;
	// end of byte
};

struct AdditionalEffect
{
	u8 moveEffect;
	u8 chance; // 0% = effect certain - If diffent than 0 the move will be affected by Sheer Force
    bool8 self:1;
	bool8 onChargeTurnOnly:1; // Effect applies only on the chaging turn of a two turns move
    bool8 onlyIfTargetRaisedStats:1; // Effect applies only if target was raised its stat
	bool8 onFinalMultiHitOnly:1; // Effect applies only on the final hit of a miltihit move
	bool8 onMoveEndOnly:1; // Effect applies only at ATK49_SECOND_MOVE_EFFECT on cmd moveend
};

#define EFFECTS_ARR(...) (const struct AdditionalEffect[]) {__VA_ARGS__}
#define ADDITIONAL_EFFECTS(...) EFFECTS_ARR( __VA_ARGS__ ), .numAdditionalEffects = ARRAY_COUNT(EFFECTS_ARR( __VA_ARGS__ ))

struct BattleMove
{
	/*0x00*/ const u8 *name;
	/*0x04*/ const u8 *description;
	/*0x08*/ const u8 *animScript;
	/*0x0C*/ u16 effect;
	/*0x0E*/ u8 power;
    /*0x0F*/ u8 type;
    /*0x10*/ u8 accuracy;
    /*0x11*/ u8 pp;
	/*0x12*/ u8 target;
	/*0x13*/ s8 priority;
	/*0x14*/ union {
				struct { u8 percentage; } absorb;
				struct { u8 trappingId; } bind;
				struct { u8 amount; } fixedDamage;
				struct { u8 increment; } tripleKick;
				struct { u8 ppToDeduct; } spite;
				struct { u8 split; } counterAttack;
				struct { u8 statusId; } cureStatus;
				struct { u16 abilityId; } setAbility;
				struct { u8 weatherId; u16 stringId; } setWeather;
				struct { u16 weather; u16 debuffWeather; } neverMissInWeather;
				struct { u16 stringId; u16 status; } semiInvulnerable;
				struct { u16 stringId; u16 weather; } twoTurns;
				struct { u16 stringId; u16 animationId; } futureAttack;
			 } argument;
	/*0x18*/ u8 split;
	/*0x19*/ u8 zMoveEffect;
    /*0x1A*/ struct MoveFlags flags;
			 const struct AdditionalEffect *additionalEffects; // primary/secondary effects
			 u8 numAdditionalEffects;
};

extern const struct BattleMove gBattleMoves[];

struct BattleMoveEffect
{
	const u8 *battleScript;
	u32 normalizeUnaffected:1; // Move effects with this flag will not change type if affected by Normalize and similar abilities.
	u32 twoTurnsEffect:1; // Move effects with this flag will be considered a Two turns effect.
	u32 semiInvulnerableEffect:1; // Move effects with this flag will cause the battler to be semi invulnerable.
	u32 healingEffect:1; // Move effects with this flag can't be used while affected by Heal Block, also Triage gives priority to it.
	u32 affectsUserSide:1; // Tipycally used to show the entire side on chose the move, but it's still used to determine protect like effects.
	u32 callOtherMove:1; // Move effects with this flag will be considered a effect of a "Move that call other moves".
	u32 noEffectiveness:1; // Move effects with this flag will not display the "Its super effective" and "Its not very effective" message.
	u32 unused:1;
	// end of byte
	u32 unused2:8;
	// end of byte
	u32 unused3:8;
	// end of byte
	u32 unused4:8;
	// end of byte
};

extern const struct BattleMoveEffect gBattleMoveEffects[];

#define GET_MOVEEFFECT_TABLE(moveEffect) ((gBattleMoveEffects[moveEffect]))
#define GET_MOVE_MOVEEFFECT_TABLE(move) GET_MOVEEFFECT_TABLE(gBattleMoves[move].effect)
#define GET_MOVE_BATTLESCRIPT(move) GET_MOVE_MOVEEFFECT_TABLE(move).battleScript

struct Ability
{
	const u8 *description;
	u8 name[ABILITY_NAME_LENGTH + 1];
	u8 cantBeCopied:1; // Can't be copied by Role Play.
	u8 cantBeSwapped:1; // Can't be swapped by Skill Swap or Wandering Spirit.
	u8 cantBeTraced:1; // Can't be copied by Trace
	u8 cantBeSuppressed:1; // Can't be suppressed by Gastro Acid or Neutralizing Gas.
	u8 cantBeOverwritten:1; // Can't be replaced by Entrainment, Worry Seed or Simple Beam (but can be by Mummy).
	u8 breakable:1; // Can be bypassed by Mold Breaker.
	u8 unused:2;
	u16 unused2;
};

struct TypeInfo
{
	const u32 *tmPalette;
	u8 name[TYPE_NAME_LENGTH + 1];
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
    GENERATE_SHINY_NORMAL,
    GENERATE_SHINY_LOCKED,
    GENERATE_SHINY_FORCED
};

enum
{
	SHINY_ROLL_NORMAL,
	SHINY_ROLL_DEXNAV,
	SHINY_ROLL_SOS_CALL,
};

struct NatureInfo
{
	const u8 *name;
	u8 statUpId;
	u8 statDownId;
	s8 flavor[FLAVOR_COUNT];
};

#define GENERATOR_FORMS(...) (const u16[]) { __VA_ARGS__, FORM_CHANGE_TERMINATOR }

struct PokemonGenerator
{
	u16 species;
	u8 level;
	u8 otIdType;
	
	u32 fixedOtId;
	
	u8 shinyType;
	u8 forcedGender; // If different than MON_GENDERLESS
	u8 forcedNature; // If different than NUM_NATURES
	bool8 hasFixedPersonality;
	
	u32 fixedPersonality;

	const u16 *formChanges; // If defined, executes all form changes on the given array
	u16 moves[MAX_MON_MOVES];
	
	u8 nPerfectIvs;
	u8 shinyRollType;
};

struct FormChange
{
	u16 method;
	u16 targetSpecies;
	u16 param;
	u16 param2;
};

extern u8 gPlayerPartyCount;
extern struct Pokemon gPlayerParty[PARTY_SIZE];
extern u8 gEnemyPartyCount;
extern struct Pokemon gEnemyParty[PARTY_SIZE];
extern const struct SpeciesInfo gSpeciesInfo[];
extern const u8 gStatStageRatios[][2];
extern struct SpriteTemplate gMultiuseSpriteTemplate;
extern struct PokemonStorage* gPokemonStoragePtr;
extern const u32 gExperienceTables[][MAX_LEVEL + 1];
extern const struct LevelUpMove *const gLevelUpLearnsets[];
extern const u8 gFacilityClassToPicIndex[];
extern const u8 gFacilityClassToTrainerClass[];
extern const struct SpriteTemplate gSpriteTemplates_Battlers[];
extern const struct NatureInfo gNaturesInfo[];
extern const struct TypeInfo gTypesInfo[NUMBER_OF_MON_TYPES];
extern const u8 gPPUpGetMask[];
extern const u8 gPPUpSetMask[];
extern const u8 gPPUpAddMask[];

void ZeroBoxMonData(struct BoxPokemon *boxMon);
void ZeroMonData(struct Pokemon *mon);
void ZeroPlayerPartyMons(void);
void ZeroEnemyPartyMons(void);
void CreateMon(struct Pokemon *mon, struct PokemonGenerator generator);
void CalculateMonStats(struct Pokemon *mon);
void BoxMonToMon(struct BoxPokemon *src, struct Pokemon *dest);
u8 GetLevelFromBoxMonExp(struct BoxPokemon *boxMon);
u8 GiveMoveToMon(struct Pokemon *mon, u16 move);
void SetMonMoveSlot(struct Pokemon *mon, u16 move, u8 slot);
u8 MonTryLearningNewMove(struct Pokemon *mon, bool8 firstMove);
u8 MonTryLearningNewMoveAfterEvolution(struct Pokemon *mon, bool8 firstMove);
void DeleteFirstMoveAndGiveMoveToMon(struct Pokemon *mon, u16 move);
u32 GetShinyRollsIncrease(void);

u8 GetMonGender(struct Pokemon *mon);
u8 GetBoxMonGender(struct BoxPokemon *boxMon);
u8 GetGenderFromSpeciesAndPersonality(u16 species, u32 personality);
bool8 SpeciesHasFixedGenderRatio(u16 species);
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
bool8 PokemonUseItemEffects(struct Pokemon *mon, u16 item, u8 partyIndex, u8 battleMonId);
u16 GetEvolutionTargetSpecies(u8 partyId, u8 type, u16 evolutionItem, struct Pokemon *tradePartner);
void DrawSpindaSpots(u16 species, u32 personality, u8 *dest, bool8 isFrontPic);
void EvolutionRenameMon(struct Pokemon *mon, u16 oldSpecies, u16 newSpecies);
u8 GetPlayerFlankId(void);
u8 GetLinkTrainerFlankId(u8 linkPlayerId);
s32 GetBattlerMultiplayerId(u16 a1);
void AdjustFriendship(struct Pokemon *mon, u8 event);
bool8 ModifyMonFriendship(struct Pokemon *mon, s8 friendshipDelta);
u8 *GetMonNickname(struct Pokemon *mon, u8 *dest);
u8 *GetBoxMonNickname(struct BoxPokemon *boxMon, u8 *dest);
void MonGainEVs(struct Pokemon *mon, u16 defeatedSpecies);
u16 GetMonEVCount(struct Pokemon *mon);
void RandomlyGivePartyPokerus(struct Pokemon *party);
u8 CheckPartyPokerus(struct Pokemon *party, u8 party_bm);
u8 CheckPartyHasHadPokerus(struct Pokemon *party, u8 selection);
void UpdatePartyPokerusTime(u32 daysSince);
void PartySpreadPokerus(struct Pokemon *party);
bool8 TryIncrementMonLevel(struct Pokemon *mon);
bool8 CanMonLearnTM(struct Pokemon *mon, u16 move);
bool8 CanSpeciesLearnTutorMove(u16 species, u16 move);
u8 GetMoveRelearnerMoves(struct Pokemon *mon, u16 *moves);
u8 GetLevelUpMovesBySpecies(u16 species, u16 *moves);
u8 GetNumberOfRelearnableMoves(struct Pokemon *mon);
void PlayBattleBGM(void);
void PlayMapChosenOrBattleBGM(u16 songId);
const u32 *GetMonSpritePal(struct Pokemon *mon);
const u32 *GetMonSpritePalFromSpecies(u16 species, bool8 isShiny);
s8 GetMonFlavorRelation(struct Pokemon *mon, u8 flavor);
bool8 IsTradedMon(struct Pokemon *mon);
bool8 IsOtherTrainer(u32 otId, u8 *otName);
void MonRestorePP(struct Pokemon *mon);
void BoxMonRestorePP(struct BoxPokemon *boxMon);
void SetMonPreventsSwitchingString(void);
void SetWildMonsHeldItem(void);
u8 *GetTrainerPartnerName(void);
u8 GetPlayerPartyHighestLevel(void);
u16 GetUnionRoomTrainerPic(void);
u16 GetUnionRoomTrainerClass(void);
u8 GetNumOfBadges(void);
void DeleteMonMove(struct Pokemon *mon, u8 movePos);
void ClearAllFusedMonSpecies(void);
bool8 HealStatusConditions(struct Pokemon *mon, u8 statusToHeal, u8 battleId);
bool8 MonCanBattle(struct Pokemon *mon);
bool8 IsMonValidSpecies(struct Pokemon *mon);
u8 GetFirstAliveMonSlotInParty(void);
u8 FindMoveSlotInMoveset(struct Pokemon *mon, u16 move);
u8 FindMoveSlotInBoxMonMoveset(struct BoxPokemon *boxMon, u16 move);
void DrawSpeciesFootprint(u8 windowId, u16 species, u8 x, u8 y);
void UpdatePartyFormChangeCountdown(u32 daysSince);
void CopyPokemonToBattleMon(u8 battlerId, struct Pokemon *mon, struct BattlePokemon *dst, bool8 setAllData);

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

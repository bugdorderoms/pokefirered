#define IS_POKEMON_C

#include "global.h"
#include "gflib.h"
#include "random.h"
#include "text.h"
#include "data.h"
#include "main.h"
#include "battle.h"
#include "battle_anim.h"
#include "item.h"
#include "event_data.h"
#include "util.h"
#include "rtc.h"
#include "dexnav.h"
#include "wild_encounter.h"
#include "field_weather.h"
#include "pokemon_icon.h"
#include "pokemon_storage_system.h"
#include "battle_gfx_sfx_util.h"
#include "battle_controllers.h"
#include "evolution_scene.h"
#include "battle_message.h"
#include "battle_util.h"
#include "battle_script_commands.h"
#include "link.h"
#include "m4a.h"
#include "item_use.h"
#include "pokedex.h"
#include "strings.h"
#include "overworld.h"
#include "dns.h"
#include "oak_speech.h"
#include "form_change.h"
#include "party_menu.h"
#include "field_specials.h"
#include "constants/items.h"
#include "constants/item_effects.h"
#include "constants/form_change.h"
#include "constants/pokedex.h"
#include "constants/pokemon.h"
#include "constants/abilities.h"
#include "constants/moves.h"
#include "constants/songs.h"
#include "constants/item_effects.h"
#include "constants/trainers.h"
#include "constants/hold_effects.h"
#include "constants/battle_move_effects.h"
#include "constants/weather.h"

static EWRAM_DATA u8 sLearningMoveTableID = 0;
EWRAM_DATA u8 gPlayerPartyCount = 0;
EWRAM_DATA u8 gEnemyPartyCount = 0;
EWRAM_DATA struct Pokemon gEnemyParty[PARTY_SIZE] = {};
EWRAM_DATA struct Pokemon gPlayerParty[PARTY_SIZE] = {};
EWRAM_DATA struct SpriteTemplate gMultiuseSpriteTemplate = {0};

static u8 GetLevelFromMonExp(struct Pokemon *mon);

#include "data/move/battle_moves.h"
#include "data/move/descriptions.h"
#include "data/move/tmhm_moves.h"
#include "data/pokemon/learnsets/tmhm_learnsets.h"
#include "data/trainer/trainer_class_lookups.h"
#include "data/pokemon/experience_tables.h"
#include "data/pokemon/base_stats.h"
#include "data/pokemon/learnsets/level_up_learnsets.h"
#include "data/pokemon/evolution.h"
#include "data/pokemon/learnsets/level_up_learnset_pointers.h"
#include "data/pokemon/form_change_tables.h"
#include "data/pokemon/form_change_table_pointers.h"
#include "data/pokemon/nature_names.h"
#include "data/item/items.h"
#include "data/item/item_effects.h"

struct SpindaSpot
{
    u8 x, y;
    u16 image[16];
};

static const struct SpindaSpot sSpindaSpotGraphics[] =
{
    {16,  7, INCBIN_U16("graphics/spinda_spots/spot_0.bin")},
    {40,  8, INCBIN_U16("graphics/spinda_spots/spot_1.bin")},
    {22, 25, INCBIN_U16("graphics/spinda_spots/spot_2.bin")},
    {34, 26, INCBIN_U16("graphics/spinda_spots/spot_3.bin")}
};

const s8 gNatureStatTable[NUM_NATURES][NUM_STATS - 1] =
{
                   // Atk Def Spd Sp.Atk Sp.Def
    [NATURE_HARDY]   = { 0,  0,  0,  0,  0},
    [NATURE_LONELY]  = {+1, -1,  0,  0,  0},
    [NATURE_BRAVE]   = {+1,  0, -1,  0,  0},
    [NATURE_ADAMANT] = {+1,  0,  0, -1,  0},
    [NATURE_NAUGHTY] = {+1,  0,  0,  0, -1},
    [NATURE_BOLD]    = {-1, +1,  0,  0,  0},
    [NATURE_DOCILE]  = { 0,  0,  0,  0,  0},
    [NATURE_RELAXED] = { 0, +1, -1,  0,  0},
    [NATURE_IMPISH]  = { 0, +1,  0, -1,  0},
    [NATURE_LAX]     = { 0, +1,  0,  0, -1},
    [NATURE_TIMID]   = {-1,  0, +1,  0,  0},
    [NATURE_HASTY]   = { 0, -1, +1,  0,  0},
    [NATURE_SERIOUS] = { 0,  0,  0,  0,  0},
    [NATURE_JOLLY]   = { 0,  0, +1, -1,  0},
    [NATURE_NAIVE]   = { 0,  0, +1,  0, -1},
    [NATURE_MODEST]  = {-1,  0,  0, +1,  0},
    [NATURE_MILD]    = { 0, -1,  0, +1,  0},
    [NATURE_QUIET]   = { 0,  0, -1, +1,  0},
    [NATURE_BASHFUL] = { 0,  0,  0,  0,  0},
    [NATURE_RASH]    = { 0,  0,  0, +1, -1},
    [NATURE_CALM]    = {-1,  0,  0,  0, +1},
    [NATURE_GENTLE]  = { 0, -1,  0,  0, +1},
    [NATURE_SASSY]   = { 0,  0, -1,  0, +1},
    [NATURE_CAREFUL] = { 0,  0,  0, -1, +1},
    [NATURE_QUIRKY]  = { 0,  0,  0,  0,  0},
};

static const s8 sPokeblockFlavorCompatibilityTable[] =
{
       // Cool, Beauty, Cute, Smart, Tough
          0,      0,    0,     0,     0, // Hardy
          1,      0,    0,     0,    -1, // Lonely
          1,      0,   -1,     0,     0, // Brave
          1,     -1,    0,     0,     0, // Adamant
          1,      0,    0,    -1,     0, // Naughty
         -1,      0,    0,     0,     1, // Bold
          0,      0,    0,     0,     0, // Docile
          0,      0,   -1,     0,     1, // Relaxed
          0,     -1,    0,     0,     1, // Impish
          0,      0,    0,    -1,     1, // Lax
         -1,      0,    1,     0,     0, // Timid
          0,      0,    1,     0,    -1, // Hasty
          0,      0,    0,     0,     0, // Serious
          0,     -1,    1,     0,     0, // Jolly
          0,      0,    1,    -1,     0, // Naive
         -1,      1,    0,     0,     0, // Modest
          0,      1,    0,     0,    -1, // Mild
          0,      1,   -1,     0,     0, // Quiet
          0,      0,    0,     0,     0, // Bashful
          0,      1,    0,    -1,     0, // Rash
         -1,      0,    0,     1,     0, // Calm
          0,      0,    0,     1,    -1, // Gentle
          0,      0,   -1,     1,     0, // Sassy
          0,     -1,    0,     1,     0, // Careful
          0,      0,    0,     0,     0  // Quirky
};

const u8 gPPUpGetMask[] = { 0x03, 0x0c, 0x30, 0xc0 }; // Masks for getting PP Up count, also PP Max values
const u8 gPPUpSetMask[] = { 0xfc, 0xf3, 0xcf, 0x3f }; // Masks for setting PP Up count
const u8 gPPUpAddMask[] = { 0x01, 0x04, 0x10, 0x40 }; // Values added to PP Up count

const u8 gStatStageRatios[][2] =
{
    { 10, 40 },
    { 10, 35 },
    { 10, 30 },
    { 10, 25 },
    { 10, 20 }, 
    { 10, 15 },
    { 10, 10 },
    { 15, 10 },
    { 20, 10 },
    { 25, 10 },
    { 30, 10 },
    { 35, 10 },
    { 40, 10 },
    { 138, 174 },
    { 108, 120 },
};

const struct SpriteTemplate gSpriteTemplates_Battlers[] = 
{
    [B_POSITION_PLAYER_LEFT] = {
        .tileTag = SPRITE_INVALID_TAG,
        .paletteTag = 0,
        .oam = &gOamData_BattlerPlayer,
        .anims = NULL, 
        .images = gSpriteImages_BattlerPlayerLeft,
        .affineAnims = gSpriteAffineAnimTable_BattlerPlayer,
        .callback = SpriteCB_AllyMon,
    },
    [B_POSITION_OPPONENT_LEFT] = {
        .tileTag = SPRITE_INVALID_TAG,
        .paletteTag = 0,
        .oam = &gOamData_BattlerOpponent,
        .anims = NULL, 
        .images = gSpriteImages_BattlerOpponentLeft,
        .affineAnims = gSpriteAffineAnimTable_BattlerOpponent,
        .callback = SpriteCB_EnemyMon,
    },
    [B_POSITION_PLAYER_RIGHT] = {
        .tileTag = SPRITE_INVALID_TAG,
        .paletteTag = 0,
        .oam = &gOamData_BattlerPlayer,
        .anims = NULL, 
        .images = gSpriteImages_BattlerPlayerRight,
        .affineAnims = gSpriteAffineAnimTable_BattlerPlayer,
        .callback = SpriteCB_AllyMon,
    },
    [B_POSITION_OPPONENT_RIGHT] = {
        .tileTag = SPRITE_INVALID_TAG,
        .paletteTag = 0,
        .oam = &gOamData_BattlerOpponent,
        .anims = NULL, 
        .images = gSpriteImages_BattlerOpponentRight,
        .affineAnims = gSpriteAffineAnimTable_BattlerOpponent,
        .callback = SpriteCB_EnemyMon,
    },
};

const struct SpriteTemplate gSpriteTemplates_TrainerBackpics[] = 
{
    {
        .tileTag = SPRITE_INVALID_TAG,
        .paletteTag = 0,
        .oam = &gOamData_BattlerPlayer,
        .anims = NULL, 
        .images = gTrainerBackPicTable_Red,
        .affineAnims = gSpriteAffineAnimTable_BattlerPlayer,
        .callback = SpriteCB_AllyMon,
    },
    {
        .tileTag = SPRITE_INVALID_TAG,
        .paletteTag = 0,
        .oam = &gOamData_BattlerPlayer,
        .anims = NULL, 
        .images = gTrainerBackPicTable_Leaf,
        .affineAnims = gSpriteAffineAnimTable_BattlerPlayer,
        .callback = SpriteCB_AllyMon,
    },
    {
        .tileTag = SPRITE_INVALID_TAG,
        .paletteTag = 0,
        .oam = &gOamData_BattlerPlayer,
        .anims = NULL, 
        .images = gTrainerBackPicTable_RSBrendan,
        .affineAnims = gSpriteAffineAnimTable_BattlerPlayer,
        .callback = SpriteCB_AllyMon,
    },
    {
        .tileTag = SPRITE_INVALID_TAG,
        .paletteTag = 0,
        .oam = &gOamData_BattlerPlayer,
        .anims = NULL, 
        .images = gTrainerBackPicTable_RSMay,
        .affineAnims = gSpriteAffineAnimTable_BattlerPlayer,
        .callback = SpriteCB_AllyMon,
    },
    {
        .tileTag = SPRITE_INVALID_TAG,
        .paletteTag = 0,
        .oam = &gOamData_BattlerPlayer,
        .anims = NULL, 
        .images = gTrainerBackPicTable_Pokedude,
        .affineAnims = gSpriteAffineAnimTable_BattlerPlayer,
        .callback = SpriteCB_AllyMon,
    },
    {
        .tileTag = SPRITE_INVALID_TAG,
        .paletteTag = 0,
        .oam = &gOamData_BattlerPlayer,
        .anims = NULL, 
        .images = gTrainerBackPicTable_OldMan,
        .affineAnims = gSpriteAffineAnimTable_BattlerPlayer,
        .callback = SpriteCB_AllyMon,
    },
};

static const s8 sFriendshipEventDeltas[][3] = 
{
    [FRIENDSHIP_EVENT_GROW_LEVEL]           = { 5,  3,  2 },
    [FRIENDSHIP_EVENT_VITAMIN]              = { 5,  3,  2 },
    [FRIENDSHIP_EVENT_BATTLE_ITEM]          = { 1,  1,  0 },
    [FRIENDSHIP_EVENT_LEAGUE_BATTLE]        = { 3,  2,  1 },
    [FRIENDSHIP_EVENT_LEARN_TMHM]           = { 1,  1,  0 },
    [FRIENDSHIP_EVENT_WALKING]              = { 1,  1,  1 },
    [FRIENDSHIP_EVENT_MASSAGE]              = { 3,  3,  3 },
    [FRIENDSHIP_EVENT_FAINT_SMALL]          = {-1, -1, -1 },
    [FRIENDSHIP_EVENT_FAINT_OUTSIDE_BATTLE] = {-5, -5, -10 },
    [FRIENDSHIP_EVENT_FAINT_LARGE]          = {-5, -5, -10 },
};

static const u16 sLinkPlayerFacilityClasses[] = 
{
    // Male
    FACILITY_CLASS_COOLTRAINER_M,
    FACILITY_CLASS_BLACK_BELT,
    FACILITY_CLASS_CAMPER,
    FACILITY_CLASS_YOUNGSTER,
    FACILITY_CLASS_PSYCHIC_M,
    FACILITY_CLASS_BUG_CATCHER,
    FACILITY_CLASS_TAMER,
    FACILITY_CLASS_JUGGLER,
    // Female
    FACILITY_CLASS_COOLTRAINER_F,
    FACILITY_CLASS_CHANNELER,
    FACILITY_CLASS_PICNICKER,
    FACILITY_CLASS_LASS,
    FACILITY_CLASS_PSYCHIC_F,
    FACILITY_CLASS_CRUSH_GIRL,
    FACILITY_CLASS_PKMN_BREEDER,
    FACILITY_CLASS_BEAUTY,
};

// code
void ZeroBoxMonData(struct BoxPokemon *boxMon)
{
    u8 *raw = (u8 *)boxMon;
    u32 i;
	
    for (i = 0; i < sizeof(struct BoxPokemon); i++)
        raw[i] = 0;
}

void ZeroMonData(struct Pokemon *mon)
{
    u32 arg;
	
    ZeroBoxMonData(&mon->box);
    arg = 0;
    SetMonData(mon, MON_DATA_STATUS, &arg);
    SetMonData(mon, MON_DATA_LEVEL, &arg);
    SetMonData(mon, MON_DATA_HP, &arg);
    SetMonData(mon, MON_DATA_MAX_HP, &arg);
    SetMonData(mon, MON_DATA_ATK, &arg);
    SetMonData(mon, MON_DATA_DEF, &arg);
    SetMonData(mon, MON_DATA_SPEED, &arg);
    SetMonData(mon, MON_DATA_SPATK, &arg);
    SetMonData(mon, MON_DATA_SPDEF, &arg);
    arg = 255;
    SetMonData(mon, MON_DATA_MAIL, &arg);
}

void ZeroPlayerPartyMons(void)
{
    u8 i;
	
    for (i = 0; i < PARTY_SIZE; i++)
        ZeroMonData(&gPlayerParty[i]);
}

void ZeroEnemyPartyMons(void)
{
    u8 i;
	
    for (i = 0; i < PARTY_SIZE; i++)
        ZeroMonData(&gEnemyParty[i]);
}

static void GiveMonInitialMoveset(struct Pokemon *mon)
{
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    s32 level = GetLevelFromMonExp(mon);
    s32 i;

    for (i = 0; gLevelUpLearnsets[species][i].move != LEVEL_UP_END; i++)
    {
        u8 moveLevel;
        u16 move;

        moveLevel = gLevelUpLearnsets[species][i].level;
		
		if (moveLevel == 0)
			continue;
        else if (moveLevel > level)
            break;

        move = gLevelUpLearnsets[species][i].move;
		
		if (GiveMoveToMon(mon, move) == MON_HAS_MAX_MOVES)
			DeleteFirstMoveAndGiveMoveToMon(mon, move);
    }
}

u32 GetShinyRollsIncrease(void)
{
	u32 shinyRollsIncrease = 1;
	
	if (gIsFishingEncounter)
		shinyRollsIncrease += 1 + 2 * gChainFishingStreak;
	
	if (CheckBagHasItem(ITEM_SHINY_CHARM, 1))
		shinyRollsIncrease += 2;
	
	return shinyRollsIncrease;
}

static inline bool8 DoesForcedGenderMatches(struct PokemonGenerator generator, u32 pid)
{
	if (!generator.forceGender || generator.forcedGender == GetGenderFromSpeciesAndPersonality(generator.species, pid))
		return TRUE; // Gender is't forced or matches
	return FALSE;
}

// There's MON_DATA_NATURE that can change it. But this way allows generate a default personality that contais the wanted nature,
// which can modify the pokémon form if based on the personality's nature.
static inline bool8 DoesForcedNatureMatches(struct PokemonGenerator generator, u32 pid)
{
	if (!generator.forceNature || generator.forcedNature == GetNatureFromPersonality(pid))
		return TRUE; // Nature is't forced or matches
	return FALSE;
}

static bool8 IsShinyOtIdPersonality(u32 otId, u32 personality)
{
	u32 shinyValue = HIHALF(otId) ^ LOHALF(otId) ^ HIHALF(personality) ^ LOHALF(personality);
	
    if (shinyValue < SHINY_ODDS)
        return TRUE;
	return FALSE;
}

void CreateMon(struct Pokemon *mon, struct PokemonGenerator generator)
{
	u8 i, speciesName[POKEMON_NAME_LENGTH + 1];
    u32 otId, value, personality, shinyRolls;
	u16 *formChanges, newSpecies, species;
	bool8 isShiny, shinyRerolls = FALSE;
	
    ZeroMonData(mon);
	ZeroBoxMonData(&mon->box);
	
	// Choose otId
	switch (generator.otIdType)
	{
		case OT_ID_PLAYER_ID:
			otId = GetPlayerTrainerId();
			shinyRerolls = TRUE; // Only rerolls shiny if it's generated with the player's otId
			break;
		case OT_ID_RANDOM:
			otId = Random32(); // Has a random otId
			break;
		case OT_ID_PRESET:
			otId = generator.fixedOtId; // Has a fixed otId
			break;
	}
	// Set otId
    SetMonData(mon, MON_DATA_OT_ID, &otId);
	
	// Set Ot name
    SetMonData(mon, MON_DATA_OT_NAME, gSaveBlock2Ptr->playerName);
	
	// Set Ot gender
    SetMonData(mon, MON_DATA_OT_GENDER, &gSaveBlock2Ptr->playerGender);
	
	// Choose personality
	if (generator.hasFixedPersonality)
		personality = generator.fixedPersonality;
	else
	{
		// Try force gender and nature
		do
		{
			personality = Random32();
			
			if (DoesForcedGenderMatches(generator, personality) && DoesForcedNatureMatches(generator, personality))
				break;
		} while (TRUE);
	}
	// Set personality
    SetMonData(mon, MON_DATA_PERSONALITY, &personality);
	
	// Calc shiny rate
	switch (generator.shinyType)
	{
		case GENERATE_SHINY_LOCKED:
		    isShiny = FALSE;
			break;
		case GENERATE_SHINY_FORCED:
		    isShiny = TRUE;
			break;
		default:
		    if (shinyRerolls)
			{
				shinyRolls = GetShinyRollsIncrease();
				value = personality;
				
				do
				{
					isShiny = IsShinyOtIdPersonality(otId, value);
					if (isShiny)
						break;
					
					value = Random32();
					--shinyRolls;
					
				} while (shinyRolls > 0);
			}
			else
				isShiny = IsShinyOtIdPersonality(otId, personality);
			break;
	}
	// Set shiny
	SetMonData(mon, MON_DATA_IS_SHINY, &isShiny);
	
	// Set nature
	value = GetNatureFromPersonality(personality);
    SetMonData(mon, MON_DATA_NATURE, &value);
	
	// Try change form
	species = generator.species;
	
	if (generator.changeForm)
	{
		if (generator.formChanges != NULL)
			formChanges = generator.formChanges;
		else // Default form changes
		{
			u16 defaultFormChanges[] = {FORM_CHANGE_GENDER, FORM_CHANGE_PERSONALITY, FORM_CHANGE_SEASON, FORM_CHANGE_NATURE, FORM_CHANGE_TERMINATOR};
			formChanges = defaultFormChanges;
		}
		
		for (i = 0; formChanges[i] != FORM_CHANGE_TERMINATOR; i++)
		{
			newSpecies = GetMonFormChangeSpecies(mon, species, formChanges[i]);
			
			if (newSpecies && newSpecies < NUM_SPECIES)
				species = newSpecies;
		}
	}
	// Set species
    SetMonData(mon, MON_DATA_SPECIES, &species);
	
	// Set species name
    GetSpeciesName(speciesName, species);
    SetMonData(mon, MON_DATA_NICKNAME, speciesName);
	
	// Set ivs
	for (i = 0; i < NUM_STATS; i++)
	{
		value = RandomRange(0, MAX_PER_STAT_IVS);
		SetMonData(mon, MON_DATA_HP_IV + i, &value);
	}
	
	// Set pokeball
    value = ITEM_POKE_BALL;
    SetMonData(mon, MON_DATA_POKEBALL, &value);
	
	// Set language
    SetMonData(mon, MON_DATA_LANGUAGE, &gGameLanguage);
	
	// Set met game
    SetMonData(mon, MON_DATA_MET_GAME, &gGameVersion);
	
	// Set met location
    value = GetCurrentRegionMapSectionId();
    SetMonData(mon, MON_DATA_MET_LOCATION, &value);
	
	// Set met level
    SetMonData(mon, MON_DATA_MET_LEVEL, &generator.level);
	
	// Set level
	SetMonData(mon, MON_DATA_LEVEL, &generator.level);
	
	// Set exp
    SetMonData(mon, MON_DATA_EXP, &gExperienceTables[gBaseStats[species].growthRate][generator.level]);
	
	// Set friendship
    SetMonData(mon, MON_DATA_FRIENDSHIP, &gBaseStats[species].friendship);
	
	// Set ability num
    if (gBaseStats[species].abilities[1])
    {
        value = personality & 1;
        SetMonData(mon, MON_DATA_ABILITY_NUM, &value);
    }
	
	// Set tera type
	value = RandomMax(2) ? gBaseStats[species].type1 : gBaseStats[species].type2;
	SetMonData(mon, MON_DATA_TERA_TYPE, &value);
	
	// Set ability hidden
#if WILD_HIDDEN_ABILITY_CHANCE != 0
    if (RandomPercent(WILD_HIDDEN_ABILITY_CHANCE))
    {
	    value = TRUE;
	    SetMonData(mon, MON_DATA_ABILITY_HIDDEN, &value);
    }
#endif
	
	// Reset mail
    value = 255;
    SetMonData(mon, MON_DATA_MAIL, &value);

	// Give initial moveset
	GiveMonInitialMoveset(mon);
	
	for (i = 0; i < MAX_MON_MOVES; i++)
	{
		if (generator.moves[i])
			SetMonMoveSlot(mon, generator.moves[i], i);
	}
	
	// Calculate stats
	CalculateMonStats(mon);
}

#define CALC_STAT(base, iv, ev, statIndex, field)                                 \
{                                                                                 \
    s32 n = (((2 * gBaseStats[species].base + iv + ev / 4) * level) / 100) + 5;   \
    n = ModifyStatByNature(GetMonData(mon, MON_DATA_NATURE, NULL), n, statIndex); \
    SetMonData(mon, field, &n);                                                   \
}

static u16 ModifyStatByNature(u8 nature, u16 n, u8 statIndex)
{
	if (statIndex >= STAT_ATK && statIndex <= NUM_STATS - 1)
	{
		switch (gNatureStatTable[nature][statIndex - 1])
		{
			case 1:
				n = (n * 110) / 100;
				break;
			case -1:
				n = (n * 90) / 100;
				break;
		}
	}
    return n;
}

void CalculateMonStats(struct Pokemon *mon)
{
    s32 oldMaxHP = GetMonData(mon, MON_DATA_MAX_HP, NULL);
    s32 currentHP = GetMonData(mon, MON_DATA_HP, NULL);
    s32 hpIV = GetMonData(mon, MON_DATA_HP_IV, NULL);
    s32 hpEV = GetMonData(mon, MON_DATA_HP_EV, NULL);
    s32 attackIV = GetMonData(mon, MON_DATA_ATK_IV, NULL);
    s32 attackEV = GetMonData(mon, MON_DATA_ATK_EV, NULL);
    s32 defenseIV = GetMonData(mon, MON_DATA_DEF_IV, NULL);
    s32 defenseEV = GetMonData(mon, MON_DATA_DEF_EV, NULL);
    s32 speedIV = GetMonData(mon, MON_DATA_SPEED_IV, NULL);
    s32 speedEV = GetMonData(mon, MON_DATA_SPEED_EV, NULL);
    s32 spAttackIV = GetMonData(mon, MON_DATA_SPATK_IV, NULL);
    s32 spAttackEV = GetMonData(mon, MON_DATA_SPATK_EV, NULL);
    s32 spDefenseIV = GetMonData(mon, MON_DATA_SPDEF_IV, NULL);
    s32 spDefenseEV = GetMonData(mon, MON_DATA_SPDEF_EV, NULL);
    u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    s32 level = GetLevelFromMonExp(mon);
    s32 newMaxHP;

    SetMonData(mon, MON_DATA_LEVEL, &level);

    if (species == SPECIES_SHEDINJA)
        newMaxHP = 1;
    else
    {
        s32 n = 2 * gBaseStats[species].baseHP + hpIV;
        newMaxHP = (((n + hpEV / 4) * level) / 100) + level + 10;
    }

    gBattleScripting.field_23 = newMaxHP - oldMaxHP;
    if (gBattleScripting.field_23 == 0)
        gBattleScripting.field_23 = 1;

    SetMonData(mon, MON_DATA_MAX_HP, &newMaxHP);
    CALC_STAT(baseAttack, attackIV, attackEV, STAT_ATK, MON_DATA_ATK)
    CALC_STAT(baseDefense, defenseIV, defenseEV, STAT_DEF, MON_DATA_DEF)
    CALC_STAT(baseSpeed, speedIV, speedEV, STAT_SPEED, MON_DATA_SPEED)
    CALC_STAT(baseSpAttack, spAttackIV, spAttackEV, STAT_SPATK, MON_DATA_SPATK)
    CALC_STAT(baseSpDefense, spDefenseIV, spDefenseEV, STAT_SPDEF, MON_DATA_SPDEF)

    if (species == SPECIES_SHEDINJA)
    {
        if (currentHP != 0 || oldMaxHP == 0)
            currentHP = 1;
        else
            return;
    }
    else
    {
        if (currentHP == 0 && oldMaxHP == 0)
            currentHP = newMaxHP;
        else if (currentHP != 0)
		{
            currentHP += newMaxHP - oldMaxHP;
            if (currentHP <= 0)
                currentHP = 1;
        }
        else
            return;
    }
    SetMonData(mon, MON_DATA_HP, &currentHP);
}

void BoxMonToMon(struct BoxPokemon *src, struct Pokemon *dest)
{
    u32 value = 0;
	
    dest->box = *src;
    SetMonData(dest, MON_DATA_STATUS, &value);
    SetMonData(dest, MON_DATA_HP, &value);
    SetMonData(dest, MON_DATA_MAX_HP, &value);
    value = 255;
    SetMonData(dest, MON_DATA_MAIL, &value);
    CalculateMonStats(dest);
}

static u8 GetLevelFromMonExp(struct Pokemon *mon)
{
	return GetLevelFromBoxMonExp(&mon->box);
}

u8 GetLevelFromBoxMonExp(struct BoxPokemon *boxMon)
{
    u16 species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
    u32 exp = GetBoxMonData(boxMon, MON_DATA_EXP, NULL);
    s32 level = 1;

    while (level <= MAX_LEVEL && gExperienceTables[gBaseStats[species].growthRate][level] <= exp)
        level++;

    return level - 1;
}

u8 GiveMoveToMon(struct Pokemon *mon, u16 move)
{
	u8 i;
	
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        u16 existingMove = GetMonData(mon, MON_DATA_MOVE1 + i, NULL);
		
        if (!existingMove)
        {
			SetMonMoveSlot(mon, move, i);
            return MON_LEARNED_MOVE;
        }
		
        if (existingMove == move)
            return MON_ALREADY_KNOWS_MOVE;
    }
    return MON_HAS_MAX_MOVES;
}

static void SetMonMoveSlotInternal(struct Pokemon *mon, u8 slot, u16 move, u8 pp)
{
	SetMonData(mon, MON_DATA_MOVE1 + slot, &move);
	SetMonData(mon, MON_DATA_PP1 + slot, &pp);
	DoOverworldFormChange(mon, FORM_CHANGE_KNOW_MOVE);
}

void SetMonMoveSlot(struct Pokemon *mon, u16 move, u8 slot)
{
	SetMonMoveSlotInternal(mon, slot, move, gBattleMoves[move].pp);
}

void DeleteFirstMoveAndGiveMoveToMon(struct Pokemon *mon, u16 move)
{
	s32 i;
    u16 moves[MAX_MON_MOVES];
    u8 pp[MAX_MON_MOVES];
    u8 ppBonuses;

    for (i = 0; i < MAX_MON_MOVES - 1; i++)
    {
        moves[i] = GetMonData(mon, MON_DATA_MOVE2 + i, NULL);
        pp[i] = GetMonData(mon, MON_DATA_PP2 + i, NULL);
    }

    ppBonuses = GetMonData(mon, MON_DATA_PP_BONUSES, NULL);
    ppBonuses >>= 2;
    moves[MAX_MON_MOVES - 1] = move;
    pp[MAX_MON_MOVES - 1] = gBattleMoves[move].pp;

    for (i = 0; i < MAX_MON_MOVES; i++)
		SetMonMoveSlotInternal(mon, i, moves[i], pp[i]);

    SetMonData(mon, MON_DATA_PP_BONUSES, &ppBonuses);
}

static void ShiftMoveSlot(struct Pokemon *mon, u8 slotTo, u8 slotFrom)
{
    u16 move1 = GetMonData(mon, MON_DATA_MOVE1 + slotTo, NULL);
    u16 move0 = GetMonData(mon, MON_DATA_MOVE1 + slotFrom, NULL);
    u8 pp1 = GetMonData(mon, MON_DATA_PP1 + slotTo, NULL);
    u8 pp0 = GetMonData(mon, MON_DATA_PP1 + slotFrom, NULL);
    u8 ppBonuses = GetMonData(mon, MON_DATA_PP_BONUSES, NULL);
    u8 ppBonusMask1 = gPPUpGetMask[slotTo];
    u8 ppBonusMove1 = (ppBonuses & ppBonusMask1) >> (slotTo * 2);
    u8 ppBonusMask2 = gPPUpGetMask[slotFrom];
    u8 ppBonusMove2 = (ppBonuses & ppBonusMask2) >> (slotFrom * 2);

    ppBonuses &= ~(ppBonusMask1);
    ppBonuses &= ~(ppBonusMask2);
    ppBonuses |= (ppBonusMove1 << (slotFrom * 2)) + (ppBonusMove2 << (slotTo * 2));
	
    SetMonData(mon, MON_DATA_MOVE1 + slotTo, &move0);
    SetMonData(mon, MON_DATA_MOVE1 + slotFrom, &move1);
    SetMonData(mon, MON_DATA_PP1 + slotTo, &pp0);
    SetMonData(mon, MON_DATA_PP1 + slotFrom, &pp1);
    SetMonData(mon, MON_DATA_PP_BONUSES, &ppBonuses);
}

void DeleteMonMove(struct Pokemon *mon, u8 movePos)
{
	u8 i;
	
	SetMonMoveSlot(mon, MOVE_NONE, movePos);
	RemoveMonPPBonus(mon, movePos);
	
	for (i = movePos; i < MAX_MON_MOVES - 1; ++i)
		ShiftMoveSlot(mon, i, i + 1);
}

u8 MonTryLearningNewMove(struct Pokemon *mon, bool8 firstMove)
{
    u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    u8 level = GetMonData(mon, MON_DATA_LEVEL, NULL);

    // since you can learn more than one move per level
    // the game needs to know whether you decided to
    // learn it or keep the old set to avoid asking
    // you to learn the same move over and over again
    if (firstMove)
    {
        sLearningMoveTableID = 0;

        while (gLevelUpLearnsets[species][sLearningMoveTableID].level != level)
        {
            if (gLevelUpLearnsets[species][++sLearningMoveTableID].move == LEVEL_UP_END)
                return MON_DONT_FIND_MOVE_TO_LEARN;
        }
    }
	
    if (gLevelUpLearnsets[species][sLearningMoveTableID].level == level)
    {
		gMoveToLearn = gLevelUpLearnsets[species][sLearningMoveTableID++].move;
		return GiveMoveToMon(mon, gMoveToLearn);
    }
    return MON_DONT_FIND_MOVE_TO_LEARN;
}

u8 MonTryLearningNewMoveAfterEvolution(struct Pokemon *mon, bool8 firstMove)
{
	u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
	u8 moveLevel, level = GetMonData(mon, MON_DATA_LEVEL, NULL);
	
	// since you can learn more than one move per level
	// the game needs to know whether you decided to
	// learn it or keep the old set to avoid asking
	// you to learn the same move over and over again
	if (firstMove)
		sLearningMoveTableID = 0;
	
	while (gLevelUpLearnsets[species][sLearningMoveTableID].move != LEVEL_UP_END)
	{
		moveLevel = gLevelUpLearnsets[species][sLearningMoveTableID].level;
		
		while (moveLevel == 0 || moveLevel == level)
		{
			gMoveToLearn = gLevelUpLearnsets[species][sLearningMoveTableID++].move;
			return GiveMoveToMon(mon, gMoveToLearn);
		}
		sLearningMoveTableID++;
	}
	return MON_DONT_FIND_MOVE_TO_LEARN;
}

u8 GetNumOfBadges(void)
{
	u8 NumOfBadges = 0;
	
	while (FlagGet(FLAG_BADGE01_GET + NumOfBadges))
	{
		if (++NumOfBadges == NUM_BADGES)
			break;
	}
	return NumOfBadges;
}

u8 GetMonGender(struct Pokemon *mon)
{
    return GetBoxMonGender(&mon->box);
}

u8 GetBoxMonGender(struct BoxPokemon *boxMon)
{
	return GetGenderFromSpeciesAndPersonality(GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL), GetBoxMonData(boxMon, MON_DATA_PERSONALITY, NULL));
}

u8 GetGenderFromSpeciesAndPersonality(u16 species, u32 personality)
{
    switch (gBaseStats[species].genderRatio)
    {
    case MON_MALE:
    case MON_FEMALE:
    case MON_GENDERLESS:
        return gBaseStats[species].genderRatio;
    }

    if (gBaseStats[species].genderRatio > (personality & 0xFF))
        return MON_FEMALE;
    else
        return MON_MALE;
}

void SetMultiuseSpriteTemplateToPokemon(u16 speciesTag, u8 battlerPosition)
{
    if (gMonSpritesGfxPtr != NULL)
    {
        if (battlerPosition >= MAX_BATTLERS_COUNT)
            battlerPosition = 0;

        gMultiuseSpriteTemplate = gMonSpritesGfxPtr->templates[battlerPosition];
    }
    else
    {
        if (gOakSpeechNidoranResources)
        {
            if (battlerPosition >= (s8)gOakSpeechNidoranResources->battlePosition) // why a cast?!? changing the unk0_2 type to s8 causes extra shifts, but a cast is the correct fix. why, compiler?
                battlerPosition = 0;

            gMultiuseSpriteTemplate = gOakSpeechNidoranResources->templates[battlerPosition];
        }
        else
        {
            if (battlerPosition >= MAX_BATTLERS_COUNT)
                battlerPosition = 0;

            gMultiuseSpriteTemplate = gSpriteTemplates_Battlers[battlerPosition];
        }
    }
    gMultiuseSpriteTemplate.paletteTag = speciesTag;
    gMultiuseSpriteTemplate.anims = gSpriteAnimTable_82349BC;
}

void SetMultiuseSpriteTemplateToTrainerBack(u16 trainerSpriteId, u8 battlerPosition)
{
    gMultiuseSpriteTemplate.paletteTag = trainerSpriteId;
	
    if (battlerPosition == B_POSITION_PLAYER_LEFT || battlerPosition == B_POSITION_PLAYER_RIGHT)
    {
        gMultiuseSpriteTemplate = gSpriteTemplates_TrainerBackpics[trainerSpriteId];
        gMultiuseSpriteTemplate.anims = gTrainerBackAnimsPtrTable[trainerSpriteId];
    }
    else
    {
		gMultiuseSpriteTemplate = gMonSpritesGfxPtr != NULL ? gMonSpritesGfxPtr->templates[battlerPosition] : gSpriteTemplates_Battlers[battlerPosition];
        gMultiuseSpriteTemplate.anims = gTrainerFrontAnimsPtrTable[trainerSpriteId];
    }
}

u32 GetMonData(struct Pokemon *mon, s32 field, u8* data)
{
    u32 ret;

    switch (field)
    {
    case MON_DATA_STATUS:
        ret = mon->status;
        break;
    case MON_DATA_LEVEL:
        ret = mon->level;
        break;
    case MON_DATA_HP:
        ret = mon->hp;
        break;
    case MON_DATA_MAX_HP:
        ret = mon->maxHP;
        break;
    case MON_DATA_ATK:
        ret = mon->attack;
        break;
    case MON_DATA_DEF:
        ret = mon->defense;
        break;
    case MON_DATA_SPEED:
        ret = mon->speed;
        break;
    case MON_DATA_SPATK:
        ret = mon->spAttack;
        break;
    case MON_DATA_SPDEF:
        ret = mon->spDefense;
        break;
    case MON_DATA_MAIL:
        ret = mon->mail;
        break;
    default:
        ret = GetBoxMonData(&mon->box, field, data);
        break;
    }
    return ret;
}

u32 GetBoxMonData(struct BoxPokemon *boxMon, s32 field, u8 *data)
{
    s32 i;
    u32 retVal = 0;
    
    switch (field)
    {
    case MON_DATA_PERSONALITY:
        retVal = boxMon->personality;
        break;
    case MON_DATA_OT_ID:
        retVal = boxMon->otId;
        break;
	case MON_DATA_TERA_TYPE:
	    retVal = boxMon->teraType;
		break;
    case MON_DATA_NICKNAME:
        if (boxMon->isEgg)
        {
            StringCopy(data, gText_EggNickname);
            retVal = StringLength(data);
        }
        else if (boxMon->language == LANGUAGE_JAPANESE)
        {
            data[0] = EXT_CTRL_CODE_BEGIN;
            data[1] = EXT_CTRL_CODE_JPN;

            // FRLG changed i < 7 to i < 6
            for (retVal = 2, i = 0; i < 6 && boxMon->nickname[i] != EOS; data[retVal] = boxMon->nickname[i], retVal++, i++) {}

            data[retVal++] = EXT_CTRL_CODE_BEGIN;
            data[retVal++] = EXT_CTRL_CODE_ENG;
            data[retVal] = EOS;
        }
        else
        {
            for (retVal = 0; retVal < POKEMON_NAME_LENGTH; data[retVal] = boxMon->nickname[retVal], retVal++) {}
            data[retVal] = EOS;
        }
        break;
    case MON_DATA_LANGUAGE:
        retVal = boxMon->language;
        break;
    case MON_DATA_SANITY_HAS_SPECIES:
        retVal = boxMon->species != SPECIES_NONE;
        break;
    case MON_DATA_OT_NAME:
        // FRLG changed this to 7 which used to be PLAYER_NAME_LENGTH + 1
        while (retVal < 7)
        {
            data[retVal] = boxMon->otName[retVal];
            retVal++;
        }
        data[retVal] = EOS;
        break;
    case MON_DATA_MARKINGS:
        retVal = boxMon->markings;
        break;
    case MON_DATA_SPECIES:
        retVal = boxMon->species;
        break;
    case MON_DATA_HELD_ITEM:
        retVal = boxMon->heldItem;
        break;
    case MON_DATA_EXP:
        retVal = boxMon->experience;
        break;
    case MON_DATA_PP_BONUSES:
        retVal = boxMon->ppBonuses;
        break;
    case MON_DATA_FRIENDSHIP:
        retVal = boxMon->friendship;
        break;
    case MON_DATA_NATURE:
        retVal = boxMon->nature;
		break;
	case MON_DATA_DYNAMAX_LEVEL:
	    retVal = boxMon->dynamaxLevel;
		break;
	case MON_DATA_HAS_GMAX_FACTOR:
	    retVal = boxMon->gMaxFactor;
		break;
    case MON_DATA_MOVE1:
		retVal = boxMon->move1;
        break;
    case MON_DATA_MOVE2:
		retVal = boxMon->move2;
        break;
    case MON_DATA_MOVE3:
		retVal = boxMon->move3;
        break;
    case MON_DATA_MOVE4:
        retVal = boxMon->move4;
        break;
    case MON_DATA_PP1:
		retVal = boxMon->pp1;
        break;
    case MON_DATA_PP2:
		retVal = boxMon->pp2;
        break;
    case MON_DATA_PP3:
		retVal = boxMon->pp3;
        break;
    case MON_DATA_PP4:
        retVal = boxMon->pp4;
        break;
    case MON_DATA_HP_EV:
        retVal = boxMon->hpEV;
        break;
    case MON_DATA_ATK_EV:
        retVal = boxMon->attackEV;
        break;
    case MON_DATA_DEF_EV:
        retVal = boxMon->defenseEV;
        break;
    case MON_DATA_SPEED_EV:
        retVal = boxMon->speedEV;
        break;
    case MON_DATA_SPATK_EV:
        retVal = boxMon->spAttackEV;
        break;
    case MON_DATA_SPDEF_EV:
        retVal = boxMon->spDefenseEV;
        break;
    case MON_DATA_POKERUS:
        retVal = boxMon->pokerus;
        break;
    case MON_DATA_MET_LOCATION:
        retVal = boxMon->metLocation;
        break;
    case MON_DATA_MET_LEVEL:
        retVal = boxMon->metLevel;
        break;
    case MON_DATA_MET_GAME:
        retVal = boxMon->metGame;
        break;
    case MON_DATA_POKEBALL:
        retVal = boxMon->pokeball;
        break;
    case MON_DATA_OT_GENDER:
        retVal = boxMon->otGender;
        break;
    case MON_DATA_HP_IV:
        retVal = boxMon->hpIV;
        break;
    case MON_DATA_ATK_IV:
        retVal = boxMon->attackIV;
        break;
    case MON_DATA_DEF_IV:
        retVal = boxMon->defenseIV;
        break;
    case MON_DATA_SPEED_IV:
        retVal = boxMon->speedIV;
        break;
    case MON_DATA_SPATK_IV:
        retVal = boxMon->spAttackIV;
        break;
    case MON_DATA_SPDEF_IV:
        retVal = boxMon->spDefenseIV;
        break;
    case MON_DATA_IS_EGG:
        retVal = boxMon->isEgg;
        break;
    case MON_DATA_ABILITY_NUM:
        retVal = boxMon->abilityNum;
        break;
    case MON_DATA_ABILITY_HIDDEN:
        retVal = boxMon->abilityHidden;
		break;
    case MON_DATA_SPECIES2:
        retVal = boxMon->species;
        if (boxMon->species && boxMon->isEgg)
            retVal = SPECIES_EGG;
        break;
	case MON_DATA_IS_SHINY:
		retVal = boxMon->shiny;
		break;
    case MON_DATA_IVS:
        retVal = boxMon->hpIV | (boxMon->attackIV << 5) | (boxMon->defenseIV << 10) | (boxMon->speedIV << 15) | (boxMon->spAttackIV << 20) | (boxMon->spDefenseIV << 25);
        break;
    case MON_DATA_KNOWN_MOVES:
        if (boxMon->species && !boxMon->isEgg)
        {
            u16 *moves = (u16 *)data;

            while (moves[i] != MOVES_COUNT_GMAX)
            {
                u16 move = moves[i];
                if (boxMon->move1 == move || boxMon->move2 == move || boxMon->move3 == move || boxMon->move4 == move)
                    retVal |= gBitTable[i];
                i++;
            }
        }
        break;
    default:
        break;
    }
    return retVal;
}

#define SET8(lhs) (lhs) = *data
#define SET16(lhs) (lhs) = READ_16(data)
#define SET32(lhs) (lhs) = READ_32(data)

void SetMonData(struct Pokemon *mon, s32 field, const void *dataArg)
{
    const u8 *data = dataArg;

    switch (field)
    {
    case MON_DATA_STATUS:
        SET32(mon->status);
        break;
    case MON_DATA_LEVEL:
        SET8(mon->level);
        break;
    case MON_DATA_HP:
        SET16(mon->hp);
        break;
    case MON_DATA_MAX_HP:
        SET16(mon->maxHP);
        break;
    case MON_DATA_ATK:
        SET16(mon->attack);
        break;
    case MON_DATA_DEF:
        SET16(mon->defense);
        break;
    case MON_DATA_SPEED:
        SET16(mon->speed);
        break;
    case MON_DATA_SPATK:
        SET16(mon->spAttack);
        break;
    case MON_DATA_SPDEF:
        SET16(mon->spDefense);
        break;
    case MON_DATA_MAIL:
        SET8(mon->mail);
        break;
    default:
        SetBoxMonData(&mon->box, field, data);
        break;
    }
}

void SetBoxMonData(struct BoxPokemon *boxMon, s32 field, const void *dataArg)
{
	s32 i;
    const u8 *data = dataArg;

    switch (field)
    {
    case MON_DATA_PERSONALITY:
        SET32(boxMon->personality);
        break;
    case MON_DATA_OT_ID:
        SET32(boxMon->otId);
        break;
	case MON_DATA_TERA_TYPE:
	    SET8(boxMon->teraType);
		break;
    case MON_DATA_NICKNAME:
        for (i = 0; i < POKEMON_NAME_LENGTH; i++)
            boxMon->nickname[i] = data[i];
        break;
    case MON_DATA_LANGUAGE:
        SET8(boxMon->language);
        break;
    case MON_DATA_OT_NAME:
        for (i = 0; i < 7; i++)
            boxMon->otName[i] = data[i];
        break;
    case MON_DATA_MARKINGS:
        SET8(boxMon->markings);
        break;
    case MON_DATA_SPECIES:
        SET16(boxMon->species);
        break;
    case MON_DATA_HELD_ITEM:
	    SET16(boxMon->heldItem);
		DoOverworldFormChange((struct Pokemon*)boxMon, FORM_CHANGE_HOLD_ITEM);
	    break;
    case MON_DATA_EXP:
        SET32(boxMon->experience);
        break;
	case MON_DATA_IS_SHINY:
		SET8(boxMon->shiny);
		break;
    case MON_DATA_PP_BONUSES:
        SET8(boxMon->ppBonuses);
        break;
    case MON_DATA_FRIENDSHIP:
        SET8(boxMon->friendship);
        break;
    case MON_DATA_MOVE1:
		SET16(boxMon->move1);
        break;
    case MON_DATA_MOVE2:
		SET16(boxMon->move2);
        break;
    case MON_DATA_MOVE3:
		SET16(boxMon->move3);
        break;
    case MON_DATA_MOVE4:
        SET16(boxMon->move4);
        break;
    case MON_DATA_PP1:
		SET8(boxMon->pp1);
		break;
    case MON_DATA_PP2:
		SET8(boxMon->pp2);
		break;
    case MON_DATA_PP3:
		SET8(boxMon->pp3);
		break;
    case MON_DATA_PP4:
        SET8(boxMon->pp4);
        break;
    case MON_DATA_HP_EV:
        SET8(boxMon->hpEV);
        break;
    case MON_DATA_ATK_EV:
        SET8(boxMon->attackEV);
        break;
    case MON_DATA_DEF_EV:
        SET8(boxMon->defenseEV);
        break;
    case MON_DATA_SPEED_EV:
        SET8(boxMon->speedEV);
        break;
    case MON_DATA_SPATK_EV:
        SET8(boxMon->spAttackEV);
        break;
    case MON_DATA_SPDEF_EV:
        SET8(boxMon->spDefenseEV);
        break;
    case MON_DATA_POKERUS:
        SET8(boxMon->pokerus);
        break;
    case MON_DATA_MET_LOCATION:
        SET8(boxMon->metLocation);
        break;
    case MON_DATA_MET_LEVEL:
		SET8(boxMon->metLevel);
        break;
    case MON_DATA_MET_GAME:
        SET8(boxMon->metGame);
        break;
    case MON_DATA_POKEBALL:
        SET8(boxMon->pokeball);
        break;
    case MON_DATA_OT_GENDER:
        SET8(boxMon->otGender);
        break;
    case MON_DATA_HP_IV:
        SET8(boxMon->hpIV);
        break;
    case MON_DATA_ATK_IV:
        SET8(boxMon->attackIV);
        break;
    case MON_DATA_DEF_IV:
        SET8(boxMon->defenseIV);
        break;
    case MON_DATA_SPEED_IV:
        SET8(boxMon->speedIV);
        break;
    case MON_DATA_SPATK_IV:
        SET8(boxMon->spAttackIV);
        break;
    case MON_DATA_SPDEF_IV:
        SET8(boxMon->spDefenseIV);
        break;
    case MON_DATA_IS_EGG:
        SET8(boxMon->isEgg);
        break;
    case MON_DATA_ABILITY_NUM:
        SET8(boxMon->abilityNum);
        break;
    case MON_DATA_ABILITY_HIDDEN:
		SET8(boxMon->abilityHidden);
		break;
    case MON_DATA_NATURE:
        SET8(boxMon->nature);
		break;
	case MON_DATA_DYNAMAX_LEVEL:
	    SET8(boxMon->dynamaxLevel);
		break;
	case MON_DATA_HAS_GMAX_FACTOR:
	    SET8(boxMon->gMaxFactor);
		break;
    case MON_DATA_IVS:
    {
        u32 ivs = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
		
        boxMon->hpIV = ivs & MAX_PER_STAT_IVS;
        boxMon->attackIV = (ivs >> 5) & MAX_PER_STAT_IVS;
        boxMon->defenseIV = (ivs >> 10) & MAX_PER_STAT_IVS;
        boxMon->speedIV = (ivs >> 15) & MAX_PER_STAT_IVS;
        boxMon->spAttackIV = (ivs >> 20) & MAX_PER_STAT_IVS;
        boxMon->spDefenseIV = (ivs >> 25) & MAX_PER_STAT_IVS;
        break;
    }
    default:
        break;
    }
}

void CopyMon(void *dest, void *src, size_t size)
{
    memcpy(dest, src, size);
}

static u8 SendMonToPC(struct Pokemon* mon)
{
    s32 boxNo, boxPos;

    SetPCBoxToSendMon(VarGet(VAR_PC_BOX_TO_SEND_MON));

    boxNo = StorageGetCurrentBox();

    do
    {
        for (boxPos = 0; boxPos < IN_BOX_COUNT; boxPos++)
        {
            struct BoxPokemon* checkingMon = GetBoxedMonPtr(boxNo, boxPos);
			
            if (!GetBoxMonData(checkingMon, MON_DATA_SPECIES, NULL))
            {
                MonRestorePP(mon);
                CopyMon(checkingMon, &mon->box, sizeof(mon->box));
				
                gSpecialVar_MonBoxId = boxNo;
                gSpecialVar_MonBoxPos = boxPos;
				
                if (GetPCBoxToSendMon() != boxNo)
                    FlagClear(FLAG_SHOWN_BOX_WAS_FULL_MESSAGE);
				
                VarSet(VAR_PC_BOX_TO_SEND_MON, boxNo);
                return MON_GIVEN_TO_PC;
            }
        }
        if (++boxNo == TOTAL_BOXES_COUNT)
            boxNo = 0;
    } while (boxNo != StorageGetCurrentBox());

    return MON_CANT_GIVE;
}

u8 GiveMonToPlayer(struct Pokemon *mon)
{
    u8 i;

    SetMonData(mon, MON_DATA_OT_NAME, gSaveBlock2Ptr->playerName);
    SetMonData(mon, MON_DATA_OT_GENDER, &gSaveBlock2Ptr->playerGender);
    SetMonData(mon, MON_DATA_OT_ID, gSaveBlock2Ptr->playerTrainerId);

    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) == SPECIES_NONE)
            break;
    }
    if (i >= PARTY_SIZE)
        return SendMonToPC(mon);

    CopyMon(&gPlayerParty[i], mon, sizeof(*mon));
    gPlayerPartyCount = i + 1;
	
    return MON_GIVEN_TO_PARTY;
}

u8 CalculatePlayerPartyCount(void)
{
    gPlayerPartyCount = 0;

    while (gPlayerPartyCount < PARTY_SIZE && GetMonData(&gPlayerParty[gPlayerPartyCount], MON_DATA_SPECIES, NULL))
        gPlayerPartyCount++;

    return gPlayerPartyCount;
}

u8 CalculateEnemyPartyCount(void)
{
    gEnemyPartyCount = 0;

    while (gEnemyPartyCount < PARTY_SIZE && GetMonData(&gEnemyParty[gEnemyPartyCount], MON_DATA_SPECIES, NULL))
        gEnemyPartyCount++;

    return gEnemyPartyCount;
}

u8 GetMonsStateToDoubles(void)
{
    u8 i, aliveCount;
	
    CalculatePlayerPartyCount();

    if (gPlayerPartyCount == 1)
        return PLAYER_HAS_ONE_MON;

    for (i = 0, aliveCount = 0; i < gPlayerPartyCount; i++)
    {
        if (MonCanBattle(&gPlayerParty[i]))
            aliveCount++;
    }
    return (aliveCount > 1) ? PLAYER_HAS_TWO_USABLE_MONS : PLAYER_HAS_ONE_USABLE_MON;
}

u16 GetAbilityBySpecies(u16 species, bool8 abilityNum, bool8 abilityHidden)
{
	return (abilityHidden && gBaseStats[species].hiddenAbility) ? gBaseStats[species].hiddenAbility : gBaseStats[species].abilities[abilityNum];
}

u16 GetMonAbility(struct Pokemon *mon)
{
    return GetAbilityBySpecies(GetMonData(mon, MON_DATA_SPECIES, NULL), GetMonData(mon, MON_DATA_ABILITY_NUM, NULL), GetMonData(mon, MON_DATA_ABILITY_HIDDEN, NULL));
}

static bool8 IsPokemonStorageFull(void)
{
    s32 i, j;

    for (i = 0; i < TOTAL_BOXES_COUNT; i++)
	{
        for (j = 0; j < IN_BOX_COUNT; j++)
		{
            if (!GetBoxMonDataAt(i, j, MON_DATA_SPECIES))
                return FALSE;
		}
	}
    return TRUE;
}

bool8 IsPlayerPartyAndPokemonStorageFull(void)
{
    u8 i;

    for (i = 0; i < PARTY_SIZE; i++)
	{
        if (!GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL))
            return FALSE;
	}
    return IsPokemonStorageFull();
}

void GetSpeciesName(u8 *name, u16 species)
{
    s32 i;
	
	species = SanitizeSpeciesId(species);

    // Hmm? FRLG has < while Ruby/Emerald has <=
    for (i = 0; i < POKEMON_NAME_LENGTH; i++)
    {
		name[i] = gSpeciesNames[species][i];
        if (name[i] == EOS)
            break;
    }
    name[i] = EOS;
}

u8 CalculatePPWithBonus(u16 move, u8 ppBonuses, u8 moveIndex)
{
    u8 basePP = gBattleMoves[move].pp;
    return basePP + ((basePP * 20 * ((gPPUpGetMask[moveIndex] & ppBonuses) >> (2 * moveIndex))) / 100);
}

void RemoveMonPPBonus(struct Pokemon *mon, u8 moveIndex)
{
    u8 ppBonuses = GetMonData(mon, MON_DATA_PP_BONUSES, NULL);
    ppBonuses &= gPPUpSetMask[moveIndex];
    SetMonData(mon, MON_DATA_PP_BONUSES, &ppBonuses);
}

bool8 HealStatusConditions(struct Pokemon *mon, u32 healMask, u8 battleId)
{
    u32 status = GetMonData(mon, MON_DATA_STATUS, NULL);

    if (status & healMask)
    {
		CopyStatusStringToBattleBuffer1(status);
        status &= ~(healMask);
        SetMonData(mon, MON_DATA_STATUS, &status);
		
        if (battleId != MAX_BATTLERS_COUNT)
		{
            gBattleMons[battleId].status1 &= ~(healMask);
			gBattleMons[battleId].status2 &= ~(STATUS2_NIGHTMARE); // fix nightmare glitch
		}
        return FALSE;
    }
	return TRUE;
}

u16 GetEvolutionTargetSpecies(struct Pokemon *mon, u8 type, u16 evolutionItem, struct Pokemon *tradePartner)
{
    int i, j;
    u16 data, friendship, targetSpecies = SPECIES_NONE;
    u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL), heldItem = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
	u16 partnerSpecies = SPECIES_NONE, partnerHeldItem = ITEM_NONE;
    u8 partnerHoldEffect, holdEffect = ItemId_GetHoldEffect(heldItem), weather, level;
	
	if (tradePartner != NULL)
	{
		partnerSpecies = GetMonData(tradePartner, MON_DATA_SPECIES, NULL);
		partnerHeldItem = GetMonData(tradePartner, MON_DATA_HELD_ITEM, NULL);
		partnerHoldEffect = ItemId_GetHoldEffect(partnerHeldItem);
	}
	else
		partnerHoldEffect = HOLD_EFFECT_NONE;

    if (holdEffect == HOLD_EFFECT_PREVENT_EVOLVE && type != EVO_MODE_ITEM_CHECK)
        return targetSpecies;

    switch (type)
    {
    case EVO_MODE_NORMAL:
        level = GetMonData(mon, MON_DATA_LEVEL, NULL);
        friendship = GetMonData(mon, MON_DATA_FRIENDSHIP, NULL);

        for (i = 0; i < EVOS_PER_MON; i++)
        {
            switch (gEvolutionTable[species][i].method)
            {
            case EVO_FRIENDSHIP:
                if (friendship >= 220)
                    targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            case EVO_FRIENDSHIP_DAY:
                if (!GetDNSTimeLapseIsNight() && friendship >= 220)
                    targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            case EVO_FRIENDSHIP_NIGHT:
                if (GetDNSTimeLapseIsNight() && friendship >= 220)
                    targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            case EVO_LEVEL:
			case EVO_LEVEL_NINJASK:
                if (gEvolutionTable[species][i].param <= level)
                    targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            case EVO_LEVEL_ATK_GT_DEF:
                if (gEvolutionTable[species][i].param <= level && GetMonData(mon, MON_DATA_ATK, NULL) > GetMonData(mon, MON_DATA_DEF, NULL))
					targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            case EVO_LEVEL_ATK_EQ_DEF:
                if (gEvolutionTable[species][i].param <= level && GetMonData(mon, MON_DATA_ATK, NULL) == GetMonData(mon, MON_DATA_DEF, NULL))
					targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
            case EVO_LEVEL_ATK_LT_DEF:
                if (gEvolutionTable[species][i].param <= level && GetMonData(mon, MON_DATA_ATK, NULL) < GetMonData(mon, MON_DATA_DEF, NULL))
					targetSpecies = gEvolutionTable[species][i].targetSpecies;
                break;
			case EVO_LEVEL_FEMALE:
			    if (gEvolutionTable[species][i].param <= level && GetMonGender(mon) == MON_FEMALE)
					targetSpecies = gEvolutionTable[species][i].targetSpecies;
				break;
			case EVO_LEVEL_MALE:
			    if (gEvolutionTable[species][i].param <= level && GetMonGender(mon) == MON_MALE)
					targetSpecies = gEvolutionTable[species][i].targetSpecies;
				break;
			case EVO_LEVEL_NIGHT:
			    if (gEvolutionTable[species][i].param <= level && GetDNSTimeLapseIsNight())
					targetSpecies = gEvolutionTable[species][i].targetSpecies;
				break;
			case EVO_LEVEL_DAY:
			    if (gEvolutionTable[species][i].param <= level && !GetDNSTimeLapseIsNight())
					targetSpecies = gEvolutionTable[species][i].targetSpecies;
				break;
			case EVO_LEVEL_DUSK:
			    if (gEvolutionTable[species][i].param <= level && gRtcLocation.hour >= AFTERNOON_OF_DAY_START && gRtcLocation.hour < AFTERNOON_OF_DAY_START + 1)
					targetSpecies = gEvolutionTable[species][i].targetSpecies;
				break;
			case EVO_ITEM_HOLD_DAY:
			    if (heldItem == gEvolutionTable[species][i].param && !GetDNSTimeLapseIsNight())
				{
					heldItem = ITEM_NONE;
					SetMonData(mon, MON_DATA_HELD_ITEM, &heldItem);
					targetSpecies = gEvolutionTable[species][i].targetSpecies;
				}
				break;
			case EVO_ITEM_HOLD_NIGHT:
			    if (heldItem == gEvolutionTable[species][i].param && GetDNSTimeLapseIsNight())
				{
					heldItem = ITEM_NONE;
					SetMonData(mon, MON_DATA_HELD_ITEM, &heldItem);
					targetSpecies = gEvolutionTable[species][i].targetSpecies;
				}
				break;
			case EVO_MOVE:
			    if (FindMoveSlotInMoveset(mon, gEvolutionTable[species][i].param) != MAX_MON_MOVES)
					targetSpecies = gEvolutionTable[species][i].targetSpecies;
				break;
			case EVO_MOVE_TYPE:
			    for (j = 0; j < MAX_MON_MOVES; j++)
				{
					if (gBattleMoves[GetMonData(mon, MON_DATA_MOVE1 + j, NULL)].type == gEvolutionTable[species][i].param)
					{
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
						break;
					}
				}
				break;
			case EVO_MAPSEC:
			    if (GetCurrentRegionMapSectionId() == gEvolutionTable[species][i].param)
					targetSpecies = gEvolutionTable[species][i].targetSpecies;
				break;
			case EVO_LEVEL_RAIN_OR_FOG:
			    weather = GetCurrentWeather();
			
			    if ((weather == WEATHER_RAIN || weather == WEATHER_RAIN_THUNDERSTORM || weather == WEATHER_DOWNPOUR || weather == WEATHER_FOG_HORIZONTAL
				|| weather == WEATHER_FOG_DIAGONAL) && gEvolutionTable[species][i].param <= level)
				    targetSpecies = gEvolutionTable[species][i].targetSpecies;
				break;
			case EVO_SPECIFIC_MON_IN_PARTY:
			    for (j = 0; j < PARTY_SIZE; j++)
				{
					if (GetMonData(&gPlayerParty[j], MON_DATA_SPECIES, NULL) == gEvolutionTable[species][i].param)
					{
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
						break;
					}
				}
				break;
			case EVO_LEVEL_DARK_TYPE_MON_IN_PARTY:
			    if (gEvolutionTable[species][i].param <= level)
				{
					for (j = 0; j < PARTY_SIZE; j++)
					{
						data = GetMonData(&gPlayerParty[j], MON_DATA_SPECIES, NULL);
						
						if (gBaseStats[data].type1 == TYPE_DARK || gBaseStats[data].type2 == TYPE_DARK)
						{
							targetSpecies = gEvolutionTable[species][i].targetSpecies;
							break;
						}
					}
				}
				break;
			case EVO_LEVEL_NATURE:
			    data = FORM_CHANGE_NATURE;
				goto EVOLUTIONS_FORM_CHANGE;
			case EVO_LEVEL_PERSONALITY:
			    data = FORM_CHANGE_PERSONALITY;
				
				EVOLUTIONS_FORM_CHANGE:
				if (gEvolutionTable[species][i].param <= level)
					targetSpecies = GetMonFormChangeSpecies(mon, gEvolutionTable[species][i].targetSpecies, data);
				break;
            }
        }
        break;
    case EVO_MODE_TRADE:
        for (i = 0; i < EVOS_PER_MON; i++)
        {
            switch (gEvolutionTable[species][i].method)
            {
			case EVO_TRADE_SPECIFIC_MON:
			    if (gEvolutionTable[species][i].param == partnerSpecies && partnerHoldEffect != HOLD_EFFECT_PREVENT_EVOLVE)
					targetSpecies = gEvolutionTable[species][i].targetSpecies;
				break;
            }
        }
        break;
    case EVO_MODE_ITEM_USE:
    case EVO_MODE_ITEM_CHECK:
        for (i = 0; i < EVOS_PER_MON; i++)
        {
			switch (gEvolutionTable[species][i].method)
			{
				case EVO_ITEM:
				    if (gEvolutionTable[species][i].param == evolutionItem)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;
				case EVO_ITEM_MALE:
				    if (gEvolutionTable[species][i].param == evolutionItem && GetMonGender(mon) == MON_MALE)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;
				case EVO_ITEM_FEMALE:
				    if (gEvolutionTable[species][i].param == evolutionItem && GetMonGender(mon) == MON_FEMALE)
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;
			}
        }
        break;
	case EVO_MODE_BATTLE_SPECIAL: // Battle evolution without leveling
	    for (i = 0; i < EVOS_PER_MON; i++)
		{
			switch (gEvolutionTable[species][i].method)
			{
				case EVO_CRITICAL_HITS:
				    if (gPartyCriticalHits[evolutionItem] >= gEvolutionTable[species][i].param) // party slot is being passed into the evolutionItem arg
					    targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;
			}
		}
		break;
	case EVO_MODE_OVERWORLD_SPECIAL: // Overworld evolution without leveling
	    for (i = 0; i < EVOS_PER_MON; i++)
		{
			switch (gEvolutionTable[species][i].method) // evolution method is being passed into the evolutionItem arg
			{
				case EVO_SCRIPT_TRIGGER_DMG:
				    data = GetMonData(mon, MON_DATA_HP, NULL);
					
					if (evolutionItem == gEvolutionTable[species][i].method && data && (GetMonData(mon, MON_DATA_MAX_HP, NULL) - data >= gEvolutionTable[species][i].param))
						targetSpecies = gEvolutionTable[species][i].targetSpecies;
					break;
			}
		}
		break;
    }
    return targetSpecies;
}

void DrawSpindaSpots(u16 species, u32 personality, u8 *dest, bool8 isFrontPic)
{
	u32 i, j, k;
	
    if (species == SPECIES_SPINDA && isFrontPic)
	{
		for (i = 0; i < 4; i++, personality >>= 8)
		{
			u8 x = sSpindaSpotGraphics[i].x + ((personality & 0x0F) - 8);
			u8 y = sSpindaSpotGraphics[i].y + (((personality & 0xF0) >> 4) - 8);
			
			for (j = 0; j < 16; j++, y++)
			{
				s32 row = sSpindaSpotGraphics[i].image[j];
				
				for (k = x; k < x + 16; k++, row >>= 1)
				{
					u8 *val = dest + ((k / 8) * 32) + ((k % 8) / 2) + ((y >> 3) << 8) + ((y & 7) << 2);
					
					if (row & 1)
					{
						if (k & 1)
						{
							if ((u8)((*val & 0xF0) - 0x10) <= 0x20)
								*val += 0x40;
						}
						else
						{
							if ((u8)((*val & 0xF) - 0x01) <= 0x02)
								*val += 0x04;
						}
					}
				}
			}
		}
	}
}

void EvolutionRenameMon(struct Pokemon *mon, u16 oldSpecies, u16 newSpecies)
{
    u8 language;
	
    GetMonData(mon, MON_DATA_NICKNAME, gStringVar1);
    language = GetMonData(mon, MON_DATA_LANGUAGE, &language);
	
    if (language == GAME_LANGUAGE && !StringCompare(gSpeciesNames[oldSpecies], gStringVar1))
        SetMonData(mon, MON_DATA_NICKNAME, gSpeciesNames[newSpecies]);
}

u8 GetPlayerFlankId(void)
{
    bool8 retVal = 0;
	
    switch (gLinkPlayers[GetMultiplayerId()].id)
    {
    case 0:
    case 3:
        retVal = 0;
        break;
    case 1:
    case 2:
        retVal = 1;
        break;
    }
    return retVal;
}

u8 GetLinkTrainerFlankId(u8 linkPlayerId)
{
    u8 retVal = 0;
	
    switch (gLinkPlayers[linkPlayerId].id)
    {
    case 0:
    case 3:
        retVal = 0;
        break;
    case 1:
    case 2:
        retVal = 1;
        break;
    }
    return retVal;
}

s32 GetBattlerMultiplayerId(u16 a1)
{
    s32 id;
	
    for (id = 0; id < MAX_LINK_PLAYERS; id++)
	{
        if (gLinkPlayers[id].id == a1)
            break;
	}
    return id;
}

bool8 ModifyMonFriendship(struct Pokemon *mon, s8 friendshipDelta)
{
	u16 heldItem = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
	s16 friendship = GetMonData(mon, MON_DATA_FRIENDSHIP, NULL);
	u8 holdEffect;
	
	if ((friendshipDelta > 0 && friendship < 255) || (friendshipDelta < 0 && friendship > 0))
	{
		holdEffect = ItemId_GetHoldEffect(heldItem);
		
		if (friendshipDelta > 0 && holdEffect == HOLD_EFFECT_HAPPINESS_UP)
			friendship += 150 * friendshipDelta / 100;
		else
			friendship += friendshipDelta;
	
		if (friendshipDelta > 0)
		{
			if (GetMonData(mon, MON_DATA_POKEBALL, NULL) == ITEM_LUXURY_BALL)
				friendship++;
			if (GetMonData(mon, MON_DATA_MET_LOCATION, NULL) == GetCurrentRegionMapSectionId())
				friendship++;
		}
		if (friendship < 0)
			friendship = 0;
		else if (friendship > 255)
			friendship = 255;
		SetMonData(mon, MON_DATA_FRIENDSHIP, &friendship);
		
		return TRUE;
	}
	return FALSE;
}

void AdjustFriendship(struct Pokemon *mon, u8 event)
{
	u8 friendshipLevel, friendship;

    if (IsMonValidSpecies(mon))
    {
		switch (event)
		{
			case FRIENDSHIP_EVENT_WALKING:
				if (RandomPercent(50)) // 50% chance every 128 steps
					return;
				break;
			case FRIENDSHIP_EVENT_LEAGUE_BATTLE:
				// Only if it's a trainer battle with league progression significance
				if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER))
					return;
				else if (!(gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_LEADER || gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_ELITE_FOUR
					|| gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_CHAMPION))
					return;
				break;
		}
		friendship = GetMonData(mon, MON_DATA_FRIENDSHIP, NULL);
		
        // Friendship level refers to the column in sFriendshipEventDeltas.
        // 0-99: Level 0 (maximum increase, typically)
        // 100-199: Level 1
        // 200-255: Level 2
		
		if (friendship >= 200)
            friendshipLevel = 2;
        else if (friendship >= 100)
            friendshipLevel = 1;
		else
			friendshipLevel = 0;

		ModifyMonFriendship(mon, sFriendshipEventDeltas[event][friendshipLevel]);
    }
}

void MonGainEVs(struct Pokemon *mon, u16 defeatedSpecies)
{
    u8 i, holdEffect, evs[NUM_STATS];
    u16 evIncrease = 0, totalEVs = 0;

    for (i = 0; i < NUM_STATS; i++)
    {
        evs[i] = GetMonData(mon, MON_DATA_HP_EV + i, NULL);
        totalEVs += evs[i];
    }

    for (i = 0; i < NUM_STATS; i++)
    {
        u8 multiplier;

        if (totalEVs >= MAX_TOTAL_EVS)
            break;

        if (CheckPartyHasHadPokerus(mon, 0))
            multiplier = 2;
        else
            multiplier = 1;

        switch (i)
        {
        case 0:
            evIncrease = gBaseStats[defeatedSpecies].evYield_HP * multiplier;
            break;
        case 1:
            evIncrease = gBaseStats[defeatedSpecies].evYield_Attack * multiplier;
            break;
        case 2:
            evIncrease = gBaseStats[defeatedSpecies].evYield_Defense * multiplier;
            break;
        case 3:
            evIncrease = gBaseStats[defeatedSpecies].evYield_Speed * multiplier;
            break;
        case 4:
            evIncrease = gBaseStats[defeatedSpecies].evYield_SpAttack * multiplier;
            break;
        case 5:
            evIncrease = gBaseStats[defeatedSpecies].evYield_SpDefense * multiplier;
            break;
        }
		holdEffect = ItemId_GetHoldEffect(GetMonData(mon, MON_DATA_HELD_ITEM, NULL));

        if (holdEffect == HOLD_EFFECT_MACHO_BRACE)
            evIncrease *= 2;

        if (totalEVs + (s16)evIncrease > MAX_TOTAL_EVS)
            evIncrease = ((s16)evIncrease + MAX_TOTAL_EVS) - (totalEVs + evIncrease);

        if (evs[i] + (s16)evIncrease > MAX_PER_STAT_EVS)
        {
            int val1 = (s16)evIncrease + MAX_PER_STAT_EVS;
            int val2 = evs[i] + evIncrease;
            evIncrease = val1 - val2;
        }
        evs[i] += evIncrease;
        totalEVs += evIncrease;
        SetMonData(mon, MON_DATA_HP_EV + i, &evs[i]);
    }
}

u16 GetMonEVCount(struct Pokemon *mon)
{
    u8 i;
    u16 count = 0;

    for (i = 0; i < NUM_STATS; i++)
        count += GetMonData(mon, MON_DATA_HP_EV + i, NULL);

    return count;
}

void RandomlyGivePartyPokerus(struct Pokemon *party)
{
	struct Pokemon *mon;
	u8 rnd2;
	u16 rnd = Random();
	
	if (rnd == 0x4000 || rnd == 0x8000 || rnd == 0xC000)
	{
		do
		{
			do
			{
				rnd = RandomMax(PARTY_SIZE);
				mon = &party[rnd];
			}
			while (!GetMonData(mon, MON_DATA_SPECIES, NULL));
		}
		while (GetMonData(mon, MON_DATA_IS_EGG, NULL));
		
		if (!CheckPartyHasHadPokerus(party, gBitTable[rnd]))
		{
			do
			{
				rnd2 = Random();
			}
			while (!(rnd2 & 0x7));
			
			if (rnd2 & 0xF0)
				rnd2 &= 0x7;
			
			rnd2 |= (rnd2 << 4);
			rnd2 &= 0xF3;
			rnd2++;
			
			SetMonData(mon, MON_DATA_POKERUS, &rnd2);
		}
	}
}

u8 CheckPartyPokerus(struct Pokemon *party, u8 party_bm)
{
    u8 retVal = 0, partyIndex = 0;
    unsigned curBit = 1;

    if (party_bm != 0) // Check mons in party based on bitmask, LSB = first mon
    {
        do
        {
            if ((party_bm & 1) && (GetMonData(&party[partyIndex], MON_DATA_POKERUS, NULL) & 0xF))
                retVal |= curBit;
			
            partyIndex++;
            curBit <<= 1;
            party_bm >>= 1;
        }
        while (party_bm);
    }
    else // Single Pokemon
    {
        if (GetMonData(&party[0], MON_DATA_POKERUS, NULL) & 0xF)
            retVal = 1;
    }
    return retVal;
}

u8 CheckPartyHasHadPokerus(struct Pokemon *party, u8 selection)
{
    u8 retVal = 0, partyIndex = 0;
    unsigned curBit = 1;

    if (selection)
    {
        do
        {
            if ((selection & 1) && GetMonData(&party[partyIndex], MON_DATA_POKERUS, NULL))
                retVal |= curBit;
			
            partyIndex++;
            curBit <<= 1;
            selection >>= 1;
        }
        while (selection);
    }
    else if (GetMonData(&party[0], MON_DATA_POKERUS, NULL))
        retVal = 1;

    return retVal;
}

void UpdatePartyPokerusTime(u32 daysSince)
{
	u8 i, pokerus;
	
	for (i = 0; i < PARTY_SIZE; i++)
	{
		if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL))
		{
			pokerus = GetMonData(&gPlayerParty[i], MON_DATA_POKERUS, NULL);
			
			if (pokerus & 0x3)
			{
				if ((pokerus & 0xF) < daysSince || daysSince > 4)
					pokerus &= 0x4;
				else
					pokerus -= daysSince;
				
				if (!pokerus)
					pokerus = 0x4;
				
				SetMonData(&gPlayerParty[i], MON_DATA_POKERUS, &pokerus);
			}
		}
	}
}

void PartySpreadPokerus(struct Pokemon *party)
{
	u8 i, pokerus;
	
	if (!RandomMax(3))
	{
		for (i = 0; i < PARTY_SIZE; i++)
		{
			if (GetMonData(&party[i], MON_DATA_SPECIES, NULL))
			{
				pokerus = GetMonData(&party[i], MON_DATA_POKERUS, NULL);
				
				if (pokerus && (pokerus & 0xF))
				{
					// Spread to adjacent party members.
					if (i != 0 && !(GetMonData(&party[i - 1], MON_DATA_POKERUS, NULL) & 0xF0))
						SetMonData(&party[i - 1], MON_DATA_POKERUS, &pokerus);
					
					if (i != (PARTY_SIZE - 1) && !(GetMonData(&party[i + 1], MON_DATA_POKERUS, NULL) & 0xF0))
					{
						SetMonData(&party[i + 1], MON_DATA_POKERUS, &pokerus);
						++i;
					}
				}
			}
		}
	}
}

static void SetMonExpWithMaxLevelCheck(struct Pokemon *mon, int species, u32 data)
{
    if (data > gExperienceTables[gBaseStats[species].growthRate][MAX_LEVEL])
    {
        data = gExperienceTables[gBaseStats[species].growthRate][MAX_LEVEL];
        SetMonData(mon, MON_DATA_EXP, &data);
    }
}

bool8 TryIncrementMonLevel(struct Pokemon *mon)
{
    u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    u8 level = GetMonData(mon, MON_DATA_LEVEL, NULL);
    u8 newLevel = level + 1;
    u32 exp = GetMonData(mon, MON_DATA_EXP, NULL);

    if (level < MAX_LEVEL)
    {
        if (exp > gExperienceTables[gBaseStats[species].growthRate][newLevel])
        {
            SetMonData(mon, MON_DATA_LEVEL, &newLevel);
            SetMonExpWithMaxLevelCheck(mon, species, exp);
            return TRUE;
        }
        else
            return FALSE;
    }
    else
    {
        SetMonExpWithMaxLevelCheck(mon, species, exp);
        return FALSE;
    }
}

bool8 CanMonLearnTMHM(struct Pokemon *mon, u8 tm)
{
    u16 species = GetMonData(mon, MON_DATA_SPECIES2, NULL);
	const u8 *learnableMoves;
	
	if (species != SPECIES_EGG)
	{
		learnableMoves = gTMHMLearnsets[species];
		
		while (*learnableMoves != 0xFF)
		{
			if (*learnableMoves == tm)
				return TRUE;
			
			learnableMoves++;
		}
	}
	return FALSE;
}

u8 GetMoveRelearnerMoves(struct Pokemon *mon, u16 *moves)
{
    u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    u8 numMoves, movePos, level = GetMonData(mon, MON_DATA_LEVEL, NULL);
    u32 i, j;

    for (i = 0, numMoves = 0; i < MAX_LV_UP_MOVES && gLevelUpLearnsets[species][i].move != LEVEL_UP_END; i++)
    {
        if (gLevelUpLearnsets[species][i].level <= level)
        {
			movePos = FindMoveSlotInMoveset(mon, gLevelUpLearnsets[species][i].move);

            if (movePos == MAX_MON_MOVES)
            {
                for (j = 0; j < numMoves && moves[j] != gLevelUpLearnsets[species][i].move; j++);

                if (j == numMoves)
                    moves[numMoves++] = gLevelUpLearnsets[species][i].move;
            }
        }
    }
    return numMoves;
}

u8 GetLevelUpMovesBySpecies(u16 species, u16 *moves)
{
    u8 i, numMoves;

    for (i = 0, numMoves = 0; i < MAX_LV_UP_MOVES && gLevelUpLearnsets[species][i].move != LEVEL_UP_END; i++)
         moves[numMoves++] = gLevelUpLearnsets[species][i].move;

     return numMoves;
}

u8 GetNumberOfRelearnableMoves(struct Pokemon *mon)
{
    u16 moves[MAX_LV_UP_MOVES], species = GetMonData(mon, MON_DATA_SPECIES2, NULL);
    u8 numMoves, movePos, level = GetMonData(mon, MON_DATA_LEVEL, NULL);
    u32 i, j;

    if (species == SPECIES_EGG)
        return 0;

    for (i = 0, numMoves = 0; i < MAX_LV_UP_MOVES && gLevelUpLearnsets[species][i].move != LEVEL_UP_END; i++)
    {
        if (gLevelUpLearnsets[species][i].level <= level)
        {
			movePos = FindMoveSlotInMoveset(mon, gLevelUpLearnsets[species][i].move);
			
            if (movePos == MAX_MON_MOVES)
            {
                for (j = 0; j < numMoves && moves[j] != gLevelUpLearnsets[species][i].move; j++);

                if (j == numMoves)
                    moves[numMoves++] = gLevelUpLearnsets[species][i].move;
            }
        }
    }
    return numMoves;
}

static u16 GetBattleBGM(void)
{
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        return MUS_RS_VS_TRAINER;
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
    {
        switch (gTrainers[gTrainerBattleOpponent_A].trainerClass)
        {
		case TRAINER_CLASS_CHAMPION:
			return MUS_VS_CHAMPION;
		case TRAINER_CLASS_LEADER:
		case TRAINER_CLASS_ELITE_FOUR:
			return MUS_VS_GYM_LEADER;
		case TRAINER_CLASS_BOSS:
		case TRAINER_CLASS_TEAM_ROCKET:
		case TRAINER_CLASS_COOLTRAINER:
		case TRAINER_CLASS_GENTLEMAN:
		case TRAINER_CLASS_RIVAL_LATE:
		default:
			return MUS_VS_TRAINER;
        }
    }
	else
		return MUS_VS_WILD;
}

void PlayBattleBGM(void)
{
    ResetMapMusic();
    m4aMPlayAllStop();
    PlayBGM(GetBattleBGM());
}

void PlayMapChosenOrBattleBGM(u16 songId)
{
    ResetMapMusic();
    m4aMPlayAllStop();
	PlayNewMapMusic(songId == 0 ? GetBattleBGM() : songId);
}

const u32 *GetMonFrontSpritePal(struct Pokemon *mon)
{
    return GetMonSpritePalFromSpecies(GetMonData(mon, MON_DATA_SPECIES2, NULL), GetMonData(mon, MON_DATA_IS_SHINY, NULL));
}

const u32 *GetMonSpritePalFromSpecies(u16 species, bool8 isShiny)
{
    if (species > NUM_SPECIES)
        return gMonPaletteTable[SPECIES_NONE].data;
    else if (isShiny)
        return gMonShinyPaletteTable[species].data;
    else
        return gMonPaletteTable[species].data;
}

const struct CompressedSpritePalette *GetMonSpritePalStruct(struct Pokemon *mon)
{
    return GetMonSpritePalStructFromSpecies(GetMonData(mon, MON_DATA_SPECIES2, NULL), GetMonData(mon, MON_DATA_IS_SHINY, NULL));
}

const struct CompressedSpritePalette *GetMonSpritePalStructFromSpecies(u16 species, bool8 isShiny)
{
    if (isShiny)
        return &gMonShinyPaletteTable[species];
    else
        return &gMonPaletteTable[species];
}

s8 GetMonFlavorRelation(struct Pokemon *mon, u8 flavor)
{
    return sPokeblockFlavorCompatibilityTable[GetMonData(mon, MON_DATA_NATURE, NULL) * 5 + flavor];
}

bool8 IsTradedMon(struct Pokemon *mon)
{
    u8 otName[PLAYER_NAME_LENGTH];
    GetMonData(mon, MON_DATA_OT_NAME, otName);
    return IsOtherTrainer(GetMonData(mon, MON_DATA_OT_ID, NULL), otName);
}

bool8 IsOtherTrainer(u32 otId, u8 *otName)
{
	u8 i;
	
    if (otId == GetPlayerTrainerId())
    {
        for (i = 0; otName[i] != EOS; i++)
		{
            if (otName[i] != gSaveBlock2Ptr->playerName[i])
                return TRUE;
		}
        return FALSE;
    }
    return TRUE;
}

void MonRestorePP(struct Pokemon *mon)
{
    BoxMonRestorePP(&mon->box);
}

void BoxMonRestorePP(struct BoxPokemon *boxMon)
{
    u8 i, ppBonus = GetBoxMonData(boxMon, MON_DATA_PP_BONUSES, NULL);
	
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (GetBoxMonData(boxMon, MON_DATA_MOVE1 + i, NULL))
        {
            u8 pp = CalculatePPWithBonus(GetBoxMonData(boxMon, MON_DATA_MOVE1 + i, NULL), ppBonus, i);
            SetBoxMonData(boxMon, MON_DATA_PP1 + i, &pp);
        }
    }
}

void SetMonPreventsSwitchingString(void)
{
	u8 battlerId = gBattleStruct->battlerPreventingSwitchout;
	
	PrepareMonNickWithPrefixBuffer(gBattleTextBuff1, battlerId,
	GetBattlerSide(battlerId) == B_SIDE_PLAYER ? GetPartyIdFromBattlePartyId(gBattlerPartyIndexes[battlerId]) : gBattlerPartyIndexes[battlerId]);
	
    PrepareAbilityBuffer(gBattleTextBuff2, GetBattlerAbility(battlerId));

    BattleStringExpandPlaceholdersToDisplayedString(COMPOUND_STRING("{B_BUFF1}'s {B_BUFF2}\nprevents switching!\p"));
}

void SetWildMonHeldItem(struct Pokemon *mon)
{
    if (!(gBattleTypeFlags & (BATTLE_TYPE_POKEDUDE | BATTLE_TYPE_TRAINER)) && !gDexnavBattle)
    {
        u16 rnd = RandomMax(100);
        u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
		
        if (gBaseStats[species].item1 == gBaseStats[species].item2)
            SetMonData(mon, MON_DATA_HELD_ITEM, &gBaseStats[species].item1);
        else if (rnd > 44)
			SetMonData(mon, MON_DATA_HELD_ITEM, rnd <= 94 ? &gBaseStats[species].item1 : &gBaseStats[species].item2);
    }
}

u8 *GetTrainerPartnerName(void)
{
    return gLinkPlayers[GetBattlerMultiplayerId(gLinkPlayers[GetMultiplayerId()].id ^ 2)].name;
}

u8 GetPlayerPartyHighestLevel(void)
{
    u8 slot, monLevel, level = 1;

    for (slot = 0; slot < PARTY_SIZE; ++slot)
    {
        if (IsMonValidSpecies(&gPlayerParty[slot]))
        {
            monLevel = GetMonData(&gPlayerParty[slot], MON_DATA_LEVEL, NULL);
            if (monLevel > level)
                level = monLevel;
        }
    }
    return level;
}

static u32 GetLinkPlayerArrId(void)
{
	u8 linkId = GetMultiplayerId() ^ 1;
	return (gLinkPlayers[linkId].trainerId & 7) | (gLinkPlayers[linkId].gender << 3);
}

u16 GetUnionRoomTrainerPic(void)
{
    return FacilityClassToPicIndex(sLinkPlayerFacilityClasses[GetLinkPlayerArrId()]);
}

u16 GetUnionRoomTrainerClass(void)
{
    return gFacilityClassToTrainerClass[sLinkPlayerFacilityClasses[GetLinkPlayerArrId()]];
}

void ClearAllFusedMonSpecies(void)
{
	u8 i;
	
	for (i = 0; i < FUSIONS_COUNT; i++)
		memset(&gSaveBlock1Ptr->fusions[i], 0, sizeof(struct Pokemon));
}

const u8* GetItemEffect(u16 item)
{
	// Check if the item is one that can be used on a Pokemon
	if (item >= ITEM_POTION && item <= BERRY_ITEMS_END)
		return gItemEffectTable[item - ITEM_POTION];
	
	return NULL;
}

u16 ItemIdToBattleMoveId(u16 item)
{
    return sTMHMMoves[item - ITEM_TM01];
}

bool8 IsMonValidSpecies(struct Pokemon *mon)
{
	return (GetMonData(mon, MON_DATA_SPECIES2, NULL) && GetMonData(mon, MON_DATA_SPECIES2, NULL) != SPECIES_EGG);
}

bool8 MonCanBattle(struct Pokemon *mon)
{
	if (IsMonValidSpecies(mon) && GetMonData(mon, MON_DATA_HP, NULL))
		return TRUE;
	return FALSE;
}

u8 FindMoveSlotInMoveset(struct Pokemon *mon, u16 move)
{
	u8 i;
	
	for (i = 0; i < MAX_MON_MOVES; i++)
	{
		if (GetMonData(mon, MON_DATA_MOVE1 + i, NULL) == move)
			break;
	}
	return i;
}

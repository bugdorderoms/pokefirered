#include "global.h"
#include "battle_controllers.h"
#include "dns.h"
#include "form_change.h"
#include "item.h"
#include "pokemon.h"
#include "constants/hold_effects.h"
#include "constants/pokedex.h"
#include "constants/pokemon.h"

///////////////////////////
// OVERWORLD FORM CHANGE //
///////////////////////////

static bool8 EndBattleFormChange(struct Pokemon *mon);
static u16 GenderFormChange(struct Pokemon *mon, u16 *newSpecies);
static u16 PersonalityFormChange(struct Pokemon *mon, u16 *newSpecies);
static u16 SeasonFormChange(struct Pokemon *mon, u16 *newSpecies);
static u16 HoldItemFormChange(struct Pokemon *mon, u16 *newSpecies);
static u16 TerrainFormChange(struct Pokemon *mon, u16 *newSpecies);

// hold item forms tables
static const u16 sTypeToArceusForm[NUMBER_OF_MON_TYPES] =
{
	[TYPE_NORMAL] =		SPECIES_ARCEUS,
	[TYPE_FIGHTING] =	SPECIES_ARCEUS_FIGHTING,
	[TYPE_FLYING] = 	SPECIES_ARCEUS_FLYING,
	[TYPE_POISON] = 	SPECIES_ARCEUS_POISON,
	[TYPE_GROUND] = 	SPECIES_ARCEUS_GROUND,
	[TYPE_ROCK] =		SPECIES_ARCEUS_ROCK,
	[TYPE_BUG] =		SPECIES_ARCEUS_BUG,
	[TYPE_GHOST] =		SPECIES_ARCEUS_GHOST,
	[TYPE_STEEL] =		SPECIES_ARCEUS_STEEL,
	[TYPE_MYSTERY] =	SPECIES_NONE,
	[TYPE_FIRE] =		SPECIES_ARCEUS_FIRE,
	[TYPE_WATER] =		SPECIES_ARCEUS_WATER,
	[TYPE_GRASS] =		SPECIES_ARCEUS_GRASS,
	[TYPE_ELECTRIC] =	SPECIES_ARCEUS_ELECTRIC,
	[TYPE_PSYCHIC] =	SPECIES_ARCEUS_PSYCHIC,
	[TYPE_ICE] =		SPECIES_ARCEUS_ICE,
	[TYPE_DRAGON] = 	SPECIES_ARCEUS_DRAGON,
	[TYPE_DARK] =		SPECIES_ARCEUS_DARK,
	[TYPE_FAIRY] =		SPECIES_ARCEUS_FAIRY
};

static const u16 sTypeToSilvallyForm[NUMBER_OF_MON_TYPES] =
{
	[TYPE_NORMAL] = 	SPECIES_SILVALLY,
	[TYPE_FIGHTING] = 	SPECIES_SILVALLY_FIGHTING,
	[TYPE_FLYING] = 	SPECIES_SILVALLY_FLYING,
	[TYPE_POISON] = 	SPECIES_SILVALLY_POISON,
	[TYPE_GROUND] = 	SPECIES_SILVALLY_GROUND,
	[TYPE_ROCK] = 		SPECIES_SILVALLY_ROCK,
	[TYPE_BUG] = 		SPECIES_SILVALLY_BUG,
	[TYPE_GHOST] =		SPECIES_SILVALLY_GHOST,
	[TYPE_STEEL] =		SPECIES_SILVALLY_STEEL,
	[TYPE_MYSTERY] = 	SPECIES_NONE,
	[TYPE_FIRE] = 		SPECIES_SILVALLY_FIRE,
	[TYPE_WATER] =		SPECIES_SILVALLY_WATER,
	[TYPE_GRASS] =		SPECIES_SILVALLY_GRASS,
	[TYPE_ELECTRIC] = 	SPECIES_SILVALLY_ELECTRIC,
	[TYPE_PSYCHIC] = 	SPECIES_SILVALLY_PSYCHIC,
	[TYPE_ICE] = 		SPECIES_SILVALLY_ICE,
	[TYPE_DRAGON] = 	SPECIES_SILVALLY_DRAGON,
	[TYPE_DARK] =		SPECIES_SILVALLY_DARK,
	[TYPE_FAIRY] =		SPECIES_SILVALLY_FAIRY
};

// gender forms table
static const u16 sGenderBasedFormChangeTable[][2] =
{
	// form when male, form when female
	{SPECIES_UNFEZANT, SPECIES_UNFEZANT_FEMALE},
	{SPECIES_FRILLISH, SPECIES_FRILLISH_FEMALE},
	{SPECIES_JELLICENT, SPECIES_JELLICENT_FEMALE},
	{SPECIES_PYROAR, SPECIES_PYROAR_FEMALE},
	{SPECIES_MEOWSTIC, SPECIES_MEOWSTIC_FEMALE},
	{SPECIES_INDEEDEE, SPECIES_INDEEDEE_FEMALE},
	{SPECIES_BASCULEGION, SPECIES_BASCULEGION_FEMALE},
	{SPECIES_OINKOLOGNE, SPECIES_OINKOLOGNE_FEMALE},
};

// personality forms tables
static const u16 sPikachuFormsTable[] =
{
	SPECIES_PIKACHU, // normal Pikachu
	SPECIES_PIKACHU_COSPLAY,
	SPECIES_PIKACHU_ROCK_STAR,
	SPECIES_PIKACHU_BELLE,
	SPECIES_PIKACHU_POP_STAR,
	SPECIES_PIKACHU_PH_D,
	SPECIES_PIKACHU_LIBRE,
	SPECIES_PIKACHU_ORIGINAL_CAP,
	SPECIES_PIKACHU_HOENN_CAP,
	SPECIES_PIKACHU_SINNOH_CAP,
	SPECIES_PIKACHU_UNOVA_CAP,
	SPECIES_PIKACHU_KALOS_CAP,
	SPECIES_PIKACHU_ALOLA_CAP,
	SPECIES_PIKACHU_PARTNER_CAP,
	SPECIES_PIKACHU_WORLD_CAP,
};

static const u16 sUnownFormsTable[] =
{
	SPECIES_UNOWN, // Unown A
	SPECIES_UNOWN_B,
	SPECIES_UNOWN_C,
	SPECIES_UNOWN_D,
	SPECIES_UNOWN_E,
	SPECIES_UNOWN_F,
	SPECIES_UNOWN_G,
    SPECIES_UNOWN_H,
    SPECIES_UNOWN_I,
    SPECIES_UNOWN_J,
    SPECIES_UNOWN_K,
    SPECIES_UNOWN_L,
    SPECIES_UNOWN_M,
    SPECIES_UNOWN_N,
    SPECIES_UNOWN_O,
    SPECIES_UNOWN_P,
    SPECIES_UNOWN_Q,
    SPECIES_UNOWN_R,
    SPECIES_UNOWN_S,
    SPECIES_UNOWN_T,
    SPECIES_UNOWN_U,
    SPECIES_UNOWN_V,
    SPECIES_UNOWN_W,
    SPECIES_UNOWN_X,
    SPECIES_UNOWN_Y,
    SPECIES_UNOWN_Z,
    SPECIES_UNOWN_EMARK,
    SPECIES_UNOWN_QMARK,
};

static const u16 sShellosFormsTable[] =
{
	SPECIES_SHELLOS, // west sea
	SPECIES_SHELLOS_EAST_SEA,
};

static const u16 sGastrodonFormsTable[] =
{
	SPECIES_GASTRODON, // west sea
	SPECIES_GASTRODON_EAST_SEA,
};

static const u16 sBasculinFormsTable[] =
{
	SPECIES_BASCULIN, // red striped
	SPECIES_BASCULIN_BLUE_STRIPED,
	SPECIES_BASCULIN_WHITE_STRIPED,
};

static const u16 sVivillonFormsTable[] =
{
	SPECIES_VIVILLON, // icy snow
	SPECIES_VIVILLON_POLAR,
    SPECIES_VIVILLON_TUNDRA,
    SPECIES_VIVILLON_CONTINENTAL,
    SPECIES_VIVILLON_GARDEN,
    SPECIES_VIVILLON_ELEGANT,
    SPECIES_VIVILLON_MEADOW,
    SPECIES_VIVILLON_MODERN,
    SPECIES_VIVILLON_MARINE,
    SPECIES_VIVILLON_ARCHIPELAGO,
    SPECIES_VIVILLON_HIGH_PLAINS,
    SPECIES_VIVILLON_SANDSTORM,
    SPECIES_VIVILLON_RIVER,
    SPECIES_VIVILLON_MONSOON,
    SPECIES_VIVILLON_SAVANNA,
    SPECIES_VIVILLON_SUN,
    SPECIES_VIVILLON_OCEAN,
    SPECIES_VIVILLON_JUNGLE,
    SPECIES_VIVILLON_FANCY,
    SPECIES_VIVILLON_POKE_BALL,
};

static const u16 sFlabebeFormsTable[] =
{
	SPECIES_FLABEBE, // red flower
	SPECIES_FLABEBE_YELLOW_FLOWER,
    SPECIES_FLABEBE_ORANGE_FLOWER,
    SPECIES_FLABEBE_BLUE_FLOWER,
    SPECIES_FLABEBE_WHITE_FLOWER,
};

static const u16 sFloetteFormsTable[] =
{
	SPECIES_FLOETTE, // red flower
	SPECIES_FLOETTE_YELLOW_FLOWER,
    SPECIES_FLOETTE_ORANGE_FLOWER,
    SPECIES_FLOETTE_BLUE_FLOWER,
    SPECIES_FLOETTE_WHITE_FLOWER,
	/* SPECIES_FLOETTE_ETERNAL_FLOWER */ // excludes this form
};

static const u16 sFlorgesFormsTable[] =
{
	SPECIES_FLORGES, // red flower
	SPECIES_FLORGES_YELLOW_FLOWER,
    SPECIES_FLORGES_ORANGE_FLOWER,
    SPECIES_FLORGES_BLUE_FLOWER,
    SPECIES_FLORGES_WHITE_FLOWER,
};

static const u16 sFurfrouFormsTable[] =
{
	SPECIES_FURFROU, // natural form
	SPECIES_FURFROU_HEART_TRIM,
    SPECIES_FURFROU_STAR_TRIM,
    SPECIES_FURFROU_DIAMOND_TRIM,
    SPECIES_FURFROU_DEBUTANTE_TRIM,
    SPECIES_FURFROU_MATRON_TRIM,
    SPECIES_FURFROU_DANDY_TRIM,
    SPECIES_FURFROU_LA_REINE_TRIM,
    SPECIES_FURFROU_KABUKI_TRIM,
    SPECIES_FURFROU_PHARAOH_TRIM,
};

static const u16 sOricorioFormsTable[] =
{
	SPECIES_ORICORIO, // baile style
	SPECIES_ORICORIO_POM_POM,
    SPECIES_ORICORIO_PAU,
    SPECIES_ORICORIO_SENSU,
};

static const u16 sMiniorFormsTable[] =
{
	SPECIES_MINIOR, // red core
	SPECIES_MINIOR_CORE_ORANGE,
    SPECIES_MINIOR_CORE_YELLOW,
    SPECIES_MINIOR_CORE_GREEN,
    SPECIES_MINIOR_CORE_BLUE,
    SPECIES_MINIOR_CORE_INDIGO,
    SPECIES_MINIOR_CORE_VIOLET,
};

static const u16 sAlcremieFormsTable[] =
{
	SPECIES_ALCREMIE, // vanilla cream
	SPECIES_ALCREMIE_RUBY_CREAM,
    SPECIES_ALCREMIE_MATCHA_CREAM,
    SPECIES_ALCREMIE_MINT_CREAM,
    SPECIES_ALCREMIE_LEMON_CREAM,
    SPECIES_ALCREMIE_SALTED_CREAM,
    SPECIES_ALCREMIE_RUBY_SWIRL,
    SPECIES_ALCREMIE_CARAMEL_SWIRL,
    SPECIES_ALCREMIE_RAINBOW_SWIRL,
};

static const u16 sSquawkabillyFormsTable[] =
{
	SPECIES_SQUAWKABILLY, // green plumage
	SPECIES_SQUAWKABILLY_BLUE,
    SPECIES_SQUAWKABILLY_YELLOW,
    SPECIES_SQUAWKABILLY_WHITE,
};

static const u16 sTatsugiriFormsTable[] =
{
	SPECIES_TATSUGIRI, // curly
	SPECIES_TATSUGIRI_DROOPY,
    SPECIES_TATSUGIRI_STRETCHY,
};

// season forms tables
static const u16 sDeerlingFormsTable[] =
{
	[SEASON_SUMMER] = SPECIES_DEERLING_SUMMER,
	[SEASON_AUTUMN] = SPECIES_DEERLING_AUTUMN,
	[SEASON_WINTER] = SPECIES_DEERLING_WINTER,
	[SEASON_SPRING] = SPECIES_DEERLING,
};

static const u16 sSawsbuckFormsTable[] =
{
	[SEASON_SUMMER] = SPECIES_SAWSBUCK_SUMMER,
	[SEASON_AUTUMN] = SPECIES_SAWSBUCK_AUTUMN,
	[SEASON_WINTER] = SPECIES_SAWSBUCK_WINTER,
	[SEASON_SPRING] = SPECIES_SAWSBUCK,
};

// terrain forms table
static const u16 sTerrainToBurmyForm[] =
{
	[BATTLE_TERRAIN_GRASS] = SPECIES_BURMY,
	[BATTLE_TERRAIN_LONG_GRASS] = SPECIES_BURMY,
	[BATTLE_TERRAIN_SAND] = SPECIES_BURMY_SANDY_CLOAK,
	[BATTLE_TERRAIN_POND] = SPECIES_BURMY_SANDY_CLOAK,
	[BATTLE_TERRAIN_MOUNTAIN] = SPECIES_BURMY_SANDY_CLOAK,
	[BATTLE_TERRAIN_CAVE] = SPECIES_BURMY_SANDY_CLOAK,
	[BATTLE_TERRAIN_BUILDING] = SPECIES_BURMY_TRASH_CLOAK,
	[BATTLE_TERRAIN_PLAIN] = SPECIES_BURMY_SANDY_CLOAK,
	[BATTLE_TERRAIN_LINK] = SPECIES_BURMY_TRASH_CLOAK,
	[BATTLE_TERRAIN_GYM] = SPECIES_BURMY_TRASH_CLOAK,
	[BATTLE_TERRAIN_LEADER] = SPECIES_BURMY_TRASH_CLOAK,
	[BATTLE_TERRAIN_INDOOR_2] = SPECIES_BURMY_TRASH_CLOAK,
	[BATTLE_TERRAIN_INDOOR_1] = SPECIES_BURMY_TRASH_CLOAK,
	[BATTLE_TERRAIN_LORELEI] = SPECIES_BURMY_TRASH_CLOAK,
	[BATTLE_TERRAIN_BRUNO] = SPECIES_BURMY_TRASH_CLOAK,
	[BATTLE_TERRAIN_AGATHA] = SPECIES_BURMY_TRASH_CLOAK,
	[BATTLE_TERRAIN_LANCE] = SPECIES_BURMY_TRASH_CLOAK,
	[BATTLE_TERRAIN_CHAMPION] = SPECIES_BURMY_TRASH_CLOAK,
};

u16 DoOverworldFormChange(struct Pokemon *mon, u32 formChangeType)
{
	u16 newSpecies = SPECIES_NONE;
	
	if (formChangeType & OVERWORLD_FORM_CHANGE_END_BATTLE)
		return EndBattleFormChange(mon);
	
	if (formChangeType & OVERWORLD_FORM_CHANGE_GENDER)
		newSpecies = GenderFormChange(mon, &newSpecies);
	
	if (formChangeType & OVERWORLD_FORM_CHANGE_PERSONALITY)
		newSpecies = PersonalityFormChange(mon, &newSpecies);
	
	if (formChangeType & OVERWORLD_FORM_CHANGE_SEASON)
		newSpecies = SeasonFormChange(mon, &newSpecies);
	
	if (formChangeType & OVERWORLD_FORM_CHANGE_HOLD_ITEM)
		newSpecies = HoldItemFormChange(mon, &newSpecies);
	
	if (formChangeType & OVERWORLD_FORM_CHANGE_TERRAIN)
		newSpecies = TerrainFormChange(mon, &newSpecies);
	
	if (newSpecies && newSpecies != GetMonData(mon, MON_DATA_SPECIES))
	{
		SetMonData(mon, MON_DATA_SPECIES, &newSpecies);
		CalculateMonStats(mon);
	}
	return GetMonData(mon, MON_DATA_SPECIES);
}

static u16 PersonalityFormChange(struct Pokemon *mon, u16 *newSpecies)
{
	u32 personality = GetMonData(mon, MON_DATA_PERSONALITY);
	
	switch (SpeciesToNationalPokedexNum(GetMonData(mon, MON_DATA_SPECIES)))
	{
		case NATIONAL_DEX_PIKACHU:
		    *newSpecies = sPikachuFormsTable[personality % NELEMS(sPikachuFormsTable)];
			break;
		case NATIONAL_DEX_UNOWN:
		    *newSpecies = sUnownFormsTable[personality % NELEMS(sUnownFormsTable)];
			break;
		case NATIONAL_DEX_SHELLOS:
		    *newSpecies = sShellosFormsTable[personality % NELEMS(sShellosFormsTable)];
			break;
		case NATIONAL_DEX_GASTRODON:
		    *newSpecies = sGastrodonFormsTable[personality % NELEMS(sGastrodonFormsTable)];
			break;
		case NATIONAL_DEX_BASCULIN:
		    *newSpecies = sBasculinFormsTable[personality % NELEMS(sBasculinFormsTable)];
			break;
		case NATIONAL_DEX_VIVILLON:
		    *newSpecies = sVivillonFormsTable[personality % NELEMS(sVivillonFormsTable)];
			break;
		case NATIONAL_DEX_FLABEBE:
		    *newSpecies = sFlabebeFormsTable[personality % NELEMS(sFlabebeFormsTable)];
			break;
		case NATIONAL_DEX_FLOETTE:
		    *newSpecies = sFloetteFormsTable[personality % NELEMS(sFloetteFormsTable)];
			break;
		case NATIONAL_DEX_FLORGES:
		    *newSpecies = sFlorgesFormsTable[personality % NELEMS(sFlorgesFormsTable)];
			break;
		case NATIONAL_DEX_FURFROU:
		    *newSpecies = sFurfrouFormsTable[personality % NELEMS(sFurfrouFormsTable)];
			break;
		case NATIONAL_DEX_ORICORIO:
		    *newSpecies = sOricorioFormsTable[personality % NELEMS(sOricorioFormsTable)];
			break;
		case NATIONAL_DEX_MINIOR:
		    *newSpecies = sMiniorFormsTable[personality % NELEMS(sMiniorFormsTable)];
			break;
		case NATIONAL_DEX_ALCREMIE:
		    *newSpecies = sAlcremieFormsTable[personality % NELEMS(sAlcremieFormsTable)];
			break;
		case NATIONAL_DEX_SQUAWKABILLY:
		    *newSpecies = sSquawkabillyFormsTable[personality % NELEMS(sSquawkabillyFormsTable)];
			break;
		case NATIONAL_DEX_TATSUGIRI:
		    *newSpecies = sTatsugiriFormsTable[personality % NELEMS(sTatsugiriFormsTable)];
			break;
	}
	return *newSpecies;
}

static u16 SeasonFormChange(struct Pokemon *mon, u16 *newSpecies)
{
	u8 season = DNSGetCurrentSeason();
	
	switch (SpeciesToNationalPokedexNum(GetMonData(mon, MON_DATA_SPECIES)))
	{
		case NATIONAL_DEX_DEERLING:
		    *newSpecies = sDeerlingFormsTable[season];
			break;
		case NATIONAL_DEX_SAWSBUCK:
		    *newSpecies = sSawsbuckFormsTable[season];
			break;
	}
	return *newSpecies;
}

static u16 TerrainFormChange(struct Pokemon *mon, u16 *newSpecies)
{
	u16 BurmyForm = sTerrainToBurmyForm[gBattleTerrain];
	
	if (BurmyForm && SpeciesToNationalPokedexNum(GetMonData(mon, MON_DATA_SPECIES)) == NATIONAL_DEX_BURMY)
		*newSpecies = BurmyForm;
	
	return *newSpecies;
}

static u16 HoldItemFormChange(struct Pokemon *mon, u16 *newSpecies)
{
	u16 itemId = GetMonData(mon, MON_DATA_HELD_ITEM), ability = GetMonAbility(mon);
	u8 itemEffect = ItemId_GetHoldEffect(itemId), type = ItemId_GetHoldEffectParam(itemId);
	
	switch (SpeciesToNationalPokedexNum(GetMonData(mon, MON_DATA_SPECIES)))
	{
		case NATIONAL_DEX_GIRATINA:
		    if (itemEffect == HOLD_EFFECT_GRISEOUS_ORB)
				*newSpecies = SPECIES_GIRATINA_ORIGIN;
			else
				*newSpecies = SPECIES_GIRATINA;
			break;
		case NATIONAL_DEX_ARCEUS:
		    if (ability == ABILITY_MULTITYPE)
			{
				if (itemEffect == HOLD_EFFECT_PLATE)
					*newSpecies = sTypeToArceusForm[type];
			}
			break;
		case NATIONAL_DEX_GENESECT:
		    if (itemEffect == HOLD_EFFECT_DRIVE)
			{
				switch (type)
				{
					case TYPE_WATER:
						*newSpecies = SPECIES_GENESECT_DOUSE_DRIVE;
						break;
					case TYPE_FIRE:
						*newSpecies = SPECIES_GENESECT_BURN_DRIVE;
						break;
					case TYPE_ICE:
						*newSpecies = SPECIES_GENESECT_CHILL_DRIVE;
						break;
					case TYPE_ELECTRIC:
						*newSpecies = SPECIES_GENESECT_SHOCK_DRIVE;
						break;
				}
			}
			break;
		case NATIONAL_DEX_SILVALLY:
		    if (ability == ABILITY_RKS_SYSTEM && itemEffect == HOLD_EFFECT_MEMORY)
				*newSpecies = sTypeToSilvallyForm[type];
			break;
	}
	return *newSpecies;
}

static u16 GenderFormChange(struct Pokemon *mon, u16 *newSpecies)
{
	u8 i;
	u16 species = GetMonData(mon, MON_DATA_SPECIES);
	bool8 isFemale = (GetMonGender(mon) == MON_FEMALE);
	
	for (i = 0; i < NELEMS(sGenderBasedFormChangeTable); i++)
	{
		if (species == sGenderBasedFormChangeTable[i][0] || species == sGenderBasedFormChangeTable[i][1])
		{
			*newSpecies = sGenderBasedFormChangeTable[i][isFemale];
			break;
		}
	}
	return *newSpecies;
}

void DoPlayerPartyEndBattleFormChange(void)
{
	u8 i;
	
	for (i = 0; i < PARTY_SIZE; i++)
	{
		DoOverworldFormChange(&gPlayerParty[i], OVERWORLD_FORM_CHANGE_END_BATTLE); // revert battle forms back
		DoOverworldFormChange(&gPlayerParty[i], OVERWORLD_FORM_CHANGE_TERRAIN); // update Burmy form
	}
}

////////////////////////
// BATTLE FORM CHANGE //
////////////////////////

static bool8 IsBannedSpeciesToRevert(u16 species);

static const u16 sHpBasedFormChangeTable[][3] = 
{
	// form when hp >, form when hp <=, hp divisor
	{SPECIES_DARMANITAN, SPECIES_DARMANITAN_ZEN_MODE, 2},
	{SPECIES_DARMANITAN_GALARIAN, SPECIES_DARMANITAN_ZEN_MODE_GALARIAN, 2},
};

// species forms to revert from on battle ends
static const u16 sEndBattleBannedRevertSpecies[] =
{
	SPECIES_DARMANITAN_ZEN_MODE,
	SPECIES_DARMANITAN_ZEN_MODE_GALARIAN,
};

void DoBattleFormChange(u8 battlerId, u16 newSpecies, bool8 reloadTypes, bool8 reloadStats, bool8 reloadAbility)
{
	struct Pokemon *mon = GetBattlerPartyIndexPtr(battlerId);
	u16 species = GetMonData(mon, MON_DATA_SPECIES);
	
	gActiveBattler = battlerId;
	
	gBattleMons[battlerId].species = newSpecies;
	SetMonData(mon, MON_DATA_SPECIES, &newSpecies);
	
	if (reloadStats)
	{
		CalculateMonStats(mon);
		gBattleMons[battlerId].hp = GetMonData(mon, MON_DATA_HP);
		gBattleMons[battlerId].maxHP = GetMonData(mon, MON_DATA_MAX_HP);
		gBattleMons[battlerId].attack = GetMonData(mon, MON_DATA_ATK);
		gBattleMons[battlerId].defense = GetMonData(mon, MON_DATA_DEF);
		gBattleMons[battlerId].speed = GetMonData(mon, MON_DATA_SPEED);
		gBattleMons[battlerId].spAttack = GetMonData(mon, MON_DATA_SPATK);
		gBattleMons[battlerId].spDefense = GetMonData(mon, MON_DATA_SPDEF);
		HANDLE_POWER_TRICK_SWAP(battlerId);
	}
	if (reloadAbility)
		gBattleMons[battlerId].ability = GetMonAbility(mon);
	
	BtlController_EmitSetMonData(0, REQUEST_FORM_CHANGE_BATTLE, 0, sizeof(struct BattlePokemon), &gBattleMons[battlerId]);
	MarkBattlerForControllerExec(gActiveBattler);
	
	if (reloadTypes)
	{
		gBattleMons[battlerId].type1 = gBaseStats[newSpecies].type1;
		gBattleMons[battlerId].type2 = gBaseStats[newSpecies].type2;
		gBattleMons[battlerId].type3 = TYPE_MYSTERY;
	}
	ClearIllusionMon(battlerId);
	SetMonData(mon, MON_DATA_SPECIES, &species);
}

u16 TryDoBattleFormChange(u8 battlerId, u8 formChangeType)
{
	u16 newSpecies = SPECIES_NONE;
	struct Pokemon *mon;
	
	switch (formChangeType)
	{
		case BATTLE_FORM_CHANGE_SWITCH_OUT:
		    switch (gBattleMons[battlerId].species)
			{
				case SPECIES_DARMANITAN_ZEN_MODE:
				    newSpecies = SPECIES_DARMANITAN;
					break;
				case SPECIES_DARMANITAN_ZEN_MODE_GALARIAN:
				    newSpecies = SPECIES_DARMANITAN_GALARIAN;
					break;
			}
			
			if (newSpecies)
				DoBattleFormChange(battlerId, newSpecies, FALSE, TRUE, FALSE);
			break;
		case BATTLE_FORM_CHANGE_LOW_HP:
		    if (!(gBattleMons[battlerId].status2 & STATUS2_TRANSFORMED))
			{
				u8 i;
				u16 species = gBattleMons[battlerId].species;
				
				for (i = 0; i < NELEMS(sHpBasedFormChangeTable); i++)
				{
					if (gBattleMons[battlerId].hp <= gBattleMons[battlerId].maxHP / sHpBasedFormChangeTable[i][2])
					{
						if (species == sHpBasedFormChangeTable[i][0])
						{
							newSpecies = sHpBasedFormChangeTable[i][1];
							break;
						}
					}
					else
					{
						if (species == sHpBasedFormChangeTable[i][1])
						{
							newSpecies = sHpBasedFormChangeTable[i][0];
							break;
						}
					}
				}
			}
			break;
		case BATTLE_FORM_CHANGE_FAINT:
		    mon = GetBattlerPartyIndexPtr(battlerId);
		
		    if (DoOverworldFormChange(mon, OVERWORLD_FORM_CHANGE_END_BATTLE))
			{
				SetMonData(mon, MON_DATA_SPECIES_BACKUP, &newSpecies);
				newSpecies = GetMonData(mon, MON_DATA_SPECIES);
				gActiveBattler = battlerId;
				BtlController_EmitSetMonData(0, REQUEST_SPECIES_BATTLE, 0, 2, &newSpecies);
				MarkBattlerForControllerExec(gActiveBattler);
				CalculateMonStats(mon);
				BtlController_EmitSetRawMonData(0, offsetof(struct Pokemon, attack), 10, &mon->attack); // reload all stats
				MarkBattlerForControllerExec(gActiveBattler);
			}
			break;
	}
	return newSpecies;
}

static bool8 EndBattleFormChange(struct Pokemon *mon)
{
	u16 none = SPECIES_NONE, speciesBackup = GetMonData(mon, MON_DATA_SPECIES_BACKUP);
	
	if (IsBannedSpeciesToRevert(speciesBackup))
		SetMonData(mon, MON_DATA_SPECIES_BACKUP, &none);
	else if (speciesBackup && speciesBackup < NUM_SPECIES)
	{
		SetMonData(mon, MON_DATA_SPECIES, &speciesBackup);
		SetMonData(mon, MON_DATA_SPECIES_BACKUP, &none);
		CalculateMonStats(mon);
		return TRUE;
	}
	return FALSE;
}

static bool8 IsBannedSpeciesToRevert(u16 species)
{
	u8 i;
	
	for (i = 0; i < NELEMS(sEndBattleBannedRevertSpecies); i++)
	{
		if (species == sEndBattleBannedRevertSpecies[i])
			return TRUE;
	}
	return FALSE;
}

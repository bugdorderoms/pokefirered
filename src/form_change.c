#include "global.h"
#include "form_change.h"
#include "item.h"
#include "pokemon.h"
#include "constants/hold_effects.h"
#include "constants/pokedex.h"
#include "constants/pokemon.h"

static const u16 sTypeToArceusForm[NUMBER_OF_MON_TYPES] =
{
	[TYPE_NORMAL] =		SPECIES_NONE,
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
	[TYPE_NORMAL] = 	SPECIES_NONE,
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

void HoldItemFormChange(struct Pokemon *mon, u16 itemId)
{
	u16 targetSpecies = SPECIES_NONE, species = GetMonData(mon, MON_DATA_SPECIES), ability = GetMonAbility(mon);
	u8 itemEffect = ItemId_GetHoldEffect(itemId), type = ItemId_GetHoldEffectParam(itemId);
	
	switch (SpeciesToNationalPokedexNum(species))
	{
		case NATIONAL_DEX_GIRATINA:
		    if (itemEffect == HOLD_EFFECT_GRISEOUS_ORB)
				targetSpecies = SPECIES_GIRATINA_ORIGIN;
			else
				targetSpecies = SPECIES_GIRATINA;
			break;
		case NATIONAL_DEX_ARCEUS:
		    if (ability == ABILITY_MULTITYPE)
			{
				if (itemEffect == HOLD_EFFECT_PLATE)
					targetSpecies = sTypeToArceusForm[type];
				
				if (!targetSpecies)
					targetSpecies = SPECIES_ARCEUS;
			}
			break;
		case NATIONAL_DEX_GENESECT:
		    if (itemEffect == HOLD_EFFECT_DRIVE)
			{
				switch (type)
				{
					case TYPE_WATER:
						targetSpecies = SPECIES_GENESECT_DOUSE_DRIVE;
						break;
					case TYPE_FIRE:
						targetSpecies = SPECIES_GENESECT_BURN_DRIVE;
						break;
					case TYPE_ICE:
						targetSpecies = SPECIES_GENESECT_CHILL_DRIVE;
						break;
					case TYPE_ELECTRIC:
						targetSpecies = SPECIES_GENESECT_SHOCK_DRIVE;
						break;
				}
			}
			if (!targetSpecies)
				targetSpecies = SPECIES_GENESECT;
			break;
		case NATIONAL_DEX_SILVALLY:
		    if (ability == ABILITY_RKS_SYSTEM)
			{
				if (itemEffect == HOLD_EFFECT_MEMORY)
					targetSpecies = sTypeToSilvallyForm[type];
				
				if (!targetSpecies)
					targetSpecies = SPECIES_SILVALLY;
			}
			break;
		default:
		    break;
	}
	if (targetSpecies && targetSpecies != species)
	{
		SetMonData(mon, MON_DATA_SPECIES, &targetSpecies);
		CalculateMonStats(mon);
	}
}

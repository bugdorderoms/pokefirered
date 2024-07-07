#define MG_MON_NATURE(nature) (nature + 1)

#define MG_GIVE_POKEMON(pokemon) 		   \
	.type = MG_TYPE_GIVE_POKEMON,		   \
	.present = { .GivePokemon = pokemon }, \
	.presentsCount = ARRAY_COUNT(pokemon)

#define MG_GIVE_ITEM(item)     		   \
	.type = MG_TYPE_GIVE_ITEM, 		   \
	.present = { .GiveItem = item },   \
	.presentsCount = ARRAY_COUNT(item)

#define MG_FEATURE_UNLOCK(feature)  		 \
	.type = MG_TYPE_UNLOCK_FEATURE, 		 \
	.present = { .FeatureUnlock = feature }, \
	.presentsCount = ARRAY_COUNT(feature)

#define MG_END_TABLE { .type = MG_TYPES_COUNT }

/* Example of mystery gifts using all types
static const struct MGPokemon sMysteryGift_GiveDarkraiAndZoruaEgg[] =
{
	{
		.species = SPECIES_DARKRAI,
		.heldItem = ITEM_EV_IV_DISPLAY,
		.moves = {MOVE_NIGHT_SHADE, MOVE_GLARE, MOVE_TORMENT, MOVE_WATER_GUN},
		.nickname = COMPOUND_STRING("Darkness"),
		.ivs = {13, 14, 15, 16, 17, 18},
		.level = 20,
		.ballId = ITEM_DUSK_BALL,
		.abilityNum = 0,
		.abilityHidden = FALSE,
		.shinyType = GENERATE_SHINY_FORCED, // Always shiny
		.nature = MG_MON_NATURE(NATURE_ADAMANT),
		.isEgg = FALSE,
	},
	{
		.species = SPECIES_ZORUA,
		.heldItem = ITEM_NONE, // Irrelevant
		.moves = {MOVE_NIGHT_SHADE, MOVE_GLARE, MOVE_TORMENT, MOVE_WATER_GUN},
		.nickname = COMPOUND_STRING("Mask"),
		.ivs = {13, 14, 15, 16, 17, 18},
		.level = 0, // Irrelevant
		.ballId = ITEM_NONE, // Irrelevant
		.abilityNum = 1,
		.abilityHidden = TRUE,
		.shinyType = GENERATE_SHINY_LOCKED, // Never shiny
		.nature = MG_MON_NATURE(NATURE_MILD),
		.isEgg = TRUE,
	}
};

static const struct MGItem sMysteryGift_Give10EnigmaBerries[] =
{
	{
		.itemId = ITEM_ENIGMA_BERRY,
		.quantity = 10, // Amount to give
	}
};

static const struct MGFeatureUnlock sMysteryGift_UnlockPokedex[] =
{
	{ .featureId = MG_FEATURE_UNLOCK_POKEDEX }
};*/

static const struct MysteryGift sMysteryGifts[] =
{
	/* Example of mystery gifts using all types
	{
		.code = _("Nightmare"),
		MG_GIVE_POKEMON(sMysteryGift_GiveDarkraiAndZoruaEgg),
		.flag = FLAG_MYSTERY_GIFT_1
	},
	{
		.code = _("? Berry"),
		MG_GIVE_ITEM(sMysteryGift_Give10EnigmaBerries),
		.flag = FLAG_MYSTERY_GIFT_2
	},
	{
		.code = _("My Dex"),
		MG_FEATURE_UNLOCK(sMysteryGift_UnlockPokedex),
		.flag = FLAG_MYSTERY_GIFT_3
	},*/
	MG_END_TABLE // End of table
};
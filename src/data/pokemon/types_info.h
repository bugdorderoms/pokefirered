// TYPE_MUL_SUPER_EFFECTIVE = ×2.0
// TYPE_MUL_NORMAL          = ×1.0
// TYPE_MUL_NOT_EFFECTIVE   = ×0.5
// TYPE_MUL_NO_EFFECT       = ×0.0
const struct TypeInfo gTypesInfo[NUMBER_OF_MON_TYPES] =
{
	[TYPE_NORMAL] =
	{
		.name = _("Normal"),
		.tmPalette = sItemIconPalette_TmNormal,
		.effectiveness = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
		                  TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NO_EFFECT,       TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
	},
	
	[TYPE_FIGHTING] =
	{
		.name = _("Fighting"),
		.tmPalette = sItemIconPalette_TmFighting,
		.effectiveness = {TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
		                  TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NO_EFFECT,       TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,
						  TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL},
	},
	
    [TYPE_FLYING] =
	{
		.name = _("Flying"),
		.tmPalette = sItemIconPalette_TmFlying,
		.effectiveness = {TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
		                  TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
	},
	
    [TYPE_POISON] =
	{
		.name = _("Poison"),
		.tmPalette = sItemIconPalette_TmPoison,
		.effectiveness = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,
		                  TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NO_EFFECT,       TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL},
	},
	
    [TYPE_GROUND] =
	{
		.name = _("Ground"),
		.tmPalette = sItemIconPalette_TmGround,
		.effectiveness = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NO_EFFECT,       TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,
		                  TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,
						  TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
	},
	
    [TYPE_ROCK] =
	{
		.name = _("Rock"),
		.tmPalette = sItemIconPalette_TmRock,
		.effectiveness = {TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,
		                  TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
						  TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
						  TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
	},
	
    [TYPE_BUG] =
	{
		.name = _("Bug"),
		.tmPalette = sItemIconPalette_TmBug,
		.effectiveness = {TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
		                  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
						  TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL},
	},
	
    [TYPE_GHOST] =
	{
		.name = _("Ghost"),
		.tmPalette = sItemIconPalette_TmGhost,
		.effectiveness = {TYPE_MUL_NO_EFFECT,       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
		                  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
	},
	
    [TYPE_STEEL] =
	{
		.name = _("Steel"),
		.tmPalette = sItemIconPalette_TmSteel,
		.effectiveness = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
		                  TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
						  TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
						  TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL},
	},

    [TYPE_MYSTERY] =
	{
		.name = _("???"),
		.tmPalette = NULL,
		.effectiveness = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
		                  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
	},
	
    [TYPE_FIRE] =
	{
		.name = _("Fire"),
		.tmPalette = sItemIconPalette_TmFire,
		.effectiveness = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
		                  TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,
						  TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
						  TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
	},
	
    [TYPE_WATER] =
	{
		.name = _("Water"),
		.tmPalette = sItemIconPalette_TmWater,
		.effectiveness = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE,
		                  TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
						  TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
	},
	
    [TYPE_GRASS] =
	{
		.name = _("Grass"),
		.tmPalette = sItemIconPalette_TmGrass,
		.effectiveness = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_SUPER_EFFECTIVE,
		                  TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
						  TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
	},
	
    [TYPE_ELECTRIC] =
	{
		.name = _("Electric"),
		.tmPalette = sItemIconPalette_TmElectric,
		.effectiveness = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NO_EFFECT,
		                  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
	},
	
    [TYPE_PSYCHIC] =
	{
		.name = _("Psychic"),
		.tmPalette = sItemIconPalette_TmPsychic,
		.effectiveness = {TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,
		                  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NO_EFFECT,       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
	},
	
    [TYPE_ICE] =
	{
		.name = _("Ice"),
		.tmPalette = sItemIconPalette_TmIce,
		.effectiveness = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE,
		                  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
						  TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
						  TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
	},
	
    [TYPE_DRAGON] =
	{
		.name = _("Dragon"),
		.tmPalette = sItemIconPalette_TmDragon,
		.effectiveness = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
		                  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NO_EFFECT,       TYPE_MUL_NORMAL},
	},

    [TYPE_DARK] =
	{
		.name = _("Dark"),
		.tmPalette = sItemIconPalette_TmDark,
		.effectiveness = {TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
		                  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL},
	},
	
	[TYPE_FAIRY] =
	{
		.name = _("Fairy"),
		.tmPalette = sItemIconPalette_TmFairy,
		.effectiveness = {TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
		                  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
						  TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
	},

	[TYPE_STELLAR] =
	{
		.name = _("Stellar"),
		.tmPalette = NULL, // No one move has this type
		.effectiveness = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
		                  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
						  TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
	},
};

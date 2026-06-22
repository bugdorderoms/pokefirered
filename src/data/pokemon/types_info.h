const struct TypeInfo gTypesInfo[NUMBER_OF_MON_TYPES] =
{
    [TYPE_NORMAL] =
    {
        .name = _("Normal"),
        .tmPalette = sItemIconPalette_TmNormal,
        .teraBlendColor = RGB_WHITE,
        .maxMove = MOVE_MAX_STRIKE,
    },
    
    [TYPE_FIGHTING] =
    {
        .name = _("Fighting"),
        .tmPalette = sItemIconPalette_TmFighting,
        .teraBlendColor = RGB(26, 8, 14),
        .maxMove = MOVE_MAX_KNUCKLE,
    },
    
    [TYPE_FLYING] =
    {
        .name = _("Flying"),
        .tmPalette = sItemIconPalette_TmFlying,
        .teraBlendColor = RGB(31, 26, 7),
        .maxMove = MOVE_MAX_AIRSTREAM,
    },
    
    [TYPE_POISON] =
    {
        .name = _("Poison"),
        .tmPalette = sItemIconPalette_TmPoison,
        .teraBlendColor = RGB(26, 10, 25),
        .maxMove = MOVE_MAX_OOZE,
    },
    
    [TYPE_GROUND] =
    {
        .name = _("Ground"),
        .tmPalette = sItemIconPalette_TmGround,
        .teraBlendColor = RGB(25, 23, 18),
        .maxMove = MOVE_MAX_QUAKE,
    },
    
    [TYPE_ROCK] =
    {
        .name = _("Rock"),
        .tmPalette = sItemIconPalette_TmRock,
        .teraBlendColor = RGB(18, 16, 8),
        .maxMove = MOVE_MAX_ROCKFALL,
    },
    
    [TYPE_BUG] =
    {
        .name = _("Bug"),
        .tmPalette = sItemIconPalette_TmBug,
        .teraBlendColor = RGB(18, 24, 6),
        .maxMove = MOVE_MAX_FLUTTERBY,
    },
    
    [TYPE_GHOST] =
    {
        .name = _("Ghost"),
        .tmPalette = sItemIconPalette_TmGhost,
        .teraBlendColor = RGB(12, 10, 16),
        .maxMove = MOVE_MAX_PHANTASM,
    },
    
    [TYPE_STEEL] =
    {
        .name = _("Steel"),
        .tmPalette = sItemIconPalette_TmSteel,
        .teraBlendColor = RGB(19, 19, 20),
        .maxMove = MOVE_MAX_STEELSPIKE,
    },

    [TYPE_MYSTERY] =
    {
        .name = _("???"),
        .tmPalette = NULL,
        .teraBlendColor = RGB_WHITE,
        .maxMove = MOVE_MAX_STRIKE,
    },
    
    [TYPE_FIRE] =
    {
        .name = _("Fire"),
        .tmPalette = sItemIconPalette_TmFire,
        .teraBlendColor = RGB(31, 20, 11),
        .maxMove = MOVE_MAX_FLARE,
    },
    
    [TYPE_WATER] =
    {
        .name = _("Water"),
        .tmPalette = sItemIconPalette_TmWater,
        .teraBlendColor = RGB(10, 18, 27),
        .maxMove = MOVE_MAX_GEYSER,
    },
    
    [TYPE_GRASS] =
    {
        .name = _("Grass"),
        .tmPalette = sItemIconPalette_TmGrass,
        .teraBlendColor = RGB(12, 24, 11),
        .maxMove = MOVE_MAX_OVERGROWTH,
    },
    
    [TYPE_ELECTRIC] =
    {
        .name = _("Electric"),
        .tmPalette = sItemIconPalette_TmElectric,
        .teraBlendColor = RGB(30, 26, 7),
        .maxMove = MOVE_MAX_LIGHTNING,
    },
    
    [TYPE_PSYCHIC] =
    {
        .name = _("Psychic"),
        .tmPalette = sItemIconPalette_TmPsychic,
        .teraBlendColor = RGB(31, 14, 15),
        .maxMove = MOVE_MAX_MINDSTORM,
    },
    
    [TYPE_ICE] =
    {
        .name = _("Ice"),
        .tmPalette = sItemIconPalette_TmIce,
        .teraBlendColor = RGB(14, 26, 25),
        .maxMove = MOVE_MAX_HAILSTORM,
    },
    
    [TYPE_DRAGON] =
    {
        .name = _("Dragon"),
        .tmPalette = sItemIconPalette_TmDragon,
        .teraBlendColor = RGB(10, 18, 27),
        .maxMove = MOVE_MAX_WYRMWIND,
    },

    [TYPE_DARK] =
    {
        .name = _("Dark"),
        .tmPalette = sItemIconPalette_TmDark,
        .teraBlendColor = RGB(6, 5, 8),
        .maxMove = MOVE_MAX_DARKNESS,
    },
    
    [TYPE_FAIRY] =
    {
        .name = _("Fairy"),
        .tmPalette = sItemIconPalette_TmFairy,
        .teraBlendColor = RGB(31, 15, 21),
        .maxMove = MOVE_MAX_STARFALL,
    },

    [TYPE_STELLAR] =
    {
        .name = _("Stellar"),
        .tmPalette = NULL, // No one move has this type
        .teraBlendColor = RGB(10, 18, 27),
        .maxMove = MOVE_MAX_STRIKE,
    },
};

const u32 gTypeEffectivenessTable[NUMBER_OF_MON_TYPES][NUMBER_OF_MON_TYPES] =
{
                      // TYPE_NORMAL,            TYPE_FIGHTING,            TYPE_FLYING,              TYPE_POISON,              TYPE_GROUND,
                      // TYPE_ROCK,              TYPE_BUG,                 TYPE_GHOST,               TYPE_STEEL,               TYPE_MYSTERY,
                      // TYPE_FIRE,              TYPE_WATER,               TYPE_GRASS,               TYPE_ELECTRIC,            TYPE_PSYCHIC,
                      // TYPE_ICE,               TYPE_DRAGON,              TYPE_DARK,                TYPE_FAIRY,               TYPE_STELLAR
    [TYPE_NORMAL]   = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NO_EFFECT,       TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
                     
    [TYPE_FIGHTING] = {TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
                       TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NO_EFFECT,       TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,
                       TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL},
    
    [TYPE_FLYING]   = {TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
    
    [TYPE_POISON]   = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,
                       TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NO_EFFECT,       TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL},
    
    [TYPE_GROUND]   = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NO_EFFECT,       TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,
                       TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,
                       TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
                       
    [TYPE_ROCK]     = {TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,
                       TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
                       TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
    
    [TYPE_BUG]      = {TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
                       TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL},
    
    [TYPE_GHOST]    = {TYPE_MUL_NO_EFFECT,       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
    
    [TYPE_STEEL]    = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
                       TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
                       TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL},
    
    [TYPE_MYSTERY]  = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
    
    [TYPE_FIRE]     = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,
                       TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
    
    [TYPE_WATER]    = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE,
                       TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
    
    [TYPE_GRASS]    = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_SUPER_EFFECTIVE,
                       TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
                       TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
    
    [TYPE_ELECTRIC] = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NO_EFFECT,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
    
    [TYPE_PSYCHIC]  = {TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NO_EFFECT,       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
    
    [TYPE_ICE]      = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
                       TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
    
    [TYPE_DRAGON]   = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NO_EFFECT,       TYPE_MUL_NORMAL},
    
    [TYPE_DARK]     = {TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL},
    
    [TYPE_FAIRY]    = {TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,
                       TYPE_MUL_NOT_EFFECTIVE,   TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_SUPER_EFFECTIVE, TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
    
    [TYPE_STELLAR]  = {TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,
                       TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL,          TYPE_MUL_NORMAL},
};

const struct BattleMove gBattleMoves[MOVES_COUNT_GMAX] =
{
    [MOVE_NONE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .type = TYPE_NORMAL,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_PHYSICAL,
		.zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_POUND] =
    {
		.name = _("Pound"),
        .effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 35,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_KARATE_CHOP] =
    {
		.name = _("Karate Chop"),
        .effect = EFFECT_HIT,
        .power = 50,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 25,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.highCritChance = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DOUBLE_SLAP] =
    {
		.name = _("Double Slap"),
        .effect = EFFECT_MULTI_HIT,
        .power = 15,
        .type = TYPE_NORMAL,
        .accuracy = 85,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_COMET_PUNCH] =
    {
		.name = _("Comet Punch"),
        .effect = EFFECT_MULTI_HIT,
        .power = 18,
        .type = TYPE_NORMAL,
        .accuracy = 85,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.punchMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MEGA_PUNCH] =
    {
		.name = _("Mega Punch"),
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_NORMAL,
        .accuracy = 85,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.punchMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PAY_DAY] =
    {
		.name = _("Pay Day"),
        .effect = EFFECT_PAY_DAY,
        .power = 40,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FIRE_PUNCH] =
    {
		.name = _("Fire Punch"),
        .effect = EFFECT_BURN_HIT,
        .power = 75,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.punchMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ICE_PUNCH] =
    {
		.name = _("Ice Punch"),
        .effect = EFFECT_FREEZE_HIT,
        .power = 75,
        .type = TYPE_ICE,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.punchMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_THUNDER_PUNCH] =
    {
		.name = _("Thunder Punch"),
        .effect = EFFECT_PARALYZE_HIT,
        .power = 75,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.punchMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SCRATCH] =
    {
		.name = _("Scratch"),
        .effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 35,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_VISE_GRIP] =
    {
		.name = _("Vise Grip"),
        .effect = EFFECT_HIT,
        .power = 55,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_GUILLOTINE] =
    {
		.name = _("Guillotine"),
        .effect = EFFECT_OHKO,
        .power = 1,
        .type = TYPE_NORMAL,
        .accuracy = 30,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_RAZOR_WIND] =
    {
		.name = _("Razor Wind"),
        .effect = EFFECT_TWO_TURNS_ATTACK,
        .power = 80,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_BOTH,
		.flags =
		{
			.kingsRockAffected = TRUE,
			.highCritChance = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE,
			.twoTurnsMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SWORDS_DANCE] =
    {
		.name = _("Swords Dance"),
        .effect = EFFECT_USER_ATTACK_UP_2,
        .type = TYPE_NORMAL,
		.pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.danceMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_CUT] =
    {
		.name = _("Cut"),
        .effect = EFFECT_HIT,
        .power = 50,
        .type = TYPE_NORMAL,
        .accuracy = 95,
        .pp = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.slicingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_GUST] =
    {
		.name = _("Gust"),
        .effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_FLYING,
        .accuracy = 100,
        .pp = 35,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.hitInAirDoubleDmg = TRUE,
			.windMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WING_ATTACK] =
    {
		.name = _("Wing Attack"),
        .effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_FLYING,
        .accuracy = 100,
        .pp = 35,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WHIRLWIND] =
    {
		.name = _("Whirlwind"),
        .effect = EFFECT_ROAR,
        .type = TYPE_NORMAL,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .priority = -6,
		.flags =
		{
			.magicCoatAffected = TRUE,
			.windMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_FLY] =
    {
		.name = _("Fly"),
        .effect = EFFECT_SEMI_INVULNERABLE,
		.power = 90,
        .type = TYPE_FLYING,
        .accuracy = 95,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.twoTurnsMove = TRUE,
			.gravityBanned = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BIND] =
    {
		.name = _("Bind"),
        .effect = EFFECT_TRAP,
        .power = 15,
        .type = TYPE_NORMAL,
		.accuracy = 85,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
		.argument = TRAP_ID_BIND,
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SLAM] =
    {
		.name = _("Slam"),
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_NORMAL,
        .accuracy = 75,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_VINE_WHIP] =
    {
		.name = _("Vine Whip"),
        .effect = EFFECT_HIT,
		.power = 45,
        .type = TYPE_GRASS,
        .accuracy = 100,
		.pp = 25,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STOMP] =
    {
		.name = _("Stomp"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 65,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.dmgMinimize = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DOUBLE_KICK] =
    {
		.name = _("Double Kick"),
        .effect = EFFECT_HIT,
        .power = 30,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.strikeCount = 2,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MEGA_KICK] =
    {
		.name = _("Mega Kick"),
        .effect = EFFECT_HIT,
        .power = 120,
        .type = TYPE_NORMAL,
        .accuracy = 75,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_JUMP_KICK] =
    {
		.name = _("Jump Kick"),
        .effect = EFFECT_RECOIL_IF_MISS,
		.power = 100,
        .type = TYPE_FIGHTING,
        .accuracy = 95,
		.pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.gravityBanned = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ROLLING_KICK] =
    {
		.name = _("Rolling Kick"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 60,
        .type = TYPE_FIGHTING,
        .accuracy = 85,
        .pp = 15,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SAND_ATTACK] =
    {
		.name = _("Sand Attack"),
        .effect = EFFECT_ACCURACY_DOWN,
        .type = TYPE_GROUND,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_EVSN_UP_1,
    },

    [MOVE_HEADBUTT] =
    {
		.name = _("Headbutt"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 70,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HORN_ATTACK] =
    {
		.name = _("Horn Attack"),
        .effect = EFFECT_HIT,
        .power = 65,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 25,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FURY_ATTACK] =
    {
		.name = _("Fury Attack"),
        .effect = EFFECT_MULTI_HIT,
        .power = 15,
        .type = TYPE_NORMAL,
        .accuracy = 85,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HORN_DRILL] =
    {
		.name = _("Horn Drill"),
        .effect = EFFECT_OHKO,
        .power = 1,
        .type = TYPE_NORMAL,
        .accuracy = 30,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TACKLE] =
    {
		.name = _("Tackle"),
        .effect = EFFECT_HIT,
		.power = 40,
        .type = TYPE_NORMAL,
		.accuracy = 100,
        .pp = 35,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BODY_SLAM] =
    {
		.name = _("Body Slam"),
        .effect = EFFECT_PARALYZE_HIT,
        .power = 85,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			.dmgMinimize = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WRAP] =
    {
		.name = _("Wrap"),
        .effect = EFFECT_TRAP,
        .power = 15,
        .type = TYPE_NORMAL,
		.accuracy = 90,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
		.argument = TRAP_ID_WRAP,
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TAKE_DOWN] =
    {
		.name = _("Take Down"),
        .effect = EFFECT_HIT,
        .power = 90,
        .type = TYPE_NORMAL,
        .accuracy = 85,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.recoilDivisor = 4, // 25%
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_THRASH] =
    {
		.name = _("Thrash"),
        .effect = EFFECT_RAMPAGE,
		.power = 120,
        .type = TYPE_NORMAL,
        .accuracy = 100,
		.pp = 10,
        .target = MOVE_TARGET_RANDOM,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenInstruct = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DOUBLE_EDGE] =
    {
		.name = _("Double-Edge"),
        .effect = EFFECT_HIT,
        .power = 120,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.recoilDivisor = 3, // 33%
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TAIL_WHIP] =
    {
		.name = _("Tail Whip"),
        .effect = EFFECT_DEFENSE_DOWN,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_1,
    },

    [MOVE_POISON_STING] =
    {
		.name = _("Poison Sting"),
        .effect = EFFECT_POISON_HIT,
        .power = 15,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 35,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TWINEEDLE] =
    {
		.name = _("Twineedle"),
        .effect = EFFECT_POISON_HIT,
        .power = 25,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.strikeCount = 2,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PIN_MISSILE] =
    {
		.name = _("Pin Missile"),
        .effect = EFFECT_MULTI_HIT,
		.power = 25,
        .type = TYPE_BUG,
		.accuracy = 95,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LEER] =
    {
		.name = _("Leer"),
        .effect = EFFECT_DEFENSE_DOWN,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_1,
    },

    [MOVE_BITE] =
    {
		.name = _("Bite"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 60,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 25,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.bitingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_GROWL] =
    {
		.name = _("Growl"),
        .effect = EFFECT_ATTACK_DOWN,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 40,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.soundMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_ROAR] =
    {
		.name = _("Roar"),
        .effect = EFFECT_ROAR,
        .type = TYPE_NORMAL,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .priority = -6,
		.flags =
		{
			.soundMove = TRUE,
			.magicCoatAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_SING] =
    {
		.name = _("Sing"),
        .effect = EFFECT_SLEEP,
        .type = TYPE_NORMAL,
        .accuracy = 55,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.soundMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_SUPERSONIC] =
    {
		.name = _("Supersonic"),
        .effect = EFFECT_CONFUSE,
        .type = TYPE_NORMAL,
        .accuracy = 55,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.soundMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_SONIC_BOOM] =
    {
		.name = _("Sonic Boom"),
        .effect = EFFECT_FIXED_DAMAGE,
        .power = 1,
        .type = TYPE_NORMAL,
        .accuracy = 90,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.noEffectiveness = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.argument = 20,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DISABLE] =
    {
		.name = _("Disable"),
        .effect = EFFECT_DISABLE,
        .type = TYPE_NORMAL,
		.accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_ACID] =
    {
		.name = _("Acid"),
        .effect = EFFECT_TARGET_SP_DEFENSE_DOWN_HIT,
        .power = 40,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 30,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_BOTH,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_EMBER] =
    {
		.name = _("Ember"),
        .effect = EFFECT_BURN_HIT,
        .power = 40,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 25,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FLAMETHROWER] =
    {
		.name = _("Flamethrower"),
        .effect = EFFECT_BURN_HIT,
		.power = 90,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MIST] =
    {
		.name = _("Mist"),
        .effect = EFFECT_MIST,
        .type = TYPE_ICE,
        .pp = 30,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.affectsUserSide = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RECOVER_HP,
    },

    [MOVE_WATER_GUN] =
    {
		.name = _("Water Gun"),
        .effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 25,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HYDRO_PUMP] =
    {
		.name = _("Hydro Pump"),
        .effect = EFFECT_HIT,
		.power = 110,
        .type = TYPE_WATER,
        .accuracy = 80,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SURF] =
    {
		.name = _("Surf"),
        .effect = EFFECT_HIT,
		.power = 90,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 15,
		.target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.hitUnderwater = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ICE_BEAM] =
    {
		.name = _("Ice Beam"),
        .effect = EFFECT_FREEZE_HIT,
		.power = 90,
        .type = TYPE_ICE,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BLIZZARD] =
    {
		.name = _("Blizzard"),
        .effect = EFFECT_FREEZE_HIT,
		.power = 110,
        .type = TYPE_ICE,
        .accuracy = 70,
        .pp = 5,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.windMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PSYBEAM] =
    {
		.name = _("Psybeam"),
        .effect = EFFECT_CONFUSE_HIT,
        .power = 65,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BUBBLE_BEAM] =
    {
		.name = _("Bubble Beam"),
        .effect = EFFECT_TARGET_SPEED_DOWN_HIT,
        .power = 65,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_AURORA_BEAM] =
    {
		.name = _("Aurora Beam"),
        .effect = EFFECT_TARGET_ATTACK_DOWN_HIT,
        .power = 65,
        .type = TYPE_ICE,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HYPER_BEAM] =
    {
		.name = _("Hyper Beam"),
        .effect = EFFECT_RECHARGE,
        .power = 150,
        .type = TYPE_NORMAL,
        .accuracy = 90,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenInstruct = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PECK] =
    {
		.name = _("Peck"),
        .effect = EFFECT_HIT,
        .power = 35,
        .type = TYPE_FLYING,
        .accuracy = 100,
        .pp = 35,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DRILL_PECK] =
    {
		.name = _("Drill Peck"),
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_FLYING,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SUBMISSION] =
    {
		.name = _("Submission"),
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_FIGHTING,
        .accuracy = 80,
		.pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.recoilDivisor = 4, // 25%
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LOW_KICK] =
    {
		.name = _("Low Kick"),
        .effect = EFFECT_LOW_KICK,
        .power = 1,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_COUNTER] =
    {
		.name = _("Counter"),
        .effect = EFFECT_COUNTER,
        .power = 1,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_DEPENDS,
        .priority = -5,
		.flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.noEffectiveness = TRUE,
			.forbiddenMeFirst = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SEISMIC_TOSS] =
    {
		.name = _("Seismic Toss"),
        .effect = EFFECT_LEVEL_DAMAGE,
        .power = 1,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.noEffectiveness = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STRENGTH] =
    {
		.name = _("Strength"),
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ABSORB] =
    {
		.name = _("Absorb"),
        .effect = EFFECT_ABSORB,
        .power = 20,
        .type = TYPE_GRASS,
        .accuracy = 100,
		.pp = 25,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.kingsRockAffected = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MEGA_DRAIN] =
    {
		.name = _("Mega Drain"),
        .effect = EFFECT_ABSORB,
        .power = 40,
        .type = TYPE_GRASS,
        .accuracy = 100,
		.pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.kingsRockAffected = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LEECH_SEED] =
    {
		.name = _("Leech Seed"),
        .effect = EFFECT_LEECH_SEED,
        .type = TYPE_GRASS,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_GROWTH] =
    {
		.name = _("Growth"),
        .effect = EFFECT_GROWTH,
        .type = TYPE_NORMAL,
		.pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_RAZOR_LEAF] =
    {
		.name = _("Razor Leaf"),
        .effect = EFFECT_HIT,
        .power = 55,
        .type = TYPE_GRASS,
        .accuracy = 95,
        .pp = 25,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.highCritChance = TRUE,
			.slicingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SOLAR_BEAM] =
    {
		.name = _("Solar Beam"),
        .effect = EFFECT_SOLARBEAM,
        .power = 120,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE,
			.twoTurnsMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_POISON_POWDER] =
    {
		.name = _("Poison Powder"),
        .effect = EFFECT_POISON,
        .type = TYPE_POISON,
        .accuracy = 75,
        .pp = 35,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.powderMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_STUN_SPORE] =
    {
		.name = _("Stun Spore"),
        .effect = EFFECT_PARALYZE,
        .type = TYPE_GRASS,
        .accuracy = 75,
        .pp = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.powderMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_SLEEP_POWDER] =
    {
		.name = _("Sleep Powder"),
        .effect = EFFECT_SLEEP,
        .type = TYPE_GRASS,
        .accuracy = 75,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.powderMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_PETAL_DANCE] =
    {
		.name = _("Petal Dance"),
        .effect = EFFECT_RAMPAGE,
		.power = 120,
        .type = TYPE_GRASS,
        .accuracy = 100,
		.pp = 10,
        .target = MOVE_TARGET_RANDOM,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.danceMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STRING_SHOT] =
    {
		.name = _("String Shot"),
        .effect = EFFECT_SPEED_DOWN_2,
        .type = TYPE_BUG,
        .accuracy = 95,
        .pp = 40,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_DRAGON_RAGE] =
    {
		.name = _("Dragon Rage"),
        .effect = EFFECT_FIXED_DAMAGE,
        .power = 1,
        .type = TYPE_DRAGON,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.noEffectiveness = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.argument = 40,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FIRE_SPIN] =
    {
		.name = _("Fire Spin"),
        .effect = EFFECT_TRAP,
		.power = 35,
        .type = TYPE_FIRE,
		.accuracy = 85,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
		.argument = TRAP_ID_FIRE_SPIN,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_THUNDER_SHOCK] =
    {
		.name = _("Thunder Shock"),
        .effect = EFFECT_PARALYZE_HIT,
        .power = 40,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 30,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_THUNDERBOLT] =
    {
		.name = _("Thunderbolt"),
        .effect = EFFECT_PARALYZE_HIT,
		.power = 90,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_THUNDER_WAVE] =
    {
		.name = _("Thunder Wave"),
        .effect = EFFECT_PARALYZE,
        .type = TYPE_ELECTRIC,
		.accuracy = 90,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_THUNDER] =
    {
		.name = _("Thunder"),
        .effect = EFFECT_PARALYZE_HIT,
		.power = 110,
        .type = TYPE_ELECTRIC,
        .accuracy = 70,
        .pp = 10,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.hitInAir = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ROCK_THROW] =
    {
		.name = _("Rock Throw"),
        .effect = EFFECT_HIT,
        .power = 50,
        .type = TYPE_ROCK,
        .accuracy = 90,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_EARTHQUAKE] =
    {
		.name = _("Earthquake"),
        .effect = EFFECT_HIT,
        .power = 100,
        .type = TYPE_GROUND,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.hitUnderground = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FISSURE] =
    {
		.name = _("Fissure"),
        .effect = EFFECT_OHKO,
        .power = 1,
        .type = TYPE_GROUND,
        .accuracy = 30,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DIG] =
    {
		.name = _("Dig"),
        .effect = EFFECT_SEMI_INVULNERABLE,
		.power = 80,
        .type = TYPE_GROUND,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.twoTurnsMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TOXIC] =
    {
		.name = _("Toxic"),
        .effect = EFFECT_TOXIC,
        .type = TYPE_POISON,
		.accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_CONFUSION] =
    {
		.name = _("Confusion"),
        .effect = EFFECT_CONFUSE_HIT,
        .power = 50,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 25,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PSYCHIC] =
    {
		.name = _("Psychic"),
        .effect = EFFECT_TARGET_SP_DEFENSE_DOWN_HIT,
        .power = 90,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HYPNOSIS] =
    {
		.name = _("Hypnosis"),
        .effect = EFFECT_SLEEP,
        .type = TYPE_PSYCHIC,
        .accuracy = 60,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_MEDITATE] =
    {
		.name = _("Meditate"),
        .effect = EFFECT_ATTACK_UP,
        .type = TYPE_PSYCHIC,
        .pp = 40,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_1,
    },

    [MOVE_AGILITY] =
    {
		.name = _("Agility"),
        .effect = EFFECT_SPEED_UP_2,
        .type = TYPE_PSYCHIC,
        .pp = 30,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_QUICK_ATTACK] =
    {
		.name = _("Quick Attack"),
        .effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_SELECTED,
        .priority = 1,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_RAGE] =
    {
		.name = _("Rage"),
        .effect = EFFECT_RAGE,
        .power = 20,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TELEPORT] =
    {
		.name = _("Teleport"),
        .effect = EFFECT_TELEPORT,
        .type = TYPE_PSYCHIC,
        .pp = 20,
        .target = MOVE_TARGET_USER,
        .priority = -6,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RECOVER_HP,
    },

    [MOVE_NIGHT_SHADE] =
    {
		.name = _("Night Shade"),
        .effect = EFFECT_LEVEL_DAMAGE,
        .power = 1,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.noEffectiveness = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MIMIC] =
    {
		.name = _("Mimic"),
        .effect = EFFECT_MIMIC,
        .type = TYPE_NORMAL,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenMimic = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
        },
		.split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ACC_UP_1,
    },

    [MOVE_SCREECH] =
    {
		.name = _("Screech"),
        .effect = EFFECT_DEFENSE_DOWN_2,
        .type = TYPE_NORMAL,
        .accuracy = 85,
        .pp = 40,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.soundMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_1,
    },

    [MOVE_DOUBLE_TEAM] =
    {
		.name = _("Double Team"),
        .effect = EFFECT_EVASION_UP,
        .type = TYPE_NORMAL,
        .pp = 15,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_RECOVER] =
    {
		.name = _("Recover"),
        .effect = EFFECT_RESTORE_HP,
        .type = TYPE_NORMAL,
		.pp = 5,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_HARDEN] =
    {
		.name = _("Harden"),
        .effect = EFFECT_DEFENSE_UP,
        .type = TYPE_NORMAL,
        .pp = 30,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_MINIMIZE] =
    {
		.name = _("Minimize"),
        .effect = EFFECT_MINIMIZE,
        .type = TYPE_NORMAL,
		.pp = 10,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_SMOKESCREEN] =
    {
		.name = _("Smokescreen"),
        .effect = EFFECT_ACCURACY_DOWN,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_EVSN_UP_1,
    },

    [MOVE_CONFUSE_RAY] =
    {
		.name = _("Confuse Ray"),
        .effect = EFFECT_CONFUSE,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_WITHDRAW] =
    {
		.name = _("Withdraw"),
        .effect = EFFECT_DEFENSE_UP,
        .type = TYPE_WATER,
        .pp = 40,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_DEFENSE_CURL] =
    {
		.name = _("Defense Curl"),
        .effect = EFFECT_DEFENSE_CURL,
        .type = TYPE_NORMAL,
        .pp = 40,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ACC_UP_1,
    },

    [MOVE_BARRIER] =
    {
		.name = _("Barrier"),
        .effect = EFFECT_DEFENSE_UP_2,
        .type = TYPE_PSYCHIC,
		.pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_LIGHT_SCREEN] =
    {
		.name = _("Light Screen"),
        .effect = EFFECT_LIGHT_SCREEN,
        .type = TYPE_PSYCHIC,
        .pp = 30,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.affectsUserSide = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_HAZE] =
    {
		.name = _("Haze"),
        .effect = EFFECT_HAZE,
        .type = TYPE_ICE,
        .pp = 30,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RECOVER_HP,
    },

    [MOVE_REFLECT] =
    {
		.name = _("Reflect"),
        .effect = EFFECT_REFLECT,
        .type = TYPE_PSYCHIC,
        .pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.affectsUserSide = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_FOCUS_ENERGY] =
    {
		.name = _("Focus Energy"),
        .effect = EFFECT_FOCUS_ENERGY,
        .type = TYPE_NORMAL,
        .pp = 30,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ACC_UP_1,
    },

    [MOVE_BIDE] =
    {
		.name = _("Bide"),
        .effect = EFFECT_BIDE,
        .power = 1,
        .type = TYPE_NORMAL,
        .pp = 10,
        .target = MOVE_TARGET_USER,
		.priority = 1,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE, // Note: Bide should work with Parental Bond. This will be addressed in future.
			.twoTurnsMove = TRUE,
			.noEffectiveness = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_METRONOME] =
    {
		.name = _("Metronome"),
        .effect = EFFECT_METRONOME,
        .type = TYPE_NORMAL,
        .pp = 10,
        .target = MOVE_TARGET_DEPENDS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.callOtherMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MIRROR_MOVE] =
    {
		.name = _("Mirror Move"),
        .effect = EFFECT_MIRROR_MOVE,
        .type = TYPE_FLYING,
        .pp = 20,
        .target = MOVE_TARGET_DEPENDS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.callOtherMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_2,
    },

    [MOVE_SELF_DESTRUCT] =
    {
		.name = _("Self-Destruct"),
        .effect = EFFECT_EXPLOSION,
        .power = 200,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_EGG_BOMB] =
    {
		.name = _("Egg Bomb"),
        .effect = EFFECT_HIT,
        .power = 100,
        .type = TYPE_NORMAL,
        .accuracy = 75,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.ballisticMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LICK] =
    {
		.name = _("Lick"),
        .effect = EFFECT_PARALYZE_HIT,
		.power = 30,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 30,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SMOG] =
    {
		.name = _("Smog"),
        .effect = EFFECT_POISON_HIT,
		.power = 30,
        .type = TYPE_POISON,
        .accuracy = 70,
        .pp = 20,
        .secondaryEffectChance = 40,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SLUDGE] =
    {
		.name = _("Sludge"),
        .effect = EFFECT_POISON_HIT,
        .power = 65,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BONE_CLUB] =
    {
		.name = _("Bone Club"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 65,
        .type = TYPE_GROUND,
        .accuracy = 85,
        .pp = 20,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FIRE_BLAST] =
    {
		.name = _("Fire Blast"),
        .effect = EFFECT_BURN_HIT,
		.power = 110,
        .type = TYPE_FIRE,
        .accuracy = 85,
        .pp = 5,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WATERFALL] =
    {
		.name = _("Waterfall"),
		.effect = EFFECT_FLINCH_HIT,
        .power = 80,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CLAMP] =
    {
		.name = _("Clamp"),
        .effect = EFFECT_TRAP,
        .power = 35,
        .type = TYPE_WATER,
		.accuracy = 85,
		.pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
		.argument = TRAP_ID_CLAMP,
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SWIFT] =
    {
		.name = _("Swift"),
        .effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_NORMAL,
        .pp = 20,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SKULL_BASH] =
    {
		.name = _("Skull Bash"),
        .effect = EFFECT_SKULL_BASH,
		.power = 130,
        .type = TYPE_NORMAL,
        .accuracy = 100,
		.pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE,
			.twoTurnsMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPIKE_CANNON] =
    {
		.name = _("Spike Cannon"),
        .effect = EFFECT_MULTI_HIT,
        .power = 20,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CONSTRICT] =
    {
		.name = _("Constrict"),
        .effect = EFFECT_TARGET_SPEED_DOWN_HIT,
        .power = 10,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 35,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_AMNESIA] =
    {
		.name = _("Amnesia"),
        .effect = EFFECT_SPECIAL_DEFENSE_UP_2,
        .type = TYPE_PSYCHIC,
        .pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_KINESIS] =
    {
		.name = _("Kinesis"),
        .effect = EFFECT_ACCURACY_DOWN,
        .type = TYPE_PSYCHIC,
        .accuracy = 80,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_EVSN_UP_1,
    },

    [MOVE_SOFT_BOILED] =
    {
		.name = _("Soft-Boiled"),
        .effect = EFFECT_RESTORE_HP,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_HI_JUMP_KICK] =
    {
		.name = _("High Jump Kick"),
        .effect = EFFECT_RECOIL_IF_MISS,
		.power = 130,
        .type = TYPE_FIGHTING,
        .accuracy = 90,
		.pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.gravityBanned = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_GLARE] =
    {
		.name = _("Glare"),
        .effect = EFFECT_PARALYZE,
        .type = TYPE_NORMAL,
		.accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_DREAM_EATER] =
    {
		.name = _("Dream Eater"),
        .effect = EFFECT_DREAM_EATER,
        .power = 100,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.healingMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_POISON_GAS] =
    {
		.name = _("Poison Gas"),
        .effect = EFFECT_POISON,
        .type = TYPE_POISON,
		.accuracy = 90,
        .pp = 40,
		.target = MOVE_TARGET_BOTH,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_BARRAGE] =
    {
		.name = _("Barrage"),
        .effect = EFFECT_MULTI_HIT,
        .power = 15,
        .type = TYPE_NORMAL,
        .accuracy = 85,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.ballisticMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LEECH_LIFE] =
    {
		.name = _("Leech Life"),
        .effect = EFFECT_ABSORB,
		.power = 80,
        .type = TYPE_BUG,
        .accuracy = 100,
		.pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LOVELY_KISS] =
    {
		.name = _("Lovely Kiss"),
        .effect = EFFECT_SLEEP,
        .type = TYPE_NORMAL,
        .accuracy = 75,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_SKY_ATTACK] =
    {
		.name = _("Sky Attack"),
        .effect = EFFECT_TWO_TURNS_ATTACK,
        .power = 140,
        .type = TYPE_FLYING,
        .accuracy = 90,
        .pp = 5,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.highCritChance = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE,
			.twoTurnsMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .argument = MOVE_EFFECT_FLINCH,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TRANSFORM] =
    {
		.name = _("Transform"),
        .effect = EFFECT_TRANSFORM,
        .type = TYPE_NORMAL,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenMimic = TRUE,
			.forbiddenInstruct = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RECOVER_HP,
    },

    [MOVE_BUBBLE] =
    {
		.name = _("Bubble"),
        .effect = EFFECT_TARGET_SPEED_DOWN_HIT,
		.power = 40,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 30,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_BOTH,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DIZZY_PUNCH] =
    {
		.name = _("Dizzy Punch"),
        .effect = EFFECT_CONFUSE_HIT,
        .power = 70,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.punchMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPORE] =
    {
		.name = _("Spore"),
        .effect = EFFECT_SLEEP,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.powderMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_FLASH] =
    {
		.name = _("Flash"),
        .effect = EFFECT_ACCURACY_DOWN,
        .type = TYPE_NORMAL,
		.accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_EVSN_UP_1,
    },

    [MOVE_PSYWAVE] =
    {
		.name = _("Psywave"),
        .effect = EFFECT_PSYWAVE,
        .power = 1,
        .type = TYPE_PSYCHIC,
		.accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.noEffectiveness = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPLASH] =
    {
		.name = _("Splash"),
        .effect = EFFECT_DO_NOTHING,
        .type = TYPE_NORMAL,
        .pp = 40,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.gravityBanned = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_3,
    },

    [MOVE_ACID_ARMOR] =
    {
		.name = _("Acid Armor"),
        .effect = EFFECT_DEFENSE_UP_2,
        .type = TYPE_POISON,
		.pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_CRABHAMMER] =
    {
		.name = _("Crabhammer"),
        .effect = EFFECT_HIT,
		.power = 100,
        .type = TYPE_WATER,
		.accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.highCritChance = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_EXPLOSION] =
    {
		.name = _("Explosion"),
        .effect = EFFECT_EXPLOSION,
        .power = 250,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FURY_SWIPES] =
    {
		.name = _("Fury Swipes"),
        .effect = EFFECT_MULTI_HIT,
        .power = 18,
        .type = TYPE_NORMAL,
        .accuracy = 80,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BONEMERANG] =
    {
		.name = _("Bonemerang"),
        .effect = EFFECT_HIT,
        .power = 50,
        .type = TYPE_GROUND,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.strikeCount = 2,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_REST] =
    {
		.name = _("Rest"),
        .effect = EFFECT_REST,
        .type = TYPE_PSYCHIC,
        .pp = 5,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_ROCK_SLIDE] =
    {
		.name = _("Rock Slide"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 75,
        .type = TYPE_ROCK,
        .accuracy = 90,
        .pp = 10,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_BOTH,
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HYPER_FANG] =
    {
		.name = _("Hyper Fang"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 80,
        .type = TYPE_NORMAL,
        .accuracy = 90,
        .pp = 15,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.bitingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHARPEN] =
    {
		.name = _("Sharpen"),
        .effect = EFFECT_ATTACK_UP,
        .type = TYPE_NORMAL,
        .pp = 30,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_1,
    },

    [MOVE_CONVERSION] =
    {
		.name = _("Conversion"),
        .effect = EFFECT_CONVERSION,
        .type = TYPE_NORMAL,
        .pp = 30,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ALL_STATS_UP_1,
    },

    [MOVE_TRI_ATTACK] =
    {
		.name = _("Tri Attack"),
        .effect = EFFECT_TRI_ATTACK,
        .power = 80,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SUPER_FANG] =
    {
		.name = _("Super Fang"),
        .effect = EFFECT_SUPER_FANG,
        .power = 1,
        .type = TYPE_NORMAL,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.noEffectiveness = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SLASH] =
    {
		.name = _("Slash"),
        .effect = EFFECT_HIT,
        .power = 70,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.highCritChance = TRUE,
			.slicingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SUBSTITUTE] =
    {
		.name = _("Substitute"),
        .effect = EFFECT_SUBSTITUTE,
        .type = TYPE_NORMAL,
        .pp = 10,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_STRUGGLE] =
    {
		.name = _("Struggle"),
        .effect = EFFECT_HIT,
        .power = 50,
        .type = TYPE_NORMAL,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.recoilDivisor = 4, // 25% HP
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenMimic = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenMeFirst = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SKETCH] =
    {
		.name = _("Sketch"),
        .effect = EFFECT_SKETCH,
        .type = TYPE_NORMAL,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenMimic = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ALL_STATS_UP_1,
    },

    [MOVE_TRIPLE_KICK] =
    {
		.name = _("Triple Kick"),
        .effect = EFFECT_TRIPLE_KICK,
        .power = 10,
        .type = TYPE_FIGHTING,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.strikeCount = 3,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_THIEF] =
    {
		.name = _("Thief"),
        .effect = EFFECT_THIEF, // TODO:
		.power = 60,
        .type = TYPE_DARK,
        .accuracy = 100,
		.pp = 25,
        .secondaryEffectChance = 100, // Remove it bc It'snt affected by Sheer Force
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenMeFirst = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPIDER_WEB] =
    {
		.name = _("Spider Web"),
        .effect = EFFECT_MEAN_LOOK,
        .type = TYPE_BUG,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.magicCoatAffected = TRUE,
			.forbiddenProtect = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_MIND_READER] =
    {
		.name = _("Mind Reader"),
        .effect = EFFECT_LOCK_ON,
        .type = TYPE_NORMAL,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_NIGHTMARE] =
    {
		.name = _("Nightmare"),
        .effect = EFFECT_NIGHTMARE,
        .type = TYPE_GHOST,
		.accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_FLAME_WHEEL] =
    {
		.name = _("Flame Wheel"),
        .effect = EFFECT_BURN_HIT,
        .power = 60,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 25,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.thawUser = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SNORE] =
    {
		.name = _("Snore"),
        .effect = EFFECT_SNORE,
		.power = 50,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.soundMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CURSE] =
    {
		.name = _("Curse"),
        .effect = EFFECT_CURSE,
		.type = TYPE_GHOST,
        .pp = 10,
        .target = MOVE_TARGET_RANDOM,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_CURSE,
    },

    [MOVE_FLAIL] =
    {
		.name = _("Flail"),
        .effect = EFFECT_FLAIL,
        .power = 1,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CONVERSION_2] =
    {
		.name = _("Conversion 2"),
        .effect = EFFECT_CONVERSION_2,
        .type = TYPE_NORMAL,
        .pp = 30,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RECOVER_HP,
    },

    [MOVE_AEROBLAST] =
    {
		.name = _("Aeroblast"),
        .effect = EFFECT_HIT,
        .power = 100,
        .type = TYPE_FLYING,
        .accuracy = 95,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.highCritChance = TRUE,
			.windMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_COTTON_SPORE] =
    {
		.name = _("Cotton Spore"),
        .effect = EFFECT_SPEED_DOWN_2,
        .type = TYPE_GRASS,
		.accuracy = 100,
        .pp = 40,
		.target = MOVE_TARGET_BOTH,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.powderMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_REVERSAL] =
    {
		.name = _("Reversal"),
        .effect = EFFECT_FLAIL,
        .power = 1,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPITE] =
    {
		.name = _("Spite"),
        .effect = EFFECT_SPITE,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.magicCoatAffected = TRUE,
		},
		.argument = 4, // PP to deduct
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RECOVER_HP,
    },

    [MOVE_POWDER_SNOW] =
    {
		.name = _("Powder Snow"),
        .effect = EFFECT_FREEZE_HIT,
        .power = 40,
        .type = TYPE_ICE,
        .accuracy = 100,
        .pp = 25,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_BOTH,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PROTECT] =
    {
		.name = _("Protect"),
        .effect = EFFECT_PROTECT,
        .type = TYPE_NORMAL,
        .pp = 10,
        .target = MOVE_TARGET_USER,
		.priority = 4,
        .flags =
		{
			.protectionMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_MACH_PUNCH] =
    {
		.name = _("Mach Punch"),
        .effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_SELECTED,
        .priority = 1,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.punchMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SCARY_FACE] =
    {
		.name = _("Scary Face"),
        .effect = EFFECT_SPEED_DOWN_2,
        .type = TYPE_NORMAL,
		.accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_FAINT_ATTACK] =
    {
		.name = _("Feint Attack"),
        .effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_DARK,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SWEET_KISS] =
    {
		.name = _("Sweet Kiss"),
        .effect = EFFECT_CONFUSE,
		.type = TYPE_FAIRY,
        .accuracy = 75,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_BELLY_DRUM] =
    {
		.name = _("Belly Drum"),
        .effect = EFFECT_BELLY_DRUM,
        .type = TYPE_NORMAL,
        .pp = 10,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RECOVER_HP,
    },

    [MOVE_SLUDGE_BOMB] =
    {
		.name = _("Sludge Bomb"),
        .effect = EFFECT_POISON_HIT,
        .power = 90,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.ballisticMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MUD_SLAP] =
    {
		.name = _("Mud-Slap"),
        .effect = EFFECT_TARGET_ACCURACY_DOWN_HIT,
        .power = 20,
        .type = TYPE_GROUND,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_OCTAZOOKA] =
    {
		.name = _("Octazooka"),
        .effect = EFFECT_TARGET_ACCURACY_DOWN_HIT,
        .power = 65,
        .type = TYPE_WATER,
        .accuracy = 85,
        .pp = 10,
        .secondaryEffectChance = 50,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.ballisticMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPIKES] =
    {
		.name = _("Spikes"),
        .effect = EFFECT_SPIKES,
        .type = TYPE_GROUND,
        .pp = 20,
        .target = MOVE_TARGET_OPPONENTS_FIELD,
		.flags =
		{
			.magicCoatAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forcePressure = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_ZAP_CANNON] =
    {
		.name = _("Zap Cannon"),
        .effect = EFFECT_PARALYZE_HIT,
		.power = 120,
        .type = TYPE_ELECTRIC,
        .accuracy = 50,
        .pp = 5,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.ballisticMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FORESIGHT] =
    {
		.name = _("Foresight"),
        .effect = EFFECT_FORESIGHT,
        .type = TYPE_NORMAL,
        .pp = 40,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_BOOST_CRITS,
    },

    [MOVE_DESTINY_BOND] =
    {
		.name = _("Destiny Bond"),
        .effect = EFFECT_DESTINY_BOND,
        .type = TYPE_GHOST,
        .pp = 5,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_FOLLOW_ME,
    },

    [MOVE_PERISH_SONG] =
    {
		.name = _("Perish Song"),
        .effect = EFFECT_PERISH_SONG,
        .type = TYPE_NORMAL,
        .pp = 5,
        .target = MOVE_TARGET_ALL_BATTLERS,
        .flags =
		{
			.soundMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_ICY_WIND] =
    {
		.name = _("Icy Wind"),
        .effect = EFFECT_TARGET_SPEED_DOWN_HIT,
        .power = 55,
        .type = TYPE_ICE,
        .accuracy = 95,
        .pp = 15,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.windMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DETECT] =
    {
		.name = _("Detect"),
        .effect = EFFECT_PROTECT,
        .type = TYPE_FIGHTING,
        .pp = 5,
        .target = MOVE_TARGET_USER,
		.priority = 4,
        .flags =
		{
			.protectionMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_EVSN_UP_1,
    },

    [MOVE_BONE_RUSH] =
    {
		.name = _("Bone Rush"),
        .effect = EFFECT_MULTI_HIT,
        .power = 25,
        .type = TYPE_GROUND,
		.accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LOCK_ON] =
    {
		.name = _("Lock-On"),
        .effect = EFFECT_LOCK_ON,
        .type = TYPE_NORMAL,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_OUTRAGE] =
    {
		.name = _("Outrage"),
        .effect = EFFECT_RAMPAGE,
		.power = 120,
        .type = TYPE_DRAGON,
        .accuracy = 100,
		.pp = 10,
        .target = MOVE_TARGET_RANDOM,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenInstruct = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SANDSTORM] =
    {
		.name = _("Sandstorm"),
        .effect = EFFECT_SANDSTORM,
        .type = TYPE_ROCK,
        .pp = 10,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.windMove = TRUE, // Only set for data purpose, no one wind ability can activate due it
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_GIGA_DRAIN] =
    {
		.name = _("Giga Drain"),
        .effect = EFFECT_ABSORB,
		.power = 75,
        .type = TYPE_GRASS,
        .accuracy = 100,
		.pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.healingMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ENDURE] =
    {
		.name = _("Endure"),
        .effect = EFFECT_ENDURE,
        .type = TYPE_NORMAL,
        .pp = 10,
        .target = MOVE_TARGET_USER,
		.priority = 4,
        .flags =
		{
			.protectionMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_CHARM] =
    {
		.name = _("Charm"),
        .effect = EFFECT_ATTACK_DOWN_2,
		.type = TYPE_FAIRY,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_ROLLOUT] =
    {
		.name = _("Rollout"),
        .effect = EFFECT_ROLLOUT,
        .power = 30,
        .type = TYPE_ROCK,
        .accuracy = 90,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE,
			.noEffectiveness = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FALSE_SWIPE] =
    {
		.name = _("False Swipe"),
        .effect = EFFECT_FALSE_SWIPE,
        .power = 40,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 40,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SWAGGER] =
    {
		.name = _("Swagger"),
        .effect = EFFECT_SWAGGER,
        .type = TYPE_NORMAL,
		.accuracy = 85,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_MILK_DRINK] =
    {
		.name = _("Milk Drink"),
        .effect = EFFECT_RESTORE_HP,
        .type = TYPE_NORMAL,
        .pp = 5,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.forbiddenProtect = TRUE,
			.snatchAffected = TRUE,
			.forbiddenMirrorMove = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_SPARK] =
    {
		.name = _("Spark"),
        .effect = EFFECT_PARALYZE_HIT,
        .power = 65,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FURY_CUTTER] =
    {
		.name = _("Fury Cutter"),
        .effect = EFFECT_FURY_CUTTER,
		.power = 40,
        .type = TYPE_BUG,
        .accuracy = 95,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.slicingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STEEL_WING] =
    {
		.name = _("Steel Wing"),
        .effect = EFFECT_USER_DEFENSE_UP_HIT,
        .power = 70,
        .type = TYPE_STEEL,
        .accuracy = 90,
        .pp = 25,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MEAN_LOOK] =
    {
		.name = _("Mean Look"),
        .effect = EFFECT_MEAN_LOOK,
        .type = TYPE_NORMAL,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.magicCoatAffected = TRUE,
			.forbiddenProtect = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_ATTRACT] =
    {
		.name = _("Attract"),
        .effect = EFFECT_ATTRACT,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_SLEEP_TALK] =
    {
		.name = _("Sleep Talk"),
        .effect = EFFECT_SLEEP_TALK,
        .type = TYPE_NORMAL,
        .pp = 10,
        .target = MOVE_TARGET_DEPENDS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.callOtherMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_BOOST_CRITS,
    },

    [MOVE_HEAL_BELL] =
    {
		.name = _("Heal Bell"),
        .effect = EFFECT_HEAL_BELL,
        .type = TYPE_NORMAL,
        .pp = 5,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.snatchAffected = TRUE,
			.soundMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.affectsUserSide = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RECOVER_HP,
    },

    [MOVE_RETURN] =
    {
		.name = _("Return"),
        .effect = EFFECT_RETURN,
        .power = 1,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PRESENT] =
    {
		.name = _("Present"),
        .effect = EFFECT_PRESENT,
        .power = 1,
        .type = TYPE_NORMAL,
        .accuracy = 90,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FRUSTRATION] =
    {
		.name = _("Frustration"),
        .effect = EFFECT_FRUSTRATION,
        .power = 1,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SAFEGUARD] =
    {
		.name = _("Safeguard"),
        .effect = EFFECT_SAFEGUARD,
        .type = TYPE_NORMAL,
        .pp = 25,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.affectsUserSide = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_PAIN_SPLIT] =
    {
		.name = _("Pain Split"),
        .effect = EFFECT_PAIN_SPLIT,
        .type = TYPE_NORMAL,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_SACRED_FIRE] =
    {
		.name = _("Sacred Fire"),
        .effect = EFFECT_BURN_HIT,
        .power = 100,
        .type = TYPE_FIRE,
        .accuracy = 95,
        .pp = 5,
        .secondaryEffectChance = 50,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.thawUser = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MAGNITUDE] =
    {
		.name = _("Magnitude"),
        .effect = EFFECT_MAGNITUDE,
        .power = 1,
        .type = TYPE_GROUND,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.hitUnderground = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DYNAMIC_PUNCH] =
    {
		.name = _("Dynamic Punch"),
        .effect = EFFECT_CONFUSE_HIT,
        .power = 100,
        .type = TYPE_FIGHTING,
        .accuracy = 50,
        .pp = 5,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.punchMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MEGAHORN] =
    {
		.name = _("Megahorn"),
        .effect = EFFECT_HIT,
        .power = 120,
        .type = TYPE_BUG,
        .accuracy = 85,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DRAGON_BREATH] =
    {
		.name = _("Dragon Breath"),
        .effect = EFFECT_PARALYZE_HIT,
        .power = 60,
        .type = TYPE_DRAGON,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BATON_PASS] =
    {
		.name = _("Baton Pass"),
        .effect = EFFECT_BATON_PASS,
        .type = TYPE_NORMAL,
        .pp = 40,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_ENCORE] =
    {
		.name = _("Encore"),
        .effect = EFFECT_ENCORE,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_PURSUIT] =
    {
		.name = _("Pursuit"),
        .effect = EFFECT_PURSUIT,
        .power = 40,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_RAPID_SPIN] =
    {
		.name = _("Rapid Spin"),
        .effect = EFFECT_RAPID_SPIN,
		.power = 50,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 40,
		.secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SWEET_SCENT] =
    {
		.name = _("Sweet Scent"),
        .effect = EFFECT_EVASION_DOWN_2,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ACC_UP_1,
    },

    [MOVE_IRON_TAIL] =
    {
		.name = _("Iron Tail"),
        .effect = EFFECT_TARGET_DEFENSE_DOWN_HIT,
        .power = 100,
        .type = TYPE_STEEL,
        .accuracy = 75,
        .pp = 15,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_METAL_CLAW] =
    {
		.name = _("Metal Claw"),
        .effect = EFFECT_USER_ATTACK_UP_HIT,
        .power = 50,
        .type = TYPE_STEEL,
        .accuracy = 95,
        .pp = 35,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_VITAL_THROW] =
    {
		.name = _("Vital Throw"),
        .effect = EFFECT_HIT,
        .power = 70,
        .type = TYPE_FIGHTING,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .priority = -1,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MORNING_SUN] =
    {
		.name = _("Morning Sun"),
        .effect = EFFECT_MORNING_SUN,
        .type = TYPE_NORMAL,
        .pp = 5,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_SYNTHESIS] =
    {
		.name = _("Synthesis"),
        .effect = EFFECT_MORNING_SUN,
        .type = TYPE_GRASS,
        .pp = 5,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_MOONLIGHT] =
    {
		.name = _("Moonlight"),
        .effect = EFFECT_MORNING_SUN,
		.type = TYPE_FAIRY,
        .pp = 5,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_HIDDEN_POWER] =
    {
		.name = _("Hidden Power"),
        .effect = EFFECT_HIDDEN_POWER,
		.power = 60,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CROSS_CHOP] =
    {
		.name = _("Cross Chop"),
        .effect = EFFECT_HIT,
        .power = 100,
        .type = TYPE_FIGHTING,
        .accuracy = 80,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.highCritChance = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TWISTER] =
    {
		.name = _("Twister"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 40,
        .type = TYPE_DRAGON,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_BOTH,
		.flags =
		{
			.hitInAirDoubleDmg = TRUE,
			.windMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_RAIN_DANCE] =
    {
		.name = _("Rain Dance"),
        .effect = EFFECT_RAIN_DANCE,
        .type = TYPE_WATER,
        .pp = 5,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_SUNNY_DAY] =
    {
		.name = _("Sunny Day"),
        .effect = EFFECT_SUNNY_DAY,
        .type = TYPE_FIRE,
        .pp = 5,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_CRUNCH] =
    {
		.name = _("Crunch"),
        .effect = EFFECT_TARGET_DEFENSE_DOWN_HIT,
        .power = 80,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.bitingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MIRROR_COAT] =
    {
		.name = _("Mirror Coat"),
        .effect = EFFECT_MIRROR_COAT,
        .power = 1,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_DEPENDS,
        .priority = -5,
		.flags =
		{
			.forbiddenMirrorMove = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.noEffectiveness = TRUE,
			.forbiddenMeFirst = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PSYCH_UP] =
    {
		.name = _("Psych Up"),
        .effect = EFFECT_PSYCH_UP,
        .type = TYPE_NORMAL,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RECOVER_HP,
    },

    [MOVE_EXTREME_SPEED] =
    {
		.name = _("Extreme Speed"),
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
		.priority = 2,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ANCIENT_POWER] =
    {
		.name = _("Ancient Power"),
        .effect = EFFECT_ALL_STATS_UP_HIT,
        .power = 60,
        .type = TYPE_ROCK,
        .accuracy = 100,
        .pp = 5,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHADOW_BALL] =
    {
		.name = _("Shadow Ball"),
        .effect = EFFECT_TARGET_SP_DEFENSE_DOWN_HIT,
        .power = 80,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.ballisticMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FUTURE_SIGHT] =
    {
		.name = _("Future Sight"),
        .effect = EFFECT_FUTURE_SIGHT,
		.power = 120,
        .type = TYPE_PSYCHIC,
		.accuracy = 100,
		.pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.argument = FUTURE_ATTACK_ID_FUTURE_SIGHT,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ROCK_SMASH] =
    {
		.name = _("Rock Smash"),
        .effect = EFFECT_TARGET_DEFENSE_DOWN_HIT,
		.power = 40,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 50,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WHIRLPOOL] =
    {
		.name = _("Whirlpool"),
        .effect = EFFECT_TRAP,
		.power = 35,
        .type = TYPE_WATER,
		.accuracy = 85,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.hitUnderwater = TRUE,
		},
		.argument = TRAP_ID_WHIRLPOOL,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BEAT_UP] =
    {
		.name = _("Beat Up"),
        .effect = EFFECT_BEAT_UP,
		.power = 1,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FAKE_OUT] =
    {
		.name = _("Fake Out"),
        .effect = EFFECT_FAKE_OUT,
        .power = 40,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
		.priority = 3,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_UPROAR] =
    {
		.name = _("Uproar"),
        .effect = EFFECT_UPROAR,
		.power = 90,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_RANDOM,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.soundMove = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STOCKPILE] =
    {
		.name = _("Stockpile"),
        .effect = EFFECT_STOCKPILE,
        .type = TYPE_NORMAL,
		.pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RECOVER_HP,
    },

    [MOVE_SPIT_UP] =
    {
		.name = _("Spit Up"),
        .effect = EFFECT_SPIT_UP,
		.power = 1,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SWALLOW] =
    {
		.name = _("Swallow"),
        .effect = EFFECT_SWALLOW,
        .type = TYPE_NORMAL,
        .pp = 10,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_HEAT_WAVE] =
    {
		.name = _("Heat Wave"),
        .effect = EFFECT_BURN_HIT,
		.power = 95,
        .type = TYPE_FIRE,
        .accuracy = 90,
        .pp = 10,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.windMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HAIL] =
    {
		.name = _("Hail"),
        .effect = EFFECT_HAIL,
        .type = TYPE_ICE,
        .pp = 10,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_TORMENT] =
    {
		.name = _("Torment"),
        .effect = EFFECT_TORMENT,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_FLATTER] =
    {
		.name = _("Flatter"),
        .effect = EFFECT_FLATTER,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_WILL_O_WISP] =
    {
		.name = _("Will-O-Wisp"),
        .effect = EFFECT_WILL_O_WISP,
        .type = TYPE_FIRE,
		.accuracy = 85,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_1,
    },

    [MOVE_MEMENTO] =
    {
		.name = _("Memento"),
        .effect = EFFECT_MEMENTO,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESTORE_REPLACEMENT_HP,
    },

    [MOVE_FACADE] =
    {
		.name = _("Facade"),
        .effect = EFFECT_FACADE,
        .power = 70,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FOCUS_PUNCH] =
    {
		.name = _("Focus Punch"),
        .effect = EFFECT_FOCUS_PUNCH,
        .power = 150,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .priority = -3,
        .flags =
		{
			.makesContact = TRUE,
			.punchMove = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenMeFirst = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SMELLING_SALT] =
    {
		.name = _("Smelling Salts"),
        .effect = EFFECT_SMELLING_SALT,
		.power = 70,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FOLLOW_ME] =
    {
		.name = _("Follow Me"),
        .effect = EFFECT_FOLLOW_ME,
        .type = TYPE_NORMAL,
        .pp = 20,
        .target = MOVE_TARGET_USER,
		.priority = 2,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_NATURE_POWER] =
    {
		.name = _("Nature Power"),
        .effect = EFFECT_NATURE_POWER,
        .type = TYPE_NORMAL,
        .pp = 20,
        .target = MOVE_TARGET_DEPENDS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.callOtherMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CHARGE] =
    {
		.name = _("Charge"),
        .effect = EFFECT_CHARGE,
        .type = TYPE_ELECTRIC,
        .pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_TAUNT] =
    {
		.name = _("Taunt"),
        .effect = EFFECT_TAUNT,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_1,
    },

    [MOVE_HELPING_HAND] =
    {
		.name = _("Helping Hand"),
        .effect = EFFECT_HELPING_HAND,
        .type = TYPE_NORMAL,
        .pp = 20,
		.target = MOVE_TARGET_ALLY,
        .priority = 5,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_TRICK] =
    {
		.name = _("Trick"),
        .effect = EFFECT_TRICK,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_2,
    },

    [MOVE_ROLE_PLAY] =
    {
		.name = _("Role Play"),
        .effect = EFFECT_ROLE_PLAY,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_WISH] =
    {
		.name = _("Wish"),
        .effect = EFFECT_WISH,
        .type = TYPE_NORMAL,
        .pp = 10,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_ASSIST] =
    {
		.name = _("Assist"),
        .effect = EFFECT_ASSIST,
        .type = TYPE_NORMAL,
        .pp = 20,
        .target = MOVE_TARGET_DEPENDS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.callOtherMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_INGRAIN] =
    {
		.name = _("Ingrain"),
        .effect = EFFECT_INGRAIN,
        .type = TYPE_GRASS,
        .pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_SUPERPOWER] =
    {
		.name = _("Superpower"),
        .effect = EFFECT_SUPERPOWER,
        .power = 120,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MAGIC_COAT] =
    {
		.name = _("Magic Coat"),
        .effect = EFFECT_MAGIC_COAT,
        .type = TYPE_PSYCHIC,
        .pp = 15,
        .target = MOVE_TARGET_DEPENDS,
        .priority = 4,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_2,
    },

    [MOVE_RECYCLE] =
    {
		.name = _("Recycle"),
        .effect = EFFECT_RECYCLE,
        .type = TYPE_NORMAL,
        .pp = 10,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_2,
    },

    [MOVE_REVENGE] =
    {
		.name = _("Revenge"),
        .effect = EFFECT_REVENGE,
        .power = 60,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .priority = -4,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BRICK_BREAK] =
    {
		.name = _("Brick Break"),
        .effect = EFFECT_BRICK_BREAK,
        .power = 75,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_YAWN] =
    {
		.name = _("Yawn"),
        .effect = EFFECT_YAWN,
        .type = TYPE_NORMAL,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_KNOCK_OFF] =
    {
		.name = _("Knock Off"),
        .effect = EFFECT_KNOCK_OFF,
		.power = 65,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ENDEAVOR] =
    {
		.name = _("Endeavor"),
        .effect = EFFECT_ENDEAVOR,
        .power = 1,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenParentalBond = TRUE,
			.noEffectiveness = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ERUPTION] =
    {
		.name = _("Eruption"),
        .effect = EFFECT_ERUPTION,
        .power = 150,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SKILL_SWAP] =
    {
		.name = _("Skill Swap"),
        .effect = EFFECT_SKILL_SWAP,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_IMPRISON] =
    {
		.name = _("Imprison"),
        .effect = EFFECT_IMPRISON,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.snatchAffected = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forcePressure = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_2,
    },

    [MOVE_REFRESH] =
    {
		.name = _("Refresh"),
        .effect = EFFECT_REFRESH,
        .type = TYPE_NORMAL,
        .pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RECOVER_HP,
    },

    [MOVE_GRUDGE] =
    {
		.name = _("Grudge"),
        .effect = EFFECT_GRUDGE,
        .type = TYPE_GHOST,
        .pp = 5,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_FOLLOW_ME,
    },

    [MOVE_SNATCH] =
    {
		.name = _("Snatch"),
        .effect = EFFECT_SNATCH,
        .type = TYPE_DARK,
        .pp = 10,
        .target = MOVE_TARGET_DEPENDS,
        .priority = 4,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenInstruct = TRUE,
			.callOtherMove = TRUE,
			.forcePressure = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_2,
    },

    [MOVE_SECRET_POWER] =
    {
		.name = _("Secret Power"),
        .effect = EFFECT_SECRET_POWER,
        .power = 70,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DIVE] =
    {
		.name = _("Dive"),
        .effect = EFFECT_SEMI_INVULNERABLE,
		.power = 80,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.twoTurnsMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ARM_THRUST] =
    {
		.name = _("Arm Thrust"),
        .effect = EFFECT_MULTI_HIT,
        .power = 15,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CAMOUFLAGE] =
    {
		.name = _("Camouflage"),
        .effect = EFFECT_CAMOUFLAGE,
        .type = TYPE_NORMAL,
        .pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_EVSN_UP_1,
    },

    [MOVE_TAIL_GLOW] =
    {
		.name = _("Tail Glow"),
        .effect = EFFECT_SPECIAL_ATTACK_UP_3,
        .type = TYPE_BUG,
        .pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_LUSTER_PURGE] =
    {
		.name = _("Luster Purge"),
        .effect = EFFECT_TARGET_SP_DEFENSE_DOWN_HIT,
        .power = 95,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 5,
        .secondaryEffectChance = 50,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MIST_BALL] =
    {
		.name = _("Mist Ball"),
        .effect = EFFECT_TARGET_SP_ATTACK_DOWN_HIT,
        .power = 95,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 5,
        .secondaryEffectChance = 50,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.ballisticMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FEATHER_DANCE] =
    {
		.name = _("Feather Dance"),
        .effect = EFFECT_ATTACK_DOWN_2,
        .type = TYPE_FLYING,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.danceMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_TEETER_DANCE] =
    {
		.name = _("Teeter Dance"),
        .effect = EFFECT_CONFUSE,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_FOES_AND_ALLY,
		.flags =
		{
			.danceMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_BLAZE_KICK] =
    {
		.name = _("Blaze Kick"),
        .effect = EFFECT_BURN_HIT,
        .power = 85,
        .type = TYPE_FIRE,
        .accuracy = 90,
        .pp = 10,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.highCritChance = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MUD_SPORT] =
    {
		.name = _("Mud Sport"),
        .effect = EFFECT_MUD_SPORT,
        .type = TYPE_GROUND,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_ICE_BALL] =
    {
		.name = _("Ice Ball"),
        .effect = EFFECT_ROLLOUT,
        .power = 30,
        .type = TYPE_ICE,
        .accuracy = 90,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.ballisticMove = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE,
			.noEffectiveness = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_NEEDLE_ARM] =
    {
		.name = _("Needle Arm"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 60,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SLACK_OFF] =
    {
		.name = _("Slack Off"),
        .effect = EFFECT_RESTORE_HP,
        .type = TYPE_NORMAL,
        .pp = 5,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_HYPER_VOICE] =
    {
		.name = _("Hyper Voice"),
        .effect = EFFECT_HIT,
        .power = 90,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.soundMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_POISON_FANG] =
    {
		.name = _("Poison Fang"),
        .effect = EFFECT_POISON_FANG,
        .power = 50,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 15,
		.secondaryEffectChance = 50,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.bitingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CRUSH_CLAW] =
    {
		.name = _("Crush Claw"),
        .effect = EFFECT_TARGET_DEFENSE_DOWN_HIT,
        .power = 75,
        .type = TYPE_NORMAL,
        .accuracy = 95,
        .pp = 10,
        .secondaryEffectChance = 50,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BLAST_BURN] =
    {
		.name = _("Blast Burn"),
        .effect = EFFECT_RECHARGE,
        .power = 150,
        .type = TYPE_FIRE,
        .accuracy = 90,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenInstruct = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HYDRO_CANNON] =
    {
		.name = _("Hydro Cannon"),
        .effect = EFFECT_RECHARGE,
        .power = 150,
        .type = TYPE_WATER,
        .accuracy = 90,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenInstruct = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_METEOR_MASH] =
    {
		.name = _("Meteor Mash"),
        .effect = EFFECT_USER_ATTACK_UP_HIT,
		.power = 90,
        .type = TYPE_STEEL,
		.accuracy = 90,
        .pp = 10,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.punchMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ASTONISH] =
    {
		.name = _("Astonish"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 30,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WEATHER_BALL] =
    {
		.name = _("Weather Ball"),
        .effect = EFFECT_WEATHER_BALL,
        .power = 50,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.ballisticMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_AROMATHERAPY] =
    {
		.name = _("Aromatherapy"),
        .effect = EFFECT_HEAL_BELL,
        .type = TYPE_GRASS,
        .pp = 5,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.affectsUserSide = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RECOVER_HP,
    },

    [MOVE_FAKE_TEARS] =
    {
		.name = _("Fake Tears"),
        .effect = EFFECT_SPECIAL_DEFENSE_DOWN_2,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_AIR_CUTTER] =
    {
		.name = _("Air Cutter"),
        .effect = EFFECT_HIT,
		.power = 60,
        .type = TYPE_FLYING,
        .accuracy = 95,
        .pp = 25,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.highCritChance = TRUE,
			.slicingMove = TRUE,
			.windMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_OVERHEAT] =
    {
		.name = _("Overheat"),
        .effect = EFFECT_OVERHEAT,
		.power = 130,
        .type = TYPE_FIRE,
        .accuracy = 90,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ODOR_SLEUTH] =
    {
		.name = _("Odor Sleuth"),
        .effect = EFFECT_FORESIGHT,
        .type = TYPE_NORMAL,
        .pp = 40,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_1,
    },

    [MOVE_ROCK_TOMB] =
    {
		.name = _("Rock Tomb"),
        .effect = EFFECT_TARGET_SPEED_DOWN_HIT,
		.power = 60,
        .type = TYPE_ROCK,
		.accuracy = 95,
		.pp = 15,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SILVER_WIND] =
    {
		.name = _("Silver Wind"),
        .effect = EFFECT_ALL_STATS_UP_HIT,
        .power = 60,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 5,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_METAL_SOUND] =
    {
		.name = _("Metal Sound"),
        .effect = EFFECT_SPECIAL_DEFENSE_DOWN_2,
        .type = TYPE_STEEL,
        .accuracy = 85,
        .pp = 40,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.soundMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_GRASS_WHISTLE] =
    {
		.name = _("Grass Whistle"),
        .effect = EFFECT_SLEEP,
        .type = TYPE_GRASS,
        .accuracy = 55,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.soundMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_TICKLE] =
    {
		.name = _("Tickle"),
        .effect = EFFECT_TICKLE,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_COSMIC_POWER] =
    {
		.name = _("Cosmic Power"),
        .effect = EFFECT_COSMIC_POWER,
        .type = TYPE_PSYCHIC,
        .pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_WATER_SPOUT] =
    {
		.name = _("Water Spout"),
        .effect = EFFECT_ERUPTION,
        .power = 150,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_BOTH,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SIGNAL_BEAM] =
    {
		.name = _("Signal Beam"),
        .effect = EFFECT_CONFUSE_HIT,
        .power = 75,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHADOW_PUNCH] =
    {
		.name = _("Shadow Punch"),
        .effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_GHOST,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.punchMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_EXTRASENSORY] =
    {
		.name = _("Extrasensory"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 80,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
		.pp = 20,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SKY_UPPERCUT] =
    {
		.name = _("Sky UpperCut"),
        .effect = EFFECT_HIT,
        .power = 85,
        .type = TYPE_FIGHTING,
        .accuracy = 90,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.punchMove = TRUE,
			.hitInAir = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SAND_TOMB] =
    {
		.name = _("Sand Tomb"),
        .effect = EFFECT_TRAP,
		.power = 35,
        .type = TYPE_GROUND,
		.accuracy = 85,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.argument = TRAP_ID_SAND_TOMB,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHEER_COLD] =
    {
		.name = _("Sheer Cold"),
        .effect = EFFECT_OHKO,
        .power = 1,
        .type = TYPE_ICE,
        .accuracy = 30,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MUDDY_WATER] =
    {
		.name = _("Muddy Water"),
        .effect = EFFECT_TARGET_ACCURACY_DOWN_HIT,
		.power = 90,
        .type = TYPE_WATER,
        .accuracy = 85,
        .pp = 10,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BULLET_SEED] =
    {
		.name = _("Bullet Seed"),
        .effect = EFFECT_MULTI_HIT,
		.power = 25,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.ballisticMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_AERIAL_ACE] =
    {
		.name = _("Aerial Ace"),
        .effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_FLYING,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.slicingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ICICLE_SPEAR] =
    {
		.name = _("Icicle Spear"),
        .effect = EFFECT_MULTI_HIT,
		.power = 25,
        .type = TYPE_ICE,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_IRON_DEFENSE] =
    {
		.name = _("Iron Defense"),
        .effect = EFFECT_DEFENSE_UP_2,
        .type = TYPE_STEEL,
        .pp = 15,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_BLOCK] =
    {
		.name = _("Block"),
        .effect = EFFECT_MEAN_LOOK,
        .type = TYPE_NORMAL,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.magicCoatAffected = TRUE,
			.forbiddenProtect = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_HOWL] =
    {
		.name = _("Howl"),
        .effect = EFFECT_HOWL,
        .type = TYPE_NORMAL,
        .pp = 40,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.snatchAffected = TRUE,
			.soundMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.affectsUserSide = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_1,
    },

    [MOVE_DRAGON_CLAW] =
    {
		.name = _("Dragon Claw"),
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_DRAGON,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FRENZY_PLANT] =
    {
		.name = _("Frenzy Plant"),
        .effect = EFFECT_RECHARGE,
        .power = 150,
        .type = TYPE_GRASS,
        .accuracy = 90,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BULK_UP] =
    {
		.name = _("Bulk Up"),
        .effect = EFFECT_BULK_UP,
        .type = TYPE_FIGHTING,
        .pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_1,
    },

    [MOVE_BOUNCE] =
    {
		.name = _("Bounce"),
        .effect = EFFECT_SEMI_INVULNERABLE,
        .power = 85,
        .type = TYPE_FLYING,
        .accuracy = 85,
        .pp = 5,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.twoTurnsMove = TRUE,
			.gravityBanned = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .argument = MOVE_EFFECT_PARALYSIS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MUD_SHOT] =
    {
		.name = _("Mud Shot"),
        .effect = EFFECT_TARGET_SPEED_DOWN_HIT,
        .power = 55,
        .type = TYPE_GROUND,
        .accuracy = 95,
        .pp = 15,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_POISON_TAIL] =
    {
		.name = _("Poison Tail"),
        .effect = EFFECT_POISON_HIT,
        .power = 50,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 25,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.highCritChance = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_COVET] =
    {
		.name = _("Covet"),
        .effect = EFFECT_THIEF,
		.power = 60,
        .type = TYPE_NORMAL,
        .accuracy = 100,
		.pp = 25,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenMeFirst = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_VOLT_TACKLE] =
    {
		.name = _("Volt Tackle"),
        .effect = EFFECT_PARALYZE_HIT,
        .power = 120,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.recoilDivisor = 3, // 33%
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MAGICAL_LEAF] =
    {
		.name = _("Magical Leaf"),
        .effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_GRASS,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WATER_SPORT] =
    {
		.name = _("Water Sport"),
        .effect = EFFECT_WATER_SPORT,
        .type = TYPE_WATER,
        .pp = 15,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_CALM_MIND] =
    {
		.name = _("Calm Mind"),
        .effect = EFFECT_CALM_MIND,
        .type = TYPE_PSYCHIC,
        .pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_LEAF_BLADE] =
    {
		.name = _("Leaf Blade"),
        .effect = EFFECT_HIT,
		.power = 90,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.highCritChance = TRUE,
			.slicingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DRAGON_DANCE] =
    {
		.name = _("Dragon Dance"),
        .effect = EFFECT_DRAGON_DANCE,
        .type = TYPE_DRAGON,
        .pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.danceMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_ROCK_BLAST] =
    {
		.name = _("Rock Blast"),
        .effect = EFFECT_MULTI_HIT,
        .power = 25,
        .type = TYPE_ROCK,
		.accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.kingsRockAffected = TRUE,
			.ballisticMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHOCK_WAVE] =
    {
		.name = _("Shock Wave"),
        .effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_ELECTRIC,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WATER_PULSE] =
    {
		.name = _("Water Pulse"),
        .effect = EFFECT_CONFUSE_HIT,
        .power = 60,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.pulseMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DOOM_DESIRE] =
    {
		.name = _("Doom Desire"),
        .effect = EFFECT_FUTURE_SIGHT,
		.power = 140,
        .type = TYPE_STEEL,
		.accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.argument = FUTURE_ATTACK_ID_DOOM_DESIRE,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PSYCHO_BOOST] =
    {
		.name = _("Psycho Boost"),
        .effect = EFFECT_OVERHEAT,
        .power = 140,
        .type = TYPE_PSYCHIC,
        .accuracy = 90,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ROOST] =
    {
		.name = _("Roost"),
        .effect = EFFECT_ROOST,
        .type = TYPE_FLYING,
        .pp = 5,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_GRAVITY] =
    {
		.name = _("Gravity"),
        .effect = EFFECT_GRAVITY,
        .type = TYPE_PSYCHIC,
        .pp = 5,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_MIRACLE_EYE] =
    {
		.name = _("Miracle Eye"),
        .effect = EFFECT_MIRACLE_EYE,
        .type = TYPE_PSYCHIC,
        .pp = 40,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_WAKE_UP_SLAP] =
    {
		.name = _("Wake-Up Slap"),
        .effect = EFFECT_WAKE_UP_SLAP,
		.power = 70,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HAMMER_ARM] =
    {
		.name = _("Hammer Arm"),
        .effect = EFFECT_USER_SPEED_DOWN_HIT,
        .power = 100,
        .type = TYPE_FIGHTING,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.punchMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_GYRO_BALL] =
    {
		.name = _("Gyro Ball"),
        .effect = EFFECT_GYRO_BALL,
        .power = 1,
        .type = TYPE_STEEL,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.ballisticMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HEALING_WISH] =
    {
		.name = _("Healing Wish"),
        .effect = EFFECT_HEALING_WISH,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BRINE] =
    {
		.name = _("Brine"),
        .effect = EFFECT_BRINE,
        .power = 65,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_NATURAL_GIFT] =
    {
		.name = _("Natural Gift"),
        .effect = EFFECT_NATURAL_GIFT,
        .power = 1,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FEINT] =
    {
		.name = _("Feint"),
        .effect = EFFECT_FEINT,
		.power = 30,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .priority = 2,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PLUCK] =
    {
		.name = _("Pluck"),
        .effect = EFFECT_BUG_BITE,
        .power = 60,
        .type = TYPE_FLYING,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TAILWIND] =
    {
		.name = _("Tailwind"),
        .effect = EFFECT_TAILWIND,
        .type = TYPE_FLYING,
		.pp = 15,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.affectsUserSide = TRUE,
			.windMove = TRUE, // Only set for data purpose, Wind Power are handled separated
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_BOOST_CRITS,
    },

    [MOVE_ACUPRESSURE] =
    {
		.name = _("Acupressure"),
        .effect = EFFECT_ACUPRESSURE,
        .type = TYPE_NORMAL,
        .pp = 30,
        .target = MOVE_TARGET_USER_OR_ALLY,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_BOOST_CRITS,
    },

    [MOVE_METAL_BURST] =
    {
		.name = _("Metal Burst"),
        .effect = EFFECT_METAL_BURST,
		.power = 1,
        .type = TYPE_STEEL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_DEPENDS,
		.flags =
		{
			.noEffectiveness = TRUE,
			.forbiddenMeFirst = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_U_TURN] =
    {
		.name = _("U-Turn"),
        .effect = EFFECT_HIT_ESCAPE,
        .power = 70,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CLOSE_COMBAT] =
    {
		.name = _("Close Combat"),
        .effect = EFFECT_CLOSE_COMBAT,
        .power = 120,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PAYBACK] =
    {
		.name = _("Payback"),
        .effect = EFFECT_PAYBACK,
        .power = 50,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ASSURANCE] =
    {
		.name = _("Assurance"),
        .effect = EFFECT_ASSURANCE,
		.power = 60,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_EMBARGO] =
    {
		.name = _("Embargo"),
        .effect = EFFECT_EMBARGO,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_FLING] =
    {
		.name = _("Fling"),
        .effect = EFFECT_FLING,
        .power = 1,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 10,
        // .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PSYCHO_SHIFT] =
    {
		.name = _("Psycho Shift"),
        .effect = EFFECT_PSYCHO_SHIFT,
        .type = TYPE_PSYCHIC,
		.accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_2,
    },

    [MOVE_TRUMP_CARD] =
    {
		.name = _("Trump Card"),
        .effect = EFFECT_TRUMP_CARD,
		.power = 1,
        .type = TYPE_NORMAL,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HEAL_BLOCK] =
    {
		.name = _("Heal Block"),
        .effect = EFFECT_HEAL_BLOCK,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_OPPONENTS,
		.flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_2,
    },

    [MOVE_WRING_OUT] =
    {
		.name = _("Wring Out"),
        .effect = EFFECT_WRING_OUT,
		.power = 1,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_POWER_TRICK] =
    {
		.name = _("Power Trick"),
        .effect = EFFECT_POWER_TRICK,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_1,
    },

    [MOVE_GASTRO_ACID] =
    {
		.name = _("Gastro Acid"),
        .effect = EFFECT_GASTRO_ACID,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_LUCKY_CHANT] =
    {
		.name = _("Lucky Chant"),
        .effect = EFFECT_LUCKY_CHANT,
        .type = TYPE_NORMAL,
        .pp = 30,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.affectsUserSide = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_EVSN_UP_1,
    },

    [MOVE_ME_FIRST] =
    {
		.name = _("Me First"),
        .effect = EFFECT_ME_FIRST,
        .type = TYPE_NORMAL,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.callOtherMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_2,
    },

    [MOVE_COPYCAT] =
    {
		.name = _("Copycat"),
        .effect = EFFECT_COPYCAT,
        .type = TYPE_NORMAL,
        .pp = 20,
        .target = MOVE_TARGET_DEPENDS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.callOtherMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ACC_UP_1,
    },

    [MOVE_POWER_SWAP] =
    {
		.name = _("Power Swap"),
        .effect = EFFECT_POWER_SWAP,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_GUARD_SWAP] =
    {
		.name = _("Guard Swap"),
        .effect = EFFECT_GUARD_SWAP,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_PUNISHMENT] =
    {
		.name = _("Punishment"),
        .effect = EFFECT_PUNISHMENT,
        .power = 1,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LAST_RESORT] =
    {
		.name = _("Last Resort"),
        .effect = EFFECT_LAST_RESORT,
		.power = 140,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WORRY_SEED] =
    {
		.name = _("Worry Seed"),
        .effect = EFFECT_SET_ABILITY,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
		.argument = ABILITY_INSOMNIA,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_SUCKER_PUNCH] =
    {
		.name = _("Sucker Punch"),
        .effect = EFFECT_SUCKER_PUNCH,
		.power = 70,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .priority = 1,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TOXIC_SPIKES] =
    {
		.name = _("Toxic Spikes"),
        .effect = EFFECT_TOXIC_SPIKES,
        .type = TYPE_POISON,
        .pp = 20,
        .target = MOVE_TARGET_OPPONENTS_FIELD,
		.flags =
		{
			.magicCoatAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forcePressure = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_HEART_SWAP] =
    {
		.name = _("-"),
        .effect = EFFECT_HEART_SWAP,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_BOOST_CRITS,
    },

    [MOVE_AQUA_RING] =
    {
		.name = _("-"),
        .effect = EFFECT_AQUA_RING,
        .type = TYPE_WATER,
        .pp = 20,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_MAGNET_RISE] =
    {
		.name = _("-"),
        .effect = EFFECT_MAGNET_RISE,
        .type = TYPE_ELECTRIC,
        .pp = 10,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.gravityBanned = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_EVSN_UP_1,
    },

    [MOVE_FLARE_BLITZ] =
    {
		.name = _("-"),
        .effect = EFFECT_BURN_HIT,
        .power = 120,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.thawUser = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FORCE_PALM] =
    {
		.name = _("-"),
        .effect = EFFECT_PARALYZE_HIT,
        .power = 60,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_AURA_SPHERE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
		.power = 80,
        .type = TYPE_FIGHTING,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.pulseMove = TRUE,
			.ballisticMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ROCK_POLISH] =
    {
		.name = _("-"),
        .effect = EFFECT_SPEED_UP_2,
        .type = TYPE_ROCK,
        .pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_POISON_JAB] =
    {
		.name = _("-"),
        .effect = EFFECT_POISON_HIT,
        .power = 80,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DARK_PULSE] =
    {
		.name = _("-"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 80,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			// .secondaryEffectMove = TRUE,
			.pulseMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_NIGHT_SLASH] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 70,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.highCritChance = TRUE,
			.slicingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_AQUA_TAIL] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 90,
        .type = TYPE_WATER,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SEED_BOMB] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.ballisticMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_AIR_SLASH] =
    {
		.name = _("-"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 75,
        .type = TYPE_FLYING,
        .accuracy = 95,
		.pp = 15,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			// .secondaryEffectMove = TRUE,
			.slicingMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_X_SCISSOR] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.slicingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BUG_BUZZ] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_SP_DEFENSE_DOWN_HIT,
        .power = 90,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.soundMove = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DRAGON_PULSE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
		.power = 85,
        .type = TYPE_DRAGON,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.pulseMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DRAGON_RUSH] =
    {
		.name = _("-"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 100,
        .type = TYPE_DRAGON,
        .accuracy = 75,
        .pp = 10,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			// .secondaryEffectMove = TRUE,
			.dmgMinimize = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_POWER_GEM] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
		.power = 80,
        .type = TYPE_ROCK,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DRAIN_PUNCH] =
    {
		.name = _("-"),
        .effect = EFFECT_ABSORB,
		.power = 75,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
		.pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.punchMove = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_VACUUM_WAVE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_SELECTED,
        .priority = 1,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FOCUS_BLAST] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_SP_DEFENSE_DOWN_HIT,
        .power = 120,
        .type = TYPE_FIGHTING,
        .accuracy = 70,
        .pp = 5,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			// .secondaryEffectMove = TRUE,
			.ballisticMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ENERGY_BALL] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_SP_DEFENSE_DOWN_HIT,
		.power = 90,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.ballisticMove = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BRAVE_BIRD] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 120,
        .type = TYPE_FLYING,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.recoilDivisor = 3, // 33%
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_EARTH_POWER] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_SP_DEFENSE_DOWN_HIT,
        .power = 90,
        .type = TYPE_GROUND,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SWITCHEROO] =
    {
		.name = _("-"),
        .effect = EFFECT_TRICK,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_2,
    },

    [MOVE_GIGA_IMPACT] =
    {
		.name = _("-"),
        .effect = EFFECT_RECHARGE,
        .power = 150,
        .type = TYPE_NORMAL,
        .accuracy = 90,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenInstruct = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_NASTY_PLOT] =
    {
		.name = _("-"),
        .effect = EFFECT_SPECIAL_ATTACK_UP_2,
        .type = TYPE_DARK,
        .pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_BULLET_PUNCH] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_STEEL,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_SELECTED,
        .priority = 1,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.punchMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_AVALANCHE] =
    {
		.name = _("-"),
        .effect = EFFECT_REVENGE,
        .power = 60,
        .type = TYPE_ICE,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_BOTH,
        .priority = -4,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ICE_SHARD] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_ICE,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_SELECTED,
        .priority = 1,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHADOW_CLAW] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 70,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.highCritChance = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_THUNDER_FANG] =
    {
		.name = _("-"),
        .effect = EFFECT_FLINCH_STATUS,
        .power = 65,
        .type = TYPE_ELECTRIC,
        .accuracy = 95,
        .pp = 15,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			// .secondaryEffectMove = TRUE,
			.bitingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .argument = STATUS1_PARALYSIS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ICE_FANG] =
    {
		.name = _("-"),
        .effect = EFFECT_FLINCH_STATUS,
        .power = 65,
        .type = TYPE_ICE,
        .accuracy = 95,
        .pp = 15,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			// .secondaryEffectMove = TRUE,
			.bitingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .argument = STATUS1_FREEZE,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FIRE_FANG] =
    {
		.name = _("-"),
        .effect = EFFECT_FLINCH_STATUS,
        .power = 65,
        .type = TYPE_FIRE,
        .accuracy = 95,
        .pp = 15,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			// .secondaryEffectMove = TRUE,
			.bitingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .argument = STATUS1_BURN,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHADOW_SNEAK] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_SELECTED,
        .priority = 1,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MUD_BOMB] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_ACCURACY_DOWN_HIT,
        .power = 65,
        .type = TYPE_GROUND,
        .accuracy = 85,
        .pp = 10,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.ballisticMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PSYCHO_CUT] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 70,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.highCritChance = TRUE,
			.slicingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ZEN_HEADBUTT] =
    {
		.name = _("-"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 80,
        .type = TYPE_PSYCHIC,
        .accuracy = 90,
        .pp = 15,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MIRROR_SHOT] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_ACCURACY_DOWN_HIT,
        .power = 65,
        .type = TYPE_STEEL,
        .accuracy = 85,
        .pp = 10,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FLASH_CANNON] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_SP_DEFENSE_DOWN_HIT,
        .power = 80,
        .type = TYPE_STEEL,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ROCK_CLIMB] =
    {
		.name = _("-"),
        .effect = EFFECT_CONFUSE_HIT,
        .power = 90,
        .type = TYPE_NORMAL,
        .accuracy = 85,
        .pp = 20,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DEFOG] =
    {
		.name = _("-"),
        .effect = EFFECT_DEFOG,
        .type = TYPE_FLYING,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ACC_UP_1,
    },

    [MOVE_TRICK_ROOM] =
    {
		.name = _("-"),
        .effect = EFFECT_TRICK_ROOM,
        .type = TYPE_PSYCHIC,
        .pp = 5,
        .target = MOVE_TARGET_ALL_BATTLERS,
        .priority = -7,
		.flags =
		{
			.forbiddenProtect = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ACC_UP_1,
    },

    [MOVE_DRACO_METEOR] =
    {
		.name = _("-"),
        .effect = EFFECT_OVERHEAT,
		.power = 130,
        .type = TYPE_DRAGON,
        .accuracy = 90,
        .pp = 5,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DISCHARGE] =
    {
		.name = _("-"),
        .effect = EFFECT_PARALYZE_HIT,
        .power = 80,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LAVA_PLUME] =
    {
		.name = _("-"),
        .effect = EFFECT_BURN_HIT,
        .power = 80,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LEAF_STORM] =
    {
		.name = _("-"),
        .effect = EFFECT_OVERHEAT,
		.power = 130,
        .type = TYPE_GRASS,
        .accuracy = 90,
        .pp = 5,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_POWER_WHIP] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 120,
        .type = TYPE_GRASS,
        .accuracy = 85,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ROCK_WRECKER] =
    {
		.name = _("-"),
        .effect = EFFECT_RECHARGE,
        .power = 150,
        .type = TYPE_ROCK,
        .accuracy = 90,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.ballisticMove = TRUE,
			.forbiddenInstruct = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CROSS_POISON] =
    {
		.name = _("-"),
        .effect = EFFECT_POISON_HIT,
        .power = 70,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.highCritChance = TRUE,
			// .secondaryEffectMove = TRUE,
			.slicingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_GUNK_SHOT] =
    {
		.name = _("-"),
        .effect = EFFECT_POISON_HIT,
        .power = 120,
        .type = TYPE_POISON,
		.accuracy = 80,
        .pp = 5,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_IRON_HEAD] =
    {
		.name = _("-"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 80,
        .type = TYPE_STEEL,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MAGNET_BOMB] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_STEEL,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.ballisticMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STONE_EDGE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 100,
        .type = TYPE_ROCK,
        .accuracy = 80,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.highCritChance = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CAPTIVATE] =
    {
		.name = _("-"),
        .effect = EFFECT_CAPTIVATE,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_2,
    },

    [MOVE_STEALTH_ROCK] =
    {
		.name = _("-"),
        .effect = EFFECT_STEALTH_ROCK,
        .type = TYPE_ROCK,
        .pp = 20,
        .target = MOVE_TARGET_OPPONENTS_FIELD,
		.flags =
		{
			.magicCoatAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forcePressure = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_GRASS_KNOT] =
    {
		.name = _("-"),
        .effect = EFFECT_LOW_KICK,
        .power = 1,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CHATTER] =
    {
		.name = _("-"),
        .effect = EFFECT_CONFUSE_HIT,
		.power = 65,
        .type = TYPE_FLYING,
        .accuracy = 100,
        .pp = 20,
		.secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.soundMove = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenMimic = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenMeFirst = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_JUDGMENT] =
    {
		.name = _("-"),
        .effect = EFFECT_CHANGE_TYPE_ON_ITEM,
        .power = 100,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .argument = HOLD_EFFECT_PLATE,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BUG_BITE] =
    {
		.name = _("-"),
        .effect = EFFECT_BUG_BITE,
        .power = 60,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CHARGE_BEAM] =
    {
		.name = _("-"),
        .effect = EFFECT_SP_ATTACK_UP_HIT,
        .power = 50,
        .type = TYPE_ELECTRIC,
        .accuracy = 90,
        .pp = 10,
        .secondaryEffectChance = 70,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WOOD_HAMMER] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 120,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.recoilDivisor = 4, // 25%
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_AQUA_JET] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .priority = 1,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ATTACK_ORDER] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 90,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.highCritChance = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DEFEND_ORDER] =
    {
		.name = _("-"),
        .effect = EFFECT_COSMIC_POWER,
        .type = TYPE_BUG,
        .pp = 10,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_HEAL_ORDER] =
    {
		.name = _("-"),
        .effect = EFFECT_RESTORE_HP,
        .type = TYPE_BUG,
        .pp = 10,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_HEAD_SMASH] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 150,
        .type = TYPE_ROCK,
        .accuracy = 80,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.recoilDivisor = 2, // 50%
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DOUBLE_HIT] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 35,
        .type = TYPE_NORMAL,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.strikeCount = 2,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ROAR_OF_TIME] =
    {
		.name = _("-"),
        .effect = EFFECT_RECHARGE,
        .power = 150,
        .type = TYPE_DRAGON,
        .accuracy = 90,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenInstruct = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPACIAL_REND] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 100,
        .type = TYPE_DRAGON,
        .accuracy = 95,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.highCritChance = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LUNAR_DANCE] =
    {
		.name = _("-"),
        .effect = EFFECT_HEALING_WISH,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.danceMove = TRUE,
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CRUSH_GRIP] =
    {
		.name = _("-"),
        .effect = EFFECT_WRING_OUT,
        .power = 1,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MAGMA_STORM] =
    {
		.name = _("-"),
        .effect = EFFECT_TRAP,
		.power = 100,
        .type = TYPE_FIRE,
		.accuracy = 75,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DARK_VOID] =
    {
		.name = _("-"),
        .effect = EFFECT_DARK_VOID,
        .type = TYPE_DARK,
		.accuracy = 50,
        .pp = 10,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_SEED_FLARE] =
    {
		.name = _("-"),
        .effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT_2,
        .power = 120,
        .type = TYPE_GRASS,
        .accuracy = 85,
        .pp = 5,
        .secondaryEffectChance = 40,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_OMINOUS_WIND] =
    {
		.name = _("-"),
        .effect = EFFECT_ALL_STATS_UP_HIT,
        .power = 60,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 5,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHADOW_FORCE] =
    {
		.name = _("-"),
        .effect = EFFECT_SEMI_INVULNERABLE,
        .power = 120,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.dmgMinimize = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.twoTurnsMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
        .argument = MOVE_EFFECT_FEINT,
    },

    [MOVE_HONE_CLAWS] =
    {
		.name = _("-"),
        .effect = EFFECT_ATTACK_ACCURACY_UP,
        .type = TYPE_DARK,
        .pp = 15,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_1,
    },

    [MOVE_WIDE_GUARD] =
    {
		.name = _("-"),
        .effect = EFFECT_PROTECT,
        .type = TYPE_ROCK,
        .pp = 10,
        .target = MOVE_TARGET_USER,
        .priority = 3,
        .flags =
		{
			.protectionMove = TRUE,
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.affectsUserSide = TRUE, // Potects the whole side.
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_GUARD_SPLIT] =
    {
		.name = _("-"),
        .effect = EFFECT_GUARD_SPLIT,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_POWER_SPLIT] =
    {
		.name = _("-"),
        .effect = EFFECT_POWER_SPLIT,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_WONDER_ROOM] =
    {
		.name = _("-"),
        .effect = EFFECT_WONDER_ROOM,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_PSYSHOCK] =
    {
		.name = _("-"),
        .effect = EFFECT_PSYSHOCK,
        .power = 80,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_VENOSHOCK] =
    {
		.name = _("-"),
        .effect = EFFECT_VENOSHOCK,
        .power = 65,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_AUTOTOMIZE] =
    {
		.name = _("-"),
        .effect = EFFECT_AUTOTOMIZE,
        .type = TYPE_STEEL,
        .pp = 15,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_RAGE_POWDER] =
    {
		.name = _("-"),
        .effect = EFFECT_FOLLOW_ME,
        .type = TYPE_BUG,
        .pp = 20,
        .target = MOVE_TARGET_USER,
		.priority = 2,
        .flags =
		{
			.powderMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_TELEKINESIS] =
    {
		.name = _("-"),
        .effect = EFFECT_TELEKINESIS,
        .type = TYPE_PSYCHIC,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.gravityBanned = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_MAGIC_ROOM] =
    {
		.name = _("-"),
        .effect = EFFECT_MAGIC_ROOM,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_SMACK_DOWN] =
    {
		.name = _("-"),
        .effect = EFFECT_SMACK_DOWN,
        .power = 50,
        .type = TYPE_ROCK,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.hitInAir = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STORM_THROW] =
    {
		.name = _("-"),
        .effect = EFFECT_ALWAYS_CRIT,
		.power = 60,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FLAME_BURST] =
    {
		.name = _("-"),
        .effect = EFFECT_FLAME_BURST,
        .power = 70,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SLUDGE_WAVE] =
    {
		.name = _("-"),
        .effect = EFFECT_POISON_HIT,
        .power = 95,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_QUIVER_DANCE] =
    {
		.name = _("-"),
        .effect = EFFECT_QUIVER_DANCE,
        .type = TYPE_BUG,
        .pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.danceMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_HEAVY_SLAM] =
    {
		.name = _("-"),
        .effect = EFFECT_HEAT_CRASH,
        .power = 1,
        .type = TYPE_STEEL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.dmgMinimize = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SYNCHRONOISE] =
    {
		.name = _("-"),
        .effect = EFFECT_SYNCHRONOISE,
		.power = 120,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
		.pp = 10,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ELECTRO_BALL] =
    {
		.name = _("-"),
        .effect = EFFECT_ELECTRO_BALL,
        .power = 1,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.ballisticMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SOAK] =
    {
		.name = _("-"),
        .effect = EFFECT_SOAK,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_FLAME_CHARGE] =
    {
		.name = _("-"),
        .effect = EFFECT_SPEED_UP_HIT,
        .power = 50,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_COIL] =
    {
		.name = _("-"),
        .effect = EFFECT_COIL,
        .type = TYPE_POISON,
        .pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_LOW_SWEEP] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_SPEED_DOWN_HIT,
		.power = 65,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ACID_SPRAY] =
    {
		.name = _("-"),
        .effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT_2,
        .power = 40,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.ballisticMove = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FOUL_PLAY] =
    {
		.name = _("-"),
        .effect = EFFECT_FOUL_PLAY,
        .power = 95,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SIMPLE_BEAM] =
    {
		.name = _("-"),
        .effect = EFFECT_SIMPLE_BEAM,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_ENTRAINMENT] =
    {
		.name = _("-"),
        .effect = EFFECT_ENTRAINMENT,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_AFTER_YOU] =
    {
		.name = _("-"),
        .effect = EFFECT_AFTER_YOU,
        .type = TYPE_NORMAL,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_ROUND] =
    {
		.name = _("-"),
        .effect = EFFECT_ROUND,
        .power = 60,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.soundMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ECHOED_VOICE] =
    {
		.name = _("-"),
        .effect = EFFECT_ECHOED_VOICE,
        .power = 40,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.soundMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CHIP_AWAY] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 70,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.targetStatStagesIgnored = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CLEAR_SMOG] =
    {
		.name = _("-"),
        .effect = EFFECT_CLEAR_SMOG,
        .power = 50,
        .type = TYPE_POISON,
        .pp = 15,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STORED_POWER] =
    {
		.name = _("-"),
        .effect = EFFECT_STORED_POWER,
        .power = 20,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_QUICK_GUARD] =
    {
		.name = _("-"),
        .effect = EFFECT_PROTECT,
        .type = TYPE_FIGHTING,
        .pp = 15,
        .target = MOVE_TARGET_USER,
        .priority = 3,
        .flags =
		{
			.protectionMove = TRUE,
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.affectsUserSide = TRUE, // Protects the whole side.
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_ALLY_SWITCH] =
    {
		.name = _("-"),
        .effect = EFFECT_ALLY_SWITCH,
        .type = TYPE_PSYCHIC,
        .pp = 15,
        .target = MOVE_TARGET_USER,
		.priority = 2,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_2,
    },

    [MOVE_SCALD] =
    {
		.name = _("-"),
        .effect = EFFECT_SCALD,
        .power = 80,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.thawUser = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHELL_SMASH] =
    {
		.name = _("-"),
        .effect = EFFECT_SHELL_SMASH,
        .type = TYPE_NORMAL,
        .pp = 15,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_HEAL_PULSE] =
    {
		.name = _("-"),
        .effect = EFFECT_HEAL_PULSE,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.pulseMove = TRUE,
			.forbiddenMirrorMove = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_HEX] =
    {
		.name = _("-"),
        .effect = EFFECT_HEX,
		.power = 65,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SKY_DROP] =
    {
		.name = _("-"),
        .effect = EFFECT_SKY_DROP,
        .power = 60,
        .type = TYPE_FLYING,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE,
			.gravityBanned = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHIFT_GEAR] =
    {
		.name = _("-"),
        .effect = EFFECT_SHIFT_GEAR,
        .type = TYPE_STEEL,
        .pp = 10,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_CIRCLE_THROW] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT_SWITCH_TARGET,
        .power = 60,
        .type = TYPE_FIGHTING,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .priority = -6,
        .flags =
		{
			.makesContact = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_INCINERATE] =
    {
		.name = _("-"),
        .effect = EFFECT_INCINERATE,
		.power = 60,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_QUASH] =
    {
		.name = _("-"),
        .effect = EFFECT_QUASH,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_ACROBATICS] =
    {
		.name = _("-"),
        .effect = EFFECT_ACROBATICS,
        .power = 55,
        .type = TYPE_FLYING,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_REFLECT_TYPE] =
    {
		.name = _("-"),
        .effect = EFFECT_REFLECT_TYPE,
        .type = TYPE_NORMAL,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_RETALIATE] =
    {
		.name = _("-"),
        .effect = EFFECT_RETALIATE,
        .power = 70,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FINAL_GAMBIT] =
    {
		.name = _("-"),
        .effect = EFFECT_FINAL_GAMBIT,
        .power = 1,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BESTOW] =
    {
		.name = _("-"),
        .effect = EFFECT_BESTOW,
        .type = TYPE_NORMAL,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_2,
    },

    [MOVE_INFERNO] =
    {
		.name = _("-"),
        .effect = EFFECT_BURN_HIT,
        .power = 100,
        .type = TYPE_FIRE,
        .accuracy = 50,
        .pp = 5,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WATER_PLEDGE] =
    {
		.name = _("-"),
        .effect = EFFECT_PLEDGE,
		.power = 80,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FIRE_PLEDGE] =
    {
		.name = _("-"),
        .effect = EFFECT_PLEDGE,
		.power = 80,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_GRASS_PLEDGE] =
    {
		.name = _("-"),
        .effect = EFFECT_PLEDGE,
		.power = 80,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_VOLT_SWITCH] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT_ESCAPE,
        .power = 70,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STRUGGLE_BUG] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_SP_ATTACK_DOWN_HIT,
		.power = 50,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BULLDOZE] =
    {
		.name = _("-"),
        .effect = EFFECT_BULLDOZE,
        .power = 60,
        .type = TYPE_GROUND,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FROST_BREATH] =
    {
		.name = _("-"),
        .effect = EFFECT_ALWAYS_CRIT,
		.power = 60,
        .type = TYPE_ICE,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DRAGON_TAIL] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT_SWITCH_TARGET,
        .power = 60,
        .type = TYPE_DRAGON,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .priority = -6,
        .flags =
		{
			.makesContact = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WORK_UP] =
    {
		.name = _("-"),
        .effect = EFFECT_ATTACK_SPATK_UP,
        .type = TYPE_NORMAL,
        .pp = 30,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_1,
    },

    [MOVE_ELECTROWEB] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_SPEED_DOWN_HIT,
        .power = 55,
        .type = TYPE_ELECTRIC,
        .accuracy = 95,
        .pp = 15,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WILD_CHARGE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 90,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.recoilDivisor = 4, // 25%
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DRILL_RUN] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_GROUND,
        .accuracy = 95,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.highCritChance = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DUAL_CHOP] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_DRAGON,
        .accuracy = 90,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.strikeCount = 2,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HEART_STAMP] =
    {
		.name = _("-"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 60,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 25,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HORN_LEECH] =
    {
		.name = _("-"),
        .effect = EFFECT_ABSORB,
        .power = 75,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SACRED_SWORD] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 90,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
		.pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.targetStatStagesIgnored = TRUE,
			.slicingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_RAZOR_SHELL] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_DEFENSE_DOWN_HIT,
        .power = 75,
        .type = TYPE_WATER,
        .accuracy = 95,
        .pp = 10,
        .secondaryEffectChance = 50,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			// .secondaryEffectMove = TRUE,
			.slicingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HEAT_CRASH] =
    {
		.name = _("-"),
        .effect = EFFECT_HEAT_CRASH,
        .power = 1,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.dmgMinimize = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LEAF_TORNADO] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_ACCURACY_DOWN_HIT,
        .power = 65,
        .type = TYPE_GRASS,
        .accuracy = 90,
        .pp = 10,
        .secondaryEffectChance = 50,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STEAMROLLER] =
    {
		.name = _("-"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 65,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			// .secondaryEffectMove = TRUE,
			.dmgMinimize = TRUE,
		},
		.split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_COTTON_GUARD] =
    {
		.name = _("-"),
        .effect = EFFECT_DEFENSE_UP_3,
        .type = TYPE_GRASS,
        .pp = 10,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_NIGHT_DAZE] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_ACCURACY_DOWN_HIT,
        .power = 85,
        .type = TYPE_DARK,
        .accuracy = 95,
        .pp = 10,
        .secondaryEffectChance = 40,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PSYSTRIKE] =
    {
		.name = _("-"),
        .effect = EFFECT_PSYSHOCK,
        .power = 100,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TAIL_SLAP] =
    {
		.name = _("-"),
        .effect = EFFECT_MULTI_HIT,
        .power = 25,
        .type = TYPE_NORMAL,
        .accuracy = 85,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HURRICANE] =
    {
		.name = _("-"),
        .effect = EFFECT_HURRICANE,
		.power = 110,
        .type = TYPE_FLYING,
        .accuracy = 70,
        .pp = 10,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.hitInAir = TRUE,
			.windMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HEAD_CHARGE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 120,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.recoilDivisor = 4, // 25%
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_GEAR_GRIND] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 50,
        .type = TYPE_STEEL,
        .accuracy = 85,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.strikeCount = 2,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SEARING_SHOT] =
    {
		.name = _("-"),
        .effect = EFFECT_BURN_HIT,
        .power = 100,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 5,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.ballisticMove = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TECHNO_BLAST] =
    {
		.name = _("-"),
        .effect = EFFECT_CHANGE_TYPE_ON_ITEM,
		.power = 120,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .argument = HOLD_EFFECT_DRIVE,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_RELIC_SONG] =
    {
		.name = _("-"),
        .effect = EFFECT_RELIC_SONG,
        .power = 75,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.soundMove = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
        .argument = STATUS1_SLEEP,
    },

    [MOVE_SECRET_SWORD] =
    {
		.name = _("-"),
        .effect = EFFECT_PSYSHOCK,
        .power = 85,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
			.slicingMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_GLACIATE] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_SPEED_DOWN_HIT,
        .power = 65,
        .type = TYPE_ICE,
        .accuracy = 95,
        .pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BOLT_STRIKE] =
    {
		.name = _("-"),
        .effect = EFFECT_PARALYZE_HIT,
        .power = 130,
        .type = TYPE_ELECTRIC,
        .accuracy = 85,
        .pp = 5,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BLUE_FLARE] =
    {
		.name = _("-"),
        .effect = EFFECT_BURN_HIT,
        .power = 130,
        .type = TYPE_FIRE,
        .accuracy = 85,
        .pp = 5,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FIERY_DANCE] =
    {
		.name = _("-"),
        .effect = EFFECT_SP_ATTACK_UP_HIT,
        .power = 80,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 50,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.danceMove = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FREEZE_SHOCK] =
    {
		.name = _("-"),
        .effect = EFFECT_TWO_TURNS_ATTACK,
        .power = 140,
        .type = TYPE_ICE,
        .accuracy = 90,
        .pp = 5,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE,
			.twoTurnsMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .argument = MOVE_EFFECT_PARALYSIS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ICE_BURN] =
    {
		.name = _("-"),
        .effect = EFFECT_TWO_TURNS_ATTACK,
        .power = 140,
        .type = TYPE_ICE,
        .accuracy = 90,
        .pp = 5,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE,
			.twoTurnsMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .argument = MOVE_EFFECT_BURN,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SNARL] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_SP_ATTACK_DOWN_HIT,
        .power = 55,
        .type = TYPE_DARK,
        .accuracy = 95,
        .pp = 15,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.soundMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ICICLE_CRASH] =
    {
		.name = _("-"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 85,
        .type = TYPE_ICE,
        .accuracy = 90,
        .pp = 10,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_V_CREATE] =
    {
		.name = _("-"),
        .effect = EFFECT_V_CREATE,
        .power = 180,
        .type = TYPE_FIRE,
        .accuracy = 95,
        .pp = 5,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FUSION_FLARE] =
    {
		.name = _("-"),
        .effect = EFFECT_FUSION_COMBO,
        .power = 100,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.thawUser = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FUSION_BOLT] =
    {
		.name = _("-"),
        .effect = EFFECT_FUSION_COMBO,
        .power = 100,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FLYING_PRESS] =
    {
		.name = _("-"),
        .effect = EFFECT_TWO_TYPED_MOVE,
		.power = 100,
        .type = TYPE_FIGHTING,
        .accuracy = 95,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.dmgMinimize = TRUE,
			.gravityBanned = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .argument = TYPE_FLYING,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MAT_BLOCK] =
    {
		.name = _("-"),
        .effect = EFFECT_MAT_BLOCK,
        .type = TYPE_FIGHTING,
        .pp = 15,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_STATUS,
        .argument = TRUE, // Protects the whole side.
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_BELCH] =
    {
		.name = _("-"),
        .effect = EFFECT_BELCH,
        .power = 120,
        .type = TYPE_POISON,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenMeFirst = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ROTOTILLER] =
    {
		.name = _("-"),
        .effect = EFFECT_ROTOTILLER,
        .type = TYPE_GROUND,
        .pp = 10,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_1,
    },

    [MOVE_STICKY_WEB] =
    {
		.name = _("-"),
        .effect = EFFECT_STICKY_WEB,
        .type = TYPE_BUG,
        .pp = 20,
        .target = MOVE_TARGET_OPPONENTS_FIELD,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_FELL_STINGER] =
    {
		.name = _("-"),
        .effect = EFFECT_FELL_STINGER,
		.power = 50,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 25,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PHANTOM_FORCE] =
    {
		.name = _("-"),
        .effect = EFFECT_SEMI_INVULNERABLE,
        .power = 90,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.dmgMinimize = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.twoTurnsMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .argument = MOVE_EFFECT_FEINT,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TRICK_OR_TREAT] =
    {
		.name = _("-"),
        .effect = EFFECT_THIRD_TYPE,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_STATUS,
        .argument = TYPE_GHOST,
        .zMoveEffect = Z_EFFECT_ALL_STATS_UP_1,
    },

    [MOVE_NOBLE_ROAR] =
    {
		.name = _("-"),
        .effect = EFFECT_NOBLE_ROAR,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.soundMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_ION_DELUGE] =
    {
		.name = _("-"),
        .effect = EFFECT_ION_DELUGE,
        .type = TYPE_ELECTRIC,
        .pp = 25,
        .target = MOVE_TARGET_ALL_BATTLERS,
        .priority = 1,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_PARABOLIC_CHARGE] =
    {
		.name = _("-"),
        .effect = EFFECT_ABSORB,
		.power = 65,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FORESTS_CURSE] =
    {
		.name = _("-"),
        .effect = EFFECT_THIRD_TYPE,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_STATUS,
        .argument = TYPE_GRASS,
        .zMoveEffect = Z_EFFECT_ALL_STATS_UP_1,
    },

    [MOVE_PETAL_BLIZZARD] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 90,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.windMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FREEZE_DRY] =
    {
		.name = _("-"),
        .effect = EFFECT_FREEZE_DRY,
        .power = 70,
        .type = TYPE_ICE,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DISARMING_VOICE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_FAIRY,
        .pp = 15,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.soundMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PARTING_SHOT] =
    {
		.name = _("-"),
        .effect = EFFECT_PARTING_SHOT,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.soundMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESTORE_REPLACEMENT_HP,
    },

    [MOVE_TOPSY_TURVY] =
    {
		.name = _("-"),
        .effect = EFFECT_TOPSY_TURVY,
        .type = TYPE_DARK,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_1,
    },

    [MOVE_DRAINING_KISS] =
    {
		.name = _("-"),
        .effect = EFFECT_ABSORB,
        .power = 50,
        .type = TYPE_FAIRY,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .argument = 75, // restores 75% HP instead of 50% HP
        .zMoveEffect = Z_EFFECT_NONE,

    },

    [MOVE_CRAFTY_SHIELD] =
    {
		.name = _("-"),
        .effect = EFFECT_PROTECT,
        .type = TYPE_FAIRY,
        .pp = 10,
        .target = MOVE_TARGET_USER,
        .priority = 3,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_STATUS,
        .argument = TRUE, // Protects the whole side.
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_FLOWER_SHIELD] =
    {
		.name = _("-"),
        .effect = EFFECT_FLOWER_SHIELD,
        .type = TYPE_FAIRY,
        .pp = 10,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_GRASSY_TERRAIN] =
    {
		.name = _("-"),
        .effect = EFFECT_GRASSY_TERRAIN,
        .type = TYPE_GRASS,
        .pp = 10,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_MISTY_TERRAIN] =
    {
		.name = _("-"),
        .effect = EFFECT_MISTY_TERRAIN,
        .type = TYPE_FAIRY,
        .pp = 10,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_ELECTRIFY] =
    {
		.name = _("-"),
        .effect = EFFECT_ELECTRIFY,
        .type = TYPE_ELECTRIC,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_PLAY_ROUGH] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_ATTACK_DOWN_HIT,
        .power = 90,
        .type = TYPE_FAIRY,
        .accuracy = 90,
        .pp = 10,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FAIRY_WIND] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_FAIRY,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.windMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MOONBLAST] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_SP_ATTACK_DOWN_HIT,
        .power = 95,
        .type = TYPE_FAIRY,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BOOMBURST] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 140,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.soundMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FAIRY_LOCK] =
    {
		.name = _("-"),
        .effect = EFFECT_FAIRY_LOCK,
        .type = TYPE_FAIRY,
        .pp = 10,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_KINGS_SHIELD] =
    {
		.name = _("-"),
        .effect = EFFECT_PROTECT,
        .type = TYPE_STEEL,
        .pp = 10,
        .target = MOVE_TARGET_USER,
        .priority = 4,
        .flags =
		{
			.protectionMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenInstruct = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_PLAY_NICE] =
    {
		.name = _("-"),
        .effect = EFFECT_ATTACK_DOWN,
        .type = TYPE_NORMAL,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.forbiddenProtect = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_CONFIDE] =
    {
		.name = _("-"),
        .effect = EFFECT_SPECIAL_ATTACK_DOWN,
        .type = TYPE_NORMAL,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.soundMove = TRUE,
			.forbiddenProtect = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_DIAMOND_STORM] =
    {
		.effect = EFFECT_DEFENSE_UP2_HIT,
        .power = 100,
        .type = TYPE_ROCK,
        .accuracy = 95,
        .pp = 5,
        .secondaryEffectChance = 50,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STEAM_ERUPTION] =
    {
		.name = _("-"),
        .effect = EFFECT_SCALD,
        .power = 110,
        .type = TYPE_WATER,
        .accuracy = 95,
        .pp = 5,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.thawUser = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HYPERSPACE_HOLE] =
    {
		.name = _("-"),
        .effect = EFFECT_FEINT,
        .power = 80,
        .type = TYPE_PSYCHIC,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WATER_SHURIKEN] =
    {
		.name = _("-"),
        .effect = EFFECT_MULTI_HIT,
        .power = 15,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .priority = 1,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,

    },

    [MOVE_MYSTICAL_FIRE] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_SP_ATTACK_DOWN_HIT,
		.power = 75,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPIKY_SHIELD] =
    {
		.name = _("-"),
        .effect = EFFECT_PROTECT,
        .type = TYPE_GRASS,
        .pp = 10,
        .target = MOVE_TARGET_USER,
        .priority = 4,
        .flags =
		{
			.protectionMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_AROMATIC_MIST] =
    {
		.name = _("-"),
        .effect = EFFECT_AROMATIC_MIST,
        .type = TYPE_FAIRY,
        .pp = 20,
        .target = MOVE_TARGET_ALLY,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_2,
    },

    [MOVE_EERIE_IMPULSE] =
    {
		.name = _("-"),
        .effect = EFFECT_SPECIAL_ATTACK_DOWN_2,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_VENOM_DRENCH] =
    {
		.name = _("-"),
        .effect = EFFECT_VENOM_DRENCH,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_POWDER] =
    {
		.name = _("-"),
        .effect = EFFECT_POWDER,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .priority = 1,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.powderMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_2,
    },

    [MOVE_GEOMANCY] =
    {
		.name = _("-"),
        .effect = EFFECT_GEOMANCY,
        .type = TYPE_FAIRY,
        .pp = 10,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ALL_STATS_UP_1,
    },

    [MOVE_MAGNETIC_FLUX] =
    {
		.name = _("-"),
        .effect = EFFECT_MAGNETIC_FLUX,
        .type = TYPE_ELECTRIC,
        .pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_HAPPY_HOUR] =
    {
		.name = _("-"),
        .effect = EFFECT_DO_NOTHING,
        .type = TYPE_NORMAL,
        .pp = 30,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ALL_STATS_UP_1,
    },

    [MOVE_ELECTRIC_TERRAIN] =
    {
		.name = _("-"),
        .effect = EFFECT_ELECTRIC_TERRAIN,
        .type = TYPE_ELECTRIC,
        .pp = 10,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_DAZZLING_GLEAM] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_FAIRY,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CELEBRATE] =
    {
		.name = _("-"),
        .effect = EFFECT_DO_NOTHING,
        .type = TYPE_NORMAL,
        .pp = 40,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ALL_STATS_UP_1,
    },

    [MOVE_HOLD_HANDS] =
    {
		.name = _("-"),
        .effect = EFFECT_DO_NOTHING,
        .type = TYPE_NORMAL,
        .pp = 40,
        .target = MOVE_TARGET_ALLY,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ALL_STATS_UP_1,
    },

    [MOVE_BABY_DOLL_EYES] =
    {
		.name = _("-"),
        .effect = EFFECT_ATTACK_DOWN,
        .type = TYPE_FAIRY,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_SELECTED,
        .priority = 1,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_NUZZLE] =
    {
		.name = _("-"),
        .effect = EFFECT_PARALYZE_HIT,
        .power = 20,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HOLD_BACK] =
    {
		.name = _("-"),
        .effect = EFFECT_FALSE_SWIPE,
        .power = 40,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 40,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_INFESTATION] =
    {
		.name = _("-"),
        .effect = EFFECT_TRAP,
        .power = 20,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_POWER_UP_PUNCH] =
    {
		.name = _("-"),
        .effect = EFFECT_USER_ATTACK_UP_HIT,
        .power = 40,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.punchMove = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_OBLIVION_WING] =
    {
		.name = _("-"),
        .effect = EFFECT_ABSORB,
        .power = 80,
        .type = TYPE_FLYING,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .argument = 75, // restores 75% HP instead of 50% HP
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_THOUSAND_ARROWS] =
    {
		.name = _("-"),
        .effect = EFFECT_SMACK_DOWN,
        .power = 90,
        .type = TYPE_GROUND,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.hitInAir = TRUE,
			.makeGrounded = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_THOUSAND_WAVES] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT_PREVENT_ESCAPE,
        .power = 90,
        .type = TYPE_GROUND,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LANDS_WRATH] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 90,
        .type = TYPE_GROUND,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LIGHT_OF_RUIN] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 140,
        .type = TYPE_FAIRY,
        .accuracy = 90,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
			.recoilDivisor = 2, // 50%
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ORIGIN_PULSE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 110,
        .type = TYPE_WATER,
        .accuracy = 85,
        .pp = 10,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.pulseMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PRECIPICE_BLADES] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 120,
        .type = TYPE_GROUND,
        .accuracy = 85,
        .pp = 10,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DRAGON_ASCENT] =
    {
		.name = _("-"),
        .effect = EFFECT_CLOSE_COMBAT,
        .power = 120,
        .type = TYPE_FLYING,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HYPERSPACE_FURY] =
    {
		.name = _("-"),
        .effect = EFFECT_HYPERSPACE_FURY,
        .power = 100,
        .type = TYPE_DARK,
        .pp = 5,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.hitSubstitute = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHORE_UP] =
    {
		.name = _("-"),
        .effect = EFFECT_SHORE_UP,
        .type = TYPE_GROUND,
        .pp = 10,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_FIRST_IMPRESSION] =
    {
		.name = _("-"),
        .effect = EFFECT_FAKE_OUT,
        .power = 90,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .priority = 2,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BANEFUL_BUNKER] =
    {
		.name = _("-"),
        .effect = EFFECT_PROTECT,
        .type = TYPE_POISON,
        .pp = 10,
        .target = MOVE_TARGET_USER,
        .priority = 4,
        .flags =
		{
			.protectionMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_SPIRIT_SHACKLE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT_PREVENT_ESCAPE,
        .power = 80,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DARKEST_LARIAT] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 85,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.targetStatStagesIgnored = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPARKLING_ARIA] =
    {
		.name = _("-"),
        .effect = EFFECT_SPARKLING_ARIA,
        .power = 90,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.soundMove = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .argument = STATUS1_BURN,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ICE_HAMMER] =
    {
		.name = _("-"),
        .effect = EFFECT_USER_SPEED_DOWN_HIT,
        .power = 100,
        .type = TYPE_ICE,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.punchMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FLORAL_HEALING] =
    {
		.name = _("-"),
        .effect = EFFECT_HEAL_PULSE,
        .type = TYPE_FAIRY,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.forbiddenMirrorMove = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_HIGH_HORSEPOWER] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 95,
        .type = TYPE_GROUND,
        .accuracy = 95,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STRENGTH_SAP] =
    {
		.name = _("-"),
        .effect = EFFECT_STRENGTH_SAP,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_SOLAR_BLADE] =
    {
		.name = _("-"),
        .effect = EFFECT_SOLARBEAM,
        .power = 125,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.slicingMove = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE,
			.twoTurnsMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LEAFAGE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 40,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPOTLIGHT] =
    {
		.name = _("-"),
        .effect = EFFECT_FOLLOW_ME,
        .type = TYPE_NORMAL,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .priority = 3,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_TOXIC_THREAD] =
    {
		.name = _("-"),
        .effect = EFFECT_TOXIC_THREAD,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_LASER_FOCUS] =
    {
		.name = _("-"),
        .effect = EFFECT_LASER_FOCUS,
        .type = TYPE_NORMAL,
        .pp = 30,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_1,
    },

    [MOVE_GEAR_UP] =
    {
		.name = _("-"),
        .effect = EFFECT_GEAR_UP,
        .type = TYPE_STEEL,
        .pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_THROAT_CHOP] =
    {
		.name = _("-"),
        .effect = EFFECT_THROAT_CHOP,
        .power = 80,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_POLLEN_PUFF] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT_ENEMY_HEAL_ALLY,
        .power = 90,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.ballisticMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ANCHOR_SHOT] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT_PREVENT_ESCAPE,
        .power = 80,
        .type = TYPE_STEEL,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PSYCHIC_TERRAIN] =
    {
		.name = _("-"),
        .effect = EFFECT_PSYCHIC_TERRAIN,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_LUNGE] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_ATTACK_DOWN_HIT,
        .power = 80,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FIRE_LASH] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_DEFENSE_DOWN_HIT,
        .power = 80,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_POWER_TRIP] =
    {
		.name = _("-"),
        .effect = EFFECT_STORED_POWER,
        .power = 20,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BURN_UP] =
    {
		.name = _("-"),
        .effect = EFFECT_BURN_UP,
        .power = 130,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 5,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.thawUser = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPEED_SWAP] =
    {
		.name = _("-"),
        .effect = EFFECT_SPEED_SWAP,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_SMART_STRIKE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 70,
        .type = TYPE_STEEL,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PURIFY] =
    {
		.name = _("-"),
        .effect = EFFECT_PURIFY,
        .type = TYPE_POISON,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.forbiddenMirrorMove = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ALL_STATS_UP_1,
    },

    [MOVE_REVELATION_DANCE] =
    {
		.name = _("-"),
        .effect = EFFECT_REVELATION_DANCE,
        .power = 90,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.danceMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CORE_ENFORCER] =
    {
		.name = _("-"),
        .effect = EFFECT_CORE_ENFORCER,
        .power = 100,
        .type = TYPE_DRAGON,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TROP_KICK] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_ATTACK_DOWN_HIT,
        .power = 70,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_INSTRUCT] =
    {
		.name = _("-"),
        .effect = EFFECT_INSTRUCT,
        .type = TYPE_PSYCHIC,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenInstruct = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_BEAK_BLAST] =
    {
		.name = _("-"),
        .effect = EFFECT_BEAK_BLAST,
        .power = 100,
        .type = TYPE_FLYING,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .priority = -3,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.ballisticMove = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenMeFirst = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CLANGING_SCALES] =
    {
		.name = _("-"),
        .effect = EFFECT_ATTACKER_DEFENSE_DOWN_HIT,
        .power = 110,
        .type = TYPE_DRAGON,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.soundMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DRAGON_HAMMER] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 90,
        .type = TYPE_DRAGON,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BRUTAL_SWING] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_AURORA_VEIL] =
    {
		.name = _("-"),
        .effect = EFFECT_AURORA_VEIL,
        .type = TYPE_ICE,
        .pp = 20,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_SHELL_TRAP] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_SHELL_TRAP,
        .power = 150,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_BOTH,
        .priority = -3,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenMeFirst = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FLEUR_CANNON] =
    {
		.name = _("-"),
        .effect = EFFECT_OVERHEAT,
        .power = 130,
        .type = TYPE_FAIRY,
        .accuracy = 90,
        .pp = 5,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PSYCHIC_FANGS] =
    {
		.name = _("-"),
        .effect = EFFECT_BRICK_BREAK,
        .power = 85,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.bitingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STOMPING_TANTRUM] =
    {
		.name = _("-"),
        .effect = EFFECT_STOMPING_TANTRUM,
        .power = 75,
        .type = TYPE_GROUND,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHADOW_BONE] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_DEFENSE_DOWN_HIT,
        .power = 85,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ACCELEROCK] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_ROCK,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .priority = 1,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LIQUIDATION] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_DEFENSE_DOWN_HIT,
        .power = 85,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PRISMATIC_LASER] =
    {
		.name = _("-"),
        .effect = EFFECT_RECHARGE,
        .power = 160,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenInstruct = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPECTRAL_THIEF] =
    {
		.name = _("-"),
        .effect = EFFECT_SPECTRAL_THIEF,
        .power = 90,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SUNSTEEL_STRIKE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 100,
        .type = TYPE_STEEL,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.ignoreAbilities = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MOONGEIST_BEAM] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 100,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.ignoreAbilities = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TEARFUL_LOOK] =
    {
		.name = _("-"),
        .effect = EFFECT_NOBLE_ROAR,
        .type = TYPE_NORMAL,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.forbiddenProtect = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_ZING_ZAP] =
    {
		.name = _("-"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 80,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_NATURES_MADNESS] =
    {
		.name = _("-"),
        .effect = EFFECT_SUPER_FANG,
        .power = 1,
        .type = TYPE_FAIRY,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
			.noEffectiveness = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MULTI_ATTACK] =
    {
		.name = _("-"),
        .effect = EFFECT_CHANGE_TYPE_ON_ITEM,
		.power = 120,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .argument = HOLD_EFFECT_MEMORY,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MIND_BLOWN] =
    {
		.name = _("-"),
        .effect = EFFECT_MIND_BLOWN,
        .power = 150,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PLASMA_FISTS] =
    {
		.name = _("-"),
        .effect = EFFECT_PLASMA_FISTS,
        .power = 100,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.punchMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PHOTON_GEYSER] =
    {
		.name = _("-"),
        .effect = EFFECT_PHOTON_GEYSER,
        .power = 100,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.ignoreAbilities = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ZIPPY_ZAP] =
    {
		.effect = EFFECT_EVASION_UP_HIT,
		.power = 80,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
		.pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .priority = 2,
		.flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPLISHY_SPLASH] =
    {
		.name = _("-"),
        .effect = EFFECT_PARALYZE_HIT,
        .power = 90,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_BOTH,
		.flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FLOATY_FALL] =
    {
		.name = _("-"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 90,
        .type = TYPE_FLYING,
        .accuracy = 95,
        .pp = 15,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PIKA_PAPOW] =
    {
		.name = _("-"),
        .effect = EFFECT_RETURN,
        .power = 1,
        .type = TYPE_ELECTRIC,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BOUNCY_BUBBLE] =
    {
		.name = _("-"),
        .effect = EFFECT_ABSORB,
		.power = 60,
        .type = TYPE_WATER,
        .accuracy = 100,
		.pp = 20,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.argument = 100, // restores 100% HP instead of 50% HP
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BUZZY_BUZZ] =
    {
		.name = _("-"),
        .effect = EFFECT_PARALYZE_HIT,
		.power = 60,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
		.pp = 20,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SIZZLY_SLIDE] =
    {
		.name = _("-"),
        .effect = EFFECT_BURN_HIT,
		.power = 60,
        .type = TYPE_FIRE,
        .accuracy = 100,
		.pp = 20,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.thawUser = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_GLITZY_GLOW] =
    {
		.name = _("-"),
        .effect = EFFECT_GLITZY_GLOW,
		.power = 80,
        .type = TYPE_PSYCHIC,
		.accuracy = 95,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BADDY_BAD] =
    {
		.name = _("-"),
        .effect = EFFECT_BADDY_BAD,
		.power = 80,
        .type = TYPE_DARK,
		.accuracy = 95,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SAPPY_SEED] =
    {
		.name = _("-"),
        .effect = EFFECT_SAPPY_SEED,
		.power = 100,
        .type = TYPE_GRASS,
		.accuracy = 90,
		.pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.magicCoatAffected = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FREEZY_FROST] =
    {
		.name = _("-"),
        .effect = EFFECT_FREEZY_FROST,
		.power = 100,
        .type = TYPE_ICE,
		.accuracy = 90,
		.pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPARKLY_SWIRL] =
    {
		.name = _("-"),
        .effect = EFFECT_SPARKLY_SWIRL,
		.power = 120,
        .type = TYPE_FAIRY,
		.accuracy = 85,
		.pp = 5,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_VEEVEE_VOLLEY] =
    {
		.name = _("-"),
        .effect = EFFECT_RETURN,
        .power = 1,
        .type = TYPE_NORMAL,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DOUBLE_IRON_BASH] =
    {
		.name = _("-"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 60,
        .type = TYPE_STEEL,
        .accuracy = 100,
        .pp = 5,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			.punchMove = TRUE,
			// .secondaryEffectMove = TRUE,
			.strikeCount = 2,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DYNAMAX_CANNON] =
    {
		.name = _("-"),
        .effect = EFFECT_DYNAMAX_DOUBLE_DMG,
        .power = 100,
        .type = TYPE_DRAGON,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SNIPE_SHOT] =
    {
		.name = _("-"),
        .effect = EFFECT_SNIPE_SHOT,
        .power = 80,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.highCritChance = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_JAW_LOCK] =
    {
		.name = _("-"),
        .effect = EFFECT_JAW_LOCK,
        .power = 80,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.bitingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STUFF_CHEEKS] =
    {
		.name = _("-"),
        .effect = EFFECT_STUFF_CHEEKS,
        .type = TYPE_NORMAL,
        .pp = 10,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_NO_RETREAT] =
    {
		.name = _("-"),
        .effect = EFFECT_NO_RETREAT,
        .type = TYPE_FIGHTING,
        .pp = 5,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TAR_SHOT] =
    {
		.name = _("-"),
        .effect = EFFECT_TAR_SHOT,
        .type = TYPE_ROCK,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MAGIC_POWDER] =
    {
		.name = _("-"),
        .effect = EFFECT_SOAK,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.powderMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .argument = TYPE_PSYCHIC,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DRAGON_DARTS] =
    {
		.name = _("-"),
        .effect = EFFECT_MULTI_HIT, //TODO
        .power = 50,
        .type = TYPE_DRAGON,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.strikeCount = 2,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TEATIME] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER,   //TODO
        .type = TYPE_NORMAL,
        .pp = 10,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_OCTOLOCK] =
    {
		.name = _("-"),
        .effect = EFFECT_OCTOLOCK,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BOLT_BEAK] =
    {
		.name = _("-"),
        .effect = EFFECT_BOLT_BEAK,
        .power = 85,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FISHIOUS_REND] =
    {
		.name = _("-"),
        .effect = EFFECT_BOLT_BEAK,
        .power = 85,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.bitingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_COURT_CHANGE] =
    {
		.name = _("-"),
        .effect = EFFECT_COURT_CHANGE,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CLANGOROUS_SOUL] =
    {
		.name = _("-"),
        .effect = EFFECT_CLANGOROUS_SOUL,
        .type = TYPE_DRAGON,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.soundMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BODY_PRESS] =
    {
		.name = _("-"),
        .effect = EFFECT_BODY_PRESS,
        .power = 80,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DECORATE] =
    {
		.name = _("-"),
        .effect = EFFECT_DECORATE,
        .type = TYPE_FAIRY,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DRUM_BEATING] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_SPEED_DOWN_HIT,
        .power = 80,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SNAP_TRAP] =
    {
		.name = _("-"),
        .effect = EFFECT_TRAP,
        .power = 35,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PYRO_BALL] =
    {
		.name = _("-"),
        .effect = EFFECT_BURN_HIT,
        .power = 120,
        .type = TYPE_FIRE,
        .accuracy = 90,
        .pp = 5,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.ballisticMove = TRUE,
			// .secondaryEffectMove = TRUE,
			.thawUser = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BEHEMOTH_BLADE] =
    {
		.name = _("-"),
        .effect = EFFECT_DYNAMAX_DOUBLE_DMG,
        .power = 100,
        .type = TYPE_STEEL,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.slicingMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BEHEMOTH_BASH] =
    {
		.name = _("-"),
        .effect = EFFECT_DYNAMAX_DOUBLE_DMG,
        .power = 100,
        .type = TYPE_STEEL,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_AURA_WHEEL] =
    {
		.name = _("-"),
        .effect = EFFECT_AURA_WHEEL,
        .power = 110,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BREAKING_SWIPE] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_ATTACK_DOWN_HIT,
        .power = 60,
        .type = TYPE_DRAGON,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BRANCH_POKE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 40,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_OVERDRIVE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.soundMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_APPLE_ACID] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_SP_DEFENSE_DOWN_HIT,
        .power = 80,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_GRAV_APPLE] =
    {
		.name = _("-"),
        .effect = EFFECT_GRAV_APPLE,
        .power = 80,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPIRIT_BREAK] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_SP_ATTACK_DOWN_HIT,
        .power = 75,
        .type = TYPE_FAIRY,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STRANGE_STEAM] =
    {
		.name = _("-"),
        .effect = EFFECT_CONFUSE_HIT,
        .power = 90,
        .type = TYPE_FAIRY,
        .accuracy = 95,
        .pp = 10,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LIFE_DEW] =
    {
		.name = _("-"),
        .effect = EFFECT_JUNGLE_HEALING,
        .type = TYPE_WATER,
        .pp = 10,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_OBSTRUCT] =
    {
		.name = _("-"),
        .effect = EFFECT_PROTECT,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_USER,
        .priority = 4,
        .flags =
		{
			.protectionMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenInstruct = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FALSE_SURRENDER] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_DARK,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_METEOR_ASSAULT] =
    {
		.name = _("-"),
        .effect = EFFECT_RECHARGE,
        .power = 150,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenInstruct = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ETERNABEAM] =
    {
		.name = _("-"),
        .effect = EFFECT_RECHARGE,
        .power = 160,
        .type = TYPE_DRAGON,
        .accuracy = 90,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenInstruct = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STEEL_BEAM] =
    {
		.name = _("-"),
        .effect = EFFECT_STEEL_BEAM,
        .power = 140,
        .type = TYPE_STEEL,
        .accuracy = 95,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_EXPANDING_FORCE] =
    {
		.name = _("-"),
        .effect = EFFECT_EXPANDING_FORCE,
        .power = 80,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STEEL_ROLLER] =
    {
		.name = _("-"),
        .effect = EFFECT_REMOVE_TERRAIN,
        .power = 130,
        .type = TYPE_STEEL,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SCALE_SHOT] =
    {
		.name = _("-"),
        .effect = EFFECT_SCALE_SHOT,
        .power = 25,
        .type = TYPE_DRAGON,
        .accuracy = 90,
        .pp = 20,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_METEOR_BEAM] =
    {
		.name = _("-"),
        .effect = EFFECT_METEOR_BEAM,
        .power = 120,
        .type = TYPE_ROCK,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHELL_SIDE_ARM] =
    {
		.name = _("-"),
        .effect = EFFECT_SHELL_SIDE_ARM,
        .power = 90,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MISTY_EXPLOSION] =
    {
		.name = _("-"),
        .effect = EFFECT_EXPLOSION,
        .power = 100,
        .type = TYPE_FAIRY,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_GRASSY_GLIDE] =
    {
		.name = _("-"),
        .effect = EFFECT_GRASSY_GLIDE,
        .power = 70,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_RISING_VOLTAGE] =
    {
		.name = _("-"),
        .effect = EFFECT_RISING_VOLTAGE,
        .power = 70,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TERRAIN_PULSE] =
    {
		.name = _("-"),
        .effect = EFFECT_TERRAIN_PULSE,
        .power = 50,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.pulseMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SKITTER_SMACK] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_SP_ATTACK_DOWN_HIT,
        .power = 70,
        .type = TYPE_BUG,
        .accuracy = 90,
        .pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BURNING_JEALOUSY] =
    {
		.name = _("-"),
        .effect = EFFECT_BURN_HIT,
        .power = 70,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 5,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LASH_OUT] =
    {
		.name = _("-"),
        .effect = EFFECT_LASH_OUT,
        .power = 75,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_POLTERGEIST] =
    {
		.name = _("-"),
        .effect = EFFECT_POLTERGEIST,
        .power = 110,
        .type = TYPE_GHOST,
        .accuracy = 90,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CORROSIVE_GAS] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_CORROSIVE_GAS, TODO
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 40,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_COACHING] =
    {
		.name = _("-"),
        .effect = EFFECT_COACHING,
        .type = TYPE_FIGHTING,
        .pp = 10,
        .target = MOVE_TARGET_ALLY,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FLIP_TURN] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT_ESCAPE,
        .power = 60,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TRIPLE_AXEL] =
    {
		.name = _("-"),
        .effect = EFFECT_TRIPLE_KICK,
        .power = 20,
        .type = TYPE_ICE,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.strikeCount = 3,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DUAL_WINGBEAT] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_FLYING,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.strikeCount = 2,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SCORCHING_SANDS] =
    {
		.name = _("-"),
        .effect = EFFECT_SCALD,
        .power = 70,
        .type = TYPE_GROUND,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.thawUser = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_JUNGLE_HEALING] =
    {
		.name = _("-"),
        .effect = EFFECT_JUNGLE_HEALING,
        .type = TYPE_GRASS,
        .pp = 10,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WICKED_BLOW] =
    {
		.name = _("-"),
        .effect = EFFECT_ALWAYS_CRIT,
        .power = 80,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.punchMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SURGING_STRIKES] =
    {
		.name = _("-"),
        .effect = EFFECT_ALWAYS_CRIT,
        .power = 25,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.punchMove = TRUE,
			.strikeCount = 3,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_THUNDER_CAGE] =
    {
		.name = _("-"),
        .effect = EFFECT_TRAP,
        .power = 80,
        .type = TYPE_ELECTRIC,
        .accuracy = 90,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DRAGON_ENERGY] =
    {
		.name = _("-"),
        .effect = EFFECT_ERUPTION,
        .power = 150,
        .type = TYPE_DRAGON,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FREEZING_GLARE] =
    {
        .power = 90,
		.name = _("-"),
        .effect = EFFECT_FREEZE_HIT,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FIERY_WRATH] =
    {
		.name = _("-"),
        .effect = EFFECT_FLINCH_HIT,
        .power = 90,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 20,
        .target = MOVE_TARGET_BOTH,
		.flags =
		{
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_THUNDEROUS_KICK] =
    {
		.name = _("-"),
        .effect = EFFECT_TARGET_DEFENSE_DOWN_HIT,
        .power = 90,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_GLACIAL_LANCE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 130,
        .type = TYPE_ICE,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ASTRAL_BARRAGE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 120,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_EERIE_SPELL] =
    {
		.name = _("-"),
        .effect = EFFECT_EERIE_SPELL,
        .power = 80,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.soundMove = TRUE,
		},
		.argument = 3, // PP to deduct
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DIRE_CLAW] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_DIRE_CLAW,
        .power = 60,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PSYSHIELD_BASH] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_PSYSHIELD_BASH,
        .power = 70,
        .type = TYPE_PSYCHIC,
        .accuracy = 90,
        .pp = 10,
        .secondaryEffectChance = 50, // TODO: Adjust this value. Currently it's set to Fiery Dance's.
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_POWER_SHIFT] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_POWER_SHIFT,
        .type = TYPE_NORMAL,
        .pp = 10,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STONE_AXE] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_STONE_AXE,
        .power = 65,
        .type = TYPE_ROCK,
        .accuracy = 90,
        .pp = 15,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.slicingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPRINGTIDE_STORM] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_SPRINGTIDE_STORM,
        .power = 95,
        .type = TYPE_FAIRY,
        .accuracy = 80,
        .pp = 5,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.windMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MYSTICAL_POWER] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_MYSTICAL_POWER,
        .power = 70,
        .type = TYPE_PSYCHIC,
        .accuracy = 90,
        .pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_RAGING_FURY] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_RAGING_FURY,
        .power = 90,
        .type = TYPE_FIRE,
        .accuracy = 85,
        .pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WAVE_CRASH] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT, // TODO: Legends: Arceus mechanics.
        .power = 75,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.recoilDivisor = 3, // 33%
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CHLOROBLAST] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_CHLOROBLAST,
        .power = 120,
        .type = TYPE_GRASS,
        .accuracy = 95,
        .pp = 5,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MOUNTAIN_GALE] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_ACTION_SPEED_DOWN_HIT,
        .power = 100,
        .type = TYPE_ICE,
        .accuracy = 85,
        .pp = 5,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_VICTORY_DANCE] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_VICTORY_DANCE,
        .type = TYPE_FIGHTING,
        .pp = 20,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.danceMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HEADLONG_RUSH] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_HEADLONG_RUSH,
        .power = 100,
        .type = TYPE_GROUND,
        .accuracy = 100,
        .pp = 5,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.punchMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BARB_BARRAGE] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_BARB_BARRAGE,
        .power = 60,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ESPER_WING] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_ESPER_WING,
        .power = 75,
        .type = TYPE_PSYCHIC,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BITTER_MALICE] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_ESPER_WING,
        .power = 60,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHELTER] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_SHELTER,
        .type = TYPE_STEEL,
        .pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TRIPLE_ARROWS] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_TRIPLE_ARROWS,
        .power = 50,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 15,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_INFERNAL_PARADE] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_INFERNAL_PARADE,
        .power = 60,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CEASELESS_EDGE] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_CEASELESS_EDGE,
        .power = 65,
        .type = TYPE_DARK,
        .accuracy = 90,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.slicingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BLEAKWIND_STORM] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_BLEAKWIND_STORM,
        .power = 95,
        .type = TYPE_FLYING,
        .accuracy = 80,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.windMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WILDBOLT_STORM] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_WILDBOLT_STORM,
        .power = 95,
        .type = TYPE_ELECTRIC,
        .accuracy = 80,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.windMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SANDSEAR_STORM] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_SANDSEAR_STORM,
        .power = 95,
        .type = TYPE_GROUND,
        .accuracy = 80,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.windMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LUNAR_BLESSING] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_LUNAR_BLESSING,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TAKE_HEART] =
    {
		.name = _("-"),
        .effect = EFFECT_PLACEHOLDER, // EFFECT_TAKE_HEART,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    // Z-Moves
	// typed z-move splits are determined from base move
    [MOVE_BREAKNECK_BLITZ] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 1,
        .type = TYPE_NORMAL,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
    },
    [MOVE_ALL_OUT_PUMMELING] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 1,
        .type = TYPE_FIGHTING,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
    },
    [MOVE_SUPERSONIC_SKYSTRIKE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 1,
        .type = TYPE_FLYING,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
    },
    [MOVE_ACID_DOWNPOUR] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 1,
        .type = TYPE_POISON,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
    },
    [MOVE_TECTONIC_RAGE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 1,
        .type = TYPE_GROUND,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
    },
    [MOVE_CONTINENTAL_CRUSH] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 1,
        .type = TYPE_ROCK,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
    },
    [MOVE_SAVAGE_SPIN_OUT] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 1,
        .type = TYPE_BUG,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
    },
    [MOVE_NEVER_ENDING_NIGHTMARE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 1,
        .type = TYPE_GHOST,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
    },
    [MOVE_CORKSCREW_CRASH] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 1,
        .type = TYPE_STEEL,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
    },
    [MOVE_INFERNO_OVERDRIVE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 1,
        .type = TYPE_FIRE,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
    },
    [MOVE_HYDRO_VORTEX] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 1,
        .type = TYPE_WATER,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
    },
    [MOVE_BLOOM_DOOM] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 1,
        .type = TYPE_GRASS,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
    },
    [MOVE_GIGAVOLT_HAVOC] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 1,
        .type = TYPE_ELECTRIC,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
    },
    [MOVE_SHATTERED_PSYCHE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 1,
        .type = TYPE_PSYCHIC,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
    },
    [MOVE_SUBZERO_SLAMMER] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 1,
        .type = TYPE_ICE,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
    },
    [MOVE_DEVASTATING_DRAKE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 1,
        .type = TYPE_DRAGON,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
    },
    [MOVE_BLACK_HOLE_ECLIPSE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 1,
        .type = TYPE_DARK,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
    },
    [MOVE_TWINKLE_TACKLE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 1,
        .type = TYPE_FAIRY,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
    },
    [MOVE_CATASTROPIKA] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 210,
        .type = TYPE_ELECTRIC,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
    },
    [MOVE_10000000_VOLT_THUNDERBOLT] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 195,
        .type = TYPE_ELECTRIC,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.highCritChance = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
    },
    [MOVE_STOKED_SPARKSURFER] =
    {
		.name = _("-"),
        .effect = EFFECT_PARALYZE_HIT,
        .power = 175,
        .type = TYPE_ELECTRIC,
        .pp = 1,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
    },
    [MOVE_EXTREME_EVOBOOST] =
    {
		.name = _("-"),
        .effect = EFFECT_EXTREME_EVOBOOST,
        .type = TYPE_NORMAL,
        .pp = 1,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
    },
    [MOVE_PULVERIZING_PANCAKE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 210,
        .type = TYPE_NORMAL,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
    },
    [MOVE_GENESIS_SUPERNOVA] =
    {
		.name = _("-"),
        .effect = EFFECT_DAMAGE_SET_TERRAIN,
        .power = 185,
        .type = TYPE_PSYCHIC,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .argument = 0,  //psychic terrain
    },
    [MOVE_SINISTER_ARROW_RAID] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 180,
        .type = TYPE_GHOST,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
    },
    [MOVE_MALICIOUS_MOONSAULT] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 180,
        .type = TYPE_DARK,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
    },
    [MOVE_OCEANIC_OPERETTA] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 195,
        .type = TYPE_WATER,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
    },
    [MOVE_SPLINTERED_STORMSHARDS] =
    {
		.name = _("-"),
        .effect = EFFECT_DAMAGE_SET_TERRAIN,
        .power = 190,
        .type = TYPE_ROCK,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .argument = 1,  //remove terrain
    },
    [MOVE_LETS_SNUGGLE_FOREVER] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 190,
        .type = TYPE_FAIRY,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
    },
    [MOVE_CLANGOROUS_SOULBLAZE] =
    {
		.name = _("-"),
        .effect = EFFECT_ALL_STATS_UP_HIT,
        .power = 185,
        .type = TYPE_DRAGON,
        .pp = 1,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.soundMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
    },
    [MOVE_GUARDIAN_OF_ALOLA] =
    {
		.name = _("-"),
        .effect = EFFECT_SUPER_FANG,
        .power = 1,
        .type = TYPE_FAIRY,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.noEffectiveness = TRUE,
		},
        .split = SPLIT_SPECIAL,
    },
    [MOVE_SEARING_SUNRAZE_SMASH] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 200,
        .type = TYPE_STEEL,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
    },
    [MOVE_MENACING_MOONRAZE_MAELSTROM] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 200,
        .type = TYPE_GHOST,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
    },
    [MOVE_LIGHT_THAT_BURNS_THE_SKY] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 200,
        .type = TYPE_PSYCHIC,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
    },
    [MOVE_SOUL_STEALING_7_STAR_STRIKE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 195,
        .type = TYPE_GHOST,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
    },
};
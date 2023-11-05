const struct BattleMove gBattleMoves[MOVES_COUNT_GMAX] =
{
    [MOVE_NONE] =
    {
        .effect = EFFECT_HIT,
        .type = TYPE_NORMAL,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenMimic = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_POUND] =
    {
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
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_COMET_PUNCH] =
    {
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
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MEGA_PUNCH] =
    {
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

    [MOVE_VICE_GRIP] =
    {
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
        .effect = EFFECT_OHKO,
        .power = 1,
        .type = TYPE_NORMAL,
        .accuracy = 30,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_RAZOR_WIND] =
    {
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
        .effect = EFFECT_ATTACK_UP_2,
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
			.forbiddenParentalBond = TRUE,
			.twoTurnsMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BIND] =
    {
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
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SLAM] =
    {
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
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ROLLING_KICK] =
    {
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
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HORN_DRILL] =
    {
        .effect = EFFECT_OHKO,
        .power = 1,
        .type = TYPE_NORMAL,
        .accuracy = 30,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TACKLE] =
    {
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
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TAKE_DOWN] =
    {
        .effect = EFFECT_RECOIL_25,
        .power = 90,
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

    [MOVE_THRASH] =
    {
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
        .effect = EFFECT_DOUBLE_EDGE,
        .power = 120,
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

    [MOVE_TAIL_WHIP] =
    {
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
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PIN_MISSILE] =
    {
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
        .effect = EFFECT_FIXED_DAMAGE,
        .power = 1,
        .type = TYPE_NORMAL,
        .accuracy = 90,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.argument = 20,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DISABLE] =
    {
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
        .effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
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
        .effect = EFFECT_SPEED_DOWN_HIT,
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
        .effect = EFFECT_ATTACK_DOWN_HIT,
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
        .effect = EFFECT_RECOIL_25,
        .power = 80,
        .type = TYPE_FIGHTING,
        .accuracy = 80,
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

    [MOVE_LOW_KICK] =
    {
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
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SEISMIC_TOSS] =
    {
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
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STRENGTH] =
    {
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
        .effect = EFFECT_ABSORB,
        .power = 20,
        .type = TYPE_GRASS,
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

    [MOVE_MEGA_DRAIN] =
    {
        .effect = EFFECT_ABSORB,
        .power = 40,
        .type = TYPE_GRASS,
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

    [MOVE_LEECH_SEED] =
    {
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
        .effect = EFFECT_FIXED_DAMAGE,
        .power = 1,
        .type = TYPE_DRAGON,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.argument = 40,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FIRE_SPIN] =
    {
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
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,

    },

    [MOVE_THUNDER_SHOCK] =
    {
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
        .effect = EFFECT_OHKO,
        .power = 1,
        .type = TYPE_GROUND,
        .accuracy = 30,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DIG] =
    {
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
			.forbiddenParentalBond = TRUE,
			.twoTurnsMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TOXIC] =
    {
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
        .effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
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
        .effect = EFFECT_LEVEL_DAMAGE,
        .power = 1,
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

    [MOVE_MIMIC] =
    {
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
        .effect = EFFECT_RESTORE_HP,
        .type = TYPE_NORMAL,
		.pp = 5,
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

    [MOVE_HARDEN] =
    {
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
        .effect = EFFECT_HAZE,
        .type = TYPE_ICE,
        .pp = 30,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RECOVER_HP,
    },

    [MOVE_REFLECT] =
    {
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
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_METRONOME] =
    {
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
        .effect = EFFECT_EXPLOSION,
        .power = 200,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_EGG_BOMB] =
    {
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
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SWIFT] =
    {
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
        .effect = EFFECT_MULTI_HIT,
        .power = 20,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CONSTRICT] =
    {
        .effect = EFFECT_SPEED_DOWN_HIT,
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
        .effect = EFFECT_RESTORE_HP,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_HI_JUMP_KICK] =
    {
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
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_GLARE] =
    {
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
        .effect = EFFECT_DREAM_EATER,
        .power = 100,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_POISON_GAS] =
    {
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
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LEECH_LIFE] =
    {
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
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LOVELY_KISS] =
    {
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
        .effect = EFFECT_SPEED_DOWN_HIT,
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
        .effect = EFFECT_PSYWAVE,
        .power = 1,
        .type = TYPE_PSYCHIC,
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

    [MOVE_SPLASH] =
    {
        .effect = EFFECT_DO_NOTHING,
        .type = TYPE_NORMAL,
        .pp = 40,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_3,
    },

    [MOVE_ACID_ARMOR] =
    {
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
        .power = 100,
        .effect = EFFECT_HIT,
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
        .effect = EFFECT_EXPLOSION,
        .power = 250,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FURY_SWIPES] =
    {
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
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BONEMERANG] =
    {
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
        .effect = EFFECT_REST,
        .type = TYPE_PSYCHIC,
        .pp = 5,
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

    [MOVE_ROCK_SLIDE] =
    {
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
        .effect = EFFECT_SUPER_FANG,
        .power = 1,
        .type = TYPE_NORMAL,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SLASH] =
    {
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
        .effect = EFFECT_RECOIL_HP_25,
        .power = 50,
        .type = TYPE_NORMAL,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenMimic = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SKETCH] =
    {
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
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPIDER_WEB] =
    {
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
        .effect = EFFECT_LOCK_ON,
        .type = TYPE_NORMAL,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_NIGHTMARE] =
    {
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
        .effect = EFFECT_CURSE,
		.type = TYPE_GHOST,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
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
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_COTTON_SPORE] =
    {
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
        .effect = EFFECT_SPITE,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RECOVER_HP,
    },

    [MOVE_POWDER_SNOW] =
    {
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
        .effect = EFFECT_ACCURACY_DOWN_HIT,
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
        .effect = EFFECT_ACCURACY_DOWN_HIT,
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
        .effect = EFFECT_SPIKES,
        .type = TYPE_GROUND,
        .pp = 20,
        .target = MOVE_TARGET_OPPONENTS_FIELD,
		.flags =
		{
			.magicCoatAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_ZAP_CANNON] =
    {
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
        .effect = EFFECT_SPEED_DOWN_HIT,
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
        .effect = EFFECT_MULTI_HIT,
        .power = 25,
        .type = TYPE_GROUND,
		.accuracy = 90,
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

    [MOVE_LOCK_ON] =
    {
        .effect = EFFECT_LOCK_ON,
        .type = TYPE_NORMAL,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_OUTRAGE] =
    {
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
        .effect = EFFECT_SANDSTORM,
        .type = TYPE_ROCK,
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

    [MOVE_GIGA_DRAIN] =
    {
        .effect = EFFECT_ABSORB,
		.power = 75,
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

    [MOVE_ENDURE] =
    {
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
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FALSE_SWIPE] =
    {
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
        .effect = EFFECT_RESTORE_HP,
        .type = TYPE_NORMAL,
        .pp = 5,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_SPARK] =
    {
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
        .effect = EFFECT_DEFENSE_UP_HIT,
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
        .effect = EFFECT_PAIN_SPLIT,
        .type = TYPE_NORMAL,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_SACRED_FIRE] =
    {
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
        .effect = EFFECT_RAPID_SPIN,
		.power = 50,
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

    [MOVE_SWEET_SCENT] =
    {
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
        .effect = EFFECT_DEFENSE_DOWN_HIT,
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
        .effect = EFFECT_ATTACK_UP_HIT,
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
        .effect = EFFECT_MORNING_SUN,
        .type = TYPE_NORMAL,
        .pp = 5,
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

    [MOVE_SYNTHESIS] =
    {
        .effect = EFFECT_MORNING_SUN,
        .type = TYPE_GRASS,
        .pp = 5,
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

    [MOVE_MOONLIGHT] =
    {
        .effect = EFFECT_MORNING_SUN,
		.type = TYPE_FAIRY,
        .pp = 5,
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

    [MOVE_HIDDEN_POWER] =
    {
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
        .effect = EFFECT_DEFENSE_DOWN_HIT,
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
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PSYCH_UP] =
    {
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
        .effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
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
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ROCK_SMASH] =
    {
        .effect = EFFECT_DEFENSE_DOWN_HIT,
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
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BEAT_UP] =
    {
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
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_UPROAR] =
    {
        .effect = EFFECT_UPROAR,
		.power = 90,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 100,
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
        .effect = EFFECT_SWALLOW,
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

    [MOVE_HEAT_WAVE] =
    {
        .effect = EFFECT_BURN_HIT,
		.power = 95,
        .type = TYPE_FIRE,
        .accuracy = 90,
        .pp = 10,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			// .secondaryEffectMove = TRUE,
			.windMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HAIL] =
    {
        .effect = EFFECT_HAIL,
        .type = TYPE_ICE,
        .pp = 10,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_TORMENT] =
    {
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
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SMELLING_SALT] =
    {
        .effect = EFFECT_SMELLINGSALT,
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
        .argument = STATUS1_PARALYSIS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FOLLOW_ME] =
    {
        .effect = EFFECT_FOLLOW_ME,
        .type = TYPE_NORMAL,
        .accuracy = 100,
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
        .effect = EFFECT_CHARGE,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
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
        .effect = EFFECT_HELPING_HAND,
        .type = TYPE_NORMAL,
        .accuracy = 100,
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
        .effect = EFFECT_WISH,
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
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_ASSIST] =
    {
        .effect = EFFECT_ASSIST,
        .type = TYPE_NORMAL,
        .accuracy = 100,
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
        .effect = EFFECT_INGRAIN,
        .type = TYPE_GRASS,
        .accuracy = 100,
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
        .effect = EFFECT_MAGIC_COAT,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
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
        .effect = EFFECT_RECYCLE,
        .type = TYPE_NORMAL,
        .accuracy = 100,
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
        .effect = EFFECT_KNOCK_OFF,
		.power = 65,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 100,
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
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ERUPTION] =
    {
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
        .effect = EFFECT_SKILL_SWAP,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_IMPRISON] =
    {
        .effect = EFFECT_IMPRISON,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.snatchAffected = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPDEF_UP_2,
    },

    [MOVE_REFRESH] =
    {
        .effect = EFFECT_REFRESH,
        .type = TYPE_NORMAL,
        .accuracy = 100,
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
        .effect = EFFECT_GRUDGE,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_USER,
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_FOLLOW_ME,
    },

    [MOVE_SNATCH] =
    {
        .effect = EFFECT_SNATCH,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_DEPENDS,
        .priority = 4,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenInstruct = TRUE,
			.callOtherMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_2,
    },

    [MOVE_SECRET_POWER] =
    {
        .effect = EFFECT_SECRET_POWER,
        .power = 70,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 20,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DIVE] =
    {
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
			.forbiddenParentalBond = TRUE,
			.twoTurnsMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ARM_THRUST] =
    {
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
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CAMOUFLAGE] =
    {
        .effect = EFFECT_CAMOUFLAGE,
        .type = TYPE_NORMAL,
        .accuracy = 100,
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
        .effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
        .power = 70,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 5,
        .secondaryEffectChance = 50,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MIST_BALL] =
    {
        .effect = EFFECT_SPECIAL_ATTACK_DOWN_HIT,
        .power = 70,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 5,
        .secondaryEffectChance = 50,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.ballisticMove = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FEATHER_DANCE] =
    {
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
        .effect = EFFECT_TEETER_DANCE,
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
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MUD_SPORT] =
    {
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
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_NEEDLE_ARM] =
    {
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
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SLACK_OFF] =
    {
        .effect = EFFECT_RESTORE_HP,
        .type = TYPE_NORMAL,
        .accuracy = 100,
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

    [MOVE_HYPER_VOICE] =
    {
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
			// .secondaryEffectMove = TRUE,
			.bitingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CRUSH_CLAW] =
    {
        .effect = EFFECT_DEFENSE_DOWN_HIT,
        .power = 75,
        .type = TYPE_NORMAL,
        .accuracy = 95,
        .pp = 10,
        .secondaryEffectChance = 50,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BLAST_BURN] =
    {
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
        .effect = EFFECT_ATTACK_UP_HIT,
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
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ASTONISH] =
    {
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
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WEATHER_BALL] =
    {
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
        .effect = EFFECT_OVERHEAT,
		.power = 130,
        .type = TYPE_FIRE,
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

    [MOVE_ODOR_SLEUTH] =
    {
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
        .effect = EFFECT_SPEED_DOWN_HIT,
		.power = 60,
        .type = TYPE_ROCK,
		.accuracy = 95,
		.pp = 15,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SILVER_WIND] =
    {
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
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_METAL_SOUND] =
    {
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
        .effect = EFFECT_TICKLE,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_COSMIC_POWER] =
    {
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
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHADOW_PUNCH] =
    {
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
        .effect = EFFECT_FLINCH_HIT,
        .power = 80,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
		.pp = 20,
        .secondaryEffectChance = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SKY_UPPERCUT] =
    {
        .effect = EFFECT_SKY_UPPERCUT,
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
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHEER_COLD] =
    {
        .effect = EFFECT_OHKO,
        .power = 1,
        .type = TYPE_ICE,
        .accuracy = 30,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MUDDY_WATER] =
    {
        .effect = EFFECT_ACCURACY_DOWN_HIT,
		.power = 90,
        .type = TYPE_WATER,
        .accuracy = 85,
        .pp = 10,
        .secondaryEffectChance = 30,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BULLET_SEED] =
    {
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
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_AERIAL_ACE] =
    {
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
        .effect = EFFECT_MULTI_HIT,
		.power = 25,
        .type = TYPE_ICE,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_IRON_DEFENSE] =
    {
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
        .effect = EFFECT_ATTACK_UP,
        .type = TYPE_NORMAL,
        .pp = 40,
        .target = MOVE_TARGET_USER,
		.flags =
		{
			.snatchAffected = TRUE,
			.soundMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_1,
    },

    [MOVE_DRAGON_CLAW] =
    {
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
			// .secondaryEffectMove = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE,
			.twoTurnsMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .argument = MOVE_EFFECT_PARALYSIS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MUD_SHOT] =
    {
        .effect = EFFECT_SPEED_DOWN_HIT,
        .power = 55,
        .type = TYPE_GROUND,
        .accuracy = 95,
        .pp = 15,
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

    [MOVE_POISON_TAIL] =
    {
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
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_COVET] =
    {
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
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_VOLT_TACKLE] =
    {
        .effect = EFFECT_RECOIL_33_STATUS,
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
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.argument = STATUS1_PARALYSIS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MAGICAL_LEAF] =
    {
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
        .effect = EFFECT_WATER_SPORT,
        .type = TYPE_WATER,
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

    [MOVE_CALM_MIND] =
    {
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
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHOCK_WAVE] =
    {
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
			// .secondaryEffectMove = TRUE,
			.pulseMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DOOM_DESIRE] =
    {
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
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PSYCHO_BOOST] =
    {
        .effect = EFFECT_OVERHEAT,
        .power = 140,
        .type = TYPE_PSYCHIC,
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

    [MOVE_ROOST] =
    {
        .effect = EFFECT_ROOST,
        .type = TYPE_FLYING,
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

    [MOVE_GRAVITY] =
    {
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
        .effect = EFFECT_WAKE_UP_SLAP,
		.power = 70,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 100,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .argument = STATUS1_SLEEP,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HAMMER_ARM] =
    {
        .effect = EFFECT_HAMMER_ARM,
        .power = 100,
        .type = TYPE_FIGHTING,
        .accuracy = 90,
        .pp = 10,
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

    [MOVE_GYRO_BALL] =
    {
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
        .effect = EFFECT_HEALING_WISH,
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

    [MOVE_BRINE] =
    {
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
        .effect = EFFECT_FEINT,
		.power = 30,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .secondaryEffectChance = 100,
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
        .effect = EFFECT_TAILWIND,
        .type = TYPE_FLYING,
		.pp = 15,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_BOOST_CRITS,
    },

    [MOVE_ACUPRESSURE] =
    {
        .effect = EFFECT_ACUPRESSURE,
        .type = TYPE_NORMAL,
        .pp = 30,
        .target = MOVE_TARGET_USER | MOVE_TARGET_ALLY,
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
        .effect = EFFECT_METAL_BURST,
        .type = TYPE_STEEL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_DEPENDS,
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_U_TURN] =
    {
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
        .effect = EFFECT_CLOSE_COMBAT,
        .power = 120,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 5,
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

    [MOVE_PAYBACK] =
    {
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
        .effect = EFFECT_FLING,
        .power = 1,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 10,
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

    [MOVE_PSYCHO_SHIFT] =
    {
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
        .effect = EFFECT_TRUMP_CARD,
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
        .effect = EFFECT_HEAL_BLOCK,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_2,
    },

    [MOVE_WRING_OUT] =
    {
        .effect = EFFECT_WRING_OUT,
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
        .effect = EFFECT_LUCKY_CHANT,
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
        .zMoveEffect = Z_EFFECT_EVSN_UP_1,
    },

    [MOVE_ME_FIRST] =
    {
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
        .effect = EFFECT_POWER_SWAP,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_GUARD_SWAP] =
    {
        .effect = EFFECT_GUARD_SWAP,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_PUNISHMENT] =
    {
        .effect = EFFECT_PUNISHMENT,
        .power = 60,
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
        .effect = EFFECT_WORRY_SEED,
        .type = TYPE_GRASS,
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

    [MOVE_SUCKER_PUNCH] =
    {
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
        .effect = EFFECT_TOXIC_SPIKES,
        .type = TYPE_POISON,
        .pp = 20,
        .target = MOVE_TARGET_OPPONENTS_FIELD,
		.flags =
		{
			.magicCoatAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_HEART_SWAP] =
    {
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
        .effect = EFFECT_MAGNET_RISE,
        .type = TYPE_ELECTRIC,
        .pp = 10,
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

    [MOVE_FLARE_BLITZ] =
    {
        .effect = EFFECT_RECOIL_33_STATUS,
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
        .argument = STATUS1_BURN,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FORCE_PALM] =
    {
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
        .effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
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
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_VACUUM_WAVE] =
    {
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
        .effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
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
        .effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
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
        .effect = EFFECT_DOUBLE_EDGE,
        .power = 120,
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

    [MOVE_EARTH_POWER] =
    {
        .effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
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
        .effect = EFFECT_ACCURACY_DOWN_HIT,
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
        .effect = EFFECT_ACCURACY_DOWN_HIT,
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
        .effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
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
        .effect = EFFECT_STEALTH_ROCK,
        .type = TYPE_ROCK,
        .pp = 20,
        .target = MOVE_TARGET_OPPONENTS_FIELD,
		.flags =
		{
			.magicCoatAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_GRASS_KNOT] =
    {
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
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_JUDGMENT] =
    {
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
        .effect = EFFECT_RECOIL_25,
        .power = 120,
        .type = TYPE_GRASS,
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

    [MOVE_AQUA_JET] =
    {
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
        .effect = EFFECT_RESTORE_HP,
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
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_HEAD_SMASH] =
    {
        .effect = EFFECT_RECOIL_50,
        .power = 150,
        .type = TYPE_ROCK,
        .accuracy = 80,
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

    [MOVE_DOUBLE_HIT] =
    {
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
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CRUSH_GRIP] =
    {
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
			.forbiddenParentalBond = TRUE,
			.twoTurnsMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
        .argument = MOVE_EFFECT_FEINT,
    },

    [MOVE_HONE_CLAWS] =
    {
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
        .effect = EFFECT_TELEKINESIS,
        .type = TYPE_PSYCHIC,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPATK_UP_1,
    },

    [MOVE_MAGIC_ROOM] =
    {
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
        .effect = EFFECT_SPEED_DOWN_HIT,
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
        .effect = EFFECT_HEAL_PULSE,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.pulseMove = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_HEX] =
    {
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
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHIFT_GEAR] =
    {
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
        .effect = EFFECT_SPECIAL_ATTACK_DOWN_HIT,
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
        .effect = EFFECT_SPEED_DOWN_HIT,
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
        .effect = EFFECT_RECOIL_25,
        .power = 90,
        .type = TYPE_ELECTRIC,
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

    [MOVE_DRILL_RUN] =
    {
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
        .effect = EFFECT_ABSORB,
        .power = 75,
        .type = TYPE_GRASS,
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

    [MOVE_SACRED_SWORD] =
    {
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
        .effect = EFFECT_DEFENSE_DOWN_HIT,
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
        .effect = EFFECT_ACCURACY_DOWN_HIT,
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
        .effect = EFFECT_ACCURACY_DOWN_HIT,
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
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HURRICANE] =
    {
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
        .effect = EFFECT_RECOIL_25,
        .power = 120,
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

    [MOVE_GEAR_GRIND] =
    {
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
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_GLACIATE] =
    {
        .effect = EFFECT_SPEED_DOWN_HIT,
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
        .effect = EFFECT_SPECIAL_ATTACK_DOWN_HIT,
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
		},
        .split = SPLIT_PHYSICAL,
        .argument = TYPE_FLYING,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MAT_BLOCK] =
    {
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
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ROTOTILLER] =
    {
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
			.forbiddenParentalBond = TRUE,
			.twoTurnsMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .argument = MOVE_EFFECT_FEINT,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TRICK_OR_TREAT] =
    {
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
        .effect = EFFECT_ABSORB,
		.power = 65,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FORESTS_CURSE] =
    {
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
		},
        .split = SPLIT_SPECIAL,
        .argument = 75, // restores 75% HP instead of 50% HP
        .zMoveEffect = Z_EFFECT_NONE,

    },

    [MOVE_CRAFTY_SHIELD] =
    {
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
        .effect = EFFECT_ATTACK_DOWN_HIT,
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
        .effect = EFFECT_SPECIAL_ATTACK_DOWN_HIT,
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
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,

    },

    [MOVE_MYSTICAL_FIRE] =
    {
        .effect = EFFECT_SPECIAL_ATTACK_DOWN_HIT,
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
        .effect = EFFECT_ATTACK_UP_HIT,
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
        .effect = EFFECT_ABSORB,
        .power = 80,
        .type = TYPE_FLYING,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .argument = 75, // restores 75% HP instead of 50% HP
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_THOUSAND_ARROWS] =
    {
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
        .effect = EFFECT_RECOIL_50,
        .power = 140,
        .type = TYPE_FAIRY,
        .accuracy = 90,
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

    [MOVE_ORIGIN_PULSE] =
    {
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
        .effect = EFFECT_CLOSE_COMBAT,
        .power = 120,
        .type = TYPE_FLYING,
        .accuracy = 100,
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

    [MOVE_HYPERSPACE_FURY] =
    {
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
        .effect = EFFECT_SHORE_UP,
        .type = TYPE_GROUND,
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

    [MOVE_FIRST_IMPRESSION] =
    {
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
        .effect = EFFECT_HAMMER_ARM,
        .power = 100,
        .type = TYPE_ICE,
        .accuracy = 90,
        .pp = 10,
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

    [MOVE_FLORAL_HEALING] =
    {
        .effect = EFFECT_HEAL_PULSE,
        .type = TYPE_FAIRY,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_HIGH_HORSEPOWER] =
    {
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
        .effect = EFFECT_STRENGTH_SAP,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_SOLAR_BLADE] =
    {
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
        .effect = EFFECT_ATTACK_DOWN_HIT,
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
        .effect = EFFECT_DEFENSE_DOWN_HIT,
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
        .effect = EFFECT_SPEED_SWAP,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_SMART_STRIKE] =
    {
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
        .effect = EFFECT_PURIFY,
        .type = TYPE_POISON,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ALL_STATS_UP_1,
    },

    [MOVE_REVELATION_DANCE] =
    {
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
        .effect = EFFECT_ATTACK_DOWN_HIT,
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
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CLANGING_SCALES] =
    {
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
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FLEUR_CANNON] =
    {
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
        .effect = EFFECT_DEFENSE_DOWN_HIT,
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
        .effect = EFFECT_DEFENSE_DOWN_HIT,
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
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MULTI_ATTACK] =
    {
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
		},
        .split = SPLIT_SPECIAL,
		.argument = 100, // restores 100% HP instead of 50% HP
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BUZZY_BUZZ] =
    {
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
        .effect = EFFECT_SPEED_DOWN_HIT,
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
        .effect = EFFECT_ATTACK_DOWN_HIT,
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
        .effect = EFFECT_SPECIAL_DEFENSE_DOWN_HIT,
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
        .effect = EFFECT_SPECIAL_ATTACK_DOWN_HIT,
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
        .effect = EFFECT_EXPLOSION,
        .power = 100,
        .type = TYPE_FAIRY,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_GRASSY_GLIDE] =
    {
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
        .effect = EFFECT_SPECIAL_ATTACK_DOWN_HIT,
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
        .effect = EFFECT_DEFENSE_DOWN_HIT,
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
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DIRE_CLAW] =
    {
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
        .effect = EFFECT_DOUBLE_EDGE, // TODO: Legends: Arceus mechanics.
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
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CHLOROBLAST] =
    {
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
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BARB_BARRAGE] =
    {
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
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BLEAKWIND_STORM] =
    {
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
        .effect = EFFECT_PLACEHOLDER, // EFFECT_LUNAR_BLESSING,
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

    [MOVE_TAKE_HEART] =
    {
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
        .effect = EFFECT_SUPER_FANG,
        .power = 1,
        .type = TYPE_FAIRY,
        .pp = 1,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
    },
    [MOVE_SEARING_SUNRAZE_SMASH] =
    {
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
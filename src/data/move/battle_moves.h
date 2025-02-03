const struct BattleMove gBattleMoves[MOVES_COUNT] =
{
    [MOVE_NONE] =
    {
		.name = _("-"),
		.description = COMPOUND_STRING("A physical attack\n"
									   "delivered with a\n"
									   "long tail or a\n"
									   "foreleg, etc."),
		.animScript = gMoveAnim_NONE,
        .effect = EFFECT_HIT,
        .type = TYPE_NORMAL,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_PHYSICAL,
		.zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_POUND] =
    {
		.name = _("Pound"),
		.description = COMPOUND_STRING("The target is\n"
                                       "physically pounded\n"
                                       "with a long tail or\n"
                                       "a foreleg, etc."),
        .animScript = gMoveAnim_POUND,
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
		.description = COMPOUND_STRING("The foe is attacked\n"
                                       "with a sharp chop.\n"
                                       "Critical hits land\n"
                                       "more easily."),
		.animScript = gMoveAnim_KARATE_CHOP,
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
			.critStage = 1,
		},
        .split = SPLIT_PHYSICAL,
		.zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DOUBLE_SLAP] =
    {
		.name = _("Double Slap"),
		.description = COMPOUND_STRING("The foe is slapped\n"
                                       "repeatedly, back\n"
                                       "and forth, two to\n"
                                       "five times in a row."),
        .animScript = gMoveAnim_DOUBLE_SLAP,
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
		.description = COMPOUND_STRING("The foe is hit with\n"
                                       "a flurry of punches\n"
                                       "that strike two to\n"
                                       "five times in a row."),
        .animScript = gMoveAnim_COMET_PUNCH,
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
		.description = COMPOUND_STRING("The target is\n"
                                       "slugged by a punch\n"
                                       "thrown with\n"
                                       "muscle-packed power."),
		.animScript = gMoveAnim_MEGA_PUNCH,
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
		.description = COMPOUND_STRING("Numerous coins are\n"
                                       "hurled at the foe\n"
                                       "to inflict damage."),
		.animScript = gMoveAnim_PAY_DAY,
        .effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_PAYDAY,
			.chance = 0,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FIRE_PUNCH] =
    {
		.name = _("Fire Punch"),
		.description = COMPOUND_STRING("The foe is attacked\n"
                                       "with a fiery punch.\n"
                                       "This may also leave\n"
                                       "the foe with a burn."),
        .animScript = gMoveAnim_FIRE_PUNCH,
		.effect = EFFECT_HIT,
        .power = 75,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.punchMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_BURN,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ICE_PUNCH] =
    {
		.name = _("Ice Punch"),
		.description = COMPOUND_STRING("The target is\n"
                                       "punched with an icy\n"
                                       "fist. This may also\n"
                                       "leave it frozen."),
        .animScript = gMoveAnim_ICE_PUNCH,
		.effect = EFFECT_HIT,
        .power = 75,
        .type = TYPE_ICE,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.punchMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FREEZE,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_THUNDER_PUNCH] =
    {
		.name = _("Thunder Punch"),
		.description = COMPOUND_STRING("The foe is punched\n"
                                       "with an electrified\n"
                                       "fist. It may leave\n"
                                       "it with paralysis."),
        .animScript = gMoveAnim_THUNDER_PUNCH,
		.effect = EFFECT_HIT,
        .power = 75,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.punchMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_PARALYSIS,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SCRATCH] =
    {
		.name = _("Scratch"),
		.description = COMPOUND_STRING("Hard, pointed,\n"
                                       "sharp claws rake\n"
                                       "the target to\n"
                                       "inflict damage."),
        .animScript = gMoveAnim_SCRATCH,
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
		.description = COMPOUND_STRING("Huge, impressive\n"
                                       "pincers grip and\n"
                                       "squeeze the foe."),
        .animScript = gMoveAnim_VISE_GRIP,
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
		.description = COMPOUND_STRING("A tearing attack\n"
                                       "with big pincers.\n"
                                       "The target faints\n"
                                       "instantly if hits."),
        .animScript = gMoveAnim_GUILLOTINE,
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
		.description = COMPOUND_STRING("Blades of wind hit\n"
                                       "the foe on the 2nd\n"
                                       "turn. It has a high\n"
                                       "critical-hit ratio."),
        .animScript = gMoveAnim_RAZOR_WIND,
		.effect = EFFECT_TWO_TURNS_ATTACK,
        .power = 80,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_BOTH,
		.flags =
		{
			.kingsRockAffected = TRUE,
			.critStage = 1,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE,
		},
		.argument = {
			.twoTurns = { .stringId = B_MSG_WHIPPED_WHIRLWIND }
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SWORDS_DANCE] =
    {
		.name = _("Swords Dance"),
		.description = COMPOUND_STRING("A dance to uplift\n"
                                       "the spirit. It\n"
                                       "sharply raises the\n"
                                       "user's Attack stat."),
		.animScript = gMoveAnim_SWORDS_DANCE,
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
		.description = COMPOUND_STRING("The target is cut\n"
                                       "with a scythe, a\n"
                                       "claw, or the like\n"
                                       "to inflict damage."),
        .animScript = gMoveAnim_CUT,
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
		.description = COMPOUND_STRING("A gust is whipped\n"
                                       "up by wings and\n"
                                       "launched at the foe\n"
                                       "to inflict damage."),
        .animScript = gMoveAnim_GUST,
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
		.description = COMPOUND_STRING("The foe is struck\n"
                                       "with large wings\n"
                                       "spread wide to\n"
                                       "inflict damage."),
        .animScript = gMoveAnim_WING_ATTACK,
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
		.description = COMPOUND_STRING("The target is blown\n"
                                       "away, and a\n"
                                       "different Pokémon\n"
                                       "is dragged out."),
        .animScript = gMoveAnim_WHIRLWIND,
		.effect = EFFECT_RANDOM_SWITCH,
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
		.description = COMPOUND_STRING("The user flies up\n"
                                       "into the sky and\n"
                                       "then attacks on the\n"
                                       "next turn."),
        .animScript = gMoveAnim_FLY,
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
			.gravityBanned = TRUE,
		},
		.argument = {
			.semiInvulnerable = {
				.stringId = B_MSG_FLEW_UP_HIGH,
				.status = COMPRESS_BITS(STATUS3_ON_AIR),
			},
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BIND] =
    {
		.name = _("Bind"),
		.description = COMPOUND_STRING("A body or tentacles\n"
                                       "are used to squeeze\n"
                                       "the foe for four to\n"
                                       "five turns."),
        .animScript = gMoveAnim_BIND,
		.effect = EFFECT_HIT,
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
		.argument = {
			.bind = { .trappingId = TRAP_ID_BIND },
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_WRAP,
			.chance = 0,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SLAM] =
    {
		.name = _("Slam"),
		.description = COMPOUND_STRING("The target is\n"
                                       "slammed with a long\n"
                                       "tail, vines, etc.,\n"
                                       "to inflict damage."),
        .animScript = gMoveAnim_SLAM,
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
		.description = COMPOUND_STRING("The foe is struck\n"
                                       "with slender,\n"
                                       "whiplike vines to\n"
                                       "inflict damage."),
        .animScript = gMoveAnim_VINE_WHIP,
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
		.description = COMPOUND_STRING("The foe is stomped\n"
                                       "with a big foot. It\n"
                                       "may also make the\n"
                                       "target flinch."),
        .animScript = gMoveAnim_STOMP,
		.effect = EFFECT_HIT,
        .power = 65,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.dmgMinimize = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DOUBLE_KICK] =
    {
		.name = _("Double Kick"),
		.description = COMPOUND_STRING("The target is\n"
                                       "quickly kicked\n"
                                       "twice in succession\n"
                                       "using both feet."),
        .animScript = gMoveAnim_DOUBLE_KICK,
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
		.description = COMPOUND_STRING("The target is\n"
                                       "attacked by a kick\n"
                                       "launched with\n"
                                       "muscle-packed power."),
        .animScript = gMoveAnim_MEGA_KICK,
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
		.description = COMPOUND_STRING("The user jumps up\n"
                                       "high, then kicks.\n"
                                       "If it misses, the\n"
                                       "user hurts itself."),
        .animScript = gMoveAnim_JUMP_KICK,
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
		.description = COMPOUND_STRING("The user lashes out\n"
                                       "with a spinning\n"
                                       "kick. It may also\n"
                                       "make the foe flinch."),
        .animScript = gMoveAnim_ROLLING_KICK,
		.effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_FIGHTING,
        .accuracy = 85,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SAND_ATTACK] =
    {
		.name = _("Sand Attack"),
		.description = COMPOUND_STRING("Sand is hurled in\n"
                                       "the target's face,\n"
                                       "lowering the\n"
                                       "target's accuracy."),
        .animScript = gMoveAnim_SAND_ATTACK,
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
		.description = COMPOUND_STRING("The user sticks its\n"
                                       "head out and rams.\n"
                                       "It may make the foe\n"
                                       "flinch."),
        .animScript = gMoveAnim_HEADBUTT,
		.effect = EFFECT_HIT,
        .power = 70,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HORN_ATTACK] =
    {
		.name = _("Horn Attack"),
		.description = COMPOUND_STRING("The foe is jabbed\n"
                                       "with a sharply\n"
                                       "pointed horn to\n"
                                       "inflict damage."),
        .animScript = gMoveAnim_HORN_ATTACK,
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
		.description = COMPOUND_STRING("The foe is jabbed\n"
                                       "repeatedly with a\n"
                                       "horn or beak two to\n"
                                       "five times in a row."),
        .animScript = gMoveAnim_FURY_ATTACK,
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
		.description = COMPOUND_STRING("The horn is rotated\n"
                                       "like a drill to\n"
                                       "ram. The foe will\n"
                                       "faint if it hits."),
        .animScript = gMoveAnim_HORN_DRILL,
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
		.description = COMPOUND_STRING("A physical attack\n"
                                       "in which the user\n"
                                       "charges, full body,\n"
                                       "into the foe."),
        .animScript = gMoveAnim_TACKLE,
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
		.description = COMPOUND_STRING("It drops onto the\n"
                                       "foe with its full\n"
                                       "body. It may leave\n"
                                       "the foe paralyzed."),
        .animScript = gMoveAnim_BODY_SLAM,
		.effect = EFFECT_HIT,
        .power = 85,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			.dmgMinimize = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_PARALYSIS,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WRAP] =
    {
		.name = _("Wrap"),
		.description = COMPOUND_STRING("A long body or\n"
                                       "vines are used to\n"
                                       "wrap the foe for\n"
                                       "four to five turns."),
        .animScript = gMoveAnim_WRAP,
		.effect = EFFECT_HIT,
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
		.argument = {
			.bind = { .trappingId = TRAP_ID_WRAP },
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_WRAP,
			.chance = 0,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TAKE_DOWN] =
    {
		.name = _("Take Down"),
		.description = COMPOUND_STRING("A reckless,\n"
                                       "full-body attack\n"
                                       "that also hurts the\n"
                                       "user a little."),
        .animScript = gMoveAnim_TAKE_DOWN,
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
		.description = COMPOUND_STRING("The user rampages\n"
                                       "about for two to\n"
                                       "three turns, then\n"
                                       "becomes confused."),
        .animScript = gMoveAnim_THRASH,
		.effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_THRASH,
			.chance = 0,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DOUBLE_EDGE] =
    {
		.name = _("Double-Edge"),
		.description = COMPOUND_STRING("A reckless,\n"
                                       "life-risking tackle\n"
                                       "that also hurts the\n"
                                       "user quite a lot."),
        .animScript = gMoveAnim_DOUBLE_EDGE,
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
		.description = COMPOUND_STRING("The user wags its\n"
                                       "tail cutely, making\n"
                                       "the foe lower its\n"
                                       "Defense stat."),
        .animScript = gMoveAnim_TAIL_WHIP,
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
		.description = COMPOUND_STRING("The user stabs the\n"
                                       "target with a toxic\n"
                                       "barb. This may also\n"
                                       "poison the target."),
        .animScript = gMoveAnim_POISON_STING,
		.effect = EFFECT_HIT,
        .power = 15,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 35,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_POISON,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TWINEEDLE] =
    {
		.name = _("Twineedle"),
		.description = COMPOUND_STRING("The foe is stabbed\n"
                                       "twice by a pair of\n"
                                       "stingers. It may\n"
                                       "poison the target."),
        .animScript = gMoveAnim_TWINEEDLE,
		.effect = EFFECT_HIT,
        .power = 25,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.strikeCount = 2,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_POISON,
			.chance = 20,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PIN_MISSILE] =
    {
		.name = _("Pin Missile"),
		.description = COMPOUND_STRING("Sharp pins are shot\n"
                                       "at the foe and hit\n"
                                       "two to five times\n"
                                       "at once."),
        .animScript = gMoveAnim_PIN_MISSILE,
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
		.description = COMPOUND_STRING("The foes are given\n"
                                       "an intimidating\n"
                                       "leer that lowers\n"
                                       "their Defense stats."),
        .animScript = gMoveAnim_LEER,
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
		.description = COMPOUND_STRING("The target is\n"
                                       "bitten with sharp\n"
                                       "fangs. It may make\n"
                                       "the target flinch."),
        .animScript = gMoveAnim_BITE,
		.effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 25,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.bitingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_GROWL] =
    {
		.name = _("Growl"),
		.description = COMPOUND_STRING("The user growls in\n"
                                       "a cute way, making\n"
                                       "the foe lower its\n"
                                       "Attack stat."),
        .animScript = gMoveAnim_GROWL,
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
		.description = COMPOUND_STRING("The target is\n"
                                       "scared off, and a\n"
                                       "different Pokémon\n"
                                       "is dragged out."),
        .animScript = gMoveAnim_ROAR,
		.effect = EFFECT_RANDOM_SWITCH,
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
		.description = COMPOUND_STRING("A soothing song in\n"
                                       "a calming voice\n"
                                       "lulls the foe into\n"
                                       "a deep slumber."),
        .animScript = gMoveAnim_SING,
		.effect = EFFECT_SLEEP,
        .type = TYPE_NORMAL,
        .accuracy = 55,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.soundMove = TRUE,
			.hasQuietBGM = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_SUPERSONIC] =
    {
		.name = _("Supersonic"),
		.description = COMPOUND_STRING("The user generates\n"
                                       "odd sound waves\n"
                                       "from its body that\n"
                                       "confuse the target."),
        .animScript = gMoveAnim_SUPERSONIC,
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
		.description = COMPOUND_STRING("The foe is hit with\n"
                                       "a shock wave that\n"
                                       "always inflicts 20\n"
                                       "HP damage."),
        .animScript = gMoveAnim_SONIC_BOOM,
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
		.argument = {
			.fixedDamage = { .amount = 20 },
		},
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DISABLE] =
    {
		.name = _("Disable"),
		.description = COMPOUND_STRING("For four turns, the\n"
                                       "foe will be unable\n"
                                       "to use whichever\n"
                                       "move it last used."),
        .animScript = gMoveAnim_DISABLE,
		.effect = EFFECT_DISABLE_MOVE,
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
		.description = COMPOUND_STRING("Foes are sprayed\n"
                                       "with harsh acid\n"
                                       "that may lower it's\n"
                                       "Sp. Defense stat."),
        .animScript = gMoveAnim_ACID,
		.effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_BOTH,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_DEF_MINUS_1,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_EMBER] =
    {
		.name = _("Ember"),
		.description = COMPOUND_STRING("The foe is attacked\n"
                                       "with small flames.\n"
                                       "This may also leave\n"
                                       "it with a burn."),
        .animScript = gMoveAnim_EMBER,
		.effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 25,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_BURN,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FLAMETHROWER] =
    {
		.name = _("Flamethrower"),
		.description = COMPOUND_STRING("The foe is scorched\n"
                                       "with intense\n"
                                       "flames. The foe may\n"
                                       "suffer a burn."),
        .animScript = gMoveAnim_FLAMETHROWER,
		.effect = EFFECT_HIT,
		.power = 90,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_BURN,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MIST] =
    {
		.name = _("Mist"),
		.description = COMPOUND_STRING("The user's side is\n"
                                       "cloaked in a mist.\n"
                                       "It prevents stat\n"
                                       "loss for five turns."),
        .animScript = gMoveAnim_MIST,
		.effect = EFFECT_SET_MIST,
        .type = TYPE_ICE,
        .pp = 30,
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

    [MOVE_WATER_GUN] =
    {
		.name = _("Water Gun"),
		.description = COMPOUND_STRING("The target is\n"
                                       "blasted with a\n"
                                       "forceful shot of\n"
                                       "water."),
        .animScript = gMoveAnim_WATER_GUN,
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
		.description = COMPOUND_STRING("The foe is blasted\n"
                                       "by a huge volume of\n"
                                       "water launched\n"
                                       "under high pressure."),
        .animScript = gMoveAnim_HYDRO_PUMP,
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
		.description = COMPOUND_STRING("The user attacks\n"
                                       "swamping its\n"
                                       "surroundings with a\n"
                                       "giant wave."),
        .animScript = gMoveAnim_SURF,
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
		.description = COMPOUND_STRING("The foe is struck\n"
                                       "with an icy-cold\n"
                                       "beam. It may freeze\n"
                                       "the target solid."),
        .animScript = gMoveAnim_ICE_BEAM,
		.effect = EFFECT_HIT,
		.power = 90,
        .type = TYPE_ICE,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FREEZE,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BLIZZARD] =
    {
		.name = _("Blizzard"),
		.description = COMPOUND_STRING("A blizzard is\n"
                                       "summoned to strike\n"
                                       "the foe. It may\n"
                                       "freeze the target."),
        .animScript = gMoveAnim_BLIZZARD,
		.effect = EFFECT_NEVER_MISS_IN_WEATHER,
		.power = 110,
        .type = TYPE_ICE,
        .accuracy = 70,
        .pp = 5,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.windMove = TRUE,
		},
		.argument = {
			.neverMissInWeather = { .weather = B_WEATHER_HAIL },
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FREEZE,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PSYBEAM] =
    {
		.name = _("Psybeam"),
		.description = COMPOUND_STRING("The foe is attacked\n"
                                       "with a peculiar\n"
                                       "ray. This may also\n"
                                       "confuse the target."),
        .animScript = gMoveAnim_PSYBEAM,
		.effect = EFFECT_HIT,
        .power = 65,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_CONFUSION,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BUBBLE_BEAM] =
    {
		.name = _("Bubble Beam"),
		.description = COMPOUND_STRING("A spray of bubbles\n"
                                       "is ejected at the\n"
                                       "target. This may\n"
                                       "lower its Speed."),
        .animScript = gMoveAnim_BUBBLE_BEAM,
		.effect = EFFECT_HIT,
        .power = 65,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SPD_MINUS_1,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_AURORA_BEAM] =
    {
		.name = _("Aurora Beam"),
		.description = COMPOUND_STRING("A rainbow-colored\n"
                                       "beam hits the foe.\n"
                                       "This may also lower\n"
                                       "the target's Attack."),
        .animScript = gMoveAnim_AURORA_BEAM,
		.effect = EFFECT_HIT,
        .power = 65,
        .type = TYPE_ICE,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_ATK_MINUS_1,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HYPER_BEAM] =
    {
		.name = _("Hyper Beam"),
		.description = COMPOUND_STRING("The foe is attacked\n"
                                       "with a powerful\n"
                                       "beam. The user must\n"
                                       "rest the next turn."),
        .animScript = gMoveAnim_HYPER_BEAM,
		.effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_RECHARGE,
			.chance = 0,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PECK] =
    {
		.name = _("Peck"),
		.description = COMPOUND_STRING("The target is\n"
                                       "jabbed with a\n"
                                       "sharply pointed\n"
                                       "beak or horn."),
        .animScript = gMoveAnim_PECK,
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
		.description = COMPOUND_STRING("A corkscrewing\n"
                                       "attack that strikes\n"
                                       "the foe with a beak\n"
                                       "acting as a drill."),
        .animScript = gMoveAnim_DRILL_PECK,
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
		.description = COMPOUND_STRING("A reckless,\n"
                                       "full-body throw\n"
                                       "attack that also\n"
                                       "hurts the user."),
        .animScript = gMoveAnim_SUBMISSION,
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
		.description = COMPOUND_STRING("A low, tripping\n"
                                       "kick that inflicts\n"
                                       "more damage on\n"
                                       "heavier foes."),
        .animScript = gMoveAnim_LOW_KICK,
		.effect = EFFECT_DAMAGE_BASED_TARGET_WEIGHT,
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
		.description = COMPOUND_STRING("A retaliation move\n"
                                       "that counters any\n"
                                       "physical hit with\n"
                                       "double the damage."),
        .animScript = gMoveAnim_COUNTER,
		.effect = EFFECT_COUNTER_ATTACK,
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
			.forbiddenMeFirst = TRUE,
		},
		.argument = {
			.counterAttack = { .split = SPLIT_PHYSICAL },
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SEISMIC_TOSS] =
    {
		.name = _("Seismic Toss"),
		.description = COMPOUND_STRING("A gravity-fed throw\n"
                                       "that causes damage\n"
                                       "matching the user's\n"
                                       "level."),
        .animScript = gMoveAnim_SEISMIC_TOSS,
		.effect = EFFECT_USER_LEVEL_TO_DAMAGE,
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
		.name = _("Strength"),
		.description = COMPOUND_STRING("The target is\n"
                                       "slugged with a\n"
                                       "punch thrown at\n"
                                       "maximum power."),
        .animScript = gMoveAnim_STRENGTH,
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
		.description = COMPOUND_STRING("A nutrient-draining\n"
                                       "attack. The user's\n"
                                       "HP is restored by\n"
                                       "half the damage."),
        .animScript = gMoveAnim_ABSORB,
		.effect = EFFECT_HIT_ABSORB,
        .power = 20,
        .type = TYPE_GRASS,
        .accuracy = 100,
		.pp = 25,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.kingsRockAffected = TRUE,
		},
		.argument = {
			.absorb = { .percentage = 50 },
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MEGA_DRAIN] =
    {
		.name = _("Mega Drain"),
		.description = COMPOUND_STRING("A nutrient-draining\n"
                                       "attack. The user's\n"
                                       "HP is restored by\n"
                                       "half the damage."),
        .animScript = gMoveAnim_MEGA_DRAIN,
		.effect = EFFECT_HIT_ABSORB,
        .power = 40,
        .type = TYPE_GRASS,
        .accuracy = 100,
		.pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.kingsRockAffected = TRUE,
		},
		.argument = {
			.absorb = { .percentage = 50 },
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LEECH_SEED] =
    {
		.name = _("Leech Seed"),
		.description = COMPOUND_STRING("A seed is planted\n"
                                       "on the target. It\n"
                                       "steals some HP from\n"
                                       "the foe every turn."),
        .animScript = gMoveAnim_LEECH_SEED,
		.effect = EFFECT_SET_SEEDED,
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
		.description = COMPOUND_STRING("The user's body\n"
                                       "grows all at once,\n"
                                       "boosting the Attack\n"
                                       "and Sp. Atk stats."),
        .animScript = gMoveAnim_GROWTH,
		.effect = EFFECT_USER_ATTACK_AND_SP_ATTACK_UP_2X_IN_SUNNY,
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
		.description = COMPOUND_STRING("The foe is hit with\n"
                                       "a cutting leaf. It\n"
                                       "has a high\n"
                                       "critical-hit ratio."),
        .animScript = gMoveAnim_RAZOR_LEAF,
		.effect = EFFECT_HIT,
        .power = 55,
        .type = TYPE_GRASS,
        .accuracy = 95,
        .pp = 25,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.critStage = 1,
			.slicingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SOLAR_BEAM] =
    {
		.name = _("Solar Beam"),
		.description = COMPOUND_STRING("The user gathers\n"
                                       "light on the first\n"
                                       "turn, then blasts a\n"
                                       "beam next turn."),
        .animScript = gMoveAnim_SOLAR_BEAM,
		.effect = EFFECT_SKIP_CHARGING_IN_WEATHER,
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
		},
		.argument = {
			.twoTurns = {
				.stringId = B_MSG_ABSORBED_LIGHT,
				.weather = B_WEATHER_SUN_ANY,
			},
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_POISON_POWDER] =
    {
		.name = _("Poison Powder"),
		.description = COMPOUND_STRING("The user scatters a\n"
                                       "cloud of poisonous\n"
                                       "dust that poisons\n"
                                       "the target."),
        .animScript = gMoveAnim_POISON_POWDER,
		.effect = EFFECT_SET_POISON,
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
		.description = COMPOUND_STRING("The user scatters a\n"
                                       "cloud of numbing\n"
                                       "powder that\n"
                                       "paralyzes the foe."),
        .animScript = gMoveAnim_STUN_SPORE,
		.effect = EFFECT_SET_PARALYZE,
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
		.description = COMPOUND_STRING("The user scatters a\n"
                                       "cloud of soporific\n"
                                       "dust that puts the\n"
                                       "target to sleep."),
        .animScript = gMoveAnim_SLEEP_POWDER,
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
		.description = COMPOUND_STRING("The user attacks\n"
                                       "with petals for two\n"
                                       "to three turns,\n"
                                       "then gets confused."),
        .animScript = gMoveAnim_PETAL_DANCE,
		.effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_THRASH,
			.chance = 0,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STRING_SHOT] =
    {
		.name = _("String Shot"),
		.description = COMPOUND_STRING("The foes are\n"
                                       "bounded with\n"
                                       "strings shot to\n"
                                       "reduce its Speed."),
        .animScript = gMoveAnim_STRING_SHOT,
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
		.description = COMPOUND_STRING("The foe is stricken\n"
                                       "by a shock wave. It\n"
                                       "always inflicts 40\n"
                                       "HP damage."),
        .animScript = gMoveAnim_DRAGON_RAGE,
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
		.argument = {
			.fixedDamage = { .amount = 40 },
		},
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FIRE_SPIN] =
    {
		.name = _("Fire Spin"),
		.description = COMPOUND_STRING("The foe is trapped\n"
                                       "in an spiral of\n"
                                       "fire that rages\n"
                                       "four to five turns."),
        .animScript = gMoveAnim_FIRE_SPIN,
		.effect = EFFECT_HIT,
		.power = 35,
        .type = TYPE_FIRE,
		.accuracy = 85,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
		.argument = {
			.bind = { .trappingId = TRAP_ID_FIRE_SPIN },
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_WRAP,
			.chance = 0,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_THUNDER_SHOCK] =
    {
		.name = _("Thunder Shock"),
		.description = COMPOUND_STRING("It attacks the foe\n"
                                       "with a jolt of\n"
                                       "electricity. It may\n"
                                       "paralyze the foe."),
        .animScript = gMoveAnim_THUNDER_SHOCK,
		.effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_PARALYSIS,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_THUNDERBOLT] =
    {
		.name = _("Thunderbolt"),
		.description = COMPOUND_STRING("A strong electric\n"
                                       "blast is loosed at\n"
                                       "the foe. It may\n"
                                       "paralyze the foe."),
        .animScript = gMoveAnim_THUNDERBOLT,
		.effect = EFFECT_HIT,
		.power = 90,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_PARALYSIS,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_THUNDER_WAVE] =
    {
		.name = _("Thunder Wave"),
		.description = COMPOUND_STRING("The user launches a\n"
                                       "weak jolt of\n"
                                       "electricity that\n"
                                       "paralyzes the foe."),
        .animScript = gMoveAnim_THUNDER_WAVE,
		.effect = EFFECT_SET_PARALYZE,
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
		.description = COMPOUND_STRING("A wicked thunder is\n"
                                       "dropped on the foe.\n"
                                       "It may also leave\n"
                                       "the foe paralyzed."),
        .animScript = gMoveAnim_THUNDER,
		.effect = EFFECT_NEVER_MISS_IN_WEATHER,
		.power = 110,
        .type = TYPE_ELECTRIC,
        .accuracy = 70,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.hitInAir = TRUE,
		},
		.argument = {
			.neverMissInWeather = {
				.weather = B_WEATHER_RAIN_ANY,
				.debuffWeather = B_WEATHER_SUN_ANY,
			},
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_PARALYSIS,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ROCK_THROW] =
    {
		.name = _("Rock Throw"),
		.description = COMPOUND_STRING("The user picks up\n"
                                       "and throws a small\n"
                                       "rock at the target\n"
                                       "to inflict damage."),
        .animScript = gMoveAnim_ROCK_THROW,
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
		.description = COMPOUND_STRING("The user sets off\n"
                                       "an earthquake that\n"
                                       "strikes every\n"
                                       "Pokémon around it."),
        .animScript = gMoveAnim_EARTHQUAKE,
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
		.description = COMPOUND_STRING("The foe is dropped\n"
                                       "into a fissure. The\n"
                                       "target instantly\n"
                                       "faints if it hits."),
        .animScript = gMoveAnim_FISSURE,
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
		.description = COMPOUND_STRING("The user burrows\n"
                                       "into the ground,\n"
                                       "then attacks on the\n"
                                       "next turn."),
        .animScript = gMoveAnim_DIG,
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
		},
		.argument = {
			.semiInvulnerable = {
				.stringId = B_MSG_BURROWED_IN_GROUND,
				.status = COMPRESS_BITS(STATUS3_UNDERGROUND),
			},
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TOXIC] =
    {
		.name = _("Toxic"),
		.description = COMPOUND_STRING("It leaves the foe\n"
                                       "badly poisoned. Its\n"
                                       "poison damage\n"
                                       "worsens every turn."),
		.animScript = gMoveAnim_TOXIC,
        .effect = EFFECT_SET_TOXIC_POISON,
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
		.description = COMPOUND_STRING("The foe is hit by a\n"
                                       "weak telekinetic\n"
                                       "force. It may also\n"
                                       "confuse the foe."),
        .animScript = gMoveAnim_CONFUSION,
		.effect = EFFECT_HIT,
        .power = 50,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 25,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_CONFUSION,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PSYCHIC] =
    {
		.name = _("Psychic"),
		.description = COMPOUND_STRING("The foe is hit by a\n"
                                       "strong telekinetic\n"
                                       "force. It may lower\n"
                                       "the foe's Sp. Def."),
        .animScript = gMoveAnim_PSYCHIC,
		.effect = EFFECT_HIT,
        .power = 90,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_DEF_MINUS_1,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HYPNOSIS] =
    {
		.name = _("Hypnosis"),
		.description = COMPOUND_STRING("The user employs\n"
                                       "hypnotic suggestion\n"
                                       "to make the target\n"
                                       "fall asleep."),
        .animScript = gMoveAnim_HYPNOSIS,
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
		.description = COMPOUND_STRING("The user meditates\n"
                                       "to awaken its power\n"
                                       "and raise its\n"
                                       "Attack stat."),
        .animScript = gMoveAnim_MEDITATE,
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
		.description = COMPOUND_STRING("The user relaxes\n"
                                       "and lightens its\n"
                                       "body to sharply\n"
                                       "boost its Speed."),
        .animScript = gMoveAnim_AGILITY,
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
		.description = COMPOUND_STRING("An almost invisibly\n"
                                       "fast attack that is\n"
                                       "certain to strike\n"
                                       "first."),
        .animScript = gMoveAnim_QUICK_ATTACK,
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
		.description = COMPOUND_STRING("While this move is\n"
                                       "in use, it gains\n"
                                       "Attack each time\n"
                                       "the user is hit."),
        .animScript = gMoveAnim_RAGE,
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
		.description = COMPOUND_STRING("It switches places\n"
                                       "with a different\n"
                                       "Pokémon, using\n"
                                       "telekinetic power."),
        .animScript = gMoveAnim_TELEPORT,
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
		.description = COMPOUND_STRING("The user makes the\n"
                                       "foe see a mirage.\n"
                                       "It damages matching\n"
                                       "the user's level."),
        .animScript = gMoveAnim_NIGHT_SHADE,
		.effect = EFFECT_USER_LEVEL_TO_DAMAGE,
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
		.name = _("Mimic"),
		.description = COMPOUND_STRING("The user copies the\n"
                                       "move last used by\n"
                                       "the foe for the\n"
                                       "rest of the battle."),
        .animScript = gMoveAnim_MIMIC,
		.effect = EFFECT_MIMIC_MOVE,
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
		.description = COMPOUND_STRING("An earsplitting\n"
                                       "screech harshly\n"
                                       "lowers the target's\n"
                                       "Defense stat."),
        .animScript = gMoveAnim_SCREECH,
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
		.description = COMPOUND_STRING("The user creates\n"
                                       "illusory copies of\n"
                                       "itself to raise its\n"
                                       "evasiveness."),
        .animScript = gMoveAnim_DOUBLE_TEAM,
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
		.description = COMPOUND_STRING("Restoring its own\n"
                                       "cells, the user\n"
                                       "restores its HP by\n"
                                       "half of its max HP."),
        .animScript = gMoveAnim_RECOVER,
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
		.name = _("Harden"),
		.description = COMPOUND_STRING("The user stiffens\n"
                                       "all the muscles in\n"
                                       "its body to boost\n"
                                       "its Defense stat."),
        .animScript = gMoveAnim_HARDEN,
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
		.description = COMPOUND_STRING("The user compresses\n"
                                       "all the cells in\n"
                                       "its body to raise\n"
                                       "its evasiveness."),
        .animScript = gMoveAnim_MINIMIZE,
		.effect = EFFECT_EVASION_UP_SET_MINIMIZE,
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
		.description = COMPOUND_STRING("An obscuring cloud\n"
                                       "of smoke or ink\n"
                                       "reduces the foe's\n"
                                       "accuracy."),
        .animScript = gMoveAnim_SMOKESCREEN,
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
		.description = COMPOUND_STRING("The target is\n"
                                       "exposed to a\n"
                                       "sinister ray that\n"
                                       "causes confusion."),
        .animScript = gMoveAnim_CONFUSE_RAY,
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
		.description = COMPOUND_STRING("The user withdraws\n"
                                       "its body into its\n"
                                       "shell, boosting its\n"
                                       "Defense stat."),
        .animScript = gMoveAnim_WITHDRAW,
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
		.description = COMPOUND_STRING("The user curls up\n"
                                       "to conceal weak\n"
                                       "spots and raise its\n"
                                       "Defense stat."),
        .animScript = gMoveAnim_DEFENSE_CURL,
		.effect = EFFECT_DEFENSE_UP_SET_DEFENSE_CURL,
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
		.description = COMPOUND_STRING("The user throws up\n"
                                       "a sturdy wall that\n"
                                       "sharply raises its\n"
                                       "Defense stat."),
        .animScript = gMoveAnim_BARRIER,
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
		.description = COMPOUND_STRING("A wall of light\n"
                                       "cuts damage from\n"
                                       "special attacks for\n"
                                       "five turns."),
        .animScript = gMoveAnim_LIGHT_SCREEN,
		.effect = EFFECT_SET_LIGHT_SCREEN,
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
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_HAZE] =
    {
		.name = _("Haze"),
		.description = COMPOUND_STRING("Eliminates all stat\n"
                                       "changes among all\n"
                                       "Pokémon engaged in\n"
                                       "battle."),
        .animScript = gMoveAnim_HAZE,
		.effect = EFFECT_NORMALISE_FIELD_BUFFS,
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
		.description = COMPOUND_STRING("A wall of light\n"
                                       "cuts damage from\n"
                                       "physical attacks\n"
                                       "for five turns."),
        .animScript = gMoveAnim_REFLECT,
		.effect = EFFECT_SET_REFLECT,
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
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_FOCUS_ENERGY] =
    {
		.name = _("Focus Energy"),
		.description = COMPOUND_STRING("The user takes a\n"
                                       "breath and focuses\n"
                                       "so that critical\n"
                                       "hits land more."),
        .animScript = gMoveAnim_FOCUS_ENERGY,
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
		.description = COMPOUND_STRING("The user endures\n"
                                       "attacks for two\n"
                                       "turns, then strikes\n"
                                       "back double."),
        .animScript = gMoveAnim_BIDE,
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
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_METRONOME] =
    {
		.name = _("Metronome"),
		.description = COMPOUND_STRING("Waggles a finger\n"
                                       "and stimulates the\n"
                                       "brain into using\n"
                                       "any move at random."),
        .animScript = gMoveAnim_METRONOME,
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
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MIRROR_MOVE] =
    {
		.name = _("Mirror Move"),
		.description = COMPOUND_STRING("The user counters\n"
                                       "the target by\n"
                                       "mimicking the\n"
                                       "target's last move."),
        .animScript = gMoveAnim_NONE,
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
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_2,
    },

    [MOVE_SELF_DESTRUCT] =
    {
		.name = _("Self-Destruct"),
		.description = COMPOUND_STRING("The user blows up\n"
                                       "to inflict severe\n"
                                       "damage, even making\n"
                                       "itself faint."),
        .animScript = gMoveAnim_SELF_DESTRUCT,
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
		.description = COMPOUND_STRING("A large egg is\n"
                                       "hurled at the foe\n"
                                       "with maximum force\n"
                                       "to inflict damage."),
        .animScript = gMoveAnim_EGG_BOMB,
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
		.description = COMPOUND_STRING("The foe is licked\n"
                                       "and hit with a long\n"
                                       "tongue. It may also\n"
                                       "paralyze."),
        .animScript = gMoveAnim_LICK,
		.effect = EFFECT_HIT,
		.power = 30,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_PARALYSIS,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SMOG] =
    {
		.name = _("Smog"),
		.description = COMPOUND_STRING("The foe is attacked\n"
                                       "with exhaust gases.\n"
                                       "It may also poison\n"
                                       "the foe."),
        .animScript = gMoveAnim_SMOG,
		.effect = EFFECT_HIT,
		.power = 30,
        .type = TYPE_POISON,
        .accuracy = 70,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_POISON,
			.chance = 40,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SLUDGE] =
    {
		.name = _("Sludge"),
		.description = COMPOUND_STRING("Unsanitary sludge\n"
                                       "is hurled at the\n"
                                       "target. It may also\n"
                                       "poison the target."),
        .animScript = gMoveAnim_SLUDGE,
		.effect = EFFECT_HIT,
        .power = 65,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_POISON,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BONE_CLUB] =
    {
		.name = _("Bone Club"),
		.description = COMPOUND_STRING("The user clubs the\n"
                                       "target with a bone.\n"
                                       "This may also make\n"
                                       "the target flinch."),
        .animScript = gMoveAnim_BONE_CLUB,
		.effect = EFFECT_HIT,
        .power = 65,
        .type = TYPE_GROUND,
        .accuracy = 85,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FIRE_BLAST] =
    {
		.name = _("Fire Blast"),
		.description = COMPOUND_STRING("The foe is hit with\n"
                                       "an intense flame.\n"
                                       "It may leave the\n"
                                       "target with a burn."),
        .animScript = gMoveAnim_FIRE_BLAST,
		.effect = EFFECT_HIT,
		.power = 110,
        .type = TYPE_FIRE,
        .accuracy = 85,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_BURN,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WATERFALL] =
    {
		.name = _("Waterfall"),
		.description = COMPOUND_STRING("A powerful charge\n"
                                       "attack. It can also\n"
                                       "be used to climb a\n"
                                       "waterfall."),
		.animScript = gMoveAnim_WATERFALL,
		.effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 20,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CLAMP] =
    {
		.name = _("Clamp"),
		.description = COMPOUND_STRING("The foe is clamped\n"
                                       "and squeezed by the\n"
                                       "user's shell for\n"
                                       "four to five turns."),
        .animScript = gMoveAnim_CLAMP,
		.effect = EFFECT_HIT,
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
		.argument = {
			.bind = { .trappingId = TRAP_ID_CLAMP },
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_WRAP,
			.chance = 0,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SWIFT] =
    {
		.name = _("Swift"),
		.description = COMPOUND_STRING("Star-shaped rays\n"
                                       "are shot at the\n"
                                       "target. This attack\n"
                                       "never misses."),
        .animScript = gMoveAnim_SWIFT,
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
		.description = COMPOUND_STRING("The user raises its\n"
                                       "Defense on first\n"
                                       "turn, then attacks\n"
                                       "on the second turn."),
        .animScript = gMoveAnim_SKULL_BASH,
		.effect = EFFECT_TWO_TURNS_ATTACK,
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
		},
		.argument = {
			.twoTurns = { .stringId = B_MSG_TUCKED_HEAD },
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_DEF_PLUS_1,
			.chance = 0,
			.self = TRUE,
			.onChargeTurnOnly = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPIKE_CANNON] =
    {
		.name = _("Spike Cannon"),
		.description = COMPOUND_STRING("Sharp spikes are\n"
                                       "fired at the foe to\n"
                                       "strike two to five\n"
                                       "times in succession."),
        .animScript = gMoveAnim_SPIKE_CANNON,
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
		.description = COMPOUND_STRING("The foe is attacked\n"
                                       "with long tentacles\n"
                                       "or vines. It may\n"
                                       "lower Speed."),
        .animScript = gMoveAnim_CONSTRICT,
		.effect = EFFECT_HIT,
        .power = 10,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 35,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SPD_MINUS_1,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_AMNESIA] =
    {
		.name = _("Amnesia"),
		.description = COMPOUND_STRING("It temporarily\n"
                                       "empties its mind.\n"
                                       "It sharply raises\n"
                                       "its Sp. Def stat."),
        .animScript = gMoveAnim_AMNESIA,
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
		.description = COMPOUND_STRING("The user distracts\n"
                                       "the foe by bending\n"
                                       "a spoon. It lowers\n"
                                       "the foe's accuracy."),
        .animScript = gMoveAnim_KINESIS,
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
		.description = COMPOUND_STRING("Heals the user by\n"
                                       "up to half its full\n"
                                       "HP. It can be used\n"
                                       "to heal an ally."),
        .animScript = gMoveAnim_SOFT_BOILED,
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
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_HI_JUMP_KICK] =
    {
		.name = _("High Jump Kick"),
		.description = COMPOUND_STRING("A strong jumping\n"
                                       "knee kick. If it\n"
                                       "misses, the user is\n"
                                       "hurt."),
        .animScript = gMoveAnim_HIGH_JUMP_KICK,
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
		.description = COMPOUND_STRING("It intimidates the\n"
                                       "foe with its\n"
                                       "belly's pattern to\n"
                                       "cause paralysis."),
        .animScript = gMoveAnim_GLARE,
		.effect = EFFECT_SET_PARALYZE,
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
		.description = COMPOUND_STRING("Absorbs half the\n"
                                       "damage it inflicted\n"
                                       "on a sleeping foe\n"
                                       "to restore HP."),
        .animScript = gMoveAnim_DREAM_EATER,
		.effect = EFFECT_DREAM_EATER,
        .power = 100,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.argument = {
			.absorb = { .percentage = 50 },
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_POISON_GAS] =
    {
		.name = _("Poison Gas"),
		.description = COMPOUND_STRING("A cloud of poison\n"
                                       "gas is sprayed at\n"
                                       "opposing Pokémon,\n"
                                       "poisoning those hit."),
        .animScript = gMoveAnim_POISON_GAS,
		.effect = EFFECT_SET_POISON,
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
		.description = COMPOUND_STRING("Round objects are\n"
                                       "hurled at the foe\n"
                                       "to strike two to\n"
                                       "five times in a row."),
        .animScript = gMoveAnim_BARRAGE,
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
		.description = COMPOUND_STRING("A blood-draining\n"
                                       "attack. The user's\n"
                                       "HP is restored by\n"
                                       "half the damage."),
        .animScript = gMoveAnim_LEECH_LIFE,
		.effect = EFFECT_HIT_ABSORB,
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
		.argument = {
			.absorb = { .percentage = 50 },
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LOVELY_KISS] =
    {
		.name = _("Lovely Kiss"),
		.description = COMPOUND_STRING("The user forces a\n"
                                       "kiss on the foe\n"
                                       "with a scary face\n"
                                       "that induces sleep."),
        .animScript = gMoveAnim_LOVELY_KISS,
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
		.description = COMPOUND_STRING("A two-turn attack\n"
                                       "with a high\n"
                                       "critical-hit ratio.\n"
                                       "The foe may flinch."),
        .animScript = gMoveAnim_SKY_ATTACK,
		.effect = EFFECT_TWO_TURNS_ATTACK,
        .power = 140,
        .type = TYPE_FLYING,
        .accuracy = 90,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.critStage = 1,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE,
		},
		.argument = {
			.twoTurns = { .stringId = B_MSG_CLOAKED_IN_SUNLIGHT },
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TRANSFORM] =
    {
		.name = _("Transform"),
		.description = COMPOUND_STRING("The user transforms\n"
                                       "into a copy of the\n"
                                       "foe with even the\n"
                                       "same move set."),
        .animScript = gMoveAnim_TRANSFORM,
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
		.description = COMPOUND_STRING("A spray of bubbles\n"
                                       "hits the foe. It\n"
                                       "may lower the foe's\n"
                                       "Speed stat."),
        .animScript = gMoveAnim_BUBBLE,
		.effect = EFFECT_HIT,
		.power = 40,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 30,
        .target = MOVE_TARGET_BOTH,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SPD_MINUS_1,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DIZZY_PUNCH] =
    {
		.name = _("Dizzy Punch"),
		.description = COMPOUND_STRING("The foe is hit with\n"
                                       "a rhythmically\n"
                                       "punch that may also\n"
                                       "leave it confused."),
        .animScript = gMoveAnim_DIZZY_PUNCH,
		.effect = EFFECT_HIT,
        .power = 70,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.punchMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_CONFUSION,
			.chance = 20,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPORE] =
    {
		.name = _("Spore"),
		.description = COMPOUND_STRING("The user scatters\n"
                                       "bursts of spores\n"
                                       "that induce sleep."),
        .animScript = gMoveAnim_SPORE,
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
		.description = COMPOUND_STRING("A blast of light\n"
                                       "that cuts the foe's\n"
                                       "accuracy. It also\n"
                                       "illuminates caves."),
        .animScript = gMoveAnim_FLASH,
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
		.description = COMPOUND_STRING("The foe is attacked\n"
                                       "with an odd psychic\n"
                                       "wave. The attack\n"
                                       "varies in intensity."),
        .animScript = gMoveAnim_PSYWAVE,
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
		.name = _("Splash"),
		.description = COMPOUND_STRING("The user just flops\n"
                                       "and splashes around\n"
                                       "to no effect at\n"
                                       "all..."),
        .animScript = gMoveAnim_SPLASH,
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
		.description = COMPOUND_STRING("The user alters its\n"
                                       "cells to liquefy\n"
                                       "itself and sharply\n"
                                       "raise Defense."),
        .animScript = gMoveAnim_ACID_ARMOR,
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
		.description = COMPOUND_STRING("A large pincer is\n"
                                       "used to hammer the\n"
                                       "foe. It has a high\n"
                                       "critical-hit ratio."),
        .animScript = gMoveAnim_CRABHAMMER,
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
			.critStage = 1,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_EXPLOSION] =
    {
		.name = _("Explosion"),
		.description = COMPOUND_STRING("The user explodes\n"
                                       "to inflict terrible\n"
                                       "damage even while\n"
                                       "fainting itself."),
        .animScript = gMoveAnim_EXPLOSION,
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
		.description = COMPOUND_STRING("The foe is raked\n"
                                       "with sharp claws or\n"
                                       "scythes two to five\n"
                                       "times."),
        .animScript = gMoveAnim_FURY_SWIPES,
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
		.description = COMPOUND_STRING("The user throws a\n"
                                       "bone that hits the\n"
                                       "foe once, then once\n"
                                       "again on return."),
        .animScript = gMoveAnim_BONEMERANG,
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
		.description = COMPOUND_STRING("The user sleeps for\n"
                                       "two turns to fully\n"
                                       "restore HP and heal\n"
                                       "any status problem."),
        .animScript = gMoveAnim_REST,
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
		.name = _("Rock Slide"),
		.description = COMPOUND_STRING("Large boulders are\n"
                                       "hurled at the foe.\n"
                                       "It may make the foe\n"
                                       "flinch."),
        .animScript = gMoveAnim_ROCK_SLIDE,
		.effect = EFFECT_HIT,
        .power = 75,
        .type = TYPE_ROCK,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_BOTH,
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HYPER_FANG] =
    {
		.name = _("Hyper Fang"),
		.description = COMPOUND_STRING("The foe is attacked\n"
                                       "with sharp fangs.\n"
                                       "It may make the foe\n"
                                       "flinch."),
        .animScript = gMoveAnim_HYPER_FANG,
		.effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_NORMAL,
        .accuracy = 90,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.bitingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHARPEN] =
    {
		.name = _("Sharpen"),
		.description = COMPOUND_STRING("The user makes its\n"
                                       "edges more jagged,\n"
                                       "which raises its\n"
                                       "Attack stat."),
        .animScript = gMoveAnim_SHARPEN,
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
		.description = COMPOUND_STRING("The user changes\n"
                                       "its type to the\n"
                                       "same type as the\n"
                                       "move in first slot."),
        .animScript = gMoveAnim_CONVERSION,
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
		.description = COMPOUND_STRING("A simultaneous\n"
                                       "3-beam attack that\n"
                                       "may paralyze, burn,\n"
                                       "or freeze the foe."),
        .animScript = gMoveAnim_TRI_ATTACK,
		.effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_TRI_ATTACK,
			.chance = 20,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SUPER_FANG] =
    {
		.name = _("Super Fang"),
		.description = COMPOUND_STRING("The user attacks\n"
                                       "with sharp fangs\n"
                                       "and halves the\n"
                                       "foe's HP."),
        .animScript = gMoveAnim_SUPER_FANG,
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
		.name = _("Slash"),
		.description = COMPOUND_STRING("The foe is slashed\n"
                                       "with claws, etc. It\n"
                                       "has a high\n"
                                       "critical-hit ratio."),
        .animScript = gMoveAnim_SLASH,
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
			.critStage = 1,
			.slicingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SUBSTITUTE] =
    {
		.name = _("Substitute"),
		.description = COMPOUND_STRING("The user creates a\n"
                                       "decoy using\n"
                                       "one-quarter of its\n"
                                       "full HP."),
        .animScript = gMoveAnim_SUBSTITUTE,
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
		.description = COMPOUND_STRING("An attack that is\n"
                                       "used only if there\n"
                                       "is no PP. It also\n"
                                       "hurts the user."),
        .animScript = gMoveAnim_STRUGGLE,
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
			.forbiddenParentalBond = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SKETCH] =
    {
		.name = _("Sketch"),
		.description = COMPOUND_STRING("This move copies\n"
                                       "the move last used\n"
                                       "by the foe, then\n"
                                       "disappears."),
        .animScript = gMoveAnim_SKETCH,
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
		.description = COMPOUND_STRING("A three-kick attack\n"
                                       "that becomes more\n"
                                       "powerful with each\n"
                                       "successful hit."),
        .animScript = gMoveAnim_TRIPLE_KICK,
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
		.argument = {
			.tripleKick = { .increment = 10 },
		},
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_THIEF] =
    {
		.name = _("Thief"),
		.description = COMPOUND_STRING("An attack that take\n"
                                       "the foe's held item\n"
                                       "if the user isn't\n"
                                       "holding one."),
        .animScript = gMoveAnim_THIEF,
		.effect = EFFECT_HIT,
		.power = 60,
        .type = TYPE_DARK,
        .accuracy = 100,
		.pp = 25,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenMeFirst = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_STEAL_ITEM,
			.chance = 0,
			.onFinalMultiHitOnly = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPIDER_WEB] =
    {
		.name = _("Spider Web"),
		.description = COMPOUND_STRING("The user ensnares\n"
                                       "the foe with gooey\n"
                                       "silk so it can't\n"
                                       "flee from battle."),
        .animScript = gMoveAnim_SPIDER_WEB,
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
		.description = COMPOUND_STRING("The user predicts\n"
                                       "the foe's action to\n"
                                       "ensure its next\n"
                                       "attack hits."),
        .animScript = gMoveAnim_MIND_READER,
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
		.description = COMPOUND_STRING("A sleeping target\n"
                                       "sees a nightmare\n"
                                       "that inflicts some\n"
                                       "damage every turn."),
        .animScript = gMoveAnim_NIGHTMARE,
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
		.description = COMPOUND_STRING("The user makes a\n"
                                       "fiery charge at the\n"
                                       "foe. It may cause a\n"
                                       "burn."),
        .animScript = gMoveAnim_FLAME_WHEEL,
		.effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 25,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.thawUser = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_BURN,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SNORE] =
    {
		.name = _("Snore"),
		.description = COMPOUND_STRING("An attack that can\n"
                                       "be used only while\n"
                                       "asleep. It may\n"
                                       "cause flinching."),
        .animScript = gMoveAnim_SNORE,
		.effect = EFFECT_SNORE,
		.power = 50,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.soundMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CURSE] =
    {
		.name = _("Curse"),
		.description = COMPOUND_STRING("A move that works\n"
                                       "differently for the\n"
                                       "Ghost type than for\n"
                                       "all other types."),
        .animScript = gMoveAnim_CURSE,
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
		.description = COMPOUND_STRING("A desperate attack\n"
                                       "that becomes more\n"
                                       "powerful the less\n"
                                       "HP the user has."),
        .animScript = gMoveAnim_FLAIL,
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
		.description = COMPOUND_STRING("The user changes\n"
                                       "type to make itself\n"
                                       "resistant to the\n"
                                       "last attack it took."),
        .animScript = gMoveAnim_CONVERSION_2,
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
		.description = COMPOUND_STRING("A vortex of air is\n"
                                       "shot at the foe. It\n"
                                       "has a high\n"
                                       "critical-hit ratio."),
        .animScript = gMoveAnim_AEROBLAST,
		.effect = EFFECT_HIT,
        .power = 100,
        .type = TYPE_FLYING,
        .accuracy = 95,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.critStage = 1,
			.windMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_COTTON_SPORE] =
    {
		.name = _("Cotton Spore"),
		.description = COMPOUND_STRING("Cotton-like spores\n"
                                       "cling to the foes,\n"
                                       "sharply reducing\n"
                                       "their Speed stat."),
        .animScript = gMoveAnim_COTTON_SPORE,
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
		.description = COMPOUND_STRING("An all-out attack\n"
                                       "that becomes more\n"
                                       "powerful the less\n"
                                       "HP the user has."),
        .animScript = gMoveAnim_REVERSAL,
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
		.description = COMPOUND_STRING("It looses grudge on\n"
                                       "the foe's last used\n"
                                       "move by cutting 4\n"
                                       "PP from it."),
        .animScript = gMoveAnim_SPITE,
		.effect = EFFECT_SPITE,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.magicCoatAffected = TRUE,
		},
		.argument = {
			.spite = { .ppToDeduct = 4 },
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RECOVER_HP,
    },

    [MOVE_POWDER_SNOW] =
    {
		.name = _("Powder Snow"),
		.description = COMPOUND_STRING("It attacks with a\n"
                                       "gust of powdery\n"
                                       "snow. It may also\n"
                                       "freeze the targets."),
        .animScript = gMoveAnim_POWDER_SNOW,
		.effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_ICE,
        .accuracy = 100,
        .pp = 25,
        .target = MOVE_TARGET_BOTH,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FREEZE,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PROTECT] =
    {
		.name = _("Protect"),
		.description = COMPOUND_STRING("Enables the user to\n"
                                       "evade all attacks.\n"
                                       "It may fail if used\n"
                                       "in succession."),
        .animScript = gMoveAnim_PROTECT,
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
		.description = COMPOUND_STRING("The user throws a\n"
                                       "punch at blinding\n"
                                       "speed. This move\n"
                                       "always goes first."),
        .animScript = gMoveAnim_MACH_PUNCH,
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
		.description = COMPOUND_STRING("It frightens the\n"
                                       "foe with a scary\n"
                                       "face to sharply\n"
                                       "lower its Speed."),
        .animScript = gMoveAnim_SCARY_FACE,
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
		.description = COMPOUND_STRING("The user draws up\n"
                                       "close to the foe\n"
                                       "disarmingly, then\n"
                                       "hits without fail."),
        .animScript = gMoveAnim_FAINT_ATTACK,
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
		.description = COMPOUND_STRING("The user kisses the\n"
                                       "foe with sweet\n"
                                       "cuteness that\n"
                                       "causes confusion."),
        .animScript = gMoveAnim_SWEET_KISS,
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
		.description = COMPOUND_STRING("The user maximizes\n"
                                       "its Attack stat but\n"
                                       "loses HP equal to\n"
                                       "half its max HP."),
        .animScript = gMoveAnim_BELLY_DRUM,
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
		.description = COMPOUND_STRING("Unsanitary sludge\n"
                                       "is hurled at the\n"
                                       "foe. This may also\n"
                                       "poison the foe."),
        .animScript = gMoveAnim_SLUDGE_BOMB,
		.effect = EFFECT_HIT,
        .power = 90,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.ballisticMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_POISON,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MUD_SLAP] =
    {
		.name = _("Mud-Slap"),
		.description = COMPOUND_STRING("It hurls mud in the\n"
                                       "foe's face to\n"
                                       "inflict damage and\n"
                                       "lower its accuracy."),
        .animScript = gMoveAnim_MUD_SLAP,
		.effect = EFFECT_HIT,
        .power = 20,
        .type = TYPE_GROUND,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_ACC_MINUS_1,
			.chance = 100,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_OCTAZOOKA] =
    {
		.name = _("Octazooka"),
		.description = COMPOUND_STRING("Ink is blasted in\n"
                                       "the foe's face or\n"
                                       "eyes to damage and\n"
                                       "lower accuracy."),
        .animScript = gMoveAnim_OCTAZOOKA,
		.effect = EFFECT_HIT,
        .power = 65,
        .type = TYPE_WATER,
        .accuracy = 85,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.ballisticMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_ACC_MINUS_1,
			.chance = 50,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SPIKES] =
    {
		.name = _("Spikes"),
		.description = COMPOUND_STRING("A trap of spikes is\n"
                                       "laid around the\n"
                                       "foe's feet to hurt\n"
                                       "foes switching in."),
        .animScript = gMoveAnim_SPIKES,
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
		.description = COMPOUND_STRING("An electric blast\n"
                                       "is fired like a\n"
                                       "cannon to inflict\n"
                                       "damage and paralyze."),
        .animScript = gMoveAnim_ZAP_CANNON,
		.effect = EFFECT_HIT,
		.power = 120,
        .type = TYPE_ELECTRIC,
        .accuracy = 50,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.ballisticMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_PARALYSIS,
			.chance = 100,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FORESIGHT] =
    {
		.name = _("Foresight"),
		.description = COMPOUND_STRING("Completely negates\n"
                                       "the foe's efforts\n"
                                       "to heighten its\n"
                                       "ability to evade."),
        .animScript = gMoveAnim_FORESIGHT,
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
		.description = COMPOUND_STRING("When used, if\n"
                                       "faints, who landed\n"
                                       "the knockout hit\n"
                                       "also faints."),
        .animScript = gMoveAnim_DESTINY_BOND,
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
		.description = COMPOUND_STRING("Anyone that hears\n"
                                       "this song faints in\n"
                                       "three turns unless\n"
                                       "it switches out."),
        .animScript = gMoveAnim_PERISH_SONG,
		.effect = EFFECT_PERISH_SONG,
        .type = TYPE_NORMAL,
        .pp = 5,
        .target = MOVE_TARGET_ALL_BATTLERS,
        .flags =
		{
			.soundMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.hasQuietBGM = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_ICY_WIND] =
    {
		.name = _("Icy Wind"),
		.description = COMPOUND_STRING("It attacks with a\n"
                                       "gust of chilled\n"
                                       "air. Also lower the\n"
                                       "foes's Speed stats."),
        .animScript = gMoveAnim_ICY_WIND,
		.effect = EFFECT_HIT,
        .power = 55,
        .type = TYPE_ICE,
        .accuracy = 95,
        .pp = 15,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.windMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SPD_MINUS_1,
			.chance = 100,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DETECT] =
    {
		.name = _("Detect"),
		.description = COMPOUND_STRING("Enables the user to\n"
                                       "evade all attacks.\n"
                                       "It may fail if used\n"
                                       "in succession."),
        .animScript = gMoveAnim_DETECT,
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
		.description = COMPOUND_STRING("The user strikes\n"
                                       "the target with a\n"
                                       "hard bone two to\n"
                                       "five times in a row."),
        .animScript = gMoveAnim_BONE_RUSH,
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
		.description = COMPOUND_STRING("The user takes sure\n"
                                       "aim at the foe. It\n"
                                       "ensures the next\n"
                                       "move does not fail."),
        .animScript = gMoveAnim_LOCK_ON,
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
		.description = COMPOUND_STRING("It rampages and\n"
                                       "attacks for two to\n"
                                       "three turns. Then\n"
                                       "becomes confused."),
        .animScript = gMoveAnim_OUTRAGE,
		.effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_THRASH,
			.chance = 0,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SANDSTORM] =
    {
		.name = _("Sandstorm"),
		.description = COMPOUND_STRING("A 5-turn sandstorm\n"
                                       "that damages all\n"
                                       "types except Rock,\n"
                                       "Ground and Steel."),
        .animScript = gMoveAnim_SANDSTORM,
		.effect = EFFECT_SET_WEATHER,
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
		.argument = {
			.setWeather = {
				.weatherId = ENUM_WEATHER_SANDSTORM,
				.stringId = B_MSG_SANDSTORM_UP,
			},
		},
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_GIGA_DRAIN] =
    {
		.name = _("Giga Drain"),
		.description = COMPOUND_STRING("A harsh attack that\n"
                                       "absorbs half the\n"
                                       "damage it inflicted\n"
                                       "to restore HP."),
        .animScript = gMoveAnim_GIGA_DRAIN,
		.effect = EFFECT_HIT_ABSORB,
		.power = 75,
        .type = TYPE_GRASS,
        .accuracy = 100,
		.pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.argument = {
			.absorb = { .percentage = 50 },
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ENDURE] =
    {
		.name = _("Endure"),
		.description = COMPOUND_STRING("The user endures\n"
                                       "any hit with 1 HP.\n"
                                       "It may fail if used\n"
                                       "in succession."),
        .animScript = gMoveAnim_ENDURE,
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
		.description = COMPOUND_STRING("The foe is charmed\n"
                                       "by the user's cute\n"
                                       "appeals, sharply\n"
                                       "cutting its Attack."),
        .animScript = gMoveAnim_CHARM,
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
		.description = COMPOUND_STRING("A 5-turn rolling\n"
                                       "attack that becomes\n"
                                       "stronger each time\n"
                                       "it hits."),
        .animScript = gMoveAnim_ROLLOUT,
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
		.name = _("False Swipe"),
		.description = COMPOUND_STRING("A restrained attack\n"
                                       "that always leaves\n"
                                       "the foe with at\n"
                                       "least 1 HP."),
        .animScript = gMoveAnim_FALSE_SWIPE,
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
		.description = COMPOUND_STRING("A move that makes\n"
                                       "the foe confused,\n"
                                       "but also sharply\n"
                                       "raises its Attack."),
        .animScript = gMoveAnim_SWAGGER,
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
		.description = COMPOUND_STRING("Heals the user by\n"
                                       "up to half its full\n"
                                       "HP. It can be used\n"
                                       "to heal an ally."),
        .animScript = gMoveAnim_MILK_DRINK,
		.effect = EFFECT_RESTORE_HP,
        .type = TYPE_NORMAL,
        .pp = 5,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.forbiddenProtect = TRUE,
			.snatchAffected = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_SPARK] =
    {
		.name = _("Spark"),
		.description = COMPOUND_STRING("An electrically\n"
                                       "charged tackle that\n"
                                       "may also paralyze\n"
                                       "the foe."),
        .animScript = gMoveAnim_SPARK,
		.effect = EFFECT_HIT,
        .power = 65,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_PARALYSIS,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FURY_CUTTER] =
    {
		.name = _("Fury Cutter"),
		.description = COMPOUND_STRING("It slashes the foe\n"
                                       "with scythes. It\n"
                                       "grows stronger on\n"
                                       "each successive hit."),
        .animScript = gMoveAnim_FURY_CUTTER,
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
		.description = COMPOUND_STRING("The foe is hit with\n"
                                       "wings of steel. It\n"
                                       "may also raise the\n"
                                       "user's Defense stat."),
        .animScript = gMoveAnim_STEEL_WING,
		.effect = EFFECT_HIT,
        .power = 70,
        .type = TYPE_STEEL,
        .accuracy = 90,
        .pp = 25,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_DEF_PLUS_1,
			.chance = 10,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MEAN_LOOK] =
    {
		.name = _("Mean Look"),
		.description = COMPOUND_STRING("The foe is fixed\n"
                                       "with a mean look\n"
                                       "that prevents it\n"
                                       "from escaping."),
        .animScript = gMoveAnim_MEAN_LOOK,
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
		.description = COMPOUND_STRING("If it is the other\n"
                                       "gender, the foe is\n"
                                       "made infatuated and\n"
                                       "unlikely to attack."),
        .animScript = gMoveAnim_ATTRACT,
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
		.description = COMPOUND_STRING("While it is asleep,\n"
                                       "the user randomly\n"
                                       "uses one of the\n"
                                       "moves it knows."),
        .animScript = gMoveAnim_SLEEP_TALK,
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
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_BOOST_CRITS,
    },

    [MOVE_HEAL_BELL] =
    {
		.name = _("Heal Bell"),
		.description = COMPOUND_STRING("A soothing bell\n"
                                       "chimes to heal the\n"
                                       "status problems of\n"
                                       "all allies."),
        .animScript = gMoveAnim_HEAL_BELL,
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
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_RECOVER_HP,
    },

    [MOVE_RETURN] =
    {
		.name = _("Return"),
		.description = COMPOUND_STRING("This attack grows\n"
                                       "more powerful the\n"
                                       "more the user likes\n"
                                       "its Trainer."),
        .animScript = gMoveAnim_RETURN,
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
		.description = COMPOUND_STRING("The foe is given a\n"
                                       "booby-trapped gift.\n"
                                       "It restores HP\n"
                                       "sometimes, however."),
        .animScript = gMoveAnim_PRESENT,
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
		.description = COMPOUND_STRING("This attack grows\n"
                                       "more powerful the\n"
                                       "less the user likes\n"
                                       "its Trainer."),
        .animScript = gMoveAnim_FRUSTRATION,
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
		.description = COMPOUND_STRING("It creates a field\n"
                                       "that protects from\n"
                                       "status problems for\n"
                                       "five turns."),
        .animScript = gMoveAnim_SAFEGUARD,
		.effect = EFFECT_SAFEGUARD,
        .type = TYPE_NORMAL,
        .pp = 25,
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

    [MOVE_PAIN_SPLIT] =
    {
		.name = _("Pain Split"),
		.description = COMPOUND_STRING("The user adds its\n"
                                       "HP to the foe's HP,\n"
                                       "then equally shares\n"
                                       "the total HP."),
        .animScript = gMoveAnim_PAIN_SPLIT,
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
		.description = COMPOUND_STRING("The foe is razed\n"
                                       "with a intensity\n"
                                       "mystical fire. It\n"
                                       "may burn the foe."),
        .animScript = gMoveAnim_SACRED_FIRE,
		.effect = EFFECT_HIT,
        .power = 100,
        .type = TYPE_FIRE,
        .accuracy = 95,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.thawUser = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_BURN,
			.chance = 50,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MAGNITUDE] =
    {
		.name = _("Magnitude"),
		.description = COMPOUND_STRING("A ground-shaking\n"
                                       "attack against all\n"
                                       "standing Pokémon.\n"
                                       "Its power varies."),
        .animScript = gMoveAnim_MAGNITUDE,
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
		.description = COMPOUND_STRING("The foe is punched\n"
                                       "with the user's\n"
                                       "full power. It\n"
                                       "confuses the foe."),
        .animScript = gMoveAnim_DYNAMIC_PUNCH,
		.effect = EFFECT_HIT,
        .power = 100,
        .type = TYPE_FIGHTING,
        .accuracy = 50,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.punchMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_CONFUSION,
			.chance = 100,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MEGAHORN] =
    {
		.name = _("Megahorn"),
		.description = COMPOUND_STRING("Using its tough\n"
                                       "horn, the user rams\n"
                                       "into the target\n"
                                       "with no letup."),
        .animScript = gMoveAnim_MEGAHORN,
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
		.description = COMPOUND_STRING("The foe is hit with\n"
                                       "an incredible blast\n"
                                       "of breath that may\n"
                                       "also paralyze."),
        .animScript = gMoveAnim_DRAGON_BREATH,
		.effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_DRAGON,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_PARALYSIS,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BATON_PASS] =
    {
		.name = _("Baton Pass"),
		.description = COMPOUND_STRING("The user switches\n"
                                       "out, passing along\n"
                                       "any stat changes to\n"
                                       "the new battler."),
        .animScript = gMoveAnim_BATON_PASS,
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
		.description = COMPOUND_STRING("It compels the foe\n"
                                       "to keep using the\n"
                                       "move it encored for\n"
                                       "three turns."),
        .animScript = gMoveAnim_ENCORE,
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
		.description = COMPOUND_STRING("An attack move that\n"
                                       "works especially\n"
                                       "well on a foe that\n"
                                       "is switching out."),
        .animScript = gMoveAnim_PURSUIT,
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
		.description = COMPOUND_STRING("A spin attack that\n"
                                       "removes trapping\n"
                                       "status. Also boosts\n"
                                       "its Speed stat."),
        .animScript = gMoveAnim_RAPID_SPIN,
		.effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_CLEAR_HAZARDS,
			.chance = 0,
			.self = TRUE,
		},{
			.moveEffect = MOVE_EFFECT_SPD_PLUS_1,
			.chance = 100,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SWEET_SCENT] =
    {
		.name = _("Sweet Scent"),
		.description = COMPOUND_STRING("Releases a scent\n"
                                       "that harshly lowers\n"
                                       "opposing Pokémon's\n"
                                       "evasiveness."),
        .animScript = gMoveAnim_SWEET_SCENT,
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
		.description = COMPOUND_STRING("The foe is slammed\n"
                                       "with a steel tail.\n"
                                       "It may also lower\n"
                                       "the foe's Defense."),
        .animScript = gMoveAnim_IRON_TAIL,
		.effect = EFFECT_HIT,
        .power = 100,
        .type = TYPE_STEEL,
        .accuracy = 75,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_DEF_MINUS_1,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_METAL_CLAW] =
    {
		.name = _("Metal Claw"),
		.description = COMPOUND_STRING("The foe is raked\n"
                                       "with steel claws.\n"
                                       "This may also raise\n"
                                       "the user's Attack."),
        .animScript = gMoveAnim_METAL_CLAW,
		.effect = EFFECT_HIT,
        .power = 50,
        .type = TYPE_STEEL,
        .accuracy = 95,
        .pp = 35,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_ATK_PLUS_1,
			.chance = 10,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_VITAL_THROW] =
    {
		.name = _("Vital Throw"),
		.description = COMPOUND_STRING("The user attacks\n"
                                       "last. In return,\n"
                                       "this throw move\n"
                                       "never misses."),
        .animScript = gMoveAnim_VITAL_THROW,
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
		.description = COMPOUND_STRING("Restores the user's\n"
                                       "HP. The amount of\n"
                                       "HP regained varies\n"
                                       "with the weather."),
        .animScript = gMoveAnim_MORNING_SUN,
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
		.name = _("Synthesis"),
		.description = COMPOUND_STRING("Restores the user's\n"
                                       "HP. The amount of\n"
                                       "HP regained varies\n"
                                       "with the weather."),
        .animScript = gMoveAnim_SYNTHESIS,
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
		.name = _("Moonlight"),
		.description = COMPOUND_STRING("Restores the user's\n"
                                       "HP. The amount of\n"
                                       "HP regained varies\n"
                                       "with the weather."),
        .animScript = gMoveAnim_MOONLIGHT,
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
		.name = _("Hidden Power"),
		.description = COMPOUND_STRING("A unique attack\n"
                                       "that varies in type\n"
                                       "depending on the\n"
                                       "Pokémon using it."),
        .animScript = gMoveAnim_HIDDEN_POWER,
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
		.description = COMPOUND_STRING("The foe is hit with\n"
                                       "double chops. It\n"
                                       "has a high\n"
                                       "critical-hit ratio."),
        .animScript = gMoveAnim_CROSS_CHOP,
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
			.critStage = 1,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_TWISTER] =
    {
		.name = _("Twister"),
		.description = COMPOUND_STRING("Whips up a twister\n"
                                       "to tear at the\n"
                                       "foes. It may also\n"
                                       "make they flinch."),
        .animScript = gMoveAnim_TWISTER,
		.effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_DRAGON,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_BOTH,
		.flags =
		{
			.hitInAirDoubleDmg = TRUE,
			.windMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 20,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_RAIN_DANCE] =
    {
		.name = _("Rain Dance"),
		.description = COMPOUND_STRING("A heavy rain falls\n"
                                       "for five turns,\n"
                                       "powering up Water\n"
                                       "type moves."),
        .animScript = gMoveAnim_RAIN_DANCE,
		.effect = EFFECT_SET_WEATHER,
        .type = TYPE_WATER,
        .pp = 5,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
		.argument = {
			.setWeather = {
				.weatherId = ENUM_WEATHER_RAIN,
				.stringId = B_MSG_STARTED_RAIN,
			},
		},
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_SUNNY_DAY] =
    {
		.name = _("Sunny Day"),
		.description = COMPOUND_STRING("It intensifies the\n"
                                       "sun for five turns,\n"
                                       "powering up Fire\n"
                                       "type moves."),
        .animScript = gMoveAnim_SUNNY_DAY,
		.effect = EFFECT_SET_WEATHER,
        .type = TYPE_FIRE,
        .pp = 5,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
		.argument = {
			.setWeather = {
				.weatherId = ENUM_WEATHER_SUN,
				.stringId = B_MSG_SUN_TURN_HARSH,
			},
		},
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_CRUNCH] =
    {
		.name = _("Crunch"),
		.description = COMPOUND_STRING("Crunches up the foe\n"
                                       "with sharp fangs.\n"
                                       "It may also lower\n"
                                       "the foe's Defense."),
        .animScript = gMoveAnim_CRUNCH,
		.effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.bitingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_DEF_MINUS_1,
			.chance = 20,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MIRROR_COAT] =
    {
		.name = _("Mirror Coat"),
		.description = COMPOUND_STRING("Counters any\n"
                                       "special attack,\n"
                                       "inflicting double\n"
                                       "the damage taken."),
        .animScript = gMoveAnim_MIRROR_COAT,
		.effect = EFFECT_COUNTER_ATTACK,
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
			.forbiddenMeFirst = TRUE,
		},
		.argument = {
			.counterAttack = { .split = SPLIT_SPECIAL },
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PSYCH_UP] =
    {
		.name = _("Psych Up"),
		.description = COMPOUND_STRING("The user hypnotizes\n"
                                       "itself into copying\n"
                                       "any stat change\n"
                                       "made by the foe."),
        .animScript = gMoveAnim_PSYCH_UP,
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
		.description = COMPOUND_STRING("The user charges\n"
                                       "the foe at blinding\n"
                                       "speed. This move\n"
                                       "always goes first."),
        .animScript = gMoveAnim_EXTREME_SPEED,
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
		.description = COMPOUND_STRING("An ancient power is\n"
                                       "used to attack. It\n"
                                       "may also raise all\n"
                                       "the user's stats."),
        .animScript = gMoveAnim_ANCIENT_POWER,
		.effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_ROCK,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_ALL_STATS_UP,
			.chance = 10,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHADOW_BALL] =
    {
		.name = _("Shadow Ball"),
		.description = COMPOUND_STRING("A shadowy blob is\n"
                                       "hurled at the foe.\n"
                                       "May also lower the\n"
                                       "foe's Sp. Def."),
        .animScript = gMoveAnim_SHADOW_BALL,
		.effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.ballisticMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_DEF_MINUS_1,
			.chance = 20,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FUTURE_SIGHT] =
    {
		.name = _("Future Sight"),
		.description = COMPOUND_STRING("Two turns after\n"
                                       "this move is used,\n"
                                       "the foe is attacked\n"
                                       "psychically."),
        .animScript = gMoveAnim_FUTURE_SIGHT,
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
		.argument = {
			.futureAttack = {
				.stringId = B_MSG_FORESAW_ATTACK,
				.animationId = B_ANIM_FUTURE_SIGHT_HIT,
			},
		},
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ROCK_SMASH] =
    {
		.name = _("Rock Smash"),
		.description = COMPOUND_STRING("The user attacks\n"
                                       "with a punch. This\n"
                                       "may also lower the\n"
                                       "foe's Defense stat."),
        .animScript = gMoveAnim_ROCK_SMASH,
		.effect = EFFECT_HIT,
		.power = 40,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_DEF_MINUS_1,
			.chance = 50,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WHIRLPOOL] =
    {
		.name = _("Whirlpool"),
		.description = COMPOUND_STRING("The user traps the\n"
                                       "foe in a violent\n"
                                       "whirlpool for four\n"
                                       "to five turns."),
        .animScript = gMoveAnim_WHIRLPOOL,
		.effect = EFFECT_HIT,
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
		.argument = {
			.bind = { .trappingId = TRAP_ID_WHIRLPOOL },
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_WRAP,
			.chance = 0,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BEAT_UP] =
    {
		.name = _("Beat Up"),
		.description = COMPOUND_STRING("All party Pokémon\n"
                                       "join in the attack.\n"
                                       "The more allies,\n"
                                       "the more damage."),
        .animScript = gMoveAnim_BEAT_UP,
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
		.description = COMPOUND_STRING("An attack that hits\n"
                                       "first and causes\n"
                                       "flinching. Usable\n"
                                       "only on 1st turn."),
        .animScript = gMoveAnim_FAKE_OUT,
		.effect = EFFECT_FAKE_OUT,
        .power = 40,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.priority = 3,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 100,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_UPROAR] =
    {
		.name = _("Uproar"),
		.description = COMPOUND_STRING("The user attacks in\n"
                                       "an uproar that\n"
                                       "prevents sleep for\n"
                                       "two to five turns."),
        .animScript = gMoveAnim_UPROAR,
		.effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_UPROAR,
			.chance = 0,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STOCKPILE] =
    {
		.name = _("Stockpile"),
		.description = COMPOUND_STRING("Charges power and\n"
                                       "raises its Defense\n"
                                       "and Sp. Def. It can\n"
                                       "be used three times."),
        .animScript = gMoveAnim_STOCKPILE,
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
		.description = COMPOUND_STRING("The power stored\n"
                                       "using Stockpile is\n"
                                       "released at once in\n"
                                       "an attack."),
        .animScript = gMoveAnim_SPIT_UP,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_STOCKPILE_WORE_OFF,
			.chance = 0,
			.self = TRUE,
			.onFinalMultiHitOnly = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SWALLOW] =
    {
		.name = _("Swallow"),
		.description = COMPOUND_STRING("The power stored\n"
                                       "using Stockpile is\n"
                                       "absorbed by the\n"
                                       "user to heal its HP."),
        .animScript = gMoveAnim_SWALLOW,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_STOCKPILE_WORE_OFF,
			.chance = 0,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_RESET_STATS,
    },

    [MOVE_HEAT_WAVE] =
    {
		.name = _("Heat Wave"),
		.description = COMPOUND_STRING("Exhales a hot\n"
                                       "breath on the foes.\n"
                                       "It may leave those\n"
                                       "with a burn."),
        .animScript = gMoveAnim_HEAT_WAVE,
		.effect = EFFECT_HIT,
		.power = 95,
        .type = TYPE_FIRE,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.windMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_BURN,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HAIL] =
    {
		.name = _("Hail"),
		.description = COMPOUND_STRING("A hailstorm lasting\n"
                                       "five turns damages\n"
                                       "all Pokémon except\n"
                                       "the Ice type."),
        .animScript = gMoveAnim_HAIL,
		.effect = EFFECT_SET_WEATHER,
        .type = TYPE_ICE,
        .pp = 10,
        .target = MOVE_TARGET_ALL_BATTLERS,
		.flags =
		{
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_STATUS,
		.argument = {
			.setWeather = {
				.weatherId = ENUM_WEATHER_HAIL,
				.stringId = B_MSG_STARTED_HAIL,
			},
		},
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_TORMENT] =
    {
		.name = _("Torment"),
		.description = COMPOUND_STRING("It enrages the foe,\n"
                                       "making it incapable\n"
                                       "of using the same\n"
                                       "move successively."),
        .animScript = gMoveAnim_TORMENT,
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
		.description = COMPOUND_STRING("Flattery is used to\n"
                                       "confuse the foe.\n"
                                       "But, it also boosts\n"
                                       "the foe's Sp. Atk."),
        .animScript = gMoveAnim_FLATTER,
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
		.description = COMPOUND_STRING("The user shoots a\n"
                                       "sinister flame at\n"
                                       "the target to\n"
                                       "inflict a burn."),
        .animScript = gMoveAnim_WILL_O_WISP,
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
		.description = COMPOUND_STRING("The user faints. In\n"
                                       "return, the foe's\n"
                                       "Attack and Sp. Atk\n"
                                       "are harshly lowered."),
        .animScript = gMoveAnim_MEMENTO,
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
		.description = COMPOUND_STRING("This move's power\n"
                                       "is doubled if the\n"
                                       "user is paralyzed,\n"
                                       "burned, or poisoned."),
        .animScript = gMoveAnim_FACADE,
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
		.description = COMPOUND_STRING("An attack that is\n"
                                       "executed last. The\n"
                                       "user flinches if\n"
                                       "hit beforehand."),
        .animScript = gMoveAnim_FOCUS_PUNCH,
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
		.description = COMPOUND_STRING("Doubly effective on\n"
                                       "a paralyzed foe,\n"
                                       "but it also cures\n"
                                       "the foe's paralysis."),
        .animScript = gMoveAnim_SMELLING_SALT,
		.effect = EFFECT_CURE_STATUS1_FROM_ARG,
		.power = 70,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
		.argument = {
			.cureStatus = { .statusId = STATUS1_PARALYSIS },
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_CURE_STATUS1,
			.chance = 0,
			.onFinalMultiHitOnly = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FOLLOW_ME] =
    {
		.name = _("Follow Me"),
		.description = COMPOUND_STRING("It draws attention\n"
                                       "to itself, making\n"
                                       "all foes take aim\n"
                                       "only at the user."),
        .animScript = gMoveAnim_FOLLOW_ME,
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
		.description = COMPOUND_STRING("An attack that\n"
                                       "changes type\n"
                                       "depending on the\n"
                                       "user's location."),
        .animScript = gMoveAnim_NONE,
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
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CHARGE] =
    {
		.name = _("Charge"),
		.description = COMPOUND_STRING("Boosts the power of\n"
                                       "the next Electric\n"
                                       "move used. Also\n"
                                       "raises its Sp. Def."),
        .animScript = gMoveAnim_CHARGE,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_DEF_PLUS_1,
			.chance = 0,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_SPDEF_UP_1,
    },

    [MOVE_TAUNT] =
    {
		.name = _("Taunt"),
		.description = COMPOUND_STRING("The foe is taunted\n"
                                       "into a rage that\n"
                                       "allows it to use\n"
                                       "only attack moves."),
        .animScript = gMoveAnim_TAUNT,
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
		.description = COMPOUND_STRING("The user assists an\n"
                                       "ally by boosting\n"
                                       "the power of that\n"
                                       "ally's attack."),
        .animScript = gMoveAnim_HELPING_HAND,
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
		.description = COMPOUND_STRING("The user catches\n"
                                       "the foe off guard\n"
                                       "and swaps its held\n"
                                       "item with its own."),
        .animScript = gMoveAnim_TRICK,
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
		.description = COMPOUND_STRING("The user mimics the\n"
                                       "target completely,\n"
                                       "copying the\n"
                                       "target's Ability."),
        .animScript = gMoveAnim_ROLE_PLAY,
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
		.description = COMPOUND_STRING("A self-healing move\n"
                                       "that restores half\n"
                                       "the full HP on the\n"
                                       "next turn."),
        .animScript = gMoveAnim_WISH,
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
		.name = _("Assist"),
		.description = COMPOUND_STRING("The user randomly\n"
                                       "picks and uses a\n"
                                       "move of an allied\n"
                                       "Pokémon."),
        .animScript = gMoveAnim_ASSIST,
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
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_INGRAIN] =
    {
		.name = _("Ingrain"),
		.description = COMPOUND_STRING("The user lays roots\n"
                                       "that restore HP on\n"
                                       "every turn. It\n"
                                       "can't switch out."),
        .animScript = gMoveAnim_INGRAIN,
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
		.description = COMPOUND_STRING("A powerful attack,\n"
                                       "but it also lowers\n"
                                       "the user's Attack\n"
                                       "and Defense stats."),
        .animScript = gMoveAnim_SUPERPOWER,
		.effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_ATK_DEF_DOWN,
			.chance = 0,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MAGIC_COAT] =
    {
		.name = _("Magic Coat"),
		.description = COMPOUND_STRING("Reflects back moves\n"
                                       "like Leech Seed and\n"
                                       "moves that damage\n"
                                       "status."),
        .animScript = gMoveAnim_MAGIC_COAT,
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
		.description = COMPOUND_STRING("Recycles a held\n"
                                       "item that has been\n"
                                       "used so it can be\n"
                                       "used again."),
        .animScript = gMoveAnim_RECYCLE,
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
		.description = COMPOUND_STRING("An attack move that\n"
                                       "gains in intensity\n"
                                       "if the target has\n"
                                       "hurt the user."),
        .animScript = gMoveAnim_REVENGE,
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
		.description = COMPOUND_STRING("An attack that also\n"
                                       "breaks any barrier\n"
                                       "like Light Screen\n"
                                       "and Reflect."),
        .animScript = gMoveAnim_BRICK_BREAK,
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
		.description = COMPOUND_STRING("A huge yawn lulls\n"
                                       "the foe into\n"
                                       "falling asleep on\n"
                                       "the next turn."),
        .animScript = gMoveAnim_YAWN,
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
		.description = COMPOUND_STRING("Knocks down the\n"
                                       "foe's held item to\n"
                                       "prevent its use\n"
                                       "during the battle."),
        .animScript = gMoveAnim_KNOCK_OFF,
		.effect = EFFECT_KNOCK_OFF,
		.power = 65,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_KNOCK_OFF,
			.chance = 0,
			.afterTargetItemsOnly = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ENDEAVOR] =
    {
		.name = _("Endeavor"),
		.description = COMPOUND_STRING("This attack move\n"
                                       "cuts down the\n"
                                       "target's HP to\n"
                                       "equal the user's HP."),
        .animScript = gMoveAnim_ENDEAVOR,
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
		.name = _("Eruption"),
		.description = COMPOUND_STRING("The higher the\n"
                                       "user's HP, the more\n"
                                       "powerful this\n"
                                       "attack becomes."),
        .animScript = gMoveAnim_ERUPTION,
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
		.description = COMPOUND_STRING("Employs its psychic\n"
                                       "power to exchange\n"
                                       "Abilities with the\n"
                                       "target."),
        .animScript = gMoveAnim_SKILL_SWAP,
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
		.description = COMPOUND_STRING("Prevents foes from\n"
                                       "using any move that\n"
                                       "is also known by\n"
                                       "the user."),
        .animScript = gMoveAnim_IMPRISON,
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
		.description = COMPOUND_STRING("The user rests to\n"
                                       "cure itself of\n"
                                       "poisoning, a burn,\n"
                                       "or paralysis."),
        .animScript = gMoveAnim_REFRESH,
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
		.description = COMPOUND_STRING("If the user faints,\n"
                                       "this move deletes\n"
                                       "the PP of the move\n"
                                       "that finished it."),
        .animScript = gMoveAnim_GRUDGE,
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
		.description = COMPOUND_STRING("Steals the effects\n"
                                       "of the foe's\n"
                                       "status-changing or\n"
                                       "healing move."),
        .animScript = gMoveAnim_SNATCH,
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
			.forcePressure = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_2,
    },

    [MOVE_SECRET_POWER] =
    {
		.name = _("Secret Power"),
		.description = COMPOUND_STRING("An attack that may\n"
                                       "have an additional\n"
                                       "effect that varies\n"
                                       "with the terrain."),
        .animScript = gMoveAnim_SECRET_POWER,
		.effect = EFFECT_HIT,
        .power = 70,
        .type = TYPE_NORMAL,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SECRET_POWER,
			.chance = 30,
			.onFinalMultiHitOnly = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DIVE] =
    {
		.name = _("Dive"),
		.description = COMPOUND_STRING("The user dives\n"
                                       "underwater on the\n"
                                       "first turn and\n"
                                       "strikes next turn."),
        .animScript = gMoveAnim_DIVE,
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
		},
		.argument = {
			.semiInvulnerable = {
				.stringId = B_MSG_HID_UNDERWATER,
				.status = COMPRESS_BITS(STATUS3_UNDERWATER),
			},
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ARM_THRUST] =
    {
		.name = _("Arm Thrust"),
		.description = COMPOUND_STRING("A quick flurry of\n"
                                       "straight-arm\n"
                                       "punches that hit\n"
                                       "two to five times."),
        .animScript = gMoveAnim_ARM_THRUST,
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
		.description = COMPOUND_STRING("The user's type is\n"
                                       "changed depending\n"
                                       "on its environment."),
        .animScript = gMoveAnim_CAMOUFLAGE,
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
		.description = COMPOUND_STRING("The user stares at\n"
                                       "flashing lights\n"
                                       "that drastically\n"
                                       "raises its Sp. Atk."),
        .animScript = gMoveAnim_TAIL_GLOW,
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
		.description = COMPOUND_STRING("A burst of light\n"
                                       "injures the foe. It\n"
                                       "may also lower the\n"
                                       "foe's Sp. Def."),
        .animScript = gMoveAnim_LUSTER_PURGE,
		.effect = EFFECT_HIT,
        .power = 95,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_DEF_MINUS_1,
			.chance = 50,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MIST_BALL] =
    {
		.name = _("Mist Ball"),
		.description = COMPOUND_STRING("A flurry of down\n"
                                       "hits the foe. It\n"
                                       "may also lower the\n"
                                       "foe's Sp. Atk."),
        .animScript = gMoveAnim_MIST_BALL,
		.effect = EFFECT_HIT,
        .power = 95,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.ballisticMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_ATK_MINUS_1,
			.chance = 50,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FEATHER_DANCE] =
    {
		.name = _("Feather Dance"),
		.description = COMPOUND_STRING("The foe is covered\n"
                                       "with a mass of down\n"
                                       "that sharply cuts\n"
                                       "the Attack stat."),
        .animScript = gMoveAnim_FEATHER_DANCE,
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
		.description = COMPOUND_STRING("The user performs a\n"
                                       "wobbly dance that\n"
                                       "confuses every\n"
                                       "Pokémon around it."),
        .animScript = gMoveAnim_TEETER_DANCE,
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
		.description = COMPOUND_STRING("A fiery kick with a\n"
                                       "high critical-hit\n"
                                       "ratio. It may also\n"
                                       "burn the foe."),
        .animScript = gMoveAnim_BLAZE_KICK,
		.effect = EFFECT_HIT,
        .power = 85,
        .type = TYPE_FIRE,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.critStage = 1,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_BURN,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MUD_SPORT] =
    {
		.name = _("Mud Sport"),
		.description = COMPOUND_STRING("Kicks up mud on the\n"
                                       "field. This weakens\n"
                                       "Electric type moves\n"
                                       "for five turns."),
        .animScript = gMoveAnim_MUD_SPORT,
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
		.description = COMPOUND_STRING("Attacks the foe for\n"
                                       "five turns. It\n"
                                       "becomes stronger\n"
                                       "each time it hits."),
        .animScript = gMoveAnim_ICE_BALL,
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
		.name = _("Needle Arm"),
		.description = COMPOUND_STRING("An attack using\n"
                                       "thorny arms. It may\n"
                                       "make the foe flinch."),
        .animScript = gMoveAnim_NEEDLE_ARM,
		.effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SLACK_OFF] =
    {
		.name = _("Slack Off"),
		.description = COMPOUND_STRING("The user slacks\n"
                                       "off, restoring its\n"
                                       "own HP by up to\n"
                                       "half its max HP."),
        .animScript = gMoveAnim_SLACK_OFF,
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

    [MOVE_HYPER_VOICE] =
    {
		.name = _("Hyper Voice"),
		.description = COMPOUND_STRING("The user attacks by\n"
                                       "letting loose a\n"
                                       "horribly loud,\n"
                                       "resounding cry."),
        .animScript = gMoveAnim_HYPER_VOICE,
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
		.description = COMPOUND_STRING("The foe is bitten\n"
                                       "with toxic fangs.\n"
                                       "It may also badly\n"
                                       "poison the foe."),
        .animScript = gMoveAnim_POISON_FANG,
		.effect = EFFECT_HIT,
        .power = 50,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.bitingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_TOXIC,
			.chance = 50,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CRUSH_CLAW] =
    {
		.name = _("Crush Claw"),
		.description = COMPOUND_STRING("The foe is attacked\n"
                                       "with sharp claws.\n"
                                       "It may also lower\n"
                                       "the foe's Defense."),
        .animScript = gMoveAnim_CRUSH_CLAW,
		.effect = EFFECT_HIT,
        .power = 75,
        .type = TYPE_NORMAL,
        .accuracy = 95,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_DEF_MINUS_1,
			.chance = 50,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BLAST_BURN] =
    {
		.name = _("Blast Burn"),
		.description = COMPOUND_STRING("The foe is razed by\n"
                                       "a fiery explosion.\n"
                                       "The user can't move\n"
                                       "on the next turn."),
        .animScript = gMoveAnim_BLAST_BURN,
		.effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_RECHARGE,
			.chance = 0,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HYDRO_CANNON] =
    {
		.name = _("Hydro Cannon"),
		.description = COMPOUND_STRING("The foe is hit with\n"
                                       "a watery blast. The\n"
                                       "user can't move on\n"
                                       "the next turn."),
        .animScript = gMoveAnim_HYDRO_CANNON,
		.effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_RECHARGE,
			.chance = 0,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_METEOR_MASH] =
    {
		.name = _("Meteor Mash"),
		.description = COMPOUND_STRING("The foe is hit with\n"
                                       "a hard, fast punch.\n"
                                       "It may also raise\n"
                                       "the user's Attack."),
        .animScript = gMoveAnim_METEOR_MASH,
		.effect = EFFECT_HIT,
		.power = 90,
        .type = TYPE_STEEL,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_ATK_PLUS_1,
			.chance = 20,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ASTONISH] =
    {
		.name = _("Astonish"),
		.description = COMPOUND_STRING("An attack using a\n"
                                       "startling shout. It\n"
                                       "also may make the\n"
                                       "foe flinch."),
        .animScript = gMoveAnim_ASTONISH,
		.effect = EFFECT_HIT,
        .power = 30,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_WEATHER_BALL] =
    {
		.name = _("Weather Ball"),
		.description = COMPOUND_STRING("This attack move\n"
                                       "varies in power and\n"
                                       "type depending on\n"
                                       "the weather."),
        .animScript = gMoveAnim_WEATHER_BALL,
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
		.description = COMPOUND_STRING("A soothing scent is\n"
                                       "released to heal\n"
                                       "all status problems\n"
                                       "in the user's party."),
        .animScript = gMoveAnim_AROMATHERAPY,
		.effect = EFFECT_HEAL_BELL,
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
        .zMoveEffect = Z_EFFECT_RECOVER_HP,
    },

    [MOVE_FAKE_TEARS] =
    {
		.name = _("Fake Tears"),
		.description = COMPOUND_STRING("The user feigns\n"
                                       "crying to sharply\n"
                                       "lower the target's\n"
                                       "Sp. Def."),
        .animScript = gMoveAnim_FAKE_TEARS,
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
		.description = COMPOUND_STRING("Launches razorlike\n"
                                       "wind to slash the\n"
                                       "foes. It has a high\n"
                                       "critical-hit ratio."),
        .animScript = gMoveAnim_AIR_CUTTER,
		.effect = EFFECT_HIT,
		.power = 60,
        .type = TYPE_FLYING,
        .accuracy = 95,
        .pp = 25,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.critStage = 1,
			.slicingMove = TRUE,
			.windMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_OVERHEAT] =
    {
		.name = _("Overheat"),
		.description = COMPOUND_STRING("An intense attack\n"
                                       "that also sharply\n"
                                       "reduces the user's\n"
                                       "Sp. Atk stat."),
        .animScript = gMoveAnim_OVERHEAT,
		.effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_ATK_MINUS_2,
			.chance = 0,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ODOR_SLEUTH] =
    {
		.name = _("Odor Sleuth"),
		.description = COMPOUND_STRING("Completely negates\n"
                                       "the foe's efforts\n"
                                       "to heighten its\n"
                                       "ability to evade."),
        .animScript = gMoveAnim_ODOR_SLEUTH,
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
		.description = COMPOUND_STRING("Boulders are hurled\n"
                                       "at the foe. It also\n"
                                       "lowers the foe's\n"
                                       "Speed if it hits."),
        .animScript = gMoveAnim_ROCK_TOMB,
		.effect = EFFECT_HIT,
		.power = 60,
        .type = TYPE_ROCK,
		.accuracy = 95,
		.pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SPD_MINUS_1,
			.chance = 100,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SILVER_WIND] =
    {
		.name = _("Silver Wind"),
		.description = COMPOUND_STRING("The foe is attacked\n"
                                       "with a silver dust.\n"
                                       "It may raise all\n"
                                       "the user's stats."),
        .animScript = gMoveAnim_SILVER_WIND,
		.effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_ALL_STATS_UP,
			.chance = 10,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_METAL_SOUND] =
    {
		.name = _("Metal Sound"),
		.description = COMPOUND_STRING("A horrible sound\n"
                                       "like scraping metal\n"
                                       "harshly lowers the\n"
                                       "foe's Sp. Def stat."),
        .animScript = gMoveAnim_METAL_SOUND,
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
		.description = COMPOUND_STRING("The user plays a\n"
                                       "pleasant melody\n"
                                       "that lulls the foe\n"
                                       "into a deep sleep."),
        .animScript = gMoveAnim_GRASS_WHISTLE,
		.effect = EFFECT_SLEEP,
        .type = TYPE_GRASS,
        .accuracy = 55,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.magicCoatAffected = TRUE,
			.soundMove = TRUE,
			.hasQuietBGM = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_TICKLE] =
    {
		.name = _("Tickle"),
		.description = COMPOUND_STRING("The foe is made to\n"
                                       "laugh, reducing its\n"
                                       "Attack and Defense\n"
                                       "stats."),
        .animScript = gMoveAnim_TICKLE,
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
		.description = COMPOUND_STRING("The user absorbs a\n"
                                       "mystic power to\n"
                                       "raise its Defense\n"
                                       "and Sp. Def."),
        .animScript = gMoveAnim_COSMIC_POWER,
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
		.description = COMPOUND_STRING("The higher the\n"
                                       "user's HP, the more\n"
                                       "powerful this\n"
                                       "attack becomes."),
        .animScript = gMoveAnim_WATER_SPOUT,
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
		.description = COMPOUND_STRING("It attacks with a\n"
                                       "sinister beam of\n"
                                       "light. It may also\n"
                                       "confuse the target."),
        .animScript = gMoveAnim_SIGNAL_BEAM,
		.effect = EFFECT_HIT,
        .power = 75,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_CONFUSION,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHADOW_PUNCH] =
    {
		.name = _("Shadow Punch"),
		.description = COMPOUND_STRING("The user throws a\n"
                                       "punch from the\n"
                                       "shadows. This\n"
                                       "attack never misses."),
        .animScript = gMoveAnim_SHADOW_PUNCH,
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
		.description = COMPOUND_STRING("The user attacks\n"
                                       "with an odd power\n"
                                       "that may make the\n"
                                       "foe flinch."),
        .animScript = gMoveAnim_EXTRASENSORY,
		.effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
		.pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SKY_UPPERCUT] =
    {
		.name = _("Sky Uppercut"),
		.description = COMPOUND_STRING("The user attacks\n"
                                       "the target with an\n"
                                       "uppercut thrown\n"
                                       "skyward with force."),
        .animScript = gMoveAnim_SKY_UPPERCUT,
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
		.description = COMPOUND_STRING("The user traps the\n"
                                       "foe inside a raging\n"
                                       "sandstorm for four\n"
                                       "to five turns."),
        .animScript = gMoveAnim_SAND_TOMB,
		.effect = EFFECT_HIT,
		.power = 35,
        .type = TYPE_GROUND,
		.accuracy = 85,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
		.argument = {
			.bind = { .trappingId = TRAP_ID_SAND_TOMB },
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_WRAP,
			.chance = 0,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SHEER_COLD] =
    {
		.name = _("Sheer Cold"),
		.description = COMPOUND_STRING("The foe is attacked\n"
                                       "with ultimate cold\n"
                                       "that causes\n"
                                       "fainting if it hits."),
        .animScript = gMoveAnim_SHEER_COLD,
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
		.description = COMPOUND_STRING("The user attacks\n"
                                       "with muddy water.\n"
                                       "It may also lower\n"
                                       "the foe's accuracy."),
        .animScript = gMoveAnim_MUDDY_WATER,
		.effect = EFFECT_HIT,
		.power = 90,
        .type = TYPE_WATER,
        .accuracy = 85,
        .pp = 10,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_ACC_MINUS_1,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BULLET_SEED] =
    {
		.name = _("Bullet Seed"),
		.description = COMPOUND_STRING("The user forcefully\n"
                                       "shoots seeds at the\n"
                                       "target two to five\n"
                                       "times in a row."),
        .animScript = gMoveAnim_BULLET_SEED,
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
		.description = COMPOUND_STRING("The user confounds\n"
                                       "the foe with speed,\n"
                                       "then slashes. This\n"
                                       "attack never misses."),
        .animScript = gMoveAnim_AERIAL_ACE,
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
		.description = COMPOUND_STRING("The user launches\n"
                                       "sharp icicles at\n"
                                       "the target two to\n"
                                       "five times in a row."),
        .animScript = gMoveAnim_ICICLE_SPEAR,
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
		.description = COMPOUND_STRING("It hardens its\n"
                                       "body's surface like\n"
                                       "iron, sharply\n"
                                       "raising its Defense."),
        .animScript = gMoveAnim_IRON_DEFENSE,
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
		.description = COMPOUND_STRING("The user blocks the\n"
                                       "target's way with\n"
                                       "arms spread wide to\n"
                                       "prevent escape."),
        .animScript = gMoveAnim_BLOCK,
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
		.description = COMPOUND_STRING("Howls loudly to\n"
                                       "rouse itself and\n"
                                       "its allies. This\n"
                                       "boosts their Attack."),
        .animScript = gMoveAnim_HOWL,
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
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ATK_UP_1,
    },

    [MOVE_DRAGON_CLAW] =
    {
		.name = _("Dragon Claw"),
		.description = COMPOUND_STRING("The user slashes\n"
                                       "the target with\n"
                                       "huge, sharp claws\n"
                                       "to inflict damage."),
        .animScript = gMoveAnim_DRAGON_CLAW,
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
		.description = COMPOUND_STRING("The foe is hit with\n"
                                       "an enormous branch.\n"
                                       "The user can't move\n"
                                       "on the next turn."),
        .animScript = gMoveAnim_FRENZY_PLANT,
		.effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_RECHARGE,
			.chance = 0,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BULK_UP] =
    {
		.name = _("Bulk Up"),
		.description = COMPOUND_STRING("The user bulks up\n"
                                       "its body to boost\n"
                                       "both its Attack and\n"
                                       "Defense stats."),
        .animScript = gMoveAnim_BULK_UP,
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
		.description = COMPOUND_STRING("The user bounces on\n"
                                       "the foe on the 2nd\n"
                                       "turn. It may\n"
                                       "paralyze the foe."),
        .animScript = gMoveAnim_BOUNCE,
		.effect = EFFECT_SEMI_INVULNERABLE,
        .power = 85,
        .type = TYPE_FLYING,
        .accuracy = 85,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.gravityBanned = TRUE,
		},
		.argument = {
			.semiInvulnerable = {
				.stringId = B_MSG_SPRANG_UP,
				.status = COMPRESS_BITS(STATUS3_ON_AIR),
			},
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_PARALYSIS,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MUD_SHOT] =
    {
		.name = _("Mud Shot"),
		.description = COMPOUND_STRING("The user attacks by\n"
                                       "hurling mud. It\n"
                                       "also reduces the\n"
                                       "foe's Speed."),
        .animScript = gMoveAnim_MUD_SHOT,
		.effect = EFFECT_HIT,
        .power = 55,
        .type = TYPE_GROUND,
        .accuracy = 95,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SPD_MINUS_1,
			.chance = 100,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_POISON_TAIL] =
    {
		.name = _("Poison Tail"),
		.description = COMPOUND_STRING("An attack with a\n"
                                       "high critical-hit\n"
                                       "ratio. It may also\n"
                                       "poison the foe."),
        .animScript = gMoveAnim_POISON_TAIL,
		.effect = EFFECT_HIT,
        .power = 50,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 25,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.critStage = 1,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_POISON,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_COVET] =
    {
		.name = _("Covet"),
		.description = COMPOUND_STRING("Endearingly\n"
                                       "approaches the foe,\n"
                                       "then steals the\n"
                                       "foe's held item."),
        .animScript = gMoveAnim_COVET,
		.effect = EFFECT_HIT,
		.power = 60,
        .type = TYPE_NORMAL,
        .accuracy = 100,
		.pp = 25,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_STEAL_ITEM,
			.chance = 0,
			.onFinalMultiHitOnly = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_VOLT_TACKLE] =
    {
		.name = _("Volt Tackle"),
		.description = COMPOUND_STRING("The user throws an\n"
                                       "electrified tackle.\n"
                                       "It hurts the user a\n"
                                       "little."),
        .animScript = gMoveAnim_VOLT_TACKLE,
		.effect = EFFECT_HIT,
        .power = 120,
        .type = TYPE_ELECTRIC,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_PARALYSIS,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MAGICAL_LEAF] =
    {
		.name = _("Magical Leaf"),
		.description = COMPOUND_STRING("The foe is attacked\n"
                                       "with a strange leaf\n"
                                       "that cannot be\n"
                                       "evaded."),
        .animScript = gMoveAnim_MAGICAL_LEAF,
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
		.description = COMPOUND_STRING("It soaks the field\n"
                                       "with water. This\n"
                                       "weakens Fire type\n"
                                       "moves for 5 turns."),
        .animScript = gMoveAnim_WATER_SPORT,
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
		.description = COMPOUND_STRING("It focuses its mind\n"
                                       "and calms its\n"
                                       "spirit to raise its\n"
                                       "Sp. Atk and Sp. Def."),
        .animScript = gMoveAnim_CALM_MIND,
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
		.description = COMPOUND_STRING("The foe is slashed\n"
                                       "with a sharp leaf.\n"
                                       "It has a high\n"
                                       "critical-hit ratio."),
        .animScript = gMoveAnim_LEAF_BLADE,
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
			.critStage = 1,
			.slicingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DRAGON_DANCE] =
    {
		.name = _("Dragon Dance"),
		.description = COMPOUND_STRING("A mystic, powerful\n"
                                       "dance that boosts\n"
                                       "the user's Attack\n"
                                       "and Speed stats."),
        .animScript = gMoveAnim_DRAGON_DANCE,
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
		.description = COMPOUND_STRING("It hurls hard rocks\n"
                                       "at the foe. Two to\n"
                                       "five rocks are\n"
                                       "launched in a row."),
        .animScript = gMoveAnim_ROCK_BLAST,
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
		.description = COMPOUND_STRING("A rapid jolt of\n"
                                       "electricity strikes\n"
                                       "the foe. It can't\n"
                                       "be evaded."),
        .animScript = gMoveAnim_SHOCK_WAVE,
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
		.description = COMPOUND_STRING("Attacks the foe\n"
                                       "with a pulsing\n"
                                       "blast of water. It\n"
                                       "may confuse the foe."),
        .animScript = gMoveAnim_WATER_PULSE,
		.effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.pulseMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_CONFUSION,
			.chance = 20,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DOOM_DESIRE] =
    {
		.name = _("Doom Desire"),
		.description = COMPOUND_STRING("A move that attacks\n"
                                       "the foe with a\n"
                                       "blast of light two\n"
                                       "turns after use."),
        .animScript = gMoveAnim_DOOM_DESIRE,
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
		.argument = {
			.futureAttack = {
				.stringId = B_MSG_CHOSE_AS_DESTINY,
				.animationId = B_ANIM_DOOM_DESIRE_HIT,
			},
		},
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PSYCHO_BOOST] =
    {
		.name = _("Psycho Boost"),
		.description = COMPOUND_STRING("An intense attack\n"
                                       "that also sharply\n"
                                       "reduces the user's\n"
                                       "Sp. Atk stat."),
        .animScript = gMoveAnim_PSYCHO_BOOST,
		.effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_ATK_MINUS_2,
			.chance = 0,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ROOST] =
    {
		.name = _("Roost"),
		.description = COMPOUND_STRING("The user lands and\n"
                                       "rests its body.\n"
                                       "Restoring it's HP\n"
                                       "by half its max HP."),
        .animScript = gMoveAnim_ROOST,
		.effect = EFFECT_ROOST,
        .type = TYPE_FLYING,
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

    [MOVE_GRAVITY] =
    {
		.name = _("Gravity"),
		.description = COMPOUND_STRING("Intensifies the\n"
                                       "gravity for five\n"
                                       "turns, making\n"
                                       "flying unusable."),
        .animScript = gMoveAnim_GRAVITY,
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
		.description = COMPOUND_STRING("Enables hit Ghost\n"
                                       "types. It also\n"
                                       "enables the user to\n"
                                       "hit an evasive foe."),
        .animScript = gMoveAnim_MIRACLE_EYE,
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
		.description = COMPOUND_STRING("Inflicts high\n"
                                       "damage on a\n"
                                       "sleeping foe. Also\n"
                                       "wakes the foe up."),
        .animScript = gMoveAnim_WAKE_UP_SLAP,
		.effect = EFFECT_CURE_STATUS1_FROM_ARG,
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
		.argument = {
			.cureStatus = { .statusId = STATUS1_SLEEP },
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_CURE_STATUS1,
			.chance = 0,
			.onFinalMultiHitOnly = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HAMMER_ARM] =
    {
		.name = _("Hammer Arm"),
		.description = COMPOUND_STRING("The user swings and\n"
                                       "hits with its heavy\n"
                                       "fist. It lowers the\n"
                                       "user's Speed."),
        .animScript = gMoveAnim_HAMMER_ARM,
		.effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SPD_MINUS_1,
			.chance = 0,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_GYRO_BALL] =
    {
		.name = _("Gyro Ball"),
		.description = COMPOUND_STRING("A high-speed spin's\n"
                                       "tackle. The slower\n"
                                       "the user, the\n"
                                       "greater the damage."),
        .animScript = gMoveAnim_GYRO_BALL,
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
		.description = COMPOUND_STRING("The user faints. In\n"
                                       "return, it heals\n"
                                       "the HP and status\n"
                                       "of its replacement."),
        .animScript = gMoveAnim_HEALING_WISH,
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
		.name = _("Brine"),
		.description = COMPOUND_STRING("This move's power\n"
                                       "is doubled if the\n"
                                       "target's HP is at\n"
                                       "half or less."),
        .animScript = gMoveAnim_BRINE,
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
		.description = COMPOUND_STRING("Attacks by using\n"
                                       "its held Berry. The\n"
                                       "Berry determines\n"
                                       "its type and power."),
        .animScript = gMoveAnim_NATURAL_GIFT,
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
		.description = COMPOUND_STRING("This attack hits a\n"
                                       "foe using Protect.\n"
                                       "This also lifts the\n"
                                       "effects of the move."),
        .animScript = gMoveAnim_FEINT,
		.effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FEINT,
			.chance = 0,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PLUCK] =
    {
		.name = _("Pluck"),
		.description = COMPOUND_STRING("It pecks the foe,\n"
                                       "plucks its held\n"
                                       "Berry and gains its\n"
                                       "effect."),
        .animScript = gMoveAnim_PLUCK,
		.effect = EFFECT_BUG_BITE,
        .power = 60,
        .type = TYPE_FLYING,
        .accuracy = 100,
        .pp = 20,
        // .secondaryEffectChance = 100,
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
		.description = COMPOUND_STRING("The user whips up a\n"
                                       "whirlwind that ups\n"
                                       "Speed of its party\n"
                                       "for four turns."),
        .animScript = gMoveAnim_TAILWIND,
		.effect = EFFECT_TAILWIND,
        .type = TYPE_FLYING,
		.pp = 15,
        .target = MOVE_TARGET_USER,
        .flags =
		{
			.snatchAffected = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
			.windMove = TRUE, // Only set for data purpose, Wind Power are handled separated
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_BOOST_CRITS,
    },

    [MOVE_ACUPRESSURE] =
    {
		.name = _("Acupressure"),
		.description = COMPOUND_STRING("It applies pressure\n"
                                       "to stress points,\n"
                                       "sharply boosting\n"
                                       "one of its stats."),
        .animScript = gMoveAnim_ACUPRESSURE,
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
		.description = COMPOUND_STRING("Retaliates against\n"
                                       "the foe that last\n"
                                       "did damage on it\n"
                                       "with greater power."),
        .animScript = gMoveAnim_METAL_BURST,
		.effect = EFFECT_COUNTER_ATTACK,
		.power = 1,
        .type = TYPE_STEEL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_DEPENDS,
		.flags =
		{
			.forbiddenMeFirst = TRUE,
		},
		.argument = {
			.counterAttack = { .split = SPLIT_STATUS }, // Counter both physical and special
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_U_TURN] =
    {
		.name = _("U-Turn"),
		.description = COMPOUND_STRING("After the attack,\n"
                                       "it rushes back to\n"
                                       "switch places with\n"
                                       "a party Pokémon."),
        .animScript = gMoveAnim_U_TURN,
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
		.description = COMPOUND_STRING("It fights the foe\n"
                                       "up close. It also\n"
                                       "cuts the user's\n"
                                       "Defense and Sp. Def."),
        .animScript = gMoveAnim_CLOSE_COMBAT,
		.effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_DEF_SPDEF_DOWN,
			.chance = 0,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PAYBACK] =
    {
		.name = _("Payback"),
		.description = COMPOUND_STRING("If the user can use\n"
                                       "this attack after\n"
                                       "the foe, its power\n"
                                       "is doubled."),
        .animScript = gMoveAnim_PAYBACK,
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
		.description = COMPOUND_STRING("Its power is\n"
                                       "doubled if the foe\n"
                                       "has taken damage in\n"
                                       "the same turn."),
        .animScript = gMoveAnim_ASSURANCE,
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
		.description = COMPOUND_STRING("It prevents the\n"
                                       "target from using\n"
                                       "its held item."),
        .animScript = gMoveAnim_EMBARGO,
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
		.description = COMPOUND_STRING("It flings its held\n"
                                       "item at the foe.\n"
                                       "Its effect and\n"
                                       "power depend on it."),
        .animScript = gMoveAnim_FLING,
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
		.description = COMPOUND_STRING("Using its psychic\n"
                                       "power, it transfers\n"
                                       "its status problems\n"
                                       "to the target."),
        .animScript = gMoveAnim_PSYCHO_SHIFT,
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
		.description = COMPOUND_STRING("The fewer PP this\n"
                                       "move has, the more\n"
                                       "power it has for\n"
                                       "attack."),
        .animScript = gMoveAnim_TRUMP_CARD,
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
		.description = COMPOUND_STRING("The user prevents\n"
                                       "the foes from any\n"
                                       "HP-recovery effect\n"
                                       "for five turns."),
        .animScript = gMoveAnim_HEAL_BLOCK,
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
		.description = COMPOUND_STRING("Powerfully wrings\n"
                                       "the foe. The more\n"
                                       "HP the foe has, the\n"
                                       "greater the power."),
        .animScript = gMoveAnim_WRING_OUT,
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
		.description = COMPOUND_STRING("Employs its psychic\n"
                                       "power to switch its\n"
                                       "Attack with its\n"
                                       "Defense stat."),
        .animScript = gMoveAnim_POWER_TRICK,
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
		.description = COMPOUND_STRING("It hurls up stomach\n"
                                       "acids to supress\n"
                                       "the effect of the\n"
                                       "foe's ability."),
        .animScript = gMoveAnim_GASTRO_ACID,
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
		.description = COMPOUND_STRING("The user chants an\n"
                                       "incantation,\n"
                                       "preventing critical\n"
                                       "hits from the foes."),
        .animScript = gMoveAnim_LUCKY_CHANT,
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
		.name = _("Me First"),
		.description = COMPOUND_STRING("Tries to cut ahead\n"
                                       "of the foe to steal\n"
                                       "and use its\n"
                                       "intended move."),
        .animScript = gMoveAnim_ME_FIRST,
		.effect = EFFECT_ME_FIRST,
        .type = TYPE_NORMAL,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED_OPPONENT,
		.flags =
		{
			.forbiddenMirrorMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenAssist = TRUE,
			.forbiddenCopycat = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_SPD_UP_2,
    },

    [MOVE_COPYCAT] =
    {
		.name = _("Copycat"),
		.description = COMPOUND_STRING("It mimics the move\n"
                                       "used before it. It\n"
                                       "fails if no move\n"
                                       "has been used yet."),
        .animScript = gMoveAnim_COPYCAT,
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
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_ACC_UP_1,
    },

    [MOVE_POWER_SWAP] =
    {
		.name = _("Power Swap"),
		.description = COMPOUND_STRING("The user switches\n"
                                       "changes to its\n"
                                       "Attack and Sp. Atk\n"
                                       "with the foe."),
        .animScript = gMoveAnim_POWER_SWAP,
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
		.description = COMPOUND_STRING("The user switches\n"
                                       "changes to its\n"
                                       "Defense and Sp. Def\n"
                                       "with the foe."),
        .animScript = gMoveAnim_GUARD_SWAP,
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
		.description = COMPOUND_STRING("Its power increases\n"
                                       "the more the foe\n"
                                       "has powered up with\n"
                                       "stat changes."),
        .animScript = gMoveAnim_PUNISHMENT,
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
		.description = COMPOUND_STRING("It can be used only\n"
                                       "after the user has\n"
                                       "used all the other\n"
                                       "moves it knows."),
        .animScript = gMoveAnim_LAST_RESORT,
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
		.description = COMPOUND_STRING("A seed that causes\n"
                                       "worry is planted on\n"
                                       "the foe. Making its\n"
                                       "ability Insomnia."),
        .animScript = gMoveAnim_WORRY_SEED,
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
		.argument = {
			.setAbility = { .abilityId = ABILITY_INSOMNIA },
		},
        .zMoveEffect = Z_EFFECT_SPD_UP_1,
    },

    [MOVE_SUCKER_PUNCH] =
    {
		.name = _("Sucker Punch"),
		.description = COMPOUND_STRING("Enables the user to\n"
                                       "attack first. Fails\n"
                                       "if the foe is not\n"
                                       "readying an attack."),
        .animScript = gMoveAnim_SUCKER_PUNCH,
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
		.description = COMPOUND_STRING("Poison spikes are\n"
                                       "laid on the foe's\n"
                                       "feet to poison foes\n"
                                       "switching in."),
        .animScript = gMoveAnim_TOXIC_SPIKES,
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
		.name = _("Heart Swap"),
		.description = COMPOUND_STRING("It employs its\n"
                                       "psychic power to\n"
                                       "switch stat changes\n"
                                       "with the target."),
        .animScript = gMoveAnim_HEART_SWAP,
		.effect = EFFECT_HEART_SWAP,
        .type = TYPE_PSYCHIC,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
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
        .effect = EFFECT_HIT,
        .power = 120,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.thawUser = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_BURN,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FORCE_PALM] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_PARALYSIS,
			.chance = 30,
		}),
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
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_POISON,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_POISON,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DARK_PULSE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			// .secondaryEffectMove = TRUE,
			.pulseMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 20,
		}),
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
			.critStage = 1,
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
        .effect = EFFECT_HIT,
        .power = 75,
        .type = TYPE_FLYING,
        .accuracy = 95,
		.pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			// .secondaryEffectMove = TRUE,
			.slicingMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 30,
		}),
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
        .effect = EFFECT_HIT,
        .power = 90,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.soundMove = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_DEF_MINUS_1,
			.chance = 10,
		}),
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
        .effect = EFFECT_HIT,
        .power = 100,
        .type = TYPE_DRAGON,
        .accuracy = 75,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			// .secondaryEffectMove = TRUE,
			.dmgMinimize = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 20,
		}),
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
        .effect = EFFECT_HIT_ABSORB,
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
			// .healingMove = TRUE,
		},
		.argument = {
			.absorb = { .percentage = 50 },
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
        .effect = EFFECT_HIT,
        .power = 120,
        .type = TYPE_FIGHTING,
        .accuracy = 70,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			// .secondaryEffectMove = TRUE,
			.ballisticMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_DEF_MINUS_1,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ENERGY_BALL] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
		.power = 90,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.ballisticMove = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_DEF_MINUS_1,
			.chance = 10,
		}),
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
        .effect = EFFECT_HIT,
        .power = 90,
        .type = TYPE_GROUND,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_DEF_MINUS_1,
			.chance = 10,
		}),
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
        .effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_RECHARGE,
			.chance = 0,
			.self = TRUE,
		}),
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
			.critStage = 1,
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
        // .secondaryEffectChance = 10,
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

    [MOVE_ICE_FANG] =
    {
		.name = _("-"),
        .effect = EFFECT_FLINCH_STATUS,
        .power = 65,
        .type = TYPE_ICE,
        .accuracy = 95,
        .pp = 15,
        // .secondaryEffectChance = 10,
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

    [MOVE_FIRE_FANG] =
    {
		.name = _("-"),
        .effect = EFFECT_FLINCH_STATUS,
        .power = 65,
        .type = TYPE_FIRE,
        .accuracy = 95,
        .pp = 15,
        // .secondaryEffectChance = 10,
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
        .effect = EFFECT_HIT,
        .power = 65,
        .type = TYPE_GROUND,
        .accuracy = 85,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.ballisticMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_ACC_MINUS_1,
			.chance = 30,
		}),
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
			.critStage = 1,
			.slicingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ZEN_HEADBUTT] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_PSYCHIC,
        .accuracy = 90,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 20,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_MIRROR_SHOT] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 65,
        .type = TYPE_STEEL,
        .accuracy = 85,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_ACC_MINUS_1,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FLASH_CANNON] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_STEEL,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_DEF_MINUS_1,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ROCK_CLIMB] =
    {
		.name = _("-"),
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
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_CONFUSION,
			.chance = 20,
		}),
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
        .effect = EFFECT_HIT,
		.power = 130,
        .type = TYPE_DRAGON,
        .accuracy = 90,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_ATK_MINUS_2,
			.chance = 0,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_DISCHARGE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_PARALYSIS,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LAVA_PLUME] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_BURN,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_LEAF_STORM] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
		.power = 130,
        .type = TYPE_GRASS,
        .accuracy = 90,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_ATK_MINUS_2,
			.chance = 0,
			.self = TRUE,
		}),
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
        .effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_RECHARGE,
			.chance = 0,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_CROSS_POISON] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 70,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.critStage = 1,
			// .secondaryEffectMove = TRUE,
			.slicingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_POISON,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_GUNK_SHOT] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 120,
        .type = TYPE_POISON,
		.accuracy = 80,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_POISON,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_IRON_HEAD] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_STEEL,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 30,
		}),
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
			.critStage = 1,
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
        .effect = EFFECT_DAMAGE_BASED_TARGET_WEIGHT,
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
        .effect = EFFECT_HIT,
		.power = 65,
        .type = TYPE_FLYING,
        .accuracy = 100,
        .pp = 20,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_CONFUSION,
			.chance = 100,
		}),
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
        // .argument = HOLD_EFFECT_PLATE,
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
        // .secondaryEffectChance = 100,
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
        // .secondaryEffectChance = 70,
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
			.critStage = 1,
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
			// .healingMove = TRUE,
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
        .effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_RECHARGE,
			.chance = 0,
			.self = TRUE,
		}),
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
			.critStage = 1,
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
			// .healingMove = TRUE,
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
        .effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_WRAP,
			.chance = 0,
		}),
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
        // .secondaryEffectChance = 40,
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
        .effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_GHOST,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_ALL_STATS_UP,
			.chance = 10,
			.self = TRUE,
		}),
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
			// .twoTurnsMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        .zMoveEffect = Z_EFFECT_NONE,
        // .argument = MOVE_EFFECT_FEINT,
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
			// .affectsUserSide = TRUE, // Potects the whole side.
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
        // .secondaryEffectChance = 100,
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
        // .secondaryEffectChance = 100,
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
        .effect = EFFECT_HIT,
        .power = 95,
        .type = TYPE_POISON,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_POISON,
			.chance = 10,
		}),
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
        // .secondaryEffectChance = 100,
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
        .effect = EFFECT_HIT,
		.power = 65,
        .type = TYPE_FIGHTING,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SPD_MINUS_1,
			.chance = 100,
		}),
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
        // .secondaryEffectChance = 100,
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
        // .secondaryEffectChance = 100,
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
			// .affectsUserSide = TRUE, // Protects the whole side.
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
        // .secondaryEffectChance = 30,
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
			// .healingMove = TRUE,
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
        // .secondaryEffectChance = 100,
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
        .effect = EFFECT_HIT,
        .power = 100,
        .type = TYPE_FIRE,
        .accuracy = 50,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_BURN,
			.chance = 100,
		}),
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
        .effect = EFFECT_HIT,
		.power = 50,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_ATK_MINUS_1,
			.chance = 100,
		}),
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
        // .secondaryEffectChance = 100,
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
        .effect = EFFECT_HIT,
        .power = 55,
        .type = TYPE_ELECTRIC,
        .accuracy = 95,
        .pp = 15,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SPD_MINUS_1,
			.chance = 100,
		}),
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
			.critStage = 1,
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
        .effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 25,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HORN_LEECH] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT_ABSORB,
        .power = 75,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			// .healingMove = TRUE,
		},
		.argument = {
			.absorb = { .percentage = 50 },
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
        .effect = EFFECT_HIT,
        .power = 75,
        .type = TYPE_WATER,
        .accuracy = 95,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			// .secondaryEffectMove = TRUE,
			.slicingMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_DEF_MINUS_1,
			.chance = 50,
		}),
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
        .effect = EFFECT_HIT,
        .power = 65,
        .type = TYPE_GRASS,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_ACC_MINUS_1,
			.chance = 50,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STEAMROLLER] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 65,
        .type = TYPE_BUG,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			// .secondaryEffectMove = TRUE,
			.dmgMinimize = TRUE,
		},
		.split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 30,
		}),
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
        .effect = EFFECT_HIT,
        .power = 85,
        .type = TYPE_DARK,
        .accuracy = 95,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_ACC_MINUS_1,
			.chance = 40,
		}),
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
        // .secondaryEffectChance = 30,
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
        .effect = EFFECT_HIT,
        .power = 100,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.ballisticMove = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_BURN,
			.chance = 30,
		}),
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
        // .argument = HOLD_EFFECT_DRIVE,
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
        // .secondaryEffectChance = 10,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.soundMove = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
        // .argument = STATUS1_SLEEP,
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
        .effect = EFFECT_HIT,
        .power = 65,
        .type = TYPE_ICE,
        .accuracy = 95,
        .pp = 10,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SPD_MINUS_1,
			.chance = 100,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BOLT_STRIKE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 130,
        .type = TYPE_ELECTRIC,
        .accuracy = 85,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_PARALYSIS,
			.chance = 20,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BLUE_FLARE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 130,
        .type = TYPE_FIRE,
        .accuracy = 85,
        .pp = 5,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_BURN,
			.chance = 20,
		}),
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
        // .secondaryEffectChance = 50,
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
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE,
			// .twoTurnsMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_PARALYSIS,
			.chance = 30,
		}),
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
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
			.forbiddenSleepTalk = TRUE,
			.forbiddenInstruct = TRUE,
			.forbiddenParentalBond = TRUE,
			// .twoTurnsMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_BURN,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SNARL] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 55,
        .type = TYPE_DARK,
        .accuracy = 95,
        .pp = 15,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.soundMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_ATK_MINUS_1,
			.chance = 100,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ICICLE_CRASH] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 85,
        .type = TYPE_ICE,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 30,
		}),
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
        // .secondaryEffectChance = 100,
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
        // .argument = TYPE_FLYING,
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
        // .argument = TRUE, // Protects the whole side.
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
        // .secondaryEffectChance = 100,
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
			// .twoTurnsMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
        // .argument = MOVE_EFFECT_FEINT,
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
        // .argument = TYPE_GHOST,
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
        .effect = EFFECT_HIT_ABSORB,
		.power = 65,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .healingMove = TRUE,
		},
		.argument = {
			.absorb = { .percentage = 50 },
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
        // .argument = TYPE_GRASS,
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
        // .secondaryEffectChance = 10,
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
        .effect = EFFECT_HIT_ABSORB,
        .power = 50,
        .type = TYPE_FAIRY,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .healingMove = TRUE,
		},
		.argument = {
			.absorb = { .percentage = 75 },
		},
        .split = SPLIT_SPECIAL,
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
        // .argument = TRUE, // Protects the whole side.
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
        .effect = EFFECT_HIT,
        .power = 90,
        .type = TYPE_FAIRY,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_ATK_MINUS_1,
			.chance = 10,
		}),
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
        .effect = EFFECT_HIT,
        .power = 95,
        .type = TYPE_FAIRY,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_ATK_MINUS_1,
			.chance = 30,
		}),
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
        // .secondaryEffectChance = 50,
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
        // .secondaryEffectChance = 30,
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
        .effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FEINT,
			.chance = 0,
		}),
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
        .effect = EFFECT_HIT,
		.power = 75,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_ATK_MINUS_1,
			.chance = 100,
		}),
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
        .effect = EFFECT_HIT,
        .power = 20,
        .type = TYPE_ELECTRIC,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_PARALYSIS,
			.chance = 100,
		}),
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
        .effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_WRAP,
			.chance = 0,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_POWER_UP_PUNCH] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 40,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 20,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.punchMove = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_ATK_PLUS_1,
			.chance = 100,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_OBLIVION_WING] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT_ABSORB,
        .power = 80,
        .type = TYPE_FLYING,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .healingMove = TRUE,
		},
		.argument = {
			.absorb = { .percentage = 75 },
		},
        .split = SPLIT_SPECIAL,
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
        // .secondaryEffectChance = 100,
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
        // .secondaryEffectChance = 100,
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
        .effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_DEF_SPDEF_DOWN,
			.chance = 0,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_HYPERSPACE_FURY] =
    {
		.name = _("-"),
        .effect = EFFECT_HYPERSPACE_FURY,
        .power = 100,
        .type = TYPE_DARK,
        .pp = 5,
        // .secondaryEffectChance = 100,
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
			// .healingMove = TRUE,
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
        // .secondaryEffectChance = 100,
        .target = MOVE_TARGET_FOES_AND_ALLY,
        .flags =
		{
			.kingsRockAffected = TRUE,
			.soundMove = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
        .argument = {
			.cureStatus = { .statusId = STATUS1_BURN },
		},
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ICE_HAMMER] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SPD_MINUS_1,
			.chance = 0,
			.self = TRUE,
		}),
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
			// .healingMove = TRUE,
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
			// .healingMove = TRUE,
		},
        .split = SPLIT_STATUS,
        .zMoveEffect = Z_EFFECT_DEF_UP_1,
    },

    [MOVE_SOLAR_BLADE] =
    {
		.name = _("-"),
        .effect = EFFECT_SKIP_CHARGING_IN_WEATHER,
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
		},
		.argument = {
			.twoTurns = {
				.stringId = B_MSG_ABSORBED_LIGHT,
				.weather = B_WEATHER_SUN_ANY,
			},
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
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_ATK_MINUS_1,
			.chance = 100,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FIRE_LASH] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_FIRE,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_DEF_MINUS_1,
			.chance = 100,
		}),
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
        // .secondaryEffectChance = 100,
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
			// .healingMove = TRUE,
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
        .effect = EFFECT_HIT,
        .power = 70,
        .type = TYPE_GRASS,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_ATK_MINUS_1,
			.chance = 100,
		}),
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
        .effect = EFFECT_HIT,
        .power = 130,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_ATK_MINUS_2,
			.chance = 0,
			.self = TRUE,
		}),
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
        .effect = EFFECT_HIT,
        .power = 85,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_DEF_MINUS_1,
			.chance = 20,
		}),
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
        .effect = EFFECT_HIT,
        .power = 85,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_DEF_MINUS_1,
			.chance = 20,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PRISMATIC_LASER] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_RECHARGE,
			.chance = 0,
			.self = TRUE,
		}),
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
        // .secondaryEffectChance = 100,
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
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 30,
		}),
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
        // .argument = HOLD_EFFECT_MEMORY,
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
        // .secondaryEffectChance = 100,
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
        .effect = EFFECT_HIT,
        .power = 90,
        .type = TYPE_WATER,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_BOTH,
		.flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_PARALYSIS,
			.chance = 30,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FLOATY_FALL] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 90,
        .type = TYPE_FLYING,
        .accuracy = 95,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 30,
		}),
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
        .effect = EFFECT_HIT_ABSORB,
		.power = 60,
        .type = TYPE_WATER,
        .accuracy = 100,
		.pp = 20,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
			// .healingMove = TRUE,
		},
		.argument = {
			.absorb = { .percentage = 100 },
		},
        .split = SPLIT_SPECIAL,
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BUZZY_BUZZ] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
		.power = 60,
        .type = TYPE_ELECTRIC,
        .accuracy = 100,
		.pp = 20,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.kingsRockAffected = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_PARALYSIS,
			.chance = 100,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SIZZLY_SLIDE] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
		.power = 60,
        .type = TYPE_FIRE,
        .accuracy = 100,
		.pp = 20,
        .target = MOVE_TARGET_SELECTED,
		.flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			.thawUser = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_BURN,
			.chance = 100,
		}),
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
        .effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_STEEL,
        .accuracy = 100,
        .pp = 5,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 30,
		}),
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
			.critStage = 1,
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
        // .argument = TYPE_PSYCHIC,
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
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SPD_MINUS_1,
			.chance = 100,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_SNAP_TRAP] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_WRAP,
			.chance = 0,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_PYRO_BALL] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 120,
        .type = TYPE_FIRE,
        .accuracy = 90,
        .pp = 5,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_BURN,
			.chance = 10,
		}),
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
        // .secondaryEffectChance = 100,
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
        .effect = EFFECT_HIT,
        .power = 60,
        .type = TYPE_DRAGON,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_ATK_MINUS_1,
			.chance = 100,
		}),
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
        .effect = EFFECT_HIT,
        .power = 80,
        .type = TYPE_GRASS,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_DEF_MINUS_1,
			.chance = 100,
		}),
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
        // .secondaryEffectChance = 100,
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
        .effect = EFFECT_HIT,
        .power = 75,
        .type = TYPE_FAIRY,
        .accuracy = 100,
        .pp = 15,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_ATK_MINUS_1,
			.chance = 100,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_STRANGE_STEAM] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 90,
        .type = TYPE_FAIRY,
        .accuracy = 95,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_CONFUSION,
			.chance = 20,
		}),
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
        .effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_RECHARGE,
			.chance = 0,
			.self = TRUE,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_ETERNABEAM] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_RECHARGE,
			.chance = 0,
			.self = TRUE,
		}),
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
        // .secondaryEffectChance = 20,
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
        .effect = EFFECT_HIT,
        .power = 70,
        .type = TYPE_BUG,
        .accuracy = 90,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_SP_ATK_MINUS_1,
			.chance = 100,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_BURNING_JEALOUSY] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 70,
        .type = TYPE_FIRE,
        .accuracy = 100,
        .pp = 5,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_BURN,
			.chance = 100,
			.onlyIfTargetRaisedStats = TRUE,
		}),
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
		.argument = {
			.tripleKick = { .increment = 20 },
		},
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
        // .secondaryEffectChance = 30,
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
        .effect = EFFECT_HIT,
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
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_WRAP,
			.chance = 0,
		}),
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
        .effect = EFFECT_HIT,
        .type = TYPE_PSYCHIC,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FREEZE,
			.chance = 10,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_FIERY_WRATH] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 90,
        .type = TYPE_DARK,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_BOTH,
		.flags =
		{
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_FLINCH,
			.chance = 20,
		}),
        .zMoveEffect = Z_EFFECT_NONE,
    },

    [MOVE_THUNDEROUS_KICK] =
    {
		.name = _("-"),
        .effect = EFFECT_HIT,
        .power = 90,
        .type = TYPE_FIGHTING,
        .accuracy = 100,
        .pp = 10,
        .target = MOVE_TARGET_SELECTED,
        .flags =
		{
			.makesContact = TRUE,
			.kingsRockAffected = TRUE,
			// .secondaryEffectMove = TRUE,
			.forbiddenMetronome = TRUE,
		},
        .split = SPLIT_PHYSICAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_DEF_MINUS_1,
			.chance = 100,
		}),
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
		.argument = {
			.spite = { .ppToDeduct = 3 },
		},
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
        // .secondaryEffectChance = 50, // TODO: Adjust this value. Currently it's set to Fiery Dance's.
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
        // .secondaryEffectChance = 30,
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
        // .secondaryEffectChance = 100,
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
        // .secondaryEffectChance = 100,
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
        // .secondaryEffectChance = 100,
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
        // .secondaryEffectChance = 100,
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
        // .secondaryEffectChance = 100,
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
        // .secondaryEffectChance = 100,
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
        // .secondaryEffectChance = 100,
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
        // .secondaryEffectChance = 100,
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
        // .secondaryEffectChance = 100,
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
			// .healingMove = TRUE,
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
			.critStage = 2,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
    },
    [MOVE_STOKED_SPARKSURFER] =
    {
		.name = _("-"),
        // .effect = EFFECT_PARALYZE_HIT,
        .power = 175,
        .type = TYPE_ELECTRIC,
        .pp = 1,
        // .secondaryEffectChance = 100,
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
        // .argument = 0,  //psychic terrain
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
        // .argument = 1,  //remove terrain
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
        .effect = EFFECT_HIT,
        .power = 185,
        .type = TYPE_DRAGON,
        .pp = 1,
        .target = MOVE_TARGET_BOTH,
        .flags =
		{
			.soundMove = TRUE,
			.forbiddenProtect = TRUE,
			.forbiddenMirrorMove = TRUE,
		},
        .split = SPLIT_SPECIAL,
		.additionalEffects = ADDITIONAL_EFFECTS({
			.moveEffect = MOVE_EFFECT_ALL_STATS_UP,
			.chance = 100,
			.self = TRUE,
		}),
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
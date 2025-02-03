const struct BattleMoveEffect gBattleMoveEffects[EFFECTS_COUNT] =
{
	[EFFECT_HIT] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_MULTI_HIT] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_OHKO] =
	{
		.battleScript = BattleScript_EffectOHKO,
	},
	
	[EFFECT_TWO_TURNS_ATTACK] =
	{
		.battleScript = BattleScript_EffectTwoTurnMoves,
		.twoTurnsEffect = TRUE,
	},
	
	[EFFECT_USER_ATTACK_UP_2] =
	{
		.battleScript = BattleScript_EffectUserAttackUp2,
	},
	
	[EFFECT_RANDOM_SWITCH] =
	{
		.battleScript = BattleScript_EffectRandomSwitch,
	},
	
	[EFFECT_SEMI_INVULNERABLE] =
	{
		.battleScript = BattleScript_EffectTwoTurnMoves,
		.twoTurnsEffect = TRUE,
		.semiInvulnerableEffect = TRUE,
	},
	
	[EFFECT_RECOIL_IF_MISS] =
	{
		.battleScript = BattleScript_EffectRecoilIfMiss,
	},
	
	[EFFECT_ACCURACY_DOWN] =
	{
		.battleScript = BattleScript_EffectAccuracyDown,
	},
	
	[EFFECT_DEFENSE_DOWN] =
	{
		.battleScript = BattleScript_EffectDefenseDown,
	},
	
	[EFFECT_ATTACK_DOWN] =
	{
		.battleScript = BattleScript_EffectAttackDown,
	},
	
	[EFFECT_SLEEP] =
	{
		.battleScript = BattleScript_EffectSleep,
	},
	
	[EFFECT_CONFUSE] =
	{
		.battleScript = BattleScript_EffectConfuse,
	},
	
	[EFFECT_FIXED_DAMAGE] =
	{
		.battleScript = BattleScript_EffectFixedDamage,
		.noEffectiveness = TRUE,
	},
	
	[EFFECT_DISABLE_MOVE] =
	{
		.battleScript = BattleScript_EffectDisableMove,
	},
	
	[EFFECT_SET_MIST] =
	{
		.battleScript = BattleScript_EffectSetMist,
		.affectsUserSide = TRUE,
	},
	
	[EFFECT_DAMAGE_BASED_TARGET_WEIGHT] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_COUNTER_ATTACK] =
	{
		.battleScript = BattleScript_EffectCounterAttack,
		.noEffectiveness = TRUE,
	},
	
	[EFFECT_USER_LEVEL_TO_DAMAGE] =
	{
		.battleScript = BattleScript_EffectUserLevelToDamage,
		.noEffectiveness = TRUE,
	},
	
	[EFFECT_HIT_ABSORB] =
	{
		.battleScript = BattleScript_EffectHitAbsorb,
		.healingEffect = TRUE,
	},
	
	[EFFECT_SET_SEEDED] =
	{
		.battleScript = BattleScript_EffectSetSeeded,
	},
	
	[EFFECT_USER_ATTACK_AND_SP_ATTACK_UP_2X_IN_SUNNY] =
	{
		.battleScript = BattleScript_EffectUserAtkAndSpAtkUp2xInSunny,
	},
	
	[EFFECT_SKIP_CHARGING_IN_WEATHER] =
	{
		.battleScript = BattleScript_EffectTwoTurnMoves,
		.twoTurnsEffect = TRUE,
	},
	
	[EFFECT_SET_POISON] =
	{
		.battleScript = BattleScript_EffectSetPoison,
	},
	
	[EFFECT_SET_PARALYZE] =
	{
		.battleScript = BattleScript_EffectSetParalyze,
	},
	
	[EFFECT_SPEED_DOWN_2] =
	{
		.battleScript = BattleScript_EffectSpeedDown2,
	},
	
	[EFFECT_SET_TOXIC_POISON] =
	{
		.battleScript = BattleScript_EffectSetToxicPoison,
	},
	
	[EFFECT_ATTACK_UP] =
	{
		.battleScript = BattleScript_EffectAttackUp,
	},
	
	[EFFECT_SPEED_UP_2] =
	{
		.battleScript = BattleScript_EffectSpeedUp2,
	},
	
	[EFFECT_RAGE] =
	{
		.battleScript = BattleScript_EffectRage,
	},
	
	[EFFECT_TELEPORT] =
	{
		.battleScript = BattleScript_EffectTeleport,
	},
	
	[EFFECT_MIMIC_MOVE] =
	{
		.battleScript = BattleScript_EffectMimicMove,
	},
	
	[EFFECT_DEFENSE_DOWN_2] =
	{
		.battleScript = BattleScript_EffectDefenseDown2,
	},
	
	[EFFECT_EVASION_UP] =
	{
		.battleScript = BattleScript_EffectEvasionUp,
	},
	
	[EFFECT_RESTORE_HP] =
	{
		.battleScript = BattleScript_EffectRestoreHp,
		.healingEffect = TRUE,
	},
	
	[EFFECT_DEFENSE_UP] =
	{
		.battleScript = BattleScript_EffectDefenseUp,
	},
	
	[EFFECT_EVASION_UP_SET_MINIMIZE] =
	{
		.battleScript = BattleScript_EffectEvasionUpSetMinimize,
	},
	
	[EFFECT_DEFENSE_UP_SET_DEFENSE_CURL] =
	{
		.battleScript = BattleScript_EffectDefenseUpSetDefenseCurl,
	},
	
	[EFFECT_DEFENSE_UP_2] =
	{
		.battleScript = BattleScript_EffectDefenseUp2,
	},
	
	[EFFECT_SET_LIGHT_SCREEN] =
	{
		.battleScript = BattleScript_EffectSetLightScreen,
		.affectsUserSide = TRUE,
	},
	
	[EFFECT_NORMALISE_FIELD_BUFFS] =
	{
		.battleScript = BattleScript_EffectNormaliseFieldBuffs,
	},
	
	[EFFECT_SET_REFLECT] =
	{
		.battleScript = BattleScript_EffectSetReflect,
		.affectsUserSide = TRUE,
	},
	
	[EFFECT_FOCUS_ENERGY] =
	{
		.battleScript = BattleScript_EffectFocusEnergy,
	},
	
	[EFFECT_BIDE] =
	{
		.battleScript = BattleScript_EffectBide,
		.twoTurnsEffect = TRUE,
		.noEffectiveness = TRUE,
	},
	
	[EFFECT_METRONOME] =
	{
		.battleScript = BattleScript_EffectMetronome,
		.callOtherMove = TRUE,
	},
	
	[EFFECT_MIRROR_MOVE] =
	{
		.battleScript = BattleScript_EffectMirrorMove,
		.callOtherMove = TRUE,
	},
	
	[EFFECT_EXPLOSION] =
	{
		.battleScript = BattleScript_EffectExplosion,
	},
	
	[EFFECT_SPECIAL_DEFENSE_UP_2] =
	{
		.battleScript = BattleScript_EffectSpecialDefenseUp2,
	},
	
	[EFFECT_DREAM_EATER] =
	{
		.battleScript = BattleScript_EffectDreamEater,
		.healingEffect = TRUE,
	},
	
	[EFFECT_NEVER_MISS_IN_WEATHER] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_TRANSFORM] =
	{
		.battleScript = BattleScript_EffectTransform,
	},
	
	[EFFECT_PSYWAVE] =
	{
		.battleScript = BattleScript_EffectPsywave,
		.noEffectiveness = TRUE,
	},
	
	[EFFECT_DO_NOTHING] =
	{
		.battleScript = BattleScript_EffectDoNothing,
	},
	
	[EFFECT_REST] =
	{
		.battleScript = BattleScript_EffectRest,
		.healingEffect = TRUE,
	},
	
	[EFFECT_CONVERSION] =
	{
		.battleScript = BattleScript_EffectConversion,
	},
	
	[EFFECT_SUPER_FANG] =
	{
		.battleScript = BattleScript_EffectSuperFang,
		.noEffectiveness = TRUE,
	},
	
	[EFFECT_SUBSTITUTE] =
	{
		.battleScript = BattleScript_EffectSubstitute,
	},
	
	[EFFECT_SKETCH] =
	{
		.battleScript = BattleScript_EffectSketch,
	},
	
	[EFFECT_TRIPLE_KICK] =
	{
		.battleScript = BattleScript_EffectTripleKick,
	},
	
	[EFFECT_MEAN_LOOK] =
	{
		.battleScript = BattleScript_EffectMeanLook,
	},
	
	[EFFECT_LOCK_ON] =
	{
		.battleScript = BattleScript_EffectLockOn,
	},
	
	[EFFECT_NIGHTMARE] =
	{
		.battleScript = BattleScript_EffectNightmare,
	},
	
	[EFFECT_SNORE] =
	{
		.battleScript = BattleScript_EffectSnore,
	},
	
	[EFFECT_CURSE] =
	{
		.battleScript = BattleScript_EffectCurse,
	},
	
	[EFFECT_FLAIL] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_CONVERSION_2] =
	{
		.battleScript = BattleScript_EffectConversion2,
	},
	
	[EFFECT_SPITE] =
	{
		.battleScript = BattleScript_EffectSpite,
	},
	
	[EFFECT_PROTECT] =
	{
		.battleScript = BattleScript_EffectProtect,
	},
	
	[EFFECT_BELLY_DRUM] =
	{
		.battleScript = BattleScript_EffectBellyDrum,
	},
	
	[EFFECT_SPIKES] =
	{
		.battleScript = BattleScript_EffectSpikes,
	},
	
	[EFFECT_FORESIGHT] =
	{
		.battleScript = BattleScript_EffectForesight,
	},
	
	[EFFECT_DESTINY_BOND] =
	{
		.battleScript = BattleScript_EffectDestinyBond,
	},
	
	[EFFECT_PERISH_SONG] =
	{
		.battleScript = BattleScript_EffectPerishSong,
	},
	
	[EFFECT_SET_WEATHER] =
	{
		.battleScript = BattleScript_EffectSetWeather,
	},
	
	[EFFECT_ENDURE] =
	{
		.battleScript = BattleScript_EffectProtect,
	},
	
	[EFFECT_ATTACK_DOWN_2] =
	{
		.battleScript = BattleScript_EffectAttackDown2,
	},
	
	[EFFECT_ROLLOUT] =
	{
		.battleScript = BattleScript_EffectRollout,
		.noEffectiveness = TRUE,
	},
	
	[EFFECT_FALSE_SWIPE] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_SWAGGER] =
	{
		.battleScript = BattleScript_EffectSwagger,
	},
	
	[EFFECT_FURY_CUTTER] =
	{
		.battleScript = BattleScript_EffectFuryCutter,
	},
	
	[EFFECT_ATTRACT] =
	{
		.battleScript = BattleScript_EffectAttract,
	},
	
	[EFFECT_SLEEP_TALK] =
	{
		.battleScript = BattleScript_EffectSleepTalk,
		.callOtherMove = TRUE,
	},
	
	[EFFECT_HEAL_BELL] =
	{
		.battleScript = BattleScript_EffectHealBell,
		.affectsUserSide = TRUE,
	},
	
	[EFFECT_RETURN] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_PRESENT] =
	{
		.battleScript = BattleScript_EffectPresent,
	},
	
	[EFFECT_FRUSTRATION] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_SAFEGUARD] =
	{
		.battleScript = BattleScript_EffectSafeguard,
		.affectsUserSide = TRUE,
	},
	
	[EFFECT_PAIN_SPLIT] =
	{
		.battleScript = BattleScript_EffectPainSplit,
	},
	
	[EFFECT_MAGNITUDE] =
	{
		.battleScript = BattleScript_EffectMagnitude,
	},
	
	[EFFECT_BATON_PASS] =
	{
		.battleScript = BattleScript_EffectBatonPass,
	},
	
	[EFFECT_ENCORE] =
	{
		.battleScript = BattleScript_EffectEncore,
	},
	
	[EFFECT_PURSUIT] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_EVASION_DOWN_2] =
	{
		.battleScript = BattleScript_EffectEvasionDown2,
	},
	
	[EFFECT_MORNING_SUN] =
	{
		.battleScript = BattleScript_EffectMorningSun,
		.healingEffect = TRUE,
	},
	
	[EFFECT_HIDDEN_POWER] =
	{
		.battleScript = BattleScript_EffectHit,
		.normalizeUnaffected = TRUE,
	},
	
	[EFFECT_PSYCH_UP] =
	{
		.battleScript = BattleScript_EffectPsychUp,
	},
	
	[EFFECT_FUTURE_SIGHT] =
	{
		.battleScript = BattleScript_EffectFutureSight,
	},
	
	[EFFECT_BEAT_UP] =
	{
		.battleScript = BattleScript_EffectBeatUp,
	},
	
	[EFFECT_FAKE_OUT] =
	{
		.battleScript = BattleScript_EffectFakeOut,
	},
	
	[EFFECT_STOCKPILE] =
	{
		.battleScript = BattleScript_EffectStockpile,
	},
	
	[EFFECT_SPIT_UP] =
	{
		.battleScript = BattleScript_EffectSpitUp,
	},
	
	[EFFECT_SWALLOW] =
	{
		.battleScript = BattleScript_EffectSwallow,
		.healingEffect = TRUE,
	},
	
	[EFFECT_TORMENT] =
	{
		.battleScript = BattleScript_EffectTorment,
	},
	
	[EFFECT_FLATTER] =
	{
		.battleScript = BattleScript_EffectFlatter,
	},
	
	[EFFECT_WILL_O_WISP] =
	{
		.battleScript = BattleScript_EffectWillOWisp,
	},
	
	[EFFECT_MEMENTO] =
	{
		.battleScript = BattleScript_EffectMemento,
	},
	
	[EFFECT_FACADE] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_FOCUS_PUNCH] =
	{
		.battleScript = BattleScript_EffectFocusPunch,
	},
	
	[EFFECT_CURE_STATUS1_FROM_ARG] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_FOLLOW_ME] =
	{
		.battleScript = BattleScript_EffectFollowMe,
	},
	
	[EFFECT_NATURE_POWER] =
	{
		.battleScript = BattleScript_EffectNaturePower,
		.callOtherMove = TRUE,
	},
	
	[EFFECT_CHARGE] =
	{
		.battleScript = BattleScript_EffectCharge,
	},
	
	[EFFECT_TAUNT] =
	{
		.battleScript = BattleScript_EffectTaunt,
	},
	
	[EFFECT_HELPING_HAND] =
	{
		.battleScript = BattleScript_EffectHelpingHand,
	},
	
	[EFFECT_TRICK] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_ROLE_PLAY] =
	{
		.battleScript = BattleScript_EffectRolePlay,
	},
	
	[EFFECT_WISH] =
	{
		.battleScript = BattleScript_EffectWish,
		.healingEffect = TRUE,
	},
	
	[EFFECT_ASSIST] =
	{
		.battleScript = BattleScript_EffectAssist,
		.callOtherMove = TRUE,
	},
	
	[EFFECT_INGRAIN] =
	{
		.battleScript = BattleScript_EffectIngrain,
	},
	
	[EFFECT_MAGIC_COAT] =
	{
		.battleScript = BattleScript_EffectMagicCoat,
	},
	
	[EFFECT_RECYCLE] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_REVENGE] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_BRICK_BREAK] =
	{
		.battleScript = BattleScript_EffectBrickBreak,
	},
	
	[EFFECT_YAWN] =
	{
		.battleScript = BattleScript_EffectYawn,
	},
	
	[EFFECT_KNOCK_OFF] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_ENDEAVOR] =
	{
		.battleScript = BattleScript_EffectEndeavor,
		.noEffectiveness = TRUE,
	},
	
	[EFFECT_ERUPTION] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_SKILL_SWAP] =
	{
		.battleScript = BattleScript_EffectSkillSwap,
	},
	
	[EFFECT_IMPRISON] =
	{
		.battleScript = BattleScript_EffectImprison,
	},
	
	[EFFECT_REFRESH] =
	{
		.battleScript = BattleScript_EffectRefresh,
	},
	
	[EFFECT_GRUDGE] =
	{
		.battleScript = BattleScript_EffectGrudge,
	},
	
	[EFFECT_SNATCH] =
	{
		.battleScript = BattleScript_EffectSnatch,
		.callOtherMove = TRUE,
	},
	
	[EFFECT_CAMOUFLAGE] =
	{
		.battleScript = BattleScript_EffectCamouflage,
	},
	
	[EFFECT_SPECIAL_ATTACK_UP_3] =
	{
		.battleScript = BattleScript_EffectSpecialAttackUp3,
	},
	
	[EFFECT_MUD_SPORT] =
	{
		.battleScript = BattleScript_EffectFieldSport,
	},
	
	[EFFECT_WEATHER_BALL] =
	{
		.battleScript = BattleScript_EffectHit,
		.normalizeUnaffected = TRUE,
	},
	
	[EFFECT_SPECIAL_DEFENSE_DOWN_2] =
	{
		.battleScript = BattleScript_EffectSpecialDefenseDown2,
	},
	
	[EFFECT_TICKLE] =
	{
		.battleScript = BattleScript_EffectTickle,
	},
	
	[EFFECT_COSMIC_POWER] =
	{
		.battleScript = BattleScript_EffectCosmicPower,
	},
	
	[EFFECT_HOWL] =
	{
		.battleScript = BattleScript_EffectHowl,
		.affectsUserSide = TRUE,
	},
	
	[EFFECT_BULK_UP] =
	{
		.battleScript = BattleScript_EffectBulkUp,
	},
	
	[EFFECT_WATER_SPORT] =
	{
		.battleScript = BattleScript_EffectFieldSport,
	},
	
	[EFFECT_CALM_MIND] =
	{
		.battleScript = BattleScript_EffectCalmMind,
	},
	
	[EFFECT_DRAGON_DANCE] =
	{
		.battleScript = BattleScript_EffectDragonDance,
	},
	
	[EFFECT_ROOST] =
	{
		.battleScript = BattleScript_EffectRoost,
		.healingEffect = TRUE,
	},
	
	[EFFECT_GRAVITY] =
	{
		.battleScript = BattleScript_EffectGravity,
	},
	
	[EFFECT_MIRACLE_EYE] =
	{
		.battleScript = BattleScript_EffectMiracleEye,
	},
	
	[EFFECT_GYRO_BALL] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_HEALING_WISH] =
	{
		.battleScript = BattleScript_EffectHealingWish,
		.healingEffect = TRUE,
	},
	
	[EFFECT_BRINE] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_NATURAL_GIFT] =
	{
		.battleScript = BattleScript_EffectNaturalGift,
		.normalizeUnaffected = TRUE,
	},
	
	[EFFECT_BUG_BITE] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_TAILWIND] =
	{
		.battleScript = BattleScript_EffectTailwind,
		.affectsUserSide = TRUE,
	},
	
	[EFFECT_ACUPRESSURE] =
	{
		.battleScript = BattleScript_EffectAcupressure,
	},
	
	[EFFECT_HIT_ESCAPE] =
	{
		.battleScript = BattleScript_EffectHitEscape,
	},
	
	[EFFECT_PAYBACK] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_ASSURANCE] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_EMBARGO] =
	{
		.battleScript = BattleScript_EffectEmbargo,
	},
	
	[EFFECT_FLING] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_PSYCHO_SHIFT] =
	{
		.battleScript = BattleScript_EffectPsychoShift,
	},
	
	[EFFECT_TRUMP_CARD] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_HEAL_BLOCK] =
	{
		.battleScript = BattleScript_EffectHealBlock,
	},
	
	[EFFECT_WRING_OUT] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_POWER_TRICK] =
	{
		.battleScript = BattleScript_EffectPowerTrick,
	},
	
	[EFFECT_GASTRO_ACID] =
	{
		.battleScript = BattleScript_EffectGastroAcid,
	},
	
	[EFFECT_LUCKY_CHANT] =
	{
		.battleScript = BattleScript_EffectLuckyChant,
		.affectsUserSide = TRUE,
	},
	
	[EFFECT_ME_FIRST] =
	{
		.battleScript = BattleScript_EffectMeFirst,
		.callOtherMove = TRUE,
	},
	
	[EFFECT_COPYCAT] =
	{
		.battleScript = BattleScript_EffectCopycat,
		.callOtherMove = TRUE,
	},
	
	[EFFECT_POWER_SWAP] =
	{
		.battleScript = BattleScript_EffectSplitSwap,
	},
	
	[EFFECT_GUARD_SWAP] =
	{
		.battleScript = BattleScript_EffectSplitSwap,
	},
	
	[EFFECT_PUNISHMENT] =
	{
		.battleScript = BattleScript_EffectHit,
	},
	
	[EFFECT_LAST_RESORT] =
	{
		.battleScript = BattleScript_EffectLastResort,
	},
	
	[EFFECT_SET_ABILITY] =
	{
		.battleScript = BattleScript_EffectSetAbility,
	},
	
	[EFFECT_SUCKER_PUNCH] =
	{
		.battleScript = BattleScript_EffectSuckerPunch,
	},
	
	[EFFECT_TOXIC_SPIKES] =
	{
		.battleScript = BattleScript_EffectToxicSpikes,
	},
	
	[EFFECT_HEART_SWAP] =
	{
		.battleScript = BattleScript_EffectSplitSwap,
	},
	
	/*
	[EFFECT_CHANGE_TYPE_ON_ITEM] =
	{
		.normalizeUnaffected = TRUE,
	},
	
	[EFFECT_TERRAIN_PULSE] =
	{
		.normalizeUnaffected = TRUE,
	},*/
};

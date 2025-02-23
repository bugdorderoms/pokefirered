static const u8* const sBattleAnims_StatusConditions[] =
{
	[B_ANIM_STATUS_PSN]         = gStatusAnim_Poison,
	[B_ANIM_STATUS_CONFUSION]   = gStatusAnim_Confusion,
	[B_ANIM_STATUS_BRN]         = gStatusAnim_Burn,
	[B_ANIM_STATUS_INFATUATION] = gStatusAnim_Infatuation,
	[B_ANIM_STATUS_SLP]         = gStatusAnim_Sleep,
	[B_ANIM_STATUS_PRZ]         = gStatusAnim_Paralysis,
	[B_ANIM_STATUS_FRZ]         = gStatusAnim_Freeze,
	[B_ANIM_STATUS_CURSED]      = gStatusAnim_Curse,
	[B_ANIM_STATUS_NIGHTMARE]   = gStatusAnim_Nightmare,
};

static const u8* const sBattleAnims_Special[] =
{
	[B_ANIM_LVL_UP]                  = gSpecialAnim_LvlUp,
	[B_ANIM_SWITCH_OUT_PLAYER_MON]   = gSpecialAnim_SwitchOutMon,
	[B_ANIM_SWITCH_OUT_OPPONENT_MON] = gSpecialAnim_SwitchOutMon,
	[B_ANIM_BALL_THROW]              = gSpecialAnim_BallThrow,
	[B_ANIM_SAFARI_BALL_THROW]       = gSpecialAnim_SafariBallThrow,
	[B_ANIM_SUBSTITUTE_TO_MON]       = gSpecialAnim_SubstituteToMon,
	[B_ANIM_MON_TO_SUBSTITUTE]       = gSpecialAnim_MonToSubstitute,
	[B_ANIM_CRITICAL_CAPTURE_THROW]  = gSpecialAnim_CriticalCaptureBallThrow,
};

const struct BattleAnimTable gBattleAnims_General[] =
{
	[B_ANIM_FORM_CHANGE] =
	{
		.script = gGeneralAnim_FormChange,
		.hideHpBoxes = TRUE,
		.shouldBePlayed = TRUE,
		.substituteRecede = TRUE,
		.changeForm = TRUE,
	},
	[B_ANIM_STATS_CHANGE] =
	{
		.script = gGeneralAnim_StatsChange,
	},
	[B_ANIM_SUBSTITUTE_FADE] =
	{
		.script = gGeneralAnim_SubstituteFade,
		.shouldBePlayed = TRUE,
		.ignoreSubstitute = TRUE,
	},
	[B_ANIM_SUBSTITUTE_APPEAR] =
	{
		.script = gGeneralAnim_SubstituteAppear,
	},
	[B_ANIM_BAIT_THROW] =
	{
		.script = gGeneralAnim_BaitThrow,
	},
	[B_ANIM_ITEM_KNOCKOFF] =
	{
		.script = gGeneralAnim_ItemKnockOff,
	},
	[B_ANIM_TURN_TRAP] =
	{
		.script = gGeneralAnim_TurnTrap,
		.hideHpBoxes = TRUE,
	},
	[B_ANIM_ITEM_EFFECT] =
	{
		.script = gGeneralAnim_ItemEffect,
	},
	[B_ANIM_SMOKEBALL_ESCAPE] =
	{
		.script = gGeneralAnim_SmokeballEscape,
	},
	[B_ANIM_HANGED_ON] =
	{
		.script = gGeneralAnim_HangedOn,
	},
	[B_ANIM_RAIN_CONTINUES] =
	{
		.script = gGeneralAnim_Rain,
		.ignoreSemiInvulnerability = TRUE,
		.ignoreSubstitute = TRUE,
	},
	[B_ANIM_SUN_CONTINUES] =
	{
		.script = gMoveAnim_SUNNY_DAY,
		.ignoreSemiInvulnerability = TRUE,
		.ignoreSubstitute = TRUE,
	},
	[B_ANIM_SANDSTORM_CONTINUES] =
	{
		.script = gMoveAnim_SANDSTORM,
		.ignoreSemiInvulnerability = TRUE,
		.ignoreSubstitute = TRUE,
	},
	[B_ANIM_HAIL_CONTINUES] =
	{
		.script = gMoveAnim_HAIL,
		.ignoreSemiInvulnerability = TRUE,
		.ignoreSubstitute = TRUE,
	},
	[B_ANIM_LEECH_SEED_DRAIN] =
	{
		.script = gGeneralAnim_LeechSeedDrain,
		.hideHpBoxes = TRUE,
	},
	[B_ANIM_MON_HIT] =
	{
		.script = gMoveAnim_POUND,
		.hideHpBoxes = TRUE,
	},
	[B_ANIM_ITEM_STEAL] =
	{
		.script = gGeneralAnim_ItemSteal,
	},
	[B_ANIM_SNATCH_MOVE] =
	{
		.script = gGeneralAnim_SnatchMove,
		.hideHpBoxes = TRUE,
		.shouldBePlayed = TRUE,
		.ignoreSubstitute = TRUE,
	},
	[B_ANIM_FUTURE_SIGHT_HIT] =
	{
		.script = gGeneralAnim_FutureSightHit,
		.hideHpBoxes = TRUE,
	},
	[B_ANIM_DOOM_DESIRE_HIT] =
	{
		.script = gGeneralAnim_DoomDesireHit,
		.hideHpBoxes = TRUE,
	},
	[B_ANIM_FOCUS_PUNCH_SETUP] =
	{
		.script = gMoveAnim_ENDURE,
	},
	[B_ANIM_INGRAIN_HEAL] =
	{
		.script = gMoveAnim_INGRAIN,
	},
	[B_ANIM_WISH_HEAL] =
	{
		.script = gGeneralAnim_WishHeal,
		.hideHpBoxes = TRUE,
	},
	[B_ANIM_MON_SCARED] =
	{
		.script = gGeneralAnim_MonScared,
	},
	[B_ANIM_GHOST_GET_OUT] =
	{
		.script = gGeneralAnim_GhostGetOut,
	},
	[B_ANIM_SILPH_SCOPED] =
	{
		.script = gGeneralAnim_SilphScoped,
		.shouldBePlayed = TRUE,
	},
	[B_ANIM_ROCK_THROW] =
	{
		.script = gGeneralAnim_SafariRockThrow,
	},
	[B_ANIM_SAFARI_REACTION] =
	{
		.script = gGeneralAnim_SafariReaction,
	},
	[B_ANIM_LOAD_ABILITY_POP_UP] =
	{
		.script = gGeneralAnim_LoadAbilityPopUp,
		.ignoreSubstitute = TRUE,
	},
	[B_ANIM_REMOVE_ABILITY_POP_UP] =
	{
		.script = gGeneralAnim_RemoveAbilityPopUp,
		.ignoreSubstitute = TRUE,
	},
	[B_ANIM_ILLUSION_OFF] =
	{
		.script = gGeneralAnim_FormChange,
		.hideHpBoxes = TRUE,
		.shouldBePlayed = TRUE,
		.substituteRecede = TRUE,
	},
	[B_ANIM_FOG_CONTINUES] =
	{
		.script = gMoveAnim_HAZE,
		.ignoreSemiInvulnerability = TRUE,
		.ignoreSubstitute = TRUE,
	},
	[B_ANIM_SLIDE_OUT_OFFSCREEN] =
	{
		.script = gGeneralAnim_SlideOutOffscreen,
	},
	[B_ANIM_ITEM_THROW] =
	{
		.script = gGeneralAnim_ItemThrow,
		.ignoreSubstitute = TRUE,
	},
	[B_ANIM_STRONG_WINDS_CONTINUE] =
	{
		.script = gMoveAnim_TAILWIND,
		.ignoreSemiInvulnerability = TRUE,
		.ignoreSubstitute = TRUE,
	},
	[B_ANIM_COMMANDER] =
	{
		.script = gGeneralAnim_Commander,
		.shouldBePlayed = TRUE,
		.ignoreSubstitute = TRUE,
		.substituteRecede = TRUE,
	},
	[B_ANIM_UPDATE_ABILITY_POP_UP] =
	{
		.script = gGeneralAnim_UpdateAbilityPopUp,
		.ignoreSubstitute = TRUE,
	},
	[B_ANIM_HEALING_WISH_HEAL] =
	{
		.script = gGeneralAnim_HealingWishHeal,
		.hideHpBoxes = TRUE,
	},
	[B_ANIM_SET_TOXIC_SPIKES] =
	{
		.script = gMoveAnim_TOXIC_SPIKES,
	},
	[B_ANIM_MEGA_EVOLUTION] =
	{
		.script = gGeneralAnim_MegaEvolution,
		.hideHpBoxes = TRUE,
		.shouldBePlayed = TRUE,
		.substituteRecede = TRUE,
		.changeForm = TRUE,
	},
	[B_ANIM_BATTLE_BOND] =
	{
		.script = gGeneralAnim_BattleBond,
		.hideHpBoxes = TRUE,
		.shouldBePlayed = TRUE,
		.substituteRecede = TRUE,
		.changeForm = TRUE,
	},
	[B_ANIM_ZYGARDE_CELL_SWIRL] =
	{
		.script = gGeneralAnim_ZygardeCellSwirl,
		.hideHpBoxes = TRUE,
		.shouldBePlayed = TRUE,
		.substituteRecede = TRUE,
		.changeForm = TRUE,
	},
	[B_ANIM_SCHOOLING] =
	{
		.script = gGeneralAnim_Schooling,
		.hideHpBoxes = TRUE,
		.shouldBePlayed = TRUE,
		.substituteRecede = TRUE,
		.changeForm = TRUE,
	},
	[B_ANIM_SPIT_OUT_PREY] =
	{
		.script = gGeneralAnim_SpitOutPrey,
		.hideHpBoxes = TRUE,
		.shouldBePlayed = TRUE,
		.substituteRecede = TRUE,
		.changeForm = TRUE,
	},
	[B_ANIM_SILENT_FORM_CHANGE] =
	{
		.script = gGeneralAnim_SilentFormChange,
		.shouldBePlayed = TRUE,
		.substituteRecede = TRUE,
		.changeForm = TRUE,
	},
	[B_ANIM_TOTEM_BOOST] =
	{
		.script = gGeneralAnim_TotemBoost,
		.hideHpBoxes = TRUE,
	},
	[B_ANIM_TAILWIND] =
	{
		.script = gMoveAnim_TAILWIND,
		.ignoreSemiInvulnerability = TRUE,
		.ignoreSubstitute = TRUE,
	},
	[B_ANIM_AQUA_RING_HEAL] =
	{
		.script = gGeneralAnim_AquaRingHeal,
	},
};

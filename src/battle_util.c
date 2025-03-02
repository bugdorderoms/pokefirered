#include "global.h"
#include "item.h"
#include "text.h"
#include "util.h"
#include "link.h"
#include "random.h"
#include "battle_interface.h"
#include "pokemon.h"
#include "string_util.h"
#include "battle_gimmicks.h"
#include "field_weather.h"
#include "form_change.h"
#include "battle_move_effects.h"
#include "event_data.h"
#include "mail.h"
#include "evolution.h"
#include "mail_data.h"
#include "party_menu.h"
#include "pokedex.h"
#include "strings.h"
#include "sprite.h"
#include "malloc.h"
#include "battle.h"
#include "battle_queued_effects.h"
#include "battle_anim.h"
#include "battle_scripts.h"
#include "battle_message.h"
#include "battle_gfx_sfx_util.h"
#include "battle_controllers.h"
#include "battle_ai_util.h"
#include "calculate_base_damage.h"
#include "wild_encounter.h"
#include "set_effect.h"
#include "constants/battle_string_ids.h"
#include "constants/battle_anim.h"
#include "constants/battle.h"
#include "constants/form_change.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/weather.h"
#include "constants/abilities.h"
#include "constants/pokemon.h"
#include "constants/hold_effects.h"
#include "constants/battle_script_commands.h"
#include "constants/pokedex.h"

static const u8 sWeatherIconRainGfx[] = INCBIN_U8("graphics/battle_interface/weather_icon_rain.4bpp");
static const u8 sWeatherIconSunGfx[] = INCBIN_U8("graphics/battle_interface/weather_icon_sun.4bpp");
static const u8 sWeatherIconHailGfx[] = INCBIN_U8("graphics/battle_interface/weather_icon_hail.4bpp");
static const u8 sWeatherIconSandstormGfx[] = INCBIN_U8("graphics/battle_interface/weather_icon_sandstorm.4bpp");
static const u8 sWeatherIconFogGfx[] = INCBIN_U8("graphics/battle_interface/weather_icon_fog.4bpp");
static const u8 sWeatherIconStrongWindsGfx[] = INCBIN_U8("graphics/battle_interface/weather_icon_strong_winds.4bpp");

static const u16 sWeatherIconRainPal[] = INCBIN_U16("graphics/battle_interface/weather_icon_rain.gbapal");
static const u16 sWeatherIconSunPal[] = INCBIN_U16("graphics/battle_interface/weather_icon_sun.gbapal");
static const u16 sWeatherIconHailPal[] = INCBIN_U16("graphics/battle_interface/weather_icon_hail.gbapal");
static const u16 sWeatherIconSandstormPal[] = INCBIN_U16("graphics/battle_interface/weather_icon_sandstorm.gbapal");
static const u16 sWeatherIconFogPal[] = INCBIN_U16("graphics/battle_interface/weather_icon_fog.gbapal");

struct BattleWeatherInfo
{
	const struct SpriteSheet iconGfx;
	const struct SpritePalette iconPalette;
	u16 flag;
	u8 continueMessage;
	u8 endMessage;
	u8 animation;
};

static const u8 sTargetPositions[B_SIDE_COUNT][NUM_BATTLERS_PER_SIDE] =
{
	[B_SIDE_PLAYER]   = {B_POSITION_OPPONENT_LEFT, B_POSITION_OPPONENT_RIGHT},
	[B_SIDE_OPPONENT] = {B_POSITION_PLAYER_LEFT, B_POSITION_PLAYER_RIGHT},
};

// icon palette is the optional param
#define WEATHER_ICON(_weather, ...)                                                                      \
	.iconGfx = {sWeatherIcon##_weather##Gfx, sizeof(sWeatherIcon##_weather##Gfx), TAG_WEATHER_ICON_GFX}, \
	.iconPalette = {CONCAT(sWeatherIcon, DEFAULT(_weather, __VA_ARGS__), Pal), TAG_WEATHER_ICON_GFX}

static const struct BattleWeatherInfo sBattleWeatherInfo[ENUM_WEATHER_COUNT] =
{
	[ENUM_WEATHER_RAIN] =
	{
		.flag = B_WEATHER_RAIN_NORMAL,
		.continueMessage = B_MSG_RAIN_CONTINUES,
		.endMessage = B_MSG_RAIN_STOPPED,
		.animation = B_ANIM_RAIN_CONTINUES,
		WEATHER_ICON(Rain),
	},
	[ENUM_WEATHER_RAIN_PRIMAL] =
	{
		.flag = B_WEATHER_RAIN_PRIMAL,
		.continueMessage = B_MSG_RAIN_CONTINUES,
		.endMessage = B_MSG_RAIN_STOPPED,
		.animation = B_ANIM_RAIN_CONTINUES,
		WEATHER_ICON(Rain),
	},
	[ENUM_WEATHER_SUN] =
	{
		.flag = B_WEATHER_SUN_NORMAL,
		.continueMessage = B_MSG_SUN_CONTINUES,
		.endMessage = B_MSG_SUN_STOPPED,
		.animation = B_ANIM_SUN_CONTINUES,
		WEATHER_ICON(Sun),
	},
	[ENUM_WEATHER_SUN_PRIMAL] =
	{
		.flag = B_WEATHER_SUN_PRIMAL,
		.continueMessage = B_MSG_SUN_CONTINUES,
		.endMessage = B_MSG_SUN_STOPPED,
		.animation = B_ANIM_SUN_CONTINUES,
		WEATHER_ICON(Sun),
	},
	[ENUM_WEATHER_SANDSTORM] =
	{
		.flag = B_WEATHER_SANDSTORM,
		.continueMessage = B_MSG_SANDSTORM_CONTINUES,
		.endMessage = B_MSG_SANDSTORM_STOPPED,
		.animation = B_ANIM_SANDSTORM_CONTINUES,
		WEATHER_ICON(Sandstorm),
	},
	[ENUM_WEATHER_HAIL] =
	{
		.flag = B_WEATHER_HAIL,
		.continueMessage = B_MSG_HAIL_CONTINUES,
		.endMessage = B_MSG_HAIL_STOPPED,
		.animation = B_ANIM_HAIL_CONTINUES,
		WEATHER_ICON(Hail),
	},
	[ENUM_WEATHER_FOG] =
	{
		.flag = B_WEATHER_FOG,
		.continueMessage = B_MSG_FOG_CONTINUES,
		.endMessage = B_MSG_FOG_STOPPED,
		.animation = B_ANIM_FOG_CONTINUES,
		WEATHER_ICON(Fog),
	},
	[ENUM_WEATHER_STRONG_WINDS] =
	{
		.flag = B_WEATHER_STRONG_WINDS,
		.continueMessage = B_MSG_STRONG_WINDS_CONTINUES,
		.endMessage = B_MSG_STRONG_WINDS_CONTINUES,
		.animation = B_ANIM_STRONG_WINDS_CONTINUE,
		WEATHER_ICON(StrongWinds, Fog), // Same palette as fog icon
	},
};

static const struct SosCall sSosCallRateTable[] =
{
	{.chainLength =  4, .nPerfectIvs = 0, .hiddenAbilityChance =  0, .shinyRolls = 1}, // No base call rate
	{.chainLength =  5, .nPerfectIvs = 1, .hiddenAbilityChance =  0, .shinyRolls = 1},
	{.chainLength = 10, .nPerfectIvs = 2, .hiddenAbilityChance =  5, .shinyRolls = 1},
	{.chainLength = 11, .nPerfectIvs = 2, .hiddenAbilityChance =  5, .shinyRolls = 5},
	{.chainLength = 20, .nPerfectIvs = 3, .hiddenAbilityChance = 10, .shinyRolls = 5},
	{.chainLength = 21, .nPerfectIvs = 3, .hiddenAbilityChance = 10, .shinyRolls = 9},
	{.chainLength = 30, .nPerfectIvs = 4, .hiddenAbilityChance = 15, .shinyRolls = 9},
	{.chainLength = 31, .nPerfectIvs = 4, .hiddenAbilityChance = 15, .shinyRolls = 13},
};

const u16 gInobedientStringIds[] =
{
    [B_MSG_LOAFING_AROUND] = STRINGID_ATKISLOAFINGAROUND,
    STRINGID_ATKWONTOBEY,
    STRINGID_ATKTURNEDAWAY,
    STRINGID_ATKPRETENDNOTNOTICE
};

#define CHECK_SIDE_PROTECT_ABILITY(battler, abilityId, retValue) \
{                                                                \
	ret = ABILITY_ON_SIDE(battler, abilityId);                   \
	                                                             \
	if (ret)                                                     \
	{                                                            \
		gBattleScripting.battlerWithAbility = ret - 1;           \
		return retValue;                                         \
	}                                                            \
}

// Check if attacker can put a status condition on defender
static u8 CanBeStatused(u8 attacker, u8 defender, u32 flags)
{
	// Check already statused
	if (!(flags & STATUS_CHANGE_FLAG_IGNORE_GENERAL_STATUS) && gBattleMons[defender].status1.id)
		return STATUS_CHANGE_FAIL_ALREADY_STATUSED;
	
	// Check defender's abilities
	switch (GetBattlerAbility(defender))
	{
		case ABILITY_LEAF_GUARD:
			if (IsBattlerWeatherAffected(defender, B_WEATHER_SUN_ANY))
				return STATUS_CHANGE_FAIL_ABILITY_PREVENTED;
			break;
		case ABILITY_SHIELDS_DOWN:
		    if (!(gBattleMons[defender].status2 & STATUS2_TRANSFORMED) && gBattleMons[defender].species == SPECIES_MINIOR_METEOR)
				return STATUS_CHANGE_FAIL_ABILITY_PREVENTED;
			break;
		case ABILITY_COMATOSE:
		case ABILITY_PURIFYING_SALT:
		    return STATUS_CHANGE_FAIL_ABILITY_PREVENTED;
	}
	
	// Check Safeguard and Flower Veil
	if (!(flags & STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD))
	{
		if (CanSafeguardProtectBattler(attacker, defender))
			return STATUS_CHANGE_FAIL_SAFEGUARD_PROTECTED;
		else if (IsBattlerProtectedByFlowerVeil(defender))
			return STATUS_CHANGE_FAIL_FLOWER_VEIL_ON_SIDE;
	}
	return STATUS_CHANGE_WORKED;
}

// Check if attacker can put defender to sleep
u8 CanBePutToSleep(u8 attacker, u8 defender, u32 flags)
{
	u8 ret;
	
	// Check already sleeping
	if (gBattleMons[defender].status1.id == STATUS1_SLEEP)
		return STATUS_CHANGE_FAIL_SPECIFIC_STATUSED;
	
	// Check general statuses
	ret = CanBeStatused(attacker, defender, flags);
	
	if (ret != STATUS_CHANGE_WORKED)
		return ret;
	
	// Check Uproar
	if ((flags & STATUS_CHANGE_FLAG_CHECK_UPROAR) && UproarWakeUpCheck())
		return STATUS_CHANGE_FAIL_UPROAR;
	
	// Check defender's abilities
	switch (GetBattlerAbility(defender))
	{
		case ABILITY_INSOMNIA:
		case ABILITY_VITAL_SPIRIT:
			return STATUS_CHANGE_FAIL_ABILITY_PREVENTED;
	}
	// Check side abilities
	CHECK_SIDE_PROTECT_ABILITY(defender, ABILITY_SWEET_VEIL, STATUS_CHANGE_FAIL_SWEET_VEIL_ON_SIDE)
	
	return STATUS_CHANGE_WORKED;
}

static bool8 CanPoisonType(u8 attacker, u8 defender)
{
	if (GetBattlerAbility(attacker) == ABILITY_CORROSION && IS_MOVE_STATUS(gCurrentMove))
		return TRUE;
	else if (IsBattlerAnyType(defender, TYPE_POISON, TYPE_STEEL))
		return FALSE;
	else
		return TRUE;
}

// Check if attacker can poison defender
u8 CanBePoisoned(u8 attacker, u8 defender, u32 flags)
{
	u8 ret;
	
	// Check already poisoned
	if (gBattleMons[defender].status1.id == STATUS1_POISON || gBattleMons[defender].status1.id == STATUS1_TOXIC_POISON)
		return STATUS_CHANGE_FAIL_SPECIFIC_STATUSED;
	
	// Check general statuses
	ret = CanBeStatused(attacker, defender, flags);
	
	if (ret != STATUS_CHANGE_WORKED)
		return ret;
	
	// Check defender's abilities
	switch (GetBattlerAbility(defender))
	{
		case ABILITY_IMMUNITY:
			return STATUS_CHANGE_FAIL_ABILITY_PREVENTED;
	}
	// Check side abilities
	CHECK_SIDE_PROTECT_ABILITY(defender, ABILITY_PASTEL_VEIL, STATUS_CHANGE_FAIL_PASTEL_VEIL_ON_SIDE)
	
	// Check types
	if (!CanPoisonType(attacker, defender))
		return STATUS_CHANGE_FAIL_TYPE_NOT_AFFECTED;
	
	return STATUS_CHANGE_WORKED;
}

// Check if attacker can burn defender
u8 CanBeBurned(u8 attacker, u8 defender, u32 flags)
{
	u8 ret;
	
	// Check already burned
	if (gBattleMons[defender].status1.id == STATUS1_BURN)
		return STATUS_CHANGE_FAIL_SPECIFIC_STATUSED;
	
	// Check general statuses
	ret = CanBeStatused(attacker, defender, flags);
	
	if (ret != STATUS_CHANGE_WORKED)
		return ret;
	
	// Check defender's abilities
	switch (GetBattlerAbility(defender))
	{
		case ABILITY_WATER_VEIL:
		case ABILITY_WATER_BUBBLE:
		case ABILITY_THERMAL_EXCHANGE:
			return STATUS_CHANGE_FAIL_ABILITY_PREVENTED;
	}
	// Check types
	if (IsBattlerOfType(defender, TYPE_FIRE))
		return STATUS_CHANGE_FAIL_TYPE_NOT_AFFECTED;
	
	return STATUS_CHANGE_WORKED;
}

// Check if attacker can frozen defender
bool8 CanBeFrozen(u8 attacker, u8 defender, u32 flags)
{
	// Check general statuses
	if (CanBeStatused(attacker, defender, flags) != STATUS_CHANGE_WORKED)
		return FALSE;
	
	// Check defender's abilities
	switch (GetBattlerAbility(defender))
	{
		case ABILITY_MAGMA_ARMOR:
			return FALSE;
	}
	if (IsBattlerOfType(defender, TYPE_ICE)) // Check types
		return FALSE;
	else if (IsBattlerWeatherAffected(defender, B_WEATHER_SUN_ANY)) // Check weather
		return FALSE;
	
	return TRUE;
}

// Check if attacker can paralyse defender
u8 CanBeParalyzed(u8 attacker, u8 defender, u32 flags)
{
	u8 ret;
	
	// Check already paralized
	if (gBattleMons[defender].status1.id == STATUS1_PARALYSIS)
		return STATUS_CHANGE_FAIL_SPECIFIC_STATUSED;
	
	// Check general statuses
	ret = CanBeStatused(attacker, defender, flags);
	
	if (ret != STATUS_CHANGE_WORKED)
		return ret;
	
	// Check defender's abilities
	switch (GetBattlerAbility(defender))
	{
		case ABILITY_LIMBER:
			return STATUS_CHANGE_FAIL_ABILITY_PREVENTED;
	}
	// Check types
	if (IsBattlerOfType(defender, TYPE_ELECTRIC))
		return STATUS_CHANGE_FAIL_TYPE_NOT_AFFECTED;
	
	return STATUS_CHANGE_WORKED;
}

u8 CanBecameConfused(u8 attacker, u8 defender, u32 flags)
{
	// Check defender's abilities
	if (GetBattlerAbility(defender) == ABILITY_OWN_TEMPO)
		return STATUS_CHANGE_FAIL_ABILITY_PREVENTED;
	
	// Check already confused
	if (gBattleMons[defender].status2 & STATUS2_CONFUSION)
		return STATUS_CHANGE_FAIL_SPECIFIC_STATUSED;
	
	// Check Safeguard
	if (!(flags & STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD) && CanSafeguardProtectBattler(attacker, defender))
		return STATUS_CHANGE_FAIL_SAFEGUARD_PROTECTED;
	
	return STATUS_CHANGE_WORKED;
}

u8 GetBattlerForBattleScript(u8 caseId)
{
    u32 ret = 0;

    switch (caseId)
    {
    case BS_TARGET:
        ret = gBattlerTarget;
        break;
    case BS_ATTACKER:
        ret = gBattlerAttacker;
        break;
    case BS_EFFECT_BATTLER:
        ret = gEffectBattler;
        break;
    case BS_SCRIPTING:
        ret = gBattleScripting.battler;
        break;
    case BS_FAINTED:
	case 5:
        ret = gBattlerFainted;
        break;
    case BS_PLAYER1:
        ret = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        break;
    case BS_OPPONENT1:
        ret = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
        break;
	case BS_SCRIPTING_PARTNER:
	    ret = BATTLE_PARTNER(gBattleScripting.battler);
		break;
    }
    return ret;
}

void MarkBattlerForControllerExec(u8 battlerId)
{
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        gBattleControllerExecFlags |= gBitTable[battlerId] << 0x1C;
    else
        gBattleControllerExecFlags |= gBitTable[battlerId];
}

void MarkBattlerReceivedLinkData(u8 battlerId)
{
    s32 i, count = GetLinkPlayerCount();

    for (i = 0; i < count; ++i)
        gBattleControllerExecFlags |= gBitTable[battlerId] << (i << 2);

    gBattleControllerExecFlags &= ~(0x10000000 << battlerId);
}

void CancelMultiTurnMoves(u8 battler)
{
    gBattleMons[battler].status2 &= ~(STATUS2_MULTIPLETURNS | STATUS2_LOCK_CONFUSE | STATUS2_UPROAR | STATUS2_BIDE);
    gStatuses3[battler] &= ~(STATUS3_SEMI_INVULNERABLE);
    gDisableStructs[battler].rolloutTimer = 0;
    gDisableStructs[battler].furyCutterCounter = 0;
}

bool8 WasUnableToUseMove(u8 battler)
{
    if (gProtectStructs[battler].usedHealBlockedMove || gProtectStructs[battler].usedGravityBannedMove || gProtectStructs[battler].usedImprisonedMove
	|| gProtectStructs[battler].prlzImmobility || gProtectStructs[battler].loveImmobility || gProtectStructs[battler].usedDisabledMove || gProtectStructs[battler].usedTauntedMove
    || gProtectStructs[battler].flinchImmobility || gProtectStructs[battler].confusionSelfDmg || gProtectStructs[battler].targetNotAffected)
        return TRUE;
    else
        return FALSE;
}

// Used internaly to check if the battler's ability has't be suppressed
static u16 GetBattlerSupressedAbility(u8 battler, bool8 forAI)
{
	if (forAI && !BattleAI_KnowsBattlerAbility(battler)) // Ignore player abilities that the AI don't have the knowledge
		return ABILITY_NONE;
	else if (gStatuses3[battler] & STATUS3_GASTRO_ACID) // Check battle effects of suppression
		return ABILITY_NONE;
	else // Ability is'nt suppressed
		return gBattleMons[battler].ability;
}

// Check Neutralizing Gas on field blocking the ability
static bool8 IsNeutralizingGasOnField(u16 ability, bool8 forAI)
{
	u8 i;
	bool8 foundOnField = FALSE;
	
	for (i = 0; i < gBattlersCount; i++)
	{
		if (IsBattlerAlive(i) && GetBattlerSupressedAbility(i, forAI) == ABILITY_NEUTRALIZING_GAS)
		{
			foundOnField = TRUE;
			break;
		}
	}
	return foundOnField ? !gAbilities[ability].cantBeSuppressed : FALSE;
}

static inline bool8 IsMoldBreakerTypeAbility(u8 battlerId)
{
	u16 attackerAbility = GetBattlerAbility(battlerId);
	
	if (attackerAbility == ABILITY_MOLD_BREAKER || attackerAbility == ABILITY_TERAVOLT || attackerAbility == ABILITY_TURBOBLAZE
	|| (attackerAbility == ABILITY_MYCELIUM_MIGHT && IS_MOVE_STATUS(gCurrentMove)))
		return TRUE;
		
	return FALSE;
}

// Used to get the battler's ability. Passing throug all effects that can suppress/block the ability effect, like Gastro Acid and Mycelium Might
u16 GetBattlerAbility(u8 battler)
{
	bool8 forAI = BattleAI_IsRunning();
	u16 ability = GetBattlerSupressedAbility(battler, forAI);
	
	if (IsNeutralizingGasOnField(ability, forAI))
		return ABILITY_NONE;
	
	if (gAbilities[ability].breakable && battler != gCurrentTurnActionBattlerId)
	{
		if ((forAI || (gCurrentTurnActionNumber < gBattlersCount && gBattlerByTurnOrder[gCurrentTurnActionNumber] == gCurrentTurnActionBattlerId
		&& gActionsByTurnOrder[gCurrentTurnActionNumber] == B_ACTION_USE_MOVE)) && IsMoldBreakerTypeAbility(gCurrentTurnActionBattlerId))
			return ABILITY_NONE;
	}
	return ability;
}

// Used internaly by ability change/suppression to clear vars related to it (e.g suppressing or removing Neutralizing Gas will cause it's effect to wears off)
static void TryEndAbilityEffectOnSuppression(u8 battlerId, u16 oldAbility, u16 newAbility)
{
	if (oldAbility == ABILITY_NEUTRALIZING_GAS && newAbility != ABILITY_NEUTRALIZING_GAS)
		gSpecialStatuses[battlerId].removedNeutralizingGas = TRUE; // for display the message and activate switch in abilities
	else if ((oldAbility == ABILITY_FORECAST && newAbility != ABILITY_FORECAST) || (oldAbility == ABILITY_FLOWER_GIFT && newAbility != ABILITY_FLOWER_GIFT))
		gSpecialStatuses[battlerId].removedWeatherChangeAbility = TRUE; // for transform the mon back
}

// Used in abilities change to clear vars related to it (e.g removing Unburden from a battler will cause it to lose it's boost)
// Returns the old ability of the pokemon
u16 SetBattlerAbility(u8 battlerId, u16 newAbility)
{
	u16 oldAbility = gBattleMons[battlerId].ability;
	
	gBattleMons[battlerId].ability = newAbility;
	
	// Separated of other abilities bc it only loses it's boost when replaced. If suppressed and then regained the boost is restored.
	if (newAbility != ABILITY_UNBURDEN) // Don't need check the old ability as well
		gDisableStructs[battlerId].unburdenBoost = FALSE;
	
	TryEndAbilityEffectOnSuppression(battlerId, oldAbility, newAbility);
	
	return oldAbility;
}

// Used in abilities suppression. Does nothing to suppress the ability itself, it's only used to clear vars related to it
// Returns the ability suppressed of the pokemon
u16 SuppressBattlerAbility(u8 battlerId)
{
	u16 suppressedAbility = gBattleMons[battlerId].ability;
	
	TryEndAbilityEffectOnSuppression(battlerId, suppressedAbility, ABILITY_NONE); // The suppressed ability has no effect now, so passes it as ABILITY_NONE
	
	return suppressedAbility;
}

void CheckSetBattlerUnburden(u8 battler)
{
	if (GetBattlerAbility(battler) == ABILITY_UNBURDEN)
		gDisableStructs[battler].unburdenBoost = TRUE;
}

static bool8 BattlerHeldItemCanBeUsed(u8 battlerId)
{
	if ((gStatuses3[battlerId] & STATUS3_EMBARGO) || GetBattlerAbility(battlerId) == ABILITY_KLUTZ || (gFieldStatus & STATUS_FIELD_MAGIC_ROOM))
		return FALSE;
	return TRUE;
}

u16 GetBattlerItem(u8 battlerId)
{
	if (BattleAI_IsRunning() && !BattleAI_KnowsBattlerItem(battlerId))
		return ITEM_NONE;
	
	return gBattleMons[battlerId].item;
}

u8 GetBattlerItemHoldEffect(u8 battler, bool8 checkNegating)
{
	if (checkNegating && !BattlerHeldItemCanBeUsed(battler))
		return HOLD_EFFECT_NONE;
	
	return ItemId_GetHoldEffect(GetBattlerItem(battler));
}

void PrepareStringBattle(u16 stringId, u8 battler)
{
    BtlController_EmitPrintString(battler, BUFFER_A, stringId);
    MarkBattlerForControllerExec(battler);
}

void ResetSentPokesToOpponentValue(void)
{
    s32 i;
    u32 bits = 0;

    gSentPokesToOpponent[0] = 0;
    gSentPokesToOpponent[1] = 0;
	
    for (i = 0; i < gBattlersCount; i += 2)
        bits |= gBitTable[gBattlerPartyIndexes[i]];
	
    for (i = 1; i < gBattlersCount; i += 2)
        gSentPokesToOpponent[(i & BIT_FLANK) >> 1] = bits;
}

void OpponentSwitchInResetSentPokesToOpponentValue(u8 battler)
{
    if (GetBattlerSide(battler) == B_SIDE_OPPONENT)
    {
		u32 bits = 0;
        u8 i, flank = ((battler & BIT_FLANK) >> 1);
        gSentPokesToOpponent[flank] = 0;

        for (i = 0; i < gBattlersCount; i += 2)
		{
            if (!(gAbsentBattlerFlags & gBitTable[i]))
                bits |= gBitTable[gBattlerPartyIndexes[i]];
		}
        gSentPokesToOpponent[flank] = bits;
    }
}

void UpdateSentPokesToOpponentValue(u8 battler)
{
    if (GetBattlerSide(battler) == B_SIDE_OPPONENT)
        OpponentSwitchInResetSentPokesToOpponentValue(battler);
    else
    {
        u8 i;
        
        for (i = 1; i < gBattlersCount; ++i)
            gSentPokesToOpponent[(i & BIT_FLANK) >> 1] |= gBitTable[gBattlerPartyIndexes[battler]];
    }
}

void BattleScriptPush(const u8 *bsPtr)
{
    gBattleResources->battleScriptsStack->ptr[gBattleResources->battleScriptsStack->size++] = bsPtr;
}

void BattleScriptPushCursor(void)
{
    gBattleResources->battleScriptsStack->ptr[gBattleResources->battleScriptsStack->size++] = gBattlescriptCurrInstr;
}

void BattleScriptPop(void)
{
    gBattlescriptCurrInstr = gBattleResources->battleScriptsStack->ptr[--gBattleResources->battleScriptsStack->size];
}

void BattleScriptCall(const u8 *bsPtr)
{
	BattleScriptPushCursor();
	gBattlescriptCurrInstr = bsPtr;
}

void BattleScriptExecute(const u8 *BS_ptr)
{
    gBattlescriptCurrInstr = BS_ptr;
    gBattleResources->battleCallbackStack->function[gBattleResources->battleCallbackStack->size++] = gBattleMainFunc;
    gBattleMainFunc = RunBattleScriptCommands_PopCallbacksStack;
    gCurrentActionFuncId = 0;
}

void BattleScriptPushCursorAndCallback(const u8 *BS_ptr)
{
	BattleScriptCall(BS_ptr);
    gBattleResources->battleCallbackStack->function[gBattleResources->battleCallbackStack->size++] = gBattleMainFunc;
    gBattleMainFunc = RunBattleScriptCommands;
}

static u8 GetImprisonedMovesCount(u8 battlerId, u16 move)
{
    u8 i, imprisonedMoves = 0;

    for (i = 0; i < gBattlersCount; ++i)
    {
        if ((gStatuses3[i] & STATUS3_IMPRISONED_OTHERS) && !IsBattlerAlly(battlerId, i))
        {
			if (FindMoveSlotInBattlerMoveset(i, move) != MAX_MON_MOVES)
				++imprisonedMoves;
        }
    }
    return imprisonedMoves;
}

bool8 TrySetCantSelectMoveBattleScript(u8 battlerId)
{
	bool8 cantSelect = FALSE;
	u8 holdEffect, movePos = gBattleBufferB[battlerId][2];
	u16 move = gBattleMons[battlerId].moves[movePos];

    if (move && gDisableStructs[battlerId].disabledMove == move)
    {
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_MOVE_DISABLED;
        cantSelect = TRUE;
    }
    else if ((gBattleMons[battlerId].status2 & STATUS2_TORMENT) && move != MOVE_STRUGGLE && move == gBattleStruct->battlers[battlerId].lastMove)
    {
        CancelMultiTurnMoves(battlerId);
		gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_MOVE_TORMENTED;
        cantSelect = TRUE;
    }
    else if (gDisableStructs[battlerId].tauntTimer && IS_MOVE_STATUS(move))
    {
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_MOVE_TAUNTED;
        cantSelect = TRUE;
    }
    else if (GetImprisonedMovesCount(battlerId, move))
    {
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_MOVE_SEALED;
        cantSelect = TRUE;
    }
	else if ((gFieldStatus & STATUS_FIELD_GRAVITY) && gBattleMoves[move].flags.gravityBanned)
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_MOVE_IN_GRAVITY;
		cantSelect = TRUE;
	}
	else if ((gStatuses3[battlerId] & STATUS3_HEAL_BLOCK) && GET_MOVE_MOVEEFFECT_TABLE(move).healingEffect)
	{
		gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_MOVE_HEAL_BLOCKED;
		cantSelect = TRUE;
	}
	holdEffect = GetBattlerItemHoldEffect(battlerId, TRUE);
	
    if (gBattleStruct->battlers[battlerId].choicedMove && gBattleStruct->battlers[battlerId].choicedMove != MOVE_UNAVAILABLE && gBattleStruct->battlers[battlerId].choicedMove != move)
    {
		if (holdEffect == HOLD_EFFECT_CHOICE_ITEM)
		{
			gLastUsedItem = gBattleMons[battlerId].item;
			gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_MOVE_LOCKED_CHOICE_ITEM;
			cantSelect = TRUE;
		}
		else if (GetBattlerAbility(battlerId) == ABILITY_GORILLA_TACTICS)
		{
			gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_MOVE_LOCKED_ABILITY;
			cantSelect = TRUE;
		}
		
		if (cantSelect)
			move = gBattleStruct->battlers[battlerId].choicedMove;
    }
	
    if (!gBattleMons[battlerId].pp[movePos])
    {
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_MOVE_NO_PP;
        cantSelect = TRUE;
    }
	
	if (cantSelect)
	{
		gCurrentMove = move;
		gSelectionBattleScripts[battlerId] = BattleScript_SelectingNotAllowedMove;
	}
	
    return cantSelect;
}

u8 CheckMoveLimitations(u8 battlerId, u32 flags)
{
    u8 i, unusableMoves = 0, holdEffect = GetBattlerItemHoldEffect(battlerId, TRUE);
	u16 move, moves[MAX_MON_MOVES];
	
	GetBattlerMovesArray(battlerId, moves);
	
    for (i = 0; i < MAX_MON_MOVES; ++i)
    {
		move = moves[i];
		
        if (!move)
            unusableMoves |= gBitTable[i];
		
        if (!(flags & MOVE_LIMITATION_IGNORE_NO_PP) && !gBattleMons[battlerId].pp[i])
            unusableMoves |= gBitTable[i];
		
		if (!(flags & MOVE_LIMITATION_IGNORE_IMPRISON) && GetImprisonedMovesCount(battlerId, move))
            unusableMoves |= gBitTable[i];
		
        if (move == gDisableStructs[battlerId].disabledMove)
            unusableMoves |= gBitTable[i];
		
        if ((gBattleMons[battlerId].status2 & STATUS2_TORMENT) && move == gBattleStruct->battlers[battlerId].lastMove)
            unusableMoves |= gBitTable[i];
		
        if (gDisableStructs[battlerId].tauntTimer && IS_MOVE_STATUS(move))
            unusableMoves |= gBitTable[i];
		
        if (gDisableStructs[battlerId].encoreTimer && gDisableStructs[battlerId].encoredMove != move)
            unusableMoves |= gBitTable[i];
		
        if ((holdEffect == HOLD_EFFECT_CHOICE_ITEM || GetBattlerAbility(battlerId) == ABILITY_GORILLA_TACTICS) && gBattleStruct->battlers[battlerId].choicedMove
		&& gBattleStruct->battlers[battlerId].choicedMove != MOVE_UNAVAILABLE && gBattleStruct->battlers[battlerId].choicedMove != move)
            unusableMoves |= gBitTable[i];
			
		if ((gFieldStatus & STATUS_FIELD_GRAVITY) && gBattleMoves[move].flags.gravityBanned)
			unusableMoves |= gBitTable[i];
		
		if ((gStatuses3[battlerId] & STATUS3_HEAL_BLOCK) && GET_MOVE_MOVEEFFECT_TABLE(move).healingEffect)
			unusableMoves |= gBitTable[i];
    }
    return unusableMoves;
}

bool8 AreAllMovesUnusable(u8 battlerId)
{
    u8 unusable = CheckMoveLimitations(battlerId, 0);

    if (unusable == MOVE_LIMITATION_ALL_MOVES_MASK) // All moves are unusable.
    {
		gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_NO_MOVES;
        gProtectStructs[battlerId].noValidMoves = TRUE;
        gSelectionBattleScripts[battlerId] = BattleScript_SelectingNotAllowedMove;
		
        if (IsDoubleBattleForBattler(battlerId))
            gBattleBufferB[battlerId][3] = GetBattlerAtPosition((BATTLE_OPPOSITE(GetBattlerPosition(battlerId))) | (Random() & 2));
        else 
            gBattleBufferB[battlerId][3] = GetBattlerAtPosition(BATTLE_OPPOSITE(GetBattlerPosition(battlerId)));
    }
    else
        gProtectStructs[battlerId].noValidMoves = FALSE;

    return (unusable == MOVE_LIMITATION_ALL_MOVES_MASK);
}

void SaveBattlersHps(void)
{
	u8 i;
	
	for (i = 0; i < MAX_BATTLERS_COUNT; i++)
		gBattleStruct->battlers[i].hpBefore = gBattleMons[i].hp;
}

static inline bool8 HadMoreThanHalfHpNowDoesnt(u8 battlerId)
{
	u16 halfHp = gBattleMons[battlerId].maxHP / 2;
	return (gBattleStruct->battlers[battlerId].hpBefore > halfHp && gBattleMons[battlerId].hp <= halfHp);
}

bool8 TryActivateEmergencyExit(u8 battler)
{
	if (IsBattlerAlive(battler))
	{
		u16 ability = GetBattlerAbility(battler);
		
		if ((ability == ABILITY_WIMP_OUT || ability == ABILITY_EMERGENCY_EXIT) && HadMoreThanHalfHpNowDoesnt(battler) && !(gStatuses3[battler] & STATUS3_SKY_DROPPED)
		&& ((!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && GetBattlerSide(battler) == B_SIDE_OPPONENT) || CanBattlerSwitch(battler)))
			return TRUE;
	}
	return FALSE;
}

static inline bool8 CheckAndRecordAbility(u8 battlerId, u16 abilityId, u16 abilityToCheck)
{
	if (abilityId == abilityToCheck)
	{
		BattleAI_RecordAbility(battlerId);
		return TRUE;
	}
	return FALSE;
}

static bool8 IsImmuneToWeatherDamage(u8 battlerId, u16 ability)
{
	if ((IS_BATTLE_TYPE_GHOST_WITHOUT_SCOPE && GetBattlerSide(battlerId) == B_SIDE_OPPONENT) || (gStatuses3[battlerId] & (STATUS3_UNDERGROUND | STATUS3_UNDERWATER))
	|| !WEATHER_HAS_EFFECT || CheckAndRecordAbility(battlerId, ability, ABILITY_OVERCOAT) || CheckAndRecordAbility(battlerId, ability, ABILITY_MAGIC_GUARD))
	    return TRUE;
	return FALSE;
}

static inline void IncrementBattlerBasedEndTurnEffects(void)
{
	if (++gBattleStruct->turnEffectsBattlerId >= gBattlersCount)
	{
		gBattleStruct->turnEffectsBattlerId = 0;
		++gBattleStruct->turnEffectsTracker;
	}
}

static inline void IncrementSideBasedEndTurnEffects(void)
{
	if (++gBattleStruct->turnSideTracker >= B_SIDE_COUNT)
	{
		gBattleStruct->turnSideTracker = 0;
		++gBattleStruct->turnEffectsTracker;
	}
}

enum
{
	ENDTURN_ORDER,
	ENDTURN_GENERAL_COUNTERS,
	ENDTURN_WEATHER_ENDS,
	ENDTURN_WEATHER_DAMAGE, // Also weather heal abilities
	ENDTURN_EMERGENCY_EXIT,
	ENDTURN_FUTURE_SIGHT_WISH,
	ENDTURN_SEA_OF_FIRE_DAMAGE, // Also G-Max Cannonade, Vine Lash, Wildfire and Volcalith
	ENDTURN_GRASSY_TERRAIN_HEAL,
	ENDTURN_HEALER_SHED_SKIN,
	ENDTURN_BLACK_SLUDGE_LEFTOVERS,
	ENDTURN_EMERGENCY_EXIT2,
	ENDTURN_AQUA_RING_HEAL,
	ENDTURN_INGRAIN_HEAL,
	ENDTURN_LEECH_SEED_DRAIN, // The battler that get seeded
	ENDTURN_POISON_DAMAGE, // Also Poison Heal
	ENDTURN_BURN_DAMAGE,
	ENDTURN_NIGHTMARE_DAMAGE,
	ENDTURN_CURSE_DAMAGE,
	ENDTURN_BINDING_COUNTER,
	ENDTURN_OCTOLOCK_DROP,
	ENDTURN_TAUNT_ENDS, // From G-Max Meltdown
	ENDTURN_ENCORE_ENDS,
	ENDTURN_DISABLE_ENDS,
	ENDTURN_LOCK_ON_ENDS,
	ENDTURN_LASER_FOCUS_ENDS,
	ENDTURN_ENDURE_ENDS,
	ENDTURN_ION_DELUGE_ENDS,
	ENDTURN_MAGNET_RISE_ENDS,
	ENDTURN_TELEKINESIS_ENDS,
	ENDTURN_HEAL_BLOCK_ENDS,
	ENDTURN_EMBARGO_ENDS,
	ENDTURN_YAWN_ASLEEP,
	ENDTURN_PERISH_SONG_COUNTER,
	ENDTURN_ROOST_ENDS,
	ENDTURN_EMERGENCY_EXIT3,
	ENDTURN_REFLECT_ENDS,
	ENDTURN_LIGHT_SCREEN_ENDS,
	ENDTURN_SAFEGUARD_ENDS,
	ENDTURN_MIST_ENDS,
	ENDTURN_TAILWIND_ENDS,
	ENDTURN_LUCKY_CHANT_ENDS,
	ENDTURN_RAINBOW_ENDS,
	ENDTURN_SEA_OF_FIRE_ENDS,
	ENDTURN_SWAMP_ENDS,
	ENDTURN_GMAX_COUNTERS, // G-Max Vine Lash, Wildfire, Cannonade and Volcalith
	ENDTURN_AURORA_VEIL_ENDS,
	ENDTURN_TRICK_ROOM_ENDS,
	ENDTURN_GRAVITY_ENDS,
	ENDTURN_WATER_SPORT_ENDS,
	ENDTURN_MUD_SPORT_ENDS,
	ENDTURN_WONDER_ROOM_ENDS,
	ENDTURN_MAGIC_ROOM_ENDS,
	ENDTURN_TERRAIN_ENDS,
	ENDTURN_UPROAR_ENDS,
	ENDTURN_THRASH_CONFUSES,
	ENDTURN_BAD_DREAMS_HARVEST, // Also Ball Fetch, Moody, Pickup, Slow Start and Speed Boost
	ENDTURN_ORBS, // Also Sticky Barb and White Herb
	ENDTURN_EMERGENCY_EXIT4,
	ENDTURN_FORM_CHANGE, // Power Construct, Schooling, Shields Down, Zen Mode and Hunger Switch
	ENDTURN_EJECT_PACK,
	ENDTURN_COUNT,
};

bool8 DoEndTurnEffects(void)
{
	u8 side;
	u32 i, j;
	u16 ability;
	bool8 effect = FALSE;

    gHitMarker |= (HITMARKER_IGNORE_GRUDGE | HITMARKER_SKIP_DMG_TRACK);
	
	do
	{
		gBattlerAttacker = gBattlerTarget = gBattlerByTurnOrder[gBattleStruct->turnEffectsBattlerId];
		ability = GetBattlerAbility(gBattlerAttacker);
		side = gBattleStruct->turnSideTracker;
		
		switch (gBattleStruct->turnEffectsTracker)
		{
			case ENDTURN_ORDER:
			    for (i = 0; i < gBattlersCount; i++)
					gBattlerByTurnOrder[i] = i;
				
				for (i = 0; i < gBattlersCount - 1; i++)
				{
					for (j = i + 1; j < gBattlersCount; j++)
					{
						if (GetWhoStrikesFirst(gBattlerByTurnOrder[i], gBattlerByTurnOrder[j], TRUE) != ATTACKER_STRIKES_FIRST)
							SwapTurnOrder(i, j);
					}
				}
				++gBattleStruct->turnEffectsTracker;
				break;
			case ENDTURN_GENERAL_COUNTERS:
			    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
				{
					if (gDisableStructs[i].destinyBondCounter)
						--gDisableStructs[i].destinyBondCounter;
				}
				++gBattleStruct->turnEffectsTracker;
				break;
			case ENDTURN_WEATHER_ENDS:
			{
				u8 weatherEnumId = GetCurrentWeatherEnumId(gBattleWeather);
				
				if (weatherEnumId != ENUM_WEATHER_COUNT)
				{
					if (gBattleStruct->weatherDuration > 0 && --gBattleStruct->weatherDuration == 0)
					{
						gBattleWeather = B_WEATHER_NONE;
						gBattleCommunication[MULTISTRING_CHOOSER] = sBattleWeatherInfo[weatherEnumId].endMessage;
						gBattleCommunication[MULTIUSE_STATE] = TRUE;
					}
					else
					{
						gBattleScripting.animArg1 = sBattleWeatherInfo[weatherEnumId].animation;
						gBattleCommunication[MULTISTRING_CHOOSER] = sBattleWeatherInfo[weatherEnumId].continueMessage;
						gBattleCommunication[MULTIUSE_STATE] = FALSE;
					}
					BattleScriptExecute(BattleScript_WeatherContinuesOrEnds);
					effect = TRUE;
				}
				++gBattleStruct->turnEffectsTracker;
				break;
			}
			case ENDTURN_WEATHER_DAMAGE:
			    if (IsBattlerAlive(gBattlerAttacker))
				{
					switch (ability)
					{
						case ABILITY_RAIN_DISH:
						case ABILITY_DRY_SKIN:
						case ABILITY_ICE_BODY:
						case ABILITY_SOLAR_POWER:
						    if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gBattlerAttacker))
								effect = TRUE;
							break;
					}
					
					if (!effect && !IsImmuneToWeatherDamage(gBattlerAttacker, ability))
					{
						if (IsBattlerWeatherAffected(gBattlerAttacker, B_WEATHER_SANDSTORM))
						{
							if (!IsBattlerAnyType(gBattlerAttacker, TYPE_ROCK, TYPE_STEEL, TYPE_GROUND) && !CheckAndRecordAbility(gBattlerAttacker, ability, ABILITY_SAND_VEIL)
							&& !CheckAndRecordAbility(gBattlerAttacker, ability, ABILITY_SAND_RUSH) && !CheckAndRecordAbility(gBattlerAttacker, ability, ABILITY_SAND_FORCE))
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_DAMAGED_BY_SANDSTORM;
								effect = TRUE;
							}
						}
						else if (IsBattlerWeatherAffected(gBattlerAttacker, B_WEATHER_HAIL))
						{
							if (!IsBattlerOfType(gBattlerAttacker, TYPE_ICE) && !CheckAndRecordAbility(gBattlerAttacker, ability, ABILITY_SNOW_CLOAK)
							&& !CheckAndRecordAbility(gBattlerAttacker, ability, ABILITY_ICE_BODY))
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_DAMAGED_BY_HAIL;
								effect = TRUE;
							}
						}
						
						if (effect)
						{
							gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 16;
							if (gBattleMoveDamage == 0)
								gBattleMoveDamage = 1;
							
							BattleScriptExecute(BattleScript_WeatherDamage);
						}
					}
				}
				IncrementBattlerBasedEndTurnEffects();
				break;
			// These are checked each time it takes damage
			case ENDTURN_EMERGENCY_EXIT:
			case ENDTURN_EMERGENCY_EXIT2:
			case ENDTURN_EMERGENCY_EXIT3:
			case ENDTURN_EMERGENCY_EXIT4:
			    if (TryActivateEmergencyExit(gBattlerTarget))
				{
					BattleScriptExecute(BattleScript_EmergencyExitEnd2);
					effect = TRUE;
				}
				IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_FUTURE_SIGHT_WISH: // Activate in order were are set
			    if (IsBattlerAlive(gBattlerAttacker) && TryDoQueuedBattleEffectsInBattlerList(gBattlerAttacker, gWishFutureSightQueuedEffectIds, QueuedEffects_DoWishFutureSight))
					effect = TRUE;
			    IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_SEA_OF_FIRE_DAMAGE: // Activate in order were are set
				if (IsBattlerAlive(gBattlerAttacker) && TryDoQueuedBattleEffectsInSideList(gBattlerAttacker, gSeaOfFireAndGMaxQueuedEffectIds, QueuedEffects_DoSeaOfFireAndGMaxEffects))
					effect = TRUE;
				IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_GRASSY_TERRAIN_HEAL:
			    IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_HEALER_SHED_SKIN:
			    if (IsBattlerAlive(gBattlerTarget))
				{
					switch (ability)
					{
						case ABILITY_HEALER:
						case ABILITY_SHED_SKIN:
						case ABILITY_HYDRATION:
						    if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gBattlerTarget))
								effect = TRUE;
							break;
					}
				}
				IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_BLACK_SLUDGE_LEFTOVERS:
			    IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_AQUA_RING_HEAL:
			    if (IsBattlerAlive(gBattlerAttacker) && (gBattleMons[gBattlerAttacker].status2 & STATUS2_AQUA_RING) && !(gStatuses3[gBattlerAttacker] & STATUS3_HEAL_BLOCK)
				&& !BATTLER_MAX_HP(gBattlerAttacker))
				{
					gBattleScripting.animArg1 = B_ANIM_AQUA_RING_HEAL;
					gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_BUFF1_RESTORED_HEALTH;
					PrepareMoveBuffer(gBattleTextBuff1, MOVE_AQUA_RING);
					goto DO_1_16_HP_HEAL;
				}
				IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_INGRAIN_HEAL:
			    if (IsBattlerAlive(gBattlerAttacker) && (gStatuses3[gBattlerAttacker] & STATUS3_ROOTED) && !(gStatuses3[gBattlerAttacker] & STATUS3_HEAL_BLOCK)
				&& !BATTLER_MAX_HP(gBattlerAttacker))
                {
					gBattleScripting.animArg1 = B_ANIM_INGRAIN_HEAL;
					gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_ABSORBED_NUTRIENTS;
					
					DO_1_16_HP_HEAL:
                    gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 16;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = 1;
					// Sing change handled in battle script
                    BattleScriptExecute(BattleScript_EndTurnHeal);
                    effect = TRUE;
                }
			    IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_LEECH_SEED_DRAIN:
			    if (IsBattlerAlive(gBattlerTarget) && (gStatuses3[gBattlerTarget] & STATUS3_LEECHSEED)
				&& IsBattlerAlive(gDisableStructs[gBattlerTarget].leechSeedBattler) && !CheckAndRecordAbility(gBattlerTarget, ability, ABILITY_MAGIC_GUARD))
                {
					gBattlerAttacker = gDisableStructs[gBattlerTarget].leechSeedBattler; // HP receiver
					gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP / 8;
					if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = 1;
                    gBattleScripting.animArg1 = gBattlerAttacker;
                    gBattleScripting.animArg2 = gBattlerTarget;
                    BattleScriptExecute(BattleScript_LeechSeedTurnDrain);
                    effect = TRUE;
                }
			    IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_POISON_DAMAGE:
			    if (IsBattlerAlive(gBattlerAttacker))
				{
					switch (ability)
					{
						case ABILITY_POISON_HEAL:
						    if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gBattlerAttacker))
								effect = TRUE;
							break;
					}
					
					if (gBattleMons[gBattlerAttacker].status1.id == STATUS1_POISON)
					{
						if (!CheckAndRecordAbility(gBattlerAttacker, ability, ABILITY_POISON_HEAL) && !CheckAndRecordAbility(gBattlerAttacker, ability, ABILITY_MAGIC_GUARD))
						{
							gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 8;
							if (gBattleMoveDamage == 0)
								gBattleMoveDamage = 1;
							BattleScriptExecute(BattleScript_PoisonTurnDmg);
							effect = TRUE;
						}
					}
					else if (gBattleMons[gBattlerAttacker].status1.id == STATUS1_TOXIC_POISON)
					{
						if (gBattleMons[gBattlerAttacker].status1.counter != 15)
							gBattleMons[gBattlerAttacker].status1.counter++;
						
						// These prevent damage, but not the counter increase
						if (!CheckAndRecordAbility(gBattlerAttacker, ability, ABILITY_POISON_HEAL) && !CheckAndRecordAbility(gBattlerAttacker, ability, ABILITY_MAGIC_GUARD))
						{
							gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 16;
							if (gBattleMoveDamage == 0)
								gBattleMoveDamage = 1;
							gBattleMoveDamage *= gBattleMons[gBattlerAttacker].status1.counter;
							BattleScriptExecute(BattleScript_PoisonTurnDmg);
							effect = TRUE;
						}
					}
				}
			    IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_BURN_DAMAGE:
			    if (IsBattlerAlive(gBattlerAttacker) && gBattleMons[gBattlerAttacker].status1.id == STATUS1_BURN && !CheckAndRecordAbility(gBattlerAttacker, ability, ABILITY_MAGIC_GUARD))
				{
#if BURN_UPDATE
					gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 16;
#else
					gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 8;
#endif

					if (CheckAndRecordAbility(gBattlerAttacker, ability, ABILITY_HEATPROOF))
						gBattleMoveDamage /= 2;
					
					if (gBattleMoveDamage == 0)
						gBattleMoveDamage = 1;
					
					BattleScriptExecute(BattleScript_BurnTurnDmg);
					effect = TRUE;
				}
			    IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_NIGHTMARE_DAMAGE:
			    if (IsBattlerAlive(gBattlerAttacker) && (gBattleMons[gBattlerAttacker].status2 & STATUS2_NIGHTMARE)
				&& (gBattleMons[gBattlerAttacker].status1.id == STATUS1_SLEEP || CheckAndRecordAbility(gBattlerAttacker, ability, ABILITY_COMATOSE))
				&& !CheckAndRecordAbility(gBattlerAttacker, ability, ABILITY_MAGIC_GUARD))
				{
					gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 4;
					if (gBattleMoveDamage == 0)
						gBattleMoveDamage = 1;
					
					BattleScriptExecute(BattleScript_NightmareTurnDmg);
					effect = TRUE;
				}
			    IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_CURSE_DAMAGE:
			    if (IsBattlerAlive(gBattlerAttacker) && (gBattleMons[gBattlerAttacker].status2 & STATUS2_CURSED) && !CheckAndRecordAbility(gBattlerAttacker, ability, ABILITY_MAGIC_GUARD))
                {
                    gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 4;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = 1;
					
                    BattleScriptExecute(BattleScript_CurseTurnDmg);
                    effect = TRUE;
                }
			    IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_BINDING_COUNTER:
			    if (IsBattlerAlive(gBattlerAttacker) && gDisableStructs[gBattlerAttacker].wrapTurns && !CheckAndRecordAbility(gBattlerAttacker, ability, ABILITY_MAGIC_GUARD))
                {
					gDisableStructs[gBattlerAttacker].wrapTurns--;
					PrepareMoveBuffer(gBattleTextBuff1, gDisableStructs[gBattlerAttacker].wrappedMove);
					
                    if (gDisableStructs[gBattlerAttacker].wrapTurns) // Damaged by wrap
                    {
                        gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 16;
                        if (gBattleMoveDamage == 0)
                            gBattleMoveDamage = 1;
						
						gBattlescriptCurrInstr = BattleScript_WrapTurnDmg;
                    }
                    else  // Broke free
                        gBattlescriptCurrInstr = BattleScript_WrapEnds;
						
                    BattleScriptExecute(gBattlescriptCurrInstr);
                    effect = TRUE;
                }
			    IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_OCTOLOCK_DROP:
			    IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_TAUNT_ENDS:
			    if (gDisableStructs[gBattlerAttacker].tauntTimer)
					--gDisableStructs[gBattlerAttacker].tauntTimer;
				
				IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_ENCORE_ENDS:
			    if (gDisableStructs[gBattlerAttacker].encoreTimer > 0)
                {
					// Does not have the encored move anymore
                    if (gBattleMons[gBattlerAttacker].moves[gDisableStructs[gBattlerAttacker].encoredMovePos] != gDisableStructs[gBattlerAttacker].encoredMove)
                    {
                        gDisableStructs[gBattlerAttacker].encoredMove = MOVE_NONE;
                        gDisableStructs[gBattlerAttacker].encoreTimer = 0;
                    }
                    else if (--gDisableStructs[gBattlerAttacker].encoreTimer == 0 || gBattleMons[gBattlerAttacker].pp[gDisableStructs[gBattlerAttacker].encoredMovePos] == 0)
                    {
                        gDisableStructs[gBattlerAttacker].encoredMove = MOVE_NONE;
                        gDisableStructs[gBattlerAttacker].encoreTimer = 0;
                        BattleScriptExecute(BattleScript_EncoredNoMore);
                        effect = TRUE;
                    }
                }
				IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_DISABLE_ENDS:
				if (gDisableStructs[gBattlerAttacker].disableTimer > 0)
                {
					// Does not have the disabled move anymore
                    if (FindMoveSlotInBattlerMoveset(gBattlerAttacker, gDisableStructs[gBattlerAttacker].disabledMove) == MAX_MON_MOVES)
                    {
                        gDisableStructs[gBattlerAttacker].disabledMove = MOVE_NONE;
                        gDisableStructs[gBattlerAttacker].disableTimer = 0;
                    }
                    else if (--gDisableStructs[gBattlerAttacker].disableTimer == 0) // Disable ends
                    {
                        gDisableStructs[gBattlerAttacker].disabledMove = MOVE_NONE;
                        BattleScriptExecute(BattleScript_DisabledNoMore);
                        effect = TRUE;
                    }
                }
				IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_LOCK_ON_ENDS:
			    if (gStatuses3[gBattlerAttacker] & STATUS3_ALWAYS_HITS)
                    gStatuses3[gBattlerAttacker] -= STATUS3_ALWAYS_HITS_TURN(1);
				
				IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_LASER_FOCUS_ENDS:
			    IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_ENDURE_ENDS:
			    gProtectStructs[gBattlerAttacker].endured = FALSE;
				IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_ION_DELUGE_ENDS:
			    ++gBattleStruct->turnEffectsTracker;
				break;
			case ENDTURN_MAGNET_RISE_ENDS:
			    if (IsBattlerAlive(gBattlerAttacker) && (gStatuses3[gBattlerAttacker] & STATUS3_MAGNET_RISE) && gDisableStructs[gBattlerAttacker].magnetRiseTimer > 0
				&& --gDisableStructs[gBattlerAttacker].magnetRiseTimer == 0)
				{
					gStatuses3[gBattlerAttacker] &= ~(STATUS3_MAGNET_RISE);
					BattleScriptExecute(BattleScript_MagnetRiseEnds);
					effect = TRUE;
				}
				IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_TELEKINESIS_ENDS:
			    IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_HEAL_BLOCK_ENDS:
			    if (IsBattlerAlive(gBattlerAttacker) && (gStatuses3[gBattlerAttacker] & STATUS3_HEAL_BLOCK) && gDisableStructs[gBattlerAttacker].healBlockTimer > 0
				&& --gDisableStructs[gBattlerAttacker].healBlockTimer == 0)
				{
					gStatuses3[gBattlerAttacker] &= ~(STATUS3_HEAL_BLOCK);
					BattleScriptExecute(BattleScript_HealBlockEnds);
					effect = TRUE;
				}
			    IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_EMBARGO_ENDS:
			    if (IsBattlerAlive(gBattlerAttacker) && (gStatuses3[gBattlerAttacker] & STATUS3_EMBARGO) && gDisableStructs[gBattlerAttacker].embargoTimer > 0
				&& --gDisableStructs[gBattlerAttacker].embargoTimer == 0)
				{
					gStatuses3[gBattlerAttacker] &= ~(STATUS3_EMBARGO);
					BattleScriptExecute(BattleScript_EmbargoEnds);
					effect = TRUE;
				}
			    IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_YAWN_ASLEEP:
			    if (IsBattlerAlive(gBattlerAttacker) && (gStatuses3[gBattlerAttacker] & STATUS3_YAWN))
                {
                    gStatuses3[gBattlerAttacker] -= STATUS3_YAWN_TURN(1);
					
					SetMoveEffect(MOVE_EFFECT_SLEEP, TRUE, TRUE);
					
                    if (!(gStatuses3[gBattlerAttacker] & STATUS3_YAWN) && DoMoveEffect(TRUE, NULL, 0))
					{
						BattleScriptExecute(BattleScript_YawnMakesAsleep);
						effect = TRUE;
					}
				}
				IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_PERISH_SONG_COUNTER:
			    if (IsBattlerAlive(gBattlerAttacker) && (gStatuses3[gBattlerAttacker] & STATUS3_PERISH_SONG))
				{
					PrepareByteNumberBuffer(gBattleTextBuff1, 1, gDisableStructs[gBattlerAttacker].perishSongTimer);
					
					if (gDisableStructs[gBattlerAttacker].perishSongTimer == 0)
					{
						gStatuses3[gBattlerAttacker] &= ~(STATUS3_PERISH_SONG);
						gBattleMoveDamage = gBattleMons[gBattlerAttacker].hp;
						gBattlescriptCurrInstr = BattleScript_PerishSongTakesLife;
					}
					else
					{
						--gDisableStructs[gBattlerAttacker].perishSongTimer;
						gBattlescriptCurrInstr = BattleScript_PerishSongCountGoesDown;
					}
					BattleScriptExecute(gBattlescriptCurrInstr);
					effect = TRUE;
				}
				IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_ROOST_ENDS:
			    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
					gDisableStructs[i].roostActive = FALSE;
				
			    ++gBattleStruct->turnEffectsTracker;
				break;
			case ENDTURN_REFLECT_ENDS:
                if ((gSideStatuses[side] & SIDE_STATUS_REFLECT) && gSideTimers[side].reflectTimer > 0 && --gSideTimers[side].reflectTimer == 0)
                {
					gSideStatuses[side] &= ~(SIDE_STATUS_REFLECT);
					gBattlerAttacker = gSideTimers[side].reflectBattlerId;
					PrepareMoveBuffer(gBattleTextBuff1, MOVE_REFLECT);
					gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SIDE_STATUS_WORE_OFF;
					BattleScriptExecute(BattleScript_SideStatusWoreOff);
					effect = TRUE;
                }
			    IncrementSideBasedEndTurnEffects();
				break;
			case ENDTURN_LIGHT_SCREEN_ENDS:
                if ((gSideStatuses[side] & SIDE_STATUS_LIGHTSCREEN) && gSideTimers[side].lightscreenTimer > 0 && --gSideTimers[side].lightscreenTimer == 0)
                {
					gSideStatuses[side] &= ~(SIDE_STATUS_LIGHTSCREEN);
					gBattlerAttacker = gSideTimers[side].lightscreenBattlerId;
					PrepareMoveBuffer(gBattleTextBuff1, MOVE_LIGHT_SCREEN);
					gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SIDE_STATUS_WORE_OFF;
					BattleScriptExecute(BattleScript_SideStatusWoreOff);
					effect = TRUE;
                }
				IncrementSideBasedEndTurnEffects();
				break;
			case ENDTURN_SAFEGUARD_ENDS:
                if ((gSideStatuses[side] & SIDE_STATUS_SAFEGUARD) && gSideTimers[side].safeguardTimer > 0 && --gSideTimers[side].safeguardTimer == 0)
                {
					gSideStatuses[side] &= ~(SIDE_STATUS_SAFEGUARD);
					gBattlerAttacker = gSideTimers[side].safeguardBattlerId;
					gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SAFEGUARD_WORE_OFF;
					BattleScriptExecute(BattleScript_SideStatusWoreOff);
					effect = TRUE;
                }
				IncrementSideBasedEndTurnEffects();
				break;
			case ENDTURN_MIST_ENDS:
                if ((gSideStatuses[side] & SIDE_STATUS_MIST) && gSideTimers[side].mistTimer > 0 && --gSideTimers[side].mistTimer == 0)
                {
                    gSideStatuses[side] &= ~(SIDE_STATUS_MIST);
					gBattlerAttacker = gSideTimers[side].mistBattlerId;
					PrepareMoveBuffer(gBattleTextBuff1, MOVE_MIST);
                    gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SIDE_STATUS_WORE_OFF;
					BattleScriptExecute(BattleScript_SideStatusWoreOff);
					effect = TRUE;
                }
				IncrementSideBasedEndTurnEffects();
				break;
			case ENDTURN_TAILWIND_ENDS:
			    if ((gSideStatuses[side] & SIDE_STATUS_TAILWIND) && gSideTimers[side].tailwindTimer > 0 && --gSideTimers[side].tailwindTimer == 0)
				{
					gSideStatuses[side] &= ~(SIDE_STATUS_TAILWIND);
					gBattlerAttacker = gSideTimers[side].tailwindBattlerId;
					gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_TAILWIND_PETERED_OUT;
					BattleScriptExecute(BattleScript_SideStatusWoreOff);
					effect = TRUE;
				}
			    IncrementSideBasedEndTurnEffects();
				break;
			case ENDTURN_LUCKY_CHANT_ENDS:
			    if ((gSideStatuses[side] & SIDE_STATUS_LUCKY_CHANT) && gSideTimers[side].luckyChantTimer > 0 && --gSideTimers[side].luckyChantTimer == 0)
				{
					gSideStatuses[side] &= ~(SIDE_STATUS_LUCKY_CHANT);
					gBattlerAttacker = gSideTimers[side].luckyChantBattlerId;
					PrepareMoveBuffer(gBattleTextBuff1, MOVE_LUCKY_CHANT);
					gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SIDE_STATUS_WORE_OFF;
					BattleScriptExecute(BattleScript_SideStatusWoreOff);
					effect = TRUE;
				}
			    IncrementSideBasedEndTurnEffects();
				break;
			case ENDTURN_RAINBOW_ENDS:
			    IncrementSideBasedEndTurnEffects();
				break;
			case ENDTURN_SEA_OF_FIRE_ENDS:
			    IncrementSideBasedEndTurnEffects();
				break;
			case ENDTURN_SWAMP_ENDS:
			    IncrementSideBasedEndTurnEffects();
				break;
			case ENDTURN_GMAX_COUNTERS:
			    IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_AURORA_VEIL_ENDS:
			    IncrementSideBasedEndTurnEffects();
				break;
			case ENDTURN_TRICK_ROOM_ENDS:
			    if ((gFieldStatus & STATUS_FIELD_TRICK_ROOM) && gFieldTimers.trickRoomTimer > 0 && --gFieldTimers.trickRoomTimer == 0)
				{
					gFieldStatus &= ~(STATUS_FIELD_TRICK_ROOM);
					gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_TWISTED_DIMENSIONS_NORMALISED;
					BattleScriptExecute(BattleScript_FieldStatusWoreOff);
					effect = TRUE;
				}
				++gBattleStruct->turnEffectsTracker;
				break;
			case ENDTURN_GRAVITY_ENDS:
			    if ((gFieldStatus & STATUS_FIELD_GRAVITY) && gFieldTimers.gravityTimer > 0 && --gFieldTimers.gravityTimer == 0)
				{
					gFieldStatus &= ~(STATUS_FIELD_GRAVITY);
					gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_GRAVITY_NORMALIZED;
					BattleScriptExecute(BattleScript_FieldStatusWoreOff);
					effect = TRUE;
				}
			    ++gBattleStruct->turnEffectsTracker;
				break;
			case ENDTURN_WATER_SPORT_ENDS:
			    if ((gFieldStatus & STATUS_FIELD_WATERSPORT) && gFieldTimers.waterSportTimer > 0 && --gFieldTimers.waterSportTimer == 0)
				{
					gFieldStatus &= ~(STATUS_FIELD_WATERSPORT);
					PrepareMoveBuffer(gBattleTextBuff1, MOVE_WATER_SPORT);
					gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_FIELD_EFFECTS_WORE_OFF;
					BattleScriptExecute(BattleScript_FieldStatusWoreOff);
					effect = TRUE;
				}
			    ++gBattleStruct->turnEffectsTracker;
				break;
			case ENDTURN_MUD_SPORT_ENDS:
			    if ((gFieldStatus & STATUS_FIELD_MUDSPORT) && gFieldTimers.mudSportTimer > 0 && --gFieldTimers.mudSportTimer == 0)
				{
					gFieldStatus &= ~(STATUS_FIELD_MUDSPORT);
					PrepareMoveBuffer(gBattleTextBuff1, MOVE_MUD_SPORT);
					gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_FIELD_EFFECTS_WORE_OFF;
					BattleScriptExecute(BattleScript_FieldStatusWoreOff);
					effect = TRUE;
				}
			    ++gBattleStruct->turnEffectsTracker;
				break;
			case ENDTURN_WONDER_ROOM_ENDS:
			    ++gBattleStruct->turnEffectsTracker;
				break;
			case ENDTURN_MAGIC_ROOM_ENDS:
			    ++gBattleStruct->turnEffectsTracker;
				break;
			case ENDTURN_TERRAIN_ENDS:
			    ++gBattleStruct->turnEffectsTracker;
				break;
			case ENDTURN_UPROAR_ENDS:
			    if (IsBattlerAlive(gBattlerAttacker) && (gBattleMons[gBattlerAttacker].status2 & STATUS2_UPROAR))
                {
					gBattleMons[gBattlerAttacker].status2 -= STATUS2_UPROAR_TURN(1);
					
					if (WasUnableToUseMove(gBattlerAttacker))
					{
						CancelMultiTurnMoves(gBattlerAttacker);
						gBattleCommunication[MULTISTRING_CHOOSER] = 1;
					}
					else if (gBattleMons[gBattlerAttacker].status2 & STATUS2_UPROAR)
					{
						gBattleCommunication[MULTISTRING_CHOOSER] = 0;
						gBattleMons[gBattlerAttacker].status2 |= STATUS2_MULTIPLETURNS;
					}
					else
					{
						CancelMultiTurnMoves(gBattlerAttacker);
						gBattleCommunication[MULTISTRING_CHOOSER] = 1;
					}
					BattleScriptExecute(BattleScript_PrintUproarOverTurns);
					effect = TRUE;
                }
				IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_THRASH_CONFUSES:
			    if (IsBattlerAlive(gBattlerAttacker) && (gBattleMons[gBattlerAttacker].status2 & STATUS2_LOCK_CONFUSE))
				{
					gBattleMons[gBattlerAttacker].status2 -= STATUS2_LOCK_CONFUSE_TURN(1);
					
                    if (WasUnableToUseMove(gBattlerAttacker))
                        CancelMultiTurnMoves(gBattlerAttacker);
                    else if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_LOCK_CONFUSE) && (gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS))
                    {
                        gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_MULTIPLETURNS);
						
						SetMoveEffect(MOVE_EFFECT_CONFUSION, TRUE, FALSE);
						
                        if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_CONFUSION) && DoMoveEffect(TRUE, NULL, 0))
                        {
							BattleScriptExecute(BattleScript_ThrashConfuses);
							effect = TRUE;
                        }
                    }
				}
				IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_BAD_DREAMS_HARVEST:
			    if (IsBattlerAlive(gBattlerAttacker))
				{
					switch (ability)
					{
						case ABILITY_TRUANT:
						case ABILITY_BAD_DREAMS:
						case ABILITY_HARVEST:
						case ABILITY_SPEED_BOOST:
						case ABILITY_MOODY:
						case ABILITY_SLOW_START:
						case ABILITY_PICKUP:
						case ABILITY_BALL_FETCH:
						    if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gBattlerAttacker))
								effect = TRUE;
							break;
					}
				}
				IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_ORBS:
			    IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_FORM_CHANGE:
			    if (IsBattlerAlive(gBattlerAttacker))
				{
					switch (ability)
					{
						case ABILITY_POWER_CONSTRUCT:
						case ABILITY_ZEN_MODE:
						case ABILITY_SCHOOLING:
						case ABILITY_SHIELDS_DOWN:
						case ABILITY_HUNGER_SWITCH:
						    if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gBattlerAttacker))
								effect = TRUE;
							break;
					}
				}
				IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_EJECT_PACK:
			    IncrementBattlerBasedEndTurnEffects();
				break;
			case ENDTURN_COUNT:
			    ResetAllQueuedEffectsDone();
			    effect = TRUE;
				break;
		}
	} while (!effect);
	
	if (gBattleMainFunc != BattleTurnPassed)
		return TRUE;
	else
	{
		gHitMarker &= ~(HITMARKER_IGNORE_GRUDGE | HITMARKER_SKIP_DMG_TRACK);
		return FALSE;
	}
}

#define FAINTED_ACTIONS_MAX_CASE 8

bool8 HandleFaintedMonActions(void)
{
    if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
        return FALSE;
    do
    {
        s32 i;
        switch (gBattleStruct->faintedActionsState)
        {
        case 0:
            gBattleStruct->faintedActionsBattlerId = 0;
            ++gBattleStruct->faintedActionsState;
			
            for (i = 0; i < gBattlersCount; ++i)
            {
				if ((gBattleTypeFlags & BATTLE_TYPE_TWO_VS_ONE) && GetBattlerPosition(i) == B_POSITION_PLAYER_RIGHT)
					continue;
				
                if ((gAbsentBattlerFlags & gBitTable[i]) && !HasNoMonsToSwitch(i, PARTY_SIZE, PARTY_SIZE))
                    gAbsentBattlerFlags &= ~(gBitTable[i]);
            }
            // fall through
        case 1:
            do
            {
                gBattlerFainted = gBattlerTarget = gBattleStruct->faintedActionsBattlerId;
                if (gBattleMons[gBattleStruct->faintedActionsBattlerId].hp == 0
                 && !(gBattleStruct->givenExpMons & gBitTable[gBattlerPartyIndexes[gBattleStruct->faintedActionsBattlerId]])
                 && !(gAbsentBattlerFlags & gBitTable[gBattleStruct->faintedActionsBattlerId]))
                {
                    BattleScriptExecute(BattleScript_GiveExp);
                    gBattleStruct->faintedActionsState = 2;
                    return TRUE;
                }
            } while (++gBattleStruct->faintedActionsBattlerId != gBattlersCount);
            gBattleStruct->faintedActionsState = 3;
            break;
        case 2:
            OpponentSwitchInResetSentPokesToOpponentValue(gBattlerFainted);
            if (++gBattleStruct->faintedActionsBattlerId == gBattlersCount)
                gBattleStruct->faintedActionsState = 3;
            else
                gBattleStruct->faintedActionsState = 1;
			
			// Don't switch mons until all pokemon performed their actions or the battle's over.
            if (gBattleOutcome == 0 && !NoAliveMonsForEitherParty() && gCurrentTurnActionNumber != gBattlersCount)
            {
                gAbsentBattlerFlags |= gBitTable[gBattlerFainted];
                return FALSE;
            }
            break;
        case 3:
		    // Don't switch mons until all pokemon performed their actions or the battle's over.
            if (gBattleOutcome == 0 && !NoAliveMonsForEitherParty() && gCurrentTurnActionNumber != gBattlersCount)
                return FALSE;
            gBattleStruct->faintedActionsBattlerId = 0;
            ++gBattleStruct->faintedActionsState;
            // fall through
        case 4:
            do
            {
                gBattlerFainted = gBattlerTarget = gBattleStruct->faintedActionsBattlerId;
                if (gBattleMons[gBattleStruct->faintedActionsBattlerId].hp == 0
                 && !(gAbsentBattlerFlags & gBitTable[gBattleStruct->faintedActionsBattlerId]))
                {
                    BattleScriptExecute(BattleScript_HandleFaintedMon);
                    gBattleStruct->faintedActionsState = 5;
                    return TRUE;
                }
            } while (++gBattleStruct->faintedActionsBattlerId != gBattlersCount);
            gBattleStruct->faintedActionsState = 6;
            break;
        case 5:
            if (++gBattleStruct->faintedActionsBattlerId == gBattlersCount)
                gBattleStruct->faintedActionsState = 6;
            else
                gBattleStruct->faintedActionsState = 4;
            break;
        case 6: // All battlers switch-in abilities happen here to prevent them happening against an empty field.
		    for (i = 0; i < gBattlersCount; i++)
            {
                if (gBattleStruct->switchInAbilityPostponed & gBitTable[i])
                {
                    if (DoSwitchInAbilitiesItems(i))
                        return TRUE;
                    gBattleStruct->switchInAbilityPostponed &= ~(gBitTable[i]);
                }
            }
            gBattleStruct->faintedActionsState++;
            break;
		case 7:
		    if (ItemBattleEffects(1, 0, TRUE))
                return TRUE;
            ++gBattleStruct->faintedActionsState;
            break;
        case FAINTED_ACTIONS_MAX_CASE:
            break;
        }
    } while (gBattleStruct->faintedActionsState != FAINTED_ACTIONS_MAX_CASE);
    return FALSE;
}

void TryClearRageStatuses(void)
{
    s32 i;

    for (i = 0; i < gBattlersCount; ++i)
	{
        if ((gBattleMons[i].status2 & STATUS2_RAGE) && gBattleMoves[gBattleStruct->battlers[i].chosenMove].effect != EFFECT_RAGE)
            gBattleMons[i].status2 &= ~(STATUS2_RAGE);
	}
}

static void SetRandomMultiHitCounter(void)
{
	gMultiHitCounter = RandomMax(100);
	
	if (gMultiHitCounter < 35)
		gMultiHitCounter = 2;
	else if (gMultiHitCounter < 35 + 35)
		gMultiHitCounter = 3;
	else if (gMultiHitCounter < 35 + 35 + 15)
		gMultiHitCounter = 4;
	else
		gMultiHitCounter = 5;
}

u8 GetNumBeatUpHits(u8 battler)
{
	u8 i, numHits;
	struct Pokemon *party = GetBattlerParty(battler);
	
	for (i = numHits = 0; i < PARTY_SIZE; i++)
	{
		if (MonCanBattle(&party[i]) && !GetMonData(&party[i], MON_DATA_STATUS_ID))
			++numHits;
	}
	return numHits;
}

u8 AtkCanceller_UnableToUseMove(void)
{
    u8 effect = 0;

    do
    {
        switch (gBattleStruct->atkCancellerTracker)
        {
			case CANCELLER_FLAGS: // flags clear
			    gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_DESTINY_BOND);
				gStatuses3[gBattlerAttacker] &= ~(STATUS3_GRUDGE);
				gBattleStruct->battlers[gBattlerAttacker].targetsDone = 0;
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_ASLEEP: // check being asleep
			    if (gBattleMons[gBattlerAttacker].status1.id == STATUS1_SLEEP)
				{
					if (UproarWakeUpCheck())
					{
						gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_UPROAR_WOKEUP;
						effect = 1; // Wake up
					}
					else
					{
						if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE) // Prevent from wake up
							effect = 2;
						else
						{
							u8 toSub = GetBattlerAbility(gBattlerAttacker) == ABILITY_EARLY_BIRD ? 2 : 1;
							
							if (gBattleMons[gBattlerAttacker].status1.counter <= toSub)
						    {
						    	gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_WOKEUP;
						    	effect = 1; // Wake up
						    }
						    else
						    {
						    	gBattleMons[gBattlerAttacker].status1.counter -= toSub;
								effect = 2; // Asleep
						    }
						}
					}
					
					switch (effect)
					{
						case 1: // Wake up
						    ClearBattlerStatus(gBattlerAttacker);
							BattleScriptCall(BattleScript_MoveUsedWokeUp);
							break;
						case 2: // Asleep
						    if (gBattleMoves[gCurrentMove].effect != EFFECT_SNORE && gBattleMoves[gCurrentMove].effect != EFFECT_SLEEP_TALK)
							{
								gBattlescriptCurrInstr = BattleScript_MoveUsedIsAsleep;
								gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
							}
						    break;
					}
                }
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_FROZEN: // check being frozen
			    // unfreezing via a move thawing happens in CANCELLER_THAW
			    if (gBattleMons[gBattlerAttacker].status1.id == STATUS1_FREEZE && !gBattleMoves[gCurrentMove].flags.thawUser)
				{
					if (RandomMax(5))
					{
						gBattlescriptCurrInstr = BattleScript_MoveUsedIsFrozen;
						gHitMarker |= HITMARKER_NO_ATTACKSTRING;
					}
					else // unfreeze
					{
						ClearBattlerStatus(gBattlerAttacker);
						PrepareMonNickWithPrefixBuffer(gBattleTextBuff1, gBattlerAttacker, gBattlerPartyIndexes[gBattlerAttacker]);
						gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_DEFROST;
						BattleScriptCall(BattleScript_MoveUsedUnfroze);
					}
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_TRUANT: // truant
			    if (GetBattlerAbility(gBattlerAttacker) == ABILITY_TRUANT && gDisableStructs[gBattlerAttacker].truantCounter)
				{
					gMoveResultFlags |= MOVE_RESULT_MISSED;
					gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_LOAFING_AROUND;
					gBattlescriptCurrInstr = BattleScript_MoveUsedLoafingAround;
					CancelMultiTurnMoves(gBattlerAttacker);
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_RECHARGE: // recharge
			    if (gBattleMons[gBattlerAttacker].status2 & STATUS2_RECHARGE)
				{
					gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_RECHARGE);
					gDisableStructs[gBattlerAttacker].rechargeTimer = 0;
					gBattlescriptCurrInstr = BattleScript_MoveUsedMustRecharge;
					CancelMultiTurnMoves(gBattlerAttacker);
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_FLINCH: // flinch
			    if (gBattleMons[gBattlerAttacker].status2 & STATUS2_FLINCHED)
				{
					gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_FLINCHED);
					gProtectStructs[gBattlerAttacker].flinchImmobility = TRUE;
					gBattlescriptCurrInstr = BattleScript_MoveUsedFlinched;
					CancelMultiTurnMoves(gBattlerAttacker);
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_GRAVITY: // check gravity banned move
			case CANCELLER_GRAVITY_2:
			    if ((gFieldStatus & STATUS_FIELD_GRAVITY) && gBattleMoves[gCurrentMove].flags.gravityBanned)
				{
					gProtectStructs[gBattlerAttacker].usedGravityBannedMove = TRUE;
					gBattlescriptCurrInstr = BattleScript_MoveUsedGravityPrevented;
					CancelMultiTurnMoves(gBattlerAttacker);
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_DISABLED: // disabled move
			    if (gDisableStructs[gBattlerAttacker].disabledMove && gDisableStructs[gBattlerAttacker].disabledMove == gCurrentMove)
				{
					gProtectStructs[gBattlerAttacker].usedDisabledMove = TRUE;
					gBattlescriptCurrInstr = BattleScript_MoveUsedIsDisabled;
					CancelMultiTurnMoves(gBattlerAttacker);
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_HEAL_BLOCKED:
			case CANCELLER_HEAL_BLOCKED_2:
			    if ((gStatuses3[gBattlerAttacker] & STATUS3_HEAL_BLOCK) && GET_MOVE_MOVEEFFECT_TABLE(gCurrentMove).healingEffect)
				{
					gProtectStructs[gBattlerAttacker].usedHealBlockedMove = TRUE;
					gBattlescriptCurrInstr = BattleScript_MoveUsedHealBlockPrevented;
					CancelMultiTurnMoves(gBattlerAttacker);
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_TAUNTED: // taunt
			    if (gDisableStructs[gBattlerAttacker].tauntTimer && IS_MOVE_STATUS(gCurrentMove))
				{
					gProtectStructs[gBattlerAttacker].usedTauntedMove = TRUE;
					gBattlescriptCurrInstr = BattleScript_MoveUsedIsTaunted;
					CancelMultiTurnMoves(gBattlerAttacker);
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_IMPRISONED: // imprisoned
			    if (GetImprisonedMovesCount(gBattlerAttacker, gCurrentMove))
				{
					gProtectStructs[gBattlerAttacker].usedImprisonedMove = TRUE;
					gBattlescriptCurrInstr = BattleScript_MoveUsedIsImprisoned;
					CancelMultiTurnMoves(gBattlerAttacker);
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_CONFUSED: // confusion
			    if (gBattleMons[gBattlerAttacker].status2 & STATUS2_CONFUSION)
				{
					gBattleMons[gBattlerAttacker].status2 -= STATUS2_CONFUSION_TURN(1);
					
					if (gBattleMons[gBattlerAttacker].status2 & STATUS2_CONFUSION)
					{
#if CONFUSION_UPDATE
                        if (!RandomMax(3))
#else
		                if (RandomMax(2))
#endif
							BattleScriptCall(BattleScript_MoveUsedIsConfused);
						else // confusion dmg
						{
							gBattleMoveDamage = CalculateConfusionDamage();
							CancelMultiTurnMoves(gBattlerAttacker);
							gProtectStructs[gBattlerAttacker].confusionSelfDmg = TRUE;
							gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
							gBattlescriptCurrInstr = BattleScript_MoveUsedIsConfusedSefHit;
						}
					}
					else // snapped out
						BattleScriptCall(BattleScript_MoveUsedIsConfusedNoMore);
						
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_PARALYSED: // paralysis
			    if (gBattleMons[gBattlerAttacker].status1.id == STATUS1_PARALYSIS && !RandomMax(4))
				{
					gProtectStructs[gBattlerAttacker].prlzImmobility = TRUE;
					gBattlescriptCurrInstr = BattleScript_MoveUsedIsParalyzed;
					CancelMultiTurnMoves(gBattlerAttacker);
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_GHOST: // GHOST in pokemon tower
                if (IS_BATTLE_TYPE_GHOST_WITHOUT_SCOPE)
				{
					gBattlescriptCurrInstr = GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER ? BattleScript_TooScaredToMove : BattleScript_GhostGetOutGetOut;
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_IN_LOVE: // infatuation
			    if (gBattleMons[gBattlerAttacker].status2 & STATUS2_INFATUATION)
				{
					gBattleScripting.battler = gDisableStructs[gBattlerAttacker].infatuatedWith;
					
					if (RandomPercent(50))
						BattleScriptCall(BattleScript_MoveUsedIsInLove);
					else
					{
						BattleScriptPush(BattleScript_MoveUsedIsInLoveCantAttack);
						gBattlescriptCurrInstr = BattleScript_MoveUsedIsInLove;
						gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
						gProtectStructs[gBattlerAttacker].loveImmobility = TRUE;
						CancelMultiTurnMoves(gBattlerAttacker);
					}
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_STANCE_CHANGE: // check Stance Change
			    if (GetBattlerAbility(gBattlerAttacker) == ABILITY_STANCE_CHANGE)
				{
					u16 newSpecies = TryDoBattleFormChange(gBattlerAttacker, FORM_CHANGE_MOVE);
					
					if (newSpecies)
						gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CHANGED_TO_SHIELD;
					else
					{
						newSpecies = TryDoBattleFormChange(gBattlerAttacker, FORM_CHANGE_MOVE_SPLIT);
						
						if (newSpecies)
							gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CHANGED_TO_BLADE;
					}
					
					if (newSpecies)
					{
						DoBattleFormChange(gBattlerAttacker, newSpecies, FALSE, TRUE, FALSE);
						BattleScriptCall(BattleScript_StanceChangeActivation);
						++effect;
					}
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_BIDE: // bide
			    if (gBattleMons[gBattlerAttacker].status2 & STATUS2_BIDE)
				{
					gBattleMons[gBattlerAttacker].status2 -= STATUS2_BIDE_TURN(1);
					
					if (gBattleMons[gBattlerAttacker].status2 & STATUS2_BIDE)
						gBattlescriptCurrInstr = BattleScript_BideStoringEnergy;
					else
					{
						if (gBattleStruct->battlers[gBattlerAttacker].bideTakenDamage)
						{
							gCurrentMove = MOVE_BIDE;
							gBattleScripting.bideDmg = gBattleStruct->battlers[gBattlerAttacker].bideTakenDamage * 2;
							gBattlerTarget = gBattleStruct->battlers[gBattlerAttacker].bideTakenDamageBattler;
							
							if (!IsBattlerAlive(gBattlerTarget))
								gBattlerTarget = GetMoveTarget(gCurrentMove, MOVE_TARGET_SELECTED + 1);
							
							gBattlescriptCurrInstr = BattleScript_BideAttack;
						}
						else
							gBattlescriptCurrInstr = BattleScript_BideNoEnergyToAttack;
					}
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_THAW: // move thawing
			    if (gBattleMons[gBattlerAttacker].status1.id == STATUS1_FREEZE && gBattleMoves[gCurrentMove].flags.thawUser)
				{
					ClearBattlerStatus(gBattlerAttacker);
					BattleScriptCall(BattleScript_MoveUsedUnfroze);
					gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_DEFROST_BY_MOVE;
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_NATURAL_GIFT:
			    if (gBattleMoves[gCurrentMove].effect == EFFECT_NATURAL_GIFT && (!BattlerHeldItemCanBeUsed(gBattlerAttacker)
				|| ItemId_GetPocket(gBattleMons[gBattlerAttacker].item) != POCKET_BERRY_POUCH))
				{
					gBattlescriptCurrInstr = BattleScript_ButItFailedAtkStringPpReduce;
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_PRIMAL_WEATHER: // primal weather
			    if (!IS_MOVE_STATUS(gCurrentMove))
				{
					switch (gBattleStruct->dynamicMoveType)
					{
						case TYPE_FIRE:
							if (IsBattlerWeatherAffected(gBattlerAttacker, B_WEATHER_RAIN_PRIMAL))
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_FIZZLES_ATTACK;
								++effect;
							}
							break;
						case TYPE_WATER:
							if (IsBattlerWeatherAffected(gBattlerAttacker, B_WEATHER_SUN_PRIMAL))
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_EVAPORATE_ATTACK;
								++effect;
							}
							break;
					}
					if (effect)
					{
						PrepareTypeBuffer(gBattleTextBuff1, gBattleStruct->dynamicMoveType);
						gBattlescriptCurrInstr = BattleScript_PrimalWeatherBlocksMove;
						CancelMultiTurnMoves(gBattlerAttacker);
						gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
						gMoveResultFlags = MOVE_RESULT_FAILED;
					}
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_PRANKSTER: // check Prankster
			    if (GetBattlerAbility(gBattlerAttacker) == ABILITY_PRANKSTER && IS_MOVE_STATUS(gCurrentMove) && IsBattlerAlive(gBattlerTarget)
				&& IsBattlerOfType(gBattlerTarget, TYPE_DARK) && !IsBattlerAlly(gBattlerAttacker, gBattlerTarget))
				{
					u8 moveTarget = GetBattlerMoveTargetType(gBattlerAttacker, gCurrentMove);
					
					if (moveTarget != MOVE_TARGET_OPPONENTS_FIELD && (moveTarget != MOVE_TARGET_ALL_BATTLERS || gCurrentMove == MOVE_PERISH_SONG || gCurrentMove == MOVE_ROTOTILLER))
					{
						if (!IsDoubleBattleForBattler(gBattlerTarget) || (moveTarget != MOVE_TARGET_BOTH && moveTarget != MOVE_TARGET_FOES_AND_ALLY
						&& moveTarget != MOVE_TARGET_OPPONENTS))
							CancelMultiTurnMoves(gBattlerAttacker);
						gBattlescriptCurrInstr = BattleScript_DarkTypePreventsPrankster;
						++effect;
					}
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_SPORE_MOVE:
			    if (IsBattlerAlive(gBattlerTarget) && gBattlerAttacker != gBattlerTarget && gBattleMoves[gCurrentMove].flags.powderMove
				&& !IsBattlerAffectedBySpore(gBattlerTarget))
				{
					if (GetBattlerAbility(gBattlerTarget) != ABILITY_OVERCOAT)
						gBattleScripting.bypassAbilityPopUp = TRUE;
					
					CancelMultiTurnMoves(gBattlerAttacker);
					gBattlescriptCurrInstr = BattleScript_PowderMoveNoEffect;
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_DAMP:
			    if (gBattleMoves[gCurrentMove].effect == EFFECT_EXPLOSION)
				{
					u8 i;
					
					for (i = 0; i < gBattlersCount; i++)
					{
						if (GetBattlerAbility(i) == ABILITY_DAMP)
							break;
					}
					if (i != gBattlersCount)
					{
						gBattleScripting.battler = i;
						gBattlescriptCurrInstr = BattleScript_DampStopsExplosion;
						gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
						++effect;
					}
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_MULTIHIT_MOVE:
			    if (!gSpecialStatuses[gBattlerAttacker].multiHitOn) // For don't stay in an infinite multi-hit loop
				{
					if (gBattleMoves[gCurrentMove].effect == EFFECT_MULTI_HIT)
					{
						if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_TRANSFORMED) && gBattleMons[gBattlerAttacker].species == SPECIES_GRENINJA_ASH
						&& gCurrentMove == MOVE_WATER_SHURIKEN)
							gMultiHitCounter = 3;
						else if (GetBattlerAbility(gBattlerAttacker) == ABILITY_SKILL_LINK)
							gMultiHitCounter = 5;
						else
							SetRandomMultiHitCounter();
						
						PrepareByteNumberBuffer(gBattleScripting.multihitString, 1, 0);
					}
					else if (gBattleMoves[gCurrentMove].effect == EFFECT_BEAT_UP)
					{
						gMultiHitCounter = GetNumBeatUpHits(gBattlerAttacker);
						gBattleCommunication[MULTIUSE_STATE] = 0; // For later
						PrepareByteNumberBuffer(gBattleScripting.multihitString, 1, 0);
					}
					else if (gBattleMoves[gCurrentMove].strikeCount > 1)
					{
						gBattleScripting.tripleKickPower = 0;
						gMultiHitCounter = gBattleMoves[gCurrentMove].strikeCount;
						PrepareByteNumberBuffer(gBattleScripting.multihitString, 1, 0);
					}
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_END:
			    break;
        }

    }
	while (gBattleStruct->atkCancellerTracker != CANCELLER_END && !effect);
	
    return effect;
}

bool8 HasNoMonsToSwitch(u8 battler, u8 partyIdBattlerOn1, u8 partyIdBattlerOn2)
{
    u8 i, playerId, flankId;
    struct Pokemon *party;

    if (!IsDoubleBattleForBattler(battler))
        return FALSE;
    else
    {
		party = GetBattlerParty(battler);
		
        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
        {
            playerId = GetBattlerMultiplayerId(battler);
            flankId = GetLinkTrainerFlankId(playerId);
			
            for (i = flankId * MULTI_PARTY_SIZE; i < flankId * MULTI_PARTY_SIZE + MULTI_PARTY_SIZE; ++i)
            {
                if (MonCanBattle(&party[i]))
                    break;
            }
            return (i == flankId * MULTI_PARTY_SIZE + MULTI_PARTY_SIZE);
        }
        else
        {
            if (GetBattlerSide(battler) == B_SIDE_OPPONENT)
            {
                playerId = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
                flankId = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
            }
            else
            {
                playerId = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
                flankId = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
            }
            if (partyIdBattlerOn1 == PARTY_SIZE)
                partyIdBattlerOn1 = gBattlerPartyIndexes[playerId];
            if (partyIdBattlerOn2 == PARTY_SIZE)
                partyIdBattlerOn2 = gBattlerPartyIndexes[flankId];
			
            for (i = 0; i < PARTY_SIZE; ++i)
            {
                if (MonCanBattle(&party[i]) && i != partyIdBattlerOn1 && i != partyIdBattlerOn2 && i != gBattleStruct->battlers[flankId].monToSwitchIntoId
				&& i != gBattleStruct->battlers[playerId].monToSwitchIntoId)
					break;
            }
            return (i == PARTY_SIZE);
        }
    }
}

void CopyStatusStringToBattleBuffer1(u8 statusId)
{
	StringCopy(gBattleTextBuff1, gNonVolatileStatusConditions[statusId - 1].placeholder);
}

void ClearBattlerStatus(u8 battler)
{
	CopyStatusStringToBattleBuffer1(gBattleMons[battler].status1.id);
	gBattleMons[battler].status1.id = STATUS1_NONE;
	gBattleMons[battler].status2 &= ~(STATUS2_NIGHTMARE); // fix nightmare glitch
	gBattleScripting.battler = battler;
	BtlController_EmitSetMonData(battler, BUFFER_A, REQUEST_STATUS_BATTLE, 0, sizeof(gBattleMons[battler].status1), &gBattleMons[battler].status1);
	MarkBattlerForControllerExec(battler);
}

static bool8 TryActivateCommander(u8 battlerId, bool8 cancelAction)
{
	u8 partner = BATTLE_PARTNER(battlerId);
	
	if (GetBattlerAbility(battlerId) == ABILITY_COMMANDER && IsBattlerAlive(partner) && gBattleMons[partner].species == SPECIES_DONDOZO && !gDisableStructs[partner].commanderActivated)
	{
		gDisableStructs[partner].commanderActivated = TRUE;
		gStatuses3[battlerId] |= STATUS3_COMMANDING;
		gBattleScripting.animArg2 = gBattleScripting.battler = battlerId;
		SaveAttackerToStack(partner);
		BattleScriptPushCursorAndCallback(BattleScript_CommanderActivates);
		
		if (cancelAction)
			gActionsByTurnOrder[battlerId] = B_ACTION_FINISHED;
		
		return TRUE;
	}
	return FALSE;
}

// Check if battlerId is currently being commanded by it's ally
bool8 IsBattlerBeingCommanded(u8 battlerId)
{
	if (gDisableStructs[battlerId].commanderActivated && IsBattlerAlive(BATTLE_PARTNER(battlerId)) && (gStatuses3[BATTLE_PARTNER(battlerId)] & STATUS3_COMMANDING))
		return TRUE;
	return FALSE;
}

static bool8 IsSwitchInAbilityBlockedByGhostBattle(u16 ability)
{
	if (IS_BATTLE_TYPE_GHOST_WITHOUT_SCOPE)
	{
		if (ability == ABILITY_TRACE || ability == ABILITY_DOWNLOAD || ability == ABILITY_FOREWARN || ability == ABILITY_UNNERVE || ability == ABILITY_FRISK
		|| ability == ABILITY_IMPOSTER || ability == ABILITY_ANTICIPATION || ability == ABILITY_INTIMIDATE || ability == ABILITY_SUPERSWEET_SYRUP)
			return TRUE;
	}
	return FALSE;
}

u8 AbilityBattleEffects(u8 caseId, u8 battler)
{
    u8 moveType, effect = 0, moveTarget;
	u16 ability;
	s32 i;

    if (!(gBattleTypeFlags & BATTLE_TYPE_SAFARI))
	{
		ability = GetBattlerAbility(battler);
		moveType = gBattleStruct->dynamicMoveType;
		
		switch (caseId)
		{
			case ABILITYEFFECT_ON_SWITCHIN:
			    if (!IsSwitchInAbilityBlockedByGhostBattle(ability))
				{
					gBattleScripting.battler = battler;
				
					switch (ability)
					{
						case ABILITY_DRIZZLE:
							if (TryChangeBattleWeather(battler, ENUM_WEATHER_RAIN))
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_STARTED_RAIN;
								gBattleScripting.animArg1 = B_ANIM_RAIN_CONTINUES;
								BattleScriptPushCursorAndCallback(BattleScript_SwitchInWeatherAbilityActivates);
								++effect;
							}
							break;
						case ABILITY_SAND_STREAM:
							if (TryChangeBattleWeather(battler, ENUM_WEATHER_SANDSTORM))
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SANDSTORM_UP;
								gBattleScripting.animArg1 = B_ANIM_SANDSTORM_CONTINUES;
								BattleScriptPushCursorAndCallback(BattleScript_SwitchInWeatherAbilityActivates);
								++effect;
							}
							break;
						case ABILITY_DROUGHT:
							if (TryChangeBattleWeather(battler, ENUM_WEATHER_SUN))
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SUN_TURN_HARSH;
								gBattleScripting.animArg1 = B_ANIM_SUN_CONTINUES;
								BattleScriptPushCursorAndCallback(BattleScript_SwitchInWeatherAbilityActivates);
								++effect;
							}
							break;
						case ABILITY_SNOW_WARNING:
							if (TryChangeBattleWeather(battler, ENUM_WEATHER_HAIL))
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_STARTED_HAIL;
								gBattleScripting.animArg1 = B_ANIM_HAIL_CONTINUES;
								BattleScriptPushCursorAndCallback(BattleScript_SwitchInWeatherAbilityActivates);
								++effect;
							}
							break;
						case ABILITY_PRIMORDIAL_SEA:
							if (TryChangeBattleWeather(battler, ENUM_WEATHER_RAIN_PRIMAL))
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_HEAVY_RAIN;
								gBattleScripting.animArg1 = B_ANIM_RAIN_CONTINUES;
								BattleScriptPushCursorAndCallback(BattleScript_SwitchInWeatherAbilityActivates);
								++effect;
							}
							break;
						case ABILITY_DESOLATE_LAND:
							if (TryChangeBattleWeather(battler, ENUM_WEATHER_SUN_PRIMAL))
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SUN_TURN_EXTREMELY_HARSH;
								gBattleScripting.animArg1 = B_ANIM_SUN_CONTINUES;
								BattleScriptPushCursorAndCallback(BattleScript_SwitchInWeatherAbilityActivates);
								++effect;
							}
							break;
						case ABILITY_DELTA_STREAM:
							if (TryChangeBattleWeather(battler, ENUM_WEATHER_STRONG_WINDS))
							{
								PrepareTypeBuffer(gBattleTextBuff1, TYPE_FLYING);
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_STRONG_WINDS_PROTECTING;
								gBattleScripting.animArg1 = B_ANIM_STRONG_WINDS_CONTINUE;
								BattleScriptPushCursorAndCallback(BattleScript_SwitchInWeatherAbilityActivates);
								++effect;
							}
							break;
						case ABILITY_INTIMIDATE:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								SaveAttackerToStack(battler);
								BattleScriptPushCursorAndCallback(BattleScript_IntimidateActivates);
								++effect;
							}
							break;
						case ABILITY_SUPERSWEET_SYRUP:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								SaveAttackerToStack(battler);
								BattleScriptPushCursorAndCallback(BattleScript_SupersweetSyrupActivates);
								++effect;
							}
							break;
						case ABILITY_TRACE:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								u8 tgt2, target = 0xFF, tgt1 = BATTLE_OPPOSITE(battler);
								u16 tgt2Ability, tgt1Ability = gBattleMons[tgt1].ability;
								
								if (IsDoubleBattleForBattler(tgt1))
								{
									tgt2 = BATTLE_PARTNER(tgt1);
									tgt2Ability = gBattleMons[tgt2].ability;
									
									if (IS_WHOLE_SIDE_ALIVE(tgt1) && !gAbilities[tgt1Ability].cantBeTraced && !gAbilities[tgt2Ability].cantBeTraced)
										target = RandomPercent(50) ? tgt1 : tgt2;
									else if (IsBattlerAlive(tgt1) && !gAbilities[tgt1Ability].cantBeTraced)
										target = tgt1;
									else if (IsBattlerAlive(tgt2) && !gAbilities[tgt2Ability].cantBeTraced)
										target = tgt2;
								}
								else if (IsBattlerAlive(tgt1) && !gAbilities[tgt1Ability].cantBeTraced)
									target = tgt1;
								
								if (target != 0xFF)
								{
									SaveAttackerToStack(battler);
									SaveTargetToStack(target);
									BattleScriptPushCursorAndCallback(BattleScript_TraceActivates);
									gSpecialStatuses[battler].switchInAbilityDone = TRUE;
									++effect;
								}
							}
							break;
						case ABILITY_CLOUD_NINE:
						case ABILITY_AIR_LOCK:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								BattleScriptPushCursorAndCallback(BattleScript_AirLock);
								++effect;
							}
							break;
						case ABILITY_MOLD_BREAKER:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_BREAKS_THE_MOLD;
								BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
								++effect;
							}
							break;
						case ABILITY_SLOW_START:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								gDisableStructs[battler].slowStartTimer = 5;
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CANT_GET_GOING;
								BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
								++effect;
							}
							break;
						case ABILITY_IMPOSTER:
							if (IsBattlerAlive(BATTLE_OPPOSITE(battler)) && !gDisableStructs[battler].imposterActivated && TryTransformIntoBattler(battler, BATTLE_OPPOSITE(battler)))
							{
								gDisableStructs[battler].imposterActivated = TRUE;
								SaveAttackerToStack(battler);
								SaveTargetToStack(BATTLE_OPPOSITE(battler));
								gBattleStruct->battlers[battler].abilityOverride = ability;
								BattleScriptPushCursorAndCallback(BattleScript_ImposterActivates);
								++effect;
							}
							break;
						case ABILITY_TURBOBLAZE:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_BLAZING_AURA;
								BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
								++effect;
							}
							break;
						case ABILITY_TERAVOLT:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_BURSTING_AURA;
								BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
								++effect;
							}
							break;
						case ABILITY_DARK_AURA:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_DARK_AURA;
								BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
								++effect;
							}
							break;
						case ABILITY_FAIRY_AURA:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_FAIRY_AURA;
								BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
								++effect;
							}
							break;
						case ABILITY_AURA_BREAK:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_REVERSED_AURAS;
								BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
								++effect;
							}
							break;
						case ABILITY_COMATOSE:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_PKMN_DROWSING;
								BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
								++effect;
							}
							break;
						case ABILITY_PRESSURE:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_EXERTING_ABILITY;
								BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
								++effect;
							}
							break;
						case ABILITY_VESSEL_OF_RUIN:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								PrepareStatBuffer(gBattleTextBuff1, STAT_SPATK);
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_WEAKENED_STAT;
								BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
								++effect;
							}
							break;
						case ABILITY_SWORD_OF_RUIN:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								PrepareStatBuffer(gBattleTextBuff1, STAT_DEF);
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_WEAKENED_STAT;
								BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
								++effect;
							}
							break;
						case ABILITY_TABLETS_OF_RUIN:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								PrepareStatBuffer(gBattleTextBuff1, STAT_ATK);
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_WEAKENED_STAT;
								BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
								++effect;
							}
							break;
						case ABILITY_BEADS_OF_RUIN:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								PrepareStatBuffer(gBattleTextBuff1, STAT_SPDEF);
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_WEAKENED_STAT;
								BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
								++effect;
							}
							break;
						case ABILITY_ZERO_TO_HERO:
							if (gBattleStruct->sides[GetBattlerSide(battler)].party[gBattlerPartyIndexes[battler]].zeroToHeroActivated)
							{
								gBattleStruct->sides[GetBattlerSide(battler)].party[gBattlerPartyIndexes[battler]].zeroToHeroActivated = FALSE;
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_UNDERWENT_TRANSFORMATION;
								BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
								++effect;
							}
							break;
						case ABILITY_SCHOOLING:
							if (gBattleMons[battler].level >= 20)
							{
								u16 newSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_HP);
								
								if (newSpecies)
								{
									DoBattleFormChange(battler, newSpecies, FALSE, TRUE, FALSE);
									gBattleScripting.animArg1 = B_ANIM_SCHOOLING;
									gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_FORMED_SCHOOL;
									BattleScriptPushCursorAndCallback(BattleScript_ZenModeActivatesPause);
									++effect;
								}
							}
							break;
						case ABILITY_ZEN_MODE:
							{
								u16 newSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_HP);
								
								if (newSpecies)
								{
									DoBattleFormChange(battler, newSpecies, TRUE, TRUE, FALSE);
									gBattleScripting.animArg1 = B_ANIM_FORM_CHANGE;
									gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_ABL_TRIGGERED;
									BattleScriptPushCursorAndCallback(BattleScript_ZenModeActivatesPause);
									++effect;
								}
							}
							break;
						case ABILITY_SHIELDS_DOWN:
						{
							u16 newSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_HP);
							
							if (newSpecies)
							{
								DoBattleFormChange(battler, newSpecies, FALSE, TRUE, FALSE);
								gBattleScripting.animArg1 = B_ANIM_FORM_CHANGE;
								gBattleCommunication[MULTISTRING_CHOOSER] = newSpecies == SPECIES_MINIOR_METEOR ? B_MSG_ABL_DEACTIVATED : B_MSG_ABL_ACTIVATED;
								BattleScriptPushCursorAndCallback(BattleScript_ZenModeActivatesPause);
								++effect;
							}
						}
						break;
						case ABILITY_ANTICIPATION:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								u8 j, type;
								u16 flags, move;
								
								for (i = 0; i < MAX_BATTLERS_COUNT; i++)
								{
									if (IsBattlerAlive(i) && !IsBattlerAlly(battler, i))
									{
										for (j = 0; j < MAX_MON_MOVES; j++)
										{
											move = gBattleMons[i].moves[j];
											
											if (move)
											{
												if (gBattleMoves[move].effect == EFFECT_OHKO)
													++effect;
												else if (!IS_MOVE_STATUS(move))
												{
													if (gBattleMoves[move].effect == EFFECT_HIDDEN_POWER)
														type = GetHiddenPowerType(GetBattlerPartyIndexPtr(i));
													else
														type = gBattleMoves[move].type;
													
													if (TypeCalc(move, type, i, battler, FALSE, &flags) == TYPE_MUL_SUPER_EFFECTIVE)
														++effect;
												}
											}
										}
									}
								}
								if (effect)
								{
									gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SHUDDERED;
									BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
									gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								}
							}
							break;
						case ABILITY_DOWNLOAD:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								u8 statId, opposingBattler = BATTLE_OPPOSITE(battler);
								u32 def = 0, spDef = 0, var;
								
								for (i = 0; i < NUM_BATTLERS_PER_SIDE; i++, opposingBattler = BATTLE_PARTNER(opposingBattler))
								{
									if (IsBattlerAlive(opposingBattler))
									{
										APPLY_STAT_MOD(var, &gBattleMons[opposingBattler], gBattleMons[opposingBattler].defense, STAT_DEF);
										def += var;
										
										APPLY_STAT_MOD(var, &gBattleMons[opposingBattler], gBattleMons[opposingBattler].spDefense, STAT_SPDEF);
										spDef += var;
									}
								}
								def /= 2;
								spDef /= 2;
								
								if (def < spDef)
									statId = STAT_ATK;
								else
									statId = STAT_SPATK;
								
								if (CompareStat(battler, statId, MAX_STAT_STAGES, CMP_LESS_THAN))
								{
									SetStatChanger(statId, +1);
									SaveAttackerToStack(battler);
									BattleScriptPushCursorAndCallback(BattleScript_BattlerAbilityStatRaiseOnSwitchIn);
									gSpecialStatuses[battler].switchInAbilityDone = TRUE;
									++effect;
								}
							}
							break;
						case ABILITY_FOREWARN:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								struct Forewarn
								{
									u8 battlerId;
									u8 power;
									u16 moveId;
								};
								u8 j, bestId, count;
								struct Forewarn *data = Alloc(sizeof(struct Forewarn) * MAX_BATTLERS_COUNT * MAX_MON_MOVES);
								
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								
								for (i = 0, count = 0; i < MAX_BATTLERS_COUNT; i++)
								{
									if (IsBattlerAlive(i) && !IsBattlerAlly(battler, i))
									{
										for (j = 0; j < MAX_MON_MOVES; j++)
										{
											if (gBattleMons[i].moves[j])
											{
												data[count].moveId = gBattleMons[i].moves[j];
												data[count].battlerId = i;
												
												switch (gBattleMoves[data[count].moveId].effect)
												{
													case EFFECT_OHKO:
														data[count].power = 150;
														break;
													case EFFECT_COUNTER_ATTACK:
														data[count].power = 120;
														break;
													default:
														if (gBattleMoves[data[count].moveId].power == 1)
															data[count].power = 80;
														else
															data[count].power = gBattleMoves[data[count].moveId].power;
														break;
												}
												++count;
											}
										}
									}
								}
								for (i = 1, bestId = 0; i < count; i++)
								{
									if (data[i].power > data[bestId].power || (data[i].power == data[bestId].power && RandomPercent(50)))
										bestId = i;
								}
								gBattleScripting.battler = data[bestId].battlerId;
								PrepareMoveBuffer(gBattleTextBuff1, data[bestId].moveId);
								Free(data);
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_ALERTED;
								BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
								++effect;
							}
							break;
						case ABILITY_FRISK:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								SaveAttackerToStack(battler);
								BattleScriptPushCursorAndCallback(BattleScript_Frisk);
								++effect;
							}
							break;
						case ABILITY_SCREEN_CLEANER:
							if (!gSpecialStatuses[battler].switchInAbilityDone && TryRemoveScreens(battler, TRUE, TRUE))
							{
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SCREENS_CLEANSED;
								BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
								++effect;
							}
							break;
						case ABILITY_CURIOUS_MEDICINE:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								gBattleScripting.battler = BATTLE_PARTNER(battler);
								
								if (IsBattlerAlive(gBattleScripting.battler) && TryResetBattlerStatChanges(gBattleScripting.battler))
								{
									gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_STAT_CHANGES_RESETED;
									BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
									gSpecialStatuses[battler].switchInAbilityDone = TRUE;
									++effect;
								}
							}
							break;
						case ABILITY_HOSPITALITY:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								u8 partner = BATTLE_PARTNER(battler);
								
								if (IsBattlerAlive(partner) && !BATTLER_MAX_HP(partner))
								{
									gBattleMoveDamage = gBattleMons[partner].maxHP / 4;
									if (gBattleMoveDamage == 0)
										gBattleMoveDamage = 1;
									gBattleMoveDamage *= -1;
									PrepareMonNickWithPrefixBuffer(gBattleTextBuff1, partner, gBattlerPartyIndexes[partner]);
									BattleScriptPushCursorAndCallback(BattleScript_Hospitality);
									gSpecialStatuses[battler].switchInAbilityDone = TRUE;
									++effect;
								}
							}
							break;
						case ABILITY_COSTAR:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								u8 partner = BATTLE_PARTNER(battler);
								
								if (IsBattlerAlive(partner))
								{
									if (CountBattlerStatIncreases(partner, FALSE)) // Does it copies accuracy and evasion or negative stat stages ?
									{
										CopyBattlerStatChanges(battler, partner);
										++effect;
									}
									
									if (CopyBattlerCritModifier(battler, partner)) // It can also copy Focus Energy status
										++effect;
	
									if (effect)
									{
										PrepareMonNickWithPrefixBuffer(gBattleTextBuff1, partner, gBattlerPartyIndexes[partner]);
										gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_COPIED_STAT_CHANGES;
										BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
										gSpecialStatuses[battler].switchInAbilityDone = TRUE;
									}
								}
							}
							break;
						case ABILITY_SUPREME_OVERLORD:
							if (!gSpecialStatuses[battler].switchInAbilityDone)
							{
								gDisableStructs[battler].supremeOverlordBoost = min(PARTY_SIZE - 1, gBattleStruct->sides[GetBattlerSide(battler)].faintCounter);
								
								if (gDisableStructs[battler].supremeOverlordBoost > 0)
								{
									gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_GAINED_STRENGTH;
									BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
									gSpecialStatuses[battler].switchInAbilityDone = TRUE;
									++effect;
								}
							}
							break;
						case ABILITY_LIMBER:
							if (!gSpecialStatuses[battler].switchInAbilityDone && gBattleMons[battler].status1.id == STATUS1_PARALYSIS)
							{
								LIMBER_ACTIVATES:
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								ClearBattlerStatus(battler);
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CURED_OF_ITS_PROBLEM;
								BattleScriptPushCursorAndCallback(BattleScript_AbilityCuredStatusEnd3);
								++effect;
							}
							break;
						case ABILITY_INSOMNIA:
						case ABILITY_VITAL_SPIRIT:
							if (!gSpecialStatuses[battler].switchInAbilityDone && gBattleMons[battler].status1.id == STATUS1_SLEEP)
								goto LIMBER_ACTIVATES;
							break;
						case ABILITY_IMMUNITY:
							if (!gSpecialStatuses[battler].switchInAbilityDone && (gBattleMons[battler].status1.id == STATUS1_POISON || gBattleMons[battler].status1.id == STATUS1_TOXIC_POISON))
								goto LIMBER_ACTIVATES;
							break;
						case ABILITY_MAGMA_ARMOR:
							if (!gSpecialStatuses[battler].switchInAbilityDone && gBattleMons[battler].status1.id == STATUS1_FREEZE)
								goto LIMBER_ACTIVATES;
							break;
						case ABILITY_PASTEL_VEIL:
							if (!gSpecialStatuses[battler].switchInAbilityDone && IsBattlerAlive(BATTLE_PARTNER(battler))
							&& (gBattleMons[BATTLE_PARTNER(battler)].status1.id == STATUS1_POISON || gBattleMons[BATTLE_PARTNER(battler)].status1.id == STATUS1_TOXIC_POISON))
							{
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								SaveTargetToStack(battler);
								ClearBattlerStatus(BATTLE_PARTNER(battler));
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CURED_OF_ITS_PROBLEM;
								BattleScriptPushCursorAndCallback(BattleScript_PastelVeilActivates);
								++effect;
							}
							break;
						case ABILITY_INTREPID_SWORD:
							if (CompareStat(battler, STAT_ATK, MAX_STAT_STAGES, CMP_LESS_THAN)
							&& !gBattleStruct->sides[GetBattlerSide(battler)].party[gBattlerPartyIndexes[battler]].intrepidSwordActivated)
							{
								gBattleStruct->sides[GetBattlerSide(battler)].party[gBattlerPartyIndexes[battler]].intrepidSwordActivated = TRUE;
								SetStatChanger(STAT_ATK, +1);
								SaveAttackerToStack(battler);
								BattleScriptPushCursorAndCallback(BattleScript_BattlerAbilityStatRaiseOnSwitchIn);
								++effect;
							}
							break;
						case ABILITY_DAUNTLESS_SHIELD:
							if (CompareStat(battler, STAT_DEF, MAX_STAT_STAGES, CMP_LESS_THAN)
							&& !gBattleStruct->sides[GetBattlerSide(battler)].party[gBattlerPartyIndexes[battler]].dauntlessShieldActivated)
							{
								gBattleStruct->sides[GetBattlerSide(battler)].party[gBattlerPartyIndexes[battler]].dauntlessShieldActivated = TRUE;
								SetStatChanger(STAT_DEF, +1);
								SaveAttackerToStack(battler);
								BattleScriptPushCursorAndCallback(BattleScript_BattlerAbilityStatRaiseOnSwitchIn);
								++effect;
							}
							break;
						case ABILITY_WIND_RIDER:
							if (!gSpecialStatuses[battler].switchInAbilityDone && (gSideStatuses[GetBattlerSide(battler)] & SIDE_STATUS_TAILWIND))
							{
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								SetStatChanger(STAT_ATK, +1);
								SaveAttackerToStack(battler);
								BattleScriptPushCursorAndCallback(BattleScript_BattlerAbilityStatRaiseOnSwitchIn);
								++effect;
							}
							break;
						case ABILITY_COMMANDER:
							if (!gSpecialStatuses[battler].switchInAbilityDone && TryActivateCommander(battler, FALSE))
								++effect;
							break;
					}
				}
				break;
			case ABILITYEFFECT_ENDTURN:
			    switch (ability)
				{
					case ABILITY_SOLAR_POWER:
					    SOLAR_POWER_SUN_CHECK:
						if (IsBattlerWeatherAffected(battler, B_WEATHER_SUN_ANY))
						{
							gBattleMoveDamage = gBattleMons[battler].maxHP / 8;
							if (gBattleMoveDamage == 0)
								gBattleMoveDamage = 1;
							BattleScriptPushCursorAndCallback(BattleScript_DrySkinSunActivates);
							++effect;
						}
						break;
					case ABILITY_RAIN_DISH:
					    if (IsBattlerWeatherAffected(battler, B_WEATHER_RAIN_ANY) && !BATTLER_MAX_HP(battler) && !(gStatuses3[battler] & STATUS3_HEAL_BLOCK))
						{
							RAIN_DISH_SCRIPT:
							gBattleMoveDamage = gBattleMons[battler].maxHP / 16;
							if (gBattleMoveDamage == 0)
								gBattleMoveDamage = 1;
							gBattleMoveDamage *= -1;
							BattleScriptPushCursorAndCallback(BattleScript_RainDishActivates);
							++effect;
						}
						break;
					case ABILITY_DRY_SKIN:
					    if (IsBattlerWeatherAffected(battler, B_WEATHER_RAIN_ANY) && !BATTLER_MAX_HP(battler) && !(gStatuses3[battler] & STATUS3_HEAL_BLOCK))
						{
							DRY_SKIN_SCRIPT:
							gBattleMoveDamage = gBattleMons[battler].maxHP / 8;
							if (gBattleMoveDamage == 0)
								gBattleMoveDamage = 1;
							gBattleMoveDamage *= -1;
							BattleScriptPushCursorAndCallback(BattleScript_RainDishActivates);
							++effect;
						}
						else
							goto SOLAR_POWER_SUN_CHECK;
						break;
					case ABILITY_ICE_BODY:
					    if (IsBattlerWeatherAffected(battler, B_WEATHER_HAIL) && !BATTLER_MAX_HP(battler) && !(gStatuses3[battler] & STATUS3_HEAL_BLOCK))
							goto RAIN_DISH_SCRIPT;
						break;
					case ABILITY_POISON_HEAL:
					    if ((gBattleMons[battler].status1.id == STATUS1_POISON || gBattleMons[battler].status1.id == STATUS1_TOXIC_POISON) && !BATTLER_MAX_HP(battler)
						&& !(gStatuses3[battler] & STATUS3_HEAL_BLOCK))
							goto DRY_SKIN_SCRIPT;
						break;
					case ABILITY_SHED_SKIN:
					    if (gBattleMons[battler].status1.id && !RandomMax(3))
						{
							SHED_SKIN_SCRIPT:
							ClearBattlerStatus(battler);
							gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CURED_ITS_PROBLEM;
							BattleScriptPushCursorAndCallback(BattleScript_ShedSkinHealerActivates);
							++effect;
						}
						break;
					case ABILITY_HYDRATION:
					    if (gBattleMons[battler].status1.id && IsBattlerWeatherAffected(battler, B_WEATHER_RAIN_ANY))
							goto SHED_SKIN_SCRIPT;
						break;
					case ABILITY_HEALER:
					    if (IsBattlerAlive(BATTLE_PARTNER(battler)) && gBattleMons[BATTLE_PARTNER(battler)].status1.id && RandomPercent(30))
						{
							ClearBattlerStatus(BATTLE_PARTNER(battler));
							gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CURED_POKEMON_PROBLEM;
							BattleScriptPushCursorAndCallback(BattleScript_ShedSkinHealerActivates);
							++effect;
						}
						break;
					case ABILITY_SPEED_BOOST:
					    if (CompareStat(battler, STAT_SPEED, MAX_STAT_STAGES, CMP_LESS_THAN) && gDisableStructs[battler].isFirstTurn != 2
						// in gen5 onwards Speed Boost fails to activate if the user failed to run from a wild battle
						&& gActionsByTurnOrder[gBattlerByTurnOrder[battler]] != B_ACTION_RUN) // tried to run. If the game reach in this part is because it failed to do it.
						{
							BattleScriptPushCursorAndCallback(BattleScript_SpeedBoostActivates);
							++effect;
						}
						break;
					case ABILITY_TRUANT:
					    if (gBattleMons[battler].status1.id != STATUS1_SLEEP)
							gDisableStructs[battler].truantCounter ^= TRUE;
						else
							gDisableStructs[battler].truantCounter = 0; // being asleep resets the Truant counter
						break; // don't record ability
					case ABILITY_SLOW_START:
					    if (gDisableStructs[battler].slowStartTimer && --gDisableStructs[battler].slowStartTimer == 0)
						{
							BattleScriptPushCursorAndCallback(BattleScript_SlowStartEnd);
							++effect;
						}
						break;
					case ABILITY_HARVEST:
					    if ((IsBattlerWeatherAffected(battler, B_WEATHER_SUN_ANY) || RandomPercent(50)) && !gBattleMons[battler].item
						&& ItemId_GetPocket(*GetUsedHeldItemPtr(battler)) == POCKET_BERRY_POUCH && TryRecycleBattlerItem(battler, battler))
						{
							BattleScriptPushCursorAndCallback(BattleScript_HarvestActivates);
							++effect;
						}
						break;
					case ABILITY_MOODY:
					    if (gDisableStructs[battler].isFirstTurn != 2)
						{
							u32 validToRaise = 0, validToLower = 0;
							
							for (i = STAT_ATK; i < NUM_STATS; i++)
							{
								if (CompareStat(battler, i, MIN_STAT_STAGES, CMP_GREATER_THAN))
									validToLower |= gBitTable[i];
								
								if (CompareStat(battler, i, MAX_STAT_STAGES, CMP_LESS_THAN))
									validToRaise |= gBitTable[i];
							}
							
							if (validToRaise || validToLower)
							{
								gBattleCommunication[MULTIUSE_STATE] = STAT_HP;
								
								if (validToLower)
								{
									do
									{
										i = RandomRange(STAT_ATK, NUM_STATS - 1);
									} while (!(validToLower & gBitTable[i]));
									
									gBattleCommunication[MULTIUSE_STATE] = i; // Save the stat id to decrease
									validToRaise &= ~(gBitTable[i]); // cannot raise the same stat
								}
								
								if (validToRaise)
								{
									do
									{
										i = RandomRange(STAT_ATK, NUM_STATS - 1);
									} while (!(validToRaise & gBitTable[i]));
									
									SetStatChanger(i, +2);
								}
								BattleScriptPushCursorAndCallback(BattleScript_MoodyActivates);
								++effect;
							}
						}
						break;
					case ABILITY_SCHOOLING:
					    if (gBattleMons[battler].level >= 20)
						{
							u16 newSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_HP);
							
							if (newSpecies)
							{
								DoBattleFormChange(battler, newSpecies, FALSE, TRUE, FALSE);
								gBattleScripting.battler = battler;
								gBattleScripting.animArg1 = B_ANIM_SCHOOLING;
								gBattleCommunication[MULTISTRING_CHOOSER] = newSpecies == SPECIES_WISHIWASHI ? B_MSG_STOPPED_SCHOOLING : B_MSG_FORMED_SCHOOL;
								BattleScriptPushCursorAndCallback(BattleScript_ZenModeActivates);
								++effect;
							}
						}
						break;
					case ABILITY_ZEN_MODE:
					{
						u16 newSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_HP);
						
						if (newSpecies)
						{
							DoBattleFormChange(battler, newSpecies, TRUE, TRUE, FALSE);
							gBattleScripting.battler = battler;
							gBattleScripting.animArg1 = B_ANIM_FORM_CHANGE;
							gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_ABL_TRIGGERED;
							BattleScriptPushCursorAndCallback(BattleScript_ZenModeActivates);
							++effect;
						}
						break;
					}
					case ABILITY_SHIELDS_DOWN:
					{
						u16 newSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_HP);
						
						if (newSpecies)
						{
							DoBattleFormChange(battler, newSpecies, FALSE, TRUE, FALSE);
							gBattleScripting.battler = battler;
							gBattleScripting.animArg1 = B_ANIM_FORM_CHANGE;
							gBattleCommunication[MULTISTRING_CHOOSER] = newSpecies == SPECIES_MINIOR_METEOR ? B_MSG_ABL_DEACTIVATED : B_MSG_ABL_ACTIVATED;
							BattleScriptPushCursorAndCallback(BattleScript_ZenModeActivates);
							++effect;
						}
						break;
					}
					case ABILITY_POWER_CONSTRUCT:
					{
						u16 newSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_HP);
						
						if (newSpecies)
						{
							DoBattleFormChange(battler, newSpecies, TRUE, TRUE, FALSE);
							gBattleScripting.battler = battler;
							gBattleScripting.animArg1 = B_ANIM_ZYGARDE_CELL_SWIRL;
							gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_TRANS_INTO_COMPLETE;
							BattleScriptPushCursorAndCallback(BattleScript_PowerConstructActivates);
							++effect;
						}
						break;
					}
					case ABILITY_HUNGER_SWITCH:
					{
						u16 newSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_ENDTURN);
						
						if (newSpecies)
						{
							DoBattleFormChange(battler, newSpecies, FALSE, FALSE, FALSE);
							gBattleScripting.battler = battler;
							gBattleScripting.animArg1 = B_ANIM_FORM_CHANGE;
							gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_TRANSFORMED;
							BattleScriptPushCursorAndCallback(BattleScript_ZenModeActivates);
							++effect;
						}
						break;
					}
					case ABILITY_PICKUP:
					{
						u8 pickupBattler = GetBattlerOnTopOfPickupStack(battler);
						
						if (pickupBattler != 0xFF && TryRecycleBattlerItem(battler, pickupBattler))
						{
							BattleScriptPushCursorAndCallback(BattleScript_Pickup);
							++effect;
						}
						break;
					}
					case ABILITY_BAD_DREAMS:
						BattleScriptPushCursorAndCallback(BattleScript_BadDreamsActivates);
						++effect;
						break;
					case ABILITY_BALL_FETCH:
						if (!gBattleMons[battler].item && !gBattleStruct->hasFetchedBall && gBattleStruct->lastFailedBallThrow)
						{
							gBattleStruct->hasFetchedBall = TRUE;
							gLastUsedItem = gBattleStruct->lastFailedBallThrow;
							GiveItemToBattler(battler, gBattleStruct->lastFailedBallThrow);
							BattleScriptPushCursorAndCallback(BattleScript_Pickup);
							++effect;
						}
						break;
				}
				break;
			case ABILITYEFFECT_NEUTRALIZING_GAS:
			    // Prints message only. separate from ABILITYEFFECT_ON_SWITCHIN bc activates before entry hazards
			    for (i = 0; i < gBattlersCount; i++)
				{
					if (IsBattlerAlive(i) && GetBattlerAbility(i) == ABILITY_NEUTRALIZING_GAS && !gDisableStructs[i].startedNeutralizingGas)
					{
						battler = i;
						gDisableStructs[battler].startedNeutralizingGas = TRUE;
						SaveAttackerToStack(battler);
						BattleScriptPushCursorAndCallback(BattleScript_NeutralizingGasActivates);
						++effect;
						break;
					}
				}
				break;
			case ABILITYEFFECT_UNNERVE:
			    // Prints message only. separate from ABILITYEFFECT_ON_SWITCHIN bc activates before entry hazards
				for (i = 0; i < gBattlersCount; i++)
				{
					if (IsBattlerAlive(i) && (GetBattlerAbility(i) == ABILITY_UNNERVE || GetBattlerAbility(i) == ABILITY_AS_ONE_ICE_RIDER
					|| GetBattlerAbility(i) == ABILITY_AS_ONE_SHADOW_RIDER) && !gSpecialStatuses[i].switchInAbilityDone)
					{
						gSpecialStatuses[battler].switchInAbilityDone = TRUE;
						if (GetBattlerAbility(i) == ABILITY_UNNERVE)
						{
							PrepareMonTeamPrefixBuffer(gBattleTextBuff1, BATTLE_OPPOSITE(battler));
							gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_NERVOUS_TO_EAT; // Unnerve message
						}
						else
							gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_HAS_TWO_ABILITIES; // As One message
						gBattleScripting.battler = battler = i;
						BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
						++effect;
						break;
					}
				}
				break;
			case ABILITYEFFECT_MOVES_BLOCK:
			case ABILITYEFFECT_WOULD_BLOCK_MOVE:
			    if (IsBattlerAlive(battler))
				{
					moveTarget = GetBattlerMoveTargetType(gBattlerAttacker, gCurrentMove);
				    
			        switch (ability)
				    {
				    	case ABILITY_SOUNDPROOF:
				    	    if (gBattleMoves[gCurrentMove].flags.soundMove && moveTarget != MOVE_TARGET_USER && moveTarget != MOVE_TARGET_ALL_BATTLERS)
				    			++effect;
				    		break;
				    	case ABILITY_BULLETPROOF:
				    	    if (gBattleMoves[gCurrentMove].flags.ballisticMove)
				    			++effect;
				    		break;
				    	case ABILITY_TELEPATHY:
				    	    if (IsBattlerAlive(BATTLE_PARTNER(battler)) && gBattlerAttacker == BATTLE_PARTNER(battler) && !IS_MOVE_STATUS(gCurrentMove))
				    			++effect;
				    		break;
				    	case ABILITY_QUEENLY_MAJESTY:
				    	case ABILITY_DAZZLING:
				    	case ABILITY_ARMOR_TAIL:
				    	    if (GetChosenMovePriority(gBattlerAttacker) > 0 && !IsBattlerAlly(gBattlerAttacker, battler) && !IsZMove(gCurrentMove)
				    		&& moveTarget != MOVE_TARGET_OPPONENTS_FIELD && (moveTarget != MOVE_TARGET_ALL_BATTLERS || gCurrentMove == MOVE_PERISH_SONG
				    		|| gCurrentMove == MOVE_FLOWER_SHIELD || gCurrentMove == MOVE_ROTOTILLER))
				    			++effect;
				    		break;
						case ABILITY_GOOD_AS_GOLD:
						    if (IS_MOVE_STATUS(gCurrentMove) && moveTarget != MOVE_TARGET_ALL_BATTLERS && moveTarget != MOVE_TARGET_OPPONENTS_FIELD
							&& moveTarget != MOVE_TARGET_USER)
							    ++effect;
							break;
				    }
					
					if (effect && caseId != ABILITYEFFECT_WOULD_BLOCK_MOVE)
				    {
				    	if (gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS)
				    		gHitMarker |= HITMARKER_NO_PPDEDUCT;
						
						gBattleScripting.battler = battler;
						gBattlescriptCurrInstr = BattleScript_SoundproofProtected;
				    }
				}
				break;
			case ABILITYEFFECT_ABSORBING:
			case ABILITYEFFECT_WOULD_ABSORB_MOVE:
			    if (gCurrentMove)
				{
					u8 statId, statAmount = +1;
					
					moveTarget = GetBattlerMoveTargetType(gBattlerAttacker, gCurrentMove);
					
					switch (ability)
					{
						case ABILITY_VOLT_ABSORB:
						    if (moveType == TYPE_ELECTRIC)
								effect = 1;
							break;
						case ABILITY_WATER_ABSORB:
						case ABILITY_DRY_SKIN:
						    if (moveType == TYPE_WATER)
								effect = 1;
							break;
						case ABILITY_EARTH_EATER: // Don't absorb Spikes
						    if (moveType == TYPE_GROUND && moveTarget != MOVE_TARGET_OPPONENTS_FIELD)
								effect = 1;
							break;
						case ABILITY_MOTOR_DRIVE:
						    if (moveType == TYPE_ELECTRIC)
							{
								statId = STAT_SPEED;
								effect = 2;
							}
							break;
						case ABILITY_LIGHTNING_ROD:
						    if (moveType == TYPE_ELECTRIC)
							{
								statId = STAT_SPATK;
								effect = 2;
							}
							break;
						case ABILITY_SAP_SIPPER:
						    if (moveType == TYPE_GRASS)
							{
								statId = STAT_ATK;
								effect = 2;
							}
							break;
						case ABILITY_WIND_RIDER:
						    if (gBattleMoves[gCurrentMove].flags.windMove && moveTarget != MOVE_TARGET_USER && moveTarget != MOVE_TARGET_ALL_BATTLERS)
							{
								statId = STAT_ATK;
								effect = 2;
							}
							break;
						case ABILITY_WELL_BAKED_BODY:
						    if (moveType == TYPE_FIRE)
							{
								statId = STAT_DEF;
								statAmount = +2;
								effect = 2;
							}
							break;
						case ABILITY_FLASH_FIRE:
						    if (moveType == TYPE_FIRE)
								effect = 3;
							break;
					}
					
					if (caseId != ABILITYEFFECT_WOULD_ABSORB_MOVE)
					{
						switch (effect)
						{
							case 1:
								if (gProtectStructs[gBattlerAttacker].notFirstStrike)
									gBattlescriptCurrInstr = BattleScript_MoveHPDrain;
								else
									gBattlescriptCurrInstr = BattleScript_MoveHPDrain_PPLoss;
								
								if (BATTLER_MAX_HP(battler) || (gStatuses3[battler] & STATUS3_HEAL_BLOCK))
								{
									if (gProtectStructs[gBattlerAttacker].notFirstStrike)
										gBattlescriptCurrInstr = BattleScript_MonMadeMoveUseless;
									else
										gBattlescriptCurrInstr = BattleScript_MonMadeMoveUseless_PPLoss;
								}
								else
								{
									gBattleMoveDamage = gBattleMons[battler].maxHP / 4;
									if (gBattleMoveDamage == 0)
										gBattleMoveDamage = 1;
									gBattleMoveDamage *= -1;
								}
								break;
							case 2:
								if (CompareStat(battler, statId, MAX_STAT_STAGES, CMP_LESS_THAN))
								{
									SetStatChanger(statId, statAmount);
									
									if (gProtectStructs[gBattlerAttacker].notFirstStrike)
										gBattlescriptCurrInstr = BattleScript_MoveStatRaise;
									else
										gBattlescriptCurrInstr = BattleScript_MoveStatRaise_PPLoss;
								}
								else
								{
									if ((gProtectStructs[gBattlerAttacker].notFirstStrike))
										gBattlescriptCurrInstr = BattleScript_MonMadeMoveUseless;
									else
										gBattlescriptCurrInstr = BattleScript_MonMadeMoveUseless_PPLoss;
								}
								break;
							case 3:
								if (gProtectStructs[gBattlerAttacker].notFirstStrike)
									gBattlescriptCurrInstr = BattleScript_FlashFireBoost;
								else
									gBattlescriptCurrInstr = BattleScript_FlashFireBoost_PPLoss;
								
								if (!gDisableStructs[battler].flashFireBoost)
								{
									gDisableStructs[battler].flashFireBoost = TRUE;
									PrepareTypeBuffer(gBattleTextBuff1, moveType);
									gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_FLASH_FIRE_BOOST;
								}
								else
									gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_FLASH_FIRE_DONT_AFFECT;
								break;
						}
					}
				}
				break;
			case ABILITYEFFECT_MOVE_END_ATTACKER: // attacker abilities that activates after makes contact with the target
				switch (ability)
				{
					case ABILITY_STENCH: // Stench check is taken care of in King's Rock check
					    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && RandomPercent(10) && !gProtectStructs[battler].confusionSelfDmg && IsBattlerAlive(gBattlerTarget)
						&& BATTLER_DAMAGED(gBattlerTarget) && !MoveHasMoveEffect(gCurrentMove, MOVE_EFFECT_FLINCH, FALSE))
						{
							SetMoveEffect(MOVE_EFFECT_FLINCH, FALSE, FALSE);
							
							if (DoMoveEffect(FALSE, NULL, 0))
							    ++effect;
						}
						break;
					case ABILITY_POISON_TOUCH:
					    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && RandomPercent(30) && !gProtectStructs[battler].confusionSelfDmg && IsBattlerAlive(gBattlerTarget)
						&& BATTLER_DAMAGED(gBattlerTarget))
						{
							SetMoveEffect(MOVE_EFFECT_POISON, FALSE, FALSE);
							
							if (DoMoveEffect(FALSE, NULL, 0))
							{
								BattleScriptCall(BattleScript_PoisonTouchActivation);
								++effect;
							}
						}
						break;
					case ABILITY_GULP_MISSILE: // Catch prey
					    if ((gCurrentMove == MOVE_SURF && BATTLER_DAMAGED(gBattlerTarget)) || (gStatuses3[battler] & STATUS3_UNDERWATER))
						{
							u16 newSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_HP);
							
							if (newSpecies)
							{
								DoBattleFormChange(battler, newSpecies, FALSE, FALSE, FALSE);
								// Play an silent form change animation if underwater
								gBattleScripting.animArg1 = (gStatuses3[battler] & STATUS3_UNDERWATER) ? B_ANIM_SILENT_FORM_CHANGE : B_ANIM_FORM_CHANGE;
								BattleScriptCall(BattleScript_GulpMissileCatchPrey);
								++effect;
							}
						}
						break;
					case ABILITY_TOXIC_CHAIN:
						if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && RandomPercent(30) && !gProtectStructs[battler].confusionSelfDmg && IsBattlerAlive(gBattlerTarget)
						&& BATTLER_DAMAGED(gBattlerTarget))
						{
							SetMoveEffect(MOVE_EFFECT_TOXIC, FALSE, FALSE);
							
							if (DoMoveEffect(FALSE, NULL, 0))
							{
								BattleScriptCall(BattleScript_ToxicChainActivation);
								++effect;
							}
						}
						break;
					case ABILITY_POISON_PUPPETEER:
					    if (gBattleStruct->poisonPuppeteerConfusion && SpeciesToNationalPokedexNum(gBattleMons[battler].species) == NATIONAL_DEX_PECHARUNT)
						{
						    SetMoveEffect(MOVE_EFFECT_CONFUSION, FALSE, FALSE);
							
					        if (DoMoveEffect(FALSE, NULL, 0))
						    {
						    	gBattleStruct->poisonPuppeteerConfusion = FALSE;
						    	BattleScriptCall(BattleScript_PoisonPuppeteerActivation);
						    	++effect;
						    }
						}
						break;
				}
			    break;
			case ABILITYEFFECT_MOVE_END_TARGET: // target abilities that activates after makes contact with it
			    // all these abilities are negated by Sheer Force, except Illusion and Gulp Missile
			    if (!ReceiveSheerForceBoost(gBattlerAttacker, gCurrentMove) || ability == ABILITY_ILLUSION || ability == ABILITY_GULP_MISSILE)
				{
					switch (ability)
					{
						case ABILITY_ROUGH_SKIN:
						case ABILITY_IRON_BARBS:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(gBattlerAttacker) && IsMoveMakingContact(gBattlerAttacker, gCurrentMove)
						    && GetBattlerAbility(gBattlerAttacker) != ABILITY_MAGIC_GUARD)
							{
								gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 8;
								if (gBattleMoveDamage == 0)
									gBattleMoveDamage = 1;
								BattleScriptCall(BattleScript_RoughSkinActivates);
								++effect;
							}
							break;
						case ABILITY_AFTERMATH:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(gBattlerAttacker) && IsMoveMakingContact(gBattlerAttacker, gCurrentMove)
							&& !IsBattlerAlive(battler) && !ABILITY_ON_FIELD(ABILITY_DAMP) && GetBattlerAbility(gBattlerAttacker) != ABILITY_MAGIC_GUARD)
							{
								gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 4;
								if (gBattleMoveDamage == 0)
									gBattleMoveDamage = 1;
								BattleScriptCall(BattleScript_RoughSkinActivates);
								++effect;
							}
							break;
						case ABILITY_INNARDS_OUT:
						    if (BATTLER_TURN_DAMAGED(battler) && !IsBattlerAlive(battler) && IsBattlerAlive(gBattlerAttacker)
							&& GetBattlerAbility(gBattlerAttacker) != ABILITY_MAGIC_GUARD)
							{
								gBattleMoveDamage = gSpecialStatuses[battler].dmg;
								BattleScriptCall(BattleScript_RoughSkinActivates);
								++effect;
							}
							break;
						case ABILITY_COLOR_CHANGE:
						    if (BATTLER_TURN_DAMAGED(battler) && gCurrentMove != MOVE_STRUGGLE && !IS_MOVE_STATUS(gCurrentMove) && IsBattlerAlive(battler)
							&& !IsBattlerOfType(battler, moveType) && moveType != TYPE_MYSTERY && !SubsBlockMove(gBattlerAttacker, battler, gCurrentMove)
						    && IS_MULTIHIT_FINAL_STRIKE)
							{
								SetBattlerType(battler, moveType);
								BattleScriptCall(BattleScript_ColorChangeActivates);
								++effect;
							}
							break;
						case ABILITY_EFFECT_SPORE:
						    if (IsBattlerAffectedBySpore(gBattlerAttacker))
							{
								i = RandomMax(100);
								
								if (i < 9)
									goto POISON_POINT;
								else if (i < 9 + 10)
									goto STATIC;
								else if (i < 9 + 10 + 11 && BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(gBattlerAttacker) && IsMoveMakingContact(gBattlerAttacker, gCurrentMove)
								&& !SubsBlockMove(gBattlerAttacker, battler, gCurrentMove) && CanBePutToSleep(battler, gBattlerAttacker, STATUS_CHANGE_FLAG_CHECK_UPROAR) == STATUS_CHANGE_WORKED)
								{
									SetMoveEffect(MOVE_EFFECT_SLEEP, TRUE, FALSE);
									BattleScriptCall(BattleScript_ApplySecondaryEffect);
									++effect;
								}
							}
							break;
						case ABILITY_POISON_POINT:
						    if (!RandomPercent(30))
								break;
						
						    POISON_POINT:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(gBattlerAttacker) && IsMoveMakingContact(gBattlerAttacker, gCurrentMove)
							&& !SubsBlockMove(gBattlerAttacker, battler, gCurrentMove) && CanBePoisoned(battler, gBattlerAttacker, 0) == STATUS_CHANGE_WORKED)
							{
								SetMoveEffect(MOVE_EFFECT_POISON, TRUE, FALSE);
								BattleScriptCall(BattleScript_ApplySecondaryEffect);
								++effect;
							}
							break;
						case ABILITY_STATIC:
						    if (!RandomPercent(30))
								break;
						
						    STATIC:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(gBattlerAttacker) && IsMoveMakingContact(gBattlerAttacker, gCurrentMove)
							&& !SubsBlockMove(gBattlerAttacker, battler, gCurrentMove) && CanBeParalyzed(battler, gBattlerAttacker, 0) == STATUS_CHANGE_WORKED)
							{
								SetMoveEffect(MOVE_EFFECT_PARALYSIS, TRUE, FALSE);
								BattleScriptCall(BattleScript_ApplySecondaryEffect);
								++effect;
							}
							break;
						case ABILITY_FLAME_BODY:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(gBattlerAttacker) && IsMoveMakingContact(gBattlerAttacker, gCurrentMove)
							&& !SubsBlockMove(gBattlerAttacker, battler, gCurrentMove) && CanBeBurned(battler, gBattlerAttacker, 0) == STATUS_CHANGE_WORKED
							&& RandomPercent(30))
							{
								SetMoveEffect(MOVE_EFFECT_BURN, TRUE, FALSE);
								BattleScriptCall(BattleScript_ApplySecondaryEffect);
								++effect;
							}
							break;
						case ABILITY_CUTE_CHARM:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(gBattlerAttacker) && IsMoveMakingContact(gBattlerAttacker, gCurrentMove)
							&& RandomPercent(30) && IsBattlerAlive(battler) && CanBeInfatuatedBy(gBattlerAttacker, battler) == STATUS_CHANGE_WORKED)
							{
								gBattleMons[gBattlerAttacker].status2 |= STATUS2_INFATUATION;
								gDisableStructs[gBattlerAttacker].infatuatedWith = battler;
								BattleScriptCall(BattleScript_CuteCharmActivates);
								++effect;
							}
							break;
						case ABILITY_ANGER_POINT:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(battler) && !SubsBlockMove(gBattlerAttacker, battler, gCurrentMove)
							&& gIsCriticalHit && CompareStat(battler, STAT_ATK, MAX_STAT_STAGES, CMP_LESS_THAN))
							{
								SetStatChanger(STAT_ATK, +6); // Max out
								BattleScriptCall(BattleScript_AngerPointActivation);
								++effect;
							}
							break;
						case ABILITY_CURSED_BODY:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(gBattlerAttacker) && !SubsBlockMove(gBattlerAttacker, battler, gCurrentMove)
						    && RandomPercent(30) && TryDisableMove(gBattlerAttacker, gCurrMovePos, gCurrentMove))
							{
								BattleScriptCall(BattleScript_CursedBodyActivation);
								++effect;
							}
							break;
						case ABILITY_WEAK_ARMOR:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(battler) && IS_MOVE_PHYSICAL(gCurrentMove)
							&& (CompareStat(battler, STAT_SPEED, MAX_STAT_STAGES, CMP_LESS_THAN) || CompareStat(battler, STAT_DEF, MIN_STAT_STAGES, CMP_GREATER_THAN)))
							{
								BattleScriptCall(BattleScript_WeakArmorActivation);
								++effect;
							}
							break;
						case ABILITY_ILLUSION:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(battler) && TryRemoveIllusion(battler))
								++effect;
							break;
						case ABILITY_MUMMY:
						case ABILITY_LINGERING_AROMA:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(gBattlerAttacker) && IsMoveMakingContact(gBattlerAttacker, gCurrentMove)
						    && gBattleMons[gBattlerTarget].ability != ABILITY_MUMMY && gBattleMons[gBattlerTarget].ability != ABILITY_LINGERING_AROMA
						    && !gAbilities[gBattleMons[gBattlerTarget].ability].cantBeSuppressed)
							{
								BattleScriptCall(BattleScript_MummyActivates);
								++effect;
							}
							break;
						case ABILITY_THERMAL_EXCHANGE:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(battler) && !SubsBlockMove(gBattlerAttacker, battler, gCurrentMove)
							&& moveType == TYPE_FIRE && CompareStat(battler, STAT_ATK, MAX_STAT_STAGES, CMP_LESS_THAN))
							{
								SetMoveEffect(MOVE_EFFECT_ATK_PLUS_1, TRUE, FALSE);
								BattleScriptCall(BattleScript_TargetAbilityStatRaiseRet);
								++effect;
							}
							break;
						case ABILITY_JUSTIFIED:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(battler) && !SubsBlockMove(gBattlerAttacker, battler, gCurrentMove)
							&& moveType == TYPE_DARK && CompareStat(battler, STAT_ATK, MAX_STAT_STAGES, CMP_LESS_THAN))
							{
								SetMoveEffect(MOVE_EFFECT_ATK_PLUS_1, TRUE, FALSE);
								BattleScriptCall(BattleScript_TargetAbilityStatRaiseRet);
								++effect;
							}
							break;
						case ABILITY_RATTLED:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(battler) && CompareStat(battler, STAT_SPEED, MAX_STAT_STAGES, CMP_LESS_THAN)
							&& (moveType == TYPE_BUG || moveType == TYPE_DARK || moveType == TYPE_GHOST))
							{
								SetMoveEffect(MOVE_EFFECT_SPD_PLUS_1, TRUE, FALSE);
								BattleScriptCall(BattleScript_TargetAbilityStatRaiseRet);
								++effect;
							}
							break;
						case ABILITY_STAMINA:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(battler) && CompareStat(battler, STAT_DEF, MAX_STAT_STAGES, CMP_LESS_THAN))
							{
								SetMoveEffect(MOVE_EFFECT_DEF_PLUS_1, TRUE, FALSE);
								BattleScriptCall(BattleScript_TargetAbilityStatRaiseRet);
								++effect;
							}
							break;
						case ABILITY_WATER_COMPACTION:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(battler) && CompareStat(battler, STAT_DEF, MAX_STAT_STAGES, CMP_LESS_THAN)
							&& moveType == TYPE_WATER)
							{
								SetMoveEffect(MOVE_EFFECT_DEF_PLUS_2, TRUE, FALSE);
								BattleScriptCall(BattleScript_TargetAbilityStatRaiseRet);
								++effect;
							}
							break;
						case ABILITY_BERSERK:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(battler) && HadMoreThanHalfHpNowDoesnt(battler)
							&& CompareStat(battler, STAT_SPATK, MAX_STAT_STAGES, CMP_LESS_THAN) && IS_MULTIHIT_FINAL_STRIKE)
							{
								SetMoveEffect(MOVE_EFFECT_SP_ATK_PLUS_1, TRUE, FALSE);
								BattleScriptCall(BattleScript_TargetAbilityStatRaiseRet);
								++effect;
							}
							break;
						case ABILITY_GOOEY:
						case ABILITY_TANGLING_HAIR:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(gBattlerAttacker) && IsMoveMakingContact(gBattlerAttacker, gCurrentMove)
						    && CompareStat(gBattlerAttacker, STAT_SPEED, MIN_STAT_STAGES, CMP_GREATER_THAN))
							{
								SetStatChanger(STAT_SPEED, -1);
								BattleScriptCall(BattleScript_GooeyActivates);
								++effect;
							}
							break;
						case ABILITY_SAND_SPIT:
						    if (BATTLER_TURN_DAMAGED(battler) && TryChangeBattleWeather(battler, ENUM_WEATHER_SANDSTORM))
							{
								BattleScriptCall(BattleScript_SandSpitActivated);
								++effect;
							}
							break;
						case ABILITY_PERISH_BODY:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(battler) && IsMoveMakingContact(gBattlerAttacker, gCurrentMove)
							&& !(gStatuses3[gBattlerAttacker] & STATUS3_PERISH_SONG))
							{
								if (!(gStatuses3[battler] & STATUS3_PERISH_SONG))
								{
									gStatuses3[battler] |= STATUS3_PERISH_SONG;
									gDisableStructs[battler].perishSongTimer = 3;
								}
								gStatuses3[gBattlerAttacker] |= STATUS3_PERISH_SONG;
								gDisableStructs[gBattlerAttacker].perishSongTimer = 3;
								BattleScriptCall(BattleScript_PerishBodyActivates);
								++effect;
							}
							break;
						case ABILITY_WANDERING_SPIRIT:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(gBattlerAttacker) && IsMoveMakingContact(gBattlerAttacker, gCurrentMove)
							&& !gAbilities[gBattleMons[gBattlerAttacker].ability].cantBeSwapped)
							{
								BattleScriptCall(BattleScript_WanderingSpiritActivates);
								++effect;
							}
							break;
						case ABILITY_WIND_POWER:
						    if (gBattleMoves[gCurrentMove].flags.windMove && gBattleMoves[gCurrentMove].target != MOVE_TARGET_USER
							&& gBattleMoves[gCurrentMove].target != MOVE_TARGET_ALL_BATTLERS)
							{
								gBattleScripting.battler = battler;
								BattleScriptCall(BattleScript_WindPowerActivates);
								++effect;
							}
							break;
						case ABILITY_ELECTROMORPHOSIS:
						    if (BATTLER_TURN_DAMAGED(battler))
							{
								gBattleScripting.battler = battler;
								BattleScriptCall(BattleScript_WindPowerActivates);
								++effect;
							}
							break;
						case ABILITY_COTTON_DOWN:
						    if (BATTLER_TURN_DAMAGED(battler))
							{
								BattleScriptCall(BattleScript_CottonDownActivates);
								++effect;
							}
							break;
						case ABILITY_GULP_MISSILE:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(gBattlerAttacker))
							{
								if (gBattleMons[battler].species == SPECIES_CRAMORANT_GULPING) // Arrokuda
								    effect = MOVE_EFFECT_DEF_MINUS_1;
								else if (gBattleMons[battler].species == SPECIES_CRAMORANT_GORGING) // Pikachu
									effect = MOVE_EFFECT_PARALYSIS;
								
								if (effect)
								{
									SetMoveEffect(effect, TRUE, FALSE);
									gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 4;
									if (gBattleMoveDamage == 0)
										gBattleMoveDamage = 1;
									DoBattleFormChange(battler, SPECIES_CRAMORANT, FALSE, FALSE, FALSE);
									gBattleScripting.animArg2 = gBattlerAttacker;
									BattleScriptCall(BattleScript_GulpMissileSpitUpPrey);
								}
							}
							break;
						case ABILITY_ANGER_SHELL:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(battler) && HadMoreThanHalfHpNowDoesnt(battler))
							{
								BattleScriptCall(BattleScript_AngerShellActivates);
								++effect;
							}
							break;
						case ABILITY_STEAM_ENGINE:
						    if (BATTLER_TURN_DAMAGED(battler) && IsBattlerAlive(battler) && CompareStat(battler, STAT_SPEED, MAX_STAT_STAGES, CMP_LESS_THAN)
							&& (moveType == TYPE_FIRE || moveType == TYPE_WATER))
							{
								SetStatChanger(STAT_SPEED, +6); // Max out
								BattleScriptCall(BattleScript_AngerPointActivation);
								++effect;
							}
							break;
						case ABILITY_TOXIC_DEBRIS:
						    if (BATTLER_TURN_DAMAGED(battler) && IS_MOVE_PHYSICAL(gCurrentMove) && !SubsBlockMove(gBattlerAttacker, battler, gCurrentMove)
							&& TrySetToxicSpikesOnBattlerSide(BATTLE_OPPOSITE(battler)))
							{
								BattleScriptCall(BattleScript_ToxicDebrisActivation);
								++effect;
							}
							break;
					}
				}
				break;
			case ABILITYEFFECT_IMMUNITY:
			    for (battler = 0; battler < gBattlersCount; battler++)
				{
					switch (GetBattlerAbility(battler))
					{
						case ABILITY_IMMUNITY:
						case ABILITY_PASTEL_VEIL:
						    if (gBattleMons[battler].status1.id == STATUS1_POISON || gBattleMons[battler].status1.id == STATUS1_TOXIC_POISON)
								effect = 1;
							break;
						case ABILITY_LIMBER:
						    if (gBattleMons[battler].status1.id == STATUS1_PARALYSIS)
								effect = 1;
							break;
						case ABILITY_INSOMNIA:
						case ABILITY_VITAL_SPIRIT:
						    if (gBattleMons[battler].status1.id == STATUS1_SLEEP)
								effect = 1;
							break;
						case ABILITY_WATER_VEIL:
						case ABILITY_WATER_BUBBLE:
						case ABILITY_THERMAL_EXCHANGE:
						    if (gBattleMons[battler].status1.id == STATUS1_BURN)
								effect = 1;
							break;
						case ABILITY_MAGMA_ARMOR:
						    if (gBattleMons[battler].status1.id == STATUS1_FREEZE)
								effect = 1;
							break;
						case ABILITY_OBLIVIOUS:
						    if (gBattleMons[battler].status2 & STATUS2_INFATUATION || gDisableStructs[battler].tauntTimer)
								effect = 2;
							break;
						case ABILITY_OWN_TEMPO:
						    if (gBattleMons[battler].status2 & STATUS2_CONFUSION)
								effect = 3;
							break;
					}
					if (effect)
					{
						switch (effect)
						{
							case 1:
							    ClearBattlerStatus(battler);
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CURED_ITS_PROBLEM;
								break;
							case 2:
							    gBattleMons[battler].status2 &= ~(STATUS2_INFATUATION);
								gDisableStructs[battler].tauntTimer = 0;
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CURED_MENTAL_STATE;
								break;
							case 3:
							    gBattleMons[battler].status2 &= ~(STATUS2_CONFUSION);
								gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CURED_CONFUSION;
								break;
						}
						gBattleScripting.battler = battler;
						BattleScriptCall(BattleScript_AbilityCuredStatus);
						break;
					}
				}
				break;
			case ABILITYEFFECT_ON_WEATHER: // abilities that activate when the weather changes
			    gBattleScripting.battler = battler;
			
			    switch (ability)
				{
					case ABILITY_ICE_FACE:
					    if (gBattleStruct->sides[GetBattlerSide(battler)].party[gBattlerPartyIndexes[battler]].allowedToChangeFormInWeather)
						{
							u16 newSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_WEATHER);
							
							if (newSpecies)
							{
								gBattleStruct->sides[GetBattlerSide(battler)].party[gBattlerPartyIndexes[battler]].allowedToChangeFormInWeather = FALSE;
								DoBattleFormChange(battler, newSpecies, FALSE, TRUE, FALSE);
								BattleScriptPushCursorAndCallback(BattleScript_CastformChange);
								++effect;
							}
						}
						break;
				}
				// Not only related to abilities
				// Reverts Castform and Cherrim when ability changed or suppressed
				if (!effect)
				{
					bool8 reloadTypes = FALSE, reloadStats = FALSE;
					u16 newSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_WEATHER);
					
					if (newSpecies)
					{
						switch (SpeciesToNationalPokedexNum(newSpecies))
						{
							case NATIONAL_DEX_CASTFORM:
							    reloadTypes = TRUE;
								break;
							case NATIONAL_DEX_CHERRIM:
							    reloadStats = TRUE;
								break;
						}
						DoBattleFormChange(battler, newSpecies, reloadTypes, reloadStats, FALSE);
						BattleScriptPushCursorAndCallback(BattleScript_CastformChange);
						++effect;
					}
				}
				gSpecialStatuses[battler].removedWeatherChangeAbility = FALSE;
				break;
			case ABILITYEFFECT_SYNCHRONIZE:
			    if (ability == ABILITY_SYNCHRONIZE && (gHitMarker & HITMARKER_SYNCHRONISE_EFFECT))
				{
					gHitMarker &= ~(HITMARKER_SYNCHRONISE_EFFECT);
					SetMoveEffect(gBattleStruct->synchronizeMoveEffect, FALSE, FALSE);
					gBattleScripting.battler = battler;
					BattleScriptCall(BattleScript_SynchronizeActivates);
					++effect;
				}
				break;
			case ABILITYEFFECT_ON_TERRAIN: // abilities that activate when the terrain changes
			    gBattleScripting.battler = battler;
				break;
		}
	}
	return effect;
}

enum
{
    ITEM_NO_EFFECT,
    ITEM_STATUS_CHANGE,
    ITEM_EFFECT_OTHER,
    ITEM_PP_CHANGE,
    ITEM_HP_CHANGE,
    ITEM_STATS_CHANGE,
};

static u8 ConfusionBerries(u8 battlerId, u8 flavor, bool8 moveTurn)
{
    u8 effect = 0;
    
    if (CheckPinchBerryActivate(battlerId, gLastUsedItem) && !moveTurn)
    {
        PrepareFlavorBuffer(gBattleTextBuff1, flavor);
		
		gBattleMoveDamage = gBattleMons[battlerId].maxHP / 3;
		if (gBattleMoveDamage == 0)
			gBattleMoveDamage = 1;
		
		if (gBattleMons[battlerId].hp + gBattleMoveDamage > gBattleMons[battlerId].maxHP)
			gBattleMoveDamage = gBattleMons[battlerId].maxHP - gBattleMons[battlerId].hp;
		gBattleMoveDamage *= -1;
		
		if (GetMonFlavorRelation(GetBattlerPartyIndexPtr(battlerId), flavor) < 0)
			BattleScriptExecute(BattleScript_BerryConfuseHealEnd2);
		else
			BattleScriptExecute(BattleScript_ItemHealHP_RemoveItem);
		
		effect = ITEM_HP_CHANGE;
    }
    return effect;
}

static u8 StatRaiseBerries(u8 battlerId, u8 statId, bool8 moveTurn)
{
    u8 effect = 0;

    if (CheckPinchBerryActivate(battlerId, gLastUsedItem) && !moveTurn && CompareStat(battlerId, statId, MAX_STAT_STAGES, CMP_LESS_THAN))
    {
        PrepareStatBuffer(gBattleTextBuff1, statId);
		// PrepareStringBuffer(gBattleTextBuff2, STRINGID_STATROSE);
		gEffectBattler = battlerId;
		SetStatChanger(statId, +1);
		gBattleScripting.animArg1 = 0xE + statId;
		gBattleScripting.animArg2 = 0;
		BattleScriptExecute(BattleScript_BerryStatRaiseEnd2);
		effect = ITEM_STATS_CHANGE;
    }
    return effect;
}

u8 ItemBattleEffects(u8 caseID, u8 battlerId, bool8 moveTurn)
{
    int i = 0;
    u8 effect = ITEM_NO_EFFECT;
    u8 changedPP = 0;
    u8 battlerHoldEffect = GetBattlerItemHoldEffect(battlerId, TRUE), defHoldEffect;
    u8 defHoldEffectParam;
    u16 defItem, battlerHoldEffectParam;

    gLastUsedItem = gBattleMons[battlerId].item;
	
    if (IsUnnerveOnOpposingField(battlerId) && ItemId_GetPocket(gLastUsedItem) == POCKET_BERRY_POUCH)
    {
	    battlerHoldEffect = 0;
	    battlerHoldEffectParam = 0;
    }
	else
		battlerHoldEffectParam = ItemId_GetHoldEffectParam(gLastUsedItem);
	
    switch (caseID)
    {
    case ITEMEFFECT_ON_SWITCH_IN:
        if (GetBattlerItemHoldEffect(battlerId, FALSE) == HOLD_EFFECT_DOUBLE_PRIZE)
            gBattleStruct->moneyMultiplier = TRUE;
        else if (battlerHoldEffect == HOLD_EFFECT_RESTORE_STATS)
	{
		for (i = 0; i < NUM_BATTLE_STATS; ++i)
		{
			if (gBattleMons[battlerId].statStages[i] < DEFAULT_STAT_STAGES)
			{
				gBattleMons[battlerId].statStages[i] = DEFAULT_STAT_STAGES;
				effect = ITEM_STATS_CHANGE;
			}
		}
	}
        if (effect)
            {
                gBattleScripting.battler = battlerId;
                gBattlerAttacker = battlerId;
                BattleScriptExecute(BattleScript_WhiteHerbEnd2);
            }
            break;
    case 1:
        if (IsBattlerAlive(battlerId))
        {
            switch (battlerHoldEffect)
            {
            case HOLD_EFFECT_RESTORE_HP:
                if (gBattleMons[battlerId].hp <= gBattleMons[battlerId].maxHP / 2 && !moveTurn)
                {
                    gBattleMoveDamage = battlerHoldEffectParam;
                    if (gBattleMons[battlerId].hp + battlerHoldEffectParam > gBattleMons[battlerId].maxHP)
                        gBattleMoveDamage = gBattleMons[battlerId].maxHP - gBattleMons[battlerId].hp;
                    gBattleMoveDamage *= -1;
                    BattleScriptExecute(BattleScript_ItemHealHP_RemoveItem);
                    effect = 4;
                }
                break;
			case HOLD_EFFECT_RESTORE_HP_PERCENTAGE:
			    if (gBattleMons[battlerId].hp <= gBattleMons[battlerId].maxHP / 2 && !moveTurn)
				{
					gBattleMoveDamage = (gBattleMons[battlerId].maxHP * battlerHoldEffectParam) / 100;
					gBattleMoveDamage *= -1;
                    BattleScriptExecute(BattleScript_ItemHealHP_RemoveItem);
                    effect = 4;
				}
				break;
            case HOLD_EFFECT_RESTORE_PP:
                if (!moveTurn)
                {
                    struct Pokemon *mon = GetBattlerPartyIndexPtr(battlerId);
                    u8 ppBonuses;
                    u16 move;

                    for (i = 0; i < MAX_MON_MOVES; ++i)
                    {
                        move = GetMonData(mon, MON_DATA_MOVE1 + i);
                        changedPP = GetMonData(mon, MON_DATA_PP1 + i);
                        ppBonuses = GetMonData(mon, MON_DATA_PP_BONUSES);
                        if (move && changedPP == 0)
                            break;
                    }
                    if (i != MAX_MON_MOVES)
                    {
                        u8 maxPP = CalculatePPWithBonus(move, ppBonuses, i);
                        if (changedPP + battlerHoldEffectParam > maxPP)
                            changedPP = maxPP;
                        else
                            changedPP = changedPP + battlerHoldEffectParam;
                        PrepareMoveBuffer(gBattleTextBuff1, move);
                        BattleScriptExecute(BattleScript_BerryPPHealEnd2);
                        BtlController_EmitSetMonData(battlerId, BUFFER_A, i + REQUEST_PPMOVE1_BATTLE, 0, sizeof(gBattleMons[battlerId].pp[i]), &changedPP);
                        MarkBattlerForControllerExec(battlerId);
                        effect = ITEM_PP_CHANGE;
                    }
                }
                break;
            case HOLD_EFFECT_RESTORE_STATS:
                for (i = 0; i < NUM_BATTLE_STATS; ++i)
                {
                    if (gBattleMons[battlerId].statStages[i] < DEFAULT_STAT_STAGES)
                    {
                        gBattleMons[battlerId].statStages[i] = DEFAULT_STAT_STAGES;
                        effect = ITEM_STATS_CHANGE;
                    }
                }
                if (effect)
                {
                    gBattleScripting.battler = battlerId;
                    gBattlerAttacker = battlerId;
                    BattleScriptExecute(BattleScript_WhiteHerbEnd2);
                }
                break;
            case HOLD_EFFECT_LEFTOVERS:
                if (!BATTLER_MAX_HP(battlerId) && !moveTurn)
                {
                    gBattleMoveDamage = gBattleMons[battlerId].maxHP / battlerHoldEffectParam;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = 1;
                    if (gBattleMons[battlerId].hp + gBattleMoveDamage > gBattleMons[battlerId].maxHP)
                        gBattleMoveDamage = gBattleMons[battlerId].maxHP - gBattleMons[battlerId].hp;
                    gBattleMoveDamage *= -1;
                    BattleScriptExecute(BattleScript_ItemHealHP_End2);
                    effect = ITEM_HP_CHANGE;
                    // RecordItemEffectBattle(battlerId, battlerHoldEffect);
                }
                break;
            case HOLD_EFFECT_CONFUSE_FLAVOR:
			    effect = ConfusionBerries(battlerId, battlerHoldEffectParam, moveTurn);
				break;
            case HOLD_EFFECT_STAT_UP:
	            effect = StatRaiseBerries(battlerId, battlerHoldEffectParam, moveTurn);
				break;
            case HOLD_EFFECT_CRITICAL_UP:
                if (CheckPinchBerryActivate(battlerId, gLastUsedItem) && !moveTurn && !(gBattleMons[battlerId].status2 & STATUS2_FOCUS_ENERGY))
                {
                    gBattleMons[battlerId].status2 |= STATUS2_FOCUS_ENERGY;
                    BattleScriptExecute(BattleScript_BerryFocusEnergyEnd2);
                    effect = ITEM_EFFECT_OTHER;
                }
                break;
            case HOLD_EFFECT_RANDOM_STAT_UP:
                if (!moveTurn && CheckPinchBerryActivate(battlerId, gLastUsedItem))
                {
                    for (i = 0; i < 5 && !CompareStat(battlerId, STAT_ATK + i, MAX_STAT_STAGES, CMP_LESS_THAN); ++i);
                    if (i != 5)
                    {
                        do
                            i = RandomMax(NUM_STATS - 1);
                        while (!CompareStat(battlerId, STAT_ATK + i, MAX_STAT_STAGES, CMP_LESS_THAN));
                        PrepareStatBuffer(gBattleTextBuff1, i + 1);
                        gBattleTextBuff2[0] = B_BUFF_PLACEHOLDER_BEGIN;
                        gBattleTextBuff2[1] = B_BUFF_STRING;
                        // gBattleTextBuff2[2] = STRINGID_STATSHARPLY;
                        // gBattleTextBuff2[3] = STRINGID_STATSHARPLY >> 8;
                        gBattleTextBuff2[4] = B_BUFF_STRING;
                        // gBattleTextBuff2[5] = STRINGID_STATROSE;
                        // gBattleTextBuff2[6] = STRINGID_STATROSE >> 8;
                        gBattleTextBuff2[7] = EOS;
                        gEffectBattler = battlerId;
                        SetStatChanger(i + 1, +2);
                        gBattleScripting.animArg1 = 0x21 + i + 6;
                        gBattleScripting.animArg2 = 0;
                        BattleScriptExecute(BattleScript_BerryStatRaiseEnd2);
                        effect = ITEM_STATS_CHANGE;
                    }
                }
                break;
            case HOLD_EFFECT_CURE_PAR:
                if (gBattleMons[battlerId].status1.id == STATUS1_PARALYSIS)
                {
                    gBattleMons[battlerId].status1.id = 0;
                    BattleScriptExecute(BattleScript_BerryCurePrlzEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_PSN:
                /*if (gBattleMons[battlerId].status1 & STATUS1_PSN_ANY)
                {
                    gBattleMons[battlerId].status1 &= ~(STATUS1_PSN_ANY | STATUS1_TOXIC_COUNTER);
                    BattleScriptExecute(BattleScript_BerryCurePsnEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }*/
                break;
            case HOLD_EFFECT_CURE_BRN:
                /*if (gBattleMons[battlerId].status1 & STATUS1_BURN)
                {
                    gBattleMons[battlerId].status1 &= ~(STATUS1_BURN);
                    BattleScriptExecute(BattleScript_BerryCureBrnEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }*/
                break;
            case HOLD_EFFECT_CURE_FRZ:
                /*if (gBattleMons[battlerId].status1 & STATUS1_FREEZE)
                {
                    gBattleMons[battlerId].status1 &= ~(STATUS1_FREEZE);
                    BattleScriptExecute(BattleScript_BerryCureFrzEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }*/
                break;
            case HOLD_EFFECT_CURE_SLP:
                /*if (gBattleMons[battlerId].status1 & STATUS1_SLEEP)
                {
                    gBattleMons[battlerId].status1 &= ~(STATUS1_SLEEP);
                    gBattleMons[battlerId].status2 &= ~(STATUS2_NIGHTMARE);
                    BattleScriptExecute(BattleScript_BerryCureSlpEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }*/
                break;
            case HOLD_EFFECT_CURE_CONFUSION:
                if (gBattleMons[battlerId].status2 & STATUS2_CONFUSION)
                {
                    gBattleMons[battlerId].status2 &= ~(STATUS2_CONFUSION);
                    BattleScriptExecute(BattleScript_BerryCureConfusionEnd2);
                    effect = ITEM_EFFECT_OTHER;
                }
                break;
            case HOLD_EFFECT_CURE_STATUS:
                /*if (gBattleMons[battlerId].status1 & STATUS1_ANY || gBattleMons[battlerId].status2 & STATUS2_CONFUSION)
                {
                    i = 0;
                    if (gBattleMons[battlerId].status1 & STATUS1_PSN_ANY)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_PoisonJpn);
                        ++i;
                    }
                    if (gBattleMons[battlerId].status1 & STATUS1_SLEEP)
                    {
                        gBattleMons[battlerId].status2 &= ~(STATUS2_NIGHTMARE);
                        StringCopy(gBattleTextBuff1, gStatusConditionString_SleepJpn);
                        ++i;
                    }
                    if (gBattleMons[battlerId].status1 & STATUS1_PARALYSIS)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_ParalysisJpn);
                        ++i;
                    }
                    if (gBattleMons[battlerId].status1 & STATUS1_BURN)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_BurnJpn);
                        ++i;
                    }
                    if (gBattleMons[battlerId].status1 & STATUS1_FREEZE)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_IceJpn);
                        ++i;
                    }
                    if (gBattleMons[battlerId].status2 & STATUS2_CONFUSION)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_ConfusionJpn);
                        ++i;
                    }
                    if (!(i > 1))
                        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                    else
                        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                    gBattleMons[battlerId].status1 = 0;
                    gBattleMons[battlerId].status2 &= ~(STATUS2_CONFUSION);
                    BattleScriptExecute(BattleScript_BerryCureChosenStatusEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }*/
                break;
            case HOLD_EFFECT_MENTAL_HERB:
                if (gBattleMons[battlerId].status2 & STATUS2_INFATUATION)
                {
                    gBattleMons[battlerId].status2 &= ~(STATUS2_INFATUATION);
                    // StringCopy(gBattleTextBuff1, gStatusConditionString_LoveJpn);
                    BattleScriptExecute(BattleScript_BerryCureChosenStatusEnd2);
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                    effect = ITEM_EFFECT_OTHER;
                }
                break;
            }
            if (effect)
            {
                gBattleScripting.battler = battlerId;
                gBattlerAttacker = battlerId;
                switch (effect)
                {
                case ITEM_STATUS_CHANGE:
                    BtlController_EmitSetMonData(battlerId, BUFFER_A, REQUEST_STATUS_BATTLE, 0, sizeof(gBattleMons[battlerId].status1), &gBattleMons[battlerId].status1);
                    MarkBattlerForControllerExec(battlerId);
                    break;
                case ITEM_PP_CHANGE:
                    if (MOVE_IS_PERMANENT(battlerId, i))
                        gBattleMons[battlerId].pp[i] = changedPP;
                    break;
                }
            }
        }
        break;
    case 2:
        break;
    case ITEMEFFECT_MOVE_END:
        for (battlerId = 0; battlerId < gBattlersCount; ++battlerId)
        {
            gLastUsedItem = gBattleMons[battlerId].item;
			battlerHoldEffect = GetBattlerItemHoldEffect(battlerId, TRUE);
            battlerHoldEffectParam = ItemId_GetHoldEffectParam(gLastUsedItem);
			
            switch (battlerHoldEffect)
            {
            case HOLD_EFFECT_CURE_PAR:
                /*if (gBattleMons[battlerId].status1 & STATUS1_PARALYSIS)
                {
                    gBattleMons[battlerId].status1 &= ~(STATUS1_PARALYSIS);
					BattleScriptCall(BattleScript_BerryCureParRet);
                    effect = ITEM_STATUS_CHANGE;
                }*/
                break;
            case HOLD_EFFECT_CURE_PSN:
                /*if (gBattleMons[battlerId].status1 & STATUS1_PSN_ANY)
                {
                    gBattleMons[battlerId].status1 &= ~(STATUS1_PSN_ANY | STATUS1_TOXIC_COUNTER);
					BattleScriptCall(BattleScript_BerryCurePsnRet);
                    effect = ITEM_STATUS_CHANGE;
                }*/
                break;
            case HOLD_EFFECT_CURE_BRN:
                /*if (gBattleMons[battlerId].status1 & STATUS1_BURN)
                {
                    gBattleMons[battlerId].status1 &= ~(STATUS1_BURN);
					BattleScriptCall(BattleScript_BerryCureBrnRet);
                    effect = ITEM_STATUS_CHANGE;
                }*/
                break;
            case HOLD_EFFECT_CURE_FRZ:
                /*if (gBattleMons[battlerId].status1 & STATUS1_FREEZE)
                {
                    gBattleMons[battlerId].status1 &= ~(STATUS1_FREEZE);
					BattleScriptCall(BattleScript_BerryCureFrzRet);
                    effect = ITEM_STATUS_CHANGE;
                }*/
                break;
            case HOLD_EFFECT_CURE_SLP:
                /*if (gBattleMons[battlerId].status1 & STATUS1_SLEEP)
                {
                    gBattleMons[battlerId].status1 &= ~(STATUS1_SLEEP);
                    gBattleMons[battlerId].status2 &= ~(STATUS2_NIGHTMARE);
					BattleScriptCall(BattleScript_BerryCureSlpRet);
                    effect = ITEM_STATUS_CHANGE;
                }*/
                break;
            case HOLD_EFFECT_CURE_CONFUSION:
                if (gBattleMons[battlerId].status2 & STATUS2_CONFUSION)
                {
                    gBattleMons[battlerId].status2 &= ~(STATUS2_CONFUSION);
					BattleScriptCall(BattleScript_BerryCureConfusionRet);
                    effect = ITEM_EFFECT_OTHER;
                }
                break;
            case HOLD_EFFECT_MENTAL_HERB:
                if (gBattleMons[battlerId].status2 & STATUS2_INFATUATION)
                {
                    gBattleMons[battlerId].status2 &= ~(STATUS2_INFATUATION);
                    // StringCopy(gBattleTextBuff1, gStatusConditionString_LoveJpn);
					BattleScriptCall(BattleScript_BerryCureChosenStatusRet);
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                    effect = ITEM_EFFECT_OTHER;
                }
                break;
            case HOLD_EFFECT_CURE_STATUS:
                /*if (gBattleMons[battlerId].status1 & STATUS1_ANY || gBattleMons[battlerId].status2 & STATUS2_CONFUSION)
                {
                    if (gBattleMons[battlerId].status1 & STATUS1_PSN_ANY)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_PoisonJpn);
                    }
                    if (gBattleMons[battlerId].status1 & STATUS1_SLEEP)
                    {
                        gBattleMons[battlerId].status2 &= ~(STATUS2_NIGHTMARE);
                        StringCopy(gBattleTextBuff1, gStatusConditionString_SleepJpn);
                    }
                    if (gBattleMons[battlerId].status1 & STATUS1_PARALYSIS)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_ParalysisJpn);
                    }
                    if (gBattleMons[battlerId].status1 & STATUS1_BURN)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_BurnJpn);
                    }
                    if (gBattleMons[battlerId].status1 & STATUS1_FREEZE)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_IceJpn);
                    }
                    if (gBattleMons[battlerId].status2 & STATUS2_CONFUSION)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_ConfusionJpn);
                    }
                    gBattleMons[battlerId].status1 = 0;
                    gBattleMons[battlerId].status2 &= ~(STATUS2_CONFUSION);
					BattleScriptCall(BattleScript_BerryCureChosenStatusRet);
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                    effect = ITEM_STATUS_CHANGE;
                }*/
                break;
            case HOLD_EFFECT_RESTORE_STATS:
                for (i = 0; i < NUM_BATTLE_STATS; ++i)
                {
                    if (gBattleMons[battlerId].statStages[i] < DEFAULT_STAT_STAGES)
                    {
                        gBattleMons[battlerId].statStages[i] = DEFAULT_STAT_STAGES;
                        effect = ITEM_STATS_CHANGE;
                    }
                }
                if (effect)
                {
                    gBattleScripting.battler = battlerId;
					BattleScriptCall(BattleScript_WhiteHerbRet);
                    return effect;
                }
                break;
            }
            if (effect)
            {
                gBattleScripting.battler = battlerId;
                BtlController_EmitSetMonData(battlerId, BUFFER_A, REQUEST_STATUS_BATTLE, 0, sizeof(gBattleMons[battlerId].status1), &gBattleMons[battlerId].status1);
                MarkBattlerForControllerExec(battlerId);
                break;
            }
        }
        break;
    case ITEMEFFECT_KINGSROCK_SHELLBELL:
        if (gBattleMoveDamage && !ReceiveSheerForceBoost(battlerId, gCurrentMove))
        {
            switch (battlerHoldEffect)
            {
            case HOLD_EFFECT_FLINCH:
                if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
                 && BATTLER_DAMAGED(gBattlerTarget)
                 && gBattleMoves[gCurrentMove].flags.kingsRockAffected
                 && IsBattlerAlive(gBattlerTarget)
				 && GetBattlerAbility(battlerId) != ABILITY_STENCH
				 && RandomPercent(CalcSecondaryEffectChance(battlerId, MOVE_EFFECT_FLINCH, battlerHoldEffectParam)))
                {
					SetMoveEffect(MOVE_EFFECT_FLINCH, FALSE, FALSE);
					DoMoveEffect(FALSE, NULL, 0);
                }
                break;
            case HOLD_EFFECT_SHELL_BELL:
                if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
                 && gSpecialStatuses[gBattlerTarget].dmg != 0
                 && gSpecialStatuses[gBattlerTarget].dmg != 0xFFFF
                 && battlerId != gBattlerTarget
                 && !BATTLER_MAX_HP(battlerId)
                 && IsBattlerAlive(battlerId))
                {
                    gBattleScripting.battler = battlerId;
                    gBattleMoveDamage = (gSpecialStatuses[gBattlerTarget].dmg / battlerHoldEffectParam) * -1;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = -1;
                    gSpecialStatuses[gBattlerTarget].dmg = 0;
					BattleScriptCall(BattleScript_ItemHealHP_Ret);
                    ++effect;
                }
                break;
            }
        }
        break;
    }
    return effect;
}

bool8 CheckPinchBerryActivate(u8 battler, u16 item)
{
    if (IsUnnerveOnOpposingField(battler) && ItemId_GetPocket(item) == POCKET_BERRY_POUCH)
		return FALSE;
    if (gBattleMons[battler].hp <= gBattleMons[battler].maxHP / 4 || (GetBattlerAbility(battler) == ABILITY_GLUTTONY && gBattleMons[battler].hp <= gBattleMons[battler].maxHP / 2))
        return TRUE;
    return FALSE;
}

void ClearFuryCutterDestinyBondGrudge(u8 battlerId)
{
    gDisableStructs[battlerId].furyCutterCounter = 0;
    gBattleMons[battlerId].status2 &= ~(STATUS2_DESTINY_BOND);
    gStatuses3[battlerId] &= ~(STATUS3_GRUDGE);
}

void HandleAction_RunBattleScript(void) // identical to RunBattleScriptCommands
{
    if (!gBattleControllerExecFlags)
        gBattleScriptingCommandsTable[*gBattlescriptCurrInstr]();
}

u8 GetMoveSplit(u16 move)
{
	u8 split = gBattleMoves[move].split;
	
	// add moves that can change its split.
	
	return split;
}

u8 GetDefaultMoveTarget(u8 battlerId)
{
    u8 opposing = BATTLE_OPPOSITE(battlerId);

    if (!IsDoubleBattleForBattler(opposing))
        return GetBattlerAtPosition(opposing);
    else if (CountAliveMonsInBattle(battlerId, BATTLE_ALIVE_EXCEPT_BATTLER) > 1)
        return GetBattlerAtPosition(RandomMax(2) ? opposing : BATTLE_PARTNER(opposing));
    else
		return GetBattlerAtPosition((gAbsentBattlerFlags & gBitTable[opposing]) ? BATTLE_PARTNER(opposing) : opposing);
}

u8 GetRandomTarget(u8 battlerId)
{
	return GetBattlerAtPosition(sTargetPositions[GetBattlerSide(battlerId)][RandomMax(2)]);
}

// Get move base target type
u8 GetBattlerMoveTargetType(u8 battlerId, u16 move)
{
	u8 target = gBattleMoves[move].target;
	
	switch (gBattleMoves[move].effect)
	{
		case EFFECT_CURSE:
		    if (!IsBattlerOfType(battlerId, TYPE_GHOST))
				target = MOVE_TARGET_USER;
			break;
	}
	return target;
}

u8 SetRandomTarget(u8 battlerId)
{
	u8 target;
	
	if (IsDoubleBattleForBattler(BATTLE_OPPOSITE(battlerId)))
	{
		target = GetRandomTarget(battlerId);
		
		if (!IsBattlerAlive(target))
			target = BATTLE_PARTNER(target);
	}
	else
		target = GetBattlerAtPosition(sTargetPositions[GetBattlerSide(battlerId)][0]);
	
	return target;
}

// Get move target and apply all redirection effects
u8 GetMoveTarget(u16 move, u8 setTarget)
{
	u8 side, moveTarget, targetBattler = 0;
	
	if (setTarget)
		moveTarget = setTarget - 1;
	else
		moveTarget = GetBattlerMoveTargetType(gBattlerAttacker, move);
	
	switch (moveTarget)
	{
		case MOVE_TARGET_SELECTED:
		case MOVE_TARGET_SELECTED_OPPONENT:
		    side = GetBattlerSide(BATTLE_OPPOSITE(gBattlerAttacker));
			
			if (IsBattlerAffectedByFollowMe(gBattlerAttacker, side, move))
				targetBattler = gSideTimers[side].followmeTarget;
			else
			{
				targetBattler = SetRandomTarget(gBattlerAttacker);
				
				if (gBattleMoves[move].type == TYPE_ELECTRIC && ABILITY_ON_OPPOSING_SIDE(gBattlerAttacker, ABILITY_LIGHTNING_ROD)
				&& GetBattlerAbility(targetBattler) != ABILITY_LIGHTNING_ROD)
				{
					targetBattler = BATTLE_PARTNER(targetBattler);
					gSpecialStatuses[targetBattler].abilityRedirected = TRUE;
				}
				else if (gBattleMoves[move].type == TYPE_WATER && ABILITY_ON_OPPOSING_SIDE(gBattlerAttacker, ABILITY_STORM_DRAIN)
				&& GetBattlerAbility(targetBattler) != ABILITY_STORM_DRAIN)
				{
					targetBattler = BATTLE_PARTNER(targetBattler);
					gSpecialStatuses[targetBattler].abilityRedirected = TRUE;
				}
			}
			break;
		case MOVE_TARGET_DEPENDS:
		case MOVE_TARGET_BOTH:
		case MOVE_TARGET_OPPONENTS:
		case MOVE_TARGET_FOES_AND_ALLY:
		case MOVE_TARGET_OPPONENTS_FIELD:
			targetBattler = BATTLE_OPPOSITE(gBattlerAttacker);
			
			if (!IsBattlerAlive(targetBattler))
				targetBattler = BATTLE_PARTNER(targetBattler);
			break;
		case MOVE_TARGET_RANDOM:
			side = GetBattlerSide(BATTLE_OPPOSITE(gBattlerAttacker));
			
			if (IsBattlerAffectedByFollowMe(gBattlerAttacker, side, move))
				targetBattler = gSideTimers[side].followmeTarget;
			else
				targetBattler = SetRandomTarget(gBattlerAttacker);
			break;
		case MOVE_TARGET_ALLY:
		    targetBattler = BATTLE_PARTNER(gBattlerAttacker);
			break;
		default:
			targetBattler = gBattlerAttacker;
			break;
	}
    gBattleStruct->battlers[gBattlerAttacker].moveTarget = targetBattler;
	
    return targetBattler;
}

void CopyMoveTargetName(u8 battlerId, u16 move, u8 *dest)
{
	const u8 *str;
	
	switch (GetBattlerMoveTargetType(battlerId, move))
	{
		case MOVE_TARGET_SELECTED:
		case MOVE_TARGET_SELECTED_OPPONENT:
		    str = COMPOUND_STRING("Selected");
			break;
		case MOVE_TARGET_DEPENDS:
		    str = COMPOUND_STRING("Depends");
			break;
		case MOVE_TARGET_RANDOM:
		    str = COMPOUND_STRING("Random");
			break;
		case MOVE_TARGET_OPPONENTS_FIELD:
		    str = COMPOUND_STRING("Foe's Side");
			break;
		case MOVE_TARGET_BOTH:
		case MOVE_TARGET_OPPONENTS:
		    str = COMPOUND_STRING("Both Foes");
			break;
		case MOVE_TARGET_USER:
		    str = COMPOUND_STRING("User");
			break;
		case MOVE_TARGET_ALLY:
		    str = COMPOUND_STRING("User's Ally");
			break;
		case MOVE_TARGET_FOES_AND_ALLY:
		    str = COMPOUND_STRING("Foes & Ally");
			break;
		case MOVE_TARGET_USER_OR_ALLY:
			str = COMPOUND_STRING("User o Ally");
			break;
		case MOVE_TARGET_ALL_BATTLERS:
		    str = COMPOUND_STRING("All Field");
			break;
	}
	StringCopyN(dest, str, 12);
}

u8 IsMonDisobedient(void)
{
    s32 calc;
    u8 levelCapLevel = GetCurrentLevelCapLevel();

    if (levelCapLevel == MAX_LEVEL || (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_POKEDUDE)) || GetBattlerSide(gBattlerAttacker) == B_SIDE_OPPONENT
	|| !IsOtherTrainer(gBattleMons[gBattlerAttacker].otId, gBattleMons[gBattlerAttacker].otName) || gBattleMons[gBattlerAttacker].level <= levelCapLevel)
        return 0;

    calc = (gBattleMons[gBattlerAttacker].level + levelCapLevel) * (Random() & 255) >> 8;
    if (calc < levelCapLevel)
        return 0;
	
    // is not obedient
    if (gBattleMoves[gCurrentMove].effect == EFFECT_RAGE)
        gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_RAGE);
	
    if (gBattleMons[gBattlerAttacker].status1.id == STATUS1_SLEEP && (gBattleMoves[gCurrentMove].effect == EFFECT_SNORE || gBattleMoves[gCurrentMove].effect == EFFECT_SLEEP_TALK))
    {
        gBattlescriptCurrInstr = BattleScript_IgnoresWhileAsleep;
        return 1;
    }
    calc = (gBattleMons[gBattlerAttacker].level + levelCapLevel) * (Random() & 255) >> 8;
	
    if (calc < levelCapLevel && gBattleMoves[gCurrentMove].effect != EFFECT_FOCUS_PUNCH) // Additional check for focus punch in FR
    {
        calc = gBitTable[gCurrMovePos] | CheckMoveLimitations(gBattlerAttacker, 0);
		
        if (calc == MOVE_LIMITATION_ALL_MOVES_MASK) // all moves cannot be used
        {
            gBattleCommunication[MULTISTRING_CHOOSER] = RandomMax(ARRAY_COUNT(gInobedientStringIds)); // Choose a random string to print
            gBattlescriptCurrInstr = BattleScript_MoveUsedLoafingAround;
            return 1;
        }
        else // use a random move
        {
            do
                gCurrMovePos = gChosenMovePos = RandomMax(MAX_MON_MOVES);
            while (gBitTable[gCurrMovePos] & calc);
			
            gCalledMove = gBattleMons[gBattlerAttacker].moves[gCurrMovePos];
            gBattlescriptCurrInstr = BattleScript_IgnoresAndUsesRandomMove;
            return 2;
        }
    }
    else
    {
        levelCapLevel = gBattleMons[gBattlerAttacker].level - levelCapLevel;
        calc = (Random() & 255);
		
        if (calc < levelCapLevel && CanBePutToSleep(gBattlerAttacker, gBattlerAttacker, STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD) == STATUS_CHANGE_WORKED
		&& IsUproarActive() == gBattlersCount)
        {
			gBattlescriptCurrInstr = BattleScript_IgnoresAndFallsAsleep;
			return 1;
        }
        calc -= levelCapLevel;
		
        if (calc < levelCapLevel)
        {
            gBattleMoveDamage = CalculateConfusionDamage();
            gBattlescriptCurrInstr = BattleScript_IgnoresAndHitsItself;
            gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
            return 2;
        }
        else
        {
            gBattleCommunication[MULTISTRING_CHOOSER] = RandomMax(ARRAY_COUNT(gInobedientStringIds)); // Choose a random string to print
            gBattlescriptCurrInstr = BattleScript_MoveUsedLoafingAround;
            return 1;
        }
    }
}

u8 GetHiddenPowerType(struct Pokemon *mon)
{
	u8 type;
	s32 typeBits = ((GetMonData(mon, MON_DATA_HP_IV) & 1) << 0)
              | ((GetMonData(mon, MON_DATA_ATK_IV) & 1) << 1)
              | ((GetMonData(mon, MON_DATA_DEF_IV) & 1) << 2)
              | ((GetMonData(mon, MON_DATA_SPEED_IV) & 1) << 3)
              | ((GetMonData(mon, MON_DATA_SPATK_IV) & 1) << 4)
              | ((GetMonData(mon, MON_DATA_SPDEF_IV) & 1) << 5);
	
	type = (15 * typeBits) / 63 + 1;
	if (type >= TYPE_MYSTERY)
		++type;
	return type;
}

u8 GetPartyMonIdForIllusion(u8 battler, struct Pokemon *party, u8 partyCount, struct Pokemon *illusionMon)
{
	struct Pokemon *partnerMon;
	s8 i, id;
	
	if (GetMonAbility(illusionMon) == ABILITY_ILLUSION)
	{
		if (&party[partyCount - 1] != illusionMon)
		{
			if (IsBattlerAlive(BATTLE_PARTNER(battler)))
				partnerMon = &party[gBattlerPartyIndexes[BATTLE_PARTNER(battler)]];
			else
				partnerMon = illusionMon;
			
			// find last alive non-egg pokemon
			for (i = PARTY_SIZE - 1; i >= 0; i--)
			{
				id = i;
				
				if (MonCanBattle(&party[id]) && &party[id] != illusionMon && &party[id] != partnerMon)
				    return id;
			}
		}
	}
	return PARTY_SIZE;
}

static void SetIllusionMon(u8 battler)
{
	struct Pokemon *party = GetBattlerParty(battler);
	u8 id;
	
	gBattleStruct->battlers[battler].illusion.set = TRUE;

	id = GetPartyMonIdForIllusion(battler, party, GetBattlerSide(battler) == B_SIDE_PLAYER ? gPlayerPartyCount : gEnemyPartyCount, &party[gBattlerPartyIndexes[battler]]);
	
	if (id != PARTY_SIZE)
	{
		gBattleStruct->battlers[battler].illusion.on = TRUE;
		gBattleStruct->battlers[battler].illusion.broken = FALSE;
		gBattleStruct->battlers[battler].illusion.partyId = id;
		gBattleStruct->battlers[battler].illusion.mon = &party[id];
	}
}

static struct Pokemon *GetIllusionMonPtr(u8 battler)
{
	if (gBattleStruct->battlers[battler].illusion.broken)
		return NULL;
	
	if (!gBattleStruct->battlers[battler].illusion.set)
		SetIllusionMon(battler);
	
	if (!gBattleStruct->battlers[battler].illusion.on)
		return NULL;
	
	return gBattleStruct->battlers[battler].illusion.mon;
}

void ClearIllusionMon(u8 battler)
{
	memset(&gBattleStruct->battlers[battler].illusion, 0, sizeof(gBattleStruct->battlers[battler].illusion));
}

bool8 TryRemoveIllusion(u8 battler)
{
	if (GetIllusionMonPtr(battler) != NULL)
	{
		gBattleScripting.battler = battler;
		BattleScriptCall(BattleScript_IllusionOff);
		return TRUE;
	}
	return FALSE;
}

bool8 IsUnnerveOnOpposingField(u8 battler)
{
	if (ABILITY_ON_OPPOSING_SIDE(battler, ABILITY_UNNERVE) || ABILITY_ON_OPPOSING_SIDE(battler, ABILITY_AS_ONE_ICE_RIDER) || ABILITY_ON_OPPOSING_SIDE(battler, ABILITY_AS_ONE_SHADOW_RIDER))
		return TRUE;
	return FALSE;
}

u16 *GetUsedHeldItemPtr(u8 battler)
{
	return &gBattleStruct->sides[GetBattlerSide(battler)].party[gBattlerPartyIndexes[battler]].usedHeldItem;
}

bool8 NoAliveMonsForParty(struct Pokemon *party)
{
	u8 i;
	u32 HP_count = 0;
	
	for (i = 0; i < PARTY_SIZE; i++)
	{
		if (IsMonValidSpecies(&party[i]))
			HP_count += GetMonData(&party[i], MON_DATA_HP);
	}
	return (HP_count == 0);
}

bool8 NoAliveMonsForEitherParty(void)
{
	return (NoAliveMonsForParty(gPlayerParty) || NoAliveMonsForParty(gEnemyParty));
}

bool8 IsBattlerAlive(u8 battlerId)
{
	if (battlerId >= gBattlersCount || gBattleMons[battlerId].hp == 0 || (gAbsentBattlerFlags & gBitTable[battlerId]))
		return FALSE;
	return TRUE;
}

static void ShouldChangeFormOnWeatherStart(u8 battlerId)
{
	u8 i, side = GetBattlerSide(battlerId);
	struct Pokemon *party = GetBattlerParty(battlerId);
	
	for (i = 0; i < PARTY_SIZE; i++)
		gBattleStruct->sides[side].party[i].allowedToChangeFormInWeather = (GetMonData(&party[i], MON_DATA_SPECIES) == SPECIES_EISCUE_NOICE_FACE);
}

bool8 TryChangeBattleWeather(u8 battlerId, u8 weatherEnumId)
{
	u16 ability = GetBattlerAbility(battlerId);
	
	if ((gBattleWeather & B_WEATHER_PRIMAL_ANY) && ability != ABILITY_PRIMORDIAL_SEA && ability != ABILITY_DESOLATE_LAND && ability != ABILITY_DELTA_STREAM)
		return FALSE;
	if (!(gBattleWeather & sBattleWeatherInfo[weatherEnumId].flag))
	{
		gBattleWeather = sBattleWeatherInfo[weatherEnumId].flag;
		
		if (gBattleWeather & B_WEATHER_PRIMAL_ANY)
			gBattleStruct->weatherDuration = 0; // Primal weather is permanent
		else
		{
			if (GetBattlerItemHoldEffect(battlerId, TRUE) == HOLD_EFFECT_WEATHER_EXTEND && ItemId_GetHoldEffectParam(gBattleMons[battlerId].item) == weatherEnumId)
				gBattleStruct->weatherDuration = 8;
			else
				gBattleStruct->weatherDuration = 5;
		}
		ShouldChangeFormOnWeatherStart(battlerId);
		
		return TRUE;
	}
	return FALSE;
}

u8 GetCurrentWeatherEnumId(u16 weatherFlags)
{
	u8 i;
	
	for (i = 0; i < ARRAY_COUNT(sBattleWeatherInfo); i++)
	{
		if (weatherFlags & sBattleWeatherInfo[i].flag)
			return i;
	}
	return ENUM_WEATHER_COUNT;
}

void LoadWeatherIconSpriteAndPalette(u8 weatherEnumId)
{
	LoadSpriteSheet(&sBattleWeatherInfo[weatherEnumId].iconGfx);
	LoadSpritePalette(&sBattleWeatherInfo[weatherEnumId].iconPalette);
}

bool8 IsMoveMakingContact(u8 battler, u16 move)
{
	if (GetBattlerAbility(battler) != ABILITY_LONG_REACH && gBattleMoves[move].flags.makesContact)
		return TRUE;
	return FALSE;
}

// Check if defender is protected from attacker's move
bool8 IsBattlerProtected(u8 attacker, u8 defender, u16 move)
{
	if (GetBattlerAbility(attacker) == ABILITY_UNSEEN_FIST && IsMoveMakingContact(attacker, move))
		return FALSE;
	else if (gProtectStructs[defender].protected && !gBattleMoves[move].flags.forbiddenProtect)
		return TRUE;
	return FALSE;
}

bool8 LiftProtectionEffects(u8 battlerId)
{
	u8 side = GetBattlerSide(battlerId);
	
	if (gProtectStructs[battlerId].protected || (gSideStatuses[side] & SIDE_STATUS_PROTECTIONS_ANY))
	{
		gProtectStructs[battlerId].protected = FALSE;
		gSideStatuses[side] &= ~(SIDE_STATUS_PROTECTIONS_ANY);
		return TRUE;
	}
	return FALSE;
}

u8 CheckAbilityInBattle(u8 mode, u8 battlerId, u16 abilityId)
{
	u8 i, ret = 0;
	
	switch (mode)
	{
		case CHECK_ABILITY_ON_FIELD:
		case CHECK_ABILITY_ON_FIELD_EXCEPT_BATTLER:
		    for (i = 0; i < gBattlersCount; i++)
			{
				if (mode == CHECK_ABILITY_ON_FIELD_EXCEPT_BATTLER && i == battlerId) // Skip the exception battler
					continue;
					
				if (IsBattlerAlive(i) && GetBattlerAbility(i) == abilityId)
				{
					ret = i + 1;
					break;
				}
			}
			break;
		case CHECK_ABILITY_ON_SIDE:
		    for (i = 0; i < gBattlersCount; i++)
			{
				if (IsBattlerAlive(i) && IsBattlerAlly(battlerId, i) && GetBattlerAbility(i) == abilityId)
				{
					ret = i + 1;
					break;
				}
			}
			break;
	}
	return ret;
}

// Don't call Neutralizing Gas and Unnerve here bc at the point this function is called the two abilities already has ben called before
bool8 DoSwitchInAbilitiesItems(u8 battlerId)
{
	if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, battlerId) || TryActivateCommander(BATTLE_PARTNER(battlerId), TRUE)
	|| AbilityBattleEffects(ABILITYEFFECT_ON_WEATHER, battlerId) || AbilityBattleEffects(ABILITYEFFECT_ON_TERRAIN, battlerId)
    || ItemBattleEffects(ITEMEFFECT_ON_SWITCH_IN, battlerId, FALSE))
		return TRUE;
	return FALSE;
}

u8 CountBattlerStatIncreases(u8 battlerId, bool8 countEvasionAccuracy)
{
	u8 i, count;
	
	for (i = 0, count = 0; i < NUM_BATTLE_STATS; i++)
	{
		if (!countEvasionAccuracy && (i == STAT_ACC || i == STAT_EVASION))
			continue;
		if (gBattleMons[battlerId].statStages[i] > DEFAULT_STAT_STAGES)
			count += gBattleMons[battlerId].statStages[i] - DEFAULT_STAT_STAGES;
	}
	return count;
}

bool8 IsBattlerGrounded(u8 battlerId)
{
	if ((gStatuses3[battlerId] & STATUS3_ROOTED) || (gFieldStatus & STATUS_FIELD_GRAVITY))
		return TRUE;
	else if (GetBattlerAbility(battlerId) == ABILITY_LEVITATE || IsBattlerOfType(battlerId, TYPE_FLYING) || (gStatuses3[battlerId] & STATUS3_MAGNET_RISE))
		return FALSE;
	return TRUE;
}

bool8 CanBattlerGetOrLoseItem(u8 battlerId, u16 itemId)
{
	u8 holdEffect;
	u16 species = gBattleMons[battlerId].species;
	
	// Can't get or lose item if it can be used to activate its form
	if (GetBattlerFormChangeSpecies(battlerId, species, itemId, FORM_CHANGE_ULTRA_BURST) || GetBattlerFormChangeSpecies(battlerId, species, itemId, FORM_CHANGE_PRIMAL)
	|| GetBattlerFormChangeSpecies(battlerId, species, itemId, FORM_CHANGE_HOLD_ITEM) || GetBattlerFormChangeSpecies(battlerId, species, itemId, FORM_CHANGE_MEGA_EVO))
		return FALSE;
	
	// Check other items that can't be changed
	holdEffect = GetBattlerItemHoldEffect(battlerId, FALSE);
	
	if (ItemIsMail(gBattleMons[battlerId].item) || ItemIsMail(itemId) || (holdEffect == HOLD_EFFECT_BOOSTER_ENERGY && (gSpeciesInfo[species].flags & SPECIES_FLAG_PARADOX))
	|| holdEffect == HOLD_EFFECT_Z_CRYSTAL)
	    return FALSE;
	
	return TRUE;
}

bool8 CanStealItem(u8 battlerAtk, u8 battlerDef, u16 itemId)
{
	if (!itemId || gBattleMons[battlerAtk].item || (GetBattlerSide(battlerAtk) == B_SIDE_OPPONENT && GetBattlerSide(battlerDef) == B_SIDE_PLAYER
	&& !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_TRAINER)) && gTrainerBattleOpponent_A != TRAINER_SECRET_BASE)
	|| !CanBattlerGetOrLoseItem(battlerDef, itemId) || !CanBattlerGetOrLoseItem(battlerAtk, itemId))
		return FALSE;

	return TRUE;
}

void RemoveBattlerItem(u8 battlerId)
{
	gLastUsedItem = gBattleMons[battlerId].item;
	gBattleMons[battlerId].item = ITEM_NONE;
	CheckSetBattlerUnburden(battlerId);
	BtlController_EmitSetMonData(battlerId, BUFFER_A, REQUEST_HELDITEM_BATTLE, 0, sizeof(gBattleMons[battlerId].item), &gBattleMons[battlerId].item);
    MarkBattlerForControllerExec(battlerId);
}

void GiveItemToBattler(u8 battlerId, u16 itemId)
{
	gBattleMons[battlerId].item = itemId;
	gDisableStructs[battlerId].unburdenBoost = FALSE;
	BtlController_EmitSetMonData(battlerId, BUFFER_A, REQUEST_HELDITEM_BATTLE, 0, sizeof(gBattleMons[battlerId].item), &gBattleMons[battlerId].item);
	MarkBattlerForControllerExec(battlerId);
}

void SortBattlersBySpeed(u8 *battlers, bool8 slowToFast)
{
	s8 j;
	u8 i, battler;
	u32 currSpeed, speeds[MAX_BATTLERS_COUNT] = {0};
	
	for (i = 0; i < gBattlersCount; i++)
		speeds[i] = GetBattlerTotalSpeed(battlers[i]);
	
	for (i = 1; i < gBattlersCount; i++)
	{
		battler = battlers[i];
		currSpeed = speeds[i];
		j = i - 1;
		
		if (slowToFast)
		{
			while (j >= 0 && speeds[j] > currSpeed)
			{
				battlers[j + 1] = battlers[j];
				speeds[j + 1] = speeds[j];
				j -= 1;
			}
		}
		else
		{
			while (j >= 0 && speeds[j] < currSpeed)
			{
				battlers[j + 1] = battlers[j];
				speeds[j + 1] = speeds[j];
				j -= 1;
			}
		}
		battlers[j + 1] = battler;
		speeds[j + 1] = currSpeed;
	}
}

bool8 CanBattlerEscape(u8 battlerId, bool8 checkIngrain)
{
	if (IsBattlerOfType(battlerId, TYPE_GHOST))
		return TRUE;
	else if (gDisableStructs[battlerId].wrapTurns || (gBattleMons[battlerId].status2 & STATUS2_ESCAPE_PREVENTION) || (checkIngrain && (gStatuses3[battlerId] & STATUS3_ROOTED)))
		return FALSE;
	else
		return TRUE;
}

u8 IsAbilityPreventingSwitchOut(u8 battlerId)
{
	u8 ret;
	
	if (!IsBattlerOfType(battlerId, TYPE_GHOST))
	{
		if (IsBattlerGrounded(battlerId) && (ret = ABILITY_ON_OPPOSING_SIDE(battlerId, ABILITY_ARENA_TRAP)))
			return ret;
		else if (GetBattlerAbility(battlerId) != ABILITY_SHADOW_TAG && (ret = ABILITY_ON_OPPOSING_SIDE(battlerId, ABILITY_SHADOW_TAG)))
			return ret;
		else if (IsBattlerOfType(battlerId, TYPE_STEEL) && (ret = ABILITY_ON_OPPOSING_SIDE(battlerId, ABILITY_MAGNET_PULL)))
			return ret;
	}
	return 0;
}

u8 CountUsablePartyMons(u8 battlerId, u8 *viableMons)
{
	u8 i, numViableMons = 0;
	u8 firstMonId, lastMonId;
	u8 battlerIn2, battlerIn1 = gBattlerPartyIndexes[battlerId];
	struct Pokemon *party;
	
	if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
	{
		if (GetLinkTrainerFlankId(GetBattlerMultiplayerId(battlerId)) == 1)
		{
			firstMonId = MULTI_PARTY_SIZE;
			lastMonId = PARTY_SIZE;
		}
		else
		{
			firstMonId = 0;
			lastMonId = MULTI_PARTY_SIZE;
		}
		battlerIn2 = gBattlerPartyIndexes[BATTLE_PARTNER(battlerId)];
	}
	else
	{
		firstMonId = 0;
		lastMonId = PARTY_SIZE;
		
		if (IsDoubleBattleForBattler(battlerId))
			battlerIn2 = gBattlerPartyIndexes[BATTLE_PARTNER(battlerId)];
		else
			battlerIn2 = battlerIn1;
	}
	party = GetBattlerParty(battlerId);
	
	for (i = firstMonId; i < lastMonId; i++)
	{
		if (i != battlerIn1 && i != battlerIn2 && MonCanBattle(&party[i]))
			viableMons[numViableMons++] = i;
	}
	return numViableMons;
}

bool8 CanBattlerSwitchOut(u8 battlerId, bool8 checkEscapePrevention)
{
	if (checkEscapePrevention && !CanBattlerEscape(battlerId, TRUE))
		return FALSE;
	else if (IsBattlerBeingCommanded(battlerId) || !CanBattlerSwitch(battlerId))
		return FALSE;
	else
		return TRUE;
}

u8 GetBattlerTurnOrderNum(u8 battlerId)
{
    u8 i;
    
    for (i = 0; i < gBattlersCount && gBattlerByTurnOrder[i] != battlerId; ++i) {};
	
	return i;
}

u32 CalcSecondaryEffectChance(u8 battlerId, u8 moveEffect, u32 chance)
{
	bool8 hasSereneGrace = (GetBattlerAbility(battlerId) == ABILITY_SERENE_GRACE);
	bool8 sideHasRainbow = (gSideStatuses[GetBattlerSide(battlerId)] & SIDE_STATUS_RAINBOW);
	
	// Flinch effect don't stack if both are active
	if (hasSereneGrace && sideHasRainbow && moveEffect == MOVE_EFFECT_FLINCH)
		return chance * 2;
	
	if (hasSereneGrace)
		chance *= 2;
	
	// Rainbow's effect can't double Secret Power's secondary effect chance
	if (sideHasRainbow && moveEffect != MOVE_EFFECT_SECRET_POWER)
		chance *= 2;
	
	return chance;
}

u8 IsUproarActive(void)
{
	u8 i;
	
	for (i = 0; i < gBattlersCount; i++)
	{
		if (gBattleMons[i].status2 & STATUS2_UPROAR)
			break;
	}
	return i;
}

bool8 UproarWakeUpCheck(void)
{
	u8 ret = IsUproarActive();
	
	if (ret != gBattlersCount)
	{
		gBattleScripting.battler = ret;
		
		if (gBattlerTarget == 0xFF)
			gBattlerTarget = ret;
		else if (gBattlerTarget == ret)
			gBattleCommunication[MULTISTRING_CHOOSER] = 0;
		else
			gBattleCommunication[MULTISTRING_CHOOSER] = 1;
		
		return TRUE;
	}
	return FALSE;
}

bool8 TryResetBattlerStatChanges(u8 battlerId)
{
	u8 i;
	bool8 statReseted = FALSE;
	
	for (i = 0; i < NUM_BATTLE_STATS; i++)
	{
		if (gBattleMons[battlerId].statStages[i] != DEFAULT_STAT_STAGES)
		{
			gBattleMons[battlerId].statStages[i] = DEFAULT_STAT_STAGES;
			statReseted = TRUE; // Return TRUE if any stat was reseted
		}
	}
	return statReseted;
}

void CopyBattlerStatChanges(u8 battler1, u8 battler2) // Battler1 copy battler2's stat changes
{
	u8 i;
	
	for (i = 0; i < NUM_BATTLE_STATS; i++)
		gBattleMons[battler1].statStages[i] = gBattleMons[battler2].statStages[i];
}

u8 GetBattlerGender(u8 battlerId)
{
	return GetGenderFromSpeciesAndPersonality(gBattleMons[battlerId].species, gBattleMons[battlerId].personality);
}

// Check if battlerIdAtk can be infatuated by battlerIdDef
u8 CanBeInfatuatedBy(u8 battlerIdAtk, u8 battlerIdDef)
{
	u8 ret, atkGender, defGender;
	
	// Check attacker's abilities
	if (GetBattlerAbility(battlerIdAtk) == ABILITY_OBLIVIOUS)
		return STATUS_CHANGE_FAIL_ABILITY_PREVENTED;
	
	// Check side abilities
	CHECK_SIDE_PROTECT_ABILITY(battlerIdAtk, ABILITY_AROMA_VEIL, STATUS_CHANGE_FAIL_AROMA_VEIL_ON_SIDE)
	
	// Check already infatuated
	if (gBattleMons[battlerIdAtk].status2 & STATUS2_INFATUATION)
		return STATUS_CHANGE_FAIL_ALREADY_STATUSED;
	
	// Check gender
	atkGender = GetBattlerGender(battlerIdAtk), defGender = GetBattlerGender(battlerIdDef);
	
	if (atkGender == MON_GENDERLESS || defGender == MON_GENDERLESS || atkGender == defGender)
		return STATUS_CHANGE_FAIL_TYPE_NOT_AFFECTED; // Use this instead of add a new return value
	
	return STATUS_CHANGE_WORKED;
}

u32 GetBattlerWeight(u8 battlerId)
{
	u32 weight = gSpeciesInfo[gBattleMons[battlerId].species].weight;
	
	switch (GetBattlerAbility(battlerId))
	{
		case ABILITY_HEAVY_METAL:
			weight *= 2;
			break;
		case ABILITY_LIGHT_METAL:
			weight /= 2;
			break;
	}
	if (weight == 0)
		weight = 1;
	
	return weight;
}

bool8 IsBattlerAffectedBySpore(u8 battlerId)
{
	if (IsBattlerOfType(battlerId, TYPE_GRASS) || GetBattlerAbility(battlerId) == ABILITY_OVERCOAT || GetBattlerItemHoldEffect(battlerId, TRUE) == HOLD_EFFECT_SAFETY_GOGGLES)
		return FALSE;
	return TRUE;
}

void RemoveOrAddBattlerOnPickupStack(u8 battlerId, bool8 addToStack)
{
	u8 i, j, newStack[MAX_BATTLERS_COUNT];
	
	for (i = 0, j = 0; i < gBattlersCount; i++)
	{
		if (gBattleStruct->pickupStack[i] != 0xFF && gBattleStruct->pickupStack[i] != battlerId)
			newStack[j++] = gBattleStruct->pickupStack[i];
	}
	if (addToStack)
		newStack[j++] = battlerId;
	
	while (j < gBattlersCount)
		newStack[j++] = 0xFF;
	
	for (i = 0; i < gBattlersCount; i++)
		gBattleStruct->pickupStack[i] = newStack[i];
}

u8 GetBattlerOnTopOfPickupStack(u8 battlerId)
{
	u8 i;
	
	for (i = 0; i < gBattlersCount; i++)
	{
		if (gBattleStruct->pickupStack[i] == 0xFF)
			break;
	}
	// Stack is empty or only contains the ignored battlerId
	if (i == 0 || (i == 1 && gBattleStruct->pickupStack[0] == battlerId))
		return 0xFF;
	
	if (gBattleStruct->pickupStack[i - 1] == battlerId)
		return gBattleStruct->pickupStack[i - 2];
	
	return gBattleStruct->pickupStack[i - 1];
}

bool8 TryRecycleBattlerItem(u8 battlerRecycler, u8 battlerItem)
{
	u16 *usedHeldItem = GetUsedHeldItemPtr(battlerItem);
	
	if (*usedHeldItem && !gBattleMons[battlerRecycler].item)
	{
		gLastUsedItem = *usedHeldItem;
        *usedHeldItem = ITEM_NONE;
		
		gBattleMons[battlerRecycler].item = gLastUsedItem;
		
		RemoveOrAddBattlerOnPickupStack(battlerItem, FALSE);
		
		gDisableStructs[battlerRecycler].unburdenBoost = FALSE;
		
		if (battlerRecycler != battlerItem)
			CheckSetBattlerUnburden(battlerItem);
		
		BtlController_EmitSetMonData(battlerRecycler, BUFFER_A, REQUEST_HELDITEM_BATTLE, 0, sizeof(gBattleMons[battlerRecycler].item), &gBattleMons[battlerRecycler].item);
        MarkBattlerForControllerExec(battlerRecycler);
		return TRUE;
	}
	return FALSE;
}

// Protosynthesis count stat stages, but Beast Boost not. Probably if it be available in SV it will be changed to count too, so just count it...
u8 GetBattlerHighestStatId(u8 battlerId)
{
	u8 i, temp2, statIds[NUM_STATS], highestStatId;
	u16 *statVal, temp, stats[NUM_STATS], highestStat;
	
	// Put the stats and ids into the array
	for (i = STAT_ATK; i < NUM_STATS; i++)
	{
		statVal = &gBattleMons[battlerId].attack + ((i - 1) * 2); // -1 bc STAT_ATK is 1, and not 0
		APPLY_STAT_MOD(stats[i], &gBattleMons[battlerId], *statVal, i);
		statIds[i] = i;
	}
	// Put stats in order of Attack, Defense, Sp. Atk, Sp. Def and Speed
	// without this the order will be Attack, Defense, Speed, Sp. Atk and Sp. Def. What this loop does is only move the Speed to the end of the array
	for (i = STAT_SPEED; i < NUM_STATS - 1; i++)
	{
		SWAP(stats[i], stats[i + 1], temp);
		SWAP(statIds[i], statIds[i + 1], temp2);
	}
	// Get the highest stat id
	for (i = highestStatId = STAT_ATK, highestStat = gBattleMons[battlerId].attack; i < NUM_STATS; i++)
	{
		if (highestStat < stats[statIds[i]])
		{
			highestStat = stats[statIds[i]];
			highestStatId = statIds[i];
		}
	}
	return highestStatId;
}

bool8 IsBattlerAffectedByFollowMe(u8 battlerId, u8 opposingSide, u16 move)
{
	u16 atkAbility = GetBattlerAbility(battlerId);
	
	if (!gSideTimers[opposingSide].followmeSet || !IsBattlerAlive(gSideTimers[opposingSide].followmeTarget) || atkAbility == ABILITY_PROPELLER_TAIL
	|| atkAbility == ABILITY_STALWART || gBattleMoves[move].effect == EFFECT_SKY_DROP || gBattleMoves[move].effect == EFFECT_FUTURE_SIGHT
	|| (gStatuses3[gSideTimers[opposingSide].followmeTarget] & STATUS3_SKY_DROPPED))
	    return FALSE;
		
	return TRUE;
}

// battlerId = the battler using the item
// partyIndex = the party index the item effect get applyed
u8 GetBattleMonForItemUse(u8 battlerId, u8 partyIndex)
{
	u8 i, battleMonId = MAX_BATTLERS_COUNT;
	
	if (gMain.inBattle)
	{
		for (i = 0; i < gBattlersCount; i++)
		{
			if (partyIndex == gBattlerPartyIndexes[i])
			{
				battleMonId = i;
				break;
			}
		}
	}
	return battleMonId;
}

u8 GetItemUseBattler(u8 battlerId)
{
	return GetBattleMonForItemUse(battlerId, gBattleStruct->battlers[battlerId].itemPartyIndex);
}

bool8 IsItemUseBlockedByBattleEffect(u8 battlerId)
{
	bool8 blocked = FALSE;
	
	if (battlerId != MAX_BATTLERS_COUNT)
	{
		if (gStatuses3[battlerId] & STATUS3_EMBARGO)
			blocked = TRUE;
	}
	return blocked;
}

#define BALL_THROW_SUCCESS                  0
#define BALL_THROW_UNABLE_TWO_MONS          1
#define BALL_THROW_UNABLE_TOTEM_BATTLE      2
#define BALL_THROW_UNABLE_NO_ROOM           3
#define BALL_THROW_UNABLE_SEMI_INVULNERABLE 4

static u8 GetBallThrowableState(void)
{
	if (IS_WHOLE_SIDE_ALIVE(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)))
		return BALL_THROW_UNABLE_TWO_MONS;
	else if (gBattleTypeFlags & BATTLE_TYPE_TOTEM)
		return BALL_THROW_UNABLE_TOTEM_BATTLE;
	else if (IsPlayerPartyAndPokemonStorageFull())
		return BALL_THROW_UNABLE_NO_ROOM;
	else if ((gStatuses3[GetCatchingBattler()] & (STATUS3_SEMI_INVULNERABLE | STATUS3_COMMANDING)))
		return BALL_THROW_UNABLE_SEMI_INVULNERABLE;
	else
		return BALL_THROW_SUCCESS;
}

const u8 *PokemonUseItemEffectsBattle(u8 battlerId, u16 itemId, bool8 *canUse)
{
	const u8 *failStr = NULL;
	u8 i;
	u16 holdEffectParam = ItemId_GetHoldEffectParam(itemId);
	
	switch (ItemId_GetBattleUsage(itemId))
	{
		case EFFECT_ITEM_INCREASE_STAT:
		    if (CompareStat(battlerId, holdEffectParam, MAX_STAT_STAGES, CMP_LESS_THAN))
				*canUse = TRUE;
			break;
		case EFFECT_ITEM_SET_FOCUS_ENERGY:
		    if (!(gBattleMons[battlerId].status2 & STATUS2_FOCUS_ENERGY))
			{
				gBattleMons[battlerId].status2 |= STATUS2_FOCUS_ENERGY;
				*canUse = TRUE;
			}
			break;
		case EFFECT_ITEM_SET_MIST:
		    if (!(gSideStatuses[GetBattlerSide(battlerId)] & SIDE_STATUS_MIST))
				*canUse = TRUE;
			break;
		case EFFECT_ITEM_ESCAPE:
		    if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER))
				*canUse = TRUE;
			break;
		case EFFECT_ITEM_THROW_BALL:
		    switch (GetBallThrowableState())
			{
				case BALL_THROW_SUCCESS:
					*canUse = TRUE;
					break;
				case BALL_THROW_UNABLE_TWO_MONS:
				    failStr = COMPOUND_STRING("Cannot throw a ball!\nThere are two Pokémon out there!\p");
					break;
				case BALL_THROW_UNABLE_TOTEM_BATTLE:
					failStr = COMPOUND_STRING("Cannot throw a ball!\nYou are in a Totem Pokémon battle!\p");
					break;
				case BALL_THROW_UNABLE_NO_ROOM:
				    failStr = COMPOUND_STRING("The Box is full.{PAUSE_UNTIL_PRESS}");
					break;
				case BALL_THROW_UNABLE_SEMI_INVULNERABLE:
				    failStr = COMPOUND_STRING("Cannot throw a ball!\nThere's no Pokémon in sight!\p");
					break;
			}
			break;
		case EFFECT_ITEM_INCREASE_ALL_STATS:
		    for (i = STAT_ATK; i < NUM_STATS; i++)
			{
				if (CompareStat(battlerId, i, MAX_STAT_STAGES, CMP_LESS_THAN))
					*canUse = TRUE;
			}
			break;
		case EFFECT_ITEM_POKE_FLUTE:
			*canUse = TRUE; // Always can be used
			break;
		case EFFECT_ITEM_ADRENALINE_ORB:
			if (gBattleTypeFlags & BATTLE_TYPE_SOS)
				*canUse = TRUE;
			break;
				
	}
	return failStr;
}

s8 GetItemStatChangeStages(u16 item)
{
	s8 stages;
	
	switch (item)
	{
		case ITEM_X_ATTACK:
		case ITEM_X_DEFEND:
		case ITEM_X_SPEED:
		case ITEM_X_ACCURACY:
		case ITEM_X_SPECIAL:
		case ITEM_X_SPECIAL_DEFENSE:
		    stages = +2;
			break;
		default:
			stages = +1;
			break;
	}
	return stages;
}

bool8 IsBattleAnimationsOn(void)
{
	if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_POKEDUDE)) && gSaveBlock2Ptr->optionsBattleSceneOff)
		return FALSE;
	return TRUE;
}

struct Pokemon *GetBattlerParty(u8 battlerId)
{
	return GetBattlerSide(battlerId) == B_SIDE_PLAYER ? gPlayerParty : gEnemyParty;
}

struct Pokemon *GetBattlerPartyIndexPtr(u8 battlerId)
{
	return &GetBattlerParty(battlerId)[gBattlerPartyIndexes[battlerId]];
}

// Same as above, but checking for Illusion
struct Pokemon *GetBattlerIllusionPartyIndexPtr(u8 battlerId)
{
	struct Pokemon *illusionMon = GetIllusionMonPtr(battlerId);
	return illusionMon != NULL ? illusionMon : GetBattlerPartyIndexPtr(battlerId);
}

u8 CountAliveMonsInBattle(u8 battlerId, u8 caseId)
{
    u8 i, retVal = 0;

    switch (caseId)
    {
    case BATTLE_ALIVE_SIDE:
        for (i = 0; i < MAX_BATTLERS_COUNT; i++)
        {
            if (IsBattlerAlive(i) && IsBattlerAlly(battlerId, i))
                retVal++;
        }
        break;
	case BATTLE_ALIVE_EXCEPT_BATTLER:
	    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
        {
            if (i != battlerId && IsBattlerAlive(i))
                retVal++;
        }
        break;
    }
    return retVal;
}

void CalculatePayDayMoney(void)
{
	u8 i;
	
	for (i = 0; i < MAX_BATTLERS_COUNT; i++)
	{
		if (GetBattlerSide(i) == B_SIDE_PLAYER && gBattleStruct->battlers[i].payDayLevel)
		{
			u16 payDayMoney = gPaydayMoney + (gBattleStruct->battlers[i].payDayLevel * 5);
			
			if (payDayMoney > 0xFFFF)
				gPaydayMoney = 0xFFFF;
			else
				gPaydayMoney = payDayMoney;
			
			gBattleStruct->battlers[i].payDayLevel = 0;
		}
	}
}

s32 GetDrainedBigRootHp(u8 battlerId, s32 hp)
{
	if (GetBattlerItemHoldEffect(battlerId, TRUE) == HOLD_EFFECT_BIG_ROOT)
		hp = (hp * (100 + ItemId_GetHoldEffectParam(gBattleMons[battlerId].item))) / 100;
	
	if (hp == 0)
		hp = 1;
	
	return hp * -1;
}

bool8 IsBattlerWeatherAffected(u8 battlerId, u16 weatherFlags)
{
	if (WEATHER_HAS_EFFECT && (gBattleWeather & weatherFlags))
	{
		return TRUE;
	}
	return FALSE;
}

u8 GetBattlerMoveType(u8 battlerId, u16 move)
{
	u8 type = gBattleMoves[move].type;
	u16 item = GetBattlerItem(battlerId);
	
	if (move != MOVE_STRUGGLE)
	{
		// These move effects are'nt affected by type change abilities
		switch (gBattleMoves[move].effect)
		{
			case EFFECT_HIDDEN_POWER:
				return GetHiddenPowerType(GetBattlerPartyIndexPtr(battlerId));
			case EFFECT_WEATHER_BALL:
				if (IsBattlerWeatherAffected(battlerId, B_WEATHER_RAIN_ANY))
					type = TYPE_WATER;
				else if (IsBattlerWeatherAffected(battlerId, B_WEATHER_SANDSTORM))
					type = TYPE_ROCK;
				else if (IsBattlerWeatherAffected(battlerId, B_WEATHER_SUN_ANY))
					type = TYPE_FIRE;
				else if (IsBattlerWeatherAffected(battlerId, B_WEATHER_HAIL))
					type = TYPE_ICE;
				return type;
			case EFFECT_NATURAL_GIFT:
				if (ItemId_GetPocket(item) == POCKET_BERRY_POUCH)
					type = gNaturalGiftTable[ITEM_TO_BERRY(item)].type;
				return type;
		}
		
		// Check type changing abilities
		switch (GetBattlerAbility(battlerId))
		{
			case ABILITY_NORMALIZE:
				if (type != TYPE_NORMAL)
					type = TYPE_NORMAL;
				break;
			case ABILITY_REFRIGERATE:
				if (type == TYPE_NORMAL)
					type = TYPE_ICE;
				break;
			case ABILITY_PIXILATE:
				if (type == TYPE_NORMAL)
					type = TYPE_FAIRY;
				break;
			case ABILITY_AERILATE:
				if (type == TYPE_NORMAL)
					type = TYPE_FLYING;
				break;
			case ABILITY_GALVANIZE:
				if (type == TYPE_NORMAL)
					type = TYPE_ELECTRIC;
				break;
			case ABILITY_LIQUID_VOICE:
				if (gBattleMoves[move].flags.soundMove)
					type = TYPE_WATER;
				break;
		}
	}
	return type;
}

// Try transform battler1 into battler2
bool8 TryTransformIntoBattler(u8 battler1, u8 battler2)
{
	u8 i, *battleMonAttacker, *battleMonTarget;
	u16 oldAbility;
	
	if (!(gBattleMons[battler2].status2 & (STATUS2_TRANSFORMED | STATUS2_SUBSTITUTE)) && !(gStatuses3[battler2] & STATUS3_SEMI_INVULNERABLE)
	&& !gBattleStruct->battlers[battler1].illusion.on && !gBattleStruct->battlers[battler2].illusion.on && !(gBattleMons[battler1].status2 & STATUS2_TRANSFORMED))
	{
		gBattleMons[battler1].status2 |= STATUS2_TRANSFORMED;
		
		CopyBattlerCritModifier(battler1, battler2); // Copy Focus Energy
		
        gDisableStructs[battler1].disabledMove = MOVE_NONE;
        gDisableStructs[battler1].disableTimer = 0;
        gDisableStructs[battler1].transformedMonPersonality = gBattleMons[battler2].personality;
		gDisableStructs[battler1].transformedMonShynies = GetMonData(GetBattlerPartyIndexPtr(battler2), MON_DATA_IS_SHINY);
        gDisableStructs[battler1].mimickedMoves = 0;
		
        PrepareSpeciesBuffer(gBattleTextBuff1, gBattleMons[battler2].species);
		
		oldAbility = gBattleMons[battler1].ability; // Save ability
		
		battleMonAttacker = (u8 *)(&gBattleMons[battler1]);
        battleMonTarget = (u8 *)(&gBattleMons[battler2]);
		
		for (i = 0; i < offsetof(struct BattlePokemon, pp); i++)
			battleMonAttacker[i] = battleMonTarget[i];
		
		gBattleMons[battler1].ability = oldAbility; // Restore ability, nessesary for SetBattlerAbility
		SetBattlerAbility(battler1, gBattleMons[battler2].ability);
		
        for (i = 0; i < MAX_MON_MOVES; ++i)
        {
            if (gBattleMoves[gBattleMons[battler1].moves[i]].pp < 5)
                gBattleMons[battler1].pp[i] = gBattleMoves[gBattleMons[battler1].moves[i]].pp;
            else
                gBattleMons[battler1].pp[i] = 5;
        }
        BtlController_EmitResetActionMoveSelection(battler1, BUFFER_A, RESET_MOVE_SELECTION);
        MarkBattlerForControllerExec(battler1);
		return TRUE;
	}
	return FALSE;
}

bool8 TryDisableMove(u8 battlerId, u8 movePos, u16 move)
{
	if (move != MOVE_STRUGGLE && !gDisableStructs[battlerId].disabledMove && gBattleMons[battlerId].pp[movePos] && !ABILITY_ON_SIDE(battlerId, ABILITY_AROMA_VEIL))
	{
		gDisableStructs[battlerId].disabledMove = move;
		gDisableStructs[battlerId].disableTimer = 4;
		return TRUE;
	}
	return FALSE;
}

// Check if target can be protected by Safeguard
bool8 CanSafeguardProtectBattler(u8 attacker, u8 defender)
{
	if (GetBattlerAbility(attacker) != ABILITY_INFILTRATOR && (gSideStatuses[GetBattlerSide(defender)] & SIDE_STATUS_SAFEGUARD))
		return TRUE;
	return FALSE;
}

bool8 IsBattlerProtectedByFlowerVeil(u8 battlerId)
{
	u8 ret;
	
	if (IsBattlerOfType(battlerId, TYPE_GRASS))
		CHECK_SIDE_PROTECT_ABILITY(battlerId, ABILITY_FLOWER_VEIL, TRUE)

	return FALSE;
}

void GetBattlerTypes(u8 battlerId, u8 *types)
{
	types[0] = gBattleMons[battlerId].type1;
	types[1] = gBattleMons[battlerId].type2;
	types[2] = gBattleMons[battlerId].type3;
	
	// Handle Roost
	if (gDisableStructs[battlerId].roostActive)
	{
		if (types[0] == TYPE_FLYING && types[1] == TYPE_FLYING)
			types[0] = types[1] = TYPE_NORMAL;
		else if (types[0] == TYPE_FLYING)
			types[0] = TYPE_MYSTERY;
		else if (types[1] == TYPE_FLYING)
			types[1] = TYPE_MYSTERY;
	}
}

u8 GetBattlerType(u8 battlerId, u8 index)
{
	u8 types[3];
	GetBattlerTypes(battlerId, types);
	return types[index - 1];
}

static inline void SetBattlerTypesInternal(u8 battlerId, u8 type1, u8 type2)
{
	gBattleMons[battlerId].type1 = type1;
    gBattleMons[battlerId].type2 = type2;
	gBattleMons[battlerId].type3 = TYPE_MYSTERY;
}

void SetBattlerType(u8 battlerId, u8 type)
{
	gBattleScripting.battler = battlerId;
	SetBattlerTypesInternal(battlerId, type, type);
	PrepareTypeBuffer(gBattleTextBuff1, type);
}

void SetBattlerInitialTypes(u8 battlerId)
{
	u16 species = gBattleMons[battlerId].species;
	SetBattlerTypesInternal(battlerId, gSpeciesInfo[species].types[0], gSpeciesInfo[species].types[1]);
}

// Attacker copy Defender's crit modifiers
bool8 CopyBattlerCritModifier(u8 attacker, u8 defender)
{
	bool8 copied = FALSE;
	
	gBattleMons[attacker].status2 &= ~(STATUS2_FOCUS_ENERGY);
	gBattleMons[attacker].status2 |= (gBattleMons[defender].status2 & STATUS2_FOCUS_ENERGY);
	
	if (gBattleMons[attacker].status2 & STATUS2_FOCUS_ENERGY)
		copied = TRUE;
	
	return copied;
}

bool8 DoesSpreadMoveStrikesOnlyOnce(u8 attacker, u8 defender, u16 move, bool8 checkTargetsDone)
{
	if (IsDoubleBattleForBattler(defender))
	{
		switch (GetBattlerMoveTargetType(attacker, move))
		{
			case MOVE_TARGET_BOTH:
			    if (IS_WHOLE_SIDE_ALIVE(defender)) // Both foes alive to be attacked
				    return FALSE;
				else if (checkTargetsDone && gBattleStruct->battlers[attacker].targetsDone & gBitTable[BATTLE_PARTNER(defender)]) // Already did damage to one foe
					return FALSE;
				break;
			case MOVE_TARGET_FOES_AND_ALLY:
				if (CountAliveMonsInBattle(attacker, BATTLE_ALIVE_EXCEPT_BATTLER) >= 2) // Both foes or ally alive to be attacked
				    return FALSE;
				else if (checkTargetsDone && ((gBattleStruct->battlers[attacker].targetsDone & gBitTable[BATTLE_PARTNER(defender)])
					|| (gBattleStruct->battlers[attacker].targetsDone & gBitTable[BATTLE_PARTNER(attacker)]))) // Already did damage at least one target
				    return FALSE;
				break;
		}
	}
	return TRUE;
}

bool8 TryRemoveScreens(u8 battler, bool8 clear, bool8 fromBothSides)
{
	u8 side, i = 0;
	bool8 removed = FALSE;
	
	do
	{
		side = GetBattlerSide(battler);
		
		if (gSideStatuses[side] & SIDE_STATUS_SCREENS_ANY)
		{
			if (clear)
				gSideStatuses[side] &= ~(SIDE_STATUS_SCREENS_ANY);
			
			removed = TRUE;
		}
		battler = BATTLE_OPPOSITE(battler);
		i++;
	} while (fromBothSides && i < B_SIDE_COUNT);
	
	return removed;
}

bool8 TryRemoveEntryHazards(u8 battlerId, bool8 clear, bool8 fromBothSides)
{
	u8 side, i = 0;
	bool8 removed = FALSE;
	
	do
	{
		side = GetBattlerSide(battlerId);
		
		if (gSideStatuses[side] & SIDE_STATUS_HAZARDS_ANY)
		{
			if (clear)
			{
				gSideStatuses[side] &= ~(SIDE_STATUS_HAZARDS_ANY);
				
				gSideTimers[side].spikesAmount = 0;
				gSideTimers[side].toxicSpikesAmount = 0;
				
				RemoveBattleEffectFromSideQueueList(side, B_SIDE_QUEUED_SPIKES);
				RemoveBattleEffectFromSideQueueList(side, B_SIDE_QUEUED_TOXIC_SPIKES);
			}
			removed = TRUE;
		}
		battlerId = BATTLE_OPPOSITE(battlerId);
		i++;
	} while (fromBothSides && i < B_SIDE_COUNT);
	
	return removed;
}

void SaveAttackerToStack(u8 battlerId)
{
	gBattleStruct->savedAttackerStack[gBattleStruct->savedAttackerStackCount++] = gBattlerAttacker;
	gBattlerAttacker = battlerId;
}

void RestoreAttackerFromStack(void)
{
	gBattlerAttacker = gBattleStruct->savedAttackerStack[--gBattleStruct->savedAttackerStackCount];
}

void SaveTargetToStack(u8 battlerId)
{
	gBattleStruct->savedTargetStack[gBattleStruct->savedTargetStackCount++] = gBattlerTarget;
	gBattlerTarget = battlerId;
}

void RestoreTargetFromStack(void)
{
	gBattlerTarget = gBattleStruct->savedTargetStack[--gBattleStruct->savedTargetStackCount];
}

void GetBattlerMovesArray(u8 battlerId, u16 *moves)
{
	u8 i;
	
	if (BattleAI_IsRunning())
	{
		for (i = 0; i < MAX_MON_MOVES; i++)
		{
			if (!BattleAI_KnowsBattlerMoveIndex(battlerId, i))
				moves[i] = MOVE_NONE;
			else
				moves[i] = gBattleMons[battlerId].moves[i];
		}
	}
	else
	{
		for (i = 0; i < MAX_MON_MOVES; i++)
			moves[i] = gBattleMons[battlerId].moves[i];
	}
}

u8 FindMoveSlotInBattlerMoveset(u8 battlerId, u16 move)
{
	u8 i;
	u16 moves[MAX_MON_MOVES];
	
	GetBattlerMovesArray(battlerId, moves);
	
	for (i = 0; i < MAX_MON_MOVES; i++)
	{
		if (moves[i] == move)
			break;
	}
	return i;
}

bool8 CanUseLastResort(u8 battlerId)
{
	u8 i;
	u16 moves[MAX_MON_MOVES];
	bool8 knowsMove = FALSE;
	
	GetBattlerMovesArray(battlerId, moves);
	
	for (i = 0; i < MAX_MON_MOVES && moves[i]; i++)
	{
		if (moves[i] == gCurrentMove)
			knowsMove = TRUE;
		else if (!(gDisableStructs[battlerId].usedMoveIndices & gBitTable[i]))
			return FALSE;
	}
	return (i != 1 && knowsMove);
}

bool8 TrySetToxicSpikesOnBattlerSide(u8 battlerId)
{
	u8 side = GetBattlerSide(battlerId);
	
	if (gSideTimers[side].toxicSpikesAmount != 2)
    {
		++gSideTimers[side].toxicSpikesAmount;
        gSideStatuses[side] |= SIDE_STATUS_TOXIC_SPIKES;
		AddBattleEffectToSideQueueList(side, B_SIDE_QUEUED_TOXIC_SPIKES);
        return TRUE;
    }
	return FALSE;
}

void TryUpdateEvolutionTracker(u16 evoMode, u32 upAmount, u16 data)
{
	u16 value;
	struct Pokemon *mon = GetBattlerPartyIndexPtr(gBattlerAttacker);
	const u8 *evolutions = gSpeciesInfo[gBattleMons[gBattlerAttacker].species].evolutions;
	
	if (evolutions != NULL && GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER && !(gBattleTypeFlags & (BATTLE_TYPE_FIRST_BATTLE | BATTLE_TYPE_POKEDUDE
	| BATTLE_TYPE_LINK | BATTLE_TYPE_OLD_MAN_TUTORIAL | BATTLE_TYPE_MULTI)) && GetMonData(mon, MON_DATA_EVOLUTION_TRACKER) < 1023)
	{
		value = min(1023, GetMonData(mon, MON_DATA_EVOLUTION_TRACKER) + upAmount);
		
		while (evolutions[0] != EVOLUTIONS_END)
		{
			if (evolutions[0] == evoMode)
			{
				switch (evolutions[0])
				{
					case EVO_REQ_USE_MOVE_X_TIMES:
					case EVO_REQ_DEFEAT_SPECIES_X_TIMES:
						if (READ_16(evolutions + 1) == data)
						{
							SetMonData(mon, MON_DATA_EVOLUTION_TRACKER, &value);
							return;
						}
						break;
					case EVO_REQ_RECOIL_DAMAGE:
						if (!IsBattlerAlive(gBattlerAttacker))
							value = 0; // Reset counter if fainted for the recoil
						
						SetMonData(mon, MON_DATA_EVOLUTION_TRACKER, &value);
						return;
				}
			}
			evolutions += gEvolutionCmdArgumentsSize[evolutions[0]] + 1;
		}
	}
}

bool8 IsMultiBattle(void)
{
    if ((gBattleTypeFlags & BATTLE_TYPE_MULTI) && (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && (gBattleTypeFlags & BATTLE_TYPE_TRAINER) && (gBattleTypeFlags & BATTLE_TYPE_LINK))
        return TRUE;
    else
        return FALSE;
}

bool8 IsDoubleBattleOnSide(u8 side)
{
	if ((gBattleTypeFlags & BATTLE_TYPE_TWO_VS_ONE) && side == B_SIDE_PLAYER)
		return FALSE;
	else
		return (gBattleTypeFlags & BATTLE_TYPE_DOUBLE);
}

bool8 IsPlayerBagDisabled(void)
{
	if ((gBattleTypeFlags & BATTLE_TYPE_LINK) || !FlagGet(FLAG_SYS_BAG_ENABLED))
		return TRUE;
	return FALSE;
}

bool8 CanTargetBattler(u8 attacker, u8 defender, u16 move, u8 moveTarget)
{
	bool8 isTargetAlly = IsBattlerAlly(attacker, defender);
	
	if (moveTarget == MOVE_TARGET_SELECTED_OPPONENT && isTargetAlly)
		return FALSE; // Can only target opponents, not allies
	else if (gBattleMoves[move].effect == EFFECT_HIT_ENEMY_HEAL_ALLY && isTargetAlly && (gStatuses3[defender] & STATUS3_HEAL_BLOCK))
		return FALSE; // Pokémon affected by Heal Block cannot target allies with Pollen Puff
	else
		return TRUE;
}

bool8 CanReceiveBadgeBoost(u8 battlerId, u16 flagId)
{
#if BADGE_BOOST
    if (!(gBattleTypeFlags & BATTLE_TYPE_LINK) && GetBattlerSide(battlerId) == B_SIDE_PLAYER && FlagGet(flagId))
		return TRUE;
#endif
    return FALSE;
}

bool8 MoveHasMoveEffect(u16 move, u8 moveEffect, bool8 self)
{
	u8 i;
	
	for (i = 0; i < gBattleMoves[move].numAdditionalEffects; i++)
	{
		if (gBattleMoves[move].additionalEffects[i].moveEffect == moveEffect && gBattleMoves[move].additionalEffects[i].self == self)
			return TRUE;
	}
	return FALSE;
}

bool8 MoveHasChargeTurnMoveEffect(u16 move)
{
	u8 i;
	
	for (i = 0; i < gBattleMoves[move].numAdditionalEffects; i++)
	{
		if (gBattleMoves[move].additionalEffects[i].onChargeTurnOnly)
			return TRUE;
	}
	return FALSE;
}

bool8 MoveIsAffectedBySheerForce(u16 move)
{
	u8 i;
	
	for (i = 0; i < gBattleMoves[move].numAdditionalEffects; i++)
	{
		if (gBattleMoves[move].additionalEffects[i].chance > 0)
			return TRUE;
	}
	return FALSE;
}

bool8 ReceiveSheerForceBoost(u8 battler, u16 move)
{
	return (GetBattlerAbility(battler) == ABILITY_SHEER_FORCE && MoveIsAffectedBySheerForce(move));
}

const struct SosCall GetSosCallRateTable(void)
{
	u8 i, chain = gBattleStruct->sos.calls;
	
	for (i = 0; i < ARRAY_COUNT(sSosCallRateTable); i++)
	{
		if (chain <= sSosCallRateTable[i].chainLength)
			break;
	}
	return sSosCallRateTable[i];
}

static u8 CalcSosBattleCallRate(u8 battlerCaller)
{
	u8 callRate = gSpeciesInfo[gBattleMons[battlerCaller].species].sosCallRate;
	
	if (callRate == 0)
		return 0; // Should never call for help
	
	if (IsBattlerTotemPokemon(battlerCaller) && gBattleStruct->battleTurnCounter == 0)
		return 100; // Totem Pokémon aways call for help on first turn
	
	switch (GetHPBarLevel(gBattleMons[battlerCaller].hp, gBattleMons[battlerCaller].maxHP))
	{
		case HP_BAR_YELLOW:
			callRate *= 3;
			break;
		case HP_BAR_RED:
			callRate *= 5;
			break;
	}
	
	if (gBattleStruct->sos.usedAdrenalineOrb)
		callRate *= 2;
	
	return callRate;
}

static u16 GetSosBattleCallRateAnswer(u8 battlerCaller, u8 playerBattler, u8 callRate)
{
	u16 answerRate;
	
	if (IsBattlerTotemPokemon(battlerCaller))
		return 100; // Totem Pokémon calls always answered
	
	answerRate = 4 * callRate; // Its 4x the call rate by default
	
	// 1.2x if the player's leading Pokémon has Intimidate, Unnerve or Pressure.
	if (GetBattlerAbility(playerBattler) == ABILITY_INTIMIDATE || GetBattlerAbility(playerBattler) == ABILITY_PRESSURE || IsUnnerveOnOpposingField(battlerCaller))
		answerRate = (answerRate * 12) / 10;
	
	// 1.5x if the wild Pokémon called for help on the previous turn and is calling again consecutively, regardless of whether the previous call was answered or not.
	if (gBattleStruct->sos.lastCallBattler == battlerCaller)
		answerRate = (answerRate * 15) / 10;
	
	// 3x if the Pokémon's most recent call for help was not answered.
	if (gBattleStruct->sos.lastCallFailed)
	{
		gBattleStruct->sos.lastCallFailed = FALSE;
		answerRate *= 3;
	}
	
	// 2x if the calling Pokémon was hit by a super effective move
	if (gSpecialStatuses[battlerCaller].hitBySuperEffective)
		answerRate *= 2;
	
	return answerRate;
}

static u16 GetSosBattleSpeciesToCall(u8 battlerCaller)
{
	u16 species, callerSpecies = gBattleMons[battlerCaller].species;
	
	if (!gSpeciesInfo[callerSpecies].sosCallAllies[0]) // No ally set
	{
#if RANDOM_SOS_CALLS
		bool8 isWaterMon;
		
		species = GetWildMonForAmbientCry(&isWaterMon); // Choose a random species from current map
#else
		species = callerSpecies; // Calls for an ally of the same species
#endif
	}
	else
	{
		do
		{
			species = gSpeciesInfo[callerSpecies].sosCallAllies[RandomMax(3)];
		} while (!species);
	}
	return species;
}

static void CreateSosAlly(u16 species, u8 level, u8 ally, u8 playerBattler)
{
	u8 newSlotPartyId = gBattlerPartyIndexes[ally];
	const struct SosCall sosCallTable = GetSosCallRateTable();
	struct PokemonGenerator generator =
	{
		.species = species,
		.level = level,
		.forcedGender = MON_GENDERLESS,
		.otIdType = OT_ID_PLAYER_ID,
		.hasFixedPersonality = FALSE,
		.fixedPersonality = 0,
		.shinyType = GENERATE_SHINY_NORMAL,
		.shinyRollType = SHINY_ROLL_SOS_CALL,
		.forcedNature = TryGetForcedWildMonNature(GetBattlerPartyIndexPtr(playerBattler), GetBattlerAbility(playerBattler)),
		.formChanges = gDefaultGeneratorFormChanges,
		.nPerfectIvs = sosCallTable.nPerfectIvs,
		.moves = {0},
	};
	CreateMon(&gEnemyParty[newSlotPartyId], generator);
	
	if (RandomPercent(sosCallTable.hiddenAbilityChance))
	{
		bool8 hidden = TRUE;
		SetMonData(&gEnemyParty[newSlotPartyId], MON_DATA_ABILITY_HIDDEN, &hidden);
	}
	CopyPokemonToBattleMon(ally, &gEnemyParty[GetPartyIdFromBattlePartyId(newSlotPartyId)], &gBattleMons[ally], TRUE);
	gBattleStruct->sides[B_SIDE_OPPONENT].hpOnSwitchout = gBattleMons[ally].hp;
	UpdateSentPokesToOpponentValue(ally);
	ClearTemporarySpeciesSpriteData(ally, FALSE);
	CalculateEnemyPartyCount();
	gBattleStruct->battlers[ally].monToSwitchIntoId = newSlotPartyId;
}

bool8 TryInitSosCall(void)
{
	u8 i, caller, ally, callRate;
	
	if (IS_WHOLE_SIDE_ALIVE(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)))
		return FALSE; // Both foes alive, can't call for help
	else if (gBattleStruct->sos.calls >= 1 && !gBattleStruct->sos.usedAdrenalineOrb)
		return FALSE; // Can only call one ally per battle, unless used an Adrenaline Orb
	
	caller = MAX_BATTLERS_COUNT;
			
	for (i = 0; i < gBattlersCount; i++)
	{
		if (IsBattlerAlive(i) && GetBattlerSide(i) == B_SIDE_OPPONENT && !IsBattlerAlive(BATTLE_PARTNER(i)))
		{
			caller = i;
			ally = BATTLE_PARTNER(i);
			break;
		}
	}
	
	if (caller != MAX_BATTLERS_COUNT)
	{
		// Can't call for help if has an status condition or is semi invulnerable
		if (!gBattleMons[caller].status1.id && !(gStatuses3[caller] & (STATUS3_SEMI_INVULNERABLE | STATUS3_COMMANDING)))
		{
			callRate = CalcSosBattleCallRate(caller);
			
			if (RandomPercent(callRate))
			{
				u8 playerBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
				u16 species = GetSosBattleSpeciesToCall(caller);
				
				gBattleStruct->sos.lastCallBattler = gEffectBattler = caller;
				gBattleScripting.battler = gBattlerFainted = ally;
				
				// Answer failed
				if (!species || !RandomPercent(GetSosBattleCallRateAnswer(caller, playerBattler, callRate)))
				{
					gBattleStruct->sos.lastCallFailed = TRUE;
					gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CALLED_FOR_HELP;
					BattleScriptExecute(BattleScript_SosCallForHelpFailed);
				}
				else
				{
					// Set some quasi-double battle params on the first call
					if (gBattleStruct->sos.calls == 0)
					{
						gBattlerPositions[B_POSITION_PLAYER_RIGHT] = B_POSITION_PLAYER_RIGHT;
						gBattlerControllerFuncs[B_POSITION_PLAYER_RIGHT] = SetControllerToPlayer;
						gAbsentBattlerFlags |= gBitTable[B_POSITION_PLAYER_RIGHT];
						
						gBattlerPositions[B_POSITION_OPPONENT_RIGHT] = B_POSITION_OPPONENT_RIGHT;
						gBattlerControllerFuncs[B_POSITION_OPPONENT_RIGHT] = SetControllerToOpponent;
						
						gBattlerPartyIndexes[ally] = 1;
						gBattleTypeFlags |= (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_VS_ONE); // If not already set
						gBattlersCount = MAX_BATTLERS_COUNT;
					}
					
					if (gBattleStruct->sos.calls < 31) // Increment num of calls for help
						gBattleStruct->sos.calls++;
					
					CreateSosAlly(species, gBattleMons[caller].level - 2 + RandomMax(5), ally, playerBattler);
					gBattleCommunication[MULTISTRING_CHOOSER] = IsBattlerTotemPokemon(caller) ? B_MSG_CALLED_ITS_ALLY : B_MSG_CALLED_FOR_HELP;
					BattleScriptExecute(BattleScript_SosCallForHelp);
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}

bool8 IsBattlerTotemPokemon(u8 battlerId)
{
	if ((gBattleTypeFlags & BATTLE_TYPE_TOTEM) && gBattleStruct->sos.totemBattlerId == battlerId)
		return TRUE;
	return FALSE;
}

static inline void SwapStructData(void *s1, void *s2, void *data, u32 size)
{
	memcpy(data, s1, size);
    memcpy(s1, s2, size);
    memcpy(s2, data, size);
}

void SwapBattlersPositions(u8 battler1, u8 battler2)
{
	u32 temp;
	u8 side = GetBattlerSide(battler1);
	void *data = Alloc(0x200);
	
	SwapStructData(&gBattleMons[battler1], &gBattleMons[battler2], data, sizeof(struct BattlePokemon));
	SwapStructData(&gDisableStructs[battler1], &gDisableStructs[battler2], data, sizeof(struct DisableStruct));
	SwapStructData(&gSpecialStatuses[battler1], &gSpecialStatuses[battler2], data, sizeof(struct SpecialStatus));
	SwapStructData(&gProtectStructs[battler1], &gProtectStructs[battler2], data, sizeof(struct ProtectStruct));
	SwapStructData(&gBattleSpritesDataPtr->battlerData[battler1], &gBattleSpritesDataPtr->battlerData[battler2], data, sizeof(struct BattleSpriteInfo));
	SwapStructData(&gBattleStruct->battlers[battler1], &gBattleStruct->battlers[battler2], data, sizeof(struct BattlerState));
	SwapStructData(&gBattleStruct->sides[side].party[gBattlerPartyIndexes[battler1]], &gBattleStruct->sides[side].party[gBattlerPartyIndexes[battler2]], data, sizeof(struct PartyState));
	
	SWAP(gTransformedPersonalities[battler1], gTransformedPersonalities[battler2], temp);
	SWAP(gTransformedShinies[battler1], gTransformedShinies[battler2], temp);
	SWAP(gStatuses3[battler1], gStatuses3[battler2], temp);
	SWAP(gPartyCriticalHits[gBattlerPartyIndexes[battler1]], gPartyCriticalHits[gBattlerPartyIndexes[battler2]], temp);
	
	if (side == B_SIDE_OPPONENT && gBattleStruct->sos.totemBattlerId != MAX_BATTLERS_COUNT) // Totem slot was swapped
		gBattleStruct->sos.totemBattlerId = BATTLE_PARTNER(gBattleStruct->sos.totemBattlerId);
}

bool8 TryBattleChallengeStartingStatus(void)
{
	bool8 effect = FALSE;
	
	switch (gBattleStruct->battleChallenge)
	{
		case TRAINER_CHALLENGE_INFINITE_TAILWIND:
			if (!(gSideStatuses[B_SIDE_OPPONENT] & SIDE_STATUS_TAILWIND))
			{
				gSideStatuses[B_SIDE_OPPONENT] |= SIDE_STATUS_TAILWIND;
				gSideTimers[B_SIDE_OPPONENT].tailwindTimer = 0;
				gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_TAILWIND_BLEW_TRAINER_TEAM;
				gBattleScripting.animArg1 = B_ANIM_TAILWIND;
				gBattlerAttacker = 1;
				effect = TRUE;
			}
			break;
	}
	return effect;
}

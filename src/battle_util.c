#include "global.h"
#include "item.h"
#include "text.h"
#include "util.h"
#include "link.h"
#include "berry.h"
#include "random.h"
#include "pokemon.h"
#include "string_util.h"
#include "field_weather.h"
#include "form_change.h"
#include "event_data.h"
#include "mail.h"
#include "mail_data.h"
#include "party_menu.h"
#include "pokedex.h"
#include "strings.h"
#include "malloc.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_scripts.h"
#include "battle_message.h"
#include "battle_controllers.h"
#include "battle_string_ids.h"
#include "battle_ai_script_commands.h"
#include "calculate_base_damage.h"
#include "set_effect.h"
#include "constants/battle_anim.h"
#include "constants/battle.h"
#include "constants/form_change.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/weather.h"
#include "constants/abilities.h"
#include "constants/pokemon.h"
#include "constants/hold_effects.h"
#include "constants/battle_move_effects.h"
#include "constants/battle_script_commands.h"
#include "constants/inserts.h"

static const u8 sTargetPositions[][2] =
{
	[B_SIDE_PLAYER] = {B_POSITION_OPPONENT_RIGHT, B_POSITION_OPPONENT_LEFT},
	[B_SIDE_OPPONENT] = {B_POSITION_PLAYER_RIGHT, B_POSITION_PLAYER_LEFT},
};

static const bool8 sIgnorableAbilities[ABILITIES_COUNT] =
{
    [ABILITY_BATTLE_ARMOR] = TRUE,
    [ABILITY_CLEAR_BODY] = TRUE,
    [ABILITY_DAMP] = TRUE,
    [ABILITY_DRY_SKIN] = TRUE,
    [ABILITY_FILTER] = TRUE,
    [ABILITY_FLASH_FIRE] = TRUE,
    [ABILITY_FLOWER_GIFT] = TRUE,
    [ABILITY_HEATPROOF] = TRUE,
    [ABILITY_HYPER_CUTTER] = TRUE,
    [ABILITY_IMMUNITY] = TRUE,
    [ABILITY_INNER_FOCUS] = TRUE,
    [ABILITY_INSOMNIA] = TRUE,
    [ABILITY_KEEN_EYE] = TRUE,
    [ABILITY_LEAF_GUARD] = TRUE,
    [ABILITY_LEVITATE] = TRUE,
    [ABILITY_LIGHTNING_ROD] = TRUE,
    [ABILITY_LIMBER] = TRUE,
    [ABILITY_MAGMA_ARMOR] = TRUE,
    [ABILITY_MARVEL_SCALE] = TRUE,
    [ABILITY_MOTOR_DRIVE] = TRUE,
    [ABILITY_OBLIVIOUS] = TRUE,
    [ABILITY_OWN_TEMPO] = TRUE,
    [ABILITY_SAND_VEIL] = TRUE,
    [ABILITY_SHELL_ARMOR] = TRUE,
    [ABILITY_SHIELD_DUST] = TRUE,
    [ABILITY_SIMPLE] = TRUE,
    [ABILITY_SNOW_CLOAK] = TRUE,
    [ABILITY_SOLID_ROCK] = TRUE,
    [ABILITY_SOUNDPROOF] = TRUE,
    [ABILITY_STICKY_HOLD] = TRUE,
    [ABILITY_STORM_DRAIN] = TRUE,
    [ABILITY_STURDY] = TRUE,
    [ABILITY_SUCTION_CUPS] = TRUE,
    [ABILITY_TANGLED_FEET] = TRUE,
    [ABILITY_THICK_FAT] = TRUE,
    [ABILITY_UNAWARE] = TRUE,
    [ABILITY_VITAL_SPIRIT] = TRUE,
    [ABILITY_VOLT_ABSORB] = TRUE,
    [ABILITY_WATER_ABSORB] = TRUE,
    [ABILITY_WATER_VEIL] = TRUE,
    [ABILITY_WHITE_SMOKE] = TRUE,
    [ABILITY_WONDER_GUARD] = TRUE,
    [ABILITY_BIG_PECKS] = TRUE,
    [ABILITY_CONTRARY] = TRUE,
    [ABILITY_FRIEND_GUARD] = TRUE,
    [ABILITY_HEAVY_METAL] = TRUE,
    [ABILITY_LIGHT_METAL] = TRUE,
    [ABILITY_MAGIC_BOUNCE] = TRUE,
    [ABILITY_MULTISCALE] = TRUE,
    [ABILITY_SAP_SIPPER] = TRUE,
    [ABILITY_TELEPATHY] = TRUE,
    [ABILITY_WONDER_SKIN] = TRUE,
    [ABILITY_AURA_BREAK] = TRUE,
    [ABILITY_AROMA_VEIL] = TRUE,
    [ABILITY_BULLETPROOF] = TRUE,
    [ABILITY_FLOWER_VEIL] = TRUE,
    [ABILITY_FUR_COAT] = TRUE,
    [ABILITY_OVERCOAT] = TRUE,
    [ABILITY_SWEET_VEIL] = TRUE,
    [ABILITY_DAZZLING] = TRUE,
    [ABILITY_DISGUISE] = TRUE,
    [ABILITY_FLUFFY] = TRUE,
    [ABILITY_QUEENLY_MAJESTY] = TRUE,
    [ABILITY_WATER_BUBBLE] = TRUE,
    [ABILITY_ICE_SCALES] = TRUE,
    [ABILITY_ICE_FACE] = TRUE,
    [ABILITY_MIRROR_ARMOR] = TRUE,
    [ABILITY_PASTEL_VEIL] = TRUE,
    [ABILITY_PUNK_ROCK] = TRUE,
    [ABILITY_ARMOR_TAIL] = TRUE,
    [ABILITY_EARTH_EATER] = TRUE,
    [ABILITY_GOOD_AS_GOLD] = TRUE,
    [ABILITY_PURIFYING_SALT] = TRUE,
    [ABILITY_WELL_BAKED_BODY] = TRUE,
    [ABILITY_WIND_RIDER] = TRUE,
};

static const u16 sWeatherFlagsInfo[][2] =
{
	[ENUM_WEATHER_RAIN] = {WEATHER_RAIN_TEMPORARY, WEATHER_RAIN_PERMANENT},
	[ENUM_WEATHER_RAIN_PRIMAL] = {WEATHER_RAIN_PRIMAL, WEATHER_RAIN_PRIMAL},
	[ENUM_WEATHER_SUN] = {WEATHER_SUN_TEMPORARY, WEATHER_SUN_PERMANENT},
	[ENUM_WEATHER_SUN_PRIMAL] = {WEATHER_SUN_PRIMAL, WEATHER_SUN_PRIMAL},
	[ENUM_WEATHER_SANDSTORM] = {WEATHER_SANDSTORM_TEMPORARY, WEATHER_SANDSTORM_PERMANENT},
	[ENUM_WEATHER_HAIL] = {WEATHER_HAIL_TEMPORARY, WEATHER_HAIL_PERMANENT},
	[ENUM_WEATHER_STRONG_WINDS] = {WEATHER_STRONG_WINDS, WEATHER_STRONG_WINDS},
	[ENUM_WEATHER_FOG] = {WEATHER_FOG_TEMPORARY, WEATHER_FOG_PERMANENT},
};

static bool8 CanBeStatused(u8 bank, bool8 checkFlowerVeil)
{
	if (gBattleMons[bank].status1 & STATUS1_ANY)
		return FALSE;
	
	switch (GetBattlerAbility(bank))
	{
		case ABILITY_LEAF_GUARD:
			if (IsBattlerWeatherAffected(bank, WEATHER_SUN_ANY))
				return FALSE;
			break;
		case ABILITY_FLOWER_VEIL:
		    if (checkFlowerVeil && IS_BATTLER_OF_TYPE(bank, TYPE_GRASS))
				return FALSE;
			break;
		case ABILITY_COMATOSE:
		case ABILITY_PURIFYING_SALT:
		    return FALSE;
	}
	if (!(gHitMarker & HITMARKER_IGNORE_SAFEGUARD))
	{
		if (IsBattlerAlive(BATTLE_PARTNER(bank)))
		{
			switch (GetBattlerAbility(BATTLE_PARTNER(bank)))
			{
				case ABILITY_FLOWER_VEIL:
				    if (checkFlowerVeil && IS_BATTLER_OF_TYPE(bank, TYPE_GRASS))
						return FALSE;
			}
		}
		if (gSideStatuses[GetBattlerSide(bank)] & SIDE_STATUS_SAFEGUARD)
			return FALSE;
	}
	return TRUE;
}

bool8 CanBePutToSleep(u8 bank, bool8 checkFlowerVeil)
{
	if (!CanBeStatused(bank, checkFlowerVeil))
		return FALSE;
	
	switch (GetBattlerAbility(bank))
	{
		case ABILITY_INSOMNIA:
		case ABILITY_VITAL_SPIRIT:
		case ABILITY_SWEET_VEIL:
			return FALSE;
	}
	if (IsBattlerAlive(BATTLE_PARTNER(bank)))
	{
		switch (GetBattlerAbility(BATTLE_PARTNER(bank)))
		{
			case ABILITY_SWEET_VEIL:
			    return FALSE;
		}
	}
	return TRUE;
}

bool8 CanBePoisoned(u8 bankDef, u8 bankAtk, bool8 checkFlowerVeil)
{
	if (!CanBeStatused(bankDef, checkFlowerVeil))
		return FALSE;
	
	switch (GetBattlerAbility(bankDef))
	{
		case ABILITY_IMMUNITY:
		case ABILITY_PASTEL_VEIL:
			return FALSE;
	}
	if (IsBattlerAlive(BATTLE_PARTNER(bankDef)))
	{
		switch (GetBattlerAbility(BATTLE_PARTNER(bankDef)))
		{
			case ABILITY_PASTEL_VEIL:
			    return FALSE;
		}
	}
	if (!CanPoisonType(bankAtk, bankDef))
		return FALSE;
	
	return TRUE;
}

bool8 CanPoisonType(u8 bankAtk, u8 bankDef)
{
	if ((GetBattlerAbility(bankAtk) == ABILITY_CORROSION && IS_MOVE_STATUS(gCurrentMove)) || !(IS_BATTLER_OF_TYPE(bankDef, TYPE_POISON) || IS_BATTLER_OF_TYPE(bankDef, TYPE_STEEL)))
		return TRUE;
	return FALSE;
}

bool8 CanBeBurned(u8 bank, bool8 checkFlowerVeil)
{
	if (!CanBeStatused(bank, checkFlowerVeil))
		return FALSE;
	
	switch (GetBattlerAbility(bank))
	{
		case ABILITY_WATER_VEIL:
		case ABILITY_WATER_BUBBLE:
		case ABILITY_THERMAL_EXCHANGE:
			return FALSE;
	}
	
	if (IS_BATTLER_OF_TYPE(bank, TYPE_FIRE))
		return FALSE;
	
	return TRUE;
}

bool8 CanBeFrozen(u8 bank, bool8 checkFlowerVeil)
{
	if (!CanBeStatused(bank, checkFlowerVeil))
		return FALSE;
	
	switch (GetBattlerAbility(bank))
	{
		case ABILITY_MAGMA_ARMOR:
			return FALSE;
	}
	
	if (IS_BATTLER_OF_TYPE(bank, TYPE_ICE))
		return FALSE;
	
	if (IsBattlerWeatherAffected(bank, WEATHER_SUN_ANY))
		return FALSE;
	
	return TRUE;
}

bool8 CanBeParalyzed(u8 bank, bool8 checkFlowerVeil)
{
	if (!CanBeStatused(bank, checkFlowerVeil))
		return FALSE;
	
	switch (GetBattlerAbility(bank))
	{
		case ABILITY_LIMBER:
			return FALSE;
	}
	
	if (IS_BATTLER_OF_TYPE(bank, TYPE_ELECTRIC))
		return FALSE;
	
	return TRUE;
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
        ret = gBattlerFainted;
        break;
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
	case BS_ACTIVE:
	    ret = gActiveBattler;
		break;
    }
    return ret;
}

void PressurePPLose(u8 target, u8 attacker, u16 move)
{
    s32 i;

    if (GetBattlerAbility(target) == ABILITY_PRESSURE)
    {
        for (i = 0; i < MAX_MON_MOVES && gBattleMons[attacker].moves[i] != move; ++i);
        if (i != MAX_MON_MOVES)
        {
            if (gBattleMons[attacker].pp[i])
                --gBattleMons[attacker].pp[i];
            if (!(gBattleMons[attacker].status2 & STATUS2_TRANSFORMED)
             && !(gDisableStructs[attacker].mimickedMoves & gBitTable[i]))
            {
                gActiveBattler = attacker;
                BtlController_EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + i, 0, 1, &gBattleMons[gActiveBattler].pp[i]);
                MarkBattlerForControllerExec(gActiveBattler);
            }
        }
    }
}

void PressurePPLoseOnUsingImprison(u8 attacker)
{
    s32 i, j;
    s32 imprisonPos = 4;
    u8 atkSide = GetBattlerSide(attacker);

    for (i = 0; i < gBattlersCount; ++i)
    {
        if (atkSide != GetBattlerSide(i) && GetBattlerAbility(i) == ABILITY_PRESSURE)
        {
            for (j = 0; j < MAX_MON_MOVES && gBattleMons[attacker].moves[j] != MOVE_IMPRISON; ++j);
            if (j != MAX_MON_MOVES)
            {
                imprisonPos = j;
                if (gBattleMons[attacker].pp[j])
                    --gBattleMons[attacker].pp[j];
            }
        }
    }
    if (imprisonPos != 4
     && !(gBattleMons[attacker].status2 & STATUS2_TRANSFORMED)
     && !(gDisableStructs[attacker].mimickedMoves & gBitTable[imprisonPos]))
    {
        gActiveBattler = attacker;
        BtlController_EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + imprisonPos, 0, 1, &gBattleMons[gActiveBattler].pp[imprisonPos]);
        MarkBattlerForControllerExec(gActiveBattler);
    }
}

void PressurePPLoseOnUsingPerishSong(u8 attacker)
{
    s32 i, j;
    s32 perishSongPos = 4;

    for (i = 0; i < gBattlersCount; ++i)
    {
        if (GetBattlerAbility(i) == ABILITY_PRESSURE && i != attacker)
        {
            for (j = 0; j < MAX_MON_MOVES && gBattleMons[attacker].moves[j] != MOVE_PERISH_SONG; ++j);
            if (j != MAX_MON_MOVES)
            {
                perishSongPos = j;
                if (gBattleMons[attacker].pp[j])
                    --gBattleMons[attacker].pp[j];
            }
        }
    }
    if (perishSongPos != MAX_MON_MOVES
     && !(gBattleMons[attacker].status2 & STATUS2_TRANSFORMED)
     && !(gDisableStructs[attacker].mimickedMoves & gBitTable[perishSongPos]))
    {
        gActiveBattler = attacker;
        BtlController_EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + perishSongPos, 0, 1, &gBattleMons[gActiveBattler].pp[perishSongPos]);
        MarkBattlerForControllerExec(gActiveBattler);
    }
}

void MarkAllBattlersForControllerExec(void)
{
    s32 i;

    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        for (i = 0; i < gBattlersCount; ++i)
            gBattleControllerExecFlags |= gBitTable[i] << 0x1C;
    else
        for (i = 0; i < gBattlersCount; ++i)
            gBattleControllerExecFlags |= gBitTable[i];
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
    s32 i;

    for (i = 0; i < GetLinkPlayerCount(); ++i)
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
    if (gProtectStructs[battler].prlzImmobility
     || gProtectStructs[battler].targetNotAffected
     || gProtectStructs[battler].usedImprisonedMove
     || gProtectStructs[battler].loveImmobility
     || gProtectStructs[battler].usedDisabledMove
     || gProtectStructs[battler].usedTauntedMove
     || gProtectStructs[battler].flinchImmobility
     || gProtectStructs[battler].confusionSelfDmg)
        return TRUE;
    else
        return FALSE;
}

// Used internaly to check if the battler's ability has't be suppressed
static u16 GetBattlerSupressedAbility(u8 battler)
{
	u16 ability = gBattleMons[battler].ability;
	
	return ability;
}

// Check Neutralizing Gas on field blocking the ability. Separated from GetBattlerSupressedAbility to prevent loop using it, bc it was called here too
static bool8 IsNeutralizingGasOnField(u16 ability)
{
	u8 i;
	bool8 foundOnField = FALSE;
	
	for (i = 0; i < gBattlersCount; i++)
	{
		if (IsBattlerAlive(i) && GetBattlerSupressedAbility(i) == ABILITY_NEUTRALIZING_GAS)
			foundOnField = TRUE;
	}
	if (foundOnField)
		foundOnField = IsAbilityBlockedByNeutralizingGas(ability);
	
	return foundOnField;
}

// Used to get the battler's ability. Passing throug all effects that can suppress/block the ability effect, like Gastro Acid and Mycelium Might
u16 GetBattlerAbility(u8 battler)
{
	u16 attackerAbility = GetBattlerSupressedAbility(gBattlerAttacker), ability = GetBattlerSupressedAbility(battler);
	
	if (IsNeutralizingGasOnField(ability))
		ability = ABILITY_NONE;
	else if (gBattlerByTurnOrder[gCurrentTurnActionNumber] == gBattlerAttacker && gActionsByTurnOrder[gBattlerByTurnOrder[gBattlerAttacker]] == B_ACTION_USE_MOVE
		&& gCurrentTurnActionNumber < gBattlersCount && sIgnorableAbilities[ability] == TRUE) // Check if gBattlerAttacker is using a move
	{
		if (attackerAbility == ABILITY_MOLD_BREAKER || attackerAbility == ABILITY_TERAVOLT || attackerAbility == ABILITY_TURBOBLAZE
		|| (attackerAbility == ABILITY_MYCELIUM_MIGHT && gProtectStructs[gBattlerAttacker].myceliumMightElevated))
			ability = ABILITY_NONE;
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
		gBattleResources->flags->flags[battlerId] &= ~(RESOURCE_FLAG_UNBURDEN_BOOST);
	
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
		gBattleResources->flags->flags[battler] |= RESOURCE_FLAG_UNBURDEN_BOOST;
}

u8 GetBattlerItemHoldEffect(u8 battler, bool8 checkNegating)
{
	u8 holdEffect;

    if (gBattleMons[battler].item == ITEM_ENIGMA_BERRY)
		holdEffect = gEnigmaBerries[battler].holdEffect;
	else	
		holdEffect = ItemId_GetHoldEffect(gBattleMons[battler].item);
	
	if (checkNegating)
	{
		if (GetBattlerAbility(battler) == ABILITY_KLUTZ)
			holdEffect = HOLD_EFFECT_NONE;
	}
	return holdEffect;
}

u8 GetBattlerHoldEffectParam(u8 battlerId)
{
	u8 holdEffectParam;

    if (gBattleMons[battlerId].item == ITEM_ENIGMA_BERRY)
		holdEffectParam = gEnigmaBerries[battlerId].holdEffectParam;
	else	
		holdEffectParam = ItemId_GetHoldEffectParam(gBattleMons[battlerId].item);
	
	return holdEffectParam;
}

static void TryActivateDefiant(u16 stringId)
{
	if (stringId == STRINGID_PKMNSSTATCHANGED4 && gSpecialStatuses[gBattlerTarget].changedStatsBattlerId != BATTLE_PARTNER(gBattlerTarget)
	&& gSpecialStatuses[gBattlerTarget].changedStatsBattlerId != gBattlerTarget)
	{
		switch (GetBattlerAbility(gBattlerTarget))
		{
			case ABILITY_DEFIANT:
			    if (CompareStat(gBattlerTarget, STAT_ATK, 12, CMP_LESS_THAN))
				{
					SET_STATCHANGER(STAT_ATK, 2, FALSE);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_DefiantCompetitive;
				}
			    break;
			case ABILITY_COMPETITIVE:
			    if (CompareStat(gBattlerTarget, STAT_SPATK, 12, CMP_LESS_THAN))
				{
					SET_STATCHANGER(STAT_SPATK, 2, FALSE);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_DefiantCompetitive;
				}
			    break;
		}
	}
}

void PrepareStringBattle(u16 stringId, u8 battler)
{
    gActiveBattler = battler;
    BtlController_EmitPrintString(0, stringId);
    MarkBattlerForControllerExec(gActiveBattler);
	TryActivateDefiant(stringId);
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
    s32 i = 0;
    u32 bits = 0;

    if (GetBattlerSide(battler) == B_SIDE_OPPONENT)
    {
        u8 flank = ((battler & BIT_FLANK) >> 1);
        gSentPokesToOpponent[flank] = 0;

        for (i = 0; i < gBattlersCount; i += 2)
            if (!(gAbsentBattlerFlags & gBitTable[i]))
                bits |= gBitTable[gBattlerPartyIndexes[i]];
        gSentPokesToOpponent[flank] = bits;
    }
}

void UpdateSentPokesToOpponentValue(u8 battler)
{
    if (GetBattlerSide(battler) == B_SIDE_OPPONENT)
    {
        OpponentSwitchInResetSentPokesToOpponentValue(battler);
    }
    else
    {
        s32 i;
        
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

u8 TrySetCantSelectMoveBattleScript(void)
{
    u8 holdEffect;
    u8 limitations = 0;
    u16 move = gBattleMons[gActiveBattler].moves[gBattleBufferB[gActiveBattler][2]];
    u16 *choicedMove = &gBattleStruct->choicedMove[gActiveBattler];

    if (gDisableStructs[gActiveBattler].disabledMove == move && move != MOVE_NONE)
    {
        gBattleScripting.battler = gActiveBattler;
        gCurrentMove = move;
        gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingDisabledMove;
        limitations = 1;
    }
    if (move == gLastMoves[gActiveBattler] && move != MOVE_STRUGGLE && (gBattleMons[gActiveBattler].status2 & STATUS2_TORMENT))
    {
        CancelMultiTurnMoves(gActiveBattler);
        gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingTormentedMove;
        ++limitations;
    }
    if (gDisableStructs[gActiveBattler].tauntTimer && !gBattleMoves[move].power)
    {
        gCurrentMove = move;
        gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingNotAllowedMoveTaunt;
        ++limitations;
    }
    if (GetImprisonedMovesCount(gActiveBattler, move))
    {
        gCurrentMove = move;
        gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingImprisonedMove;
        ++limitations;
    }
	holdEffect = GetBattlerItemHoldEffect(gActiveBattler, TRUE);
    gPotentialItemEffectBattler = gActiveBattler;
    if (holdEffect == HOLD_EFFECT_CHOICE_ITEM && *choicedMove && *choicedMove != 0xFFFF && *choicedMove != move)
    {
        gCurrentMove = *choicedMove;
        gLastUsedItem = gBattleMons[gActiveBattler].item;
        gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingNotAllowedMoveChoiceItem;
        ++limitations;
    }
    if (!gBattleMons[gActiveBattler].pp[gBattleBufferB[gActiveBattler][2]])
    {
        gSelectionBattleScripts[gActiveBattler] = BattleScript_SelectingMoveWithNoPP;
        ++limitations;
    }
    return limitations;
}

u8 CheckMoveLimitations(u8 battlerId, u8 unusableMoves, u8 check)
{
    u8 holdEffect = GetBattlerItemHoldEffect(battlerId, TRUE);
    u16 *choicedMove = &gBattleStruct->choicedMove[battlerId];
    s32 i;

    gPotentialItemEffectBattler = battlerId;

    for (i = 0; i < MAX_MON_MOVES; ++i)
    {
        if (gBattleMons[battlerId].moves[i] == 0 && check & MOVE_LIMITATION_ZEROMOVE)
            unusableMoves |= gBitTable[i];
        if (gBattleMons[battlerId].pp[i] == 0 && check & MOVE_LIMITATION_PP)
            unusableMoves |= gBitTable[i];
        if (gBattleMons[battlerId].moves[i] == gDisableStructs[battlerId].disabledMove && check & MOVE_LIMITATION_DISABLED)
            unusableMoves |= gBitTable[i];
        if (gBattleMons[battlerId].moves[i] == gLastMoves[battlerId] && check & MOVE_LIMITATION_TORMENTED && gBattleMons[battlerId].status2 & STATUS2_TORMENT)
            unusableMoves |= gBitTable[i];
        if (gDisableStructs[battlerId].tauntTimer && check & MOVE_LIMITATION_TAUNT && gBattleMoves[gBattleMons[battlerId].moves[i]].power == 0)
            unusableMoves |= gBitTable[i];
        if (GetImprisonedMovesCount(battlerId, gBattleMons[battlerId].moves[i]) && check & MOVE_LIMITATION_IMPRISON)
            unusableMoves |= gBitTable[i];
        if (gDisableStructs[battlerId].encoreTimer && gDisableStructs[battlerId].encoredMove != gBattleMons[battlerId].moves[i])
            unusableMoves |= gBitTable[i];
        if (holdEffect == HOLD_EFFECT_CHOICE_ITEM && *choicedMove != 0 && *choicedMove != 0xFFFF && *choicedMove != gBattleMons[battlerId].moves[i])
            unusableMoves |= gBitTable[i];
    }
    return unusableMoves;
}

bool8 AreAllMovesUnusable(void)
{
    u8 unusable = CheckMoveLimitations(gActiveBattler, 0, 0xFF);

    if (unusable == 0xF) // All moves are unusable.
    {
        gProtectStructs[gActiveBattler].noValidMoves = 1;
        gSelectionBattleScripts[gActiveBattler] = BattleScript_NoMovesLeft;
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            gBattleBufferB[gActiveBattler][3] = GetBattlerAtPosition((BATTLE_OPPOSITE(GetBattlerPosition(gActiveBattler))) | (Random() & 2));
        else 
            gBattleBufferB[gActiveBattler][3] = GetBattlerAtPosition(BATTLE_OPPOSITE(GetBattlerPosition(gActiveBattler)));
    }
    else
    {
        gProtectStructs[gActiveBattler].noValidMoves = 0;
    }
    return (unusable == 0xF);
}

u8 GetImprisonedMovesCount(u8 battlerId, u16 move)
{
    s32 i;
    u8 imprisonedMoves = 0;
    u8 battlerSide = GetBattlerSide(battlerId);

    for (i = 0; i < gBattlersCount; ++i)
    {
        if (battlerSide != GetBattlerSide(i) && gStatuses3[i] & STATUS3_IMPRISONED_OTHERS)
        {
            s32 j;

            for (j = 0; j < MAX_MON_MOVES && move != gBattleMons[i].moves[j]; ++j);
            if (j < MAX_MON_MOVES)
                ++imprisonedMoves;
        }
    }
    return imprisonedMoves;
}

enum
{
    ENDTURN_ORDER,
    ENDTURN_REFLECT,
    ENDTURN_LIGHT_SCREEN,
    ENDTURN_MIST,
    ENDTURN_SAFEGUARD,
    ENDTURN_WISH,
    ENDTURN_RAIN,
    ENDTURN_SANDSTORM,
    ENDTURN_SUN,
    ENDTURN_HAIL,
	ENDTURN_FOG,
    ENDTURN_FIELD_COUNT,
};

u8 DoFieldEndTurnEffects(void)
{
    u8 effect = 0;
    s32 i;

    for (gBattlerAttacker = 0; gBattlerAttacker < gBattlersCount && gAbsentBattlerFlags & gBitTable[gBattlerAttacker]; ++gBattlerAttacker);
    for (gBattlerTarget = 0; gBattlerTarget < gBattlersCount && gAbsentBattlerFlags & gBitTable[gBattlerTarget]; ++gBattlerTarget);
    do
    {
        u8 side;

        switch (gBattleStruct->turnCountersTracker)
        {
        case ENDTURN_ORDER:
            for (i = 0; i < gBattlersCount; ++i)
                gBattlerByTurnOrder[i] = i;
            for (i = 0; i < gBattlersCount - 1; ++i)
            {
                s32 j;
                
                for (j = i + 1; j < gBattlersCount; ++j)
                    if (GetWhoStrikesFirst(gBattlerByTurnOrder[i], gBattlerByTurnOrder[j], 0))
                        SwapTurnOrder(i, j);
            }
            {
                u8 *var = &gBattleStruct->turnCountersTracker;
                
                ++*var;
                gBattleStruct->turnSideTracker = 0;
            }
            // fall through
        case ENDTURN_REFLECT:
            while (gBattleStruct->turnSideTracker < B_SIDE_COUNT)
            {
                side = gBattleStruct->turnSideTracker;
                gActiveBattler = gBattlerAttacker = gSideTimers[side].reflectBattlerId;
                if (gSideStatuses[side] & SIDE_STATUS_REFLECT)
                {
                    if (--gSideTimers[side].reflectTimer == 0)
                    {
                        gSideStatuses[side] &= ~SIDE_STATUS_REFLECT;
                        BattleScriptExecute(BattleScript_SideStatusWoreOff);
                        PREPARE_MOVE_BUFFER(gBattleTextBuff1, MOVE_REFLECT);
                        ++effect;
                    }
                }
                ++gBattleStruct->turnSideTracker;
                if (effect)
                    break;
            }
            if (!effect)
            {
                ++gBattleStruct->turnCountersTracker;
                gBattleStruct->turnSideTracker = 0;
            }
            break;
        case ENDTURN_LIGHT_SCREEN:
            while (gBattleStruct->turnSideTracker < B_SIDE_COUNT)
            {
                side = gBattleStruct->turnSideTracker;
                gActiveBattler = gBattlerAttacker = gSideTimers[side].lightscreenBattlerId;
                if (gSideStatuses[side] & SIDE_STATUS_LIGHTSCREEN)
                {
                    if (--gSideTimers[side].lightscreenTimer == 0)
                    {
                        gSideStatuses[side] &= ~SIDE_STATUS_LIGHTSCREEN;
                        BattleScriptExecute(BattleScript_SideStatusWoreOff);
                        gBattleCommunication[MULTISTRING_CHOOSER] = side;
                        PREPARE_MOVE_BUFFER(gBattleTextBuff1, MOVE_LIGHT_SCREEN);
                        ++effect;
                    }
                }
                ++gBattleStruct->turnSideTracker;
                if (effect)
                    break;
            }
            if (!effect)
            {
                ++gBattleStruct->turnCountersTracker;
                gBattleStruct->turnSideTracker = 0;
            }
            break;
        case ENDTURN_MIST:
            while (gBattleStruct->turnSideTracker < B_SIDE_COUNT)
            {
                side = gBattleStruct->turnSideTracker;
                gActiveBattler = gBattlerAttacker = gSideTimers[side].mistBattlerId;
                if (gSideTimers[side].mistTimer != 0 && --gSideTimers[side].mistTimer == 0)
                {
                    gSideStatuses[side] &= ~SIDE_STATUS_MIST;
                    BattleScriptExecute(BattleScript_SideStatusWoreOff);
                    gBattleCommunication[MULTISTRING_CHOOSER] = side;
                    PREPARE_MOVE_BUFFER(gBattleTextBuff1, MOVE_MIST);
                    ++effect;
                }
                ++gBattleStruct->turnSideTracker;
                if (effect)
                    break;
            }
            if (!effect)
            {
                ++gBattleStruct->turnCountersTracker;
                gBattleStruct->turnSideTracker = 0;
            }
            break;
        case ENDTURN_SAFEGUARD:
            while (gBattleStruct->turnSideTracker < B_SIDE_COUNT)
            {
                side = gBattleStruct->turnSideTracker;
                gActiveBattler = gBattlerAttacker = gSideTimers[side].safeguardBattlerId;
                if (gSideStatuses[side] & SIDE_STATUS_SAFEGUARD)
                {
                    if (--gSideTimers[side].safeguardTimer == 0)
                    {
                        gSideStatuses[side] &= ~SIDE_STATUS_SAFEGUARD;
                        BattleScriptExecute(BattleScript_SafeguardEnds);
                        ++effect;
                    }
                }
                ++gBattleStruct->turnSideTracker;
                if (effect)
                    break;
            }
            if (!effect)
            {
                ++gBattleStruct->turnCountersTracker;
                gBattleStruct->turnSideTracker = 0;
            }
            break;
        case ENDTURN_WISH:
            while (gBattleStruct->turnSideTracker < gBattlersCount)
            {
                gActiveBattler = gBattlerByTurnOrder[gBattleStruct->turnSideTracker];
                if (gWishFutureKnock.wishCounter[gActiveBattler] != 0
                 && --gWishFutureKnock.wishCounter[gActiveBattler] == 0
                 && IsBattlerAlive(gActiveBattler))
                {
                    gBattlerTarget = gActiveBattler;
                    BattleScriptExecute(BattleScript_WishComesTrue);
                    ++effect;
                }
                ++gBattleStruct->turnSideTracker;
                if (effect)
                    break;
            }
            if (!effect)
                ++gBattleStruct->turnCountersTracker;
            break;
        case ENDTURN_RAIN:
            if (gBattleWeather & WEATHER_RAIN_ANY)
            {
                if (!(gBattleWeather & (WEATHER_RAIN_PERMANENT | WEATHER_RAIN_PRIMAL)))
                {
                    if (--gWishFutureKnock.weatherDuration == 0)
                    {
                        gBattleWeather &= ~(WEATHER_RAIN_TEMPORARY | WEATHER_RAIN_DOWNPOUR);
                        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
                    }
                    else if (gBattleWeather & WEATHER_RAIN_DOWNPOUR)
                        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                    else
                        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                }
                else if (gBattleWeather & WEATHER_RAIN_DOWNPOUR)
                {
                    gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                }
                else
                {
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                }
                BattleScriptExecute(BattleScript_RainContinuesOrEnds);
                ++effect;
            }
            ++gBattleStruct->turnCountersTracker;
            break;
        case ENDTURN_SANDSTORM:
            if (gBattleWeather & WEATHER_SANDSTORM_ANY)
            {
                if (!(gBattleWeather & WEATHER_SANDSTORM_PERMANENT) && --gWishFutureKnock.weatherDuration == 0)
                {
                    gBattleWeather &= ~WEATHER_SANDSTORM_TEMPORARY;
                    gBattlescriptCurrInstr = BattleScript_SandStormHailEnds;
                }
                else
                {
                    gBattlescriptCurrInstr = BattleScript_DamagingWeatherContinues;
                }
                gBattleScripting.animArg1 = B_ANIM_SANDSTORM_CONTINUES;
                gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                BattleScriptExecute(gBattlescriptCurrInstr);
                ++effect;
            }
            ++gBattleStruct->turnCountersTracker;
            break;
        case ENDTURN_SUN:
            if (gBattleWeather & WEATHER_SUN_ANY)
            {
                if (!(gBattleWeather & (WEATHER_SUN_PERMANENT | WEATHER_SUN_PRIMAL)) && --gWishFutureKnock.weatherDuration == 0)
                {
                    gBattleWeather &= ~WEATHER_SUN_TEMPORARY;
                    gBattlescriptCurrInstr = BattleScript_SunlightFaded;
                }
                else
                {
                    gBattlescriptCurrInstr = BattleScript_SunlightContinues;
                }
                BattleScriptExecute(gBattlescriptCurrInstr);
                ++effect;
            }
            ++gBattleStruct->turnCountersTracker;
            break;
        case ENDTURN_HAIL:
            if (gBattleWeather & WEATHER_HAIL_ANY)
            {
                if (!(gBattleWeather & WEATHER_HAIL_PERMANENT) && --gWishFutureKnock.weatherDuration == 0)
                {
                    gBattleWeather &= ~WEATHER_HAIL_TEMPORARY;
                    gBattlescriptCurrInstr = BattleScript_SandStormHailEnds;
                }
                else
                {
                    gBattlescriptCurrInstr = BattleScript_DamagingWeatherContinues;
                }
                gBattleScripting.animArg1 = B_ANIM_HAIL_CONTINUES;
                gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                BattleScriptExecute(gBattlescriptCurrInstr);
                ++effect;
            }
            ++gBattleStruct->turnCountersTracker;
            break;
		case ENDTURN_FOG:
		    if (gBattleWeather & WEATHER_FOG_ANY)
			{
				if (!(gBattleWeather & WEATHER_FOG_PERMANENT) && --gWishFutureKnock.weatherDuration == 0)
                {
                    gBattleWeather &= ~WEATHER_FOG_TEMPORARY;
                    gBattlescriptCurrInstr = BattleScript_FogEnded;
                }
                else
                {
                    gBattlescriptCurrInstr = BattleScript_FogContinues;
                }
                BattleScriptExecute(gBattlescriptCurrInstr);
                ++effect;
			}
			++gBattleStruct->turnCountersTracker;
			break;
        case ENDTURN_FIELD_COUNT:
            ++effect;
            break;
        }
    } while (!effect);
    return (gBattleMainFunc != BattleTurnPassed);
}

enum
{
    ENDTURN_INGRAIN,
    ENDTURN_ABILITIES,
    ENDTURN_ITEMS1,
    ENDTURN_LEECH_SEED,
    ENDTURN_POISON,
    ENDTURN_BAD_POISON,
    ENDTURN_BURN,
    ENDTURN_NIGHTMARES,
    ENDTURN_CURSE,
    ENDTURN_WRAP,
    ENDTURN_UPROAR,
    ENDTURN_THRASH,
    ENDTURN_DISABLE,
    ENDTURN_ENCORE,
    ENDTURN_LOCK_ON,
    ENDTURN_CHARGE,
    ENDTURN_TAUNT,
    ENDTURN_YAWN,
	ENDTURN_BAD_DREAMS,
    ENDTURN_ITEMS2,
    ENDTURN_BATTLER_COUNT
};

u8 DoBattlerEndTurnEffects(void)
{
    u8 effect = 0;

    gHitMarker |= (HITMARKER_GRUDGE | HITMARKER_SKIP_DMG_TRACK);
    while (gBattleStruct->turnEffectsBattlerId < gBattlersCount && gBattleStruct->turnEffectsTracker <= ENDTURN_BATTLER_COUNT)
    {
        gActiveBattler = gBattlerAttacker = gBattlerByTurnOrder[gBattleStruct->turnEffectsBattlerId];
        if (gAbsentBattlerFlags & gBitTable[gActiveBattler])
        {
            ++gBattleStruct->turnEffectsBattlerId;
        }
        else
        {
            switch (gBattleStruct->turnEffectsTracker)
            {
            case ENDTURN_INGRAIN:  // ingrain
                if ((gStatuses3[gActiveBattler] & STATUS3_ROOTED)
                 && gBattleMons[gActiveBattler].hp != gBattleMons[gActiveBattler].maxHP
                 && IsBattlerAlive(gActiveBattler))
                {
                    gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 16;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = 1;
                    gBattleMoveDamage *= -1;
                    BattleScriptExecute(BattleScript_IngrainTurnHeal);
                    ++effect;
                }
                ++gBattleStruct->turnEffectsTracker;
                break;
            case ENDTURN_ABILITIES:  // end turn abilities
                if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, MOVE_NONE))
                    ++effect;
                ++gBattleStruct->turnEffectsTracker;
                break;
            case ENDTURN_ITEMS1:  // item effects
                if (ItemBattleEffects(1, gActiveBattler, FALSE))
                    ++effect;
                ++gBattleStruct->turnEffectsTracker;
                break;
            case ENDTURN_ITEMS2:  // item effects again
                if (ItemBattleEffects(1, gActiveBattler, TRUE))
                    ++effect;
                ++gBattleStruct->turnEffectsTracker;
                break;
            case ENDTURN_LEECH_SEED:  // leech seed
                if ((gStatuses3[gActiveBattler] & STATUS3_LEECHSEED)
                 && IsBattlerAlive(gStatuses3[gActiveBattler] & STATUS3_LEECHSEED_BATTLER)
                 && IsBattlerAlive(gActiveBattler) && GetBattlerAbility(gActiveBattler) != ABILITY_MAGIC_GUARD)
                {
                    gBattlerTarget = gStatuses3[gActiveBattler] & STATUS3_LEECHSEED_BATTLER; // Notice gBattlerTarget is actually the HP receiver.
                    gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 8;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = 1;
                    gBattleScripting.animArg1 = gBattlerTarget;
                    gBattleScripting.animArg2 = gBattlerAttacker;
                    BattleScriptExecute(BattleScript_LeechSeedTurnDrain);
                    ++effect;
                }
                ++gBattleStruct->turnEffectsTracker;
                break;
            case ENDTURN_POISON:  // poison
                if ((gBattleMons[gActiveBattler].status1 & STATUS1_POISON) && IsBattlerAlive(gActiveBattler)
		 && GetBattlerAbility(gActiveBattler) != ABILITY_MAGIC_GUARD
		 && GetBattlerAbility(gActiveBattler) != ABILITY_POISON_HEAL)
                {
                    gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 8;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = 1;
                    BattleScriptExecute(BattleScript_PoisonTurnDmg);
                    ++effect;
                }
                ++gBattleStruct->turnEffectsTracker;
                break;
            case ENDTURN_BAD_POISON:  // toxic poison
                if ((gBattleMons[gActiveBattler].status1 & STATUS1_TOXIC_POISON) && IsBattlerAlive(gActiveBattler))
                {
			if ((gBattleMons[gActiveBattler].status1 & 0xF00) != 0xF00) // not 16 turns
				gBattleMons[gActiveBattler].status1 += 0x100;
			if (GetBattlerAbility(gActiveBattler) != ABILITY_POISON_HEAL && GetBattlerAbility(gActiveBattler) != ABILITY_MAGIC_GUARD)
			{
				gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 16;
				if (gBattleMoveDamage == 0)
					gBattleMoveDamage = 1;
				gBattleMoveDamage *= (gBattleMons[gActiveBattler].status1 & 0xF00) >> 8;
				BattleScriptExecute(BattleScript_PoisonTurnDmg);
				++effect;
			}
                }
                ++gBattleStruct->turnEffectsTracker;
                break;
            case ENDTURN_BURN:  // burn
                if ((gBattleMons[gActiveBattler].status1 & STATUS1_BURN) && IsBattlerAlive(gActiveBattler) 
		 && GetBattlerAbility(gActiveBattler) != ABILITY_MAGIC_GUARD)
                {
#if BURN_UPDATE
			gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 16;
#else
			gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 8;
#endif
			if (GetBattlerAbility(gActiveBattler) == ABILITY_HEATPROOF)
				gBattleMoveDamage /= 2;
			if (gBattleMoveDamage == 0)
				gBattleMoveDamage = 1;
			BattleScriptExecute(BattleScript_BurnTurnDmg);
			++effect;
                }
                ++gBattleStruct->turnEffectsTracker;
                break;
            case ENDTURN_NIGHTMARES:  // spooky nightmares
                if ((gBattleMons[gActiveBattler].status2 & STATUS2_NIGHTMARE) && IsBattlerAlive(gActiveBattler)
		 && GetBattlerAbility(gActiveBattler) != ABILITY_MAGIC_GUARD)
                {
                    // R/S does not perform this sleep check, which causes the nightmare effect to
                    // persist even after the affected Pokemon has been awakened by Shed Skin.
                    if (gBattleMons[gActiveBattler].status1 & STATUS1_SLEEP)
                    {
                        gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 4;
                        if (gBattleMoveDamage == 0)
                            gBattleMoveDamage = 1;
                        BattleScriptExecute(BattleScript_NightmareTurnDmg);
                        ++effect;
                    }
                    else
                    {
                        gBattleMons[gActiveBattler].status2 &= ~STATUS2_NIGHTMARE;
                    }
                }
                ++gBattleStruct->turnEffectsTracker;
                break;
            case ENDTURN_CURSE:  // curse
                if ((gBattleMons[gActiveBattler].status2 & STATUS2_CURSED) && IsBattlerAlive(gActiveBattler)
		 && GetBattlerAbility(gActiveBattler) != ABILITY_MAGIC_GUARD)
                {
                    gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 4;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = 1;
                    BattleScriptExecute(BattleScript_CurseTurnDmg);
                    ++effect;
                }
                ++gBattleStruct->turnEffectsTracker;
                break;
            case ENDTURN_WRAP:  // wrap
                if ((gBattleMons[gActiveBattler].status2 & STATUS2_WRAPPED) && IsBattlerAlive(gActiveBattler)
		 && GetBattlerAbility(gActiveBattler) != ABILITY_MAGIC_GUARD)
                {
					PREPARE_MOVE_BUFFER(gBattleTextBuff1, gBattleStruct->wrappedMove[gActiveBattler]);
                    gBattleMons[gActiveBattler].status2 -= 0x2000;
                    if (gBattleMons[gActiveBattler].status2 & STATUS2_WRAPPED)  // damaged by wrap
                    {
                        gBattleScripting.animArg1 = gBattleStruct->wrappedMove[gActiveBattler];
                        gBattleScripting.animArg2 = gBattleStruct->wrappedMove[gActiveBattler] >> 8;
                        gBattlescriptCurrInstr = BattleScript_WrapTurnDmg;
                        gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 16;
                        if (gBattleMoveDamage == 0)
                            gBattleMoveDamage = 1;
                    }
                    else  // broke free
                        gBattlescriptCurrInstr = BattleScript_WrapEnds;
						
                    BattleScriptExecute(gBattlescriptCurrInstr);
                    ++effect;
                }
                ++gBattleStruct->turnEffectsTracker;
                break;
            case ENDTURN_UPROAR:  // uproar
                if (gBattleMons[gActiveBattler].status2 & STATUS2_UPROAR)
                {
                    for (gBattlerAttacker = 0; gBattlerAttacker < gBattlersCount; ++gBattlerAttacker)
                    {
                        if ((gBattleMons[gBattlerAttacker].status1 & STATUS1_SLEEP)
                         && GetBattlerAbility(gBattlerAttacker) != ABILITY_SOUNDPROOF)
                        {
                            gBattleMons[gBattlerAttacker].status1 &= ~(STATUS1_SLEEP);
                            gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_NIGHTMARE);
                            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                            BattleScriptExecute(BattleScript_MonWokeUpInUproar);
                            gActiveBattler = gBattlerAttacker;
                            BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
                            MarkBattlerForControllerExec(gActiveBattler);
                            break;
                        }
                    }
                    if (gBattlerAttacker != gBattlersCount)
                    {
                        effect = 2;  // a pokemon was awaken
                        break;
                    }
                    else
                    {
                        gBattlerAttacker = gActiveBattler;
                        gBattleMons[gActiveBattler].status2 -= 0x10;  // uproar timer goes down
                        if (WasUnableToUseMove(gActiveBattler))
                        {
                            CancelMultiTurnMoves(gActiveBattler);
                            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                        }
                        else if (gBattleMons[gActiveBattler].status2 & STATUS2_UPROAR)
                        {
                            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                            gBattleMons[gActiveBattler].status2 |= STATUS2_MULTIPLETURNS;
                        }
                        else
                        {
                            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                            CancelMultiTurnMoves(gActiveBattler);
                        }
                        BattleScriptExecute(BattleScript_PrintUproarOverTurns);
                        effect = 1;
                    }
                }
                if (effect != 2)
                    ++gBattleStruct->turnEffectsTracker;
                break;
            case ENDTURN_THRASH:  // thrash
                if (gBattleMons[gActiveBattler].status2 & STATUS2_LOCK_CONFUSE)
                {
                    gBattleMons[gActiveBattler].status2 -= 0x400;
                    if (WasUnableToUseMove(gActiveBattler))
                        CancelMultiTurnMoves(gActiveBattler);
                    else if (!(gBattleMons[gActiveBattler].status2 & STATUS2_LOCK_CONFUSE)
                          && (gBattleMons[gActiveBattler].status2 & STATUS2_MULTIPLETURNS))
                    {
                        gBattleMons[gActiveBattler].status2 &= ~(STATUS2_MULTIPLETURNS);
                        if (!(gBattleMons[gActiveBattler].status2 & STATUS2_CONFUSION))
                        {
							SetMoveEffect(MOVE_EFFECT_CONFUSION, TRUE, FALSE);
							DoMoveEffect(TRUE);
                            if (gBattleMons[gActiveBattler].status2 & STATUS2_CONFUSION)
                                BattleScriptExecute(BattleScript_ThrashConfuses);
                            ++effect;
                        }
                    }
                }
                ++gBattleStruct->turnEffectsTracker;
                break;
            case ENDTURN_DISABLE:  // disable
                if (gDisableStructs[gActiveBattler].disableTimer != 0)
                {
                    s32 i;

                    for (i = 0; i < MAX_MON_MOVES; ++i)
                    {
                        if (gDisableStructs[gActiveBattler].disabledMove == gBattleMons[gActiveBattler].moves[i])
                            break;
                    }
                    if (i == MAX_MON_MOVES)  // pokemon does not have the disabled move anymore
                    {
                        gDisableStructs[gActiveBattler].disabledMove = MOVE_NONE;
                        gDisableStructs[gActiveBattler].disableTimer = 0;
                    }
                    else if (--gDisableStructs[gActiveBattler].disableTimer == 0)  // disable ends
                    {
                        gDisableStructs[gActiveBattler].disabledMove = MOVE_NONE;
                        BattleScriptExecute(BattleScript_DisabledNoMore);
                        ++effect;
                    }
                }
                ++gBattleStruct->turnEffectsTracker;
                break;
            case ENDTURN_ENCORE:  // encore
                if (gDisableStructs[gActiveBattler].encoreTimer != 0)
                {
                    if (gBattleMons[gActiveBattler].moves[gDisableStructs[gActiveBattler].encoredMovePos] != gDisableStructs[gActiveBattler].encoredMove)  // pokemon does not have the encored move anymore
                    {
                        gDisableStructs[gActiveBattler].encoredMove = MOVE_NONE;
                        gDisableStructs[gActiveBattler].encoreTimer = 0;
                    }
                    else if (--gDisableStructs[gActiveBattler].encoreTimer == 0
                          || gBattleMons[gActiveBattler].pp[gDisableStructs[gActiveBattler].encoredMovePos] == 0)
                    {
                        gDisableStructs[gActiveBattler].encoredMove = MOVE_NONE;
                        gDisableStructs[gActiveBattler].encoreTimer = 0;
                        BattleScriptExecute(BattleScript_EncoredNoMore);
                        ++effect;
                    }
                }
                ++gBattleStruct->turnEffectsTracker;
                break;
            case ENDTURN_LOCK_ON:  // lock-on decrement
                if (gStatuses3[gActiveBattler] & STATUS3_ALWAYS_HITS)
                    gStatuses3[gActiveBattler] -= 0x8;
                ++gBattleStruct->turnEffectsTracker;
                break;
            case ENDTURN_CHARGE:  // charge
                if (gDisableStructs[gActiveBattler].chargeTimer && --gDisableStructs[gActiveBattler].chargeTimer == 0)
                    gStatuses3[gActiveBattler] &= ~STATUS3_CHARGED_UP;
                ++gBattleStruct->turnEffectsTracker;
                break;
            case ENDTURN_TAUNT:  // taunt
                if (gDisableStructs[gActiveBattler].tauntTimer)
                    --gDisableStructs[gActiveBattler].tauntTimer;
                ++gBattleStruct->turnEffectsTracker;
                break;
            case ENDTURN_YAWN:  // yawn
                if (gStatuses3[gActiveBattler] & STATUS3_YAWN)
                {
                    gStatuses3[gActiveBattler] -= 0x800;
                    if (!(gStatuses3[gActiveBattler] & STATUS3_YAWN) && !UproarWakeUpCheck() && !(gBattleMons[gActiveBattler].status1 & STATUS1_ANY))
		    {
			    if (GetBattlerAbility(gActiveBattler) == ABILITY_VITAL_SPIRIT || GetBattlerAbility(gActiveBattler) == ABILITY_INSOMNIA
			       || (GetBattlerAbility(gActiveBattler) == ABILITY_LEAF_GUARD && IsBattlerWeatherAffected(gActiveBattler, WEATHER_SUN_ANY)))
			    {
				    ++gBattleStruct->turnEffectsTracker;
				    break;
			    }
			    CancelMultiTurnMoves(gActiveBattler);
#if SLEEP_UPDATE
			    gBattleMons[gActiveBattler].status1 |= (Random() & 2) + 1;
#else
			    gBattleMons[gActiveBattler].status1 |= (Random() & 3) + 2;
#endif
			    BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
			    MarkBattlerForControllerExec(gActiveBattler);
			    gEffectBattler = gActiveBattler;
			    BattleScriptExecute(BattleScript_YawnMakesAsleep);
			    ++effect;
		    }
		}
                ++gBattleStruct->turnEffectsTracker;
                break;
			case ENDTURN_BAD_DREAMS:
			    if (GetBattlerAbility(gActiveBattler) == ABILITY_BAD_DREAMS)
				{
					BattleScriptExecute(BattleScript_BadDreamsActivates);
					++effect;
				}
				++gBattleStruct->turnEffectsTracker;
                break;
            case ENDTURN_BATTLER_COUNT:  // done
                gBattleStruct->turnEffectsTracker = 0;
                ++gBattleStruct->turnEffectsBattlerId;
                break;
            }
            if (effect)
                return effect;
        }
    }
    gHitMarker &= ~(HITMARKER_GRUDGE | HITMARKER_SKIP_DMG_TRACK);
    return 0;
}

bool8 HandleWishPerishSongOnTurnEnd(void)
{
    gHitMarker |= (HITMARKER_GRUDGE | HITMARKER_SKIP_DMG_TRACK);
    switch (gBattleStruct->wishPerishSongState)
    {
    case 0:
        while (gBattleStruct->wishPerishSongBattlerId < gBattlersCount)
        {
            gActiveBattler = gBattleStruct->wishPerishSongBattlerId;
            if (gAbsentBattlerFlags & gBitTable[gActiveBattler])
            {
                ++gBattleStruct->wishPerishSongBattlerId;
                continue;
            }
            ++gBattleStruct->wishPerishSongBattlerId;
            if (gWishFutureKnock.futureSightCounter[gActiveBattler] != 0
             && --gWishFutureKnock.futureSightCounter[gActiveBattler] == 0
             && gBattleMons[gActiveBattler].hp != 0)
            {
                if (gWishFutureKnock.futureSightMove[gActiveBattler] == MOVE_FUTURE_SIGHT)
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                else
                    gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                PREPARE_MOVE_BUFFER(gBattleTextBuff1, gWishFutureKnock.futureSightMove[gActiveBattler]);
		gCurrentMove = gWishFutureKnock.futureSightMove[gActiveBattler];
		gBattleStruct->dynamicMoveType = gBattleMoves[gCurrentMove].type;
                gBattlerTarget = gActiveBattler;
                gBattlerAttacker = gWishFutureKnock.futureSightAttacker[gActiveBattler];
                gBattleMoveDamage = gWishFutureKnock.futureSightDmg[gActiveBattler];
                gSpecialStatuses[gBattlerTarget].dmg = 0xFFFF;
                BattleScriptExecute(BattleScript_MonTookFutureAttack);
                return TRUE;
            }
        }
        {
            u8 *state = &gBattleStruct->wishPerishSongState;

            *state = 1;
            gBattleStruct->wishPerishSongBattlerId = 0;
        }
        // fall through
    case 1:
        while (gBattleStruct->wishPerishSongBattlerId < gBattlersCount)
        {
            gActiveBattler = gBattlerAttacker = gBattlerByTurnOrder[gBattleStruct->wishPerishSongBattlerId];
            if (gAbsentBattlerFlags & gBitTable[gActiveBattler])
            {
                ++gBattleStruct->wishPerishSongBattlerId;
                continue;
            }
            ++gBattleStruct->wishPerishSongBattlerId;
            if (gStatuses3[gActiveBattler] & STATUS3_PERISH_SONG)
            {
                PREPARE_BYTE_NUMBER_BUFFER(gBattleTextBuff1, 1, gDisableStructs[gActiveBattler].perishSongTimer);
                if (gDisableStructs[gActiveBattler].perishSongTimer == 0)
                {
                    gStatuses3[gActiveBattler] &= ~STATUS3_PERISH_SONG;
                    gBattleMoveDamage = gBattleMons[gActiveBattler].hp;
                    gBattlescriptCurrInstr = BattleScript_PerishSongTakesLife;
                }
                else
                {
                    --gDisableStructs[gActiveBattler].perishSongTimer;
                    gBattlescriptCurrInstr = BattleScript_PerishSongCountGoesDown;
                }
                BattleScriptExecute(gBattlescriptCurrInstr);
                return TRUE;
            }
        }
        break;
    }
    gHitMarker &= ~(HITMARKER_GRUDGE | HITMARKER_SKIP_DMG_TRACK);
    return FALSE;
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
                if (gAbsentBattlerFlags & gBitTable[i] && !HasNoMonsToSwitch(i, PARTY_SIZE, PARTY_SIZE))
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
        if ((gBattleMons[i].status2 & STATUS2_RAGE) && gChosenMoveByBattler[i] != MOVE_RAGE)
            gBattleMons[i].status2 &= ~(STATUS2_RAGE);
}

static void SetRandomMultiHitCounter(void)
{
	gMultiHitCounter = Random() % 100;
	
	if (gMultiHitCounter < 35)
		gMultiHitCounter = 2;
	else if (gMultiHitCounter < 35 + 35)
		gMultiHitCounter = 3;
	else if (gMultiHitCounter < 35 + 35 + 15)
		gMultiHitCounter = 4;
	else
		gMultiHitCounter = 5;
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
				gBattleStruct->targetsDone[gBattlerAttacker] = 0;
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_ASLEEP: // check being asleep
			    if (gBattleMons[gBattlerAttacker].status1 & STATUS1_SLEEP)
				{
					if (UproarWakeUpCheck())
					{
						gBattleCommunication[MULTISTRING_CHOOSER] = 1;
						ClearBattlerStatus(gBattlerAttacker);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_MoveUsedWokeUp;
						++effect;
					}
					else
					{
						u8 toSub = GetBattlerAbility(gBattlerAttacker) == ABILITY_EARLY_BIRD ? 2 : 1;
						
						if ((gBattleMons[gBattlerAttacker].status1 & STATUS1_SLEEP) < toSub)
						{
							gBattleCommunication[MULTISTRING_CHOOSER] = 0;
							ClearBattlerStatus(gBattlerAttacker);
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_MoveUsedWokeUp;
							++effect;
						}
						else
						{
							gBattleMons[gBattlerAttacker].status1 -= toSub;
							
							if (gCurrentMove != MOVE_SNORE && gCurrentMove != MOVE_SLEEP_TALK)
							{
								gBattlescriptCurrInstr = BattleScript_MoveUsedIsAsleep;
								gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
								++effect;
							}
						}
					}
                }
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_FROZEN: // check being frozen
			    // unfreezing via a move effect happens in CANCELLER_THAW
			    if (gBattleMons[gBattlerAttacker].status1 & STATUS1_FREEZE && !gBattleMoves[gCurrentMove].flags.thawUser)
				{
					if (Random() % 5)
					{
						gBattlescriptCurrInstr = BattleScript_MoveUsedIsFrozen;
						gHitMarker |= HITMARKER_NO_ATTACKSTRING;
					}
					else // unfreeze
					{
						gBattleCommunication[MULTISTRING_CHOOSER] = 0;
						ClearBattlerStatus(gBattlerAttacker);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_MoveUsedUnfroze;
					}
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_TRUANT: // truant
			    if (GetBattlerAbility(gBattlerAttacker) == ABILITY_TRUANT && gDisableStructs[gBattlerAttacker].truantCounter)
				{
					gMoveResultFlags |= MOVE_RESULT_MISSED;
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
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
					gProtectStructs[gBattlerAttacker].flinchImmobility = 1;
					gBattlescriptCurrInstr = BattleScript_MoveUsedFlinched;
					CancelMultiTurnMoves(gBattlerAttacker);
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_DISABLED: // disabled move
			    if (gDisableStructs[gBattlerAttacker].disabledMove == gCurrentMove && gDisableStructs[gBattlerAttacker].disabledMove)
				{
					gBattleScripting.battler = gBattlerAttacker;
					gProtectStructs[gBattlerAttacker].usedDisabledMove = 1;
					gBattlescriptCurrInstr = BattleScript_MoveUsedIsDisabled;
					CancelMultiTurnMoves(gBattlerAttacker);
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_TAUNTED: // taunt
			    if (gDisableStructs[gBattlerAttacker].tauntTimer && IS_MOVE_STATUS(gCurrentMove))
				{
					gProtectStructs[gBattlerAttacker].usedTauntedMove = 1;
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
					gProtectStructs[gBattlerAttacker].usedImprisonedMove = 1;
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
					--gBattleMons[gBattlerAttacker].status2;
					
					if (gBattleMons[gBattlerAttacker].status2 & STATUS2_CONFUSION)
					{
#if CONFUSION_UPDATE
                        if (Random() % 3)
#else
		                if (Random() & 1)
#endif
                        {
							gBattleCommunication[MULTISTRING_CHOOSER] = 0;
							BattleScriptPushCursor();
						}
						else // confusion dmg
						{
							gBattleCommunication[MULTISTRING_CHOOSER] = 1;
							gBattlerTarget = gBattlerAttacker;
							gBattleMoveDamage = CalculateBaseDamage(MOVE_NONE, TYPE_MYSTERY, gBattlerAttacker, gBattlerTarget, FALSE, TRUE);
							gProtectStructs[gBattlerAttacker].confusionSelfDmg = 1;
							gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
						}
						gBattlescriptCurrInstr = BattleScript_MoveUsedIsConfused;
					}
					else // snapped out of confusion
					{
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_MoveUsedIsConfusedNoMore;
					}
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_PARALYSED: // paralysis
			    if ((gBattleMons[gBattlerAttacker].status1 & STATUS1_PARALYSIS) && (Random() % 4) == 0)
				{
					gProtectStructs[gBattlerAttacker].prlzImmobility = 1;
					gBattlescriptCurrInstr = BattleScript_MoveUsedIsParalyzed;
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_GHOST: // GHOST in pokemon tower
                if (IS_BATTLE_TYPE_GHOST_WITHOUT_SCOPE())
				{
					if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
						gBattlescriptCurrInstr = BattleScript_TooScaredToMove;
					else
						gBattlescriptCurrInstr = BattleScript_GhostGetOutGetOut;
					
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_IN_LOVE: // infatuation
			    if (gBattleMons[gBattlerAttacker].status2 & STATUS2_INFATUATION)
				{
					gBattleScripting.battler = CountTrailingZeroBits((gBattleMons[gBattlerAttacker].status2 & STATUS2_INFATUATION) >> 0x10);
					
					if (Random() & 1)
						BattleScriptPushCursor();
					else
					{
						BattleScriptPush(BattleScript_MoveUsedIsInLoveCantAttack);
						gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
						gProtectStructs[gBattlerAttacker].loveImmobility = 1;
						CancelMultiTurnMoves(gBattlerAttacker);
					}
					gBattlescriptCurrInstr = BattleScript_MoveUsedIsInLove;
					++effect;
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_BIDE: // bide
			    if (gBattleMons[gBattlerAttacker].status2 & STATUS2_BIDE)
				{
					gBattleMons[gBattlerAttacker].status2 -= 0x100;
					
					if (gBattleMons[gBattlerAttacker].status2 & STATUS2_BIDE)
						gBattlescriptCurrInstr = BattleScript_BideStoringEnergy;
					else
					{
						if (gTakenDmg[gBattlerAttacker])
						{
							gCurrentMove = MOVE_BIDE;
							gBattleScripting.bideDmg = gTakenDmg[gBattlerAttacker] * 2;
							gBattlerTarget = gTakenDmgByBattler[gBattlerAttacker];
							
							if (gAbsentBattlerFlags & gBitTable[gBattlerTarget])
								gBattlerTarget = GetMoveTarget(MOVE_BIDE, 1);
							
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
			    if (gBattleMons[gBattlerAttacker].status1 & STATUS1_FREEZE && gBattleMoves[gCurrentMove].flags.thawUser)
				{
					ClearBattlerStatus(gBattlerAttacker);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_MoveUsedUnfroze;
					gBattleCommunication[MULTISTRING_CHOOSER] = 1;
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
							if (IsBattlerWeatherAffected(gBattlerAttacker, WEATHER_RAIN_PRIMAL))
							{
								gBattlescriptCurrInstr = BattleScript_PrimordialSeaFizzlesOutFireTypeMoves;
								++effect;
							}
							break;
						case TYPE_WATER:
							if (IsBattlerWeatherAffected(gBattlerAttacker, WEATHER_SUN_PRIMAL))
							{
								gBattlescriptCurrInstr = BattleScript_DesolateLandEvaporatesWaterTypeMoves;
								++effect;
							}
							break;
					}
					if (effect)
					{
						CancelMultiTurnMoves(gBattlerAttacker);
						gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
						gMoveResultFlags = MOVE_RESULT_FAILED;
					}
				}
				++gBattleStruct->atkCancellerTracker;
				break;
			case CANCELLER_SPORE_MOVE:
			    if (gBattlerAttacker != gBattlerTarget && gBattleMoves[gCurrentMove].flags.powderMove && !IsBattlerAffectedBySpore(gBattlerTarget))
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
						gLastUsedAbility = GetBattlerAbility(i);
						RecordAbilityBattle(i, gLastUsedAbility);
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
						if (GetBattlerAbility(gBattlerAttacker) == ABILITY_SKILL_LINK)
							gMultiHitCounter = 5;
						else
							SetRandomMultiHitCounter();
						
						PREPARE_BYTE_NUMBER_BUFFER(gBattleScripting.multihitString, 1, 0);
					}
					else if (gBattleMoves[gCurrentMove].effect == EFFECT_BEAT_UP)
					{
						u8 i;
						struct Pokemon *party = GetBattlerParty(gBattlerAttacker);
						
						for (i = 0; i < PARTY_SIZE; i++)
						{
							if (GetMonData(&party[i], MON_DATA_HP) && GetMonData(&party[i], MON_DATA_SPECIES) && !GetMonData(&party[i], MON_DATA_IS_EGG)
								&& !GetMonData(&party[i], MON_DATA_STATUS))
							    ++gMultiHitCounter;
						}
						gBattleCommunication[MULTIUSE_STATE] = 0; // For later
						PREPARE_BYTE_NUMBER_BUFFER(gBattleScripting.multihitString, 1, 0);
					}
					else if (gBattleMoves[gCurrentMove].flags.twoStrikes)
					{
						gMultiHitCounter = 2;
						PREPARE_BYTE_NUMBER_BUFFER(gBattleScripting.multihitString, 1, 0);
					}
					else if (gBattleMoves[gCurrentMove].flags.threeStrikes)
					{
						gBattleScripting.tripleKickPower = 0;
						gMultiHitCounter = 3;
						PREPARE_BYTE_NUMBER_BUFFER(gBattleScripting.multihitString, 1, 0);
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
    u8 playerId, flankId;
    struct Pokemon *party;
    s32 i;

    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
    {
        return FALSE;
    }
    else
    {
		party = GetBattlerParty(battler);
		
        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
        {
            playerId = GetBattlerMultiplayerId(battler);
            flankId = GetLinkTrainerFlankId(playerId);
			
            for (i = flankId * 3; i < flankId * 3 + 3; ++i)
            {
                if (GetMonData(&party[i], MON_DATA_HP) != 0
                 && GetMonData(&party[i], MON_DATA_SPECIES2) != SPECIES_NONE
                 && GetMonData(&party[i], MON_DATA_SPECIES2) != SPECIES_EGG)
                    break;
            }
            return (i == flankId * 3 + 3);
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
                if (GetMonData(&party[i], MON_DATA_HP) != 0
                 && GetMonData(&party[i], MON_DATA_SPECIES2) != SPECIES_NONE
                 && GetMonData(&party[i], MON_DATA_SPECIES2) != SPECIES_EGG
                 && i != partyIdBattlerOn1
                 && i != partyIdBattlerOn2
                 && i != gBattleStruct->monToSwitchIntoId[flankId]
                 && i != playerId[gBattleStruct->monToSwitchIntoId])
                    break;
            }
            return (i == PARTY_SIZE);
        }
    }
}

void ClearBattlerStatus(u8 battler)
{
	if (gBattleMons[battler].status1 & STATUS1_PSN_ANY)
		StringCopy(gBattleTextBuff1, gStatusConditionString_PoisonJpn);
	else if (gBattleMons[battler].status1 & STATUS1_PARALYSIS)
		StringCopy(gBattleTextBuff1, gStatusConditionString_ParalysisJpn);
	else if (gBattleMons[battler].status1 & STATUS1_BURN)
		StringCopy(gBattleTextBuff1, gStatusConditionString_BurnJpn);
	else if (gBattleMons[battler].status1 & STATUS1_FREEZE)
		StringCopy(gBattleTextBuff1, gStatusConditionString_IceJpn);
	else if (gBattleMons[battler].status1 & STATUS1_SLEEP)
		StringCopy(gBattleTextBuff1, gStatusConditionString_SleepJpn);
	
	gBattleMons[battler].status1 = 0;
	gBattleMons[battler].status2 &= ~(STATUS2_NIGHTMARE);  // fix nightmare glitch
	gBattleScripting.battler = gActiveBattler = battler;
	BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[battler].status1);
	MarkBattlerForControllerExec(gActiveBattler);
}

static bool8 IsAbilityBlockedByGhostBattle(u16 ability)
{
	if (IS_BATTLE_TYPE_GHOST_WITHOUT_SCOPE())
	{
		if (ability == ABILITY_INTIMIDATE || ability == ABILITY_TRACE || ability == ABILITY_ANTICIPATION || ability == ABILITY_DOWNLOAD
		|| ability == ABILITY_FOREWARN || ability == ABILITY_FRISK || ability == ABILITY_UNNERVE || ability == ABILITY_IMPOSTER)
			return TRUE;
	}
	return FALSE;
}

static bool8 IsBattlerAbilityCopyableByTrace(u8 battlerId)
{
	switch (gBattleMons[battlerId].ability)
	{
		case ABILITY_TRACE:
		case ABILITY_ZEN_MODE:
		case ABILITY_AS_ONE_ICE_RIDER:
		case ABILITY_AS_ONE_SHADOW_RIDER:
		case ABILITY_BATTLE_BOND:
		case ABILITY_COMATOSE:
		case ABILITY_DISGUISE:
		case ABILITY_FLOWER_GIFT:
		case ABILITY_FORECAST:
		case ABILITY_GULP_MISSILE:
		case ABILITY_HUNGER_SWITCH:
		case ABILITY_ICE_FACE:
		case ABILITY_ILLUSION:
		case ABILITY_IMPOSTER:
		case ABILITY_MULTITYPE:
		case ABILITY_NEUTRALIZING_GAS:
		case ABILITY_POWER_CONSTRUCT:
		case ABILITY_POWER_OF_ALCHEMY:
		case ABILITY_RECEIVER:
		case ABILITY_RKS_SYSTEM:
		case ABILITY_SCHOOLING:
		case ABILITY_SHIELDS_DOWN:
		case ABILITY_STANCE_CHANGE:
		    return FALSE;
	}
	return TRUE;
}

static bool8 MoveHasFlinchChance(u16 move)
{
	switch (gBattleMoves[move].effect)
	{
		case EFFECT_TWO_TURNS_ATTACK:
		    if (gBattleMoves[move].argument != MOVE_EFFECT_FLINCH)
				break;
		// fallthrough
		case EFFECT_FLINCH_HIT:
		case EFFECT_FLINCH_STATUS:
		case EFFECT_FAKE_OUT:
		case EFFECT_FLING:
		case EFFECT_SECRET_POWER:
		case EFFECT_SNORE:
		    return TRUE;
	}
	return FALSE;
}

#define IS_MULTIHIT_FINAL_STRIKE (gMultiHitCounter <= 1)

u8 AbilityBattleEffects(u8 caseId, u8 battler, u16 moveArg)
{
    u8 moveType, effect = 0, moveTarget;
	s32 i;

    if (!(gBattleTypeFlags & BATTLE_TYPE_SAFARI))
	{
		gLastUsedAbility = GetBattlerAbility(battler);
		
		if (gBattlerAttacker >= gBattlersCount)
			gBattlerAttacker = battler;
		
		if (gBattlerTarget >= gBattlersCount)
			gBattlerTarget = battler;
		
		if (IsAbilityBlockedByGhostBattle(gLastUsedAbility))
			return effect;
		
		if (!moveArg)
            moveArg = gCurrentMove;
		
		moveType = gBattleStruct->dynamicMoveType;
		
		switch (caseId)
		{
			case ABILITYEFFECT_SWITCH_IN_WEATHER: // not an actual ability effect, but handled here.
			    switch (GetCurrentWeather())
				{
					case WEATHER_RAIN:
					case WEATHER_RAIN_THUNDERSTORM:
					case WEATHER_DOWNPOUR:
					    if (!(gBattleWeather & WEATHER_RAIN_ANY))
						{
							gBattleWeather = (WEATHER_RAIN_TEMPORARY | WEATHER_RAIN_PERMANENT);
							gBattleScripting.animArg1 = B_ANIM_RAIN_CONTINUES;
							++effect;
						}
						break;
					case WEATHER_SANDSTORM:
					    if (!(gBattleWeather & WEATHER_SANDSTORM_ANY))
						{
							gBattleWeather = (WEATHER_SANDSTORM_PERMANENT | WEATHER_SANDSTORM_TEMPORARY);
							gBattleScripting.animArg1 = B_ANIM_SANDSTORM_CONTINUES;
							++effect;
						}
						break;
					case WEATHER_SNOW:
					    if (!(gBattleWeather & WEATHER_HAIL_ANY))
						{
							gBattleWeather = (WEATHER_HAIL_PERMANENT | WEATHER_HAIL_TEMPORARY);
							gBattleScripting.animArg1 = B_ANIM_HAIL_CONTINUES;
							++effect;
						}
						break;
#if FOG_IN_BATTLE
					case WEATHER_FOG_HORIZONTAL:
					case WEATHER_FOG_DIAGONAL:
				        if (!(gBattleWeather & WEATHER_FOG_ANY))
						{
							gBattleWeather = (WEATHER_FOG_PERMANENT | WEATHER_FOG_TEMPORARY);
							gBattleScripting.animArg1 = B_ANIM_FOG_CONTINUES;
							++effect;
						}
						break;
#endif
				}
				if (effect)
				{
					gBattleScripting.battler = battler;
					gBattleCommunication[MULTISTRING_CHOOSER] = GetCurrentWeather();
                    BattleScriptPushCursorAndCallback(BattleScript_OverworldWeatherStarts);
				}
				break;
			case ABILITYEFFECT_ON_SWITCHIN:
			    gBattleScripting.battler = battler;
			
			    switch (gLastUsedAbility)
				{
					case ABILITY_DRIZZLE:
					    if (TryChangeBattleWeather(battler, ENUM_WEATHER_RAIN))
						{
							gBattleCommunication[MULTISTRING_CHOOSER] = 0;
							gBattleScripting.animArg1 = B_ANIM_RAIN_CONTINUES;
							BattleScriptPushCursorAndCallback(BattleScript_SwitchInWeatherAbilityActivates);
							++effect;
						}
						break;
					case ABILITY_SAND_STREAM:
					    if (TryChangeBattleWeather(battler, ENUM_WEATHER_SANDSTORM))
						{
							gBattleCommunication[MULTISTRING_CHOOSER] = 1;
							gBattleScripting.animArg1 = B_ANIM_SANDSTORM_CONTINUES;
							BattleScriptPushCursorAndCallback(BattleScript_SwitchInWeatherAbilityActivates);
							++effect;
						}
						break;
					case ABILITY_DROUGHT:
					    if (TryChangeBattleWeather(battler, ENUM_WEATHER_SUN))
						{
							gBattleCommunication[MULTISTRING_CHOOSER] = 2;
							gBattleScripting.animArg1 = B_ANIM_SUN_CONTINUES;
							BattleScriptPushCursorAndCallback(BattleScript_SwitchInWeatherAbilityActivates);
							++effect;
						}
						break;
					case ABILITY_SNOW_WARNING:
					    if (TryChangeBattleWeather(battler, ENUM_WEATHER_HAIL))
						{
							gBattleCommunication[MULTISTRING_CHOOSER] = 3;
							gBattleScripting.animArg1 = B_ANIM_HAIL_CONTINUES;
							BattleScriptPushCursorAndCallback(BattleScript_SwitchInWeatherAbilityActivates);
							++effect;
						}
						break;
					case ABILITY_PRIMORDIAL_SEA:
					    if (TryChangeBattleWeather(battler, ENUM_WEATHER_RAIN_PRIMAL))
						{
							gBattleCommunication[MULTISTRING_CHOOSER] = 4;
							gBattleScripting.animArg1 = B_ANIM_RAIN_CONTINUES;
							BattleScriptPushCursorAndCallback(BattleScript_SwitchInWeatherAbilityActivates);
							++effect;
						}
						break;
					case ABILITY_DESOLATE_LAND:
					    if (TryChangeBattleWeather(battler, ENUM_WEATHER_SUN_PRIMAL))
						{
							gBattleCommunication[MULTISTRING_CHOOSER] = 5;
							gBattleScripting.animArg1 = B_ANIM_SUN_CONTINUES;
							BattleScriptPushCursorAndCallback(BattleScript_SwitchInWeatherAbilityActivates);
							++effect;
						}
						break;
					case ABILITY_DELTA_STREAM:
					    if (TryChangeBattleWeather(battler, ENUM_WEATHER_STRONG_WINDS))
						{
							gBattleCommunication[MULTISTRING_CHOOSER] = 6;
							gBattleScripting.animArg1 = B_ANIM_RAIN_CONTINUES; // assign it an apropriated animation
							BattleScriptPushCursorAndCallback(BattleScript_SwitchInWeatherAbilityActivates);
							++effect;
						}
						break;
					case ABILITY_INTIMIDATE:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							gBattlerAttacker = battler;
							BattleScriptPushCursorAndCallback(BattleScript_IntimidateActivates);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						break;
					case ABILITY_TRACE:
					    if (!gSpecialStatuses[battler].traced)
						{
							gBattleResources->flags->flags[battler] |= RESOURCE_FLAG_TRACED;
							gSpecialStatuses[battler].traced = TRUE;
						}
						break;
					case ABILITY_CLOUD_NINE:
					case ABILITY_AIR_LOCK:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							BattleScriptPushCursorAndCallback(BattleScript_AirLock);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
					    break;
					case ABILITY_MOLD_BREAKER:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							gBattleCommunication[MULTISTRING_CHOOSER] = 0;
							BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						break;
					case ABILITY_SLOW_START:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							gDisableStructs[battler].slowStartTimer = 5;
							gBattleCommunication[MULTISTRING_CHOOSER] = 1;
							BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						break;
					case ABILITY_IMPOSTER:
					    if (IsBattlerAlive(BATTLE_OPPOSITE(battler)) && !(gBattleMons[BATTLE_OPPOSITE(battler)].status2 & (STATUS2_TRANSFORMED | STATUS2_SUBSTITUTE))
						&& !(gStatuses3[BATTLE_OPPOSITE(battler)] & STATUS3_SEMI_INVULNERABLE) && !gBattleStruct->illusion[BATTLE_OPPOSITE(battler)].on
						&& !(gBattleMons[battler].status2 & STATUS2_TRANSFORMED) && !gDisableStructs[battler].imposterActivated)
						{
							gDisableStructs[battler].imposterActivated = TRUE;
							gBattlerAttacker = battler;
							gBattlerTarget = BATTLE_OPPOSITE(battler);
							BattleScriptPushCursorAndCallback(BattleScript_ImposterActivates);
							++effect;
						}
						break;
					case ABILITY_TURBOBLAZE:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							gBattleCommunication[MULTISTRING_CHOOSER] = 3;
							BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						break;
					case ABILITY_TERAVOLT:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							gBattleCommunication[MULTISTRING_CHOOSER] = 4;
							BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						break;
					case ABILITY_DARK_AURA:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							gBattleCommunication[MULTISTRING_CHOOSER] = 5;
							BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						break;
					case ABILITY_FAIRY_AURA:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							gBattleCommunication[MULTISTRING_CHOOSER] = 6;
							BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						break;
					case ABILITY_AURA_BREAK:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							gBattleCommunication[MULTISTRING_CHOOSER] = 7;
							BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						break;
					case ABILITY_COMATOSE:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							gBattleCommunication[MULTISTRING_CHOOSER] = 8;
							BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						break;
					case ABILITY_PRESSURE:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							gBattleCommunication[MULTISTRING_CHOOSER] = 16;
							BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						break;
					case ABILITY_VESSEL_OF_RUIN:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_SPATK);
							gBattleCommunication[MULTISTRING_CHOOSER] = 18;
							BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						break;
					case ABILITY_SWORD_OF_RUIN:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_DEF);
							gBattleCommunication[MULTISTRING_CHOOSER] = 18;
							BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						break;
					case ABILITY_TABLETS_OF_RUIN:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_ATK);
							gBattleCommunication[MULTISTRING_CHOOSER] = 18;
							BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						break;
					case ABILITY_BEADS_OF_RUIN:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_SPDEF);
							gBattleCommunication[MULTISTRING_CHOOSER] = 18;
							BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						break;
					case ABILITY_ZERO_TO_HERO:
					    if (gBattleStruct->zeroToHeroActivated[GetBattlerSide(battler)] & gBitTable[gBattlerPartyIndexes[battler]])
						{
							gBattleStruct->zeroToHeroActivated[GetBattlerSide(battler)] &= ~(gBitTable[gBattlerPartyIndexes[battler]]);
							gBattleCommunication[MULTISTRING_CHOOSER] = 9;
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
								gBattleCommunication[MULTISTRING_CHOOSER] = 0;
								DoBattleFormChange(battler, newSpecies, FALSE, FALSE);
								BattleScriptPushCursorAndCallback(BattleScript_SchoolingActivatesPause);
								++effect;
							}
						}
						break;
					case ABILITY_ZEN_MODE:
					    {
							u16 newSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_HP);
							
							if (newSpecies)
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = 0;
								DoBattleFormChange(battler, newSpecies, TRUE, TRUE);
								BattleScriptPushCursorAndCallback(BattleScript_ZenModeActivatesPause);
								++effect;
							}
					    }
						break;
					case ABILITY_ANTICIPATION:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							u8 j, affectedBy;
							
							for (i = 0; i < MAX_BATTLERS_COUNT; i++)
							{
								if (IsBattlerAlive(i) && GetBattlerSide(i) != GetBattlerSide(battler))
								{
									for (j = 0, affectedBy = 0; j < MAX_MON_MOVES; j++, affectedBy = 0)
									{
										moveArg = gBattleMons[i].moves[j];
										
										if (gBattleMoves[moveArg].effect == EFFECT_OHKO)
											++effect;
										else if (moveArg && !IS_MOVE_STATUS(moveArg))
										{
											if (gBattleMoves[moveArg].effect == EFFECT_HIDDEN_POWER)
												moveType = GetHiddenPowerType(GetBattlerPartyIndexPtr(i));
											else
												moveType = gBattleMoves[moveArg].type;
											
											if (TypeCalc(moveArg, moveType, i, battler, FALSE, FALSE, &affectedBy) & MOVE_RESULT_SUPER_EFFECTIVE)
												++effect;
										}
									}
								}
							}
							if (effect)
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = 10;
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
							
							for (i = 0; i < (MAX_BATTLERS_COUNT / 2); i++, opposingBattler = BATTLE_PARTNER(opposingBattler))
							{
								if (IsBattlerAlive(opposingBattler))
								{
									APPLY_STAT_MOD(var, &gBattleMons[opposingBattler], gBattleMons[opposingBattler].defense, STAT_DEF);
									def += var;
									
									APPLY_STAT_MOD(var, &gBattleMons[opposingBattler], gBattleMons[opposingBattler].spDefense, STAT_SPDEF);
									spDef += var;
								}
							}
							if (def < spDef)
								statId = STAT_ATK;
							else
								statId = STAT_SPATK;
							
							if (CompareStat(battler, statId, 12, CMP_LESS_THAN))
							{
								SET_STATCHANGER(statId, 1, FALSE);
								gBattlerAttacker = battler;
								PREPARE_STAT_BUFFER(gBattleTextBuff1, statId);
								BattleScriptPushCursorAndCallback(BattleScript_AttackerAbilityStatRaiseEnd3);
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
							
							for (i = 0, count = 0; i < MAX_BATTLERS_COUNT; i++)
							{
								if (IsBattlerAlive(i) && GetBattlerSide(i) != GetBattlerSide(battler))
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
												case EFFECT_COUNTER:
												case EFFECT_MIRROR_COAT:
												case EFFECT_METAL_BURST:
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
								if (data[i].power > data[bestId].power || (data[i].power == data[bestId].power && Random() & 1))
									bestId = i;
							}
							gBattlerTarget = data[bestId].battlerId;
							PREPARE_MOVE_BUFFER(gBattleTextBuff1, data[bestId].moveId);
							Free(data);
							gBattleCommunication[MULTISTRING_CHOOSER] = 11;
							BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						break;
					case ABILITY_FRISK:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							gBattlerAttacker = battler;
							BattleScriptPushCursorAndCallback(BattleScript_Frisk);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						return effect; // don't record ability if don't activate
					case ABILITY_SCREEN_CLEANER:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							u8 battlerSide = GetBattlerSide(battler), opposingSide = GetBattlerSide(BATTLE_OPPOSITE(battler));
							
							if (gSideStatuses[battlerSide] & (SIDE_STATUS_REFLECT | SIDE_STATUS_LIGHTSCREEN))
							{
								gSideStatuses[battlerSide] &= ~(SIDE_STATUS_REFLECT | SIDE_STATUS_LIGHTSCREEN);
								gSideTimers[battlerSide].reflectTimer = 0;
								gSideTimers[battlerSide].lightscreenTimer = 0;
								++effect;
							}
							if (gSideStatuses[opposingSide] & (SIDE_STATUS_REFLECT | SIDE_STATUS_LIGHTSCREEN))
							{
								gSideStatuses[opposingSide] &= ~(SIDE_STATUS_REFLECT | SIDE_STATUS_LIGHTSCREEN);
								gSideTimers[opposingSide].reflectTimer = 0;
								gSideTimers[opposingSide].lightscreenTimer = 0;
								++effect;
							}
							if (effect)
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = 12;
								BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							}
						}
						break;
					case ABILITY_CURIOUS_MEDICINE:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							gBattlerTarget = BATTLE_PARTNER(battler);
							
							if (IsBattlerAlive(gBattlerTarget) && TryResetBattlerStatChanges(gBattlerTarget))
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = 13;
								BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								++effect;
							}
						}
						break;
					case ABILITY_COSTAR:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							gBattlerTarget = BATTLE_PARTNER(battler);
							
							if (IsBattlerAlive(gBattlerTarget))
							{
								if (CountBattlerStatIncreases(gBattlerTarget, FALSE)) // Does it copies accuracy and evasion or negative stat stages ?
								{
									CopyBattlerStatChanges(battler, gBattlerTarget);
									++effect;
								}
								if (gBattleMons[gBattlerTarget].status2 & STATUS2_FOCUS_ENERGY) // It can also copy Focus Energy status
								{
									gBattleMons[battler].status2 |= STATUS2_FOCUS_ENERGY;
									++effect;
								}
								if (effect)
								{
									gBattlerAttacker = battler;
									gBattleCommunication[MULTISTRING_CHOOSER] = 14;
									BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
									gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								}
							}
						}
						break;
					case ABILITY_SUPREME_OVERLORD:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							gBattleStruct->supremeOverlordBoosts[battler] = min(5, gBattleStruct->faintCounter[GetBattlerSide(battler)]);
							gBattleCommunication[MULTISTRING_CHOOSER] = 17;
							BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						break;
					case ABILITY_LIMBER:
					    if (!gSpecialStatuses[battler].switchInAbilityDone && gBattleMons[battler].status1 & STATUS1_PARALYSIS)
						{
							gBattlerTarget = battler;
							ClearBattlerStatus(gBattlerTarget);
							BattleScriptPushCursorAndCallback(BattleScript_PastelVeilActivates);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						break;
					case ABILITY_INSOMNIA:
					case ABILITY_VITAL_SPIRIT:
					    if (!gSpecialStatuses[battler].switchInAbilityDone && gBattleMons[battler].status1 & STATUS1_SLEEP)
						{
							gBattlerTarget = battler;
							ClearBattlerStatus(gBattlerTarget);
							BattleScriptPushCursorAndCallback(BattleScript_PastelVeilActivates);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						break;
					case ABILITY_IMMUNITY:
					    if (!gSpecialStatuses[battler].switchInAbilityDone && gBattleMons[battler].status1 & STATUS1_PSN_ANY)
						{
							gBattlerTarget = battler;
							ClearBattlerStatus(gBattlerTarget);
							BattleScriptPushCursorAndCallback(BattleScript_PastelVeilActivates);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						break;
					case ABILITY_MAGMA_ARMOR:
					    if (!gSpecialStatuses[battler].switchInAbilityDone && gBattleMons[battler].status1 & STATUS1_FREEZE)
						{
							gBattlerTarget = battler;
							ClearBattlerStatus(gBattlerTarget);
							BattleScriptPushCursorAndCallback(BattleScript_PastelVeilActivates);
							gSpecialStatuses[battler].switchInAbilityDone = TRUE;
							++effect;
						}
						break;
					case ABILITY_PASTEL_VEIL:
					    if (!gSpecialStatuses[battler].switchInAbilityDone)
						{
							gBattlerTarget = BATTLE_PARTNER(battler);
							
							if (IsBattlerAlive(gBattlerTarget) && gBattleMons[gBattlerTarget].status1 & STATUS1_PSN_ANY)
							{
								ClearBattlerStatus(gBattlerTarget);
								BattleScriptPushCursorAndCallback(BattleScript_PastelVeilActivates);
								gSpecialStatuses[battler].switchInAbilityDone = TRUE;
								++effect;
							}
						}
						break;
					case ABILITY_INTREPID_SWORD:
					    if (!(gBattleStruct->intrepidSwordActivated[GetBattlerSide(battler)] & gBitTable[gBattlerPartyIndexes[battler]])
							&& CompareStat(battler, STAT_ATK, 12, CMP_LESS_THAN))
						{
							gBattleStruct->intrepidSwordActivated[GetBattlerSide(battler)] |= gBitTable[gBattlerPartyIndexes[battler]];
							SET_STATCHANGER(STAT_ATK, 1, FALSE);
							gBattlerAttacker = battler;
							BattleScriptPushCursorAndCallback(BattleScript_BattlerAbilityStatRaiseOnSwitchIn);
							++effect;
						}
						break;
					case ABILITY_DAUNTLESS_SHIELD:
					    if (!(gBattleStruct->dauntlessShieldActivated[GetBattlerSide(battler)] & gBitTable[gBattlerPartyIndexes[battler]])
							&& CompareStat(battler, STAT_DEF, 12, CMP_LESS_THAN))
						{
							gBattleStruct->dauntlessShieldActivated[GetBattlerSide(battler)] |= gBitTable[gBattlerPartyIndexes[battler]];
							SET_STATCHANGER(STAT_DEF, 1, FALSE);
							gBattlerAttacker = battler;
							BattleScriptPushCursorAndCallback(BattleScript_BattlerAbilityStatRaiseOnSwitchIn);
							++effect;
						}
						break;
				}
				break;
			case ABILITYEFFECT_ENDTURN:
			    if (IsBattlerAlive(battler))
				{
					switch (gLastUsedAbility)
					{
						case ABILITY_SOLAR_POWER:
						    SOLAR_POWER_SUN_CHECK:
						    if (IsBattlerWeatherAffected(battler, WEATHER_SUN_ANY))
							{
								BattleScriptPushCursorAndCallback(BattleScript_DrySkinSunActivates);
								gBattleMoveDamage = gBattleMons[battler].maxHP / 8;
								if (gBattleMoveDamage == 0)
									gBattleMoveDamage = 1;
								++effect;
							}
							break;
						case ABILITY_RAIN_DISH:
						    if (IsBattlerWeatherAffected(battler, WEATHER_RAIN_ANY) && gBattleMons[battler].maxHP > gBattleMons[battler].hp)
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = 0;
								gBattleMoveDamage = gBattleMons[battler].maxHP / 16;
								if (gBattleMoveDamage == 0)
									gBattleMoveDamage = 1;
								gBattleMoveDamage *= -1;
								BattleScriptPushCursorAndCallback(BattleScript_RainDishActivates);
								++effect;
							}
							break;
						case ABILITY_ICE_BODY:
						    if (IsBattlerWeatherAffected(battler, WEATHER_HAIL_ANY) && gBattleMons[battler].maxHP > gBattleMons[battler].hp)
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = 0;
								gBattleMoveDamage = gBattleMons[battler].maxHP / 16;
								if (gBattleMoveDamage == 0)
									gBattleMoveDamage = 1;
								gBattleMoveDamage *= -1;
								BattleScriptPushCursorAndCallback(BattleScript_RainDishActivates);
								++effect;
							}
							break;
						case ABILITY_POISON_HEAL:
						    if (gBattleMons[battler].status1 & STATUS1_PSN_ANY && gBattleMons[battler].maxHP > gBattleMons[battler].hp)
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = 0;
								gBattleMoveDamage = gBattleMons[battler].maxHP / 8;
								if (gBattleMoveDamage == 0)
									gBattleMoveDamage = 1;
								gBattleMoveDamage *= -1;
								BattleScriptPushCursorAndCallback(BattleScript_RainDishActivates);
								++effect;
							}
							break;
						case ABILITY_DRY_SKIN:
						    if (IsBattlerWeatherAffected(battler, WEATHER_RAIN_ANY) && gBattleMons[battler].maxHP > gBattleMons[battler].hp)
							{
								gBattleCommunication[MULTISTRING_CHOOSER] = 1;
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
						case ABILITY_SHED_SKIN:
						    if (gBattleMons[battler].status1 & STATUS1_ANY && (Random() % 3) == 0)
							{
								ClearBattlerStatus(battler);
								BattleScriptPushCursorAndCallback(BattleScript_ShedSkinActivates);
								++effect;
							}
							break;
						case ABILITY_HYDRATION:
						    if (gBattleMons[battler].status1 & STATUS1_ANY && IsBattlerWeatherAffected(battler, WEATHER_RAIN_ANY))
							{
								ClearBattlerStatus(battler);
								BattleScriptPushCursorAndCallback(BattleScript_ShedSkinActivates);
								++effect;
							}
							break;
						case ABILITY_HEALER:
						    if (IsBattlerAlive(BATTLE_PARTNER(battler)) && gBattleMons[BATTLE_PARTNER(battler)].status1 & STATUS1_ANY && (Random() % 3) == 0)
							{
								gEffectBattler = BATTLE_PARTNER(battler);
								ClearBattlerStatus(gEffectBattler);
								BattleScriptPushCursorAndCallback(BattleScript_HealerActivates);
								++effect;
							}
							break;
						case ABILITY_SPEED_BOOST:
						    if (CompareStat(battler, STAT_SPEED, 12, CMP_LESS_THAN) && gDisableStructs[battler].isFirstTurn != 2
							// in gen5 onwards Speed Boost fails to activate if the user failed to run from a wild battle
							&& gActionsByTurnOrder[gBattlerByTurnOrder[battler]] != B_ACTION_RUN) // tried to run. If the game reach in this part is because it failed to do it.
							{
								SET_STATCHANGER(STAT_SPEED, 1, FALSE);
								BattleScriptPushCursorAndCallback(BattleScript_SpeedBoostActivates);
								++effect;
							}
							break;
						case ABILITY_TRUANT:
						    if (!(gBattleMons[battler].status1 & STATUS1_SLEEP))
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
						    if ((IsBattlerWeatherAffected(battler, WEATHER_SUN_ANY) || (Random() % 2) == 0) && !gBattleMons[battler].item
							&& !gBattleStruct->changedItems[battler] && ItemId_GetPocket(GetUsedHeldItem(battler)) == POCKET_BERRY_POUCH)
							{
								gLastUsedItem = GetUsedHeldItem(battler);
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
									if (CompareStat(battler, i, 0, CMP_GREATER_THAN))
										validToLower |= gBitTable[i];
									
									if (CompareStat(battler, i, 12, CMP_LESS_THAN))
										validToRaise |= gBitTable[i];
								}
								
								if (validToRaise || validToLower)
								{
									gBattleCommunication[MULTIUSE_STATE] = gBattleScripting.statChanger = 0;
									
									if (validToLower)
									{
										do
										{
											i = (Random() % NUM_STATS) + STAT_ATK;
										} while (!(validToLower & gBitTable[i]));
										
										SET_STATCHANGER(i, 1, TRUE);
										gBattleCommunication[MULTIUSE_STATE] = gBattleScripting.statChanger; // save it in multiuse state
										validToRaise &= ~(gBitTable[i]); // cannot raise the same stat
									}
									if (validToRaise)
									{
										do
										{
											i = (Random() % NUM_STATS) + STAT_ATK;
										} while (!(validToRaise & gBitTable[i]));
										
										SET_STATCHANGER(i, 2, FALSE);
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
									if (newSpecies == SPECIES_WISHIWASHI)
										gBattleCommunication[MULTISTRING_CHOOSER] = 1;
									else
										gBattleCommunication[MULTISTRING_CHOOSER] = 0;
									DoBattleFormChange(battler, newSpecies, FALSE, FALSE);
									BattleScriptPushCursorAndCallback(BattleScript_SchoolingActivates);
									++effect;
								}
							}
							break;
						case ABILITY_ZEN_MODE:
						    {
								u16 newSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_HP);
								
								if (newSpecies)
								{
									gBattleCommunication[MULTISTRING_CHOOSER] = 0;
									DoBattleFormChange(battler, newSpecies, TRUE, TRUE);
									BattleScriptPushCursorAndCallback(BattleScript_ZenModeActivates);
									gBattleScripting.battler = battler;
									++effect;
								}
							}
							break;
						case ABILITY_HUNGER_SWITCH:
						    if (!(gBattleMons[battler].status2 & STATUS2_TRANSFORMED))
							{
								u16 newSpecies = SPECIES_NONE;
								
								switch (gBattleMons[battler].species)
								{
									case SPECIES_MORPEKO:
									    newSpecies = SPECIES_MORPEKO_HANGRY;
										break;
									case SPECIES_MORPEKO_HANGRY:
									    newSpecies = SPECIES_MORPEKO;
										break;
								}
								if (newSpecies)
								{
									gBattleCommunication[MULTISTRING_CHOOSER] = 1;
									DoBattleFormChange(battler, newSpecies, FALSE, FALSE);
									BattleScriptPushCursorAndCallback(BattleScript_ZenModeActivates);
									gBattleScripting.battler = battler;
									++effect;
								}
							}
							break;
						case ABILITY_PICKUP:
							{
								u8 pickupBattler = GetBattlerOnTopOfPickupStack(battler);
								
								if (pickupBattler != 0xFF && TryRecycleBattlerItem(battler, pickupBattler))
								{
									BattleScriptPushCursorAndCallback(BattleScript_Pickup);
									++effect;
								}
							}
							break;
					}
				}
				break;
			case ABILITYEFFECT_TRACE1:
			case ABILITYEFFECT_TRACE2:
			    for (i = 0; i < gBattlersCount; i++)
				{
					if (GetBattlerAbility(i) == ABILITY_TRACE && gBattleResources->flags->flags[i] & RESOURCE_FLAG_TRACED)
					{
						u8 opposingSide = BATTLE_OPPOSITE(GetBattlerPosition(battler)) & BIT_SIDE;
						u8 target1 = GetBattlerAtPosition(opposingSide);
						u8 target2 = GetBattlerAtPosition(opposingSide + BIT_FLANK);
						
						if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
						{
							if (IsBattlerAlive(target1) && IsBattlerAlive(target2) && IsBattlerAbilityCopyableByTrace(target1)
								&& IsBattlerAbilityCopyableByTrace(target2))
							{
								gBattlerTarget = GetBattlerAtPosition(((Random() & 1) * 2) | opposingSide);
								++effect;
							}
							else if (IsBattlerAlive(target1) && IsBattlerAbilityCopyableByTrace(target1))
							{
								gBattlerTarget = target1;
								++effect;
							}
							else if (IsBattlerAlive(target2) && IsBattlerAbilityCopyableByTrace(target2))
							{
								gBattlerTarget = target2;
								++effect;
							}
						}
						else
						{
							if (IsBattlerAlive(target1) && IsBattlerAbilityCopyableByTrace(target1))
							{
								gBattlerTarget = target1;
								++effect;
							}
						}
						if (effect)
						{
							gBattlerAttacker = gBattleScripting.battler = i;
							gBattleResources->flags->flags[i] &= ~(RESOURCE_FLAG_TRACED);
							RecordAbilityBattle(i, gBattleMons[gBattlerTarget].ability);
							
							if (caseId == ABILITYEFFECT_TRACE1)
								BattleScriptPushCursorAndCallback(BattleScript_TraceActivatesEnd3);
							else
							{
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_TraceActivates;
							}
							break;
						}
					}
				}
				break;
			case ABILITYEFFECT_NEUTRALIZING_GAS:
			    // Prints message only. separate from ABILITYEFFECT_ON_SWITCHIN bc activates before entry hazards
			    for (i = 0; i < gBattlersCount; i++)
				{
					if (IsBattlerAlive(i) && GetBattlerAbility(i) == ABILITY_NEUTRALIZING_GAS && !(gBattleResources->flags->flags[i] & RESOURCE_FLAG_NEUTRALIZING_GAS))
					{
						gBattleResources->flags->flags[i] |= RESOURCE_FLAG_NEUTRALIZING_GAS;
						gBattleScripting.battler = battler = i;
						gLastUsedAbility = GetBattlerAbility(i);
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
					if (IsBattlerAlive(i) && !gSpecialStatuses[i].switchInAbilityDone && (GetBattlerAbility(i) == ABILITY_UNNERVE
					|| GetBattlerAbility(i) == ABILITY_AS_ONE_ICE_RIDER || GetBattlerAbility(i) == ABILITY_AS_ONE_SHADOW_RIDER))
					{
						if (GetBattlerAbility(i) == ABILITY_UNNERVE)
							gBattleCommunication[MULTISTRING_CHOOSER] = 2; // Unnerve message
						else
							gBattleCommunication[MULTISTRING_CHOOSER] = 15; // As One message
						gBattlerAttacker = gBattleScripting.battler = battler = i;
						gLastUsedAbility = GetBattlerAbility(i);
						BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
						gSpecialStatuses[battler].switchInAbilityDone = TRUE;
						++effect;
						break;
					}
				}
				break;
			case ABILITYEFFECT_MOVES_BLOCK:
			    moveTarget = GetBattlerMoveTargetType(gBattlerAttacker, moveArg);
			
			    switch (gLastUsedAbility)
				{
					case ABILITY_SOUNDPROOF:
					    if (gBattleMoves[moveArg].flags.soundMove && moveTarget != MOVE_TARGET_USER)
							effect = 1;
						break;
					case ABILITY_BULLETPROOF:
					    if (gBattleMoves[moveArg].flags.ballisticMove)
							effect = 1;
						break;
					case ABILITY_TELEPATHY:
					    if (IsBattlerAlive(BATTLE_PARTNER(battler)) && gBattlerAttacker == BATTLE_PARTNER(battler) && !IS_MOVE_STATUS(moveArg))
							effect = 1;
						break;
					case ABILITY_QUEENLY_MAJESTY:
					case ABILITY_DAZZLING:
					case ABILITY_ARMOR_TAIL:
					    if (GetChosenMovePriority(gBattlerAttacker) > 0 && GetBattlerSide(gBattlerAttacker) != GetBattlerSide(battler)
							&& !(moveTarget & MOVE_TARGET_OPPONENTS_FIELD) && (moveTarget != MOVE_TARGET_ALL_BATTLERS || moveArg == MOVE_PERISH_SONG
						    || moveArg == MOVE_FLOWER_SHIELD || moveArg == MOVE_ROTOTILLER))
							effect = 2;
						break;
				}
				if (effect)
				{
					if (gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS)
						gHitMarker |= HITMARKER_NO_PPDEDUCT;
					
					if (effect == 1)
						gBattlescriptCurrInstr = BattleScript_SoundproofProtected;
					else
						gBattlescriptCurrInstr = BattleScript_DazzlingProtected;
				}
				break;
			case ABILITYEFFECT_ABSORBING:
			    if (moveArg)
				{
					u8 statId, statAmount = 1;
					
					moveTarget = GetBattlerMoveTargetType(gBattlerAttacker, moveArg);
					
					switch (gLastUsedAbility)
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
						    if (moveType == TYPE_GROUND && !(moveTarget & MOVE_TARGET_OPPONENTS_FIELD))
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
						case ABILITY_WIND_RIDER: // Don't affects moves that target the user or all battlers, like Tailwind (will be handled separated)
						    // Sandstorm is a wind move, but can't trigger Wind Rider as well. Why?...
						    if (gBattleMoves[moveArg].flags.windMove && !(moveTarget & MOVE_TARGET_USER))
							{
								statId = STAT_ATK;
								effect = 2;
							}
							break;
						case ABILITY_WELL_BAKED_BODY:
						    if (moveType == TYPE_FIRE)
							{
								statId = STAT_DEF;
								statAmount = 2;
								effect = 2;
							}
							break;
						case ABILITY_FLASH_FIRE:
						    if (moveType == TYPE_FIRE)
							{
								if (gProtectStructs[gBattlerAttacker].notFirstStrike)
									gBattlescriptCurrInstr = BattleScript_FlashFireBoost;
								else
									gBattlescriptCurrInstr = BattleScript_FlashFireBoost_PPLoss;
								
								if (!(gBattleResources->flags->flags[battler] & RESOURCE_FLAG_FLASH_FIRE))
								{
									gBattleResources->flags->flags[battler] |= RESOURCE_FLAG_FLASH_FIRE;
									gBattleCommunication[MULTISTRING_CHOOSER] = 0;
								}
								else
									gBattleCommunication[MULTISTRING_CHOOSER] = 1;
								
								effect = 3;
							}
							break;
					}
					if (effect == 1)
					{
						if (gProtectStructs[gBattlerAttacker].notFirstStrike)
                            gBattlescriptCurrInstr = BattleScript_MoveHPDrain;
                        else
                            gBattlescriptCurrInstr = BattleScript_MoveHPDrain_PPLoss;
						
						if (gBattleMons[battler].maxHP == gBattleMons[battler].hp)
						{
							if ((gProtectStructs[gBattlerAttacker].notFirstStrike))
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
					}
					else if (effect == 2)
					{
						if (CompareStat(battler, statId, 12, CMP_LESS_THAN))
						{
							SET_STATCHANGER(statId, statAmount, FALSE);
							
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
					}
				}
				break;
			case ABILITYEFFECT_MOVE_END_ATTACKER: // attacker abilities that activates after makes contact with the target
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(gBattlerTarget) && !gProtectStructs[battler].confusionSelfDmg
				&& IsBattlerAlive(gBattlerTarget) && !SubsBlockMove(battler, gBattlerTarget, moveArg))
				{
					switch (gLastUsedAbility)
					{
						case ABILITY_STENCH: // Stench check is taken care of in King's Rock check
						    if (Random() % 100 < 10 && GetBattlerTurnOrderNum(gBattlerTarget) > gCurrentTurnActionNumber && !MoveHasFlinchChance(moveArg))
							{
								BattleScriptPushCursor();
								SetMoveEffect(MOVE_EFFECT_FLINCH, FALSE, FALSE);
								DoMoveEffect(FALSE);
								BattleScriptPop();
								++effect;
							}
							break;
						case ABILITY_POISON_TOUCH:
						    if (CanBePoisoned(gBattlerTarget, battler, TRUE) && IsMoveMakingContact(battler, moveArg) && (Random() % 3) == 0)
							{
								SetMoveEffect(MOVE_EFFECT_POISON, FALSE, FALSE);
								PREPARE_ABILITY_BUFFER(gBattleTextBuff1, gLastUsedAbility);
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_PoisonTouchActivation;
								gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
								++effect;
							}
							break;
					}
				}
				break;
			case ABILITYEFFECT_MOVE_END_TARGET: // target abilities that activates after makes contact with it
			    if (!ReceiveSheerForceBoost(gBattlerAttacker, moveArg) || gLastUsedAbility == ABILITY_ILLUSION) // all these abilities are negated by Sheer Force, except Illusion
				{
					switch (gLastUsedAbility)
					{
						case ABILITY_ROUGH_SKIN:
						case ABILITY_IRON_BARBS:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && IsBattlerAlive(gBattlerAttacker) && BATTLER_DAMAGED(battler)
							&& !gProtectStructs[gBattlerAttacker].confusionSelfDmg && IsMoveMakingContact(gBattlerAttacker, moveArg)
							&& GetBattlerAbility(gBattlerAttacker) != ABILITY_MAGIC_GUARD)
							{
								gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 8;
								if (gBattleMoveDamage == 0)
									gBattleMoveDamage = 1;
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_RoughSkinActivates;
								++effect;
							}
							break;
						case ABILITY_AFTERMATH:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(battler) && IsBattlerAlive(gBattlerAttacker)
							&& GetBattlerAbility(gBattlerAttacker) != ABILITY_MAGIC_GUARD && IsMoveMakingContact(gBattlerAttacker, moveArg)
							&& !IsBattlerAlive(battler) && !ABILITY_ON_FIELD(ABILITY_DAMP))
							{
								gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 4;
								if (gBattleMoveDamage == 0)
									gBattleMoveDamage = 1;
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_RoughSkinActivates;
								++effect;
							}
							break;
						case ABILITY_INNARDS_OUT:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && !IsBattlerAlive(battler) && IsBattlerAlive(gBattlerAttacker))
							{
								gBattleMoveDamage = gSpecialStatuses[battler].dmg;
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_RoughSkinActivates;
								++effect;
							}
							break;
						case ABILITY_COLOR_CHANGE:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && moveArg != MOVE_STRUGGLE && !IS_MOVE_STATUS(moveArg)
							&& BATTLER_DAMAGED(battler) && !IS_BATTLER_OF_TYPE(battler, moveType) && IsBattlerAlive(battler)
							&& !SubsBlockMove(gBattlerAttacker, battler, moveArg) && IS_MULTIHIT_FINAL_STRIKE) // Only occours on final strike of multi-hit moves
							{
								SET_BATTLER_TYPE(battler, moveType);
								PREPARE_TYPE_BUFFER(gBattleTextBuff1, moveType);
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_ColorChangeActivates;
								++effect;
							}
							break;
						case ABILITY_EFFECT_SPORE:
						    if (IsBattlerAffectedBySpore(gBattlerAttacker))
							{
								i = Random() % 3;
								
								if (i == 0)
									goto POISON_POINT;
								if (i == 1)
									goto STATIC;
								// sleep
								if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && IsBattlerAlive(gBattlerAttacker) && BATTLER_DAMAGED(battler)
								&& !gProtectStructs[gBattlerAttacker].confusionSelfDmg && IsMoveMakingContact(gBattlerAttacker, moveArg)
								&& (Random() % 3) == 0 && CanBePutToSleep(gBattlerAttacker, TRUE))
								{
									SetMoveEffect(MOVE_EFFECT_SLEEP, TRUE, FALSE);
									BattleScriptPushCursor();
									gBattlescriptCurrInstr = BattleScript_ApplySecondaryEffect;
									gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
									++effect;
								}
							}
							break;
						case ABILITY_POISON_POINT:
						    POISON_POINT:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && IsBattlerAlive(gBattlerAttacker) && BATTLER_DAMAGED(battler)
							&& !gProtectStructs[gBattlerAttacker].confusionSelfDmg && IsMoveMakingContact(gBattlerAttacker, moveArg) && (Random() % 3) == 0
							&& CanBePoisoned(gBattlerAttacker, battler, TRUE))
							{
								SetMoveEffect(MOVE_EFFECT_POISON, TRUE, FALSE);
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_ApplySecondaryEffect;
								gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
								++effect;
							}
							break;
						case ABILITY_STATIC:
						    STATIC:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && IsBattlerAlive(gBattlerAttacker) && BATTLER_DAMAGED(battler)
							&& !gProtectStructs[gBattlerAttacker].confusionSelfDmg && IsMoveMakingContact(gBattlerAttacker, moveArg) && (Random() % 3) == 0
							&& CanBeParalyzed(gBattlerAttacker, TRUE))
							{
								SetMoveEffect(MOVE_EFFECT_PARALYSIS, TRUE, FALSE);
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_ApplySecondaryEffect;
								gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
								++effect;
							}
							break;
						case ABILITY_FLAME_BODY:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && IsBattlerAlive(gBattlerAttacker) && BATTLER_DAMAGED(battler)
							&& !gProtectStructs[gBattlerAttacker].confusionSelfDmg && IsMoveMakingContact(gBattlerAttacker, moveArg) && (Random() % 3) == 0
							&& CanBeBurned(gBattlerAttacker, TRUE))
							{
								SetMoveEffect(MOVE_EFFECT_BURN, TRUE, FALSE);
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_ApplySecondaryEffect;
								gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
								++effect;
							}
							break;
						case ABILITY_CUTE_CHARM:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && IsBattlerAlive(gBattlerAttacker) && BATTLER_DAMAGED(battler)
							&& !gProtectStructs[gBattlerAttacker].confusionSelfDmg && IsMoveMakingContact(gBattlerAttacker, moveArg)
							&& IsBattlerAlive(gBattlerTarget) && (Random() % 3) == 0 && CanBeInfatuatedBy(gBattlerAttacker, battler))
							{
								gBattleMons[gBattlerAttacker].status2 |= STATUS2_INFATUATED_WITH(battler);
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_CuteCharmActivates;
								++effect;
							}
							break;
						case ABILITY_ANGER_POINT:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(battler) && gIsCriticalHit && IsBattlerAlive(gBattlerTarget)
								&& !SubsBlockMove(gBattlerAttacker, battler, moveArg) && CompareStat(battler, STAT_ATK, 12, CMP_LESS_THAN))
							{
								gBattleMons[battler].statStages[STAT_ATK] = 0xC;
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_AngerPointActivation;
								++effect;
							}
							break;
						case ABILITY_CURSED_BODY:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(battler) && IsBattlerAlive(gBattlerAttacker) && (Random() % 3) == 0
							&& !gProtectStructs[gBattlerAttacker].confusionSelfDmg && !SubsBlockMove(gBattlerAttacker, battler, moveArg)
							&& moveArg != MOVE_STRUGGLE && !gDisableStructs[gBattlerAttacker].disabledMove && gBattleMons[gBattlerAttacker].pp[gCurrMovePos]
							&& !CheckAbilityInBattle(CHECK_ABILITY_ON_SIDE, gBattlerAttacker, ABILITY_AROMA_VEIL))
							{
								gDisableStructs[gBattlerAttacker].disabledMove = moveArg;
								gDisableStructs[gBattlerAttacker].disableTimer = 4;
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_CursedBodyActivation;
								++effect;
							}
							break;
						case ABILITY_WEAK_ARMOR:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && IsBattlerAlive(battler) && IS_MOVE_PHYSICAL(moveArg) && BATTLER_DAMAGED(battler)
							&& (CompareStat(battler, STAT_SPEED, 12, CMP_LESS_THAN) || CompareStat(battler, STAT_DEF, 0, CMP_GREATER_THAN)))
							{
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_WeakArmorActivation;
								++effect;
							}
							break;
						case ABILITY_ILLUSION:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(battler) && TryRemoveIllusion(battler))
								++effect;
							break;
						case ABILITY_MUMMY:
						case ABILITY_LINGERING_AROMA:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && IsBattlerAlive(gBattlerAttacker) && BATTLER_DAMAGED(battler) && IsMoveMakingContact(gBattlerAttacker, moveArg))
							{
								switch (gBattleMons[gBattlerAttacker].ability)
								{
									case ABILITY_MULTITYPE:
									case ABILITY_ZEN_MODE:
									case ABILITY_STANCE_CHANGE:
									case ABILITY_SHIELDS_DOWN:
									case ABILITY_SCHOOLING:
									case ABILITY_DISGUISE:
									case ABILITY_BATTLE_BOND:
									case ABILITY_POWER_CONSTRUCT:
									case ABILITY_COMATOSE:
									case ABILITY_RKS_SYSTEM:
									case ABILITY_AS_ONE_ICE_RIDER:
									case ABILITY_AS_ONE_SHADOW_RIDER:
									case ABILITY_ZERO_TO_HERO:
									case ABILITY_COMMANDER:
									case ABILITY_LINGERING_AROMA:
									case ABILITY_MUMMY:
									    break;
									default:
										BattleScriptPushCursor();
										gBattlescriptCurrInstr = BattleScript_MummyActivates;
										// for recording the correct ability
										RecordAbilityBattle(gBattlerAttacker, gBattleMons[battler].ability); // same as target ability
										RecordAbilityBattle(battler, gBattleMons[battler].ability);
										return ++effect;
								}
							}
							break;
						case ABILITY_THERMAL_EXCHANGE:
						    if (moveType == TYPE_FIRE && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(battler) && IsBattlerAlive(battler)
							&& CompareStat(battler, STAT_ATK, 12, CMP_LESS_THAN) && !SubsBlockMove(gBattlerAttacker, battler, moveArg))
							{
								SET_STATCHANGER(STAT_ATK, 1, FALSE);
								gEffectBattler = battler;
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaiseRet;
								++effect;
							}
							break;
						case ABILITY_JUSTIFIED:
						    if (moveType == TYPE_DARK && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(battler) && IsBattlerAlive(battler)
							&& CompareStat(battler, STAT_ATK, 12, CMP_LESS_THAN) && !SubsBlockMove(gBattlerAttacker, battler, moveArg))
							{
								SET_STATCHANGER(STAT_ATK, 1, FALSE);
								gEffectBattler = battler;
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaiseRet;
								++effect;
							}
							break;
						case ABILITY_RATTLED:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(battler) && CompareStat(battler, STAT_SPEED, 12, CMP_LESS_THAN)
							&& IsBattlerAlive(battler) && (moveType == TYPE_BUG || moveType == TYPE_DARK || moveType == TYPE_GHOST))
							{
								SET_STATCHANGER(STAT_SPEED, 1, FALSE);
								gEffectBattler = battler;
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaiseRet;
								++effect;
							}
							break;
						case ABILITY_STAMINA:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(battler) && CompareStat(battler, STAT_DEF, 12, CMP_LESS_THAN)
							&& IsBattlerAlive(battler))
							{
								SET_STATCHANGER(STAT_DEF, 1, FALSE);
								gEffectBattler = battler;
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaiseRet;
								++effect;
							}
							break;
						case ABILITY_WATER_COMPACTION:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(battler) && IsBattlerAlive(battler) && moveType == TYPE_WATER
							&& CompareStat(battler, STAT_DEF, 12, CMP_LESS_THAN))
							{
								SET_STATCHANGER(STAT_DEF, 2, FALSE);
								gEffectBattler = battler;
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaiseRet;
								++effect;
							}
							break;
						case ABILITY_BERSERK:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(battler) && IsBattlerAlive(battler) && gBattleStruct->hpBefore[battler] > gBattleMons[battler].maxHP / 2
							&& gBattleMons[battler].hp < gBattleMons[battler].maxHP / 2 && CompareStat(battler, STAT_SPATK, 12, CMP_LESS_THAN) && IS_MULTIHIT_FINAL_STRIKE)
							{
								SET_STATCHANGER(STAT_SPATK, 1, FALSE);
								gEffectBattler = battler;
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaiseRet;
								++effect;
							}
							break;
						case ABILITY_WIMP_OUT:
						case ABILITY_EMERGENCY_EXIT:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(battler) && IsBattlerAlive(battler) && gBattleStruct->hpBefore[battler] > gBattleMons[battler].maxHP / 2
							&& gBattleMons[battler].hp < gBattleMons[battler].maxHP / 2 && ((!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && GetBattlerSide(battler) == B_SIDE_OPPONENT)
							|| (CanBattlerSwitch(battler) && CountUsablePartyMons(battler) > 0)) && !gProtectStructs[gBattlerAttacker].confusionSelfDmg)
								gBattleResources->flags->flags[battler] |= RESOURCE_FLAG_EMERGENCY_EXIT;
							break; // don't record ability
						case ABILITY_GOOEY:
						case ABILITY_TANGLING_HAIR:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(battler) && IsBattlerAlive(gBattlerAttacker) && IsMoveMakingContact(gBattlerAttacker, moveArg)
							&& CompareStat(gBattlerAttacker, STAT_SPEED, 0, CMP_GREATER_THAN))
							{
								SET_STATCHANGER(STAT_SPEED, 1, TRUE);
								SetMoveEffect(MOVE_EFFECT_SPD_MINUS_1, FALSE, FALSE);
								PREPARE_ABILITY_BUFFER(gBattleTextBuff1, gLastUsedAbility);
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_GooeyActivates;
								gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
								++effect;
							}
							break;
						case ABILITY_SAND_SPIT:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(battler) && !gProtectStructs[gBattlerAttacker].confusionSelfDmg
							&& TryChangeBattleWeather(battler, ENUM_WEATHER_SANDSTORM))
							{
								gBattleScripting.battler = gActiveBattler = battler;
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_SandSpitActivated;
								++effect;
							}
							break;
						case ABILITY_PERISH_BODY:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(battler) && IsMoveMakingContact(gBattlerAttacker, moveArg)
							&& !gProtectStructs[gBattlerAttacker].confusionSelfDmg && !(gStatuses3[gBattlerAttacker] & STATUS3_PERISH_SONG)
							&& IsBattlerAlive(battler))
							{
								if (!(gStatuses3[battler] & STATUS3_PERISH_SONG))
								{
									gStatuses3[battler] |= STATUS3_PERISH_SONG;
									gDisableStructs[battler].perishSongTimer = 3;
								}
								gStatuses3[gBattlerAttacker] |= STATUS3_PERISH_SONG;
								gDisableStructs[gBattlerAttacker].perishSongTimer = 3;
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_PerishBodyActivates;
								++effect;
							}
							break;
						case ABILITY_WANDERING_SPIRIT:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(battler) && IsMoveMakingContact(gBattlerAttacker, moveArg)
							&& IsBattlerAlive(gBattlerAttacker))
							{
								switch (gBattleMons[gBattlerAttacker].ability)
								{
									case ABILITY_AS_ONE_ICE_RIDER:
									case ABILITY_AS_ONE_SHADOW_RIDER:
									case ABILITY_BATTLE_BOND:
									case ABILITY_COMATOSE:
									case ABILITY_COMMANDER:
									case ABILITY_DISGUISE:
									case ABILITY_FLOWER_GIFT:
									case ABILITY_FORECAST:
									case ABILITY_GULP_MISSILE:
									case ABILITY_HUNGER_SWITCH:
									case ABILITY_ICE_FACE:
									case ABILITY_ILLUSION:
									case ABILITY_IMPOSTER:
									case ABILITY_MULTITYPE:
									case ABILITY_NEUTRALIZING_GAS:
									case ABILITY_POWER_OF_ALCHEMY:
									case ABILITY_RECEIVER:
									case ABILITY_RKS_SYSTEM:
									case ABILITY_SCHOOLING:
									case ABILITY_SHIELDS_DOWN:
									case ABILITY_STANCE_CHANGE:
									case ABILITY_WONDER_GUARD:
									case ABILITY_ZEN_MODE:
									case ABILITY_ZERO_TO_HERO:
									    break;
									default:
										// for recording the correct ability
										RecordAbilityBattle(gBattlerAttacker, gLastUsedAbility);
										RecordAbilityBattle(battler, gBattleMons[gBattlerAttacker].ability);
										BattleScriptPushCursor();
										gBattlescriptCurrInstr = BattleScript_WanderingSpiritActivates;
										return ++effect;
								}
							}
							break;
						case ABILITY_WIND_POWER:
						    if (!gBattleMoves[moveArg].flags.windMove)
								break;
							// fallthrough
						case ABILITY_ELECTROMORPHOSIS:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(battler) && !gProtectStructs[gBattlerAttacker].confusionSelfDmg)
							{
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_WindPowerActivates;
								++effect;
							}
							break;
						case ABILITY_COTTON_DOWN:
						    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_DAMAGED(battler) && !gProtectStructs[gBattlerAttacker].confusionSelfDmg)
							{
								BattleScriptPushCursor();
								gBattlescriptCurrInstr = BattleScript_CottonDownActivates;
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
						    if (gBattleMons[battler].status1 & STATUS1_PSN_ANY)
								effect = 1;
							break;
						case ABILITY_LIMBER:
						    if (gBattleMons[battler].status1 & STATUS1_PARALYSIS)
								effect = 1;
							break;
						case ABILITY_INSOMNIA:
						case ABILITY_VITAL_SPIRIT:
						    if (gBattleMons[battler].status1 & STATUS1_SLEEP)
								effect = 1;
							break;
						case ABILITY_WATER_VEIL:
						case ABILITY_WATER_BUBBLE:
						case ABILITY_THERMAL_EXCHANGE:
						    if (gBattleMons[battler].status1 & STATUS1_BURN)
								effect = 1;
							break;
						case ABILITY_MAGMA_ARMOR:
						    if (gBattleMons[battler].status1 & STATUS1_FREEZE)
								effect = 1;
							break;
						case ABILITY_OBLIVIOUS:
						    if (gBattleMons[battler].status2 & STATUS2_INFATUATION)
								effect = 2;
							else if (gDisableStructs[battler].tauntTimer)
								effect = 3;
							break;
						case ABILITY_OWN_TEMPO:
						    if (gBattleMons[battler].status2 & STATUS2_CONFUSION)
								effect = 4;
							break;
					}
					if (effect)
					{
						gBattleCommunication[MULTISTRING_CHOOSER] = effect - 1;
						gBattleScripting.battler = gActiveBattler = battler;
						gLastUsedAbility = GetBattlerAbility(battler);
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_AbilityCuredStatus;
						
						switch (effect)
						{
							case 1:
							    ClearBattlerStatus(battler);
								gBattleCommunication[MULTISTRING_CHOOSER] = 0;
								break;
							case 2:
							    gBattleMons[battler].status2 &= ~(STATUS2_INFATUATION);
								gBattleCommunication[MULTISTRING_CHOOSER] = 1;
								break;
							case 3:
							    gDisableStructs[battler].tauntTimer = 0;
								gBattleCommunication[MULTISTRING_CHOOSER] = 2;
								break;
							case 4:
							    gBattleMons[battler].status2 &= ~(STATUS2_CONFUSION);
								StringCopy(gBattleTextBuff1, gStatusConditionString_ConfusionJpn);
								gBattleCommunication[MULTISTRING_CHOOSER] = 0;
								break;
						}
					}
				}
				break;
			case ABILITYEFFECT_ON_WEATHER: // abilities that activate when the weather changes
			    gBattleScripting.battler = battler;
			
			    switch (gLastUsedAbility)
				{
					case ABILITY_ICE_FACE:
					    if (gBattleStruct->allowedToChangeFormInWeather[GetBattlerSide(battler)] & gBitTable[gBattlerPartyIndexes[battler]])
						{
							u16 newSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_WEATHER);
							
							if (newSpecies)
							{
								gBattleStruct->allowedToChangeFormInWeather[GetBattlerSide(battler)] &= ~(gBitTable[gBattlerPartyIndexes[battler]]);
								DoBattleFormChange(battler, newSpecies, TRUE, TRUE);
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
					u16 newSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_WEATHER);
					
					if (newSpecies)
					{
						DoBattleFormChange(battler, newSpecies, TRUE, TRUE);
						BattleScriptPushCursorAndCallback(BattleScript_CastformChange);
						++effect;
					}
				}
				gSpecialStatuses[battler].removedWeatherChangeAbility = FALSE;
				break;
			case ABILITYEFFECT_SYNCHRONIZE:
			    if (gLastUsedAbility == ABILITY_SYNCHRONIZE && gHitMarker & HITMARKER_SYNCHRONISE_EFFECT)
				{
					gHitMarker &= ~(HITMARKER_SYNCHRONISE_EFFECT);
					SetMoveEffect(gBattleStruct->synchronizeMoveEffect, FALSE, FALSE);
					gBattleScripting.battler = battler;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_SynchronizeActivates;
					gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
					++effect;
				}
				break;
			case ABILITYEFFECT_ON_TERRAIN: // abilities that activate when the terrain changes
			    gBattleScripting.battler = battler;
				break;
		}
		if (effect && caseId != ABILITYEFFECT_SWITCH_IN_WEATHER)
			RecordAbilityBattle(battler, gLastUsedAbility);
	}
	return effect;
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
    BattleScriptPushCursor();
    gBattlescriptCurrInstr = BS_ptr;
    gBattleResources->battleCallbackStack->function[gBattleResources->battleCallbackStack->size++] = gBattleMainFunc;
    gBattleMainFunc = RunBattleScriptCommands;
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
        PREPARE_FLAVOR_BUFFER(gBattleTextBuff1, flavor);
	gBattleMoveDamage = gBattleMons[battlerId].maxHP / 3;
	if (gBattleMoveDamage == 0)
	    gBattleMoveDamage = 1;
	if (gBattleMons[battlerId].hp + gBattleMoveDamage > gBattleMons[battlerId].maxHP)
	    gBattleMoveDamage = gBattleMons[battlerId].maxHP - gBattleMons[battlerId].hp;
	gBattleMoveDamage *= -1;
	if (GetFlavorRelation(battlerId, flavor) < 0)
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

    if (CheckPinchBerryActivate(battlerId, gLastUsedItem) && !moveTurn && CompareStat(battlerId, statId, 12, CMP_LESS_THAN))
    {
        PREPARE_STAT_BUFFER(gBattleTextBuff1, statId);
	PREPARE_STRING_BUFFER(gBattleTextBuff2, STRINGID_STATROSE);
	gEffectBattler = battlerId;
	SET_STATCHANGER(statId, 1, FALSE);
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
    u8 battlerHoldEffectParam = GetBattlerHoldEffectParam(battlerId), defHoldEffectParam;
    u16 defItem;

    gLastUsedItem = gBattleMons[battlerId].item;
	
    if (IsUnnerveOnOpposingField(battlerId) && ItemId_GetPocket(gLastUsedItem) == POCKET_BERRY_POUCH)
    {
	    battlerHoldEffect = 0;
	    battlerHoldEffectParam = 0;
    }
    switch (caseID)
    {
    case ITEMEFFECT_ON_SWITCH_IN:
        if (GetBattlerItemHoldEffect(battlerId, FALSE) == HOLD_EFFECT_DOUBLE_PRIZE)
            gBattleStruct->moneyMultiplier = 2;
        else if (battlerHoldEffect == HOLD_EFFECT_RESTORE_STATS)
	{
		for (i = 0; i < NUM_BATTLE_STATS; ++i)
		{
			if (gBattleMons[battlerId].statStages[i] < 6)
			{
				gBattleMons[battlerId].statStages[i] = 6;
				effect = ITEM_STATS_CHANGE;
			}
		}
	}
        if (effect)
            {
                gBattleScripting.battler = battlerId;
                gPotentialItemEffectBattler = battlerId;
                gActiveBattler = gBattlerAttacker = battlerId;
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
                        PREPARE_MOVE_BUFFER(gBattleTextBuff1, move);
                        BattleScriptExecute(BattleScript_BerryPPHealEnd2);
                        BtlController_EmitSetMonData(0, i + REQUEST_PPMOVE1_BATTLE, 0, 1, &changedPP);
                        MarkBattlerForControllerExec(gActiveBattler);
                        effect = ITEM_PP_CHANGE;
                    }
                }
                break;
            case HOLD_EFFECT_RESTORE_STATS:
                for (i = 0; i < NUM_BATTLE_STATS; ++i)
                {
                    if (gBattleMons[battlerId].statStages[i] < 6)
                    {
                        gBattleMons[battlerId].statStages[i] = 6;
                        effect = ITEM_STATS_CHANGE;
                    }
                }
                if (effect)
                {
                    gBattleScripting.battler = battlerId;
                    gPotentialItemEffectBattler = battlerId;
                    gActiveBattler = gBattlerAttacker = battlerId;
                    BattleScriptExecute(BattleScript_WhiteHerbEnd2);
                }
                break;
            case HOLD_EFFECT_LEFTOVERS:
                if (gBattleMons[battlerId].hp < gBattleMons[battlerId].maxHP && !moveTurn)
                {
                    gBattleMoveDamage = gBattleMons[battlerId].maxHP / battlerHoldEffectParam;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = 1;
                    if (gBattleMons[battlerId].hp + gBattleMoveDamage > gBattleMons[battlerId].maxHP)
                        gBattleMoveDamage = gBattleMons[battlerId].maxHP - gBattleMons[battlerId].hp;
                    gBattleMoveDamage *= -1;
                    BattleScriptExecute(BattleScript_ItemHealHP_End2);
                    effect = ITEM_HP_CHANGE;
                    RecordItemEffectBattle(battlerId, battlerHoldEffect);
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
                    for (i = 0; i < 5 && !CompareStat(battlerId, STAT_ATK + i, 12, CMP_LESS_THAN); ++i);
                    if (i != 5)
                    {
                        do
                            i = Random() % 5;
                        while (!CompareStat(battlerId, STAT_ATK + i, 12, CMP_LESS_THAN));
                        PREPARE_STAT_BUFFER(gBattleTextBuff1, i + 1);
                        gBattleTextBuff2[0] = B_BUFF_PLACEHOLDER_BEGIN;
                        gBattleTextBuff2[1] = B_BUFF_STRING;
                        gBattleTextBuff2[2] = STRINGID_STATSHARPLY;
                        gBattleTextBuff2[3] = STRINGID_STATSHARPLY >> 8;
                        gBattleTextBuff2[4] = B_BUFF_STRING;
                        gBattleTextBuff2[5] = STRINGID_STATROSE;
                        gBattleTextBuff2[6] = STRINGID_STATROSE >> 8;
                        gBattleTextBuff2[7] = EOS;
                        gEffectBattler = battlerId;
                        SET_STATCHANGER(i + 1, 2, FALSE);
                        gBattleScripting.animArg1 = 0x21 + i + 6;
                        gBattleScripting.animArg2 = 0;
                        BattleScriptExecute(BattleScript_BerryStatRaiseEnd2);
                        effect = ITEM_STATS_CHANGE;
                    }
                }
                break;
            case HOLD_EFFECT_CURE_PAR:
                if (gBattleMons[battlerId].status1 & STATUS1_PARALYSIS)
                {
                    gBattleMons[battlerId].status1 &= ~(STATUS1_PARALYSIS);
                    BattleScriptExecute(BattleScript_BerryCurePrlzEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_PSN:
                if (gBattleMons[battlerId].status1 & STATUS1_PSN_ANY)
                {
                    gBattleMons[battlerId].status1 &= ~(STATUS1_PSN_ANY | STATUS1_TOXIC_COUNTER);
                    BattleScriptExecute(BattleScript_BerryCurePsnEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_BRN:
                if (gBattleMons[battlerId].status1 & STATUS1_BURN)
                {
                    gBattleMons[battlerId].status1 &= ~(STATUS1_BURN);
                    BattleScriptExecute(BattleScript_BerryCureBrnEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_FRZ:
                if (gBattleMons[battlerId].status1 & STATUS1_FREEZE)
                {
                    gBattleMons[battlerId].status1 &= ~(STATUS1_FREEZE);
                    BattleScriptExecute(BattleScript_BerryCureFrzEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_SLP:
                if (gBattleMons[battlerId].status1 & STATUS1_SLEEP)
                {
                    gBattleMons[battlerId].status1 &= ~(STATUS1_SLEEP);
                    gBattleMons[battlerId].status2 &= ~(STATUS2_NIGHTMARE);
                    BattleScriptExecute(BattleScript_BerryCureSlpEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }
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
                if (gBattleMons[battlerId].status1 & STATUS1_ANY || gBattleMons[battlerId].status2 & STATUS2_CONFUSION)
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
                }
                break;
            case HOLD_EFFECT_MENTAL_HERB:
                if (gBattleMons[battlerId].status2 & STATUS2_INFATUATION)
                {
                    gBattleMons[battlerId].status2 &= ~(STATUS2_INFATUATION);
                    StringCopy(gBattleTextBuff1, gStatusConditionString_LoveJpn);
                    BattleScriptExecute(BattleScript_BerryCureChosenStatusEnd2);
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                    effect = ITEM_EFFECT_OTHER;
                }
                break;
            }
            if (effect)
            {
                gBattleScripting.battler = battlerId;
                gPotentialItemEffectBattler = battlerId;
                gActiveBattler = gBattlerAttacker = battlerId;
                switch (effect)
                {
                case ITEM_STATUS_CHANGE:
                    BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[battlerId].status1);
                    MarkBattlerForControllerExec(gActiveBattler);
                    break;
                case ITEM_PP_CHANGE:
                    if (!(gBattleMons[battlerId].status2 & STATUS2_TRANSFORMED) && !(gDisableStructs[battlerId].mimickedMoves & gBitTable[i]))
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
            battlerHoldEffectParam = GetBattlerHoldEffectParam(battlerId);
			
            switch (battlerHoldEffect)
            {
            case HOLD_EFFECT_CURE_PAR:
                if (gBattleMons[battlerId].status1 & STATUS1_PARALYSIS)
                {
                    gBattleMons[battlerId].status1 &= ~(STATUS1_PARALYSIS);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_BerryCureParRet;
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_PSN:
                if (gBattleMons[battlerId].status1 & STATUS1_PSN_ANY)
                {
                    gBattleMons[battlerId].status1 &= ~(STATUS1_PSN_ANY | STATUS1_TOXIC_COUNTER);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_BerryCurePsnRet;
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_BRN:
                if (gBattleMons[battlerId].status1 & STATUS1_BURN)
                {
                    gBattleMons[battlerId].status1 &= ~(STATUS1_BURN);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_BerryCureBrnRet;
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_FRZ:
                if (gBattleMons[battlerId].status1 & STATUS1_FREEZE)
                {
                    gBattleMons[battlerId].status1 &= ~(STATUS1_FREEZE);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_BerryCureFrzRet;
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_SLP:
                if (gBattleMons[battlerId].status1 & STATUS1_SLEEP)
                {
                    gBattleMons[battlerId].status1 &= ~(STATUS1_SLEEP);
                    gBattleMons[battlerId].status2 &= ~(STATUS2_NIGHTMARE);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_BerryCureSlpRet;
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_CURE_CONFUSION:
                if (gBattleMons[battlerId].status2 & STATUS2_CONFUSION)
                {
                    gBattleMons[battlerId].status2 &= ~(STATUS2_CONFUSION);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_BerryCureConfusionRet;
                    effect = ITEM_EFFECT_OTHER;
                }
                break;
            case HOLD_EFFECT_MENTAL_HERB:
                if (gBattleMons[battlerId].status2 & STATUS2_INFATUATION)
                {
                    gBattleMons[battlerId].status2 &= ~(STATUS2_INFATUATION);
                    StringCopy(gBattleTextBuff1, gStatusConditionString_LoveJpn);
                    BattleScriptPushCursor();
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                    gBattlescriptCurrInstr = BattleScript_BerryCureChosenStatusRet;
                    effect = ITEM_EFFECT_OTHER;
                }
                break;
            case HOLD_EFFECT_CURE_STATUS:
                if (gBattleMons[battlerId].status1 & STATUS1_ANY || gBattleMons[battlerId].status2 & STATUS2_CONFUSION)
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
                    BattleScriptPushCursor();
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                    gBattlescriptCurrInstr = BattleScript_BerryCureChosenStatusRet;
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
            case HOLD_EFFECT_RESTORE_STATS:
                for (i = 0; i < NUM_BATTLE_STATS; ++i)
                {
                    if (gBattleMons[battlerId].statStages[i] < 6)
                    {
                        gBattleMons[battlerId].statStages[i] = 6;
                        effect = ITEM_STATS_CHANGE;
                    }
                }
                if (effect)
                {
                    gBattleScripting.battler = battlerId;
                    gPotentialItemEffectBattler = battlerId;
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_WhiteHerbRet;
                    return effect;
                }
                break;
            }
            if (effect)
            {
                gBattleScripting.battler = battlerId;
                gPotentialItemEffectBattler = battlerId;
                gActiveBattler = battlerId;
                BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
                MarkBattlerForControllerExec(gActiveBattler);
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
				 && (Random() % 100) < GetEffectChanceIncreases(battlerId, battlerHoldEffectParam))
                {
					BattleScriptPushCursor();
					SetMoveEffect(MOVE_EFFECT_FLINCH, FALSE, FALSE);
					DoMoveEffect(FALSE);
					BattleScriptPop();
                }
                break;
            case HOLD_EFFECT_SHELL_BELL:
                if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
                 && gSpecialStatuses[gBattlerTarget].dmg != 0
                 && gSpecialStatuses[gBattlerTarget].dmg != 0xFFFF
                 && battlerId != gBattlerTarget
                 && gBattleMons[battlerId].hp != gBattleMons[battlerId].maxHP
                 && IsBattlerAlive(battlerId))
                {
                    gPotentialItemEffectBattler = battlerId;
                    gBattleScripting.battler = battlerId;
                    gBattleMoveDamage = (gSpecialStatuses[gBattlerTarget].dmg / battlerHoldEffectParam) * -1;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = -1;
                    gSpecialStatuses[gBattlerTarget].dmg = 0;
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_ItemHealHP_Ret;
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

u8 GetRandomTarget(u8 battlerId)
{
	return GetBattlerAtPosition(sTargetPositions[GetBattlerSide(battlerId)][Random() & 1]);
}

// Get move base target type
u8 GetBattlerMoveTargetType(u8 battlerId, u16 move)
{
	u8 target = gBattleMoves[move].target;
	
	// Add moves that can change target's type in the future, like Expanding Force.
	
	return target;
}

// Get move target and apply all redirection effects
u8 GetMoveTarget(u16 move, u8 setTarget)
{
    u8 side, moveTarget, targetBattler = 0, battlerOpposite = BATTLE_OPPOSITE(gBattlerAttacker);
    
    if (setTarget)
        moveTarget = setTarget - 1;
    else
        moveTarget = GetBattlerMoveTargetType(gBattlerAttacker, move);
	
    switch (moveTarget)
    {
		case MOVE_TARGET_SELECTED:
            side = GetBattlerSide(battlerOpposite);
			
			if (IsBattlerAffectedByFollowMe(gBattlerAttacker, side, move))
				targetBattler = gSideTimers[side].followmeTarget;
			else
			{
				do
				{
					targetBattler = Random() % gBattlersCount;
				} while (GetBattlerSide(gBattlerAttacker) == GetBattlerSide(targetBattler) || !IsBattlerAlive(targetBattler));
				
				if (gBattleMoves[move].type == TYPE_ELECTRIC && ABILITY_ON_OPPOSING_SIDE(gBattlerAttacker, ABILITY_LIGHTNING_ROD)
				&& GetBattlerAbility(targetBattler) != ABILITY_LIGHTNING_ROD)
				{
					targetBattler = BATTLE_PARTNER(targetBattler);
					RecordAbilityBattle(targetBattler, gBattleMons[targetBattler].ability);
					gSpecialStatuses[targetBattler].lightningRodRedirected = 1;
				}
				else if (gBattleMoves[move].type == TYPE_WATER && ABILITY_ON_OPPOSING_SIDE(gBattlerAttacker, ABILITY_STORM_DRAIN)
				&& GetBattlerAbility(targetBattler) != ABILITY_STORM_DRAIN)
				{
					targetBattler = BATTLE_PARTNER(targetBattler);
					RecordAbilityBattle(targetBattler, gBattleMons[targetBattler].ability);
					gSpecialStatuses[targetBattler].stormDrainRedirected = 1;
				}
			}
			break;
		case MOVE_TARGET_DEPENDS:
		case MOVE_TARGET_BOTH:
		case MOVE_TARGET_FOES_AND_ALLY:
		case MOVE_TARGET_OPPONENTS_FIELD:
			targetBattler = battlerOpposite;
			
			if (!IsBattlerAlive(targetBattler))
				targetBattler = BATTLE_PARTNER(targetBattler);
			break;
		case MOVE_TARGET_RANDOM:
			side = GetBattlerSide(battlerOpposite);
			
			if (IsBattlerAffectedByFollowMe(gBattlerAttacker, side, move))
				targetBattler = gSideTimers[side].followmeTarget;
			else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
				targetBattler = GetRandomTarget(gBattlerAttacker);
				
				if (!IsBattlerAlive(targetBattler))
					targetBattler = BATTLE_PARTNER(targetBattler);
			}
			else
				targetBattler = battlerOpposite;
			break;
		case MOVE_TARGET_ALLY:
		    targetBattler = BATTLE_PARTNER(gBattlerAttacker);
			
			if (IsBattlerAlive(targetBattler))
				break;
			// Fallthrough
		default:
			targetBattler = gBattlerAttacker;
			break;
    }
    gBattleStruct->moveTarget[gBattlerAttacker] = targetBattler;
	
    return targetBattler;
}

static bool32 IsNotEventLegalMewOrDeoxys(u8 battlerId)
{
    if (GetBattlerSide(battlerId) == B_SIDE_OPPONENT
     || (GetMonData(&gPlayerParty[gBattlerPartyIndexes[battlerId]], MON_DATA_SPECIES, NULL) != SPECIES_DEOXYS
     && GetMonData(&gPlayerParty[gBattlerPartyIndexes[battlerId]], MON_DATA_SPECIES, NULL) != SPECIES_MEW))
        return TRUE;
    return GetMonData(&gPlayerParty[gBattlerPartyIndexes[battlerId]], MON_DATA_EVENT_LEGAL, NULL);
}

u8 IsMonDisobedient(void)
{
    s32 rnd;
    s32 calc;
    u8 obedienceLevel = 0;

    if ((gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_POKEDUDE)) || GetBattlerSide(gBattlerAttacker) == B_SIDE_OPPONENT)
        return 0;
    if (IsNotEventLegalMewOrDeoxys(gBattlerAttacker)) // only if species is Mew or Deoxys
    {
        if (!IsOtherTrainer(gBattleMons[gBattlerAttacker].otId, gBattleMons[gBattlerAttacker].otName) || FlagGet(FLAG_BADGE08_GET))
            return 0;
        obedienceLevel = 10;
        if (FlagGet(FLAG_BADGE02_GET))
            obedienceLevel = 30;
        if (FlagGet(FLAG_BADGE04_GET))
            obedienceLevel = 50;
        if (FlagGet(FLAG_BADGE06_GET))
            obedienceLevel = 70;
    }
    if (gBattleMons[gBattlerAttacker].level <= obedienceLevel)
        return 0;
    rnd = (Random() & 255);
    calc = (gBattleMons[gBattlerAttacker].level + obedienceLevel) * rnd >> 8;
    if (calc < obedienceLevel)
        return 0;
    // is not obedient
    if (gCurrentMove == MOVE_RAGE)
        gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_RAGE);
    if (gBattleMons[gBattlerAttacker].status1 & STATUS1_SLEEP && (gCurrentMove == MOVE_SNORE || gCurrentMove == MOVE_SLEEP_TALK))
    {
        gBattlescriptCurrInstr = BattleScript_IgnoresWhileAsleep;
        return 1;
    }
    rnd = (Random() & 255);
    calc = (gBattleMons[gBattlerAttacker].level + obedienceLevel) * rnd >> 8;
    if (calc < obedienceLevel && gCurrentMove != MOVE_FOCUS_PUNCH) // Additional check for focus punch in FR
    {
        calc = CheckMoveLimitations(gBattlerAttacker, gBitTable[gCurrMovePos], 0xFF);
        if (calc == 0xF) // all moves cannot be used
        {
            gBattleCommunication[MULTISTRING_CHOOSER] = Random() & 3;
            gBattlescriptCurrInstr = BattleScript_MoveUsedLoafingAround;
            return 1;
        }
        else // use a random move
        {
            do
                gCurrMovePos = gChosenMovePos = Random() & 3;
            while (gBitTable[gCurrMovePos] & calc);
            gCalledMove = gBattleMons[gBattlerAttacker].moves[gCurrMovePos];
            gBattlescriptCurrInstr = BattleScript_IgnoresAndUsesRandomMove;
            gBattlerTarget = GetMoveTarget(gCalledMove, 0);
            return 2;
        }
    }
    else
    {
        obedienceLevel = gBattleMons[gBattlerAttacker].level - obedienceLevel;
        calc = (Random() & 255);
        if (calc < obedienceLevel && !(gBattleMons[gBattlerAttacker].status1 & STATUS1_ANY) && GetBattlerAbility(gBattlerAttacker) != ABILITY_VITAL_SPIRIT && GetBattlerAbility(gBattlerAttacker) != ABILITY_INSOMNIA)
        {
            // try putting asleep
            int i;

            for (i = 0; i < gBattlersCount; ++i)
                if (gBattleMons[i].status2 & STATUS2_UPROAR)
                    break;
            if (i == gBattlersCount)
            {
                gBattlescriptCurrInstr = BattleScript_IgnoresAndFallsAsleep;
                return 1;
            }
        }
        calc -= obedienceLevel;
        if (calc < obedienceLevel)
        {
            gBattlerTarget = gBattlerAttacker;
            gBattleMoveDamage = CalculateBaseDamage(MOVE_NONE, TYPE_MYSTERY, gBattlerAttacker, gBattlerTarget, FALSE, TRUE);
            gBattlescriptCurrInstr = BattleScript_IgnoresAndHitsItself;
            gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
            return 2;
        }
        else
        {
            gBattleCommunication[MULTISTRING_CHOOSER] = Random() & 3;
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

static void SetIllusionMon(u8 battler)
{
	struct Pokemon *party, *partnerMon, *mon = GetBattlerPartyIndexPtr(battler);
	u8 partyCount;
	s8 i, id;
	
	gBattleStruct->illusion[battler].set = TRUE;
	
	if (GetBattlerAbility(battler) == ABILITY_ILLUSION)
	{
		if (GetBattlerSide(battler) == B_SIDE_PLAYER)
		{
			party = gPlayerParty;
			partyCount = gPlayerPartyCount;
		}
		else
		{
			party = gEnemyParty;
			partyCount = gEnemyPartyCount;
		}
		if (&party[partyCount - 1] != mon)
		{
			if (IsBattlerAlive(BATTLE_PARTNER(battler)))
				partnerMon = &party[gBattlerPartyIndexes[BATTLE_PARTNER(battler)]];
			else
				partnerMon = mon;
			
			// find last alive non-egg pokemon
			for (i = PARTY_SIZE - 1; i >= 0; i--)
			{
				id = i;
				
				if (GetMonData(&party[id], MON_DATA_SANITY_HAS_SPECIES) && GetMonData(&party[id], MON_DATA_HP) && !GetMonData(&party[id], MON_DATA_IS_EGG)
					&& &party[id] != mon && &party[id] != partnerMon)
				{
					gBattleStruct->illusion[battler].on = TRUE;
					gBattleStruct->illusion[battler].broken = FALSE;
					gBattleStruct->illusion[battler].partyId = id;
					gBattleStruct->illusion[battler].mon = &party[id];
					return;
				}
			}
		}
	}
}

struct Pokemon *GetIllusionMonPtr(u8 battler)
{
	if (gBattleStruct->illusion[battler].broken)
		return NULL;
	
	if (!gBattleStruct->illusion[battler].set)
		SetIllusionMon(battler);
	
	if (!gBattleStruct->illusion[battler].on)
		return NULL;
	
	return gBattleStruct->illusion[battler].mon;
}

void ClearIllusionMon(u8 battler)
{
	memset(&gBattleStruct->illusion[battler], 0, sizeof(gBattleStruct->illusion[battler]));
}

bool8 TryRemoveIllusion(u8 battler)
{
	if (GetIllusionMonPtr(battler) != NULL)
	{
		gBattleScripting.battler = battler;
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_IllusionOff;
		return TRUE;
	}
	return FALSE;
}

bool8 ReceiveSheerForceBoost(u8 battler, u16 move)
{
	return (GetBattlerAbility(battler) == ABILITY_SHEER_FORCE && gBattleMoves[move].flags.secondaryEffectMove);
}

bool8 IsUnnerveOnOpposingField(u8 battler)
{
	if (ABILITY_ON_OPPOSING_SIDE(battler, ABILITY_UNNERVE) || ABILITY_ON_OPPOSING_SIDE(battler, ABILITY_AS_ONE_ICE_RIDER) || ABILITY_ON_OPPOSING_SIDE(battler, ABILITY_AS_ONE_SHADOW_RIDER))
		return TRUE;
	return FALSE;
}

u16 GetUsedHeldItem(u8 battler)
{
	return gBattleStruct->usedHeldItems[gBattlerPartyIndexes[battler]][GetBattlerSide(battler)];
}

bool8 NoAliveMonsForParty(struct Pokemon *party)
{
	u8 i;
	u32 HP_count = 0;
	
	for (i = 0; i < PARTY_SIZE; i++)
	{
		if (GetMonData(&party[i], MON_DATA_SPECIES) && !GetMonData(&party[i], MON_DATA_IS_EGG))
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
	if (battlerId >= gBattlersCount || gBattleMons[battlerId].hp == 0 || gAbsentBattlerFlags & gBitTable[battlerId])
		return FALSE;
	return TRUE;
}

bool8 IsBattlerWeatherAffected(u8 battlerId, u16 weatherFlags)
{
	if (WEATHER_HAS_EFFECT && gBattleWeather & weatherFlags)
	{
		return TRUE;
	}
	return FALSE;
}

static void ShouldChangeFormOnWeatherStart(u8 battlerId)
{
	u8 i, side = GetBattlerSide(battlerId);
	struct Pokemon *party = GetBattlerParty(battlerId);
	
	for (i = 0; i < PARTY_SIZE; i++)
	{
		if (GetMonData(&party[i], MON_DATA_SPECIES) == SPECIES_EISCUE_NOICE_FACE)
			gBattleStruct->allowedToChangeFormInWeather[side] |= gBitTable[i];
		else
			gBattleStruct->allowedToChangeFormInWeather[side] &= ~(gBitTable[i]);
	}
}

bool8 TryChangeBattleWeather(u8 battlerId, u8 weatherEnumId)
{
	u16 ability = GetBattlerAbility(battlerId);
	
	if (gBattleWeather & WEATHER_PRIMAL_ANY && ability != ABILITY_PRIMORDIAL_SEA && ability != ABILITY_DESOLATE_LAND && ability != ABILITY_DELTA_STREAM)
		return FALSE;
	else if (!(gBattleWeather & (sWeatherFlagsInfo[weatherEnumId][0] | sWeatherFlagsInfo[weatherEnumId][1])))
	{
		gBattleWeather = sWeatherFlagsInfo[weatherEnumId][0];
		gWishFutureKnock.weatherDuration = 5;
		ShouldChangeFormOnWeatherStart(battlerId);
		return TRUE;
	}
	return FALSE;
}

bool8 IsMoveMakingContact(u8 battler, u16 move)
{
	if (GetBattlerAbility(battler) != ABILITY_LONG_REACH && gBattleMoves[move].flags.makesContact)
		return TRUE;
	return FALSE;
}

bool8 IsBattlerProtected(u8 battlerId, u16 move)
{
	if (GetBattlerAbility(gBattlerAttacker) == ABILITY_UNSEEN_FIST && IsMoveMakingContact(gBattlerAttacker, move))
		return FALSE;
	else if (gProtectStructs[battlerId].protected && !gBattleMoves[move].flags.forbiddenProtect)
		return TRUE;
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
					gLastUsedAbility = abilityId;
					ret = i + 1;
				}
			}
			break;
		case CHECK_ABILITY_ON_SIDE:
		    for (i = 0; i < gBattlersCount; i++)
			{
				if (IsBattlerAlive(i) && GetBattlerSide(i) == GetBattlerSide(battlerId) && GetBattlerAbility(i) == abilityId)
				{
					gLastUsedAbility = abilityId;
					ret = i + 1;
				}
			}
			break;
	}
	return ret;
}

// Don't call Neutralizing Gas and Unnerve here bc at the point this function is called the two abilities already has ben called before
bool8 DoSwitchInAbilitiesItems(u8 battlerId)
{
	if (AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, battlerId, MOVE_NONE) || AbilityBattleEffects(ABILITYEFFECT_ON_WEATHER, battlerId, MOVE_NONE)
		|| AbilityBattleEffects(ABILITYEFFECT_ON_TERRAIN, battlerId, MOVE_NONE) || AbilityBattleEffects(ABILITYEFFECT_TRACE2, 0, MOVE_NONE)
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
		if (gBattleMons[battlerId].statStages[i] > 6)
			count += gBattleMons[battlerId].statStages[i] - 6;
	}
	return count;
}

bool8 IsBattlerGrounded(u8 battlerId)
{
	if (gStatuses3[battlerId] & STATUS3_ROOTED)
		return TRUE;
	else if (GetBattlerAbility(battlerId) == ABILITY_LEVITATE || IS_BATTLER_OF_TYPE(battlerId, TYPE_FLYING))
		return FALSE;
	return TRUE;
}

static bool8 CanBattlerGetOrLoseItem(u8 battlerId, u16 itemId)
{
	u16 ability = GetBattlerAbility(battlerId), species = gBattleMons[battlerId].species;
	u32 personality = gBattleMons[battlerId].personality;
	
	if (GetBattlerItemHoldEffect(battlerId, FALSE) == HOLD_EFFECT_Z_CRYSTAL || itemId == ITEM_ENIGMA_BERRY || ItemIsMail(itemId)
	|| GetSpeciesFormChange(FORM_CHANGE_HOLD_ITEM, species, personality, ability, itemId, 0, FALSE)
	|| GetSpeciesFormChange(FORM_CHANGE_MEGA_EVO, species, personality, ability, itemId, 0, FALSE)
	|| GetSpeciesFormChange(FORM_CHANGE_PRIMAL, species, personality, ability, itemId, 0, FALSE)
	|| GetSpeciesFormChange(FORM_CHANGE_ULTRA_BURST, species, personality, ability, itemId, 0, FALSE))
	    return FALSE;
	return TRUE;
}

bool8 CanStealItem(u8 battlerAtk, u8 battlerDef, u16 itemId)
{
	u8 battlerAtkSide = GetBattlerSide(battlerAtk);
	bool8 battleTypeBlocksItemSteal = !((gBattleTypeFlags & (BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_LINK))
	&& gTrainerBattleOpponent_A != 0x400);
	
	if (((battlerAtkSide == B_SIDE_OPPONENT || gWishFutureKnock.knockedOffMons[battlerAtkSide] & gBitTable[gBattlerPartyIndexes[battlerAtk]])
		&& battleTypeBlocksItemSteal) || !CanBattlerGetOrLoseItem(battlerDef, itemId) || !CanBattlerGetOrLoseItem(battlerAtk, itemId))
		return FALSE;
		
	return TRUE;
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

u8 CountUsablePartyMons(u8 battlerId)
{
	u8 i, ret, battlerOnField2, battlerOnField1 = gBattlerPartyIndexes[battlerId];
	struct Pokemon *party = GetBattlerParty(battlerId);
	
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
		battlerOnField2 = gBattlerPartyIndexes[BATTLE_PARTNER(battlerId)];
	else // In singles there's only one battlerId by side
		battlerOnField2 = battlerOnField1;
		
	for (i = 0, ret = 0; i < PARTY_SIZE; i++)
	{
		if (i != battlerOnField1 && i != battlerOnField2 && GetMonData(&party[i], MON_DATA_SPECIES) && !GetMonData(&party[i], MON_DATA_IS_EGG)
			&& GetMonData(&party[i], MON_DATA_HP))
			++ret;
	}
	return ret;
}

bool8 CanBattlerEscape(u8 battlerId)
{
	if (IS_BATTLER_OF_TYPE(battlerId, TYPE_GHOST))
		return TRUE;
	else if (gBattleMons[battlerId].status2 & (STATUS2_ESCAPE_PREVENTION | STATUS2_WRAPPED) || gStatuses3[battlerId] & STATUS3_ROOTED)
		return FALSE;
	else
		return TRUE;
}

u8 IsAbilityPreventingSwitchOut(u8 battlerId)
{
	u8 ret;
	
	if (!IS_BATTLER_OF_TYPE(battlerId, TYPE_GHOST))
	{
		if (IsBattlerGrounded(battlerId) && (ret = ABILITY_ON_OPPOSING_SIDE(battlerId, ABILITY_ARENA_TRAP)))
			return ret;
		else if (GetBattlerAbility(battlerId) != ABILITY_SHADOW_TAG && (ret = ABILITY_ON_OPPOSING_SIDE(battlerId, ABILITY_SHADOW_TAG)))
			return ret;
		else if (IS_BATTLER_OF_TYPE(battlerId, TYPE_STEEL) && (ret = ABILITY_ON_OPPOSING_SIDE(battlerId, ABILITY_MAGNET_PULL)))
			return ret;
	}
	return 0;
}

bool8 CanBattlerSwitch(u8 battlerId)
{
	u8 i, battlerIn1, battlerIn2, lastMonId;
    struct Pokemon *party = GetBattlerParty(battlerId);
	
    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        if (GetLinkTrainerFlankId(GetBattlerMultiplayerId(battlerId)) == TRUE)
            i = 3;
		else
			i = 0;
		
        for (lastMonId = i + 3; i < lastMonId; ++i)
        {
            if (GetMonData(&party[i], MON_DATA_SPECIES) && !GetMonData(&party[i], MON_DATA_IS_EGG) && GetMonData(&party[i], MON_DATA_HP)
				&& gBattlerPartyIndexes[battlerId] != i)
                break;
        }
    }
    else
    {
        if (GetBattlerSide(battlerId) == B_SIDE_OPPONENT)
        {
            battlerIn1 = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
		
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                battlerIn2 = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
            else
                battlerIn2 = battlerIn1;
        }
        else
        {
            battlerIn1 = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
		
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                battlerIn2 = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
            else
                battlerIn2 = battlerIn1;
        }
        for (i = 0, lastMonId = PARTY_SIZE; i < PARTY_SIZE; ++i)
        {
            if (GetMonData(&party[i], MON_DATA_HP) && GetMonData(&party[i], MON_DATA_SPECIES) && !GetMonData(&party[i], MON_DATA_IS_EGG)
				&& i != gBattlerPartyIndexes[battlerIn1] && i != gBattlerPartyIndexes[battlerIn2])
                break;
        }
    }
	return (i != lastMonId);
}

u8 GetBattlerTurnOrderNum(u8 battlerId)
{
    u8 i;
    
    for (i = 0; i < gBattlersCount && gBattlerByTurnOrder[i] != battlerId; ++i) {};
	
	return i;
}

u32 GetEffectChanceIncreases(u8 battlerId, u32 secondaryEffectChance)
{
	if (GetBattlerAbility(battlerId) == ABILITY_SERENE_GRACE) // Check Serene Grace
		secondaryEffectChance *= 2;
	
	return secondaryEffectChance;
}

bool8 UproarWakeUpCheck(void)
{
    u8 i;

    for (i = 0; i < gBattlersCount; ++i)
    {
        if (gBattleMons[i].status2 & STATUS2_UPROAR)
		{
			gBattleScripting.battler = i;
			
			if (gBattlerTarget == 0xFF)
				gBattlerTarget = i;
			else if (gBattlerTarget == i)
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
			else
				gBattleCommunication[MULTISTRING_CHOOSER] = 1;
			break;
		}
    }
    if (i == gBattlersCount)
        return FALSE;
	
	return TRUE;
}

bool8 TryResetBattlerStatChanges(u8 battlerId)
{
	u8 i;
	bool8 statReseted = FALSE;
	
	for (i = 0; i < NUM_BATTLE_STATS; i++)
	{
		if (gBattleMons[battlerId].statStages[i] != 6)
		{
			gBattleMons[battlerId].statStages[i] = 6;
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
bool8 CanBeInfatuatedBy(u8 battlerIdAtk, u8 battlerIdDef)
{
	if (GetBattlerGender(battlerIdAtk) == MON_GENDERLESS || GetBattlerGender(battlerIdDef) == MON_GENDERLESS || ARE_BATTLERS_OF_SAME_GENDER(battlerIdAtk, battlerIdDef)
		|| GetBattlerAbility(battlerIdAtk) == ABILITY_OBLIVIOUS || gBattleMons[battlerIdAtk].status2 & STATUS2_INFATUATION)
	    return FALSE;
	return TRUE;
}

u32 GetBattlerWeight(u8 battlerId)
{
	u32 weight = GetPokedexHeightWeight(SpeciesToNationalPokedexNum(gBattleMons[battlerId].species), 1);
	
	switch (GetBattlerAbility(battlerId))
	{
		case ABILITY_HEAVY_METAL:
			weight *= 2;
			break;
		case ABILITY_LIGHT_METAL:
			weight /= 2;
			break;
	}
	return weight;
}

bool8 IsBattlerAffectedBySpore(u8 battlerId)
{
	if (IS_BATTLER_OF_TYPE(battlerId, TYPE_GRASS) || GetBattlerAbility(battlerId) == ABILITY_OVERCOAT)
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
	u16 *usedHeldItem = &gBattleStruct->usedHeldItems[gBattlerPartyIndexes[battlerItem]][GetBattlerSide(battlerItem)];
	
	if (*usedHeldItem && !gBattleMons[battlerRecycler].item)
	{
		gLastUsedItem = *usedHeldItem;
        *usedHeldItem = ITEM_NONE;
		
		gBattleMons[battlerRecycler].item = gLastUsedItem;
		
		RemoveOrAddBattlerOnPickupStack(battlerItem, FALSE);
		
		gBattleResources->flags->flags[battlerRecycler] &= ~(RESOURCE_FLAG_UNBURDEN_BOOST);
		
		if (battlerRecycler != battlerItem)
			CheckSetBattlerUnburden(battlerItem);
		
		gActiveBattler = battlerRecycler;
		BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
        MarkBattlerForControllerExec(gActiveBattler);
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

bool8 MoveHasHealingEffect(u16 move)
{
	switch (gBattleMoves[move].effect)
	{
		case EFFECT_RESTORE_HP:
		case EFFECT_REST:
		case EFFECT_MORNING_SUN:
		case EFFECT_MOONLIGHT:
		case EFFECT_SYNTHESIS:
		case EFFECT_HEAL_PULSE:
		case EFFECT_HEALING_WISH:
		case EFFECT_SWALLOW:
		case EFFECT_WISH:
		case EFFECT_SOFTBOILED:
		case EFFECT_ABSORB:
		case EFFECT_ROOST:
		    return TRUE;
	}
	return FALSE;
}

bool8 IsBattlerAffectedByFollowMe(u8 battlerId, u8 opposingSide, u16 move)
{
	u16 atkAbility = GetBattlerAbility(battlerId);
	
	if (!gSideTimers[opposingSide].followmeTimer || !IsBattlerAlive(gSideTimers[opposingSide].followmeTarget) || atkAbility == ABILITY_PROPELLER_TAIL
	|| atkAbility == ABILITY_STALWART)
	    return FALSE;
		
	return TRUE;
}

bool8 IsAbilityBlockedByNeutralizingGas(u16 ability)
{
	switch (ability)
	{
		case ABILITY_NEUTRALIZING_GAS:
		case ABILITY_MULTITYPE:
		case ABILITY_ZEN_MODE:
		case ABILITY_STANCE_CHANGE:
		case ABILITY_POWER_CONSTRUCT:
		case ABILITY_SCHOOLING:
		case ABILITY_RKS_SYSTEM:
		case ABILITY_SHIELDS_DOWN:
		case ABILITY_BATTLE_BOND:
		case ABILITY_COMATOSE:
		case ABILITY_DISGUISE:
		case ABILITY_GULP_MISSILE:
		case ABILITY_ICE_FACE:
		case ABILITY_AS_ONE_ICE_RIDER:
		case ABILITY_AS_ONE_SHADOW_RIDER:
		    return FALSE;
	}
	return TRUE;
}

u8 GetBattleMonForItemUse(u8 partyIndex)
{
	u8 battleMonId = MAX_BATTLERS_COUNT;
	
	if (gMain.inBattle)
	{
		if (partyIndex == gBattlerPartyIndexes[gBattlerAttacker])
			battleMonId = gBattlerAttacker;
		else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && partyIndex == gBattlerPartyIndexes[BATTLE_PARTNER(gBattlerAttacker)])
			battleMonId = BATTLE_PARTNER(gBattlerAttacker);
	}
	return battleMonId;
}

u8 GetItemUseBattler(u8 battlerId)
{
	return GetBattleMonForItemUse(gBattleStruct->itemPartyIndex[battlerId]);
}

bool8 IsItemUseBlockedByBattleEffect(void)
{
	bool8 blocked = FALSE;
	u8 playerLeft = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), playerRight = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
	
	if (gPartyMenu.slotId == 0) // player left mon effects check
	{
		if ((gStatuses3[playerLeft] & STATUS3_EMBARGO))
			blocked = TRUE;
	}
	else if (gPartyMenu.slotId == 1) // player right mon effects check
	{
		if ((gStatuses3[playerRight] & STATUS3_EMBARGO))
			blocked = TRUE;
	}
	return blocked;
}

#define BALL_THROW_SUCCESS                  0
#define BALL_THROW_UNABLE_TWO_MONS          1
#define BALL_THROW_UNABLE_NO_ROOM           2
#define BALL_THROW_UNABLE_SEMI_INVULNERABLE 3

static u8 GetBallThrowableState(void)
{
	if (IsBattlerAlive(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)) && IsBattlerAlive(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT)))
		return BALL_THROW_UNABLE_TWO_MONS;
	else if (IsPlayerPartyAndPokemonStorageFull())
		return BALL_THROW_UNABLE_NO_ROOM;
	else if (gStatuses3[GetCatchingBattler()] & STATUS3_SEMI_INVULNERABLE)
		return BALL_THROW_UNABLE_SEMI_INVULNERABLE;
	else
		return BALL_THROW_SUCCESS;
}

const u8 *PokemonUseItemEffectsBattle(u8 battlerId, u16 itemId, bool8 *canUse)
{
	const u8 *failStr = NULL;
	u8 i, holdEffectParam = ItemId_GetHoldEffectParam(itemId);
	
	switch (ItemId_GetBattleUsage(itemId))
	{
		case EFFECT_ITEM_INCREASE_STAT:
		    if (CompareStat(battlerId, holdEffectParam, 12, CMP_LESS_THAN))
				*canUse = TRUE;
			break;
		case EFFECT_ITEM_SET_FOCUS_ENERGY:
		    if (!(gBattleMons[battlerId].status2 & STATUS2_FOCUS_ENERGY))
				*canUse = TRUE;
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
				    failStr = gText_CantThrowPokeBall_TwoMons;
					break;
				case BALL_THROW_UNABLE_NO_ROOM:
				    failStr = gUnknown_8416631;
					break;
				case BALL_THROW_UNABLE_SEMI_INVULNERABLE:
				    failStr = gText_CantThrowPokeBall_SemiInvulnerable;
					break;
			}
			break;
		case EFFECT_ITEM_INCREASE_ALL_STATS:
		    for (i = STAT_ATK; i < NUM_STATS; i++)
			{
				if (CompareStat(battlerId, i, 12, CMP_LESS_THAN))
					*canUse = TRUE;
			}
			break;
		case EFFECT_ITEM_POKE_FLUTE:
			*canUse = TRUE; // Always can be used
			break;
	}
	return failStr;
}

bool8 IsBattleAnimationsOn(void)
{
	if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_POKEDUDE)) && gSaveBlock2Ptr->optionsBattleSceneOff)
		return FALSE;
	return TRUE;
}

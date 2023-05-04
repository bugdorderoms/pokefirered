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
#include "battle.h"
#include "battle_anim.h"
#include "battle_scripts.h"
#include "battle_message.h"
#include "constants/battle_anim.h"
#include "battle_controllers.h"
#include "battle_string_ids.h"
#include "battle_ai_script_commands.h"
#include "calculate_base_damage.h"
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

//used strings
static const u8 sBadDreamsString[] = _("{B_DEF_NAME_WITH_PREFIX} is\ntormented!");
static const u8 sDrySkinRainString[] = _("{B_ATK_NAME_WITH_PREFIX} gain some\nof its HP!");
static const u8 sDrySkinSunString[] = _("{B_ATK_NAME_WITH_PREFIX} lost some\nof its HP!");
static const u8 sMoldBreakerString[] = _("{B_ATK_NAME_WITH_PREFIX} breaks the mold!");
static const u8 sAngerPointString[] = _("{B_DEF_NAME_WITH_PREFIX} maxed its\nAttack!");
static const u8 sMoveStatUpString[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_LAST_ABILITY}\nraised its {B_BUFF1}!");
static const u8 sSlowStartStartString[] = _("{B_ATK_NAME_WITH_PREFIX} can't\nget it going!");
static const u8 sSlowStartEndString[] = _("{B_ATK_NAME_WITH_PREFIX} got its\nact together!");
static const u8 sSnowWarningString[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nmade it hail!");
static const u8 sSteadfastString[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY}\nraised its Speed!");
static const u8 sUnnerveString[] = _("{B_DEF_SIDE_NAME} is too\nnervous to eat berries!");
static const u8 sAirLockString[] = _("The effects of weather\ndisappeared!");
static const u8 sCursedBodyString[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\ndisabled {B_ATK_NAME_WITH_PREFIX}'s\l{B_CURRENT_MOVE}!");
static const u8 sHealerString[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY}\ncured {B_EFF_NAME_WITH_PREFIX}'s\l{B_BUFF1} problem!");
static const u8 sHarvestString[] = _("{B_ATK_NAME_WITH_PREFIX} harvested\nits {B_LAST_ITEM}!");
static const u8 sIllusionOffString[] = _("{B_DEF_NAME_WITH_PREFIX}'s illusion wore off!");
static const u8 sMummyString[] = _("{B_ATK_NAME_WITH_PREFIX}'s ability became\n{B_ATK_ABILITY}!");
static const u8 sZenModeString[] = _("{B_ATK_ABILITY} triggered!");
static const u8 sTurboblazeString[] = _("{B_ATK_NAME_WITH_PREFIX} is radiating\na blazing aura!");
static const u8 sTeravoltString[] = _("{B_ATK_NAME_WITH_PREFIX} is radiating\na bursting aura!");
static const u8 sDarkAuraString[] = _("{B_ATK_NAME_WITH_PREFIX} is radiating\na dark aura!");
static const u8 sFairyAuraString[] = _("{B_ATK_NAME_WITH_PREFIX} is radiating\na fairy aura!");
static const u8 sAuraBreakString[] = _("{B_ATK_NAME_WITH_PREFIX} reversed all\nother Pokémon's auras!");
static const u8 sPrimordialSeaString[] = _("A heavy rain began to fall!");
static const u8 sDesolateLandString[] = _("The sunlight turned\nextremely harsh!");
static const u8 sDeltaStreamString[] = _("A mysterious air current is\nprotecting Flying-type Pokémon!");
static const u8 sComatoseString[] = _("{B_ATK_NAME_WITH_PREFIX} is drowsing!");
static const u8 sDazzlingString[] = _("{B_ATK_NAME_WITH_PREFIX} cannot\nuse {B_CURRENT_MOVE}!");
static const u8 sZeroToHeroString[] = _("{B_ATK_NAME_WITH_PREFIX} underwent\na heroic transformation!");

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
		    return FALSE;
	}
	if (!(gHitMarker & HITMARKER_IGNORE_SAFEGUARD))
	{
		if (checkFlowerVeil && IsBattlerAlive(BATTLE_PARTNER(bank)) && GetBattlerAbility(BATTLE_PARTNER(bank)) == ABILITY_FLOWER_VEIL && IS_BATTLER_OF_TYPE(bank, TYPE_GRASS))
			return FALSE;
		
		if (gSideStatuses[GET_BATTLER_SIDE(bank)] & SIDE_STATUS_SAFEGUARD)
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
	if (IsBattlerAlive(BATTLE_PARTNER(bank)) && GetBattlerAbility(BATTLE_PARTNER(bank)) == ABILITY_SWEET_VEIL)
		return FALSE;
	
	return TRUE;
}

bool8 CanBePoisoned(u8 bankDef, u8 bankAtk, bool8 checkFlowerVeil)
{
	if (!CanBeStatused(bankDef, checkFlowerVeil))
		return FALSE;
	
	switch (GetBattlerAbility(bankDef))
	{
		case ABILITY_IMMUNITY:
			return FALSE;
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
    case BS_BATTLER_0:
        ret = 0;
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
    case 4:
    case 6:
    case 8:
    case 9:
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
    gBattleMons[battler].status2 &= ~(STATUS2_MULTIPLETURNS);
    gBattleMons[battler].status2 &= ~(STATUS2_LOCK_CONFUSE);
    gBattleMons[battler].status2 &= ~(STATUS2_UPROAR);
    gBattleMons[battler].status2 &= ~(STATUS2_BIDE);
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

u16 GetBattlerAbility(u8 battler)
{
	u16 ability = gBattleMons[battler].ability;
	
	if ((gBattleMons[gBattlerAttacker].ability == ABILITY_MOLD_BREAKER || gBattleMons[gBattlerAttacker].ability == ABILITY_TERAVOLT
		|| gBattleMons[gBattlerAttacker].ability == ABILITY_TURBOBLAZE) && sIgnorableAbilities[ability] == TRUE
		&& gBattlerByTurnOrder[gCurrentTurnActionNumber] == gBattlerAttacker && gActionsByTurnOrder[gBattlerByTurnOrder[gBattlerAttacker]] == B_ACTION_USE_MOVE
		&& gCurrentTurnActionNumber < gBattlersCount)
		ability = ABILITY_NONE;
	
	return ability;
}

static void TryRemoveMonUnburdenBoost(u8 battler)
{
	if (GetBattlerAbility(battler) != ABILITY_UNBURDEN || gBattleMons[battler].item)
		gNewBattleStruct.UnburdenBoostBits &= ~(gBitTable[battler]);
}

void ResetVarsForAbilityChange(u8 battler) // used in abilities change
{
	TryRemoveMonUnburdenBoost(battler);
}

void TryGiveUnburdenBoostToMon(u8 battler)
{
	// used in items change
	if (GetBattlerAbility(battler) == ABILITY_UNBURDEN && !gBattleMons[battler].item)
		gNewBattleStruct.UnburdenBoostBits |= gBitTable[battler];
	else
		TryRemoveMonUnburdenBoost(battler);
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
	if ((stringId == STRINGID_PKMNSSTATCHANGED4 || stringId == STRINGID_PKMNCUTSATTACKWITH) && gSpecialStatuses[gBattlerTarget].changedStatsBattlerId != BATTLE_PARTNER(gBattlerTarget)
	&& gSpecialStatuses[gBattlerTarget].changedStatsBattlerId != gBattlerTarget)
	{
		switch (GetBattlerAbility(gBattlerTarget))
		{
			case ABILITY_DEFIANT:
			    if (gBattleMons[gBattlerTarget].statStages[STAT_ATK] < 0xC)
				{
					gLastUsedAbility = ABILITY_DEFIANT;
					SET_STATCHANGER(STAT_ATK, 2, FALSE);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_DefiantCompetitive;
				}
			    break;
			case ABILITY_COMPETITIVE:
			    if (gBattleMons[gBattlerTarget].statStages[STAT_SPATK] < 0xC)
				{
					gLastUsedAbility = ABILITY_COMPETITIVE;
					SET_STATCHANGER(STAT_SPATK, 2, FALSE);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_DefiantCompetitive;
				}
			    break;
		}
	}
	if (stringId == STRINGID_PKMNCUTSATTACKWITH && GetBattlerAbility(gBattlerTarget) == ABILITY_RATTLED && gBattleMons[gBattlerTarget].statStages[STAT_SPEED] < 0xC)
	{
		gLastUsedAbility = ABILITY_RATTLED;
		SET_STATCHANGER(STAT_SPEED, 1, FALSE);
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_DefiantCompetitive;
	}
}

void PrepareStringBattle(u16 stringId, u8 battler)
{
    TryActivateDefiant(stringId);
    gActiveBattler = battler;
    BtlController_EmitPrintString(0, stringId);
    MarkBattlerForControllerExec(gActiveBattler);
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
    if (holdEffect == HOLD_EFFECT_CHOICE_BAND && *choicedMove && *choicedMove != 0xFFFF && *choicedMove != move)
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
        if (holdEffect == HOLD_EFFECT_CHOICE_BAND && *choicedMove != 0 && *choicedMove != 0xFFFF && *choicedMove != gBattleMons[battlerId].moves[i])
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
            while (gBattleStruct->turnSideTracker < 2)
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
            while (gBattleStruct->turnSideTracker < 2)
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
            while (gBattleStruct->turnSideTracker < 2)
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
            while (gBattleStruct->turnSideTracker < 2)
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
                if (AbilityBattleEffects(ABILITYEFFECT_ENDTURN, gActiveBattler, 0, 0, 0))
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
                    gBattleMons[gActiveBattler].status2 -= 0x2000;
                    if (gBattleMons[gActiveBattler].status2 & STATUS2_WRAPPED)  // damaged by wrap
                    {
                        gBattleScripting.animArg1 = *(gBattleStruct->wrappedMove + gActiveBattler * 2 + 0);
                        gBattleScripting.animArg2 = *(gBattleStruct->wrappedMove + gActiveBattler * 2 + 1);
                        gBattleTextBuff1[0] = B_BUFF_PLACEHOLDER_BEGIN;
                        gBattleTextBuff1[1] = B_BUFF_MOVE;
                        gBattleTextBuff1[2] = *(gBattleStruct->wrappedMove + gActiveBattler * 2 + 0);
                        gBattleTextBuff1[3] = *(gBattleStruct->wrappedMove + gActiveBattler * 2 + 1);
                        gBattleTextBuff1[4] = EOS;
                        gBattlescriptCurrInstr = BattleScript_WrapTurnDmg;
                        gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 16;
                        if (gBattleMoveDamage == 0)
                            gBattleMoveDamage = 1;
                    }
                    else  // broke free
                    {
                        gBattleTextBuff1[0] = B_BUFF_PLACEHOLDER_BEGIN;
                        gBattleTextBuff1[1] = B_BUFF_MOVE;
                        gBattleTextBuff1[2] = *(gBattleStruct->wrappedMove + gActiveBattler * 2 + 0);
                        gBattleTextBuff1[3] = *(gBattleStruct->wrappedMove + gActiveBattler * 2 + 1);
                        gBattleTextBuff1[4] = EOS;
                        gBattlescriptCurrInstr = BattleScript_WrapEnds;
                    }
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
                            gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_CONFUSION | MOVE_EFFECT_AFFECTS_USER;
                            SetMoveEffect(1, 0);
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
                    if (!(gStatuses3[gActiveBattler] & STATUS3_YAWN) && !UproarWakeUpCheck(gActiveBattler) && !(gBattleMons[gActiveBattler].status1 & STATUS1_ANY))
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

#define FAINTED_ACTIONS_MAX_CASE 7

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
                if (gAbsentBattlerFlags & gBitTable[i] && !HasNoMonsToSwitch(i, 6, 6))
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
            break;
        case 3:
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
        case 6:
            if (AbilityBattleEffects(ABILITYEFFECT_INTIMIDATE1, 0, 0, 0, 0) || AbilityBattleEffects(ABILITYEFFECT_TRACE, 0, 0, 0, 0) || ItemBattleEffects(1, 0, TRUE) || AbilityBattleEffects(ABILITYEFFECT_FORECAST, 0, 0, 0, 0))
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

enum
{
    CANCELLER_FLAGS,
    CANCELLER_ASLEEP,
    CANCELLER_FROZEN,
    CANCELLER_TRUANT,
    CANCELLER_RECHARGE,
    CANCELLER_FLINCH,
    CANCELLER_DISABLED,
    CANCELLER_TAUNTED,
    CANCELLER_IMPRISONED,
    CANCELLER_CONFUSED,
    CANCELLER_PARALYSED,
    CANCELLER_GHOST,
    CANCELLER_IN_LOVE,
    CANCELLER_BIDE,
    CANCELLER_THAW,
    CANCELLER_POWDER_MOVE,
    CANCELLER_END,
};

u8 AtkCanceller_UnableToUseMove(void)
{
    u8 effect = 0;
    s32 *bideDmg = &gBattleScripting.bideDmg;

    do
    {
        switch (gBattleStruct->atkCancellerTracker)
        {
        case CANCELLER_FLAGS: // flags clear
            gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_DESTINY_BOND);
            gStatuses3[gBattlerAttacker] &= ~(STATUS3_GRUDGE);
            ++gBattleStruct->atkCancellerTracker;
            break;
        case CANCELLER_ASLEEP: // check being asleep
            if (gBattleMons[gBattlerAttacker].status1 & STATUS1_SLEEP)
            {
                if (UproarWakeUpCheck(gBattlerAttacker))
                {
                    gBattleMons[gBattlerAttacker].status1 &= ~(STATUS1_SLEEP);
                    gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_NIGHTMARE);
                    BattleScriptPushCursor();
                    gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                    gBattlescriptCurrInstr = BattleScript_MoveUsedWokeUp;
                    effect = 2;
                }
                else
                {
                    u8 toSub;

                    if (GetBattlerAbility(gBattlerAttacker) == ABILITY_EARLY_BIRD)
                        toSub = 2;
                    else
                        toSub = 1;
                    if ((gBattleMons[gBattlerAttacker].status1 & STATUS1_SLEEP) < toSub)
                        gBattleMons[gBattlerAttacker].status1 &= ~(STATUS1_SLEEP);
                    else
                        gBattleMons[gBattlerAttacker].status1 -= toSub;
                    if (gBattleMons[gBattlerAttacker].status1 & STATUS1_SLEEP)
                    {
                        if (gCurrentMove != MOVE_SNORE && gCurrentMove != MOVE_SLEEP_TALK)
                        {
                            gBattlescriptCurrInstr = BattleScript_MoveUsedIsAsleep;
                            gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                            effect = 2;
                        }
                    }
                    else
                    {
                        gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_NIGHTMARE);
                        BattleScriptPushCursor();
                        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                        gBattlescriptCurrInstr = BattleScript_MoveUsedWokeUp;
                        effect = 2;
                    }
                }
            }
            ++gBattleStruct->atkCancellerTracker;
            break;
        case CANCELLER_FROZEN: // check being frozen
            if (gBattleMons[gBattlerAttacker].status1 & STATUS1_FREEZE)
            {
                if (Random() % 5)
                {
                    if (gBattleMoves[gCurrentMove].effect != EFFECT_THAW_HIT) // unfreezing via a move effect happens in case 13
                    {
                        gBattlescriptCurrInstr = BattleScript_MoveUsedIsFrozen;
                        gHitMarker |= HITMARKER_NO_ATTACKSTRING;
                    }
                    else
                    {
                        ++gBattleStruct->atkCancellerTracker;
                        break;
                    }
                }
                else // unfreeze
                {
                    gBattleMons[gBattlerAttacker].status1 &= ~(STATUS1_FREEZE);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_MoveUsedUnfroze;
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                }
                effect = 2;
            }
            ++gBattleStruct->atkCancellerTracker;
            break;
        case CANCELLER_TRUANT: // truant
            if (GetBattlerAbility(gBattlerAttacker) == ABILITY_TRUANT && gDisableStructs[gBattlerAttacker].truantCounter)
            {
                CancelMultiTurnMoves(gBattlerAttacker);
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                gBattlescriptCurrInstr = BattleScript_MoveUsedLoafingAround;
                gMoveResultFlags |= MOVE_RESULT_MISSED;
                effect = 1;
            }
            ++gBattleStruct->atkCancellerTracker;
            break;
        case CANCELLER_RECHARGE: // recharge
            if (gBattleMons[gBattlerAttacker].status2 & STATUS2_RECHARGE)
            {
                gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_RECHARGE);
                gDisableStructs[gBattlerAttacker].rechargeTimer = 0;
                CancelMultiTurnMoves(gBattlerAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedMustRecharge;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            ++gBattleStruct->atkCancellerTracker;
            break;
        case CANCELLER_FLINCH: // flinch
            if (gBattleMons[gBattlerAttacker].status2 & STATUS2_FLINCHED)
            {
		gSetWordLoc = sSteadfastString; // in case of steadfast activate
                gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_FLINCHED);
                gProtectStructs[gBattlerAttacker].flinchImmobility = 1;
                CancelMultiTurnMoves(gBattlerAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedFlinched;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            ++gBattleStruct->atkCancellerTracker;
            break;
        case CANCELLER_DISABLED: // disabled move
            if (gDisableStructs[gBattlerAttacker].disabledMove == gCurrentMove && gDisableStructs[gBattlerAttacker].disabledMove != MOVE_NONE)
            {
                gProtectStructs[gBattlerAttacker].usedDisabledMove = 1;
                gBattleScripting.battler = gBattlerAttacker;
                CancelMultiTurnMoves(gBattlerAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedIsDisabled;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            ++gBattleStruct->atkCancellerTracker;
            break;
        case CANCELLER_TAUNTED: // taunt
            if (gDisableStructs[gBattlerAttacker].tauntTimer && gBattleMoves[gCurrentMove].power == 0)
            {
                gProtectStructs[gBattlerAttacker].usedTauntedMove = 1;
                CancelMultiTurnMoves(gBattlerAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedIsTaunted;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            ++gBattleStruct->atkCancellerTracker;
            break;
        case CANCELLER_IMPRISONED: // imprisoned
            if (GetImprisonedMovesCount(gBattlerAttacker, gCurrentMove))
            {
                gProtectStructs[gBattlerAttacker].usedImprisonedMove = 1;
                CancelMultiTurnMoves(gBattlerAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedIsImprisoned;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
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
                        gBattleMoveDamage = CalculateBaseDamage(MOVE_POUND, TYPE_NORMAL, gBattlerAttacker, gBattlerTarget, TRUE, FALSE, FALSE);
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
                effect = 1;
            }
            ++gBattleStruct->atkCancellerTracker;
            break;
        case CANCELLER_PARALYSED: // paralysis
            if ((gBattleMons[gBattlerAttacker].status1 & STATUS1_PARALYSIS) && (Random() % 4) == 0)
            {
                gProtectStructs[gBattlerAttacker].prlzImmobility = 1;
                gBattlescriptCurrInstr = BattleScript_MoveUsedIsParalyzed;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            ++gBattleStruct->atkCancellerTracker;
            break;
        case CANCELLER_GHOST: // GHOST in pokemon tower
            if (IS_BATTLE_TYPE_GHOST_WITHOUT_SCOPE(gBattleTypeFlags))
            {
                if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
                    gBattlescriptCurrInstr = BattleScript_TooScaredToMove;
                else
                    gBattlescriptCurrInstr = BattleScript_GhostGetOutGetOut;
                gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                effect = 1;
            }
            ++gBattleStruct->atkCancellerTracker;
            break;
        case CANCELLER_IN_LOVE: // infatuation
            if (gBattleMons[gBattlerAttacker].status2 & STATUS2_INFATUATION)
            {
                gBattleScripting.battler = CountTrailingZeroBits((gBattleMons[gBattlerAttacker].status2 & STATUS2_INFATUATION) >> 0x10);
                if (Random() & 1)
                {
                    BattleScriptPushCursor();
                }
                else
                {
                    BattleScriptPush(BattleScript_MoveUsedIsInLoveCantAttack);
                    gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                    gProtectStructs[gBattlerAttacker].loveImmobility = 1;
                    CancelMultiTurnMoves(gBattlerAttacker);
                }
                gBattlescriptCurrInstr = BattleScript_MoveUsedIsInLove;
                effect = 1;
            }
            ++gBattleStruct->atkCancellerTracker;
            break;
        case CANCELLER_BIDE: // bide
            if (gBattleMons[gBattlerAttacker].status2 & STATUS2_BIDE)
            {
                gBattleMons[gBattlerAttacker].status2 -= 0x100;
                if (gBattleMons[gBattlerAttacker].status2 & STATUS2_BIDE)
                {
                    gBattlescriptCurrInstr = BattleScript_BideStoringEnergy;
                }
                else
                {
                    if (gTakenDmg[gBattlerAttacker])
                    {
                        gCurrentMove = MOVE_BIDE;
                        *bideDmg = gTakenDmg[gBattlerAttacker] * 2;
                        gBattlerTarget = gTakenDmgByBattler[gBattlerAttacker];
                        if (gAbsentBattlerFlags & gBitTable[gBattlerTarget])
                            gBattlerTarget = GetMoveTarget(MOVE_BIDE, 1);
                        gBattlescriptCurrInstr = BattleScript_BideAttack;
                    }
                    else
                    {
                        gBattlescriptCurrInstr = BattleScript_BideNoEnergyToAttack;
                    }
                }
                effect = 1;
            }
            ++gBattleStruct->atkCancellerTracker;
            break;
        case CANCELLER_THAW: // move thawing
            if (gBattleMons[gBattlerAttacker].status1 & STATUS1_FREEZE)
            {
                if (gBattleMoves[gCurrentMove].effect == EFFECT_THAW_HIT)
                {
                    gBattleMons[gBattlerAttacker].status1 &= ~(STATUS1_FREEZE);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_MoveUsedUnfroze;
                    gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                }
                effect = 2;
            }
            ++gBattleStruct->atkCancellerTracker;
            break;
	case CANCELLER_POWDER_MOVE:
	    if (gBattlerAttacker != gBattlerTarget && gBattleMoves[gCurrentMove].flags & FLAG_POWDER
		&& (GetBattlerAbility(gBattlerTarget) == ABILITY_OVERCOAT || IS_BATTLER_OF_TYPE(gBattlerTarget, TYPE_GRASS)))
	    {
		    gBattlescriptCurrInstr = BattleScript_PowderMoveNoEffect;
		    effect = 1;
	    }
	    ++gBattleStruct->atkCancellerTracker;
	    break;
        case CANCELLER_END:
            break;
        }

    } while (gBattleStruct->atkCancellerTracker != CANCELLER_END && !effect);
    if (effect == 2)
    {
        gActiveBattler = gBattlerAttacker;
        BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
        MarkBattlerForControllerExec(gActiveBattler);
    }
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
        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
        {
            playerId = GetBattlerMultiplayerId(battler);
            if (GetBattlerSide(battler) == B_SIDE_PLAYER)
                party = gPlayerParty;
            else
                party = gEnemyParty;
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
                party = gEnemyParty;
            }
            else
            {
                playerId = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
                flankId = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
                party = gPlayerParty;
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
                 && i != *(gBattleStruct->monToSwitchIntoId + playerId)
                 && i != flankId[gBattleStruct->monToSwitchIntoId])
                    break;
            }
            return (i == PARTY_SIZE);
        }
    }
}

void ClearBattlerStatus(u8 battler)
{
	if (gBattleMons[battler].status1 & (STATUS1_POISON | STATUS1_TOXIC_POISON))
		StringCopy(gBattleTextBuff1, gStatusConditionString_PoisonJpn);
	if (gBattleMons[battler].status1 & STATUS1_SLEEP)
		StringCopy(gBattleTextBuff1, gStatusConditionString_SleepJpn);
	if (gBattleMons[battler].status1 & STATUS1_PARALYSIS)
		StringCopy(gBattleTextBuff1, gStatusConditionString_ParalysisJpn);
	if (gBattleMons[battler].status1 & STATUS1_BURN)
		StringCopy(gBattleTextBuff1, gStatusConditionString_BurnJpn);
	if (gBattleMons[battler].status1 & STATUS1_FREEZE)
		StringCopy(gBattleTextBuff1, gStatusConditionString_IceJpn);
	
	gBattleMons[battler].status1 = 0;
	gBattleMons[battler].status2 &= ~(STATUS2_NIGHTMARE);  // fix nightmare glitch
	gBattleScripting.battler = gActiveBattler = battler;
	BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[battler].status1);
	MarkBattlerForControllerExec(gActiveBattler);
}

u8 AbilityBattleEffects(u8 caseID, u8 battler, u16 ability, u8 special, u16 moveArg)
{
    u8 effect = 0;
    struct Pokemon *pokeAtk;
    struct Pokemon *pokeDef;
    u16 speciesAtk;
    u16 speciesDef;
    u32 pidAtk;
    u32 pidDef;

    if (gBattlerAttacker >= gBattlersCount)
        gBattlerAttacker = battler;
	pokeAtk = GetBattlerPartyIndexPtr(gBattlerAttacker);
    if (gBattlerTarget >= gBattlersCount)
        gBattlerTarget = battler;
    pokeDef = GetBattlerPartyIndexPtr(gBattlerTarget);
    speciesAtk = GetMonData(pokeAtk, MON_DATA_SPECIES);
    pidAtk = GetMonData(pokeAtk, MON_DATA_PERSONALITY);
    speciesDef = GetMonData(pokeDef, MON_DATA_SPECIES);
    pidDef = GetMonData(pokeDef, MON_DATA_PERSONALITY);
    if (!(gBattleTypeFlags & BATTLE_TYPE_SAFARI)) // Why isn't that check done at the beginning?
    {
        u8 moveType;
        s32 i;
        u8 side;
        u8 target1;

        if (special)
            gLastUsedAbility = special;
        else
            gLastUsedAbility = GetBattlerAbility(battler);
        if (!moveArg)
            moveArg = gCurrentMove;
        moveType = gBattleStruct->dynamicMoveType;
        if (IS_BATTLE_TYPE_GHOST_WITHOUT_SCOPE(gBattleTypeFlags)
         && (gLastUsedAbility == ABILITY_INTIMIDATE 
	  || gLastUsedAbility == ABILITY_TRACE
	  || gLastUsedAbility == ABILITY_ANTICIPATION
	  || gLastUsedAbility == ABILITY_DOWNLOAD
	  || gLastUsedAbility == ABILITY_FOREWARN
	  || gLastUsedAbility == ABILITY_FRISK
	  || gLastUsedAbility == ABILITY_UNNERVE
	  || gLastUsedAbility == ABILITY_IMPOSTER))
            return effect;
        switch (caseID)
        {
        case ABILITYEFFECT_ON_SWITCHIN: // 0
            if (gBattlerAttacker >= gBattlersCount)
                gBattlerAttacker = battler;
            switch (gLastUsedAbility)
            {
            case ABILITYEFFECT_SWITCH_IN_WEATHER:
                switch (GetCurrentWeather())
                {
                case WEATHER_RAIN:
                case WEATHER_RAIN_THUNDERSTORM:
                case WEATHER_DOWNPOUR:
                    if (!(gBattleWeather & WEATHER_RAIN_ANY))
                    {
                        gBattleWeather = (WEATHER_RAIN_TEMPORARY | WEATHER_RAIN_PERMANENT);
                        gBattleScripting.animArg1 = B_ANIM_RAIN_CONTINUES;
                        gBattleScripting.battler = battler;
                        ++effect;
                    }
                    break;
                case WEATHER_SANDSTORM:
                    if (!(gBattleWeather & WEATHER_SANDSTORM_ANY))
                    {
                        gBattleWeather = (WEATHER_SANDSTORM_PERMANENT | WEATHER_SANDSTORM_TEMPORARY);
                        gBattleScripting.animArg1 = B_ANIM_SANDSTORM_CONTINUES;
                        gBattleScripting.battler = battler;
                        ++effect;
                    }
                    break;
		case WEATHER_SNOW:
                    if (!(gBattleWeather & WEATHER_HAIL_ANY))
		    {
			    gBattleWeather = (WEATHER_HAIL_PERMANENT | WEATHER_HAIL_TEMPORARY);
			    gBattleScripting.animArg1 = B_ANIM_HAIL_CONTINUES;
			    gBattleScripting.battler = battler;
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
						gBattleScripting.battler = battler;
						++effect;
					}
					break;
#endif
                }
                if (effect)
                {
                    gBattleCommunication[MULTISTRING_CHOOSER] = GetCurrentWeather();
                    BattleScriptPushCursorAndCallback(BattleScript_OverworldWeatherStarts);
                }
                break;
            case ABILITY_DRIZZLE:
                if (TryChangeBattleWeather(battler, ENUM_WEATHER_RAIN))
                {
                    gBattleWeather |= WEATHER_RAIN_PERMANENT;
                    BattleScriptPushCursorAndCallback(BattleScript_DrizzleActivates);
                    gBattleScripting.battler = battler;
                    ++effect;
                }
                break;
            case ABILITY_SAND_STREAM:
                if (TryChangeBattleWeather(battler, ENUM_WEATHER_SANDSTORM))
                {
                    gBattleWeather |= WEATHER_SANDSTORM_PERMANENT;
                    BattleScriptPushCursorAndCallback(BattleScript_SandstreamActivates);
                    gBattleScripting.battler = battler;
                    ++effect;
                }
                break;
            case ABILITY_DROUGHT:
                if (TryChangeBattleWeather(battler, ENUM_WEATHER_SUN))
                {
                    gBattleWeather |= WEATHER_SUN_PERMANENT;
                    BattleScriptPushCursorAndCallback(BattleScript_DroughtActivates);
                    gBattleScripting.battler = battler;
                    ++effect;
                }
                break;
	    case ABILITY_SNOW_WARNING:
		if (TryChangeBattleWeather(battler, ENUM_WEATHER_HAIL))
		{
		    gSetWordLoc = sSnowWarningString;
		    BattleScriptPushCursorAndCallback(BattleScript_SnowWarningActivates);
		    gBattleScripting.battler = battler;
                    ++effect;
		}
                break;
		case ABILITY_PRIMORDIAL_SEA:
		if (TryChangeBattleWeather(battler, ENUM_WEATHER_RAIN_PRIMAL))
		{
			gSetWordLoc = sPrimordialSeaString;
		    BattleScriptPushCursorAndCallback(BattleScript_PrimordialSeaActivates);
		    gBattleScripting.battler = battler;
                    ++effect;
		}
		break;
		case ABILITY_DESOLATE_LAND:
		if (TryChangeBattleWeather(battler, ENUM_WEATHER_SUN_PRIMAL))
		{
			gSetWordLoc = sDesolateLandString;
		    BattleScriptPushCursorAndCallback(BattleScript_DesolateLandActivates);
		    gBattleScripting.battler = battler;
                    ++effect;
		}
		break;
		case ABILITY_DELTA_STREAM:
		if (TryChangeBattleWeather(battler, ENUM_WEATHER_STRONG_WINDS))
		{
			gSetWordLoc = sDeltaStreamString;
		    BattleScriptPushCursorAndCallback(BattleScript_DeltaStreamActivates);
		    gBattleScripting.battler = battler;
                    ++effect;
		}
		break;
	    case ABILITY_INTIMIDATE:
	    case ABILITY_ANTICIPATION:
	    case ABILITY_DOWNLOAD:
	    case ABILITY_FOREWARN:
            case ABILITY_FRISK:
                if (!(gSpecialStatuses[battler].intimidatedMon))
                {
                    gStatuses3[battler] |= STATUS3_INTIMIDATE_POKES;
                    gSpecialStatuses[battler].intimidatedMon = 1;
                }
                break;
            case ABILITY_FORECAST:
			case ABILITY_FLOWER_GIFT:
			    {
					u16 newSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_WEATHER);
					
					if (newSpecies)
					{
						DoBattleFormChange(battler, newSpecies, TRUE, TRUE);
						BattleScriptPushCursorAndCallback(BattleScript_CastformChange);
						gBattleScripting.battler = battler;
						++effect;
					}
			    }
                break;
            case ABILITY_TRACE:
                if (!(gSpecialStatuses[battler].traced))
                {
                    gStatuses3[battler] |= STATUS3_TRACE;
                    gSpecialStatuses[battler].traced = 1;
                }
                break;
            case ABILITY_CLOUD_NINE:
            case ABILITY_AIR_LOCK:
                gSetWordLoc = sAirLockString;
	        BattleScriptPushCursorAndCallback(BattleScript_AirLock);
	        ++effect;
                break;
	    case ABILITY_MOLD_BREAKER:
	        gSetWordLoc = sMoldBreakerString;
		BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
		++effect;
		break;
	    case ABILITY_SLOW_START:
	        gDisableStructs[battler].slowStartTimer = 5;
		gSetWordLoc = sSlowStartStartString;
		BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
		++effect;
		break;
	    case ABILITY_UNNERVE:
	        gBattlerAttacker = battler;
	        gSetWordLoc = sUnnerveString;
		BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
		++effect;
		break;
	    case ABILITY_IMPOSTER:	    
		if (IsBattlerAlive(BATTLE_OPPOSITE(battler)) && !(gBattleMons[BATTLE_OPPOSITE(battler)].status2 & (STATUS2_TRANSFORMED | STATUS2_SUBSTITUTE)) && !(gStatuses3[BATTLE_OPPOSITE(battler)] & STATUS3_SEMI_INVULNERABLE)
		    && !gNewBattleStruct.illusion[BATTLE_OPPOSITE(battler)].on && !(gBattleMons[battler].status2 & STATUS2_TRANSFORMED))
		{
			gBattlerAttacker = battler;
			gBattlerTarget = BATTLE_OPPOSITE(battler);
			BattleScriptPushCursorAndCallback(BattleScript_ImposterActivates);
			effect++;
		}
		break;
		case ABILITY_TURBOBLAZE:
		    gSetWordLoc = sTurboblazeString;
		BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
		++effect;
		break;
		case ABILITY_TERAVOLT:
		    gSetWordLoc = sTeravoltString;
		BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
		++effect;
		break;
		case ABILITY_PROTEAN:
		case ABILITY_LIBERO:
		    gDisableStructs[battler].canProteanActivate = TRUE;
		break;
		case ABILITY_DARK_AURA:
		    gSetWordLoc = sDarkAuraString;
		BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
		++effect;
		break;
		case ABILITY_FAIRY_AURA:
		    gSetWordLoc = sFairyAuraString;
		BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
		++effect;
		break;
		case ABILITY_AURA_BREAK:
		    gSetWordLoc = sAuraBreakString;
		BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
		++effect;
		break;
		case ABILITY_COMATOSE:
		    gSetWordLoc = sComatoseString;
		BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
		++effect;
		break;
		case ABILITY_ZERO_TO_HERO:
		    if (gBattleStruct->zeroToHeroActivated[gBattlerPartyIndexes[battler]][GetBattlerSide(battler)])
			{
				gBattleStruct->zeroToHeroActivated[gBattlerPartyIndexes[battler]][GetBattlerSide(battler)] = FALSE;
				gSetWordLoc = sZeroToHeroString;
				BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
				++effect;
			}
			break;
		case ABILITY_ZEN_MODE:
		{
			u16 newSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_HP);
			
			if (newSpecies)
			{
				DoBattleFormChange(battler, newSpecies, TRUE, TRUE);
				gSetWordLoc = sZenModeString;
				BattleScriptPushCursorAndCallback(BattleScript_ZenModeActivates);
				++effect;
			}
			break;
		}
            }
            break;
        case ABILITYEFFECT_ENDTURN: // 1
            if (IsBattlerAlive(battler))
            {
                gBattlerAttacker = battler;
                switch (gLastUsedAbility)
                {
			case ABILITY_RAIN_DISH:
				if (IsBattlerWeatherAffected(battler, WEATHER_RAIN_ANY) && gBattleMons[battler].maxHP > gBattleMons[battler].hp)
				{
					BattleScriptPushCursorAndCallback(BattleScript_RainDishActivates);
					gBattleMoveDamage = gBattleMons[battler].maxHP / 16;
					if (gBattleMoveDamage == 0)
						gBattleMoveDamage = 1;
					gBattleMoveDamage *= -1;
					++effect;
				}
				break;
			case ABILITY_ICE_BODY:
				if (IsBattlerWeatherAffected(battler, WEATHER_HAIL_ANY) && gBattleMons[battler].maxHP > gBattleMons[battler].hp)
				{
					BattleScriptPushCursorAndCallback(BattleScript_RainDishActivates);
					gBattleMoveDamage = gBattleMons[battler].maxHP / 16;
					if (gBattleMoveDamage == 0)
						gBattleMoveDamage = 1;
					gBattleMoveDamage *= -1;
					++effect;
				}
				break;
			case ABILITY_POISON_HEAL:
				if (gBattleMons[battler].status1 & STATUS1_PSN_ANY && gBattleMons[battler].maxHP > gBattleMons[battler].hp)
				{
					BattleScriptPushCursorAndCallback(BattleScript_RainDishActivates);
					gBattleMoveDamage = gBattleMons[battler].maxHP / 8;
					if (gBattleMoveDamage == 0)
						gBattleMoveDamage = 1;
					gBattleMoveDamage *= -1;
					++effect;
				}
				break;	
			case ABILITY_DRY_SKIN:
			    if (IsBattlerWeatherAffected(battler, WEATHER_RAIN_ANY) && gBattleMons[battler].maxHP > gBattleMons[battler].hp)
				{
					gSetWordLoc = sDrySkinRainString;
					BattleScriptPushCursorAndCallback(BattleScript_DrySkinRainActivates);
					gBattleMoveDamage = gBattleMons[battler].maxHP / 8;
					if (gBattleMoveDamage == 0)
						gBattleMoveDamage = 1;
					gBattleMoveDamage *= -1;
					++effect;
				}
				else if (IsBattlerWeatherAffected(battler, WEATHER_SUN_ANY))
				{
					gSetWordLoc = sDrySkinSunString;
					BattleScriptPushCursorAndCallback(BattleScript_DrySkinSunActivates);
					gBattleMoveDamage = gBattleMons[battler].maxHP / 8;
					if (gBattleMoveDamage == 0)
						gBattleMoveDamage = 1;
					++effect;
				}
				break;
			case ABILITY_SOLAR_POWER:
				if (IsBattlerWeatherAffected(battler, WEATHER_SUN_ANY))
				{
					gSetWordLoc = sDrySkinSunString;
					BattleScriptPushCursorAndCallback(BattleScript_DrySkinSunActivates);
					gBattleMoveDamage = gBattleMons[battler].maxHP / 8;
					if (gBattleMoveDamage == 0)
						gBattleMoveDamage = 1;
					++effect;
				}
				break;
			case ABILITY_SHED_SKIN:
				if ((gBattleMons[battler].status1 & STATUS1_ANY) && (Random() % 3) == 0)
				{
					ClearBattlerStatus(battler);
					BattleScriptPushCursorAndCallback(BattleScript_ShedSkinActivates);
					++effect;
				}
				break;
			case ABILITY_HYDRATION:
				if ((gBattleMons[battler].status1 & STATUS1_ANY) && IsBattlerWeatherAffected(battler, WEATHER_RAIN_ANY))
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
					gSetWordLoc = sHealerString;
					BattleScriptPushCursorAndCallback(BattleScript_HealerActivates);
					++effect;
				}
				break;
			case ABILITY_SPEED_BOOST:
				if (gBattleMons[battler].statStages[STAT_SPEED] < 0xC && gDisableStructs[battler].isFirstTurn != 2)
				{
					++gBattleMons[battler].statStages[STAT_SPEED];
					gBattleScripting.animArg1 = 0x11;
					gBattleScripting.animArg2 = 0;
					BattleScriptPushCursorAndCallback(BattleScript_SpeedBoostActivates);
					gBattleScripting.battler = battler;
					++effect;
				}
				break;
			case ABILITY_TRUANT:
				gDisableStructs[gBattlerAttacker].truantCounter ^= 1;
				break;
			case ABILITY_SLOW_START:
				if (gDisableStructs[battler].slowStartTimer && --gDisableStructs[battler].slowStartTimer == 0)
				{
					gSetWordLoc = sSlowStartEndString;
					BattleScriptPushCursorAndCallback(BattleScript_DisplaySwitchInMsg);
					++effect;
				}
				break;
			case ABILITY_BAD_DREAMS:
			    gSetWordLoc = sBadDreamsString;
			    BattleScriptPushCursorAndCallback(BattleScript_BadDreamsActivates);
				++effect;
				break;
			case ABILITY_HARVEST:
				if ((IsBattlerWeatherAffected(battler, WEATHER_SUN_ANY) || (Random() % 2) == 0) && !gBattleMons[battler].item
				    && !gBattleStruct->changedItems[battler] && ItemId_GetPocket(GetUsedHeldItem(battler)) == POCKET_BERRY_POUCH)
				{
					gLastUsedItem = GetUsedHeldItem(battler);
					gSetWordLoc = sHarvestString;
					BattleScriptPushCursorAndCallback(BattleScript_HarvestActivates);
					effect++;
				}
				break;
			case ABILITY_MOODY:
				if (gDisableStructs[battler].isFirstTurn != 2)
				{
					u32 validToRaise = 0, validToLower = 0;
					
					for (i = STAT_ATK; i < NUM_STATS; i++)
					{
						if (gBattleMons[battler].statStages[i] > 0)
							validToLower |= gBitTable[i];
						
						if (gBattleMons[battler].statStages[i] < 0xC)
							validToRaise |= gBitTable[i];
					}
					
					if (validToRaise || validToLower)
					{
						gBattleCommunication[MOVE_EFFECT_BYTE] = gBattleScripting.statChanger = 0;
						
						if (validToLower)
						{
							do
							{
								i = (Random() % NUM_STATS) + STAT_ATK;
							} while (!(validToLower & gBitTable[i]));
							SET_STATCHANGER(i, 1, TRUE);
							gBattleCommunication[MOVE_EFFECT_BYTE] = gBattleScripting.statChanger; // save it in move effect byte
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
						effect++;
					}
				}
				break;
			case ABILITY_ZEN_MODE:
			    {
					u16 newSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_HP);
					
					if (newSpecies)
					{
						DoBattleFormChange(battler, newSpecies, TRUE, TRUE);
						gSetWordLoc = sZenModeString;
						BattleScriptPushCursorAndCallback(BattleScript_ZenModeActivates);
						effect++;
					}
				}
				break;
		}
	    }
	    break;
        case ABILITYEFFECT_MOVES_BLOCK: // 2
	    switch (gLastUsedAbility)
	    {
		    case ABILITY_SOUNDPROOF:
			    if (gBattleMoves[moveArg].flags & FLAG_SOUND)
				    effect = 1;
			    break;
		    case ABILITY_TELEPATHY:
			    if (IsBattlerAlive(BATTLE_PARTNER(battler)) && gBattlerAttacker == BATTLE_PARTNER(battler) && !IS_MOVE_STATUS(moveArg))
				    effect = 1;
			    break;
			case ABILITY_BULLETPROOF:
			    if (gBattleMoves[moveArg].flags & FLAG_BALLISTIC)
					effect = 1;
				break;
			case ABILITY_QUEENLY_MAJESTY:
			case ABILITY_DAZZLING:
			case ABILITY_ARMOR_TAIL:
			    if (GetChosenMovePriority(gBattlerAttacker) > 0 && GetBattlerSide(gBattlerAttacker) != GetBattlerSide(battler))
				{
					gSetWordLoc = sDazzlingString;
					effect = 2;
				}
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
        case ABILITYEFFECT_ABSORBING: // 3
            if (moveArg)
            {
		u8 StatId;    
		    
                switch (gLastUsedAbility)
                {
                case ABILITY_VOLT_ABSORB:
                    if (moveType == TYPE_ELECTRIC && gBattleMoves[moveArg].power != 0)
                    {
                        if (gProtectStructs[gBattlerAttacker].notFirstStrike)
                            gBattlescriptCurrInstr = BattleScript_MoveHPDrain;
                        else
                            gBattlescriptCurrInstr = BattleScript_MoveHPDrain_PPLoss;
                        effect = 1;
                    }
                    break;
                case ABILITY_WATER_ABSORB:
		case ABILITY_DRY_SKIN:
                    if (moveType == TYPE_WATER && gBattleMoves[moveArg].power != 0)
                    {
                        if (gProtectStructs[gBattlerAttacker].notFirstStrike)
                            gBattlescriptCurrInstr = BattleScript_MoveHPDrain;
                        else
                            gBattlescriptCurrInstr = BattleScript_MoveHPDrain_PPLoss;
                        effect = 1;
                    }
                    break;
		case ABILITY_MOTOR_DRIVE:
		   if (moveType == TYPE_ELECTRIC && gBattleMoves[moveArg].power != 0) 
		   {
			   StatId = STAT_SPEED;
			   effect = 3;
		   }
		    break;
		case ABILITY_SAP_SIPPER:
		    if (moveType == TYPE_GRASS && gBattleMoves[moveArg].power != 0)
			{
				StatId = STAT_ATK;
				effect = 3;
			}
			break;
                case ABILITY_FLASH_FIRE:
                    if (moveType == TYPE_FIRE && !(gBattleMons[battler].status1 & STATUS1_FREEZE))
                    {
                        if (!(gBattleResources->flags->flags[battler] & RESOURCE_FLAG_FLASH_FIRE))
                        {
                            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                            if (gProtectStructs[gBattlerAttacker].notFirstStrike)
                                gBattlescriptCurrInstr = BattleScript_FlashFireBoost;
                            else
                                gBattlescriptCurrInstr = BattleScript_FlashFireBoost_PPLoss;
                            gBattleResources->flags->flags[battler] |= RESOURCE_FLAG_FLASH_FIRE;
                            effect = 2;
                        }
                        else
                        {
                            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                            if (gProtectStructs[gBattlerAttacker].notFirstStrike)
                                gBattlescriptCurrInstr = BattleScript_FlashFireBoost;
                            else
                                gBattlescriptCurrInstr = BattleScript_FlashFireBoost_PPLoss;
                            effect = 2;
                        }
                    }
                    break;
                }
                if (effect == 1)
                {
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
	        else if (effect == 3)
		{
			if (BattlerStatCanRaise(battler, StatId))
			{
				PREPARE_STAT_BUFFER(gBattleTextBuff1, StatId);
				SET_STATCHANGER(StatId, 1, FALSE);
				gBattleScripting.animArg1 = 0xE + StatId;
				gBattleScripting.animArg2 = 0;
			        gSetWordLoc = sMoveStatUpString;
				
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
        case ABILITYEFFECT_MOVE_END: // Think contact abilities.
		    if (ReceiveSheerForceBoost(gBattlerAttacker, moveArg) && gLastUsedAbility != ABILITY_ILLUSION)
				break;
			
            switch (gLastUsedAbility)
	    {
		    case ABILITY_COLOR_CHANGE:
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && moveArg != MOVE_STRUGGLE && gBattleMoves[moveArg].power != 0
				&& TARGET_TURN_DAMAGED && !IS_BATTLER_OF_TYPE(battler, moveType) && IsBattlerAlive(battler))
			    {
				    SET_BATTLER_TYPE(battler, moveType);
				    PREPARE_TYPE_BUFFER(gBattleTextBuff1, moveType);
				    BattleScriptPushCursor();
				    gBattlescriptCurrInstr = BattleScript_ColorChangeActivates;
				    ++effect;
			    }
			    break;
		    case ABILITY_ROUGH_SKIN:
			case ABILITY_IRON_BARBS:
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && IsBattlerAlive(gBattlerAttacker) && TARGET_TURN_DAMAGED
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
		    case ABILITY_EFFECT_SPORE:
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && IsBattlerAlive(gBattlerAttacker) && TARGET_TURN_DAMAGED
				&& !gProtectStructs[gBattlerAttacker].confusionSelfDmg && IsMoveMakingContact(gBattlerAttacker, moveArg) && (Random() % 10) == 0
				&& GetBattlerAbility(gBattlerAttacker) != ABILITY_OVERCOAT)
			    {
				    do
					    gBattleCommunication[MOVE_EFFECT_BYTE] = Random() & 3;
				    while (gBattleCommunication[MOVE_EFFECT_BYTE] == 0);

				    switch (gBattleCommunication[MOVE_EFFECT_BYTE])
				    {
					    case MOVE_EFFECT_SLEEP:
						    if (CanBePutToSleep(gBattlerAttacker, TRUE))
							    ++effect;
						    break;
					    case MOVE_EFFECT_POISON:
						    if (CanBePoisoned(gBattlerAttacker, battler, TRUE))
							    ++effect;
						    break;
					    case MOVE_EFFECT_BURN:
						    gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_PARALYSIS;
						    if (CanBeParalyzed(gBattlerAttacker, TRUE))
							    ++effect;
						    break;
				    }
				    if (effect)
				    {
					    gBattleCommunication[MOVE_EFFECT_BYTE] |= MOVE_EFFECT_AFFECTS_USER;
					    BattleScriptPushCursor();
					    gBattlescriptCurrInstr = BattleScript_ApplySecondaryEffect;
					    gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
				    }
			    }
			    break;
		    case ABILITY_POISON_POINT:
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && IsBattlerAlive(gBattlerAttacker) && TARGET_TURN_DAMAGED
				&& !gProtectStructs[gBattlerAttacker].confusionSelfDmg && IsMoveMakingContact(gBattlerAttacker, moveArg) && (Random() % 3) == 0
				&& CanBePoisoned(gBattlerAttacker, battler, TRUE))
			    {
				    gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_POISON;
				    BattleScriptPushCursor();
				    gBattlescriptCurrInstr = BattleScript_ApplySecondaryEffect;
				    gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
				    ++effect;
			    }
			    break;
		    case ABILITY_STATIC:
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && IsBattlerAlive(gBattlerAttacker) && TARGET_TURN_DAMAGED
				&& !gProtectStructs[gBattlerAttacker].confusionSelfDmg && IsMoveMakingContact(gBattlerAttacker, moveArg) && (Random() % 3) == 0
				&& CanBeParalyzed(gBattlerAttacker, TRUE))
			    {
				    gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_PARALYSIS;
				    BattleScriptPushCursor();
				    gBattlescriptCurrInstr = BattleScript_ApplySecondaryEffect;
				    gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
				    ++effect;
			    }
			    break;
		    case ABILITY_FLAME_BODY:
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && IsBattlerAlive(gBattlerAttacker) && TARGET_TURN_DAMAGED
				&& !gProtectStructs[gBattlerAttacker].confusionSelfDmg && IsMoveMakingContact(gBattlerAttacker, moveArg) && (Random() % 3) == 0
				&& CanBeBurned(gBattlerAttacker, TRUE))
			    {
				    gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_BURN;
				    BattleScriptPushCursor();
				    gBattlescriptCurrInstr = BattleScript_ApplySecondaryEffect;
				    gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
				    ++effect;
			    }
			    break;
		    case ABILITY_CUTE_CHARM:
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && IsBattlerAlive(gBattlerAttacker) && TARGET_TURN_DAMAGED
				&& !gProtectStructs[gBattlerAttacker].confusionSelfDmg && IsMoveMakingContact(gBattlerAttacker, moveArg)
				&& IsBattlerAlive(gBattlerTarget) && GetBattlerAbility(gBattlerAttacker) != ABILITY_OBLIVIOUS
				&& !(gBattleMons[gBattlerAttacker].status2 & STATUS2_INFATUATION) && (Random() % 3) == 0
				&& GetGenderFromSpeciesAndPersonality(speciesAtk, pidAtk) != GetGenderFromSpeciesAndPersonality(speciesDef, pidDef)
				&& GetGenderFromSpeciesAndPersonality(speciesAtk, pidAtk) != MON_GENDERLESS
				&& GetGenderFromSpeciesAndPersonality(speciesDef, pidDef) != MON_GENDERLESS)
			    {
				    gBattleMons[gBattlerAttacker].status2 |= STATUS2_INFATUATED_WITH(gBattlerTarget);
				    BattleScriptPushCursor();
				    gBattlescriptCurrInstr = BattleScript_CuteCharmActivates;
				    ++effect;
			    }
			    break;
		    case ABILITY_ANGER_POINT:
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && TARGET_TURN_DAMAGED && gIsCriticalHit
				&& IsBattlerAlive(gBattlerTarget) && !SubsBlockMove(gBattlerAttacker, gBattlerTarget, moveArg)
				&& gBattleMons[gBattlerTarget].statStages[STAT_ATK] < 0xC)
			    {
				    gBattleMons[gBattlerTarget].statStages[STAT_ATK] = 0xC;
				    gSetWordLoc = sAngerPointString;
				    BattleScriptPushCursor();
				    gBattlescriptCurrInstr = BattleScript_AngerPointActivation;
				    ++effect;
			    }
			    break;
			case ABILITY_AFTERMATH:
				if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && TARGET_TURN_DAMAGED && IsBattlerAlive(gBattlerAttacker)
				&& GetBattlerAbility(gBattlerAttacker) != ABILITY_MAGIC_GUARD && IsMoveMakingContact(gBattlerAttacker, moveArg)
				&& !IsBattlerAlive(gBattlerTarget) && !AbilityBattleEffects(ABILITYEFFECT_CHECK_ON_FIELD, 0, ABILITY_DAMP, 0, 0))
				{
					gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 4;
				    if (gBattleMoveDamage == 0)
					    gBattleMoveDamage = 1;
				    BattleScriptPushCursor();
				    gBattlescriptCurrInstr = BattleScript_RoughSkinActivates;
				    ++effect;
				}
				break;
		    case ABILITY_PICKPOCKET:
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && !gProtectStructs[gBattlerAttacker].confusionSelfDmg && TARGET_TURN_DAMAGED
				&& IsMoveMakingContact(gBattlerAttacker, moveArg) && !SubsBlockMove(gBattlerAttacker, gBattlerTarget, moveArg)
				&& !gBattleMons[gBattlerTarget].item && IsBattlerAlive(gBattlerTarget) && GetBattlerAbility(gBattlerAttacker) != ABILITY_STICKY_HOLD
				&& gBattleMons[gBattlerAttacker].item)
			    {
				    gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_STEAL_ITEM;
				    BattleScriptPushCursor();
				    gBattlescriptCurrInstr = BattleScript_PickpocketActivation;
				    ++effect;
			    }
			    break;
		    case ABILITY_CURSED_BODY:
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && IsBattlerAlive(gBattlerAttacker) && TARGET_TURN_DAMAGED && (Random() % 3) == 0
				&& !gProtectStructs[gBattlerAttacker].confusionSelfDmg && !(gBattleMons[gBattlerTarget].status2 & STATUS2_SUBSTITUTE) && moveArg != MOVE_STRUGGLE
			        && !gDisableStructs[gBattlerAttacker].disabledMove && gBattleMons[gBattlerAttacker].pp[gCurrMovePos]
					&& !AbilityBattleEffects(ABILITYEFFECT_CHECK_BATTLER_SIDE, gBattlerTarget, ABILITY_AROMA_VEIL, 0, 0))
			    {
				    gDisableStructs[gBattlerAttacker].disabledMove = moveArg;
				    gDisableStructs[gBattlerAttacker].disableTimer = 4;
				    gSetWordLoc = sCursedBodyString;
				    BattleScriptPushCursor();
				    gBattlescriptCurrInstr = BattleScript_CursedBodyActivation;
				    ++effect;
			    }
			    break;
		    case ABILITY_WEAK_ARMOR:
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && IsBattlerAlive(gBattlerTarget) && IS_MOVE_PHYSICAL(moveArg)
				&& (gBattleMons[gBattlerTarget].statStages[STAT_SPEED] < 0xC || gBattleMons[gBattlerTarget].statStages[STAT_DEF] > 0) 
				&& TARGET_TURN_DAMAGED)
			    {
				    BattleScriptPushCursor();
				    gBattlescriptCurrInstr = BattleScript_WeakArmorActivation;
				    ++effect;
			    }
			    break;
		    case ABILITY_ILLUSION:
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && TARGET_TURN_DAMAGED && TryRemoveIllusion(gBattlerTarget))
				    ++effect;
			    break;
			case ABILITY_MUMMY:
			case ABILITY_LINGERING_AROMA:
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && IsBattlerAlive(gBattlerAttacker) && TARGET_TURN_DAMAGED && IsMoveMakingContact(gBattlerAttacker, moveArg))
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
						    gLastUsedAbility = gBattleMons[gBattlerAttacker].ability;
							gBattleMons[gBattlerAttacker].ability = gBattleMons[gBattlerTarget].ability;
							ResetVarsForAbilityChange(gBattlerAttacker);
							gSetWordLoc = sMummyString;
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_MummyActivates;
							RecordAbilityBattle(battler, gBattleMons[gBattlerTarget].ability);
							effect++;
							return effect;
					}
				}
				break;
			case ABILITY_JUSTIFIED:
				if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && TARGET_TURN_DAMAGED && IsBattlerAlive(gBattlerTarget) && gBattleMons[gBattlerTarget].statStages[STAT_ATK] < 0xC
				&& moveType == TYPE_DARK && !SubsBlockMove(gBattlerAttacker, gBattlerTarget, moveArg))
				{
					gEffectBattler = gBattlerTarget;
					SET_STATCHANGER(STAT_ATK, 1, FALSE);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaiseRet;
					effect++;
				}
				break;
			case ABILITY_RATTLED:
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && TARGET_TURN_DAMAGED && IsBattlerAlive(gBattlerTarget) && gBattleMons[gBattlerTarget].statStages[STAT_SPEED] < 0xC
				&& (moveType == TYPE_BUG || moveType == TYPE_DARK || moveType == TYPE_GHOST))
				{
					gEffectBattler = gBattlerTarget;
					SET_STATCHANGER(STAT_SPEED, 1, FALSE);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaiseRet;
					effect++;
				}
				break;
			case ABILITY_GOOEY:
			case ABILITY_TANGLING_HAIR:
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && TARGET_TURN_DAMAGED && IsBattlerAlive(gBattlerAttacker) && IsMoveMakingContact(gBattlerAttacker, moveArg)
				&& gBattleMons[gBattlerAttacker].statStages[STAT_SPEED] > 0)
				{
					SET_STATCHANGER(STAT_SPEED, 1, TRUE);
					gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_SPD_MINUS_1;
					PREPARE_ABILITY_BUFFER(gBattleTextBuff1, gLastUsedAbility);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_GooeyActivates;
					gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
					effect++;
				}
				break;
			case ABILITY_STAMINA:
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && TARGET_TURN_DAMAGED && IsBattlerAlive(gBattlerTarget) && gBattleMons[gBattlerTarget].statStages[STAT_DEF] < 0xC)
				{
					gEffectBattler = gBattlerTarget;
					SET_STATCHANGER(STAT_DEF, 1, FALSE);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaiseRet;
					effect++;
				}
				break;
			case ABILITY_WATER_COMPACTION:
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && TARGET_TURN_DAMAGED && IsBattlerAlive(gBattlerTarget) && gBattleMons[gBattlerTarget].statStages[STAT_DEF] < 0xC
				&& moveType == TYPE_WATER)
				{
					gEffectBattler = gBattlerTarget;
					SET_STATCHANGER(STAT_DEF, 2, FALSE);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaiseRet;
					effect++;
				}
				break;
			case ABILITY_BERSERK:
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && TARGET_TURN_DAMAGED && IsBattlerAlive(gBattlerTarget) && gBattleStruct->hpBefore[gBattlerTarget] >= gBattleMons[gBattlerTarget].maxHP / 2
				&& gBattleMons[gBattlerTarget].hp < gBattleMons[gBattlerTarget].maxHP / 2 && (gMultiHitCounter == 0 || gMultiHitCounter == 1) && !ReceiveSheerForceBoost(gBattlerAttacker, gCurrentMove)
				&& gBattleMons[gBattlerTarget].statStages[STAT_SPATK] < 0xC)
				{
					gEffectBattler = gBattlerTarget;
					SET_STATCHANGER(STAT_SPATK, 1, FALSE);
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_TargetAbilityStatRaiseRet;
					effect++;
				}
				break;
			case ABILITY_INNARDS_OUT:
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && !IsBattlerAlive(gBattlerTarget) && IsBattlerAlive(gBattlerAttacker))
				{
					gBattleMoveDamage = gSpecialStatuses[gBattlerTarget].dmg;
					BattleScriptPushCursor();
				    gBattlescriptCurrInstr = BattleScript_RoughSkinActivates;
				    ++effect;
				}
				break;
	    }
	    break;
	case ABILITYEFFECT_MOVE_END_ATTACKER:
	    switch (gLastUsedAbility)
	    {
		    case ABILITY_POISON_TOUCH:
			if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && IsBattlerAlive(gBattlerTarget) && !gProtectStructs[gBattlerAttacker].confusionSelfDmg
			   && CanBePoisoned(gBattlerTarget, gBattlerAttacker, TRUE) && IsMoveMakingContact(gBattlerAttacker, moveArg) && TARGET_TURN_DAMAGED && (Random() % 3) == 0)
			{
				gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_POISON;
				PREPARE_ABILITY_BUFFER(gBattleTextBuff1, gLastUsedAbility);
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_PoisonTouchActivation;
				gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
				++effect;
			}
			break;
	    }
	    break;
        case ABILITYEFFECT_IMMUNITY: // 5
            for (battler = 0; battler < gBattlersCount; ++battler)
            {
                switch (GetBattlerAbility(battler))
                {
                case ABILITY_IMMUNITY:
                    if (gBattleMons[battler].status1 & (STATUS1_POISON | STATUS1_TOXIC_POISON | STATUS1_TOXIC_COUNTER))
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_PoisonJpn);
                        effect = 1;
                    }
                    break;
                case ABILITY_OWN_TEMPO:
                    if (gBattleMons[battler].status2 & STATUS2_CONFUSION)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_ConfusionJpn);
                        effect = 2;
                    }
                    break;
                case ABILITY_LIMBER:
                    if (gBattleMons[battler].status1 & STATUS1_PARALYSIS)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_ParalysisJpn);
                        effect = 1;
                    }
                    break;
                case ABILITY_INSOMNIA:
                case ABILITY_VITAL_SPIRIT:
                    if (gBattleMons[battler].status1 & STATUS1_SLEEP)
                    {
                        gBattleMons[battler].status2 &= ~(STATUS2_NIGHTMARE);
                        StringCopy(gBattleTextBuff1, gStatusConditionString_SleepJpn);
                        effect = 1;
                    }
                    break;
                case ABILITY_WATER_VEIL:
				case ABILITY_WATER_BUBBLE:
                    if (gBattleMons[battler].status1 & STATUS1_BURN)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_BurnJpn);
                        effect = 1;
                    }
                    break;
                case ABILITY_MAGMA_ARMOR:
                    if (gBattleMons[battler].status1 & STATUS1_FREEZE)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_IceJpn);
                        effect = 1;
                    }
                    break;
                case ABILITY_OBLIVIOUS:
                    if (gBattleMons[battler].status2 & STATUS2_INFATUATION)
                    {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_LoveJpn);
                        effect = 3;
                    }
                    break;
                }
                if (effect)
                {
                    switch (effect)
                    {
                    case 1: // status cleared
                        gBattleMons[battler].status1 = 0;
                        break;
                    case 2: // get rid of confusion
                        gBattleMons[battler].status2 &= ~(STATUS2_CONFUSION);
                        break;
                    case 3: // get rid of infatuation
                        gBattleMons[battler].status2 &= ~(STATUS2_INFATUATION);
                        break;
                    }

                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_AbilityCuredStatus;
                    gBattleScripting.battler = battler;
                    gActiveBattler = battler;
                    BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
                    MarkBattlerForControllerExec(gActiveBattler);
                    return effect;
                }
            }
            break;
        case ABILITYEFFECT_FORECAST: // 6
            for (battler = 0; battler < gBattlersCount; ++battler)
            {
				if (GetBattlerAbility(battler) == ABILITY_FORECAST || GetBattlerAbility(battler) == ABILITY_FLOWER_GIFT)
				{
					u16 newSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_WEATHER);
					
					if (newSpecies)
					{
						DoBattleFormChange(battler, newSpecies, TRUE, TRUE);
						gLastUsedAbility = GetBattlerAbility(battler);
						BattleScriptPushCursorAndCallback(BattleScript_CastformChange);
						gBattleScripting.battler = battler;
						++effect;
					}
				}
            }
            break;
        case ABILITYEFFECT_SYNCHRONIZE: // 7
            if (gLastUsedAbility == ABILITY_SYNCHRONIZE && (gHitMarker & HITMARKER_SYNCHRONISE_EFFECT))
            {
                gHitMarker &= ~(HITMARKER_SYNCHRONISE_EFFECT);
                gBattleStruct->synchronizeMoveEffect &= ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
                if (gBattleStruct->synchronizeMoveEffect == MOVE_EFFECT_TOXIC)
                    gBattleStruct->synchronizeMoveEffect = MOVE_EFFECT_POISON;
                gBattleCommunication[MOVE_EFFECT_BYTE] = gBattleStruct->synchronizeMoveEffect + MOVE_EFFECT_AFFECTS_USER;
                gBattleScripting.battler = gBattlerTarget;
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_SynchronizeActivates;
                gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
                ++effect;
            }
            break;
        case ABILITYEFFECT_ATK_SYNCHRONIZE: // 8
            if (gLastUsedAbility == ABILITY_SYNCHRONIZE && (gHitMarker & HITMARKER_SYNCHRONISE_EFFECT))
            {
                gHitMarker &= ~(HITMARKER_SYNCHRONISE_EFFECT);
                gBattleStruct->synchronizeMoveEffect &= ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
                if (gBattleStruct->synchronizeMoveEffect == MOVE_EFFECT_TOXIC)
                    gBattleStruct->synchronizeMoveEffect = MOVE_EFFECT_POISON;
                gBattleCommunication[MOVE_EFFECT_BYTE] = gBattleStruct->synchronizeMoveEffect;
                gBattleScripting.battler = gBattlerAttacker;
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_SynchronizeActivates;
                gHitMarker |= HITMARKER_IGNORE_SAFEGUARD;
                ++effect;
            }
            break;
        case ABILITYEFFECT_INTIMIDATE1: // 9		
            for (i = 0; i < gBattlersCount; ++i)
            {
		if (gStatuses3[i] & STATUS3_INTIMIDATE_POKES)
		{
			if (GetBattlerAbility(i) == ABILITY_INTIMIDATE)
			{
				BattleScriptPushCursorAndCallback(BattleScript_IntimidateActivatesEnd3);
				++effect;
				break;
			}
			else if (GetBattlerAbility(i) == ABILITY_ANTICIPATION)
			{
				BattleScriptPushCursorAndCallback(BattleScript_Anticipation);
				++effect;
				break;
			}
			else if (GetBattlerAbility(i) == ABILITY_DOWNLOAD)
			{
				BattleScriptPushCursorAndCallback(BattleScript_Download);
				++effect;
				break;
			}
			else if (GetBattlerAbility(i) == ABILITY_FOREWARN)
			{
				BattleScriptPushCursorAndCallback(BattleScript_Forewarn);
				++effect;
				break;
			}
			else if (GetBattlerAbility(i) == ABILITY_FRISK)
			{
				BattleScriptPushCursorAndCallback(BattleScript_Frisk);
				++effect;
				break;
			}
		}
	    }
	    if (effect)
	    {
		    gLastUsedAbility = GetBattlerAbility(i);
		    gStatuses3[i] &= ~(STATUS3_INTIMIDATE_POKES);
		    gBattlerAttacker = i;
		    gBattleStruct->intimidateBattler = i;
	    }
            break;
        case ABILITYEFFECT_TRACE: // 11
            for (i = 0; i < gBattlersCount; ++i)
            {
                if (GetBattlerAbility(i) == ABILITY_TRACE && (gStatuses3[i] & STATUS3_TRACE))
                {
                    u8 target2;
                    
                    side = (BATTLE_OPPOSITE(GetBattlerPosition(i))) & BIT_SIDE; // side of the opposing pokemon
                    target1 = GetBattlerAtPosition(side);
                    target2 = GetBattlerAtPosition(side + BIT_FLANK);
                    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                    {
                        if (IsBattlerAlive(target1) && GetBattlerAbility(target1) != ABILITY_FLOWER_GIFT
                         && IsBattlerAlive(target2) && GetBattlerAbility(target2) != ABILITY_FLOWER_GIFT)
                        {
                            gActiveBattler = GetBattlerAtPosition(((Random() & 1) * 2) | side);
                            gLastUsedAbility = gBattleMons[i].ability = gBattleMons[gActiveBattler].ability;
                            ++effect;
                        }
                        else if (GetBattlerAbility(target1) != ABILITY_FLOWER_GIFT && IsBattlerAlive(target1))
                        {
                            gActiveBattler = target1;
                            gLastUsedAbility = gBattleMons[i].ability = gBattleMons[gActiveBattler].ability;
                            ++effect;
                        }
                        else if (GetBattlerAbility(target2) != ABILITY_FLOWER_GIFT && IsBattlerAlive(target2))
                        {
                            gActiveBattler = target2;
                            gLastUsedAbility = gBattleMons[i].ability = gBattleMons[gActiveBattler].ability;
                            ++effect;
                        }
                    }
                    else
                    {
                        gActiveBattler = target1;
                        if (GetBattlerAbility(target1) != ABILITY_FLOWER_GIFT && IsBattlerAlive(target1))
                        {
                            gLastUsedAbility = gBattleMons[i].ability = gBattleMons[target1].ability;
                            ++effect;
                        }
                    }
                    if (effect)
                    {
                        BattleScriptPushCursorAndCallback(BattleScript_TraceActivates);
                        gStatuses3[i] &= ~(STATUS3_TRACE);
                        gBattleScripting.battler = i;
                        PREPARE_MON_NICK_WITH_PREFIX_BUFFER(gBattleTextBuff1, gActiveBattler, gBattlerPartyIndexes[gActiveBattler])
                        PREPARE_ABILITY_BUFFER(gBattleTextBuff2, gLastUsedAbility)
                        break;
                    }
                }
            }
            break;
        case ABILITYEFFECT_INTIMIDATE2: // 10
            for (i = 0; i < gBattlersCount; ++i)
            {
                if (GetBattlerAbility(i) == ABILITY_INTIMIDATE && (gStatuses3[i] & STATUS3_INTIMIDATE_POKES))
                {
                    gLastUsedAbility = ABILITY_INTIMIDATE;
                    gStatuses3[i] &= ~(STATUS3_INTIMIDATE_POKES);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_IntimidateActivates;
                    gBattleStruct->intimidateBattler = i;
                    ++effect;
                    break;
                }
            }
            break;
        case ABILITYEFFECT_CHECK_OTHER_SIDE: // 12
            side = GetBattlerSide(battler);
            for (i = 0; i < gBattlersCount; ++i)
            {
                if (GetBattlerSide(i) != side && GetBattlerAbility(i) == ability)
                {
                    gLastUsedAbility = ability;
                    effect = i + 1;
                }
            }
            break;
        case ABILITYEFFECT_CHECK_BATTLER_SIDE: // 13
            side = GetBattlerSide(battler);
            for (i = 0; i < gBattlersCount; ++i)
            {
                if (GetBattlerSide(i) == side && GetBattlerAbility(i) == ability)
                {
                    gLastUsedAbility = ability;
                    effect = i + 1;
                }
            }
            break;
        case ABILITYEFFECT_FIELD_SPORT: // 14
            switch (gLastUsedAbility)
            {
            case 0xFD:
                for (i = 0; i < gBattlersCount; ++i)
                    if (gStatuses3[i] & STATUS3_MUDSPORT)
                        effect = i + 1;
                break;
            case 0xFE:
                for (i = 0; i < gBattlersCount; ++i)
                    if (gStatuses3[i] & STATUS3_WATERSPORT)
                        effect = i + 1;
                break;
            default:
                for (i = 0; i < gBattlersCount; ++i)
                {
                    if (GetBattlerAbility(i) == ability)
                    {
                        gLastUsedAbility = ability;
                        effect = i + 1;
                    }
                }
                break;
            }
            break;
        case ABILITYEFFECT_CHECK_ON_FIELD: // 19
            for (i = 0; i < gBattlersCount; ++i)
            {
                if (GetBattlerAbility(i) == ability && IsBattlerAlive(i))
                {
                    gLastUsedAbility = ability;
                    effect = i + 1;
                }
            }
            break;
        case ABILITYEFFECT_CHECK_FIELD_EXCEPT_BATTLER: // 15
            side = GetBattlerSide(battler);
            for (i = 0; i < gBattlersCount; ++i)
            {
                if (GetBattlerSide(i) != side && GetBattlerAbility(i) == ability)
                {
                    gLastUsedAbility = ability;
                    effect = i + 1;
                    break;
                }
            }
            if (!effect)
            {
                for (i = 0; i < gBattlersCount; ++i)
                {
                    if (GetBattlerAbility(i) == ability && GetBattlerSide(i) == side && i != battler)
                    {
                        gLastUsedAbility = ability;
                        effect = i + 1;
                    }
                }
            }
            break;
        case ABILITYEFFECT_COUNT_OTHER_SIDE: // 16
            side = GetBattlerSide(battler);
            for (i = 0; i < gBattlersCount; ++i)
            {
                if (GetBattlerSide(i) != side && GetBattlerAbility(i) == ability)
                {
                    gLastUsedAbility = ability;
                    ++effect;
                }
            }
            break;
        case ABILITYEFFECT_COUNT_BATTLER_SIDE: // 17
            side = GetBattlerSide(battler);
            for (i = 0; i < gBattlersCount; ++i)
            {
                if (GetBattlerSide(i) == side && GetBattlerAbility(i) == ability)
                {
                    gLastUsedAbility = ability;
                    ++effect;
                }
            }
            break;
        case ABILITYEFFECT_COUNT_ON_FIELD: // 18
            for (i = 0; i < gBattlersCount; ++i)
            {
                if (GetBattlerAbility(i) == ability && i != battler)
                {
                    gLastUsedAbility = ability;
                    ++effect;
                }
            }
            break;
        }
        if (effect && caseID < ABILITYEFFECT_CHECK_OTHER_SIDE && gLastUsedAbility != 0xFFFF)
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

#define IS_ITEM_BERRY(itemId)((itemId >= FIRST_BERRY_INDEX && itemId <= LAST_BERRY_INDEX))

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
	gBattleMoveDamage = gBattleMons[battlerId].maxHP / 2;
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

    if (CheckPinchBerryActivate(battlerId, gLastUsedItem) && !moveTurn && BattlerStatCanRaise(battlerId, statId))
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
	
    if (IsUnnerveOnOpposingField(battlerId) && IS_ITEM_BERRY(gLastUsedItem))
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
                    gBattleMoveDamage = gBattleMons[battlerId].maxHP / 16;
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
            case HOLD_EFFECT_CONFUSE_SPICY:
	        effect = ConfusionBerries(battlerId, FLAVOR_SPICY, moveTurn);
                break;
            case HOLD_EFFECT_CONFUSE_DRY:
	        effect = ConfusionBerries(battlerId, FLAVOR_DRY, moveTurn);
                break;
            case HOLD_EFFECT_CONFUSE_SWEET:
	        effect = ConfusionBerries(battlerId, FLAVOR_SWEET, moveTurn);
                break;
            case HOLD_EFFECT_CONFUSE_BITTER:
	        effect = ConfusionBerries(battlerId, FLAVOR_BITTER, moveTurn);
                break;
            case HOLD_EFFECT_CONFUSE_SOUR:
	        effect = ConfusionBerries(battlerId, FLAVOR_SOUR, moveTurn);
                break;
            case HOLD_EFFECT_ATTACK_UP:
	        effect = StatRaiseBerries(battlerId, STAT_ATK, moveTurn);
                break;
            case HOLD_EFFECT_DEFENSE_UP:
	        effect = StatRaiseBerries(battlerId, STAT_DEF, moveTurn);
                break;
            case HOLD_EFFECT_SPEED_UP:
	        effect = StatRaiseBerries(battlerId, STAT_SPEED, moveTurn);
                break;
            case HOLD_EFFECT_SP_ATTACK_UP:
	        effect = StatRaiseBerries(battlerId, STAT_SPATK, moveTurn);
                break;
            case HOLD_EFFECT_SP_DEFENSE_UP:
	        effect = StatRaiseBerries(battlerId, STAT_SPDEF, moveTurn);
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
                    for (i = 0; i < 5 && !BattlerStatCanRaise(battlerId, STAT_ATK + i); ++i);
                    if (i != 5)
                    {
                        do
                            i = Random() % 5;
                        while (!BattlerStatCanRaise(battlerId, STAT_ATK + i));
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
            case HOLD_EFFECT_CURE_ATTRACT:
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
            case HOLD_EFFECT_CURE_ATTRACT:
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
                 && TARGET_TURN_DAMAGED
                 && (Random() % 100) < battlerHoldEffectParam
                 && gBattleMoves[gCurrentMove].flags & FLAG_KINGSROCK_AFFECTED
                 && IsBattlerAlive(gBattlerTarget)
		 && GetBattlerAbility(battlerId) != ABILITY_STENCH)
                {
                    gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_FLINCH;
                    BattleScriptPushCursor();
                    SetMoveEffect(0, 0);
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

u8 GetMoveTarget(u16 move, u8 setTarget)
{
    u8 targetBattler = 0;
    u8 moveTarget;
    u8 side;

    if (setTarget)
        moveTarget = setTarget - 1;
    else
        moveTarget = gBattleMoves[move].target;
    switch (moveTarget)
    {
    case MOVE_TARGET_SELECTED:
        side = BATTLE_OPPOSITE(GetBattlerSide(gBattlerAttacker));
        if (gSideTimers[side].followmeTimer && IsBattlerAlive(gSideTimers[side].followmeTarget))
            targetBattler = gSideTimers[side].followmeTarget;
        else
        {
            side = GetBattlerSide(gBattlerAttacker);
            do
            {
                targetBattler = Random() % gBattlersCount;
            } while (targetBattler == gBattlerAttacker || side == GetBattlerSide(targetBattler) || gAbsentBattlerFlags & gBitTable[targetBattler]);
            if (gBattleMoves[move].type == TYPE_ELECTRIC && AbilityBattleEffects(ABILITYEFFECT_COUNT_OTHER_SIDE, gBattlerAttacker, ABILITY_LIGHTNING_ROD, 0, 0)
             && GetBattlerAbility(targetBattler) != ABILITY_LIGHTNING_ROD)
            {
                targetBattler = BATTLE_PARTNER(targetBattler);
                RecordAbilityBattle(targetBattler, gBattleMons[targetBattler].ability);
                gSpecialStatuses[targetBattler].lightningRodRedirected = 1;
            }
	    else if (gBattleMoves[move].type == TYPE_WATER && AbilityBattleEffects(ABILITYEFFECT_COUNT_OTHER_SIDE, gBattlerAttacker, ABILITY_STORM_DRAIN, 0, 0)
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
        targetBattler = GetBattlerAtPosition((BATTLE_OPPOSITE(GetBattlerPosition(gBattlerAttacker) & BIT_SIDE)));
        if (gAbsentBattlerFlags & gBitTable[targetBattler])
            targetBattler = BATTLE_PARTNER(targetBattler);
        break;
    case MOVE_TARGET_RANDOM:
        side = BATTLE_OPPOSITE(GetBattlerSide(gBattlerAttacker));
        if (gSideTimers[side].followmeTimer && IsBattlerAlive(gSideTimers[side].followmeTarget))
            targetBattler = gSideTimers[side].followmeTarget;
        else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && moveTarget & MOVE_TARGET_RANDOM)
        {
            if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
            {
                if (Random() & 1)
                    targetBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
                else
                    targetBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
            }
            else
            {
                if (Random() & 1)
                    targetBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
                else
                    targetBattler = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
            }
            if (gAbsentBattlerFlags & gBitTable[targetBattler])
                targetBattler = BATTLE_PARTNER(targetBattler);
        }
        else
            targetBattler = GetBattlerAtPosition(BATTLE_OPPOSITE(GetBattlerPosition(gBattlerAttacker) & BIT_SIDE));
        break;
    case MOVE_TARGET_USER_OR_SELECTED:
    case MOVE_TARGET_USER:
        targetBattler = gBattlerAttacker;
        break;
    }
    *(gBattleStruct->moveTarget + gBattlerAttacker) = targetBattler;
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
            gBattleMoveDamage = CalculateBaseDamage(MOVE_POUND, TYPE_NORMAL, gBattlerAttacker, gBattlerTarget, TRUE, FALSE, FALSE);
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

static void SetIllusionMon(struct Pokemon *mon, u8 battler)
{
	struct Pokemon *party, *partnerMon;
	s8 i, id;
	
	gNewBattleStruct.illusion[battler].set = TRUE;
	
	if (GetMonAbility(mon) == ABILITY_ILLUSION)
	{
		if (GetBattlerSide(battler) == B_SIDE_PLAYER)
			party = gPlayerParty;
		else
			party = gEnemyParty;
		
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
				gNewBattleStruct.illusion[battler].on = TRUE;
				gNewBattleStruct.illusion[battler].broken = FALSE;
				gNewBattleStruct.illusion[battler].partyId = id;
				gNewBattleStruct.illusion[battler].mon = &party[id];
				return;
			}
		}
	}
}

struct Pokemon *GetIllusionMonPtr(u8 battler)
{
	if (gNewBattleStruct.illusion[battler].broken)
		return NULL;
	
	if (!gNewBattleStruct.illusion[battler].set)
		SetIllusionMon(GetBattlerPartyIndexPtr(battler), battler);
	
	if (!gNewBattleStruct.illusion[battler].on)
		return NULL;
	
	return gNewBattleStruct.illusion[battler].mon;
}

void ClearIllusionMon(u8 battler)
{
	memset(&gNewBattleStruct.illusion[battler], 0, sizeof(gNewBattleStruct.illusion[battler]));
}

bool8 TryRemoveIllusion(u8 battler)
{
	if (GetIllusionMonPtr(battler) != NULL)
	{
		gSetWordLoc = sIllusionOffString;
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_IllusionOff;
		return TRUE;
	}
	return FALSE;
}

bool8 ReceiveSheerForceBoost(u8 battler, u16 move)
{
	return (GetBattlerAbility(battler) == ABILITY_SHEER_FORCE && (gBattleMoves[move].flags & FLAG_SHEER_FORCE_BOOST));
}

bool8 BattlerStatCanRaise(u8 battler, u8 statId)
{
	u8 statStage = gBattleMons[battler].statStages[statId];
	bool8 hasContrary = GetBattlerAbility(battler) == ABILITY_CONTRARY;
	
	return ((statStage < 12 && !hasContrary) || (statStage > 0 && hasContrary));
}

bool8 BattlerStatCanFall(u8 battler, u8 statId)
{
	u8 statStage = gBattleMons[battler].statStages[statId];
	bool8 hasContrary = GetBattlerAbility(battler) == ABILITY_CONTRARY;
	
	return ((statStage > 0 && !hasContrary) || (statStage < 12 && hasContrary));
}

bool8 IsUnnerveOnOpposingField(u8 battler)
{
	if (ABILITY_ON_OPPOSING_FIELD(battler, ABILITY_UNNERVE) || ABILITY_ON_OPPOSING_FIELD(battler, ABILITY_AS_ONE_ICE_RIDER) || ABILITY_ON_OPPOSING_FIELD(battler, ABILITY_AS_ONE_SHADOW_RIDER))
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
	if (gBattleMons[battlerId].hp == 0 || battlerId >= gBattlersCount || gAbsentBattlerFlags & gBitTable[battlerId])
		return FALSE;
	return TRUE;
}

struct Pokemon *GetBattlerPartyIndexPtr(u8 battler)
{
	struct Pokemon *mon;
	
	if (GetBattlerSide(battler) == B_SIDE_PLAYER)
		mon = gPlayerParty;
	else
		mon = gEnemyParty;
	
	return &mon[gBattlerPartyIndexes[battler]];
}

bool8 IsBattlerWeatherAffected(u8 battlerId, u16 weatherFlags)
{
	if (WEATHER_HAS_EFFECT && gBattleWeather & weatherFlags)
	{
		return TRUE;
	}
	return FALSE;
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
		return TRUE;
	}
	return FALSE;
}

bool8 IsMoveMakingContact(u8 battler, u16 move)
{
	if (GetBattlerAbility(battler) != ABILITY_LONG_REACH && gBattleMoves[move].flags & FLAG_MAKES_CONTACT)
		return TRUE;
	return FALSE;
}

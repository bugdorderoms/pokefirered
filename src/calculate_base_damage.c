#include "global.h"
#include "gflib.h"
#include "random.h"
#include "text.h"
#include "data.h"
#include "main.h"
#include "battle.h"
#include "battle_anim.h"
#include "item.h"
#include "event_data.h"
#include "util.h"
#include "wild_encounter.h"
#include "pokedex.h"
#include "pokemon_storage_system.h"
#include "battle_gfx_sfx_util.h"
#include "battle_controllers.h"
#include "evolution_scene.h"
#include "battle_message.h"
#include "battle_util.h"
#include "battle_interface.h"
#include "battle_script_commands.h"
#include "calculate_base_damage.h"
#include "link.h"
#include "m4a.h"
#include "strings.h"
#include "overworld.h"
#include "party_menu.h"
#include "field_specials.h"
#include "constants/battle_script_commands.h"
#include "constants/items.h"
#include "constants/item_effects.h"
#include "constants/pokemon.h"
#include "constants/abilities.h"
#include "constants/moves.h"
#include "constants/songs.h"
#include "constants/item_effects.h"
#include "constants/trainers.h"
#include "constants/hold_effects.h"
#include "constants/battle_move_effects.h"

struct DamageCalc
{
	u16 move;
	u16 atkAbility;
	u16 defAbility;
	u8 effectiveness;
	u8 moveType;
	u8 atkHoldEffect;
	u8 defHoldEffect;
	u8 atkHoldEffParam;
	u8 defHoldEffParam;
	bool8 isCrit;
};

static u16 GetMoveBasePower(u8 attacker, u8 defender, struct DamageCalc *damageStruct);
static u16 CalcBaseAttackStat(u8 attacker, u8 defender, struct DamageCalc *damageStruct, bool8 confusionDmg);
static u16 CalcBaseDefenseStat(u8 attacker, u8 defender, struct DamageCalc *damageStruct, bool8 confusionDmg);

///////////////////////////////
// GLOBAL DAMAGE CALCULATION //
///////////////////////////////

static struct DamageCalc *PopulateDamageStruct(u8 attacker, u8 defender, u16 move, u8 moveType, u8 effectiveness, bool8 isCrit)
{
	struct DamageCalc *damageStruct = AllocZeroed(sizeof(struct DamageCalc));
	damageStruct->atkAbility = GetBattlerAbility(attacker);
	damageStruct->defAbility = GetBattlerAbility(defender);
	damageStruct->atkHoldEffect = GetBattlerItemHoldEffect(attacker, TRUE);
	damageStruct->defHoldEffect = GetBattlerItemHoldEffect(defender, TRUE);
	damageStruct->atkHoldEffParam = ItemId_GetHoldEffectParam(gBattleMons[attacker].item);
	damageStruct->defHoldEffParam = ItemId_GetHoldEffectParam(gBattleMons[defender].item);
	damageStruct->move = move;
	damageStruct->moveType = moveType;
	damageStruct->effectiveness = effectiveness;
	damageStruct->isCrit = isCrit;
	return damageStruct;
}

/*
s32 CalculateBaseDamage(u16 move, u8 type, u8 battlerIdAtk, u8 battlerIdDef, bool8 isCrit, bool8 randomFactor)
{
	// attacker items check
		switch (attackerHoldEffect)
		{
			case HOLD_EFFECT_CHOICE_ITEM:
			    if (attackerHoldEffectParam == STAT_ATK)
					attack = (15 * attack) / 10;
				else if (attackerHoldEffectParam == STAT_SPATK)
					spAttack = (15 * spAttack) / 10;
				break;
			case HOLD_EFFECT_TYPE_POWER:
			    if (type == attackerHoldEffectParam)
				{
					attack = (attack * 120) / 100;
					spAttack = (spAttack * 120) / 100;
				}
				break;
			case HOLD_EFFECT_SOUL_DEW:
				if (!(gBattleTypeFlags & (BATTLE_TYPE_BATTLE_TOWER)) && (attacker->species == SPECIES_LATIAS || attacker->species == SPECIES_LATIOS)
					&& (type == TYPE_PSYCHIC || type == TYPE_DRAGON))
					gBattleMovePower = (gBattleMovePower * 120) / 100;
				break;
			case HOLD_EFFECT_DEEP_SEA_TOOTH:
				if (attacker->species == SPECIES_CLAMPERL)
					spAttack *= 2;
				break;
			case HOLD_EFFECT_LIGHT_BALL:
				if (SpeciesToNationalPokedexNum(attacker->species) == NATIONAL_DEX_PIKACHU)
				{
					attack *= 2;
					spAttack *= 2;
				}
				break;
			case HOLD_EFFECT_THICK_CLUB:
				if (attacker->species == SPECIES_CUBONE || SpeciesToNationalPokedexNum(attacker->species) == NATIONAL_DEX_MAROWAK)
					attack *= 2;
				break;
			case HOLD_EFFECT_ADAMANT_ORB:
			    if (SpeciesToNationalPokedexNum(attacker->species) == NATIONAL_DEX_DIALGA && (type == TYPE_DRAGON || type == TYPE_STEEL))
					gBattleMovePower = (gBattleMovePower * 120) / 100;
				break;
			case HOLD_EFFECT_LUSTROUS_ORB:
			    if (SpeciesToNationalPokedexNum(attacker->species) == NATIONAL_DEX_PALKIA && (type == TYPE_DRAGON || type == TYPE_WATER))
					gBattleMovePower = (gBattleMovePower * 120) / 100;
				break;
			case HOLD_EFFECT_GRISEOUS_ORB:
			    if (SpeciesToNationalPokedexNum(attacker->species) == NATIONAL_DEX_GIRATINA && (type == TYPE_DRAGON || type == TYPE_GHOST))
					gBattleMovePower = (gBattleMovePower * 120) / 100;
				break;
		}
		
		// defender items check
		switch (defenderHoldEffect)
		{
			case HOLD_EFFECT_DEEP_SEA_SCALE:
				if (defender->species == SPECIES_CLAMPERL)
					spDefense *= 2;
				break;
			case HOLD_EFFECT_METAL_POWDER:
				if (defender->species == SPECIES_DITTO && !(defender->status2 & STATUS2_TRANSFORMED) && defenderHoldEffectParam == STAT_DEF)
					defense *= 2;
				break;
		}
}
*/

static s32 CalculateDamage(u8 attacker, u8 defender, struct DamageCalc *damageStruct, u16 basePower, bool8 randomFactor, bool8 confusionDmg)
{
	u16 baseAttack = CalcBaseAttackStat(attacker, defender, damageStruct, confusionDmg);
	u16 baseDefense = CalcBaseDefenseStat(attacker, defender, damageStruct, confusionDmg);
	s32 damage;
	
	gBattleMovePower = basePower;
	
	// Standard damage formula
	damage = basePower * baseAttack * (2 * gBattleMons[attacker].level / 5 + 2) / baseDefense / 50 + 2;

	// targets modifier
	if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && (GetBattlerMoveTargetType(attacker, damageStruct->move) == MOVE_TARGET_BOTH
	|| GetBattlerMoveTargetType(attacker, damageStruct->move) == MOVE_TARGET_FOES_AND_ALLY) && CountAliveMonsInBattle(defender, BATTLE_ALIVE_SIDE) > 1)
	    damage = (damage * 75) / 100;
	
	// Parental Bond modifier
	if (gSpecialStatuses[attacker].parentalBondState == PARENTAL_BOND_2ND_HIT)
		damage /= 4;
	
	// Weather modifiers
	if (IsBattlerWeatherAffected(attacker, WEATHER_SUN_ANY))
	{
		if (damageStruct->moveType == TYPE_FIRE)
			damage = (damage * 15) / 10;
		else if (damageStruct->moveType == TYPE_WATER)
			damage /= 2;
	}
	else if (IsBattlerWeatherAffected(attacker, WEATHER_RAIN_ANY))
	{
		if (damageStruct->moveType == TYPE_WATER)
			damage = (damage * 15) / 10;
		else if (damageStruct->moveType == TYPE_FIRE)
			damage /= 2;
	}
	
	// TODO: Glaive Rush modifier
	
	// Critical hit modifier
	if (damageStruct->isCrit)
		damage = (damage * 15) / 10;
	
	// Random factor modifier
	if (randomFactor)
	{
		damage *= 100 - (Random() % 16);
		damage /= 100;
	}
	
	// Stab modifier
	if (!confusionDmg && IS_BATTLER_OF_TYPE(attacker, damageStruct->moveType))
	{
		if (damageStruct->atkAbility == ABILITY_ADAPTABILITY)
			damage *= 2;
		else
			damage = (damage * 15) / 10;
	}
	
	// Type effectiveness modifier
	damage = (damage * damageStruct->effectiveness) / 10;
	
	// Burn modifier
	if (!confusionDmg && (gBattleMons[attacker].status1 & STATUS1_BURN) && IS_MOVE_PHYSICAL(damageStruct->move) && damageStruct->atkAbility != ABILITY_GUTS
	&& gBattleMoves[damageStruct->move].effect != EFFECT_FACADE)
	    damage /= 2;
	
	// TODO: Dynamax Cannon modifier
	
	
	// Minimize modifier
	if (gBattleMoves[damageStruct->move].flags.dmgMinimize && gStatuses3[defender] & STATUS3_MINIMIZED)
		damage *= 2;
	
	// Underground modifier
	if (gBattleMoves[damageStruct->move].flags.hitUnderground && gStatuses3[defender] & STATUS3_UNDERGROUND)
		damage *= 2;
	
	// Underwater modifier
	if (gBattleMoves[damageStruct->move].flags.hitUnderwater && gStatuses3[defender] & STATUS3_UNDERWATER)
		damage *= 2;
	
	// In air modifier
	if (gBattleMoves[damageStruct->move].flags.hitInAirDoubleDmg && gStatuses3[defender] & STATUS3_ON_AIR)
		damage *= 2;
	
	// Screens modifier
	if (!damageStruct->isCrit && damageStruct->atkAbility != ABILITY_INFILTRATOR && !confusionDmg)
	{
		if (((gSideStatuses[GetBattlerSide(defender)] & SIDE_STATUS_LIGHTSCREEN) && IS_MOVE_SPECIAL(damageStruct->move))
			|| ((gSideStatuses[GetBattlerSide(defender)] & SIDE_STATUS_REFLECT) && IS_MOVE_PHYSICAL(damageStruct->move)))
		{
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
				damage = (damage * 66) / 100;
			else
				damage /= 2;
		}
	}
	
	// TODO: Collision Course and Electro Drift modifiers
	
	
	// Others damage modifiers
	if (!confusionDmg)
	{
		// Check attacker's abilities
		switch (damageStruct->atkAbility)
		{
			case ABILITY_NEUROFORCE:
			    if (damageStruct->effectiveness == TYPE_MUL_SUPER_EFFECTIVE)
					damage = (damage * 125) / 100;
				break;
			case ABILITY_SNIPER:
				if (damageStruct->isCrit)
					damage = (damage * 15) / 10;
				break;
			case ABILITY_TINTED_LENS:
				if (damageStruct->effectiveness == TYPE_MUL_NOT_EFFECTIVE)
					damage *= 2;
				break;
		}
		
		// Check defender's abilities
		switch (damageStruct->defAbility)
		{
			case ABILITY_MULTISCALE:
			case ABILITY_SHADOW_SHIELD:
				if (gBattleMons[defender].hp == gBattleMons[defender].maxHP)
					damage /= 2;
				break;
			case ABILITY_FLUFFY:
			    if (IsMoveMakingContact(attacker, damageStruct->move))
					damage /= 2;
				if (damageStruct->moveType == TYPE_FIRE)
					damage *= 2;
				break;
			case ABILITY_PUNK_ROCK:
			    if (gBattleMoves[damageStruct->move].flags.soundMove)
					damage /= 2;
				break;
			case ABILITY_ICE_SCALES:
			    if (IS_MOVE_SPECIAL(damageStruct->move))
					damage /= 2;
				break;
			case ABILITY_FILTER:
			case ABILITY_SOLID_ROCK:
			case ABILITY_PRISM_ARMOR:
				if (damageStruct->effectiveness == TYPE_MUL_SUPER_EFFECTIVE)
					damage = (damage * 75) / 100;
				break;
			case ABILITY_HEATPROOF:
				if (damageStruct->moveType == TYPE_FIRE)
					damage /= 2;
				break;
			case ABILITY_DRY_SKIN:
				if (damageStruct->moveType == TYPE_FIRE)
					damage = (damage * 125) / 100;
				break;
			case ABILITY_WATER_BUBBLE:
			    if (damageStruct->moveType == TYPE_FIRE)
					damage /= 2;
				break;
		}
		
		// Check defender's ally abilities
		if (IsBattlerAlive(BATTLE_PARTNER(defender)))
		{
			switch (GetBattlerAbility(BATTLE_PARTNER(defender)))
			{
				case ABILITY_FRIEND_GUARD:
				    damage = (damage * 75) / 100;
					break;
			}
		}
	}
	
	// TODO: Z-Moves multiplier
	
	
	// TODO: Raid Shield multiplier
	
	
	if (damage == 0)
		damage = 1;
	
	FREE_AND_SET_NULL(damageStruct);
	
	return damage;
}

s32 CalculateMoveDamage(u16 move, u8 moveType, u8 attacker, u8 defender, bool8 isCrit, bool8 randomFactor)
{
	u8 affectedBy;
	u16 flags;
	u8 effectiveness = CalcTypeEffectivenessMultiplier(move, moveType, attacker, defender, FALSE, &affectedBy, &flags);
	struct DamageCalc *damageStruct = PopulateDamageStruct(attacker, defender, move, moveType, effectiveness, isCrit);
	return CalculateDamage(attacker, defender, damageStruct, GetMoveBasePower(attacker, defender, damageStruct), randomFactor, FALSE);
}

// Not affected by Life Orb
// Badge boost apply if raises the user's Attack or Defense
// Type enhancing items don't apply
// Silk Scarf, Choice Band, Thick Club and Light Ball boosts don't apply
s32 CalculateConfusionDamage(u8 attacker, u8 defender)
{
	return CalculateDamage(attacker, defender, PopulateDamageStruct(attacker, defender, MOVE_NONE, TYPE_MYSTERY, TYPE_MUL_NORMAL, FALSE), 40, TRUE, TRUE);
}

/////////////////////////////////
// MOVE BASE POWER CALCULATION //
/////////////////////////////////

static const u16 sWeightToDamageTable[] =
{
    100, 20,
    250, 40,
    500, 60,
    1000, 80,
    2000, 100,
    0xFFFF, 0xFFFF
};

static const u8 sFlailHpScaleToPowerTable[] =
{
    1, 200,
    4, 150,
    9, 100,
    16, 80,
    32, 40,
    48, 20
};

static bool8 IsRecoilDamageMoveEffect(u16 moveEffect)
{
	switch (moveEffect)
	{
		case EFFECT_RECOIL_IF_MISS:
		case EFFECT_RECOIL_25:
		case EFFECT_DOUBLE_EDGE:
		case EFFECT_RECOIL_33_STATUS:
		case EFFECT_RECOIL_50:
		    return TRUE;
	}
	return FALSE;
}

static inline bool8 CanReceiveBadgeBoost(u8 battlerId, u16 flagId)
{
#if BADGE_BOOST
    if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_EREADER_TRAINER)) && GetBattlerSide(battlerId) == B_SIDE_PLAYER
	&& FlagGet(flagId))
		return TRUE;
#endif
    return FALSE;
}

static inline u16 GetSupremeOverlordModifier(u8 battlerId)
{
	u8 i;
	u16 modifier = 0;
	
	if (gBattleStruct->supremeOverlordBoosts[battlerId])
	{
		for (i = 0; i < gBattleStruct->supremeOverlordBoosts[battlerId]; i++)
			modifier += 10;
	}
	return modifier;
}

static u16 GetMoveBasePower(u8 attacker, u8 defender, struct DamageCalc *damageStruct)
{
	u16 move = damageStruct->move;
	u16 basePower = gBattleMoves[move].power;
	u16 moveEffect = gBattleMoves[move].effect;
	u32 i;
	
	// Move effects
	switch (moveEffect)
	{
		case EFFECT_LOW_KICK:
		{
			u32 weight = GetBattlerWeight(defender);
			
			for (i = 0; sWeightToDamageTable[i] != 0xFFFF; i += 2)
			{
				if (sWeightToDamageTable[i] > weight)
					break;
			}
			
			if (sWeightToDamageTable[i] != 0xFFFF)
				basePower = sWeightToDamageTable[i + 1];
			else
				basePower = 120;
			break;
		}
		case EFFECT_TRIPLE_KICK:
			basePower += gBattleScripting.tripleKickPower;
			break;
		case EFFECT_FLAIL:
		{
			u8 hpFracion = GetScaledHPFraction(gBattleMons[attacker].hp, gBattleMons[attacker].maxHP, 48);
			
			for (i = 0; i < sizeof(sFlailHpScaleToPowerTable); i += 2)
			{
				if (hpFracion <= sFlailHpScaleToPowerTable[i])
					break;
			}
			basePower = sFlailHpScaleToPowerTable[i + 1];
			break;
		}
		case EFFECT_SOLARBEAM:
		    if (IsBattlerWeatherAffected(attacker, (WEATHER_ANY & ~(WEATHER_SUN_ANY | WEATHER_STRONG_WINDS))))
				basePower /= 2;
			break;
		case EFFECT_ROLLOUT:
			for (i = 1; i < (5 - gDisableStructs[attacker].rolloutTimer); i++)
				basePower *= 2;
			
			if ((gBattleMons[attacker].status2 & STATUS2_DEFENSE_CURL))
				basePower *= 2;
			break;
		case EFFECT_FURY_CUTTER:
			for (i = 1; i < gDisableStructs[attacker].furyCutterCounter; i++)
				basePower *= 2;
			break;
		case EFFECT_ERUPTION:
			basePower = (gBattleMons[attacker].hp * basePower) / gBattleMons[attacker].maxHP;
			break;
		case EFFECT_BEAT_UP:
		    basePower = (gBaseStats[GetMonData(&GetBattlerParty(attacker)[gBattleCommunication[0] - 1], MON_DATA_SPECIES)].baseAttack / 10) + 5;
			break;
		case EFFECT_FACADE:
			if ((gBattleMons[attacker].status1 & (STATUS1_PARALYSIS | STATUS1_BURN | STATUS1_PSN_ANY)))
				basePower *= 2;
			break;
		case EFFECT_SMELLINGSALT:
			if (!SubsBlockMove(attacker, defender, move) && (gBattleMons[defender].status1 & STATUS1_PARALYSIS))
				basePower *= 2;
			break;
		case EFFECT_WEATHER_BALL:
			if (IsBattlerWeatherAffected(attacker, WEATHER_ANY & ~(WEATHER_STRONG_WINDS)))
				basePower *= 2;
			break;
		case EFFECT_REVENGE:
			if ((gProtectStructs[attacker].physicalDmg && gProtectStructs[attacker].physicalBattlerId == defender)
			    || (gProtectStructs[attacker].specialDmg && gProtectStructs[attacker].specialBattlerId == defender))
				basePower *= 2;
		    break;
		case EFFECT_SPIT_UP:
			basePower = 100 * gDisableStructs[attacker].stockpileCounter;
			break;
		case EFFECT_RETURN:
			basePower = 10 * (gBattleMons[attacker].friendship) / 25;
			break;
		case EFFECT_FRUSTRATION:
			basePower = 10 * (255 - gBattleMons[attacker].friendship) / 25;
			break;
		case EFFECT_PRESENT:
		    basePower = gBattleStruct->presentBasePower;
			break;
		case EFFECT_MAGNITUDE:
			basePower = gBattleStruct->magnitudeBasePower;
			break;
	}
	
	// Attacker's abilities
	switch (damageStruct->atkAbility)
	{
		case ABILITY_NORMALIZE:
		    if (damageStruct->moveType == TYPE_NORMAL)
			{
				NORMALIZE_CHECK:
				if (moveEffect != EFFECT_HIDDEN_POWER && moveEffect != EFFECT_WEATHER_BALL && moveEffect != EFFECT_NATURAL_GIFT
				&& moveEffect != EFFECT_CHANGE_TYPE_ON_ITEM && moveEffect != EFFECT_TERRAIN_PULSE)
				    basePower = (12 * basePower) / 10;
			}
			break;
		case ABILITY_REFRIGERATE:
		    if (damageStruct->moveType == TYPE_ICE)
				goto NORMALIZE_CHECK;
			break;
		case ABILITY_PIXILATE:
		    if (damageStruct->moveType == TYPE_FAIRY)
				goto NORMALIZE_CHECK;
			break;
		case ABILITY_AERILATE:
		    if (damageStruct->moveType == TYPE_FLYING)
				goto NORMALIZE_CHECK;
			break;
		case ABILITY_GALVANIZE:
		    if (damageStruct->moveType == TYPE_ELECTRIC)
				goto NORMALIZE_CHECK;
			break;
		case ABILITY_IRON_FIST:
		    if (gBattleMoves[move].flags.punchMove)
				basePower = (12 * basePower) / 10;
			break;
		case ABILITY_RECKLESS:
		    if (IsRecoilDamageMoveEffect(moveEffect))
				basePower = (12 * basePower) / 10;
			break;
		case ABILITY_RIVALRY:
		    if (GetBattlerGender(attacker) != MON_GENDERLESS && GetBattlerGender(defender) != MON_GENDERLESS)
			{
				if (ARE_BATTLERS_OF_SAME_GENDER(attacker, defender))
					basePower = (basePower * 125) / 100;
				else
					basePower = (basePower * 75) / 100;
			}
			break;
		case ABILITY_TECHNICIAN:
		    if (basePower <= 60)
				basePower = (basePower * 15) / 10;
			break;
		case ABILITY_SHEER_FORCE:
		    if (gBattleMoves[move].secondaryEffectChance)
				basePower = (basePower * 13) / 10;
			break;
		case ABILITY_TOXIC_BOOST:
		    if ((gBattleMons[attacker].status1 & STATUS1_PSN_ANY) && IS_MOVE_PHYSICAL(move))
				basePower = (15 * basePower) / 10;
			break;
		case ABILITY_FLARE_BOOST:
		    if ((gBattleMons[attacker].status1 & STATUS1_BURN) && IS_MOVE_SPECIAL(move))
				basePower = (15 * basePower) / 10;
			break;
		case ABILITY_ANALYTIC:
		    if (GetBattlerTurnOrderNum(attacker) == gBattlersCount - 1)
				basePower = (13 * basePower) / 10;
			break;
		case ABILITY_SAND_FORCE:
		    if (IsBattlerWeatherAffected(attacker, WEATHER_SANDSTORM_ANY) && (damageStruct->moveType == TYPE_ROCK || damageStruct->moveType == TYPE_GROUND
			|| damageStruct->moveType == TYPE_STEEL))
				basePower = (13 * basePower) / 10;
			break;
		case ABILITY_STRONG_JAW:
		    if (gBattleMoves[move].flags.bitingMove)
				basePower = (15 * basePower) / 10;
			break;
		case ABILITY_MEGA_LAUNCHER:
		    if (gBattleMoves[move].flags.pulseMove)
				basePower = (15 * basePower) / 10;
			break;
		case ABILITY_TOUGH_CLAWS:
		    if (IsMoveMakingContact(attacker, move))
				basePower = (13 * basePower) / 10;
			break;
		case ABILITY_STAKEOUT:
		    if (gDisableStructs[defender].isFirstTurn == 2)
				basePower *= 2;
			break;
		case ABILITY_WATER_BUBBLE:
		    if (damageStruct->moveType == TYPE_WATER)
				basePower *= 2;
			break;
		case ABILITY_PUNK_ROCK:
		    if (gBattleMoves[move].flags.soundMove)
				basePower = (13 * basePower) / 10;
			break;
		case ABILITY_STEELY_SPIRIT:
		    if (damageStruct->moveType == TYPE_STEEL)
				basePower = (15 * basePower) / 10;
			break;
		case ABILITY_SHARPNESS:
		    if (gBattleMoves[move].flags.slicingMove)
				basePower = (15 * basePower) / 10;
			break;
		case ABILITY_SUPREME_OVERLORD:
		    basePower = (basePower * (100 + GetSupremeOverlordModifier(attacker))) / 100;
			break;
	}
	
	// Attacker's ally abilities
	if (IsBattlerAlive(BATTLE_PARTNER(attacker)))
	{
		switch (GetBattlerAbility(BATTLE_PARTNER(attacker)))
		{
			case ABILITY_BATTERY:
			    if (IS_MOVE_SPECIAL(move))
					basePower = (basePower * 13) / 10;
				break;
			case ABILITY_POWER_SPOT:
			    basePower = (basePower * 13) / 10;
				break;
			case ABILITY_STEELY_SPIRIT:
			    if (damageStruct->moveType == TYPE_STEEL)
					basePower = (15 * basePower) / 10;
				break;
		}
	}
	
	// Aura abilities
	if ((ABILITY_ON_FIELD(ABILITY_DARK_AURA) && damageStruct->moveType == TYPE_DARK) || (ABILITY_ON_FIELD(ABILITY_FAIRY_AURA) && damageStruct->moveType == TYPE_FAIRY))
	{
		if (!ABILITY_ON_FIELD(ABILITY_AURA_BREAK))
			basePower = (basePower * 133) / 100;
		else
			basePower = (basePower * 75) / 100;
	}
	
	// Various effects
	if ((gStatuses3[attacker] & STATUS3_CHARGED_UP) && damageStruct->moveType == TYPE_ELECTRIC)
		basePower *= 2;
	
	if (gProtectStructs[attacker].helpingHand)
		basePower = (basePower * 15) / 10;
	
	if (((gFieldStatus & STATUS_FIELD_WATERSPORT) && damageStruct->moveType == TYPE_FIRE)
		|| ((gFieldStatus & STATUS_FIELD_MUDSPORT) && damageStruct->moveType == TYPE_ELECTRIC))
	    basePower = (basePower * 33) / 100;
	
	if (basePower == 0)
		basePower = 1;
	
	return basePower;
}

/////////////////////////////
// BASE ATTACK CALCULATION //
/////////////////////////////

static u16 CalcBaseAttackStat(u8 attacker, u8 defender, struct DamageCalc *damageStruct, bool8 confusionDmg)
{
	u8 statStages;
	u16 baseAttack;
	u16 move = damageStruct->move;
	
	if (IS_MOVE_PHYSICAL(move))
	{
		baseAttack = gBattleMons[attacker].attack;
		statStages = gBattleMons[attacker].statStages[STAT_ATK];
	}
	else
	{
		baseAttack = gBattleMons[attacker].spAttack;
		statStages = gBattleMons[attacker].statStages[STAT_SPATK];
	}
	
	// Check effects that ignores stat stages
	if ((damageStruct->isCrit && statStages < DEFAULT_STAT_STAGES) || (!confusionDmg && damageStruct->defAbility == ABILITY_UNAWARE))
		statStages = DEFAULT_STAT_STAGES;
	
	// Calc base attack stat
	baseAttack *= gStatStageRatios[statStages][0];
    baseAttack /= gStatStageRatios[statStages][1];
	
	// Calculate base attack modifiers
	if (!confusionDmg)
	{
		// Check attacker's abilities
		switch (damageStruct->atkAbility)
		{
		    case ABILITY_HUGE_POWER:
		    case ABILITY_PURE_POWER:
			    if (IS_MOVE_PHYSICAL(move))
					baseAttack *= 2;
			    break;
		    case ABILITY_HUSTLE:
			    if (IS_MOVE_PHYSICAL(move))
					baseAttack = (15 * baseAttack) / 10;
			    break;
		    case ABILITY_GUTS:
		        if (IS_MOVE_PHYSICAL(move) && (gBattleMons[attacker].status1 & STATUS1_ANY))
				    baseAttack = (15 * baseAttack) / 10;
			    break;
		    case ABILITY_SLOW_START:
		        if (IS_MOVE_PHYSICAL(move) && gDisableStructs[attacker].slowStartTimer)
				    baseAttack /= 2;
			    break;
		    case ABILITY_FLOWER_GIFT:
		        if (IS_MOVE_PHYSICAL(move) && IsBattlerWeatherAffected(attacker, WEATHER_SUN_ANY))
				    baseAttack = (15 * baseAttack) / 10;
			    break;
			case ABILITY_PLUS:
			case ABILITY_MINUS:
		        if (IS_MOVE_SPECIAL(move) && IsBattlerAlive(BATTLE_PARTNER(attacker))
				&& (GetBattlerAbility(BATTLE_PARTNER(attacker)) == ABILITY_PLUS || GetBattlerAbility(BATTLE_PARTNER(attacker)) == ABILITY_MINUS))
			        baseAttack = (15 * baseAttack) / 10;
				break;
			case ABILITY_SOLAR_POWER:
		        if (IS_MOVE_SPECIAL(move) && IsBattlerWeatherAffected(attacker, WEATHER_SUN_ANY))
					baseAttack = (15 * baseAttack) / 10;
				break;
			case ABILITY_OVERGROW:
			    if (damageStruct->moveType == TYPE_GRASS && gBattleMons[attacker].hp <= (gBattleMons[attacker].maxHP / 3))
					baseAttack = (15 * baseAttack) / 10;
				break;
			case ABILITY_BLAZE:
		        if (damageStruct->moveType == TYPE_FIRE && gBattleMons[attacker].hp <= (gBattleMons[attacker].maxHP / 3))
					baseAttack = (15 * baseAttack) / 10;
				break;
			case ABILITY_TORRENT:
			    if (damageStruct->moveType == TYPE_WATER && gBattleMons[attacker].hp <= (gBattleMons[attacker].maxHP / 3))
					baseAttack = (15 * baseAttack) / 10;
				break;
			case ABILITY_SWARM:
			    if (damageStruct->moveType == TYPE_BUG && gBattleMons[attacker].hp <= (gBattleMons[attacker].maxHP / 3))
					baseAttack = (15 * baseAttack) / 10;
				break;
			case ABILITY_DEFEATIST:
		        if (gBattleMons[attacker].hp <= (gBattleMons[attacker].maxHP / 2))
					baseAttack /= 2;
				break;
			case ABILITY_STEELWORKER:
		        if (damageStruct->moveType == TYPE_STEEL)
					baseAttack = (15 * baseAttack) / 10;
				break;
			case ABILITY_TRANSISTOR:
		        if (damageStruct->moveType == TYPE_ELECTRIC)
					baseAttack = (13 * baseAttack) / 10;
				break;
			case ABILITY_DRAGONS_MAW:
		        if (damageStruct->moveType == TYPE_DRAGON)
					baseAttack = (15 * baseAttack) / 10;
				break;
			case ABILITY_ROCKY_PAYLOAD:
		        if (damageStruct->moveType == TYPE_ROCK)
					baseAttack = (15 * baseAttack) / 10;
				break;
			case ABILITY_FLASH_FIRE:
			    if ((gBattleResources->flags->flags[attacker] & RESOURCE_FLAG_FLASH_FIRE) && damageStruct->moveType == TYPE_FIRE)
					baseAttack = (15 * baseAttack) / 10;
				break;
		}
		
		// Check attacker's ally abilities
		if (IsBattlerAlive(BATTLE_PARTNER(attacker)))
		{
			switch (GetBattlerAbility(BATTLE_PARTNER(attacker)))
			{
				case ABILITY_FLOWER_GIFT:
				    if (IS_MOVE_PHYSICAL(move) && IsBattlerWeatherAffected(attacker, WEATHER_SUN_ANY))
						baseAttack = (15 * baseAttack) / 10;
					break;
			}
		}
		
		// Check defender's abilities
		switch (damageStruct->defAbility)
		{
			case ABILITY_THICK_FAT:
				if (damageStruct->moveType == TYPE_FIRE || damageStruct->moveType == TYPE_ICE)
					baseAttack /= 2;
				break;
			case ABILITY_PURIFYING_SALT:
			    if (damageStruct->moveType == TYPE_GHOST)
					baseAttack /= 2;
				break;
		}
		
		// Ruin abilities
		if (IS_MOVE_PHYSICAL(move) && ABILITY_ON_FIELD_EXCPET_BATTLER(attacker, ABILITY_TABLETS_OF_RUIN) && damageStruct->atkAbility != ABILITY_TABLETS_OF_RUIN)
			baseAttack = (baseAttack * 75) / 100;
		
		if (IS_MOVE_SPECIAL(move) && ABILITY_ON_FIELD_EXCPET_BATTLER(attacker, ABILITY_VESSEL_OF_RUIN) && damageStruct->atkAbility != ABILITY_VESSEL_OF_RUIN)
			baseAttack = (baseAttack * 75) / 100;
	}
	
	// Badges modifier
	if (CanReceiveBadgeBoost(attacker, IS_MOVE_PHYSICAL(move) ? FLAG_BADGE01_GET : FLAG_BADGE07_GET))
		baseAttack = (baseAttack * 110) / 100;
	
	return baseAttack;
}

//////////////////////////////
// BASE DEFENSE CALCULATION //
//////////////////////////////

static u16 CalcBaseDefenseStat(u8 attacker, u8 defender, struct DamageCalc *damageStruct, bool8 confusionDmg)
{
	u8 statStages;
	u16 baseDefense;
	u16 move = damageStruct->move;
	
	if (IS_MOVE_PHYSICAL(move))
	{
		baseDefense = gBattleMons[defender].defense;
		statStages = gBattleMons[defender].statStages[STAT_DEF];
	}
	else
	{
		baseDefense = gBattleMons[defender].spDefense;
		statStages = gBattleMons[defender].statStages[STAT_SPDEF];
	}
	
	// Check effects that ignores stat stages
	if ((damageStruct->isCrit && statStages < DEFAULT_STAT_STAGES) || (!confusionDmg && damageStruct->atkAbility == ABILITY_UNAWARE))
		statStages = DEFAULT_STAT_STAGES;
	
	// Calc base defense stat
	baseDefense *= gStatStageRatios[statStages][0];
    baseDefense /= gStatStageRatios[statStages][1];
	
	// Calculate base defense modifiers
	if (!confusionDmg)
	{
		// Check defender's abilities
		switch (damageStruct->defAbility)
		{
			case ABILITY_MARVEL_SCALE:
				if (IS_MOVE_PHYSICAL(move) && (gBattleMons[defender].status1 & STATUS1_ANY))
					baseDefense = (15 * baseDefense) / 10;
				break;
			case ABILITY_FUR_COAT:
			    if (IS_MOVE_PHYSICAL(move))
					baseDefense *= 2;
				break;
			case ABILITY_FLOWER_GIFT:
			    if (IS_MOVE_SPECIAL(move) && IsBattlerWeatherAffected(defender, WEATHER_SUN_ANY))
					baseDefense = (15 * baseDefense) / 10;
				break;
		}
		
		// Check defender's ally abilities
		if (IsBattlerAlive(BATTLE_PARTNER(defender)))
		{
			switch (GetBattlerAbility(BATTLE_PARTNER(defender)))
			{
				case ABILITY_FLOWER_GIFT:
				    if (IS_MOVE_SPECIAL(move) && IsBattlerWeatherAffected(defender, WEATHER_SUN_ANY))
						baseDefense = (15 * baseDefense) / 10;
					break;
			}
		}
		
		// Ruin abilities
		if (IS_MOVE_PHYSICAL(move) && ABILITY_ON_FIELD_EXCPET_BATTLER(defender, ABILITY_SWORD_OF_RUIN) && damageStruct->defAbility != ABILITY_SWORD_OF_RUIN)
			baseDefense = (baseDefense * 75) / 100;
		
		if (IS_MOVE_SPECIAL(move) && ABILITY_ON_FIELD_EXCPET_BATTLER(defender, ABILITY_BEADS_OF_RUIN) && damageStruct->defAbility != ABILITY_BEADS_OF_RUIN)
			baseDefense = (baseDefense * 75) / 100;
	}
	
	// Badges modifier
	if (CanReceiveBadgeBoost(defender, IS_MOVE_PHYSICAL(move) ? FLAG_BADGE05_GET : FLAG_BADGE07_GET))
		baseDefense = (baseDefense * 110) / 100;
	
	// Sandstorm modifier
	if (IS_MOVE_SPECIAL(move) && IsBattlerWeatherAffected(defender, WEATHER_SANDSTORM_ANY) && IS_BATTLER_OF_TYPE(defender, TYPE_ROCK))
		baseDefense = (baseDefense * 15) / 10;
	
	// Hail modifier
#if HAIL_BOOST_DEFENSE
	if (IS_MOVE_PHYSICAL(move) && IsBattlerWeatherAffected(defender, WEATHER_HAIL_ANY) && IS_BATTLER_OF_TYPE(defender, TYPE_ICE))
		baseDefense = (baseDefense * 15) / 10;
#endif

    return baseDefense;
}

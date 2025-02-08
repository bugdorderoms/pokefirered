#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_interface.h"
#include "battle_move_effects.h"
#include "battle_util.h"
#include "calculate_base_damage.h"
#include "event_data.h"
#include "item.h"
#include "malloc.h"
#include "random.h"
#include "constants/abilities.h"
#include "constants/battle_string_ids.h"
#include "constants/hold_effects.h"
#include "constants/moves.h"

#define FLAG_CONFUSION_DAMAGE (1 << 0)
#define FLAG_AI_DAMAGE_CALC   (1 << 1)

struct DamageCalc
{
	u16 move;
	u16 atkAbility;
	u16 defAbility;
	u8 effectiveness;
	u8 moveType;
	u8 atkHoldEffect;
	u8 defHoldEffect;
	u16 atkHoldEffParam;
	u16 defHoldEffParam;
	bool8 isCrit;
	u8 flags;
	u8 moveSplit;
};

static u16 GetMoveBasePower(u8 attacker, u8 defender, struct DamageCalc *damageStruct);
static u16 CalcBaseAttackStat(u8 attacker, u8 defender, struct DamageCalc *damageStruct);
static u16 CalcBaseDefenseStat(u8 attacker, u8 defender, struct DamageCalc *damageStruct);

///////////////////////////////
// GLOBAL DAMAGE CALCULATION //
///////////////////////////////

static struct DamageCalc *PopulateDamageStruct(u8 attacker, u8 defender, u16 move, u8 moveType, u8 effectiveness, bool8 isCrit, u8 flags)
{
	struct DamageCalc *damageStruct = AllocZeroed(sizeof(struct DamageCalc));
	damageStruct->atkAbility = GetBattlerAbility(attacker);
	damageStruct->defAbility = GetBattlerAbility(defender);
	damageStruct->atkHoldEffect = GetBattlerItemHoldEffect(attacker, TRUE);
	damageStruct->defHoldEffect = GetBattlerItemHoldEffect(defender, TRUE);
	damageStruct->atkHoldEffParam = ItemId_GetHoldEffectParam(gBattleMons[attacker].item);
	damageStruct->defHoldEffParam = ItemId_GetHoldEffectParam(gBattleMons[defender].item);
	damageStruct->move = move;
	damageStruct->moveSplit = GetMoveSplit(move);
	damageStruct->moveType = moveType;
	damageStruct->effectiveness = effectiveness;
	damageStruct->isCrit = isCrit;
	damageStruct->flags = flags;
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
				if ((attacker->species == SPECIES_LATIAS || attacker->species == SPECIES_LATIOS)
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

static s32 CalculateDamage(u8 attacker, u8 defender, struct DamageCalc *damageStruct, u16 basePower, bool8 randomFactor)
{
	u16 baseAttack = CalcBaseAttackStat(attacker, defender, damageStruct);
	u16 baseDefense = CalcBaseDefenseStat(attacker, defender, damageStruct);
	u16 move = damageStruct->move;
	s32 damage;
	
	gBattleMovePower = basePower;
	
	// Standard damage formula
	damage = basePower * baseAttack * (2 * gBattleMons[attacker].level / 5 + 2) / baseDefense / 50 + 2;

	// targets modifier
	if (!DoesSpreadMoveStrikesOnlyOnce(attacker, defender, move, FALSE))
		damage = (damage * 75) / 100;
	
	// Parental Bond modifier
	if (gSpecialStatuses[attacker].parentalBondState == PARENTAL_BOND_2ND_HIT)
		damage /= 4;
	
	// Weather modifiers
	if (IsBattlerWeatherAffected(attacker, B_WEATHER_SUN_ANY))
	{
		if (damageStruct->moveType == TYPE_FIRE)
			damage = (damage * 15) / 10;
		else if (damageStruct->moveType == TYPE_WATER)
			damage /= 2;
	}
	else if (IsBattlerWeatherAffected(attacker, B_WEATHER_RAIN_ANY))
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
		damage *= 100 - RandomMax(16);
		damage /= 100;
	}
	
	// Stab modifier
	if (!(damageStruct->flags & FLAG_CONFUSION_DAMAGE) && IsBattlerOfType(attacker, damageStruct->moveType))
	{
		if (damageStruct->atkAbility == ABILITY_ADAPTABILITY)
			damage *= 2;
		else
			damage = (damage * 15) / 10;
	}
	
	// Type effectiveness modifier
	damage = (damage * damageStruct->effectiveness) / TYPE_MUL_NORMAL;
	
	// Burn modifier
	if (!(damageStruct->flags & FLAG_CONFUSION_DAMAGE) && gBattleMons[attacker].status1.id == STATUS1_BURN && damageStruct->moveSplit == SPLIT_PHYSICAL
	&& gBattleMoves[move].effect != EFFECT_FACADE && damageStruct->atkAbility != ABILITY_GUTS)
	    damage /= 2;
	
	// TODO: Dynamax Cannon modifier
	
	
	// Minimize modifier
	if (gBattleMoves[move].flags.dmgMinimize && (gStatuses3[defender] & STATUS3_MINIMIZED))
		damage *= 2;
	
	// Underground modifier
	if (gBattleMoves[move].flags.hitUnderground && (gStatuses3[defender] & STATUS3_UNDERGROUND))
		damage *= 2;
	
	// Underwater modifier
	if (gBattleMoves[move].flags.hitUnderwater && (gStatuses3[defender] & STATUS3_UNDERWATER))
		damage *= 2;
	
	// In air modifier
	if (gBattleMoves[move].flags.hitInAirDoubleDmg && (gStatuses3[defender] & STATUS3_ON_AIR))
		damage *= 2;
	
	// Screens modifier
	if (!damageStruct->isCrit && damageStruct->atkAbility != ABILITY_INFILTRATOR && !(damageStruct->flags & FLAG_CONFUSION_DAMAGE))
	{
		if (((gSideStatuses[GetBattlerSide(defender)] & SIDE_STATUS_LIGHTSCREEN) && damageStruct->moveSplit == SPLIT_SPECIAL)
		|| ((gSideStatuses[GetBattlerSide(defender)] & SIDE_STATUS_REFLECT) && damageStruct->moveSplit == SPLIT_PHYSICAL))
		{
			if (IsDoubleBattleForBattler(defender))
				damage = (damage * 66) / 100;
			else
				damage /= 2;
		}
	}
	
	// TODO: Collision Course and Electro Drift modifiers
	
	
	// Others damage modifiers
	if (!(damageStruct->flags & FLAG_CONFUSION_DAMAGE))
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
				if (BATTLER_MAX_HP(defender))
					damage /= 2;
				break;
			case ABILITY_FLUFFY:
			    if (IsMoveMakingContact(attacker, move))
					damage /= 2;
				if (damageStruct->moveType == TYPE_FIRE)
					damage *= 2;
				break;
			case ABILITY_PUNK_ROCK:
			    if (gBattleMoves[move].flags.soundMove)
					damage /= 2;
				break;
			case ABILITY_ICE_SCALES:
			    if (damageStruct->moveSplit == SPLIT_SPECIAL)
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

s32 CalculateMoveDamage(u16 move, u8 moveType, u8 attacker, u8 defender, bool8 isCrit)
{
	u16 flags;
	struct DamageCalc *damageStruct = PopulateDamageStruct(attacker, defender, move, moveType, TypeCalc(move, moveType, attacker, defender, FALSE, &flags), isCrit, 0);
	return CalculateDamage(attacker, defender, damageStruct, GetMoveBasePower(attacker, defender, damageStruct), TRUE);
}

// Not affected by Life Orb
// Type enhancing items don't apply
// Silk Scarf, Choice Band, Thick Club and Light Ball boosts don't apply
s32 CalculateConfusionDamage(void)
{
	u8 battler = gBattlerTarget = gBattlerAttacker;
	return CalculateDamage(battler, battler, PopulateDamageStruct(battler, battler, MOVE_NONE, TYPE_MYSTERY, TYPE_MUL_NORMAL, FALSE, FLAG_CONFUSION_DAMAGE), 40, TRUE);
}

s32 AI_CalcMoveDamage(u16 move, u8 attacker, u8 defender, u8 moveType, u8 effectiveness)
{
	struct DamageCalc *damageStruct = PopulateDamageStruct(attacker, defender, move, moveType, effectiveness, CalcMoveIsCritical(attacker, defender, move), FLAG_AI_DAMAGE_CALC);
	return CalculateDamage(attacker, defender, damageStruct, GetMoveBasePower(attacker, defender, damageStruct), FALSE);
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

static inline u16 GetSupremeOverlordModifier(u8 battlerId)
{
	u8 i;
	u16 modifier = 0;
	
	if (gDisableStructs[battlerId].supremeOverlordBoost > 0)
	{
		for (i = 0; i < gDisableStructs[battlerId].supremeOverlordBoost; i++)
			modifier += 10;
	}
	return modifier;
}

static inline u16 GetBeatUpPower(u8 battler, bool8 forAI)
{
	if (!forAI)
		return (gSpeciesInfo[GetMonData(&GetBattlerParty(battler)[gBattleCommunication[0] - 1], MON_DATA_SPECIES)].baseAttack / 10) + 5;
	else // Calc max possible damage
	{
		u32 power = 0;
		u8 i, numHits = GetNumBeatUpHits(battler);
		
		for (i = 0; i < numHits; i++)
			power += (gSpeciesInfo[GetMonData(&GetBattlerParty(battler)[i], MON_DATA_SPECIES)].baseAttack / 10) + 5;
		
		power /= numHits;
		
		return power;
	}
}

static u16 GetMoveBasePower(u8 attacker, u8 defender, struct DamageCalc *damageStruct)
{
	u16 move = damageStruct->move;
	u16 basePower = gBattleMoves[move].power;
	u16 moveEffect = gBattleMoves[move].effect;
	u32 i;
	
	// Moves
	switch (move)
	{
		case MOVE_WATER_SHURIKEN:
		    if (!(gBattleMons[attacker].status2 & STATUS2_TRANSFORMED) && gBattleMons[attacker].species == SPECIES_GRENINJA_ASH)
				basePower = 20;
			break;
	}
	
	// Move effects
	switch (moveEffect)
	{
		case EFFECT_DAMAGE_BASED_TARGET_WEIGHT:
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
		    if (damageStruct->flags & FLAG_AI_DAMAGE_CALC)
			{
				for (i = 0; i < gBattleMoves[move].flags.strikeCount; i++)
					basePower += gBattleMoves[move].argument.tripleKick.increment; // Get max possible dmg
			}
			else
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
		case EFFECT_SKIP_CHARGING_IN_WEATHER:
		    if (IsBattlerWeatherAffected(attacker, (B_WEATHER_ANY & ~(gBattleMoves[move].argument.twoTurns.weather | B_WEATHER_STRONG_WINDS))))
				basePower /= 2;
			break;
		case EFFECT_ROLLOUT:
			for (i = 1; i < (5 - gDisableStructs[attacker].rolloutTimer); i++)
				basePower *= 2;
			
			if (gBattleMons[attacker].status2 & STATUS2_DEFENSE_CURL)
				basePower *= 2;
			break;
		case EFFECT_FURY_CUTTER:
			for (i = 1; i < gDisableStructs[attacker].furyCutterCounter; i++)
				basePower *= 2;
			break;
		case EFFECT_RETURN:
			basePower = 10 * (gBattleMons[attacker].friendship) / 25;
			break;
		case EFFECT_PRESENT:
			basePower = (damageStruct->flags & FLAG_AI_DAMAGE_CALC) ? 120 : gBattleStruct->presentBasePower;
			break;
		case EFFECT_FRUSTRATION:
			basePower = 10 * (255 - gBattleMons[attacker].friendship) / 25;
			break;
		case EFFECT_MAGNITUDE:
			basePower = (damageStruct->flags & FLAG_AI_DAMAGE_CALC) ? 50 : gBattleStruct->magnitudeBasePower;
			break;
		case EFFECT_PURSUIT:
			if (gBattleStruct->pursuitSwitchDmg)
				basePower *= 2;
			break;
		case EFFECT_BEAT_UP:
		    basePower = GetBeatUpPower(attacker, (damageStruct->flags & FLAG_AI_DAMAGE_CALC));
			break;
		case EFFECT_FACADE:
			if (gBattleMons[attacker].status1.id == STATUS1_PARALYSIS || gBattleMons[attacker].status1.id == STATUS1_BURN || gBattleMons[attacker].status1.id == STATUS1_POISON
			|| gBattleMons[attacker].status1.id == STATUS1_TOXIC_POISON)
				basePower *= 2;
			break;
		case EFFECT_ERUPTION:
			basePower = (gBattleMons[attacker].hp * basePower) / gBattleMons[attacker].maxHP;
			break;
		case EFFECT_CURE_STATUS1_FROM_ARG:
			if (!SubsBlockMove(attacker, defender, move) && (gBattleMons[defender].status1.id == gBattleMoves[move].argument.cureStatus.statusId
			|| (gBattleMoves[move].argument.cureStatus.statusId == STATUS1_SLEEP && damageStruct->defAbility == ABILITY_COMATOSE)))
				basePower *= 2;
			break;
		case EFFECT_REVENGE:
			if ((gProtectStructs[attacker].physicalDmg && gProtectStructs[attacker].physicalBattlerId == defender)
			|| (gProtectStructs[attacker].specialDmg && gProtectStructs[attacker].specialBattlerId == defender))
				basePower *= 2;
		    break;
		case EFFECT_KNOCK_OFF:
			if (gBattleMons[defender].item && CanBattlerGetOrLoseItem(defender, gBattleMons[defender].item))
				basePower = (15 * basePower) / 10;
			break;
		case EFFECT_SPIT_UP:
			basePower = 100 * gDisableStructs[attacker].stockpileCounter;
			break;
		case EFFECT_WEATHER_BALL:
			if (IsBattlerWeatherAffected(attacker, B_WEATHER_ANY & ~(B_WEATHER_STRONG_WINDS)))
				basePower *= 2;
			break;
		case EFFECT_GYRO_BALL:
		    basePower = ((25 * GetBattlerTotalSpeed(defender)) / GetBattlerTotalSpeed(attacker)) + 1;
			
			if (basePower > 150)
				basePower = 150;
			break;
		case EFFECT_BRINE:
		    if (gBattleMons[defender].hp <= gBattleMons[defender].maxHP / 2)
				basePower *= 2;
			break;
		case EFFECT_NATURAL_GIFT:
		    basePower = gNaturalGiftTable[ITEM_TO_BERRY(GetBattlerItem(attacker))].power;
			break;
		case EFFECT_PAYBACK:
		    if (gBattleStruct->battlers[defender].chosenAction == B_ACTION_USE_ITEM || GetBattlerTurnOrderNum(attacker) > GetBattlerTurnOrderNum(defender))
				basePower *= 2;
			break;
		case EFFECT_ASSURANCE:
		    if (gProtectStructs[defender].physicalDmg || gProtectStructs[defender].specialDmg || gProtectStructs[defender].confusionSelfDmg)
				basePower *= 2;
			break;
		case EFFECT_TRUMP_CARD:
			switch (gBattleMons[attacker].pp[gBattleStruct->battlers[attacker].chosenMovePosition])
			{
				case 0:
				    basePower = 200;
					break;
				case 1:
				    basePower = 80;
					break;
				case 2:
				    basePower = 60;
					break;
				case 3:
				    basePower = 50;
					break;
				default:
				    basePower = 40;
					break;
			}
			break;
		case EFFECT_WRING_OUT:
		    basePower = 120 * (gBattleMons[defender].hp / gBattleMons[defender].maxHP);
			break;
		case EFFECT_PUNISHMENT:
		    basePower = 60 + (CountBattlerStatIncreases(defender, TRUE) * 20);
			
			if (basePower > 200)
				basePower = 200;
			break;
	}
	
	if (basePower == 0)
		basePower = 1;
	
	// Attacker's abilities
	switch (damageStruct->atkAbility)
	{
		case ABILITY_NORMALIZE:
		    if (damageStruct->moveType == TYPE_NORMAL)
			{
				NORMALIZE_CHECK:
				if (!GET_MOVEEFFECT_TABLE(moveEffect).normalizeUnaffected)
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
		    if ((move != MOVE_STRUGGLE && gBattleMoves[move].flags.recoilDivisor) || moveEffect == EFFECT_RECOIL_IF_MISS)
				basePower = (12 * basePower) / 10;
			break;
		case ABILITY_RIVALRY:
		{
			u8 atkGender = GetBattlerGender(attacker), defGender = GetBattlerGender(defender);
			
		    if (atkGender != MON_GENDERLESS && defGender != MON_GENDERLESS)
			{
				if (atkGender == defGender)
					basePower = (basePower * 125) / 100;
				else
					basePower = (basePower * 75) / 100;
			}
			break;
		}
		case ABILITY_TECHNICIAN:
		    if (basePower <= 60)
				basePower = (basePower * 15) / 10;
			break;
		case ABILITY_SHEER_FORCE:
		    if (MoveIsAffectedBySheerForce(move))
				basePower = (basePower * 13) / 10;
			break;
		case ABILITY_TOXIC_BOOST:
		    if ((gBattleMons[attacker].status1.id == STATUS1_POISON || gBattleMons[attacker].status1.id == STATUS1_TOXIC_POISON) && damageStruct->moveSplit == SPLIT_PHYSICAL)
				basePower = (15 * basePower) / 10;
			break;
		case ABILITY_FLARE_BOOST:
		    if (gBattleMons[attacker].status1.id == STATUS1_BURN && damageStruct->moveSplit == SPLIT_SPECIAL)
				basePower = (15 * basePower) / 10;
			break;
		case ABILITY_ANALYTIC:
		    if (GetBattlerTurnOrderNum(attacker) == gBattlersCount - 1)
				basePower = (13 * basePower) / 10;
			break;
		case ABILITY_SAND_FORCE:
		    if (IsBattlerWeatherAffected(attacker, B_WEATHER_SANDSTORM) && (damageStruct->moveType == TYPE_ROCK || damageStruct->moveType == TYPE_GROUND
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
			    if (damageStruct->moveSplit == SPLIT_SPECIAL)
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
	
	// Charge
	if ((gStatuses3[attacker] & STATUS3_CHARGED_UP) && damageStruct->moveType == TYPE_ELECTRIC)
		basePower *= 2;
	
	// Helping Hand
	for (i = 0; i < gProtectStructs[attacker].helpingHandUses; i++)
		basePower = (basePower * 15) / 10;
	
	// Water/Mud Sport
	if (((gFieldStatus & STATUS_FIELD_WATERSPORT) && damageStruct->moveType == TYPE_FIRE)
		|| ((gFieldStatus & STATUS_FIELD_MUDSPORT) && damageStruct->moveType == TYPE_ELECTRIC))
	    basePower = (basePower * 33) / 100;
	
	// Me First
	if (gBattleStruct->meFirstBoost)
		basePower = (basePower * 15) / 10;
	
	if (basePower == 0)
		basePower = 1;
	
	return basePower;
}

/////////////////////////////
// BASE ATTACK CALCULATION //
/////////////////////////////

static u16 CalcBaseAttackStat(u8 attacker, u8 defender, struct DamageCalc *damageStruct)
{
	u8 statStages;
	u16 baseAttack;
	u16 move = damageStruct->move;
	
	if (damageStruct->moveSplit == SPLIT_PHYSICAL)
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
	if ((damageStruct->isCrit && statStages < DEFAULT_STAT_STAGES) || (!(damageStruct->flags & FLAG_CONFUSION_DAMAGE) && damageStruct->defAbility == ABILITY_UNAWARE))
		statStages = DEFAULT_STAT_STAGES;
	
	// Calc base attack stat
	baseAttack *= gStatStageRatios[statStages][0];
    baseAttack /= gStatStageRatios[statStages][1];
	
	// Calculate base attack modifiers
	if (!(damageStruct->flags & FLAG_CONFUSION_DAMAGE))
	{
		// Check attacker's abilities
		switch (damageStruct->atkAbility)
		{
		    case ABILITY_HUGE_POWER:
		    case ABILITY_PURE_POWER:
			    if (damageStruct->moveSplit == SPLIT_PHYSICAL)
					baseAttack *= 2;
			    break;
		    case ABILITY_HUSTLE:
			case ABILITY_GORILLA_TACTICS:
			    if (damageStruct->moveSplit == SPLIT_PHYSICAL)
					baseAttack = (15 * baseAttack) / 10;
			    break;
		    case ABILITY_GUTS:
		        if (damageStruct->moveSplit == SPLIT_PHYSICAL && gBattleMons[attacker].status1.id)
				    baseAttack = (15 * baseAttack) / 10;
			    break;
		    case ABILITY_SLOW_START:
			    if (gDisableStructs[attacker].slowStartTimer)
				{
					// Halves Sp. Attack of type based Z-Moves
					if (damageStruct->moveSplit == SPLIT_PHYSICAL || (damageStruct->moveSplit == SPLIT_SPECIAL && move >= FIRST_Z_MOVE && move <= LAST_TYPED_Z_MOVE))
						baseAttack /= 2;
				}
			    break;
		    case ABILITY_FLOWER_GIFT:
		        if (damageStruct->moveSplit == SPLIT_PHYSICAL && IsBattlerWeatherAffected(attacker, B_WEATHER_SUN_ANY))
				    baseAttack = (15 * baseAttack) / 10;
			    break;
			case ABILITY_PLUS:
			case ABILITY_MINUS:
		        if (damageStruct->moveSplit == SPLIT_SPECIAL && IsBattlerAlive(BATTLE_PARTNER(attacker))
				&& (GetBattlerAbility(BATTLE_PARTNER(attacker)) == ABILITY_PLUS || GetBattlerAbility(BATTLE_PARTNER(attacker)) == ABILITY_MINUS))
			        baseAttack = (15 * baseAttack) / 10;
				break;
			case ABILITY_SOLAR_POWER:
		        if (damageStruct->moveSplit == SPLIT_SPECIAL && IsBattlerWeatherAffected(attacker, B_WEATHER_SUN_ANY))
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
			    if (gDisableStructs[attacker].flashFireBoost && damageStruct->moveType == TYPE_FIRE)
					baseAttack = (15 * baseAttack) / 10;
				break;
		}
		
		// Check attacker's ally abilities
		if (IsBattlerAlive(BATTLE_PARTNER(attacker)))
		{
			switch (GetBattlerAbility(BATTLE_PARTNER(attacker)))
			{
				case ABILITY_FLOWER_GIFT:
				    if (damageStruct->moveSplit == SPLIT_PHYSICAL && IsBattlerWeatherAffected(attacker, B_WEATHER_SUN_ANY))
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
		if (damageStruct->moveSplit == SPLIT_PHYSICAL && ABILITY_ON_FIELD_EXCPET_BATTLER(attacker, ABILITY_TABLETS_OF_RUIN)
		&& damageStruct->atkAbility != ABILITY_TABLETS_OF_RUIN)
			baseAttack = (baseAttack * 75) / 100;
		
		if (damageStruct->moveSplit == SPLIT_SPECIAL && ABILITY_ON_FIELD_EXCPET_BATTLER(attacker, ABILITY_VESSEL_OF_RUIN)
		&& damageStruct->atkAbility != ABILITY_VESSEL_OF_RUIN)
			baseAttack = (baseAttack * 75) / 100;
	}
	
	// Badges modifier
	if (CanReceiveBadgeBoost(attacker, damageStruct->moveSplit == SPLIT_PHYSICAL ? FLAG_BADGE01_GET : FLAG_BADGE07_GET))
		baseAttack = (baseAttack * 110) / 100;
	
	return baseAttack;
}

//////////////////////////////
// BASE DEFENSE CALCULATION //
//////////////////////////////

static u16 CalcBaseDefenseStat(u8 attacker, u8 defender, struct DamageCalc *damageStruct)
{
	u8 statStages;
	u16 baseDefense;
	
	if (damageStruct->moveSplit == SPLIT_PHYSICAL)
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
	if ((damageStruct->isCrit && statStages < DEFAULT_STAT_STAGES) || (!(damageStruct->flags & FLAG_CONFUSION_DAMAGE) && damageStruct->atkAbility == ABILITY_UNAWARE))
		statStages = DEFAULT_STAT_STAGES;
	
	// Calc base defense stat
	baseDefense *= gStatStageRatios[statStages][0];
    baseDefense /= gStatStageRatios[statStages][1];
	
	// Calculate base defense modifiers
	if (!(damageStruct->flags & FLAG_CONFUSION_DAMAGE))
	{
		// Check defender's abilities
		switch (damageStruct->defAbility)
		{
			case ABILITY_MARVEL_SCALE:
				if (damageStruct->moveSplit == SPLIT_PHYSICAL && gBattleMons[defender].status1.id)
					baseDefense = (15 * baseDefense) / 10;
				break;
			case ABILITY_FUR_COAT:
			    if (damageStruct->moveSplit == SPLIT_PHYSICAL)
					baseDefense *= 2;
				break;
			case ABILITY_FLOWER_GIFT:
			    if (damageStruct->moveSplit == SPLIT_SPECIAL && IsBattlerWeatherAffected(defender, B_WEATHER_SUN_ANY))
					baseDefense = (15 * baseDefense) / 10;
				break;
		}
		
		// Check defender's ally abilities
		if (IsBattlerAlive(BATTLE_PARTNER(defender)))
		{
			switch (GetBattlerAbility(BATTLE_PARTNER(defender)))
			{
				case ABILITY_FLOWER_GIFT:
				    if (damageStruct->moveSplit == SPLIT_SPECIAL && IsBattlerWeatherAffected(defender, B_WEATHER_SUN_ANY))
						baseDefense = (15 * baseDefense) / 10;
					break;
			}
		}
		
		// Ruin abilities
		if (damageStruct->moveSplit == SPLIT_PHYSICAL && ABILITY_ON_FIELD_EXCPET_BATTLER(defender, ABILITY_SWORD_OF_RUIN)
		&& damageStruct->defAbility != ABILITY_SWORD_OF_RUIN)
			baseDefense = (baseDefense * 75) / 100;
		
		if (damageStruct->moveSplit == SPLIT_SPECIAL && ABILITY_ON_FIELD_EXCPET_BATTLER(defender, ABILITY_BEADS_OF_RUIN)
		&& damageStruct->defAbility != ABILITY_BEADS_OF_RUIN)
			baseDefense = (baseDefense * 75) / 100;
	}
	
	// Badges modifier
	if (CanReceiveBadgeBoost(defender, damageStruct->moveSplit == SPLIT_PHYSICAL ? FLAG_BADGE05_GET : FLAG_BADGE07_GET))
		baseDefense = (baseDefense * 110) / 100;
	
	// Sandstorm modifier
	if (damageStruct->moveSplit == SPLIT_SPECIAL && IsBattlerWeatherAffected(defender, B_WEATHER_SANDSTORM) && IsBattlerOfType(defender, TYPE_ROCK))
		baseDefense = (baseDefense * 15) / 10;
	
	// Hail modifier
#if HAIL_BOOST_DEFENSE
	if (damageStruct->moveSplit == SPLIT_PHYSICAL && IsBattlerWeatherAffected(defender, B_WEATHER_HAIL) && IsBattlerOfType(defender, TYPE_ICE))
		baseDefense = (baseDefense * 15) / 10;
#endif

    return baseDefense;
}

////////////////////////////////////
// TYPE EFFECTIVENESS CALCULATION //
////////////////////////////////////

// Update effectiveness result for the type calc
static void UpdateMoveResults(u8 *multiplier, u8 mod, u16 move, u16 *flags)
{
    switch (mod)
    {
		case TYPE_MUL_NO_EFFECT:
		    *multiplier = TYPE_MUL_NO_EFFECT;
		    *flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
			*flags &= ~(MOVE_RESULT_NOT_VERY_EFFECTIVE | MOVE_RESULT_SUPER_EFFECTIVE);
			break;
		case TYPE_MUL_NOT_EFFECTIVE:
		    if (!IS_MOVE_STATUS(move) && !(*flags & MOVE_RESULT_NO_EFFECT))
			{
				if (*flags & MOVE_RESULT_SUPER_EFFECTIVE)
					*flags &= ~(MOVE_RESULT_SUPER_EFFECTIVE);
				else
					*flags |= MOVE_RESULT_NOT_VERY_EFFECTIVE;
				
				*multiplier = TYPE_MUL_NOT_EFFECTIVE;
			}
			break;
		case TYPE_MUL_SUPER_EFFECTIVE:
		    if (!IS_MOVE_STATUS(move) && !(*flags & MOVE_RESULT_NO_EFFECT))
			{
				if (*flags & MOVE_RESULT_NOT_VERY_EFFECTIVE)
					*flags &= ~(MOVE_RESULT_NOT_VERY_EFFECTIVE);
				else
					*flags |= MOVE_RESULT_SUPER_EFFECTIVE;
				
				*multiplier = TYPE_MUL_SUPER_EFFECTIVE;
			}
			break;
    }
}

// Get effectiveness betwen two types
u8 GetTypeModifier(u8 atkType, u8 defType)
{
	u8 modifier = gTypeEffectivenessTable[atkType][defType];
	
	if (gBattleStruct->battleChallenge == TRAINER_CHALLENGE_INVERSE_BATTLE)
	{
		switch (modifier)
		{
			case TYPE_MUL_SUPER_EFFECTIVE:
				modifier = TYPE_MUL_NOT_EFFECTIVE;
				break;
			case TYPE_MUL_NO_EFFECT:
			case TYPE_MUL_NOT_EFFECTIVE:
				modifier = TYPE_MUL_SUPER_EFFECTIVE;
				break;
		}
	}
	return modifier;
}

static void MulByTypeEffectiveness(u16 move, u8 moveType, u16 atkAbility, u8 defender, u8 defenderType, u8 *multiplier, bool8 setAbilityFlags, u16 *flags)
{
	u8 mod = GetTypeModifier(moveType, defenderType);
	
	// Check Foresight and Scrappy on Ghost types
	if ((moveType == TYPE_FIGHTING || moveType == TYPE_NORMAL) && defenderType == TYPE_GHOST && mod == TYPE_MUL_NO_EFFECT && (move == MOVE_GLARE
	|| (gBattleMons[defender].status2 & STATUS2_FORESIGHT) || atkAbility == ABILITY_SCRAPPY || atkAbility == ABILITY_MINDS_EYE))
	    mod = TYPE_MUL_NORMAL;
	
	// Check Miracle Eye
	if (moveType == TYPE_PSYCHIC && defenderType == TYPE_DARK && mod == TYPE_MUL_NO_EFFECT && (gBattleMons[defender].status2 & STATUS2_MIRACLE_EYE))
		mod = TYPE_MUL_NORMAL;
	
	// Check ground immunities
	if (moveType == TYPE_GROUND && mod == TYPE_MUL_NO_EFFECT && IsBattlerGrounded(defender))
		mod = TYPE_MUL_NORMAL;
	
	// Check strong winds
	if (IsBattlerWeatherAffected(defender, B_WEATHER_STRONG_WINDS) && defenderType == TYPE_FLYING && mod == TYPE_MUL_SUPER_EFFECTIVE)
	{
		mod = TYPE_MUL_NORMAL;
		
		if (setAbilityFlags && !IS_MOVE_STATUS(move) && gBattleStruct->strongWindsMessageState == 0)
			++gBattleStruct->strongWindsMessageState;
	}
	
	// Check moves that dont display "super effective" or "not very effective" messages
	if (GET_MOVE_MOVEEFFECT_TABLE(move).noEffectiveness && (mod == TYPE_MUL_SUPER_EFFECTIVE || mod == TYPE_MUL_NOT_EFFECTIVE))
		mod = TYPE_MUL_NORMAL;
	
	// Check status moves, except Thunder Wave
	if (IS_MOVE_STATUS(move) && move != MOVE_THUNDER_WAVE)
		mod = TYPE_MUL_NORMAL;
	
	UpdateMoveResults(multiplier, mod, move, flags);
}

static u8 CalcTypeEffectivenessMultiplierInternal(u16 move, u8 moveType, u16 atkAbility, u8 defender, u8 multiplier, bool8 setAbilityFlags, u16 *flags)
{
	u16 defAbility;
	u8 types[3];
	
	GetBattlerTypes(defender, types);
	
	MulByTypeEffectiveness(move, moveType, atkAbility, defender, types[0], &multiplier, setAbilityFlags, flags);
	
	if (types[0] != types[1])
		MulByTypeEffectiveness(move, moveType, atkAbility, defender, types[1], &multiplier, setAbilityFlags, flags);
	
	if (types[2] != TYPE_MYSTERY && types[2] != types[0] && types[2] != types[1])
		MulByTypeEffectiveness(move, moveType, atkAbility, defender, types[2], &multiplier, setAbilityFlags, flags);
	
	defAbility = GetBattlerAbility(defender);
	
	if (!IS_MOVE_STATUS(move) || move == MOVE_THUNDER_WAVE)
	{
		if (moveType == TYPE_GROUND && !IsBattlerGrounded(defender) && defAbility == ABILITY_LEVITATE)
	    {
	    	multiplier = TYPE_MUL_NO_EFFECT;
	    	*flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
	    	
	    	if (setAbilityFlags)
	    		gBattleCommunication[MISS_TYPE] = B_MSG_ABILITY_AVOID;
	    }
	    
	    if (!IS_MOVE_STATUS(move) && defAbility == ABILITY_WONDER_GUARD && multiplier != TYPE_MUL_SUPER_EFFECTIVE)
	    {
	    	multiplier = TYPE_MUL_NO_EFFECT;
	    	*flags |= MOVE_RESULT_MISSED;
	    	
	    	if (setAbilityFlags)
	    		gBattleCommunication[MISS_TYPE] = B_MSG_ABILITY_AVOID;
	    }
	}
	return multiplier;
}

static u8 CalcTypeEffectivenessMultiplier(u16 move, u8 moveType, u16 atkAbility, u8 defender, bool8 setAbilityFlags, u16 *flags)
{
	u8 multiplier = TYPE_MUL_NORMAL;
	
	if (move != MOVE_STRUGGLE && moveType != TYPE_MYSTERY)
	{
		multiplier = CalcTypeEffectivenessMultiplierInternal(move, moveType, atkAbility, defender, multiplier, setAbilityFlags, flags);
	}
	return multiplier;
}

u8 TypeCalc(u16 move, u8 moveType, u8 attacker, u8 defender, bool8 setAbilityFlags, u16 *flags)
{
	return CalcTypeEffectivenessMultiplier(move, moveType, GetBattlerAbility(attacker), defender, setAbilityFlags, flags);
}

// Calc effectiveness betwen a party mon's move and the defender
u8 AI_TypeCalc(struct Pokemon *mon, u16 move, u8 defender)
{
	u8 effectiveness, battler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
	struct BattlePokemon savedCopy = gBattleMons[battler];
	u32 status3 = gStatuses3[battler];
	u16 flags;
	
	// Overrrides the opponent's mon data with the ones of its party for the calculation
	CopyPokemonToBattleMon(battler, mon, &gBattleMons[battler], TRUE);
	gStatuses3[battler] = 0;
	
	effectiveness = TypeCalc(move, GetBattlerMoveType(battler, move), battler, defender, FALSE, &flags);
	
	gBattleMons[battler] = savedCopy;
	gStatuses3[battler] = status3;
	
	return effectiveness;
}

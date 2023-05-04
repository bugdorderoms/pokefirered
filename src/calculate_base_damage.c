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
#include "pokedex.h"
#include "strings.h"
#include "overworld.h"
#include "party_menu.h"
#include "field_specials.h"
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
#include "constants/inserts.h"

static u16 GetModifiedMovePower(u8 battlerIdAtk, u8 battlerIdDef, u16 move);
static u32 GetBattlerWeight(u8 battler);

static const u8 sFlailHpScaleToPowerTable[] =
{
    1, 200,
    4, 150,
    9, 100,
    16, 80,
    32, 40,
    48, 20
};

static const u16 sWeightToDamageTable[] =
{
    100, 20,
    250, 40,
    500, 60,
    1000, 80,
    2000, 100,
    0xFFFF, 0xFFFF
};

static const u8 sHoldEffectToType[][2] = 
{
    {HOLD_EFFECT_BUG_POWER, TYPE_BUG},
    {HOLD_EFFECT_STEEL_POWER, TYPE_STEEL},
    {HOLD_EFFECT_GROUND_POWER, TYPE_GROUND},
    {HOLD_EFFECT_ROCK_POWER, TYPE_ROCK},
    {HOLD_EFFECT_GRASS_POWER, TYPE_GRASS},
    {HOLD_EFFECT_DARK_POWER, TYPE_DARK},
    {HOLD_EFFECT_FIGHTING_POWER, TYPE_FIGHTING},
    {HOLD_EFFECT_ELECTRIC_POWER, TYPE_ELECTRIC},
    {HOLD_EFFECT_WATER_POWER, TYPE_WATER},
    {HOLD_EFFECT_FLYING_POWER, TYPE_FLYING},
    {HOLD_EFFECT_POISON_POWER, TYPE_POISON},
    {HOLD_EFFECT_ICE_POWER, TYPE_ICE},
    {HOLD_EFFECT_GHOST_POWER, TYPE_GHOST},
    {HOLD_EFFECT_PSYCHIC_POWER, TYPE_PSYCHIC},
    {HOLD_EFFECT_FIRE_POWER, TYPE_FIRE},
    {HOLD_EFFECT_DRAGON_POWER, TYPE_DRAGON},
    {HOLD_EFFECT_NORMAL_POWER, TYPE_NORMAL},
};

static u32 GetBattlerWeight(u8 battler)
{
	u32 weight = GetPokedexHeightWeight(SpeciesToNationalPokedexNum(gBattleMons[battler].species), 1);
	
	switch (GetBattlerAbility(battler))
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

static u16 GetModifiedMovePower(u8 battlerIdAtk, u8 battlerIdDef, u16 move)
{
	s32 i, data;
	struct BattlePokemon attacker = gBattleMons[battlerIdAtk];
	struct BattlePokemon defender = gBattleMons[battlerIdDef];
	u16 power = gBattleMoves[move].power;
	
	switch (gBattleMoves[move].effect)
	{
		case EFFECT_FLAIL:
			data = GetScaledHPFraction(attacker.hp, attacker.maxHP, 48);
			
			for (i = 0; i < (s32)sizeof(sFlailHpScaleToPowerTable); i += 2)
			{
				if (data <= sFlailHpScaleToPowerTable[i])
					break;
			}
			power = sFlailHpScaleToPowerTable[i + 1];
			break;
		case EFFECT_ROLLOUT:
			for (i = 1; i < (5 - gDisableStructs[battlerIdAtk].rolloutTimer); i++)
				power *= 2;
			
			if (attacker.status2 & STATUS2_DEFENSE_CURL)
				power *= 2;
			break;
		case EFFECT_FURY_CUTTER:
			for (i = 1; i < gDisableStructs[battlerIdAtk].furyCutterCounter; i++)
				power *= 2;
			break;
		case EFFECT_RETURN:
			power = 10 * (attacker.friendship) / 25;
			break;
		case EFFECT_FRUSTRATION:
			power = 10 * (255 - attacker.friendship) / 25;
			break;
		case EFFECT_PRESENT:
			do
			{
				data = Random() & 0xFF;
			} while (data >= 204);
			
			if (data < 102)
				power = 40;
			else if (data < 178)
				power = 80;
			else
				power = 120;
			break;
		case EFFECT_MAGNITUDE:
			power = gBattleStruct->magnitudeBasePower;
			break;
		case EFFECT_TRIPLE_KICK:
			power += gBattleScripting.tripleKickPower;
			gBattleScripting.tripleKickPower += 10;
			break;
		case EFFECT_ERUPTION:
			power = attacker.hp * power / attacker.maxHP;
			
			if (power == 0)
				power = 1;
			break;
		case EFFECT_LOW_KICK:
			data = GetBattlerWeight(battlerIdDef);
			
			for (i = 0; sWeightToDamageTable[i] != 0xFFFF; i += 2)
			{
				if (sWeightToDamageTable[i] > data)
					break;
			}
			
			if (sWeightToDamageTable[i] != 0xFFFF)
				power = sWeightToDamageTable[i + 1];
			else
				power = 120;
			break;
		case EFFECT_SPIT_UP:
			power = 100 * gDisableStructs[battlerIdAtk].stockpileCounter;
			break;
		case EFFECT_REVENGE:
			if ((gProtectStructs[battlerIdAtk].physicalDmg != 0 && gProtectStructs[battlerIdAtk].physicalBattlerId == battlerIdDef)
			    || (gProtectStructs[battlerIdAtk].specialDmg != 0 && gProtectStructs[battlerIdAtk].specialBattlerId == battlerIdDef))
				power *= 2;
		case EFFECT_WEATHER_BALL:
			if (IsBattlerWeatherAffected(battlerIdAtk, WEATHER_ANY))
				power *= 2;
			break;
		case EFFECT_SMELLINGSALT:
			if (!SubsBlockMove(battlerIdAtk, battlerIdDef, move) && defender.status1 & STATUS1_PARALYSIS)
				power *= 2;
			break;
		case EFFECT_GUST:
		case EFFECT_TWISTER:
			if (gStatuses3[battlerIdDef] & STATUS3_ON_AIR)
				power *= 2;
			break;
		case EFFECT_FACADE:
			if (attacker.status1 & (STATUS1_PARALYSIS | STATUS1_BURN | STATUS1_PSN_ANY))
				power *= 2;
			break;
		default:
			break;
	}
	
	return power;
}

s32 CalculateBaseDamage(u16 move, u8 type, u8 battlerIdAtk, u8 battlerIdDef, bool8 isConfusionDmg, bool8 isCrit, bool8 randomFactor)
{
	u8 attackerGender, defenderGender;
	u8 attackerHoldEffect, attackerHoldEffectParam, defenderHoldEffect, defenderHoldEffectParam;
	u16 attack, defense, spAttack, spDefense, atkStat, atkStage, defStat, defStage, flags = 0;
	u32 i;
	s32 j, damage;
	struct BattlePokemon *attacker = &gBattleMons[battlerIdAtk];
	struct BattlePokemon *defender = &gBattleMons[battlerIdDef];

    gBattleMovePower = GetModifiedMovePower(battlerIdAtk, battlerIdDef, move);
    TypeCalc(move, type, battlerIdAtk, battlerIdDef, FALSE, FALSE, &flags);
	
	attack = attacker->attack;
	defense = defender->defense;
	spAttack = attacker->spAttack;
	spDefense = defender->spDefense;

	attackerHoldEffect = GetBattlerItemHoldEffect(battlerIdAtk, TRUE);
	attackerHoldEffectParam = GetBattlerHoldEffectParam(battlerIdAtk);
	
	defenderHoldEffect = GetBattlerItemHoldEffect(battlerIdDef, TRUE);
	defenderHoldEffectParam = GetBattlerHoldEffectParam(battlerIdDef);

	if (!isConfusionDmg) // makes confusion damage not affected by effects of items, abilities or boosts granted by badges
	{
#if BADGE_BOOST
		if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_EREADER_TRAINER)))
		{
			if (FlagGet(FLAG_BADGE01_GET) && GetBattlerSide(battlerIdAtk) == B_SIDE_PLAYER)
				attack = (110 * attack) / 100;
			if (FlagGet(FLAG_BADGE05_GET) && GetBattlerSide(battlerIdDef) == B_SIDE_PLAYER)
				defense = (110 * defense) / 100;
			if (FlagGet(FLAG_BADGE07_GET) && GetBattlerSide(battlerIdAtk) == B_SIDE_PLAYER)
				spAttack = (110 * spAttack) / 100;
			if (FlagGet(FLAG_BADGE07_GET) && GetBattlerSide(battlerIdDef) == B_SIDE_PLAYER)
				spDefense = (110 * spDefense) / 100;
		}
#endif
		for (i = 0; i < NELEMS(sHoldEffectToType); i++)
		{
			if (attackerHoldEffect == sHoldEffectToType[i][0] && type == sHoldEffectToType[i][1])
			{
				attack = (attack * (attackerHoldEffectParam + 100)) / 100;
				spAttack = (spAttack * (attackerHoldEffectParam + 100)) / 100;
				break;
			}
		}
		// attacker items check
		switch (attackerHoldEffect)
		{
			case HOLD_EFFECT_CHOICE_BAND:
				attack = (15 * attack) / 10;
				break;
			case HOLD_EFFECT_SOUL_DEW:
				if (!(gBattleTypeFlags & (BATTLE_TYPE_BATTLE_TOWER)) && (attacker->species == SPECIES_LATIAS || attacker->species == SPECIES_LATIOS))
					spAttack = (15 * spAttack) / 10;
				break;
			case HOLD_EFFECT_DEEP_SEA_TOOTH:
				if (attacker->species == SPECIES_CLAMPERL)
					spAttack *= 2;
				break;
			case HOLD_EFFECT_LIGHT_BALL:
				if (attacker->species == SPECIES_PIKACHU)
					spAttack *= 2;
				break;
			case HOLD_EFFECT_THICK_CLUB:
				if (attacker->species == SPECIES_CUBONE || attacker->species == SPECIES_MAROWAK)
					attack *= 2;
				break;
		}
		// defender items check
		switch (defenderHoldEffect)
		{
			case HOLD_EFFECT_SOUL_DEW:
				if (!(gBattleTypeFlags & (BATTLE_TYPE_BATTLE_TOWER)) && (defender->species == SPECIES_LATIAS || defender->species == SPECIES_LATIOS))
					spDefense = (15 * spDefense) / 10;
				break;
			case HOLD_EFFECT_DEEP_SEA_SCALE:
				if (defender->species == SPECIES_CLAMPERL)
					spDefense *= 2;
				break;
			case HOLD_EFFECT_METAL_POWDER:
				if (defender->species == SPECIES_DITTO)
					defense *= 2;
				break;
		}
		// attacker abilities check
		switch (GetBattlerAbility(battlerIdAtk))
		{
			case ABILITY_HUGE_POWER:
			case ABILITY_PURE_POWER:
				attack *= 2;
				break;
			case ABILITY_HUSTLE:
				attack = (15 * attack) / 10;
				break;
			case ABILITY_PLUS:
			case ABILITY_MINUS:
				if (IsBattlerAlive(BATTLE_PARTNER(battlerIdAtk)) && (GetBattlerAbility(BATTLE_PARTNER(battlerIdAtk)) == ABILITY_PLUS || GetBattlerAbility(BATTLE_PARTNER(battlerIdAtk)) == ABILITY_MINUS))
					spAttack = (15 * spAttack) / 10;
				break;
			case ABILITY_GUTS:
				if (attacker->status1 & STATUS1_ANY)
					attack = (15 * attack) / 10;
				break;
			case ABILITY_OVERGROW:
				if (type == TYPE_GRASS && attacker->hp <= (attacker->maxHP / 3))
					gBattleMovePower = (15 * gBattleMovePower) / 10;
				break;
			case ABILITY_BLAZE:
				if (type == TYPE_FIRE && attacker->hp <= (attacker->maxHP / 3))
					gBattleMovePower = (15 * gBattleMovePower) / 10;
				break;
			case ABILITY_TORRENT:
				if (type == TYPE_WATER && attacker->hp <= (attacker->maxHP / 3))
					gBattleMovePower = (15 * gBattleMovePower) / 10;
				break;
			case ABILITY_SWARM:
				if (type == TYPE_BUG && attacker->hp <= (attacker->maxHP / 3))
					gBattleMovePower = (15 * gBattleMovePower) / 10;
				break;
			case ABILITY_NORMALIZE:
				if (type == TYPE_NORMAL)
				{
					NORMALIZE_CHECK:
					if (gBattleMoves[move].effect != EFFECT_HIDDEN_POWER && gBattleMoves[move].effect != EFFECT_WEATHER_BALL
					&& gBattleMoves[move].effect != EFFECT_NATURAL_GIFT && gBattleMoves[move].effect != EFFECT_CHANGE_TYPE_ON_ITEM
					&& gBattleMoves[move].effect != EFFECT_TERRAIN_PULSE)
					    gBattleMovePower = (12 * gBattleMovePower) / 10;
				}
				break;
			case ABILITY_IRON_FIST:
				if (gBattleMoves[move].flags & FLAG_IRON_FIST_BOOST)
					gBattleMovePower = (12 * gBattleMovePower) / 10;
				break;
			case ABILITY_RECKLESS:
				if (gBattleMoves[move].flags & FLAG_RECKLESS_BOOST)
					gBattleMovePower = (12 * gBattleMovePower) / 10;
				break;
			case ABILITY_RIVALRY:
				attackerGender = GetGenderFromSpeciesAndPersonality(attacker->species, attacker->personality);
				defenderGender = GetGenderFromSpeciesAndPersonality(defender->species, defender->personality);
		    
				if (attackerGender != MON_GENDERLESS && defenderGender != MON_GENDERLESS)
				{
					if (attackerGender == defenderGender)
						gBattleMovePower += gBattleMovePower / 4;
					else
						gBattleMovePower -= gBattleMovePower / 4;
				}
				break;
			case ABILITY_SLOW_START:
				if (gDisableStructs[battlerIdAtk].slowStartTimer)
					attack /= 2;
				break;
			case ABILITY_SNIPER:
				if (isCrit)
					gBattleMovePower = (gBattleMovePower * 15) / 10;
				break;
			case ABILITY_SOLAR_POWER:
				if (IsBattlerWeatherAffected(battlerIdAtk, WEATHER_SUN_ANY))
					spAttack = (15 * spAttack) / 10;
				break;
			case ABILITY_FLOWER_GIFT:
			    if (IsBattlerWeatherAffected(battlerIdAtk, WEATHER_SUN_ANY))
					attack = (15 * attack) / 10;
				break;
			case ABILITY_TECHNICIAN:
				if (gBattleMovePower <= 60)
					gBattleMovePower = (gBattleMovePower * 15) / 10;
				break;
			case ABILITY_TINTED_LENS:
				if (flags & MOVE_RESULT_NOT_VERY_EFFECTIVE)
					gBattleMovePower *= 2;
				break;
			case ABILITY_SHEER_FORCE:
				if (gBattleMoves[move].flags & FLAG_SHEER_FORCE_BOOST)
					gBattleMovePower = (gBattleMovePower * 12) / 10;
				break;
			case ABILITY_DEFEATIST:
				if (attacker->hp <= (attacker->maxHP / 2))
				{
					spAttack /= 2;
					attack /= 2;
				}
				break;
			case ABILITY_TOXIC_BOOST:
				if (attacker->status1 & STATUS1_PSN_ANY && IS_MOVE_PHYSICAL(move))
					gBattleMovePower = (15 * gBattleMovePower) / 10;
				break;
			case ABILITY_FLARE_BOOST:
				if (attacker->status1 & STATUS1_BURN && IS_MOVE_SPECIAL(move))
					gBattleMovePower = (15 * gBattleMovePower) / 10;
				break;
			case ABILITY_ANALYTIC:
				if (GetBattlerTurnOrderNum(battlerIdAtk) == gBattlersCount - 1)
					gBattleMovePower = (13 * gBattleMovePower) / 10;
				break;
			case ABILITY_SAND_FORCE:
			    if (IsBattlerWeatherAffected(battlerIdAtk, WEATHER_SANDSTORM_ANY) && (type == TYPE_ROCK || type == TYPE_GROUND || type == TYPE_STEEL))
					gBattleMovePower = (13 * gBattleMovePower) / 10;
				break;
			case ABILITY_STRONG_JAW:
			    if (gBattleMoves[move].flags & FLAG_STRONG_JAW_BOOST)
					gBattleMovePower = (15 * gBattleMovePower) / 10;
				break;
			case ABILITY_REFRIGERATE:
			    if (type == TYPE_ICE)
					goto NORMALIZE_CHECK;
				break;
			case ABILITY_MEGA_LAUNCHER:
			    if (gBattleMoves[move].flags & FLAG_MEGA_LAUNCHER_BOOST)
					gBattleMovePower = (15 * gBattleMovePower) / 10;
				break;
			case ABILITY_TOUGH_CLAWS:
			    if (IsMoveMakingContact(battlerIdAtk, move))
					gBattleMovePower = (13 * gBattleMovePower) / 10;
				break;
			case ABILITY_PIXILATE:
			    if (type == TYPE_FAIRY)
					goto NORMALIZE_CHECK;
				break;
			case ABILITY_AERILATE:
			    if (type == TYPE_FLYING)
					goto NORMALIZE_CHECK;
				break;
			case ABILITY_STAKEOUT:
			    if (gDisableStructs[battlerIdDef].isFirstTurn == 2)
					gBattleMovePower *= 2;
				break;
			case ABILITY_WATER_BUBBLE:
			    if (type == TYPE_WATER)
					gBattleMovePower *= 2;
				break;
			case ABILITY_STEELWORKER:
			    if (type == TYPE_STEEL)
				{
					attack += (attack / 2);
					spAttack += (spAttack / 2);
				}
				break;
			case ABILITY_GALVANIZE:
			    if (type == TYPE_ELECTRIC)
					goto NORMALIZE_CHECK;
				break;
			case ABILITY_NEUROFORCE:
			    if (flags & MOVE_RESULT_SUPER_EFFECTIVE)
					gBattleMovePower += gBattleMovePower / 4;
				break;
			case ABILITY_PUNK_ROCK:
			    if (gBattleMoves[move].flags & FLAG_SOUND)
					gBattleMovePower = (13 * gBattleMovePower) / 10;
				break;
			case ABILITY_STEELY_SPIRIT:
			    if (type == TYPE_STEEL)
					gBattleMovePower = (15 * gBattleMovePower) / 10;
				break;
			case ABILITY_TRANSISTOR:
			    if (type == TYPE_ELECTRIC)
				{
					attack += (attack / 2);
					spAttack += (spAttack / 2);
				}
				break;
			case ABILITY_DRAGONS_MAW:
			    if (type == TYPE_DRAGON)
				{
					attack += (attack / 2);
					spAttack += (spAttack / 2);
				}
				break;
			case ABILITY_ROCKY_PAYLOAD:
			    if (type == TYPE_ROCK)
				{
					attack += (attack / 2);
					spAttack += (spAttack / 2);
				}
				break;
			case ABILITY_SHARPNESS:
			    if (gBattleMoves[move].flags & FLAG_SLICING)
					gBattleMovePower = (15 * gBattleMovePower) / 10;
				break;
		}
		// Aura abilities
		if ((ABILITY_ON_FIELD(ABILITY_DARK_AURA) && type == TYPE_DARK) || (ABILITY_ON_FIELD(ABILITY_FAIRY_AURA) && type == TYPE_FAIRY))
		{
			if (ABILITY_ON_FIELD(ABILITY_AURA_BREAK))
				gBattleMovePower = (75 * gBattleMovePower) / 100;
			else
				gBattleMovePower = (4 * gBattleMovePower) / 3;
		}
		// attacker's ally abilities check
		if (IsBattlerAlive(BATTLE_PARTNER(battlerIdAtk)))
		{
			switch (GetBattlerAbility(BATTLE_PARTNER(battlerIdAtk)))
			{
				case ABILITY_FLOWER_GIFT:
				    if (IsBattlerWeatherAffected(battlerIdAtk, WEATHER_SUN_ANY))
						attack = (15 * attack) / 10;
					break;
				case ABILITY_BATTERY:
				    if (IS_MOVE_SPECIAL(move))
						gBattleMovePower = (gBattleMovePower * 13) / 10;
					break;
				case ABILITY_POWER_SPOT:
				    gBattleMovePower = (gBattleMovePower * 13) / 10;
					break;
				case ABILITY_STEELY_SPIRIT:
				    if (type == TYPE_STEEL)
						gBattleMovePower = (15 * gBattleMovePower) / 10;
					break;
			}
		}
		// defender abilities check
		switch (GetBattlerAbility(battlerIdDef))
		{
			case ABILITY_THICK_FAT:
				if (type == TYPE_FIRE || type == TYPE_ICE)
				{
					spAttack /= 2;
					attack /= 2; 
				}
				break;
			case ABILITY_MARVEL_SCALE:
				if (defender->status1 & STATUS1_ANY)
					defense = (15 * defense) / 10;
				break;
			case ABILITY_HEATPROOF:
				if (type == TYPE_FIRE)
				{
					spAttack /= 2;
					attack /= 2;
				}
				break;
			case ABILITY_DRY_SKIN:
				if (type == TYPE_FIRE)
					gBattleMovePower += gBattleMovePower / 4;
				break;
			case ABILITY_FILTER:
			case ABILITY_SOLID_ROCK:
			case ABILITY_PRISM_ARMOR:
				if (flags & MOVE_RESULT_SUPER_EFFECTIVE)
					gBattleMovePower = (gBattleMovePower * 75) / 100;
				break;
			case ABILITY_MULTISCALE:
			case ABILITY_SHADOW_SHIELD:
				if (defender->hp == defender->maxHP)
					gBattleMovePower /= 2;
				break;
			case ABILITY_FLOWER_GIFT:
			    if (IsBattlerWeatherAffected(battlerIdDef, WEATHER_SUN_ANY))
					spDefense = (15 * spDefense) / 10;
				break;
			case ABILITY_FUR_COAT:
			    defense *= 2;
				break;
			case ABILITY_WATER_BUBBLE:
			    if (type == TYPE_FIRE)
					gBattleMovePower /= 2;
				break;
			case ABILITY_FLUFFY:
			    if (IsMoveMakingContact(battlerIdAtk, move))
					gBattleMovePower /= 2;
				if (type == TYPE_FIRE)
					gBattleMovePower *= 2;
				break;
			case ABILITY_PUNK_ROCK:
			    if (gBattleMoves[move].flags & FLAG_SOUND)
					gBattleMovePower /= 2;
				break;
			case ABILITY_ICE_SCALES:
			    if (IS_MOVE_SPECIAL(move))
					gBattleMovePower /= 2;
				break;
		}
		// defender's ally abilities check
		if (IsBattlerAlive(BATTLE_PARTNER(battlerIdDef)))
		{
			switch (GetBattlerAbility(BATTLE_PARTNER(battlerIdDef)))
			{
				case ABILITY_FLOWER_GIFT:
				    if (IsBattlerWeatherAffected(battlerIdDef, WEATHER_SUN_ANY))
						spDefense = (15 * spDefense) / 10;
					break;
				case ABILITY_FRIEND_GUARD:
				    gBattleMovePower = (gBattleMovePower * 75) / 100;
					break;
			}
		}
	}
	if (type == TYPE_ELECTRIC && AbilityBattleEffects(ABILITYEFFECT_FIELD_SPORT, 0, 0, 0xFD, 0))
		gBattleMovePower /= 2;
	if (type == TYPE_FIRE && AbilityBattleEffects(ABILITYEFFECT_FIELD_SPORT, 0, 0, 0xFE, 0))
		gBattleMovePower /= 2;
    
	// sandstorm stats boost
	if (IsBattlerWeatherAffected(battlerIdDef, WEATHER_SANDSTORM_ANY) && IS_BATTLER_OF_TYPE(battlerIdDef, TYPE_ROCK))
	{
		spDefense += spDefense / 2;
		spAttack = (10 * spAttack) / 15;
	}
#if HAIL_BOOST_DEFENSE
	// hail stat boost
	if (IsBattlerWeatherAffected(battlerIdDef, WEATHER_HAIL_ANY) && IS_BATTLER_OF_TYPE(battlerIdDef, TYPE_ICE))
		defense += (defense / 2);
#endif
	// burn attack drop
	if ((attacker->status1 & STATUS1_BURN) && GetBattlerAbility(battlerIdAtk) != ABILITY_GUTS && !isConfusionDmg)
		attack /= 2;
   
	if (IS_MOVE_PHYSICAL(move))
	{
		atkStat = attack;
		atkStage = attacker->statStages[STAT_ATK];
		defStat = defense;
		defStage = defender->statStages[STAT_DEF];
	}
	else
	{
		atkStat = spAttack;
		atkStage = attacker->statStages[STAT_SPATK];
		defStat = spDefense;
		defStage = defender->statStages[STAT_SPDEF];
	}
	if ((isCrit && atkStage < 6) || GetBattlerAbility(battlerIdDef) == ABILITY_UNAWARE)
		atkStage = 6;
	if ((isCrit && defStage > 6) || GetBattlerAbility(battlerIdAtk) == ABILITY_UNAWARE)
		defStage = 6;
	
	atkStat *= gStatStageRatios[atkStage][0];
	atkStat /= gStatStageRatios[atkStage][1];
	
	defStat *= gStatStageRatios[defStage][0];
	defStat /= gStatStageRatios[defStage][1];
	
	damage = atkStat * gBattleMovePower;
	damage *= (2 * attacker->level / 5 + 2);
	damage = (damage / defStat);
	damage /= 50;
	
	if (!isCrit)
	{
		switch (gBattleMoves[move].split)
		{
			case MOVE_PHYSICAL:
			    i = SIDE_STATUS_REFLECT;
			    break;
		        case MOVE_SPECIAL:
			    i = SIDE_STATUS_LIGHTSCREEN;
			    break;
		}
		if (!IS_MOVE_STATUS(move))
		{
			if ((gSideStatuses[GET_BATTLER_SIDE(battlerIdDef)] & i) && GetBattlerAbility(battlerIdAtk) != ABILITY_INFILTRATOR && !isConfusionDmg)
			{
				if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE) == 2)
					damage = 2 * (damage / 3);
				else
					damage /= 2;
			}
		}
	}
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && gBattleMoves[move].target == MOVE_TARGET_BOTH && CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE) > 1) 
		damage -= damage / 4;
	
	if (IsBattlerWeatherAffected(battlerIdAtk, WEATHER_RAIN_ANY))
	{
		switch (type)
		{
			case TYPE_FIRE:
			    damage /= 2;
				break;
			case TYPE_WATER:
			    damage = (15 * damage) / 10;
				break;
		}
	}
	else if (IsBattlerWeatherAffected(battlerIdAtk, WEATHER_SUN_ANY))
	{
		switch (type)
		{
			case TYPE_FIRE:
			    damage = (15 * damage) / 10;
				break;
			case TYPE_WATER:
			    damage /= 2;
				break;
		}
	}
	// any weather except sun weakens solar beam
	if (IsBattlerWeatherAffected(battlerIdAtk, (WEATHER_ANY | ~(WEATHER_SUN_ANY))) && move == MOVE_SOLAR_BEAM)
		damage /= 2;
	
	// flash fire triggered
	if ((gBattleResources->flags->flags[battlerIdAtk] & RESOURCE_FLAG_FLASH_FIRE) && type == TYPE_FIRE)
		damage = (15 * damage) / 10;
	
	// charge up
	if (gStatuses3[battlerIdAtk] & STATUS3_CHARGED_UP && type == TYPE_ELECTRIC)
		damage *= 2;
	
	// helping hand check
	if (gProtectStructs[battlerIdAtk].helpingHand)
		damage = (15 * damage) / 10;
    
	damage += 2;
	
	if (isCrit)
		damage *= 2;
	
	damage *= gBattleScripting.dmgMultiplier;
	
	if (randomFactor)
	{
		damage *= 100 - (Random() % 16);
		damage /= 100;
	}
	
	// check stab
	if (IS_BATTLER_OF_TYPE(battlerIdAtk, type) && !isConfusionDmg)
	{
		if (GetBattlerAbility(battlerIdAtk) == ABILITY_ADAPTABILITY)
			damage *= 2;
		else
			damage = (damage * 15) / 10;
	}
	
	if (damage == 0)
		damage = 1;
	
	return damage;     
}

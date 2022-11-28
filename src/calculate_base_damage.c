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
#include "constants/hoenn_cries.h"
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

static const u16 sIronFistTable[] =
{
    MOVE_COMET_PUNCH,
    MOVE_DIZZY_PUNCH,
    MOVE_DYNAMIC_PUNCH,
    MOVE_FIRE_PUNCH,
    MOVE_FOCUS_PUNCH,
    MOVE_ICE_PUNCH,
    MOVE_MACH_PUNCH,
    MOVE_MEGA_PUNCH,
    MOVE_SHADOW_PUNCH,
    MOVE_SKY_UPPERCUT,
    MOVE_THUNDER_PUNCH,
    TABLE_END,
};

static const u16 sRecklessTable[] =
{
    MOVE_DOUBLE_EDGE,
    MOVE_HI_JUMP_KICK,
    MOVE_JUMP_KICK,
    MOVE_SUBMISSION,
    MOVE_TAKE_DOWN,
    MOVE_VOLT_TACKLE,
    TABLE_END,
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
			if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_ANY)
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

s32 CalculateBaseDamage(struct BattlePokemon *attacker, struct BattlePokemon *defender, u16 move, u16 sideStatus, bool8 isConfusionDmg, u8 battlerIdAtk, u8 battlerIdDef)
{
	u8 type, flags, attackerGender, defenderGender;
	u8 attackerHoldEffect, attackerHoldEffectParam, defenderHoldEffect, defenderHoldEffectParam;
	u16 attack, defense, spAttack, spDefense, atkStat, atkStage, defStat, defStage;
	u32 i;
	s32 j, damage;

	if (isConfusionDmg)
	{
		i = gBattleStruct->dynamicMoveType;
		gBattleStruct->dynamicMoveType = TYPE_NORMAL;
	}
	type = gBattleStruct->dynamicMoveType;
	flags = TypeCalc(move, battlerIdAtk, battlerIdDef, FALSE);
	
	if (isConfusionDmg)
		gBattleStruct->dynamicMoveType = i;
	
	gBattleMovePower = GetModifiedMovePower(battlerIdAtk, battlerIdDef, move);
	
	attack = attacker->attack;
	defense = defender->defense;
	spAttack = attacker->spAttack;
	spDefense = defender->spDefense;

	if (attacker->item == ITEM_ENIGMA_BERRY)
	{
		attackerHoldEffect = gEnigmaBerries[battlerIdAtk].holdEffect;
		attackerHoldEffectParam = gEnigmaBerries[battlerIdAtk].holdEffectParam;
	}
	else
	{
		attackerHoldEffect = GetBattlerItemHoldEffect(battlerIdAtk, TRUE);
		attackerHoldEffectParam = ItemId_GetHoldEffectParam(attacker->item);
	}

	if (defender->item == ITEM_ENIGMA_BERRY)
	{
		defenderHoldEffect = gEnigmaBerries[battlerIdDef].holdEffect;
		defenderHoldEffectParam = gEnigmaBerries[battlerIdDef].holdEffectParam;
	}
	else
	{
		defenderHoldEffect = GetBattlerItemHoldEffect(battlerIdDef, TRUE);
		defenderHoldEffectParam = ItemId_GetHoldEffectParam(defender->item);
	}

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
				if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && gBattleMons[battlerIdAtk ^ BIT_FLANK].hp
				    && (GetBattlerAbility(battlerIdAtk ^ BIT_FLANK) == ABILITY_PLUS || GetBattlerAbility(battlerIdAtk ^ BIT_FLANK) == ABILITY_MINUS))
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
				if (type == TYPE_NORMAL && gBattleMoves[move].effect != EFFECT_HIDDEN_POWER && gBattleMoves[move].effect != EFFECT_WEATHER_BALL)
					gBattleMovePower = (12 * gBattleMovePower) / 10;
				break;
			case ABILITY_IRON_FIST:
				if (IsMoveInTable(sIronFistTable, move))
					gBattleMovePower = (12 * gBattleMovePower) / 10;
				break;
			case ABILITY_RECKLESS:
				if (IsMoveInTable(sRecklessTable, move))
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
				if (gNewBattleStruct.SlowStartTimers[battlerIdAtk] != 0)
					attack /= 2;
				break;
			case ABILITY_SNIPER:
				if (gCritMultiplier == 2)
					gBattleMovePower = (gBattleMovePower * 15) / 10;
				break;
			case ABILITY_SOLAR_POWER:
				if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY)
					spAttack = (15 * spAttack) / 10;
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
				if (IsMoveInTable(gSheerForceBoostedMoves, move))
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
				if (flags & MOVE_RESULT_SUPER_EFFECTIVE)
					gBattleMovePower = (gBattleMovePower * 75) / 100;
				break;
			case ABILITY_MULTISCALE:
				if (defender->hp == defender->maxHP)
					gBattleMovePower /= 2;
				break;
		}
		if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
		{
			// defender's ally abilities check
			if (gBattleMons[battlerIdDef ^ BIT_FLANK].hp != 0)
			{
				switch (GetBattlerAbility(battlerIdDef ^ BIT_FLANK))
				{
					case ABILITY_FRIEND_GUARD:
						gBattleMovePower = (gBattleMovePower * 75) / 100;
						break;
				}
			}
		}
	}
	if (type == TYPE_ELECTRIC && AbilityBattleEffects(ABILITYEFFECT_FIELD_SPORT, 0, 0, 0xFD, 0))
		gBattleMovePower /= 2;
	if (type == TYPE_FIRE && AbilityBattleEffects(ABILITYEFFECT_FIELD_SPORT, 0, 0, 0xFE, 0))
		gBattleMovePower /= 2;
    
	if (WEATHER_HAS_EFFECT)
	{
		// sandstorm stats boost
		if (gBattleWeather & WEATHER_SANDSTORM_ANY && IS_BATTLER_OF_TYPE(battlerIdDef, TYPE_ROCK))
		{
			spDefense += spDefense / 2;
			spAttack = (10 * spAttack) / 15;
		}
	    
		if (gBattleWeather & WEATHER_SUN_ANY && !isConfusionDmg)
		{
			if (GetBattlerAbility(battlerIdAtk) == ABILITY_FLOWER_GIFT)
				attack = (15 * attack) / 10;
			if (GetBattlerAbility(battlerIdDef) == ABILITY_FLOWER_GIFT)
				spDefense = (15 * spDefense) / 10;
			
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
				if (GetBattlerAbility(battlerIdAtk ^ BIT_FLANK) == ABILITY_FLOWER_GIFT && gBattleMons[battlerIdAtk ^ BIT_FLANK].hp != 0)
					attack = (15 * attack) / 10;
				if (GetBattlerAbility(battlerIdDef ^ BIT_FLANK) == ABILITY_FLOWER_GIFT && gBattleMons[battlerIdDef ^ BIT_FLANK].hp != 0)
					spDefense = (15 * spDefense) / 10;
			}
		}
	}
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
	if ((gCritMultiplier == 2 && atkStage < 6) || GetBattlerAbility(battlerIdDef) == ABILITY_UNAWARE)
		atkStage = 6;
	if ((gCritMultiplier == 2 && defStage > 6) || GetBattlerAbility(battlerIdAtk) == ABILITY_UNAWARE)
		defStage = 6;
	
	atkStat *= gStatStageRatios[atkStage][0];
	atkStat /= gStatStageRatios[atkStage][1];
	
	defStat *= gStatStageRatios[defStage][0];
	defStat /= gStatStageRatios[defStage][1];
	
	damage = atkStat * gBattleMovePower;
	damage *= (2 * attacker->level / 5 + 2);
	damage = (damage / defStat);
	damage /= 50;
	
	if (gCritMultiplier == 1)
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
		if ((sideStatus & i) && GetBattlerAbility(battlerIdAtk) != ABILITY_INFILTRATOR)
		{
			if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE) == 2)
				damage = 2 * (damage / 3);
			else
				damage /= 2;
		}
	}
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && gBattleMoves[move].target == MOVE_TARGET_BOTH && CountAliveMonsInBattle(BATTLE_ALIVE_DEF_SIDE) > 1) 
		damage -= damage / 4;
	
	// are effects of weather negated with cloud nine or air lock
	if (WEATHER_HAS_EFFECT2)
	{
		// rain
		if (gBattleWeather & WEATHER_RAIN_ANY)
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
		// sunny
		if (gBattleWeather & WEATHER_SUN_ANY)
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
		if ((gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_SANDSTORM_ANY | WEATHER_HAIL)) && move == MOVE_SOLAR_BEAM)
			damage /= 2;
	}
	// flash fire triggered
	if ((gBattleResources->flags->flags[battlerIdAtk] & RESOURCE_FLAG_FLASH_FIRE) && type == TYPE_FIRE)
		damage = (15 * damage) / 10;
	
	// charge up
	if (gStatuses3[battlerIdAtk] & STATUS3_CHARGED_UP && type == TYPE_ELECTRIC)
		damage *= 2;
	
	// helping hand check
	if (gProtectStructs[battlerIdAtk].helpingHand)
		damage = (15 * damage) / 10;
    
	return damage + 2;     
}

#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_gimmicks.h"
#include "battle_interface.h"
#include "battle_move_effects.h"
#include "battle_util.h"
#include "battle_damage_calc.h"
#include "event_data.h"
#include "item.h"
#include "malloc.h"
#include "random.h"
#include "constants/abilities.h"
#include "constants/battle_string_ids.h"
#include "constants/hold_effects.h"
#include "constants/moves.h"

#define FLAG_CONFUSION_DAMAGE Bit(0)
#define FLAG_AI_DAMAGE_CALC   Bit(1)

struct DamageCalc
{
    u8 attacker;
    u8 defender;
    u16 move;
    u16 atkAbility;
    u16 defAbility;
    u8 moveType;
    u8 moveSplit;
    u8 moveSlot;
    bool8 isCrit;
    u8 atkHoldEffect;
    u8 defHoldEffect;
    u16 atkHoldEffParam;
    u16 defHoldEffParam;
    u32 effectiveness;
    u8 flags;
};

/*
s32 CalculateBaseDamage(u16 move, u8 type, u8 battlerIdAtk, u8 battlerIdDef, bool8 isCrit, bool8 randomFactor)
{
    // attacker items check
        switch (attackerHoldEffect)
        {
            case HOLD_EFFECT_CHOICE_ITEM:
                if (GetActiveGimmick(battlerIdAtk) != GIMMICK_DYNAMAX)
                {
                    if (attackerHoldEffectParam == STAT_ATK)
                        attack = (15 * attack) / 10;
                    else if (attackerHoldEffectParam == STAT_SPATK)
                        spAttack = (15 * spAttack) / 10;
                }
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

static struct DamageCalc *PopulateDamageStruct(u32 attacker, u32 defender, u32 move, u32 moveSlot, u32 moveType, u32 effectiveness, bool32 isCrit, u32 flags)
{
    struct DamageCalc *ctx = AllocZeroed(sizeof(struct DamageCalc));
    ctx->attacker = attacker;
    ctx->defender = defender;
    ctx->atkAbility = GetBattlerAbility(attacker);
    ctx->defAbility = GetBattlerAbility(defender);
    ctx->atkHoldEffect = GetBattlerItemHoldEffect(attacker, TRUE);
    ctx->defHoldEffect = GetBattlerItemHoldEffect(defender, TRUE);
    ctx->atkHoldEffParam = ItemId_GetHoldEffectParam(gBattleMons[attacker].item);
    ctx->defHoldEffParam = ItemId_GetHoldEffectParam(gBattleMons[defender].item);
    ctx->move = move;
    ctx->moveSlot = moveSlot;
    ctx->moveSplit = GetBattleMoveSplit(move);
    ctx->moveType = moveType;
    ctx->effectiveness = effectiveness;
    ctx->isCrit = isCrit;
    ctx->flags = flags;
    return ctx;
}

///////////////////////////
// BASE ATTACK MODIFIERS //
///////////////////////////

static inline u32 CalcBaseAttackStat(struct DamageCalc *ctx, bool32 isConfusionDmg)
{
    u32 attacker = ctx->attacker, defender = ctx->defender;
    u32 statStages, baseAttack;
    u32 move = ctx->move;
    u32 modifier = UQ_4_12(1.0);

    switch (gBattleMoves[move].effect)
    {
        case EFFECT_FOUL_PLAY:
            if (ctx->moveSplit == SPLIT_PHYSICAL)
            {
                baseAttack = gBattleMons[defender].attack;
                statStages = gBattleMons[defender].statStages[STAT_ATK];
            }
            else
            {
                baseAttack = gBattleMons[defender].spAttack;
                statStages = gBattleMons[defender].statStages[STAT_SPATK];
            }
            break;
        default:
            if (ctx->moveSplit == SPLIT_PHYSICAL)
            {
                baseAttack = gBattleMons[attacker].attack;
                statStages = gBattleMons[attacker].statStages[STAT_ATK];
            }
            else
            {
                baseAttack = gBattleMons[attacker].spAttack;
                statStages = gBattleMons[attacker].statStages[STAT_SPATK];
            }
            break;
    }
    
    // Check effects that ignores stat stages
    if ((ctx->isCrit && statStages < DEFAULT_STAT_STAGES) || (!isConfusionDmg && ctx->defAbility == ABILITY_UNAWARE))
        statStages = DEFAULT_STAT_STAGES;
    
    // Calc base attack stat
    APPLY_STAT_MOD(baseAttack, baseAttack, statStages);
    
    // Calculate base attack modifiers
    if (!isConfusionDmg)
    {
        // Check attacker's abilities
        switch (ctx->atkAbility)
        {
            case ABILITY_HUGE_POWER:
            case ABILITY_PURE_POWER:
                if (ctx->moveSplit == SPLIT_PHYSICAL)
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(2.0));
                break;
            case ABILITY_GORILLA_TACTICS:
                if (GetActiveGimmick(attacker) == GIMMICK_DYNAMAX)
                    break;
                // fallthrough
            case ABILITY_HUSTLE:
                if (ctx->moveSplit == SPLIT_PHYSICAL)
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.5));
                break;
            case ABILITY_GUTS:
                if (ctx->moveSplit == SPLIT_PHYSICAL && gBattleMons[attacker].status1.id)
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.5));
                break;
            case ABILITY_SLOW_START:
                if (gDisableStructs[attacker].slowStartTimer)
                {
                    // Halves Sp. Attack of type based Z-Moves
                    if (ctx->moveSplit == SPLIT_PHYSICAL || (ctx->moveSplit == SPLIT_SPECIAL && IsTypeBasedZMove(move)))
                        modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(0.5));
                }
                break;
            case ABILITY_FLOWER_GIFT:
                if (ctx->moveSplit == SPLIT_PHYSICAL && IsBattlerWeatherAffected(attacker, B_WEATHER_SUN_ANY))
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.5));
                break;
            case ABILITY_PLUS:
            case ABILITY_MINUS:
                if (ctx->moveSplit == SPLIT_SPECIAL && IsBattlerAlive(BATTLE_PARTNER(attacker))
                && (GetBattlerAbility(BATTLE_PARTNER(attacker)) == ABILITY_PLUS || GetBattlerAbility(BATTLE_PARTNER(attacker)) == ABILITY_MINUS))
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.5));
                break;
            case ABILITY_SOLAR_POWER:
                if (ctx->moveSplit == SPLIT_SPECIAL && IsBattlerWeatherAffected(attacker, B_WEATHER_SUN_ANY))
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.5));
                break;
            case ABILITY_OVERGROW:
                if (ctx->moveType == TYPE_GRASS && gBattleMons[attacker].hp <= (gBattleMons[attacker].maxHP / 3))
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.5));
                break;
            case ABILITY_BLAZE:
                if (ctx->moveType == TYPE_FIRE && gBattleMons[attacker].hp <= (gBattleMons[attacker].maxHP / 3))
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.5));
                break;
            case ABILITY_TORRENT:
                if (ctx->moveType == TYPE_WATER && gBattleMons[attacker].hp <= (gBattleMons[attacker].maxHP / 3))
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.5));
                break;
            case ABILITY_SWARM:
                if (ctx->moveType == TYPE_BUG && gBattleMons[attacker].hp <= (gBattleMons[attacker].maxHP / 3))
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.5));
                break;
            case ABILITY_DEFEATIST:
                if (gBattleMons[attacker].hp <= (gBattleMons[attacker].maxHP / 2))
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(0.5));
                break;
            case ABILITY_STEELWORKER:
                if (ctx->moveType == TYPE_STEEL)
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.5));
                break;
            case ABILITY_TRANSISTOR:
                if (ctx->moveType == TYPE_ELECTRIC)
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.3));
                break;
            case ABILITY_DRAGONS_MAW:
                if (ctx->moveType == TYPE_DRAGON)
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.5));
                break;
            case ABILITY_ROCKY_PAYLOAD:
                if (ctx->moveType == TYPE_ROCK)
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.5));
                break;
            case ABILITY_FLASH_FIRE:
                if (gDisableStructs[attacker].flashFireBoost && ctx->moveType == TYPE_FIRE)
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.5));
                break;
        }
        
        // Check attacker's ally abilities
        if (IsBattlerAlive(BATTLE_PARTNER(attacker)))
        {
            switch (GetBattlerAbility(BATTLE_PARTNER(attacker)))
            {
                case ABILITY_FLOWER_GIFT:
                    if (ctx->moveSplit == SPLIT_PHYSICAL && IsBattlerWeatherAffected(attacker, B_WEATHER_SUN_ANY))
                        modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.5));
                    break;
            }
        }
        
        // Check defender's abilities
        switch (ctx->defAbility)
        {
            case ABILITY_THICK_FAT:
                if (ctx->moveType == TYPE_FIRE || ctx->moveType == TYPE_ICE)
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(0.5));
                break;
            case ABILITY_PURIFYING_SALT:
                if (ctx->moveType == TYPE_GHOST)
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(0.5));
                break;
        }
        
        // Ruin abilities
        if (ctx->moveSplit == SPLIT_PHYSICAL && ABILITY_ON_FIELD_EXCEPT_BATTLER(attacker, ABILITY_TABLETS_OF_RUIN) && ctx->atkAbility != ABILITY_TABLETS_OF_RUIN)
            modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(0.75));
        
        if (ctx->moveSplit == SPLIT_SPECIAL && ABILITY_ON_FIELD_EXCEPT_BATTLER(attacker, ABILITY_VESSEL_OF_RUIN) && ctx->atkAbility != ABILITY_VESSEL_OF_RUIN)
            modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(0.75));
        
        // Badges modifier
        if (CanReceiveBadgeBoost(attacker, ctx->moveSplit == SPLIT_PHYSICAL ? FLAG_BADGE01_GET : FLAG_BADGE07_GET))
            modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.1));
    }
    return uq4_12_multiply_by_int_half_down(modifier, baseAttack);
}

////////////////////////////
// BASE DEFENSE MODIFIERS //
////////////////////////////

static inline u32 CalcBaseDefenseStat(struct DamageCalc *ctx, bool32 isConfusionDmg)
{
    u32 attacker = ctx->attacker, defender = ctx->defender;
    u32 statStages, baseDefense;
    u32 defense = gBattleMons[defender].defense, spDefense = gBattleMons[defender].spDefense;
    u32 modifier = UQ_4_12(1.0);
    
    if (gFieldStatus & STATUS_FIELD_WONDER_ROOM)
        SWAP(defense, spDefense, statStages);
    
    if (ctx->moveSplit == SPLIT_PHYSICAL || (gBattleMoves[ctx->move].effect == EFFECT_PSYSHOCK && gBattleMoves[ctx->move].argument.generic == SPLIT_PHYSICAL))
    {
        baseDefense = defense;
        statStages = gBattleMons[defender].statStages[STAT_DEF];
    }
    else
    {
        baseDefense = spDefense;
        statStages = gBattleMons[defender].statStages[STAT_SPDEF];
    }
    
    // Check effects that ignores stat stages
    if ((ctx->isCrit && statStages < DEFAULT_STAT_STAGES) || (!isConfusionDmg && (ctx->atkAbility == ABILITY_UNAWARE
    || gBattleMoves[ctx->move].flags.targetStatStagesIgnored)))
        statStages = DEFAULT_STAT_STAGES;
    
    // Calc base defense stat
    APPLY_STAT_MOD(baseDefense, baseDefense, statStages);
    
    // Calculate base defense modifiers
    if (!isConfusionDmg)
    {
        // Check defender's abilities
        switch (ctx->defAbility)
        {
            case ABILITY_MARVEL_SCALE:
                if (ctx->moveSplit == SPLIT_PHYSICAL && gBattleMons[defender].status1.id)
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.5));
                break;
            case ABILITY_FUR_COAT:
                if (ctx->moveSplit == SPLIT_PHYSICAL)
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(2.0));
                break;
            case ABILITY_FLOWER_GIFT:
                if (ctx->moveSplit == SPLIT_SPECIAL && IsBattlerWeatherAffected(defender, B_WEATHER_SUN_ANY))
                    modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.5));
                break;
        }
        
        // Check defender's ally abilities
        if (IsBattlerAlive(BATTLE_PARTNER(defender)))
        {
            switch (GetBattlerAbility(BATTLE_PARTNER(defender)))
            {
                case ABILITY_FLOWER_GIFT:
                    if (ctx->moveSplit == SPLIT_SPECIAL && IsBattlerWeatherAffected(defender, B_WEATHER_SUN_ANY))
                        modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.5));
                    break;
            }
        }
        
        // Ruin abilities
        if (ctx->moveSplit == SPLIT_PHYSICAL && ABILITY_ON_FIELD_EXCEPT_BATTLER(defender, ABILITY_SWORD_OF_RUIN) && ctx->defAbility != ABILITY_SWORD_OF_RUIN)
            modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(0.75));
        
        if (ctx->moveSplit == SPLIT_SPECIAL && ABILITY_ON_FIELD_EXCEPT_BATTLER(defender, ABILITY_BEADS_OF_RUIN) && ctx->defAbility != ABILITY_BEADS_OF_RUIN)
            modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(0.75));
        
        // Badges modifier
        if (CanReceiveBadgeBoost(defender, ctx->moveSplit == SPLIT_PHYSICAL ? FLAG_BADGE05_GET : FLAG_BADGE07_GET))
            modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.1));
        
        // Sandstorm modifier
        if (ctx->moveSplit == SPLIT_SPECIAL && IsBattlerWeatherAffected(defender, B_WEATHER_SANDSTORM) && IsBattlerOfType(defender, TYPE_ROCK))
            modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.5));
        
        // Hail modifier
#if HAIL_BOOST_DEFENSE
        if (ctx->moveSplit == SPLIT_PHYSICAL && IsBattlerWeatherAffected(defender, B_WEATHER_HAIL) && IsBattlerOfType(defender, TYPE_ICE))
            modifier = uq4_12_multiply_half_down(modifier, UQ_4_12(1.5));
#endif
    }
    return uq4_12_multiply_by_int_half_down(modifier, baseDefense);
}

////////////////////////////
// BASE POWER CALCULATION //
////////////////////////////

static const u16 sWeightToPowerTable[] =
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

static const u8 sHeatCrashWeightToPowerTable[] = {40, 40, 60, 80, 100, 120};
static const u8 sTrumpCardPowerTable[] = {200, 80, 60, 50, 40};
static const u8 sSpeedDiffToPowerTable[] = {40, 60, 80, 120, 150};

static inline u32 GetBeatUpPower(u32 battler, bool32 forAI)
{
    struct Pokemon *party = GetBattlerParty(battler);
    
    if (!forAI)
        return (gSpeciesInfo[GetMonData(&party[gBattleScripting.beatUpHitCounter - 1], MON_DATA_SPECIES)].baseAttack / 10) + 5;
    else // Calc max possible damage
    {
        u32 power = 0;
        u32 i, numHits = GetNumBeatUpHits(battler);
        
        for (i = 0; i < numHits; i++)
            power += (gSpeciesInfo[GetMonData(&party[i], MON_DATA_SPECIES)].baseAttack / 10) + 5;
        
        power /= numHits;
        
        return power;
    }
}

static u32 GetMoveBasePower(struct DamageCalc *ctx)
{
    u32 i;
    u32 attacker = ctx->attacker, defender = ctx->defender;
    u32 move = ctx->move;
    u32 basePower;
    bool32 forAI;
    
    // Max Moves
    if (IsMaxMove(move))
        return GetMaxMovePower(gBattleStruct->battlers[attacker].baseUsedMove, move, ctx->moveType);
    
    basePower = gBattleMoves[move].power;
    forAI = (ctx->flags & FLAG_AI_DAMAGE_CALC);
    
    // Moves
    switch (move)
    {
        case MOVE_WATER_SHURIKEN:
            if (!(gBattleMons[attacker].status2 & STATUS2_TRANSFORMED) && gBattleMons[attacker].species == SPECIES_GRENINJA_ASH)
                basePower = 20;
            break;
    }
    
    // Move effects
    switch (gBattleMoves[move].effect)
    {
        case EFFECT_DAMAGE_BASED_TARGET_WEIGHT:
        {
            u32 weight = GetBattlerWeight(defender);
            
            for (i = 0; sWeightToPowerTable[i] != 0xFFFF; i += 2)
            {
                if (sWeightToPowerTable[i] > weight)
                    break;
            }
            
            if (sWeightToPowerTable[i] != 0xFFFF)
                basePower = sWeightToPowerTable[i + 1];
            else
                basePower = 120;
            break;
        }
        case EFFECT_TRIPLE_KICK:
            if (forAI)
            {
                for (i = 0; i < gBattleMoves[move].strikeCount; i++)
                    basePower += gBattleMoves[move].argument.generic; // Get max possible dmg
            }
            else
                basePower += gBattleScripting.tripleKickPower;
            break;
        case EFFECT_FLAIL:
        {
            u32 hpFracion = GetScaledHPFraction(gBattleMons[attacker].hp, gBattleMons[attacker].maxHP, 48);
            
            for (i = 0; i < sizeof(sFlailHpScaleToPowerTable); i += 2)
            {
                if (hpFracion <= sFlailHpScaleToPowerTable[i])
                    break;
            }
            basePower = sFlailHpScaleToPowerTable[i + 1];
            break;
        }
        case EFFECT_RETURN:
            basePower = 10 * (gBattleMons[attacker].friendship) / 25;
            break;
        case EFFECT_PRESENT:
            basePower = forAI ? 120 : gBattleStruct->presentBasePower;
            break;
        case EFFECT_FRUSTRATION:
            basePower = 10 * (255 - gBattleMons[attacker].friendship) / 25;
            break;
        case EFFECT_MAGNITUDE:
            basePower = forAI ? 50 : gBattleStruct->magnitudeBasePower;
            break;
        case EFFECT_BEAT_UP:
            basePower = GetBeatUpPower(attacker, forAI);
            break;
        case EFFECT_ERUPTION:
            basePower = (gBattleMons[attacker].hp * basePower) / gBattleMons[attacker].maxHP;
            break;
        case EFFECT_SPIT_UP:
            basePower = 100 * gDisableStructs[attacker].stockpileCounter;
            break;
        case EFFECT_GYRO_BALL:
            basePower = ((25 * GetBattlerTotalSpeed(defender)) / GetBattlerTotalSpeed(attacker)) + 1;
            
            if (basePower > 150)
                basePower = 150;
            break;
        case EFFECT_NATURAL_GIFT:
            basePower = gNaturalGiftTable[ITEM_TO_BERRY(gBattleMons[attacker].item)].power;
            break;
        case EFFECT_TRUMP_CARD:
        {
            u32 pp = gBattleMons[attacker].pp[ctx->moveSlot];
            
            if (pp >= ARRAY_COUNT(sTrumpCardPowerTable))
                basePower = sTrumpCardPowerTable[ARRAY_COUNT(sTrumpCardPowerTable) - 1];
            else
                basePower = sTrumpCardPowerTable[pp];
            break;
        }
        case EFFECT_WRING_OUT:
            basePower = 120 * (gBattleMons[defender].hp / gBattleMons[defender].maxHP);
            break;
        case EFFECT_PUNISHMENT:
            basePower = 60 + (CountBattlerStatIncreases(defender, TRUE) * 20);
            
            if (basePower > 200)
                basePower = 200;
            break;
        case EFFECT_HEAT_CRASH:
        {
            u32 weight = GetBattlerWeight(attacker) / GetBattlerWeight(defender);
            
            if (weight >= ARRAY_COUNT(sHeatCrashWeightToPowerTable))
                basePower = sHeatCrashWeightToPowerTable[ARRAY_COUNT(sHeatCrashWeightToPowerTable) - 1];
            else
                basePower = sHeatCrashWeightToPowerTable[weight];
            break;
        }
        case EFFECT_ELECTRO_BALL:
        {
            u32 speed = GetBattlerTotalSpeed(attacker) / GetBattlerTotalSpeed(defender);
            
            if (speed >= ARRAY_COUNT(sSpeedDiffToPowerTable))
                basePower = sSpeedDiffToPowerTable[ARRAY_COUNT(sSpeedDiffToPowerTable) - 1];
            else
                basePower = sSpeedDiffToPowerTable[speed];
            break;
        }
        case EFFECT_ECHOED_VOICE:
            basePower = min(200, basePower + (40 * gBattleStruct->echoedVoiceDmgScale));
            break;
        case EFFECT_STORED_POWER:
            basePower += (CountBattlerStatIncreases(attacker, TRUE) * 20);
            break;
    }
    
    if (basePower == 0)
        basePower = 1;

    return basePower;
}

//////////////////////////
// BASE POWER MODIFIERS //
//////////////////////////

static inline u32 GetSupremeOverlordModifier(u32 battlerId)
{
    return UQ_4_12(1.0) + (PercentToUQ4_12(gDisableStructs[battlerId].supremeOverlordBoost * 10));
}

static inline u32 CalcMoveBasePowerModifiers(struct DamageCalc *ctx, u32 basePower)
{
    u32 i, finalBasePower;
    u32 attacker = ctx->attacker, defender = ctx->defender;
    u32 move = ctx->move;
    u32 moveEffect = gBattleMoves[move].effect;
    u32 modifier = UQ_4_12(1.0);
    
    switch (moveEffect)
    {
        case EFFECT_SKIP_CHARGING_IN_WEATHER:
            if (IsBattlerWeatherAffected(attacker, (B_WEATHER_ANY & ~(gBattleMoves[move].argument.twoTurns.statusOrweather | B_WEATHER_STRONG_WINDS))))
                modifier = uq4_12_mul(modifier, UQ_4_12(0.5));
            break;
        case EFFECT_ROLLOUT:
            for (i = 1; i < (5 - gDisableStructs[attacker].rolloutTimer); i++)
                modifier = uq4_12_mul(modifier, UQ_4_12(2.0));
            
            if (gBattleMons[attacker].status2 & STATUS2_DEFENSE_CURL)
                modifier = uq4_12_mul(modifier, UQ_4_12(2.0));
            break;
        case EFFECT_FURY_CUTTER:
            for (i = 1; i < gDisableStructs[attacker].furyCutterCounter; i++)
                modifier = uq4_12_mul(modifier, UQ_4_12(2.0));
            break;
        case EFFECT_PURSUIT:
            if (gBattleStruct->pursuitSwitchDmg)
                modifier = uq4_12_mul(modifier, UQ_4_12(2.0));
            break;
        case EFFECT_FACADE:
            if (gBattleMons[attacker].status1.id == STATUS1_PARALYSIS || gBattleMons[attacker].status1.id == STATUS1_BURN || gBattleMons[attacker].status1.id == STATUS1_POISON
            || gBattleMons[attacker].status1.id == STATUS1_TOXIC_POISON)
                modifier = uq4_12_mul(modifier, UQ_4_12(2.0));
            break;
        case EFFECT_CURE_STATUS1_FROM_ARG:
            if (!SubstituteBlocksMove(attacker, defender, move) && (gBattleMons[defender].status1.id == gBattleMoves[move].argument.generic
            || (gBattleMoves[move].argument.generic == STATUS1_SLEEP && ctx->defAbility == ABILITY_COMATOSE)))
                modifier = uq4_12_mul(modifier, UQ_4_12(2.0));
            break;
        case EFFECT_REVENGE:
            if ((gProtectStructs[attacker].physicalDmg && gProtectStructs[attacker].physicalBattlerId == defender)
            || (gProtectStructs[attacker].specialDmg && gProtectStructs[attacker].specialBattlerId == defender))
                modifier = uq4_12_mul(modifier, UQ_4_12(2.0));
            break;
        case EFFECT_KNOCK_OFF:
            if (gBattleMons[defender].item && CanBattlerGetOrLoseItem(defender, gBattleMons[defender].item))
                modifier = uq4_12_mul(modifier, UQ_4_12(1.5));
            break;
        case EFFECT_WEATHER_BALL:
            if (IsBattlerWeatherAffected(attacker, B_WEATHER_ANY & ~(B_WEATHER_STRONG_WINDS)))
                modifier = uq4_12_mul(modifier, UQ_4_12(2.0));
            break;
        case EFFECT_BRINE:
            if (gBattleMons[defender].hp <= gBattleMons[defender].maxHP / 2)
                modifier = uq4_12_mul(modifier, UQ_4_12(2.0));
            break;
        case EFFECT_PAYBACK:
            if (gBattleStruct->battlers[defender].chosenAction == B_ACTION_USE_ITEM || GetBattlerTurnOrderNum(attacker) > GetBattlerTurnOrderNum(defender))
                modifier = uq4_12_mul(modifier, UQ_4_12(2.0));
            break;
        case EFFECT_ASSURANCE:
            if (gProtectStructs[defender].physicalDmg || gProtectStructs[defender].specialDmg || gProtectStructs[defender].confusionSelfDmg)
                modifier = uq4_12_mul(modifier, UQ_4_12(2.0));
            break;
        case EFFECT_VENOSHOCK:
            if (gBattleMons[defender].status1.id == STATUS1_POISON || gBattleMons[defender].status1.id == STATUS1_TOXIC_POISON)
                modifier = uq4_12_mul(modifier, UQ_4_12(2.0));
            break;
        case EFFECT_ROUND:
            if (gBattleStruct->roundUsed)
                modifier = uq4_12_mul(modifier, UQ_4_12(2.0));
            break;
        case EFFECT_HEX:
            if (gBattleMons[defender].status1.id || ctx->defAbility == ABILITY_COMATOSE)
                modifier = uq4_12_mul(modifier, UQ_4_12(2.0));
            break;
        case EFFECT_ACROBATICS:
            if (!gBattleMons[attacker].item)
                modifier = uq4_12_mul(modifier, UQ_4_12(2.0));
            break;
    }
    
    // Attacker's abilities
    switch (ctx->atkAbility)
    {
        case ABILITY_NORMALIZE:
            if (ctx->moveType == TYPE_NORMAL)
            {
                NORMALIZE_CHECK:
                if (!IsMaxMove(move) && !GET_MOVEEFFECT_TABLE(moveEffect).normalizeUnaffected)
                    modifier = uq4_12_mul(modifier, UQ_4_12(1.2));
            }
            break;
        case ABILITY_REFRIGERATE:
            if (ctx->moveType == TYPE_ICE)
                goto NORMALIZE_CHECK;
            break;
        case ABILITY_PIXILATE:
            if (ctx->moveType == TYPE_FAIRY)
                goto NORMALIZE_CHECK;
            break;
        case ABILITY_AERILATE:
            if (ctx->moveType == TYPE_FLYING)
                goto NORMALIZE_CHECK;
            break;
        case ABILITY_GALVANIZE:
            if (ctx->moveType == TYPE_ELECTRIC)
                goto NORMALIZE_CHECK;
            break;
        case ABILITY_IRON_FIST:
            if (gBattleMoves[move].flags.punchMove)
                modifier = uq4_12_mul(modifier, UQ_4_12(1.2));
            break;
        case ABILITY_RECKLESS:
            if ((move != MOVE_STRUGGLE && gBattleMoves[move].recoilDivisor) || moveEffect == EFFECT_RECOIL_IF_MISS)
                modifier = uq4_12_mul(modifier, UQ_4_12(1.2));
            break;
        case ABILITY_RIVALRY:
        {
            u32 atkGender = GetBattlerGender(attacker), defGender = GetBattlerGender(defender);
            
            if (atkGender != MON_GENDERLESS && defGender != MON_GENDERLESS)
            {
                if (atkGender == defGender)
                    modifier = uq4_12_mul(modifier, UQ_4_12(1.25));
                else
                    modifier = uq4_12_mul(modifier, UQ_4_12(0.75));
            }
            break;
        }
        case ABILITY_TECHNICIAN:
            if (basePower <= 60)
                modifier = uq4_12_mul(modifier, UQ_4_12(1.5));
            break;
        case ABILITY_SHEER_FORCE:
            if (MoveIsAffectedBySheerForce(move))
                modifier = uq4_12_mul(modifier, UQ_4_12(1.3));
            break;
        case ABILITY_TOXIC_BOOST:
            if ((gBattleMons[attacker].status1.id == STATUS1_POISON || gBattleMons[attacker].status1.id == STATUS1_TOXIC_POISON) && ctx->moveSplit == SPLIT_PHYSICAL)
                modifier = uq4_12_mul(modifier, UQ_4_12(1.5));
            break;
        case ABILITY_FLARE_BOOST:
            if (gBattleMons[attacker].status1.id == STATUS1_BURN && ctx->moveSplit == SPLIT_SPECIAL)
                modifier = uq4_12_mul(modifier, UQ_4_12(1.5));
            break;
        case ABILITY_ANALYTIC:
            if (GetBattlerTurnOrderNum(attacker) == gBattlersCount - 1)
                modifier = uq4_12_mul(modifier, UQ_4_12(1.3));
            break;
        case ABILITY_SAND_FORCE:
            if (IsBattlerWeatherAffected(attacker, B_WEATHER_SANDSTORM) && (ctx->moveType == TYPE_ROCK || ctx->moveType == TYPE_GROUND
            || ctx->moveType == TYPE_STEEL))
                modifier = uq4_12_mul(modifier, UQ_4_12(1.3));
            break;
        case ABILITY_STRONG_JAW:
            if (gBattleMoves[move].flags.bitingMove)
                modifier = uq4_12_mul(modifier, UQ_4_12(1.5));
            break;
        case ABILITY_MEGA_LAUNCHER:
            if (gBattleMoves[move].flags.pulseMove)
                modifier = uq4_12_mul(modifier, UQ_4_12(1.5));
            break;
        case ABILITY_TOUGH_CLAWS:
            if (IsMoveMakingContact(attacker, move))
                modifier = uq4_12_mul(modifier, UQ_4_12(1.3));
            break;
        case ABILITY_STAKEOUT:
            if (gDisableStructs[defender].isFirstTurn == 2)
                modifier = uq4_12_mul(modifier, UQ_4_12(2.0));
            break;
        case ABILITY_WATER_BUBBLE:
            if (ctx->moveType == TYPE_WATER)
                modifier = uq4_12_mul(modifier, UQ_4_12(2.0));
            break;
        case ABILITY_PUNK_ROCK:
            if (gBattleMoves[move].flags.soundMove)
                modifier = uq4_12_mul(modifier, UQ_4_12(1.3));
            break;
        case ABILITY_STEELY_SPIRIT:
            if (ctx->moveType == TYPE_STEEL)
                modifier = uq4_12_mul(modifier, UQ_4_12(1.5));
            break;
        case ABILITY_SHARPNESS:
            if (gBattleMoves[move].flags.slicingMove)
                modifier = uq4_12_mul(modifier, UQ_4_12(1.5));
            break;
        case ABILITY_SUPREME_OVERLORD:
            modifier = uq4_12_mul(modifier, GetSupremeOverlordModifier(attacker));
            break;
    }
    
    // Attacker's ally abilities
    if (IsBattlerAlive(BATTLE_PARTNER(attacker)))
    {
        switch (GetBattlerAbility(BATTLE_PARTNER(attacker)))
        {
            case ABILITY_BATTERY:
                if (ctx->moveSplit == SPLIT_SPECIAL)
                    modifier = uq4_12_mul(modifier, UQ_4_12(1.3));
                break;
            case ABILITY_POWER_SPOT:
                modifier = uq4_12_mul(modifier, UQ_4_12(1.3));
                break;
            case ABILITY_STEELY_SPIRIT:
                if (ctx->moveType == TYPE_STEEL)
                    modifier = uq4_12_mul(modifier, UQ_4_12(1.5));
                break;
        }
    }
    
    // Aura abilities
    if ((ABILITY_ON_FIELD(ABILITY_DARK_AURA) && ctx->moveType == TYPE_DARK) || (ABILITY_ON_FIELD(ABILITY_FAIRY_AURA) && ctx->moveType == TYPE_FAIRY))
    {
        if (!ABILITY_ON_FIELD(ABILITY_AURA_BREAK))
            modifier = uq4_12_mul(modifier, UQ_4_12(1.33));
        else
            modifier = uq4_12_mul(modifier, UQ_4_12(0.75));
    }
    
    // Various effects
    
    // Charge
    if ((gStatuses3[attacker] & STATUS3_CHARGED_UP) && ctx->moveType == TYPE_ELECTRIC)
        modifier = uq4_12_mul(modifier, UQ_4_12(2.0));
    
    // Helping Hand
    for (i = 0; i < gProtectStructs[attacker].helpingHandUses; i++)
        modifier = uq4_12_mul(modifier, UQ_4_12(1.5));
    
    // Water/Mud Sport
    if (((gFieldStatus & STATUS_FIELD_WATERSPORT) && ctx->moveType == TYPE_FIRE) || ((gFieldStatus & STATUS_FIELD_MUDSPORT) && ctx->moveType == TYPE_ELECTRIC))
        modifier = uq4_12_mul(modifier, UQ_4_12(0.33));
    
    // Me First
    if (gBattleStruct->meFirstBoost)
        modifier = uq4_12_mul(modifier, UQ_4_12(1.5));
    
    finalBasePower = uq4_12_multiply_by_int_half_down(modifier, basePower);
    
    // Terastallization boosts weak, non-priority, non-multi hit moves after modifiers to 60 BP.
    if (finalBasePower < 60 && GetActiveGimmick(attacker) == GIMMICK_TERA && gBattleMoves[move].power > 1 && !(gBattleMoves[move].strikeCount > 1)
    && GetMovePriority(attacker, move) == 0 && moveEffect != EFFECT_MULTI_HIT && moveEffect != EFFECT_ERUPTION && moveEffect != EFFECT_WRING_OUT)
    {
        u32 teraType = GetBattlerTeraType(attacker);
        
        if (ctx->moveType == teraType || (teraType == TYPE_STELLAR && IsTypeStellarBoosted(attacker, ctx->moveType)))
            finalBasePower = 60;
    }
    return finalBasePower;
}

////////////////////////
// DAMAGE CALCULATION //
////////////////////////

#define DAMAGE_APPLY_MODIFIER(modifier) do {                     \
    damage = uq4_12_multiply_by_int_half_down(modifier, damage); \
} while (0)

#define DAMAGE_MULTIPLY_MODIFIER(mod) do {               \
    modifier = uq4_12_multiply_half_down(mod, modifier); \
} while (0)

static inline s32 DoDamageFormula(u32 power, u32 userAttack, u32 level, u32 targetDefense)
{
    return power * userAttack * (2 * level / 5 + 2) / targetDefense / 50 + 2;
}

static inline u32 GetTargetDamageModifier(struct DamageCalc *ctx)
{
    if (!DoesSpreadMoveStrikesOnlyOnce(ctx->attacker, ctx->defender, ctx->move, FALSE))
        return UQ_4_12(0.75);
    else
        return UQ_4_12(1.0);
}

static inline u32 GetParentalBondDamageModifier(struct DamageCalc *ctx)
{
    return gSpecialStatuses[ctx->attacker].parentalBondState == PARENTAL_BOND_2ND_HIT ? UQ_4_12(0.25) : UQ_4_12(1.0);
}

static inline u32 GetWeatherDamageModifier(struct DamageCalc *ctx)
{
    if (IsBattlerWeatherAffected(ctx->attacker, B_WEATHER_SUN_ANY))
    {
        if (ctx->moveType == TYPE_FIRE)
            return UQ_4_12(1.5);
        else if (ctx->moveType == TYPE_WATER)
            return UQ_4_12(0.5);
    }
    else if (IsBattlerWeatherAffected(ctx->attacker, B_WEATHER_RAIN_ANY))
    {
        if (ctx->moveType == TYPE_WATER)
            return UQ_4_12(1.5);
        else if (ctx->moveType == TYPE_FIRE)
            return UQ_4_12(0.5);
    }
    return UQ_4_12(1.0);
}

static inline u32 GetCriticalHitDamageModifier(struct DamageCalc *ctx)
{
    return ctx->isCrit ? UQ_4_12(1.5) : UQ_4_12(1.0);
}

static inline u32 GetSameTypeAttackBonusDamageModifier(struct DamageCalc *ctx)
{
    if (IsBattlerOfType(ctx->attacker, ctx->moveType))
    {
        if (ctx->atkAbility == ABILITY_ADAPTABILITY)
            return UQ_4_12(2.0);
        else
            return UQ_4_12(1.5);
    }
    return UQ_4_12(1.0);
}

static inline u32 GetTeraTypeDamageModifier(struct DamageCalc *ctx)
{
    u32 teraType = GetBattlerTeraType(ctx->attacker);
    bool32 isOfBaseType = IsBattlerOfBaseType(ctx->attacker, ctx->moveType);
    
    // Stellar type.
    if (teraType == TYPE_STELLAR)
    {
        bool32 shouldBoost = IsTypeStellarBoosted(ctx->attacker, ctx->moveType);
        
        if (isOfBaseType)
        {
            if (shouldBoost)
                return UQ_4_12(2.0);
            else
                return UQ_4_12(1.5);
        }
        else if (shouldBoost)
            return UQ_4_12(1.2);
        else
            return UQ_4_12(1.0);
    }
    
    // Base and Tera type.
    if (isOfBaseType && ctx->moveType == teraType)
    {
        if (ctx->atkAbility == ABILITY_ADAPTABILITY)
            return UQ_4_12(2.25);
        else
            return UQ_4_12(2.0);
    }
    // Base or Tera type only.
    else if ((!isOfBaseType && ctx->moveType == teraType) || (isOfBaseType && ctx->moveType != teraType))
    {
        if (ctx->atkAbility == ABILITY_ADAPTABILITY)
            return UQ_4_12(2.0);
        else
            return UQ_4_12(1.5);
    }
    return UQ_4_12(1.0);
}

static inline u32 GetBurnDamageModifier(struct DamageCalc *ctx)
{
    if (gBattleMons[ctx->attacker].status1.id == STATUS1_BURN && ctx->moveSplit == SPLIT_PHYSICAL && gBattleMoves[ctx->move].effect != EFFECT_FACADE
    && ctx->atkAbility != ABILITY_GUTS)
        return UQ_4_12(0.5);
    else
        return UQ_4_12(1.0);
}

static inline u32 GetMinimizeDamageModifier(struct DamageCalc *ctx)
{
    if (gBattleMoves[ctx->move].flags.dmgMinimize && (gStatuses3[ctx->defender] & STATUS3_MINIMIZED))
        return UQ_4_12(2.0);
    else
        return UQ_4_12(1.0);
}

static inline u32 GetSemiInvulnerableDamageModifier(struct DamageCalc *ctx)
{
    if (gBattleMoves[ctx->move].flags.hitUnderground && (gStatuses3[ctx->defender] & STATUS3_UNDERGROUND))
        return UQ_4_12(2.0);
    else if (gBattleMoves[ctx->move].flags.hitUnderwater && (gStatuses3[ctx->defender] & STATUS3_UNDERWATER))
        return UQ_4_12(2.0);
    else if (gBattleMoves[ctx->move].flags.hitInAirDoubleDmg && (gStatuses3[ctx->defender] & (STATUS3_ON_AIR | STATUS3_SKY_DROPPED)))
        return UQ_4_12(2.0);
    else
        return UQ_4_12(1.0);
}

static inline u32 GetScreenDamageModifier(struct DamageCalc *ctx)
{
    if (IsDoubleBattleForBattler(ctx->defender))
        return UQ_4_12(0.667);
    else
        return UQ_4_12(0.5);
}

static inline u32 GetScreensDamageModifier(struct DamageCalc *ctx)
{
    u32 modifier = UQ_4_12(1.0);
    
    if (!ctx->isCrit && ctx->atkAbility != ABILITY_INFILTRATOR && ctx->moveSplit != SPLIT_STATUS)
    {
        u32 side = GetBattlerSide(ctx->defender);
        
        switch (ctx->moveSplit)
        {
            case SPLIT_PHYSICAL:
                if (gSideStatuses[side] & SIDE_STATUS_REFLECT)
                    DAMAGE_MULTIPLY_MODIFIER(GetScreenDamageModifier(ctx));
                break;
            case SPLIT_SPECIAL:
                if (gSideStatuses[side] & SIDE_STATUS_LIGHTSCREEN)
                    DAMAGE_MULTIPLY_MODIFIER(GetScreenDamageModifier(ctx));
                break;
        }
        
        if (gSideStatuses[side] & SIDE_STATUS_AURORA_VEIL)
            DAMAGE_MULTIPLY_MODIFIER(GetScreenDamageModifier(ctx));
    }
    return modifier;
}

static inline u32 GetAttackerAbilityDamageModifier(struct DamageCalc *ctx)
{
    switch (ctx->atkAbility)
    {
        case ABILITY_NEUROFORCE:
            if (ctx->effectiveness >= TYPE_MUL_SUPER_EFFECTIVE)
                return UQ_4_12(1.25);
            break;
        case ABILITY_SNIPER:
            if (ctx->isCrit)
                return UQ_4_12(1.5);
            break;
        case ABILITY_TINTED_LENS:
            if (ctx->effectiveness > TYPE_MUL_NO_EFFECT && ctx->effectiveness <= TYPE_MUL_NOT_EFFECTIVE)
                return UQ_4_12(2.0);
            break;
    }
    return UQ_4_12(1.0);
}

static inline u32 GetDefenderAbilityDamageModifier(struct DamageCalc *ctx)
{
    switch (ctx->defAbility)
    {
        case ABILITY_MULTISCALE:
        case ABILITY_SHADOW_SHIELD:
            if (BATTLER_MAX_HP(ctx->defender))
                return UQ_4_12(0.5);
            break;
        case ABILITY_FLUFFY:
        {
            bool32 makesContact = IsMoveMakingContact(ctx->attacker, ctx->move);
            
            if (!makesContact && ctx->moveType == TYPE_FIRE)
                return UQ_4_12(2.0);
            if (makesContact && ctx->moveType != TYPE_FIRE)
                return UQ_4_12(0.5);
            break;
        }
        case ABILITY_PUNK_ROCK:
            if (gBattleMoves[ctx->move].flags.soundMove)
                return UQ_4_12(0.5);
            break;
        case ABILITY_ICE_SCALES:
            if (ctx->moveSplit == SPLIT_SPECIAL)
                return UQ_4_12(0.5);
            break;
        case ABILITY_FILTER:
        case ABILITY_SOLID_ROCK:
        case ABILITY_PRISM_ARMOR:
            if (ctx->effectiveness >= TYPE_MUL_SUPER_EFFECTIVE)
                return UQ_4_12(0.75);
            break;
        case ABILITY_HEATPROOF:
        case ABILITY_WATER_BUBBLE:
            if (ctx->moveType == TYPE_FIRE)
                return UQ_4_12(0.5);
            break;
        case ABILITY_DRY_SKIN:
            if (ctx->moveType == TYPE_FIRE)
                return UQ_4_12(1.25);
            break;
    }
    return UQ_4_12(1.0);
}

static inline u32 GetDefenderPartnerAbilityDamageModifier(struct DamageCalc *ctx)
{
    if (IsBattlerAlive(BATTLE_PARTNER(ctx->defender)))
    {
        switch (GetBattlerAbility(BATTLE_PARTNER(ctx->defender)))
        {
            case ABILITY_FRIEND_GUARD:
                return UQ_4_12(0.75);
        }
    }
    return UQ_4_12(1.0);
}

static inline u32 GetAttackerItemDamageModifier(struct DamageCalc *ctx)
{
    return UQ_4_12(1.0);
}

static inline u32 GetDefenderItemDamageModifier(struct DamageCalc *ctx)
{
    return UQ_4_12(1.0);
}

static inline u32 GetZMaxMoveAgainstProtectDamageModifier(struct DamageCalc *ctx)
{
    if (IsZMove(ctx->move) || IsMaxMove(ctx->move))
    {
        if (IsBattlerProtected(ctx->attacker, ctx->defender, ctx->move, TRUE))
            return UQ_4_12(0.25);
    }
    return UQ_4_12(1.0);
}

static inline u32 GetOthersDamageModifier(struct DamageCalc *ctx)
{
    u32 modifier = UQ_4_12(1.0);
    
    // TODO: Dynamax Cannon modifier
    DAMAGE_MULTIPLY_MODIFIER(GetMinimizeDamageModifier(ctx));
    DAMAGE_MULTIPLY_MODIFIER(GetSemiInvulnerableDamageModifier(ctx));
    DAMAGE_MULTIPLY_MODIFIER(GetScreensDamageModifier(ctx));
    // TODO: Collision Course and Electro Drift modifiers
    DAMAGE_MULTIPLY_MODIFIER(GetAttackerAbilityDamageModifier(ctx));
    DAMAGE_MULTIPLY_MODIFIER(GetDefenderAbilityDamageModifier(ctx));
    DAMAGE_MULTIPLY_MODIFIER(GetDefenderPartnerAbilityDamageModifier(ctx));
    DAMAGE_MULTIPLY_MODIFIER(GetAttackerItemDamageModifier(ctx));
    DAMAGE_MULTIPLY_MODIFIER(GetDefenderItemDamageModifier(ctx));
    DAMAGE_MULTIPLY_MODIFIER(GetZMaxMoveAgainstProtectDamageModifier(ctx));
    
    return modifier;
}

static inline s32 CalculateDamageInternal(struct DamageCalc *ctx, u32 basePower)
{
    s32 damage;
    bool32 isConfusionDmg = (ctx->flags & FLAG_CONFUSION_DAMAGE);

    if (basePower == 0)
        basePower = CalcMoveBasePowerModifiers(ctx, GetMoveBasePower(ctx));
    
    gBattleMovePower = basePower;
    
    damage = DoDamageFormula(basePower, CalcBaseAttackStat(ctx, isConfusionDmg), gBattleMons[ctx->attacker].level, CalcBaseDefenseStat(ctx, isConfusionDmg));
    
    if (!isConfusionDmg)
    {
        DAMAGE_APPLY_MODIFIER(GetTargetDamageModifier(ctx));
        DAMAGE_APPLY_MODIFIER(GetParentalBondDamageModifier(ctx));
        DAMAGE_APPLY_MODIFIER(GetWeatherDamageModifier(ctx));
        // TODO: Glaive Rush modifier
        DAMAGE_APPLY_MODIFIER(GetCriticalHitDamageModifier(ctx));
    }

    // Random factor modifier
    damage *= 100 - RandomUniform(RNG_DAMAGE_MODIFIER, 0, 15);
    damage /= 100;
    
    if (!isConfusionDmg)
    {
        if (GetActiveGimmick(ctx->attacker) == GIMMICK_TERA)
            DAMAGE_APPLY_MODIFIER(GetTeraTypeDamageModifier(ctx));
        else
            DAMAGE_APPLY_MODIFIER(GetSameTypeAttackBonusDamageModifier(ctx));
        
        DAMAGE_APPLY_MODIFIER(ctx->effectiveness);
        DAMAGE_APPLY_MODIFIER(GetBurnDamageModifier(ctx));
        DAMAGE_APPLY_MODIFIER(GetOthersDamageModifier(ctx));
    }
    return damage;
}

static s32 CalculateDamage(struct DamageCalc *ctx, u32 basePower)
{
    s32 damage;

    switch (gBattleMoves[ctx->move].effect)
    {
        case EFFECT_FIXED_DAMAGE:
            damage = gBattleMoves[ctx->move].argument.generic;
            break;
        case EFFECT_USER_LEVEL_TO_DAMAGE:
            damage = gBattleMons[ctx->attacker].level;
            break;
        case EFFECT_PSYWAVE:
            damage = (gBattleMons[ctx->attacker].level * RandomUniform(RNG_PSYWAVE, 50, 150)) / 100;
            break;
        case EFFECT_SUPER_FANG:
            damage = GetNonDynamaxHP(ctx->defender) / 2;
            break;
        case EFFECT_ENDEAVOR:
            damage = GetNonDynamaxHP(ctx->defender) - gBattleMons[ctx->attacker].hp;
            break;
        default:
            damage = CalculateDamageInternal(ctx, basePower);
            break;
    }

    if (damage == 0)
        damage = 1;
    
    FREE_AND_SET_NULL(ctx);
    
    return damage;
}

s32 CalculateMoveDamage(u32 move, u32 moveType, u32 attacker, u32 defender, bool32 isCrit)
{
    u32 multiplier = TypeCalc(move, moveType, attacker, defender, TRUE, FALSE, &gMoveResultFlags);
    return CalculateDamage(PopulateDamageStruct(attacker, defender, move, gBattleStruct->battlers[attacker].chosenMovePosition, moveType, multiplier, isCrit, 0), 0);
}

s32 CalculateConfusionDamage(void)
{
    u32 battler = gBattlerTarget = gBattlerAttacker;
    return CalculateDamage(PopulateDamageStruct(battler, battler, MOVE_NONE, 0, TYPE_MYSTERY, TYPE_MUL_NORMAL, FALSE, FLAG_CONFUSION_DAMAGE), 40);
}

s32 AI_CalcMoveDamage(u32 move, u32 moveSlot, u32 attacker, u32 defender, u32 moveType, u32 effectiveness)
{
    bool32 isCrit = CalcMoveIsCritical(attacker, defender, move);
    return CalculateDamage(PopulateDamageStruct(attacker, defender, move, moveSlot, moveType, effectiveness, isCrit, FLAG_AI_DAMAGE_CALC), 0);
}

////////////////////////////////////
// TYPE EFFECTIVENESS CALCULATION //
////////////////////////////////////

// Update effectiveness result for the type calc
static void UpdateMoveResults(u32 *multiplier, u32 mod, u32 move, u16 *flags)
{
    switch (mod)
    {
        case TYPE_MUL_NO_EFFECT:
            *flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
            *flags &= ~(MOVE_RESULT_NOT_VERY_EFFECTIVE | MOVE_RESULT_SUPER_EFFECTIVE);
            break;
        case TYPE_MUL_NOT_EFFECTIVE:
            if (GetBattleMoveSplit(move) != SPLIT_STATUS && !(*flags & MOVE_RESULT_NO_EFFECT))
            {
                if (*flags & MOVE_RESULT_SUPER_EFFECTIVE)
                    *flags &= ~(MOVE_RESULT_SUPER_EFFECTIVE);
                else
                    *flags |= MOVE_RESULT_NOT_VERY_EFFECTIVE;
            }
            break;
        case TYPE_MUL_SUPER_EFFECTIVE:
            if (GetBattleMoveSplit(move) != SPLIT_STATUS && !(*flags & MOVE_RESULT_NO_EFFECT))
            {
                if (*flags & MOVE_RESULT_NOT_VERY_EFFECTIVE)
                    *flags &= ~(MOVE_RESULT_NOT_VERY_EFFECTIVE);
                else
                    *flags |= MOVE_RESULT_SUPER_EFFECTIVE;
            }
            break;
    }
    *multiplier = uq4_12_mul(*multiplier, mod);
}

// Get effectiveness betwen two types
u32 GetTypeModifier(u32 atkType, u32 defType)
{
    u32 modifier = gTypeEffectivenessTable[atkType][defType];
    
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

static void MulByTypeEffectiveness(u32 move, u32 moveType, u32 atkAbility, u32 defender, u32 defenderType, u32 *multiplier, bool32 setAbilityFlags, bool32 forAnticipation, u16 *flags)
{
    u32 mod = GetTypeModifier(moveType, defenderType);
    
    // Check Stellar type on terastallized target
    if (moveType == TYPE_STELLAR && GetActiveGimmick(defender) == GIMMICK_TERA)
        mod = TYPE_MUL_SUPER_EFFECTIVE;
    
    // Check Foresight and Scrappy on Ghost types
    if ((moveType == TYPE_FIGHTING || moveType == TYPE_NORMAL) && defenderType == TYPE_GHOST && mod == TYPE_MUL_NO_EFFECT)
    {
        if (move == MOVE_GLARE || (gBattleMons[defender].status2 & STATUS2_FORESIGHT) || (!forAnticipation && (atkAbility == ABILITY_SCRAPPY || atkAbility == ABILITY_MINDS_EYE)))
            mod = TYPE_MUL_NORMAL;
    }

    // Check Miracle Eye
    if (moveType == TYPE_PSYCHIC && defenderType == TYPE_DARK && mod == TYPE_MUL_NO_EFFECT && (gBattleMons[defender].status2 & STATUS2_MIRACLE_EYE))
        mod = TYPE_MUL_NORMAL;
    
    // Check ground immunities
    if (moveType == TYPE_GROUND && mod == TYPE_MUL_NO_EFFECT && IsBattlerGroundedInternal(defender, forAnticipation, TRUE))
        mod = TYPE_MUL_NORMAL;
    
    if (!forAnticipation)
    {
        // Check strong winds
        if (IsBattlerWeatherAffected(defender, B_WEATHER_STRONG_WINDS) && defenderType == TYPE_FLYING && mod == TYPE_MUL_SUPER_EFFECTIVE)
        {
            mod = TYPE_MUL_NORMAL;
            
            if (setAbilityFlags && GetBattleMoveSplit(move) != SPLIT_STATUS && gBattleStruct->strongWindsMessageState == 0)
                ++gBattleStruct->strongWindsMessageState;
        }
        
        // Check moves that dont display "super effective" or "not very effective" messages
        if (GET_MOVE_MOVEEFFECT_TABLE(move).noEffectiveness && (mod == TYPE_MUL_SUPER_EFFECTIVE || mod == TYPE_MUL_NOT_EFFECTIVE))
            mod = TYPE_MUL_NORMAL;
    }
    
    // Check status moves, except Thunder Wave
    if (GetBattleMoveSplit(move) == SPLIT_STATUS && move != MOVE_THUNDER_WAVE)
        mod = TYPE_MUL_NORMAL;
    
    UpdateMoveResults(multiplier, mod, move, flags);
}

static u32 CalcTypeEffectivenessMultiplierInternal(u32 move, u32 moveType, u32 atkAbility, u32 defender, u32 multiplier, bool32 setAbilityFlags, bool32 forAnticipation, u16 *flags)
{
    u32 defAbility;
    u32 types[3];
    
    GetBattlerTypes(defender, FALSE, types);
    
    MulByTypeEffectiveness(move, moveType, atkAbility, defender, types[0], &multiplier, setAbilityFlags, forAnticipation, flags);
    
    if (types[0] != types[1])
        MulByTypeEffectiveness(move, moveType, atkAbility, defender, types[1], &multiplier, setAbilityFlags, forAnticipation, flags);
    
    if (types[2] != TYPE_MYSTERY && types[2] != types[0] && types[2] != types[1])
        MulByTypeEffectiveness(move, moveType, atkAbility, defender, types[2], &multiplier, setAbilityFlags, forAnticipation, flags);

    if (GetBattleMoveSplit(move) != SPLIT_STATUS)
    {
        defAbility = GetBattlerAbility(defender);
        
        // Check special ground immunities
        if (moveType == TYPE_GROUND && !IsBattlerGroundedInternal(defender, forAnticipation, TRUE))
        {
            bool32 immune = FALSE;
            
            if (defAbility == ABILITY_LEVITATE)
            {
                immune = TRUE;
                
                if (setAbilityFlags)
                    gBattleCommunication[MISS_TYPE] = B_MSG_ABILITY_AVOID;
            }
            else if (gStatuses3[defender] & STATUS3_MAGNET_RISE)
            {
                immune = TRUE;
                
                if (setAbilityFlags)
                    gBattleCommunication[MISS_TYPE] = B_MSG_MAGNET_RISE_AVOID;
            }
            
            if (immune)
            {
                multiplier = TYPE_MUL_NO_EFFECT;
                *flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
            }
        }
        
        // Check Wonder Guard
        if (defAbility == ABILITY_WONDER_GUARD && multiplier < TYPE_MUL_SUPER_EFFECTIVE)
        {
            multiplier = TYPE_MUL_NO_EFFECT;
            *flags |= MOVE_RESULT_MISSED;
            
            if (setAbilityFlags)
                gBattleCommunication[MISS_TYPE] = B_MSG_ABILITY_AVOID;
        }
    }
    return multiplier;
}

static u32 CalcTypeEffectivenessMultiplier(u32 move, u32 moveType, u32 atkAbility, u32 defender, bool32 setAbilityFlags, bool32 forAnticipation, u16 *flags)
{
    u32 multiplier = TYPE_MUL_NORMAL;
    
    if (move != MOVE_STRUGGLE && moveType != TYPE_MYSTERY)
    {
        multiplier = CalcTypeEffectivenessMultiplierInternal(move, moveType, atkAbility, defender, multiplier, setAbilityFlags, forAnticipation, flags);
    }
    return multiplier;
}

u32 TypeCalc(u32 move, u32 moveType, u32 attacker, u32 defender, bool32 setAbilityFlags, bool32 forAnticipation, u16 *flags)
{
    u32 multiplier;
    
    *flags = 0;
    
    multiplier = CalcTypeEffectivenessMultiplier(move, moveType, GetBattlerAbility(attacker), defender, setAbilityFlags, forAnticipation, flags);
    
    if (setAbilityFlags)
    {
        if (*flags & MOVE_RESULT_MISSED)
        {
            gBattleStruct->battlers[defender].lastLandedMove = 0;
            gBattleStruct->battlers[defender].lastHitMoveType = 0;
        }
        
        if (*flags & MOVE_RESULT_DOESNT_AFFECT_FOE)
            gProtectStructs[attacker].targetNotAffected = TRUE;
    }
    return multiplier;
}

// Calc effectiveness betwen a party mon's move and the defender
u32 AI_TypeCalc(struct Pokemon *mon, u32 move, u32 defender)
{
    u32 effectiveness, battler = GetBattlerAtPosition(GetBattlerSide(defender) == B_SIDE_PLAYER ? B_POSITION_OPPONENT_LEFT : B_POSITION_PLAYER_LEFT);
    struct BattlePokemon savedCopy = gBattleMons[battler];
    u32 status3 = gStatuses3[battler];
    u16 flags;
    
    // Overrrides the opponent's mon data with the ones of its party for the calculation
    CopyPokemonToBattleMon(battler, mon, &gBattleMons[battler], TRUE);
    gStatuses3[battler] = 0;
    
    effectiveness = TypeCalc(move, GetBattlerMoveType(battler, move), battler, defender, FALSE, FALSE, &flags);
    
    gBattleMons[battler] = savedCopy;
    gStatuses3[battler] = status3;
    
    return effectiveness;
}

// Calc resitance betwhen the mon's types and the battler's types
u32 AI_GetSwitchInTypeMatchup(struct Pokemon *mon, u32 opposingBattler)
{
    u32 i, multiplier, battler = GetBattlerAtPosition(GetBattlerSide(opposingBattler) == B_SIDE_PLAYER ? B_POSITION_OPPONENT_LEFT : B_POSITION_PLAYER_LEFT);
    struct BattlePokemon savedCopy = gBattleMons[battler];
    u32 status3 = gStatuses3[battler];
    u16 flags;
    u32 types[3];
    
    GetBattlerTypes(opposingBattler, FALSE, types);
    
    // Overrrides the opponent's mon data with the ones of its party for the calculation
    CopyPokemonToBattleMon(battler, mon, &gBattleMons[battler], TRUE);
    gStatuses3[battler] = 0;

    multiplier = TYPE_MUL_NORMAL;
    for (i = 0; i < 3; i++)
        multiplier = CalcTypeEffectivenessMultiplierInternal(MOVE_NONE, types[i], GetBattlerAbility(opposingBattler), battler, multiplier, FALSE, FALSE, &flags);

    gBattleMons[battler] = savedCopy;
    gStatuses3[battler] = status3;
    
    return multiplier;
}

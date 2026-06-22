#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_move_effects.h"
#include "battle_raid.h"
#include "battle_scripts.h"
#include "event_data.h"
#include "random.h"
#include "constants/battle_string_ids.h"
#include "constants/raid.h"

// Raid Storm Turn Thresholds
// Number of turns
#define RAID_STORM_TURNS_LEVEL_1 3
#define RAID_STORM_TURNS_LEVEL_2 6
#define RAID_STORM_TURNS_LEVEL_3 9
#define RAID_STORM_TURNS_MAX     10

static const u32 sRaidHPMultipliers[RAID_STAR_COUNT - 1] =
{
    [ONE_STAR_RAID - 1]   = UQ_4_12(1.4),
    [TWO_STAR_RAID - 1]   = UQ_4_12(1.6),
    [THREE_STAR_RAID - 1] = UQ_4_12(1.9),
    [FOUR_STAR_RAID - 1]  = UQ_4_12(2.5),
    [FIVE_STAR_RAID - 1]  = UQ_4_12(3.0),
    [SIX_STAR_RAID - 1]   = UQ_4_12(3.0),
};

bool32 IsRaidBoss(u32 battlerId)
{
    if ((gBattleTypeFlags & BATTLE_TYPE_RAID) && gBattleStruct->sos.totemBattlerId == battlerId) // Reused totem battler var for reasons
        return TRUE;
    return FALSE;
}

bool32 ShouldRaidKickPlayer(void) // Updates Raid Storm state and returns whether battle should end
{
    if (gBattleStruct->battleTurnCounter == RAID_STORM_TURNS_MAX || gBattleStruct->sides[B_SIDE_PLAYER].faintCounter >= 4)
    {
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_GETTING_TOO_STRONG;
        BattleScriptExecute(BattleScript_RaidDefeat);
        return TRUE;
    }
    else
    {
        switch (gBattleStruct->battleTurnCounter)
        {
            case RAID_STORM_TURNS_LEVEL_1:
                gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_GETTING_STRONGER;
                BattleScriptExecute(BattleScript_RaidStormBrews);
                break;
            case RAID_STORM_TURNS_LEVEL_2:
                gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_GETTING_STRONGER;
                BattleScriptExecute(BattleScript_RaidStormBrews);
                break;
            case RAID_STORM_TURNS_LEVEL_3:
                gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_GETTING_EVEN_STRONGER;
                BattleScriptExecute(BattleScript_RaidStormBrews);
                break;
        }
    }
    return FALSE;
}

bool32 ShouldRaidStartWithShields(u32 battlerId)
{
    return gBattleMons[battlerId].species == SPECIES_SHEDINJA || gBattleMons[battlerId].species == SPECIES_PYUKUMUKU || FlagGet(FLAG_START_RAID_SHIELDS);
}

u32 GetNumRaidShieldsToCreate(u32 battlerId)
{
    u32 species = gBattleMons[battlerId].species;
    
    if (species == SPECIES_SHEDINJA || species == SPECIES_PYUKUMUKU)
        return MAX_RAID_SHIELDS;
    
    switch (GetSpeciesBaseStatsTotal(species))
    {
        case 0 ... 349:
            return 1;
        case 350 ... 494:
            return 2;
        case 495 ... 568:
            return 3;
        default:
            return 4;
    }
}

u32 GetNumRaidShieldsMoveDestroys(u32 move)
{
    if (IsZMove(move))
        return 3;
    else if (IsMaxMove(move) || gBattleMoves[move].effect == EFFECT_OHKO)
        return 2;
    else
        return 1;
}

void DestroyRaidBossShields(u32 numShieldsToBreak)
{
    gBattleScripting.animArg1 = numShieldsToBreak;
    
    if (numShieldsToBreak >= gBattleStruct->raid.numShieldsUp)
    {
        gBattleMoveDamage = GetNonDynamaxMaxHP(gBattlerTarget) / 6;
        
        if (numShieldsToBreak > gBattleStruct->raid.numShieldsUp)
            gBattleMoveDamage = (gBattleMoveDamage * 15) / 10;
        
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        
        gBattleStruct->raid.turnStartHP = GetNonDynamaxHP(gBattlerTarget) - gBattleMoveDamage; // No reactivating barrier yet
        
        gBattleCommunication[MULTIUSE_STATE] = TRUE; // All shields has break
    }
    else
        gBattleCommunication[MULTIUSE_STATE] = FALSE;
}

static u32 GetRaidShieldHealthRatio(u32 battlerId)
{
    u32 ratio;
    
    switch (gBattleMons[battlerId].level)
    {
        case 0 ... 19:
            ratio = 1; // Never
            break;
        case 20 ... 40:
            ratio = 2; // Every 1/2 health lost
            break;
        case 41 ... 70:
            ratio = 3; // Every 1/3 health lost
            break;
        default:
            ratio = 4; // Every 1/4 health lost
            break;
    }
    
    if (ShouldRaidStartWithShields(battlerId) && ratio > 0)
        ratio--; // Started with shields up so battle would have one less round of shields later
    
    return ratio;
}

u32 GetNextRaidShieldHP(u32 battlerId)
{
    u32 i, healthRatio, cutOff, prevCutOff;
    
    if (IsBattlerAlive(battlerId) && gBattleStruct->raid.numShieldsUp == 0)
    {
        healthRatio = GetRaidShieldHealthRatio(battlerId);
        cutOff = 0;
        
        for (i = 1; i <= healthRatio; i++)
        {
            prevCutOff = cutOff;
            cutOff = (gBattleMons[battlerId].maxHP / healthRatio) * i;
            
            if (i == healthRatio)
                return prevCutOff;
            
            if (gBattleMons[battlerId].hp == cutOff && gBattleStruct->raid.turnStartHP != cutOff)
                return cutOff;
            
            if (gBattleMons[battlerId].hp > prevCutOff && gBattleMons[battlerId].hp <= cutOff)
                return prevCutOff;
        }
    }
    return 0;
}

bool32 ShouldCreateRaidShields(u32 battlerId)
{
    u32 i, healthRatio, cutOff;
    
    if (IsBattlerAlive(battlerId) && gNumRaidBattleStars >= THREE_STAR_RAID && gBattleStruct->raid.numShieldsUp == 0)
    {
        healthRatio = GetRaidShieldHealthRatio(battlerId);
        
        for (i = 1; i <= healthRatio; i++)
        {
            cutOff = (gBattleMons[battlerId].maxHP / healthRatio) * i;
            if (i == healthRatio)
                cutOff = gBattleMons[battlerId].maxHP;
            
            if (gBattleMons[battlerId].hp <= cutOff + (gBattleMons[battlerId].maxHP / 16) && gBattleStruct->raid.turnStartHP > cutOff)
                return TRUE;
        }
    }
    return FALSE;
}

u32 GetRaidBattleStatNullificationChance(u32 battlerId)
{
    // Don't use first attack with this
    if (!gDisableStructs[battlerId].isFirstTurn)
    {
        switch (gBattleMons[battlerId].level)
        {
            case 0 ... 19:
                return 0; // Never
            case 20 ... 40:
                return 20; // 20% chance before each attack
            case 41 ... 70:
                return 35; // 35% chance before each attack
            default:
                return 50; // 50% chance before each attack
        }
    }
    return 0;
}

bool32 IsRaidBossUsingRegularMove(u32 battlerId, u32 baseMove)
{
    if (IsRaidBoss(battlerId))
    {
        if (baseMove == MOVE_STRUGGLE || GetBattleMoveSplit(baseMove) == SPLIT_STATUS || (gBattleMons[battlerId].status2 & (STATUS2_RECHARGE | STATUS2_MULTIPLETURNS))
        || gQuickClawTurnRandom[battlerId].raidBossUsingRegularMove)
            return TRUE;
    }
    return FALSE;
}

u32 GetRaidBossDynamaxHPMultiplier(bool32 inverseMultiplier)
{
    u32 multiplier = sRaidHPMultipliers[gNumRaidBattleStars - 1];
    
    if (inverseMultiplier)
        return (UQ_4_12(1.0) * 4096) / multiplier;
    else
        return multiplier;
}

void StartRaidDynamaxEnergyPosition(void)
{
    gBattleStruct->raid.dynamaxEnergy = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
}

void ChangeRaidDynamaxEnergyPosition(void)
{
    gBattleStruct->raid.dynamaxEnergy = BATTLE_PARTNER(gBattleStruct->raid.dynamaxEnergy);
}

bool32 CanActivateGimmickInRaid(u32 battlerId)
{
    if (gBattleTypeFlags & BATTLE_TYPE_RAID)
    {
        // Check Dynamax energy position or if gimmick already activated
        if (gBattleStruct->raid.dynamaxEnergy != battlerId || gBattleStruct->raid.gimmickActivated)
            return FALSE;
    }
    return TRUE;
}

bool32 IsMoveBlockedByRaidBattle(u32 target, u32 move)
{
    if (gBattleTypeFlags & BATTLE_TYPE_RAID)
    {
        if (IsRaidBoss(target) && gBattleStruct->raid.numShieldsUp > 0 && GetBattleMoveSplit(move) == SPLIT_STATUS)
            return TRUE; // Status moves aways fail if the raid boss has shields active
        
        switch (gBattleMoves[move].effect)
        {
            case EFFECT_BUG_BITE:
            case EFFECT_CURSE:
            case EFFECT_PERISH_SONG:
            case EFFECT_DESTINY_BOND:
            case EFFECT_EXPLOSION:
            case EFFECT_INCINERATE:
            case EFFECT_SUPER_FANG:
            case EFFECT_TRICK:
                return TRUE;
        }
        
        if (MoveHasMoveEffect(move, MOVE_EFFECT_STEAL_ITEM, FALSE) || MoveHasMoveEffect(move, MOVE_EFFECT_KNOCK_OFF, FALSE))
            return TRUE;
    }
    return FALSE;
}

u32 GetRaidSheerEffect(void)
{
    u32 effectId = RAID_CHEER_NOTHING, rand = RandomUniform(RNG_RAID_CHEER, 0, 99);
    
    if (rand >= 20)
    {
        rand = rand - 20;
        
        if (gBattleStruct->raid.numShieldsUp > 0)
            effectId = rand % 6;
        else
            effectId = rand % 5;
    }
    return effectId;
}

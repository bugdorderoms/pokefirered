#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Battle Armor and Shell Armor block critical hits")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_KINGLER; ability = ABILITY_SHELL_ARMOR; }
    PARAMETRIZE { species = SPECIES_ARMALDO; ability = ABILITY_BATTLE_ARMOR; }
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(species) { Ability(ability); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, criticalHit: TRUE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        NOT MESSAGE("A critical hit!");
    }
}

SINGLE_BATTLE_TEST("Battle Armor and Shell Armor protects from always crit moves")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_KINGLER; ability = ABILITY_SHELL_ARMOR; }
    PARAMETRIZE { species = SPECIES_ARMALDO; ability = ABILITY_BATTLE_ARMOR; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_STORM_THROW].critStage == 3);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(species) { Ability(ability); }
    } WHEN {
        TURN { MOVE(player, MOVE_STORM_THROW); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STORM_THROW, player);
        NOT MESSAGE("A critical hit!");
    }
}

SINGLE_BATTLE_TEST("Battle Armor and Shell Armor protects from Merciless when poisoned")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_KINGLER; ability = ABILITY_SHELL_ARMOR; }
    PARAMETRIZE { species = SPECIES_ARMALDO; ability = ABILITY_BATTLE_ARMOR; }
    
    GIVEN {
        PLAYER(SPECIES_MAREANIE) { Ability(ABILITY_MERCILESS); }
        OPPONENT(species) { Ability(ability); Status1(STATUS1_POISON); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        NOT MESSAGE("A critical hit!");
    }
}

SINGLE_BATTLE_TEST("Mold Breaker, Teravolt and Turboblaze ignore Battle Armor and Shell Armor")
{
    KNOWN_FAILING; // MESSAGE does'nt handle special color codes, used by the critical hit message to be red
    
    u32 j;
    u32 species1, species2, ability1, ability2;
    static const u32 breakerData[][2] =
    {
        {SPECIES_PINSIR,   ABILITY_MOLD_BREAKER},
        {SPECIES_ZEKROM,   ABILITY_TERAVOLT},
        {SPECIES_RESHIRAM, ABILITY_TURBOBLAZE},
    };

    for (j = 0; j < ARRAY_COUNT(breakerData); j++)
    {
        PARAMETRIZE { species1 = breakerData[j][0]; ability1 = breakerData[j][1]; species2 = SPECIES_KINGLER; ability2 = ABILITY_SHELL_ARMOR; }
        PARAMETRIZE { species1 = breakerData[j][0]; ability1 = breakerData[j][1]; species2 = SPECIES_ARMALDO; ability2 = ABILITY_BATTLE_ARMOR; }
    }

    GIVEN {
        PLAYER(species1) { Ability(ability1); }
        OPPONENT(species2) { Ability(ability2); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, criticalHit: TRUE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        MESSAGE("A critical hit!");
    }
}

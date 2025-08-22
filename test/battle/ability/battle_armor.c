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
        NOT MESSAGE("{COLOR 2}{SHADOW 9}A critical hit!");
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
        NOT MESSAGE("{COLOR 2}{SHADOW 9}A critical hit!");
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
        NOT MESSAGE("{COLOR 2}{SHADOW 9}A critical hit!");
    }
}

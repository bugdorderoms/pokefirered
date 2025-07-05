#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Battle Armor protects from critical hits")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CUBONE) { Ability(ABILITY_BATTLE_ARMOR); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, criticalHit: TRUE); }
    } SCENE {
        NOT MESSAGE("A critical hit!");
    }
}

SINGLE_BATTLE_TEST("Battle Armor protects from moves that always land criticals")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_STORM_THROW].critStage == 3);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CUBONE) { Ability(ABILITY_BATTLE_ARMOR); }
    } WHEN {
        TURN { MOVE(player, MOVE_STORM_THROW); }
    } SCENE {
        NOT MESSAGE("A critical hit!");
    }
}

SINGLE_BATTLE_TEST("Battle Armor protects from Merciless when poisoned")
{
    GIVEN {
        PLAYER(SPECIES_MAREANIE) { Ability(ABILITY_MERCILESS); }
        OPPONENT(SPECIES_CUBONE) { Ability(ABILITY_BATTLE_ARMOR); Status1(STATUS1_POISON); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        NOT MESSAGE("A critical hit!");
    }
}

#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Stench has a 10% chance to flinch")
{
    KNOWN_FAILING; // RNG untestable
    
    PASSES_RANDOMLY(1, 10); // 10% chance
    
    GIVEN {
        PLAYER(SPECIES_GRIMER) { Ability(ABILITY_STENCH); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("Grimer used Scratch!");
        MESSAGE("Foe Wobbuffet flinched!");
    }
}

SINGLE_BATTLE_TEST("Stench only triggers if target takes damage")
{
    PASSES_RANDOMLY(10, 10); // Will pass every time
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SAND_ATTACK].power == 0);
        
        PLAYER(SPECIES_GRIMER) { Ability(ABILITY_STENCH); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SAND_ATTACK); }
    } SCENE {
        MESSAGE("Grimer used Sand Attack!");
        NOT MESSAGE("Foe Wobbuffet flinched!");
    }
}

SINGLE_BATTLE_TEST("Stench does not stack with King's Rock")
{
    KNOWN_FAILING; // RNG untestable
    
    PASSES_RANDOMLY(1, 10); // 10% chance, not stacked with King's Rock
    
    GIVEN {
        ASSUME(gItems[ITEM_KINGS_ROCK].holdEffect == HOLD_EFFECT_FLINCH);

        PLAYER(SPECIES_GRIMER) { Ability(ABILITY_STENCH); Item(ITEM_KINGS_ROCK); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("Grimer used Scratch!");
        MESSAGE("Foe Wobbuffet flinched!");
    }
}

SINGLE_BATTLE_TEST("Stench does not stack with moves with flinch chance")
{
    KNOWN_FAILING; // RNG untestable
    
    PASSES_RANDOMLY(3, 10); // 30% chance of move Bite
    
    GIVEN {
        ASSUME(MoveHasMoveEffect(MOVE_BITE, MOVE_EFFECT_FLINCH, FALSE));

        PLAYER(SPECIES_GRIMER) { Ability(ABILITY_STENCH); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_BITE); }
    } SCENE {
        MESSAGE("Grimer used Bite!");
        MESSAGE("Foe Wobbuffet flinched!");
    }
}

// TODO:
// Stench has a 10% chance to flinch in each multi-strike hit

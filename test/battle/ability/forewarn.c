#include "global.h"
#include "test/battle.h"

DOUBLE_BATTLE_TEST("Forewarn alerts about the opponent's strongest move on switch in")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_CLOSE_COMBAT].power > gBattleMoves[MOVE_SCRATCH].power);
        
        PLAYER(SPECIES_DROWZEE) { Ability(ABILITY_FOREWARN); }
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_BULBASAUR);
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH, MOVE_SCRATCH, MOVE_CLOSE_COMBAT, MOVE_FLASH); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_FOREWARN);
        MESSAGE("It was alerted to Foe Wobbuffet's Close Combat!");
    }
}

SINGLE_BATTLE_TEST("Forewarn choses the strongest move at random in a tie")
{
    PASSES_RANDOMLY(1, 2, RNG_FOREWARN);
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_POUND].power == gBattleMoves[MOVE_SCRATCH].power);
        
        PLAYER(SPECIES_DROWZEE) { Ability(ABILITY_FOREWARN); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_SCRATCH, MOVE_SPLASH, MOVE_POUND); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_FOREWARN);
        MESSAGE("It was alerted to Foe Wobbuffet's Pound!");
    }
}

TO_DO_BATTLE_TEST("Forewarn can activate by a non-adjacent Pokémon's strong move");

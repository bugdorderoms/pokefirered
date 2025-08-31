#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Shed Skin cures status 33% of the time at end turn")
{
    PASSES_RANDOMLY(33, 100, RNG_SHED_SKIN);
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_ARBOK) { Status1(STATUS1_POISON); Ability(ABILITY_SHED_SKIN); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_SHED_SKIN);
        MESSAGE("Foe Arbok's Shed Skin cured its poison problem!");
        STATUS_ICON(opponent, poison: FALSE);
    }
}

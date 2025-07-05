#include "global.h"
#include "math_util.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Speed Boost boosts speed every end turn")
{
    GIVEN {
        PLAYER(SPECIES_TORCHIC) { Ability(ABILITY_SPEED_BOOST); Speed(99); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(100); }
    } WHEN {
        TURN { }
        TURN { }
    } SCENE {
        // Turn 1
        MESSAGE("Foe Wobbuffet used Splash!");
        MESSAGE("Torchic used Splash!");
        ABILITY_POPUP(player, ABILITY_SPEED_BOOST);
        MESSAGE("Torchic's Speed rose!");
        // Turn 2
        MESSAGE("Torchic used Splash!");
        MESSAGE("Foe Wobbuffet used Splash!");
    }
}

SINGLE_BATTLE_TEST("Speed Boost does not activate the turn it switches in")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_TORCHIC) { Ability(ABILITY_SPEED_BOOST); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { SWITCH(player, 1); }
    } SCENE {
        NONE_OF {
            ABILITY_POPUP(player, ABILITY_SPEED_BOOST);
            MESSAGE("Torchic's Speed rose!");
        }
    }
}

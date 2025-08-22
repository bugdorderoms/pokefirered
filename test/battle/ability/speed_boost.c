#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Speed Boost boosts speed every end turn")
{
    GIVEN {
        PLAYER(SPECIES_TORCHIC) { Ability(ABILITY_SPEED_BOOST); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_SPEED_BOOST);
        MESSAGE("Torchic's Speed rose!");
    } THEN {
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGES + 1);
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

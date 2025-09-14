#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Stall makes moves used moves last")
{
    GIVEN {
        PLAYER(SPECIES_SABLEYE) { Ability(ABILITY_STALL); Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SPLASH); MOVE(player, MOVE_SPLASH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
    }
}

TO_DO_BATTLE_TEST("Stall doesn't activate if holding a Lagging Tail");
TO_DO_BATTLE_TEST("Stall moves before a Pokémon holding a Lagging Tail");

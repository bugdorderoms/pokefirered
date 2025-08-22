#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Early bird halves sleep duration")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SLEEP_POWDER].effect == EFFECT_SLEEP);
        
        PLAYER(SPECIES_DODRIO) { Ability(ABILITY_EARLY_BIRD); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SLEEP_POWDER, WITH_RNG(RNG_SLEEP_TURNS, 2)); }
        TURN { }
    } SCENE {
        MESSAGE("Dodrio woke up!");
        STATUS_ICON(player, none: TRUE);
        
        NONE_OF {
            MESSAGE("Dodrio is fast asleep.");
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_SLP, player);
        }
    }
}

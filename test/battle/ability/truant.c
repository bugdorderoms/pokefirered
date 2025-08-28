#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Truant will cause the user to loafing around every two turns")
{
    GIVEN {
        PLAYER(SPECIES_SLAKING) { Ability(ABILITY_TRUANT); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        // Turn 1
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        // Turn 2
        MESSAGE("Slaking is loafing around!");
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
    }
}

SINGLE_BATTLE_TEST("Truant doesn't cause loafing around the turn it wake up")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_REST].effect == EFFECT_REST);
        
        PLAYER(SPECIES_SLAKING) { Ability(ABILITY_TRUANT); HP(1); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_REST); }
        TURN { MOVE(player, MOVE_SPLASH); }
        TURN { MOVE(player, MOVE_SPLASH); }
        TURN { MOVE(player, MOVE_SPLASH); }
    } SCENE {
        // Turn 1
        ANIMATION(ANIM_TYPE_MOVE, MOVE_REST, player);
        
        // Last turn
        MESSAGE("Slaking woke up!");
        NOT MESSAGE("Slaking is loafing around!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
    }
}

TO_DO_BATTLE_TEST("Truant will loafing around if instructed to use a move right after it has moved");

#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Quick Feet boosts speed by 1.5x if statused")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_TOXIC].effect == EFFECT_SET_TOXIC_POISON);
        
        PLAYER(SPECIES_ZIGZAGOON) { Ability(ABILITY_QUICK_FEET); Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(149); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_TOXIC); MOVE(player, MOVE_SPLASH); }
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TOXIC, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);
    }
}

SINGLE_BATTLE_TEST("Quick Feet prevents paralysis speed reduction")
{
    GIVEN {
        PLAYER(SPECIES_ZIGZAGOON) { Ability(ABILITY_QUICK_FEET); Speed(100); Status1(STATUS1_PARALYSIS); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(60); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);
    }
}

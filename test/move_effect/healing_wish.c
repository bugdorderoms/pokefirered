#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_HEALING_WISH].effect == EFFECT_HEALING_WISH);
    ASSUME(gBattleMoves[MOVE_HEALING_WISH].argument.generic == HR_CASE_HEALING_WISH);
}

DOUBLE_BATTLE_TEST("Healing Wish activates when ally switches to it's slot using Ally Switch")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_ALLY_SWITCH].effect == EFFECT_ALLY_SWITCH);
        
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(100); HP(1); }
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerRight, MOVE_HEALING_WISH); SEND_OUT(playerRight, 2); }
        TURN { MOVE(playerLeft, MOVE_ALLY_SWITCH); }
    } SCENE {
        // Turn 1
        MESSAGE("Wobbuffet used Healing Wish!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HEALING_WISH, playerRight);
        HP_BAR(playerRight, hp: 0);
        
        // Turn 2
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ALLY_SWITCH, playerLeft);
        
        MESSAGE("The healing wish came true for Wobbuffet!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HEALING_WISH_HEAL, playerRight);
        HP_BAR(playerRight, hp: 100);
    }
}

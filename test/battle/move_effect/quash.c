#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_QUASH].effect == EFFECT_QUASH);
}

DOUBLE_BATTLE_TEST("Quash makes the target move last")
{
    GIVEN {
        PLAYER(SPECIES_VOLBEAT) { Speed(4); Ability(ABILITY_PRANKSTER); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(3); }
        
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_QUASH, target: opponentLeft); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_QUASH, playerLeft);
        MESSAGE("Foe Wobbuffet's move was postponed!");
        
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, playerRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponentRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponentLeft);
    }
}

DOUBLE_BATTLE_TEST("Quash fails if target has already moved")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(3); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_QUASH, target: opponentLeft); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_QUASH, playerLeft);
        MESSAGE("But it failed!");
    }
}

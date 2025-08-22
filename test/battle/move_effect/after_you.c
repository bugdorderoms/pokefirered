#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_AFTER_YOU].effect == EFFECT_AFTER_YOU);
}

DOUBLE_BATTLE_TEST("After You makes the target move after user")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(1); }
        
        OPPONENT(SPECIES_WOBBUFFET) { Speed(3); }
        OPPONENT(SPECIES_WYNAUT) { Speed(2); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_AFTER_YOU, target: playerRight);
            MOVE(playerRight, MOVE_SPLASH);
            MOVE(opponentLeft, MOVE_SPLASH);
            MOVE(opponentRight, MOVE_SPLASH);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AFTER_YOU, playerLeft);
        MESSAGE("Wynaut took the kind offer!");
        
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, playerRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponentRight);
    }
}

DOUBLE_BATTLE_TEST("After You fails if target has already moved")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        
        OPPONENT(SPECIES_WOBBUFFET) { Speed(4); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(3); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_AFTER_YOU, target: opponentLeft); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_AFTER_YOU, playerLeft);
        MESSAGE("But it failed!");
    }
}

DOUBLE_BATTLE_TEST("After You doesn't fail if the turn order remains the same")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(1); }
        
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(3); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_SPLASH);
            MOVE(playerRight, MOVE_SPLASH);
            MOVE(opponentLeft, MOVE_SPLASH);
            MOVE(opponentRight, MOVE_AFTER_YOU, target: opponentLeft);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AFTER_YOU, opponentRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, playerRight);
    }
}

DOUBLE_BATTLE_TEST("After You ignores the effects of Quash")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_QUASH].effect == EFFECT_QUASH);
        
        PLAYER(SPECIES_WOBBUFFET) { Speed(4); }
        PLAYER(SPECIES_WYNAUT) { Speed(1); }
        
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WYNAUT) { Speed(3); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_QUASH, target: opponentLeft);
            MOVE(playerRight, MOVE_SPLASH);
            MOVE(opponentLeft, MOVE_SPLASH);
            MOVE(opponentRight, MOVE_AFTER_YOU, target: opponentLeft);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_QUASH, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_AFTER_YOU, opponentRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, playerRight);
    }
}

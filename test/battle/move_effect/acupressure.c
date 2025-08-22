#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_ACUPRESSURE].effect == EFFECT_ACUPRESSURE);
    ASSUME(gBattleMoves[MOVE_ACUPRESSURE].target == MOVE_TARGET_USER_OR_ALLY);
}

DOUBLE_BATTLE_TEST("Acupressure affects ally if targeted the user and it switched slots")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_ALLY_SWITCH].effect == EFFECT_ALLY_SWITCH);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        
        OPPONENT(SPECIES_KADABRA);
        OPPONENT(SPECIES_ABRA);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_ALLY_SWITCH); MOVE(playerRight, MOVE_ACUPRESSURE, target: playerRight); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ALLY_SWITCH, playerLeft);
        
        MESSAGE("Wynaut used Acupressure!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ACUPRESSURE, playerLeft);
        NOT MESSAGE("But it failed!");
    }
}

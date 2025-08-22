#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Keen Eye ignore target's evasion stat")
{
    PASSES_RANDOMLY(100, 100, RNG_ACCURACY);
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].accuracy == 100);
        ASSUME(gBattleMoves[MOVE_DOUBLE_TEAM].effect == EFFECT_EVASION_UP);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_HITMONCHAN) { Ability(ABILITY_KEEN_EYE); }
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_TEAM); MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_TEAM, player);
        MESSAGE("Wobbuffet's evasiveness fell!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
    }
}

// Other tests for Keen Eye are handled in test/battle/ability/illuminate.c

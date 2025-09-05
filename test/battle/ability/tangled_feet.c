#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Tangled Feet doubles evasion if confused")
{
    PASSES_RANDOMLY(50, 100, RNG_ACCURACY);
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_CONFUSE_RAY].effect == EFFECT_CONFUSE);
        ASSUME(gBattleMoves[MOVE_SCRATCH].accuracy == 100);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_PIDGEY) { Ability(ABILITY_TANGLED_FEET); }
    } WHEN {
        TURN { MOVE(player, MOVE_CONFUSE_RAY); }
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
    }
}

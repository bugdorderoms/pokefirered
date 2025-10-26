#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Scrappy makes Normal and Fighting type moves hit Ghost types")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_SCRATCH; }
    PARAMETRIZE { move = MOVE_KARATE_CHOP; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].type == TYPE_NORMAL);
        ASSUME(gBattleMoves[MOVE_KARATE_CHOP].type == TYPE_FIGHTING);
        ASSUME(gSpeciesInfo[SPECIES_GASTLY].types[0] == TYPE_GHOST || gSpeciesInfo[SPECIES_GASTLY].types[1] == TYPE_GHOST);
        
        PLAYER(SPECIES_MILTANK) { Ability(ABILITY_SCRAPPY); }
        OPPONENT(SPECIES_GASTLY);
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        NOT MESSAGE("It doesn't affect Foe Gastly…");
    }
}

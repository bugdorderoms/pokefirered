#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Super Luck increases the critical hit ratio by 1 stage")
{
    PASSES_RANDOMLY(1, 8, RNG_CRITICAL_HIT);
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].critStage == 0);
        
        PLAYER(SPECIES_TOGEPI) { Ability(ABILITY_SUPER_LUCK); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("{COLOR 2}{SHADOW 9}A critical hit!");
    }
}

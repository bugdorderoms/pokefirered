#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Compound Eyes raises accuracy by 1.3x")
{
    PASSES_RANDOMLY(91, 100, RNG_ACCURACY);
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_THUNDER].accuracy == 70);
        
        PLAYER(SPECIES_BUTTERFREE) { Ability(ABILITY_COMPOUND_EYES); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_THUNDER); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_THUNDER, player);
        HP_BAR(opponent);
    }
}

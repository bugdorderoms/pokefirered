#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Poison Point has a 30% chance to poison")
{
    PASSES_RANDOMLY(3, 10, RNG_POISON_POINT);
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].flags.makesContact == TRUE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_NIDORINA) { Ability(ABILITY_POISON_POINT); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_POISON_POINT);
        MESSAGE("Wobbuffet was poisoned!");
    }
}

SINGLE_BATTLE_TEST("Poison Point only triggers if move makes contact")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_SCRATCH; };
    PARAMETRIZE { move = MOVE_GUST; };
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].flags.makesContact == TRUE);
        ASSUME(gBattleMoves[MOVE_GUST].flags.makesContact == FALSE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_NIDORINA) { Ability(ABILITY_POISON_POINT); }
    } WHEN {
        TURN { MOVE(player, move, WITH_RNG(RNG_POISON_POINT, TRUE)); }
    } SCENE {
        if (gBattleMoves[move].flags.makesContact) {
            ABILITY_POPUP(opponent, ABILITY_POISON_POINT);
            MESSAGE("Wobbuffet was poisoned!");
        } else {
            NONE_OF {
                ABILITY_POPUP(opponent, ABILITY_POISON_POINT);
                MESSAGE("Wobbuffet was poisoned!");
            }
        }
    }
}

TO_DO_BATTLE_TEST("Poison Point has a 30% chance to poison in each multi-strike hit");

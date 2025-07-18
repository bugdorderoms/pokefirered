#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Static has a 30% chance to paralyze")
{
    PASSES_RANDOMLY(3, 10, RNG_STATIC);
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].flags.makesContact == TRUE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_PIKACHU) { Ability(ABILITY_STATIC); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_STATIC);
        MESSAGE("Wobbuffet is paralyzed! It may be unable to move!");
    }
}

SINGLE_BATTLE_TEST("Static only triggers if move makes contact")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_SCRATCH; };
    PARAMETRIZE { move = MOVE_GUST; };
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].flags.makesContact == TRUE);
        ASSUME(gBattleMoves[MOVE_GUST].flags.makesContact == FALSE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_PIKACHU) { Ability(ABILITY_STATIC); }
    } WHEN {
        TURN { MOVE(player, move, WITH_RNG(RNG_STATIC, TRUE)); }
    } SCENE {
        if (gBattleMoves[move].flags.makesContact) {
            ABILITY_POPUP(opponent, ABILITY_STATIC);
            MESSAGE("Wobbuffet is paralyzed! It may be unable to move!");
        } else {
            NONE_OF {
                ABILITY_POPUP(opponent, ABILITY_STATIC);
                MESSAGE("Wobbuffet is paralyzed! It may be unable to move!");
            }
        }
    }
}

TO_DO_BATTLE_TEST("Static has a 30% chance to paralyze in each multi-strike hit");

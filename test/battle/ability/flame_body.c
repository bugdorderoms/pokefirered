#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_SCRATCH].flags.makesContact == TRUE);
}

SINGLE_BATTLE_TEST("Flame Body has a 30% chance to burn")
{
    PASSES_RANDOMLY(3, 10, RNG_FLAME_BODY);
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_MAGMAR) { Ability(ABILITY_FLAME_BODY); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_FLAME_BODY);
        MESSAGE("Wobbuffet was burned!");
    }
}

SINGLE_BATTLE_TEST("Flame Body only triggers if move makes contact")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_SCRATCH; };
    PARAMETRIZE { move = MOVE_GUST; };
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_GUST].flags.makesContact == FALSE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_MAGMAR) { Ability(ABILITY_FLAME_BODY); }
    } WHEN {
        TURN { MOVE(player, move, WITH_RNG(RNG_FLAME_BODY, TRUE)); }
    } SCENE {
        if (gBattleMoves[move].flags.makesContact) {
            ABILITY_POPUP(opponent, ABILITY_FLAME_BODY);
            MESSAGE("Wobbuffet was burned!");
        } else {
            NONE_OF {
                ABILITY_POPUP(opponent, ABILITY_FLAME_BODY);
                MESSAGE("Wobbuffet was burned!");
            }
        }
    }
}

#include "global.h"
#include "test/battle.h"

DOUBLE_BATTLE_TEST("Damp prevents self-destruction moves")
{
    u32 move;
    
    KNOWN_FAILING; // Mind Blown not implemented yet
    
    PARAMETRIZE { move = MOVE_EXPLOSION; }
    PARAMETRIZE { move = MOVE_SELF_DESTRUCT; }
    PARAMETRIZE { move = MOVE_MISTY_EXPLOSION; }
    PARAMETRIZE { move = MOVE_MIND_BLOWN; }
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_PARAS) { Ability(ABILITY_DAMP); }
    } WHEN {
        TURN { MOVE(playerLeft, move); }
    } SCENE {
        ABILITY_POPUP(opponentRight, ABILITY_DAMP);
        
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, move, playerLeft);
            HP_BAR(playerLeft);
        }
    }
}

SINGLE_BATTLE_TEST("Damp prevents damage from Aftermath")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].flags.makesContact == TRUE);
        
        PLAYER(SPECIES_PARAS) { Ability(ABILITY_DAMP); }
        
        OPPONENT(SPECIES_VOLTORB) { Ability(ABILITY_AFTERMATH); HP(1); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); SEND_OUT(opponent, 1); }
    } SCENE {
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_AFTERMATH);
            HP_BAR(player);
        }
    }
}

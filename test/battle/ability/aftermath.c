#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_SCRATCH].flags.makesContact == TRUE);
}

SINGLE_BATTLE_TEST("Aftermath damages the attacker by 1/4th of its max HP if fainted by a contact move")
{
    GIVEN {
        PLAYER(SPECIES_VOLTORB) { Ability(ABILITY_AFTERMATH); HP(1); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(100); HP(100); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_AFTERMATH);
        HP_BAR(opponent, damage: 25);
        MESSAGE("Foe Wobbuffet was hurt!");
        MESSAGE("Voltorb fainted!");
    }
}

SINGLE_BATTLE_TEST("Aftermath only triggers if it faints")
{
    GIVEN {
        PLAYER(SPECIES_VOLTORB) { Ability(ABILITY_AFTERMATH); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        NONE_OF {
            ABILITY_POPUP(player, ABILITY_AFTERMATH);
            HP_BAR(opponent);
            MESSAGE("Foe Wobbuffet was hurt!");
            MESSAGE("Voltorb fainted!");
        }
    }
}

SINGLE_BATTLE_TEST("Aftermath only triggers if move makes contact")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_SCRATCH; };
    PARAMETRIZE { move = MOVE_GUST; };
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_GUST].flags.makesContact == FALSE);
        
        PLAYER(SPECIES_WOBBUFFET)
        OPPONENT(SPECIES_VOLTORB) { Ability(ABILITY_AFTERMATH); HP(1); }
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        if (gBattleMoves[move].flags.makesContact) {
            ABILITY_POPUP(opponent, ABILITY_AFTERMATH);
            HP_BAR(player);
            MESSAGE("Wobbuffet was hurt!");
        } else {
            NONE_OF {
                ABILITY_POPUP(opponent, ABILITY_AFTERMATH);
                HP_BAR(player);
                MESSAGE("Wobbuffet was hurt!");
            }
        }
    }
}

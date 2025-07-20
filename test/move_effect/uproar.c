#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_UPROAR].effect == EFFECT_UPROAR);
}

DOUBLE_BATTLE_TEST("Uproar wakes up sleeping Pokémon when used")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET) { Status1(STATUS1_SLEEP); }
        
        OPPONENT(SPECIES_WOBBUFFET) { Status1(STATUS1_SLEEP); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_UPROAR); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_UPROAR, playerLeft);
        
        STATUS_ICON(opponentLeft, sleep: FALSE);
        MESSAGE("Foe Wobbuffet was woken up by the uproar!");
        
        STATUS_ICON(playerRight, sleep: FALSE);
        MESSAGE("Wobbuffet was woken up by the uproar!");
        
        NONE_OF {
            STATUS_ICON(opponentRight, sleep: FALSE);
            MESSAGE("Foe Wobbuffet was woken up by the uproar!");
        }
    }
}

DOUBLE_BATTLE_TEST("Uproar prevents pokemon on field from sleep")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SLEEP_POWDER].effect == EFFECT_SLEEP);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_UPROAR);
            MOVE(opponentLeft, MOVE_SLEEP_POWDER, target: playerRight); 
            MOVE(playerRight, MOVE_SLEEP_POWDER, target: opponentLeft);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_UPROAR, playerLeft);
        
        MESSAGE("But the uproar kept Wobbuffet awake!");
        NOT STATUS_ICON(playerRight, sleep: TRUE);
        
        MESSAGE("But the uproar kept Foe Wobbuffet awake!");
        NOT STATUS_ICON(opponentLeft, sleep: TRUE);
    }
}

SINGLE_BATTLE_TEST("Uproar does not cause Yawn to fail, but a drowsy pokemon cant fall asleep")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_YAWN].effect == EFFECT_YAWN);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_UPROAR); MOVE(opponent, MOVE_YAWN); }
        TURN { }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_UPROAR, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_YAWN, opponent);
        
        NONE_OF {
            MESSAGE("Wobbuffet fell asleep!");
            STATUS_ICON(player, sleep: TRUE);
        }
    }
}

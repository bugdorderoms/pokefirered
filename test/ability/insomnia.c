#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Insomnia prevents sleep")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SPORE].effect == EFFECT_SLEEP);

        PLAYER(SPECIES_DROWZEE) { Ability(ABILITY_INSOMNIA); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SPORE); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INSOMNIA);
        
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SPORE, opponent);
            STATUS_ICON(player, sleep: TRUE);
        }
    }
}

SINGLE_BATTLE_TEST("Insomnia prevents yawn")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_YAWN].effect == EFFECT_YAWN);
        
        PLAYER(SPECIES_DROWZEE) { Ability(ABILITY_INSOMNIA); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_YAWN); }
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INSOMNIA);
        
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_YAWN, opponent);
            STATUS_ICON(player, sleep: TRUE);
        }
    }
}

SINGLE_BATTLE_TEST("Insomnia prevents a drowsy pokemon fall asleep")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_YAWN].effect == EFFECT_YAWN);
        
        PLAYER(SPECIES_PINSIR) { Ability(ABILITY_MOLD_BREAKER); }
        OPPONENT(SPECIES_DROWZEE) { Ability(ABILITY_INSOMNIA); }
    } WHEN {
        TURN { MOVE(player, MOVE_YAWN); }
        TURN { }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_YAWN, player);
        
        NONE_OF {
            MESSAGE("Foe Drowzee fell asleep!");
            STATUS_ICON(opponent, sleep: TRUE);
        }
    }
}

SINGLE_BATTLE_TEST("Insomnia prevents rest")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_REST].effect == EFFECT_REST);
        
        PLAYER(SPECIES_DROWZEE) { Ability(ABILITY_INSOMNIA); HP(1); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_REST); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INSOMNIA);
        
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_REST, player);
            STATUS_ICON(player, sleep: TRUE);
            HP_BAR(player);
        }
    }
}

SINGLE_BATTLE_TEST("Insomnia wakes it up if its sleeping")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SPORE].effect == EFFECT_SLEEP);
        
        PLAYER(SPECIES_PINSIR) { Ability(ABILITY_MOLD_BREAKER); }
        OPPONENT(SPECIES_DROWZEE) { Ability(ABILITY_INSOMNIA); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPORE); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_INSOMNIA);
        MESSAGE("Foe Drowzee's Insomnia cured its sleep problem!");
        STATUS_ICON(opponent, none: TRUE);
    }
}

SINGLE_BATTLE_TEST("Insomnia wakes it up when it enters battle")
{
    GIVEN {
        PLAYER(SPECIES_DROWZEE) { Ability(ABILITY_INSOMNIA); Status1(STATUS1_SLEEP); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_INSOMNIA);
        MESSAGE("Drowzee was cured of its sleep!");
        STATUS_ICON(player, none: TRUE);
    }
}

TO_DO_BATTLE_TEST("A held berry will activate and wake it up before Insomnia activates");

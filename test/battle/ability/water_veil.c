#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Water Veil prevents burn")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_WILL_O_WISP].effect == EFFECT_WILL_O_WISP);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_MANTINE) { Ability(ABILITY_WATER_VEIL); }
    } WHEN {
        TURN { MOVE(player, MOVE_WILL_O_WISP); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_WATER_VEIL);
        MESSAGE("Foe Mantine's Water Veil prevents burns!");
        
        NONE_OF {
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_BRN, opponent);
            STATUS_ICON(opponent, burn: TRUE);
        }
    }
}

SINGLE_BATTLE_TEST("Water Veil cures burn if burned")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_WILL_O_WISP].effect == EFFECT_WILL_O_WISP);
        
        PLAYER(SPECIES_PINSIR) { Ability(ABILITY_MOLD_BREAKER); }
        OPPONENT(SPECIES_MANTINE) { Ability(ABILITY_WATER_VEIL); }
    } WHEN {
        TURN { MOVE(player, MOVE_WILL_O_WISP); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_WATER_VEIL);
        MESSAGE("Foe Mantine's Water Veil cured its burn problem!");
        STATUS_ICON(opponent, none: TRUE);
    }
}

SINGLE_BATTLE_TEST("Water Veil cures burn when it enters battle")
{
    GIVEN {
        PLAYER(SPECIES_MANTINE) { Ability(ABILITY_WATER_VEIL); Status1(STATUS1_BURN); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_WATER_VEIL);
        MESSAGE("Mantine was cured of its burn!");
        STATUS_ICON(player, none: TRUE);
    }
}

TO_DO_BATTLE_TEST("A held berry will activate and cure the burn before Water Veil activates");

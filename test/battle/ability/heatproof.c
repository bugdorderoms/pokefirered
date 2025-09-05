#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Heatproof halves the damage taken from burn")
{
    GIVEN {
        PLAYER(SPECIES_BRONZOR) { Ability(ABILITY_HEATPROOF); Status1(STATUS1_BURN); MaxHP(160); HP(160); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        MESSAGE("Bronzor is hurt by its burn!");
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_BRN, player);
        HP_BAR(player, damage: BURN_UPDATE ? 5 : 10);
    }
}

TO_DO_BATTLE_TEST("Heatproof halves the damage taken from Fire-type attacks"); // Need Electrify to be tested

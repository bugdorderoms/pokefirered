#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Dry Skin loses 1/8 max HP in sun at end turn")
{
    GIVEN {
        PLAYER(SPECIES_PARAS) { Ability(ABILITY_DRY_SKIN); MaxHP(96); HP(96); }
        OPPONENT(SPECIES_VULPIX) { Ability(ABILITY_DROUGHT); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_DRY_SKIN);
        MESSAGE("Paras lost some of its HP!");
        HP_BAR(player, damage: 12);
    }
}

SINGLE_BATTLE_TEST("Dry Skin recovers 1/8 max HP in rain at end turn")
{
    GIVEN {
        PLAYER(SPECIES_PARAS) { Ability(ABILITY_DRY_SKIN); MaxHP(96); HP(1); }
        OPPONENT(SPECIES_POLITOED) { Ability(ABILITY_DRIZZLE); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_DRY_SKIN);
        MESSAGE("Paras gain some of its HP!");
        HP_BAR(player, damage: -12);
    }
}

TO_DO_BATTLE_TEST("Dry Skin increases damage taken from Fire-type moves by 25%");

// Other tests for Dry Skin are handled in test/battle/ability/water_absorb.c

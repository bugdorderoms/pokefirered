#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Drizzle summons rain when it enters battle")
{
    GIVEN {
        PLAYER(SPECIES_POLITOED) { Ability(ABILITY_DRIZZLE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_DRIZZLE);
        MESSAGE("It started to rain!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RAIN_CONTINUES);
    }
}

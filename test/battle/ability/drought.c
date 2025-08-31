#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Drought summons sun when it enters battle")
{
    GIVEN {
        PLAYER(SPECIES_VULPIX) { Ability(ABILITY_DROUGHT); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_DROUGHT);
        MESSAGE("The sunlight turned harsh!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SUN_CONTINUES);
    }
}

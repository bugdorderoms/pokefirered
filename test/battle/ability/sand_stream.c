#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Sand Stream summons sandstorm when it enters battle")
{
    GIVEN {
        PLAYER(SPECIES_TYRANITAR) { Ability(ABILITY_SAND_STREAM); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_SAND_STREAM);
        MESSAGE("A sandstorm kicked up!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SANDSTORM_CONTINUES);
    }
}

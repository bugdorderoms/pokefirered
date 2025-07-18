#include "global.h"
#include "math_util.h"
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

SINGLE_BATTLE_TEST("Drizzle will not activate if it undergo Primal Reversion")
{
    GIVEN {
        PLAYER(SPECIES_KYOGRE) { Ability(ABILITY_DRIZZLE); Item(ITEM_BLUE_ORB); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_BLUE_PRIMAL_REVERSION, player);
        MESSAGE("Kyogre's Primal Reversion! It reverted to its primal state!");
        
        NONE_OF {
            ABILITY_POPUP(player, ABILITY_DRIZZLE);
            MESSAGE("It started to rain!");
        }
    }
}

SINGLE_BATTLE_TEST("Drizzle will not activate if primal weather is active")
{
    GIVEN {
        PLAYER(SPECIES_KYOGRE) { Ability(ABILITY_DRIZZLE); Item(ITEM_BLUE_ORB); }
        OPPONENT(SPECIES_KYOGRE) { Ability(ABILITY_DRIZZLE); }
    } WHEN {
        TURN { }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_BLUE_PRIMAL_REVERSION, player);
        MESSAGE("Kyogre's Primal Reversion! It reverted to its primal state!");
        
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_DRIZZLE);
            MESSAGE("It started to rain!");
        }
    }
}

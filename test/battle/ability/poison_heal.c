#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Poison Heal cures 1/8 of max HP when poisoned at end turn")
{
    u32 status;
    
    PARAMETRIZE { status = STATUS1_POISON; }
    PARAMETRIZE { status = STATUS1_TOXIC_POISON; }
    
    GIVEN {
        PLAYER(SPECIES_GLISCOR) { Ability(ABILITY_POISON_HEAL); Status1(status); MaxHP(176); HP(1); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_POISON_HEAL);
        MESSAGE("Gliscor gain some of its HP!");
        HP_BAR(player, damage: -22);
    }
}

SINGLE_BATTLE_TEST("Poison Heal prevents poison damage")
{
    u32 status;
    
    PARAMETRIZE { status = STATUS1_POISON; }
    PARAMETRIZE { status = STATUS1_TOXIC_POISON; }
    
    GIVEN {
        PLAYER(SPECIES_GLISCOR) { Ability(ABILITY_POISON_HEAL); Status1(status); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        NONE_OF {
            ABILITY_POPUP(player, ABILITY_POISON_HEAL);
            MESSAGE("Gliscor gain some of its HP!");
            HP_BAR(player);
        }
    }
}

SINGLE_BATTLE_TEST("Poison Heal doesn't prevents bad poison counter increase")
{
    GIVEN {
        PLAYER(SPECIES_GLISCOR) { Ability(ABILITY_POISON_HEAL); Status1(STATUS1_TOXIC_POISON); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        NOT HP_BAR(player);
    } THEN {
        EXPECT(player->status1.counter == 1);
    }
}

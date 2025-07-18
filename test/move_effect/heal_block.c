#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_HEAL_BLOCK].effect == EFFECT_HEAL_BLOCK);
}

SINGLE_BATTLE_TEST("Heal Block prevents Volt Absorb from restore HP")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_HEAL_BLOCK].type != TYPE_ELECTRIC);
        ASSUME(gBattleMoves[MOVE_THUNDER_SHOCK].type == TYPE_ELECTRIC);
        
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_JOLTEON) { Ability(ABILITY_VOLT_ABSORB); MaxHP(100); HP(1); Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_HEAL_BLOCK); }
        TURN { MOVE(player, MOVE_THUNDER_SHOCK); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_VOLT_ABSORB);
        NOT HP_BAR(opponent, damage: -25);
        MESSAGE("It doesn't affect Foe Jolteon…");
    }
}

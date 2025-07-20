#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Mega Launcher boosts the amount of HP restored by pulse moves")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_HEAL_PULSE].effect == EFFECT_HEAL_PULSE);
        ASSUME(gBattleMoves[MOVE_HEAL_PULSE].flags.pulseMove == TRUE);
        
        PLAYER(SPECIES_CLAUNCHER) { Ability(ABILITY_MEGA_LAUNCHER); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(100); HP(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_HEAL_PULSE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HEAL_PULSE, player);
        HP_BAR(opponent, damage: -75);
        MESSAGE("Foe Wobbuffet regained health!");
    }
}

TO_DO_BATTLE_TEST("Mega Launcher boosts damage of pulse moves by 1.5x");

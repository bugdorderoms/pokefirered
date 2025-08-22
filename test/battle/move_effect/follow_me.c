#include "global.h"
#include "test/battle.h"

DOUBLE_BATTLE_TEST("Follow Me redirect moves before Lightning Rod")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_THUNDER].type == TYPE_ELECTRIC);
        ASSUME(gBattleMoves[MOVE_THUNDER].target == MOVE_TARGET_SELECTED);
        ASSUME(gBattleMoves[MOVE_FOLLOW_ME].effect == EFFECT_FOLLOW_ME);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_RAICHU) { Ability(ABILITY_LIGHTNING_ROD); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponentRight, MOVE_FOLLOW_ME); MOVE(playerLeft, MOVE_THUNDER, target: opponentLeft); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FOLLOW_ME, opponentRight);
        
        NONE_OF {
            ABILITY_POPUP(opponentLeft, ABILITY_LIGHTNING_ROD);
            MESSAGE("Foe Raichu's Sp. Atk rose!");
        }
        HP_BAR(opponentRight);
    }
}

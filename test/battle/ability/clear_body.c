#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Clear Body prevents stat reduction")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SAND_ATTACK].effect == EFFECT_ACCURACY_DOWN);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_METANG) { Ability(ABILITY_CLEAR_BODY); }
    } WHEN {
        TURN { MOVE(player, MOVE_SAND_ATTACK); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SAND_ATTACK, player);
        ABILITY_POPUP(opponent, ABILITY_CLEAR_BODY);
        MESSAGE("Foe Metang prevents stat loss!");
    }
}

SINGLE_BATTLE_TEST("Clear Body does not prevents self-inflicted stat reduction")
{
    GIVEN {
        ASSUME(MoveHasMoveEffect(MOVE_SUPERPOWER, MOVE_EFFECT_ATK_DEF_DOWN, TRUE) == TRUE);
        
        PLAYER(SPECIES_METANG) { Ability(ABILITY_CLEAR_BODY); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SUPERPOWER); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUPERPOWER, player);
        
        NONE_OF {
            ABILITY_POPUP(player, ABILITY_CLEAR_BODY);
            MESSAGE("Metang prevents stat loss!");
        }
    }
}

TO_DO_BATTLE_TEST("Clear Body prevents Sticky Web's Speed drop");

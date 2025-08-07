#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Own Tempo prevents confusion")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_CONFUSE_RAY].effect == EFFECT_CONFUSE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_SLOWPOKE) { Ability(ABILITY_OWN_TEMPO); }
    } WHEN {
        TURN { MOVE(player, MOVE_CONFUSE_RAY); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_CONFUSE_RAY, player);
        ABILITY_POPUP(opponent, ABILITY_OWN_TEMPO);
        MESSAGE("Foe Slowpoke's Own Tempo prevents confusion!");
    }
}

SINGLE_BATTLE_TEST("Own Tempo cures confusion if confused")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_CONFUSE_RAY].effect == EFFECT_CONFUSE);
        
        PLAYER(SPECIES_PINSIR) { Ability(ABILITY_MOLD_BREAKER); }
        OPPONENT(SPECIES_SLOWPOKE) { Ability(ABILITY_OWN_TEMPO); }
    } WHEN {
        TURN { MOVE(player, MOVE_CONFUSE_RAY); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_OWN_TEMPO);
        MESSAGE("Foe Slowpoke's Own Tempo cured its confusion!");
    }
}

SINGLE_BATTLE_TEST("Own Tempo prevents the self-inflicted confusion from Thrash")
{
    GIVEN {
        ASSUME(MoveHasMoveEffect(MOVE_THRASH, MOVE_EFFECT_THRASH, TRUE) == TRUE);
        
        PLAYER(SPECIES_SLOWPOKE) { Ability(ABILITY_OWN_TEMPO); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_THRASH); }
        TURN { SKIP_TURN(player); }
        TURN { SKIP_TURN(player); }
        TURN { }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_THRASH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_THRASH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_THRASH, player);
        NOT MESSAGE("Slowpoke became confused due to fatigue!");
    }
}

TO_DO_BATTLE_TEST("A held berry will activate and cure the confusion before Own Tempo activates");

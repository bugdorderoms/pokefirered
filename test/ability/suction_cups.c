#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Suction Cups prevents Roar/Dragon Tail from forcing it switch out")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_ROAR; }
    PARAMETRIZE { move = MOVE_DRAGON_TAIL; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_ROAR].effect == EFFECT_RANDOM_SWITCH);
        ASSUME(gBattleMoves[MOVE_DRAGON_TAIL].effect == EFFECT_HIT_SWITCH_TARGET);
        
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_OCTILLERY) { Ability(ABILITY_SUCTION_CUPS); }
        OPPONENT(SPECIES_BULBASAUR);
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_SUCTION_CUPS);
        MESSAGE("Foe Octillery anchors itself with Suction Cups!");
        NOT MESSAGE("Foe Bulbasaur was dragged out!");
    }
}

SINGLE_BATTLE_TEST("Suction Cups doesn't prevents self switch out with moves")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_BATON_PASS; }
    PARAMETRIZE { move = MOVE_TELEPORT; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_BATON_PASS].effect == EFFECT_BATON_PASS);
        ASSUME(gBattleMoves[MOVE_TELEPORT].effect == EFFECT_TELEPORT);
        
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_OCTILLERY) { Ability(ABILITY_SUCTION_CUPS); }
        OPPONENT(SPECIES_BULBASAUR);
    } WHEN {
        TURN { MOVE(opponent, move); SEND_OUT(opponent, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, opponent);
        
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_SUCTION_CUPS);
            MESSAGE("Foe Octillery anchors itself with Suction Cups!");
        }
    }
}

SINGLE_BATTLE_TEST("Suction Cups doesn't prevents switching out manually")
{
    GIVEN {
        PLAYER(SPECIES_OCTILLERY) { Ability(ABILITY_SUCTION_CUPS); }
        PLAYER(SPECIES_BULBASAUR);
        
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { SWITCH(player, 1); }
    } SCENE {
        NONE_OF {
            ABILITY_POPUP(player, ABILITY_SUCTION_CUPS);
            MESSAGE("Octillery anchors itself with Suction Cups!");
        }
    }
}

TO_DO_BATTLE_TEST("Suction Cups prevents the Red Card from forcing it switch out, but it still is consumed");
TO_DO_BATTLE_TEST("Suction Cups doesn't prevent a held Eject Button from activating");

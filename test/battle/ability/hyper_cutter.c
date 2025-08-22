#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Hyper Cutter prevents Attack stat reduction")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_GROWL].effect == EFFECT_ATTACK_DOWN);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_KINGLER) { Ability(ABILITY_HYPER_CUTTER); }
    } WHEN {
        TURN { MOVE(player, MOVE_GROWL); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, player);
        ABILITY_POPUP(opponent, ABILITY_HYPER_CUTTER);
        MESSAGE("Foe Kingler prevents Attack loss!");
    }
}

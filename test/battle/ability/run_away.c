#include "global.h"
#include "test/battle.h"

WILD_BATTLE_TEST("Run Away prevents abilities from preventing Teleport")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_TELEPORT].effect == EFFECT_TELEPORT);
        
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_SHADOW_TAG); }
        OPPONENT(SPECIES_CATERPIE) { Ability(ABILITY_RUN_AWAY); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_TELEPORT); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TELEPORT, opponent);
        MESSAGE("Wild Caterpie fled from battle!");
        NOT MESSAGE("Wobbuffet's Shadow Tag made it ineffective!");
    }
}

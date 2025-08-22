#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Levitate protects against Ground-type attacks")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_EARTHQUAKE].type == TYPE_GROUND);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_HAUNTER) { Ability(ABILITY_LEVITATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_EARTHQUAKE); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_EARTHQUAKE, player);
        ABILITY_POPUP(opponent, ABILITY_LEVITATE);
        MESSAGE("It doesn't affect Foe Haunter…");
    }
}

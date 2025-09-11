#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Natural Gift's type isn't affected by Normalize, etc")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_NATURAL_GIFT].effect == EFFECT_NATURAL_GIFT);
        ASSUME(gSpeciesInfo[SPECIES_DIGLETT].types[0] == TYPE_GROUND);
        
        PLAYER(SPECIES_SKITTY) { Ability(ABILITY_NORMALIZE); Item(ITEM_PECHA_BERRY); }
        OPPONENT(SPECIES_DIGLETT);
    } WHEN {
        TURN { MOVE(player, MOVE_NATURAL_GIFT); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_NATURAL_GIFT, player);
        MESSAGE("It doesn't affect Foe Diglett…");
    }
}

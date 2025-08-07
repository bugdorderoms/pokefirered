#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_ACROBATICS].effect == EFFECT_ACROBATICS);
}

SINGLE_BATTLE_TEST("Acrobatics doubles in power if the user has no held item", s16 damage)
{
    u32 heldItem;
    
    PARAMETRIZE { heldItem = ITEM_POTION; }
    PARAMETRIZE { heldItem = ITEM_NONE; }
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Item(heldItem); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_ACROBATICS); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(2.0), results[1].damage);
    }
}

TO_DO_BATTLE_TEST("Acrobatics still doubles in power when Flying Gem is consumed");

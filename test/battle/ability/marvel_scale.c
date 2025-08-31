#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Marvel Scale boosts Defense by 50% if statused", s16 damage)
{
    u32 status;
    
    PARAMETRIZE { status = STATUS1_POISON; }
    PARAMETRIZE { status = STATUS1_NONE; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].split == SPLIT_PHYSICAL);
        
        PLAYER(SPECIES_DRATINI) { Ability(ABILITY_MARVEL_SCALE); Status1(status); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.5), results[1].damage);
    }
}

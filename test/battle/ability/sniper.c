#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Sniper boosts power by 1.5x if move is critical", s16 damage)
{
    bool32 crit;
    
    PARAMETRIZE { crit = FALSE; }
    PARAMETRIZE { crit = TRUE; }
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_REMORAID) { Ability(ABILITY_SNIPER); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH, criticalHit: crit); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(2.25), results[1].damage); // Crit + boost
    }
}

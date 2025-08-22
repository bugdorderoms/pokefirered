#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Huge Power doubles the user's Attack", s16 damage)
{
    u32 ability;
    
    PARAMETRIZE { ability = ABILITY_SAP_SIPPER; }
    PARAMETRIZE { ability = ABILITY_HUGE_POWER; }
    
    GIVEN {
        PLAYER(SPECIES_MARILL) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(2.0), results[1].damage);
    }
}

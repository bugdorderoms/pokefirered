#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Swarm boosts Bug-type moves by 50% in a pinch", s16 damage)
{
    u32 hp;
    
    PARAMETRIZE { hp = 99; }
    PARAMETRIZE { hp = 33; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SILVER_WIND].type == TYPE_BUG);
        
        PLAYER(SPECIES_LEDIAN) { Ability(ABILITY_SWARM); MaxHP(99); HP(hp); }
        OPPONENT(SPECIES_TOTODILE);
    } WHEN {
        TURN { MOVE(player, MOVE_SILVER_WIND); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.5), results[1].damage);
    }
}

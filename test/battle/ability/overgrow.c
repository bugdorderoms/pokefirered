#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Overgrow boosts Grass-type moves by 50% in a pinch", s16 damage)
{
    u32 hp;
    
    PARAMETRIZE { hp = 99; }
    PARAMETRIZE { hp = 33; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_VINE_WHIP].type == TYPE_GRASS);
        
        PLAYER(SPECIES_BULBASAUR) { Ability(ABILITY_OVERGROW); MaxHP(99); HP(hp); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_VINE_WHIP); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.5), results[1].damage);
    }
}

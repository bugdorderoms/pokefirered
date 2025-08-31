#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Blaze boosts Fire-type moves by 50% in a pinch", s16 damage)
{
    u32 hp;
    
    PARAMETRIZE { hp = 99; }
    PARAMETRIZE { hp = 33; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_FIRE_PUNCH].type == TYPE_FIRE);
        
        PLAYER(SPECIES_CHARMANDER) { Ability(ABILITY_BLAZE); MaxHP(99); HP(hp); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_FIRE_PUNCH); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.5), results[1].damage);
    }
}

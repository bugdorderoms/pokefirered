#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Technician boosts by 1.5x power of moves with 60 or lower power", s16 damage)
{
    u32 ability;
    
    PARAMETRIZE { ability = ABILITY_LIMBER; }
    PARAMETRIZE { ability = ABILITY_TECHNICIAN; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].power <= 60);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_PERSIAN) { Ability(ability); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.5), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Technician doesn't boosts confusion damage", s16 damage)
{
    u32 ability;
    
    PARAMETRIZE { ability = ABILITY_LIMBER; }
    PARAMETRIZE { ability = ABILITY_TECHNICIAN; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_CONFUSE_RAY].effect == EFFECT_CONFUSE);
        
        PLAYER(SPECIES_PERSIAN) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_CONFUSE_RAY); MOVE(player, MOVE_SPLASH, WITH_RNG(RNG_CONFUSION, TRUE)); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}

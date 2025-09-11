#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Adaptability increases same-type attack bonus from 1.5x to 2x", s16 damage)
{
    u32 ability;
    
    PARAMETRIZE { ability = ABILITY_HYPER_CUTTER; }
    PARAMETRIZE { ability = ABILITY_ADAPTABILITY; }
    
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_CRAWDAUNT].types[0] == TYPE_WATER || gSpeciesInfo[SPECIES_CRAWDAUNT].types[1] == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_WATER_GUN].type == TYPE_WATER);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CRAWDAUNT) { Ability(ability); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_WATER_GUN); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        // The jump from 1.5x STAB to 2.0x STAB is a 1.33x boost.
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.33), results[1].damage);
    }
}

TO_DO_BATTLE_TEST("Adaptability does not affect Stellar-type moves");

#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Tinted Lens doubles power of not very effective moves", s16 damage)
{
    u32 ability;
    
    PARAMETRIZE { ability = ABILITY_RUN_AWAY; }
    PARAMETRIZE { ability = ABILITY_TINTED_LENS; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].type == TYPE_NORMAL);
        ASSUME(gSpeciesInfo[SPECIES_REGIROCK].types[0] == TYPE_ROCK && gSpeciesInfo[SPECIES_REGIROCK].types[1] == TYPE_ROCK);
        
        PLAYER(SPECIES_REGIROCK);
        OPPONENT(SPECIES_VENONAT) { Ability(ability); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
        MESSAGE("It's not very effective…");
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(2.0), results[1].damage);
    }
}

#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_SCRATCH].split == SPLIT_PHYSICAL);
    ASSUME(gBattleMoves[MOVE_SCRATCH].type == TYPE_NORMAL);
    ASSUME(gSpeciesInfo[SPECIES_MACHOP].types[0] != TYPE_NORMAL && gSpeciesInfo[SPECIES_MACHOP].types[1] != TYPE_NORMAL);
}

SINGLE_BATTLE_TEST("Guts boosts Attack by 50% if statused", s16 damage)
{
    u32 status;
    
    PARAMETRIZE { status = STATUS1_NONE; }
    PARAMETRIZE { status = STATUS1_POISON; }
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_MACHOP) { Ability(ABILITY_GUTS); Status1(status); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.5), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Guts prevents burn's Attack reduction", s16 damage)
{
    u32 status;
    
    PARAMETRIZE { status = STATUS1_NONE; }
    PARAMETRIZE { status = STATUS1_BURN; }
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_MACHOP) { Ability(ABILITY_GUTS); Status1(status); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.5), results[1].damage); // No damage reduction
    }
}

#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Rivalry increases power by 1.25x towards Pokémon of the same gender", s16 damage)
{
    u32 ability;
    
    PARAMETRIZE { ability = ABILITY_UNNERVE; }
    PARAMETRIZE { ability = ABILITY_RIVALRY; }
    
    GIVEN {
        PLAYER(SPECIES_AXEW, MON_FEMALE);
        OPPONENT(SPECIES_AXEW, MON_FEMALE) { Ability(ability); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.25), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Rivalry decreases power by 0.75x towards Pokémon of different gender", s16 damage)
{
    u32 ability;
    
    PARAMETRIZE { ability = ABILITY_UNNERVE; }
    PARAMETRIZE { ability = ABILITY_RIVALRY; }
    
    GIVEN {
        PLAYER(SPECIES_AXEW, MON_MALE);
        OPPONENT(SPECIES_AXEW, MON_FEMALE) { Ability(ability); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(0.75), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Rivalry doesn't modify power if attacker is genderless", s16 damage)
{
    u32 ability;
    
    PARAMETRIZE { ability = ABILITY_POISON_POINT; }
    PARAMETRIZE { ability = ABILITY_RIVALRY; }
    
    GIVEN {
        PLAYER(SPECIES_NIDORINO) { Ability(ability); }
        OPPONENT(SPECIES_PORYGON) { Ability(ABILITY_TRACE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Rivalry doesn't modify power if target is genderless", s16 damage)
{
    u32 ability;
    
    PARAMETRIZE { ability = ABILITY_POISON_POINT; }
    PARAMETRIZE { ability = ABILITY_RIVALRY; }
    
    GIVEN {
        PLAYER(SPECIES_ARCEUS);
        OPPONENT(SPECIES_NIDORINO) { Ability(ability); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}

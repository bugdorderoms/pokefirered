#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Normalize turns a move into the Normal-type")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_WATER_GUN].type == TYPE_WATER);
        ASSUME(gSpeciesInfo[SPECIES_GASTLY].types[0] == TYPE_GHOST);
        
        PLAYER(SPECIES_SKITTY) { Ability(ABILITY_NORMALIZE); }
        OPPONENT(SPECIES_GASTLY);
    } WHEN {
        TURN { MOVE(player, MOVE_WATER_GUN); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_WATER_GUN, player);
        MESSAGE("It doesn't affect Foe Gastly…");
    }
}

SINGLE_BATTLE_TEST("Normalize boosts power of affected moves by 20%", s16 damage)
{
    u32 ability;
    
    PARAMETRIZE { ability = ABILITY_CUTE_CHARM; }
    PARAMETRIZE { ability = ABILITY_NORMALIZE; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_WATER_GUN].type == TYPE_WATER);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_SKITTY) { Ability(ability); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_WATER_GUN); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.8), results[1].damage); // STAB + boost
    }
}

TO_DO_BATTLE_TEST("Normalize makes Flying Press do Normal/Flying damage");
TO_DO_BATTLE_TEST("Normalize still makes Freeze-Dry do super effective damage to Water-type Pokémon");
TO_DO_BATTLE_TEST("Normalize-affected moves become Electric-type under Electrify and Ion Deluge's effect");
TO_DO_BATTLE_TEST("Normalize doesn't affect damaging Z-Moves");

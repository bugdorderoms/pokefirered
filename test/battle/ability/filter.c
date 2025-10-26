#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Filter/Solid Rock reduces damage to super effective moves by 0.75x", s16 damage)
{
    u32 j;
    u32 species, ability, move;
    static const u32 filterData[][4] =
    {
        {SPECIES_MR_MIME,  ABILITY_SOUNDPROOF,  ABILITY_FILTER,     MOVE_POISON_JAB},
        {SPECIES_CAMERUPT, ABILITY_MAGMA_ARMOR, ABILITY_SOLID_ROCK, MOVE_WATER_GUN},
    };
    
    for (j = 0; j < ARRAY_COUNT(filterData); j++)
    {
        PARAMETRIZE { species = filterData[j][0]; ability = filterData[j][1]; move = filterData[j][3]; }
        PARAMETRIZE { species = filterData[j][0]; ability = filterData[j][2]; move = filterData[j][3]; }
    }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_POISON_JAB].type == TYPE_POISON);
        ASSUME(gBattleMoves[MOVE_WATER_GUN].type == TYPE_WATER);
        ASSUME(gSpeciesInfo[SPECIES_MR_MIME].types[0] == TYPE_PSYCHIC && gSpeciesInfo[SPECIES_MR_MIME].types[1] == TYPE_FAIRY);
        ASSUME(gSpeciesInfo[SPECIES_CAMERUPT].types[0] == TYPE_FIRE && gSpeciesInfo[SPECIES_CAMERUPT].types[1] == TYPE_GROUND);
        
        PLAYER(species) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, move); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
        MESSAGE("It's super effective!");
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(0.75), results[1].damage);
        EXPECT_MUL_EQ(results[2].damage, UQ_4_12(0.75), results[3].damage);
    }
}

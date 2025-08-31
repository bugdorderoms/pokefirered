#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Huge Power/Pure Power doubles the user's Attack", s16 damage)
{
    u32 j;
    u32 species, ability, move;
    static const u32 hugePowerData[][2] =
    {
        {SPECIES_BUNNELBY,   ABILITY_HUGE_POWER},
        {SPECIES_MEDITITE,   ABILITY_PURE_POWER},
    };
    
    for (j = 0; j < ARRAY_COUNT(hugePowerData); j++)
    {
        PARAMETRIZE { species = hugePowerData[j][0]; ability = hugePowerData[j][1]; move = MOVE_WATER_GUN; }
        PARAMETRIZE { species = hugePowerData[j][0]; ability = hugePowerData[j][1]; move = MOVE_AQUA_JET; }
    }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_AQUA_JET].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_WATER_GUN].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_AQUA_JET].split == SPLIT_PHYSICAL);
        ASSUME(gBattleMoves[MOVE_WATER_GUN].split == SPLIT_SPECIAL);
        ASSUME(gBattleMoves[MOVE_AQUA_JET].power == gBattleMoves[MOVE_WATER_GUN].power);
        
        PLAYER(species) { Ability(ability); Attack(10); SpAttack(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Defense(10); SpDefense(10); }
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(2.0), results[1].damage);
        EXPECT_MUL_EQ(results[2].damage, UQ_4_12(2.0), results[3].damage);
    }
}

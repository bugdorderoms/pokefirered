#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Huge Power doubles the user's Attack", s16 damage)
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_WATER_GUN; }
    PARAMETRIZE { move = MOVE_AQUA_JET; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_AQUA_JET].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_WATER_GUN].type == TYPE_WATER);
        ASSUME(gSpeciesInfo[SPECIES_BUNNELBY].types[0] != TYPE_WATER && gSpeciesInfo[SPECIES_BUNNELBY].types[1] != TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_AQUA_JET].split == SPLIT_PHYSICAL);
        ASSUME(gBattleMoves[MOVE_WATER_GUN].split == SPLIT_SPECIAL);
        ASSUME(gBattleMoves[MOVE_AQUA_JET].power == gBattleMoves[MOVE_WATER_GUN].power);
        
        PLAYER(SPECIES_BUNNELBY) { Ability(ABILITY_HUGE_POWER); Attack(10); SpAttack(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Defense(10); SpDefense(10); }
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(2.0), results[1].damage);
    }
}

#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Hustle boosts the user's Attack by 50%", s16 damage)
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_WATER_GUN; }
    PARAMETRIZE { move = MOVE_AQUA_JET; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_AQUA_JET].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_WATER_GUN].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_AQUA_JET].split == SPLIT_PHYSICAL);
        ASSUME(gBattleMoves[MOVE_WATER_GUN].split == SPLIT_SPECIAL);
        ASSUME(gBattleMoves[MOVE_AQUA_JET].power == gBattleMoves[MOVE_WATER_GUN].power);
        
        PLAYER(SPECIES_RATTATA) { Ability(ABILITY_HUSTLE); Attack(10); SpAttack(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Defense(10); SpDefense(10); }
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.5), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Hustle lowers the accuracy of the user's physical attacks by 20%")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_SCRATCH; }
    PARAMETRIZE { move = MOVE_HYDRO_PUMP; }
    
    PASSES_RANDOMLY(80, 100, RNG_ACCURACY);
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].split == SPLIT_PHYSICAL);
        ASSUME(gBattleMoves[MOVE_SCRATCH].accuracy == 100);
        ASSUME(gBattleMoves[MOVE_HYDRO_PUMP].split == SPLIT_SPECIAL);
        ASSUME(gBattleMoves[MOVE_HYDRO_PUMP].accuracy == 80);
        
        PLAYER(SPECIES_RATTATA) { Ability(ABILITY_HUSTLE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        HP_BAR(opponent);
    }
}

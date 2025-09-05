#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Iron Fist boosts power of punching moves by 1.2x", s16 damage)
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_SLAM; }
    PARAMETRIZE { move = MOVE_MEGA_PUNCH; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SLAM].power == gBattleMoves[MOVE_MEGA_PUNCH].power);
        ASSUME(gBattleMoves[MOVE_SLAM].split == gBattleMoves[MOVE_MEGA_PUNCH].split);
        ASSUME(gBattleMoves[MOVE_SLAM].type == gBattleMoves[MOVE_MEGA_PUNCH].type);
        ASSUME(gBattleMoves[MOVE_SLAM].flags.punchMove == FALSE);
        ASSUME(gBattleMoves[MOVE_MEGA_PUNCH].flags.punchMove == TRUE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CHIMCHAR) { Ability(ABILITY_IRON_FIST); }
    } WHEN {
        TURN { MOVE(opponent, move); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.2), results[1].damage);
    }
}

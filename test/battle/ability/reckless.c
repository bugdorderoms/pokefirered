#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Reckless boosts by 20% the power of moves with recoil damage", s16 damage)
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_ROCK_CLIMB; }
    PARAMETRIZE { move = MOVE_TAKE_DOWN; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_ROCK_CLIMB].recoilDivisor == 0);
        ASSUME(gBattleMoves[MOVE_TAKE_DOWN].recoilDivisor > 0);
        ASSUME(gBattleMoves[MOVE_ROCK_CLIMB].power == gBattleMoves[MOVE_TAKE_DOWN].power);
        ASSUME(gBattleMoves[MOVE_ROCK_CLIMB].type == gBattleMoves[MOVE_TAKE_DOWN].type);
        ASSUME(gBattleMoves[MOVE_ROCK_CLIMB].split == gBattleMoves[MOVE_TAKE_DOWN].split);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_HITMONLEE) { Ability(ABILITY_RECKLESS); }
    } WHEN {
        TURN { MOVE(opponent, move); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.2), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Reckless doesn't boosts Struggle's power", s16 damage)
{
    u32 ability;
    
    PARAMETRIZE { ability = ABILITY_LIMBER; }
    PARAMETRIZE { ability = ABILITY_RECKLESS; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_STRUGGLE].recoilDivisor > 0);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_HITMONLEE) { Ability(ABILITY_RECKLESS); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_STRUGGLE); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Reckless boosts by 20% the power of moves with crash damage", s16 damage)
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_DYNAMIC_PUNCH; }
    PARAMETRIZE { move = MOVE_JUMP_KICK; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_DYNAMIC_PUNCH].effect == EFFECT_HIT);
        ASSUME(gBattleMoves[MOVE_JUMP_KICK].effect == EFFECT_RECOIL_IF_MISS);
        ASSUME(gBattleMoves[MOVE_DYNAMIC_PUNCH].power == gBattleMoves[MOVE_JUMP_KICK].power);
        ASSUME(gBattleMoves[MOVE_DYNAMIC_PUNCH].type == gBattleMoves[MOVE_JUMP_KICK].type);
        ASSUME(gBattleMoves[MOVE_DYNAMIC_PUNCH].split == gBattleMoves[MOVE_JUMP_KICK].split);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_HITMONLEE) { Ability(ABILITY_RECKLESS); }
    } WHEN {
        TURN { MOVE(opponent, move); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.2), results[1].damage);
    }
}

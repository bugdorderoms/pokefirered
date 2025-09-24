#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Unaware ignore target's accuracy stat")
{
    PASSES_RANDOMLY(85, 100, RNG_ACCURACY);

    GIVEN {
        ASSUME(gBattleMoves[MOVE_MEGA_PUNCH].accuracy == 85);
        ASSUME(gBattleMoves[MOVE_HONE_CLAWS].effect == EFFECT_ATTACK_ACCURACY_UP);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CLEFABLE) { Ability(ABILITY_UNAWARE); }
    } WHEN {
        TURN { MOVE(player, MOVE_HONE_CLAWS); }
        TURN { MOVE(player, MOVE_MEGA_PUNCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HONE_CLAWS, player);
        MESSAGE("Wobbuffet's accuracy rose!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MEGA_PUNCH, player);
    }
}

SINGLE_BATTLE_TEST("Unaware ignore target's evasion stat")
{
    PASSES_RANDOMLY(100, 100, RNG_ACCURACY);
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].accuracy == 100);
        ASSUME(gBattleMoves[MOVE_DOUBLE_TEAM].effect == EFFECT_EVASION_UP);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CLEFABLE) { Ability(ABILITY_UNAWARE); }
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_TEAM); MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_TEAM, player);
        MESSAGE("Wobbuffet's evasiveness rose!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
    }
}

SINGLE_BATTLE_TEST("Unaware ignore target's Defense and Sp. Defense stat", s16 damage)
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_SPLASH; }
    PARAMETRIZE { move = MOVE_HARDEN; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].split == SPLIT_PHYSICAL);
        ASSUME(gBattleMoves[MOVE_HARDEN].effect == EFFECT_DEFENSE_UP);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CLEFABLE) { Ability(ABILITY_UNAWARE); }
    } WHEN {
        TURN { MOVE(player, move); MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Unaware ignore target's Attack and Sp. Attack stat", s16 damage)
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_SPLASH; }
    PARAMETRIZE { move = MOVE_SHARPEN; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].split == SPLIT_PHYSICAL);
        ASSUME(gBattleMoves[MOVE_SHARPEN].effect == EFFECT_ATTACK_UP);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CLEFABLE) { Ability(ABILITY_UNAWARE); }
    } WHEN {
        TURN { MOVE(player, move); }
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}

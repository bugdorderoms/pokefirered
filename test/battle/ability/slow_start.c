#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Slow Start displays a message on switch in")
{
    GIVEN {
        PLAYER(SPECIES_REGIGIGAS) { Ability(ABILITY_SLOW_START); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_SLOW_START);
        MESSAGE("Regigigas can't get it going!");
    }
}

SINGLE_BATTLE_TEST("Slow Start displays a message when the timer ends")
{
    u32 j;
    
    GIVEN {
        PLAYER(SPECIES_REGIGIGAS) { Ability(ABILITY_SLOW_START); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        for (j = 0; j < 5; j++) {
            TURN { }
        }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_SLOW_START);
        MESSAGE("Regigigas got its act together!");
    }
}

SINGLE_BATTLE_TEST("Slow Start halves Attack for five turns")
{
    u32 j;
    s16 damage[6];
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].split == SPLIT_PHYSICAL);
        
        PLAYER(SPECIES_WOBBUFFET) { Defense(200); }
        OPPONENT(SPECIES_REGIGIGAS) { Ability(ABILITY_SLOW_START); Attack(5); }
    } WHEN {
        for (j = 0; j < 6; j++) {
            TURN { MOVE(opponent, MOVE_SCRATCH); }
        }
    } SCENE {
        for (j = 0; j < 6; j++) {
            HP_BAR(player, captureDamage: &damage[j]);
        }
    } THEN {
        for (j = 0; j < 5; j++) {
            EXPECT_MUL_EQ(damage[5], UQ_4_12(0.5), damage[j]);
        }
    }
}

SINGLE_BATTLE_TEST("Slow Start halves Speed for five turns")
{
    u32 j;
    
    GIVEN {
        PLAYER(SPECIES_REGIGIGAS) { Ability(ABILITY_SLOW_START); Speed(199); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(100); }
    } WHEN {
        for (j = 0; j < 5; j++) {
            TURN { MOVE(opponent, MOVE_SPLASH); MOVE(player, MOVE_SPLASH); }
        }
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
    } SCENE {
        for (j = 0; j < 5; j++) {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
        }
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);
    }
}

TO_DO_BATTLE_TEST("Slow Start halves Attack of special Z-Moves");

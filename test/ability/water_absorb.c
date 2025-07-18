#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Water Absorb heals 25% when hit by water type moves")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_WATER_GUN; }
    PARAMETRIZE { move = MOVE_SOAK; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_WATER_GUN].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_SOAK].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_SOAK].split == SPLIT_STATUS);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_LAPRAS) { Ability(ABILITY_WATER_ABSORB); MaxHP(100); HP(1); }
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_WATER_ABSORB);
        HP_BAR(opponent, damage: -25);
        MESSAGE("Foe Lapras had its HP restored.");
    }
}

SINGLE_BATTLE_TEST("Water Absorb does not activate if protected")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_WATER_GUN; }
    PARAMETRIZE { move = MOVE_SOAK; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_WATER_GUN].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_SOAK].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_SOAK].split == SPLIT_STATUS);
        ASSUME(gBattleMoves[MOVE_PROTECT].effect == EFFECT_PROTECT);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_LAPRAS) { Ability(ABILITY_WATER_ABSORB); MaxHP(100); HP(1); }
    } WHEN {
        TURN { MOVE(player, move); MOVE(opponent, MOVE_PROTECT); }
    } SCENE {
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_WATER_ABSORB);
            HP_BAR(opponent);
            MESSAGE("Foe Lapras had its HP restored.");
        }
    }
}

SINGLE_BATTLE_TEST("Water Absorb makes move useless when its HP is full")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_WATER_GUN; }
    PARAMETRIZE { move = MOVE_SOAK; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_WATER_GUN].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_SOAK].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_SOAK].split == SPLIT_STATUS);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_LAPRAS) { Ability(ABILITY_WATER_ABSORB); MaxHP(100); HP(100); }
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_WATER_ABSORB);
        NOT HP_BAR(opponent, damage: -25);
        MESSAGE("It doesn't affect Foe Lapras…");
    }
}

SINGLE_BATTLE_TEST("Water Absorb is only triggered once on multi strike moves")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_WATER_SHURIKEN].effect == EFFECT_MULTI_HIT);
        ASSUME(gBattleMoves[MOVE_WATER_SHURIKEN].type == TYPE_WATER);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_LAPRAS) { Ability(ABILITY_WATER_ABSORB); MaxHP(100); HP(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_WATER_SHURIKEN); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_WATER_ABSORB);
        HP_BAR(opponent, damage: -25);
        MESSAGE("Foe Lapras had its HP restored.");
    }
}

TO_DO_BATTLE_TEST("Water Absorb activates before a held Absorb Bulb");

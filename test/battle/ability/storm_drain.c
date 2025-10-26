#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Storm Drain absorbs Water-type moves and boost Sp. Attack")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_HYDRO_PUMP].type == TYPE_WATER);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_LILEEP) { Ability(ABILITY_STORM_DRAIN); }
    } WHEN {
        TURN { MOVE(player, MOVE_HYDRO_PUMP); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_HYDRO_PUMP, player);
        ABILITY_POPUP(opponent, ABILITY_STORM_DRAIN);
        MESSAGE("Foe Lileep's Sp. Atk rose!");
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_SPATK], DEFAULT_STAT_STAGES + 1);
    }
}

DOUBLE_BATTLE_TEST("Storm Drain draws in single-target Water-type moves")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_HYDRO_PUMP].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_HYDRO_PUMP].target == MOVE_TARGET_SELECTED);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_LILEEP) { Ability(ABILITY_STORM_DRAIN); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_HYDRO_PUMP, target: opponentRight);
            MOVE(playerRight, MOVE_HYDRO_PUMP, target: opponentRight);
        }
    } SCENE {
        NOT HP_BAR(opponentRight);
        
        ABILITY_POPUP(opponentLeft, ABILITY_STORM_DRAIN);
        MESSAGE("Foe Lileep's Sp. Atk rose!");
        
        ABILITY_POPUP(opponentLeft, ABILITY_STORM_DRAIN);
        MESSAGE("Foe Lileep's Sp. Atk rose!");
    }
}

DOUBLE_BATTLE_TEST("Storm Drain redirects an ally's attack")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_HYDRO_PUMP].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_HYDRO_PUMP].target == MOVE_TARGET_SELECTED);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_LILEEP) { Ability(ABILITY_STORM_DRAIN); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponentRight, MOVE_HYDRO_PUMP, target: playerLeft); }
    } SCENE {
        NOT HP_BAR(playerLeft);
        ABILITY_POPUP(opponentLeft, ABILITY_STORM_DRAIN);
        MESSAGE("Foe Lileep took the attack!");
        MESSAGE("Foe Lileep's Sp. Atk rose!");
    }
}

DOUBLE_BATTLE_TEST("Storm Drain redirected moves will never misses")
{
    PASSES_RANDOMLY(100, 100, RNG_ACCURACY);
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_HYDRO_PUMP].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_HYDRO_PUMP].target == MOVE_TARGET_SELECTED);
        ASSUME(gBattleMoves[MOVE_HYDRO_PUMP].accuracy != 100);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_LILEEP) { Ability(ABILITY_STORM_DRAIN); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_HYDRO_PUMP, target: opponentRight); }
    } SCENE {
        ABILITY_POPUP(opponentLeft, ABILITY_STORM_DRAIN);
        MESSAGE("Foe Lileep's Sp. Atk rose!");
    }
}

SINGLE_BATTLE_TEST("Storm Drain is only triggered once on multi strike moves")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_WATER_SHURIKEN].effect == EFFECT_MULTI_HIT);
        
        PLAYER(SPECIES_LILEEP) { Ability(ABILITY_STORM_DRAIN); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_WATER_SHURIKEN); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_STORM_DRAIN);
        MESSAGE("Lileep's Sp. Atk rose!");
    }
}

DOUBLE_BATTLE_TEST("Storm Drain does not activate if protected")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_HYDRO_PUMP].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_PROTECT].effect == EFFECT_PROTECT);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_LILEEP) { Ability(ABILITY_STORM_DRAIN); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_PROTECT); MOVE(playerLeft, MOVE_HYDRO_PUMP, target: opponentRight); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PROTECT, opponentLeft);
        ABILITY_POPUP(opponentLeft, ABILITY_STORM_DRAIN);
        MESSAGE("Foe Lileep took the attack!");
        NOT MESSAGE("Foe Lileep's Sp. Atk rose!");
    }
}

TO_DO_BATTLE_TEST("Storm Drain absorbs the move even if its immune to it");
TO_DO_BATTLE_TEST("Storm Drain can't redirect moves if its out of range in triples");
TO_DO_BATTLE_TEST("Storm Drain activates before a held Absorb Bulb");

#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Volt Absorb heals 25% when hit by electric type moves")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_THUNDER_SHOCK; }
    PARAMETRIZE { move = MOVE_THUNDER_WAVE; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_THUNDER_SHOCK].type == TYPE_ELECTRIC);
        ASSUME(gBattleMoves[MOVE_THUNDER_WAVE].type == TYPE_ELECTRIC);
        ASSUME(gBattleMoves[MOVE_THUNDER_WAVE].split == SPLIT_STATUS);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_JOLTEON) { Ability(ABILITY_VOLT_ABSORB); MaxHP(100); HP(1); }
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_VOLT_ABSORB);
        HP_BAR(opponent, damage: -25);
        MESSAGE("Foe Jolteon had its HP restored.");
    }
}

SINGLE_BATTLE_TEST("Volt Absorb does not activate if protected")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_THUNDER_SHOCK; }
    PARAMETRIZE { move = MOVE_THUNDER_WAVE; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_THUNDER_SHOCK].type == TYPE_ELECTRIC);
        ASSUME(gBattleMoves[MOVE_THUNDER_WAVE].type == TYPE_ELECTRIC);
        ASSUME(gBattleMoves[MOVE_THUNDER_WAVE].split == SPLIT_STATUS);
        ASSUME(gBattleMoves[MOVE_PROTECT].effect == EFFECT_PROTECT);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_JOLTEON) { Ability(ABILITY_VOLT_ABSORB); MaxHP(100); HP(1); }
    } WHEN {
        TURN { MOVE(player, move); MOVE(opponent, MOVE_PROTECT); }
    } SCENE {
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_VOLT_ABSORB);
            HP_BAR(opponent);
            MESSAGE("Foe Jolteon had its HP restored.");
        }
    }
}

SINGLE_BATTLE_TEST("Volt Absorb makes move useless when its HP is full")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_THUNDER_SHOCK; }
    PARAMETRIZE { move = MOVE_THUNDER_WAVE; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_THUNDER_SHOCK].type == TYPE_ELECTRIC);
        ASSUME(gBattleMoves[MOVE_THUNDER_WAVE].type == TYPE_ELECTRIC);
        ASSUME(gBattleMoves[MOVE_THUNDER_WAVE].split == SPLIT_STATUS);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_JOLTEON) { Ability(ABILITY_VOLT_ABSORB); MaxHP(100); HP(100); }
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_VOLT_ABSORB);
        NOT HP_BAR(opponent, damage: -25);
        MESSAGE("It doesn't affect Foe Jolteon…");
    }
}

SINGLE_BATTLE_TEST("Volt Absorb is only triggered once on multi strike moves")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_FURY_SWIPES].type == TYPE_NORMAL);
        ASSUME(gBattleMoves[MOVE_FURY_SWIPES].effect == EFFECT_MULTI_HIT);
        
        PLAYER(SPECIES_JOLTEON) { Ability(ABILITY_VOLT_ABSORB); MaxHP(100); HP(1); }
        OPPONENT(SPECIES_GRAVELER_ALOLAN) { Ability(ABILITY_GALVANIZE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_FURY_SWIPES); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_VOLT_ABSORB);
        HP_BAR(player, damage: -25);
        MESSAGE("Jolteon had its HP restored.");
    }
}

TO_DO_BATTLE_TEST("Volt Absorb activates before a held Cell Battery");

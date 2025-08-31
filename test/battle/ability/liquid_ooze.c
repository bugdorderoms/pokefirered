#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Liquid Ooze causes Absorb user to lose HP instead of heal")
{
    s16 damage;
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_ABSORB].effect == EFFECT_HIT_ABSORB);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_TENTACOOL) { Ability(ABILITY_LIQUID_OOZE); }
    } WHEN {
        TURN { MOVE(player, MOVE_ABSORB); }
    } SCENE {
        HP_BAR(player, captureDamage: &damage);
        ABILITY_POPUP(opponent, ABILITY_LIQUID_OOZE);
        MESSAGE("It sucked up the Liquid Ooze!");
    } THEN {
        EXPECT_GE(damage, 0);
    }
}

SINGLE_BATTLE_TEST("Liquid Ooze causes Dream Eater user to lose HP instead of heal")
{
    s16 damage;
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_DREAM_EATER].effect == EFFECT_DREAM_EATER);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_TENTACOOL) { Ability(ABILITY_LIQUID_OOZE); Status1(STATUS1_SLEEP); }
    } WHEN {
        TURN { MOVE(player, MOVE_DREAM_EATER); }
    } SCENE {
        HP_BAR(player, captureDamage: &damage);
        ABILITY_POPUP(opponent, ABILITY_LIQUID_OOZE);
        MESSAGE("It sucked up the Liquid Ooze!");
    } THEN {
        EXPECT_GE(damage, 0);
    }
}

SINGLE_BATTLE_TEST("Liquid Ooze causes Leech Seed user to lose HP instead of heal")
{
    s16 damage;
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_LEECH_SEED].effect == EFFECT_SET_SEEDED);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_TENTACOOL) { Ability(ABILITY_LIQUID_OOZE); }
    } WHEN {
        TURN { MOVE(player, MOVE_LEECH_SEED); }
    } SCENE {
        HP_BAR(player, captureDamage: &damage);
        ABILITY_POPUP(opponent, ABILITY_LIQUID_OOZE);
        MESSAGE("It sucked up the Liquid Ooze!");
    } THEN {
        EXPECT_GE(damage, 0);
    }
}

SINGLE_BATTLE_TEST("Liquid Ooze causes leech seed victim to faint before seeder")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_LEECH_SEED].effect == EFFECT_SET_SEEDED);
        
        PLAYER(SPECIES_BULBASAUR) { HP(1); }
        OPPONENT(SPECIES_TENTACOOL) { HP(1); Ability(ABILITY_LIQUID_OOZE); }
    } WHEN {
        TURN { MOVE(player, MOVE_LEECH_SEED); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_LIQUID_OOZE);
        MESSAGE("It sucked up the Liquid Ooze!");
        MESSAGE("Foe Tentacool fainted!");
        MESSAGE("Bulbasaur fainted!");
    }
}

TO_DO_BATTLE_TEST("Liquid Ooze causes Strength Sap user to lose HP instead of heal");

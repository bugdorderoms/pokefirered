#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_SUNNY_DAY].effect == EFFECT_SET_WEATHER);
    ASSUME(gBattleMoves[MOVE_SUNNY_DAY].argument.setWeather.weatherId == ENUM_WEATHER_SUN);
}

SINGLE_BATTLE_TEST("Solar Power loses 1/8 of max HP in sun every end turn")
{
    GIVEN {
        PLAYER(SPECIES_CHARIZARD) { Ability(ABILITY_SOLAR_POWER); HP(128); MaxHP(128); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUNNY_DAY); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_SOLAR_POWER);
        MESSAGE("Charizard lost some of its HP!");
        HP_BAR(player, damage: 16);
    }
}

SINGLE_BATTLE_TEST("Solar Power increases Sp. Attack by 1.5x in sun", s16 damage)
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_SPLASH; }
    PARAMETRIZE { move = MOVE_SUNNY_DAY; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_HYPER_VOICE].split == SPLIT_SPECIAL);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CHARIZARD) { Ability(ABILITY_SOLAR_POWER); }
    } WHEN {
        TURN { MOVE(player, move); MOVE(opponent, MOVE_HYPER_VOICE); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.5), results[1].damage);
    }
}

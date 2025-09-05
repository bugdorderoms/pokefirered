#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_HAIL].effect == EFFECT_SET_WEATHER);
    ASSUME(gBattleMoves[MOVE_HAIL].argument.setWeather.weatherId == ENUM_WEATHER_HAIL);
}

SINGLE_BATTLE_TEST("Snow Cloak prevents hail damage")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SKILL_SWAP].effect == EFFECT_SKILL_SWAP);
        
        PLAYER(SPECIES_ARTICUNO) { Ability(ABILITY_SNOW_CLOAK); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_HAIL); MOVE(opponent, MOVE_SKILL_SWAP); }
    } SCENE {
        NOT HP_BAR(opponent);
    }
}

SINGLE_BATTLE_TEST("Snow Cloak increases evasion during hail")
{
    PASSES_RANDOMLY(4, 5, RNG_ACCURACY);
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].accuracy == 100);
        
        PLAYER(SPECIES_ARTICUNO) { Ability(ABILITY_SNOW_CLOAK); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_HAIL); }
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        HP_BAR(player);
    }
}

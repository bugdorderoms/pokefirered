#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_SANDSTORM].effect == EFFECT_SET_WEATHER);
    ASSUME(gBattleMoves[MOVE_SANDSTORM].argument.setWeather.weatherId == ENUM_WEATHER_SANDSTORM);
}

SINGLE_BATTLE_TEST("Sand Veil prevents sandstorm damage")
{
    GIVEN {
        PLAYER(SPECIES_CACNEA) { Ability(ABILITY_SAND_VEIL); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SANDSTORM); }
        TURN {}
    } SCENE {
        NOT HP_BAR(player);
    }
}

SINGLE_BATTLE_TEST("Sand Veil increases evasion during sandstorm")
{
    PASSES_RANDOMLY(4, 5, RNG_ACCURACY);
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].accuracy == 100);
        
        PLAYER(SPECIES_SANDSHREW) { Ability(ABILITY_SAND_VEIL); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SANDSTORM); }
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        HP_BAR(player);
    }
}

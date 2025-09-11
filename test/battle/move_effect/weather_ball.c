#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Weather Ball's type isn't affected by Normalize, etc")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SUNNY_DAY].effect == EFFECT_SET_WEATHER);
        ASSUME(gBattleMoves[MOVE_SUNNY_DAY].argument.setWeather.weatherId == ENUM_WEATHER_SUN);
        ASSUME(gBattleMoves[MOVE_WEATHER_BALL].effect == EFFECT_WEATHER_BALL);
        ASSUME(gSpeciesInfo[SPECIES_MEGANIUM].types[0] == TYPE_GRASS);
        
        PLAYER(SPECIES_SKITTY) { Ability(ABILITY_NORMALIZE); }
        OPPONENT(SPECIES_MEGANIUM);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUNNY_DAY); MOVE(player, MOVE_WEATHER_BALL); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WEATHER_BALL, player);
        MESSAGE("It's super effective!");
    }
}

#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Chlorophyll doubles speed in sun")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SUNNY_DAY].effect == EFFECT_SET_WEATHER);
        ASSUME(gBattleMoves[MOVE_SUNNY_DAY].argument.setWeather.weatherId == ENUM_WEATHER_SUN);
        
        PLAYER(SPECIES_BULBASAUR) { Ability(ABILITY_CHLOROPHYLL); Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(199); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SPLASH); MOVE(player, MOVE_SUNNY_DAY); }
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUNNY_DAY, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);
    }
}

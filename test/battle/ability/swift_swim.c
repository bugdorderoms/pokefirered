#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Swift Swim doubles speed in rain")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_RAIN_DANCE].effect == EFFECT_SET_WEATHER);
        ASSUME(gBattleMoves[MOVE_RAIN_DANCE].argument.setWeather.weatherId == ENUM_WEATHER_RAIN);
        
        PLAYER(SPECIES_PSYDUCK) { Ability(ABILITY_SWIFT_SWIM); Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(199); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SPLASH); MOVE(player, MOVE_RAIN_DANCE); }
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_RAIN_DANCE, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);
    }
}

#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Hydration cures status in rain at end turn")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_RAIN_DANCE].effect == EFFECT_SET_WEATHER);
        ASSUME(gBattleMoves[MOVE_RAIN_DANCE].argument.setWeather.weatherId == ENUM_WEATHER_RAIN);
        
        PLAYER(SPECIES_VAPOREON) { Ability(ABILITY_HYDRATION); Status1(STATUS1_BURN); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_RAIN_DANCE); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_HYDRATION);
        MESSAGE("Vaporeon's Hydration cured its burn problem!");
        STATUS_ICON(player, burn: FALSE);
    }
}

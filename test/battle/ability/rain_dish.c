#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Rain Dish recovers 1/16th of Max HP in Rain every end turn")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_RAIN_DANCE].effect == EFFECT_SET_WEATHER);
        ASSUME(gBattleMoves[MOVE_RAIN_DANCE].argument.setWeather.weatherId == ENUM_WEATHER_RAIN);
        
        PLAYER(SPECIES_LUDICOLO) { Ability(ABILITY_RAIN_DISH); HP(1); MaxHP(128); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_RAIN_DANCE); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_RAIN_DISH);
        MESSAGE("Ludicolo gain some of its HP!");
        HP_BAR(player, damage: -8);
    }
}

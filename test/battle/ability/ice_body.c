#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_HAIL].effect == EFFECT_SET_WEATHER);
    ASSUME(gBattleMoves[MOVE_HAIL].argument.setWeather.weatherId == ENUM_WEATHER_HAIL);
}

SINGLE_BATTLE_TEST("Ice Body prevents hail damage")
{
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_SEEL].types[0] == TYPE_WATER && gSpeciesInfo[SPECIES_SEEL].types[1] == TYPE_WATER);
        
        PLAYER(SPECIES_SEEL) { Ability(ABILITY_ICE_BODY); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_HAIL); }
    } SCENE {
        NOT HP_BAR(player);
    }
}

SINGLE_BATTLE_TEST("Ice Body recovers 1/16th of Max HP in hail every end turn")
{
    GIVEN {
        PLAYER(SPECIES_SEEL) { Ability(ABILITY_ICE_BODY); HP(1); MaxHP(128); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_HAIL); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_ICE_BODY);
        MESSAGE("Seel gain some of its HP!");
        HP_BAR(player, damage: -8);
    }
}

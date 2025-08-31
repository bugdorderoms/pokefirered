#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Forecast transforms Castform in weather")
{
    GIVEN {
        PLAYER(SPECIES_CASTFORM) { Ability(ABILITY_FORECAST); }
        OPPONENT(SPECIES_ABOMASNOW) { Ability(ABILITY_SNOW_WARNING); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_FORECAST);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Castform transformed!");
    } THEN {
        EXPECT_EQ(player->species, SPECIES_CASTFORM_SNOWY);
    }
}

SINGLE_BATTLE_TEST("Forecast transforms Castform back when the weather ends")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SUNNY_DAY].effect == EFFECT_SET_WEATHER);
        ASSUME(gBattleMoves[MOVE_SUNNY_DAY].argument.setWeather.weatherId == ENUM_WEATHER_SUN);
        
        PLAYER(SPECIES_CASTFORM) { Ability(ABILITY_FORECAST); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUNNY_DAY); }
        TURN { }
        TURN { }
        TURN { }
        TURN { }
    } SCENE {
        // transforms
        ABILITY_POPUP(player, ABILITY_FORECAST);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Castform transformed!");
        // back to normal
        ABILITY_POPUP(player, ABILITY_FORECAST);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Castform transformed!");
    } THEN {
        EXPECT_EQ(player->species, SPECIES_CASTFORM);
    }
}

SINGLE_BATTLE_TEST("Forecast transforms Castform back under Cloud Nine/Air Lock")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_PSYDUCK; ability = ABILITY_CLOUD_NINE; }
    PARAMETRIZE { species = SPECIES_RAYQUAZA; ability = ABILITY_AIR_LOCK; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_RAIN_DANCE].effect == EFFECT_SET_WEATHER);
        ASSUME(gBattleMoves[MOVE_RAIN_DANCE].argument.setWeather.weatherId == ENUM_WEATHER_RAIN);
        
        PLAYER(SPECIES_CASTFORM) { Ability(ABILITY_FORECAST); }
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(species) { Ability(ability); }
    } WHEN {
        TURN { MOVE(player, MOVE_RAIN_DANCE); }
        TURN { SWITCH(opponent, 1); }
    } SCENE {
        // transforms
        ABILITY_POPUP(player, ABILITY_FORECAST);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Castform transformed!");
        // back to normal
        ABILITY_POPUP(opponent, ability);
        ABILITY_POPUP(player, ABILITY_FORECAST);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Castform transformed!");
    } THEN {
        EXPECT_EQ(player->species, SPECIES_CASTFORM);
    }
}

SINGLE_BATTLE_TEST("Forecast transforms Castform back when its replaced")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_ROLE_PLAY].effect == EFFECT_ROLE_PLAY);
        
        PLAYER(SPECIES_CASTFORM) { Ability(ABILITY_FORECAST); }
        OPPONENT(SPECIES_ABOMASNOW) { Ability(ABILITY_SNOW_WARNING); }
    } WHEN {
        TURN { MOVE(player, MOVE_ROLE_PLAY); }
    } SCENE {
        // transforms
        ABILITY_POPUP(player, ABILITY_FORECAST);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Castform transformed!");
        // back to normal
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Castform transformed!");
    } THEN {
        EXPECT_EQ(player->species, SPECIES_CASTFORM);
    }
}

SINGLE_BATTLE_TEST("Forecast transforms Castform back when its suppressed")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SUNNY_DAY].effect == EFFECT_SET_WEATHER);
        ASSUME(gBattleMoves[MOVE_SUNNY_DAY].argument.setWeather.weatherId == ENUM_WEATHER_SUN);
        
        PLAYER(SPECIES_CASTFORM) { Ability(ABILITY_FORECAST); }
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_KOFFING) { Ability(ABILITY_NEUTRALIZING_GAS); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUNNY_DAY); }
        TURN { SWITCH(opponent, 1); }
    } SCENE {
        // transforms
        ABILITY_POPUP(player, ABILITY_FORECAST);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Castform transformed!");
        // back to normal
        ABILITY_POPUP(opponent, ABILITY_NEUTRALIZING_GAS);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Castform transformed!");
    } THEN {
        EXPECT_EQ(player->species, SPECIES_CASTFORM);
    }
}

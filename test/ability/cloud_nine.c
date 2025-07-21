#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Cloud Nine/Air Lock displays a message on switch in")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_PSYDUCK; ability = ABILITY_CLOUD_NINE; }
    PARAMETRIZE { species = SPECIES_RAYQUAZA; ability = ABILITY_AIR_LOCK; }
    
    GIVEN {
        PLAYER(species) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ability);
        MESSAGE("The effects of weather disappeared!");
    }
}

DOUBLE_BATTLE_TEST("Cloud Nine/Air Lock prevent weather effects, but without them disappearing")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_PSYDUCK; ability = ABILITY_CLOUD_NINE; }
    PARAMETRIZE { species = SPECIES_RAYQUAZA; ability = ABILITY_AIR_LOCK; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_THUNDER].effect == EFFECT_NEVER_MISS_IN_WEATHER);
        ASSUME(gBattleMoves[MOVE_THUNDER].argument.neverMissInWeather.weather == B_WEATHER_RAIN_ANY);
        
        PLAYER(species) { Ability(ability); }
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_POLITOED) { Ability(ABILITY_DRIZZLE); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_THUNDER, WITH_RNG(RNG_ACCURACY, FALSE), target: playerRight); }
    } SCENE {
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_THUNDER, opponentLeft);
            HP_BAR(playerRight);
        }
        MESSAGE("Rain continues to fall.");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RAIN_CONTINUES);
    }
}

#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Mold Breaker, Teravolt and Turboblaze displays a message on switch in")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_PINSIR; ability = ABILITY_MOLD_BREAKER; }
    PARAMETRIZE { species = SPECIES_ZEKROM; ability = ABILITY_TERAVOLT; }
    PARAMETRIZE { species = SPECIES_RESHIRAM; ability = ABILITY_TURBOBLAZE; }
    
    GIVEN {
        PLAYER(species) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ability);
        
        switch (ability)
        {
            case ABILITY_MOLD_BREAKER:
                MESSAGE("Pinsir breaks the mold!");
                break;
            case ABILITY_TERAVOLT:
                MESSAGE("Zekrom is radiating a bursting aura!");
                break;
            case ABILITY_TURBOBLAZE:
                MESSAGE("Reshiram is radiating a blazing aura!");
                break;
        }
    }
}

SINGLE_BATTLE_TEST("Mold Breaker, Teravolt and Turboblaze ignore target abilities")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_PINSIR; ability = ABILITY_MOLD_BREAKER; }
    PARAMETRIZE { species = SPECIES_ZEKROM; ability = ABILITY_TERAVOLT; }
    PARAMETRIZE { species = SPECIES_RESHIRAM; ability = ABILITY_TURBOBLAZE; }
    
    GIVEN {
        PLAYER(species) { Ability(ability); }
        OPPONENT(SPECIES_KINGLER) { Ability(ABILITY_SHELL_ARMOR); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, criticalHit: TRUE); }
    } SCENE {
        MESSAGE("{COLOR 2}{SHADOW 9}A critical hit!");
    }
}

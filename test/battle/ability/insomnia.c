#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Insomnia/Vital Spirit prevents sleep")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_DROWZEE; ability = ABILITY_INSOMNIA; }
    PARAMETRIZE { species = SPECIES_MANKEY; ability = ABILITY_VITAL_SPIRIT; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SPORE].effect == EFFECT_SLEEP);

        PLAYER(species) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SPORE); }
    } SCENE {
        ABILITY_POPUP(player, ability);
        
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SPORE, opponent);
            STATUS_ICON(player, sleep: TRUE);
        }
    }
}

SINGLE_BATTLE_TEST("Insomnia/Vital Spirit prevents yawn")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_DROWZEE; ability = ABILITY_INSOMNIA; }
    PARAMETRIZE { species = SPECIES_MANKEY; ability = ABILITY_VITAL_SPIRIT; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_YAWN].effect == EFFECT_YAWN);
        
        PLAYER(species) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_YAWN); }
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ability);
        
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_YAWN, opponent);
            STATUS_ICON(player, sleep: TRUE);
        }
    }
}

SINGLE_BATTLE_TEST("Insomnia/Vital Spirit prevents a drowsy pokemon fall asleep")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_DROWZEE; ability = ABILITY_INSOMNIA; }
    PARAMETRIZE { species = SPECIES_MANKEY; ability = ABILITY_VITAL_SPIRIT; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_YAWN].effect == EFFECT_YAWN);
        
        PLAYER(SPECIES_PINSIR) { Ability(ABILITY_MOLD_BREAKER); }
        OPPONENT(species) { Ability(ability); }
    } WHEN {
        TURN { MOVE(player, MOVE_YAWN); }
        TURN { }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_YAWN, player);
        NOT STATUS_ICON(opponent, sleep: TRUE);
    }
}

SINGLE_BATTLE_TEST("Insomnia/Vital Spirit prevents rest")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_DROWZEE; ability = ABILITY_INSOMNIA; }
    PARAMETRIZE { species = SPECIES_MANKEY; ability = ABILITY_VITAL_SPIRIT; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_REST].effect == EFFECT_REST);
        
        PLAYER(species) { Ability(ability); HP(1); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_REST); }
    } SCENE {
        ABILITY_POPUP(player, ability);
        
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_REST, player);
            STATUS_ICON(player, sleep: TRUE);
            HP_BAR(player);
        }
    }
}

SINGLE_BATTLE_TEST("Insomnia/Vital Spirit wakes it up if its sleeping")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_DROWZEE; ability = ABILITY_INSOMNIA; }
    PARAMETRIZE { species = SPECIES_MANKEY; ability = ABILITY_VITAL_SPIRIT; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SPORE].effect == EFFECT_SLEEP);
        
        PLAYER(SPECIES_PINSIR) { Ability(ABILITY_MOLD_BREAKER); }
        OPPONENT(species) { Ability(ability); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPORE); }
    } SCENE {
        ABILITY_POPUP(opponent, ability);
        STATUS_ICON(opponent, none: TRUE);
    }
}

SINGLE_BATTLE_TEST("Insomnia/Vital Spirit wakes it up when it enters battle")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_DROWZEE; ability = ABILITY_INSOMNIA; }
    PARAMETRIZE { species = SPECIES_MANKEY; ability = ABILITY_VITAL_SPIRIT; }
    
    GIVEN {
        PLAYER(species) { Ability(ability); Status1(STATUS1_SLEEP); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ability);
        STATUS_ICON(player, none: TRUE);
    }
}

TO_DO_BATTLE_TEST("A held berry will activate and wake it up before Insomnia/Vital Spirit activates");

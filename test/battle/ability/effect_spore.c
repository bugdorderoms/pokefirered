#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_SCRATCH].flags.makesContact == TRUE);
}

SINGLE_BATTLE_TEST("Effect Spore only inflicts status on contact")
{
    u32 move;

    PARAMETRIZE { move = MOVE_SCRATCH; }
    PARAMETRIZE { move = MOVE_SWIFT; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SWIFT].flags.makesContact == FALSE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_BRELOOM) { Ability(ABILITY_EFFECT_SPORE); }
    } WHEN {
        TURN { MOVE(player, move, WITH_RNG(RNG_EFFECT_SPORE, 1)); }
    } SCENE {
        if (gBattleMoves[move].flags.makesContact) {
            ABILITY_POPUP(opponent, ABILITY_EFFECT_SPORE);
            MESSAGE("Wobbuffet was poisoned!");
            STATUS_ICON(player, poison: TRUE);
        } else {
            NONE_OF {
                ABILITY_POPUP(opponent, ABILITY_EFFECT_SPORE);
                MESSAGE("Wobbuffet was poisoned!");
                STATUS_ICON(player, poison: TRUE);
            }
        }
    }
}

SINGLE_BATTLE_TEST("Effect Spore causes poison 9% of the time")
{
    PASSES_RANDOMLY(9, 100, RNG_EFFECT_SPORE);
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_BRELOOM) { Ability(ABILITY_EFFECT_SPORE); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_EFFECT_SPORE);
        MESSAGE("Wobbuffet was poisoned!");
        STATUS_ICON(player, poison: TRUE);
    }
}

SINGLE_BATTLE_TEST("Effect Spore causes paralysis 10% of the time")
{
    PASSES_RANDOMLY(10, 100, RNG_EFFECT_SPORE);
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_BRELOOM) { Ability(ABILITY_EFFECT_SPORE); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_EFFECT_SPORE);
        MESSAGE("Wobbuffet is paralyzed! It may be unable to move!");
        STATUS_ICON(player, paralysis: TRUE);
    }
}

SINGLE_BATTLE_TEST("Effect Spore causes sleep 11% of the time")
{
    PASSES_RANDOMLY(11, 100, RNG_EFFECT_SPORE);
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_BRELOOM) { Ability(ABILITY_EFFECT_SPORE); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_EFFECT_SPORE);
        MESSAGE("Wobbuffet fell asleep!");
        STATUS_ICON(player, sleep: TRUE);
    }
}

SINGLE_BATTLE_TEST("Effect Spore doesn't activates on Grass-type Pokémons")
{
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_TANGELA].types[0] == TYPE_GRASS || gSpeciesInfo[SPECIES_TANGELA].types[1] == TYPE_GRASS);
        
        PLAYER(SPECIES_TANGELA);
        OPPONENT(SPECIES_BRELOOM) { Ability(ABILITY_EFFECT_SPORE); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, WITH_RNG(RNG_EFFECT_SPORE, 1)); }
    } SCENE {
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_EFFECT_SPORE);
            MESSAGE("Tangela was poisoned!");
            STATUS_ICON(player, poison: TRUE);
        }
    }
}

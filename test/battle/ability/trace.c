#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Trace copies opponents ability when it enters battle")
{
    GIVEN {
        PLAYER(SPECIES_RALTS) { Ability(ABILITY_TRACE); }
        OPPONENT(SPECIES_TORCHIC) { Ability(ABILITY_BLAZE); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_TRACE);
        ABILITY_POPUP(opponent, ABILITY_BLAZE);
        ABILITY_POPUP(player, ABILITY_BLAZE);
        MESSAGE("It traced Foe Torchic's Blaze!");
    }
}

DOUBLE_BATTLE_TEST("Trace copies opponents ability randomly when it enters battle")
{
    PASSES_RANDOMLY(1, 2, RNG_TRACE_TARGET);
    
    GIVEN {
        PLAYER(SPECIES_RALTS) { Ability(ABILITY_TRACE); }
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_TORCHIC) { Ability(ABILITY_SPEED_BOOST); }
        OPPONENT(SPECIES_TORCHIC) { Ability(ABILITY_BLAZE); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_TRACE);
        ABILITY_POPUP(opponentRight, ABILITY_BLAZE);
        ABILITY_POPUP(playerLeft, ABILITY_BLAZE);
        MESSAGE("It traced Foe Torchic's Blaze!");
    }
}

DOUBLE_BATTLE_TEST("Trace - In a double battle it can fail due to selecting a fainted Pokémon")
{
    PASSES_RANDOMLY(1, 2, RNG_TRACE_TARGET);
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_RALTS) { Ability(ABILITY_TRACE); }
        
        OPPONENT(SPECIES_TORCHIC) { Ability(ABILITY_BLAZE); }
        OPPONENT(SPECIES_TORCHIC) { Ability(ABILITY_BLAZE); HP(1); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SCRATCH, target: opponentRight); }
        TURN { SWITCH(playerLeft, 2); }
    } SCENE {
        NONE_OF {
            ABILITY_POPUP(playerLeft, ABILITY_TRACE);
            ABILITY_POPUP(opponentLeft, ABILITY_BLAZE);
            ABILITY_POPUP(playerLeft, ABILITY_BLAZE);
            MESSAGE("It traced Foe Torchic's Blaze!");
        }
    }
}

SINGLE_BATTLE_TEST("Trace copies opponent's Intimidate and triggers it immediately")
{
    GIVEN {
        PLAYER(SPECIES_RALTS) { Ability(ABILITY_TRACE); }
        OPPONENT(SPECIES_MASQUERAIN) { Ability(ABILITY_INTIMIDATE); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_TRACE);
        ABILITY_POPUP(opponent, ABILITY_INTIMIDATE);
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        MESSAGE("It traced Foe Masquerain's Intimidate!");
        
        ABILITY_POPUP(player, ABILITY_INTIMIDATE);
        MESSAGE("Foe Masquerain's Attack fell!");
    }
}

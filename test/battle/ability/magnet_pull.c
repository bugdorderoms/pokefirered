#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gSpeciesInfo[SPECIES_MELTAN].types[0] == TYPE_STEEL);
}

WILD_BATTLE_TEST("Magnet Pull prevents Steel-type Pokémon flee with Teleport")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_TELEPORT].effect == EFFECT_TELEPORT);
        
        PLAYER(SPECIES_MAGNETON) { Ability(ABILITY_MAGNET_PULL); }
        OPPONENT(SPECIES_MELTAN);
    } WHEN {
        TURN { MOVE(opponent, MOVE_TELEPORT); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_TELEPORT, opponent);
        MESSAGE("Magneton's Magnet Pull made it ineffective!");
    }
}

WILD_DOUBLE_BATTLE_TEST("Magnet Pull doesn't prevents ally Steel-type flee with Teleport")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_TELEPORT].effect == EFFECT_TELEPORT);
        
        PLAYER(SPECIES_BULBASAUR);
        PLAYER(SPECIES_BULBASAUR);
        
        OPPONENT(SPECIES_MELTAN);
        OPPONENT(SPECIES_MAGNETON) { Ability(ABILITY_MAGNET_PULL); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_TELEPORT); }
    } SCENE {
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_TELEPORT, opponentLeft);
            MESSAGE("Wild Magneton's Magnet Pull made it ineffective!");
        }
        // It fails bc is a double wild battle
        MESSAGE("But it failed!");
    }
}

SINGLE_BATTLE_TEST("Magnet Pull doesn't prevents switching out with moves")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_BATON_PASS].effect == EFFECT_BATON_PASS);
        
        PLAYER(SPECIES_MELTAN);
        PLAYER(SPECIES_BULBASAUR);
        
        OPPONENT(SPECIES_MAGNETON) { Ability(ABILITY_MAGNET_PULL); }
    } WHEN {
        TURN { MOVE(player, MOVE_BATON_PASS); SEND_OUT(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BATON_PASS, player);
        NOT MESSAGE("Foe Magneton's Magnet Pull made it ineffective!");
    }
}

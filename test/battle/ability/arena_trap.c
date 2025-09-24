#include "global.h"
#include "test/battle.h"

WILD_BATTLE_TEST("Arena Trap prevents grounded Pokémon flee with Teleport")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_TELEPORT].effect == EFFECT_TELEPORT);
        
        PLAYER(SPECIES_DIGLETT) { Ability(ABILITY_ARENA_TRAP); }
        OPPONENT(SPECIES_BULBASAUR);
    } WHEN {
        TURN { MOVE(opponent, MOVE_TELEPORT); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_TELEPORT, opponent);
        MESSAGE("Diglett's Arena Trap made it ineffective!");
    }
}

WILD_DOUBLE_BATTLE_TEST("Arena Trap doesn't prevents grounded ally flee with Teleport")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_TELEPORT].effect == EFFECT_TELEPORT);
        
        PLAYER(SPECIES_BULBASAUR);
        PLAYER(SPECIES_BULBASAUR);
        
        OPPONENT(SPECIES_BULBASAUR);
        OPPONENT(SPECIES_DIGLETT) { Ability(ABILITY_ARENA_TRAP); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_TELEPORT); }
    } SCENE {
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_TELEPORT, opponentLeft);
            MESSAGE("Wild Diglett's Arena Trap made it ineffective!");
        }
        // It fails bc is a double wild battle
        MESSAGE("But it failed!");
    }
}

SINGLE_BATTLE_TEST("Arena Trap doesn't prevents switching out with moves")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_BATON_PASS].effect == EFFECT_BATON_PASS);
        
        PLAYER(SPECIES_CHARMANDER);
        PLAYER(SPECIES_BULBASAUR);
        
        OPPONENT(SPECIES_DIGLETT) { Ability(ABILITY_ARENA_TRAP); }
    } WHEN {
        TURN { MOVE(player, MOVE_BATON_PASS); SEND_OUT(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BATON_PASS, player);
        NOT MESSAGE("Foe Diglett's Arena Trap made it ineffective!");
    }
}

TO_DO_BATTLE_TEST("Arena Trap doesn't prevents non-adjacent grounded Pokémon from flee");

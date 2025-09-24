#include "global.h"
#include "test/battle.h"

WILD_BATTLE_TEST("Shadow Tag prevents opposing Pokémon flee with Teleport")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_TELEPORT].effect == EFFECT_TELEPORT);
        
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_SHADOW_TAG); }
        OPPONENT(SPECIES_BULBASAUR);
    } WHEN {
        TURN { MOVE(opponent, MOVE_TELEPORT); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_TELEPORT, opponent);
        MESSAGE("Wobbuffet's Shadow Tag made it ineffective!");
    }
}

WILD_DOUBLE_BATTLE_TEST("Shadow Tag doesn't prevents ally flee with Teleport")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_TELEPORT].effect == EFFECT_TELEPORT);
        
        PLAYER(SPECIES_BULBASAUR);
        PLAYER(SPECIES_BULBASAUR);
        
        OPPONENT(SPECIES_BULBASAUR);
        OPPONENT(SPECIES_WOBBUFFET) { Ability(ABILITY_SHADOW_TAG); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_TELEPORT); MOVE(playerLeft, MOVE_SPLASH); }
    } SCENE {
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_TELEPORT, opponentLeft);
            MESSAGE("Wild Wobbuffet's Shadow Tag made it ineffective!");
        }
        // It fails bc is a double wild battle
        MESSAGE("But it failed!");
    }
}

WILD_BATTLE_TEST("Shadow Tag doesn't prevents flee if opposing Pokémon has Shadow Tag")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_TELEPORT].effect == EFFECT_TELEPORT);
        
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_SHADOW_TAG); }
        OPPONENT(SPECIES_WOBBUFFET) { Ability(ABILITY_SHADOW_TAG); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_TELEPORT); MOVE(player, MOVE_SPLASH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TELEPORT, opponent);
        NOT MESSAGE("Wobbuffet's Shadow Tag made it ineffective!");
    }
}

SINGLE_BATTLE_TEST("Shadow Tag doesn't prevents switching out with moves")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_BATON_PASS].effect == EFFECT_BATON_PASS);
        
        PLAYER(SPECIES_CHARMANDER);
        PLAYER(SPECIES_BULBASAUR);
        
        OPPONENT(SPECIES_WOBBUFFET) { Ability(ABILITY_SHADOW_TAG); }
    } WHEN {
        TURN { MOVE(player, MOVE_BATON_PASS); SEND_OUT(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BATON_PASS, player);
        NOT MESSAGE("Foe Wobbuffet's Shadow Tag made it ineffective!");
    }
}

TO_DO_BATTLE_TEST("Shadow Tag doesn't prevents non-adjacent Pokémon from flee");

#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_DRAGON_TAIL].effect == EFFECT_HIT_SWITCH_TARGET);
}

SINGLE_BATTLE_TEST("Dragon Tail switches the target with a random non-fainted replacement")
{
    PASSES_RANDOMLY(1, 2, RNG_FORCE_RANDOM_SWITCH);
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_BULBASAUR);
        OPPONENT(SPECIES_CHARMANDER);
        OPPONENT(SPECIES_SQUIRTLE) { HP(0); }
    } WHEN {
        TURN { MOVE(player, MOVE_DRAGON_TAIL); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_TAIL, player);
        MESSAGE("Foe Bulbasaur was dragged out!");
    }
}

SINGLE_BATTLE_TEST("Dragon Tail fails to switch if no replacements")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_BULBASAUR) { HP(0); }
    } WHEN {
        TURN { MOVE(player, MOVE_DRAGON_TAIL); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_TAIL, player);
        NOT MESSAGE("Foe Bulbasaur was dragged out!");
    }
}

SINGLE_BATTLE_TEST("Dragon Tail fails to switch if move had no effect")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_DRAGON_TAIL].type == TYPE_DRAGON);
        ASSUME(gSpeciesInfo[SPECIES_CLEFFA].types[0] == TYPE_FAIRY);
        
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_CLEFFA);
        OPPONENT(SPECIES_BULBASAUR);
    } WHEN {
        TURN { MOVE(player, MOVE_DRAGON_TAIL); }
    } SCENE {
        MESSAGE("It doesn't affect Foe Cleffa…");
        
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_TAIL, player);
            MESSAGE("Foe Bulbasaur was dragged out!");
        }
    }
}

SINGLE_BATTLE_TEST("Dragon Tail fails to switch if hit substitute")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SUBSTITUTE].effect == EFFECT_SUBSTITUTE);
        
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_BULBASAUR);
    } WHEN {
        TURN { MOVE(player, MOVE_DRAGON_TAIL); MOVE(opponent, MOVE_SUBSTITUTE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_TAIL, player);
        NOT MESSAGE("Foe Bulbasaur was dragged out!");
    }
}

SINGLE_BATTLE_TEST("Dragon Tail fails to switch if target is rooted")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_INGRAIN].effect == EFFECT_SET_ATTACKER_STATUS);
        ASSUME(gBattleMoves[MOVE_INGRAIN].argument.setStatus.statusId == ID_STATUS3 && UNCOMPRESS_BITS(gBattleMoves[MOVE_INGRAIN].argument.setStatus.status) == STATUS3_ROOTED);
        
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_BULBASAUR);
    } WHEN {
        TURN { MOVE(player, MOVE_DRAGON_TAIL); MOVE(opponent, MOVE_INGRAIN); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_INGRAIN, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_TAIL, player);
        NOT MESSAGE("Foe Bulbasaur was dragged out!");
    }
}

SINGLE_BATTLE_TEST("Dragon Tail fails to switch if user faints")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(1); }
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_SHARPEDO) { Ability(ABILITY_ROUGH_SKIN); }
        OPPONENT(SPECIES_BULBASAUR);
    } WHEN {
        TURN { MOVE(player, MOVE_DRAGON_TAIL); SEND_OUT(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_TAIL, player);
        MESSAGE("Wobbuffet fainted!");
        NOT MESSAGE("Foe Bulbasaur was dragged out!");
    }
}

SINGLE_BATTLE_TEST("Dragon Tail fails to switch if target has Suction Cups")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_OCTILLERY) { Ability(ABILITY_SUCTION_CUPS); }
        OPPONENT(SPECIES_BULBASAUR);
    } WHEN {
        TURN { MOVE(player, MOVE_DRAGON_TAIL); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_TAIL, player);
        MESSAGE("Foe Octillery anchors itself with Suction Cups!");
        NOT MESSAGE("Foe Bulbasaur was dragged out!");
    }
}

TO_DO_BATTLE_TEST("Dragon Tail fails to switch if target has Guard Dog");
TO_DO_BATTLE_TEST("Dragon Tail fails to switch if target is Dynamaxed");

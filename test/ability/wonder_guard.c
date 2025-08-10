#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gSpeciesInfo[SPECIES_SHEDINJA].types[0] == TYPE_BUG && gSpeciesInfo[SPECIES_SHEDINJA].types[1] == TYPE_GHOST);
}

SINGLE_BATTLE_TEST("Wonder Guard protect against moves that are not super effective")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_VINE_WHIP].type == TYPE_GRASS);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_SHEDINJA) { Ability(ABILITY_WONDER_GUARD); }
    } WHEN {
        TURN { MOVE(player, MOVE_VINE_WHIP); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_VINE_WHIP, player);
        ABILITY_POPUP(opponent, ABILITY_WONDER_GUARD);
        MESSAGE("It doesn't affect Foe Shedinja…");
    }
}

SINGLE_BATTLE_TEST("Wonder Guard doesn't protect against status moves")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_TAIL_WHIP].split == SPLIT_STATUS);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_SHEDINJA) { Ability(ABILITY_WONDER_GUARD); }
    } WHEN {
        TURN { MOVE(player, MOVE_TAIL_WHIP); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TAIL_WHIP, player);
        
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_WONDER_GUARD);
            MESSAGE("It doesn't affect Foe Shedinja…");
        }
    }
}

SINGLE_BATTLE_TEST("Wonder Guard doesn't protect against Struggle")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_STRUGGLE].type == TYPE_NORMAL);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_SHEDINJA) { Ability(ABILITY_WONDER_GUARD); }
    } WHEN {
        TURN { MOVE(player, MOVE_STRUGGLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STRUGGLE, player);
        
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_WONDER_GUARD);
            MESSAGE("It doesn't affect Foe Shedinja…");
        }
    }
}

SINGLE_BATTLE_TEST("Wonder Guard doesn't protect from the confusion damage")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_CONFUSE_RAY].effect == EFFECT_CONFUSE);
        
        PLAYER(SPECIES_SHEDINJA) { Ability(ABILITY_WONDER_GUARD); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_CONFUSE_RAY); MOVE(player, MOVE_SCRATCH, WITH_RNG(RNG_CONFUSION, TRUE)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CONFUSE_RAY, opponent);
        
        MESSAGE("Shedinja is confused!");
        MESSAGE("It hurt itself in its confusion!");
        HP_BAR(player);
        
        NONE_OF {
            ABILITY_POPUP(player, ABILITY_WONDER_GUARD);
            MESSAGE("It doesn't affect Shedinja…");
        }
    }
}

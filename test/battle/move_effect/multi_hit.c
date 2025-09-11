#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_DOUBLE_SLAP].effect == EFFECT_MULTI_HIT);
}

SINGLE_BATTLE_TEST("Multi hit moves hit twice 35% of the time")
{
    PASSES_RANDOMLY(7, 20, RNG_HITS);
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_SLAP); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_SLAP, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_SLAP, player);
        MESSAGE("Hit 2 time(s)!");
    }
}

SINGLE_BATTLE_TEST("Multi hit moves hit thrice 35% of the time")
{
    PASSES_RANDOMLY(7, 20, RNG_HITS);
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_SLAP); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_SLAP, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_SLAP, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_SLAP, player);
        MESSAGE("Hit 3 time(s)!");
    }
}

SINGLE_BATTLE_TEST("Multi hit moves hit four times 15% of the time")
{
    PASSES_RANDOMLY(3, 20, RNG_HITS);
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_SLAP); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_SLAP, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_SLAP, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_SLAP, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_SLAP, player);
        MESSAGE("Hit 4 time(s)!");
    }
}

SINGLE_BATTLE_TEST("Multi hit moves hit five times 15% of the time")
{
    PASSES_RANDOMLY(3, 20, RNG_HITS);
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_SLAP); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_SLAP, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_SLAP, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_SLAP, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_SLAP, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_SLAP, player);
        MESSAGE("Hit 5 time(s)!");
    }
}

SINGLE_BATTLE_TEST("Multi hit moves activate target contact abilities in each strike")
{
    u32 j;
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_DOUBLE_SLAP].flags.makesContact == TRUE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_SHARPEDO) { Ability(ABILITY_ROUGH_SKIN); }
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_SLAP, WITH_RNG(RNG_HITS, 3)); }
    } SCENE {
        for (j = 0; j < 3; j++)
        {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_SLAP, player);
            ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
            MESSAGE("Wobbuffet was hurt!");
        }
        MESSAGE("Hit 3 time(s)!");
    }
}

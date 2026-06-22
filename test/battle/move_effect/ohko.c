#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(gBattleMoves[MOVE_FISSURE].effect == EFFECT_OHKO);
}

SINGLE_BATTLE_TEST("OHKO moves faints the target, skipping regular damage calculations")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_FISSURE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FISSURE, player);
        HP_BAR(opponent, hp: 0);
        MESSAGE("It's a one-hit KO!");
    }
}

SINGLE_BATTLE_TEST("OHKO moves are unaffected by accuracy checks")
{
    PASSES_RANDOMLY(30, 100, RNG_OHKO);
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_FISSURE].accuracy == 30);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_FISSURE, WITH_RNG(RNG_ACCURACY, FALSE)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FISSURE, player);
        MESSAGE("It's a one-hit KO!");
    }
}

SINGLE_BATTLE_TEST("OHKO moves can't hit a semi-invulnerable target")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_FLY].effect == EFFECT_SEMI_INVULNERABLE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_FLY); MOVE(player, MOVE_FISSURE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FLY, opponent);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_FISSURE, player);
        MESSAGE("But it failed!");
    }
}

SINGLE_BATTLE_TEST("OHKO moves never misses if the user has No Guard")
{
    GIVEN {
        PLAYER(SPECIES_MACHOP) { Ability(ABILITY_NO_GUARD); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_FISSURE, WITH_RNG(RNG_OHKO, FALSE)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FISSURE, player);
        MESSAGE("It's a one-hit KO!");
    }
}

SINGLE_BATTLE_TEST("OHKO moves never misses if the user has used Lock-On")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_LOCK_ON].effect == EFFECT_LOCK_ON);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_LOCK_ON); }
        TURN { MOVE(player, MOVE_FISSURE, WITH_RNG(RNG_OHKO, FALSE)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LOCK_ON, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FISSURE, player);
        MESSAGE("It's a one-hit KO!");
    }
}

SINGLE_BATTLE_TEST("OHKO moves always fails if the target has a higher level than the user")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Level(2); }
    } WHEN {
        TURN { MOVE(player, MOVE_FISSURE); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_FISSURE, player);
        MESSAGE("Foe Wobbuffet is unaffected!");
    }
}

SINGLE_BATTLE_TEST("OHKO moves always fails if target is dynamaxed")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_FISSURE); MOVE(opponent, MOVE_SCRATCH, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        MESSAGE("Foe Wobbuffet grew huge into its Dynamax form!");
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_FISSURE, player);
        MESSAGE("Foe Wobbuffet is unaffected!");
    }
}

TO_DO_BATTLE_TEST("OHKO moves doesn't fails in max raid battles if target has no shields");
TO_DO_BATTLE_TEST("OHKO moves always fails if target is a tera raid boss");

#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Skill Link enable multi-hit moves to always hit five times")
{
    PASSES_RANDOMLY(100, 100, RNG_HITS);
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_BULLET_SEED].effect == EFFECT_MULTI_HIT);
        
        PLAYER(SPECIES_AIPOM) { Ability(ABILITY_SKILL_LINK); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_BULLET_SEED); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BULLET_SEED, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BULLET_SEED, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BULLET_SEED, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BULLET_SEED, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BULLET_SEED, player);
        MESSAGE("Hit 5 time(s)!");
    }
}

TO_DO_BATTLE_TEST("Skill Link makes Triple Kick's accuracy be checked only on first hit");
TO_DO_BATTLE_TEST("Skill Link makes Population Bomb's accuracy be checked only on first hit");

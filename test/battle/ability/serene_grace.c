#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Serene Grace doubles additional effects chance")
{
    PASSES_RANDOMLY(20, 100, RNG_SECONDARY_EFFECT);
    
    GIVEN {
        ASSUME(MoveHasMoveEffectWithChance(MOVE_PSYCHIC, MOVE_EFFECT_SP_DEF_MINUS_1, 10) == TRUE);
        
        PLAYER(SPECIES_TOGEPI) { Ability(ABILITY_SERENE_GRACE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_PSYCHIC); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PSYCHIC, player);
        MESSAGE("Foe Wobbuffet's Sp. Def fell!");
    }
}

TO_DO_BATTLE_TEST("Serene Grace doubles King's Rock's effect chance");
TO_DO_BATTLE_TEST("Serene Grace stack with Rainbow's effect, except with flinching");
TO_DO_BATTLE_TEST("Serene Grace doesn't doubles the G-Max Replenish and Snooze's effect chance");

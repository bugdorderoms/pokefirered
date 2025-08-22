#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Inner Focus prevents flinching")
{
    GIVEN {
        ASSUME(MoveHasMoveEffectWithChance(MOVE_FAKE_OUT, MOVE_EFFECT_FLINCH, 100) == TRUE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_ZUBAT) { Ability(ABILITY_INNER_FOCUS); }
    } WHEN {
        TURN { MOVE(player, MOVE_FAKE_OUT); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FAKE_OUT, player);
        ABILITY_POPUP(opponent, ABILITY_INNER_FOCUS);
        MESSAGE("Foe Zubat's won't flinch because of its Inner Focus!");
        NOT MESSAGE("Foe Zubat flinched!");
    }
}

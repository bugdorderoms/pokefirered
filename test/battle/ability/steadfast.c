#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Steadfast boosts speed when flinched")
{
    GIVEN {
        ASSUME(MoveHasMoveEffectWithChance(MOVE_FAKE_OUT, MOVE_EFFECT_FLINCH, 100) == TRUE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_MACHOP) { Ability(ABILITY_STEADFAST); }
    } WHEN {
        TURN { MOVE(player, MOVE_FAKE_OUT); }
    } SCENE {
        MESSAGE("Foe Machop flinched!");
        ABILITY_POPUP(opponent, ABILITY_STEADFAST);
        MESSAGE("Foe Machop's Speed rose!");
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGES + 1);
    }
}

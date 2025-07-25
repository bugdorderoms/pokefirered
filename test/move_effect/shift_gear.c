#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_SHIFT_GEAR].effect == EFFECT_SHIFT_GEAR);
}

SINGLE_BATTLE_TEST("Shift Gear raises Speed by two stages and Attack by one")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SHIFT_GEAR); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHIFT_GEAR, player);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGES + 2);
        EXPECT_EQ(player->statStages[STAT_ATK], DEFAULT_STAT_STAGES + 1);
    }
}

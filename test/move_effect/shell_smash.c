#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_SHELL_SMASH].effect == EFFECT_SHELL_SMASH);
}

SINGLE_BATTLE_TEST("Shell Smash lowers defensive stats and sharply raises offensive stats and Speed")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SHELL_SMASH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHELL_SMASH, player);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_DEF], DEFAULT_STAT_STAGES - 1);
        EXPECT_EQ(player->statStages[STAT_SPDEF], DEFAULT_STAT_STAGES - 1);
        EXPECT_EQ(player->statStages[STAT_ATK], DEFAULT_STAT_STAGES + 2);
        EXPECT_EQ(player->statStages[STAT_SPATK], DEFAULT_STAT_STAGES + 2);
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGES + 2);
    }
}

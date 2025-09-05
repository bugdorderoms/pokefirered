#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Simple doubles the user's stat stage effects")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_HARDEN].effect == EFFECT_DEFENSE_UP);
        
        PLAYER(SPECIES_NUMEL) { Ability(ABILITY_SIMPLE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_HARDEN); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HARDEN, player);
        MESSAGE("Numel's Defense sharply rose!");
    } THEN {
        EXPECT_EQ(player->statStages[STAT_DEF], DEFAULT_STAT_STAGES + 2);
    }
}

TO_DO_BATTLE_TEST("Simple can't double an stat change caused by status Z-Move effects");
TO_DO_BATTLE_TEST("Simple can double an stat change caused by Extreme Evoboost and Clangorous Soulblaze");

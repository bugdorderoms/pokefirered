#include "global.h"
#include "test/battle.h"
#include "constants/battle_string_ids.h"

SINGLE_BATTLE_TEST("Two turns moves charges on first turn and attacks on the second")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_RAZOR_WIND].effect == EFFECT_TWO_TURNS_ATTACK);
        ASSUME(gBattleMoves[MOVE_RAZOR_WIND].argument.twoTurns.stringId == B_MSG_WHIPPED_WHIRLWIND);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_RAZOR_WIND); }
        TURN { SKIP_TURN(player); }
    } SCENE {
        // Charging turn
        MESSAGE("Wobbuffet used Razor Wind!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_RAZOR_WIND, player);
        MESSAGE("Wobbuffet whipped up a whirlwind!");
        // Attack turn
        MESSAGE("Wobbuffet used Razor Wind!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_RAZOR_WIND, player);
        HP_BAR(opponent);
    }
}

SINGLE_BATTLE_TEST("Two turns moves are only blocked by protect on its second turn")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_RAZOR_WIND].effect == EFFECT_TWO_TURNS_ATTACK);
        ASSUME(gBattleMoves[MOVE_RAZOR_WIND].argument.twoTurns.stringId == B_MSG_WHIPPED_WHIRLWIND);
        ASSUME(gBattleMoves[MOVE_PROTECT].effect == EFFECT_PROTECT);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_RAZOR_WIND); }
        TURN { SKIP_TURN(player); MOVE(opponent, MOVE_PROTECT); }
    } SCENE {
        // Charging turn
        MESSAGE("Wobbuffet used Razor Wind!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_RAZOR_WIND, player);
        MESSAGE("Wobbuffet whipped up a whirlwind!");
        
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PROTECT, opponent);
        
        // Attack turn
        MESSAGE("Wobbuffet used Razor Wind!");
        
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_RAZOR_WIND, player);
            HP_BAR(opponent);
        }
        MESSAGE("Foe Wobbuffet protected itself!");
    }
}

SINGLE_BATTLE_TEST("Two turn moves activates its secondary effect on the charging turn")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SKULL_BASH].effect == EFFECT_TWO_TURNS_ATTACK);
        ASSUME(gBattleMoves[MOVE_SKULL_BASH].argument.twoTurns.stringId == B_MSG_TUCKED_HEAD);
        ASSUME(MoveHasMoveEffect(MOVE_SKULL_BASH, MOVE_EFFECT_DEF_PLUS_1, TRUE) == TRUE);
        ASSUME(MoveHasChargeTurnMoveEffect(MOVE_SKULL_BASH) == TRUE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SKULL_BASH); }
        TURN { SKIP_TURN(player); }
    } SCENE {
        // Charging turn
        MESSAGE("Wobbuffet used Skull Bash!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SKULL_BASH, player);
        MESSAGE("Wobbuffet tucked in its head!");
        MESSAGE("Wobbuffet's Defense rose!");
        
        // Attack turn
        MESSAGE("Wobbuffet used Skull Bash!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SKULL_BASH, player);
        HP_BAR(opponent);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_DEF], DEFAULT_STAT_STAGES + 1);
    }
}

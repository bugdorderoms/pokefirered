#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_SKY_DROP].effect == EFFECT_SKY_DROP);
}

SINGLE_BATTLE_TEST("Sky Drop fails if target is behind a substitute")
{
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_WOBBUFFET].weight < 2000);
        ASSUME(gBattleMoves[MOVE_SUBSTITUTE].effect == EFFECT_SUBSTITUTE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUBSTITUTE); MOVE(player, MOVE_SKY_DROP); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, opponent);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SKY_DROP, player);
        MESSAGE("But it failed!");
    }
}

DOUBLE_BATTLE_TEST("Sky Drop fails if used on ally")
{
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_WOBBUFFET].weight < 2000);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SKY_DROP, target: playerRight); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SKY_DROP, playerLeft);
        MESSAGE("But it failed!");
    }
}

SINGLE_BATTLE_TEST("Sky Drop fails if target is in a Semi-Invulnerable state")
{
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_WOBBUFFET].weight < 2000);
        ASSUME(gBattleMoves[MOVE_FLY].effect == EFFECT_SEMI_INVULNERABLE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_FLY); MOVE(player, MOVE_SKY_DROP); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FLY, opponent);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SKY_DROP, player);
        MESSAGE("But it failed!");
    }
}

SINGLE_BATTLE_TEST("Sky Drop fails on heavy targets")
{
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_METAGROSS].weight >= 2000);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_METAGROSS);
    } WHEN {
        TURN { MOVE(player, MOVE_SKY_DROP); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SKY_DROP, player);
        MESSAGE("But it failed!");
    }
}

SINGLE_BATTLE_TEST("Sky Drop does no damage to Flying type Pokémon")
{
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_PIDGEY].weight < 2000);
        ASSUME(gSpeciesInfo[SPECIES_PIDGEY].types[1] == TYPE_FLYING);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_PIDGEY);
    } WHEN {
        TURN { MOVE(player, MOVE_SKY_DROP); }
        TURN { SKIP_TURN(player); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SKY_DROP, player);
        MESSAGE("Wobbuffet took Foe Pidgey into the sky!");
        NOT HP_BAR(opponent);
    }
}

DOUBLE_BATTLE_TEST("Sky Drop is cancelled if Gravity activated")
{
    u32 j;
    
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_WOBBUFFET].weight < 2000);
        ASSUME(gBattleMoves[MOVE_GRAVITY].effect == EFFECT_GRAVITY);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SKY_DROP, target: opponentLeft); MOVE(playerRight, MOVE_GRAVITY); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SKY_DROP, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GRAVITY, playerRight);
        MESSAGE("Wobbuffet can't stay airbone because of gravity!");
    } THEN {
        // all battlers should be visible
        for (j = 0; j < MAX_BATTLERS_COUNT; j++)
            EXPECT_EQ(gBattleSpritesDataPtr->battlerData[j].invisible, FALSE);
    }
}

SINGLE_BATTLE_TEST("Sky Drop cancels targets two turn moves")
{
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_WOBBUFFET].weight < 2000);
        ASSUME(GET_MOVE_MOVEEFFECT_TABLE(MOVE_SOLAR_BEAM).twoTurnsEffect == TRUE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SOLAR_BEAM); MOVE(player, MOVE_SKY_DROP); }
        TURN { SKIP_TURN(player); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SOLAR_BEAM, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SKY_DROP, player);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SOLAR_BEAM, opponent);
        HP_BAR(opponent);
    }
}

SINGLE_BATTLE_TEST("Sky Drop stops the confusion count until the target is dropped")
{
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_WOBBUFFET].weight < 2000);
        ASSUME(MoveHasMoveEffect(MOVE_THRASH, MOVE_EFFECT_THRASH, TRUE) == TRUE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_THRASH); }
        TURN { SKIP_TURN(player); }
        TURN { SKIP_TURN(player); }
        TURN { MOVE(opponent, MOVE_SKY_DROP); }
        TURN { SKIP_TURN(opponent); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_THRASH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_THRASH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_THRASH, player);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_CONFUSION, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SKY_DROP, opponent);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_CONFUSION, player);
    }
}

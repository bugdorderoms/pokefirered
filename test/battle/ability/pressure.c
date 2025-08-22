#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Pressure displays a message on switch in")
{
    GIVEN {
        PLAYER(SPECIES_MEWTWO) { Ability(ABILITY_PRESSURE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_PRESSURE);
        MESSAGE("Mewtwo is exerting its Pressure!");
    }
}

SINGLE_BATTLE_TEST("Pressure causes opponent's moves to use up 1 additional PP")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MovesWithPP({MOVE_SCRATCH, 35}); }
        OPPONENT(SPECIES_MEWTWO) { Ability(ABILITY_PRESSURE); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } THEN {
        EXPECT_EQ(player->pp[0], 33);
    }
}

DOUBLE_BATTLE_TEST("Pressure doesn't increase an ally's PP usage")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MovesWithPP({MOVE_SCRATCH, 35}); }
        PLAYER(SPECIES_MEWTWO) { Ability(ABILITY_PRESSURE); }
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SCRATCH, target: playerRight); }
    } THEN {
        EXPECT_EQ(playerLeft->pp[0], 34);
    }
}

DOUBLE_BATTLE_TEST("Pressure's effect stacks with multiple Pokémon")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SWIFT].target == MOVE_TARGET_BOTH);
        
        PLAYER(SPECIES_WOBBUFFET) { MovesWithPP({MOVE_SWIFT, 20}); }
        PLAYER(SPECIES_WYNAUT);
        
        OPPONENT(SPECIES_MEWTWO) { Ability(ABILITY_PRESSURE); }
        OPPONENT(SPECIES_ZAPDOS) { Ability(ABILITY_PRESSURE); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SWIFT); }
    } THEN {
        EXPECT_EQ(playerLeft->pp[0], 17);
    }
}

SINGLE_BATTLE_TEST("Pressure's effect applies to Imprison and Snatch")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MovesWithPP({MOVE_IMPRISON, 10}, {MOVE_SNATCH, 10}); }
        OPPONENT(SPECIES_MEWTWO) { Ability(ABILITY_PRESSURE); }
    } WHEN {
        TURN { MOVE(player, MOVE_IMPRISON); }
        TURN { MOVE(player, MOVE_SNATCH); }
    } THEN {
        EXPECT_EQ(player->pp[0], 8);
        EXPECT_EQ(player->pp[1], 8);
    }
}

SINGLE_BATTLE_TEST("Pressure's effect applies to Spikes, Stealth Rock and Toxic Spikes")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MovesWithPP({MOVE_SPIKES, 20}, {MOVE_STEALTH_ROCK, 20}, {MOVE_TOXIC_SPIKES, 20}); }
        OPPONENT(SPECIES_MEWTWO) { Ability(ABILITY_PRESSURE); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPIKES); }
        TURN { MOVE(player, MOVE_STEALTH_ROCK); }
        TURN { MOVE(player, MOVE_TOXIC_SPIKES); }
    } THEN {
        EXPECT_EQ(player->pp[0], 18);
        EXPECT_EQ(player->pp[1], 18);
        EXPECT_EQ(player->pp[2], 18);
    }
}

TO_DO_BATTLE_TEST("Pressure's effect doesn't apply to Sticky Web");
TO_DO_BATTLE_TEST("Pressure increases Tera Blast's PP even if its'nt the target of the move"); // add forcePressure flag to Tera Blast

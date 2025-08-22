#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Soundproof blocks others Pokémon sound-based moves")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_HYPER_VOICE].flags.soundMove == TRUE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_VOLTORB) { Ability(ABILITY_SOUNDPROOF); }
    } WHEN {
        TURN { MOVE(player, MOVE_HYPER_VOICE); }
    } SCENE {
        MESSAGE("Wobbuffet used Hyper Voice!");
        ABILITY_POPUP(opponent, ABILITY_SOUNDPROOF);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, player);
    }
}

SINGLE_BATTLE_TEST("Soundproof does not block its own sound-based move")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_HOWL].flags.soundMove == TRUE);
        
        PLAYER(SPECIES_VOLTORB) { Ability(ABILITY_SOUNDPROOF); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_HOWL); }
    } SCENE {
        NOT ABILITY_POPUP(player, ABILITY_SOUNDPROOF);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HOWL, player);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_ATK], DEFAULT_STAT_STAGES + 1);
    }
}

DOUBLE_BATTLE_TEST("Soundproof prevents receiving the Perish Song's counter, except self")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_PERISH_SONG].effect == EFFECT_PERISH_SONG);
        ASSUME(gBattleMoves[MOVE_PERISH_SONG].flags.soundMove == TRUE);
        
        PLAYER(SPECIES_VOLTORB) { Ability(ABILITY_SOUNDPROOF); }
        PLAYER(SPECIES_VOLTORB) { Ability(ABILITY_SOUNDPROOF); }
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_VOLTORB) { Ability(ABILITY_SOUNDPROOF); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_PERISH_SONG); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PERISH_SONG, playerLeft);
        MESSAGE("All Pokémon hearing the song will faint in three turns!");
        
        NOT ABILITY_POPUP(playerLeft, ABILITY_SOUNDPROOF);
        ABILITY_POPUP(playerRight, ABILITY_SOUNDPROOF);
        ABILITY_POPUP(opponentRight, ABILITY_SOUNDPROOF);
    } THEN {
        EXPECT_EQ(gDisableStructs[B_POSITION_PLAYER_LEFT].perishSongTimer, 2);
        EXPECT_EQ(gDisableStructs[B_POSITION_OPPONENT_LEFT].perishSongTimer, 2);
        EXPECT_EQ(gDisableStructs[B_POSITION_PLAYER_RIGHT].perishSongTimer, 0);
        EXPECT_EQ(gDisableStructs[B_POSITION_OPPONENT_RIGHT].perishSongTimer, 0);
    }
}

DOUBLE_BATTLE_TEST("Soundproof only blocks Heal Bell if its on field")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_HEAL_BELL].effect == EFFECT_HEAL_BELL);
        ASSUME(gBattleMoves[MOVE_HEAL_BELL].flags.soundMove == TRUE);
        
        PLAYER(SPECIES_VOLTORB) { Ability(ABILITY_SOUNDPROOF); Status1(STATUS1_POISON); }
        PLAYER(SPECIES_VOLTORB) { Ability(ABILITY_SOUNDPROOF); Status1(STATUS1_POISON); }
        PLAYER(SPECIES_ELECTRODE) { Ability(ABILITY_SOUNDPROOF); Status1(STATUS1_POISON); }
        PLAYER(SPECIES_LOUDRED) { Ability(ABILITY_SOUNDPROOF); Status1(STATUS1_POISON); }
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_HEAL_BELL); }
    } SCENE {
        // player left
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HEAL_BELL, playerLeft);
        MESSAGE("Voltorb was cured of its poison.");
        
        // player right
        ABILITY_POPUP(playerRight, ABILITY_SOUNDPROOF);
        
        // party members
        MESSAGE("Electrode was cured of its poison.");
        MESSAGE("Loudred was cured of its poison.");
    }
}

TO_DO_BATTLE_TEST("Soundproof blocks ally's Howl");

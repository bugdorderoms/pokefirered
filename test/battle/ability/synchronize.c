#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Synchronize will mirror back non volatile status back at opposing mon")
{

    GIVEN {
        ASSUME(gBattleMoves[MOVE_TOXIC].effect == EFFECT_SET_TOXIC_POISON);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_ABRA) { Ability(ABILITY_SYNCHRONIZE); }
    } WHEN {
        TURN { MOVE(player, MOVE_TOXIC); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TOXIC, player);
        MESSAGE("Foe Abra is badly poisoned!");
        STATUS_ICON(opponent, badPoison: TRUE);
        
        ABILITY_POPUP(opponent, ABILITY_SYNCHRONIZE);
        MESSAGE("Wobbuffet is badly poisoned!");
        STATUS_ICON(player, badPoison: TRUE);
    }
}

SINGLE_BATTLE_TEST("Synchronize show up the ability pop up even if it fails to inflict the status")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_TACKLE].flags.makesContact == TRUE);
        ASSUME(gSpeciesInfo[SPECIES_PIKACHU].types[0] == TYPE_ELECTRIC);
        
        PLAYER(SPECIES_PIKACHU) { Ability(ABILITY_STATIC); }
        OPPONENT(SPECIES_ABRA) { Ability(ABILITY_SYNCHRONIZE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_TACKLE, WITH_RNG(RNG_STATIC, TRUE)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponent);
        ABILITY_POPUP(player, ABILITY_STATIC);
        STATUS_ICON(opponent, paralysis: TRUE);
        
        ABILITY_POPUP(opponent, ABILITY_SYNCHRONIZE);
        NONE_OF {
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PRZ, player);
            STATUS_ICON(player, paralysis: TRUE);
        }
    }
}

SINGLE_BATTLE_TEST("Synchronize can affect a Pokémon behind Substitute")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SUBSTITUTE].effect == EFFECT_SUBSTITUTE);
        ASSUME(gBattleMoves[MOVE_TOXIC].effect == EFFECT_SET_TOXIC_POISON);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_ABRA) { Ability(ABILITY_SYNCHRONIZE); }
    } WHEN {
        TURN { MOVE(player, MOVE_SUBSTITUTE); }
        TURN { MOVE(player, MOVE_TOXIC); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, player);
        
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TOXIC, player);
        MESSAGE("Foe Abra is badly poisoned!");
        STATUS_ICON(opponent, badPoison: TRUE);
        
        ABILITY_POPUP(opponent, ABILITY_SYNCHRONIZE);
        MESSAGE("Wobbuffet is badly poisoned!");
        STATUS_ICON(player, badPoison: TRUE);
    }
}

SINGLE_BATTLE_TEST("Synchronize isn't activated by Toxic Spikes")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_TOXIC_SPIKES].effect == EFFECT_TOXIC_SPIKES);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_ABRA) { Ability(ABILITY_SYNCHRONIZE); }
        
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_TOXIC_SPIKES); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TOXIC_SPIKES, opponent);
        STATUS_ICON(player, poison: TRUE);
        NOT ABILITY_POPUP(player, ABILITY_SYNCHRONIZE);
    }
}

DOUBLE_BATTLE_TEST("Synchronize activates for each Pokémon that was hit")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_DISCHARGE].target == MOVE_TARGET_FOES_AND_ALLY);
        ASSUME(MoveHasMoveEffect(MOVE_DISCHARGE, MOVE_EFFECT_PARALYSIS, FALSE) == TRUE);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_ABRA) { Ability(ABILITY_SYNCHRONIZE); }
        OPPONENT(SPECIES_ABRA) { Ability(ABILITY_SYNCHRONIZE); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_DISCHARGE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DISCHARGE, playerLeft);
        
        ABILITY_POPUP(opponentLeft, ABILITY_SYNCHRONIZE);
        STATUS_ICON(playerLeft, paralysis: TRUE);
        
        ABILITY_POPUP(opponentRight, ABILITY_SYNCHRONIZE); // Second's mon Synchronize activates, but attacker gets statused by Synchronize above
    }
}

SINGLE_BATTLE_TEST("Synchronize can cause another Pokémon's Synchronize activates")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_TOXIC].effect == EFFECT_SET_TOXIC_POISON);
        
        PLAYER(SPECIES_ABRA) { Ability(ABILITY_SYNCHRONIZE); }
        OPPONENT(SPECIES_ABRA) { Ability(ABILITY_SYNCHRONIZE); }
    } WHEN {
        TURN { MOVE(player, MOVE_TOXIC); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TOXIC, player);
        STATUS_ICON(opponent, badPoison: TRUE);
        
        ABILITY_POPUP(opponent, ABILITY_SYNCHRONIZE);
        STATUS_ICON(player, badPoison: TRUE);
        
        ABILITY_POPUP(player, ABILITY_SYNCHRONIZE);
    }
}

TO_DO_BATTLE_TEST("Synchronize is not activated by Toxic Orb and Flame Orb");

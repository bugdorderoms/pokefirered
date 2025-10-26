#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Lightning Rod absorbs Electric-type moves and boost Sp. Attack")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_THUNDER].type == TYPE_ELECTRIC);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_RAICHU) { Ability(ABILITY_LIGHTNING_ROD); }
    } WHEN {
        TURN { MOVE(player, MOVE_THUNDER); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_THUNDER, player);
        ABILITY_POPUP(opponent, ABILITY_LIGHTNING_ROD);
        MESSAGE("Foe Raichu's Sp. Atk rose!");
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_SPATK], DEFAULT_STAT_STAGES + 1);
    }
}

SINGLE_BATTLE_TEST("Lightning Rod absorbs the move even if its immune to it")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_THUNDER].type == TYPE_ELECTRIC);
        ASSUME(gSpeciesInfo[SPECIES_CUBONE].types[0] == TYPE_GROUND || gSpeciesInfo[SPECIES_CUBONE].types[1] == TYPE_GROUND);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CUBONE) { Ability(ABILITY_LIGHTNING_ROD); }
    } WHEN {
        TURN { MOVE(player, MOVE_THUNDER); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_THUNDER, player);
        ABILITY_POPUP(opponent, ABILITY_LIGHTNING_ROD);
        MESSAGE("Foe Cubone's Sp. Atk rose!");
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_SPATK], DEFAULT_STAT_STAGES + 1);
    }
}

DOUBLE_BATTLE_TEST("Lightning Rod draws in single-target Electric-type moves")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_THUNDER].type == TYPE_ELECTRIC);
        ASSUME(gBattleMoves[MOVE_THUNDER].target == MOVE_TARGET_SELECTED);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_RAICHU) { Ability(ABILITY_LIGHTNING_ROD); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_THUNDER, target: opponentRight);
            MOVE(playerRight, MOVE_THUNDER, target: opponentRight);
        }
    } SCENE {
        NOT HP_BAR(opponentRight);
        
        ABILITY_POPUP(opponentLeft, ABILITY_LIGHTNING_ROD);
        MESSAGE("Foe Raichu's Sp. Atk rose!");
        
        ABILITY_POPUP(opponentLeft, ABILITY_LIGHTNING_ROD);
        MESSAGE("Foe Raichu's Sp. Atk rose!");
    }
}

DOUBLE_BATTLE_TEST("Lightning Rod redirects an ally's attack")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_THUNDER].type == TYPE_ELECTRIC);
        ASSUME(gBattleMoves[MOVE_THUNDER].target == MOVE_TARGET_SELECTED);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_RAICHU) { Ability(ABILITY_LIGHTNING_ROD); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponentRight, MOVE_THUNDER, target: playerLeft); }
    } SCENE {
        NOT HP_BAR(playerLeft);
        ABILITY_POPUP(opponentLeft, ABILITY_LIGHTNING_ROD);
        MESSAGE("Foe Raichu took the attack!");
        MESSAGE("Foe Raichu's Sp. Atk rose!");
    }
}

DOUBLE_BATTLE_TEST("Lightning Rod redirected moves will never misses")
{
    PASSES_RANDOMLY(100, 100, RNG_ACCURACY);
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_THUNDER].type == TYPE_ELECTRIC);
        ASSUME(gBattleMoves[MOVE_THUNDER].target == MOVE_TARGET_SELECTED);
        ASSUME(gBattleMoves[MOVE_THUNDER].accuracy != 100);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_RAICHU) { Ability(ABILITY_LIGHTNING_ROD); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_THUNDER, target: opponentRight); }
    } SCENE {
        ABILITY_POPUP(opponentLeft, ABILITY_LIGHTNING_ROD);
        MESSAGE("Foe Raichu's Sp. Atk rose!");
    }
}

SINGLE_BATTLE_TEST("Lightning Rod is only triggered once on multi strike moves")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_FURY_SWIPES].type == TYPE_NORMAL);
        ASSUME(gBattleMoves[MOVE_FURY_SWIPES].effect == EFFECT_MULTI_HIT);
        
        PLAYER(SPECIES_RAICHU) { Ability(ABILITY_LIGHTNING_ROD); }
        OPPONENT(SPECIES_GRAVELER_ALOLAN) { Ability(ABILITY_GALVANIZE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_FURY_SWIPES); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_LIGHTNING_ROD);
        MESSAGE("Raichu's Sp. Atk rose!");
    }
}

DOUBLE_BATTLE_TEST("Lightning Rod does not activate if protected")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_THUNDER].type == TYPE_ELECTRIC);
        ASSUME(gBattleMoves[MOVE_PROTECT].effect == EFFECT_PROTECT);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_RAICHU) { Ability(ABILITY_LIGHTNING_ROD); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_PROTECT); MOVE(playerLeft, MOVE_THUNDER, target: opponentRight); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PROTECT, opponentLeft);
        ABILITY_POPUP(opponentLeft, ABILITY_LIGHTNING_ROD);
        MESSAGE("Foe Raichu took the attack!");
        NOT MESSAGE("Foe Raichu's Sp. Atk rose!");
    }
}

TO_DO_BATTLE_TEST("Lightning Rod can't redirect moves if its out of range in triples");
TO_DO_BATTLE_TEST("Lightning Rod activates before a held Cell Battery");

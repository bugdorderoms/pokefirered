#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_TRANSFORM].effect == EFFECT_TRANSFORM);
}

SINGLE_BATTLE_TEST("Transform copy the target's base form if Dynamaxed")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CHARIZARD) { GigantamaxFactor(TRUE); }
    } WHEN {
        TURN { MOVE(player, MOVE_TRANSFORM); MOVE(opponent, MOVE_SCRATCH, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_DYNAMAX_GROWTH, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TRANSFORM, player);
    } THEN {
        EXPECT_EQ(gBattleSpritesDataPtr->battlerData[B_POSITION_PLAYER_LEFT].transformSpecies, SPECIES_CHARIZARD);
    }
}

SINGLE_BATTLE_TEST("Transformed Pokémons can Gigantamax if it has the Gigantamax Factor and the target doesn't")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { GigantamaxFactor(TRUE); }
        OPPONENT(SPECIES_CHARIZARD) { Moves(MOVE_SCRATCH, MOVE_SPLASH); }
    } WHEN {
        TURN { MOVE(player, MOVE_TRANSFORM); MOVE(opponent, MOVE_SCRATCH, gimmick: GIMMICK_DYNAMAX); }
        TURN { MOVE(player, MOVE_SCRATCH, gimmick: GIMMICK_DYNAMAX); MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        // turn 1
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_DYNAMAX_GROWTH, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TRANSFORM, player);
        // turn 2
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_DYNAMAX_GROWTH, player);
    } THEN {
        EXPECT_EQ(player->species, SPECIES_CHARIZARD);
        EXPECT_EQ(gBattleSpritesDataPtr->battlerData[B_POSITION_PLAYER_LEFT].transformSpecies, SPECIES_CHARIZARD_GIGA);
    }
}

SINGLE_BATTLE_TEST("Dyna: Pokémons transformed into Zacian/Zamazenta/Eternatus can't Dynamax")
{
    u32 species;
    
    PARAMETRIZE { species = SPECIES_ZACIAN; }
    PARAMETRIZE { species = SPECIES_ZAMAZENTA; }
    PARAMETRIZE { species = SPECIES_ETERNATUS; }

    GIVEN {
        PLAYER(species) { Moves(MOVE_SCRATCH, MOVE_SPLASH); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_TRANSFORM); }
        TURN { MOVE(opponent, moveSlot: 0, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TRANSFORM, opponent);
        NOT MESSAGE("Foe %s grew huge into its Dynamax form!", species);
    }
}

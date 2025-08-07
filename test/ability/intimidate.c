#include "global.h"
#include "test/battle.h"

DOUBLE_BATTLE_TEST("Intimidate lowers opposing Pokemon's Attack")
{
    GIVEN {
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_INTIMIDATE);
        MESSAGE("Foe Wobbuffet's Attack fell!");
        MESSAGE("Foe Wynaut's Attack fell!");
    } THEN {
        EXPECT_EQ(opponentLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGES - 1);
        EXPECT_EQ(opponentRight->statStages[STAT_ATK], DEFAULT_STAT_STAGES - 1);
    }
}

DOUBLE_BATTLE_TEST("Intimidate doesn't affect a target behid Substitute")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SUBSTITUTE].effect == EFFECT_SUBSTITUTE);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_SUBSTITUTE); }
        TURN { SWITCH(playerLeft, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, opponentLeft);
        
        ABILITY_POPUP(playerLeft, ABILITY_INTIMIDATE);
        NOT MESSAGE("Foe Wobbuffet's Attack fell!");
        MESSAGE("Foe Wynaut's Attack fell!");
    } THEN {
        EXPECT_EQ(opponentLeft->statStages[STAT_ATK], DEFAULT_STAT_STAGES);
        EXPECT_EQ(opponentRight->statStages[STAT_ATK], DEFAULT_STAT_STAGES - 1);
    }
}

SINGLE_BATTLE_TEST("Intimidate is blocked by Oblivious, Scrappy, etc")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_SLOWPOKE; ability = ABILITY_OBLIVIOUS; }
    PARAMETRIZE { species = SPECIES_SLOWPOKE; ability = ABILITY_OWN_TEMPO; }
    PARAMETRIZE { species = SPECIES_ZUBAT; ability = ABILITY_INNER_FOCUS; }
    PARAMETRIZE { species = SPECIES_MILTANK; ability = ABILITY_SCRAPPY; }
    
    GIVEN {
        PLAYER(species) { Ability(ability); }
        OPPONENT(SPECIES_EKANS) { Ability(ABILITY_INTIMIDATE); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_INTIMIDATE);
        ABILITY_POPUP(player, ability);
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
    }
}

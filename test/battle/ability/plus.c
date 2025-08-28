#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_GUST].split == SPLIT_SPECIAL);
}

DOUBLE_BATTLE_TEST("Plus boosts the user's Sp. Attack by 50% if ally has Plus", s16 damage)
{
    u32 allyAbility;
    
    PARAMETRIZE { allyAbility = ABILITY_CLEAR_BODY; }
    PARAMETRIZE { allyAbility = ABILITY_PLUS; }
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_MAREEP) { Ability(ABILITY_PLUS); }
        OPPONENT(SPECIES_KLINK) { Ability(allyAbility); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_GUST, target: playerLeft); }
    } SCENE {
        HP_BAR(playerLeft, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.5), results[1].damage);
    }
}

DOUBLE_BATTLE_TEST("Plus boosts the user's Sp. Attack by 50% if ally has Minus", s16 damage)
{
    u32 allyAbility;
    
    PARAMETRIZE { allyAbility = ABILITY_CLEAR_BODY; }
    PARAMETRIZE { allyAbility = ABILITY_MINUS; }
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_MAREEP) { Ability(ABILITY_PLUS); }
        OPPONENT(SPECIES_KLINK) { Ability(allyAbility); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_GUST, target: playerLeft); }
    } SCENE {
        HP_BAR(playerLeft, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.5), results[1].damage);
    }
}

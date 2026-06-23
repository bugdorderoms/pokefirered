#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Adaptability increases same-type attack bonus from 1.5x to 2x", s16 damage)
{
    u32 ability;
    
    PARAMETRIZE { ability = ABILITY_HYPER_CUTTER; }
    PARAMETRIZE { ability = ABILITY_ADAPTABILITY; }
    
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_CRAWDAUNT].types[0] == TYPE_WATER || gSpeciesInfo[SPECIES_CRAWDAUNT].types[1] == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_WATER_GUN].type == TYPE_WATER);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CRAWDAUNT) { Ability(ability); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_WATER_GUN); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        // The jump from 1.5x STAB to 2.0x STAB is a 1.33x boost.
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.33), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Terastallizing into a different type with Adaptability gives 2.0x STAB", s16 damage)
{
    u32 gimmick;
    
    PARAMETRIZE { gimmick = GIMMICK_NONE; }
    PARAMETRIZE { gimmick = GIMMICK_TERA; }
    
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_CRAWDAUNT].types[0] != TYPE_NORMAL && gSpeciesInfo[SPECIES_CRAWDAUNT].types[1] != TYPE_NORMAL);
        ASSUME(gBattleMoves[MOVE_HEADBUTT].type == TYPE_NORMAL);
        
        PLAYER(SPECIES_CRAWDAUNT) { Ability(ABILITY_ADAPTABILITY); TeraType(TYPE_NORMAL); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_HEADBUTT, gimmick: gimmick); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HEADBUTT, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        // The jump from no STAB to 2.0x STAB is a 2.0x boost.
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(2.0), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("(Terastallizing into the same type with Adaptability gives 2.25x STAB", s16 damage)
{
    u32 gimmick;
    
    PARAMETRIZE { gimmick = GIMMICK_NONE; }
    PARAMETRIZE { gimmick = GIMMICK_TERA; }
    
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_CRAWDAUNT].types[0] == TYPE_WATER || gSpeciesInfo[SPECIES_CRAWDAUNT].types[1] == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_WATER_PULSE].type == TYPE_WATER);
        
        PLAYER(SPECIES_CRAWDAUNT) { Ability(ABILITY_ADAPTABILITY); TeraType(TYPE_WATER); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_WATER_PULSE, gimmick: gimmick); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WATER_PULSE, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        // The jump from 2x STAB to 2.25x STAB is a 1.125x boost.
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.125), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Adaptability does not increase non-Tera base STAB beyond 1.5x", s16 damage)
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_GUST; }
    PARAMETRIZE { move = MOVE_WATER_GUN; }
    
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_CRAWDAUNT].types[0] == TYPE_WATER || gSpeciesInfo[SPECIES_CRAWDAUNT].types[1] == TYPE_WATER);
        ASSUME(gSpeciesInfo[SPECIES_CRAWDAUNT].types[0] != gBattleMoves[MOVE_GUST].type && gSpeciesInfo[SPECIES_CRAWDAUNT].types[1] != gBattleMoves[MOVE_GUST].type);
        ASSUME(gBattleMoves[MOVE_GUST].type != TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_WATER_GUN].type == TYPE_WATER);
        
        PLAYER(SPECIES_CRAWDAUNT) { Ability(ABILITY_ADAPTABILITY); TeraType(TYPE_NORMAL); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, move, gimmick: GIMMICK_TERA); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        // With Adaptability, non-Tera base type should still be 1.5x STAB (not 2.0x).
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.5), results[1].damage);
    }
}

TO_DO_BATTLE_TEST("Adaptability does not affect Stellar-type moves");

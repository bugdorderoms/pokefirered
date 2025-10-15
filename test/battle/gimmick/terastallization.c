#include "global.h"
#include "form_change.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Tera: Terastallization changes type effectiveness")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_EARTHQUAKE].type == TYPE_GROUND);
        
        PLAYER(SPECIES_WOBBUFFET) { TeraType(TYPE_FLYING); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH, gimmick: GIMMICK_TERA); MOVE(opponent, MOVE_EARTHQUAKE); }
    } SCENE {
        MESSAGE("Wobbuffet is storing energy!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TERA_CHARGE, player);
        MESSAGE("Wobbuffet terastallized into the Flying type!");
        MESSAGE("It doesn't affect Wobbuffet…");
        NOT HP_BAR(player);
    }
}

SINGLE_BATTLE_TEST("Tera: Terastallizing into a different type preserves other STAB boosts", s16 damage1, s16 damage2)
{
    u32 gimmick;
    
    PARAMETRIZE { gimmick = GIMMICK_NONE; }
    PARAMETRIZE { gimmick = GIMMICK_TERA; }
    
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_BULBASAUR].types[0] == TYPE_GRASS && gSpeciesInfo[SPECIES_BULBASAUR].types[1] == TYPE_POISON);
        ASSUME(gBattleMoves[MOVE_VINE_WHIP].type == TYPE_GRASS);
        ASSUME(gBattleMoves[MOVE_SLUDGE_BOMB].type == TYPE_POISON);
        
        PLAYER(SPECIES_BULBASAUR) { TeraType(TYPE_NORMAL); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_VINE_WHIP, gimmick: gimmick); }
        TURN { MOVE(player, MOVE_SLUDGE_BOMB); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_VINE_WHIP, player);
        HP_BAR(opponent, captureDamage: &results[i].damage1);
        
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SLUDGE_BOMB, player);
        HP_BAR(opponent, captureDamage: &results[i].damage2);
    } FINALLY {
        EXPECT_EQ(results[0].damage1, results[1].damage1);
        EXPECT_EQ(results[0].damage2, results[1].damage2);
    }
}

SINGLE_BATTLE_TEST("Tera: Terastallizing does not affect the power of non-STAB moves", s16 damage)
{
    u32 gimmick;
    
    PARAMETRIZE { gimmick = GIMMICK_NONE; }
    PARAMETRIZE { gimmick = GIMMICK_TERA; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_HEADBUTT].type != TYPE_PSYCHIC);
        
        PLAYER(SPECIES_WOBBUFFET) { TeraType(TYPE_PSYCHIC); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_HEADBUTT, gimmick: gimmick); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Tera: Terastallizing into a different type gives that type 1.5x STAB", s16 damage)
{
    u32 gimmick;
    
    PARAMETRIZE { gimmick = GIMMICK_NONE; }
    PARAMETRIZE { gimmick = GIMMICK_TERA; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_HEADBUTT].type == TYPE_NORMAL);
        
        PLAYER(SPECIES_WOBBUFFET) { TeraType(TYPE_NORMAL); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_HEADBUTT, gimmick: gimmick); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        // The jump from no STAB to 1.5x STAB is a 1.5x boost.
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.5), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Tera: Terastallizing into the same type gives that type 2x STAB", s16 damage)
{
    u32 gimmick;
    
    PARAMETRIZE { gimmick = GIMMICK_NONE; }
    PARAMETRIZE { gimmick = GIMMICK_TERA; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_PSYCHIC].type == TYPE_PSYCHIC);
        
        PLAYER(SPECIES_WOBBUFFET) { TeraType(TYPE_PSYCHIC); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_PSYCHIC, gimmick: gimmick); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        // The jump from 1.5x STAB to 2.0x STAB is a 1.33x boost.
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.33), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Tera: Terastallizing boosts moves of the same type to 60 BP", s16 damage)
{
    u32 gimmick;
    
    PARAMETRIZE { gimmick = GIMMICK_NONE; }
    PARAMETRIZE { gimmick = GIMMICK_TERA; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_VINE_WHIP].power == 45);
        ASSUME(gBattleMoves[MOVE_VINE_WHIP].type == TYPE_GRASS);
        
        PLAYER(SPECIES_WOBBUFFET) { TeraType(TYPE_GRASS); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_VINE_WHIP, gimmick: gimmick); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        // The jump from 45 BP to 90 BP (60 * 1.5x) is a 2.0x boost.
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(2.0), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Tera: Terastallization's 60 BP floor does not apply to multi-hit moves", s16 damage)
{
    u32 gimmick;
    
    PARAMETRIZE { gimmick = GIMMICK_NONE; }
    PARAMETRIZE { gimmick = GIMMICK_TERA; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_FURY_SWIPES].effect == EFFECT_MULTI_HIT);
        ASSUME(gBattleMoves[MOVE_FURY_SWIPES].type == TYPE_NORMAL);
        
        PLAYER(SPECIES_WOBBUFFET) { TeraType(TYPE_NORMAL); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_FURY_SWIPES, gimmick: gimmick); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.5), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Tera: Terastallization's 60 BP floor does not apply to priority moves", s16 damage)
{
    u32 gimmick;
    
    PARAMETRIZE { gimmick = GIMMICK_NONE; }
    PARAMETRIZE { gimmick = GIMMICK_TERA; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_QUICK_ATTACK].priority > 0);
        ASSUME(gBattleMoves[MOVE_QUICK_ATTACK].type == TYPE_NORMAL);
        
        PLAYER(SPECIES_WOBBUFFET) { TeraType(TYPE_NORMAL); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_QUICK_ATTACK, gimmick: gimmick); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.5), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Tera: Terastallization's 60 BP floor does not apply to dynamic base power moves", s16 damage)
{
    u32 gimmick;
    
    PARAMETRIZE { gimmick = GIMMICK_NONE; }
    PARAMETRIZE { gimmick = GIMMICK_TERA; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_WATER_SPOUT].effect == EFFECT_ERUPTION);
        ASSUME(gBattleMoves[MOVE_WATER_SPOUT].type == TYPE_WATER);
        
        PLAYER(SPECIES_WOBBUFFET) { HP(1); TeraType(TYPE_WATER); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_WATER_SPOUT, gimmick: gimmick); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Tera: Stellar type does not change the user's defensive profile", s16 damage)
{
    u32 gimmick;
    
    PARAMETRIZE { gimmick = GIMMICK_NONE; }
    PARAMETRIZE { gimmick = GIMMICK_TERA; }
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { TeraType(TYPE_STELLAR); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH, gimmick: gimmick); MOVE(opponent, MOVE_PSYCHIC); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Tera: Terastallizing into the Stellar-type provides a one-time 2.0x boost to STAB moves")
{
    s16 damage[3];
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_EXTRASENSORY].type == TYPE_PSYCHIC);
        
        PLAYER(SPECIES_WOBBUFFET) { TeraType(TYPE_STELLAR); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_EXTRASENSORY); }
        TURN { MOVE(player, MOVE_EXTRASENSORY, gimmick: GIMMICK_TERA); }
        TURN { MOVE(player, MOVE_EXTRASENSORY); }
    } SCENE {
        // turn 1
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EXTRASENSORY, player);
        HP_BAR(opponent, captureDamage: &damage[0]);
        
        // turn 2
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EXTRASENSORY, player);
        HP_BAR(opponent, captureDamage: &damage[1]);
        
        // turn 3
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EXTRASENSORY, player);
        HP_BAR(opponent, captureDamage: &damage[2]);
    } THEN {
        // Extrasensory goes from a 50% boost to a 100% boost for a 1.33x total multiplier
        EXPECT_MUL_EQ(damage[0], UQ_4_12(1.33), damage[1]);
        EXPECT_EQ(damage[0], damage[2]);
    }
}

SINGLE_BATTLE_TEST("Tera: Terastallizing into the Stellar-type provides a one-time 1.2x boost to non-STAB moves")
{
    s16 damage[3];
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { TeraType(TYPE_STELLAR); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_TAKE_DOWN); }
        TURN { MOVE(player, MOVE_TAKE_DOWN, gimmick: GIMMICK_TERA); }
        TURN { MOVE(player, MOVE_TAKE_DOWN); }
    } SCENE {
        // turn 1
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TAKE_DOWN, player);
        HP_BAR(opponent, captureDamage: &damage[0]);
        
        // turn 2
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TAKE_DOWN, player);
        HP_BAR(opponent, captureDamage: &damage[1]);
        
        // turn 3
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TAKE_DOWN, player);
        HP_BAR(opponent, captureDamage: &damage[2]);
    } THEN {
        EXPECT_MUL_EQ(damage[0], UQ_4_12(1.2), damage[1]);
        EXPECT_EQ(damage[0], damage[2]);
    }
}

SINGLE_BATTLE_TEST("Tera: Terastallizing into the Stellar type boosts all moves up to 60 BP once per type")
{
    s16 damage[4];
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_MEGA_DRAIN].power == 40);
        ASSUME(gBattleMoves[MOVE_WATER_GUN].power == 40);
        
        PLAYER(SPECIES_WOBBUFFET) { TeraType(TYPE_STELLAR); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_MEGA_DRAIN); }
        TURN { MOVE(player, MOVE_MEGA_DRAIN, gimmick: GIMMICK_TERA); }
        TURN { MOVE(player, MOVE_MEGA_DRAIN); }
        TURN { MOVE(player, MOVE_WATER_GUN); }
    } SCENE {
        // turn 1
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MEGA_DRAIN, player);
        HP_BAR(opponent, captureDamage: &damage[0]);
        
        // turn 2
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MEGA_DRAIN, player);
        HP_BAR(opponent, captureDamage: &damage[1]);
        
        // turn 3
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MEGA_DRAIN, player);
        HP_BAR(opponent, captureDamage: &damage[2]);
        
        // turn 4
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WATER_GUN, player);
        HP_BAR(opponent, captureDamage: &damage[3]);
    } THEN {
        // The jump from 40 BP to 72 BP (60 * 1.2x) is a 1.8x boost.
        EXPECT_MUL_EQ(damage[0], Q_4_12(1.8), damage[1]);
        EXPECT_EQ(damage[0], damage[2]);
        EXPECT_EQ(damage[1], damage[3]);
    }
}

SINGLE_BATTLE_TEST("Tera: Status moves don't expend Stellar's one-time type boost")
{
    s16 damage[2];
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_GROWL].type == gBattleMoves[MOVE_TAKE_DOWN].type);
        
        PLAYER(SPECIES_WOBBUFFET) { TeraType(TYPE_STELLAR); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_GROWL, gimmick: GIMMICK_TERA); }
        TURN { MOVE(player, MOVE_TAKE_DOWN); }
        TURN { MOVE(player, MOVE_TAKE_DOWN); }
    } SCENE {
        // turn 1
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GROWL, player);
        
        // turn 2
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TAKE_DOWN, player);
        HP_BAR(opponent, captureDamage: &damage[0]);
        
        // turn 3
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TAKE_DOWN, player);
        HP_BAR(opponent, captureDamage: &damage[1]);
    } THEN {
        EXPECT_MUL_EQ(damage[1], UQ_4_12(1.20), damage[0]);
    }
}

SINGLE_BATTLE_TEST("Tera: Terastallization persist even when switched out")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_EARTHQUAKE].type == TYPE_GROUND);
        
        PLAYER(SPECIES_WOBBUFFET) { TeraType(TYPE_FLYING); }
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH, gimmick: GIMMICK_TERA); MOVE(opponent, MOVE_EARTHQUAKE); }
        TURN { SWITCH(player, 1); }
        TURN { SWITCH(player, 0); }
        TURN { MOVE(opponent, MOVE_EARTHQUAKE); }
    } SCENE {
        NONE_OF {
            // turn 1
            ANIMATION(ANIM_TYPE_MOVE, MOVE_EARTHQUAKE, opponent);
            HP_BAR(player);
            
            // turn 4
            ANIMATION(ANIM_TYPE_MOVE, MOVE_EARTHQUAKE, opponent);
            HP_BAR(player);
        }
    }
}

SINGLE_BATTLE_TEST("Tera: Illusion doesn't break upon Terastallizing when illusioned as a mon that doesn't change forms by Terastallizing")
{
    u32 species;
    
    PARAMETRIZE { species = SPECIES_WOBBUFFET; }
    PARAMETRIZE { species = SPECIES_ZIGZAGOON; }
    
    GIVEN {
        ASSUME(SpeciesHasFormChangeType(species, FORM_CHANGE_TERASTAL) == FALSE);
        
        PLAYER(SPECIES_ZOROARK) { TeraType(TYPE_BUG); }
        PLAYER(species);
        
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH, gimmick: GIMMICK_TERA); MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ILLUSION_OFF, player);
    }
}

TO_DO_BATTLE_TEST("Tera: Illusion breaks if the Pokémon Terastallizes when illusioned as a mon that changes forms by Terastallizing");
TO_DO_BATTLE_TEST("Tera: Pokemon with Tera forms change upon Terastallizing");
TO_DO_BATTLE_TEST("Tera: Burn Up/Double Shock dos not remove the user's type while Terastallized");
TO_DO_BATTLE_TEST("Tera: Stellar type boost isn't consumed if used by Terapagos Stellar");
TO_DO_BATTLE_TEST("Tera: Stellar type boost isn't consumed in tera raid battles");

#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_SUNNY_DAY].effect == EFFECT_SET_WEATHER);
    ASSUME(gBattleMoves[MOVE_SUNNY_DAY].argument.setWeather.weatherId == ENUM_WEATHER_SUN);
}

SINGLE_BATTLE_TEST("Flower Gift transforms Cherrim in sun")
{
    GIVEN {
        PLAYER(SPECIES_CHERRIM) { Ability(ABILITY_FLOWER_GIFT); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUNNY_DAY); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_FLOWER_GIFT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Cherrim transformed!");
    } THEN {
        EXPECT_EQ(player->species, SPECIES_CHERRIM_SUNSHINE);
    }
}

SINGLE_BATTLE_TEST("Flower Gift transforms Cherrim back when the weather ends")
{
    GIVEN {
        PLAYER(SPECIES_CHERRIM) { Ability(ABILITY_FLOWER_GIFT); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUNNY_DAY); }
        TURN { }
        TURN { }
        TURN { }
        TURN { }
    } SCENE {
        // transforms
        ABILITY_POPUP(player, ABILITY_FLOWER_GIFT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Cherrim transformed!");
        // back to normal
        ABILITY_POPUP(player, ABILITY_FLOWER_GIFT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Cherrim transformed!");
    } THEN {
        EXPECT_EQ(player->species, SPECIES_CHERRIM);
    }
}

SINGLE_BATTLE_TEST("Flower Gift transforms Cherrim back under Cloud Nine/Air Lock")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_PSYDUCK; ability = ABILITY_CLOUD_NINE; }
    PARAMETRIZE { species = SPECIES_RAYQUAZA; ability = ABILITY_AIR_LOCK; }
    
    GIVEN {
        PLAYER(SPECIES_CHERRIM) { Ability(ABILITY_FLOWER_GIFT); }
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(species) { Ability(ability); }
    } WHEN {
        TURN { MOVE(player, MOVE_SUNNY_DAY); }
        TURN { SWITCH(opponent, 1); }
    } SCENE {
        // transforms
        ABILITY_POPUP(player, ABILITY_FLOWER_GIFT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Cherrim transformed!");
        // back to normal
        ABILITY_POPUP(opponent, ability);
        ABILITY_POPUP(player, ABILITY_FLOWER_GIFT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Cherrim transformed!");
    } THEN {
        EXPECT_EQ(player->species, SPECIES_CHERRIM);
    }
}

SINGLE_BATTLE_TEST("Flower Gift transforms Cherrim back when its replaced")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_ROLE_PLAY].effect == EFFECT_ROLE_PLAY);
        
        PLAYER(SPECIES_CHERRIM) { Ability(ABILITY_FLOWER_GIFT); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUNNY_DAY); MOVE(player, MOVE_ROLE_PLAY); }
    } SCENE {
        // transforms
        ABILITY_POPUP(player, ABILITY_FLOWER_GIFT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Cherrim transformed!");
        // back to normal
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Cherrim transformed!");
    } THEN {
        EXPECT_EQ(player->species, SPECIES_CHERRIM);
    }
}

SINGLE_BATTLE_TEST("Flower Gift transforms Cherrim back when its suppressed")
{
    GIVEN {
        PLAYER(SPECIES_CHERRIM) { Ability(ABILITY_FLOWER_GIFT); }
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_KOFFING) { Ability(ABILITY_NEUTRALIZING_GAS); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUNNY_DAY); }
        TURN { SWITCH(opponent, 1); }
    } SCENE {
        // transforms
        ABILITY_POPUP(player, ABILITY_FLOWER_GIFT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Cherrim transformed!");
        // back to normal
        ABILITY_POPUP(opponent, ABILITY_NEUTRALIZING_GAS);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Cherrim transformed!");
    } THEN {
        EXPECT_EQ(player->species, SPECIES_CHERRIM);
    }
}

SINGLE_BATTLE_TEST("Flower Gift doesn't transforms Cherrim back if dynamaxed, even when it ends")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_GASTRO_ACID].effect == EFFECT_GASTRO_ACID);
        
        PLAYER(SPECIES_CHERRIM) { Ability(ABILITY_FLOWER_GIFT); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, gimmick: GIMMICK_DYNAMAX); MOVE(opponent, MOVE_SUNNY_DAY); }
        TURN { MOVE(player, MOVE_SCRATCH); MOVE(opponent, MOVE_GASTRO_ACID); }
        TURN { MOVE(player, MOVE_SCRATCH); }
        TURN { }
        TURN { }
    } SCENE {
        // transforms
        ABILITY_POPUP(player, ABILITY_FLOWER_GIFT);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Cherrim transformed!");
        
        // doesn't transforms back
        MESSAGE("Cherrim is no longer Dynamaxed!");

        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
            MESSAGE("Cherrim transformed!");
        }
    } THEN {
        EXPECT_EQ(player->species, SPECIES_CHERRIM_SUNSHINE);
    }
}

DOUBLE_BATTLE_TEST("Flower Gift increases the attack of Cherrim and its allies by 1.5x", s16 damageL, s16 damageR)
{
    bool32 sunny;
    
    PARAMETRIZE { sunny = FALSE; }
    PARAMETRIZE { sunny = TRUE; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].split == SPLIT_PHYSICAL);
        
        PLAYER(SPECIES_CHERRIM) { Ability(ABILITY_FLOWER_GIFT); }
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        if (sunny) {
            TURN { MOVE(playerLeft, MOVE_SUNNY_DAY); }
        }
        TURN { MOVE(playerLeft, MOVE_SCRATCH, target: opponentLeft);
               MOVE(playerRight, MOVE_SCRATCH, target: opponentLeft); }
    } SCENE {
        // sun activates
        if (sunny) {
            ABILITY_POPUP(playerLeft, ABILITY_FLOWER_GIFT);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, playerLeft);
            MESSAGE("Cherrim transformed!");
        }
        // player uses Scratch
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, playerLeft);
        HP_BAR(opponentLeft, captureDamage: &results[i].damageL);
        
        // partner uses Scratch
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, playerRight);
        HP_BAR(opponentLeft, captureDamage: &results[i].damageR);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damageL, UQ_4_12(1.5), results[1].damageL);
        EXPECT_MUL_EQ(results[0].damageR, UQ_4_12(1.5), results[1].damageR);
    }
}

DOUBLE_BATTLE_TEST("Flower Gift increases the Sp. Def of Cherrim and its allies by 1.5x", s16 damageL, s16 damageR)
{
    bool32 sunny;
    
    PARAMETRIZE { sunny = FALSE; }
    PARAMETRIZE { sunny = TRUE; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_HYPER_VOICE].split == SPLIT_SPECIAL);
        
        PLAYER(SPECIES_CHERRIM) { Ability(ABILITY_FLOWER_GIFT); }
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        if (sunny) {
            TURN { MOVE(playerLeft, MOVE_SUNNY_DAY); }
        }
        TURN { MOVE(opponentLeft, MOVE_HYPER_VOICE, target: playerLeft); }
    } SCENE {
        // sun activates
        if (sunny) {
            ABILITY_POPUP(playerLeft, ABILITY_FLOWER_GIFT);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, playerLeft);
            MESSAGE("Cherrim transformed!");
        }
        // opponentLeft uses Hyper Voice
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_VOICE, opponentLeft);
        HP_BAR(playerLeft, captureDamage: &results[i].damageL);
        HP_BAR(playerRight, captureDamage: &results[i].damageR);
    } FINALLY {
        EXPECT_MUL_EQ(results[1].damageL, UQ_4_12(1.5), results[0].damageL);
        EXPECT_MUL_EQ(results[1].damageR, UQ_4_12(1.5), results[0].damageR);
    }
}

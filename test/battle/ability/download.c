#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Download raises Attack or Sp Attack based on opposing Pokémon's defensive stats")
{
    u32 def, spDef;
    bool32 raisesAttack;
    
    PARAMETRIZE { def = 99; spDef = 100; raisesAttack = TRUE; }
    PARAMETRIZE { def = 100; spDef = 99; raisesAttack = FALSE; }
    
    GIVEN {
        PLAYER(SPECIES_PORYGON) { Ability(ABILITY_DOWNLOAD); }
        OPPONENT(SPECIES_WOBBUFFET) { Defense(def); SpDefense(spDef); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_DOWNLOAD);
        
        if (raisesAttack) {
            MESSAGE("Porygon's Attack rose!");
        } else {
            MESSAGE("Porygon's Sp. Atk rose!");
        }
    } THEN {
        EXPECT_EQ(player->statStages[raisesAttack ? STAT_ATK : STAT_SPATK], DEFAULT_STAT_STAGES + 1);
    }
}

DOUBLE_BATTLE_TEST("Download raises stats based on the average of opposing Pokémon's defensive stats")
{
    u32 def1, spDef1, def2, spDef2;
    bool32 raisesAttack;
    
    PARAMETRIZE { def1 = 99; def2 = 99; spDef1 = 100; spDef2 = 100; raisesAttack = TRUE; }
    PARAMETRIZE { def1 = 100; def2 = 100; spDef1 = 99; spDef2 = 99; raisesAttack = FALSE; }
    
    GIVEN {
        PLAYER(SPECIES_PORYGON) { Ability(ABILITY_DOWNLOAD); }
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET) { Defense(def1); SpDefense(spDef1); }
        OPPONENT(SPECIES_WOBBUFFET) { Defense(def2); SpDefense(spDef2); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_DOWNLOAD);
        
        if (raisesAttack) {
            MESSAGE("Porygon's Attack rose!");
        } else {
            MESSAGE("Porygon's Sp. Atk rose!");
        }
    } THEN {
        EXPECT_EQ(playerLeft->statStages[raisesAttack ? STAT_ATK : STAT_SPATK], DEFAULT_STAT_STAGES + 1);
    }
}

SINGLE_BATTLE_TEST("Download raises Sp. Attack in a tie with the two stats")
{
    GIVEN {
        PLAYER(SPECIES_PORYGON) { Ability(ABILITY_DOWNLOAD); }
        OPPONENT(SPECIES_WOBBUFFET) { Defense(100); SpDefense(100); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_DOWNLOAD);
        MESSAGE("Porygon's Sp. Atk rose!");
    } THEN {
        EXPECT_EQ(player->statStages[STAT_SPATK], DEFAULT_STAT_STAGES + 1);
    }
}

SINGLE_BATTLE_TEST("Download takes opposing Pokémon's stat stages into account")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_HARDEN].effect == EFFECT_DEFENSE_UP);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_PORYGON) { Ability(ABILITY_DOWNLOAD); }
        
        OPPONENT(SPECIES_WOBBUFFET) { Defense(90); SpDefense(100); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_HARDEN); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HARDEN, opponent);
        ABILITY_POPUP(player, ABILITY_DOWNLOAD);
        MESSAGE("Porygon's Sp. Atk rose!");
    } THEN {
        EXPECT_EQ(player->statStages[STAT_SPATK], DEFAULT_STAT_STAGES + 1);
    }
}

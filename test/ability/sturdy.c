#include "global.h"
#include "test/battle.h"

#undef CONFUSION_UPDATE
#define CONFUSION_UPDATE TRUE

SINGLE_BATTLE_TEST("Sturdy prevents OHKO moves")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_FISSURE].effect == EFFECT_OHKO);
        
        PLAYER(SPECIES_GEODUDE) { Ability(ABILITY_STURDY); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_FISSURE); }
    } SCENE {
        MESSAGE("Foe Wobbuffet used Fissure!");
        ABILITY_POPUP(player, ABILITY_STURDY);
        MESSAGE("Geodude was protected by Sturdy!");
    } THEN {
        EXPECT_EQ(player->hp, player->maxHP);
    }
}

SINGLE_BATTLE_TEST("Sturdy prevents fainting from OHKOs")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SEISMIC_TOSS].effect == EFFECT_USER_LEVEL_TO_DAMAGE);
        
        PLAYER(SPECIES_GEODUDE) { Ability(ABILITY_STURDY); MaxHP(100); HP(100); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SEISMIC_TOSS); }
    } SCENE {
        MESSAGE("Foe Wobbuffet used Seismic Toss!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SEISMIC_TOSS, opponent);
        HP_BAR(player, hp: 1);
        ABILITY_POPUP(player, ABILITY_STURDY);
        MESSAGE("Geodude endured the hit!");
    }
}

SINGLE_BATTLE_TEST("Sturdy prevents fainting from confusion damage")
{
    PASSES_RANDOMLY(5, 10); // 50% chance confusion activating
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_CONFUSE_RAY].effect == EFFECT_CONFUSE);
        
        PLAYER(SPECIES_GEODUDE) { Ability(ABILITY_STURDY); MaxHP(2); HP(2); Speed(4); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(5); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_CONFUSE_RAY); }
    } SCENE {
        MESSAGE("Foe Wobbuffet used Confuse Ray!");
        
        MESSAGE("Geodude is confused!");
        MESSAGE("It hurt itself in its confusion!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_MON_HIT, player);
        HP_BAR(player, hp: 1);
        ABILITY_POPUP(player, ABILITY_STURDY);
        MESSAGE("Geodude endured the hit!");
    }
}

// TODO:
// Sturdy activates before a held Focus Sash

#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Sturdy prevents OHKO moves")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_FISSURE].effect == EFFECT_OHKO);
        
        PLAYER(SPECIES_GEODUDE) { Ability(ABILITY_STURDY); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_FISSURE); }
    } SCENE {
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
        HP_BAR(player, hp: 1);
        ABILITY_POPUP(player, ABILITY_STURDY);
        MESSAGE("Geodude endured the hit!");
    }
}

SINGLE_BATTLE_TEST("Sturdy prevents fainting from confusion damage")
{
    PASSES_RANDOMLY(CONFUSION_UPDATE ? 33 : 50, 100, RNG_CONFUSION);
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_CONFUSE_RAY].effect == EFFECT_CONFUSE);
        
        PLAYER(SPECIES_GEODUDE) { Ability(ABILITY_STURDY); MaxHP(1); HP(1); Speed(4); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(5); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_CONFUSE_RAY); }
    } SCENE {
        MESSAGE("Geodude is confused!");
        MESSAGE("It hurt itself in its confusion!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_MON_HIT, player);
        HP_BAR(player, hp: 1);
        ABILITY_POPUP(player, ABILITY_STURDY);
        MESSAGE("Geodude endured the hit!");
    }
}

TO_DO_BATTLE_TEST("Sturdy activates before a held Focus Sash");

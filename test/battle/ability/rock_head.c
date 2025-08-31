#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Rock Head prevents recoil damage")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_FLARE_BLITZ].recoilDivisor > 0);
        
        PLAYER(SPECIES_GEODUDE) { Ability(ABILITY_ROCK_HEAD); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_FLARE_BLITZ); }
    } SCENE {
        NONE_OF {
            HP_BAR(player);
            MESSAGE("Geodude is hit with recoil!");
        }
    }
}

SINGLE_BATTLE_TEST("Rock Head doesn't prevent recoil if replaced by Mummy")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_FLARE_BLITZ].recoilDivisor > 0);
        ASSUME(gBattleMoves[MOVE_FLARE_BLITZ].flags.makesContact == TRUE);
        
        PLAYER(SPECIES_GEODUDE) { Ability(ABILITY_ROCK_HEAD); }
        OPPONENT(SPECIES_YAMASK) { Ability(ABILITY_MUMMY); }
    } WHEN {
        TURN { MOVE(player, MOVE_FLARE_BLITZ); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_MUMMY);
        HP_BAR(player);
        MESSAGE("Geodude is hit with recoil!");
    }
}

SINGLE_BATTLE_TEST("Rock Head doesn't prevent recoil from Struggle")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_STRUGGLE].recoilDivisor > 0);
        
        PLAYER(SPECIES_GEODUDE) { Ability(ABILITY_ROCK_HEAD); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_STRUGGLE); }
    } SCENE {
        HP_BAR(player);
        MESSAGE("Geodude is hit with recoil!");
    }
}

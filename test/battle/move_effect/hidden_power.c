#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Hidden Power's type isn't affected by Normalize, etc")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_HIDDEN_POWER].effect == EFFECT_HIDDEN_POWER);
        ASSUME(gSpeciesInfo[SPECIES_DIGLETT].types[0] == TYPE_GROUND);
        
        PLAYER(SPECIES_SKITTY) { Ability(ABILITY_NORMALIZE); HPIV(31); AttackIV(31); DefenseIV(31); SpAttackIV(30); SpDefenseIV(31); SpeedIV(31); } // HP electric
        OPPONENT(SPECIES_DIGLETT);
    } WHEN {
        TURN { MOVE(player, MOVE_HIDDEN_POWER); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_HIDDEN_POWER, player);
        MESSAGE("It doesn't affect Foe Diglett…");
    }
}

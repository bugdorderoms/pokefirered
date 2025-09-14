#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_MEGA_PUNCH].accuracy < 100);
}

SINGLE_BATTLE_TEST("No Guard makes moves used by or against it never miss")
{
    struct BattlePokemon *attacker;
    
    PARAMETRIZE { attacker = player; }
    PARAMETRIZE { attacker = opponent; }
    
    GIVEN {
        PLAYER(SPECIES_MACHOP) { Ability(ABILITY_NO_GUARD); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(attacker, MOVE_MEGA_PUNCH, hit: FALSE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MEGA_PUNCH, attacker);
        NOT MESSAGE("%s's attack missed!", attacker->species);
    }
}

SINGLE_BATTLE_TEST("No Guard makes moves hit even if semi-invulnerable")
{
    struct BattlePokemon *attacker, *invulBattler;
    
    PARAMETRIZE { attacker = player; invulBattler = opponent; }
    PARAMETRIZE { attacker = opponent; invulBattler = player; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_FLY].effect == EFFECT_SEMI_INVULNERABLE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_MACHOP) { Ability(ABILITY_NO_GUARD); }
    } WHEN {
        TURN { MOVE(invulBattler, MOVE_FLY); MOVE(attacker, MOVE_MEGA_PUNCH, hit: FALSE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FLY, invulBattler);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MEGA_PUNCH, attacker);
        NOT MESSAGE("%s's attack missed!", attacker->species);
    }
}

TO_DO_BATTLE_TEST("No Guard can't hit a Tatsugiri inside Dondozo");

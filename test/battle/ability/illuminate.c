#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Illuminate/Keen Eye prevents accuracy stat reduction")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_LANTURN; ability = ABILITY_ILLUMINATE; }
    PARAMETRIZE { species = SPECIES_HITMONCHAN; ability = ABILITY_KEEN_EYE; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SAND_ATTACK].effect == EFFECT_ACCURACY_DOWN);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(species) { Ability(ability); }
    } WHEN {
        TURN { MOVE(player, MOVE_SAND_ATTACK); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SAND_ATTACK, player);
        ABILITY_POPUP(opponent, ability);
        MESSAGE("Foe %s prevents accuracy loss!", species);
    }
}

#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Limber prevents paralysis")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_THUNDER_WAVE].effect == EFFECT_SET_PARALYZE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_PERSIAN) { Ability(ABILITY_LIMBER); }
    } WHEN {
        TURN { MOVE(player, MOVE_THUNDER_WAVE); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_LIMBER);
        MESSAGE("Foe Persian's Limber prevents paralysis!");
        
        NONE_OF {
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PRZ, opponent);
            STATUS_ICON(opponent, paralysis: TRUE);
        }
    }
}

SINGLE_BATTLE_TEST("Limber cures paralysis if paralyzed")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_THUNDER_WAVE].effect == EFFECT_SET_PARALYZE);
        
        PLAYER(SPECIES_PINSIR) { Ability(ABILITY_MOLD_BREAKER); }
        OPPONENT(SPECIES_PERSIAN) { Ability(ABILITY_LIMBER); }
    } WHEN {
        TURN { MOVE(player, MOVE_THUNDER_WAVE); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_LIMBER);
        MESSAGE("Foe Persian's Limber cured its paralysis problem!");
        STATUS_ICON(opponent, none: TRUE);
    }
}

SINGLE_BATTLE_TEST("Limber cures paralysis when it enters battle")
{
    GIVEN {
        PLAYER(SPECIES_PERSIAN) { Ability(ABILITY_LIMBER); Status1(STATUS1_PARALYSIS); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_LIMBER);
        MESSAGE("Persian was cured of its paralysis!");
        STATUS_ICON(player, none: TRUE);
    }
}

TO_DO_BATTLE_TEST("A held berry will activate and cure the paralysis before Limber activates");

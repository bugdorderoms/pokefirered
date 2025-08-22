#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Magma Armor prevents freeze")
{
    GIVEN {
        ASSUME(MoveHasMoveEffect(MOVE_ICE_PUNCH, MOVE_EFFECT_FREEZE, FALSE) == TRUE);

        PLAYER(SPECIES_SLUGMA) { Ability(ABILITY_MAGMA_ARMOR); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_ICE_PUNCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ICE_PUNCH, opponent);
        NOT STATUS_ICON(player, freeze: TRUE);
    }
}

SINGLE_BATTLE_TEST("Magma Armor thaws it out if its frozen")
{
    GIVEN {
        ASSUME(MoveHasMoveEffect(MOVE_ICE_PUNCH, MOVE_EFFECT_FREEZE, FALSE) == TRUE);
        
        PLAYER(SPECIES_PINSIR) { Ability(ABILITY_MOLD_BREAKER); }
        OPPONENT(SPECIES_SLUGMA) { Ability(ABILITY_MAGMA_ARMOR); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SPLASH); MOVE(player, MOVE_ICE_PUNCH); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_MAGMA_ARMOR);
        MESSAGE("Foe Slugma's Magma Armor cured its freeze problem!");
        STATUS_ICON(opponent, none: TRUE);
    }
}

SINGLE_BATTLE_TEST("Magma Armor thaws it out when it enters battle")
{
    GIVEN {
        PLAYER(SPECIES_SLUGMA) { Ability(ABILITY_MAGMA_ARMOR); Status1(STATUS1_FREEZE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_MAGMA_ARMOR);
        MESSAGE("Slugma was cured of its freeze!");
        STATUS_ICON(player, none: TRUE);
    }
}

TO_DO_BATTLE_TEST("A held berry will activate and cures the freeze before Magma Armor activates");

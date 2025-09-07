#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Clear Body/White Smoke prevents stat reduction")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_METANG; ability = ABILITY_CLEAR_BODY; }
    PARAMETRIZE { species = SPECIES_HEATMOR; ability = ABILITY_WHITE_SMOKE; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SAND_ATTACK].effect == EFFECT_ACCURACY_DOWN);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(species) { Ability(ability); }
    } WHEN {
        TURN { MOVE(player, MOVE_SAND_ATTACK); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SAND_ATTACK, player);
        ABILITY_POPUP(opponent, ability);
        MESSAGE("Foe %s prevents stat loss!", species);
    }
}

SINGLE_BATTLE_TEST("Clear Body/White Smoke does not prevents self-inflicted stat reduction")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_METANG; ability = ABILITY_CLEAR_BODY; }
    PARAMETRIZE { species = SPECIES_HEATMOR; ability = ABILITY_WHITE_SMOKE; }
    
    GIVEN {
        ASSUME(MoveHasMoveEffect(MOVE_SUPERPOWER, MOVE_EFFECT_ATK_DEF_DOWN, TRUE) == TRUE);
        
        PLAYER(species) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SUPERPOWER); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUPERPOWER, player);
        NOT ABILITY_POPUP(player, ability);
    }
}

TO_DO_BATTLE_TEST("Clear Body/White Smoke prevents Sticky Web's Speed drop");

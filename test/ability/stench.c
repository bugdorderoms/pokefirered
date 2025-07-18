#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Stench has a 10% chance to flinch")
{
    PASSES_RANDOMLY(1, 10, RNG_STENCH);
    
    GIVEN {
        PLAYER(SPECIES_GRIMER) { Ability(ABILITY_STENCH); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("Foe Wobbuffet flinched!");
    }
}

SINGLE_BATTLE_TEST("Stench only triggers if target takes damage")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCARY_FACE].power == 0);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_GRIMER) { Ability(ABILITY_STENCH); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCARY_FACE, WITH_RNG(RNG_STENCH, TRUE)); }
    } SCENE {
        NOT MESSAGE("Wynaut flinched!");
    }
}

SINGLE_BATTLE_TEST("Stench does not stack with King's Rock")
{
    PASSES_RANDOMLY(1, 10, RNG_STENCH);
    
    GIVEN {
        ASSUME(gItems[ITEM_KINGS_ROCK].holdEffect == HOLD_EFFECT_FLINCH);

        PLAYER(SPECIES_GRIMER) { Ability(ABILITY_STENCH); Item(ITEM_KINGS_ROCK); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, WITH_RNG(RNG_HOLD_EFFECT_FLINCH, TRUE)); }
    } SCENE {
        MESSAGE("Foe Wobbuffet flinched!");
    }
}

SINGLE_BATTLE_TEST("Stench does not stack with moves with flinch chance")
{
    PASSES_RANDOMLY(3, 10, RNG_SECONDARY_EFFECT);
    
    GIVEN {
        ASSUME(MoveHasMoveEffectWithChance(MOVE_BITE, MOVE_EFFECT_FLINCH, 30));

        PLAYER(SPECIES_GRIMER) { Ability(ABILITY_STENCH); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_BITE, WITH_RNG(RNG_STENCH, TRUE)); }
    } SCENE {
        MESSAGE("Foe Wobbuffet flinched!");
    }
}

TO_DO_BATTLE_TEST("Stench has a 10% chance to flinch in each multi-strike hit");

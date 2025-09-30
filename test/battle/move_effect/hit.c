#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Accuracy controls the proportion of misses")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_DYNAMIC_PUNCH; }
    PARAMETRIZE { move = MOVE_THUNDER; }
    PARAMETRIZE { move = MOVE_HYDRO_PUMP; }
    PARAMETRIZE { move = MOVE_RAZOR_LEAF; }
    PARAMETRIZE { move = MOVE_SCRATCH; }
    
    PASSES_RANDOMLY(gBattleMoves[move].accuracy, 100, RNG_ACCURACY);
    
    GIVEN {
        ASSUME(gBattleMoves[move].accuracy >= 1 && gBattleMoves[move].accuracy <= 100);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, player);
    }
}

SINGLE_BATTLE_TEST("Critical hits deal 50% more damage", s16 damage)
{
    bool32 crit;
    
    PARAMETRIZE { crit = FALSE; }
    PARAMETRIZE { crit = TRUE; }
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, criticalHit: crit); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.5), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("AdditionalEffects.chance controls the proportion of secondary effects")
{
    u32 move, chance;
    
    PARAMETRIZE { move = MOVE_THUNDER_SHOCK; chance = 10; }
    PARAMETRIZE { move = MOVE_DISCHARGE; chance = 30; }
    
    PASSES_RANDOMLY(chance, 100, RNG_SECONDARY_EFFECT);
    
    GIVEN {
        ASSUME(MoveHasMoveEffect(move, MOVE_EFFECT_PARALYSIS, FALSE) == TRUE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        STATUS_ICON(opponent, paralysis: TRUE);
    }
}

SINGLE_BATTLE_TEST("Target contact abilities activates before the target faints")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(160); HP(20); }
        OPPONENT(SPECIES_SHARPEDO) { Ability(ABILITY_ROUGH_SKIN); HP(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
        HP_BAR(player);
        MESSAGE("Wobbuffet was hurt!");
        MESSAGE("Wobbuffet fainted!");
        MESSAGE("Foe Sharpedo fainted!");
    }
}

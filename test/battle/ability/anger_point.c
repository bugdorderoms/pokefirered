#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Anger Point maxes Attack if hit by a critical hit")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_STORM_THROW].critStage == 3);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_TAUROS) { Ability(ABILITY_ANGER_POINT); }
    } WHEN {
        TURN { MOVE(player, MOVE_STORM_THROW); }
    } SCENE {
        MESSAGE("{COLOR 2}{SHADOW 9}A critical hit!");
        ABILITY_POPUP(opponent, ABILITY_ANGER_POINT);
        MESSAGE("Foe Tauros maxed its Attack!");
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_ATK], MAX_STAT_STAGES);
    }
}

SINGLE_BATTLE_TEST("Anger Point only triggers if move is critical")
{
    bool32 crit;
    
    PARAMETRIZE { crit = TRUE; }
    PARAMETRIZE { crit = FALSE; }
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_TAUROS) { Ability(ABILITY_ANGER_POINT); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, criticalHit: crit); }
    } SCENE {
        if (crit) {
            ABILITY_POPUP(opponent, ABILITY_ANGER_POINT);
            MESSAGE("Foe Tauros maxed its Attack!");
        } else {
            NONE_OF {
                ABILITY_POPUP(opponent, ABILITY_ANGER_POINT);
                MESSAGE("Foe Tauros maxed its Attack!");
            }
        }
    }
}

SINGLE_BATTLE_TEST("Anger Point doesn't activate if hit the substitute")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SUBSTITUTE].effect == EFFECT_SUBSTITUTE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_TAUROS) { Ability(ABILITY_ANGER_POINT); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUBSTITUTE), MOVE(player, MOVE_SCRATCH, criticalHit: TRUE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, opponent);
        
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_ANGER_POINT);
            MESSAGE("Foe Tauros maxed its Attack!");
        }
    }
}

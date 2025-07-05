#include "global.h"
#include "math_util.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Drizzle summons rain when it enters battle", s16 damage)
{
    u32 ability;
    
    KNOWN_FAILING; // Need refactor damage calc to use fixed point calculation

    PARAMETRIZE { ability = ABILITY_DAMP; }
    PARAMETRIZE { ability = ABILITY_DRIZZLE; }

    GIVEN {
        ASSUME(gBattleMoves[MOVE_BUBBLE].type == TYPE_WATER);
        
        PLAYER(SPECIES_POLITOED) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_BUBBLE); }
    } SCENE {
        if (ability == ABILITY_DRIZZLE) {
            ABILITY_POPUP(player, ABILITY_DRIZZLE);
            MESSAGE("It started to rain!");
        }
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.5), results[1].damage);
    }
}

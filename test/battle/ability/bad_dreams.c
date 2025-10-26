#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_SING].effect == EFFECT_SLEEP);
}

SINGLE_BATTLE_TEST("Bad Dreams damages sleeping enemy Pokémon by 1/8 max HP")
{
    bool32 sleep;
    
    PARAMETRIZE { sleep = FALSE; }
    PARAMETRIZE { sleep = TRUE; }
    
    GIVEN {
        PLAYER(SPECIES_DARKRAI) { Ability(ABILITY_BAD_DREAMS); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(112); HP(112); }
    } WHEN {
        TURN {
            if (sleep)
                MOVE(player, MOVE_SING);
        }
    } SCENE {
        if (sleep) {
            ABILITY_POPUP(player, ABILITY_BAD_DREAMS);
            MESSAGE("Foe Wobbuffet is tormented!");
            HP_BAR(opponent, damage: 14);
        } else {
            NONE_OF {
                ABILITY_POPUP(player, ABILITY_BAD_DREAMS);
                MESSAGE("Foe Wobbuffet is tormented!");
                HP_BAR(opponent, damage: 14);
            }
        }
    }
}

TO_DO_BATTLE_TEST("Bad Dreams doesn't damage non-adjacent sleeping Pokémon in triples");

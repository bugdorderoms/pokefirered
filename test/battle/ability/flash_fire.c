#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_EMBER].type == TYPE_FIRE);
    ASSUME(gBattleMoves[MOVE_WILL_O_WISP].type == TYPE_FIRE);
    ASSUME(gBattleMoves[MOVE_WILL_O_WISP].split == SPLIT_STATUS);
}

SINGLE_BATTLE_TEST("Flash Fire boosts fire type moves by 50% but no subsequent increase is applied")
{
    u32 opponentMove;
    s16 damage[3];
    
    PARAMETRIZE { opponentMove = MOVE_EMBER; }
    PARAMETRIZE { opponentMove = MOVE_WILL_O_WISP; }
    
    GIVEN {
        PLAYER(SPECIES_HEATRAN) { Ability(ABILITY_FLASH_FIRE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_EMBER); MOVE(opponent, opponentMove); }
        TURN { MOVE(player, MOVE_EMBER); MOVE(opponent, opponentMove); }
        TURN { MOVE(player, MOVE_EMBER); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EMBER, player);
        HP_BAR(opponent, captureDamage: &damage[0]);
        ABILITY_POPUP(player, ABILITY_FLASH_FIRE);
        
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EMBER, player);
        HP_BAR(opponent, captureDamage: &damage[1]);
        ABILITY_POPUP(player, ABILITY_FLASH_FIRE);
        
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EMBER, player);
        HP_BAR(opponent, captureDamage: &damage[2]);
    } THEN {
        EXPECT_MUL_EQ(damage[0], UQ_4_12(1.5), damage[1]);
        EXPECT_EQ(damage[1], damage[2]);
    }
}

SINGLE_BATTLE_TEST("Flash Fire does not activate if protected")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_EMBER; }
    PARAMETRIZE { move = MOVE_WILL_O_WISP; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_PROTECT].effect == EFFECT_PROTECT);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_HEATRAN) { Ability(ABILITY_FLASH_FIRE); }
    } WHEN {
        TURN { MOVE(player, move); MOVE(opponent, MOVE_PROTECT); }
    } SCENE {
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_FLASH_FIRE);
            MESSAGE("The power of Foe Heatran's Fire-type moves rose!");
        }
    }
}

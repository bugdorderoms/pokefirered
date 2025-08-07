#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Oblivious prevents Infatuation")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_ATTRACT].effect == EFFECT_ATTRACT);

        PLAYER(SPECIES_SLOWPOKE, MON_MALE) { Ability(ABILITY_OBLIVIOUS); }
        OPPONENT(SPECIES_WOBBUFFET, MON_FEMALE);
    } WHEN {
        TURN { MOVE(opponent, MOVE_ATTRACT); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_OBLIVIOUS);
        NOT ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_INFATUATION, player);
        MESSAGE("It doesn't affect Slowpoke…");
    }
}

SINGLE_BATTLE_TEST("Oblivious prevents Captivate")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_CAPTIVATE].effect == EFFECT_CAPTIVATE);

        PLAYER(SPECIES_SLOWPOKE, MON_MALE) { Ability(ABILITY_OBLIVIOUS); }
        OPPONENT(SPECIES_WOBBUFFET, MON_FEMALE);
    } WHEN {
        TURN { MOVE(opponent, MOVE_CAPTIVATE); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_OBLIVIOUS);
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, player);
        MESSAGE("It doesn't affect Slowpoke…");
    }
}

SINGLE_BATTLE_TEST("Oblivious prevents Taunt")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_TAUNT].effect == EFFECT_TAUNT);

        PLAYER(SPECIES_SLOWPOKE) { Ability(ABILITY_OBLIVIOUS); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_TAUNT); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_OBLIVIOUS);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_TAUNT, opponent);
        MESSAGE("It doesn't affect Slowpoke…");
    }
}

SINGLE_BATTLE_TEST("Oblivious cures infatuation and Taunt if afflicted by it")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_ATTRACT; }
    PARAMETRIZE { move = MOVE_TAUNT; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_ATTRACT].effect == EFFECT_ATTRACT);
        ASSUME(gBattleMoves[MOVE_TAUNT].effect == EFFECT_TAUNT);
        
        PLAYER(SPECIES_PINSIR, MON_MALE) { Ability(ABILITY_MOLD_BREAKER); }
        OPPONENT(SPECIES_SLOWPOKE, MON_FEMALE) { Ability(ABILITY_OBLIVIOUS); }
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        ABILITY_POPUP(opponent, ABILITY_OBLIVIOUS);
        MESSAGE("Foe Slowpoke's Oblivious cured its mental state!");
    }
}

TO_DO_BATTLE_TEST("A held Mental Herb will activate and cure infatuation and Taunt before Oblivious activates");

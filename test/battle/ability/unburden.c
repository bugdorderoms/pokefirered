#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Unburden doubles speed when its item is consumed")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_NATURAL_GIFT].effect == EFFECT_NATURAL_GIFT);
        
        PLAYER(SPECIES_HITMONLEE) { Ability(ABILITY_UNBURDEN); Item(ITEM_LUM_BERRY); Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(199); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SPLASH); MOVE(player, MOVE_NATURAL_GIFT); }
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_NATURAL_GIFT, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);
    }
}

SINGLE_BATTLE_TEST("Unburden - If its lost and then regained, the boost isn't restored")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_NATURAL_GIFT].effect == EFFECT_NATURAL_GIFT);
        ASSUME(gBattleMoves[MOVE_SKILL_SWAP].effect == EFFECT_SKILL_SWAP);
        
        PLAYER(SPECIES_HITMONLEE) { Ability(ABILITY_UNBURDEN); Item(ITEM_LUM_BERRY); Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(199); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SPLASH); MOVE(player, MOVE_NATURAL_GIFT); }
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SKILL_SWAP); }
        TURN { MOVE(opponent, MOVE_SPLASH); MOVE(player, MOVE_SKILL_SWAP); }
        TURN { MOVE(opponent, MOVE_SPLASH); MOVE(player, MOVE_SPLASH); }
    } SCENE {
        // turn 1 slower - has item
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_NATURAL_GIFT, player);
        // turn 2 faster - no item
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SKILL_SWAP, opponent);
        // turn 3 slower - lost Unburden
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SKILL_SWAP, player);
        // turn 3 slower - boost isn't restored
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
    }
}

SINGLE_BATTLE_TEST("Unburden - If its suppressed and then regained, the boost is restored")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_NATURAL_GIFT].effect == EFFECT_NATURAL_GIFT);
        ASSUME(gBattleMoves[MOVE_U_TURN].effect == EFFECT_HIT_ESCAPE);
        
        PLAYER(SPECIES_HITMONLEE) { Ability(ABILITY_UNBURDEN); Item(ITEM_LUM_BERRY); Speed(100); }
        
        OPPONENT(SPECIES_WOBBUFFET) { Speed(199); }
        OPPONENT(SPECIES_WEEZING) { Ability(ABILITY_NEUTRALIZING_GAS); Speed(199); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SPLASH); MOVE(player, MOVE_NATURAL_GIFT); }
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_U_TURN); SEND_OUT(opponent, 1); }
        TURN { MOVE(opponent, MOVE_U_TURN); SEND_OUT(opponent, 0); MOVE(player, MOVE_SPLASH); }
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
    } SCENE {
        // turn 1 slower - has item
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_NATURAL_GIFT, player);
        // turn 2 faster - no item
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_U_TURN, opponent);
        ABILITY_POPUP(opponent, ABILITY_NEUTRALIZING_GAS);
        // turn 3 slower - Unburden suppressed
        ANIMATION(ANIM_TYPE_MOVE, MOVE_U_TURN, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
        // turn 4 faster - boost is restored
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);
    }
}

DOUBLE_BATTLE_TEST("Unburden boost is lost if it gains an item")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_NATURAL_GIFT].effect == EFFECT_NATURAL_GIFT);
        
        PLAYER(SPECIES_HITMONLEE) { Ability(ABILITY_UNBURDEN); Item(ITEM_LUM_BERRY); Speed(100); }
        PLAYER(SPECIES_ORANGURU) { Ability(ABILITY_SYMBIOSIS); Item(ITEM_ORAN_BERRY); Speed(99); }
        
        OPPONENT(SPECIES_WOBBUFFET) { Speed(199); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(99); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_SPLASH); MOVE(playerLeft, MOVE_NATURAL_GIFT, target: opponentLeft); }
        TURN { MOVE(opponentLeft, MOVE_SPLASH); MOVE(playerLeft, MOVE_SPLASH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_NATURAL_GIFT, playerLeft);
        
        ABILITY_POPUP(playerRight, ABILITY_SYMBIOSIS);
        
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, playerLeft);
    }
}

SINGLE_BATTLE_TEST("Unburden doubles speed when its item is knocked off")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_KNOCK_OFF].effect == EFFECT_KNOCK_OFF);
        
        PLAYER(SPECIES_HITMONLEE) { Ability(ABILITY_UNBURDEN); Item(ITEM_LUM_BERRY); Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(199); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_KNOCK_OFF); MOVE(player, MOVE_SPLASH); }
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_KNOCK_OFF, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);
    }
}

TO_DO_BATTLE_TEST("Unburden doubles speed if its item is consumed by Fling");
TO_DO_BATTLE_TEST("Unburden doubles speed if its item is eaten by Bug Bite");
TO_DO_BATTLE_TEST("Unburden doubles speed if its item is incinerated");
TO_DO_BATTLE_TEST("Unburden doubles speed if its Air Balloon pops");
TO_DO_BATTLE_TEST("Unburden doesn't doubles speed if its item is swapped to another");

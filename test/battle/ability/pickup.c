#include "global.h"
#include "test/battle.h"

WILD_BATTLE_TEST("Pickup has a 10% chance create an item when win a battle")
{
    PASSES_RANDOMLY(1, 10, RNG_PICKUP);
    
    GIVEN {
        ASSUME(PICKUP_ITEM_TO_BAG == FALSE);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_AIPOM) { Ability(ABILITY_PICKUP); }
        
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } THEN {
        EXPECT_NE(GetMonData(&gPlayerParty[1], MON_DATA_HELD_ITEM), ITEM_NONE);
    }
}

DOUBLE_BATTLE_TEST("Pickup collects the last used item of a Pokémon on field at the end turn")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_NATURAL_GIFT].effect == EFFECT_NATURAL_GIFT);
        
        PLAYER(SPECIES_AIPOM) { Ability(ABILITY_PICKUP); }
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_LUM_BERRY); }
        OPPONENT(SPECIES_BULBASAUR) { Item(ITEM_ORAN_BERRY); }
    } WHEN {
        TURN {
            MOVE(opponentLeft, MOVE_NATURAL_GIFT, target: playerLeft);
            MOVE(opponentRight, MOVE_NATURAL_GIFT, target: playerRight);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_NATURAL_GIFT, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_NATURAL_GIFT, opponentRight);
        
        ABILITY_POPUP(playerLeft, ABILITY_PICKUP);
        MESSAGE("Aipom's found one Oran Berry!");
    } THEN {
        EXPECT_EQ(playerLeft->item, ITEM_ORAN_BERRY);
    }
}

DOUBLE_BATTLE_TEST("Pickup doesn't triggers if the item has already been recovered")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_NATURAL_GIFT].effect == EFFECT_NATURAL_GIFT);
        
        PLAYER(SPECIES_AIPOM) { Ability(ABILITY_PICKUP); }
        PLAYER(SPECIES_LINOONE) { Ability(ABILITY_PICKUP); }
        
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_LUM_BERRY); }
        OPPONENT(SPECIES_BULBASAUR);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_NATURAL_GIFT, target: playerLeft); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_NATURAL_GIFT, opponentLeft);
        
        ABILITY_POPUP(playerLeft, ABILITY_PICKUP);
        MESSAGE("Aipom's found one Lum Berry!");
        
        NONE_OF {
            ABILITY_POPUP(playerRight, ABILITY_PICKUP);
            MESSAGE("Linoone's found one Lum Berry!");
        }
    } THEN {
        EXPECT_EQ(playerLeft->item, ITEM_LUM_BERRY);
        EXPECT_EQ(playerRight->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("Pickup doesn't trigger if the Pokémon that consumed the item is no longer on field")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_NATURAL_GIFT].effect == EFFECT_NATURAL_GIFT);
        ASSUME(gBattleMoves[MOVE_ROAR].effect == EFFECT_RANDOM_SWITCH);
        
        PLAYER(SPECIES_AIPOM) { Ability(ABILITY_PICKUP); }
        
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_LUM_BERRY); }
        OPPONENT(SPECIES_BULBASAUR);
    } WHEN {
        TURN { MOVE(opponent, MOVE_NATURAL_GIFT); MOVE(player, MOVE_ROAR); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_NATURAL_GIFT, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROAR, player);
        
        NONE_OF {
            ABILITY_POPUP(player, ABILITY_PICKUP);
            MESSAGE("Aipom's found one Lum Berry!");
        }
    } THEN {
        EXPECT_EQ(player->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("Pickup can't collect an item knocked off by Knock Off")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_KNOCK_OFF].effect == EFFECT_KNOCK_OFF);
        
        PLAYER(SPECIES_AIPOM) { Ability(ABILITY_PICKUP); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_LUM_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_KNOCK_OFF); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_KNOCK_OFF, player);
        
        NONE_OF {
            ABILITY_POPUP(player, ABILITY_PICKUP);
            MESSAGE("Aipom's found one Lum Berry!");
        }
    } THEN {
        EXPECT_EQ(player->item, ITEM_NONE);
    }
}

TO_DO_BATTLE_TEST("Pickup will collect an item throw using Fling");
TO_DO_BATTLE_TEST("Pickup can't collect an item destroyed by Incinerate");
TO_DO_BATTLE_TEST("Pickup can't collect an item eaten by Bug Bite");
TO_DO_BATTLE_TEST("Pickup can't collect an popped Air Ballon");

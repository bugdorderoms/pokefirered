#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Klutz prevents its held item from activating", s16 damage)
{
    u32 item;
    
    PARAMETRIZE { item = ITEM_NONE; }
    PARAMETRIZE { item = ITEM_METAL_COAT; }
    
    GIVEN {
        ASSUME(gItems[ITEM_METAL_COAT].holdEffect == HOLD_EFFECT_TYPE_POWER);
        ASSUME(gItems[ITEM_METAL_COAT].holdEffectParam == TYPE_STEEL);
        ASSUME(gBattleMoves[MOVE_STEEL_WING].type == TYPE_STEEL);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_AUDINO) { Ability(ABILITY_KLUTZ); Item(item); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_STEEL_WING); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Klutz prevents from using Natural Gift")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_NATURAL_GIFT].effect == EFFECT_NATURAL_GIFT);
        
        PLAYER(SPECIES_AUDINO) { Ability(ABILITY_KLUTZ); Item(ITEM_PECHA_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_NATURAL_GIFT); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_NATURAL_GIFT, player);
        MESSAGE("But it failed!");
    }
}

SINGLE_BATTLE_TEST("Klutz doesn't prevents Macho Brace speed drop")
{
    GIVEN {
        ASSUME(gItems[ITEM_MACHO_BRACE].holdEffect == HOLD_EFFECT_MACHO_BRACE);
        
        PLAYER(SPECIES_AUDINO) { Ability(ABILITY_KLUTZ); Item(ITEM_MACHO_BRACE); Speed(30); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(25); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SPLASH); MOVE(player, MOVE_SPLASH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
    }
}

WILD_BATTLE_TEST("Klutz doesn't negate effects that affects experience", s32 exp)
{
    u32 item;
    
    PARAMETRIZE { item = ITEM_NONE; }
    PARAMETRIZE { item = ITEM_LUCKY_EGG; }
    
    GIVEN {
        ASSUME(gItems[ITEM_LUCKY_EGG].holdEffect == HOLD_EFFECT_LUCKY_EGG);
        
        PLAYER(SPECIES_AUDINO) { Ability(ABILITY_KLUTZ); Item(item); Level(5); }
        OPPONENT(SPECIES_CATERPIE) { HP(1); Level(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        EXPERIENCE_BAR(player, captureGainedExp: &results[i].exp);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].exp, UQ_4_12(1.5), results[1].exp);
    }
}

TO_DO_BATTLE_TEST("Klutz doesn't negate effects that affects prize money");
TO_DO_BATTLE_TEST("Klutz doesn't negate effects that affects effort values");
TO_DO_BATTLE_TEST("Klutz doesn't negate effects that affects friendship");
TO_DO_BATTLE_TEST("Klutz prevents from using Fling");
TO_DO_BATTLE_TEST("Klutz doesn't prevents an eaten berry from activating when using Pluck");

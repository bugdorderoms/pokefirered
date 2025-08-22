#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Shield Dust blocks secondary effects")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_FAKE_OUT; }
    PARAMETRIZE { move = MOVE_ROCK_TOMB; }
    
    GIVEN {
        ASSUME(MoveHasMoveEffectWithChance(MOVE_FAKE_OUT, MOVE_EFFECT_FLINCH, 100) == TRUE);
        ASSUME(MoveHasMoveEffectWithChance(MOVE_ROCK_TOMB, MOVE_EFFECT_SPD_MINUS_1, 100) == TRUE);

        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_VIVILLON) { Ability(ABILITY_SHIELD_DUST); }
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        
        NONE_OF {
            MESSAGE("Foe Vivillon flinched!");
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        }
    }
}

SINGLE_BATTLE_TEST("Shield Dust does not block self-targeting effects")
{
    GIVEN {
        ASSUME(MoveHasMoveEffect(MOVE_RAPID_SPIN, MOVE_EFFECT_SPD_PLUS_1, TRUE) == TRUE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_VIVILLON) { Ability(ABILITY_SHIELD_DUST); }
    } WHEN {
        TURN { MOVE(player, MOVE_RAPID_SPIN); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_RAPID_SPIN, player);
        MESSAGE("Wobbuffet's Speed rose!");
    } THEN {
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGES + 1);
    }
}

SINGLE_BATTLE_TEST("Shield Dust does not block primary effects")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_THUNDER_WAVE].effect == EFFECT_SET_PARALYZE);

        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_VIVILLON) { Ability(ABILITY_SHIELD_DUST); }
    } WHEN {
        TURN { MOVE(player, MOVE_THUNDER_WAVE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_THUNDER_WAVE, player);
        MESSAGE("Foe Vivillon is paralyzed! It may be unable to move!");
        STATUS_ICON(opponent, paralysis: TRUE);
    }
}

SINGLE_BATTLE_TEST("Shield Dust blocks Stench")
{
    GIVEN {
        PLAYER(SPECIES_GRIMER) { Ability(ABILITY_STENCH); }
        OPPONENT(SPECIES_VIVILLON) { Ability(ABILITY_SHIELD_DUST); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, WITH_RNG(RNG_STENCH, TRUE)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        NOT MESSAGE("Foe Vivillon flinched!");
    }
}

SINGLE_BATTLE_TEST("Shield Dust blocks Poison Touch")
{
    GIVEN {
        PLAYER(SPECIES_GRIMER) { Ability(ABILITY_POISON_TOUCH); }
        OPPONENT(SPECIES_VIVILLON) { Ability(ABILITY_SHIELD_DUST); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, WITH_RNG(RNG_POISON_TOUCH, TRUE)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        
        NONE_OF {
            ABILITY_POPUP(player, ABILITY_POISON_TOUCH);
            MESSAGE("Foe Vivillon was poisoned!");
            STATUS_ICON(opponent, poison: TRUE);
        }
    }
}

TO_DO_BATTLE_TEST("Shield Dust blocks Toxic Chain");
TO_DO_BATTLE_TEST("Shield Dust blocks King's Rock");
TO_DO_BATTLE_TEST("Shield Dust does or does not block Sparkling Aria depending on number of targets hit");
TO_DO_BATTLE_TEST("Shield Dust blocks Sparkling Aria in singles");
TO_DO_BATTLE_TEST("Shield Dust blocks Sparkling Aria if all other targets avoid getting hit by");

#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_HEAL_PULSE].effect == EFFECT_HEAL_PULSE);
}

SINGLE_BATTLE_TEST("Heal Pulse heals the target HP by 50% of its max HP")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(100); HP(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_HEAL_PULSE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HEAL_PULSE, player);
        HP_BAR(opponent, damage: -50);
        MESSAGE("Foe Wobbuffet regained health!");
    }
}

SINGLE_BATTLE_TEST("Heal Pulse fails if the target's HP is full")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(100); HP(100); }
    } WHEN {
        TURN { MOVE(player, MOVE_HEAL_PULSE); }
    } SCENE {
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_HEAL_PULSE, player);
            HP_BAR(opponent, damage: -50);
            MESSAGE("Foe Wobbuffet regained health!");
        }
    }
}

SINGLE_BATTLE_TEST("Heal Pulse fails if target is behind Substitute")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SUBSTITUTE].effect == EFFECT_SUBSTITUTE);
        
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(100); HP(50); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SUBSTITUTE); MOVE(opponent, MOVE_HEAL_PULSE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, player);
        
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_HEAL_PULSE, opponent);
            HP_BAR(player, damage: -50);
        }
    }
}

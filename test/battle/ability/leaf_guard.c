#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Leaf Guard prevents burn in sun")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_WILL_O_WISP].effect == EFFECT_WILL_O_WISP);
        
        PLAYER(SPECIES_VULPIX) { Ability(ABILITY_DROUGHT); }
        OPPONENT(SPECIES_TANGELA) { Ability(ABILITY_LEAF_GUARD); }
    } WHEN {
        TURN { MOVE(player, MOVE_WILL_O_WISP); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_LEAF_GUARD);
        MESSAGE("Foe Tangela's Leaf Guard prevents burns!");
        
        NONE_OF {
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_BRN, opponent);
            STATUS_ICON(opponent, burn: TRUE);
        }
    }
}

SINGLE_BATTLE_TEST("Leaf Guard prevents sleep in sun")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_HYPNOSIS].effect == EFFECT_SLEEP);
        
        PLAYER(SPECIES_VULPIX) { Ability(ABILITY_DROUGHT); }
        OPPONENT(SPECIES_TANGELA) { Ability(ABILITY_LEAF_GUARD); }
    } WHEN {
        TURN { MOVE(player, MOVE_HYPNOSIS); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_LEAF_GUARD);
        MESSAGE("It doesn't affect Foe Tangela…");
        
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPNOSIS, player);
            STATUS_ICON(opponent, sleep: TRUE);
        }
    }
}

SINGLE_BATTLE_TEST("Leaf Guard prevents yawn in sun")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_YAWN].effect == EFFECT_YAWN);
        
        PLAYER(SPECIES_VULPIX) { Ability(ABILITY_DROUGHT); }
        OPPONENT(SPECIES_TANGELA) { Ability(ABILITY_LEAF_GUARD); }
    } WHEN {
        TURN { MOVE(player, MOVE_YAWN); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_LEAF_GUARD);
        MESSAGE("It doesn't affect Foe Tangela…");
        
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_YAWN, player);
            STATUS_ICON(opponent, sleep: TRUE);
        }
    }
}

SINGLE_BATTLE_TEST("Leaf Guard prevents a drowsy pokemon fall asleep in sun")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_YAWN].effect == EFFECT_YAWN);
        
        PLAYER(SPECIES_VULPIX) { Ability(ABILITY_DROUGHT); }
        PLAYER(SPECIES_PINSIR) { Ability(ABILITY_MOLD_BREAKER); }
        
        OPPONENT(SPECIES_TANGELA) { Ability(ABILITY_LEAF_GUARD); }
    } WHEN {
        TURN { SWITCH(player, 1); }
        TURN { MOVE(player, MOVE_YAWN); }
        TURN { }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_YAWN, player);
        NOT STATUS_ICON(opponent, sleep: TRUE);
    }
}

SINGLE_BATTLE_TEST("Leaf Guard prevents rest in sun")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_REST].effect == EFFECT_REST);
        
        PLAYER(SPECIES_VULPIX) { Ability(ABILITY_DROUGHT); }
        OPPONENT(SPECIES_TANGELA) { Ability(ABILITY_LEAF_GUARD); HP(1); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_REST); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_LEAF_GUARD);
        MESSAGE("It doesn't affect Foe Tangela…");
        
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_REST, opponent);
            STATUS_ICON(opponent, sleep: TRUE);
            HP_BAR(opponent);
        }
    }
}

SINGLE_BATTLE_TEST("Leaf Guard prevents paralysis in sun")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_THUNDER_WAVE].effect == EFFECT_SET_PARALYZE);
        
        PLAYER(SPECIES_VULPIX) { Ability(ABILITY_DROUGHT); }
        OPPONENT(SPECIES_TANGELA) { Ability(ABILITY_LEAF_GUARD); }
    } WHEN {
        TURN { MOVE(player, MOVE_THUNDER_WAVE); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_LEAF_GUARD);
        MESSAGE("Foe Tangela's Leaf Guard prevents paralysis!");
        
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_THUNDER_WAVE, player);
            STATUS_ICON(opponent, paralysis: TRUE);
        }
    }
}

SINGLE_BATTLE_TEST("Leaf Guard prevents poison in sun")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_TOXIC].effect == EFFECT_SET_TOXIC_POISON);
        
        PLAYER(SPECIES_VULPIX) { Ability(ABILITY_DROUGHT); }
        OPPONENT(SPECIES_TANGELA) { Ability(ABILITY_LEAF_GUARD); }
    } WHEN {
        TURN { MOVE(player, MOVE_TOXIC); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_LEAF_GUARD);
        MESSAGE("Foe Tangela's Leaf Guard prevents poisoning!");
        
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_TOXIC, player);
            STATUS_ICON(opponent, poison: TRUE);
        }
    }
}

TO_DO_BATTLE_TEST("Leaf Guard prevents poison from Toxic Orb in sun");
TO_DO_BATTLE_TEST("Leaf Guard prevents burn from Flame Orb in sun");

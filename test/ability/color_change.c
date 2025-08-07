#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Color Change changes the type of the user to the type of the move that it got hit")
{
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_KECLEON].types[0] != TYPE_PSYCHIC && gSpeciesInfo[SPECIES_KECLEON].types[1] != TYPE_PSYCHIC);
        ASSUME(gBattleMoves[MOVE_PSYWAVE].type == TYPE_PSYCHIC);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_KECLEON) { Ability(ABILITY_COLOR_CHANGE); }
    } WHEN {
        TURN { MOVE(player, MOVE_PSYWAVE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PSYWAVE, player);
        ABILITY_POPUP(opponent, ABILITY_COLOR_CHANGE);
        MESSAGE("Foe Kecleon's type changed to Psychic!");
    }
}

SINGLE_BATTLE_TEST("Color Change does not activate if its the same type of the move")
{
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_KECLEON].types[0] == TYPE_NORMAL || gSpeciesInfo[SPECIES_KECLEON].types[1] == TYPE_NORMAL);
        ASSUME(gBattleMoves[MOVE_SCRATCH].type == TYPE_NORMAL);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_KECLEON) { Ability(ABILITY_COLOR_CHANGE); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_COLOR_CHANGE);
            MESSAGE("Foe Kecleon's type changed to Normal!");
        }
    }
}

SINGLE_BATTLE_TEST("Color Change does not activate if hit by Struggle")
{
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_KECLEON].types[0] != TYPE_ELECTRIC && gSpeciesInfo[SPECIES_KECLEON].types[1] != TYPE_ELECTRIC);
        ASSUME(gBattleMoves[MOVE_PSYWAVE].type == TYPE_PSYCHIC);
        ASSUME(gBattleMoves[MOVE_STRUGGLE].type == TYPE_NORMAL);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_KECLEON) { Ability(ABILITY_COLOR_CHANGE); }
    } WHEN {
        TURN { MOVE(player, MOVE_PSYWAVE); }
        TURN { MOVE(player, MOVE_STRUGGLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PSYWAVE, player);
        ABILITY_POPUP(opponent, ABILITY_COLOR_CHANGE);
        MESSAGE("Foe Kecleon's type changed to Psychic!");
        
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STRUGGLE, player);
        
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_COLOR_CHANGE);
            MESSAGE("Foe Kecleon's type changed to Normal!");
        }
    }
}

SINGLE_BATTLE_TEST("Color Change does not activate if shared HP by Pain Split")
{
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_KECLEON].types[0] != TYPE_ELECTRIC && gSpeciesInfo[SPECIES_KECLEON].types[1] != TYPE_ELECTRIC);
        ASSUME(gBattleMoves[MOVE_PAIN_SPLIT].effect == EFFECT_PAIN_SPLIT);
        ASSUME(gBattleMoves[MOVE_PAIN_SPLIT].type == TYPE_NORMAL);
        
        PLAYER(SPECIES_GEODUDE_ALOLAN) { Ability(ABILITY_GALVANIZE); }
        OPPONENT(SPECIES_KECLEON) { Ability(ABILITY_COLOR_CHANGE); }
    } WHEN {
        TURN { MOVE(player, MOVE_PAIN_SPLIT); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PAIN_SPLIT, player);
        MESSAGE("The battlers shared their pain!");
        
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_COLOR_CHANGE);
            MESSAGE("Foe Kecleon's type changed to Electric!");
        }
    }
}

SINGLE_BATTLE_TEST("Color Change does not activate if it hit the Substitute")
{
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_KECLEON].types[0] != TYPE_PSYCHIC && gSpeciesInfo[SPECIES_KECLEON].types[1] != TYPE_PSYCHIC);
        ASSUME(gBattleMoves[MOVE_PSYWAVE].type == TYPE_PSYCHIC);
        ASSUME(gBattleMoves[MOVE_SUBSTITUTE].effect == EFFECT_SUBSTITUTE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_KECLEON) { Ability(ABILITY_COLOR_CHANGE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUBSTITUTE); MOVE(player, MOVE_PSYWAVE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PSYWAVE, player);
        
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_COLOR_CHANGE);
            MESSAGE("Foe Kecleon's type changed to Psychic!");
        }
    }
}

SINGLE_BATTLE_TEST("Color Change changes the type when a Pokemon is hit by Future Sight")
{
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_KECLEON].types[0] != TYPE_PSYCHIC && gSpeciesInfo[SPECIES_KECLEON].types[1] != TYPE_PSYCHIC);
        ASSUME(gBattleMoves[MOVE_FUTURE_SIGHT].effect == EFFECT_FUTURE_SIGHT);
        ASSUME(gBattleMoves[MOVE_FUTURE_SIGHT].type == TYPE_PSYCHIC);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_KECLEON) { Ability(ABILITY_COLOR_CHANGE); }
    } WHEN {
        TURN { MOVE(player, MOVE_FUTURE_SIGHT); }
        TURN { }
        TURN { }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FUTURE_SIGHT, player);
        MESSAGE("Foe Kecleon took the Future Sight attack!");
        ABILITY_POPUP(opponent, ABILITY_COLOR_CHANGE);
        MESSAGE("Foe Kecleon's type changed to Psychic!");
    }
}

SINGLE_BATTLE_TEST("Color Change activates only on the final hit of a multi-strike move")
{
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_KECLEON].types[0] != TYPE_FIGHTING && gSpeciesInfo[SPECIES_KECLEON].types[1] != TYPE_FIGHTING);
        ASSUME(gBattleMoves[MOVE_DOUBLE_KICK].strikeCount == 2);
        ASSUME(gBattleMoves[MOVE_DOUBLE_KICK].type == TYPE_FIGHTING);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_KECLEON) { Ability(ABILITY_COLOR_CHANGE); }
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_KICK); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_KICK, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_KICK, player);
        ABILITY_POPUP(opponent, ABILITY_COLOR_CHANGE);
        MESSAGE("Foe Kecleon's type changed to Fighting!");
        MESSAGE("Hit 2 time(s)!");
    }
}

TO_DO_BATTLE_TEST("Color Change is not activated when hit by a typeless Revelation Dance");

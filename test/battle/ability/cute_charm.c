#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Cute Charm has a 30% chance to inflict infatuation")
{
    PASSES_RANDOMLY(3, 10, RNG_CUTE_CHARM);
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].flags.makesContact == TRUE);
        
        PLAYER(SPECIES_WOBBUFFET, MON_MALE);
        OPPONENT(SPECIES_MILOTIC, MON_FEMALE) { Ability(ABILITY_CUTE_CHARM); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_INFATUATION, player);
        ABILITY_POPUP(opponent, ABILITY_CUTE_CHARM);
        MESSAGE("Wobbuffet fell in love!");
    }
}

SINGLE_BATTLE_TEST("Cute Charm doesn't triggers on genderless or same gender Pokémon")
{
    u32 species, gender;
    
    PARAMETRIZE { species = SPECIES_ARCEUS; gender = MON_GENDERLESS; }
    PARAMETRIZE { species = SPECIES_WOBBUFFET; gender = MON_FEMALE; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].flags.makesContact == TRUE);
        ASSUME(gSpeciesInfo[SPECIES_ARCEUS].genderRatio == MON_GENDERLESS);
        
        PLAYER(species, gender);
        OPPONENT(SPECIES_MILOTIC, MON_FEMALE) { Ability(ABILITY_CUTE_CHARM); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, WITH_RNG(RNG_CUTE_CHARM, TRUE)); }
    } SCENE {
        NONE_OF {
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_INFATUATION, player);
            ABILITY_POPUP(opponent, ABILITY_CUTE_CHARM);
        }
    }
}

SINGLE_BATTLE_TEST("Cute Charm only triggers if move makes contact")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_SCRATCH; };
    PARAMETRIZE { move = MOVE_GUST; };
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].flags.makesContact == TRUE);
        ASSUME(gBattleMoves[MOVE_GUST].flags.makesContact == FALSE);
        
        PLAYER(SPECIES_WOBBUFFET, MON_MALE);
        OPPONENT(SPECIES_MILOTIC, MON_FEMALE) { Ability(ABILITY_CUTE_CHARM); }
    } WHEN {
        TURN { MOVE(player, move, WITH_RNG(RNG_CUTE_CHARM, TRUE)); }
    } SCENE {
        if (gBattleMoves[move].flags.makesContact) {
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_INFATUATION, player);
            ABILITY_POPUP(opponent, ABILITY_CUTE_CHARM);
            MESSAGE("Wobbuffet fell in love!");
        } else {
            NONE_OF {
                ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_INFATUATION, player);
                ABILITY_POPUP(opponent, ABILITY_CUTE_CHARM);
                MESSAGE("Wobbuffet fell in love!");
            }
        }
    }
}

TO_DO_BATTLE_TEST("Cute Charm has a 30% chance to inflic infatuation in each multi-strike hit");
TO_DO_BATTLE_TEST("Cute Charm doesn't affects a tera raid boss");

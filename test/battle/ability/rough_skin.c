#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_SCRATCH].flags.makesContact == TRUE);
}

SINGLE_BATTLE_TEST("Rough Skin damages the attacker by 1/8 its max HP")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(160); HP(160); }
        OPPONENT(SPECIES_SHARPEDO) { Ability(ABILITY_ROUGH_SKIN); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
        HP_BAR(player, damage: 20);
        MESSAGE("Wobbuffet was hurt!");
    }
}

SINGLE_BATTLE_TEST("Rough Skin only triggers if move makes contact")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_SCRATCH; };
    PARAMETRIZE { move = MOVE_GUST; };
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_GUST].flags.makesContact == FALSE);
        
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(160); HP(160); }
        OPPONENT(SPECIES_SHARPEDO) { Ability(ABILITY_ROUGH_SKIN); }
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        if (gBattleMoves[move].flags.makesContact) {
            ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
            HP_BAR(player);
            MESSAGE("Wobbuffet was hurt!");
        } else {
            NONE_OF {
                ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
                HP_BAR(player);
                MESSAGE("Wobbuffet was hurt!");
            }
        }
    }
}

SINGLE_BATTLE_TEST("Rough Skin - If both Pokémon faints, the attacker will faint first")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(160); HP(20); }
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_SHARPEDO) { Ability(ABILITY_ROUGH_SKIN); HP(1); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); SEND_OUT(player, 1); SEND_OUT(opponent, 1); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
        HP_BAR(player);
        MESSAGE("Wobbuffet was hurt!");
        MESSAGE("Wobbuffet fainted!");
        MESSAGE("Foe Sharpedo fainted!");
    }
}

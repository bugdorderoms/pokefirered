#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Motor Drive boosts Speed when hit by electric type moves")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_THUNDER_SHOCK; }
    PARAMETRIZE { move = MOVE_THUNDER_WAVE; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_THUNDER_SHOCK].type == TYPE_ELECTRIC);
        ASSUME(gBattleMoves[MOVE_THUNDER_WAVE].type == TYPE_ELECTRIC);
        ASSUME(gBattleMoves[MOVE_THUNDER_WAVE].split == SPLIT_STATUS);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_EMOLGA) { Ability(ABILITY_MOTOR_DRIVE); }
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_MOTOR_DRIVE);
        MESSAGE("Foe Emolga's Speed rose!");
    } THEN {
        EXPECT_EQ(opponent->statStages[STAT_SPEED], DEFAULT_STAT_STAGES + 1);
    }
}

SINGLE_BATTLE_TEST("Motor Drive does not activate if protected")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_THUNDER_SHOCK; }
    PARAMETRIZE { move = MOVE_THUNDER_WAVE; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_THUNDER_SHOCK].type == TYPE_ELECTRIC);
        ASSUME(gBattleMoves[MOVE_THUNDER_WAVE].type == TYPE_ELECTRIC);
        ASSUME(gBattleMoves[MOVE_THUNDER_WAVE].split == SPLIT_STATUS);
        ASSUME(gBattleMoves[MOVE_PROTECT].effect == EFFECT_PROTECT);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_EMOLGA) { Ability(ABILITY_MOTOR_DRIVE); }
    } WHEN {
        TURN { MOVE(player, move); MOVE(opponent, MOVE_PROTECT); }
    } SCENE {
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_MOTOR_DRIVE);
            MESSAGE("Foe Emolga's Speed rose!");
        }
    }
}

SINGLE_BATTLE_TEST("Motor Drive makes move useless if its speed is maxed")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_THUNDER_SHOCK; }
    PARAMETRIZE { move = MOVE_THUNDER_WAVE; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_THUNDER_SHOCK].type == TYPE_ELECTRIC);
        ASSUME(gBattleMoves[MOVE_THUNDER_WAVE].type == TYPE_ELECTRIC);
        ASSUME(gBattleMoves[MOVE_THUNDER_WAVE].split == SPLIT_STATUS);
        ASSUME(gBattleMoves[MOVE_AGILITY].effect == EFFECT_SPEED_UP_2);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_EMOLGA) { Ability(ABILITY_MOTOR_DRIVE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { MOVE(opponent, MOVE_AGILITY); }
        TURN { MOVE(player, move); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_MOTOR_DRIVE);
        NOT MESSAGE("Foe Emolga's Speed rose!");
        MESSAGE("It doesn't affect Foe Emolga…");
    }
}

SINGLE_BATTLE_TEST("Motor Drive is only triggered once on multi strike moves")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_FURY_SWIPES].type == TYPE_NORMAL);
        ASSUME(gBattleMoves[MOVE_FURY_SWIPES].effect == EFFECT_MULTI_HIT);
        
        PLAYER(SPECIES_EMOLGA) { Ability(ABILITY_MOTOR_DRIVE); }
        OPPONENT(SPECIES_GRAVELER_ALOLAN) { Ability(ABILITY_GALVANIZE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_FURY_SWIPES); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_MOTOR_DRIVE);
        MESSAGE("Emolga's Speed rose!");
    } THEN {
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGES + 1);
    }
}

TO_DO_BATTLE_TEST("Motor Drive activates before a held Cell Battery");

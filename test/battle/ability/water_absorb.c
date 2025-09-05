#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Water Absorb/Dry Skin heals 25% when hit by water type moves")
{
    u32 j, move;
    u32 species, ability;
    static const u32 waterAbsorbData[][2] =
    {
        {SPECIES_LAPRAS,   ABILITY_WATER_ABSORB},
        {SPECIES_PARAS,    ABILITY_DRY_SKIN},
    };
    
    for (j = 0; j < ARRAY_COUNT(waterAbsorbData); j++)
    {
        PARAMETRIZE { species = waterAbsorbData[j][0]; ability = waterAbsorbData[j][1]; move = MOVE_WATER_GUN; }
        PARAMETRIZE { species = waterAbsorbData[j][0]; ability = waterAbsorbData[j][1]; move = MOVE_SOAK; }
    }

    GIVEN {
        ASSUME(gBattleMoves[MOVE_WATER_GUN].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_SOAK].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_SOAK].split == SPLIT_STATUS);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(species) { Ability(ability); MaxHP(100); HP(1); }
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        ABILITY_POPUP(opponent, ability);
        HP_BAR(opponent, damage: -25);
        
        switch (species)
        {
            case SPECIES_LAPRAS:
                MESSAGE("Foe Lapras had its HP restored.");
                break;
            case SPECIES_PARAS:
                MESSAGE("Foe Paras had its HP restored.");
                break;
        }
    }
}

SINGLE_BATTLE_TEST("Water Absorb/Dry Skin does not activate if protected")
{
    u32 j, move;
    u32 species, ability;
    static const u32 waterAbsorbData[][2] =
    {
        {SPECIES_LAPRAS,   ABILITY_WATER_ABSORB},
        {SPECIES_PARAS,    ABILITY_DRY_SKIN},
    };
    
    for (j = 0; j < ARRAY_COUNT(waterAbsorbData); j++)
    {
        PARAMETRIZE { species = waterAbsorbData[j][0]; ability = waterAbsorbData[j][1]; move = MOVE_WATER_GUN; }
        PARAMETRIZE { species = waterAbsorbData[j][0]; ability = waterAbsorbData[j][1]; move = MOVE_SOAK; }
    }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_WATER_GUN].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_SOAK].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_SOAK].split == SPLIT_STATUS);
        ASSUME(gBattleMoves[MOVE_PROTECT].effect == EFFECT_PROTECT);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(species) { Ability(ability); MaxHP(100); HP(1); }
    } WHEN {
        TURN { MOVE(player, move); MOVE(opponent, MOVE_PROTECT); }
    } SCENE {
        NONE_OF {
            ABILITY_POPUP(opponent, ability);
            HP_BAR(opponent);
            
            switch (species)
            {
                case SPECIES_LAPRAS:
                    MESSAGE("Foe Lapras had its HP restored.");
                    break;
                case SPECIES_PARAS:
                    MESSAGE("Foe Paras had its HP restored.");
                    break;
            }
        }
    }
}

SINGLE_BATTLE_TEST("Water Absorb/Dry Skin makes move useless when its HP is full")
{
    u32 j, move;
    u32 species, ability;
    static const u32 waterAbsorbData[][2] =
    {
        {SPECIES_LAPRAS,   ABILITY_WATER_ABSORB},
        {SPECIES_PARAS,    ABILITY_DRY_SKIN},
    };
    
    for (j = 0; j < ARRAY_COUNT(waterAbsorbData); j++)
    {
        PARAMETRIZE { species = waterAbsorbData[j][0]; ability = waterAbsorbData[j][1]; move = MOVE_WATER_GUN; }
        PARAMETRIZE { species = waterAbsorbData[j][0]; ability = waterAbsorbData[j][1]; move = MOVE_SOAK; }
    }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_WATER_GUN].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_SOAK].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_SOAK].split == SPLIT_STATUS);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(species) { Ability(ability); MaxHP(100); HP(100); }
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        ABILITY_POPUP(opponent, ability);
        NOT HP_BAR(opponent, damage: -25);
        
        switch (species)
        {
            case SPECIES_LAPRAS:
                MESSAGE("It doesn't affect Foe Lapras…");
                break;
            case SPECIES_PARAS:
                MESSAGE("It doesn't affect Foe Paras…");
                break;
        }
    }
}

SINGLE_BATTLE_TEST("Water Absorb/Dry Skin is only triggered once on multi strike moves")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_LAPRAS; ability = ABILITY_WATER_ABSORB; }
    PARAMETRIZE { species = SPECIES_PARAS; ability = ABILITY_DRY_SKIN; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_WATER_SHURIKEN].effect == EFFECT_MULTI_HIT);
        ASSUME(gBattleMoves[MOVE_WATER_SHURIKEN].type == TYPE_WATER);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(species) { Ability(ability); MaxHP(100); HP(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_WATER_SHURIKEN); }
    } SCENE {
        ABILITY_POPUP(opponent, ability);
        HP_BAR(opponent, damage: -25);
        
        switch (species)
        {
            case SPECIES_LAPRAS:
                MESSAGE("Foe Lapras had its HP restored.");
                break;
            case SPECIES_PARAS:
                MESSAGE("Foe Paras had its HP restored.");
                break;
        }
    }
}

TO_DO_BATTLE_TEST("Water Absorb/Dry Skin activates before a held Absorb Bulb");

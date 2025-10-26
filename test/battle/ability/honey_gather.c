#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(PICKUP_ITEM_TO_BAG == FALSE);
}

WILD_BATTLE_TEST("Honey Gather may create a Honey when win a battle")
{
    PASSES_RANDOMLY(50, 100, RNG_HONEY_GATHER);
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_TEDDIURSA) { Ability(ABILITY_HONEY_GATHER); }
        
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } THEN {
        EXPECT_EQ(GetMonData(&gPlayerParty[1], MON_DATA_HELD_ITEM), ITEM_HONEY);
    }
}

WILD_BATTLE_TEST("Honey Gather chance increases by 5% every 10 levels")
{
    u32 j;
    u32 level, chance;
    
    for (j = 1; j <= 10; j++) {
        PARAMETRIZE { level = 10 * j; chance = 5 * j; }
    }
    
    PASSES_RANDOMLY(chance, 100, RNG_HONEY_GATHER);
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_TEDDIURSA) { Level(level); Ability(ABILITY_HONEY_GATHER); }
        
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } THEN {
        EXPECT_EQ(GetMonData(&gPlayerParty[1], MON_DATA_HELD_ITEM), ITEM_HONEY);
    }
}

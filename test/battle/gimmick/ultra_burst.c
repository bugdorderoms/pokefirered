#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Ultra: Dusk Mane/Dawn Wings Necrozma will transform into Ultra Necrozma")
{
    u32 species;
    
    PARAMETRIZE { species = SPECIES_NECROZMA_DUSK_MANE; }
    PARAMETRIZE { species = SPECIES_NECROZMA_DAWN_WINGS; }
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(species) { Item(ITEM_ULTRANECROZIUM_Z); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SPLASH, gimmick: GIMMICK_ULTRA_BURST); }
    } SCENE {
        MESSAGE("Bright light is about to burst out of Foe Necrozma!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ULTRA_BURST, opponent);
        MESSAGE("Foe Necrozma regained its true power through Ultra Burst!");
    }
}

SINGLE_BATTLE_TEST("Ultra: Ultra Necrozma will not revert form when switched out")
{
    u32 species;
    
    PARAMETRIZE { species = SPECIES_NECROZMA_DUSK_MANE; }
    PARAMETRIZE { species = SPECIES_NECROZMA_DAWN_WINGS; }
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(species) { Item(ITEM_ULTRANECROZIUM_Z); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SPLASH, gimmick: GIMMICK_ULTRA_BURST); }
        TURN { SWITCH(opponent, 1); }
        TURN { SWITCH(opponent, 0); }
    } SCENE {
        MESSAGE("Foe Necrozma regained its true power through Ultra Burst!");
    } THEN {
        EXPECT(GetMonData(&gEnemyParty[0], MON_DATA_SPECIES) == SPECIES_NECROZMA_ULTRA);
    }
}

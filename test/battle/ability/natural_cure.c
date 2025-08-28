#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Natural Cure heals status conditions upon switch out")
{
    GIVEN {
        PLAYER(SPECIES_CELEBI) { Ability(ABILITY_NATURAL_CURE); Status1(STATUS1_POISON); }
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { SWITCH(player, 1); }
        TURN { SWITCH(player, 0); }
    } THEN {
        EXPECT(GetMonData(&gPlayerParty[0], MON_DATA_STATUS_ID) == STATUS1_NONE);
    }
}

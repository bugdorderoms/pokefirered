#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Filter reduces damage to super effective moves by 0.75x", s16 damage)
{
    u32 ability;
    
    PARAMETRIZE { ability = ABILITY_SOUNDPROOF; }
    PARAMETRIZE { ability = ABILITY_FILTER; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_POISON_JAB].type == TYPE_POISON);
        ASSUME(gSpeciesInfo[SPECIES_MR_MIME].types[0] == TYPE_PSYCHIC && gSpeciesInfo[SPECIES_MR_MIME].types[1] == TYPE_FAIRY);
        
        PLAYER(SPECIES_MR_MIME) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_POISON_JAB); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
        MESSAGE("It's super effective!");
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(0.75), results[1].damage);
    }
}

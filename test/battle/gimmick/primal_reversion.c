#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Primal: Primal Reversion activates before switch in abilities")
{
    u32 species, ability, item;
    
    PARAMETRIZE { species = SPECIES_KYOGRE; ability = ABILITY_DRIZZLE; item = ITEM_BLUE_ORB; }
    PARAMETRIZE { species = SPECIES_GROUDON; ability = ABILITY_DROUGHT; item = ITEM_RED_ORB; }
    
    GIVEN {
        PLAYER(species) { Ability(ability); Item(item); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        switch (item)
        {
            case ITEM_BLUE_ORB:
                ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_BLUE_PRIMAL_REVERSION, player);
                MESSAGE("Kyogre's Primal Reversion! It reverted to its primal state!");
                break;
            case ITEM_RED_ORB:
                ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_RED_PRIMAL_REVERSION, player);
                MESSAGE("Groudon's Primal Reversion! It reverted to its primal state!");
                break;
        }
        NOT ABILITY_POPUP(player, ability);
    }
}

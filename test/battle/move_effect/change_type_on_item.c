#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(gBattleMoves[MOVE_JUDGMENT].effect == EFFECT_CHANGE_TYPE_ON_ITEM);
    ASSUME(gBattleMoves[MOVE_JUDGMENT].argument.generic == HOLD_EFFECT_PLATE);
    ASSUME(gItems[ITEM_ZAP_PLATE].holdEffect == HOLD_EFFECT_PLATE);
    ASSUME(gItems[ITEM_ZAP_PLATE].holdEffectParam == TYPE_ELECTRIC);
    ASSUME(gSpeciesInfo[SPECIES_DIGLETT].types[0] == TYPE_GROUND);
}

SINGLE_BATTLE_TEST("Judgment's type isn't affected by Normalize, etc")
{
    GIVEN {
        PLAYER(SPECIES_SKITTY) { Ability(ABILITY_NORMALIZE); Item(ITEM_ZAP_PLATE); }
        OPPONENT(SPECIES_DIGLETT);
    } WHEN {
        TURN { MOVE(player, MOVE_JUDGMENT); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_JUDGMENT, player);
        MESSAGE("It doesn't affect Foe Diglett…");
    }
}

SINGLE_BATTLE_TEST("Judgment's type isn't changed if the user has Klutz")
{
    GIVEN {
        PLAYER(SPECIES_AUDINO) { Ability(ABILITY_KLUTZ); Item(ITEM_ZAP_PLATE); }
        OPPONENT(SPECIES_DIGLETT);
    } WHEN {
        TURN { MOVE(player, MOVE_JUDGMENT); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_JUDGMENT, player);
        NOT MESSAGE("It doesn't affect Foe Diglett…");
    }
}

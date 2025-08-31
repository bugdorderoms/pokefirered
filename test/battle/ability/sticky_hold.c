#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(MoveHasMoveEffect(MOVE_THIEF, MOVE_EFFECT_STEAL_ITEM, FALSE) == TRUE);
}

SINGLE_BATTLE_TEST("Sticky Hold prevents item thief")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_NONE); }
        OPPONENT(SPECIES_GRIMER) { Ability(ABILITY_STICKY_HOLD); Item(ITEM_LIFE_ORB); }
    } WHEN {
        TURN { MOVE(player, MOVE_THIEF); }
    } SCENE {
        NOT MESSAGE("Wobbuffet stole Foe Grimer's Life Orb!");
        ABILITY_POPUP(opponent, ABILITY_STICKY_HOLD);
        MESSAGE("Foe Grimer's Sticky Hold made Thief ineffective!");
    }
}

SINGLE_BATTLE_TEST("Sticky Hold doesn't prevents item thief if it faints")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_NONE); }
        OPPONENT(SPECIES_GRIMER) { Ability(ABILITY_STICKY_HOLD); Item(ITEM_LIFE_ORB); HP(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_THIEF); }
    } SCENE {
        MESSAGE("Wobbuffet stole Foe Grimer's Life Orb!");
        
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_STICKY_HOLD);
            MESSAGE("Foe Grimer's Sticky Hold made Thief ineffective!");
        }
    }
}

TO_DO_BATTLE_TEST("Sticky Hold doesn't prevents a Sticky Barb being transferred");

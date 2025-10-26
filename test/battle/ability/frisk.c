#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Frisk identifies the foe's held item when it enters battle")
{
    GIVEN {
        PLAYER(SPECIES_SENTRET) { Ability(ABILITY_FRISK); }
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_ORAN_BERRY); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_FRISK);
        MESSAGE("Sentret frisked Foe Wobbuffet and found its Oran Berry!");
    }
}

DOUBLE_BATTLE_TEST("Frisk identifies the item of opposing Pokémon's in doubles")
{
    GIVEN {
        PLAYER(SPECIES_SENTRET) { Ability(ABILITY_FRISK); }
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_ORAN_BERRY); }
        OPPONENT(SPECIES_BULBASAUR) { Item(ITEM_LEEK); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_FRISK);
        MESSAGE("Sentret frisked Foe Wobbuffet and found its Oran Berry!");
        MESSAGE("Sentret frisked Foe Bulbasaur and found its Leek!");
    }
}

TO_DO_BATTLE_TEST("Frisk can identifies the held item of a non-adjacent Pokémon in triples");

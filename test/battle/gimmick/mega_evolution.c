#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Mega: Rayquaza can Mega Evolve if it knows Dragon Ascent")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_RAYQUAZA) { Moves(MOVE_DRAGON_ASCENT); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_DRAGON_ASCENT, gimmick: GIMMICK_MEGA); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_MEGA_EVOLUTION, opponent);
        MESSAGE("Foe Rayquaza has Mega Evolved into Mega Rayquaza!");
    }
}

SINGLE_BATTLE_TEST("Mega: Mega Evolved Pokémon will not revert form when switched out")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_GENGAR) { Item(ITEM_GENGARITE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SPLASH, gimmick: GIMMICK_MEGA); }
        TURN { SWITCH(opponent, 1); }
        TURN { SWITCH(opponent, 0); }
    } SCENE {
        MESSAGE("Foe Gengar's Gengarite is reacting to the Key Stone!");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_MEGA_EVOLUTION, opponent);
        MESSAGE("Foe Gengar has Mega Evolved into Mega Gengar!");
    } THEN {
        EXPECT(GetMonData(&gEnemyParty[0], MON_DATA_SPECIES) == SPECIES_GENGAR_MEGA);
    }
}

SINGLE_BATTLE_TEST("Mega: Mega-Evolved Pokémon will attack first if applicable")
{
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_GENGAR_MEGA].baseSpeed > 110);
        
        PLAYER(SPECIES_WOBBUFFET) { Speed(110); }
        OPPONENT(SPECIES_GENGAR) { Item(ITEM_GENGARITE); Speed(100); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
        TURN { MOVE(opponent, MOVE_SPLASH, gimmick: GIMMICK_MEGA); MOVE(player, MOVE_SPLASH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);

        MESSAGE("Foe Gengar has Mega Evolved into Mega Gengar!");
        
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPLASH, player);
    }
}

SINGLE_BATTLE_TEST("Mega: Gengar will remove Telekinesis's effect when it Mega Evolves")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_TELEKINESIS].effect == EFFECT_TELEKINESIS);
        ASSUME(gBattleMoves[MOVE_EARTHQUAKE].type == TYPE_GROUND);
        ASSUME(gSpeciesInfo[SPECIES_GENGAR].suppressEnemyShadow == FALSE);
        ASSUME(gSpeciesInfo[SPECIES_GENGAR_MEGA].suppressEnemyShadow == TRUE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_GENGAR) { Item(ITEM_GENGARITE); }
    } WHEN {
        TURN { MOVE(player, MOVE_TELEKINESIS); }
        TURN { MOVE(opponent, MOVE_SPLASH, gimmick: GIMMICK_MEGA); MOVE(player, MOVE_EARTHQUAKE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TELEKINESIS, player);
        MESSAGE("Foe Gengar has Mega Evolved into Mega Gengar!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EARTHQUAKE, player);
    }
}

SINGLE_BATTLE_TEST("Mega: Intimidate will activate when the Pokémon Mega Evolves")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_MANECTRIC) { Item(ITEM_MANECTITE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SPLASH, gimmick: GIMMICK_MEGA); }
    } SCENE {
        MESSAGE("Foe Manectric has Mega Evolved into Mega Manectric!");
        ABILITY_POPUP(opponent, ABILITY_INTIMIDATE);
        MESSAGE("Wobbuffet's Attack fell!");
    }
}

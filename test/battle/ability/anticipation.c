#include "global.h"
#include "test/battle.h"

DOUBLE_BATTLE_TEST("Anticipation notifies if an opponent has a super-effective move on switch in")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_CLOSE_COMBAT].type == TYPE_FIGHTING);
        ASSUME(gSpeciesInfo[SPECIES_EEVEE].types[0] == TYPE_NORMAL || gSpeciesInfo[SPECIES_EEVEE].types[1] == TYPE_NORMAL);
        
        PLAYER(SPECIES_EEVEE) { Ability(ABILITY_ANTICIPATION); }
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_SCRATCH, MOVE_SPLASH, MOVE_CLOSE_COMBAT, MOVE_POUND); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(playerLeft, ABILITY_ANTICIPATION);
        MESSAGE("Eevee shuddered!");
    }
}

SINGLE_BATTLE_TEST("Anticipation doesn't notifies super-effective status moves")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_WILL_O_WISP].type == TYPE_FIRE);
        ASSUME(gBattleMoves[MOVE_WILL_O_WISP].split == SPLIT_STATUS);
        ASSUME(gSpeciesInfo[SPECIES_FERROTHORN].types[0] == TYPE_GRASS || gSpeciesInfo[SPECIES_FERROTHORN].types[1] == TYPE_GRASS);
        
        PLAYER(SPECIES_FERROTHORN) { Ability(ABILITY_ANTICIPATION); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH, MOVE_WILL_O_WISP, MOVE_SCRATCH, MOVE_POUND); }
    } WHEN {
        TURN { }
    } SCENE {
        NONE_OF {
            ABILITY_POPUP(player, ABILITY_ANTICIPATION);
            MESSAGE("Ferrothorn shuddered!");
        }
    }
}

SINGLE_BATTLE_TEST("Anticipation notifies a one-hit ko move")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_GUILLOTINE].effect == EFFECT_OHKO);
        ASSUME(gBattleMoves[MOVE_GUILLOTINE].type == TYPE_NORMAL);
        ASSUME(gSpeciesInfo[SPECIES_EEVEE].types[0] == TYPE_NORMAL || gSpeciesInfo[SPECIES_EEVEE].types[1] == TYPE_NORMAL);
        
        PLAYER(SPECIES_EEVEE) { Ability(ABILITY_ANTICIPATION); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH, MOVE_GUILLOTINE, MOVE_SCRATCH, MOVE_POUND); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_ANTICIPATION);
        MESSAGE("Eevee shuddered!");
    }
}

SINGLE_BATTLE_TEST("Anticipation notifies against Counter, Mirror Coat and Metal Burst")
{
    u32 move, species, typeAtk, typeDef;
    
    PARAMETRIZE { move = MOVE_COUNTER; species = SPECIES_RATICATE; typeAtk = TYPE_FIGHTING; typeDef = TYPE_NORMAL; }
    PARAMETRIZE { move = MOVE_MIRROR_COAT; species = SPECIES_NIDORINO; typeAtk = TYPE_PSYCHIC; typeDef = TYPE_POISON; }
    PARAMETRIZE { move = MOVE_METAL_BURST; species = SPECIES_ROGGENROLA; typeAtk = TYPE_STEEL; typeDef = TYPE_ROCK; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SKILL_SWAP].effect == EFFECT_SKILL_SWAP);
        ASSUME(gBattleMoves[move].effect == EFFECT_COUNTER_ATTACK);
        ASSUME(gBattleMoves[move].type == typeAtk);
        ASSUME(gSpeciesInfo[species].types[0] == typeDef || gSpeciesInfo[species].types[1] == typeDef);
        
        PLAYER(species);
        OPPONENT(SPECIES_EEVEE) { Ability(ABILITY_ANTICIPATION); Moves(move, MOVE_SKILL_SWAP, MOVE_SCRATCH, MOVE_SPLASH); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SKILL_SWAP); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SKILL_SWAP, opponent);
        MESSAGE("Foe Eevee swapped Abilities with its target!");
        
        ABILITY_POPUP(player, ABILITY_ANTICIPATION);
        MESSAGE("%s shuddered!", species);
    }
}

SINGLE_BATTLE_TEST("Anticipation considers Synchronoise as an ordinary Psychic-type move")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SKILL_SWAP].effect == EFFECT_SKILL_SWAP);
        ASSUME(gBattleMoves[MOVE_SYNCHRONOISE].effect == EFFECT_SYNCHRONOISE);
        ASSUME(gBattleMoves[MOVE_SYNCHRONOISE].type == TYPE_PSYCHIC);
        ASSUME(gSpeciesInfo[SPECIES_NIDORINO].types[0] == TYPE_POISON || gSpeciesInfo[SPECIES_NIDORINO].types[1] == TYPE_POISON);
        ASSUME(gSpeciesInfo[SPECIES_EEVEE].types[0] != TYPE_POISON && gSpeciesInfo[SPECIES_EEVEE].types[1] != TYPE_POISON);
        
        PLAYER(SPECIES_NIDORINO);
        OPPONENT(SPECIES_EEVEE) { Ability(ABILITY_ANTICIPATION); Moves(MOVE_SYNCHRONOISE, MOVE_SKILL_SWAP, MOVE_SCRATCH, MOVE_SPLASH); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SKILL_SWAP); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SKILL_SWAP, opponent);
        MESSAGE("Foe Eevee swapped Abilities with its target!");
        
        ABILITY_POPUP(player, ABILITY_ANTICIPATION);
        MESSAGE("Nidorino shuddered!");
    }
}

SINGLE_BATTLE_TEST("Anticipation doesn't considers Judgment, Natural Gift, etc type change")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_NATURAL_GIFT].effect == EFFECT_NATURAL_GIFT);
        ASSUME(gBattleMoves[MOVE_NATURAL_GIFT].type == TYPE_NORMAL);
        ASSUME(gSpeciesInfo[SPECIES_EEVEE].types[0] == TYPE_NORMAL || gSpeciesInfo[SPECIES_EEVEE].types[1] == TYPE_NORMAL);
        
        PLAYER(SPECIES_EEVEE) { Ability(ABILITY_ANTICIPATION); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH, MOVE_NATURAL_GIFT, MOVE_SCRATCH, MOVE_POUND); Item(ITEM_LEPPA_BERRY); }
    } WHEN {
        TURN { }
    } SCENE {
        NONE_OF {
            ABILITY_POPUP(player, ABILITY_ANTICIPATION);
            MESSAGE("Eevee shuddered!");
        }
    }
}

SINGLE_BATTLE_TEST("Anticipation considers Hidden Power's real type")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_HIDDEN_POWER].effect == EFFECT_HIDDEN_POWER);
        ASSUME(gBattleMoves[MOVE_HIDDEN_POWER].type == TYPE_NORMAL);
        ASSUME(gSpeciesInfo[SPECIES_EEVEE].types[0] == TYPE_NORMAL || gSpeciesInfo[SPECIES_EEVEE].types[1] == TYPE_NORMAL);
        
        PLAYER(SPECIES_EEVEE) { Ability(ABILITY_ANTICIPATION); }
        OPPONENT(SPECIES_WOBBUFFET) {
            Moves(MOVE_SPLASH, MOVE_HIDDEN_POWER, MOVE_SCRATCH, MOVE_POUND);
            HPIV(31); AttackIV(31); DefenseIV(30); SpAttackIV(30); SpDefenseIV(30); SpeedIV(30); // HP fighting
        }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_ANTICIPATION);
        MESSAGE("Eevee shuddered!");
    }
}

SINGLE_BATTLE_TEST("Anticipation doesn't considers strong winds on type matchups")
{
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_RAYQUAZA_MEGA].types[0] == TYPE_DRAGON && gSpeciesInfo[SPECIES_RAYQUAZA_MEGA].types[1] == TYPE_FLYING);
        ASSUME(gBattleMoves[MOVE_SKILL_SWAP].effect == EFFECT_SKILL_SWAP);
        
        PLAYER(SPECIES_RAYQUAZA) { Moves(MOVE_DRAGON_ASCENT, MOVE_SPLASH); }
        OPPONENT(SPECIES_EEVEE) { Ability(ABILITY_ANTICIPATION); Moves(MOVE_ROCK_SLIDE, MOVE_SKILL_SWAP, MOVE_SCRATCH, MOVE_SPLASH); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH, gimmick: GIMMICK_MEGA); MOVE(opponent, MOVE_SKILL_SWAP); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_DELTA_STREAM);
        
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SKILL_SWAP, opponent);
        MESSAGE("Foe Eevee swapped Abilities with its target!");
        
        ABILITY_POPUP(player, ABILITY_ANTICIPATION);
        MESSAGE("Rayquaza shuddered!");
    }
}

SINGLE_BATTLE_TEST("Anticipation doesn't consider Normalize, etc into their effectiveness")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_CLOSE_COMBAT].type == TYPE_FIGHTING);
        ASSUME(gSpeciesInfo[SPECIES_EEVEE].types[0] == TYPE_NORMAL || gSpeciesInfo[SPECIES_EEVEE].types[1] == TYPE_NORMAL);
        
        PLAYER(SPECIES_EEVEE) { Ability(ABILITY_ANTICIPATION); }
        OPPONENT(SPECIES_DELCATTY) { Ability(ABILITY_NORMALIZE); Moves(MOVE_CLOSE_COMBAT, MOVE_SCRATCH, MOVE_POUND, MOVE_SPLASH); }
    } WHEN {
        TURN { }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_ANTICIPATION);
        MESSAGE("Eevee shuddered!");
    }
}

SINGLE_BATTLE_TEST("Anticipation doesn't consider Gravity into their effectiveness")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SKILL_SWAP].effect == EFFECT_SKILL_SWAP);
        ASSUME(gBattleMoves[MOVE_GRAVITY].effect == EFFECT_GRAVITY);
        ASSUME(gBattleMoves[MOVE_EARTHQUAKE].type == TYPE_GROUND);
        ASSUME(gSpeciesInfo[SPECIES_SKARMORY].types[0] == TYPE_STEEL && gSpeciesInfo[SPECIES_SKARMORY].types[1] == TYPE_FLYING);
        
        PLAYER(SPECIES_SKARMORY);
        OPPONENT(SPECIES_EEVEE) { Ability(ABILITY_ANTICIPATION); Moves(MOVE_EARTHQUAKE, MOVE_GRAVITY, MOVE_SCRATCH, MOVE_POUND); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_GRAVITY); MOVE(player, MOVE_SKILL_SWAP); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GRAVITY, opponent);
        
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SKILL_SWAP, player);
        MESSAGE("Skarmory swapped Abilities with its target!");
        
        NONE_OF {
            ABILITY_POPUP(player, ABILITY_ANTICIPATION);
            MESSAGE("Skarmory shuddered!");
        }
    }
}

TO_DO_BATTLE_TEST("Anticipation considers Freeze-Dry as an ordinary Ice-type move");
TO_DO_BATTLE_TEST("Anticipation considers Flying Press as an ordinary Fighting-type move");
TO_DO_BATTLE_TEST("Anticipation doesn't consider Scrappy into their effectiveness");
TO_DO_BATTLE_TEST("Anticipation can activate by a non-adjacent Pokémon's super-effective move");

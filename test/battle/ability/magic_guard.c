#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Magic Guard prevents weather damage")
{
    GIVEN {
        PLAYER(SPECIES_ABRA) { Ability(ABILITY_MAGIC_GUARD); }
        OPPONENT(SPECIES_TYRANITAR) { Ability(ABILITY_SAND_STREAM); }
    } WHEN {
        TURN { }
    } SCENE {
        NOT HP_BAR(player);
    }
}

SINGLE_BATTLE_TEST("Magic Guard prevents burn damage")
{
    GIVEN {
        PLAYER(SPECIES_ABRA) { Ability(ABILITY_MAGIC_GUARD); Status1(STATUS1_BURN); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        NONE_OF {
            MESSAGE("Abra is hurt by its burn!");
            HP_BAR(player);
        }
    }
}

SINGLE_BATTLE_TEST("Magic Guard prevents poison damage")
{
    u32 status;
    
    PARAMETRIZE { status = STATUS1_POISON; }
    PARAMETRIZE { status = STATUS1_TOXIC_POISON; }
    
    GIVEN {
        PLAYER(SPECIES_ABRA) { Ability(ABILITY_MAGIC_GUARD); Status1(status); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        NONE_OF {
            MESSAGE("Abra is hurt by poison!");
            HP_BAR(player);
        }
    }
}

SINGLE_BATTLE_TEST("Magic Guard doesn't prevents bad poison counter increase")
{
    GIVEN {
        PLAYER(SPECIES_ABRA) { Ability(ABILITY_MAGIC_GUARD); Status1(STATUS1_TOXIC_POISON); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } SCENE {
        NOT HP_BAR(player);
    } THEN {
        EXPECT(player->status1.counter == 1);
    }
}

SINGLE_BATTLE_TEST("Magic Guard prevents curse damage")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_CURSE].effect == EFFECT_CURSE);
        ASSUME(gSpeciesInfo[SPECIES_GENGAR].types[0] == TYPE_GHOST || gSpeciesInfo[SPECIES_GENGAR].types[1] == TYPE_GHOST);
        
        PLAYER(SPECIES_ABRA) { Ability(ABILITY_MAGIC_GUARD); }
        OPPONENT(SPECIES_GENGAR);
    } WHEN {
        TURN { MOVE(opponent, MOVE_CURSE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CURSE, opponent);
        
        NONE_OF {
            MESSAGE("Abra is afflicted by the curse!");
            HP_BAR(player);
        }
    }
}

SINGLE_BATTLE_TEST("Magic Guard prevents nightmare damage")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_REST].effect == EFFECT_REST);
        ASSUME(gBattleMoves[MOVE_NIGHTMARE].effect == EFFECT_NIGHTMARE);
        
        PLAYER(SPECIES_ABRA) { Ability(ABILITY_MAGIC_GUARD); HP(50); MaxHP(100); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_REST); MOVE(opponent, MOVE_NIGHTMARE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_REST, player);
        HP_BAR(player);
        
        NONE_OF {
            MESSAGE("Abra is locked in a nightmare!");
            HP_BAR(player);
        }
    }
}

SINGLE_BATTLE_TEST("Magic Guard prevents Leech Seed damage")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_LEECH_SEED].effect == EFFECT_SET_SEEDED);
        
        PLAYER(SPECIES_ABRA) { Ability(ABILITY_MAGIC_GUARD); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_LEECH_SEED); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LEECH_SEED, opponent);
        
        NONE_OF {
            HP_BAR(player);
            HP_BAR(opponent);
            MESSAGE("Abra's health is sapped by Leech Seed!");
        }
    }
}

SINGLE_BATTLE_TEST("Magic Guard prevents entry hazards damage")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SPIKES].effect == EFFECT_SPIKES);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_ABRA) { Ability(ABILITY_MAGIC_GUARD); }
        
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SPIKES); }
        TURN { SWITCH(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPIKES, opponent);
        
        NONE_OF {
            HP_BAR(player);
            MESSAGE("Abra is hurt by spikes!");
        }
    }
}

SINGLE_BATTLE_TEST("Magic Guard prevents recoil damage, except from Struggle")
{
    u32 move;
    
    PARAMETRIZE { move = MOVE_FLARE_BLITZ; }
    PARAMETRIZE { move = MOVE_STRUGGLE; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_FLARE_BLITZ].recoilDivisor > 0);
        ASSUME(gBattleMoves[MOVE_STRUGGLE].recoilDivisor > 0);
        
        PLAYER(SPECIES_ABRA) { Ability(ABILITY_MAGIC_GUARD); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        
        if (move == MOVE_STRUGGLE) {
            HP_BAR(player);
            MESSAGE("Abra is hit with recoil!");
        } else {
            NONE_OF {
                HP_BAR(player);
                MESSAGE("Abra is hit with recoil!");
            }
        }
    }
}

SINGLE_BATTLE_TEST("Magic Guard prevents crash damage")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_JUMP_KICK].effect == EFFECT_RECOIL_IF_MISS);
        
        PLAYER(SPECIES_ABRA) { Ability(ABILITY_MAGIC_GUARD); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_JUMP_KICK, hit: FALSE); }
    } SCENE {
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_JUMP_KICK, player);
            MESSAGE("Abra kept going and crashed!");
            HP_BAR(player);
        }
    }
}

SINGLE_BATTLE_TEST("Magic Guard prevents bind damage, but not the counter decrease")
{
    GIVEN {
        ASSUME(MoveHasMoveEffect(MOVE_FIRE_SPIN, MOVE_EFFECT_WRAP, FALSE) == TRUE);
        
        PLAYER(SPECIES_ABRA) { Ability(ABILITY_MAGIC_GUARD); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_FIRE_SPIN, WITH_RNG(RNG_WRAP_TURNS, 4)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FIRE_SPIN, opponent);
        HP_BAR(player);
        
        NONE_OF {
            MESSAGE("Abra is hurt by Fire Spin!");
            HP_BAR(player);
        }
    } THEN {
        EXPECT_EQ(gDisableStructs[B_POSITION_PLAYER_LEFT].wrapTurns, 3);
    }
}

SINGLE_BATTLE_TEST("Magic Guard prevents Bad Dreams damage")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_REST].effect == EFFECT_REST);
        
        PLAYER(SPECIES_ABRA) { Ability(ABILITY_MAGIC_GUARD); HP(50); MaxHP(100); }
        OPPONENT(SPECIES_DARKRAI) { Ability(ABILITY_BAD_DREAMS); }
    } WHEN {
        TURN { MOVE(player, MOVE_REST); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_REST, player);
        HP_BAR(player);
        
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_BAD_DREAMS);
            MESSAGE("Abra is tormented!");
            HP_BAR(player);
        }
    }
}

DOUBLE_BATTLE_TEST("Magic Guard prevents Flame Burst's secondary damage")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_FLAME_BURST].effect == EFFECT_FLAME_BURST);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_ABRA) { Ability(ABILITY_MAGIC_GUARD); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_FLAME_BURST, target: opponentLeft); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FLAME_BURST, playerLeft);
        HP_BAR(opponentLeft);
        
        NONE_OF {
            HP_BAR(opponentRight);
            MESSAGE("The bursting flame hit Foe Abra!");
        }
    }
}

SINGLE_BATTLE_TEST("Magic Guard prevents damage from Iron Barbs, Aftermath, etc")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_SHARPEDO; ability = ABILITY_ROUGH_SKIN; }
    PARAMETRIZE { species = SPECIES_FERROSEED; ability = ABILITY_IRON_BARBS; }
    PARAMETRIZE { species = SPECIES_VOLTORB; ability = ABILITY_AFTERMATH; }
    PARAMETRIZE { species = SPECIES_PYUKUMUKU; ability = ABILITY_INNARDS_OUT; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].flags.makesContact == TRUE);
        
        PLAYER(SPECIES_ABRA) { Ability(ABILITY_MAGIC_GUARD); }
        OPPONENT(species) { Ability(ability); MaxHP(100); HP(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        HP_BAR(opponent);
        
        NONE_OF {
            ABILITY_POPUP(opponent, ability);
            HP_BAR(player);
            MESSAGE("Abra was hurt!");
        }
    }
}

TO_DO_BATTLE_TEST("Magic Guard prevents Mind Blown and Steel Beam damage");
TO_DO_BATTLE_TEST("Magic Guard prevents Life Orb damage");
TO_DO_BATTLE_TEST("Magic Guard prevents Black Sludge damage");
TO_DO_BATTLE_TEST("Magic Guard prevents Rocky Helmet damage");
TO_DO_BATTLE_TEST("Magic Guard prevents Gulp Missile damage");
TO_DO_BATTLE_TEST("Magic Guard prevents sea of fire damage");
TO_DO_BATTLE_TEST("Magic Guard prevents Sticky Barb damage");
TO_DO_BATTLE_TEST("Magic Guard prevents Spiky Shield damage");
TO_DO_BATTLE_TEST("Magic Guard prevents Powder damage");

#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Dyna: Dynamax increases HP and max HP by 1.5x", u16 hp)
{
    u32 gimmick;

    PARAMETRIZE { gimmick = GIMMICK_NONE; }
    PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; }
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, gimmick: gimmick); }
    } SCENE {
        if (gimmick) {
            MESSAGE("Time to Dynamax!");
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_DYNAMAX_GROWTH, player);
            HP_BAR(player);
            MESSAGE("Wobbuffet grew huge into its Dynamax form!");
        }
    } THEN {
        results[i].hp = player->hp;
    } FINALLY {
        EXPECT_MUL_EQ(results[0].hp, UQ_4_12(1.5), results[1].hp);
    }
}

SINGLE_BATTLE_TEST("Dyna: Dynamax Level increases HP and max HP multipliers by 0.05x for each level", u16 hp)
{
    u32 gimmick, level;

    PARAMETRIZE { gimmick = GIMMICK_NONE; level = 0; }
    PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; level = 0; }
    PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; level = 1; }
    PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; level = 2; }
    PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; level = 3; }
    PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; level = 4; }
    PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; level = 5; }
    PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; level = 6; }
    PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; level = 7; }
    PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; level = 8; }
    PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; level = 9; }
    PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; level = 10; }
    
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { DynamaxLevel(level); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, gimmick: gimmick); }
    } SCENE {
        if (gimmick) {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_DYNAMAX_GROWTH, player);
            HP_BAR(player);
            MESSAGE("Wobbuffet grew huge into its Dynamax form!");
        }
    } THEN {
        results[i].hp = player->hp;
    } FINALLY {
        EXPECT_MUL_EQ(results[0].hp, UQ_4_12(1.5), results[1].hp);
        EXPECT_MUL_EQ(results[0].hp, UQ_4_12(1.55), results[2].hp);
        EXPECT_MUL_EQ(results[0].hp, UQ_4_12(1.6), results[3].hp);
        EXPECT_MUL_EQ(results[0].hp, UQ_4_12(1.65), results[4].hp);
        EXPECT_MUL_EQ(results[0].hp, UQ_4_12(1.7), results[5].hp);
        EXPECT_MUL_EQ(results[0].hp, UQ_4_12(1.75), results[6].hp);
        EXPECT_MUL_EQ(results[0].hp, UQ_4_12(1.8), results[7].hp);
        EXPECT_MUL_EQ(results[0].hp, UQ_4_12(1.85), results[8].hp);
        EXPECT_MUL_EQ(results[0].hp, UQ_4_12(1.9), results[9].hp);
        EXPECT_MUL_EQ(results[0].hp, UQ_4_12(1.95), results[10].hp);
        EXPECT_MUL_EQ(results[0].hp, UQ_4_12(2.0), results[11].hp);
    }
}

SINGLE_BATTLE_TEST("Dyna: Dynamax HP boost doesn't applies to Shedinja")
{
    GIVEN {
        ASSUME(gSpeciesInfo[SPECIES_SHEDINJA].baseHP == 1);
        
        PLAYER(SPECIES_SHEDINJA);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        MESSAGE("Shedinja grew huge into its Dynamax form!");
    } THEN {
        EXPECT_EQ(gSpeciesInfo[SPECIES_SHEDINJA].baseHP, player->hp);
    }
}

SINGLE_BATTLE_TEST("Dyna: Dynamaxed Pokemon that changes forms does not gain HP")
{
    u16 capturedHP;
    
    GIVEN {
        PLAYER(SPECIES_CHERRIM) { Ability(ABILITY_FLOWER_GIFT); }
        OPPONENT(SPECIES_TORKOAL) { Ability(ABILITY_DROUGHT); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, gimmick: GIMMICK_DYNAMAX); MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_DROUGHT);
        ABILITY_POPUP(player, ABILITY_FLOWER_GIFT);
        
        MESSAGE("Cherrim grew huge into its Dynamax form!");
        
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
        HP_BAR(player, captureHP: &capturedHP);
    } THEN {
        EXPECT_EQ(capturedHP, player->hp);
    }
}

SINGLE_BATTLE_TEST("Dyna: Dynamaxing with the gigantamax factor changes the user's form if applicable")
{
    bool32 gigantamaxFactor;
    
    PARAMETRIZE { gigantamaxFactor = FALSE; }
    PARAMETRIZE { gigantamaxFactor = TRUE; }
    
    GIVEN {
        PLAYER(SPECIES_CHARIZARD) { GigantamaxFactor(gigantamaxFactor); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        if (gigantamaxFactor) {
            MESSAGE("Time to Gigantamax!");
        } else {
            MESSAGE("Time to Dynamax!");
        }
        
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_DYNAMAX_GROWTH, player);
        HP_BAR(player);
        
        if (gigantamaxFactor) {
            MESSAGE("Charizard grew huge into its Gigantamax form!");
        } else {
            MESSAGE("Charizard grew huge into its Dynamax form!");
        }
    }
}

SINGLE_BATTLE_TEST("Dyna: Dynamax expires after three turns", u16 hp)
{
    u32 j;

    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].type == TYPE_NORMAL);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, gimmick: GIMMICK_DYNAMAX); }
        TURN { MOVE(player, MOVE_SCRATCH); }
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("Wobbuffet grew huge into its Dynamax form!");
        
        for (j = 0; j < 3; ++j) {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_STRIKE, player);
        }
        
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        MESSAGE("Wobbuffet is no longer Dynamaxed!");
    } THEN {
        results[i].hp = player->hp;
    } FINALLY {
        EXPECT_EQ(results[0].hp, results[1].hp);
    }
}

SINGLE_BATTLE_TEST("Dyna: Illusion doesn't break upon Dynamaxing when illusioned")
{
    GIVEN {
        PLAYER(SPECIES_ZOROARK);
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, gimmick: GIMMICK_DYNAMAX); MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        MESSAGE("Wobbuffet grew huge into its Dynamax form!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
        ABILITY_POPUP(player, ABILITY_ILLUSION);
    }
}

SINGLE_BATTLE_TEST("Dyna: Dynamaxed Pokemon cannot be flinched")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].type == TYPE_NORMAL);
        ASSUME(MoveHasMoveEffectWithChance(MOVE_FAKE_OUT, MOVE_EFFECT_FLINCH, 100) == TRUE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_FAKE_OUT); MOVE(player, MOVE_SCRATCH, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        MESSAGE("Wobbuffet grew huge into its Dynamax form!");
        
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FAKE_OUT, opponent);
        
        NOT MESSAGE("Wobbuffet flinched!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_STRIKE, player);
    }
}

SINGLE_BATTLE_TEST("Dyna: Focus Punch doesn't set up if its Dynamaxed")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_FOCUS_PUNCH].effect == EFFECT_FOCUS_PUNCH);
        ASSUME(gBattleMoves[MOVE_FOCUS_PUNCH].type == TYPE_FIGHTING);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_FOCUS_PUNCH, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        MESSAGE("Wobbuffet grew huge into its Dynamax form!");
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FOCUS_PUNCH_SETUP, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_KNUCKLE, player);
    }
}

SINGLE_BATTLE_TEST("Dyna: The user loses its Substitute upon Dynamaxing")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SUBSTITUTE].effect == EFFECT_SUBSTITUTE);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SUBSTITUTE); }
        TURN { MOVE(player, MOVE_SCRATCH, gimmick: GIMMICK_DYNAMAX); MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, player);
        
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SUBSTITUTE_FADE, player);
        MESSAGE("Wobbuffet grew huge into its Dynamax form!");
        
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
        HP_BAR(player);
    }
}

SINGLE_BATTLE_TEST("Dyna: Cramorant reverts form back before it Dynamaxes")
{
    GIVEN {
        PLAYER(SPECIES_CRAMORANT) { Ability(ABILITY_GULP_MISSILE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SURF); }
        TURN { MOVE(player, MOVE_SCRATCH, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        // turn 1
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SURF, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        // turn 2
        MESSAGE("Cramorant grew huge into its Dynamax form!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_STRIKE, player);
    } THEN {
        EXPECT_EQ(player->species, SPECIES_CRAMORANT);
    }
}

SINGLE_BATTLE_TEST("Dyna: Dynamax turns the user's moves into Max Moves depending on its type and category")
{
    u32 move, maxMove;
    
    PARAMETRIZE { move = MOVE_SCRATCH; maxMove = MOVE_MAX_STRIKE; }
    PARAMETRIZE { move = MOVE_SCALD; maxMove = MOVE_MAX_GEYSER; }
    PARAMETRIZE { move = MOVE_SWORDS_DANCE; maxMove = MOVE_MAX_GUARD; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SCRATCH].type == TYPE_NORMAL);
        ASSUME(gBattleMoves[MOVE_SCALD].type == TYPE_WATER);
        ASSUME(gBattleMoves[MOVE_SWORDS_DANCE].split == SPLIT_STATUS);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, move, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        MESSAGE("Wobbuffet grew huge into its Dynamax form!");
        ANIMATION(ANIM_TYPE_MOVE, maxMove, player);
    }
}

SINGLE_BATTLE_TEST("Dyna: Dynamaxing with the gigantamax factor turns the user's moves to their G-Max Moves depending on its type")
{
    u32 species, move, gMaxMove;
    
    PARAMETRIZE { species = SPECIES_VENUSAUR; move = MOVE_SEED_BOMB; gMaxMove = MOVE_GMAX_VINE_LASH; }
    PARAMETRIZE { species = SPECIES_CHARIZARD; move = MOVE_EMBER; gMaxMove = MOVE_GMAX_WILDFIRE; }
    PARAMETRIZE { species = SPECIES_BLASTOISE; move = MOVE_WATER_GUN; gMaxMove = MOVE_GMAX_CANNONADE; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SEED_BOMB].type == TYPE_GRASS);
        ASSUME(gBattleMoves[MOVE_EMBER].type == TYPE_FIRE);
        ASSUME(gBattleMoves[MOVE_WATER_GUN].type == TYPE_WATER);
        
        PLAYER(species) { GigantamaxFactor(TRUE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, move, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        MESSAGE("%s grew huge into its Gigantamax form!", species);
        ANIMATION(ANIM_TYPE_MOVE, gMaxMove, player);
    }
}

SINGLE_BATTLE_TEST("Dyna: Dynamax can't turn Struggle into a Max Move")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_STRUGGLE, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        MESSAGE("Wobbuffet grew huge into its Dynamax form!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STRUGGLE, player);
    }
}

SINGLE_BATTLE_TEST("Dyna: Dynamaxed Pokémon are affected by Grudge")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_GRUDGE].effect == EFFECT_SET_ATTACKER_STATUS);
        ASSUME(gBattleMoves[MOVE_GRUDGE].argument.setStatus.statusId == ID_STATUS3 && UNCOMPRESS_BITS(gBattleMoves[MOVE_GRUDGE].argument.setStatus.status) == STATUS3_GRUDGE);
        
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Speed(100); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_GRUDGE); MOVE(player, MOVE_SCRATCH, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GRUDGE, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_STRIKE, player);
        MESSAGE("Wobbuffet's Scratch lost all its PP due to the grudge!");
        MESSAGE("Foe Wobbuffet fainted!");
    }
}

SINGLE_BATTLE_TEST("Dyna: Max Moves deals 1/4 damage through protect", s16 damage)
{
    bool32 protected;
    
    PARAMETRIZE { protected = FALSE; }
    PARAMETRIZE { protected = TRUE; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_PROTECT].effect == EFFECT_PROTECT);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        if (protected) {
            TURN { MOVE(player, MOVE_SCRATCH, gimmick: GIMMICK_DYNAMAX); MOVE(opponent, MOVE_PROTECT); }
        } else {
            TURN { MOVE(player, MOVE_SCRATCH, gimmick: GIMMICK_DYNAMAX); }
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_STRIKE, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
        
        if (protected) {
            MESSAGE("Foe Wobbuffet couldn't fully protect itself and got hurt!");
        }
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(0.25), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Dyna: Max Moves don't bypass Max Guard")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_PROTECT].split == SPLIT_STATUS);
        ASSUME(gBattleMoves[MOVE_MAX_GUARD].effect == EFFECT_MAX_GUARD);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, gimmick: GIMMICK_DYNAMAX); MOVE(opponent, MOVE_PROTECT, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_GUARD, opponent);
        
        MESSAGE("Wobbuffet used Max Strike!");
        MESSAGE("Foe Wobbuffet protected itself!");
        
        NONE_OF {
            HP_BAR(opponent);
            MESSAGE("Foe Wobbuffet couldn't fully protect itself and got hurt!");
        }
    }
}

DOUBLE_BATTLE_TEST("Dyna: Feint bypasses Max Guard but doesn't break it")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_PROTECT].split == SPLIT_STATUS);
        ASSUME(gBattleMoves[MOVE_MAX_GUARD].effect == EFFECT_MAX_GUARD);
        ASSUME(MoveHasMoveEffect(MOVE_FEINT, MOVE_EFFECT_FEINT, FALSE) == TRUE);
        
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_PROTECT, gimmick: GIMMICK_DYNAMAX);
               MOVE(opponentLeft, MOVE_FEINT, target: playerLeft);
               MOVE(opponentRight, MOVE_SCRATCH, target: playerLeft);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_GUARD, playerLeft);
        
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FEINT, opponentLeft);
        HP_BAR(playerLeft);
        
        NONE_OF {
            MESSAGE("Wobbuffet fell for the feint!");
            
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponentRight);
            HP_BAR(playerLeft);
        }
    }
}

SINGLE_BATTLE_TEST("Dyna: G-Max Drum Solo, G-Max Hydrosnipe and G-Max Fireball have fixed base powers", s16 damage)
{
    u32 j;
    u32 species, move1, move2, type;
    static const u32 maxMoveFixedPowerData[][4] =
    {
        {SPECIES_RILLABOOM,  MOVE_SEED_BOMB,  MOVE_BULLET_SEED,   TYPE_GRASS},
        {SPECIES_CINDERACE,  MOVE_EMBER,      MOVE_FLAMETHROWER,  TYPE_FIRE},
        {SPECIES_INTELEON,   MOVE_WATER_GUN,  MOVE_HYDRO_PUMP,    TYPE_WATER},
    };
    
    for (j = 0; j < ARRAY_COUNT(maxMoveFixedPowerData); j++)
    {
        PARAMETRIZE { species = maxMoveFixedPowerData[j][0]; move1 = maxMoveFixedPowerData[j][1]; move2 = maxMoveFixedPowerData[j][2]; type = maxMoveFixedPowerData[j][3]; }
        PARAMETRIZE { species = maxMoveFixedPowerData[j][0]; move1 = maxMoveFixedPowerData[j][2]; move2 = maxMoveFixedPowerData[j][1]; type = maxMoveFixedPowerData[j][3]; }
    }
    
    GIVEN {
        ASSUME(gBattleMoves[move1].type == type);
        ASSUME(gBattleMoves[move2].type == type);
        ASSUME(gBattleMoves[move1].power != gBattleMoves[move2].power);
        
        PLAYER(species) { GigantamaxFactor(TRUE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, move1, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
        EXPECT_EQ(results[2].damage, results[3].damage);
        EXPECT_EQ(results[4].damage, results[5].damage);
    }
}

DOUBLE_BATTLE_TEST("Dyna: G-Max One Blow and G-Max Rapid Flow bypasses Max Guard for full damage", s16 damage)
{
    u32 j;
    u32 species, move, gMaxMove;
    bool32 protected;
    static const u32 maxOneBlowData[][3] =
    {
        {SPECIES_URSHIFU,                     MOVE_WICKED_BLOW,      MOVE_GMAX_ONE_BLOW},
        {SPECIES_URSHIFU_RAPID_STRIKE_STYLE,  MOVE_SURGING_STRIKES,  MOVE_GMAX_RAPID_FLOW},
    };
    
    for (j = 0; j < ARRAY_COUNT(maxOneBlowData); j++)
    {
        PARAMETRIZE { species = maxOneBlowData[j][0]; move = maxOneBlowData[j][1]; gMaxMove = maxOneBlowData[j][2]; protected = TRUE; }
        PARAMETRIZE { species = maxOneBlowData[j][0]; move = maxOneBlowData[j][1]; gMaxMove = maxOneBlowData[j][2]; protected = FALSE; }
    }

    GIVEN {
        ASSUME(gBattleMoves[MOVE_GMAX_ONE_BLOW].flags.forbiddenProtect == TRUE);
        ASSUME(gBattleMoves[MOVE_GMAX_RAPID_FLOW].flags.forbiddenProtect == TRUE);
        ASSUME(gBattleMoves[MOVE_PROTECT].split == SPLIT_STATUS);
        
        PLAYER(species) { GigantamaxFactor(TRUE); }
        PLAYER(SPECIES_WOBBUFFET);
        
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        if (protected) {
            TURN { MOVE(playerLeft, move, target: opponentLeft, gimmick: GIMMICK_DYNAMAX);
                   MOVE(opponentLeft, MOVE_PROTECT, gimmick: GIMMICK_DYNAMAX); }
        } else {
            TURN { MOVE(playerLeft, move, target: opponentLeft, gimmick: GIMMICK_DYNAMAX);
                   MOVE(opponentLeft, MOVE_PSYCHIC, target: playerLeft, gimmick: GIMMICK_DYNAMAX); }
        }
    } SCENE {
        if (protected) {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_GUARD, opponentLeft);
        }
        ANIMATION(ANIM_TYPE_MOVE, gMaxMove, playerLeft);
        HP_BAR(opponentLeft, captureDamage: &results[i].damage);
        NOT MESSAGE("Foe Wobbuffet couldn't fully protect itself and got hurt!");
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Dyna: Max Moves don't bypass absorbing abilities")
{
    u32 species, ability;
    
    PARAMETRIZE { species = SPECIES_LANTURN; ability = ABILITY_VOLT_ABSORB; }
    PARAMETRIZE { species = SPECIES_PIKACHU; ability = ABILITY_LIGHTNING_ROD; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_SPARK].type == TYPE_ELECTRIC);
        
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(species) { Ability(ability); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPARK, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_LIGHTNING, player);
        ABILITY_POPUP(opponent, ability);
    }
}

SINGLE_BATTLE_TEST("Dyna: Max Moves turns into the appropriate type if the user has Normalize, etc")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_TACKLE].type != TYPE_ELECTRIC);
        
        PLAYER(SPECIES_GEODUDE_ALOLAN) { Ability(ABILITY_GALVANIZE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_LIGHTNING, player);
    }
}

SINGLE_BATTLE_TEST("Dyna: Max Moves affected by Normalize/etc doesn't get a power boost", s16 damage)
{
    u32 ability;
    
    PARAMETRIZE { ability = ABILITY_STURDY; }
    PARAMETRIZE { ability = ABILITY_GALVANIZE; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_TACKLE].type != TYPE_ELECTRIC);

        PLAYER(SPECIES_GEODUDE_ALOLAN) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, UQ_4_12(1.5), results[1].damage); // STAB only
    }
}

SINGLE_BATTLE_TEST("Dyna: Max Moves turns into the appropriate type if used by a move that changes type")
{
    GIVEN {
        ASSUME(gBattleMoves[MOVE_WEATHER_BALL].effect == EFFECT_WEATHER_BALL);
        
        PLAYER(SPECIES_TORKOAL) { Ability(ABILITY_DROUGHT); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_WEATHER_BALL, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_DROUGHT);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_FLARE, player);
    }
}

// Almost anything that calculates damage based on HP has been changed to non-Dynamax HP and Max HP.
// This includes Leftovers, Life Orb, Heal Pulse, Rocky Helmet, Sandstorm, etc. etc.
// There are some redundant cases (i.e Substitute) that can never be used by a Dynamaxed Pokémon.
// Below are some tests, but very far from all encompassing:
SINGLE_BATTLE_TEST("Dyna: Endeavor uses a Pokemon's non-Dynamax HP", s16 damage)
{
    u32 gimmick;
    
    PARAMETRIZE { gimmick = GIMMICK_NONE; }
    PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_ENDEAVOR].effect == EFFECT_ENDEAVOR);
        
        PLAYER(SPECIES_WOBBUFFET) { Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(100); HP(1); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_ENDEAVOR); MOVE(player, MOVE_SCRATCH, gimmick: gimmick); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ENDEAVOR, opponent);
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Dyna: Heal Pulse heals based on a Pokemon's non-Dynamax Max HP", s16 damage)
{
    u32 gimmick;
    
    PARAMETRIZE { gimmick = GIMMICK_NONE; }
    PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; }
    
    GIVEN {
        ASSUME(gBattleMoves[MOVE_HEAL_PULSE].effect == EFFECT_HEAL_PULSE);
        
        PLAYER(SPECIES_WOBBUFFET) { HP(1); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(100); Speed(100); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_HEAL_PULSE); MOVE(player, MOVE_SCRATCH, gimmick: gimmick); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HEAL_PULSE, opponent);
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_EQ(results[0].damage, results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Dyna: Zacian/Zamazenta/Eternatus can't Dynamax")
{
    u32 species;
    
    PARAMETRIZE { species = SPECIES_ZACIAN; }
    PARAMETRIZE { species = SPECIES_ZAMAZENTA; }
    PARAMETRIZE { species = SPECIES_ETERNATUS; }

    GIVEN {
        PLAYER(species);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, gimmick: GIMMICK_DYNAMAX); }
    } SCENE {
        NOT MESSAGE("%s grew huge into its Dynamax form!", species);
    }
}






TO_DO_BATTLE_TEST("Dyna: Substitute doesn't prevents the secondary effects of Max Moves");
TO_DO_BATTLE_TEST("Dyna: Raid boss can always use Belch");

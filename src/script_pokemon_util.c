#include "global.h"
#include "gflib.h"
#include "daycare.h"
#include "event_data.h"
#include "load_save.h"
#include "overworld.h"
#include "item.h"
#include "form_change.h"
#include "party_menu.h"
#include "pokedex.h"
#include "script_pokemon_util.h"
#include "constants/battle.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/daycare.h"
#include "constants/moves.h"

void HealPlayerParty(void)
{
    u32 i, status = STATUS1_NONE;

    // restore HP.
    for (i = 0; i < gPlayerPartyCount; i++)
    {
        u32 maxHP = GetMonData(&gPlayerParty[i], MON_DATA_MAX_HP);
        
        SetMonData(&gPlayerParty[i], MON_DATA_HP, &maxHP);
        MonRestorePP(&gPlayerParty[i]);
        SetMonData(&gPlayerParty[i], MON_DATA_STATUS_ID, &status);
    }
    
    // Recharge Tera Orb, if possible.
    if (CheckBagHasItem(ITEM_TERA_ORB, 1))
        FlagSet(FLAG_TERA_ORB_CHARGED);
}

u32 ScriptGiveMon(u32 species, u32 level, u32 item, u8 *ivs, u32 pokeBall, u32 shinyType, bool32 hiddenAbility, u32 nature, u32 gender, u16 *moves)
{
    u32 i, sentToPc;
    struct Pokemon *mon = AllocZeroed(sizeof(struct Pokemon));
    struct PokemonGenerator generator =
    {
        .species = species,
        .level = level,
        .forcedGender = gender,
        .otIdType = OT_ID_PLAYER_ID,
        .hasFixedPersonality = FALSE,
        .fixedPersonality = 0,
        .shinyType = shinyType,
        .shinyRollType = SHINY_ROLL_NORMAL,
        .forcedNature = nature,
        .formChanges = NULL,
        .nPerfectIvs = 0,
    };
    memcpy(generator.moves, moves, sizeof(generator.moves));
    
    CreateMon(mon, generator);
    
    SetMonData(mon, MON_DATA_HELD_ITEM, &item);
    
    if (pokeBall)
    {
        pokeBall = ITEM_TO_BALL(pokeBall);
        SetMonData(mon, MON_DATA_POKEBALL, &pokeBall);
    }
    
    if (hiddenAbility)
        SetMonData(mon, MON_DATA_ABILITY_HIDDEN, &hiddenAbility);
    
    for (i = 0; i < NUM_STATS; i++)
    {
        if (ivs[i] != USE_RANDOM_IVS)
            SetMonData(mon, MON_DATA_HP_IV + i, &ivs[i]);
    }
    CalculateMonStats(mon);
    
    sentToPc = GiveScriptCreatedMonToPlayer(mon, TRUE);
    Free(mon);
    return sentToPc;
}

u32 ScriptGiveEgg(u32 species, u8 *ivs, u32 shinyType, bool32 hiddenAbility, u32 nature, u16 *moves)
{
    u32 i, sentToPc;
    struct Pokemon *mon = AllocZeroed(sizeof(struct Pokemon));
    struct PokemonGenerator generator =
    {
        .species = species,
        .level = EGG_HATCH_LEVEL,
        .otIdType = OT_ID_PLAYER_ID,
        .shinyType = shinyType,
        .shinyRollType = SHINY_ROLL_NORMAL,
        .forcedGender = MON_GENDERLESS,
        .hasFixedPersonality = FALSE,
        .fixedPersonality = 0,
        .forcedNature = nature,
        .formChanges = NULL,
        .nPerfectIvs = 0,
    };
    memcpy(generator.moves, moves, sizeof(generator.moves));
    
    CreateEgg(mon, generator);
    
    if (hiddenAbility)
        SetMonData(mon, MON_DATA_ABILITY_HIDDEN, &hiddenAbility);
    
    for (i = 0; i < NUM_STATS; i++)
    {
        if (ivs[i] != USE_RANDOM_IVS)
            SetMonData(mon, MON_DATA_HP_IV + i, &ivs[i]);
    }
    CalculateMonStats(mon);
    
    sentToPc = GiveScriptCreatedMonToPlayer(mon, FALSE);
    Free(mon);
    return sentToPc;
}

u32 GiveScriptCreatedMonToPlayer(struct Pokemon *mon, bool32 setPokedexFlags)
{
    u32 nationalDexNum, sentToPc = GiveMonToPlayer(mon);
    
    if (setPokedexFlags)
    {
        nationalDexNum = SpeciesToNationalPokedexNum(GetMonData(mon, MON_DATA_SPECIES));
        
        switch(sentToPc)
        {
        case MON_GIVEN_TO_PARTY:
        case MON_GIVEN_TO_PC:
            GetSetPokedexFlag(nationalDexNum, FLAG_SET_SEEN);
            GetSetPokedexFlag(nationalDexNum, FLAG_SET_CAUGHT);
            break;
        }
    }
    return sentToPc;
}

void HasEnoughMonsForDoubleBattle(void)
{
    gSpecialVar_Result = GetMonsStateToDoubles();
}

void CreateScriptedWildMon(u32 species, u32 level, u32 item, u32 species2, u32 level2, u32 item2)
{
    struct PokemonGenerator generator =
    {
        .species = species,
        .level = level,
        .forcedGender = MON_GENDERLESS,
        .otIdType = OT_ID_PLAYER_ID,
        .hasFixedPersonality = FALSE,
        .fixedPersonality = 0,
        .shinyType = GENERATE_SHINY_NORMAL,
        .shinyRollType = SHINY_ROLL_NORMAL,
        .forcedNature = NUM_NATURES,
        .formChanges = NULL,
        .moves = {0},
        .nPerfectIvs = 0,
    };
    ZeroEnemyPartyMons();
    
    CreateMon(&gEnemyParty[0], generator);

    SetMonData(&gEnemyParty[0], MON_DATA_HELD_ITEM, &item);
    
    if (species2)
    {
        generator.species = species2;
        generator.level = level2;
        
        CreateMon(&gEnemyParty[1], generator);
        SetMonData(&gEnemyParty[1], MON_DATA_HELD_ITEM, &item2);
    }
}

void ScriptSetMonMoveSlot(u32 monIndex, u32 move, u32 slot)
{
    if (monIndex > PARTY_SIZE)
        monIndex = gPlayerPartyCount - 1;

    SetMonMoveSlot(&gPlayerParty[monIndex], move, slot);
}

static void CB2_ReturnFromChooseHalfParty(void)
{
    switch (gSelectedOrderFromParty[0])
    {
    case 0:
        gSpecialVar_Result = FALSE;
        break;
    default:
        gSpecialVar_Result = TRUE;
        break;
    }
    SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

// Note: When control returns to the event script, gSpecialVar_Result will be
// TRUE if the party selection was successful.
void ChooseHalfPartyForBattle(void)
{
    gMain.savedCallback = CB2_ReturnFromChooseHalfParty;
//    VarSet(VAR_FRONTIER_FACILITY, FACILITY_MULTI_OR_EREADER);
    InitChooseHalfPartyForBattle(CHOOSE_MONS_FOR_CABLE_CLUB_BATTLE);
}

void ReducePlayerPartyToThree(void)
{
    u32 i;
    struct Pokemon * party = AllocZeroed((PARTY_SIZE / 2) * sizeof(struct Pokemon));

    // copy the selected pokemon according to the order.
    for (i = 0; i < (PARTY_SIZE / 2); i++)
    {
        if (gSelectedOrderFromParty[i]) // as long as the order keeps going (did the player select 1 mon? 2? 3?), do not stop
            party[i] = gPlayerParty[gSelectedOrderFromParty[i] - 1]; // index is 0 based, not literal
    }
    CpuFill32(0, gPlayerParty, sizeof(gPlayerParty));

    // overwrite the first 3 with the order copied to.
    for (i = 0; i < (PARTY_SIZE / 2); i++)
        gPlayerParty[i] = party[i];

    CalculatePlayerPartyCount();
    Free(party);
}

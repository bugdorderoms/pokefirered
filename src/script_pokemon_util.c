#include "global.h"
#include "gflib.h"
#include "daycare.h"
#include "event_data.h"
#include "load_save.h"
#include "overworld.h"
#include "form_change.h"
#include "party_menu.h"
#include "pokedex.h"
#include "script_pokemon_util.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/daycare.h"
#include "constants/moves.h"

static void CB2_ReturnFromChooseHalfParty(void);

void HealPlayerParty(void)
{
    u8 i, arg[4];

    // restore HP.
    for(i = 0; i < gPlayerPartyCount; i++)
    {
        u16 maxHP = GetMonData(&gPlayerParty[i], MON_DATA_MAX_HP);
        arg[0] = maxHP;
        arg[1] = maxHP >> 8;
        SetMonData(&gPlayerParty[i], MON_DATA_HP, arg);
		
		// restore PP.
		MonRestorePP(&gPlayerParty[i]);
		
        // since status is u32, the four 0 assignments here are probably for safety to prevent undefined data from reaching SetMonData.
        arg[0] = 0;
        arg[1] = 0;
        arg[2] = 0;
        arg[3] = 0;
        SetMonData(&gPlayerParty[i], MON_DATA_STATUS, arg);
    }
}

u8 ScriptGiveMon(u16 species, u8 level, u16 item, u8 *ivs, u16 pokeBall, u8 shinyType, bool8 hiddenAbility, u8 nature, u8 gender, u16 moves[])
{
    u16 nationalDexNum;
    int sentToPc;
    u8 i, heldItem[2];
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
		.forcedNature = nature,
		.formChanges = NULL,
		.moves = {moves[0], moves[1], moves[2], moves[3]},
	};
    CreateMon(mon, generator);
    
    heldItem[0] = item;
    heldItem[1] = item >> 8;
    SetMonData(mon, MON_DATA_HELD_ITEM, heldItem);
    
	if (pokeBall)
	{
		heldItem[0] = pokeBall;
		heldItem[1] = pokeBall >> 8;
		SetMonData(mon, MON_DATA_POKEBALL, heldItem);
	}
    
	if (hiddenAbility)
		SetMonData(mon, MON_DATA_ABILITY_HIDDEN, &hiddenAbility);
	
    for (i = 0; i < NUM_STATS; i++)
    {
        if (ivs[i] != USE_RANDOM_IVS)
            SetMonData(mon, MON_DATA_HP_IV + i, &ivs[i]);
    }
	CalculateMonStats(mon);
	
    sentToPc = GiveMonToPlayer(mon);
    nationalDexNum = SpeciesToNationalPokedexNum(GetMonData(mon, MON_DATA_SPECIES));

    switch(sentToPc)
    {
    case MON_GIVEN_TO_PARTY:
    case MON_GIVEN_TO_PC:
        GetSetPokedexFlag(nationalDexNum, FLAG_SET_SEEN);
        GetSetPokedexFlag(nationalDexNum, FLAG_SET_CAUGHT);
        break;
    }

    Free(mon);
    return sentToPc;
}

u8 ScriptGiveEgg(u16 species, u8 *ivs, u8 shinyType, bool8 hiddenAbility, u8 nature, u16 moves[])
{
	u8 i;
	bool8 sentToPc;
    struct Pokemon *mon = AllocZeroed(sizeof(struct Pokemon));
	struct PokemonGenerator generator =
	{
		.species = species,
		.level = EGG_HATCH_LEVEL,
		.otIdType = OT_ID_PLAYER_ID,
		.shinyType = shinyType,
		.forcedGender = MON_GENDERLESS,
		.hasFixedPersonality = FALSE,
		.fixedPersonality = 0,
		.forcedNature = nature,
		.formChanges = NULL,
		.moves = {moves[0], moves[1], moves[2], moves[3]},
	};
    CreateEgg(mon, generator);
	
	if (hiddenAbility)
		SetMonData(mon, MON_DATA_ABILITY_HIDDEN, &hiddenAbility);
	
	for (i = 0; i < NUM_STATS; i++)
    {
        if (ivs[i] != USE_RANDOM_IVS)
            SetMonData(mon, MON_DATA_HP_IV + i, &ivs[i]);
    }
	CalculateMonStats(mon);
	
    sentToPc = GiveMonToPlayer(mon);
    Free(mon);
    return sentToPc;
}

void HasEnoughMonsForDoubleBattle(void)
{
    switch (GetMonsStateToDoubles())
    {
    case PLAYER_HAS_TWO_USABLE_MONS:
        gSpecialVar_Result = PLAYER_HAS_TWO_USABLE_MONS;
        break;
    case PLAYER_HAS_ONE_MON:
        gSpecialVar_Result = PLAYER_HAS_ONE_MON;
        break;
    case PLAYER_HAS_ONE_USABLE_MON:
        gSpecialVar_Result = PLAYER_HAS_ONE_USABLE_MON;
        break;
    }
}

static bool8 CheckPartyMonHasHeldItem(u16 item)
{
    int i;

    for(i = 0; i < PARTY_SIZE; i++)
    {
        if (IsMonValidSpecies(&gPlayerParty[i]) && GetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM) == item)
            return TRUE;
    }
    return FALSE;
}

void CreateScriptedWildMon(u16 species, u8 level, u16 item, u16 species2, u8 level2, u16 item2)
{
    u8 heldItem[2];
	struct PokemonGenerator generator =
	{
		.species = species,
		.level = level,
		.forcedGender = MON_GENDERLESS,
		.otIdType = OT_ID_PLAYER_ID,
		.hasFixedPersonality = FALSE,
		.fixedPersonality = 0,
		.shinyType = GENERATE_SHINY_NORMAL,
		.forcedNature = NUM_NATURES,
		.formChanges = NULL,
		.moves = {MOVE_NONE, MOVE_NONE, MOVE_NONE, MOVE_NONE},
	};
    ZeroEnemyPartyMons();
	
    CreateMon(&gEnemyParty[0], generator);
	heldItem[0] = item;
	heldItem[1] = item >> 8;
	SetMonData(&gEnemyParty[0], MON_DATA_HELD_ITEM, heldItem);
	
	if (species2)
	{
		generator.species = species2;
		generator.level = level2;
		
		CreateMon(&gEnemyParty[1], generator);
		heldItem[0] = item2;
		heldItem[1] = item2 >> 8;
		SetMonData(&gEnemyParty[1], MON_DATA_HELD_ITEM, heldItem);
	}
}

void ScriptSetMonMoveSlot(u8 monIndex, u16 move, u8 slot)
{
    if (monIndex > PARTY_SIZE)
        monIndex = gPlayerPartyCount - 1;

    SetMonMoveSlot(&gPlayerParty[monIndex], move, slot);
}

// Note: When control returns to the event script, gSpecialVar_Result will be
// TRUE if the party selection was successful.
void ChooseHalfPartyForBattle(void)
{
    gMain.savedCallback = CB2_ReturnFromChooseHalfParty;
//    VarSet(VAR_FRONTIER_FACILITY, FACILITY_MULTI_OR_EREADER);
    InitChooseHalfPartyForBattle(CHOOSE_MONS_FOR_CABLE_CLUB_BATTLE);
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

void ReducePlayerPartyToThree(void)
{
    struct Pokemon * party = AllocZeroed(3 * sizeof(struct Pokemon));
    int i;

    // copy the selected pokemon according to the order.
    for (i = 0; i < 3; i++)
	{
        if (gSelectedOrderFromParty[i]) // as long as the order keeps going (did the player select 1 mon? 2? 3?), do not stop
            party[i] = gPlayerParty[gSelectedOrderFromParty[i] - 1]; // index is 0 based, not literal
	}
    CpuFill32(0, gPlayerParty, sizeof(gPlayerParty));

    // overwrite the first 3 with the order copied to.
    for (i = 0; i < 3; i++)
        gPlayerParty[i] = party[i];

    CalculatePlayerPartyCount();
    Free(party);
}

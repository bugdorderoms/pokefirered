#include "global.h"
#include "gflib.h"
#include "strings.h"
#include "task.h"
#include "field_message_box.h"
#include "script.h"
#include "event_data.h"
#include "fldeff.h"
#include "party_menu.h"
#include "field_poison.h"
#include "constants/battle.h"

static bool32 AllMonsFainted(void)
{
    int i;

    for (i = 0; i < PARTY_SIZE; i++)
	{
        if (MonCanBattle(&gPlayerParty[i]))
            return FALSE;
	}
    return TRUE;
}

static void FaintFromFieldPoison(u8 partyIdx)
{
    struct Pokemon *pokemon = &gPlayerParty[partyIdx];
    u32 status;
	
#if POISON_SURVIVAL == FALSE
    AdjustFriendship(pokemon, FRIENDSHIP_EVENT_FAINT_OUTSIDE_BATTLE);
#endif

	status = STATUS1_NONE;
    SetMonData(pokemon, MON_DATA_STATUS, &status);
    GetMonData(pokemon, MON_DATA_NICKNAME, gStringVar1);
    StringGet_Nickname(gStringVar1);
}

static bool32 MonFaintedFromPoison(u8 partyIdx)
{
    struct Pokemon *pokemon = &gPlayerParty[partyIdx];
	
#if POISON_SURVIVAL
    if (IsMonValidSpecies(pokemon) && GetMonData(pokemon, MON_DATA_HP) == 1 && GetAilmentFromStatus(GetMonData(pokemon, MON_DATA_STATUS)) == AILMENT_PSN)
        return TRUE;
#else
    if (IsMonValidSpecies(pokemon) && !GetMonData(pokemon, MON_DATA_HP) && GetAilmentFromStatus(GetMonData(pokemon, MON_DATA_STATUS)) == AILMENT_PSN)
        return TRUE;
#endif

    return FALSE;
}

#define tState   data[0]
#define tPartyId data[1]

static void Task_TryFieldPoisonWhiteOut(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    switch (tState)
    {
    case 0:
        for (; tPartyId < PARTY_SIZE; tPartyId++)
        {
            if (MonFaintedFromPoison(tPartyId))
            {
                FaintFromFieldPoison(tPartyId);
				
#if POISON_SURVIVAL
                ShowFieldMessage(gText_PkmnPoisonSurvived);
#else
                ShowFieldMessage(gText_PkmnFainted3);
#endif

                data[0]++;
                return;
            }
        }
        tState = 2;
        break;
    case 1:
        if (IsFieldMessageBoxHidden())
            tState--;
        break;
    case 2:
        gSpecialVar_Result = AllMonsFainted();
        EnableBothScriptContexts();
        DestroyTask(taskId);
        break;
    }
}

void TryFieldPoisonWhiteOut(void)
{
    CreateTask(Task_TryFieldPoisonWhiteOut, 80);
    ScriptContext1_Stop();
}

s32 DoPoisonFieldEffect(void)
{
    int i;
    u32 hp, numPoisoned, numFainted;
    struct Pokemon *pokemon;

    for (i = 0, numPoisoned = 0, numFainted = 0; i < PARTY_SIZE; i++)
    {
		pokemon = &gPlayerParty[i];
		
        if (GetMonData(pokemon, MON_DATA_SANITY_HAS_SPECIES) && GetAilmentFromStatus(GetMonData(pokemon, MON_DATA_STATUS)) == AILMENT_PSN)
        {
            hp = GetMonData(pokemon, MON_DATA_HP);
			
#if POISON_SURVIVAL
            if (hp == 1 || --hp == 1)
#else
            if (hp == 0 || --hp == 0)
#endif			
				numFainted++;
			
            SetMonData(pokemon, MON_DATA_HP, &hp);
            numPoisoned++;
        }
    }
    if (numFainted || numPoisoned)
        FldEffPoison_Start();
    if (numFainted)
        return FLDPSN_FNT;
    if (numPoisoned)
        return FLDPSN_PSN;
    return FLDPSN_NONE;
}

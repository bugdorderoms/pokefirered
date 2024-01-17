#include "global.h"
#include "gflib.h"
#include "data.h"
#include "script.h"
#include "overworld.h"
#include "party_menu.h"
#include "field_fadetransition.h"
#include "pokemon_summary_screen.h"
#include "event_data.h"
#include "constants/moves.h"

static void Task_WaitFadeAndInitChoosePartyPokemon(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        gPaletteFade.bufferTransferDisabled = TRUE;
        ChoosePartyMonByMenuType((u8)gTasks[taskId].data[0]);
        DestroyTask(taskId);
    }
}

void ChoosePartyMon(void)
{
    ScriptContext2_Enable();
    gTasks[CreateTask(Task_WaitFadeAndInitChoosePartyPokemon, 10)].data[0] = PARTY_MENU_TYPE_CHOOSE_MON;
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
}

void SelectMoveTutorMon(void)
{
    ScriptContext2_Enable();
    gTasks[CreateTask(Task_WaitFadeAndInitChoosePartyPokemon, 10)].data[0] = PARTY_MENU_TYPE_MOVE_RELEARNER;
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
}

void SelectMoveDeleterMove(void)
{
    ShowSelectMovePokemonSummaryScreen(gPlayerParty, gSpecialVar_0x8004, gPlayerPartyCount - 1, CB2_ReturnToField, 0);
    SetPokemonSummaryScreenMode(PSS_MODE_FORGET_MOVE);
    gFieldCallback = FieldCB_ContinueScriptHandleMusic;
}

void GetNumMovesSelectedMonHas(void)
{
    u8 i;

    gSpecialVar_Result = 0;
	
    for (i = 0; i < MAX_MON_MOVES; ++i)
	{
        if (GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_MOVE1 + i))
            ++gSpecialVar_Result;
	}
}

void BufferMoveDeleterNicknameAndMove(void)
{
    struct Pokemon *mon = &gPlayerParty[gSpecialVar_0x8004];

    GetMonNickname(mon, gStringVar1);
    StringCopy(gStringVar2, gBattleMoves[GetMonData(mon, MON_DATA_MOVE1 + gSpecialVar_0x8005)].name);
}

void MoveDeleterForgetMove(void)
{
    DeleteMonMove(&gPlayerParty[gSpecialVar_0x8004], gSpecialVar_0x8005);
}

void IsSelectedMonEgg(void)
{
    if (GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_IS_EGG))
        gSpecialVar_Result = TRUE;
    else
        gSpecialVar_Result = FALSE;
}

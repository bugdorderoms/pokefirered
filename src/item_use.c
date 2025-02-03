#include "global.h"
#include "gflib.h"
#include "battle.h"
#include "battle_anim.h"
#include "berry_pouch.h"
#include "bike.h"
#include "coins.h"
#include "util.h"
#include "event_data.h"
#include "event_scripts.h"
#include "ev_iv_display.h"
#include "field_effect.h"
#include "field_fadetransition.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "field_specials.h"
#include "field_weather.h"
#include "fieldmap.h"
#include "item.h"
#include "ride_pager.h"
#include "script_menu.h"
#include "item_menu.h"
#include "item_use.h"
#include "itemfinder.h"
#include "mail.h"
#include "fldeff.h"
#include "event_object_lock.h"
#include "metatile_behavior.h"
#include "new_menu_helpers.h"
#include "overworld.h"
#include "party_menu.h"
#include "region_map.h"
#include "script.h"
#include "strings.h"
#include "task.h"
#include "teachy_tv.h"
#include "tm_case.h"
#include "vs_seeker.h"
#include "constants/sound.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/songs.h"
#include "constants/sound.h"
#include "constants/field_weather.h"

static EWRAM_DATA void (*sItemUseOnFieldCB)(u8 taskId) = NULL;

static bool8 FieldCB2_UseItemFromField(void);
static void ItemUseOnFieldCB_Bicycle(u8 taskId);
static void ItemUseOnFieldCB_Honey(u8 taskId);
static void Task_PlayPokeFlute(u8 taskId);
static void Task_UseRepel(u8 taskId);
static void RemoveUsedItem(void);
static void Task_UsedBlackWhiteFlute(u8 taskId);
static void FieldUseFunc_HoneyCB(u8 taskId);

static void (*const sExitCallbackByItemType[])(void) = {
    CB2_ShowPartyMenuForItemUse,
    CB2_ReturnToField,
    NULL,
	CB2_ShowPartyMenuForItemUse
};

static void Task_FadeOuFromBackToField(u8 taskId)
{
    u8 itemType = ItemId_GetType(gSpecialVar_ItemId) - 1;

    if (ItemId_GetPocket(gSpecialVar_ItemId) == POCKET_BERRY_POUCH)
    {
        BerryPouch_SetExitCallback(sExitCallbackByItemType[itemType]);
        BerryPouch_StartFadeToExitCallback(taskId);
    }
    else
    {
        ItemMenu_SetExitCallback(sExitCallbackByItemType[itemType]);
		
        if (itemType == ITEM_TYPE_FIELD - 1)
            Bag_BeginCloseWin0Animation();
		
        ItemMenu_StartFadeToExitCallback(taskId);
    }
}

static void Task_WaitFadeIn_CallItemUseOnFieldCB(u8 taskId)
{
    if (IsWeatherNotFadingIn())
        sItemUseOnFieldCB(taskId);
}

static void FieldCB_FadeInFromBlack(void)
{
    FadeInFromBlack();
    CreateTask(Task_WaitFadeIn_CallItemUseOnFieldCB, 8);
}

static void SetUpItemUseOnFieldCallback(u8 taskId)
{
    if (gTasks[taskId].data[3] != 1) // Go from bag to the field
    {
        gFieldCallback = FieldCB_FadeInFromBlack;
        Task_FadeOuFromBackToField(taskId);
    }
    else // Called from the SELECT button
        sItemUseOnFieldCB(taskId);
}

static void DisplayItemMessageInCurrentContext(u8 taskId, bool8 inField, u8 fontId, const u8 * str)
{
    StringExpandPlaceholders(gStringVar4, str);
	
    if (!inField)
        DisplayItemMessageInBag(taskId, fontId, gStringVar4, Task_ReturnToBagFromContextMenu);
    else
        DisplayItemMessageOnField(taskId, fontId, gStringVar4, Task_ItemUse_CloseMessageBoxAndReturnToField);
}

static void PrintNotTheTimeToUseThat(u8 taskId, bool8 inField)
{
    DisplayItemMessageInCurrentContext(taskId, inField, 4, gText_OakForbidsUseOfItemHere);
}

void Task_ItemUse_CloseMessageBoxAndReturnToField(u8 taskId)
{
    ClearDialogWindowAndFrame(0, 1);
    DestroyTask(taskId);
    ClearPlayerHeldMovementAndUnfreezeObjectEvents();
    ScriptContext2_Disable();
}

static void RemoveUsedItem(void)
{
    RemoveBagItem(gSpecialVar_ItemId, 1);
    Pocket_CalculateNItemsAndMaxShowed(ItemId_GetPocket(gSpecialVar_ItemId));
    PocketCalculateInitialCursorPosAndItemsAbove(ItemId_GetPocket(gSpecialVar_ItemId));
    CopyItemName(gSpecialVar_ItemId, gStringVar2);
    StringExpandPlaceholders(gStringVar4, gUnknown_841658C);
}

static inline void SetFieldCallback2ForItemUse(void)
{
    gFieldCallback2 = FieldCB2_UseItemFromField;
}

static void Task_ItemUseWaitForFade(u8 taskId)
{
    if (IsWeatherNotFadingIn())
    {
        UnfreezeObjectEvents();
        ScriptContext2_Disable();
        DestroyTask(taskId);
    }
}

static bool8 FieldCB2_UseItemFromField(void)
{
    FreezeObjectEvents();
    ScriptContext2_Enable();
    FadeInFromBlack();
    CreateTask(Task_ItemUseWaitForFade, 10);
    gUnknown_2031DE0 = 0;
    return TRUE;
}

static void CB2_CheckMail(void)
{
    struct Mail mail;
    mail.itemId = gSpecialVar_ItemId;
    ReadMail(&mail, CB2_BagMenuFromStartMenu, 0);
}

void FieldUseFunc_OrangeMail(u8 taskId)
{
    ItemMenu_SetExitCallback(CB2_CheckMail);
    ItemMenu_StartFadeToExitCallback(taskId);
}

void FieldUseFunc_MachBike(u8 taskId)
{
    s16 x, y;
    u8 behavior;

    PlayerGetDestCoords(&x, &y);
    behavior = MapGridGetMetatileBehaviorAt(x, y);

    if (FlagGet(FLAG_SYS_ON_CYCLING_ROAD) || MetatileBehavior_IsVerticalRail(behavior) || MetatileBehavior_IsHorizontalRail(behavior)
     || MetatileBehavior_IsIsolatedVerticalRail(behavior) || MetatileBehavior_IsIsolatedHorizontalRail(behavior) || MetatileBehavior_IsGroundRocks(behavior))
        DisplayItemMessageInCurrentContext(taskId, gTasks[taskId].data[3], 2, gUnknown_8416451);
    else if (Overworld_IsBikingAllowed() == TRUE && !IsBikingDisallowedByPlayer())
    {
        sItemUseOnFieldCB = ItemUseOnFieldCB_Bicycle;
        SetUpItemUseOnFieldCallback(taskId);
    }
    else
        PrintNotTheTimeToUseThat(taskId, gTasks[taskId].data[3]);
}

static void ItemUseOnFieldCB_Bicycle(u8 taskId)
{
    if (!TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE | PLAYER_AVATAR_FLAG_ACRO_BIKE))
        PlaySE(SE_BIKE_BELL);
	
    GetOnOffBike(PLAYER_AVATAR_FLAG_MACH_BIKE | PLAYER_AVATAR_FLAG_ACRO_BIKE);
    ClearPlayerHeldMovementAndUnfreezeObjectEvents();
    ScriptContext2_Disable();
    DestroyTask(taskId);
}

static bool8 ItemUseCheckFunc_Rod(void)
{
    s16 x, y;
    u16 behavior;

    GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
    behavior = MapGridGetMetatileBehaviorAt(x, y);

    if (MetatileBehavior_IsWaterfall(behavior) || CheckPlayerInGroundRocks() || TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_UNDERWATER))
        return FALSE;

    if (!TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
    {
        if (IsPlayerFacingSurfableFishableWater())
            return TRUE;
    }
    else
    {
        if ((MetatileBehavior_IsSurfable(behavior) && !MapGridIsImpassableAt(x, y)) || MetatileBehavior_IsBridge(behavior))
            return TRUE;
    }
    return FALSE;
}

static void ItemUseOnFieldCB_Rod(u8 taskId)
{
    StartFishing(ItemId_GetHoldEffectParam(gSpecialVar_ItemId));
    DestroyTask(taskId);
}

void FieldUseFunc_OldRod(u8 taskId)
{
    if (ItemUseCheckFunc_Rod())
    {
        sItemUseOnFieldCB = ItemUseOnFieldCB_Rod;
        SetUpItemUseOnFieldCallback(taskId);
    }
    else
        PrintNotTheTimeToUseThat(taskId, gTasks[taskId].data[3]);
}

void ItemUseOutOfBattle_Itemfinder(u8 taskId)
{
    IncrementGameStat(GAME_STAT_USED_ITEMFINDER);
    sItemUseOnFieldCB = ItemUseOnFieldCB_Itemfinder;
    SetUpItemUseOnFieldCallback(taskId);
}

void FieldUseFunc_CoinCase(u8 taskId)
{
    ConvertIntToDecimalStringN(gStringVar1, GetCoins(), STR_CONV_MODE_LEFT_ALIGN, 4);
    DisplayItemMessageInCurrentContext(taskId, gTasks[taskId].data[3], 2, gUnknown_8416537);
}

void FieldUseFunc_PokeFlute(u8 taskId)
{
    bool8 wokeSomeoneUp = FALSE;
    u8 i, partyCount;
	
	RemoveBagItem(gSpecialVar_ItemId, 1);
	
	partyCount = CalculatePlayerPartyCount();
	
    for (i = 0; i < partyCount; i++)
    {
        if (!HealStatusConditions(&gPlayerParty[i], STATUS1_SLEEP, MAX_BATTLERS_COUNT))
            wokeSomeoneUp = TRUE;
    }
	
    if (wokeSomeoneUp)
    {
        if (!gTasks[taskId].data[3])
            DisplayItemMessageInBag(taskId, 2, gUnknown_8416690, Task_PlayPokeFlute);
        else
            DisplayItemMessageOnField(taskId, 2, gUnknown_8416690, Task_PlayPokeFlute);
    }
    else // Now that's a catchy tune!
        DisplayItemMessageInCurrentContext(taskId, gTasks[taskId].data[3], 2, gUnknown_841665C);
}

static void Task_DisplayPokeFluteMessage(u8 taskId)
{
    if (WaitFanfare(FALSE))
        DisplayItemMessageInCurrentContext(taskId, gTasks[taskId].data[3], 2, gUnknown_84166A7);
}

static void Task_PlayPokeFlute(u8 taskId)
{
    PlayFanfareByFanfareNum(FANFARE_POKEFLUTE);
    gTasks[taskId].func = Task_DisplayPokeFluteMessage;
}

void FieldUseFunc_Medicine(u8 taskId)
{
    gItemUseCB = ItemUseCB_Medicine;
    Task_FadeOuFromBackToField(taskId);
}

void FieldUseFunc_Ether(u8 taskId)
{
    gItemUseCB = ItemUseCB_PPRecoveryOneMove;
    Task_FadeOuFromBackToField(taskId);
}

void FieldUseFunc_Elixir(u8 taskId)
{
	gItemUseCB = ItemUseCB_PPRecoveryAllMoves;
	Task_FadeOuFromBackToField(taskId);
}

void FieldUseFunc_PpUp(u8 taskId)
{
    gItemUseCB = ItemUseCB_PPUp;
    Task_FadeOuFromBackToField(taskId);
}

void FieldUseFunc_EvoItem(u8 taskId)
{
    gItemUseCB = ItemUseCB_EvolutionStone;
    Task_FadeOuFromBackToField(taskId);
}

void FieldUseFunc_SacredAsh(u8 taskId)
{
    gItemUseCB = ItemUseCB_SacredAsh;
    Task_FadeOuFromBackToField(taskId);
}

static void InitTMCaseFromBag(void)
{
    InitTMCase(0, CB2_BagMenuFromStartMenu, 0);
}

static void Task_InitTMCaseFromField(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        SetFieldCallback2ForItemUse();
        InitTMCase(0, CB2_ReturnToField, 1);
        DestroyTask(taskId);
    }
}

void FieldUseFunc_TmCase(u8 taskId)
{
    if (gTasks[taskId].data[3] == 0)
    {
        ItemMenu_SetExitCallback(InitTMCaseFromBag);
        ItemMenu_StartFadeToExitCallback(taskId);
    }
    else
    {
        StopPokemonLeagueLightingEffectTask();
        FadeScreen(FADE_TO_BLACK, 0);
        gTasks[taskId].func = Task_InitTMCaseFromField;
    }
}

static void InitBerryPouchFromBag(void)
{
    InitBerryPouch(BERRYPOUCH_FROMFIELD, CB2_BagMenuFromStartMenu, 0);
}

static void Task_InitBerryPouchFromField(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        SetFieldCallback2ForItemUse();
        InitBerryPouch(BERRYPOUCH_FROMFIELD, CB2_ReturnToField, 1);
        DestroyTask(taskId);
    }
}

void FieldUseFunc_BerryPouch(u8 taskId)
{
    if (gTasks[taskId].data[3] == 0)
    {
        ItemMenu_SetExitCallback(InitBerryPouchFromBag);
        ItemMenu_StartFadeToExitCallback(taskId);
    }
    else
    {
        StopPokemonLeagueLightingEffectTask();
        FadeScreen(FADE_TO_BLACK, 0);
        gTasks[taskId].func = Task_InitBerryPouchFromField;
    }
}

static void InitBerryPouchFromBattle(void)
{
    InitBerryPouch(BERRYPOUCH_FROMBATTLE, CB2_BagMenuFromBattle, 0);
}

void BattleUseFunc_BerryPouch(u8 taskId)
{
    ItemMenu_SetExitCallback(InitBerryPouchFromBattle);
    ItemMenu_StartFadeToExitCallback(taskId);
}

static void InitTeachyTvFromBag(void)
{
    InitTeachyTvController(0, CB2_BagMenuFromStartMenu);
}

static void Task_InitTeachyTvFromField(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        SetFieldCallback2ForItemUse();
        InitTeachyTvController(0, CB2_ReturnToField);
        DestroyTask(taskId);
    }
}

void FieldUseFunc_TeachyTv(u8 taskId)
{
    if (gTasks[taskId].data[3] == 0)
    {
        ItemMenu_SetExitCallback(InitTeachyTvFromBag);
        ItemMenu_StartFadeToExitCallback(taskId);
    }
    else
    {
        StopPokemonLeagueLightingEffectTask();
        FadeScreen(FADE_TO_BLACK, 0);
        gTasks[taskId].func = Task_InitTeachyTvFromField;
    }
}

void FieldUseFunc_SuperRepel(u8 taskId)
{
    if (VarGet(VAR_REPEL_STEP_COUNT) == 0)
    {
        PlaySE(SE_REPEL);
        gTasks[taskId].func = Task_UseRepel;
    }
    else // An earlier repel is still in effect
        DisplayItemMessageInBag(taskId, 2, gUnknown_841659E, Task_ReturnToBagFromContextMenu);
}

static void Task_UseRepel(u8 taskId)
{
    if (!IsSEPlaying())
    {
        VarSet(VAR_REPEL_STEP_COUNT, ItemId_GetHoldEffectParam(gSpecialVar_ItemId));
        RemoveUsedItem();
        DisplayItemMessageInBag(taskId, 2, gStringVar4, Task_ReturnToBagFromContextMenu);
    }
}

void FieldUseFunc_BlackFlute(u8 taskId)
{
	switch (gSpecialVar_ItemId)
	{
		case ITEM_WHITE_FLUTE:
		    FlagSet(FLAG_SYS_WHITE_FLUTE_ACTIVE);
			FlagClear(FLAG_SYS_BLACK_FLUTE_ACTIVE);
			StringExpandPlaceholders(gStringVar4, gUnknown_84165D2);
			break;
		case ITEM_BLACK_FLUTE:
		    FlagSet(FLAG_SYS_BLACK_FLUTE_ACTIVE);
			FlagClear(FLAG_SYS_WHITE_FLUTE_ACTIVE);
			StringExpandPlaceholders(gStringVar4, gUnknown_8416600);
			break;
	}
	CopyItemName(gSpecialVar_ItemId, gStringVar2);
	gTasks[taskId].func = Task_UsedBlackWhiteFlute;
	gTasks[taskId].data[8] = 0;
}

static void Task_UsedBlackWhiteFlute(u8 taskId)
{
    if (++gTasks[taskId].data[8] > 7)
    {
        PlaySE(SE_GLASS_FLUTE);
        DisplayItemMessageInBag(taskId, 2, gStringVar4, Task_ReturnToBagFromContextMenu);
    }
}

bool8 CanUseEscapeRopeOnCurrMap(void)
{
    if (gMapHeader.allowEscaping)
        return TRUE;
	return FALSE;
}

static void ItemUseOnFieldCB_EscapeRope(u8 taskId)
{
    Overworld_ResetStateForLeavingMap();
    RemoveUsedItem();
    gTasks[taskId].data[0] = 0;
    DisplayItemMessageOnField(taskId, 2, gStringVar4, sub_80A1C44);
}

void ItemUseOutOfBattle_EscapeRope(u8 taskId)
{
    if (CanUseEscapeRopeOnCurrMap())
    {
        sItemUseOnFieldCB = ItemUseOnFieldCB_EscapeRope;
        SetUpItemUseOnFieldCallback(taskId);
    }
    else
        PrintNotTheTimeToUseThat(taskId, gTasks[taskId].data[3]);
}

void sub_80A1C44(u8 taskId)
{
    ResetInitialPlayerAvatarState();
    StartEscapeRopeFieldEffect();
    DestroyTask(taskId);
}

static void UseTownMapFromBag(void)
{
    InitRegionMapWithExitCB(REGIONMAP_TYPE_NORMAL, CB2_BagMenuFromStartMenu);
}

static void Task_UseTownMapFromField(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        SetFieldCallback2ForItemUse();
        InitRegionMapWithExitCB(REGIONMAP_TYPE_NORMAL, CB2_ReturnToField);
        DestroyTask(taskId);
    }
}

void FieldUseFunc_TownMap(u8 taskId)
{
    if (gTasks[taskId].data[3] == 0)
    {
        ItemMenu_SetExitCallback(UseTownMapFromBag);
        ItemMenu_StartFadeToExitCallback(taskId);
    }
    else
    {
        StopPokemonLeagueLightingEffectTask();
        FadeScreen(FADE_TO_BLACK, 0);
        gTasks[taskId].func = Task_UseTownMapFromField;
    }
}

static void UseFameCheckerFromBag(void)
{
    UseFameChecker(CB2_BagMenuFromStartMenu);
}

static void Task_UseFameCheckerFromField(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        SetFieldCallback2ForItemUse();
        UseFameChecker(CB2_ReturnToField);
        DestroyTask(taskId);
    }
}

void FieldUseFunc_FameChecker(u8 taskId)
{
    if (gTasks[taskId].data[3] == 0)
    {
        ItemMenu_SetExitCallback(UseFameCheckerFromBag);
        ItemMenu_StartFadeToExitCallback(taskId);
    }
    else
    {
        StopPokemonLeagueLightingEffectTask();
        FadeScreen(FADE_TO_BLACK, 0);
        gTasks[taskId].func = Task_UseFameCheckerFromField;
    }
}

void FieldUseFunc_VsSeeker(u8 taskId)
{
    if ((gMapHeader.mapType != MAP_TYPE_ROUTE && gMapHeader.mapType != MAP_TYPE_TOWN && gMapHeader.mapType != MAP_TYPE_CITY)
		|| (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(VIRIDIAN_FOREST) && (gSaveBlock1Ptr->location.mapNum == MAP_NUM(VIRIDIAN_FOREST)
		|| gSaveBlock1Ptr->location.mapNum == MAP_NUM(MT_EMBER_EXTERIOR) || gSaveBlock1Ptr->location.mapNum == MAP_NUM(THREE_ISLAND_BERRY_FOREST)
		|| gSaveBlock1Ptr->location.mapNum == MAP_NUM(SIX_ISLAND_PATTERN_BUSH))))
        PrintNotTheTimeToUseThat(taskId, gTasks[taskId].data[3]);
    else
    {
        sItemUseOnFieldCB = Task_VsSeeker_0;
        SetUpItemUseOnFieldCallback(taskId);
    }
}

void FieldUseFunc_Honey(u8 taskId)
{
    sItemUseOnFieldCB = ItemUseOnFieldCB_Honey;
    SetUpItemUseOnFieldCallback(taskId);
}

static void ItemUseOnFieldCB_Honey(u8 taskId)
{
    RemoveUsedItem();
    gTasks[taskId].data[0] = 0;
    DisplayItemMessageOnField(taskId, 2, gStringVar4, FieldUseFunc_HoneyCB);
}

static void FieldUseFunc_HoneyCB(u8 taskId)
{
    SetWeatherScreenFadeOut();
    StartSweetScentFieldEffect();
    DestroyTask(taskId);
}

void FieldUseFunc_ExpShare(u8 taskId)
{
	const u8 *txt;
	
	gSaveBlock2Ptr->expShare ^= TRUE;
    
	if (gSaveBlock2Ptr->expShare)
	{
		txt = gText_ExpShareOn;
		PlaySE(SE_EXP_MAX);
	}
	else
	{
		txt = gText_ExpShareOff;
		PlaySE(SE_PC_OFF);
	}
	DisplayItemMessageInCurrentContext(taskId, gTasks[taskId].data[3], 2, txt);
}

static void OpenEvIvDisplayFromBag(void)
{
	ShowEvIvDisplay(CB2_BagMenuFromStartMenu);
}

static void Task_OpenEvIvDisplayFromField(u8 taskId)
{
	if (!gPaletteFade.active)
	{
		CleanupOverworldWindowsAndTilemaps();
		SetFieldCallback2ForItemUse();
		ShowEvIvDisplay(CB2_ReturnToField);
		DestroyTask(taskId);
	}
}

void FieldUseFunc_EvIvDisplay(u8 taskId)
{
	if (gTasks[taskId].data[3] == 0)
	{
		ItemMenu_SetExitCallback(OpenEvIvDisplayFromBag);
		ItemMenu_StartFadeToExitCallback(taskId);
	}
	else
	{
		StopPokemonLeagueLightingEffectTask();
		FadeScreen(FADE_TO_BLACK, 0);
		gTasks[taskId].func = Task_OpenEvIvDisplayFromField;
	}
}

static void FormChangeItem_ShowPartyMenuFromField(u8 taskId)
{
	if (!gPaletteFade.active)
	{
		CleanupOverworldWindowsAndTilemaps();
		SetFieldCallback2ForItemUse();
		InitPartyMenu(PARTY_MENU_TYPE_FIELD, PARTY_LAYOUT_SINGLE, PARTY_ACTION_USE_ITEM, TRUE, PARTY_MSG_USE_ON_WHICH_MON, Task_HandleChooseMonInput, CB2_ReturnToFieldContinueScript);
		DestroyTask(taskId);
	}
}

void FieldUseFunc_FormChangeItem(u8 taskId)
{
	gItemUseCB = ItemUseCB_FormChange;
	
	if (gTasks[taskId].data[3] == 0)
		Task_FadeOuFromBackToField(taskId);
	else
	{
		FadeScreen(FADE_TO_BLACK, 0);
		gTasks[taskId].func = FormChangeItem_ShowPartyMenuFromField;
	}
}

void FieldUseFunc_FormChangeItemListMenu(u8 taskId)
{
	gItemUseCB = ItemUseCB_FormChangeListMenu;
	
	if (gTasks[taskId].data[3] == 0)
		Task_FadeOuFromBackToField(taskId);
	else
	{
		FadeScreen(FADE_TO_BLACK, 0);
		gTasks[taskId].func = FormChangeItem_ShowPartyMenuFromField;
	}
}

void FieldUseFunc_FusionItem(u8 taskId)
{
	gItemUseCB = ItemUseCB_Fusion;
	
	if (gTasks[taskId].data[3] == 0)
		Task_FadeOuFromBackToField(taskId);
	else
	{
		FadeScreen(FADE_TO_BLACK, 0);
		gTasks[taskId].func = FormChangeItem_ShowPartyMenuFromField;
	}
}

void FieldUseFunc_ChangeAbility(u8 taskId)
{
	gItemUseCB = ItemUseCB_ChangeAbility;
    Task_FadeOuFromBackToField(taskId);
}

void FieldUseFunc_Mint(u8 taskId)
{
	gItemUseCB = ItemUseCB_Mint;
    Task_FadeOuFromBackToField(taskId);
}

static void ItemUse_SwitchToPartyMenuInBattle(u8 taskId)
{
    if (ItemId_GetPocket(gSpecialVar_ItemId) == POCKET_BERRY_POUCH)
    {
        BerryPouch_SetExitCallback(EnterPartyFromItemMenuInBattle);
        BerryPouch_StartFadeToExitCallback(taskId);
    }
    else
    {
        ItemMenu_SetExitCallback(EnterPartyFromItemMenuInBattle);
        ItemMenu_StartFadeToExitCallback(taskId);
    }
}

void ItemUseInBattle_PartyMenu(u8 taskId)
{
    gItemUseCB = ItemUseCB_BattleScript;
    ItemUse_SwitchToPartyMenuInBattle(taskId);
}

void ItemUseInBattle_PartyMenuChooseMove(u8 taskId)
{
    gItemUseCB = ItemUseCB_PPRecoveryOneMove;
    ItemUse_SwitchToPartyMenuInBattle(taskId);
}

void ItemUseInBattle_BagMenu(u8 taskId)
{
	PlaySE(SE_SELECT);
	
	if (!CanUseItemInBattle(TRUE, gSpecialVar_ItemId))
		DisplayItemMessageInBag(taskId, 2, gStringVar4, Task_ReturnToBagFromContextMenu);
	else
	{
		SetBattlerUsedItemForBattleScript(gSpecialVar_ItemId, TRUE);
		gTasks[taskId].func = ItemMenu_StartFadeToExitCallback;
	}
}

void FieldUseFunc_OakStopsYou(u8 taskId)
{
    if (ItemId_GetPocket(gSpecialVar_ItemId) == POCKET_BERRY_POUCH)
    {
        StringExpandPlaceholders(gStringVar4, gText_OakForbidsUseOfItemHere);
        DisplayItemMessageInBerryPouch(taskId, 4, gStringVar4, Task_BerryPouch_DestroyDialogueWindowAndRefreshListMenu);
    }
    else
        PrintNotTheTimeToUseThat(taskId, gTasks[taskId].data[3]);
}

bool8 ExecuteTableBasedItemEffect(u8 partyMonIndex, u16 item)
{
	if (gMain.inBattle)
		gBattleStruct->battlers[gBattlerInMenuId].usedReviveItem = FALSE;
	
	return PokemonUseItemEffects(&gPlayerParty[partyMonIndex], item, partyMonIndex, GetBattleMonForItemUse(gBattlerInMenuId, GetPartyIdFromBattleSlot(partyMonIndex)));
}

bool8 CanUseItemInBattle(bool8 fromBagMenu, u16 itemId)
{
	bool8 canUse = FALSE;
	const u8* failStr = NULL;
	
	if (!IsItemUseBlockedByBattleEffect(fromBagMenu ? gBattlerInMenuId : GetBattleMonForItemUse(gBattlerInMenuId, GetPartyIdFromBattleSlot(gPartyMenu.slotId)))
	|| ItemId_GetBattleUsage(itemId) == EFFECT_ITEM_THROW_BALL) // Poke balls can be used regardless of preventing effects
	{
		if (!fromBagMenu) // use it on a party mon
		{
			if (!ExecuteTableBasedItemEffect(gPartyMenu.slotId, itemId))
				canUse = TRUE;
		}
		else // use it in self
			failStr = PokemonUseItemEffectsBattle(gBattlerInMenuId, itemId, &canUse);
	}
	StringExpandPlaceholders(gStringVar4, failStr != NULL ? failStr : gText_WontHaveEffect);
	
	return canUse;
}

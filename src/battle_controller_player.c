#include "global.h"
#include "gflib.h"
#include "data.h"
#include "item.h"
#include "item_menu.h"
#include "link.h"
#include "m4a.h"
#include "party_menu.h"
#include "pokeball.h"
#include "strings.h"
#include "pokemon_special_anim.h"
#include "task.h"
#include "util.h"
#include "battle.h"
#include "menu.h"
#include "menu_indicators.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "pokemon_summary_screen.h"
#include "battle_message.h"
#include "battle_script_commands.h"
#include "reshow_battle_screen.h"
#include "constants/battle_anim.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/songs.h"
#include "constants/battle_move_effects.h"

static void PlayerBufferRunCommand(u8 battlerId);
static void PlayerBufferExecCompleted(u8 battlerId);
static void PlayerHandleLoadMonSprite(u8 battlerId);
static void PlayerHandleSwitchInAnim(u8 battlerId);
static void PlayerHandleTrainerSlide(u8 battlerId);
static void PlayerHandleTrainerSlideBack(u8 battlerId);
static void PlayerHandleChoosePokemon(u8 battlerId);
static void PlayerHandleTwoReturnValues(u8 battlerId);
static void PlayerHandleChosenMonReturnValue(u8 battlerId);
static void PlayerHandleOneReturnValue(u8 battlerId);
static void PlayerHandleIntroTrainerBallThrow(u8 battlerId);
static void PlayerHandleResetActionMoveSelection(u8 battlerId);
static void HandleInputChooseAction(u8 battlerId);
static void SetPlayerChooseMoveInput(u8 battlerId);
static void HandleInputChooseTarget(u8 battlerId);
static void HandleMoveSwitching(u8 battlerId);
static void HandleInputMoveInfo(u8 battlerId);
static void TryShowAsTarget(u8 battlerId);
static void HandleInputShowTargets(u8 battlerId);
static void HandleInputShowEntireFieldTargets(u8 battlerId);
static void MoveInfoPrintMoveNameAndDescription(u8 battlerId);
static void MoveInfoPrintSubmenuString(u8 battlerId, u8 stateId);
static void MoveInfoPrintPowerAndAccuracy(u16 move);
static void MoveInfoPrintPriorityAndCategory(u16 move);
static void MoveInfoPrintMoveTarget(u16 move);

static void (*const sPlayerBufferCommands[CONTROLLER_CMDS_COUNT])(u8) =
{
    [CONTROLLER_GETMONDATA]               = BtlController_HandleGetMonData,
	[CONTROLLER_SETMONDATA]               = BtlController_HandleSetMonData,
	[CONTROLLER_SETRAWMONDATA]            = BtlController_HandleSetRawMonData,
	[CONTROLLER_LOADMONSPRITE]            = PlayerHandleLoadMonSprite,
	[CONTROLLER_SWITCHINANIM]             = PlayerHandleSwitchInAnim,
	[CONTROLLER_RETURNMONTOBALL]          = BtlController_HandleReturnMonToBall,
	[CONTROLLER_DRAWTRAINERPIC]           = PlayerHandleDrawTrainerPic,
	[CONTROLLER_TRAINERSLIDE]             = PlayerHandleTrainerSlide,
	[CONTROLLER_TRAINERSLIDEBACK]         = PlayerHandleTrainerSlideBack,
	[CONTROLLER_FAINTANIMATION]           = BtlController_HandleFaintAnimation,
	[CONTROLLER_BALLTHROWANIM]            = PlayerHandleBallThrowAnim,
	[CONTROLLER_MOVEANIMATION]            = BtlController_HandleMoveAnimation,
	[CONTROLLER_PRINTSTRING]              = PlayerHandlePrintString,
	[CONTROLLER_PRINTSELECTIONSTRING]     = PlayerHandlePrintSelectionString,
	[CONTROLLER_CHOOSEACTION]             = PlayerHandleChooseAction,
	[CONTROLLER_CHOOSEMOVE]               = PlayerHandleChooseMove,
	[CONTROLLER_OPENBAG]                  = PlayerHandleChooseItem,
	[CONTROLLER_CHOOSEPOKEMON]            = PlayerHandleChoosePokemon,
	[CONTROLLER_HEALTHBARUPDATE]          = PlayerHandleHealthbarUpdate,
	[CONTROLLER_EXPUPDATE]                = BtlController_HandleExpUpdate,
	[CONTROLLER_STATUSICONUPDATE]         = BtlController_HandleStatusIconUpdate,
	[CONTROLLER_STATUSANIMATION]          = BtlController_HandleStatusAnimation,
	[CONTROLLER_DATATRANSFER]             = BattleControllerComplete,
	[CONTROLLER_TWORETURNVALUES]          = PlayerHandleTwoReturnValues,
	[CONTROLLER_CHOSENMONRETURNVALUE]     = PlayerHandleChosenMonReturnValue,
	[CONTROLLER_ONERETURNVALUE]           = PlayerHandleOneReturnValue,
	[CONTROLLER_EFFECTIVENESSSOUND]       = BtlController_HandlePlaySE,
	[CONTROLLER_PLAYFANFAREORBGM]         = BtlController_HandlePlayFanfare,
	[CONTROLLER_FAINTINGCRY]              = BtlController_HandleFaintingCry,
	[CONTROLLER_INTROSLIDE]               = BtlController_HandleIntroSlide,
	[CONTROLLER_INTROTRAINERBALLTHROW]    = PlayerHandleIntroTrainerBallThrow,
	[CONTROLLER_DRAWPARTYSTATUSSUMMARY]   = BtlController_HandleDrawPartyStatusSummary,
	[CONTROLLER_HIDEPARTYSTATUSSUMMARY]   = BtlController_HandleHidePartyStatusSummary,
	[CONTROLLER_ENDBOUNCE]                = BtlController_HandleEndBounceEffect,
	[CONTROLLER_SPRITEINVISIBILITY]       = BtlController_HandleSpriteInvisibility,
	[CONTROLLER_BATTLEANIMATION]          = BtlController_HandleBattleAnimation,
	[CONTROLLER_LINKSTANDBYMSG]           = PlayerHandleLinkStandbyMsg,
	[CONTROLLER_RESETACTIONMOVESELECTION] = PlayerHandleResetActionMoveSelection,
	[CONTROLLER_ENDLINKBATTLE]            = PlayerHandleEndLinkBattle,
	[CONTROLLER_TERMINATOR_NOP]           = ControllerDummy,
};

void SetControllerToPlayer(u8 battlerId)
{
    gBattlerControllerFuncs[battlerId] = PlayerBufferRunCommand;
	gBattlerControllerEndFuncs[battlerId] = PlayerBufferExecCompleted;
    gDoingBattleAnim = FALSE;
}

static void PlayerBufferRunCommand(u8 battlerId)
{
    if (gBattleControllerExecFlags & gBitTable[battlerId])
    {
        if (gBattleBufferA[battlerId][0] < ARRAY_COUNT(sPlayerBufferCommands))
            sPlayerBufferCommands[gBattleBufferA[battlerId][0]](battlerId);
        else
            PlayerBufferExecCompleted(battlerId);
    }
}

static void PlayerBufferExecCompleted(u8 battlerId)
{
	gBattlerControllerFuncs[battlerId] = PlayerBufferRunCommand;
	
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        u8 playerId = GetMultiplayerId();
        PrepareBufferDataTransferLink(battlerId, BUFFER_LINK, 4, &playerId);
        gBattleBufferA[battlerId][0] = CONTROLLER_TERMINATOR_NOP;
    }
    else
        gBattleControllerExecFlags &= ~(gBitTable[battlerId]);
}

////////////////////////
// BATTLE CONTROLLERS //
////////////////////////

static void PlayerHandleLoadMonSprite(u8 battlerId)
{
    BattleLoadMonSpriteGfx(battlerId);
    gSprites[gBattlerSpriteIds[battlerId]].oam.paletteNum = battlerId;
    gBattlerControllerFuncs[battlerId] = CompleteOnBattlerSpritePosX_0;
}

static void SwitchIn_HandleSoundAndEnd(u8 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].specialAnimActive && !IsCryPlayingOrClearCrySongs())
    {
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 0x100);
        HandleLowHpMusicChange(&gPlayerParty[gBattlerPartyIndexes[battlerId]], battlerId);
        BattleControllerComplete(battlerId);
    }
}

static void SwitchIn_CleanShinyAnimShowSubstitute(u8 battlerId)
{
    if (gSprites[gHealthboxSpriteIds[battlerId]].callback == SpriteCallbackDummy && gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim)
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim = FALSE;
        gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim = FALSE;
        FreeSpriteTilesByTag(ANIM_TAG_GOLD_STARS);
        FreeSpritePaletteByTag(ANIM_TAG_GOLD_STARS);
		
        if (gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute)
            InitAndLaunchSpecialAnimation(battlerId, battlerId, battlerId, B_ANIM_MON_TO_SUBSTITUTE);
		
        gBattlerControllerFuncs[battlerId] = SwitchIn_HandleSoundAndEnd;
    }
}

static void SwitchIn_TryShinyAnimShowHealthbox(u8 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim && !gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive)
        TryShinyAnimation(battlerId);
	
    if (gSprites[gBattleControllerData[battlerId]].callback == SpriteCallbackDummy && !gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive)
    {
        DestroySprite(&gSprites[gBattleControllerData[battlerId]]);
        UpdateHealthboxAttribute(battlerId, HEALTHBOX_ALL);
        StartHealthboxSlideIn(battlerId);
        SetHealthboxSpriteVisible(gHealthboxSpriteIds[battlerId]);
        CopyBattleSpriteInvisibility(battlerId);
        gBattlerControllerFuncs[battlerId] = SwitchIn_CleanShinyAnimShowSubstitute;
    }
}

static void PlayerHandleSwitchInAnim(u8 battlerId)
{
	BtlController_HandleSwitchInAnim(battlerId, TRUE, SwitchIn_TryShinyAnimShowHealthbox);
	gActionSelectionCursor[battlerId] = 0;
    gMoveSelectionCursor[battlerId] = 0;
}

static u32 GetPlayerTrainerPicId(void)
{
	u8 playerId;
	
	if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
		playerId = GetMultiplayerId();
		
        if ((gLinkPlayers[playerId].version & 0xFF) == VERSION_RUBY || (gLinkPlayers[playerId].version & 0xFF) == VERSION_SAPPHIRE
         || (gLinkPlayers[playerId].version & 0xFF) == VERSION_EMERALD)
            return gLinkPlayers[playerId].gender + BACK_PIC_RS_BRENDAN;
        else
            return gLinkPlayers[playerId].gender + BACK_PIC_RED;
    }
    else
        return gSaveBlock2Ptr->playerGender + BACK_PIC_RED;
}

void PlayerHandleDrawTrainerPic(u8 battlerId)
{
	u8 playerId;
	s16 xPos;
    u32 trainerPicId = GetPlayerTrainerPicId();

    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        if ((GetBattlerPosition(battlerId) & BIT_FLANK) != B_FLANK_LEFT) // Second mon, on the right.
            xPos = 90;
        else // First mon, on the left.
            xPos = 32;
    }
    else
        xPos = 80;
	
	BtlController_HandleDrawTrainerPic(battlerId, trainerPicId, FALSE, xPos, (8 - gTrainerBackPicCoords[trainerPicId].size) * 4 + 80, GetBattlerSpriteSubpriority(battlerId));
}

static void PlayerHandleTrainerSlide(u8 battlerId)
{
	u32 trainerPicId = GetPlayerTrainerPicId();
	BtlController_HandleTrainerSlide(battlerId, trainerPicId, FALSE, 80, (8 - gTrainerBackPicCoords[trainerPicId].size) * 4 + 80);
}

static void PlayerHandleTrainerSlideBack(u8 battlerId)
{
	BtlController_HandleTrainerSlideBack(battlerId, 50, TRUE);
}

void PlayerHandleBallThrowAnim(u8 battlerId)
{
	BtlController_HandleBallThrowAnim(battlerId, B_ANIM_BALL_THROW);
}

static void PlayerHandlePrintStringInternal(u8 battlerId, bool8 isSelection)
{
	u16 *stringId = (u16 *)(&gBattleBufferA[battlerId][2]);
	BtlController_HandlePrintString(battlerId, *stringId, isSelection);
}

void PlayerHandlePrintString(u8 battlerId)
{
	PlayerHandlePrintStringInternal(battlerId, FALSE);
}

void PlayerHandlePrintSelectionString(u8 battlerId)
{
	if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
        PlayerHandlePrintStringInternal(battlerId, TRUE);
    else
        BattleControllerComplete(battlerId);
}

static void HandleChooseActionAfterDma3(u8 battlerId)
{
    if (!IsDma3ManagerBusyWithBgCopy())
    {
        gBattle_BG0_X = 0;
        gBattle_BG0_Y = 160;
#if WEATHER_ICON_IN_BATTLE
		if (!(gBattleTypeFlags & (BATTLE_TYPE_OLD_MAN_TUTORIAL | BATTLE_TYPE_POKEDUDE)))
			TryCreateWeatherAnimIcon();
#endif
        if (gBattleTypeFlags & BATTLE_TYPE_OLD_MAN_TUTORIAL)
			gBattlerControllerFuncs[battlerId] = OakOldMan_SimulateInputChooseAction;
		else if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
			gBattlerControllerFuncs[battlerId] = Pokedude_SimulateInputChooseAction;
		else
			gBattlerControllerFuncs[battlerId] = HandleInputChooseAction;
    }
}

void PlayerHandleChooseAction(u8 battlerId)
{
	if (gBattleTypeFlags & BATTLE_TYPE_OLD_MAN_TUTORIAL)
		StringCopy(gBattleTextBuff1, gText_TheOldMan);
	else
		PrepareMonNickNoIllusionBuffer(gBattleTextBuff1, battlerId, gBattlerPartyIndexes[battlerId]);
	
	BtlController_HandleChooseAction(battlerId, gText_BattleMenu, gText_WhatWillBufferDo, HandleChooseActionAfterDma3);
}

static void HandleChooseMoveAfterDma3(u8 battlerId)
{
    if (!IsDma3ManagerBusyWithBgCopy())
    {
        gBattle_BG0_X = 0;
        gBattle_BG0_Y = 320;
		
		SetPlayerChooseMoveInput(battlerId);
    }
}

void PlayerHandleChooseMove(u8 battlerId)
{
	InitMoveSelectionsVarsAndStrings(battlerId);
    gBattlerControllerFuncs[battlerId] = HandleChooseMoveAfterDma3;
}

static void CompleteWhenChooseItem(u8 battlerId)
{
    if (gMain.callback2 == BattleMainCB2 && !gPaletteFade.active)
    {
		if (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE && !BtlCtrl_OakOldMan_TestState2Flag(FIRST_BATTLE_MSG_FLAG_HP_RESTORE) && gSpecialVar_ItemId == ITEM_POTION)
        {
            BtlCtrl_OakOldMan_SetState2Flag(FIRST_BATTLE_MSG_FLAG_HP_RESTORE);
            gBattlerControllerFuncs[battlerId] = PrintOakText_KeepAnEyeOnHP;
        }
		else
		{
			BtlController_EmitOneReturnValue(battlerId, BUFFER_B, gSpecialVar_ItemId);
			BattleControllerComplete(battlerId);
		}
    }
}

static void OpenBagAndChooseItem(u8 battlerId)
{
    if (!gPaletteFade.active)
    {
        gBattlerControllerFuncs[battlerId] = CompleteWhenChooseItem;
        FreeAllWindowBuffers();
		
		if (gBattleTypeFlags & BATTLE_TYPE_OLD_MAN_TUTORIAL)
			InitOldManBag();
		else
			CB2_BagMenuFromBattle();
    }
}

void PlayerHandleChooseItem(u8 battlerId)
{
	u8 i;
	
	BtlController_HandleChooseItem(battlerId, OpenBagAndChooseItem);
	
	for (i = 0; i < 3; ++i)
        gBattlePartyCurrentOrder[i] = gBattleBufferA[battlerId][1 + i];
}

static void PrintLinkStandbyMsg(void)
{
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        gBattle_BG0_X = 0;
        gBattle_BG0_Y = 0;
        BattlePutTextOnWindow(gText_LinkStandby, B_WIN_MSG);
    }
}

static void WaitForMonSelection(u8 battlerId)
{
    if (gMain.callback2 == BattleMainCB2 && !gPaletteFade.active)
    {
        if (gPartyMenuUseExitCallback)
            BtlController_EmitChosenMonReturnValue(battlerId, BUFFER_B, gSelectedMonPartyId, gBattlePartyCurrentOrder);
        else
            BtlController_EmitChosenMonReturnValue(battlerId, BUFFER_B, PARTY_SIZE, NULL);
		
        if ((gBattleBufferA[battlerId][1] & 0xF) == 1)
            PrintLinkStandbyMsg();
		
        BattleControllerComplete(battlerId);
    }
}

static void OpenPartyMenuToChooseMon(u8 battlerId)
{
    if (!gPaletteFade.active)
    {
        u8 caseId;

        gBattlerControllerFuncs[battlerId] = WaitForMonSelection;
        caseId = gTasks[gBattleControllerData[battlerId]].data[0];
        DestroyTask(gBattleControllerData[battlerId]);
        FreeAllWindowBuffers();
        OpenPartyMenuInTutorialBattle(caseId);
    }
}

static void PlayerHandleChoosePokemon(u8 battlerId)
{
	BtlController_HandleChoosePokemon(battlerId, OpenPartyMenuToChooseMon);
}

void PlayerHandleHealthbarUpdate(u8 battlerId)
{
	BtlController_HandleHealthbarUpdate(battlerId, TRUE);
}

static void PlayerHandleTwoReturnValues(u8 battlerId)
{
	BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, 0, 0);
	BattleControllerComplete(battlerId);
}

static void PlayerHandleChosenMonReturnValue(u8 battlerId)
{
	BtlController_EmitChosenMonReturnValue(battlerId, BUFFER_B, 0, NULL);
	BattleControllerComplete(battlerId);
}

static void PlayerHandleOneReturnValue(u8 battlerId)
{
	BtlController_EmitOneReturnValue(battlerId, BUFFER_B, 0);
	BattleControllerComplete(battlerId);
}

static void Intro_DelayAndEnd(u8 battlerId)
{
    if (--gBattleSpritesDataPtr->healthBoxesData[battlerId].introEndDelay == 0xFF)
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].introEndDelay = 0;
        BattleControllerComplete(battlerId);
    }
}

static void Intro_WaitForShinyAnimAndHealthbox(u8 battlerId)
{
    bool8 var = FALSE;

    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE) || ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && (gBattleTypeFlags & BATTLE_TYPE_MULTI)))
    {
        if (gSprites[gHealthboxSpriteIds[battlerId]].callback == SpriteCallbackDummy)
            var = TRUE;
    }
    else
    {
        if (gSprites[gHealthboxSpriteIds[battlerId]].callback == SpriteCallbackDummy && gSprites[gHealthboxSpriteIds[BATTLE_PARTNER(battlerId)]].callback == SpriteCallbackDummy)
            var = TRUE;
    }
    if (IsCryPlayingOrClearCrySongs())
        var = FALSE;
	
    if (var && gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim && gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].finishedShinyMonAnim)
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim = FALSE;
        gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim = FALSE;
        gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].triedShinyMonAnim = FALSE;
        gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].finishedShinyMonAnim = FALSE;
		
        FreeSpriteTilesByTag(ANIM_TAG_GOLD_STARS);
        FreeSpritePaletteByTag(ANIM_TAG_GOLD_STARS);
		
		if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
			m4aMPlayContinue(&gMPlayInfo_BGM);
		else
			m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 256);
		
        HandleLowHpMusicChange(&gPlayerParty[gBattlerPartyIndexes[battlerId]], battlerId);
		
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            HandleLowHpMusicChange(&gPlayerParty[gBattlerPartyIndexes[BATTLE_PARTNER(battlerId)]], BATTLE_PARTNER(battlerId));
		
        gBattleSpritesDataPtr->healthBoxesData[battlerId].introEndDelay = 3;
        gBattlerControllerFuncs[battlerId] = Intro_DelayAndEnd;
    }
}

static void Intro_TryShinyAnimShowHealthbox(u8 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive && !gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].ballAnimActive)
    {
        if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim)
            TryShinyAnimation(battlerId);
		
        if (!gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].triedShinyMonAnim)
            TryShinyAnimation(BATTLE_PARTNER(battlerId));
		
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !(gBattleTypeFlags & BATTLE_TYPE_MULTI))
			ShowHealthBox(BATTLE_PARTNER(battlerId));
		
        ShowHealthBox(battlerId);
        gBattleSpritesDataPtr->animationData->healthboxSlideInStarted = FALSE;
        gBattlerControllerFuncs[battlerId] = Intro_WaitForShinyAnimAndHealthbox;
    }
}

static void PlayerHandleIntroTrainerBallThrow(u8 battlerId)
{
	BtlController_HandleIntroTrainerBallThrow(battlerId, 0xD6F8, BACK_PIC_RED + gSaveBlock2Ptr->playerGender, PlayerThrowBall_StartAnimLinearTranslation, 31, Intro_TryShinyAnimShowHealthbox);
}

void PlayerHandleLinkStandbyMsg(u8 battlerId)
{
	switch (gBattleBufferA[battlerId][1])
    {
    case 0:
        PrintLinkStandbyMsg();
        // fall through
    case 1:
        EndBounceEffect(battlerId, BOUNCE_HEALTHBOX);
        EndBounceEffect(battlerId, BOUNCE_MON);
        break;
    case 2:
        PrintLinkStandbyMsg();
        break;
    }
    BattleControllerComplete(battlerId);
}

static void PlayerHandleResetActionMoveSelection(u8 battlerId)
{
	switch (gBattleBufferA[battlerId][1])
    {
    case RESET_ACTION_MOVE_SELECTION:
        gActionSelectionCursor[battlerId] = 0;
        gMoveSelectionCursor[battlerId] = 0;
        break;
    case RESET_ACTION_SELECTION:
        gActionSelectionCursor[battlerId] = 0;
        break;
    case RESET_MOVE_SELECTION:
        gMoveSelectionCursor[battlerId] = 0;
        break;
    }
	BattleControllerComplete(battlerId);
}

void PlayerHandleEndLinkBattle(u8 battlerId)
{
	BtlController_HandleEndLinkBattle(battlerId, gBattleBufferA[battlerId][1], SetBattleEndCallbacks);
}

////////////////////////////
// ACTION SELECTION INPUT //
////////////////////////////

void ActionSelectionCreateCursorAt(u8 cursorPosition)
{
    u16 src[2] = {1, 2};
    CopyToBgTilemapBufferRect_ChangePalette(0, src, 7 * (cursorPosition & 1) + 16, 35 + (cursorPosition & 2), 1, 2, 0x11);
    CopyBgTilemapBufferToVram(0);
}

void ActionSelectionDestroyCursorAt(u8 cursorPosition)
{
    u16 src[2] = {32, 32};
    CopyToBgTilemapBufferRect_ChangePalette(0, src, 7 * (cursorPosition & 1) + 16, 35 + (cursorPosition & 2), 1, 2, 0x11);
    CopyBgTilemapBufferToVram(0);
}

static void HandleInputChooseAction(u8 battlerId)
{
    u16 itemId = gBattleBufferA[battlerId][2] | (gBattleBufferA[battlerId][3] << 8);

    DoBounceEffect(battlerId, BOUNCE_HEALTHBOX, 7, 1);
    DoBounceEffect(battlerId, BOUNCE_MON, 7, 1);
	
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
#if WEATHER_ICON_IN_BATTLE
        ShowOrHideWeatherAnimIcon(TRUE); // hide icon
#endif

        switch (gActionSelectionCursor[battlerId])
        {
        case 0:
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_USE_MOVE, 0);
            break;
        case 1:
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_USE_ITEM, 0);
            break;
        case 2:
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_SWITCH, 0);
            break;
        case 3:
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_RUN, 0);
            break;
        }
        BattleControllerComplete(battlerId);
    }
    else if (JOY_NEW(DPAD_LEFT))
    {
        if (gActionSelectionCursor[battlerId] & 1) // if is B_ACTION_USE_ITEM or B_ACTION_RUN
        {
            PlaySE(SE_SELECT);
            ActionSelectionDestroyCursorAt(gActionSelectionCursor[battlerId]);
            gActionSelectionCursor[battlerId] ^= 1;
            ActionSelectionCreateCursorAt(gActionSelectionCursor[battlerId]);
        }
    }
    else if (JOY_NEW(DPAD_RIGHT))
    {
        if (!(gActionSelectionCursor[battlerId] & 1)) // if is B_ACTION_USE_MOVE or B_ACTION_SWITCH
        {
            PlaySE(SE_SELECT);
            ActionSelectionDestroyCursorAt(gActionSelectionCursor[battlerId]);
            gActionSelectionCursor[battlerId] ^= 1;
            ActionSelectionCreateCursorAt(gActionSelectionCursor[battlerId]);
        }
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if (gActionSelectionCursor[battlerId] & 2) // if is B_ACTION_SWITCH or B_ACTION_RUN
        {
            PlaySE(SE_SELECT);
            ActionSelectionDestroyCursorAt(gActionSelectionCursor[battlerId]);
            gActionSelectionCursor[battlerId] ^= 2;
            ActionSelectionCreateCursorAt(gActionSelectionCursor[battlerId]);
        }
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (!(gActionSelectionCursor[battlerId] & 2)) // if is B_ACTION_USE_MOVE or B_ACTION_USE_ITEM
        {
            PlaySE(SE_SELECT);
            ActionSelectionDestroyCursorAt(gActionSelectionCursor[battlerId]);
            gActionSelectionCursor[battlerId] ^= 2;
            ActionSelectionCreateCursorAt(gActionSelectionCursor[battlerId]);
        }
    }
    else if (JOY_NEW(B_BUTTON))
    {
        if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && GetBattlerPosition(battlerId) == B_POSITION_PLAYER_RIGHT
         && !(gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)]) && !(gBattleTypeFlags & BATTLE_TYPE_MULTI))
        {
			// Return item to bag if partner had selected one.
            if (gBattleBufferA[battlerId][1] == B_ACTION_USE_ITEM)
				AddBagItem(itemId, 1);
			
            PlaySE(SE_SELECT);
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_CANCEL_PARTNER, 0);
            BattleControllerComplete(battlerId);
        }
    }
    else if (JOY_NEW(START_BUTTON))
        SwapHpBarsWithHpText();
}

///////////////////////////////
// MOVE SELECTION DISPLAYERS //
///////////////////////////////

static const u16 sEffectivenessColours[] =
{
    // super effective colors
    RGB(18, 30, 18), 
    RGB(11, 19, 8),
    // super effective colors + stab
    RGB(13, 30, 13), 
    RGB(0, 20, 0),
    // not very effect colors
    RGB(31, 30, 17),
    RGB(29, 27, 0),
    // not very effect colors + stab
    RGB(31, 29, 14),
    RGB(31, 18, 0),
    // not affect colors
    RGB(30, 27, 19),
    RGB(28, 8, 8),
    // not affect colors + stab (unused)
    RGB(30, 27, 14),
    RGB(29, 0, 0),
    // normal colors
    RGB(24, 24, 23),
    RGB(9, 9, 9),
    // normal colors + stab
    RGB(15, 15, 15),
    RGB_BLACK,
};

static void MoveSelectionDisplayMoveNames(u8 battlerId)
{
    u8 i;
    struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct *)(&gBattleBufferA[battlerId][4]);
	
    gNumberOfMovesToChoose = 0;

    for (i = 0; i < MAX_MON_MOVES; ++i)
    {
        MoveSelectionDestroyCursorAt(i);
        StringCopy(gDisplayedStringBattle, gText_MoveInterfaceMoveNamesColor);
        StringAppend(gDisplayedStringBattle, gBattleMoves[moveInfo->moves[i]].name);
        BattlePutTextOnWindow(gDisplayedStringBattle, i + B_WIN_MOVE_NAME_1);
		
        if (moveInfo->moves[i])
            ++gNumberOfMovesToChoose;
    }
}

static void MoveSelectionDisplayPpString(void)
{
    StringCopy(gDisplayedStringBattle, gText_MoveInterfacePP);
    BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_PP);
}

static void MoveSelectionDisplayPpNumber(u8 battlerId)
{
    u8 *txtPtr;
    struct ChooseMoveStruct *moveInfo;
	
	if (!gBattleBufferA[battlerId][2]) // check if we didn't want to display pp number
	{
		moveInfo = (struct ChooseMoveStruct *)(&gBattleBufferA[battlerId][4]);
		
		SetPpNumbersPaletteInMoveSelection(battlerId);
		txtPtr = ConvertIntToDecimalStringN(gDisplayedStringBattle, moveInfo->currentPp[gMoveSelectionCursor[battlerId]], STR_CONV_MODE_RIGHT_ALIGN, 2);
		*txtPtr = CHAR_SLASH;
		ConvertIntToDecimalStringN(++txtPtr, moveInfo->maxPp[gMoveSelectionCursor[battlerId]], STR_CONV_MODE_RIGHT_ALIGN, 2);
		BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_PP_REMAINING);
	}
}

static void MoveSelectionDisplayMoveType(u8 battlerId)
{
	u8 *txtPtr;
	struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct *)(&gBattleBufferA[battlerId][4]);
    u16 flags, move = moveInfo->moves[gMoveSelectionCursor[battlerId]];
    u8 target, effect, type = GetTypeChangingMoveType(GetBattlerPartyIndexPtr(battlerId), move);
    
    txtPtr = StringCopy(gDisplayedStringBattle, gText_MoveInterfaceType);
    *txtPtr++ = EXT_CTRL_CODE_BEGIN;
    *txtPtr++ = 6;
    *txtPtr++ = 1;
   
#if EFFECTIVENESS_ON_MENU
    target = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT); // default target
    effect = flags = 0;
    
    // check if move is stab
    if (!IS_MOVE_STATUS(move) && IS_BATTLER_OF_TYPE(battlerId, type))
        effect = 2;
	
    // try change move target in double
    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        if (gBattlerControllerFuncs[battlerId] == HandleInputChooseTarget)
            target = gMultiUsePlayerCursor;
        else if (!IsBattlerAlive(target))
            target = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
    }
	CalcTypeEffectivenessMultiplier(move, type, battlerId, target, FALSE, &flags);
    
    // set respective colours
    if (flags & MOVE_RESULT_SUPER_EFFECTIVE)
    {
        gPlttBufferUnfaded[88] = sEffectivenessColours[effect];
        gPlttBufferUnfaded[89] = sEffectivenessColours[effect + 1];
    }
    else if (flags & MOVE_RESULT_NOT_VERY_EFFECTIVE)
    {
        gPlttBufferUnfaded[88] = sEffectivenessColours[effect + 4];
        gPlttBufferUnfaded[89] = sEffectivenessColours[effect + 5];
    }
    else if (!(flags & MOVE_RESULT_NO_EFFECT))
    {
        gPlttBufferUnfaded[88] = sEffectivenessColours[effect + 12];
        gPlttBufferUnfaded[89] = sEffectivenessColours[effect + 13];
    }
    else
    {
        gPlttBufferUnfaded[88] = sEffectivenessColours[8];
        gPlttBufferUnfaded[89] = sEffectivenessColours[9];
    }
    CpuCopy16(&gPlttBufferUnfaded[88], &gPlttBufferFaded[88], sizeof(u16));
    CpuCopy16(&gPlttBufferUnfaded[89], &gPlttBufferFaded[89], sizeof(u16));
    
    txtPtr = StringCopy(txtPtr, gText_MoveInterfaceEffectiveness);
#else
    txtPtr = StringCopy(txtPtr, gText_MoveInterfaceMoveNamesColor);
#endif
    StringCopy(txtPtr, gTypeNames[type]);
    BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MOVE_TYPE);
}

static void MoveSelectionDisplayMoveMenu(u8 battlerId)
{
	MoveSelectionDisplayPpString();
	MoveSelectionDisplayPpNumber(battlerId);
	MoveSelectionDisplayMoveType(battlerId);
}

void InitMoveSelectionsVarsAndStrings(u8 battlerId)
{
    MoveSelectionDisplayMoveNames(battlerId);
    gMultiUsePlayerCursor = 0xFF;
    MoveSelectionCreateCursorAt(gMoveSelectionCursor[battlerId], 0);
	MoveSelectionDisplayMoveMenu(battlerId);
}

//////////////////////////
// MOVE SELECTION INPUT //
//////////////////////////

static void SetPlayerChooseMoveInput(u8 battlerId)
{
	CreateMoveInfoTriggerSprite(battlerId);
	gBattlerControllerFuncs[battlerId] = HandleInputChooseMove;
}

void MoveSelectionCreateCursorAt(u8 cursorPosition, u8 arg1)
{
    u16 src[2];
    src[0] = arg1 + 1;
    src[1] = arg1 + 2;
    CopyToBgTilemapBufferRect_ChangePalette(0, src, 9 * (cursorPosition & 1) + 1, 55 + (cursorPosition & 2), 1, 2, 0x11);
    CopyBgTilemapBufferToVram(0);
}

void MoveSelectionDestroyCursorAt(u8 cursorPosition)
{
    u16 src[2] = {32, 32};
    CopyToBgTilemapBufferRect_ChangePalette(0, src, 9 * (cursorPosition & 1) + 1, 55 + (cursorPosition & 2), 1, 2, 0x11);
    CopyBgTilemapBufferToVram(0);
}

static u8 PreviewDeterminativeMoveTargets(u8 battlerId, struct ChooseMoveStruct *moveInfo)
{
    u32 bitMask;
	u16 move = moveInfo->moves[gMoveSelectionCursor[battlerId]];
    u8 startY, moveTarget = GetBattlerMoveTargetType(battlerId, move);
	
    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
		bitMask = 0;
		startY = 0;
		
        switch (moveTarget)
        {
			case MOVE_TARGET_SELECTED:
			case MOVE_TARGET_DEPENDS:
			case MOVE_TARGET_RANDOM:
			case MOVE_TARGET_ALL_BATTLERS:
                bitMask = 0xF0000;
				break;
			case MOVE_TARGET_BOTH:
			case MOVE_TARGET_OPPONENTS_FIELD:
			case MOVE_TARGET_OPPONENTS:
                bitMask = (gBitTable[GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)] | gBitTable[GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT)]) << 16; 
				startY = 8;
				break;
			case MOVE_TARGET_USER:
			    if (!gBattleMoves[move].flags.affectsUserSide)
				{
					bitMask = (gBitTable[battlerId]) << 16;
					startY = 8;
					break;
				}
				// fallthrough
			case MOVE_TARGET_USER_OR_ALLY:
			    bitMask = (gBitTable[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)] | gBitTable[GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)]) << 16;
				startY = 8;
				break;
			case MOVE_TARGET_ALLY:
			    bitMask = (gBitTable[GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battlerId)))]) << 16;
			    startY = 8;
				break;
			case MOVE_TARGET_FOES_AND_ALLY:
				bitMask = (gBitTable[GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)] | gBitTable[GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battlerId)))] 
				| gBitTable[GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT)]) << 16;
				startY = 8;
				break;
        }
        BeginNormalPaletteFade(bitMask, 8, startY, 0, RGB_WHITE);
    }
	return moveTarget;
}

void HandleInputChooseMove(u8 battlerId)
{
    u32 canSelectTarget = 0;
    struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct *)(&gBattleBufferA[battlerId][4]);
	u8 i, moveTarget = PreviewDeterminativeMoveTargets(battlerId, moveInfo);
	
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
		ShowOrHideMoveInfoTriggerSprite(TRUE); // Hide trigger
		
        if (moveTarget == MOVE_TARGET_USER || moveTarget == MOVE_TARGET_ALL_BATTLERS || moveTarget == MOVE_TARGET_USER_OR_ALLY)
            gMultiUsePlayerCursor = battlerId;
        else
            gMultiUsePlayerCursor = GetBattlerAtPosition(BATTLE_OPPOSITE(GetBattlerPosition(battlerId) & BIT_SIDE));

        if (gBattleBufferA[battlerId][1]) // double battle
        {
            if (moveTarget == MOVE_TARGET_SELECTED)
                canSelectTarget = 1; // either selected or user
			
			if (moveTarget == MOVE_TARGET_USER_OR_ALLY && IsBattlerAlive(BATTLE_PARTNER(battlerId)))
				canSelectTarget = 1;
			
            if (!moveInfo->currentPp[gMoveSelectionCursor[battlerId]])
                canSelectTarget = 0;
            else if (moveTarget != MOVE_TARGET_USER && moveTarget != MOVE_TARGET_ALL_BATTLERS && moveTarget != MOVE_TARGET_USER_OR_ALLY
			&& CountAliveMonsInBattle(battlerId, BATTLE_ALIVE_EXCEPT_BATTLER) <= 1)
            {
				gMultiUsePlayerCursor = GetDefaultMoveTarget(battlerId);
				canSelectTarget = 0;
            }
			
			if (moveTarget == MOVE_TARGET_ALL_BATTLERS)
			{
				for (i = 0; i < gBattlersCount; i++)
					TryShowAsTarget(i);
				
				canSelectTarget = 3;
			}
			else if (moveTarget == MOVE_TARGET_OPPONENTS_FIELD || moveTarget == MOVE_TARGET_BOTH || moveTarget == MOVE_TARGET_FOES_AND_ALLY
			|| moveTarget == MOVE_TARGET_OPPONENTS)
			{
				TryShowAsTarget(gMultiUsePlayerCursor);
                TryShowAsTarget(BATTLE_PARTNER(gMultiUsePlayerCursor));
				
				if (moveTarget == MOVE_TARGET_FOES_AND_ALLY)
					TryShowAsTarget(BATTLE_PARTNER(battlerId));
				
				canSelectTarget = 2;
			}
        }
        ResetPaletteFade();
        BeginNormalPaletteFade(0xF0000, 0, 0, 0, RGB_WHITE);
		
		switch (canSelectTarget)
		{
			case 1:
			    gBattlerControllerFuncs[battlerId] = HandleInputChooseTarget;
				
				if (moveTarget == MOVE_TARGET_USER || moveTarget == MOVE_TARGET_ALL_BATTLERS || moveTarget == MOVE_TARGET_USER_OR_ALLY)
					gMultiUsePlayerCursor = battlerId;
				else if (gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)])
					gMultiUsePlayerCursor = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
				else
					gMultiUsePlayerCursor = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
				
				gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = SpriteCb_ShowAsMoveTarget;
				break;
			case 2:
			    gBattlerControllerFuncs[battlerId] = HandleInputShowTargets;
				break;
			case 3:
			    gBattlerControllerFuncs[battlerId] = HandleInputShowEntireFieldTargets;
				break;
			default:
			    BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, gMoveSelectionCursor[battlerId] | (gMultiUsePlayerCursor << 8));
				BattleControllerComplete(battlerId);
				break;
		}
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
		ShowOrHideMoveInfoTriggerSprite(TRUE); // Hide trigger
        BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, 0xFFFF);
        BattleControllerComplete(battlerId);
        ResetPaletteFade();
        BeginNormalPaletteFade(0xF0000, 0, 0, 0, RGB_WHITE);
    }
    else if (JOY_NEW(DPAD_LEFT))
    {
        if (gMoveSelectionCursor[battlerId] & 1)
        {
			PlaySE(SE_SELECT);
            MoveSelectionDestroyCursorAt(gMoveSelectionCursor[battlerId]);
            gMoveSelectionCursor[battlerId] ^= 1;
            MoveSelectionCreateCursorAt(gMoveSelectionCursor[battlerId], 0);
            MoveSelectionDisplayPpNumber(battlerId);
            MoveSelectionDisplayMoveType(battlerId);
            BeginNormalPaletteFade(0xF0000, 0, 0, 0, RGB_WHITE);
        }
    }
    else if (JOY_NEW(DPAD_RIGHT))
    {
        if (!(gMoveSelectionCursor[battlerId] & 1) && (gMoveSelectionCursor[battlerId] ^ 1) < gNumberOfMovesToChoose)
        {
			PlaySE(SE_SELECT);
            MoveSelectionDestroyCursorAt(gMoveSelectionCursor[battlerId]);
            gMoveSelectionCursor[battlerId] ^= 1;
            MoveSelectionCreateCursorAt(gMoveSelectionCursor[battlerId], 0);
            MoveSelectionDisplayPpNumber(battlerId);
            MoveSelectionDisplayMoveType(battlerId);
            BeginNormalPaletteFade(0xF0000, 0, 0, 0, RGB_WHITE);
        }
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if (gMoveSelectionCursor[battlerId] & 2)
        {
			PlaySE(SE_SELECT);
            MoveSelectionDestroyCursorAt(gMoveSelectionCursor[battlerId]);
            gMoveSelectionCursor[battlerId] ^= 2;
            MoveSelectionCreateCursorAt(gMoveSelectionCursor[battlerId], 0);
            MoveSelectionDisplayPpNumber(battlerId);
            MoveSelectionDisplayMoveType(battlerId);
            BeginNormalPaletteFade(0xF0000, 0, 0, 0, RGB_WHITE);
        }
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (!(gMoveSelectionCursor[battlerId] & 2) && (gMoveSelectionCursor[battlerId] ^ 2) < gNumberOfMovesToChoose)
        {
			PlaySE(SE_SELECT);
            MoveSelectionDestroyCursorAt(gMoveSelectionCursor[battlerId]);
            gMoveSelectionCursor[battlerId] ^= 2;
            MoveSelectionCreateCursorAt(gMoveSelectionCursor[battlerId], 0);
            MoveSelectionDisplayPpNumber(battlerId);
            MoveSelectionDisplayMoveType(battlerId);
            BeginNormalPaletteFade(0xF0000, 0, 0, 0, RGB_WHITE);
        }
    }
    else if (JOY_NEW(SELECT_BUTTON))
    {
        if (gNumberOfMovesToChoose > 1 && !(gBattleTypeFlags & BATTLE_TYPE_LINK))
        {
            MoveSelectionCreateCursorAt(gMoveSelectionCursor[battlerId], 29);
			ShowOrHideMoveInfoTriggerSprite(TRUE); // Hide trigger sprite
			gMultiUsePlayerCursor = gMoveSelectionCursor[battlerId] != 0 ? 0 : gMoveSelectionCursor[battlerId] + 1;
            MoveSelectionCreateCursorAt(gMultiUsePlayerCursor, 27);
            BattlePutTextOnWindow(gText_BattleSwitchWhich, B_WIN_SWITCH_PROMPT);
            gBattlerControllerFuncs[battlerId] = HandleMoveSwitching;
        }
    }
#if BATTLE_MOVE_INFO
	else if (JOY_NEW(L_BUTTON))
	{
		PlaySE(SE_SELECT);
		MoveSelectionDestroyCursorAt(gMoveSelectionCursor[battlerId]);
		ShowOrHideMoveInfoTriggerSprite(TRUE); // Hide trigger sprite
		gBattleStruct->moveInfo.submenuState = 0; // Always initialize on first submenu
		MoveInfoPrintMoveNameAndDescription(battlerId);
		MoveInfoPrintSubmenuString(battlerId, gBattleStruct->moveInfo.submenuState);
		gBattlerControllerFuncs[battlerId] = HandleInputMoveInfo;
	}
#endif
}

////////////////////////////
// TARGET SELECTION INPUT //
////////////////////////////

static void TryShowAsTarget(u8 battlerId)
{
	if (IsBattlerAlive(battlerId))
	{
		DoBounceEffect(battlerId, BOUNCE_HEALTHBOX, 15, 1);
		gSprites[gBattlerSpriteIds[battlerId]].callback = SpriteCb_ShowAsMoveTarget;
	}
}

static void HideAllTargets(u8 battlerId, bool8 checkBattler)
{
	u8 i;
	
	for (i = 0; i < MAX_BATTLERS_COUNT; i++)
	{
		if (IsBattlerAlive(i) && gBattleSpritesDataPtr->healthBoxesData[i].healthboxIsBouncing)
		{
			if (checkBattler && i == battlerId)
				continue;
			
			gSprites[gBattlerSpriteIds[i]].callback = SpriteCb_HideAsMoveTarget;
			EndBounceEffect(i, BOUNCE_HEALTHBOX);
		}
	}
}

static void HandleInputShowTargets(u8 battlerId)
{
	if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        HideAllTargets(battlerId, TRUE);
		BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, gMoveSelectionCursor[battlerId] | (gMultiUsePlayerCursor << 8));
        BattleControllerComplete(battlerId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        HideAllTargets(battlerId, TRUE);
		SetPlayerChooseMoveInput(battlerId);
		MoveSelectionDisplayMoveType(battlerId);
        DoBounceEffect(battlerId, BOUNCE_HEALTHBOX, 7, 1);
        DoBounceEffect(battlerId, BOUNCE_MON, 7, 1);
    }
}

static void HandleInputShowEntireFieldTargets(u8 battlerId)
{
	if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        HideAllTargets(battlerId, FALSE);
		BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, gMoveSelectionCursor[battlerId] | (gMultiUsePlayerCursor << 8));
        BattleControllerComplete(battlerId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        HideAllTargets(battlerId, FALSE);
        SetPlayerChooseMoveInput(battlerId);
		MoveSelectionDisplayMoveType(battlerId);
        DoBounceEffect(battlerId, BOUNCE_HEALTHBOX, 7, 1);
        DoBounceEffect(battlerId, BOUNCE_MON, 7, 1);
    }
}

static void HandleInputChooseTarget(u8 battlerId)
{
	u8 currSelIdentity, identities[MAX_BATTLERS_COUNT] = {B_POSITION_PLAYER_LEFT, B_POSITION_PLAYER_RIGHT, B_POSITION_OPPONENT_RIGHT, B_POSITION_OPPONENT_LEFT};
	struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct *)(&gBattleBufferA[battlerId][4]);
	u8 moveTarget = GetBattlerMoveTargetType(battlerId, moveInfo->moves[gMoveSelectionCursor[battlerId]]);
	s32 i;

    DoBounceEffect(gMultiUsePlayerCursor, BOUNCE_HEALTHBOX, 15, 1);
	
	for (i = 0; i < gBattlersCount; i++)
	{
		if (i != gMultiUsePlayerCursor)
			EndBounceEffect(i, BOUNCE_HEALTHBOX);
	}
	
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = SpriteCb_HideAsMoveTarget;
        BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, gMoveSelectionCursor[battlerId] | (gMultiUsePlayerCursor << 8));
        EndBounceEffect(gMultiUsePlayerCursor, BOUNCE_HEALTHBOX);
        BattleControllerComplete(battlerId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = SpriteCb_HideAsMoveTarget;
        SetPlayerChooseMoveInput(battlerId);
        DoBounceEffect(battlerId, BOUNCE_HEALTHBOX, 7, 1);
        DoBounceEffect(battlerId, BOUNCE_MON, 7, 1);
        EndBounceEffect(gMultiUsePlayerCursor, BOUNCE_HEALTHBOX);
        MoveSelectionDisplayMoveType(battlerId);
    }
    else if (JOY_NEW(DPAD_LEFT | DPAD_UP))
    {
        PlaySE(SE_SELECT);
        gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = SpriteCb_HideAsMoveTarget;
		
		if (moveTarget == MOVE_TARGET_USER_OR_ALLY)
			gMultiUsePlayerCursor = BATTLE_PARTNER(gMultiUsePlayerCursor);
		else
		{
			do
			{
				currSelIdentity = GetBattlerPosition(gMultiUsePlayerCursor);
				
				for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
				{
					if (currSelIdentity == identities[i])
						break;
				}
				do
				{
					if (--i < 0)
						i = MAX_BATTLERS_COUNT - 1;
					
					gMultiUsePlayerCursor = GetBattlerAtPosition(identities[i]);
					
				} while (gMultiUsePlayerCursor == gBattlersCount);
				
				i = 0;
				
				switch (GetBattlerPosition(gMultiUsePlayerCursor))
				{
					case B_POSITION_PLAYER_LEFT:
					case B_POSITION_PLAYER_RIGHT:
						if (battlerId != gMultiUsePlayerCursor)
							++i;
						break;
					default:
					    ++i;
						break;
				}
				if (gAbsentBattlerFlags & gBitTable[gMultiUsePlayerCursor])
					i = 0;
				
			} while (i == 0);
		}
        gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = SpriteCb_ShowAsMoveTarget;
        MoveSelectionDisplayMoveType(battlerId);
    }
    else if (JOY_NEW(DPAD_RIGHT | DPAD_DOWN))
    {
        PlaySE(SE_SELECT);
        gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = SpriteCb_HideAsMoveTarget;
		
		if (moveTarget == MOVE_TARGET_USER_OR_ALLY)
			gMultiUsePlayerCursor = BATTLE_PARTNER(gMultiUsePlayerCursor);
		else
		{
			do
			{
				currSelIdentity = GetBattlerPosition(gMultiUsePlayerCursor);
				
				for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
				{
					if (currSelIdentity == identities[i])
						break;
				}
				do
				{
					if (++i > 3)
						i = 0;
					
					gMultiUsePlayerCursor = GetBattlerAtPosition(identities[i]);
					
				} while (gMultiUsePlayerCursor == gBattlersCount);
				
				i = 0;
				
				switch (GetBattlerPosition(gMultiUsePlayerCursor))
				{
					case B_POSITION_PLAYER_LEFT:
					case B_POSITION_PLAYER_RIGHT:
					    if (battlerId != gMultiUsePlayerCursor)
							++i;
						break;
					default:
					    ++i;
						break;
				}
				if (gAbsentBattlerFlags & gBitTable[gMultiUsePlayerCursor])
					i = 0;
				
			} while (i == 0);
		}
        gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = SpriteCb_ShowAsMoveTarget;
        MoveSelectionDisplayMoveType(battlerId);
    }
}

//////////////////////////
// MOVE SWITCHING INPUT //
//////////////////////////

static void HandleMoveSwitching(u8 battlerId)
{
	s32 i;
	u8 totalPPBonuses, perMovePPBonuses[MAX_MON_MOVES];
    struct ChooseMoveStruct moveStruct;
	struct Pokemon *mon;

    if (JOY_NEW(A_BUTTON | SELECT_BUTTON))
    {
        PlaySE(SE_SELECT);

        if (gMoveSelectionCursor[battlerId] != gMultiUsePlayerCursor)
        {
			struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct *)(&gBattleBufferA[battlerId][4]);
			mon = GetBattlerPartyIndexPtr(battlerId);

            // swap moves and pp
            i = moveInfo->moves[gMoveSelectionCursor[battlerId]];
            moveInfo->moves[gMoveSelectionCursor[battlerId]] = moveInfo->moves[gMultiUsePlayerCursor];
            moveInfo->moves[gMultiUsePlayerCursor] = i;
            i = moveInfo->currentPp[gMoveSelectionCursor[battlerId]];
            moveInfo->currentPp[gMoveSelectionCursor[battlerId]] = moveInfo->currentPp[gMultiUsePlayerCursor];
            moveInfo->currentPp[gMultiUsePlayerCursor] = i;
            i = moveInfo->maxPp[gMoveSelectionCursor[battlerId]];
            moveInfo->maxPp[gMoveSelectionCursor[battlerId]] = moveInfo->maxPp[gMultiUsePlayerCursor];
            moveInfo->maxPp[gMultiUsePlayerCursor] = i;
            if (gDisableStructs[battlerId].mimickedMoves & gBitTable[gMoveSelectionCursor[battlerId]])
            {
                gDisableStructs[battlerId].mimickedMoves &= ~(gBitTable[gMoveSelectionCursor[battlerId]]);
                gDisableStructs[battlerId].mimickedMoves |= gBitTable[gMultiUsePlayerCursor];
            }
            MoveSelectionDisplayMoveNames(battlerId);
            for (i = 0; i < MAX_MON_MOVES; ++i)
                perMovePPBonuses[i] = (gBattleMons[battlerId].ppBonuses & (3 << (i * 2))) >> (i * 2);
            totalPPBonuses = perMovePPBonuses[gMoveSelectionCursor[battlerId]];
            perMovePPBonuses[gMoveSelectionCursor[battlerId]] = perMovePPBonuses[gMultiUsePlayerCursor];
            perMovePPBonuses[gMultiUsePlayerCursor] = totalPPBonuses;
            totalPPBonuses = 0;
            for (i = 0; i < MAX_MON_MOVES; ++i)
                totalPPBonuses |= perMovePPBonuses[i] << (i * 2);

            gBattleMons[battlerId].ppBonuses = totalPPBonuses;
            for (i = 0; i < MAX_MON_MOVES; ++i)
            {
                gBattleMons[battlerId].moves[i] = moveInfo->moves[i];
                gBattleMons[battlerId].pp[i] = moveInfo->currentPp[i];
            }
            if (!(gBattleMons[battlerId].status2 & STATUS2_TRANSFORMED))
            {
                for (i = 0; i < MAX_MON_MOVES; ++i)
                {
                    moveStruct.moves[i] = GetMonData(mon, MON_DATA_MOVE1 + i);
                    moveStruct.currentPp[i] = GetMonData(mon, MON_DATA_PP1 + i);
                }

                totalPPBonuses = GetMonData(mon, MON_DATA_PP_BONUSES);
                for (i = 0; i < MAX_MON_MOVES; ++i)
                    perMovePPBonuses[i] = (totalPPBonuses & (3 << (i * 2))) >> (i * 2);
                i = moveStruct.moves[gMoveSelectionCursor[battlerId]];
                moveStruct.moves[gMoveSelectionCursor[battlerId]] = moveStruct.moves[gMultiUsePlayerCursor];
                moveStruct.moves[gMultiUsePlayerCursor] = i;
                i = moveStruct.currentPp[gMoveSelectionCursor[battlerId]];
                moveStruct.currentPp[gMoveSelectionCursor[battlerId]] = moveStruct.currentPp[gMultiUsePlayerCursor];
                moveStruct.currentPp[gMultiUsePlayerCursor] = i;
                totalPPBonuses = perMovePPBonuses[gMoveSelectionCursor[battlerId]];
                perMovePPBonuses[gMoveSelectionCursor[battlerId]] = perMovePPBonuses[gMultiUsePlayerCursor];
                perMovePPBonuses[gMultiUsePlayerCursor] = totalPPBonuses;
                totalPPBonuses = 0;
                for (i = 0; i < MAX_MON_MOVES; ++i)
                    totalPPBonuses |= perMovePPBonuses[i] << (i * 2);
                for (i = 0; i < MAX_MON_MOVES; ++i)
                {
                    SetMonData(mon, MON_DATA_MOVE1 + i, &moveStruct.moves[i]);
                    SetMonData(mon, MON_DATA_PP1 + i, &moveStruct.currentPp[i]);
                }
                SetMonData(mon, MON_DATA_PP_BONUSES, &totalPPBonuses);
            }
        }
        if (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE)
            gBattlerControllerFuncs[battlerId] = OakOldManHandleInputChooseMove;
        else
            SetPlayerChooseMoveInput(battlerId);
		
        gMoveSelectionCursor[battlerId] = gMultiUsePlayerCursor;
        MoveSelectionCreateCursorAt(gMoveSelectionCursor[battlerId], 0);
		MoveSelectionDisplayMoveMenu(battlerId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        MoveSelectionDestroyCursorAt(gMultiUsePlayerCursor);
        MoveSelectionCreateCursorAt(gMoveSelectionCursor[battlerId], 0);
		
        if (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE)
            gBattlerControllerFuncs[battlerId] = OakOldManHandleInputChooseMove;
        else
            SetPlayerChooseMoveInput(battlerId);
		
		MoveSelectionDisplayMoveMenu(battlerId);
    }
    else if (JOY_NEW(DPAD_LEFT))
    {
        if (gMultiUsePlayerCursor & 1)
        {
			PlaySE(SE_SELECT);
			
            if (gMultiUsePlayerCursor == gMoveSelectionCursor[battlerId])
                MoveSelectionCreateCursorAt(gMoveSelectionCursor[battlerId], 29);
            else
                MoveSelectionDestroyCursorAt(gMultiUsePlayerCursor);
			
            gMultiUsePlayerCursor ^= 1;
			MoveSelectionCreateCursorAt(gMultiUsePlayerCursor, gMultiUsePlayerCursor == gMoveSelectionCursor[battlerId] ? 0 : 27);
        }
    }
    else if (JOY_NEW(DPAD_RIGHT))
    {
        if (!(gMultiUsePlayerCursor & 1) && (gMultiUsePlayerCursor ^ 1) < gNumberOfMovesToChoose)
        {
			PlaySE(SE_SELECT);
			
            if (gMultiUsePlayerCursor == gMoveSelectionCursor[battlerId])
                MoveSelectionCreateCursorAt(gMoveSelectionCursor[battlerId], 29);
            else
                MoveSelectionDestroyCursorAt(gMultiUsePlayerCursor);
			
            gMultiUsePlayerCursor ^= 1;
            MoveSelectionCreateCursorAt(gMultiUsePlayerCursor, gMultiUsePlayerCursor == gMoveSelectionCursor[battlerId] ? 0 : 27);
        }
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if (gMultiUsePlayerCursor & 2)
        {
			PlaySE(SE_SELECT);
			
            if (gMultiUsePlayerCursor == gMoveSelectionCursor[battlerId])
                MoveSelectionCreateCursorAt(gMoveSelectionCursor[battlerId], 29);
            else
                MoveSelectionDestroyCursorAt(gMultiUsePlayerCursor);
			
            gMultiUsePlayerCursor ^= 2;
			MoveSelectionCreateCursorAt(gMultiUsePlayerCursor, gMultiUsePlayerCursor == gMoveSelectionCursor[battlerId] ? 0 : 27);
        }
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (!(gMultiUsePlayerCursor & 2) && (gMultiUsePlayerCursor ^ 2) < gNumberOfMovesToChoose)
        {
			PlaySE(SE_SELECT);
			
            if (gMultiUsePlayerCursor == gMoveSelectionCursor[battlerId])
                MoveSelectionCreateCursorAt(gMoveSelectionCursor[battlerId], 29);
            else
                MoveSelectionDestroyCursorAt(gMultiUsePlayerCursor);
			
            gMultiUsePlayerCursor ^= 2;
			MoveSelectionCreateCursorAt(gMultiUsePlayerCursor, gMultiUsePlayerCursor == gMoveSelectionCursor[battlerId] ? 0 : 27);
        }
    }
}

/////////////////////
// MOVE INFO INPUT //
/////////////////////

static void (*const sMoveInfoSubmenuFuncs[NUM_MOVEINFO_SUBMENUS])(u16) =
{
	MoveInfoPrintPowerAndAccuracy,
	MoveInfoPrintPriorityAndCategory,
	MoveInfoPrintMoveTarget,
};

static void MoveInfoPrintMoveNameAndDescription(u8 battlerId)
{
	struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct *)(&gBattleBufferA[battlerId][4]);
	u16 move = moveInfo->moves[gMoveSelectionCursor[battlerId]];
	
	// Move's name and description
	ReformatStringToMaxChars(gMoveDescriptionPointers[move - 1], 0, 30, FALSE);
	CreateBattleMoveInfoWindowsAndArrows(move);
}

static void MoveInfoPrintPowerAndAccuracy(u16 move)
{
	// Move's power
	if (gBattleMoves[move].power <= 1)
		StringCopy(gBattleTextBuff1, gText_ThreeHyphens);
	else
		ConvertIntToDecimalStringN(gBattleTextBuff1, gBattleMoves[move].power, STR_CONV_MODE_LEFT_ALIGN, 3);
	
	// Move's accuracy
	if (gBattleMoves[move].accuracy == 0)
		StringCopy(gBattleTextBuff2, gText_ThreeHyphens);
	else
		ConvertIntToDecimalStringN(gBattleTextBuff2, gBattleMoves[move].accuracy, STR_CONV_MODE_LEFT_ALIGN, 3);
	
	BattleStringExpandPlaceholdersToDisplayedString(gText_MoveInfoPowerAndAccuracy);
}

static void MoveInfoPrintPriorityAndCategory(u16 move)
{
	// Move's priority
	ConvertIntToDecimalStringN(gBattleTextBuff1, gBattleMoves[move].priority, STR_CONV_MODE_LEFT_ALIGN, 2);
	
	// Move's category
	StringCopy(gBattleTextBuff2, gCategoryNames[gBattleMoves[move].split]);
	
	BattleStringExpandPlaceholdersToDisplayedString(gText_MoveInfoPriorityAndCategory);
}

static void MoveInfoPrintMoveTarget(u16 move)
{
	// Move's target
	CopyMoveTargetName(gBattleTextBuff1, move);
	
	BattleStringExpandPlaceholdersToDisplayedString(gText_MoveInfoTarget);
}

static void MoveInfoPrintSubmenuString(u8 battlerId, u8 stateId)
{
	struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct *)(&gBattleBufferA[battlerId][4]);
	sMoveInfoSubmenuFuncs[stateId](moveInfo->moves[gMoveSelectionCursor[battlerId]]);
	BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_SWITCH_PROMPT);
}

// Redrawn moves window
static void HandleCloseMoveInfo_Step(u8 battlerId)
{
	DestroyBattleMoveInfoWindows();
	InitMoveSelectionsVarsAndStrings(battlerId);
	SetPlayerChooseMoveInput(battlerId);
}

static void HandleInputMoveInfo(u8 battlerId)
{
	if (JOY_NEW(A_BUTTON | B_BUTTON | L_BUTTON))
	{
		PlaySE(SE_SELECT);
		// To create a smooth animation when closing the move info, first the arrow pair is removed, and then in the next frame, the moves window is redrawn.
		RemoveScrollIndicatorArrowPair(gBattleStruct->moveInfo.arrowTaskId);
		gBattlerControllerFuncs[battlerId] = HandleCloseMoveInfo_Step;
	}
	else if (JOY_NEW(DPAD_LEFT) && gBattleStruct->moveInfo.submenuState > 0)
	{
		PlaySE(SE_SELECT);
		MoveInfoPrintSubmenuString(battlerId, --gBattleStruct->moveInfo.submenuState);
	}
	else if (JOY_NEW(DPAD_RIGHT) && gBattleStruct->moveInfo.submenuState < (NUM_MOVEINFO_SUBMENUS - 1))
	{
		PlaySE(SE_SELECT);
		MoveInfoPrintSubmenuString(battlerId, ++gBattleStruct->moveInfo.submenuState);
	}
}

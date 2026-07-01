#include "global.h"
#include "gflib.h"
#include "battle_damage_calc.h"
#include "data.h"
#include "item.h"
#include "item_menu.h"
#include "link.h"
#include "form_change.h"
#include "battle_move_effects.h"
#include "m4a.h"
#include "party_menu.h"
#include "battle_gfx_sfx_util.h"
#include "pokeball.h"
#include "battle_main.h"
#include "battle_ai_util.h"
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

static void PlayerBufferRunCommand(u32 battlerId);
static void PlayerBufferExecCompleted(u32 battlerId);
static void PlayerHandleLoadMonSprite(u32 battlerId);
static void PlayerHandleTrainerSlide(u32 battlerId);
static void PlayerHandleTrainerSlideBack(u32 battlerId);
static void PlayerHandleTwoReturnValues(u32 battlerId);
static void PlayerHandleChosenMonReturnValue(u32 battlerId);
static void PlayerHandleOneReturnValue(u32 battlerId);
static void PlayerHandleIntroTrainerBallThrow(u32 battlerId);
static void PlayerHandleResetActionMoveSelection(u32 battlerId);
static void HandleInputChooseAction(u32 battlerId);
static void BuildPlayerChooseMoveStruct(u32 battlerId);
static void SetPlayerChooseMoveInput(u32 battlerId);
static void HandleInputChooseMove(u32 battlerId);
static void HandleInputChooseTarget(u32 battlerId);
static void HandleInputMoveInfo(u32 battlerId);
static void TryShowAsTarget(u32 battlerId);
static void HandleInputShowTargets(u32 battlerId);
static void HandleInputShowEntireFieldTargets(u32 battlerId);
static void MoveInfoPrintMoveNameAndDescription(u32 battlerId);
static void MoveInfoPrintSubmenuString(u32 battlerId, u32 stateId);
static void MoveInfoPrintPowerAndAccuracy(u32 moveSlot);
static void MoveInfoPrintPriorityAndCategory(u32 moveSlot);
static void MoveInfoPrintMoveTarget(u32 moveSlot);
static void ChangeBattlerSpritesInvisibilities(bool32 invisible);
static void HandleInputTeamPreview(u32 battlerId);
static u32 GetPrevBall(u32 ballId);
static u32 GetNextBall(u32 ballId);

static EWRAM_DATA struct ChooseMoveStruct sChooseMoveStructOverride = {0};

static void (*const sPlayerBufferCommands[CONTROLLER_CMDS_COUNT])(u32) =
{
    [CONTROLLER_GETMONDATA]               = BtlController_HandleGetMonData,
    [CONTROLLER_SETMONDATA]               = BtlController_HandleSetMonData,
    [CONTROLLER_LOADMONSPRITE]            = PlayerHandleLoadMonSprite,
    [CONTROLLER_SWITCHINANIM]             = PlayerHandleSwitchInAnim,
    [CONTROLLER_RETURNMONTOBALL]          = BtlController_HandleReturnMonToBall,
    [CONTROLLER_DRAWTRAINERPIC]           = PlayerHandleDrawTrainerPic,
    [CONTROLLER_TRAINERSLIDE]             = PlayerHandleTrainerSlide,
    [CONTROLLER_TRAINERSLIDEBACK]         = PlayerHandleTrainerSlideBack,
    [CONTROLLER_FAINTANIMATION]           = BtlController_HandleFaintAnimation,
    [CONTROLLER_BALLTHROWANIM]            = PlayerHandleBallThrowAnim,
    [CONTROLLER_MOVEANIMATION]            = BtlController_HandleMoveAnimation,
    [CONTROLLER_PRINTSTRING]              = BtlController_HandlePrintString,
    [CONTROLLER_PRINTSELECTIONSTRING]     = BtlController_HandlePrintSelectionString,
    [CONTROLLER_CHOOSEACTION]             = PlayerHandleChooseAction,
    [CONTROLLER_CHOOSEMOVE]               = PlayerHandleChooseMove,
    [CONTROLLER_OPENBAG]                  = PlayerHandleChooseItem,
    [CONTROLLER_CHOOSEPOKEMON]            = PlayerHandleChoosePokemon,
    [CONTROLLER_HEALTHBARUPDATE]          = BtlController_HandleHealthbarUpdateWithHpText,
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
    [CONTROLLER_GIMMICKSTATE]             = PlayerHandleGimmickState,
    [CONTROLLER_HEALTHBOXUPDATE]          = BtlController_HandleHealthboxUpdate,
    [CONTROLLER_HIDEALLHEALTHBOXES]       = BtlController_HandleHideAllHealthboxes,
    [CONTROLLER_BATTLEFORMCHANGE]         = BtlController_HandleBattleFormChange,
    [CONTROLLER_PARTYFORMCHANGE]          = BtlController_HandlePartyFormChange,
    [CONTROLLER_ISPOCKETNOTEMPTY]         = BtlController_HandleIsPocketNotEmpty,
    [CONTROLLER_YESNOBOX]                 = BtlController_HandleYesNoBox,
    [CONTROLLER_MONCAUGHTEFFECTS]         = PlayerHandleMonCaughtEffects,
    [CONTROLLER_TERMINATOR_NOP]           = ControllerDummy,
};

void SetControllerToPlayer(u32 battlerId)
{
    gBattlerControllersData[battlerId].func = PlayerBufferRunCommand;
    gBattlerControllersData[battlerId].endFunc = PlayerBufferExecCompleted;
    gDoingBattleAnim = FALSE;
}

static void PlayerBufferRunCommand(u32 battlerId)
{
    if (gBattleControllerExecFlags & Bit(battlerId))
    {
        if (gBattleBufferA[battlerId][0] < ARRAY_COUNT(sPlayerBufferCommands))
            sPlayerBufferCommands[gBattleBufferA[battlerId][0]](battlerId);
        else
            PlayerBufferExecCompleted(battlerId);
    }
}

static void PlayerBufferExecCompleted(u32 battlerId)
{
    gBattlerControllersData[battlerId].func = PlayerBufferRunCommand;
    
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        u8 playerId = GetMultiplayerId();
        PrepareBufferDataTransferLink(battlerId, BUFFER_LINK, 4, &playerId);
        gBattleBufferA[battlerId][0] = CONTROLLER_TERMINATOR_NOP;
    }
    else
        gBattleControllerExecFlags &= ~(Bit(battlerId));
}

////////////////////////
// BATTLE CONTROLLERS //
////////////////////////

static void PlayerHandleLoadMonSprite(u32 battlerId)
{
    BattleLoadMonSpriteGfx(battlerId);
    gSprites[gBattlerSpriteIds[battlerId]].oam.paletteNum = battlerId;
    gBattlerControllersData[battlerId].func = CompleteOnBattlerSpritePosX_0;
}

void PlayerHandleSwitchInAnim(u32 battlerId)
{
    BtlController_HandleSwitchInAnim(battlerId, SwitchIn_TryShinyAnimShowHealthbox);
    gActionSelectionCursor[battlerId] = 0;
    
    if (!(gBattleBufferA[battlerId][2] & SWITCHIN_ANIM_DONT_RESET_MOVE_CURSOR))
        gBattleStruct->battlers[battlerId].moveSelectionCursor = 0;
}

u32 GetPlayerTrainerPicId(u32 multiplayerId)
{
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        if (IsLinkPlayerFromHoenn(multiplayerId))
            return gLinkPlayers[multiplayerId].gender + TRAINER_BACK_PIC_RS_BRENDAN;
        else
            return gLinkPlayers[multiplayerId].gender + TRAINER_BACK_PIC_RED;
    }
    else
        return gSaveBlock2Ptr->playerGender + TRAINER_BACK_PIC_RED;
}

void PlayerHandleDrawTrainerPic(u32 battlerId)
{
    BtlController_HandleDrawTrainerPic(battlerId, GetPlayerTrainerPicId(GetMultiplayerId()));
}

static void PlayerHandleTrainerSlide(u32 battlerId)
{
    BtlController_HandleTrainerSlide(battlerId, GetPlayerTrainerPicId(GetMultiplayerId()));
}

static void PlayerHandleTrainerSlideBack(u32 battlerId)
{
    BtlController_HandleTrainerSlideBack(battlerId, 50, TRUE);
}

void PlayerHandleBallThrowAnim(u32 battlerId)
{
    BtlController_HandleBallThrowAnim(battlerId, B_ANIM_BALL_THROW, !(gBattleTypeFlags & BATTLE_TYPE_POKEDUDE));
}

static void HandleChooseActionAfterDma3(u32 battlerId)
{
    if (!IsDma3ManagerBusyWithBgCopy())
    {
        gBattle_BG0_X = 0;
        gBattle_BG0_Y = 160;

        if (gBattleTypeFlags & BATTLE_TYPE_OLD_MAN_TUTORIAL)
            gBattlerControllersData[battlerId].func = OakOldMan_SimulateInputChooseAction;
        else if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
            gBattlerControllersData[battlerId].func = Pokedude_SimulateInputChooseAction;
        else
        {
#if WEATHER_ICON_IN_BATTLE
            TryCreateWeatherAnimIcon();
#endif

#if BATTLE_TEAM_PREVIEW
            TryLoadTeamPreviewTrigger();
#endif

#if LAST_USED_BALL_THROW
            TryAddLastUsedBallTrigger();
#endif

            gBattlerControllersData[battlerId].func = HandleInputChooseAction;
        }
    }
}

void PlayerHandleChooseAction(u32 battlerId)
{
    const u8 *actionsText;
    
    if (gBattleTypeFlags & BATTLE_TYPE_OLD_MAN_TUTORIAL)
        StringCopy(gBattleTextBuff1, COMPOUND_STRING("the old man"));
    else
        PrepareMonNickNoIllusionBuffer(gBattleTextBuff1, battlerId, gBattlerPartyIndexes[battlerId]);
    
    if (gBattleTypeFlags & BATTLE_TYPE_RAID)
        actionsText = gText_RaidMenu;
    else
    {
        if (IsPlayerBagDisabled())
            actionsText = gText_BattleMenuBagDisabled;
        else
            actionsText = gText_BattleMenu;
    }
    BtlController_HandleChooseAction(battlerId, actionsText, gText_WhatWillBufferDo, HandleChooseActionAfterDma3);
}

static void HandleChooseMoveAfterDma3(u32 battlerId)
{
    if (!IsDma3ManagerBusyWithBgCopy())
    {
        gBattle_BG0_X = 0;
        gBattle_BG0_Y = 320;
        
        SetPlayerChooseMoveInput(battlerId);
    }
}

void PlayerHandleChooseMove(u32 battlerId)
{
    BuildPlayerChooseMoveStruct(battlerId);
    
    InitMoveSelectionsVarsAndStrings(battlerId);
    gBattleStruct->playerSelectedGimmick = FALSE;
    
    if (!IsGimmickTriggerSpriteActive())
        gBattleStruct->gimmickTriggerSpriteId = 0xFF;
    
    CreateGimmickTriggerSprite(battlerId);

    gBattlerControllersData[battlerId].func = HandleChooseMoveAfterDma3;
}

static void CompleteWhenChooseItem(u32 battlerId)
{
    if (gMain.callback2 == BattleMainCB2 && !gPaletteFade.active)
    {
        if ((gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE) && !BtlCtrl_OakOldMan_TestState2Flag(FIRST_BATTLE_MSG_FLAG_HP_RESTORE) && gSpecialVar_ItemId == ITEM_POTION)
        {
            BtlCtrl_OakOldMan_SetState2Flag(FIRST_BATTLE_MSG_FLAG_HP_RESTORE);
            gBattlerControllersData[battlerId].func = PrintOakText_KeepAnEyeOnHP;
        }
        else
        {
            BtlController_EmitOneReturnValue(battlerId, BUFFER_B, gSpecialVar_ItemId);
            BattleControllerComplete(battlerId);
        }
    }
}

static void OpenBagAndChooseItem(u32 battlerId)
{
    if (!gPaletteFade.active)
    {
        gBattlerControllersData[battlerId].func = CompleteWhenChooseItem;
        FreeAllWindowBuffers();
        
        if (gBattleTypeFlags & BATTLE_TYPE_OLD_MAN_TUTORIAL)
            InitOldManBag();
        else if (gBattleTypeFlags & BATTLE_TYPE_RAID)
            CB2_ChooseBall();
        else
            CB2_BagMenuFromBattle();
    }
}

void PlayerHandleChooseItem(u32 battlerId)
{
    u32 i;
    
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
        BattlePutTextOnWindow(COMPOUND_STRING("{PAUSE 16}Link standby…"), B_WIN_MSG);
    }
}

static void Player_WaitForMonSelection(u32 battlerId)
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

static void OpenPartyMenuToChooseMon(u32 battlerId)
{
    if (!gPaletteFade.active)
    {
        u32 caseId;

        gBattlerControllersData[battlerId].func = Player_WaitForMonSelection;
        caseId = gTasks[gBattlerControllersData[battlerId].data].data[0];
        DestroyTask(gBattlerControllersData[battlerId].data);
        FreeAllWindowBuffers();
        OpenPartyMenuInTutorialBattle(caseId);
    }
}

void PlayerHandleChoosePokemon(u32 battlerId)
{
    BtlController_HandleChoosePokemon(battlerId, OpenPartyMenuToChooseMon);
}

static void PlayerHandleTwoReturnValues(u32 battlerId)
{
    BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, 0, 0);
    BattleControllerComplete(battlerId);
}

static void PlayerHandleChosenMonReturnValue(u32 battlerId)
{
    BtlController_EmitChosenMonReturnValue(battlerId, BUFFER_B, 0, NULL);
    BattleControllerComplete(battlerId);
}

static void PlayerHandleOneReturnValue(u32 battlerId)
{
    BtlController_EmitOneReturnValue(battlerId, BUFFER_B, 0);
    BattleControllerComplete(battlerId);
}

static void PlayerHandleIntroTrainerBallThrow(u32 battlerId)
{
    BtlController_HandleIntroTrainerBallThrow(battlerId, 0xD6F8, TRAINER_BACK_PIC_RED + gSaveBlock2Ptr->playerGender, PlayerThrowBall_StartAnimLinearTranslation, 31, Intro_TryShinyAnimShowHealthbox);
}

void PlayerHandleLinkStandbyMsg(u32 battlerId)
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

static void PlayerHandleResetActionMoveSelection(u32 battlerId)
{
    switch (gBattleBufferA[battlerId][1])
    {
    case RESET_ACTION_MOVE_SELECTION:
        gActionSelectionCursor[battlerId] = 0;
        gBattleStruct->battlers[battlerId].moveSelectionCursor = 0;
        break;
    case RESET_ACTION_SELECTION:
        gActionSelectionCursor[battlerId] = 0;
        break;
    case RESET_MOVE_SELECTION:
        gBattleStruct->battlers[battlerId].moveSelectionCursor = 0;
        break;
    }
    BattleControllerComplete(battlerId);
}

void PlayerHandleEndLinkBattle(u32 battlerId)
{
    BtlController_HandleEndLinkBattle(battlerId, gBattleBufferA[battlerId][1], SetBattleEndCallbacks);
}

void PlayerHandleGimmickState(u32 battlerId)
{
    BtlController_HandleGimmickState(battlerId, TRUE);
}

void PlayerHandleMonCaughtEffects(u32 battlerId)
{
    struct Pokemon *mon = GetBattlerPartyIndexPtr(gBattleBufferA[battlerId][1]);
    u32 data, pokeball = READ_16(&gBattleBufferA[battlerId][2]);
    
    SetMonData(mon, MON_DATA_POKEBALL, &pokeball);
    
    switch (pokeball)
    {
        case ITEM_HEAL_BALL:
            MonRestorePP(mon);
            HealStatusConditions(mon, STATUS1_NONE, MAX_BATTLERS_COUNT);
            
            data = GetMonData(mon, MON_DATA_MAX_HP);
            SetMonData(mon, MON_DATA_HP, &data);
            break;
        case ITEM_FRIEND_BALL:
            data = 150;
            SetMonData(mon, MON_DATA_FRIENDSHIP, &data);
            break;
    }
    gCapturedWildMon = TRUE;
    BattleControllerComplete(battlerId);
}

////////////////////////////
// ACTION SELECTION INPUT //
////////////////////////////

void ActionSelectionCreateCursorAt(u32 cursorPosition)
{
    u16 src[2] = {1, 2};
    CopyToBgTilemapBufferRect_ChangePalette(0, src, 7 * (cursorPosition & 1) + 16, 35 + (cursorPosition & 2), 1, 2, 0x11);
    CopyBgTilemapBufferToVram(0);
}

void ActionSelectionDestroyCursorAt(u32 cursorPosition)
{
    u16 src[2] = {32, 32};
    CopyToBgTilemapBufferRect_ChangePalette(0, src, 7 * (cursorPosition & 1) + 16, 35 + (cursorPosition & 2), 1, 2, 0x11);
    CopyBgTilemapBufferToVram(0);
}

static inline void TryHideAllTriggerSprites(void)
{
#if WEATHER_ICON_IN_BATTLE
    ShowOrHideWeatherAnimIcon(TRUE);
#endif

#if BATTLE_TEAM_PREVIEW
    ShowOrHideTeamPreviewTrigger(TRUE);
#endif

#if LAST_USED_BALL_THROW
    ShowOrHideLastUsedBall(TRUE);
#endif
}

static void HandleInputChooseAction(u32 battlerId)
{
    DoBounceEffect(battlerId, BOUNCE_HEALTHBOX, 7, 1);
    DoBounceEffect(battlerId, BOUNCE_MON, 7, 1);
    
#if LAST_USED_BALL_THROW
    if (gBattleStruct->lastUsedBall.ballSwapping)
        return; // Can't choose action while ball is swapping
    
    if (!gBattleStruct->lastUsedBall.menuPresent)
        gBattleStruct->lastUsedBall.ackBallUseBtn = FALSE;
    else if (JOY_NEW(L_BUTTON))
    {
        ChangeLastBallCycleArrowsColor(TRUE);
        gBattleStruct->lastUsedBall.ackBallUseBtn = TRUE;
    }
    
    if (gBattleStruct->lastUsedBall.ackBallUseBtn)
    {
        bool32 isSameBall, holdingButton = JOY_HELD(L_BUTTON);
        u32 newBall;
        
        if (holdingButton && JOY_NEW(DPAD_DOWN | DPAD_RIGHT))
        {
            newBall = GetNextBall(gBallToDisplay);
            
            if (newBall)
            {
                PlaySE(SE_SELECT);
                
                gBattleStruct->lastUsedBall.ballSwapping = TRUE;
                
                if (gBallToDisplay == newBall)
                    isSameBall = TRUE;
                else
                {
                    gBallToDisplay = newBall;
                    isSameBall = FALSE;
                }
                SwapBallToDisplay(isSameBall);
            }
        }
        else if (holdingButton && JOY_NEW(DPAD_UP | DPAD_LEFT))
        {
            newBall = GetPrevBall(gBallToDisplay);
                
            if (newBall)
            {
                PlaySE(SE_SELECT);
                gBattleStruct->lastUsedBall.ballSwapping = TRUE;
                
                if (gBallToDisplay == newBall)
                    isSameBall = TRUE;
                else
                {
                    gBallToDisplay = newBall;
                    isSameBall = FALSE;
                }
                SwapBallToDisplay(isSameBall);
            }
        }
        else if (holdingButton && JOY_NEW(B_BUTTON)) // Failsafe to cancel the action
        {
            gBattleStruct->lastUsedBall.ackBallUseBtn = FALSE;
            ChangeLastBallCycleArrowsColor(FALSE);
        }
        else if (!holdingButton && CanThrowLastUsedBall())
        {
            PlaySE(SE_SELECT);
            gBattleStruct->lastUsedBall.ackBallUseBtn = FALSE;
            ShowOrHideLastUsedBall(TRUE); // hide trigger
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_THROW_BALL, 0);
            BattleControllerComplete(battlerId);
        }
        return;
    }
#endif
    
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        TryHideAllTriggerSprites();

        switch (gActionSelectionCursor[battlerId])
        {
        case 0:
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_USE_MOVE, 0);
            break;
        case 1:
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, (gBattleTypeFlags & BATTLE_TYPE_RAID) ? B_ACTION_CHEER : B_ACTION_USE_ITEM, 0);
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
        if (IsDoubleBattleForBattler(battlerId) && GetBattlerPosition(battlerId) == B_POSITION_PLAYER_RIGHT && IsPartnerMonFromSameTrainer(battlerId)
        && IsBattlerAlive(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)))
        {
            // Return item to bag if partner had selected one.
            if (gBattleBufferA[battlerId][1] == B_ACTION_USE_ITEM)
                AddBagItem(gBattleBufferA[battlerId][2] | (gBattleBufferA[battlerId][3] << 8), 1);

            PlaySE(SE_SELECT);
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_CANCEL_PARTNER, 0);
            BattleControllerComplete(battlerId);
        }
    }
    else if (JOY_NEW(START_BUTTON))
        SwapHpBarsWithHpText();
#if BATTLE_TEAM_PREVIEW
    else if (JOY_NEW(R_BUTTON))
    {
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        {
            PlaySE(SE_SELECT);
            UpdateOamPriorityInAllHealthboxes(0, TRUE);
            ChangeBattlerSpritesInvisibilities(TRUE);
            DisplayInBattleTeamPreview();
            gBattlerControllersData[battlerId].func = HandleInputTeamPreview;
        }
    }
#endif
}

///////////////////////////////
// MOVE SELECTION DISPLAYERS //
///////////////////////////////

// Builds the player choose move struct, some gimmicks changes the move visuals like name.
static void BuildPlayerChooseMoveStruct(u32 battlerId)
{
    u32 i;

    memcpy(&sChooseMoveStructOverride, &gBattleBufferA[battlerId][4], sizeof(struct ChooseMoveStruct));
    
    if (IsGimmickActiveOrSelected(battlerId, GIMMICK_DYNAMAX))
    {
        for (i = 0; i < MAX_MON_MOVES; i++)
        {
            u16 *move = &sChooseMoveStructOverride.moves[i].move;
            u32 baseMove = *move;
            
            *move = GetMaxMove(battlerId, sChooseMoveStructOverride.species, *move, gBattleMoves[*move].type, sChooseMoveStructOverride.moves[i].split);
            
            // Max Guard is always a Normal type move
            if (*move == MOVE_MAX_GUARD)
                sChooseMoveStructOverride.moves[i].type = gBattleMoves[*move].type;
            else
                sChooseMoveStructOverride.moves[i].power = GetMaxMovePower(baseMove, *move, sChooseMoveStructOverride.moves[i].type);
            
            sChooseMoveStructOverride.moves[i].target = gBattleMoves[*move].target;
        }
    }
}

#if EFFECTIVENESS_ON_MENU
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
    // normal colors
    RGB(24, 24, 23),
    RGB(9, 9, 9),
    // normal colors + stab
    RGB(15, 15, 15),
    RGB_BLACK,
    // not affect colors
    RGB(30, 27, 19),
    RGB(28, 8, 8)
};
#endif

static void MoveSelectionDisplayMoveNames(u32 battlerId)
{
    u32 i, move;

    gNumberOfMovesToChoose = 0;

    for (i = 0; i < MAX_MON_MOVES; ++i)
    {
        move = sChooseMoveStructOverride.moves[i].move;
        
        MoveSelectionDestroyCursorAt(i);
        StringCopy(gDisplayedStringBattle, gText_MoveInterfaceMoveNamesColor);
        StringAppend(gDisplayedStringBattle, gBattleMoves[move].name);
        BattlePutTextOnWindow(gDisplayedStringBattle, i + B_WIN_MOVE_NAME_1);
        
        if (move)
            ++gNumberOfMovesToChoose;
    }
}

static void MoveSelectionDisplayPpString(u32 battlerId)
{
    u32 moveSlot = gBattleStruct->battlers[battlerId].moveSelectionCursor;
    u8 *txtPtr = StringCopy(gDisplayedStringBattle, gText_MoveInterfacePP);
    
    // Stellar boost indicator
    if (GetActiveGimmick(battlerId) == GIMMICK_TERA && GetBattlerTeraType(battlerId) == TYPE_STELLAR && sChooseMoveStructOverride.moves[moveSlot].split != SPLIT_STATUS
    && sChooseMoveStructOverride.moves[moveSlot].isStellarBoosted)
        StringCopy(txtPtr, COMPOUND_STRING("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}{UP_ARROW}"));
    
    BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_PP);
}

static void MoveSelectionDisplayPpNumber(u32 battlerId)
{
    u8 *txtPtr;
    u32 moveSlot, currentPp, maxPp;
    
    if (!gBattleBufferA[battlerId][2]) // check if we didn't want to display pp number
    {
        moveSlot = gBattleStruct->battlers[battlerId].moveSelectionCursor;
        currentPp = sChooseMoveStructOverride.moves[moveSlot].currentPp;
        maxPp = sChooseMoveStructOverride.moves[moveSlot].maxPp;
        
        SetPpNumbersPaletteInMoveSelection(currentPp, maxPp);
        txtPtr = ConvertIntToDecimalStringN(gDisplayedStringBattle, currentPp, STR_CONV_MODE_RIGHT_ALIGN, 2);
        *txtPtr = CHAR_SLASH;
        ConvertIntToDecimalStringN(++txtPtr, maxPp, STR_CONV_MODE_RIGHT_ALIGN, 2);
        BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_PP_REMAINING);
    }
}

#undef IsBattlerOfType

// Use this instead of directly acessing the battler's data
#define _IS_BATTLER_ANY_TYPE_CHOOSING_MOVE(battlerId, ...)                                                                                    \
    ({                                                                                                                                        \
        u32 types[3] = {sChooseMoveStructOverride.monTypes[0], sChooseMoveStructOverride.monTypes[1], sChooseMoveStructOverride.monTypes[2]}; \
        RECURSIVELY(R_FOR_EACH(IS_BATTLER_ANY_TYPE_HELPER, __VA_ARGS__)) FALSE;                                                               \
    })

#define IsBattlerOfType(battler, ...) _IS_BATTLER_ANY_TYPE_CHOOSING_MOVE(battler, __VA_ARGS__)

static void MoveSelectionDisplayMoveType(u32 battlerId)
{
    u8 *txtPtr;
    u32 target, flags, effect;
    u32 type, moveSlot = gBattleStruct->battlers[battlerId].moveSelectionCursor;

#if BATTLE_MENU_REAL_MOVE_TYPE
    // Dynamax displays the original move's type
    if (!IsGimmickActiveOrSelected(battlerId, GIMMICK_DYNAMAX))
        type = sChooseMoveStructOverride.moves[moveSlot].type;
    else
#endif
    {
        type = gBattleMoves[sChooseMoveStructOverride.moves[moveSlot].move].type;
    }

    txtPtr = StringCopy(gDisplayedStringBattle, gText_MoveInterfaceType);
    
#if EFFECTIVENESS_ON_MENU
    // Choose move target
    if (gBattlerControllersData[battlerId].func == HandleInputChooseTarget)
        target = gMultiUsePlayerCursor;
    else
        target = BATTLE_OPPOSITE(battlerId);
    
    if (IsDoubleBattleForBattler(target) && !IsBattlerAlive(target))
        target = BATTLE_PARTNER(target);

    // set respective colours
    flags = sChooseMoveStructOverride.moves[moveSlot].effectivenessFlags[target];
    
    if (flags & MOVE_RESULT_NO_EFFECT)
    {
        gPlttBufferUnfaded[88] = sEffectivenessColours[12];
        gPlttBufferUnfaded[89] = sEffectivenessColours[13];
    }
    else
    {
        // check if move is stab
        if (sChooseMoveStructOverride.moves[moveSlot].split != SPLIT_STATUS && IsBattlerOfType(battlerId, type))
            effect = 2;
        else
            effect = 0;
        
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
        else
        {
            gPlttBufferUnfaded[88] = sEffectivenessColours[effect + 8];
            gPlttBufferUnfaded[89] = sEffectivenessColours[effect + 9];
        }
    }
    CpuCopy16(&gPlttBufferUnfaded[88], &gPlttBufferFaded[88], sizeof(u16));
    CpuCopy16(&gPlttBufferUnfaded[89], &gPlttBufferFaded[89], sizeof(u16));
    
    txtPtr = StringCopy(txtPtr, gText_MoveInterfaceEffectiveness);
#else
    txtPtr = StringCopy(txtPtr, gText_MoveInterfaceMoveNamesColor);
#endif
    PrependFontIdToFit(txtPtr, StringCopy(txtPtr, gTypesInfo[type].name), 0, WindowWidthPx(B_WIN_MOVE_TYPE) - 32);
    BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MOVE_TYPE);
}

static void MoveSelectionDisplayMoveMenu(u32 battlerId)
{
    MoveSelectionDisplayPpString(battlerId);
    MoveSelectionDisplayPpNumber(battlerId);
    MoveSelectionDisplayMoveType(battlerId);
}

void InitMoveSelectionsVarsAndStrings(u32 battlerId)
{
    MoveSelectionDisplayMoveNames(battlerId);
    gMultiUsePlayerCursor = 0xFF;
    MoveSelectionCreateCursorAt(gBattleStruct->battlers[battlerId].moveSelectionCursor, 0);
    MoveSelectionDisplayMoveMenu(battlerId);
}

static void MoveSelectionReloadMoveNames(u32 battlerId)
{
    MoveSelectionDisplayMoveNames(battlerId);
    MoveSelectionCreateCursorAt(gBattleStruct->battlers[battlerId].moveSelectionCursor, 0);
    MoveSelectionDisplayMoveType(battlerId);
}

//////////////////////////
// MOVE SELECTION INPUT //
//////////////////////////

static void SetPlayerChooseMoveInput(u32 battlerId)
{
    CreateMoveInfoTriggerSprite();
    gBattlerControllersData[battlerId].func = HandleInputChooseMove;
}

void MoveSelectionCreateCursorAt(u32 cursorPosition, u32 arg1)
{
    u16 src[2];
    src[0] = arg1 + 1;
    src[1] = arg1 + 2;
    CopyToBgTilemapBufferRect_ChangePalette(0, src, 9 * (cursorPosition & 1) + 1, 55 + (cursorPosition & 2), 1, 2, 0x11);
    CopyBgTilemapBufferToVram(0);
}

void MoveSelectionDestroyCursorAt(u32 cursorPosition)
{
    u16 src[2] = {32, 32};
    CopyToBgTilemapBufferRect_ChangePalette(0, src, 9 * (cursorPosition & 1) + 1, 55 + (cursorPosition & 2), 1, 2, 0x11);
    CopyBgTilemapBufferToVram(0);
}

static u32 PreviewDeterminativeMoveTargets(u32 battlerId)
{
    u32 startY, bitMask;
    u32 moveSlot = gBattleStruct->battlers[battlerId].moveSelectionCursor;
    u32 moveTarget = sChooseMoveStructOverride.moves[moveSlot].target;

    if (IsDoubleBattleForBattler(BATTLE_OPPOSITE(battlerId)))
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
            case MOVE_TARGET_SELECTED_OPPONENT:
                bitMask = (Bit(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)) | Bit(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT))) << 16; 
                startY = 8;
                break;
            case MOVE_TARGET_USER:
                if (!GET_MOVE_MOVEEFFECT_TABLE(sChooseMoveStructOverride.moves[moveSlot].move).affectsUserSide)
                {
                    bitMask = (Bit(battlerId)) << 16;
                    startY = 8;
                    break;
                }
                // fallthrough
            case MOVE_TARGET_USER_OR_ALLY:
                bitMask = (Bit(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)) | Bit(GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT))) << 16;
                startY = 8;
                break;
            case MOVE_TARGET_ALLY:
                bitMask = (Bit(GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battlerId))))) << 16;
                startY = 8;
                break;
            case MOVE_TARGET_FOES_AND_ALLY:
                bitMask = (Bit(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)) | Bit(GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(battlerId))))
                          | Bit(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT))) << 16;
                startY = 8;
                break;
        }
        BeginNormalPaletteFade(bitMask, 8, startY, 0, RGB_WHITE);
    }
    return moveTarget;
}

static void PlayerEmitMoveUsed(u32 battlerId)
{
    if (gBattleStruct->playerSelectedGimmick)
        BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, gBattleStruct->battlers[battlerId].moveSelectionCursor | RET_GIMMICK | (gMultiUsePlayerCursor << 8));
    else
        BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, gBattleStruct->battlers[battlerId].moveSelectionCursor | (gMultiUsePlayerCursor << 8));
    
    BattleControllerComplete(battlerId);
}

static void HandleInputChooseMove(u32 battlerId)
{
    u32 canSelectTarget = 0;
    u32 i, moveTarget = PreviewDeterminativeMoveTargets(battlerId);
    
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
            if (moveTarget == MOVE_TARGET_SELECTED || moveTarget == MOVE_TARGET_SELECTED_OPPONENT)
                canSelectTarget = 1; // either selected or user
            
            if (moveTarget == MOVE_TARGET_USER_OR_ALLY && IsBattlerAlive(BATTLE_PARTNER(battlerId)))
                canSelectTarget = 1;
            
            if (!sChooseMoveStructOverride.moves[gBattleStruct->battlers[battlerId].moveSelectionCursor].currentPp)
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
            else if (moveTarget == MOVE_TARGET_OPPONENTS_FIELD || moveTarget == MOVE_TARGET_BOTH || moveTarget == MOVE_TARGET_FOES_AND_ALLY || moveTarget == MOVE_TARGET_OPPONENTS)
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
                gBattlerControllersData[battlerId].func = HandleInputChooseTarget;
                
                if (moveTarget == MOVE_TARGET_USER || moveTarget == MOVE_TARGET_ALL_BATTLERS || moveTarget == MOVE_TARGET_USER_OR_ALLY)
                    gMultiUsePlayerCursor = battlerId;
                else if (!IsBattlerAlive(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)))
                    gMultiUsePlayerCursor = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
                else
                    gMultiUsePlayerCursor = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
                
                gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = SpriteCb_ShowAsMoveTarget;
                break;
            case 2:
                gBattlerControllersData[battlerId].func = HandleInputShowTargets;
                break;
            case 3:
                gBattlerControllersData[battlerId].func = HandleInputShowEntireFieldTargets;
                break;
            default:
                HideGimmickTriggerSprite();
                PlayerEmitMoveUsed(battlerId);
                break;
        }
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        ShowOrHideMoveInfoTriggerSprite(TRUE); // Hide trigger
        gBattleStruct->playerSelectedGimmick = FALSE;
        HideGimmickTriggerSprite();
        BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, 0xFFFF);
        BattleControllerComplete(battlerId);
        ResetPaletteFade();
        BeginNormalPaletteFade(0xF0000, 0, 0, 0, RGB_WHITE);
    }
    else if (JOY_NEW(DPAD_LEFT))
    {
        if (gBattleStruct->battlers[battlerId].moveSelectionCursor & 1)
        {
            PlaySE(SE_SELECT);
            MoveSelectionDestroyCursorAt(gBattleStruct->battlers[battlerId].moveSelectionCursor);
            gBattleStruct->battlers[battlerId].moveSelectionCursor ^= 1;
            MoveSelectionCreateCursorAt(gBattleStruct->battlers[battlerId].moveSelectionCursor, 0);
            MoveSelectionDisplayMoveMenu(battlerId);
            BeginNormalPaletteFade(0xF0000, 0, 0, 0, RGB_WHITE);
        }
    }
    else if (JOY_NEW(DPAD_RIGHT))
    {
        if (!(gBattleStruct->battlers[battlerId].moveSelectionCursor & 1) && (gBattleStruct->battlers[battlerId].moveSelectionCursor ^ 1) < gNumberOfMovesToChoose)
        {
            PlaySE(SE_SELECT);
            MoveSelectionDestroyCursorAt(gBattleStruct->battlers[battlerId].moveSelectionCursor);
            gBattleStruct->battlers[battlerId].moveSelectionCursor ^= 1;
            MoveSelectionCreateCursorAt(gBattleStruct->battlers[battlerId].moveSelectionCursor, 0);
            MoveSelectionDisplayMoveMenu(battlerId);
            BeginNormalPaletteFade(0xF0000, 0, 0, 0, RGB_WHITE);
        }
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if (gBattleStruct->battlers[battlerId].moveSelectionCursor & 2)
        {
            PlaySE(SE_SELECT);
            MoveSelectionDestroyCursorAt(gBattleStruct->battlers[battlerId].moveSelectionCursor);
            gBattleStruct->battlers[battlerId].moveSelectionCursor ^= 2;
            MoveSelectionCreateCursorAt(gBattleStruct->battlers[battlerId].moveSelectionCursor, 0);
            MoveSelectionDisplayMoveMenu(battlerId);
            BeginNormalPaletteFade(0xF0000, 0, 0, 0, RGB_WHITE);
        }
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (!(gBattleStruct->battlers[battlerId].moveSelectionCursor & 2) && (gBattleStruct->battlers[battlerId].moveSelectionCursor ^ 2) < gNumberOfMovesToChoose)
        {
            PlaySE(SE_SELECT);
            MoveSelectionDestroyCursorAt(gBattleStruct->battlers[battlerId].moveSelectionCursor);
            gBattleStruct->battlers[battlerId].moveSelectionCursor ^= 2;
            MoveSelectionCreateCursorAt(gBattleStruct->battlers[battlerId].moveSelectionCursor, 0);
            MoveSelectionDisplayMoveMenu(battlerId);
            BeginNormalPaletteFade(0xF0000, 0, 0, 0, RGB_WHITE);
        }
    }
    else if (JOY_NEW(START_BUTTON))
    {
        if (gBattleStruct->battlers[battlerId].usableGimmick != GIMMICK_NONE && !HasTrainerUsedGimmick(battlerId, gBattleStruct->battlers[battlerId].usableGimmick))
        {
            gBattleStruct->playerSelectedGimmick ^= TRUE;
            PlaySE(gBattleStruct->playerSelectedGimmick ? SE_PC_LOGIN : SE_PC_OFF);
            ChangeGimmickTriggerSprite(gBattleStruct->gimmickTriggerSpriteId, gBattleStruct->playerSelectedGimmick);
            BuildPlayerChooseMoveStruct(battlerId);
            MoveSelectionReloadMoveNames(battlerId);
        }
    }
#if BATTLE_MOVE_INFO
    else if (JOY_NEW(L_BUTTON))
    {
        PlaySE(SE_SELECT);
        MoveSelectionDestroyCursorAt(gBattleStruct->battlers[battlerId].moveSelectionCursor);
        ShowOrHideMoveInfoTriggerSprite(TRUE); // Hide trigger sprite
        gBattleStruct->moveInfo.submenuState = 0; // Always initialize on first submenu
        MoveInfoPrintMoveNameAndDescription(battlerId);
        MoveInfoPrintSubmenuString(battlerId, gBattleStruct->moveInfo.submenuState);
        gBattlerControllersData[battlerId].func = HandleInputMoveInfo;
    }
#endif
}

////////////////////////////
// TARGET SELECTION INPUT //
////////////////////////////

static void TryShowAsTarget(u32 battlerId)
{
    if (IsBattlerAlive(battlerId))
    {
        DoBounceEffect(battlerId, BOUNCE_HEALTHBOX, 15, 1);
        gSprites[gBattlerSpriteIds[battlerId]].callback = SpriteCb_ShowAsMoveTarget;
    }
}

static void HideAllTargets(u32 battlerId, bool32 checkBattler)
{
    u32 i;
    
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

static void HandleInputShowTargets(u32 battlerId)
{
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        HideAllTargets(battlerId, TRUE);
        HideGimmickTriggerSprite();
        PlayerEmitMoveUsed(battlerId);
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

static void HandleInputShowEntireFieldTargets(u32 battlerId)
{
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        HideAllTargets(battlerId, FALSE);
        HideGimmickTriggerSprite();
        PlayerEmitMoveUsed(battlerId);
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

bool32 CanTargetBattler(u32 attacker, u32 defender, u32 move, u32 moveTarget, bool32 isDefenderHealBlocked)
{
    if (IsBattlerAlly(attacker, defender))
    {
        if (moveTarget == MOVE_TARGET_SELECTED_OPPONENT)
            return FALSE; // Can only target opponents, not allies
        else if (gBattleMoves[move].effect == EFFECT_HIT_ENEMY_HEAL_ALLY && isDefenderHealBlocked)
            return FALSE; // Pokémon affected by Heal Block cannot target allies with Pollen Puff
    }
    return TRUE;
}

static void HandleInputChooseTarget(u32 battlerId)
{
    u32 currSelIdentity;
    u8 identities[MAX_BATTLERS_COUNT] = {B_POSITION_PLAYER_LEFT, B_POSITION_PLAYER_RIGHT, B_POSITION_OPPONENT_RIGHT, B_POSITION_OPPONENT_LEFT};
    u32 moveSlot = gBattleStruct->battlers[battlerId].moveSelectionCursor;
    u32 move = sChooseMoveStructOverride.moves[moveSlot].move, moveTarget = sChooseMoveStructOverride.moves[moveSlot].target;
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
        EndBounceEffect(gMultiUsePlayerCursor, BOUNCE_HEALTHBOX);
        HideGimmickTriggerSprite();
        PlayerEmitMoveUsed(battlerId);
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
                if (!IsBattlerAlive(gMultiUsePlayerCursor) || !CanTargetBattler(battlerId, gMultiUsePlayerCursor, move, moveTarget, sChooseMoveStructOverride.isHealBlocked[gMultiUsePlayerCursor]))
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
                if (!IsBattlerAlive(gMultiUsePlayerCursor) || !CanTargetBattler(battlerId, gMultiUsePlayerCursor, move, moveTarget, sChooseMoveStructOverride.isHealBlocked[gMultiUsePlayerCursor]))
                    i = 0;
                
            } while (i == 0);
        }
        gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = SpriteCb_ShowAsMoveTarget;
        MoveSelectionDisplayMoveType(battlerId);
    }
}

/////////////////////
// MOVE INFO INPUT //
/////////////////////

struct
{
    void(*func)(u32);
    const u8 *msg;
} static const sMoveInfoSubmenuFuncs[NUM_MOVEINFO_SUBMENUS] =
{
    {MoveInfoPrintPowerAndAccuracy,    gText_MoveInfoPowerAndAccuracy},
    {MoveInfoPrintPriorityAndCategory, gText_MoveInfoPriorityAndCategory},
    {MoveInfoPrintMoveTarget,          gText_MoveInfoTarget}
};

static void MoveInfoPrintMoveNameAndDescription(u32 battlerId)
{
    u32 move = sChooseMoveStructOverride.moves[gBattleStruct->battlers[battlerId].moveSelectionCursor].move;
    
    // Move's name and description
    ReformatStringToMaxChars(gBattleMoves[move].description, 0, 30, FALSE);
    CreateBattleMoveInfoWindowsAndArrows(move);
}

static void MoveInfoPrintPowerAndAccuracy(u32 moveSlot)
{
    u32 move;
    
    // Move's power
    if (sChooseMoveStructOverride.moves[moveSlot].power <= 1)
        StringCopy(gBattleTextBuff1, gText_MoveNoPowerAccuracy);
    else
        ConvertIntToDecimalStringN(gBattleTextBuff1, sChooseMoveStructOverride.moves[moveSlot].power, STR_CONV_MODE_LEFT_ALIGN, 3);
    
    // Move's accuracy
    move = sChooseMoveStructOverride.moves[moveSlot].move;
    
    if (gBattleMoves[move].accuracy == 0)
        StringCopy(gBattleTextBuff2, gText_MoveNoPowerAccuracy);
    else
        ConvertIntToDecimalStringN(gBattleTextBuff2, gBattleMoves[move].accuracy, STR_CONV_MODE_LEFT_ALIGN, 3);
}

static void MoveInfoPrintPriorityAndCategory(u32 moveSlot)
{
    // Move's priority
    ConvertIntToDecimalStringN(gBattleTextBuff1, gBattleMoves[sChooseMoveStructOverride.moves[moveSlot].move].priority, STR_CONV_MODE_LEFT_ALIGN, 2);
    
    // Move's category
    StringCopy(gBattleTextBuff2, gCategoryNames[sChooseMoveStructOverride.moves[moveSlot].split]);
}

static void MoveInfoPrintMoveTarget(u32 moveSlot)
{
    // Move's target
    CopyMoveTargetName(sChooseMoveStructOverride.moves[moveSlot].target, gBattleTextBuff1);
}

static void MoveInfoPrintSubmenuString(u32 battlerId, u32 stateId)
{
    sMoveInfoSubmenuFuncs[stateId].func(gBattleStruct->battlers[battlerId].moveSelectionCursor);
    BattleStringExpandPlaceholdersToDisplayedString(sMoveInfoSubmenuFuncs[stateId].msg);
    BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MOVE_INFO);
}

// Redrawn moves window
static void HandleCloseMoveInfo_Step(u32 battlerId)
{
    DestroyBattleMoveInfoWindows();
    InitMoveSelectionsVarsAndStrings(battlerId);
    SetPlayerChooseMoveInput(battlerId);
}

static void HandleInputMoveInfo(u32 battlerId)
{
    if (JOY_NEW(A_BUTTON | B_BUTTON | L_BUTTON))
    {
        PlaySE(SE_SELECT);
        // To create a smooth animation when closing the move info, first the arrow pair is removed, and then in the next frame, the moves window is redrawn.
        RemoveScrollIndicatorArrowPair(gBattleStruct->moveInfo.arrowTaskId);
        gBattlerControllersData[battlerId].func = HandleCloseMoveInfo_Step;
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

//////////////////
// TEAM PREVIEW //
//////////////////

static void ChangeBattlerSpritesInvisibilities(bool32 invisible)
{
    u32 i, spriteId;
    
    for (i = 0; i < gBattlersCount; i++)
    {
        spriteId = gBattlerSpriteIds[i];
        
        if (spriteId == 0xFF || !IsBattlerSpriteVisible(i))
            gBattleSpritesDataPtr->battlerData[i].keepInvisible = invisible;
        else
            gSprites[spriteId].invisible = invisible;
    }
}

static void HandleInputTeamPreview(u32 battlerId)
{
    if (JOY_NEW(A_BUTTON | B_BUTTON | R_BUTTON | DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        UpdateOamPriorityInAllHealthboxes(1, FALSE);
        ChangeBattlerSpritesInvisibilities(FALSE);
        HideInBattleTeamPreview();
        gBattlerControllersData[battlerId].func = HandleChooseActionAfterDma3;
    }
}

//////////////////////////
// LAST USED BALL THROW //
//////////////////////////

static u32 GetPrevBall(u32 ballId)
{
    u32 i, prevBall;
    struct BagPocket *pocket = &gBagPockets[POCKET_POKE_BALLS - 1];
    
    BagPocketCompaction(pocket);
    
    for (i = 0; i < pocket->capacity; i++)
    {
        if (pocket->itemSlots[i].itemId == ballId)
        {
            if (i != 0)
            {
                prevBall = pocket->itemSlots[i - 1].itemId;
                if (prevBall)
                    return prevBall;
            }
            break;
        }
    }
    return ITEM_NONE;
}

static u32 GetNextBall(u32 ballId)
{
    u32 i, nextBall;
    struct BagPocket *pocket = &gBagPockets[POCKET_POKE_BALLS - 1];
    
    BagPocketCompaction(pocket);
    
    for (i = 1; i < pocket->capacity; i++)
    {
        if (pocket->itemSlots[i - 1].itemId == ballId)
        {
            nextBall = pocket->itemSlots[i].itemId;
            if (nextBall)
                return nextBall;
            else
                break;
        }
    }
    return ITEM_NONE;
}

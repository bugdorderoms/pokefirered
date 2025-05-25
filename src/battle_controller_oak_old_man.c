#include "global.h"
#include "gflib.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "battle_message.h"
#include "battle_gfx_sfx_util.h"
#include "item_menu.h"
#include "link.h"
#include "party_menu.h"
#include "sound.h"
#include "strings.h"
#include "util.h"
#include "constants/battle_anim.h"
#include "constants/songs.h"
#include "constants/trainers.h"
#include "constants/battle_string_ids.h"

static void OakOldManBufferRunCommand(u32 battlerId);
static void OakOldManBufferExecCompleted(u32 battlerId);
static void OakOldManHandleDrawTrainerPic(u32 battlerId);
static void OakOldManHandleTrainerSlide(u32 battlerId);
static void OakOldManHandlePrintStringInternal(u32 battlerId, bool32 isSelection);
static void OakOldManHandlePrintString(u32 battlerId);
static void OakOldManHandlePrintSelectionString(u32 battlerId);
static void OakOldManHandleChooseMove(u32 battlerId);
static void OakOldManHandleChooseItem(u32 battlerId);
static void OakOldManHandleChoosePokemon(u32 battlerId);
static void OakOldManHandleIntroTrainerBallThrow(u32 battlerId);
static void PrintOakText_LoweringStats(u32 battlerId);
static void PrintOakText_WinEarnsPrizeMoney(u32 battlerId);
static void PrintOakText_ForPetesSake(u32 battlerId);

static void (*const sOakOldManBufferCommands[CONTROLLER_CMDS_COUNT])(u32) =
{
    [CONTROLLER_GETMONDATA]               = BtlController_HandleGetMonData,
    [CONTROLLER_SETMONDATA]               = BtlController_HandleSetMonData,
    [CONTROLLER_SETRAWMONDATA]            = BtlController_HandleSetRawMonData,
    [CONTROLLER_LOADMONSPRITE]            = BattleControllerComplete,
    [CONTROLLER_SWITCHINANIM]             = BattleControllerComplete,
    [CONTROLLER_RETURNMONTOBALL]          = BtlController_HandleReturnMonToBall,
    [CONTROLLER_DRAWTRAINERPIC]           = OakOldManHandleDrawTrainerPic,
    [CONTROLLER_TRAINERSLIDE]             = OakOldManHandleTrainerSlide,
    [CONTROLLER_TRAINERSLIDEBACK]         = BattleControllerComplete,
    [CONTROLLER_FAINTANIMATION]           = BtlController_HandleFaintAnimation,
    [CONTROLLER_BALLTHROWANIM]            = SafariHandleBallThrowAnim,
    [CONTROLLER_MOVEANIMATION]            = BtlController_HandleMoveAnimation,
    [CONTROLLER_PRINTSTRING]              = OakOldManHandlePrintString,
    [CONTROLLER_PRINTSELECTIONSTRING]     = OakOldManHandlePrintSelectionString,
    [CONTROLLER_CHOOSEACTION]             = PlayerHandleChooseAction,
    [CONTROLLER_CHOOSEMOVE]               = OakOldManHandleChooseMove,
    [CONTROLLER_OPENBAG]                  = OakOldManHandleChooseItem,
    [CONTROLLER_CHOOSEPOKEMON]            = OakOldManHandleChoosePokemon,
    [CONTROLLER_HEALTHBARUPDATE]          = PlayerHandleHealthbarUpdate,
    [CONTROLLER_EXPUPDATE]                = BtlController_HandleExpUpdate,
    [CONTROLLER_STATUSICONUPDATE]         = BtlController_HandleStatusIconUpdate,
    [CONTROLLER_STATUSANIMATION]          = BtlController_HandleStatusAnimation,
    [CONTROLLER_DATATRANSFER]             = BattleControllerComplete,
    [CONTROLLER_TWORETURNVALUES]          = BattleControllerComplete,
    [CONTROLLER_CHOSENMONRETURNVALUE]     = BattleControllerComplete,
    [CONTROLLER_ONERETURNVALUE]           = BattleControllerComplete,
    [CONTROLLER_EFFECTIVENESSSOUND]       = OakOldManHandlePlaySE,
    [CONTROLLER_PLAYFANFAREORBGM]         = BtlController_HandlePlayFanfare,
    [CONTROLLER_FAINTINGCRY]              = SafariHandleFaintingCry,
    [CONTROLLER_INTROSLIDE]               = BtlController_HandleIntroSlide,
    [CONTROLLER_INTROTRAINERBALLTHROW]    = OakOldManHandleIntroTrainerBallThrow,
    [CONTROLLER_DRAWPARTYSTATUSSUMMARY]   = OakOldManHandleDrawPartyStatusSummary,
    [CONTROLLER_HIDEPARTYSTATUSSUMMARY]   = BattleControllerComplete,
    [CONTROLLER_ENDBOUNCE]                = BtlController_HandleEndBounceEffect,
    [CONTROLLER_SPRITEINVISIBILITY]       = BattleControllerComplete,
    [CONTROLLER_BATTLEANIMATION]          = BtlController_HandleBattleAnimation,
    [CONTROLLER_LINKSTANDBYMSG]           = PlayerHandleLinkStandbyMsg,
    [CONTROLLER_RESETACTIONMOVESELECTION] = BattleControllerComplete,
    [CONTROLLER_ENDLINKBATTLE]            = OakOldManHandleEndLinkBattle,
    [CONTROLLER_TERMINATOR_NOP]           = ControllerDummy,
};

void SetControllerToOakOrOldMan(u32 battlerId)
{
    gBattlerControllerFuncs[battlerId] = OakOldManBufferRunCommand;
    gBattlerControllerEndFuncs[battlerId] = OakOldManBufferExecCompleted;
    gBattleStruct->simulatedInputState[0] = 0;
    gBattleStruct->simulatedInputState[1] = 0;
    gBattleStruct->simulatedInputState[2] = 0;
    gBattleStruct->simulatedInputState[3] = 0;
}

static void OakOldManBufferRunCommand(u32 battlerId)
{
    if (gBattleControllerExecFlags & Bit(battlerId))
    {
        if (gBattleBufferA[battlerId][0] < ARRAY_COUNT(sOakOldManBufferCommands))
            sOakOldManBufferCommands[gBattleBufferA[battlerId][0]](battlerId);
        else
            OakOldManBufferExecCompleted(battlerId);
    }
}

static void OakOldManBufferExecCompleted(u32 battlerId)
{
    gBattlerControllerFuncs[battlerId] = OakOldManBufferRunCommand;
    
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

static void OakOldManHandleDrawTrainerPic(u32 battlerId)
{
    u32 trainerPicId = (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE) ? TRAINER_BACK_PIC_RED + gSaveBlock2Ptr->playerGender : TRAINER_BACK_PIC_OLD_MAN;
    BtlController_HandleDrawTrainerPic(battlerId, trainerPicId, FALSE, 80, (8 - gTrainerBackPicTable[trainerPicId].coords.size) * 4 + 80, 30);
}

static void OakOldManHandleTrainerSlide(u32 battlerId)
{
    u32 trainerPicId = (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE) ? TRAINER_BACK_PIC_RED + gSaveBlock2Ptr->playerGender : TRAINER_BACK_PIC_OLD_MAN;
    BtlController_HandleTrainerSlide(battlerId, trainerPicId, FALSE, 80, (8 - gTrainerBackPicTable[trainerPicId].coords.size) * 4 + 80);
}

static void OakOldManHandlePrintStringInternal(u32 battlerId, bool32 isSelection)
{
    u16 *stringId = (u16 *)(&gBattleBufferA[battlerId][2]);
    
    if (gBattleTypeFlags & BATTLE_TYPE_OLD_MAN_TUTORIAL && *stringId == STRINGID_INTROSENDOUT)
    {
        gBattle_BG0_X = 0;
        gBattle_BG0_Y = 0;
        BattleControllerComplete(battlerId);
    }
    else
    {
        BtlController_HandlePrintString(battlerId, *stringId, isSelection);
        
        if (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE)
        {
            switch (*stringId)
            {
            case STRINGID_EFFSTATCHANGED:
                if (gBattleStruct->statChange.buff < 0 && !(gBattleStruct->statChange.flags & STAT_CHANGE_FLAG_SELF_INFLICT)
                && !BtlCtrl_OakOldMan_TestState2Flag(FIRST_BATTLE_MSG_FLAG_STAT_CHG))
                {
                    BtlCtrl_OakOldMan_SetState2Flag(FIRST_BATTLE_MSG_FLAG_STAT_CHG);
                    gBattlerControllerFuncs[battlerId] = PrintOakText_LoweringStats;
                }
                break;
            case STRINGID_PLAYERGOTMONEY:
                gBattlerControllerFuncs[battlerId] = PrintOakText_WinEarnsPrizeMoney;
                break;
            case STRINGID_TRAINER1WINTEXT:
                gBattlerControllerFuncs[battlerId] = PrintOakText_HowDisappointing;
                break;
            case STRINGID_DONTLEAVEBIRCH:
                gBattlerControllerFuncs[battlerId] = PrintOakText_OakNoRunningFromATrainer;
                break;
            }
        }
    }
}

static void OakOldManHandlePrintString(u32 battlerId)
{
    OakOldManHandlePrintStringInternal(battlerId, FALSE);
}

static void OakOldManHandlePrintSelectionString(u32 battlerId)
{
    if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
        OakOldManHandlePrintStringInternal(battlerId, TRUE);
    else
        BattleControllerComplete(battlerId);
}

static void OakOldManHandleChooseMove(u32 battlerId)
{
    if (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE)
        PlayerHandleChooseMove(battlerId);
    else
    {
        switch (gBattleStruct->simulatedInputState[1])
        {
        case 0:
            InitMoveSelectionsVarsAndStrings(battlerId);
            ++gBattleStruct->simulatedInputState[1];
            gBattleStruct->simulatedInputState[3] = 80;
            // fall through
        case 1:
            if (--gBattleStruct->simulatedInputState[3] == 0)
            {
                PlaySE(SE_SELECT);
                BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, 0x100);
                BattleControllerComplete(battlerId);
            }
            break;
        }
    }
}

static void OakOldManHandleChooseItem(u32 battlerId)
{
    gBattleStruct->battlers[battlerId].itemPartyIndex = 0;
    PlayerHandleChooseItem(battlerId);
}

static void WaitForMonSelection(u32 battlerId)
{
    if (gMain.callback2 == BattleMainCB2 && !gPaletteFade.active)
    {
        if (gPartyMenuUseExitCallback)
            BtlController_EmitChosenMonReturnValue(battlerId, BUFFER_B, gSelectedMonPartyId, gBattlePartyCurrentOrder);
        else
            BtlController_EmitChosenMonReturnValue(battlerId, BUFFER_B, PARTY_SIZE, NULL);
        
        BattleControllerComplete(battlerId);
    }
}

static void OpenPartyMenuToChooseMon(u32 battlerId)
{
    if (!gPaletteFade.active)
    {
        u32 caseId;

        gBattlerControllerFuncs[battlerId] = WaitForMonSelection;
        caseId = gTasks[gBattleControllerData[battlerId]].data[0];
        DestroyTask(gBattleControllerData[battlerId]);
        FreeAllWindowBuffers();
        OpenPartyMenuInTutorialBattle(caseId);
    }
}

static void OakOldManHandleChoosePokemon(u32 battlerId)
{
    BtlController_HandleChoosePokemon(battlerId, OpenPartyMenuToChooseMon);
}

void OakOldManHandlePlaySE(u32 battlerId)
{
    PlaySE(gBattleBufferA[battlerId][1] | (gBattleBufferA[battlerId][2] << 8));
    BattleControllerComplete(battlerId);
}

static void Intro_WaitForShinyAnimAndHealthbox(u32 battlerId)
{
    if (gSprites[gHealthboxSpriteIds[battlerId]].callback == SpriteCallbackDummy && gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim
    && gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].finishedShinyMonAnim)
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim = FALSE;
        gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim = FALSE;
        gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].triedShinyMonAnim = FALSE;
        gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].finishedShinyMonAnim = FALSE;
        
        FreeSpriteTilesByTag(ANIM_TAG_GOLD_STARS);
        FreeSpritePaletteByTag(ANIM_TAG_GOLD_STARS);
        
        CreateTask(Task_BltController_RestoreBgmAfterCry, 10);
        HandleLowHpMusicChange(&gPlayerParty[gBattlerPartyIndexes[battlerId]], battlerId);
        gBattlerControllerFuncs[battlerId] = PrintOakText_ForPetesSake;
    }
}

static void Intro_TryShinyAnimShowHealthbox(u32 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive && !gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].ballAnimActive)
    {
        if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim)
            TryShinyAnimation(battlerId);
        
        if (!gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].triedShinyMonAnim)
            TryShinyAnimation(BATTLE_PARTNER(battlerId));
        
        if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && !(gBattleTypeFlags & BATTLE_TYPE_MULTI))
            ShowHealthBox(BATTLE_PARTNER(battlerId));
        
        ShowHealthBox(battlerId);
        gBattleSpritesDataPtr->animationData->healthboxSlideInStarted = FALSE;
        gBattlerControllerFuncs[battlerId] = Intro_WaitForShinyAnimAndHealthbox;
    }
}

static void OakOldManHandleIntroTrainerBallThrow(u32 battlerId)
{
    if (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE)
        BtlController_HandleIntroTrainerBallThrow(battlerId, 0xD6F8, TRAINER_BACK_PIC_RED + gSaveBlock2Ptr->playerGender, StartAnimLinearTranslation, 31, Intro_TryShinyAnimShowHealthbox);
    else
    {
        if (gBattleSpritesDataPtr->healthBoxesData[battlerId].partyStatusSummaryShown)
            gTasks[gBattlerStatusSummaryTaskId[battlerId]].func = Task_HidePartyStatusSummary;
        
        BattleControllerComplete(battlerId);
    }
}

void OakOldManHandleDrawPartyStatusSummary(u32 battlerId)
{
    if (!(gBattleBufferA[battlerId][1] && GetBattlerSide(battlerId) == B_SIDE_PLAYER))
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].partyStatusSummaryShown = TRUE;
        gBattlerStatusSummaryTaskId[battlerId] = CreatePartyStatusSummarySprites(battlerId, (struct HpAndStatus*)&gBattleBufferA[battlerId][4], gBattleBufferA[battlerId][1], gBattleBufferA[battlerId][2]);
    }
    BattleControllerComplete(battlerId);
}

static void OakOldManSetBattleEndCallbacks(u32 battlerId)
{
    if (!gPaletteFade.active)
    {
        gMain.inBattle = FALSE;
        gMain.callback1 = gPreBattleCallback1;
        SetMainCallback2(gMain.savedCallback);
    }
}

void OakOldManHandleEndLinkBattle(u32 battlerId)
{
    BtlController_HandleEndLinkBattle(battlerId, gBattleBufferA[battlerId][1], (!(gBattleTypeFlags & BATTLE_TYPE_IS_MASTER) && gBattleTypeFlags & BATTLE_TYPE_LINK) ? OakOldManSetBattleEndCallbacks : NULL);
}

///////////
// INPUT //
///////////

void OakOldManHandleInputChooseMove(u32 battlerId)
{
    HandleInputChooseMove(battlerId); // Player input
    
    if (!(gBattleControllerExecFlags & Bit(battlerId)))
        BattleControllerComplete(battlerId);
}

/////////////////////
// SIMULATED INPUT //
/////////////////////

void OakOldMan_SimulateInputChooseAction(u32 battlerId)
{
    // Old Man
    switch (gBattleStruct->simulatedInputState[0])
    {
    case 0:
        gBattleStruct->simulatedInputState[2] = 64;
        ++gBattleStruct->simulatedInputState[0];
        // fall through
    case 1:
        if (--gBattleStruct->simulatedInputState[2] == 0)
        {
            // Move cursor to Bag
            PlaySE(SE_SELECT);
            ActionSelectionDestroyCursorAt(0);
            ActionSelectionCreateCursorAt(1);
            gBattleStruct->simulatedInputState[2] = 64;
            ++gBattleStruct->simulatedInputState[0];
        }
        break;
    case 2:
        if (--gBattleStruct->simulatedInputState[2] == 0)
        {
            // Open bag
            PlaySE(SE_SELECT);
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_USE_ITEM, 0);
            BattleControllerComplete(battlerId);
        }
        break;
    }
}

/////////////////////
// SPECIAL STRINGS //
/////////////////////

bool32 BtlCtrl_OakOldMan_TestState2Flag(u32 mask)
{
    return (gBattleStruct->simulatedInputState[2] & mask);
}

void BtlCtrl_OakOldMan_SetState2Flag(u32 mask)
{
    gBattleStruct->simulatedInputState[2] |= mask;
}

static void PrintOakTextWithMainBgDarkened(u32 battlerId, const u8 *str, u32 delay);

static void PrintOakText_LoweringStats(u32 battlerId)
{
    PrintOakTextWithMainBgDarkened(battlerId, gText_LoweringStats, 64);
}

static void PrintOakText_WinEarnsPrizeMoney(u32 battlerId)
{
    PrintOakTextWithMainBgDarkened(battlerId, gText_WinEarnsPrizeMoney, 64);
}

void PrintOakText_HowDisappointing(u32 battlerId)
{
    PrintOakTextWithMainBgDarkened(battlerId, gText_HowDissapointing, 64);
}

void PrintOakText_OakNoRunningFromATrainer(u32 battlerId)
{
    PrintOakTextWithMainBgDarkened(battlerId, gText_OakNoRunningFromATrainer, 1);
}

void PrintOakText_InflictingDamageIsKey(u32 battlerId)
{
    PrintOakTextWithMainBgDarkened(battlerId, gText_InflictingDamageIsKey, 1);
}

void PrintOakText_KeepAnEyeOnHP(u32 battlerId)
{
    switch (gBattleStruct->simulatedInputState[0])
    {
    case 0:
        if (!gPaletteFade.active)
        {
            DoLoadHealthboxPalsForLevelUp(&gBattleStruct->simulatedInputState[1], &gBattleStruct->simulatedInputState[3], battlerId);
            BeginNormalPaletteFade(0xFFFFFF7E, 4, 0, 8, RGB_BLACK);
            ++gBattleStruct->simulatedInputState[0];
        }
        break;
    case 1:
        if (!gPaletteFade.active)
        {
            BeginNormalPaletteFade((Bit(gBattleStruct->simulatedInputState[1]) | Bit(gBattleStruct->simulatedInputState[3])) << 16, 4, 8, 0, RGB_BLACK);
            ++gBattleStruct->simulatedInputState[0];
        }
        break;
    case 2:
        if (!gPaletteFade.active)
        {
            BtlCtrl_DrawVoiceoverMessageFrame();
            ++gBattleStruct->simulatedInputState[0];
        }
        break;
    case 3:
        BattleStringExpandPlaceholdersToDisplayedString(gText_KeepAnEyeOnHP);
        BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_OAK_OLD_MAN);
        ++gBattleStruct->simulatedInputState[0];
        break;
    case 4:
        if (!IsTextPrinterActive(24))
        {
            BeginNormalPaletteFade((Bit(gBattleStruct->simulatedInputState[1]) | Bit(gBattleStruct->simulatedInputState[3])) << 16, 4, 0, 8, RGB_BLACK);
            ++gBattleStruct->simulatedInputState[0];
        }
        break;
    case 5:
        if (!gPaletteFade.active)
        {
            BeginNormalPaletteFade(0xFFFFFF7E, 4, 8, 0, RGB_BLACK);
            ++gBattleStruct->simulatedInputState[0];
        }
        break;
    case 6:
        if (!gPaletteFade.active)
        {
            BtlCtrl_RemoveVoiceoverMessageFrame();
            BtlController_EmitOneReturnValue(battlerId, BUFFER_B, gSpecialVar_ItemId);
            BattleControllerComplete(battlerId);
            gBattleStruct->simulatedInputState[0] = 0;
        }
        break;
    }
}

static void PrintOakText_ForPetesSake(u32 battlerId)
{
    switch (gBattleStruct->simulatedInputState[0])
    {
    case 0:
        if (!gPaletteFade.active)
        {
            DoLoadHealthboxPalsForLevelUp(&gBattleStruct->simulatedInputState[1], &gBattleStruct->simulatedInputState[3], GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT));
            BeginNormalPaletteFade(0xFFFFFF7E, 4, 0, 8, RGB_BLACK);
            ++gBattleStruct->simulatedInputState[0];
        }
        break;
    case 1:
        if (!gPaletteFade.active)
        {
            BtlCtrl_DrawVoiceoverMessageFrame();
            ++gBattleStruct->simulatedInputState[0];
        }
        break;
    case 2:
        BattleStringExpandPlaceholdersToDisplayedString(gText_ForPetesSake);
        BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_OAK_OLD_MAN);
        ++gBattleStruct->simulatedInputState[0];
        break;
    case 3:
        if (!IsTextPrinterActive(24))
        {
            BeginNormalPaletteFade((Bit(gBattleStruct->simulatedInputState[1]) | Bit(gBattleStruct->simulatedInputState[3])) << 16, 4, 8, 0, RGB_BLACK);
            ++gBattleStruct->simulatedInputState[0];
        }
        break;
    case 4:
        if (!gPaletteFade.active)
        {
            BattleStringExpandPlaceholdersToDisplayedString(gText_TheTrainerThat);
            BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_OAK_OLD_MAN);
            ++gBattleStruct->simulatedInputState[0];
        }
        break;
    case 5:
        if (!IsTextPrinterActive(24))
        {
            BeginNormalPaletteFade((Bit(gBattleStruct->simulatedInputState[1]) | Bit(gBattleStruct->simulatedInputState[3])) << 16, 4, 0, 8, RGB_BLACK);
            ++gBattleStruct->simulatedInputState[0];
        }
        break;
    case 6:
        if (!gPaletteFade.active)
        {
            BattleStringExpandPlaceholdersToDisplayedString(gText_TryBattling);
            BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_OAK_OLD_MAN);
            ++gBattleStruct->simulatedInputState[0];
        }
        break;
    case 7:
        if (!IsTextPrinterActive(24))
        {
            BeginNormalPaletteFade(0xFFFFFF7E, 4, 8, 0, RGB_BLACK);
            ++gBattleStruct->simulatedInputState[0];
        }
        break;
    case 8:
        if (!gPaletteFade.active)
        {
            DoFreeHealthboxPalsForLevelUp(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT));
            BtlCtrl_RemoveVoiceoverMessageFrame();
            gBattleStruct->simulatedInputState[0] = 0;
            BattleControllerComplete(battlerId);
        }
        break;
    }
}

static void PrintOakTextWithMainBgDarkened(u32 battlerId, const u8 *str, u32 delay)
{
    // If delay is 0, it's treated as 256.
    switch (gBattleStruct->simulatedInputState[0])
    {
    case 0:
        if (!IsTextPrinterActive(0))
        {
            gBattleStruct->simulatedInputState[3] = delay;
            ++gBattleStruct->simulatedInputState[0];
        }
        break;
    case 1:
        if (--gBattleStruct->simulatedInputState[3] == 0)
        {
            BeginNormalPaletteFade(0xFFFFFF7E, 4, 0, 8, RGB_BLACK);
            ++gBattleStruct->simulatedInputState[0];
        }
        break;
    case 2:
        if (!gPaletteFade.active)
        {
            BtlCtrl_DrawVoiceoverMessageFrame();
            ++gBattleStruct->simulatedInputState[0];
        }
        break;
    case 3:
        BattleStringExpandPlaceholdersToDisplayedString(str);
        BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_OAK_OLD_MAN);
        ++gBattleStruct->simulatedInputState[0];
        break;
    case 4:
        if (!IsTextPrinterActive(24))
        {
            BeginNormalPaletteFade(0xFFFFFF7E, 4, 8, 0, RGB_BLACK);
            ++gBattleStruct->simulatedInputState[0];
        }
        break;
    case 5:
        if (!gPaletteFade.active)
        {
            BtlCtrl_RemoveVoiceoverMessageFrame();
            BattleControllerComplete(battlerId);
            gBattleCommunication[MSG_DISPLAY] = FALSE;
            gBattleStruct->simulatedInputState[0] = 0;
        }
        break;
    }
}

void BtlCtrl_DrawVoiceoverMessageFrame(void)
{
    u32 width = 0x1A, pal = 7;

    FillBgTilemapBufferRect(0, 0x30,                 0,    0xE,  1,     1, pal);
    FillBgTilemapBufferRect(0, 0x31,                 1,    0xE,  1,     1, pal);
    FillBgTilemapBufferRect(0, 0x32,                 2,    0xE,  width, 1, pal);
    FillBgTilemapBufferRect(0, 0x33,                 0x1C, 0xE,  1,     1, pal);
    FillBgTilemapBufferRect(0, 0x34,                 0x1D, 0xE,  1,     1, pal);
    FillBgTilemapBufferRect(0, 0x35,                 0,    0xF,  1,     1, pal);
    FillBgTilemapBufferRect(0, 0x36,                 1,    0xF,  1,     1, pal);
    FillBgTilemapBufferRect(0, 0x38,                 0x1C, 0xF,  1,     1, pal);
    FillBgTilemapBufferRect(0, 0x39,                 0x1D, 0xF,  1,     1, pal);
    FillBgTilemapBufferRect(0, 0x3A,                 0,    0x10, 1,     1, pal);
    FillBgTilemapBufferRect(0, 0x3B,                 1,    0x10, 1,     1, pal);
    FillBgTilemapBufferRect(0, 0x3C,                 0x1C, 0x10, 1,     1, pal);
    FillBgTilemapBufferRect(0, 0x3D,                 0x1D, 0x10, 1,     1, pal);
    FillBgTilemapBufferRect(0, BG_TILE_V_FLIP(0x3A), 0,    0x11, 1,     1, pal);
    FillBgTilemapBufferRect(0, BG_TILE_V_FLIP(0x3B), 1,    0x11, 1,     1, pal);
    FillBgTilemapBufferRect(0, BG_TILE_V_FLIP(0x3C), 0x1C, 0x11, 1,     1, pal);
    FillBgTilemapBufferRect(0, BG_TILE_V_FLIP(0x3D), 0x1D, 0x11, 1,     1, pal);
    FillBgTilemapBufferRect(0, BG_TILE_V_FLIP(0x35), 0,    0x12, 1,     1, pal);
    FillBgTilemapBufferRect(0, BG_TILE_V_FLIP(0x36), 1,    0x12, 1,     1, pal);
    FillBgTilemapBufferRect(0, BG_TILE_V_FLIP(0x38), 0x1C, 0x12, 1,     1, pal);
    FillBgTilemapBufferRect(0, BG_TILE_V_FLIP(0x39), 0x1D, 0x12, 1,     1, pal);
    FillBgTilemapBufferRect(0, BG_TILE_V_FLIP(0x30), 0,    0x13, 1,     1, pal);
    FillBgTilemapBufferRect(0, BG_TILE_V_FLIP(0x31), 1,    0x13, 1,     1, pal);
    FillBgTilemapBufferRect(0, BG_TILE_V_FLIP(0x32), 2,    0x13, width, 1, pal);
    FillBgTilemapBufferRect(0, BG_TILE_V_FLIP(0x33), 0x1C, 0x13, 1,     1, pal);
    FillBgTilemapBufferRect(0, BG_TILE_V_FLIP(0x34), 0x1D, 0x13, 1,     1, pal);
}

void BtlCtrl_RemoveVoiceoverMessageFrame(void)
{
    u32 pal = 0, width = 0x1A, height = 4;

    FillBgTilemapBufferRect(0, 3,    0,    0xE,  1,     1,      pal);
    FillBgTilemapBufferRect(0, 4,    1,    0xE,  1,     1,      pal);
    FillBgTilemapBufferRect(0, 5,    2,    0xE,  width, 1,      pal);
    FillBgTilemapBufferRect(0, 6,    0x1C, 0xE,  1,     1,      pal);
    FillBgTilemapBufferRect(0, 7,    0x1D, 0xE,  1,     1,      pal);
    FillBgTilemapBufferRect(0, 8,    0,    0xF,  1,     height, pal);
    FillBgTilemapBufferRect(0, 9,    1,    0xF,  1,     height, pal);
    FillBgTilemapBufferRect(0, 0xA,  2,    0xF,  width, height, pal);
    FillBgTilemapBufferRect(0, 0xB,  0x1C, 0xF,  1,     height, pal);
    FillBgTilemapBufferRect(0, 0xC,  0x1D, 0xF,  1,     height, pal);
    FillBgTilemapBufferRect(0, 0xD,  0,    0x13, 1,     1,      pal);
    FillBgTilemapBufferRect(0, 0xE,  1,    0x13, 1,     1,      pal);
    FillBgTilemapBufferRect(0, 0xF,  2,    0x13, width, 1,      pal);
    FillBgTilemapBufferRect(0, 0x10, 0x1C, 0x13, 1,     1,      pal);
    FillBgTilemapBufferRect(0, 0x11, 0x1D, 0x13, 1,     1,      pal);
}

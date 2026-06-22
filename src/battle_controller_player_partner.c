#include "global.h"
#include "battle_ai.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "link.h"
#include "sound.h"

static void PlayerPartnerBufferRunCommand(u32 battlerId);
static void PlayerPartnerBufferExecCompleted(u32 battlerId);
static void PlayerPartnerHandleChooseAction(u32 battlerId);

static void (*const sPlayerPartnerBufferCommands[CONTROLLER_CMDS_COUNT])(u32) =
{
    [CONTROLLER_GETMONDATA]               = BtlController_HandleGetMonData,
    [CONTROLLER_SETMONDATA]               = BtlController_HandleSetMonData,
    [CONTROLLER_LOADMONSPRITE]            = BtlController_HandleLoadMonSprite,
    [CONTROLLER_SWITCHINANIM]             = LinkOpponentHandleSwitchInAnim,
    [CONTROLLER_RETURNMONTOBALL]          = BtlController_HandleReturnMonToBall,
    [CONTROLLER_DRAWTRAINERPIC]           = PlayerPartnerHandleDrawTrainerPic,
    [CONTROLLER_TRAINERSLIDE]             = BattleControllerComplete,
    [CONTROLLER_TRAINERSLIDEBACK]         = OpponentHandleTrainerSlideBack,
    [CONTROLLER_FAINTANIMATION]           = BtlController_HandleFaintAnimation,
    [CONTROLLER_BALLTHROWANIM]            = BattleControllerComplete,
    [CONTROLLER_MOVEANIMATION]            = BtlController_HandleMoveAnimation,
    [CONTROLLER_PRINTSTRING]              = BtlController_HandlePrintString,
    [CONTROLLER_PRINTSELECTIONSTRING]     = BattleControllerComplete,
    [CONTROLLER_CHOOSEACTION]             = PlayerPartnerHandleChooseAction,
    [CONTROLLER_CHOOSEMOVE]               = OpponentHandleChooseMove,
    [CONTROLLER_OPENBAG]                  = BattleControllerComplete,
    [CONTROLLER_CHOOSEPOKEMON]            = OpponentHandleChoosePokemon,
    [CONTROLLER_HEALTHBARUPDATE]          = BtlController_HandleHealthbarUpdateWithHpText,
    [CONTROLLER_EXPUPDATE]                = BtlController_HandleExpUpdate,
    [CONTROLLER_STATUSICONUPDATE]         = BtlController_HandleStatusIconUpdate,
    [CONTROLLER_STATUSANIMATION]          = BtlController_HandleStatusAnimation,
    [CONTROLLER_DATATRANSFER]             = BattleControllerComplete,
    [CONTROLLER_TWORETURNVALUES]          = BattleControllerComplete,
    [CONTROLLER_CHOSENMONRETURNVALUE]     = BattleControllerComplete,
    [CONTROLLER_ONERETURNVALUE]           = BattleControllerComplete,
    [CONTROLLER_EFFECTIVENESSSOUND]       = BtlController_HandlePlaySE,
    [CONTROLLER_PLAYFANFAREORBGM]         = BtlController_HandlePlayFanfare,
    [CONTROLLER_FAINTINGCRY]              = BtlController_HandleFaintingCry,
    [CONTROLLER_INTROSLIDE]               = BtlController_HandleIntroSlide,
    [CONTROLLER_INTROTRAINERBALLTHROW]    = PlayerPartnerHandleIntroTrainerBallThrow,
    [CONTROLLER_DRAWPARTYSTATUSSUMMARY]   = BtlController_HandleDrawPartyStatusSummary,
    [CONTROLLER_HIDEPARTYSTATUSSUMMARY]   = BtlController_HandleHidePartyStatusSummary,
    [CONTROLLER_ENDBOUNCE]                = BattleControllerComplete,
    [CONTROLLER_SPRITEINVISIBILITY]       = BtlController_HandleSpriteInvisibility,
    [CONTROLLER_BATTLEANIMATION]          = BtlController_HandleBattleAnimation,
    [CONTROLLER_LINKSTANDBYMSG]           = BattleControllerComplete,
    [CONTROLLER_RESETACTIONMOVESELECTION] = BattleControllerComplete,
    [CONTROLLER_ENDLINKBATTLE]            = PlayerHandleEndLinkBattle,
    [CONTROLLER_GIMMICKSTATE]             = PlayerPartnerHandleGimmickState,
    [CONTROLLER_HEALTHBOXUPDATE]          = BtlController_HandleHealthboxUpdate,
    [CONTROLLER_HIDEALLHEALTHBOXES]       = BtlController_HandleHideAllHealthboxes,
    [CONTROLLER_BATTLEFORMCHANGE]         = BtlController_HandleBattleFormChange,
    [CONTROLLER_PARTYFORMCHANGE]          = BtlController_HandlePartyFormChange,
    [CONTROLLER_ISPOCKETNOTEMPTY]         = BattleControllerComplete,
    [CONTROLLER_YESNOBOX]                 = BattleControllerComplete,
    [CONTROLLER_MONCAUGHTEFFECTS]         = BattleControllerComplete,
    [CONTROLLER_TERMINATOR_NOP]           = ControllerDummy,
};

void SetControllerToPlayerPartner(u32 battlerId)
{
    gBattlerControllersData[battlerId].func = PlayerPartnerBufferRunCommand;
    gBattlerControllersData[battlerId].endFunc = PlayerPartnerBufferExecCompleted;
}

static void PlayerPartnerBufferRunCommand(u32 battlerId)
{
    if (gBattleControllerExecFlags & Bit(battlerId))
    {
        if (gBattleBufferA[battlerId][0] < ARRAY_COUNT(sPlayerPartnerBufferCommands))
            sPlayerPartnerBufferCommands[gBattleBufferA[battlerId][0]](battlerId);
        else
            PlayerPartnerBufferExecCompleted(battlerId);
    }
}

static void PlayerPartnerBufferExecCompleted(u32 battlerId)
{
    gBattlerControllersData[battlerId].func = PlayerPartnerBufferRunCommand;
    
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

static u32 GetPlayerPartnerTrainerPicId(u32 battlerId)
{
    if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
        return gBattlePartners[gPartnerTrainerId].trainerPic;
    else
        return GetPlayerTrainerPicId(GetBattlerMultiplayerId(battlerId));
}

void PlayerPartnerHandleDrawTrainerPic(u32 battlerId)
{
    BtlController_HandleDrawTrainerPic(battlerId, GetPlayerPartnerTrainerPicId(battlerId));
}

static void PlayerPartnerHandleChooseAction(u32 battlerId)
{
    BattleAI_ChooseAction(battlerId);
    BattleControllerComplete(battlerId);
}

static void Intro_WaitForHealthbox(u32 battlerId)
{
    bool32 var = FALSE;

    if (!IsDoubleBattleForBattler(battlerId) || !IsPartnerMonFromSameTrainer(battlerId))
    {
        if (gSprites[gHealthboxSpriteIds[battlerId]].callback == SpriteCallbackDummy)
            var = TRUE;
    }
    else if (gSprites[gHealthboxSpriteIds[battlerId]].callback == SpriteCallbackDummy && gSprites[gHealthboxSpriteIds[BATTLE_PARTNER(battlerId)]].callback == SpriteCallbackDummy)
        var = TRUE;

    if (IsCryPlayingOrClearCrySongs())
        var = FALSE;
    
    if (var)
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].introEndDelay = 3;
        gBattlerControllersData[battlerId].func = Intro_DelayAndEnd;
    }
}

static void Intro_ShowHealthbox(u32 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive && !gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].ballAnimActive)
    {
        if (++gBattleSpritesDataPtr->healthBoxesData[battlerId].introEndDelay != 1)
        {
            gBattleSpritesDataPtr->healthBoxesData[battlerId].introEndDelay = 0;
            
            if (IsDoubleBattleForBattler(battlerId) && IsPartnerMonFromSameTrainer(battlerId))
                ShowHealthBox(BATTLE_PARTNER(battlerId));

            ShowHealthBox(battlerId);
            gBattleSpritesDataPtr->animationData->healthboxSlideInStarted = FALSE;
            gBattlerControllersData[battlerId].func = Intro_WaitForHealthbox;
        }
    }
}

void PlayerPartnerHandleIntroTrainerBallThrow(u32 battlerId)
{
    BtlController_HandleIntroTrainerBallThrow(battlerId, 0xD6F9, GetPlayerPartnerTrainerPicId(battlerId), StartAnimLinearTranslation, 24, Intro_ShowHealthbox);
}

void PlayerPartnerHandleGimmickState(u32 battlerId)
{
    switch (gBattleBufferA[battlerId][1])
    {
        case STATE_CHECK_GIMMICK_KEY_ITEMS:
        {
            u8 data[NUM_GIMMICK_BITS + 1];
            memset(data, 0xFF, sizeof(data)); // Can always use gimmick regardless of item limitations
            BtlController_EmitDataTransfer(battlerId, BUFFER_B, NUM_GIMMICK_BITS, data);
            break;
        }
        case STATE_USABLE_GIMMICK:
            gBattleStruct->battlers[battlerId].usableGimmick = gBattleBufferA[battlerId][2];
            break;
        case STATE_GIMMICK_IN_PROGRESS:
            gBattleSpritesDataPtr->battlerData[battlerId].gimmickInProgress = TRUE;
            break;
        case STATE_ACTIVE_GIMMICK:
            SetActiveGimmick(battlerId, gBattleBufferA[battlerId][2]);
            break;
    }
    BattleControllerComplete(battlerId);
}

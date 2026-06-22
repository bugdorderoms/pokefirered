#include "global.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "battle_gfx_sfx_util.h"
#include "link.h"
#include "sound.h"
#include "util.h"
#include "constants/battle_anim.h"

static void LinkPartnerBufferRunCommand(u32 battlerId);
static void LinkPartnerBufferExecCompleted(u32 battlerId);

static void (*const sLinkPartnerBufferCommands[CONTROLLER_CMDS_COUNT])(u32) =
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
    [CONTROLLER_BALLTHROWANIM]            = PlayerHandleBallThrowAnim,
    [CONTROLLER_MOVEANIMATION]            = BtlController_HandleMoveAnimation,
    [CONTROLLER_PRINTSTRING]              = BtlController_HandlePrintString,
    [CONTROLLER_PRINTSELECTIONSTRING]     = BattleControllerComplete,
    [CONTROLLER_CHOOSEACTION]             = BattleControllerComplete,
    [CONTROLLER_CHOOSEMOVE]               = BattleControllerComplete,
    [CONTROLLER_OPENBAG]                  = BattleControllerComplete,
    [CONTROLLER_CHOOSEPOKEMON]            = BattleControllerComplete,
    [CONTROLLER_HEALTHBARUPDATE]          = BtlController_HandleHealthbarUpdateNoHpText,
    [CONTROLLER_EXPUPDATE]                = BattleControllerComplete,
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
    [CONTROLLER_GIMMICKSTATE]             = LinkOpponentHandleGimmickState,
    [CONTROLLER_HEALTHBOXUPDATE]          = BtlController_HandleHealthboxUpdate,
    [CONTROLLER_HIDEALLHEALTHBOXES]       = BtlController_HandleHideAllHealthboxes,
    [CONTROLLER_BATTLEFORMCHANGE]         = BtlController_HandleBattleFormChange,
    [CONTROLLER_PARTYFORMCHANGE]          = BtlController_HandlePartyFormChange,
    [CONTROLLER_ISPOCKETNOTEMPTY]         = BattleControllerComplete,
    [CONTROLLER_YESNOBOX]                 = BattleControllerComplete,
    [CONTROLLER_MONCAUGHTEFFECTS]         = BattleControllerComplete,
    [CONTROLLER_TERMINATOR_NOP]           = ControllerDummy,
};

void SetControllerToLinkPartner(u32 battlerId)
{
    gBattlerControllersData[battlerId].func = LinkPartnerBufferRunCommand;
    gBattlerControllersData[battlerId].endFunc = LinkPartnerBufferExecCompleted;
}

static void LinkPartnerBufferRunCommand(u32 battlerId)
{
    if (gBattleControllerExecFlags & Bit(battlerId))
    {
        if (gBattleBufferA[battlerId][0] < ARRAY_COUNT(sLinkPartnerBufferCommands))
            sLinkPartnerBufferCommands[gBattleBufferA[battlerId][0]](battlerId);
        else
            LinkPartnerBufferExecCompleted(battlerId);
    }
}

static void LinkPartnerBufferExecCompleted(u32 battlerId)
{
    gBattlerControllersData[battlerId].func = LinkPartnerBufferRunCommand;
    
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        u8 playerId = GetMultiplayerId();
        PrepareBufferDataTransferLink(battlerId, BUFFER_LINK, 4, &playerId);
        gBattleBufferA[battlerId][0] = CONTROLLER_TERMINATOR_NOP;
    }
    else
        gBattleControllerExecFlags &= ~(Bit(battlerId));
}

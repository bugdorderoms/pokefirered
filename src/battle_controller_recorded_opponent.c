#include "global.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "battle_gfx_sfx_util.h"
#include "battle_interface.h"
#include "link.h"
#include "m4a.h"
#include "recorded_battle.h"
#include "sound.h"

static void RecordedOpponentBufferRunCommand(u32 battlerId);
static void RecordedOpponentBufferExecCompleted(u32 battlerId);

static void (*const sRecordedOpponentBufferCommands[CONTROLLER_CMDS_COUNT])(u32) =
{
    [CONTROLLER_GETMONDATA]               = BtlController_HandleGetMonData,
    [CONTROLLER_SETMONDATA]               = BtlController_HandleSetMonData,
    [CONTROLLER_SETRAWMONDATA]            = BtlController_HandleSetRawMonData,
    [CONTROLLER_LOADMONSPRITE]            = LinkOpponentHandleLoadMonSprite,
    [CONTROLLER_SWITCHINANIM]             = LinkOpponentHandleSwitchInAnim,
    [CONTROLLER_RETURNMONTOBALL]          = BtlController_HandleReturnMonToBall,
    [CONTROLLER_DRAWTRAINERPIC]           = LinkOpponentHandleDrawTrainerPic,
    [CONTROLLER_TRAINERSLIDE]             = BattleControllerComplete,
    [CONTROLLER_TRAINERSLIDEBACK]         = OpponentHandleTrainerSlideBack,
    [CONTROLLER_FAINTANIMATION]           = BtlController_HandleFaintAnimation,
    [CONTROLLER_BALLTHROWANIM]            = BattleControllerComplete,
    [CONTROLLER_MOVEANIMATION]            = BtlController_HandleMoveAnimation,
    [CONTROLLER_PRINTSTRING]              = BtlController_HandlePrintString,
    [CONTROLLER_PRINTSELECTIONSTRING]     = BattleControllerComplete,
    [CONTROLLER_CHOOSEACTION]             = RecordedPlayerHandleChooseAction,
    [CONTROLLER_CHOOSEMOVE]               = RecordedPlayerHandleChooseMove,
    [CONTROLLER_OPENBAG]                  = BattleControllerComplete,
    [CONTROLLER_CHOOSEPOKEMON]            = RecordedPlayerHandleChoosePokemon,
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
    [CONTROLLER_INTROTRAINERBALLTHROW]    = LinkOpponentHandleIntroTrainerBallThrow,
    [CONTROLLER_DRAWPARTYSTATUSSUMMARY]   = BtlController_HandleDrawPartyStatusSummary,
    [CONTROLLER_HIDEPARTYSTATUSSUMMARY]   = BtlController_HandleHidePartyStatusSummary,
    [CONTROLLER_ENDBOUNCE]                = BattleControllerComplete,
    [CONTROLLER_SPRITEINVISIBILITY]       = BtlController_HandleSpriteInvisibility,
    [CONTROLLER_BATTLEANIMATION]          = BtlController_HandleBattleAnimation,
    [CONTROLLER_LINKSTANDBYMSG]           = BattleControllerComplete,
    [CONTROLLER_RESETACTIONMOVESELECTION] = BattleControllerComplete,
    [CONTROLLER_ENDLINKBATTLE]            = LinkOpponentHandleEndLinkBattle,
    [CONTROLLER_TERMINATOR_NOP]           = ControllerDummy,
};

void SetControllerToRecordedOpponent(u32 battlerId)
{
    gBattlerControllersData[battlerId].func = RecordedOpponentBufferRunCommand;
    gBattlerControllersData[battlerId].endFunc = RecordedOpponentBufferExecCompleted;
}

static void RecordedOpponentBufferRunCommand(u32 battlerId)
{
    if (gBattleControllerExecFlags & Bit(battlerId))
    {
        if (gBattleBufferA[battlerId][0] < ARRAY_COUNT(sRecordedOpponentBufferCommands))
            sRecordedOpponentBufferCommands[gBattleBufferA[battlerId][0]](battlerId);
        else
            RecordedOpponentBufferExecCompleted(battlerId);
    }
}

static void RecordedOpponentBufferExecCompleted(u32 battlerId)
{
    gBattlerControllersData[battlerId].func = RecordedOpponentBufferRunCommand;
    
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        u8 playerId = GetMultiplayerId();
        PrepareBufferDataTransferLink(battlerId, BUFFER_LINK, 4, &playerId);
        gBattleBufferA[battlerId][0] = CONTROLLER_TERMINATOR_NOP;
    }
    else
        gBattleControllerExecFlags &= ~(Bit(battlerId));
}

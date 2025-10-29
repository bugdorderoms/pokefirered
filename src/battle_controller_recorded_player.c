#include "global.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "battle_gfx_sfx_util.h"
#include "battle_interface.h"
#include "link.h"
#include "m4a.h"
#include "pokemon.h"
#include "recorded_battle.h"
#include "sound.h"

static void RecordedPlayerBufferRunCommand(u32 battlerId);
static void RecordedPlayerBufferExecCompleted(u32 battlerId);
static void RecordedPlayerHandleDrawTrainerPic(u32 battlerId);
static void RecordedPlayerHandleIntroTrainerBallThrow(u32 battlerId);

static void (*const sRecordedPlayerBufferCommands[CONTROLLER_CMDS_COUNT])(u32) =
{
    [CONTROLLER_GETMONDATA]               = BtlController_HandleGetMonData,
    [CONTROLLER_SETMONDATA]               = BtlController_HandleSetMonData,
    [CONTROLLER_SETRAWMONDATA]            = BtlController_HandleSetRawMonData,
    [CONTROLLER_LOADMONSPRITE]            = BtlController_HandleLoadMonSprite,
    [CONTROLLER_SWITCHINANIM]             = LinkOpponentHandleSwitchInAnim,
    [CONTROLLER_RETURNMONTOBALL]          = BtlController_HandleReturnMonToBall,
    [CONTROLLER_DRAWTRAINERPIC]           = RecordedPlayerHandleDrawTrainerPic,
    [CONTROLLER_TRAINERSLIDE]             = BattleControllerComplete,
    [CONTROLLER_TRAINERSLIDEBACK]         = OpponentHandleTrainerSlideBack,
    [CONTROLLER_FAINTANIMATION]           = BtlController_HandleFaintAnimation,
    [CONTROLLER_BALLTHROWANIM]            = PlayerHandleBallThrowAnim,
    [CONTROLLER_MOVEANIMATION]            = BtlController_HandleMoveAnimation,
    [CONTROLLER_PRINTSTRING]              = BtlController_HandlePrintString,
    [CONTROLLER_PRINTSELECTIONSTRING]     = BattleControllerComplete,
    [CONTROLLER_CHOOSEACTION]             = RecordedPlayerHandleChooseAction,
    [CONTROLLER_CHOOSEMOVE]               = RecordedPlayerHandleChooseMove,
    [CONTROLLER_OPENBAG]                  = BattleControllerComplete,
    [CONTROLLER_CHOOSEPOKEMON]            = RecordedPlayerHandleChoosePokemon,
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
    [CONTROLLER_INTROTRAINERBALLTHROW]    = RecordedPlayerHandleIntroTrainerBallThrow,
    [CONTROLLER_DRAWPARTYSTATUSSUMMARY]   = BtlController_HandleDrawPartyStatusSummary,
    [CONTROLLER_HIDEPARTYSTATUSSUMMARY]   = BtlController_HandleHidePartyStatusSummary,
    [CONTROLLER_ENDBOUNCE]                = BattleControllerComplete,
    [CONTROLLER_SPRITEINVISIBILITY]       = BtlController_HandleSpriteInvisibility,
    [CONTROLLER_BATTLEANIMATION]          = BtlController_HandleBattleAnimation,
    [CONTROLLER_LINKSTANDBYMSG]           = BattleControllerComplete,
    [CONTROLLER_RESETACTIONMOVESELECTION] = BattleControllerComplete,
    [CONTROLLER_ENDLINKBATTLE]            = PlayerHandleEndLinkBattle,
    [CONTROLLER_TERMINATOR_NOP]           = ControllerDummy,
};

void SetControllerToRecordedPlayer(u32 battlerId)
{
    gBattlerControllersData[battlerId].func = RecordedPlayerBufferRunCommand;
    gBattlerControllersData[battlerId].endFunc = RecordedPlayerBufferExecCompleted;
}

static void RecordedPlayerBufferRunCommand(u32 battlerId)
{
    if (gBattleControllerExecFlags & Bit(battlerId))
    {
        if (gBattleBufferA[battlerId][0] < ARRAY_COUNT(sRecordedPlayerBufferCommands))
            sRecordedPlayerBufferCommands[gBattleBufferA[battlerId][0]](battlerId);
        else
            RecordedPlayerBufferExecCompleted(battlerId);
    }
}

static void RecordedPlayerBufferExecCompleted(u32 battlerId)
{
    gBattlerControllersData[battlerId].func = RecordedPlayerBufferRunCommand;
    
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

static void RecordedPlayerHandleDrawTrainerPic(u32 battlerId)
{
    BtlController_HandleDrawTrainerPic(battlerId, gLinkPlayers[gRecordedBattleMultiplayerId].gender);
}

void RecordedPlayerHandleChooseAction(u32 battlerId)
{
    BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, RecordedBattle_GetBattlerAction(RECORDED_ACTION_TYPE, battlerId), 0);
    BattleControllerComplete(battlerId);
}

void RecordedPlayerHandleChooseMove(u32 battlerId)
{
    u32 moveIdx = RecordedBattle_GetBattlerAction(RECORDED_MOVE_SLOT, battlerId), target = RecordedBattle_GetBattlerAction(RECORDED_MOVE_TARGET, battlerId);
    BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, moveIdx | (target << 8));
    BattleControllerComplete(battlerId);
}

void RecordedPlayerHandleChoosePokemon(u32 battlerId)
{
    gBattleStruct->battlers[battlerId].monToSwitchIntoId = RecordedBattle_GetBattlerAction(RECORDED_PARTY_INDEX, battlerId);
    BtlController_EmitChosenMonReturnValue(battlerId, BUFFER_B, gBattleStruct->battlers[battlerId].monToSwitchIntoId, NULL);
    BattleControllerComplete(battlerId);
}

static void RecordedPlayerHandleIntroTrainerBallThrow(u32 battlerId)
{
    BtlController_HandleIntroTrainerBallThrow(battlerId, 0xD6F9, gLinkPlayers[GetBattlerMultiplayerId(battlerId)].gender, StartAnimLinearTranslation, 24, Intro_TryShinyAnimShowHealthbox);
}

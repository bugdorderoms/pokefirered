#include "global.h"
#include "battle.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "item_menu.h"
#include "link.h"
#include "palette.h"
#include "sound.h"
#include "strings.h"
#include "util.h"
#include "constants/battle.h"
#include "constants/battle_anim.h"
#include "constants/songs.h"

static void SafariBufferRunCommand(u8 battlerId);
static void SafariBufferExecCompleted(u8 battlerId);
static void SafariHandleChooseAction(u8 battlerId);
static void HandleInputChooseAction(u8 battlerId);
static void SafariHandleChooseItem(u8 battlerId);
static void SafariHandleStatusIconUpdate(u8 battlerId);
static void SafariHandleIntroTrainerBallThrow(u8 battlerId);

static void (*const sSafariBufferCommands[CONTROLLER_CMDS_COUNT])(u8) =
{
    [CONTROLLER_GETMONDATA]               = BtlController_HandleGetMonData,
	[CONTROLLER_SETMONDATA]               = BtlController_HandleSetMonData,
	[CONTROLLER_SETRAWMONDATA]            = BtlController_HandleSetRawMonData,
	[CONTROLLER_LOADMONSPRITE]            = BattleControllerComplete,
	[CONTROLLER_SWITCHINANIM]             = BattleControllerComplete,
	[CONTROLLER_RETURNMONTOBALL]          = BattleControllerComplete,
	[CONTROLLER_DRAWTRAINERPIC]           = PlayerHandleDrawTrainerPic,
	[CONTROLLER_TRAINERSLIDE]             = BattleControllerComplete,
	[CONTROLLER_TRAINERSLIDEBACK]         = BattleControllerComplete,
	[CONTROLLER_FAINTANIMATION]           = BattleControllerComplete,
	[CONTROLLER_BALLTHROWANIM]            = SafariHandleBallThrowAnim,
	[CONTROLLER_MOVEANIMATION]            = BattleControllerComplete,
	[CONTROLLER_PRINTSTRING]              = PlayerHandlePrintString,
	[CONTROLLER_PRINTSELECTIONSTRING]     = PlayerHandlePrintSelectionString,
	[CONTROLLER_CHOOSEACTION]             = SafariHandleChooseAction,
	[CONTROLLER_CHOOSEMOVE]               = BattleControllerComplete,
	[CONTROLLER_OPENBAG]                  = SafariHandleChooseItem,
	[CONTROLLER_CHOOSEPOKEMON]            = BattleControllerComplete,
	[CONTROLLER_HEALTHBARUPDATE]          = BattleControllerComplete,
	[CONTROLLER_EXPUPDATE]                = BattleControllerComplete,
	[CONTROLLER_STATUSICONUPDATE]         = SafariHandleStatusIconUpdate,
	[CONTROLLER_STATUSANIMATION]          = BattleControllerComplete,
	[CONTROLLER_DATATRANSFER]             = BattleControllerComplete,
	[CONTROLLER_TWORETURNVALUES]          = BattleControllerComplete,
	[CONTROLLER_CHOSENMONRETURNVALUE]     = BattleControllerComplete,
	[CONTROLLER_ONERETURNVALUE]           = BattleControllerComplete,
	[CONTROLLER_EFFECTIVENESSSOUND]       = BtlController_HandlePlaySE,
	[CONTROLLER_PLAYFANFAREORBGM]         = BtlController_HandlePlayFanfare,
	[CONTROLLER_FAINTINGCRY]              = SafariHandleFaintingCry,
	[CONTROLLER_INTROSLIDE]               = BtlController_HandleIntroSlide,
	[CONTROLLER_INTROTRAINERBALLTHROW]    = SafariHandleIntroTrainerBallThrow,
	[CONTROLLER_DRAWPARTYSTATUSSUMMARY]   = BattleControllerComplete,
	[CONTROLLER_HIDEPARTYSTATUSSUMMARY]   = BattleControllerComplete,
	[CONTROLLER_ENDBOUNCE]                = BattleControllerComplete,
	[CONTROLLER_SPRITEINVISIBILITY]       = BattleControllerComplete,
	[CONTROLLER_BATTLEANIMATION]          = BtlController_HandleBattleAnimation,
	[CONTROLLER_LINKSTANDBYMSG]           = BattleControllerComplete,
	[CONTROLLER_RESETACTIONMOVESELECTION] = BattleControllerComplete,
	[CONTROLLER_ENDLINKBATTLE]            = OakOldManHandleEndLinkBattle,
	[CONTROLLER_TERMINATOR_NOP]           = ControllerDummy,
};

void SetControllerToSafari(u8 battlerId)
{
	gBattlerControllerFuncs[battlerId] = SafariBufferRunCommand;
	gBattlerControllerEndFuncs[battlerId] = SafariBufferExecCompleted;
}

static void SafariBufferRunCommand(u8 battlerId)
{
    if (gBattleControllerExecFlags & gBitTable[battlerId])
    {
        if (gBattleBufferA[battlerId][0] < ARRAY_COUNT(sSafariBufferCommands))
            sSafariBufferCommands[gBattleBufferA[battlerId][0]](battlerId);
        else
            SafariBufferExecCompleted(battlerId);
    }
}

static void SafariBufferExecCompleted(u8 battlerId)
{
    gBattlerControllerFuncs[battlerId] = SafariBufferRunCommand;
	
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

void SafariHandleBallThrowAnim(u8 battlerId)
{
	BtlController_HandleBallThrowAnim(battlerId, B_ANIM_SAFARI_BALL_THROW);
}

static void HandleChooseActionAfterDma3(u8 battlerId)
{
    if (!IsDma3ManagerBusyWithBgCopy())
    {
        gBattle_BG0_X = 0;
        gBattle_BG0_Y = 160;
        gBattlerControllerFuncs[battlerId] = HandleInputChooseAction;
    }
}

static void SafariHandleChooseAction(u8 battlerId)
{
	BtlController_HandleChooseAction(battlerId, gText_SafariMenu, gText_WhatWillPlayerThrow, HandleChooseActionAfterDma3);
}

static void CompleteWhenChoosePokeblock(u8 battlerId)
{
    if (gMain.callback2 == BattleMainCB2 && !gPaletteFade.active)
    {
        BtlController_EmitOneReturnValue(battlerId, BUFFER_B, gSpecialVar_ItemId);
        BattleControllerComplete(battlerId);
    }
}

static void SafariOpenPokeblockCase(u8 battlerId)
{
	if (!gPaletteFade.active)
        gBattlerControllerFuncs[battlerId] = CompleteWhenChoosePokeblock;
}

static void SafariHandleChooseItem(u8 battlerId)
{
	BtlController_HandleChooseItem(battlerId, SafariOpenPokeblockCase);
}

static void SafariHandleStatusIconUpdate(u8 battlerId)
{
	UpdateHealthboxAttribute(battlerId, HEALTHBOX_SAFARI_BALLS_TEXT);
    BattleControllerComplete(battlerId);
}

void SafariHandleFaintingCry(u8 battlerId)
{
    PlayCry1(GetMonData(GetBattlerPartyIndexPtr(battlerId), MON_DATA_SPECIES), 25);
    BattleControllerComplete(battlerId);
}

static void CompleteOnHealthboxSpriteCallbackDummy(u8 battlerId)
{
    if (gSprites[gHealthboxSpriteIds[battlerId]].callback == SpriteCallbackDummy)
        BattleControllerComplete(battlerId);
}

static void SafariHandleIntroTrainerBallThrow(u8 battlerId)
{
	UpdateHealthboxAttribute(battlerId, HEALTHBOX_SAFARI_ALL_TEXT);
    StartHealthboxSlideIn(battlerId);
    SetHealthboxSpriteVisible(gHealthboxSpriteIds[battlerId]);
    gBattlerControllerFuncs[battlerId] = CompleteOnHealthboxSpriteCallbackDummy;
}

///////////
// INPUT //
///////////

static void HandleInputChooseAction(u8 battlerId)
{
	if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);

        switch (gActionSelectionCursor[battlerId])
        {
        case 0:
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_SAFARI_BALL, 0);
            break;
        case 1:
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_SAFARI_BAIT, 0);
            break;
        case 2:
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_SAFARI_GO_NEAR, 0);
            break;
        case 3:
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_SAFARI_RUN, 0);
            break;
        }
        BattleControllerComplete(battlerId);
    }
    else if (JOY_NEW(DPAD_LEFT))
    {
        if (gActionSelectionCursor[battlerId] & 1)
        {
            PlaySE(SE_SELECT);
            ActionSelectionDestroyCursorAt(gActionSelectionCursor[battlerId]);
            gActionSelectionCursor[battlerId] ^= 1;
            ActionSelectionCreateCursorAt(gActionSelectionCursor[battlerId]);
        }
    }
    else if (JOY_NEW(DPAD_RIGHT))
    {
        if (!(gActionSelectionCursor[battlerId] & 1))
        {
            PlaySE(SE_SELECT);
            ActionSelectionDestroyCursorAt(gActionSelectionCursor[battlerId]);
            gActionSelectionCursor[battlerId] ^= 1;
            ActionSelectionCreateCursorAt(gActionSelectionCursor[battlerId]);
        }
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if (gActionSelectionCursor[battlerId] & 2)
        {
            PlaySE(SE_SELECT);
            ActionSelectionDestroyCursorAt(gActionSelectionCursor[battlerId]);
            gActionSelectionCursor[battlerId] ^= 2;
            ActionSelectionCreateCursorAt(gActionSelectionCursor[battlerId]);
        }
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (!(gActionSelectionCursor[battlerId] & 2))
        {
            PlaySE(SE_SELECT);
            ActionSelectionDestroyCursorAt(gActionSelectionCursor[battlerId]);
            gActionSelectionCursor[battlerId] ^= 2;
            ActionSelectionCreateCursorAt(gActionSelectionCursor[battlerId]);
        }
    }
}

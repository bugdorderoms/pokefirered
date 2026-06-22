#include "global.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "battle_gfx_sfx_util.h"
#include "link.h"
#include "m4a.h"
#include "sound.h"
#include "util.h"
#include "constants/battle_anim.h"
#include "gba/m4a_internal.h"

static void LinkOpponentBufferRunCommand(u32 battlerId);
static void LinkOpponentBufferExecCompleted(u32 battlerId);
static void LinkOpponentHandleHealthbarUpdate(u32 battlerId);

static void (*const sLinkOpponentBufferCommands[CONTROLLER_CMDS_COUNT])(u32) =
{
    [CONTROLLER_GETMONDATA]               = BtlController_HandleGetMonData,
    [CONTROLLER_SETMONDATA]               = BtlController_HandleSetMonData,
    [CONTROLLER_LOADMONSPRITE]            = BtlController_HandleLoadMonSprite,
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
    [CONTROLLER_CHOOSEACTION]             = BattleControllerComplete,
    [CONTROLLER_CHOOSEMOVE]               = BattleControllerComplete,
    [CONTROLLER_OPENBAG]                  = BattleControllerComplete,
    [CONTROLLER_CHOOSEPOKEMON]            = BattleControllerComplete,
    [CONTROLLER_HEALTHBARUPDATE]          = LinkOpponentHandleHealthbarUpdate,
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

void SetControllerToLinkOpponent(u32 battlerId)
{
    gBattlerControllersData[battlerId].func = LinkOpponentBufferRunCommand;
    gBattlerControllersData[battlerId].endFunc = LinkOpponentBufferExecCompleted;
}

static void LinkOpponentBufferRunCommand(u32 battlerId)
{
    if (gBattleControllerExecFlags & Bit(battlerId))
    {
        if (gBattleBufferA[battlerId][0] < ARRAY_COUNT(sLinkOpponentBufferCommands))
            sLinkOpponentBufferCommands[gBattleBufferA[battlerId][0]](battlerId);
        else
            LinkOpponentBufferExecCompleted(battlerId);
    }
}

static void LinkOpponentBufferExecCompleted(u32 battlerId)
{
    gBattlerControllersData[battlerId].func = LinkOpponentBufferRunCommand;
    
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

void LinkOpponentHandleSwitchInAnim(u32 battlerId)
{
    BtlController_HandleSwitchInAnim(battlerId, SwitchIn_TryShinyAnim);
}

void LinkOpponentHandleDrawTrainerPic(u32 battlerId)
{
    u32 playerId, trainerPicId;
    
    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        playerId = GetBattlerMultiplayerId(battlerId);
        
        if (IsLinkPlayerFromHoenn(playerId))
            trainerPicId = FacilityClassToPicIndex(gLinkPlayers[playerId].gender != MALE ? FACILITY_CLASS_PKMN_TRAINER_MAY : FACILITY_CLASS_PKMN_TRAINER_BRENDAN);
        else
            trainerPicId = FacilityClassToPicIndex(gLinkPlayers[playerId].gender != MALE ? FACILITY_CLASS_LEAF : FACILITY_CLASS_RED);
    }
    else
    {
        if (gTrainerBattleOpponent_A == TRAINER_UNION_ROOM)
            trainerPicId = GetUnionRoomTrainerPic();
        else
        {
            playerId = BATTLE_OPPOSITE(GetMultiplayerId());
            
            if (IsLinkPlayerFromHoenn(playerId))
                trainerPicId = FacilityClassToPicIndex(gLinkPlayers[playerId].gender != MALE ? FACILITY_CLASS_PKMN_TRAINER_MAY : FACILITY_CLASS_PKMN_TRAINER_BRENDAN);
            else
                trainerPicId = FacilityClassToPicIndex(gLinkPlayers[playerId].gender != MALE ? FACILITY_CLASS_LEAF : FACILITY_CLASS_RED);
        }
    }
    BtlController_HandleDrawTrainerPic(battlerId, trainerPicId);
}

static void LinkOpponentHandleHealthbarUpdate(u32 battlerId)
{
    BtlController_HandleHealthbarUpdate(battlerId, FALSE, TRUE);
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
        if (GetBattlerPosition(battlerId) == B_POSITION_OPPONENT_LEFT)
        {
            if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim || !gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].finishedShinyMonAnim)
                return;
                
            gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim = FALSE;
            gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim = FALSE;
            gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].triedShinyMonAnim = FALSE;
            gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].finishedShinyMonAnim = FALSE;
            
            FreeSpriteTilesByTag(ANIM_TAG_GOLD_STARS);
            FreeSpritePaletteByTag(ANIM_TAG_GOLD_STARS);
        }
        
        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
        {
            if (GetBattlerPosition(battlerId) == B_POSITION_OPPONENT_LEFT)
                m4aMPlayContinue(&gMPlayInfo_BGM);
        }
        else
            m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 256);

        gBattleSpritesDataPtr->healthBoxesData[battlerId].introEndDelay = 3;
        gBattlerControllersData[battlerId].func = Intro_DelayAndEnd;
    }
}

static void Intro_ShowHealthbox(u32 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive && !gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].ballAnimActive)
    {
        if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim)
            TryShinyAnimation(battlerId);
        
        if (!gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].triedShinyMonAnim)
            TryShinyAnimation(BATTLE_PARTNER(battlerId));
        
        if ((gBattleTypeFlags & BATTLE_TYPE_MULTI) && GetBattlerPosition(battlerId) == B_POSITION_OPPONENT_RIGHT)
        {
            if (++gBattleSpritesDataPtr->healthBoxesData[battlerId].introEndDelay == 1)
                return;
            
            gBattleSpritesDataPtr->healthBoxesData[battlerId].introEndDelay = 0;
        }
        
        if (IsDoubleBattleForBattler(battlerId) && IsPartnerMonFromSameTrainer(battlerId))
            ShowHealthBox(BATTLE_PARTNER(battlerId));
        
        ShowHealthBox(battlerId);
        gBattleSpritesDataPtr->animationData->healthboxSlideInStarted = FALSE;
        gBattlerControllersData[battlerId].func = Intro_WaitForHealthbox;
    }
}

void LinkOpponentHandleIntroTrainerBallThrow(u32 battlerId)
{
    BtlController_HandleIntroTrainerBallThrow(battlerId, 0x0000, 0, StartAnimLinearTranslation, 0, Intro_ShowHealthbox);
}

void LinkOpponentHandleEndLinkBattle(u32 battlerId)
{
    BtlController_HandleEndLinkBattle(battlerId, gBattleBufferA[battlerId][1] == B_OUTCOME_DREW ? gBattleBufferA[battlerId][1] : gBattleBufferA[battlerId][1] ^ B_OUTCOME_DREW, SetBattleEndCallbacks);
}

void LinkOpponentHandleGimmickState(u32 battlerId)
{
    BtlController_HandleGimmickState(battlerId, FALSE);
}

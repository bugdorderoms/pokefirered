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
static void LinkPartnerHandleDrawTrainerPic(u32 battlerId);
static void LinkPartnerHandleTrainerSlideBack(u32 battlerId);
static void LinkPartnerHandleIntroTrainerBallThrow(u32 battlerId);

static void (*const sLinkPartnerBufferCommands[CONTROLLER_CMDS_COUNT])(u32) =
{
    [CONTROLLER_GETMONDATA]               = BtlController_HandleGetMonData,
    [CONTROLLER_SETMONDATA]               = BtlController_HandleSetMonData,
    [CONTROLLER_SETRAWMONDATA]            = BtlController_HandleSetRawMonData,
    [CONTROLLER_LOADMONSPRITE]            = LinkPartnerHandleLoadMonSprite,
    [CONTROLLER_SWITCHINANIM]             = LinkPartnerHandleSwitchInAnim,
    [CONTROLLER_RETURNMONTOBALL]          = BtlController_HandleReturnMonToBall,
    [CONTROLLER_DRAWTRAINERPIC]           = LinkPartnerHandleDrawTrainerPic,
    [CONTROLLER_TRAINERSLIDE]             = BattleControllerComplete,
    [CONTROLLER_TRAINERSLIDEBACK]         = LinkPartnerHandleTrainerSlideBack,
    [CONTROLLER_FAINTANIMATION]           = BtlController_HandleFaintAnimation,
    [CONTROLLER_BALLTHROWANIM]            = BattleControllerComplete,
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
    [CONTROLLER_INTROTRAINERBALLTHROW]    = LinkPartnerHandleIntroTrainerBallThrow,
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

////////////////////////
// BATTLE CONTROLLERS //
////////////////////////

void LinkPartnerHandleLoadMonSprite(u32 battlerId)
{
    BtlController_HandleLoadMonSprite(battlerId, FALSE, WaitForMonAnimAfterLoad);
}

static void SwitchIn_WaitAndEnd(u32 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].specialAnimActive)
        BattleControllerComplete(battlerId);
}

static void SwitchIn_ShowSubstitute(u32 battlerId)
{
    if (gSprites[gHealthboxSpriteIds[battlerId]].callback == SpriteCallbackDummy)
    {
        if (gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute)
            InitAndLaunchSpecialAnimation(battlerId, battlerId, B_ANIM_MON_TO_SUBSTITUTE);
        
        gBattlerControllersData[battlerId].func = SwitchIn_WaitAndEnd;
    }
}

static void SwitchIn_ShowHealthbox(u32 battlerId)
{
    if (gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim)
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim = FALSE;
        gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim = FALSE;
        FreeSpriteTilesByTag(ANIM_TAG_GOLD_STARS);
        FreeSpritePaletteByTag(ANIM_TAG_GOLD_STARS);
        CreateTask(Task_BltController_RestoreBgmAfterCry, 10);
        HandleLowHpMusicChange(&gPlayerParty[gBattlerPartyIndexes[battlerId]], battlerId);
        StartSpriteAnim(&gSprites[gBattlerSpriteIds[battlerId]], 0);
        UpdateHealthboxAttribute(battlerId, HEALTHBOX_ALL);
        StartHealthboxSlideIn(battlerId);
        SetHealthboxSpriteVisible(gHealthboxSpriteIds[battlerId]);
        CopyBattleSpriteInvisibility(battlerId);
        gBattlerControllersData[battlerId].func = SwitchIn_ShowSubstitute;
    }
}

static void SwitchIn_TryShinyAnim(u32 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim && !gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive)
        TryShinyAnimation(battlerId);
    
    if (gSprites[gBattlerControllersData[battlerId].data].callback == SpriteCallbackDummy && !gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive)
    {
        DestroySprite(&gSprites[gBattlerControllersData[battlerId].data]);
        gBattlerControllersData[battlerId].func = SwitchIn_ShowHealthbox;
    }
}

void LinkPartnerHandleSwitchInAnim(u32 battlerId)
{
    BtlController_HandleSwitchInAnim(battlerId, TRUE, SwitchIn_TryShinyAnim);
}

static u32 GetLinkPartnerTrainerPicId(u32 battlerId)
{
    return GetPlayerTrainerPicId(GetBattlerMultiplayerId(battlerId));
}

static void LinkPartnerHandleDrawTrainerPic(u32 battlerId)
{
    s16 xPos;

    if ((GetBattlerPosition(battlerId) & BIT_FLANK) != B_FLANK_LEFT) // Second mon, on the right.
        xPos = 90;
    else // First mon, on the left.
        xPos = 32;
    
    BtlController_HandleDrawTrainerPic(battlerId, GetLinkPartnerTrainerPicId(battlerId), xPos, GetBattlerSpriteSubpriority(battlerId));
}

static void LinkPartnerHandleTrainerSlideBack(u32 battlerId)
{
    BtlController_HandleTrainerSlideBack(battlerId, 35, FALSE);
}

static void Intro_WaitForHealthbox(u32 battlerId)
{
    bool32 var = FALSE;

    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE) || ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && (gBattleTypeFlags & BATTLE_TYPE_MULTI)))
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
            
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !(gBattleTypeFlags & BATTLE_TYPE_MULTI))
                ShowHealthBox(BATTLE_PARTNER(battlerId));

            ShowHealthBox(battlerId);
            gBattleSpritesDataPtr->animationData->healthboxSlideInStarted = FALSE;
            gBattlerControllersData[battlerId].func = Intro_WaitForHealthbox;
        }
    }
}

static void LinkPartnerHandleIntroTrainerBallThrow(u32 battlerId)
{
    BtlController_HandleIntroTrainerBallThrow(battlerId, 0xD6F9, GetLinkPartnerTrainerPicId(battlerId), StartAnimLinearTranslation, 24, Intro_ShowHealthbox);
}

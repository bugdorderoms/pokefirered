#include "global.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "link.h"
#include "sound.h"
#include "util.h"
#include "constants/battle_anim.h"

static void LinkPartnerBufferRunCommand(u8 battlerId);
static void LinkPartnerBufferExecCompleted(u8 battlerId);
static void LinkPartnerHandleLoadMonSprite(u8 battlerId);
static void LinkPartnerHandleSwitchInAnim(u8 battlerId);
static void LinkPartnerHandleDrawTrainerPic(u8 battlerId);
static void LinkPartnerHandleTrainerSlideBack(u8 battlerId);
static void LinkPartnerHandleIntroTrainerBallThrow(u8 battlerId);

static void (*const sLinkPartnerBufferCommands[CONTROLLER_CMDS_COUNT])(u8) =
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
	[CONTROLLER_PRINTSTRING]              = PlayerHandlePrintString,
	[CONTROLLER_PRINTSELECTIONSTRING]     = BattleControllerComplete,
	[CONTROLLER_CHOOSEACTION]             = BattleControllerComplete,
	[CONTROLLER_CHOOSEMOVE]               = BattleControllerComplete,
	[CONTROLLER_OPENBAG]                  = BattleControllerComplete,
	[CONTROLLER_CHOOSEPOKEMON]            = BattleControllerComplete,
	[CONTROLLER_HEALTHBARUPDATE]          = OpponentHandleHealthbarUpdate,
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

void SetControllerToLinkPartner(u8 battlerId)
{
	gBattlerControllerFuncs[battlerId] = LinkPartnerBufferRunCommand;
	gBattlerControllerEndFuncs[battlerId] = LinkPartnerBufferExecCompleted;
}

static void LinkPartnerBufferRunCommand(u8 battlerId)
{
    if (gBattleControllerExecFlags & gBitTable[battlerId])
    {
        if (gBattleBufferA[battlerId][0] < ARRAY_COUNT(sLinkPartnerBufferCommands))
            sLinkPartnerBufferCommands[gBattleBufferA[battlerId][0]](battlerId);
        else
            LinkPartnerBufferExecCompleted(battlerId);
    }
}

static void LinkPartnerBufferExecCompleted(u8 battlerId)
{
	gBattlerControllerFuncs[battlerId] = LinkPartnerBufferRunCommand;
	
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

static void LinkPartnerHandleLoadMonSprite(u8 battlerId)
{
    BtlController_HandleLoadMonSprite(battlerId, FALSE, WaitForMonAnimAfterLoad);
}

static void SwitchIn_WaitAndEnd(u8 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].specialAnimActive)
        BattleControllerComplete(battlerId);
}

static void SwitchIn_ShowSubstitute(u8 battlerId)
{
    if (gSprites[gHealthboxSpriteIds[battlerId]].callback == SpriteCallbackDummy)
    {
        if (gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute)
            InitAndLaunchSpecialAnimation(battlerId, battlerId, B_ANIM_MON_TO_SUBSTITUTE);
		
        gBattlerControllerFuncs[battlerId] = SwitchIn_WaitAndEnd;
    }
}

static void SwitchIn_ShowHealthbox(u8 battlerId)
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
        gBattlerControllerFuncs[battlerId] = SwitchIn_ShowSubstitute;
    }
}

static void SwitchIn_TryShinyAnim(u8 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim && !gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive)
        TryShinyAnimation(battlerId);
	
    if (gSprites[gBattleControllerData[battlerId]].callback == SpriteCallbackDummy && !gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive)
    {
        DestroySprite(&gSprites[gBattleControllerData[battlerId]]);
        gBattlerControllerFuncs[battlerId] = SwitchIn_ShowHealthbox;
    }
}

static void LinkPartnerHandleSwitchInAnim(u8 battlerId)
{
	BtlController_HandleSwitchInAnim(battlerId, TRUE, SwitchIn_TryShinyAnim);
}

static u32 GetLinkPartnerTrainerPicId(u8 battlerId)
{
	u8 playerId = GetBattlerMultiplayerId(battlerId);
	
    if ((gLinkPlayers[playerId].version & 0xFF) == VERSION_RUBY || (gLinkPlayers[playerId].version & 0xFF) == VERSION_SAPPHIRE
     || (gLinkPlayers[playerId].version & 0xFF) == VERSION_EMERALD)
        return gLinkPlayers[playerId].gender + TRAINER_BACK_PIC_RS_BRENDAN;
    else
        return gLinkPlayers[playerId].gender + TRAINER_BACK_PIC_RED;
}

static void LinkPartnerHandleDrawTrainerPic(u8 battlerId)
{
	s16 xPos;
    u32 trainerPicId = GetLinkPartnerTrainerPicId(battlerId);

    if ((GetBattlerPosition(battlerId) & BIT_FLANK) != B_FLANK_LEFT) // Second mon, on the right.
        xPos = 90;
    else // First mon, on the left.
        xPos = 32;
	
	BtlController_HandleDrawTrainerPic(battlerId, trainerPicId, FALSE, xPos, (8 - gTrainerBackPicTable[trainerPicId].coords.size) * 4 + 80, GetBattlerSpriteSubpriority(battlerId));
}

static void LinkPartnerHandleTrainerSlideBack(u8 battlerId)
{
	BtlController_HandleTrainerSlideBack(battlerId, 35, FALSE);
}

static void Intro_DelayAndEnd(u8 battlerId)
{
    if (--gBattleSpritesDataPtr->healthBoxesData[battlerId].introEndDelay == 0xFF)
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].introEndDelay = 0;
        BattleControllerComplete(battlerId);
    }
}

static void Intro_WaitForHealthbox(u8 battlerId)
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
        gBattlerControllerFuncs[battlerId] = Intro_DelayAndEnd;
    }
}

static void Intro_ShowHealthbox(u8 battlerId)
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
            gBattlerControllerFuncs[battlerId] = Intro_WaitForHealthbox;
        }
    }
}

static void LinkPartnerHandleIntroTrainerBallThrow(u8 battlerId)
{
	BtlController_HandleIntroTrainerBallThrow(battlerId, 0xD6F9, GetLinkPartnerTrainerPicId(battlerId), StartAnimLinearTranslation, 24, Intro_ShowHealthbox);
}

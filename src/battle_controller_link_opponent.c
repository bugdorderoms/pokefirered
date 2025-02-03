#include "global.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "link.h"
#include "m4a.h"
#include "sound.h"
#include "util.h"
#include "constants/battle_anim.h"
#include "gba/m4a_internal.h"

static void LinkOpponentBufferRunCommand(u8 battlerId);
static void LinkOpponentBufferExecCompleted(u8 battlerId);
static void LinkOpponentHandleLoadMonSprite(u8 battlerId);
static void LinkOpponentHandleSwitchInAnim(u8 battlerId);
static void LinkOpponentHandleDrawTrainerPic(u8 battlerId);
static void LinkOpponentHandleIntroTrainerBallThrow(u8 battlerId);
static void LinkOpponentHandleEndLinkBattle(u8 battlerId);

static void (*const sLinkOpponentBufferCommands[CONTROLLER_CMDS_COUNT])(u8) =
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

void SetControllerToLinkOpponent(u8 battlerId)
{
	gBattlerControllerFuncs[battlerId] = LinkOpponentBufferRunCommand;
	gBattlerControllerEndFuncs[battlerId] = LinkOpponentBufferExecCompleted;
}

static void LinkOpponentBufferRunCommand(u8 battlerId)
{
    if (gBattleControllerExecFlags & gBitTable[battlerId])
    {
        if (gBattleBufferA[battlerId][0] < ARRAY_COUNT(sLinkOpponentBufferCommands))
            sLinkOpponentBufferCommands[gBattleBufferA[battlerId][0]](battlerId);
        else
            LinkOpponentBufferExecCompleted(battlerId);
    }
}

static void LinkOpponentBufferExecCompleted(u8 battlerId)
{
	gBattlerControllerFuncs[battlerId] = LinkOpponentBufferRunCommand;
	
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

static void LinkOpponentHandleLoadMonSprite(u8 battlerId)
{
	BtlController_HandleLoadMonSprite(battlerId, FALSE, TryShinyAnimAfterMonAnim);
}

static void SwitchIn_HandleSoundAndEnd(u8 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].specialAnimActive && !IsCryPlayingOrClearCrySongs())
    {
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 0x100);
        BattleControllerComplete(battlerId);
    }
}

static void SwitchIn_ShowSubstitute(u8 battlerId)
{
    if (gSprites[gHealthboxSpriteIds[battlerId]].callback == SpriteCallbackDummy)
    {
        if (gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute)
            InitAndLaunchSpecialAnimation(battlerId, battlerId, B_ANIM_MON_TO_SUBSTITUTE);
		
        gBattlerControllerFuncs[battlerId] = SwitchIn_HandleSoundAndEnd;
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
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive && !gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim)
        TryShinyAnimation(battlerId);

    if (gSprites[gBattleControllerData[battlerId]].callback == SpriteCallbackDummy && !gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive)
    {
        DestroySprite(&gSprites[gBattleControllerData[battlerId]]);
        SetBattlerShadowSpriteCallback(battlerId);
        gBattlerControllerFuncs[battlerId] = SwitchIn_ShowHealthbox;
    }
}

static void LinkOpponentHandleSwitchInAnim(u8 battlerId)
{
	BtlController_HandleSwitchInAnim(battlerId, FALSE, SwitchIn_TryShinyAnim);
}

static void LinkOpponentHandleDrawTrainerPic(u8 battlerId)
{
	u8 playerId;
    s16 xPos;
    u32 trainerPicId;

    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        if (GetBattlerPosition(battlerId) & BIT_FLANK) // second mon
            xPos = 152;
        else // first mon
            xPos = 200;
			
		playerId = GetBattlerMultiplayerId(battlerId);
		
        if ((gLinkPlayers[playerId].version & 0xFF) == VERSION_RUBY || (gLinkPlayers[playerId].version & 0xFF) == VERSION_SAPPHIRE
         || (gLinkPlayers[playerId].version & 0xFF) == VERSION_EMERALD)
			trainerPicId = gFacilityClassToPicIndex[gLinkPlayers[playerId].gender != MALE ? FACILITY_CLASS_PKMN_TRAINER_MAY : FACILITY_CLASS_PKMN_TRAINER_BRENDAN];
        else
			trainerPicId = gFacilityClassToPicIndex[gLinkPlayers[playerId].gender != MALE ? FACILITY_CLASS_LEAF : FACILITY_CLASS_RED];
    }
    else
    {
        xPos = 176;
		
        if (gTrainerBattleOpponent_A == TRAINER_UNION_ROOM)
            trainerPicId = GetUnionRoomTrainerPic();
		else
		{
			playerId = BATTLE_OPPOSITE(GetMultiplayerId());
			
			if ((gLinkPlayers[playerId].version & 0xFF) == VERSION_RUBY || (gLinkPlayers[playerId].version & 0xFF) == VERSION_SAPPHIRE
              || (gLinkPlayers[playerId].version & 0xFF) == VERSION_EMERALD)
				trainerPicId = gFacilityClassToPicIndex[gLinkPlayers[playerId].gender != MALE ? FACILITY_CLASS_PKMN_TRAINER_MAY : FACILITY_CLASS_PKMN_TRAINER_BRENDAN];
			else
				trainerPicId = gFacilityClassToPicIndex[gLinkPlayers[playerId].gender != MALE ? FACILITY_CLASS_LEAF : FACILITY_CLASS_RED];
		}
	}
	BtlController_HandleDrawTrainerPic(battlerId, trainerPicId, TRUE, xPos, (8 - gTrainerFrontPicTable[trainerPicId].coords.size) * 4 + 40, GetBattlerSpriteSubpriority(battlerId));
}

static void Intro_DelayAndEnd(u8 battlerId)
{
    if (--gBattleSpritesDataPtr->healthBoxesData[battlerId].introEndDelay == 0xFF)
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].introEndDelay = 0;
        BattleControllerComplete(battlerId);
    }
}

static void Intro_WaitForShinyAnimAndHealthbox(u8 battlerId)
{
    bool8 var = FALSE;

    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE) || (((gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && (gBattleTypeFlags & BATTLE_TYPE_MULTI))))
    {
        if (gSprites[gHealthboxSpriteIds[battlerId]].callback == SpriteCallbackDummy)
            var = TRUE;
    }
    else if (gSprites[gHealthboxSpriteIds[battlerId]].callback == SpriteCallbackDummy && gSprites[gHealthboxSpriteIds[BATTLE_PARTNER(battlerId)]].callback == gSprites[gHealthboxSpriteIds[battlerId]].callback)
        var = TRUE;

    if (IsCryPlayingOrClearCrySongs())
        var = FALSE;
	
    if (var)
    {
        if (GetBattlerPosition(battlerId) == B_POSITION_OPPONENT_LEFT)
        {
            if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim
             || !gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].finishedShinyMonAnim)
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
        gBattlerControllerFuncs[battlerId] = Intro_DelayAndEnd;
    }
}

static void Intro_TryShinyAnimShowHealthbox(u8 battlerId)
{
	if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive && !gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].ballAnimActive)
    {
        if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim)
            TryShinyAnimation(battlerId);
		
        if (!gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].triedShinyMonAnim)
            TryShinyAnimation(BATTLE_PARTNER(battlerId));
		
        if (gBattleTypeFlags & BATTLE_TYPE_MULTI && GetBattlerPosition(battlerId) == 3)
        {
            if (++gBattleSpritesDataPtr->healthBoxesData[battlerId].introEndDelay == 1)
                return;
            gBattleSpritesDataPtr->healthBoxesData[battlerId].introEndDelay = 0;
        }
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !(gBattleTypeFlags & BATTLE_TYPE_MULTI))
			ShowHealthBox(BATTLE_PARTNER(battlerId));
		
        ShowHealthBox(battlerId);
        gBattleSpritesDataPtr->animationData->healthboxSlideInStarted = FALSE;
        gBattlerControllerFuncs[battlerId] = Intro_WaitForShinyAnimAndHealthbox;
    }
}

static void LinkOpponentHandleIntroTrainerBallThrow(u8 battlerId)
{
	BtlController_HandleIntroTrainerBallThrow(battlerId, 0x0000, 0, StartAnimLinearTranslation, 0, Intro_TryShinyAnimShowHealthbox);
}

static void LinkOpponentHandleEndLinkBattle(u8 battlerId)
{
	BtlController_HandleEndLinkBattle(battlerId, gBattleBufferA[battlerId][1] == B_OUTCOME_DREW ? gBattleBufferA[battlerId][1] : gBattleBufferA[battlerId][1] ^ B_OUTCOME_DREW, SetBattleEndCallbacks);
}

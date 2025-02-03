#include "global.h"
#include "gflib.h"
#include "task.h"
#include "party_menu.h"
#include "pokeball.h"
#include "data.h"
#include "util.h"
#include "m4a.h"
#include "link.h"
#include "event_data.h"
#include "item_menu.h"
#include "strings.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "battle_message.h"
#include "reshow_battle_screen.h"
#include "teachy_tv.h"
#include "constants/songs.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/battle_string_ids.h"

struct PokedudeTextScriptHeader
{
    u8 btlcmd;
    u8 side;
    u16 stringid;
    void (*callback)(u8);
};

struct PokedudeBattlePartyInfo
{
    u8 side;
    u8 level;
    u16 species;
    u16 moves[MAX_MON_MOVES];
    u8 nature;
    u8 gender;
};

struct PokedudeInputScript
{
    // 0-3 for selection, 4 to repeat action, 255 to repeat move
    u8 cursorPos[MAX_BATTLERS_COUNT];
    u8 delay[MAX_BATTLERS_COUNT];
};

#define pdHealthboxPal1  simulatedInputState[0]
#define pdHealthboxPal2  simulatedInputState[1]
#define pdScriptNum      simulatedInputState[2]
#define pdMessageNo      simulatedInputState[3]

static void PokedudeBufferRunCommand(u8 battlerId);
static void PokedudeBufferExecCompleted(u8 battlerId);
static void PokedudeHandleLoadMonSprite(u8 battlerId);
static void PokedudeHandleSwitchInAnim(u8 battlerId);
static void PokedudeHandleDrawTrainerPic(u8 battlerId);
static void PokedudeHandleTrainerSlide(u8 battlerId);
static void PokedudeHandleChooseAction(u8 battlerId);
static void PokedudeHandleChooseMove(u8 battlerId);
static void Pokedude_SimulateInputChooseMove(u8 battlerId);
static void PokedudeHandleChooseItem(u8 battlerId);
static void PokedudeHandleChoosePokemon(u8 battlerId);
static void PokedudeHandleHealthbarUpdate(u8 battlerId);
static void PokedudeHandleIntroTrainerBallThrow(u8 battlerId);
static bool8 HandlePokedudeVoiceoverEtc(u8 battlerId);
static void PokedudeAction_PrintVoiceoverMessage(u8 battlerId);
static void PokedudeAction_PrintMessageWithHealthboxPals(u8 battlerId);

struct PokedudeBattlerState *gPokedudeBattlerStates[MAX_BATTLERS_COUNT];

static void (*const sPokedudeBufferCommands[CONTROLLER_CMDS_COUNT])(u8) =
{
	[CONTROLLER_GETMONDATA]               = BtlController_HandleGetMonData,
	[CONTROLLER_SETMONDATA]               = BtlController_HandleSetMonData,
	[CONTROLLER_SETRAWMONDATA]            = BtlController_HandleSetRawMonData,
	[CONTROLLER_LOADMONSPRITE]            = PokedudeHandleLoadMonSprite,
	[CONTROLLER_SWITCHINANIM]             = PokedudeHandleSwitchInAnim,
	[CONTROLLER_RETURNMONTOBALL]          = BtlController_HandleReturnMonToBall,
	[CONTROLLER_DRAWTRAINERPIC]           = PokedudeHandleDrawTrainerPic,
	[CONTROLLER_TRAINERSLIDE]             = PokedudeHandleTrainerSlide,
	[CONTROLLER_TRAINERSLIDEBACK]         = BattleControllerComplete,
	[CONTROLLER_FAINTANIMATION]           = BtlController_HandleFaintAnimation,
	[CONTROLLER_BALLTHROWANIM]            = PlayerHandleBallThrowAnim,
	[CONTROLLER_MOVEANIMATION]            = BtlController_HandleMoveAnimation,
	[CONTROLLER_PRINTSTRING]              = PlayerHandlePrintString,
	[CONTROLLER_PRINTSELECTIONSTRING]     = PlayerHandlePrintSelectionString,
	[CONTROLLER_CHOOSEACTION]             = PokedudeHandleChooseAction,
	[CONTROLLER_CHOOSEMOVE]               = PokedudeHandleChooseMove,
	[CONTROLLER_OPENBAG]                  = PokedudeHandleChooseItem,
	[CONTROLLER_CHOOSEPOKEMON]            = PokedudeHandleChoosePokemon,
	[CONTROLLER_HEALTHBARUPDATE]          = PokedudeHandleHealthbarUpdate,
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
	[CONTROLLER_INTROTRAINERBALLTHROW]    = PokedudeHandleIntroTrainerBallThrow,
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

void SetControllerToPokedude(u8 battlerId)
{
	gBattlerControllerFuncs[battlerId] = PokedudeBufferRunCommand;
	gBattlerControllerEndFuncs[battlerId] = PokedudeBufferExecCompleted;
    gBattleStruct->pdScriptNum = gSpecialVar_0x8004;
    gBattleStruct->pdMessageNo = 0;
}

static void PokedudeBufferRunCommand(u8 battlerId)
{
	if (gBattleControllerExecFlags & gBitTable[battlerId])
    {
        if (gBattleBufferA[battlerId][0] < ARRAY_COUNT(sPokedudeBufferCommands))
        {
            if (!HandlePokedudeVoiceoverEtc(battlerId))
                sPokedudeBufferCommands[gBattleBufferA[battlerId][0]](battlerId);
        }
        else
            PokedudeBufferExecCompleted(battlerId);
    }
}

static void PokedudeBufferExecCompleted(u8 battlerId)
{
	gBattlerControllerFuncs[battlerId] = PokedudeBufferRunCommand;
	
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

static void PokedudeHandleLoadMonSprite(u8 battlerId)
{
	BtlController_HandleLoadMonSprite(battlerId, TRUE, CompleteOnBattlerSpritePosX_0);
	BattleControllerComplete(battlerId);
}

static void SwitchIn_HandleSoundAndEnd(u8 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].specialAnimActive)
    {
        CreateTask(Task_BltController_RestoreBgmAfterCry, 10);
        HandleLowHpMusicChange(&gPlayerParty[gBattlerPartyIndexes[battlerId]], battlerId);
        BattleControllerComplete(battlerId);
    }
}

static void SwitchIn_CleanShinyAnimShowSubstitute(u8 battlerId)
{
    if (gSprites[gHealthboxSpriteIds[battlerId]].callback == SpriteCallbackDummy && gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim)
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim = 0;
        gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim = 0;
        FreeSpriteTilesByTag(ANIM_TAG_GOLD_STARS);
        FreeSpritePaletteByTag(ANIM_TAG_GOLD_STARS);
		
        if (gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute)
            InitAndLaunchSpecialAnimation(battlerId, battlerId, B_ANIM_MON_TO_SUBSTITUTE);
		
        gBattlerControllerFuncs[battlerId] = SwitchIn_HandleSoundAndEnd;
    }
}

static void SwitchIn_TryShinyAnimShowHealthbox(u8 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim && !gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive)
        TryShinyAnimation(battlerId);
		
    if (gSprites[gBattleControllerData[battlerId]].callback == SpriteCallbackDummy && !gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive)
    {
        DestroySprite(&gSprites[gBattleControllerData[battlerId]]);
        UpdateHealthboxAttribute(battlerId, HEALTHBOX_ALL);
        StartHealthboxSlideIn(battlerId);
        SetHealthboxSpriteVisible(gHealthboxSpriteIds[battlerId]);
        CopyBattleSpriteInvisibility(battlerId);
        gBattlerControllerFuncs[battlerId] = SwitchIn_CleanShinyAnimShowSubstitute;
    }
}

static void PokedudeHandleSwitchInAnim(u8 battlerId)
{
	BtlController_HandleSwitchInAnim(battlerId, TRUE, SwitchIn_TryShinyAnimShowHealthbox);
	gActionSelectionCursor[battlerId] = 0;
    gBattleStruct->battlers[battlerId].moveSelectionCursor = 0;
}

static void PokedudeHandleDrawTrainerPic(u8 battlerId)
{
	u8 subpriority;
	u32 trainerPicId;
	s16 xPos, yPos;
	bool8 isFront;
	
	if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
    {
		trainerPicId = TRAINER_BACK_PIC_POKEDUDE;
		isFront = FALSE;
		xPos = 80;
		yPos = (8 - gTrainerBackPicTable[trainerPicId].coords.size) * 4 + 80;
		subpriority = 30;
	}
	else
	{
		trainerPicId = TRAINER_PIC_PROFESSOR_OAK;
		isFront = TRUE;
		xPos = 176;
		yPos = (8 - gTrainerFrontPicTable[trainerPicId].coords.size) * 4 + 40;
		subpriority = GetBattlerSpriteSubpriority(battlerId);
	}
	BtlController_HandleDrawTrainerPic(battlerId, trainerPicId, isFront, xPos, yPos, subpriority);
}

static void PokedudeHandleTrainerSlide(u8 battlerId)
{
	BtlController_HandleTrainerSlide(battlerId, TRAINER_BACK_PIC_POKEDUDE, FALSE, 80, (8 - gTrainerBackPicTable[TRAINER_BACK_PIC_POKEDUDE].coords.size) * 4 + 80);
}

static void PokedudeHandleChooseAction(u8 battlerId)
{
	if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
        PlayerHandleChooseAction(battlerId);
    else
        gBattlerControllerFuncs[battlerId] = Pokedude_SimulateInputChooseAction;
}

static void PokedudeHandleChooseMoveAfterDma3(u8 battlerId)
{
    if (!IsDma3ManagerBusyWithBgCopy())
    {
        gBattle_BG0_X = 0;
        gBattle_BG0_Y = 320;
        gBattlerControllerFuncs[battlerId] = Pokedude_SimulateInputChooseMove;
    }
}

static void PokedudeHandleChooseMove(u8 battlerId)
{
	if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
    {
        InitMoveSelectionsVarsAndStrings(battlerId);
        gBattlerControllerFuncs[battlerId] = PokedudeHandleChooseMoveAfterDma3;
    }
    else
        gBattlerControllerFuncs[battlerId] = Pokedude_SimulateInputChooseMove;
}

static void CompleteWhenChooseItem(u8 battlerId)
{
	if (gMain.callback2 == BattleMainCB2 && !gPaletteFade.active)
    {
        BtlController_EmitOneReturnValue(battlerId, BUFFER_B, gSpecialVar_ItemId);
        BattleControllerComplete(battlerId);
    }
}

static void OpenBagAndChooseItem(u8 battlerId)
{
    u8 callbackId;

    if (!gPaletteFade.active)
    {
        gBattlerControllerFuncs[battlerId] = CompleteWhenChooseItem;
        FreeAllWindowBuffers();
		
        switch (gSpecialVar_0x8004)
        {
        case TTVSCR_STATUS:
        default:
            callbackId = ITEMMENULOCATION_TTVSCR_STATUS;
            break;
        case TTVSCR_CATCHING:
            callbackId = ITEMMENULOCATION_TTVSCR_CATCHING;
            break;
        }
        InitPokedudeBag(callbackId);
    }
}

static void PokedudeHandleChooseItem(u8 battlerId)
{
	u8 i;
	
	gBattleStruct->battlers[battlerId].itemPartyIndex = 0;
	BtlController_HandleChooseItem(battlerId, OpenBagAndChooseItem);
	
	for (i = 0; i < 3; ++i)
        gBattlePartyCurrentOrder[i] = gBattleBufferA[battlerId][i + 1];
}

static void WaitForMonSelection(u8 battlerId)
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

static void OpenPartyMenuToChooseMon(u8 battlerId)
{
    if (!gPaletteFade.active)
    {
        gBattlerControllerFuncs[battlerId] = WaitForMonSelection;
        DestroyTask(gBattleControllerData[battlerId]);
        FreeAllWindowBuffers();
        Pokedude_OpenPartyMenuInBattle();
    }
}

static void PokedudeHandleChoosePokemon(u8 battlerId)
{
	BtlController_HandleChoosePokemon(battlerId, OpenPartyMenuToChooseMon);
}

static void PokedudeHandleHealthbarUpdate(u8 battlerId)
{
	BtlController_HandleHealthbarUpdate(battlerId, (GetBattlerSide(battlerId) == B_SIDE_PLAYER));
}

static void Intro_DelayAndEnd(u8 battlerId)
{
    if (--gBattleSpritesDataPtr->healthBoxesData[battlerId].introEndDelay == 255)
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].introEndDelay = 0;
        BattleControllerComplete(battlerId);
    }
}

static void Intro_WaitForShinyAnimAndHealthbox(u8 battlerId)
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
		
        if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && !(gBattleTypeFlags & BATTLE_TYPE_MULTI))
			ShowHealthBox(BATTLE_PARTNER(battlerId));
		
        ShowHealthBox(battlerId);
        gBattleSpritesDataPtr->animationData->healthboxSlideInStarted = FALSE;
        gBattlerControllerFuncs[battlerId] = Intro_WaitForShinyAnimAndHealthbox;
    }
}

static void PokedudeHandleIntroTrainerBallThrow(u8 battlerId)
{
	BtlController_HandleIntroTrainerBallThrow(battlerId, 0xD6F8, TRAINER_BACK_PIC_POKEDUDE, StartAnimLinearTranslation, 31, Intro_TryShinyAnimShowHealthbox);
}

/////////////////////
// SIMULATED INPUT //
/////////////////////

// Scripts for choose action
static const struct PokedudeInputScript sInputScripts_ChooseAction_Battle[] =
{
    {
        .cursorPos = {0, 0},
        .delay     = {64, 0}
    },
    {
        .cursorPos = {4, 4},
        .delay     = {0, 0}
    },
};

static const struct PokedudeInputScript sInputScripts_ChooseAction_Status[] =
{
    {
        .cursorPos = {0, 0},
        .delay     = {64, 0}
    },
    {
        .cursorPos = {1, 0},
        .delay     = {64, 0}
    },
    {
        .cursorPos = {0, 0},
        .delay     = {64, 0}
    },
};

static const struct PokedudeInputScript sInputScripts_ChooseAction_Matchups[] =
{
    {
        .cursorPos = {0, 0},
        .delay     = {64, 0}
    },
    {
        .cursorPos = {2, 0},
        .delay     = {64, 0}
    },
    {
        .cursorPos = {0, 0},
        .delay     = {64, 0}
    },
	{
        .cursorPos = {0, 0},
        .delay     = {64, 0}
    },
};

static const struct PokedudeInputScript sInputScripts_ChooseAction_Catching[] =
{
    {
        .cursorPos = {0, 0},
        .delay     = {64, 0}
    },
    {
        .cursorPos = {0, 0},
        .delay     = {64, 0}
    },
    {
        .cursorPos = {1, 0},
        .delay     = {64, 0}
    },
};

// Scripts for choose move
static const struct PokedudeInputScript sInputScripts_ChooseMove_Battle[] =
{
    {
        .cursorPos = {  2,   2},
        .delay     = { 64,   0}
    },
    {
        .cursorPos = {255, 255},
        .delay     = {  0,   0}
    },
};

static const struct PokedudeInputScript sInputScripts_ChooseMove_Status[] =
{
    {
        .cursorPos = {  2,   2},
        .delay     = { 64,   0}
    },
    {
        .cursorPos = {  2,   0},
        .delay     = { 64,   0}
    },
    {
        .cursorPos = {  2,   0},
        .delay     = { 64,   0}
    },
    {
        .cursorPos = {255, 255},
        .delay     = {  0,   0}
    },
};

static const struct PokedudeInputScript sInputScripts_ChooseMove_Matchups[] =
{
    {
        .cursorPos = {  2,   0},
        .delay     = { 64,   0}
    },
    {
        .cursorPos = {  0,   0},
        .delay     = { 64,   0}
    },
    {
        .cursorPos = {  0,   0},
        .delay     = { 64,   0}
    },
	{
		.cursorPos = {  0,   0},
        .delay     = { 64,   0}
	},
    {
        .cursorPos = {255, 255},
        .delay     = {  0,   0}
    },
};

static const struct PokedudeInputScript sInputScripts_ChooseMove_Catching[] =
{
    {
        .cursorPos = {  0,   2},
        .delay     = { 64,   0}
    },
    {
        .cursorPos = {  2,   2},
        .delay     = { 64,   0}
    },
    {
        .cursorPos = {255, 255},
        .delay     = {  0,   0}
    },
};

// Scripts for texts
static const struct PokedudeTextScriptHeader sPokedudeTextScripts_Battle[] =
{
    {
        .btlcmd = CONTROLLER_CHOOSEACTION,
        .side = B_SIDE_PLAYER,
        .callback = PokedudeAction_PrintVoiceoverMessage,
    },
    {
        .btlcmd = CONTROLLER_PRINTSTRING,
        .side = B_SIDE_OPPONENT,
        .stringid = STRINGID_USEDMOVE,
        .callback = PokedudeAction_PrintVoiceoverMessage,
    },
    {
        .btlcmd = CONTROLLER_CHOOSEACTION,
        .side = B_SIDE_PLAYER,
        .callback = PokedudeAction_PrintVoiceoverMessage,
    },
    {
        .btlcmd = CONTROLLER_PRINTSTRING,
        .side = B_SIDE_PLAYER,
        .stringid = STRINGID_PKMNGAINEDEXP,
        .callback = PokedudeAction_PrintVoiceoverMessage,
    },
};

static const struct PokedudeTextScriptHeader sPokedudeTextScripts_Status[] =
{
    {
        .btlcmd = CONTROLLER_CHOOSEACTION,
        .side = B_SIDE_PLAYER,
        .callback = NULL,
    },
    {
        .btlcmd = CONTROLLER_CHOOSEACTION,
        .side = B_SIDE_PLAYER,
        .callback = PokedudeAction_PrintMessageWithHealthboxPals,
    },
    {
        .btlcmd = CONTROLLER_OPENBAG,
        .side = B_SIDE_PLAYER,
        .callback = PokedudeAction_PrintVoiceoverMessage,
    },
    {
        .btlcmd = CONTROLLER_PRINTSTRING,
        .side = B_SIDE_OPPONENT,
        .stringid = STRINGID_USEDMOVE,
        .callback = PokedudeAction_PrintVoiceoverMessage,
    },
    {
        .btlcmd = CONTROLLER_PRINTSTRING,
        .side = B_SIDE_PLAYER,
        .stringid = STRINGID_PKMNGAINEDEXP,
        .callback = PokedudeAction_PrintVoiceoverMessage,
    },
};

static const struct PokedudeTextScriptHeader sPokedudeTextScripts_Matchups[] =
{
    {
        .btlcmd = CONTROLLER_PRINTSTRING,
        .side = B_SIDE_OPPONENT,
        .stringid = STRINGID_USEDMOVE,
        .callback = PokedudeAction_PrintVoiceoverMessage,
    },
    {
        .btlcmd = CONTROLLER_CHOOSEACTION,
        .side = B_SIDE_PLAYER,
        .callback = PokedudeAction_PrintVoiceoverMessage,
    },
    {
        .btlcmd = CONTROLLER_CHOOSEPOKEMON,
        .side = B_SIDE_PLAYER,
        .callback = PokedudeAction_PrintVoiceoverMessage,
    },
    {
        .btlcmd = CONTROLLER_PRINTSTRING,
        .side = B_SIDE_OPPONENT,
        .stringid = STRINGID_USEDMOVE,
        .callback = PokedudeAction_PrintVoiceoverMessage,
    },
    {
        .btlcmd = CONTROLLER_CHOOSEACTION,
        .side = B_SIDE_PLAYER,
        .callback = PokedudeAction_PrintVoiceoverMessage,
    },
    {
        .btlcmd = CONTROLLER_CHOOSEMOVE,
        .side = B_SIDE_PLAYER,
        .callback = PokedudeAction_PrintVoiceoverMessage,
    },
    {
        .btlcmd = CONTROLLER_PRINTSTRING,
        .side = B_SIDE_PLAYER,
        .stringid = STRINGID_PKMNGAINEDEXP,
        .callback = PokedudeAction_PrintVoiceoverMessage,
    },
};

static const struct PokedudeTextScriptHeader sPokedudeTextScripts_Catching[] =
{
    {
        .btlcmd = CONTROLLER_CHOOSEACTION,
        .side = B_SIDE_PLAYER,
        .callback = PokedudeAction_PrintVoiceoverMessage,
    },
    {
        .btlcmd = CONTROLLER_CHOOSEACTION,
        .side = B_SIDE_PLAYER,
        .callback = NULL,
    },
    {
        .btlcmd = CONTROLLER_CHOOSEACTION,
        .side = B_SIDE_PLAYER,
        .callback = PokedudeAction_PrintVoiceoverMessage,
    },
    {
        .btlcmd = CONTROLLER_PRINTSTRING,
        .side = B_SIDE_OPPONENT,
        .stringid = STRINGID_ATKFASTASLEEP,
        .callback = PokedudeAction_PrintVoiceoverMessage,
    },
    {
        .btlcmd = CONTROLLER_OPENBAG,
        .side = B_SIDE_PLAYER,
        .callback = PokedudeAction_PrintVoiceoverMessage,
    },
    {
        .btlcmd = CONTROLLER_ENDLINKBATTLE,
        .side = B_SIDE_PLAYER,
        .callback = PokedudeAction_PrintVoiceoverMessage,
    },
};

// Parties
static const struct PokedudeBattlePartyInfo sParties_Battle[] =
{
    {
        .side = B_SIDE_PLAYER,
        .level = 15,
        .species = SPECIES_RATTATA,
        .moves = { MOVE_TACKLE, MOVE_TAIL_WHIP, MOVE_HYPER_FANG, MOVE_QUICK_ATTACK },
        .nature = NATURE_LONELY,
        .gender = MALE,
    },
    {
        .side = B_SIDE_OPPONENT,
        .level = 15,
        .species = SPECIES_PIDGEY,
        .moves = { MOVE_TACKLE, MOVE_SAND_ATTACK, MOVE_GUST, MOVE_QUICK_ATTACK },
        .nature = NATURE_NAUGHTY,
        .gender = MALE,
    },
    {0xFF}
};

static const struct PokedudeBattlePartyInfo sParties_Status[] =
{
    {
        .side = B_SIDE_PLAYER,
        .level = 15,
        .species = SPECIES_RATTATA,
        .moves = { MOVE_TACKLE, MOVE_TAIL_WHIP, MOVE_HYPER_FANG, MOVE_QUICK_ATTACK },
        .nature = NATURE_LONELY,
        .gender = MALE,
    },
    {
        .side = B_SIDE_OPPONENT,
        .level = 14,
        .species = SPECIES_ODDISH,
        .moves = { MOVE_ABSORB, MOVE_SWEET_SCENT, MOVE_POISON_POWDER },
        .nature = NATURE_RASH,
        .gender = MALE,
    },
    {0xFF}
};

static const struct PokedudeBattlePartyInfo sParties_Matchups[] =
{
    {
        .side = B_SIDE_PLAYER,
        .level = 15,
        .species = SPECIES_POLIWAG,
        .moves = { MOVE_WATER_GUN, MOVE_HYPNOSIS, MOVE_BUBBLE },
        .nature = NATURE_RASH,
        .gender = MALE,
    },
    {
        .side = B_SIDE_PLAYER,
        .level = 15,
        .species = SPECIES_BUTTERFREE,
        .moves = { MOVE_CONFUSION, MOVE_POISON_POWDER, MOVE_STUN_SPORE, MOVE_SLEEP_POWDER },
        .nature = NATURE_RASH,
        .gender = MALE,
    },
    {
        .side = B_SIDE_OPPONENT,
        .level = 14,
        .species = SPECIES_ODDISH,
        .moves = { MOVE_ABSORB, MOVE_SWEET_SCENT, MOVE_POISON_POWDER },
        .nature = NATURE_RASH,
        .gender = MALE,
    },
    {0xFF}
};

static const struct PokedudeBattlePartyInfo sParties_Catching[] =
{
    {
        .side = B_SIDE_PLAYER,
        .level = 15,
        .species = SPECIES_BUTTERFREE,
        .moves = { MOVE_CONFUSION, MOVE_POISON_POWDER, MOVE_SLEEP_POWDER, MOVE_STUN_SPORE },
        .nature = NATURE_RASH,
        .gender = MALE,
    },
    {
        .side = B_SIDE_OPPONENT,
        .level = 11,
        .species = SPECIES_JIGGLYPUFF,
        .moves = { MOVE_SING, MOVE_DEFENSE_CURL, MOVE_POUND },
        .nature = NATURE_CAREFUL,
        .gender = MALE,
    },
    {0xFF}
};

// Texts
static const u8 *const sPokedudeTexts_Battle[] =
{
    Pokedude_Text_SpeedierBattlerGoesFirst,
    Pokedude_Text_MyRattataFasterThanPidgey,
    Pokedude_Text_BattlersTakeTurnsAttacking,
    Pokedude_Text_MyRattataWonGetsEXP,
};

static const u8 *const sPokedudeTexts_Status[] =
{
    Pokedude_Text_UhOhRattataPoisoned,
    Pokedude_Text_UhOhRattataPoisoned,
    Pokedude_Text_HealStatusRightAway,
    Pokedude_Text_UsingItemTakesTurn,
    Pokedude_Text_YayWeManagedToWin,
};

static const u8 *const sPokedudeTexts_TypeMatchup[] =
{
    Pokedude_Text_WaterNotVeryEffectiveAgainstGrass,
    Pokedude_Text_GrassEffectiveAgainstWater,
    Pokedude_Text_LetsTryShiftingMons,
    Pokedude_Text_ShiftingUsesTurn,
    Pokedude_Text_ButterfreeDoubleResistsGrass,
    Pokedude_Text_ButterfreeGoodAgainstOddish,
    Pokedude_Text_YeahWeWon,
};

static const u8 *const sPokedudeTexts_Catching[] =
{
    Pokedude_Text_WeakenMonBeforeCatching,
    Pokedude_Text_WeakenMonBeforeCatching,
    Pokedude_Text_BestIfTargetStatused,
    Pokedude_Text_CantDoubleUpOnStatus,
    Pokedude_Text_LetMeThrowBall,
    Pokedude_Text_PickBestKindOfBall,
};

static const struct PokedudeInputScript *const sInputScripts_ChooseAction[] =
{
    [TTVSCR_BATTLE]   = sInputScripts_ChooseAction_Battle,
    [TTVSCR_STATUS]   = sInputScripts_ChooseAction_Status,
    [TTVSCR_MATCHUPS] = sInputScripts_ChooseAction_Matchups,
    [TTVSCR_CATCHING] = sInputScripts_ChooseAction_Catching,
};

static const struct PokedudeInputScript *const sInputScripts_ChooseMove[] =
{
    [TTVSCR_BATTLE]   = sInputScripts_ChooseMove_Battle,
    [TTVSCR_STATUS]   = sInputScripts_ChooseMove_Status,
    [TTVSCR_MATCHUPS] = sInputScripts_ChooseMove_Matchups,
    [TTVSCR_CATCHING] = sInputScripts_ChooseMove_Catching,
};

static const struct PokedudeTextScriptHeader *const sPokedudeTextScripts[] =
{
    [TTVSCR_BATTLE]   = sPokedudeTextScripts_Battle,
    [TTVSCR_STATUS]   = sPokedudeTextScripts_Status,
    [TTVSCR_MATCHUPS] = sPokedudeTextScripts_Matchups,
    [TTVSCR_CATCHING] = sPokedudeTextScripts_Catching,
};

static const struct PokedudeBattlePartyInfo *const sPokedudeBattlePartyPointers[] =
{
    [TTVSCR_BATTLE]   = sParties_Battle,
    [TTVSCR_STATUS]   = sParties_Status,
    [TTVSCR_MATCHUPS] = sParties_Matchups,
    [TTVSCR_CATCHING] = sParties_Catching,
};

void Pokedude_SimulateInputChooseAction(u8 battlerId)
{
	const struct PokedudeInputScript *script_p = sInputScripts_ChooseAction[gBattleStruct->pdScriptNum];

    if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
    {
        DoBounceEffect(battlerId, BOUNCE_HEALTHBOX, 7, 1);
        DoBounceEffect(battlerId, BOUNCE_MON, 7, 1);
    }
	
    if (script_p[gPokedudeBattlerStates[battlerId]->action_idx].delay[battlerId] == gPokedudeBattlerStates[battlerId]->timer)
    {
        if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
            PlaySE(SE_SELECT);
		
        gPokedudeBattlerStates[battlerId]->timer = 0;
		
        switch (script_p[gPokedudeBattlerStates[battlerId]->action_idx].cursorPos[battlerId])
        {
        case 0:
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_USE_MOVE, 0);
            break;
        case 1:
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_USE_ITEM, 0);
            break;
        case 2:
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_SWITCH, 0);
            break;
        case 3:
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_RUN, 0);
            break;
        }
        BattleControllerComplete(battlerId);

        if (script_p[++gPokedudeBattlerStates[battlerId]->action_idx].cursorPos[battlerId] == 4)
            gPokedudeBattlerStates[battlerId]->action_idx = 0;
    }
    else
    {
        if (gActionSelectionCursor[battlerId] != script_p[gPokedudeBattlerStates[battlerId]->action_idx].cursorPos[battlerId]
            && script_p[gPokedudeBattlerStates[battlerId]->action_idx].delay[battlerId] / 2 == gPokedudeBattlerStates[battlerId]->timer)
        {
            PlaySE(SE_SELECT);
            ActionSelectionDestroyCursorAt(gActionSelectionCursor[battlerId]);
            gActionSelectionCursor[battlerId] = script_p[gPokedudeBattlerStates[battlerId]->action_idx].cursorPos[battlerId];
            ActionSelectionCreateCursorAt(gActionSelectionCursor[battlerId]);
        }
        ++gPokedudeBattlerStates[battlerId]->timer;
    }
}

static void Pokedude_SimulateInputChooseMove(u8 battlerId)
{
	const struct PokedudeInputScript *script_p = sInputScripts_ChooseMove[gBattleStruct->pdScriptNum];

    if (script_p[gPokedudeBattlerStates[battlerId]->move_idx].delay[battlerId] == gPokedudeBattlerStates[battlerId]->timer)
    {
        if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
            PlaySE(SE_SELECT);
		
        gPokedudeBattlerStates[battlerId]->timer = 0;
        BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, script_p[gPokedudeBattlerStates[battlerId]->move_idx].cursorPos[battlerId] | ((BATTLE_OPPOSITE(battlerId)) << 8));
        BattleControllerComplete(battlerId);

        if (script_p[++gPokedudeBattlerStates[battlerId]->move_idx].cursorPos[battlerId] == 255)
            gPokedudeBattlerStates[battlerId]->move_idx = 0;
    }
    else
    {
        if (script_p[gPokedudeBattlerStates[battlerId]->move_idx].cursorPos[battlerId] != gBattleStruct->battlers[battlerId].moveSelectionCursor
            && script_p[gPokedudeBattlerStates[battlerId]->move_idx].delay[battlerId] / 2 == gPokedudeBattlerStates[battlerId]->timer)
        {
            PlaySE(SE_SELECT);
            MoveSelectionDestroyCursorAt(gBattleStruct->battlers[battlerId].moveSelectionCursor);
            gBattleStruct->battlers[battlerId].moveSelectionCursor = script_p[gPokedudeBattlerStates[battlerId]->move_idx].cursorPos[battlerId];
            MoveSelectionCreateCursorAt(gBattleStruct->battlers[battlerId].moveSelectionCursor, 0);
        }
        ++gPokedudeBattlerStates[battlerId]->timer;
    }
}

////////////////
// VOICEOVERS //
////////////////

static bool8 HandlePokedudeVoiceoverEtc(u8 battlerId)
{
	const struct PokedudeTextScriptHeader *header_p = sPokedudeTextScripts[gBattleStruct->pdScriptNum];
    const u16 * bstringid_p = (const u16 *)&gBattleBufferA[battlerId][2];

    if (gBattleBufferA[battlerId][0] != header_p[gBattleStruct->pdMessageNo].btlcmd)
        return FALSE;
    else if (battlerId != header_p[gBattleStruct->pdMessageNo].side)
        return FALSE;
    else if (gBattleBufferA[battlerId][0] == CONTROLLER_PRINTSTRING && header_p[gBattleStruct->pdMessageNo].stringid != *bstringid_p)
        return FALSE;
    else if (header_p[gBattleStruct->pdMessageNo].callback == NULL)
    {
        gBattleStruct->pdMessageNo++;
        return FALSE;
    }
	else
	{
		gBattlerControllerFuncs[battlerId] = header_p[gBattleStruct->pdMessageNo].callback;
		gPokedudeBattlerStates[battlerId]->timer = 0;
		gPokedudeBattlerStates[battlerId]->msg_idx = header_p[gBattleStruct->pdMessageNo].stringid;
		gBattleStruct->pdMessageNo++;
		return TRUE;
	}
}

static const u8 *GetPokedudeText(void)
{
    switch (gBattleStruct->pdScriptNum)
    {
    default:
        return sPokedudeTexts_Battle[gBattleStruct->pdMessageNo - 1];
    case TTVSCR_STATUS:
        return sPokedudeTexts_Status[gBattleStruct->pdMessageNo - 1];
    case TTVSCR_MATCHUPS:
        return sPokedudeTexts_TypeMatchup[gBattleStruct->pdMessageNo - 1];
    case TTVSCR_CATCHING:
        return sPokedudeTexts_Catching[gBattleStruct->pdMessageNo - 1];
    }
}

static void ReturnFromPokedudeAction(u8 battlerId)
{
    gPokedudeBattlerStates[battlerId]->timer = 0;
    gBattlerControllerFuncs[battlerId] = PokedudeBufferRunCommand;
}

static void PokedudeAction_PrintVoiceoverMessage(u8 battlerId)
{
	switch (gPokedudeBattlerStates[battlerId]->timer)
    {
    case 0:
        if (!gPaletteFade.active)
        {
            BeginNormalPaletteFade(0xFFFFFF7F, 4, 0, 8, RGB_BLACK);
            ++gPokedudeBattlerStates[battlerId]->timer;
        }
        break;
    case 1:
        if (!gPaletteFade.active)
        {
            gPokedudeBattlerStates[battlerId]->saved_bg0y = gBattle_BG0_Y;
            BtlCtrl_DrawVoiceoverMessageFrame();
            ++gPokedudeBattlerStates[battlerId]->timer;
        }
        break;
    case 2:
        gBattle_BG0_Y = 0;
        BattleStringExpandPlaceholdersToDisplayedString(GetPokedudeText());
        BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_OAK_OLD_MAN);
        ++gPokedudeBattlerStates[battlerId]->timer;
        break;
    case 3:
        if (!IsTextPrinterActive(24) && JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            BeginNormalPaletteFade(0xFFFFFF7F, 4, 8, 0, RGB_BLACK);
            ++gPokedudeBattlerStates[battlerId]->timer;
        }
        break;
    case 4:
        if (!gPaletteFade.active)
        {
            if (gPokedudeBattlerStates[battlerId]->msg_idx == STRINGID_PKMNGAINEDEXP)
            {
                BattleStopLowHpSound();
                PlayBGM(MUS_VICTORY_WILD);
            }
            gBattle_BG0_Y = gPokedudeBattlerStates[battlerId]->saved_bg0y;
            BtlCtrl_RemoveVoiceoverMessageFrame();
            ReturnFromPokedudeAction(battlerId);
        }
        break;
    }
}

static void PokedudeAction_PrintMessageWithHealthboxPals(u8 battlerId)
{
	switch (gPokedudeBattlerStates[battlerId]->timer)
    {
    case 0:
        if (!gPaletteFade.active)
        {
            DoLoadHealthboxPalsForLevelUp(&gBattleStruct->pdHealthboxPal2, &gBattleStruct->pdHealthboxPal1, GetBattlerAtPosition(B_POSITION_PLAYER_LEFT));
            BeginNormalPaletteFade(0xFFFFFF7F, 4, 0, 8, RGB_BLACK);
            ++gPokedudeBattlerStates[battlerId]->timer;
        }
        break;
    case 1:
        if (!gPaletteFade.active)
        {
            BeginNormalPaletteFade((gBitTable[gBattleStruct->pdHealthboxPal2] | gBitTable[gBattleStruct->pdHealthboxPal1]) << 16, 4, 8, 0, RGB_BLACK);
            ++gPokedudeBattlerStates[battlerId]->timer;
        }
        break;
    case 2:
        if (!gPaletteFade.active)
        {
            BtlCtrl_DrawVoiceoverMessageFrame();
            ++gPokedudeBattlerStates[battlerId]->timer;
        }
        break;
    case 3:
        BattleStringExpandPlaceholdersToDisplayedString(GetPokedudeText());
        BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_OAK_OLD_MAN);
        ++gPokedudeBattlerStates[battlerId]->timer;
        break;
    case 4:
        if (!IsTextPrinterActive(24) && JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            BeginNormalPaletteFade((gBitTable[gBattleStruct->pdHealthboxPal2] | gBitTable[gBattleStruct->pdHealthboxPal1]) << 16, 4, 0, 8, RGB_BLACK);
            ++gPokedudeBattlerStates[battlerId]->timer;
        }
        break;
    case 5:
        if (!gPaletteFade.active)
        {
            BeginNormalPaletteFade(0xFFFFFF7F, 4, 8, 0, RGB_BLACK);
            ++gPokedudeBattlerStates[battlerId]->timer;
        }
        break;
    case 6:
        if (!gPaletteFade.active)
        {
            if (gPokedudeBattlerStates[battlerId]->msg_idx == STRINGID_PKMNGAINEDEXP)
            {
                BattleStopLowHpSound();
                PlayBGM(MUS_VICTORY_WILD);
            }
            DoFreeHealthboxPalsForLevelUp(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT));
            BtlCtrl_RemoveVoiceoverMessageFrame();
            ReturnFromPokedudeAction(battlerId);
        }
        break;
    }
}

void InitPokedudePartyAndOpponent(void)
{
    u8 j, i = 0, myIdx = 0, opIdx = 0;
    const struct PokedudeBattlePartyInfo *data = sPokedudeBattlePartyPointers[gSpecialVar_0x8004];
	struct Pokemon *mon;
	struct PokemonGenerator generator =
	{
		.otIdType = OT_ID_PLAYER_ID,
		.shinyType = GENERATE_SHINY_NORMAL,
		.shinyRollType = SHINY_ROLL_NORMAL,
		.hasFixedPersonality = FALSE,
		.fixedPersonality = 0,
		.formChanges = NULL,
		.nPerfectIvs = 0,
	};
    gBattleTypeFlags = BATTLE_TYPE_POKEDUDE;
	
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();

    do
    {
        if (data[i].side == B_SIDE_PLAYER)
            mon = &gPlayerParty[myIdx++];
        else
            mon = &gEnemyParty[opIdx++];
		
		generator.species = data[i].species;
		generator.level = data[i].level;
		generator.forcedGender = data[i].gender;
		generator.forcedNature = data[i].nature;
		memcpy(generator.moves, data[i].moves, sizeof(generator.moves));
		
        CreateMon(mon, generator);

    } while (data[++i].side != 0xFF);
}

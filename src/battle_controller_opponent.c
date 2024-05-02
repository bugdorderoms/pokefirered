#include "global.h"
#include "gflib.h"
#include "data.h"
#include "m4a.h"
#include "task.h"
#include "util.h"
#include "pokeball.h"
#include "random.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "battle_message.h"
#include "battle_interface.h"
#include "battle_gfx_sfx_util.h"
#include "battle_ai_script_commands.h"
#include "battle_ai_switch_items.h"
#include "constants/battle_anim.h"
#include "constants/moves.h"
#include "constants/songs.h"
#include "constants/battle_string_ids.h"

static void OpponentBufferRunCommand(u8 battlerId);
static void OpponentBufferExecCompleted(u8 battlerId);
static void OpponentHandleLoadMonSprite(u8 battlerId);
static void OpponentHandleSwitchInAnim(u8 battlerId);
static void OpponentHandleDrawTrainerPic(u8 battlerId);
static void OpponentHandleTrainerSlide(u8 battlerId);
static void OpponentHandlePrintString(u8 battlerId);
static void OpponentHandleChooseAction(u8 battlerId);
static void OpponentHandleChooseMove(u8 battlerId);
static void OpponentHandleChooseItem(u8 battlerId);
static void OpponentHandleChoosePokemon(u8 battlerId);
static void OpponentHandleIntroTrainerBallThrow(u8 battlerId);
static void OpponentHandleEndLinkBattle(u8 battlerId);

static void (*const sOpponentBufferCommands[CONTROLLER_CMDS_COUNT])(u8) =
{
    [CONTROLLER_GETMONDATA]               = BtlController_HandleGetMonData,
	[CONTROLLER_SETMONDATA]               = BtlController_HandleSetMonData,
	[CONTROLLER_SETRAWMONDATA]            = BtlController_HandleSetRawMonData,
	[CONTROLLER_LOADMONSPRITE]            = OpponentHandleLoadMonSprite,
	[CONTROLLER_SWITCHINANIM]             = OpponentHandleSwitchInAnim,
	[CONTROLLER_RETURNMONTOBALL]          = BtlController_HandleReturnMonToBall,
	[CONTROLLER_DRAWTRAINERPIC]           = OpponentHandleDrawTrainerPic,
	[CONTROLLER_TRAINERSLIDE]             = OpponentHandleTrainerSlide,
	[CONTROLLER_TRAINERSLIDEBACK]         = OpponentHandleTrainerSlideBack,
	[CONTROLLER_FAINTANIMATION]           = BtlController_HandleFaintAnimation,
	[CONTROLLER_BALLTHROWANIM]            = BattleControllerComplete,
	[CONTROLLER_MOVEANIMATION]            = BtlController_HandleMoveAnimation,
	[CONTROLLER_PRINTSTRING]              = OpponentHandlePrintString,
	[CONTROLLER_PRINTSELECTIONSTRING]     = BattleControllerComplete,
	[CONTROLLER_CHOOSEACTION]             = OpponentHandleChooseAction,
	[CONTROLLER_CHOOSEMOVE]               = OpponentHandleChooseMove,
	[CONTROLLER_OPENBAG]                  = OpponentHandleChooseItem,
	[CONTROLLER_CHOOSEPOKEMON]            = OpponentHandleChoosePokemon,
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
	[CONTROLLER_INTROTRAINERBALLTHROW]    = OpponentHandleIntroTrainerBallThrow,
	[CONTROLLER_DRAWPARTYSTATUSSUMMARY]   = BtlController_HandleDrawPartyStatusSummary,
	[CONTROLLER_HIDEPARTYSTATUSSUMMARY]   = BtlController_HandleHidePartyStatusSummary,
	[CONTROLLER_ENDBOUNCE]                = BattleControllerComplete,
	[CONTROLLER_SPRITEINVISIBILITY]       = BtlController_HandleSpriteInvisibility,
	[CONTROLLER_BATTLEANIMATION]          = BtlController_HandleBattleAnimation,
	[CONTROLLER_LINKSTANDBYMSG]           = BattleControllerComplete,
	[CONTROLLER_RESETACTIONMOVESELECTION] = BattleControllerComplete,
	[CONTROLLER_ENDLINKBATTLE]            = OpponentHandleEndLinkBattle,
	[CONTROLLER_TERMINATOR_NOP]           = ControllerDummy,
};

void SetControllerToOpponent(u8 battlerId)
{
	gBattlerControllerFuncs[battlerId] = OpponentBufferRunCommand;
	gBattlerControllerEndFuncs[battlerId] = OpponentBufferExecCompleted;
}

static void OpponentBufferRunCommand(u8 battlerId)
{
	if (gBattleControllerExecFlags & gBitTable[battlerId])
    {
        if (gBattleBufferA[battlerId][0] < ARRAY_COUNT(sOpponentBufferCommands))
            sOpponentBufferCommands[gBattleBufferA[battlerId][0]](battlerId);
        else
            OpponentBufferExecCompleted(battlerId);
    }
}

static void OpponentBufferExecCompleted(u8 battlerId)
{
	gBattlerControllerFuncs[battlerId] = OpponentBufferRunCommand;
    gBattleControllerExecFlags &= ~(gBitTable[battlerId]);
}

////////////////////////
// BATTLE CONTROLLERS //
////////////////////////

static void OpponentHandleLoadMonSprite(u8 battlerId)
{
    u16 species = GetMonData(&gEnemyParty[gBattlerPartyIndexes[battlerId]], MON_DATA_SPECIES);
    u8 y;

    if (gBattleTypeFlags & BATTLE_TYPE_GHOST)
    {
        DecompressGhostFrontPic(battlerId);
        y = GetGhostSpriteDefault_Y(battlerId);
        gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim = TRUE;
        gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim = TRUE;
    }
    else
    {
        BattleLoadMonSpriteGfx(battlerId);
        y = GetBattlerSpriteDefault_Y(battlerId);
    }
    SetMultiuseSpriteTemplateToPokemon(species, GetBattlerPosition(battlerId));
    gBattlerSpriteIds[battlerId] = CreateSprite(&gMultiuseSpriteTemplate, GetBattlerSpriteCoord(battlerId, 2), y, GetBattlerSpriteSubpriority(battlerId));
    gSprites[gBattlerSpriteIds[battlerId]].x2 = -240;
    gSprites[gBattlerSpriteIds[battlerId]].data[0] = battlerId;
    gSprites[gBattlerSpriteIds[battlerId]].data[2] = species;
    gSprites[gBattlerSpriteIds[battlerId]].oam.paletteNum = battlerId;
    StartSpriteAnim(&gSprites[gBattlerSpriteIds[battlerId]], 0);
	
    if (!(gBattleTypeFlags & BATTLE_TYPE_GHOST))
        SetBattlerShadowSpriteCallback(battlerId, species);
	
    gBattlerControllerFuncs[battlerId] = TryShinyAnimAfterMonAnim;
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
            InitAndLaunchSpecialAnimation(battlerId, battlerId, battlerId, B_ANIM_MON_TO_SUBSTITUTE);
		
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
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim && !gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive)
        TryShinyAnimation(battlerId);
	
    if (gSprites[gBattleControllerData[battlerId]].callback == SpriteCallbackDummy && !gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive)
    {
        DestroySprite(&gSprites[gBattleControllerData[battlerId]]);
        SetBattlerShadowSpriteCallback(battlerId, GetMonData(&gEnemyParty[gBattlerPartyIndexes[battlerId]], MON_DATA_SPECIES));
        gBattlerControllerFuncs[battlerId] = SwitchIn_ShowHealthbox;
    }
}

static void OpponentHandleSwitchInAnim(u8 battlerId)
{
	gBattleStruct->monToSwitchIntoId[battlerId] = PARTY_SIZE;
	BtlController_HandleSwitchInAnim(battlerId, FALSE, SwitchIn_TryShinyAnim);
}

static inline u32 GetOpponentTrainerPicId(void)
{
	return gTrainers[gTrainerBattleOpponent_A].trainerPic;
}

static void OpponentHandleDrawTrainerPic(u8 battlerId)
{
	u32 trainerPicId = GetOpponentTrainerPicId();
	BtlController_HandleDrawTrainerPic(battlerId, trainerPicId, TRUE, 176, (8 - gTrainerFrontPicCoords[trainerPicId].size) * 4 + 40, GetBattlerSpriteSubpriority(battlerId));
}

static void OpponentHandleTrainerSlide(u8 battlerId)
{
	u32 trainerPicId = GetOpponentTrainerPicId();
	BtlController_HandleTrainerSlide(battlerId, trainerPicId, TRUE, 176, (8 - gTrainerFrontPicCoords[trainerPicId].size) * 4 + 40);
}

void OpponentHandleTrainerSlideBack(u8 battlerId)
{
	BtlController_HandleTrainerSlideBack(battlerId, 35, FALSE);
}

static void OpponentHandlePrintString(u8 battlerId)
{
	u16 *stringId = (u16 *)(&gBattleBufferA[battlerId][2]);
	
	BtlController_HandlePrintString(battlerId, *stringId, FALSE);

    if (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE)
    {
        switch (*stringId)
        {
        case STRINGID_TRAINER1WINTEXT:
            gBattlerControllerFuncs[battlerId] = PrintOakText_HowDisappointing;
            break;
        case STRINGID_DONTLEAVEBIRCH:
            gBattlerControllerFuncs[battlerId] = PrintOakText_OakNoRunningFromATrainer;
            break;
        }
    }
}

static void OpponentHandleChooseAction(u8 battlerId)
{
	AI_TrySwitchOrUseItem(battlerId);
    BattleControllerComplete(battlerId);
}

static const u16 sNaturalEnemySpecies[][2] =
{
	// Attacker         Target
	{SPECIES_ZANGOOSE,  SPECIES_SEVIPER},
	{SPECIES_SEVIPER,   SPECIES_ZANGOOSE},
	{SPECIES_HEATMOR,   SPECIES_DURANT},
	{SPECIES_DURANT,    SPECIES_HEATMOR},
	{SPECIES_SABLEYE,   SPECIES_CARBINK},
	{SPECIES_MAREANIE,  SPECIES_CORSOLA},
};

static void OpponentHandleChooseMove(u8 battlerId)
{
	u8 chosenMoveId, target;
    struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct *)(&gBattleBufferA[battlerId][4]);

    if (gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_FIRST_BATTLE | BATTLE_TYPE_SAFARI | BATTLE_TYPE_ROAMER))
    {
        BattleAI_SetupAIData(battlerId);
        chosenMoveId = BattleAI_ChooseMoveOrAction();

        switch (chosenMoveId)
        {
        case AI_CHOICE_WATCH:
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_SAFARI_WATCH_CAREFULLY, 0);
            break;
        case AI_CHOICE_FLEE:
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_RUN, 0);
            break;
        default:
            if (gBattleMoves[moveInfo->moves[chosenMoveId]].target == MOVE_TARGET_USER
			|| gBattleMoves[moveInfo->moves[chosenMoveId]].target == MOVE_TARGET_ALL_BATTLERS || gBattleMoves[moveInfo->moves[chosenMoveId]].target == MOVE_TARGET_USER_OR_ALLY)
                gBattlerTarget = battlerId;
            if (gBattleMoves[moveInfo->moves[chosenMoveId]].target == MOVE_TARGET_BOTH || gBattleMoves[moveInfo->moves[chosenMoveId]].target == MOVE_TARGET_FOES_AND_ALLY
			|| gBattleMoves[moveInfo->moves[chosenMoveId]].target == MOVE_TARGET_ALL_BATTLERS || gBattleMoves[moveInfo->moves[chosenMoveId]].target == MOVE_TARGET_OPPONENTS)
            {
                gBattlerTarget = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
                if (gAbsentBattlerFlags & gBitTable[gBattlerTarget])
                    gBattlerTarget = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
            }
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, (chosenMoveId) | (gBattlerTarget << 8));
            break;
        }
        BattleControllerComplete(battlerId);
    }
    else
    {
        u16 i, move, speciesAttacker, speciesAttackerPartner;

        do
        {
            chosenMoveId = Random() & 3;
            move = moveInfo->moves[chosenMoveId];
        }
        while (move == MOVE_NONE);
		
        if (gBattleMoves[move].target == MOVE_TARGET_USER
		|| gBattleMoves[move].target == MOVE_TARGET_ALL_BATTLERS || gBattleMoves[move].target == MOVE_TARGET_USER_OR_ALLY)
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, (chosenMoveId) | (battlerId << 8));
        else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
		{
			target = GetBattlerAtPosition(Random() & 2);
		
#if DOUBLE_WILD_ATTACK_NATURAL_ENEMY
			if (gBattleMoves[move].target != MOVE_TARGET_BOTH && gBattleMoves[move].target != MOVE_TARGET_FOES_AND_ALLY
			&& gBattleMoves[move].target != MOVE_TARGET_OPPONENTS)
			{
				speciesAttacker = gBattleMons[battlerId].species;
				speciesAttackerPartner = gBattleMons[GetBattlerAtPosition(BATTLE_PARTNER(battlerId))].species;
				
				for (i = 0; i < ARRAY_COUNT(sNaturalEnemySpecies); i++)
				{
					if (speciesAttacker == sNaturalEnemySpecies[i][0] && speciesAttackerPartner == sNaturalEnemySpecies[i][1])
					{
						target = GetBattlerAtPosition(BATTLE_PARTNER(battlerId));
						break;
					}
				}
			}
#endif
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, (chosenMoveId) | (target << 8));
		}
        else
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, (chosenMoveId) | (GetBattlerAtPosition(B_POSITION_PLAYER_LEFT) << 8));

        BattleControllerComplete(battlerId);
    }
}

static void OpponentHandleChooseItem(u8 battlerId)
{
    BtlController_EmitOneReturnValue(battlerId, BUFFER_B, gBattleStruct->chosenItem[battlerId]);
    BattleControllerComplete(battlerId);
}

static void OpponentHandleChoosePokemon(u8 battlerId)
{
	s32 chosenMonId;

    if (*(gBattleStruct->AI_monToSwitchIntoId + (GetBattlerPosition(battlerId) >> 1)) == PARTY_SIZE)
    {
        chosenMonId = GetMostSuitableMonToSwitchInto(battlerId);

        if (chosenMonId == PARTY_SIZE)
        {
            s32 battler1, battler2;

            if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
                battler2 = battler1 = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
            else
            {
                battler1 = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
                battler2 = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
            }
            for (chosenMonId = 0; chosenMonId < PARTY_SIZE; ++chosenMonId)
			{
                if (GetMonData(&gEnemyParty[chosenMonId], MON_DATA_HP) && chosenMonId != gBattlerPartyIndexes[battler1] && chosenMonId != gBattlerPartyIndexes[battler2])
                    break;
			}
        }
    }
    else
    {
        chosenMonId = *(gBattleStruct->AI_monToSwitchIntoId + (GetBattlerPosition(battlerId) >> 1));
        *(gBattleStruct->AI_monToSwitchIntoId + (GetBattlerPosition(battlerId) >> 1)) = PARTY_SIZE;
    }
    gBattleStruct->monToSwitchIntoId[battlerId] = chosenMonId;
    BtlController_EmitChosenMonReturnValue(battlerId, BUFFER_B, chosenMonId, NULL);
    BattleControllerComplete(battlerId);
}

void OpponentHandleHealthbarUpdate(u8 battlerId)
{
	BtlController_HandleHealthbarUpdate(battlerId, FALSE);
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
	
    if (var && gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim && gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].finishedShinyMonAnim)
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim = FALSE;
        gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim = FALSE;
        gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].triedShinyMonAnim = FALSE;
        gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].finishedShinyMonAnim = FALSE;
		
        FreeSpriteTilesByTag(ANIM_TAG_GOLD_STARS);
        FreeSpritePaletteByTag(ANIM_TAG_GOLD_STARS);
		
        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            m4aMPlayContinue(&gMPlayInfo_BGM);
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
		
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !(gBattleTypeFlags & BATTLE_TYPE_MULTI))
			ShowHealthBox(BATTLE_PARTNER(battlerId));
		
        ShowHealthBox(battlerId);
        gBattleSpritesDataPtr->animationData->healthboxSlideInStarted = FALSE;
        gBattlerControllerFuncs[battlerId] = Intro_WaitForShinyAnimAndHealthbox;
    }
}

static void OpponentHandleIntroTrainerBallThrow(u8 battlerId)
{
	BtlController_HandleIntroTrainerBallThrow(battlerId, 0x0000, 0, StartAnimLinearTranslation, 0, Intro_TryShinyAnimShowHealthbox);
}

static void OpponentHandleEndLinkBattle(u8 battlerId)
{
	if (gBattleTypeFlags & BATTLE_TYPE_LINK && !(gBattleTypeFlags & BATTLE_TYPE_IS_MASTER))
    {
        gMain.inBattle = FALSE;
        gMain.callback1 = gPreBattleCallback1;
        SetMainCallback2(gMain.savedCallback);
    }
	BattleControllerComplete(battlerId);
}

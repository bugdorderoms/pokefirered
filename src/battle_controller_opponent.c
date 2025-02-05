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
#include "battle_ai.h"
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
    gBattlerSpriteIds[battlerId] = CreateSprite(&gMultiuseSpriteTemplate, GetBattlerSpriteCoord(battlerId, BATTLER_COORD_X), y, GetBattlerSpriteSubpriority(battlerId));
    gSprites[gBattlerSpriteIds[battlerId]].x2 = -240;
    gSprites[gBattlerSpriteIds[battlerId]].data[0] = battlerId;
    gSprites[gBattlerSpriteIds[battlerId]].data[2] = species;
    gSprites[gBattlerSpriteIds[battlerId]].oam.paletteNum = battlerId;
    StartSpriteAnim(&gSprites[gBattlerSpriteIds[battlerId]], 0);
	
    if (!(gBattleTypeFlags & BATTLE_TYPE_GHOST))
        SetBattlerShadowSpriteCallback(battlerId);
	
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
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim && !gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive)
        TryShinyAnimation(battlerId);
	
    if (gSprites[gBattleControllerData[battlerId]].callback == SpriteCallbackDummy && !gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive)
    {
        DestroySprite(&gSprites[gBattleControllerData[battlerId]]);
        SetBattlerShadowSpriteCallback(battlerId);
        gBattlerControllerFuncs[battlerId] = SwitchIn_ShowHealthbox;
    }
}

static void OpponentHandleSwitchInAnim(u8 battlerId)
{
	gBattleStruct->battlers[battlerId].monToSwitchIntoId = PARTY_SIZE;
	BtlController_HandleSwitchInAnim(battlerId, FALSE, SwitchIn_TryShinyAnim);
}

static inline u32 GetOpponentTrainerPicId(void)
{
	return gTrainers[gTrainerBattleOpponent_A].trainerPic;
}

static void OpponentHandleDrawTrainerPic(u8 battlerId)
{
	u32 trainerPicId = GetOpponentTrainerPicId();
	BtlController_HandleDrawTrainerPic(battlerId, trainerPicId, TRUE, 176, (8 - gTrainerFrontPicTable[trainerPicId].coords.size) * 4 + 40, GetBattlerSpriteSubpriority(battlerId));
}

static void OpponentHandleTrainerSlide(u8 battlerId)
{
	u32 trainerPicId = GetOpponentTrainerPicId();
	BtlController_HandleTrainerSlide(battlerId, trainerPicId, TRUE, 176, (8 - gTrainerFrontPicTable[trainerPicId].coords.size) * 4 + 40);
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
	BattleAI_ChooseAction(battlerId);
    BattleControllerComplete(battlerId);
}

#if DOUBLE_WILD_ATTACK_NATURAL_ENEMY

// based off: https://bulbapedia.bulbagarden.net/wiki/Pokémon_predation#Known_predatory_relation
// The first species attacks the second.
static const u16 sEnemyPreySpecies[][2] =
{
	// Predator-prey
	{SPECIES_PIDGEOTTO,         SPECIES_EXEGGCUTE},
	{SPECIES_PIDGEOTTO,         SPECIES_MAGIKARP},
	{SPECIES_SPEAROW,           SPECIES_SUNKERN},
	{SPECIES_EKANS,             SPECIES_PIDGEY},
	{SPECIES_EKANS,             SPECIES_SPEAROW},
	{SPECIES_DEWGONG,           SPECIES_WISHIWASHI},
	{SPECIES_GRIMER_ALOLAN,     SPECIES_TRUBBISH},
	{SPECIES_GRIMER_ALOLAN,     SPECIES_GARBODOR},
	{SPECIES_MUK_ALOLAN,        SPECIES_TRUBBISH},
	{SPECIES_MUK_ALOLAN,        SPECIES_GARBODOR},
	{SPECIES_SHELLDER,          SPECIES_SLOWPOKE},
	{SPECIES_CLOYSTER,          SPECIES_SLOWPOKE},
	{SPECIES_KINGLER,           SPECIES_SHELLDER},
	{SPECIES_KINGLER,           SPECIES_CLOYSTER},
	{SPECIES_MAROWAK_ALOLAN,    SPECIES_MANDIBUZZ},
	{SPECIES_WEEZING_GALARIAN,  SPECIES_TRUBBISH},
	{SPECIES_SCYTHER,           SPECIES_TAROUNTULA},
	{SPECIES_OMASTAR,           SPECIES_SHELLDER},
	{SPECIES_FURRET,            SPECIES_RATTATA},
	{SPECIES_SPINARAK,          SPECIES_CUTIEFLY},
	{SPECIES_AIPOM,             SPECIES_BOUNSWEET},
	{SPECIES_PINECO,            SPECIES_CUTIEFLY},
	{SPECIES_SNEASEL,           SPECIES_PIDGEY},
	{SPECIES_SNEASEL,           SPECIES_SANDSHREW_ALOLAN},
	{SPECIES_REMORAID,          SPECIES_BURMY},
	{SPECIES_TAILLOW,           SPECIES_WURMPLE},
	{SPECIES_SWELLOW,           SPECIES_WURMPLE},
	{SPECIES_WINGULL,           SPECIES_FINNEON},
	{SPECIES_WINGULL,           SPECIES_WISHIWASHI},
	{SPECIES_PELIPPER,          SPECIES_LUVDISC},
	{SPECIES_PELIPPER,          SPECIES_WISHIWASHI},
	{SPECIES_SABLEYE,           SPECIES_CARBINK},
	{SPECIES_SHARPEDO,          SPECIES_WAILMER},
	{SPECIES_WAILMER,           SPECIES_WISHIWASHI},
	{SPECIES_WAILORD,           SPECIES_WISHIWASHI},
	{SPECIES_GLALIE,            SPECIES_VANILLITE},
	{SPECIES_METANG,            SPECIES_NOSEPASS},
	{SPECIES_STARLY,            SPECIES_WURMPLE},
	{SPECIES_STARLY,            SPECIES_WORMADAM_SANDY_CLOAK},
	{SPECIES_STARLY,            SPECIES_CHERUBI},
	{SPECIES_LUMINEON,          SPECIES_STARYU},
	{SPECIES_LUMINEON,          SPECIES_STARMIE},
	{SPECIES_WEAVILE,           SPECIES_SANDSHREW_ALOLAN},
	{SPECIES_WEAVILE,           SPECIES_VULPIX_ALOLAN},
	{SPECIES_WEAVILE,           SPECIES_MAMOSWINE},
	{SPECIES_SANDILE,           SPECIES_TRAPINCH},
	{SPECIES_DARUMAKA_GALARIAN, SPECIES_SNOVER},
	{SPECIES_CARRACOSTA,        SPECIES_OMANYTE},
	{SPECIES_CARRACOSTA,        SPECIES_OMASTAR},
	{SPECIES_ARCHEOPS,          SPECIES_OMANYTE},
	{SPECIES_KARRABLAST,        SPECIES_SHELMET},
	{SPECIES_BEHEEYEM,          SPECIES_DUBWOOL},
	{SPECIES_DRUDDIGON,         SPECIES_DIGLETT_ALOLAN},
	{SPECIES_DRUDDIGON,         SPECIES_DUGTRIO_ALOLAN},
	{SPECIES_RUFFLET,           SPECIES_SHELLDER},
	{SPECIES_RUFFLET,           SPECIES_SPEWPA},
	{SPECIES_MANDIBUZZ,         SPECIES_CUBONE},
	{SPECIES_HEATMOR,           SPECIES_DURANT},
	{SPECIES_TALONFLAME,        SPECIES_WINGULL},
	{SPECIES_TALONFLAME,        SPECIES_PIKIPEK},
	{SPECIES_DRAGALGE,          SPECIES_FINIZEN},
	{SPECIES_PIKIPEK,           SPECIES_METAPOD},
	{SPECIES_TOUCANNON,         SPECIES_BOUNSWEET},
	{SPECIES_GUMSHOOS,          SPECIES_RATTATA_ALOLAN},
	{SPECIES_GUMSHOOS,          SPECIES_RATICATE_ALOLAN},
	{SPECIES_GUMSHOOS,          SPECIES_SKWOVET},
	{SPECIES_CRABRAWLER,        SPECIES_EXEGGCUTE},
	{SPECIES_LYCANROC,          SPECIES_DEERLING},
	{SPECIES_MAREANIE,          SPECIES_CORSOLA},
	{SPECIES_MAREANIE,          SPECIES_PINCURCHIN},
	{SPECIES_TOXAPEX,           SPECIES_CORSOLA},
	{SPECIES_SALANDIT,          SPECIES_SPINDA},
	{SPECIES_BRUXISH,           SPECIES_SHELLDER},
	{SPECIES_BRUXISH,           SPECIES_MAREANIE},
	{SPECIES_DHELMISE,          SPECIES_WAILMER},
	{SPECIES_DHELMISE,          SPECIES_WAILORD},
	{SPECIES_GREEDENT,          SPECIES_BOUNSWEET},
	{SPECIES_ROOKIDEE,          SPECIES_GRUBBIN},
	{SPECIES_CORVISQUIRE,       SPECIES_STEENEE},
	{SPECIES_CORVIKNIGHT,       SPECIES_BUNNELBY},
	{SPECIES_SANDACONDA,        SPECIES_DURANT},
	{SPECIES_CRAMORANT,         SPECIES_ARROKUDA},
	{SPECIES_BARRASKEWDA,       SPECIES_WINGULL},
	{SPECIES_WATTREL,           SPECIES_ARROKUDA},
	{SPECIES_DONDOZO,           SPECIES_BASCULIN},
	{SPECIES_VELUZA,            SPECIES_WIGLETT},
	{SPECIES_BOMBIRDIER,        SPECIES_BASCULIN},
	// Parasitic prey
	{SPECIES_HYPNO,             SPECIES_KOMALA},
	{SPECIES_DWEBBLE,           SPECIES_ROGGENROLA},
	{SPECIES_DWEBBLE,           SPECIES_ROLYCOLY},
	{SPECIES_JOLTIK,            SPECIES_YAMPER},
	{SPECIES_CUTIEFLY,          SPECIES_GOSSIFLEUR},
	{SPECIES_LECHONK,           SPECIES_APPLETUN},
	{SPECIES_TINKATUFF,         SPECIES_PAWNIARD},
	{SPECIES_TINKATUFF,         SPECIES_BISHARP},
	{SPECIES_TINKATON,          SPECIES_CORVIKNIGHT},
};

// The species attacks each other.
static const u16 sNaturalEnemySpecies[][2] =
{
	// Rivals
	{SPECIES_BUTTERFREE,      SPECIES_CUTIEFLY},
	{SPECIES_BEEDRILL,        SPECIES_TEDDIURSA},
	{SPECIES_PARASECT,        SPECIES_SHIINOTIC},
	{SPECIES_MEOWTH,          SPECIES_MURKROW},
	{SPECIES_PERSIAN,         SPECIES_PERRSERKER},
	{SPECIES_PRIMEAPE,        SPECIES_URSARING},
	{SPECIES_PRIMEAPE,        SPECIES_HAWLUCHA},
	{SPECIES_GROWLITHE,       SPECIES_ROCKRUFF},
	{SPECIES_PINSIR,          SPECIES_VIKAVOLT},
	{SPECIES_DRAGONITE,       SPECIES_KINGDRA},
	{SPECIES_LANTURN,         SPECIES_LUMINEON},
	{SPECIES_HERACROSS,       SPECIES_VIKAVOLT},
	{SPECIES_TEDDIURSA,       SPECIES_COMBEE},
	{SPECIES_SKARMORY,        SPECIES_CORVIKNIGHT},
	{SPECIES_BEAUTIFLY,       SPECIES_COMBEE},
	{SPECIES_SURSKIT,         SPECIES_DEWPIDER},
	{SPECIES_SABLEYE,         SPECIES_GABITE},
	{SPECIES_CARVANHA,        SPECIES_BASCULIN},
	{SPECIES_ZANGOOSE,        SPECIES_SEVIPER},
	{SPECIES_SALAMENCE,       SPECIES_GARCHOMP},
	{SPECIES_MOTHIM,          SPECIES_COMBEE},
	{SPECIES_AMBIPOM,         SPECIES_PASSIMIAN},
	{SPECIES_VENIPEDE,        SPECIES_SIZZLIPEDE},
	{SPECIES_SCOLIPEDE,       SPECIES_CENTISKORCH},
	{SPECIES_FLETCHLING,      SPECIES_SQUAWKABILLY},
	{SPECIES_FLETCHLING,      SPECIES_SQUAWKABILLY_BLUE},
	{SPECIES_FLETCHLING,      SPECIES_SQUAWKABILLY_YELLOW},
	{SPECIES_FLETCHLING,      SPECIES_SQUAWKABILLY_WHITE},
	{SPECIES_BERGMITE,        SPECIES_FRIGIBAX},
	{SPECIES_TOXAPEX,         SPECIES_BRUXISH},
	{SPECIES_GOLISOPOD,       SPECIES_GRAPPLOCT},
	{SPECIES_THIEVUL,         SPECIES_BOLTUND},
	{SPECIES_BARRASKEWDA,     SPECIES_FINIZEN},
	{SPECIES_ELEKID,          SPECIES_TOGEDEMARU},
};

static bool8 IsAllySpeciesPrey(u16 speciesAttacker, u16 speciesAttackerPartner)
{
	u32 i;
	
	for (i = 0; i < ARRAY_COUNT(sEnemyPreySpecies); i++)
	{
		if (speciesAttacker == sEnemyPreySpecies[i][0] && speciesAttackerPartner == sEnemyPreySpecies[i][1])
			return TRUE;
	}
	return FALSE;
}

static bool8 IsAllySpeciesNaturalEnemy(u16 speciesAttacker, u16 speciesAttackerPartner)
{
	u32 i;
	
	for (i = 0; i < ARRAY_COUNT(sNaturalEnemySpecies); i++)
	{
		if ((speciesAttacker == sNaturalEnemySpecies[i][0] && speciesAttackerPartner == sNaturalEnemySpecies[i][1])
		|| (speciesAttackerPartner == sNaturalEnemySpecies[i][0] && speciesAttacker == sNaturalEnemySpecies[i][1]))
			return TRUE;
	}
	return FALSE;
}
#endif

static void OpponentHandleChooseMove(u8 battlerId)
{
	u8 moveTarget;
	struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct *)(&gBattleBufferA[battlerId][4]);
	
    if ((gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_FIRST_BATTLE | BATTLE_TYPE_SAFARI | BATTLE_TYPE_ROAMER)))
    {
		BattleAI_ComputeMovesScore(battlerId);
		
        switch (gBattleStruct->battlers[battlerId].aiMoveOrAction)
        {
        case AI_CHOICE_WATCH:
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_SAFARI_WATCH_CAREFULLY, 0);
            break;
        case AI_CHOICE_FLEE:
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_RUN, 0);
            break;
		case AI_CHOICE_SWITCH:
			BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_SWITCH, 0);
			break;
        default:
			gBattlerTarget = gBattleStruct->battlers[battlerId].aiChosenTarget;
			
			moveTarget = GetBattlerMoveTargetType(battlerId, moveInfo->moves[gBattleStruct->battlers[battlerId].aiMoveOrAction]);
			
			if (moveTarget == MOVE_TARGET_USER || moveTarget == MOVE_TARGET_ALL_BATTLERS)
				gBattlerTarget = battlerId;
			else if (moveTarget == MOVE_TARGET_BOTH)
			{
				gBattlerTarget = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
				if (gAbsentBattlerFlags & gBitTable[gBattlerTarget])
					gBattlerTarget = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
			}
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, (gBattleStruct->battlers[battlerId].aiMoveOrAction) | (gBattlerTarget << 8));
            break;
        }
    }
    else
    {
		u8 target, chosenMoveId;
        u16 move, speciesAttacker, speciesAttackerPartner;

        do
        {
            chosenMoveId = RandomMax(MAX_MON_MOVES);
            move = moveInfo->moves[chosenMoveId];
        }
        while (!move);
		
		moveTarget = GetBattlerMoveTargetType(battlerId, move);
		
		if (moveTarget == MOVE_TARGET_USER || moveTarget == MOVE_TARGET_ALL_BATTLERS)
			BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, (chosenMoveId) | (battlerId << 8));
		else if (IsDoubleBattleOnSide(B_SIDE_PLAYER))
		{
			do
			{
				target = GetBattlerAtPosition(Random() & 2);
			} while (!CanTargetBattler(battlerId, target, move, moveTarget));
			
#if DOUBLE_WILD_ATTACK_NATURAL_ENEMY
			if (moveTarget != MOVE_TARGET_BOTH && moveTarget != MOVE_TARGET_FOES_AND_ALLY && moveTarget != MOVE_TARGET_OPPONENTS && CanTargetBattler(battlerId, BATTLE_PARTNER(battlerId), move, moveTarget))
			{
				speciesAttacker = gBattleMons[battlerId].species;
				speciesAttackerPartner = gBattleMons[BATTLE_PARTNER(battlerId)].species;
				
				if (IsAllySpeciesPrey(speciesAttacker, speciesAttackerPartner) || IsAllySpeciesNaturalEnemy(speciesAttacker, speciesAttackerPartner))
					target = BATTLE_PARTNER(battlerId);
			}
#endif
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, (chosenMoveId) | (target << 8));
		}
        else
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, (chosenMoveId) | (GetBattlerAtPosition(B_POSITION_PLAYER_LEFT) << 8));
    }
	BattleControllerComplete(battlerId);
}

static void OpponentHandleChooseItem(u8 battlerId)
{
    BtlController_EmitOneReturnValue(battlerId, BUFFER_B, gBattleStruct->battlers[battlerId].chosenItem);
    BattleControllerComplete(battlerId);
}

static void OpponentHandleChoosePokemon(u8 battlerId)
{
	s32 chosenMonId;

    if (*(gBattleStruct->AI_monToSwitchIntoId + (GetBattlerPosition(battlerId) >> 1)) == PARTY_SIZE)
    {
        chosenMonId = 0/*GetMostSuitableMonToSwitchInto(battlerId)*/;

        if (chosenMonId == PARTY_SIZE)
        {
            s32 battler1, battler2;

            if (!IsDoubleBattleOnSide(B_SIDE_OPPONENT))
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
    gBattleStruct->battlers[battlerId].monToSwitchIntoId = chosenMonId;
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

    if (!IsDoubleBattleForBattler(battlerId) || (((gBattleTypeFlags & BATTLE_TYPE_MULTI) && (gBattleTypeFlags & BATTLE_TYPE_DOUBLE))))
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
		
        if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI) && IsDoubleBattleForBattler(battlerId))
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

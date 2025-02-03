#ifndef GUARD_BATTLE_CONTROLLERS_H
#define GUARD_BATTLE_CONTROLLERS_H

#include "global.h"
#include "battle.h"

enum
{
    CONTROLLER_GETMONDATA,
    CONTROLLER_SETMONDATA,
    CONTROLLER_SETRAWMONDATA,
    CONTROLLER_LOADMONSPRITE,
    CONTROLLER_SWITCHINANIM,
    CONTROLLER_RETURNMONTOBALL,
    CONTROLLER_DRAWTRAINERPIC,
    CONTROLLER_TRAINERSLIDE,
    CONTROLLER_TRAINERSLIDEBACK,
    CONTROLLER_FAINTANIMATION,
    CONTROLLER_BALLTHROWANIM,
    CONTROLLER_MOVEANIMATION,
    CONTROLLER_PRINTSTRING,
    CONTROLLER_PRINTSELECTIONSTRING,
    CONTROLLER_CHOOSEACTION,
    CONTROLLER_CHOOSEMOVE,
    CONTROLLER_OPENBAG,
    CONTROLLER_CHOOSEPOKEMON,
    CONTROLLER_HEALTHBARUPDATE,
    CONTROLLER_EXPUPDATE,
    CONTROLLER_STATUSICONUPDATE,
    CONTROLLER_STATUSANIMATION,
	CONTROLLER_DATATRANSFER,
    CONTROLLER_TWORETURNVALUES,
    CONTROLLER_CHOSENMONRETURNVALUE,
    CONTROLLER_ONERETURNVALUE,
    CONTROLLER_EFFECTIVENESSSOUND,
    CONTROLLER_PLAYFANFAREORBGM,
    CONTROLLER_FAINTINGCRY,
    CONTROLLER_INTROSLIDE,
    CONTROLLER_INTROTRAINERBALLTHROW,
    CONTROLLER_DRAWPARTYSTATUSSUMMARY,
    CONTROLLER_HIDEPARTYSTATUSSUMMARY,
    CONTROLLER_ENDBOUNCE,
    CONTROLLER_SPRITEINVISIBILITY,
    CONTROLLER_BATTLEANIMATION,
    CONTROLLER_LINKSTANDBYMSG,
    CONTROLLER_RESETACTIONMOVESELECTION,
    CONTROLLER_ENDLINKBATTLE,
    /*new controllers should go here*/
    CONTROLLER_TERMINATOR_NOP,
    CONTROLLER_CMDS_COUNT
};

// Values given to the emit functions to choose gBattleBufferA or gBattleBufferB
enum
{
	BUFFER_A,
	BUFFER_B,
	BUFFER_LINK
};

enum
{
    REQUEST_ALL_BATTLE,
    REQUEST_SPECIES_BATTLE,
    REQUEST_HELDITEM_BATTLE,
    REQUEST_MOVES_PP_BATTLE,
    REQUEST_MOVE1_BATTLE,
    REQUEST_MOVE2_BATTLE,
    REQUEST_MOVE3_BATTLE,
    REQUEST_MOVE4_BATTLE,
    REQUEST_PP_DATA_BATTLE,
    REQUEST_PPMOVE1_BATTLE,
    REQUEST_PPMOVE2_BATTLE,
    REQUEST_PPMOVE3_BATTLE,
    REQUEST_PPMOVE4_BATTLE,
    REQUEST_OTID_BATTLE,
    REQUEST_EXP_BATTLE,
    REQUEST_HP_EV_BATTLE,
    REQUEST_ATK_EV_BATTLE,
    REQUEST_DEF_EV_BATTLE,
    REQUEST_SPEED_EV_BATTLE,
    REQUEST_SPATK_EV_BATTLE,
    REQUEST_SPDEF_EV_BATTLE,
    REQUEST_FRIENDSHIP_BATTLE,
    REQUEST_POKERUS_BATTLE,
    REQUEST_MET_LOCATION_BATTLE,
    REQUEST_MET_LEVEL_BATTLE,
    REQUEST_MET_GAME_BATTLE,
    REQUEST_POKEBALL_BATTLE,
    REQUEST_ALL_IVS_BATTLE,
    REQUEST_HP_IV_BATTLE,
    REQUEST_ATK_IV_BATTLE,
    REQUEST_DEF_IV_BATTLE,
    REQUEST_SPEED_IV_BATTLE,
    REQUEST_SPATK_IV_BATTLE,
    REQUEST_SPDEF_IV_BATTLE,
    REQUEST_PERSONALITY_BATTLE,
    REQUEST_STATUS_BATTLE,
    REQUEST_LEVEL_BATTLE,
	REQUEST_MAX_HP_BATTLE,
    REQUEST_HP_BATTLE,
    REQUEST_ATK_BATTLE,
    REQUEST_DEF_BATTLE,
    REQUEST_SPEED_BATTLE,
    REQUEST_SPATK_BATTLE,
    REQUEST_SPDEF_BATTLE,
};

enum
{
	BALL_NO_SHAKES,
	BALL_1_SHAKE,
	BALL_2_SHAKES,
	BALL_3_SHAKES_FAIL,
	BALL_3_SHAKES_SUCCESS,
	BALL_TRAINER_BLOCK,
	BALL_GHOST_DODGE,
};

enum
{
	RESET_ACTION_MOVE_SELECTION,
	RESET_ACTION_SELECTION,
	RESET_MOVE_SELECTION,
};

#define INSTANT_HP_BAR_DROP     0x7FFF

// Special return values in gBattleBufferB from Battle Controller functions.
#define RET_VALUE_LEVELED_UP   11

struct MovePpInfo
{
    u16 moves[MAX_MON_MOVES];
    u8 pp[MAX_MON_MOVES];
    u8 ppBonuses;
};

struct HpAndStatus
{
    u16 hp;
    struct Status1 status;
};

struct ChooseMoveStruct
{
	u16 species;
    u16 moves[MAX_MON_MOVES];
    u8 currentPp[MAX_MON_MOVES];
    u8 maxPp[MAX_MON_MOVES];
};

#define FIRST_BATTLE_MSG_FLAG_INFLICT_DMG (1 << 0) // Inflicting damage is key
#define FIRST_BATTLE_MSG_FLAG_STAT_CHG    (1 << 1) // Lowering stats is advantageous
#define FIRST_BATTLE_MSG_FLAG_HP_RESTORE  (1 << 2) // Keep an eye on your HP
#define FIRST_BATTLE_MSG_FLAG_PARTY_MENU  (1 << 3)

// general functions
void HandleLinkBattleSetup(void);
void SetUpBattleVars(void);
void InitBtlControllers(void);
void SetBattleEndCallbacks(u8 battlerId);
void TryReceiveLinkBattleData(void);
void PrepareBufferDataTransferLink(u8 battlerId, u8 bufferId, u16 size, u8 *data);
void CompleteOnBattlerSpritePosX_0(u8 battlerId);
void TryShinyAnimAfterMonAnim(u8 battlerId);
void WaitForMonAnimAfterLoad(u8 battlerId);
void Task_BltController_RestoreBgmAfterCry(u8 taskId);
void ShowHealthBox(u8 battlerId);

// emitters
void BtlController_EmitGetMonData(u8 battlerId, u8 bufferId, u8 requestId, u8 monToCheck);
void BtlController_EmitSetMonData(u8 battlerId, u8 bufferId, u8 requestId, u8 monToCheck, u8 bytes, void *data);
void BtlController_EmitSetRawMonData(u8 battlerId, u8 bufferId, u8 offset, u8 bytes, void *data);
void BtlController_EmitLoadMonSprite(u8 battlerId, u8 bufferId);
void BtlController_EmitSwitchInAnim(u8 battlerId, u8 bufferId, u8 partyId, bool8 dontClearSubstituteBit);
void BtlController_EmitReturnMonToBall(u8 battlerId, u8 bufferId, u8 arg1);
void BtlController_EmitDrawTrainerPic(u8 battlerId, u8 bufferId);
void BtlController_EmitTrainerSlide(u8 battlerId, u8 bufferId);
void BtlController_EmitTrainerSlideBack(u8 battlerId, u8 bufferId);
void BtlController_EmitFaintAnimation(u8 battlerId, u8 bufferId);
void BtlController_EmitBallThrowAnim(u8 battlerId, u8 bufferId, u8 caseId);
void BtlController_EmitMoveAnimation(u8 battlerId, u8 bufferId, u16 move, u8 turnOfMove, u16 movePower, s32 dmg, u8 friendship, struct DisableStruct *disableStructPtr);
void BtlController_EmitPrintString(u8 battlerId, u8 bufferId, u16 stringId);
void BtlController_EmitPrintSelectionString(u8 battlerId, u8 bufferId, u16 stringId);
void BtlController_EmitChooseAction(u8 battlerId, u8 bufferId, u8 arg1, u16 arg2);
void BtlController_EmitChooseMove(u8 battlerId, u8 bufferId, bool8 isDoubleBattle, bool8 NoPpNumber, struct ChooseMoveStruct *movePpData);
void BtlController_EmitChooseItem(u8 battlerId, u8 bufferId, u8 *arg1);
void BtlController_EmitChoosePokemon(u8 battlerId, u8 bufferId, u8 caseId, u8 arg2, u8* arg4);
void BtlController_EmitHealthBarUpdate(u8 battlerId, u8 bufferId, u16 hpValue);
void BtlController_EmitExpUpdate(u8 battlerId, u8 bufferId, u8 partyId, u16 expPoints);
void BtlController_EmitStatusIconUpdate(u8 battlerId, u8 bufferId, u8 status1Id, u32 status2);
void BtlController_EmitStatusAnimation(u8 battlerId, u8 bufferId, u8 animStatusId, u32 status);
void BtlController_EmitDataTransfer(u8 battlerId, u8 bufferId, u16 size, void *data);
void BtlController_EmitTwoReturnValues(u8 battlerId, u8 bufferId, u8 arg1, u16 arg2);
void BtlController_EmitChosenMonReturnValue(u8 battlerId, u8 bufferId, u8 b, u8 *c);
void BtlController_EmitOneReturnValue(u8 battlerId, u8 bufferId, u16 arg1);
void BtlController_EmitPlaySE(u8 battlerId, u8 bufferId, u16 songId);
void BtlController_EmitPlayFanfare(u8 battlerId, u8 bufferId, u16 songId);
void BtlController_EmitFaintingCry(u8 battlerId, u8 bufferId);
void BtlController_EmitIntroSlide(u8 battlerId, u8 bufferId, u8 terrainId);
void BtlController_EmitIntroTrainerBallThrow(u8 battlerId, u8 bufferId);
void BtlController_EmitDrawPartyStatusSummary(u8 battlerId, u8 bufferId, struct HpAndStatus *hpAndStatus, u8 param);
void BtlController_EmitHidePartyStatusSummary(u8 battlerId, u8 bufferId);
void BtlController_EmitEndBounceEffect(u8 battlerId, u8 bufferId);
void BtlController_EmitSpriteInvisibility(u8 battlerId, u8 bufferId, bool8 isInvisible);
void BtlController_EmitBattleAnimation(u8 battlerId, u8 bufferId, u8 animationId, u16 argument);
void BtlController_EmitLinkStandbyMsg(u8 battlerId, u8 bufferId, u8 arg1);
void BtlController_EmitResetActionMoveSelection(u8 battlerId, u8 bufferId, u8 caseId);
void BtlController_EmitEndLinkBattle(u8 battlerId, u8 bufferId, u8 battleOutcome);

// general controllers functions
void ControllerDummy(u8 battlerId);
void BattleControllerComplete(u8 battlerId);
void BtlController_HandleGetMonData(u8 battlerId);
void BtlController_HandleSetMonData(u8 battlerId);
void BtlController_HandleSetRawMonData(u8 battlerId);
void BtlController_HandleLoadMonSprite(u8 battlerId, bool8 setSpecies, void(*controllerFunc)(u8));
void BtlController_HandleSwitchInAnim(u8 battlerId, bool8 loadSprite, void(*controllerFunc)(u8));
void BtlController_HandleReturnMonToBall(u8 battlerId);
void BtlController_HandleDrawTrainerPic(u8 battlerId, u32 trainerPicId, bool8 isFront, s16 xPos, s16 yPos, u8 subpiority);
void BtlController_HandleTrainerSlide(u8 battlerId, u32 trainerPicId, bool8 isFront, s16 xPos, s16 yPos);
void BtlController_HandleTrainerSlideBack(u8 battlerId, s16 data0, bool8 animateSprite);
void BtlController_HandleFaintAnimation(u8 battlerId);
void BtlController_HandleBallThrowAnim(u8 battlerId, u8 animId);
void BtlController_HandleMoveAnimation(u8 battlerId);
void BtlController_HandlePrintString(u8 battlerId, u16 stringId, bool8 isSelection);
void BtlController_HandleChooseAction(u8 battlerId, const u8 *actionsStr, const u8 *whatDoStr, void(*controllerFunc)(u8));
void BtlController_HandleChooseItem(u8 battlerId, void(*controllerFunc)(u8));
void BtlController_HandleChoosePokemon(u8 battlerId, void(*controllerFunc)(u8));
void BtlController_HandleHealthbarUpdate(u8 battlerId, bool8 updateHpText);
void BtlController_HandleExpUpdate(u8 battlerId);
void BtlController_HandleStatusIconUpdate(u8 battlerId);
void BtlController_HandleStatusAnimation(u8 battlerId);
void BtlController_HandlePlaySE(u8 battlerId);
void BtlController_HandlePlayFanfare(u8 battlerId);
void BtlController_HandleFaintingCry(u8 battlerId);
void BtlController_HandleIntroSlide(u8 battlerId);
void BtlController_HandleIntroTrainerBallThrow(u8 battlerId, u16 loadPicTag, u32 trainerPicId, void(*spriteCallback)(struct Sprite *), u8 delay, void(*controllerFunc)(u8));
void BtlController_HandleDrawPartyStatusSummary(u8 battlerId);
void BtlController_HandleHidePartyStatusSummary(u8 battlerId);
void BtlController_HandleEndBounceEffect(u8 battlerId);
void BtlController_HandleSpriteInvisibility(u8 battlerId);
void BtlController_HandleBattleAnimation(u8 battlerId);
void BtlController_HandleEndLinkBattle(u8 battlerId, u8 battleOutcome, void(*controllerFunc)(u8));

// player controller
void SetControllerToPlayer(u8 battlerId);
void PlayerHandleDrawTrainerPic(u8 battlerId);
void PlayerHandleBallThrowAnim(u8 battlerId);
void PlayerHandlePrintString(u8 battlerId);
void PlayerHandlePrintSelectionString(u8 battlerId);
void PlayerHandleChooseAction(u8 battlerId);
void PlayerHandleChooseMove(u8 battlerId);
void PlayerHandleChooseItem(u8 battlerId);
void PlayerHandleHealthbarUpdate(u8 battlerId);
void PlayerHandleLinkStandbyMsg(u8 battlerId);
void PlayerHandleEndLinkBattle(u8 battlerId);
void ActionSelectionCreateCursorAt(u8 cursorPos);
void ActionSelectionDestroyCursorAt(u8 cursorPos);
void HandleInputChooseMove(u8 battlerId);
void InitMoveSelectionsVarsAndStrings(u8 battlerId);
void MoveSelectionCreateCursorAt(u8 cursorPos, u8 arg1);
void MoveSelectionDestroyCursorAt(u8 cursorPos);

// opponent controller
void SetControllerToOpponent(u8 battlerId);
void OpponentHandleTrainerSlideBack(u8 battlerId);
void OpponentHandleHealthbarUpdate(u8 battlerId);

// safari controller
void SetControllerToSafari(u8 battlerId);
void SafariHandleBallThrowAnim(u8 battlerId);
void SafariHandleFaintingCry(u8 battlerId);

// pokedude controller
void SetControllerToPokedude(u8 battlerId);
void InitPokedudePartyAndOpponent(void);
void Pokedude_SimulateInputChooseAction(u8 battlerId);

// oak and old man controller
void SetControllerToOakOrOldMan(u8 battlerId);
void OakOldManHandlePlaySE(u8 battlerId);
void OakOldMan_SimulateInputChooseAction(u8 battlerId);
void OakOldManHandleDrawPartyStatusSummary(u8 battlerId);
void OakOldManHandleEndLinkBattle(u8 battlerId);
void OakOldManHandleInputChooseMove(u8 battlerId);
bool8 BtlCtrl_OakOldMan_TestState2Flag(u8 mask);
void BtlCtrl_OakOldMan_SetState2Flag(u8 mask);
void PrintOakText_OakNoRunningFromATrainer(u8 battlerId);
void PrintOakText_HowDisappointing(u8 battlerId);
void PrintOakText_KeepAnEyeOnHP(u8 battlerId);
void PrintOakText_InflictingDamageIsKey(u8 battlerId);
void BtlCtrl_DrawVoiceoverMessageFrame(void);
void BtlCtrl_RemoveVoiceoverMessageFrame(void);

// link partner controller
void SetControllerToLinkPartner(u8 battlerId);

// link opponent controller
void SetControllerToLinkOpponent(u8 battlerId);

#endif // GUARD_BATTLE_CONTROLLERS_H

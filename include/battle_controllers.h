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
#define RET_GIMMICK            Bit(7)

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

#define FIRST_BATTLE_MSG_FLAG_INFLICT_DMG Bit(0) // Inflicting damage is key
#define FIRST_BATTLE_MSG_FLAG_STAT_CHG    Bit(1) // Lowering stats is advantageous
#define FIRST_BATTLE_MSG_FLAG_HP_RESTORE  Bit(2) // Keep an eye on your HP
#define FIRST_BATTLE_MSG_FLAG_PARTY_MENU  Bit(3)

// general functions
void HandleLinkBattleSetup(void);
void SetUpBattleVars(void);
void InitBtlControllers(void);
void InitSinglePlayerBtlControllers(bool32 fromBattleStart);
void SetBattleEndCallbacks(u32 battlerId);
void TryReceiveLinkBattleData(void);
void PrepareBufferDataTransferLink(u32 battlerId, u32 bufferId, u16 size, u8 *data);
void CompleteOnBattlerSpritePosX_0(u32 battlerId);
void TryShinyAnimAfterMonAnim(u32 battlerId);
void WaitForMonAnimAfterLoad(u32 battlerId);
void Task_BltController_RestoreBgmAfterCry(u32 taskId);
void ShowHealthBox(u32 battlerId);

// emitters
void BtlController_EmitGetMonData(u32 battlerId, u32 bufferId, u32 requestId, u32 monToCheck);
void BtlController_EmitSetMonData(u32 battlerId, u32 bufferId, u32 requestId, u32 monToCheck, u32 bytes, void *data);
void BtlController_EmitSetRawMonData(u32 battlerId, u32 bufferId, u32 offset, u32 bytes, void *data);
void BtlController_EmitLoadMonSprite(u32 battlerId, u32 bufferId);
void BtlController_EmitSwitchInAnim(u32 battlerId, u32 bufferId, u32 partyId, bool32 dontClearSubstituteBit);
void BtlController_EmitReturnMonToBall(u32 battlerId, u32 bufferId, u32 arg1);
void BtlController_EmitDrawTrainerPic(u32 battlerId, u32 bufferId);
void BtlController_EmitTrainerSlide(u32 battlerId, u32 bufferId);
void BtlController_EmitTrainerSlideBack(u32 battlerId, u32 bufferId);
void BtlController_EmitFaintAnimation(u32 battlerId, u32 bufferId);
void BtlController_EmitBallThrowAnim(u32 battlerId, u32 bufferId, u32 caseId);
void BtlController_EmitMoveAnimation(u32 battlerId, u32 bufferId, u32 move, u32 turnOfMove, u32 movePower, s32 dmg, u32 friendship, struct DisableStruct *disableStructPtr);
void BtlController_EmitPrintString(u32 battlerId, u32 bufferId, u32 stringId);
void BtlController_EmitPrintSelectionString(u32 battlerId, u32 bufferId, u32 stringId);
void BtlController_EmitChooseAction(u32 battlerId, u32 bufferId, u32 arg1, u32 arg2);
void BtlController_EmitChooseMove(u32 battlerId, u32 bufferId, bool32 isDoubleBattle, bool32 NoPpNumber, struct ChooseMoveStruct *movePpData);
void BtlController_EmitChooseItem(u32 battlerId, u32 bufferId, u8 *arg1);
void BtlController_EmitChoosePokemon(u32 battlerId, u32 bufferId, u32 caseId, u32 arg2, u8* arg4);
void BtlController_EmitHealthBarUpdate(u32 battlerId, u32 bufferId, u16 hpValue);
void BtlController_EmitExpUpdate(u32 battlerId, u32 bufferId, u32 partyId, u16 expPoints);
void BtlController_EmitStatusIconUpdate(u32 battlerId, u32 bufferId, u32 status1Id, u32 status2);
void BtlController_EmitStatusAnimation(u32 battlerId, u32 bufferId, u32 animStatusId, u32 status);
void BtlController_EmitDataTransfer(u32 battlerId, u32 bufferId, u16 size, void *data);
void BtlController_EmitTwoReturnValues(u32 battlerId, u32 bufferId, u32 arg1, u32 arg2);
void BtlController_EmitChosenMonReturnValue(u32 battlerId, u32 bufferId, u32 b, u8 *c);
void BtlController_EmitOneReturnValue(u32 battlerId, u32 bufferId, u32 arg1);
void BtlController_EmitPlaySE(u32 battlerId, u32 bufferId, u32 songId);
void BtlController_EmitPlayFanfare(u32 battlerId, u32 bufferId, u32 songId);
void BtlController_EmitFaintingCry(u32 battlerId, u32 bufferId);
void BtlController_EmitIntroSlide(u32 battlerId, u32 bufferId, u32 terrainId);
void BtlController_EmitIntroTrainerBallThrow(u32 battlerId, u32 bufferId);
void BtlController_EmitDrawPartyStatusSummary(u32 battlerId, u32 bufferId, struct HpAndStatus *hpAndStatus, u8 param);
void BtlController_EmitHidePartyStatusSummary(u32 battlerId, u32 bufferId);
void BtlController_EmitEndBounceEffect(u32 battlerId, u32 bufferId);
void BtlController_EmitSpriteInvisibility(u32 battlerId, u32 bufferId, bool32 isInvisible);
void BtlController_EmitBattleAnimation(u32 battlerId, u32 bufferId, u32 animationId, u32 argument);
void BtlController_EmitLinkStandbyMsg(u32 battlerId, u32 bufferId, u32 arg1);
void BtlController_EmitResetActionMoveSelection(u32 battlerId, u32 bufferId, u32 caseId);
void BtlController_EmitEndLinkBattle(u32 battlerId, u32 bufferId, u32 battleOutcome);

// general controllers functions
void ControllerDummy(u32 battlerId);
void BattleControllerComplete(u32 battlerId);
void BtlController_HandleGetMonData(u32 battlerId);
void BtlController_HandleSetMonData(u32 battlerId);
void BtlController_HandleSetRawMonData(u32 battlerId);
void BtlController_HandleLoadMonSprite(u32 battlerId, bool32 setSpecies, void(*controllerFunc)(u32));
void BtlController_HandleSwitchInAnim(u32 battlerId, bool32 loadSprite, void(*controllerFunc)(u32));
void BtlController_HandleReturnMonToBall(u32 battlerId);
void BtlController_HandleDrawTrainerPic(u32 battlerId, u32 trainerPicId, bool32 isFront, s16 xPos, s16 yPos, u32 subpriority);
void BtlController_HandleTrainerSlide(u32 battlerId, u32 trainerPicId, bool32 isFront, s16 xPos, s16 yPos);
void BtlController_HandleTrainerSlideBack(u32 battlerId, s16 data0, bool32 animateSprite);
void BtlController_HandleFaintAnimation(u32 battlerId);
void BtlController_HandleBallThrowAnim(u32 battlerId, u32 animId, bool32 allowCriticalCapture);
void BtlController_HandleMoveAnimation(u32 battlerId);
void BtlController_HandlePrintString(u32 battlerId, u32 stringId, bool32 isSelection);
void BtlController_HandleChooseAction(u32 battlerId, const u8 *actionsStr, const u8 *whatDoStr, void(*controllerFunc)(u32));
void BtlController_HandleChooseItem(u32 battlerId, void(*controllerFunc)(u32));
void BtlController_HandleChoosePokemon(u32 battlerId, void(*controllerFunc)(u32));
void BtlController_HandleHealthbarUpdate(u32 battlerId, bool32 updateHpText);
void BtlController_HandleExpUpdate(u32 battlerId);
void BtlController_HandleStatusIconUpdate(u32 battlerId);
void BtlController_HandleStatusAnimation(u32 battlerId);
void BtlController_HandlePlaySE(u32 battlerId);
void BtlController_HandlePlayFanfare(u32 battlerId);
void BtlController_HandleFaintingCry(u32 battlerId);
void BtlController_HandleIntroSlide(u32 battlerId);
void BtlController_HandleIntroTrainerBallThrow(u32 battlerId, u32 loadPicTag, u32 trainerPicId, void(*spriteCallback)(struct Sprite *), u32 delay, void(*controllerFunc)(u32));
void BtlController_HandleDrawPartyStatusSummary(u32 battlerId);
void BtlController_HandleHidePartyStatusSummary(u32 battlerId);
void BtlController_HandleEndBounceEffect(u32 battlerId);
void BtlController_HandleSpriteInvisibility(u32 battlerId);
void BtlController_HandleBattleAnimation(u32 battlerId);
void BtlController_HandleEndLinkBattle(u32 battlerId, u32 battleOutcome, void(*controllerFunc)(u32));

// player controller
void SetControllerToPlayer(u32 battlerId);
void PlayerHandleDrawTrainerPic(u32 battlerId);
void PlayerHandleBallThrowAnim(u32 battlerId);
void PlayerHandlePrintString(u32 battlerId);
void PlayerHandlePrintSelectionString(u32 battlerId);
void PlayerHandleChooseAction(u32 battlerId);
void PlayerHandleChooseMove(u32 battlerId);
void PlayerHandleChooseItem(u32 battlerId);
void PlayerHandleHealthbarUpdate(u32 battlerId);
void PlayerHandleLinkStandbyMsg(u32 battlerId);
void PlayerHandleEndLinkBattle(u32 battlerId);
void ActionSelectionCreateCursorAt(u32 cursorPos);
void ActionSelectionDestroyCursorAt(u32 cursorPos);
void HandleInputChooseMove(u32 battlerId);
void InitMoveSelectionsVarsAndStrings(u32 battlerId);
void MoveSelectionCreateCursorAt(u32 cursorPos, u32 arg1);
void MoveSelectionDestroyCursorAt(u32 cursorPos);

// opponent controller
void SetControllerToOpponent(u32 battlerId);
void OpponentHandleTrainerSlideBack(u32 battlerId);
void OpponentHandleHealthbarUpdate(u32 battlerId);

// safari controller
void SetControllerToSafari(u32 battlerId);
void SafariHandleBallThrowAnim(u32 battlerId);
void SafariHandleFaintingCry(u32 battlerId);

// pokedude controller
void SetControllerToPokedude(u32 battlerId);
void InitPokedudePartyAndOpponent(void);
void Pokedude_SimulateInputChooseAction(u32 battlerId);

// oak and old man controller
void SetControllerToOakOrOldMan(u32 battlerId);
void OakOldManHandlePlaySE(u32 battlerId);
void OakOldMan_SimulateInputChooseAction(u32 battlerId);
void OakOldManHandleDrawPartyStatusSummary(u32 battlerId);
void OakOldManHandleEndLinkBattle(u32 battlerId);
void OakOldManHandleInputChooseMove(u32 battlerId);
bool32 BtlCtrl_OakOldMan_TestState2Flag(u32 mask);
void BtlCtrl_OakOldMan_SetState2Flag(u32 mask);
void PrintOakText_OakNoRunningFromATrainer(u32 battlerId);
void PrintOakText_HowDisappointing(u32 battlerId);
void PrintOakText_KeepAnEyeOnHP(u32 battlerId);
void PrintOakText_InflictingDamageIsKey(u32 battlerId);
void BtlCtrl_DrawVoiceoverMessageFrame(void);
void BtlCtrl_RemoveVoiceoverMessageFrame(void);

// link partner controller
void SetControllerToLinkPartner(u32 battlerId);

// link opponent controller
void SetControllerToLinkOpponent(u32 battlerId);

#endif // GUARD_BATTLE_CONTROLLERS_H

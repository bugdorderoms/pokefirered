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

static void OpponentBufferRunCommand(u32 battlerId);
static void OpponentBufferExecCompleted(u32 battlerId);
static void OpponentHandleSwitchInAnim(u32 battlerId);
static void OpponentHandleDrawTrainerPic(u32 battlerId);
static void OpponentHandleTrainerSlide(u32 battlerId);
static void OpponentHandlePrintString(u32 battlerId);
static void OpponentHandleChooseAction(u32 battlerId);
static void OpponentHandleChooseItem(u32 battlerId);
static void OpponentHandleIntroTrainerBallThrow(u32 battlerId);
static void OpponentHandleEndLinkBattle(u32 battlerId);

static void (*const sOpponentBufferCommands[CONTROLLER_CMDS_COUNT])(u32) =
{
    [CONTROLLER_GETMONDATA]               = BtlController_HandleGetMonData,
    [CONTROLLER_SETMONDATA]               = BtlController_HandleSetMonData,
    [CONTROLLER_SETRAWMONDATA]            = BtlController_HandleSetRawMonData,
    [CONTROLLER_LOADMONSPRITE]            = BtlController_HandleLoadMonSprite,
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

void SetControllerToOpponent(u32 battlerId)
{
    gBattlerControllersData[battlerId].func = OpponentBufferRunCommand;
    gBattlerControllersData[battlerId].endFunc = OpponentBufferExecCompleted;
}

static void OpponentBufferRunCommand(u32 battlerId)
{
    if (gBattleControllerExecFlags & Bit(battlerId))
    {
        if (gBattleBufferA[battlerId][0] < ARRAY_COUNT(sOpponentBufferCommands))
            sOpponentBufferCommands[gBattleBufferA[battlerId][0]](battlerId);
        else
            OpponentBufferExecCompleted(battlerId);
    }
}

static void OpponentBufferExecCompleted(u32 battlerId)
{
    gBattlerControllersData[battlerId].func = OpponentBufferRunCommand;
    gBattleControllerExecFlags &= ~(Bit(battlerId));
}

////////////////////////
// BATTLE CONTROLLERS //
////////////////////////

static void OpponentHandleSwitchInAnim(u32 battlerId)
{
    gBattleStruct->battlers[battlerId].monToSwitchIntoId = PARTY_SIZE;
    BtlController_HandleSwitchInAnim(battlerId, SwitchIn_TryShinyAnim);
}

static inline u32 GetOpponentTrainerPicId(void)
{
    return gTrainers[gTrainerBattleOpponent_A].trainerPic;
}

static void OpponentHandleDrawTrainerPic(u32 battlerId)
{
    BtlController_HandleDrawTrainerPic(battlerId, GetOpponentTrainerPicId());
}

static void OpponentHandleTrainerSlide(u32 battlerId)
{
    BtlController_HandleTrainerSlide(battlerId, GetOpponentTrainerPicId());
}

void OpponentHandleTrainerSlideBack(u32 battlerId)
{
    BtlController_HandleTrainerSlideBack(battlerId, 35, FALSE);
}

static void OpponentHandlePrintString(u32 battlerId)
{
    u16 *stringId = (u16 *)(&gBattleBufferA[battlerId][2]);
    
    BtlController_HandlePrintStringInternal(battlerId, *stringId, FALSE);

    if (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE)
    {
        switch (*stringId)
        {
        case STRINGID_TRAINER1WINTEXT:
            gBattlerControllersData[battlerId].func = PrintOakText_HowDisappointing;
            break;
        case STRINGID_DONTLEAVEBIRCH:
            gBattlerControllersData[battlerId].func = PrintOakText_OakNoRunningFromATrainer;
            break;
        }
    }
}

static void OpponentHandleChooseAction(u32 battlerId)
{
    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        BattleAI_ChooseAction(battlerId);
    else
        BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_USE_MOVE, (BATTLE_OPPOSITE(battlerId) << 8));
    
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

static bool32 IsAllySpeciesPrey(u32 speciesAttacker, u32 speciesAttackerPartner)
{
    u32 i;
    
    for (i = 0; i < ARRAY_COUNT(sEnemyPreySpecies); i++)
    {
        if (speciesAttacker == sEnemyPreySpecies[i][0] && speciesAttackerPartner == sEnemyPreySpecies[i][1])
            return TRUE;
    }
    return FALSE;
}

static bool32 IsAllySpeciesNaturalEnemy(u32 speciesAttacker, u32 speciesAttackerPartner)
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

void OpponentHandleChooseMove(u32 battlerId)
{
    u32 moveTarget;
    struct ChooseMoveStruct *moveInfo = (struct ChooseMoveStruct *)(&gBattleBufferA[battlerId][4]);
    
    if ((gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_FIRST_BATTLE | BATTLE_TYPE_SAFARI | BATTLE_TYPE_ROAMER))
    || ((gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER) && GetBattlerSide(battlerId) == B_SIDE_PLAYER)) // Trainer
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
        default:
            gBattlerTarget = gBattleStruct->battlers[battlerId].aiChosenTarget;
            
            moveTarget = GetBattlerMoveTargetType(battlerId, moveInfo->moves[gBattleStruct->battlers[battlerId].aiMoveOrAction]);
            
            if (moveTarget == MOVE_TARGET_USER || moveTarget == MOVE_TARGET_ALL_BATTLERS)
                gBattlerTarget = battlerId;
            else if (moveTarget == MOVE_TARGET_BOTH)
            {
                gBattlerTarget = BATTLE_OPPOSITE(battlerId);
                if (gAbsentBattlerFlags & Bit(gBattlerTarget))
                    gBattlerTarget = BATTLE_PARTNER(gBattlerTarget);
            }
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, (gBattleStruct->battlers[battlerId].aiMoveOrAction) | (gBattlerTarget << 8));
            break;
        }
    }
    else // Wild
    {
        u32 target, chosenMoveId;
        u32 move, speciesAttacker, speciesAttackerPartner;

        do
        {
            chosenMoveId = Random() % MAX_MON_MOVES;
            move = moveInfo->moves[chosenMoveId];
        }
        while (!move);
        
        moveTarget = GetBattlerMoveTargetType(battlerId, move);
        
        if (moveTarget == MOVE_TARGET_USER || moveTarget == MOVE_TARGET_ALL_BATTLERS)
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, B_ACTION_EXEC_SCRIPT, (chosenMoveId) | (battlerId << 8));
        else if (IsDoubleBattleForBattler(BATTLE_OPPOSITE(battlerId)))
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

static void OpponentHandleChooseItem(u32 battlerId)
{
    BtlController_EmitOneReturnValue(battlerId, BUFFER_B, gBattleStruct->battlers[battlerId].chosenItem);
    BattleControllerComplete(battlerId);
}

void OpponentHandleChoosePokemon(u32 battlerId)
{
    u32 chosenMonId;

    if (gBattleStruct->battlers[battlerId].AI_monToSwitchIntoId == PARTY_SIZE)
    {
        chosenMonId = GetMostSuitableMonToSwitchInto(battlerId);
        
        if (chosenMonId == PARTY_SIZE)
        {
            u32 battler1, battler2;
            u32 side = GetBattlerSide(battlerId);
            struct Pokemon *party;
            
            if (side == B_SIDE_PLAYER)
                battler1 = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
            else
                battler1 = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
            
            if (!IsDoubleBattleForBattler(battlerId))
                battler2 = battler1;
            else
                battler2 = BATTLE_PARTNER(battler1);
            
            party = GetSideParty(side);
            
            for (chosenMonId = 0; chosenMonId < PARTY_SIZE; ++chosenMonId)
            {
                if (MonCanBattle(&party[chosenMonId]) && chosenMonId != gBattlerPartyIndexes[battler1] && chosenMonId != gBattlerPartyIndexes[battler2])
                    break;
            }
        }
    }
    else
    {
        chosenMonId = gBattleStruct->battlers[battlerId].AI_monToSwitchIntoId;
        gBattleStruct->battlers[battlerId].AI_monToSwitchIntoId = PARTY_SIZE;
    }
    gBattleStruct->battlers[battlerId].monToSwitchIntoId = chosenMonId;
    BtlController_EmitChosenMonReturnValue(battlerId, BUFFER_B, chosenMonId, NULL);
    BattleControllerComplete(battlerId);
}

static void OpponentHandleIntroTrainerBallThrow(u32 battlerId)
{
    BtlController_HandleIntroTrainerBallThrow(battlerId, 0x0000, 0, StartAnimLinearTranslation, 0, Intro_TryShinyAnimShowHealthbox);
}

static void OpponentHandleEndLinkBattle(u32 battlerId)
{
    if ((gBattleTypeFlags & BATTLE_TYPE_LINK) && !(gBattleTypeFlags & BATTLE_TYPE_IS_MASTER))
    {
        gMain.inBattle = FALSE;
        gMain.callback1 = gPreBattleCallback1;
        SetMainCallback2(gMain.savedCallback);
    }
    BattleControllerComplete(battlerId);
}

#include "global.h"
#include "gflib.h"
#include "battle_controllers.h"
#include "battle.h"
#include "battle_interface.h"
#include "battle_gfx_sfx_util.h"
#include "battle_gimmicks.h"
#include "battle_main.h"
#include "battle_ai.h"
#include "battle_anim.h"
#include "battle_util.h"
#include "battle_message.h"
#include "battle_move_effects.h"
#include "cable_club.h"
#include "data.h"
#include "form_change.h"
#include "item.h"
#include "link.h"
#include "link_rfu.h"
#include "m4a.h"
#include "pokemon_special_anim.h"
#include "party_menu.h"
#include "palette.h"
#include "pokeball.h"
#include "recorded_battle.h"
#include "task.h"
#include "test_runner.h"
#include "util.h"
#include "constants/battle.h"
#include "constants/battle_string_ids.h"
#include "constants/songs.h"
#include "constants/sound.h"

enum
{
    LINK_BUFF_BUFFER_ID,
    LINK_BUFF_ACTIVE_BATTLER,
    LINK_BUFF_ATTACKER,
    LINK_BUFF_TARGET,
    LINK_BUFF_SIZE_LO,
    LINK_BUFF_SIZE_HI,
    LINK_BUFF_ABSENT_BATTLER_FLAGS,
    LINK_BUFF_EFFECT_BATTLER,
    LINK_BUFF_DATA
};

#define tExpTask_monId     data[0]
#define tExpTask_gainedExp data[1]
#define tExpTask_battler   data[2]
#define tExpTask_frames    data[3]
#define tExpTask_StateId   data[4]
#define tExpTask_BgX       data[5]
#define tExpTask_BgY       data[6]

#define tSendOutTask_Battler        data[0]
#define tSendOutTask_Frames         data[1]
#define tSendOutTask_Delay          data[2]
#define tSendOutTask_ControllerFunc 3

///////////////////////
// GENERAL FUNCTIONS //
///////////////////////

static void CreateTasksForSendRecvLinkBuffers(void);
static void Task_HandleSendLinkBuffersData(u32 taskId);
static void Task_HandleCopyReceivedLinkBuffersData(u32 taskId);
static void SetBattlePartyIds(void);
static void InitLinkBtlControllers(void);
static u32 GetBattlerMonData(u32 battlerId, struct Pokemon *party, u32 monId, u8 *dst);
static void SetBattlerMonData(u32 battlerId, struct Pokemon *party, u32 monId);
static void TryShinyAnimAfterMonAnim(u32 battlerId);
static void DoSwitchOutAnimation(u32 battlerId);
static void FreeMonSprite(u32 battlerId);
static void CompleteOnBattlerSpriteCallbackDummy(u32 battlerId);
static void FreeTrainerSpriteAfterSlide(u32 battlerId);
static void FreeMonSpriteAfterFaintAnim(u32 battlerId);
static void HideHealthboxAfterMonFaint(u32 battlerId);
static void CompleteOnSpecialAnimDone(u32 battlerId);
static void DoMoveAnimation(u32 battlerId);
static void CompleteOnInactiveTextPrinter(u32 battlerId);
static void CompleteOnHealthbarDone(u32 battlerId);
static void Task_GiveExpToMon(u32 taskId);
static void Task_PrepareToGiveExpWithExpBar(u32 taskId);
static void Task_GiveExpWithExpBar(u32 taskId);
static void Task_LaunchLvlUpAnim(u32 taskId);
static void Task_UpdateLvlInHealthbox(u32 taskId);
static void Task_TryCreateVerticalStripes(u32 taskId);
static void Task_CreateLevelUpVerticalStripes(u32 taskId);
static void CompleteOnFinishedStatusAnimation(u32 battlerId);
static void SpriteCB_FreePlayerSpriteLoadMonSprite(struct Sprite *sprite);
static void SpriteCB_FreeOpponentSprite(struct Sprite *sprite);
static void Task_StartSendOutAnim(u32 taskId);
static void EndDrawPartyStatusSummary(u32 battlerId);
static void CompleteOnFinishedBattleAnimation(u32 battlerId);
static void StartSendOutAnim(u32 battlerId, u32 flags);
static void CalculateMonStatsAfterChangeForm(struct Pokemon *mon);
static void CreateYesNoBoxOnInactiveTextPrinter(u32 battlerId);
static void CreateYesNoBox(u32 battlerId);
static void HandleYesNoBoxInput(u32 battlerId);

static EWRAM_DATA u8 sLinkSendTaskId = 0;
static EWRAM_DATA u8 sLinkReceiveTaskId = 0;
static EWRAM_DATA u8 sBattleBuffersTransferData[0x100] = {0};

void HandleLinkBattleSetup(void)
{
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        if (gWirelessCommType)
            SetWirelessCommType1();
        
        if (!gReceivedRemoteLinkPlayers)
            OpenLink();
        
        CreateTask(Task_WaitForReceivedRemoteLinkPlayers5SecondTimeout, 0);
        CreateTasksForSendRecvLinkBuffers();
    }
}

static void CreateTasksForSendRecvLinkBuffers(void)
{
    sLinkSendTaskId = CreateTask(Task_HandleSendLinkBuffersData, 0);
    gTasks[sLinkSendTaskId].data[11] = 0;
    gTasks[sLinkSendTaskId].data[12] = 0;
    gTasks[sLinkSendTaskId].data[13] = 0;
    gTasks[sLinkSendTaskId].data[14] = 0;
    gTasks[sLinkSendTaskId].data[15] = 0;
    
    sLinkReceiveTaskId = CreateTask(Task_HandleCopyReceivedLinkBuffersData, 0);
    gTasks[sLinkReceiveTaskId].data[12] = 0;
    gTasks[sLinkReceiveTaskId].data[13] = 0;
    gTasks[sLinkReceiveTaskId].data[14] = 0;
    gTasks[sLinkReceiveTaskId].data[15] = 0;
}

static void Task_HandleSendLinkBuffersData(u32 taskId)
{
    u16 blockSize;
    s32 playerCount;
    bool32 advance;

    switch (gTasks[taskId].data[11])
    {
    case 0:
        gTasks[taskId].data[10] = 100;
        ++gTasks[taskId].data[11];
        break;
    case 1:
        if (--gTasks[taskId].data[10] == 0)
        {
            ++gTasks[taskId].data[11];
            
            if (gReceivedRemoteLinkPlayers)
                gTasks[taskId].data[11] = 3;
        }
        break;
    case 2:
        advance = FALSE;
        playerCount = GetLinkPlayerCount_2();
        
        if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
        {
            if (playerCount > 1)
                advance = TRUE;
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_RAID)
            {
                if (playerCount > 1)
                    advance = TRUE;
            }
            else
            {
                if (playerCount > 3)
                    advance = TRUE;
            }
        }
        
        if (advance)
        {
            if (IsLinkMaster())
                CheckShouldAdvanceLinkState();
            
            ++gTasks[taskId].data[11];
        }
        break;
    case 3:
        if (gTasks[taskId].data[15] != gTasks[taskId].data[14])
        {
            if (!gTasks[taskId].data[13])
            {
                if (gTasks[taskId].data[15] > gTasks[taskId].data[14] && gTasks[taskId].data[15] == gTasks[taskId].data[12])
                {
                    gTasks[taskId].data[12] = 0;
                    gTasks[taskId].data[15] = 0;
                }
                blockSize = (gLinkBattleSendBuffer[gTasks[taskId].data[15] + LINK_BUFF_SIZE_LO] | (gLinkBattleSendBuffer[gTasks[taskId].data[15] + LINK_BUFF_SIZE_HI] << 8)) + LINK_BUFF_DATA;
                SendBlock(&gLinkBattleSendBuffer[gTasks[taskId].data[15]], blockSize);
                ++gTasks[taskId].data[11];
            }
            else
                --gTasks[taskId].data[13];
        }
        break;
    case 4:
        if (IsLinkTaskFinished())
        {
            blockSize = gLinkBattleSendBuffer[gTasks[taskId].data[15] + LINK_BUFF_SIZE_LO] | (gLinkBattleSendBuffer[gTasks[taskId].data[15] + LINK_BUFF_SIZE_HI] << 8);
            gTasks[taskId].data[13] = 5;
            gTasks[taskId].data[15] = gTasks[taskId].data[15] + blockSize + LINK_BUFF_DATA;
            gTasks[taskId].data[11] = 3;
        }
        break;
    case 5:
        if (--gTasks[taskId].data[13] == 0)
        {
            gTasks[taskId].data[13] = 5;
            gTasks[taskId].data[11] = 3;
        }
        break;
    }
}

static void Task_HandleCopyReceivedLinkBuffersData(u32 taskId)
{
    u16 blockSize;
    u32 battlerId;

    if (gTasks[taskId].data[15] != gTasks[taskId].data[14])
    {
        if (gTasks[taskId].data[15] > gTasks[taskId].data[14] && gTasks[taskId].data[15] == gTasks[taskId].data[12])
        {
            gTasks[taskId].data[12] = 0;
            gTasks[taskId].data[15] = 0;
        }
        battlerId = gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_ACTIVE_BATTLER];
        blockSize = gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_SIZE_LO] | (gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_SIZE_HI] << 8);
        
        switch (gLinkBattleRecvBuffer[gTasks[taskId].data[15] + 0])
        {
        case 0:
            if (gBattleControllerExecFlags & Bit(battlerId))
                return;
            
            memcpy(gBattleBufferA[battlerId], &gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_DATA], blockSize);
            MarkBattlerReceivedLinkData(battlerId);
            
            if (!(gBattleTypeFlags & BATTLE_TYPE_IS_MASTER))
            {
                gBattlerAttacker = gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_ATTACKER];
                gBattlerTarget = gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_TARGET];
                gAbsentBattlerFlags = gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_ABSENT_BATTLER_FLAGS];
                gEffectBattler = gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_EFFECT_BATTLER];
            }
            break;
        case 1:
            memcpy(gBattleBufferB[battlerId], &gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_DATA], blockSize);
            break;
        case 2:
            gBattleControllerExecFlags &= ~(Bit(battlerId) << (gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_DATA] * 4));
            break;
        }
        gTasks[taskId].data[15] = gTasks[taskId].data[15] + blockSize + LINK_BUFF_DATA;
    }
}

void TryReceiveLinkBattleData(void)
{
    u8 *recvBuffer;
    u32 i, j;

    if (gReceivedRemoteLinkPlayers && (gBattleTypeFlags & BATTLE_TYPE_LINK_ESTABLISHED) && (gLinkPlayers[0].linkType == LINKTYPE_BATTLE))
    {
        LinkRfu_DestroyIdleTask();
        
        for (i = 0; i < GetLinkPlayerCount(); ++i)
        {
            if (GetBlockReceivedStatus() & Bit(i))
            {
                ResetBlockReceivedFlag(i);
                recvBuffer = (u8 *)gBlockRecvBuffer[i];
                {
                    u8 *dest, *src;
                    u16 r6 = gBlockRecvBuffer[i][2];

                    if (gTasks[sLinkReceiveTaskId].data[14] + 9 + r6 > 0x1000)
                    {
                        gTasks[sLinkReceiveTaskId].data[12] = gTasks[sLinkReceiveTaskId].data[14];
                        gTasks[sLinkReceiveTaskId].data[14] = 0;
                    }
                    dest = &gLinkBattleRecvBuffer[gTasks[sLinkReceiveTaskId].data[14]];
                    src = recvBuffer;
                    
                    for (j = 0; j < r6 + 8; ++j)
                        dest[j] = src[j];
                    
                    gTasks[sLinkReceiveTaskId].data[14] = gTasks[sLinkReceiveTaskId].data[14] + r6 + 8;
                }
            }
        }
    }
}

void SetUpBattleVars(void)
{
    u32 i;

    gBattleMainFunc = BattleDummy;
    
    for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
    {
        gBattlerControllersData[i].func = ControllerDummy;
        gBattlerPositions[i] = 0xFF;
        gActionSelectionCursor[i] = 0;
    }
    HandleLinkBattleSetup();
    gBattleControllerExecFlags = 0;
    ClearBattleAnimationVars();
    BattleAI_SetupAILogicData();
}

void InitBtlControllers(void)
{
    if (!(gBattleTypeFlags & BATTLE_TYPE_RECORDED))
    {
        RecordedBattle_Init(B_RECORD_MODE_RECORDING);
        RecordedBattle_SaveParties();
    }
    else
        RecordedBattle_Init(B_RECORD_MODE_PLAYBACK);
    
    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        InitLinkBtlControllers();
    else
        InitSinglePlayerBtlControllers(TRUE);
    
    if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
    {
        BufferBattlePartyCurrentOrderBySide(0, 0);
        BufferBattlePartyCurrentOrderBySide(1, 0);
        BufferBattlePartyCurrentOrderBySide(2, 1);
        BufferBattlePartyCurrentOrderBySide(3, 1);
        
        gBattlerPartyIndexes[0] = 0;
        gBattlerPartyIndexes[1] = 0;
        gBattlerPartyIndexes[2] = 3;
        gBattlerPartyIndexes[3] = 3;
    }
    SetBattlePartyIds();
    
    if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
    {
        u32 i;
        
        for (i = 0; i < gBattlersCount; ++i)
            BufferBattlePartyCurrentOrderBySide(i, 0);
    }
    gIntroSlideFlags = INTRO_SLIDE_FLAG_SLIDE_IN_STARTED;
    
    gBattleStruct->sos.totemBattlerId = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
}

static void SetLinkBattleEndCallbacks(u32 battlerId)
{
    if (!gWirelessCommType)
    {
        if (!gReceivedRemoteLinkPlayers)
        {
            m4aSongNumStop(SE_LOW_HEALTH);
            gMain.inBattle = FALSE;
            gMain.callback1 = gPreBattleCallback1;
            SetMainCallback2(CB2_InitEndLinkBattle);
            FreeAllWindowBuffers();
        }
    }
    else if (IsLinkTaskFinished())
    {
        m4aSongNumStop(SE_LOW_HEALTH);
        gMain.inBattle = FALSE;
        gMain.callback1 = gPreBattleCallback1;
        SetMainCallback2(CB2_InitEndLinkBattle);
        FreeAllWindowBuffers();
    }
}

void SetBattleEndCallbacks(u32 battlerId)
{
    if (!gPaletteFade.active)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        {
            if (!gWirelessCommType)
                SetCloseLinkCallback();
            else
                SetLinkStandbyCallback();
            
            gBattlerControllersData[battlerId].func = SetLinkBattleEndCallbacks;
        }
        else
        {
            m4aSongNumStop(SE_LOW_HEALTH);
            gMain.inBattle = FALSE;
            gMain.callback1 = gPreBattleCallback1;
            SetMainCallback2(gMain.savedCallback);
        }
    }
}

static void SetBattlePartyIds(void)
{
    u32 i, j;
    struct Pokemon *party;

    if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
    {
        for (i = 0; i < gBattlersCount; ++i)
        {
            party = GetBattlerParty(i);
            
            for (j = 0; j < PARTY_SIZE; ++j)
            {
                if (MonCanBattle(&party[j]) && (i < 2 || gBattlerPartyIndexes[i - 2] != j))
                {
                    gBattlerPartyIndexes[i] = j;
                    break;
                }
            }
        }
    }
}

static void SetBattlerBtlControllerFunc(u32 battler)
{
    void (*controllerFunc)(u32);
    
    gBattlerPositions[battler] = battler;
    
    if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
        controllerFunc = SetControllerToPokedude;
    else if (GetBattlerSide(battler) == B_SIDE_PLAYER)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
            controllerFunc = SetControllerToSafari;
        else if (gBattleTypeFlags & (BATTLE_TYPE_OLD_MAN_TUTORIAL | BATTLE_TYPE_FIRST_BATTLE))
            controllerFunc = SetControllerToOakOrOldMan;
        else if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
            controllerFunc = SetControllerToRecordedPlayer;
        else if ((gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER) && GetBattlerPosition(battler) == B_POSITION_PLAYER_RIGHT)
            controllerFunc = SetControllerToPlayerPartner;
        else
            controllerFunc = SetControllerToPlayer;
    }
    else
    {
        if ((gBattleTypeFlags & BATTLE_TYPE_RECORDED) && (gBattleTypeFlags & BATTLE_TYPE_TRAINER))
            controllerFunc = SetControllerToRecordedOpponent;
        else
            controllerFunc = SetControllerToOpponent;
    }
    gBattlerControllersData[battler].func = controllerFunc;
}

void InitSinglePlayerBtlControllers(bool32 fromBattleStart)
{
    u32 i;
    bool32 doubles = FALSE;
    
    for (i = 0; i < MAX_BATTLERS_COUNT / 2; i++)
    {
        SetBattlerBtlControllerFunc(i);
        
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
        {
            SetBattlerBtlControllerFunc(i + 2);
            
            // Set as absent if not on field
            if (!IsDoubleBattleForBattler(i))
                gAbsentBattlerFlags |= Bit(i + 2);
            
            doubles = TRUE;
        }
    }
    
    if (!doubles)
        gBattlersCount = MAX_BATTLERS_COUNT / 2;
    else
        gBattlersCount = MAX_BATTLERS_COUNT;
    
    if (fromBattleStart)
        gBattleMainFunc = BeginBattleIntro;
}

static void SetLinkBattlerBtlControllerFunc(u32 battler, bool32 isMaster)
{
    void (*controllerFunc)(u32);
    bool32 isPlayer = ((battler & BIT_SIDE) == B_SIDE_PLAYER);
    
    if (isMaster)
        gBattlerPositions[battler] = battler;
    else
    {
        u32 oppositeBattler = BATTLE_OPPOSITE(battler);
        
        gBattlerPositions[oppositeBattler] = battler;
        battler = oppositeBattler;
    }
    
    if (isPlayer)
        controllerFunc = SetControllerToPlayer;
    else
        controllerFunc = SetControllerToLinkOpponent;

    gBattlerControllersData[battler].func = controllerFunc;
}

static void SetMultiLinkBattlerBtlControllerFunc(u32 battler, u32 multiplayerId)
{
    void (*controllerFunc)(u32);
    u32 id = gLinkPlayers[battler].id;
    
    // Multi link master
    if (battler == multiplayerId)
    {
        switch (id)
        {
            case 0:
            case 3:
                gBattlerPositions[id] = B_POSITION_PLAYER_LEFT;
                gBattlerPartyIndexes[id] = 0;
                break;
            case 1:
            case 2:
                gBattlerPositions[id] = B_POSITION_PLAYER_RIGHT;
                gBattlerPartyIndexes[id] = MULTI_PARTY_SIZE;
                break;
        }
        controllerFunc = SetControllerToPlayer;
    }
    // Multi link partner
    else if ((!(id & 1) && !(gLinkPlayers[multiplayerId].id & 1)) || ((id & 1) && (gLinkPlayers[multiplayerId].id & 1)))
    {
        switch (id)
        {
            case 0:
            case 3:
                gBattlerPositions[id] = B_POSITION_PLAYER_LEFT;
                gBattlerPartyIndexes[id] = 0;
                break;
            case 1:
            case 2:
                gBattlerPositions[id] = B_POSITION_PLAYER_RIGHT;
                gBattlerPartyIndexes[id] = MULTI_PARTY_SIZE;
                break;
        }
        controllerFunc = SetControllerToLinkPartner;
    }
    else
    {
        switch (id)
        {
            case 0:
            case 3:
                gBattlerPositions[id] = B_POSITION_OPPONENT_LEFT;
                gBattlerPartyIndexes[id] = 0;
                break;
            case 1:
            case 2:
                gBattlerPositions[id] = B_POSITION_OPPONENT_RIGHT;
                gBattlerPartyIndexes[id] = MULTI_PARTY_SIZE;
                break;
        }
        controllerFunc = SetControllerToLinkOpponent;
    }
    gBattlerControllersData[id].func = controllerFunc;
}

static void SetMultiLinkRaidBattlerBtlControllerFunc(u32 battler, bool32 isMaster, bool32 isLinkPlayer)
{
    void (*controllerFunc)(u32);
    u32 id;
    
    if (isLinkPlayer)
    {
        id = gLinkPlayers[battler].id;
        
        // Multi link master
        if (isMaster)
        {
            switch (id)
            {
                case 0:
                case 3:
                    gBattlerPositions[id] = B_POSITION_PLAYER_LEFT;
                    gBattlerPartyIndexes[id] = 0;
                    break;
                case 1:
                case 2:
                    gBattlerPositions[id] = B_POSITION_PLAYER_RIGHT;
                    gBattlerPartyIndexes[id] = MULTI_PARTY_SIZE;
                    break;
            }
            controllerFunc = SetControllerToPlayer;
        }
        else // Multi link partner
        {
            switch (id)
            {
                case 0:
                case 3:
                    gBattlerPositions[id] = B_POSITION_PLAYER_LEFT;
                    gBattlerPartyIndexes[id] = 0;
                    break;
                case 1:
                case 2:
                    gBattlerPositions[id] = B_POSITION_PLAYER_RIGHT;
                    gBattlerPartyIndexes[id] = MULTI_PARTY_SIZE;
                    break;
            }
            controllerFunc = SetControllerToLinkPartner;
        }
    }
    else
    {
        id = battler;
        gBattlerPositions[id] = id;
        gBattlerPartyIndexes[id] = isMaster ? 0 : 1;
        controllerFunc = SetControllerToOpponent;
    }
    gBattlerControllersData[id].func = controllerFunc;
}

static void InitLinkBtlControllers(void)
{
    u32 i;
    bool32 doubles = FALSE, isMaster = (gBattleTypeFlags & BATTLE_TYPE_IS_MASTER);
    
    if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
    {
        for (i = 0; i < MAX_BATTLERS_COUNT / 2; i++)
        {
            SetLinkBattlerBtlControllerFunc(i, isMaster);
            
            if (IsDoubleBattleForBattler(i))
            {
                SetLinkBattlerBtlControllerFunc(i + 2, isMaster);
                doubles = TRUE;
            }
        }
    }
    else
    {
        u32 multiplayerId = GetMultiplayerId();
        
        if (gBattleTypeFlags & BATTLE_TYPE_RAID)
        {
            for (i = 0; i < MAX_BATTLERS_COUNT / 2; i++)
            {
                BufferBattlePartyCurrentOrderBySide(gLinkPlayers[i].id, i);
                SetMultiLinkRaidBattlerBtlControllerFunc(i, (i == multiplayerId), TRUE);
            }
            
            for (i = 1; i < MAX_BATTLERS_COUNT / 2; i++)
            {
                SetMultiLinkRaidBattlerBtlControllerFunc(i, TRUE, FALSE);
                SetMultiLinkRaidBattlerBtlControllerFunc(i + 2, FALSE, FALSE);
                
                // Set as absent if not on field
                if (!IsDoubleBattleForBattler(i))
                    gAbsentBattlerFlags |= Bit(i + 2);
            }
        }
        else
        {
            for (i = 0; i < MAX_BATTLERS_COUNT; i++)
            {
                switch (gLinkPlayers[i].id)
                {
                    case 0:
                    case 3:
                        BufferBattlePartyCurrentOrderBySide(gLinkPlayers[i].id, 0);
                        break;
                    case 1:
                    case 2:
                        BufferBattlePartyCurrentOrderBySide(gLinkPlayers[i].id, 1);
                        break;
                }
                SetMultiLinkBattlerBtlControllerFunc(i, multiplayerId);
            }
        }
        doubles = TRUE;
    }
    
    if (!doubles)
        gBattlersCount = MAX_BATTLERS_COUNT / 2;
    else
        gBattlersCount = MAX_BATTLERS_COUNT;

    if (isMaster)
        gBattleMainFunc = BeginBattleIntro;
}

/////////////////////////
// CONTROLLER EMITTERS //
/////////////////////////

void PrepareBufferDataTransferLink(u32 battlerId, u32 bufferId, u16 size, u8 *data)
{
    s32 i, alignedSize = size - size % 4 + 4;

    if (gTasks[sLinkSendTaskId].data[14] + alignedSize + LINK_BUFF_DATA + 1 > BATTLE_BUFFER_LINK_SIZE)
    {
        gTasks[sLinkSendTaskId].data[12] = gTasks[sLinkSendTaskId].data[14];
        gTasks[sLinkSendTaskId].data[14] = 0;
    }
    gLinkBattleSendBuffer[gTasks[sLinkSendTaskId].data[14] + LINK_BUFF_BUFFER_ID] = bufferId;
    gLinkBattleSendBuffer[gTasks[sLinkSendTaskId].data[14] + LINK_BUFF_ACTIVE_BATTLER] = battlerId;
    gLinkBattleSendBuffer[gTasks[sLinkSendTaskId].data[14] + LINK_BUFF_ATTACKER] = gBattlerAttacker;
    gLinkBattleSendBuffer[gTasks[sLinkSendTaskId].data[14] + LINK_BUFF_TARGET] = gBattlerTarget;
    gLinkBattleSendBuffer[gTasks[sLinkSendTaskId].data[14] + LINK_BUFF_SIZE_LO] = alignedSize;
    gLinkBattleSendBuffer[gTasks[sLinkSendTaskId].data[14] + LINK_BUFF_SIZE_HI] = (alignedSize & 0x0000FF00) >> 8;
    gLinkBattleSendBuffer[gTasks[sLinkSendTaskId].data[14] + LINK_BUFF_ABSENT_BATTLER_FLAGS] = gAbsentBattlerFlags;
    gLinkBattleSendBuffer[gTasks[sLinkSendTaskId].data[14] + LINK_BUFF_EFFECT_BATTLER] = gEffectBattler;
    
    for (i = 0; i < size; ++i)
        gLinkBattleSendBuffer[gTasks[sLinkSendTaskId].data[14] + LINK_BUFF_DATA + i] = data[i];
    
    gTasks[sLinkSendTaskId].data[14] = gTasks[sLinkSendTaskId].data[14] + alignedSize + LINK_BUFF_DATA;
}

static void PrepareBufferDataTransfer(u32 battlerId, u32 bufferId, u8 *data, u16 size)
{
    u32 i;

    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        PrepareBufferDataTransferLink(battlerId, bufferId, size, data);
    else
    {
        switch (bufferId)
        {
        case BUFFER_A:
            for (i = 0; i < size; ++data, ++i)
                gBattleBufferA[battlerId][i] = *data;
            break;
        case BUFFER_B:
            for (i = 0; i < size; ++data, ++i)
                gBattleBufferB[battlerId][i] = *data;
            break;
        }
    }
}

void BtlController_EmitGetMonData(u32 battlerId, u32 bufferId, u32 requestId, u32 monToCheck)
{
    sBattleBuffersTransferData[0] = CONTROLLER_GETMONDATA;
    sBattleBuffersTransferData[1] = requestId;
    sBattleBuffersTransferData[2] = monToCheck;
    sBattleBuffersTransferData[3] = 0;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitSetMonData(u32 battlerId, u32 bufferId, u32 requestId, u32 monToCheck, u32 bytes, void *data)
{
    u32 i;
    sBattleBuffersTransferData[0] = CONTROLLER_SETMONDATA;
    sBattleBuffersTransferData[1] = requestId;
    sBattleBuffersTransferData[2] = monToCheck;
    for (i = 0; i < bytes; ++i)
        sBattleBuffersTransferData[3 + i] = *(u8 *)(data++);
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 3 + bytes);
}

void BtlController_EmitLoadMonSprite(u32 battlerId, u32 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_LOADMONSPRITE;
    sBattleBuffersTransferData[1] = CONTROLLER_LOADMONSPRITE;
    sBattleBuffersTransferData[2] = CONTROLLER_LOADMONSPRITE;
    sBattleBuffersTransferData[3] = CONTROLLER_LOADMONSPRITE;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitSwitchInAnim(u32 battlerId, u32 bufferId, u32 partyId, u32 flags)
{
    sBattleBuffersTransferData[0] = CONTROLLER_SWITCHINANIM;
    sBattleBuffersTransferData[1] = partyId;
    sBattleBuffersTransferData[2] = flags;
    sBattleBuffersTransferData[3] = 0;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitReturnMonToBall(u32 battlerId, u32 bufferId, u32 arg1)
{
    sBattleBuffersTransferData[0] = CONTROLLER_RETURNMONTOBALL;
    sBattleBuffersTransferData[1] = arg1;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 2);
}

void BtlController_EmitDrawTrainerPic(u32 battlerId, u32 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_DRAWTRAINERPIC;
    sBattleBuffersTransferData[1] = CONTROLLER_DRAWTRAINERPIC;
    sBattleBuffersTransferData[2] = CONTROLLER_DRAWTRAINERPIC;
    sBattleBuffersTransferData[3] = CONTROLLER_DRAWTRAINERPIC;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitTrainerSlide(u32 battlerId, u32 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_TRAINERSLIDE;
    sBattleBuffersTransferData[1] = CONTROLLER_TRAINERSLIDE;
    sBattleBuffersTransferData[2] = CONTROLLER_TRAINERSLIDE;
    sBattleBuffersTransferData[3] = CONTROLLER_TRAINERSLIDE;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitTrainerSlideBack(u32 battlerId, u32 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_TRAINERSLIDEBACK;
    sBattleBuffersTransferData[1] = CONTROLLER_TRAINERSLIDEBACK;
    sBattleBuffersTransferData[2] = CONTROLLER_TRAINERSLIDEBACK;
    sBattleBuffersTransferData[3] = CONTROLLER_TRAINERSLIDEBACK;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitFaintAnimation(u32 battlerId, u32 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_FAINTANIMATION;
    sBattleBuffersTransferData[1] = CONTROLLER_FAINTANIMATION;
    sBattleBuffersTransferData[2] = CONTROLLER_FAINTANIMATION;
    sBattleBuffersTransferData[3] = CONTROLLER_FAINTANIMATION;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitBallThrowAnim(u32 battlerId, u32 bufferId, u32 caseId, bool32 isCriticalCapture, bool32 criticalCaptureSuccess)
{
    sBattleBuffersTransferData[0] = CONTROLLER_BALLTHROWANIM;
    sBattleBuffersTransferData[1] = caseId;
    sBattleBuffersTransferData[2] = GetCatchingBattler();
    sBattleBuffersTransferData[3] = gLastUsedItem;
    sBattleBuffersTransferData[4] = (gLastUsedItem & 0xFF00) >> 8;
    sBattleBuffersTransferData[5] = isCriticalCapture;
    sBattleBuffersTransferData[6] = criticalCaptureSuccess;
    sBattleBuffersTransferData[7] = 0;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 8);
}

void BtlController_EmitMoveAnimation(u32 battlerId, u32 bufferId, u32 move, u32 turnOfMove, u32 movePower, s32 dmg, u32 friendship, struct DisableStruct *disableStructPtr)
{
    sBattleBuffersTransferData[0] = CONTROLLER_MOVEANIMATION;
    sBattleBuffersTransferData[1] = move;
    sBattleBuffersTransferData[2] = (move & 0xFF00) >> 8;
    sBattleBuffersTransferData[3] = turnOfMove;
    sBattleBuffersTransferData[4] = movePower;
    sBattleBuffersTransferData[5] = (movePower & 0xFF00) >> 8;
    sBattleBuffersTransferData[6] = dmg;
    sBattleBuffersTransferData[7] = (dmg & 0x0000FF00) >> 8;
    sBattleBuffersTransferData[8] = (dmg & 0x00FF0000) >> 16;
    sBattleBuffersTransferData[9] = (dmg & 0xFF000000) >> 24;
    sBattleBuffersTransferData[10] = friendship;
    sBattleBuffersTransferData[11] = gMultiHitCounter;
    if (IsBattlerWeatherAffected(battlerId, B_WEATHER_ANY))
    {
        sBattleBuffersTransferData[12] = gBattleWeather;
        sBattleBuffersTransferData[13] = (gBattleWeather & 0xFF00) >> 8;
    }
    else
    {
        sBattleBuffersTransferData[12] = 0;
        sBattleBuffersTransferData[13] = 0;
    }
    sBattleBuffersTransferData[14] = gLastUsedItem;
    sBattleBuffersTransferData[15] = (gLastUsedItem & 0xFF00) >> 8;
    memcpy(&sBattleBuffersTransferData[16], disableStructPtr, sizeof(struct DisableStruct));
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 16 + sizeof(struct DisableStruct));
}

static void InitStringInfoTransferBuffer(void)
{
    u32 i;
    struct BattleMsgData *stringInfo = (struct BattleMsgData *)(&sBattleBuffersTransferData[4]);

    stringInfo->currentMove = gCurrentMove;
    stringInfo->chosenMove = gChosenMove;
    stringInfo->lastItem = gLastUsedItem;
    stringInfo->scrActive = gBattleScripting.battler;
    stringInfo->hpScale = gBattleStruct->hpScale;
    
    for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
        stringInfo->abilities[i] = gBattleMons[i].ability;
    
    for (i = 0; i < TEXT_BUFF_ARRAY_COUNT; ++i)
    {
        stringInfo->textBuffs[0][i] = gBattleTextBuff1[i];
        stringInfo->textBuffs[1][i] = gBattleTextBuff2[i];
        stringInfo->textBuffs[2][i] = gBattleTextBuff3[i];
    }
}

void BtlController_EmitPrintString(u32 battlerId, u32 bufferId, u32 stringId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_PRINTSTRING;
    sBattleBuffersTransferData[1] = gBattleOutcome;
    sBattleBuffersTransferData[2] = stringId;
    sBattleBuffersTransferData[3] = (stringId & 0xFF00) >> 8;
    InitStringInfoTransferBuffer();

    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, sizeof(struct BattleMsgData) + 4);
}

void BtlController_EmitPrintSelectionString(u32 battlerId, u32 bufferId, u32 stringId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_PRINTSELECTIONSTRING;
    sBattleBuffersTransferData[1] = CONTROLLER_PRINTSELECTIONSTRING;
    sBattleBuffersTransferData[2] = stringId;
    sBattleBuffersTransferData[3] = (stringId & 0xFF00) >> 8;
    InitStringInfoTransferBuffer();

    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, sizeof(struct BattleMsgData) + 4);
}

void BtlController_EmitChooseAction(u32 battlerId, u32 bufferId, u32 arg1, u32 arg2)
{
    sBattleBuffersTransferData[0] = CONTROLLER_CHOOSEACTION;
    sBattleBuffersTransferData[1] = arg1;
    sBattleBuffersTransferData[2] = arg2;
    sBattleBuffersTransferData[3] = (arg2 & 0xFF00) >> 8;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitChooseMove(u32 battlerId, u32 bufferId, bool32 isDoubleBattle, bool32 NoPpNumber, struct ChooseMoveStruct *movePpData)
{
    u32 i;
    sBattleBuffersTransferData[0] = CONTROLLER_CHOOSEMOVE;
    sBattleBuffersTransferData[1] = isDoubleBattle;
    sBattleBuffersTransferData[2] = NoPpNumber;
    sBattleBuffersTransferData[3] = 0;
    for (i = 0; i < sizeof(*movePpData); ++i)
        sBattleBuffersTransferData[4 + i] = *((u8 *)(movePpData) + i);
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, sizeof(*movePpData) + 4);
}

void BtlController_EmitChooseItem(u32 battlerId, u32 bufferId, u8 *arg1)
{
    u32 i;
    sBattleBuffersTransferData[0] = CONTROLLER_OPENBAG;
    for (i = 0; i < 3; ++i)
        sBattleBuffersTransferData[1 + i] = arg1[i];
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitChoosePokemon(u32 battlerId, u32 bufferId, u32 caseId, u32 arg2, u8* arg4)
{
    u32 i;
    sBattleBuffersTransferData[0] = CONTROLLER_CHOOSEPOKEMON;
    sBattleBuffersTransferData[1] = caseId;
    sBattleBuffersTransferData[2] = arg2;
    for (i = 0; i < 3; ++i)
        sBattleBuffersTransferData[3 + i] = arg4[i];
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 8);
}

void BtlController_EmitHealthBarUpdate(u32 battlerId, u32 bufferId, u16 hpValue)
{
    sBattleBuffersTransferData[0] = CONTROLLER_HEALTHBARUPDATE;
    sBattleBuffersTransferData[1] = 0;
    sBattleBuffersTransferData[2] = (s16)hpValue;
    sBattleBuffersTransferData[3] = ((s16)hpValue & 0xFF00) >> 8;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitExpUpdate(u32 battlerId, u32 bufferId, u32 partyId, u16 expPoints)
{
    sBattleBuffersTransferData[0] = CONTROLLER_EXPUPDATE;
    sBattleBuffersTransferData[1] = partyId;
    sBattleBuffersTransferData[2] = (s16)expPoints;
    sBattleBuffersTransferData[3] = ((s16)expPoints & 0xFF00) >> 8;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitStatusIconUpdate(u32 battlerId, u32 bufferId, u32 status1Id, u32 status2)
{
    sBattleBuffersTransferData[0] = CONTROLLER_STATUSICONUPDATE;
    sBattleBuffersTransferData[1] = status1Id;
    sBattleBuffersTransferData[2] = status2;
    sBattleBuffersTransferData[3] = (status2 & 0x0000FF00) >> 8;
    sBattleBuffersTransferData[4] = (status2 & 0x00FF0000) >> 16;
    sBattleBuffersTransferData[5] = (status2 & 0xFF000000) >> 24;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 6);
}

void BtlController_EmitStatusAnimation(u32 battlerId, u32 bufferId, u32 animStatusId, u32 status)
{
    sBattleBuffersTransferData[0] = CONTROLLER_STATUSANIMATION;
    sBattleBuffersTransferData[1] = animStatusId;
    sBattleBuffersTransferData[2] = status;
    sBattleBuffersTransferData[3] = (status & 0x0000FF00) >> 8;
    sBattleBuffersTransferData[4] = (status & 0x00FF0000) >> 16;
    sBattleBuffersTransferData[5] = (status & 0xFF000000) >> 24;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 6);
}

void BtlController_EmitDataTransfer(u32 battlerId, u32 bufferId, u16 size, void *data)
{
    u32 i;
    sBattleBuffersTransferData[0] = CONTROLLER_DATATRANSFER;
    sBattleBuffersTransferData[1] = CONTROLLER_DATATRANSFER;
    sBattleBuffersTransferData[2] = size;
    sBattleBuffersTransferData[3] = (size & 0xFF00) >> 8;
    for (i = 0; i < size; ++i)
        sBattleBuffersTransferData[4 + i] = *(u8 *)(data++);
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, size + 4);
}

void BtlController_EmitTwoReturnValues(u32 battlerId, u32 bufferId, u32 arg1, u32 arg2)
{
    sBattleBuffersTransferData[0] = CONTROLLER_TWORETURNVALUES;
    sBattleBuffersTransferData[1] = arg1;
    sBattleBuffersTransferData[2] = arg2;
    sBattleBuffersTransferData[3] = (arg2 & 0xFF00) >> 8;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitChosenMonReturnValue(u32 battlerId, u32 bufferId, u32 b, u8 *c)
{
    u32 i;
    sBattleBuffersTransferData[0] = CONTROLLER_CHOSENMONRETURNVALUE;
    sBattleBuffersTransferData[1] = b;
    for (i = 0; i < 3; ++i)
        sBattleBuffersTransferData[2 + i] = c[i];
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 5);
}

void BtlController_EmitOneReturnValue(u32 battlerId, u32 bufferId, u32 arg1)
{
    sBattleBuffersTransferData[0] = CONTROLLER_ONERETURNVALUE;
    sBattleBuffersTransferData[1] = arg1;
    sBattleBuffersTransferData[2] = (arg1 & 0xFF00) >> 8;
    sBattleBuffersTransferData[3] = 0;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitPlaySE(u32 battlerId, u32 bufferId, u32 songId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_EFFECTIVENESSSOUND;
    sBattleBuffersTransferData[1] = songId;
    sBattleBuffersTransferData[2] = (songId & 0xFF00) >> 8;
    sBattleBuffersTransferData[3] = 0;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitPlayFanfare(u32 battlerId, u32 bufferId, u32 songId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_PLAYFANFAREORBGM;
    sBattleBuffersTransferData[1] = songId;
    sBattleBuffersTransferData[2] = (songId & 0xFF00) >> 8;
    sBattleBuffersTransferData[3] = 0;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitFaintingCry(u32 battlerId, u32 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_FAINTINGCRY;
    sBattleBuffersTransferData[1] = CONTROLLER_FAINTINGCRY;
    sBattleBuffersTransferData[2] = CONTROLLER_FAINTINGCRY;
    sBattleBuffersTransferData[3] = CONTROLLER_FAINTINGCRY;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitIntroSlide(u32 battlerId, u32 bufferId, u32 terrainId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_INTROSLIDE;
    sBattleBuffersTransferData[1] = terrainId;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 2);
}

void BtlController_EmitIntroTrainerBallThrow(u32 battlerId, u32 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_INTROTRAINERBALLTHROW;
    sBattleBuffersTransferData[1] = CONTROLLER_INTROTRAINERBALLTHROW;
    sBattleBuffersTransferData[2] = CONTROLLER_INTROTRAINERBALLTHROW;
    sBattleBuffersTransferData[3] = CONTROLLER_INTROTRAINERBALLTHROW;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitDrawPartyStatusSummary(u32 battlerId, u32 bufferId, struct HpAndStatus *hpAndStatus, u8 param)
{
    u32 i;
    sBattleBuffersTransferData[0] = CONTROLLER_DRAWPARTYSTATUSSUMMARY;
    sBattleBuffersTransferData[1] = param & ~(PARTY_SUMM_SKIP_DRAW_DELAY); // If true, skip player side
    sBattleBuffersTransferData[2] = (param & PARTY_SUMM_SKIP_DRAW_DELAY) >> 7; // If true, skip delay after drawing. True during intro
    sBattleBuffersTransferData[3] = CONTROLLER_DRAWPARTYSTATUSSUMMARY;
    for (i = 0; i < (u32)(sizeof(struct HpAndStatus) * PARTY_SIZE); ++i)
        sBattleBuffersTransferData[4 + i] = *(i + (u8 *)(hpAndStatus));
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, sizeof(struct HpAndStatus) * PARTY_SIZE + 4);
}

void BtlController_EmitHidePartyStatusSummary(u32 battlerId, u32 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_HIDEPARTYSTATUSSUMMARY;
    sBattleBuffersTransferData[1] = CONTROLLER_HIDEPARTYSTATUSSUMMARY;
    sBattleBuffersTransferData[2] = CONTROLLER_HIDEPARTYSTATUSSUMMARY;
    sBattleBuffersTransferData[3] = CONTROLLER_HIDEPARTYSTATUSSUMMARY;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitEndBounceEffect(u32 battlerId, u32 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_ENDBOUNCE;
    sBattleBuffersTransferData[1] = CONTROLLER_ENDBOUNCE;
    sBattleBuffersTransferData[2] = CONTROLLER_ENDBOUNCE;
    sBattleBuffersTransferData[3] = CONTROLLER_ENDBOUNCE;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitSpriteInvisibility(u32 battlerId, u32 bufferId, bool32 isInvisible)
{
    sBattleBuffersTransferData[0] = CONTROLLER_SPRITEINVISIBILITY;
    sBattleBuffersTransferData[1] = isInvisible;
    sBattleBuffersTransferData[2] = CONTROLLER_SPRITEINVISIBILITY;
    sBattleBuffersTransferData[3] = CONTROLLER_SPRITEINVISIBILITY;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitBattleAnimation(u32 battlerId, u32 bufferId, u32 animationId, u32 argument)
{
    sBattleBuffersTransferData[0] = CONTROLLER_BATTLEANIMATION;
    sBattleBuffersTransferData[1] = animationId;
    sBattleBuffersTransferData[2] = argument;
    sBattleBuffersTransferData[3] = (argument & 0xFF00) >> 8;
    sBattleBuffersTransferData[4] = gLastUsedItem;
    sBattleBuffersTransferData[5] = (gLastUsedItem & 0xFF00) >> 8;
    sBattleBuffersTransferData[6] = 0;
    sBattleBuffersTransferData[7] = 0;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 8);
}

void BtlController_EmitLinkStandbyMsg(u32 battlerId, u32 bufferId, u32 arg1)
{
    sBattleBuffersTransferData[0] = CONTROLLER_LINKSTANDBYMSG;
    sBattleBuffersTransferData[1] = arg1;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 2);
}

void BtlController_EmitResetActionMoveSelection(u32 battlerId, u32 bufferId, u32 caseId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_RESETACTIONMOVESELECTION;
    sBattleBuffersTransferData[1] = caseId;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 2);
}

void BtlController_EmitEndLinkBattle(u32 battlerId, u32 bufferId, u32 battleOutcome)
{
    sBattleBuffersTransferData[0] = CONTROLLER_ENDLINKBATTLE;
    sBattleBuffersTransferData[1] = battleOutcome;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 2);
}

void BtlController_EmitGimmickState(u32 battlerId, u32 bufferId, u32 stateId, u32 data)
{
    sBattleBuffersTransferData[0] = CONTROLLER_GIMMICKSTATE;
    sBattleBuffersTransferData[1] = stateId;
    sBattleBuffersTransferData[2] = data;
    sBattleBuffersTransferData[3] = 0;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitHealthBoxUpdate(u32 battlerId, u32 bufferId, u32 attributeId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_HEALTHBOXUPDATE;
    sBattleBuffersTransferData[1] = attributeId;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 2);
}

void BtlController_EmitHideAllHealthboxes(u32 battlerId, u32 bufferId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_HIDEALLHEALTHBOXES;
    sBattleBuffersTransferData[1] = CONTROLLER_HIDEALLHEALTHBOXES;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 2);
}

void BtlController_EmitBattleFormChange(u32 battlerId, u32 bufferId, u32 newSpecies, bool32 reloadStats)
{
    sBattleBuffersTransferData[0] = CONTROLLER_BATTLEFORMCHANGE;
    sBattleBuffersTransferData[1] = newSpecies;
    sBattleBuffersTransferData[2] = (newSpecies & 0xFF00) >> 8;
    sBattleBuffersTransferData[3] = reloadStats;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitPartyFormChange(u32 battlerId, u32 bufferId, u32 partyId, u32 caseId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_PARTYFORMCHANGE;
    sBattleBuffersTransferData[1] = partyId;
    sBattleBuffersTransferData[2] = caseId;
    sBattleBuffersTransferData[3] = 0;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitIsPocketNotEmpty(u32 battlerId, u32 bufferId, u32 pocketId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_ISPOCKETNOTEMPTY;
    sBattleBuffersTransferData[1] = pocketId;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 2);
}

void BtlController_EmitYesNoBox(u32 battlerId, u32 bufferId, u32 stringId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_YESNOBOX;
    sBattleBuffersTransferData[1] = CONTROLLER_YESNOBOX;
    sBattleBuffersTransferData[2] = stringId;
    sBattleBuffersTransferData[3] = (stringId & 0xFF00) >> 8;
    InitStringInfoTransferBuffer();

    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, sizeof(struct BattleMsgData) + 4);
}

void BtlController_EmitMonCaughtEffects(u32 battlerId, u32 bufferId, u32 capturedBattlerId, u32 pokeballId)
{
    sBattleBuffersTransferData[0] = CONTROLLER_MONCAUGHTEFFECTS;
    sBattleBuffersTransferData[1] = capturedBattlerId;
    sBattleBuffersTransferData[2] = pokeballId;
    sBattleBuffersTransferData[3] = (pokeballId & 0xFF00) >> 8;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

/////////////////////////
// GENERAL CONTROLLERS //
/////////////////////////

void ControllerDummy(u32 battlerId) { }

void BattleControllerComplete(u32 battlerId)
{
    gBattlerControllersData[battlerId].endFunc(battlerId);
}

void BtlController_HandleGetMonData(u32 battlerId)
{
    u8 monData[sizeof(struct Pokemon) * 2 + 56]; // this allows to get full data of two pokemon, trying to get more will result in overwriting data
    struct Pokemon *party = GetBattlerParty(battlerId);
    u32 i, monToCheck, size = 0;
    
    if (!gBattleBufferA[battlerId][2])
        size += GetBattlerMonData(battlerId, party, gBattlerPartyIndexes[battlerId], monData);
    else
    {
        monToCheck = gBattleBufferA[battlerId][2];
        
        for (i = 0; i < PARTY_SIZE; ++i)
        {
            if (monToCheck & 1)
                size += GetBattlerMonData(battlerId, party, i, monData + size);
            
            monToCheck >>= 1;
        }
    }
    BtlController_EmitDataTransfer(battlerId, BUFFER_B, size, monData);
    BattleControllerComplete(battlerId);
}

void BtlController_HandleSetMonData(u32 battlerId)
{
    u32 i, monToCheck;
    struct Pokemon *party = GetBattlerParty(battlerId);

    if (!gBattleBufferA[battlerId][2])
        SetBattlerMonData(battlerId, party, gBattlerPartyIndexes[battlerId]);
    else
    {
        monToCheck = gBattleBufferA[battlerId][2];
        
        for (i = 0; i < PARTY_SIZE; ++i)
        {
            if (monToCheck & 1)
                SetBattlerMonData(battlerId, party, i);
            
            monToCheck >>= 1;
        }
    }
    BattleControllerComplete(battlerId);
}

void BtlController_HandleLoadMonSprite(u32 battlerId)
{
    struct Pokemon *mon = GetBattlerPartyIndexPtr(battlerId);
    u32 y;
    bool32 isGhost = (GetBattlerSide(battlerId) == B_SIDE_OPPONENT && (gBattleTypeFlags & BATTLE_TYPE_GHOST));
    
    if (isGhost)
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
    SetMultiuseSpriteTemplateToPokemon(GetMonData(mon, MON_DATA_SPECIES), GetBattlerPosition(battlerId));
    
    gBattlerSpriteIds[battlerId] = CreateSprite(&gMultiuseSpriteTemplate, GetBattlerSpriteCoord(battlerId, BATTLER_COORD_X), y, GetBattlerSpriteSubpriority(battlerId));
    gSprites[gBattlerSpriteIds[battlerId]].x2 = -240;
    gSprites[gBattlerSpriteIds[battlerId]].data[0] = battlerId;
    gSprites[gBattlerSpriteIds[battlerId]].oam.paletteNum = battlerId;
    
    StartSpriteAnim(&gSprites[gBattlerSpriteIds[battlerId]], 0);
    
    if (!isGhost)
        SetBattlerShadowSpriteCallback(battlerId);
    
    gBattlerControllersData[battlerId].func = TryShinyAnimAfterMonAnim;
}

void BtlController_HandleSwitchInAnim(u32 battlerId, void(*controllerFunc)(u32))
{
    bool32 loadSprite = (GetBattlerSide(battlerId) == B_SIDE_PLAYER || (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE));
    
    if (loadSprite)
        ClearTemporarySpeciesSpriteData(battlerId, gBattleBufferA[battlerId][2]);
    
    gBattlerPartyIndexes[battlerId] = gBattleBufferA[battlerId][1];
    
    if (loadSprite)
        BattleLoadMonSpriteGfx(battlerId);
    
    StartSendOutAnim(battlerId, gBattleBufferA[battlerId][2]);
    gBattlerControllersData[battlerId].func = controllerFunc;
}

void BtlController_HandleReturnMonToBall(u32 battlerId)
{
    if (!gBattleBufferA[battlerId][1])
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].animationState = 0;
        gBattlerControllersData[battlerId].func = DoSwitchOutAnimation;
    }
    else
    {
        FreeMonSprite(battlerId);
        BattleControllerComplete(battlerId);
    }
}

void BtlController_HandleDrawTrainerPic(u32 battlerId, u32 trainerPicId)
{
    bool32 isFront = (GetBattlerSide(battlerId) == B_SIDE_OPPONENT);
    s16 xPos, yPos;
    struct Sprite *sprite;
    
    if (isFront)
    {
        DecompressTrainerFrontPic(trainerPicId, battlerId);
        yPos = (8 - gTrainerFrontPicTable[trainerPicId].coords.size) * 4 + 40;
        
        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
        {
            if ((GetBattlerPosition(battlerId) & BIT_FLANK) != B_FLANK_LEFT) // second mon
                xPos = 152;
            else // first mon
                xPos = 200;
        }
        else
            xPos = 176;
    }
    else
    {
        DecompressTrainerBackPalette(trainerPicId, battlerId);
        yPos = (8 - gTrainerBackPicTable[trainerPicId].coords.size) * 4 + 80;
        
        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
        {
            if ((GetBattlerPosition(battlerId) & BIT_FLANK) != B_FLANK_LEFT) // Second mon, on the right.
                xPos = 90;
            else // First mon, on the left.
                xPos = 32;
        }
        else
            xPos = 80;
    }
    SetMultiuseSpriteTemplateToTrainerBack(trainerPicId, battlerId, GetBattlerPosition(battlerId));
    gBattlerSpriteIds[battlerId] = CreateSprite(&gMultiuseSpriteTemplate, xPos, yPos, GetBattlerSpriteSubpriority(battlerId));
    sprite = &gSprites[gBattlerSpriteIds[battlerId]];
    
    sprite->x2 = isFront ? -240 : 240;
    sprite->data[0] = isFront ? 2 : -2;
    
    if (isFront)
    {
        sprite->oam.paletteNum = IndexOfSpritePaletteTag(gTrainerFrontPicTable[trainerPicId].palette.tag);
        sprite->data[5] = sprite->oam.tileNum;
        sprite->oam.tileNum = GetSpriteTileStartByTag(gTrainerFrontPicTable[trainerPicId].pic.tag);
        sprite->oam.affineParam = trainerPicId;
    }
    else
        sprite->oam.paletteNum = battlerId;
    
    sprite->callback = SpriteCB_TrainerSlideIn;
    gBattlerControllersData[battlerId].func = CompleteOnBattlerSpriteCallbackDummy;
}

void BtlController_HandleTrainerSlide(u32 battlerId, u32 trainerPicId)
{
    bool32 isFront = (GetBattlerSide(battlerId) == B_SIDE_OPPONENT);
    s16 xPos, yPos;
    struct Sprite *sprite;
    
    if (isFront)
    {
        DecompressTrainerFrontPic(trainerPicId, battlerId);
        yPos = (8 - gTrainerFrontPicTable[trainerPicId].coords.size) * 4 + 40;
        xPos = 176;
    }
    else
    {
        DecompressTrainerBackPalette(trainerPicId, battlerId);
        yPos = (8 - gTrainerBackPicTable[trainerPicId].coords.size) * 4 + 80;
        xPos = 80;
    }
    SetMultiuseSpriteTemplateToTrainerBack(trainerPicId, battlerId, GetBattlerPosition(battlerId));
    gBattlerSpriteIds[battlerId] = CreateSprite(&gMultiuseSpriteTemplate, xPos, yPos, 30);
    sprite = &gSprites[gBattlerSpriteIds[battlerId]];
    
    if (isFront)
    {
        sprite->x2 = 96;
        sprite->x += 32;
        sprite->data[0] = -2;
        sprite->oam.paletteNum = IndexOfSpritePaletteTag(gTrainerFrontPicTable[trainerPicId].palette.tag);
        sprite->data[5] = sprite->oam.tileNum;
        sprite->oam.tileNum = GetSpriteTileStartByTag(gTrainerFrontPicTable[trainerPicId].pic.tag);
        sprite->oam.affineParam = trainerPicId;
    }
    else
    {
        sprite->x2 = -96;
        sprite->data[0] = 2;
        sprite->oam.paletteNum = battlerId;
    }
    sprite->callback = SpriteCB_TrainerSlideIn;
    gBattlerControllersData[battlerId].func = CompleteOnBattlerSpriteCallbackDummy;
}

void BtlController_HandleTrainerSlideBack(u32 battlerId, s16 data0, bool32 animateSprite)
{
    struct Sprite *sprite = &gSprites[gBattlerSpriteIds[battlerId]];
    
    SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
    sprite->data[0] = data0;
    sprite->data[2] = GetBattlerSide(battlerId) == B_SIDE_PLAYER ? -40 : 280;
    sprite->data[4] = sprite->y;
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, SpriteCallbackDummy);
    
    if (animateSprite)
        StartSpriteAnim(sprite, 1);
    
    gBattlerControllersData[battlerId].func = FreeTrainerSpriteAfterSlide;
}

void BtlController_HandleFaintAnimation(u32 battlerId)
{
    switch (gBattleSpritesDataPtr->healthBoxesData[battlerId].animationState)
    {
        case 0:
            if (gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute)
                InitAndLaunchSpecialAnimation(battlerId, battlerId, B_ANIM_SUBSTITUTE_TO_MON);
            
            ++gBattleSpritesDataPtr->healthBoxesData[battlerId].animationState;
            break;
        case 1:
            if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].specialAnimActive)
            {
                gBattleSpritesDataPtr->healthBoxesData[battlerId].animationState = 0;
                
                HandleLowHpMusicChange(battlerId, GetBattlerPartyIndexPtr(battlerId));
                
                if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
                {
                    PlaySE12WithPanning(SE_FAINT, SOUND_PAN_ATTACKER);
                    gSprites[gBattlerSpriteIds[battlerId]].data[1] = 0;
                    gSprites[gBattlerSpriteIds[battlerId]].data[2] = 5;
                    gSprites[gBattlerSpriteIds[battlerId]].callback = SpriteCB_FaintSlideAnim;
                    gBattlerControllersData[battlerId].func = FreeMonSpriteAfterFaintAnim;
                }
                else
                {
                    PlaySE12WithPanning(SE_FAINT, SOUND_PAN_TARGET);
                    gSprites[gBattlerSpriteIds[battlerId]].callback = SpriteCB_FaintOpponentMon;
                    gBattlerControllersData[battlerId].func = HideHealthboxAfterMonFaint;
                }
            }
            break;
    }
}

void BtlController_HandleBallThrowAnim(u32 battlerId, u32 animId, bool32 allowCriticalCapture)
{
    gBattleSpritesDataPtr->animationData->ballThrowCaseId = gBattleBufferA[battlerId][1];
    gDoingBattleAnim = TRUE;
    gAnimLastUsedItem = (gBattleTypeFlags & BATTLE_TYPE_RAID) ? DYNAMAX_BALL : READ_16(&gBattleBufferA[battlerId][3]);

    gBattleSpritesDataPtr->animationData->isCriticalCapture = gBattleBufferA[battlerId][5];
    gBattleSpritesDataPtr->animationData->criticalCaptureSuccess = gBattleBufferA[battlerId][6];
    
    if (allowCriticalCapture && gBattleSpritesDataPtr->animationData->isCriticalCapture)
        animId = B_ANIM_CRITICAL_CAPTURE_THROW;

    InitAndLaunchSpecialAnimation(battlerId, gBattleBufferA[battlerId][2], animId);
    gBattlerControllersData[battlerId].func = CompleteOnSpecialAnimDone;
}

void BtlController_HandleMoveAnimation(u32 battlerId)
{
    if (!IsBattleSEPlaying(battlerId))
    {
        gAnimMoveTurn = gBattleBufferA[battlerId][3];
        gAnimMovePower = READ_16(&gBattleBufferA[battlerId][4]);
        gAnimMoveDmg = READ_32(&gBattleBufferA[battlerId][6]);
        gAnimFriendship = gBattleBufferA[battlerId][10];
        gWeatherMoveAnim = READ_16(&gBattleBufferA[battlerId][12]);
        gAnimLastUsedItem = READ_16(&gBattleBufferA[battlerId][14]);
        gAnimDisableStructPtr = (struct DisableStruct *)&gBattleBufferA[battlerId][16];
        
        gTransformedPersonalities[battlerId] = gAnimDisableStructPtr->transformedMonPersonality;
        gTransformedShinies[battlerId] = gAnimDisableStructPtr->transformedMonShynies;
        
        gBattleSpritesDataPtr->healthBoxesData[battlerId].animationState = 0;
        gBattlerControllersData[battlerId].func = DoMoveAnimation;
    }
}

void BtlController_HandlePrintStringInternal(u32 battlerId, u32 stringId, bool32 isSelection)
{
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
    BufferStringBattle(battlerId, stringId);
    
    if (gTestRunnerEnabled)
    {
        TestRunner_Battle_RecordMessage(gDisplayedStringBattle);
        
        if (gTestRunnerHeadless)
        {
            BattleControllerComplete(battlerId);
            return;
        }
    }
    
    if (isSelection && stringId != STRINGID_DONTLEAVEBIRCH && stringId != STRINGID_CANTESCAPE)
        StringAppend(gDisplayedStringBattle, COMPOUND_STRING("\p"));
    
    BattlePutTextOnWindow(gDisplayedStringBattle, BattleStringShouldBeColored(stringId) ? (B_WIN_MSG | B_TEXT_FLAG_NPC_CONTEXT_FONT) : B_WIN_MSG);
    gBattlerControllersData[battlerId].func = CompleteOnInactiveTextPrinter;
}

void BtlController_HandlePrintString(u32 battlerId)
{
    BtlController_HandlePrintStringInternal(battlerId, READ_16(&gBattleBufferA[battlerId][2]), FALSE);
}

void BtlController_HandlePrintSelectionString(u32 battlerId)
{
    if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
        BtlController_HandlePrintStringInternal(battlerId, READ_16(&gBattleBufferA[battlerId][2]), TRUE);
    else
        BattleControllerComplete(battlerId);
}

void BtlController_HandleChooseAction(u32 battlerId, const u8 *actionsStr, const u8 *whatDoStr, void(*controllerFunc)(u32))
{
    u32 i;

    gBattlerControllersData[battlerId].func = controllerFunc;
    
    BattlePutTextOnWindow(gText_EmptyString, B_WIN_MSG);
    BattlePutTextOnWindow(actionsStr, B_WIN_ACTION_MENU);
    
    for (i = 0; i < MAX_MON_MOVES; ++i)
        ActionSelectionDestroyCursorAt(i);
    
    ActionSelectionCreateCursorAt(gActionSelectionCursor[battlerId]);
    BattleStringExpandPlaceholdersToDisplayedString(whatDoStr);
    BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_ACTION_PROMPT);
}

void BtlController_HandleChooseItem(u32 battlerId, void(*controllerFunc)(u32))
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
    gBattlerControllersData[battlerId].func = controllerFunc;
    gBattlerInMenuId = battlerId;
}

void BtlController_HandleChoosePokemon(u32 battlerId, void(*controllerFunc)(u32))
{
    u32 i;

    gBattlerControllersData[battlerId].data = CreateTask(TaskDummy, 0xFF);
    gTasks[gBattlerControllersData[battlerId].data].data[0] = gBattleBufferA[battlerId][1] & 0xF;
    gBattleStruct->battlerPreventingSwitchout = gBattleBufferA[battlerId][1] >> 4;
    gBattleStruct->playerPartyIdx = gBattleBufferA[battlerId][2];
    
    for (i = 0; i < 3; ++i)
        gBattlePartyCurrentOrder[i] = gBattleBufferA[battlerId][3 + i];
    
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
    gBattlerControllersData[battlerId].func = controllerFunc;
    gBattlerInMenuId = battlerId;
}

void BtlController_HandleHealthbarUpdate(u32 battlerId, bool32 updateHpTextNow, bool32 updateHpTextAfter)
{
    s16 hpVal = READ_16(&gBattleBufferA[battlerId][2]);
    struct Pokemon *mon = GetBattlerPartyIndexPtr(battlerId);
    u32 curHP = GetMonData(mon, MON_DATA_HP), maxHP = GetMonData(mon, MON_DATA_MAX_HP);

    LoadBattleBarGfx();
    
    if (hpVal != INSTANT_HP_BAR_DROP)
    {
        SetBattleBarStruct(battlerId, maxHP, curHP, hpVal);
        
        if (gTestRunnerEnabled)
            TestRunner_Battle_RecordHP(battlerId, curHP, clamp(curHP - hpVal, 0, maxHP));
    }
    else
    {
        SetBattleBarStruct(battlerId, maxHP, 0, hpVal);
        
        if (updateHpTextNow)
            UpdateHpTextInHealthbox(gHealthboxSpriteIds[battlerId], 0, maxHP);
        
        if (gTestRunnerEnabled)
            TestRunner_Battle_RecordHP(battlerId, curHP, 0);
    }
    gBattlerControllersData[battlerId].data = updateHpTextAfter;
    gBattlerControllersData[battlerId].func = CompleteOnHealthbarDone;
}

void BtlController_HandleHealthbarUpdateWithHpText(u32 battlerId)
{
    BtlController_HandleHealthbarUpdate(battlerId, TRUE, TRUE);
}

void BtlController_HandleHealthbarUpdateNoHpText(u32 battlerId)
{
    BtlController_HandleHealthbarUpdate(battlerId, FALSE, FALSE);
}

void BtlController_HandleExpUpdate(u32 battlerId)
{
    u32 monId = gBattleBufferA[battlerId][1];

    if (GetMonData(&gPlayerParty[monId], MON_DATA_LEVEL) >= MAX_LEVEL)
        BattleControllerComplete(battlerId);
    else
    {
        u32 taskId;

        LoadBattleBarGfx();

        taskId = CreateTask(Task_GiveExpToMon, 10);
        gTasks[taskId].tExpTask_monId = monId;
        gTasks[taskId].tExpTask_gainedExp = READ_16(&gBattleBufferA[battlerId][2]);
        gTasks[taskId].tExpTask_battler = battlerId;
        
        gBattlerControllersData[battlerId].func = ControllerDummy;
    }
}

void BtlController_HandleStatusIconUpdate(u32 battlerId)
{
    if (!IsBattleSEPlaying(battlerId))
    {
        UpdateHealthboxAttribute(battlerId, HEALTHBOX_STATUS_ICON);
        gBattleSpritesDataPtr->healthBoxesData[battlerId].statusAnimActive = FALSE;
        
        if (gTestRunnerEnabled)
            TestRunner_Battle_RecordStatus1(battlerId, GetMonData(GetBattlerPartyIndexPtr(battlerId), MON_DATA_STATUS_ID));
        
        BattleControllerComplete(battlerId);
    }
}

void BtlController_HandleStatusAnimation(u32 battlerId)
{
    if (!IsBattleSEPlaying(battlerId))
    {
        InitAndLaunchChosenStatusAnimation(battlerId, gBattleBufferA[battlerId][1], READ_32(&gBattleBufferA[battlerId][2]));
        gBattlerControllersData[battlerId].func = CompleteOnFinishedStatusAnimation;
    }
}

void BtlController_HandlePlaySE(u32 battlerId)
{
    PlaySE12WithPanning(READ_16(&gBattleBufferA[battlerId][1]), GetBattlerSide(battlerId) == B_SIDE_PLAYER ? SOUND_PAN_ATTACKER : SOUND_PAN_TARGET);
    BattleControllerComplete(battlerId);
}

void BtlController_HandlePlayFanfare(u32 battlerId)
{
    PlayFanfare(READ_16(&gBattleBufferA[battlerId][1]));
    BattleControllerComplete(battlerId);
}

void BtlController_HandleFaintingCry(u32 battlerId)
{
    PlayCry_ByMode(GetMonData(GetBattlerPartyIndexPtr(battlerId), MON_DATA_SPECIES), GetBattlerSide(battlerId) == B_SIDE_PLAYER ? -25 : 25, CRY_MODE_FAINT);
    BattleControllerComplete(battlerId);
}

void BtlController_HandleIntroSlide(u32 battlerId)
{
    HandleIntroSlide(gBattleBufferA[battlerId][1]);
    gIntroSlideFlags |= INTRO_SLIDE_FLAG_SLIDE_IN_COUNTER;
    BattleControllerComplete(battlerId);
}

void BtlController_HandleIntroTrainerBallThrow(u32 battlerId, u32 loadPicTag, u32 trainerPicId, void(*spriteCallback)(struct Sprite *), u32 delay, void(*controllerFunc)(u32))
{
    u32 paletteNum, taskId;
    bool32 isOpponent;
    struct Sprite *sprite = &gSprites[gBattlerSpriteIds[battlerId]];

    SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
    
    isOpponent = (GetBattlerSide(battlerId) == B_SIDE_OPPONENT);
    
    if (isOpponent)
    {
        sprite->data[0] = 35;
        sprite->data[2] = 280;
    }
    else
    {
        sprite->data[0] = 50;
        sprite->data[2] = -40;
    }
    sprite->data[4] = sprite->y;
    sprite->callback = spriteCallback;
    StoreSpriteCallbackInData6(sprite, isOpponent ? SpriteCB_FreeOpponentSprite : SpriteCB_FreePlayerSpriteLoadMonSprite);
    
    if (loadPicTag)
    {
        sprite->data[5] = battlerId;
        StartSpriteAnim(sprite, 1);
        paletteNum = AllocSpritePalette(loadPicTag);
        LoadCompressedPalette(gTrainerBackPicTable[trainerPicId].palette.data, 0x100 + paletteNum * 16, 32);
        sprite->oam.paletteNum = paletteNum;
    }
    taskId = CreateTask(Task_StartSendOutAnim, 5);
    gTasks[taskId].tSendOutTask_Battler = battlerId;
    gTasks[taskId].tSendOutTask_Delay = delay;
    SetWordTaskArg(taskId, tSendOutTask_ControllerFunc, (u32)controllerFunc);
    
    if (gBattleSpritesDataPtr->healthBoxesData[battlerId].partyStatusSummaryShown)
        gTasks[gBattlerStatusSummaryTaskId[battlerId]].func = Task_HidePartyStatusSummary;
    
    gBattleSpritesDataPtr->animationData->healthboxSlideInStarted = TRUE;
    gBattlerControllersData[battlerId].func = ControllerDummy;
}

void BtlController_HandleDrawPartyStatusSummary(u32 battlerId)
{
    if (gBattleBufferA[battlerId][1] && GetBattlerSide(battlerId) == B_SIDE_PLAYER)
        BattleControllerComplete(battlerId);
    else
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].partyStatusSummaryShown = TRUE;

        if (GetBattlerSide(battlerId) == B_SIDE_OPPONENT && gBattleBufferA[battlerId][2])
        {
            if (gBattleSpritesDataPtr->healthBoxesData[battlerId].opponentDrawPartyStatusSummaryDelay < 2)
            {
                ++gBattleSpritesDataPtr->healthBoxesData[battlerId].opponentDrawPartyStatusSummaryDelay;
                return;
            }
            else
                gBattleSpritesDataPtr->healthBoxesData[battlerId].opponentDrawPartyStatusSummaryDelay = 0;
        }
        gBattlerStatusSummaryTaskId[battlerId] = CreatePartyStatusSummarySprites(battlerId, (struct HpAndStatus*)&gBattleBufferA[battlerId][4], gBattleBufferA[battlerId][1], gBattleBufferA[battlerId][2]);
        gBattleSpritesDataPtr->healthBoxesData[battlerId].partyStatusDelayTimer = 0;
        
        if (gBattleBufferA[battlerId][2])
            gBattleSpritesDataPtr->healthBoxesData[battlerId].partyStatusDelayTimer = 93;
        
        gBattlerControllersData[battlerId].func = EndDrawPartyStatusSummary;
    }
}

void BtlController_HandleHidePartyStatusSummary(u32 battlerId)
{
    if (gBattleSpritesDataPtr->healthBoxesData[battlerId].partyStatusSummaryShown)
        gTasks[gBattlerStatusSummaryTaskId[battlerId]].func = Task_HidePartyStatusSummary;
    BattleControllerComplete(battlerId);
}

void BtlController_HandleEndBounceEffect(u32 battlerId)
{
    EndBounceEffect(battlerId, BOUNCE_HEALTHBOX);
    EndBounceEffect(battlerId, BOUNCE_MON);
    BattleControllerComplete(battlerId);
}

void BtlController_HandleSpriteInvisibility(u32 battlerId)
{
    if (IsBattlerSpritePresent(battlerId))
    {
        gSprites[gBattlerSpriteIds[battlerId]].invisible = gBattleBufferA[battlerId][1];
        CopyBattleSpriteInvisibility(battlerId);
    }
    BattleControllerComplete(battlerId);
}

void BtlController_HandleBattleAnimation(u32 battlerId)
{
    if (!IsBattleSEPlaying(battlerId))
    {
        gAnimLastUsedItem = READ_16(&gBattleBufferA[battlerId][4]);
        
        if (TryHandleLaunchBattleTableAnimation(battlerId, gBattleBufferA[battlerId][1], READ_16(&gBattleBufferA[battlerId][2])))
            BattleControllerComplete(battlerId);
        else
            gBattlerControllersData[battlerId].func = CompleteOnFinishedBattleAnimation;
    }
}

void BtlController_HandleEndLinkBattle(u32 battlerId, u32 battleOutcome, void(*controllerFunc)(u32))
{
    gBattleOutcome = battleOutcome;
    FadeOutMapMusic(5);
    BeginFastPaletteFade(FAST_FADE_OUT_TO_BLACK);
    BattleControllerComplete(battlerId);
    
    if (controllerFunc != NULL)
        gBattlerControllersData[battlerId].func = controllerFunc;
}

void BtlController_HandleGimmickState(u32 battlerId, bool32 canCheckKeyItems)
{
    switch (gBattleBufferA[battlerId][1])
    {
        case STATE_CHECK_GIMMICK_KEY_ITEMS:
            if (canCheckKeyItems)
            {
                u8 data[NUM_GIMMICK_BITS + 1] = {0};
                GetGimmicksKeyItemsBits(battlerId, data);
                BtlController_EmitDataTransfer(battlerId, BUFFER_B, NUM_GIMMICK_BITS, data);
            }
            break;
        case STATE_USABLE_GIMMICK:
            gBattleStruct->battlers[battlerId].usableGimmick = gBattleBufferA[battlerId][2];
            break;
        case STATE_GIMMICK_IN_PROGRESS:
            gBattleSpritesDataPtr->battlerData[battlerId].gimmickInProgress = TRUE;
            break;
        case STATE_ACTIVE_GIMMICK:
            SetActiveGimmick(battlerId, gBattleBufferA[battlerId][2]);
            break;
    }
    BattleControllerComplete(battlerId);
}

void BtlController_HandleHealthboxUpdate(u32 battlerId)
{
    UpdateHealthboxAttribute(battlerId, gBattleBufferA[battlerId][1]);
    BattleControllerComplete(battlerId);
}

void BtlController_HandleHideAllHealthboxes(u32 battlerId)
{
    UpdateOamPriorityInAllHealthboxes(1, TRUE);
    gBattleStruct->raid.keepHealthboxesHidden = TRUE;
    BattleControllerComplete(battlerId);
}

void BtlController_HandleBattleFormChange(u32 battlerId)
{
    u16 maxHP;
    u32 newSpecies;
    struct Pokemon *mon = GetBattlerPartyIndexPtr(battlerId);
    
    gBattleSpritesDataPtr->battlerData[battlerId].formChangeSpecies = GetMonData(mon, MON_DATA_SPECIES); // For the form change anim
    
    newSpecies = READ_16(&gBattleBufferA[battlerId][1]);
    SetMonData(mon, MON_DATA_SPECIES, &newSpecies);
    
    if (gBattleBufferA[battlerId][3]) // Reload stats
    {
        bool32 isDynamaxed = (GetActiveGimmick(battlerId) == GIMMICK_DYNAMAX);
        
        if (isDynamaxed)
        {
            maxHP = GetMonData(mon, MON_DATA_MAX_HP);
            CalcBattlerDynamaxHP(battlerId, NULL, &maxHP, TRUE);
            SetMonData(mon, MON_DATA_MAX_HP, &maxHP);
        }
        CalculateMonStats(mon);
        
        if (isDynamaxed)
        {
            maxHP = GetMonData(mon, MON_DATA_MAX_HP);
            CalcBattlerDynamaxHP(battlerId, NULL, &maxHP, FALSE);
            SetMonData(mon, MON_DATA_MAX_HP, &maxHP);
        }
    }
    BattleControllerComplete(battlerId);
}

void BtlController_HandlePartyFormChange(u32 battlerId)
{
    u32 side = GetBattlerSide(battlerId);
    u32 partyId = gBattleBufferA[battlerId][1];
    struct Pokemon *mon = &GetSideParty(side)[partyId];
    
    switch (gBattleBufferA[battlerId][2])
    {
        case FORMCHANGE_SAVE_ORIGINAL_FORM:
            // Try revert shaymin
            DoOverworldFormChange(mon, FORM_CHANGE_TIME);
            
            // Save original species for battle form changes
            gBattleMonForms[side][partyId] = GetMonData(mon, MON_DATA_SPECIES);
            
            // Transform Xerneas into active form
            DoOverworldFormChange(mon, FORM_CHANGE_START_BATTLE);
            
            // Try Transform Zacian and Zamazenta's Iron Head into their moves
            TryTransformZacianAndZamazentaIronHead(mon, FALSE);
            break;
        case FORMCHANGE_CHANGE_RAID_BOSS_FORM:
            DoOverworldFormChange(mon, FORM_CHANGE_GIGANTAMAX);
            break;
        case FORMCHANGE_RESTORE_ORIGINAL_FORM:
            SetMonData(mon, MON_DATA_SPECIES, &gBattleMonForms[side][partyId]);
            CalculateMonStatsAfterChangeForm(mon);
            
            // Try Transform Zacian and Zamazenta's moves into Iron Head
            TryTransformZacianAndZamazentaIronHead(mon, TRUE);
            
            DoOverworldFormChange(mon, FORM_CHANGE_KNOW_MOVE);
            break;
        case FORMCHANGE_UPDATE_BURMY_FORM:
            DoOverworldFormChange(mon, FORM_CHANGE_TERRAIN);
            break;
        case FORMCHANGE_REVERT_ON_FAINT:
        {
            u32 i;
            u16 newHP, stats[NUM_STATS - 1];
            
            SetMonData(mon, MON_DATA_SPECIES, &gBattleMonForms[side][partyId]);
            CalculateMonStatsAfterChangeForm(mon);
            
            for (i = 0; i < NUM_STATS - 1; i++)
                stats[i] = GetMonData(mon, MON_DATA_ATK + i);
            
            // Reload all stats
            CalculateMonStats(mon);
            
            for (i = 0; i < NUM_STATS - 1; i++)
                SetMonData(mon, MON_DATA_ATK + i, &stats[i]);
            
            newHP = min(GetMonData(mon, MON_DATA_MAX_HP), GetMonData(mon, MON_DATA_HP));
            SetMonData(mon, MON_DATA_HP, &newHP);
            break;
        }
    }
    BattleControllerComplete(battlerId);
}

void BtlController_HandleIsPocketNotEmpty(u32 battlerId)
{
    BtlController_EmitOneReturnValue(battlerId, BUFFER_B, IsPocketNotEmpty(gBattleBufferA[battlerId][1]));
    BattleControllerComplete(battlerId);
}

void BtlController_HandleYesNoBox(u32 battlerId)
{
    u32 stringId = READ_16(&gBattleBufferA[battlerId][2]);
    
    if (stringId != STRINGID_EMPTYSTRING)
    {
        gBattle_BG0_X = 0;
        gBattle_BG0_Y = 0;
        BufferStringBattle(battlerId, stringId);
        BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_MSG);
        
        gBattlerControllersData[battlerId].func = CreateYesNoBoxOnInactiveTextPrinter;
    }
    else
        gBattlerControllersData[battlerId].func = CreateYesNoBox;
}

////////////////////////
// ULTILITY FUNCTIONS //
////////////////////////

void CompleteOnBattlerSpritePosX_0(u32 battlerId)
{
    if (gSprites[gBattlerSpriteIds[battlerId]].x2 == 0)
        BattleControllerComplete(battlerId);
}

#define SET_8(value)    \
    dst[size++] = value

#define SET_16(value)           \
    dst[size + 0] = value;      \
    dst[size + 1] = value >> 8; \
    size += 2

#define SET_32(value)                           \
    dst[size + 0] = (value & 0x000000FF);       \
    dst[size + 1] = (value & 0x0000FF00) >> 8;  \
    dst[size + 2] = (value & 0x00FF0000) >> 16; \
    dst[size + 3] = (value & 0xFF000000) >> 24; \
    size += 4

static u32 GetBattlerMonData(u32 battlerId, struct Pokemon *party, u32 monId, u8 *dst)
{
    struct BattlePokemon battleMon;
    struct MovePpInfo moveData;
    u16 stats[NUM_STATS - 1];
    u8 *src, nickname[POKEMON_NAME_LENGTH + 1];
    s32 size = 0;
    
    switch (gBattleBufferA[battlerId][1])
    {
        case REQUEST_ALL_BATTLE:
            // Apply raid boss Dynamax HP boost, handled here so it starts the battle with the boost already in effect
            if (IsRaidBoss(battlerId))
            {
                u16 hp = GetMonData(&party[monId], MON_DATA_HP), maxHP = GetMonData(&party[monId], MON_DATA_MAX_HP);
                
                CalcBattlerDynamaxHP(battlerId, &hp, &maxHP, FALSE);
                
                SetMonData(&party[monId], MON_DATA_HP, &hp);
                SetMonData(&party[monId], MON_DATA_MAX_HP, &maxHP);
            }
            CopyPokemonToBattleMon(battlerId, &party[monId], &battleMon, FALSE);

            src = (u8 *)&battleMon;
            for (size = 0; size < sizeof(battleMon); ++size)
                dst[size] = src[size];
            break;
        case REQUEST_MOVES_PP_BATTLE:
            for (size = 0; size < MAX_MON_MOVES; size++)
            {
                moveData.moves[size] = GetMonData(&party[monId], MON_DATA_MOVE1 + size);
                moveData.pp[size] = GetMonData(&party[monId], MON_DATA_PP1 + size);
            }
            moveData.ppBonuses = GetMonData(&party[monId], MON_DATA_PP_BONUSES);
            
            src = (u8 *)(&moveData);
            for (size = 0; size < sizeof(moveData); ++size)
                dst[size] = src[size];
            break;
        case REQUEST_ALL_IVS_BATTLE:
            for (size = 0; size < NUM_STATS; size++)
                dst[size] = GetMonData(&party[monId], MON_DATA_HP_IV + size);
            break;
        case REQUEST_HP_IV_BATTLE:
        case REQUEST_ATK_IV_BATTLE:
        case REQUEST_DEF_IV_BATTLE:
        case REQUEST_SPEED_IV_BATTLE:
        case REQUEST_SPATK_IV_BATTLE:
        case REQUEST_SPDEF_IV_BATTLE:
            SET_8(GetMonData(&party[monId], MON_DATA_HP_IV + gBattleBufferA[battlerId][1] - REQUEST_HP_IV_BATTLE));
            break;
        case REQUEST_PP_DATA_BATTLE:
            for (size = 0; size < MAX_MON_MOVES; ++size)
                dst[size] = GetMonData(&party[monId], MON_DATA_PP1 + size);
            SET_8(GetMonData(&party[monId], MON_DATA_PP_BONUSES));
            break;
        case REQUEST_PPMOVE1_BATTLE:
        case REQUEST_PPMOVE2_BATTLE:
        case REQUEST_PPMOVE3_BATTLE:
        case REQUEST_PPMOVE4_BATTLE:
            SET_8(GetMonData(&party[monId], MON_DATA_PP1 + gBattleBufferA[battlerId][1] - REQUEST_PPMOVE1_BATTLE));
            break;
        case REQUEST_HP_EV_BATTLE:
        case REQUEST_ATK_EV_BATTLE:
        case REQUEST_DEF_EV_BATTLE:
        case REQUEST_SPEED_EV_BATTLE:
        case REQUEST_SPATK_EV_BATTLE:
        case REQUEST_SPDEF_EV_BATTLE:
            SET_8(GetMonData(&party[monId], MON_DATA_HP_EV + gBattleBufferA[battlerId][1] - REQUEST_HP_EV_BATTLE));
            break;
        case REQUEST_FRIENDSHIP_BATTLE:
            SET_8(GetMonData(&party[monId], MON_DATA_FRIENDSHIP));
            break;
        case REQUEST_POKERUS_BATTLE:
            SET_8(GetMonData(&party[monId], MON_DATA_POKERUS));
            break;
        case REQUEST_MET_LOCATION_BATTLE:
            SET_8(GetMonData(&party[monId], MON_DATA_MET_LOCATION));
            break;
        case REQUEST_MET_LEVEL_BATTLE:
            SET_8(GetMonData(&party[monId], MON_DATA_MET_LEVEL));
            break;
        case REQUEST_MET_GAME_BATTLE:
            SET_8(GetMonData(&party[monId], MON_DATA_MET_GAME));
            break;
        case REQUEST_POKEBALL_BATTLE:
            SET_8(GetMonData(&party[monId], MON_DATA_POKEBALL));
            break;
        case REQUEST_LEVEL_BATTLE:
            SET_8(GetMonData(&party[monId], MON_DATA_LEVEL));
            break;
        case REQUEST_SPECIES_BATTLE:
            SET_16(GetMonData(&party[monId], MON_DATA_SPECIES));
            break;
        case REQUEST_HELDITEM_BATTLE:
            SET_16(GetMonData(&party[monId], MON_DATA_HELD_ITEM));
            break;
        case REQUEST_MOVE1_BATTLE:
        case REQUEST_MOVE2_BATTLE:
        case REQUEST_MOVE3_BATTLE:
        case REQUEST_MOVE4_BATTLE:
            SET_16(GetMonData(&party[monId], MON_DATA_MOVE1 + gBattleBufferA[battlerId][1] - REQUEST_MOVE1_BATTLE));
            break;
        case REQUEST_MAX_HP_BATTLE:
            SET_16(GetMonData(&party[monId], MON_DATA_MAX_HP));
            break;
        case REQUEST_HP_BATTLE:
        case REQUEST_ATK_BATTLE:
        case REQUEST_DEF_BATTLE:
        case REQUEST_SPEED_BATTLE:
        case REQUEST_SPATK_BATTLE:
        case REQUEST_SPDEF_BATTLE:
            SET_16(GetMonData(&party[monId], MON_DATA_HP + gBattleBufferA[battlerId][1] - REQUEST_HP_BATTLE));
            break;
        case REQUEST_OTID_BATTLE:
            SET_32(GetMonData(&party[monId], MON_DATA_OT_ID));
            break;
        case REQUEST_EXP_BATTLE:
            SET_32(GetMonData(&party[monId], MON_DATA_EXP));
            break;
        case REQUEST_PERSONALITY_BATTLE:
            SET_32(GetMonData(&party[monId], MON_DATA_PERSONALITY));
            break;
        case REQUEST_STATUS_BATTLE:
            SET_8(GetMonData(&party[monId], MON_DATA_STATUS_ID) | (GetMonData(&party[monId], MON_DATA_STATUS_COUNTER) << 4));
            break;
    }
    return size;
}

static void SetBattlerMonData(u32 battlerId, struct Pokemon *party, u32 monId)
{
     struct BattlePokemon *battlePokemon;
     struct MovePpInfo *moveData;
     struct Status1 *status;
     u16 *stats;
     s32 i;
     
     switch (gBattleBufferA[battlerId][1])
     {
         case REQUEST_ALL_BATTLE:
         {
            u32 val;
            
            battlePokemon = (struct BattlePokemon *)&gBattleBufferA[battlerId][3];
            
            SetMonData(&party[monId], MON_DATA_SPECIES, &battlePokemon->species);
            SetMonData(&party[monId], MON_DATA_HELD_ITEM, &battlePokemon->item);
            SetMonData(&party[monId], MON_DATA_PP_BONUSES, &battlePokemon->ppBonuses);
            SetMonData(&party[monId], MON_DATA_FRIENDSHIP, &battlePokemon->friendship);
            SetMonData(&party[monId], MON_DATA_EXP, &battlePokemon->experience);
            SetMonData(&party[monId], MON_DATA_PERSONALITY, &battlePokemon->personality);
            val = battlePokemon->status1.id;
            SetMonData(&party[monId], MON_DATA_STATUS_ID, &val);
            val = battlePokemon->status1.counter;
            SetMonData(&party[monId], MON_DATA_STATUS_COUNTER, &val);
            SetMonData(&party[monId], MON_DATA_LEVEL, &battlePokemon->level);
            SetMonData(&party[monId], MON_DATA_HP, &battlePokemon->hp);
            SetMonData(&party[monId], MON_DATA_MAX_HP, &battlePokemon->maxHP);
            SetMonData(&party[monId], MON_DATA_ATK, &battlePokemon->attack);
            SetMonData(&party[monId], MON_DATA_DEF, &battlePokemon->defense);
            SetMonData(&party[monId], MON_DATA_SPEED, &battlePokemon->speed);
            SetMonData(&party[monId], MON_DATA_SPATK, &battlePokemon->spAttack);
            SetMonData(&party[monId], MON_DATA_SPDEF, &battlePokemon->spDefense);
            i = battlePokemon->hpIV;
            SetMonData(&party[monId], MON_DATA_HP_IV, &i);
            i = battlePokemon->attackIV;
            SetMonData(&party[monId], MON_DATA_ATK_IV, &i);
            i = battlePokemon->defenseIV;
            SetMonData(&party[monId], MON_DATA_DEF_IV, &i);
            i = battlePokemon->speedIV;
            SetMonData(&party[monId], MON_DATA_SPEED_IV, &i);
            i = battlePokemon->spAttackIV;
            SetMonData(&party[monId], MON_DATA_SPATK_IV, &i);
            i = battlePokemon->spDefenseIV;
            SetMonData(&party[monId], MON_DATA_SPDEF_IV, &i);
            
            for (i = 0; i < MAX_MON_MOVES; ++i)
            {
                SetMonData(&party[monId], MON_DATA_MOVE1 + i, &battlePokemon->moves[i]);
                SetMonData(&party[monId], MON_DATA_PP1 + i, &battlePokemon->pp[i]);
            }
            break;
         }
        case REQUEST_SPECIES_BATTLE:
            SetMonData(&party[monId], MON_DATA_SPECIES, &gBattleBufferA[battlerId][3]);
            break;
        case REQUEST_HELDITEM_BATTLE:
            SetMonData(&party[monId], MON_DATA_HELD_ITEM, &gBattleBufferA[battlerId][3]);
            break;
        case REQUEST_MOVES_PP_BATTLE:
            moveData = (struct MovePpInfo *)&gBattleBufferA[battlerId][3];
            
            for (i = 0; i < MAX_MON_MOVES; ++i)
            {
                SetMonData(&party[monId], MON_DATA_MOVE1 + i, &moveData->moves[i]);
                SetMonData(&party[monId], MON_DATA_PP1 + i, &moveData->pp[i]);
            }
            SetMonData(&party[monId], MON_DATA_PP_BONUSES, &moveData->ppBonuses);
            break;
        case REQUEST_MOVE1_BATTLE:
        case REQUEST_MOVE2_BATTLE:
        case REQUEST_MOVE3_BATTLE:
        case REQUEST_MOVE4_BATTLE:
            SetMonData(&party[monId], MON_DATA_MOVE1 + gBattleBufferA[battlerId][1] - REQUEST_MOVE1_BATTLE, &gBattleBufferA[battlerId][3]);
            break;
        case REQUEST_PP_DATA_BATTLE:
            for (i = 0; i < MAX_MON_MOVES; ++i)
                SetMonData(&party[monId], MON_DATA_PP1 + i, &gBattleBufferA[battlerId][3 + i]);
            SetMonData(&party[monId], MON_DATA_PP_BONUSES, &gBattleBufferA[battlerId][3 + MAX_MON_MOVES]);
            break;
        case REQUEST_PPMOVE1_BATTLE:
        case REQUEST_PPMOVE2_BATTLE:
        case REQUEST_PPMOVE3_BATTLE:
        case REQUEST_PPMOVE4_BATTLE:
            SetMonData(&party[monId], MON_DATA_PP1 + gBattleBufferA[battlerId][1] - REQUEST_PPMOVE1_BATTLE, &gBattleBufferA[battlerId][3]);
            break;
        case REQUEST_OTID_BATTLE:
            SetMonData(&party[monId], MON_DATA_OT_ID, &gBattleBufferA[battlerId][3]);
            break;
        case REQUEST_EXP_BATTLE:
            SetMonData(&party[monId], MON_DATA_EXP, &gBattleBufferA[battlerId][3]);
            break;
        case REQUEST_HP_EV_BATTLE:
        case REQUEST_ATK_EV_BATTLE:
        case REQUEST_DEF_EV_BATTLE:
        case REQUEST_SPEED_EV_BATTLE:
        case REQUEST_SPATK_EV_BATTLE:
        case REQUEST_SPDEF_EV_BATTLE:
            SetMonData(&party[monId], MON_DATA_HP_EV + gBattleBufferA[battlerId][1] - REQUEST_HP_EV_BATTLE, &gBattleBufferA[battlerId][3]);
            break;
        case REQUEST_FRIENDSHIP_BATTLE:
            SetMonData(&party[monId], MON_DATA_FRIENDSHIP, &gBattleBufferA[battlerId][3]);
            break;
        case REQUEST_POKERUS_BATTLE:
            SetMonData(&party[monId], MON_DATA_POKERUS, &gBattleBufferA[battlerId][3]);
            break;
        case REQUEST_MET_LOCATION_BATTLE:
            SetMonData(&party[monId], MON_DATA_MET_LOCATION, &gBattleBufferA[battlerId][3]);
            break;
        case REQUEST_MET_LEVEL_BATTLE:
            SetMonData(&party[monId], MON_DATA_MET_LEVEL, &gBattleBufferA[battlerId][3]);
            break;
        case REQUEST_MET_GAME_BATTLE:
            SetMonData(&party[monId], MON_DATA_MET_GAME, &gBattleBufferA[battlerId][3]);
            break;
        case REQUEST_POKEBALL_BATTLE:
            SetMonData(&party[monId], MON_DATA_POKEBALL, &gBattleBufferA[battlerId][3]);
            break;
        case REQUEST_ALL_IVS_BATTLE:
            for (i = 0; i < NUM_STATS; i++)
                SetMonData(&party[monId], MON_DATA_HP_IV + i, &gBattleBufferA[battlerId][3 + i]);
            break;
        case REQUEST_HP_IV_BATTLE:
        case REQUEST_ATK_IV_BATTLE:
        case REQUEST_DEF_IV_BATTLE:
        case REQUEST_SPEED_IV_BATTLE:
        case REQUEST_SPATK_IV_BATTLE:
        case REQUEST_SPDEF_IV_BATTLE:
            SetMonData(&party[monId], MON_DATA_HP_IV + gBattleBufferA[battlerId][1] - REQUEST_HP_IV_BATTLE, &gBattleBufferA[battlerId][3]);
            break;
        case REQUEST_PERSONALITY_BATTLE:
            SetMonData(&party[monId], MON_DATA_PERSONALITY, &gBattleBufferA[battlerId][3]);
            break;
        case REQUEST_STATUS_BATTLE:
        {
            u32 val;
            
            status = (struct Status1 *)&gBattleBufferA[battlerId][3];
            
            val = status->id;
            SetMonData(&party[monId], MON_DATA_STATUS_ID, &val);
            val = status->counter;
            SetMonData(&party[monId], MON_DATA_STATUS_COUNTER, &val);
            break;
        }
        case REQUEST_LEVEL_BATTLE:
            SetMonData(&party[monId], MON_DATA_LEVEL, &gBattleBufferA[battlerId][3]);
            break;
        case REQUEST_MAX_HP_BATTLE:
            SetMonData(&party[monId], MON_DATA_MAX_HP, &gBattleBufferA[battlerId][3]);
            break;
        case REQUEST_HP_BATTLE:
        case REQUEST_ATK_BATTLE:
        case REQUEST_DEF_BATTLE:
        case REQUEST_SPEED_BATTLE:
        case REQUEST_SPATK_BATTLE:
        case REQUEST_SPDEF_BATTLE:
            SetMonData(&party[monId], MON_DATA_HP + gBattleBufferA[battlerId][1] - REQUEST_HP_BATTLE, &gBattleBufferA[battlerId][3]);
            break;
     }
}

static void TryShinyAnimAfterMonAnim(u32 battlerId)
{
    if (gSprites[gBattlerSpriteIds[battlerId]].animEnded && gSprites[gBattlerSpriteIds[battlerId]].x2 == 0)
    {
        if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim)
            TryShinyAnimation(battlerId);
        else if (gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim)
        {
            gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim = FALSE;
            gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim = FALSE;
            
            FreeSpriteTilesByTag(ANIM_TAG_GOLD_STARS);
            FreeSpritePaletteByTag(ANIM_TAG_GOLD_STARS);
            
            BattleControllerComplete(battlerId);
        }
    }
}

static void FreeMonSpriteAfterSwitchOutAnim(u32 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].specialAnimActive)
    {
        FreeMonSprite(battlerId);
        BattleControllerComplete(battlerId);
    }
}

static void DoSwitchOutAnimation(u32 battlerId)
{
    switch (gBattleSpritesDataPtr->healthBoxesData[battlerId].animationState)
    {
    case 0:
        if (gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute)
            InitAndLaunchSpecialAnimation(battlerId, battlerId, B_ANIM_SUBSTITUTE_TO_MON);

        gBattleSpritesDataPtr->healthBoxesData[battlerId].animationState = 1;
        break;
    case 1:
        if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].specialAnimActive)
        {
            gBattleSpritesDataPtr->healthBoxesData[battlerId].animationState = 0;
            InitAndLaunchSpecialAnimation(battlerId, battlerId, GetBattlerSide(battlerId) == B_SIDE_PLAYER ? B_ANIM_SWITCH_OUT_PLAYER_MON : B_ANIM_SWITCH_OUT_OPPONENT_MON);
            gBattlerControllersData[battlerId].func = FreeMonSpriteAfterSwitchOutAnim;
        }
        break;
    }
}

static void FreeMonSprite(u32 battlerId)
{
    DestroySpriteAndFreeMatrix(&gSprites[gBattlerSpriteIds[battlerId]]);
    
    if (GetBattlerSide(battlerId) == B_SIDE_OPPONENT)
        HideBattlerShadowSprite(battlerId);
    
    SetHealthboxSpriteVisibility(gHealthboxSpriteIds[battlerId], TRUE);
}

static void CompleteOnBattlerSpriteCallbackDummy(u32 battlerId)
{
    if (gSprites[gBattlerSpriteIds[battlerId]].callback == SpriteCallbackDummy)
        BattleControllerComplete(battlerId);
}

static void FreeTrainerSpriteAfterSlide(u32 battlerId)
{
    struct Sprite *sprite = &gSprites[gBattlerSpriteIds[battlerId]];
    
    if (sprite->callback == SpriteCallbackDummy)
    {
        if (GetBattlerSide(battlerId) == B_SIDE_OPPONENT)
        {
            FreeTrainerFrontPicPaletteAndTile(sprite->oam.affineParam);
            sprite->oam.tileNum = sprite->data[5];
        }
        DestroySpriteAndFreeMatrix(sprite);
        
        BattleControllerComplete(battlerId);
    }
}

static void FreeMonSpriteAfterFaintAnim(u32 battlerId)
{
    if (gSprites[gBattlerSpriteIds[battlerId]].y + gSprites[gBattlerSpriteIds[battlerId]].y2 > DISPLAY_HEIGHT)
    {
        DestroySpriteAndFreeMatrix(&gSprites[gBattlerSpriteIds[battlerId]]);
        SetHealthboxSpriteVisibility(gHealthboxSpriteIds[battlerId], TRUE);
        BattleControllerComplete(battlerId);
    }
}

static void HideHealthboxAfterMonFaint(u32 battlerId)
{
    if (!gSprites[gBattlerSpriteIds[battlerId]].inUse)
    {
        SetHealthboxSpriteVisibility(gHealthboxSpriteIds[battlerId], TRUE);
        BattleControllerComplete(battlerId);
    }
}

static void CompleteOnSpecialAnimDone(u32 battlerId)
{
    if (!gDoingBattleAnim)
        BattleControllerComplete(battlerId);
}

static void DoMoveAnimation(u32 battlerId)
{
    switch (gBattleSpritesDataPtr->healthBoxesData[battlerId].animationState)
    {
    case 0:
        if (gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute && !gBattleSpritesDataPtr->battlerData[battlerId].flag_x8)
        {
            gBattleSpritesDataPtr->battlerData[battlerId].flag_x8 = 1;
            InitAndLaunchSpecialAnimation(battlerId, battlerId, B_ANIM_SUBSTITUTE_TO_MON);
        }
        gBattleSpritesDataPtr->healthBoxesData[battlerId].animationState = 1;
        break;
    case 1:
        if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].specialAnimActive)
        {
            SetBattlerSpriteAffineMode(ST_OAM_AFFINE_OFF);
            DoMoveAnim((gBattleBufferA[battlerId][1] | (gBattleBufferA[battlerId][2] << 8)));
            gBattleSpritesDataPtr->healthBoxesData[battlerId].animationState = 2;
        }
        break;
    case 2:
        gAnimScriptCallback();
        
        if (!gAnimScriptActive)
        {
            if (gBattleMoves[gAnimMoveIndex].effect != EFFECT_ALLY_SWITCH)
                SetBattlerSpriteAffineMode(ST_OAM_AFFINE_NORMAL);
            
            if (gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute && gBattleBufferA[battlerId][11] < 2)
            {
                InitAndLaunchSpecialAnimation(battlerId, battlerId, B_ANIM_MON_TO_SUBSTITUTE);
                gBattleSpritesDataPtr->battlerData[battlerId].flag_x8 = 0;
            }
            gBattleSpritesDataPtr->healthBoxesData[battlerId].animationState = 3;
        }
        break;
    case 3:
        if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].specialAnimActive)
        {
            CopyAllBattleSpritesInvisibilities();
            TrySetBehindSubstituteSpriteBit(battlerId, gBattleBufferA[battlerId][1] | (gBattleBufferA[battlerId][2] << 8));
            gBattleSpritesDataPtr->healthBoxesData[battlerId].animationState = 0;
            BattleControllerComplete(battlerId);
        }
        break;
    }
}

static void CompleteOnInactiveTextPrinter(u32 battlerId)
{
    if (!IsTextPrinterActive(0))
        BattleControllerComplete(battlerId);
}

static void CompleteOnHealthbarDone(u32 battlerId)
{
    s16 hpValue = MoveBattleBar(battlerId, HEALTH_BAR);
    struct Pokemon *mon = GetBattlerPartyIndexPtr(battlerId);

    SetHealthboxSpriteVisibility(gHealthboxSpriteIds[battlerId], FALSE);
    
    if (hpValue == -1) // Done
    {
        HandleLowHpMusicChange(battlerId, mon);
        
        if ((gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE) && !BtlCtrl_OakOldMan_TestState2Flag(FIRST_BATTLE_MSG_FLAG_INFLICT_DMG))
        {
            BtlCtrl_OakOldMan_SetState2Flag(FIRST_BATTLE_MSG_FLAG_INFLICT_DMG);
            gBattlerControllersData[battlerId].func = PrintOakText_InflictingDamageIsKey;
        }
        else
            BattleControllerComplete(battlerId);
    }
    else
    {
        if (gBattlerControllersData[battlerId].data) // Update hp text
            UpdateHpTextInHealthbox(gHealthboxSpriteIds[battlerId], hpValue, GetMonData(mon, MON_DATA_MAX_HP));
    }
}

static void DynamaxModifyHPLevelUp(struct Pokemon *mon, u32 battlerId, u32 oldMaxHP)
{
    u16 hp = GetMonData(mon, MON_DATA_HP), maxHP = GetMonData(mon, MON_DATA_MAX_HP);
    
    CalcBattlerDynamaxHP(battlerId, &hp, &maxHP, FALSE);
    SetMonData(mon, MON_DATA_HP, &hp);
    SetMonData(mon, MON_DATA_MAX_HP, &maxHP);

    gBattleScripting.field_23 = GetMonData(mon, MON_DATA_MAX_HP) - oldMaxHP; // Overwrite it since it overflows
    
    gBattleMons[battlerId].hp += gBattleScripting.field_23;
    SetMonData(mon, MON_DATA_HP, &gBattleMons[battlerId].hp);
}

static void Task_GiveExpToMon(u32 taskId)
{
    u32 monId = gTasks[taskId].tExpTask_monId, battlerId = gTasks[taskId].tExpTask_battler;
    s16 gainedExp = gTasks[taskId].tExpTask_gainedExp;

    if (IsDoubleBattleForBattler(battlerId) || monId != gBattlerPartyIndexes[battlerId]) // Give exp without moving the expbar.
    {
        struct Pokemon *mon = &gPlayerParty[monId];
        u32 currExp = GetMonData(mon, MON_DATA_EXP);
        u32 nextLvlExp = gExperienceTables[gSpeciesInfo[GetMonData(mon, MON_DATA_SPECIES)].growthRate][GetMonData(mon, MON_DATA_LEVEL) + 1];

        if (currExp + gainedExp >= nextLvlExp)
        {
            u32 oldMaxHP = GetMonData(mon, MON_DATA_MAX_HP);
            
            SetMonData(mon, MON_DATA_EXP, &nextLvlExp);
            CalculateMonStats(mon);
            
            // Reapply Dynamax HP multiplier after stats are recalculated
            if (GetActiveGimmick(battlerId) == GIMMICK_DYNAMAX && monId == gBattlerPartyIndexes[battlerId])
                DynamaxModifyHPLevelUp(mon, battlerId, oldMaxHP);
            
            gainedExp -= nextLvlExp - currExp;
            BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, RET_VALUE_LEVELED_UP, gainedExp);

            if (IsDoubleBattleForBattler(battlerId) && (monId == gBattlerPartyIndexes[battlerId] || monId == gBattlerPartyIndexes[BATTLE_PARTNER(battlerId)]))
                gTasks[taskId].func = Task_LaunchLvlUpAnim;
            else
                gTasks[taskId].func = Task_TryCreateVerticalStripes;
        }
        else
        {
            currExp += gainedExp;
            SetMonData(mon, MON_DATA_EXP, &currExp);
            gBattlerControllersData[battlerId].func = CompleteOnInactiveTextPrinter;
            DestroyTask(taskId);
        }
    }
    else
        gTasks[taskId].func = Task_PrepareToGiveExpWithExpBar;
}

static void Task_PrepareToGiveExpWithExpBar(u32 taskId)
{
    struct Pokemon *mon = &gPlayerParty[gTasks[taskId].tExpTask_monId];
    u32 level = GetMonData(mon, MON_DATA_LEVEL);
    u32 species = GetMonData(mon, MON_DATA_SPECIES);
    u32 exp = GetMonData(mon, MON_DATA_EXP);
    u32 currLvlExp = gExperienceTables[gSpeciesInfo[species].growthRate][level];
    u32 expToNextLvl;

    exp -= currLvlExp;
    expToNextLvl = gExperienceTables[gSpeciesInfo[species].growthRate][level + 1] - currLvlExp;
    
    SetBattleBarStruct(gTasks[taskId].tExpTask_battler, expToNextLvl, exp, -gTasks[taskId].tExpTask_gainedExp);
    
    if (gTestRunnerEnabled)
        TestRunner_Battle_RecordExp(gTasks[taskId].tExpTask_battler, exp, -gTasks[taskId].tExpTask_gainedExp);
    
    PlaySE(SE_EXP);
    gTasks[taskId].func = Task_GiveExpWithExpBar;
}

static void Task_GiveExpWithExpBar(u32 taskId)
{
    if (gTasks[taskId].tExpTask_frames < 13)
        ++gTasks[taskId].tExpTask_frames;
    else
    {
        u32 monId = gTasks[taskId].tExpTask_monId;
        s16 gainedExp = gTasks[taskId].tExpTask_gainedExp;
        u32 battlerId = gTasks[taskId].tExpTask_battler;
        s16 newExpPoints = MoveBattleBar(battlerId, EXP_BAR);
        
        SetHealthboxSpriteVisibility(gHealthboxSpriteIds[battlerId], FALSE);
        
        if (newExpPoints == -1) // The bar has been filled with given exp points.
        {
            s32 currExp = GetMonData(&gPlayerParty[monId], MON_DATA_EXP);
            s32 expOnNextLvl = gExperienceTables[gSpeciesInfo[GetMonData(&gPlayerParty[monId], MON_DATA_SPECIES)].growthRate][GetMonData(&gPlayerParty[monId], MON_DATA_LEVEL) + 1];

            m4aSongNumStop(SE_EXP);

            if (currExp + gainedExp >= expOnNextLvl)
            {
                u32 oldMaxHP = GetMonData(&gPlayerParty[monId], MON_DATA_MAX_HP);
                
                SetMonData(&gPlayerParty[monId], MON_DATA_EXP, &expOnNextLvl);
                CalculateMonStats(&gPlayerParty[monId]);
                
                // Reapply Dynamax HP multiplier after stats are recalculated
                if (GetActiveGimmick(battlerId) == GIMMICK_DYNAMAX && monId == gBattlerPartyIndexes[battlerId])
                    DynamaxModifyHPLevelUp(&gPlayerParty[monId], battlerId, oldMaxHP);
                
                gainedExp -= expOnNextLvl - currExp;
                BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, RET_VALUE_LEVELED_UP, gainedExp);

                gTasks[taskId].func = Task_LaunchLvlUpAnim;
            }
            else
            {
                currExp += gainedExp;
                SetMonData(&gPlayerParty[monId], MON_DATA_EXP, &currExp);
                gBattlerControllersData[battlerId].func = CompleteOnInactiveTextPrinter;
                DestroyTask(taskId);
            }
        }
    }
}

static void Task_LaunchLvlUpAnim(u32 taskId)
{
    u32 battlerId = gTasks[taskId].tExpTask_battler;

    if (IsDoubleBattleForBattler(battlerId) && gTasks[taskId].tExpTask_monId == gBattlerPartyIndexes[BATTLE_PARTNER(battlerId)])
        battlerId = BATTLE_PARTNER(battlerId);
    
    InitAndLaunchSpecialAnimation(battlerId, battlerId, B_ANIM_LVL_UP);
    
    gTasks[taskId].func = Task_UpdateLvlInHealthbox;
}

static void Task_UpdateLvlInHealthbox(u32 taskId)
{
    u32 battlerId = gTasks[taskId].tExpTask_battler;

    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].specialAnimActive)
    {
        if (IsDoubleBattleForBattler(battlerId) && gTasks[taskId].tExpTask_monId == gBattlerPartyIndexes[BATTLE_PARTNER(battlerId)])
            UpdateHealthboxAttribute(BATTLE_PARTNER(battlerId), HEALTHBOX_ALL);
        else
            UpdateHealthboxAttribute(battlerId, HEALTHBOX_ALL);
        
        gTasks[taskId].func = Task_TryCreateVerticalStripes;
    }
}

static void Task_TryCreateVerticalStripes(u32 taskId)
{
    u32 battlerId = gTasks[taskId].tExpTask_battler;
    
    if (!((gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER) && GetBattlerPosition(battlerId) == B_POSITION_PLAYER_RIGHT) && IsBattlerSpriteVisible(battlerId))
        gTasks[taskId].func = Task_CreateLevelUpVerticalStripes;
    else
    {
        gBattlerControllersData[battlerId].func = CompleteOnInactiveTextPrinter;
        DestroyTask(taskId);
    }
}

static void Task_CreateLevelUpVerticalStripes(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    u32 battlerId = tExpTask_battler;
    bool32 isOnBg2 = (GetBattlerSpriteBGPriorityRank(battlerId) ^ 1);
    struct Sprite *sprite = &gSprites[gBattlerSpriteIds[battlerId]];

    switch (tExpTask_StateId)
    {
    case 0:
        if (!IsTextPrinterActive(0))
        {
            if (!isOnBg2)
            {
                tExpTask_BgX = gBattle_BG1_X;
                tExpTask_BgX = gBattle_BG1_Y;
                gBattle_BG1_X = -(sprite->x + sprite->x2) + 32;
                gBattle_BG1_Y = -(sprite->y + sprite->y2) + 32;
            }
            else
            {
                tExpTask_BgX = gBattle_BG2_X;
                tExpTask_BgY = gBattle_BG2_Y;
                gBattle_BG2_X = -(sprite->x + sprite->x2) + 32;
                gBattle_BG2_Y = -(sprite->y + sprite->y2) + 32;
            }
            ++tExpTask_StateId;
        }
        break;
    case 1:
        MoveBattlerSpriteToBG(battlerId, isOnBg2);
        ++tExpTask_StateId;
        break;
    case 2:
        PlaySE(SE_RS_SHOP);
        
        if (IsMonGettingExpSentOut())
            CreateLevelUpVerticalSpritesTask(sprite->x + sprite->x2, sprite->y + sprite->y2, 10000, 10000, 1, 0);
        ++tExpTask_StateId;
        break;
    case 3:
        if (!LevelUpVerticalSpritesTaskIsRunning())
        {
            sprite->invisible = FALSE;
            ++tExpTask_StateId;
        }
        break;
    case 4:
        ++tExpTask_StateId;
        break;
    case 5:
        ResetBattleAnimBg(isOnBg2);
        ++tExpTask_StateId;
        break;
    case 6:
        if (!isOnBg2)
        {
            gBattle_BG1_X = tExpTask_BgX;
            gBattle_BG1_Y = tExpTask_BgY;
        }
        else
        {
            gBattle_BG2_X = tExpTask_BgX;
            gBattle_BG2_Y = tExpTask_BgY;
        }
        gBattlerControllersData[battlerId].func = CompleteOnInactiveTextPrinter;
        DestroyTask(taskId);
        break;
    }
}

static void CompleteOnFinishedStatusAnimation(u32 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].statusAnimActive)
        BattleControllerComplete(battlerId);
}

static void SpriteCB_FreePlayerSpriteLoadMonSprite(struct Sprite *sprite)
{
    u32 battlerId = sprite->data[5];
    
    FreeSpritePaletteByTag(GetSpritePaletteTagByPaletteNum(sprite->oam.paletteNum));
    
    DestroySpriteAndFreeMatrix(sprite);
    BattleLoadMonSpriteGfx(battlerId);
    StartSpriteAnim(&gSprites[gBattlerSpriteIds[battlerId]], 0);
}

static void SpriteCB_FreeOpponentSprite(struct Sprite *sprite)
{
    FreeTrainerFrontPicPaletteAndTile(sprite->oam.affineParam);
    sprite->oam.tileNum = sprite->data[5];
    DestroySpriteAndFreeMatrix(sprite);
}

static void Task_StartSendOutAnim(u32 taskId)
{
    u32 battlerId = gTasks[taskId].tSendOutTask_Battler;
    
    if (gTasks[taskId].tSendOutTask_Delay && gTasks[taskId].tSendOutTask_Frames < gTasks[taskId].tSendOutTask_Delay)
        ++gTasks[taskId].tSendOutTask_Frames;
    else
    {
        if (!IsDoubleBattleForBattler(battlerId) || !IsPartnerMonFromSameTrainer(battlerId))
        {
            gBattleBufferA[battlerId][1] = gBattlerPartyIndexes[battlerId];
            StartSendOutAnim(battlerId, 0);
        }
        else
        {
            gBattleBufferA[battlerId][1] = gBattlerPartyIndexes[battlerId];
            StartSendOutAnim(battlerId, 0);
            battlerId = BATTLE_PARTNER(battlerId);
            gBattleBufferA[battlerId][1] = gBattlerPartyIndexes[battlerId];
            
            if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
                BattleLoadMonSpriteGfx(battlerId);
            
            StartSendOutAnim(battlerId, 0);
            battlerId = BATTLE_PARTNER(battlerId);
        }
        
        if (GetBattlerSide(battlerId) == B_SIDE_OPPONENT)
            gIntroSlideFlags &= ~(INTRO_SLIDE_FLAG_SLIDE_IN_STARTED);
        
        gBattlerControllersData[battlerId].func = (void*)(GetWordTaskArg(taskId, tSendOutTask_ControllerFunc));
        DestroyTask(taskId);
    }
}

void ShowHealthBox(u32 battlerId)
{
    DestroySprite(&gSprites[gBattlerControllersData[battlerId].data]);
    UpdateHealthboxAttribute(battlerId, HEALTHBOX_ALL);
    StartHealthboxSlideIn(battlerId);
    SetHealthboxSpriteVisibility(gHealthboxSpriteIds[battlerId], FALSE);
    SetBattlerShadowSpriteCallback(battlerId);
}

static void EndDrawPartyStatusSummary(u32 battlerId)
{
    if (gBattleSpritesDataPtr->healthBoxesData[battlerId].partyStatusDelayTimer++ > 92)
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].partyStatusDelayTimer = 0;
        BattleControllerComplete(battlerId);
    }
}

static void CompleteOnFinishedBattleAnimation(u32 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].animFromTableActive)
        BattleControllerComplete(battlerId);
}

static void StartSendOutAnim(u32 battlerId, u32 flags)
{
    bool32 isOpponent = (GetBattlerSide(battlerId) == B_SIDE_OPPONENT);
    
    ClearTemporarySpeciesSpriteData(battlerId, flags);
    gBattlerPartyIndexes[battlerId] = gBattleBufferA[battlerId][1];
    gBattlerControllersData[battlerId].data = CreateInvisibleSpriteWithCallback(SpriteCB_WaitForBattlerBallReleaseAnim);
    
    if (isOpponent)
        BattleLoadMonSpriteGfx(battlerId);
    
    SetMultiuseSpriteTemplateToPokemon(GetMonData(GetBattlerPartyIndexPtr(battlerId), MON_DATA_SPECIES), GetBattlerPosition(battlerId));
    gBattlerSpriteIds[battlerId] = CreateSprite(&gMultiuseSpriteTemplate, GetBattlerSpriteCoord(battlerId, BATTLER_COORD_X), GetBattlerSpriteDefault_Y(battlerId), GetBattlerSpriteSubpriority(battlerId));
    gSprites[gBattlerControllersData[battlerId].data].data[1] = gBattlerSpriteIds[battlerId];
    gSprites[gBattlerSpriteIds[battlerId]].data[0] = battlerId;
    gSprites[gBattlerSpriteIds[battlerId]].oam.paletteNum = battlerId;
    StartSpriteAnim(&gSprites[gBattlerSpriteIds[battlerId]], 0);
    gSprites[gBattlerSpriteIds[battlerId]].invisible = TRUE;
    gSprites[gBattlerSpriteIds[battlerId]].callback = SpriteCallbackDummy;
    gSprites[gBattlerControllersData[battlerId].data].data[0] = 0;
    DoPokeballSendOutAnimation(battlerId, isOpponent ? POKEBALL_OPPONENT_SENDOUT : POKEBALL_PLAYER_SENDOUT);
}

#undef tExpTask_monId
#undef tExpTask_gainedExp
#undef tExpTask_battler
#undef tExpTask_frames
#undef tExpTask_StateId
#undef tExpTask_BgX
#undef tExpTask_BgY

#undef tSendOutTask_Battler
#undef tSendOutTask_Frames
#undef tSendOutTask_Delay
#undef tSendOutTask_ControllerFunc

void Intro_DelayAndEnd(u32 battlerId)
{
    if (--gBattleSpritesDataPtr->healthBoxesData[battlerId].introEndDelay == 0xFF)
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].introEndDelay = 0;
        BattleControllerComplete(battlerId);
    }
}

static void Intro_WaitForShinyAnimAndHealthbox(u32 battlerId)
{
    bool32 var = FALSE;

    if (!IsDoubleBattleForBattler(battlerId) || !IsPartnerMonFromSameTrainer(battlerId))
    {
        if (gSprites[gHealthboxSpriteIds[battlerId]].callback == SpriteCallbackDummy)
            var = TRUE;
    }
    else
    {
        if (gSprites[gHealthboxSpriteIds[battlerId]].callback == SpriteCallbackDummy && gSprites[gHealthboxSpriteIds[BATTLE_PARTNER(battlerId)]].callback == SpriteCallbackDummy)
            var = TRUE;
    }
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
        
        HandleLowHpMusicChange(battlerId, GetBattlerPartyIndexPtr(battlerId));
        
        if (IsDoubleBattleForBattler(battlerId))
            HandleLowHpMusicChange(BATTLE_PARTNER(battlerId), GetBattlerPartyIndexPtr(BATTLE_PARTNER(battlerId)));
        
        if ((gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE) && GetBattlerPosition(battlerId) == B_POSITION_PLAYER_LEFT)
            gBattlerControllersData[battlerId].func = PrintOakText_ForPetesSake;
        else
        {
            if (!(gBattleTypeFlags & BATTLE_TYPE_POKEDUDE))
                gBattleSpritesDataPtr->healthBoxesData[battlerId].introEndDelay = 3;
            
            gBattlerControllersData[battlerId].func = Intro_DelayAndEnd;
        }
    }
}

void Intro_TryShinyAnimShowHealthbox(u32 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive && !gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].ballAnimActive)
    {
        if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim)
            TryShinyAnimation(battlerId);
        
        if (!gBattleSpritesDataPtr->healthBoxesData[BATTLE_PARTNER(battlerId)].triedShinyMonAnim)
            TryShinyAnimation(BATTLE_PARTNER(battlerId));
        
        if (IsDoubleBattleForBattler(battlerId) && IsPartnerMonFromSameTrainer(battlerId))
            ShowHealthBox(BATTLE_PARTNER(battlerId));
        
        ShowHealthBox(battlerId);
        gBattleSpritesDataPtr->animationData->healthboxSlideInStarted = FALSE;
        gBattlerControllersData[battlerId].func = Intro_WaitForShinyAnimAndHealthbox;
    }
}

void WaitForMonSelection(u32 battlerId)
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

static void SwitchIn_HandleSoundAndEnd(u32 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].specialAnimActive && !IsCryPlayingOrClearCrySongs())
    {
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 0x100);
        BattleControllerComplete(battlerId);
    }
}

static void SwitchIn_ShowSubstitute(u32 battlerId)
{
    if (gSprites[gHealthboxSpriteIds[battlerId]].callback == SpriteCallbackDummy)
    {
        if (gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute)
            InitAndLaunchSpecialAnimation(battlerId, battlerId, B_ANIM_MON_TO_SUBSTITUTE);
        
        gBattlerControllersData[battlerId].func = SwitchIn_HandleSoundAndEnd;
    }
}

static void SwitchIn_ShowHealthbox(u32 battlerId)
{
    if (!gBattlerControllersData[battlerId].data && gSprites[gHealthboxSpriteIds[battlerId]].callback != SpriteCallbackDummy)
        return;

    if (gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim)
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim = FALSE;
        gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim = FALSE;
        
        FreeSpriteTilesByTag(ANIM_TAG_GOLD_STARS);
        FreeSpritePaletteByTag(ANIM_TAG_GOLD_STARS);
        
        HandleLowHpMusicChange(battlerId, GetBattlerPartyIndexPtr(battlerId));
        
        if (gBattlerControllersData[battlerId].data)
        {
            StartSpriteAnim(&gSprites[gBattlerSpriteIds[battlerId]], 0);
            
            UpdateHealthboxAttribute(battlerId, HEALTHBOX_ALL);
            StartHealthboxSlideIn(battlerId);
            SetHealthboxSpriteVisibility(gHealthboxSpriteIds[battlerId], FALSE);
            
            CopyBattleSpriteInvisibility(battlerId);
            
            gBattlerControllersData[battlerId].func = SwitchIn_ShowSubstitute;
        }
        else
        {
            if (gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute)
                InitAndLaunchSpecialAnimation(battlerId, battlerId, B_ANIM_MON_TO_SUBSTITUTE);
                
            gBattlerControllersData[battlerId].func = SwitchIn_HandleSoundAndEnd;
        }
    }
}

void SwitchIn_TryShinyAnim(u32 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim && !gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive)
        TryShinyAnimation(battlerId);
    
    if (gSprites[gBattlerControllersData[battlerId].data].callback == SpriteCallbackDummy && !gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive)
    {
        DestroySprite(&gSprites[gBattlerControllersData[battlerId].data]);
        SetBattlerShadowSpriteCallback(battlerId);
        
        gBattlerControllersData[battlerId].data = TRUE;
        gBattlerControllersData[battlerId].func = SwitchIn_ShowHealthbox;
    }
}

void SwitchIn_TryShinyAnimShowHealthbox(u32 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim && !gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive)
        TryShinyAnimation(battlerId);
    
    if (gSprites[gBattlerControllersData[battlerId].data].callback == SpriteCallbackDummy && !gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive)
    {
        DestroySprite(&gSprites[gBattlerControllersData[battlerId].data]);
        
        UpdateHealthboxAttribute(battlerId, HEALTHBOX_ALL);
        StartHealthboxSlideIn(battlerId);
        SetHealthboxSpriteVisibility(gHealthboxSpriteIds[battlerId], FALSE);
        CopyBattleSpriteInvisibility(battlerId);
        
        gBattlerControllersData[battlerId].data = FALSE;
        gBattlerControllersData[battlerId].func = SwitchIn_ShowHealthbox;
    }
}

static void CalculateMonStatsAfterChangeForm(struct Pokemon *mon)
{
    u32 newHP = GetMonData(mon, MON_DATA_HP);
    CalculateMonStats(mon);
    newHP = min(GetMonData(mon, MON_DATA_MAX_HP), newHP);
    SetMonData(mon, MON_DATA_HP, &newHP);
}

void BattleCreateYesNoCursorAt(u32 pos)
{
    u16 src[2] = {1, 2};
    CopyToBgTilemapBufferRect_ChangePalette(0, src, 0x18, 9 + (2 * pos), 1, 2, 0x11);
    CopyBgTilemapBufferToVram(0);
}

void BattleDestroyYesNoCursorAt(u32 pos)
{
    u16 src[2] = {32, 32};
    CopyToBgTilemapBufferRect_ChangePalette(0, src, 0x18, 9 + (2 * pos), 1, 2, 0x11);
    CopyBgTilemapBufferToVram(0);
}

void HandleBattleWindow(u32 xStart, u32 yStart, u32 xEnd, u32 yEnd, u32 flags)
{
    u16 var = 0;
    s32 destY, destX;

    for (destY = yStart; destY <= yEnd; ++destY)
    {
        for (destX = xStart; destX <= xEnd; ++destX)
        {
            if (destY == yStart)
            {
                if (destX == xStart)
                    var = 0x1022;
                else if (destX == xEnd)
                    var = 0x1024;
                else
                    var = 0x1023;
            }
            else if (destY == yEnd)
            {
                if (destX == xStart)
                    var = 0x1028;
                else if (destX == xEnd)
                    var = 0x102A;
                else
                    var = 0x1029;
            }
            else
            {
                if (destX == xStart)
                    var = 0x1025;
                else if (destX == xEnd)
                    var = 0x1027;
                else
                    var = 0x1026;
            }
            if (flags & WINDOW_CLEAR)
                var = 0;
            
            if (flags & WINDOW_x80)
                CopyToBgTilemapBufferRect_ChangePalette(1, &var, destX, destY, 1, 1, 0x11);
            else
                CopyToBgTilemapBufferRect_ChangePalette(0, &var, destX, destY, 1, 1, 0x11);
        }
    }
    CopyBgTilemapBufferToVram(1);
}

static void CreateYesNoBoxOnInactiveTextPrinter(u32 battlerId)
{
    if (!IsTextPrinterActive(0))
        gBattlerControllersData[battlerId].func = CreateYesNoBox;
}

static void CreateYesNoBox(u32 battlerId)
{
    HandleBattleWindow(0x17, 8, 0x1D, 0xD, 0);
    BattlePutTextOnWindow(gText_BattleYesNoChoice, B_WIN_YESNO);
    gBattlerControllersData[battlerId].data = 0;
    BattleCreateYesNoCursorAt(gBattlerControllersData[battlerId].data);
    gBattlerControllersData[battlerId].func = HandleYesNoBoxInput;
}

static void HandleYesNoBoxInput(u32 battlerId)
{
    bool32 selectedOption = FALSE;
    
    if (JOY_NEW(DPAD_UP) && gBattlerControllersData[battlerId].data)
    {
        PlaySE(SE_SELECT);
        BattleDestroyYesNoCursorAt(gBattlerControllersData[battlerId].data);
        BattleCreateYesNoCursorAt(--gBattlerControllersData[battlerId].data);
    }
    if (JOY_NEW(DPAD_DOWN) && gBattlerControllersData[battlerId].data == 0)
    {
        PlaySE(SE_SELECT);
        BattleDestroyYesNoCursorAt(gBattlerControllersData[battlerId].data);
        BattleCreateYesNoCursorAt(++gBattlerControllersData[battlerId].data);
    }
    
    if (JOY_NEW(B_BUTTON))
    {
        gBattlerControllersData[battlerId].data = 1;
        PlaySE(SE_SELECT);
        HandleBattleWindow(0x17, 8, 0x1D, 0xD, WINDOW_CLEAR);
        selectedOption = TRUE;
    }
    else if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        HandleBattleWindow(0x17, 8, 0x1D, 0xD, WINDOW_CLEAR);
        selectedOption = TRUE;
    }
    
    if (selectedOption)
    {
        BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, gBattlerControllersData[battlerId].data, 0);
        BattleControllerComplete(battlerId);
    }
}

#include "global.h"
#include "gflib.h"
#include "battle_controllers.h"
#include "battle.h"
#include "battle_interface.h"
#include "battle_gfx_sfx_util.h"
#include "battle_main.h"
#include "battle_ai.h"
#include "battle_anim.h"
#include "battle_util.h"
#include "battle_message.h"
#include "data.h"
#include "link.h"
#include "link_rfu.h"
#include "strings.h"
#include "cable_club.h"
#include "m4a.h"
#include "pokemon_special_anim.h"
#include "party_menu.h"
#include "palette.h"
#include "pokeball.h"
#include "task.h"
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
static void Task_HandleSendLinkBuffersData(u8 taskId);
static void Task_HandleCopyReceivedLinkBuffersData(u8 taskId);
static void SetBattlePartyIds(void);
static void InitSinglePlayerBtlControllers(void);
static void InitLinkBtlControllers(void);
static u32 GetBattlerMonData(u8 battlerId, struct Pokemon *party, u8 monId, u8 *dst);
static void SetBattlerMonData(u8 battlerId, struct Pokemon *party, u8 monId);
static void DoSwitchOutAnimation(u8 battlerId);
static void FreeMonSprite(u8 battlerId);
static void CompleteOnBattlerSpriteCallbackDummy(u8 battlerId);
static void FreeTrainerSpriteAfterSlide(u8 battlerId);
static void FreeMonSpriteAfterFaintAnim(u8 battlerId);
static void HideHealthboxAfterMonFaint(u8 battlerId);
static void CompleteOnSpecialAnimDone(u8 battlerId);
static void DoMoveAnimation(u8 battlerId);
static void CompleteOnInactiveTextPrinter(u8 battlerId);
static void CompleteOnHealthbarDone(u8 battlerId);
static void Task_GiveExpToMon(u8 taskId);
static void Task_PrepareToGiveExpWithExpBar(u8 taskId);
static void Task_GiveExpWithExpBar(u8 taskId);
static void Task_LaunchLvlUpAnim(u8 taskId);
static void Task_UpdateLvlInHealthbox(u8 taskId);
static void Task_TryCreateVerticalStripes(u8 taskId);
static void Task_CreateLevelUpVerticalStripes(u8 taskId);
static void CompleteOnFinishedStatusAnimation(u8 battlerId);
static void SpriteCB_FreePlayerSpriteLoadMonSprite(struct Sprite *sprite);
static void SpriteCB_FreeOpponentSprite(struct Sprite *sprite);
static void Task_StartSendOutAnim(u8 taskId);
static void EndDrawPartyStatusSummary(u8 battlerId);
static void CompleteOnFinishedBattleAnimation(u8 battlerId);
static void StartSendOutAnim(u8 battlerId, bool8 dontClearSubstituteBit);

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

static void Task_HandleSendLinkBuffersData(u8 taskId)
{
    u16 blockSize;
    s32 playerCount;

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
        playerCount = GetLinkPlayerCount_2();
		
        if ((gBattleTypeFlags & BATTLE_TYPE_MULTI && playerCount > 3) || (!(gBattleTypeFlags & BATTLE_TYPE_MULTI) && playerCount > 1))
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
                SendBlock(bitmask_all_link_players_but_self(), &gLinkBattleSendBuffer[gTasks[taskId].data[15]], blockSize);
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

static void Task_HandleCopyReceivedLinkBuffersData(u8 taskId)
{
    u16 blockSize;
    u8 battlerId;

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
            if (gBattleControllerExecFlags & gBitTable[battlerId])
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
            gBattleControllerExecFlags &= ~(gBitTable[battlerId] << (gLinkBattleRecvBuffer[gTasks[taskId].data[15] + LINK_BUFF_DATA] * 4));
            break;
        }
        gTasks[taskId].data[15] = gTasks[taskId].data[15] + blockSize + LINK_BUFF_DATA;
    }
}

void TryReceiveLinkBattleData(void)
{
    u8 i, *recvBuffer;
    s32 j;

    if (gReceivedRemoteLinkPlayers && (gBattleTypeFlags & BATTLE_TYPE_LINK_ESTABLISHED) && (gLinkPlayers[0].linkType == 0x2211))
    {
        LinkRfu_DestroyIdleTask();
		
        for (i = 0; i < GetLinkPlayerCount(); ++i)
        {
            if (GetBlockReceivedStatus() & gBitTable[i])
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
    u8 i;

    gBattleMainFunc = BattleDummy;
	
    for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
    {
        gBattlerControllerFuncs[i] = ControllerDummy;
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
    u8 i;

    if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        InitLinkBtlControllers();
    else
        InitSinglePlayerBtlControllers();
	
    SetBattlePartyIds();
	
    if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
	{
        for (i = 0; i < gBattlersCount; ++i)
            BufferBattlePartyCurrentOrderBySide(i, 0);
	}
}

static void SetLinkBattleEndCallbacks(u8 battlerId)
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

void SetBattleEndCallbacks(u8 battlerId)
{
	if (!gPaletteFade.active)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_LINK)
        {
            if (!gWirelessCommType)
                SetCloseLinkCallback();
            else
                SetLinkStandbyCallback();
			
            gBattlerControllerFuncs[battlerId] = SetLinkBattleEndCallbacks;
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
    u8 i, j;
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

static void InitSinglePlayerBtlControllers(void)
{
	gBattleMainFunc = BeginBattleIntro;
	
    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
    {
        if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
        {
            gBattlerControllerFuncs[0] = SetControllerToPokedude;
            gBattlerPositions[0] = B_POSITION_PLAYER_LEFT;
            gBattlerControllerFuncs[1] = SetControllerToPokedude;
            gBattlerPositions[1] = B_POSITION_OPPONENT_LEFT;
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
                gBattlerControllerFuncs[0] = SetControllerToSafari;
            else if (gBattleTypeFlags & (BATTLE_TYPE_OLD_MAN_TUTORIAL | BATTLE_TYPE_FIRST_BATTLE))
                gBattlerControllerFuncs[0] = SetControllerToOakOrOldMan;
            else
                gBattlerControllerFuncs[0] = SetControllerToPlayer;
			
            gBattlerPositions[0] = B_POSITION_PLAYER_LEFT;
            gBattlerControllerFuncs[1] = SetControllerToOpponent;
            gBattlerPositions[1] = B_POSITION_OPPONENT_LEFT;
        }
		gBattlersCount = 2;
    }
    else
    {
        if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
        {
            gBattlerControllerFuncs[0] = SetControllerToPokedude;
            gBattlerPositions[0] = B_POSITION_PLAYER_LEFT;
            gBattlerControllerFuncs[1] = SetControllerToPokedude;
            gBattlerPositions[1] = B_POSITION_OPPONENT_LEFT;
            gBattlerControllerFuncs[2] = SetControllerToPokedude;
            gBattlerPositions[2] = B_POSITION_PLAYER_RIGHT;
            gBattlerControllerFuncs[3] = SetControllerToPokedude;
            gBattlerPositions[3] = B_POSITION_OPPONENT_RIGHT;
        }
        else
        {
            gBattlerControllerFuncs[0] = SetControllerToPlayer;
            gBattlerPositions[0] = B_POSITION_PLAYER_LEFT;
            gBattlerControllerFuncs[1] = SetControllerToOpponent;
            gBattlerPositions[1] = B_POSITION_OPPONENT_LEFT;
            gBattlerControllerFuncs[2] = SetControllerToPlayer;
            gBattlerPositions[2] = B_POSITION_PLAYER_RIGHT;
            gBattlerControllerFuncs[3] = SetControllerToOpponent;
            gBattlerPositions[3] = B_POSITION_OPPONENT_RIGHT;
        }
		gBattlersCount = MAX_BATTLERS_COUNT;
    }
}

static void InitLinkBtlControllers(void)
{
    u8 i, multiplayerId;

    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
    {
        if (gBattleTypeFlags & BATTLE_TYPE_IS_MASTER)
        {
            gBattleMainFunc = BeginBattleIntro;
            gBattlerControllerFuncs[0] = SetControllerToPlayer;
            gBattlerPositions[0] = B_POSITION_PLAYER_LEFT;
            gBattlerControllerFuncs[1] = SetControllerToLinkOpponent;
            gBattlerPositions[1] = B_POSITION_OPPONENT_LEFT;
        }
        else
        {
            gBattlerControllerFuncs[1] = SetControllerToPlayer;
            gBattlerPositions[1] = B_POSITION_PLAYER_LEFT;
            gBattlerControllerFuncs[0] = SetControllerToLinkOpponent;
            gBattlerPositions[0] = B_POSITION_OPPONENT_LEFT;
        }
		gBattlersCount = 2;
    }
    else if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI) && (gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
    {
        if (gBattleTypeFlags & BATTLE_TYPE_IS_MASTER)
        {
            gBattleMainFunc = BeginBattleIntro;
            gBattlerControllerFuncs[0] = SetControllerToPlayer;
            gBattlerPositions[0] = B_POSITION_PLAYER_LEFT;
            gBattlerControllerFuncs[1] = SetControllerToLinkOpponent;
            gBattlerPositions[1] = B_POSITION_OPPONENT_LEFT;
            gBattlerControllerFuncs[2] = SetControllerToPlayer;
            gBattlerPositions[2] = B_POSITION_PLAYER_RIGHT;
            gBattlerControllerFuncs[3] = SetControllerToLinkOpponent;
            gBattlerPositions[3] = B_POSITION_OPPONENT_RIGHT;
        }
        else
        {
            gBattlerControllerFuncs[1] = SetControllerToPlayer;
            gBattlerPositions[1] = B_POSITION_PLAYER_LEFT;
            gBattlerControllerFuncs[0] = SetControllerToLinkOpponent;
            gBattlerPositions[0] = B_POSITION_OPPONENT_LEFT;
            gBattlerControllerFuncs[3] = SetControllerToPlayer;
            gBattlerPositions[3] = B_POSITION_PLAYER_RIGHT;
            gBattlerControllerFuncs[2] = SetControllerToLinkOpponent;
            gBattlerPositions[2] = B_POSITION_OPPONENT_RIGHT;
        }
		gBattlersCount = MAX_BATTLERS_COUNT;
    }
    else
    {
        multiplayerId = GetMultiplayerId();
		
        if (gBattleTypeFlags & BATTLE_TYPE_IS_MASTER)
            gBattleMainFunc = BeginBattleIntro;
		
        for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
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
			
            if (i == multiplayerId)
            {
                gBattlerControllerFuncs[gLinkPlayers[i].id] = SetControllerToPlayer;
				
                switch (gLinkPlayers[i].id)
                {
                case 0:
                case 3:
                    gBattlerPositions[gLinkPlayers[i].id] = B_POSITION_PLAYER_LEFT;
                    gBattlerPartyIndexes[gLinkPlayers[i].id] = B_POSITION_PLAYER_LEFT;
                    break;
                case 1:
                case 2:
                    gBattlerPositions[gLinkPlayers[i].id] = B_POSITION_PLAYER_RIGHT;
                    gBattlerPartyIndexes[gLinkPlayers[i].id] = 3;
                    break;
                }
            }
            else
            {
                if ((!(gLinkPlayers[i].id & 1) && !(gLinkPlayers[multiplayerId].id & 1)) || ((gLinkPlayers[i].id & 1) && (gLinkPlayers[multiplayerId].id & 1)))
                {
                    gBattlerControllerFuncs[gLinkPlayers[i].id] = SetControllerToLinkPartner;
					
                    switch (gLinkPlayers[i].id)
                    {
                    case 0:
                    case 3:
                        gBattlerPositions[gLinkPlayers[i].id] = B_POSITION_PLAYER_LEFT;
                        gBattlerPartyIndexes[gLinkPlayers[i].id] = B_POSITION_PLAYER_LEFT;
                        break;
                    case 1:
                    case 2:
                        gBattlerPositions[gLinkPlayers[i].id] = B_POSITION_PLAYER_RIGHT;
                        gBattlerPartyIndexes[gLinkPlayers[i].id] = B_POSITION_OPPONENT_RIGHT;
                        break;
                    }
                }
                else
                {
                    gBattlerControllerFuncs[gLinkPlayers[i].id] = SetControllerToLinkOpponent;
					
                    switch (gLinkPlayers[i].id)
                    {
                    case 0:
                    case 3:
                        gBattlerPositions[gLinkPlayers[i].id] = B_POSITION_OPPONENT_LEFT;
                        gBattlerPartyIndexes[gLinkPlayers[i].id] = B_POSITION_PLAYER_LEFT;
                        break;
                    case 1:
                    case 2:
                        gBattlerPositions[gLinkPlayers[i].id] = B_POSITION_OPPONENT_RIGHT;
                        gBattlerPartyIndexes[gLinkPlayers[i].id] = B_POSITION_OPPONENT_RIGHT;
                        break;
                    }
                }
            }
        }
        gBattlersCount = MAX_BATTLERS_COUNT;
    }
}

/////////////////////////
// CONTROLLER EMITTERS //
/////////////////////////

void PrepareBufferDataTransferLink(u8 battlerId, u8 bufferId, u16 size, u8 *data)
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

static void PrepareBufferDataTransfer(u8 battlerId, u8 bufferId, u8 *data, u16 size)
{
    s32 i;

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

void BtlController_EmitGetMonData(u8 battlerId, u8 bufferId, u8 requestId, u8 monToCheck)
{
    sBattleBuffersTransferData[0] = CONTROLLER_GETMONDATA;
    sBattleBuffersTransferData[1] = requestId;
    sBattleBuffersTransferData[2] = monToCheck;
    sBattleBuffersTransferData[3] = 0;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitSetMonData(u8 battlerId, u8 bufferId, u8 requestId, u8 monToCheck, u8 bytes, void *data)
{
	s32 i;
    sBattleBuffersTransferData[0] = CONTROLLER_SETMONDATA;
    sBattleBuffersTransferData[1] = requestId;
    sBattleBuffersTransferData[2] = monToCheck;
    for (i = 0; i < bytes; ++i)
        sBattleBuffersTransferData[3 + i] = *(u8 *)(data++);
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 3 + bytes);
}

void BtlController_EmitSetRawMonData(u8 battlerId, u8 bufferId, u8 offset, u8 bytes, void *data)
{
	s32 i;
    sBattleBuffersTransferData[0] = CONTROLLER_SETRAWMONDATA;
    sBattleBuffersTransferData[1] = offset;
    sBattleBuffersTransferData[2] = bytes;
    for (i = 0; i < bytes; ++i)
        sBattleBuffersTransferData[3 + i] = *(u8 *)(data++);
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, bytes + 3);
}

void BtlController_EmitLoadMonSprite(u8 battlerId, u8 bufferId)
{
	sBattleBuffersTransferData[0] = CONTROLLER_LOADMONSPRITE;
    sBattleBuffersTransferData[1] = CONTROLLER_LOADMONSPRITE;
    sBattleBuffersTransferData[2] = CONTROLLER_LOADMONSPRITE;
    sBattleBuffersTransferData[3] = CONTROLLER_LOADMONSPRITE;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitSwitchInAnim(u8 battlerId, u8 bufferId, u8 partyId, bool8 dontClearSubstituteBit)
{
	sBattleBuffersTransferData[0] = CONTROLLER_SWITCHINANIM;
    sBattleBuffersTransferData[1] = partyId;
    sBattleBuffersTransferData[2] = dontClearSubstituteBit;
    sBattleBuffersTransferData[3] = 5;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitReturnMonToBall(u8 battlerId, u8 bufferId, u8 arg1)
{
    sBattleBuffersTransferData[0] = CONTROLLER_RETURNMONTOBALL;
    sBattleBuffersTransferData[1] = arg1;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 2);
}

void BtlController_EmitDrawTrainerPic(u8 battlerId, u8 bufferId)
{
	sBattleBuffersTransferData[0] = CONTROLLER_DRAWTRAINERPIC;
    sBattleBuffersTransferData[1] = CONTROLLER_DRAWTRAINERPIC;
    sBattleBuffersTransferData[2] = CONTROLLER_DRAWTRAINERPIC;
    sBattleBuffersTransferData[3] = CONTROLLER_DRAWTRAINERPIC;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitTrainerSlide(u8 battlerId, u8 bufferId)
{
	sBattleBuffersTransferData[0] = CONTROLLER_TRAINERSLIDE;
    sBattleBuffersTransferData[1] = CONTROLLER_TRAINERSLIDE;
    sBattleBuffersTransferData[2] = CONTROLLER_TRAINERSLIDE;
    sBattleBuffersTransferData[3] = CONTROLLER_TRAINERSLIDE;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitTrainerSlideBack(u8 battlerId, u8 bufferId)
{
	sBattleBuffersTransferData[0] = CONTROLLER_TRAINERSLIDEBACK;
    sBattleBuffersTransferData[1] = CONTROLLER_TRAINERSLIDEBACK;
    sBattleBuffersTransferData[2] = CONTROLLER_TRAINERSLIDEBACK;
    sBattleBuffersTransferData[3] = CONTROLLER_TRAINERSLIDEBACK;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitFaintAnimation(u8 battlerId, u8 bufferId)
{
	sBattleBuffersTransferData[0] = CONTROLLER_FAINTANIMATION;
    sBattleBuffersTransferData[1] = CONTROLLER_FAINTANIMATION;
    sBattleBuffersTransferData[2] = CONTROLLER_FAINTANIMATION;
    sBattleBuffersTransferData[3] = CONTROLLER_FAINTANIMATION;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitBallThrowAnim(u8 battlerId, u8 bufferId, u8 caseId)
{
	sBattleBuffersTransferData[0] = CONTROLLER_BALLTHROWANIM;
    sBattleBuffersTransferData[1] = caseId;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 2);
}

void BtlController_EmitMoveAnimation(u8 battlerId, u8 bufferId, u16 move, u8 turnOfMove, u16 movePower, s32 dmg, u8 friendship, struct DisableStruct *disableStructPtr)
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
    sBattleBuffersTransferData[11] = gMultiHitCounter; // multihit in pokeem
    if (WEATHER_HAS_EFFECT)
    {
        sBattleBuffersTransferData[12] = gBattleWeather;
        sBattleBuffersTransferData[13] = (gBattleWeather & 0xFF00) >> 8;
    }
    else
    {
        sBattleBuffersTransferData[12] = 0;
        sBattleBuffersTransferData[13] = 0;
    }
    sBattleBuffersTransferData[14] = 0;
    sBattleBuffersTransferData[15] = 0;
    memcpy(&sBattleBuffersTransferData[16], disableStructPtr, sizeof(struct DisableStruct));
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 16 + sizeof(struct DisableStruct));
}

void BtlController_EmitPrintString(u8 battlerId, u8 bufferId, u16 stringId)
{
	s32 i;
    struct BattleMsgData *stringInfo;

    sBattleBuffersTransferData[0] = CONTROLLER_PRINTSTRING;
    sBattleBuffersTransferData[1] = gBattleOutcome;
    sBattleBuffersTransferData[2] = stringId;
    sBattleBuffersTransferData[3] = (stringId & 0xFF00) >> 8;
	
    stringInfo = (struct BattleMsgData *)(&sBattleBuffersTransferData[4]);
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
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, sizeof(struct BattleMsgData) + 4);
}

void BtlController_EmitPrintSelectionString(u8 battlerId, u8 bufferId, u16 stringId)
{
	s32 i;
    struct BattleMsgData *stringInfo;

    sBattleBuffersTransferData[0] = CONTROLLER_PRINTSELECTIONSTRING;
    sBattleBuffersTransferData[1] = CONTROLLER_PRINTSELECTIONSTRING;
    sBattleBuffersTransferData[2] = stringId;
    sBattleBuffersTransferData[3] = (stringId & 0xFF00) >> 8;
	
    stringInfo = (struct BattleMsgData *)(&sBattleBuffersTransferData[4]);
    stringInfo->currentMove = gCurrentMove;
    stringInfo->chosenMove = gChosenMove;
    stringInfo->lastItem = gLastUsedItem;
    stringInfo->scrActive = gBattleScripting.battler;
    for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
        stringInfo->abilities[i] = gBattleMons[i].ability;
    for (i = 0; i < TEXT_BUFF_ARRAY_COUNT; ++i)
    {
        stringInfo->textBuffs[0][i] = gBattleTextBuff1[i];
        stringInfo->textBuffs[1][i] = gBattleTextBuff2[i];
        stringInfo->textBuffs[2][i] = gBattleTextBuff3[i];
    }
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, sizeof(struct BattleMsgData) + 4);
}

void BtlController_EmitChooseAction(u8 battlerId, u8 bufferId, u8 arg1, u16 arg2)
{
	sBattleBuffersTransferData[0] = CONTROLLER_CHOOSEACTION;
    sBattleBuffersTransferData[1] = arg1;
    sBattleBuffersTransferData[2] = arg2;
    sBattleBuffersTransferData[3] = (arg2 & 0xFF00) >> 8;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitChooseMove(u8 battlerId, u8 bufferId, bool8 isDoubleBattle, bool8 NoPpNumber, struct ChooseMoveStruct *movePpData)
{
	s32 i;
    sBattleBuffersTransferData[0] = CONTROLLER_CHOOSEMOVE;
    sBattleBuffersTransferData[1] = isDoubleBattle;
    sBattleBuffersTransferData[2] = NoPpNumber;
    sBattleBuffersTransferData[3] = 0;
    for (i = 0; i < sizeof(*movePpData); ++i)
        sBattleBuffersTransferData[4 + i] = *((u8 *)(movePpData) + i);
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, sizeof(*movePpData) + 4);
}

void BtlController_EmitChooseItem(u8 battlerId, u8 bufferId, u8 *arg1)
{
	s32 i;
    sBattleBuffersTransferData[0] = CONTROLLER_OPENBAG;
    for (i = 0; i < 3; ++i)
        sBattleBuffersTransferData[1 + i] = arg1[i];
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitChoosePokemon(u8 battlerId, u8 bufferId, u8 caseId, u8 arg2, u8* arg4)
{
	s32 i;
    sBattleBuffersTransferData[0] = CONTROLLER_CHOOSEPOKEMON;
    sBattleBuffersTransferData[1] = caseId;
    sBattleBuffersTransferData[2] = arg2;
    for (i = 0; i < 3; ++i)
        sBattleBuffersTransferData[3 + i] = arg4[i];
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 8);
}

void BtlController_EmitHealthBarUpdate(u8 battlerId, u8 bufferId, u16 hpValue)
{
	sBattleBuffersTransferData[0] = CONTROLLER_HEALTHBARUPDATE;
    sBattleBuffersTransferData[1] = 0;
    sBattleBuffersTransferData[2] = (s16)hpValue;
    sBattleBuffersTransferData[3] = ((s16)hpValue & 0xFF00) >> 8;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitExpUpdate(u8 battlerId, u8 bufferId, u8 partyId, u16 expPoints)
{
	sBattleBuffersTransferData[0] = CONTROLLER_EXPUPDATE;
    sBattleBuffersTransferData[1] = partyId;
    sBattleBuffersTransferData[2] = (s16)expPoints;
    sBattleBuffersTransferData[3] = ((s16)expPoints & 0xFF00) >> 8;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitStatusIconUpdate(u8 battlerId, u8 bufferId, u8 status1Id, u32 status2)
{
	sBattleBuffersTransferData[0] = CONTROLLER_STATUSICONUPDATE;
    sBattleBuffersTransferData[1] = status1Id;
    sBattleBuffersTransferData[2] = status2;
    sBattleBuffersTransferData[3] = (status2 & 0x0000FF00) >> 8;
    sBattleBuffersTransferData[4] = (status2 & 0x00FF0000) >> 16;
    sBattleBuffersTransferData[5] = (status2 & 0xFF000000) >> 24;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 6);
}

void BtlController_EmitStatusAnimation(u8 battlerId, u8 bufferId, u8 animStatusId, u32 status)
{
	sBattleBuffersTransferData[0] = CONTROLLER_STATUSANIMATION;
    sBattleBuffersTransferData[1] = animStatusId;
    sBattleBuffersTransferData[2] = status;
    sBattleBuffersTransferData[3] = (status & 0x0000FF00) >> 8;
    sBattleBuffersTransferData[4] = (status & 0x00FF0000) >> 16;
    sBattleBuffersTransferData[5] = (status & 0xFF000000) >> 24;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 6);
}

void BtlController_EmitDataTransfer(u8 battlerId, u8 bufferId, u16 size, void *data)
{
	s32 i;
    sBattleBuffersTransferData[0] = CONTROLLER_DATATRANSFER;
    sBattleBuffersTransferData[1] = CONTROLLER_DATATRANSFER;
    sBattleBuffersTransferData[2] = size;
    sBattleBuffersTransferData[3] = (size & 0xFF00) >> 8;
    for (i = 0; i < size; ++i)
        sBattleBuffersTransferData[4 + i] = *(u8 *)(data++);
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, size + 4);
}

void BtlController_EmitTwoReturnValues(u8 battlerId, u8 bufferId, u8 arg1, u16 arg2)
{
    sBattleBuffersTransferData[0] = CONTROLLER_TWORETURNVALUES;
    sBattleBuffersTransferData[1] = arg1;
    sBattleBuffersTransferData[2] = arg2;
    sBattleBuffersTransferData[3] = (arg2 & 0xFF00) >> 8;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitChosenMonReturnValue(u8 battlerId, u8 bufferId, u8 b, u8 *c)
{
	s32 i;
    sBattleBuffersTransferData[0] = CONTROLLER_CHOSENMONRETURNVALUE;
    sBattleBuffersTransferData[1] = b;
    for (i = 0; i < 3; ++i)
        sBattleBuffersTransferData[2 + i] = c[i];
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 5);
}

void BtlController_EmitOneReturnValue(u8 battlerId, u8 bufferId, u16 arg1)
{
	sBattleBuffersTransferData[0] = CONTROLLER_ONERETURNVALUE;
    sBattleBuffersTransferData[1] = arg1;
    sBattleBuffersTransferData[2] = (arg1 & 0xFF00) >> 8;
    sBattleBuffersTransferData[3] = 0;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitPlaySE(u8 battlerId, u8 bufferId, u16 songId)
{
	sBattleBuffersTransferData[0] = CONTROLLER_EFFECTIVENESSSOUND;
    sBattleBuffersTransferData[1] = songId;
    sBattleBuffersTransferData[2] = (songId & 0xFF00) >> 8;
    sBattleBuffersTransferData[3] = 0;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitPlayFanfare(u8 battlerId, u8 bufferId, u16 songId)
{
	sBattleBuffersTransferData[0] = CONTROLLER_PLAYFANFAREORBGM;
    sBattleBuffersTransferData[1] = songId;
    sBattleBuffersTransferData[2] = (songId & 0xFF00) >> 8;
    sBattleBuffersTransferData[3] = 0;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitFaintingCry(u8 battlerId, u8 bufferId)
{
	sBattleBuffersTransferData[0] = CONTROLLER_FAINTINGCRY;
    sBattleBuffersTransferData[1] = CONTROLLER_FAINTINGCRY;
    sBattleBuffersTransferData[2] = CONTROLLER_FAINTINGCRY;
    sBattleBuffersTransferData[3] = CONTROLLER_FAINTINGCRY;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitIntroSlide(u8 battlerId, u8 bufferId, u8 terrainId)
{
	sBattleBuffersTransferData[0] = CONTROLLER_INTROSLIDE;
    sBattleBuffersTransferData[1] = terrainId;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 2);
}

void BtlController_EmitIntroTrainerBallThrow(u8 battlerId, u8 bufferId)
{
	sBattleBuffersTransferData[0] = CONTROLLER_INTROTRAINERBALLTHROW;
    sBattleBuffersTransferData[1] = CONTROLLER_INTROTRAINERBALLTHROW;
    sBattleBuffersTransferData[2] = CONTROLLER_INTROTRAINERBALLTHROW;
    sBattleBuffersTransferData[3] = CONTROLLER_INTROTRAINERBALLTHROW;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitDrawPartyStatusSummary(u8 battlerId, u8 bufferId, struct HpAndStatus *hpAndStatus, u8 param)
{
	s32 i;
    sBattleBuffersTransferData[0] = CONTROLLER_DRAWPARTYSTATUSSUMMARY;
    sBattleBuffersTransferData[1] = param & 0x7F;
    sBattleBuffersTransferData[2] = (param & 0x80) >> 7;
    sBattleBuffersTransferData[3] = CONTROLLER_DRAWPARTYSTATUSSUMMARY;
    for (i = 0; i < (s32)(sizeof(struct HpAndStatus) * PARTY_SIZE); ++i)
        sBattleBuffersTransferData[4 + i] = *(i + (u8 *)(hpAndStatus));
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, sizeof(struct HpAndStatus) * PARTY_SIZE + 4);
}

void BtlController_EmitHidePartyStatusSummary(u8 battlerId, u8 bufferId)
{
	sBattleBuffersTransferData[0] = CONTROLLER_HIDEPARTYSTATUSSUMMARY;
    sBattleBuffersTransferData[1] = CONTROLLER_HIDEPARTYSTATUSSUMMARY;
    sBattleBuffersTransferData[2] = CONTROLLER_HIDEPARTYSTATUSSUMMARY;
    sBattleBuffersTransferData[3] = CONTROLLER_HIDEPARTYSTATUSSUMMARY;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitEndBounceEffect(u8 battlerId, u8 bufferId)
{
	sBattleBuffersTransferData[0] = CONTROLLER_ENDBOUNCE;
    sBattleBuffersTransferData[1] = CONTROLLER_ENDBOUNCE;
    sBattleBuffersTransferData[2] = CONTROLLER_ENDBOUNCE;
    sBattleBuffersTransferData[3] = CONTROLLER_ENDBOUNCE;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitSpriteInvisibility(u8 battlerId, u8 bufferId, bool8 isInvisible)
{
	sBattleBuffersTransferData[0] = CONTROLLER_SPRITEINVISIBILITY;
    sBattleBuffersTransferData[1] = isInvisible;
    sBattleBuffersTransferData[2] = CONTROLLER_SPRITEINVISIBILITY;
    sBattleBuffersTransferData[3] = CONTROLLER_SPRITEINVISIBILITY;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitBattleAnimation(u8 battlerId, u8 bufferId, u8 animationId, u16 argument)
{
	sBattleBuffersTransferData[0] = CONTROLLER_BATTLEANIMATION;
    sBattleBuffersTransferData[1] = animationId;
    sBattleBuffersTransferData[2] = argument;
    sBattleBuffersTransferData[3] = (argument & 0xFF00) >> 8;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 4);
}

void BtlController_EmitLinkStandbyMsg(u8 battlerId, u8 bufferId, u8 arg1)
{
	sBattleBuffersTransferData[0] = CONTROLLER_LINKSTANDBYMSG;
    sBattleBuffersTransferData[1] = arg1;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 2);
}

void BtlController_EmitResetActionMoveSelection(u8 battlerId, u8 bufferId, u8 caseId)
{
	sBattleBuffersTransferData[0] = CONTROLLER_RESETACTIONMOVESELECTION;
    sBattleBuffersTransferData[1] = caseId;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 2);
}

void BtlController_EmitEndLinkBattle(u8 battlerId, u8 bufferId, u8 battleOutcome)
{
	sBattleBuffersTransferData[0] = CONTROLLER_ENDLINKBATTLE;
    sBattleBuffersTransferData[1] = battleOutcome;
    PrepareBufferDataTransfer(battlerId, bufferId, sBattleBuffersTransferData, 2);
}

/////////////////////////
// GENERAL CONTROLLERS //
/////////////////////////

void ControllerDummy(u8 battlerId) { }

void BattleControllerComplete(u8 battlerId)
{
	gBattlerControllerEndFuncs[battlerId](battlerId);
}

void BtlController_HandleGetMonData(u8 battlerId)
{
	u8 i, monToCheck, monData[sizeof(struct Pokemon) * 2 + 56]; // this allows to get full data of two pokemon, trying to get more will result in overwriting data
    struct Pokemon *party = GetBattlerParty(battlerId);
	u32 size = 0;
	
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

void BtlController_HandleSetMonData(u8 battlerId)
{
	u8 i, monToCheck;
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

void BtlController_HandleSetRawMonData(u8 battlerId)
{
	u8 i, *dst = (u8 *)GetBattlerPartyIndexPtr(battlerId) + gBattleBufferA[battlerId][1];

    for (i = 0; i < gBattleBufferA[battlerId][2]; ++i)
        dst[i] = gBattleBufferA[battlerId][3 + i];
	
    BattleControllerComplete(battlerId);
}

void BtlController_HandleLoadMonSprite(u8 battlerId, bool8 setSpecies, void(*controllerFunc)(u8))
{
	struct Pokemon *mon = GetBattlerPartyIndexPtr(battlerId);
	u16 species = GetMonData(mon, MON_DATA_SPECIES);
	
	BattleLoadMonSpriteGfx(battlerId);
	SetMultiuseSpriteTemplateToPokemon(species, GetBattlerPosition(battlerId));
	gBattlerSpriteIds[battlerId] = CreateSprite(&gMultiuseSpriteTemplate, GetBattlerSpriteCoord(battlerId, BATTLER_COORD_X), GetBattlerSpriteDefault_Y(battlerId), GetBattlerSpriteSubpriority(battlerId));
	gSprites[gBattlerSpriteIds[battlerId]].x2 = -240;
    gSprites[gBattlerSpriteIds[battlerId]].data[0] = battlerId;
	
	if (setSpecies)
		gSprites[gBattlerSpriteIds[battlerId]].data[2] = species;
	
    gSprites[gBattlerSpriteIds[battlerId]].oam.paletteNum = battlerId;
	StartSpriteAnim(&gSprites[gBattlerSpriteIds[battlerId]], 0);
	SetBattlerShadowSpriteCallback(battlerId);
	
	gBattlerControllerFuncs[battlerId] = controllerFunc;
}

void BtlController_HandleSwitchInAnim(u8 battlerId, bool8 loadSprite, void(*controllerFunc)(u8))
{
	if (loadSprite)
		ClearTemporarySpeciesSpriteData(battlerId, gBattleBufferA[battlerId][2]);
	
	gBattlerPartyIndexes[battlerId] = gBattleBufferA[battlerId][1];
	
	if (loadSprite)
		BattleLoadMonSpriteGfx(battlerId);
	
    StartSendOutAnim(battlerId, gBattleBufferA[battlerId][2]);
    gBattlerControllerFuncs[battlerId] = controllerFunc;
}

void BtlController_HandleReturnMonToBall(u8 battlerId)
{
	if (!gBattleBufferA[battlerId][1])
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].animationState = 0;
        gBattlerControllerFuncs[battlerId] = DoSwitchOutAnimation;
    }
    else
    {
		FreeMonSprite(battlerId);
        BattleControllerComplete(battlerId);
    }
}

void BtlController_HandleDrawTrainerPic(u8 battlerId, u32 trainerPicId, bool8 isFront, s16 xPos, s16 yPos, u8 subpriority)
{
	if (isFront)
		DecompressTrainerFrontPic(trainerPicId, battlerId);
	else
		DecompressTrainerBackPalette(trainerPicId, battlerId);
	
    SetMultiuseSpriteTemplateToTrainerBack(trainerPicId, GetBattlerPosition(battlerId));
    gBattlerSpriteIds[battlerId] = CreateSprite(&gMultiuseSpriteTemplate, xPos, yPos, subpriority);
    gSprites[gBattlerSpriteIds[battlerId]].x2 = isFront ? -240 : 240;
    gSprites[gBattlerSpriteIds[battlerId]].data[0] = isFront ? 2 : -2;
	
	if (isFront)
	{
		gSprites[gBattlerSpriteIds[battlerId]].oam.paletteNum = IndexOfSpritePaletteTag(gTrainerFrontPicTable[trainerPicId].palette.tag);
		gSprites[gBattlerSpriteIds[battlerId]].data[5] = gSprites[gBattlerSpriteIds[battlerId]].oam.tileNum;
		gSprites[gBattlerSpriteIds[battlerId]].oam.tileNum = GetSpriteTileStartByTag(gTrainerFrontPicTable[trainerPicId].pic.tag);
		gSprites[gBattlerSpriteIds[battlerId]].oam.affineParam = trainerPicId;
	}
	else
		gSprites[gBattlerSpriteIds[battlerId]].oam.paletteNum = battlerId;
	
    gSprites[gBattlerSpriteIds[battlerId]].callback = SpriteCB_TrainerSlideIn;
    gBattlerControllerFuncs[battlerId] = CompleteOnBattlerSpriteCallbackDummy;
}

void BtlController_HandleTrainerSlide(u8 battlerId, u32 trainerPicId, bool8 isFront, s16 xPos, s16 yPos)
{
	if (isFront)
		DecompressTrainerFrontPic(trainerPicId, battlerId);
	else
		DecompressTrainerBackPalette(trainerPicId, battlerId);
	
	SetMultiuseSpriteTemplateToTrainerBack(trainerPicId, GetBattlerPosition(battlerId));
    gBattlerSpriteIds[battlerId] = CreateSprite(&gMultiuseSpriteTemplate, xPos, yPos, 30);
	
	if (isFront)
	{
		gSprites[gBattlerSpriteIds[battlerId]].x2 = 96;
		gSprites[gBattlerSpriteIds[battlerId]].x += 32;
		gSprites[gBattlerSpriteIds[battlerId]].data[0] = -2;
		gSprites[gBattlerSpriteIds[battlerId]].oam.paletteNum = IndexOfSpritePaletteTag(gTrainerFrontPicTable[trainerPicId].palette.tag);
		gSprites[gBattlerSpriteIds[battlerId]].data[5] = gSprites[gBattlerSpriteIds[battlerId]].oam.tileNum;
		gSprites[gBattlerSpriteIds[battlerId]].oam.tileNum = GetSpriteTileStartByTag(gTrainerFrontPicTable[trainerPicId].pic.tag);
		gSprites[gBattlerSpriteIds[battlerId]].oam.affineParam = trainerPicId;
	}
	else
	{
		gSprites[gBattlerSpriteIds[battlerId]].x2 = -96;
		gSprites[gBattlerSpriteIds[battlerId]].data[0] = 2;
		gSprites[gBattlerSpriteIds[battlerId]].oam.paletteNum = battlerId;
	}
    gSprites[gBattlerSpriteIds[battlerId]].callback = SpriteCB_TrainerSlideIn;
    gBattlerControllerFuncs[battlerId] = CompleteOnBattlerSpriteCallbackDummy;
}

void BtlController_HandleTrainerSlideBack(u8 battlerId, s16 data0, bool8 animateSprite)
{
	SetSpritePrimaryCoordsFromSecondaryCoords(&gSprites[gBattlerSpriteIds[battlerId]]);
    gSprites[gBattlerSpriteIds[battlerId]].data[0] = data0;
    gSprites[gBattlerSpriteIds[battlerId]].data[2] = GetBattlerSide(battlerId) == B_SIDE_PLAYER ? -40 : 280;
    gSprites[gBattlerSpriteIds[battlerId]].data[4] = gSprites[gBattlerSpriteIds[battlerId]].y;
    gSprites[gBattlerSpriteIds[battlerId]].callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(&gSprites[gBattlerSpriteIds[battlerId]], SpriteCallbackDummy);
	
	if (animateSprite)
		StartSpriteAnim(&gSprites[gBattlerSpriteIds[battlerId]], 1);
	
    gBattlerControllerFuncs[battlerId] = FreeTrainerSpriteAfterSlide;
}

void BtlController_HandleFaintAnimation(u8 battlerId)
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
				
				if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
				{
					HandleLowHpMusicChange(&gPlayerParty[gBattlerPartyIndexes[battlerId]], battlerId);
					PlaySE12WithPanning(SE_FAINT, SOUND_PAN_ATTACKER);
					gSprites[gBattlerSpriteIds[battlerId]].data[1] = 0;
					gSprites[gBattlerSpriteIds[battlerId]].data[2] = 5;
					gSprites[gBattlerSpriteIds[battlerId]].callback = SpriteCB_FaintSlideAnim;
					gBattlerControllerFuncs[battlerId] = FreeMonSpriteAfterFaintAnim;
				}
				else
				{
					PlaySE12WithPanning(SE_FAINT, SOUND_PAN_TARGET);
					gSprites[gBattlerSpriteIds[battlerId]].callback = SpriteCB_FaintOpponentMon;
					gBattlerControllerFuncs[battlerId] = HideHealthboxAfterMonFaint;
				}
			}
			break;
	}
}

void BtlController_HandleBallThrowAnim(u8 battlerId, u8 animId)
{
    gBattleSpritesDataPtr->animationData->ballThrowCaseId = gBattleBufferA[battlerId][1];
    gDoingBattleAnim = TRUE;
    InitAndLaunchSpecialAnimation(battlerId, GetCatchingBattler(), animId);
    gBattlerControllerFuncs[battlerId] = CompleteOnSpecialAnimDone;
}

void BtlController_HandleMoveAnimation(u8 battlerId)
{
	if (!IsBattleSEPlaying(battlerId))
    {
        gAnimMoveTurn = gBattleBufferA[battlerId][3];
        gAnimMovePower = gBattleBufferA[battlerId][4] | (gBattleBufferA[battlerId][5] << 8);
        gAnimMoveDmg = gBattleBufferA[battlerId][6] | (gBattleBufferA[battlerId][7] << 8) | (gBattleBufferA[battlerId][8] << 16) | (gBattleBufferA[battlerId][9] << 24);
        gAnimFriendship = gBattleBufferA[battlerId][10];
        gWeatherMoveAnim = gBattleBufferA[battlerId][12] | (gBattleBufferA[battlerId][13] << 8);
        gAnimDisableStructPtr = (struct DisableStruct *)&gBattleBufferA[battlerId][16];
        gTransformedPersonalities[battlerId] = gAnimDisableStructPtr->transformedMonPersonality;
		gTransformedShinies[battlerId] = gAnimDisableStructPtr->transformedMonShynies;
        gBattleSpritesDataPtr->healthBoxesData[battlerId].animationState = 0;
        gBattlerControllerFuncs[battlerId] = DoMoveAnimation;
    }
}

void BtlController_HandlePrintString(u8 battlerId, u16 stringId, bool8 isSelection)
{
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
	BufferStringBattle(battlerId, stringId);
	
	if (isSelection && stringId != STRINGID_DONTLEAVEBIRCH && stringId != STRINGID_CANTESCAPE)
		StringAppend(gDisplayedStringBattle, COMPOUND_STRING("\p"));
	
	BattlePutTextOnWindow(gDisplayedStringBattle, BattleStringShouldBeColored(stringId) ? (B_WIN_MSG | B_TEXT_FLAG_NPC_CONTEXT_FONT) : B_WIN_MSG);
	gBattlerControllerFuncs[battlerId] = CompleteOnInactiveTextPrinter;
}

void BtlController_HandleChooseAction(u8 battlerId, const u8 *actionsStr, const u8 *whatDoStr, void(*controllerFunc)(u8))
{
	u8 i;

    gBattlerControllerFuncs[battlerId] = controllerFunc;
	
    BattlePutTextOnWindow(gText_EmptyString, B_WIN_MSG);
    BattlePutTextOnWindow(actionsStr, B_WIN_ACTION_MENU);
	
    for (i = 0; i < MAX_MON_MOVES; ++i)
        ActionSelectionDestroyCursorAt(i);
	
    ActionSelectionCreateCursorAt(gActionSelectionCursor[battlerId]);
	BattleStringExpandPlaceholdersToDisplayedString(whatDoStr);
    BattlePutTextOnWindow(gDisplayedStringBattle, B_WIN_ACTION_PROMPT);
}

void BtlController_HandleChooseItem(u8 battlerId, void(*controllerFunc)(u8))
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
    gBattlerControllerFuncs[battlerId] = controllerFunc;
    gBattlerInMenuId = battlerId;
}

void BtlController_HandleChoosePokemon(u8 battlerId, void(*controllerFunc)(u8))
{
	u8 i;

    gBattleControllerData[battlerId] = CreateTask(TaskDummy, 0xFF);
    gTasks[gBattleControllerData[battlerId]].data[0] = gBattleBufferA[battlerId][1] & 0xF;
    gBattleStruct->battlerPreventingSwitchout = gBattleBufferA[battlerId][1] >> 4;
    gBattleStruct->playerPartyIdx = gBattleBufferA[battlerId][2];
	
    for (i = 0; i < 3; ++i)
        gBattlePartyCurrentOrder[i] = gBattleBufferA[battlerId][3 + i];
	
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
    gBattlerControllerFuncs[battlerId] = controllerFunc;
    gBattlerInMenuId = battlerId;
}

void BtlController_HandleHealthbarUpdate(u8 battlerId, bool8 updateHpText)
{
	s16 hpVal = gBattleBufferA[battlerId][2] | (gBattleBufferA[battlerId][3] << 8);
	struct Pokemon *mon = GetBattlerPartyIndexPtr(battlerId);
	u32 maxHP = GetMonData(mon, MON_DATA_MAX_HP);

    LoadBattleBarGfx();
	
	if (hpVal != INSTANT_HP_BAR_DROP)
		SetBattleBarStruct(battlerId, maxHP, GetMonData(mon, MON_DATA_HP), hpVal);
	else
	{
		SetBattleBarStruct(battlerId, maxHP, 0, hpVal);
		
		if (updateHpText)
			UpdateHpTextInHealthbox(gHealthboxSpriteIds[battlerId], HP_CURRENT, 0, maxHP);
	}
	gBattleControllerData[battlerId] = updateHpText;
    gBattlerControllerFuncs[battlerId] = CompleteOnHealthbarDone;
}

void BtlController_HandleExpUpdate(u8 battlerId)
{
	u8 monId = gBattleBufferA[battlerId][1];

    if (GetMonData(&gPlayerParty[monId], MON_DATA_LEVEL) >= MAX_LEVEL)
        BattleControllerComplete(battlerId);
    else
    {
        u8 taskId;

        LoadBattleBarGfx();

        taskId = CreateTask(Task_GiveExpToMon, 10);
        gTasks[taskId].tExpTask_monId = monId;
        gTasks[taskId].tExpTask_gainedExp = READ_16(&gBattleBufferA[battlerId][2]);
        gTasks[taskId].tExpTask_battler = battlerId;
		
        gBattlerControllerFuncs[battlerId] = ControllerDummy;
    }
}

void BtlController_HandleStatusIconUpdate(u8 battlerId)
{
	if (!IsBattleSEPlaying(battlerId))
    {
        UpdateHealthboxAttribute(battlerId, HEALTHBOX_STATUS_ICON);
        gBattleSpritesDataPtr->healthBoxesData[battlerId].statusAnimActive = FALSE;
        BattleControllerComplete(battlerId);
    }
}

void BtlController_HandleStatusAnimation(u8 battlerId)
{
	if (!IsBattleSEPlaying(battlerId))
    {
		u32 status = gBattleBufferA[battlerId][2] | (gBattleBufferA[battlerId][3] << 8) | (gBattleBufferA[battlerId][4] << 16) | (gBattleBufferA[battlerId][5] << 24);
        InitAndLaunchChosenStatusAnimation(battlerId, gBattleBufferA[battlerId][1], status);
        gBattlerControllerFuncs[battlerId] = CompleteOnFinishedStatusAnimation;
    }
}

void BtlController_HandlePlaySE(u8 battlerId)
{
    PlaySE12WithPanning(gBattleBufferA[battlerId][1] | (gBattleBufferA[battlerId][2] << 8), GetBattlerSide(battlerId) == B_SIDE_PLAYER ? SOUND_PAN_ATTACKER : SOUND_PAN_TARGET);
    BattleControllerComplete(battlerId);
}

void BtlController_HandlePlayFanfare(u8 battlerId)
{
	PlayFanfare(gBattleBufferA[battlerId][1] | (gBattleBufferA[battlerId][2] << 8));
	BattleControllerComplete(battlerId);
}

void BtlController_HandleFaintingCry(u8 battlerId)
{
    PlayCry_ByMode(GetMonData(GetBattlerPartyIndexPtr(battlerId), MON_DATA_SPECIES), GetBattlerSide(battlerId) == B_SIDE_PLAYER ? -25 : 25, CRY_MODE_FAINT);
    BattleControllerComplete(battlerId);
}

void BtlController_HandleIntroSlide(u8 battlerId)
{
	HandleIntroSlide(gBattleBufferA[battlerId][1]);
    gIntroSlideFlags |= (INTRO_SLIDE_FLAG_SLIDE_IN_COUNTER | INTRO_SLIDE_FLAG_SLIDE_IN_STARTED);
	BattleControllerComplete(battlerId);
}

void BtlController_HandleIntroTrainerBallThrow(u8 battlerId, u16 loadPicTag, u32 trainerPicId, void(*spriteCallback)(struct Sprite *), u8 delay, void(*controllerFunc)(u8))
{
	u8 paletteNum, taskId, side = GetBattlerSide(battlerId);

    SetSpritePrimaryCoordsFromSecondaryCoords(&gSprites[gBattlerSpriteIds[battlerId]]);
	
	if (side == B_SIDE_OPPONENT)
	{
		gSprites[gBattlerSpriteIds[battlerId]].data[0] = 35;
		gSprites[gBattlerSpriteIds[battlerId]].data[2] = 280;
	}
	else
	{
		gSprites[gBattlerSpriteIds[battlerId]].data[0] = 50;
		gSprites[gBattlerSpriteIds[battlerId]].data[2] = -40;
	}
    gSprites[gBattlerSpriteIds[battlerId]].data[4] = gSprites[gBattlerSpriteIds[battlerId]].y;
    gSprites[gBattlerSpriteIds[battlerId]].callback = spriteCallback;
	StoreSpriteCallbackInData6(&gSprites[gBattlerSpriteIds[battlerId]], side == B_SIDE_PLAYER ? SpriteCB_FreePlayerSpriteLoadMonSprite : SpriteCB_FreeOpponentSprite);
	
	if (loadPicTag)
	{
		gSprites[gBattlerSpriteIds[battlerId]].data[5] = battlerId;
		StartSpriteAnim(&gSprites[gBattlerSpriteIds[battlerId]], 1);
		paletteNum = AllocSpritePalette(loadPicTag);
		LoadCompressedPalette(gTrainerBackPicTable[trainerPicId].palette.data, 0x100 + paletteNum * 16, 32);
		gSprites[gBattlerSpriteIds[battlerId]].oam.paletteNum = paletteNum;
	}
	taskId = CreateTask(Task_StartSendOutAnim, 5);
    gTasks[taskId].tSendOutTask_Battler = battlerId;
	gTasks[taskId].tSendOutTask_Delay = delay;
	SetWordTaskArg(taskId, tSendOutTask_ControllerFunc, (u32)controllerFunc);
	
    if (gBattleSpritesDataPtr->healthBoxesData[battlerId].partyStatusSummaryShown)
        gTasks[gBattlerStatusSummaryTaskId[battlerId]].func = Task_HidePartyStatusSummary;
	
    gBattleSpritesDataPtr->animationData->healthboxSlideInStarted = TRUE;
    gBattlerControllerFuncs[battlerId] = ControllerDummy;
}

void BtlController_HandleDrawPartyStatusSummary(u8 battlerId)
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
        
		gBattlerControllerFuncs[battlerId] = EndDrawPartyStatusSummary;
    }
}

void BtlController_HandleHidePartyStatusSummary(u8 battlerId)
{
	if (gBattleSpritesDataPtr->healthBoxesData[battlerId].partyStatusSummaryShown)
        gTasks[gBattlerStatusSummaryTaskId[battlerId]].func = Task_HidePartyStatusSummary;
    BattleControllerComplete(battlerId);
}

void BtlController_HandleEndBounceEffect(u8 battlerId)
{
	EndBounceEffect(battlerId, BOUNCE_HEALTHBOX);
    EndBounceEffect(battlerId, BOUNCE_MON);
	BattleControllerComplete(battlerId);
}

void BtlController_HandleSpriteInvisibility(u8 battlerId)
{
	if (IsBattlerSpritePresent(battlerId))
    {
        gSprites[gBattlerSpriteIds[battlerId]].invisible = gBattleBufferA[battlerId][1];
        CopyBattleSpriteInvisibility(battlerId);
    }
	BattleControllerComplete(battlerId);
}

void BtlController_HandleBattleAnimation(u8 battlerId)
{
	if (!IsBattleSEPlaying(battlerId))
    {
        if (TryHandleLaunchBattleTableAnimation(battlerId, gBattleBufferA[battlerId][1], gBattleBufferA[battlerId][2] | (gBattleBufferA[battlerId][3] << 8)))
            BattleControllerComplete(battlerId);
        else
            gBattlerControllerFuncs[battlerId] = CompleteOnFinishedBattleAnimation;
    }
}

void BtlController_HandleEndLinkBattle(u8 battlerId, u8 battleOutcome, void(*controllerFunc)(u8))
{
	gBattleOutcome = battleOutcome;
    FadeOutMapMusic(5);
    BeginFastPaletteFade(FAST_FADE_OUT_TO_BLACK);
    BattleControllerComplete(battlerId);
	
	if (controllerFunc != NULL)
		gBattlerControllerFuncs[battlerId] = controllerFunc;
}

////////////////////////
// ULTILITY FUNCTIONS //
////////////////////////

void CompleteOnBattlerSpritePosX_0(u8 battlerId)
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

static u32 GetBattlerMonData(u8 battlerId, struct Pokemon *party, u8 monId, u8 *dst)
{
	struct BattlePokemon battleMon;
	struct MovePpInfo moveData;
	u8 *src, nickname[POKEMON_NAME_LENGTH + 1];
	s32 size = 0;
	
	switch (gBattleBufferA[battlerId][1])
	{
		case REQUEST_ALL_BATTLE:
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
			for (size = 0; size < NUM_STATS; size++)
				dst[size] = GetMonData(&party[monId], MON_DATA_HP_IV + gBattleBufferA[battlerId][1] - REQUEST_HP_IV_BATTLE);
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

static void SetBattlerMonData(u8 battlerId, struct Pokemon *party, u8 monId)
{
	 struct BattlePokemon *battlePokemon;
	 struct MovePpInfo *moveData;
	 struct Status1 *status;
	 s32 i;
	 
	 switch (gBattleBufferA[battlerId][1])
	 {
		 case REQUEST_ALL_BATTLE:
		 {
			u8 val;
			
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
			u8 val;
			
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

void TryShinyAnimAfterMonAnim(u8 battlerId)
{
	if (gSprites[gBattlerSpriteIds[battlerId]].animEnded && gSprites[gBattlerSpriteIds[battlerId]].x2 == 0)
    {
        if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim)
            TryShinyAnimation(battlerId);
        else if (gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim)
        {
            gBattleSpritesDataPtr->healthBoxesData[battlerId].triedShinyMonAnim = FALSE;
            gBattleSpritesDataPtr->healthBoxesData[battlerId].finishedShinyMonAnim = FALSE;
            FreeSpriteTilesByTag(ANIM_TAG_GOLD_STARS);
            FreeSpritePaletteByTag(ANIM_TAG_GOLD_STARS);
            BattleControllerComplete(battlerId);
        }
    }
}

void WaitForMonAnimAfterLoad(u8 battlerId)
{
	if (gSprites[gBattlerSpriteIds[battlerId]].animEnded && gSprites[gBattlerSpriteIds[battlerId]].x2 == 0)
        BattleControllerComplete(battlerId);
}

void Task_BltController_RestoreBgmAfterCry(u8 taskId)
{
	if (!IsCryPlayingOrClearCrySongs())
    {
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 0x100);
        DestroyTask(taskId);
    }
}

static void FreeMonSpriteAfterSwitchOutAnim(u8 battlerId)
{
    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].specialAnimActive)
    {
        FreeMonSprite(battlerId);
        BattleControllerComplete(battlerId);
    }
}

static void DoSwitchOutAnimation(u8 battlerId)
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
            gBattlerControllerFuncs[battlerId] = FreeMonSpriteAfterSwitchOutAnim;
        }
        break;
    }
}

static void FreeMonSprite(u8 battlerId)
{
	DestroySpriteAndFreeMatrix(&gSprites[gBattlerSpriteIds[battlerId]]);
	
	if (GetBattlerSide(battlerId) == B_SIDE_OPPONENT)
		HideBattlerShadowSprite(battlerId);
	
	SetHealthboxSpriteInvisible(gHealthboxSpriteIds[battlerId]);
}

static void CompleteOnBattlerSpriteCallbackDummy(u8 battlerId)
{
	if (gSprites[gBattlerSpriteIds[battlerId]].callback == SpriteCallbackDummy)
        BattleControllerComplete(battlerId);
}

static void FreeTrainerSpriteAfterSlide(u8 battlerId)
{
	if (gSprites[gBattlerSpriteIds[battlerId]].callback == SpriteCallbackDummy)
    {
		if (GetBattlerSide(battlerId) == B_SIDE_OPPONENT)
		{
			FreeTrainerFrontPicPaletteAndTile(gSprites[gBattlerSpriteIds[battlerId]].oam.affineParam);
			gSprites[gBattlerSpriteIds[battlerId]].oam.tileNum = gSprites[gBattlerSpriteIds[battlerId]].data[5];
		}
        DestroySpriteAndFreeMatrix(&gSprites[gBattlerSpriteIds[battlerId]]);
		
        BattleControllerComplete(battlerId);
    }
}

static void FreeMonSpriteAfterFaintAnim(u8 battlerId)
{
	if (gSprites[gBattlerSpriteIds[battlerId]].y + gSprites[gBattlerSpriteIds[battlerId]].y2 > DISPLAY_HEIGHT)
    {
        DestroySpriteAndFreeMatrix(&gSprites[gBattlerSpriteIds[battlerId]]);
        SetHealthboxSpriteInvisible(gHealthboxSpriteIds[battlerId]);
        BattleControllerComplete(battlerId);
    }
}

static void HideHealthboxAfterMonFaint(u8 battlerId)
{
	if (!gSprites[gBattlerSpriteIds[battlerId]].inUse)
    {
        SetHealthboxSpriteInvisible(gHealthboxSpriteIds[battlerId]);
        BattleControllerComplete(battlerId);
    }
}

static void CompleteOnSpecialAnimDone(u8 battlerId)
{
	if (!gDoingBattleAnim)
		BattleControllerComplete(battlerId);
}

static void DoMoveAnimation(u8 battlerId)
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
            SetBattlerSpriteAffineMode(0);
            DoMoveAnim((gBattleBufferA[battlerId][1] | (gBattleBufferA[battlerId][2] << 8)));
            gBattleSpritesDataPtr->healthBoxesData[battlerId].animationState = 2;
        }
        break;
    case 2:
        gAnimScriptCallback();
		
        if (!gAnimScriptActive)
        {
            SetBattlerSpriteAffineMode(1);
			
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

static void CompleteOnInactiveTextPrinter(u8 battlerId)
{
	if (!IsTextPrinterActive(0))
		BattleControllerComplete(battlerId);
}

static void CompleteOnHealthbarDone(u8 battlerId)
{
	s16 hpValue = MoveBattleBar(battlerId, gHealthboxSpriteIds[battlerId], HEALTH_BAR);

    SetHealthboxSpriteVisible(gHealthboxSpriteIds[battlerId]);
	
    if (hpValue == -1) // Done
	{
		if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
			HandleLowHpMusicChange(&gPlayerParty[gBattlerPartyIndexes[battlerId]], battlerId);
		
		if ((gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE) && !BtlCtrl_OakOldMan_TestState2Flag(FIRST_BATTLE_MSG_FLAG_INFLICT_DMG))
        {
            BtlCtrl_OakOldMan_SetState2Flag(FIRST_BATTLE_MSG_FLAG_INFLICT_DMG);
            gBattlerControllerFuncs[battlerId] = PrintOakText_InflictingDamageIsKey;
        }
		else
			BattleControllerComplete(battlerId);
	}
	else
	{
		if (gBattleControllerData[battlerId]) // Update hp text
			UpdateHpTextInHealthbox(gHealthboxSpriteIds[battlerId], HP_CURRENT, hpValue, gBattleMons[battlerId].maxHP);
	}
}

static void Task_GiveExpToMon(u8 taskId)
{
    u8 monId = gTasks[taskId].tExpTask_monId, battlerId = gTasks[taskId].tExpTask_battler;
    s16 gainedExp = gTasks[taskId].tExpTask_gainedExp;

    if (IsDoubleBattleForBattler(battlerId) || monId != gBattlerPartyIndexes[battlerId]) // Give exp without moving the expbar.
    {
        struct Pokemon *mon = &gPlayerParty[monId];
        u32 currExp = GetMonData(mon, MON_DATA_EXP);
        u32 nextLvlExp = gExperienceTables[gSpeciesInfo[GetMonData(mon, MON_DATA_SPECIES)].growthRate][GetMonData(mon, MON_DATA_LEVEL) + 1];

        if (currExp + gainedExp >= nextLvlExp)
        {
            SetMonData(mon, MON_DATA_EXP, &nextLvlExp);
            CalculateMonStats(mon);
			
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
            gBattlerControllerFuncs[battlerId] = CompleteOnInactiveTextPrinter;
            DestroyTask(taskId);
        }
    }
    else
        gTasks[taskId].func = Task_PrepareToGiveExpWithExpBar;
}

static void Task_PrepareToGiveExpWithExpBar(u8 taskId)
{
    struct Pokemon *mon = &gPlayerParty[gTasks[taskId].tExpTask_monId];
    u8 level = GetMonData(mon, MON_DATA_LEVEL);
    u16 species = GetMonData(mon, MON_DATA_SPECIES);
    u32 exp = GetMonData(mon, MON_DATA_EXP);
    u32 currLvlExp = gExperienceTables[gSpeciesInfo[species].growthRate][level];
    u32 expToNextLvl;

    exp -= currLvlExp;
    expToNextLvl = gExperienceTables[gSpeciesInfo[species].growthRate][level + 1] - currLvlExp;
    SetBattleBarStruct(gTasks[taskId].tExpTask_battler, expToNextLvl, exp, -gTasks[taskId].tExpTask_gainedExp);
    PlaySE(SE_EXP);
    gTasks[taskId].func = Task_GiveExpWithExpBar;
}

static void Task_GiveExpWithExpBar(u8 taskId)
{
	if (gTasks[taskId].tExpTask_frames < 13)
        ++gTasks[taskId].tExpTask_frames;
    else
    {
        u8 monId = gTasks[taskId].tExpTask_monId;
        s16 gainedExp = gTasks[taskId].tExpTask_gainedExp;
        u8 battlerId = gTasks[taskId].tExpTask_battler;
        s16 newExpPoints = MoveBattleBar(battlerId, gHealthboxSpriteIds[battlerId], EXP_BAR);
		
        SetHealthboxSpriteVisible(gHealthboxSpriteIds[battlerId]);
		
        if (newExpPoints == -1) // The bar has been filled with given exp points.
        {
            s32 currExp = GetMonData(&gPlayerParty[monId], MON_DATA_EXP);
            s32 expOnNextLvl = gExperienceTables[gSpeciesInfo[GetMonData(&gPlayerParty[monId], MON_DATA_SPECIES)].growthRate][GetMonData(&gPlayerParty[monId], MON_DATA_LEVEL) + 1];

            m4aSongNumStop(SE_EXP);

            if (currExp + gainedExp >= expOnNextLvl)
            {
                SetMonData(&gPlayerParty[monId], MON_DATA_EXP, &expOnNextLvl);
                CalculateMonStats(&gPlayerParty[monId]);
				
                gainedExp -= expOnNextLvl - currExp;
                BtlController_EmitTwoReturnValues(battlerId, BUFFER_B, RET_VALUE_LEVELED_UP, gainedExp);

                gTasks[taskId].func = Task_LaunchLvlUpAnim;
            }
            else
            {
                currExp += gainedExp;
                SetMonData(&gPlayerParty[monId], MON_DATA_EXP, &currExp);
                gBattlerControllerFuncs[battlerId] = CompleteOnInactiveTextPrinter;
                DestroyTask(taskId);
            }
        }
    }
}

static void Task_LaunchLvlUpAnim(u8 taskId)
{
	u8 battlerId = gTasks[taskId].tExpTask_battler;

    if (IsDoubleBattleForBattler(battlerId) && gTasks[taskId].tExpTask_monId == gBattlerPartyIndexes[BATTLE_PARTNER(battlerId)])
        battlerId = BATTLE_PARTNER(battlerId);
	
    InitAndLaunchSpecialAnimation(battlerId, battlerId, B_ANIM_LVL_UP);
	
    gTasks[taskId].func = Task_UpdateLvlInHealthbox;
}

static void Task_UpdateLvlInHealthbox(u8 taskId)
{
	u8 battlerId = gTasks[taskId].tExpTask_battler;

    if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].specialAnimActive)
    {
        if (IsDoubleBattleForBattler(battlerId) && gTasks[taskId].tExpTask_monId == gBattlerPartyIndexes[BATTLE_PARTNER(battlerId)])
            UpdateHealthboxAttribute(BATTLE_PARTNER(battlerId), HEALTHBOX_ALL);
        else
            UpdateHealthboxAttribute(battlerId, HEALTHBOX_ALL);
		
        gTasks[taskId].func = Task_TryCreateVerticalStripes;
    }
}

static void Task_TryCreateVerticalStripes(u8 taskId)
{
	u8 battlerId = gTasks[taskId].tExpTask_battler;
	
	if (IsBattlerSpriteVisible(battlerId))
		gTasks[taskId].func = Task_CreateLevelUpVerticalStripes;
	else
	{
		gBattlerControllerFuncs[battlerId] = CompleteOnInactiveTextPrinter;
		DestroyTask(taskId);
	}
}

static void Task_CreateLevelUpVerticalStripes(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
    u8 battlerId = tExpTask_battler;
    u16 bgPriorityRank = GetBattlerSpriteBGPriorityRank(battlerId);
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
        gBattlerControllerFuncs[battlerId] = CompleteOnInactiveTextPrinter;
        DestroyTask(taskId);
        break;
    }
}

static void CompleteOnFinishedStatusAnimation(u8 battlerId)
{
	if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].statusAnimActive)
		BattleControllerComplete(battlerId);
}

static void SpriteCB_FreePlayerSpriteLoadMonSprite(struct Sprite *sprite)
{
	u8 battlerId = sprite->data[5];
	
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

static void Task_StartSendOutAnim(u8 taskId)
{
	u8 battlerId = gTasks[taskId].tSendOutTask_Battler;
	
	if (gTasks[taskId].tSendOutTask_Delay && gTasks[taskId].tSendOutTask_Frames < gTasks[taskId].tSendOutTask_Delay)
		++gTasks[taskId].tSendOutTask_Frames;
    else
    {
		if (!IsDoubleBattleForBattler(battlerId) || (gBattleTypeFlags & BATTLE_TYPE_MULTI))
		{
			gBattleBufferA[battlerId][1] = gBattlerPartyIndexes[battlerId];
			StartSendOutAnim(battlerId, FALSE);
		}
		else
		{
			gBattleBufferA[battlerId][1] = gBattlerPartyIndexes[battlerId];
			StartSendOutAnim(battlerId, FALSE);
			battlerId = BATTLE_PARTNER(battlerId);
			gBattleBufferA[battlerId][1] = gBattlerPartyIndexes[battlerId];
			
			if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
				BattleLoadMonSpriteGfx(battlerId);
			
			StartSendOutAnim(battlerId, FALSE);
			battlerId = BATTLE_PARTNER(battlerId);
		}
		gBattlerControllerFuncs[battlerId] = (void*)(GetWordTaskArg(taskId, tSendOutTask_ControllerFunc));
		DestroyTask(taskId);
    }
}

void ShowHealthBox(u8 battlerId)
{
	DestroySprite(&gSprites[gBattleControllerData[battlerId]]);
	UpdateHealthboxAttribute(battlerId, HEALTHBOX_ALL);
	StartHealthboxSlideIn(battlerId);
	SetHealthboxSpriteVisible(gHealthboxSpriteIds[battlerId]);
	SetBattlerShadowSpriteCallback(battlerId);
}

static void EndDrawPartyStatusSummary(u8 battlerId)
{
	if (gBattleSpritesDataPtr->healthBoxesData[battlerId].partyStatusDelayTimer++ > 92)
    {
        gBattleSpritesDataPtr->healthBoxesData[battlerId].partyStatusDelayTimer = 0;
        BattleControllerComplete(battlerId);
    }
}

static void CompleteOnFinishedBattleAnimation(u8 battlerId)
{
	if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].animFromTableActive)
		BattleControllerComplete(battlerId);
}

static void StartSendOutAnim(u8 battlerId, bool8 dontClearSubstituteBit)
{
	u16 species;
	bool8 isOpponent = (GetBattlerSide(battlerId) == B_SIDE_OPPONENT);
	
	ClearTemporarySpeciesSpriteData(battlerId, dontClearSubstituteBit);
    gBattlerPartyIndexes[battlerId] = gBattleBufferA[battlerId][1];
    species = GetMonData(GetBattlerPartyIndexPtr(battlerId), MON_DATA_SPECIES);
    gBattleControllerData[battlerId] = CreateInvisibleSpriteWithCallback(SpriteCB_WaitForBattlerBallReleaseAnim);
	
	if (isOpponent)
		BattleLoadMonSpriteGfx(battlerId);
	
    SetMultiuseSpriteTemplateToPokemon(species, GetBattlerPosition(battlerId));
    gBattlerSpriteIds[battlerId] = CreateSprite(&gMultiuseSpriteTemplate, GetBattlerSpriteCoord(battlerId, BATTLER_COORD_X), GetBattlerSpriteDefault_Y(battlerId), GetBattlerSpriteSubpriority(battlerId));
    gSprites[gBattleControllerData[battlerId]].data[1] = gBattlerSpriteIds[battlerId];
    gSprites[gBattlerSpriteIds[battlerId]].data[0] = battlerId;
    gSprites[gBattlerSpriteIds[battlerId]].data[2] = species;
    gSprites[gBattlerSpriteIds[battlerId]].oam.paletteNum = battlerId;
    StartSpriteAnim(&gSprites[gBattlerSpriteIds[battlerId]], 0);
    gSprites[gBattlerSpriteIds[battlerId]].invisible = TRUE;
    gSprites[gBattlerSpriteIds[battlerId]].callback = SpriteCallbackDummy;
    gSprites[gBattleControllerData[battlerId]].data[0] = 0;
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

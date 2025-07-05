#include "global.h"
#include "battle.h"
#include "battle_ai.h"
#include "event_data.h"
#include "link.h"
#include "palette.h"
#include "recorded_battle.h"
#include "string_util.h"
#include "test_runner.h"
#include "text.h"

EWRAM_DATA static u32 sAI_Scripts = 0;
EWRAM_DATA static struct Pokemon sSavedPlayerParty[PARTY_SIZE] = {0};
EWRAM_DATA static struct Pokemon sSavedOpponentParty[PARTY_SIZE] = {0};
EWRAM_DATA static u16 sBattlerRecordSizes[MAX_BATTLERS_COUNT] = {0};
EWRAM_DATA static u8 sBattleRecords[MAX_BATTLERS_COUNT][BATTLER_RECORD_SIZE] = {0};
EWRAM_DATA static bool8 sIsPlaybackFinished = FALSE;
EWRAM_DATA u8 gRecordedBattleMultiplayerId = 0;

void RecordedBattle_Init(u32 mode)
{
    u32 i, j;
    
    sIsPlaybackFinished = FALSE;

    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
    {
        sBattlerRecordSizes[i] = 0;

        if (mode == B_RECORD_MODE_RECORDING)
        {
            for (j = 0; j < BATTLER_RECORD_SIZE; j++)
                sBattleRecords[i][j] = 0xFF;
            
            sAI_Scripts = AI_DATA->aiFlags;
        }
    }
}

void RecordedBattle_SaveParties(void)
{
    u32 i;
    
    for (i = 0; i < PARTY_SIZE; i++)
    {
        sSavedPlayerParty[i] = gPlayerParty[i];
        sSavedOpponentParty[i] = gEnemyParty[i];
    }
}

void SetVariablesForRecordedBattle(struct RecordedBattleSave *src)
{
    bool32 var;
    u32 i, j;
    
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();

    for (i = 0; i < PARTY_SIZE; i++)
    {
        gPlayerParty[i] = src->playerParty[i];
        gEnemyParty[i] = src->opponentParty[i];
    }

    for (i = 0; i < MAX_LINK_PLAYERS; i++)
    {
        for (var = FALSE, j = 0; j < PLAYER_NAME_LENGTH + 1; j++)
        {
            gLinkPlayers[i].name[j] = src->playersName[i][j];
            if (src->playersName[i][j] == EOS)
                var = TRUE;
        }
        gLinkPlayers[i].gender = src->playersGender[i];
        gLinkPlayers[i].language = src->playersLanguage[i];
        gLinkPlayers[i].id = src->playersBattlers[i];
        gLinkPlayers[i].trainerId = src->playersTrainerId[i];

        if (var)
            ConvertInternationalString(gLinkPlayers[i].name, gLinkPlayers[i].language);
    }
    gBattleTypeFlags = src->battleFlags | BATTLE_TYPE_RECORDED;
    gTrainerBattleOpponent_A = src->opponentA;
    gRecordedBattleMultiplayerId = src->multiplayerId;
    sAI_Scripts = src->AI_scripts;

    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
    {
        for (j = 0; j < BATTLER_RECORD_SIZE; j++)
            sBattleRecords[i][j] = src->battleRecord[i][j];
    }
}

bool32 RecordedBattle_CanStopPlayback(void)
{
    return (sIsPlaybackFinished == FALSE);
}

u32 GetAiScriptsInRecordedBattle(void)
{
    return sAI_Scripts;
}

void RecordedBattle_ClearBattlerAction(u32 battler, u32 bytesToClear)
{
    u32 i;
    
    for (i = 0; i < bytesToClear; i++)
    {
        sBattlerRecordSizes[battler]--;
        sBattleRecords[battler][sBattlerRecordSizes[battler]] = 0xFF;
        if (sBattlerRecordSizes[battler] == 0)
            break;
    }
}

u32 RecordedBattle_GetBattlerAction(u32 actionType, u32 battlerId)
{
    if (gTestRunnerEnabled)
        BattleTest_CheckBattleRecordActionType(battlerId, sBattlerRecordSizes[battlerId], actionType);
    
    // Trying to read past array or invalid action byte, battle is over.
    if (sBattlerRecordSizes[battlerId] >= BATTLER_RECORD_SIZE || sBattleRecords[battlerId][sBattlerRecordSizes[battlerId]] == 0xFF)
    {
        gSpecialVar_Result = gBattleOutcome = B_OUTCOME_PLAYER_TELEPORTED; // hah
        ResetPaletteFade();
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        SetMainCallback2(CB2_QuitRecordedBattle);
        return 0xFF;
    }
    else
        return sBattleRecords[battlerId][sBattlerRecordSizes[battlerId]++];
}

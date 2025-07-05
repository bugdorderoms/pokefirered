#ifndef GUARD_RECORDED_BATTLE_H
#define GUARD_RECORDED_BATTLE_H

#include "constants/battle.h"

#define BATTLER_RECORD_SIZE 664

struct RecordedBattleSave
{
    struct Pokemon playerParty[PARTY_SIZE];
    struct Pokemon opponentParty[PARTY_SIZE];
    u8 playersName[MAX_BATTLERS_COUNT][PLAYER_NAME_LENGTH + 1];
    u8 playersGender[MAX_BATTLERS_COUNT];
    u32 playersTrainerId[MAX_BATTLERS_COUNT];
    u8 playersLanguage[MAX_BATTLERS_COUNT];
    u32 battleFlags;
    u8 playersBattlers[MAX_BATTLERS_COUNT];
    u16 opponentA;
    u16 multiplayerId;
    u32 AI_scripts;
    u8 battleRecord[MAX_BATTLERS_COUNT][BATTLER_RECORD_SIZE];
    u32 checksum;
};

enum
{
    RECORDED_BYTE, // Generic.
    RECORDED_ACTION_TYPE,
    RECORDED_MOVE_SLOT,
    RECORDED_MOVE_TARGET,
    RECORDED_PARTY_INDEX
};

extern u8 gRecordedBattleMultiplayerId;

#define B_RECORD_MODE_RECORDING 1
#define B_RECORD_MODE_PLAYBACK  2

void RecordedBattle_Init(u32 mode);
void RecordedBattle_SaveParties(void);
void SetVariablesForRecordedBattle(struct RecordedBattleSave *src);
bool32 RecordedBattle_CanStopPlayback(void);
u32 GetAiScriptsInRecordedBattle(void);
u32 RecordedBattle_GetBattlerAction(u32 actionType, u32 battlerId);
void RecordedBattle_ClearBattlerAction(u32 battler, u32 bytesToClear);

#endif // GUARD_RECORDED_BATTLE_H

#ifndef GUARD_TEST_RUNNER_H
#define GUARD_TEST_RUNNER_H

extern const bool8 gTestRunnerEnabled;
extern const bool8 gTestRunnerHeadless;
extern const bool8 gTestRunnerSkipIsFail;

void TestRunner_Battle_RecordAbilityPopUp(u32 battlerId, u32 ability);
void TestRunner_Battle_RecordAnimation(u32 animType, u32 animId);
void TestRunner_Battle_RecordHP(u32 battlerId, u32 oldHP, u32 newHP);
void TestRunner_Battle_RecordExp(u32 battlerId, u32 oldExp, u32 newExp);
void TestRunner_Battle_RecordMessage(const u8 *message);
void TestRunner_Battle_RecordStatus1(u32 battlerId, u32 status1);
void TestRunner_Battle_AfterLastTurn(void);

u32 TestRunner_Battle_GetChosenGimmick(u32 battlerId, u32 partyIndex);

void BattleTest_CheckBattleRecordActionType(u32 battlerId, u32 recordIndex, u32 actionType);

#endif // GUARD_TEST_RUNNER_H

#ifndef GUARD_BATTLE_RAID_H
#define GUARD_BATTLE_RAID_H

#define MAX_RAID_SHIELDS 5

// The order here matters, see GetRaidSheerEffect
enum
{
    RAID_CHEER_HEAL_HP,
    RAID_CHEER_CURE_STATUS,
    RAID_CHEER_BOOST_STAT,
    RAID_CHEER_SET_REFLECT,
    RAID_CHEER_SET_LIGHTSCREEN,
    RAID_CHEER_BREAK_SHIELD,
    RAID_CHEER_NOTHING
};

bool32 IsRaidBoss(u32 battlerId);
bool32 ShouldRaidKickPlayer(void);
bool32 ShouldRaidStartWithShields(u32 battlerId);
u32 GetNumRaidShieldsToCreate(u32 battlerId);
u32 GetNumRaidShieldsMoveDestroys(u32 move);
void DestroyRaidBossShields(u32 numShieldsToBreak);
u32 GetNextRaidShieldHP(u32 battlerId);
bool32 ShouldCreateRaidShields(u32 battlerId);
bool32 IsRaidBossUsingRegularMove(u32 battlerId, u32 baseMove);
u32 GetRaidBossDynamaxHPMultiplier(bool32 inverseMultiplier);
void StartRaidDynamaxEnergyPosition(void);
void ChangeRaidDynamaxEnergyPosition(void);
bool32 CanActivateGimmickInRaid(u32 battlerId);
bool32 IsMoveBlockedByRaidBattle(u32 target, u32 move);
u32 GetRaidBattleStatNullificationChance(u32 battlerId);
u32 GetRaidSheerEffect(void);

#endif // GUARD_BATTLE_RAID_H

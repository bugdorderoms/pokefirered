#ifndef GUARD_BATTLE_QUEUED_EFFECTS_H
#define GUARD_BATTLE_QUEUED_EFFECTS_H

// Per battler queued effects
enum
{
	B_BATTLER_QUEUED_FUTURE_SIGHT,
	B_BATTLER_QUEUED_WISH,
	B_BATTLER_QUEUED_COUNT,
};

// Per side queued effects
enum
{
	B_SIDE_QUEUED_SEA_OF_FIRE,
	B_SIDE_QUEUED_GMAX_CANNONADE,
	B_SIDE_QUEUED_GMAX_VINE_LASH,
	B_SIDE_QUEUED_GMAX_VOLCALITH,
	B_SIDE_QUEUED_GMAX_WILDFIRE,
	B_SIDE_QUEUED_GMAX_STEELSURGE,
	B_SIDE_QUEUED_SPIKES,
	B_SIDE_QUEUED_TOXIC_SPIKES,
	B_SIDE_QUEUED_STEALTH_ROCK,
	B_SIDE_QUEUED_STICKY_WEB,
	B_SIDE_QUEUED_COUNT,
};

// Queued effects tables
extern const u8 gWishFutureSightQueuedEffectIds[];
extern const u8 gSeaOfFireAndGMaxQueuedEffectIds[];
extern const u8 gEntryHazardsQueuedEffectIds[];

void ResetAllQueuedEffectsDone(void);
void AddBattleEffectToBattlerQueueList(u32 battlerId, u32 id);
void AddBattleEffectToSideQueueList(u32 side, u32 id);
void RemoveBattleEffectFromBattlerQueueList(u32 battlerId, u32 id);
void RemoveBattleEffectFromSideQueueList(u32 side, u32 id);
bool32 TryDoQueuedBattleEffectsInBattlerList(u32 battlerId, const u8 *list, bool32(*func)(u32, u32));
bool32 TryDoQueuedBattleEffectsInSideList(u32 battlerId, const u8 *list, bool32(*func)(u32, u32, u32));

// Checkers
bool32 QueuedEffects_DoWishFutureSight(u32 battlerId, u32 id);
bool32 QueuedEffects_DoSeaOfFireAndGMaxEffects(u32 battlerId, u32 side, u32 id);
bool32 QueuedEffects_DoEntryHazardsEffects(u32 battlerId, u32 side, u32 id);

#endif // GUARD_BATTLE_QUEUED_EFFECTS_H
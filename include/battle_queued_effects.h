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
void AddBattleEffectToBattlerQueueList(u8 battlerId, u8 id);
void AddBattleEffectToSideQueueList(u8 side, u8 id);
void RemoveBattleEffectFromBattlerQueueList(u8 battlerId, u8 id);
void RemoveBattleEffectFromSideQueueList(u8 side, u8 id);
bool8 TryDoQueuedBattleEffectsInBattlerList(u8 battlerId, const u8 *list, bool8(*func)(u8, u8));
bool8 TryDoQueuedBattleEffectsInSideList(u8 battlerId, const u8 *list, bool8(*func)(u8, u8, u8));

// Checkers
bool8 QueuedEffects_DoWishFutureSight(u8 battlerId, u8 id);
bool8 QueuedEffects_DoSeaOfFireAndGMaxEffects(u8 battlerId, u8 side, u8 id);
bool8 QueuedEffects_DoEntryHazardsEffects(u8 battlerId, u8 side, u8 id);

#endif // GUARD_BATTLE_QUEUED_EFFECTS_H
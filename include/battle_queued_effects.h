#ifndef GUARD_BATTLE_QUEUED_EFFECTS_H
#define GUARD_BATTLE_QUEUED_EFFECTS_H

enum
{
	B_QUEUED_FUTURE_SIGHT,
	B_QUEUED_WISH,
	B_QUEUED_SEA_OF_FIRE,
	B_QUEUED_GMAX_CANNONADE,
	B_QUEUED_GMAX_VINE_LASH,
	B_QUEUED_GMAX_VOLCALITH,
	B_QUEUED_GMAX_WILDFIRE,
	B_QUEUED_COUNT,
};

// Queued effects tables
extern const u8 gWishFutureSightQueuedEffectIds[];
extern const u8 gSeaOfFireAndGMaxQueuedEffectIds[];

// Functions
void ResetAllQueuedEffectsDone(void);
void AddBattleEffectToQueueList(u8 battlerId, u8 id);
void RemoveBattleEffectFromQueueList(u8 battlerId, u8 id);
bool8 TryDoQueuedBattleEffectsInList(const u8 *list, bool8(*func)(u8, u8));

// Checkers
bool8 QueuedEffects_DoWishFutureSight(u8 battlerId, u8 id);
bool8 QueuedEffects_DoSeaOfFireAndGMaxEffects(u8 battlerId, u8 id);

#endif // GUARD_BATTLE_QUEUED_EFFECTS_H
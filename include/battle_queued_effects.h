#ifndef GUARD_BATTLE_QUEUED_EFFECTS_H
#define GUARD_BATTLE_QUEUED_EFFECTS_H

#include "constants/battle_queued_effects.h"

// Queued effects tables
extern const u8 gWishFutureSightQueuedEffectIds[];
extern const u8 gSeaOfFireAndGMaxQueuedEffectIds[];
extern const u8 gEntryHazardsQueuedEffectIds[];

void ResetAllQueuedEffectsDone(void);
u32 FindQueuedEffectsInBattlerList(u32 battlerId, const u8 *list);
u32 FindQueuedEffectsInSideList(u32 side, const u8 *list);
bool32 AddBattleEffectToBattlerQueueList(u32 battlerId, u32 id);
bool32 AddBattleEffectToSideQueueList(u32 side, u32 id);
void RemoveBattleEffectFromBattlerQueueList(u32 battlerId, u32 id);
void RemoveBattleEffectFromSideQueueList(u32 side, u32 id);
bool32 TryDoQueuedBattleEffectsInBattlerList(u32 battlerId, const u8 *list, bool32(*func)(u32, u32));
bool32 TryDoQueuedBattleEffectsInSideList(u32 battlerId, const u8 *list, bool32(*func)(u32, u32, u32));

// Checkers
bool32 QueuedEffects_DoWishFutureSight(u32 battlerId, u32 id);
bool32 QueuedEffects_DoSeaOfFireAndGMaxEffects(u32 battlerId, u32 side, u32 id);
bool32 QueuedEffects_DoEntryHazardsEffects(u32 battlerId, u32 side, u32 id);

#endif // GUARD_BATTLE_QUEUED_EFFECTS_H
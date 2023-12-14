#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_message.h"
#include "battle_queued_effects.h"
#include "battle_scripts.h"
#include "constants/battle_string_ids.h"

// Lists
const u8 gWishFutureSightQueuedEffectIds[] =
{
	B_QUEUED_FUTURE_SIGHT,
	B_QUEUED_WISH,
	B_QUEUED_COUNT
};

const u8 gSeaOfFireAndGMaxQueuedEffectIds[] =
{
	B_QUEUED_SEA_OF_FIRE,
	B_QUEUED_GMAX_CANNONADE,
	B_QUEUED_GMAX_VINE_LASH,
	B_QUEUED_GMAX_VOLCALITH,
	B_QUEUED_GMAX_WILDFIRE,
	B_QUEUED_COUNT,
};

// Functions
static u8 FindBattlerQueuedEffectInList(u8 battlerId, u8 id)
{
	u8 i;
	
	for (i = 0; i < gBattleStruct->queuedEffectsCount; i++)
	{
		if (gBattleStruct->queuedEffectsList[i].id == id && gBattleStruct->queuedEffectsList[i].battler == battlerId)
			return i;
	}
	return B_QUEUED_COUNT;
}

bool8 TryDoQueuedBattleEffectsInList(const u8 *list, bool8(*func)(u8, u8))
{
	u8 i, j;
	
	for (i = 0; i < gBattleStruct->queuedEffectsCount; i++)
	{
		for (j = 0; list[j] != B_QUEUED_COUNT; j++)
		{
			if (gBattleStruct->queuedEffectsList[i].id == list[j] && !gBattleStruct->queuedEffectsList[i].done && func(gBattleStruct->queuedEffectsList[i].battler, list[j]))
				return TRUE;
		}
	}
	return FALSE;
}

void AddBattleEffectToQueueList(u8 battlerId, u8 id)
{
	if (FindBattlerQueuedEffectInList(battlerId, id) == B_QUEUED_COUNT)
	{
		gBattleStruct->queuedEffectsList[gBattleStruct->queuedEffectsCount].id = id;
		gBattleStruct->queuedEffectsList[gBattleStruct->queuedEffectsCount].battler = battlerId;
		gBattleStruct->queuedEffectsList[gBattleStruct->queuedEffectsCount].done = FALSE;
		gBattleStruct->queuedEffectsList[++gBattleStruct->queuedEffectsCount].id = B_QUEUED_COUNT;
	}
}

void RemoveBattleEffectFromQueueList(u8 battlerId, u8 id)
{
	u8 i, temp, listId = FindBattlerQueuedEffectInList(battlerId, id);
	bool8 temp2;
	
	if (listId != B_QUEUED_COUNT)
	{
		for (i = listId; i < gBattleStruct->queuedEffectsCount; i++)
		{
			SWAP(gBattleStruct->queuedEffectsList[i].id, gBattleStruct->queuedEffectsList[i + 1].id, temp);
			SWAP(gBattleStruct->queuedEffectsList[i].battler, gBattleStruct->queuedEffectsList[i + 1].battler, temp);
			SWAP(gBattleStruct->queuedEffectsList[i].done, gBattleStruct->queuedEffectsList[i + 1].done, temp2);
		}
		gBattleStruct->queuedEffectsList[gBattleStruct->queuedEffectsCount--].id = B_QUEUED_COUNT;
	}
}

void ResetAllQueuedEffectsDone(void)
{
	u8 i;
	
	for (i = 0; i < gBattleStruct->queuedEffectsCount; i++)
		gBattleStruct->queuedEffectsList[i].done = FALSE;
}

// Checkers
bool8 QueuedEffects_DoWishFutureSight(u8 battlerId, u8 id)
{
	bool8 effect = FALSE;
	
	switch (id)
	{
		case B_QUEUED_FUTURE_SIGHT:
		    if (IsBattlerAlive(battlerId) && gWishFutureKnock.futureSightCounter[battlerId] != 0 && --gWishFutureKnock.futureSightCounter[battlerId] == 0)
			{
				gBattlerAttacker = gWishFutureKnock.futureSightAttacker[battlerId];
				gCurrentMove = gWishFutureKnock.futureSightMove[battlerId];
				SetTypeBeforeUsingMove(gCurrentMove, gBattlerAttacker);
				
				gSpecialStatuses[battlerId].dmg = 0xFFFF;
				
				switch (GetFutureAttackStringId(gCurrentMove))
				{
					case B_MSG_FORESAW_ATTACK:
					    gBattleScripting.animArg1 = B_ANIM_FUTURE_SIGHT_HIT;
						break;
					case B_MSG_CHOSE_AS_DESTINY:
					    gBattleScripting.animArg1 = B_ANIM_DOOM_DESIRE_HIT;
						break;
				}
				BattleScriptExecute(BattleScript_MonTookFutureAttack);
				effect = TRUE;
			}
			break;
		case B_QUEUED_WISH:
		    if (IsBattlerAlive(battlerId) && gWishFutureKnock.wishCounter[battlerId] != 0 && --gWishFutureKnock.wishCounter[battlerId] == 0)
			{
				BattleScriptExecute(BattleScript_WishComesTrue);
				effect = TRUE;
			}
			break;
	}
	if (effect)
	{
		gBattlerTarget = battlerId;
		RemoveBattleEffectFromQueueList(battlerId, id);
	}
	return effect;
}

bool8 QueuedEffects_DoSeaOfFireAndGMaxEffects(u8 battlerId, u8 id)
{
	return FALSE;
}

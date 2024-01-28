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

const u8 gEntryHazardsQueuedEffectIds[] =
{
	B_QUEUED_SPIKES,
	B_QUEUED_TOXIC_SPIKES,
	B_QUEUED_STEALTH_ROCK,
	B_QUEUED_STICKY_WEB,
	B_QUEUED_COUNT,
};

// Functions
static u8 FindBattlerQueuedEffectInList(u8 battlerId, u8 id)
{
	u8 i;
	
	for (i = 0; i < gBattleStruct->queuedEffectsCount[battlerId]; i++)
	{
		if (gBattleStruct->queuedEffectsList[battlerId][i].id == id)
			return i;
	}
	return B_QUEUED_COUNT;
}

bool8 TryDoQueuedBattleEffectsInList(u8 battlerId, const u8 *list, bool8(*func)(u8, u8))
{
	u8 i, j;
	
	for (i = 0; i < gBattleStruct->queuedEffectsCount[battlerId]; i++)
	{
		for (j = 0; list[j] != B_QUEUED_COUNT; j++)
		{
			if (gBattleStruct->queuedEffectsList[battlerId][i].id == list[j] && !gBattleStruct->queuedEffectsList[battlerId][i].done)
			{
				gBattleStruct->queuedEffectsList[battlerId][i].done = TRUE;
				
				if (func(battlerId, list[j]))
					return TRUE;
			}
		}
	}
	return FALSE;
}

void AddBattleEffectToQueueList(u8 battlerId, u8 id)
{
	if (FindBattlerQueuedEffectInList(battlerId, id) == B_QUEUED_COUNT)
	{
		gBattleStruct->queuedEffectsList[battlerId][gBattleStruct->queuedEffectsCount[battlerId]].id = id;
		gBattleStruct->queuedEffectsList[battlerId][gBattleStruct->queuedEffectsCount[battlerId]].done = FALSE;
		++gBattleStruct->queuedEffectsCount[battlerId];
	}
}

void RemoveBattleEffectFromQueueList(u8 battlerId, u8 id)
{
	u8 i, temp, pos = FindBattlerQueuedEffectInList(battlerId, id);
	bool8 temp2;
	
	if (pos != B_QUEUED_COUNT)
	{
		if (gBattleStruct->queuedEffectsCount[battlerId] > 1)
		{
			for (i = pos + 1; gBattleStruct->queuedEffectsCount[battlerId]; i++)
			{
				SWAP(gBattleStruct->queuedEffectsList[battlerId][i - 1].id, gBattleStruct->queuedEffectsList[battlerId][i].id, temp);
				SWAP(gBattleStruct->queuedEffectsList[battlerId][i - 1].done, gBattleStruct->queuedEffectsList[battlerId][i].done, temp2);
			}
		}
		--gBattleStruct->queuedEffectsCount[battlerId];
	}
}

void ResetAllQueuedEffectsDone(void)
{
	u8 i, j;
	
	for (i = 0; i < MAX_BATTLERS_COUNT; i++)
	{
		for (j = 0; j < gBattleStruct->queuedEffectsCount[i]; j++)
			gBattleStruct->queuedEffectsList[i][j].done = FALSE;
	}
}

// Checkers
bool8 QueuedEffects_DoWishFutureSight(u8 battlerId, u8 id)
{
	bool8 effect = FALSE;
	
	switch (id)
	{
		case B_QUEUED_FUTURE_SIGHT:
		    if (gWishFutureKnock.futureSightCounter[battlerId] != 0 && --gWishFutureKnock.futureSightCounter[battlerId] == 0)
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
		    if (gWishFutureKnock.wishCounter[battlerId] != 0 && --gWishFutureKnock.wishCounter[battlerId] == 0 && !(gStatuses3[battlerId] & STATUS3_HEAL_BLOCK))
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

static void SetDmgHazardsBattleScript(u8 multistringId)
{
	gBattleCommunication[MULTISTRING_CHOOSER] = multistringId;
	
	BattleScriptPushCursor();
	
	if (gBattlescriptCurrInstr[1] == BS_TARGET)
		gBattlescriptCurrInstr = BattleScript_DmgHazardsOnTarget;
	else if (gBattlescriptCurrInstr[1] == BS_ATTACKER)
		gBattlescriptCurrInstr = BattleScript_DmgHazardsOnAttacker;
	else
		gBattlescriptCurrInstr = BattleScript_DmgHazardsOnFaintedBattler;
	
	gHitMarker |= (HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_PASSIVE_DAMAGE | HITMARKER_IGNORE_DISGUISE);
}

bool8 QueuedEffects_DoEntryHazardsEffects(u8 battlerId, u8 id)
{
	u8 side;
	bool8 effect = FALSE;
	
	if (IsBattlerGrounded(battlerId))
	{
		side = GetBattlerSide(battlerId);
		
		switch (id)
		{
			case B_QUEUED_SPIKES:
			    if ((gSideStatuses[side] & SIDE_STATUS_SPIKES) && GetBattlerAbility(battlerId) != ABILITY_MAGIC_GUARD)
				{
					gBattleMoveDamage = gBattleMons[battlerId].maxHP / ((5 - gSideTimers[side].spikesAmount) * 2);
					if (gBattleMoveDamage == 0)
						gBattleMoveDamage = 1;
					SetDmgHazardsBattleScript(B_MSG_HURT_BY_SPIKES);
					effect = TRUE;
				}
				break;
			case B_QUEUED_TOXIC_SPIKES:
			    break;
			case B_QUEUED_STEALTH_ROCK:
			    if (GetBattlerAbility(battlerId) != ABILITY_MAGIC_GUARD)
				{
					
				}
				break;
			case B_QUEUED_STICKY_WEB:
			    if (GetBattlerAbility(battlerId) != ABILITY_MAGIC_GUARD)
				{
					
				}
				break;
			case B_QUEUED_STEELSURGE:
			    if (GetBattlerAbility(battlerId) != ABILITY_MAGIC_GUARD)
				{
					
				}
				break;
		}
	}
	
	if (effect)
		gBattleScripting.battler = battlerId;
	
	return effect;
}

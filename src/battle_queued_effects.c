#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_move_effects.h"
#include "battle_message.h"
#include "battle_queued_effects.h"
#include "battle_scripts.h"
#include "set_effect.h"
#include "constants/battle_string_ids.h"
#include "constants/moves.h"

// Lists
const u8 gWishFutureSightQueuedEffectIds[] =
{
	B_BATTLER_QUEUED_FUTURE_SIGHT,
	B_BATTLER_QUEUED_WISH,
	B_BATTLER_QUEUED_COUNT
};

const u8 gSeaOfFireAndGMaxQueuedEffectIds[] =
{
	B_SIDE_QUEUED_SEA_OF_FIRE,
	B_SIDE_QUEUED_GMAX_CANNONADE,
	B_SIDE_QUEUED_GMAX_VINE_LASH,
	B_SIDE_QUEUED_GMAX_VOLCALITH,
	B_SIDE_QUEUED_GMAX_WILDFIRE,
	B_SIDE_QUEUED_COUNT
};

const u8 gEntryHazardsQueuedEffectIds[] =
{
	B_SIDE_QUEUED_SPIKES,
	B_SIDE_QUEUED_TOXIC_SPIKES,
	B_SIDE_QUEUED_STEALTH_ROCK,
	B_SIDE_QUEUED_STICKY_WEB,
	B_SIDE_QUEUED_COUNT
};

// Functions
static u8 FindQueuedEffectInList(u8 count, struct QueuedEffect *queuedEffectsList, u8 id, u8 entryNoFound)
{
	u8 i;
	
	for (i = 0; i < count; i++)
	{
		if (queuedEffectsList[i].id == id)
			return i;
	}
	return entryNoFound;
}

static inline u8 FindQueuedEffectInBattlerList(u8 battlerId, u8 id)
{
	return FindQueuedEffectInList(gBattleStruct->battlers[battlerId].queuedEffectsCount, gBattleStruct->battlers[battlerId].queuedEffectsList, id, B_BATTLER_QUEUED_COUNT);
}

static inline u8 FindQueuedEffectInSideList(u8 side, u8 id)
{
	return FindQueuedEffectInList(gBattleStruct->sides[side].queuedEffectsCount, gBattleStruct->sides[side].queuedEffectsList, id, B_SIDE_QUEUED_COUNT);
}

bool8 TryDoQueuedBattleEffectsInBattlerList(u8 battlerId, const u8 *list, bool8(*func)(u8, u8))
{
	u8 i, j;
	
	for (i = 0; i < gBattleStruct->battlers[battlerId].queuedEffectsCount; i++)
	{
		for (j = 0; list[j] != B_BATTLER_QUEUED_COUNT; j++)
		{
			if (gBattleStruct->battlers[battlerId].queuedEffectsList[i].id == list[j] && !gBattleStruct->battlers[battlerId].queuedEffectsList[i].done)
			{
				gBattleStruct->battlers[battlerId].queuedEffectsList[i].done = TRUE;
				
				if (func(battlerId, list[j]))
					return TRUE;
			}
		}
	}
	return FALSE;
}

bool8 TryDoQueuedBattleEffectsInSideList(u8 battlerId, const u8 *list, bool8(*func)(u8, u8, u8))
{
	u8 i, j, side = GetBattlerSide(battlerId);
	
	for (i = 0; i < gBattleStruct->sides[side].queuedEffectsCount; i++)
	{
		for (j = 0; list[j] != B_SIDE_QUEUED_COUNT; j++)
		{
			if (gBattleStruct->sides[side].queuedEffectsList[i].id == list[j] && !gBattleStruct->sides[side].queuedEffectsList[i].done)
			{
				gBattleStruct->sides[side].queuedEffectsList[i].done = TRUE;
				
				if (func(battlerId, side, list[j]))
					return TRUE;
			}
		}
	}
	return FALSE;
}

void AddBattleEffectToBattlerQueueList(u8 battlerId, u8 id)
{
	if (FindQueuedEffectInBattlerList(battlerId, id) == B_BATTLER_QUEUED_COUNT)
	{
		gBattleStruct->battlers[battlerId].queuedEffectsList[gBattleStruct->battlers[battlerId].queuedEffectsCount].id = id;
		gBattleStruct->battlers[battlerId].queuedEffectsList[gBattleStruct->battlers[battlerId].queuedEffectsCount].done = FALSE;
		++gBattleStruct->battlers[battlerId].queuedEffectsCount;
	}
}

void AddBattleEffectToSideQueueList(u8 side, u8 id)
{
	if (FindQueuedEffectInSideList(side, id) == B_SIDE_QUEUED_COUNT)
	{
		gBattleStruct->sides[side].queuedEffectsList[gBattleStruct->sides[side].queuedEffectsCount].id = id;
		gBattleStruct->sides[side].queuedEffectsList[gBattleStruct->sides[side].queuedEffectsCount].done = FALSE;
		++gBattleStruct->sides[side].queuedEffectsCount;
	}
}

void RemoveBattleEffectFromBattlerQueueList(u8 battlerId, u8 id)
{
	u8 i, temp, pos = FindQueuedEffectInBattlerList(battlerId, id);
	bool8 temp2;
	
	if (pos != B_BATTLER_QUEUED_COUNT)
	{
		if (gBattleStruct->battlers[battlerId].queuedEffectsCount > 1)
		{
			for (i = pos + 1; gBattleStruct->battlers[battlerId].queuedEffectsCount; i++)
			{
				SWAP(gBattleStruct->battlers[battlerId].queuedEffectsList[i - 1].id, gBattleStruct->battlers[battlerId].queuedEffectsList[i].id, temp);
				SWAP(gBattleStruct->battlers[battlerId].queuedEffectsList[i - 1].done, gBattleStruct->battlers[battlerId].queuedEffectsList[i].done, temp2);
			}
		}
		--gBattleStruct->battlers[battlerId].queuedEffectsCount;
	}
}

void RemoveBattleEffectFromSideQueueList(u8 side, u8 id)
{
	u8 i, temp, pos = FindQueuedEffectInSideList(side, id);
	bool8 temp2;
	
	if (pos != B_SIDE_QUEUED_COUNT)
	{
		if (gBattleStruct->sides[side].queuedEffectsCount > 1)
		{
			for (i = pos + 1; gBattleStruct->sides[side].queuedEffectsCount; i++)
			{
				SWAP(gBattleStruct->sides[side].queuedEffectsList[i - 1].id, gBattleStruct->sides[side].queuedEffectsList[i].id, temp);
				SWAP(gBattleStruct->sides[side].queuedEffectsList[i - 1].done, gBattleStruct->sides[side].queuedEffectsList[i].done, temp2);
			}
		}
		--gBattleStruct->sides[side].queuedEffectsCount;
	}
}

void ResetAllQueuedEffectsDone(void)
{
	u8 i, j;
	
	for (i = 0; i < MAX_BATTLERS_COUNT; i++)
	{
		for (j = 0; j < gBattleStruct->battlers[i].queuedEffectsCount; j++)
			gBattleStruct->battlers[i].queuedEffectsList[j].done = FALSE;
	}
	
	for (i = 0; i < B_SIDE_COUNT; i++)
	{
		for (j = 0; j < gBattleStruct->sides[i].queuedEffectsCount; j++)
			gBattleStruct->sides[i].queuedEffectsList[j].done = FALSE;
	}
}

// Checkers
bool8 QueuedEffects_DoWishFutureSight(u8 battlerId, u8 id)
{
	bool8 effect = FALSE;
	
	switch (id)
	{
		case B_BATTLER_QUEUED_FUTURE_SIGHT:
		    if (gBattleStruct->battlers[battlerId].futureSightCounter > 0 && --gBattleStruct->battlers[battlerId].futureSightCounter == 0)
			{
				gBattlerTarget = battlerId;
				gBattlerAttacker = gBattleStruct->battlers[battlerId].futureSightAttacker;
				gCurrentMove = gBattleStruct->battlers[battlerId].futureSightMove;
				gSpecialStatuses[gBattlerAttacker].dmg = 0xFFFF;
				gBattleStruct->dynamicMoveType = GetBattlerMoveType(gBattlerAttacker, gCurrentMove);
				BattleScriptExecute(BattleScript_MonTookFutureAttack);
				RemoveBattleEffectFromBattlerQueueList(battlerId, id);
				effect = TRUE;
			}
			break;
		case B_BATTLER_QUEUED_WISH:
		    if (gBattleStruct->battlers[battlerId].wishCounter > 0 && --gBattleStruct->battlers[battlerId].wishCounter == 0)
			{
				if (!(gStatuses3[battlerId] & STATUS3_HEAL_BLOCK))
				{
					gBattlerTarget = battlerId;
					BattleScriptExecute(BattleScript_WishComesTrue);
					effect = TRUE;
				}
				RemoveBattleEffectFromBattlerQueueList(battlerId, id);
			}
			break;
	}
	return effect;
}

bool8 QueuedEffects_DoSeaOfFireAndGMaxEffects(u8 battlerId, u8 side, u8 id)
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

bool8 QueuedEffects_DoEntryHazardsEffects(u8 battlerId, u8 side, u8 id)
{
	u16 currMove;
	bool8 badPoison, effect = FALSE;
	
	if (IsBattlerGrounded(battlerId) && GetBattlerAbility(battlerId) != ABILITY_MAGIC_GUARD)
	{
		switch (id)
		{
			case B_SIDE_QUEUED_SPIKES:
			    if (gSideStatuses[side] & SIDE_STATUS_SPIKES)
				{
					gBattleMoveDamage = gBattleMons[battlerId].maxHP / ((5 - gSideTimers[side].spikesAmount) * 2);
					if (gBattleMoveDamage == 0)
						gBattleMoveDamage = 1;
					SetDmgHazardsBattleScript(B_MSG_HURT_BY_SPIKES);
					effect = TRUE;
				}
				break;
			case B_SIDE_QUEUED_TOXIC_SPIKES:
			    if (gSideStatuses[side] & SIDE_STATUS_TOXIC_SPIKES)
				{
					if (IsBattlerOfType(battlerId, TYPE_POISON)) // Absorb the Toxic Spikes
					{
						gSideStatuses[side] &= ~(SIDE_STATUS_TOXIC_SPIKES);
						gSideTimers[side].toxicSpikesAmount = 0;
						RemoveBattleEffectFromSideQueueList(side, B_SIDE_QUEUED_TOXIC_SPIKES);
						
						gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_ABSORBED_TOXIC_SPIKES;
						BattleScriptCall(BattleScript_PrintEntryHazardsDmgString);
						effect = TRUE;
					}
					else
					{
						badPoison = (gSideTimers[side].toxicSpikesAmount != 1);
						
						currMove = gCurrentMove;
					    gCurrentMove = MOVE_NONE; // So Corrosion won't activate
						
						SaveAttackerToStack(battlerId);
						SaveTargetToStack(battlerId);
						
					    SetMoveEffect(badPoison ? MOVE_EFFECT_TOXIC : MOVE_EFFECT_POISON, TRUE, FALSE);
					    
					    if (DoMoveEffect(FALSE, NULL, STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD | STATUS_CHANGE_FLAG_NO_SYNCHRONISE))
					    {
					    	BattleScriptCall(badPoison ? BattleScript_MoveEffectToxic : BattleScript_MoveEffectPoison);
							effect = TRUE;
					    }
						RestoreTargetFromStack();
						RestoreAttackerFromStack();
					    gCurrentMove = currMove;
					}
				}
			    break;
			case B_SIDE_QUEUED_STEALTH_ROCK:
				break;
			case B_SIDE_QUEUED_STICKY_WEB:
				break;
			case B_SIDE_QUEUED_GMAX_STEELSURGE:
				break;
		}
	}
	
	if (effect)
		gBattleScripting.battler = battlerId;
	
	return effect;
}

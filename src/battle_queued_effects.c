#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_message.h"
#include "battle_queued_effects.h"
#include "battle_scripts.h"
#include "set_effect.h"
#include "constants/battle_move_effects.h"
#include "constants/battle_string_ids.h"
#include "constants/moves.h"

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
		    if (gBattleStruct->futureSightCounter[battlerId] != 0 && --gBattleStruct->futureSightCounter[battlerId] == 0)
			{
				gBattlerTarget = battlerId;
				gBattlerAttacker = gBattleStruct->futureSightAttacker[battlerId];
				gCurrentMove = gBattleStruct->futureSightMove[battlerId];
				gSpecialStatuses[gBattlerAttacker].dmg = 0xFFFF;
				SetTypeBeforeUsingMove(gCurrentMove, gBattlerAttacker);
				BattleScriptExecute(BattleScript_MonTookFutureAttack);
				RemoveBattleEffectFromQueueList(battlerId, id);
				effect = TRUE;
			}
			break;
		case B_QUEUED_WISH:
		    if (gBattleStruct->wishCounter[battlerId] != 0 && --gBattleStruct->wishCounter[battlerId] == 0)
			{
				if (!(gStatuses3[battlerId] & STATUS3_HEAL_BLOCK))
				{
					gBattlerTarget = battlerId;
					BattleScriptExecute(BattleScript_WishComesTrue);
					effect = TRUE;
				}
				RemoveBattleEffectFromQueueList(battlerId, id);
			}
			break;
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
	u16 currMove;
	bool8 badPoison, effect = FALSE;
	
	if (IsBattlerGrounded(battlerId) && GetBattlerAbility(battlerId) != ABILITY_MAGIC_GUARD)
	{
		side = GetBattlerSide(battlerId);
		
		switch (id)
		{
			case B_QUEUED_SPIKES:
			    if (gSideStatuses[side] & SIDE_STATUS_SPIKES)
				{
					gBattleMoveDamage = gBattleMons[battlerId].maxHP / ((5 - gSideTimers[side].spikesAmount) * 2);
					if (gBattleMoveDamage == 0)
						gBattleMoveDamage = 1;
					SetDmgHazardsBattleScript(B_MSG_HURT_BY_SPIKES);
					effect = TRUE;
				}
				break;
			case B_QUEUED_TOXIC_SPIKES:
			    if (gSideStatuses[side] & SIDE_STATUS_TOXIC_SPIKES)
				{
					if (IS_BATTLER_OF_TYPE(battlerId, TYPE_POISON)) // Absorb the Toxic Spikes
					{
						gSideStatuses[side] &= ~(SIDE_STATUS_TOXIC_SPIKES);
						gSideTimers[side].toxicSpikesAmount = 0;
						RemoveBattleEffectFromQueueList(battlerId, B_QUEUED_TOXIC_SPIKES);
						RemoveBattleEffectFromQueueList(BATTLE_PARTNER(battlerId), B_QUEUED_TOXIC_SPIKES);
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
						
					    BattleScriptPushCursor();
						
					    SetMoveEffect(badPoison ? MOVE_EFFECT_TOXIC : MOVE_EFFECT_POISON, TRUE, FALSE);
					    
					    if (DoMoveEffect(FALSE, FALSE, STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD | STATUS_CHANGE_FLAG_NO_SYNCHRONISE))
					    {
					    	BattleScriptPop();
					    	BattleScriptCall(badPoison ? BattleScript_MoveEffectToxic : BattleScript_MoveEffectPoison);
							effect = TRUE;
					    }
					    else
					    	BattleScriptPop();
					    
						RestoreTargetFromStack();
						RestoreAttackerFromStack();
					    gCurrentMove = currMove;
					}
				}
			    break;
			case B_QUEUED_STEALTH_ROCK:
				break;
			case B_QUEUED_STICKY_WEB:
				break;
			case B_QUEUED_STEELSURGE:
				break;
		}
	}
	
	if (effect)
		gBattleScripting.battler = battlerId;
	
	return effect;
}

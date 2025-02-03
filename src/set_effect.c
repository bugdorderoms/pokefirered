#include "global.h"
#include "gflib.h"
#include "random.h"
#include "text.h"
#include "data.h"
#include "main.h"
#include "battle_move_effects.h"
#include "battle.h"
#include "battle_anim.h"
#include "item.h"
#include "util.h"
#include "battle_message.h"
#include "battle_scripts.h"
#include "battle_controllers.h"
#include "evolution_scene.h"
#include "battle_util.h"
#include "battle_script_commands.h"
#include "link.h"
#include "m4a.h"
#include "set_effect.h"
#include "battle_ai.h"
#include "constants/battle_script_commands.h"
#include "constants/items.h"
#include "constants/item_effects.h"
#include "constants/pokemon.h"
#include "constants/abilities.h"
#include "constants/moves.h"
#include "constants/item_effects.h"
#include "constants/hold_effects.h"
#include "constants/battle_string_ids.h"

/////////////////
// MOVE EFFECT //
/////////////////

static const u8 *const sMoveEffectBS_Ptrs[] =
{
    [MOVE_EFFECT_SLEEP]              = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_POISON]             = BattleScript_MoveEffectPoison,
    [MOVE_EFFECT_BURN]               = BattleScript_MoveEffectBurn,
    [MOVE_EFFECT_FREEZE]             = BattleScript_MoveEffectFreeze,
    [MOVE_EFFECT_PARALYSIS]          = BattleScript_MoveEffectParalysis,
    [MOVE_EFFECT_TOXIC]              = BattleScript_MoveEffectToxic,
    [MOVE_EFFECT_CONFUSION]          = BattleScript_MoveEffectConfusion,
    [MOVE_EFFECT_UPROAR]             = BattleScript_MoveEffectUproar,
    [MOVE_EFFECT_PAYDAY]             = BattleScript_MoveEffectPayDay,
    [MOVE_EFFECT_WRAP]               = BattleScript_MoveEffectWrap,
	[MOVE_EFFECT_ATK_PLUS_1]         = BattleScript_StatChange,
	[MOVE_EFFECT_DEF_PLUS_1]         = BattleScript_StatChange,
	[MOVE_EFFECT_SPD_PLUS_1]         = BattleScript_StatChange,
	[MOVE_EFFECT_SP_ATK_PLUS_1]      = BattleScript_StatChange,
	[MOVE_EFFECT_SP_DEF_PLUS_1]      = BattleScript_StatChange,
	[MOVE_EFFECT_ACC_PLUS_1]         = BattleScript_StatChange,
	[MOVE_EFFECT_EVS_PLUS_1]         = BattleScript_StatChange,
	[MOVE_EFFECT_ATK_PLUS_2]         = BattleScript_StatChange,
	[MOVE_EFFECT_DEF_PLUS_2]         = BattleScript_StatChange,
	[MOVE_EFFECT_SPD_PLUS_2]         = BattleScript_StatChange,
	[MOVE_EFFECT_SP_ATK_PLUS_2]      = BattleScript_StatChange,
	[MOVE_EFFECT_SP_DEF_PLUS_2]      = BattleScript_StatChange,
	[MOVE_EFFECT_ACC_PLUS_2]         = BattleScript_StatChange,
	[MOVE_EFFECT_EVS_PLUS_2]         = BattleScript_StatChange,
	[MOVE_EFFECT_ATK_MINUS_1]        = BattleScript_StatChange,
	[MOVE_EFFECT_DEF_MINUS_1]        = BattleScript_StatChange,
	[MOVE_EFFECT_SPD_MINUS_1]        = BattleScript_StatChange,
	[MOVE_EFFECT_SP_ATK_MINUS_1]     = BattleScript_StatChange,
	[MOVE_EFFECT_SP_DEF_MINUS_1]     = BattleScript_StatChange,
	[MOVE_EFFECT_ACC_MINUS_1]        = BattleScript_StatChange,
	[MOVE_EFFECT_EVS_MINUS_1]        = BattleScript_StatChange,
	[MOVE_EFFECT_ATK_MINUS_2]        = BattleScript_StatChange,
	[MOVE_EFFECT_DEF_MINUS_2]        = BattleScript_StatChange,
	[MOVE_EFFECT_SPD_MINUS_2]        = BattleScript_StatChange,
	[MOVE_EFFECT_SP_ATK_MINUS_2]     = BattleScript_StatChange,
	[MOVE_EFFECT_SP_DEF_MINUS_2]     = BattleScript_StatChange,
	[MOVE_EFFECT_ACC_MINUS_2]        = BattleScript_StatChange,
	[MOVE_EFFECT_EVS_MINUS_2]        = BattleScript_StatChange,
	[MOVE_EFFECT_ALL_STATS_UP]       = BattleScript_AllStatsUp,
	[MOVE_EFFECT_CURE_STATUS1]       = BattleScript_TargetStatusHeal,
	[MOVE_EFFECT_ATK_DEF_DOWN]       = BattleScript_MoveEffectAtkDefDown,
	[MOVE_EFFECT_STOCKPILE_WORE_OFF] = BattleScript_MoveEffectStockpileWoreOff,
	[MOVE_EFFECT_FEINT]              = BattleScript_MoveEffectFeint,
	[MOVE_EFFECT_DEF_SPDEF_DOWN]     = BattleScript_MoveEffectDefSpDefDown,
	[MOVE_EFFECT_STEAL_ITEM]         = BattleScript_MoveEffectItemSteal,
	[MOVE_EFFECT_CLEAR_HAZARDS]      = BattleScript_MoveEffectClearHazards,
	[MOVE_EFFECT_KNOCK_OFF]          = BattleScript_MoveEffectKnockOff,
};

static const u8 sTriAttackEffects[] =
{
	MOVE_EFFECT_BURN,
	MOVE_EFFECT_FREEZE,
	MOVE_EFFECT_PARALYSIS
};

void SetMoveEffect(u8 moveEffect, bool8 affectsUser, bool8 certain)
{
	gBattleStruct->moveEffect.moveEffectByte = moveEffect;
	gBattleStruct->moveEffect.affectsUser = affectsUser;
	gBattleStruct->moveEffect.certain = certain;
}

static bool8 IsStatLoweringMoveEffect(u8 moveEffect)
{
	if ((moveEffect >= MOVE_EFFECT_ATK_MINUS_1 && moveEffect <= MOVE_EFFECT_EVS_MINUS_1) || (moveEffect >= MOVE_EFFECT_ATK_MINUS_2 && moveEffect <= MOVE_EFFECT_EVS_MINUS_2))
		return TRUE;
	return FALSE;
}

// moveEffect = the effect that will be applyed
// affectsUser = if the effect affects the user
// certain = if the effect always be applyed (e.g has 100% chance)
// primary = if the effect is the main use (e.g. Thunder Wave)
// scriptStr = if set, then the function will call the scripts of sucess/fail
bool8 DoMoveEffect(bool8 primary, const u8 *scriptStr, u32 flags)
{
	u8 statId;
	s8 buff;
	u8 moveEffect = gBattleStruct->moveEffect.moveEffectByte;
	bool8 affectsUser = gBattleStruct->moveEffect.affectsUser;
	bool8 certain = gBattleStruct->moveEffect.certain;
	u8 ret, effect = 0;
	
	if (affectsUser)
	{
		gEffectBattler = gBattlerAttacker; // BattlerId that effects get applied on
		gBattleScripting.battler = gBattlerTarget; // Theorically the attacker
	}
	else
	{
		gEffectBattler = gBattlerTarget;
        gBattleScripting.battler = gBattlerAttacker;
	}
	
	// Check effects that can prevents the effect to be applyed
	// Any effect other than sleep in't applyed on the Poke Dude battle
	if (((gBattleTypeFlags & BATTLE_TYPE_POKEDUDE) && GetBattlerSide(gEffectBattler) == B_SIDE_OPPONENT && moveEffect != MOVE_EFFECT_SLEEP)
	|| CheckSecondaryEffectsBlockers(gBattleScripting.battler, gEffectBattler, gCurrentMove, moveEffect, affectsUser, primary, flags))
		return FALSE;
	
	switch (moveEffect)
	{
		case MOVE_EFFECT_SLEEP:
			if (IsUproarActive() == gBattlersCount && CanBePutToSleep(gBattleScripting.battler, gEffectBattler, STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD) == STATUS_CHANGE_WORKED)
			{
				gBattleMons[gEffectBattler].status1.id = STATUS1_SLEEP;
#if SLEEP_UPDATE
                gBattleMons[gEffectBattler].status1.counter = RandomRange(2, 3);
#else
	            gBattleMons[gEffectBattler].status1.counter = RandomRange(2, 5);
#endif
				CancelMultiTurnMoves(gEffectBattler);
				effect = 1;
			}
			break;
		case MOVE_EFFECT_POISON:
		    ret = CanBePoisoned(gBattleScripting.battler, gEffectBattler, STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD);
		
			if (ret == STATUS_CHANGE_WORKED)
			{
				gBattleMons[gEffectBattler].status1.id = STATUS1_POISON;
				effect = 1;
			}
			else
			{
				POISON_FAIL_SCRIPTS:
				if (scriptStr != NULL && (primary || certain)) // Basically for Synchronize
				{
					switch (ret)
					{
						case STATUS_CHANGE_FAIL_ABILITY_PREVENTED:
							BattleScriptPush(scriptStr);
							gBattlescriptCurrInstr = BattleScript_ImmunityProtectedRet;
							break;
					}
				}
			}
			break;
		case MOVE_EFFECT_TOXIC:
		    ret = CanBePoisoned(gBattleScripting.battler, gEffectBattler, STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD);
		
			if (ret == STATUS_CHANGE_WORKED)
			{
				gBattleMons[gEffectBattler].status1.id = STATUS1_TOXIC_POISON;
				gBattleMons[gEffectBattler].status1.counter = 0;
				effect = 1;
			}
			else
				goto POISON_FAIL_SCRIPTS;
			break;
		case MOVE_EFFECT_BURN:
		    ret = CanBeBurned(gBattleScripting.battler, gEffectBattler, STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD);
		
			if (ret == STATUS_CHANGE_WORKED)
			{
				gBattleMons[gEffectBattler].status1.id = STATUS1_BURN;
				effect = 1;
			}
			else if (scriptStr != NULL && (primary || certain)) // Basically for Synchronize
			{
				switch (ret)
				{
					case STATUS_CHANGE_FAIL_ABILITY_PREVENTED:
						BattleScriptPush(scriptStr);
						gBattlescriptCurrInstr = BattleScript_WaterVeilProtectedRet;
						break;
				}
			}
			break;
		case MOVE_EFFECT_FREEZE:
			if (CanBeFrozen(gBattleScripting.battler, gEffectBattler, STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD))
			{
				gBattleMons[gEffectBattler].status1.id = STATUS1_FREEZE;
				CancelMultiTurnMoves(gEffectBattler);
				effect = 1;
			}
			break;
		case MOVE_EFFECT_PARALYSIS:
		    ret = CanBeParalyzed(gBattleScripting.battler, gEffectBattler, STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD);
			
			if (ret == STATUS_CHANGE_WORKED)
			{
				gBattleMons[gEffectBattler].status1.id = STATUS1_PARALYSIS;
				effect = 1;
			}
			else if (scriptStr != NULL && (primary || certain)) // Basically for Synchronize
			{
				switch (ret)
				{
					case STATUS_CHANGE_FAIL_ABILITY_PREVENTED:
						BattleScriptPush(scriptStr);
						gBattlescriptCurrInstr = BattleScript_LimberProtectedRet;
						break;
				}
			}
			break;
		case MOVE_EFFECT_CONFUSION:
			if (CanBecameConfused(gBattleScripting.battler, gEffectBattler, STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD) == STATUS_CHANGE_WORKED)
			{
				gBattleMons[gEffectBattler].status2 |= STATUS2_CONFUSION_TURN(RandomRange(2, 5));
				effect = 2;
			}
			break;
		case MOVE_EFFECT_FLINCH:
		    if (GetBattlerAbility(gEffectBattler) == ABILITY_INNER_FOCUS)
			{
				if (scriptStr != NULL && certain)
				{
					BattleScriptPush(scriptStr);
					gBattlescriptCurrInstr = BattleScript_FlinchPrevention;
					break;
				}
			}
			else if (!(gBattleMons[gEffectBattler].status2 & STATUS2_FLINCHED) && GetBattlerTurnOrderNum(gEffectBattler) > gCurrentTurnActionNumber)
			{
				gBattleMons[gEffectBattler].status2 |= STATUS2_FLINCHED;
				effect = 3; // No script
			}
			break;
		case MOVE_EFFECT_TRI_ATTACK:
		    SetMoveEffect(RandomElement(sTriAttackEffects), FALSE, FALSE);
			return DoMoveEffect(primary, scriptStr, 0);
		case MOVE_EFFECT_SECRET_POWER:
			SetMoveEffect(gBattleTerrainTable[gBattleTerrain].secretPowerEffect, FALSE, FALSE);
			return DoMoveEffect(primary, scriptStr, 0);
		case MOVE_EFFECT_UPROAR:
		    if (!(gBattleMons[gEffectBattler].status2 & STATUS2_UPROAR))
			{
				gBattleMons[gEffectBattler].status2 |= STATUS2_MULTIPLETURNS;
				gBattleMons[gEffectBattler].status2 |= STATUS2_UPROAR_TURN(3);
				gBattleStruct->battlers[gEffectBattler].lockedMove = gCurrentMove;
				effect = 2;
			}
			break;
		case MOVE_EFFECT_PAYDAY:
		    // Only scatter coins on the first hit
			if (GetBattlerSide(gBattleScripting.battler) == B_SIDE_PLAYER && gSpecialStatuses[gBattleScripting.battler].parentalBondState != PARENTAL_BOND_2ND_HIT)
			{
				gBattleStruct->battlers[gBattleScripting.battler].payDayLevel = gBattleMons[gBattleScripting.battler].level;
				effect = 2;
			}
			break;
		case MOVE_EFFECT_WRAP:
		    if (gDisableStructs[gEffectBattler].wrapTurns == 0)
			{
				gDisableStructs[gEffectBattler].wrapTurns = RandomRange(4, 5);
				gDisableStructs[gEffectBattler].wrappedBy = gBattleScripting.battler;
				gDisableStructs[gEffectBattler].wrappedMove = gCurrentMove;
				gBattleCommunication[MULTISTRING_CHOOSER] = gBattleMoves[gCurrentMove].argument.bind.trappingId;
				effect = 2;
			}
			break;
		case MOVE_EFFECT_ATK_PLUS_1:
		case MOVE_EFFECT_DEF_PLUS_1:
		case MOVE_EFFECT_SPD_PLUS_1:
		case MOVE_EFFECT_SP_ATK_PLUS_1:
		case MOVE_EFFECT_SP_DEF_PLUS_1:
		case MOVE_EFFECT_ACC_PLUS_1:
		case MOVE_EFFECT_EVS_PLUS_1:
		    statId = moveEffect - MOVE_EFFECT_ATK_PLUS_1 + 1;
			buff = +1;
			
			STAT_CHANGE:
			    if (!NoAliveMonsForEitherParty())
				{
					SetStatChanger(statId, buff);
					
					if (!ChangeStatBuffs(affectsUser ? STAT_CHANGE_FLAG_SELF_INFLICT : 0, FALSE))
					{
						if (certain)
							effect = 2;
					}
					else
						effect = 2;
				}
				break;
		case MOVE_EFFECT_ATK_PLUS_2:
		case MOVE_EFFECT_DEF_PLUS_2:
		case MOVE_EFFECT_SPD_PLUS_2:
		case MOVE_EFFECT_SP_ATK_PLUS_2:
		case MOVE_EFFECT_SP_DEF_PLUS_2:
		case MOVE_EFFECT_ACC_PLUS_2:
		case MOVE_EFFECT_EVS_PLUS_2:
		    statId = moveEffect - MOVE_EFFECT_ATK_PLUS_2 + 1;
			buff = +2;
			goto STAT_CHANGE;
		case MOVE_EFFECT_ATK_MINUS_1:
		case MOVE_EFFECT_DEF_MINUS_1:
		case MOVE_EFFECT_SPD_MINUS_1:
		case MOVE_EFFECT_SP_ATK_MINUS_1:
		case MOVE_EFFECT_SP_DEF_MINUS_1:
		case MOVE_EFFECT_ACC_MINUS_1:
		case MOVE_EFFECT_EVS_MINUS_1:
		    statId = moveEffect - MOVE_EFFECT_ATK_MINUS_1 + 1;
			buff = -1;
			goto STAT_CHANGE;
		case MOVE_EFFECT_ATK_MINUS_2:
		case MOVE_EFFECT_DEF_MINUS_2:
		case MOVE_EFFECT_SPD_MINUS_2:
		case MOVE_EFFECT_SP_ATK_MINUS_2:
		case MOVE_EFFECT_SP_DEF_MINUS_2:
		case MOVE_EFFECT_ACC_MINUS_2:
		case MOVE_EFFECT_EVS_MINUS_2:
		    statId = moveEffect - MOVE_EFFECT_ATK_MINUS_2 + 1;
			buff = -2;
			goto STAT_CHANGE;
		case MOVE_EFFECT_RECHARGE:
		    if (!(gBattleMons[gEffectBattler].status2 & STATUS2_RECHARGE))
			{
				gBattleMons[gEffectBattler].status2 |= STATUS2_RECHARGE;
				gDisableStructs[gEffectBattler].rechargeTimer = 2;
				gBattleStruct->battlers[gEffectBattler].lockedMove = gCurrentMove;
				effect = 3; // No script
			}
			break;
		case MOVE_EFFECT_ALL_STATS_UP:
		case MOVE_EFFECT_ATK_DEF_DOWN:
		case MOVE_EFFECT_DEF_SPDEF_DOWN:
		    if (!NoAliveMonsForEitherParty())
				effect = 2;
			break;
		case MOVE_EFFECT_STOCKPILE_WORE_OFF:
		    gDisableStructs[gEffectBattler].stockpileCounter = 0; // Reset counter
			// fallthrough
		case MOVE_EFFECT_CLEAR_HAZARDS:
		    effect = 2;
			break;
		case MOVE_EFFECT_CURE_STATUS1:
		    if (gBattleMons[gEffectBattler].status1.id == gBattleMoves[gCurrentMove].argument.cureStatus.statusId)
			{
				ClearBattlerStatus(gEffectBattler);
				gBattleCommunication[MULTISTRING_CHOOSER] = gNonVolatileStatusConditions[gBattleMoves[gCurrentMove].argument.cureStatus.statusId - 1].statusCuredByMoveMultistringId;
				effect = 2;
			}
			break;
		case MOVE_EFFECT_THRASH:
		    if (!(gBattleMons[gEffectBattler].status2 & STATUS2_LOCK_CONFUSE))
			{
				gBattleMons[gEffectBattler].status2 |= STATUS2_MULTIPLETURNS;
				gBattleMons[gEffectBattler].status2 |= STATUS2_LOCK_CONFUSE_TURN(RandomRange(2, 3));
				gBattleStruct->battlers[gEffectBattler].lockedMove = gCurrentMove;
				effect = 3; // No script
			}
			break;
		case MOVE_EFFECT_FEINT:
		    if (LiftProtectionEffects(gEffectBattler))
				effect = 2;
			break;
		case MOVE_EFFECT_STEAL_ITEM:
		    if (IsBattlerAlive(gBattleScripting.battler) && CanStealItem(gBattleScripting.battler, gEffectBattler, gBattleMons[gEffectBattler].item))
			{
				if (IsBattlerAlive(gEffectBattler) && GetBattlerAbility(gEffectBattler) == ABILITY_STICKY_HOLD)
				{
					if (scriptStr != NULL && certain)
					{
						BattleScriptPush(scriptStr);
						gBattlescriptCurrInstr = BattleScript_StickyHoldActivates;
					}
				}
				else
				{
					u16 itemId = gBattleMons[gEffectBattler].item;
	
#if STEAL_WILD_ITEM_TO_BAG
					if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) && GetBattlerSide(gBattleScripting.battler) == B_SIDE_PLAYER && GetBattlerSide(gEffectBattler) == B_SIDE_OPPONENT
					&& CheckBagHasSpace(itemId, 1))
						AddBagItem(itemId, 1);
					else
#endif
					{
						GiveItemToBattler(gBattleScripting.battler, itemId);
					}
					RemoveBattlerItem(gEffectBattler);
					
					if (GetBattlerAbility(gEffectBattler) != ABILITY_GORILLA_TACTICS)
						gBattleStruct->battlers[gEffectBattler].choicedMove = MOVE_NONE;
					
					gBattleScripting.animArg1 = gBattleScripting.battler;
					gBattleScripting.animArg2 = gEffectBattler;
					effect = 2;
				}
			}
			break;
		case MOVE_EFFECT_KNOCK_OFF:
		    if (IsBattlerAlive(gBattleScripting.battler) && !NoAliveMonsForEitherParty() && gBattleMons[gEffectBattler].item && CanBattlerGetOrLoseItem(gEffectBattler, gBattleMons[gEffectBattler].item))
			{
				if (IsBattlerAlive(gEffectBattler) && GetBattlerAbility(gEffectBattler) == ABILITY_STICKY_HOLD)
				{
					if (scriptStr != NULL && certain)
					{
						BattleScriptPush(scriptStr);
						gBattlescriptCurrInstr = BattleScript_StickyHoldActivates;
					}
				}
				else
				{
					RemoveBattlerItem(gEffectBattler);
					
					if (GetBattlerAbility(gEffectBattler) != ABILITY_GORILLA_TACTICS)
						gBattleStruct->battlers[gEffectBattler].choicedMove = MOVE_NONE;
					
					effect = 2;
				}
			}
			break;
	}
	
	// Check if the effect worked
	switch (effect)
	{
		case 0: // Failed
			return FALSE;
		case 1: // Update status1
			BtlController_EmitSetMonData(gEffectBattler, BUFFER_A, REQUEST_STATUS_BATTLE, 0, sizeof(gBattleMons[gEffectBattler].status1), &gBattleMons[gEffectBattler].status1);
			MarkBattlerForControllerExec(gEffectBattler);
			
			// For synchronize and Poison Puppeteer
			if (!(flags & STATUS_CHANGE_FLAG_NO_SYNCHRONISE))
			{
				if (moveEffect == MOVE_EFFECT_POISON || moveEffect == MOVE_EFFECT_TOXIC || moveEffect == MOVE_EFFECT_PARALYSIS || moveEffect == MOVE_EFFECT_BURN)
				{
					gBattleStruct->synchronizeMoveEffect = moveEffect;
					gHitMarker |= HITMARKER_SYNCHRONISE_EFFECT;
				}
				
				if (moveEffect == MOVE_EFFECT_POISON || moveEffect == MOVE_EFFECT_TOXIC)
					gBattleStruct->poisonPuppeteerConfusion = TRUE;
			}
			// fallthrough
		case 2: // Try execute script
			if (scriptStr != NULL)
			{
				BattleScriptPush(scriptStr);
				gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[moveEffect];
			}
			// fallthrough
		case 3: // Worked without script
			return TRUE;
	}
}

bool8 CheckSecondaryEffectsBlockers(u8 attacker, u8 defender, u16 move, u8 moveEffect, bool8 affectsUser, bool8 primary, u32 flags)
{
	// Check Safeguard, Flower Veil and Shield Dust preventing the effect
	if (!(flags & STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD))
	{
		// Check Shield Dust
		if (!primary && !affectsUser && GetBattlerAbility(defender) == ABILITY_SHIELD_DUST && moveEffect <= MOVE_EFFECT_SECRET_POWER)
			return TRUE;
		
		// Check safeguard
		if (!primary && CanSafeguardProtectBattler(attacker, defender) && moveEffect <= MOVE_EFFECT_CONFUSION)
			return TRUE;
		
		// Check Flower Veil
		if (!affectsUser && IsBattlerProtectedByFlowerVeil(defender) && (moveEffect <= MOVE_EFFECT_CONFUSION || IsStatLoweringMoveEffect(moveEffect)))
			return TRUE;
	}
	
	// Check if target is't alive
	if (!IsBattlerAlive(defender) && moveEffect != MOVE_EFFECT_PAYDAY && moveEffect != MOVE_EFFECT_STEAL_ITEM && moveEffect != MOVE_EFFECT_KNOCK_OFF
	&& moveEffect != MOVE_EFFECT_FEINT && moveEffect != MOVE_EFFECT_CLEAR_HAZARDS)
		return TRUE;
	
	// Check substitute
	if (!(flags & STATUS_CHANGE_FLAG_IGNORE_SUBSTITUTE) && !affectsUser && SubsBlockMove(attacker, defender, move))
		return TRUE;
	
	return FALSE;
}

/////////////////
// STAT CHANGE //
/////////////////

void SetStatChanger(u8 statId, s8 buff)
{
	gBattleStruct->statChange.statId = statId;
	gBattleStruct->statChange.buff = buff;
	gBattleStruct->statChange.str = NULL;
	gBattleStruct->statChange.mirrorArmorState = 0;
}

u8 CheckStatDecreaseBlockEffects(u8 attacker, u8 target, u8 statId, u8 flags)
{
	if ((gSideStatuses[GetBattlerSide(target)] & SIDE_STATUS_MIST) && GetBattlerAbility(attacker) != ABILITY_INFILTRATOR) // Check Mist
		return STAT_CHANGE_FAIL_MIST;
	else if (!(flags & STAT_CHANGE_FLAG_IGNORE_PROTECT) && JumpIfMoveAffectedByProtect(0, gBattlescriptCurrInstr)) // Check protect
		return STAT_CHANGE_FAIL_PROTECTED;
	else if (IsBattlerProtectedByFlowerVeil(target)) // Check Flower Veil
		return STAT_CHANGE_FAIL_FLOWER_VEIL;
	
	switch (GetBattlerAbility(target))
	{
		case ABILITY_KEEN_EYE:
		case ABILITY_ILLUMINATE:
		case ABILITY_MINDS_EYE:
		    if (statId == STAT_ACC)
				return STAT_CHANGE_FAIL_ABILITY_PREVENT_SPECIFIC_STAT;
			break;
		case ABILITY_HYPER_CUTTER:
		    if (statId == STAT_ATK)
				return STAT_CHANGE_FAIL_ABILITY_PREVENT_SPECIFIC_STAT;
			break;
		case ABILITY_BIG_PECKS:
		    if (statId == STAT_DEF)
				return STAT_CHANGE_FAIL_ABILITY_PREVENT_SPECIFIC_STAT;
			break;
		case ABILITY_CLEAR_BODY:
		case ABILITY_WHITE_SMOKE:
		case ABILITY_FULL_METAL_BODY:
		    return STAT_CHANGE_FAIL_ABILITY_PREVENTED;
	}
	return STAT_CHANGE_WORKED;
}

// onlyChecks = used for playstatchangeanimation to check if other stats can be changed
bool8 ChangeStatBuffs(u8 flags, bool8 onlyChecks)
{
	u8 result = STAT_CHANGE_WORKED;
	u8 statId = gBattleStruct->statChange.statId;
	s8 buff = gBattleStruct->statChange.buff;
	bool8 selfInflict;
	
	gBattleStruct->statChange.maxOut = (buff == +6 || buff == -6);
	gBattleStruct->statChange.flags = flags; // For playstatchangeanimation
	
	selfInflict = (flags & STAT_CHANGE_FLAG_SELF_INFLICT);
	
	if (selfInflict)
		gEffectBattler = gBattlerAttacker;
	else
	{
		gEffectBattler = gBattlerTarget;
		
		// Check Mirror Armor
		if (!onlyChecks && GetBattlerAbility(gEffectBattler) == ABILITY_MIRROR_ARMOR && buff < 0 && !(flags & STAT_CHANGE_FLAG_NO_MIRROR_ARMOR))
		{
			u8 temp;
			bool8 worked;
			
			flags |= STAT_CHANGE_FLAG_NO_MIRROR_ARMOR;
			++gBattleStruct->statChange.mirrorArmorState; // Bounced back
			
			// For the correct stat change target
			SWAP(gBattlerAttacker, gBattlerTarget, temp);
			worked = ChangeStatBuffs(flags, FALSE);
			SWAP(gBattlerAttacker, gBattlerTarget, temp);
			
			return worked;
		}
	}
	gSpecialStatuses[gEffectBattler].changedStatsBattlerId = gBattlerAttacker; // The battler that caused the stat to change, for Defiant
	
	// Check abilities that changes the buff value
	switch (GetBattlerAbility(gEffectBattler))
	{
		case ABILITY_SIMPLE:
		    buff *= 2; // Double buff
			break;
		case ABILITY_CONTRARY:
		    buff *= -1; // Invert buff
			break;
	}
	
	// Clamp buff to max and min values
	if (buff > +6)
		buff = +6;
	else if (buff < -6)
		buff = -6;
	
	if (buff > 0) // Stat increase
	{
		if (gBattleMons[gEffectBattler].statStages[statId] < MAX_STAT_STAGES)
		{
			if (!onlyChecks) // Stat can be increased, stop here
			{
				if (gBattleStruct->statChange.maxOut)
					gBattleMons[gEffectBattler].statStages[statId] = MAX_STAT_STAGES;
				else
				{
					gBattleMons[gEffectBattler].statStages[statId] += buff;
					if (gBattleMons[gEffectBattler].statStages[statId] > MAX_STAT_STAGES)
						gBattleMons[gEffectBattler].statStages[statId] = MAX_STAT_STAGES;
				}
			}
		}
		else
			result = STAT_CHANGE_FAIL_WONT_CHANGE;
	}
	else if (selfInflict || (result = CheckStatDecreaseBlockEffects(gBattlerAttacker, gEffectBattler, statId, flags)) == STAT_CHANGE_WORKED) // Stat decrease
	{
		if (gBattleMons[gEffectBattler].statStages[statId] > MIN_STAT_STAGES)
		{
		   	if (!onlyChecks) // Stat can be decreased, stop here
		   	{
				if (gBattleStruct->statChange.maxOut)
					gBattleMons[gEffectBattler].statStages[statId] = MIN_STAT_STAGES;
				else
				{
					gBattleMons[gEffectBattler].statStages[statId] += buff;
					if (gBattleMons[gEffectBattler].statStages[statId] < MIN_STAT_STAGES)
						gBattleMons[gEffectBattler].statStages[statId] = MIN_STAT_STAGES;
				}
		   	}
		}
		else
			result = STAT_CHANGE_FAIL_WONT_CHANGE;
	}
	gBattleStruct->statChange.buff = buff; // Store new buff in case it was changed
	gBattleStruct->statChange.result = result;
	
	return (result == STAT_CHANGE_WORKED);
}

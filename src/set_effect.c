#include "global.h"
#include "gflib.h"
#include "random.h"
#include "text.h"
#include "data.h"
#include "main.h"
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
#include "battle_ai_script_commands.h"
#include "constants/battle_script_commands.h"
#include "constants/items.h"
#include "constants/item_effects.h"
#include "constants/pokemon.h"
#include "constants/abilities.h"
#include "constants/moves.h"
#include "constants/item_effects.h"
#include "constants/hold_effects.h"
#include "constants/battle_move_effects.h"
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
	[MOVE_EFFECT_SMELLING_SALT]      = BattleScript_TargetStatusHeal,
	[MOVE_EFFECT_WAKE_UP_SLAP]       = BattleScript_TargetStatusHeal,
	[MOVE_EFFECT_ATK_DEF_DOWN]       = BattleScript_MoveEffectAtkDefDown,
	[MOVE_EFFECT_STOCKPILE_WORE_OFF] = BattleScript_MoveEffectStockpileWoreOff,
	[MOVE_EFFECT_FEINT]              = BattleScript_MoveEffectFeint,
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

#define INCREMENT_RETURN      \
{                             \
    ++gBattlescriptCurrInstr; \
	return FALSE;             \
}

// moveEffect = the effect that will be applyed
// affectsUser = if the effect affects the user
// certain = if the effect always be applyed (e.g has 100% chance)
// primary = if the effect is the main use (e.g. Thunder Wave)
bool8 DoMoveEffect(bool8 primary, bool8 jumpToScript, u32 flags)
{
	u8 statId;
	s8 buff;
	u8 moveEffect = gBattleStruct->moveEffect.moveEffectByte;
	bool8 affectsUser = gBattleStruct->moveEffect.affectsUser;
	bool8 certain = gBattleStruct->moveEffect.certain;
	u8 ret, effect = 0;
	u16 defAbility;
	
	if (affectsUser)
	{
		gEffectBattler = gBattlerAttacker; // BattlerId that effects get applied on
		gBattleScripting.battler = gBattlerTarget; // Theoretically the attacker
	}
	else
	{
		gEffectBattler = gBattlerTarget;
        gBattleScripting.battler = gBattlerAttacker;
	}
	defAbility = GetBattlerAbility(gEffectBattler);
	
	// Check effects that can prevents the effect to be applyed
	
	// Any effect other than sleep in't applyed on the Poke Dude battle
	if ((gBattleTypeFlags & BATTLE_TYPE_POKEDUDE) && GetBattlerSide(gEffectBattler) == B_SIDE_OPPONENT && moveEffect != MOVE_EFFECT_SLEEP)
		INCREMENT_RETURN
	
	// Check Safeguard, Flower Veil and Shield Dust preventing the effect
	if (!(flags & STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD))
	{
		// Check Shield Dust
		if (!primary && !affectsUser && defAbility == ABILITY_SHIELD_DUST && moveEffect <= MOVE_EFFECT_SECRET_POWER)
			INCREMENT_RETURN
		
		if (!primary && CanSafeguardProtectBattler(gBattleScripting.battler, gEffectBattler) && moveEffect <= MOVE_EFFECT_CONFUSION)
			INCREMENT_RETURN
		
		if (!affectsUser && IsBattlerProtectedByFlowerVeil(gEffectBattler) && (moveEffect <= MOVE_EFFECT_CONFUSION || IsStatLoweringMoveEffect(moveEffect)))
			INCREMENT_RETURN
	}
	
	// Check if target is't alive
	if (!IsBattlerAlive(gEffectBattler) && moveEffect != MOVE_EFFECT_PAYDAY && moveEffect != MOVE_EFFECT_STEAL_ITEM && moveEffect != MOVE_EFFECT_FEINT)
		INCREMENT_RETURN
	
	// Check substitute
	if (!(flags & STATUS_CHANGE_FLAG_IGNORE_SUBSTITUTE) && !affectsUser && SubsBlockMove(gBattleScripting.battler, gEffectBattler, gCurrentMove))
		INCREMENT_RETURN
	
	flags = STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD; // Ignore it since it's already checked above
	
	switch (moveEffect)
	{
		case MOVE_EFFECT_SLEEP:
			if (IsUproarActive() == gBattlersCount && CanBePutToSleep(gBattleScripting.battler, gEffectBattler, flags) == STATUS_CHANGE_WORKED)
			{
#if SLEEP_UPDATE
                gBattleMons[gEffectBattler].status1 |= STATUS1_SLEEP_TURN(((Random() & 2) + 1));
#else
	            gBattleMons[gEffectBattler].status1 |= STATUS1_SLEEP_TURN(((Random() & 3) + 2));
#endif
				CancelMultiTurnMoves(gEffectBattler);
				effect = 1;
			}
			break;
		case MOVE_EFFECT_POISON:
		    ret = CanBePoisoned(gBattleScripting.battler, gEffectBattler, flags);
		
			if (ret == STATUS_CHANGE_WORKED)
			{
				gBattleMons[gEffectBattler].status1 |= STATUS1_POISON;
				effect = 1;
			}
			else
			{
				POISON_FAIL_SCRIPTS:
				if (primary || certain) // Basically for Synchronize
				{
					switch (ret)
					{
						case STATUS_CHANGE_FAIL_ABILITY_PREVENTED:
						    gLastUsedAbility = defAbility;
							RecordAbilityBattle(gEffectBattler, gLastUsedAbility);
							BattleScriptPush(gBattlescriptCurrInstr + 1);
							gBattlescriptCurrInstr = BattleScript_ImmunityProtectedRet;
							return FALSE;
					}
				}
			}
			break;
		case MOVE_EFFECT_TOXIC:
		    ret = CanBePoisoned(gBattleScripting.battler, gEffectBattler, flags);
		
			if (ret == STATUS_CHANGE_WORKED)
			{
				gBattleMons[gEffectBattler].status1 |= STATUS1_TOXIC_POISON;
				effect = 1;
			}
			else
				goto POISON_FAIL_SCRIPTS;
			break;
		case MOVE_EFFECT_BURN:
		    ret = CanBeBurned(gBattleScripting.battler, gEffectBattler, flags);
		
			if (ret == STATUS_CHANGE_WORKED)
			{
				gBattleMons[gEffectBattler].status1 |= STATUS1_BURN;
				effect = 1;
			}
			else if (primary || certain) // Basically for Synchronize
			{
				switch (ret)
				{
					case STATUS_CHANGE_FAIL_ABILITY_PREVENTED:
					    gLastUsedAbility = defAbility;
						RecordAbilityBattle(gEffectBattler, gLastUsedAbility);
						BattleScriptPush(gBattlescriptCurrInstr + 1);
						gBattlescriptCurrInstr = BattleScript_WaterVeilProtectedRet;
						return FALSE;
				}
			}
			break;
		case MOVE_EFFECT_FREEZE:
			if (CanBeFrozen(gBattleScripting.battler, gEffectBattler, flags))
			{
				gBattleMons[gEffectBattler].status1 |= STATUS1_FREEZE;
				CancelMultiTurnMoves(gEffectBattler);
				effect = 1;
			}
			break;
		case MOVE_EFFECT_PARALYSIS:
		    ret = CanBeParalyzed(gBattleScripting.battler, gEffectBattler, flags);
			
			if (ret == STATUS_CHANGE_WORKED)
			{
				gBattleMons[gEffectBattler].status1 |= STATUS1_PARALYSIS;
				effect = 1;
			}
			else if (primary || certain) // Basically for Synchronize
			{
				switch (ret)
				{
					case STATUS_CHANGE_FAIL_ABILITY_PREVENTED:
					    gLastUsedAbility = defAbility;
						RecordAbilityBattle(gEffectBattler, gLastUsedAbility);
						BattleScriptPush(gBattlescriptCurrInstr + 1);
						gBattlescriptCurrInstr = BattleScript_LimberProtectedRet;
						return FALSE;
				}
			}
			break;
		case MOVE_EFFECT_CONFUSION:
			if (CanBecameConfused(gBattleScripting.battler, gEffectBattler, flags) == STATUS_CHANGE_WORKED)
			{
				gBattleMons[gEffectBattler].status2 |= STATUS2_CONFUSION_TURN((Random() % 4) + 2);
				effect = 2;
			}
			break;
		case MOVE_EFFECT_FLINCH:
		    if (defAbility == ABILITY_INNER_FOCUS)
			{
				if (certain)
				{
					gLastUsedAbility = defAbility;
					RecordAbilityBattle(gEffectBattler, gLastUsedAbility);
					gBattlescriptCurrInstr = BattleScript_FlinchPrevention;
					return FALSE;
				}
				else
					break;
			}
			else if (!(gBattleMons[gEffectBattler].status2 & STATUS2_FLINCHED) && GetBattlerTurnOrderNum(gEffectBattler) > gCurrentTurnActionNumber)
			{
				gBattleMons[gEffectBattler].status2 |= STATUS2_FLINCHED;
				effect = 3; // No script
			}
			break;
		case MOVE_EFFECT_TRI_ATTACK:
		    SetMoveEffect(Random() % 3 + MOVE_EFFECT_BURN, FALSE, FALSE);
			return DoMoveEffect(primary, jumpToScript, 0);
		case MOVE_EFFECT_SECRET_POWER:
		    if (gSpecialStatuses[gBattleScripting.battler].parentalBondState != PARENTAL_BOND_1ST_HIT) // Only apply on final hit
			{
				SetMoveEffect(GetSecretPowerEffect(), FALSE, FALSE);
				return DoMoveEffect(primary, jumpToScript, 0);
			}
			break;
		case MOVE_EFFECT_UPROAR:
		    if (!(gBattleMons[gEffectBattler].status2 & STATUS2_UPROAR))
			{
				gBattleMons[gEffectBattler].status2 |= STATUS2_MULTIPLETURNS;
				gBattleMons[gEffectBattler].status2 |= STATUS2_UPROAR_TURN(3);
				gLockedMoves[gEffectBattler] = gCurrentMove;
				effect = 2;
			}
			break;
		case MOVE_EFFECT_PAYDAY:
		    // Only scatter coins on the first hit
			if (GetBattlerSide(gBattleScripting.battler) == B_SIDE_PLAYER && gSpecialStatuses[gBattleScripting.battler].parentalBondState != PARENTAL_BOND_2ND_HIT)
			{
				gBattleStruct->payDayLevels[(GetBattlerPosition(gBattleScripting.battler) != B_POSITION_PLAYER_LEFT)] = gBattleMons[gBattleScripting.battler].level;
				effect = 2;
			}
			break;
		case MOVE_EFFECT_WRAP:
		    if (!(gBattleMons[gEffectBattler].status2 & STATUS2_WRAPPED))
			{
				gBattleMons[gEffectBattler].status2 |= STATUS2_WRAPPED_TURN((Random() & 1) + 4);
				gBattleStruct->wrappedMove[gEffectBattler] = gCurrentMove;
				gBattleStruct->wrappedBy[gEffectBattler] = gBattleScripting.battler;
				gBattleCommunication[MULTISTRING_CHOOSER] = GetTrappingIdByMove(gCurrentMove);
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
				gLockedMoves[gEffectBattler] = gCurrentMove;
				effect = 3; // No script
			}
			break;
		case MOVE_EFFECT_ALL_STATS_UP:
		case MOVE_EFFECT_ATK_DEF_DOWN:
		    if (!NoAliveMonsForEitherParty())
				effect = 2;
			break;
		case MOVE_EFFECT_STOCKPILE_WORE_OFF:
		    // Reset counter
		    gDisableStructs[gEffectBattler].stockpileCounter = 0;
		    effect = 2;
			break;
		case MOVE_EFFECT_SMELLING_SALT:
		    if (gSpecialStatuses[gBattleScripting.battler].parentalBondState != PARENTAL_BOND_1ST_HIT && (gBattleMons[gEffectBattler].status1 & STATUS1_PARALYSIS))
			{
				ClearBattlerStatus(gEffectBattler);
				gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_CURED_BUFF1;
				effect = 2;
			}
			break;
		case MOVE_EFFECT_WAKE_UP_SLAP:
		    if (gSpecialStatuses[gBattleScripting.battler].parentalBondState != PARENTAL_BOND_1ST_HIT && (gBattleMons[gEffectBattler].status1 & STATUS1_SLEEP))
			{
				ClearBattlerStatus(gEffectBattler);
				gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_WOKEUP_EFFECT;
				effect = 2;
			}
			break;
		case MOVE_EFFECT_THRASH:
		    if (!(gBattleMons[gEffectBattler].status2 & STATUS2_LOCK_CONFUSE))
			{
				gBattleMons[gEffectBattler].status2 |= STATUS2_MULTIPLETURNS;
				gBattleMons[gEffectBattler].status2 |= STATUS2_LOCK_CONFUSE_TURN((Random() & 1) + 2);
				gLockedMoves[gEffectBattler] = gCurrentMove;
				effect = 3; // No script
			}
			break;
		case MOVE_EFFECT_FEINT:
		    if (LiftProtectionEffects(gEffectBattler))
				effect = 2;
			break;
		case MOVE_EFFECT_KNOCK_OFF: // TODO:
		    if (gBattleMons[gEffectBattler].item)
			{
				if (IsBattlerAlive(gEffectBattler) && defAbility == ABILITY_STICKY_HOLD)
				{
					gLastUsedAbility = defAbility;
					RecordAbilityBattle(gEffectBattler, gLastUsedAbility);
					gBattlescriptCurrInstr = BattleScript_StickyHoldActivates;
				}
				else
				{
					gLastUsedItem = gBattleMons[gEffectBattler].item;
					gBattleMons[gEffectBattler].item = ITEM_NONE;
					gWishFutureKnock.knockedOffMons[GetBattlerSide(gEffectBattler)] |= gBitTable[gBattlerPartyIndexes[gEffectBattler]];
					CheckSetBattlerUnburden(gEffectBattler);
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = BattleScript_KnockedOff;
					gBattleStruct->choicedMove[gEffectBattler] = MOVE_NONE;
				}
				return TRUE;
			}
			break;
		case MOVE_EFFECT_STEAL_ITEM: // TODO
		    if (CanStealItem(gBattleScripting.battler, gEffectBattler, gBattleMons[gEffectBattler].item))
			{
				if (gBattleMons[gEffectBattler].item && IsBattlerAlive(gEffectBattler) && defAbility == ABILITY_STICKY_HOLD)
				{
					gLastUsedAbility = defAbility;
					RecordAbilityBattle(gEffectBattler, gLastUsedAbility);
					gBattlescriptCurrInstr = BattleScript_StickyHoldActivates;
					return TRUE;
				}
				else if (!gBattleMons[gBattleScripting.battler].item && gBattleMons[gEffectBattler].item)
				{
					StealTargetItem(gBattleScripting.battler, gEffectBattler);
					gBattleStruct->changedItems[gBattleScripting.battler] = gLastUsedItem;
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = BattleScript_ItemSteal;
					return TRUE;
				}
			}
			break;
	}
	
	// Check if the effect worked
	switch (effect)
	{
		case 0: // Failed
			INCREMENT_RETURN
		
		case 1: // Update status1
			BtlController_EmitSetMonData(gEffectBattler, BUFFER_A, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gEffectBattler].status1);
			MarkBattlerForControllerExec(gEffectBattler);
			
			// For synchronize
			if (moveEffect == MOVE_EFFECT_POISON || moveEffect == MOVE_EFFECT_TOXIC || moveEffect == MOVE_EFFECT_PARALYSIS || moveEffect == MOVE_EFFECT_BURN)
			{
				gBattleStruct->synchronizeMoveEffect = moveEffect;
				gHitMarker |= HITMARKER_SYNCHRONISE_EFFECT;
			}
			// fallthrough
		case 2: // Try execute script
			if (jumpToScript)
			{
				BattleScriptPush(gBattlescriptCurrInstr + 1);
				gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[moveEffect];
				return TRUE;
			}
			// fallthrough
		case 3: // Worked without script
		    ++gBattlescriptCurrInstr;
			return TRUE;
	}
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

static u8 CheckStatDecreaseBlockEffects(u8 battlerId, u8 statId, u8 flags)
{
	if ((gSideStatuses[GetBattlerSide(battlerId)] & SIDE_STATUS_MIST) && GetBattlerAbility(gBattlerAttacker) != ABILITY_INFILTRATOR) // Check Mist
		return STAT_CHANGE_FAIL_MIST;
	else if (!(flags & STAT_CHANGE_FLAG_IGNORE_PROTECT) && JumpIfMoveAffectedByProtect(0, gBattlescriptCurrInstr)) // Check protect
		return STAT_CHANGE_FAIL_PROTECTED;
	else if (IsBattlerProtectedByFlowerVeil(battlerId)) // Check Flower Veil
		return STAT_CHANGE_FAIL_FLOWER_VEIL;
	
	switch (GetBattlerAbility(battlerId))
	{
		case ABILITY_KEEN_EYE:
		case ABILITY_ILLUMINATE:
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
	else if (selfInflict || (result = CheckStatDecreaseBlockEffects(gEffectBattler, statId, flags)) == STAT_CHANGE_WORKED) // Stat decrease
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

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

static const u8 *const sMoveEffectBS_Ptrs[] =
{
    [MOVE_EFFECT_SLEEP]            = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_POISON]           = BattleScript_MoveEffectPoison,
    [MOVE_EFFECT_BURN]             = BattleScript_MoveEffectBurn,
    [MOVE_EFFECT_FREEZE]           = BattleScript_MoveEffectFreeze,
    [MOVE_EFFECT_PARALYSIS]        = BattleScript_MoveEffectParalysis,
    [MOVE_EFFECT_TOXIC]            = BattleScript_MoveEffectToxic,
    [MOVE_EFFECT_CONFUSION]        = BattleScript_MoveEffectConfusion,
    [MOVE_EFFECT_UPROAR]           = BattleScript_MoveEffectUproar,
    [MOVE_EFFECT_PAYDAY]           = BattleScript_MoveEffectPayDay,
    [MOVE_EFFECT_WRAP]             = BattleScript_MoveEffectWrap,
	[MOVE_EFFECT_ATK_PLUS_1]       = BattleScript_StatUp,
	[MOVE_EFFECT_DEF_PLUS_1]       = BattleScript_StatUp,
	[MOVE_EFFECT_SPD_PLUS_1]       = BattleScript_StatUp,
	[MOVE_EFFECT_SP_ATK_PLUS_1]    = BattleScript_StatUp,
	[MOVE_EFFECT_SP_DEF_PLUS_1]    = BattleScript_StatUp,
	[MOVE_EFFECT_ACC_PLUS_1]       = BattleScript_StatUp,
	[MOVE_EFFECT_EVS_PLUS_1]       = BattleScript_StatUp,
	[MOVE_EFFECT_ATK_PLUS_2]       = BattleScript_StatUp,
	[MOVE_EFFECT_DEF_PLUS_2]       = BattleScript_StatUp,
	[MOVE_EFFECT_SPD_PLUS_2]       = BattleScript_StatUp,
	[MOVE_EFFECT_SP_ATK_PLUS_2]    = BattleScript_StatUp,
	[MOVE_EFFECT_SP_DEF_PLUS_2]    = BattleScript_StatUp,
	[MOVE_EFFECT_ACC_PLUS_2]       = BattleScript_StatUp,
	[MOVE_EFFECT_EVS_PLUS_2]       = BattleScript_StatUp,
	[MOVE_EFFECT_ATK_MINUS_1]      = BattleScript_StatDown,
	[MOVE_EFFECT_DEF_MINUS_1]      = BattleScript_StatDown,
	[MOVE_EFFECT_SPD_MINUS_1]      = BattleScript_StatDown,
	[MOVE_EFFECT_SP_ATK_MINUS_1]   = BattleScript_StatDown,
	[MOVE_EFFECT_SP_DEF_MINUS_1]   = BattleScript_StatDown,
	[MOVE_EFFECT_ACC_MINUS_1]      = BattleScript_StatDown,
	[MOVE_EFFECT_EVS_MINUS_1]      = BattleScript_StatDown,
	[MOVE_EFFECT_ATK_MINUS_2]      = BattleScript_StatDown,
	[MOVE_EFFECT_DEF_MINUS_2]      = BattleScript_StatDown,
	[MOVE_EFFECT_SPD_MINUS_2]      = BattleScript_StatDown,
	[MOVE_EFFECT_SP_ATK_MINUS_2]   = BattleScript_StatDown,
	[MOVE_EFFECT_SP_DEF_MINUS_2]   = BattleScript_StatDown,
	[MOVE_EFFECT_ACC_MINUS_2]      = BattleScript_StatDown,
	[MOVE_EFFECT_EVS_MINUS_2]      = BattleScript_StatDown,
	[MOVE_EFFECT_ALL_STATS_UP]     = BattleScript_AllStatsUp,
	[MOVE_EFFECT_RAPIDSPIN]        = BattleScript_RapidSpinAway,
	[MOVE_EFFECT_REMOVE_STATUS]    = BattleScript_TargetStatusHeal,
	[MOVE_EFFECT_ATK_DEF_DOWN]     = BattleScript_AtkDefDown,
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
		if (!primary && defAbility == ABILITY_SHIELD_DUST && moveEffect <= MOVE_EFFECT_TRI_ATTACK)
			INCREMENT_RETURN
		
		if (!primary && CanSafeguardProtectBattler(gBattleScripting.battler, gEffectBattler) && moveEffect <= MOVE_EFFECT_CONFUSION)
			INCREMENT_RETURN
		
		if (!affectsUser && IsBattlerProtectedByFlowerVeil(gEffectBattler) && (moveEffect <= MOVE_EFFECT_CONFUSION || IsStatLoweringMoveEffect(moveEffect)))
			INCREMENT_RETURN
	}
	
	// Check if target is't alive
	if (!IsBattlerAlive(gEffectBattler) && moveEffect != MOVE_EFFECT_PAYDAY && moveEffect != MOVE_EFFECT_STEAL_ITEM)
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
			return DoMoveEffect(FALSE, TRUE, 0);
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
		    if (!NoAliveMonsForEitherParty() && ChangeStatBuffs(SET_STAT_BUFF_VALUE(1), moveEffect - MOVE_EFFECT_ATK_PLUS_1 + 1, affectsUser ? MOVE_EFFECT_AFFECTS_USER : 0, NULL) == STAT_CHANGE_WORKED)
			{
				gBattleScripting.animArg1 = moveEffect;
				gBattleScripting.animArg2 = 0;
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
		    if (!NoAliveMonsForEitherParty() && ChangeStatBuffs(SET_STAT_BUFF_VALUE(2), moveEffect - MOVE_EFFECT_ATK_PLUS_2 + 1, affectsUser ? MOVE_EFFECT_AFFECTS_USER : 0, NULL) == STAT_CHANGE_WORKED)
			{
				gBattleScripting.animArg1 = moveEffect;
				gBattleScripting.animArg2 = 0;
				effect = 2;
			}
			break;
		case MOVE_EFFECT_ATK_MINUS_1:
		case MOVE_EFFECT_DEF_MINUS_1:
		case MOVE_EFFECT_SPD_MINUS_1:
		case MOVE_EFFECT_SP_ATK_MINUS_1:
		case MOVE_EFFECT_SP_DEF_MINUS_1:
		case MOVE_EFFECT_ACC_MINUS_1:
		case MOVE_EFFECT_EVS_MINUS_1:
		    if (!NoAliveMonsForEitherParty() && ChangeStatBuffs(SET_STAT_BUFF_VALUE(1) | STAT_BUFF_NEGATIVE, moveEffect - MOVE_EFFECT_ATK_MINUS_1 + 1, affectsUser ? MOVE_EFFECT_AFFECTS_USER : 0, NULL) == STAT_CHANGE_WORKED)
			{
				gBattleScripting.animArg1 = moveEffect;
				gBattleScripting.animArg2 = 0;
				effect = 2;
			}
			break;
		case MOVE_EFFECT_ATK_MINUS_2:
		case MOVE_EFFECT_DEF_MINUS_2:
		case MOVE_EFFECT_SPD_MINUS_2:
		case MOVE_EFFECT_SP_ATK_MINUS_2:
		case MOVE_EFFECT_SP_DEF_MINUS_2:
		case MOVE_EFFECT_ACC_MINUS_2:
		case MOVE_EFFECT_EVS_MINUS_2:
		    if (!NoAliveMonsForEitherParty() && ChangeStatBuffs(SET_STAT_BUFF_VALUE(2) | STAT_BUFF_NEGATIVE, moveEffect - MOVE_EFFECT_ATK_MINUS_2 + 1, affectsUser ? MOVE_EFFECT_AFFECTS_USER : 0, NULL) == STAT_CHANGE_WORKED)
			{
				gBattleScripting.animArg1 = moveEffect;
				gBattleScripting.animArg2 = 0;
				effect = 2;
			}
			break;
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
		    if (!NoAliveMonsForEitherParty())
				effect = 2;
			break;
		case MOVE_EFFECT_RAPIDSPIN:
		    effect = 2;
			break;
		case MOVE_EFFECT_REMOVE_STATUS:
		    if (gSpecialStatuses[gBattleScripting.battler].parentalBondState != PARENTAL_BOND_1ST_HIT && (gBattleMons[gEffectBattler].status1 & gBattleMoves[gCurrentMove].argument))
			{
				ClearBattlerStatus(gEffectBattler);
				gBattleCommunication[MULTISTRING_CHOOSER] = gBattleMoves[gCurrentMove].argument == STATUS1_SLEEP ? B_MSG_WOKEUP_EFFECT : B_MSG_CURED_BUFF1;
				effect = 2;
			}
			break;
		case MOVE_EFFECT_ATK_DEF_DOWN:
		    if (!NoAliveMonsForEitherParty())
				effect = 2;
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

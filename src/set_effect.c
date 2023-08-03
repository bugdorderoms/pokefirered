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
#include "constants/inserts.h"

void SetMoveEffect(u8 moveEffect, bool8 affectsUser, bool8 certain)
{
	gBattleStruct->moveEffectByte = moveEffect;
	gBattleStruct->affectsUser = affectsUser;
	gBattleStruct->moveEffectCertain = certain;
}

static const u16 sFinalStrikeOnlyEffects[] =
{
	EFFECT_RELIC_SONG,
    EFFECT_BUG_BITE,
    EFFECT_THIEF,
    EFFECT_BURN_UP,
    EFFECT_SECRET_POWER,
    EFFECT_SMACK_DOWN,
    EFFECT_SPARKLING_ARIA,
    EFFECT_SMELLINGSALT,
    EFFECT_WAKE_UP_SLAP,
    EFFECT_HIT_ESCAPE,
    EFFECT_RECOIL_HP_25,
    EFFECT_HIT_PREVENT_ESCAPE,
    EFFECT_HIT_SWITCH_TARGET,
};

const u32 gStatusFlagsForMoveEffects[] =
{
    [MOVE_EFFECT_SLEEP]          = STATUS1_SLEEP,
    [MOVE_EFFECT_POISON]         = STATUS1_POISON,
    [MOVE_EFFECT_BURN]           = STATUS1_BURN,
    [MOVE_EFFECT_FREEZE]         = STATUS1_FREEZE,
    [MOVE_EFFECT_PARALYSIS]      = STATUS1_PARALYSIS,
    [MOVE_EFFECT_TOXIC]          = STATUS1_TOXIC_POISON,
    [MOVE_EFFECT_CONFUSION]      = STATUS2_CONFUSION,
    [MOVE_EFFECT_FLINCH]         = STATUS2_FLINCHED,
    [MOVE_EFFECT_UPROAR]         = STATUS2_UPROAR,
    [MOVE_EFFECT_CHARGING]       = STATUS2_MULTIPLETURNS,
    [MOVE_EFFECT_WRAP]           = STATUS2_WRAPPED,
    [MOVE_EFFECT_RECHARGE]       = STATUS2_RECHARGE,
    [MOVE_EFFECT_PREVENT_ESCAPE] = STATUS2_ESCAPE_PREVENTION,
    [MOVE_EFFECT_NIGHTMARE]      = STATUS2_NIGHTMARE,
    [MOVE_EFFECT_THRASH]         = STATUS2_LOCK_CONFUSE,
};

static const u8 *const sMoveEffectBS_Ptrs[] =
{
    [MOVE_EFFECT_SLEEP]     = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_POISON]    = BattleScript_MoveEffectPoison,
    [MOVE_EFFECT_BURN]      = BattleScript_MoveEffectBurn,
    [MOVE_EFFECT_FREEZE]    = BattleScript_MoveEffectFreeze,
    [MOVE_EFFECT_PARALYSIS] = BattleScript_MoveEffectParalysis,
    [MOVE_EFFECT_TOXIC]     = BattleScript_MoveEffectToxic,
    [MOVE_EFFECT_CONFUSION] = BattleScript_MoveEffectConfusion,
    [MOVE_EFFECT_UPROAR]    = BattleScript_MoveEffectUproar,
    [MOVE_EFFECT_PAYDAY]    = BattleScript_MoveEffectPayDay,
    [MOVE_EFFECT_WRAP]      = BattleScript_MoveEffectWrap,
};

static bool8 IsFinalStrikeMoveEffect(u16 move)
{
	u8 i;
	u16 moveEffect = gBattleMoves[move].effect;
	
	for (i = 0; i < NELEMS(sFinalStrikeOnlyEffects); i++)
	{
		if (sFinalStrikeOnlyEffects[i] == moveEffect)
			return TRUE;
	}
	return FALSE;
}

#define SAFEGUARD_CHECK                                \
{                                                      \
	if (gHitMarker & HITMARKER_IGNORE_SAFEGUARD)       \
	{                                                  \
		gBattleCommunication[MULTISTRING_CHOOSER] = 1; \
		gHitMarker &= ~(HITMARKER_IGNORE_SAFEGUARD);   \
	}                                                  \
	else                                               \
		gBattleCommunication[MULTISTRING_CHOOSER] = 0; \
}

void DoMoveEffect(bool8 primary)
{
	u8 moveEffect = gBattleStruct->moveEffectByte;
	u16 defAbility;
	bool8 affectsUser = gBattleStruct->affectsUser, certain = gBattleStruct->moveEffectCertain, statusChanged = FALSE;
	const u8 *BS_Ptr = gBattlescriptCurrInstr + 1;
	
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
	
	// Check effects that prevents the use of effects
	// Skip effects that happens only after the last hit of Parental Bond
	if (gSpecialStatuses[gBattlerAttacker].parentalBondState == PARENTAL_BOND_1ST_HIT && IsFinalStrikeMoveEffect(gCurrentMove)
	&& IsBattlerAlive(gBattlerTarget))
	{
	    gBattlescriptCurrInstr = BS_Ptr;
		return;
	}
	// Check effects blocked in the poke dude battle
	else if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE && GetBattlerSide(gEffectBattler) == B_SIDE_OPPONENT && moveEffect != MOVE_EFFECT_SLEEP)
	{
		gBattlescriptCurrInstr = BS_Ptr;
		return;
	}
	// Check abilities
	else if (defAbility == ABILITY_SHIELD_DUST && !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD) && moveEffect <= MOVE_EFFECT_TRI_ATTACK
	&& !primary)
	{
		gBattlescriptCurrInstr = BS_Ptr;
		return;
	}
	// Check Safeguard
	else if (gSideStatuses[GetBattlerSide(gEffectBattler)] & SIDE_STATUS_SAFEGUARD && !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD) && !primary
	&& moveEffect <= MOVE_EFFECT_CONFUSION && GetBattlerAbility(gBattlerAttacker) != ABILITY_INFILTRATOR)
	{
		gBattlescriptCurrInstr = BS_Ptr;
		return;
	}
	// Check if target is't alive
	else if (!IsBattlerAlive(gEffectBattler) && moveEffect != MOVE_EFFECT_PAYDAY && moveEffect != MOVE_EFFECT_STEAL_ITEM)
	{
		gBattlescriptCurrInstr = BS_Ptr;
		return;
	}
	// Check Substitute
	else if (SubsBlockMove(gBattlerAttacker, gEffectBattler, gCurrentMove) && !affectsUser)
	{
		gBattlescriptCurrInstr = BS_Ptr;
		return;
	}
	
	if (moveEffect <= MOVE_EFFECT_TOXIC) // Status change
	{
		switch (gStatusFlagsForMoveEffects[moveEffect])
		{
			case STATUS1_SLEEP:
				if (defAbility != ABILITY_SOUNDPROOF) // Check active Uproar
					for (gActiveBattler = 0; gActiveBattler < gBattlersCount && !(gBattleMons[gActiveBattler].status2 & STATUS2_UPROAR); ++gActiveBattler);
				else
					gActiveBattler = gBattlersCount;
				
				if (gActiveBattler == gBattlersCount && CanBePutToSleep(gEffectBattler, FALSE))
				{
					CancelMultiTurnMoves(gEffectBattler);
					statusChanged = TRUE;
				}
				break;
			case STATUS1_POISON:
			    // Check poison immunity abilities
			    if ((defAbility == ABILITY_IMMUNITY || defAbility == ABILITY_PASTEL_VEIL) && (primary || certain))
				{
					gLastUsedAbility = defAbility;
					RecordAbilityBattle(gEffectBattler, gLastUsedAbility);
					BattleScriptPush(BS_Ptr);
					gBattlescriptCurrInstr = BattleScript_PSNPrevention;
					SAFEGUARD_CHECK;
					return;
				}
				else if (!CanPoisonType(gBattleScripting.battler, gEffectBattler) && gHitMarker & HITMARKER_IGNORE_SAFEGUARD && (primary || certain))
				{
					BattleScriptPush(BS_Ptr);
					gBattlescriptCurrInstr = BattleScript_PSNPrevention;
					gBattleCommunication[MULTISTRING_CHOOSER] = 2;
					return;
				}
				else if (CanBePoisoned(gEffectBattler, gBattleScripting.battler, FALSE))
					statusChanged = TRUE;
				break;
			case STATUS1_TOXIC_POISON:
			    // Check poison immunity abilities
			    if ((defAbility == ABILITY_IMMUNITY || defAbility == ABILITY_PASTEL_VEIL) && (primary || certain))
				{
					gLastUsedAbility = defAbility;
					RecordAbilityBattle(gEffectBattler, gLastUsedAbility);
					BattleScriptPush(BS_Ptr);
					gBattlescriptCurrInstr = BattleScript_PSNPrevention;
					SAFEGUARD_CHECK;
					return;
				}
				else if (!CanPoisonType(gBattleScripting.battler, gEffectBattler) && gHitMarker & HITMARKER_IGNORE_SAFEGUARD && (primary || certain))
				{
					BattleScriptPush(BS_Ptr);
					gBattlescriptCurrInstr = BattleScript_PSNPrevention;
					gBattleCommunication[MULTISTRING_CHOOSER] = 2;
					return;
				}
				else if (gBattleMons[gEffectBattler].status1)
					break;
				else if (CanBePoisoned(gEffectBattler, gBattleScripting.battler, FALSE))
					statusChanged = TRUE;
				else
					gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
				break;
			case STATUS1_BURN:
			    // Check burn immunity abilities
				if ((defAbility == ABILITY_WATER_VEIL || defAbility == ABILITY_WATER_BUBBLE) && (primary || certain))
				{
					gLastUsedAbility = defAbility;
					RecordAbilityBattle(gEffectBattler, gLastUsedAbility);
					BattleScriptPush(BS_Ptr);
					gBattlescriptCurrInstr = BattleScript_BRNPrevention;
					SAFEGUARD_CHECK;
					return;
				}
				else if (IS_BATTLER_OF_TYPE(gEffectBattler, TYPE_FIRE) && gHitMarker & HITMARKER_IGNORE_SAFEGUARD && (primary || certain))
				{
					BattleScriptPush(BS_Ptr);
					gBattlescriptCurrInstr = BattleScript_BRNPrevention;
					gBattleCommunication[MULTISTRING_CHOOSER] = 2;
					return;
				}
				else if (CanBeBurned(gEffectBattler, FALSE))
					statusChanged = TRUE;
				break;
			case STATUS1_FREEZE:
			    if (CanBeFrozen(gEffectBattler, FALSE))
				{
					CancelMultiTurnMoves(gEffectBattler);
					statusChanged = TRUE;
				}
				break;
			case STATUS1_PARALYSIS:
			    // Check paralysis immunity abilities
			    if (defAbility == ABILITY_LIMBER)
				{
					if (primary || certain)
					{
						gLastUsedAbility = defAbility;
						RecordAbilityBattle(gEffectBattler, gLastUsedAbility);
						BattleScriptPush(BS_Ptr);
						gBattlescriptCurrInstr = BattleScript_PRLZPrevention;
						SAFEGUARD_CHECK;
						return;
					}
					else
						break;
				}
				else if (!IS_BATTLER_OF_TYPE(gEffectBattler, TYPE_ELECTRIC) && gHitMarker & HITMARKER_IGNORE_SAFEGUARD && (primary || certain))
				{
					BattleScriptPush(BS_Ptr);
					gBattlescriptCurrInstr = BattleScript_PRLZPrevention;
					gBattleCommunication[MULTISTRING_CHOOSER] = 2;
					return;
				}
				else if (CanBeParalyzed(gEffectBattler, FALSE))
					statusChanged = TRUE;
				break;
		}
		if (statusChanged)
		{
			BattleScriptPush(BS_Ptr);
			
			if (gStatusFlagsForMoveEffects[moveEffect] == STATUS1_SLEEP) // Set sleep counter
#if SLEEP_UPDATE
                gBattleMons[gEffectBattler].status1 |= ((Random() & 2) + 1);
#else
	            gBattleMons[gEffectBattler].status1 |= ((Random() & 3) + 2);
#endif
            else
				gBattleMons[gEffectBattler].status1 |= gStatusFlagsForMoveEffects[moveEffect];
			
			gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[moveEffect];
			gActiveBattler = gEffectBattler;
            BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gEffectBattler].status1);
            MarkBattlerForControllerExec(gActiveBattler);
			SAFEGUARD_CHECK;
			// For synchronize
			if (moveEffect == MOVE_EFFECT_POISON || moveEffect == MOVE_EFFECT_TOXIC || moveEffect == MOVE_EFFECT_PARALYSIS || moveEffect == MOVE_EFFECT_BURN)
			{
				gBattleStruct->synchronizeMoveEffect = moveEffect;
				gHitMarker |= HITMARKER_SYNCHRONISE_EFFECT;
			}
			return;
		}
	}
	else
	{
		if (!(gBattleMons[gEffectBattler].status2 & gStatusFlagsForMoveEffects[moveEffect]))
		{
			switch (moveEffect)
			{
				case MOVE_EFFECT_CONFUSION:
				    if (defAbility != ABILITY_OWN_TEMPO)
					{
						gBattleMons[gEffectBattler].status2 |= (((Random()) % 0x4)) + 2;
						BattleScriptPush(BS_Ptr);
						gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[moveEffect];
						return;
					}
					break;
				case MOVE_EFFECT_FLINCH:
				    if (defAbility == ABILITY_INNER_FOCUS)
					{
						if (primary || certain)
						{
							gLastUsedAbility = defAbility;
							RecordAbilityBattle(gEffectBattler, gLastUsedAbility);
							gBattlescriptCurrInstr = BattleScript_FlinchPrevention;
							return;
						}
					}
					else if (GetBattlerTurnOrderNum(gEffectBattler) > gCurrentTurnActionNumber)
						gBattleMons[gEffectBattler].status2 |= gStatusFlagsForMoveEffects[moveEffect];
					break;
				case MOVE_EFFECT_UPROAR:
				    if (!(gBattleMons[gEffectBattler].status2 & STATUS2_UPROAR))
					{
						gBattleMons[gEffectBattler].status2 |= STATUS2_MULTIPLETURNS;
						gBattleMons[gEffectBattler].status2 |= ((Random() & 3) + 2) << 4;
						gLockedMoves[gEffectBattler] = gCurrentMove;
						BattleScriptPush(BS_Ptr);
						gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[moveEffect];
						return;
					}
					break;
				case MOVE_EFFECT_PAYDAY:
				    if (GetBattlerSide(gBattleScripting.battler) == B_SIDE_PLAYER && gSpecialStatuses[gBattleScripting.battler].parentalBondState != PARENTAL_BOND_2ND_HIT)
					{
						u16 payDayMoney = gPaydayMoney + (gBattleMons[gBattleScripting.battler].level * 5);
						
						if (payDayMoney > 0xFFFF)
							gPaydayMoney = 0xFFFF;
						else
							gPaydayMoney = payDayMoney;
						
						BattleScriptPush(BS_Ptr);
						gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[moveEffect];
						return;
					}
					break;
				case MOVE_EFFECT_TRI_ATTACK:
				    if (!gBattleMons[gEffectBattler].status1)
					{
						SetMoveEffect(Random() % 3 + 3, FALSE, FALSE);
						DoMoveEffect(FALSE);
						return;
					}
					break;
				case MOVE_EFFECT_CHARGING:
				    gBattleMons[gEffectBattler].status2 |= STATUS2_MULTIPLETURNS;
					gLockedMoves[gEffectBattler] = gCurrentMove;
					gProtectStructs[gEffectBattler].chargingTurn = 1;
					break;
				case MOVE_EFFECT_WRAP:
				    if (!(gBattleMons[gEffectBattler].status2 & STATUS2_WRAPPED))
					{
						gBattleMons[gEffectBattler].status2 |= ((Random() & 3) + 3) << 0xD;
						gBattleStruct->wrappedMove[gEffectBattler] = gCurrentMove;
						gBattleStruct->wrappedBy[gEffectBattler] = gBattleScripting.battler;
						BattleScriptPush(BS_Ptr);
						gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[moveEffect];
						
						for (gBattleCommunication[MULTISTRING_CHOOSER] = 0; ; ++gBattleCommunication[MULTISTRING_CHOOSER])
						{
							if (gBattleCommunication[MULTISTRING_CHOOSER] > 4 || gTrappingMoves[gBattleCommunication[MULTISTRING_CHOOSER]] == gCurrentMove)
								break;
						}
						return;
					}
					break;
				case MOVE_EFFECT_ATK_PLUS_1:
				case MOVE_EFFECT_DEF_PLUS_1:
				case MOVE_EFFECT_SPD_PLUS_1:
				case MOVE_EFFECT_SP_ATK_PLUS_1:
				case MOVE_EFFECT_SP_DEF_PLUS_1:
				case MOVE_EFFECT_ACC_PLUS_1:
				case MOVE_EFFECT_EVS_PLUS_1:
				    if (!NoAliveMonsForEitherParty() && ChangeStatBuffs(SET_STAT_BUFF_VALUE(1), moveEffect - MOVE_EFFECT_ATK_PLUS_1 + 1,
					affectsUser ? MOVE_EFFECT_AFFECTS_USER : 0, NULL) == STAT_CHANGE_WORKED)
				    {
						gBattleScripting.animArg1 = moveEffect;
						gBattleScripting.animArg2 = 0;
						BattleScriptPush(BS_Ptr);
						gBattlescriptCurrInstr = BattleScript_StatUp;
						return;
					}
					break;
				case MOVE_EFFECT_ATK_MINUS_1:
				case MOVE_EFFECT_DEF_MINUS_1:
				case MOVE_EFFECT_SPD_MINUS_1:
				case MOVE_EFFECT_SP_ATK_MINUS_1:
				case MOVE_EFFECT_SP_DEF_MINUS_1:
				case MOVE_EFFECT_ACC_MINUS_1:
				case MOVE_EFFECT_EVS_MINUS_1:
				    if (!NoAliveMonsForEitherParty() && ChangeStatBuffs(SET_STAT_BUFF_VALUE(1) | STAT_BUFF_NEGATIVE, moveEffect - MOVE_EFFECT_ATK_MINUS_1 + 1,
					affectsUser ? MOVE_EFFECT_AFFECTS_USER : 0, NULL) == STAT_CHANGE_WORKED)
					{
						gBattleScripting.animArg1 = moveEffect;
						gBattleScripting.animArg2 = 0;
						BattleScriptPush(BS_Ptr);
						gBattlescriptCurrInstr = BattleScript_StatDown;
						return;
					}
					break;
				case MOVE_EFFECT_ATK_PLUS_2:
				case MOVE_EFFECT_DEF_PLUS_2:
				case MOVE_EFFECT_SPD_PLUS_2:
				case MOVE_EFFECT_SP_ATK_PLUS_2:
				case MOVE_EFFECT_SP_DEF_PLUS_2:
				case MOVE_EFFECT_ACC_PLUS_2:
				case MOVE_EFFECT_EVS_PLUS_2:
				    if (!NoAliveMonsForEitherParty() && ChangeStatBuffs(SET_STAT_BUFF_VALUE(2), moveEffect - MOVE_EFFECT_ATK_PLUS_2 + 1,
					affectsUser ? MOVE_EFFECT_AFFECTS_USER : 0, NULL) == STAT_CHANGE_WORKED)
				    {
						gBattleScripting.animArg1 = moveEffect;
						gBattleScripting.animArg2 = 0;
						BattleScriptPush(BS_Ptr);
						gBattlescriptCurrInstr = BattleScript_StatUp;
						return;
					}
					break;
				case MOVE_EFFECT_ATK_MINUS_2:
				case MOVE_EFFECT_DEF_MINUS_2:
				case MOVE_EFFECT_SPD_MINUS_2:
				case MOVE_EFFECT_SP_ATK_MINUS_2:
				case MOVE_EFFECT_SP_DEF_MINUS_2:
				case MOVE_EFFECT_ACC_MINUS_2:
				case MOVE_EFFECT_EVS_MINUS_2:
				    if (!NoAliveMonsForEitherParty() && ChangeStatBuffs(SET_STAT_BUFF_VALUE(2) | STAT_BUFF_NEGATIVE, moveEffect - MOVE_EFFECT_ATK_MINUS_2 + 1,
					affectsUser ? MOVE_EFFECT_AFFECTS_USER : 0, NULL) == STAT_CHANGE_WORKED)
					{
						gBattleScripting.animArg1 = moveEffect;
						gBattleScripting.animArg2 = 0;
						BattleScriptPush(BS_Ptr);
						gBattlescriptCurrInstr = BattleScript_StatDown;
						return;
					}
					break;
				case MOVE_EFFECT_RECHARGE:
				    gBattleMons[gEffectBattler].status2 |= STATUS2_RECHARGE;
					gDisableStructs[gEffectBattler].rechargeTimer = 2;
					gLockedMoves[gEffectBattler] = gCurrentMove;
					break;
				case MOVE_EFFECT_RAGE:
				    gBattleMons[gBattleScripting.battler].status2 |= STATUS2_RAGE;
					break;
				case MOVE_EFFECT_STEAL_ITEM:
				    if (CanStealItem(gBattleScripting.battler, gEffectBattler, gBattleMons[gEffectBattler].item))
					{
						if (gBattleMons[gEffectBattler].item && IsBattlerAlive(gEffectBattler) && defAbility == ABILITY_STICKY_HOLD)
						{
							gLastUsedAbility = defAbility;
							RecordAbilityBattle(gEffectBattler, gLastUsedAbility);
							gBattlescriptCurrInstr = BattleScript_StickyHoldActivates;
							return;
						}
						else if (!gBattleMons[gBattleScripting.battler].item && gBattleMons[gEffectBattler].item)
						{
							StealTargetItem(gBattleScripting.battler, gEffectBattler);
							gBattleStruct->changedItems[gBattleScripting.battler] = gLastUsedItem;
							BattleScriptPush(BS_Ptr);
							gBattlescriptCurrInstr = BattleScript_ItemSteal;
							return;
						}
					}
					break;
				case MOVE_EFFECT_PREVENT_ESCAPE:
				    gBattleMons[gEffectBattler].status2 |= STATUS2_ESCAPE_PREVENTION;
					gDisableStructs[gEffectBattler].battlerPreventingEscape = gBattleScripting.battler;
					break;
				case MOVE_EFFECT_NIGHTMARE:
				    gBattleMons[gEffectBattler].status2 |= STATUS2_NIGHTMARE;
					break;
				case MOVE_EFFECT_ALL_STATS_UP:
				    if (!NoAliveMonsForEitherParty())
					{
						BattleScriptPush(BS_Ptr);
						gBattlescriptCurrInstr = BattleScript_AllStatsUp;
						return;
					}
					break;
				case MOVE_EFFECT_RAPIDSPIN:
				    BattleScriptPush(BS_Ptr);
					gBattlescriptCurrInstr = BattleScript_RapidSpinAway;
					return;
				case MOVE_EFFECT_REMOVE_PARALYSIS:
				    if (gBattleMons[gEffectBattler].status1 & STATUS1_PARALYSIS)
					{
						ClearBattlerStatus(gEffectBattler);
						BattleScriptPush(BS_Ptr);
						gBattlescriptCurrInstr = BattleScript_TargetPRLZHeal;
						return;
					}
					break;
				case MOVE_EFFECT_ATK_DEF_DOWN:
				    if (!NoAliveMonsForEitherParty())
					{
						BattleScriptPush(BS_Ptr);
						gBattlescriptCurrInstr = BattleScript_AtkDefDown;
						return;
					}
					break;
				case MOVE_EFFECT_THRASH:
				    if (!(gBattleMons[gEffectBattler].status2 & STATUS2_LOCK_CONFUSE))
					{
						gBattleMons[gEffectBattler].status2 |= STATUS2_MULTIPLETURNS;
						gBattleMons[gEffectBattler].status2 |= (((Random() & 1) + 2) << 0xA);
						gLockedMoves[gEffectBattler] = gCurrentMove;
					}
					break;
				case MOVE_EFFECT_KNOCK_OFF:
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
							BattleScriptPush(BS_Ptr);
							gBattlescriptCurrInstr = BattleScript_KnockedOff;
							gBattleStruct->choicedMove[gEffectBattler] = MOVE_NONE;
						}
						return;
					}
					break; 
			}
		}
	}
	gBattlescriptCurrInstr = BS_Ptr;
}

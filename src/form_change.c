#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "dns.h"
#include "event_data.h"
#include "form_change.h"
#include "item.h"
#include "item_menu.h"
#include "party_menu.h"
#include "pokemon.h"
#include "sound.h"
#include "util.h"
#include "constants/form_change.h"
#include "constants/pokedex.h"
#include "constants/pokemon.h"

u16 GetSpeciesFormChange(u16 formChangeType, u16 species, u32 personality, u16 ability, u16 itemId, u8 battlerId, bool8 knowsMove)
{
	u8 i;
	u16 param, targetSpecies = SPECIES_NONE;
	const struct FormChange *formsTable = gFormChangeTable[species];
	
	if (formsTable != NULL)
	{
		for (i = 0; formsTable[i].method != FORM_CHANGE_TERMINATOR; i++)
		{
			if (formsTable[i].method == formChangeType)
			{
				param = formsTable[i].param;
				
				switch (formsTable[i].method)
				{
					case FORM_CHANGE_GENDER:
					    if (param == GetGenderFromSpeciesAndPersonality(species, personality))
							targetSpecies = formsTable[i].targetSpecies;
					    break;
					case FORM_CHANGE_PERSONALITY:
					    targetSpecies = formsTable[personality % param].targetSpecies;
						break;
					case FORM_CHANGE_SEASON:
					    if (param == DNSGetCurrentSeason())
							targetSpecies = formsTable[i].targetSpecies;
						break;
					case FORM_CHANGE_HOLD_ITEM:
					case FORM_CHANGE_MEGA_EVO: // TODO:
					case FORM_CHANGE_PRIMAL:
					case FORM_CHANGE_ULTRA_BURST:
					    if (!formsTable[i].param2 || ability == formsTable[i].param2)
						{
							if (param == itemId || !param)
								targetSpecies = formsTable[i].targetSpecies;
						}
						break;
					case FORM_CHANGE_TERRAIN:
					    if (param == gBattleTerrain)
							targetSpecies = formsTable[i].targetSpecies;
						break;
					case FORM_CHANGE_HP:
					    if (formsTable[i].param2 == HP_FORM_GT)
						{
							if (gBattleMons[battlerId].hp > gBattleMons[battlerId].maxHP / param)
								targetSpecies = formsTable[i].targetSpecies;
						}
						else
						{
							if (gBattleMons[battlerId].hp <= gBattleMons[battlerId].maxHP / param)
								targetSpecies = formsTable[i].targetSpecies;
						}
						break;
					case FORM_CHANGE_WEATHER:
					    if (formsTable[i].param2)
						{
							if (formsTable[i].param2 != ability && !gSpecialStatuses[battlerId].removedWeatherChangeAbility)
								break;
						}
					    if ((!param && (!gBattleWeather || !WEATHER_HAS_EFFECT)) || IsBattlerWeatherAffected(battlerId, param))
							targetSpecies = formsTable[i].targetSpecies;
						break;
					case FORM_CHANGE_SWITCH_OUT:
					case FORM_CHANGE_START_BATTLE:
					case FORM_CHANGE_WITHDRAW:
					    targetSpecies = formsTable[i].targetSpecies;
						break;
					case FORM_CHANGE_NATURE:
					    switch (param)
						{
							case NATURE_FORM_AMPED:
							    switch (GetNatureFromPersonality(personality))
								{
									case NATURE_HARDY:
									case NATURE_BRAVE:
									case NATURE_ADAMANT:
									case NATURE_NAUGHTY:
									case NATURE_DOCILE:
									case NATURE_IMPISH:
									case NATURE_LAX:
									case NATURE_HASTY:
									case NATURE_JOLLY:
									case NATURE_NAIVE:
									case NATURE_RASH:
									case NATURE_SASSY:
									case NATURE_QUIRKY:
									    targetSpecies = formsTable[i].targetSpecies;
										break;
								}
								break;
							case NATURE_FORM_LOW_KEY:
							    switch (GetNatureFromPersonality(personality))
								{
									case NATURE_LONELY:
									case NATURE_BOLD:
									case NATURE_RELAXED:
									case NATURE_TIMID:
									case NATURE_SERIOUS:
									case NATURE_MODEST:
									case NATURE_MILD:
									case NATURE_QUIET:
									case NATURE_BASHFUL:
									case NATURE_CALM:
									case NATURE_GENTLE:
									case NATURE_CAREFUL:
									    targetSpecies = formsTable[i].targetSpecies;
										break;
								}
								break;
						}
						break;
					case FORM_CHANGE_TIME:
					    if (param == GetDNSTimeLapseIsNight())
							targetSpecies = formsTable[i].targetSpecies;
						break;
					case FORM_CHANGE_KNOW_MOVE:
					    if (formsTable[i].param2 == knowsMove)
							targetSpecies = formsTable[i].targetSpecies;
						break;
					case FORM_CHANGE_USE_ITEM:
					    if (param == gSpecialVar_ItemId)
						{
							targetSpecies = formsTable[i].targetSpecies;
							
							if (species == targetSpecies && formsTable[i].param2) // if already transformed revert to it's original form
							    targetSpecies = formsTable[i].param2;
						}
						break;
					case FORM_CHANGE_ENDTURN:
					    if (param == species)
							targetSpecies = formsTable[i].targetSpecies;
						break;
					case FORM_CHANGE_MOVE:
					    if (param == gCurrentMove)
							targetSpecies = formsTable[i].targetSpecies;
						break;
					case FORM_CHANGE_MOVE_SPLIT:
					    if ((formsTable[i].param2 && param != GetMoveSplit(gCurrentMove))
							|| (!formsTable[i].param2 && param == GetMoveSplit(gCurrentMove)))
							targetSpecies = formsTable[i].targetSpecies;
						break;
				}
			}
		}
	}
	return targetSpecies;
}

static u16 SpeciesHasFormChangeType(u16 species, u16 formChangeType)
{
	u8 i;
	const struct FormChange *formsTable = gFormChangeTable[species];
	
	if (formsTable != NULL)
	{
		for (i = 0; formsTable[i].method != FORM_CHANGE_TERMINATOR; i++)
		{
			if (formsTable[i].method == formChangeType)
				return formsTable[i].param;
		}
	}
	return FORM_CHANGE_TERMINATOR;
}

///////////////////////////
// OVERWORLD FORM CHANGE //
///////////////////////////

u16 GetMonFormChangeSpecies(struct Pokemon *mon, u16 formChangeType)
{
	u16 species = GetMonData(mon, MON_DATA_SPECIES), item = GetMonData(mon, MON_DATA_HELD_ITEM), ability = GetMonAbility(mon);
	u32 personality = GetMonData(mon, MON_DATA_PERSONALITY);
	u16 param = SpeciesHasFormChangeType(species, FORM_CHANGE_KNOW_MOVE);
	bool8 hasWantedMoveKnow = FALSE;
	
	if (param != FORM_CHANGE_TERMINATOR)
		hasWantedMoveKnow = MonKnowsMove(mon, param);
	
	return GetSpeciesFormChange(formChangeType, species, personality, ability, item, 0, hasWantedMoveKnow);
}

u16 DoOverworldFormChange(struct Pokemon *mon, u16 formChangeType)
{
	u16 targetSpecies = GetMonFormChangeSpecies(mon, formChangeType);
	
	if (targetSpecies && targetSpecies < NUM_SPECIES && targetSpecies != GetMonData(mon, MON_DATA_SPECIES))
	{
		SetMonData(mon, MON_DATA_SPECIES, &targetSpecies);
		CalculateMonStats(mon);
		
		if (formChangeType == FORM_CHANGE_HOLD_ITEM) // update mon icon sprite if it's in pc or in party
		{
			if (gMain.inPc)
				UpdatePcMonIconSpecies();
			else if (gMain.inParty)
			{
				UpdateCurrentPartyMonIconSpecies(targetSpecies);
				PlayCry1(targetSpecies, 0);
			}
		}
	}
	return GetMonData(mon, MON_DATA_SPECIES);
}

u16 DoWildEncounterFormChange(struct Pokemon *mon)
{
	u8 i;
	u16 wildEncounterFormChange[] =
	{
		FORM_CHANGE_GENDER, FORM_CHANGE_PERSONALITY, FORM_CHANGE_SEASON, FORM_CHANGE_NATURE, FORM_CHANGE_TERMINATOR
	};
	
	for (i = 0; wildEncounterFormChange[i] != FORM_CHANGE_TERMINATOR; i++)
		DoOverworldFormChange(mon, wildEncounterFormChange[i]);
	
	GiveMonInitialMoveset(mon); // Other forms may have diferent movesets
	
	return GetMonData(mon, MON_DATA_SPECIES);
}

void DoPlayerPartyEndBattleFormChange(void)
{
	u8 i;
	
	for (i = 0; i < PARTY_SIZE; i++)
	{
		DoSpecialFormChange(0xFF, i, FORM_CHANGE_END_BATTLE); // revert battle forms back
		
		if (gBattleStruct->appearedInBattle & gBitTable[i]) // only change form if appeared in battle
			DoOverworldFormChange(&gPlayerParty[i], FORM_CHANGE_TERRAIN); // update Burmy form
	}
}

////////////////////////
// BATTLE FORM CHANGE //
////////////////////////

void DoBattleFormChange(u8 battlerId, u16 newSpecies, bool8 reloadTypes, bool8 reloadAbility)
{
	struct Pokemon *mon = GetBattlerPartyIndexPtr(battlerId);
	
	SetMonData(mon, MON_DATA_SPECIES, &newSpecies);
	CalculateMonStats(mon);
	
	gBattleMons[battlerId].species = newSpecies;
	gBattleMons[battlerId].hp = GetMonData(mon, MON_DATA_HP);
	gBattleMons[battlerId].maxHP = GetMonData(mon, MON_DATA_MAX_HP);
	gBattleMons[battlerId].attack = GetMonData(mon, MON_DATA_ATK);
	gBattleMons[battlerId].defense = GetMonData(mon, MON_DATA_DEF);
	gBattleMons[battlerId].speed = GetMonData(mon, MON_DATA_SPEED);
	gBattleMons[battlerId].spAttack = GetMonData(mon, MON_DATA_SPATK);
	gBattleMons[battlerId].spDefense = GetMonData(mon, MON_DATA_SPDEF);
	HANDLE_POWER_TRICK_SWAP(battlerId);
	
	if (reloadAbility)
		gBattleMons[battlerId].ability = GetMonAbility(mon);
	
	if (reloadTypes)
	{
		gBattleMons[battlerId].type1 = gBaseStats[newSpecies].type1;
		gBattleMons[battlerId].type2 = gBaseStats[newSpecies].type2;
		gBattleMons[battlerId].type3 = TYPE_MYSTERY;
	}
	ClearIllusionMon(battlerId);
}

u16 TryDoBattleFormChange(u8 battlerId, u16 formChangeType)
{
	u16 ability, item, species, personalitySpecies, targetSpecies = SPECIES_NONE;
	u32 personality;
	
	if (!(gBattleMons[battlerId].status2 & STATUS2_TRANSFORMED)) // no change form if transformed
	{
		item = gBattleMons[battlerId].item; // form change items are not affected by Kluts, etc.
		personality = gBattleMons[battlerId].personality;
		species = gBattleMons[battlerId].species;
		ability = GetBattlerAbility(battlerId);
		targetSpecies = GetSpeciesFormChange(formChangeType, species, personality, ability, item, battlerId, FALSE);
		personalitySpecies = GetSpeciesFormChange(FORM_CHANGE_PERSONALITY, targetSpecies, personality, ability, item, 0, FALSE);
		
		if (personalitySpecies) // handle minior forms
		    targetSpecies = personalitySpecies;
			
		if (targetSpecies == species)
			targetSpecies = SPECIES_NONE;
	}
	return targetSpecies;
}

void DoSpecialFormChange(u8 battlerId, u8 partyId, u16 formChangeType)
{
	u16 targetSpecies;
	struct Pokemon *mon;
	
	if (battlerId != 0xFF)
		mon = GetBattlerPartyIndexPtr(battlerId);
	
	switch (formChangeType)
	{
		case FORM_CHANGE_SWITCH_OUT:
		    if (SpeciesHasFormChangeType(gBattleMons[battlerId].species, FORM_CHANGE_SWITCH_OUT) == FORM_CHANGE_TERMINATOR) // don't revert form when switched out
				DoBattleFormChange(battlerId, gBattleMonForms[partyId][GetBattlerSide(battlerId)], FALSE, FALSE);
			break;
		case FORM_CHANGE_FAINT:
		    if (SpeciesHasFormChangeType(gBattleMons[battlerId].species, FORM_CHANGE_FAINT) == FORM_CHANGE_TERMINATOR) // don't revert form when faint
			{
				targetSpecies = gBattleMonForms[partyId][GetBattlerSide(battlerId)];
				BtlController_EmitSetMonData(battlerId, BUFFER_A, REQUEST_SPECIES_BATTLE, 0, 2, &targetSpecies);
				MarkBattlerForControllerExec(battlerId);
				CalculateMonStats(mon);
				BtlController_EmitSetRawMonData(battlerId, BUFFER_A, offsetof(struct Pokemon, attack), 10, &mon->attack); // reload all stats
				MarkBattlerForControllerExec(battlerId);
			}
			break;
		case FORM_CHANGE_END_BATTLE:
		    if (battlerId == 0xFF)
			{
				targetSpecies = gBattleMonForms[partyId][B_SIDE_PLAYER];
				mon = &gPlayerParty[partyId];
			}
			else
				targetSpecies = gBattleMonForms[partyId][GetBattlerSide(battlerId)];
			
			if (targetSpecies && targetSpecies < NUM_SPECIES)
			{
				SetMonData(mon, MON_DATA_SPECIES, &targetSpecies);
				CalculateMonStats(mon);
			}
			DoOverworldFormChange(mon, FORM_CHANGE_KNOW_MOVE);
			break;
	}
}

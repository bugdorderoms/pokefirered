#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "dns.h"
#include "event_data.h"
#include "form_change.h"
#include "item.h"
#include "item_menu.h"
#include "item_use.h"
#include "overworld.h"
#include "party_menu.h"
#include "pokemon.h"
#include "region_map.h"
#include "sound.h"
#include "util.h"
#include "constants/form_change.h"
#include "constants/pokemon.h"
#include "constants/regions.h"
#include "constants/species.h"

const u16 gDeafultGeneratorFormChanges[] =
{
	// No region form, so it's possible to encounter/breed an regional form in another region
	// FORM_CHANGE_REGION,
	FORM_CHANGE_GENDER,
	FORM_CHANGE_PERSONALITY,
	FORM_CHANGE_SEASON,
	FORM_CHANGE_NATURE,
	FORM_CHANGE_TERMINATOR
};

static u16 GetSpeciesForm(u16 formChangeType, u16 species, u32 personality, u16 ability, u16 itemId, u16 *moves, u8 battlerId)
{
	u8 i, j;
	bool8 knowsMove;
	u16 param, targetSpecies = SPECIES_NONE;
	const struct FormChange *formsTable = gSpeciesInfo[species].formChangeTable;

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
					    targetSpecies = formsTable[(personality % param) + formsTable[i].param2].targetSpecies;
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
					    if (gSpecialStatuses[battlerId].removedWeatherChangeAbility)
						{
							if (!param)
								targetSpecies = formsTable[i].targetSpecies;
						}
						else if ((!param && (!gBattleWeather || !WEATHER_HAS_EFFECT)) || IsBattlerWeatherAffected(battlerId, param))
							targetSpecies = formsTable[i].targetSpecies;
						break;
					case FORM_CHANGE_SWITCH_OUT:
					case FORM_CHANGE_START_BATTLE:
					case FORM_CHANGE_WITHDRAW:
					case FORM_CHANGE_FAINT_TARGET:
					case FORM_CHANGE_COUNTDOWN:
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
					    if (param == GetDNSTimeLapseDayOrNight())
							targetSpecies = formsTable[i].targetSpecies;
						break;
					case FORM_CHANGE_KNOW_MOVE:
					    knowsMove = FALSE;
						
						for (j = 0; j < MAX_MON_MOVES; j++)
						{
							if (param == moves[j])
								knowsMove = TRUE;
						}
					
					    if (formsTable[i].param2 == knowsMove)
							targetSpecies = formsTable[i].targetSpecies;
						break;
					case FORM_CHANGE_USE_ITEM:
					    if (param == gSpecialVar_ItemId)
						{
							targetSpecies = formsTable[i].targetSpecies;
							
							if (ItemId_GetFieldFunc(param) == FieldUseFunc_FormChangeItemListMenu) // Save list id
								gSpecialVar_0x8000 = formsTable[i].param2;
							else
							{
								if (species == targetSpecies && formsTable[i].param2) // If already transformed revert to it's original form
									targetSpecies = formsTable[i].param2;
							}
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
					    if (formsTable[i].param2)
						{
							if (param != GetMoveSplit(gCurrentMove))
								targetSpecies = formsTable[i].targetSpecies;
						}
						else
						{
							if (param == GetMoveSplit(gCurrentMove))
								targetSpecies = formsTable[i].targetSpecies;
						}
						break;
					case FORM_CHANGE_REGION:
					    if (param == REGIONS_COUNT || param == gMapSectionsInfo[GetCurrentRegionMapSectionId()].region)
							targetSpecies = formsTable[i].targetSpecies;
						break;
				}
			}
		}
	}
	return targetSpecies;
}

///////////////////////////
// OVERWORLD FORM CHANGE //
///////////////////////////

u16 GetMonFormChangeSpecies(struct Pokemon *mon, u16 species, u16 formChangeType)
{
	u8 i;
	u16 moves[MAX_MON_MOVES];
	
	for (i = 0; i < MAX_MON_MOVES; i++)
		moves[i] = GetMonData(mon, MON_DATA_MOVE1 + i);
	
	return GetSpeciesForm(formChangeType, species, GetMonData(mon, MON_DATA_PERSONALITY), GetMonAbility(mon), GetMonData(mon, MON_DATA_HELD_ITEM), moves, 0);
}

u16 DoOverworldFormChange(struct Pokemon *mon, u16 formChangeType)
{
	u16 species = GetMonData(mon, MON_DATA_SPECIES);
	u16 targetSpecies = GetMonFormChangeSpecies(mon, species, formChangeType);
	
	if (targetSpecies && targetSpecies < NUM_SPECIES && targetSpecies != species)
	{
		species = targetSpecies;
		SetMonData(mon, MON_DATA_SPECIES, &species);
		CalculateMonStats(mon);
		
		if (formChangeType == FORM_CHANGE_HOLD_ITEM) // update mon icon sprite if it's in pc or in party
		{
			if (gMain.inPc)
				UpdatePcMonIconSpecies();
			else if (gMain.inParty)
			{
				UpdateCurrentPartyMonIconSpecies(species);
				PlayCry1(species, 0);
			}
		}
	}
	return species;
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

void TrySetMonFormChangeCountdown(struct Pokemon *mon)
{
	u8 i;
	u16 species = GetMonData(mon, MON_DATA_SPECIES);
	const struct FormChange *formsTable = gSpeciesInfo[species].formChangeTable;
	
	if (formsTable != NULL)
	{
		for (i = 0; formsTable[i].method != FORM_CHANGE_TERMINATOR; i++)
		{
			if (formsTable[i].method == FORM_CHANGE_COUNTDOWN && formsTable[i].targetSpecies != species)
			{
				SetMonData(mon, MON_DATA_FORM_COUNTDOWN, &formsTable[i].param);
				break;
			}
		}
	}
}

////////////////////////
// BATTLE FORM CHANGE //
////////////////////////

u16 GetBattlerFormChangeSpecies(u8 battlerId, u16 species, u16 itemId, u16 formChangeType)
{
	return GetSpeciesForm(formChangeType, species, gBattleMons[battlerId].personality, GetBattlerAbility(battlerId), itemId, &gBattleMons[battlerId].moves[0], battlerId);
}

u16 TryDoBattleFormChange(u8 battlerId, u16 formChangeType)
{
	u16 item, species, personalitySpecies, targetSpecies = SPECIES_NONE;
	
	if (!(gBattleMons[battlerId].status2 & STATUS2_TRANSFORMED)) // no change form if transformed
	{
		item = gBattleMons[battlerId].item; // form change items are not affected by Kluts, etc.
		species = gBattleMons[battlerId].species;
		
		targetSpecies = GetBattlerFormChangeSpecies(battlerId, species, item, formChangeType);
		personalitySpecies = GetBattlerFormChangeSpecies(battlerId, targetSpecies, item, FORM_CHANGE_PERSONALITY);
		
		if (personalitySpecies) // handle minior forms
		    targetSpecies = personalitySpecies;
			
		if (targetSpecies == species)
			targetSpecies = SPECIES_NONE;
	}
	return targetSpecies;
}

void DoBattleFormChange(u8 battlerId, u16 newSpecies, bool8 reloadTypes, bool8 reloadStats, bool8 reloadAbility)
{
	struct Pokemon *mon = GetBattlerPartyIndexPtr(battlerId);
	
	gBattleMons[battlerId].species = newSpecies;
	SetMonData(mon, MON_DATA_SPECIES, &newSpecies);
	
	if (reloadStats)
	{
		CalculateMonStats(mon);
		gBattleMons[battlerId].hp = GetMonData(mon, MON_DATA_HP);
		gBattleMons[battlerId].maxHP = GetMonData(mon, MON_DATA_MAX_HP);
		gBattleMons[battlerId].attack = GetMonData(mon, MON_DATA_ATK);
		gBattleMons[battlerId].defense = GetMonData(mon, MON_DATA_DEF);
		gBattleMons[battlerId].speed = GetMonData(mon, MON_DATA_SPEED);
		gBattleMons[battlerId].spAttack = GetMonData(mon, MON_DATA_SPATK);
		gBattleMons[battlerId].spDefense = GetMonData(mon, MON_DATA_SPDEF);
		HANDLE_POWER_TRICK_SWAP(battlerId)
	}
	
	if (reloadAbility)
		gBattleMons[battlerId].ability = GetMonAbility(mon);
	
	if (reloadTypes)
		SetBattlerInitialTypes(battlerId);
	
	ClearIllusionMon(battlerId);
}

bool8 SpeciesHasFormChangeType(u16 species, u16 formChangeType)
{
	u8 i;
	const struct FormChange *formsTable = gSpeciesInfo[species].formChangeTable;
	
	if (formsTable != NULL)
	{
		for (i = 0; formsTable[i].method != FORM_CHANGE_TERMINATOR; i++)
		{
			if (formsTable[i].method == formChangeType)
				return TRUE;
		}
	}
	return FALSE;
}

void DoSpecialFormChange(u8 battlerId, u8 partyId, u16 formChangeType)
{
	u16 targetSpecies;
	struct Pokemon *mon = battlerId == 0xFF ? &gPlayerParty[partyId] : GetBattlerPartyIndexPtr(battlerId);
	
	switch (formChangeType)
	{
		case FORM_CHANGE_SWITCH_OUT:
		    if (!SpeciesHasFormChangeType(gBattleMons[battlerId].species, FORM_CHANGE_SWITCH_OUT)) // don't revert form when switched out
				DoBattleFormChange(battlerId, gBattleMonForms[GetBattlerSide(battlerId)][partyId], FALSE, TRUE, FALSE);
			break;
		case FORM_CHANGE_FAINT:
		    if (!SpeciesHasFormChangeType(gBattleMons[battlerId].species, FORM_CHANGE_FAINT)) // don't revert form when faint
			{
				BtlController_EmitSetMonData(battlerId, BUFFER_A, REQUEST_SPECIES_BATTLE, 0, 2, &gBattleMonForms[GetBattlerSide(battlerId)][partyId]);
				MarkBattlerForControllerExec(battlerId);
				CalculateMonStats(mon);
				BtlController_EmitSetRawMonData(battlerId, BUFFER_A, offsetof(struct Pokemon, attack), 10, &mon->attack); // reload all stats
				MarkBattlerForControllerExec(battlerId);
			}
			break;
		case FORM_CHANGE_END_BATTLE:
		    targetSpecies = gBattleMonForms[battlerId == 0xFF ? B_SIDE_PLAYER : GetBattlerSide(battlerId)][partyId];
			
			if (targetSpecies && targetSpecies < NUM_SPECIES)
			{
				SetMonData(mon, MON_DATA_SPECIES, &targetSpecies);
				CalculateMonStats(mon);
			}
			DoOverworldFormChange(mon, FORM_CHANGE_KNOW_MOVE);
			break;
	}
}

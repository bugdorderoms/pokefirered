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

/////////////////
// FORM CHANGE //
/////////////////

static bool32 CheckSpeciesKnowsMove(u32 battlerId, u16 *moves, u32 wantedMove, bool32 checkIsPermanent)
{
    u32 i;
    
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (checkIsPermanent && !MOVE_IS_PERMANENT(battlerId, i)) // Can't Mega Evolve if move isn't permanent, e.g due to Mimic
            continue;
        
        if (wantedMove == moves[i])
            return TRUE;
    }
    return FALSE;
}

u32 GetSpeciesForm(u32 formChangeType, u32 species, u32 personality, u32 ability, u32 itemId, u16 *moves, u32 battlerId)
{
    u32 i, param, targetSpecies = SPECIES_NONE;
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
                    case FORM_CHANGE_MEGA_EVO:
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
                        else if ((!param && !IsBattlerWeatherAffected(battlerId, B_WEATHER_ANY)) || IsBattlerWeatherAffected(battlerId, param))
                            targetSpecies = formsTable[i].targetSpecies;
                        break;
                    case FORM_CHANGE_SWITCH_OUT:
                    case FORM_CHANGE_START_BATTLE:
                    case FORM_CHANGE_FAINT_TARGET:
                    case FORM_CHANGE_COUNTDOWN:
                    case FORM_CHANGE_SWITCH_IN:
                    case FORM_CHANGE_GIGANTAMAX:
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
                        if (formsTable[i].param2 == CheckSpeciesKnowsMove(battlerId, moves, param, FALSE))
                            targetSpecies = formsTable[i].targetSpecies;
                        break;
                    case FORM_CHANGE_MOVE_MEGA_EVO:
                        if (CheckSpeciesKnowsMove(battlerId, moves, param, TRUE))
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
                            if (param != GetBattleMoveSplit(gCurrentMove))
                                targetSpecies = formsTable[i].targetSpecies;
                        }
                        else
                        {
                            if (param == GetBattleMoveSplit(gCurrentMove))
                                targetSpecies = formsTable[i].targetSpecies;
                        }
                        break;
                    case FORM_CHANGE_REGION:
                        if (param == REGIONS_COUNT || param == gMapSectionsInfo[GetCurrentRegionMapSectionId()].region)
                            targetSpecies = formsTable[i].targetSpecies;
                        break;
                    case FORM_CHANGE_TERASTAL:
                        if (param == GetBattlerTeraType(battlerId))
                            targetSpecies = formsTable[i].targetSpecies;
                        break;
                }
            }
        }
    }
    return targetSpecies;
}

bool32 SpeciesHasFormChangeType(u32 species, u32 formChangeType)
{
    u32 i;
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

///////////////////////////
// OVERWORLD FORM CHANGE //
///////////////////////////

u32 GetMonFormChangeSpecies(struct Pokemon *mon, u32 species, u32 formChangeType)
{
    u32 i;
    u16 moves[MAX_MON_MOVES];
    
    for (i = 0; i < MAX_MON_MOVES; i++)
        moves[i] = GetMonData(mon, MON_DATA_MOVE1 + i);
    
    return GetSpeciesForm(formChangeType, species, GetMonData(mon, MON_DATA_PERSONALITY), GetMonAbility(mon), GetMonData(mon, MON_DATA_HELD_ITEM), moves, 0);
}

u32 DoOverworldFormChange(struct Pokemon *mon, u32 formChangeType)
{
    u32 species = GetMonData(mon, MON_DATA_SPECIES2);
    u32 targetSpecies = GetMonFormChangeSpecies(mon, species, formChangeType);
    
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
                PlayCry_Normal(species, 0);
            }
        }
    }
    return species;
}

void TrySetMonFormChangeCountdown(struct Pokemon *mon)
{
    u32 i, species = GetMonData(mon, MON_DATA_SPECIES2);
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

u32 GetBattlerFormChangeSpecies(u32 battlerId, u32 species, u32 itemId, u32 formChangeType)
{
    return GetSpeciesForm(formChangeType, species, gBattleMons[battlerId].personality, GetBattlerAbility(battlerId), itemId, gBattleMons[battlerId].moves, battlerId);
}

u32 TryDoBattleFormChange(u32 battlerId, u32 formChangeType)
{
    u32 itemId, species, personalitySpecies, targetSpecies = SPECIES_NONE;
    
    if (!(gBattleMons[battlerId].status2 & STATUS2_TRANSFORMED)) // no change form if transformed
    {
        itemId = gBattleMons[battlerId].item; // form change items are not affected by Kluts, etc.
        species = gBattleMons[battlerId].species;
        
        targetSpecies = GetBattlerFormChangeSpecies(battlerId, species, itemId, formChangeType);
        personalitySpecies = GetBattlerFormChangeSpecies(battlerId, targetSpecies, itemId, FORM_CHANGE_PERSONALITY);
        
        if (personalitySpecies) // handle minior forms
            targetSpecies = personalitySpecies;
        
        if (targetSpecies == species)
            targetSpecies = SPECIES_NONE;
    }
    return targetSpecies;
}

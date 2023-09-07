#include "global.h"
#include "gflib.h"
#include "task.h"
#include "scanline_effect.h"
#include "text_window.h"
#include "menu.h"
#include "new_menu_helpers.h"
#include "mystery_gift_menu.h"
#include "title_screen.h"
#include "data.h"
#include "item.h"
#include "list_menu.h"
#include "script_pokemon_util.h"
#include "link_rfu.h"
#include "mevent.h"
#include "save.h"
#include "constants/moves.h"
#include "link.h"
#include "event_data.h"
#include "strings.h"
#include "constants/songs.h"
#include "constants/union_room.h"
#include "constants/region_map_sections.h"

enum
{
	MG_TYPE_GIVE_POKEMON,
	MG_TYPE_GIVE_ITEM,
	MG_TYPE_UNLOCK_FEATURE,
	// You can add new options of Mystery Gift here
	MG_TYPES_COUNT, // Used to indicates the end of the table
};

// Features that can be unlocked by using the MG_TYPE_UNLOCK_FEATURE
enum
{
	MG_FEATURE_UNLOCK_POKEDEX,
	// You can add you new features here to unlock
};

struct MGPokemon
{
	const u8 *nickname;
	u16 species;
	u16 heldItem; // Irrelevant if it's a egg
	u16 moves[MAX_MON_MOVES];
	u8 ivs[NUM_STATS];
	u8 level; // Irrelevant if it's a egg
	u8 ballId:6; // Up to 63 poke balls, irrelevant if it's a egg
	u8 abilityNum:1;
	u8 abilityHidden:1;
	u8 shinyType:2;
	u8 nature:5; // 0 = Random, Up to 31 natures
	u8 isEgg:1;
};

struct MGItem
{
	u16 itemId;
	u16 quantity;
};

struct MGFeatureUnlock
{
	u16 featureId;
};

union MysteryGiftPresent
{
	const struct MGPokemon *GivePokemon;
	const struct MGItem *GiveItem;
	const struct MGFeatureUnlock *FeatureUnlock;
};

struct MysteryGift
{
	/*0x00*/ u8 code[MYSTERY_GIFT_CODE_LENGTH + 1];
	/*0x0B*/ u8 type;
	/*0x0C*/ const union MysteryGiftPresent present; // The type field above determine which struct to use
	/*0x10*/ u16 presentsCount;
	/*0x12*/ u16 flag; // If it's 0 the code always can be used
};

static u8 MysteryGift_GivePokemon(struct MysteryGift mysteryGift);
static u8 MysteryGift_GiveItem(struct MysteryGift mysteryGift);
static u8 MysteryGift_UnlockFeature(struct MysteryGift mysteryGift);

static u8 (*const sMysteryGiftGivePresentFuncs[MG_TYPES_COUNT])(struct MysteryGift) =
{
	[MG_TYPE_GIVE_POKEMON]   = MysteryGift_GivePokemon,
	[MG_TYPE_GIVE_ITEM]      = MysteryGift_GiveItem,
	[MG_TYPE_UNLOCK_FEATURE] = MysteryGift_UnlockFeature,
};

#include "data/mystery_gifts.h"

u8 GetMysteryGiftCodeState(const u8 *code)
{
	u32 i;
	
	for (i = 0; sMysteryGifts[i].type != MG_TYPES_COUNT; i++)
	{
		if (!StringCompare(code, sMysteryGifts[i].code))
		{
			if (sMysteryGifts[i].flag == 0 || !FlagGet(sMysteryGifts[i].flag)) // Check code is infinite or was't already obtained
				return sMysteryGiftGivePresentFuncs[sMysteryGifts[i].type](sMysteryGifts[i]);
			else
				return MYSTERY_GIFT_CODE_ALREADY_OBTAINED;
		}
	}
	// Code does't exists or is different than expected
	return MYSTERY_GIFT_CODE_INVALID;
}

static inline void TrySetMysteryGiftFlag(u16 flag)
{
	if (flag != 0) // Set flag if code is't infinite
		FlagSet(flag);
}

static const u8 sText_NeedToHavePokemon[] = _("You'll need to have at least\none Pokémon in your party\pto receive this gift\nusing the code {STR_VAR_1}!");
static const u8 sText_NeedSpaceInParty[] = _("You'll need space in your party\nto receive this gift\pusing the code {STR_VAR_1}!");

static u8 MysteryGift_GivePokemon(struct MysteryGift mysteryGift)
{
	u8 i, j, nature, *ivs;
	struct Pokemon *mon;
	const struct MGPokemon *mgPokemon = mysteryGift.present.GivePokemon;
	
	if (!FlagGet(FLAG_SYS_POKEMON_GET))
	{
		StringExpandPlaceholders(gStringVar4, sText_NeedToHavePokemon);
		return MYSTERY_GIFT_CODE_FAILED;
	}
	else if (CalculatePlayerPartyCount() + mysteryGift.presentsCount > PARTY_SIZE) // Only give the gift if has enough space in party
	{
		StringExpandPlaceholders(gStringVar4, sText_NeedSpaceInParty);
		return MYSTERY_GIFT_CODE_FAILED;
	}
	else
	{
		TrySetMysteryGiftFlag(mysteryGift.flag);
		PlayFanfare(MUS_LEVEL_UP);
		
		for (i = 0; i < mysteryGift.presentsCount; i++)
		{
			ivs = (u8*)mgPokemon[i].ivs;
			nature = mgPokemon[i].nature != 0 ? mgPokemon[i].nature - 1 : NUM_NATURES;
			
			if (mgPokemon[i].isEgg)
				ScriptGiveEgg(mgPokemon[i].species, ivs, mgPokemon[i].shinyType, FALSE, nature);
			else
				ScriptGiveMon(mgPokemon[i].species, mgPokemon[i].level, mgPokemon[i].heldItem, ivs, mgPokemon[i].ballId, mgPokemon[i].shinyType, FALSE, nature, MON_GENDERLESS);
			
			mon = &gPlayerParty[gPlayerPartyCount - 1];
			
			for (j = 0; j < MAX_MON_MOVES; j++)
			{
				if (mgPokemon[i].moves[j])
					SetMonMoveSlot(mon, mgPokemon[i].moves[j], j);
			}
			
			if (mgPokemon[i].nickname != NULL)
				SetMonData(mon, MON_DATA_NICKNAME, mgPokemon[i].nickname);
			
			j = mgPokemon[i].abilityNum;
			SetMonData(mon, MON_DATA_ABILITY_NUM, &j);
			
			j = mgPokemon->abilityHidden;
			SetMonData(mon, MON_DATA_ABILITY_HIDDEN, &j);
			
			j = METLOC_SPECIAL_ENCOUNTER;
			SetMonData(mon, MON_DATA_MET_LOCATION, &j);
			
			CalculateMonStats(mon);
		}
		return MYSTERY_GIFT_CODE_SUCCESS;
	}
}

static const u8 sText_NeedSpaceInBag[] = _("You'll need some space in your\nbag to receive this gift\pusing the code {STR_VAR_1}!");

static u8 MysteryGift_GiveItem(struct MysteryGift mysteryGift)
{
	u16 i;
	const struct MGItem *mgItem = mysteryGift.present.GiveItem;
	
	for (i = 0; i < mysteryGift.presentsCount; i++)
	{
		if (!CheckBagHasSpace(mgItem[i].itemId, mgItem[i].quantity)) // Only give gift if has enough space in bag
		{
			StringExpandPlaceholders(gStringVar4, sText_NeedSpaceInBag);
			return MYSTERY_GIFT_CODE_FAILED;
		}
	}
	TrySetMysteryGiftFlag(mysteryGift.flag);
	PlayFanfare(MUS_LEVEL_UP);
	
	for (i = 0; i < mysteryGift.presentsCount; i++)
		AddBagItem(mgItem[i].itemId, mgItem[i].quantity);
	
	return MYSTERY_GIFT_CODE_SUCCESS;
}

static u8 MysteryGift_UnlockFeature(struct MysteryGift mysteryGift)
{
	const struct MGFeatureUnlock *mgFeature = mysteryGift.present.FeatureUnlock;
	
	switch (mgFeature->featureId)
	{
		/* For example
		case MG_FEATURE_UNLOCK_POKEDEX:
			FlagSet(FLAG_SYS_POKEDEX_GET);
			break;*/
		// Add your new features unlock logic here
	}
	TrySetMysteryGiftFlag(mysteryGift.flag);
	PlayFanfare(MUS_LEVEL_UP);
	
	return MYSTERY_GIFT_CODE_SUCCESS;
}

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
#include "mevent_server.h"
#include "menews_jisan.h"
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

EWRAM_DATA u8 sDownArrowCounterAndYCoordIdx[8] = {};
EWRAM_DATA bool8 gGiftIsFromEReader = FALSE;

void task_add_00_ereader(void);
static u8 MysteryGift_GivePokemon(struct MysteryGift mysteryGift);
static u8 MysteryGift_GiveItem(struct MysteryGift mysteryGift);
static u8 MysteryGift_UnlockFeature(struct MysteryGift mysteryGift);

const u16 gUnkTextboxBorderPal[] = INCBIN_U16("graphics/interface/unk_textbox_border.gbapal");
const u32 gUnkTextboxBorderGfx[] = INCBIN_U32("graphics/interface/unk_textbox_border.4bpp.lz");

static u8 (*const sMysteryGiftGivePresentFuncs[MG_TYPES_COUNT])(struct MysteryGift) =
{
	[MG_TYPE_GIVE_POKEMON]   = MysteryGift_GivePokemon,
	[MG_TYPE_GIVE_ITEM]      = MysteryGift_GiveItem,
	[MG_TYPE_UNLOCK_FEATURE] = MysteryGift_UnlockFeature,
};

#include "data/mystery_gifts.h"

const struct BgTemplate sBGTemplates[] = {
    {
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 15,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0x000
    }, {
        .bg = 1,
        .charBaseIndex = 0,
        .mapBaseIndex = 14,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0x000
    }, {
        .bg = 2,
        .charBaseIndex = 0,
        .mapBaseIndex = 13,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0x000
    }, {
        .bg = 3,
        .charBaseIndex = 0,
        .mapBaseIndex = 12,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0x000
    }
};

const struct WindowTemplate sMainWindows[] = {
    {
        .bg = 0x00,
        .tilemapLeft = 0x00,
        .tilemapTop = 0x00,
        .width = 0x1e,
        .height = 0x02,
        .paletteNum = 0x0f,
        .baseBlock = 0x0013
    }, {
        .bg = 0x00,
        .tilemapLeft = 0x01,
        .tilemapTop = 0x0f,
        .width = 0x1c,
        .height = 0x04,
        .paletteNum = 0x0f,
        .baseBlock = 0x004f
    }, {
        .bg = 0x00,
        .tilemapLeft = 0x00,
        .tilemapTop = 0x0f,
        .width = 0x1e,
        .height = 0x05,
        .paletteNum = 0x0d,
        .baseBlock = 0x004f
    }, {
        0xFF
    }
};

ALIGNED(4) const u8 sMG_Ereader_TextColor_1[3] = { 0, 1, 2 };
ALIGNED(4) const u8 sMG_Ereader_TextColor_2[3] = { 1, 2, 3 };

void vblankcb_mystery_gift_e_reader_run(void)
{
    ProcessSpriteCopyRequests();
    LoadOam();
    TransferPlttBuffer();
}

void c2_mystery_gift_e_reader_run(void)
{
    RunTasks();
    RunTextPrinters();
    AnimateSprites();
    BuildOamBuffer();
}

bool32 HandleMysteryGiftOrEReaderSetup(s32 mg_or_ereader)
{
    switch (gMain.state)
    {
    case 0:
        SetVBlankCallback(NULL);
        ResetPaletteFade();
        ResetSpriteData();
        FreeAllSpritePalettes();
        ResetTasks();
        ScanlineEffect_Stop();
        ResetBgsAndClearDma3BusyFlags(1);

        InitBgsFromTemplates(0, sBGTemplates, NELEMS(sBGTemplates));
        ResetAllBgsPos();

        SetBgTilemapBuffer(3, Alloc(0x800));
        SetBgTilemapBuffer(2, Alloc(0x800));
        SetBgTilemapBuffer(1, Alloc(0x800));
        SetBgTilemapBuffer(0, Alloc(0x800));

        LoadUserWindowBorderGfx(0, 10, 0xE0);
        DrawWindowBorderWithStdpal3(0,  1, 0xF0);
        DecompressAndLoadBgGfxUsingHeap(3, gUnkTextboxBorderGfx, 0x100, 0, 0);
        InitWindows(sMainWindows);
        DeactivateAllTextPrinters();
        ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        gMain.state++;
        break;
    case 1:
        LoadPalette(gUnkTextboxBorderPal, 0, 0x20);
        LoadPalette(stdpal_get(2), 0xd0, 0x20);
        FillBgTilemapBufferRect(0, 0x000, 0, 0, 32, 32, 0x11);
        FillBgTilemapBufferRect(1, 0x000, 0, 0, 32, 32, 0x11);
        FillBgTilemapBufferRect(2, 0x000, 0, 0, 32, 32, 0x11);
        MG_DrawCheckerboardPattern();
        PrintMysteryGiftOrEReaderTopMenu(mg_or_ereader, 0);
        gMain.state++;
        break;
    case 2:
        CopyBgTilemapBufferToVram(3);
        CopyBgTilemapBufferToVram(2);
        CopyBgTilemapBufferToVram(1);
        CopyBgTilemapBufferToVram(0);
        gMain.state++;
        break;
    case 3:
        ShowBg(0);
        ShowBg(3);
        PlayBGM(MUS_MYSTERY_GIFT);
        SetVBlankCallback(vblankcb_mystery_gift_e_reader_run);
        EnableInterrupts(INTR_FLAG_VBLANK | INTR_FLAG_VCOUNT | INTR_FLAG_TIMER3 | INTR_FLAG_SERIAL);
        return TRUE;
    }

    return FALSE;
}

void c2_ereader(void)
{
    if (HandleMysteryGiftOrEReaderSetup(1))
    {
        SetMainCallback2(c2_mystery_gift_e_reader_run);
        gGiftIsFromEReader = TRUE;
        task_add_00_ereader();
    }
}

void MainCB_FreeAllBuffersAndReturnToInitTitleScreen(void)
{
    gGiftIsFromEReader = FALSE;
    FreeAllWindowBuffers();
    Free(GetBgTilemapBuffer(0));
    Free(GetBgTilemapBuffer(1));
    Free(GetBgTilemapBuffer(2));
    Free(GetBgTilemapBuffer(3));
    SetMainCallback2(CB2_InitTitleScreen);
}

void PrintMysteryGiftOrEReaderTopMenu(bool8 mg_or_ereader, bool32 usePickOkCancel)
{
    const u8 * src;
    s32 width;
    FillWindowPixelBuffer(0, 0x00);
    if (!mg_or_ereader)
    {
        src = usePickOkCancel == TRUE ? gText_PickOKExit : gText_PickOKCancel;
        AddTextPrinterParameterized4(0, 2, 2, 2, 0, 0, sMG_Ereader_TextColor_1, 0, gText_MysteryGift2);
        width = 222 - GetStringWidth(0, src, 0);
        AddTextPrinterParameterized4(0, 0, width, 2, 0, 0, sMG_Ereader_TextColor_1, 0, src);
    }
    else
    {
        AddTextPrinterParameterized4(0, 2, 2, 2, 0, 0, sMG_Ereader_TextColor_1, 0, gJPText_MysteryGift);
        AddTextPrinterParameterized4(0, 0, 0x78, 2, 0, 0, sMG_Ereader_TextColor_1, 0, gJPText_DecideStop);
    }
    CopyWindowToVram(0, COPYWIN_GFX);
    PutWindowTilemap(0);
}

void MG_DrawCheckerboardPattern(void)
{
    s32 i = 0, j;

    FillBgTilemapBufferRect(3, 0x003, 0, 0, 32, 2, 0x11);

    for (i = 0; i < 18; i++)
    {
        for (j = 0; j < 32; j++)
        {
            if ((i & 1) != (j & 1))
            {
                FillBgTilemapBufferRect(3, 1, j, i + 2, 1, 1, 0x11);
            }
            else
            {
                FillBgTilemapBufferRect(3, 2, j, i + 2, 1, 1, 0x11);
            }
        }
    }
}

void AddTextPrinterToWindow1(const u8 *str)
{
    StringExpandPlaceholders(gStringVar4, str);
    FillWindowPixelBuffer(1, 0x11);
    AddTextPrinterParameterized4(1, 2, 0, 2, 0, 2, sMG_Ereader_TextColor_2, 0, gStringVar4);
    DrawTextBorderOuter(1, 0x001, 0xF);
    PutWindowTilemap(1);
    CopyWindowToVram(1, COPYWIN_BOTH);
}

void ClearTextWindow(void)
{
    rbox_fill_rectangle(1);
    ClearWindowTilemap(1);
    CopyWindowToVram(1, COPYWIN_MAP);
}

bool32 MG_PrintTextOnWindow1AndWaitButton(u8 *textState, const u8 *str)
{
    switch (*textState)
    {
    case 0:
        AddTextPrinterToWindow1(str);
        goto inc;
    case 1:
        DrawDownArrow(1, 0xD0, 0x14, 1, FALSE, &sDownArrowCounterAndYCoordIdx[0], &sDownArrowCounterAndYCoordIdx[1]);
        if (JOY_NEW(A_BUTTON | B_BUTTON))
        {
            inc:
            (*textState)++;
        }
        break;
    case 2:
        DrawDownArrow(1, 0xD0, 0x14, 1, TRUE, &sDownArrowCounterAndYCoordIdx[0], &sDownArrowCounterAndYCoordIdx[1]);
        *textState = 0;
        ClearTextWindow();
        return TRUE;
    case 0xFF:
        *textState = 2;
        break;
    }
    return FALSE;
}

void HideDownArrow(void)
{
    DrawDownArrow(1, 0xD0, 0x14, 1, FALSE, &sDownArrowCounterAndYCoordIdx[0], &sDownArrowCounterAndYCoordIdx[1]);
}

void ShowDownArrow(void)
{
    DrawDownArrow(1, 0xD0, 0x14, 1, TRUE, &sDownArrowCounterAndYCoordIdx[0], &sDownArrowCounterAndYCoordIdx[1]);
}

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
	u32 i, j;
	u8 nature, *ivs;
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

static const u8 sText_NeedSpaceInBag[] = _("You'll need some space in your\nbag to receive this gift\pusing the code {STR_VAR_1}.");

static u8 MysteryGift_GiveItem(struct MysteryGift mysteryGift)
{
	u32 i;
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

#include "global.h"
#include "gflib.h"
#include "data.h"
#include "battle.h"
#include "data.h"
#include "constants/items.h"
#include "mail_data.h"
#include "form_change.h"
#include "pokemon_storage_system.h"
#include "event_data.h"
#include "random.h"
#include "constants/moves.h"
#include "menu.h"
#include "new_menu_helpers.h"
#include "script.h"
#include "item.h"
#include "strings.h"
#include "party_menu.h"
#include "list_menu.h"
#include "overworld.h"
#include "pokedex.h"
#include "evolution.h"
#include "decompress.h"
#include "constants/songs.h"
#include "text_window.h"
#include "trig.h"
#include "m4a.h"
#include "util.h"
#include "graphics.h"
#include "scanline_effect.h"
#include "naming_screen.h"
#include "field_fadetransition.h"
#include "trade.h"
#include "constants/form_change.h"
#include "constants/daycare.h"
#include "constants/region_map_sections.h"
#include "constants/global.h"
#include "constants/pokedex.h"
#include "constants/hold_effects.h"

// Combination of RSE's Day-Care (re-used on Four Island), FRLG's Day-Care, and egg_hatch.c
struct EggHatchData
{
    u8 eggSpriteID;
    u8 pokeSpriteID;
    u8 CB2_state;
    u8 CB2_PalCounter;
    u16 species;
    u8 eggPartyID;
    u8 eggShardVelocityID;
    u8 windowId;
    u8 textColor[3];
};

static void ShiftDaycareSlots(struct DayCare *daycare);
static s8 Daycare_FindEmptySpot(struct DayCare *daycare);
static void StorePokemonInDaycare(struct Pokemon *mon, struct DaycareMon *daycareMon);
static u16 TakeSelectedPokemonFromDaycare(struct DaycareMon *daycareMon);
static u8 GetDaycareCompatibilityScore(struct DayCare *daycare);
static void ClearDaycareMonMail(struct DayCareMail *mail);
static void BuildEggMoveset(struct Pokemon *egg, struct BoxPokemon *father, struct BoxPokemon *mother, u16 fatherItem, u16 motherItem);
static void InheritIVs(struct Pokemon *egg, struct DayCare *daycare, u16 fatherItem, u16 motherItem);
static void InheritAbilitySlot(struct Pokemon *egg, struct BoxPokemon *mon);
static void InheritPokeball(struct Pokemon *egg, struct BoxPokemon *father, struct BoxPokemon *mother);
static u8 GetParentSlotToInheritNature(struct DayCare *daycare);
static void TriggerPendingDaycareEgg(void);
static void Task_EggHatch(u8 taskId);
static void CB2_EggHatch_0(void);
static void CB2_EggHatch_1(void);
static void Task_EggHatchPlayBGM(u8 taskId);
static void SpriteCB_Egg_0(struct Sprite* sprite);
static void SpriteCB_Egg_1(struct Sprite* sprite);
static void SpriteCB_Egg_2(struct Sprite* sprite);
static void SpriteCB_Egg_3(struct Sprite* sprite);
static void SpriteCB_Egg_4(struct Sprite* sprite);
static void SpriteCB_Egg_5(struct Sprite* sprite);
static void SpriteCB_EggShard(struct Sprite* sprite);
static void EggHatchPrintMessage(u8 windowId, u8* string, u8 x, u8 y, u8 speed);
static void EggHatchSetMonNickname(void);
static void Task_HandleDaycareLevelMenuInput(u8 taskId);
static void DaycarePrintMonInfo(u8 windowId, u32 daycareSlotId, u8 y);

// RAM buffers used to assist with BuildEggMoveset
EWRAM_DATA static u16 sHatchedEggLevelUpMoves[MAX_LV_UP_MOVES] = {0};
EWRAM_DATA static u16 sHatchedEggEggMoves[EGG_MOVES_ARRAY_COUNT] = {0};
EWRAM_DATA static u16 sHatchedEggFinalMoves[MAX_MON_MOVES] = {0};

// IWRAM bss
static struct EggHatchData *sEggHatchData;

#include "data/pokemon/learnsets/egg_moves.h"

static const u8 *const sCompatibilityMessages[] =
{
    gDaycareText_GetAlongVeryWell,
    gDaycareText_GetAlong,
    gDaycareText_DontLikeOther,
    gDaycareText_PlayOther
};

static const u16 sEggPalette[] = INCBIN_U16("graphics/pokemon/egg/normal.gbapal");
static const u8 sEggHatchTiles[] = INCBIN_U8("graphics/misc/egg_hatch.4bpp");
static const u8 sEggShardTiles[] = INCBIN_U8("graphics/misc/egg_shard.4bpp");

static const struct SpriteSheet sEggHatch_Sheet =
{
    .data = sEggHatchTiles,
    .size = 2048,
    .tag = 12345,
};

static const struct SpriteSheet sEggShards_Sheet =
{
    .data = sEggShardTiles,
    .size = 128,
    .tag = 23456,
};

static const struct SpritePalette sEgg_SpritePalette =
{
    .data = sEggPalette,
    .tag = 54321
};

static const struct OamData sOamData_EggHatch =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sSpriteAnim_EggHatch0[] =
{
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_EggHatch1[] =
{
    ANIMCMD_FRAME(16, 5),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_EggHatch2[] =
{
    ANIMCMD_FRAME(32, 5),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_EggHatch3[] =
{
    ANIMCMD_FRAME(48, 5),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_EggHatch[] =
{
    sSpriteAnim_EggHatch0,
    sSpriteAnim_EggHatch1,
    sSpriteAnim_EggHatch2,
    sSpriteAnim_EggHatch3,
};

static const struct SpriteTemplate sSpriteTemplate_EggHatch =
{
    .tileTag = 12345,
    .paletteTag = 54321,
    .oam = &sOamData_EggHatch,
    .anims = sSpriteAnimTable_EggHatch,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

static const struct OamData sOamData_EggShard =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sSpriteAnim_EggShard0[] =
{
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_EggShard1[] =
{
    ANIMCMD_FRAME(1, 5),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_EggShard2[] =
{
    ANIMCMD_FRAME(2, 5),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_EggShard3[] =
{
    ANIMCMD_FRAME(3, 5),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_EggShard[] =
{
    sSpriteAnim_EggShard0,
    sSpriteAnim_EggShard1,
    sSpriteAnim_EggShard2,
    sSpriteAnim_EggShard3,
};

static const struct SpriteTemplate sSpriteTemplate_EggShard =
{
    .tileTag = 23456,
    .paletteTag = 54321,
    .oam = &sOamData_EggShard,
    .anims = sSpriteAnimTable_EggShard,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_EggShard
};

static const struct BgTemplate sBgTemplates_EggHatch[2] =
{
    {
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 24,
        .screenSize = 3,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },

    {
        .bg = 1,
        .charBaseIndex = 0,
        .mapBaseIndex = 8,
        .screenSize = 1,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0
    },
};

static const struct WindowTemplate sWinTemplates_EggHatch[2] =
{
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 0,
        .baseBlock = 64
    },
    DUMMY_WIN_TEMPLATE
};

static const struct WindowTemplate sYesNoWinTemplate =
{
    .bg = 0,
    .tilemapLeft = 21,
    .tilemapTop = 9,
    .width = 6,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = 424
};

static const struct WindowTemplate sDaycareLevelMenuWindowTemplate =
{
    .bg = 0,
    .tilemapLeft = 12,
    .tilemapTop = 1,
    .width = 17,
    .height = 5,
    .paletteNum = 15,
    .baseBlock = 8
};

static const struct ListMenuItem sLevelMenuItems[] =
{
    {gExpandedPlaceholder_Empty, 0},
    {gExpandedPlaceholder_Empty, 1},
    {gOtherText_Exit, DAYCARE_LEVEL_MENU_EXIT}
};

static const struct ListMenuTemplate sDaycareListMenuLevelTemplate =
{
    .items = sLevelMenuItems,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .itemPrintFunc = DaycarePrintMonInfo,
    .totalItems = 3,
    .maxShowed = 3,
    .windowId = 0,
    .header_X = 2,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 0,
    .cursorPal = 2,
    .fillValue = 1,
    .cursorShadowPal = 3,
    .lettersSpacing = 1,
    .itemVerticalPadding = 0,
    .scrollMultiple = 0,
    .fontId = 3,
    .cursorKind = 0
};

static const s16 sEggShardVelocities[][2] =
{
    {Q_8_8(-1.5),       Q_8_8(-3.75)},
    {Q_8_8(-5),         Q_8_8(-3)},
    {Q_8_8(3.5),        Q_8_8(-3)},
    {Q_8_8(-4),         Q_8_8(-3.75)},
    {Q_8_8(2),          Q_8_8(-1.5)},
    {Q_8_8(-0.5),       Q_8_8(-6.75)},
    {Q_8_8(5),          Q_8_8(-2.25)},
    {Q_8_8(-1.5),       Q_8_8(-3.75)},
    {Q_8_8(4.5),        Q_8_8(-1.5)},
    {Q_8_8(-1),         Q_8_8(-6.75)},
    {Q_8_8(4),          Q_8_8(-2.25)},
    {Q_8_8(-3.5),       Q_8_8(-3.75)},
    {Q_8_8(1),          Q_8_8(-1.5)},
    {Q_8_8(-3.515625),  Q_8_8(-6.75)},
    {Q_8_8(4.5),        Q_8_8(-2.25)},
    {Q_8_8(-0.5),       Q_8_8(-7.5)},
    {Q_8_8(1),          Q_8_8(-4.5)},
    {Q_8_8(-2.5),       Q_8_8(-2.25)},
    {Q_8_8(2.5),        Q_8_8(-7.5)},
};

//////////////////////////
// STORE/TAKE FUNCTIONS //
//////////////////////////

void StoreSelectedPokemonInDaycare(void)
{
	struct DayCare *daycare = &gSaveBlock1Ptr->daycare;
	StorePokemonInDaycare(&gPlayerParty[GetCursorSelectionMonId()], &daycare->mons[Daycare_FindEmptySpot(daycare)]);
}

static void StorePokemonInDaycare(struct Pokemon *mon, struct DaycareMon *daycareMon)
{
    if (MonHasMail(mon))
    {
        StringCopy(daycareMon->mail.OT_name, gSaveBlock2Ptr->playerName);
		GetMonNickname(mon, daycareMon->mail.monName);
		// StripExtCtrlCodes(daycareMon->mail.monName);
		// daycareMon->mail.gameLanguage = LANGUAGE_ENGLISH;
		// daycareMon->mail.monLanguage = GetMonData(mon, MON_DATA_LANGUAGE);
        daycareMon->mail.message = gSaveBlock1Ptr->mail[GetMonData(mon, MON_DATA_MAIL)];
        TakeMailFromMon(mon);
    }
    daycareMon->mon = mon->box;
    BoxMonRestorePP(&daycareMon->mon);
    daycareMon->steps = 0;
    ZeroMonData(mon);
    CompactPartySlots();
    CalculatePlayerPartyCount();
}

u16 TakePokemonFromDaycare(void)
{
	struct DayCare *daycare = &gSaveBlock1Ptr->daycare;
	u16 species = TakeSelectedPokemonFromDaycare(&daycare->mons[gSpecialVar_0x8004]);
	
	ShiftDaycareSlots(daycare);
	
	return species;
}

static void ApplyDaycareExperience(struct Pokemon *mon)
{
    u8 i, ret;
    bool8 firstMove;

    for (i = 0; i < MAX_LEVEL; i++)
    {
        // Add the mon's gained daycare experience level by level until it can't level up anymore.
        if (!TryIncrementMonLevel(mon))
			break;
		
        // Teach the mon new moves it learned while in the daycare.
        firstMove = TRUE;
		
		while ((ret = MonTryLearningNewMove(mon, firstMove)) != MON_DONT_FIND_MOVE_TO_LEARN)
		{
			firstMove = FALSE;
			
			if (ret == MON_HAS_MAX_MOVES)
				DeleteFirstMoveAndGiveMoveToMon(mon, gMoveToLearn);
		}
    }
    // Re-calculate the mons stats at its new level.
    CalculateMonStats(mon);
}

static u16 TakeSelectedPokemonFromDaycare(struct DaycareMon *daycareMon)
{
    u16 species;
    u32 experience;
    struct Pokemon pokemon;

    GetBoxMonNickname(&daycareMon->mon, gStringVar1);
    BoxMonToMon(&daycareMon->mon, &pokemon);
    species = DoOverworldFormChange(&pokemon, FORM_CHANGE_WITHDRAW);

    if (GetMonData(&pokemon, MON_DATA_LEVEL) != MAX_LEVEL)
    {
        experience = GetMonData(&pokemon, MON_DATA_EXP) + daycareMon->steps;
        SetMonData(&pokemon, MON_DATA_EXP, &experience);
        ApplyDaycareExperience(&pokemon);
    }
    gPlayerParty[PARTY_SIZE - 1] = pokemon;
	
    if (daycareMon->mail.message.itemId)
    {
        GiveMailToMon2(&gPlayerParty[PARTY_SIZE - 1], &daycareMon->mail.message);
        ClearDaycareMonMail(&daycareMon->mail);
    }
	
    ZeroBoxMonData(&daycareMon->mon);
    daycareMon->steps = 0;
    CompactPartySlots();
    CalculatePlayerPartyCount();
	
    return species;
}

/////////////////////////////////////////////
// LEVEL CALCULATION AND DISPLAY FUNCTIONS //
/////////////////////////////////////////////

static u8 GetLevelAfterDaycareSteps(struct BoxPokemon *mon, u32 steps)
{
    struct BoxPokemon tempMon = *mon;
    u32 experience = GetBoxMonData(mon, MON_DATA_EXP) + steps;
    SetBoxMonData(&tempMon, MON_DATA_EXP, &experience);
    return GetLevelFromBoxMonExp(&tempMon);
}

static u8 GetNumLevelsGainedFromSteps(struct DaycareMon *daycareMon)
{
    return GetLevelAfterDaycareSteps(&daycareMon->mon, daycareMon->steps) - GetLevelFromBoxMonExp(&daycareMon->mon);
}

static u8 GetNumLevelsGainedForDaycareMon(struct DaycareMon *daycareMon)
{
    u8 numLevelsGained = GetNumLevelsGainedFromSteps(daycareMon);
    ConvertIntToDecimalStringN(gStringVar2, numLevelsGained, STR_CONV_MODE_LEFT_ALIGN, 2);
    GetBoxMonNickname(&daycareMon->mon, gStringVar1);
    return numLevelsGained;
}

u8 GetNumLevelsGainedFromDaycare(void)
{
	struct DaycareMon *daycareMon = &gSaveBlock1Ptr->daycare.mons[gSpecialVar_0x8004];
	return GetBoxMonData(daycareMon, MON_DATA_SPECIES) ? GetNumLevelsGainedForDaycareMon(daycareMon) : 0;
}

#define tMenuListTaskId     data[0]
#define tWindowId           data[1]

void ShowDaycareLevelMenu(void)
{
    struct ListMenuTemplate menuTemplate = sDaycareListMenuLevelTemplate;
    u8 listMenuTaskId, daycareMenuTaskId, windowId = AddWindow(&sDaycareLevelMenuWindowTemplate);
	
    DrawStdWindowFrame(windowId, FALSE);

    menuTemplate.windowId = windowId;
    listMenuTaskId = ListMenuInit(&menuTemplate, 0, 0);

    CopyWindowToVram(windowId, COPYWIN_BOTH);

    daycareMenuTaskId = CreateTask(Task_HandleDaycareLevelMenuInput, 3);
    gTasks[daycareMenuTaskId].tMenuListTaskId = listMenuTaskId;
    gTasks[daycareMenuTaskId].tWindowId = windowId;
}

static void ExitDaycareLevelMenu(u8 taskId)
{
	DestroyListMenuTask(gTasks[taskId].tMenuListTaskId, NULL, NULL);
	ClearStdWindowAndFrame(gTasks[taskId].tWindowId, TRUE);
	RemoveWindow(gTasks[taskId].tWindowId);
	DestroyTask(taskId);
	EnableBothScriptContexts();
}

static void Task_HandleDaycareLevelMenuInput(u8 taskId)
{
    u32 input = ListMenu_ProcessInput(gTasks[taskId].tMenuListTaskId);

    if (gMain.newKeys & A_BUTTON)
    {
        switch (input)
        {
        case 0:
        case 1:
            gSpecialVar_Result = input;
            break;
        case DAYCARE_LEVEL_MENU_EXIT:
            gSpecialVar_Result = DAYCARE_EXITED_LEVEL_MENU;
            break;
        }
        ExitDaycareLevelMenu(taskId);
    }
    else if (gMain.newKeys & B_BUTTON)
    {
        gSpecialVar_Result = DAYCARE_EXITED_LEVEL_MENU;
        ExitDaycareLevelMenu(taskId);
    }
}

bool8 NameHasGenderSymbol(const u8 *name, u8 genderRatio)
{
    u8 i, symbolsCount[GENDER_COUNT];
    
	symbolsCount[MALE] = symbolsCount[FEMALE] = 0;

    for (i = 0; name[i] != EOS; i++)
    {
        if (name[i] == CHAR_MALE)
            symbolsCount[MALE]++;
        else if (name[i] == CHAR_FEMALE)
            symbolsCount[FEMALE]++;
    }

    if (genderRatio == MON_MALE && symbolsCount[MALE] != 0 && symbolsCount[FEMALE] == 0)
        return TRUE;
	
    if (genderRatio == MON_FEMALE && symbolsCount[FEMALE] != 0 && symbolsCount[MALE] == 0)
        return TRUE;

    return FALSE;
}

static u8 *AppendMonGenderSymbol(u8 *name, struct BoxPokemon *boxMon)
{
	u8 gender = GetBoxMonGender(boxMon);
	
	if (gender == MON_MALE)
    {
        if (!NameHasGenderSymbol(name, MON_MALE))
            return StringAppend(name, gText_MaleSymbol4);
    }
    else if (gender == MON_FEMALE)
    {
        if (!NameHasGenderSymbol(name, MON_FEMALE))
            return StringAppend(name, gText_FemaleSymbol4);
    }
    return StringAppend(name, gText_GenderlessSymbol);
}

static void DaycareAddTextPrinter(u8 windowId, const u8 *text, u32 x, u32 y)
{
    struct TextPrinterTemplate printer;

    printer.currentChar = text;
    printer.windowId = windowId;
    printer.fontId = 3;
    printer.x = x;
    printer.y = y;
    printer.currentX = x;
    printer.currentY = y;
    printer.unk = 0;
    gTextFlags.useAlternateDownArrow = 0;
    printer.letterSpacing = 1;
    printer.lineSpacing = 1;
    printer.fgColor = 2;
    printer.bgColor = 1;
    printer.shadowColor = 3;

    AddTextPrinter(&printer, 0xFF, NULL);
}

static void DaycarePrintMonInfo(u8 windowId, u32 daycareSlotId, u8 y)
{
	u8 level, nickname[POKEMON_NAME_LENGTH * 2], lvlText[12], intText[8];
	u32 x;
	
    if (daycareSlotId < (unsigned)DAYCARE_MON_COUNT)
    {
		struct DayCare *daycare = &gSaveBlock1Ptr->daycare;
		
		// Nickname
		GetBoxMonNickname(&daycare->mons[daycareSlotId].mon, nickname);
		AppendMonGenderSymbol(nickname, &daycare->mons[daycareSlotId].mon);
		DaycareAddTextPrinter(windowId, nickname, 8, y);

        // Level
#if REVISION == 0
        strcpy((char *)lvlText, (const char *)gText_Lv);
#else
        StringCopy(lvlText, gText_Lv);
#endif

        level = GetLevelAfterDaycareSteps(&daycare->mons[daycareSlotId].mon, daycare->mons[daycareSlotId].steps);
		ConvertIntToDecimalStringN(intText, level, STR_CONV_MODE_LEFT_ALIGN, 3);
		StringAppend(lvlText, intText);
		
		x = 132 - GetStringWidth(3, lvlText, 0);
		DaycareAddTextPrinter(windowId, lvlText, x, y);
    }
}

#undef tMenuListTaskId
#undef tWindowId

//////////////////////
// COST CALCULATION //
//////////////////////

static u32 GetDaycareCostForSelectedMon(struct DaycareMon *daycareMon)
{
    u32 cost;
	
    GetBoxMonNickname(&daycareMon->mon, gStringVar1);
	
    cost = 100 + 100 * GetNumLevelsGainedFromSteps(daycareMon);
    ConvertIntToDecimalStringN(gStringVar2, cost, STR_CONV_MODE_LEFT_ALIGN, 5);
	
    return cost;
}

void GetDaycareCost(void)
{
    gSpecialVar_0x8005 = GetDaycareCostForSelectedMon(&gSaveBlock1Ptr->daycare.mons[gSpecialVar_0x8004]);
}

///////////////////////////////
// ROUTE 5 DAYCARE FUNCTIONS //
///////////////////////////////

void PutMonInRoute5Daycare(void)
{
    StorePokemonInDaycare(&gPlayerParty[GetCursorSelectionMonId()], &gSaveBlock1Ptr->route5DayCareMon);
}

u16 TakePokemonFromRoute5Daycare(void)
{
    return TakeSelectedPokemonFromDaycare(&gSaveBlock1Ptr->route5DayCareMon);
}

u8 GetNumLevelsGainedForRoute5DaycareMon(void)
{
    return GetNumLevelsGainedForDaycareMon(&gSaveBlock1Ptr->route5DayCareMon);
}

void GetCostToWithdrawRoute5DaycareMon(void)
{
    gSpecialVar_0x8005 = GetDaycareCostForSelectedMon(&gSaveBlock1Ptr->route5DayCareMon);
}

bool8 IsThereMonInRoute5Daycare(void)
{
	return (GetBoxMonData(&gSaveBlock1Ptr->route5DayCareMon.mon, MON_DATA_SPECIES));
}

////////////////////
// UTIL FUNCTIONS //
////////////////////

// Counts the number of egg moves a pokemon learns and stores the moves in the given array.
u8 GetEggMoves(struct Pokemon *pokemon, u16 *eggMoves)
{
	/*u8 i;
	u16 species = GetMonData(pokemon, MON_DATA_SPECIES);
	const u16 *eggMoves
	
	for (i = 0; i < )
	{
		
	}
	
	
	
	
	u16 eggMoveIdx;
    u16 numEggMoves;
    u16 i;

    numEggMoves = 0;
    eggMoveIdx = 0;

    for (i = 0; i < ARRAY_COUNT(gEggMoves) - 1; i++)
    {
        if (gEggMoves[i] == species + EGG_MOVES_SPECIES_OFFSET)
        {
            eggMoveIdx = i + 1;
            break;
        }
    }

    for (i = 0; i < EGG_MOVES_ARRAY_COUNT; i++)
    {
        if (gEggMoves[eggMoveIdx + i] > EGG_MOVES_SPECIES_OFFSET)
        {
            // TODO: the curly braces around this if statement are required for a matching build.
            break;
        }

        eggMoves[i] = gEggMoves[eggMoveIdx + i];
        numEggMoves++;
    }

    return numEggMoves;*/
}

static u8 CountPokemonInDaycare(struct DayCare *daycare)
{
    u8 i, count;

    for (i = 0, count = 0; i < DAYCARE_MON_COUNT; i++)
    {
        if (GetBoxMonData(&daycare->mons[i].mon, MON_DATA_SPECIES))
            count++;
    }
    return count;
}

static s8 Daycare_FindEmptySpot(struct DayCare *daycare)
{
    u8 i;

    for (i = 0; i < DAYCARE_MON_COUNT; i++)
    {
        if (!GetBoxMonData(&daycare->mons[i].mon, MON_DATA_SPECIES))
            return i;
    }
    return -1;
}

// Shifts the second daycare pokemon slot into the first slot.
static void ShiftDaycareSlots(struct DayCare *daycare)
{
    // This condition is only satisfied when the player takes out the first pokemon from the daycare.
    if (GetBoxMonData(&daycare->mons[1].mon, MON_DATA_SPECIES) && !GetBoxMonData(&daycare->mons[0].mon, MON_DATA_SPECIES))
    {
        daycare->mons[0].mon = daycare->mons[1].mon;
        ZeroBoxMonData(&daycare->mons[1].mon);
        daycare->mons[0].mail = daycare->mons[1].mail;
        daycare->mons[0].steps = daycare->mons[1].steps;
        daycare->mons[1].steps = 0;
        ClearDaycareMonMail(&daycare->mons[1].mail);
    }
}

static void ClearDaycareMonMail(struct DayCareMail *mail)
{
    u8 i;

    for (i = 0; i < PLAYER_NAME_LENGTH + 1; i++)
        mail->OT_name[i] = 0;
	
    for (i = 0; i < POKEMON_NAME_LENGTH + 1; i++)
        mail->monName[i] = 0;

    ClearMailStruct(&mail->message);
}

u8 GetDaycareState(void)
{
	u8 numMons;
	
    if (gSaveBlock1Ptr->daycare.pending)
        return DAYCARE_EGG_WAITING;

    numMons = CountPokemonInDaycare(&gSaveBlock1Ptr->daycare);
    if (numMons != 0)
        return numMons + 1; // DAYCARE_ONE_MON or DAYCARE_TWO_MONS

    return DAYCARE_NO_MONS;
}

void ChooseSendDaycareMon(void)
{
    ChooseMonForDaycare();
    gMain.savedCallback = CB2_ReturnToField;
}

static void RemoveEggFromDayCare(struct DayCare *daycare)
{
    daycare->pending = FALSE;
    daycare->stepCounter = 0;
}

void RejectEggFromDayCare(void)
{
    RemoveEggFromDayCare(&gSaveBlock1Ptr->daycare);
}

static bool8 TryProduceOrHatchEgg(struct DayCare *daycare)
{
    u8 i, validEggs = 0;

    for (i = 0; i < DAYCARE_MON_COUNT; i++)
    {
        if (GetBoxMonData(&daycare->mons[i].mon, MON_DATA_SANITY_HAS_SPECIES))
		{
            daycare->mons[i].steps++,
			validEggs++;
		}
    }

    // Check if an egg should be produced
    if (!daycare->pending && validEggs == DAYCARE_MON_COUNT && (daycare->mons[1].steps & 0xFF) == 0xFF)
    {
        if (GetDaycareCompatibilityScore(daycare) > (Random() * 100u) / USHRT_MAX)
            TriggerPendingDaycareEgg();
    }

    // Hatch Egg
    if (++daycare->stepCounter == 255)
    {
        u8 steps;

        for (i = 0; i < gPlayerPartyCount; i++)
        {
            if (!GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG))
                continue;

            steps = GetMonData(&gPlayerParty[i], MON_DATA_FRIENDSHIP);
            if (steps != 0)
            {
                steps -= 1;
                SetMonData(&gPlayerParty[i], MON_DATA_FRIENDSHIP, &steps);
            }
            else // hatch the egg
            {
                gSpecialVar_0x8004 = i;
                return TRUE;
            }
        }
    }
    return FALSE; // no hatching
}

bool8 ShouldEggHatch(void)
{
    if (IsThereMonInRoute5Daycare())
        gSaveBlock1Ptr->route5DayCareMon.steps++;
	
    return TryProduceOrHatchEgg(&gSaveBlock1Ptr->daycare);
}

// gStringVar1 = first mon's nickname
// gStringVar2 = second mon's nickname
// gStringVar3 = first mon trainer's name
void GetDaycareMonNicknames(void)
{
    u8 text[12];
	struct DayCare *daycare = &gSaveBlock1Ptr->daycare;
	
    if (GetBoxMonData(&daycare->mons[0].mon, MON_DATA_SPECIES))
    {
        GetBoxMonNickname(&daycare->mons[0].mon, gStringVar1);
        GetBoxMonData(&daycare->mons[0].mon, MON_DATA_OT_NAME, text);
        StringCopy(gStringVar3, text);
    }

    if (GetBoxMonData(&daycare->mons[1].mon, MON_DATA_SPECIES))
        GetBoxMonNickname(&daycare->mons[1].mon, gStringVar2);
}

u16 GetSelectedMonNicknameAndSpecies(void)
{
    GetBoxMonNickname(&gPlayerParty[GetCursorSelectionMonId()].box, gStringVar1);
    return GetBoxMonData(&gPlayerParty[GetCursorSelectionMonId()].box, MON_DATA_SPECIES);
}

/////////////////////////////
// COMPATIBILITY FUNCTIONS //
/////////////////////////////

// Determine if the two given egg group lists contain any of the
// same egg groups.
static bool8 EggGroupsOverlap(u8 *eggGroups1, u8 *eggGroups2)
{
    u8 i, j;

    for (i = 0; i < EGG_GROUPS_PER_MON; i++)
    {
        for (j = 0; j < EGG_GROUPS_PER_MON; j++)
        {
            if (eggGroups1[i] == eggGroups2[j])
                return TRUE;
        }
    }
    return FALSE;
}

static u8 GetDaycareCompatibilityScore(struct DayCare *daycare)
{
	u16 species[DAYCARE_MON_COUNT];
    u8 i, eggGroups[DAYCARE_MON_COUNT][EGG_GROUPS_PER_MON], genders[DAYCARE_MON_COUNT];
	u32 trainerIds[DAYCARE_MON_COUNT];

    for (i = 0; i < DAYCARE_MON_COUNT; i++)
    {
		species[i] = GetBoxMonData(&daycare->mons[i].mon, MON_DATA_SPECIES);
		genders[i] = GetGenderFromSpeciesAndPersonality(species[i], GetBoxMonData(&daycare->mons[i].mon, MON_DATA_PERSONALITY));
		eggGroups[i][0] = gSpeciesInfo[species[i]].eggGroup1;
        eggGroups[i][1] = gSpeciesInfo[species[i]].eggGroup2;
		trainerIds[i] = GetBoxMonData(&daycare->mons[i].mon, MON_DATA_OT_ID);
    }

    // check unbreedable egg group
    if (eggGroups[0][0] == EGG_GROUP_UNDISCOVERED || eggGroups[1][0] == EGG_GROUP_UNDISCOVERED)
        return PARENTS_INCOMPATIBLE;
	
    // two Ditto can't breed
    if (eggGroups[0][0] == EGG_GROUP_DITTO && eggGroups[1][0] == EGG_GROUP_DITTO)
        return PARENTS_INCOMPATIBLE;

    // one parent is Ditto
    if (eggGroups[0][0] == EGG_GROUP_DITTO || eggGroups[1][0] == EGG_GROUP_DITTO)
    {
        if (trainerIds[0] == trainerIds[1])
            return PARENTS_LOW_COMPATIBILITY;

        return PARENTS_MED_COMPATIBILITY;
    }
    // neither parent is Ditto
    else
    {
        if (genders[0] == genders[1] || genders[0] == MON_GENDERLESS || genders[1] == MON_GENDERLESS || !EggGroupsOverlap(eggGroups[0], eggGroups[1]))
            return PARENTS_INCOMPATIBLE;
        
        if (species[0] == species[1])
        {
            if (trainerIds[0] == trainerIds[1])
                return PARENTS_MED_COMPATIBILITY; // same species, same trainer

            return PARENTS_MAX_COMPATIBILITY; // same species, different trainers
        }
        else
        {
            if (trainerIds[0] != trainerIds[1])
                return PARENTS_MED_COMPATIBILITY; // different species, different trainers

            return PARENTS_LOW_COMPATIBILITY; // different species, same trainer
        }
    }
}

void SetDaycareCompatibilityString(void)
{
	u8 whichString;
	
	switch (GetDaycareCompatibilityScore(&gSaveBlock1Ptr->daycare))
	{
		case PARENTS_MAX_COMPATIBILITY:
		    whichString = 0;
			break;
		case PARENTS_MED_COMPATIBILITY:
		    whichString = 1;
			break;
		case PARENTS_LOW_COMPATIBILITY:
		    whichString = 2;
			break;
		case PARENTS_INCOMPATIBLE:
		    whichString = 3;
			break;
	}
	StringCopy(gStringVar4, sCompatibilityMessages[whichString]);
}

//////////////////
// EGG CREATION //
//////////////////

// Used in scripts
void CreateEgg(struct Pokemon *mon, struct PokemonGenerator generator)
{
	u32 value;
	
    CreateMon(mon, generator);
	
	// Set friendship
	SetMonData(mon, MON_DATA_FRIENDSHIP, &gSpeciesInfo[GetMonData(mon, MON_DATA_SPECIES)].eggCycles);
	
	// Reset met level
	value = 0;
	SetMonData(mon, MON_DATA_MET_LEVEL, &value);
	
	// Set language ?
    value = LANGUAGE_JAPANESE;
    SetMonData(mon, MON_DATA_LANGUAGE, &value);
	
	// Set met loc
	value = METLOC_SPECIAL_EGG;
	SetMonData(mon, MON_DATA_MET_LOCATION, &value);
	
	// Set is an egg
    value = TRUE;
    SetMonData(mon, MON_DATA_IS_EGG, &value);
}

// Determines what the species of an Egg would be based on the given species.
// It determines this by working backwards through the evolution chain of the
// given species.
static u16 GetEggSpecies(u16 species)
{
    u8 i, k;
	u16 j;
    bool8 found;

    // Working backwards up to 5 times seems arbitrary, since the maximum number
    // of times would only be 3 for 3-stage evolutions.
    for (i = 0; i < 5; i++)
    {
        found = FALSE;
		
        for (j = SPECIES_NONE + 1; j < NUM_SPECIES; j++)
        {
			const u8 *evolutions = gSpeciesInfo[j].evolutions;
			if (evolutions == NULL)
				continue;
			
			while (*evolutions != EVOLUTIONS_END)
			{
				if (*evolutions == EVO_TARGET_SPECIES && species == READ_16(evolutions + 1))
				{
					species = j;
					found = TRUE;
					break;
				}
				evolutions += gEvolutionCmdArgumentsSize[*evolutions] + 1;
			}
			
            if (found)
                break;
        }

        if (j == NUM_SPECIES)
            break;
    }
    return species;
}

static u8 DetermineEggSpeciesAndParentSlots(struct DayCare *daycare, u8 *parentSlots, u16 *eggSpecies)
{
    u8 i, noDittoParent;
    u16 species[DAYCARE_MON_COUNT];
    u16 motherSpecies;

    for (i = 0; i < DAYCARE_MON_COUNT; ++i)
		species[i] = GetBoxMonData(&daycare->mons[i].mon, MON_DATA_SPECIES);

    // Determine which of the daycare mons is the mother and father of the egg.
    // The 0th index of the parentSlots array is considered the mother slot, and the
    // 1st index is the father slot.
    for (i = 0, motherSpecies = SPECIES_NONE; i < DAYCARE_MON_COUNT; i++)
    {
        if (species[i] == SPECIES_DITTO)
        {
			motherSpecies = species[i ^ 1];
            noDittoParent = parentSlots[0] = i ^ 1;
            parentSlots[1] = i;
        }
        else if (GetBoxMonGender(&daycare->mons[i].mon) == MON_FEMALE)
        {
			motherSpecies = species[i];
			
			if (species[i] != species[i ^ 1])
			{
				if (SpeciesToNationalPokedexNum(species[i]) == SpeciesToNationalPokedexNum(species[i ^ 1]))
				{
					if (GetBoxMonData(&daycare->mons[i].mon, MON_DATA_HELD_ITEM) != ITEM_EVERSTONE && GetBoxMonData(&daycare->mons[i ^ 1].mon, MON_DATA_HELD_ITEM) == ITEM_EVERSTONE)
						motherSpecies = species[i ^ 1];
				}
			}
            noDittoParent = parentSlots[0] = i;
            parentSlots[1] = i ^ 1;
        }
    }
    *eggSpecies = GetEggSpecies(motherSpecies);
	
	// Special cases
	switch (SpeciesToNationalPokedexNum(*eggSpecies))
	{
		case NATIONAL_DEX_NIDORAN_F: // Not present in the form change table
		    if (GetGenderFromSpeciesAndPersonality(*eggSpecies, daycare->offspringPersonality) == MON_MALE)
				*eggSpecies = SPECIES_NIDORAN_M;
			break;
		case NATIONAL_DEX_ILLUMISE: // Not present in the form change table
		    if (GetGenderFromSpeciesAndPersonality(*eggSpecies, daycare->offspringPersonality) == MON_MALE)
				*eggSpecies = SPECIES_VOLBEAT;
			break;
		case NATIONAL_DEX_MANAPHY: // Breeding a Manaphy always create an Phione
		    *eggSpecies = SPECIES_PHIONE;
			break;
		case NATIONAL_DEX_URSALUNA: // Blood moon Ursaluna will generate an Teddiursa's egg
		    *eggSpecies = SPECIES_TEDDIURSA;
			break;
	}
    // Make Ditto the "mother" slot if the other daycare mon is male.
    if (species[parentSlots[1]] == SPECIES_DITTO && GetBoxMonGender(&daycare->mons[parentSlots[0]].mon) != MON_FEMALE)
    {
        u8 ditto = parentSlots[1];
        noDittoParent = parentSlots[1] = parentSlots[0];
        parentSlots[0] = ditto;
    }
    return noDittoParent;
}

static void AlterEggSpeciesWithIncenseItem(u16 *species, u16 fatherItem, u16 motherItem)
{
    if (*species == SPECIES_WYNAUT || *species == SPECIES_AZURILL)
    {
        if (*species == SPECIES_WYNAUT && motherItem != ITEM_LAX_INCENSE && fatherItem != ITEM_LAX_INCENSE)
            *species = SPECIES_WOBBUFFET;
        else if (*species == SPECIES_AZURILL && motherItem != ITEM_SEA_INCENSE && fatherItem != ITEM_SEA_INCENSE)
            *species = SPECIES_MARILL;
		else if (*species == SPECIES_BUDEW && motherItem != ITEM_ROSE_INCENSE && fatherItem != ITEM_ROSE_INCENSE)
			*species = SPECIES_ROSELIA;
		else if (*species == SPECIES_CHINGLING && motherItem != ITEM_PURE_INCENSE && fatherItem != ITEM_PURE_INCENSE)
			*species = SPECIES_CHIMECHO;
		else if (*species == SPECIES_BONSLY && motherItem != ITEM_ROCK_INCENSE && fatherItem != ITEM_ROCK_INCENSE)
			*species = SPECIES_SUDOWOODO;
		else if (*species == SPECIES_MIME_JR && motherItem != ITEM_ODD_INCENSE && fatherItem != ITEM_ODD_INCENSE)
			*species = SPECIES_MR_MIME;
		else if (*species == SPECIES_HAPPINY && motherItem != ITEM_LUCK_INCENSE && fatherItem != ITEM_LUCK_INCENSE)
			*species = SPECIES_CHANSEY;
		else if (*species == SPECIES_MANTYKE && motherItem != ITEM_WAVE_INCENSE && fatherItem != ITEM_WAVE_INCENSE)
			*species = SPECIES_MANTINE;
		else if (*species == SPECIES_MUNCHLAX && motherItem != ITEM_FULL_INCENSE && fatherItem != ITEM_FULL_INCENSE)
			*species = SPECIES_SNORLAX;
    }
}

// Sets the initial data for the egg, after ii's used when the egg is hatched and given to the player
static void SetInitialEggData(struct Pokemon *mon, u16 species, u32 personality)
{
	u32 value;
	struct PokemonGenerator generator =
	{
	    .species = species,
	    .level = EGG_HATCH_LEVEL,
	    .forcedGender = MON_GENDERLESS,
	    .shinyType = GENERATE_SHINY_NORMAL,
	    .otIdType = OT_ID_PLAYER_ID,
	    .hasFixedPersonality = TRUE,
	    .fixedPersonality = personality,
	    .forcedNature = NUM_NATURES,
		.formChanges = gDeafultGeneratorFormChanges,
		.moves = {MOVE_NONE, MOVE_NONE, MOVE_NONE, MOVE_NONE},
	};
    CreateMon(mon, generator);

	// Set friendship
    SetMonData(mon, MON_DATA_FRIENDSHIP, &gSpeciesInfo[GetMonData(mon, MON_DATA_SPECIES)].eggCycles);
	
	// Reset met level
	value = 0;
    SetMonData(mon, MON_DATA_MET_LEVEL, &value);
	
	// Set language ?
	value = LANGUAGE_JAPANESE;
    SetMonData(mon, MON_DATA_LANGUAGE, &value);
}

void GiveEggFromDaycare(void)
{
	u16 species;
	struct Pokemon egg;
	struct DayCare *daycare = &gSaveBlock1Ptr->daycare;
	u8 parentSlots[DAYCARE_MON_COUNT], noDittoParent = DetermineEggSpeciesAndParentSlots(daycare, parentSlots, &species);
	struct BoxPokemon *father = &daycare->mons[parentSlots[1]].mon, *mother = &daycare->mons[parentSlots[0]].mon;
	u16 fatherItem = GetBoxMonData(father, MON_DATA_HELD_ITEM), motherItem = GetBoxMonData(mother, MON_DATA_HELD_ITEM);
	bool8 isEgg;
	
#if INCENSE_ITEM_BREEDING
    AlterEggSpeciesWithIncenseItem(&species, fatherItem, motherItem);
#endif
	
    SetInitialEggData(&egg, species, daycare->offspringPersonality);
	
    InheritIVs(&egg, daycare, fatherItem, motherItem);
    BuildEggMoveset(&egg, father, mother, fatherItem, motherItem);
	InheritAbilitySlot(&egg, &daycare->mons[noDittoParent].mon);
	InheritPokeball(&egg, &daycare->mons[noDittoParent ^ 1].mon, &daycare->mons[noDittoParent].mon);
	
    isEgg = TRUE;
    SetMonData(&egg, MON_DATA_IS_EGG, &isEgg);
	
    gPlayerParty[PARTY_SIZE - 1] = egg;
    CompactPartySlots();
    CalculatePlayerPartyCount();
	
    RemoveEggFromDayCare(daycare);
}

static void TriggerPendingDaycareEgg(void)
{
	struct Pokemon *mon = Alloc(sizeof(struct Pokemon));
	struct DayCare *daycare = &gSaveBlock1Ptr->daycare;
	u8 natureSlot = GetParentSlotToInheritNature(daycare);
	struct PokemonGenerator generator =
	{
		.species = SPECIES_BULBASAUR,
		.level = 1,
		.otIdType = OT_ID_PLAYER_ID,
		.shinyType = GENERATE_SHINY_LOCKED,
		.forcedGender = MON_GENDERLESS,
		.hasFixedPersonality = FALSE,
		.fixedPersonality = 0,
		.forcedNature = natureSlot == DAYCARE_MON_COUNT ? NUM_NATURES : GetNatureFromPersonality(GetBoxMonData(&daycare->mons[natureSlot].mon, MON_DATA_PERSONALITY)),
		.formChanges = NULL,
		.moves = {MOVE_NONE, MOVE_NONE, MOVE_NONE, MOVE_NONE},
	};
	
	// Generate a mon only to get its personality
	CreateMon(mon, generator);
	
	daycare->offspringPersonality = GetMonData(mon, MON_DATA_PERSONALITY);
	daycare->pending = TRUE;
	
	Free(mon);
	
    FlagSet(FLAG_PENDING_DAYCARE_EGG);
}

static void AddHatchedMonToParty(u8 id)
{
	u8 name[POKEMON_NAME_LENGTH + 1];
    struct Pokemon* mon = &gPlayerParty[id];
	u32 value;
	
	// Set friendship
	value = 120;
    SetMonData(mon, MON_DATA_FRIENDSHIP, &value);
	
	// Remove isEgg flag
	value = FALSE;
    SetMonData(mon, MON_DATA_IS_EGG, &value);
	
	// Set nickname
    value = GetMonData(mon, MON_DATA_SPECIES);
    GetSpeciesName(name, value);
    SetMonData(mon, MON_DATA_NICKNAME, name);
	
	// Register in pokedex
	value = SpeciesToNationalPokedexNum(value);
    GetSetPokedexFlag(value, FLAG_SET_SEEN);
    GetSetPokedexFlag(value, FLAG_SET_CAUGHT);

    GetMonNickname(mon, gStringVar1);
	
	// Set met location
    value = GetCurrentRegionMapSectionId();
    SetMonData(mon, MON_DATA_MET_LOCATION, &value);

    MonRestorePP(mon);
    CalculateMonStats(mon);
}

///////////////////////
// INHERIT FUNCTIONS //
///////////////////////

static void BuildEggMoveset(struct Pokemon *egg, struct BoxPokemon *father, struct BoxPokemon *mother, u16 fatherItem, u16 motherItem)
{
	u8 i, j, numLevelUpMoves, numEggMoves, numSharedParentMoves;
	u16 motherMoves[MAX_MON_MOVES], fatherMoves[MAX_MON_MOVES], eggFinalMoves[MAX_MON_MOVES];
	
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
		motherMoves[i] = GetBoxMonData(mother, MON_DATA_MOVE1 + i);
		fatherMoves[i] = GetBoxMonData(father, MON_DATA_MOVE1 + i);
		eggFinalMoves[i] = MOVE_NONE;
    }
	memset(sHatchedEggEggMoves, MOVE_NONE, EGG_MOVES_ARRAY_COUNT);
	memset(sHatchedEggLevelUpMoves, MOVE_NONE, MAX_LV_UP_MOVES);
	
	// Inherit egg moves
	numEggMoves = GetEggMoves(egg, sHatchedEggEggMoves);
	
	for (i = 0; i < numEggMoves; i++)
	{
		for (j = 0; j < MAX_MON_MOVES; j++)
		{
			if ((fatherMoves[j] && fatherMoves[j] == sHatchedEggEggMoves[i]) || (motherMoves[j] && motherMoves[j] == sHatchedEggEggMoves[i]))
			{
				if (GiveMoveToMon(egg, sHatchedEggEggMoves[i]) == MON_HAS_MAX_MOVES)
					DeleteFirstMoveAndGiveMoveToMon(egg, sHatchedEggEggMoves[i]);
			}
		}
	}
	
#if INHERIT_TMS
	// Inherit TM moves if father has it known
    for (i = 0; i < MAX_MON_MOVES && fatherMoves[i] != MOVE_NONE; i++)
    {
        for (j = 0; j < NUM_TECHNICAL_MACHINES; j++)
        {
			u16 move = ItemId_GetHoldEffectParam(ITEM_TM01 + j);
			
            if (fatherMoves[i] == move && CanMonLearnTM(egg, move))
            {
                if (GiveMoveToMon(egg, fatherMoves[i]) == MON_HAS_MAX_MOVES)
                    DeleteFirstMoveAndGiveMoveToMon(egg, fatherMoves[i]);
            }
        }
    }
#endif
	 
	// Inherit moves that both parents known
    for (i = 0, numSharedParentMoves = 0; i < MAX_MON_MOVES && fatherMoves[i] != MOVE_NONE; i++)
    {
        for (j = 0; j < MAX_MON_MOVES; j++)
        {
            if (fatherMoves[i] == motherMoves[j] && fatherMoves[i] != MOVE_NONE)
                eggFinalMoves[numSharedParentMoves++] = fatherMoves[i];
        }
    }
	numLevelUpMoves = GetLevelUpMovesBySpecies(GetMonData(egg, MON_DATA_SPECIES), sHatchedEggLevelUpMoves);
	
    for (i = 0; i < MAX_MON_MOVES && eggFinalMoves[i] != MOVE_NONE; i++)
    {
        for (j = 0; j < numLevelUpMoves; j++)
        {
            if (sHatchedEggLevelUpMoves[j] && eggFinalMoves[i] == sHatchedEggLevelUpMoves[j])
            {
                if (GiveMoveToMon(egg, eggFinalMoves[i]) == MON_HAS_MAX_MOVES)
                    DeleteFirstMoveAndGiveMoveToMon(egg, eggFinalMoves[i]);
				
                break;
            }
        }
    }
	
#if VOLT_TACKLE_BY_BREEDING
    // Pichu having Volt Tackle
    if (SpeciesToNationalPokedexNum(GetMonData(&egg, MON_DATA_SPECIES)) == NATIONAL_DEX_PICHU && (fatherItem == ITEM_LIGHT_BALL || motherItem == ITEM_LIGHT_BALL))
	{
		if (GiveMoveToMon(egg, MOVE_VOLT_TACKLE) == MON_HAS_MAX_MOVES)
            DeleteFirstMoveAndGiveMoveToMon(egg, MOVE_VOLT_TACKLE);
	}
#endif
}

#define SET_IV_TO_INHERIT(parent, statId)       \
	selectedIvs[selectedIvsCount] = statId;     \
	selectedParents[selectedIvsCount] = parent; \
	unavailableIvs |= gBitTable[statId];

static void InheritIVs(struct Pokemon *egg, struct DayCare *daycare, u16 fatherItem, u16 motherItem)
{
	u8 i, iv;
	u8 selectedIvs[(fatherItem == ITEM_DESTINY_KNOT || motherItem == ITEM_DESTINY_KNOT) ? DESTINY_KNOT_INHERITED_IV_COUNT : INHERITED_IV_COUNT];
	u8 selectedParents[ARRAY_COUNT(selectedIvs)], selectedIvsCount = 0;
	u32 unavailableIvs = 0;
	u8 fatherItemHoldEffectParam = ItemId_GetHoldEffectParam(fatherItem);
	u8 motherItemHoldEffectParam = ItemId_GetHoldEffectParam(motherItem);
	bool8 fatherHasPowerItem = (ItemId_GetHoldEffect(fatherItem) == HOLD_EFFECT_POWER_ITEM);
	bool8 motherHasPowerItem = (ItemId_GetHoldEffect(motherItem) == HOLD_EFFECT_POWER_ITEM);
	
	// Both parents have a power item hold, choose one to inherit at random
	if (fatherHasPowerItem && motherHasPowerItem)
	{
		i = RandomMax(DAYCARE_MON_COUNT); // Parent
		
		SET_IV_TO_INHERIT(i, i == 0 ? motherItemHoldEffectParam : fatherItemHoldEffectParam)
		selectedIvsCount++;
	}
	else if (fatherHasPowerItem) // Inherit iv from father
	{
		SET_IV_TO_INHERIT(1, fatherItemHoldEffectParam)
		selectedIvsCount++;
	}
	else if (motherHasPowerItem) // Inherit iv from mother
	{
		SET_IV_TO_INHERIT(0, motherItemHoldEffectParam)
		selectedIvsCount++;
	}
	
	// Select the random stats that the ivs will be inherited
	for (i = selectedIvsCount; i < ARRAY_COUNT(selectedIvs); i++)
	{
		do
		{
			iv = RandomMax(NUM_STATS);

		} while (unavailableIvs & gBitTable[iv]);
		
		SET_IV_TO_INHERIT(RandomMax(DAYCARE_MON_COUNT), iv)
	}

    // Set each of inherited IVs on the egg mon
    for (i = 0; i < ARRAY_COUNT(selectedIvs); i++)
    {
		iv = GetBoxMonData(&daycare->mons[selectedParents[i]].mon, MON_DATA_HP_IV + selectedIvs[i]);
		SetMonData(egg, MON_DATA_HP_IV + selectedIvs[i], &iv);
    }
}

static u8 GetParentSlotToInheritNature(struct DayCare *daycare)
{
	bool8 motherHasEverstone = (GetBoxMonData(&daycare->mons[0].mon, MON_DATA_HELD_ITEM) == ITEM_EVERSTONE);
	bool8 fatherHasEverstone = (GetBoxMonData(&daycare->mons[1].mon, MON_DATA_HELD_ITEM) == ITEM_EVERSTONE);
	
	// Both parents holding everstone, chose one at random
	if (motherHasEverstone && fatherHasEverstone)
		return RandomMax(DAYCARE_MON_COUNT);
	else if (motherHasEverstone) // The mother is holding an Everstone
	    return 0;
	else if (fatherHasEverstone) // The father is holding an Everstone
		return 1;
	else // No one is holding Everstone
		return DAYCARE_MON_COUNT;
}

static void InheritAbilitySlot(struct Pokemon *egg, struct BoxPokemon *mon)
{
	bool8 hasHidddenAbility = GetBoxMonData(mon, MON_DATA_ABILITY_HIDDEN);
	u8 abilitySlot = 0xFF; // If no inherit ability slot, stay with its generated ability slot
	bool8 abilityHidden = FALSE; // If no inherit ability hidden, always born with no hidden ability
	
	if (!hasHidddenAbility)
	{
		if (RandomPercent(80))
			abilitySlot = GetBoxMonData(mon, MON_DATA_ABILITY_NUM);
	}
	else
	{
		if (RandomPercent(60))
			abilityHidden = hasHidddenAbility;
	}
	
	if (abilitySlot != 0xFF)
		SetMonData(egg, MON_DATA_ABILITY_NUM, &abilitySlot);
	
	SetMonData(egg, MON_DATA_ABILITY_HIDDEN, &abilityHidden);
}

// Some pokeballs are banned from breeding
static u8 CorrectPokeballForBreeding(u8 pokeball)
{
	switch (pokeball)
	{
		case ITEM_MASTER_BALL:
		case ITEM_CHERISH_BALL:
		    return ITEM_POKE_BALL;
		default:
		    return pokeball;
	}
}

static void InheritPokeball(struct Pokemon *egg, struct BoxPokemon *father, struct BoxPokemon *mother)
{
	u8 eggPokeball;
	u8 fatherPokeball = CorrectPokeballForBreeding(GetBoxMonData(father, MON_DATA_POKEBALL));
	u8 motherPokeball = CorrectPokeballForBreeding(GetBoxMonData(mother, MON_DATA_POKEBALL));
	
	if (SpeciesToNationalPokedexNum(GetBoxMonData(father, MON_DATA_SPECIES)) == SpeciesToNationalPokedexNum(GetBoxMonData(mother, MON_DATA_SPECIES)))
		eggPokeball = RandomPercent(50) ? fatherPokeball : motherPokeball;
	else
		eggPokeball = motherPokeball;
	
	SetMonData(egg, MON_DATA_POKEBALL, &eggPokeball);
}

///////////////
// EGG HATCH //
///////////////

void EggHatch(void)
{
    ScriptContext2_Enable();
    CreateTask(Task_EggHatch, 10);
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, 0);
}

static void Task_EggHatch(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        SetMainCallback2(CB2_EggHatch_0);
        gFieldCallback = FieldCB_ContinueScriptHandleMusic;
        DestroyTask(taskId);
    }
}

static void VBlankCB_EggHatch(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void EggHatch_RunFrame(void)
{
	RunTasks();
    RunTextPrinters();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static u8 EggHatchCreateMonSprite(u8 a0, u8 switchID, u8 pokeID, u16* speciesLoc)
{
    u8 r4 = 0;
    u8 spriteID = 0; // r7
    struct Pokemon* mon = NULL; // r5

    if (a0 == 0)
    {
        mon = &gPlayerParty[pokeID];
        r4 = 1;
    }
    if (a0 == 1)
    {
        mon = &gPlayerParty[pokeID];
        r4 = 3;
    }
    switch (switchID)
    {
    case 0:
    {
        u16 species = GetMonData(mon, MON_DATA_SPECIES);
        LoadSpecialPokePic(species, GetMonData(mon, MON_DATA_PERSONALITY), TRUE, gMonSpritesGfxPtr->sprites[(a0 * 2) + 1]);
        LoadMonPalette(mon);
        *speciesLoc = species;
    }
        break;
    case 1:
        SetMultiuseSpriteTemplateToPokemon(GetMonData(mon, MON_DATA_SPECIES), r4);
        spriteID = CreateSprite(&gMultiuseSpriteTemplate, 120, 70, 6);
        gSprites[spriteID].invisible = TRUE;
        gSprites[spriteID].callback = SpriteCallbackDummy;
        break;
    }
    return spriteID;
}

static void CB2_EggHatch_0(void)
{
    switch (gMain.state)
    {
    case 0:
        SetGpuReg(REG_OFFSET_DISPCNT, 0);

        sEggHatchData = Alloc(sizeof(struct EggHatchData));
        AllocateMonSpritesGfx();
        sEggHatchData->eggPartyID = gSpecialVar_0x8004;
        sEggHatchData->eggShardVelocityID = 0;

        SetVBlankCallback(VBlankCB_EggHatch);
        gSpecialVar_0x8005 = GetCurrentMapMusic();

        ResetTempTileDataBuffers();
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sBgTemplates_EggHatch, ARRAY_COUNT(sBgTemplates_EggHatch));

        ChangeBgX(1, 0, 0);
        ChangeBgY(1, 0, 0);
        ChangeBgX(0, 0, 0);
        ChangeBgY(0, 0, 0);

        SetBgAttribute(1, 7, 2);
        SetBgTilemapBuffer(1, Alloc(0x1000));
        SetBgTilemapBuffer(0, Alloc(0x2000));

        DeactivateAllTextPrinters();
        ResetPaletteFade();
        FreeAllSpritePalettes();
        ResetSpriteData();
        ResetTasks();
        ScanlineEffect_Stop();
        m4aSoundVSyncOn();
        gMain.state++;
        break;
    case 1:
        InitWindows(sWinTemplates_EggHatch);
        sEggHatchData->windowId = 0;
        gMain.state++;
        break;
    case 2:
        DecompressAndLoadBgGfxUsingHeap(0, gBattleTextboxTiles, 0, 0, 0);
        CopyToBgTilemapBuffer(0, gBattleTextboxTilemap, 0, 0);
        LoadCompressedPalette(gBattleTextboxPalette, 0, 0x20);
        gMain.state++;
        break;
    case 3:
        LoadSpriteSheet(&sEggHatch_Sheet);
        LoadSpriteSheet(&sEggShards_Sheet);
        LoadSpritePalette(&sEgg_SpritePalette);
        gMain.state++;
        break;
    case 4:
        CopyBgTilemapBufferToVram(0);
        AddHatchedMonToParty(sEggHatchData->eggPartyID);
        gMain.state++;
        break;
    case 5:
        EggHatchCreateMonSprite(0, 0, sEggHatchData->eggPartyID, &sEggHatchData->species);
        gMain.state++;
        break;
    case 6:
        sEggHatchData->pokeSpriteID = EggHatchCreateMonSprite(0, 1, sEggHatchData->eggPartyID, &sEggHatchData->species);
        gMain.state++;
        break;
    case 7:
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        LoadPalette(gTradeGba2_Pal, 0x10, 0xA0);
        LoadBgTiles(1, gTradeGba_Gfx, 0x1420, 0);
        CopyToBgTilemapBuffer(1, gTradeOrHatchMonShadowTilemap, 0x1000, 0);
        CopyBgTilemapBufferToVram(1);
        gMain.state++;
        break;
    case 8:
        SetMainCallback2(CB2_EggHatch_1);
        sEggHatchData->CB2_state = 0;
        break;
    }
    EggHatch_RunFrame();
}

static void CB2_EggHatch_1(void)
{
    u16 species;
    u8 gender;

    switch (sEggHatchData->CB2_state)
    {
    case 0:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK);
        sEggHatchData->eggSpriteID = CreateSprite(&sSpriteTemplate_EggHatch, 120, 75, 5);
        ShowBg(0);
        ShowBg(1);
        sEggHatchData->CB2_state++;
        CreateTask(Task_EggHatchPlayBGM, 5);
        break;
    case 1:
        if (!gPaletteFade.active)
        {
            FillWindowPixelBuffer(sEggHatchData->windowId, 0x00);
            sEggHatchData->CB2_PalCounter = 0;
            sEggHatchData->CB2_state++;
        }
        break;
    case 2:
        if (++sEggHatchData->CB2_PalCounter > 30)
        {
            sEggHatchData->CB2_state++;
            gSprites[sEggHatchData->eggSpriteID].callback = SpriteCB_Egg_0;
        }
        break;
    case 3:
        if (gSprites[sEggHatchData->eggSpriteID].callback == SpriteCallbackDummy)
        {
            PlayCry1(sEggHatchData->species, 0);
            sEggHatchData->CB2_state++;
        }
        break;
    case 4:
        if (IsCryFinished())
        {
            sEggHatchData->CB2_state++;
        }
        break;
    case 5:
        GetMonNickname(&gPlayerParty[sEggHatchData->eggPartyID], gStringVar1);
        StringExpandPlaceholders(gStringVar4, gText_HatchedFromEgg);
        EggHatchPrintMessage(sEggHatchData->windowId, gStringVar4, 0, 3, 0xFF);
        PlayFanfare(MUS_EVOLVED);
        sEggHatchData->CB2_state++;
        PutWindowTilemap(sEggHatchData->windowId);
        CopyWindowToVram(sEggHatchData->windowId, COPYWIN_BOTH);
        break;
    case 6:
        if (IsFanfareTaskInactive())
            sEggHatchData->CB2_state++;
        break;
    case 7:
        if (IsFanfareTaskInactive())
            sEggHatchData->CB2_state++;
        break;
    case 8:
        GetMonNickname(&gPlayerParty[sEggHatchData->eggPartyID], gStringVar1);
        StringExpandPlaceholders(gStringVar4, gText_NickHatchPrompt);
        EggHatchPrintMessage(sEggHatchData->windowId, gStringVar4, 0, 2, 1);
        sEggHatchData->CB2_state++;
        break;
    case 9:
        if (!IsTextPrinterActive(sEggHatchData->windowId))
        {
            LoadUserWindowBorderGfx(sEggHatchData->windowId, 0x140, 0xE0);
            CreateYesNoMenu(&sYesNoWinTemplate, 3, 0, 2, 0x140, 0xE, 0);
            sEggHatchData->CB2_state++;
        }
        break;
    case 10:
        switch (Menu_ProcessInputNoWrapClearOnChoose())
        {
        case 0:
            GetMonNickname(&gPlayerParty[sEggHatchData->eggPartyID], gStringVar3);
            DoNamingScreen(NAMING_SCREEN_NAME_RATER, gStringVar3, GetMonData(&gPlayerParty[sEggHatchData->eggPartyID], MON_DATA_SPECIES), GetMonGender(&gPlayerParty[sEggHatchData->eggPartyID]), EggHatchSetMonNickname);
            break;
        case 1:
        case -1:
            sEggHatchData->CB2_state++;
        }
        break;
    case 11:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
        sEggHatchData->CB2_state++;
        break;
    case 12:
        if (!gPaletteFade.active)
        {
            RemoveWindow(sEggHatchData->windowId);
            UnsetBgTilemapBuffer(0);
            UnsetBgTilemapBuffer(1);
            Free(sEggHatchData);
            SetMainCallback2(CB2_ReturnToField);
        }
        break;
    }
    EggHatch_RunFrame();
}

static void EggHatchPrintMessage(u8 windowId, u8* string, u8 x, u8 y, u8 speed)
{
    FillWindowPixelBuffer(windowId, 0xFF);
    sEggHatchData->textColor[0] = 0;
    sEggHatchData->textColor[1] = 5;
    sEggHatchData->textColor[2] = 6;
    AddTextPrinterParameterized4(windowId, 3, x, y, 1, 1, sEggHatchData->textColor, speed, string);
}

static void EggHatchSetMonNickname(void)
{
    SetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_NICKNAME, gStringVar3);
    FreeMonSpritesGfx();
    Free(sEggHatchData);
    SetMainCallback2(CB2_ReturnToField);
}

static void Task_EggHatchPlayBGM(u8 taskId)
{
    if (gTasks[taskId].data[0] == 0)
        StopMapMusic();
	
    if (gTasks[taskId].data[0] == 1)
        PlayBGM(MUS_EVOLUTION_INTRO);
	
    if (gTasks[taskId].data[0] > 60)
    {
        PlayBGM(MUS_EVOLUTION);
        DestroyTask(taskId);
        return;
    }
    gTasks[taskId].data[0]++;
}

static void CreateRandomEggShardSprite(void)
{
	u8 spriteId = CreateSprite(&sSpriteTemplate_EggShard, 120, 60, 4);
	
    gSprites[spriteId].data[1] = sEggShardVelocities[sEggHatchData->eggShardVelocityID][0];
    gSprites[spriteId].data[2] = sEggShardVelocities[sEggHatchData->eggShardVelocityID][1];
    gSprites[spriteId].data[3] = 100;
	
    StartSpriteAnim(&gSprites[spriteId], RandomMax(4));
	
	sEggHatchData->eggShardVelocityID++;
}

static void SpriteCB_Egg_0(struct Sprite* sprite)
{
    if (++sprite->data[0] > 20)
    {
        sprite->callback = SpriteCB_Egg_1;
        sprite->data[0] = 0;
    }
    else
    {
        sprite->data[1] = (sprite->data[1] + 20) & 0xFF;
        sprite->x2 = Sin(sprite->data[1], 1);
		
        if (sprite->data[0] == 15)
        {
            PlaySE(SE_BALL);
            StartSpriteAnim(sprite, 1);
            CreateRandomEggShardSprite();
        }
    }
}

static void SpriteCB_Egg_1(struct Sprite* sprite)
{
    if (++sprite->data[2] > 30)
    {
        if (++sprite->data[0] > 20)
        {
            sprite->callback = SpriteCB_Egg_2;
            sprite->data[0] = 0;
            sprite->data[2] = 0;
        }
        else
        {
            sprite->data[1] = (sprite->data[1] + 20) & 0xFF;
            sprite->x2 = Sin(sprite->data[1], 2);
			
            if (sprite->data[0] == 15)
            {
                PlaySE(SE_BALL);
                StartSpriteAnim(sprite, 2);
            }
        }
    }
}

static void SpriteCB_Egg_2(struct Sprite* sprite)
{
    if (++sprite->data[2] > 30)
    {
        if (++sprite->data[0] > 38)
        {
            sprite->callback = SpriteCB_Egg_3;
            sprite->data[0] = 0;
            gSprites[sEggHatchData->pokeSpriteID].x2 = 0;
            gSprites[sEggHatchData->pokeSpriteID].y2 = gSpeciesInfo[GetMonData(&gPlayerParty[sEggHatchData->eggPartyID], MON_DATA_SPECIES)].frontPicYOffset;
        }
        else
        {
            sprite->data[1] = (sprite->data[1] + 20) & 0xFF;
            sprite->x2 = Sin(sprite->data[1], 2);
			
            if (sprite->data[0] == 15)
            {
                PlaySE(SE_BALL);
                StartSpriteAnim(sprite, 2);
                CreateRandomEggShardSprite();
                CreateRandomEggShardSprite();
            }
            if (sprite->data[0] == 30)
                PlaySE(SE_BALL);
        }
    }
}

static void SpriteCB_Egg_3(struct Sprite* sprite)
{
    if (++sprite->data[0] > 50)
    {
        sprite->callback = SpriteCB_Egg_4;
        sprite->data[0] = 0;
    }
}

static void SpriteCB_Egg_4(struct Sprite* sprite)
{
    u8 i;
	
    if (sprite->data[0] == 0)
        BeginNormalPaletteFade(PALETTES_ALL, -1, 0, 0x10, 0xFFFF);
	
    if (sprite->data[0] < 4u)
    {
        for (i = 0; i <= 3; i++)
            CreateRandomEggShardSprite();
    }
    sprite->data[0]++;
	
    if (!gPaletteFade.active)
    {
        PlaySE(SE_EGG_HATCH);
        sprite->invisible = TRUE;
        sprite->callback = SpriteCB_Egg_5;
        sprite->data[0] = 0;
    }
}

static void SpriteCB_Egg_5(struct Sprite* sprite)
{
    if (sprite->data[0] == 0)
    {
        gSprites[sEggHatchData->pokeSpriteID].invisible = FALSE;
        StartSpriteAffineAnim(&gSprites[sEggHatchData->pokeSpriteID], 1);
    }
    if (sprite->data[0] == 8)
        BeginNormalPaletteFade(PALETTES_ALL, -1, 0x10, 0, 0xFFFF);
	
    if (sprite->data[0] <= 9)
        gSprites[sEggHatchData->pokeSpriteID].y -= 1;
	
    if (sprite->data[0] > 40)
        sprite->callback = SpriteCallbackDummy;
	
    sprite->data[0]++;
}

static void SpriteCB_EggShard(struct Sprite* sprite)
{
    sprite->data[4] += sprite->data[1];
    sprite->data[5] += sprite->data[2];

    sprite->x2 = sprite->data[4] / 256;
    sprite->y2 = sprite->data[5] / 256;

    sprite->data[2] += sprite->data[3];

    if (sprite->y + sprite->y2 > sprite->y + 20 && sprite->data[2] > 0)
        DestroySprite(sprite);
}

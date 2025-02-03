#include "battle_util.h"
#include "bg.h"
#include "ev_iv_display.h"
#include "global.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "new_menu_helpers.h"
#include "oak_speech.h"
#include "overworld.h"
#include "palette.h"
#include "party_menu.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "region_map.h"
#include "scanline_effect.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "strings.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "window.h"
#include "constants/pokemon.h"
#include "constants/songs.h"

/* used windows */
enum
{
	WIN_INSTRUCTIONS,
	WIN_TITLE,
	WIN_HIDDEN_POWER_TYPE,
	WIN_EVIV_TEXT,
	WIN_POKEMON_NAME,
	WIN_STAT_NAMES,
	WIN_STATS,
};

// this file's functions
static void CB2_EvIvDisplay(void);
static void VBlank_EvIvDisplay(void);
static void CreateShadowBox(void);
static void CreateMonIconSprites(void);
static void CreateHandSprite(u8 taskId);
static void PrintInfoText(void);
static void PrintMonStats(u8 taskId);
static void Task_LoadEvIvDisplay(u8 taskId);
static void Task_EvIvDisplay_HandleInput(u8 taskId);
static void Task_WaitFadeAndCloseEvIvDisplay(u8 taskId);
static void SpriteCB_Hand(struct Sprite *sprite);

#define TAG_EV_IV_HAND 9999

#define HAND_INITIAL_X 145
#define HAND_INITIAL_Y 70
#define HAND_X_Y_DIFF  40

static const struct BgTemplate sEvIvBgTemplates[] = 
{
	{ // diploma bg
		.bg = 0,
		.charBaseIndex = 1,
		.mapBaseIndex = 29,
		.screenSize = 0,
		.paletteMode = 0,
		.priority = 1,
		.baseTile = 0,
	},{ // texts
		.bg = 3,
		.charBaseIndex = 0,
		.mapBaseIndex = 31,
		.screenSize = 0,
		.paletteMode = 0,
		.priority = 0,
		.baseTile = 1,
	}
};

static const struct WindowTemplate sEvIvWinTemplates[] = 
{
	[WIN_INSTRUCTIONS] =
	{
		.bg = 3,
		.tilemapLeft = 19,
		.tilemapTop = 18,
		.width = 11,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 0x000
	},
	[WIN_TITLE] =
	{
		.bg = 3,
		.tilemapLeft = 10,
		.tilemapTop = 2,
		.width = 10,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 0x016
	},
	[WIN_HIDDEN_POWER_TYPE] =
	{
		.bg = 3,
		.tilemapLeft = 0,
		.tilemapTop = 5,
		.width = 5,
		.height = 2,
		.paletteNum = 1,
		.baseBlock = 0x02A
	},
	[WIN_EVIV_TEXT] =
	{
		.bg = 3,
		.tilemapLeft = 8,
		.tilemapTop = 5,
		.width = 6,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 0x034
	},
	[WIN_POKEMON_NAME] =
	{
		.bg = 3,
		.tilemapLeft = 16,
		.tilemapTop = 5,
		.width = 14,
		.height = 2,
		.paletteNum = 15,
		.baseBlock = 0x040
	},
	[WIN_STAT_NAMES] =
	{
		.bg = 3,
		.tilemapLeft = 0,
		.tilemapTop = 7,
		.width = 8,
		.height = 11,
		.paletteNum = 15,
		.baseBlock = 0x05C
	},
	[WIN_STATS] =
	{
		.bg = 3,
		.tilemapLeft = 8,
		.tilemapTop = 7,
		.width = 6,
		.height = 11,
		.paletteNum = 15,
		.baseBlock = 0x0B4
	},
	DUMMY_WIN_TEMPLATE
};

static const struct SpriteSheet sEvIvHandSpriteSheet = 
{
	.data = sHandCursorTiles,
	.size = 0x800,
	.tag = TAG_EV_IV_HAND
};
	
static const struct SpritePalette sEvIvHandSpritePalette = 
{
	.data = gUnknown_83CE7F0,
	.tag = TAG_EV_IV_HAND
};

static const struct OamData sEvIvHandOamData = 
{
	.shape = SPRITE_SHAPE(32x32),
	.size = SPRITE_SIZE(32x32),
	.priority = 1,
};

static const struct SpriteTemplate sEvIvHandSpriteTemplate = 
{
	.tileTag = TAG_EV_IV_HAND,
	.paletteTag = TAG_EV_IV_HAND,
	.oam = &sEvIvHandOamData,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = gDummySpriteAffineAnimTable,
	.callback = SpriteCB_Hand,
};

static const u8 *const sStatNameStrings[] =
{
	[STAT_HP]    = COMPOUND_STRING("Hp:"),
	[STAT_ATK]   = COMPOUND_STRING("Attack:"),
	[STAT_DEF]   = COMPOUND_STRING("Defense:"),
	[STAT_SPEED] = COMPOUND_STRING("Speed:"),
	[STAT_SPATK] = COMPOUND_STRING("Sp.Attack:"),
	[STAT_SPDEF] = COMPOUND_STRING("Sp.Defense:"),
};

// the text colors
static const u8 sWhiteTextColor[3] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY};
static const u8 sDarkGrayTextColor[3] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY};

// task defines
#define tCurrMonId    data[0]
#define tOnInit       data[1]
#define tCallbackStep data[2]
#define tExitCB       3

void ShowEvIvDisplay(MainCallback exitCallback)
{
	u8 taskId;
	
	ResetSpriteData();
	ResetPaletteFade();
	FreeAllSpritePalettes();
	ResetTasks();
	ScanlineEffect_Stop();
	PlaySE(SE_PC_ON);
	taskId = CreateTask(Task_LoadEvIvDisplay, 0);
	gTasks[taskId].tCurrMonId = 0;
	gTasks[taskId].tCallbackStep = 0;
	gTasks[taskId].tOnInit = TRUE; // for don't play the mon cry on open the system
	SetWordTaskArg(taskId, tExitCB, (u32)exitCallback);
	SetMainCallback2(CB2_EvIvDisplay);
}

static void CB2_EvIvDisplay(void)
{
	RunTasks();
	AnimateSprites();
	BuildOamBuffer();
	UpdatePaletteFade();
}

static void VBlank_EvIvDisplay(void)
{
	LoadOam();
	ProcessSpriteCopyRequests();
	TransferPlttBuffer();
}

static void CreateShadowBox(void)
{
	SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_NONE);
	SetGpuReg(REG_OFFSET_BLDY, 0);
	SetGpuReg(REG_OFFSET_WIN0V, 0);
	SetGpuReg(REG_OFFSET_WIN0H, 0);
	SetGpuReg(REG_OFFSET_WIN1V, 0);
	SetGpuReg(REG_OFFSET_WIN1H, 0);
	SetGpuReg(REG_OFFSET_WININ, 0);
	SetDispCnt(0, TRUE);
	SetDispCnt(1, TRUE);
	SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_DARKEN);
	SetGpuReg(REG_OFFSET_BLDY, BLDCNT_TGT1_BG0 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG1);
	SetGpuReg(REG_OFFSET_WININ, 0x3939);
	SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ);
	// first box
	SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(40, 144));
	SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(8, 112));
	// second box
	SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(40, 144));
	SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(128, 112));
	
	SetDispCnt(0, FALSE);
}

static void Task_LoadEvIvDisplay(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	
	switch (tCallbackStep)
	{
		case 0:
		    SetVBlankCallback(NULL);
		    break;
		case 1:
		    DmaClearLarge16(3, (void *)VRAM, VRAM_SIZE, 0x1000);
		    DmaClear32(3, (void *)OAM, OAM_SIZE);
		    DmaClear16(3, (void *)PLTT, PLTT_SIZE);
		    SetGpuReg(REG_OFFSET_DISPCNT, 0);
		    SetGpuReg(REG_OFFSET_BG0CNT, 0);
		    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
		    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
		    SetGpuReg(REG_OFFSET_BG1CNT, 0);
		    SetGpuReg(REG_OFFSET_BG1HOFS, 0);
		    SetGpuReg(REG_OFFSET_BG1VOFS, 0);
		    SetGpuReg(REG_OFFSET_BG2CNT, 0);
		    SetGpuReg(REG_OFFSET_BG2HOFS, 0);
		    SetGpuReg(REG_OFFSET_BG2VOFS, 0);
		    SetGpuReg(REG_OFFSET_BG3CNT, 0);
		    SetGpuReg(REG_OFFSET_BG3HOFS, 0);
		    SetGpuReg(REG_OFFSET_BG3VOFS, 0);
		    break;
		case 2:
		    ResetBgsAndClearDma3BusyFlags(0);
		    InitBgsFromTemplates(0, sEvIvBgTemplates, 2);
			ResetAllBgsPos();
		    break;
		case 3:
		    InitWindows(sEvIvWinTemplates);
		    DeactivateAllTextPrinters();
		    break;
		case 4:
		    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
		    break;
		case 5:
		    SetBgTilemapBuffer(0, (u16*)AllocZeroed(BG_SCREEN_SIZE));
		    break;
		case 6:
		    CreateShadowBox();
		    break;
		case 7:
		    ShowBg(0);
		    ShowBg(3);
		    break;
		case 8:
		    FillBgTilemapBufferRect_Palette0(0, 0, 0, 0, 30, 20);
		    FillBgTilemapBufferRect_Palette0(3, 0, 0, 0, 30, 20);
		    break;
		case 9:
		    ResetTempTileDataBuffers();
		    DecompressAndCopyTileDataToVram(0, sDiplomaGfx, 0, 0, 0);
		    break;
		case 10:
		    if (!(FreeTempTileDataBuffersIfPossible() == 1))
			    break;
		    return;
		case 11:
		    LoadPalette(sDiplomaPal, 0, 0x20);
		    ListMenuLoadStdPalAt(0x10, 1);
		    break;
		case 12:
		    CopyToBgTilemapBuffer(0, sDiplomaTilemap, 0, 0);
		    break;
		case 13:
		    SetGpuReg(REG_OFFSET_BG3HOFS, 0);
		    break;
		case 14:
		    LoadMonIconPalettes();
		    break;
		case 15:
#if PIKACHU_ICON
		    CreatePikaOrGrassPlatformSpriteAndLinkToCurrentTask(taskId, 0);
#endif
		    CreateHandSprite(taskId);
		    CreateMonIconSprites();
		    break;
		case 16:
		    PrintInfoText();
		    PrintMonStats(taskId);
		    break;
		case 17:
		    CopyBgTilemapBufferToVram(0);
		    CopyBgTilemapBufferToVram(3);
		    break;
		case 18:
		    BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
		    break;
		case 19:
		    SetVBlankCallback(VBlank_EvIvDisplay);
		    break;
		case 20:
		    if (gPaletteFade.active)
			    return;
		    gTasks[taskId].tOnInit = FALSE;
		    gTasks[taskId].func = Task_EvIvDisplay_HandleInput;
	}
	tCallbackStep++;
}

static void CreateHandSprite(u8 taskId)
{
	LoadSpriteSheet(&sEvIvHandSpriteSheet);
	LoadSpritePalette(&sEvIvHandSpritePalette);
	gSprites[CreateSprite(&sEvIvHandSpriteTemplate, HAND_INITIAL_X, HAND_INITIAL_Y, 0)].data[0] = taskId;
}

#define GET_HAND_POS_X(partyId) (((partyId % (PARTY_SIZE / 2)) * HAND_X_Y_DIFF + HAND_INITIAL_X))
#define GET_HAND_POS_Y(partyId) ((partyId / (PARTY_SIZE / 2)) * HAND_X_Y_DIFF + HAND_INITIAL_Y)

static void CreateMonIconSprites(void)
{
	u8 i;
	
	for (i = 0; i < gPlayerPartyCount; i++)
		CreateMonIcon(GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2), SpriteCB_MonIcon, GET_HAND_POS_X(i), GET_HAND_POS_Y(i) + 10, 1);
}

static void SpriteCB_Hand(struct Sprite *sprite)
{
	u8 partyId = gTasks[sprite->data[0]].tCurrMonId;
	sprite->x = GET_HAND_POS_X(partyId);
	sprite->y = GET_HAND_POS_Y(partyId);
}

// X and Y positions of the texts and type icon
#define INSTRUCTION_PRINTER_X_POS 6
#define TITLE_PRINTER_X_POS 3
#define HIDDEN_POWER_PRINTER_X_POS 4
#define EV_IV_PRINTER_X_POS 2
#define POKEMON_NAME_PRINTER_X_POS HIDDEN_POWER_PRINTER_X_POS
#define POKEMON_NUM_PRINTER_X_POS POKEMON_NAME_PRINTER_X_POS + 86
#define STATS_NAME_PRINTER_X_POS HIDDEN_POWER_PRINTER_X_POS
#define EV_IV_STATS_PRINTER_X_POS EV_IV_PRINTER_X_POS

#define PRINTER_Y_POS 2
#define GET_STAT_PRINTER_Y_POS(i)((i * 13) + 6)

static void PrintInfoText(void)
{
	u8 i;
	
	FillWindowPixelBuffer(WIN_INSTRUCTIONS, PIXEL_FILL(0));
	FillWindowPixelBuffer(WIN_TITLE, PIXEL_FILL(0));
	FillWindowPixelBuffer(WIN_EVIV_TEXT, PIXEL_FILL(0));
	FillWindowPixelBuffer(WIN_STAT_NAMES, PIXEL_FILL(0));
	
	// Print The Instructions
	AddTextPrinterParameterized3(WIN_INSTRUCTIONS, 2, INSTRUCTION_PRINTER_X_POS + 39, PRINTER_Y_POS, sDarkGrayTextColor, 0, COMPOUND_STRING("{A_BUTTON}{B_BUTTON}Quit"));
	
	if (gPlayerPartyCount > 1)
		AddTextPrinterParameterized3(WIN_INSTRUCTIONS, 2, INSTRUCTION_PRINTER_X_POS, PRINTER_Y_POS, sDarkGrayTextColor, 0, COMPOUND_STRING("{DPAD_ANY}Move"));
	
	// Print The "Ev-Iv Display" Text
	AddTextPrinterParameterized3(WIN_TITLE, 2, TITLE_PRINTER_X_POS, PRINTER_Y_POS, sDarkGrayTextColor, 0, COMPOUND_STRING("Ev-Iv Display"));
	
	// Print The Ev Iv Text
	AddTextPrinterParameterized3(WIN_EVIV_TEXT, 2, EV_IV_PRINTER_X_POS, PRINTER_Y_POS - 2, sWhiteTextColor, 0, COMPOUND_STRING("Ev   Iv"));
	
	for (i = 0; i < NUM_STATS; i++)
		// Print The Nº Stat Name
		AddTextPrinterParameterized3(WIN_STAT_NAMES, 2, STATS_NAME_PRINTER_X_POS, GET_STAT_PRINTER_Y_POS(i), sWhiteTextColor, 0, sStatNameStrings[i]);
	
	PutWindowTilemap(WIN_INSTRUCTIONS);
	PutWindowTilemap(WIN_TITLE);
	PutWindowTilemap(WIN_EVIV_TEXT);
	PutWindowTilemap(WIN_STAT_NAMES);
}

static void PrintMonStats(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	u8 i, ev, iv;
	struct Pokemon *mon = &gPlayerParty[tCurrMonId];
	bool8 isEgg = GetMonData(mon, MON_DATA_IS_EGG);
	
	FillWindowPixelBuffer(WIN_HIDDEN_POWER_TYPE, PIXEL_FILL(0));
	FillWindowPixelBuffer(WIN_POKEMON_NAME, PIXEL_FILL(0));
	FillWindowPixelBuffer(WIN_STATS, PIXEL_FILL(0));
	
#if HIDDEN_POWER_TYPE
	// Print The Hidden Power Type
	if (!isEgg)
		BlitMoveInfoIcon(WIN_HIDDEN_POWER_TYPE, GetHiddenPowerType(mon) + 1, HIDDEN_POWER_PRINTER_X_POS, PRINTER_Y_POS);
#endif
	
	// Print The Mon Nickname
	GetMonNickname(mon, gStringVar4);
	AddTextPrinterParameterized3(WIN_POKEMON_NAME, 2, POKEMON_NAME_PRINTER_X_POS, PRINTER_Y_POS - 2, sWhiteTextColor, 0, gStringVar4);
	
	// Print The Current Mon Num
	ConvertIntToDecimalStringN(gStringVar4, tCurrMonId + 1, STR_CONV_MODE_LEFT_ALIGN, 1);
	StringAppend(gStringVar4, gText_Slash);
	ConvertIntToDecimalStringN(gStringVar1, gPlayerPartyCount, STR_CONV_MODE_LEFT_ALIGN, 1);
	StringAppend(gStringVar4, gStringVar1);
	AddTextPrinterParameterized3(WIN_POKEMON_NAME, 2, POKEMON_NUM_PRINTER_X_POS, PRINTER_Y_POS - 2, sWhiteTextColor, 0, gStringVar4);
	
	for (i = 0; i < NUM_STATS; i++)
	{
		if (isEgg)
			ev = iv = 0;
		else
		{
			ev = GetMonData(mon, MON_DATA_HP_EV + i);
			iv = GetMonData(mon, MON_DATA_HP_IV + i);
		}
		// Print The Nº Ev
		ConvertIntToDecimalStringN(gStringVar4, ev, STR_CONV_MODE_LEFT_ALIGN, 3);
		AddTextPrinterParameterized3(WIN_STATS, 2, EV_IV_STATS_PRINTER_X_POS, GET_STAT_PRINTER_Y_POS(i), sWhiteTextColor, 0, gStringVar4);
		
		// Print The Nº Iv
		ConvertIntToDecimalStringN(gStringVar4, iv, STR_CONV_MODE_LEFT_ALIGN, 2);
		AddTextPrinterParameterized3(WIN_STATS, 2, EV_IV_STATS_PRINTER_X_POS + 30, GET_STAT_PRINTER_Y_POS(i), sWhiteTextColor, 0, gStringVar4);
	}
	// Play The Mon Cry
	if (!isEgg && !tOnInit)
		PlayCry_Normal(GetMonData(mon, MON_DATA_SPECIES), 0);
	
	PutWindowTilemap(WIN_HIDDEN_POWER_TYPE);
	CopyWindowToVram(WIN_HIDDEN_POWER_TYPE, COPYWIN_GFX);
	PutWindowTilemap(WIN_POKEMON_NAME);
	PutWindowTilemap(WIN_STATS);
}

static void Task_EvIvDisplay_HandleInput(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	
	if (gPlayerPartyCount > 1)
	{
		if (JOY_REPT(DPAD_RIGHT))
		{
			if (tCurrMonId == (gPlayerPartyCount - 1))
				tCurrMonId = 0;
			else
				tCurrMonId++;
			PrintMonStats(taskId);
		}
		if (JOY_REPT(DPAD_LEFT))
		{
			if (tCurrMonId == 0)
				tCurrMonId = (gPlayerPartyCount - 1);
			else
				tCurrMonId--;
			PrintMonStats(taskId);
		}
		if (JOY_REPT(DPAD_DOWN | DPAD_UP))
		{
			if (gPlayerPartyCount > (PARTY_SIZE / 2))
			{
				if (tCurrMonId > ((PARTY_SIZE / 2) - 1))
					tCurrMonId -= (PARTY_SIZE / 2);
				else
				{
					tCurrMonId += (PARTY_SIZE / 2);
					if (tCurrMonId > (gPlayerPartyCount - 1))
						tCurrMonId = (gPlayerPartyCount - 1);
				}
				PrintMonStats(taskId);
			}
		}
	}
	if (JOY_NEW(A_BUTTON | B_BUTTON))
	{
		PlaySE(SE_PC_OFF);
		BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
		gTasks[taskId].func = Task_WaitFadeAndCloseEvIvDisplay;
	}
}

static void Task_WaitFadeAndCloseEvIvDisplay(u8 taskId)
{
	void *bgBuff;
	
	if (!gPaletteFade.active && !IsCryPlaying())
	{
		FreeAllWindowBuffers();
	    SetMainCallback2((MainCallback)GetWordTaskArg(taskId, tExitCB));
		bgBuff = GetBgTilemapBuffer(0);
	    FREE_AND_SET_NULL(bgBuff);
	    DestroyTask(taskId);
	}
}

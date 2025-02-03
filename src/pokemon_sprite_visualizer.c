// Credits: Gamer2020, AsparagusEduardo, TheXaman, ShinyDragonHunter
#include "global.h"
#include "gflib.h"
#include "battle_anim.h"
#include "battle_interface.h"
#include "decompress.h"
#include "m4a.h"
#include "overworld.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "pokemon_sprite_visualizer.h"
#include "scanline_effect.h"
#include "text_window.h"
#include "trainer_pokemon_sprites.h"
#include "constants/battle.h"
#include "constants/event_objects.h"
#include "constants/map_types.h"
#include "constants/songs.h"
#include "gba/m4a_internal.h"

// Windows
enum
{
	WIN_NAME_NUMBERS,
	WIN_INSTRUCTIONS,
	WIN_BOTTOM_LEFT,
	WIN_BOTTOM_RIGHT,
	WIN_FOOTPRINT,
	NUM_SPRITE_VISUALIZER_WINDOWS,
};

enum
{
    ARROW_DOWN,
    ARROW_UP,
    ARROW_RIGHT,
};

// Defines
#define VISUALIZER_MON_BACK_X 62
#define VISUALIZER_MON_BACK_Y 80

#define VISUALIZER_ICON_X 224
#define VISUALIZER_ICON_Y 144

#define VISUALIZER_FOLLOWER_X 176
#define VISUALIZER_FOLLOWER_Y 128

#define MODIFY_DIGITS_MAX      4
#define MODIFY_DIGITS_ARROW_X  129
#define MODIFY_DIGITS_ARROW1_Y 94
#define MODIFY_DIGITS_ARROW2_Y 113

#define OPTIONS_ARROW_1_X   4
#define OPTIONS_ARROW_2_X   236
#define OPTIONS_ARROW_Y     119
#define OPTIONS_ARROW_Y_MOD 13

// Sprite offset
#define MAX_Y_OFFSET 20

#define PALTAG_ARROW 0x1000

struct PokemonSpriteVisualizerModifyArrows
{
    u8 arrowSpriteId[2];
    u16 minValue;
    u16 maxValue;
    int currValue;
    u8 currentDigit;
    u8 maxDigits;
    u8 charDigits[MODIFY_DIGITS_MAX];
};

struct PokemonSpriteVisualizerOptionArrows
{
    u8 arrowSpriteId[1];
    u8 currentDigit;
};

struct PokemonShadowSettings
{
    u8 definedSize:4;
    u8 overrideSize:4;
	bool8 definedVisibility:1;
	bool8 overrideVisibility:1;
	s8 definedX;
	s8 overrideX;
};

struct PokemonSpriteVisualizerYPosModifiyArrows
{
    u8 arrowSpriteId[1];
    u8 currentDigit;
};

struct PokemonSpriteConstValues
{
    u8 backPicCoords;
    u8 frontPicCoords;
	s8 frontPicElevation;
};

struct PokemonSpriteOffsets
{
    s8 offset_back_picCoords;
    s8 offset_front_picCoords;
	u8 offset_front_elevation;
};

struct PokemonSpriteVisualizer
{
    u16 currentmonId;
    u8 currentmonWindowId;
    u8 InstructionsWindowId;
    u8 frontspriteId;
    u8 backspriteId;
    u8 iconspriteId;
    // u8 followerspriteId;

    u8 isShiny:1;
	u8 currentSubmenu:2;
	u8 submenuYpos[2];
	
	u8 battleTerrain;

    u8 frontShadowSpriteIdPrimary;
    u8 frontShadowSpriteIdSecondary;
    struct PokemonShadowSettings shadowSettings;

    struct PokemonSpriteVisualizerModifyArrows modifyArrows;
    struct PokemonSpriteVisualizerOptionArrows optionArrows;
    struct PokemonSpriteVisualizerYPosModifiyArrows yPosModifyArrows;
    struct PokemonSpriteConstValues constSpriteValues;
    struct PokemonSpriteOffsets offsetsSpriteValues;
};

static void ResetBGs_PokemonSpriteVisualizer(u16 a);
static void ResetPokemonSpriteVisualizerWindows(void);
static void Task_PokemonSpriteVisualizer_HandleInput(u8 taskId);
static void Task_ExitPokemonSpriteVisualizer(u8 taskId);
static void PrintInstructionsOnWindow(struct PokemonSpriteVisualizer *data);
static void LoadMonSprites(struct PokemonSpriteVisualizer *data, bool8 clearOldSprites);
static void ApplyOffsetSpriteValues(struct PokemonSpriteVisualizer *data, u16 species);
static void ResetOffsetSpriteValues(struct PokemonSpriteVisualizer *data);
static void ResetShadowSettings(struct PokemonSpriteVisualizer *data, u16 species);
static void UpdateShadowSettingsText(struct PokemonSpriteVisualizer *data);
static void SetConstSpriteValues(struct PokemonSpriteVisualizer *data, u16 species);
static void UpdateYPosOffsetText(struct PokemonSpriteVisualizer *data);
static void SpriteCB_Shadow(struct Sprite *sprite);
static void SetUpModifyArrows(struct PokemonSpriteVisualizer *data);
static void SetArrowInvisibility(struct PokemonSpriteVisualizer *data);
static void PrintDigitChars(struct PokemonSpriteVisualizer *data);
static bool8 TryMoveDigit(struct PokemonSpriteVisualizerModifyArrows *modArrows, bool8 moveUp);
static void PrintBattleBgName(u8 battleTerrain);
static void UpdateSubmenuOneOptionValue(struct PokemonSpriteVisualizer *data, bool8 increment);
static void UpdateSubmenuTwoOptionValue(struct PokemonSpriteVisualizer *data, bool8 increment);
static void UpdateSubmenuThreeOptionValue(struct PokemonSpriteVisualizer *data, bool8 increment);

static const u8 sArrowDown_Gfx[] = INCBIN_U8("graphics/pokemon_sprite_visualizer/arrow_down.4bpp");
static const u8 sArrowRight_Gfx[] = INCBIN_U8("graphics/pokemon_sprite_visualizer/arrow_right.4bpp");
static const u16 sArrow_Pal[] = INCBIN_U16("graphics/pokemon_sprite_visualizer/arrow.gbapal");
static const u16 sBgColor[] = {RGB_WHITE, RGB(12, 12, 12)};

static const struct SpritePalette sSpritePalette_Arrow =
{
    sArrow_Pal, PALTAG_ARROW
};

static const struct OamData sOamData_Arrow =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sAnim_Arrow_Down[] =
{
    ANIMCMD_FRAME(0, 30),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_Arrow_Up[] =
{
    ANIMCMD_FRAME(0, 30, .vFlip = TRUE),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_Arrow_Right[] =
{
    ANIMCMD_FRAME(1, 30),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnims_Arrow[] =
{
    [ARROW_DOWN]  = sAnim_Arrow_Down,
    [ARROW_UP]    = sAnim_Arrow_Up,
    [ARROW_RIGHT] = sAnim_Arrow_Right,
};

static const struct SpriteFrameImage sPicTable_Arrow[] =
{
    obj_frame_tiles(sArrowDown_Gfx),
    obj_frame_tiles(sArrowRight_Gfx)
};

static const struct SpriteTemplate sSpriteTemplate_Arrow =
{
    .tileTag = 0xFFFF,
    .paletteTag = PALTAG_ARROW,
    .oam = &sOamData_Arrow,
    .anims = sAnims_Arrow,
    .images = sPicTable_Arrow,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

// Bg templates
static const struct BgTemplate sBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 24,
        .screenSize = 2,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 28,
        .screenSize = 2,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 2,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 1,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    },
   {
        .bg = 3,
        .charBaseIndex = 2,
        .mapBaseIndex = 26,
        .screenSize = 1,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0
    },
};

// Win templates
static const struct WindowTemplate sPokemonSpriteVisualizerWindowTemplate[] =
{
    [WIN_NAME_NUMBERS] = {
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 12,
        .width = 15,
        .height = 2,
        .paletteNum = 0xF,
        .baseBlock = 1
    },
    [WIN_INSTRUCTIONS] = {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 15,
        .height = 4,
        .paletteNum = 0xF,
        .baseBlock = 1 + 30
    },
    [WIN_BOTTOM_LEFT] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 14,
        .width = 6,
        .height = 6,
        .paletteNum = 0xF,
        .baseBlock = 1 + 30 + 60
    },
    [WIN_BOTTOM_RIGHT] = {
        .bg = 0,
        .tilemapLeft = 7,
        .tilemapTop = 14,
        .width = 24,
        .height = 6,
        .paletteNum = 0xF,
        .baseBlock = 1 + 30 + 60 + 36
    },
    [WIN_FOOTPRINT] = {
        .bg = 0,
        .tilemapLeft = 27,
        .tilemapTop = 14,
        .width = 2,
        .height = 2,
        .paletteNum = 0xE,
        .baseBlock = 1 + 30 + 60 + 36 + 144,
    },
    DUMMY_WIN_TEMPLATE,
};

static const u8 sShadowSizeLabels[][3] =
{
	[SHADOW_SIZE_S]  = _("S"),
	[SHADOW_SIZE_M]  = _("M"),
	[SHADOW_SIZE_L]  = _("L"),
	[SHADOW_SIZE_XL] = _("XL"),
};

static const u8 sShadowVisibilityLabels[][10] =
{
	[FALSE] = _("False"),
	[TRUE]  = _("True"),
};

static struct PokemonSpriteVisualizer *GetStructPtr(u8 taskId)
{
	u8 *taskDataPtr = (u8*)(&gTasks[taskId].data[0]);
	return (struct PokemonSpriteVisualizer*)(READ_PTR(taskDataPtr));
}

static void SetStructPtr(u8 taskId, void *ptr)
{
	u32 structPtr = (u32)(ptr);
	u8 *taskDataPtr = (u8*)(&gTasks[taskId].data[0]);
	taskDataPtr[0] = structPtr >> 0;
	taskDataPtr[1] = structPtr >> 8;
	taskDataPtr[2] = structPtr >> 16;
	taskDataPtr[3] = structPtr >> 24;
}

static void VBlankCB_PokemonSpriteVisualizer(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void CB2_PokemonSpriteVisualizerRunner(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

void CB2_PokemonSpriteVisualizer(void)
{
	u8 taskId;
	struct PokemonSpriteVisualizer *data;
	
	switch (gMain.state)
	{
		case 0:
			SetVBlankCallback(NULL);
			FreeMonSpritesGfx();
			ResetBGs_PokemonSpriteVisualizer(0);
			DmaFillLarge16(3, 0, (u8 *)VRAM, VRAM_SIZE, 0x1000);
			DmaClear32(3, OAM, OAM_SIZE);
			DmaClear16(3, PLTT, PLTT_SIZE);
			break;
		case 1:
			ScanlineEffect_Stop();
			ResetTasks();
			ResetSpriteData();
			ResetPaletteFade();
			FreeAllSpritePalettes();
			ResetAllPicSprites();
			BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
			LoadPalette(stdpal_get(0), 0xF0, 0x40);
			
			FillBgTilemapBufferRect(0, 0, 0, 0, 32, 20, 15);
			InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
			LoadBattleTerrainGfx(0);
			break;
		case 2:
			ResetPokemonSpriteVisualizerWindows();
			break;
		case 3:
			LoadPalette(sBgColor, 0, 2);
			LoadPalette(sBgColor, 0xE0, 4); // For the footprint
			
			SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
			ShowBg(0);
			ShowBg(1);
			ShowBg(2);
			ShowBg(3);
			
			taskId = CreateTask(Task_PokemonSpriteVisualizer_HandleInput, 0);
			
			data = AllocZeroed(sizeof(struct PokemonSpriteVisualizer));
			SetStructPtr(taskId, data);
			
			data->currentmonId = SPECIES_BULBASAUR;
			LoadMonSprites(data, FALSE);
			
			SetUpModifyArrows(data);
			gSprites[data->optionArrows.arrowSpriteId[0]].invisible = TRUE;
			gSprites[data->yPosModifyArrows.arrowSpriteId[0]].invisible = TRUE;
			
			SetConstSpriteValues(data, data->currentmonId);
			UpdateYPosOffsetText(data);
			break;
		case 4:
			EnableInterrupts(INTR_FLAG_VBLANK);
			SetVBlankCallback(VBlankCB_PokemonSpriteVisualizer);
			SetMainCallback2(CB2_PokemonSpriteVisualizerRunner);
			m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 0x80);
			return;
	}
	gMain.state++;
}

static void ResetBGs_PokemonSpriteVisualizer(u16 a)
{
	if (!(a & DISPCNT_BG0_ON))
	{
		ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_BG0_ON);
		SetGpuReg(REG_OFFSET_BG0CNT, 0);
		SetGpuReg(REG_OFFSET_BG0HOFS, 0);
		SetGpuReg(REG_OFFSET_BG0VOFS, 0);
	}
	
	if (!(a & DISPCNT_BG1_ON))
	{
		ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_BG1_ON);
		SetGpuReg(REG_OFFSET_BG1CNT, 0);
		SetGpuReg(REG_OFFSET_BG1HOFS, 0);
		SetGpuReg(REG_OFFSET_BG1VOFS, 0);
	}
	
	if (!(a & DISPCNT_BG2_ON))
	{
		ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_BG2_ON);
		SetGpuReg(REG_OFFSET_BG2CNT, 0);
		SetGpuReg(REG_OFFSET_BG2HOFS, 0);
		SetGpuReg(REG_OFFSET_BG2VOFS, 0);
	}
	
	if (!(a & DISPCNT_BG3_ON))
	{
		ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_BG3_ON);
		SetGpuReg(REG_OFFSET_BG3CNT, 0);
		SetGpuReg(REG_OFFSET_BG3HOFS, 0);
		SetGpuReg(REG_OFFSET_BG3VOFS, 0);
	}
	
	if (!(a & DISPCNT_OBJ_ON))
	{
		ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON);
		ResetSpriteData();
		FreeAllSpritePalettes();
	}
}

static void ResetPokemonSpriteVisualizerWindows(void)
{
	u8 i;
	
	FreeAllWindowBuffers();
	InitWindows(sPokemonSpriteVisualizerWindowTemplate);
	
	for (i = 0; i < NUM_SPRITE_VISUALIZER_WINDOWS + 1; i++)
	{
		FillWindowPixelBuffer(i, PIXEL_FILL(0));
		PutWindowTilemap(i);
		CopyWindowToVram(i, COPYWIN_BOTH);
	}
}

#define GET_OPTION_ARROW_Y_POS(currentDigit) ((OPTIONS_ARROW_Y + currentDigit * OPTIONS_ARROW_Y_MOD))

static void Task_PokemonSpriteVisualizer_HandleInput(u8 taskId)
{
	struct PokemonSpriteVisualizer *data = GetStructPtr(taskId);
	
	if (JOY_NEW(START_BUTTON))
	{
		data->isShiny ^= TRUE;
		
		if (data->isShiny)
			PlaySE(SE_SHINY);
		
		LoadMonSprites(data, TRUE);
		ApplyOffsetSpriteValues(data, data->currentmonId);
	}
	
	if (data->currentSubmenu == 0)
	{
		if (JOY_NEW(A_BUTTON))
		{
			data->currentSubmenu = 1;
			SetArrowInvisibility(data);
			PrintInstructionsOnWindow(data);
			
			// gSprites[data->followerspriteId].invisible = TRUE;
		}
		else if (JOY_NEW(B_BUTTON))
		{
			PlaySE(SE_PC_OFF);
			BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
			gTasks[taskId].func = Task_ExitPokemonSpriteVisualizer;
		}
		else if (JOY_NEW(DPAD_DOWN))
		{
			if (TryMoveDigit(&data->modifyArrows, FALSE))
			{
				PrintDigitChars(data);
				data->currentmonId = data->modifyArrows.currValue;
				ResetShadowSettings(data, data->currentmonId);
				SetConstSpriteValues(data, data->currentmonId);
				LoadMonSprites(data, TRUE);
				ResetOffsetSpriteValues(data);
			}
			PlaySE(SE_DEX_SCROLL);
            VBlankIntrWait();
		}
		else if (JOY_NEW(DPAD_UP))
		{
			if (TryMoveDigit(&data->modifyArrows, TRUE))
			{
				PrintDigitChars(data);
				data->currentmonId = data->modifyArrows.currValue;
				ResetShadowSettings(data, data->currentmonId);
				SetConstSpriteValues(data, data->currentmonId);
				LoadMonSprites(data, TRUE);
				ResetOffsetSpriteValues(data);
			}
			PlaySE(SE_DEX_SCROLL);
		}
		else if (JOY_NEW(DPAD_LEFT))
		{
			if (data->modifyArrows.currentDigit != 0)
			{
				data->modifyArrows.currentDigit--;
                gSprites[data->modifyArrows.arrowSpriteId[0]].x2 -= 6;
                gSprites[data->modifyArrows.arrowSpriteId[1]].x2 -= 6;
			}
		}
		else if (JOY_NEW(DPAD_RIGHT))
		{
			if (data->modifyArrows.currentDigit != (data->modifyArrows.maxDigits - 1))
			{
				data->modifyArrows.currentDigit++;
                gSprites[data->modifyArrows.arrowSpriteId[0]].x2 += 6;
                gSprites[data->modifyArrows.arrowSpriteId[1]].x2 += 6;
			}
		}
	}
	else if (data->currentSubmenu == 1)
	{
		if (JOY_NEW(A_BUTTON))
		{
			data->currentSubmenu = 2;
			SetArrowInvisibility(data);
			PrintInstructionsOnWindow(data);
			UpdateShadowSettingsText(data);
		}
		else if (JOY_NEW(B_BUTTON))
		{
			data->currentSubmenu = 0;
			SetArrowInvisibility(data);
            PrintInstructionsOnWindow(data);
			
			// gSprites[data->followerspriteId].invisible = FALSE;
		}
		else if (JOY_NEW(DPAD_DOWN))
		{
			data->submenuYpos[0]++;
			if (data->submenuYpos[0] == 3)
				data->submenuYpos[0] = 0;
			
			data->optionArrows.currentDigit = data->submenuYpos[0];
            gSprites[data->optionArrows.arrowSpriteId[0]].y = GET_OPTION_ARROW_Y_POS(data->optionArrows.currentDigit);
		}
		else if (JOY_NEW(DPAD_UP))
		{
			if (data->submenuYpos[0] == 0)
				data->submenuYpos[0] = 2;
			else
				data->submenuYpos[0]--;
			
			data->optionArrows.currentDigit = data->submenuYpos[0];
            gSprites[data->optionArrows.arrowSpriteId[0]].y = GET_OPTION_ARROW_Y_POS(data->optionArrows.currentDigit);
		}
		else if (JOY_NEW(DPAD_LEFT))
			UpdateSubmenuOneOptionValue(data, FALSE);
		else if (JOY_NEW(DPAD_RIGHT))
			UpdateSubmenuOneOptionValue(data, TRUE);
	}
	else if (data->currentSubmenu == 2)
	{
		if (JOY_NEW(A_BUTTON))
		{
			data->currentSubmenu = 3;
			SetArrowInvisibility(data);
			PrintInstructionsOnWindow(data);
			PrintBattleBgName(data->battleTerrain);
		}
		else if (JOY_NEW(B_BUTTON))
		{
			data->currentSubmenu = 1;
			SetArrowInvisibility(data);
			PrintInstructionsOnWindow(data);
			SetConstSpriteValues(data, data->currentmonId);
            UpdateYPosOffsetText(data);
		}
		else if (JOY_NEW(DPAD_DOWN))
		{
			data->submenuYpos[1]++;
			if (data->submenuYpos[1] == 3)
				data->submenuYpos[1]  = 0;
			
			data->yPosModifyArrows.currentDigit = data->submenuYpos[1];
            gSprites[data->yPosModifyArrows.arrowSpriteId[0]].y = GET_OPTION_ARROW_Y_POS(data->yPosModifyArrows.currentDigit);
		}
		else if (JOY_NEW(DPAD_UP))
		{
			if (data->submenuYpos[1] == 0)
				data->submenuYpos[1] = 2;
			else
				data->submenuYpos[1]--;
			
			data->yPosModifyArrows.currentDigit = data->submenuYpos[1];
            gSprites[data->yPosModifyArrows.arrowSpriteId[0]].y = GET_OPTION_ARROW_Y_POS(data->yPosModifyArrows.currentDigit);
		}
		else if (JOY_NEW(DPAD_LEFT))
			UpdateSubmenuTwoOptionValue(data, FALSE);
		else if (JOY_NEW(DPAD_RIGHT))
			UpdateSubmenuTwoOptionValue(data, TRUE);
	}
	else
	{
		if (JOY_NEW(B_BUTTON))
		{
			data->currentSubmenu = 2;
			SetArrowInvisibility(data);
			PrintInstructionsOnWindow(data);
			UpdateShadowSettingsText(data);
		}
		else if (JOY_NEW(DPAD_LEFT))
			UpdateSubmenuThreeOptionValue(data, FALSE);
		else if (JOY_NEW(DPAD_RIGHT))
			UpdateSubmenuThreeOptionValue(data, TRUE);
	}
}

static void Task_ExitPokemonSpriteVisualizer(u8 taskId)
{
	if (!gPaletteFade.active)
	{
		Free(GetStructPtr(taskId));
		FreeMonSpritesGfx();
		DestroyTask(taskId);
		SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
		m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 0x100);
	}
}

static void PrintInstructionsOnWindow(struct PokemonSpriteVisualizer *data)
{
	// Instruction window
	FillWindowPixelBuffer(WIN_INSTRUCTIONS, PIXEL_FILL(1));
	
	switch (data->currentSubmenu)
	{
		case 0:
			AddTextPrinterParameterized(WIN_INSTRUCTIONS, 0, COMPOUND_STRING("{START_BUTTON} Shiny  {B_BUTTON} Exit\n{A_BUTTON} Sprite Coords$"), 2, 0, 0, NULL);
			break;
		case 1:
			AddTextPrinterParameterized(WIN_INSTRUCTIONS, 0, COMPOUND_STRING("{START_BUTTON} Shiny  {B_BUTTON} Back\n{A_BUTTON} Enemy shadow$"), 2, 0, 0, NULL);
			break;
		case 2:
			AddTextPrinterParameterized(WIN_INSTRUCTIONS, 0, COMPOUND_STRING("{START_BUTTON} Shiny  {B_BUTTON} Back\n{A_BUTTON} Background$"), 2, 0, 0, NULL);
			break;
		case 3:
			AddTextPrinterParameterized(WIN_INSTRUCTIONS, 0, COMPOUND_STRING("{START_BUTTON} Shiny  {B_BUTTON} Back$"), 2, 0, 0, NULL);
			break;
	}
	CopyWindowToVram(WIN_INSTRUCTIONS, COPYWIN_BOTH);
	
	// Bottom left text
	FillWindowPixelBuffer(WIN_BOTTOM_LEFT, PIXEL_FILL(0));
	
	switch (data->currentSubmenu)
	{
		case 0:
		case 1:
			AddTextPrinterParameterized(WIN_BOTTOM_LEFT, 0, COMPOUND_STRING("B coords:\nF coords:\nF elev.:$"), 0, 0, 0, NULL);
			break;
		case 2:
			AddTextPrinterParameterized(WIN_BOTTOM_LEFT, 0, COMPOUND_STRING("Shadow X:\nS size:\nInvisible:$"), 0, 0, 0, NULL);
			break;
		case 3:
			AddTextPrinterParameterized(WIN_BOTTOM_LEFT, 0, COMPOUND_STRING("BG:$"), 0, 0, 0, NULL);
			break;
	}
}

static void PrintBattleBgName(u8 battleTerrain)
{
	u8 text[27];
	FillWindowPixelBuffer(WIN_BOTTOM_RIGHT, PIXEL_FILL(0));
	StringCopyPadded(text, gBattleTerrainTable[battleTerrain].name, CHAR_SPACE, 26);
	AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, text, 0, 0, 0, NULL);
}

static u8 CalcFrontSpriteYOffset(u16 species, s8 offset_picCoords, s8 offset_elevation)
{
	u8 y = gSpeciesInfo[species].frontPicYOffset + offset_picCoords + gBattlerCoords[FALSE][B_POSITION_OPPONENT_LEFT].y;
	
	y -= gSpeciesInfo[species].frontPicElevation + offset_elevation;
	
	if (y > DISPLAY_HEIGHT - 64 + 8)
		y = DISPLAY_HEIGHT - 64 + 8;
	
	return y;
}

static void ApplyOffsetSpriteValues(struct PokemonSpriteVisualizer *data, u16 species)
{
	gSprites[data->backspriteId].y = VISUALIZER_MON_BACK_Y + gSpeciesInfo[species].backPicYOffset + data->offsetsSpriteValues.offset_back_picCoords;
	gSprites[data->frontspriteId].y = CalcFrontSpriteYOffset(species, data->offsetsSpriteValues.offset_front_picCoords, data->offsetsSpriteValues.offset_front_elevation);
}

static void ResetOffsetSpriteValues(struct PokemonSpriteVisualizer *data)
{
	data->offsetsSpriteValues.offset_back_picCoords = 0;
    data->offsetsSpriteValues.offset_front_picCoords = 0;
	data->offsetsSpriteValues.offset_front_elevation = 0;
	UpdateYPosOffsetText(data);
}

static void SetConstSpriteValues(struct PokemonSpriteVisualizer *data, u16 species)
{
    data->constSpriteValues.frontPicCoords = gSpeciesInfo[species].frontPicYOffset;
    data->constSpriteValues.backPicCoords = gSpeciesInfo[species].backPicYOffset;
	data->constSpriteValues.frontPicElevation = gSpeciesInfo[species].frontPicElevation;
}

static void UpdateYPosOffsetText(struct PokemonSpriteVisualizer *data)
{
	u8 text[34];
	u8 textConst[] = _("const val:");
    u8 textNew[] = _("new val:");
	u8 x_const_val = 50;
    u8 x_new_text = 80;
    u8 x_new_val = 120;
	u8 backPicCoords = data->constSpriteValues.backPicCoords;
    u8 frontPicCoords = data->constSpriteValues.frontPicCoords;
	s8 frontPicElevation = data->constSpriteValues.frontPicElevation;
    s8 offset_back_picCoords = data->offsetsSpriteValues.offset_back_picCoords;
    s8 offset_front_picCoords = data->offsetsSpriteValues.offset_front_picCoords;
	s8 offset_front_elevation = data->offsetsSpriteValues.offset_front_elevation;
    u8 newBackPicCoords = backPicCoords + offset_back_picCoords;
    u8 newFrontPicCoords = frontPicCoords + offset_front_picCoords;
	s8 newElevationCoords = frontPicElevation + offset_front_elevation;
	
	FillWindowPixelBuffer(WIN_BOTTOM_RIGHT, PIXEL_FILL(0));
	
	// Back
	AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, textConst, 0, 0, 0, NULL);
	ConvertIntToDecimalStringN(text, backPicCoords, STR_CONV_MODE_LEFT_ALIGN, 2);
	AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, text, x_const_val, 0, 0, NULL);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, textNew, x_new_text, 0, 0, NULL);
    ConvertIntToDecimalStringN(text, newBackPicCoords, STR_CONV_MODE_LEFT_ALIGN, 2);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, text, x_new_val, 0, 0, NULL);
	
	// Front
	AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, textConst, 0, OPTIONS_ARROW_Y_MOD, 0, NULL);
    ConvertIntToDecimalStringN(text, frontPicCoords, STR_CONV_MODE_LEFT_ALIGN, 2);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, text, x_const_val, OPTIONS_ARROW_Y_MOD, 0, NULL);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, textNew, x_new_text, OPTIONS_ARROW_Y_MOD, 0, NULL);
    ConvertIntToDecimalStringN(text, newFrontPicCoords, STR_CONV_MODE_LEFT_ALIGN, 2);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, text, x_new_val, OPTIONS_ARROW_Y_MOD, 0, NULL);
	
	// Front elevation
	AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, textConst, 0, OPTIONS_ARROW_Y_MOD * 2, 0, NULL);
    ConvertIntToDecimalStringN(text, frontPicElevation, STR_CONV_MODE_LEFT_ALIGN, 2);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, text, x_const_val, OPTIONS_ARROW_Y_MOD * 2, 0, NULL);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, textNew, x_new_text, OPTIONS_ARROW_Y_MOD * 2, 0, NULL);
    ConvertIntToDecimalStringN(text, newElevationCoords, STR_CONV_MODE_LEFT_ALIGN, 2);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, text, x_new_val, OPTIONS_ARROW_Y_MOD * 2, 0, NULL);
}

static void ResetShadowSettings(struct PokemonSpriteVisualizer *data, u16 species)
{
	data->shadowSettings.definedSize = data->shadowSettings.overrideSize = gSpeciesInfo[species].shadowSize;
	data->shadowSettings.definedX = data->shadowSettings.overrideX = gSpeciesInfo[species].shadowXOffset;
	data->shadowSettings.definedVisibility = data->shadowSettings.overrideVisibility = gSpeciesInfo[species].suppressEnemyShadow;
}

static void UpdateShadowSettingsText(struct PokemonSpriteVisualizer *data)
{
	u8 text[16];
	u8 textConst[] = _("const val:");
    u8 textNew[] = _("new val:");
    u8 x_const_val = 50;
    u8 x_new_text = 80;
    u8 x_new_val = 120;
	
	FillWindowPixelBuffer(WIN_BOTTOM_RIGHT, PIXEL_FILL(0));
	
	// X offset
	AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, textConst, 0, 0, 0, NULL);
	ConvertIntToDecimalStringN(text, data->shadowSettings.definedX, STR_CONV_MODE_LEFT_ALIGN, 2);
	AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, text, x_const_val, 0, 0, NULL);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, textNew, x_new_text, 0, 0, NULL);
    ConvertIntToDecimalStringN(text, data->shadowSettings.overrideX, STR_CONV_MODE_LEFT_ALIGN, 2);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, text, x_new_val, 0, 0, NULL);
	
	// Size
	AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, textConst, 0, OPTIONS_ARROW_Y_MOD, 0, NULL);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, sShadowSizeLabels[data->shadowSettings.definedSize], x_const_val, OPTIONS_ARROW_Y_MOD, 0, NULL);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, textNew, x_new_text, OPTIONS_ARROW_Y_MOD, 0, NULL);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, sShadowSizeLabels[data->shadowSettings.overrideSize], x_new_val, OPTIONS_ARROW_Y_MOD, 0, NULL);
	
	// Visibility
	AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, textConst, 0, OPTIONS_ARROW_Y_MOD * 2, 0, NULL);
	AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, sShadowVisibilityLabels[data->shadowSettings.definedVisibility], x_const_val, OPTIONS_ARROW_Y_MOD * 2, 0, NULL);
	AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, textNew, x_new_text, OPTIONS_ARROW_Y_MOD * 2, 0, NULL);
	AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, 0, sShadowVisibilityLabels[data->shadowSettings.overrideVisibility], x_new_val, OPTIONS_ARROW_Y_MOD * 2, 0, NULL);
}

static void SpriteCB_Shadow(struct Sprite *sprite)
{
	struct Sprite *battlerSprite = &gSprites[sprite->data[0]];
	sprite->x = battlerSprite->x + sprite->data[2] + (sprite->data[1] == 0 ? -16 : 16);
	sprite->x2 = battlerSprite->x2;
}

static void CreateEnemyShadowSpriteInternal(u8 *dest, u8 frontSpriteId, s16 x, s16 y, u8 size, s8 overrideX, u8 side, bool8 invisible)
{
	*dest = CreateSprite(&gSpriteTemplate_EnemyShadow, x, y, 0xC8);
	gSprites[*dest].data[0] = frontSpriteId;
	gSprites[*dest].data[1] = side;
	gSprites[*dest].data[2] = overrideX;
	gSprites[*dest].oam.priority = 0;
	gSprites[*dest].oam.tileNum += ((8 * size) + (4 * side));
	gSprites[*dest].callback = SpriteCB_Shadow;
	gSprites[*dest].invisible = invisible;
}

static void CreateEnemyShadow(struct PokemonSpriteVisualizer *data, u16 species)
{
	s16 x = gBattlerCoords[FALSE][B_POSITION_OPPONENT_LEFT].x + gSpeciesInfo[species].shadowXOffset, y = gBattlerCoords[FALSE][B_POSITION_OPPONENT_LEFT].y + ENEMY_SHADOW_Y_OFFSET;
	s8 overrideX = data->shadowSettings.overrideX;
	u8 size = data->shadowSettings.overrideSize;
	bool8 invisible = data->shadowSettings.overrideVisibility;
	
	LoadCompressedSpriteSheet(&gSpriteSheet_EnemyShadowsSized);
	LoadSpritePalette(&gSpritePalettes_HealthBoxHealthBar[0]);
	
	CreateEnemyShadowSpriteInternal(&data->frontShadowSpriteIdPrimary, data->frontspriteId, x, y, size, overrideX, 0, invisible);
	CreateEnemyShadowSpriteInternal(&data->frontShadowSpriteIdSecondary, data->frontspriteId, x, y, size, overrideX, 1, invisible);
}

static void LoadMonSprites(struct PokemonSpriteVisualizer *data, bool8 clearOldSprites)
{
	u16 species = data->currentmonId;
	
	if (clearOldSprites)
	{
		DestroySprite(&gSprites[data->frontspriteId]);
		DestroySprite(&gSprites[data->backspriteId]);
		DestroySprite(&gSprites[data->iconspriteId]);
		// DestroySprite(&gSprites[data->followerspriteId]);
		
		DestroySprite(&gSprites[data->frontShadowSpriteIdPrimary]);
		DestroySprite(&gSprites[data->frontShadowSpriteIdSecondary]);
		
		FreeMonSpritesGfx();
		ResetSpriteData();
		ResetPaletteFade();
		FreeAllSpritePalettes();
		ResetAllPicSprites();
		FreeMonIconPalettes();
	}
	AllocateMonSpritesGfx();
	
	PrintInstructionsOnWindow(data);
	
	// Palettes
	LoadMonIconPalette(species);
	LoadMonPaletteFromSpecies(species, data->isShiny);
	
	// Front
	LoadSpecialPokePic(species, 0xFE, TRUE, gMonSpritesGfxPtr->sprites[B_POSITION_OPPONENT_LEFT]);
	SetMultiuseSpriteTemplateToPokemon(species, B_POSITION_OPPONENT_LEFT);
	gMultiuseSpriteTemplate.paletteTag = species;
	data->frontspriteId = CreateSprite(&gMultiuseSpriteTemplate, gBattlerCoords[FALSE][B_POSITION_OPPONENT_LEFT].x, CalcFrontSpriteYOffset(species, 0, 0), 0);
	gSprites[data->frontspriteId].oam.paletteNum = 1;
    gSprites[data->frontspriteId].callback = SpriteCallbackDummy;
    gSprites[data->frontspriteId].oam.priority = 0;
	CreateEnemyShadow(data, species);
	
	// Back
	LoadSpecialPokePic(species, 0xFE, FALSE, gMonSpritesGfxPtr->sprites[B_POSITION_PLAYER_RIGHT]);
	SetMultiuseSpriteTemplateToPokemon(species, B_POSITION_PLAYER_RIGHT);
	data->backspriteId = CreateSprite(&gMultiuseSpriteTemplate, VISUALIZER_MON_BACK_X, VISUALIZER_MON_BACK_Y + gSpeciesInfo[species].backPicYOffset, 0);
	gSprites[data->backspriteId].oam.paletteNum = 1;
    gSprites[data->backspriteId].callback = SpriteCallbackDummy;
    gSprites[data->backspriteId].oam.priority = 0;
	
	// Icon
	data->iconspriteId = CreateMonIcon(species, SpriteCB_MonIcon, VISUALIZER_ICON_X, VISUALIZER_ICON_Y, 4);
	gSprites[data->iconspriteId].oam.priority = 0;
	
	// Follower Sprite
    // data->followerspriteId = CreateObjectGraphicsSprite(OBJ_EVENT_GFX_MON_BASE + species, SpriteCB_Follower, VISUALIZER_FOLLOWER_X, VISUALIZER_FOLLOWER_Y, 0);
    // gSprites[data->followerspriteId].oam.priority = 0;
    // gSprites[data->followerspriteId].anims = sAnims_Follower;
	
	// Modify Arrows
	LoadSpritePalette(&sSpritePalette_Arrow);
	data->modifyArrows.arrowSpriteId[0] = CreateSprite(&sSpriteTemplate_Arrow, MODIFY_DIGITS_ARROW_X + (data->modifyArrows.currentDigit * 6), MODIFY_DIGITS_ARROW1_Y, 0);
    data->modifyArrows.arrowSpriteId[1] = CreateSprite(&sSpriteTemplate_Arrow, MODIFY_DIGITS_ARROW_X + (data->modifyArrows.currentDigit * 6), MODIFY_DIGITS_ARROW2_Y, 0);
    gSprites[data->modifyArrows.arrowSpriteId[1]].animNum = 1;
	
	// Option Arrow
	LoadSpritePalette(&sSpritePalette_Arrow);
	data->optionArrows.arrowSpriteId[0] = CreateSprite(&sSpriteTemplate_Arrow, OPTIONS_ARROW_1_X, GET_OPTION_ARROW_Y_POS(data->optionArrows.currentDigit), 0);
	gSprites[data->optionArrows.arrowSpriteId[0]].animNum = 2;
	
	// Modify Y Pos Arrow
	LoadSpritePalette(&sSpritePalette_Arrow);
	data->yPosModifyArrows.arrowSpriteId[0] = CreateSprite(&sSpriteTemplate_Arrow, OPTIONS_ARROW_1_X, GET_OPTION_ARROW_Y_POS(data->yPosModifyArrows.currentDigit), 0);
	gSprites[data->yPosModifyArrows.arrowSpriteId[0]].animNum = 2;
	
	if (clearOldSprites)
		SetArrowInvisibility(data);
	
	// Footprint
	FillWindowPixelBuffer(WIN_FOOTPRINT, PIXEL_FILL(0));
	DrawSpeciesFootprint(WIN_FOOTPRINT, species, 0, 0);
	CopyWindowToVram(WIN_FOOTPRINT, COPYWIN_GFX);
}

static void ValueToCharDigits(u8 *charDigits, u32 newValue, u8 maxDigits)
{
    u8 valueDigits[MODIFY_DIGITS_MAX];
    u8 i, id = 0;

    if (maxDigits >= MODIFY_DIGITS_MAX)
        valueDigits[id++] = newValue / 1000;
    if (maxDigits >= MODIFY_DIGITS_MAX - 1)
        valueDigits[id++] = (newValue % 1000) / 100;
    if (maxDigits >= MODIFY_DIGITS_MAX - 2)
        valueDigits[id++] = (newValue % 100) / 10;
    if (maxDigits >= MODIFY_DIGITS_MAX - 3)
        valueDigits[id++] = newValue % 10;

    for (i = 0; i < MODIFY_DIGITS_MAX; i++)
        charDigits[i] = valueDigits[i] + CHAR_0;
}

static u32 CharDigitsToValue(u8 *charDigits, u8 maxDigits)
{
	u8 valueDigits[MODIFY_DIGITS_MAX];
    u8 i, id = 0;
    u32 newValue = 0;

    for (i = 0; i < MODIFY_DIGITS_MAX; i++)
        valueDigits[i] = charDigits[i] - CHAR_0;

    if (maxDigits >= MODIFY_DIGITS_MAX)
        newValue += valueDigits[id++] * 1000;
    if (maxDigits >= MODIFY_DIGITS_MAX - 1)
        newValue += valueDigits[id++] * 100;
    if (maxDigits >= MODIFY_DIGITS_MAX - 2)
        newValue += valueDigits[id++] * 10;
    if (maxDigits >= MODIFY_DIGITS_MAX - 3)
        newValue += valueDigits[id++];

    return newValue;
}

static void PrintDigitChars(struct PokemonSpriteVisualizer *data)
{
	u8 i;
    u16 species = data->modifyArrows.currValue;
    u8 text[MODIFY_DIGITS_MAX + POKEMON_NAME_LENGTH + 8];

    for (i = 0; i < data->modifyArrows.maxDigits; i++)
        text[i] = data->modifyArrows.charDigits[i];

    text[i++] = CHAR_SPACE;
    text[i++] = CHAR_HYPHEN;
    text[i++] = CHAR_SPACE;
    GetSpeciesName(&text[i], species);

    FillWindowPixelBuffer(WIN_NAME_NUMBERS, PIXEL_FILL(1));
    AddTextPrinterParameterized(WIN_NAME_NUMBERS, 2, text, 6, 0, 0, NULL);
}

static void SetUpModifyArrows(struct PokemonSpriteVisualizer *data)
{
	data->modifyArrows.minValue = data->currentmonId;
    data->modifyArrows.maxValue = NUM_SPECIES - 1;
    data->modifyArrows.maxDigits = 4;
    data->modifyArrows.currValue = data->currentmonId;
	
	ValueToCharDigits(data->modifyArrows.charDigits, data->modifyArrows.currValue, data->modifyArrows.maxDigits);
	PrintDigitChars(data);
}

static bool8 TryMoveDigit(struct PokemonSpriteVisualizerModifyArrows *modArrows, bool8 moveUp)
{
	s32 i;
    u8 charDigits[MODIFY_DIGITS_MAX];
    u32 newValue;

    for (i = 0; i < MODIFY_DIGITS_MAX; i++)
        charDigits[i] = modArrows->charDigits[i];

    if (moveUp)
    {
        if (charDigits[modArrows->currentDigit] == CHAR_9)
        {
            charDigits[modArrows->currentDigit] = CHAR_0;
			
            for (i = modArrows->currentDigit - 1; i >= 0; i--)
            {
                if (charDigits[i] == CHAR_9)
                    charDigits[i] = CHAR_0;
                else
                {
                    charDigits[i]++;
                    break;
                }
            }
        }
        else
            charDigits[modArrows->currentDigit]++;
    }
    else
    {
        if (charDigits[modArrows->currentDigit] == CHAR_0)
        {
            charDigits[modArrows->currentDigit] = CHAR_9;

            for (i = modArrows->currentDigit - 1; i >= 0; i--)
            {
                if (charDigits[i] == CHAR_0)
                    charDigits[i] = CHAR_9;
                else
                {
                    charDigits[i]--;
                    break;
                }
            }
        }
        else
            charDigits[modArrows->currentDigit]--;
    }
    newValue = CharDigitsToValue(charDigits, modArrows->maxDigits);
	
    if (newValue > modArrows->maxValue || newValue < modArrows->minValue)
        return FALSE;
    else
    {
        modArrows->currValue = newValue;
		
        for (i = 0; i < MODIFY_DIGITS_MAX; i++)
             modArrows->charDigits[i] = charDigits[i];
		 
        return TRUE;
    }
}

static void SetArrowInvisibility(struct PokemonSpriteVisualizer *data)
{
	switch (data->currentSubmenu)
	{
		case 0:
			gSprites[data->modifyArrows.arrowSpriteId[0]].invisible = FALSE;
			gSprites[data->modifyArrows.arrowSpriteId[1]].invisible = FALSE;
			gSprites[data->optionArrows.arrowSpriteId[0]].invisible = TRUE;
			gSprites[data->yPosModifyArrows.arrowSpriteId[0]].invisible = TRUE;
			break;
		case 1:
			gSprites[data->modifyArrows.arrowSpriteId[0]].invisible = TRUE;
			gSprites[data->modifyArrows.arrowSpriteId[1]].invisible = TRUE;
			gSprites[data->optionArrows.arrowSpriteId[0]].invisible = FALSE;
			gSprites[data->yPosModifyArrows.arrowSpriteId[0]].invisible = TRUE;
			break;
		case 2:
			gSprites[data->modifyArrows.arrowSpriteId[0]].invisible = TRUE;
			gSprites[data->modifyArrows.arrowSpriteId[1]].invisible = TRUE;
			gSprites[data->optionArrows.arrowSpriteId[0]].invisible = TRUE;
			gSprites[data->yPosModifyArrows.arrowSpriteId[0]].invisible = FALSE;
			break;
		case 3:
			gSprites[data->modifyArrows.arrowSpriteId[0]].invisible = TRUE;
			gSprites[data->modifyArrows.arrowSpriteId[1]].invisible = TRUE;
			gSprites[data->optionArrows.arrowSpriteId[0]].invisible = TRUE;
			gSprites[data->yPosModifyArrows.arrowSpriteId[0]].invisible = TRUE;
			break;
	}
}

static void UpdateSubmenuOneOptionValue(struct PokemonSpriteVisualizer *data, bool8 increment)
{
	s8 offset;
	
	switch (data->submenuYpos[0])
	{
		case 0: // Back pic coords
			offset = data->offsetsSpriteValues.offset_back_picCoords;
			
			if (increment)
			{
				if (offset == MAX_Y_OFFSET)
					offset = -data->constSpriteValues.backPicCoords;
				else
					offset++;
			}
			else
			{
				if (offset == -data->constSpriteValues.backPicCoords)
					offset = MAX_Y_OFFSET;
				else
					offset--;
			}
			data->offsetsSpriteValues.offset_back_picCoords = offset;
			gSprites[data->backspriteId].y = VISUALIZER_MON_BACK_Y + gSpeciesInfo[data->currentmonId].backPicYOffset + offset;
			break;
		case 1: // Front pic coords
			offset = data->offsetsSpriteValues.offset_front_picCoords;
			
			if (increment)
			{
				if (offset == MAX_Y_OFFSET)
					offset = -data->constSpriteValues.frontPicCoords;
				else
					offset++;
			}
			else
			{
				if (offset == -data->constSpriteValues.frontPicCoords)
					offset = MAX_Y_OFFSET;
				else
					offset--;
			}
			data->offsetsSpriteValues.offset_front_picCoords = offset;
			gSprites[data->frontspriteId].y = CalcFrontSpriteYOffset(data->currentmonId, offset, data->offsetsSpriteValues.offset_front_elevation);
			break;
		case 2: // Front elevation
			offset = data->offsetsSpriteValues.offset_front_elevation;
			
			if (increment)
			{
				if (offset == MAX_Y_OFFSET)
					offset = -MAX_Y_OFFSET;
				else
					offset++;
			}
			else
			{
				if (offset == -MAX_Y_OFFSET)
					offset = MAX_Y_OFFSET;
				else
					offset--;
			}
			data->offsetsSpriteValues.offset_front_elevation = offset;
			gSprites[data->frontspriteId].y = CalcFrontSpriteYOffset(data->currentmonId, data->offsetsSpriteValues.offset_front_picCoords, offset);
			break;
	}
	UpdateYPosOffsetText(data);
}

static void UpdateSubmenuTwoOptionValue(struct PokemonSpriteVisualizer *data, bool8 increment)
{
	struct Sprite *leftSprite = &gSprites[data->frontShadowSpriteIdPrimary];
	struct Sprite *rightSprite = &gSprites[data->frontShadowSpriteIdSecondary];
	s8 update;
	
	switch (data->submenuYpos[1])
	{
		case 0: // Shadow X offset
			if (increment)
				data->shadowSettings.overrideX++;
			else
				data->shadowSettings.overrideX--;
			
			if (data->shadowSettings.overrideX > 20)
				data->shadowSettings.overrideX = -20;
			else if (data->shadowSettings.overrideX < -20)
				data->shadowSettings.overrideX = 20;
			
			leftSprite->data[2] = data->shadowSettings.overrideX;
			rightSprite->data[2] = data->shadowSettings.overrideX;
			break;
		case 1: // Shadow size
			if (increment)
			{
				if (data->shadowSettings.overrideSize == ARRAY_COUNT(sShadowSizeLabels) - 1)
				{
					update = -data->shadowSettings.overrideSize;
					data->shadowSettings.overrideSize = 0;
				}
				else
				{
					update = 1;
					data->shadowSettings.overrideSize++;
				}
			}
			else
			{
				if (data->shadowSettings.overrideSize == 0)
					data->shadowSettings.overrideSize = update = ARRAY_COUNT(sShadowSizeLabels) - 1;
				else
				{
					update = -1;
					data->shadowSettings.overrideSize--;
				}
			}
			leftSprite->oam.tileNum += (8 * update);
			rightSprite->oam.tileNum += (8 * update);
			break;
		case 2: // Shadow visibility
			data->shadowSettings.overrideVisibility ^= TRUE;
			leftSprite->invisible ^= TRUE;
			rightSprite->invisible ^= TRUE;
			break;
	}
	UpdateShadowSettingsText(data);
}

static void UpdateSubmenuThreeOptionValue(struct PokemonSpriteVisualizer *data, bool8 increment)
{
	if (increment)
	{
		data->battleTerrain++;
		if (data->battleTerrain >= BATTLE_TERRAINS_COUNT)
			data->battleTerrain = 0;
	}
	else
	{
		if (data->battleTerrain == 0)
			data->battleTerrain = BATTLE_TERRAINS_COUNT - 1;
		else
			data->battleTerrain--;
	}
	LoadBattleTerrainGfx(data->battleTerrain);
	PrintBattleBgName(data->battleTerrain);
}

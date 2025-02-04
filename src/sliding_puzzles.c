// Credits: Spherical Ice
#include "decompress.h"
#include "event_data.h"
#include "gflib.h"
#include "menu_helpers.h"
#include "new_menu_helpers.h"
#include "overworld.h"
#include "scanline_effect.h"
#include "task.h"
#include "text_window.h"
#include "constants/sliding_puzzles.h"
#include "constants/songs.h"

#define TAG_CURSOR   244
#define GFXTAG_TILES 245

#define NUM_COLS 6
#define NUM_ROWS 4

#define FIRST_COL 0
#define FINAL_COL (NUM_COLS - 1)
#define FIRST_ROW 0
#define FINAL_ROW (NUM_ROWS - 1)

struct SlidingPuzzle
{
    u8 tiles[NUM_ROWS][NUM_COLS];
    u8 puzzleId;
    u8 cursorSpriteId;
    u8 heldTile;
    bool8 solved;
};

enum
{
    INSTRUCTION_NO_SELECTION,
    INSTRUCTION_PICK_UP,
    INSTRUCTION_PLACE,
    INSTRUCTION_SWAP,
    INSTRUCTION_ROTATE,
    INSTRUCTION_CONTINUE,
};

enum
{
    ROTATE_NONE,
    ROTATE_ANTICLOCKWISE,
    ROTATE_CLOCKWISE,
};

enum
{
    ORIENTATION_0,
    ORIENTATION_90,
    ORIENTATION_180,
    ORIENTATION_270,
    ORIENTATION_MAX,
};

#define IMMOVABLE_TILE ORIENTATION_MAX

static const u32 sSlidingPuzzle_Gfx[] = INCBIN_U32("graphics/sliding_puzzle/bg.4bpp.lz");
static const u32 sSlidingPuzzle_Tilemap[] = INCBIN_U32("graphics/sliding_puzzle/tilemap.bin.lz");
static const u16 sSlidingPuzzle_Pal[] = INCBIN_U16("graphics/sliding_puzzle/bg.gbapal");

static const u32 sCursor_Gfx[] = INCBIN_U32("graphics/sliding_puzzle/cursor.4bpp.lz");
static const u16 sCursor_Pal[] = INCBIN_U16("graphics/sliding_puzzle/cursor.gbapal");

static void CB2_LoadSlidingPuzzle(void);
static void DrawInstructionsBar(u8 msgId);
static void CreateCursorSprite(void);
static void CreateTileSprites(void);
static void SpriteCB_Cursor(struct Sprite *sprite);
static void MoveCursor_SlidingPuzzle(s8 deltaX, s8 deltaY);
static bool8 CursorIsOnTile(void);
static bool8 CursorIsOnImmovableTile(void);
static void SelectTile(void);
static void PlaceTile(void);
static void RotateTile(u8 rotDir);
static void Task_SlidingPuzzle_WaitFadeIn(u8 taskId);
static void Task_SlidingPuzzle_HandleInput(u8 taskId);
static void Task_SlidingPuzzle_Glow(u8 taskId);
static void Task_SlidingPuzzle_Solved(u8 taskId);
static void CheckForSolution(void);
static void ExitSlidingPuzzle(u8 taskId);

static EWRAM_DATA struct SlidingPuzzle *sSlidingPuzzle = NULL;

#include "data/sliding_puzzles.h"

static const u8 sTextColor_Instructions[3] = { TEXT_DYNAMIC_COLOR_6, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY };

static const u8 *const sInstructions[] =
{
    [INSTRUCTION_NO_SELECTION] = COMPOUND_STRING("{DPAD_ANY}Move {B_BUTTON}Quit"),
    [INSTRUCTION_PICK_UP]      = COMPOUND_STRING("{DPAD_ANY}Move {A_BUTTON}Pick Up {B_BUTTON}Quit"),
    [INSTRUCTION_PLACE]        = COMPOUND_STRING("{DPAD_ANY}Move {A_BUTTON}Place {L_BUTTON}{R_BUTTON}Rotate {B_BUTTON}Quit"),
    [INSTRUCTION_SWAP]         = COMPOUND_STRING("{DPAD_ANY}Move {A_BUTTON}Swap {L_BUTTON}{R_BUTTON}Rotate {B_BUTTON}Quit"),
    [INSTRUCTION_ROTATE]       = COMPOUND_STRING("{DPAD_ANY}Move {L_BUTTON}{R_BUTTON}Rotate {B_BUTTON}Quit"),
    [INSTRUCTION_CONTINUE]     = COMPOUND_STRING("{A_BUTTON}{B_BUTTON}Continue"),
};

static const u8 sColumnXCoords[NUM_COLS] = { 32, 72, 104, 136, 168, 208 };
static const u8 sRowYCoords[NUM_ROWS] = { 40, 72, 104, 136 };

static const struct SpritePalette sSpritePalette_Cursor[] =
{
    {
        .data = sCursor_Pal,
        .tag = TAG_CURSOR
    },
};

static const struct CompressedSpriteSheet sSpriteSheet_Cursor =
{
    .data = sCursor_Gfx,
    .size = 0x400,
    .tag = TAG_CURSOR,
};

static const struct BgTemplate sBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 1,
        .mapBaseIndex = 29,
        .priority = 3,
    },
    {
        .bg = 1,
        .charBaseIndex = 0,
        .mapBaseIndex = 7,
        .priority = 3,
    },
};

static const struct WindowTemplate sWindowTemplates[] =
{
    {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 30,
        .height = 2,
        .paletteNum = 13,
        .baseBlock = 1,
    },
    DUMMY_WIN_TEMPLATE,
};

void DoSlidingPuzzle(void)
{
    SetMainCallback2(CB2_LoadSlidingPuzzle);
    gMain.savedCallback = CB2_ReturnToFieldContinueScriptPlayMapMusic;
}

static void VBlankCB_SlidingPuzzle(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void CB2_SlidingPuzzle(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void CB2_LoadSlidingPuzzle(void)
{
	switch (gMain.state)
	{
		case 0:
			SetVBlankCallback(NULL);
			ResetVramOamAndBgCntRegs();
			break;
		case 1:
			ScanlineEffect_Stop();
			ResetTasks();
			ResetSpriteData();
			ResetPaletteFade();
			FreeAllSpritePalettes();
			break;
		case 2:
			ResetBgsAndClearDma3BusyFlags(FALSE);
			InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
			InitWindows(sWindowTemplates);
			SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_BG_ALL_ON | DISPCNT_OBJ_1D_MAP);
			
			ShowBg(0);
			ShowBg(1);
			
			SetGpuReg(REG_OFFSET_BLDCNT, 0);
			SetGpuReg(REG_OFFSET_BLDALPHA, 0);
			SetGpuReg(REG_OFFSET_BLDY, 0);
			break;
		case 3:
			LZDecompressVram(sSlidingPuzzle_Gfx, (void *)VRAM);
			LZDecompressVram(sSlidingPuzzle_Tilemap, (u16 *)BG_SCREEN_ADDR(7));
			LoadPalette(sSlidingPuzzle_Pal, 0, 32);
			LoadPalette(stdpal_get(2), 0xD0, 32);
			
			LoadCompressedSpriteSheet(&sSpriteSheet_Cursor);
			LoadSpritePalettes(sSpritePalette_Cursor);
			break;
		case 4:
			sSlidingPuzzle = AllocZeroed(sizeof(struct SlidingPuzzle));
			sSlidingPuzzle->puzzleId = gSpecialVar_0x8004;
			sSlidingPuzzle->solved = gSpecialVar_0x8005;
			
			DrawInstructionsBar(sSlidingPuzzle->solved ? INSTRUCTION_CONTINUE : INSTRUCTION_NO_SELECTION);
			LoadCompressedSpriteSheet(&sSpriteSheet_Tiles[sSlidingPuzzle->puzzleId]);
			
			CreateCursorSprite();
			CreateTileSprites();
			break;
		case 5:
			BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
			EnableInterrupts(INTR_FLAG_VBLANK);
			SetVBlankCallback(VBlankCB_SlidingPuzzle);
			SetMainCallback2(CB2_SlidingPuzzle);
			CreateTask(Task_SlidingPuzzle_WaitFadeIn, 0);
			return;
	}
	gMain.state++;
}

// Sprite defines
#define sTimer       data[0]
#define sAnimating   data[1]
#define sRow         data[2]
#define sCol         data[3]
#define sTileId      data[4]
#define sOrientation data[5]

// Task defines
#define tState data[0]
#define tTimer data[1]
#define tGlow  data[2]

static void CreateCursorSprite(void)
{
	u8 spriteId = 0xFF;

    if (!sSlidingPuzzle->solved)
    {
        spriteId = CreateSprite(&sSpriteTemplate_Cursor, sColumnXCoords[0], sRowYCoords[0], 1);
        gSprites[spriteId].sTimer = 0;
        gSprites[spriteId].sAnimating = TRUE;
        gSprites[spriteId].sRow = 0;
        gSprites[spriteId].sCol = 0;
    }
    sSlidingPuzzle->cursorSpriteId = spriteId;
}

static void SpriteCB_Cursor(struct Sprite *sprite)
{
    if (++sprite->sTimer % 16 == 0)
    {
        sprite->sTimer = 0;
		
        if (sprite->sAnimating)
            sprite->invisible ^= TRUE;
        else
            sprite->invisible = FALSE;
    }
}

static void CreateTileSprites(void)
{
	u8 row, col, puzzleId = sSlidingPuzzle->solved ? PUZZLE_SOLVED : sSlidingPuzzle->puzzleId;
	
    for (row = 0; row < NUM_ROWS; ++row)
    {
        for (col = 0; col < NUM_COLS; ++col)
        {
            u8 tile = sPuzzleLayouts[puzzleId][row][col];
			
            if (tile)
            {
                u8 spriteId = CreateSprite(&sSpriteTemplate_Tiles, sColumnXCoords[col], sRowYCoords[row], 2);
                struct Sprite *sprite = &gSprites[spriteId];
				
                sprite->sAnimating = FALSE;
                sprite->sRow = row;
                sprite->sCol = col;
                sprite->sTileId = tile;
                sprite->sOrientation = sTileOrientations[puzzleId][row][col];
                if (sprite->sOrientation >= ORIENTATION_MAX)
                    sprite->sOrientation = ORIENTATION_0;
				
                StartSpriteAnim(sprite, tile - 1);
                StartSpriteAffineAnim(sprite, sprite->sOrientation);
				
                tile = spriteId;
            }
            sSlidingPuzzle->tiles[row][col] = tile;
        }
    }
}

static void CheckForSolution(void)
{
	u8 row, col;
	
    for (row = 0; row < NUM_ROWS; row++)
    {
        for (col = 0; col < NUM_COLS; col++)
        {
            struct Sprite *tile = &gSprites[sSlidingPuzzle->tiles[row][col]];

            if (tile->sTileId != sPuzzleLayouts[PUZZLE_SOLVED][row][col] || tile->sOrientation != ORIENTATION_0)
            {
                sSlidingPuzzle->solved = FALSE;
                return;
            }
        }
    }
    sSlidingPuzzle->solved = TRUE;
}

static void DrawInstructionsBar(u8 msgId)
{
	FillWindowPixelBuffer(0, PIXEL_FILL(15));
    AddTextPrinterParameterized3(0, 0, 2, 1, sTextColor_Instructions, 0, sInstructions[msgId]);
    PutWindowTilemap(0);
    CopyWindowToVram(0, COPYWIN_BOTH);
    ScheduleBgCopyTilemapToVram(0);
}

static void Task_SlidingPuzzle_WaitFadeIn(u8 taskId)
{
	if (!gPaletteFade.active)
		gTasks[taskId].func = sSlidingPuzzle->solved ? Task_SlidingPuzzle_Solved : Task_SlidingPuzzle_HandleInput;
}

static void Task_SlidingPuzzle_HandleInput(u8 taskId)
{
	if (sSlidingPuzzle->solved)
		gTasks[taskId].func = Task_SlidingPuzzle_Glow;
	else
	{
		// Prevent input while the tile is rotating
		if (sSlidingPuzzle->heldTile)
		{
			struct Sprite *sprite = &gSprites[sSlidingPuzzle->heldTile];
			
			if (sprite->sAnimating && !sprite->affineAnimEnded)
				return;
		}
		
		if (JOY_NEW(B_BUTTON))
			ExitSlidingPuzzle(taskId);
		else if (JOY_NEW(DPAD_LEFT))
			MoveCursor_SlidingPuzzle(-1, 0);
		else if (JOY_NEW(DPAD_RIGHT))
			MoveCursor_SlidingPuzzle(1, 0);
		else if (JOY_NEW(DPAD_UP))
			MoveCursor_SlidingPuzzle(0, -1);
		else if (JOY_NEW(DPAD_DOWN))
			MoveCursor_SlidingPuzzle(0, 1);
		
		if (!sSlidingPuzzle->heldTile)
		{
			// Not holding a tile
			if (CursorIsOnTile() && !CursorIsOnImmovableTile())
			{
				DrawInstructionsBar(INSTRUCTION_PICK_UP);
				
				if (JOY_NEW(A_BUTTON))
					SelectTile();
			}
			else
				DrawInstructionsBar(INSTRUCTION_NO_SELECTION);
		}
		else
		{
			// Currently holding a tile
			if (CursorIsOnTile() && !CursorIsOnImmovableTile())
			{
				DrawInstructionsBar(INSTRUCTION_SWAP);
				
				if (JOY_NEW(A_BUTTON))
					SelectTile();
			}
			else if (CursorIsOnImmovableTile())
				DrawInstructionsBar(INSTRUCTION_ROTATE);
			else
			{
				DrawInstructionsBar(INSTRUCTION_PLACE);
				
				if (JOY_NEW(A_BUTTON))
					PlaceTile();
			}
	
			if (JOY_NEW(L_BUTTON))
				RotateTile(ROTATE_ANTICLOCKWISE);
			else if (JOY_NEW(R_BUTTON))
				RotateTile(ROTATE_CLOCKWISE);
		}
	}
}

static void Task_SlidingPuzzle_Glow(u8 taskId)
{
	u16 color;
    s16* data = gTasks[taskId].data;
	
    switch (tState)
    {
		case 0:
			DrawInstructionsBar(INSTRUCTION_CONTINUE);
			tState++;
			break;
		case 1:
			DestroySprite(&gSprites[sSlidingPuzzle->cursorSpriteId]);
			PlayFanfare(MUS_LEVEL_UP);
			tGlow = 22;
			tState++;
			break;
		case 2:
			if ((++tTimer % 4) == 0)
			{
				if (++tGlow == 31)
					tState++;
				
				color = RGB(tGlow, tGlow, tGlow);
				LoadPalette(&color, 0x103, sizeof(color));
			}
			break;
		case 3:
			if ((++tTimer % 4) == 0)
			{
				if (--tGlow > 22)
					color = RGB(tGlow, tGlow, tGlow);
				else
				{
					color = RGB(28, 22, 13);
					tState++;
				}
				LoadPalette(&color, 0x103, sizeof(color));
			}
			break;
		default:
			if (IsFanfareTaskInactive())
				gTasks[taskId].func = Task_SlidingPuzzle_Solved;
			break;
    }
}

static void Task_SlidingPuzzle_Solved(u8 taskId)
{
	if (JOY_NEW(A_BUTTON | B_BUTTON))
		ExitSlidingPuzzle(taskId);
}

static void Task_SlidingPuzzle_Exit(u8 taskId)
{
	if (!gPaletteFade.active)
    {
        gSpecialVar_Result = sSlidingPuzzle->solved;
        Free(sSlidingPuzzle);
        FreeAllWindowBuffers();
        SetMainCallback2(gMain.savedCallback);
    }
}

static void ExitSlidingPuzzle(u8 taskId)
{
	BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_SlidingPuzzle_Exit;
}

static void MoveCursorSprite_SlidingPuzzle(s8 deltaX, s8 deltaY, u8 spriteId)
{
	struct Sprite *sprite = &gSprites[spriteId];
    s8 row = sprite->sRow + deltaY;
    s8 col = sprite->sCol + deltaX;

    if (row < FIRST_ROW)
        row = FINAL_ROW;
    else if (row > FINAL_ROW)
        row = FIRST_ROW;

    if (col < FIRST_COL)
        col = FINAL_COL;
    else if (col > FINAL_COL)
        col = FIRST_COL;

    sprite->sCol = col;
    sprite->sRow = row;
    sprite->x = sColumnXCoords[sprite->sCol];
    sprite->y = sRowYCoords[sprite->sRow];

    sprite->invisible = FALSE;
    sprite->sTimer = 0;
}

static void MoveCursor_SlidingPuzzle(s8 deltaX, s8 deltaY)
{
	MoveCursorSprite_SlidingPuzzle(deltaX, deltaY, sSlidingPuzzle->cursorSpriteId);

    if (sSlidingPuzzle->heldTile)
    {
        MoveCursorSprite_SlidingPuzzle(deltaX, deltaY, sSlidingPuzzle->heldTile);
        PlaySE(SE_BALL_TRAY_ENTER);
    }
}

static bool8 CursorIsOnTile(void)
{
    struct Sprite *cursor = &gSprites[sSlidingPuzzle->cursorSpriteId];

    if (sSlidingPuzzle->tiles[cursor->sRow][cursor->sCol])
        return TRUE;

    return FALSE;
}

static bool8 CursorIsOnImmovableTile(void)
{
	struct Sprite *cursor = &gSprites[sSlidingPuzzle->cursorSpriteId];

    if (sTileOrientations[sSlidingPuzzle->puzzleId][cursor->sRow][cursor->sCol] == IMMOVABLE_TILE)
        return TRUE;

    return FALSE;
}

static void SelectTile(void)
{
	struct Sprite *cursor = &gSprites[sSlidingPuzzle->cursorSpriteId];
    u8 tile = sSlidingPuzzle->heldTile;
	
    if (tile)
        gSprites[tile].subpriority = 2;

    sSlidingPuzzle->heldTile = sSlidingPuzzle->tiles[cursor->sRow][cursor->sCol];
    gSprites[sSlidingPuzzle->heldTile].subpriority = 0;

    sSlidingPuzzle->tiles[cursor->sRow][cursor->sCol] = tile;

    cursor->sAnimating = FALSE;
    PlaySE(SE_SELECT);
    CheckForSolution();
}

static void PlaceTile(void)
{
	struct Sprite *cursor = &gSprites[sSlidingPuzzle->cursorSpriteId];

    sSlidingPuzzle->tiles[cursor->sRow][cursor->sCol] = sSlidingPuzzle->heldTile;
    gSprites[sSlidingPuzzle->heldTile].subpriority = 2;

    sSlidingPuzzle->heldTile = 0;

    cursor->sAnimating = TRUE;
    PlaySE(SE_SELECT);
    CheckForSolution();
}

static void RotateTile(u8 rotDir)
{
	struct Sprite *sprite = &gSprites[sSlidingPuzzle->heldTile];
    u8 affineAnimation;

    if (rotDir == ROTATE_ANTICLOCKWISE)
    {
        affineAnimation = sprite->sOrientation + 4;
		
        if (sprite->sOrientation)
            sprite->sOrientation--;
        else
            sprite->sOrientation = ORIENTATION_270;
    }
    else if (rotDir == ROTATE_CLOCKWISE)
    {
        affineAnimation = sprite->sOrientation + 8;
        sprite->sOrientation++;
        sprite->sOrientation = sprite->sOrientation % ORIENTATION_MAX;
    }
    StartSpriteAffineAnim(sprite, affineAnimation);
	sprite->sAnimating = TRUE;
}

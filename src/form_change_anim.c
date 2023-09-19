#include "global.h"
#include "gflib.h"
#include "form_change.h"
#include "party_menu.h"
#include "sound.h"
#include "task.h"
#include "constants/songs.h"

#define NUM_FORM_CHANGE_ANIM_SPRITES 10

#define FORM_CHANGE_ANIM_TAG 0x57D0

struct FormChangeAnimData
{
	struct Sprite *icon1; // The icon sprite of the pokemon
	struct Sprite *icon2; // Same as above, but for the second mon when fusing
	struct Sprite *sprites1[NUM_FORM_CHANGE_ANIM_SPRITES]; // Sprites created for the anim
	struct Sprite *sprites2[NUM_FORM_CHANGE_ANIM_SPRITES]; // Same as above, but used to create sprites on the second icon when fusing
	u8 stepId; // Main task step Id
	u8 finished:1; // Set when anim ends
	u8 isFusion:1; // If true, apply the white screen effect on the end of the animations
	u8 numSpritesCreated:4;
	u8 unused:2;
	u16 species;
};

static void Task_DoFusionWhiteScreen(u8 taskId);
static void Task_DoMosaicFormChangeAnim(u8 taskId);
static void Task_DoGracideaFlowersFormChangeAnim(u8 taskId);
static void SpriteCB_WaitGracideaFlowerAnim(struct Sprite *sprite);

static EWRAM_DATA struct FormChangeAnimData *sFormChangeAnimData = NULL;

static void (*const sFormChangeAnimsTable[])(u8) =
{
	[FORM_CHANGE_ANIM_MOSAIC]           = Task_DoMosaicFormChangeAnim,
	[FORM_CHANGE_ANIM_GRACIDEA_FLOWERS] = Task_DoGracideaFlowersFormChangeAnim
};

// Init the form change animation
// animId = The anim index to play
// species = The species the mon is transforming or changing into
// isFusion = Determines if white screen flash occours at the final animation
// icon1 = The sprite of the mon icon
// icon2 = The sprite of the second mon icon, when fusing
void DoFormChangeAnim(u8 animId, u16 species, bool8 isFusion, struct Sprite *icon1, struct Sprite *icon2)
{
	sFormChangeAnimData = AllocZeroed(sizeof(struct FormChangeAnimData));
	
	if (sFormChangeAnimData != NULL)
	{
		CreateTask(sFormChangeAnimsTable[animId], 0);
		
		sFormChangeAnimData->stepId = 0;
		sFormChangeAnimData->icon1 = icon1;
		sFormChangeAnimData->icon2 = icon2;
		sFormChangeAnimData->species = species;
		sFormChangeAnimData->isFusion = isFusion;
		sFormChangeAnimData->finished = FALSE;
		sFormChangeAnimData->numSpritesCreated = 0;
	}
}

// Check if form change anim ends
bool8 IsFormChangeAnimFinished(void)
{
	if (sFormChangeAnimData != NULL)
	{
		if (!sFormChangeAnimData->finished)
			return FALSE;
		
		FREE_AND_SET_NULL(sFormChangeAnimData);
	}
	return TRUE;
}

// Update icon
static void UpdateIconForFormChange(void)
{
	u16 species = sFormChangeAnimData->species;
	
	UpdateCurrentPartyMonIconSpecies(species);
	
	if (sFormChangeAnimData->icon2 != NULL) // Update party after fusion
		UpdatePartyAfterPokemonFusion(species);
}

//////////////////
// WHITE SCREEN //
//////////////////

#define FUSION_WHITE_SCREEN_DELAY 30 // Num frames to stay in the white screen before blend the screen back to normal

#define tStep  data[0]
#define tDelay data[1]

static void DoFusionWhiteScreenOrUpdateIcon(void)
{
	if (sFormChangeAnimData->isFusion)
		gTasks[CreateTask(Task_DoFusionWhiteScreen, 1)].tStep = 0;
	else
		UpdateIconForFormChange();
}

static void Task_DoFusionWhiteScreen(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	
	switch (tStep)
	{
		case 0:
			PlaySE(SE_M_REFLECT);
			BeginNormalPaletteFade(0xFFFFFFFF, -2, 0, 16, RGB_WHITE); // Blend screen to white
			++tStep;
			break;
		case 1:
			if (!gPaletteFade.active) // Wait screen blend to white
			{
				UpdateIconForFormChange();
				tDelay = 0;
				++tStep;
			}
			break;
		case 2:
			if (++tDelay >= FUSION_WHITE_SCREEN_DELAY)
			{
				BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_WHITE); // Blend screen back
				++tStep;
			}
			break;
		case 3:
			if (!gPaletteFade.active) // Wait screen blend back
				DestroyTask(taskId);
			break;
	}
}

#undef tStep
#undef tDelay

//////////////////////////
// FORM CHANGE CRY PLAY //
//////////////////////////

#define PLAY_CRY_WAIT_FRAMES 15 // Num frames to wait after the anim, before start play the pokemon cry

#define tCryWait data[0]

static void Task_PlayMonCryAfterFormChangeAnim(u8 taskId)
{
	switch (sFormChangeAnimData->stepId)
	{
		case 0:
			if (!FuncIsActiveTask(Task_DoFusionWhiteScreen) && sFormChangeAnimData->numSpritesCreated == 0)
			{
				FreeSpriteTilesByTag(FORM_CHANGE_ANIM_TAG);
				FreeSpritePaletteByTag(FORM_CHANGE_ANIM_TAG);
				
				gTasks[taskId].tCryWait = 0;
				++sFormChangeAnimData->stepId;
			}
			break;
		case 1:
			if (++gTasks[taskId].tCryWait >= PLAY_CRY_WAIT_FRAMES)
			{
				PlayCry1(sFormChangeAnimData->species, 0);
				DestroyTask(taskId);
				sFormChangeAnimData->finished = TRUE;
			}
			break;
	}
}

static void InitPlayMonCryAfterFormChangeTask(u8 taskId)
{
	sFormChangeAnimData->stepId = 0;
	gTasks[taskId].func = Task_PlayMonCryAfterFormChangeAnim;
}

#undef tCryWait

/////////////////////////////
// FORM_CHANGE_ANIM_MOSAIC //
/////////////////////////////

#define MOSAIC_ANIM_WAIT_COUNT 80

#define tMosaicData1 data[0]
#define tMosaicData2 data[1]
#define tAnimWait    data[2]

static void InitOrResetIconsForMosaicAnim(u8 taskId, bool8 init)
{
	sFormChangeAnimData->icon1->oam.mosaic = init;
	
	if (sFormChangeAnimData->icon2 != NULL)
		sFormChangeAnimData->icon2->oam.mosaic = init;
	
	if (init)
	{
		gTasks[taskId].tAnimWait = 0;
		gTasks[taskId].tMosaicData1 = 10;
		gTasks[taskId].tMosaicData2 = 1;
		
		SetGpuReg(REG_OFFSET_MOSAIC, (gTasks[taskId].tMosaicData1 << 12) | (gTasks[taskId].tMosaicData2 << 8));
	}
}

static void Task_DoMosaicFormChangeAnim(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	
	switch (sFormChangeAnimData->stepId)
	{
		case 0:
			PlaySE(SE_M_TELEPORT);
			InitOrResetIconsForMosaicAnim(taskId, TRUE);
			++sFormChangeAnimData->stepId;
			break;
		case 1:
			if (++tAnimWait == (MOSAIC_ANIM_WAIT_COUNT / 2))
				DoFusionWhiteScreenOrUpdateIcon();
			
			tMosaicData1 -= tMosaicData2;
			
			if (tMosaicData1 <= 0)
				tMosaicData1 = tAnimWait == MOSAIC_ANIM_WAIT_COUNT ? 0 : 10;
			
			SetGpuReg(REG_OFFSET_MOSAIC, (tMosaicData1 << 12) | (tMosaicData2 << 8));
			
			if (tMosaicData1 == 0)
				InitOrResetIconsForMosaicAnim(taskId, FALSE);
			
			if (tAnimWait == MOSAIC_ANIM_WAIT_COUNT)
				InitPlayMonCryAfterFormChangeTask(taskId);
			break;
	}
}

#undef tMosaicData1
#undef tMosaicData2
#undef tAnimWait

///////////////////////////////////////
// FORM_CHANGE_ANIM_GRACIDEA_FLOWERS //
///////////////////////////////////////

#define GRACIDEA_FLOWER_CREATION_DELAY 5 // Num frames to wait before create a new gracidea sprite

static const u16 sGracideaFlowerGfx[] = INCBIN_U16("graphics/form_change/gracidea_flower.4bpp");
static const u16 sGracideaFlowerPalette[] = INCBIN_U16("graphics/form_change/gracidea_flower.gbapal");

static const struct SpriteSheet sSpriteSheet_GracideaFlower =
{
	sGracideaFlowerGfx, sizeof(sGracideaFlowerGfx), FORM_CHANGE_ANIM_TAG
};

static const struct SpritePalette sSpritePalette_GracideaFlower =
{
    sGracideaFlowerPalette, FORM_CHANGE_ANIM_TAG
};

static const union AffineAnimCmd sAffineAnim_GracideaFlower[] =
{
	AFFINEANIMCMD_FRAME(128, 128, -64, 0), // Start small and rotated
	AFFINEANIMCMD_FRAME(0, 0, 0, 15), // Wait for 15 frames
	AFFINEANIMCMD_FRAME(4, 4, -2, 32), // Increase size + rotate right for 32 frames
	AFFINEANIMCMD_FRAME(8, 8, 0, 16), // Increase size for 16 frames
	AFFINEANIMCMD_FRAME(0, 0, 0, 25), // Wait for 25 frames
	AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnimTable_GracideaFlower[] =
{
	sAffineAnim_GracideaFlower,
};

static const struct OamData sOamData_GracideaFlower =
{
	.y = 0,
	.affineMode = ST_OAM_AFFINE_DOUBLE,
	.objMode = ST_OAM_OBJ_NORMAL,
	.bpp = ST_OAM_4BPP,
	.shape = SPRITE_SHAPE(16x16),
	.x = 0,
	.size = SPRITE_SIZE(16x16),
	.tileNum = 0,
	.priority = 0,
	.paletteNum = 0,
};

static const struct SpriteTemplate sSpriteTemplate_GracideaFlower =
{
	.tileTag = FORM_CHANGE_ANIM_TAG,
	.paletteTag = FORM_CHANGE_ANIM_TAG,
	.oam = &sOamData_GracideaFlower,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = sAffineAnimTable_GracideaFlower,
	.callback = SpriteCB_WaitGracideaFlowerAnim
};

static const struct Coords8 sGracideaFlowersDelta[NUM_FORM_CHANGE_ANIM_SPRITES] =
{
	{.x = 31, .y =  8},
	{.x = 17, .y = 22},
	{.x = 11, .y =  2},
	{.x =  5, .y = 24},
	{.x = 14, .y = 30},
	{.x =  7, .y = 16},
	{.x = 20, .y = 10},
	{.x = 24, .y =  7},
	{.x = 30, .y = 20},
	{.x = 26, .y = 29},
};

static struct Sprite *CreateGracideaFlowerSprite(u8 idx, s16 x, s16 y)
{
	return &gSprites[CreateSprite(&sSpriteTemplate_GracideaFlower, x, y, idx & 2)];
}

static bool8 CreateGracideaFlowerSprites(u8 idx)
{
	if (idx < NUM_FORM_CHANGE_ANIM_SPRITES)
	{
		s8 deltaX = sGracideaFlowersDelta[idx].x, deltaY = sGracideaFlowersDelta[idx].y;
		
		sFormChangeAnimData->sprites1[idx] = CreateGracideaFlowerSprite(idx, sFormChangeAnimData->icon1->oam.x + deltaX, sFormChangeAnimData->icon1->oam.y + deltaY);
		
		if (sFormChangeAnimData->icon2 != NULL)
			sFormChangeAnimData->sprites2[idx] = CreateGracideaFlowerSprite(idx, sFormChangeAnimData->icon2->oam.x + deltaX, sFormChangeAnimData->icon2->oam.y + deltaY);
		
		++sFormChangeAnimData->numSpritesCreated;
		return TRUE;
	}
	return FALSE;
}

static void SpriteCB_WaitGracideaFlowerAnim(struct Sprite *sprite)
{
	if (sprite->affineAnimEnded)
	{
		DestroySprite(sprite);
		--sFormChangeAnimData->numSpritesCreated;
	}
}

#define tDelay        data[0]
#define tCurrSpriteId data[1]

static void Task_DoGracideaFlowersFormChangeAnim(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	
	switch (sFormChangeAnimData->stepId)
	{
		case 0:
			LoadSpriteSheet(&sSpriteSheet_GracideaFlower);
			LoadSpritePalette(&sSpritePalette_GracideaFlower);
			++sFormChangeAnimData->stepId;
			// Fallthrough
		case 1:
			PlaySE(SE_M_PETAL_DANCE);
			tDelay = 0;
			tCurrSpriteId = 0;
			++sFormChangeAnimData->stepId;
			// Fallthrough
		case 2:
			if (--tDelay <= 0)
			{
				if (CreateGracideaFlowerSprites(tCurrSpriteId))
				{
					++tCurrSpriteId;
					tDelay = GRACIDEA_FLOWER_CREATION_DELAY;
				}
				else
				{
					tDelay = 32;
					++sFormChangeAnimData->stepId;
				}
			}
			break;
		case 3:
			if (--tDelay == 0)
			{
				DoFusionWhiteScreenOrUpdateIcon();
				InitPlayMonCryAfterFormChangeTask(taskId);
			}
			break;
	}
}

#undef tDelay
#undef tCurrSpriteId

#include "global.h"
#include "gflib.h"
#include "form_change.h"
#include "party_menu.h"
#include "sound.h"
#include "task.h"
#include "constants/songs.h"

#define NUM_FORM_CHANGE_ANIM_SPRITES 10

#define FORM_CHANGE_ANIM_SPRITE_TAG 0x57D0

struct FormChangeAnimData
{
	struct Sprite *icon1; // The icon sprite of the pokemon
	struct Sprite *icon2; // Same as above, but for the second mon when fusing
	struct Sprite *sprites1[NUM_FORM_CHANGE_ANIM_SPRITES]; // Sprites created for the anim
	struct Sprite *sprites2[NUM_FORM_CHANGE_ANIM_SPRITES]; // Same as above, but used to create sprites on the second icon when fusing
	u8 stepId; // Main task step Id
	u8 finished:1; // Set when anim ends
	u8 isFusion:1; // If true, apply the white screen effect on the end of the animations
	u8 numSpritesCreated:5; // Max 31 sprites. To support NUM_FORM_CHANGE_ANIM_SPRITES for both sprites1 and sprites2
	u8 unused:1;
	u16 oldSpecies;
	u16 newSpecies;
};

static void Task_DoFusionWhiteScreen(u8 taskId);
static void Task_DoMosaicFormChangeAnim(u8 taskId);
static void Task_DoGracideaFlowersFormChangeAnim(u8 taskId);
static void Task_DoPurpleCloudFormChangeAnim(u8 taskId);

static EWRAM_DATA struct FormChangeAnimData *sFormChangeAnimData = NULL;

static const TaskFunc sFormChangeAnimsTable[] =
{
	[FORM_CHANGE_ANIM_MOSAIC]           = Task_DoMosaicFormChangeAnim,
	[FORM_CHANGE_ANIM_GRACIDEA_FLOWERS] = Task_DoGracideaFlowersFormChangeAnim,
	[FORM_CHANGE_ANIM_PURPLE_CLOUD]     = Task_DoPurpleCloudFormChangeAnim,
};

// Init the form change animation
// animId = The anim index to play
// oldSpecies = The original mon's species
// newSpecies = The species the mon is transforming or changing into
// isFusion = Determines if white screen flash occours at the final animation
// icon1 = The sprite of the mon icon
// icon2 = The sprite of the second mon icon, when fusing
void DoFormChangeAnim(u8 animId, u16 oldSpecies, u16 newSpecies, bool8 isFusion, struct Sprite *icon1, struct Sprite *icon2)
{
	sFormChangeAnimData = AllocZeroed(sizeof(struct FormChangeAnimData));
	
	if (sFormChangeAnimData != NULL)
	{
		CreateTask(sFormChangeAnimsTable[animId], 0);
		
		sFormChangeAnimData->stepId = 0;
		sFormChangeAnimData->icon1 = icon1;
		sFormChangeAnimData->icon2 = icon2;
		sFormChangeAnimData->oldSpecies = oldSpecies;
		sFormChangeAnimData->newSpecies = newSpecies;
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
	u16 newSpecies = sFormChangeAnimData->newSpecies;
	
	UpdateCurrentPartyMonIconSpecies(newSpecies);
	
	if (sFormChangeAnimData->icon2 != NULL) // Update party after fusion
		UpdatePartyAfterPokemonFusion(newSpecies);
}

// Create a sprite
static struct Sprite *CreateSpriteForFormChangeAnim(const struct SpriteTemplate *template, struct Sprite *linker, s8 addrX, s8 addrY, u8 subpriority)
{
	struct Sprite *sprite = &gSprites[CreateSprite(template, linker->oam.x + 16 + addrX, linker->oam.y + 16 + addrY, subpriority)];
	sprite->oam.priority = linker->oam.priority - 1;
	++sFormChangeAnimData->numSpritesCreated;
	return sprite;
}

static void SpriteCB_WaitSpriteAffineAnim(struct Sprite *sprite)
{
	if (sprite->affineAnimEnded)
	{
		DestroySprite(sprite);
		--sFormChangeAnimData->numSpritesCreated;
	}
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
			BeginNormalPaletteFade(PALETTES_ALL, -2, 0, 16, RGB_WHITE); // Blend screen to white
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
				BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_WHITE); // Blend screen back
				++tStep;
			}
			break;
		case 3:
			if (!gPaletteFade.active) // Wait screen blend back
			{
				if (sFormChangeAnimData->icon2 == NULL) // Create second mon when defusing
					CreatePartyMonAfterDefusing(sFormChangeAnimData->oldSpecies);
				
				DestroyTask(taskId);
			}
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
				FreeSpriteTilesByTag(FORM_CHANGE_ANIM_SPRITE_TAG);
				FreeSpritePaletteByTag(FORM_CHANGE_ANIM_SPRITE_TAG);
				
				gTasks[taskId].tCryWait = 0;
				++sFormChangeAnimData->stepId;
			}
			break;
		case 1:
			if (++gTasks[taskId].tCryWait >= PLAY_CRY_WAIT_FRAMES)
			{
				PlayCry1(sFormChangeAnimData->newSpecies, 0);
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
	sGracideaFlowerGfx, sizeof(sGracideaFlowerGfx), FORM_CHANGE_ANIM_SPRITE_TAG
};

static const struct SpritePalette sSpritePalette_GracideaFlower =
{
    sGracideaFlowerPalette, FORM_CHANGE_ANIM_SPRITE_TAG
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
	.tileTag = FORM_CHANGE_ANIM_SPRITE_TAG,
	.paletteTag = FORM_CHANGE_ANIM_SPRITE_TAG,
	.oam = &sOamData_GracideaFlower,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = sAffineAnimTable_GracideaFlower,
	.callback = SpriteCB_WaitSpriteAffineAnim
};

static const struct Coords8 sGracideaFlowersDelta[NUM_FORM_CHANGE_ANIM_SPRITES] =
{
	{.x =  15, .y =  -8},
	{.x =   1, .y =   6},
	{.x =  -5, .y = -14},
	{.x = -11, .y =   8},
	{.x =  -2, .y =  14},
	{.x =  -9, .y =   0},
	{.x =   4, .y =  -6},
	{.x =   8, .y =  -9},
	{.x =  14, .y =   4},
	{.x =  10, .y =  13},
};

static inline struct Sprite *CreateGracideaFlowerSprite(struct Sprite *linker, s8 addrX, s8 addrY, u8 idx)
{
	return CreateSpriteForFormChangeAnim(&sSpriteTemplate_GracideaFlower, linker, addrX, addrY, idx & 1);
}

static bool8 CreateGracideaFlowerSprites(u8 idx)
{
	if (idx < NUM_FORM_CHANGE_ANIM_SPRITES)
	{
		s8 deltaX = sGracideaFlowersDelta[idx].x, deltaY = sGracideaFlowersDelta[idx].y;
		
		sFormChangeAnimData->sprites1[idx] = CreateGracideaFlowerSprite(sFormChangeAnimData->icon1, deltaX, deltaY, idx);
		
		if (sFormChangeAnimData->icon2 != NULL)
			sFormChangeAnimData->sprites2[idx] = CreateGracideaFlowerSprite(sFormChangeAnimData->icon2, deltaX, deltaY, idx);
		
		return TRUE;
	}
	return FALSE;
}

#define tDelay        data[0]
#define tCurrSpriteId data[1]

static void Task_DoGracideaFlowersFormChangeAnim(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	
	switch (sFormChangeAnimData->stepId)
	{
		case 0:
		    PlaySE(SE_M_PETAL_DANCE);
			LoadSpriteSheet(&sSpriteSheet_GracideaFlower);
			LoadSpritePalette(&sSpritePalette_GracideaFlower);
			++sFormChangeAnimData->stepId;
			// Fallthrough
		case 1:
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

///////////////////////////////////
// FORM_CHANGE_ANIM_PURPLE_CLOUD //
///////////////////////////////////

#define CLOUD_ANIM_COUNTER 60 // Num frames the clouds will do their anim

static const u16 sPurpleCloudGfx[] = INCBIN_U16("graphics/form_change/purple_cloud.4bpp");
static const u16 sPurpleCloudPalette[] = INCBIN_U16("graphics/form_change/purple_cloud.gbapal");

static const struct SpriteSheet sSpriteSheet_PurpleCloud =
{
	sPurpleCloudGfx, sizeof(sPurpleCloudGfx), FORM_CHANGE_ANIM_SPRITE_TAG
};

static const struct SpritePalette sSpritePalette_PurpleCloud =
{
    sPurpleCloudPalette, FORM_CHANGE_ANIM_SPRITE_TAG
};

static const union AffineAnimCmd sAffineAnim_PurpleCloud[] =
{
	AFFINEANIMCMD_FRAME(330, 330, 0, 0), // Start a little big
	AFFINEANIMCMD_FRAME(3, 3, 0, CLOUD_ANIM_COUNTER), // Increase size for CLOUD_ANIM_COUNTER frames
	AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const sAffineAnimTable_PurpleCloud[] =
{
	sAffineAnim_PurpleCloud,
};

static const struct OamData sOamData_PurpleCloud =
{
	.y = 0,
	.affineMode = ST_OAM_AFFINE_DOUBLE,
	.objMode = ST_OAM_OBJ_NORMAL,
	.bpp = ST_OAM_4BPP,
	.shape = SPRITE_SHAPE(32x32),
	.x = 0,
	.size = SPRITE_SIZE(32x32),
	.tileNum = 0,
	.priority = 0,
	.paletteNum = 0,
};

static const struct SpriteTemplate sSpriteTemplate_PurpleCloud =
{
	.tileTag = FORM_CHANGE_ANIM_SPRITE_TAG,
	.paletteTag = FORM_CHANGE_ANIM_SPRITE_TAG,
	.oam = &sOamData_PurpleCloud,
	.anims = gDummySpriteAnimTable,
	.images = NULL,
	.affineAnims = sAffineAnimTable_PurpleCloud,
	.callback = SpriteCB_WaitSpriteAffineAnim
};

static inline struct Sprite *CreatePurpleCloudSpriteInternal(struct Sprite *linker)
{
	return CreateSpriteForFormChangeAnim(&sSpriteTemplate_PurpleCloud, linker, 0, 0, 0);
}

static void CreatePurpleCloudSprite(void)
{
	PlaySE(SE_M_SELF_DESTRUCT);
	
	LoadSpriteSheet(&sSpriteSheet_PurpleCloud);
	LoadSpritePalette(&sSpritePalette_PurpleCloud);
	
	sFormChangeAnimData->sprites1[0] = CreatePurpleCloudSpriteInternal(sFormChangeAnimData->icon1);
	
	if (sFormChangeAnimData->icon2 != NULL)
		sFormChangeAnimData->sprites2[0] = CreatePurpleCloudSpriteInternal(sFormChangeAnimData->icon2);
}

#define tWaitCounter data[0]

static void Task_DoPurpleCloudFormChangeAnim(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	
	switch (sFormChangeAnimData->stepId)
	{
		case 0:
			CreatePurpleCloudSprite();
			tWaitCounter = 0;
			++sFormChangeAnimData->stepId;
			break;
		case 1:
		    if (++tWaitCounter >= (CLOUD_ANIM_COUNTER - 16))
			{
				DoFusionWhiteScreenOrUpdateIcon();
				InitPlayMonCryAfterFormChangeTask(taskId);
			}
			break;
	}
}

#undef tWaitCounter

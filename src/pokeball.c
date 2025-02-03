#include "global.h"
#include "gflib.h"
#include "battle.h"
#include "battle_anim.h"
#include "decompress.h"
#include "graphics.h"
#include "m4a.h"
#include "pokeball.h"
#include "task.h"
#include "trig.h"
#include "util.h"
#include "link.h"
#include "battle_gfx_sfx_util.h"
#include "constants/songs.h"
#include "constants/sound.h"

struct Pokeball
{
	const u32 *interfaceIcon; // LZ77 compressed pixel data.
	const u32 *interfacePalette; // LZ77 compressed palette data.
	const TaskFunc animationFunc;
	u16 fadeColor;
	u8 particleId:2;
	u8 particleAnimNum:5;
	u8 hasOpenPokeballGfx:1;
};

struct CaptureStar
{
    s8 xOffset;
    s8 yOffset;
    s8 amplitude;
};

#define GET_BALL_TAG(ballId) 	      ((ballId + 55000))
#define GET_BALL_PARTICLE_TAG(ballId) ((ballId + 65030))

// Function Declarations
static void Task_DoPokeballSendOutAnim(u8 taskId);
static void Task_PlayCryWhenReleasedFromBall(u8 taskId);
static void SpriteCB_ReleaseMonFromBall(struct Sprite *sprite);
static void HandleBallAnimEnd(struct Sprite *sprite);
static void SpriteCB_PlayerMonSendOut_1(struct Sprite *sprite);
static void SpriteCB_PlayerMonSendOut_2(struct Sprite *sprite);
static void SpriteCB_ReleaseMon2FromBall(struct Sprite *sprite);
static void SpriteCB_OpponentMonSendOut(struct Sprite *sprite);
static void SpriteCB_PokeballReleaseMon(struct Sprite *sprite);
static void SpriteCB_ReleasedMonFlyOut(struct Sprite *sprite);
static void SpriteCB_TradePokeball(struct Sprite *sprite);
static void SpriteCB_TradePokeballSendOff(struct Sprite *sprite);
static void SpriteCB_TradePokeballEnd(struct Sprite *sprite);
static void Task_FadeMon_ToBallColor(u8);
static void Task_FadeMon_ToNormal(u8);
static void Task_FadeMon_ToNormal_Step(u8);
static void SpriteCB_BallCaptureSuccessStar(struct Sprite *sprite);
static void DestroyBallOpenAnimationParticle(struct Sprite *sprite);
static void PokeBallOpenParticleAnimation(u8);
static void PokeBallOpenParticleAnimation_Step1(struct Sprite *sprite);
static void PokeBallOpenParticleAnimation_Step2(struct Sprite *sprite);
static void GreatBallOpenParticleAnimation(u8);
static void FanOutBallOpenParticles_Step1(struct Sprite *sprite);
static void SafariBallOpenParticleAnimation(u8);
static void UltraBallOpenParticleAnimation(u8);
static void MasterBallOpenParticleAnimation(u8);
static void DiveBallOpenParticleAnimation(u8);
static void RepeatBallOpenParticleAnimation(u8);
static void RepeatBallOpenParticleAnimation_Step1(struct Sprite *sprite);
static void TimerBallOpenParticleAnimation(u8);
static void PremierBallOpenParticleAnimation(u8);
static void PremierBallOpenParticleAnimation_Step1(struct Sprite *sprite);

#include "data/item/pokeballs_graphics.h"

// Data
static const struct CaptureStar sCaptureStar[] =
{
    {
        .xOffset = 10,
        .yOffset = 2,
        .amplitude = -3,
    },
    {
        .xOffset = 15,
        .yOffset = 0,
        .amplitude = -4,
    },
    {
        .xOffset = -10,
        .yOffset = 2,
        .amplitude = -4,
    },
};

static const struct OamData sBallOamData =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sBallAnimSeq0[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

static const union AnimCmd sBallAnimSeq1[] =
{
    ANIMCMD_FRAME(4, 5),
    ANIMCMD_FRAME(8, 5),
    ANIMCMD_END,
};

static const union AnimCmd sBallAnimSeq2[] =
{
    ANIMCMD_FRAME(4, 5),
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_END,
};

static const union AnimCmd sBallAnimSeq3[] =
{
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sBallAnimSeq4[] =
{
    ANIMCMD_FRAME(8, 5),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sBallAnimSeq5[] =
{
    ANIMCMD_FRAME(4, 1),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sBallAnimSeq6[] =
{
    ANIMCMD_FRAME(12, 1),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sBallAnimSequences[] =
{
    sBallAnimSeq0,
    sBallAnimSeq1,
    sBallAnimSeq2,
    sBallAnimSeq3,
    sBallAnimSeq4,
    sBallAnimSeq5,
    sBallAnimSeq6,
};

static const union AffineAnimCmd sBallAffineAnimSeq0[] =
{
    AFFINEANIMCMD_FRAME(0, 0, 0, 1),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sBallAffineAnimSeq1[] =
{
    AFFINEANIMCMD_FRAME(0, 0, -3, 1),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sBallAffineAnimSeq2[] =
{
    AFFINEANIMCMD_FRAME(0, 0, 3, 1),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sBallAffineAnimSeq3[] =
{
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sBallAffineAnimSeq4[] =
{
    AFFINEANIMCMD_FRAME(0, 0, 25, 1),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd *const sBallAffineAnimSequences[] =
{
    sBallAffineAnimSeq0,
    sBallAffineAnimSeq1,
    sBallAffineAnimSeq2,
    sBallAffineAnimSeq3,
    sBallAffineAnimSeq4,
};

static const union AnimCmd sAnim_RegularBall[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_FRAME(1, 1),
    ANIMCMD_FRAME(2, 1),
    ANIMCMD_FRAME(0, 1, .hFlip = TRUE),
    ANIMCMD_FRAME(2, 1),
    ANIMCMD_FRAME(1, 1),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_MasterBall[] =
{
    ANIMCMD_FRAME(3, 1),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_NetDiveBall[] =
{
    ANIMCMD_FRAME(4, 1),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_NestBall[] =
{
    ANIMCMD_FRAME(5, 1),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_LuxuryPremierBall[] =
{
    ANIMCMD_FRAME(6, 4),
    ANIMCMD_FRAME(7, 4),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_UltraRepeatTimerBall[] =    
{
    ANIMCMD_FRAME(7, 4),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_BallParticles[] =    
{
    sAnim_RegularBall,
    sAnim_MasterBall,
    sAnim_NetDiveBall,
    sAnim_NestBall,
    sAnim_LuxuryPremierBall,
    sAnim_UltraRepeatTimerBall,
};

static const void *const sBallParticlesTable[][2] =
{
	{ gBattleAnimSpriteGfx_Particles, gBattleAnimSpritePal_CircleImpact },
	{ gBattleAnimSpriteGfx_Particles2, gBattleAnimSpritePal_Particles2 }
};

static const struct Pokeball sPokeballs[POKE_BALL_ITEMS_END] =
{
	[ITEM_TO_BALL(ITEM_MASTER_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_MasterBall,
		.interfacePalette = sInterfacePal_MasterBall,
		.animationFunc = MasterBallOpenParticleAnimation,
		.particleId = 0,
		.particleAnimNum = 1,
		.fadeColor = RGB(23, 20, 28),
		.hasOpenPokeballGfx = TRUE,
	},
	
	[ITEM_TO_BALL(ITEM_ULTRA_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_UltraBall,
		.interfacePalette = sInterfacePal_UltraBall,
		.animationFunc = UltraBallOpenParticleAnimation,
		.particleId = 0,
		.particleAnimNum = 5,
		.fadeColor = RGB(31, 31, 15),
		.hasOpenPokeballGfx = TRUE,
	},
	
	[ITEM_TO_BALL(ITEM_GREAT_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_GreatBall,
		.interfacePalette = sInterfacePal_GreatBall,
		.animationFunc = GreatBallOpenParticleAnimation,
		.particleId = 0,
		.particleAnimNum = 0,
		.fadeColor = RGB(16, 23, 30),
		.hasOpenPokeballGfx = TRUE,
	},
	
	[ITEM_TO_BALL(ITEM_POKE_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_PokeBall,
		.interfacePalette = sInterfacePal_PokeBall,
		.animationFunc = PokeBallOpenParticleAnimation,
		.particleId = 0,
		.particleAnimNum = 0,
		.fadeColor = RGB(31, 22, 30),
		.hasOpenPokeballGfx = TRUE,
	},
	
	[ITEM_TO_BALL(ITEM_SAFARI_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_SafariBall,
		.interfacePalette = sInterfacePal_SafariBall,
		.animationFunc = SafariBallOpenParticleAnimation,
		.particleId = 0,
		.particleAnimNum = 0,
		.fadeColor = RGB(23, 30, 20),
		.hasOpenPokeballGfx = TRUE,
	},
	
	[ITEM_TO_BALL(ITEM_NET_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_NetBall,
		.interfacePalette = sInterfacePal_NetBall,
		.animationFunc = SafariBallOpenParticleAnimation,
		.particleId = 0,
		.particleAnimNum = 2,
		.fadeColor = RGB(21, 31, 25),
		.hasOpenPokeballGfx = TRUE,
	},
	
	[ITEM_TO_BALL(ITEM_DIVE_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_DiveBall,
		.interfacePalette = sInterfacePal_DiveBall,
		.animationFunc = DiveBallOpenParticleAnimation,
		.particleId = 0,
		.particleAnimNum = 2,
		.fadeColor = RGB(12, 25, 30),
	},
	
	[ITEM_TO_BALL(ITEM_NEST_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_NestBall,
		.interfacePalette = sInterfacePal_NestBall,
		.animationFunc = UltraBallOpenParticleAnimation,
		.particleId = 0,
		.particleAnimNum = 3,
		.fadeColor = RGB(30, 27, 10),
		.hasOpenPokeballGfx = TRUE,
	},
	
	[ITEM_TO_BALL(ITEM_REPEAT_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_RepeatBall,
		.interfacePalette = sInterfacePal_RepeatBall,
		.animationFunc = RepeatBallOpenParticleAnimation,
		.particleId = 0,
		.particleAnimNum = 5,
		.fadeColor = RGB(31, 24, 16),
		.hasOpenPokeballGfx = TRUE,
	},
	
	[ITEM_TO_BALL(ITEM_TIMER_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_TimerBall,
		.interfacePalette = sInterfacePal_TimerBall,
		.animationFunc = TimerBallOpenParticleAnimation,
		.particleId = 0,
		.particleAnimNum = 5,
		.fadeColor = RGB(29, 30, 30),
		.hasOpenPokeballGfx = TRUE,
	},
	
	[ITEM_TO_BALL(ITEM_LUXURY_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_LuxuryBall,
		.interfacePalette = sInterfacePal_LuxuryBall,
		.animationFunc = GreatBallOpenParticleAnimation,
		.particleId = 0,
		.particleAnimNum = 4,
		.fadeColor = RGB(31, 17, 10),
	},
	
	[ITEM_TO_BALL(ITEM_PREMIER_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_PremierBall,
		.interfacePalette = sInterfacePal_PremierBall,
		.animationFunc = PremierBallOpenParticleAnimation,
		.particleId = 0,
		.particleAnimNum = 4,
		.fadeColor = RGB(31, 9, 10),
	},
	
	[ITEM_TO_BALL(ITEM_PARK_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_ParkBall,
		.interfacePalette = sInterfacePal_ParkBall,
		.animationFunc = UltraBallOpenParticleAnimation,
		.particleId = 0,
		.particleAnimNum = 5,
		.fadeColor = RGB(31, 31, 15),
		.hasOpenPokeballGfx = TRUE,
	},
	
	[ITEM_TO_BALL(ITEM_CHERISH_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_CherishBall,
		.interfacePalette = sInterfacePal_CherishBall,
		.animationFunc = MasterBallOpenParticleAnimation,
		.particleId = 1,
		.particleAnimNum = 0,
		.fadeColor = RGB(25, 4, 3),
		.hasOpenPokeballGfx = TRUE,
	},
	
	[ITEM_TO_BALL(ITEM_DUSK_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_DuskBall,
		.interfacePalette = sInterfacePal_DuskBall,
		.animationFunc = UltraBallOpenParticleAnimation,
		.particleId = 1,
		.particleAnimNum = 2,
		.fadeColor = RGB(7, 1, 13),
		.hasOpenPokeballGfx = TRUE,
	},
	
	[ITEM_TO_BALL(ITEM_HEAL_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_HealBall,
		.interfacePalette = sInterfacePal_HealBall,
		.animationFunc = PokeBallOpenParticleAnimation,
		.particleId = 1,
		.particleAnimNum = 0,
		.fadeColor = RGB(31, 23, 27),
		.hasOpenPokeballGfx = TRUE,
	},
	
	[ITEM_TO_BALL(ITEM_QUICK_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_QuickBall,
		.interfacePalette = sInterfacePal_QuickBall,
		.animationFunc = UltraBallOpenParticleAnimation,
		.particleId = 1,
		.particleAnimNum = 4,
		.fadeColor = RGB(16, 25, 30),
		.hasOpenPokeballGfx = TRUE,
	},
	
	[ITEM_TO_BALL(ITEM_FAST_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_FastBall,
		.interfacePalette = sInterfacePal_FastBall,
		.animationFunc = GreatBallOpenParticleAnimation,
		.particleId = 1,
		.particleAnimNum = 4,
		.fadeColor = RGB(29, 17, 8),
		.hasOpenPokeballGfx = TRUE,
	},
	
	[ITEM_TO_BALL(ITEM_LEVEL_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_LevelBall,
		.interfacePalette = sInterfacePal_LevelBall,
		.animationFunc = SafariBallOpenParticleAnimation,
		.particleId = 1,
		.particleAnimNum = 5,
		.fadeColor = RGB(24, 4, 4),
	},
	
	[ITEM_TO_BALL(ITEM_LURE_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_LureBall,
		.interfacePalette = sInterfacePal_LureBall,
		.animationFunc = GreatBallOpenParticleAnimation,
		.particleId = 0,
		.particleAnimNum = 2,
		.fadeColor = RGB(9, 22, 27),
	},
	
	[ITEM_TO_BALL(ITEM_HEAVY_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_HeavyBall,
		.interfacePalette = sInterfacePal_HeavyBall,
		.animationFunc = GreatBallOpenParticleAnimation,
		.particleId = 1,
		.particleAnimNum = 0,
		.fadeColor = RGB(7, 11, 20),
	},
	
	[ITEM_TO_BALL(ITEM_LOVE_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_LoveBall,
		.interfacePalette = sInterfacePal_LoveBall,
		.animationFunc = GreatBallOpenParticleAnimation,
		.particleId = 0,
		.particleAnimNum = 3,
		.fadeColor = RGB(31, 19, 26),
	},
	
	[ITEM_TO_BALL(ITEM_FRIEND_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_FriendBall,
		.interfacePalette = sInterfacePal_FriendBall,
		.animationFunc = UltraBallOpenParticleAnimation,
		.particleId = 0,
		.particleAnimNum = 3,
		.fadeColor = RGB(17, 24, 7),
	},
	
	[ITEM_TO_BALL(ITEM_MOON_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_MoonBall,
		.interfacePalette = sInterfacePal_MoonBall,
		.animationFunc = UltraBallOpenParticleAnimation,
		.particleId = 1,
		.particleAnimNum = 4,
		.fadeColor = RGB(30, 25, 8),
	},
	
	[ITEM_TO_BALL(ITEM_SPORT_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_SportBall,
		.interfacePalette = sInterfacePal_SportBall,
		.animationFunc = UltraBallOpenParticleAnimation,
		.particleId = 0,
		.particleAnimNum = 0,
		.fadeColor = RGB(31, 31, 15),
	},
	
	[ITEM_TO_BALL(ITEM_BEAST_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_BeastBall,
		.interfacePalette = sInterfacePal_BeastBall,
		.animationFunc = UltraBallOpenParticleAnimation,
		.particleId = 0,
		.particleAnimNum = 5,
		.fadeColor = RGB(31, 31, 15),
	},
	
	[ITEM_TO_BALL(ITEM_DREAM_BALL)] =
	{
		.interfaceIcon = sInterfaceGfx_DreamBall,
		.interfacePalette = sInterfacePal_DreamBall,
		.animationFunc = UltraBallOpenParticleAnimation,
		.particleId = 0,
		.particleAnimNum = 5,
		.fadeColor = RGB(31, 31, 15),
	},
};

/////////////////////////
// BALL ULTILITY FUNCS //
/////////////////////////

u8 ItemIdToBallId(u16 itemId)
{
    return itemId > POKE_BALL_ITEMS_END ? ITEM_TO_BALL(ITEM_POKE_BALL) : ITEM_TO_BALL(itemId);
}

static void LoadBallSpriteSheetAndPalette(u8 ballId)
{
	u16 tag = GET_BALL_TAG(ballId);
	struct CompressedSpriteSheet sheet;
	struct CompressedSpritePalette palette;
	
	sheet.data = sPokeballs[ballId].interfaceIcon;
	sheet.size = 0x180;
	sheet.tag = tag;
	LoadCompressedSpriteSheetUsingHeap(&sheet);
	
	palette.data = sPokeballs[ballId].interfacePalette;
	palette.tag = tag;
	LoadCompressedSpritePaletteUsingHeap(&palette);
}

void LoadBallGfx(u8 ballId)
{
	u16 tag = GET_BALL_TAG(ballId);
	
    if (GetSpriteTileStartByTag(tag) == SPRITE_INVALID_TAG)
        LoadBallSpriteSheetAndPalette(ballId);
    
	if (sPokeballs[ballId].hasOpenPokeballGfx)
		LZDecompressVram(sOpenPokeballGfx, (void *)(VRAM + 0x10100 + GetSpriteTileStartByTag(tag) * 32));
}

void FreeBallGfx(u8 ballId)
{
	u16 tag = GET_BALL_TAG(ballId);
    FreeSpriteTilesByTag(tag);
    FreeSpritePaletteByTag(tag);
}

u8 CreateBallSprite(u8 ballId, s16 x, s16 y, u8 subpriority)
{
	u16 tag = GET_BALL_TAG(ballId);
	struct SpriteTemplate template =
	{
		.tileTag = tag,
        .paletteTag = tag,
        .oam = &sBallOamData,
        .anims = sBallAnimSequences,
        .images = NULL,
        .affineAnims = sBallAffineAnimSequences,
        .callback = SpriteCallbackDummy
	};
	return CreateSprite(&template, x, y, subpriority);
}

static void LoadBallParticleGfx(u8 ballId)
{
	struct CompressedSpriteSheet sheet;
	struct CompressedSpritePalette palette;
	u16 tag = GET_BALL_PARTICLE_TAG(ballId);
	
    if (GetSpriteTileStartByTag(tag) == SPRITE_INVALID_TAG)
    {
		u8 particleId = sPokeballs[ballId].particleId;
		
		sheet.data = sBallParticlesTable[particleId][0];
		sheet.size = 0x100;
		sheet.tag = tag;
        LoadCompressedSpriteSheetUsingHeap(&sheet);
		
		palette.data = sBallParticlesTable[particleId][1];
		palette.tag = tag;
        LoadCompressedSpritePaletteUsingHeap(&palette);
    }
}

static u8 CreateBallParticleSprite(u8 ballId, s16 x, s16 y, u8 subpriority)
{
	u16 tag = GET_BALL_PARTICLE_TAG(ballId);
	struct SpriteTemplate template =
	{
		.tileTag = tag,
        .paletteTag = tag,
        .oam = &gOamData_AffineOff_ObjNormal_8x8,
        .anims = sAnims_BallParticles,
        .images = NULL,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCallbackDummy,
	};
	return CreateSprite(&template, x, y, subpriority);
}

u16 GetBattlerPokeballItemId(u8 battlerId)
{
	return GetMonData(GetBattlerIllusionPartyIndexPtr(battlerId), MON_DATA_POKEBALL);
}

#define tFrames          data[0]
#define tThrowId         data[1]
#define tBattler         data[2]
#define tOpponentBattler data[3]

#define sBattler         data[6]

////////////////////////
// BALL SEND OUT ANIM //
////////////////////////

void DoPokeballSendOutAnimation(u8 battlerId, u8 kindOfThrow)
{
    u8 taskId;
    
    gDoingBattleAnim = TRUE;
    gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive = TRUE;
	
    taskId = CreateTask(Task_DoPokeballSendOutAnim, 5);
    gTasks[taskId].tThrowId = kindOfThrow;
    gTasks[taskId].tBattler = battlerId;
}

static void Task_DoPokeballSendOutAnim(u8 taskId)
{
    u8 battlerId, ballId, ballSpriteId;
    s16 x, y;

    if (gTasks[taskId].tFrames == 0)
    {
        gTasks[taskId].tFrames++;
        return;
    }
    battlerId = gTasks[taskId].tBattler;
    ballId = ItemIdToBallId(GetBattlerPokeballItemId(battlerId));
	
    LoadBallGfx(ballId);
    
    ballSpriteId = CreateBallSprite(ballId, 32, 80, 29);
    gSprites[ballSpriteId].data[0] = 0x80;
    gSprites[ballSpriteId].data[1] = 0;
    gSprites[ballSpriteId].data[7] = gTasks[taskId].tThrowId;

    switch (gTasks[taskId].tThrowId)
    {
    case POKEBALL_PLAYER_SENDOUT:
        if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
        {
            x = 32;
            y = 64;
        }
        else
        {
            x = 48;
            y = 70;
        }
        gSprites[ballSpriteId].x = x;
        gSprites[ballSpriteId].y = y;
        gSprites[ballSpriteId].callback = SpriteCB_PlayerMonSendOut_1;
        break;
    case POKEBALL_OPPONENT_SENDOUT:
        gSprites[ballSpriteId].x = GetBattlerSpriteCoord(battlerId, BATTLER_COORD_X);
        gSprites[ballSpriteId].y = GetBattlerSpriteCoord(battlerId, BATTLER_COORD_Y) + 24;
        gSprites[ballSpriteId].data[0] = 0;
        gSprites[ballSpriteId].callback = SpriteCB_OpponentMonSendOut;
        break;
    }
    gSprites[ballSpriteId].sBattler = gBattlerTarget = battlerId;
	DestroyTask(taskId);
}

#undef tFrames
#undef tThrowId
#undef tBattler
#undef tOpponentBattler

#define tCryTaskSpecies         data[0]
#define tCryTaskPan             data[1]
#define tCryTaskWantedCry       data[2]
#define tCryTaskMonPtr1         data[3]
#define tCryTaskMonPtr2         data[4]
#define tCryTaskFrames          data[10]
#define tCryTaskState           data[15]

static void Task_PlayCryWhenReleasedFromBall(u8 taskId)
{
    s8 pan = gTasks[taskId].tCryTaskPan;
    u16 species = gTasks[taskId].tCryTaskSpecies;
    struct Pokemon *mon = (void*)(u32)((u32)(gTasks[taskId].tCryTaskMonPtr1 << 0x10) | ((u16)gTasks[taskId].tCryTaskMonPtr2));

    switch (gTasks[taskId].tCryTaskState)
    {
    case 0:
    default:
        if (gTasks[taskId].data[8] < 3)
            gTasks[taskId].data[8]++;
        else
            gTasks[taskId].tCryTaskState = gTasks[taskId].data[2] + 1;
        break;
    case 1:
		PlayCry_ByMode(species, pan, ShouldPlayNormalPokeCry(mon) ? CRY_MODE_NORMAL : CRY_MODE_WEAK);
        DestroyTask(taskId);
        break;
    case 2:
        StopCryAndClearCrySongs();
        gTasks[taskId].tCryTaskFrames = 3;
        gTasks[taskId].tCryTaskState = 20;
        break;
    case 20:
        if (gTasks[taskId].tCryTaskFrames == 0)
        {
			PlayCry_ReleaseDouble(species, pan, ShouldPlayNormalPokeCry(mon) ? CRY_MODE_DOUBLES : CRY_MODE_WEAK_DOUBLES);
            DestroyTask(taskId);
        }
        else
            gTasks[taskId].tCryTaskFrames--;
        break;
    case 3:
        gTasks[taskId].tCryTaskFrames = 6;
        gTasks[taskId].tCryTaskState = 30;
        break;
    case 30:
        if (gTasks[taskId].tCryTaskFrames != 0)
        {
            gTasks[taskId].tCryTaskFrames--;
            break;
        }
        gTasks[taskId].tCryTaskState++;
    case 31:
        if (!IsCryPlayingOrClearCrySongs())
        {
            StopCryAndClearCrySongs();
            gTasks[taskId].tCryTaskFrames = 3;
            gTasks[taskId].tCryTaskState++;
        }
        break;
    case 32:
        if (gTasks[taskId].tCryTaskFrames != 0)
        {
            gTasks[taskId].tCryTaskFrames--;
            break;
        }
		PlayCry_ReleaseDouble(species, pan, ShouldPlayNormalPokeCry(mon) ? CRY_MODE_NORMAL : CRY_MODE_WEAK);
        DestroyTask(taskId);
        break;
    }
}

static void SpriteCB_ReleaseMonFromBall(struct Sprite *sprite)
{
    u8 battlerId = sprite->sBattler, ballId = ItemIdToBallId(GetBattlerPokeballItemId(battlerId));

    StartSpriteAnim(sprite, 1);
    LaunchBallStarsTask(sprite->x, sprite->y - 5, 1, 0x1C, ballId);
	
    sprite->data[0] = LaunchBallFadeMonTask(TRUE, battlerId, 14, ballId);
    sprite->callback = HandleBallAnimEnd;

    if (gMain.inBattle)
    {
		bool8 isLeftBattlers = (battlerId == GetBattlerAtPosition(B_POSITION_PLAYER_LEFT) || battlerId == GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT));
        struct Pokemon *mon = GetBattlerPartyIndexPtr(battlerId);
        u16 wantedCryCase;
        u8 taskId;

        if (isLeftBattlers && IsDoubleBattleForBattler(battlerId) && gBattleSpritesDataPtr->animationData->healthboxSlideInStarted)
        {
            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                if (IsBGMPlaying())
                    m4aMPlayStop(&gMPlayInfo_BGM);
            }
            else
                m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 128);
        }

        if (!IsDoubleBattleForBattler(battlerId) || !gBattleSpritesDataPtr->animationData->healthboxSlideInStarted)
            wantedCryCase = 0;
        else if (isLeftBattlers)
            wantedCryCase = 1;
        else
            wantedCryCase = 2;
		
        taskId = CreateTask(Task_PlayCryWhenReleasedFromBall, 3);
        gTasks[taskId].tCryTaskSpecies = GetMonData(GetBattlerIllusionPartyIndexPtr(battlerId), MON_DATA_SPECIES);
        gTasks[taskId].tCryTaskPan = GetBattlerSide(battlerId) != B_SIDE_PLAYER ? 25 : -25;
        gTasks[taskId].tCryTaskWantedCry = wantedCryCase;
        gTasks[taskId].tCryTaskMonPtr1 = (u32)(mon) >> 0x10;
        gTasks[taskId].tCryTaskMonPtr2 = (u32)(mon);
        gTasks[taskId].tCryTaskState = 0;
    }
    StartSpriteAffineAnim(&gSprites[gBattlerSpriteIds[sprite->sBattler]], 1);
    AnimateSprite(&gSprites[gBattlerSpriteIds[sprite->sBattler]]);
    gSprites[gBattlerSpriteIds[sprite->sBattler]].data[1] = 0x1000;
}

#undef tCryTaskSpecies
#undef tCryTaskPan
#undef tCryTaskWantedCry
#undef tCryTaskMonPtr1
#undef tCryTaskMonPtr2
#undef tCryTaskFrames
#undef tCryTaskState

static void HandleBallAnimEnd(struct Sprite *sprite)
{
    bool8 affineAnimEnded = FALSE;
    u8 battlerId = sprite->sBattler;

    gSprites[gBattlerSpriteIds[battlerId]].invisible = FALSE;
	
	if (sprite->animEnded)
		sprite->invisible = TRUE;
	
    if (gSprites[gBattlerSpriteIds[battlerId]].affineAnimEnded)
    {
        StartSpriteAffineAnim(&gSprites[gBattlerSpriteIds[battlerId]], 0);
        affineAnimEnded = TRUE;
    }
    else
    {
        gSprites[gBattlerSpriteIds[battlerId]].data[1] -= 288;
        gSprites[gBattlerSpriteIds[battlerId]].y2 = gSprites[gBattlerSpriteIds[battlerId]].data[1] >> 8;
    }
	
    if (sprite->animEnded && affineAnimEnded)
    {
        s32 i, doneBattlers;

        gSprites[gBattlerSpriteIds[battlerId]].y2 = 0;
        gDoingBattleAnim = FALSE;
        gBattleSpritesDataPtr->healthBoxesData[battlerId].ballAnimActive = FALSE;
        DestroySpriteAndFreeMatrix(sprite);

        for (doneBattlers = 0, i = 0; i < MAX_BATTLERS_COUNT; i++)
        {
            if (!gBattleSpritesDataPtr->healthBoxesData[i].ballAnimActive)
                doneBattlers++;
        }
		
        if (doneBattlers == MAX_BATTLERS_COUNT)
        {
            for (i = 0; i < ITEM_TO_BALL(POKE_BALL_ITEMS_END); i++)
                FreeBallGfx(i);
        }
    }
}

static void SpriteCB_PlayerMonSendOut_1(struct Sprite *sprite)
{
    sprite->data[0] = 25;
    sprite->data[2] = GetBattlerSpriteCoord(sprite->sBattler, BATTLER_COORD_X);
    sprite->data[4] = GetBattlerSpriteCoord(sprite->sBattler, BATTLER_COORD_Y_PIC_OFFSET) + 24;
    sprite->data[5] = -30;
    sprite->oam.affineParam = sprite->sBattler;
    InitAnimArcTranslation(sprite);
    sprite->callback = SpriteCB_PlayerMonSendOut_2;
}

#define HIBYTE(x) (((x) >> 8) & 0xFF)

static void SpriteCB_PlayerMonSendOut_2(struct Sprite *sprite)
{
    u32 r6;
    u32 r7;

    if (HIBYTE(sprite->data[7]) >= 35 && HIBYTE(sprite->data[7]) < 80)
    {
        s16 r4;

        if ((sprite->oam.affineParam & 0xFF00) == 0)
        {
            r6 = sprite->data[1] & 1;
            r7 = sprite->data[2] & 1;
            sprite->data[1] = ((sprite->data[1] / 3) & ~1) | r6;
            sprite->data[2] = ((sprite->data[2] / 3) & ~1) | r7;
            StartSpriteAffineAnim(sprite, 4);
        }
        r4 = sprite->data[0];
        AnimTranslateLinear(sprite);
        sprite->data[7] += sprite->sBattler / 3;
        sprite->y2 += Sin(HIBYTE(sprite->data[7]), sprite->data[5]);
        sprite->oam.affineParam += 0x100;
        if ((sprite->oam.affineParam >> 8) % 3 != 0)
            sprite->data[0] = r4;
        else
            sprite->data[0] = r4 - 1;
        if (HIBYTE(sprite->data[7]) >= 80)
        {
            r6 = sprite->data[1] & 1;
            r7 = sprite->data[2] & 1;
            sprite->data[1] = ((sprite->data[1] * 3) & ~1) | r6;
            sprite->data[2] = ((sprite->data[2] * 3) & ~1) | r7;
        }
    }
    else
    {
        if (TranslateAnimHorizontalArc(sprite))
        {
            SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
			
            sprite->sBattler = sprite->oam.affineParam & 0xFF;
            sprite->data[0] = 0;

            if (sprite->sBattler == GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT) && IsDoubleBattleForBattler(sprite->sBattler) && gBattleSpritesDataPtr->animationData->healthboxSlideInStarted)
                sprite->callback = SpriteCB_ReleaseMon2FromBall;
            else
                sprite->callback = SpriteCB_ReleaseMonFromBall;

            StartSpriteAffineAnim(sprite, 0);
        }
    }
}

static void SpriteCB_ReleaseMon2FromBall(struct Sprite *sprite)
{
    if (sprite->data[0]++ > 24)
    {
        sprite->data[0] = 0;
        sprite->callback = SpriteCB_ReleaseMonFromBall;
    }
}

static void SpriteCB_OpponentMonSendOut(struct Sprite *sprite)
{
    if (++sprite->data[0] > 15)
    {
        sprite->data[0] = 0;
		
        if (sprite->sBattler == GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT) && IsDoubleBattleForBattler(sprite->sBattler) && gBattleSpritesDataPtr->animationData->healthboxSlideInStarted)
            sprite->callback = SpriteCB_ReleaseMon2FromBall;
        else
            sprite->callback = SpriteCB_ReleaseMonFromBall;
    }
}

#undef sBattler

//////////////////////////////////
// POKEBALL FOR RELEASE POKEMON //
//////////////////////////////////

void CreatePokeballSpriteToReleaseMon(u8 monSpriteId, u8 battlerId, u8 x, u8 y, u8 oamPriority, u8 subpriortiy, u8 g, u32 h)
{
    u8 spriteId, ballId = ITEM_TO_BALL(ITEM_POKE_BALL);
	
	LoadBallSpriteSheetAndPalette(ballId);
	
    spriteId = CreateBallSprite(ballId, x, y, subpriortiy);
    gSprites[spriteId].data[0] = monSpriteId;
    gSprites[spriteId].data[5] = gSprites[monSpriteId].x;
    gSprites[spriteId].data[6] = gSprites[monSpriteId].y;
    gSprites[monSpriteId].x = x;
    gSprites[monSpriteId].y = y;
    gSprites[spriteId].data[1] = g;
    gSprites[spriteId].data[2] = battlerId;
    gSprites[spriteId].data[3] = h;
    gSprites[spriteId].data[4] = h >> 0x10;
    gSprites[spriteId].oam.priority = oamPriority;
    gSprites[spriteId].callback = SpriteCB_PokeballReleaseMon;
    gSprites[monSpriteId].invisible = TRUE;
}

static void SpriteCB_PokeballReleaseMon(struct Sprite *sprite)
{
    if (sprite->data[1] == 0)
    {
        u8 r5;
        u8 r7 = sprite->data[0];
        u8 battlerId = sprite->data[2];
        u32 r4 = (u16)sprite->data[3] | ((u16)sprite->data[4] << 16);

        if (sprite->subpriority != 0)
            r5 = sprite->subpriority - 1;
        else
            r5 = 0;

        StartSpriteAnim(sprite, 1);
        LaunchBallStarsTask(sprite->x, sprite->y - 5, sprite->oam.priority, r5, ITEM_TO_BALL(ITEM_POKE_BALL));
        sprite->data[1] = LaunchBallFadeMonTask(TRUE, battlerId, r4, ITEM_TO_BALL(ITEM_POKE_BALL));
        sprite->callback = SpriteCB_ReleasedMonFlyOut;
        gSprites[r7].invisible = FALSE;
        StartSpriteAffineAnim(&gSprites[r7], 1);
        AnimateSprite(&gSprites[r7]);
        gSprites[r7].data[1] = 0x1000;
        sprite->data[7] = 0;
    }
    else
        sprite->data[1]--;
}

static void SpriteCB_ReleasedMonFlyOut(struct Sprite *sprite)
{
    bool8 r12 = FALSE;
    bool8 r6 = FALSE;
    u8 monSpriteId = sprite->data[0];
    u16 var1;
    u16 var2;

    if (sprite->animEnded)
        sprite->invisible = TRUE;
    
    if (gSprites[monSpriteId].affineAnimEnded)
    {
        StartSpriteAffineAnim(&gSprites[monSpriteId], 0);
        r12 = TRUE;
    }
    var1 = (sprite->data[5] - sprite->x) * sprite->data[7] / 128 + sprite->x;
    var2 = (sprite->data[6] - sprite->y) * sprite->data[7] / 128 + sprite->y;
    gSprites[monSpriteId].x = var1;
    gSprites[monSpriteId].y = var2;
	
    if (sprite->data[7] < 128)
    {
        s16 sine = -(gSineTable[(u8)sprite->data[7]] / 8);

        sprite->data[7] += 4;
        gSprites[monSpriteId].x2 = sine;
        gSprites[monSpriteId].y2 = sine;
    }
    else
    {
        gSprites[monSpriteId].x = sprite->data[5];
        gSprites[monSpriteId].y = sprite->data[6];
        gSprites[monSpriteId].x2 = 0;
        gSprites[monSpriteId].y2 = 0;
        r6 = TRUE;
    }
    if (sprite->animEnded && r12 && r6)
        DestroySpriteAndFreeResources(sprite);
}

////////////////////
// TRADE POKEBALL //
////////////////////

u8 CreateTradePokeballSprite(u8 monSpriteId, u8 battlerId, u8 x, u8 y, u8 oamPriority, u8 subpriortiy, u8 g, u32 h)
{
    u8 spriteId, ballId = ITEM_TO_BALL(ITEM_POKE_BALL);

    LoadBallSpriteSheetAndPalette(ballId);
	
    spriteId = CreateBallSprite(ballId, x, y, subpriortiy);
    gSprites[spriteId].data[0] = monSpriteId;
    gSprites[spriteId].data[1] = g;
    gSprites[spriteId].data[2] = battlerId;
    gSprites[spriteId].data[3] = h;
    gSprites[spriteId].data[4] = h >> 16;
    gSprites[spriteId].oam.priority = oamPriority;
    gSprites[spriteId].callback = SpriteCB_TradePokeball;
	
    return spriteId;
}

static void SpriteCB_TradePokeball(struct Sprite *sprite)
{
    if (sprite->data[1] == 0)
    {
        u8 r6;
        u8 r7 = sprite->data[0];
        u8 r8 = sprite->data[2];
        u32 r5 = (u16)sprite->data[3] | ((u16)sprite->data[4] << 16);

        if (sprite->subpriority != 0)
            r6 = sprite->subpriority - 1;
        else
            r6 = 0;

        StartSpriteAnim(sprite, 1);
        LaunchBallStarsTask(sprite->x, sprite->y - 5, sprite->oam.priority, r6, ITEM_TO_BALL(ITEM_POKE_BALL));
        sprite->data[1] = LaunchBallFadeMonTask(TRUE, r8, r5, ITEM_TO_BALL(ITEM_POKE_BALL));
        sprite->callback = SpriteCB_TradePokeballSendOff;
        StartSpriteAffineAnim(&gSprites[r7], 2);
        AnimateSprite(&gSprites[r7]);
        gSprites[r7].data[1] = 0;
    }
    else
        sprite->data[1]--;
}

static void SpriteCB_TradePokeballSendOff(struct Sprite *sprite)
{
    u8 monSpriteId = sprite->data[0];

    if (++sprite->data[5] == 11)
        PlaySE(SE_BALL_TRADE);
	
    if (gSprites[monSpriteId].affineAnimEnded)
    {
        StartSpriteAnim(sprite, 2);
        gSprites[monSpriteId].invisible = TRUE;
        sprite->data[5] = 0;
        sprite->callback = SpriteCB_TradePokeballEnd;
    }
    else
    {
        gSprites[monSpriteId].data[1] += 96;
        gSprites[monSpriteId].y2 = -gSprites[monSpriteId].data[1] >> 8;
    }
}

static void SpriteCB_TradePokeballEnd(struct Sprite *sprite)
{
    if (sprite->animEnded)
        sprite->callback = SpriteCallbackDummy;
}

//////////////////////////
// BALL STARS PARTICLES //
//////////////////////////

u8 LaunchBallStarsTask(u8 x, u8 y, u8 priority, u8 subpriority, u8 ballId)
{
    u8 taskId;

    LoadBallParticleGfx(ballId);
	
    taskId = CreateTask(sPokeballs[ballId].animationFunc, 5);
    gTasks[taskId].data[1] = x;
    gTasks[taskId].data[2] = y;
    gTasks[taskId].data[3] = priority;
    gTasks[taskId].data[4] = subpriority;
    gTasks[taskId].data[15] = ballId;
	
    PlaySE(SE_BALL_OPEN);
	
    return taskId;
}

static void DestroyBallOpenAnimationParticle(struct Sprite *sprite)
{
    int i;

    if (!gMain.inBattle)
    {
        if (sprite->data[7] == 1)
            DestroySpriteAndFreeResources(sprite);
        else
            DestroySprite(sprite);
    }
    else
    {
        if (--gBattleSpritesDataPtr->animationData->particleCounter == 0)
        {
            for (i = 0; i < ITEM_TO_BALL(POKE_BALL_ITEMS_END); i++)
            {
				u16 tag = GET_BALL_PARTICLE_TAG(i);
                FreeSpriteTilesByTag(tag);
                FreeSpritePaletteByTag(tag);
            }
        }
        DestroySprite(sprite);
    }
}

static void PokeBallOpenParticleAnimation(u8 taskId)
{
    u8 var0, spriteId, ballId;

    if (gTasks[taskId].data[0] < 16)
    {
		ballId = gTasks[taskId].data[15];
        spriteId = CreateBallParticleSprite(ballId, gTasks[taskId].data[1], gTasks[taskId].data[2], gTasks[taskId].data[4]);
		
        if (spriteId != MAX_SPRITES)
        {
            IncrementBattleParticleCounter();
            StartSpriteAnim(&gSprites[spriteId], sPokeballs[ballId].particleAnimNum);
            gSprites[spriteId].callback = PokeBallOpenParticleAnimation_Step1;
            gSprites[spriteId].oam.priority = gTasks[taskId].data[3];

            var0 = (u8)gTasks[taskId].data[0];
            if (var0 >= 8)
                var0 -= 8;

            gSprites[spriteId].data[0] = var0 * 32;
        }
        if (gTasks[taskId].data[0] == 15)
        {
            if (!gMain.inBattle)
                gSprites[spriteId].data[7] = 1;

            DestroyTask(taskId);
            return;
        }
    }
    gTasks[taskId].data[0]++;
}

static void PokeBallOpenParticleAnimation_Step1(struct Sprite *sprite)
{
    if (sprite->data[1] == 0)
        sprite->callback = PokeBallOpenParticleAnimation_Step2;
    else
        sprite->data[1]--;
}

static void PokeBallOpenParticleAnimation_Step2(struct Sprite *sprite)
{
    sprite->x2 = Sin(sprite->data[0], sprite->data[1]);
    sprite->y2 = Cos(sprite->data[0], sprite->data[1]);
    sprite->data[1] += 2;
    if (sprite->data[1] == 50)
        DestroyBallOpenAnimationParticle(sprite);
}

static void GreatBallOpenParticleAnimation(u8 taskId)
{
    u8 i, ballId, spriteId, x, y, priority, subpriority;

    if (gTasks[taskId].data[7])
        gTasks[taskId].data[7]--;
    else
    {
        ballId = gTasks[taskId].data[15];
		x = gTasks[taskId].data[1];
		y = gTasks[taskId].data[2];
		priority = gTasks[taskId].data[3];
		subpriority = gTasks[taskId].data[4];

        for (i = 0; i < 8; i++)
        {
            spriteId = CreateBallParticleSprite(ballId, x, y, subpriority);
			
            if (spriteId != MAX_SPRITES)
            {
                IncrementBattleParticleCounter();
                StartSpriteAnim(&gSprites[spriteId], sPokeballs[ballId].particleAnimNum);
                gSprites[spriteId].callback = FanOutBallOpenParticles_Step1;
                gSprites[spriteId].oam.priority = priority;
                gSprites[spriteId].data[0] = i * 32;
                gSprites[spriteId].data[4] = 8;
                gSprites[spriteId].data[5] = 2;
                gSprites[spriteId].data[6] = 2;
            }
        }
        gTasks[taskId].data[7] = 8;
		
        if (++gTasks[taskId].data[0] == 2)
        {
            if (!gMain.inBattle)
                gSprites[spriteId].data[7] = 1;

            DestroyTask(taskId);
        }
    }
}

static void FanOutBallOpenParticles_Step1(struct Sprite *sprite)
{
    sprite->x2 = Sin(sprite->data[0], sprite->data[1]);
    sprite->y2 = Cos(sprite->data[0], sprite->data[2]);
    sprite->data[0] = (sprite->data[0] + sprite->data[4]) & 0xFF;
    sprite->data[1] += sprite->data[5];
    sprite->data[2] += sprite->data[6];
    if (++sprite->data[3] == 51)
        DestroyBallOpenAnimationParticle(sprite);
}

static void SafariBallOpenParticleAnimation(u8 taskId)
{
    u8 i, spriteId, ballId = gTasks[taskId].data[15];
    u8 x = gTasks[taskId].data[1], y = gTasks[taskId].data[2], priority = gTasks[taskId].data[3], subpriority = gTasks[taskId].data[4];

    for (i = 0; i < 8; i++)
    {
        spriteId = CreateBallParticleSprite(ballId, x, y, subpriority);
		
        if (spriteId != MAX_SPRITES)
        {
            IncrementBattleParticleCounter();
            StartSpriteAnim(&gSprites[spriteId], sPokeballs[ballId].particleAnimNum);
            gSprites[spriteId].callback = FanOutBallOpenParticles_Step1;
            gSprites[spriteId].oam.priority = priority;
            gSprites[spriteId].data[0] = i * 32;
            gSprites[spriteId].data[4] = 4;
            gSprites[spriteId].data[5] = 1;
            gSprites[spriteId].data[6] = 1;
        }
    }
    if (!gMain.inBattle)
        gSprites[spriteId].data[7] = 1;

    DestroyTask(taskId);
}

static void UltraBallOpenParticleAnimation(u8 taskId)
{
    u8 i, spriteId, ballId = gTasks[taskId].data[15];
    u8 x = gTasks[taskId].data[1], y = gTasks[taskId].data[2], priority = gTasks[taskId].data[3], subpriority = gTasks[taskId].data[4];

    for (i = 0; i < 10; i++)
    {
        spriteId = CreateBallParticleSprite(ballId, x, y, subpriority);
		
        if (spriteId != MAX_SPRITES)
        {
            IncrementBattleParticleCounter();
            StartSpriteAnim(&gSprites[spriteId], sPokeballs[ballId].particleAnimNum);
            gSprites[spriteId].callback = FanOutBallOpenParticles_Step1;
            gSprites[spriteId].oam.priority = priority;
            gSprites[spriteId].data[0] = i * 25;
            gSprites[spriteId].data[4] = 5;
            gSprites[spriteId].data[5] = 1;
            gSprites[spriteId].data[6] = 1;
        }
    }
    if (!gMain.inBattle)
        gSprites[spriteId].data[7] = 1;

    DestroyTask(taskId);
}

static void MasterBallOpenParticleAnimation(u8 taskId)
{
    u8 i, j, spriteId, ballId = gTasks[taskId].data[15];
    u8 x = gTasks[taskId].data[1], y = gTasks[taskId].data[2], priority = gTasks[taskId].data[3], subpriority = gTasks[taskId].data[4];

    for (j = 0; j < 2; j++)
    {
        for (i = 0; i < 8; i++)
        {
            spriteId = CreateBallParticleSprite(ballId, x, y, subpriority);
			
            if (spriteId != MAX_SPRITES)
            {
                IncrementBattleParticleCounter();
                StartSpriteAnim(&gSprites[spriteId], sPokeballs[ballId].particleAnimNum);
                gSprites[spriteId].callback = FanOutBallOpenParticles_Step1;
                gSprites[spriteId].oam.priority = priority;
                gSprites[spriteId].data[0] = i * 32;
                gSprites[spriteId].data[4] = 8;

                if (j == 0)
                {
                    gSprites[spriteId].data[5] = 2;
                    gSprites[spriteId].data[6] = 1;
                }
                else
                {
                    gSprites[spriteId].data[5] = 1;
                    gSprites[spriteId].data[6] = 2;
                }
            }
        }
    }
    if (!gMain.inBattle)
        gSprites[spriteId].data[7] = 1;

    DestroyTask(taskId);
}

static void DiveBallOpenParticleAnimation(u8 taskId)
{
    u8 i, spriteId, ballId = gTasks[taskId].data[15];
    u8 x = gTasks[taskId].data[1], y = gTasks[taskId].data[2], priority = gTasks[taskId].data[3], subpriority = gTasks[taskId].data[4];

    for (i = 0; i < 8; i++)
    {
        spriteId = CreateBallParticleSprite(ballId, x, y, subpriority);
		
        if (spriteId != MAX_SPRITES)
        {
            IncrementBattleParticleCounter();
            StartSpriteAnim(&gSprites[spriteId], sPokeballs[ballId].particleAnimNum);
            gSprites[spriteId].callback = FanOutBallOpenParticles_Step1;
            gSprites[spriteId].oam.priority = priority;
            gSprites[spriteId].data[0] = i * 32;
            gSprites[spriteId].data[4] = 10;
            gSprites[spriteId].data[5] = 1;
            gSprites[spriteId].data[6] = 2;
        }
    }
    if (!gMain.inBattle)
        gSprites[spriteId].data[7] = 1;

    DestroyTask(taskId);
}

static void RepeatBallOpenParticleAnimation(u8 taskId)
{
    u8 i, spriteId, ballId = gTasks[taskId].data[15];
    u8 x = gTasks[taskId].data[1], y = gTasks[taskId].data[2], priority = gTasks[taskId].data[3], subpriority = gTasks[taskId].data[4];

    for (i = 0; i < ITEM_TO_BALL(POKE_BALL_ITEMS_END); i++)
    {
        spriteId = CreateBallParticleSprite(ballId, x, y, subpriority);
		
        if (spriteId != MAX_SPRITES)
        {
            IncrementBattleParticleCounter();
            StartSpriteAnim(&gSprites[spriteId], sPokeballs[ballId].particleAnimNum);
            gSprites[spriteId].callback = RepeatBallOpenParticleAnimation_Step1;
            gSprites[spriteId].oam.priority = priority;
            gSprites[spriteId].data[0] = i * 21;
        }
    }
    if (!gMain.inBattle)
        gSprites[spriteId].data[7] = 1;

    DestroyTask(taskId);
}

static void RepeatBallOpenParticleAnimation_Step1(struct Sprite *sprite)
{
    sprite->x2 = Sin(sprite->data[0], sprite->data[1]);
    sprite->y2 = Cos(sprite->data[0], Sin(sprite->data[0], sprite->data[2]));
    sprite->data[0] = (sprite->data[0] + 6) & 0xFF;
    sprite->data[1]++;
    sprite->data[2]++;
    if (++sprite->data[3] == 51)
        DestroyBallOpenAnimationParticle(sprite);
}

static void TimerBallOpenParticleAnimation(u8 taskId)
{
    u8 i, spriteId, ballId = gTasks[taskId].data[15];
    u8 x = gTasks[taskId].data[1], y = gTasks[taskId].data[2], priority = gTasks[taskId].data[3], subpriority = gTasks[taskId].data[4];

    for (i = 0; i < 8; i++)
    {
        spriteId = CreateBallParticleSprite(ballId, x, y, subpriority);
		
        if (spriteId != MAX_SPRITES)
        {
            IncrementBattleParticleCounter();
            StartSpriteAnim(&gSprites[spriteId], sPokeballs[ballId].particleAnimNum);
            gSprites[spriteId].callback = FanOutBallOpenParticles_Step1;
            gSprites[spriteId].oam.priority = priority;
            gSprites[spriteId].data[0] = i * 32;
            gSprites[spriteId].data[4] = 10;
            gSprites[spriteId].data[5] = 2;
            gSprites[spriteId].data[6] = 1;
        }
    }
    if (!gMain.inBattle)
        gSprites[spriteId].data[7] = 1;

    DestroyTask(taskId);
}

static void PremierBallOpenParticleAnimation(u8 taskId)
{
    u8 i, spriteId, ballId = gTasks[taskId].data[15];
    u8 x = gTasks[taskId].data[1], y = gTasks[taskId].data[2], priority = gTasks[taskId].data[3], subpriority = gTasks[taskId].data[4];

    for (i = 0; i < 8; i++)
    {
        spriteId = CreateBallParticleSprite(ballId, x, y, subpriority);
		
        if (spriteId != MAX_SPRITES)
        {
            IncrementBattleParticleCounter();
            StartSpriteAnim(&gSprites[spriteId], sPokeballs[ballId].particleAnimNum);
            gSprites[spriteId].callback = PremierBallOpenParticleAnimation_Step1;
            gSprites[spriteId].oam.priority = priority;
            gSprites[spriteId].data[0] = i * 32;
        }
    }
    if (!gMain.inBattle)
        gSprites[spriteId].data[7] = 1;

    DestroyTask(taskId);
}

static void PremierBallOpenParticleAnimation_Step1(struct Sprite *sprite)
{
    sprite->x2 = Sin(sprite->data[0], sprite->data[1]);
    sprite->y2 = Cos(sprite->data[0], Sin(sprite->data[0] & 0x3F, sprite->data[2]));
    sprite->data[0] = (sprite->data[0] + 10) & 0xFF;
    sprite->data[1]++;
    sprite->data[2]++;
    if (++sprite->data[3] == 51)
        DestroyBallOpenAnimationParticle(sprite);
}

void CreateStarsWhenBallClicks(struct Sprite *sprite)
{
    u32 i;
    u8 subpriority, ballId = ITEM_TO_BALL(ITEM_MASTER_BALL);

    if (sprite->subpriority)
        subpriority = sprite->subpriority - 1;
    else
    {
        subpriority = 0;
        sprite->subpriority = 1;
    }
    LoadBallParticleGfx(ballId);
	
    for (i = 0; i < 3; i++)
    {
        u8 spriteId = CreateBallParticleSprite(ballId, sprite->x, sprite->y, subpriority);
		
        if (spriteId != MAX_SPRITES)
        {
            gSprites[spriteId].sTransl_Speed = 24;
            gSprites[spriteId].sTransl_DestX = sprite->x + sCaptureStar[i].xOffset;
            gSprites[spriteId].sTransl_DestY = sprite->y + sCaptureStar[i].yOffset;
            gSprites[spriteId].sTransl_ArcAmpl = sCaptureStar[i].amplitude;
            InitAnimArcTranslation(&gSprites[spriteId]);
            gSprites[spriteId].callback = SpriteCB_BallCaptureSuccessStar;
            StartSpriteAnim(&gSprites[spriteId], sPokeballs[ballId].particleAnimNum);
        }
    }
}

static void SpriteCB_BallCaptureSuccessStar(struct Sprite *sprite)
{
    sprite->invisible = !sprite->invisible;
    if (TranslateAnimHorizontalArc(sprite))
        DestroySprite(sprite);
}

///////////////
// BALL FADE //
///////////////

u8 LaunchBallFadeMonTask(bool8 unfadeLater, u8 battler, u32 selectedPalettes, u8 ballId)
{
    u8 taskId;

    taskId = CreateTask(Task_FadeMon_ToBallColor, 5);
    gTasks[taskId].data[15] = ballId;
    gTasks[taskId].data[3] = battler;
    gTasks[taskId].data[10] = selectedPalettes;
    gTasks[taskId].data[11] = selectedPalettes >> 16;

    if (!unfadeLater)
    {
        BlendPalette(battler * 16 + 0x100, 16, 0, sPokeballs[ballId].fadeColor);
        gTasks[taskId].data[1] = 1;
    }
    else
    {
        BlendPalette(battler * 16 + 0x100, 16, 16, sPokeballs[ballId].fadeColor);
        gTasks[taskId].data[0] = 16;
        gTasks[taskId].data[1] = -1;
        gTasks[taskId].func = Task_FadeMon_ToNormal;
    }
    BeginNormalPaletteFade(selectedPalettes, 0, 0, 16, RGB_WHITE);
	
    return taskId;
}

static void Task_FadeMon_ToBallColor(u8 taskId)
{
    if (gTasks[taskId].data[2] <= 16)
    {
        BlendPalette(gTasks[taskId].data[3] * 16 + 0x100, 16, gTasks[taskId].data[0], sPokeballs[gTasks[taskId].data[15]].fadeColor);
        gTasks[taskId].data[0] += gTasks[taskId].data[1];
        gTasks[taskId].data[2]++;
    }
    else if (!gPaletteFade.active)
    {
        u32 selectedPalettes = (u16)gTasks[taskId].data[10] | ((u16)gTasks[taskId].data[11] << 16);
        BeginNormalPaletteFade(selectedPalettes, 0, 16, 0, RGB_WHITE);
        DestroyTask(taskId);
    }
}

static void Task_FadeMon_ToNormal(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        u32 selectedPalettes = (u16)gTasks[taskId].data[10] | ((u16)gTasks[taskId].data[11] << 16);
        BeginNormalPaletteFade(selectedPalettes, 0, 16, 0, RGB_WHITE);
        gTasks[taskId].func = Task_FadeMon_ToNormal_Step;
    }
}

static void Task_FadeMon_ToNormal_Step(u8 taskId)
{
    if (gTasks[taskId].data[2] <= 16)
    {
        BlendPalette(gTasks[taskId].data[3] * 16 + 0x100, 16, gTasks[taskId].data[0], sPokeballs[gTasks[taskId].data[15]].fadeColor);
        gTasks[taskId].data[0] += gTasks[taskId].data[1];
        gTasks[taskId].data[2]++;
    }
    else
        DestroyTask(taskId);
}

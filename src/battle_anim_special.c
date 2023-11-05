#include "global.h"
#include "gflib.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_main.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "decompress.h"
#include "graphics.h"
#include "m4a.h"
#include "pokeball.h"
#include "item_menu_icons.h"
#include "task.h"
#include "trig.h"
#include "util.h"
#include "menu.h"
#include "constants/battle_script_commands.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/songs.h"

#define PIXEL_COORDS_TO_OFFSET(x, y)(		    \
/*Add tiles by X*/								\
((y / 8) * 32 * 8)								\
/*Add tiles by X*/								\
+ ((x / 8) * 32)								\
/*Add pixels by Y*/								\
+ ((((y) - ((y / 8) * 8))) * 4)				    \
/*Add pixels by X*/								\
+ ((((x) - ((x / 8) * 8)) / 2)))

EWRAM_DATA static u8 gAbilityPopUpIds[MAX_BATTLERS_COUNT][2];
EWRAM_DATA u8 gActiveAbilityPopUps = 0;

// Function Declarations
static void AnimTask_FlashHealthboxOnLevelUp_Step(u8);
static void AnimTask_ThrowBall_WaitAnimObjComplete(u8);
static void SpriteCB_ThrowBall_Init(struct Sprite *);
static void AnimTask_ThrowBallSpecial_PlaySfx(u8);
static void AnimTask_ThrowBallSpecial_ResetPlayerSprite(u8);
static void SpriteCB_ThrowBall_ArcFlight(struct Sprite *);
static void TrainerBallBlock(struct Sprite *);
static void SpriteCB_ThrowBall_TenFrameDelay(struct Sprite *);
static void SpriteCB_ThrowBall_ShrinkMon(struct Sprite *);
static void SpriteCB_ThrowBall_InitialFall(struct Sprite *);
static void SpriteCB_ThrowBall_Bounce(struct Sprite *);
static void SpriteCB_ThrowBall_DelayThenBreakOut(struct Sprite *);
static void SpriteCB_ThrowBall_InitShake(struct Sprite *);
static void SpriteCB_ThrowBall_DoShake(struct Sprite *);
static void SpriteCB_ThrowBall_InitClick(struct Sprite *);
static void SpriteCB_ThrowBall_BeginBreakOut(struct Sprite *);
static void SpriteCB_ThrowBall_DoClick(struct Sprite *);
static void SpriteCB_ThrowBall_FinishClick(struct Sprite *);
static void BattleAnimObj_SignalEnd(struct Sprite *);
static void SpriteCB_ThrowBall_RunBreakOut(struct Sprite *);
static void TrainerBallBlock2(struct Sprite *);
static void GhostBallDodge(struct Sprite *sprite);
static void GhostBallDodge2(struct Sprite *sprite);
static void DestroyBallOpenAnimationParticle(struct Sprite *);
static void AnimTask_ShinySparkles(u8);
static void SpriteCB_ShinySparkles_1(struct Sprite *);
static void SpriteCB_ShinySparkles_2(struct Sprite *);
static void AnimTask_ShinySparkles_WaitSparkles(u8);
static void SpriteCB_SafariBaitOrRock_WaitPlayerThrow(struct Sprite *);
static void SpriteCB_SafariBaitOrRock_ArcFlight(struct Sprite *);
static void SpriteCB_SafariBaitOrRock_Finish(struct Sprite *);
static void SpriteCB_SafariBaitOrRock_Init(struct Sprite *);
static void SpriteCB_AbilityPopUp(struct Sprite * sprite);

// Data
const struct SpriteTemplate gSafariBaitSpriteTemplate =
{
    .tileTag = ANIM_TAG_SAFARI_BAIT,
    .paletteTag = ANIM_TAG_SAFARI_BAIT,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_SafariBaitOrRock_Init,
};

static const union AnimCmd sSpriteAnim_SafariRock_0[] =
{
    ANIMCMD_FRAME(64, 1),
    ANIMCMD_END,
};

static const union AnimCmd *const sSpriteAnimTable_SafariRock[] =
{
    sSpriteAnim_SafariRock_0,
};

const struct SpriteTemplate gSafariRockTemplate =
{
    .tileTag = ANIM_TAG_ROCKS,
    .paletteTag = ANIM_TAG_ROCKS,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = sSpriteAnimTable_SafariRock,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_SafariBaitOrRock_Init,
};

static const struct OamData sOamData_AbilityPopUp =
{
	.shape = ST_OAM_H_RECTANGLE,
	.size = 3,
};

static const union AnimCmd sSpriteAnim_AbilityPopUp[] =
{
	ANIMCMD_FRAME(0, 0),
	ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_AbilityPopUp2[] =
{
	ANIMCMD_FRAME(32, 0),
	ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_AbilityPopUp[] =
{
	sSpriteAnim_AbilityPopUp
};

static const union AnimCmd *const sSpriteAnimTable_AbilityPopUp2[] =
{
	sSpriteAnim_AbilityPopUp2
};

static const u16 sAbilityPopUpOverwrittenPixelsTable[][2] =
{
	// First row of image
	{PIXEL_COORDS_TO_OFFSET(0, 4), 2},
	{PIXEL_COORDS_TO_OFFSET(0, 5), 2},
	{PIXEL_COORDS_TO_OFFSET(0, 6), 2},
	{PIXEL_COORDS_TO_OFFSET(0, 7), 2},
	{PIXEL_COORDS_TO_OFFSET(0, 8), 2},
	{PIXEL_COORDS_TO_OFFSET(0, 9), 2},
	{PIXEL_COORDS_TO_OFFSET(0, 10), 2},
	{PIXEL_COORDS_TO_OFFSET(0, 11), 2},
	{PIXEL_COORDS_TO_OFFSET(0, 12), 2},
	{PIXEL_COORDS_TO_OFFSET(0, 13), 8},
	{PIXEL_COORDS_TO_OFFSET(8, 13), 8},
	{PIXEL_COORDS_TO_OFFSET(16, 13), 8},
	{PIXEL_COORDS_TO_OFFSET(24, 13), 8},
	{PIXEL_COORDS_TO_OFFSET(32, 13), 8},
	{PIXEL_COORDS_TO_OFFSET(40, 13), 8},
	{PIXEL_COORDS_TO_OFFSET(48, 13), 8},
	{PIXEL_COORDS_TO_OFFSET(56, 13), 8},
	{PIXEL_COORDS_TO_OFFSET(0, 15), 2},
	{PIXEL_COORDS_TO_OFFSET(0, 16), 2},
	{PIXEL_COORDS_TO_OFFSET(0, 17), 2},
	{PIXEL_COORDS_TO_OFFSET(0, 18), 2},
	{PIXEL_COORDS_TO_OFFSET(0, 19), 2},
	{PIXEL_COORDS_TO_OFFSET(0, 20), 2},
	{PIXEL_COORDS_TO_OFFSET(0, 21), 2},
	{PIXEL_COORDS_TO_OFFSET(0, 22), 2},
	{PIXEL_COORDS_TO_OFFSET(0, 23), 2},
	// Second row of image
	{PIXEL_COORDS_TO_OFFSET(0, 45), 8},
	{PIXEL_COORDS_TO_OFFSET(8, 45), 8},
	{PIXEL_COORDS_TO_OFFSET(16, 45), 8},
	{PIXEL_COORDS_TO_OFFSET(0, 46), 8},
	{PIXEL_COORDS_TO_OFFSET(8, 46), 8},
	{PIXEL_COORDS_TO_OFFSET(16, 46), 8},
};

static const struct UCoords8 sAbilityPopUpCoords[][MAX_BATTLERS_COUNT] =
{
	{ // singles
		{29, 97}, // player
		{186, 57}, // opponent
	},
	{ // doubles
		{29, 80}, // player left
		{186, 19}, // opponent left
		{29, 97}, // player right
		{186, 36}, // opponent right
	}
};

void DoLoadHealthboxPalsForLevelUp(u8 *paletteId1, u8 *paletteId2, u8 battler)
{
    u8 healthBoxSpriteId = gHealthboxSpriteIds[battler], healthBarSpriteId = gSprites[healthBoxSpriteId].hMain_HealthBarSpriteId;
    u16 offset1, offset2;

    *paletteId1 = AllocSpritePalette(TAG_HEALTHBOX_PALS_1);
    *paletteId2 = AllocSpritePalette(TAG_HEALTHBOX_PALS_2);
	
    offset1 = (gSprites[healthBoxSpriteId].oam.paletteNum * 16) + 0x100;
    offset2 = (gSprites[healthBarSpriteId].oam.paletteNum * 16) + 0x100;
	
    LoadPalette(&gPlttBufferUnfaded[offset1], *paletteId1 * 16 + 0x100, 0x20);
    LoadPalette(&gPlttBufferUnfaded[offset2], *paletteId2 * 16 + 0x100, 0x20);
	
    gSprites[healthBoxSpriteId].oam.paletteNum = *paletteId1;
    gSprites[gSprites[healthBoxSpriteId].hMain_HealthBoxOtherSpriteId].oam.paletteNum = *paletteId1;
    gSprites[healthBarSpriteId].oam.paletteNum = *paletteId2;
}

void AnimTask_LoadHealthboxPalsForLevelUp(u8 taskId)
{
    u8 paletteId1, paletteId2;
    DoLoadHealthboxPalsForLevelUp(&paletteId1, &paletteId2, gBattleAnimAttacker);
    DestroyAnimVisualTask(taskId);
}

void DoFreeHealthboxPalsForLevelUp(u8 battler)
{
    u8 healthBoxSpriteId = gHealthboxSpriteIds[battler];
    u8 paletteId1, paletteId2;

    FreeSpritePaletteByTag(TAG_HEALTHBOX_PALS_1);
    FreeSpritePaletteByTag(TAG_HEALTHBOX_PALS_2);
	
    paletteId1 = IndexOfSpritePaletteTag(TAG_HEALTHBOX_PAL);
    paletteId2 = IndexOfSpritePaletteTag(TAG_HEALTHBAR_PAL);
	
    gSprites[healthBoxSpriteId].oam.paletteNum = paletteId1;
    gSprites[gSprites[healthBoxSpriteId].hMain_HealthBoxOtherSpriteId].oam.paletteNum = paletteId1;
    gSprites[gSprites[healthBoxSpriteId].hMain_HealthBarSpriteId].oam.paletteNum = paletteId2;
}

void AnimTask_FreeHealthboxPalsForLevelUp(u8 taskId)
{
    DoFreeHealthboxPalsForLevelUp(gBattleAnimAttacker);
    DestroyAnimVisualTask(taskId);
}

void AnimTask_FlashHealthboxOnLevelUp(u8 taskId)
{
    gTasks[taskId].data[10] = gBattleAnimArgs[0];
    gTasks[taskId].data[11] = gBattleAnimArgs[1];
    gTasks[taskId].func = AnimTask_FlashHealthboxOnLevelUp_Step;
}

static void AnimTask_FlashHealthboxOnLevelUp_Step(u8 taskId)
{
    u8 paletteNum;
    int paletteOffset, colorOffset;

    gTasks[taskId].data[0]++;
    if (gTasks[taskId].data[0]++ >= gTasks[taskId].data[11])
    {
        gTasks[taskId].data[0] = 0;
        paletteNum = IndexOfSpritePaletteTag(TAG_HEALTHBOX_PALS_1);
        colorOffset = gTasks[taskId].data[10] == 0 ? 6 : 2;
        switch (gTasks[taskId].data[1])
        {
        case 0:
            gTasks[taskId].data[2] += 2;
            if (gTasks[taskId].data[2] > 16)
                gTasks[taskId].data[2] = 16;

            paletteOffset = paletteNum * 16 + 0x100;
            BlendPalette(paletteOffset + colorOffset, 1, gTasks[taskId].data[2], RGB(20, 27, 31));
            if (gTasks[taskId].data[2] == 16)
                gTasks[taskId].data[1]++;
            break;
        case 1:
            gTasks[taskId].data[2] -= 2;
            if (gTasks[taskId].data[2] < 0)
                gTasks[taskId].data[2] = 0;

            paletteOffset = paletteNum * 16 + 0x100;
            BlendPalette(paletteOffset + colorOffset, 1, gTasks[taskId].data[2], RGB(20, 27, 31));
            if (gTasks[taskId].data[2] == 0)
                DestroyAnimVisualTask(taskId);
            break;
        }
    }
}

void AnimTask_SwitchOutShrinkMon(u8 taskId)
{
    u8 spriteId;

    spriteId = gBattlerSpriteIds[gBattleAnimAttacker];
    switch (gTasks[taskId].data[0])
    {
    case 0:
        PrepareBattlerSpriteForRotScale(spriteId, ST_OAM_OBJ_NORMAL);
        gTasks[taskId].data[10] = 0x100;
        gTasks[taskId].data[0]++;
        break;
    case 1:
        gTasks[taskId].data[10] += 0x30;
        SetSpriteRotScale(spriteId, gTasks[taskId].data[10], gTasks[taskId].data[10], 0);
        SetBattlerSpriteYOffsetFromYScale(spriteId);
        if (gTasks[taskId].data[10] >= 0x2D0)
            gTasks[taskId].data[0]++;
        break;
    case 2:
        ResetSpriteRotScale(spriteId);
        gSprites[spriteId].invisible = TRUE;
        DestroyAnimVisualTask(taskId);
        break;
    }
}

void AnimTask_SwitchOutBallEffect(u8 taskId)
{
    u8 spriteId = gBattlerSpriteIds[gBattleAnimAttacker];
    u8 ballId = ItemIdToBallId(GetBattlerPokeballItemId(gBattleAnimAttacker));
    u8 x, y;
    u8 priority, subpriority;

    switch (gTasks[taskId].data[0])
    {
    case 0:
        x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
        y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y);
        priority = gSprites[spriteId].oam.priority;
        subpriority = gSprites[spriteId].subpriority;
        gTasks[taskId].data[10] = LaunchBallStarsTask(x, y + 32, priority, subpriority, ballId);
        gTasks[taskId].data[11] = LaunchBallFadeMonTask(FALSE, gBattleAnimAttacker, SelectBattleAnimSpriteAndBgPalettes(1, 0, 0, 0, 0, 0, 0), ballId);
        gTasks[taskId].data[0]++;
        break;
    case 1:
        if (!gTasks[gTasks[taskId].data[10]].isActive && !gTasks[gTasks[taskId].data[11]].isActive)
            DestroyAnimVisualTask(taskId);
        break;
    }
}

void AnimTask_LoadBallGfx(u8 taskId)
{
    LoadBallGfx(ItemIdToBallId(gLastUsedItem));
    DestroyAnimVisualTask(taskId);
}

void AnimTask_FreeBallGfx(u8 taskId)
{
    FreeBallGfx(ItemIdToBallId(gLastUsedItem));
    DestroyAnimVisualTask(taskId);
}

void AnimTask_IsBallBlockedByTrainerOrDodged(u8 taskId)
{
    switch (gBattleSpritesDataPtr->animationData->ballThrowCaseId)
    {
    case BALL_TRAINER_BLOCK:
        gBattleAnimArgs[ARG_RET_ID] = -1;
        break;
    case BALL_GHOST_DODGE:
        gBattleAnimArgs[ARG_RET_ID] = -2;
        break;
    default:
        gBattleAnimArgs[ARG_RET_ID] = 0;
        break;
    }

    DestroyAnimVisualTask(taskId);
}

void AnimTask_ThrowBall(u8 taskId)
{
    u8 spriteId = CreateBallSprite(ItemIdToBallId(gLastUsedItem), 32, 80, 29);
	
    gSprites[spriteId].data[0] = 34;
    gSprites[spriteId].data[1] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    gSprites[spriteId].data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y) - 16;
    gSprites[spriteId].callback = SpriteCB_ThrowBall_Init;
    gBattleSpritesDataPtr->animationData->battlerSpriteVisibility = gSprites[gBattlerSpriteIds[gBattleAnimTarget]].invisible;
    gTasks[taskId].data[0] = spriteId;
    gTasks[taskId].func = AnimTask_ThrowBall_WaitAnimObjComplete;
}

static void AnimTask_ThrowBall_WaitAnimObjComplete(u8 taskId)
{
    if ((u16)gSprites[gTasks[taskId].data[0]].data[0] == 0xFFFF)
        DestroyAnimVisualTask(taskId);
}

void AnimTask_ThrowBallSpecial(u8 taskId)
{
    int x, y;
    u8 subpriority;
    u8 spriteId;

    if (gBattleTypeFlags & BATTLE_TYPE_OLD_MAN_TUTORIAL)
    {
        x = 28;
        y = 11;
    }
    else
    {
        x = 23;
        y = 11;
        if (gSaveBlock2Ptr->playerGender == FEMALE)
            y = 13;
    }
    subpriority = GetBattlerSpriteSubpriority(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)) + 1;
    spriteId = CreateBallSprite(ItemIdToBallId(gLastUsedItem), x | 32, y | 80, subpriority);
    gSprites[spriteId].data[0] = 34;
    gSprites[spriteId].data[1] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
    gSprites[spriteId].data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y) - 16;
    gSprites[gBattlerSpriteIds[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)]].callback = SpriteCB_PlayerThrowInit;
    gTasks[taskId].data[0] = spriteId;
    gTasks[taskId].func = AnimTask_ThrowBallSpecial_PlaySfx;
}

static void AnimTask_ThrowBallSpecial_PlaySfx(u8 taskId)
{
    if (gSprites[gBattlerSpriteIds[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)]].animCmdIndex == 1)
    {
        PlaySE12WithPanning(SE_BALL_THROW, 0);
        gSprites[gTasks[taskId].data[0]].callback = SpriteCB_ThrowBall_Init;
        CreateTask(AnimTask_ThrowBallSpecial_ResetPlayerSprite, 10);
        gTasks[taskId].func = AnimTask_ThrowBall_WaitAnimObjComplete;
    }
}

static void AnimTask_ThrowBallSpecial_ResetPlayerSprite(u8 taskId)
{
    if (gSprites[gBattlerSpriteIds[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)]].animEnded)
    {
        StartSpriteAnim(&gSprites[gBattlerSpriteIds[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)]], 0);
        DestroyTask(taskId);
    }
}

static void SpriteCB_ThrowBall_Init(struct Sprite *sprite)
{
    u16 destX = sprite->data[1];
    u16 destY = sprite->data[2];
    
    sprite->sTransl_InitX = sprite->x;
    sprite->sTransl_DestX = destX;
    sprite->sTransl_InitY = sprite->y;
    sprite->sTransl_DestY = destY;
    sprite->sTransl_ArcAmpl = -40;
    InitAnimArcTranslation(sprite);
    sprite->callback = SpriteCB_ThrowBall_ArcFlight;
}

static void SpriteCB_ThrowBall_ArcFlight(struct Sprite *sprite)
{
    int i;
    u8 ballId;

    if (TranslateAnimHorizontalArc(sprite))
    {
        if (gBattleSpritesDataPtr->animationData->ballThrowCaseId == BALL_TRAINER_BLOCK)
        {
            sprite->callback = TrainerBallBlock;
        }
        else if (gBattleSpritesDataPtr->animationData->ballThrowCaseId == BALL_GHOST_DODGE)
        {
            sprite->callback = GhostBallDodge;
        }
        else
        {
            StartSpriteAnim(sprite, 1);
            sprite->x += sprite->x2;
            sprite->y += sprite->y2;
            sprite->x2 = 0;
            sprite->y2 = 0;

            for (i = 0; i < 8; i++)
                sprite->data[i] = 0;

            sprite->data[5] = 0;
            sprite->callback = SpriteCB_ThrowBall_TenFrameDelay;

            ballId = ItemIdToBallId(gLastUsedItem);
            switch (ballId)
            {
            case 0 ... ITEM_TO_BALL(POKE_BALL_ITEMS_END - 1):
                LaunchBallStarsTask(sprite->x, sprite->y - 5, 1, 28, ballId);
                LaunchBallFadeMonTask(FALSE, gBattleAnimTarget, 14, ballId);
                break;
            }
        }
    }
}

static void SpriteCB_ThrowBall_TenFrameDelay(struct Sprite *sprite)
{
    if (++sprite->data[5] == 10)
    {
        sprite->data[5] = CreateTask(TaskDummy, 50);
        sprite->callback = SpriteCB_ThrowBall_ShrinkMon;
        gSprites[gBattlerSpriteIds[gBattleAnimTarget]].data[1] = 0;
    }
}

static void SpriteCB_ThrowBall_ShrinkMon(struct Sprite *sprite)
{
    u8 spriteId;
    u8 taskId;
    u16 shrinkDistance, shrinkDelta;
	
    spriteId = gBattlerSpriteIds[gBattleAnimTarget];
    taskId = sprite->data[5];
    if (++gTasks[taskId].data[1] == 11)
        PlaySE(SE_BALL_TRADE);

    switch (gTasks[taskId].data[0])
    {
    case 0:
        PrepareBattlerSpriteForRotScale(spriteId, ST_OAM_OBJ_NORMAL);
        gTasks[taskId].data[10] = 256;
        shrinkDistance = (gSprites[spriteId].y + gSprites[spriteId].y2) - (sprite->y + sprite->y2);
        shrinkDelta = (shrinkDistance * 256) / 28;
        gTasks[taskId].data[2] = shrinkDelta;
        gTasks[taskId].data[0]++;
        break;
    case 1:
        gTasks[taskId].data[10] += 0x20;
        SetSpriteRotScale(spriteId, gTasks[taskId].data[10], gTasks[taskId].data[10], 0);
        gTasks[taskId].data[3] += gTasks[taskId].data[2];
        gSprites[spriteId].y2 = -gTasks[taskId].data[3] >> 8;
        if (gTasks[taskId].data[10] >= 0x480)
            gTasks[taskId].data[0]++;
        break;
    case 2:
        ResetSpriteRotScale(spriteId);
        gSprites[spriteId].invisible = TRUE;
        gTasks[taskId].data[0]++;
        break;
    default:
        if (gTasks[taskId].data[1] > 10)
        {
            DestroyTask(taskId);
            StartSpriteAnim(sprite, 2);
            sprite->data[5] = 0;
            sprite->callback = SpriteCB_ThrowBall_InitialFall;
        }
        break;
    }
}

static void SpriteCB_ThrowBall_InitialFall(struct Sprite *sprite)
{
    int angle;

    if (sprite->animEnded)
    {
        sprite->data[3] = 0;
        sprite->data[4] = 40;
        sprite->data[5] = 0;
        angle = 0;
        sprite->y += Cos(angle, 40);
        sprite->y2 = -Cos(angle, sprite->data[4]);
        sprite->callback = SpriteCB_ThrowBall_Bounce;
    }
}

static void SpriteCB_ThrowBall_Bounce(struct Sprite *sprite)
{
    bool8 lastBounce;
    int bounceCount;

    lastBounce = FALSE;
    switch (sprite->data[3] & 0xFF)
    {
    case 0:
        sprite->y2 = -Cos(sprite->data[5], sprite->data[4]);
        sprite->data[5] += (sprite->data[3] >> 8) + 4;
        if (sprite->data[5] >= 64)
        {
            sprite->data[4] -= 10;
            sprite->data[3] += 257;

            bounceCount = sprite->data[3] >> 8;
            if (bounceCount == 4)
                lastBounce = TRUE;

            // Play a different sound effect for each pokeball bounce.
            switch (bounceCount)
            {
            case 1:
                PlaySE(SE_BALL_BOUNCE_1);
                break;
            case 2:
                PlaySE(SE_BALL_BOUNCE_2);
                break;
            case 3:
                PlaySE(SE_BALL_BOUNCE_3);
                break;
            default:
                PlaySE(SE_BALL_BOUNCE_4);
                break;
            }
        }
        break;
    case 1:
        sprite->y2 = -Cos(sprite->data[5], sprite->data[4]);
        sprite->data[5] -= (sprite->data[3] >> 8) + 4;
        if (sprite->data[5] <= 0)
        {
            sprite->data[5] = 0;
            sprite->data[3] &= -0x100;
        }
        break;
    }

    if (lastBounce)
    {
        sprite->data[3] = 0;
		sprite->data[5] = 0;
        sprite->y += Cos(64, 40);
        sprite->y2 = 0;
        if (gBattleSpritesDataPtr->animationData->ballThrowCaseId == BALL_NO_SHAKES)
            sprite->callback = SpriteCB_ThrowBall_DelayThenBreakOut;
        else
        {
			sprite->data[4] = 1;
            sprite->callback = SpriteCB_ThrowBall_InitShake;
        }
    }
}

static void SpriteCB_ThrowBall_InitShake(struct Sprite *sprite)
{
    if (++sprite->data[3] == 31)
    {
        sprite->data[3] = 0;
        sprite->affineAnimPaused = TRUE;
        StartSpriteAffineAnim(sprite, 1);
        gBattleSpritesDataPtr->animationData->ballShakeThing = 0;
        sprite->callback = SpriteCB_ThrowBall_DoShake;
        PlaySE(SE_BALL);
    }
}

static void SpriteCB_ThrowBall_DoShake(struct Sprite *sprite)
{
    s8 state;
    u16 var0;

    switch (sprite->data[3] & 0xFF)
    {
    case 0:
        if (gBattleSpritesDataPtr->animationData->ballShakeThing > 0xFF)
        {
            sprite->x2 += sprite->data[4];
            gBattleSpritesDataPtr->animationData->ballShakeThing &= 0xFF;
        }
        else
        {
            gBattleSpritesDataPtr->animationData->ballShakeThing += 0xB0;
        }

        sprite->data[5]++;
        sprite->affineAnimPaused = FALSE;
        var0 = sprite->data[5] + 7;
        if (var0 > 14)
        {
            gBattleSpritesDataPtr->animationData->ballShakeThing = 0;
            sprite->data[3]++;
            sprite->data[5] = 0;
        }
        break;
    case 1:
        if (++sprite->data[5] == 1)
        {
            sprite->data[5] = 0;
            sprite->data[4] = -sprite->data[4];
            sprite->data[3]++;
            sprite->affineAnimPaused = FALSE;
			ChangeSpriteAffineAnim(sprite, sprite->data[4] < 0 ? 2 : 1);
        }
        else
        {
            sprite->affineAnimPaused = TRUE;
        }
        break;
    case 2:
        if (gBattleSpritesDataPtr->animationData->ballShakeThing > 0xFF)
        {
            sprite->x2 += sprite->data[4];
            gBattleSpritesDataPtr->animationData->ballShakeThing &= 0xFF;
        }
        else
        {
            gBattleSpritesDataPtr->animationData->ballShakeThing += 0xB0;
        }

        sprite->data[5]++;
        sprite->affineAnimPaused = FALSE;
        var0 = sprite->data[5] + 12;
        if (var0 > 24)
        {
            gBattleSpritesDataPtr->animationData->ballShakeThing = 0;
            sprite->data[3]++;
            sprite->data[5] = 0;
        }
        break;
    case 3:
        if (sprite->data[5]++ < 0)
        {
            sprite->affineAnimPaused = TRUE;
            break;
        }

        sprite->data[5] = 0;
        sprite->data[4] = -sprite->data[4];
        sprite->data[3]++;
        sprite->affineAnimPaused = FALSE;
		ChangeSpriteAffineAnim(sprite, sprite->data[4] < 0 ? 2 : 1);
        // fall through
    case 4:
        if (gBattleSpritesDataPtr->animationData->ballShakeThing > 0xFF)
        {
            sprite->x2 += sprite->data[4];
            gBattleSpritesDataPtr->animationData->ballShakeThing &= 0xFF;
        }
        else
        {
            gBattleSpritesDataPtr->animationData->ballShakeThing += 0xB0;
        }

        sprite->data[5]++;
        sprite->affineAnimPaused = FALSE;
        var0 = sprite->data[5] + 4;
        if (var0 > 8)
        {
            gBattleSpritesDataPtr->animationData->ballShakeThing = 0;
            sprite->data[3]++;
            sprite->data[5] = 0;
            sprite->data[4] = -sprite->data[4];
        }
        break;
    case 5:
        sprite->data[3] += 0x100;
        state = sprite->data[3] >> 8;
        if (state == gBattleSpritesDataPtr->animationData->ballThrowCaseId)
        {
            sprite->affineAnimPaused = TRUE;
            sprite->callback = SpriteCB_ThrowBall_DelayThenBreakOut;
        }
        else
        {
            if (gBattleSpritesDataPtr->animationData->ballThrowCaseId == BALL_3_SHAKES_SUCCESS && state == 3)
            {
                sprite->callback = SpriteCB_ThrowBall_InitClick;
                sprite->affineAnimPaused = TRUE;
            }
            else
            {
                sprite->data[3]++;
                sprite->affineAnimPaused = TRUE;
            }
        }
        break;
    case 6:
    default:
        if (++sprite->data[5] == 31)
        {
            sprite->data[5] = 0;
            sprite->data[3] &= -0x100;
            StartSpriteAffineAnim(sprite, 3);
			StartSpriteAffineAnim(sprite, sprite->data[4] < 0 ? 2 : 1);
            PlaySE(SE_BALL);
        }
        break;
    }
}

static void SpriteCB_ThrowBall_DelayThenBreakOut(struct Sprite *sprite)
{
    if (++sprite->data[5] == 31)
    {
        sprite->data[5] = 0;
        sprite->callback = SpriteCB_ThrowBall_BeginBreakOut;
    }
}

static void SpriteCB_ThrowBall_InitClick(struct Sprite *sprite)
{
    sprite->animPaused = TRUE;
    sprite->callback = SpriteCB_ThrowBall_DoClick;
    sprite->data[3] = 0;
    sprite->data[4] = 0;
    sprite->data[5] = 0;
}

static void SpriteCB_ThrowBall_DoClick(struct Sprite *sprite)
{
    u8 *battler = &gBattleAnimTarget;

    sprite->data[4]++;
    if (sprite->data[4] == 40)
    {
        PlaySE(SE_BALL_CLICK);
        BlendPalettes(0x10000 << sprite->oam.paletteNum, 6, RGB_BLACK);
        CreateStarsWhenBallClicks(sprite);
    }
    else if (sprite->data[4] == 60)
    {
        BeginNormalPaletteFade(0x10000 << sprite->oam.paletteNum, 2, 6, 0, RGB_BLACK);
    }
    else if (sprite->data[4] == 95)
    {
        gDoingBattleAnim = FALSE;
        UpdateOamPriorityInAllHealthboxes(1, FALSE);
        m4aMPlayAllStop();
        PlaySE(MUS_CAUGHT_INTRO);
    }
    else if (sprite->data[4] == 315)
    {
        FreeOamMatrix(gSprites[gBattlerSpriteIds[*battler]].oam.matrixNum);
        DestroySprite(&gSprites[gBattlerSpriteIds[*battler]]);
        sprite->data[0] = 0;
        sprite->callback = SpriteCB_ThrowBall_FinishClick;
    }
}

static void SpriteCB_ThrowBall_FinishClick(struct Sprite *sprite)
{
    u8 paletteIndex;

    switch (sprite->data[0])
    {
    case 0:
        sprite->data[1] = 0;
        sprite->data[2] = 0;
        sprite->oam.objMode = ST_OAM_OBJ_BLEND;
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16, 0));
        paletteIndex = IndexOfSpritePaletteTag(sprite->template->paletteTag);
        BeginNormalPaletteFade(1 << (paletteIndex + 0x10), 0, 0, 16, RGB_WHITE);
        sprite->data[0]++;
        break;
    case 1:
        if (sprite->data[1]++ > 0)
        {
            sprite->data[1] = 0;
            sprite->data[2]++;
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16 - sprite->data[2], sprite->data[2]));
            if (sprite->data[2] == 16)
                sprite->data[0]++;
        }
        break;
    case 2:
        sprite->invisible = TRUE;
        sprite->data[0]++;
        break;
    default:
        if (!gPaletteFade.active)
        {
            SetGpuReg(REG_OFFSET_BLDCNT, 0);
            SetGpuReg(REG_OFFSET_BLDALPHA, 0);
            sprite->data[0] = 0;
            sprite->callback = BattleAnimObj_SignalEnd;
        }
        break;
    }
}

static void BattleAnimObj_SignalEnd(struct Sprite *sprite)
{
    if (sprite->data[0] == 0)
    {
        sprite->data[0] = -1;
    }
    else
    {
        FreeSpriteOamMatrix(sprite);
        DestroySprite(sprite);
    }
}

static void SpriteCB_ThrowBall_BeginBreakOut(struct Sprite *sprite)
{
    u8 ballId = ItemIdToBallId(gLastUsedItem);

    StartSpriteAnim(sprite, 1);
    StartSpriteAffineAnim(sprite, 0);
    sprite->callback = SpriteCB_ThrowBall_RunBreakOut;

    switch (ballId)
    {
    case 0 ... ITEM_TO_BALL(POKE_BALL_ITEMS_END - 1):
        LaunchBallStarsTask(sprite->x, sprite->y - 5, 1, 28, ballId);
        LaunchBallFadeMonTask(TRUE, gBattleAnimTarget, 14, ballId);
        break;
    }
    gSprites[gBattlerSpriteIds[gBattleAnimTarget]].invisible = FALSE;
    StartSpriteAffineAnim(&gSprites[gBattlerSpriteIds[gBattleAnimTarget]], 1);
    AnimateSprite(&gSprites[gBattlerSpriteIds[gBattleAnimTarget]]);
    gSprites[gBattlerSpriteIds[gBattleAnimTarget]].data[1] = 0x1000;
}

static void SpriteCB_ThrowBall_RunBreakOut(struct Sprite *sprite)
{
    bool32 next = FALSE;

    if (sprite->animEnded)
        sprite->invisible = TRUE;

    if (gSprites[gBattlerSpriteIds[gBattleAnimTarget]].affineAnimEnded)
    {
        StartSpriteAffineAnim(&gSprites[gBattlerSpriteIds[gBattleAnimTarget]], 0);
        next = TRUE;
    }
    else
    {
        gSprites[gBattlerSpriteIds[gBattleAnimTarget]].data[1] -= 288;
        gSprites[gBattlerSpriteIds[gBattleAnimTarget]].y2 = gSprites[gBattlerSpriteIds[gBattleAnimTarget]].data[1] >> 8;
    }

    if (sprite->animEnded && next)
    {
        gSprites[gBattlerSpriteIds[gBattleAnimTarget]].y2 = 0;
        gSprites[gBattlerSpriteIds[gBattleAnimTarget]].invisible = gBattleSpritesDataPtr->animationData->battlerSpriteVisibility;
        sprite->data[0] = 0;
        sprite->callback = BattleAnimObj_SignalEnd;
        gDoingBattleAnim = FALSE;
        UpdateOamPriorityInAllHealthboxes(1, FALSE);
    }
}

static void TrainerBallBlock(struct Sprite *sprite)
{
    int i;

    sprite->x += sprite->x2;
    sprite->y += sprite->y2;
    sprite->x2 = sprite->y2 = 0;
    for (i = 0; i < 6; i++)
        sprite->data[i] = 0;

    sprite->callback = TrainerBallBlock2;
}

static void TrainerBallBlock2(struct Sprite *sprite)
{
    s16 var0 = sprite->data[0] + 0x800;
    s16 var1 = sprite->data[1] + 0x680;
    
    sprite->x2 -= var1 >> 8;
    sprite->y2 += var0 >> 8;
    sprite->data[0] = (sprite->data[0] + 0x800) & 0xFF;
    sprite->data[1] = (sprite->data[1] + 0x680) & 0xFF;
    if (sprite->y + sprite->y2 > 160
     || sprite->x + sprite->x2 < -8)
    {
        sprite->data[0] = 0;
        sprite->callback = BattleAnimObj_SignalEnd;
        gDoingBattleAnim = FALSE;
        UpdateOamPriorityInAllHealthboxes(1, FALSE);
    }
}

static void GhostBallDodge(struct Sprite *sprite)
{
    sprite->x += sprite->x2;
    sprite->y += sprite->y2;
    sprite->x2 = sprite->y2 = 0;
    sprite->data[0] = 0x22;
    sprite->data[1] = sprite->x;
    sprite->data[2] = sprite->x - 8;
    sprite->data[3] = sprite->y;
    sprite->data[4] = 0x90;
    sprite->data[5] = 0x20;
    InitAnimArcTranslation(sprite);
    TranslateAnimVerticalArc(sprite);
    sprite->callback = GhostBallDodge2;
}

static void GhostBallDodge2(struct Sprite *sprite)
{
    if (!TranslateAnimVerticalArc(sprite))
    {
        if ((sprite->y + sprite->y2) < 65)
            return;
    }
    
    sprite->data[0] = 0;
    sprite->callback = BattleAnimObj_SignalEnd;
    gDoingBattleAnim = FALSE;
    UpdateOamPriorityInAllHealthboxes(1, FALSE);
}

void IncrementBattleParticleCounter(void)
{
    if (gMain.inBattle)
        gBattleSpritesDataPtr->animationData->particleCounter++;
}

void AnimTask_SwapMonSpriteToFromSubstitute(u8 taskId)
{
    u8 spriteId;
    u32 x;
    bool32 done = FALSE;

    spriteId = gBattlerSpriteIds[gBattleAnimAttacker];
    switch (gTasks[taskId].data[10])
    {
    case 0:
        gTasks[taskId].data[11] = gBattleAnimArgs[0];
        gTasks[taskId].data[0] += 0x500;
        if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
            gSprites[spriteId].x2 += gTasks[taskId].data[0] >> 8;
        else
            gSprites[spriteId].x2 -= gTasks[taskId].data[0] >> 8;

        gTasks[taskId].data[0] &= 0xFF;
        x = gSprites[spriteId].x + gSprites[spriteId].x2 + 32;
        if (x > 304)
            gTasks[taskId].data[10]++;
        break;
    case 1:
        LoadBattleMonGfxAndAnimate(gBattleAnimAttacker, gTasks[taskId].data[11], spriteId);
        gTasks[taskId].data[10]++;
        break;
    case 2:
        gTasks[taskId].data[0] += 0x500;
        if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
            gSprites[spriteId].x2 -= gTasks[taskId].data[0] >> 8;
        else
            gSprites[spriteId].x2 += gTasks[taskId].data[0] >> 8;

        gTasks[taskId].data[0] &= 0xFF;
        if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        {
            if (gSprites[spriteId].x2 <= 0)
            {
                gSprites[spriteId].x2 = 0;
                done = TRUE;
            }
        }
        else
        {
            if (gSprites[spriteId].x2 >= 0)
            {
                gSprites[spriteId].x2 = 0;
                done = TRUE;
            }
        }

        if (done)
            DestroyAnimVisualTask(taskId);

        break;
    }
}

void AnimTask_SubstituteFadeToInvisible(u8 taskId)
{
    u8 spriteId;

    switch (gTasks[taskId].data[15])
    {
    case 0:
        if (GetBattlerSpriteBGPriorityRank(gBattleAnimAttacker) == B_POSITION_OPPONENT_LEFT)
            SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
        else
            SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);

        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16, 0));
        gTasks[taskId].data[15]++;
        break;
    case 1:
        if (gTasks[taskId].data[1]++ > 1)
        {
            gTasks[taskId].data[1] = 0;
            gTasks[taskId].data[0]++;
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16 - gTasks[taskId].data[0], gTasks[taskId].data[0]));
            if (gTasks[taskId].data[0] == 16)
                gTasks[taskId].data[15]++;
        }
        break;
    case 2:
        spriteId = gBattlerSpriteIds[gBattleAnimAttacker];
        RequestDma3Fill(0, (void *)OBJ_VRAM0 + gSprites[spriteId].oam.tileNum * TILE_SIZE_4BPP, 0x800, DMA3_32BIT);
        ClearBehindSubstituteBit(gBattleAnimAttacker);
        DestroyAnimVisualTask(taskId);
        break;
    }
}

void AnimTask_IsAttackerBehindSubstitute(u8 taskId)
{
    gBattleAnimArgs[ARG_RET_ID] = gBattleSpritesDataPtr->battlerData[gBattleAnimAttacker].behindSubstitute;
    DestroyAnimVisualTask(taskId);
}

void AnimTask_TargetToEffectBattler(u8 taskId)
{
    gBattleAnimTarget = gEffectBattler;
    DestroyAnimVisualTask(taskId);
}

void TryShinyAnimation(u8 battler)
{
    u8 taskId1, taskId2;

    gBattleSpritesDataPtr->healthBoxesData[battler].triedShinyMonAnim = TRUE;

    if (IsBattlerSpriteVisible(battler))
    {
        if (GetMonData(GetBattlerIllusionPartyIndexPtr(battler), MON_DATA_IS_SHINY))
        {
            if (GetSpriteTileStartByTag(ANIM_TAG_GOLD_STARS) == 0xFFFF)
            {
                LoadCompressedSpriteSheetUsingHeap(&gBattleAnimPicTable[ANIM_TAG_GOLD_STARS - ANIM_SPRITES_START]);
                LoadCompressedSpritePaletteUsingHeap(&gBattleAnimPaletteTable[ANIM_TAG_GOLD_STARS - ANIM_SPRITES_START]);
            }
            taskId1 = CreateTask(AnimTask_ShinySparkles, 10);
            taskId2 = CreateTask(AnimTask_ShinySparkles, 10);
            gTasks[taskId1].data[0] = battler;
            gTasks[taskId2].data[0] = battler;
            gTasks[taskId1].data[1] = 0;
            gTasks[taskId2].data[1] = 1;
            return;
        }
    }
    gBattleSpritesDataPtr->healthBoxesData[battler].finishedShinyMonAnim = 1;
}

static void AnimTask_ShinySparkles(u8 taskId)
{
    u8 battler;
    u8 x, y;
    u8 spriteId;
    u16 counter;
    s16 state;
    u8 pan;

    if (gTasks[taskId].data[13] < 60)
    {
        gTasks[taskId].data[13]++;
        return;
    }

    if (gBattleSpritesDataPtr->animationData->particleCounter)
        return;

    counter = gTasks[taskId].data[10]++;
    if (counter & 3)
        return;

    battler = gTasks[taskId].data[0];
    x = GetBattlerSpriteCoord(battler, BATTLER_COORD_X);
    y = GetBattlerSpriteCoord(battler, BATTLER_COORD_Y);
    state = gTasks[taskId].data[11];
    if (state == 0)
    {
        spriteId = CreateSprite(&gWishStarSpriteTemplate, x, y, 5);
    }
    else if (state >= 0 && gTasks[taskId].data[11] < 4)
    {
        spriteId = CreateSprite(&gMiniTwinklingStarSpriteTemplate, x, y, 5);
        gSprites[spriteId].oam.tileNum += 4;
    }
    else
    {
        spriteId = CreateSprite(&gMiniTwinklingStarSpriteTemplate, x, y, 5);
        gSprites[spriteId].oam.tileNum += 5;
    }

    if (gTasks[taskId].data[1] == FALSE)
    {
        gSprites[spriteId].callback = SpriteCB_ShinySparkles_1;
    }
    else
    {
        gSprites[spriteId].callback = SpriteCB_ShinySparkles_2;
        gSprites[spriteId].x2 = -32;
        gSprites[spriteId].y2 = 32;
        gSprites[spriteId].invisible = TRUE;
        if (gTasks[taskId].data[11] == FALSE)
        {
			pan = GetBattlerSide(battler) == B_SIDE_PLAYER ? SOUND_PAN_ATTACKER : SOUND_PAN_TARGET;
            PlaySE12WithPanning(SE_SHINY, pan);
        }
    }

    gSprites[spriteId].data[0] = taskId;
    gTasks[taskId].data[11]++;
    if (spriteId != MAX_SPRITES)
        gTasks[taskId].data[12]++;

    if (gTasks[taskId].data[11] == 5)
        gTasks[taskId].func = AnimTask_ShinySparkles_WaitSparkles;
}

static void AnimTask_ShinySparkles_WaitSparkles(u8 taskId)
{
    u8 battler;

    if (gTasks[taskId].data[12] == FALSE)
    {
        if (gTasks[taskId].data[1] == TRUE)
        {
            battler = gTasks[taskId].data[0];
            gBattleSpritesDataPtr->healthBoxesData[battler].finishedShinyMonAnim = 1;
        }

        DestroyTask(taskId);
    }
}

static void SpriteCB_ShinySparkles_1(struct Sprite *sprite)
{
    sprite->x2 = Sin(sprite->data[1], 24);
    sprite->y2 = Cos(sprite->data[1], 24);
    sprite->data[1] += 12;
    if (sprite->data[1] > 0xFF)
    {
        gTasks[sprite->data[0]].data[12]--;
        FreeSpriteOamMatrix(sprite);
        DestroySprite(sprite);
    }
}

static void SpriteCB_ShinySparkles_2(struct Sprite *sprite)
{
    if (sprite->data[1] < 4)
    {
        sprite->data[1]++;
    }
    else
    {
        sprite->invisible = FALSE;
        sprite->x2 += 5;
        sprite->y2 -= 5;
        if (sprite->x2 > 32)
        {
            gTasks[sprite->data[0]].data[12]--;
            FreeSpriteOamMatrix(sprite);
            DestroySprite(sprite);
        }
    }
}

void AnimTask_LoadBaitGfx(u8 taskId)
{
    LoadCompressedSpriteSheetUsingHeap(&gBattleAnimPicTable[ANIM_TAG_SAFARI_BAIT - ANIM_SPRITES_START]);
    LoadCompressedSpritePaletteUsingHeap(&gBattleAnimPaletteTable[ANIM_TAG_SAFARI_BAIT - ANIM_SPRITES_START]);
    DestroyAnimVisualTask(taskId);
}

void AnimTask_FreeBaitGfx(u8 taskId)
{
    FreeSpriteTilesByTag(ANIM_TAG_SAFARI_BAIT);
    FreeSpritePaletteByTag(ANIM_TAG_SAFARI_BAIT);
    DestroyAnimVisualTask(taskId);
}

static void SpriteCB_SafariBaitOrRock_Init(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, 0);
    sprite->sTransl_Speed = 30;
    sprite->sTransl_DestX = GetBattlerSpriteCoord(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), BATTLER_COORD_X) + gBattleAnimArgs[2];
    sprite->sTransl_DestY = GetBattlerSpriteCoord(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), BATTLER_COORD_Y) + gBattleAnimArgs[3];
    sprite->sTransl_ArcAmpl = -32;
    InitAnimArcTranslation(sprite);
    gSprites[gBattlerSpriteIds[gBattleAnimAttacker]].callback = SpriteCB_PlayerThrowInit;
    sprite->callback = SpriteCB_SafariBaitOrRock_WaitPlayerThrow;
}

static void SpriteCB_SafariBaitOrRock_WaitPlayerThrow(struct Sprite *sprite)
{
    if (gSprites[gBattlerSpriteIds[gBattleAnimAttacker]].animCmdIndex == 1)
        sprite->callback = SpriteCB_SafariBaitOrRock_ArcFlight;
}

static void SpriteCB_SafariBaitOrRock_ArcFlight(struct Sprite *sprite)
{
    if (TranslateAnimHorizontalArc(sprite))
    {
        sprite->data[0] = 0;
        sprite->invisible = TRUE;
        sprite->callback = SpriteCB_SafariBaitOrRock_Finish;
    }
}

static void SpriteCB_SafariBaitOrRock_Finish(struct Sprite *sprite)
{
    if (gSprites[gBattlerSpriteIds[gBattleAnimAttacker]].animEnded)
    {
        if (++sprite->data[0] > 0)
        {
            StartSpriteAnim(&gSprites[gBattlerSpriteIds[gBattleAnimAttacker]], 0);
            DestroyAnimSprite(sprite);
        }
    }
}

void AnimTask_SafariOrGhost_DecideAnimSides(u8 taskId)
{
    switch (gBattleAnimArgs[0])
    {
    case 0:
        gBattleAnimAttacker = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        gBattleAnimTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
        break;
    case 1:
        gBattleAnimAttacker = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
        gBattleAnimTarget = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        break;
    }

    DestroyAnimVisualTask(taskId);
}

void AnimTask_SafariGetReaction(u8 taskId)
{
	gBattleAnimArgs[7] = gBattleCommunication[MULTISTRING_CHOOSER] > 2 ? 0 : gBattleCommunication[MULTISTRING_CHOOSER];
    DestroyAnimVisualTask(taskId);
}

void AnimTask_GetTrappedMoveAnimId(u8 taskId)
{
	gBattleAnimArgs[0] = GetTrappingIdByMove(gBattleSpritesDataPtr->animationData->animArg);
    DestroyAnimVisualTask(taskId);
}

void AnimTask_GetBattlersFromArg(u8 taskId)
{
    gBattleAnimAttacker = gBattleSpritesDataPtr->animationData->animArg;
    gBattleAnimTarget = gBattleSpritesDataPtr->animationData->animArg >> 8;
    DestroyAnimVisualTask(taskId);
}

#define ABILITY_POP_UP_POS_X_SLIDE 68
#define ABILITY_POP_UP_POS_X_DIFF  64

#define tOriginalX       data[0]
#define tHide            data[1]
#define tFrames          data[2]
#define tPos1x           data[3]
#define tBattler         data[4]
#define tIsMain          data[5]

static void RestoreAbilityPopUpOverwrittenPixels(u8 * tiles)
{
	u32 i, j, pixelCount;
	const u8 *src, *PopUpImg = (u8 *)gBattleAnimSpriteGfx_AbilityPopUp;
	u8 *dest, *buffer = AllocZeroed(0x800);
	
	CpuCopy32(tiles, buffer, 0x800);
	
	for (i = 0; i < ARRAY_COUNT(sAbilityPopUpOverwrittenPixelsTable); i++)
	{
		dest = buffer + sAbilityPopUpOverwrittenPixelsTable[i][0];
		src = PopUpImg + sAbilityPopUpOverwrittenPixelsTable[i][0];
		pixelCount = sAbilityPopUpOverwrittenPixelsTable[i][1];
		
		for (j = 0; j < pixelCount; j++)
			dest[j] = src[j];
	}
	CpuCopy32(buffer, tiles, 0x800);
	Free(buffer);
}

static u8* AddTextPrinterAndCreateWindowOnAbilityPopUp(const u8* str, u32 x, u32 y, u32 bgColor, u32 fgColor, u32 shadowColor, u32* WindowId)
{
	u8 *tiledata, *buffer = AllocZeroed(16);
	u8 i, colours[3] = {bgColor, fgColor, shadowColor};
	struct WindowTemplate template = {0};
	template.width = 16;
	template.height = 2;
	
	*WindowId = AddWindow(&template);
	tiledata = (u8*)(GetWindowAttribute(*WindowId, WINDOW_TILE_DATA));
	FillWindowPixelBuffer(*WindowId, PIXEL_FILL(bgColor));
	AddTextPrinterParameterized4(*WindowId, 0, x, y, 0, 0, colours, 0xFF, str);
	
	for (i = 0; i < 16; i++)
	{
		CpuCopy32(tiledata + 0x200 + (i * 16), buffer, 16);
		CpuCopy32(tiledata + 0x100 + (i * 16), tiledata + 0x200 + (i * 16), 16);
		CpuCopy32(buffer, tiledata + 0x100 + (i * 16), 16);
	}
	Free(buffer);
	
	return tiledata;
}

static void PutTextInAbilityPopUp(void * dest, u8 * WindowTile, s32 arg2, bool32 arg3)
{
	CpuCopy32(WindowTile + 256, dest + 256, arg2 * TILE_SIZE_4BPP);
	
	if (arg2 > 0)
	{
		do
		{
			if (arg3)
				CpuCopy32(WindowTile + 16, dest + 16, 16);
			else
				CpuCopy32(WindowTile + 20, dest + 20, 12);
			
			dest += 32, WindowTile += 32;
			arg2--;
		}
		while (arg2 != 0);
	}
}

static void AbilityPopUpPrinter(const u8 * str, u8 * tiledata, u8 * tiledata2, u32 y, u32 bgColor, u32 fgColor, u32 shadowColor)
{
	u32 WindowId;
	u8 *WindowTile = AddTextPrinterAndCreateWindowOnAbilityPopUp(str, 4, y, bgColor, fgColor, shadowColor, &WindowId);
	
	PutTextInAbilityPopUp(tiledata, WindowTile, 8, (y == 0));
	PutTextInAbilityPopUp(tiledata2, WindowTile + 0x200, 3, (y == 0));
	RemoveWindow(WindowId);
}

static void PrintBattlerAndAbilityOnAbilityPopUp(u8 battler, u8 sprite, u8 sprite2)
{
    int i;
    u8 pokemonName[POKEMON_NAME_LENGTH + 3] = {0};
    u8 *textPtr;
    u16 ability;
	
	GetMonData(GetBattlerIllusionPartyIndexPtr(battler), MON_DATA_NICKNAME, gStringVar1);
    
    for (i = 0; i < POKEMON_NAME_LENGTH; i++)
    {
        pokemonName[i] = gStringVar1[i];
        
        if (pokemonName[i] == EOS || i + 1 == POKEMON_NAME_LENGTH)
            break;
    }
    
    textPtr = pokemonName + i + 1;
    
    if (*(textPtr - 1) == EOS)
        --textPtr;
    textPtr[0] = CHAR_SGL_QUOT_RIGHT;
    textPtr[1] = CHAR_s;
    textPtr[2] = EOS;
    
    AbilityPopUpPrinter((const u8*) pokemonName, (void*)(OBJ_VRAM0) + (gSprites[sprite].oam.tileNum * TILE_SIZE_4BPP),
			(void*)(OBJ_VRAM0) + (gSprites[sprite2].oam.tileNum * TILE_SIZE_4BPP), 0, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GREEN, TEXT_COLOR_WHITE);
    
	if (gBattleStruct->abilityOverride[battler])
	{
		ability = gBattleStruct->abilityOverride[battler];
		gBattleStruct->abilityOverride[battler] = ABILITY_NONE;
	}
	else
		ability = gBattleMons[battler].ability;
    
    AbilityPopUpPrinter(gAbilityNames[ability], (void*)(OBJ_VRAM0) + (gSprites[sprite].oam.tileNum * TILE_SIZE_4BPP) + 256,
			(void*)(OBJ_VRAM0) + (gSprites[sprite2].oam.tileNum * TILE_SIZE_4BPP) + 256, 4, TEXT_COLOR_LIGHT_GREEN, TEXT_COLOR_LIGHT_BLUE, TEXT_COLOR_WHITE);
}

static void SpriteCB_AbilityPopUp(struct Sprite * sprite)
{
    if (!sprite->tHide)
    {
        if (sprite->tIsMain && ++sprite->tFrames == 4)
            PlaySE(SE_M_DOUBLE_TEAM);
        if ((GetBattlerSide(sprite->tBattler) != B_SIDE_PLAYER && (sprite->x -= 4) <= sprite->tOriginalX)
            || (GetBattlerSide(sprite->tBattler) == B_SIDE_PLAYER && (sprite->x += 4) >= sprite->tOriginalX))
        {
            sprite->x = sprite->tOriginalX;
            sprite->tHide = TRUE;
            sprite->tFrames = 1;
        }
    }
    else
    {
        if (sprite->tFrames == 0)
        {
            if ((GetBattlerSide(sprite->tBattler) != B_SIDE_PLAYER && (sprite->x += 4) >= sprite->tOriginalX + ABILITY_POP_UP_POS_X_SLIDE + 16)
                || (GetBattlerSide(sprite->tBattler) == B_SIDE_PLAYER && (sprite->x -= 4) <= sprite->tOriginalX - ABILITY_POP_UP_POS_X_SLIDE - 16))
            {
                gActiveAbilityPopUps &= ~(gBitTable[sprite->tBattler]);
				gAbilityPopUpIds[sprite->tBattler][0] = 0;
				gAbilityPopUpIds[sprite->tBattler][1] = 0;
				FreeSpriteTilesByTag(ANIM_TAG_ABILITY_POP_UP + sprite->tBattler);
				FreeSpritePaletteByTag(ANIM_TAG_ABILITY_POP_UP);
				DestroySprite(sprite);
            }
        }
    }
}

void AnimTask_CreateAbilityPopUp(u8 taskId)
{
	s16 x, y;
    u8 position, spriteId1, spriteId2, battler = GetBattlerForBattleScript(gBattlescriptCurrInstr[-1] & ~(ATKFC_REMOVE_POP_UP));
	const struct SpriteTemplate sSpriteTemplate_AbilityPopUp =
	{
		.tileTag = ANIM_TAG_ABILITY_POP_UP + battler,
		.paletteTag = ANIM_TAG_ABILITY_POP_UP,
		.oam = &sOamData_AbilityPopUp,
		.anims = sSpriteAnimTable_AbilityPopUp,
		.images = NULL,
		.affineAnims = gDummySpriteAffineAnimTable,
		.callback = SpriteCB_AbilityPopUp
	};
	const struct SpriteTemplate sSpriteTemplate_AbilityPopUp2 =
	{
		.tileTag = ANIM_TAG_ABILITY_POP_UP + battler,
		.paletteTag = ANIM_TAG_ABILITY_POP_UP,
		.oam = &sOamData_AbilityPopUp,
		.anims = sSpriteAnimTable_AbilityPopUp2,
		.images = NULL,
		.affineAnims = gDummySpriteAffineAnimTable,
		.callback = SpriteCB_AbilityPopUp
	};
	LoadSpriteSheet((const struct SpriteSheet*)&gBattleAnimPicTable[GET_TRUE_SPRITE_INDEX(ANIM_TAG_ABILITY_POP_UP + battler)]);
    LoadSpritePalette((const struct SpritePalette*)&gBattleAnimPaletteTable[GET_TRUE_SPRITE_INDEX(ANIM_TAG_ABILITY_POP_UP)]);
	
    gActiveAbilityPopUps |= gBitTable[battler];
    
	position = GetBattlerPosition(battler);
	
	x = sAbilityPopUpCoords[gBattleTypeFlags & BATTLE_TYPE_DOUBLE][position].x;
	y = sAbilityPopUpCoords[gBattleTypeFlags & BATTLE_TYPE_DOUBLE][position].y;
	
	if (GetBattlerSide(battler) == B_SIDE_PLAYER)
	{
		spriteId1 = CreateSprite(&sSpriteTemplate_AbilityPopUp, x - ABILITY_POP_UP_POS_X_SLIDE, y, 0);
		spriteId2 = CreateSprite(&sSpriteTemplate_AbilityPopUp2, x - ABILITY_POP_UP_POS_X_SLIDE + ABILITY_POP_UP_POS_X_DIFF, y, 0);
	}
	else
	{
		spriteId1 = CreateSprite(&sSpriteTemplate_AbilityPopUp, x + ABILITY_POP_UP_POS_X_SLIDE, y, 0);
		spriteId2 = CreateSprite(&sSpriteTemplate_AbilityPopUp2, x + ABILITY_POP_UP_POS_X_SLIDE + ABILITY_POP_UP_POS_X_DIFF, y, 0);
	}
    gSprites[spriteId1].tOriginalX = x;
    gSprites[spriteId2].tOriginalX = x + ABILITY_POP_UP_POS_X_DIFF;

    gAbilityPopUpIds[battler][0] = spriteId1;
    gAbilityPopUpIds[battler][1] = spriteId2;
	
    gSprites[spriteId1].tIsMain = TRUE;
    gSprites[spriteId1].tBattler = gSprites[spriteId2].tBattler = battler;
	
    StartSpriteAnim(&gSprites[spriteId1], 0);
    StartSpriteAnim(&gSprites[spriteId2], 0);
    
    PrintBattlerAndAbilityOnAbilityPopUp(battler, spriteId1, spriteId2);
    RestoreAbilityPopUpOverwrittenPixels((void*)(OBJ_VRAM0) + (gSprites[spriteId1].oam.tileNum * TILE_SIZE_4BPP));
    DestroyAnimVisualTask(taskId);
}

void AnimTask_DestroyAbilityPopUp(u8 taskId)
{
	u8 battler = GetBattlerForBattleScript(gBattlescriptCurrInstr[-1] & ~(ATKFC_REMOVE_POP_UP));
	
	if (gActiveAbilityPopUps & gBitTable[battler])
	{
		gSprites[gAbilityPopUpIds[battler][0]].tFrames = 0;
		gSprites[gAbilityPopUpIds[battler][1]].tFrames = 0;
	}
    DestroyAnimVisualTask(taskId);
}

void AnimTask_ItemThrow(u8 taskId)
{
	u8 spriteId = AddItemIconObject(ANIM_TAG_ITEM_BAG, ANIM_TAG_ITEM_BAG, gLastUsedItem);
	struct Sprite *sprite;
	
	if (spriteId != MAX_SPRITES)
	{
		sprite = &gSprites[spriteId];
		
		if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER) // get initial coords
		{
			sprite->x = 249;
			sprite->y = 29;
		}
		else
		{
			sprite->x = 0;
			sprite->y = 81;
		}
		sprite->oam.priority = 0;
		sprite->data[0] = 40; // duration
		sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X_2); // dest X
		sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET) - 10; // dest Y
		sprite->data[5] = -35; // arc translation
		InitAnimArcTranslation(sprite);
		sprite->callback = DestroyAnimSpriteAfterHorizontalTranslation;
		DestroyTask(taskId);
	}
	else
		DestroyAnimVisualTask(taskId);
}

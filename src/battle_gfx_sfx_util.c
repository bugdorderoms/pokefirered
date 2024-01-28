#include "global.h"
#include "gflib.h"
#include "m4a.h"
#include "task.h"
#include "graphics.h"
#include "decompress.h"
#include "palette.h"
#include "sprite.h"
#include "data.h"
#include "util.h"
#include "party_menu.h"
#include "battle.h"
#include "battle_main.h"
#include "battle_anim.h"
#include "battle_interface.h"
#include "constants/battle_anim.h"
#include "constants/moves.h"
#include "constants/songs.h"

static bool8 ShouldAnimBeDoneRegardlessOfSubsitute(u8 animId);
static void Task_ClearBitWhenBattleTableAnimDone(u8 taskId);
static void Task_ClearBitWhenSpecialAnimDone(u8 taskId);
static void ClearSpritesBattlerHealthboxAnimData(void);

static const struct CompressedSpriteSheet sSpriteSheet_SinglesPlayerHealthbox =
{
    .data = gHealthboxSinglesPlayerGfx,
    .size = 0x1000,
    .tag = TAG_HEALTHBOX_PLAYER1_TILE,
};

static const struct CompressedSpriteSheet sSpriteSheet_SinglesOpponentHealthbox =
{
    .data = gHealthboxSinglesOpponentGfx,
    .size = 0x1000,
    .tag = TAG_HEALTHBOX_OPPONENT1_TILE,
};

static const struct CompressedSpriteSheet sSpriteSheets_DoublesPlayerHealthbox[2] =
{
    {
        .data = gHealthboxDoublesPlayerGfx,
        .size = 0x800,
        .tag = TAG_HEALTHBOX_PLAYER1_TILE,
    },
    {
        .data = gHealthboxDoublesPlayerGfx,
        .size = 0x800,
        .tag = TAG_HEALTHBOX_PLAYER2_TILE,
    },
};

static const struct CompressedSpriteSheet sSpriteSheets_DoublesOpponentHealthbox[2] =
{
    {
        .data = gHealthboxDoublesOpponentGfx,
        .size = 0x800,
        .tag = TAG_HEALTHBOX_OPPONENT1_TILE,
    },
    {
        .data = gHealthboxDoublesOpponentGfx,
        .size = 0x800,
        .tag = TAG_HEALTHBOX_OPPONENT2_TILE,
    },
};

static const struct CompressedSpriteSheet sSpriteSheet_SafariHealthbox =
{
    .data = gHealthboxSafariGfx,
    .size = 0x1000,
    .tag = TAG_HEALTHBOX_SAFARI_TILE,
};

static const struct CompressedSpriteSheet sSpriteSheets_HealthBar[MAX_BATTLERS_COUNT] =
{
    {
        .data = gBlankGfxCompressed,
        .size = 0x100,
        .tag = TAG_HEALTHBAR_PLAYER1_TILE,
    },
    {
        .data = gBlankGfxCompressed,
        .size = 0x120,
        .tag = TAG_HEALTHBAR_OPPONENT1_TILE,
    },
    {
        .data = gBlankGfxCompressed,
        .size = 0x100,
        .tag = TAG_HEALTHBAR_PLAYER2_TILE,
    },
    {
        .data = gBlankGfxCompressed,
        .size = 0x120,
        .tag = TAG_HEALTHBAR_OPPONENT2_TILE,
    },
};

static const struct SpritePalette sSpritePalettes_HealthBoxHealthBar[2] =
{
    {
        .data = gBattleInterface_BallStatusBarPal,
        .tag = TAG_HEALTHBOX_PAL,
    },
    {
        .data = gBattleInterface_BallDisplayPal,
        .tag = TAG_HEALTHBAR_PAL,
    },
};

void AllocateBattleSpritesData(void)
{
    gBattleSpritesDataPtr = AllocZeroed(sizeof(struct BattleSpriteData));
    gBattleSpritesDataPtr->battlerData = AllocZeroed(sizeof(struct BattleSpriteInfo) * MAX_BATTLERS_COUNT);
    gBattleSpritesDataPtr->healthBoxesData = AllocZeroed(sizeof(struct BattleHealthboxInfo) * MAX_BATTLERS_COUNT);
    gBattleSpritesDataPtr->animationData = AllocZeroed(sizeof(struct BattleAnimationInfo));
    gBattleSpritesDataPtr->battleBars = AllocZeroed(sizeof(struct BattleBarInfo) * MAX_BATTLERS_COUNT);
}

void FreeBattleSpritesData(void)
{
    if (gBattleSpritesDataPtr)
    {
        FREE_AND_SET_NULL(gBattleSpritesDataPtr->battleBars);
        FREE_AND_SET_NULL(gBattleSpritesDataPtr->animationData);
        FREE_AND_SET_NULL(gBattleSpritesDataPtr->healthBoxesData);
        FREE_AND_SET_NULL(gBattleSpritesDataPtr->battlerData);
        FREE_AND_SET_NULL(gBattleSpritesDataPtr);
    }
}

void SpriteCB_WaitForBattlerBallReleaseAnim(struct Sprite *sprite)
{
    u8 spriteId = sprite->data[1];

    if (!gSprites[spriteId].affineAnimEnded || gSprites[spriteId].invisible)
        return;
    if (gSprites[spriteId].animPaused)
        gSprites[spriteId].animPaused = 0;
    else if (gSprites[spriteId].animEnded)
    {
        gSprites[spriteId].callback = SpriteCB_SetToDummy3;
        StartSpriteAffineAnim(&gSprites[spriteId], 0);
        sprite->callback = SpriteCallbackDummy;
    }
}

void SpriteCB_TrainerSlideIn(struct Sprite *sprite)
{
    if (!(gIntroSlideFlags & 1))
    {
        sprite->x2 += sprite->data[0];
        if (sprite->x2 == 0)
            sprite->callback = SpriteCallbackDummy;
    }
}

void InitAndLaunchChosenStatusAnimation(u8 battlerId, u8 animStatusId, u32 status)
{
	switch (animStatusId)
	{
		case ID_STATUS1:
			if (status & STATUS1_FREEZE)
				LaunchStatusAnimation(battlerId, B_ANIM_STATUS_FRZ);
			else if (status & STATUS1_PSN_ANY)
				LaunchStatusAnimation(battlerId, B_ANIM_STATUS_PSN);
			else if (status & STATUS1_BURN)
				LaunchStatusAnimation(battlerId, B_ANIM_STATUS_BRN);
			else if (status & STATUS1_SLEEP)
				LaunchStatusAnimation(battlerId, B_ANIM_STATUS_SLP);
			else if (status & STATUS1_PARALYSIS)
				LaunchStatusAnimation(battlerId, B_ANIM_STATUS_PRZ);
			else // no animation
				return;
			break;
		case ID_STATUS2:
			if (status & STATUS2_INFATUATION)
				LaunchStatusAnimation(battlerId, B_ANIM_STATUS_INFATUATION);
			else if (status & STATUS2_CONFUSION)
				LaunchStatusAnimation(battlerId, B_ANIM_STATUS_CONFUSION);
			else if (status & STATUS2_CURSED)
				LaunchStatusAnimation(battlerId, B_ANIM_STATUS_CURSED);
			else if (status & STATUS2_NIGHTMARE)
				LaunchStatusAnimation(battlerId, B_ANIM_STATUS_NIGHTMARE);
			else // no animation
				return;
			break;
	}
	gBattleSpritesDataPtr->healthBoxesData[battlerId].statusAnimActive = TRUE;
}

#define tBattlerId data[0]

bool8 TryHandleLaunchBattleTableAnimation(u8 activeBattler, u8 atkBattler, u8 defBattler, u8 tableId, u16 argument)
{
    if (gBattleSpritesDataPtr->battlerData[activeBattler].behindSubstitute && !ShouldAnimBeDoneRegardlessOfSubsitute(tableId))
    {
        return TRUE;
    }
    else if (gBattleSpritesDataPtr->battlerData[activeBattler].behindSubstitute && tableId == B_ANIM_SUBSTITUTE_FADE
	&& gSprites[gBattlerSpriteIds[activeBattler]].invisible)
    {
        LoadBattleMonGfxAndAnimate(activeBattler, TRUE, gBattlerSpriteIds[activeBattler]);
        ClearBehindSubstituteBit(activeBattler);
        return TRUE;
    }
    else if (tableId == B_ANIM_ILLUSION_OFF)
    {
        gBattleStruct->illusion[activeBattler].broken = TRUE;
		gBattleStruct->illusion[activeBattler].on = FALSE;
    }
    gBattleAnimAttacker = atkBattler;
    gBattleAnimTarget = defBattler;
    gBattleSpritesDataPtr->animationData->animArg = argument;
    LaunchBattleAnimation(ANIM_TYPE_GENERAL, tableId);
    gTasks[CreateTask(Task_ClearBitWhenBattleTableAnimDone, 10)].tBattlerId = activeBattler;
    gBattleSpritesDataPtr->healthBoxesData[activeBattler].animFromTableActive = 1;
    return FALSE;
}

static void Task_ClearBitWhenBattleTableAnimDone(u8 taskId)
{
    gAnimScriptCallback();
    if (!gAnimScriptActive)
    {
        gBattleSpritesDataPtr->healthBoxesData[gTasks[taskId].tBattlerId].animFromTableActive = 0;
        DestroyTask(taskId);
    }
}

static bool8 ShouldAnimBeDoneRegardlessOfSubsitute(u8 animId)
{
    switch (animId)
    {
    case B_ANIM_SUBSTITUTE_FADE:
    case B_ANIM_RAIN_CONTINUES:
    case B_ANIM_SUN_CONTINUES:
    case B_ANIM_SANDSTORM_CONTINUES:
    case B_ANIM_HAIL_CONTINUES:
    case B_ANIM_SNATCH_MOVE:
	case B_ANIM_FOG_CONTINUES:
	case B_ANIM_ITEM_THROW:
	case B_ANIM_STRONG_WINDS_CONTINUE:
	case B_ANIM_LOAD_ABILITY_POP_UP:
	case B_ANIM_REMOVE_ABILITY_POP_UP:
	case B_ANIM_COMMANDER:
        return TRUE;
    }
	return FALSE;
}

void InitAndLaunchSpecialAnimation(u8 activeBattler, u8 atkBattler, u8 defBattler, u8 tableId)
{
    u8 taskId;

    gBattleAnimAttacker = atkBattler;
    gBattleAnimTarget = defBattler;
    LaunchBattleAnimation(ANIM_TYPE_SPECIAL, tableId);
    taskId = CreateTask(Task_ClearBitWhenSpecialAnimDone, 10);
    gTasks[taskId].tBattlerId = activeBattler;
    gBattleSpritesDataPtr->healthBoxesData[gTasks[taskId].tBattlerId].specialAnimActive = 1;
}

static void Task_ClearBitWhenSpecialAnimDone(u8 taskId)
{
    gAnimScriptCallback();
    if (!gAnimScriptActive)
    {
        gBattleSpritesDataPtr->healthBoxesData[gTasks[taskId].tBattlerId].specialAnimActive = 0;
        DestroyTask(taskId);
    }
}

bool8 IsBattleSEPlaying(u8 battlerId)
{
    if (IsSEPlaying())
    {
        ++gBattleSpritesDataPtr->healthBoxesData[battlerId].soundTimer;
        
        if (gBattleSpritesDataPtr->healthBoxesData[battlerId].soundTimer < 30)
            return TRUE;
        m4aMPlayStop(&gMPlayInfo_SE1);
        m4aMPlayStop(&gMPlayInfo_SE2);
    }
	gBattleSpritesDataPtr->healthBoxesData[battlerId].soundTimer = 0;
	return FALSE;
}

void BattleLoadMonSpriteGfx(u8 battlerId)
{
	struct Pokemon *mon = GetBattlerIllusionPartyIndexPtr(battlerId);
    u32 currentPersonality;
    u16 species;
    u16 paletteOffset;
    void *buffer;
	const u32 *frontSpritePal;
	
    if (gBattleSpritesDataPtr->battlerData[battlerId].transformSpecies == SPECIES_NONE)
    {
        species = GetMonData(mon, MON_DATA_SPECIES);
        currentPersonality = GetMonData(mon, MON_DATA_PERSONALITY);
		frontSpritePal = GetMonFrontSpritePal(mon);
    }
    else
    {
        species = gBattleSpritesDataPtr->battlerData[battlerId].transformSpecies;
        currentPersonality = gTransformedPersonalities[battlerId];
		frontSpritePal = GetMonSpritePalFromSpecies(species, gTransformedShinies[battlerId]);
    }
	HandleLoadSpecialPokePic(GetBattlerSide(battlerId) == B_SIDE_OPPONENT ? &gMonFrontPicTable[species] : &gMonBackPicTable[species],
	gMonSpritesGfxPtr->sprites[GetBattlerPosition(battlerId)], species, currentPersonality);
	
    paletteOffset = 0x100 + battlerId * 16;
	
    buffer = AllocZeroed(0x400);
    LZDecompressWram(frontSpritePal, buffer);
    LoadPalette(buffer, paletteOffset, 0x20);
    LoadPalette(buffer, 0x80 + battlerId * 16, 0x20);
    Free(buffer);
	
    // transform's pink color
    if (gBattleSpritesDataPtr->battlerData[battlerId].transformSpecies != SPECIES_NONE)
    {
        BlendPalette(paletteOffset, 16, 6, RGB_WHITE);
        CpuCopy32(gPlttBufferFaded + paletteOffset, gPlttBufferUnfaded + paletteOffset, 32);
    }
}

void DecompressGhostFrontPic(u8 battlerId)
{
    void *buffer;
    
    LZDecompressWram(gGhostFrontPic, gMonSpritesGfxPtr->sprites[GetBattlerPosition(battlerId)]);
    buffer = AllocZeroed(0x400);
    LZDecompressWram(gGhostPalette, buffer);
    LoadPalette(buffer, 0x100 + 16 * battlerId, 0x20);
    LoadPalette(buffer, 0x80 + 16 * battlerId, 0x20);
    Free(buffer);
}

void DecompressTrainerFrontPic(u16 frontPicId, u8 battlerId)
{
    struct SpriteSheet sheet;
    u8 position = GetBattlerPosition(battlerId);

    DecompressPicFromTable(&gTrainerFrontPicTable[frontPicId], gMonSpritesGfxPtr->sprites[position], SPECIES_NONE);
    sheet.data = gMonSpritesGfxPtr->sprites[position];
    sheet.size = gTrainerFrontPicTable[frontPicId].size;
    sheet.tag = gTrainerFrontPicTable[frontPicId].tag;
    LoadSpriteSheet(&sheet);
    LoadCompressedSpritePaletteUsingHeap(&gTrainerFrontPicPaletteTable[frontPicId]);
}

void DecompressTrainerBackPalette(u16 index, u8 palette)
{
    LoadCompressedPalette(gTrainerBackPicPaletteTable[index].data, (palette + 16) * 16, 0x20);
}

void FreeTrainerFrontPicPaletteAndTile(u16 frontPicId)
{
    FreeSpritePaletteByTag(gTrainerFrontPicPaletteTable[frontPicId].tag);
    FreeSpriteTilesByTag(gTrainerFrontPicTable[frontPicId].tag);
}

bool8 BattleLoadAllHealthBoxesGfx(u8 state)
{
    bool8 retVal = FALSE;

    if (state)
    {
        if (state == 1)
        {
            LoadSpritePalette(&sSpritePalettes_HealthBoxHealthBar[0]);
            LoadSpritePalette(&sSpritePalettes_HealthBoxHealthBar[1]);
        }
        else if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
        {
            if (state == 2)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
                    LoadCompressedSpriteSheetUsingHeap(&sSpriteSheet_SafariHealthbox);
                else
                    LoadCompressedSpriteSheetUsingHeap(&sSpriteSheet_SinglesPlayerHealthbox);
            }
            else if (state == 3)
                LoadCompressedSpriteSheetUsingHeap(&sSpriteSheet_SinglesOpponentHealthbox);
            else if (state == 4)
                LoadCompressedSpriteSheetUsingHeap(&sSpriteSheets_HealthBar[gBattlerPositions[0]]);
            else if (state == 5)
                LoadCompressedSpriteSheetUsingHeap(&sSpriteSheets_HealthBar[gBattlerPositions[1]]);
            else
                retVal = TRUE;
        }
        else
        {
            if (state == 2)
                LoadCompressedSpriteSheetUsingHeap(&sSpriteSheets_DoublesPlayerHealthbox[0]);
            else if (state == 3)
                LoadCompressedSpriteSheetUsingHeap(&sSpriteSheets_DoublesPlayerHealthbox[1]);
            else if (state == 4)
                LoadCompressedSpriteSheetUsingHeap(&sSpriteSheets_DoublesOpponentHealthbox[0]);
            else if (state == 5)
                LoadCompressedSpriteSheetUsingHeap(&sSpriteSheets_DoublesOpponentHealthbox[1]);
            else if (state == 6)
                LoadCompressedSpriteSheetUsingHeap(&sSpriteSheets_HealthBar[gBattlerPositions[0]]);
            else if (state == 7)
                LoadCompressedSpriteSheetUsingHeap(&sSpriteSheets_HealthBar[gBattlerPositions[1]]);
            else if (state == 8)
                LoadCompressedSpriteSheetUsingHeap(&sSpriteSheets_HealthBar[gBattlerPositions[2]]);
            else if (state == 9)
                LoadCompressedSpriteSheetUsingHeap(&sSpriteSheets_HealthBar[gBattlerPositions[3]]);
            else
                retVal = TRUE;
        }
    }
    return retVal;
}

void LoadBattleBarGfx(void)
{
    LZDecompressWram(gInterfaceGfx_HPNumbers, gMonSpritesGfxPtr->barFontGfx);
}

bool8 BattleInitAllSprites(u8 *state, u8 *battlerId)
{
    switch (*state)
    {
    case 0:
        ClearSpritesBattlerHealthboxAnimData();
        ++*state;
        break;
    case 1:
        if (!BattleLoadAllHealthBoxesGfx(*battlerId))
        {
            ++*battlerId;
        }
        else
        {
            *battlerId = 0;
            ++*state;
        }
        break;
    case 2:
        ++*state;
        break;
    case 3:
        if ((gBattleTypeFlags & BATTLE_TYPE_SAFARI) && *battlerId == 0)
            gHealthboxSpriteIds[*battlerId] = CreateSafariPlayerHealthboxSprites();
        else
            gHealthboxSpriteIds[*battlerId] = CreateBattlerHealthboxSprites(*battlerId);
		
        if (++*battlerId == gBattlersCount)
        {
            *battlerId = 0;
            ++*state;
        }
        break;
    case 4:
        InitBattlerHealthboxCoords(*battlerId);
        if (++*battlerId == gBattlersCount)
        {
            *battlerId = 0;
            ++*state;
        }
        break;
    case 5:
        if (GetBattlerSide(*battlerId) == B_SIDE_PLAYER)
        {
            if (!(gBattleTypeFlags & BATTLE_TYPE_SAFARI))
                UpdateHealthboxAttribute(*battlerId, HEALTHBOX_ALL);
        }
        else
            UpdateHealthboxAttribute(*battlerId, HEALTHBOX_ALL);
		
        SetHealthboxSpriteInvisible(gHealthboxSpriteIds[*battlerId]);
		
        if (++*battlerId == gBattlersCount)
        {
            *battlerId = 0;
            ++*state;
        }
        break;
    case 6:
        LoadAndCreateEnemyShadowSprites();
        BufferBattlePartyCurrentOrder();
        return TRUE;
    }
    return FALSE;
}

void ClearSpritesHealthboxAnimData(void)
{
    memset(gBattleSpritesDataPtr->healthBoxesData, 0, sizeof(struct BattleHealthboxInfo) * MAX_BATTLERS_COUNT);
    memset(gBattleSpritesDataPtr->animationData, 0, sizeof(struct BattleAnimationInfo));
}

static void ClearSpritesBattlerHealthboxAnimData(void)
{
    ClearSpritesHealthboxAnimData();
    memset(gBattleSpritesDataPtr->battlerData, 0, sizeof(struct BattleSpriteInfo) * MAX_BATTLERS_COUNT);
}

void CopyAllBattleSpritesInvisibilities(void)
{
    s32 i;

    for (i = 0; i < gBattlersCount; ++i)
		CopyBattleSpriteInvisibility(i);
}

void CopyBattleSpriteInvisibility(u8 battlerId)
{
    gBattleSpritesDataPtr->battlerData[battlerId].invisible = gSprites[gBattlerSpriteIds[battlerId]].invisible;
}

void HandleSpeciesGfxDataChange(u8 battlerAtk, u8 battlerDef, u8 flags)
{
	struct Pokemon *atkMon = GetBattlerPartyIndexPtr(battlerAtk);
	struct Pokemon *defMon = GetBattlerPartyIndexPtr(battlerDef);
	u8 atkPosition = GetBattlerPosition(battlerAtk);
	void *buffer;
	u16 paletteOffset, targetSpecies;
	bool8 isShiny;
	
	targetSpecies = GetMonData((flags & SPECIESGFX_FLAG_IS_GHOST) ? atkMon : defMon, MON_DATA_SPECIES);
	isShiny = GetMonData((flags & SPECIESGFX_FLAG_NO_TRANSFORM_PALFADE) ? atkMon : defMon, MON_DATA_IS_SHINY);
	
	HandleLoadSpecialPokePic(GetBattlerSide(battlerAtk) == B_SIDE_OPPONENT ? &gMonFrontPicTable[targetSpecies] : &gMonBackPicTable[targetSpecies],
	gMonSpritesGfxPtr->sprites[atkPosition], targetSpecies, GetMonData(atkMon, MON_DATA_PERSONALITY));
	
	DmaCopy32(3, gMonSpritesGfxPtr->sprites[atkPosition], (void *)(VRAM + 0x10000 + gSprites[gBattlerSpriteIds[battlerAtk]].oam.tileNum * 32), 0x800);
	
	buffer = AllocZeroed(0x400);
	LZDecompressWram(GetMonSpritePalFromSpecies(targetSpecies, isShiny), buffer);
	paletteOffset = 0x100 + battlerAtk * 16;
	LoadPalette(buffer, paletteOffset, 32);
	Free(buffer);
	
	if (!(flags & (SPECIESGFX_FLAG_IS_GHOST | SPECIESGFX_FLAG_NO_TRANSFORM_PALFADE)))
	{
		BlendPalette(paletteOffset, 16, 6, RGB_WHITE);
		CpuCopy32(gPlttBufferFaded + paletteOffset, gPlttBufferUnfaded + paletteOffset, 32);
		gBattleSpritesDataPtr->battlerData[battlerAtk].transformSpecies = targetSpecies;
	}
	gSprites[gBattlerSpriteIds[battlerAtk]].y = GetBattlerSpriteDefault_Y(battlerAtk);
	StartSpriteAnim(&gSprites[gBattlerSpriteIds[battlerAtk]], 0);
	
	if (flags & SPECIESGFX_FLAG_IS_GHOST)
	{
		SetMonData(atkMon, MON_DATA_NICKNAME, gSpeciesNames[targetSpecies]);
        UpdateHealthboxAttribute(battlerAtk, HEALTHBOX_NICK);
        TryAddPokeballIconToHealthbox(gHealthboxSpriteIds[battlerAtk], TRUE);
	}
}

void BattleLoadSubstituteOrMonSpriteGfx(u8 battlerId, bool8 loadMonSprite)
{
    u8 position;
    s32 i;

    if (!loadMonSprite)
    {
        position = GetBattlerPosition(battlerId);
		LZDecompressVram(GetBattlerSide(battlerId) != B_SIDE_PLAYER ? gSubstituteDollGfx : gSubstituteDollTilemap, gMonSpritesGfxPtr->sprites[position]);
		
        for (i = 1; i < 4; ++i)
        {
            u8 (*ptr)[4][0x800] = gMonSpritesGfxPtr->sprites[position];

            ++ptr;
            --ptr;
            DmaCopy32Defvars(3, (*ptr)[0], (*ptr)[i], 0x800);
        }
        LoadCompressedPalette(gSubstituteDollPal, (battlerId * 16) + 0x100, 32);
    }
    else
		BattleLoadMonSpriteGfx(battlerId);
}

void LoadBattleMonGfxAndAnimate(u8 battlerId, bool8 loadMonSprite, u8 spriteId)
{
    BattleLoadSubstituteOrMonSpriteGfx(battlerId, loadMonSprite);
    StartSpriteAnim(&gSprites[spriteId], 0);
	gSprites[spriteId].y = loadMonSprite ? GetBattlerSpriteDefault_Y(battlerId) : GetSubstituteSpriteDefault_Y(battlerId);
}

void TrySetBehindSubstituteSpriteBit(u8 battlerId, u16 move)
{
    if (move == MOVE_SUBSTITUTE)
        gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute = 1;
}

void ClearBehindSubstituteBit(u8 battlerId)
{
    gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute = 0;
}

void HandleLowHpMusicChange(struct Pokemon *mon, u8 battlerId)
{
    if (GetHPBarLevel(GetMonData(mon, MON_DATA_HP), GetMonData(mon, MON_DATA_MAX_HP)) == HP_BAR_RED)
    {
        if (!gBattleSpritesDataPtr->battlerData[battlerId].lowHpSong)
        {
            if (!gBattleSpritesDataPtr->battlerData[BATTLE_PARTNER(battlerId)].lowHpSong)
                PlaySE(SE_LOW_HEALTH);
            gBattleSpritesDataPtr->battlerData[battlerId].lowHpSong = 1;
        }
    }
    else
    {
        gBattleSpritesDataPtr->battlerData[battlerId].lowHpSong = 0;
        if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
            m4aSongNumStop(SE_LOW_HEALTH);
        else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !gBattleSpritesDataPtr->battlerData[BATTLE_PARTNER(battlerId)].lowHpSong)
            m4aSongNumStop(SE_LOW_HEALTH);
    }
}

void BattleStopLowHpSound(void)
{
    u8 playerBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);

    gBattleSpritesDataPtr->battlerData[playerBattler].lowHpSong = 0;
    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
        gBattleSpritesDataPtr->battlerData[BATTLE_PARTNER(playerBattler)].lowHpSong = 0;
    m4aSongNumStop(SE_LOW_HEALTH);
}

void HandleBattleLowHpMusicChange(void)
{
    if (gMain.inBattle)
    {
        u8 playerBattler1 = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        u8 playerBattler2 = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
        u8 battler1PartyId = GetPartyIdFromBattlePartyId(gBattlerPartyIndexes[playerBattler1]);
        u8 battler2PartyId = GetPartyIdFromBattlePartyId(gBattlerPartyIndexes[playerBattler2]);

        if (GetMonData(&gPlayerParty[battler1PartyId], MON_DATA_HP))
            HandleLowHpMusicChange(&gPlayerParty[battler1PartyId], playerBattler1);
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && GetMonData(&gPlayerParty[battler2PartyId], MON_DATA_HP))
            HandleLowHpMusicChange(&gPlayerParty[battler2PartyId], playerBattler2);
    }
}

void SetBattlerSpriteAffineMode(u8 affineMode)
{
    s32 i;

    for (i = 0; i < gBattlersCount; ++i)
    {
        if (IsBattlerSpritePresent(i))
        {
            gSprites[gBattlerSpriteIds[i]].oam.affineMode = affineMode;
            if (affineMode == ST_OAM_AFFINE_OFF)
            {
                gBattleSpritesDataPtr->healthBoxesData[i].matrixNum = gSprites[gBattlerSpriteIds[i]].oam.matrixNum;
                gSprites[gBattlerSpriteIds[i]].oam.matrixNum = 0;
            }
            else
            {
                gSprites[gBattlerSpriteIds[i]].oam.matrixNum = gBattleSpritesDataPtr->healthBoxesData[i].matrixNum;
            }
        }
    }
}

static void LoadBattlerShadowSprite(u8 position)
{
	u8 battlerId = GetBattlerAtPosition(position);
	
	gBattleSpritesDataPtr->healthBoxesData[battlerId].shadowSpriteId = CreateSprite(&gSpriteTemplate_EnemyShadow, GetBattlerSpriteCoord(battlerId, 0), GetBattlerSpriteCoord(battlerId, 1) + 29, 0xC8);
    gSprites[gBattleSpritesDataPtr->healthBoxesData[battlerId].shadowSpriteId].data[0] = battlerId;
}

void LoadAndCreateEnemyShadowSprites(void)
{
    LoadCompressedSpriteSheetUsingHeap(&gSpriteSheet_EnemyShadow);
	
	LoadBattlerShadowSprite(B_POSITION_OPPONENT_LEFT);
	
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
		LoadBattlerShadowSprite(B_POSITION_OPPONENT_RIGHT);
}

static void SpriteCB_EnemyShadow(struct Sprite *shadowSprite)
{
    bool8 invisible = FALSE;
    u8 battlerId = shadowSprite->tBattlerId;
    struct Sprite *battlerSprite = &gSprites[gBattlerSpriteIds[battlerId]];

    if (!battlerSprite->inUse || !IsBattlerSpritePresent(battlerId))
    {
        shadowSprite->callback = SpriteCB_SetInvisible;
        return;
    }
    if (gAnimScriptActive || battlerSprite->invisible || (gBattleSpritesDataPtr->battlerData[battlerId].transformSpecies != SPECIES_NONE
		&& gEnemyMonElevation[gBattleSpritesDataPtr->battlerData[battlerId].transformSpecies] == 0) || gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute)
        invisible = TRUE;
		
    shadowSprite->x = battlerSprite->x;
    shadowSprite->x2 = battlerSprite->x2;
    shadowSprite->invisible = invisible;
}

void SpriteCB_SetInvisible(struct Sprite *sprite)
{
    sprite->invisible = TRUE;
}

void SetBattlerShadowSpriteCallback(u8 battlerId, u16 species)
{
    // The player's shadow is never seen.
    if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
        return;

    if (gBattleSpritesDataPtr->battlerData[battlerId].transformSpecies != SPECIES_NONE)
        species = gBattleSpritesDataPtr->battlerData[battlerId].transformSpecies;
	
	gSprites[gBattleSpritesDataPtr->healthBoxesData[battlerId].shadowSpriteId].callback = gEnemyMonElevation[species] != 0 ? SpriteCB_EnemyShadow : SpriteCB_SetInvisible;
}

void HideBattlerShadowSprite(u8 battlerId)
{
    gSprites[gBattleSpritesDataPtr->healthBoxesData[battlerId].shadowSpriteId].callback = SpriteCB_SetInvisible;
}

// Low-level function that sets specific interface tiles' palettes, overwriting any pixel with value 0.
void BattleInterfaceSetWindowPals(void)
{
    // 9 tiles at 0x06000240
    u16 *vramPtr = (u16 *)(BG_VRAM + 0x240);
    s32 i, j;

    for (i = 0; i < 9; ++i)
    {
        for (j = 0; j < 16; ++vramPtr, ++j)
        {
            if (!(*vramPtr & 0xF000))
                *vramPtr |= 0xF000;
            if (!(*vramPtr & 0x0F00))
                *vramPtr |= 0x0F00;
            if (!(*vramPtr & 0x00F0))
                *vramPtr |= 0x00F0;
            if (!(*vramPtr & 0x000F))
                *vramPtr |= 0x000F;
        }
    }

    // 18 tiles at 0x06000600
    vramPtr = (u16 *)(BG_VRAM + 0x600);
	
    for (i = 0; i < 18; ++i)
    {
        for (j = 0; j < 16; ++vramPtr, ++j)
        {
            if (!(*vramPtr & 0xF000))
                *vramPtr |= 0x6000;
            if (!(*vramPtr & 0x0F00))
                *vramPtr |= 0x0600;
            if (!(*vramPtr & 0x00F0))
                *vramPtr |= 0x0060;
            if (!(*vramPtr & 0x000F))
                *vramPtr |= 0x0006;
        }
    }
}

void ClearTemporarySpeciesSpriteData(u8 battlerId, bool8 dontClearSubstitute)
{
    gBattleSpritesDataPtr->battlerData[battlerId].transformSpecies = SPECIES_NONE;
	
    if (!dontClearSubstitute)
        ClearBehindSubstituteBit(battlerId);
}

void AllocateMonSpritesGfx(void)
{
    u8 i, j;

    gMonSpritesGfxPtr = NULL;
    gMonSpritesGfxPtr = AllocZeroed(sizeof(*gMonSpritesGfxPtr));
    gMonSpritesGfxPtr->firstDecompressed = AllocZeroed(0x8000);
    for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
    {
        gMonSpritesGfxPtr->sprites[i] = gMonSpritesGfxPtr->firstDecompressed + (i * 0x2000);
        *(gMonSpritesGfxPtr->templates + i) = gSpriteTemplates_Battlers[i];

        for (j = 0; j < 4; ++j)
        {
            gMonSpritesGfxPtr->images[i][j].data = gMonSpritesGfxPtr->sprites[i] + (j * 0x800);
            gMonSpritesGfxPtr->images[i][j].size = 0x800;
        }
        gMonSpritesGfxPtr->templates[i].images = gMonSpritesGfxPtr->images[i];
    }
    gMonSpritesGfxPtr->barFontGfx = AllocZeroed(0x1000);
}

void FreeMonSpritesGfx(void)
{
    if (gMonSpritesGfxPtr == NULL)
        return;
    if (gMonSpritesGfxPtr->multiUseBuffer != NULL)
        FREE_AND_SET_NULL(gMonSpritesGfxPtr->multiUseBuffer);
    FREE_AND_SET_NULL(gMonSpritesGfxPtr->barFontGfx);
    FREE_AND_SET_NULL(gMonSpritesGfxPtr->firstDecompressed);
    gMonSpritesGfxPtr->sprites[0] = NULL;
    gMonSpritesGfxPtr->sprites[1] = NULL;
    gMonSpritesGfxPtr->sprites[2] = NULL;
    gMonSpritesGfxPtr->sprites[3] = NULL;
    FREE_AND_SET_NULL(gMonSpritesGfxPtr);
}

bool32 ShouldPlayNormalPokeCry(struct Pokemon *mon)
{
    if ((GetMonData(mon, MON_DATA_STATUS) & (STATUS1_ANY | STATUS1_TOXIC_COUNTER)))
        return FALSE;
	else if (GetHPBarLevel(GetMonData(mon, MON_DATA_HP), GetMonData(mon, MON_DATA_MAX_HP)) <= HP_BAR_YELLOW)
		return FALSE;
	else
		return TRUE;
}

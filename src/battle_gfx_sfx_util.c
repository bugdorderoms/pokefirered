#include "battle.h"
#include "battle_anim.h"
#include "battle_gimmicks.h"
#include "battle_gfx_sfx_util.h"
#include "battle_interface.h"
#include "battle_move_effects.h"
#include "decompress.h"
#include "gflib.h"
#include "graphics.h"
#include "m4a.h"
#include "party_menu.h"
#include "constants/songs.h"

static void Task_HandleBattleTableAnimation(u32 taskId);
static void Task_ClearBitWhenSpecialAnimDone(u32 taskId);
static void ClearSpritesBattlerHealthboxAnimData(void);
static void TintBattlerSprite(u32 battlerId, u32 paletteOffset);

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

const struct SpritePalette gSpritePalettes_HealthBoxHealthBar[2] =
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

const struct CompressedSpriteSheet gSpriteSheet_EnemyShadowsSized =
{
    .data = gFile_graphics_battle_interface_enemy_mon_shadow_sheet,
    .size = TILE_SIZE_4BPP * 8 * 4, // 8 tiles per sprite, 4 sprites total
    .tag = TAG_SHADOW_TILE,
};

static const struct OamData sOamData_EnemyShadow =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x8),
    .tileNum = 0,
    .priority = 3,
    .paletteNum = 0,
    .affineParam = 0
};

const struct SpriteTemplate gSpriteTemplate_EnemyShadow =
{
    .tileTag = TAG_SHADOW_TILE,
    .paletteTag = TAG_SHADOW_PAL,
    .oam = &sOamData_EnemyShadow,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
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
    u32 spriteId = sprite->data[1];

    if (!gSprites[spriteId].affineAnimEnded || gSprites[spriteId].invisible)
        return;
    
    if (gSprites[spriteId].animPaused)
        gSprites[spriteId].animPaused = FALSE;
    else if (gSprites[spriteId].animEnded)
    {
        gSprites[spriteId].callback = SpriteCB_SetToDummy3;
        StartSpriteAffineAnim(&gSprites[spriteId], 0);
        sprite->callback = SpriteCallbackDummy;
    }
}

void SpriteCB_TrainerSlideIn(struct Sprite *sprite)
{
    if (!(gIntroSlideFlags & INTRO_SLIDE_FLAG_SLIDE_IN_COUNTER))
    {
        sprite->x2 += sprite->data[0];
        
        if (sprite->x2 == 0)
            sprite->callback = SpriteCallbackDummy;
    }
}

void InitAndLaunchChosenStatusAnimation(u32 battlerId, u32 animStatusId, u32 status)
{
    switch (animStatusId)
    {
        case ID_STATUS1:
            if (status)
                LaunchStatusAnimation(battlerId, gNonVolatileStatusConditions[status - 1].animationId);
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

#define tBattlerId        data[0]
#define tAnimId           data[1]
#define tArgument         data[2]
#define tSubstituteRecede data[3]
#define tChangeMonForm    data[4]
#define tState            data[5]

bool32 TryHandleLaunchBattleTableAnimation(u32 battlerId, u32 tableId, u32 argument)
{
    u32 taskId;
    bool32 substituteRecede = gBattleAnims_General[tableId].substituteRecede;
    
    if (gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute)
    {
        if (!gBattleAnims_General[tableId].ignoreSubstitute && !substituteRecede)
            return TRUE;
        
        if (tableId == B_ANIM_SUBSTITUTE_FADE && gSprites[gBattlerSpriteIds[battlerId]].invisible)
        {
            LoadBattleMonGfxAndAnimate(battlerId, TRUE, gBattlerSpriteIds[battlerId]);
            ClearBehindSubstituteBit(battlerId);
            return TRUE;
        }
    }
    
    if (tableId == B_ANIM_ILLUSION_OFF)
    {
        gBattleStruct->battlers[battlerId].illusion.broken = TRUE;
        gBattleStruct->battlers[battlerId].illusion.on = FALSE;
    }
    gBattleAnimAttacker = gBattleAnimTarget = battlerId;
    
    gAnimScriptCallback = NULL;
    
    taskId = CreateTask(Task_HandleBattleTableAnimation, 10);
    gTasks[taskId].tBattlerId = battlerId;
    gTasks[taskId].tAnimId = tableId;
    gTasks[taskId].tArgument = argument;
    gTasks[taskId].tSubstituteRecede = substituteRecede;
    gTasks[taskId].tChangeMonForm = gBattleAnims_General[tableId].changeForm;
    
    gBattleSpritesDataPtr->healthBoxesData[battlerId].animFromTableActive = TRUE;
    
    return FALSE;
}

static void SwapSpeciesForFormChangeAnim(u32 battlerId)
{
    struct Pokemon *mon = GetBattlerPartyIndexPtr(battlerId);
    u32 temp = GetMonData(mon, MON_DATA_SPECIES);
    SetMonData(mon, MON_DATA_SPECIES, &gBattleSpritesDataPtr->battlerData[battlerId].formChangeSpecies);
    gBattleSpritesDataPtr->battlerData[battlerId].formChangeSpecies = temp;
}

static void Task_HandleBattleTableAnimation(u32 taskId)
{
    u32 battlerId = gTasks[taskId].tBattlerId;
    
    if (gAnimScriptCallback != NULL)
        gAnimScriptCallback();
    
    switch (gTasks[taskId].tState)
    {
        case 0:
            if (gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute && !gBattleSpritesDataPtr->battlerData[battlerId].flag_x8 && gTasks[taskId].tSubstituteRecede)
            {
                gBattleSpritesDataPtr->battlerData[battlerId].flag_x8 = TRUE;
                
                if (gTasks[taskId].tChangeMonForm)
                    SwapSpeciesForFormChangeAnim(battlerId);
                
                InitAndLaunchSpecialAnimation(battlerId, battlerId, B_ANIM_SUBSTITUTE_TO_MON);
            }
            gTasks[taskId].tState++;
            break;
        case 1:
            if (!gBattleSpritesDataPtr->healthBoxesData[battlerId].specialAnimActive)
            {
                if (gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute && gBattleSpritesDataPtr->battlerData[battlerId].flag_x8
                && gTasks[taskId].tSubstituteRecede && gTasks[taskId].tChangeMonForm)
                {
                    SwapSpeciesForFormChangeAnim(battlerId);
                    gBattleSpritesDataPtr->battlerData[battlerId].formChangeSpecies = SPECIES_NONE;
                }
                gBattleSpritesDataPtr->animationData->animArg = gTasks[taskId].tArgument;
                LaunchBattleAnimation(ANIM_TYPE_GENERAL, gTasks[taskId].tAnimId);
                gTasks[taskId].tState++;
            }
            break;
        case 2:
            if (!gAnimScriptActive)
            {
                CopyAllBattleSpritesInvisibilities(); // To maintain battlers sprites that became invisible in the anim
                
                if (gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute && gBattleSpritesDataPtr->battlerData[battlerId].flag_x8)
                {
                    InitAndLaunchSpecialAnimation(battlerId, battlerId, B_ANIM_MON_TO_SUBSTITUTE);
                    gBattleSpritesDataPtr->battlerData[battlerId].flag_x8 = FALSE;
                }
                gTasks[taskId].tState++;
            }
            break;
        case 3:
            gBattleSpritesDataPtr->healthBoxesData[battlerId].animFromTableActive = FALSE;
            DestroyTask(taskId);
            break;
    }
}

void InitAndLaunchSpecialAnimation(u32 attacker, u32 defender, u32 tableId)
{
    gBattleAnimAttacker = attacker;
    gBattleAnimTarget = defender;
    LaunchBattleAnimation(ANIM_TYPE_SPECIAL, tableId);
    gTasks[CreateTask(Task_ClearBitWhenSpecialAnimDone, 10)].tBattlerId = attacker;
    gBattleSpritesDataPtr->healthBoxesData[attacker].specialAnimActive = TRUE;
}

static void Task_ClearBitWhenSpecialAnimDone(u32 taskId)
{
    gAnimScriptCallback();
    
    if (!gAnimScriptActive)
    {
        gBattleSpritesDataPtr->healthBoxesData[gTasks[taskId].tBattlerId].specialAnimActive = FALSE;
        DestroyTask(taskId);
    }
}

bool32 IsBattleSEPlaying(u32 battlerId)
{
    if (IsSEPlaying())
    {
        if (++gBattleSpritesDataPtr->healthBoxesData[battlerId].soundTimer < 30)
            return TRUE;
        
        m4aMPlayStop(&gMPlayInfo_SE1);
        m4aMPlayStop(&gMPlayInfo_SE2);
    }
    gBattleSpritesDataPtr->healthBoxesData[battlerId].soundTimer = 0;
    return FALSE;
}

void BattleLoadMonSpriteGfx(u32 battlerId)
{
    struct Pokemon *mon = GetBattlerIllusionPartyIndexPtr(battlerId);
    u32 species, currentPersonality, paletteOffset;
    void *buffer;
    const u32 *frontSpritePal;
    
    if (!gBattleSpritesDataPtr->battlerData[battlerId].transformSpecies)
    {
        species = GetMonData(mon, MON_DATA_SPECIES);
        currentPersonality = GetMonData(mon, MON_DATA_PERSONALITY);
        frontSpritePal = GetMonSpritePal(mon);
    }
    else
    {
        species = gBattleSpritesDataPtr->battlerData[battlerId].transformSpecies;
        currentPersonality = gTransformedPersonalities[battlerId];
        frontSpritePal = GetMonSpritePalFromSpecies(species, gTransformedShinies[battlerId]);
    }
    LoadSpecialPokePic(species, currentPersonality, (GetBattlerSide(battlerId) == B_SIDE_OPPONENT), gMonSpritesGfxPtr->battlers[GetBattlerPosition(battlerId)].sprite);
    
    paletteOffset = 0x100 + battlerId * 16;
    
    buffer = AllocZeroed(0x400);
    LZDecompressWram(frontSpritePal, buffer);
    LoadPalette(buffer, paletteOffset, 0x20);
    LoadPalette(buffer, 0x80 + battlerId * 16, 0x20);
    Free(buffer);
    
    TintBattlerSprite(battlerId, paletteOffset);
}

static void TintBattlerSprite(u32 battlerId, u32 paletteOffset)
{
    // Transform's white color
    if (gBattleSpritesDataPtr->battlerData[battlerId].transformSpecies)
    {
        BlendPalette(paletteOffset, 16, 6, RGB_WHITE);
        CpuCopy32(gPlttBufferFaded + paletteOffset, gPlttBufferUnfaded + paletteOffset, 32);
    }
    
    // Totem orange tint
    if (IsBattlerTotemPokemon(battlerId))
    {
        BlendPalette(paletteOffset, 16, 6, RGB(29, 16, 1));
        CpuCopy32(gPlttBufferFaded + paletteOffset, gPlttBufferUnfaded + paletteOffset, 32);
    }
}

void DecompressGhostFrontPic(u32 battlerId)
{
    void *buffer;
    LZDecompressWram(gGhostFrontPic, gMonSpritesGfxPtr->battlers[GetBattlerPosition(battlerId)].sprite);
    buffer = AllocZeroed(0x400);
    LZDecompressWram(gGhostPalette, buffer);
    LoadPalette(buffer, 0x100 + 16 * battlerId, 0x20);
    LoadPalette(buffer, 0x80 + 16 * battlerId, 0x20);
    Free(buffer);
}

void DecompressTrainerFrontPic(u32 frontPicId, u32 battlerId)
{
    struct SpriteSheet sheet;
    u32 position = GetBattlerPosition(battlerId);

    LZDecompressWram(gTrainerFrontPicTable[frontPicId].pic.data, gMonSpritesGfxPtr->battlers[position].sprite);
    sheet.data = gMonSpritesGfxPtr->battlers[position].sprite;
    sheet.size = gTrainerFrontPicTable[frontPicId].pic.size;
    sheet.tag = gTrainerFrontPicTable[frontPicId].pic.tag;
    LoadSpriteSheet(&sheet);
    LoadCompressedSpritePaletteUsingHeap(&gTrainerFrontPicTable[frontPicId].palette);
}

void DecompressTrainerBackPalette(u32 index, u32 palette)
{
    LoadCompressedPalette(gTrainerBackPicTable[index].palette.data, (palette + 16) * 16, 0x20);
}

void FreeTrainerFrontPicPaletteAndTile(u32 frontPicId)
{
    FreeSpritePaletteByTag(gTrainerFrontPicTable[frontPicId].palette.tag);
    FreeSpriteTilesByTag(gTrainerFrontPicTable[frontPicId].pic.tag);
}

bool32 BattleLoadAllHealthBoxesGfx(u32 state)
{
    bool32 retVal = FALSE;
    
    switch (state)
    {
        case 0:
            break;
        case 1:
            LoadSpritePalette(&gSpritePalettes_HealthBoxHealthBar[0]);
            LoadSpritePalette(&gSpritePalettes_HealthBoxHealthBar[1]);
            LoadSpritePalette(&gSpritePalette_GimmickIndicator);
            break;
        case 2:
            if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
                LoadCompressedSpriteSheetUsingHeap(&sSpriteSheet_SafariHealthbox);
            else if (IsDoubleBattleOnSide(B_SIDE_PLAYER))
                LoadCompressedSpriteSheetUsingHeap(&sSpriteSheets_DoublesPlayerHealthbox[0]);
            else
                LoadCompressedSpriteSheetUsingHeap(&sSpriteSheet_SinglesPlayerHealthbox);
            break;
        case 3:
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                LoadCompressedSpriteSheet(&sSpriteSheets_DoublesPlayerHealthbox[1]); // In a Two vs One battle this goes unused, it's loaded only for align purpose
            break;
        case 4:
            if (IsDoubleBattleOnSide(B_SIDE_OPPONENT))
                LoadCompressedSpriteSheetUsingHeap(&sSpriteSheets_DoublesOpponentHealthbox[0]);
            else
                LoadCompressedSpriteSheetUsingHeap(&sSpriteSheet_SinglesOpponentHealthbox);
            break;
        case 5:
            if (IsDoubleBattleOnSide(B_SIDE_OPPONENT))
                LoadCompressedSpriteSheetUsingHeap(&sSpriteSheets_DoublesOpponentHealthbox[1]);
            break;
        case 6:
            LoadCompressedSpriteSheetUsingHeap(&sSpriteSheets_HealthBar[gBattlerPositions[0]]);
            break;
        case 7:
            LoadCompressedSpriteSheetUsingHeap(&sSpriteSheets_HealthBar[gBattlerPositions[1]]);
            break;
        case 8:
            if (IsDoubleBattleOnSide(B_SIDE_OPPONENT))
                LoadCompressedSpriteSheetUsingHeap(&sSpriteSheets_HealthBar[gBattlerPositions[2]]);
            break;
        case 9:
            if (IsDoubleBattleOnSide(B_SIDE_OPPONENT))
                LoadCompressedSpriteSheetUsingHeap(&sSpriteSheets_HealthBar[gBattlerPositions[3]]);
            break;
        case 10:
            retVal = TRUE;
            break;
    }
    return retVal;
}

void LoadBattleBarGfx(void)
{
    LZDecompressWram(gInterfaceGfx_HPNumbers, gMonSpritesGfxPtr->barFontGfx);
}

bool32 BattleInitAllSprites(u8 *state, u8 *battlerId)
{
    switch (*state)
    {
    case 0:
        ClearSpritesBattlerHealthboxAnimData();
        ++*state;
        break;
    case 1:
        if (!BattleLoadAllHealthBoxesGfx(*battlerId))
            ++*battlerId;
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
        if ((gBattleTypeFlags & BATTLE_TYPE_SAFARI) && *battlerId == B_POSITION_PLAYER_LEFT)
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
    u32 i;

    for (i = 0; i < gBattlersCount; ++i)
        CopyBattleSpriteInvisibility(i);
}

void CopyBattleSpriteInvisibility(u32 battlerId)
{
    gBattleSpritesDataPtr->battlerData[battlerId].invisible = gSprites[gBattlerSpriteIds[battlerId]].invisible;
}

void HandleSpeciesGfxDataChange(u32 battlerAtk, u32 battlerDef, u32 flags)
{
    void *buffer;
    struct Pokemon *defMon = GetBattlerPartyIndexPtr(battlerDef);
    u32 paletteOffset, targetSpecies = GetMonData(defMon, MON_DATA_SPECIES);
    u32 atkPosition, personality;
    bool32 isShiny;
    
    if (battlerAtk == battlerDef) // Battler self transforming, keep personality and shynies
    {
        personality = GetMonData(defMon, MON_DATA_PERSONALITY);
        isShiny = GetMonData(defMon, MON_DATA_IS_SHINY);
    }
    else // Battler transforming into another pokemon, copy personality and shynies
    {
        personality = gTransformedPersonalities[battlerAtk];
        isShiny = gTransformedShinies[battlerAtk];
    }
    atkPosition = GetBattlerPosition(battlerAtk);
    LoadSpecialPokePic(targetSpecies, personality, (GetBattlerSide(battlerAtk) == B_SIDE_OPPONENT), gMonSpritesGfxPtr->battlers[atkPosition].sprite);
    DmaCopy32(3, gMonSpritesGfxPtr->battlers[atkPosition].sprite, (void *)(VRAM + 0x10000 + gSprites[gBattlerSpriteIds[battlerAtk]].oam.tileNum * 32), 0x800);
    
    buffer = AllocZeroed(0x400);
    LZDecompressWram(GetMonSpritePalFromSpecies(targetSpecies, isShiny), buffer);
    paletteOffset = 0x100 + battlerAtk * 16;
    LoadPalette(buffer, paletteOffset, 32);
    Free(buffer);
    
    if (!(flags & (SPECIESGFX_FLAG_IS_GHOST | SPECIESGFX_FLAG_NO_TRANSFORM_PAL_FADE)))
        gBattleSpritesDataPtr->battlerData[battlerAtk].transformSpecies = targetSpecies;
    
    TintBattlerSprite(battlerAtk, paletteOffset);
    
    gSprites[gBattlerSpriteIds[battlerAtk]].y = GetBattlerSpriteDefault_Y(battlerAtk);
    StartSpriteAnim(&gSprites[gBattlerSpriteIds[battlerAtk]], 0);
    
    if (flags & SPECIESGFX_FLAG_IS_GHOST)
    {
        SetMonData(GetBattlerPartyIndexPtr(battlerAtk), MON_DATA_NICKNAME, gSpeciesInfo[targetSpecies].name);
        UpdateHealthboxAttribute(battlerAtk, HEALTHBOX_NICK);
        TryAddPokeballIconToHealthbox(gHealthboxSpriteIds[battlerAtk], TRUE);
    }
}

void BattleLoadSubstituteOrMonSpriteGfx(u32 battlerId, bool32 loadMonSprite)
{
    u32 i, position;

    if (!loadMonSprite)
    {
        position = GetBattlerPosition(battlerId);
        LZDecompressVram(GetBattlerSide(battlerId) != B_SIDE_PLAYER ? gSubstituteDollGfx : gSubstituteDollTilemap, gMonSpritesGfxPtr->battlers[position].sprite);
        
        for (i = 1; i < 4; ++i)
        {
            u8 (*ptr)[4][0x800] = gMonSpritesGfxPtr->battlers[position].sprite;
            DmaCopy32Defvars(3, (*ptr)[0], (*ptr)[i], 0x800);
        }
        LoadCompressedPalette(gSubstituteDollPal, (battlerId * 16) + 0x100, 32);
    }
    else
        BattleLoadMonSpriteGfx(battlerId);
}

void LoadBattleMonGfxAndAnimate(u32 battlerId, bool32 loadMonSprite, u32 spriteId)
{
    BattleLoadSubstituteOrMonSpriteGfx(battlerId, loadMonSprite);
    StartSpriteAnim(&gSprites[spriteId], 0);
    gSprites[spriteId].y = loadMonSprite ? GetBattlerSpriteDefault_Y(battlerId) : GetSubstituteSpriteDefault_Y(battlerId);
}

void TrySetBehindSubstituteSpriteBit(u32 battlerId, u32 move)
{
    if (gBattleMoves[move].effect == EFFECT_SUBSTITUTE)
        gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute = TRUE;
}

void ClearBehindSubstituteBit(u32 battlerId)
{
    gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute = FALSE;
}

void HandleLowHpMusicChange(struct Pokemon *mon, u32 battlerId)
{
    if (GetHPBarLevel(GetMonData(mon, MON_DATA_HP), GetMonData(mon, MON_DATA_MAX_HP)) == HP_BAR_RED)
    {
        if (!gBattleSpritesDataPtr->battlerData[battlerId].lowHpSong)
        {
            if (!gBattleSpritesDataPtr->battlerData[BATTLE_PARTNER(battlerId)].lowHpSong)
                PlaySE(SE_LOW_HEALTH);
            
            gBattleSpritesDataPtr->battlerData[battlerId].lowHpSong = TRUE;
        }
    }
    else
    {
        gBattleSpritesDataPtr->battlerData[battlerId].lowHpSong = FALSE;
        
        m4aSongNumStop(SE_LOW_HEALTH);
        
        if (IsDoubleBattleForBattler(battlerId) && !gBattleSpritesDataPtr->battlerData[BATTLE_PARTNER(battlerId)].lowHpSong)
            m4aSongNumStop(SE_LOW_HEALTH);
    }
}

void BattleStopLowHpSound(void)
{
    u32 playerBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);

    gBattleSpritesDataPtr->battlerData[playerBattler].lowHpSong = FALSE;
    
    if (IsDoubleBattleForBattler(playerBattler))
        gBattleSpritesDataPtr->battlerData[BATTLE_PARTNER(playerBattler)].lowHpSong = FALSE;
    
    m4aSongNumStop(SE_LOW_HEALTH);
}

void HandleBattleLowHpMusicChange(void)
{
    if (gMain.inBattle)
    {
        u32 playerBattler1 = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        u32 playerBattler2 = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
        u32 battler1PartyId = GetPartyIdFromBattlePartyId(gBattlerPartyIndexes[playerBattler1]);
        u32 battler2PartyId = GetPartyIdFromBattlePartyId(gBattlerPartyIndexes[playerBattler2]);

        if (GetMonData(&gPlayerParty[battler1PartyId], MON_DATA_HP))
            HandleLowHpMusicChange(&gPlayerParty[battler1PartyId], playerBattler1);
        
        if (IsDoubleBattleForBattler(playerBattler1) && GetMonData(&gPlayerParty[battler2PartyId], MON_DATA_HP))
            HandleLowHpMusicChange(&gPlayerParty[battler2PartyId], playerBattler2);
    }
}

void SetBattlerSpriteAffineMode(u32 affineMode)
{
    u32 i;

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
                gSprites[gBattlerSpriteIds[i]].oam.matrixNum = gBattleSpritesDataPtr->healthBoxesData[i].matrixNum;
        }
    }
}

static void CreateBattlerShadowSprite(u8 *dest, u32 battlerId, u32 side)
{
    *dest = CreateSprite(&gSpriteTemplate_EnemyShadow, GetBattlerSpriteCoord(battlerId, BATTLER_COORD_X), GetBattlerSpriteCoord(battlerId, BATTLER_COORD_Y) + ENEMY_SHADOW_Y_OFFSET, 0xC8);
    gSprites[*dest].data[0] = battlerId;
    gSprites[*dest].data[1] = side;
    gSprites[*dest].data[2] = gSprites[*dest].oam.tileNum;
}

static void LoadBattlerShadowSprite(u32 battlerId)
{
    CreateBattlerShadowSprite(&gBattleSpritesDataPtr->healthBoxesData[battlerId].shadowSpriteIdPrimary, battlerId, 0);
    CreateBattlerShadowSprite(&gBattleSpritesDataPtr->healthBoxesData[battlerId].shadowSpriteIdSecondary, battlerId, 1);
    SetBattlerShadowSpriteCallback(battlerId);
}

void LoadAndCreateEnemyShadowSprites(void)
{
    u32 i;
    
    LoadCompressedSpriteSheetUsingHeap(&gSpriteSheet_EnemyShadowsSized);
    
    for (i = 0; i < gBattlersCount; i++)
    {
        if (GetBattlerSide(i) == B_SIDE_OPPONENT)
            LoadBattlerShadowSprite(i);
    }
}

static void SpriteCB_EnemyShadow(struct Sprite *shadowSprite)
{
    bool32 invisible;
    u32 battlerId = shadowSprite->data[0];
    struct Sprite *battlerSprite = &gSprites[gBattlerSpriteIds[battlerId]];

    if (!battlerSprite->inUse || !IsBattlerSpritePresent(battlerId))
        shadowSprite->callback = SpriteCB_SetInvisible;
    else
    {
        u32 species = GetBattlerVisualSpecies(battlerId);
        
        if (gSpeciesInfo[species].suppressEnemyShadow || gAnimScriptActive || battlerSprite->invisible || gBattleSpritesDataPtr->battlerData[battlerId].behindSubstitute
        || ((gIntroSlideFlags & INTRO_SLIDE_FLAG_SLIDE_IN_STARTED) && (gBattleTypeFlags & BATTLE_TYPE_TRAINER))) // Don't show shadow on intro trainer slide
            invisible = TRUE;
        else
            invisible = FALSE;
        
        shadowSprite->x = battlerSprite->x + (shadowSprite->data[1] == 0 ? -16 : 16);
        shadowSprite->x2 = battlerSprite->x2 + gSpeciesInfo[species].shadowXOffset;
        shadowSprite->oam.tileNum = shadowSprite->data[2] + 8 * gSpeciesInfo[species].shadowSize + 4 * shadowSprite->data[1];
        shadowSprite->invisible = invisible;
    }
}

void SpriteCB_SetInvisible(struct Sprite *sprite)
{
    sprite->invisible = TRUE;
}

void SetBattlerShadowSpriteCallback(u32 battlerId)
{
    // The player's shadow is never seen.
    if (GetBattlerSide(battlerId) == B_SIDE_OPPONENT)
    {
        gSprites[gBattleSpritesDataPtr->healthBoxesData[battlerId].shadowSpriteIdPrimary].callback = SpriteCB_EnemyShadow;
        gSprites[gBattleSpritesDataPtr->healthBoxesData[battlerId].shadowSpriteIdSecondary].callback = SpriteCB_EnemyShadow;
    }
}

void HideBattlerShadowSprite(u32 battlerId)
{
    gSprites[gBattleSpritesDataPtr->healthBoxesData[battlerId].shadowSpriteIdPrimary].callback = SpriteCB_SetInvisible;
    gSprites[gBattleSpritesDataPtr->healthBoxesData[battlerId].shadowSpriteIdSecondary].callback = SpriteCB_SetInvisible;
}

// Low-level function that sets specific interface tiles' palettes, overwriting any pixel with value 0.
void BattleInterfaceSetWindowPals(void)
{
    // 9 tiles at 0x06000240
    u32 i, j;
    u16 *vramPtr = (u16 *)(BG_VRAM + 0x240);

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

void ClearTemporarySpeciesSpriteData(u32 battlerId, bool32 dontClearSubstitute)
{
    gBattleSpritesDataPtr->battlerData[battlerId].transformSpecies = SPECIES_NONE;
    
    if (!dontClearSubstitute)
        ClearBehindSubstituteBit(battlerId);
}

void AllocateMonSpritesGfx(void)
{
    u32 i, j;

    gMonSpritesGfxPtr = NULL;
    gMonSpritesGfxPtr = AllocZeroed(sizeof(*gMonSpritesGfxPtr));
    gMonSpritesGfxPtr->firstDecompressed = AllocZeroed(0x8000);
    gMonSpritesGfxPtr->barFontGfx = AllocZeroed(0x1000);
    
    for (i = 0; i < MAX_BATTLERS_COUNT; ++i)
    {
        gMonSpritesGfxPtr->battlers[i].sprite = gMonSpritesGfxPtr->firstDecompressed + (i * 0x2000);
        gMonSpritesGfxPtr->battlers[i].template = gSpriteTemplates_Battlers[i];

        for (j = 0; j < 4; ++j)
        {
            gMonSpritesGfxPtr->battlers[i].images[j].data = gMonSpritesGfxPtr->battlers[i].sprite + (j * 0x800);
            gMonSpritesGfxPtr->battlers[i].images[j].size = 0x800;
        }
        gMonSpritesGfxPtr->battlers[i].template.images = gMonSpritesGfxPtr->battlers[i].images;
    }
}

void FreeMonSpritesGfx(void)
{
    u32 i;
    
    if (gMonSpritesGfxPtr == NULL)
        return;
    
    if (gMonSpritesGfxPtr->multiUseBuffer != NULL)
        FREE_AND_SET_NULL(gMonSpritesGfxPtr->multiUseBuffer);
    
    FREE_AND_SET_NULL(gMonSpritesGfxPtr->barFontGfx);
    FREE_AND_SET_NULL(gMonSpritesGfxPtr->firstDecompressed);
    
    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
        gMonSpritesGfxPtr->battlers[i].sprite = NULL;
    
    FREE_AND_SET_NULL(gMonSpritesGfxPtr);
}

bool32 ShouldPlayNormalPokeCry(struct Pokemon *mon)
{
    if (GetMonData(mon, MON_DATA_STATUS_ID))
        return FALSE;
    else if (GetHPBarLevel(GetMonData(mon, MON_DATA_HP), GetMonData(mon, MON_DATA_MAX_HP)) <= HP_BAR_YELLOW)
        return FALSE;
    else
        return TRUE;
}

u32 GetBattlerVisualSpecies(u32 battlerId)
{
    u32 transformSpecies = gBattleSpritesDataPtr->battlerData[battlerId].transformSpecies;
    return transformSpecies ? transformSpecies : GetMonData(GetBattlerIllusionPartyIndexPtr(battlerId), MON_DATA_SPECIES);
}

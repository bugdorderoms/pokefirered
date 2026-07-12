#include "global.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "fieldmap.h"
#include "field_camera.h"
#include "field_control_avatar.h"
#include "field_effect.h"
#include "field_fadetransition.h"
#include "field_player_avatar.h"
#include "field_weather.h"
#include "fldeff.h"
#include "gflib.h"
#include "math_util.h"
#include "metatile_behavior.h"
#include "new_menu_helpers.h"
#include "object_event_graphics.h"
#include "overworld.h"
#include "party_menu.h"
#include "random.h"
#include "ride_pager.h"
#include "script.h"
#include "task.h"
#include "trainer_pokemon_sprites.h"
#include "trig.h"
#include "util.h"
#include "constants/event_objects.h"
#include "constants/event_object_movement.h"
#include "constants/field_effects.h"
#include "constants/poke_ride.h"
#include "constants/songs.h"
#include "constants/sound.h"

#define FIELD_EFFECT_COUNT 32

EWRAM_DATA u32 gFieldEffectArguments[8] = {0};
EWRAM_DATA u16 gReflectionPaletteBuffer[16] = {0};
EWRAM_DATA struct MapPosition gPlayerFacingPosition = {};

static u8 sFieldEffectActiveList[FIELD_EFFECT_COUNT];

static void FieldEffectActiveListAdd(u32 fldeff);
static void FieldEffectFreeGraphicsResources(struct Sprite * sprite);

#include "data/field_effects.h"

// Returns the id of the sprite created, if necessary
u32 FieldEffectStart(u32 fldeff)
{
    FieldEffectActiveListAdd(fldeff);
    return sFldEffectsTable[fldeff]();
}

void FieldEffectStop(struct Sprite * sprite, u32 fldeff)
{
    FieldEffectFreeGraphicsResources(sprite);
    FieldEffectActiveListRemove(fldeff);
}

void FieldEffectActiveListClear(void)
{
    u32 i;
    
    for (i = 0; i < FIELD_EFFECT_COUNT; i++)
        sFieldEffectActiveList[i] = 0xFF;
}

static void FieldEffectActiveListAdd(u32 fldeff)
{
    u32 i;
    
    for (i = 0; i < FIELD_EFFECT_COUNT; i++)
    {
        if (sFieldEffectActiveList[i] == 0xFF)
        {
            sFieldEffectActiveList[i] = fldeff;
            return;
        }
    }
}

void FieldEffectActiveListRemove(u32 fldeff)
{
    u32 i;
    
    for (i = 0; i < FIELD_EFFECT_COUNT; i++)
    {
        if (sFieldEffectActiveList[i] == fldeff)
        {
            sFieldEffectActiveList[i] = 0xFF;
            return;
        }
    }
}

bool32 FieldEffectActiveListContains(u32 fldeff)
{
    u32 i;
    
    for (i = 0; i < FIELD_EFFECT_COUNT; i++)
    {
        if (sFieldEffectActiveList[i] == fldeff)
            return TRUE;
    }
    return FALSE;
}

static void FieldEffectFreeTilesIfUnused(u32 tileStart)
{
    u32 i, tileTag = GetSpriteTileTagByTileStart(tileStart);

    if (tileTag == SPRITE_INVALID_TAG)
        return;
    
    for (i = 0; i < MAX_SPRITES; i++)
    {
        if (gSprites[i].inUse && gSprites[i].usingSheet && tileStart == gSprites[i].sheetTileStart)
            return;
    }
    FreeSpriteTilesByTag(tileTag);
}

void FieldEffectFreePaletteIfUnused(u32 paletteNum)
{
    u32 i, paletteTag = GetSpritePaletteTagByPaletteNum(paletteNum);
    
    if (paletteTag == SPRITE_INVALID_TAG)
        return;
    
    for (i = 0; i < MAX_SPRITES; i++)
    {
        if (gSprites[i].inUse && gSprites[i].oam.paletteNum == paletteNum)
            return;
    }
    FreeSpritePaletteByTag(paletteTag);
}

static void FieldEffectFreeGraphicsResources(struct Sprite * sprite)
{
    u32 tileStart = sprite->sheetTileStart;
    u32 paletteNum = sprite->oam.paletteNum;
    
    DestroySprite(sprite);
    FieldEffectFreeTilesIfUnused(tileStart);
    FieldEffectFreePaletteIfUnused(paletteNum);
}

void LoadFieldEffectPalette(const struct SpriteTemplate *spriteTemplate)
{
    if (spriteTemplate->paletteTag != SPRITE_INVALID_TAG)
        LoadObjectEventPalette(spriteTemplate->paletteTag);
}

static void FieldEffect_LoadPal(const struct SpritePalette *spritePalette)
{
    u32 idx = IndexOfSpritePaletteTag(spritePalette->tag);
    
    LoadSpritePalette(spritePalette);
    
    if (idx == 0xFF)
        ApplyGlobalFieldPaletteTint(IndexOfSpritePaletteTag(spritePalette->tag));
}

void FieldEffect_LoadFadedPal(const struct SpritePalette *spritePalette)
{
    FieldEffect_LoadPal(spritePalette);
    UpdateSpritePaletteWithWeather(IndexOfSpritePaletteTag(spritePalette->tag));
}

void FreeResourcesAndDestroySprite(struct Sprite * sprite, u32 spriteId)
{
    FreeSpriteOamMatrix(sprite);
    FreeAndDestroyMonPicSprite(spriteId);
}

void ApplyGlobalFieldPaletteTint(u32 paletteIdx)
{
    switch (gGlobalFieldTintMode)
    {
    case GF_TINT_NONE:
        return;
    case GF_TINT_GRAYSCALE:
        TintPalette_GrayScale(&gPlttBufferUnfaded[(paletteIdx + 16) * 16], 0x10);
        break;
    case GF_TINT_SEPIA:
        TintPalette_SepiaTone(&gPlttBufferUnfaded[(paletteIdx + 16) * 16], 0x10);
        break;
    default:
        return;
    }
    CpuFastCopy(&gPlttBufferUnfaded[(paletteIdx + 16) * 16], &gPlttBufferFaded[(paletteIdx + 16) * 16], 0x20);
}

//////////////////////////////////
// FLDEFF_EXCLAMATION_MARK_ICON //
//////////////////////////////////

static void SpriteCB_Emoticons(struct Sprite * sprite);

static const struct OamData sOamData_Emoticons = {
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .size = SPRITE_SIZE(16x16),
    .priority = 1,
    .paletteNum = 0,
};

static const union AnimCmd sAnimCmd_ExclamationMark[] = {
    ANIMCMD_FRAME( 0,  4),
    ANIMCMD_FRAME( 1,  4),
    ANIMCMD_FRAME( 2, 52),
    ANIMCMD_END
};

static const union AnimCmd sAnimCmd_DoubleExclMark[] = {
    ANIMCMD_FRAME( 6,  4),
    ANIMCMD_FRAME( 7,  4),
    ANIMCMD_FRAME( 8, 52),
    ANIMCMD_END
};

static const union AnimCmd sAnimCmd_X[] = {
    ANIMCMD_FRAME( 3,  4),
    ANIMCMD_FRAME( 4,  4),
    ANIMCMD_FRAME( 5, 52),
    ANIMCMD_END
};

static const union AnimCmd sAnimCmd_SmileyFace[] = {
    ANIMCMD_FRAME( 9,  4),
    ANIMCMD_FRAME(10,  4),
    ANIMCMD_FRAME(11, 52),
    ANIMCMD_END
};

static const union AnimCmd sAnimCmd_QuestionMark[] = {
    ANIMCMD_FRAME(12,  4),
    ANIMCMD_FRAME(13,  4),
    ANIMCMD_FRAME(14, 52),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_Emoticons[] = {
    sAnimCmd_ExclamationMark,
    sAnimCmd_DoubleExclMark,
    sAnimCmd_X,
    sAnimCmd_SmileyFace,
    sAnimCmd_QuestionMark
};

static const struct SpriteFrameImage sSpriteImages_Emoticons[] = {
    {sGfx_Emoticons + 0x000, 0x80},
    {sGfx_Emoticons + 0x040, 0x80},
    {sGfx_Emoticons + 0x080, 0x80},

    {sGfx_Emoticons + 0x180, 0x80},
    {sGfx_Emoticons + 0x1C0, 0x80},
    {sGfx_Emoticons + 0x200, 0x80},

    {sGfx_Emoticons + 0x0C0, 0x80},
    {sGfx_Emoticons + 0x100, 0x80},
    {sGfx_Emoticons + 0x140, 0x80},

    {sGfx_Emoticons + 0x240, 0x80},
    {sGfx_Emoticons + 0x280, 0x80},
    {sGfx_Emoticons + 0x2C0, 0x80},

    {sGfx_Emoticons + 0x300, 0x80},
    {sGfx_Emoticons + 0x340, 0x80},
    {sGfx_Emoticons + 0x380, 0x80},
};

static const struct SpriteTemplate sSpriteTemplate_Emoticons = {
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = OBJ_EVENT_PAL_TAG_PLAYER_RED,
    .oam = &sOamData_Emoticons,
    .anims = sSpriteAnimTable_Emoticons,
    .images = sSpriteImages_Emoticons,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Emoticons
};

#define sLocalId    data[0]
#define sMapNum     data[1]
#define sMapGroup   data[2]
#define sData3      data[3]
#define sData4      data[4]
#define sFldEffId   data[7]

static void CreateEmoticonSprite(u32 fldEffId, u32 subpriority, u32 spriteAnimNum)
{
    u32 spriteId;

    LoadObjectEventPalette(OBJ_EVENT_PAL_TAG_PLAYER_RED);
    UpdateSpritePaletteWithWeather(IndexOfSpritePaletteTag(OBJ_EVENT_PAL_TAG_PLAYER_RED));
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_Emoticons, 0, 0, subpriority);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        
        sprite->oam.priority = 1;
        sprite->coordOffsetEnabled = TRUE;
    
        sprite->sLocalId = gFieldEffectArguments[0];
        sprite->sMapNum = gFieldEffectArguments[1];
        sprite->sMapGroup = gFieldEffectArguments[2];
        sprite->sData3 = -5;
        sprite->sFldEffId = fldEffId;
    
        StartSpriteAnim(sprite, spriteAnimNum);
    }
}

static u32 FldEff_ExclamationMarkIcon(void)
{
    CreateEmoticonSprite(FLDEFF_EXCLAMATION_MARK_ICON, 0x53, 0);
    return 0;
}

static void SpriteCB_Emoticons(struct Sprite *sprite)
{
    u8 objEventId;

    if (TryGetObjectEventIdByLocalIdAndMap(sprite->sLocalId, sprite->sMapNum, sprite->sMapGroup, &objEventId) || sprite->animEnded)
        FieldEffectStop(sprite, sprite->sFldEffId);
    else
    {
        struct Sprite *objEventSprite = &gSprites[gObjectEvents[objEventId].spriteId];

        sprite->sData4 += sprite->sData3;
        
        sprite->x = objEventSprite->x;
        sprite->y = objEventSprite->y - 16;
        sprite->x2 = objEventSprite->x2;
        sprite->y2 = objEventSprite->y2 + sprite->sData4;
        
        if (sprite->sData4)
            sprite->sData3++;
        else
            sprite->sData3 = 0;
    }
}

#undef sLocalId
#undef sMapNum
#undef sMapGroup
#undef sData3
#undef sData4
#undef sFldEffId

///////////////////
// FLDEFF_SHADOW //
///////////////////

static void SpriteCB_Shadow(struct Sprite *sprite);

static const union AnimCmd sAnim_Shadow[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_Shadow[] =
{
    sAnim_Shadow,
};

static const struct SpriteFrameImage sPicTable_ShadowSmall[] = { gFieldEffectObjectPic_ShadowSmall, 0x20 };
static const struct SpriteFrameImage sPicTable_ShadowMedium[] = { gFieldEffectObjectPic_ShadowMedium, 0x40 };
static const struct SpriteFrameImage sPicTable_ShadowLarge[] = { gFieldEffectObjectPic_ShadowLarge, 0x80 };
static const struct SpriteFrameImage sPicTable_ShadowExtraLarge[] = { gFieldEffectObjectPic_ShadowExtraLarge, 0x400 };

static const struct SpriteTemplate sSpriteTemplate_ShadowSmall =
{
    .tileTag = FLDEFF_TILE_TAG_SHADOW_SMALL,
    .paletteTag = PALTAG_WEATHER,
    .oam = &gObjectEventBaseOam_8x8,
    .anims = sAnimTable_Shadow,
    .images = sPicTable_ShadowSmall,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Shadow,
};

static const struct SpriteTemplate sSpriteTemplate_ShadowMedium =
{
    .tileTag = FLDEFF_TILE_TAG_SHADOW_MEDIUM,
    .paletteTag = PALTAG_WEATHER,
    .oam = &gObjectEventBaseOam_16x8,
    .anims = sAnimTable_Shadow,
    .images = sPicTable_ShadowMedium,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Shadow,
};

static const struct SpriteTemplate sSpriteTemplate_ShadowLarge =
{
    .tileTag = FLDEFF_TILE_TAG_SHADOW_LARGE,
    .paletteTag = PALTAG_WEATHER,
    .oam = &gObjectEventBaseOam_32x8,
    .anims = sAnimTable_Shadow,
    .images = sPicTable_ShadowLarge,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Shadow,
};

static const struct SpriteTemplate sSpriteTemplate_ShadowExtraLarge =
{
    .tileTag = FLDEFF_TILE_TAG_SHADOW_EXTRA_LARGE,
    .paletteTag = PALTAG_WEATHER,
    .oam = &gObjectEventBaseOam_64x32,
    .anims = sAnimTable_Shadow,
    .images = sPicTable_ShadowExtraLarge,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Shadow,
};

static const struct SpriteTemplate *const sShadowsTemplatePointers[] = {
    [SHADOW_SIZE_S]  = &sSpriteTemplate_ShadowSmall,
    [SHADOW_SIZE_M]  = &sSpriteTemplate_ShadowMedium,
    [SHADOW_SIZE_L]  = &sSpriteTemplate_ShadowLarge,
    [SHADOW_SIZE_XL] = &sSpriteTemplate_ShadowExtraLarge
};

static const u16 sShadowVerticalOffsets[] = {
    [SHADOW_SIZE_S]  =  4,
    [SHADOW_SIZE_M]  =  4,
    [SHADOW_SIZE_L]  =  4,
    [SHADOW_SIZE_XL] = 16
};

static u32 FldEff_Shadow(void)
{
    s32 i;
    u32 objectEventId, spriteId;
    const struct ObjectEventGraphicsInfo * graphicsInfo;
    
    // Search backwards, bc of CreateSpriteAtEnd
    for (i = MAX_SPRITES - 1; i > -1; i--)
    {
        // Check if a shadow sprite already exists
        if (gSprites[i].callback == SpriteCB_Shadow && gSprites[i].data[0] == gFieldEffectArguments[0] && gSprites[i].data[1] == gFieldEffectArguments[1]
        && gSprites[i].data[2] == gFieldEffectArguments[2])
            return 0;
    }
    objectEventId = GetObjectEventIdByLocalIdAndMap(gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2]);
    graphicsInfo = GetObjectEventGraphicsInfo(gObjectEvents[objectEventId].graphicsId);
    
    // Don't create shadow for this obj event
    if (graphicsInfo->shadowSize == SHADOW_NONE)
        return 0;
    
    LoadSpriteSheetByTemplate(sShadowsTemplatePointers[graphicsInfo->shadowSize]);
    spriteId = CreateSpriteAtEnd(sShadowsTemplatePointers[graphicsInfo->shadowSize], 0, 0, 0x94);
    
    if (spriteId != MAX_SPRITES)
    {
        gSprites[spriteId].coordOffsetEnabled = TRUE;
        gSprites[spriteId].oam.objMode = ST_OAM_OBJ_BLEND;
        gSprites[spriteId].data[0] = gFieldEffectArguments[0];
        gSprites[spriteId].data[1] = gFieldEffectArguments[1];
        gSprites[spriteId].data[2] = gFieldEffectArguments[2];
        gSprites[spriteId].data[3] = (graphicsInfo->height >> 1) - sShadowVerticalOffsets[graphicsInfo->shadowSize];
    }
    return 0;
}

static void SpriteCB_Shadow(struct Sprite * sprite)
{
    u8 objectEventId;

    if (TryGetObjectEventIdByLocalIdAndMap(sprite->data[0], sprite->data[1], sprite->data[2], &objectEventId))
        FieldEffectStop(sprite, FLDEFF_SHADOW);
    else
    {
        struct ObjectEvent *objectEvent = &gObjectEvents[objectEventId];
        struct Sprite *linkedSprite = &gSprites[objectEvent->spriteId];
        
        sprite->oam.priority = linkedSprite->oam.priority;
        sprite->x = linkedSprite->x;
        sprite->y = linkedSprite->y + sprite->data[3];
        sprite->invisible = linkedSprite->invisible;
        
        if (objectEvent->jumpDone)
        {
            objectEvent->jumpDone = FALSE;
            objectEvent->noShadow = TRUE;
        }
        
        if (!objectEvent->active || objectEvent->noShadow || objectEvent->inHotSprings || objectEvent->inSandPile || gWeatherPtr->noShadows
        || MetatileBehavior_IsPokeGrass(objectEvent->currentMetatileBehavior) || MetatileBehavior_IsSurfable(objectEvent->currentMetatileBehavior)
        || MetatileBehavior_IsReflective(objectEvent->currentMetatileBehavior) || MetatileBehavior_IsPuddle(objectEvent->currentMetatileBehavior)
        || MetatileBehavior_IsSurfable(objectEvent->previousMetatileBehavior) || MetatileBehavior_IsReflective(objectEvent->previousMetatileBehavior))
            FieldEffectStop(sprite, FLDEFF_SHADOW);
    }
}

void SetUpShadow(struct ObjectEvent *objectEvent)
{
    gFieldEffectArguments[0] = objectEvent->localId;
    gFieldEffectArguments[1] = objectEvent->mapNum;
    gFieldEffectArguments[2] = objectEvent->mapGroup;
    FldEff_Shadow();
}

///////////////////////
// FLDEFF_TALL_GRASS //
///////////////////////

static void SpriteCB_TallGrass(struct Sprite * sprite);
static void UpdateGrassFieldEffectSubpriority(struct Sprite * sprite, u32 z, u32 offset);

static const struct SpriteFrameImage sPicTable_TallGrass[] =
{
    overworld_frame(gFieldEffectObjectPic_TallGrass, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_TallGrass, 2, 2, 1),
    overworld_frame(gFieldEffectObjectPic_TallGrass, 2, 2, 2),
    overworld_frame(gFieldEffectObjectPic_TallGrass, 2, 2, 3),
    overworld_frame(gFieldEffectObjectPic_TallGrass, 2, 2, 4),
};

static const union AnimCmd sAnim_TallGrass[] =
{
    ANIMCMD_FRAME(1, 10),
    ANIMCMD_FRAME(2, 10),
    ANIMCMD_FRAME(3, 10),
    ANIMCMD_FRAME(4, 10),
    ANIMCMD_FRAME(0, 10),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_TallGrass[] =
{
    sAnim_TallGrass,
};

const struct SpriteTemplate gSpriteTemplate_TallGrass =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_1,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_TallGrass,
    .images = sPicTable_TallGrass,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_TallGrass,
};

static u32 FldEff_TallGrass(void)
{
    u32 spriteId;
    s16 x = gFieldEffectArguments[0];
    s16 y = gFieldEffectArguments[1];

    SetSpritePosToOffsetMapCoords(&x, &y, 8, 8);
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect1);
    
    spriteId = CreateSpriteAtEnd(&gSpriteTemplate_TallGrass, x, y, 0);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gFieldEffectArguments[3];
        sprite->data[0] = gFieldEffectArguments[2];
        sprite->data[1] = gFieldEffectArguments[0];
        sprite->data[2] = gFieldEffectArguments[1];
        sprite->data[3] = gFieldEffectArguments[4];
        sprite->data[4] = gFieldEffectArguments[5];
        sprite->data[5] = gFieldEffectArguments[6];
        
        if (gFieldEffectArguments[7]) // Is spawning
            SeekSpriteAnim(sprite, 4);
    }
    return 0;
}

static void SpriteCB_TallGrass(struct Sprite * sprite)
{
    u8 localId;
    u8 mapNum = sprite->data[5] >> 8;
    u8 mapGroup = sprite->data[5];
    u8 objectEventId;

    if (gCamera.active && (gSaveBlock1Ptr->location.mapNum != mapNum || gSaveBlock1Ptr->location.mapGroup != mapGroup))
    {
        sprite->data[1] -= gCamera.x;
        sprite->data[2] -= gCamera.y;
        sprite->data[5] = ((u8)gSaveBlock1Ptr->location.mapNum << 8) | (u8)gSaveBlock1Ptr->location.mapGroup;
    }
    localId = sprite->data[3] >> 8;
    mapNum = sprite->data[3];
    mapGroup = sprite->data[4];
    
    if (TryGetObjectEventIdByLocalIdAndMap(localId, mapNum, mapGroup, &objectEventId) || !MetatileBehavior_IsTallGrass_2(MapGridGetMetatileBehaviorAt(sprite->data[1], sprite->data[2]))
    || (sprite->data[7] && sprite->animEnded))
        FieldEffectStop(sprite, FLDEFF_TALL_GRASS);
    else
    {
        struct ObjectEvent *objectEvent = &gObjectEvents[objectEventId];
        
        if ((objectEvent->currentCoords.x != sprite->data[1] || objectEvent->currentCoords.y != sprite->data[2])
        && (objectEvent->previousCoords.x != sprite->data[1] || objectEvent->previousCoords.y != sprite->data[2]))
            sprite->data[7] = TRUE;

        UpdateObjectEventSpriteVisibility(sprite, FALSE);
        UpdateGrassFieldEffectSubpriority(sprite, sprite->data[0], sprite->animCmdIndex == 0 ? 4 : 0);
    }
}

static void UpdateGrassFieldEffectSubpriority(struct Sprite * sprite, u32 z, u32 offset)
{
    u32 i;
    s16 var, xhi, lyhi, yhi, ylo;

    SetObjectSubpriorityByZCoord(z, sprite, offset);
    
    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        struct ObjectEvent * objectEvent = &gObjectEvents[i];
        
        if (objectEvent->active)
        {
            struct Sprite *linkedSprite = &gSprites[objectEvent->spriteId];
            
            xhi = sprite->x + sprite->centerToCornerVecX;
            var = sprite->x - sprite->centerToCornerVecX;
            
            if (xhi < linkedSprite->x && var > linkedSprite->x)
            {
                lyhi = linkedSprite->y + linkedSprite->centerToCornerVecY;
                var = linkedSprite->y;
                
                ylo = sprite->y - sprite->centerToCornerVecY;
                yhi = ylo + linkedSprite->centerToCornerVecY;
                
                if ((lyhi < yhi || lyhi < ylo) && var > yhi && sprite->subpriority <= linkedSprite->subpriority)
                {
                    sprite->subpriority = linkedSprite->subpriority + 2;
                    break;
                }
            }
        }
    }
}

u32 FindTallGrassFieldEffectSpriteId(u32 localId, u32 mapNum, u32 mapGroup, s16 x, s16 y)
{
    u32 i;

    for (i = 0; i < MAX_SPRITES; i++)
    {
        struct Sprite *sprite = &gSprites[i];
        
        if (sprite->inUse)
        {
            if (sprite->callback == SpriteCB_TallGrass && (x == sprite->data[1] && y == sprite->data[2]) && (localId == (sprite->data[3] >> 8) && mapNum == (sprite->data[3] & 0xFF)
            && mapGroup == sprite->data[4]))
                return i;
        }
    }
    return MAX_SPRITES;
}

///////////////////
// FLDEFF_RIPPLE //
///////////////////

static void SpriteCB_WaitFieldEffectSpriteAnim(struct Sprite * sprite);

static const struct SpriteFrameImage sPicTable_Ripple[] =
{
    overworld_frame(gFieldEffectObjectPic_Ripple, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_Ripple, 2, 2, 1),
    overworld_frame(gFieldEffectObjectPic_Ripple, 2, 2, 2),
    overworld_frame(gFieldEffectObjectPic_Ripple, 2, 2, 3),
    overworld_frame(gFieldEffectObjectPic_Ripple, 2, 2, 4),
};

static const union AnimCmd sAnim_Ripple[] =
{
    ANIMCMD_FRAME(0, 12),
    ANIMCMD_FRAME(1, 9),
    ANIMCMD_FRAME(2, 9),
    ANIMCMD_FRAME(3, 9),
    ANIMCMD_FRAME(0, 9),
    ANIMCMD_FRAME(1, 9),
    ANIMCMD_FRAME(2, 11),
    ANIMCMD_FRAME(4, 11),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_Ripple[] =
{
    sAnim_Ripple,
};

static const struct SpriteTemplate sSpriteTemplate_Ripple =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_1,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_Ripple,
    .images = sPicTable_Ripple,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_WaitFieldEffectSpriteAnim,
};

static u32 FldEff_Ripple(void)
{
    u32 spriteId;
    
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect1);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_Ripple, gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2]);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gFieldEffectArguments[3];
        sprite->data[0] = FLDEFF_RIPPLE;
    }
    return 0;
}

static void SpriteCB_WaitFieldEffectSpriteAnim(struct Sprite * sprite)
{
    if (sprite->animEnded)
        FieldEffectStop(sprite, sprite->data[0]);
    else
        UpdateObjectEventSpriteVisibility(sprite, FALSE);
}

////////////////////////////////
// FLDEFF_FIELD_MOVE_SHOW_MON //
////////////////////////////////

static void Task_ShowMon_Outdoors(u32 taskId);
static void ShowMonEffect_Outdoors_1(struct Task * task);
static void ShowMonEffect_Outdoors_2(struct Task * task);
static void ShowMonEffect_Outdoors_3(struct Task * task);
static void ShowMonEffect_Outdoors_4(struct Task * task);
static void ShowMonEffect_Outdoors_5(struct Task * task);
static void ShowMonEffect_Outdoors_6(struct Task * task);
static void ShowMonEffect_Outdoors_7(struct Task * task);
static void Task_ShowMon_Indoors(u32 taskId);
static void ShowMonEffect_Indoors_1(struct Task * task);
static void ShowMonEffect_Indoors_2(struct Task * task);
static void ShowMonEffect_Indoors_3(struct Task * task);
static void ShowMonEffect_Indoors_4(struct Task * task);
static void ShowMonEffect_Indoors_5(struct Task * task);
static void ShowMonEffect_Indoors_6(struct Task * task);
static void ShowMonEffect_Indoors_7(struct Task * task);
static void SpriteCB_FieldMoveMonSlideOnscreen(struct Sprite * sprite);
static void SpriteCB_FieldMoveMonWaitAfterCry(struct Sprite * sprite);
static void SpriteCB_FieldMoveMonSlideOffscreen(struct Sprite * sprite);

static void (*const sShowMonOutdoorsEffectFuncs[])(struct Task * task) = {
    ShowMonEffect_Outdoors_1,
    ShowMonEffect_Outdoors_2,
    ShowMonEffect_Outdoors_3,
    ShowMonEffect_Outdoors_4,
    ShowMonEffect_Outdoors_5,
    ShowMonEffect_Outdoors_6,
    ShowMonEffect_Outdoors_7
};

static void (*const sShowMonIndoorsEffectFuncs[])(struct Task * ) = {
    ShowMonEffect_Indoors_1,
    ShowMonEffect_Indoors_2,
    ShowMonEffect_Indoors_3,
    ShowMonEffect_Indoors_4,
    ShowMonEffect_Indoors_5,
    ShowMonEffect_Indoors_6,
    ShowMonEffect_Indoors_7
};

static u32 CreateMonSprite_FieldMove(u32 species, bool32 isShiny, u32 personality, s16 x, s16 y)
{
    u32 spriteId = CreateMonPicSprite(species, isShiny, personality, 1, x, y, 0, species);
    return spriteId == 0xFFFF ? MAX_SPRITES : spriteId;
}

static u32 InitFieldMoveMonSprite(u32 species, bool32 isShiny, u32 personality)
{
    bool16 noDucking = (species & SHOW_MON_CRY_NO_DUCKING) >> 16;
    u32 monSprite;
    struct Sprite * sprite;
    
    species &= ~(SHOW_MON_CRY_NO_DUCKING);
    monSprite = CreateMonSprite_FieldMove(species, isShiny, personality, 0x140, 0x50);
    
    sprite = &gSprites[monSprite];
    sprite->callback = SpriteCallbackDummy;
    sprite->oam.priority = 0;
    sprite->data[0] = species;
    sprite->data[6] = noDucking;
    
    return monSprite;
}

static u32 FldEff_FieldMoveShowMon(void)
{
    u32 taskId = CreateTask(IsMapTypeOutdoors(GetCurrentMapType()) ? Task_ShowMon_Outdoors : Task_ShowMon_Indoors, 0xFF);
    gTasks[taskId].data[15] = InitFieldMoveMonSprite(gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2]);
    return 0;
}

static void Task_ShowMon_Outdoors(u32 taskId)
{
    sShowMonOutdoorsEffectFuncs[gTasks[taskId].data[0]](&gTasks[taskId]);
}

static void VBlankCB_ShowMonEffect_Outdoors(void)
{
    IntrCallback callback;
    struct Task * task = &gTasks[FindTaskIdByFunc(Task_ShowMon_Outdoors)];
    
    LoadWordFromTwoHalfwords((u16 *)&task->data[13], (uintptr_t *)&callback);
    callback();
    
    SetGpuReg(REG_OFFSET_WIN0H, task->data[1]);
    SetGpuReg(REG_OFFSET_WIN0V, task->data[2]);
    SetGpuReg(REG_OFFSET_WININ, task->data[3]);
    SetGpuReg(REG_OFFSET_WINOUT, task->data[4]);
    SetGpuReg(REG_OFFSET_BG0HOFS, task->data[5]);
    SetGpuReg(REG_OFFSET_BG0VOFS, task->data[6]);
}

static void ShowMonEffect_Outdoors_1(struct Task * task)
{
    task->data[11] = GetGpuReg(REG_OFFSET_WININ);
    task->data[12] = GetGpuReg(REG_OFFSET_WINOUT);
    StoreWordInTwoHalfwords((u16*)&task->data[13], (u32)gMain.vblankCallback);
    
    task->data[1] = WIN_RANGE(0xF0, 0xF1);
    task->data[2] = WIN_RANGE(0x50, 0x51);
    task->data[3] = WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR;
    task->data[4] = WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 | WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR;
    
    SetGpuReg(REG_OFFSET_WIN0H, task->data[1]);
    SetGpuReg(REG_OFFSET_WIN0V, task->data[2]);
    SetGpuReg(REG_OFFSET_WININ, task->data[3]);
    SetGpuReg(REG_OFFSET_WINOUT, task->data[4]);
    
    SetVBlankCallback(VBlankCB_ShowMonEffect_Outdoors);
    
    task->data[0]++;
}

static void LoadFieldMoveStreaksTilemapToVram(u16 screenbase)
{
    u32 i;
    u16 *dest = (u16 *)(VRAM + (10 * 32) + screenbase);
    
    for (i = 0; i < (10 * 32); i++, dest++)
        *dest = sFieldMoveStreaksTilemap[i] | METATILE_ELEVATION_MASK;
}

static void ShowMonEffect_Outdoors_2(struct Task * task)
{
    u16 charbase = ((GetGpuReg(REG_OFFSET_BG0CNT) >> 2) << 14);
    u16 screenbase = ((GetGpuReg(REG_OFFSET_BG0CNT) >> 8) << 11);
    
    CpuCopy16(sFieldMoveStreaksTiles, (void *)(VRAM + charbase), 0x200);
    CpuFill32(0, (void *)(VRAM + screenbase), 0x800);
    
    LoadPalette(sFieldMoveStreaksPalette, 0xf0, 0x20);
    LoadFieldMoveStreaksTilemapToVram(screenbase);
    
    task->data[0]++;
}

static void ShowMonEffect_Outdoors_3(struct Task * task)
{
    s16 win0h_lo;
    s16 win0v_lo;
    s16 win0v_hi;
    
    task->data[5] -= 16;
    
    win0h_lo = ((u16)task->data[1] >> 8);
    win0v_lo = ((u16)task->data[2] >> 8);
    win0v_hi = ((u16)task->data[2] & 0xff);
    win0h_lo -= 16;
    win0v_lo -= 2;
    win0v_hi += 2;
    
    if (win0h_lo < 0)
        win0h_lo = 0;
    
    if (win0v_lo < 0x28)
        win0v_lo = 0x28;
    
    if (win0v_hi > 0x78)
        win0v_hi = 0x78;

    task->data[1] = WIN_RANGE(win0h_lo, task->data[1] & 0xff);
    task->data[2] = WIN_RANGE(win0v_lo, win0v_hi);
    
    if (win0h_lo == 0 && win0v_lo == 0x28 && win0v_hi == 0x78)
    {
        gSprites[task->data[15]].callback = SpriteCB_FieldMoveMonSlideOnscreen;
        task->data[0]++;
    }
}

static void ShowMonEffect_Outdoors_4(struct Task * task)
{
    task->data[5] -= 16;
    
    if (gSprites[task->data[15]].data[7])
        task->data[0]++;
}

static void ShowMonEffect_Outdoors_5(struct Task * task)
{
    s16 win0v_lo;
    s16 win0v_hi;
    
    task->data[5] -= 16;
    
    win0v_lo = (task->data[2] >> 8);
    win0v_hi = (task->data[2] & 0xff);
    win0v_lo += 6;
    win0v_hi -= 6;
    
    if (win0v_lo > 0x50)
        win0v_lo = 0x50;

    if (win0v_hi < 0x51)
        win0v_hi = 0x51;

    task->data[2] = WIN_RANGE(win0v_lo, win0v_hi);
    
    if (win0v_lo == 0x50 && win0v_hi == 0x51)
        task->data[0]++;
}

static void ShowMonEffect_Outdoors_6(struct Task * task)
{
    u16 bg0cnt = (GetGpuReg(REG_OFFSET_BG0CNT) >> 8) << 11;
    
    CpuFill32(0, (void *)VRAM + bg0cnt, 0x800);
    
    task->data[1] = WIN_RANGE(0x00, 0xf1);
    task->data[2] = WIN_RANGE(0x00, 0xa1);
    task->data[3] = task->data[11];
    task->data[4] = task->data[12];
    task->data[0]++;
}

static void ShowMonEffect_Outdoors_7(struct Task * task)
{
    IntrCallback callback;
    
    LoadWordFromTwoHalfwords((u16 *)&task->data[13], (uintptr_t *)&callback);
    SetVBlankCallback(callback);
    
    ChangeBgX(0, 0, 0);
    ChangeBgY(0, 0, 0);
    
    Menu_LoadStdPalAt(STD_WINDOW_PALETTE_NUM * 0x10, 0x14);
    FreeResourcesAndDestroySprite(&gSprites[task->data[15]], task->data[15]);
    FieldEffectActiveListRemove(FLDEFF_FIELD_MOVE_SHOW_MON);
    DestroyTask(FindTaskIdByFunc(Task_ShowMon_Outdoors));
}

static void Task_ShowMon_Indoors(u32 taskId)
{
    sShowMonIndoorsEffectFuncs[gTasks[taskId].data[0]](&gTasks[taskId]);
}

static void VBlankCB_ShowMonEffect_Indoors(void)
{
    IntrCallback intrCallback;
    struct Task * task = &gTasks[FindTaskIdByFunc(Task_ShowMon_Indoors)];
    
    LoadWordFromTwoHalfwords((u16 *)&task->data[13], (uintptr_t *)&intrCallback);
    intrCallback();
    
    SetGpuReg(REG_OFFSET_BG0HOFS, task->data[1]);
    SetGpuReg(REG_OFFSET_BG0VOFS, task->data[2]);
}

static void ShowMonEffect_Indoors_1(struct Task * task)
{
    SetGpuReg(REG_OFFSET_BG0HOFS, task->data[1]);
    SetGpuReg(REG_OFFSET_BG0VOFS, task->data[2]);
    StoreWordInTwoHalfwords((u16 *)&task->data[13], (u32)gMain.vblankCallback);
    SetVBlankCallback(VBlankCB_ShowMonEffect_Indoors);
    task->data[0]++;
}

static void ShowMonEffect_Indoors_2(struct Task * task)
{
    u16 charbase = ((GetGpuReg(REG_OFFSET_BG0CNT) >> 2) << 14);
    u16 screenbase = ((GetGpuReg(REG_OFFSET_BG0CNT) >> 8) << 11);

    task->data[12] = screenbase;
    
    CpuCopy16(sDarknessFieldMoveStreaksTiles, (void *)(VRAM + charbase), 0x80);
    CpuFill32(0, (void *)(VRAM + screenbase), 0x800);
    
    LoadPalette(sDarknessFieldMoveStreaksPalette, 0xf0, 0x20);
    
    task->data[0]++;
}

static bool32 SlideIndoorBannerOnscreen(struct Task * task)
{
    u32 i;
    u16 srcOffs;
    u16 dstOffs;
    u16 *dest;
    
    if (task->data[4] >= 32)
        return TRUE;

    dstOffs = (task->data[3] >> 3) & 0x1f;
    if (dstOffs >= task->data[4])
    {
        dstOffs = (32 - dstOffs) & 0x1f;
        srcOffs = (32 - task->data[4]) & 0x1f;
        dest = (u16 *)(VRAM + 0x140 + (u16)task->data[12]);
        
        for (i = 0; i < 10; i++)
        {
            dest[dstOffs + i * 32] = sDarknessFieldMoveStreaksTilemap[srcOffs + i * 32];
            dest[dstOffs + i * 32] |= 0xf000;

            dest[((dstOffs + 1) & 0x1f) + i * 32] = sDarknessFieldMoveStreaksTilemap[((srcOffs + 1) & 0x1f) + i * 32] | 0xf000;
            dest[((dstOffs + 1) & 0x1f) + i * 32] |= 0xf000;
        }
        task->data[4] += 2;
    }
    return FALSE;
}

static bool32 SlideIndoorBannerOffscreen(struct Task * task)
{
    u32 i;
    u16 dstOffs;
    u16 *dest;
    
    if (task->data[4] >= 32)
        return TRUE;

    dstOffs = task->data[3] >> 3;
    if (dstOffs >= task->data[4])
    {
        dstOffs = (task->data[1] >> 3) & 0x1f;
        dest = (u16 *)(VRAM + 0x140 + (u16)task->data[12]);
        
        for (i = 0; i < 10; i++)
        {
            dest[dstOffs + i * 32] = 0xf000;
            dest[((dstOffs + 1) & 0x1f) + i * 32] = 0xf000;
        }
        task->data[4] += 2;
    }
    return FALSE;
}

static void AnimateIndoorShowMonBg(struct Task * task)
{
    task->data[1] -= 16;
    task->data[3] += 16;
}

static void ShowMonEffect_Indoors_3(struct Task * task)
{
    if (SlideIndoorBannerOnscreen(task))
    {
        task->data[5] = GetGpuReg(REG_OFFSET_WININ);
        
        SetGpuReg(REG_OFFSET_WININ, (task->data[5] & 0xFF) | WININ_WIN1_BG0 | WININ_WIN1_OBJ);
        SetGpuReg(REG_OFFSET_WIN1H, WIN_RANGE(0x00, 0xf0));
        SetGpuReg(REG_OFFSET_WIN1V, WIN_RANGE(0x28, 0x78));
        
        gSprites[task->data[15]].callback = SpriteCB_FieldMoveMonSlideOnscreen;
        task->data[0]++;
    }
    AnimateIndoorShowMonBg(task);
}

static void ShowMonEffect_Indoors_4(struct Task * task)
{
    AnimateIndoorShowMonBg(task);
    
    if (gSprites[task->data[15]].data[7])
        task->data[0]++;
}

static void ShowMonEffect_Indoors_5(struct Task * task)
{
    AnimateIndoorShowMonBg(task);
    
    task->data[3] = task->data[1] & 7;
    task->data[4] = 0;
    
    SetGpuReg(REG_OFFSET_WIN1H, WIN_RANGE(0xff, 0xff));
    SetGpuReg(REG_OFFSET_WIN1V, WIN_RANGE(0xff, 0xff));
    SetGpuReg(REG_OFFSET_WININ, task->data[5]);
    
    task->data[0]++;
}

static void ShowMonEffect_Indoors_6(struct Task * task)
{
    AnimateIndoorShowMonBg(task);
    
    if (SlideIndoorBannerOffscreen(task))
        task->data[0]++;
}

static void ShowMonEffect_Indoors_7(struct Task * task)
{
    IntrCallback intrCallback;
    u16 charbase = (GetGpuReg(REG_OFFSET_BG0CNT) >> 8) << 11;

    CpuFill32(0, (void *)VRAM + charbase, 0x800);
    LoadWordFromTwoHalfwords((u16 *)&task->data[13], (uintptr_t *)&intrCallback);
    SetVBlankCallback(intrCallback);
    
    ChangeBgX(0, 0, 0);
    ChangeBgY(0, 0, 0);
    
    Menu_LoadStdPalAt(STD_WINDOW_PALETTE_NUM * 0x10, 0x14);
    FreeResourcesAndDestroySprite(&gSprites[task->data[15]], task->data[15]);
    FieldEffectActiveListRemove(FLDEFF_FIELD_MOVE_SHOW_MON);
    DestroyTask(FindTaskIdByFunc(Task_ShowMon_Indoors));
}

static void SpriteCB_FieldMoveMonSlideOnscreen(struct Sprite * sprite)
{
    sprite->x -= 20;
    
    if (sprite->x <= 0x78)
    {
        sprite->x = 0x78;
        sprite->data[1] = 30;
        sprite->callback = SpriteCB_FieldMoveMonWaitAfterCry;
        
        if (sprite->data[6])
            PlayCry_NormalNoDucking(sprite->data[0], 0, 125, CRY_PRIORITY_NORMAL);
        else
            PlayCry_Normal(sprite->data[0], 0);
    }
}

static void SpriteCB_FieldMoveMonWaitAfterCry(struct Sprite * sprite)
{
    if (--sprite->data[1] == 0)
        sprite->callback = SpriteCB_FieldMoveMonSlideOffscreen;
}

static void SpriteCB_FieldMoveMonSlideOffscreen(struct Sprite * sprite)
{
    if (sprite->x < -0x40)
        sprite->data[7] = 1;
    else
        sprite->x -= 20;
}

////////////////
// FLDEFF_ASH //
////////////////

static void SpriteCB_Ash(struct Sprite * sprite);
static void UpdateAshFieldEffect_Step0(struct Sprite * sprite);
static void UpdateAshFieldEffect_Step1(struct Sprite * sprite);
static void UpdateAshFieldEffect_Step2(struct Sprite * sprite);

static const struct SpriteFrameImage sPicTable_Ash[] =
{
    overworld_frame(gFieldEffectObjectPic_Ash, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_Ash, 2, 2, 1),
    overworld_frame(gFieldEffectObjectPic_Ash, 2, 2, 2),
    overworld_frame(gFieldEffectObjectPic_Ash, 2, 2, 3),
    overworld_frame(gFieldEffectObjectPic_Ash, 2, 2, 4),
};

static const union AnimCmd sAnim_Ash[] =
{
    ANIMCMD_FRAME(0, 12),
    ANIMCMD_FRAME(1, 12),
    ANIMCMD_FRAME(2, 8),
    ANIMCMD_FRAME(3, 12),
    ANIMCMD_FRAME(4, 12),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_Ash[] =
{
    sAnim_Ash,
};

static const struct SpriteTemplate sSpriteTemplate_Ash =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_1,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_Ash,
    .images = sPicTable_Ash,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Ash,
};

static void (*const sAshFieldEffectFuncs[])(struct Sprite * sprite) = {
    UpdateAshFieldEffect_Step0,
    UpdateAshFieldEffect_Step1,
    UpdateAshFieldEffect_Step2
};

static u32 FldEff_Ash(void)
{
    s16 x = gFieldEffectArguments[0];
    s16 y = gFieldEffectArguments[1];
    u32 spriteId;

    SetSpritePosToOffsetMapCoords(&x, &y, 8, 8);
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect1);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_Ash, x, y, gFieldEffectArguments[2]);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gFieldEffectArguments[3];
        sprite->data[1] = gFieldEffectArguments[0];
        sprite->data[2] = gFieldEffectArguments[1];
        sprite->data[3] = gFieldEffectArguments[4];
        sprite->data[4] = gFieldEffectArguments[5];
    }
    return 0;
}

static void SpriteCB_Ash(struct Sprite * sprite)
{
    sAshFieldEffectFuncs[sprite->data[0]](sprite);
}

static void UpdateAshFieldEffect_Step0(struct Sprite * sprite)
{
    sprite->invisible = TRUE;
    sprite->animPaused = TRUE;
    
    if (--sprite->data[4] == 0)
        sprite->data[0] = 1;
}

static void UpdateAshFieldEffect_Step1(struct Sprite * sprite)
{
    sprite->invisible = FALSE;
    sprite->animPaused = FALSE;
    
    MapGridSetMetatileIdAt(sprite->data[1], sprite->data[2], sprite->data[3]);
    CurrentMapDrawMetatileAt(sprite->data[1], sprite->data[2]);
    
    gObjectEvents[gPlayerAvatar.objectEventId].triggerGroundEffectsOnMove = TRUE;
    sprite->data[0] = 2;
}

static void UpdateAshFieldEffect_Step2(struct Sprite * sprite)
{
    UpdateObjectEventSpriteVisibility(sprite, FALSE);
    
    if (sprite->animEnded)
        FieldEffectStop(sprite, FLDEFF_ASH);
}

void StartAshFieldEffect(s16 x, s16 y, u32 metatileId, s16 d)
{
    gFieldEffectArguments[0] = x;
    gFieldEffectArguments[1] = y;
    gFieldEffectArguments[2] = 0x52;
    gFieldEffectArguments[3] = 1;
    gFieldEffectArguments[4] = metatileId;
    gFieldEffectArguments[5] = d;
    FieldEffectStart(FLDEFF_ASH);
}

//////////////////////
// FLDEFF_SURF_BLOB //
//////////////////////

static void SpriteCB_SurfBlob(struct Sprite * sprite);
static void SynchroniseSurfAnim(struct ObjectEvent * objectEvent, struct Sprite * sprite);
static void SynchroniseSurfPosition(struct ObjectEvent * objectEvent, struct Sprite * sprite);
static void CreateBobbingEffect(struct ObjectEvent * objectEvent, struct Sprite * linkedSprite, struct Sprite * sprite);

static const struct SpriteFrameImage sPicTable_SurfBlob[] =
{
    overworld_frame(gObjectEventPic_SurfBlob, 2, 8, 0),
    overworld_frame(gObjectEventPic_SurfBlob, 2, 8, 1),
    overworld_frame(gObjectEventPic_SurfBlob, 2, 8, 2),
    overworld_frame(gObjectEventPic_SurfBlob, 2, 8, 3),
    overworld_frame(gObjectEventPic_SurfBlob, 2, 8, 4),
    overworld_frame(gObjectEventPic_SurfBlob, 2, 8, 5),
};

static const union AnimCmd sSurfBlobAnim_FaceSouth[] =
{
    ANIMCMD_FRAME(0, 48),
    ANIMCMD_FRAME(1, 48),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sSurfBlobAnim_FaceNorth[] =
{
    ANIMCMD_FRAME(2, 48),
    ANIMCMD_FRAME(3, 48),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sSurfBlobAnim_FaceWest[] =
{
    ANIMCMD_FRAME(4, 48),
    ANIMCMD_FRAME(5, 48),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sSurfBlobAnim_FaceEast[] =
{
    ANIMCMD_FRAME(4, 48, .hFlip = TRUE),
    ANIMCMD_FRAME(5, 48, .hFlip = TRUE),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnimTable_SurfBlob[] =
{
    [DIR_SOUTH - 1] = sSurfBlobAnim_FaceSouth,
    [DIR_NORTH - 1] = sSurfBlobAnim_FaceNorth,
    [DIR_WEST  - 1] = sSurfBlobAnim_FaceWest,
    [DIR_EAST  - 1] = sSurfBlobAnim_FaceEast,
};

static const struct SpriteTemplate sSpriteTemplate_SurfBlob =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = OBJ_EVENT_PAL_TAG_PLAYER_RED,
    .oam = &gObjectEventBaseOam_32x32,
    .anims = sAnimTable_SurfBlob,
    .images = sPicTable_SurfBlob,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_SurfBlob,
};

static u32 FldEff_SurfBlob(void)
{
    u32 spriteId;

    SetSpritePosToOffsetMapCoords((s16 *)&gFieldEffectArguments[0], (s16 *)&gFieldEffectArguments[1], 8, 8);
    LoadFieldEffectPalette(&sSpriteTemplate_SurfBlob);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_SurfBlob, gFieldEffectArguments[0], gFieldEffectArguments[1], 0x96);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        sprite->coordOffsetEnabled = TRUE;
        sprite->data[2] = gFieldEffectArguments[2];
        sprite->data[3] = 0;
        sprite->data[6] = -1;
        sprite->data[7] = -1;
    }
    FieldEffectActiveListRemove(FLDEFF_SURF_BLOB);
    
    return spriteId;
}

static void SpriteCB_SurfBlob(struct Sprite * sprite)
{
    struct ObjectEvent * objectEvent = &gObjectEvents[sprite->data[2]];
    struct Sprite * linkedSprite = &gSprites[objectEvent->spriteId];

    SynchroniseSurfAnim(objectEvent, sprite);
    SynchroniseSurfPosition(objectEvent, sprite);
    CreateBobbingEffect(objectEvent, linkedSprite, sprite);
    sprite->oam.priority = linkedSprite->oam.priority;
}

static void SynchroniseSurfAnim(struct ObjectEvent * objectEvent, struct Sprite * sprite)
{
    u8 surfBlobDirectionAnims[] = {
        [DIR_NONE] = 0,
        [DIR_SOUTH] = 0,
        [DIR_NORTH] = 1,
        [DIR_WEST] = 2,
        [DIR_EAST] = 3
    };

    if (((sprite->data[0] & 0xF0) >> 4) == 0)
        StartSpriteAnimIfDifferent(sprite, surfBlobDirectionAnims[objectEvent->movementDirection]);
}

static void SynchroniseSurfPosition(struct ObjectEvent * objectEvent, struct Sprite * sprite)
{
    u32 i;
    s16 x = objectEvent->currentCoords.x;
    s16 y = objectEvent->currentCoords.y;
    s32 spriteY = sprite->y2;

    if (spriteY == 0 && (x != sprite->data[6] || y != sprite->data[7]))
    {
        sprite->data[5] = spriteY;
        sprite->data[6] = x;
        sprite->data[7] = y;
        
        for (i = DIR_SOUTH; i <= DIR_EAST; i++, x = sprite->data[6], y = sprite->data[7])
        {
            MoveCoords(i, &x, &y);
            
            if (MapGridGetZCoordAt(x, y) == 3)
            {
                sprite->data[5]++;
                break;
            }
        }
    }
}

static void CreateBobbingEffect(struct ObjectEvent * objectEvent, struct Sprite * linkedSprite, struct Sprite * sprite)
{
    u16 unk_83FECFA[] = {7, 15};
    u8 v0 = sprite->data[0] & 0xF;
    
    if (v0 != 0)
    {
        if (((u16)(++ sprite->data[4]) & unk_83FECFA[sprite->data[5]]) == 0)
            sprite->y2 += sprite->data[3];

        if ((sprite->data[4] & 0x1F) == 0)
            sprite->data[3] = -sprite->data[3];

        if (v0 != 2)
        {
            linkedSprite->y2 = ((sprite->data[0] & 0xF00) >> 8) == 0 ? sprite->y2 : sprite->data[1] + sprite->y2;
            
            if (sprite->animCmdIndex != 0)
                linkedSprite->y2++;
            
            sprite->x = linkedSprite->x;
            sprite->y = linkedSprite->y + 8;
        }
    }
}

void SetSurfBlob_BobState(u32 spriteId, u8 value)
{
    gSprites[spriteId].data[0] = (gSprites[spriteId].data[0] & ~0xF) | (value & 0xF);
}

void SetSurfBlob_DontSyncAnim(u32 spriteId, u8 value)
{
    gSprites[spriteId].data[0] = (gSprites[spriteId].data[0] & ~0xF0) | ((value & 0xF) << 4);
}

void SetSurfBlob_PlayerOffset(u32 spriteId, u8 hasOffset, s16 offset)
{
    gSprites[spriteId].data[0] = (gSprites[spriteId].data[0] & ~0xF00) | ((hasOffset & 0xF) << 8);
    gSprites[spriteId].data[1] = offset;
}

/////////////////////
// FLDEFF_USE_SURF //
/////////////////////

static void Task_FldEffUseSurf(u32 taskId);
static void UseSurfEffect_1(struct Task * task);
static void UseSurfEffect_2(struct Task * task);
static void UseSurfEffect_3(struct Task * task);
static void UseSurfEffect_4(struct Task * task);
static void UseSurfEffect_5(struct Task * task);

static void (*const sUseSurfEffectFuncs[])(struct Task * ) = {
    UseSurfEffect_1,
    UseSurfEffect_2,
    UseSurfEffect_3,
    UseSurfEffect_4,
    UseSurfEffect_5,
};

static u32 FldEff_UseSurf(void)
{
    gTasks[CreateTask(Task_FldEffUseSurf, 0xff)].data[15] = gFieldEffectArguments[0];
    
    Overworld_ClearSavedMusic();
    
    if (Overworld_MusicCanOverrideMapMusic(MUS_SURF))
        Overworld_ChangeMusicTo(MUS_SURF);
    
    return 0;
}

static void Task_FldEffUseSurf(u32 taskId)
{
    sUseSurfEffectFuncs[gTasks[taskId].data[0]](&gTasks[taskId]);
}

static void UseSurfEffect_1(struct Task * task)
{
    ScriptContext2_Enable();
    FreezeObjectEvents();
    gPlayerAvatar.preventStep = TRUE;
    SetPlayerAvatarStateMask(PLAYER_AVATAR_FLAG_SURFING);
    PlayerGetDestCoords(&task->data[1], &task->data[2]);
    MoveCoords(gObjectEvents[gPlayerAvatar.objectEventId].movementDirection, &task->data[1], &task->data[2]);
    task->data[0]++;
}

static void UseSurfEffect_2(struct Task * task)
{
    struct ObjectEvent * objectEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    if (!ObjectEventIsMovementOverridden(objectEvent) || ObjectEventClearHeldMovementIfFinished(objectEvent))
    {
        StartPlayerAvatarSummonMonForFieldMoveAnim();
        ObjectEventSetHeldMovement(objectEvent, MOVEMENT_ACTION_START_ANIM_IN_DIRECTION);
        task->data[0]++;
    }
}

static void UseSurfEffect_3(struct Task * task)
{
    if (ObjectEventCheckHeldMovementStatus(&gObjectEvents[gPlayerAvatar.objectEventId]))
    {
        gFieldEffectArguments[0] = task->data[15] | SHOW_MON_CRY_NO_DUCKING;
        FieldEffectStart(FLDEFF_FIELD_MOVE_SHOW_MON_INIT);
        task->data[0]++;
    }
}

static void UseSurfEffect_4(struct Task * task)
{
    if (!FieldEffectActiveListContains(FLDEFF_FIELD_MOVE_SHOW_MON))
    {
        struct ObjectEvent *objectEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
        
        ObjectEventSetGraphicsId(objectEvent, GetPlayerAvatarGraphicsIdByStateId(PLAYER_AVATAR_GFX_RIDE));
        ObjectEventClearHeldMovementIfFinished(objectEvent);
        ObjectEventSetHeldMovement(objectEvent, GetJumpSpecialMovementAction(objectEvent->movementDirection));
        
        gFieldEffectArguments[0] = task->data[1];
        gFieldEffectArguments[1] = task->data[2];
        gFieldEffectArguments[2] = gPlayerAvatar.objectEventId;
        objectEvent->fieldEffectSpriteId = FieldEffectStart(FLDEFF_SURF_BLOB);
        task->data[0]++;
    }
}

static void UseSurfEffect_5(struct Task * task)
{
    struct ObjectEvent * objectEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    if (ObjectEventClearHeldMovementIfFinished(objectEvent))
    {
        gPlayerAvatar.preventStep = FALSE;
        gPlayerAvatar.flags &= ~(PLAYER_AVATAR_FLAG_CONTROLLABLE);
        
        ObjectEventSetHeldMovement(objectEvent, GetFaceDirectionMovementAction(objectEvent->movementDirection));
        SetSurfBlob_BobState(objectEvent->fieldEffectSpriteId, 1);
        
        UnfreezeObjectEvents();
        ScriptContext2_Disable();
        
        FieldEffectActiveListRemove(FLDEFF_USE_SURF);
        
        DestroyTask(FindTaskIdByFunc(Task_FldEffUseSurf));
    }
}

/////////////////
// FLDEFF_DUST //
/////////////////

static void SpriteCB_JumpImpact(struct Sprite *sprite);

static const struct SpriteFrameImage sPicTable_GroundImpactDust[] =
{
    overworld_frame(gFieldEffectObjectPic_GroundImpactDust, 2, 1, 0),
    overworld_frame(gFieldEffectObjectPic_GroundImpactDust, 2, 1, 1),
    overworld_frame(gFieldEffectObjectPic_GroundImpactDust, 2, 1, 2),
};

static const union AnimCmd sAnim_GroundImpactDust[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(1, 8),
    ANIMCMD_FRAME(2, 8),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_GroundImpactDust[] =
{
    sAnim_GroundImpactDust,
};

static const struct SpriteTemplate sSpriteTemplate_GroundImpactDust =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_0,
    .oam = &gObjectEventBaseOam_16x8,
    .anims = sAnimTable_GroundImpactDust,
    .images = sPicTable_GroundImpactDust,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_JumpImpact,
};

static u32 FldEff_Dust(void)
{
    u32 spriteId;

    SetSpritePosToOffsetMapCoords((s16 *)&gFieldEffectArguments[0], (s16 *)&gFieldEffectArguments[1], 8, 12);
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect0);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_GroundImpactDust, gFieldEffectArguments[0], gFieldEffectArguments[1], 0);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gFieldEffectArguments[3];
        sprite->data[0] = gFieldEffectArguments[2];
        sprite->data[1] = FLDEFF_DUST;
    }
    return 0;
}

static void SpriteCB_JumpImpact(struct Sprite *sprite)
{
    if (sprite->animEnded)
        FieldEffectStop(sprite, sprite->data[1]);
    else
    {
        UpdateObjectEventSpriteVisibility(sprite, FALSE);
        SetObjectSubpriorityByZCoord(sprite->data[0], sprite, 0);
    }
}

//////////////////////////////////
// FLDEFF_DOUBLE_EXCL_MARK_ICON //
//////////////////////////////////

static u32 FldEff_DoubleExclMarkIcon(void)
{
    CreateEmoticonSprite(FLDEFF_DOUBLE_EXCL_MARK_ICON, 0x52, 1);
    return 0;
}

////////////////////////////
// FLDEFF_JUMP_TALL_GRASS //
////////////////////////////

static const struct SpriteFrameImage sPicTable_JumpTallGrass[] =
{
    overworld_frame(gFieldEffectObjectPic_JumpTallGrass, 2, 1, 0),
    overworld_frame(gFieldEffectObjectPic_JumpTallGrass, 2, 1, 1),
    overworld_frame(gFieldEffectObjectPic_JumpTallGrass, 2, 1, 2),
    overworld_frame(gFieldEffectObjectPic_JumpTallGrass, 2, 1, 3),
};

static const union AnimCmd sAnim_JumpTallGrass[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(1, 8),
    ANIMCMD_FRAME(2, 8),
    ANIMCMD_FRAME(3, 8),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_JumpTallGrass[] =
{
    sAnim_JumpTallGrass,
};

const struct SpriteTemplate sSpriteTemplate_JumpTallGrass =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_1,
    .oam = &gObjectEventBaseOam_16x8,
    .anims = sAnimTable_JumpTallGrass,
    .images = sPicTable_JumpTallGrass,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_JumpImpact,
};

static u32 FldEff_JumpTallGrass(void)
{
    u32 spriteId;

    SetSpritePosToOffsetMapCoords((s16 *)&gFieldEffectArguments[0], (s16 *)&gFieldEffectArguments[1], 8, 12);
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect1);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_JumpTallGrass, gFieldEffectArguments[0], gFieldEffectArguments[1], 0);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gFieldEffectArguments[3];
        sprite->data[0] = gFieldEffectArguments[2];
        sprite->data[1] = FLDEFF_JUMP_TALL_GRASS;
    }
    return 0;
}

////////////////////////////
// FLDEFF_SAND_FOOTPRINTS //
////////////////////////////

static void SpriteCB_UpdateFootprintsTireTracks(struct Sprite *sprite);
static void FadeFootprintsTireTracks_Step0(struct Sprite * sprite);
static void FadeFootprintsTireTracks_Step1(struct Sprite * sprite);

static const struct SpriteFrameImage sPicTable_SandFootprints[] =
{
    overworld_frame(gFieldEffectObjectPic_SandFootprints, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_SandFootprints, 2, 2, 1),
};

static const union AnimCmd sSandFootprintsAnim_South[] =
{
    ANIMCMD_FRAME(0, 1, .vFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd sSandFootprintsAnim_North[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

static const union AnimCmd sSandFootprintsAnim_West[] =
{
    ANIMCMD_FRAME(1, 1),
    ANIMCMD_END,
};

static const union AnimCmd sSandFootprintsAnim_East[] =
{
    ANIMCMD_FRAME(1, 1, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_SandFootprints[] =
{
    [DIR_NONE]  = sSandFootprintsAnim_South,
    [DIR_SOUTH] = sSandFootprintsAnim_South,
    [DIR_NORTH] = sSandFootprintsAnim_North,
    [DIR_WEST]  = sSandFootprintsAnim_West,
    [DIR_EAST]  = sSandFootprintsAnim_East,
};

static const struct SpriteTemplate sSpriteTemplate_SandFootprints =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_0,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_SandFootprints,
    .images = sPicTable_SandFootprints,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_UpdateFootprintsTireTracks,
};

static void (*const sFadeFootprintsTireTracksFuncs[])(struct Sprite * sprite) = {
    FadeFootprintsTireTracks_Step0,
    FadeFootprintsTireTracks_Step1
};

static u32 FldEff_SandFootprints(void)
{
    u32 spriteId;

    SetSpritePosToOffsetMapCoords((s16 *)&gFieldEffectArguments[0], (s16 *)&gFieldEffectArguments[1], 8, 8);
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect0);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_SandFootprints, gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2]);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gFieldEffectArguments[3];
        sprite->data[7] = FLDEFF_SAND_FOOTPRINTS;
        StartSpriteAnim(sprite, gFieldEffectArguments[4]);
    }
    return 0;
}

static void SpriteCB_UpdateFootprintsTireTracks(struct Sprite *sprite)
{
    sFadeFootprintsTireTracksFuncs[sprite->data[0]](sprite);
}

static void FadeFootprintsTireTracks_Step0(struct Sprite * sprite)
{
    // Wait 40 frames before the flickering starts.
    if (++sprite->data[1] > 40)
        sprite->data[0] = 1;

    UpdateObjectEventSpriteVisibility(sprite, FALSE);
}

static void FadeFootprintsTireTracks_Step1(struct Sprite * sprite)
{
    sprite->invisible ^= TRUE;
    sprite->data[1]++;
    
    UpdateObjectEventSpriteVisibility(sprite, sprite->invisible);
    
    if (sprite->data[1] > 56)
        FieldEffectStop(sprite, sprite->data[7]);
}

////////////////////////////
// FLDEFF_JUMP_BIG_SPLASH //
////////////////////////////

static const struct SpriteFrameImage sPicTable_JumpBigSplash[] =
{
    overworld_frame(gFieldEffectObjectPic_JumpBigSplash, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_JumpBigSplash, 2, 2, 1),
    overworld_frame(gFieldEffectObjectPic_JumpBigSplash, 2, 2, 2),
    overworld_frame(gFieldEffectObjectPic_JumpBigSplash, 2, 2, 3),
};

static const union AnimCmd sAnim_JumpBigSplash[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(1, 8),
    ANIMCMD_FRAME(2, 8),
    ANIMCMD_FRAME(3, 8),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_JumpBigSplash[] =
{
    sAnim_JumpBigSplash,
};

static const struct SpriteTemplate sSpriteTemplate_JumpBigSplash =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_0,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_JumpBigSplash,
    .images = sPicTable_JumpBigSplash,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_JumpImpact,
};

static u32 FldEff_JumpBigSplash(void)
{
    u32 spriteId;

    SetSpritePosToOffsetMapCoords((s16 *)&gFieldEffectArguments[0], (s16 *)&gFieldEffectArguments[1], 8, 8);
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect0);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_JumpBigSplash, gFieldEffectArguments[0], gFieldEffectArguments[1], 0);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gFieldEffectArguments[3];
        sprite->data[0] = gFieldEffectArguments[2];
        sprite->data[1] = FLDEFF_JUMP_BIG_SPLASH;
    }
    return 0;
}

///////////////////
// FLDEFF_SPLASH //
///////////////////

static void SpriteCB_Splash(struct Sprite * sprite);

static const struct SpriteFrameImage sPicTable_Splash[] =
{
    overworld_frame(gFieldEffectObjectPic_Splash, 2, 1, 0),
    overworld_frame(gFieldEffectObjectPic_Splash, 2, 1, 1),
};

static const union AnimCmd sAnim_Splash_0[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Splash_1[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_FRAME(0, 6),
    ANIMCMD_FRAME(1, 6),
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(1, 8),
    ANIMCMD_FRAME(0, 6),
    ANIMCMD_FRAME(1, 6),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnimTable_Splash[] =
{
    sAnim_Splash_0,
    sAnim_Splash_1,
};

static const struct SpriteTemplate sSpriteTemplate_Splash =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_0,
    .oam = &gObjectEventBaseOam_16x8,
    .anims = sAnimTable_Splash,
    .images = sPicTable_Splash,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Splash,
};

static u32 FldEff_Splash(void)
{
    u32 spriteId;
    
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect0);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_Splash, 0, 0, 0);
    if (spriteId != MAX_SPRITES)
    {
        struct ObjectEvent * objectEvent = &gObjectEvents[GetObjectEventIdByLocalIdAndMap(gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2])];
        struct Sprite *sprite = &gSprites[spriteId];
        
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gSprites[objectEvent->spriteId].oam.priority;
        sprite->data[0] = gFieldEffectArguments[0];
        sprite->data[1] = gFieldEffectArguments[1];
        sprite->data[2] = gFieldEffectArguments[2];
        sprite->y2 = (GetObjectEventGraphicsInfo(objectEvent->graphicsId)->height >> 1) - 4;
        
        PlaySE(SE_PUDDLE);
    }
    return 0;
}

static void SpriteCB_Splash(struct Sprite * sprite)
{
    u8 objectEventId;

    if (sprite->animEnded || TryGetObjectEventIdByLocalIdAndMap(sprite->data[0], sprite->data[1], sprite->data[2], &objectEventId))
        FieldEffectStop(sprite, FLDEFF_SPLASH);
    else
    {
        sprite->x = gSprites[gObjectEvents[objectEventId].spriteId].x;
        sprite->y = gSprites[gObjectEvents[objectEventId].spriteId].y;
        UpdateObjectEventSpriteVisibility(sprite, FALSE);
    }
}

//////////////////////////////
// FLDEFF_JUMP_SMALL_SPLASH //
//////////////////////////////

static const struct SpriteFrameImage sPicTable_JumpSmallSplash[] =
{
    overworld_frame(gFieldEffectObjectPic_JumpSmallSplash, 2, 1, 0),
    overworld_frame(gFieldEffectObjectPic_JumpSmallSplash, 2, 1, 1),
    overworld_frame(gFieldEffectObjectPic_JumpSmallSplash, 2, 1, 2),
};

static const union AnimCmd sAnim_JumpSmallSplash[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_FRAME(2, 4),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_JumpSmallSplash[] =
{
    sAnim_JumpSmallSplash,
};

static const struct SpriteTemplate sSpriteTemplate_JumpSmallSplash =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_0,
    .oam = &gObjectEventBaseOam_16x8,
    .anims = sAnimTable_JumpSmallSplash,
    .images = sPicTable_JumpSmallSplash,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_JumpImpact,
};

static u32 FldEff_JumpSmallSplash(void)
{
    u32 spriteId;

    SetSpritePosToOffsetMapCoords((s16 *)&gFieldEffectArguments[0], (s16 *)&gFieldEffectArguments[1], 8, 12);
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect0);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_JumpSmallSplash, gFieldEffectArguments[0], gFieldEffectArguments[1], 0);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gFieldEffectArguments[3];
        sprite->data[0] = gFieldEffectArguments[2];
        sprite->data[1] = FLDEFF_JUMP_SMALL_SPLASH;
    }
    return 0;
}

///////////////////////
// FLDEFF_LONG_GRASS //
///////////////////////

static void SpriteCB_LongGrass(struct Sprite *sprite);

static const struct SpriteFrameImage sPicTable_LongGrass[] =
{
    overworld_frame(gFieldEffectObjectPic_LongGrass, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_LongGrass, 2, 2, 1),
    overworld_frame(gFieldEffectObjectPic_LongGrass, 2, 2, 2),
    overworld_frame(gFieldEffectObjectPic_LongGrass, 2, 2, 3),
};

static const union AnimCmd sAnim_LongGrass[] =
{
    ANIMCMD_FRAME(1, 3),
    ANIMCMD_FRAME(2, 3),
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(3, 4),
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(3, 4),
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_LongGrass[] =
{
    sAnim_LongGrass,
};

static const struct SpriteTemplate sSpriteTemplate_LongGrass =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_1,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_LongGrass,
    .images = sPicTable_LongGrass,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_LongGrass,
};

static u32 FldEff_LongGrass(void)
{
    s16 x = gFieldEffectArguments[0];
    s16 y = gFieldEffectArguments[1];
    u32 spriteId;
    
    SetSpritePosToOffsetMapCoords(&x, &y, 8, 8);
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect1);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_LongGrass, x, y, 0);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = ZCoordToPriority(gFieldEffectArguments[2]);
        sprite->data[0] = gFieldEffectArguments[2];
        sprite->data[1] = gFieldEffectArguments[0];
        sprite->data[2] = gFieldEffectArguments[1];
        sprite->data[3] = gFieldEffectArguments[4];
        sprite->data[4] = gFieldEffectArguments[5];
        sprite->data[5] = gFieldEffectArguments[6];
        
        if (gFieldEffectArguments[7]) // Is spawning
            SeekSpriteAnim(sprite, 6);
    }
    return 0;
}

static void SpriteCB_LongGrass(struct Sprite *sprite)
{
    u8 localId;
    u8 mapNum = sprite->data[5] >> 8;
    u8 mapGroup = sprite->data[5];
    u8 objectEventId;

    if (gCamera.active && (gSaveBlock1Ptr->location.mapNum != mapNum || gSaveBlock1Ptr->location.mapGroup != mapGroup))
    {
        sprite->data[1] -= gCamera.x;
        sprite->data[2] -= gCamera.y;
        sprite->data[5] = ((u8)gSaveBlock1Ptr->location.mapNum << 8) | (u8)gSaveBlock1Ptr->location.mapGroup;
    }
    localId = sprite->data[3] >> 8;
    mapNum = sprite->data[3];
    mapGroup = sprite->data[4];

    if (TryGetObjectEventIdByLocalIdAndMap(localId, mapNum, mapGroup, &objectEventId) || !MetatileBehavior_IsLongGrass(MapGridGetMetatileBehaviorAt(sprite->data[1], sprite->data[2]))
    || (sprite->data[7] && sprite->animEnded))
        FieldEffectStop(sprite, FLDEFF_LONG_GRASS);
    else
    {
        struct ObjectEvent *objectEvent = &gObjectEvents[objectEventId];
        
        if ((objectEvent->currentCoords.x != sprite->data[1] || objectEvent->currentCoords.y != sprite->data[2])
        && (objectEvent->previousCoords.x != sprite->data[1] || objectEvent->previousCoords.y != sprite->data[2]))
            sprite->data[7] = TRUE;

        UpdateObjectEventSpriteVisibility(sprite, FALSE);
        UpdateGrassFieldEffectSubpriority(sprite, sprite->data[0], 0);
    }
}

////////////////////////////
// FLDEFF_JUMP_LONG_GRASS //
////////////////////////////

static const struct SpriteFrameImage sPicTable_JumpLongGrass[] =
{
    overworld_frame(gFieldEffectObjectPic_JumpLongGrass, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_JumpLongGrass, 2, 2, 1),
    overworld_frame(gFieldEffectObjectPic_JumpLongGrass, 2, 2, 2),
    overworld_frame(gFieldEffectObjectPic_JumpLongGrass, 2, 2, 3),
    overworld_frame(gFieldEffectObjectPic_JumpLongGrass, 2, 2, 4),
    overworld_frame(gFieldEffectObjectPic_JumpLongGrass, 2, 2, 6),
};

static const union AnimCmd sAnim_JumpLongGrass[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_FRAME(2, 8),
    ANIMCMD_FRAME(3, 8),
    ANIMCMD_FRAME(4, 8),
    ANIMCMD_FRAME(5, 8),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_JumpLongGrass[] =
{
    sAnim_JumpLongGrass,
};

static const struct SpriteTemplate sSpriteTemplate_JumpLongGrass =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_1,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_JumpLongGrass,
    .images = sPicTable_JumpLongGrass,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_JumpImpact,
};

static u32 FldEff_JumpLongGrass(void)
{
    u32 spriteId;

    SetSpritePosToOffsetMapCoords((s16 *)&gFieldEffectArguments[0], (s16 *)&gFieldEffectArguments[1], 8, 8);
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect1);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_JumpLongGrass, gFieldEffectArguments[0], gFieldEffectArguments[1], 0);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gFieldEffectArguments[3];
        sprite->data[0] = gFieldEffectArguments[2];
        sprite->data[1] = FLDEFF_JUMP_LONG_GRASS;
    }
    return 0;
}

//////////////////////////
// FLDEFF_SHAKING_GRASS //
//////////////////////////

static const struct SpriteFrameImage sPicTable_ShakingGrass[] =
{
    overworld_frame(gFieldEffectObjectPic_ShakingGrass, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_ShakingGrass, 2, 2, 1),
    overworld_frame(gFieldEffectObjectPic_ShakingGrass, 2, 2, 2),
    overworld_frame(gFieldEffectObjectPic_ShakingGrass, 2, 2, 3),
    overworld_frame(gFieldEffectObjectPic_ShakingGrass, 2, 2, 4),
    overworld_frame(gFieldEffectObjectPic_ShakingGrass, 2, 2, 5),
    overworld_frame(gFieldEffectObjectPic_ShakingGrass, 2, 2, 6),
    overworld_frame(gFieldEffectObjectPic_ShakingGrass, 2, 2, 7),
    overworld_frame(gFieldEffectObjectPic_ShakingGrass, 2, 2, 8),
};

static const union AnimCmd sAnim_ShakingGrass[] =
{
    ANIMCMD_FRAME(0, 10),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_FRAME(2, 4),
    ANIMCMD_FRAME(3, 4),
    ANIMCMD_FRAME(4, 4),
    ANIMCMD_FRAME(5, 4),
    ANIMCMD_FRAME(6, 4),
    ANIMCMD_FRAME(7, 4),
    ANIMCMD_FRAME(8, 4),
    ANIMCMD_JUMP(7),
};

static const union AnimCmd *const sAnimTable_ShakingGrass[] =
{
    sAnim_ShakingGrass,
};

static const struct SpriteTemplate sSpriteTemplate_ShakingGrass =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_1,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_ShakingGrass,
    .images = sPicTable_ShakingGrass,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_WaitFieldEffectSpriteAnim,
};

static u32 FldEff_ShakingGrass(void)
{
    u32 spriteId;

    SetSpritePosToOffsetMapCoords((s16 *)&gFieldEffectArguments[0], (s16 *)&gFieldEffectArguments[1], 8, 8);
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect1);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_ShakingGrass, gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2]);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gFieldEffectArguments[3];
        sprite->data[0] = FLDEFF_SHAKING_GRASS;
    }
    return spriteId;
}

///////////////////////////////
// FLDEFF_SHAKING_LONG_GRASS //
///////////////////////////////

static const struct SpriteFrameImage sPicTable_ShakingLongGrass[] =
{
    overworld_frame(gFieldEffectObjectPic_ShakingLongGrass, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_ShakingLongGrass, 2, 2, 1),
    overworld_frame(gFieldEffectObjectPic_ShakingLongGrass, 2, 2, 2),
    overworld_frame(gFieldEffectObjectPic_ShakingLongGrass, 2, 2, 3),
};

static const union AnimCmd sAnim_ShakingLongGrass[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_FRAME(2, 4),
    ANIMCMD_FRAME(3, 4),
    ANIMCMD_FRAME(2, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnimTable_ShakingLongGrass[] =
{
    sAnim_ShakingLongGrass,
};

static const struct SpriteTemplate sSpriteTemplate_ShakingLongGrass =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_1,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_ShakingLongGrass,
    .images = sPicTable_ShakingLongGrass,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_WaitFieldEffectSpriteAnim,
};

static u32 FldEff_ShakingLongGrass(void)
{
    u32 spriteId;

    SetSpritePosToOffsetMapCoords((s16 *)&gFieldEffectArguments[0], (s16 *)&gFieldEffectArguments[1], 8, 8);
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect1);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_ShakingLongGrass, gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2]);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gFieldEffectArguments[3];
        sprite->data[0] = FLDEFF_SHAKING_LONG_GRASS;
    }
    return spriteId;
}

//////////////////////
// FLDEFF_SAND_HOLE //
//////////////////////

static const struct SpriteFrameImage sPicTable_SandHole[] =
{
    overworld_frame(gFieldEffectObjectPic_SandHole, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_SandHole, 2, 2, 1),
    overworld_frame(gFieldEffectObjectPic_SandHole, 2, 2, 2),
    overworld_frame(gFieldEffectObjectPic_SandHole, 2, 2, 3),
};

static const union AnimCmd sAnim_SandHole[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_FRAME(2, 4),
    ANIMCMD_FRAME(3, 4),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnimTable_SandHole[] =
{
    sAnim_SandHole,
};

static const struct SpriteTemplate sSpriteTemplate_SandHole =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_0,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_SandHole,
    .images = sPicTable_SandHole,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_WaitFieldEffectSpriteAnim,
};

static u32 FldEff_SandHole(void)
{
    u32 spriteId;

    SetSpritePosToOffsetMapCoords((s16 *)&gFieldEffectArguments[0], (s16 *)&gFieldEffectArguments[1], 8, 8);
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect0);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_SandHole, gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2]);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gFieldEffectArguments[3];
        sprite->data[0] = FLDEFF_SAND_HOLE;
    }
    return spriteId;
}

////////////////////////////
// FLDEFF_WATER_SURFACING //
////////////////////////////

static const struct SpriteFrameImage sPicTable_WaterSurfacing[] =
{
    overworld_frame(gFieldEffectObjectPic_WaterSurfacing, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_WaterSurfacing, 2, 2, 1),
    overworld_frame(gFieldEffectObjectPic_WaterSurfacing, 2, 2, 2),
    overworld_frame(gFieldEffectObjectPic_WaterSurfacing, 2, 2, 3),
};

static const union AnimCmd sAnim_WaterSurfacing[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_FRAME(2, 4),
    ANIMCMD_FRAME(3, 4),
    ANIMCMD_FRAME(2, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnimTable_WaterSurfacing[] =
{
    sAnim_WaterSurfacing,
};

static const struct SpriteTemplate sSpriteTemplate_WaterSurfacing =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_0,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_WaterSurfacing,
    .images = sPicTable_WaterSurfacing,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_WaitFieldEffectSpriteAnim,
};

static u32 FldEff_WaterSurfacing(void)
{
    u32 spriteId;

    SetSpritePosToOffsetMapCoords((s16 *)&gFieldEffectArguments[0], (s16 *)&gFieldEffectArguments[1], 8, 8);
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect0);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_WaterSurfacing, gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2]);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gFieldEffectArguments[3];
        sprite->data[0] = FLDEFF_WATER_SURFACING;
    }
    return spriteId;
}

//////////////////////////////////////
// FLDEFF_BERRY_TREE_GROWTH_SPARKLE //
//////////////////////////////////////

static const struct SpriteFrameImage sPicTable_Sparkle[] = {
    overworld_frame(gFieldEffectObjectPic_Sparkle, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_Sparkle, 2, 2, 1),
    overworld_frame(gFieldEffectObjectPic_Sparkle, 2, 2, 2),
    overworld_frame(gFieldEffectObjectPic_Sparkle, 2, 2, 3),
    overworld_frame(gFieldEffectObjectPic_Sparkle, 2, 2, 4),
    overworld_frame(gFieldEffectObjectPic_Sparkle, 2, 2, 5),
};

static const union AnimCmd sAnim_Sparkle[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(1, 8),
    ANIMCMD_FRAME(2, 8),
    ANIMCMD_FRAME(3, 8),
    ANIMCMD_FRAME(4, 8),
    ANIMCMD_FRAME(5, 8),
    ANIMCMD_LOOP(0),
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_FRAME(2, 4),
    ANIMCMD_FRAME(3, 4),
    ANIMCMD_FRAME(4, 4),
    ANIMCMD_FRAME(5, 4),
    ANIMCMD_LOOP(3),
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(1, 8),
    ANIMCMD_FRAME(2, 8),
    ANIMCMD_FRAME(3, 8),
    ANIMCMD_FRAME(4, 8),
    ANIMCMD_FRAME(5, 8),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_Sparkle[] =
{
    sAnim_Sparkle,
};

static const struct SpriteTemplate sSpriteTemplate_Sparkle = {
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = OBJ_EVENT_PAL_TAG_NPC_PINK,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_Sparkle,
    .images = sPicTable_Sparkle,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_WaitFieldEffectSpriteAnim,
};

static u32 FldEff_BerryTreeGrowthSparkle(void)
{
    u32 spriteId;

    SetSpritePosToOffsetMapCoords((s16 *)&gFieldEffectArguments[0], (s16 *)&gFieldEffectArguments[1], 8, 4);
    LoadFieldEffectPalette(&sSpriteTemplate_Sparkle);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_Sparkle, gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2]);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gFieldEffectArguments[3];
        sprite->data[0] = FLDEFF_BERRY_TREE_GROWTH_SPARKLE;
    }
    return spriteId;
}

/////////////////////////////////
// FLDEFF_DEEP_SAND_FOOTPRINTS //
/////////////////////////////////

static const struct SpriteFrameImage sPicTable_DeepSandFootprints[] =
{
    overworld_frame(gFieldEffectObjectPic_DeepSandFootprints, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_DeepSandFootprints, 2, 2, 1),
};

static const union AnimCmd sDeepSandFootprintsAnim_South[] =
{
    ANIMCMD_FRAME(0, 1, .vFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd sDeepSandFootprintsAnim_North[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

static const union AnimCmd sDeepSandFootprintsAnim_West[] =
{
    ANIMCMD_FRAME(1, 1),
    ANIMCMD_END,
};

static const union AnimCmd sDeepSandFootprintsAnim_East[] =
{
    ANIMCMD_FRAME(1, 1, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_DeepSandFootprints[] =
{
    [DIR_NONE]  = sDeepSandFootprintsAnim_South,
    [DIR_SOUTH] = sDeepSandFootprintsAnim_South,
    [DIR_NORTH] = sDeepSandFootprintsAnim_North,
    [DIR_WEST]  = sDeepSandFootprintsAnim_West,
    [DIR_EAST]  = sDeepSandFootprintsAnim_East,
};

static const struct SpriteTemplate sSpriteTemplate_DeepSandFootprints =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_0,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_DeepSandFootprints,
    .images = sPicTable_DeepSandFootprints,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_UpdateFootprintsTireTracks,
};

static u32 FldEff_DeepSandFootprints(void)
{
    u32 spriteId;

    SetSpritePosToOffsetMapCoords((s16 *)&gFieldEffectArguments[0], (s16 *)&gFieldEffectArguments[1], 8, 8);
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect0);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_DeepSandFootprints, gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2]);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gFieldEffectArguments[3];
        sprite->data[7] = FLDEFF_DEEP_SAND_FOOTPRINTS;
        StartSpriteAnim(sprite, gFieldEffectArguments[4]);
    }
    return 0;
}

////////////////////////////
// FLDEFF_POKECENTER_HEAL //
////////////////////////////

static void Task_PokecenterHeal(u32 taskId);
static void PokecenterHealEffect_0(struct Task * task);
static void PokecenterHealEffect_1(struct Task * task);
static void PokecenterHealEffect_2(struct Task * task);
static void PokecenterHealEffect_3(struct Task * task);
static u32 CreatePokeballGlowSprite(s16 duration, s16 x, s16 y, bool32 fanfare);
static void SpriteCB_PokeballGlowEffect(struct Sprite * sprite);
static void PokeballGlowEffect_0(struct Sprite * sprite);
static void PokeballGlowEffect_1(struct Sprite * sprite);
static void PokeballGlowEffect_2(struct Sprite * sprite);
static void PokeballGlowEffect_3(struct Sprite * sprite);
static void PokeballGlowEffect_4(struct Sprite * sprite);
static void PokeballGlowEffect_5(struct Sprite * sprite);
static void PokeballGlowEffect_6(struct Sprite * sprite);
static void PokeballGlowEffect_7(struct Sprite * sprite);
static void SpriteCB_PokecenterMonitor(struct Sprite * sprite);
static void SpriteCB_PokeballGlow(struct Sprite * sprite);

static void (*const sPokecenterHealTaskCBTable[])(struct Task * ) = {
    PokecenterHealEffect_0,
    PokecenterHealEffect_1,
    PokecenterHealEffect_2,
    PokecenterHealEffect_3
};

static void (*const sPokeballGlowSpriteCBTable[])(struct Sprite * ) = {
    PokeballGlowEffect_0,
    PokeballGlowEffect_1,
    PokeballGlowEffect_2,
    PokeballGlowEffect_3,
    PokeballGlowEffect_4,
    PokeballGlowEffect_5,
    PokeballGlowEffect_6,
    PokeballGlowEffect_7
};

static const struct OamData sOamData_83CBEAC = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x16),
    .tileNum = 0x000,
    .priority = 0,
    .paletteNum = 0x0,
    .affineParam = 0
};

static const union AnimCmd sUnknown_83CBF2C[] = {
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sUnknown_83CBF34[] = {
    ANIMCMD_FRAME(1, 5),
    ANIMCMD_FRAME(2, 5),
    ANIMCMD_FRAME(3, 7),
    ANIMCMD_FRAME(2, 5),
    ANIMCMD_FRAME(1, 5),
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_LOOP(3),
    ANIMCMD_END
};

static const union AnimCmd *const sUnknown_83CBF54[] = {
    sUnknown_83CBF2C,
    sUnknown_83CBF34
};

static const struct SpriteFrameImage sUnknown_83CBEBC[] = {
    {sUnknown_83CAFD0 + 0x000, 0x100},
    {sUnknown_83CAFD0 + 0x080, 0x100},
    {sUnknown_83CAFD0 + 0x100, 0x100},
    {sUnknown_83CAFD0 + 0x180, 0x100}
};

static const struct SpriteTemplate sUnknown_83CBFA0 = {
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_POKECENTER_MONITOR,
    .oam = &sOamData_83CBEAC,
    .anims = sUnknown_83CBF54,
    .images = sUnknown_83CBEBC,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_PokecenterMonitor
};

static const struct SpritePalette sUnknown_83CBE9C = {
    sPokeballPal, FLDEFF_PAL_TAG_POKECENTER_MONITOR
};

static const struct SpriteFrameImage sUnknown_83CBEB4[] = {
    {sPokeballGfx, 0x20}
};

static const struct OamData sOamData_8x8 = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0x000,
    .priority = 0,
    .paletteNum = 0x0,
    .affineParam = 0
};

static const struct SpriteTemplate sUnknown_83CBF88 = {
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_POKECENTER_MONITOR,
    .oam = &sOamData_8x8,
    .anims = sUnknown_83CBF54,
    .images = sUnknown_83CBEB4,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_PokeballGlow
};

static const struct Coords16 sUnknown_83CC010[] = {
    {0, 0},
    {6, 0},
    {0, 4},
    {6, 4},
    {0, 8},
    {6, 8}
};

static const u8 sUnknown_83CC028[] = {16, 12,  8,  0};
static const u8 sUnknown_83CC02C[] = {16, 12,  8,  0};
static const u8 sUnknown_83CC030[] = { 0,  0,  0,  0};

static u32 FldEff_PokecenterHeal(void)
{
    u32 nPokemon;
    struct Task * task;
    
    FieldEffect_LoadFadedPal(&sUnknown_83CBE9C);
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect0);

#if NO_HEAL_EGGS
    nPokemon = CountPartyNonEggMons();
#else
    nPokemon = CalculatePlayerPartyCount();
#endif

    task = &gTasks[CreateTask(Task_PokecenterHeal, 0xff)];
    task->data[1] = nPokemon;
    task->data[2] = 0x5d;
    task->data[3] = 0x24;
    task->data[4] = 0x80;
    task->data[5] = 0x18;
    
    return 0;
}

static void Task_PokecenterHeal(u32 taskId)
{
    sPokecenterHealTaskCBTable[gTasks[taskId].data[0]](&gTasks[taskId]);
}

static u32 PokecenterHealEffectHelper(s32 x, s32 y)
{
    u32 spriteId = CreateSpriteAtEnd(&sUnknown_83CBFA0, x, y, 0);
    struct Sprite * sprite = &gSprites[spriteId];

    sprite->oam.priority = 2;
    sprite->invisible = TRUE;
    return spriteId;
}

static void PokecenterHealEffect_0(struct Task * task)
{
    task->data[0]++;
    task->data[6] = CreatePokeballGlowSprite(task->data[1], task->data[2], task->data[3], TRUE);
    task->data[7] = PokecenterHealEffectHelper(task->data[4], task->data[5]);
}

static void PokecenterHealEffect_1(struct Task * task)
{
    if (gSprites[task->data[6]].data[0] > 1)
    {
        gSprites[task->data[7]].data[0]++;
        task->data[0]++;
    }
}

static void PokecenterHealEffect_2(struct Task * task)
{
    if (gSprites[task->data[6]].data[0] > 4)
        task->data[0]++;
}

static void PokecenterHealEffect_3(struct Task * task)
{
    if (gSprites[task->data[6]].data[0] > 6)
    {
        DestroySprite(&gSprites[task->data[6]]);
        FieldEffectActiveListRemove(FLDEFF_POKECENTER_HEAL);
        DestroyTask(FindTaskIdByFunc(Task_PokecenterHeal));
    }
}

static u32 CreatePokeballGlowSprite(s16 duration, s16 x, s16 y, bool32 fanfare)
{
    u32 spriteId = CreateInvisibleSprite(SpriteCB_PokeballGlowEffect);
    struct Sprite * sprite = &gSprites[spriteId];

    sprite->x2 = x;
    sprite->y2 = y;
    sprite->subpriority = 0xFF;
    sprite->data[5] = fanfare;
    sprite->data[6] = duration;
    sprite->data[7] = spriteId;
    return spriteId;
}

static void SpriteCB_PokeballGlowEffect(struct Sprite * sprite)
{
    sPokeballGlowSpriteCBTable[sprite->data[0]](sprite);
}

static void PokeballGlowEffect_0(struct Sprite * sprite)
{
    u32 endSpriteId;
    
    if (sprite->data[1] == 0 || (--sprite->data[1]) == 0)
    {
        sprite->data[1] = 25;
        
        endSpriteId = CreateSpriteAtEnd(&sUnknown_83CBF88, sUnknown_83CC010[sprite->data[2]].x + sprite->x2, sUnknown_83CC010[sprite->data[2]].y + sprite->y2, 0xFF);
        gSprites[endSpriteId].oam.priority = 2;
        gSprites[endSpriteId].data[0] = sprite->data[7];
        
        sprite->data[2]++;
        sprite->data[6]--;
        
        PlaySE(SE_BALL);
    }
    
    if (sprite->data[6] == 0)
    {
        sprite->data[1] = 32;
        sprite->data[0]++;
    }
}

static void PokeballGlowEffect_1(struct Sprite * sprite)
{
    if ((--sprite->data[1]) == 0)
    {
        sprite->data[0]++;
        sprite->data[1] = 8;
        sprite->data[2] = 0;
        sprite->data[3] = 0;
        
        if (sprite->data[5])
            PlayFanfare(MUS_HEAL);
    }
}

static void PokeballGlowEffect_2(struct Sprite * sprite)
{
    u8 phase;
    u32 palIndex;
    
    if ((--sprite->data[1]) == 0)
    {
        sprite->data[1] = 8;
        sprite->data[2]++;
        sprite->data[2] &= 3;
        if (sprite->data[2] == 0)
            sprite->data[3]++;
    }
    palIndex = IndexOfSpritePaletteTag(FLDEFF_PAL_TAG_POKECENTER_MONITOR);
    
    phase = (sprite->data[2] + 3) & 3;
    MultiplyInvertedPaletteRGBComponents((palIndex << 4) + 0x108, sUnknown_83CC028[phase], sUnknown_83CC02C[phase], sUnknown_83CC030[phase]);
    
    phase = (sprite->data[2] + 2) & 3;
    MultiplyInvertedPaletteRGBComponents((palIndex << 4) + 0x106, sUnknown_83CC028[phase], sUnknown_83CC02C[phase], sUnknown_83CC030[phase]);
    
    phase = (sprite->data[2] + 1) & 3;
    MultiplyInvertedPaletteRGBComponents((palIndex << 4) + 0x102, sUnknown_83CC028[phase], sUnknown_83CC02C[phase], sUnknown_83CC030[phase]);
    
    phase = sprite->data[2];
    MultiplyInvertedPaletteRGBComponents((palIndex << 4) + 0x105, sUnknown_83CC028[phase], sUnknown_83CC02C[phase], sUnknown_83CC030[phase]);
    MultiplyInvertedPaletteRGBComponents((palIndex << 4) + 0x103, sUnknown_83CC028[phase], sUnknown_83CC02C[phase], sUnknown_83CC030[phase]);
    
    if (sprite->data[3] > 2)
    {
        sprite->data[0]++;
        sprite->data[1] = 8;
        sprite->data[2] = 0;
    }
}

static void PokeballGlowEffect_3(struct Sprite * sprite)
{
    u8 phase;
    u32 palIndex;
    
    if ((--sprite->data[1]) == 0)
    {
        sprite->data[1] = 8;
        sprite->data[2]++;
        sprite->data[2] &= 3;
        if (sprite->data[2] == 3)
        {
            sprite->data[0]++;
            sprite->data[1] = 30;
        }
    }
    phase = sprite->data[2];
    palIndex = IndexOfSpritePaletteTag(FLDEFF_PAL_TAG_POKECENTER_MONITOR);
    
    MultiplyInvertedPaletteRGBComponents((palIndex << 4) + 0x108, sUnknown_83CC028[phase], sUnknown_83CC02C[phase], sUnknown_83CC030[phase]);
    MultiplyInvertedPaletteRGBComponents((palIndex << 4) + 0x106, sUnknown_83CC028[phase], sUnknown_83CC02C[phase], sUnknown_83CC030[phase]);
    MultiplyInvertedPaletteRGBComponents((palIndex << 4) + 0x102, sUnknown_83CC028[phase], sUnknown_83CC02C[phase], sUnknown_83CC030[phase]);
    MultiplyInvertedPaletteRGBComponents((palIndex << 4) + 0x105, sUnknown_83CC028[phase], sUnknown_83CC02C[phase], sUnknown_83CC030[phase]);
    MultiplyInvertedPaletteRGBComponents((palIndex << 4) + 0x103, sUnknown_83CC028[phase], sUnknown_83CC02C[phase], sUnknown_83CC030[phase]);
}

static void PokeballGlowEffect_4(struct Sprite * sprite)
{
    if ((--sprite->data[1]) == 0)
        sprite->data[0]++;
}

static void PokeballGlowEffect_5(struct Sprite * sprite)
{
    sprite->data[0]++;
}

static void PokeballGlowEffect_6(struct Sprite * sprite)
{
    if (sprite->data[5] == 0 || IsFanfareTaskInactive())
        sprite->data[0]++;
}

static void PokeballGlowEffect_7(struct Sprite * sprite)
{
}

static void SpriteCB_PokeballGlow(struct Sprite * sprite)
{
    if (gSprites[sprite->data[0]].data[0] > 4)
        FieldEffectFreeGraphicsResources(sprite);
}

static void SpriteCB_PokecenterMonitor(struct Sprite * sprite)
{
    if (sprite->data[0] != 0)
    {
        sprite->data[0] = 0;
        sprite->invisible = FALSE;
        StartSpriteAnim(sprite, 1);
    }
    
    if (sprite->animEnded)
        FieldEffectFreeGraphicsResources(sprite);
}

// r, g, b are between 0 and 16
void MultiplyInvertedPaletteRGBComponents(u16 i, u8 r, u8 g, u8 b)
{
    int curRed;
    int curGreen;
    int curBlue;
    u16 outPal;

    outPal = gPlttBufferUnfaded[i];
    curRed = outPal & 0x1f;
    curGreen = (outPal & (0x1f << 5)) >> 5;
    curBlue = (outPal & (0x1f << 10)) >> 10;
    curRed += (((0x1f - curRed) * r) >> 4);
    curGreen += (((0x1f - curGreen) * g) >> 4);
    curBlue += (((0x1f - curBlue) * b) >> 4);
    outPal = curRed;
    outPal |= curGreen << 5;
    outPal |= curBlue << 10;
    gPlttBufferFaded[i] = outPal;
}

/////////////////////////////
// FLDEFF_MOVE_DEOXYS_ROCK //
/////////////////////////////

static void Task_MoveDeoxysRock_Step(u32 taskId);

static u32 FldEff_MoveDeoxysRock(void)
{
    u32 taskId;
    u8 objectEventIdBuffer;
    s32 x;
    s32 y;

    if (!TryGetObjectEventIdByLocalIdAndMap(gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2], &objectEventIdBuffer))
    {
        struct ObjectEvent *objectEvent = &gObjectEvents[objectEventIdBuffer];
        
        x = objectEvent->currentCoords.x - 7;
        y = objectEvent->currentCoords.y - 7;
        x = (gFieldEffectArguments[3] - x) * 16;
        y = (gFieldEffectArguments[4] - y) * 16;
        
        ShiftObjectEventCoords(objectEvent, gFieldEffectArguments[3] + 7, gFieldEffectArguments[4] + 7);
        
        taskId = CreateTask(Task_MoveDeoxysRock_Step, 0x50);
        gTasks[taskId].data[1] = objectEvent->spriteId;
        gTasks[taskId].data[2] = gSprites[objectEvent->spriteId].x + x;
        gTasks[taskId].data[3] = gSprites[objectEvent->spriteId].y + y;
        gTasks[taskId].data[8] = gFieldEffectArguments[5];
        gTasks[taskId].data[9] = objectEventIdBuffer;
    }
    return 0;
}

static void Task_MoveDeoxysRock_Step(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    struct Sprite * sprite = &gSprites[data[1]];
    struct ObjectEvent * objectEvent;
    
    switch (data[0])
    {
    case 0:
        data[4] = sprite->x << 4;
        data[5] = sprite->y << 4;

        // UB: Possible divide by zero
        data[6] = SAFE_DIV(((data[2] << 4) - data[4]), data[8]);
        data[7] = SAFE_DIV(((data[3] << 4) - data[5]), data[8]);
        data[0]++;
        // fallthrough
    case 1:
        if (data[8] != 0)
        {
            data[8]--;
            data[4] += data[6];
            data[5] += data[7];
            sprite->x = data[4] >> 4;
            sprite->y = data[5] >> 4;
        }
        else
        {
            objectEvent = &gObjectEvents[data[9]];
            
            sprite->x = data[2];
            sprite->y = data[3];
            
            ShiftStillObjectEventCoords(objectEvent);
            objectEvent->triggerGroundEffectsOnStop = TRUE;
            FieldEffectActiveListRemove(FLDEFF_MOVE_DEOXYS_ROCK);
            DestroyTask(taskId);
        }
        break;
    }
}

////////////////////////////////
// FLDEFF_DESTROY_DEOXYS_ROCK //
////////////////////////////////

static void Task_DestroyDeoxysRock(u32 taskId);
static void DestroyDeoxysRockEffect_CameraShake(s16 *data, u32 taskId);
static void DestroyDeoxysRockEffect_RockFragments(s16 *data, u32 taskId);
static void DestroyDeoxysRockEffect_WaitAndEnd(s16 *data, u32 taskId);
static void SpriteCB_DeoxysRockFragment(struct Sprite *sprite);

static void (*const sDestroyDeoxysRocksEffectFuncs[])(s16 *data, u32 taskId) = {
    DestroyDeoxysRockEffect_CameraShake,
    DestroyDeoxysRockEffect_RockFragments,
    DestroyDeoxysRockEffect_WaitAndEnd
};

static const struct SpriteFrameImage sImages_FldEffUnk44[] = {
    {sFldEffUnk44_Tiles + 0x00, 0x20},
    {sFldEffUnk44_Tiles + 0x10, 0x20},
    {sFldEffUnk44_Tiles + 0x20, 0x20},
    {sFldEffUnk44_Tiles + 0x30, 0x20}
};

static const union AnimCmd sAnimCmd_FldEffUnk44_0[] = {
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd sAnimCmd_FldEffUnk44_1[] = {
    ANIMCMD_FRAME(1, 0),
    ANIMCMD_END
};

static const union AnimCmd sAnimCmd_FldEffUnk44_2[] = {
    ANIMCMD_FRAME(2, 0),
    ANIMCMD_END
};

static const union AnimCmd sAnimCmd_FldEffUnk44_3[] = {
    ANIMCMD_FRAME(3, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sAnimCmdTable_FldEffUnk44[] = {
    sAnimCmd_FldEffUnk44_0,
    sAnimCmd_FldEffUnk44_1,
    sAnimCmd_FldEffUnk44_2,
    sAnimCmd_FldEffUnk44_3
};

static const struct SpriteTemplate sUnknown_83CC2A0 = {
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = OBJ_EVENT_PAL_TAG_METEORITE,
    .oam = &sOamData_8x8,
    .anims = sAnimCmdTable_FldEffUnk44,
    .images = sImages_FldEffUnk44,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_DeoxysRockFragment
};

static u32 FldEff_DestroyDeoxysRock(void)
{
    u32 taskId;
    u8 objectEventIdBuffer;
    
    if (!TryGetObjectEventIdByLocalIdAndMap(gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2], &objectEventIdBuffer))
    {
        taskId = CreateTask(Task_DestroyDeoxysRock, 0x50);
        gTasks[taskId].data[2] = objectEventIdBuffer;
        gTasks[taskId].data[6] = gFieldEffectArguments[0];
        gTasks[taskId].data[7] = gFieldEffectArguments[1];
        gTasks[taskId].data[8] = gFieldEffectArguments[2];
    }
    else
        FieldEffectActiveListRemove(FLDEFF_DESTROY_DEOXYS_ROCK);

    return FALSE;
}

static void Task_DestroyDeoxysRock(u32 taskId)
{
    InstallCameraPanAheadCallback();
    SetCameraPanningCallback(NULL);
    sDestroyDeoxysRocksEffectFuncs[gTasks[taskId].data[1]](gTasks[taskId].data, taskId);
}

static void Task_DeoxysRockCameraShake(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    
    if (data[7] != 0)
    {
        if (++data[6] > 20)
        {
            data[6] = 0;
            
            if (data[5] != 0)
                data[5]--;
        }
    }
    else
        data[5] = 4;

    if (++data[0] > 1)
    {
        data[0] = 0;
        SetCameraPanning(0, (++data[1] & 1) ? -data[5] : data[5]);
    }
    UpdateCameraPanning();
    
    if (data[5] == 0)
        DestroyTask(taskId);
}

static void DestroyDeoxysRockEffect_CameraShake(s16 *data, u32 taskId)
{
    PlaySE(SE_THUNDER2);
    data[5] = CreateTask(Task_DeoxysRockCameraShake, 90);
    data[1]++;
}

static void CreateDeoxysRockFragments(struct Sprite* sprite)
{
    u32 i;
    int xPos = (s16)gTotalCameraPixelOffsetX + sprite->x + sprite->x2;
    int yPos = (s16)gTotalCameraPixelOffsetY + sprite->y + sprite->y2 - 4;

    for (i = 0; i < 4; i++)
    {
        u32 spriteId = CreateSprite(&sUnknown_83CC2A0, xPos, yPos, 0);
        
        if (spriteId != MAX_SPRITES)
        {
            StartSpriteAnim(&gSprites[spriteId], i);
            gSprites[spriteId].data[0] = i;
            gSprites[spriteId].oam.paletteNum = sprite->oam.paletteNum;
        }
    }
}

static void DestroyDeoxysRockEffect_RockFragments(s16 *data, u32 taskId)
{
    if (++data[3] > 0x78)
    {
        gObjectEvents[data[2]].invisible = TRUE;
        
        BlendPalettes(PALETTES_BG, 0x10, RGB_WHITE);
        BeginNormalPaletteFade(PALETTES_BG, 0, 0x10, 0, RGB_WHITE);
        
        CreateDeoxysRockFragments(&gSprites[gObjectEvents[data[2]].spriteId]);
        
        PlaySE(SE_THUNDER);
        
        gTasks[data[5]].data[7] = 1;
        data[3] = 0;
        data[1]++;
    }
}

static void DestroyDeoxysRockEffect_WaitAndEnd(s16 *data, u32 taskId)
{
    if (!gPaletteFade.active && !FuncIsActiveTask(Task_DeoxysRockCameraShake))
    {
        InstallCameraPanAheadCallback();
        RemoveObjectEventByLocalIdAndMap(data[6], data[7], data[8]);
        FieldEffectActiveListRemove(FLDEFF_DESTROY_DEOXYS_ROCK);
        DestroyTask(taskId);
    }
}

static void SpriteCB_DeoxysRockFragment(struct Sprite *sprite)
{
    switch (sprite->data[0])
    {
    case 0:
        sprite->x -= 16;
        sprite->y -= 12;
        break;
    case 1:
        sprite->x += 16;
        sprite->y -= 12;
        break;
    case 2:
        sprite->x -= 16;
        sprite->y += 12;
        break;
    case 3:
        sprite->x += 16;
        sprite->y += 12;
        break;
    }
    
    if (sprite->x < -4 || sprite->x > DISPLAY_WIDTH + 4 || sprite->y < -4 || sprite->y > DISPLAY_HEIGHT + 4)
        DestroySprite(sprite);
}

//////////////////////////
// FLDEFF_TREE_DISGUISE //
//////////////////////////

static void SpriteCB_Disguise(struct Sprite *sprite);

static const struct SpriteFrameImage sPicTable_TreeDisguise[] =
{
    overworld_frame(gFieldEffectObjectPic_TreeDisguise, 2, 4, 0),
    overworld_frame(gFieldEffectObjectPic_TreeDisguise, 2, 4, 1),
    overworld_frame(gFieldEffectObjectPic_TreeDisguise, 2, 4, 2),
    overworld_frame(gFieldEffectObjectPic_TreeDisguise, 2, 4, 3),
    overworld_frame(gFieldEffectObjectPic_TreeDisguise, 2, 4, 4),
    overworld_frame(gFieldEffectObjectPic_TreeDisguise, 2, 4, 5),
    overworld_frame(gFieldEffectObjectPic_TreeDisguise, 2, 4, 6),
};

static const union AnimCmd sAnim_TreeDisguise[] =
{
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_TreeDisguiseReveal[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_FRAME(2, 4),
    ANIMCMD_FRAME(3, 4),
    ANIMCMD_FRAME(4, 4),
    ANIMCMD_FRAME(5, 4),
    ANIMCMD_FRAME(6, 4),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_TreeDisguise[] =
{
    sAnim_TreeDisguise,
    sAnim_TreeDisguiseReveal,
};

static const struct SpriteTemplate sSpriteTemplate_TreeDisguise =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = OBJ_EVENT_PAL_TAG_NPC_GREEN,
    .oam = &gObjectEventBaseOam_16x32,
    .anims = sAnimTable_TreeDisguise,
    .images = sPicTable_TreeDisguise,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Disguise,
};

static u32 ShowDisguiseFieldEffect(u32 fldEff, const struct SpriteTemplate *spriteTemplate)
{
    u32 spriteId;
    u8 objEventId;
    struct Sprite * sprite;

    if (TryGetObjectEventIdByLocalIdAndMap(gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2], &objEventId))
    {
        FieldEffectActiveListRemove(fldEff);
        return MAX_SPRITES;
    }
    LoadFieldEffectPalette(spriteTemplate);
    
    spriteId = CreateSpriteAtEnd(spriteTemplate, 0, 0, 0);
    if (spriteId != MAX_SPRITES)
    {
        sprite = &gSprites[spriteId];
        sprite->coordOffsetEnabled = TRUE;
        sprite->data[1] = fldEff;
        sprite->data[2] = gFieldEffectArguments[0];
        sprite->data[3] = gFieldEffectArguments[1];
        sprite->data[4] = gFieldEffectArguments[2];
    }
    return spriteId;
}

static u32 FldEff_TreeDisguise(void)
{
    return ShowDisguiseFieldEffect(FLDEFF_TREE_DISGUISE, &sSpriteTemplate_TreeDisguise);
}

static void SpriteCB_Disguise(struct Sprite *sprite)
{
    u8 objectEventId;
    struct Sprite * linkedSprite;

    if (TryGetObjectEventIdByLocalIdAndMap(sprite->data[2], sprite->data[3], sprite->data[4], &objectEventId))
        FieldEffectStop(sprite, sprite->data[1]);

    linkedSprite = &gSprites[gObjectEvents[objectEventId].spriteId];
    sprite->invisible = linkedSprite->invisible;
    sprite->x = linkedSprite->x;
    sprite->y = (GetObjectEventGraphicsInfo(gObjectEvents[objectEventId].graphicsId)->height >> 1) + linkedSprite->y - 16;
    sprite->subpriority = linkedSprite->subpriority - 1;
    
    if (sprite->data[0] == 1)
    {
        sprite->data[0]++;
        StartSpriteAnim(sprite, 1);
    }
    
    if (sprite->data[0] == 2 && sprite->animEnded)
        sprite->data[7] = 1;

    if (sprite->data[0] == 3)
        FieldEffectStop(sprite, sprite->data[1]);
}

void StartRevealDisguise(struct ObjectEvent * objectEvent)
{
    if (objectEvent->directionSequenceIndex == 1)
        gSprites[objectEvent->fieldEffectSpriteId].data[0]++;
}

bool32 UpdateRevealDisguise(struct ObjectEvent * objectEvent)
{
    struct Sprite * sprite;

    if (objectEvent->directionSequenceIndex == 2 || objectEvent->directionSequenceIndex == 0)
        return TRUE;

    sprite = &gSprites[objectEvent->fieldEffectSpriteId];
    
    if (sprite->data[7])
    {
        objectEvent->directionSequenceIndex = 2;
        sprite->data[0]++;
        return TRUE;
    }
    return FALSE;
}

//////////////////////////////
// FLDEFF_MOUNTAIN_DISGUISE //
//////////////////////////////

static const struct SpriteFrameImage sPicTable_MountainDisguise[] =
{
    overworld_frame(gFieldEffectObjectPic_MountainDisguise, 2, 4, 0),
    overworld_frame(gFieldEffectObjectPic_MountainDisguise, 2, 4, 1),
    overworld_frame(gFieldEffectObjectPic_MountainDisguise, 2, 4, 2),
    overworld_frame(gFieldEffectObjectPic_MountainDisguise, 2, 4, 3),
    overworld_frame(gFieldEffectObjectPic_MountainDisguise, 2, 4, 4),
    overworld_frame(gFieldEffectObjectPic_MountainDisguise, 2, 4, 5),
    overworld_frame(gFieldEffectObjectPic_MountainDisguise, 2, 4, 6),
};

static const union AnimCmd sAnim_MountainDisguise[] =
{
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_MountainDisguiseReveal[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_FRAME(2, 4),
    ANIMCMD_FRAME(3, 4),
    ANIMCMD_FRAME(4, 4),
    ANIMCMD_FRAME(5, 4),
    ANIMCMD_FRAME(6, 4),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_MountainDisguise[] =
{
    sAnim_MountainDisguise,
    sAnim_MountainDisguiseReveal,
};

static const struct SpriteTemplate sSpriteTemplate_MountainDisguise =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = OBJ_EVENT_PAL_TAG_NPC_PINK,
    .oam = &gObjectEventBaseOam_16x32,
    .anims = sAnimTable_MountainDisguise,
    .images = sPicTable_MountainDisguise,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Disguise,
};

static u32 FldEff_MountainDisguise(void)
{
    return ShowDisguiseFieldEffect(FLDEFF_MOUNTAIN_DISGUISE, &sSpriteTemplate_MountainDisguise);
}

///////////////////////
// FLDEFF_NPCFLY_OUT //
///////////////////////

static void SpriteCB_NPCFlyOut(struct Sprite * sprite);

static const struct SpriteFrameImage sPicTable_Bird[] =
{
    overworld_frame(gFieldEffectObjectPic_Bird, 8, 8, 0),
    overworld_frame(gFieldEffectObjectPic_Bird, 8, 8, 1),
    overworld_frame(gFieldEffectObjectPic_Bird, 8, 8, 2),
    overworld_frame(gFieldEffectObjectPic_Bird, 8, 8, 3),
    overworld_frame(gFieldEffectObjectPic_Bird, 8, 8, 4),
};

static const union AnimCmd sAnim_Bird_0[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Bird_1[] =
{
    ANIMCMD_FRAME(1, 1),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Bird_2[] =
{
    ANIMCMD_FRAME(2, 1),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Bird_3[] =
{
    ANIMCMD_FRAME(3, 1),
    ANIMCMD_END,
};

static const union AnimCmd sAnim_Bird_4[] =
{
    ANIMCMD_FRAME(4, 1),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_Bird[] =
{
    sAnim_Bird_0,
    sAnim_Bird_1,
    sAnim_Bird_2,
    sAnim_Bird_3,
    sAnim_Bird_4,
};

static const struct SpriteTemplate sSpriteTemplate_Bird =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = OBJ_EVENT_PAL_TAG_PLAYER_RED,
    .oam = &gObjectEventBaseOam_64x64,
    .anims = sAnimTable_Bird,
    .images = sPicTable_Bird,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static u32 FldEff_NpcFlyOut(void)
{
    u32 spriteId;

    LoadFieldEffectPalette(&sSpriteTemplate_Bird);
    
    spriteId = CreateSprite(&sSpriteTemplate_Bird, 0x78, 0, 1);

    gSprites[spriteId].oam.priority = 1;
    gSprites[spriteId].callback = SpriteCB_NPCFlyOut;
    gSprites[spriteId].data[1] = gFieldEffectArguments[0];
    
    PlaySE(SE_M_FLY);
    
    return spriteId;
}

static void SpriteCB_NPCFlyOut(struct Sprite * sprite)
{
    struct Sprite * npcSprite;

    sprite->x2 = Cos(sprite->data[2], 0x8c);
    sprite->y2 = Sin(sprite->data[2], 0x48);
    sprite->data[2] = (sprite->data[2] + 4) & 0xff;
    
    if (sprite->data[0])
    {
        npcSprite = &gSprites[sprite->data[1]];
        npcSprite->coordOffsetEnabled = FALSE;
        npcSprite->x = sprite->x + sprite->x2;
        npcSprite->y = sprite->y + sprite->y2 - 8;
        npcSprite->x2 = 0;
        npcSprite->y2 = 0;
    }
    
    if (sprite->data[2] >= 0x80)
        FieldEffectStop(sprite, FLDEFF_NPCFLY_OUT);
}

////////////////////
// FLDEFF_USE_FLY //
////////////////////

static void Task_UseFly(u32 taskId);
static void UseFlyEffect_1(struct Task * task);
static void UseFlyEffect_2(struct Task * task);
static void UseFlyEffect_3(struct Task * task);
static void UseFlyEffect_4(struct Task * task);
static void UseFlyEffect_5(struct Task * task);
static void UseFlyEffect_6(struct Task * task);
static void UseFlyEffect_7(struct Task * task);
static void UseFlyEffect_8(struct Task * task);
static void UseFlyEffect_9(struct Task * task);
static u32 CreateFlyBirdSprite(void);
static void SetFlyBirdPlayerSpriteId(u32 flyBlobSpriteId, u32 playerSpriteId);
static void SpriteCB_FlyBirdLeaveBall(struct Sprite * sprite);
static void StartFlyBirdSwoopDown(u32 flyBlobSpriteId);
static void SpriteCB_FlyBirdSwoopDown(struct Sprite * sprite);
static void sub_80877FC(struct Sprite * sprite, u32 affineAnimId);
static void sub_8087828(struct Sprite * sprite);

static void (*const sUseFlyEffectFuncs[])(struct Task * ) = {
    UseFlyEffect_1,
    UseFlyEffect_2,
    UseFlyEffect_3,
    UseFlyEffect_4,
    UseFlyEffect_5,
    UseFlyEffect_6,
    UseFlyEffect_7,
    UseFlyEffect_8,
    UseFlyEffect_9
};

static const union AffineAnimCmd sUnknown_83CC19C[] = {
    AFFINEANIMCMD_FRAME( 8,  8, 226,  0),
    AFFINEANIMCMD_FRAME(28, 28,   0, 30),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd sUnknown_83CC1B4[] = {
    AFFINEANIMCMD_FRAME(256, 256, 64,  0),
    AFFINEANIMCMD_FRAME(-10, -10,  0, 22),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const sUnknown_83CC1CC[] = {
    sUnknown_83CC19C,
    sUnknown_83CC1B4
};

static const union AffineAnimCmd sUnknown_83CC214[] = {
    AFFINEANIMCMD_FRAME(24, 24, 0, 1),
    AFFINEANIMCMD_JUMP(0)
};

static const union AffineAnimCmd sUnknown_83CC224[] = {
    AFFINEANIMCMD_FRAME(512, 512, 0, 1),
    AFFINEANIMCMD_FRAME(-16, -16, 0, 1),
    AFFINEANIMCMD_JUMP(1)
};

static const union AffineAnimCmd *const sUnknown_83CC23C[] = {
    sUnknown_83CC214,
    sUnknown_83CC224
};

static u32 FldEff_UseFly(void)
{
    gTasks[CreateTask(Task_UseFly, 0xFE)].data[1] = gFieldEffectArguments[0];
    return 0;
}

static void Task_UseFly(u32 taskId)
{
    sUseFlyEffectFuncs[gTasks[taskId].data[0]](&gTasks[taskId]);
}

static void UseFlyEffect_1(struct Task * task)
{
    struct ObjectEvent * objectEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
    
    if (!ObjectEventIsMovementOverridden(objectEvent) || ObjectEventClearHeldMovementIfFinished(objectEvent))
    {
        task->data[15] = gPlayerAvatar.flags;
        gPlayerAvatar.preventStep = TRUE;
        SetPlayerAvatarStateMask(PLAYER_AVATAR_FLAG_ON_FOOT);
        StartPlayerAvatarSummonMonForFieldMoveAnim();
        ObjectEventSetHeldMovement(objectEvent, MOVEMENT_ACTION_START_ANIM_IN_DIRECTION);
        task->data[0]++;
    }
}

static void UseFlyEffect_2(struct Task * task)
{
    if (ObjectEventClearHeldMovementIfFinished(&gObjectEvents[gPlayerAvatar.objectEventId]))
    {
        task->data[0]++;
        gFieldEffectArguments[0] = task->data[1];
        FieldEffectStart(FLDEFF_FIELD_MOVE_SHOW_MON_INIT);
    }
}

static void UseFlyEffect_3(struct Task * task)
{
    if (!FieldEffectActiveListContains(FLDEFF_FIELD_MOVE_SHOW_MON))
    {
        struct ObjectEvent * objectEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
        
        if (task->data[15] & 0x08)
        {
            SetSurfBlob_BobState(objectEvent->fieldEffectSpriteId, 2);
            SetSurfBlob_DontSyncAnim(objectEvent->fieldEffectSpriteId, 0);
        }
        task->data[1] = CreateFlyBirdSprite();
        task->data[0]++;
    }
}

static void UseFlyEffect_4(struct Task * task)
{
    if (gSprites[task->data[1]].data[7])
    {
        task->data[0]++;
        task->data[2] = 16;
        SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_ON_FOOT);
        ObjectEventSetHeldMovement(&gObjectEvents[gPlayerAvatar.objectEventId], MOVEMENT_ACTION_FACE_LEFT);
    }
}

static void UseFlyEffect_5(struct Task * task)
{
    if ((task->data[2] == 0 || (--task->data[2]) == 0) && ObjectEventClearHeldMovementIfFinished(&gObjectEvents[gPlayerAvatar.objectEventId]))
    {
        task->data[0]++;
        PlaySE(SE_M_FLY);
        StartFlyBirdSwoopDown(task->data[1]);
    }
}

static void UseFlyEffect_6(struct Task * task)
{
    if ((++task->data[2]) >= 8)
    {
        struct ObjectEvent * objectEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
        
        ObjectEventSetGraphicsId(objectEvent, GetPlayerAvatarGraphicsIdByStateId(PLAYER_AVATAR_GFX_RIDE));
        StartSpriteAnim(&gSprites[objectEvent->spriteId], ANIM_GET_ON_OFF_POKEMON_WEST);
        objectEvent->inanimate = TRUE;
        ObjectEventSetHeldMovement(objectEvent, MOVEMENT_ACTION_JUMP_IN_PLACE_LEFT);
        task->data[0]++;
        task->data[2] = 0;
    }
}

static void UseFlyEffect_7(struct Task * task)
{
    if ((++task->data[2]) >= 10)
    {
        struct ObjectEvent * objectEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
        
        ObjectEventClearHeldMovementIfActive(objectEvent);
        
        objectEvent->inanimate = FALSE;
        objectEvent->noShadow = TRUE;
        
        SetFlyBirdPlayerSpriteId(task->data[1], objectEvent->spriteId);
        StartSpriteAnim(&gSprites[task->data[1]], gSaveBlock2Ptr->playerGender * 2 + 1);
        sub_80877FC(&gSprites[task->data[1]], 0);
        gSprites[task->data[1]].callback = sub_8087828;
        
        CameraObjectReset2();
        
        task->data[0]++;
    }
}

static void UseFlyEffect_8(struct Task * task)
{
    if (gSprites[task->data[1]].data[7])
    {
        WarpFadeOutScreen();
        task->data[0]++;
    }
}

static void UseFlyEffect_9(struct Task * task)
{
    if (!gPaletteFade.active)
    {
        FieldEffectActiveListRemove(FLDEFF_USE_FLY);
        DestroyTask(FindTaskIdByFunc(Task_UseFly));
    }
}

static u32 CreateFlyBirdSprite(void)
{
    u32 spriteId;

    LoadFieldEffectPalette(&sSpriteTemplate_Bird);
    
    spriteId = CreateSprite(&sSpriteTemplate_Bird, 0xff, 0xb4, 0x1);
    
    gSprites[spriteId].oam.priority = 1;
    gSprites[spriteId].callback = SpriteCB_FlyBirdLeaveBall;
    
    return spriteId;
}

static void SetFlyBirdPlayerSpriteId(u32 flyBlobSpriteId, u32 playerSpriteId)
{
    gSprites[flyBlobSpriteId].data[6] = playerSpriteId;
}

static void SpriteCB_FlyBirdLeaveBall(struct Sprite * sprite)
{
    if (sprite->data[7] == 0)
    {
        if (sprite->data[0] == 0)
        {
            sprite->oam.affineMode = ST_OAM_AFFINE_DOUBLE;
            sprite->affineAnims = sUnknown_83CC1CC;
            
            InitSpriteAffineAnim(sprite);
            StartSpriteAffineAnim(sprite, 0);
            
            sprite->x = gSaveBlock2Ptr->playerGender == MALE ? 0x80 : 0x76;
            sprite->y = -0x30;
            
            sprite->data[0]++;
            sprite->data[1] = 0x40;
            sprite->data[2] = 0x100;
        }
        sprite->data[1] += (sprite->data[2] >> 8);
        sprite->x2 = Cos(sprite->data[1], 0x78);
        sprite->y2 = Sin(sprite->data[1], 0x78);
        
        if (sprite->data[2] < 0x800)
            sprite->data[2] += 0x60;

        if (sprite->data[1] > 0x81)
        {
            sprite->data[7]++;
            FreeSpriteOamMatrix(sprite);
            CalcCenterToCornerVec(sprite, sprite->oam.shape, sprite->oam.size, ST_OAM_AFFINE_OFF);
        }
    }
}

static void StartFlyBirdSwoopDown(u32 spriteId)
{
    struct Sprite * sprite = &gSprites[spriteId];

    sprite->callback = SpriteCB_FlyBirdSwoopDown;
    sprite->x = 0x78;
    sprite->y = 0x00;
    sprite->x2 = 0;
    sprite->y2 = 0;
    memset(&sprite->data[0], 0, 8 * sizeof(u16) /* zero all data cells */);
    sprite->data[6] = 0x40;
}

static void SpriteCB_FlyBirdSwoopDown(struct Sprite * sprite)
{
    sprite->x2 = Cos(sprite->data[2], 0x8c);
    sprite->y2 = Sin(sprite->data[2], 0x48);
    sprite->data[2] = (sprite->data[2] + 4) & 0xff;
    
    if (sprite->data[6] != MAX_SPRITES)
    {
        struct Sprite * sprite1 = &gSprites[sprite->data[6]];
        sprite1->coordOffsetEnabled = FALSE;
        sprite1->x = sprite->x + sprite->x2;
        sprite1->y = sprite->y + sprite->y2 - 8;
        sprite1->x2 = 0;
        sprite1->y2 = 0;
    }
    
    if (sprite->data[2] >= 0x80)
        sprite->data[7] = 1;
}

static void sub_80877FC(struct Sprite * sprite, u32 affineAnimId)
{
    sprite->oam.affineMode = ST_OAM_AFFINE_DOUBLE;
    sprite->affineAnims = sUnknown_83CC23C;
    InitSpriteAffineAnim(sprite);
    StartSpriteAffineAnim(sprite, affineAnimId);
}

static void sub_8087828(struct Sprite * sprite)
{
    struct Sprite * sprite2;
    
    sprite->x2 = Cos(sprite->data[2], 0xB4);
    sprite->y2 = Sin(sprite->data[2], 0x48);
    
    sprite->data[2] += 2;
    sprite->data[2] &= 0xFF;
    
    if (sprite->data[6] != MAX_SPRITES)
    {
        sprite2 = &gSprites[sprite->data[6]];
        sprite2->coordOffsetEnabled = FALSE;
        sprite2->x = sprite->x + sprite->x2;
        sprite2->y = sprite->y + sprite->y2 - 8;
        sprite2->x2 = 0;
        sprite2->y2 = 0;
    }
    
    if (sprite->data[2] >= 0x80)
    {
        sprite->data[7] = 1;
        FreeSpriteOamMatrix(sprite);
        CalcCenterToCornerVec(sprite, sprite->oam.shape, sprite->oam.size, ST_OAM_AFFINE_OFF);
    }
}

///////////////////
// FLDEFF_FLY_IN //
///////////////////

static void Task_FldEffFlyIn(u32 taskId);
static void FlyInEffect_1(struct Task * task);
static void FlyInEffect_2(struct Task * task);
static void FlyInEffect_3(struct Task * task);
static void FlyInEffect_4(struct Task * task);
static void FlyInEffect_5(struct Task * task);
static void FlyInEffect_6(struct Task * task);
static void FlyInEffect_7(struct Task * task);
static void StartFlyBirdReturnToBall(u32 spriteId);
static void Task_FlyOut(u32 taskId);
static void Task_FlyIn(u32 taskId);

static void (*const sFlyInEffectFuncs[])(struct Task * task) = {
    FlyInEffect_1,
    FlyInEffect_2,
    FlyInEffect_3,
    FlyInEffect_4,
    FlyInEffect_5,
    FlyInEffect_6,
    FlyInEffect_7
};

static u32 FldEff_FlyIn(void)
{
    CreateTask(Task_FldEffFlyIn, 0xfe);
    return 0;
}

static void Task_FldEffFlyIn(u32 taskId)
{
    sFlyInEffectFuncs[gTasks[taskId].data[0]](&gTasks[taskId]);
}

static void FlyInEffect_1(struct Task * task)
{
    struct ObjectEvent * objectEvent = &gObjectEvents[gPlayerAvatar.objectEventId];

    if (!ObjectEventIsMovementOverridden(objectEvent) || ObjectEventClearHeldMovementIfFinished(objectEvent))
    {
        task->data[0]++;
        task->data[2] = 33;
        task->data[15] = gPlayerAvatar.flags;
        
        gPlayerAvatar.preventStep = TRUE;
        SetPlayerAvatarStateMask(PLAYER_AVATAR_FLAG_ON_FOOT);
        
        if (task->data[15] & PLAYER_AVATAR_FLAG_SURFING)
            SetSurfBlob_BobState(objectEvent->fieldEffectSpriteId, 0);

        ObjectEventSetGraphicsId(objectEvent, GetPlayerAvatarGraphicsIdByStateId(PLAYER_AVATAR_GFX_RIDE));
        CameraObjectReset2();
        ObjectEventTurn(objectEvent, DIR_WEST);
        StartSpriteAnim(&gSprites[objectEvent->spriteId], ANIM_GET_ON_OFF_POKEMON_WEST);
        
        objectEvent->invisible = FALSE;
        objectEvent->noShadow = TRUE;
        
        task->data[1] = CreateFlyBirdSprite();
        StartFlyBirdSwoopDown(task->data[1]);
        SetFlyBirdPlayerSpriteId(task->data[1], objectEvent->spriteId);
        StartSpriteAnim(&gSprites[task->data[1]], gSaveBlock2Ptr->playerGender * 2 + 2);
        sub_80877FC(&gSprites[task->data[1]], 1);
        gSprites[task->data[1]].callback = sub_8087828;
    }
}

static void sub_80878C0(struct Sprite * sprite)
{
    if (sprite->oam.affineMode != ST_OAM_AFFINE_OFF)
    {
        if (gOamMatrices[sprite->oam.matrixNum].a == 0x100 || gOamMatrices[sprite->oam.matrixNum].d == 0x100)
        {
            FreeSpriteOamMatrix(sprite);
            CalcCenterToCornerVec(sprite, sprite->oam.shape, sprite->oam.size, ST_OAM_AFFINE_OFF);
            StartSpriteAnim(sprite, 0);
            sprite->callback = SpriteCB_FlyBirdSwoopDown;
        }
    }
}

static void FlyInEffect_2(struct Task * task)
{
    struct Sprite * sprite;
    
    sub_80878C0(&gSprites[task->data[1]]);
    
    if (task->data[2] == 0 || (--task->data[2]) == 0)
    {
        sprite = &gSprites[gObjectEvents[gPlayerAvatar.objectEventId].spriteId];
        SetFlyBirdPlayerSpriteId(task->data[1], MAX_SPRITES);
        sprite->x += sprite->x2;
        sprite->y += sprite->y2;
        sprite->x2 = 0;
        sprite->y2 = 0;
        task->data[0]++;
        task->data[2] = 0;
    }
}

static void FlyInEffect_3(struct Task * task)
{
    s16 gUnknown_83CC1F0[18] = {
        -2,
        -4,
        -5,
        -6,
        -7,
        -8,
        -8,
        -8,
        -7,
        -7,
        -6,
        -5,
        -3,
        -2,
        0,
        2,
        4,
        8
    };

    gSprites[gPlayerAvatar.spriteId].y2 = gUnknown_83CC1F0[task->data[2]];
    
    if ((++task->data[2]) >= 18)
        task->data[0]++;
}

static void FlyInEffect_4(struct Task * task)
{
    if (gSprites[task->data[1]].data[7])
    {
        struct ObjectEvent *objectEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
        struct Sprite *sprite = &gSprites[objectEvent->spriteId];
        
        objectEvent->inanimate = FALSE;
        MoveObjectEventToMapCoords(objectEvent, objectEvent->currentCoords.x, objectEvent->currentCoords.y);
        
        sprite->x2 = 0;
        sprite->y2 = 0;
        sprite->coordOffsetEnabled = TRUE;
        
        StartPlayerAvatarSummonMonForFieldMoveAnim();
        ObjectEventSetHeldMovement(objectEvent, MOVEMENT_ACTION_START_ANIM_IN_DIRECTION);
        
        task->data[0]++;
    }
}

static void FlyInEffect_5(struct Task * task)
{
    if (ObjectEventClearHeldMovementIfFinished(&gObjectEvents[gPlayerAvatar.objectEventId]))
    {
        task->data[0]++;
        StartFlyBirdReturnToBall(task->data[1]);
    }
}

static void FlyInEffect_6(struct Task * task)
{
    if (gSprites[task->data[1]].data[7])
    {
        DestroySprite(&gSprites[task->data[1]]);
        task->data[0]++;
        task->data[1] = 0x10;
    }
}

static void FlyInEffect_7(struct Task * task)
{
    u32 state;
    struct ObjectEvent * objectEvent;
    
    if ((--task->data[1]) == 0)
    {
        objectEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
        
        if (task->data[15] & PLAYER_AVATAR_FLAG_SURFING)
        {
            state = PLAYER_AVATAR_GFX_RIDE;
            SetSurfBlob_BobState(objectEvent->fieldEffectSpriteId, 1);
        }
        else
            state = PLAYER_AVATAR_GFX_NORMAL;
        
        ObjectEventSetGraphicsId(objectEvent, GetPlayerAvatarGraphicsIdByStateId(state));
        ObjectEventTurn(objectEvent, DIR_SOUTH);
        gPlayerAvatar.flags = task->data[15];
        gPlayerAvatar.preventStep = FALSE;
        FieldEffectActiveListRemove(FLDEFF_FLY_IN);
        DestroyTask(FindTaskIdByFunc(Task_FldEffFlyIn));
    }
}

static void SpriteCB_FlyBirdReturnToBall(struct Sprite * sprite)
{
    if (sprite->data[7] == 0)
    {
        if (sprite->data[0] == 0)
        {
            sprite->oam.affineMode = ST_OAM_AFFINE_DOUBLE;
            sprite->affineAnims = sUnknown_83CC1CC;
            InitSpriteAffineAnim(sprite);
            StartSpriteAffineAnim(sprite, 1);
            sprite->x = gSaveBlock2Ptr->playerGender == MALE ? 0x70 : 0x64;
            sprite->y = -0x20;
            sprite->data[0]++;
            sprite->data[1] = 0xf0;
            sprite->data[2] = 0x800;
            sprite->data[4] = 0x80;
        }
        sprite->data[1] += sprite->data[2] >> 8;
        sprite->data[3] += sprite->data[2] >> 8;
        sprite->data[1] &= 0xff;
        
        sprite->x2 = Cos(sprite->data[1], 0x20);
        sprite->y2 = Sin(sprite->data[1], 0x78);
        
        if (sprite->data[2] > 0x100)
            sprite->data[2] -= sprite->data[4];

        if (sprite->data[4] < 0x100)
            sprite->data[4] += 24;

        if (sprite->data[2] < 0x100)
            sprite->data[2] = 0x100;

        if (sprite->data[3] >= 60)
        {
            sprite->data[7]++;
            FreeSpriteOamMatrix(sprite);
            sprite->invisible = TRUE;
        }
    }
}

static void StartFlyBirdReturnToBall(u32 spriteId)
{
    StartFlyBirdSwoopDown(spriteId);
    gSprites[spriteId].callback = SpriteCB_FlyBirdReturnToBall;
}

static void FieldCallback_Fly(void)
{
    FadeInFromBlack();
    CreateTask(Task_FlyOut, 0);
    ScriptContext2_Enable();
    FreezeObjectEvents();
    gFieldCallback = NULL;
}

void ReturnToFieldFromFlyMapSelect(void)
{
    SetMainCallback2(CB2_ReturnToField);
    gFieldCallback = FieldCallback_Fly;
}

static void FieldCallback_FlyArrive(void)
{
    Overworld_PlaySpecialMapMusic();
    FadeInFromBlack();
    CreateTask(Task_FlyIn, 0);
    gObjectEvents[gPlayerAvatar.objectEventId].invisible = TRUE;
    
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
        ObjectEventTurn(&gObjectEvents[gPlayerAvatar.objectEventId], DIR_WEST);

    ScriptContext2_Enable();
    FreezeObjectEvents();
    gFieldCallback = NULL;
}

static void Task_FlyOut(u32 taskId)
{
    struct Task * task = &gTasks[taskId];

    if (task->data[0] == 0)
    {
        if (!IsWeatherNotFadingIn())
            return;
        
        if (gUsingRideMon == RIDE_CHARIZARD)
            gFieldEffectArguments[0] = 0;
        else
        {
            gFieldEffectArguments[0] = GetCursorSelectionMonId();
            if (gFieldEffectArguments[0] >= PARTY_SIZE)
                gFieldEffectArguments[0] = 0;
        }
        FieldEffectStart(FLDEFF_USE_FLY);
        task->data[0]++;
    }
    
    if (!FieldEffectActiveListContains(FLDEFF_USE_FLY))
    {
        Overworld_ResetStateForLeavingMap();
        WarpIntoMap();
        SetMainCallback2(CB2_LoadMap);
        gFieldCallback = FieldCallback_FlyArrive;
        DestroyTask(taskId);
    }
}

static void Task_FlyIn(u32 taskId)
{
    struct Task * task = &gTasks[taskId];

    if (task->data[0] == 0)
    {
        if (gPaletteFade.active)
            return;
        
        FieldEffectStart(FLDEFF_FLY_IN);
        task->data[0]++;
    }
    
    if (!FieldEffectActiveListContains(FLDEFF_FLY_IN))
    {
        ScriptContext2_Disable();
        UnfreezeObjectEvents();
        DestroyTask(taskId);
    }
}

///////////////////////////////
// FLDEFF_QUESTION_MARK_ICON //
///////////////////////////////

static u32 FldEff_QuestionMarkIcon(void)
{
    CreateEmoticonSprite(FLDEFF_QUESTION_MARK_ICON, 0x52, 4);
    return 0;
}

//////////////////////////////////
// FLDEFF_FEET_IN_FLOWING_WATER //
//////////////////////////////////

static void SpriteCB_FeetInFlowingWater(struct Sprite * sprite)
{
    u8 objectEventId;

    if (TryGetObjectEventIdByLocalIdAndMap(sprite->data[0], sprite->data[1], sprite->data[2], &objectEventId) || !gObjectEvents[objectEventId].inShallowFlowingWater)
        FieldEffectStop(sprite, FLDEFF_FEET_IN_FLOWING_WATER);
    else
    {
        struct ObjectEvent *objectEvent = &gObjectEvents[objectEventId];
        struct Sprite *linkedSprite = &gSprites[objectEvent->spriteId];
        
        sprite->x = linkedSprite->x;
        sprite->y = linkedSprite->y;
        sprite->subpriority = linkedSprite->subpriority;
        
        UpdateObjectEventSpriteVisibility(sprite, FALSE);
        
        if (objectEvent->currentCoords.x != sprite->data[3] || objectEvent->currentCoords.y != sprite->data[4])
        {
            sprite->data[3] = objectEvent->currentCoords.x;
            sprite->data[4] = objectEvent->currentCoords.y;
            
            if (!sprite->invisible)
                PlaySE(SE_PUDDLE);
        }
    }
}

static u32 FldEff_FeetInFlowingWater(void)
{
    u32 spriteId;
    
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect0);

    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_Splash, 0, 0, 0);
    if (spriteId != MAX_SPRITES)
    {
        struct ObjectEvent * objectEvent = &gObjectEvents[GetObjectEventIdByLocalIdAndMap(gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2])];
        struct Sprite *sprite = &gSprites[spriteId];
        
        sprite->callback = SpriteCB_FeetInFlowingWater;
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gSprites[objectEvent->spriteId].oam.priority;
        sprite->data[0] = gFieldEffectArguments[0];
        sprite->data[1] = gFieldEffectArguments[1];
        sprite->data[2] = gFieldEffectArguments[2];
        sprite->data[3] = -1;
        sprite->data[4] = -1;
        sprite->y2 = (GetObjectEventGraphicsInfo(objectEvent->graphicsId)->height >> 1) - 4;
        StartSpriteAnim(sprite, 1);
    }
    return 0;
}

/////////////////////////////
// FLDEFF_BIKE_TIRE_TRACKS //
/////////////////////////////

static const struct SpriteFrameImage sPicTable_BikeTireTracks[] =
{
    overworld_frame(gFieldEffectObjectPic_BikeTireTracks, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_BikeTireTracks, 2, 2, 1),
    overworld_frame(gFieldEffectObjectPic_BikeTireTracks, 2, 2, 2),
    overworld_frame(gFieldEffectObjectPic_BikeTireTracks, 2, 2, 3),
};

static const union AnimCmd sBikeTireTracksAnim_South[] =
{
    ANIMCMD_FRAME(2, 1),
    ANIMCMD_END,
};

static const union AnimCmd sBikeTireTracksAnim_North[] =
{
    ANIMCMD_FRAME(2, 1),
    ANIMCMD_END,
};

static const union AnimCmd sBikeTireTracksAnim_West[] =
{
    ANIMCMD_FRAME(1, 1),
    ANIMCMD_END,
};

static const union AnimCmd sBikeTireTracksAnim_East[] =
{
    ANIMCMD_FRAME(1, 1),
    ANIMCMD_END,
};

static const union AnimCmd sBikeTireTracksAnim_SECornerTurn[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

static const union AnimCmd sBikeTireTracksAnim_SWCornerTurn[] =
{
    ANIMCMD_FRAME(0, 1, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd sBikeTireTracksAnim_NWCornerTurn[] =
{
    ANIMCMD_FRAME(3, 1, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd sBikeTireTracksAnim_NECornerTurn[] =
{
    ANIMCMD_FRAME(3, 1),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_BikeTireTracks[] =
{
    [DIR_NONE]      = sBikeTireTracksAnim_South,
    [DIR_SOUTH]     = sBikeTireTracksAnim_South,
    [DIR_NORTH]     = sBikeTireTracksAnim_North,
    [DIR_WEST]      = sBikeTireTracksAnim_West,
    [DIR_EAST]      = sBikeTireTracksAnim_East,
    [DIR_SOUTHWEST] = sBikeTireTracksAnim_SECornerTurn,
    [DIR_SOUTHEAST] = sBikeTireTracksAnim_SWCornerTurn,
    [DIR_NORTHWEST] = sBikeTireTracksAnim_NWCornerTurn,
    [DIR_NORTHEAST] = sBikeTireTracksAnim_NECornerTurn,
};

static const struct SpriteTemplate sSpriteTemplate_BikeTireTracks =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_0,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_BikeTireTracks,
    .images = sPicTable_BikeTireTracks,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_UpdateFootprintsTireTracks,
};

static u32 FldEff_BikeTireTracks(void)
{
    u32 spriteId;

    SetSpritePosToOffsetMapCoords((s16 *)&gFieldEffectArguments[0], (s16 *)&gFieldEffectArguments[1], 8, 8);
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect0);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_BikeTireTracks, gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2]);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gFieldEffectArguments[3];
        sprite->data[7] = FLDEFF_BIKE_TIRE_TRACKS;
        StartSpriteAnim(sprite, gFieldEffectArguments[4]);
    }
    return spriteId;
}

//////////////////////////
// FLDEFF_SAND_DISGUISE //
//////////////////////////

static const struct SpriteFrameImage sPicTable_SandDisguise[] =
{
    overworld_frame(gFieldEffectObjectPic_SandDisguise, 2, 4, 0),
    overworld_frame(gFieldEffectObjectPic_SandDisguise, 2, 4, 1),
    overworld_frame(gFieldEffectObjectPic_SandDisguise, 2, 4, 2),
    overworld_frame(gFieldEffectObjectPic_SandDisguise, 2, 4, 3),
    overworld_frame(gFieldEffectObjectPic_SandDisguise, 2, 4, 4),
    overworld_frame(gFieldEffectObjectPic_SandDisguise, 2, 4, 5),
    overworld_frame(gFieldEffectObjectPic_SandDisguise, 2, 4, 6),
};

static const struct SpriteTemplate sSpriteTemplate_SandDisguise =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = OBJ_EVENT_PAL_TAG_NPC_BLUE,
    .oam = &gObjectEventBaseOam_16x32,
    .anims = sAnimTable_TreeDisguise,
    .images = sPicTable_SandDisguise,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Disguise,
};

static u32 FldEff_SandDisguise(void)
{
    return ShowDisguiseFieldEffect(FLDEFF_SAND_DISGUISE, &sSpriteTemplate_SandDisguise);
}

/////////////////////////////
// FLDEFF_SMILEY_FACE_ICON //
/////////////////////////////

static u32 FldEff_SmileyFaceIcon(void)
{
    CreateEmoticonSprite(FLDEFF_SMILEY_FACE_ICON, 0x52, 3);
    return 0;
}

//////////////////////
// FLDEFF_SAND_PILE //
//////////////////////

static void SpriteCB_SandPile(struct Sprite *sprite);

static const struct SpriteFrameImage sPicTable_SandPile[] =
{
    overworld_frame(gFieldEffectObjectPic_SandPile, 2, 1, 0),
    overworld_frame(gFieldEffectObjectPic_SandPile, 2, 1, 1),
    overworld_frame(gFieldEffectObjectPic_SandPile, 2, 1, 2),
};

static const union AnimCmd sAnim_SandPile[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_FRAME(2, 4),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_SandPile[] =
{
    sAnim_SandPile,
};

static const struct SpriteTemplate sSpriteTemplate_SandPile =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_0,
    .oam = &gObjectEventBaseOam_16x8,
    .anims = sAnimTable_SandPile,
    .images = sPicTable_SandPile,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_SandPile,
};

static u32 FldEff_SandPile(void)
{
    u32 spriteId;
    
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect0);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_SandPile, 0, 0, 0);
    if (spriteId != MAX_SPRITES)
    {
        struct ObjectEvent * objectEvent = &gObjectEvents[GetObjectEventIdByLocalIdAndMap(gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2])];
        struct Sprite *sprite = &gSprites[spriteId];
        
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gSprites[objectEvent->spriteId].oam.priority;
        sprite->data[0] = gFieldEffectArguments[0];
        sprite->data[1] = gFieldEffectArguments[1];
        sprite->data[2] = gFieldEffectArguments[2];
        sprite->data[3] = gSprites[objectEvent->spriteId].x;
        sprite->data[4] = gSprites[objectEvent->spriteId].y;
        sprite->y2 = (GetObjectEventGraphicsInfo(objectEvent->graphicsId)->height >> 1) - 2;
        SeekSpriteAnim(sprite, 2);
    }
    return 0;
}

static void SpriteCB_SandPile(struct Sprite *sprite)
{
    u8 objectEventId;
    s16 x, y;

    if (TryGetObjectEventIdByLocalIdAndMap(sprite->data[0], sprite->data[1], sprite->data[2], &objectEventId) || !gObjectEvents[objectEventId].inSandPile)
        FieldEffectStop(sprite, FLDEFF_SAND_PILE);
    else
    {
        y = gSprites[gObjectEvents[objectEventId].spriteId].y;
        x = gSprites[gObjectEvents[objectEventId].spriteId].x;
        
        if (x != sprite->data[3] || y != sprite->data[4])
        {
            sprite->data[3] = x;
            sprite->data[4] = y;
            
            if (sprite->animEnded)
                StartSpriteAnim(sprite, 0);
        }
        sprite->x = x;
        sprite->y = y;
        sprite->subpriority = gSprites[gObjectEvents[objectEventId].spriteId].subpriority;
        UpdateObjectEventSpriteVisibility(sprite, FALSE);
    }
}

//////////////////////////
// FLDEFF_USE_VS_SEEKER //
//////////////////////////

static void Task_FldEffUseVsSeeker(u32 taskId);
static void UseVsSeekerEffect_1(struct Task * task);
static void UseVsSeekerEffect_2(struct Task * task);
static void UseVsSeekerEffect_3(struct Task * task);
static void UseVsSeekerEffect_4(struct Task * task);

static void (*const sUseVsSeekerEffectFuncs[])(struct Task * task) = {
    UseVsSeekerEffect_1,
    UseVsSeekerEffect_2,
    UseVsSeekerEffect_3,
    UseVsSeekerEffect_4
};

static u32 FldEff_UseVsSeeker(void)
{
    CreateTask(Task_FldEffUseVsSeeker, 0xFF);
    return 0;
}

static void Task_FldEffUseVsSeeker(u32 taskId)
{
    sUseVsSeekerEffectFuncs[gTasks[taskId].data[0]](&gTasks[taskId]);
}

static void UseVsSeekerEffect_1(struct Task * task)
{
    ScriptContext2_Enable();
    FreezeObjectEvents();
    gPlayerAvatar.preventStep = TRUE;
    task->data[0]++;
}

static void UseVsSeekerEffect_2(struct Task * task)
{
    struct ObjectEvent * playerObj = &gObjectEvents[gPlayerAvatar.objectEventId];
    
    if (!ObjectEventIsMovementOverridden(playerObj) || ObjectEventClearHeldMovementIfFinished(playerObj))
    {
        StartPlayerAvatarVsSeekerAnim();
        ObjectEventSetHeldMovement(playerObj, MOVEMENT_ACTION_START_ANIM_IN_DIRECTION);
        task->data[0]++;
    }
}

static void UseVsSeekerEffect_3(struct Task * task)
{
    struct ObjectEvent * playerObj = &gObjectEvents[gPlayerAvatar.objectEventId];
    
    if (ObjectEventClearHeldMovementIfFinished(playerObj))
    {
        ReturnPlayerToDefaultGraphicsIdByStateId(playerObj);
        ObjectEventForceSetHeldMovement(playerObj, GetFaceDirectionMovementAction(playerObj->facingDirection));
        task->data[0]++;
    }
}

static void UseVsSeekerEffect_4(struct Task * task)
{
    if (ObjectEventClearHeldMovementIfFinished(&gObjectEvents[gPlayerAvatar.objectEventId]))
    {
        gPlayerAvatar.preventStep = FALSE;
        FieldEffectActiveListRemove(FLDEFF_USE_VS_SEEKER);
        DestroyTask(FindTaskIdByFunc(Task_FldEffUseVsSeeker));
    }
}

////////////////////////
// FLDEFF_SHORT_GRASS //
////////////////////////

static void SpriteCB_ShortGrass(struct Sprite *sprite);

static const struct SpriteFrameImage sPicTable_ShortGrass[] =
{
    overworld_frame(gFieldEffectObjectPic_ShortGrass, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_ShortGrass, 2, 2, 1),
};

static const union AnimCmd sAnim_ShortGrass[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_ShortGrass[] =
{
    sAnim_ShortGrass,
};

static const struct SpriteTemplate sSpriteTemplate_ShortGrass =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_1,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_ShortGrass,
    .images = sPicTable_ShortGrass,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_ShortGrass,
};

static u32 FldEff_ShortGrass(void)
{
    u32 spriteId;
    
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect1);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_ShortGrass, 0, 0, 0);
    if (spriteId != MAX_SPRITES)
    {
        struct ObjectEvent * objectEvent = &gObjectEvents[GetObjectEventIdByLocalIdAndMap(gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2])];
        struct Sprite *sprite = &gSprites[spriteId];
        
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gSprites[objectEvent->spriteId].oam.priority;
        sprite->data[0] = gFieldEffectArguments[0];
        sprite->data[1] = gFieldEffectArguments[1];
        sprite->data[2] = gFieldEffectArguments[2];
        sprite->data[3] = gSprites[objectEvent->spriteId].x;
        sprite->data[4] = gSprites[objectEvent->spriteId].y;
    }
    return 0;
}

static void SpriteCB_ShortGrass(struct Sprite * sprite)
{
    u8 objectEventId;
    s16 x, y;

    if (TryGetObjectEventIdByLocalIdAndMap(sprite->data[0], sprite->data[1], sprite->data[2], &objectEventId) || !gObjectEvents[objectEventId].inShortGrass)
        FieldEffectStop(sprite, FLDEFF_SHORT_GRASS);
    else
    {
        struct Sprite *linkedSprite = &gSprites[gObjectEvents[objectEventId].spriteId];
        
        y = linkedSprite->y;
        x = linkedSprite->x;
        
        if (x != sprite->data[3] || y != sprite->data[4])
        {
            sprite->data[3] = x;
            sprite->data[4] = y;
            
            if (sprite->animEnded)
                StartSpriteAnim(sprite, 0);
        }
        sprite->x = x;
        sprite->y = y;
        sprite->y2 = (GetObjectEventGraphicsInfo(gObjectEvents[objectEventId].graphicsId)->height >> 1) - 8;
        sprite->subpriority = linkedSprite->subpriority - 1;
        sprite->oam.priority = linkedSprite->oam.priority;
        UpdateObjectEventSpriteVisibility(sprite, linkedSprite->invisible);
    }
}

//////////////////////////////
// FLDEFF_HOT_SPRINGS_WATER //
//////////////////////////////

static void SpriteCB_HotSpringsWater(struct Sprite * sprite);

static const struct SpriteFrameImage sPicTable_HotSpringsWater[] = { gFieldEffectObjectPic_HotSpringsWater, 0x80 };

static const union AnimCmd sAnim_HotSpringsWater[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_HotSpringsWater[] =
{
    sAnim_HotSpringsWater,
};

static const struct SpriteTemplate sSpriteTemplate_HotSpringsWater =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_1,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_HotSpringsWater,
    .images = sPicTable_HotSpringsWater,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_HotSpringsWater,
};

static u32 FldEff_HotSpringsWater(void)
{
    u32 spriteId;
    
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect1);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_HotSpringsWater, 0, 0, 0);
    if (spriteId != MAX_SPRITES)
    {
        struct ObjectEvent * objectEvent = &gObjectEvents[GetObjectEventIdByLocalIdAndMap(gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2])];
        struct Sprite *sprite = &gSprites[spriteId];
        
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gSprites[objectEvent->spriteId].oam.priority;
        sprite->data[0] = gFieldEffectArguments[0];
        sprite->data[1] = gFieldEffectArguments[1];
        sprite->data[2] = gFieldEffectArguments[2];
        sprite->data[3] = gSprites[objectEvent->spriteId].x;
        sprite->data[4] = gSprites[objectEvent->spriteId].y;
    }
    return 0;
}

static void SpriteCB_HotSpringsWater(struct Sprite * sprite)
{
    u8 objectEventId;

    if (TryGetObjectEventIdByLocalIdAndMap(sprite->data[0], sprite->data[1], sprite->data[2], &objectEventId) || !gObjectEvents[objectEventId].inHotSprings)
        FieldEffectStop(sprite, FLDEFF_HOT_SPRINGS_WATER);
    else
    {
        struct Sprite *linkedSprite = &gSprites[gObjectEvents[objectEventId].spriteId];
        sprite->x = linkedSprite->x;
        sprite->y = (GetObjectEventGraphicsInfo(gObjectEvents[objectEventId].graphicsId)->height >> 1) + linkedSprite->y - 8;
        sprite->subpriority = linkedSprite->subpriority - 1;
        UpdateObjectEventSpriteVisibility(sprite, FALSE);
    }
}

//////////////////////////
// FLDEFF_USE_WATERFALL //
//////////////////////////

static void Task_UseWaterfall(u32 taskId);
static bool32 waterfall_0_setup(struct Task * task, struct ObjectEvent * playerObj);
static bool32 waterfall_1_do_anim_probably(struct Task * task, struct ObjectEvent * playerObj);
static bool32 waterfall_2_wait_anim_finish_probably(struct Task * task, struct ObjectEvent * playerObj);
static bool32 waterfall_3_move_player_probably(struct Task * task, struct ObjectEvent * playerObj);
static bool32 waterfall_4_wait_player_move_probably(struct Task * task, struct ObjectEvent * playerObj);

static bool32 (*const sUseWaterfallFieldEffectFuncs[])(struct Task * task, struct ObjectEvent * playerObj) = {
    waterfall_0_setup,
    waterfall_1_do_anim_probably,
    waterfall_2_wait_anim_finish_probably,
    waterfall_3_move_player_probably,
    waterfall_4_wait_player_move_probably
};

static u32 FldEff_UseWaterfall(void)
{
    u32 taskId = CreateTask(Task_UseWaterfall, 0xFF);
    gTasks[taskId].data[1] = gFieldEffectArguments[0];
    Task_UseWaterfall(taskId);
    return 0;
}

static void Task_UseWaterfall(u32 taskId)
{
    while (sUseWaterfallFieldEffectFuncs[gTasks[taskId].data[0]](&gTasks[taskId], &gObjectEvents[gPlayerAvatar.objectEventId]));
}

static bool32 waterfall_0_setup(struct Task * task, struct ObjectEvent * playerObj)
{
    ScriptContext2_Enable();
    gPlayerAvatar.preventStep = TRUE;
    task->data[0]++;
    return FALSE;
}

static bool32 waterfall_1_do_anim_probably(struct Task * task, struct ObjectEvent * playerObj)
{
    ScriptContext2_Enable();
    
    if (!ObjectEventIsMovementOverridden(playerObj))
    {
        ObjectEventClearHeldMovementIfFinished(playerObj);
        gFieldEffectArguments[0] = task->data[1];
        FieldEffectStart(FLDEFF_FIELD_MOVE_SHOW_MON_INIT);
        task->data[0]++;
    }
    return FALSE;
}

static bool32 waterfall_2_wait_anim_finish_probably(struct Task * task, struct ObjectEvent * playerObj)
{
    if (FieldEffectActiveListContains(FLDEFF_FIELD_MOVE_SHOW_MON))
        return FALSE;
    
    task->data[0]++;
    return TRUE;
}

static bool32 waterfall_3_move_player_probably(struct Task * task, struct ObjectEvent * playerObj)
{
    ObjectEventSetHeldMovement(playerObj, GetWalkSlowestMovementAction(DIR_NORTH));
    task->data[0]++;
    return FALSE;
}

static bool32 waterfall_4_wait_player_move_probably(struct Task * task, struct ObjectEvent * playerObj)
{
    if (!ObjectEventClearHeldMovementIfFinished(playerObj))
        return FALSE;
    
    if (MetatileBehavior_IsWaterfall(playerObj->currentMetatileBehavior))
    {
        task->data[0] = 3;
        return TRUE;
    }
    ScriptContext2_Disable();
    gPlayerAvatar.preventStep = FALSE;
    DestroyTask(FindTaskIdByFunc(Task_UseWaterfall));
    FieldEffectActiveListRemove(FLDEFF_USE_WATERFALL);
    return FALSE;
}

/////////////////////
// FLDEFF_USE_DIVE //
/////////////////////

static void Task_Dive(u32 taskId);
static bool32 dive_1_lock(struct Task * task);
static bool32 dive_2_unknown(struct Task * task);
static bool32 dive_3_unknown(struct Task * task);

static bool32 (*const sDiveFieldEffectFuncPtrs[])(struct Task * task) = {
    dive_1_lock,
    dive_2_unknown,
    dive_3_unknown
};

static u32 FldEff_UseDive(void)
{
    u32 taskId = CreateTask(Task_Dive, 0xFF);
    gTasks[taskId].data[15] = gFieldEffectArguments[0];
    gTasks[taskId].data[14] = gFieldEffectArguments[1];
    Task_Dive(taskId);
    return 0;
}

static void Task_Dive(u32 taskId)
{
    while (sDiveFieldEffectFuncPtrs[gTasks[taskId].data[0]](&gTasks[taskId]));
}

static bool32 dive_1_lock(struct Task * task)
{
    gPlayerAvatar.preventStep = TRUE;
    task->data[0]++;
    return FALSE;
}

static bool32 dive_2_unknown(struct Task * task)
{
    ScriptContext2_Enable();
    gFieldEffectArguments[0] = task->data[15];
    FieldEffectStart(FLDEFF_FIELD_MOVE_SHOW_MON_INIT);
    task->data[0]++;
    return FALSE;
}

static bool32 dive_3_unknown(struct Task * task)
{
    struct MapPosition pos;
    
    PlayerGetDestCoords(&pos.x, &pos.y);
    
    if (!FieldEffectActiveListContains(FLDEFF_FIELD_MOVE_SHOW_MON))
    {
        dive_warp(&pos, gObjectEvents[gPlayerAvatar.objectEventId].currentMetatileBehavior);
        DestroyTask(FindTaskIdByFunc(Task_Dive));
        FieldEffectActiveListRemove(FLDEFF_USE_DIVE);
    }
    return FALSE;
}

///////////////////
// FLDEFF_UNK_45 //
///////////////////

static void Task_FldEffUnk45(u32 taskId)
{
    if (!gPaletteFade.active)
    {
        FieldEffectActiveListRemove(FLDEFF_UNK_45);
        DestroyTask(taskId);
    }
}

static u32 FldEff_Unk45(void)
{
    BlendPalettes(PALETTES_ALL, 0x10, RGB_WHITE);
    BeginNormalPaletteFade(PALETTES_ALL, -1, 0x0F, 0x00, RGB_WHITE);
    CreateTask(Task_FldEffUnk45, 90);
    return 0;
}

///////////////////
// FLDEFF_X_ICON //
///////////////////

static u32 FldEff_XIcon(void)
{
    CreateEmoticonSprite(FLDEFF_X_ICON, 0x52, 2);
    return 0;
}

//////////////////////
// FLDEFF_CAVE_DUST //
//////////////////////

static const struct SpriteFrameImage sPicTable_CaveDust[] = 
{
    overworld_frame(gFieldEffectObjectPic_CaveDust, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_CaveDust, 2, 2, 1),
    overworld_frame(gFieldEffectObjectPic_CaveDust, 2, 2, 2),
    overworld_frame(gFieldEffectObjectPic_CaveDust, 2, 2, 3),
};

static const struct SpriteTemplate sSpriteTemplate_CaveDust = {
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_2,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_WaterSurfacing,
    .images = sPicTable_CaveDust,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_WaitFieldEffectSpriteAnim,
};

static u32 FldEff_CaveDust(void)
{
    u32 spriteId;
    
    SetSpritePosToOffsetMapCoords((s16 *)&gFieldEffectArguments[0], (s16 *)&gFieldEffectArguments[1], 8, 8);
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect2);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_CaveDust, gFieldEffectArguments[0], gFieldEffectArguments[1], 0xFF);
    if (spriteId != MAX_SPRITES)
    {
        gSprites[spriteId].coordOffsetEnabled = TRUE;
        gSprites[spriteId].data[0] = FLDEFF_CAVE_DUST;
    }
    return spriteId;
}

/////////////////////
// FLDEFF_POKEBALL //
/////////////////////

static void SpriteCB_SlidingPokeballs(struct Sprite *sprite);

static const union AnimCmd sAnim_SlidingPokeball[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_SlidingPokeball[] = { sAnim_SlidingPokeball };

static const struct SpriteFrameImage sPicTable_SlidingPokeball[] =
{
    {
        .data = sSpriteImage_SlidingPokeball, 
        .size = 0x200,
    },
};

static const union AffineAnimCmd sSpriteAffineAnim_SlidingPokeball1[] =
{
    AFFINEANIMCMD_FRAME(0, 0, -4, 1),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd sSpriteAffineAnim_SlidingPokeball2[] =
{
    AFFINEANIMCMD_FRAME(0, 0, 4, 1),
    AFFINEANIMCMD_JUMP(0),
};

static const union AffineAnimCmd *const sSpriteAffineAnimTable_SlidingPokeball[] =
{
    sSpriteAffineAnim_SlidingPokeball1,
    sSpriteAffineAnim_SlidingPokeball2,
};

static const struct SpriteTemplate sSpriteTemplate_SlidingPokeball =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_SLIDING_POKEBALL,
    .oam = &gObjectEventBaseOam_32x32,
    .anims = sAnimTable_SlidingPokeball,
    .images = sPicTable_SlidingPokeball,
    .affineAnims = sSpriteAffineAnimTable_SlidingPokeball,
    .callback = SpriteCB_SlidingPokeballs,
};

static const s16 sUnknown_83FA40E[] = { 8, -8 };

static u32 FldEff_Pokeball(void)
{
    u32 spriteId;
    
    FieldEffect_LoadPal(&gSpritePalette_SlidingPokeball);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_SlidingPokeball, gFieldEffectArguments[0], gFieldEffectArguments[1], 0);
    gSprites[spriteId].oam.priority = 0;
    gSprites[spriteId].oam.affineMode = 1;
    gSprites[spriteId].data[0] = gFieldEffectArguments[2];
    gSprites[spriteId].data[1] = gFieldEffectArguments[3];
    gSprites[spriteId].data[2] = -1;
    InitSpriteAffineAnim(&gSprites[spriteId]);
    StartSpriteAffineAnim(&gSprites[spriteId], gFieldEffectArguments[2]);
    return 0;
}

#define SOME_VRAM_STORE(ptr, posY, posX, toStore)                       \
{                                                                       \
    u32 index = (posY) * 32 + posX;                                     \
    ptr[index] = toStore;                                               \
}

static void SpriteCB_SlidingPokeballs(struct Sprite *sprite)
{
    s16 arr0[ARRAY_COUNT(sUnknown_83FA40E)];

    memcpy(arr0, sUnknown_83FA40E, sizeof(sUnknown_83FA40E));
    
    if (sprite->data[1])
        --sprite->data[1];
    else
    {
        if ((u16)sprite->x <= 240)
        {
            s16 posX = sprite->x >> 3;
            s16 posY = sprite->y >> 3;

            if (posX != sprite->data[2])
            {
                u16 *ptr;

                sprite->data[2] = posX;
                ptr = (u16 *)BG_SCREEN_ADDR((GetGpuReg(REG_OFFSET_BG0CNT) >> 8) & 0x1F);
                SOME_VRAM_STORE(ptr, posY - 2, posX, 0xF001);
                SOME_VRAM_STORE(ptr, posY - 1, posX, 0xF001);
                SOME_VRAM_STORE(ptr, posY - 0, posX, 0xF001);
                SOME_VRAM_STORE(ptr, posY + 1, posX, 0xF001);
            }
        }
        sprite->x += arr0[sprite->data[0]];
        
        if (sprite->x < -15 || sprite->x > 255)
            FieldEffectStop(sprite, FLDEFF_POKEBALL);
    }
}

///////////////////////////
// FLDEFF_POP_OUT_OF_ASH //
///////////////////////////

static void SpriteCB_PopOutOfAsh(struct Sprite * sprite);

static const struct SpriteFrameImage sPicTable_AshPuff[] =
{
    overworld_frame(gFieldEffectObjectPic_AshPuff, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_AshPuff, 2, 2, 1),
    overworld_frame(gFieldEffectObjectPic_AshPuff, 2, 2, 2),
    overworld_frame(gFieldEffectObjectPic_AshPuff, 2, 2, 3),
    overworld_frame(gFieldEffectObjectPic_AshPuff, 2, 2, 4),
};

static const union AnimCmd sAnim_AshPuff[] =
{
    ANIMCMD_FRAME(0, 6),
    ANIMCMD_FRAME(1, 6),
    ANIMCMD_FRAME(2, 6),
    ANIMCMD_FRAME(3, 6),
    ANIMCMD_FRAME(4, 6),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_AshPuff[] =
{
    sAnim_AshPuff,
};

static const struct SpriteTemplate sSpriteTemplate_AshPuff =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_ASH,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_AshPuff,
    .images = sPicTable_AshPuff,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_PopOutOfAsh,
};

static u32 FldEff_PopOutOfAsh(void)
{
    u32 spriteId;
    
    SetSpritePosToOffsetMapCoords((s16 *)&gFieldEffectArguments[0], (s16 *)&gFieldEffectArguments[1], 8, 8);
    FieldEffect_LoadFadedPal(&gSpritePalette_Ash);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_AshPuff, gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2]);
    gSprites[spriteId].oam.priority = gFieldEffectArguments[3];
    gSprites[spriteId].coordOffsetEnabled = TRUE;
    gSprites[spriteId].data[0] = FLDEFF_POP_OUT_OF_ASH;
    
    return spriteId;
}

static void SpriteCB_PopOutOfAsh(struct Sprite * sprite)
{
    if (sprite->animEnded)
        FieldEffectStop(sprite, sprite->data[0]);
}

///////////////////////////////
// FLDEFF_LAVARIDGE_GYM_WARP //
///////////////////////////////

static void Task_LavaridgeGymB1FWarp(u32 taskId);
static bool32 LavaridgeGymB1FWarpEffect_1(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite);
static bool32 LavaridgeGymB1FWarpEffect_2(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite);
static bool32 LavaridgeGymB1FWarpEffect_3(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite);
static bool32 LavaridgeGymB1FWarpEffect_4(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite);
static bool32 LavaridgeGymB1FWarpEffect_5(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite);
static bool32 LavaridgeGymB1FWarpEffect_6(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite);
static void Task_LavaridgeGymB1FWarpExit(u32 taskId);
static bool32 LavaridgeGymB1FWarpExitEffect_1(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite);
static bool32 LavaridgeGymB1FWarpExitEffect_2(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite);
static bool32 LavaridgeGymB1FWarpExitEffect_3(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite);
static bool32 LavaridgeGymB1FWarpExitEffect_4(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite);
static void Task_LavaridgeGym1FWarp(u32 taskId);
static bool32 LavaridgeGym1FWarpEffect_1(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite);
static bool32 LavaridgeGym1FWarpEffect_2(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite);
static bool32 LavaridgeGym1FWarpEffect_3(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite);
static bool32 LavaridgeGym1FWarpEffect_4(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite);
static bool32 LavaridgeGym1FWarpEffect_5(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite);
static void Task_FallWarpFieldEffect(u32 taskId);
static bool32 FallWarpEffect_1(struct Task * task);
static bool32 FallWarpEffect_2(struct Task * task);
static bool32 FallWarpEffect_3(struct Task * task);
static bool32 FallWarpEffect_4(struct Task * task);
static bool32 FallWarpEffect_5(struct Task * task);
static bool32 FallWarpEffect_6(struct Task * task);
static bool32 FallWarpEffect_7(struct Task * task);

static bool32 (*const sLavaridgeGymB1FWarpEffectFuncs[])(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite) = {
    LavaridgeGymB1FWarpEffect_1,
    LavaridgeGymB1FWarpEffect_2,
    LavaridgeGymB1FWarpEffect_3,
    LavaridgeGymB1FWarpEffect_4,
    LavaridgeGymB1FWarpEffect_5,
    LavaridgeGymB1FWarpEffect_6
};

static bool32 (*const sLavaridgeGymB1FWarpExitEffectFuncs[])(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite) = {
    LavaridgeGymB1FWarpExitEffect_1,
    LavaridgeGymB1FWarpExitEffect_2,
    LavaridgeGymB1FWarpExitEffect_3,
    LavaridgeGymB1FWarpExitEffect_4
};

static bool32 (*const sLavaridgeGym1FWarpEffectFuncs[])(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite) = {
    LavaridgeGym1FWarpEffect_1,
    LavaridgeGym1FWarpEffect_2,
    LavaridgeGym1FWarpEffect_3,
    LavaridgeGym1FWarpEffect_4,
    LavaridgeGym1FWarpEffect_5
};

static bool32 (*const sFallWarpEffectCBPtrs[])(struct Task * task) = {
    FallWarpEffect_1,
    FallWarpEffect_2,
    FallWarpEffect_3,
    FallWarpEffect_4,
    FallWarpEffect_5,
    FallWarpEffect_6,
    FallWarpEffect_7
};

static const struct SpriteFrameImage sPicTable_AshLaunch[] =
{
    overworld_frame(gFieldEffectObjectPic_AshLaunch, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_AshLaunch, 2, 2, 1),
    overworld_frame(gFieldEffectObjectPic_AshLaunch, 2, 2, 2),
    overworld_frame(gFieldEffectObjectPic_AshLaunch, 2, 2, 3),
    overworld_frame(gFieldEffectObjectPic_AshLaunch, 2, 2, 4),
};

static const union AnimCmd sAnim_AshLaunch[] =
{
    ANIMCMD_FRAME(0, 6),
    ANIMCMD_FRAME(1, 6),
    ANIMCMD_FRAME(2, 6),
    ANIMCMD_FRAME(3, 6),
    ANIMCMD_FRAME(4, 6),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_AshLaunch[] =
{
    sAnim_AshLaunch,
};

static const struct SpriteTemplate sSpriteTemplate_AshLaunch =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_ASH,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_AshLaunch,
    .images = sPicTable_AshLaunch,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_PopOutOfAsh,
};

static u32 FldEff_LavaridgeGymWarp(void)
{
    u32 spriteId;
    
    SetSpritePosToOffsetMapCoords((s16 *)&gFieldEffectArguments[0], (s16 *)&gFieldEffectArguments[1], 8, 8);
    FieldEffect_LoadFadedPal(&gSpritePalette_Ash);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_AshLaunch, gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2]);
    gSprites[spriteId].oam.priority = gFieldEffectArguments[3];
    gSprites[spriteId].coordOffsetEnabled = TRUE;
    gSprites[spriteId].data[0] = FLDEFF_LAVARIDGE_GYM_WARP;
    
    return spriteId;
}

void StartLavaridgeGymB1FWarp(u32 priority)
{
    CreateTask(Task_LavaridgeGymB1FWarp, priority);
}

static void Task_LavaridgeGymB1FWarp(u32 taskId)
{
    while (sLavaridgeGymB1FWarpEffectFuncs[gTasks[taskId].data[0]](&gTasks[taskId], &gObjectEvents[gPlayerAvatar.objectEventId], &gSprites[gPlayerAvatar.spriteId]));
}

static bool32 LavaridgeGymB1FWarpEffect_1(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite)
{
    FreezeObjectEvents();
    CameraObjectReset2();
    SetCameraPanningCallback(NULL);
    gPlayerAvatar.preventStep = TRUE;
    objectEvent->fixedPriority = TRUE;
    task->data[1] = 1;
    task->data[0]++;
    return TRUE;
}

static bool32 LavaridgeGymB1FWarpEffect_2(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite)
{
    SetCameraPanning(0, task->data[1]);
    
    task->data[1] = -task->data[1];
    
    if (++task->data[2] > 7)
    {
        task->data[2] = 0;
        task->data[0]++;
    }
    return FALSE;
}

static bool32 LavaridgeGymB1FWarpEffect_3(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite)
{
    sprite->y2 = 0;
    task->data[3] = 1;
    
    gFieldEffectArguments[0] = objectEvent->currentCoords.x;
    gFieldEffectArguments[1] = objectEvent->currentCoords.y;
    gFieldEffectArguments[2] = sprite->subpriority - 1;
    gFieldEffectArguments[3] = sprite->oam.priority;
    FieldEffectStart(FLDEFF_LAVARIDGE_GYM_WARP);
    
    PlaySE(SE_M_EXPLOSION);
    
    task->data[0]++;
    return TRUE;
}

static bool32 LavaridgeGymB1FWarpEffect_4(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite)
{
    s16 centerToCornerVecY;
    
    SetCameraPanning(0, task->data[1]);
    
    if (task->data[1] = -task->data[1], ++task->data[2] <= 17)
    {
        if (!(task->data[2] & 1) && (task->data[1] <= 3))
            task->data[1] <<= 1;
    }
    else if (!(task->data[2] & 4) && (task->data[1] > 0))
        task->data[1] >>= 1;
    
    if (task->data[2] > 6)
    {
        centerToCornerVecY = -(sprite->centerToCornerVecY << 1);
        
        if (sprite->y2 > -(sprite->y + sprite->centerToCornerVecY + gSpriteCoordOffsetY + centerToCornerVecY))
        {
            sprite->y2 -= task->data[3];
            
            if (task->data[3] <= 7)
                task->data[3]++;
        }
        else
            task->data[4] = 1;
    }
    
    if (task->data[5] == 0 && sprite->y2 < -0x10)
    {
        task->data[5]++;
        objectEvent->fixedPriority = TRUE;
        sprite->oam.priority = 1;
        sprite->subspriteMode = SUBSPRITES_IGNORE_PRIORITY;
    }
    
    if (task->data[1] == 0 && task->data[4] != 0)
        task->data[0]++;

    return FALSE;
}

static bool32 LavaridgeGymB1FWarpEffect_5(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite)
{
    TryFadeOutOldMapMusic();
    WarpFadeOutScreen();
    task->data[0]++;
    return FALSE;
}

static void FieldCB_LavaridgeGymB1FWarpExit(void)
{
    Overworld_PlaySpecialMapMusic();
    WarpFadeInScreen();
    ScriptContext2_Enable();
    gFieldCallback = NULL;
    CreateTask(Task_LavaridgeGymB1FWarpExit, 0);
}

static bool32 LavaridgeGymB1FWarpEffect_6(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite)
{
    if (!gPaletteFade.active && IsNotWaitingForBGMStop())
    {
        WarpIntoMap();
        gFieldCallback = FieldCB_LavaridgeGymB1FWarpExit;
        SetMainCallback2(CB2_LoadMap);
        DestroyTask(FindTaskIdByFunc(Task_LavaridgeGymB1FWarp));
    }
    return FALSE;
}

static void Task_LavaridgeGymB1FWarpExit(u32 taskId)
{
    while (sLavaridgeGymB1FWarpExitEffectFuncs[gTasks[taskId].data[0]](&gTasks[taskId], &gObjectEvents[gPlayerAvatar.objectEventId], &gSprites[gPlayerAvatar.spriteId]));
}

static bool32 LavaridgeGymB1FWarpExitEffect_1(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite)
{
    CameraObjectReset2();
    FreezeObjectEvents();
    gPlayerAvatar.preventStep = TRUE;
    objectEvent->invisible = TRUE;
    task->data[0]++;
    return FALSE;
}

static bool32 LavaridgeGymB1FWarpExitEffect_2(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite)
{
    if (IsWeatherNotFadingIn())
    {
        gFieldEffectArguments[0] = objectEvent->currentCoords.x;
        gFieldEffectArguments[1] = objectEvent->currentCoords.y;
        gFieldEffectArguments[2] = sprite->subpriority - 1;
        gFieldEffectArguments[3] = sprite->oam.priority;
        task->data[1] = FieldEffectStart(FLDEFF_POP_OUT_OF_ASH);
        task->data[0]++;
    }
    return FALSE;
}

static bool32 LavaridgeGymB1FWarpExitEffect_3(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite)
{
    sprite = &gSprites[task->data[1]];
    
    if (sprite->animCmdIndex > 1)
    {
        task->data[0]++;
        objectEvent->invisible = FALSE;
        CameraObjectReset1();
        PlaySE(SE_M_DIG);
        ObjectEventSetHeldMovement(objectEvent, GetJumpMovementAction(DIR_EAST));
    }
    return FALSE;
}

static bool32 LavaridgeGymB1FWarpExitEffect_4(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite)
{
    if (ObjectEventClearHeldMovementIfFinished(objectEvent))
    {
        gPlayerAvatar.preventStep = FALSE;
        ScriptContext2_Disable();
        UnfreezeObjectEvents();
        DestroyTask(FindTaskIdByFunc(Task_LavaridgeGymB1FWarpExit));
    }
    return FALSE;
}

void StartLavaridgeGym1FWarp(u32 priority)
{
    CreateTask(Task_LavaridgeGym1FWarp, priority);
}

static void Task_LavaridgeGym1FWarp(u32 taskId)
{
    while(sLavaridgeGym1FWarpEffectFuncs[gTasks[taskId].data[0]](&gTasks[taskId], &gObjectEvents[gPlayerAvatar.objectEventId], &gSprites[gPlayerAvatar.spriteId]));
}

static bool32 LavaridgeGym1FWarpEffect_1(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite)
{
    FreezeObjectEvents();
    CameraObjectReset2();
    gPlayerAvatar.preventStep = TRUE;
    objectEvent->fixedPriority = TRUE;
    task->data[0]++;
    return FALSE;
}

static bool32 LavaridgeGym1FWarpEffect_2(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite)
{
    if (ObjectEventClearHeldMovementIfFinished(objectEvent))
    {
        if (task->data[1] > 3)
        {
            gFieldEffectArguments[0] = objectEvent->currentCoords.x;
            gFieldEffectArguments[1] = objectEvent->currentCoords.y;
            gFieldEffectArguments[2] = sprite->subpriority - 1;
            gFieldEffectArguments[3] = sprite->oam.priority;
            task->data[1] = FieldEffectStart(FLDEFF_POP_OUT_OF_ASH);
            task->data[0]++;
        }
        else
        {
            task->data[1]++;
            ObjectEventSetHeldMovement(objectEvent, GetWalkInPlaceFastMovementAction(objectEvent->facingDirection));
            PlaySE(SE_LAVARIDGE_FALL_WARP);
        }
    }
    return FALSE;
}

static bool32 LavaridgeGym1FWarpEffect_3(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite)
{
    if (gSprites[task->data[1]].animCmdIndex == 2)
    {
        objectEvent->invisible = TRUE;
        task->data[0]++;
    }
    return FALSE;
}

static bool32 LavaridgeGym1FWarpEffect_4(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite)
{
    if (!FieldEffectActiveListContains(FLDEFF_POP_OUT_OF_ASH))
    {
        TryFadeOutOldMapMusic();
        WarpFadeOutScreen();
        task->data[0]++;
    }
    return FALSE;
}

static bool32 LavaridgeGym1FWarpEffect_5(struct Task * task, struct ObjectEvent * objectEvent, struct Sprite * sprite)
{
    if (!gPaletteFade.active && IsNotWaitingForBGMStop())
    {
        WarpIntoMap();
        gFieldCallback = FieldCB_FallWarpExit;
        SetMainCallback2(CB2_LoadMap);
        DestroyTask(FindTaskIdByFunc(Task_LavaridgeGym1FWarp));
    }
    return FALSE;
}

void FieldCB_FallWarpExit(void)
{
    Overworld_PlaySpecialMapMusic();
    WarpFadeInScreen();
    ScriptContext2_Enable();
    FreezeObjectEvents();
    CreateTask(Task_FallWarpFieldEffect, 0);
    gFieldCallback = NULL;
}

static void Task_FallWarpFieldEffect(u32 taskId)
{
    while (sFallWarpEffectCBPtrs[gTasks[taskId].data[0]](&gTasks[taskId]));
}

static bool32 FallWarpEffect_1(struct Task * task)
{
    struct ObjectEvent * playerObject = &gObjectEvents[gPlayerAvatar.objectEventId];
    struct Sprite * playerSprite = &gSprites[gPlayerAvatar.spriteId];

    CameraObjectReset2();
    playerObject->invisible = TRUE;
    gPlayerAvatar.preventStep = TRUE;
    ObjectEventSetHeldMovement(playerObject, GetFaceDirectionMovementAction(GetPlayerFacingDirection()));
    task->data[4] = playerSprite->subspriteMode;
    playerObject->fixedPriority = TRUE;
    playerSprite->oam.priority = 1;
    playerSprite->subspriteMode = SUBSPRITES_IGNORE_PRIORITY;
    task->data[0]++;
    return TRUE;
}

static bool32 FallWarpEffect_2(struct Task * task)
{
    if (IsWeatherNotFadingIn())
        task->data[0]++;

    return FALSE;
}

static bool32 FallWarpEffect_3(struct Task * task)
{
    struct Sprite * sprite = &gSprites[gPlayerAvatar.spriteId];
    s16 centerToCornerVecY = -(sprite->centerToCornerVecY << 1);

    sprite->y2 = -(sprite->y + sprite->centerToCornerVecY + gSpriteCoordOffsetY + centerToCornerVecY);
    task->data[1] = 1;
    task->data[2] = 0;
    gObjectEvents[gPlayerAvatar.objectEventId].invisible = FALSE;
    PlaySE(SE_FALL);
    task->data[0]++;
    return FALSE;
}

static bool32 FallWarpEffect_4(struct Task * task)
{
    struct ObjectEvent * objectEvent = &gObjectEvents[gPlayerAvatar.objectEventId];
    struct Sprite * sprite = &gSprites[gPlayerAvatar.spriteId];

    sprite->y2 += task->data[1];
    
    if (task->data[1] < 8)
    {
        task->data[2] += task->data[1];
        if (task->data[2] & 0xf)
            task->data[1] <<= 1;
    }
    if (task->data[3] == 0 && sprite->y2 >= -16)
    {
        task->data[3]++;
        objectEvent->fixedPriority = FALSE;
        sprite->subspriteMode = task->data[4];
        objectEvent->triggerGroundEffectsOnMove = TRUE;
    }
    if (sprite->y2 >= 0)
    {
        PlaySE(SE_M_STRENGTH);
        objectEvent->triggerGroundEffectsOnStop = TRUE;
        objectEvent->landingJump = TRUE;
        sprite->y2 = 0;
        task->data[0]++;
    }
    return FALSE;
}

static bool32 FallWarpEffect_5(struct Task * task)
{
    task->data[0]++;
    task->data[1] = 4;
    task->data[2] = 0;
    SetCameraPanningCallback(NULL);
    return TRUE;
}

static bool32 FallWarpEffect_6(struct Task * task)
{
    SetCameraPanning(0, task->data[1]);
    
    task->data[1] = -task->data[1];
    task->data[2]++;
    
    if ((task->data[2] & 3) == 0)
        task->data[1] >>= 1;

    if (task->data[1] == 0)
        task->data[0]++;

    return FALSE;
}

static bool32 FallWarpEffect_7(struct Task * task)
{
    s16 x, y;
    
    gPlayerAvatar.preventStep = FALSE;
    ScriptContext2_Disable();
    CameraObjectReset1();
    UnfreezeObjectEvents();
    InstallCameraPanAheadCallback();
    PlayerGetDestCoords(&x, &y);
    
    // Seafoam Islands
    if (MetatileBehavior_IsSurfableInSeafoamIslands(MapGridGetMetatileBehaviorAt(x, y)))
    {
        VarSet(VAR_TEMP_1, 1);
        SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_SURFING);
    }
    DestroyTask(FindTaskIdByFunc(Task_FallWarpFieldEffect));
    return FALSE;
}

////////////////////
// FLDEFF_BUBBLES //
////////////////////

static void SpriteCB_Bubbles(struct Sprite * sprite);

static const struct SpriteFrameImage sPicTable_Bubbles[] =
{
    overworld_frame(gFieldEffectObjectPic_Bubbles, 2, 4, 0),
    overworld_frame(gFieldEffectObjectPic_Bubbles, 2, 4, 1),
    overworld_frame(gFieldEffectObjectPic_Bubbles, 2, 4, 2),
    overworld_frame(gFieldEffectObjectPic_Bubbles, 2, 4, 3),
    overworld_frame(gFieldEffectObjectPic_Bubbles, 2, 4, 4),
    overworld_frame(gFieldEffectObjectPic_Bubbles, 2, 4, 5),
    overworld_frame(gFieldEffectObjectPic_Bubbles, 2, 4, 6),
    overworld_frame(gFieldEffectObjectPic_Bubbles, 2, 4, 7),
};

static const union AnimCmd sAnim_Bubbles[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_FRAME(2, 4),
    ANIMCMD_FRAME(3, 6),
    ANIMCMD_FRAME(4, 6),
    ANIMCMD_FRAME(5, 4),
    ANIMCMD_FRAME(6, 4),
    ANIMCMD_FRAME(7, 4),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_Bubbles[] =
{
    sAnim_Bubbles,
};

static const struct SpriteTemplate sSpriteTemplate_Bubbles =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_0,
    .oam = &gObjectEventBaseOam_16x32,
    .anims = sAnimTable_Bubbles,
    .images = sPicTable_Bubbles,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Bubbles,
};

static u32 FldEff_Bubbles(void)
{
    u32 spriteId;

    SetSpritePosToOffsetMapCoords((s16 *)&gFieldEffectArguments[0], (s16 *)&gFieldEffectArguments[1], 8, 0);
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect0);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_Bubbles, gFieldEffectArguments[0], gFieldEffectArguments[1], 0x52);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = 1;
    }
    return 0;
}

static void SpriteCB_Bubbles(struct Sprite * sprite)
{
    sprite->data[0] += 0x80;
    sprite->data[0] &= 0x100;
    
    sprite->y -= sprite->data[0] >> 8;
    
    UpdateObjectEventSpriteVisibility(sprite, FALSE);
    
    if (sprite->invisible || sprite->animEnded)
        FieldEffectStop(sprite, FLDEFF_BUBBLES);
}

////////////////////
// FLDEFF_SPARKLE //
////////////////////

static void SpriteCB_Sparkle(struct Sprite * sprite);

static const struct SpriteFrameImage sPicTable_SmallSparkle[] =
{
    overworld_frame(gFieldEffectObjectPic_SmallSparkle, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_SmallSparkle, 2, 2, 1),
};

static const union AnimCmd sAnim_SmallSparkle[] =
{
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_FRAME(1, 5),
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnimTable_SmallSparkle[] =
{
    sAnim_SmallSparkle,
};

static const struct SpriteTemplate sSpriteTemplate_SmallSparkle =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_2,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_SmallSparkle,
    .images = sPicTable_SmallSparkle,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Sparkle,
};

static u32 FldEff_Sparkle(void)
{
    u32 spriteId;

    gFieldEffectArguments[0] += 7;
    gFieldEffectArguments[1] += 7;
    SetSpritePosToOffsetMapCoords((s16 *)&gFieldEffectArguments[0], (s16 *)&gFieldEffectArguments[1], 8, 8);
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect2);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_SmallSparkle, gFieldEffectArguments[0], gFieldEffectArguments[1], 0x52);
    if (spriteId != MAX_SPRITES)
    {
        gSprites[spriteId].oam.priority = gFieldEffectArguments[2];
        gSprites[spriteId].coordOffsetEnabled = TRUE;
    }
    return 0;
}

static void SpriteCB_Sparkle(struct Sprite * sprite)
{
    if (sprite->data[0] == 0)
    {
        if (sprite->animEnded)
        {
            sprite->invisible = TRUE;
            sprite->data[0]++;
        }

        if (sprite->data[0] == 0)
            return;
    }

    if (++sprite->data[1] > 34)
        FieldEffectStop(sprite, FLDEFF_SPARKLE);
}

/////////////////////////////////////
// FLDEFF_FIELD_MOVE_SHOW_MON_INIT //
/////////////////////////////////////

static u32 FldEff_FieldMoveShowMonInit(void)
{
    u32 partyIdx = gFieldEffectArguments[0];
    bool32 noDucking = (gFieldEffectArguments[0] & SHOW_MON_CRY_NO_DUCKING);
    
    if (gUsingRideMon == RIDE_NONE)
    {
        gFieldEffectArguments[0] = GetMonData(&gPlayerParty[partyIdx], MON_DATA_SPECIES);
        gFieldEffectArguments[1] = GetMonData(&gPlayerParty[partyIdx], MON_DATA_IS_SHINY);
        gFieldEffectArguments[2] = GetMonData(&gPlayerParty[partyIdx], MON_DATA_PERSONALITY);
    }
    else
    {
        gFieldEffectArguments[0] = RideToSpeciesId(gUsingRideMon);
        gFieldEffectArguments[1] = FALSE;
        gFieldEffectArguments[2] = Random32();
        gUsingRideMon = RIDE_NONE;
    }
    gFieldEffectArguments[0] |= noDucking;
    FieldEffectStart(FLDEFF_FIELD_MOVE_SHOW_MON);
    FieldEffectActiveListRemove(FLDEFF_FIELD_MOVE_SHOW_MON_INIT);
    
    return 0;
}

////////////////////////////////
// FLDEFF_HALL_OF_FAME_RECORD //
////////////////////////////////

static void Task_HallOfFameRecord(u32 taskId);
static void HallOfFameRecordEffect_0(struct Task * task);
static void HallOfFameRecordEffect_1(struct Task * task);
static void HallOfFameRecordEffect_2(struct Task * task);
static void HallOfFameRecordEffect_3(struct Task * task);
static void SpriteCB_HallOfFameMonitor(struct Sprite *sprite);

static void (*const sHallOfFameRecordTaskCBTable[])(struct Task * ) = {
    HallOfFameRecordEffect_0,
    HallOfFameRecordEffect_1,
    HallOfFameRecordEffect_2,
    HallOfFameRecordEffect_3
};

static const union AnimCmd sUnknown_83CBF5C[] = {
    ANIMCMD_FRAME(3, 8),
    ANIMCMD_FRAME(2, 8),
    ANIMCMD_FRAME(1, 8),
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(1, 8),
    ANIMCMD_FRAME(2, 8),
    ANIMCMD_LOOP(2),
    ANIMCMD_FRAME(1, 8),
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_END
};

static const union AnimCmd *const sUnknown_83CBF84[] = {
    sUnknown_83CBF5C
};

static const struct SpriteFrameImage sUnknown_83CBEDC[] = {
    {sUnknown_83CB3F0 + 0x00, 0x80},
    {sUnknown_83CB3F0 + 0x40, 0x80},
    {sUnknown_83CB3F0 + 0x80, 0x80},
    {sUnknown_83CB3F0 + 0xC0, 0x80}
};

static const struct OamData sOamData_16x16 = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0x000,
    .priority = 0,
    .paletteNum = 0x0,
    .affineParam = 0
};

static const struct SpriteTemplate sUnknown_83CBFB8 = {
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_HALLOFFAME_MONITOR,
    .oam = &sOamData_16x16,
    .anims = sUnknown_83CBF84,
    .images = sUnknown_83CBEDC,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_HallOfFameMonitor
};

static const struct SpritePalette sUnknown_83CBEA4 = {
    sUnknown_83CB3D0, FLDEFF_PAL_TAG_HALLOFFAME_MONITOR
};

static u32 FldEff_HallOfFameRecord(void)
{
    struct Task *task;
    
    FieldEffect_LoadFadedPal(&sUnknown_83CBE9C);
    FieldEffect_LoadFadedPal(&sUnknown_83CBEA4);
    
    task = &gTasks[CreateTask(Task_HallOfFameRecord, 0xFF)];
    task->data[1] = CalculatePlayerPartyCount();
    task->data[2] = 0x75;
    task->data[3] = 0x3C;
    
    return 0;
}

static void Task_HallOfFameRecord(u32 taskId)
{
    sHallOfFameRecordTaskCBTable[gTasks[taskId].data[0]](&gTasks[taskId]);
}

static void HallOfFameRecordEffect_0(struct Task * task)
{
    task->data[0]++;
    task->data[6] = CreatePokeballGlowSprite(task->data[1], task->data[2], task->data[3], FALSE);
}

static void HallOfFameRecordEffect_1(struct Task * task)
{
    if (gSprites[task->data[6]].data[0] > 1)
    {
        CreateSpriteAtEnd(&sUnknown_83CBFB8, 0x78, 0x19, 0);
        task->data[15]++; // was this ever initialized? is this ever used?
        task->data[0]++;
    }
}

static void HallOfFameRecordEffect_2(struct Task * task)
{
    if (gSprites[task->data[6]].data[0] > 4)
        task->data[0]++;
}

static void HallOfFameRecordEffect_3(struct Task * task)
{
    if (gSprites[task->data[6]].data[0] > 6)
    {
        DestroySprite(&gSprites[task->data[6]]);
        FieldEffectActiveListRemove(FLDEFF_HALL_OF_FAME_RECORD);
        DestroyTask(FindTaskIdByFunc(Task_HallOfFameRecord));
    }
}

static void SpriteCB_HallOfFameMonitor(struct Sprite * sprite)
{
    if (sprite->animEnded)
        FieldEffectFreeGraphicsResources(sprite);
}

////////////////////////////
// FLDEFF_SNOW_FOOTPRINTS //
////////////////////////////

static const struct SpriteFrameImage sPicTable_SnowFootprints[] =
{
    overworld_frame(gFieldEffectObjectPic_SnowFootprints, 2, 2, 0),
    overworld_frame(gFieldEffectObjectPic_SnowFootprints, 2, 2, 1),
};

static const struct SpriteTemplate sSpriteTemplate_SnowFootprints =
{
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = FLDEFF_PAL_TAG_GENERAL_2,
    .oam = &gObjectEventBaseOam_16x16,
    .anims = sAnimTable_SandFootprints,
    .images = sPicTable_SnowFootprints,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_UpdateFootprintsTireTracks,
};

static u32 FldEff_SnowFootprints(void)
{
    u32 spriteId;

    SetSpritePosToOffsetMapCoords((s16 *)&gFieldEffectArguments[0], (s16 *)&gFieldEffectArguments[1], 8, 8);
    FieldEffect_LoadFadedPal(&gSpritePalette_GeneralFieldEffect2);
    
    spriteId = CreateSpriteAtEnd(&sSpriteTemplate_SnowFootprints, gFieldEffectArguments[0], gFieldEffectArguments[1], gFieldEffectArguments[2]);
    if (spriteId != MAX_SPRITES)
    {
        struct Sprite *sprite = &gSprites[spriteId];
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = gFieldEffectArguments[3];
        sprite->data[7] = FLDEFF_SNOW_FOOTPRINTS;
        StartSpriteAnim(sprite, gFieldEffectArguments[4]);
    }
    return 0;
}

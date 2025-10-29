#include "global.h"
#include "battle.h"
#include "battle_gimmicks.h"
#include "battle_interface.h"
#include "battle_message.h"
#include "battle_scripts.h"
#include "event_data.h"
#include "form_change.h"
#include "item.h"
#include "palette.h"
#include "pokedex.h"
#include "pokemon_icon.h"
#include "test_runner.h"
#include "util.h"
#include "constants/battle_string_ids.h"
#include "constants/hold_effects.h"
#include "constants/moves.h"
#include "constants/pokedex.h"

/////////////////////
// GIMMICKS SYSTEM //
/////////////////////

enum
{
    FLAG_GET_ACTIVATED,
    FLAG_SET_ACTIVATED
};

struct GimmickInfo
{
    const struct SpriteSheet *triggerSheet;
    const u16 *triggerPal;
    bool32 (*canActivate)(u32 battler);
    void (*activateGimmick)(u32 battler);
    u8 indicatorId;
};

static bool32 CanActivateGimmick(u32 battler, u32 gimmick);
static void SpriteCB_GimmickTrigger(struct Sprite *sprite);
static void SpriteCB_GimmickIndicator(struct Sprite *sprite);

#include "data/gimmicks/graphics.h"
#include "data/gimmicks/gimmicks_info.h"

// Populates usable gimmicks for each battler.
void AssignUsableGimmicks(void)
{
    u32 battler, gimmick;
    
    for (battler = 0; battler < gBattlersCount; battler++)
    {
        gBattleStruct->battlers[battler].usableGimmick = GIMMICK_NONE;
        
        if (!(gStatuses3[battler] & (STATUS3_SEMI_INVULNERABLE | STATUS3_COMMANDING)) && GetActiveGimmick(battler) == GIMMICK_NONE)
        {
            for (gimmick = 0; gimmick < GIMMICKS_COUNT; gimmick++)
            {
                if (!HasTrainerUsedGimmick(battler, gimmick) && CanActivateGimmick(battler, gimmick))
                {
                    gBattleStruct->battlers[battler].usableGimmick = gimmick;
                    break;
                }
            }
        }
    }
}

// Returns whether a battler is able to use a gimmick. Checks consumption and gimmick specific functions.
static bool32 CanActivateGimmick(u32 battler, u32 gimmick)
{
    return (sGimmicksInfo[gimmick].canActivate != NULL && sGimmicksInfo[gimmick].canActivate(battler));
}

// Returns whether the player has a gimmick selected while in the move selection menu.
bool32 IsGimmickSelected(u32 battler, u32 gimmick)
{
    if (gBattleStruct->battlers[battler].usableGimmick != gimmick)
        return FALSE;
    
    // There's no player select in tests, but some gimmicks need to test choice before they are fully activated.
    if (gTestRunnerEnabled)
        return gBattleStruct->battlers[battler].toActivateGimmick;
    else
        return gBattleStruct->playerSelectedGimmick;
}

// Sets a battler as having a gimmick active.
void SetActiveGimmick(u32 battler, u32 gimmick)
{
    gBattleStruct->sides[GetBattlerSide(battler)].party[gBattlerPartyIndexes[battler]].activeGimmick = gimmick;
}

// Removes a battler's active gimmick on fainting.
void RemoveActiveGimmick(u32 battler, u32 gimmick)
{
    if (gimmick != GIMMICK_NONE)
        SetActiveGimmick(battler, GIMMICK_NONE);
}

// Returns a battler's active gimmick, if any.
u32 GetActiveGimmick(u32 battler)
{
    return gBattleStruct->sides[GetBattlerSide(battler)].party[gBattlerPartyIndexes[battler]].activeGimmick;
}

// Returns whether the battler has a gimmick active or the player has a gimmick selected while in the move selection menu.
bool32 IsGimmickActiveOrSelected(u32 battler, u32 gimmick)
{
    return (GetActiveGimmick(battler) == gimmick || IsGimmickSelected(battler, gimmick));
}

// Gets or set where a battler has activated the gimmick.
static bool32 GetSetActivatedGimmick(u32 battlerId, u32 gimmick, u32 caseId)
{
    u32 index = gimmick / 8, mask = Bit(gimmick % 8);
    
    switch (caseId)
    {
        case FLAG_GET_ACTIVATED:
            return (gBattleStruct->battlers[battlerId].activatedGimmick[index] & mask);
        case FLAG_SET_ACTIVATED:
            gBattleStruct->battlers[battlerId].activatedGimmick[index] |= mask;
            break;
    }
    return FALSE;
}

// Returns whether a trainer has used a gimmick during a battle.
bool32 HasTrainerUsedGimmick(u32 battler, u32 gimmick)
{
    // Check whether partner battler has used gimmick or plans to during turn.
    if (IsDoubleBattleForBattler(battler) && IsPartnerMonFromSameTrainer(battler) && (GetSetActivatedGimmick(BATTLE_PARTNER(battler), gimmick, FLAG_GET_ACTIVATED)
    || (gBattleStruct->battlers[BATTLE_PARTNER(battler)].toActivateGimmick && gBattleStruct->battlers[BATTLE_PARTNER(battler)].usableGimmick == gimmick)))
        return TRUE;
    else // Otherwise, return whether current battler has used gimmick.
        return GetSetActivatedGimmick(battler, gimmick, FLAG_GET_ACTIVATED);
}

// Sets a gimmick as used by a trainer with checks for Multi Battles.
void SetGimmickAsActivated(u32 battler, u32 gimmick)
{
    GetSetActivatedGimmick(battler, gimmick, FLAG_SET_ACTIVATED);
    
    if (IsDoubleBattleForBattler(battler) && IsPartnerMonFromSameTrainer(battler))
        GetSetActivatedGimmick(BATTLE_PARTNER(battler), gimmick, FLAG_SET_ACTIVATED);
}

// Executes a gimmick's activation function.
void ActivateGimmick(u32 battler)
{
    u32 gimmick = gBattleStruct->battlers[battler].usableGimmick;
    
    if (sGimmicksInfo[gimmick].activateGimmick != NULL)
    {
        sGimmicksInfo[gimmick].activateGimmick(battler);
        
        SetActiveGimmick(battler, gimmick);
        SetGimmickAsActivated(battler, gimmick);
    }
}

// Returns whether a trainer has a gimmick key item on its bag.
static bool32 TrainerHasGimmickKeyItem(u32 battler, u16 itemId)
{
    if (!gTestRunnerEnabled)
    {
        u32 position = GetBattlerPosition(battler);
        
        if ((position == B_POSITION_PLAYER_LEFT || (position == B_POSITION_PLAYER_RIGHT && IsPartnerMonFromSameTrainer(battler))) && !CheckBagHasItem(itemId, 1))
            return FALSE;
    }
    return TRUE;
}

//////////////////////
// GIMMICKS TRIGGER //
//////////////////////

#define SINGLES_GIMMICK_TRIGGER_POS_X_OPTIMAL (39)
#define SINGLES_GIMMICK_TRIGGER_POS_X_PRIORITY (38)
#define SINGLES_GIMMICK_TRIGGER_POS_X_SLIDE (15)
#define SINGLES_GIMMICK_TRIGGER_POS_Y_DIFF (-1)

#define DOUBLES_GIMMICK_TRIGGER_POS_X_OPTIMAL (38)
#define DOUBLES_GIMMICK_TRIGGER_POS_X_PRIORITY (37)
#define DOUBLES_GIMMICK_TRIGGER_POS_X_SLIDE (15)
#define DOUBLES_GIMMICK_TRIGGER_POS_Y_DIFF (1)

#define sBattler   data[0]
#define sGimmick   data[1]
#define sHide      data[2]
#define sLightUp   data[3] // Light up on/off
#define sXSlide    data[4]
#define sXPriority data[5]
#define sXOptimal  data[6]
#define sYDiff     data[7]

// Creates the trigger sprite for player mons to select gimmick.
void CreateGimmickTriggerSprite(u32 battler)
{
    struct Sprite *sprite;
    u32 gimmick = gBattleStruct->battlers[battler].usableGimmick;
    bool32 isDoubleBattle;
    s32 xSlide, xPriority, xOptimal;
    s32 yDiff;
    
    if (gimmick != GIMMICK_NONE && GetBattlerSide(battler) == B_SIDE_PLAYER && sGimmicksInfo[gimmick].triggerSheet != NULL && !HasTrainerUsedGimmick(battler, gimmick))
    {
        // Load palette
        struct SpritePalette palette =
        {
            .data = sGimmicksInfo[gimmick].triggerPal,
            .tag = TAG_GIMMICK_TRIGGER_GFX,
        };
        LoadSpritePalette(&palette);
        
        // Load tiles
        if (GetSpriteTileStartByTag(TAG_GIMMICK_TRIGGER_GFX) == 0xFFFF)
            LoadSpriteSheet(sGimmicksInfo[gimmick].triggerSheet);
        
        // Get coords
        isDoubleBattle = IsDoubleBattleForBattler(battler);
        
        if (isDoubleBattle)
        {
            xSlide = DOUBLES_GIMMICK_TRIGGER_POS_X_SLIDE;
            xPriority = DOUBLES_GIMMICK_TRIGGER_POS_X_PRIORITY;
            xOptimal = DOUBLES_GIMMICK_TRIGGER_POS_X_OPTIMAL;
            yDiff = DOUBLES_GIMMICK_TRIGGER_POS_Y_DIFF;
        }
        else
        {
            xSlide = SINGLES_GIMMICK_TRIGGER_POS_X_SLIDE;
            xPriority = SINGLES_GIMMICK_TRIGGER_POS_X_PRIORITY;
            xOptimal = SINGLES_GIMMICK_TRIGGER_POS_X_OPTIMAL;
            yDiff = SINGLES_GIMMICK_TRIGGER_POS_Y_DIFF;
        }
        
        // Create sprite if not created yet
        if (gBattleStruct->gimmickTriggerSpriteId == 0xFF)
        {
            sprite = &gSprites[gHealthboxSpriteIds[battler]];
            gBattleStruct->gimmickTriggerSpriteId = CreateSprite(&sSpriteTemplate_GimmickTrigger, sprite->x - xSlide, sprite->y - yDiff, 0);
        }
        // Set sprite properties
        sprite = &gSprites[gBattleStruct->gimmickTriggerSpriteId];
        
        sprite->sBattler = battler;
        sprite->sGimmick = gimmick;
        sprite->sHide = FALSE;
        sprite->sLightUp = FALSE;
        sprite->sXSlide = xSlide;
        sprite->sXPriority = xPriority;
        sprite->sXOptimal = xOptimal;
        sprite->sYDiff = yDiff;
        
        StartSpriteAffineAnim(sprite, isDoubleBattle);
        ChangeGimmickTriggerSprite(gBattleStruct->gimmickTriggerSpriteId, FALSE);
    }
}

// Hides the player's gimmick trigger sprite.
void HideGimmickTriggerSprite(void)
{
    u32 triggerSpriteId = gBattleStruct->gimmickTriggerSpriteId;
    
    if (triggerSpriteId != 0xFF)
    {
        ChangeGimmickTriggerSprite(triggerSpriteId, FALSE);
        gSprites[triggerSpriteId].sHide = TRUE;
    }
}

// Destroy the player's gimmick trigger sprite.
static void DestroyGimmickTriggerSprite(void)
{
    FreeSpritePaletteByTag(TAG_GIMMICK_TRIGGER_GFX);
    FreeSpriteTilesByTag(TAG_GIMMICK_TRIGGER_GFX);
    
    if (gBattleStruct->gimmickTriggerSpriteId != 0xFF)
        DestroySprite(&gSprites[gBattleStruct->gimmickTriggerSpriteId]);
    
    gBattleStruct->gimmickTriggerSpriteId = 0xFF;
}

// Returns whether the player's gimmick trigger is active.
bool32 IsGimmickTriggerSpriteActive(void)
{
    if (GetSpriteTileStartByTag(TAG_GIMMICK_TRIGGER_GFX) == 0xFFFF || IndexOfSpritePaletteTag(TAG_GIMMICK_TRIGGER_GFX) == 0xFF)
        return FALSE;
    return TRUE;
}

#define TRIGGER_LIGHT_UP_ALPHA 20

static u32 LightUpGimmickTriggerPalette(u32 clra)
{
    u32 clrb = RGB_WHITE;
    u32 rbmask = RGB_MAGENTA, gmask = RGB_GREEN;
    u32 rbhalf = RGB(16, 0, 16), ghalf = RGB(0, 16, 0);
    
    // Red and blue
    u32 parta = clra & rbmask;
    u32 partb = clrb & rbmask;
    u32 part = (partb - parta) * (32 - TRIGGER_LIGHT_UP_ALPHA) + parta * 32 + rbhalf;
    u16 clr = (part >> 5) & rbmask;
    
    // Green
    parta = clra & gmask;
    partb = clrb & gmask;
    part = (partb - parta) * (32 - TRIGGER_LIGHT_UP_ALPHA) + parta * 32 + ghalf;
    clr |= (part >> 5) & gmask;
    
    return clr;
}

#define TRIGGER_LIGHT_UP_START_PAL_INDEX 4

// Add or remove light up on the specifics trigger colors.
void ChangeGimmickTriggerSprite(u32 triggerSpriteId, bool32 lightUp)
{
    u32 i;
    const u16 *triggerPal;
    u16 *dest;
    
    if (gSprites[triggerSpriteId].sLightUp != lightUp)
    {
        gSprites[triggerSpriteId].sLightUp = lightUp;
        
        triggerPal = sGimmicksInfo[gSprites[triggerSpriteId].sGimmick].triggerPal;
        dest = &gPlttBufferFaded[(IndexOfSpritePaletteTag(TAG_GIMMICK_TRIGGER_GFX) + 16) * 16];
        
        for (i = TRIGGER_LIGHT_UP_START_PAL_INDEX; i < 16; i++)
            dest[i] = lightUp ? LightUpGimmickTriggerPalette(triggerPal[i]) : triggerPal[i];
    }
}

static void SpriteCB_GimmickTrigger(struct Sprite *sprite)
{
    struct Sprite *healthboxSprite = &gSprites[gHealthboxSpriteIds[sprite->sBattler]];
    s16 x, xPriority = sprite->sXPriority, yDiff = sprite->sYDiff;
    
    if (sprite->sHide)
    {
        x = sprite->sXSlide;
        
        if (sprite->x < healthboxSprite->x - x)
            sprite->x++;

        if (sprite->x >= healthboxSprite->x - xPriority)
            sprite->oam.priority = 2;
        else
            sprite->oam.priority = 1;

        sprite->y = healthboxSprite->y - yDiff;
        sprite->y2 = healthboxSprite->y2 - yDiff;
        
        if (sprite->x == healthboxSprite->x - x)
            DestroyGimmickTriggerSprite();
    }
    else
    {
        if (sprite->y != healthboxSprite->y - yDiff)
            sprite->x = healthboxSprite->x - sprite->sXSlide;
        
        if (sprite->x > healthboxSprite->x - sprite->sXOptimal)
            sprite->x--;
        
        if (sprite->x >= healthboxSprite->x - xPriority)
            sprite->oam.priority = 2;
        else
            sprite->oam.priority = 1;
        
        sprite->y = healthboxSprite->y - yDiff;
        sprite->y2 = healthboxSprite->y2 - yDiff;
    }
}

#undef sBattler
#undef sGimmick
#undef sHide
#undef sLightUp
#undef sXSlide
#undef sXPriority
#undef sXOptimal
#undef sYDiff

////////////////////////
// GIMMICKS INDICATOR //
////////////////////////

// Updates current indicator sprite frame.
static void UpdateGimmickIndicatorSprite(struct Sprite *sprite, u32 indicatorId)
{
    SetPartyHPBarSprite(sprite, indicatorId);
    UpdateMonIconFrame(sprite);
}

#define sBattler     data[0]
#define sInvisible   data[1]
#define sPosX        data[2]
#define sPosY        data[3]
#define sLevelXDelta data[4] // X position depends whether level has 3, 2 or 1 digit

// Creates the gimmick indicator sprite for this battler.
u32 CreateGimmickIndicatorSprite(u32 battler)
{
    struct SpriteFrameImage image = { NULL, 0x20 };
    struct SpriteTemplate spriteTemplate =
    {
        .tileTag = SPRITE_INVALID_TAG,
        .paletteTag = TAG_GIMMICK_INDICATOR_GFX,
        .oam = &sOamData_GimmickIndicator,
        .anims = sGimmickIndicators,
        .images = &image,
        .affineAnims = gDummySpriteAffineAnimTable,
        .callback = SpriteCB_GimmickIndicator,
    };
    u32 spriteId = CreateSpriteAtEnd(&spriteTemplate, 0, 0, 0);
    u32 position = GetBattlerPosition(battler);
    
    gSprites[spriteId].sPosX = sIndicatorPositions[position][0];
    gSprites[spriteId].sPosY = sIndicatorPositions[position][1];
    gSprites[spriteId].sBattler = battler;
    gSprites[spriteId].invisible = TRUE;
    gSprites[spriteId].animPaused = TRUE;
    gSprites[spriteId].animBeginning = FALSE;
    gSprites[spriteId].images = (const struct SpriteFrameImage *)sGimmickIndicatorGfx;
    
    UpdateGimmickIndicatorSprite(&gSprites[spriteId], GetGimmickIndicatorId(battler));
    
    return spriteId;
}

// Update sLevelXDelta depending if the battler's level has 1, 2 or 3 chars.
void UpdateIndicatorLevelData(u32 indicatorSpriteId, u32 level)
{
    s16 xDelta;
    
    if (level >= 100)
        xDelta = -4;
    else if (level < 10)
        xDelta = 5;
    else
        xDelta = 0;
    
    gSprites[indicatorSpriteId].sLevelXDelta = xDelta;
}

// Sets the gimmick indicator to be hidden.
void SetGimmickIndicatorSpriteVisibility(u32 indicatorSpriteId, bool32 invisible)
{
    gSprites[indicatorSpriteId].sInvisible = invisible;
}

// Returns the battler's gimmick indicator id.
u32 GetGimmickIndicatorId(u32 battler)
{
    u32 gimmickIndicatorId = sGimmicksInfo[GetActiveGimmick(battler)].indicatorId;
    
    // If the battler's active gimmick has an indicator, displays it. Otherwise, try display a special gimmick indicator.
    // So, its possible to have a Dynamax icon over the Primal icon when active.
    // And return to the Primal icon when the Dynamax ends.
    if (gimmickIndicatorId != GIMMICK_INDICATOR_NONE)
        return gimmickIndicatorId;
    else
    {
        if (gSpeciesInfo[gBattleMons[battler].species].flags & SPECIES_FLAG_PRIMAL)
            return ItemId_GetHoldEffectParam(gBattleMons[battler].item);
        else if (IsBattlerTotemPokemon(battler))
            return GIMMICK_INDICATOR_TOTEM;
    }
    return GIMMICK_INDICATOR_NONE;
}

static void SpriteCB_GimmickIndicator(struct Sprite *sprite)
{
    u32 indicatorId, battler = sprite->sBattler;
    bool32 invisible = sprite->invisible;
    
    if (invisible != sprite->sInvisible && !gBattleStruct->battlers[battler].gimmickInProgress)
    {
        invisible ^= TRUE;
        
        indicatorId = GetGimmickIndicatorId(battler);
        
        if (!invisible)
            invisible = (indicatorId == GIMMICK_INDICATOR_NONE); // Only be visible if has an active gimmick indicator
        
        sprite->sInvisible = sprite->invisible = invisible;
        
        if (!invisible)
            UpdateGimmickIndicatorSprite(sprite, indicatorId);
    }
    sprite->x = gSprites[gHealthboxSpriteIds[battler]].x + sprite->sPosX + sprite->sLevelXDelta;
    sprite->x2 = gSprites[gHealthboxSpriteIds[battler]].x2;
    
    sprite->y = gSprites[gHealthboxSpriteIds[battler]].y + sprite->sPosY;
    sprite->y2 = gSprites[gHealthboxSpriteIds[battler]].y2;
}

#undef sBattler
#undef sInvisible
#undef sPosX
#undef sPosY
#undef sLevelXDelta

////////////////////
// MEGA EVOLUTION //
////////////////////

bool32 CanMegaEvolve(u32 battler)
{
    if (!TrainerHasGimmickKeyItem(battler, ITEM_MEGA_BRACELET))
        return FALSE;
    else if (GetBattlerItemHoldEffect(battler, FALSE) == HOLD_EFFECT_Z_CRYSTAL)
        return FALSE;
#if MEGA_EVO_FRIENDSHIP_THRESHOLD != 0
    else if (gBattleMons[battler].friendship < MEGA_EVO_FRIENDSHIP_THRESHOLD)
        return FALSE;
#endif
    else if (TryDoBattleFormChange(battler, FORM_CHANGE_MEGA_EVO))
        return TRUE;
    else if (TryDoBattleFormChange(battler, FORM_CHANGE_MOVE_MEGA_EVO))
        return TRUE;
    else
        return FALSE;
}

void ActivateMegaEvolution(u32 battler)
{
    u32 targetSpecies;
    
    if ((targetSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_MOVE_MEGA_EVO)))
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_FERVENT_WISH_REACHED;
    else
    {
        targetSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_MEGA_EVO);
        gLastUsedItem = gBattleMons[battler].item;
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_REACTING_TO_KEY_STONE;
    }
    PrepareSpeciesBuffer(gBattleTextBuff1, targetSpecies);
    DoBattleFormChange(battler, targetSpecies, TRUE, TRUE, TRUE);
    BattleScriptExecute(BattleScript_MegaEvolution);
}

/////////////////
// ULTRA BURST //
/////////////////

bool32 CanUltraBurst(u32 battler)
{
    if (!TrainerHasGimmickKeyItem(battler, ITEM_Z_RING))
        return FALSE;
    else if (TryDoBattleFormChange(battler, FORM_CHANGE_ULTRA_BURST))
        return TRUE;
    else
        return FALSE;
}

void ActivateUltraBurst(u32 battler)
{
    DoBattleFormChange(battler, TryDoBattleFormChange(battler, FORM_CHANGE_ULTRA_BURST), TRUE, TRUE, TRUE);
    BattleScriptExecute(BattleScript_UltraBurst);
}

////////////
// Z-MOVE //
////////////

bool32 IsZMove(u32 move)
{
    return (move >= FIRST_Z_MOVE && move <= LAST_Z_MOVE);
}

bool32 IsTypeBasedZMove(u32 move)
{
    return (move >= FIRST_Z_MOVE && move <= LAST_TYPED_Z_MOVE);
}

/////////////
// DYNAMAX //
/////////////

bool32 IsMaxMove(u32 move)
{
    return (move >= FIRST_GMAX_MOVE && move <= LAST_GMAX_MOVE);
}

//////////////////////
// TERASTALLIZATION //
//////////////////////

bool32 CanTerastallize(u32 battler)
{
    if (TryDoBattleFormChange(battler, FORM_CHANGE_MEGA_EVO) || TryDoBattleFormChange(battler, FORM_CHANGE_MOVE_MEGA_EVO))
        return FALSE;
    else if (GetBattlerItemHoldEffect(battler, FALSE) == HOLD_EFFECT_Z_CRYSTAL)
        return FALSE;
    else if ((gBattleMons[battler].status2 & STATUS2_TRANSFORMED) && SpeciesToNationalPokedexNum(gBattleMons[battler].species) == NATIONAL_DEX_TERAPAGOS)
        return FALSE;
    else if (!TrainerHasGimmickKeyItem(battler, ITEM_TERA_ORB))
        return FALSE;
    else if (!gTestRunnerEnabled && GetBattlerSide(battler) == B_SIDE_PLAYER && !FlagGet(FLAG_TERA_ORB_NO_COST) && !FlagGet(FLAG_TERA_ORB_CHARGED))
        return FALSE;
    else
        return TRUE;
}

void ActivateTera(u32 battler)
{
    // Remove Tera Orb charge
    if (!gTestRunnerEnabled && !FlagGet(FLAG_TERA_ORB_NO_COST) && GetBattlerSide(battler) == B_SIDE_PLAYER
    && !(IsDoubleBattleForBattler(battler) && !IsPartnerMonFromSameTrainer(battler)))
        FlagClear(FLAG_TERA_ORB_CHARGED);
    
    PrepareTypeBuffer(gBattleTextBuff1, GetBattlerTeraType(battler));
    BattleScriptExecute(BattleScript_Terastallization);
}

u32 GetBattlerTeraType(u32 battler)
{
    return GetMonTeraType(GetBattlerPartyIndexPtr(battler));
}

bool32 IsTypeStellarBoosted(u32 battler, u32 type)
{
    return !(gBattleStruct->sides[GetBattlerSide(battler)].stellarBoostFlags & Bit(type));
}

void TryExpendTypeStellarBoost(u32 battler, u32 type)
{
    // The boost isn't used up by Terapagos tera Stellar
    if (gBattleMons[battler].species != SPECIES_TERAPAGOS_STELLAR)
        gBattleStruct->sides[GetBattlerSide(battler)].stellarBoostFlags |= Bit(type);
}

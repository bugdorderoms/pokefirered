#include "global.h"
#include "battle.h"
#include "battle_gfx_sfx_util.h"
#include "battle_gimmicks.h"
#include "battle_interface.h"
#include "battle_message.h"
#include "battle_move_effects.h"
#include "battle_raid.h"
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
    bool32 (*keyItemCheck)(u32 battler);
    bool32 (*canActivate)(u32 battler);
    void (*activateGimmick)(u32 battler);
    const u8 *activationScript;
    u8 indicatorId;
    bool8 removesOnFaint:1;
    bool8 removesOnSwitchOut:1;
};

static bool32 CanActivateGimmick(u32 battler, u32 gimmick);
static void SpriteCB_GimmickTrigger(struct Sprite *sprite);
static void SpriteCB_GimmickIndicator(struct Sprite *sprite);

#include "data/gimmicks/graphics.h"
#include "data/gimmicks/gimmicks_info.h"

// Populate usable gimmick for this battler.
void AssignUsableGimmick(u32 battler)
{
    u32 gimmick;
    
    gBattleStruct->battlers[battler].usableGimmick = GIMMICK_NONE;
    
    if (!(gStatuses3[battler] & (STATUS3_SEMI_INVULNERABLE | STATUS3_COMMANDING)) && GetActiveGimmick(battler) == GIMMICK_NONE && CanActivateGimmickInRaid(battler))
    {
        for (gimmick = GIMMICK_NONE + 1; gimmick < GIMMICKS_COUNT; gimmick++)
        {
            if (!HasTrainerUsedGimmick(battler, gimmick) && CanActivateGimmick(battler, gimmick))
            {
                gBattleStruct->battlers[battler].usableGimmick = gimmick;
                break;
            }
        }
    }
    BtlController_EmitGimmickState(battler, BUFFER_A, STATE_USABLE_GIMMICK, gBattleStruct->battlers[battler].usableGimmick);
    MarkBattlerForControllerExec(battler);
}

// Returns whether a battler is able to use a gimmick. Checks consumption and gimmick specific functions.
static bool32 CanActivateGimmick(u32 battler, u32 gimmick)
{
    return (sGimmicksInfo[gimmick].canActivate != NULL && sGimmicksInfo[gimmick].canActivate(battler));
}

// Returns whether the player has a gimmick selected while in the move selection menu.
static bool32 IsGimmickSelected(u32 battler, u32 gimmick)
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

// Returns whether a battler's gimmick should be removed, if any.
bool32 ShouldRemoveActiveGimmick(u32 battler, u32 state)
{
    u32 gimmick = GetActiveGimmick(battler);
    
    if (gimmick != GIMMICK_NONE)
    {
        switch (state)
        {
            case REMOVE_GIMMICK_ON_FAINT:
                if (sGimmicksInfo[gimmick].removesOnFaint)
                    return TRUE;
                break;
            case REMOVE_GIMMICK_ON_SWITCHOUT:
                if (sGimmicksInfo[gimmick].removesOnSwitchOut)
                    return TRUE;
                break;
            case REMOVE_GIMMICK_ON_DYNAMAX_END:
                return TRUE;
        }
    }
    return FALSE;
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
static void SetGimmickAsActivated(u32 battler, u32 gimmick)
{
    GetSetActivatedGimmick(battler, gimmick, FLAG_SET_ACTIVATED);
    
    if (IsDoubleBattleForBattler(battler) && IsPartnerMonFromSameTrainer(battler))
        GetSetActivatedGimmick(BATTLE_PARTNER(battler), gimmick, FLAG_SET_ACTIVATED);
}

// Executes a gimmick's activation function.
void ActivateGimmick(u32 battler, u32 gimmick)
{
    if (sGimmicksInfo[gimmick].activateGimmick != NULL)
        sGimmicksInfo[gimmick].activateGimmick(battler);
    
    SetGimmickAsActivated(battler, gimmick);
}

// Returns the activation script of the gimmick.
const u8 *GetGimmickActivationScript(u32 gimmick)
{
    return sGimmicksInfo[gimmick].activationScript;
}

//////////////////////
// GIMMICK KEY ITEM //
//////////////////////

// Gets a bitfield with which Key Item this battler's trainer has to activate a gimmick.
void GetGimmicksKeyItemsBits(u32 battler, u8 *data)
{
    u32 gimmick;
    
    for (gimmick = GIMMICK_NONE + 1; gimmick < GIMMICKS_COUNT; gimmick++)
    {
        // Check if gimmick doesn't need an item or if has it on the bag to activate
        if (sGimmicksInfo[gimmick].keyItemCheck != NULL && sGimmicksInfo[gimmick].keyItemCheck(battler))
            data[gimmick / 8] |= Bit(gimmick % 8);
    }
}

// Returns whether the battler can activate the gimmick using its Key Item.
static bool32 CanActivateGimmickWithKeyItem(u32 battler, u32 gimmick)
{
    if (gTestRunnerEnabled)
        return (TestRunner_Battle_GetChosenGimmick(battler, gBattlerPartyIndexes[battler]) == gimmick);
    else
        return (gBattleStruct->battlers[battler].hasGimmickKeyItem[gimmick / 8] & Bit(gimmick % 8));
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
        u32 species = GetMonData(GetBattlerPartyIndexPtr(battler), MON_DATA_SPECIES);
        
        if (gSpeciesInfo[species].flags & SPECIES_FLAG_RED_PRIMAL)
            return GIMMICK_INDICATOR_OMEGA;
        else if (gSpeciesInfo[species].flags & SPECIES_FLAG_BLUE_PRIMAL)
            return GIMMICK_INDICATOR_ALPHA;
        else if (IsBattlerTotemPokemon(battler))
            return GIMMICK_INDICATOR_TOTEM;
    }
    return GIMMICK_INDICATOR_NONE;
}

static void SpriteCB_GimmickIndicator(struct Sprite *sprite)
{
    u32 indicatorId, battler = sprite->sBattler;
    bool32 invisible = sprite->invisible;
    
    if (invisible != sprite->sInvisible && !gBattleSpritesDataPtr->battlerData[battler].gimmickInProgress)
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

bool32 HasMegaBracelet(u32 battler)
{
    return CheckBagHasItem(ITEM_MEGA_BRACELET, 1);
}

bool32 CanMegaEvolve(u32 battler)
{
    if (!CanActivateGimmickWithKeyItem(battler, GIMMICK_MEGA))
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
    
    gBattleFormChangeData.species = targetSpecies;
    gBattleFormChangeData.reloadTypes = TRUE;
    gBattleFormChangeData.reloadStats = TRUE;
    gBattleFormChangeData.reloadAbility = TRUE;
}

/////////////////
// ULTRA BURST //
/////////////////

bool32 CanUltraBurst(u32 battler)
{
    if (!CanActivateGimmickWithKeyItem(battler, GIMMICK_ULTRA_BURST))
        return FALSE;
    else if (!TryDoBattleFormChange(battler, FORM_CHANGE_ULTRA_BURST))
        return FALSE;
    else
        return TRUE;
}

void ActivateUltraBurst(u32 battler)
{
    gBattleFormChangeData.species = TryDoBattleFormChange(battler, FORM_CHANGE_ULTRA_BURST);
    gBattleFormChangeData.reloadTypes = TRUE;
    gBattleFormChangeData.reloadStats = TRUE;
    gBattleFormChangeData.reloadAbility = TRUE;
}

////////////
// Z-MOVE //
////////////

bool32 HasZRing(u32 battler)
{
    return CheckBagHasItem(ITEM_Z_RING, 1);
}

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

// First value is for Fighting-type, Poison-type and Multi-Attack. The second is for everything else.
enum
{
    MAX_POWER_TIER_1, // 70 or 90 damage
    MAX_POWER_TIER_2, // 75 or 100 damage
    MAX_POWER_TIER_3, // 80 or 110 damage
    MAX_POWER_TIER_4, // 85 or 120 damage
    MAX_POWER_TIER_5, // 90 or 130 damage
    MAX_POWER_TIER_6, // 95 or 140 damage
    MAX_POWER_TIER_7, // 100 or 130 damage
    MAX_POWER_TIER_8, // 100 or 150 damage
};

struct
{
    u16 species;
    u8 moveType;
    u16 gmaxMove;
} static const sGMaxMoveTable[] =
{
    {SPECIES_VENUSAUR_GIGA,               TYPE_GRASS,      MOVE_GMAX_VINE_LASH},
    {SPECIES_BLASTOISE_GIGA,              TYPE_WATER,      MOVE_GMAX_CANNONADE},
    {SPECIES_CHARIZARD_GIGA,              TYPE_FIRE,       MOVE_GMAX_WILDFIRE},
    {SPECIES_BUTTERFREE_GIGA,             TYPE_BUG,        MOVE_GMAX_BEFUDDLE},
    {SPECIES_PIKACHU_GIGA,                TYPE_ELECTRIC,   MOVE_GMAX_VOLT_CRASH},
    {SPECIES_MEOWTH_GIGA,                 TYPE_NORMAL,     MOVE_GMAX_GOLD_RUSH},
    {SPECIES_MACHAMP_GIGA,                TYPE_FIGHTING,   MOVE_GMAX_CHI_STRIKE},
    {SPECIES_GENGAR_GIGA,                 TYPE_GHOST,      MOVE_GMAX_TERROR},
    {SPECIES_KINGLER_GIGA,                TYPE_WATER,      MOVE_GMAX_FOAM_BURST},
    {SPECIES_LAPRAS_GIGA,                 TYPE_ICE,        MOVE_GMAX_RESONANCE},
    {SPECIES_EEVEE_GIGA,                  TYPE_NORMAL,     MOVE_GMAX_CUDDLE},
    {SPECIES_SNORLAX_GIGA,                TYPE_NORMAL,     MOVE_GMAX_REPLENISH},
    {SPECIES_GARBODOR_GIGA,               TYPE_POISON,     MOVE_GMAX_MALODOR},
    {SPECIES_MELMETAL_GIGA,               TYPE_STEEL,      MOVE_GMAX_MELTDOWN},
    {SPECIES_RILLABOOM_GIGA,              TYPE_GRASS,      MOVE_GMAX_DRUM_SOLO},
    {SPECIES_CINDERACE_GIGA,              TYPE_FIRE,       MOVE_GMAX_FIREBALL},
    {SPECIES_INTELEON_GIGA,               TYPE_WATER,      MOVE_GMAX_HYDROSNIPE},
    {SPECIES_CORVIKNIGHT_GIGA,            TYPE_FLYING,     MOVE_GMAX_WIND_RAGE},
    {SPECIES_ORBEETLE_GIGA,               TYPE_PSYCHIC,    MOVE_GMAX_GRAVITAS},
    {SPECIES_DREDNAW_GIGA,                TYPE_WATER,      MOVE_GMAX_STONESURGE},
    {SPECIES_COALOSSAL_GIGA,              TYPE_ROCK,       MOVE_GMAX_VOLCALITH},
    {SPECIES_FLAPPLE_GIGA,                TYPE_GRASS,      MOVE_GMAX_TARTNESS},
    {SPECIES_APPLETUN_GIGA,               TYPE_GRASS,      MOVE_GMAX_SWEETNESS},
    {SPECIES_SANDACONDA_GIGA,             TYPE_GROUND,     MOVE_GMAX_SANDBLAST},
    {SPECIES_TOXTRICITY_GIGA,             TYPE_ELECTRIC,   MOVE_GMAX_STUN_SHOCK},
    {SPECIES_TOXTRICITY_LOW_KEY_GIGA,     TYPE_ELECTRIC,   MOVE_GMAX_STUN_SHOCK},
    {SPECIES_CENTISKORCH_GIGA,            TYPE_FIRE,       MOVE_GMAX_CENTIFERNO},
    {SPECIES_HATTERENE_GIGA,              TYPE_FAIRY,      MOVE_GMAX_SMITE},
    {SPECIES_GRIMMSNARL_GIGA,             TYPE_DARK,       MOVE_GMAX_SNOOZE},
    {SPECIES_ALCREMIE_GIGA,               TYPE_FAIRY,      MOVE_GMAX_FINALE},
    {SPECIES_COPPERAJAH_GIGA,             TYPE_STEEL,      MOVE_GMAX_STEELSURGE},
    {SPECIES_DURALUDON_GIGA,              TYPE_DRAGON,     MOVE_GMAX_DEPLETION},
    {SPECIES_URSHIFU_GIGA,                TYPE_DARK,       MOVE_GMAX_ONE_BLOW},
    {SPECIES_URSHIFU_RAPID_STRIKE_GIGA,   TYPE_WATER,      MOVE_GMAX_RAPID_FLOW},
};

bool32 HasDynamaxBand(u32 battler)
{
    if (GetBattlerSide(battler) == B_SIDE_PLAYER && !FlagGet(FLAG_DYNAMAX_ENABLED))
        return FALSE;
    else if (!CheckBagHasItem(ITEM_DYNAMAX_BAND, 1))
        return FALSE;
    else
        return TRUE;
}

bool32 CanDynamax(u32 battler)
{
    if (!CanActivateGimmickWithKeyItem(battler, GIMMICK_DYNAMAX))
        return FALSE;
    else if (TryDoBattleFormChange(battler, FORM_CHANGE_MEGA_EVO) || TryDoBattleFormChange(battler, FORM_CHANGE_MOVE_MEGA_EVO))
        return FALSE;
    else if (TryDoBattleFormChange(battler, FORM_CHANGE_PRIMAL) || TryDoBattleFormChange(battler, FORM_CHANGE_ULTRA_BURST))
        return FALSE;
    else if (GetBattlerItemHoldEffect(battler, FALSE) == HOLD_EFFECT_Z_CRYSTAL)
        return FALSE;
    else if (IsBannedSpeciesForDynamaxing(GetBattlerVisualSpecies(battler)))
        return FALSE;
    else
        return TRUE;
}

void ActivateDynamax(u32 battler)
{
    u32 targetSpecies;

    if (GetMonData(GetBattlerPartyIndexPtr(battler), MON_DATA_GIGANTAMAX_FACTOR) && (targetSpecies = TryDoBattleFormChange(battler, FORM_CHANGE_GIGANTAMAX)))
    {
        gBattleFormChangeData.species = targetSpecies;
        gBattleFormChangeData.reloadTypes = FALSE;
        gBattleFormChangeData.reloadStats = FALSE;
        gBattleFormChangeData.reloadAbility = FALSE;
        
        gBattleCommunication[MULTIUSE_STATE] = TRUE;
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_GIGANTAMAX;
    }
    else
    {
        gBattleCommunication[MULTIUSE_STATE] = FALSE;
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_DYNAMAX;
    }
    gBattleMons[battler].status2 &= ~(STATUS2_SUBSTITUTE);
    gDisableStructs[battler].substituteHP = 0;
    
    gBattleStruct->battlers[battler].choicedMove = MOVE_NONE;
    gBattleStruct->battlers[battler].dynamaxTurns = 3;
}

bool32 IsGigantamaxed(u32 battler)
{
    struct Pokemon *mon = GetBattlerPartyIndexPtr(battler);
    
    if (GetMonData(mon, MON_DATA_GIGANTAMAX_FACTOR) && IsGigantamaxSpecies(GetMonData(mon, MON_DATA_SPECIES)))
        return TRUE;
    
    return FALSE;
}

bool32 TryRevertGigantamax(u32 battler)
{
    if (IsGigantamaxed(battler))
    {
        gBattleFormChangeData.species = gBattleMonForms[GetBattlerSide(battler)][gBattlerPartyIndexes[battler]];
        gBattleFormChangeData.reloadTypes = FALSE;
        gBattleFormChangeData.reloadStats = FALSE;
        gBattleFormChangeData.reloadAbility = FALSE;
        return TRUE;
    }
    return FALSE;
}

static u32 GetBattlerDynamaxHPMultiplier(u32 battler, struct Pokemon *mon, bool32 inverseMultiplier)
{
    if (IsRaidBoss(battler))
        return GetRaidBossDynamaxHPMultiplier(inverseMultiplier);
    else
        return GetDynamaxLevelHPMultiplier(GetMonData(mon, MON_DATA_DYNAMAX_LEVEL), inverseMultiplier);
}

// Calculates the new HP of the battler after it Dynamaxes
bool32 CalcBattlerDynamaxHP(u32 battler, u16 *hp, u16 *maxHP, bool32 inverseMultiplier)
{
    struct Pokemon *mon = GetBattlerPartyIndexPtr(battler);
    
    if (GetMonData(mon, MON_DATA_SPECIES) != SPECIES_SHEDINJA)
    {
        u32 multiplier = GetBattlerDynamaxHPMultiplier(battler, mon, inverseMultiplier);
        
        if (hp != NULL)
            *hp = UQ_4_12_TO_INT((*hp * multiplier) + UQ_4_12_ROUND);
        
        if (maxHP != NULL)
            *maxHP = UQ_4_12_TO_INT((*maxHP * multiplier) + UQ_4_12_ROUND);
        
        return TRUE;
    }
    return FALSE;
}

// Returns the non-Dynamax HP of the battler
u32 GetNonDynamaxHP(u32 battler)
{
    u16 hp = gBattleMons[battler].hp;
    
    if (GetActiveGimmick(battler) == GIMMICK_DYNAMAX)
        CalcBattlerDynamaxHP(battler, &hp, NULL, TRUE);
    
    return hp;
}

// Returns the non-Dynamax Max HP of the battler
u32 GetNonDynamaxMaxHP(u32 battler)
{
    u16 maxHP = gBattleMons[battler].maxHP;
    
    if (GetActiveGimmick(battler) == GIMMICK_DYNAMAX)
        CalcBattlerDynamaxHP(battler, NULL, &maxHP, TRUE);
    
    return maxHP;
}

// Returns the appropriate Max Move or G-Max Move for a battler to use.
u32 GetMaxMove(u32 battler, u32 species, u32 baseMove, u32 moveType, u32 moveSplit)
{
    if (!baseMove || baseMove == MOVE_STRUGGLE)
        return baseMove;
    else if (moveSplit == SPLIT_STATUS)
        return MOVE_MAX_GUARD;
    else
        return GetTypeBasedMaxMove(battler, species, moveType);
}

// Returns the Max Move or G-Max Move of the given type.
u32 GetTypeBasedMaxMove(u32 battler, u32 species, u32 type)
{
    u32 i;
    
    // Try to Gigantamax species to get their respective G-Max move
    if (!IsGigantamaxSpecies(species))
    {
        struct Pokemon *mon = GetBattlerPartyIndexPtr(battler);
        
        if (GetMonData(mon, MON_DATA_GIGANTAMAX_FACTOR))
        {
            u32 gigantamaxSpecies = GetMonFormChangeSpecies(mon, species, FORM_CHANGE_GIGANTAMAX);
            
            if (gigantamaxSpecies)
                species = gigantamaxSpecies;
        }
    }

    if (IsGigantamaxSpecies(species))
    {
        for (i = 0; i < ARRAY_COUNT(sGMaxMoveTable); i++)
        {
            if (sGMaxMoveTable[i].species == species && sGMaxMoveTable[i].moveType == type)
                return sGMaxMoveTable[i].gmaxMove;
        }
    }
    return gTypesInfo[type].maxMove;
}

// Returns the Max Move power tier of this base move.
static u32 GetMaxMovePowerTier(u32 baseMove)
{
    if (gBattleMoves[baseMove].strikeCount >= 2 && gBattleMoves[baseMove].strikeCount <= 5)
    {
        switch (gBattleMoves[baseMove].power)
        {
            case 0 ... 25:
                return MAX_POWER_TIER_2;
            case 26 ... 30:
                return MAX_POWER_TIER_3;
            case 31 ... 35:
                return MAX_POWER_TIER_4;
            case 36 ... 50:
                return MAX_POWER_TIER_5;
            default:
                return MAX_POWER_TIER_6;
        }
    }
    
    switch (gBattleMoves[baseMove].effect)
    {
        case EFFECT_BIDE:
        case EFFECT_SUPER_FANG:
        case EFFECT_USER_LEVEL_TO_DAMAGE:
        case EFFECT_PSYWAVE:
        case EFFECT_COUNTER_ATTACK:
        case EFFECT_PRESENT:
        case EFFECT_BEAT_UP:
        case EFFECT_WEATHER_BALL:
        case EFFECT_FLING:
        case EFFECT_ELECTRO_BALL:
        case EFFECT_TERRAIN_PULSE:
        case EFFECT_PUNISHMENT:
        case EFFECT_TRUMP_CARD:
        case EFFECT_FIXED_DAMAGE:
        case EFFECT_SPIT_UP:
        case EFFECT_NATURAL_GIFT:
        case EFFECT_FINAL_GAMBIT:
            return MAX_POWER_TIER_2;
        case EFFECT_OHKO:
        case EFFECT_RETURN:
        case EFFECT_FRUSTRATION:
        case EFFECT_HEAT_CRASH:
        case EFFECT_STORED_POWER:
        case EFFECT_GYRO_BALL:
            return MAX_POWER_TIER_5;
        case EFFECT_MAGNITUDE:
        case EFFECT_WRING_OUT:
            return MAX_POWER_TIER_6;
        case EFFECT_FLAIL:
        case EFFECT_DAMAGE_BASED_TARGET_WEIGHT:
            return MAX_POWER_TIER_7;
        case EFFECT_MULTI_HIT:
            switch (gBattleMoves[baseMove].power)
            {
                case 0 ... 15:
                    return MAX_POWER_TIER_1;
                case 16 ... 18:
                    return MAX_POWER_TIER_2;
                case 19 ... 20:
                    return MAX_POWER_TIER_4;
                default:
                    return MAX_POWER_TIER_5;
            }
            break;
    }
    
    switch (gBattleMoves[baseMove].power)
    {
        case 0 ... 40:
            return MAX_POWER_TIER_1;
        case 45 ... 50:
            return MAX_POWER_TIER_2;
        case 55 ... 60:
            return MAX_POWER_TIER_3;
        case 65 ... 70:
            return MAX_POWER_TIER_4;
        case 75 ... 100:
            return MAX_POWER_TIER_5;
        case 110 ... 140:
            return MAX_POWER_TIER_6;
        default:
            return MAX_POWER_TIER_8;
    }
}

// Return the power of this Max Move.
u32 GetMaxMovePower(u32 baseMove, u32 maxMove, u32 moveType)
{
    u32 powerTier;
    
    // G-Max Drum Solo, G-Max Hydrosnipe, and G-Max Fireball have fixed base powers.
    if (gBattleMoves[maxMove].power > 1)
        return gBattleMoves[maxMove].power;
    
    // Exceptions to all other rules below:
    switch (baseMove)
    {
        case MOVE_TRIPLE_KICK:
            return 80;
        case MOVE_GEAR_GRIND:
            return 100;
        case MOVE_DUAL_WINGBEAT:
            return 100;
        case MOVE_TRIPLE_AXEL:
            return 140;
    }
    powerTier = GetMaxMovePowerTier(baseMove);
    
    if (moveType == TYPE_FIGHTING || moveType == TYPE_POISON || baseMove == MOVE_MULTI_ATTACK)
    {
        switch (powerTier)
        {
            case MAX_POWER_TIER_1:
                return 70;
            case MAX_POWER_TIER_2:
                return 75;
            case MAX_POWER_TIER_3:
                return 80;
            case MAX_POWER_TIER_4:
                return 85;
            case MAX_POWER_TIER_5:
                return 90;
            case MAX_POWER_TIER_6:
                return 95;
            case MAX_POWER_TIER_7:
                return 100;
            case MAX_POWER_TIER_8:
                return 100;
        }
    }
    else
    {
        switch (powerTier)
        {
            case MAX_POWER_TIER_1:
                return 90;
            case MAX_POWER_TIER_2:
                return 100;
            case MAX_POWER_TIER_3:
                return 110;
            case MAX_POWER_TIER_4:
                return 120;
            case MAX_POWER_TIER_5:
                return 130;
            case MAX_POWER_TIER_6:
                return 140;
            case MAX_POWER_TIER_7:
                return 130;
            case MAX_POWER_TIER_8:
                return 150;
        }
    }
}

// Returns whether the move is a Max Move or G-Max Move.
bool32 IsMaxMove(u32 move)
{
    return (move >= FIRST_MAX_MOVE && move <= LAST_GMAX_MOVE);
}

bool32 IsMoveBlockedByDynamax(u32 move)
{
    switch (gBattleMoves[move].effect)
    {
        case EFFECT_AUTOTOMIZE:
        case EFFECT_DAMAGE_BASED_TARGET_WEIGHT:
        case EFFECT_HEAT_CRASH:
        case EFFECT_SKY_DROP:
        case EFFECT_ENCORE:
        case EFFECT_TORMENT:
        case EFFECT_ENTRAINMENT:
        case EFFECT_DESTINY_BOND:
        case EFFECT_INSTRUCT:
        // Those two are handled separated
        // case EFFECT_RANDOM_SWITCH:
        // case EFFECT_OHKO:
            return TRUE;
    }
    return FALSE;
}

//////////////////////
// TERASTALLIZATION //
//////////////////////

bool32 HasTeraOrb(u32 battler)
{
    if (!CheckBagHasItem(ITEM_TERA_ORB, 1))
        return FALSE;
    else if (GetBattlerSide(battler) == B_SIDE_PLAYER && !FlagGet(FLAG_TERA_ORB_NO_COST) && !FlagGet(FLAG_TERA_ORB_CHARGED))
        return FALSE;
    else
        return TRUE;
}

bool32 CanTerastallize(u32 battler)
{
    if (TryDoBattleFormChange(battler, FORM_CHANGE_MEGA_EVO) || TryDoBattleFormChange(battler, FORM_CHANGE_MOVE_MEGA_EVO))
        return FALSE;
    else if (GetBattlerItemHoldEffect(battler, FALSE) == HOLD_EFFECT_Z_CRYSTAL)
        return FALSE;
    else if ((gBattleMons[battler].status2 & STATUS2_TRANSFORMED) && SpeciesToNationalPokedexNum(gBattleMons[battler].species) == NATIONAL_DEX_TERAPAGOS)
        return FALSE;
    else if (!CanActivateGimmickWithKeyItem(battler, GIMMICK_TERA))
        return FALSE;
    else
        return TRUE;
}

void ActivateTera(u32 battler)
{
    // Remove Tera Orb charge
    if (!gTestRunnerEnabled && !FlagGet(FLAG_TERA_ORB_NO_COST) && GetBattlerSide(battler) == B_SIDE_PLAYER && !(gBattleTypeFlags & BATTLE_TYPE_LINK)
    && !(IsDoubleBattleForBattler(battler) && !IsPartnerMonFromSameTrainer(battler)))
        FlagClear(FLAG_TERA_ORB_CHARGED);
    
    PrepareTypeBuffer(gBattleTextBuff1, GetBattlerTeraType(battler));
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

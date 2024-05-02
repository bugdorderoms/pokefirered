#include "global.h"
#include "gflib.h"
#include "battle_anim.h"
#include "event_object_movement.h"
#include "field_weather.h"
#include "field_weather_effects.h"
#include "graphics.h"
#include "random.h"
#include "script.h"
#include "constants/weather.h"
#include "constants/songs.h"
#include "task.h"
#include "trig.h"

const struct SpritePalette sSandstormSpritePalette = {gSandstormWeatherPalette, GFXTAG_SANDSTORM};
const struct SpritePalette sCloudsSpritePalette    = {gCloudWeatherPalette, GFXTAG_CLOUD};

//------------------------------------------------------------------------------
// WEATHER_RAIN
//------------------------------------------------------------------------------

static void LoadRainSpriteSheet(void);
static bool8 CreateRainSprite(void);
static void UpdateRainSprite(struct Sprite *sprite);
static bool8 UpdateVisibleRainSprites(void);
static void DestroyRainSprites(void);

static const struct Coords16 sRainSpriteCoords[] = {
    {  0,   0},
    {  0, 160},
    {  0,  64},
    {144, 224},
    {144, 128},
    { 32,  32},
    { 32, 192},
    { 32,  96},
    { 72, 128},
    { 72,  32},
    { 72, 192},
    {216,  96},
    {216,   0},
    {104, 160},
    {104,  64},
    {104, 224},
    {144,   0},
    {144, 160},
    {144,  64},
    { 32, 224},
    { 32, 128},
    { 72,  32},
    { 72, 192},
    { 48,  96},
};

static const struct OamData sRainSpriteOamData = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(16x32),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 2,
    .affineParam = 0,
};

static const union AnimCmd sRainSpriteFallAnimCmd[] = {
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sRainSpriteSplashAnimCmd[] = {
    ANIMCMD_FRAME(8, 3),
    ANIMCMD_FRAME(32, 2),
    ANIMCMD_FRAME(40, 2),
    ANIMCMD_END,
};

static const union AnimCmd sRainSpriteHeavySplashAnimCmd[] = {
    ANIMCMD_FRAME(8, 3),
    ANIMCMD_FRAME(16, 3),
    ANIMCMD_FRAME(24, 4),
    ANIMCMD_END,
};

static const union AnimCmd *const sRainSpriteAnimCmds[] = {
    sRainSpriteFallAnimCmd,
    sRainSpriteSplashAnimCmd,
    sRainSpriteHeavySplashAnimCmd,
};

static const struct SpriteTemplate sRainSpriteTemplate = {
    .tileTag = GFXTAG_RAIN,
    .paletteTag = PALTAG_WEATHER,
    .oam = &sRainSpriteOamData,
    .anims = sRainSpriteAnimCmds,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = UpdateRainSprite,
};

// Q28.4 fixed-point format values
static const s16 sRainSpriteMovement[][2] = {
    {-0x68,  0xD0},
    {-0xA0, 0x140},
};

// First byte is the number of frames a raindrop falls before it splashes.
// Second byte is the maximum number of frames a raindrop can "wait" before
// it appears and starts falling. (This is only for the initial raindrop spawn.)
static const u16 sRainSpriteFallingDurations[][2] = {
    {18, 7},
    {12, 10},
};

static const struct SpriteSheet sRainSpriteSheet = {
    .data = gWeatherRainTiles,
    .size = 0x0600,
    .tag = GFXTAG_RAIN,
};

void Rain_InitVars(void)
{
    gWeatherPtr->initStep = 0;
    gWeatherPtr->weatherGfxLoaded = FALSE;
    gWeatherPtr->rainSpriteVisibleCounter = 0;
    gWeatherPtr->rainSpriteVisibleDelay = 8;
    gWeatherPtr->isDownpour = FALSE;
    gWeatherPtr->targetRainSpriteCount = 10;
    gWeatherPtr->gammaTargetIndex = 3;
    gWeatherPtr->gammaStepDelay = 20;
    SetRainStrengthFromSoundEffect(SE_RAIN);
}

void Rain_InitAll(void)
{
    Rain_InitVars();
    while (!gWeatherPtr->weatherGfxLoaded)
        Rain_Main();
}

void Rain_Main(void)
{
    switch (gWeatherPtr->initStep)
    {
    case 0:
        LoadRainSpriteSheet();
        gWeatherPtr->initStep++;
        break;
    case 1:
        if (!CreateRainSprite())
            gWeatherPtr->initStep++;
        break;
    case 2:
        if (!UpdateVisibleRainSprites())
        {
            gWeatherPtr->weatherGfxLoaded = TRUE;
            gWeatherPtr->initStep++;
        }
        break;
    }
}

bool8 Rain_Finish(void)
{
    switch (gWeatherPtr->finishStep)
    {
    case 0:
        if (gWeatherPtr->nextWeather == WEATHER_RAIN || gWeatherPtr->nextWeather == WEATHER_RAIN_THUNDERSTORM || gWeatherPtr->nextWeather == WEATHER_DOWNPOUR)
        {
            gWeatherPtr->finishStep = 0xFF;
            return FALSE;
        }
        else
        {
            gWeatherPtr->targetRainSpriteCount = 0;
            gWeatherPtr->finishStep++;
        }
        // fall through
    case 1:
        if (!UpdateVisibleRainSprites())
        {
            DestroyRainSprites();
            gWeatherPtr->finishStep++;
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

#define tCounter data[0]
#define tRandom  data[1]
#define tPosX    data[2]
#define tPosY    data[3]
#define tState   data[4]
#define tActive  data[5]
#define tWaiting data[6]

static void StartRainSpriteFall(struct Sprite *sprite)
{
    u16 numFallingFrames;
    int tileX, tileY;

    if (sprite->tRandom == 0)
        sprite->tRandom = 361;

    // Standard RNG sequence.
    sprite->tRandom = ((ISO_RANDOMIZE2(sprite->tRandom) & 0x7FFF0000) >> 16) % 600;

    numFallingFrames = sRainSpriteFallingDurations[gWeatherPtr->isDownpour][0];

    tileX = sprite->tRandom % 30;
    tileY = sprite->tRandom / 30;

    sprite->tPosX = tileX;
    sprite->tPosX <<= 7; // This is tileX * 8, using a fixed-point value with 4 decimal places

    sprite->tPosY = tileY;
    sprite->tPosY <<= 7; // This is tileX * 8, using a fixed-point value with 4 decimal places

    // "Rewind" the rain sprites, from their ending position.
    sprite->tPosX -= sRainSpriteMovement[gWeatherPtr->isDownpour][0] * numFallingFrames;
    sprite->tPosY -= sRainSpriteMovement[gWeatherPtr->isDownpour][1] * numFallingFrames;

    StartSpriteAnim(sprite, 0);
    sprite->tState = 0;
    sprite->coordOffsetEnabled = FALSE;
    sprite->tCounter = numFallingFrames;
}

static void UpdateRainSprite(struct Sprite *sprite)
{
    if (!sprite->tState)
    {
        // Raindrop is in its "falling" motion.
        sprite->tPosX += sRainSpriteMovement[gWeatherPtr->isDownpour][0];
        sprite->tPosY += sRainSpriteMovement[gWeatherPtr->isDownpour][1];
        sprite->x = sprite->tPosX >> 4;
        sprite->y = sprite->tPosY >> 4;

        if (sprite->tActive && (sprite->x >= -8 && sprite->x <= 248) && sprite->y >= -16 && sprite->y <= 176)
            sprite->invisible = FALSE;
        else
            sprite->invisible = TRUE;

        if (--sprite->tCounter == 0)
        {
            // Make raindrop splash on the ground
            StartSpriteAnim(sprite, gWeatherPtr->isDownpour + 1);
            sprite->tState = 1;
            sprite->x -= gSpriteCoordOffsetX;
            sprite->y -= gSpriteCoordOffsetY;
            sprite->coordOffsetEnabled = TRUE;
        }
    }
    else if (sprite->animEnded)
    {
        // The splashing animation ended.
        sprite->invisible = TRUE;
        StartRainSpriteFall(sprite);
    }
}

static void WaitRainSprite(struct Sprite *sprite)
{
    if (!sprite->tCounter)
    {
        StartRainSpriteFall(sprite);
        sprite->callback = UpdateRainSprite;
    }
    else
        sprite->tCounter--;
}

static void InitRainSpriteMovement(struct Sprite *sprite, u16 val)
{
    u16 numFallingFrames = sRainSpriteFallingDurations[gWeatherPtr->isDownpour][0];
    u16 numAdvanceRng = val / (sRainSpriteFallingDurations[gWeatherPtr->isDownpour][1] + numFallingFrames);
    u16 frameVal = val % (sRainSpriteFallingDurations[gWeatherPtr->isDownpour][1] + numFallingFrames);

    while (--numAdvanceRng != 0xFFFF)
        StartRainSpriteFall(sprite);

    if (frameVal < numFallingFrames)
    {
        while (--frameVal != 0xFFFF)
            UpdateRainSprite(sprite);

        sprite->tWaiting = FALSE;
    }
    else
    {
        sprite->tCounter = frameVal - numFallingFrames;
        sprite->invisible = TRUE;
        sprite->tWaiting = TRUE;
    }
}

static void LoadRainSpriteSheet(void)
{
    LoadSpriteSheet(&sRainSpriteSheet);
}

static bool8 CreateRainSprite(void)
{
	u16 i;
    u8 spriteIndex, spriteId;

    if (gWeatherPtr->rainSpriteCount == NUM_RAIN_SPRITES)
        return FALSE;

    spriteIndex = gWeatherPtr->rainSpriteCount;
    spriteId = CreateSpriteAtEnd(&sRainSpriteTemplate, sRainSpriteCoords[spriteIndex].x, sRainSpriteCoords[spriteIndex].y, 78);

    if (spriteId != MAX_SPRITES)
    {
        gSprites[spriteId].tActive = 0;
        gSprites[spriteId].tRandom = spriteIndex * 145;
        while (gSprites[spriteId].tRandom >= 600)
            gSprites[spriteId].tRandom -= 600;

        StartRainSpriteFall(&gSprites[spriteId]);
        InitRainSpriteMovement(&gSprites[spriteId], spriteIndex * 9);
        gSprites[spriteId].invisible = TRUE;
        gWeatherPtr->rainSprites[spriteIndex] = &gSprites[spriteId];
    }
    else
        gWeatherPtr->rainSprites[spriteIndex] = NULL;

    if (++gWeatherPtr->rainSpriteCount == NUM_RAIN_SPRITES)
    {
        for (i = 0; i < NUM_RAIN_SPRITES; i++)
        {
            if (gWeatherPtr->rainSprites[i])
            {
                if (!gWeatherPtr->rainSprites[i]->tWaiting)
                    gWeatherPtr->rainSprites[i]->callback = UpdateRainSprite;
                else
                    gWeatherPtr->rainSprites[i]->callback = WaitRainSprite;
            }
        }
        return FALSE;
    }
    return TRUE;
}

static bool8 UpdateVisibleRainSprites(void)
{
    if (gWeatherPtr->curRainSpriteIndex == gWeatherPtr->targetRainSpriteCount)
        return FALSE;

    if (++gWeatherPtr->rainSpriteVisibleCounter > gWeatherPtr->rainSpriteVisibleDelay)
    {
        gWeatherPtr->rainSpriteVisibleCounter = 0;
        if (gWeatherPtr->curRainSpriteIndex < gWeatherPtr->targetRainSpriteCount)
			
            gWeatherPtr->rainSprites[gWeatherPtr->curRainSpriteIndex++]->tActive = 1;
        else
        {
            gWeatherPtr->curRainSpriteIndex--;
            gWeatherPtr->rainSprites[gWeatherPtr->curRainSpriteIndex]->tActive = 0;
            gWeatherPtr->rainSprites[gWeatherPtr->curRainSpriteIndex]->invisible = TRUE;
        }
    }
    return TRUE;
}

static void DestroyRainSprites(void)
{
    u16 i;

    for (i = 0; i < gWeatherPtr->rainSpriteCount; i++)
    {
        if (gWeatherPtr->rainSprites[i] != NULL)
            DestroySprite(gWeatherPtr->rainSprites[i]);
    }
    gWeatherPtr->rainSpriteCount = 0;
    FreeSpriteTilesByTag(GFXTAG_RAIN);
}

#undef tCounter
#undef tRandom
#undef tPosX
#undef tPosY
#undef tState
#undef tActive
#undef tWaiting

//------------------------------------------------------------------------------
// WEATHER_SNOW
//------------------------------------------------------------------------------

static void UpdateSnowflakeSprite(struct Sprite *);
static bool8 UpdateVisibleSnowflakeSprites(void);
static bool8 CreateSnowflakeSprite(void);
static bool8 DestroySnowflakeSprite(void);
static void InitSnowflakeSpriteMovement(struct Sprite *);

static const struct OamData sSnowflakeSpriteOamData = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const struct SpriteFrameImage sSnowflakeSpriteImages[] = {
    {gWeatherSnow1Tiles, 0x20},
    {gWeatherSnow2Tiles, 0x20},
};

static const union AnimCmd sSnowflakeAnimCmd0[] = {
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_END,
};

static const union AnimCmd sSnowflakeAnimCmd1[] = {
    ANIMCMD_FRAME(1, 16),
    ANIMCMD_END,
};

static const union AnimCmd *const sSnowflakeAnimCmds[] = {
    sSnowflakeAnimCmd0,
    sSnowflakeAnimCmd1,
};

static const struct SpriteTemplate sSnowflakeSpriteTemplate = {
    .tileTag = SPRITE_INVALID_TAG,
    .paletteTag = PALTAG_WEATHER,
    .oam = &sSnowflakeSpriteOamData,
    .anims = sSnowflakeAnimCmds,
    .images = sSnowflakeSpriteImages,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = UpdateSnowflakeSprite,
};

void Snow_InitVars(void)
{
    gWeatherPtr->initStep = 0;
    gWeatherPtr->weatherGfxLoaded = FALSE;
    gWeatherPtr->gammaTargetIndex = 3;
    gWeatherPtr->gammaStepDelay = 20;
    gWeatherPtr->targetSnowflakeSpriteCount = 16;
    gWeatherPtr->snowflakeVisibleCounter = 0;
}

void Snow_InitAll(void)
{
    u16 i;

    Snow_InitVars();
    while (!gWeatherPtr->weatherGfxLoaded)
    {
        Snow_Main();
        for (i = 0; i < gWeatherPtr->snowflakeSpriteCount; i++)
            UpdateSnowflakeSprite(gWeatherPtr->snowflakeSprites[i]);
    }
}

void Snow_Main(void)
{
    if (gWeatherPtr->initStep == 0 && !UpdateVisibleSnowflakeSprites())
    {
        gWeatherPtr->weatherGfxLoaded = TRUE;
        gWeatherPtr->initStep++;
    }
}

bool8 Snow_Finish(void)
{
    switch (gWeatherPtr->finishStep)
    {
    case 0:
        gWeatherPtr->targetSnowflakeSpriteCount = 0;
        gWeatherPtr->snowflakeVisibleCounter = 0;
        gWeatherPtr->finishStep++;
        // fall through
    case 1:
        if (!UpdateVisibleSnowflakeSprites())
        {
            gWeatherPtr->finishStep++;
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

#define tPosY         data[0]
#define tDeltaY       data[1]
#define tWaveDelta    data[2]
#define tWaveIndex    data[3]
#define tSnowflakeId  data[4]
#define tFallCounter  data[5]
#define tFallDuration data[6]
#define tDeltaY2      data[7]

static bool8 UpdateVisibleSnowflakeSprites(void)
{
    if (gWeatherPtr->snowflakeSpriteCount == gWeatherPtr->targetSnowflakeSpriteCount)
        return FALSE;

    if (++gWeatherPtr->snowflakeVisibleCounter > 36)
    {
        gWeatherPtr->snowflakeVisibleCounter = 0;
        if (gWeatherPtr->snowflakeSpriteCount < gWeatherPtr->targetSnowflakeSpriteCount)
            CreateSnowflakeSprite();
        else
            DestroySnowflakeSprite();
    }
    return gWeatherPtr->snowflakeSpriteCount != gWeatherPtr->targetSnowflakeSpriteCount;
}

static bool8 CreateSnowflakeSprite(void)
{
    u8 spriteId = CreateSpriteAtEnd(&sSnowflakeSpriteTemplate, 0, 0, 78);
	
    if (spriteId == MAX_SPRITES)
        return FALSE;

    gSprites[spriteId].tSnowflakeId = gWeatherPtr->snowflakeSpriteCount;
    InitSnowflakeSpriteMovement(&gSprites[spriteId]);
    gSprites[spriteId].coordOffsetEnabled = TRUE;
    gWeatherPtr->snowflakeSprites[gWeatherPtr->snowflakeSpriteCount++] = &gSprites[spriteId];
    return TRUE;
}

static bool8 DestroySnowflakeSprite(void)
{
    if (gWeatherPtr->snowflakeSpriteCount)
    {
        DestroySprite(gWeatherPtr->snowflakeSprites[--gWeatherPtr->snowflakeSpriteCount]);
        return TRUE;
    }
    return FALSE;
}

static void InitSnowflakeSpriteMovement(struct Sprite *sprite)
{
    u16 rand, x = ((sprite->tSnowflakeId * 5) & 7) * 30 + (Random() % 30);

    sprite->y = -3 - (gSpriteCoordOffsetY + sprite->centerToCornerVecY);
    sprite->x = x - (gSpriteCoordOffsetX + sprite->centerToCornerVecX);
    sprite->tPosY = sprite->y * 128;
    sprite->x2 = 0;
    rand = Random();
    sprite->tDeltaY = (rand & 3) * 5 + 64;
    sprite->tDeltaY2 = sprite->tDeltaY;
    StartSpriteAnim(sprite, (rand & 1) ? 0 : 1);
    sprite->tWaveIndex = 0;
    sprite->tWaveDelta = ((rand & 3) == 0) ? 2 : 1;
    sprite->tFallDuration = (rand & 0x1F) + 210;
    sprite->tFallCounter = 0;
}

static void WaitSnowflakeSprite(struct Sprite *sprite)
{
    if (++gWeatherPtr->snowflakeTimer > 18)
    {
        sprite->invisible = FALSE;
        sprite->callback = UpdateSnowflakeSprite;
        sprite->y = 250 - (gSpriteCoordOffsetY + sprite->centerToCornerVecY);
        sprite->tPosY = sprite->y * 128;
        gWeatherPtr->snowflakeTimer = 0;
    }
}

static void UpdateSnowflakeSprite(struct Sprite *sprite)
{
    s16 x, y;

    sprite->tPosY += sprite->tDeltaY;
    sprite->y = sprite->tPosY >> 7;
    sprite->tWaveIndex += sprite->tWaveDelta;
    sprite->tWaveIndex &= 0xFF;
    sprite->x2 = gSineTable[sprite->tWaveIndex] / 64;

    x = (sprite->x + sprite->centerToCornerVecX + gSpriteCoordOffsetX) & 0x1FF;
    if (x & 0x100)
        x |= -0x100;

    if (x < -3)
        sprite->x = 242 - (gSpriteCoordOffsetX + sprite->centerToCornerVecX);
    else if (x > 242)
        sprite->x = -3 - (gSpriteCoordOffsetX + sprite->centerToCornerVecX);

    y = (sprite->y + sprite->centerToCornerVecY + gSpriteCoordOffsetY) & 0xFF;
    if (y > 163 && y < 171)
    {
        sprite->y = 250 - (gSpriteCoordOffsetY + sprite->centerToCornerVecY);
        sprite->tPosY = sprite->y * 128;
        sprite->tFallCounter = 0;
        sprite->tFallDuration = 220;
    }
    else if (y > 242 && y < 250)
    {
        sprite->y = 163;
        sprite->tPosY = sprite->y * 128;
        sprite->tFallCounter = 0;
        sprite->tFallDuration = 220;
        sprite->invisible = TRUE;
        sprite->callback = WaitSnowflakeSprite;
    }

    if (++sprite->tFallCounter == sprite->tFallDuration)
    {
        InitSnowflakeSpriteMovement(sprite);
        sprite->y = 250;
        sprite->invisible = TRUE;
        sprite->callback = WaitSnowflakeSprite;
    }
}

#undef tPosY
#undef tDeltaY
#undef tWaveDelta
#undef tWaveIndex
#undef tSnowflakeId
#undef tFallCounter
#undef tFallDuration
#undef tDeltaY2

//------------------------------------------------------------------------------
// WEATHER_RAIN_THUNDERSTORM
//------------------------------------------------------------------------------

static void SetThunderCounter(u16 max);
static void UpdateThunderSound(void);

void Thunderstorm_InitVars(void)
{
    gWeatherPtr->initStep = 0;
    gWeatherPtr->weatherGfxLoaded = FALSE;
    gWeatherPtr->rainSpriteVisibleCounter = 0;
    gWeatherPtr->rainSpriteVisibleDelay = 4;
    gWeatherPtr->isDownpour = FALSE;
    gWeatherPtr->targetRainSpriteCount = 16;
    gWeatherPtr->gammaTargetIndex = 3;
    gWeatherPtr->gammaStepDelay = 20;
    gWeatherPtr->thunderTriggered = FALSE;
    SetRainStrengthFromSoundEffect(SE_THUNDERSTORM);
}

void Thunderstorm_InitAll(void)
{
    Thunderstorm_InitVars();
    while (!gWeatherPtr->weatherGfxLoaded)
        Thunderstorm_Main();
}

void Thunderstorm_Main(void)
{
    UpdateThunderSound();
	
    switch (gWeatherPtr->initStep)
    {
    case 0:
        LoadRainSpriteSheet();
        gWeatherPtr->initStep++;
        break;
    case 1:
        if (!CreateRainSprite())
            gWeatherPtr->initStep++;
        break;
    case 2:
        if (!UpdateVisibleRainSprites())
        {
            gWeatherPtr->weatherGfxLoaded = TRUE;
            gWeatherPtr->initStep++;
        }
        break;
    case 3:
        if (gWeatherPtr->palProcessingState != WEATHER_PAL_STATE_CHANGING_WEATHER)
            gWeatherPtr->initStep = 6;
        break;
    case 4:
        gWeatherPtr->thunderAllowEnd = TRUE;
        gWeatherPtr->thunderDelay = (Random() % 360) + 360;
        gWeatherPtr->initStep++;
        // fall through
    case 5:
        if (--gWeatherPtr->thunderDelay == 0)
            gWeatherPtr->initStep++;
        break;
    case 6:
        gWeatherPtr->thunderAllowEnd = TRUE;
        gWeatherPtr->thunderSkipShort = Random() % 2;
        gWeatherPtr->initStep++;
        break;
    case 7:
        gWeatherPtr->thunderShortRetries = (Random() & 1) + 1;
        gWeatherPtr->initStep++;
        // fall through
    case 8:
        WeatherShiftGammaIfPalStateIdle(19);
        if (!gWeatherPtr->thunderSkipShort && gWeatherPtr->thunderShortRetries == 1)
            SetThunderCounter(20);

        gWeatherPtr->thunderDelay = (Random() % 3) + 6;
        gWeatherPtr->initStep++;
        break;
    case 9:
        if (--gWeatherPtr->thunderDelay == 0)
        {
            WeatherShiftGammaIfPalStateIdle(3);
            gWeatherPtr->thunderAllowEnd = TRUE;
			
            if (--gWeatherPtr->thunderShortRetries != 0)
            {
                gWeatherPtr->thunderDelay = (Random() % 16) + 60;
                gWeatherPtr->initStep = 10;
            }
            else if (!gWeatherPtr->thunderSkipShort)
                gWeatherPtr->initStep = 4;
            else
                gWeatherPtr->initStep = 11;
        }
        break;
    case 10:
        if (--gWeatherPtr->thunderDelay == 0)
            gWeatherPtr->initStep = 8;
        break;
    case 11:
        gWeatherPtr->thunderDelay = (Random() % 16) + 60;
        gWeatherPtr->initStep++;
        break;
    case 12:
        if (--gWeatherPtr->thunderDelay == 0)
        {
            SetThunderCounter(100);
            WeatherShiftGammaIfPalStateIdle(19);
            gWeatherPtr->thunderDelay = (Random() & 0xF) + 30;
            gWeatherPtr->initStep++;
        }
        break;
    case 13:
        if (--gWeatherPtr->thunderDelay == 0)
        {
            WeatherBeginGammaFade(19, 3, 5);
            gWeatherPtr->initStep++;
        }
        break;
    case 14:
        if (gWeatherPtr->palProcessingState == WEATHER_PAL_STATE_IDLE)
        {
            gWeatherPtr->thunderAllowEnd = TRUE;
            gWeatherPtr->initStep = 4;
        }
        break;
    }
}

bool8 Thunderstorm_Finish(void)
{
    switch (gWeatherPtr->finishStep)
    {
    case 0:
        gWeatherPtr->thunderAllowEnd = FALSE;
        gWeatherPtr->finishStep++;
        // fall through
    case 1:
        Thunderstorm_Main();
        if (gWeatherPtr->thunderAllowEnd)
        {
            if (gWeatherPtr->nextWeather == WEATHER_RAIN || gWeatherPtr->nextWeather == WEATHER_RAIN_THUNDERSTORM || gWeatherPtr->nextWeather == WEATHER_DOWNPOUR)
                return FALSE;

            gWeatherPtr->targetRainSpriteCount = 0;
            gWeatherPtr->finishStep++;
        }
        break;
    case 2:
        if (!UpdateVisibleRainSprites())
        {
            DestroyRainSprites();
            gWeatherPtr->thunderTriggered = FALSE;
            gWeatherPtr->finishStep++;
            return FALSE;
        }
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

static void SetThunderCounter(u16 max)
{
    if (!gWeatherPtr->thunderTriggered)
    {
        gWeatherPtr->thunderCounter = Random() % max;
        gWeatherPtr->thunderTriggered = TRUE;
    }
}

static void UpdateThunderSound(void)
{
    if (gWeatherPtr->thunderTriggered)
    {
        if (!gWeatherPtr->thunderCounter)
        {
            if (IsSEPlaying())
                return;

            if (Random() & 1)
                PlaySE(SE_THUNDER);
            else
                PlaySE(SE_THUNDER2);

            gWeatherPtr->thunderTriggered = FALSE;
        }
        else
            gWeatherPtr->thunderCounter--;
    }
}

//------------------------------------------------------------------------------
// WEATHER_FOG_HORIZONTAL
//------------------------------------------------------------------------------

static void CreateFogHorizontalSprites(void);
static void DestroyFogHorizontalSprites(void);
static void FogHorizontalSpriteCallback(struct Sprite *);

static const struct OamData gOamData_839AB2C = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_BLEND,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd gSpriteAnim_839AB34[] = {
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_END,
};

static const union AnimCmd gSpriteAnim_839AB3C[] = {
    ANIMCMD_FRAME(32, 16),
    ANIMCMD_END,
};

static const union AnimCmd gSpriteAnim_839AB44[] = {
    ANIMCMD_FRAME(64, 16),
    ANIMCMD_END,
};

static const union AnimCmd gSpriteAnim_839AB4C[] = {
    ANIMCMD_FRAME(96, 16),
    ANIMCMD_END,
};

static const union AnimCmd gSpriteAnim_839AB54[] = {
    ANIMCMD_FRAME(128, 16),
    ANIMCMD_END,
};

static const union AnimCmd gSpriteAnim_839AB5C[] = {
    ANIMCMD_FRAME(160, 16),
    ANIMCMD_END,
};

static const union AnimCmd *const gSpriteAnimTable_839AB64[] = {
    gSpriteAnim_839AB34,
    gSpriteAnim_839AB3C,
    gSpriteAnim_839AB44,
    gSpriteAnim_839AB4C,
    gSpriteAnim_839AB54,
    gSpriteAnim_839AB5C,
};

static const union AffineAnimCmd gSpriteAffineAnim_839AB7C[] = {
    AFFINEANIMCMD_FRAME(0x200, 0x200, 0, 0),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd *const gSpriteAffineAnimTable_839AB8C[] = {
    gSpriteAffineAnim_839AB7C,
};

static const struct SpriteTemplate sFogHorizontalSpriteTemplate = {
    .tileTag = GFXTAG_FOG_H,
    .paletteTag = PALTAG_WEATHER,
    .oam = &gOamData_839AB2C,
    .anims = gSpriteAnimTable_839AB64,
    .images = NULL,
    .affineAnims = gSpriteAffineAnimTable_839AB8C,
    .callback = FogHorizontalSpriteCallback,
};

void FogHorizontal_InitVars(void)
{
    gWeatherPtr->initStep = 0;
    gWeatherPtr->weatherGfxLoaded = FALSE;
    gWeatherPtr->gammaTargetIndex = 0;
    gWeatherPtr->gammaStepDelay = 20;
	
    if (!gWeatherPtr->fogHSpritesCreated)
    {
        gWeatherPtr->fogHScrollCounter = 0;
        gWeatherPtr->fogHScrollOffset = 0;
        gWeatherPtr->fogHScrollPosX = 0;
        Weather_SetBlendCoeffs(8, 10);
    }
}

void FogHorizontal_InitAll(void)
{
    FogHorizontal_InitVars();
    while (!gWeatherPtr->weatherGfxLoaded)
        FogHorizontal_Main();
}

void FogHorizontal_Main(void)
{
    gWeatherPtr->fogHScrollPosX = (gSpriteCoordOffsetX - gWeatherPtr->fogHScrollOffset) & 0xFF;
	
    if (++gWeatherPtr->fogHScrollCounter > 3)
    {
        gWeatherPtr->fogHScrollCounter = 0;
        gWeatherPtr->fogHScrollOffset++;
    }
    switch (gWeatherPtr->initStep)
    {
    case 0:
        CreateFogHorizontalSprites();
        if (gWeatherPtr->currWeather == WEATHER_FOG_HORIZONTAL)
            Weather_SetTargetBlendCoeffs(10, 9, 6);
        else
            Weather_SetTargetBlendCoeffs(9, 11, 0); // WEATHER_UNDERWATER_BUBBLES
		SetGpuRegBits(REG_OFFSET_WININ, WININ_WIN0_CLR);
		SetGpuRegBits(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG1);
        gWeatherPtr->initStep++;
        break;
    case 1:
        if (Weather_UpdateBlend())
        {
            gWeatherPtr->weatherGfxLoaded = TRUE;
            gWeatherPtr->initStep++;
        }
        break;
    }
}

bool8 FogHorizontal_Finish(void)
{
    gWeatherPtr->fogHScrollPosX = (gSpriteCoordOffsetX - gWeatherPtr->fogHScrollOffset) & 0xFF;
	
    if (++gWeatherPtr->fogHScrollCounter > 3)
    {
        gWeatherPtr->fogHScrollCounter = 0;
        gWeatherPtr->fogHScrollOffset++;
    }
    switch (gWeatherPtr->finishStep)
    {
    case 0:
        Weather_SetTargetBlendCoeffs(6, 12, 3);
        gWeatherPtr->finishStep++;
        break;
    case 1:
        if (Weather_UpdateBlend())
		{
			ClearGpuRegBits(REG_OFFSET_WININ, WININ_WIN0_CLR);
			ClearGpuRegBits(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG1);
			Weather_SetTargetBlendCoeffs(0, 16, 0);
            gWeatherPtr->finishStep++;
		}
        break;
    case 2:
	    if (Weather_UpdateBlend())
		{
			DestroyFogHorizontalSprites();
			gWeatherPtr->finishStep++;
		}
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

#define tSpriteColumn data[0]

static void FogHorizontalSpriteCallback(struct Sprite *sprite)
{
    sprite->y2 = (u8)gSpriteCoordOffsetY;
    sprite->x = gWeatherPtr->fogHScrollPosX + 32 + sprite->tSpriteColumn * 64;
	
    if (sprite->x > 271)
    {
        sprite->x = 480 + gWeatherPtr->fogHScrollPosX - (4 - sprite->tSpriteColumn) * 64;
        sprite->x &= 0x1FF;
    }
}

static void CreateFogHorizontalSprites(void)
{
	u8 spriteId;
    u16 i;
    struct Sprite *sprite;

    if (!gWeatherPtr->fogHSpritesCreated)
    {
        struct SpriteSheet fogHorizontalSpriteSheet = {
            .data = gWeatherFogHorizontalTiles,
            .size = 0x0800,
            .tag = GFXTAG_FOG_H,
        };
        LoadSpriteSheet(&fogHorizontalSpriteSheet);
		
        for (i = 0; i < NUM_FOG_HORIZONTAL_SPRITES; i++)
        {
            spriteId = CreateSpriteAtEnd(&sFogHorizontalSpriteTemplate, 0, 0, 0xFF);
			
            if (spriteId != MAX_SPRITES)
            {
                sprite = &gSprites[spriteId];
                sprite->tSpriteColumn = i % 5;
                sprite->x = (i % 5) * 64 + 32;
                sprite->y = (i / 5) * 64 + 32;
                gWeatherPtr->fogHSprites[i] = sprite;
            }
            else
                gWeatherPtr->fogHSprites[i] = NULL;
        }
        gWeatherPtr->fogHSpritesCreated = TRUE;
    }
}

static void DestroyFogHorizontalSprites(void)
{
    u16 i;

    if (gWeatherPtr->fogHSpritesCreated)
    {
        for (i = 0; i < NUM_FOG_HORIZONTAL_SPRITES; i++)
        {
            if (gWeatherPtr->fogHSprites[i] != NULL)
                DestroySprite(gWeatherPtr->fogHSprites[i]);
        }
        FreeSpriteTilesByTag(GFXTAG_FOG_H);
        gWeatherPtr->fogHSpritesCreated = FALSE;
    }
}

#undef tSpriteColumn

//------------------------------------------------------------------------------
// WEATHER_VOLCANIC_ASH
//------------------------------------------------------------------------------

static void CreateAshSprites(void);
static void DestroyAshSprites(void);
static void UpdateAshSprite(struct Sprite *);

static const struct SpriteSheet sAshSpriteSheet = {
    .data = gWeatherAshTiles,
    .size = 0x1000,
    .tag = GFXTAG_ASH,
};

static const struct OamData sAshSpriteOamData = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 15,
};

static const union AnimCmd sAshSpriteAnimCmd0[] = {
    ANIMCMD_FRAME(0, 60),
    ANIMCMD_FRAME(64, 60),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAshSpriteAnimCmds[] = {
    sAshSpriteAnimCmd0,
};

static const struct SpriteTemplate sAshSpriteTemplate = {
    .tileTag = GFXTAG_ASH,
    .paletteTag = PALTAG_WEATHER,
    .oam = &sAshSpriteOamData,
    .anims = sAshSpriteAnimCmds,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = UpdateAshSprite,
};

void Ash_InitVars(void)
{
    gWeatherPtr->initStep = 0;
    gWeatherPtr->weatherGfxLoaded = FALSE;
    gWeatherPtr->gammaTargetIndex = 0;
    gWeatherPtr->gammaStepDelay = 20;
}

void Ash_InitAll(void)
{
    Ash_InitVars();
    while (!gWeatherPtr->weatherGfxLoaded)
        Ash_Main();
}

void Ash_Main(void)
{
    gWeatherPtr->ashBaseSpritesX = gSpriteCoordOffsetX & 0x1FF;
    while (gWeatherPtr->ashBaseSpritesX >= 240)
        gWeatherPtr->ashBaseSpritesX -= 240;

    switch (gWeatherPtr->initStep)
    {
    case 0:
        LoadSpriteSheet(&sAshSpriteSheet);
        gWeatherPtr->initStep++;
        break;
    case 1:
        if (!gWeatherPtr->ashSpritesCreated)
            CreateAshSprites();
		gWeatherPtr->weatherGfxLoaded = TRUE;
        gWeatherPtr->initStep++;
        break;
    default:
        break;
    }
}

bool8 Ash_Finish(void)
{
    DestroyAshSprites();
    return FALSE;
}

#define tOffsetY      data[0]
#define tCounterY     data[1]
#define tSpriteColumn data[2]
#define tSpriteRow    data[3]

static void CreateAshSprites(void)
{
    u8 i, spriteId;
    struct Sprite *sprite;

    if (!gWeatherPtr->ashSpritesCreated)
    {
        for (i = 0; i < NUM_ASH_SPRITES; i++)
        {
            spriteId = CreateSpriteAtEnd(&sAshSpriteTemplate, 0, 0, 0x4E);
            if (spriteId != MAX_SPRITES)
            {
                sprite = &gSprites[spriteId];
                sprite->tCounterY = 0;
                sprite->tSpriteColumn = (u8)(i % 5);
                sprite->tSpriteRow = (u8)(i / 5);
                sprite->tOffsetY = sprite->tSpriteRow * 64 + 32;
                gWeatherPtr->ashSprites[i] = sprite;
            }
            else
                gWeatherPtr->ashSprites[i] = NULL;
        }
        gWeatherPtr->ashSpritesCreated = TRUE;
    }
}

static void DestroyAshSprites(void)
{
    u16 i;

    if (gWeatherPtr->ashSpritesCreated)
    {
        for (i = 0; i < NUM_ASH_SPRITES; i++)
        {
            if (gWeatherPtr->ashSprites[i] != NULL)
                DestroySprite(gWeatherPtr->ashSprites[i]);
        }
        FreeSpriteTilesByTag(GFXTAG_ASH);
        gWeatherPtr->ashSpritesCreated = FALSE;
    }
}

static void UpdateAshSprite(struct Sprite *sprite)
{
    if (++sprite->tCounterY > 5)
    {
        sprite->tCounterY = 0;
        sprite->tOffsetY++;
    }
    sprite->y = gSpriteCoordOffsetY + sprite->tOffsetY;
    sprite->x = gWeatherPtr->ashBaseSpritesX + 32 + sprite->tSpriteColumn * 64;
	
    if (sprite->x > 271)
    {
        sprite->x = gWeatherPtr->ashBaseSpritesX + 480 - (4 - sprite->tSpriteColumn) * 64;
        sprite->x &= 0x1FF;
    }
}

#undef tOffsetY
#undef tCounterY
#undef tSpriteColumn
#undef tSpriteRow

//------------------------------------------------------------------------------
// WEATHER_SANDSTORM
//------------------------------------------------------------------------------

static void UpdateSandstormWaveIndex(void);
static void UpdateSandstormMovement(void);
static void CreateSandstormSprites(void);
static void CreateSwirlSandstormSprites(void);
static void DestroySandstormSprites(void);
static void UpdateSandstormSprite(struct Sprite *);
static void WaitSandSwirlSpriteEntrance(struct Sprite *);
static void UpdateSandstormSwirlSprite(struct Sprite *);

#define MIN_SANDSTORM_WAVE_INDEX 0x20

static const u16 sSwirlEntranceDelays[] = {0, 120, 80, 160, 40, 0};

static const struct OamData sSandstormSpriteOamData = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
};

static const union AnimCmd sSandstormSpriteAnimCmd0[] = {
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_END,
};

static const union AnimCmd sSandstormSpriteAnimCmd1[] = {
    ANIMCMD_FRAME(64, 3),
    ANIMCMD_END,
};

static const union AnimCmd *const sSandstormSpriteAnimCmds[] = {
    sSandstormSpriteAnimCmd0,
    sSandstormSpriteAnimCmd1,
};

static const struct SpriteTemplate sSandstormSpriteTemplate = {
    .tileTag = GFXTAG_SANDSTORM,
    .paletteTag = GFXTAG_SANDSTORM,
    .oam = &sSandstormSpriteOamData,
    .anims = sSandstormSpriteAnimCmds,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = UpdateSandstormSprite,
};

static const struct SpriteSheet sSandstormSpriteSheet = {
    .data = gWeatherSandstormTiles,
    .size = 0x0a00,
    .tag = GFXTAG_SANDSTORM,
};

void Sandstorm_InitVars(void)
{
    gWeatherPtr->initStep = 0;
    gWeatherPtr->weatherGfxLoaded = 0;
    gWeatherPtr->gammaTargetIndex = 0;
    gWeatherPtr->gammaStepDelay = 20;
	
    if (!gWeatherPtr->sandstormSpritesCreated)
    {
        gWeatherPtr->sandstormXOffset = gWeatherPtr->sandstormYOffset = 0;
        gWeatherPtr->sandstormWaveIndex = 8;
        gWeatherPtr->sandstormWaveCounter = 0;
        Weather_SetBlendCoeffs(0, 16);
    }
}

void Sandstorm_InitAll(void)
{
    Sandstorm_InitVars();
    while (!gWeatherPtr->weatherGfxLoaded)
        Sandstorm_Main();
}

void Sandstorm_Main(void)
{
    UpdateSandstormMovement();
    UpdateSandstormWaveIndex();
	
    if (gWeatherPtr->sandstormWaveIndex >= 0x80 - MIN_SANDSTORM_WAVE_INDEX)
        gWeatherPtr->sandstormWaveIndex = MIN_SANDSTORM_WAVE_INDEX;

    switch (gWeatherPtr->initStep)
    {
    case 0:
        CreateSandstormSprites();
        CreateSwirlSandstormSprites();
        gWeatherPtr->initStep++;
        break;
    case 1:
        Weather_SetTargetBlendCoeffs(16, 0, 0);
        gWeatherPtr->initStep++;
        break;
    case 2:
        if (Weather_UpdateBlend())
        {
            gWeatherPtr->weatherGfxLoaded = TRUE;
            gWeatherPtr->initStep++;
        }
        break;
    }
}

bool8 Sandstorm_Finish(void)
{
    UpdateSandstormMovement();
    UpdateSandstormWaveIndex();
	
    switch (gWeatherPtr->finishStep)
    {
    case 0:
        Weather_SetTargetBlendCoeffs(0, 16, 0);
        gWeatherPtr->finishStep++;
        break;
    case 1:
        if (Weather_UpdateBlend())
            gWeatherPtr->finishStep++;
        break;
    case 2:
        DestroySandstormSprites();
        gWeatherPtr->finishStep++;
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

// Regular sandstorm sprites
#define tSpriteColumn  data[0]
#define tSpriteRow     data[1]

// Swirly sandstorm sprites
#define tRadius        data[0]
#define tWaveIndex     data[1]
#define tRadiusCounter data[2]
#define tEntranceDelay data[3]

static void UpdateSandstormWaveIndex(void)
{
    if (++gWeatherPtr->sandstormWaveCounter > 4)
    {
        gWeatherPtr->sandstormWaveIndex++;
        gWeatherPtr->sandstormWaveCounter = 0;
    }
}

static void UpdateSandstormMovement(void)
{
    gWeatherPtr->sandstormXOffset -= gSineTable[gWeatherPtr->sandstormWaveIndex] * 4;
    gWeatherPtr->sandstormYOffset -= gSineTable[gWeatherPtr->sandstormWaveIndex];
    gWeatherPtr->sandstormBaseSpritesX = (gSpriteCoordOffsetX + (gWeatherPtr->sandstormXOffset >> 8)) & 0xFF;
    gWeatherPtr->sandstormPosY = gSpriteCoordOffsetY + (gWeatherPtr->sandstormYOffset >> 8);
}

static void DestroySandstormSprites(void)
{
    u16 i;

    if (gWeatherPtr->sandstormSpritesCreated)
    {
        for (i = 0; i < NUM_SANDSTORM_SPRITES; i++)
        {
            if (gWeatherPtr->sandstormSprites1[i])
                DestroySprite(gWeatherPtr->sandstormSprites1[i]);
        }
        gWeatherPtr->sandstormSpritesCreated = FALSE;
        FreeSpriteTilesByTag(GFXTAG_SANDSTORM);
    }
    if (gWeatherPtr->sandstormSwirlSpritesCreated)
    {
        for (i = 0; i < NUM_SWIRL_SANDSTORM_SPRITES; i++)
        {
            if (gWeatherPtr->sandstormSprites2[i] != NULL)
                DestroySprite(gWeatherPtr->sandstormSprites2[i]);
        }
        gWeatherPtr->sandstormSwirlSpritesCreated = FALSE;
    }
	FreeSpritePaletteByTag(GFXTAG_SANDSTORM);
}

static void CreateSandstormSprites(void)
{
    u16 i;
    u8 spriteId;

    if (!gWeatherPtr->sandstormSpritesCreated)
    {
        LoadSpriteSheet(&sSandstormSpriteSheet);
        LoadCustomWeatherSpritePalette(&sSandstormSpritePalette);
		
        for (i = 0; i < NUM_SANDSTORM_SPRITES; i++)
        {
            spriteId = CreateSpriteAtEnd(&sSandstormSpriteTemplate, 0, (i / 5) * 64, 1);
            if (spriteId != MAX_SPRITES)
            {
                gWeatherPtr->sandstormSprites1[i] = &gSprites[spriteId];
                gWeatherPtr->sandstormSprites1[i]->tSpriteColumn = i % 5;
                gWeatherPtr->sandstormSprites1[i]->tSpriteRow = i / 5;
            }
            else
                gWeatherPtr->sandstormSprites1[i] = NULL;
        }
        gWeatherPtr->sandstormSpritesCreated = TRUE;
    }
}

static void CreateSwirlSandstormSprites(void)
{
    u16 i;
    u8 spriteId;

    if (!gWeatherPtr->sandstormSwirlSpritesCreated)
    {
        for (i = 0; i < NUM_SWIRL_SANDSTORM_SPRITES; i++)
        {
            spriteId = CreateSpriteAtEnd(&sSandstormSpriteTemplate, i * 48 + 24, 208, 1);
            if (spriteId != MAX_SPRITES)
            {
                gWeatherPtr->sandstormSprites2[i] = &gSprites[spriteId];
                gWeatherPtr->sandstormSprites2[i]->oam.size = ST_OAM_SIZE_2;
                gWeatherPtr->sandstormSprites2[i]->tSpriteRow = i * 51;
                gWeatherPtr->sandstormSprites2[i]->tRadius = 8;
                gWeatherPtr->sandstormSprites2[i]->tRadiusCounter = 0;
                gWeatherPtr->sandstormSprites2[i]->tEntranceDelay = sSwirlEntranceDelays[i];
                StartSpriteAnim(gWeatherPtr->sandstormSprites2[i], 1);
                CalcCenterToCornerVec(gWeatherPtr->sandstormSprites2[i], SPRITE_SHAPE(32x32), SPRITE_SIZE(32x32), ST_OAM_AFFINE_OFF);
                gWeatherPtr->sandstormSprites2[i]->callback = WaitSandSwirlSpriteEntrance;
            }
            else
                gWeatherPtr->sandstormSprites2[i] = NULL;
        }
		gWeatherPtr->sandstormSwirlSpritesCreated = TRUE;
    }
}

static void UpdateSandstormSprite(struct Sprite *sprite)
{
    sprite->y2 = gWeatherPtr->sandstormPosY;
    sprite->x = gWeatherPtr->sandstormBaseSpritesX + 32 + sprite->tSpriteColumn * 64;
	
    if (sprite->x > 271)
    {
        sprite->x = gWeatherPtr->sandstormBaseSpritesX + 480 - (4 - sprite->tSpriteColumn) * 64;
        sprite->x &= 0x1FF;
    }
}

static void WaitSandSwirlSpriteEntrance(struct Sprite *sprite)
{
    if (--sprite->tEntranceDelay == -1)
        sprite->callback = UpdateSandstormSwirlSprite;
}

static void UpdateSandstormSwirlSprite(struct Sprite *sprite)
{
    u32 x, y;

    if (--sprite->y < -48)
    {
        sprite->y = 208;
        sprite->tRadius = 4;
    }

    x = sprite->tRadius * gSineTable[sprite->tWaveIndex];
    y = sprite->tRadius * gSineTable[sprite->tWaveIndex + 0x40];
    sprite->x2 = x >> 8;
    sprite->y2 = y >> 8;
    sprite->tWaveIndex = (sprite->tWaveIndex + 10) & 0xFF;
	
    if (++sprite->tRadiusCounter > 8)
    {
        sprite->tRadiusCounter = 0;
        sprite->tRadius++;
    }
}

#undef tSpriteColumn
#undef tSpriteRow

#undef tRadius
#undef tWaveIndex
#undef tRadiusCounter
#undef tEntranceDelay

//------------------------------------------------------------------------------
// WEATHER_FOG_DIAGONAL
//------------------------------------------------------------------------------

static void UpdateFogDiagonalMovement(void);
static void CreateFogDiagonalSprites(void);
static void DestroyFogDiagonalSprites(void);
static void UpdateFogDiagonalSprite(struct Sprite *);

static const struct SpriteSheet gFogDiagonalSpriteSheet = {
    .data = gWeatherFogDiagonalTiles,
    .size = 0x0800,
    .tag = GFXTAG_FOG_D,
};

static const struct OamData sFogDiagonalSpriteOamData = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_BLEND,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

static const union AnimCmd sFogDiagonalSpriteAnimCmd0[] = {
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_END,
};

static const union AnimCmd *const sFogDiagonalSpriteAnimCmds[] = {
    sFogDiagonalSpriteAnimCmd0,
};

static const struct SpriteTemplate sFogDiagonalSpriteTemplate = {
    .tileTag = GFXTAG_FOG_D,
    .paletteTag = PALTAG_WEATHER,
    .oam = &sFogDiagonalSpriteOamData,
    .anims = sFogDiagonalSpriteAnimCmds,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = UpdateFogDiagonalSprite,
};

void FogDiagonal_InitVars(void)
{
    gWeatherPtr->initStep = 0;
    gWeatherPtr->weatherGfxLoaded = 0;
    gWeatherPtr->gammaTargetIndex = 0;
    gWeatherPtr->gammaStepDelay = 20;
    gWeatherPtr->fogHScrollCounter = 0;
    gWeatherPtr->fogHScrollOffset = 1;
	
    if (!gWeatherPtr->fogDSpritesCreated)
    {
        gWeatherPtr->fogDScrollXCounter = 0;
        gWeatherPtr->fogDScrollYCounter = 0;
        gWeatherPtr->fogDXOffset = 0;
        gWeatherPtr->fogDYOffset = 0;
        gWeatherPtr->fogDBaseSpritesX = 0;
        gWeatherPtr->fogDPosY = 0;
        Weather_SetBlendCoeffs(8, 10);
    }
}

void FogDiagonal_InitAll(void)
{
    FogDiagonal_InitVars();
    while (!gWeatherPtr->weatherGfxLoaded)
        FogDiagonal_Main();
}

void FogDiagonal_Main(void)
{
    UpdateFogDiagonalMovement();
	
    switch (gWeatherPtr->initStep)
    {
    case 0:
        CreateFogDiagonalSprites();
        gWeatherPtr->initStep++;
        break;
    case 1:
        Weather_SetTargetBlendCoeffs(10, 9, 6);
		SetGpuRegBits(REG_OFFSET_WININ, WININ_WIN0_CLR);
		SetGpuRegBits(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG1);
        gWeatherPtr->initStep++;
        break;
    case 2:
	    if (Weather_UpdateBlend())
		{
			gWeatherPtr->weatherGfxLoaded = TRUE;
			gWeatherPtr->initStep++;
		}
        break;
    }
}

bool8 FogDiagonal_Finish(void)
{
    UpdateFogDiagonalMovement();
	
    switch (gWeatherPtr->finishStep)
    {
    case 0:
        Weather_SetTargetBlendCoeffs(6, 12, 3);
        gWeatherPtr->finishStep++;
        break;
    case 1:
        if (Weather_UpdateBlend())
		{
			ClearGpuRegBits(REG_OFFSET_WININ, WININ_WIN0_CLR);
			ClearGpuRegBits(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG1);
			Weather_SetTargetBlendCoeffs(0, 16, 0);
			gWeatherPtr->finishStep++;
		}
        break;
    case 2:
        if (Weather_UpdateBlend())
		{
			DestroyFogDiagonalSprites();
			gWeatherPtr->finishStep++;
		}
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

#define tSpriteColumn data[0]
#define tSpriteRow    data[1]

static void UpdateFogDiagonalMovement(void)
{
    if (++gWeatherPtr->fogDScrollXCounter > 2)
    {
        gWeatherPtr->fogDXOffset++;
        gWeatherPtr->fogDScrollXCounter = 0;
    }
    if (++gWeatherPtr->fogDScrollYCounter > 4)
    {
        gWeatherPtr->fogDYOffset++;
        gWeatherPtr->fogDScrollYCounter = 0;
    }
    gWeatherPtr->fogDBaseSpritesX = (gSpriteCoordOffsetX - gWeatherPtr->fogDXOffset) & 0xFF;
    gWeatherPtr->fogDPosY = gSpriteCoordOffsetY + gWeatherPtr->fogDYOffset;
}

static void CreateFogDiagonalSprites(void)
{
    u16 i;
    u8 spriteId;
    struct Sprite *sprite;

    if (!gWeatherPtr->fogDSpritesCreated)
    {
        LoadSpriteSheet(&gFogDiagonalSpriteSheet);
		
        for (i = 0; i < NUM_FOG_DIAGONAL_SPRITES; i++)
        {
            spriteId = CreateSpriteAtEnd(&sFogDiagonalSpriteTemplate, 0, (i / 5) * 64, 0xFF);
            if (spriteId != MAX_SPRITES)
            {
                sprite = &gSprites[spriteId];
                sprite->tSpriteColumn = i % 5;
                sprite->tSpriteRow = i / 5;
                gWeatherPtr->fogDSprites[i] = sprite;
            }
            else
                gWeatherPtr->fogDSprites[i] = NULL;
        }
        gWeatherPtr->fogDSpritesCreated = TRUE;
    }
}

static void DestroyFogDiagonalSprites(void)
{
    u16 i;

    if (gWeatherPtr->fogDSpritesCreated)
    {
        for (i = 0; i < NUM_FOG_DIAGONAL_SPRITES; i++)
        {
            if (gWeatherPtr->fogDSprites[i])
                DestroySprite(gWeatherPtr->fogDSprites[i]);
        }
        FreeSpriteTilesByTag(GFXTAG_FOG_D);
        gWeatherPtr->fogDSpritesCreated = FALSE;
    }
}

static void UpdateFogDiagonalSprite(struct Sprite *sprite)
{
    sprite->y2 = gWeatherPtr->fogDPosY;
    sprite->x = gWeatherPtr->fogDBaseSpritesX + 32 + sprite->tSpriteColumn * 64;
	
    if (sprite->x > 271)
    {
        sprite->x = gWeatherPtr->fogDBaseSpritesX + 480 - (4 - sprite->tSpriteColumn) * 64;
        sprite->x &= 0x1FF;
    }
}

#undef tSpriteColumn
#undef tSpriteRow

//------------------------------------------------------------------------------
// WEATHER_SHADE
//------------------------------------------------------------------------------

void Shade_InitVars(void)
{
    gWeatherPtr->initStep = 0;
    gWeatherPtr->gammaTargetIndex = 3;
    gWeatherPtr->gammaStepDelay = 20;
}

void Shade_InitAll(void)
{
    Shade_InitVars();
}

void Shade_Main(void)
{
}

bool8 Shade_Finish(void)
{
    return FALSE;
}

//------------------------------------------------------------------------------
// WEATHER_DOWNPOUR
//------------------------------------------------------------------------------

void Downpour_InitVars(void)
{
    gWeatherPtr->initStep = 0;
    gWeatherPtr->weatherGfxLoaded = FALSE;
    gWeatherPtr->rainSpriteVisibleCounter = 0;
    gWeatherPtr->rainSpriteVisibleDelay = 4;
    gWeatherPtr->isDownpour = TRUE;
    gWeatherPtr->targetRainSpriteCount = 24;
    gWeatherPtr->gammaTargetIndex = 3;
    gWeatherPtr->gammaStepDelay = 20;
    SetRainStrengthFromSoundEffect(SE_DOWNPOUR);
}

void Downpour_InitAll(void)
{
    Downpour_InitVars();
    while (!gWeatherPtr->weatherGfxLoaded)
        Thunderstorm_Main();
}

//------------------------------------------------------------------------------
// WEATHER_UNDERWATER_BUBBLES
//------------------------------------------------------------------------------

static void CreateBubbleSprite(u16);
static void DestroyBubbleSprites(void);
static void UpdateBubbleSprite(struct Sprite *);

static const u8 sBubbleStartDelays[] = {40, 90, 60, 90, 2, 60, 40, 30};

static const struct SpriteSheet sWeatherBubbleSpriteSheet = {
    .data = gWeatherBubbleTiles,
    .size = 0x0040,
    .tag = GFXTAG_BUBBLE,
};

static const union AnimCmd sBubbleSpriteAnimCmd0[] = {
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_FRAME(1, 16),
    ANIMCMD_END,
};

static const union AnimCmd *const sBubbleSpriteAnimCmds[] = {
    sBubbleSpriteAnimCmd0,
};

static const struct SpriteTemplate sBubbleSpriteTemplate = {
    .tileTag = GFXTAG_BUBBLE,
    .paletteTag = PALTAG_WEATHER,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = sBubbleSpriteAnimCmds,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = UpdateBubbleSprite,
};

static const s16 sBubbleStartCoords[][2] = {
    {120, 160},
    {376, 160},
    { 40, 140},
    {296, 140},
    {180, 130},
    {436, 130},
    { 60, 160},
    {436, 160},
    {220, 180},
    {476, 180},
    { 10,  90},
    {266,  90},
    {256, 160},
};

void Bubbles_InitVars(void)
{
    FogHorizontal_InitVars();
    if (!gWeatherPtr->bubblesSpritesCreated)
    {
        LoadSpriteSheet(&sWeatherBubbleSpriteSheet);
        gWeatherPtr->bubblesDelayIndex = 0;
        gWeatherPtr->bubblesDelayCounter = sBubbleStartDelays[0];
        gWeatherPtr->bubblesCoordsIndex = 0;
        gWeatherPtr->bubblesSpriteCount = 0;
    }
}

void Bubbles_InitAll(void)
{
    Bubbles_InitVars();
    while (!gWeatherPtr->weatherGfxLoaded)
        Bubbles_Main();
}

void Bubbles_Main(void)
{
    FogHorizontal_Main();
	
    if (++gWeatherPtr->bubblesDelayCounter > sBubbleStartDelays[gWeatherPtr->bubblesDelayIndex])
    {
        gWeatherPtr->bubblesDelayCounter = 0;
        if (++gWeatherPtr->bubblesDelayIndex > ARRAY_COUNT(sBubbleStartDelays) - 1)
            gWeatherPtr->bubblesDelayIndex = 0;

        CreateBubbleSprite(gWeatherPtr->bubblesCoordsIndex);
        if (++gWeatherPtr->bubblesCoordsIndex > ARRAY_COUNT(sBubbleStartCoords) - 1)
            gWeatherPtr->bubblesCoordsIndex = 0;
    }
}

bool8 Bubbles_Finish(void)
{
    if (!FogHorizontal_Finish())
    {
        DestroyBubbleSprites();
        return FALSE;
    }
    return TRUE;
}

#define tScrollXCounter data[0]
#define tScrollXDir     data[1]
#define tCounter        data[2]

static void CreateBubbleSprite(u16 coordsIndex)
{
    s16 x = sBubbleStartCoords[coordsIndex][0];
    s16 y = sBubbleStartCoords[coordsIndex][1] - gSpriteCoordOffsetY;
    u8 spriteId = CreateSpriteAtEnd(&sBubbleSpriteTemplate, x, y, 0);
	
    if (spriteId != MAX_SPRITES)
    {
        gSprites[spriteId].oam.priority = 1;
        gSprites[spriteId].coordOffsetEnabled = TRUE;
        gSprites[spriteId].tScrollXCounter = 0;
        gSprites[spriteId].tScrollXDir = 0;
        gSprites[spriteId].tCounter = 0;
        gWeatherPtr->bubblesSpriteCount++;
    }
}

static void DestroyBubbleSprites(void)
{
    u16 i;

    for (i = 0; i < MAX_SPRITES; i++)
    {
        if (gSprites[i].template == &sBubbleSpriteTemplate)
            DestroySprite(&gSprites[i]);
    }
    FreeSpriteTilesByTag(GFXTAG_BUBBLE);
}

static void UpdateBubbleSprite(struct Sprite *sprite)
{
    ++sprite->tScrollXCounter;
	
    if (++sprite->tScrollXCounter > 8) // double increment
    {
        sprite->tScrollXCounter = 0;
        if (sprite->tScrollXDir == 0)
        {
            if (++sprite->x2 > 4)
                sprite->tScrollXDir = 1;
        }
        else
        {
            if (--sprite->x2 <= 0)
                sprite->tScrollXDir = 0;
        }
    }
    sprite->y -= 3;
	
    if (++sprite->tCounter >= 120)
        DestroySprite(sprite);
}

#undef tScrollXCounter
#undef tScrollXDir
#undef tCounter

//------------------------------------------------------------------------------
// WEATHER_CLOUDS
//------------------------------------------------------------------------------

static void CreateCloudSprites(void);
static void DestroyCloudSprites(void);
static void UpdateCloudSprite(struct Sprite *);

// The clouds are positioned on the map's grid.
static const struct Coords16 sCloudSpriteMapCoords[] = {
    { 0, 66},
    { 5, 73},
    {10, 78},
};

static const struct SpriteSheet sCloudsSpriteSheet = {
    .data = gWeatherCloudTiles,
    .size = 0x0800,
    .tag = GFXTAG_CLOUD,
};

static const struct OamData sCloudSpriteOamData = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sCloudSpriteAnimCmd[] = {
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_END,
};

static const union AnimCmd *const sCloudSpriteAnimCmds[] = {
    sCloudSpriteAnimCmd,
};

static const struct SpriteTemplate sCloudSpriteTemplate = {
    .tileTag = GFXTAG_CLOUD,
    .paletteTag = GFXTAG_CLOUD,
    .oam = &sCloudSpriteOamData,
    .anims = sCloudSpriteAnimCmds,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = UpdateCloudSprite,
};

void Clouds_InitVars(void)
{
	gWeatherPtr->initStep = 0;
    gWeatherPtr->weatherGfxLoaded = FALSE;
    gWeatherPtr->gammaTargetIndex = 0;
    gWeatherPtr->gammaStepDelay = 20;
	if (!gWeatherPtr->cloudSpritesCreated)
		Weather_SetBlendCoeffs(0, 16);
}

void Clouds_InitAll(void)
{
	Clouds_InitVars();
	while (!gWeatherPtr->weatherGfxLoaded)
		Clouds_Main();
}

void Clouds_Main(void)
{
	CreateCloudSprites();
	gWeatherPtr->weatherGfxLoaded = TRUE;
}

bool8 Clouds_Finish(void)
{
	DestroyCloudSprites();
	return FALSE;
}

static void CreateCloudSprites(void)
{
	u8 i, spriteId;
	struct Sprite *sprite;
	
	if (!gWeatherPtr->cloudSpritesCreated)
	{
		gWeatherPtr->cloudSpritesCreated = TRUE;
		
		LoadSpriteSheet(&sCloudsSpriteSheet);
		LoadCustomWeatherSpritePalette(&sCloudsSpritePalette);
		
		for (i = 0; i < NUM_CLOUD_SPRITES; i++)
		{
			spriteId = CreateSprite(&sCloudSpriteTemplate, 0, 0, 0);
			
			if (spriteId != MAX_SPRITES)
			{
				gWeatherPtr->cloudSprites[i] = sprite = &gSprites[spriteId];
				SetSpritePosToMapCoords(sCloudSpriteMapCoords[i].x + 7, sCloudSpriteMapCoords[i].y + 7, &sprite->x, &sprite->y);
				sprite->coordOffsetEnabled = TRUE;
			}
			else
				gWeatherPtr->cloudSprites[i] = NULL;
		}
	}
}

static void DestroyCloudSprites(void)
{
	u8 i;
	
	if (gWeatherPtr->cloudSpritesCreated)
	{
		gWeatherPtr->cloudSpritesCreated = FALSE;
		
		for (i = 0; i < NUM_CLOUD_SPRITES; i++)
		{
			if (gWeatherPtr->cloudSprites[i] != NULL)
				DestroySprite(gWeatherPtr->cloudSprites[i]);
		}
	}
	FreeSpriteTilesByTag(GFXTAG_CLOUD);
}

#define tMovementDelay data[0]

static void UpdateCloudSprite(struct Sprite *sprite)
{
	// Every 10 frames move sprite 3 pixels to right and 1 pixel to down.
	if (++sprite->tMovementDelay == 10)
	{
		sprite->tMovementDelay = 0;
		sprite->x += 3;
		sprite->y++;
	}
}

#undef tMovementDelay

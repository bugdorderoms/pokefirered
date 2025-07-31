#include "global.h"
#include "gflib.h"
#include "field_camera.h"
#include "field_effect.h"
#include "field_weather.h"
#include "field_weather_effects.h"
#include "map_preview_screen.h"
#include "task.h"
#include "trig.h"
#include "constants/field_weather.h"
#include "constants/weather.h"
#include "constants/songs.h"

enum
{
    GAMMA_NORMAL,
    GAMMA_BLEND,
};

#define DROUGHT_COLOR_INDEX(color) ((((color) >> 1) & 0xF) | (((color) >> 2) & 0xF0) | (((color) >> 3) & 0xF00))

#define WEATHER_PALETTE_INDEX 15

struct WeatherCallbacks
{
    void (*initVars)(void);
    void (*main)(void);
    void (*initAll)(void);
    bool32 (*finish)(void);
};

static void Task_WeatherMain(u32 taskId);
static void Task_WeatherInit(u32 taskId);
static void None_Init(void);
static void None_Main(void);
static bool32 None_Finish(void);
static void UpdateWeatherGammaShift(void);
static void ApplyGammaShift(u32 gammaType, u32 startPalIndex, u32 numPalettes, s8 gammaIndex);
static void FadeInScreenWithWeather(void);
static bool32 FadeInScreen_RainSnowShade(void);
static bool32 FadeInScreen_Drought(void);
static void DoNothing(void);

static EWRAM_DATA struct Weather sWeather = {};
struct Weather *const gWeatherPtr = &sWeather;

const u16 gDefaultWeatherSpritePalette[] = INCBIN_U16("graphics/weather/default.gbapal");
const u16 gSandstormWeatherPalette[] = INCBIN_U16("graphics/weather/sandstorm.gbapal");
const u16 gSnowstormWeatherPalette[] = INCBIN_U16("graphics/weather/snowstorm.gbapal");
const u16 gLeavesWeatherPalette[] = INCBIN_U16("graphics/weather/leaves.gbapal");
const u8 gWeatherFogDiagonalTiles[] = INCBIN_U8("graphics/weather/fog_diagonal.4bpp");
const u8 gWeatherFogHorizontalTiles[] = INCBIN_U8("graphics/weather/fog_horizontal.4bpp");
const u8 gWeatherSnow1Tiles[] = INCBIN_U8("graphics/weather/snow0.4bpp");
const u8 gWeatherSnow2Tiles[] = INCBIN_U8("graphics/weather/snow1.4bpp");
const u8 gWeatherBubbleTiles[] = INCBIN_U8("graphics/weather/bubble.4bpp");
const u8 gWeatherAshTiles[] = INCBIN_U8("graphics/weather/ash.4bpp");
const u8 gWeatherRainTiles[] = INCBIN_U8("graphics/weather/rain.4bpp");
const u8 gWeatherSandstormTiles[] = INCBIN_U8("graphics/weather/sandstorm.4bpp");
const u8 gWeatherCloudTiles[] = INCBIN_U8("graphics/weather/cloud.4bpp");
const u8 gWeatherLeavesTiles[] = INCBIN_U8("graphics/weather/leaves.4bpp");

static const struct SpritePalette sDefaultWeatherSpritePalette = {gDefaultWeatherSpritePalette, TAG_WEATHER_START};

static const struct WeatherCallbacks sWeatherFuncs[] = {
    [WEATHER_NONE]               = {None_Init, None_Main, None_Init, None_Finish},
    [WEATHER_RAIN]               = {Rain_InitVars, Rain_Main, Rain_InitAll, Rain_Finish},
    [WEATHER_SNOW]               = {Snow_InitVars, Snow_Main, Snow_InitAll, Snow_Finish},
    [WEATHER_RAIN_THUNDERSTORM]  = {Thunderstorm_InitVars, Thunderstorm_Main, Thunderstorm_InitAll, Thunderstorm_Finish},
    [WEATHER_FOG_HORIZONTAL]     = {FogHorizontal_InitVars, FogHorizontal_Main, FogHorizontal_InitAll, FogHorizontal_Finish},
    [WEATHER_VOLCANIC_ASH]       = {Ash_InitVars, Ash_Main, Ash_InitAll, Ash_Finish},
    [WEATHER_SANDSTORM]          = {Sandstorm_InitVars, Sandstorm_Main, Sandstorm_InitAll, Sandstorm_Finish},
    [WEATHER_FOG_DIAGONAL]       = {FogDiagonal_InitVars, FogDiagonal_Main, FogDiagonal_InitAll, FogDiagonal_Finish},
    [WEATHER_SHADE]              = {Shade_InitVars, Shade_Main, Shade_InitAll, Shade_Finish},
    [WEATHER_DOWNPOUR]           = {Downpour_InitVars, Thunderstorm_Main, Downpour_InitAll, Thunderstorm_Finish},
    [WEATHER_UNDERWATER_BUBBLES] = {Bubbles_InitVars, Bubbles_Main, Bubbles_InitAll, Bubbles_Finish},
    [WEATHER_CLOUDS]             = {Clouds_InitVars, Clouds_Main, Clouds_InitAll, Clouds_Finish},
    [WEATHER_SNOWSTORM]          = {Snowstorm_InitVars, Snowstorm_Main, Snowstorm_InitAll, Snowstorm_Finish},
    [WEATHER_DROUGHT]            = {Drought_InitVars, Drought_Main, Drought_InitAll, Drought_Finish},
    [WEATHER_FLYING_LEAVES]      = {FlyingLeaves_InitVars, FlyingLeaves_Main, FlyingLeaves_InitAll, FlyingLeaves_Finish},
};

static void (*const sWeatherPalStateFuncs[])(void) = {
    UpdateWeatherGammaShift,
    FadeInScreenWithWeather,
    DoNothing,
    DoNothing
};

// The drought weather effect uses a precalculated color lookup table. Presumably this
// is because the underlying color shift calculation is slow.
static const u16 sDroughtWeatherColors[][0x1000] =
{
    INCBIN_U16("graphics/weather/drought/colors_0.bin"),
    INCBIN_U16("graphics/weather/drought/colors_1.bin"),
    INCBIN_U16("graphics/weather/drought/colors_2.bin"),
    INCBIN_U16("graphics/weather/drought/colors_3.bin"),
    INCBIN_U16("graphics/weather/drought/colors_4.bin"),
    INCBIN_U16("graphics/weather/drought/colors_5.bin")
};

static const u8 sGammaShiftTable[19][32] =
{
    // 1
    0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
    0x0F, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
    // 2
    0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
    0x0E, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B,
    // 3
    0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x04, 0x05, 0x06, 0x07, 0x08, 0x08, 0x09, 0x0A, 0x0B, 0x0C,
    0x0D, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x11, 0x12, 0x13, 0x14, 0x15, 0x15, 0x16, 0x17, 0x18, 0x19,
    // 4
    0x00, 0x01, 0x02, 0x03, 0x04, 0x04, 0x05, 0x06, 0x07, 0x08, 0x08, 0x09, 0x0B, 0x0B, 0x0C, 0x0D,
    0x0E, 0x0E, 0x0F, 0x10, 0x11, 0x11, 0x12, 0x13, 0x14, 0x14, 0x15, 0x16, 0x17, 0x18, 0x18, 0x19,
    // 5
    0x01, 0x02, 0x03, 0x03, 0x04, 0x05, 0x06, 0x06, 0x07, 0x08, 0x09, 0x09, 0x0C, 0x0D, 0x0D, 0x0E,
    0x0F, 0x0F, 0x10, 0x11, 0x12, 0x12, 0x13, 0x14, 0x14, 0x15, 0x16, 0x17, 0x17, 0x18, 0x19, 0x19,
    // 6
    0x01, 0x02, 0x03, 0x04, 0x04, 0x05, 0x06, 0x07, 0x07, 0x08, 0x09, 0x0A, 0x0D, 0x0E, 0x0F, 0x0F,
    0x10, 0x11, 0x11, 0x12, 0x13, 0x13, 0x14, 0x14, 0x15, 0x16, 0x16, 0x17, 0x18, 0x18, 0x19, 0x1A,
    // 7
    0x01, 0x02, 0x03, 0x04, 0x04, 0x05, 0x06, 0x07, 0x07, 0x08, 0x09, 0x0A, 0x0F, 0x0F, 0x10, 0x10,
    0x11, 0x12, 0x12, 0x13, 0x13, 0x14, 0x15, 0x15, 0x16, 0x16, 0x17, 0x18, 0x18, 0x19, 0x1A, 0x1A,
    // 8
    0x01, 0x02, 0x03, 0x04, 0x04, 0x05, 0x06, 0x07, 0x07, 0x08, 0x09, 0x0A, 0x10, 0x10, 0x11, 0x12,
    0x12, 0x13, 0x13, 0x14, 0x14, 0x15, 0x15, 0x16, 0x17, 0x17, 0x18, 0x18, 0x19, 0x19, 0x1A, 0x1B,
    // 9
    0x01, 0x02, 0x03, 0x04, 0x04, 0x05, 0x06, 0x07, 0x08, 0x08, 0x09, 0x0A, 0x11, 0x12, 0x12, 0x13,
    0x13, 0x14, 0x14, 0x15, 0x15, 0x16, 0x16, 0x17, 0x17, 0x18, 0x18, 0x19, 0x19, 0x1A, 0x1A, 0x1B,
    // 10
    0x01, 0x02, 0x03, 0x04, 0x04, 0x05, 0x06, 0x07, 0x08, 0x08, 0x09, 0x0A, 0x13, 0x13, 0x13, 0x14,
    0x14, 0x15, 0x15, 0x16, 0x16, 0x17, 0x17, 0x18, 0x18, 0x18, 0x19, 0x19, 0x1A, 0x1A, 0x1B, 0x1B,
    // 11
    0x01, 0x02, 0x03, 0x04, 0x04, 0x05, 0x06, 0x07, 0x08, 0x08, 0x09, 0x0A, 0x14, 0x14, 0x15, 0x15,
    0x16, 0x16, 0x16, 0x17, 0x17, 0x18, 0x18, 0x18, 0x19, 0x19, 0x1A, 0x1A, 0x1A, 0x1B, 0x1B, 0x1C,
    // 12
    0x01, 0x02, 0x03, 0x04, 0x04, 0x05, 0x06, 0x07, 0x08, 0x08, 0x09, 0x0A, 0x15, 0x16, 0x16, 0x16,
    0x17, 0x17, 0x17, 0x18, 0x18, 0x18, 0x19, 0x19, 0x19, 0x1A, 0x1A, 0x1B, 0x1B, 0x1B, 0x1C, 0x1C,
    // 13
    0x01, 0x02, 0x03, 0x04, 0x04, 0x05, 0x06, 0x07, 0x08, 0x08, 0x09, 0x0A, 0x17, 0x17, 0x17, 0x17,
    0x18, 0x18, 0x18, 0x19, 0x19, 0x19, 0x1A, 0x1A, 0x1A, 0x1A, 0x1B, 0x1B, 0x1B, 0x1C, 0x1C, 0x1C,
    // 14
    0x01, 0x02, 0x03, 0x04, 0x04, 0x05, 0x06, 0x07, 0x08, 0x08, 0x09, 0x0A, 0x18, 0x18, 0x18, 0x19,
    0x19, 0x19, 0x19, 0x1A, 0x1A, 0x1A, 0x1A, 0x1B, 0x1B, 0x1B, 0x1B, 0x1C, 0x1C, 0x1C, 0x1C, 0x1D,
    // 15
    0x01, 0x02, 0x03, 0x04, 0x04, 0x05, 0x06, 0x07, 0x08, 0x08, 0x09, 0x0A, 0x19, 0x19, 0x1A, 0x1A,
    0x1A, 0x1A, 0x1A, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1D, 0x1D, 0x1D,
    // 16
    0x01, 0x02, 0x03, 0x04, 0x04, 0x05, 0x06, 0x07, 0x08, 0x08, 0x09, 0x0A, 0x1B, 0x1B, 0x1B, 0x1B,
    0x1B, 0x1B, 0x1B, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D,
    // 17
    0x01, 0x02, 0x03, 0x04, 0x04, 0x05, 0x06, 0x07, 0x08, 0x08, 0x09, 0x0A, 0x1C, 0x1C, 0x1C, 0x1C,
    0x1C, 0x1C, 0x1C, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1E, 0x1E, 0x1E,
    // 18
    0x01, 0x02, 0x03, 0x04, 0x04, 0x05, 0x06, 0x07, 0x08, 0x08, 0x09, 0x0A, 0x1D, 0x1D, 0x1D, 0x1D,
    0x1D, 0x1D, 0x1D, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
    // 19
    0x01, 0x02, 0x03, 0x04, 0x04, 0x05, 0x06, 0x07, 0x08, 0x08, 0x09, 0x0A, 0x1F, 0x1F, 0x1F, 0x1F,
    0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F
};

// code
void StartWeather(void)
{
    if (!FuncIsActiveTask(Task_WeatherMain))
    {
        LoadWeatherDefaultPalette(); // Make the palette slot used even if it not has an weather
        
        gWeatherPtr->rainSpriteCount = 0;
        gWeatherPtr->curRainSpriteIndex = 0;
        gWeatherPtr->snowflakeSpriteCount = 0;
        gWeatherPtr->ashSpritesCreated = FALSE;
        gWeatherPtr->fogHSpritesCreated = FALSE;
        gWeatherPtr->fogDSpritesCreated = FALSE;
        gWeatherPtr->sandstormSpritesCreated = FALSE;
        gWeatherPtr->sandstormSwirlSpritesCreated = FALSE;
        gWeatherPtr->bubblesSpritesCreated = FALSE;
        gWeatherPtr->cloudSpritesCreated = FALSE;
        gWeatherPtr->snowstormSpritesCreated = FALSE;
        gWeatherPtr->snowstormSwirlSpritesCreated = FALSE;
        gWeatherPtr->flyingLeavesSpritesCreated = FALSE;
        gWeatherPtr->currWeather = WEATHER_NONE;
        gWeatherPtr->palProcessingState = WEATHER_PAL_STATE_IDLE;
        gWeatherPtr->readyForInit = FALSE;
        gWeatherPtr->weatherChangeComplete = TRUE;
        Weather_SetBlendCoeffs(16, 0);
        
        CreateTask(Task_WeatherInit, 80);
    }
}

void SetNextWeather(u32 weather)
{
    if (weather != WEATHER_RAIN && weather != WEATHER_RAIN_THUNDERSTORM && weather != WEATHER_DOWNPOUR)
        PlayRainStoppingSoundEffect();

    if (gWeatherPtr->nextWeather != weather && gWeatherPtr->currWeather == weather)
        sWeatherFuncs[weather].initVars();

    gWeatherPtr->weatherChangeComplete = FALSE;
    gWeatherPtr->nextWeather = weather;
    gWeatherPtr->finishStep = 0;
}

void SetCurrentAndNextWeather(u32 weather)
{
    PlayRainStoppingSoundEffect();
    gWeatherPtr->currWeather = weather;
    gWeatherPtr->nextWeather = weather;
}

static void Task_WeatherInit(u32 taskId)
{
    // Waits until it's ok to initialize weather.
    // When the screen fades in, this is set to TRUE.
    if (gWeatherPtr->readyForInit)
    {
        UpdateCameraPanning();
        sWeatherFuncs[gWeatherPtr->currWeather].initAll();
        gTasks[taskId].func = Task_WeatherMain;
    }
}

static void Task_WeatherMain(u32 taskId)
{
    if (gWeatherPtr->currWeather != gWeatherPtr->nextWeather)
    {
        if (!sWeatherFuncs[gWeatherPtr->currWeather].finish())
        {
            // Finished cleaning up previous weather. Now transition to next weather.
            sWeatherFuncs[gWeatherPtr->nextWeather].initVars();
            gWeatherPtr->gammaStepFrameCounter = 0;
            gWeatherPtr->palProcessingState = WEATHER_PAL_STATE_CHANGING_WEATHER;
            gWeatherPtr->currWeather = gWeatherPtr->nextWeather;
            gWeatherPtr->weatherChangeComplete = TRUE;
        }
    }
    else
        sWeatherFuncs[gWeatherPtr->currWeather].main();

    sWeatherPalStateFuncs[gWeatherPtr->palProcessingState]();
}


static void None_Init(void)
{
    gWeatherPtr->noShadows = FALSE;
    gWeatherPtr->gammaTargetIndex = 0;
    gWeatherPtr->gammaStepDelay = 0;
    Weather_SetBlendCoeffs(8, BASE_SHADOW_INTENSITY);
}

static void None_Main(void)
{
}

static bool32 None_Finish(void)
{
    return FALSE;
}

// When the weather is changing, it gradually updates the palettes
// towards the desired gamma shift.
static void UpdateWeatherGammaShift(void)
{
    if (gWeatherPtr->gammaIndex == gWeatherPtr->gammaTargetIndex)
        gWeatherPtr->palProcessingState = WEATHER_PAL_STATE_IDLE;
    else
    {
        if (++gWeatherPtr->gammaStepFrameCounter >= gWeatherPtr->gammaStepDelay)
        {
            gWeatherPtr->gammaStepFrameCounter = 0;
            if (gWeatherPtr->gammaIndex < gWeatherPtr->gammaTargetIndex)
                gWeatherPtr->gammaIndex++;
            else
                gWeatherPtr->gammaIndex--;

            ApplyGammaShift(GAMMA_NORMAL, 0, 32, gWeatherPtr->gammaIndex);
        }
    }
}

static void FadeInScreenWithWeather(void)
{
    if (++gWeatherPtr->fadeInCounter > 1)
        gWeatherPtr->fadeInActive = FALSE;

    switch (gWeatherPtr->currWeather)
    {
    case WEATHER_RAIN:
    case WEATHER_RAIN_THUNDERSTORM:
    case WEATHER_DOWNPOUR:
    case WEATHER_SNOW:
    case WEATHER_SHADE:
        if (!FadeInScreen_RainSnowShade())
        {
            gWeatherPtr->gammaIndex = 3;
            gWeatherPtr->palProcessingState = WEATHER_PAL_STATE_IDLE;
        }
        break;
    case WEATHER_DROUGHT:
        if (!FadeInScreen_Drought())
        {
            gWeatherPtr->gammaIndex = -6;
            gWeatherPtr->palProcessingState = WEATHER_PAL_STATE_IDLE;
        }
        break;
    default:
        if (!gPaletteFade.active)
        {
            gWeatherPtr->gammaIndex = gWeatherPtr->gammaTargetIndex;
            gWeatherPtr->palProcessingState = WEATHER_PAL_STATE_IDLE;
        }
        break;
    }
}

static bool32 FadeInScreen_RainSnowShade(void)
{
    if (gWeatherPtr->fadeScreenCounter == 16)
        return FALSE;

    if (++gWeatherPtr->fadeScreenCounter >= 16)
    {
        ApplyGammaShift(GAMMA_NORMAL, 0, 32, 3);
        gWeatherPtr->fadeScreenCounter = 16;
        return FALSE;
    }
    ApplyGammaShift(GAMMA_BLEND, 0, 32, 3);
    return TRUE;
}

static bool32 FadeInScreen_Drought(void)
{
    if (gWeatherPtr->fadeScreenCounter == 16)
        return FALSE;

    if (++gWeatherPtr->fadeScreenCounter >= 16)
    {
        ApplyGammaShift(GAMMA_NORMAL, 0, 32, -6);
        gWeatherPtr->fadeScreenCounter = 16;
        return FALSE;
    }
    ApplyGammaShift(GAMMA_BLEND, 0, 32, -6);
    return TRUE;
}

static void DoNothing(void)
{ }

static void ApplyGammaShift(u32 gammaType, u32 startPalIndex, u32 numPalettes, s8 gammaIndex)
{
    u32 i, offset, curPalIndex = startPalIndex;
    u8 r, g, b, blendCoeff;
    u32 fadeColor, palOffset = startPalIndex * 16;
    
    numPalettes += startPalIndex;
    
    switch (gammaType)
    {
        case GAMMA_NORMAL: // Apply gamma shift to colors.
            if (gammaIndex > 0)
            {
                --gammaIndex;
                
                // Loop through the speficied palette range and apply necessary gamma shifts to the colors.
                while (curPalIndex < numPalettes)
                {
                    if (curPalIndex < 13) // Excludes palettes 13, 14, 15 and sprite palettes.
                    {
                        for (i = 0; i < 16; i++)
                        {
                            // Apply gamma shift to the original color.
                            r = sGammaShiftTable[gammaIndex][GET_R(gPlttBufferUnfaded[palOffset])];
                            g = sGammaShiftTable[gammaIndex][GET_G(gPlttBufferUnfaded[palOffset])];
                            b = sGammaShiftTable[gammaIndex][GET_B(gPlttBufferUnfaded[palOffset])];
                            
                            gPlttBufferFaded[palOffset++] = RGB2(r, g, b);
                        }
                    }
                    else
                    {
                        // No palette change.
                        CpuFastCopy(gPlttBufferUnfaded + palOffset, gPlttBufferFaded + palOffset, 16 * sizeof(u16));
                        palOffset += 16;
                    }
                    curPalIndex++;
                }
            }
            // A negative gammIndex value means that the blending will come from the special Drought weather's palette tables.
            else if (gammaIndex < 0)
            {
                gammaIndex = -gammaIndex - 1;
                
                // Loop through the speficied palette range and apply necessary gamma shifts to the colors.
                while (curPalIndex < numPalettes)
                {
                    if (curPalIndex < 13) // Excludes palettes 13, 14, 15 and sprite palettes.
                    {
                        for (i = 0; i < 16; i++)
                            gPlttBufferFaded[palOffset++] = sDroughtWeatherColors[gammaIndex][DROUGHT_COLOR_INDEX(gPlttBufferUnfaded[palOffset])];
                    }
                    else
                    {
                        // No palette change.
                        CpuFastCopy(gPlttBufferUnfaded + palOffset, gPlttBufferFaded + palOffset, 16 * sizeof(u16));
                        palOffset += 16;
                    }
                    curPalIndex++;
                }
            }
            else
            {
                // No palette blending.
                CpuFastCopy(gPlttBufferUnfaded + startPalIndex * 16, gPlttBufferFaded + startPalIndex * 16, numPalettes * 16 * sizeof(u16));
            }
            break;
        case GAMMA_BLEND: // Apply gamma shift and blend effect to colors.
            blendCoeff = 16 - gWeatherPtr->fadeScreenCounter;
            fadeColor = gWeatherPtr->fadeDestColor;
            
            if (gammaIndex != 0)
            {
                if (gammaIndex > 0)
                {
                    --gammaIndex;
                    
                    while (curPalIndex < numPalettes)
                    {
                        if (curPalIndex < 13) // Excludes palettes 13, 14, 15 and sprite palettes.
                        {
                            for (i = 0; i < 16; i++)
                            {
                                // Apply gamma shift to the original color.
                                r = sGammaShiftTable[gammaIndex][GET_R(gPlttBufferUnfaded[palOffset])];
                                g = sGammaShiftTable[gammaIndex][GET_G(gPlttBufferUnfaded[palOffset])];
                                b = sGammaShiftTable[gammaIndex][GET_B(gPlttBufferUnfaded[palOffset])];
                                
                                // Apply target blend color to the original color.
                                r += ((GET_R(fadeColor) - r) * blendCoeff) >> 4;
                                g += ((GET_G(fadeColor) - g) * blendCoeff) >> 4;
                                b += ((GET_B(fadeColor) - b) * blendCoeff) >> 4;
                                
                                gPlttBufferFaded[palOffset++] = RGB2(r, g, b);
                            }
                        }
                        else
                        {
                            // No gamma shift. Simply blend the colors.
                            BlendPalette(palOffset, 16, blendCoeff, fadeColor);
                            palOffset += 16;
                        }
                        curPalIndex++;
                    }
                }
                else // Drought blend
                {
                    gammaIndex = -gammaIndex - 1;
                    
                    while (curPalIndex < numPalettes)
                    {
                        if (curPalIndex < 13) // Excludes palettes 13, 14, 15 and sprite palettes.
                        {
                            for (i = 0; i < 16; i++)
                            {
                                r = GET_R(gPlttBufferUnfaded[palOffset]);
                                g = GET_G(gPlttBufferUnfaded[palOffset]);
                                b = GET_B(gPlttBufferUnfaded[palOffset]);
                                
                                offset = ((b & 0x1E) << 7) | ((g & 0x1E) << 3) | ((r & 0x1E) >> 1);
                                r = GET_R(sDroughtWeatherColors[gammaIndex][offset]);
                                g = GET_G(sDroughtWeatherColors[gammaIndex][offset]);
                                b = GET_B(sDroughtWeatherColors[gammaIndex][offset]);
                                
                                // Apply target blend color to the original color.
                                r += ((GET_R(fadeColor) - r) * blendCoeff) >> 4;
                                g += ((GET_G(fadeColor) - g) * blendCoeff) >> 4;
                                b += ((GET_B(fadeColor) - b) * blendCoeff) >> 4;
                                
                                gPlttBufferFaded[palOffset++] = RGB2(r, g, b);
                            }
                        }
                        else
                        {
                            // No gamma shift. Simply blend the colors.
                            BlendPalette(palOffset, 16, blendCoeff, fadeColor);
                            palOffset += 16;
                        }
                        curPalIndex++;
                    }
                }
            }
            break;
    }
}

void WeatherShiftGammaIfPalStateIdle(s8 gammaIndex)
{
    if (gWeatherPtr->palProcessingState == WEATHER_PAL_STATE_IDLE)
    {
        ApplyGammaShift(GAMMA_NORMAL, 0, 32, gammaIndex);
        gWeatherPtr->gammaIndex = gammaIndex;
    }
}

void WeatherBeginGammaFade(u32 gammaIndex, u32 gammaTargetIndex, u32 gammaStepDelay)
{
    if (gWeatherPtr->palProcessingState == WEATHER_PAL_STATE_IDLE)
    {
        gWeatherPtr->palProcessingState = WEATHER_PAL_STATE_CHANGING_WEATHER;
        gWeatherPtr->gammaIndex = gammaIndex;
        gWeatherPtr->gammaTargetIndex = gammaTargetIndex;
        gWeatherPtr->gammaStepFrameCounter = 0;
        gWeatherPtr->gammaStepDelay = gammaStepDelay;
        WeatherShiftGammaIfPalStateIdle(gammaIndex);
    }
}

void FadeScreen(u32 mode, s8 delay)
{
    FadeSelectedPals(mode, delay, PALETTES_ALL);
}

void FadeSelectedPals(u32 mode, s8 delay, u32 selectedPalettes)
{
    u32 fadeColor;
    bool32 fadeOut, useWeatherPal;

    switch (mode)
    {
    case FADE_FROM_BLACK:
        fadeColor = RGB_BLACK;
        fadeOut = FALSE;
        break;
    case FADE_FROM_WHITE:
        fadeColor = RGB_WHITEALPHA;
        fadeOut = FALSE;
        break;
    case FADE_TO_BLACK:
        fadeColor = RGB_BLACK;
        fadeOut = TRUE;
        break;
    case FADE_TO_WHITE:
        fadeColor = RGB_WHITEALPHA;
        fadeOut = TRUE;
        break;
    default:
        return;
    }

    switch (gWeatherPtr->currWeather)
    {
    case WEATHER_RAIN:
    case WEATHER_RAIN_THUNDERSTORM:
    case WEATHER_DOWNPOUR:
    case WEATHER_SNOW:
    case WEATHER_SHADE:
    case WEATHER_DROUGHT:
        useWeatherPal = TRUE;
        break;
    default:
        useWeatherPal = FALSE;
        break;
    }

    if (fadeOut)
    {
        if (useWeatherPal)
            CpuFastCopy(gPlttBufferFaded, gPlttBufferUnfaded, PLTT_SIZE);

        BeginNormalPaletteFade(selectedPalettes, delay, 0, 16, fadeColor);
        gWeatherPtr->palProcessingState = WEATHER_PAL_STATE_SCREEN_FADING_OUT;
    }
    else
    {
        gWeatherPtr->fadeDestColor = fadeColor;
        
        if (useWeatherPal)
            gWeatherPtr->fadeScreenCounter = 0;
        else
            BeginNormalPaletteFade(selectedPalettes, delay, 16, 0, fadeColor);

        gWeatherPtr->palProcessingState = WEATHER_PAL_STATE_SCREEN_FADING_IN;
        gWeatherPtr->fadeInActive = TRUE;
        gWeatherPtr->fadeInCounter = 0;
        Weather_SetBlendCoeffs(gWeatherPtr->currBlendEVA, gWeatherPtr->currBlendEVB);
        gWeatherPtr->readyForInit = TRUE;
    }
}

bool32 IsWeatherNotFadingIn(void)
{
    return (gWeatherPtr->palProcessingState != WEATHER_PAL_STATE_SCREEN_FADING_IN);
}

void UpdateSpritePaletteWithWeather(u32 spritePaletteIndex)
{
    u32 paletteIndex = 16 + spritePaletteIndex;
    u32 i;

    switch (gWeatherPtr->palProcessingState)
    {
    case WEATHER_PAL_STATE_SCREEN_FADING_IN:
        if (gWeatherPtr->fadeInActive)
        {
            paletteIndex *= 16;
            for (i = 0; i < 16; i++)
                gPlttBufferFaded[paletteIndex + i] = gWeatherPtr->fadeDestColor;
        }
        break;
    case WEATHER_PAL_STATE_SCREEN_FADING_OUT:
        paletteIndex *= 16;
        CpuFastCopy(gPlttBufferFaded + paletteIndex, gPlttBufferUnfaded + paletteIndex, 32);
        BlendPalette(paletteIndex, 16, gPaletteFade.y, gPaletteFade.blendColor);
        break;
    default:
        ApplyWeatherGammaShiftToPal(paletteIndex);
        break;
    }
}

void ApplyWeatherGammaShiftToPal(u32 paletteIndex)
{
    ApplyGammaShift(GAMMA_NORMAL, paletteIndex, 1, gWeatherPtr->gammaIndex);
}

void LoadWeatherDefaultPalette(void)
{
    LoadWeatherSpritePalette(&sDefaultWeatherSpritePalette);
}

void LoadWeatherSpritePalette(const struct SpritePalette *palette)
{
    LoadSpritePaletteAtIndex(palette, WEATHER_PALETTE_INDEX);
    UpdateSpritePaletteWithWeather(WEATHER_PALETTE_INDEX);
}

void Weather_SetBlendCoeffs(u32 eva, u32 evb)
{
    if (ForestMapPreviewScreenIsRunning())
    {
        eva = 16;
        evb = 0;
    }
    gWeatherPtr->currBlendEVA = eva;
    gWeatherPtr->currBlendEVB = evb;
    gWeatherPtr->targetBlendEVA = eva;
    gWeatherPtr->targetBlendEVB = evb;
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(eva, evb));
}

void Weather_SetTargetBlendCoeffs(u32 eva, u32 evb, int delay)
{
    gWeatherPtr->targetBlendEVA = eva;
    gWeatherPtr->targetBlendEVB = evb;
    gWeatherPtr->blendDelay = delay;
    gWeatherPtr->blendFrameCounter = 0;
    gWeatherPtr->blendUpdateCounter = 0;
}

bool32 Weather_UpdateBlend(void)
{
    if (gWeatherPtr->currBlendEVA == gWeatherPtr->targetBlendEVA && gWeatherPtr->currBlendEVB == gWeatherPtr->targetBlendEVB)
        return TRUE;

    if (++gWeatherPtr->blendFrameCounter > gWeatherPtr->blendDelay)
    {
        gWeatherPtr->blendFrameCounter = 0;
        gWeatherPtr->blendUpdateCounter++;

        // Update currBlendEVA and currBlendEVB on alternate frames
        if (gWeatherPtr->blendUpdateCounter & 1)
        {
            if (gWeatherPtr->currBlendEVA < gWeatherPtr->targetBlendEVA)
                gWeatherPtr->currBlendEVA++;
            else if (gWeatherPtr->currBlendEVA > gWeatherPtr->targetBlendEVA)
                gWeatherPtr->currBlendEVA--;
        }
        else
        {
            if (gWeatherPtr->currBlendEVB < gWeatherPtr->targetBlendEVB)
                gWeatherPtr->currBlendEVB++;
            else if (gWeatherPtr->currBlendEVB > gWeatherPtr->targetBlendEVB)
                gWeatherPtr->currBlendEVB--;
        }
    }
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gWeatherPtr->currBlendEVA, gWeatherPtr->currBlendEVB));

    if (gWeatherPtr->currBlendEVA == gWeatherPtr->targetBlendEVA && gWeatherPtr->currBlendEVB == gWeatherPtr->targetBlendEVB)
        return TRUE;

    return FALSE;
}

u32 GetCurrentWeather(void)
{
    return gWeatherPtr->currWeather;
}

void SetRainStrengthFromSoundEffect(u32 soundEffect)
{
    if (gWeatherPtr->palProcessingState != WEATHER_PAL_STATE_SCREEN_FADING_OUT)
    {
        switch (soundEffect)
        {
        case SE_RAIN:
            gWeatherPtr->rainStrength = 0;
            break;
        case SE_DOWNPOUR:
            gWeatherPtr->rainStrength = 1;
            break;
        case SE_THUNDERSTORM:
            gWeatherPtr->rainStrength = 2;
            break;
        default:
            return;
        }
        PlaySE(soundEffect);
    }
}

void PlayRainStoppingSoundEffect(void)
{
    if (IsSpecialSEPlaying())
    {
        switch (gWeatherPtr->rainStrength)
        {
        case 0:
            PlaySE(SE_RAIN_STOP);
            break;
        case 1:
            PlaySE(SE_DOWNPOUR_STOP);
            break;
        case 2:
            PlaySE(SE_THUNDERSTORM_STOP);
            break;
        }
    }
}

void SetWeatherScreenFadeOut(void)
{
    gWeatherPtr->palProcessingState = WEATHER_PAL_STATE_SCREEN_FADING_OUT;
}

void WeatherProcessingIdle(void)
{
    gWeatherPtr->palProcessingState = WEATHER_PAL_STATE_IDLE;
}

u32 GetShadowColor(void)
{
    return gPlttBufferFaded[((WEATHER_PALETTE_INDEX + 16) * 16) + SHADOW_COLOR_INDEX];
}

void UpdateShadowColor(u32 color)
{
    u32 index = ((WEATHER_PALETTE_INDEX + 16) * 16) + SHADOW_COLOR_INDEX;
    gPlttBufferUnfaded[index] = gPlttBufferFaded[index] = color;
}

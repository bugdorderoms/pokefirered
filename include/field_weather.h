#ifndef GUARD_WEATHER_H
#define GUARD_WEATHER_H

#include "global.h"
#include "constants/field_weather.h"

#define TAG_WEATHER_START 0x1200
#define PALTAG_WEATHER TAG_WEATHER_START

enum
{
	GFXTAG_RAIN = TAG_WEATHER_START,
    GFXTAG_FOG_H,
    GFXTAG_ASH,
    GFXTAG_SANDSTORM,
	GFXTAG_FOG_D,
    GFXTAG_BUBBLE,
	GFXTAG_CLOUD,
};

struct Weather
{
	u16 readyForInit:1;
	u16 fadeInActive:1;
	u16 fadeInCounter:2;
	u16 weatherGfxLoaded:1;
	u16 weatherChangeComplete:1;
	u16 rainStrength:2;
	u16 thunderTriggered:1;
	u16 thunderAllowEnd:1;
	u16 thunderSkipShort:1;
	u16 fogHSpritesCreated:1;
	u16 ashSpritesCreated:1;
	u16 sandstormSpritesCreated:1;
    u16 sandstormSwirlSpritesCreated:1;
	u16 fogDSpritesCreated:1;
	u8 isDownpour:1;
	u8 bubblesSpritesCreated:1;
	u8 cloudSpritesCreated:1;
	u8 unused:5; // can be used to add new weathers
	u8 taskId;
	
	u8 palProcessingState;
	u8 fadeScreenCounter;
	u16 fadeDestColor;
	
	u8 initStep;
	u8 finishStep;
	u8 currWeather;
    u8 nextWeather;
	
	s8 gammaIndex;
    s8 gammaTargetIndex;
    u8 gammaStepDelay;
    u8 gammaStepFrameCounter;
	
	u16 currBlendEVA;
    u16 currBlendEVB;
	
    u16 targetBlendEVA;
    u16 targetBlendEVB;
	
	u8 blendDelay;
	u8 blendUpdateCounter;
    u8 blendFrameCounter;
	u8 targetRainSpriteCount;
	
	u8 rainSpriteCount;
	u8 rainSpriteVisibleDelay;
	u16 rainSpriteVisibleCounter;
	
	u8 curRainSpriteIndex;
	u8 targetSnowflakeSpriteCount;
	u16 snowflakeVisibleCounter;
	
	u16 snowflakeTimer;
	u8 snowflakeSpriteCount;
	u8 thunderShortRetries;
	
	u16 thunderDelay;
	u16 thunderCounter;
	
	u16 fogHScrollCounter;
	u16 fogHScrollOffset;
	
	u16 fogHScrollPosX;
	u16 ashBaseSpritesX;
	
	u32 sandstormXOffset;
	
    u32 sandstormYOffset;
	
	u16 sandstormWaveCounter;
	u16 sandstormBaseSpritesX;
	
	u16 sandstormPosY;
	u16 sandstormWaveIndex;
	
	u16 fogDScrollXCounter;
    u16 fogDScrollYCounter;
	
	u16 fogDXOffset;
    u16 fogDYOffset;
	
	u16 fogDBaseSpritesX;
    u16 fogDPosY;
	
	u16 bubblesDelayIndex;
	u16 bubblesDelayCounter;
	
	u16 bubblesCoordsIndex;
    u16 bubblesSpriteCount;
	
	struct Sprite *rainSprites[NUM_RAIN_SPRITES];
	struct Sprite *snowflakeSprites[NUM_SNOWFLAKE_SPRITES];
	struct Sprite *fogHSprites[NUM_FOG_HORIZONTAL_SPRITES];
	struct Sprite *ashSprites[NUM_ASH_SPRITES];
	struct Sprite *sandstormSprites1[NUM_SANDSTORM_SPRITES];
	struct Sprite *sandstormSprites2[NUM_SWIRL_SANDSTORM_SPRITES];
	struct Sprite *fogDSprites[NUM_FOG_DIAGONAL_SPRITES];
	struct Sprite *cloudSprites[NUM_CLOUD_SPRITES];
};

extern struct Weather *const gWeatherPtr;

void FadeScreen(u8 mode, s8 delay);
void FadeSelectedPals(u8 mode, s8 delay, u32 selectedPalettes);
void SetSav1Weather(u8 weather);
void SetNextWeather(u8 weather);
void SetCurrentAndNextWeather(u8 weather);
void DoCurrentWeather(void);
void SetSav1WeatherFromCurrMapHeader(void);
void PlayRainStoppingSoundEffect(void);
u8 GetCurrentWeather(void);
void StartWeather(void);
void ResumePausedWeather(void);
bool8 IsWeatherNotFadingIn(void);
void SetWeatherScreenFadeOut(void);
void WeatherProcessingIdle(void);
void UpdateSpritePaletteWithWeather(u8 palIdx);
void Weather_SetBlendCoeffs(u8 eva, u8 evb);
void Weather_SetTargetBlendCoeffs(u8 eva, u8 evb, int delay);
bool8 Weather_UpdateBlend(void);
void LoadCustomWeatherSpritePalette(const struct SpritePalette *palette);
void SetRainStrengthFromSoundEffect(u16 soundEffect);
void WeatherShiftGammaIfPalStateIdle(s8 gammaIndex);
void WeatherBeginGammaFade(u8 gammaIndex, u8 gammaTargetIndex, u8 gammaStepDelay);
void ApplyWeatherGammaShiftToPal(u8 paletteIndex);

extern const u16 gSandstormWeatherPalette[];
extern const u16 gCloudWeatherPalette[];
extern const u8 gWeatherFogDiagonalTiles[];
extern const u8 gWeatherFogHorizontalTiles[];
extern const u8 gWeatherSnow1Tiles[];
extern const u8 gWeatherSnow2Tiles[];
extern const u8 gWeatherBubbleTiles[];
extern const u8 gWeatherAshTiles[];
extern const u8 gWeatherRainTiles[];
extern const u8 gWeatherSandstormTiles[];
extern const u8 gWeatherCloudTiles[];

#endif // GUARD_WEATHER_H

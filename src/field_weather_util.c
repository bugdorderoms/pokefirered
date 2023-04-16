#include "global.h"
#include "field_weather.h"
#include "overworld.h"
#include "constants/weather.h"

static void UpdateRainCounter(u8 newWeather, u8 oldWeather)
{
    if (newWeather != oldWeather && (newWeather == WEATHER_RAIN || newWeather == WEATHER_RAIN_THUNDERSTORM))
        IncrementGameStat(GAME_STAT_GOT_RAINED_ON);
}

void SetSav1Weather(u8 weather)
{
    u8 oldWeather = gSaveBlock1Ptr->weather;
    gSaveBlock1Ptr->weather = weather;
    UpdateRainCounter(gSaveBlock1Ptr->weather, oldWeather);
}

void SetSav1WeatherFromCurrMapHeader(void)
{
    u8 oldWeather = gSaveBlock1Ptr->weather;
    gSaveBlock1Ptr->weather = gMapHeader.weather;
    UpdateRainCounter(gSaveBlock1Ptr->weather, oldWeather);
}

void DoCurrentWeather(void)
{
    SetNextWeather(gSaveBlock1Ptr->weather);
}

void ResumePausedWeather(void)
{
    SetCurrentAndNextWeather(gSaveBlock1Ptr->weather);
}

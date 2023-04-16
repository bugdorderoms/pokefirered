#include "global.h"
#include "constants/weather.h"

// These were all dummied out because it's always sunny in Viridian
void WeatherCoordEvent_Rain(void) {}
void WeatherCoordEvent_Snow(void) {}
void WeatherCoordEvent_RainThunderstorm(void) {}
void WeatherCoordEvent_FogHorizontal(void) {}
void WeatherCoordEvent_VolcanicAsh(void) {}
void WeatherCoordEvent_Sandstorm(void) {}
void WeatherCoordEvent_FogDiagonal(void) {}
void WeatherCoordEvent_Shade(void) {}

static struct {
    u8 weatherId;
    void (*callback)(void);
} const sWeatherCoordEventFuncs[] = {
    {WEATHER_RAIN,              WeatherCoordEvent_Rain            },
    {WEATHER_SNOW,              WeatherCoordEvent_Snow            },
    {WEATHER_RAIN_THUNDERSTORM, WeatherCoordEvent_RainThunderstorm},
    {WEATHER_FOG_HORIZONTAL,    WeatherCoordEvent_FogHorizontal   },
    {WEATHER_VOLCANIC_ASH,      WeatherCoordEvent_VolcanicAsh     },
    {WEATHER_SANDSTORM,         WeatherCoordEvent_Sandstorm       },
    {WEATHER_FOG_DIAGONAL,      WeatherCoordEvent_FogDiagonal     },
    {WEATHER_SHADE,             WeatherCoordEvent_Shade           }
};

void DoCoordEventWeather(u8 weatherId)
{
    u8 i;
    for (i = 0; i < NELEMS(sWeatherCoordEventFuncs); i++)
    {
        if (sWeatherCoordEventFuncs[i].weatherId == weatherId)
        {
            sWeatherCoordEventFuncs[i].callback();
            return;
        }
    }
}

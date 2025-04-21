#include "global.h"
#include "dns.h"
#include "field_weather.h"
#include "overworld.h"
#include "random.h"
#include "constants/weather.h"

// every time the player changes map weatherDelay is incremented, if weatherDelay counter reach the number bellow random weathers can occours.
// So, the player need change map "X" times to have a chance to activate it.
#define RANDOM_WEATHER_DELAY 40

#if DYNAMIC_WEATHER
static u32 TryStartDynamicWeather(void)
{
	u32 weatherChance;
	u32 newWeather = gSaveBlock1Ptr->weather;
	
	if (++gSaveBlock1Ptr->weatherDelay == RANDOM_WEATHER_DELAY) // Wait weather delay
	{
		gSaveBlock1Ptr->weatherDelay = 0;
		
		// only start a random weather if map's weather is WEATHER_NONE or WEATHER_SHADE
	    if (!IsMapDNSException() && (newWeather == WEATHER_NONE || newWeather == WEATHER_SHADE))
	    {
	    	newWeather = WEATHER_RAIN; // Default new weather
	    	weatherChance = 10; // Default chance
	    	
	    	// Get chance modification by season
	    	switch (DNSGetCurrentSeason())
	    	{
	    		case SEASON_SUMMER:
	    		case SEASON_SPRING:
	    		    weatherChance += 10; // +10% chance
	    			
	    			if (RandomPercent(5)) // 5% chance that becames a heavy rain
	    				newWeather = WEATHER_RAIN_THUNDERSTORM;
	    			break;
	    		case SEASON_WINTER:
				    weatherChance = 100; // Set default chance to 100%
					
					// Always init a snow or snowstorm
					if (RandomPercent(40)) // 40% chance that becames a snowstorm
						newWeather = WEATHER_SNOWSTORM;
					else
						newWeather = WEATHER_SNOW;
	    			break;
	    		case SEASON_AUTUMN:
	    		    weatherChance = 5; // Set default chance to 5%
	    			
	    			if (RandomPercent(2)) // 2% chance that becames a heavy rain
	    			    newWeather = WEATHER_RAIN_THUNDERSTORM;
	    			break;
	    	}
	    	
	    	// Snow and snowstorm weathers are't affected by the time of day
	    	if (newWeather != WEATHER_SNOW && newWeather != WEATHER_SNOWSTORM)
	    	{
	    		// Get chance modification by time of day
	    	    switch (GetDNSTimeLapse())
	    	    {
					case TIME_DAY:
						if (RandomPercent(65)) // 65% chance starts a drought
						{
							newWeather = WEATHER_DROUGHT;
							weatherChance = 50; // Set default chance to 50%
						}
						break;
	    	    	case TIME_NIGHTFALL:
	    	    	    weatherChance += 4; // +4% chance
	    	    		break;
	    	    	case TIME_NIGHT:
	    	    	    weatherChance += 7; // +7% chance
	    	    		break;
	    	    	case TIME_MIDNIGHT:
	    	    	    weatherChance += 10; // +10% chance
	    	    		break;
	    	    }
	    	}
	    	
	    	// Final calculation
	    	if (newWeather != WEATHER_NONE && RandomPercent(weatherChance))
	    		gSaveBlock1Ptr->weather = newWeather; // Apply new weather
	    	else
	    		newWeather = gSaveBlock1Ptr->weather; // Failed to apply
	    }
	}
	return newWeather;
}
#endif

static void UpdateRainCounter(u32 newWeather, u32 oldWeather)
{
    if (newWeather != oldWeather && (newWeather == WEATHER_RAIN || newWeather == WEATHER_RAIN_THUNDERSTORM))
        IncrementGameStat(GAME_STAT_GOT_RAINED_ON);
}

void SetSav1Weather(u32 weather)
{
    u32 oldWeather = gSaveBlock1Ptr->weather;
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
#if DYNAMIC_WEATHER
    SetCurrentAndNextWeather(TryStartDynamicWeather());
#else
	SetCurrentAndNextWeather(gSaveBlock1Ptr->weather);
#endif
}

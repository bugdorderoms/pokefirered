#include "global.h"
#include "battle.h"
#include "task.h"
#include "main.h"
#include "dns.h"
#include "rtc.h"
#include "fieldmap.h"
#include "overworld.h"
#include "palette.h"
#include "pokedex_screen.h"
#include "battle_main.h"
#include "constants/battle.h"
#include "constants/map_types.h"

/*******************************************************/
/*********    Day and Night Configuration     **********/
/********************************************************
 * You can customize the DNS by editing the following   *
 * timelapses and the filters used to change the        *
 * palette colours.                                     *
 * In addition to that, you can also configure which    *
 * palettes are affected by the system, as well as      *
 * establishing sprite palettes exceptions by its TAG.  *
 * 
 * It is highly recommended to read the following config*
 * options, to understand how the dns works.            *
 * ******************************************************/

/* This array contains the colours used for the windows or other    *
 * tiles that have to be illuminated at night.                      *
 * You can add or remove light slots as you whish, each entry       *
 * requires the paletteNum and the colourNum of each colour slot,   *
 * as well as the RGB 15 bit colour that's gonna be used as         *
 * "light colour".                                                  */
static const struct lightingColour sLightingColours[] =
{
    {
        .paletteNum = 0,
        .colourNum = 1,
        .lightColour = RGB2(30, 30, 5),
    },
    {
        .paletteNum = 0,
        .colourNum = 2,
        .lightColour = RGB2(26, 25, 4),
    },
    {
        .paletteNum = 0,
        .colourNum = 3,
        .lightColour = RGB2(22, 21, 3),
    },
    {
        .paletteNum = 1,
        .colourNum = 1,
        .lightColour = RGB2(30, 30, 5),
    },
    {
        .paletteNum = 1,
        .colourNum = 2,
        .lightColour = RGB2(26, 25, 4),
    },
    {
        .paletteNum = 6,
        .colourNum = 1,
        .lightColour = RGB2(30, 30, 5),
    },
    {
        .paletteNum = 6,
        .colourNum = 2,
        .lightColour = RGB2(26, 25, 4),
    },
    {
        .paletteNum = 6,
        .colourNum = 3,
        .lightColour = RGB2(22, 21, 3),
    },
};

/* Maptypes that are not affected by DNS */
static const u8 sDNSMapExceptions[] =
{
	MAP_TYPE_NONE,
	MAP_TYPE_INDOOR,
	MAP_TYPE_UNDERGROUND,
	MAP_TYPE_SECRET_BASE,
	MAP_TYPE_UNDERWATER
};

/*******************************************************/
/*************    DNS Colour Filters     ***************/
/*******************************************************/
/* DNS filters are actual 15bit RGB colours.            *
 * This colours R - G - B channels are substracted from *
 * the level of each channel porportionally.            *
 *                                                      *
 *  [BUFFER] -> (Value - Filter) -> [PAL_RAM]           *
 *                                                      *
 * This means that you shouln't use too high values for *
 * RGB channels in the filters. Otherwie, the channels  *
 * will easily reach 0, giving you plain colours.       *
 * I Suggest to not use channels with a value above 16. *
 *                                                      *
 * Feel free to experiment with your own filters.       *
 * ******************************************************

/* Filters used at midnight.                    *
 * From 00:00 to 00:59 filters are cycled every *
 * 8 minutes.                                   *
 * From 01:00 to 03:59 the last filter is used. */
static const u16 sMidnightFilters[] =
{
    RGB2(14, 14, 6),    //CE19
    RGB2(14, 14, 7),    //CE1D
    RGB2(14, 14, 8),    //CE21
    RGB2(15, 15, 8),    //EF21
    RGB2(15, 15, 9),    //EF25
    RGB2(15, 15, 9),    //EF25
    RGB2(16, 16, 9),    //1026
    RGB2(16, 16, 10),   //102A
};

/* Filters used at dawn. (30 filters).          *
 * From 04:00 to 05:59 filters are cycled every *
 * 2 minutes.                                   */
static const u16 sDawnFilters[] =
{
    RGB2(15, 15, 10),
    RGB2(15, 15, 10),   //1
    RGB2(14, 14, 10),   //2
    RGB2(13, 13, 10),   //3
    RGB2(12, 12, 10),   //4
    RGB2(11, 11, 10),   //5
    RGB2(10, 10, 10),   //6
    RGB2(9, 9, 10),     //7
    RGB2(8, 8, 10),     //8
    RGB2(8, 8, 11),     //9
    RGB2(7, 7, 11),     //10
    RGB2(6, 6, 11),     //11
    RGB2(5, 5, 11),     //12
    RGB2(4, 4, 11),     //13
    RGB2(3, 3, 11),     //14
    RGB2(2, 2, 11),     //15
    RGB2(1, 1, 11),     //16
    RGB2(0, 0, 11),     //17
    RGB2(0, 0, 10),     //18
    RGB2(0, 0, 9),      //19
    RGB2(0, 0, 8),      //20
    RGB2(0, 0, 7),      //21
    RGB2(0, 0, 6),      //22
    RGB2(0, 0, 5),      //23
    RGB2(0, 0, 4),      //24
    RGB2(0, 0, 3),      //0003
    RGB2(0, 0, 2),      //0002
    RGB2(0, 0, 1),      //0001
    RGB2(0, 0, 0),      //0000
    RGB2(0, 0, 0),      //0000
};

/* Filters used at day. (no filter actually lul)*/
static const u16 sDayFilter = RGB2(0, 0, 0);   //0000

/* Filters used at sunset. (30 filters).        *
 * From 17:00 to 18:59 filters are cycled every *
 * 2 minutes.                                   */
static const u16 sSunsetFilters[] = 
{
    RGB2(0, 0, 1),      //0004
    RGB2(0, 1, 1),      //2004
    RGB2(0, 1, 2),      //2008
    RGB2(0, 1, 3),      //200C
    RGB2(0, 2, 3),      //400C
    RGB2(0, 2, 4),      //4010
    RGB2(0, 2, 5),      //4014
    RGB2(0, 3, 5),      //6014
    RGB2(0, 3, 6),      //6018
    RGB2(0, 3, 7),      //601C
    RGB2(0, 4, 7),      //801C
    RGB2(0, 4, 8),      //8020
    RGB2(0, 4, 9),      //8024
    RGB2(0, 5, 9),      //A024
    RGB2(0, 5, 10),     //A028
    RGB2(0, 5, 11),     //A02C
    RGB2(0, 6, 11),     //C02C
    RGB2(0, 6, 12),     //C030
    RGB2(0, 6, 13),     //C034
    RGB2(0, 7, 13),     //E034
    RGB2(0, 7, 14),     //E038
    RGB2(0, 7, 14),     //E038
    RGB2(0, 8, 14),     //0039
    RGB2(0, 9, 14),     //2039
    RGB2(0, 10, 14),    //4039
    RGB2(0, 11, 14),    //6039
    RGB2(0, 12, 14),    //8039
    RGB2(0, 13, 14),    //A039
    RGB2(0, 14, 14),    //C039
    RGB2(0, 14, 14),    //C039
};

/* Filters used at nightfall. (30 filters).     *
 * From 19:00 to 21:59 filters are cycled every *
 * 2 minutes.                                   */
static const u16 sNightfallFilters[] = 
{
    RGB2(0, 14, 14),    //39C0
    RGB2(0, 14, 14),    //39C0
    RGB2(0, 14, 13),    //35C0
    RGB2(0, 14, 12),    //31C0
    RGB2(0, 14, 11),    //2DC0
    RGB2(0, 14, 10),    //29C0
    RGB2(1, 14, 10),    //29C1
    RGB2(1, 14, 9),     //25C1
    RGB2(0, 14, 8),     //21C0
    RGB2(1, 14, 7),     //1DC1
    RGB2(1, 14, 6),     //19C1
    RGB2(2, 14, 6),     //19C2
    RGB2(2, 14, 5),     //15C2
    RGB2(2, 14, 4),     //11C2
    RGB2(2, 14, 3),     //0DC2
    RGB2(2, 14, 2),     //09C2
    RGB2(2, 14, 2),     //09C2
    RGB2(3, 14, 3),     //0DC3
    RGB2(4, 14, 4),     //11C4
    RGB2(5, 14, 5),     //15C5
    RGB2(6, 14, 6),     //19C6
    RGB2(7, 14, 6),     //19C7
    RGB2(8, 14, 6),     //19C8
    RGB2(9, 14, 6),     //19C9
    RGB2(10, 14, 6),    //19CA
    RGB2(11, 14, 6),    //19CB
    RGB2(12, 14, 6),    //19CC
    RGB2(13, 14, 6),    //19CD
    RGB2(14, 14, 6),    //19CE
    RGB2(14, 14, 6),    //19CE
};

/* Filter used at night. From 22:00 to 24:59 */
static const u16 sNightFilter = RGB2(14, 14, 6);   //19CE

// The season for each month of the year
static const u8 sSeasonsByMonth[MONTH_COUNT] =
{
	[MONTH_JAN - 1] = SEASON_SUMMER,
	[MONTH_FEB - 1] = SEASON_SUMMER,
	[MONTH_MAR - 1] = SEASON_SUMMER,
	[MONTH_APR - 1] = SEASON_AUTUMN,
	[MONTH_MAY - 1] = SEASON_AUTUMN,
	[MONTH_JUN - 1] = SEASON_AUTUMN,
	[MONTH_JUL - 1] = SEASON_WINTER,
	[MONTH_AUG - 1] = SEASON_WINTER,
	[MONTH_SEP - 1] = SEASON_WINTER,
	[MONTH_OCT - 1] = SEASON_SPRING,
	[MONTH_NOV - 1] = SEASON_SPRING,
	[MONTH_DEC - 1] = SEASON_SPRING
};

/***********************************************
 * --------- DNS CONFIGURATION END ----------- *
 * ******************************************* */

// Functions
static u16 GetDNSFilter(void);
static void DoDNSLightningWindowsEffect(void);

// DNS palette buffer in EWRAM
ALIGNED(4) EWRAM_DATA static u16 sDNSPaletteDmaBuffer[PLTT_BUFFER_SIZE] = {0};

#define IN_OVERWORLD ((gMain.callback2 == CB2_Overworld || gMain.callback2 == CB2_OverworldBasic))
#define IN_BATTLE ((gMain.callback2 == BattleMainCB2 && gMain.vblankCallback != VBlankCB_PokedexScreen))
#define LIT_UP_TIME ((gRtcLocation.hour < MORNING_OF_DAY_START || gRtcLocation.hour >= NIGHT_OF_DAY_START - 1))

/* **************************************************** *
 * **************** D&N for pokefirered *************** *
 * **************************************************** *
 * Based on Prime Dialga DNS for Pokemon GBA Games.     *
 * Additional credits to Andrea & Eing                  *
 * Author: Xhyz/Samu                                    *
 ****************************************************** */

bool8 IsMapDNSException(void)
{
	u8 i, mapType = gMapHeader.mapType;
	
	for (i = 0; i < ARRAY_COUNT(sDNSMapExceptions); i++)
	{
		if (sDNSMapExceptions[i] == mapType)
			return TRUE;
	}
	return FALSE;
}

void DNSTransferPlttBuffer(void *src, void *dest)
{
	if (!IsMapDNSException() && !(gBattleTypeFlags & BATTLE_TYPE_POKEDUDE))
	{
		if (IN_OVERWORLD)
		{
#if LIT_UP_WINDOWS
			DoDNSLightningWindowsEffect();
#endif
			DmaCopy16(3, sDNSPaletteDmaBuffer, dest, PLTT_SIZE);
			return;
		}
#if USE_DNS_IN_BATTLE
		else if (IN_BATTLE)
		{
			DmaCopy16(3, sDNSPaletteDmaBuffer, dest, PLTT_SIZE);
			return;
		}
#endif
	}
	DmaCopy16(3, src, dest, PLTT_SIZE);
}

static bool8 IsSpritePaletteTagDNSException(u8 palNum, const u16 *tagExceptions, u8 tagExceptionsCount)
{
	u8 i;
	u16 tag = GetSpritePaletteTagByPaletteNum(palNum);

	for (i = 0; i < tagExceptionsCount; i++)
	{
		if (tagExceptions[i] == tag)
			return TRUE;
	}
	return FALSE;
}

static inline u16 DNSApplyProportionalFilterToColour(u16 colour, u16 filter)
{
	u32 red, green, blue;
    
	red = (colour & 0x1F) * (0x1F - (filter & 0x1F)) >> 5;
	green = ((colour & 0x3E0) >> 5) * ((0x3E0 - (filter & 0x3E0)) >> 5) >> 5;
	blue = ((colour & 0x7C00) >> 10) * ((0x7C00 - (filter & 0x7C00)) >> 10) >> 5;
	
	return RGB2(red <= 31 ? red : 0, green <= 31 ? green : 0, blue <= 31 ? blue : 0);
}

void DNSApplyFilters(const struct DNSPalExceptions palExceptionFlags, const u16 *tagExceptions, u8 tagExceptionsCount)
{
	u8 palNum, colNum;
	u16 colourSlot, rgbFilter = GetDNSFilter();
	
	for (palNum = 0; palNum < 32; palNum++)
	{
		if (palExceptionFlags.pal[palNum] && (palNum < 16 || tagExceptions == NULL || !IsSpritePaletteTagDNSException(palNum - 16, tagExceptions, tagExceptionsCount)))
		{
			for (colNum = 0; colNum < 16; colNum++)
			{
				colourSlot = palNum * 16 + colNum;
				sDNSPaletteDmaBuffer[colourSlot] = DNSApplyProportionalFilterToColour(gPlttBufferFaded[colourSlot], rgbFilter);
			}
		}
		else
		{
			for (colNum = 0; colNum < 16; colNum++)
			{
				colourSlot = palNum * 16 + colNum;
				sDNSPaletteDmaBuffer[colourSlot] = gPlttBufferFaded[colourSlot];
			}
		}
	}
}

static void DoDNSLightningWindowsEffect(void)
{
	u8 i;
	u16 colourSlot;
	bool8 fadeActive = gPaletteFade.active;
	
	if (LIT_UP_TIME)
	{
		for (i = 0; i < ARRAY_COUNT(sLightingColours); i++)
		{
			colourSlot = sLightingColours[i].paletteNum * 16 + sLightingColours[i].colourNum;
			
			if (fadeActive || (gPlttBufferUnfaded[colourSlot] != 0x0000 && gPlttBufferFaded[colourSlot] == 0x0000))
			{
				sDNSPaletteDmaBuffer[colourSlot] = gPlttBufferFaded[colourSlot];
				gPlttBufferUnfaded[colourSlot] = sLightingColours[i].lightColour;
			}
			else
				sDNSPaletteDmaBuffer[colourSlot] = sLightingColours[i].lightColour;
		}
		gMain.tilesetPaletteReloaded = FALSE;
	}
	else if (!fadeActive && !gMain.tilesetPaletteReloaded)
	{
		LoadMapTilesetPalettes(gMapHeader.mapLayout);
		gMain.tilesetPaletteReloaded = TRUE;
	}
}

u8 GetDNSTimeLapse(void)
{
	u8 hour = gRtcLocation.hour;
	
	if (hour < DAWN_OF_DAY_START)
        return TIME_MIDNIGHT;
    else if (hour < MORNING_OF_DAY_START)
        return TIME_DAWN;
    else if (hour < AFTERNOON_OF_DAY_START)
        return TIME_DAY;
    else if (hour < NIGHT_OF_DAY_START)
        return TIME_SUNSET;
    else if (hour < MIDNIGHT_OF_DAY_START)
        return TIME_NIGHTFALL;
    else 
        return TIME_NIGHT;
}

static u16 GetDNSFilter(void)
{
	u8 minutes = gRtcLocation.minute;
	
	switch (GetDNSTimeLapse())
	{
		case TIME_MIDNIGHT:
			return sMidnightFilters[gRtcLocation.hour < 1 ? minutes >> 3 : 7];
		case TIME_DAWN:
		    return sDawnFilters[minutes >> 1];
		case TIME_DAY:
		    return sDayFilter;
		case TIME_SUNSET: 
		    return sSunsetFilters[minutes >> 1];
		case TIME_NIGHTFALL:
		    return sNightfallFilters[minutes >> 1];
		case TIME_NIGHT:
	        return sNightFilter;
	}
}

u8 GetDNSTimeLapseDayOrNight(void)
{
	switch (GetDNSTimeLapse())
	{
		case TIME_MIDNIGHT:
		case TIME_NIGHTFALL:
		case TIME_NIGHT:
		    return TIME_NIGHT;
		default:
			return TIME_DAY;
	}
}

u8 DNSGetCurrentSeason(void)
{
	return sSeasonsByMonth[gRtcLocation.month - 1];
}

// Based off: https://blog.eletrogate.com/relogio-de-fases-lunares-com-o-arduino/
u8 DNSGetMoonPhase(void)
{
	u8 month = gRtcLocation.month;
	int moonPhase, year = gRtcLocation.year - (int)((MONTH_COUNT - month) / 10);
	f64 yearDays, monthDays, leapYear, julianaDate;
	
	month += 9;
	if (month >= MONTH_COUNT)
		month -= MONTH_COUNT;
	
	yearDays = 365.25 * (year + 4172);
	monthDays = (int)((30.6001 * month) + 0.5);
	leapYear = (int)((((year / 100) + 4) * 0.75) - 38);
	
	julianaDate = yearDays + monthDays + gRtcLocation.day + 59;
	julianaDate -= leapYear;
	julianaDate = (int)(julianaDate - 2244116.75);
	julianaDate /= 29.53;
	
	moonPhase = julianaDate;
	julianaDate -= moonPhase;
	
	moonPhase = julianaDate * 8 + 0.5;
	moonPhase &= 7;
	
	switch (moonPhase)
	{
		case 0:
		    moonPhase = PHASE_NEW_MOON;
			break;
		case 1 ... 3:
		    moonPhase = PHASE_CRESCENT_MOON;
			break;
		case 4:
		    moonPhase = PHASE_FULL_MOON;
			break;
		case 5 ... 7:
		    moonPhase = PHASE_WANING_MOON;
			break;
	}
	return moonPhase;
}

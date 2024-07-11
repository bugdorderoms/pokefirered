#include "global.h"
#include "daily_events.h"
#include "event_data.h"
#include "pokemon.h"
#include "rtc.h"

static bool8 CanDoDailyEvents(void)
{
	struct DailyEvent *dailyData = &gSaveBlock2Ptr->dailyEvent;
	u8 dailyDay = dailyData->day;
	u8 dailyMonth = dailyData->month;
	u32 dailyYear = dailyData->year + dailyData->century * 100;
	
	if (dailyYear > gRtcLocation.year
	|| (dailyYear == gRtcLocation.year && dailyMonth > gRtcLocation.month)
	|| (dailyYear == gRtcLocation.year && dailyMonth == gRtcLocation.month && dailyDay > gRtcLocation.day))
		return FALSE;
		
	if (dailyDay != gRtcLocation.day || dailyMonth != gRtcLocation.month || dailyYear != gRtcLocation.year)
	{
		dailyData->minute = gRtcLocation.minute;
		dailyData->hour = gRtcLocation.hour;
		dailyData->day = gRtcLocation.day;
		dailyData->month = gRtcLocation.month;
		dailyData->year = gRtcLocation.year % 100;
		dailyData->century = gRtcLocation.year / 100;
		
		return TRUE;
	}
	return FALSE;
}

static u32 GetDaysSinceTimeInDailyEvent(struct DailyEvent dailyEvent)
{
	u16 startYear = dailyEvent.year + dailyEvent.century * 100;
	if (startYear < 1900)
		startYear = 1900;
	return GetDayDifference(startYear, dailyEvent.month, dailyEvent.day, gRtcLocation.year, gRtcLocation.month, gRtcLocation.day);
}

void DoDailyEvents(void)
{
	struct DailyEvent backupDailyEvent = gSaveBlock2Ptr->dailyEvent;
	
	if (CanDoDailyEvents())
	{
		u32 daysSince = GetDaysSinceTimeInDailyEvent(backupDailyEvent);
		
		UpdatePartyPokerusTime(daysSince);
		UpdatePartyFormChangeCountdown(daysSince);
		ClearDailyEventFlags();
	}
}

#include "global.h"
#include "pokemon.h"
#include "rtc_events.h"

static void DoPerMonthBasedEvents(void);
static void DoPerDayBasedEvents(void);
static void DoPerHourBasedEvents(void);
static void DoPerMinuteBasedEvents(void);

void RtcUpdatePerMonthEvents(u8 thisMonth, u8 lastMonth)
{
	if (thisMonth != lastMonth)
		DoPerMonthBasedEvents();
}

void RtcUpdatePerDayEvents(u8 thisDay, u8 yesterDay)
{
	if (thisDay != yesterDay)
		DoPerDayBasedEvents();
}

void RtcUpdatePerHourEvents(u8 thisHour, u8 lastHour)
{
	if (thisHour != lastHour)
		DoPerHourBasedEvents();
}

void RtcUpdatePerMinuteEvents(u8 thisMinute, u8 lastMinute)
{
	if (thisMinute != lastMinute)
		DoPerMinuteBasedEvents();
}

static void DoPerMonthBasedEvents(void)
{
	
}

static void DoPerDayBasedEvents(void)
{
	UpdatePartyPokerusTime();
}

static void DoPerHourBasedEvents(void)
{
	
}

static void DoPerMinuteBasedEvents(void)
{
	
}

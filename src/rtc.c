#include "global.h"
#include "event_data.h"
#include "rtc.h"

// Rtc
static void RtcGetRawInfo(struct SiiRtcInfo *rtc);
static u16 RtcCheckInfo(struct SiiRtcInfo *rtc);
static void UpdateClockFromRtc(struct SiiRtcInfo *rtc);
static u32 ConvertBcdToBinary(u8 bcd);
// SiiRtc
static void SiiRtcUnprotect(void);
static u8 SiiRtcProbe(void);
static bool8 SiiRtcGetStatus(struct SiiRtcInfo *rtc);
static bool8 SiiRtcGetDateTime(struct SiiRtcInfo *rtc);

// Ew ram
static EWRAM_DATA u16 sRTCErrorStatus = 0;
static EWRAM_DATA u16 sRTCSavedIme = 0;
static EWRAM_DATA bool8 sSiiRTCLocked = FALSE;
static EWRAM_DATA u8 sRTCProbeResult = 0;

// Iw ram
struct SiiRtcInfo Rtc;
struct Rtc gRtcLocation;

static const struct SiiRtcInfo sRtcDummy =
{
	// 2000 Jan 1
	.year = 0,
	.month = MONTH_JAN,
	.day = 1
};

static const u8 sNumDaysInMonths[MONTH_COUNT] =
{
	[MONTH_JAN - 1] = 31,
	[MONTH_FEB - 1] = 28,
	[MONTH_MAR - 1] = 31,
	[MONTH_APR - 1] = 30,
	[MONTH_MAY - 1] = 31,
	[MONTH_JUN - 1] = 30,
	[MONTH_JUL - 1] = 31,
	[MONTH_AUG - 1] = 31,
	[MONTH_SEP - 1] = 30,
	[MONTH_OCT - 1] = 31,
	[MONTH_NOV - 1] = 30,
	[MONTH_DEC - 1] = 31
};

/////////
// RTC //
/////////

#define RTCINFO_FREQUENCY (1 << 0) // Frequency interrupt enable
#define RTCINFO_PERMINUTE (1 << 1) // Per-minute interrupt enable
#define RTCINFO_ALARM     (1 << 2) // Alarm interrupt enable
#define RTCINFO_UNUSED1   (1 << 3)
#define RTCINFO_UNUSED2   (1 << 4)
#define RTCINFO_UNUSED3   (1 << 5)
#define RTCINFO_24HOUR    (1 << 6) // 0: 12-hour mode, 1: 24-hour mode
#define RTCINFO_POWER     (1 << 7) // Power on or power failure occurred

#define RTC_ERR_12HOUR_CLOCK   0x0010
#define RTC_ERR_POWER_FAILURE  0x0020
#define RTC_ERR_INVALID_YEAR   0x0040
#define RTC_ERR_INVALID_MONTH  0x0080
#define RTC_ERR_INVALID_DAY    0x0100
#define RTC_ERR_INVALID_HOUR   0x0200
#define RTC_ERR_INVALID_MINUTE 0x0400
#define RTC_ERR_INVALID_SECOND 0x0800

#define RTC_ERR_FLAG_MASK      0x0FF0

#define RTC_INIT_ERROR   0x0001
#define RTC_INIT_WARNING 0x0002

#define RTC_DISABLE_INTERRUPTS \
	sRTCSavedIme = REG_IME;    \
	REG_IME = 0;

#define RTC_RESTORE_INTERRUPTS \
    REG_IME = sRTCSavedIme;

void RtcCalcLocalTime(void)
{
	if (sRTCErrorStatus & RTC_ERR_FLAG_MASK)
		Rtc = sRtcDummy;
	else
		RtcGetRawInfo(&Rtc);
	
	UpdateClockFromRtc(&Rtc);
}

void RtcInit(void)
{
	sRTCErrorStatus = 0;
	
	RTC_DISABLE_INTERRUPTS
	SiiRtcUnprotect();
	sRTCProbeResult = SiiRtcProbe();
	RTC_RESTORE_INTERRUPTS
	
	if ((sRTCProbeResult & 0xF) != 1)
		sRTCErrorStatus = RTC_INIT_ERROR;
	else
	{
		sRTCErrorStatus = (sRTCProbeResult & 0xF0) ? RTC_INIT_WARNING : 0;
	    
	    RtcGetRawInfo(&Rtc);
	    sRTCErrorStatus = RtcCheckInfo(&Rtc);
	}
}

static void RtcGetStatus(struct SiiRtcInfo *rtc)
{
	RTC_DISABLE_INTERRUPTS
	SiiRtcGetStatus(rtc);
	RTC_RESTORE_INTERRUPTS
}

static void RtcGetDateTime(struct SiiRtcInfo *rtc)
{
	RTC_DISABLE_INTERRUPTS
	SiiRtcGetDateTime(rtc);
	RTC_RESTORE_INTERRUPTS
}

static void RtcGetRawInfo(struct SiiRtcInfo *rtc)
{
	RtcGetStatus(rtc);
	RtcGetDateTime(rtc);
}

static bool8 IsLeapYear(u32 year)
{
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
		return TRUE;
	return FALSE;
}

static u16 RtcCheckInfo(struct SiiRtcInfo *rtc)
{
	u16 errorFlags = 0;
	u32 year, month, day, hour, minute, second;
	
	if (rtc->status & RTCINFO_POWER)
		errorFlags |= RTC_ERR_POWER_FAILURE;
	
	if (!(rtc->status & RTCINFO_24HOUR))
		errorFlags |= RTC_ERR_12HOUR_CLOCK;
	
	year = ConvertBcdToBinary(rtc->year);
	
	if (year == 0xFF)
		errorFlags |= RTC_ERR_INVALID_YEAR;
	
	month = ConvertBcdToBinary(rtc->month);
	
	if (month == 0xFF || month < MONTH_JAN || month > MONTH_DEC)
		errorFlags |= RTC_ERR_INVALID_MONTH;
	
	day = ConvertBcdToBinary(rtc->day);
	
	if (day == 0xFF)
		errorFlags |= RTC_ERR_INVALID_DAY;
	
	if (month == MONTH_FEB)
	{
		if (day > IsLeapYear(year) + sNumDaysInMonths[month - 1])
			errorFlags |= RTC_ERR_INVALID_DAY;
	}
	else
	{
		if (day > sNumDaysInMonths[month - 1])
			errorFlags |= RTC_ERR_INVALID_DAY;
	}
	hour = ConvertBcdToBinary(rtc->hour);
	
	if (hour > HOURS_PER_DAY)
		errorFlags |= RTC_ERR_INVALID_HOUR;
	
	minute = ConvertBcdToBinary(rtc->minute);
	
	if (minute > MINUTES_PER_HOUR)
		errorFlags |= RTC_ERR_INVALID_MINUTE;
	
	second = ConvertBcdToBinary(rtc->second);

	if (second > SECONDS_PER_MINUTE)
		errorFlags |= RTC_ERR_INVALID_SECOND;

	return errorFlags;
}

static void UpdateClockFromRtc(struct SiiRtcInfo *rtc)
{
	gRtcLocation.year = ConvertBcdToBinary(rtc->year) + 2000; // Base year is 2000
	gRtcLocation.month = ConvertBcdToBinary(rtc->month);
	gRtcLocation.day = ConvertBcdToBinary(rtc->day);
	gRtcLocation.dayOfWeek = ConvertBcdToBinary(rtc->dayOfWeek);
	gRtcLocation.hour = ConvertBcdToBinary(rtc->hour);
	gRtcLocation.minute = ConvertBcdToBinary(rtc->minute);
	gRtcLocation.second = ConvertBcdToBinary(rtc->second);
}

static u32 ConvertBcdToBinary(u8 bcd)
{
	if (bcd > 0x9F)
		return 0xFF;
	else
		return (bcd & 0xF) <= 9 ? (10 * ((bcd >> 4) & 0xF)) + (bcd & 0xF) : 0xFF;
}

////////////
// SIIRTC //
////////////

#define STATUS_FREQUENCY 0x02 // Frequency interrupt enable
#define STATUS_PERMINUTE 0x08 // Per-minute interrupt enable
#define STATUS_ALARM     0x20 // Alarm interrupt enable
#define STATUS_24HOUR    0x40 // 0: 12-hour mode, 1: 24-hour mode
#define STATUS_POWER     0x80 // Power on or power failure occurred

#define TEST_MODE 0x80 // flag in the "second" byte

#define ALARM_AM 0x00
#define ALARM_PM 0x80

#define OFFSET_YEAR         offsetof(struct SiiRtcInfo, year)
#define OFFSET_MONTH        offsetof(struct SiiRtcInfo, month)
#define OFFSET_DAY          offsetof(struct SiiRtcInfo, day)
#define OFFSET_DAY_OF_WEEK  offsetof(struct SiiRtcInfo, dayOfWeek)
#define OFFSET_HOUR         offsetof(struct SiiRtcInfo, hour)
#define OFFSET_MINUTE       offsetof(struct SiiRtcInfo, minute)
#define OFFSET_SECOND       offsetof(struct SiiRtcInfo, second)
#define OFFSET_STATUS       offsetof(struct SiiRtcInfo, status)
#define OFFSET_ALARM_HOUR   offsetof(struct SiiRtcInfo, alarmHour)
#define OFFSET_ALARM_MINUTE offsetof(struct SiiRtcInfo, alarmMinute)

#define INFO_BUF(info, index) (*((u8 *)(info) + (index)))

#define DATETIME_BUF(info, index) INFO_BUF(info, OFFSET_YEAR + index)
#define DATETIME_BUF_LEN (OFFSET_SECOND - OFFSET_YEAR + 1)

#define TIME_BUF(info, index) INFO_BUF(info, OFFSET_HOUR + index)
#define TIME_BUF_LEN (OFFSET_SECOND - OFFSET_HOUR + 1)

#define WR 0 // command for writing data
#define RD 1 // command for reading data

#define CMD(n) (0x60 | (n << 1))

#define CMD_RESET    CMD(0)
#define CMD_STATUS   CMD(1)
#define CMD_DATETIME CMD(2)
#define CMD_TIME     CMD(3)
#define CMD_ALARM    CMD(4)

#define SCK_HI (1 << 0)
#define SIO_HI (1 << 1)
#define CS_HI  (1 << 2)

#define DIR_0_IN    0
#define DIR_0_OUT   1
#define DIR_1_IN    0
#define DIR_1_OUT   2
#define DIR_2_IN    0
#define DIR_2_OUT   4
#define DIR_ALL_IN  (DIR_0_IN | DIR_1_IN | DIR_2_IN)
#define DIR_ALL_OUT (DIR_0_OUT | DIR_1_OUT | DIR_2_OUT)

#define GPIO_PORT_DATA        (*(vu16 *)0x80000C4)
#define GPIO_PORT_DIRECTION   (*(vu16 *)0x80000C6)
#define GPIO_PORT_READ_ENABLE (*(vu16 *)0x80000C8)

static void SiiRtcUnprotect(void)
{
	GPIO_PORT_READ_ENABLE = 1;
	sSiiRTCLocked = FALSE;
}

static int WriteCommand(u8 value)
{
	u8 i, temp;

	for (i = 0; i < 8; i++)
	{
		temp = ((value >> (7 - i)) & 1);
		GPIO_PORT_DATA = (temp << 1) | CS_HI;
		GPIO_PORT_DATA = (temp << 1) | CS_HI;
		GPIO_PORT_DATA = (temp << 1) | CS_HI;
		GPIO_PORT_DATA = (temp << 1) | SCK_HI | CS_HI;
	}
	// Control reaches end of non-void function
	return 0;
}

static int WriteData(u8 value)
{
	u8 i, temp;

	for (i = 0; i < 8; i++)
	{
		temp = ((value >> i) & 1);
		GPIO_PORT_DATA = (temp << 1) | CS_HI;
		GPIO_PORT_DATA = (temp << 1) | CS_HI;
		GPIO_PORT_DATA = (temp << 1) | CS_HI;
		GPIO_PORT_DATA = (temp << 1) | SCK_HI | CS_HI;
	}
	// Control reaches end of non-void function
	return 0;
}

static u8 ReadData(void)
{
	u8 i, temp, value = 0;

	for (i = 0; i < 8; i++)
	{
		GPIO_PORT_DATA = CS_HI;
		GPIO_PORT_DATA = CS_HI;
		GPIO_PORT_DATA = CS_HI;
		GPIO_PORT_DATA = CS_HI;
		GPIO_PORT_DATA = CS_HI;
		GPIO_PORT_DATA = SCK_HI | CS_HI;

		temp = ((GPIO_PORT_DATA & SIO_HI) >> 1);
		value = (value >> 1) | (temp << 7); // UB: accessing uninitialized var
	}
	return value;
}

static bool8 SiiRtcGetStatus(struct SiiRtcInfo *rtc)
{
	u8 statusData;
	
	if (!sSiiRTCLocked)
	{
		sSiiRTCLocked = TRUE;
	    
	    GPIO_PORT_DATA = SCK_HI;
	    GPIO_PORT_DATA = SCK_HI | CS_HI;
	    
	    GPIO_PORT_DIRECTION = DIR_ALL_OUT;
	    
	    WriteCommand(CMD_STATUS | RD);
	    
	    GPIO_PORT_DIRECTION = DIR_0_OUT | DIR_1_IN | DIR_2_OUT;
	    
	    statusData = ReadData();
	    
	    rtc->status = (statusData & (STATUS_POWER | STATUS_24HOUR)) | ((statusData & STATUS_ALARM) >> 3) | ((statusData & STATUS_PERMINUTE) >> 2)
	    			| ((statusData & STATUS_FREQUENCY) >> 1);
	    			
	    GPIO_PORT_DATA = SCK_HI;
	    GPIO_PORT_DATA = SCK_HI;
	    
	    sSiiRTCLocked = FALSE;
	    
	    return TRUE;
	}
	return FALSE;
}

static bool8 SiiRtcSetStatus(struct SiiRtcInfo *rtc)
{
	u8 statusData;
	
	if (!sSiiRTCLocked)
	{
		sSiiRTCLocked = TRUE;
		
		GPIO_PORT_DATA = SCK_HI;
		GPIO_PORT_DATA = SCK_HI | CS_HI;
		
		statusData = STATUS_24HOUR | ((rtc->status & RTCINFO_ALARM) << 3) | ((rtc->status & RTCINFO_PERMINUTE) << 2) | ((rtc->status & RTCINFO_FREQUENCY) << 1);
		
		GPIO_PORT_DIRECTION = DIR_ALL_OUT;
		
		WriteCommand(CMD_STATUS | WR);
		
		WriteData(statusData);
		
		GPIO_PORT_DATA = SCK_HI;
		GPIO_PORT_DATA = SCK_HI;
		
		sSiiRTCLocked = FALSE;
		
		return TRUE;
	}
	return FALSE;
}

static bool8 SiiRtcGetTime(struct SiiRtcInfo *rtc)
{
	u8 i;
	
	if (!sSiiRTCLocked)
	{
		sSiiRTCLocked = TRUE;
		
		GPIO_PORT_DATA = SCK_HI;
		GPIO_PORT_DATA = SCK_HI | CS_HI;
		
		GPIO_PORT_DIRECTION = DIR_ALL_OUT;
		
		WriteCommand(CMD_TIME | RD);
		
		GPIO_PORT_DIRECTION = DIR_0_OUT | DIR_1_IN | DIR_2_OUT;
		
		for (i = 0; i < TIME_BUF_LEN; i++)
			TIME_BUF(rtc, i) = ReadData();
		
		INFO_BUF(rtc, OFFSET_HOUR) &= 0x7F;
		
		GPIO_PORT_DATA = SCK_HI;
		GPIO_PORT_DATA = SCK_HI;
		
		sSiiRTCLocked = FALSE;
		
		return TRUE;
	}
	return FALSE;
}

static bool8 SiiRtcGetDateTime(struct SiiRtcInfo *rtc)
{
	u8 i;
	
	if (!sSiiRTCLocked)
	{
		sSiiRTCLocked = TRUE;
		
		GPIO_PORT_DATA = SCK_HI;
	    GPIO_PORT_DATA = SCK_HI | CS_HI;
	    
	    GPIO_PORT_DIRECTION = DIR_ALL_OUT;
	    
	    WriteCommand(CMD_DATETIME | RD);
	    
	    GPIO_PORT_DIRECTION = DIR_0_OUT | DIR_1_IN | DIR_2_OUT;
	    
	    for (i = 0; i < DATETIME_BUF_LEN; i++)
	    	DATETIME_BUF(rtc, i) = ReadData();
	    
	    INFO_BUF(rtc, OFFSET_HOUR) &= 0x7F;
	    
	    GPIO_PORT_DATA = SCK_HI;
	    GPIO_PORT_DATA = SCK_HI;
		
		sSiiRTCLocked = FALSE;
		
		return TRUE;
	}
	return FALSE;
}

static bool8 SiiRtcReset(void)
{
	struct SiiRtcInfo rtc;
	rtc.status = RTCINFO_24HOUR;
	return SiiRtcSetStatus(&rtc);
}

static u8 SiiRtcProbe(void)
{
	u8 errorCode = 0;
	struct SiiRtcInfo rtc;
	
	if (!SiiRtcGetStatus(&rtc))
		return errorCode;
	
	// The RTC is in 12-hour mode. Reset it and switch to 24-hour mode.
	if ((rtc.status & (RTCINFO_POWER | RTCINFO_24HOUR)) == RTCINFO_POWER || (rtc.status & (RTCINFO_POWER | RTCINFO_24HOUR)) == 0)
	{
		if (!SiiRtcReset())
			return errorCode;
		
		++errorCode;
	}
	SiiRtcGetTime(&rtc);
	
	// The RTC is in test mode. Reset it to leave test mode.
	if (rtc.second & TEST_MODE)
	{
		if (!SiiRtcReset())
			return (errorCode << 4) & 0xF0;
		
		++errorCode;
	}
	return (errorCode << 4) | 1;
}

//////////////////
// DAILY EVENTS //
//////////////////

/*
// Cumulative Days by month
static const u16 sCumDaysByMonth[MONTH_COUNT] =
{
	[MONTH_JAN - 1] = 0,
	[MONTH_FEB - 1] = 31,
	[MONTH_MAR - 1] = 59,
	[MONTH_APR - 1] = 90,
	[MONTH_MAY - 1] = 120,
	[MONTH_JUN - 1] = 151,
	[MONTH_JUL - 1] = 181,
	[MONTH_AUG - 1] = 212,
	[MONTH_SEP - 1] = 243,
	[MONTH_OCT - 1] = 273,
	[MONTH_NOV - 1] = 304,
	[MONTH_DEC - 1] = 334
};

// Cumulative Days by month for leap year
static const u16 sCumDaysByMonthLeapYear[MONTH_COUNT] =
{
	[MONTH_JAN - 1] = 0,
	[MONTH_FEB - 1] = 31,
	[MONTH_MAR - 1] = 60,
	[MONTH_APR - 1] = 91,
	[MONTH_MAY - 1] = 121,
	[MONTH_JUN - 1] = 152,
	[MONTH_JUL - 1] = 182,
	[MONTH_AUG - 1] = 213,
	[MONTH_SEP - 1] = 244,
	[MONTH_OCT - 1] = 274,
	[MONTH_NOV - 1] = 305,
	[MONTH_DEC - 1] = 335
};

bool8 CheckSetDailyEvent(u16 var, bool8 set)
{
	bool8 ret = FALSE;
	struct DailyEvent *dailyEventData = (struct DailyEvent*)GetVarPointer(var);
	u8 dailyYear = dailyEventData->year + dailyEventData->century * 100;
	u8 dailyMonth = dailyEventData->month;
	u8 dailyDay = dailyEventData->day;
	u16 year = gRtcLocation.year;
	u8 month = gRtcLocation.month;
	u8 day = gRtcLocation.day;
	
	if (!(dailyYear > year || (dailyYear == year && dailyMonth > month) || (dailyYear == year && dailyMonth == month && dailyDay > day)) && (dailyDay != day
	|| dailyMonth != month || dailyYear != year))
	{
		if (set)
		{
			dailyEventData->year = year % 100;
			dailyEventData->century = year / 100;
			dailyEventData->month = month;
			dailyEventData->day = day;
			dailyEventData->hour = gRtcLocation.hour;
			dailyEventData->minute = gRtcLocation.minute;
		}
		ret = TRUE;
	}
	return ret;
}

void DoTimeBasedEvents(void)
{
	u32 i;
	u16 var;
	
	for (i = 0; i < ARRAY_COUNT(sTimeBasedEventFuncs); i++)
	{
		var = sTimeBasedEventFuncs[i].var;
		
		if (CheckSetDailyEvent(var, TRUE))
			sTimeBasedEventFuncs[i].func(GetDaysSinceTimeInValue((struct DailyEvent *)(VarGet(var) | (VarGet(var + 1) << 16))));
	}
}

static inline bool8 IsDate1BeforeDate2(u32 y1, u32 m1, u32 d1, u32 y2, u32 m2, u32 d2)
{
	return y1 < y2 ? TRUE : (y1 == y2 ? (m1 < m2 ? TRUE : (m1 == m2 ? d1 < d2 : FALSE)) : FALSE);
}

static u32 GetDayDifference(u32 startYear, u8 startMonth, u8 startDay, u32 endYear, u8 endMonth, u8 endDay)
{
	const u16 *cumDays;
	u32 year, totalDays = 0;
	bool8 isLeapYear;
	
	if (!IsDate1BeforeDate2(startYear, startMonth, startDay, endYear, endMonth, endDay))
		return totalDays;
	
	isLeapYear = IsLeapYear(startYear);
	cumDays = isLeapYear ? sCumDaysByMonthLeapYear : sCumDaysByMonth;
	
	if (startYear == endYear)
		return (cumDays[endMonth - 1] + endDay) - (cumDays[startMonth - 1] + startDay);
	
	totalDays = (isLeapYear ? 366 : 365) - (cumDays[startMonth - 1] + startDay);
	
	year = startYear + 1;
	if (year < endYear)
		totalDays += (IsLeapYear(year) ? 366 : 365);
	
	totalDays += (IsLeapYear(endYear) ? sCumDaysByMonthLeapYear[endMonth - 1] : sCumDaysByMonth[endMonth - 1]) + endDay;
	
	return totalDays;
}

u32 GetDaysSinceTimeInValue(struct DailyEvent *dailyEventData)
{
	u16 startYear = dailyEventData->year + dailyEventData->century * 100;
	if (startYear < 1900)
		startYear = 1900;
	return GetDayDifference(startYear, dailyEventData->month, dailyEventData->day, gRtcLocation.year, gRtcLocation.month, gRtcLocation.day);
}*/

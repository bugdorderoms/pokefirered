#ifndef GUARD_RTC_H
#define GUARD_RTC_H

// Months of the year
enum
{
	MONTH_JAN = 1,
	MONTH_FEB,
	MONTH_MAR,
	MONTH_APR,
	MONTH_MAY,
	MONTH_JUN,
	MONTH_JUL,
	MONTH_AUG,
	MONTH_SEP,
	MONTH_OCT,
	MONTH_NOV,
	MONTH_DEC,
	MONTH_COUNT = MONTH_DEC
};

#define HOURS_PER_DAY      24
#define MINUTES_PER_HOUR   60
#define SECONDS_PER_MINUTE 60

struct SiiRtcInfo
{
	u8 year;
	u8 month;
	u8 day;
	u8 dayOfWeek;
	u8 hour;
	u8 minute;
	u8 second;
	u8 status;
	u8 alarmHour;
	u8 alarmMinute;
};

struct Rtc
{
	u16 year;
	u8 month;
	u8 day;
	u8 dayOfWeek;
	u8 hour;
	u8 minute;
	u8 second;
};

void RtcInit(void);
void RtcCalcLocalTime(void);

extern struct Rtc gRtcLocation;

#endif // GUARD_RTC_H
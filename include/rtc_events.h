#ifndef GUARD_RTC_EVENTS_H
#define GUARD_RTC_EVENTS_H

void RtcUpdatePerMonthEvents(u8 thisMonth, u8 lastMonth);
void RtcUpdatePerDayEvents(u8 thisDay, u8 yesterDay);
void RtcUpdatePerHourEvents(u8 thisHour, u8 lastHour);
void RtcUpdatePerMinuteEvents(u8 thisMinute, u8 lastMinute);

#endif // GUARD_RTC_EVENTS_H
#ifndef GUARD_DNS_UTILS_H
#define GUARD_DNS_UTILS_H

#define DNS_PAL_EXCEPTION FALSE
#define DNS_PAL_ACTIVE    TRUE

#define DAWN_OF_DAY_START      4
#define MORNING_OF_DAY_START   6
#define AFTERNOON_OF_DAY_START 17
#define NIGHT_OF_DAY_START     19
#define MIDNIGHT_OF_DAY_START  22

enum
{
    TIME_MIDNIGHT,
    TIME_DAWN,
    TIME_DAY,
    TIME_SUNSET,
    TIME_NIGHTFALL,
    TIME_NIGHT
};

enum
{
	SEASON_SUMMER,
	SEASON_AUTUMN,
	SEASON_WINTER,
	SEASON_SPRING
};

enum
{
	PHASE_NEW_MOON,
	PHASE_CRESCENT_MOON,
	PHASE_FULL_MOON,
	PHASE_WANING_MOON,
};

struct lightingColour
{
    u8 paletteNum;
    u8 colourNum;
    u16 lightColour;
};

struct DNSPalExceptions
{
    bool8 pal[32];
};

void DNSTransferPlttBuffer(void *src, void *dest);
void DNSApplyFilters(const struct DNSPalExceptions palExceptionFlags, const u16 *tagExceptions, u8 tagExceptionsCount);
u8 GetDNSTimeLapse(void);
u8 GetDNSTimeLapseDayOrNight(void);
u8 DNSGetCurrentSeason(void);
bool8 IsMapDNSException(void);
u8 DNSGetMoonPhase(void);

#endif // GUARD_DNS_UTILS_H

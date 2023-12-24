#ifndef GUARD_DNS_UTILS_H
#define GUARD_DNS_UTILS_H

#define DNS_PAL_EXCEPTION FALSE
#define DNS_PAL_ACTIVE    TRUE

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
bool8 GetDNSTimeLapseIsNight(void);
u8 DNSGetCurrentSeason(void);
bool8 IsMapDNSException(void);

#endif // GUARD_DNS_UTILS_H

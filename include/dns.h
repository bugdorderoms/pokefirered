#ifndef GUARD_DNS_UTILS_H
#define GUARD_DNS_UTILS_H

#define USE_DNS_IN_BATTLE   TRUE
#define LIT_UP_WINDOWS      TRUE

#define DNS_PAL_EXCEPTION   FALSE
#define DNS_PAL_ACTIVE      TRUE

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
void DNSApplyFilters(void);

#endif // GUARD_DNS_UTILS_H

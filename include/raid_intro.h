#ifndef GUARD_RAID_INTRO_H
#define GUARD_RAID_INTRO_H

#include "constants/raid.h"

struct Raid
{
    u16 species;
    u16 item;
    u16 drops[MAX_RAID_DROPS];
};

struct RaidData
{
    const struct Raid *data;
    u32 amount;
};

struct RaidPartner
{
    u16 trainerId;
    u16 graphicsId;
};

struct RaidIntroData
{
    u16 species;
    u8 level;
    u8 numStars;
    const u16 *drops;
    struct RaidPartner *partners;
    u16 item;
    bool8 canCancel:1;
    bool8 canConnectInLink:1;
    bool8 tryGigantamax:1;
};

void RaidDenInitIntro(void);
void InitFixedRaidIntro(struct RaidIntroData *fixedRaidData);
u32 GetRaidRecommendedLevel(u32 numStars);
bool32 IsRaidClearedFlagSet(void);
bool32 IsRaidBattleAvailable(void);
void InitRaidClearScreen(u32 species, u32 numStars, bool32 bossCaptured);

extern struct ItemSlot gRaidBattleDrops[MAX_RAID_DROPS];

#endif // GUARD_RAID_INTRO_H

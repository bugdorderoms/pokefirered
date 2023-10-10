#include "global.h"
#include "random.h"

u32 gRngValue;

u16 Random(void)
{
    gRngValue = ISO_RANDOMIZE(gRngValue);
    return gRngValue >> 16;
}

void SeedRng(u16 seed)
{
    gRngValue = seed;
}

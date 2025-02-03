#include "global.h"
#include "random.h"

u32 gRngValue;

void SeedRng(u16 seed)
{
    gRngValue = seed;
}

u16 Random(void)
{
    gRngValue = ISO_RANDOMIZE(gRngValue);
    return gRngValue >> 16;
}

u16 RandomRange(u16 min, u16 max)
{
	return (Random() & (max - min)) + min;
}

bool8 RandomPercent(u16 percentage)
{
	return (RandomMax(100) < percentage);
}

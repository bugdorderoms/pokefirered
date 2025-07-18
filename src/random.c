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

__attribute__((weak, alias("RandomUniformDefault")))
u32 RandomUniform(u32 randomTag, u32 lo, u32 hi);

__attribute__((weak, alias("RandomWeightedArrayDefault")))
u32 RandomWeightedArray(u32 randomTag, u32 sum, u32 n, const u8 *weights);

__attribute__((weak, alias("RandomElementArrayDefault")))
const void *RandomElementArray(u32 randomTag, const void *array, u32 size, u32 count);

u32 RandomUniformDefault(u32 randomTag, u32 lo, u32 hi)
{
    return lo + (((hi - lo + 1) * Random()) >> 16);
}

u32 RandomWeightedArrayDefault(u32 randomTag, u32 sum, u32 n, const u8 *weights)
{
    s32 i, targetSum = (sum * Random()) >> 16;

    for (i = 0; i < n - 1; i++)
    {
        targetSum -= weights[i];
        if (targetSum < 0)
            return i;
    }
    return n - 1;
}

const void *RandomElementArrayDefault(u32 randomTag, const void *array, u32 size, u32 count)
{
    return (const u8 *)array + size * RandomUniformDefault(randomTag, 0, count - 1);
}

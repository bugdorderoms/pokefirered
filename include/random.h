#ifndef GUARD_RANDOM_H
#define GUARD_RANDOM_H

#include "global.h"

// The number 1103515245 comes from the example implementation
// of rand and srand in the ISO C standard.
#define ISO_RANDOMIZE(val)  (1103515245 * val + 24691)
#define ISO_RANDOMIZE2(val) (1103515245 * val + 12345)

extern u32 gRngValue;

void SeedRng(u16 seed); // Sets the initial seed value of the pseudorandom number generator
u16 Random(void); // Returns a 16-bit pseudorandom number
u16 RandomRange(u16 min, u16 max); // Returns a 16-bit pseudorandom number betwen min(inclusive) and max(inclusive)

// Returns a 32-bit pseudorandom number
#define Random32() (Random() | (Random() << 16))

// Returns a 16-bit pseudorandom number betwen 0 and max(exclusive)
#define RandomMax(max) ((Random() % (max)))

// Returns the given table's element, chosen at random
#define RandomElement(table) ((table[RandomMax(ARRAY_COUNT(table))]))

// Returns where a pseudorandom percentage chance will occours
static inline bool8 RandomPercent(u16 percentage)
{
	return (RandomMax(100) < percentage);
}

#endif // GUARD_RANDOM_H

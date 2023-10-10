#ifndef GUARD_RANDOM_H
#define GUARD_RANDOM_H

#include "global.h"

// The number 1103515245 comes from the example implementation
// of rand and srand in the ISO C standard.
#define ISO_RANDOMIZE(val)  (1103515245 * val + 24691)
#define ISO_RANDOMIZE2(val) (1103515245 * val + 12345)

extern u32 gRngValue;

//Returns a 16-bit pseudorandom number
u16 Random(void);

//Returns a 32-bit pseudorandom number
#define Random32() (Random() | (Random() << 16))

//Sets the initial seed value of the pseudorandom number generator
void SeedRng(u16 seed);

#endif // GUARD_RANDOM_H

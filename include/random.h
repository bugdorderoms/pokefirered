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

// Returns a 32-bit pseudorandom number
#define Random32() (Random() | (Random() << 16))

// Returns a 16-bit pseudorandom number betwen 0 and max(exclusive)
#define RandomMax(max) ((Random() % (max)))

/* Structured random number generator.
 * Instead of the caller converting bits from Random() to a meaningful
 * value, the caller provides metadata that is used to return the
 * meaningful value directly. This allows code to interpret the random
 * call, for example, battle tests know what the domain of a random call
 * is, and can exhaustively test it.
 *
 * RandomTag identifies the purpose of the value.
 *
 * RandomUniform(tag, lo, hi) returns a number from lo to hi inclusive
 * with uniform probability.
 *
 * RandomElement(tag, array) returns an element in array with uniform
 * probability. The array must be known at compile-time (e.g. a global
 * const array).
 *
 * RandomPercentage(tag, t) returns FALSE with probability (1-t)/100,
 * and TRUE with probability t/100.
 *
 * RandomWeighted(tag, w0, w1, ... wN) returns a number from 0 to N
 * inclusive. The return value is proportional to the weights, e.g.
 * RandomWeighted(..., 1, 1) returns 50% 0s and 50% 1s.
 * RandomWeighted(..., 2, 1) returns 2/3 0s and 1/3 1s. */

enum RandomTag
{
    RNG_NONE,
    RNG_WILD_IVS,
    RNG_HITS,
    RNG_ACCURACY,
    RNG_CONFUSION,
    RNG_CRITICAL_HIT,
    RNG_CUTE_CHARM,
    RNG_DAMAGE_MODIFIER,
    RNG_FLAME_BODY,
    RNG_FORCE_RANDOM_SWITCH,
    RNG_FROZEN,
    RNG_HOLD_EFFECT_FLINCH,
    RNG_INFATUATION,
    RNG_PARALYSIS,
    RNG_POISON_POINT,
    RNG_RAMPAGE_TURNS,
    RNG_SECONDARY_EFFECT,
    RNG_SECONDARY_EFFECT_2,
    RNG_SECONDARY_EFFECT_3,
    RNG_SLEEP_TURNS,
    RNG_SPEED_TIE,
    RNG_STATIC,
    RNG_STENCH,
    RNG_TRACE_TARGET,
    RNG_FOREWARN,
    RNG_HEALER,
    RNG_HARVEST,
    RNG_TOXIC_CHAIN,
    RNG_POISON_TOUCH,
    RNG_SOS_CALL_RATE,
    RNG_SOS_CALL_ANSWER,
    RNG_FOCUS_BAND,
    RNG_WRAP_TURNS,
    RNG_CONFUSION_TURNS,
    RNG_METRONOME_MOVE,
    RNG_MOODY_STAT_DOWN,
    RNG_MOODY_STAT_UP,
    RNG_RANDOM_STAT_UP,
    RNG_SHED_SKIN,
    RNG_EFFECT_SPORE,
    RNG_TRI_ATTACK,
    RNG_RANDOM_TARGET,
    RNG_QUICK_CLAW,
    RNG_QUICK_DRAW,
    RNG_MIRROR_MOVE,
    RNG_ASSIST,
    RNG_OHKO,
    RNG_CONVERSION_2,
    RNG_SLEEP_TALK,
    RNG_CURSED_BODY,
    RNG_PICKUP,
    RNG_PICKUP_RANDOM_ITEM,
};

#define RandomWeighted(tag, ...) \
    ({ \
        const u8 weights[] = { __VA_ARGS__ }; \
        u32 sum, i; \
        for (i = 0, sum = 0; i < ARRAY_COUNT(weights); i++) \
            sum += weights[i]; \
        RandomWeightedArray(tag, sum, ARRAY_COUNT(weights), weights); \
    })

#define RandomPercentage(tag, t) \
    ({ \
        u32 r; \
        if (t <= 0) \
        { \
            r = FALSE; \
        } \
        else if (t >= 100) \
        { \
            r = TRUE; \
        } \
        else \
        { \
          const u8 weights[] = { 100 - t, t }; \
          r = RandomWeightedArray(tag, 100, ARRAY_COUNT(weights), weights); \
        } \
        r; \
    })

#define RandomElement(tag, array) \
    ({ \
        *(typeof((array)[0]) *)RandomElementArray(tag, array, sizeof((array)[0]), ARRAY_COUNT(array)); \
    })

u32 RandomUniform(u32 randomTag, u32 lo, u32 hi);
u32 RandomWeightedArray(u32 randomTag, u32 sum, u32 n, const u8 *weights);
const void *RandomElementArray(u32 RandomTag, const void *array, u32 size, u32 count);

u32 RandomUniformDefault(u32 randomTag, u32 lo, u32 hi);
u32 RandomWeightedArrayDefault(u32 randomTag, u32 sum, u32 n, const u8 *weights);
const void *RandomElementArrayDefault(u32 RandomTag, const void *array, u32 size, u32 count);

#endif // GUARD_RANDOM_H

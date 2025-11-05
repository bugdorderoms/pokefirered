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
    RNG_SOS_HIDDEN_ABILITY,
    RNG_FOCUS_BAND,
    RNG_WRAP_TURNS,
    RNG_MAGNITUDE,
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
    RNG_PSYWAVE,
    RNG_OHKO,
    RNG_CONVERSION_2,
    RNG_SLEEP_TALK,
    RNG_CURSED_BODY,
    RNG_PICKUP,
    RNG_PICKUP_RANDOM_ITEM,
    RNG_HONEY_GATHER,
    RNG_SHUCKLE_BERRY_JUICE,
    RNG_AI_CHOSEN_MOVE,
    RNG_AI_CHOSEN_TARGET,
    RNG_RANDOM_WEATHER,
    RNG_RANDOM_THUNDERSTORM,
    RNG_RANDOM_SNOWSTORM,
    RNG_RANDOM_DROUGHT,
    RNG_INTIMIDATE_WILD_ENCOUNTER_INFLUENCE,
    RNG_WILD_ENCOUNTER,
    RNG_WILD_ENCOUNTER_COOLDOWN,
    RNG_ABILITY_WILD_TYPE_INFLUENCE,
    RNG_ABILITY_WILD_TYPE_INFLUENCE_INDEX,
    RNG_SYNCHRONIZE_WILD_NATURE_INFLUENCE,
    RNG_PRESSURE_WILD_LEVEL_INFLUENCE,
    RNG_CUTE_CHARM_WILD_GENDER_INFLUENCE,
    RNG_DOUBLE_WILD_BATTLE,
    RNG_SOS_WILD_BATTLE,
    RNG_AMBIENT_CRY_FROM_LAND,
    RNG_WILD_ENCOUNTER_LEVEL,
    RNG_WILD_ENCOUNTER_LAND_INDEX,
    RNG_WILD_ENCOUNTER_WATER_INDEX,
    RNG_WILD_ENCOUNTER_FISHING_INDEX,
    RNG_BLACK_FLUTE,
    RNG_WHITE_FLUTE,
    RNG_WILD_IVS,
    RNG_WILD_HIDDEN_ABILITY,
    RNG_WILD_TERA_TYPE,
    RNG_WILD_HELD_ITEM,
    RNG_DEXNAV_ROLL,
    RNG_DEXNAV_WILD_LEVEL,
    RNG_DEXNAV_LEVEL_BONUS,
    RNG_DEXNAV_EGG_MOVE_CHANCE,
    RNG_DEXNAV_EGG_MOVE_ID,
    RNG_DEXNAV_HELD_ITEM,
    RNG_DEXNAV_ABILITY,
    RNG_DEXNAV_HIDDEN_ABILITY,
    RNG_DEXNAV_DISTANCE,
    RNG_DEXNAV_POTENTIAL,
    RNG_DEXNAV_SHINY_ROLL,
    RNG_ABILITY_INHERIT,
    RNG_HIDDEN_ABILITY_INHERIT,
    RNG_POKEBALL_INHERIT,
    RNG_NATURE_INHERIT,
    RNG_POWER_ITEM_IV_INHERIT,
    RNG_FRIENDSHIP_WALKING,
    RNG_PLAYER_NAME,
    RNG_GAIN_FAN_CLUBE_MEMBER,
    RNG_LOSE_FAN_CLUBE_MEMBER,
    RNG_THUNDERSTORM_DELAY,
    RNG_THUNDERSTORM_RETRY,
    RNG_FLYING_LEAVE_Y,
    RNG_FLYING_LEAVE_SPEED_X,
    RNG_FLYING_LEAVE_SPEED_Y,
    RNG_FLYING_LEAVE_AMPLITUDE,
    RNG_SLOT_MACHINE_INDICE,
    RNG_SLOT_MACHINE_BIAS,
    RNG_SAMPLE_RESORT_DELUXE,
    RNG_SAMPLE_RESORT_DELUXE_REWARD,
    RNG_SAMPLE_RESORT_SPECIES,
    RNG_SAFARI_BAIT_THROW,
    RNG_SAFARI_ROCK_THROW,
    RNG_SAFARI_FLEE_RATE,
    RNG_POKEMON_JUMP_PRIZE_ITEM,
    RNG_FISHING,
    RNG_ABILITY_FISHING_INFLUENCE,
    RNG_NPC_MOVEMENT_DELAY,
    RNG_NPC_MOVEMENT_DIRECTION,
    RNG_ROAMER_MAP_NUM,
    RNG_ROAMER_MAP_CHANGE,
    RNG_ROAMER_WILD_ENCOUNTER,
    RNG_POKERUS,
    RNG_POKERUS_SPREAD,
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

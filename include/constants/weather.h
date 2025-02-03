#ifndef GUARD_CONSTANTS_WEATHER_H
#define GUARD_CONSTANTS_WEATHER_H

#define WEATHER_NONE               0
#define WEATHER_RAIN               1
#define WEATHER_SNOW               2
#define WEATHER_RAIN_THUNDERSTORM  3
#define WEATHER_FOG_HORIZONTAL     4
#define WEATHER_VOLCANIC_ASH       5
#define WEATHER_SANDSTORM          6
#define WEATHER_FOG_DIAGONAL       7
#define WEATHER_SHADE              8
#define WEATHER_DOWNPOUR           9
#define WEATHER_UNDERWATER_BUBBLES 10
#define WEATHER_CLOUDS             11

// Weathers as bits
#define BIT_WEATHER_RAIN         (1 << 0)
#define BIT_WEATHER_SNOW         (1 << 1)
#define BIT_WEATHER_FOG          (1 << 2)
#define BIT_WEATHER_VOLCANIC_ASH (1 << 3)
#define BIT_WEATHER_SANDSTORM    (1 << 4)
#define BIT_WEATHER_UNDERWATER   (1 << 5)

#endif  // GUARD_CONSTANTS_WEATHER_H

#ifndef GUARD_CONSTANTS_WEATHER_H
#define GUARD_CONSTANTS_WEATHER_H

#define WEATHER_NONE               0
#define WEATHER_RAIN               1
#define WEATHER_SNOW               2 // snowflakes
#define WEATHER_RAIN_THUNDERSTORM  3
#define WEATHER_FOG_HORIZONTAL     4
#define WEATHER_VOLCANIC_ASH       5
#define WEATHER_SANDSTORM          6
#define WEATHER_FOG_DIAGONAL       7
#define WEATHER_SHADE              8
#define WEATHER_DOWNPOUR           9
#define WEATHER_UNDERWATER_BUBBLES 10
#define WEATHER_CLOUDS             11
#define WEATHER_SNOWSTORM          12
#define WEATHER_DROUGHT            13

// Weathers as bits
#define BIT_WEATHER_RAIN         Bit(0)
#define BIT_WEATHER_SNOW         Bit(1)
#define BIT_WEATHER_FOG          Bit(2)
#define BIT_WEATHER_VOLCANIC_ASH Bit(3)
#define BIT_WEATHER_SANDSTORM    Bit(4)
#define BIT_WEATHER_UNDERWATER   Bit(5)
#define BIT_WEATHER_DROUGHT      Bit(6)

#endif  // GUARD_CONSTANTS_WEATHER_H

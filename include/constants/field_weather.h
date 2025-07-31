#ifndef GUARD_CONSTANTS_FIELD_WEATHER_H
#define GUARD_CONSTANTS_FIELD_WEATHER_H

#define NUM_RAIN_SPRITES             24
#define NUM_SNOWFLAKE_SPRITES        101
#define NUM_FOG_HORIZONTAL_SPRITES   20
#define NUM_ASH_SPRITES              20
#define NUM_FOG_DIAGONAL_SPRITES     20
#define NUM_SANDSTORM_SPRITES        20
#define NUM_SWIRL_SANDSTORM_SPRITES  5
#define NUM_CLOUD_SPRITES            3
#define NUM_SNOWSTORM_SPRITES        20
#define NUM_SWIRL_SNOWSTORM_SPRITES  5
#define NUM_LEAVE_SPRITES            8

// Controls how the weather should be changing the screen palettes.
#define WEATHER_PAL_STATE_CHANGING_WEATHER   0
#define WEATHER_PAL_STATE_SCREEN_FADING_IN   1
#define WEATHER_PAL_STATE_SCREEN_FADING_OUT  2
#define WEATHER_PAL_STATE_IDLE               3

// Modes for FadeScreen
#define FADE_FROM_BLACK  0
#define FADE_TO_BLACK    1
#define FADE_FROM_WHITE  2
#define FADE_TO_WHITE    3

// Shadows values
#define BASE_SHADOW_INTENSITY 12
#define SHADOW_COLOR_INDEX    9 // This color index of weather palettes should be reserved for shadows

#endif // GUARD_CONSTANTS_FIELD_WEATHER_H

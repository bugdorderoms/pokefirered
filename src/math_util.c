#include "global.h"
#include "math_util.h"
#include "util.h"

s16 Q_8_8_mul(s16 x, s16 y)
{
    s32 result = x;
    result *= y;
    result /= 256;
    return result;
}

s16 Q_N_S_mul(u32 s, s16 x, s16 y)
{
    s32 result = x;
    result *= y;
    result /= Bit(s);
    return result;
}

s32 Q_24_8_mul(s32 x, s32 y)
{
    s64 result = x;
    result *= y;
    result /= 256;
    return result;
}

s16 Q_8_8_div(s16 x, s16 y)
{
    if (y == 0)
        return 0;

    return (x << 8) / y;
}

s16 Q_N_S_div(u32 s, s16 x, s16 y)
{
    if (y == 0)
        return 0;

    return (x << s) / y;
}

s32 Q_24_8_div(s32 x, s32 y)
{
    s64 _x;

    if (y == 0)
        return 0;

    _x = x;
    _x *= 256;
    return _x / y;
}

s16 Q_8_8_inv(s16 y)
{
    s32 x = 0x10000;
    return x / y;
}

s16 Q_N_S_inv(u32 s, s16 y)
{
    s32 x = 0x100 << s;
    return x / y;
}

s32 Q_24_8_inv(s32 y)
{
    s64 x = 0x10000;
    return x / y;
}

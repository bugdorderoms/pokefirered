#ifndef GUARD_MATH_UTIL_H
#define GUARD_MATH_UTIL_H

// Fixed-point arithmetic library.

// useful math macros

// Rounding value for Q4.12 fixed-point format
#define UQ_4_12_SHIFT (12)
#define UQ_4_12_ROUND (Bit(UQ_4_12_SHIFT - 1))

// Converts a number to Q8.8 fixed-point format
#define Q_8_8(n) ((s16)((n) * 256))

// Converts a number from Q8.8 fixed-point format
#define Q_8_8_TO_INT(n) ((s16)((n) >> 8))

// Converts a number to Q4.12 fixed-point format
#define Q_4_12(n)  ((s16)((n) * 4096))
#define UQ_4_12(n) ((u32)((n) * 4096 + 0.5))

// Converts a number from Q4.12 fixed-point format
#define Q_4_12_TO_INT(n) ((s16)((n) >> 12))

// Converts a number to QN.S fixed-point format (16-bits)
#define Q_N_S(s, n) ((s16)((n) * Bit(s)))

// converts a number from QN.S fixed-point format (16-bits)
#define Q_N_S_TO_INT(s, n) ((s16)((n) >> (s)))

// Converts a number to Q24.8 fixed-point format
#define Q_24_8(n) ((s32)((n) << 8))

// Converts a number from Q24.8 fixed-point format
#define Q_24_8_TO_INT(n) ((s32)((n) >> 8))

// x * y
s16 Q_8_8_mul(s16 x, s16 y);
s16 Q_N_S_mul(u32 s, s16 x, s16 y);
s32 Q_24_8_mul(s32 x, s32 y);
u32 uq4_12_mul(u32 x, u32 y);

// x / y
s16 Q_8_8_div(s16 x, s16 y);
s16 Q_N_S_div(u32 s, s16 x, s16 y);
s32 Q_24_8_div(s32 x, s32 y);

// 1.0 / y
s16 Q_8_8_inv(s16 y);
s16 Q_N_S_inv(u32 s, s16 y);
s32 Q_24_8_inv(s32 y);

#endif //GUARD_MATH_UTIL_H

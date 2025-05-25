#ifndef GUARD_STRING_UTIL_H
#define GUARD_STRING_UTIL_H

#include "global.h"
#include "text.h"

enum StringConvertMode
{
    STR_CONV_MODE_LEFT_ALIGN,
    STR_CONV_MODE_RIGHT_ALIGN,
    STR_CONV_MODE_LEADING_ZEROS
};

u8 *StringCopy_WithLimit(u8 *dest, const u8 *src, u32 limit);
u8 *StringGet_WithLimit(u8 *str, u32 limit);
u8 *StringCopy(u8 *dest, const u8 *src);
u8 *StringAppend(u8 *dest, const u8 *src);
u8 *StringCopyN(u8 *dest, const u8 *src, u32 n);
u8 *StringAppendN(u8 *dest, const u8 *src, u32 n);
u16 StringLength(const u8 *str);
s32 StringCompare(const u8 *str1, const u8 *str2);
s32 StringCompareN(const u8 *str1, const u8 *str2, u32 n);
bool8 IsStringLengthAtLeast(const u8 *str, s32 n);
u8 *ConvertIntToDecimalStringN(u8 *dest, s32 value, enum StringConvertMode mode, u32 n);
u8 *StringExpandPlaceholders(u8 *dest, const u8 *src);
u8 *StringBraille(u8 *dest, const u8 *src);
u8 *GetExpandedPlaceholder(u32 id);
u8 *StringFill(u8 *dest, u32 c, u16 n);
u8 *StringCopyPadded(u8 *dest, const u8 *src, u32 c, u16 n);
u8 *StringCopyN_Multibyte(u8 *dest, const u8 *src, u32 n);
u32 StringLength_Multibyte(const u8 *str);
u8 *WriteColorChangeControlCode(u8 *dest, u32 colorType, u32 color);
bool32 IsStringJapanese(u8 *str);
u32 GetExtCtrlCodeLength(u32 code);
s32 StringCompareWithoutExtCtrlCodes(const u8 *str1, const u8 *str2);
void ConvertInternationalString(u8 *s, u32 language);
void StripExtCtrlCodes(u8 *str);
u32 ReformatStringToMaxChars(const u8 *src, u32 fontId, u32 maxChars, bool32 allowsJumpLine);

static inline u8 *StringCopy_Nickname(u8 *dest, const u8 *src)
{
    return StringCopy_WithLimit(dest, src, POKEMON_NAME_LENGTH);
}

static inline u8 *StringGet_Nickname(u8 *str)
{
    return StringGet_WithLimit(str, POKEMON_NAME_LENGTH);
}

static inline u8 *StringCopy7(u8 *dest, const u8 *src)
{
    return StringCopy_WithLimit(dest, src, 7);
}

static inline u8 *StringFillWithTerminator(u8 *dest, u16 n)
{
    return StringFill(dest, EOS, n);
}

#endif // GUARD_STRING_UTIL_H

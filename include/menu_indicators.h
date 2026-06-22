#ifndef GUARD_MENU_INDICATORS_H
#define GUARD_MENU_INDICATORS_H

#include "global.h"

struct ScrollArrowsTemplate
{
    u8 firstArrowType;
    u8 firstX;
    u8 firstY;
    u8 secondArrowType;
    u8 secondX;
    u8 secondY;
    u16 fullyUpThreshold;
    u16 fullyDownThreshold;
    u16 tileTag;
    u16 palTag;
    u8 palNum;
};

struct CursorStruct
{
    u8 left;
    u8 top;
    u16 rowWidth;
    u16 rowHeight;
    u16 tileTag;
    u16 palTag;
    u8 palNum;
};

u32 AddScrollIndicatorArrowPairParameterized(u32 arrowType, s32 commonPos, s32 firstPos, s32 secondPos, s32 fullyDownThreshold, s32 tileTag, s32 palTag, u16 *scrollOffset);
u32 AddScrollIndicatorArrowPair(const struct ScrollArrowsTemplate * template, u16 * a1);
void RemoveScrollIndicatorArrowPair(u32);
void SetScrollIndicatorArrowPairSpritePriority(u32 taskId, u32 priority);
u32 ListMenuAddCursorObjectInternal(const struct CursorStruct *cursor, u32 cursorKind);
u32 ListMenuAddRedOutlineCursorObject(const struct CursorStruct *cursor);
void ListMenuUpdateRedOutlineCursorObject(u32 taskId, u32 x, u32 y);
void ListMenuRemoveRedOutlineCursorObject(u32 taskId);
void ListMenuRemoveCursorObject(u32 taskId, u32 cursorKind);
void ListMenuUpdateCursorObject(u32 taskId, u32 x, u32 y, u32 cursorKind);

#endif //GUARD_MENU_INDICATORS_H

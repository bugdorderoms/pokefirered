#ifndef GUARD_ITEMFINDER_H
#define GUARD_ITEMFINDER_H

// Task data
#define tItemX             data[0]
#define tItemY             data[1]
#define tHiddenItemFound   data[2]
#define tDingTimer         data[3]
#define tNumDingsRemaining data[4]
#define tDingNum           data[5]
#define tUnderfoot         data[6]
#define tStartSpriteId     data[7]
#define tDestroyStar       data[8]

void ItemUseOnFieldCB_Itemfinder(u32 taskId);
void Task_StoutlandSearch(u8 taskId);

#endif //GUARD_ITEMFINDER_H

#ifndef GUARD_DAYCARE_CONSTANTS_H
#define GUARD_DAYCARE_CONSTANTS_H

// Parent compatibility scores
#define PARENTS_INCOMPATIBLE       0
#define PARENTS_LOW_COMPATIBILITY  20
#define PARENTS_MED_COMPATIBILITY  50
#define PARENTS_MAX_COMPATIBILITY  70

// Daycare state
#define DAYCARE_NO_MONS     0
#define DAYCARE_EGG_WAITING 1
#define DAYCARE_ONE_MON     2
#define DAYCARE_TWO_MONS    3

#define INHERITED_IV_COUNT 3
#define DESTINY_KNOT_INHERITED_IV_COUNT 5

#define EGG_HATCH_LEVEL 1

#define DAYCARE_LEVEL_MENU_EXIT   5
#define DAYCARE_EXITED_LEVEL_MENU 2 // would be redundant with above if GF had used the same value

// Array buffers
#define EGG_MOVES_ARRAY_COUNT           10

#endif //GUARD_DAYCARE_CONSTANTS_H

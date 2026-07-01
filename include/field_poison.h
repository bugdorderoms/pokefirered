#ifndef GUARD_FIELD_POISON_H
#define GUARD_FIELD_POISON_H

#include "global.h"

enum {
    FLDPSN_NONE,
    FLDPSN_PSN,
    FLDPSN_FNT
};

u32 DoPoisonFieldEffect(void);

extern const u8 gText_PkmnPoisonFainted[];
extern const u8 gText_PkmnPoisonSurvived[];

#endif //GUARD_FIELD_POISON_H

#ifndef GUARD_EVENT_DATA_H
#define GUARD_EVENT_DATA_H

#include "global.h"

extern u16 gSpecialVar_0x8000;
extern u16 gSpecialVar_0x8001;
extern u16 gSpecialVar_0x8002;
extern u16 gSpecialVar_0x8003;
extern u16 gSpecialVar_0x8004;
extern u16 gSpecialVar_0x8005;
extern u16 gSpecialVar_0x8006;
extern u16 gSpecialVar_0x8007;
extern u16 gSpecialVar_0x8008;
extern u16 gSpecialVar_0x8009;
extern u16 gSpecialVar_0x800A;
extern u16 gSpecialVar_0x800B;
extern u16 gSpecialVar_Facing;
extern u16 gSpecialVar_Result;
extern u16 gSpecialVar_LastTalked;
extern u16 gSpecialVar_MonBoxId;
extern u16 gSpecialVar_MonBoxPos;
extern u16 gSpecialVar_TextColor;
extern u16 gSpecialVar_PrevTextColor;

void InitEventData(void);
void ClearTempFieldEventData(void);
u16 *GetVarPointer(u32 id);
u32 VarGet(u32 id);
bool32 VarSet(u32 id, u32 value);
u32 VarGetObjectEventGraphicsId(u32 id);
bool32 FlagSet(u32 id);
bool32 FlagClear(u32 id);
bool32 FlagGet(u32 id);
void ResetSpecialVars(void);
void ResetMysteryGiftFlags(void);
void ClearDailyEventFlags(void);

#endif // GUARD_EVENT_DATA_H

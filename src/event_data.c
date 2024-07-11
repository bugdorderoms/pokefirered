#include "global.h"
#include "event_data.h"
#include "item_menu.h"
#include "field_player_avatar.h"

#define NUM_SPECIAL_FLAGS (SPECIAL_FLAGS_END - SPECIAL_FLAGS_START + 1)
#define NUM_TEMP_FLAGS    (TEMP_FLAGS_END - TEMP_FLAGS_START + 1)
#define NUM_TEMP_VARS     (TEMP_VARS_END - VARS_START + 1)

EWRAM_DATA u16 gSpecialVar_0x8000 = 0;
EWRAM_DATA u16 gSpecialVar_0x8001 = 0;
EWRAM_DATA u16 gSpecialVar_0x8002 = 0;
EWRAM_DATA u16 gSpecialVar_0x8003 = 0;
EWRAM_DATA u16 gSpecialVar_0x8004 = 0;
EWRAM_DATA u16 gSpecialVar_0x8005 = 0;
EWRAM_DATA u16 gSpecialVar_0x8006 = 0;
EWRAM_DATA u16 gSpecialVar_0x8007 = 0;
EWRAM_DATA u16 gSpecialVar_0x8008 = 0;
EWRAM_DATA u16 gSpecialVar_0x8009 = 0;
EWRAM_DATA u16 gSpecialVar_0x800A = 0;
EWRAM_DATA u16 gSpecialVar_0x800B = 0;
EWRAM_DATA u16 gSpecialVar_Result = 0;
EWRAM_DATA u16 gSpecialVar_LastTalked = 0;
EWRAM_DATA u16 gSpecialVar_Facing = 0;
EWRAM_DATA u16 gSpecialVar_MonBoxId = 0;
EWRAM_DATA u16 gSpecialVar_MonBoxPos = 0;
EWRAM_DATA u16 gSpecialVar_TextColor = 0;
EWRAM_DATA u16 gSpecialVar_PrevTextColor = 0;
EWRAM_DATA u16 gSpecialVar_0x8014 = 0;
EWRAM_DATA u8 sSpecialFlags[NUM_SPECIAL_FLAGS] = {};

extern u16 *const gSpecialVars[];

void InitEventData(void)
{
    memset(gSaveBlock1Ptr->flags, 0, sizeof(gSaveBlock1Ptr->flags));
    memset(gSaveBlock1Ptr->vars, 0, sizeof(gSaveBlock1Ptr->vars));
    memset(sSpecialFlags, 0, NUM_SPECIAL_FLAGS);
}

void ClearTempFieldEventData(void)
{
    memset(gSaveBlock1Ptr->flags + (TEMP_FLAGS_START / 8), 0, (NUM_TEMP_FLAGS / 8));
    memset(gSaveBlock1Ptr->vars, 0, (NUM_TEMP_VARS * 2));
    FlagClear(FLAG_SYS_WHITE_FLUTE_ACTIVE);
    FlagClear(FLAG_SYS_BLACK_FLUTE_ACTIVE);
    if (!TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACHAMP_RIDE))
        FlagClear(FLAG_SYS_USE_STRENGTH);
    FlagClear(FLAG_SYS_SPECIAL_WILD_BATTLE);
    FlagClear(FLAG_SYS_INFORMED_OF_LOCAL_WIRELESS_PLAYER);
}

void ResetMysteryGiftFlags(void)
{
	u16 i;
	
	for (i = FLAG_MYSTERY_GIFT_1; i <= FLAG_MYSTERY_GIFT_15; i++)
		FlagClear(i);
}

void ClearDailyEventFlags(void)
{
	u16 i;
	
	for (i = FLAG_0x0B2; i <= FLAG_0x0BB; i++)
		FlagClear(i);
}

u16 *GetVarPointer(u16 idx)
{
    if (idx < VARS_START)
        return NULL;
	
    if (idx < SPECIAL_VARS_START)
        return &gSaveBlock1Ptr->vars[idx - VARS_START];
	
    return gSpecialVars[idx - SPECIAL_VARS_START];
}

u16 VarGet(u16 idx)
{
    u16 *ptr = GetVarPointer(idx);
    if (ptr == NULL)
        return idx;
    return *ptr;
}

bool8 VarSet(u16 idx, u16 val)
{
    u16 *ptr = GetVarPointer(idx);
    if (ptr == NULL)
        return FALSE;
    *ptr = val;
    return TRUE;
}

u8 VarGetObjectEventGraphicsId(u8 idx)
{
    return VarGet(VAR_OBJ_GFX_ID_0 + idx);
}

u8 *GetFlagAddr(u16 idx)
{
    if (idx == 0)
        return NULL;
	
    if (idx < SPECIAL_FLAGS_START)
        return &gSaveBlock1Ptr->flags[idx / 8];
	
    return &sSpecialFlags[(idx - SPECIAL_FLAGS_START) / 8];
}

bool8 FlagSet(u16 idx)
{
    u8 *ptr = GetFlagAddr(idx);
    if (ptr != NULL)
        *ptr |= 1 << (idx & 7);
    return FALSE;
}

bool8 FlagClear(u16 idx)
{
    u8 *ptr = GetFlagAddr(idx);
    if (ptr != NULL)
        *ptr &= ~(1 << (idx & 7));
    return FALSE;
}

bool8 FlagGet(u16 idx)
{
    u8 *ptr = GetFlagAddr(idx);
    if (ptr == NULL)
        return FALSE;
    if (!(*ptr & 1 << (idx & 7)))
        return FALSE;
    return TRUE;
}

void ResetSpecialVars(void)
{
    gSpecialVar_0x8000 = 0;
    gSpecialVar_0x8001 = 0;
    gSpecialVar_0x8002 = 0;
    gSpecialVar_0x8003 = 0;
    gSpecialVar_0x8004 = 0;
    gSpecialVar_0x8005 = 0;
    gSpecialVar_0x8006 = 0;
    gSpecialVar_0x8007 = 0;
    gSpecialVar_0x8008 = 0;
    gSpecialVar_0x8009 = 0;
    gSpecialVar_0x800A = 0;
    gSpecialVar_0x800B = 0;
    gSpecialVar_Facing = 0;
    gSpecialVar_Result = 0;
    gSpecialVar_ItemId = 0;
    gSpecialVar_LastTalked = 0;
    gSpecialVar_MonBoxId = 0;
    gSpecialVar_MonBoxPos = 0;
    gSpecialVar_TextColor = 0;
    gSpecialVar_PrevTextColor = 0;
    gSpecialVar_0x8014 = 0;
}

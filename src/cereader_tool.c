#include "global.h"
#include "gflib.h"
#include "util.h"
#include "save.h"
#include "cereader_tool.h"

#define SEC30_SIZE  (offsetof(struct EReaderTrainerTowerSet, floors[4]))
#define SEC31_SIZE  (sizeof(struct EReaderTrainerTowerSet) - SEC30_SIZE)

// The trainer tower data exceeds SECTOR_DATA_SIZE. They're allowed to use the full save sector up to the counter field.
STATIC_ASSERT(SEC30_SIZE + SEC31_SIZE <= 0x1FF8 * 2, EReaderTrainerTowerSetFreeSpace);

static bool32 ValidateTrainerTowerTrainer(struct TrainerTowerFloor * floor)
{
    if (floor->floorIdx < 1 || floor->floorIdx > MAX_TRAINER_TOWER_FLOORS || floor->challengeType > CHALLENGE_TYPE_KNOCKOUT
	|| CalcByteArraySum((const u8 *)floor, offsetof(typeof(*floor), checksum)) != floor->checksum)
		return FALSE;
	
    return TRUE;
}

bool32 ValidateTrainerTowerData(struct EReaderTrainerTowerSet * ttdata)
{
    u32 numFloors = ttdata->numFloors;
    s32 i;
    if (numFloors < 1 || numFloors > MAX_TRAINER_TOWER_FLOORS)
        return FALSE;
    for (i = 0; i < numFloors; i++)
    {
        if (!ValidateTrainerTowerTrainer(&ttdata->floors[i]))
            return FALSE;
    }
    if (CalcByteArraySum((const u8 *)ttdata->floors, numFloors * sizeof(ttdata->floors[0])) != ttdata->checksum)
        return FALSE;
    return TRUE;
}

static bool32 CEReaderTool_SaveTrainerTower_r(struct EReaderTrainerTowerSet * ttdata, u8 * buffer)
{
    AGB_ASSERT_EX(ttdata->dummy == 0, ABSPATH("cereader_tool.c"), 198);
    AGB_ASSERT_EX(ttdata->id == 0, ABSPATH("cereader_tool.c"), 199)

    memset(buffer, 0, 0x1000);
    memcpy(buffer, ttdata, SEC30_SIZE);
    buffer[1] = (gSaveBlock1Ptr->trainerTower[0].unk9 + 1) % 256;
    if (TryWriteSpecialSaveSection(SECTOR_TTOWER(0), buffer) != TRUE)
        return FALSE;
    memset(buffer, 0, 0x1000);
    memcpy(buffer, (u8 *)ttdata + SEC30_SIZE, SEC31_SIZE);
    if (TryWriteSpecialSaveSection(SECTOR_TTOWER(1), buffer) != TRUE)
        return FALSE;
    return TRUE;
}

bool32 CEReaderTool_SaveTrainerTower(struct EReaderTrainerTowerSet * ttdata)
{
    u8 * buffer = AllocZeroed(0x1000);
    bool32 result = CEReaderTool_SaveTrainerTower_r(ttdata, buffer);
    Free(buffer);
    return result;
}

#include "global.h"
#include "save.h"
#include "decompress.h"
#include "overworld.h"
#include "load_save.h"
#include "malloc.h"
#include "new_game.h"
#include "task.h"
#include "gpu_regs.h"
#include "link.h"
#include "save_failed_screen.h"
#include "fieldmap.h"
#include "gba/flash_internal.h"
#include "gba/m4a_internal.h"

static u8 HandleWriteSector(u16 sectorId, const struct SaveBlockChunk *locations);
static u8 GetSaveValidStatus(const struct SaveBlockChunk *locations);

// Divide save blocks into individual chunks to be written to flash sectors
/*
 * Sector Layout:
 *
 * Sectors 0 - 13:      Save Slot 1
 * Sectors 14 - 27:     Save Slot 2
 * Sectors 28 - 29:     Hall of Fame
 * Sector 30:           e-Reader/Mystery Gift Stuff (note: e-Reader is deprecated in Emerald US)
 * Sector 31:           Recorded Battle
 *
 * There are two save slots for saving the player's game data. We alternate between
 * them each time the game is saved, so that if the current save slot is corrupt,
 * we can load the previous one. We also rotate the sectors in each save slot
 * so that the same data is not always being written to the same sector. This
 * might be done to reduce wear on the flash memory, but I'm not sure, since all
 * 14 sectors get written anyway.
 */

// (u8 *)structure was removed from the first statement of the macro in Emerald
// and Fire Red/Leaf Green. This is because malloc is used to allocate addresses
// so storing the raw addresses should not be done in the offsets information.
#define SAVEBLOCK_CHUNK(structure, chunkNum)                                   \
{                                                                              \
    .toAdd = chunkNum * SECTOR_DATA_SIZE,                                      \
    .size = sizeof(structure) >= chunkNum * SECTOR_DATA_SIZE ?                 \
    min(sizeof(structure) - chunkNum * SECTOR_DATA_SIZE, SECTOR_DATA_SIZE) : 0 \
}

struct
{
	u16 toAdd;
    u16 size;
} static const sSaveSectionOffsets[NUM_SECTORS_PER_SAVE_SLOT] =
{
    SAVEBLOCK_CHUNK(gSaveBlock2, 0), // SECTOR_ID_SAVEBLOCK2

    SAVEBLOCK_CHUNK(gSaveBlock1, 0), // SECTOR_ID_SAVEBLOCK1_START
    SAVEBLOCK_CHUNK(gSaveBlock1, 1),
    SAVEBLOCK_CHUNK(gSaveBlock1, 2),
    SAVEBLOCK_CHUNK(gSaveBlock1, 3), // SECTOR_ID_SAVEBLOCK1_END

    SAVEBLOCK_CHUNK(gPokemonStorage, 0), // SECTOR_ID_PKMN_STORAGE_START
    SAVEBLOCK_CHUNK(gPokemonStorage, 1),
    SAVEBLOCK_CHUNK(gPokemonStorage, 2),
    SAVEBLOCK_CHUNK(gPokemonStorage, 3),
    SAVEBLOCK_CHUNK(gPokemonStorage, 4),
    SAVEBLOCK_CHUNK(gPokemonStorage, 5),
    SAVEBLOCK_CHUNK(gPokemonStorage, 6),
    SAVEBLOCK_CHUNK(gPokemonStorage, 7),
    SAVEBLOCK_CHUNK(gPokemonStorage, 8), // SECTOR_ID_PKMN_STORAGE_END
};

// These will produce an error if a save struct is larger than the space
// alloted for it in the flash.
STATIC_ASSERT(sizeof(struct SaveBlock2) <= SECTOR_DATA_SIZE, SaveBlock2FreeSpace);
STATIC_ASSERT(sizeof(struct SaveBlock1) <= SECTOR_DATA_SIZE * 4, SaveBlock1FreeSpace);
STATIC_ASSERT(sizeof(struct PokemonStorage) <= SECTOR_DATA_SIZE * 9, PokemonStorageFreeSpace);

// Sector num to begin writing save data. Sectors are rotated each time the game is saved. (possibly to avoid wear on flash memory?)
u16 gFirstSaveSector;
u32 gPrevSaveCounter;
u16 gLastKnownGoodSector;
u32 gDamagedSaveSectors;
u32 gSaveCounter;
struct SaveSection *gFastSaveSection; // the pointer is in fast IWRAM but may sometimes point to the slower EWRAM.
u16 gIncrementalSectorId;
u16 gSaveFileStatus;
void (*gGameContinueCallback)(void);
struct SaveBlockChunk gRamSaveSectionLocations[NUM_SECTORS_PER_SAVE_SLOT];
u16 gSaveSucceeded;

EWRAM_DATA struct SaveSection gSaveDataBuffer = {0};

void ClearSaveData(void)
{
    u8 i;

    for (i = 0; i < TOTAL_FLASH_SECTORS; i++)
        EraseFlashSector(i);
}

void Save_ResetSaveCounters(void)
{
    gSaveCounter = 0;
    gFirstSaveSector = 0;
    gDamagedSaveSectors = 0;
}

static bool8 CheckSetSectorDamagedStatus(u8 caseId, u8 sectorNum)
{
    switch (caseId)
    {
    case ENABLE:
        gDamagedSaveSectors |= (1 << sectorNum);
        break;
    case DISABLE:
        gDamagedSaveSectors &= ~(1 << sectorNum);
        break;
    case CHECK:
        if (gDamagedSaveSectors & (1 << sectorNum))
            return TRUE;
        break;
    }
    return FALSE;
}

static u8 WriteSaveSectorOrSlot(u16 sectorId, const struct SaveBlockChunk *locations)
{
    u32 status;
    u8 i;

    gFastSaveSection = &gSaveDataBuffer;

    if (sectorId != FULL_SAVE_SLOT)  // write single sector
        status = HandleWriteSector(sectorId, locations);
    else  // write all sectors
    {
        gLastKnownGoodSector = gFirstSaveSector; // backup the current written sector before attempting to write.
        gPrevSaveCounter = gSaveCounter;
        gFirstSaveSector++;
        gFirstSaveSector %= NUM_SECTORS_PER_SAVE_SLOT; // array count save sector locations
        gSaveCounter++;
        status = SAVE_STATUS_OK;

        for (i = 0; i < NUM_SECTORS_PER_SAVE_SLOT; i++)
            HandleWriteSector(i, locations);

        // Check for any bad sectors
        if (gDamagedSaveSectors != 0) // skip the damaged sector.
        {
            status = SAVE_STATUS_ERROR;
            gFirstSaveSector = gLastKnownGoodSector;
            gSaveCounter = gPrevSaveCounter;
        }
    }
    return status;
}

static u16 CalculateChecksum(void *data, u16 size)
{
    u16 i;
    u32 checksum = 0;

    for (i = 0; i < (size / 4); i++)
    {
        // checksum += *(u32 *)data++;
        // For compatibility with modern gcc, these statements were separated.
        checksum += *(u32 *)data;
        data += 4;
    }
    return ((checksum >> 16) + checksum);
}

static u8 TryWriteSector(u8 sectorNum, u8 *data)
{
	if (ProgramFlashSectorAndVerify(sectorNum, data)) // is damaged?
    {
        CheckSetSectorDamagedStatus(ENABLE, sectorNum); // set damaged sector bits.
        return SAVE_STATUS_ERROR;
    }
    else
    {
        CheckSetSectorDamagedStatus(DISABLE, sectorNum); // unset damaged sector bits. it's safe now.
        return SAVE_STATUS_OK;
    }
}

static u8 HandleWriteSector(u16 sectorId, const struct SaveBlockChunk *locations)
{
    u16 i, sectorNum, size;
    u8 *data;

    // select sector number
    sectorNum = sectorId + gFirstSaveSector;
    sectorNum %= NUM_SECTORS_PER_SAVE_SLOT;
    // select save slot
    sectorNum += NUM_SECTORS_PER_SAVE_SLOT * (gSaveCounter % NUM_SAVE_SLOTS);

    data = locations[sectorId].data;
    size = locations[sectorId].size;

    // clear buffer
    for (i = 0; i < sizeof(struct SaveSection); i++)
        ((char *)gFastSaveSection)[i] = 0;

    gFastSaveSection->id = sectorId;
    gFastSaveSection->signature = FILE_SIGNATURE;
    gFastSaveSection->counter = gSaveCounter;

    for (i = 0; i < size; i++)
        gFastSaveSection->data[i] = data[i];

    gFastSaveSection->checksum = CalculateChecksum(data, size);
	
    return TryWriteSector(sectorNum, gFastSaveSection->data);
}

static u8 HandleWriteSectorNBytes(u8 sectorId, u8 *data, u16 size)
{
    u16 i;
    struct SaveSection *sector = &gSaveDataBuffer;

    for (i = 0; i < sizeof(struct SaveSection); i++)
        ((char *)sector)[i] = 0;

    sector->signature = FILE_SIGNATURE;

    for (i = 0; i < size; i++)
        sector->data[i] = data[i];

    sector->id = CalculateChecksum(data, size); // though this appears to be incorrect, it might be some sector checksum instead of a whole save checksum and only appears to be relevent to HOF data, if used.
    
	return TryWriteSector(sectorId, sector->data);
}

static void RestoreSaveBackupVars(void)
{
    gFastSaveSection = &gSaveDataBuffer;
    gLastKnownGoodSector = gFirstSaveSector;
    gPrevSaveCounter = gSaveCounter;
    gIncrementalSectorId = 0;
    gDamagedSaveSectors = 0;
}

static void RestoreSaveBackupVarsAndIncrement(void)
{
	RestoreSaveBackupVars();
    gFirstSaveSector++;
    gFirstSaveSector %= NUM_SECTORS_PER_SAVE_SLOT;
    gSaveCounter++;
}

static u8 HandleWriteIncrementalSector(u16 numSectors, const struct SaveBlockChunk *locations)
{
    u8 status;

    if (gIncrementalSectorId < numSectors - 1)
    {
        status = SAVE_STATUS_OK;
        HandleWriteSector(gIncrementalSectorId, locations);
        gIncrementalSectorId++;
		
        if (gDamagedSaveSectors)
        {
            status = SAVE_STATUS_ERROR;
            gFirstSaveSector = gLastKnownGoodSector;
            gSaveCounter = gPrevSaveCounter;
        }
    }
    else
        status = SAVE_STATUS_ERROR;

    return status;
}

static u8 HandleReplaceSector(u16 sectorId, const struct SaveBlockChunk *locations)
{
    u16 i, sectorNum, size;
    u8 *data;
    u8 status;

    // select sector number
    sectorNum = sectorId + gFirstSaveSector;
    sectorNum %= NUM_SECTORS_PER_SAVE_SLOT;
    // select save slot
    sectorNum += NUM_SECTORS_PER_SAVE_SLOT * (gSaveCounter % NUM_SAVE_SLOTS);

    data = locations[sectorId].data;
    size = locations[sectorId].size;

    // clear temp save section.
    for (i = 0; i < sizeof(struct SaveSection); i++)
        ((char *)gFastSaveSection)[i] = 0;

    gFastSaveSection->id = sectorId;
    gFastSaveSection->signature = FILE_SIGNATURE;
    gFastSaveSection->counter = gSaveCounter;

    // set temp section's data.
    for (i = 0; i < size; i++)
        gFastSaveSection->data[i] = data[i];

    // calculate checksum.
    gFastSaveSection->checksum = CalculateChecksum(data, size);

    EraseFlashSector(sectorNum);

    status = SAVE_STATUS_OK;

    for (i = 0; i < sizeof(struct UnkSaveSection); i++)
    {
        if (ProgramFlashByte(sectorNum, i, gFastSaveSection->data[i]))
        {
            status = SAVE_STATUS_ERROR;
            break;
        }
    }

    if (status == SAVE_STATUS_ERROR)
    {
        CheckSetSectorDamagedStatus(ENABLE, sectorNum);
        return SAVE_STATUS_ERROR;
    }
    else
    {
        status = SAVE_STATUS_OK;

        for (i = 0; i < 7; i++)
        {
            if (ProgramFlashByte(sectorNum, 0xFF9 + i, ((u8 *)gFastSaveSection)[0xFF9 + i]))
            {
                status = SAVE_STATUS_ERROR;
                break;
            }
        }

        if (status == SAVE_STATUS_ERROR)
        {
            CheckSetSectorDamagedStatus(ENABLE, sectorNum);
            return SAVE_STATUS_ERROR;
        }
        else
        {
            CheckSetSectorDamagedStatus(DISABLE, sectorNum);
            return SAVE_STATUS_OK;
        }
    }
}

static u8 HandleReplaceSectorAndVerify(u16 sectorId, const struct SaveBlockChunk *locations)
{
    u8 status = SAVE_STATUS_OK;

    HandleReplaceSector(sectorId - 1, locations);

    if (gDamagedSaveSectors)
    {
        status = SAVE_STATUS_ERROR;
        gFirstSaveSector = gLastKnownGoodSector;
        gSaveCounter = gPrevSaveCounter;
    }
    return status;
}

static u8 CopySectorSignatureByte(u16 sectorId)
{
    u16 sector;

    // select sector number
    sector = sectorId + gFirstSaveSector - 1;
    sector %= NUM_SECTORS_PER_SAVE_SLOT;
    // select save slot
    sector += NUM_SECTORS_PER_SAVE_SLOT * (gSaveCounter % NUM_SAVE_SLOTS);

    if (ProgramFlashByte(sector, sizeof(struct UnkSaveSection), ((u8 *)gFastSaveSection)[sizeof(struct UnkSaveSection)]))
    {
        // sector is damaged, so enable the bit in gDamagedSaveSectors and restore the last written sector and save counter.
        CheckSetSectorDamagedStatus(ENABLE, sector);
        gFirstSaveSector = gLastKnownGoodSector;
        gSaveCounter = gPrevSaveCounter;
        return SAVE_STATUS_ERROR;
    }
    else
    {
        CheckSetSectorDamagedStatus(DISABLE, sector);
        return SAVE_STATUS_OK;
    }
}

static u8 WriteSectorSignatureByte(u16 sectorId)
{
    u16 sector;

    sector = sectorId + gFirstSaveSector - 1;
    sector %= NUM_SECTORS_PER_SAVE_SLOT;
    sector += NUM_SECTORS_PER_SAVE_SLOT * (gSaveCounter % NUM_SAVE_SLOTS);

    if (ProgramFlashByte(sector, sizeof(struct UnkSaveSection), 0x25))
    {
        // sector is damaged, so enable the bit in gDamagedSaveSectors and restore the last written sector and save counter.
        CheckSetSectorDamagedStatus(ENABLE, sector);
        gFirstSaveSector = gLastKnownGoodSector;
        gSaveCounter = gPrevSaveCounter;
        return SAVE_STATUS_ERROR;
    }
    else
    {
        CheckSetSectorDamagedStatus(DISABLE, sector);
        return SAVE_STATUS_OK;
    }
}

static void ReadFlashSector(u8 sectorId, struct SaveSection *locations)
{
    ReadFlash(sectorId, 0, locations->data, sizeof(struct SaveSection));
}

static void CopySaveSlotData(const struct SaveBlockChunk *locations)
{
    u16 id, i, j, checksum;
    u16 sector = NUM_SECTORS_PER_SAVE_SLOT * (gSaveCounter % NUM_SAVE_SLOTS);

    for (i = 0; i < NUM_SECTORS_PER_SAVE_SLOT; i++)
    {
        ReadFlashSector(i + sector, gFastSaveSection);
		
        id = gFastSaveSection->id;
        if (id == 0)
            gFirstSaveSector = i;
		
        checksum = CalculateChecksum(gFastSaveSection->data, locations[id].size);
		
        if (gFastSaveSection->signature == FILE_SIGNATURE && gFastSaveSection->checksum == checksum)
        {
            for (j = 0; j < locations[id].size; j++)
                locations[id].data[j] = gFastSaveSection->data[j];
        }
    }
}

static u8 TryLoadSaveSlot(u16 sectorId, const struct SaveBlockChunk *locations)
{
    u8 status;
	
    gFastSaveSection = &gSaveDataBuffer;
	
    if (sectorId != FULL_SAVE_SLOT)
        status = SAVE_STATUS_ERROR;
    else
    {
        status = GetSaveValidStatus(locations);
        CopySaveSlotData(locations);
    }
    return status;
}

static u8 GetSaveValidStatus(const struct SaveBlockChunk *locations)
{
    u16 sector;
    bool8 signatureValid;
    u16 checksum;
    u32 slot1saveCounter = 0;
    u32 slot2saveCounter = 0;
    u8 slot1Status;
    u8 slot2Status;
    u32 validSectors;
    const u32 ALL_SECTORS = (1 << NUM_SECTORS_PER_SAVE_SLOT) - 1;  // bitmask of all saveblock sectors

    // check save slot 1.
    validSectors = 0;
    signatureValid = FALSE;
    for (sector = 0; sector < NUM_SECTORS_PER_SAVE_SLOT; sector++)
    {
        ReadFlashSector(sector, gFastSaveSection);
		
        if (gFastSaveSection->signature == FILE_SIGNATURE)
        {
            signatureValid = TRUE;
            checksum = CalculateChecksum(gFastSaveSection->data, locations[gFastSaveSection->id].size);
            
			if (gFastSaveSection->checksum == checksum)
            {
                slot1saveCounter = gFastSaveSection->counter;
                validSectors |= 1 << gFastSaveSection->id;
            }
        }
    }

    if (signatureValid)
    {
        if (validSectors == ALL_SECTORS)
            slot1Status = SAVE_STATUS_OK;
        else
            slot1Status = SAVE_STATUS_ERROR;
    }
    else
        slot1Status = SAVE_STATUS_EMPTY;

    // check save slot 2.
    validSectors = 0;
    signatureValid = FALSE;
    for (sector = 0; sector < NUM_SECTORS_PER_SAVE_SLOT; sector++)
    {
        ReadFlashSector(NUM_SECTORS_PER_SAVE_SLOT + sector, gFastSaveSection);
        
		if (gFastSaveSection->signature == FILE_SIGNATURE)
        {
            signatureValid = TRUE;
            checksum = CalculateChecksum(gFastSaveSection->data, locations[gFastSaveSection->id].size);
            
			if (gFastSaveSection->checksum == checksum)
            {
                slot2saveCounter = gFastSaveSection->counter;
                validSectors |= 1 << gFastSaveSection->id;
            }
        }
    }

    if (signatureValid)
    {
        if (validSectors == ALL_SECTORS)
            slot2Status = SAVE_STATUS_OK;
        else
            slot2Status = SAVE_STATUS_ERROR;
    }
    else
        slot2Status = SAVE_STATUS_EMPTY;

    if (slot1Status == SAVE_STATUS_OK && slot2Status == SAVE_STATUS_OK)
    {
        // Choose counter of the most recent save file
        if ((slot1saveCounter == -1 && slot2saveCounter == 0) || (slot1saveCounter == 0 && slot2saveCounter == -1))
        {
            if ((unsigned)(slot1saveCounter + 1) < (unsigned)(slot2saveCounter + 1))
                gSaveCounter = slot2saveCounter;
            else
                gSaveCounter = slot1saveCounter;
        }
        else
        {
            if (slot1saveCounter < slot2saveCounter)
                gSaveCounter = slot2saveCounter;
            else
                gSaveCounter = slot1saveCounter;
        }
        return SAVE_STATUS_OK;
    }

    if (slot1Status == SAVE_STATUS_OK)
    {
        gSaveCounter = slot1saveCounter;
		
        if (slot2Status == SAVE_STATUS_ERROR)
            return SAVE_STATUS_ERROR;
        else
            return SAVE_STATUS_OK;
    }

    if (slot2Status == SAVE_STATUS_OK)
    {
        gSaveCounter = slot2saveCounter;
		
        if (slot1Status == SAVE_STATUS_ERROR)
            return SAVE_STATUS_ERROR;
        else
            return SAVE_STATUS_OK;
    }

    if (slot1Status == SAVE_STATUS_EMPTY && slot2Status == SAVE_STATUS_EMPTY)
    {
        gSaveCounter = 0;
        gFirstSaveSector = 0;
        return SAVE_STATUS_EMPTY;
    }

    gSaveCounter = 0;
    gFirstSaveSector = 0;
    return SAVE_STATUS_INVALID;
}

static u8 TryLoadSaveSector(u8 sectorId, u8 *data, u16 size)
{
    u16 i;
    struct SaveSection *section = &gSaveDataBuffer;

    ReadFlashSector(sectorId, section);
	
    if (section->signature == FILE_SIGNATURE)
    {
        u16 checksum = CalculateChecksum(section->data, size);
        
		if (section->id == checksum)
        {
            for (i = 0; i < size; i++)
                data[i] = section->data[i];
			
            return SAVE_STATUS_OK;
        }
        else
            return SAVE_STATUS_INVALID;
    }
    else
        return SAVE_STATUS_EMPTY;
}

static void UpdateSaveAddresses(void)
{
    int i = 0;

    gRamSaveSectionLocations[i].data = (void*)(gSaveBlock2Ptr) + sSaveSectionOffsets[i].toAdd;
    gRamSaveSectionLocations[i].size = sSaveSectionOffsets[i].size;

    for (i = SECTOR_ID_SAVEBLOCK1_START; i <= SECTOR_ID_SAVEBLOCK1_END; i++)
    {
        gRamSaveSectionLocations[i].data = (void*)(gSaveBlock1Ptr) + sSaveSectionOffsets[i].toAdd;
        gRamSaveSectionLocations[i].size = sSaveSectionOffsets[i].size;
    }

    for (; i <= SECTOR_ID_PKMN_STORAGE_END; i++)
    {
        gRamSaveSectionLocations[i].data = (void*)(gPokemonStoragePtr) + sSaveSectionOffsets[i].toAdd;
        gRamSaveSectionLocations[i].size = sSaveSectionOffsets[i].size;
    }
}

void HandleSavingData(u8 saveType)
{
    u8 i;
    u32 *backupPtr = gMain.vblankCounter1;
    u8 *tempAddr;

    gMain.vblankCounter1 = NULL;
    UpdateSaveAddresses();
	
    switch (saveType)
    {
    case SAVE_HALL_OF_FAME:
        if (GetGameStat(GAME_STAT_ENTERED_HOF) < 999)
            IncrementGameStat(GAME_STAT_ENTERED_HOF);
		
        tempAddr = gDecompressionBuffer;
        HandleWriteSectorNBytes(SECTOR_ID_HOF_1, tempAddr, SECTOR_DATA_SIZE);
        HandleWriteSectorNBytes(SECTOR_ID_HOF_2, tempAddr + SECTOR_DATA_SIZE, SECTOR_DATA_SIZE);
        // fallthrough
    case SAVE_NORMAL: // normal save. also called by overwriting your own save.
    default:
        SaveSerializedGame();
        WriteSaveSectorOrSlot(FULL_SAVE_SLOT, gRamSaveSectionLocations);
        break;
    case SAVE_LINK:
        SaveSerializedGame();
		
		// only SaveBlock2 and SaveBlock1 (ignores storage in PC)
        for(i = SECTOR_ID_SAVEBLOCK2; i <= SECTOR_ID_SAVEBLOCK1_END; i++)
            WriteSaveSectorOrSlot(i, gRamSaveSectionLocations);
		
        break;
    case SAVE_OVERWRITE_DIFFERENT_FILE:
        for (i = SECTOR_ID_HOF_1; i < TOTAL_FLASH_SECTORS; i++)
            EraseFlashSector(i);
		
        SaveSerializedGame();
        WriteSaveSectorOrSlot(FULL_SAVE_SLOT, gRamSaveSectionLocations);
        break;
    }
    gMain.vblankCounter1 = backupPtr;
}

void TrySavingData(u8 saveType)
{
    if (!gFlashMemoryPresent)
		gSaveSucceeded = SAVE_STATUS_ERROR;
	else
	{
		HandleSavingData(saveType);
		
        if (gDamagedSaveSectors)
		{
            DoSaveFailedScreen(saveType);
			gSaveSucceeded = SAVE_STATUS_ERROR;
		}
		else
			gSaveSucceeded = SAVE_STATUS_OK;
	}
}

void SaveGame_AfterLinkTrade(void)
{
    if (gFlashMemoryPresent)
	{
		UpdateSaveAddresses();
		SaveSerializedGame();
		RestoreSaveBackupVarsAndIncrement();
	}
}

bool8 AfterLinkTradeSaveFailed(void) 
{
    u8 status = HandleWriteIncrementalSector(NUM_SECTORS_PER_SAVE_SLOT, gRamSaveSectionLocations);
	
    if (gDamagedSaveSectors)
        DoSaveFailedScreen(SAVE_NORMAL);
	
    return (status == SAVE_STATUS_ERROR);
}

void ClearSaveAfterLinkTradeSaveFailure(void)
{
    HandleReplaceSectorAndVerify(NUM_SECTORS_PER_SAVE_SLOT, gRamSaveSectionLocations);
	
    if (gDamagedSaveSectors)
        DoSaveFailedScreen(SAVE_NORMAL);
}

void LinkTradeSetLastSectorSignature(void)
{
    CopySectorSignatureByte(NUM_SECTORS_PER_SAVE_SLOT);
	
    if (gDamagedSaveSectors)
        DoSaveFailedScreen(SAVE_NORMAL);
}

void WriteSaveBlock2(void)
{
    if (gFlashMemoryPresent)
	{
		UpdateSaveAddresses();
		SaveSerializedGame();
		RestoreSaveBackupVars();
		HandleReplaceSectorAndVerify(gIncrementalSectorId + 1, gRamSaveSectionLocations);
	}
}

// Used in conjunction with WriteSaveBlock2 to write both for certain link saves.
// This is called repeatedly in a task, writing one sector of SaveBlock1 each time it is called.
// Returns TRUE when all sectors of SaveBlock1 have been written.
bool8 WriteSaveBlock1Sector(void)
{
	bool8 finished;
    u16 sectorId = ++gIncrementalSectorId;
	
    if (sectorId <= SECTOR_ID_SAVEBLOCK1_END)
    {
        HandleReplaceSectorAndVerify(gIncrementalSectorId + 1, gRamSaveSectionLocations);
        WriteSectorSignatureByte(sectorId);
		finished = FALSE;
    }
    else
    {
        WriteSectorSignatureByte(sectorId);
        finished = TRUE;
    }
    if (gDamagedSaveSectors)
        DoSaveFailedScreen(SAVE_LINK);
	
    return finished;
}

u8 Save_LoadGameData(u8 saveType)
{
    u8 result;

    if (!gFlashMemoryPresent)
    {
        gSaveFileStatus = SAVE_STATUS_NO_FLASH;
        return SAVE_STATUS_ERROR;
    }

    UpdateSaveAddresses();
	
    switch (saveType)
    {
    case SAVE_NORMAL:
    default:
        result = TryLoadSaveSlot(FULL_SAVE_SLOT, gRamSaveSectionLocations);
        LoadSerializedGame();
        gSaveFileStatus = result;
        gGameContinueCallback = NULL;
        break;
    case SAVE_HALL_OF_FAME:
        result = TryLoadSaveSector(SECTOR_ID_HOF_1, gDecompressionBuffer, SECTOR_DATA_SIZE);
		
        if (result == SAVE_STATUS_OK)
            result = TryLoadSaveSector(SECTOR_ID_HOF_2, gDecompressionBuffer + SECTOR_DATA_SIZE, SECTOR_DATA_SIZE);
        break;
    }

    return result;
}

void Task_LinkSave(u8 taskId)
{
    switch (gTasks[taskId].data[0])
    {
    case 0:
        gSoftResetDisabled = TRUE;
        gTasks[taskId].data[0] = 1;
        break;
    case 1:
        SetLinkStandbyCallback();
        gTasks[taskId].data[0] = 2;
        break;
    case 2:
        if (IsLinkTaskFinished())
        {
            SaveMapView();
            gTasks[taskId].data[0] = 3;
        }
        break;
    case 3:
        SetContinueGameWarpStatusToDynamicWarp();
        SaveGame_AfterLinkTrade();
        gTasks[taskId].data[0] = 4;
        break;
    case 4:
        if (++gTasks[taskId].data[1] == 5)
        {
            gTasks[taskId].data[1] = 0;
            gTasks[taskId].data[0] = 5;
        }
        break;
    case 5:
        if (AfterLinkTradeSaveFailed())
            gTasks[taskId].data[0] = 6;
        else
            gTasks[taskId].data[0] = 4;
        break;
    case 6:
        ClearSaveAfterLinkTradeSaveFailure();
        gTasks[taskId].data[0] = 7;
        break;
    case 7:
        ClearContinueGameWarpStatus2();
        SetLinkStandbyCallback();
        gTasks[taskId].data[0] = 8;
        break;
    case 8:
        if (IsLinkTaskFinished())
        {
            LinkTradeSetLastSectorSignature();
            gTasks[taskId].data[0] = 9;
        }
        break;
    case 9:
        SetLinkStandbyCallback();
        gTasks[taskId].data[0] = 10;
        break;
    case 10:
        if (IsLinkTaskFinished())
            gTasks[taskId].data[0]++;
        break;
    case 11:
        if (++gTasks[taskId].data[1] > 5)
        {
            gSoftResetDisabled = FALSE;
            DestroyTask(taskId);
        }
        break;
    }
}

void ResetSaveHeap(void)
{
    u16 imeBackup = REG_IME;
    
    REG_IME = 0;
    RegisterRamReset(RESET_EWRAM);
    ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_FORCED_BLANK);
    REG_IME = imeBackup;
    gMain.inBattle = FALSE;
    SetSaveBlocksPointers();
    ResetMenuAndMonGlobals();
    Save_ResetSaveCounters();
    Save_LoadGameData(SAVE_NORMAL);
	
    if (gSaveFileStatus == SAVE_STATUS_EMPTY || gSaveFileStatus == SAVE_STATUS_INVALID)
        Sav2_ClearSetDefault();
	
    SetPokemonCryStereo(gSaveBlock2Ptr->optionsSound);
    InitHeap(gHeap, HEAP_SIZE);
    SetMainCallback2(CB2_ContinueSavedGame);
}

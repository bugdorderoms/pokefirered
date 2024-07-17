#ifndef GUARD_SAVE_H
#define GUARD_SAVE_H

#include "global.h"

#define SAVE_STATUS_EMPTY    0
#define SAVE_STATUS_OK       1
#define SAVE_STATUS_INVALID  2
//
#define SAVE_STATUS_NO_FLASH 4
#define SAVE_STATUS_ERROR    0xFF

#define FILE_SIGNATURE 0x08012025  // signature value to determine if a sector is in use

#define NUM_SAVE_SLOTS 2

enum
{
    SAVE_NORMAL,
    SAVE_LINK,
    SAVE_HALL_OF_FAME,
    SAVE_OVERWRITE_DIFFERENT_FILE,
};

// Each 4 KiB flash sector contains 3968 bytes of actual data followed by a 128 byte footer
#define SECTOR_DATA_SIZE 4084

struct SaveBlockChunk
{
    u8 *data;
    u16 size;
};

struct SaveSection
{
    u8 data[SECTOR_DATA_SIZE];
    u16 id;
    u16 checksum;
    u32 signature;
    u32 counter;
}; // size is 0x1000

// headless save section?
struct UnkSaveSection
{
    u8 data[SECTOR_DATA_SIZE];
    u32 signature;
}; // size is 0xFF8

// Special sector id value for certain save functions
// to indicate that all sectors should be used
// instead of a specific sector.
#define FULL_SAVE_SLOT 0xFFFF

#define SECTOR_ID_SAVEBLOCK2          0
#define SECTOR_ID_SAVEBLOCK1_START    1
#define SECTOR_ID_SAVEBLOCK1_END      4
#define SECTOR_ID_PKMN_STORAGE_START  5
#define SECTOR_ID_PKMN_STORAGE_END   13
#define NUM_SECTORS_PER_SAVE_SLOT    14
// Save Slot 1: 0-13;  Save Slot 2: 14-27
#define SECTOR_ID_HOF_1              28
#define SECTOR_ID_HOF_2              29
#define SECTOR_ID_TRAINER_TOWER_1    30 // Unused
#define SECTOR_ID_TRAINER_TOWER_2    31 // Unused
#define TOTAL_FLASH_SECTORS 		 32

// SetSectorDamagedStatus states
enum
{
    ENABLE,
    DISABLE,
    CHECK
};

extern u16 gFirstSaveSector;
extern u32 gPrevSaveCounter;
extern u16 gLastKnownGoodSector;
extern u32 gDamagedSaveSectors;
extern u32 gSaveCounter;
extern struct SaveSection *gFastSaveSection; // the pointer is in fast IWRAM but may sometimes point to the slower EWRAM.
extern u16 gIncrementalSectorId;
extern u16 gSaveFileStatus;
extern void (*gGameContinueCallback)(void);
extern struct SaveBlockChunk gRamSaveSectionLocations[NUM_SECTORS_PER_SAVE_SLOT];
extern u16 gSaveSucceeded;

extern struct SaveSection gSaveDataBuffer;

void ClearSaveData(void);
void Save_ResetSaveCounters(void);
void HandleSavingData(u8 saveType);
void TrySavingData(u8 saveType);
void SaveGame_AfterLinkTrade(void);
bool8 AfterLinkTradeSaveFailed(void);
void ClearSaveAfterLinkTradeSaveFailure(void);
void LinkTradeSetLastSectorSignature(void);
void WriteSaveBlock2(void);
bool8 WriteSaveBlock1Sector(void);
u8 Save_LoadGameData(u8 saveType);
void Task_LinkSave(u8 taskId);
void ResetSaveHeap(void);

#endif // GUARD_SAVE_H

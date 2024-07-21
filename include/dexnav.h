#ifndef GUARD_DEXNAV_H
#define GUARD_DEXNAV_H

#define DEXNAV_TIMEOUT                  15  // 15 seconds is the time out. Max of 1092 seconds allowed
#define SNEAKING_PROXIMITY              4   // Tile amount
#define CREEPING_PROXIMITY              2
#define MAX_PROXIMITY                   20

#define DEXNAV_CHAIN_MAX                100

//// SEARCH PROBABILITIES
// see https://m.bulbapedia.bulbagarden.net/wiki/DexNav#Benefits
// Chance of encountering egg move at search levels
#define SEARCHLEVEL0_MOVECHANCE         0
#define SEARCHLEVEL5_MOVECHANCE         21
#define SEARCHLEVEL10_MOVECHANCE        46
#define SEARCHLEVEL25_MOVECHANCE        58
#define SEARCHLEVEL50_MOVECHANCE        63
#define SEARCHLEVEL100_MOVECHANCE       83
// Chance of encountering Hidden Abilities at search levels
#define SEARCHLEVEL0_ABILITYCHANCE      0
#define SEARCHLEVEL5_ABILITYCHANCE      0
#define SEARCHLEVEL10_ABILITYCHANCE     5
#define SEARCHLEVEL25_ABILITYCHANCE     15
#define SEARCHLEVEL50_ABILITYCHANCE     20
#define SEARCHLEVEL100_ABILITYCHANCE    23
// Chance of encountering held item
#define SEARCHLEVEL0_ITEM               0
#define SEARCHLEVEL5_ITEM               0
#define SEARCHLEVEL10_ITEM              1
#define SEARCHLEVEL25_ITEM              7
#define SEARCHLEVEL50_ITEM              6
#define SEARCHLEVEL100_ITEM             12
// Chance of encountering one star potential
#define SEARCHLEVEL0_ONESTAR            0
#define SEARCHLEVEL5_ONESTAR            14
#define SEARCHLEVEL10_ONESTAR           17
#define SEARCHLEVEL25_ONESTAR           17
#define SEARCHLEVEL50_ONESTAR           15
#define SEARCHLEVEL100_ONESTAR          8
// Chance of encountering two star potential
#define SEARCHLEVEL0_TWOSTAR            0
#define SEARCHLEVEL5_TWOSTAR            1
#define SEARCHLEVEL10_TWOSTAR           9
#define SEARCHLEVEL25_TWOSTAR           16
#define SEARCHLEVEL50_TWOSTAR           17
#define SEARCHLEVEL100_TWOSTAR          24
// Chance of encountering three star potential
#define SEARCHLEVEL0_THREESTAR          0
#define SEARCHLEVEL5_THREESTAR          0
#define SEARCHLEVEL10_THREESTAR         1
#define SEARCHLEVEL25_THREESTAR         7
#define SEARCHLEVEL50_THREESTAR         6
#define SEARCHLEVEL100_THREESTAR        12

// GUI Info
#define ROW_WATER       0
#define ROW_LAND_TOP    1
#define ROW_LAND_BOT    2
#define ROWS_COUNT      3

#define ROW_WATER_ICON_X        30
#define ROW_WATER_ICON_Y        48

#define ROW_LAND_ICON_X         20
#define ROW_LAND_TOP_ICON_Y     92
#define ROW_LAND_BOT_ICON_Y     (ROW_LAND_TOP_ICON_Y + 28)

#define COL_WATER_COUNT         5
#define COL_LAND_COUNT          6

#define COL_WATER_MAX           (COL_WATER_COUNT - 1)
#define COL_LAND_MAX            (COL_LAND_COUNT - 1)

// SEARCH INFO
#define SCANSTART_X             0
#define SCANSTART_Y             0
#define SCANSIZE_X              12
#define SCANSIZE_Y              12

#define SPECIES_INFO_Y          3
#define SEARCH_LEVEL_Y          (SPECIES_INFO_Y + 24)
#define CHAIN_BONUS_Y           (SEARCH_LEVEL_Y + 24)
#define HA_INFO_Y               (CHAIN_BONUS_Y + 24)

#define MON_LEVEL_NONEXISTENT   255 // if mon not in area GetEncounterLevel returns this to exit the search

// funcs
void DexNavGuiInit(MainCallback callback);
bool8 TryStartDexnavSearch(void);
bool8 IsDexNavSearchActive(void);
void ResetDexNavSearch(void);
void IncrementOrResetDexNavChain(bool8 increment);

// ewram
extern bool8 gDexnavBattle;

#endif // GUARD_DEXNAV_H

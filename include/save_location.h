#ifndef GUARD_SAVE_LOCATION_H
#define GUARD_SAVE_LOCATION_H

// specialSaveWarp flags
#define CONTINUE_GAME_WARP           Bit(0)
#define POKECENTER_SAVEWARP          Bit(1)
#define LOBBY_SAVEWARP               Bit(2)
#define UNK_SPECIAL_SAVE_WARP_FLAG_3 Bit(3)
#define CHAMPION_SAVEWARP            Bit(7)

void TrySetMapSaveWarpStatus(void);
bool32 IsCurMapPokeCenter(void);
void SetUnlockedPokedexFlags(void);
void SetPostgameFlags(void);

#endif // GUARD_SAVE_LOCATION_H

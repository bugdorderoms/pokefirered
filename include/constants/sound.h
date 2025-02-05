#ifndef GUARD_CONSTANTS_SOUND_H
#define GUARD_CONSTANTS_SOUND_H

#define FANFARE_LEVEL_UP       0
#define FANFARE_OBTAIN_ITEM    1
#define FANFARE_EVOLVED        2
#define FANFARE_OBTAIN_TMHM    3
#define FANFARE_HEAL           4
#define FANFARE_OBTAIN_BADGE   5
#define FANFARE_MOVE_DELETED   6
#define FANFARE_OBTAIN_BERRY   7
#define FANFARE_SLOTS_JACKPOT  8
#define FANFARE_SLOTS_WIN      9
#define FANFARE_TOO_BAD        10
#define FANFARE_POKEFLUTE      11
#define FANFARE_KEY_ITEM       12
#define FANFARE_DEX_EVAL       13

#define CRY_MODE_NORMAL        0 // Default
#define CRY_MODE_DOUBLES       1 // Shortened cry for double battles
#define CRY_MODE_ENCOUNTER     2 // Used when starting a static encounter, or when a Pokémon is "aggressive"
#define CRY_MODE_HIGH_PITCH    3 // Highest pitch mode
#define CRY_MODE_ECHO_END      4 // For 2nd cry used by the move Hyper Voice. Played in reverse
#define CRY_MODE_FAINT         5 // Used when a Pokémon faints
#define CRY_MODE_ECHO_START    6 // For 1st cry used by the move Hyper Voice
#define CRY_MODE_ROAR_1        7 // For 1st cry used by the move Roar
#define CRY_MODE_ROAR_2        8 // For 2nd cry used by the move Roar
#define CRY_MODE_GROWL_1       9 // For 1st cry used by the move Growl. Played in reverse
#define CRY_MODE_GROWL_2       10 // For 2nd cry used by the move Growl
#define CRY_MODE_WEAK          11 // Used when a Pokémon is unhealthy
#define CRY_MODE_WEAK_DOUBLES  12 // Equivalent to CRY_MODE_DOUBLES for CRY_MODE_WEAK

#define CRY_PRIORITY_NORMAL  10
#define CRY_PRIORITY_AMBIENT 1

#endif // GUARD_CONSTANTS_SOUND_H
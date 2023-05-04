#ifndef GUARD_CONSTANTS_BATTLE_H
#define GUARD_CONSTANTS_BATTLE_H

/*
 * A battler may be in one of four positions on the field. The first bit determines
 * what side the battler is on, either the player's side or the opponent's side.
 * The second bit determines what flank the battler is on, either the left or right.
 * Note that the opponent's flanks are drawn corresponding to their perspective, so
 * their right mon appears on the left, and their left mon appears on the right.
 * The battler ID is usually the same as the position, except in the case of link battles.
 *
 *   + ------------------------- +
 *   |           Opponent's side |
 *   |            Right    Left  |
 *   |              3       1    |
 *   |                           |
 *   | Player's side             |
 *   |  Left   Right             |
 *   |   0       2               |
 *   ----------------------------+
 *   |                           |
 *   |                           |
 *   +---------------------------+
 */

#define MAX_BATTLERS_COUNT  4

#define B_POSITION_PLAYER_LEFT        0
#define B_POSITION_OPPONENT_LEFT      1
#define B_POSITION_PLAYER_RIGHT       2
#define B_POSITION_OPPONENT_RIGHT     3

// These macros can be used with either battler ID or positions to get the partner or the opposite mon
#define BATTLE_OPPOSITE(id) ((id) ^ BIT_SIDE)
#define BATTLE_PARTNER(id) ((id) ^ BIT_FLANK)

#define B_SIDE_PLAYER     0
#define B_SIDE_OPPONENT   1

#define B_FLANK_LEFT 0
#define B_FLANK_RIGHT 1

#define BIT_SIDE        1
#define BIT_FLANK       2

// Battle Type Flags
#define BATTLE_TYPE_DOUBLE           (1 << 0)
#define BATTLE_TYPE_LINK             (1 << 1)
#define BATTLE_TYPE_IS_MASTER        (1 << 2) // In not-link battles, it's always set.
#define BATTLE_TYPE_TRAINER          (1 << 3)
#define BATTLE_TYPE_FIRST_BATTLE     (1 << 4)
#define BATTLE_TYPE_LINK_ESTABLISHED (1 << 5) // Set when the link battle setup callback finishes.
#define BATTLE_TYPE_MULTI            (1 << 6)
#define BATTLE_TYPE_SAFARI           (1 << 7)
#define BATTLE_TYPE_BATTLE_TOWER     (1 << 8)
#define BATTLE_TYPE_OLD_MAN_TUTORIAL (1 << 9)
#define BATTLE_TYPE_ROAMER           (1 << 10)
#define BATTLE_TYPE_EREADER_TRAINER  (1 << 11)
#define BATTLE_TYPE_KYOGRE_GROUDON   (1 << 12)
#define BATTLE_TYPE_LEGENDARY        (1 << 13)
#define BATTLE_TYPE_GHOST_UNVEILED   (1 << 13) // Re-use of BATTLE_TYPE_LEGENDARY, when combined with BATTLE_TYPE_GHOST
#define BATTLE_TYPE_REGI             (1 << 14)
#define BATTLE_TYPE_GHOST            (1 << 15)
#define BATTLE_TYPE_POKEDUDE         (1 << 16)
#define BATTLE_TYPE_WILD_SCRIPTED    (1 << 17)
#define BATTLE_TYPE_LEGENDARY_FRLG   (1 << 18)
#define BATTLE_TYPE_TRAINER_TOWER    (1 << 19)

#define IS_BATTLE_TYPE_GHOST_WITHOUT_SCOPE(flags) ((flags) & BATTLE_TYPE_GHOST && !((flags) & BATTLE_TYPE_GHOST_UNVEILED))
#define IS_BATTLE_TYPE_GHOST_WITH_SCOPE(flags) ((flags) & BATTLE_TYPE_GHOST && (flags) & BATTLE_TYPE_GHOST_UNVEILED)

#define RIVAL_BATTLE_HEAL_AFTER  1
#define RIVAL_BATTLE_TUTORIAL    3

// Battle Outcome defines
#define B_OUTCOME_WON                  0x1
#define B_OUTCOME_LOST                 0x2
#define B_OUTCOME_DREW                 0x3
#define B_OUTCOME_RAN                  0x4
#define B_OUTCOME_PLAYER_TELEPORTED    0x5
#define B_OUTCOME_MON_FLED             0x6
#define B_OUTCOME_CAUGHT               0x7
#define B_OUTCOME_NO_SAFARI_BALLS      0x8
#define B_OUTCOME_FORFEITED            0x9
#define B_OUTCOME_MON_TELEPORTED       0xA
#define B_OUTCOME_LINK_BATTLE_RAN      0x80

// Non-volatile status conditions
// These persist remain outside of battle and after switching out
#define STATUS1_NONE             0
#define STATUS1_SLEEP            (1 << 0 | 1 << 1 | 1 << 2)
#define STATUS1_POISON           (1 << 3)
#define STATUS1_BURN             (1 << 4)
#define STATUS1_FREEZE           (1 << 5)
#define STATUS1_PARALYSIS        (1 << 6)
#define STATUS1_TOXIC_POISON     (1 << 7)
#define STATUS1_TOXIC_COUNTER    (1 << 8 | 1 << 9 | 1 << 10 | 1 << 11)
#define STATUS1_PSN_ANY          (STATUS1_POISON | STATUS1_TOXIC_POISON)
#define STATUS1_ANY              (STATUS1_SLEEP | STATUS1_POISON | STATUS1_BURN | STATUS1_FREEZE | STATUS1_PARALYSIS | STATUS1_TOXIC_POISON)

// Volatile status ailments
// These are removed after exiting the battle or switching out
#define STATUS2_CONFUSION                (1 << 0 | 1 << 1 | 1 << 2)
#define STATUS2_FLINCHED                 (1 << 3)
#define STATUS2_UPROAR                   (1 << 4 | 1 << 5 | 1 << 6)
#define STATUS2_BIDE                     (1 << 7 | 1 << 8)
#define STATUS2_LOCK_CONFUSE             (1 << 9 | 1 << 10)
#define STATUS2_MULTIPLETURNS            (1 << 11)
#define STATUS2_WRAPPED                  (1 << 12)
#define STATUS2_INFATUATION              (1 << 13 | 1 << 14 | 1 << 15 | 1 << 16) // 4 bits, one for every battler
#define STATUS2_INFATUATED_WITH(battler) (gBitTable[battler] << 13)
#define STATUS2_FOCUS_ENERGY             (1 << 18)
#define STATUS2_TRANSFORMED              (1 << 19)
#define STATUS2_RECHARGE                 (1 << 20)
#define STATUS2_RAGE                     (1 << 21)
#define STATUS2_SUBSTITUTE               (1 << 22)
#define STATUS2_DESTINY_BOND             (1 << 23)
#define STATUS2_ESCAPE_PREVENTION        (1 << 24)
#define STATUS2_NIGHTMARE                (1 << 25)
#define STATUS2_CURSED                   (1 << 26)
#define STATUS2_FORESIGHT                (1 << 27)
#define STATUS2_DEFENSE_CURL             (1 << 28)
#define STATUS2_TORMENT                  (1 << 29)

// Seems like per-battler statuses. Not quite sure how to categorize these
#define STATUS3_LEECHSEED_BATTLER       (1 << 0 | 1 << 1)
#define STATUS3_LEECHSEED               (1 << 2)
#define STATUS3_ALWAYS_HITS             (1 << 3 | 1 << 4)
#define STATUS3_PERISH_SONG             (1 << 5)
#define STATUS3_ON_AIR                  (1 << 6)
#define STATUS3_UNDERGROUND             (1 << 7)
#define STATUS3_MINIMIZED               (1 << 8)
#define STATUS3_ROOTED                  (1 << 9)
#define STATUS3_CHARGED_UP              (1 << 10)
#define STATUS3_YAWN                    (1 << 11 | 1 << 12)
#define STATUS3_IMPRISONED_OTHERS       (1 << 13)
#define STATUS3_GRUDGE                  (1 << 14)
#define STATUS3_CANT_SCORE_A_CRIT       (1 << 15)
#define STATUS3_MUDSPORT                (1 << 16)
#define STATUS3_WATERSPORT              (1 << 17)
#define STATUS3_UNDERWATER              (1 << 18)
#define STATUS3_INTIMIDATE_POKES        (1 << 19)
#define STATUS3_TRACE                   (1 << 20)
#define STATUS3_POWER_TRICK             (1 << 21)
#define STATUS3_SEMI_INVULNERABLE       (STATUS3_UNDERGROUND | STATUS3_ON_AIR | STATUS3_UNDERWATER)

// Not really sure what a "hitmarker" is.
#define HITMARKER_SKIP_DMG_TRACK        (1 << 0)
#define HITMARKER_DESTINYBOND           (1 << 1)
#define HITMARKER_NO_ANIMATIONS         (1 << 2)
#define HITMARKER_IGNORE_SUBSTITUTE     (1 << 3)
#define HITMARKER_NO_ATTACKSTRING       (1 << 4)
#define HITMARKER_ATTACKSTRING_PRINTED  (1 << 5)
#define HITMARKER_NO_PPDEDUCT           (1 << 6)
#define HITMARKER_SWAP_ATTACKER_TARGET  (1 << 7)
#define HITMARKER_IGNORE_SAFEGUARD      (1 << 8)
#define HITMARKER_SYNCHRONISE_EFFECT    (1 << 9)
#define HITMARKER_RUN                   (1 << 10)
#define HITMARKER_UNABLE_TO_USE_MOVE    (1 << 11)
#define HITMARKER_PASSIVE_DAMAGE        (1 << 12)
#define HITMARKER_PLAYER_FAINTED        (1 << 13)
#define HITMARKER_ALLOW_NO_PP           (1 << 14)
#define HITMARKER_GRUDGE                (1 << 15)
#define HITMARKER_OBEYS                 (1 << 16)
#define HITMARKER_CHARGING              (1 << 17)
#define HITMARKER_FAINTED(battler)      (gBitTable[battler] << 18) // four bits

// Per-side statuses that affect an entire party
#define SIDE_STATUS_REFLECT          (1 << 0)
#define SIDE_STATUS_LIGHTSCREEN      (1 << 1)
#define SIDE_STATUS_SPIKES           (1 << 2)
#define SIDE_STATUS_SPIKES_DAMAGED   (1 << 3)
#define SIDE_STATUS_SAFEGUARD        (1 << 4)
#define SIDE_STATUS_FUTUREATTACK     (1 << 5)
#define SIDE_STATUS_MIST             (1 << 6)

// Flags describing move's result
#define MOVE_RESULT_MISSED             (1 << 0)
#define MOVE_RESULT_SUPER_EFFECTIVE    (1 << 1)
#define MOVE_RESULT_NOT_VERY_EFFECTIVE (1 << 2)
#define MOVE_RESULT_DOESNT_AFFECT_FOE  (1 << 3)
#define MOVE_RESULT_ONE_HIT_KO         (1 << 4)
#define MOVE_RESULT_FAILED             (1 << 5)
#define MOVE_RESULT_FOE_ENDURED        (1 << 6)
#define MOVE_RESULT_FOE_HUNG_ON        (1 << 7)
#define MOVE_RESULT_FOE_STURDIED       (1 << 8)
#define MOVE_RESULT_NO_EFFECT          (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE | MOVE_RESULT_FAILED)

// Battle Weather flags
#define WEATHER_RAIN_TEMPORARY      (1 << 0)
#define WEATHER_RAIN_DOWNPOUR       (1 << 1)  // unused
#define WEATHER_RAIN_PERMANENT      (1 << 2)
#define WEATHER_RAIN_PRIMAL         (1 << 3)
#define WEATHER_RAIN_ANY            (WEATHER_RAIN_TEMPORARY | WEATHER_RAIN_DOWNPOUR | WEATHER_RAIN_PERMANENT | WEATHER_RAIN_PRIMAL)
#define WEATHER_SANDSTORM_TEMPORARY (1 << 4)
#define WEATHER_SANDSTORM_PERMANENT (1 << 5)
#define WEATHER_SANDSTORM_ANY       (WEATHER_SANDSTORM_TEMPORARY | WEATHER_SANDSTORM_PERMANENT)
#define WEATHER_SUN_TEMPORARY       (1 << 6)
#define WEATHER_SUN_PERMANENT       (1 << 7)
#define WEATHER_SUN_PRIMAL          (1 << 8)
#define WEATHER_SUN_ANY             (WEATHER_SUN_TEMPORARY | WEATHER_SUN_PERMANENT | WEATHER_SUN_PRIMAL)
#define WEATHER_HAIL_TEMPORARY      (1 << 9)
#define WEATHER_HAIL_PERMANENT      (1 << 10)
#define WEATHER_HAIL_ANY            (WEATHER_HAIL_TEMPORARY | WEATHER_HAIL_PERMANENT)
#define WEATHER_STRONG_WINDS        (1 << 11)
#define WEATHER_FOG_TEMPORARY       (1 << 12)
#define WEATHER_FOG_PERMANENT       (1 << 13)
#define WEATHER_FOG_ANY             (WEATHER_FOG_TEMPORARY | WEATHER_FOG_PERMANENT)
#define WEATHER_ANY                 (WEATHER_RAIN_ANY | WEATHER_SANDSTORM_ANY | WEATHER_SUN_ANY | WEATHER_HAIL_ANY | WEATHER_FOG_ANY | WEATHER_STRONG_WINDS)
#define WEATHER_PRIMAL_ANY          (WEATHER_RAIN_PRIMAL | WEATHER_SUN_PRIMAL | WEATHER_STRONG_WINDS)

// Weather as enum
#define ENUM_WEATHER_RAIN         0
#define ENUM_WEATHER_SUN          1
#define ENUM_WEATHER_SANDSTORM    2
#define ENUM_WEATHER_HAIL         3
#define ENUM_WEATHER_FOG          4
#define ENUM_WEATHER_STRONG_WINDS 5
#define ENUM_WEATHER_RAIN_PRIMAL  6
#define ENUM_WEATHER_SUN_PRIMAL   7

// Move Effects
#define MOVE_EFFECT_NOTHING_0           0x0
#define MOVE_EFFECT_SLEEP               0x1
#define MOVE_EFFECT_POISON              0x2
#define MOVE_EFFECT_BURN                0x3
#define MOVE_EFFECT_FREEZE              0x4
#define MOVE_EFFECT_PARALYSIS           0x5
#define MOVE_EFFECT_TOXIC               0x6
#define MOVE_EFFECT_CONFUSION           0x7
#define MOVE_EFFECT_FLINCH              0x8
#define MOVE_EFFECT_TRI_ATTACK          0x9
#define MOVE_EFFECT_UPROAR              0xA
#define MOVE_EFFECT_PAYDAY              0xB
#define MOVE_EFFECT_CHARGING            0xC
#define MOVE_EFFECT_WRAP                0xD
#define MOVE_EFFECT_RECOIL_25           0xE
#define MOVE_EFFECT_ATK_PLUS_1          0xF
#define MOVE_EFFECT_DEF_PLUS_1          0x10
#define MOVE_EFFECT_SPD_PLUS_1          0x11
#define MOVE_EFFECT_SP_ATK_PLUS_1       0x12
#define MOVE_EFFECT_SP_DEF_PLUS_1       0x13
#define MOVE_EFFECT_ACC_PLUS_1          0x14
#define MOVE_EFFECT_EVS_PLUS_1          0x15
#define MOVE_EFFECT_ATK_MINUS_1         0x16
#define MOVE_EFFECT_DEF_MINUS_1         0x17
#define MOVE_EFFECT_SPD_MINUS_1         0x18
#define MOVE_EFFECT_SP_ATK_MINUS_1      0x19
#define MOVE_EFFECT_SP_DEF_MINUS_1      0x1A
#define MOVE_EFFECT_ACC_MINUS_1         0x1B
#define MOVE_EFFECT_EVS_MINUS_1         0x1C
#define MOVE_EFFECT_RECHARGE            0x1D
#define MOVE_EFFECT_RAGE                0x1E
#define MOVE_EFFECT_STEAL_ITEM          0x1F
#define MOVE_EFFECT_PREVENT_ESCAPE      0x20
#define MOVE_EFFECT_NIGHTMARE           0x21
#define MOVE_EFFECT_ALL_STATS_UP        0x22
#define MOVE_EFFECT_RAPIDSPIN           0x23
#define MOVE_EFFECT_REMOVE_PARALYSIS    0x24
#define MOVE_EFFECT_ATK_DEF_DOWN        0x25
#define MOVE_EFFECT_RECOIL_33           0x26
#define MOVE_EFFECT_ATK_PLUS_2          0x27
#define MOVE_EFFECT_DEF_PLUS_2          0x28
#define MOVE_EFFECT_SPD_PLUS_2          0x29
#define MOVE_EFFECT_SP_ATK_PLUS_2       0x2A
#define MOVE_EFFECT_SP_DEF_PLUS_2       0x2B
#define MOVE_EFFECT_ACC_PLUS_2          0x2C
#define MOVE_EFFECT_EVS_PLUS_2          0x2D
#define MOVE_EFFECT_ATK_MINUS_2         0x2E
#define MOVE_EFFECT_DEF_MINUS_2         0x2F
#define MOVE_EFFECT_SPD_MINUS_2         0x30
#define MOVE_EFFECT_SP_ATK_MINUS_2      0x31
#define MOVE_EFFECT_SP_DEF_MINUS_2      0x32
#define MOVE_EFFECT_ACC_MINUS_2         0x33
#define MOVE_EFFECT_EVS_MINUS_2         0x34
#define MOVE_EFFECT_THRASH              0x35
#define MOVE_EFFECT_KNOCK_OFF           0x36
#define MOVE_EFFECT_FEINT               0x37
#define MOVE_EFFECT_NOTHING_38          0x38
#define MOVE_EFFECT_NOTHING_39          0x39
#define MOVE_EFFECT_NOTHING_3A          0x3A
#define MOVE_EFFECT_SP_ATK_TWO_DOWN     0x3B
#define MOVE_EFFECT_NOTHING_3C          0x3C
#define MOVE_EFFECT_NOTHING_3D          0x3D
#define MOVE_EFFECT_NOTHING_3E          0x3E
#define MOVE_EFFECT_NOTHING_3F          0x3F

#define NUM_MOVE_EFFECTS MOVE_EFFECT_NOTHING_3F + 1

#define MOVE_EFFECT_AFFECTS_USER        0x40
#define MOVE_EFFECT_CERTAIN             0x80

// Battle terrain defines for gBattleTerrain.
#define BATTLE_TERRAIN_GRASS        0
#define BATTLE_TERRAIN_LONG_GRASS   1
#define BATTLE_TERRAIN_SAND         2
#define BATTLE_TERRAIN_UNDERWATER   3
#define BATTLE_TERRAIN_WATER        4
#define BATTLE_TERRAIN_POND         5
#define BATTLE_TERRAIN_MOUNTAIN     6
#define BATTLE_TERRAIN_CAVE         7
#define BATTLE_TERRAIN_BUILDING     8
#define BATTLE_TERRAIN_PLAIN        9
#define BATTLE_TERRAIN_LINK        10
#define BATTLE_TERRAIN_GYM         11
#define BATTLE_TERRAIN_LEADER      12
#define BATTLE_TERRAIN_INDOOR_2    13
#define BATTLE_TERRAIN_INDOOR_1    14
#define BATTLE_TERRAIN_LORELEI     15
#define BATTLE_TERRAIN_BRUNO       16
#define BATTLE_TERRAIN_AGATHA      17
#define BATTLE_TERRAIN_LANCE       18
#define BATTLE_TERRAIN_CHAMPION    19

// Return value for IsRunningFromBattleImpossible. 
#define BATTLE_RUN_SUCCESS        0
#define BATTLE_RUN_FORBIDDEN      1
#define BATTLE_RUN_FAILURE        2

// Macros for loadabilitypopupcommand
#define REMOVE_POP_UP 0
#define LOAD_ABILITY_FROM_BUFFER 0
#define LOAD_ABILITY_NORMAL 1
#define LOAD_ABILITY_FROM_SECOND_BANK 2

#endif // GUARD_CONSTANTS_BATTLE_H

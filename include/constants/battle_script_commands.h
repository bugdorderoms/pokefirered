#ifndef GUARD_CONSTANTS_BATTLE_SCRIPT_COMMANDS_H
#define GUARD_CONSTANTS_BATTLE_SCRIPT_COMMANDS_H

// Flags for the CanBe* statused functions
#define STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD      (1 << 0) // Also Flower Veil
#define STATUS_CHANGE_FLAG_CHECK_UPROAR          (1 << 1)
#define STATUS_CHANGE_FLAG_IGNORE_GENERAL_STATUS (1 << 2)
#define STATUS_CHANGE_FLAG_IGNORE_SUBSTITUTE     (1 << 3)
#define STATUS_CHANGE_FLAG_ALL                   (STATUS_CHANGE_FLAG_IGNORE_SAFEGUARD | STATUS_CHANGE_FLAG_CHECK_UPROAR             \
                                                 | STATUS_CHANGE_FLAG_IGNORE_GENERAL_STATUS | STATUS_CHANGE_FLAG_IGNORE_SUBSTITUTE)
#define STATUS_CHANGE_FLAG_NO_SYNCHRONISE        (1 << 4)

#define STAT_CHANGE_FLAG_SELF_INFLICT    (1 << 0) // The stat change is self inflicted. So, can't be bounced back by Mirror Armor or blocked by any effect
#define STAT_CHANGE_FLAG_IGNORE_PROTECT  (1 << 1) // Ignore Protect effects
#define STAT_CHANGE_FLAG_UPDATE_RESULT   (1 << 2) // If stat change failed to apply set gMoveResultFlags as MOVE_RESULT_MISSED
#define STAT_CHANGE_FLAG_NO_MIRROR_ARMOR (1 << 3) // Ignore Mirror Armor

// Battle Scripting addresses
#define sPAINSPLIT_HP gBattleScripting + 0x0
#define sBIDE_DMG gBattleScripting + 0x4
#define sSAVED_DMG gBattleScripting + 0x8
#define sMULTIHIT_STRING gBattleScripting + 0x0C
#define sPREFAINT_MOVEEND_STATE gBattleScripting + 0x12
#define sBYPASS_ABILITY_POP_UP gBattleScripting + 0x13
#define sB_ANIM_ARG1 gBattleScripting + 0x14
#define sB_ANIM_ARG2 gBattleScripting + 0x15
#define sMOVEEND_STATE gBattleScripting + 0x17
#define sBATTLER gBattleScripting + 0x19
#define sB_ANIM_TURN gBattleScripting + 0x1A
#define sB_ANIM_TARGETS_HIT gBattleScripting + 0x1B
#define sFIXED_ABILITY_POPUP gBattleScripting + 0x1C
#define sILLUSION_NICK_HACK gBattleScripting + 0x1D
#define sGIVEEXP_STATE gBattleScripting + 0x1E
#define sBATTLE_STYLE gBattleScripting + 0x1F
#define sLVLBOX_STATE gBattleScripting + 0x20
#define sLEARNMOVE_STATE gBattleScripting + 0x21
#define sSWITCHIN_EFFECTS_STATE gBattleScripting + 0x25
#define sEXP_CATCH gBattleScripting + 0x26

// array entries for battle communication
#define MULTIUSE_STATE                      0
#define CURSOR_POSITION                     1
#define TASK_ID                             1 // task Id and cursor position share the same field
#define SPRITES_INIT_STATE1                 1 // shares the Id as well
#define SPRITES_INIT_STATE2                 2
#define FAINT_EFFECTS_STATE                 3
#define ACTIONS_CONFIRMED_COUNT             4
#define MULTISTRING_CHOOSER                 5
#define MISS_TYPE                           6
#define MSG_DISPLAY                         7
#define BATTLE_COMMUNICATION_ENTRIES_COUNT  8

// BattleCommunication addresses
#define sMULTIUSE_STATE gBattleCommunication + MULTIUSE_STATE
#define sCURSOR_POSITION gBattleCommunication + CURSOR_POSITION
#define sFAINT_EFFECTS_STATE gBattleCommunication + FAINT_EFFECTS_STATE
#define sMULTISTRING_CHOOSER gBattleCommunication + MULTISTRING_CHOOSER
#define sMISS_TYPE gBattleCommunication + MISS_TYPE

// Battle Script defines for getting the wanted battler
#define BS_TARGET                   0
#define BS_ATTACKER                 1
#define BS_EFFECT_BATTLER           2
#define BS_FAINTED                  3
#define BS_ATTACKER_WITH_PARTNER    4 // for atk98_updatestatusicon
#define BS_UNKNOWN_5                5 // for openpartyscreen
#define BS_UNKNOWN_6                6 // for openpartyscreen
#define BS_BATTLER_0                7
#define BS_ATTACKER_SIDE            8 // unused
#define BS_NOT_ATTACKER_SIDE        9 // unused
#define BS_SCRIPTING                10
#define BS_PLAYER1                  11
#define BS_OPPONENT1                12
#define BS_PLAYER2                  13 // for atk98_updatestatusicon
#define BS_OPPONENT2                14
#define BS_SCRIPTING_PARTNER        15

// Compare operands
#define CMP_EQUAL               0x0
#define CMP_NOT_EQUAL           0x1
#define CMP_GREATER_THAN        0x2
#define CMP_LESS_THAN           0x3
#define CMP_COMMON_BITS         0x4
#define CMP_NO_COMMON_BITS      0x5

// Args for UpdateHealthboxAttribute
#define HEALTHBOX_ALL               0
#define HEALTHBOX_CURRENT_HP        1
#define HEALTHBOX_MAX_HP            2
#define HEALTHBOX_LEVEL             3
#define HEALTHBOX_NICK              4
#define HEALTHBOX_HEALTH_BAR        5
#define HEALTHBOX_EXP_BAR           6
#define HEALTHBOX_STATUS_ICON       7
#define HEALTHBOX_SAFARI_ALL_TEXT   8
#define HEALTHBOX_SAFARI_BALLS_TEXT 9

// status ids
#define ID_STATUS1 0
#define ID_STATUS2 1
#define ID_STATUS3 2

// Parental Bond counter states
#define PARENTAL_BOND_1ST_HIT 2
#define PARENTAL_BOND_2ND_HIT 1
#define PARENTAL_BOND_OFF     0

// moveend states
#define MOVEEND_ALL           0 // loop through all cases
#define MOVEEND_CASE          1 // do only the specified case
#define MOVEEND_TO            2 // loop from first to specified cases
#define MOVEEND_FUTURE_ATTACK 3 // only cases that occours on a future attack hit

// atk48, prefaintmoveend cases
#define ATK48_SET_UP                 0
#define ATK48_ATTACKER_ABILITIES     1
#define ATK48_PROTECTION_EFFECTS     2
#define ATK48_RAGE                   3
#define ATK48_SYNCHRONIZE_TARGET     4
#define ATK48_BEAK_BLAST             5
#define ATK48_SYNCHRONIZE_ATTACKER   6
#define ATK48_TARGET_ABILITIES       7
#define ATK48_SYNCHRONIZE_ATTACKER_2 8
#define ATK48_TARGET_CONTACT_ITEMS   9
#define ATK48_COUNT                  10

// Atk49, moveend cases
#define ATK49_SKY_DROP                  0
#define ATK49_ATTACKER_INVISIBLE        1
#define ATK49_ATTACKER_VISIBLE          2
#define ATK49_TARGET_VISIBLE            3
#define ATK49_CHOICE_MOVE               4
#define ATK49_TARGET_ENDTURN_ITEMS      5
#define ATK49_KINGS_ROCK                6
#define ATK49_ATTACKER_ENDTURN_ITEMS    7
#define ATK49_STATUS_IMMUNITY_ABILITIES 8
#define ATK49_UPDATE_LAST_MOVES         9
#define ATK49_MIRROR_MOVE               10
#define ATK49_MULTIHIT_MOVE             11
#define ATK49_DEFROST                   12
#define ATK49_SECOND_MOVE_EFFECT        13
#define ATK49_MAGICIAN                  14
#define ATK49_ATTACKER_ENDTURN_ITEMS_2  15
#define ATK49_NEXT_TARGET               16
#define ATK49_RECOIL                    17
#define ATK49_ATTACKER_ENDTURN_ITEMS_3  18
#define ATK49_EJECT_BUTTON              19
#define ATK49_RED_CARD                  20
#define ATK49_EJECT_PACK                21
#define ATK49_SHELL_BELL_LIFE_ORB       22
#define ATK49_EMERGENCY_EXIT            23
#define ATK49_PICKPOCKET                24
#define ATK49_SUBSTITUTE                25
#define ATK49_EVOLUTIONS_TRACKER        26
#define ATK49_CLEAR_BITS                27
#define ATK49_COUNT                     28

// Atk4F, a flag used for the jumpifcantswitch command
#define ATK4F_DONT_CHECK_STATUSES   0x80

// Atk50, a flag used for the openpartyscreen command
#define OPEN_PARTY_ALLOW_CANCEL     0x80

// atk60, a flag used for the handleabilitypopup command
#define ATK60_UPDATE_POP_UP 0x40
#define ATK60_REMOVE_POP_UP 0x80

// Atk66
// Used when there's multiple buffs
#define BIT_ATK     (1 << 0)
#define BIT_DEF     (1 << 1)
#define BIT_SPEED   (1 << 2)
#define BIT_SPATK   (1 << 3)
#define BIT_SPDEF   (1 << 4)
#define BIT_ACC     (1 << 5)
#define BIT_EVASION (1 << 6)

#define ATK66_STAT_NEGATIVE      (1 << 0) // Used when there's multiples negative stat buffs
#define ATK66_IGNORE_ANIM_PLAYED (1 << 1) // Play stat anim even if disabled, for Defiant
#define ATK66_SET_ANIM_PLAYED    (1 << 2) // Used when there's multiples buffs, play the anim only on the first buff
#define ATK66_CLEAR_ANIM_PLAYED  (1 << 3) // Reset anim played, for the next stat buff that can occours

// Atk6A, flags
#define ATK6A_FLAG_CHECK_CHEEK_POUCH    (1 << 0) // Try activate Cheek pouch
#define ATK6A_FLAG_DONT_CHECK_SYMBIOSIS (1 << 1) // Don't check Symbiosis

// Atk6B, trainerslide cases
#define ATK6B_TRAINER_SLIDE_CASE_SAVE_SPRITES    0
#define ATK6B_TRAINER_SLIDE_CASE_SLIDE_IN        1
#define ATK6B_TRAINER_SLIDE_CASE_PRINT_STRING    2
#define ATK6B_TRAINER_SLIDE_CASE_SLIDE_OUT       3
#define ATK6B_TRAINER_SLIDE_CASE_RESTORE_SPRITES 4

// Atk76, various cases
#define VARIOUS_JUMP_IF_PARENTAL_BOND_COUNTER       0
#define VARIOUS_TRY_COPY_ABILITY                    1
#define VARIOUS_TRY_RATTLED_ON_INTIMIDATE           2
#define VARIOUS_GET_BATTLER_SIDE                    3
#define VARIOUS_TRY_LAST_MON_TRAINER_SLIDE          4
#define VARIOUS_RESET_INTIMIDATE_TRACE_BITS         5
#define VARIOUS_JUMP_IF_DEFIANT_ACTIVATE            6
#define VARIOUS_RESET_PLAYER_FAINTED_FLAG           7
#define VARIOUS_TRY_SET_DESTINY_BOND                8
#define VARIOUS_JUMP_IF_NOT_FIRST_TURN              9
#define VARIOUS_TRY_CONVERSION_TYPE_CHANGE          10
#define VARIOUS_CHECK_POKEFLUTE                     11
#define VARIOUS_WAIT_FANFARE                        12
#define VARIOUS_TRY_ABILITY_SUPPRESSION             13
#define VARIOUS_JUMP_IF_TARGET_ALLY                 14
#define VARIOUS_TRY_ACTIVATE_SAP_SIPPER             15
#define VARIOUS_TRY_ACTIVATE_WIND_ABILITIES         16
#define VARIOUS_UPDATE_HEALTHBOX_ATTRIBUTE          17
#define VARIOUS_JUMP_IF_WEATHER_AFFECTED            18
#define VARIOUS_TRY_REMOVE_ILLUSION                 19
#define VARIOUS_PLAY_MOVE_ANIMATION                 20
#define VARIOUS_JUMP_IF_BATTLE_END                  21
#define VARIOUS_TRY_REMOVE_PRIMAL_WEATHER           22
#define VARIOUS_JUMP_IF_NOT_ALIVE                   23
#define VARIOUS_TRY_FRISK_TARGET                    24
#define VARIOUS_SWITCHIN_ABILITIES                  25
#define VARIOUS_TRY_END_NEUTRALIZING_GAS            26
#define VARIOUS_SET_TELEPORT_OUTCOME                27
#define VARIOUS_TRY_FAINT_ON_SPIKES_DAMAGE          28
#define VARIOUS_JUMP_IF_CONFUSED_AND_STAT_MAXED     29
#define VARIOUS_JUMP_IF_EMERGENCY_EXITED            30
#define VARIOUS_TRY_NEUTRALIZING_GAS_SUPPRESSION    31
#define VARIOUS_JUMP_IF_BATTLER_REVIVED             32
#define VARIOUS_ACTIVATE_WEATHER_ABILITIES          33
#define VARIOUS_TRY_STATUS_TRANSFER                 34
#define VARIOUS_TRY_BRING_DOWN_IN_AIR               35
#define VARIOUS_TRY_SET_GRAVITY                     36
#define VARIOUS_TRY_SWAP_ABILITIES                  37
#define VARIOUS_JUMP_IF_NO_DAMAGE                   38

// Atk80, dmg manipulation
#define ATK80_DMG_CHANGE_SIGN       0
#define ATK80_DMG_HALF_USER_HP      1
#define ATK80_DMG_DRAINED           2
#define ATK80_DMG_1_8_TARGET_MAX_HP 3
#define ATK48_DMG_HALF_TARGET_HP    4
#define ATK48_DMG_ATK_LEVEL         5
#define ATK48_DMG_BIG_ROOT          6
#define ATK80_DMG_HEALTH_DIFFERENCE 7

#endif // GUARD_CONSTANTS_BATTLE_SCRIPT_COMMANDS_H

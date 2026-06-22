#ifndef GUARD_BATTLE_MESSAGE_H
#define GUARD_BATTLE_MESSAGE_H

#include "global.h"
#include "constants/battle.h"

#define TEXT_BUFF_ARRAY_COUNT   50

struct BattleMsgData
{
    u16 currentMove;
    u16 chosenMove;
    u16 lastItem;
    u8 scrActive;
    u8 hpScale;
    u16 abilities[MAX_BATTLERS_COUNT];
    u8 textBuffs[3][TEXT_BUFF_ARRAY_COUNT];
};

// for 0xFD

#define B_TXT_BUFF1                       0x00
#define B_TXT_BUFF2                       0x01
#define B_TXT_COPY_VAR_1                  0x02
#define B_TXT_COPY_VAR_2                  0x03
#define B_TXT_COPY_VAR_3                  0x04
#define B_TXT_PLAYER_MON1_NAME            0x05
#define B_TXT_OPPONENT_MON1_NAME          0x06
#define B_TXT_PLAYER_MON2_NAME            0x07
#define B_TXT_OPPONENT_MON2_NAME          0x08
#define B_TXT_LINK_PLAYER_MON1_NAME       0x09
#define B_TXT_LINK_OPPONENT_MON1_NAME     0x0A
#define B_TXT_LINK_PLAYER_MON2_NAME       0x0B
#define B_TXT_LINK_OPPONENT_MON2_NAME     0x0C
#define B_TXT_ATK_NAME_WITH_PREFIX        0x0D
#define B_TXT_DEF_NAME_WITH_PREFIX        0x0E
#define B_TXT_EFF_NAME_WITH_PREFIX        0x0F // EFF = short for gEffectBattler
#define B_TXT_SCR_ACTIVE_NAME_WITH_PREFIX 0x10
#define B_TXT_CURRENT_MOVE                0x11
#define B_TXT_CHOSEN_MOVE                 0x12
#define B_TXT_LAST_ITEM                   0x13
#define B_TXT_DEF_NAME                    0x14
#define B_TXT_ATK_ABILITY                 0x15
#define B_TXT_DEF_ABILITY                 0x16
#define B_TXT_SCR_ACTIVE_ABILITY          0x17
#define B_TXT_EFF_ABILITY                 0x18
#define B_TXT_TRAINER1_CLASS              0x19
#define B_TXT_TRAINER1_NAME               0x1A
#define B_TXT_LINK_PLAYER_NAME            0x1B
#define B_TXT_LINK_PARTNER_NAME           0x1C
#define B_TXT_LINK_OPPONENT1_NAME         0x1D
#define B_TXT_LINK_OPPONENT2_NAME         0x1E
#define B_TXT_LINK_SCR_TRAINER_NAME       0x1F
#define B_TXT_PLAYER_NAME                 0x20
#define B_TXT_TRAINER1_LOSE_TEXT          0x21
#define B_TXT_TRAINER1_WIN_TEXT           0x22
#define B_TXT_ITEM_USE_SPECIES_NAME       0x23
#define B_TXT_ATK_TEAM_PREFIX             0x24
#define B_TXT_DEF_TEAM_PREFIX             0x25
#define B_TXT_CURRENT_TRAINER_NAME        0x26
#define B_TXT_BUFF3                       0x27
#define B_TXT_PARTNER_CLASS               0x28
#define B_TXT_PARTNER_NAME                0x29

// for B_TXT_BUFF1, B_TXT_BUFF2 and B_TXT_BUFF3

#define B_BUFF_STRING                   0
#define B_BUFF_NUMBER                   1
#define B_BUFF_MOVE                     2
#define B_BUFF_TYPE                     3
#define B_BUFF_MON_NICK_WITH_PREFIX     4
#define B_BUFF_STAT                     5
#define B_BUFF_SPECIES                  6
#define B_BUFF_MON_NICK                 7
#define B_BUFF_NEGATIVE_FLAVOR          8
#define B_BUFF_ABILITY                  9
#define B_BUFF_ITEM                     10
#define B_BUFF_MON_NICK_NO_ILLUSION     11
#define B_BUFF_MON_TEAM_PREFIX          12

#define B_BUFF_PLACEHOLDER_BEGIN        0xFD
#define B_BUFF_EOS                      0xFF

// String buffers
void BufferStringBattle(u32 battlerId, u32 stringId);
void BattleStringExpandPlaceholdersToDisplayedString(const u8* src);
void SetPpNumbersPaletteInMoveSelection(u32 currentPp, u32 maxPp);
void BattlePutTextOnWindow(const u8* text, u8 windowId_flags);
bool32 BattleStringShouldBeColored(u32);

extern u8 gDisplayedStringBattle[300];
extern u8 gBattleTextBuff1[TEXT_BUFF_ARRAY_COUNT];
extern u8 gBattleTextBuff2[TEXT_BUFF_ARRAY_COUNT];
extern u8 gBattleTextBuff3[TEXT_BUFF_ARRAY_COUNT];

// Prepare buffers
void PrepareFlavorBuffer(u8 *textVar, u32 flavorId);
void PrepareStatBuffer(u8 *textVar, u32 statId);
void PrepareAbilityBuffer(u8 *textVar, u32 abilityId);
void PrepareTypeBuffer(u8 *textVar, u32 typeId);
void PrepareByteNumberBuffer(u8 *textVar, u32 maxDigits, u32 number);
void PrepareHWordNumberBuffer(u8 *textVar, u32 maxDigits, u32 number);
void PrepareWordNumberBuffer(u8 *textVar, u32 maxDigits, u32 number);
void PrepareStringBuffer(u8 *textVar, u32 stringId);
void PrepareMoveBuffer(u8 *textVar, u32 move);
void PrepareItemBuffer(u8 *textVar, u32 item);
void PrepareSpeciesBuffer(u8 *textVar, u32 species);
void PrepareMonNickWithPrefixBuffer(u8 *textVar, u32 battlerId, u32 partyId);
void PrepareMonNickBuffer(u8 *textVar, u32 battlerId, u32 partyId);
void PrepareMonNickNoIllusionBuffer(u8 *textVar, u32 battlerId, u32 partyId);
void PrepareMonTeamPrefixBuffer(u8 *textVar, u32 battlerId);

#define PrepareBattlerNickWithPrefixBuffer(textVar, battlerId) PrepareMonNickWithPrefixBuffer(textVar, battlerId, gBattlerPartyIndexes[battlerId])

extern const u8* const gBattleStringsTable[];

extern const u16 gMissStringIds[];

extern const u8 gText_Win[];
extern const u8 gText_Loss[];
extern const u8 gText_Draw[];

#endif // GUARD_BATTLE_MESSAGE_H

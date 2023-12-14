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
    u16 lastAbility;
	u16 abilities[MAX_BATTLERS_COUNT];
    u8 scrActive;
    u8 hpScale;
    u8 moveType;
    u8 textBuffs[3][TEXT_BUFF_ARRAY_COUNT];
};

// for 0xFD

#define B_TXT_BUFF1                       0x0
#define B_TXT_BUFF2                       0x1
#define B_TXT_BUFF3                       0x2
#define B_TXT_PLAYER_MON1_NAME            0x3
#define B_TXT_OPPONENT_MON1_NAME          0x4
#define B_TXT_PLAYER_MON2_NAME            0x5
#define B_TXT_OPPONENT_MON2_NAME          0x6
#define B_TXT_LINK_PLAYER_MON1_NAME       0x7
#define B_TXT_LINK_OPPONENT_MON1_NAME     0x8
#define B_TXT_LINK_PLAYER_MON2_NAME       0x9
#define B_TXT_LINK_OPPONENT_MON2_NAME     0xA
#define B_TXT_ATK_NAME_WITH_PREFIX        0xB
#define B_TXT_DEF_NAME_WITH_PREFIX        0xC
#define B_TXT_EFF_NAME_WITH_PREFIX        0xD // EFF = short for gEffectBattler
#define B_TXT_SCR_ACTIVE_NAME_WITH_PREFIX 0xE
#define B_TXT_CURRENT_MOVE                0x0F
#define B_TXT_CHOSEN_MOVE                 0x10
#define B_TXT_LAST_ITEM                   0x11
#define B_TXT_LAST_ABILITY                0x12
#define B_TXT_ATK_ABILITY                 0x13
#define B_TXT_DEF_ABILITY                 0x14
#define B_TXT_SCR_ACTIVE_ABILITY          0x15
#define B_TXT_EFF_ABILITY                 0x16
#define B_TXT_TRAINER1_CLASS              0x17
#define B_TXT_TRAINER1_NAME               0x18
#define B_TXT_LINK_PLAYER_NAME            0x19
#define B_TXT_LINK_PARTNER_NAME           0x1A
#define B_TXT_LINK_OPPONENT1_NAME         0x1B
#define B_TXT_LINK_OPPONENT2_NAME         0x1C
#define B_TXT_LINK_SCR_TRAINER_NAME       0x1D
#define B_TXT_PLAYER_NAME                 0x1E
#define B_TXT_TRAINER1_LOSE_TEXT          0x1F
#define B_TXT_TRAINER1_WIN_TEXT           0x20
#define B_TXT_ITEM_USE_SPECIES_NAME       0x21
#define B_TXT_ATK_TEAM_PREFIX             0x22
#define B_TXT_DEF_TEAM_PREFIX             0x23
#define B_TXT_DEF_NAME                    0x24

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

// Prepare buffers
void PrepareFlavorBuffer(u8 *textVar, u8 flavorId);
void PrepareStatBuffer(u8 *textVar, u8 statId);
void PrepareAbilityBuffer(u8 *textVar, u16 abilityId);
void PrepareTypeBuffer(u8 *textVar, u8 typeId);
void PrepareByteNumberBuffer(u8 *textVar, u8 maxDigits, u8 number);
void PrepareHWordNumberBuffer(u8 *textVar, u8 maxDigits, u16 number);
void PrepareWordNumberBuffer(u8 *textVar, u8 maxDigits, u32 number);
void PrepareStringBuffer(u8 *textVar, u16 stringId);
void PrepareMoveBuffer(u8 *textVar, u16 move);
void PrepareItemBuffer(u8 *textVar, u16 item);
void PrepareSpeciesBuffer(u8 *textVar, u16 species);
void PrepareMonNickWithPrefixBuffer(u8 *textVar, u8 battlerId, u8 partyId);
void PrepareMonNickBuffer(u8 *textVar, u8 battlerId, u8 partyId);
void PrepareMonNickNoIllusionBuffer(u8 *textVar, u8 battlerId);
void PrepareMonTeamPrefixBuffer(u8 *textVar, u8 battlerId);

// String buffers
void BufferStringBattle(u8 battlerId, u16 stringId);
void BattleStringExpandPlaceholdersToDisplayedString(const u8* src);
void BattleStringExpandPlaceholders(const u8* src, u8* dst);
void SetPpNumbersPaletteInMoveSelection(u8 battlerId);
void BattlePutTextOnWindow(const u8* text, u8 windowId_flags);
bool8 BattleStringShouldBeColored(u16);

extern u8 gDisplayedStringBattle[300];
extern u8 gBattleTextBuff1[TEXT_BUFF_ARRAY_COUNT];
extern u8 gBattleTextBuff2[TEXT_BUFF_ARRAY_COUNT];
extern u8 gBattleTextBuff3[TEXT_BUFF_ARRAY_COUNT];

extern const u8* const gBattleStringsTable[];
extern const u8* const gPokeblockWasTooXStringTable[];

extern const u16 gMissStringIds[];

extern const u8 gText_Sleep[];
extern const u8 gText_Poison[];
extern const u8 gText_Burn[];
extern const u8 gText_Paralysis[];
extern const u8 gText_Ice[];
extern const u8 gText_Confusion[];
extern const u8 gText_Love[];

extern const u8 gText_SafariBalls[];
extern const u8 gText_HighlightRed_Left[];

extern const u8 gText_Win[];
extern const u8 gText_Loss[];
extern const u8 gText_Draw[];

#endif // GUARD_BATTLE_MESSAGE_H

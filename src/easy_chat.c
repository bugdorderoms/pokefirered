#include "global.h"
#include "gflib.h"
#include "data.h"
#include "easy_chat.h"
#include "event_data.h"
#include "field_message_box.h"
#include "menu.h"
#include "mail.h"
#include "pokedex.h"
#include "random.h"
#include "constants/easy_chat.h"

struct Unk203A120
{
    u16 numGroups;
    u16 groups[EC_NUM_GROUPS];
    u16 alphabeticalGroups[27];
    u16 alphabeticalWordsByGroup[27][270];
    u16 allWords[270];
    u16 totalWords;
}; /*size = 0x3B78*/

static EWRAM_DATA struct Unk203A120 * sEasyChatSelectionData = NULL;

static void PopulateECGroups(void);
static void PopulateAlphabeticalGroups(void);
static u16 GetUnlockedWordsInECGroup(u16);
static u16 GetUnlockedWordsInAlphabeticalGroup(u16);
static bool32 UnlockedECMonOrMove(u16, u32);
static bool32 EC_IsDeoxys(u32 species);
static bool32 IsWordUnlocked(u16 word);

#include "data/easy_chat/easy_chat_groups.h"
#include "data/easy_chat/easy_chat_words_by_letter.h"

static const u8 sText_ThreeQuestionMarks[] = _("???");

static const u8 *const sEasyChatGroupNamePointers[] = {
    [EC_GROUP_POKEMON]       = COMPOUND_STRING("Pokémon"),
    [EC_GROUP_TRAINER]       = COMPOUND_STRING("Trainer"),
    [EC_GROUP_STATUS]        = COMPOUND_STRING("Status"),
    [EC_GROUP_BATTLE]        = COMPOUND_STRING("Battle"),
    [EC_GROUP_GREETINGS]     = COMPOUND_STRING("Greetings"),
    [EC_GROUP_PEOPLE]        = COMPOUND_STRING("People"),
    [EC_GROUP_VOICES]        = COMPOUND_STRING("Voices"),
    [EC_GROUP_SPEECH]        = COMPOUND_STRING("Speech"),
    [EC_GROUP_ENDINGS]       = COMPOUND_STRING("Endings"),
    [EC_GROUP_FEELINGS]      = COMPOUND_STRING("Feelings"),
    [EC_GROUP_CONDITIONS]    = COMPOUND_STRING("Conditions"),
    [EC_GROUP_ACTIONS]       = COMPOUND_STRING("Actions"),
    [EC_GROUP_LIFESTYLE]     = COMPOUND_STRING("Lifestyle"),
    [EC_GROUP_HOBBIES]       = COMPOUND_STRING("Hobbies"),
    [EC_GROUP_TIME]          = COMPOUND_STRING("Time"),
    [EC_GROUP_MISC]          = COMPOUND_STRING("Misc."),
    [EC_GROUP_ADJECTIVES]    = COMPOUND_STRING("Adjectives"),
    [EC_GROUP_EVENTS]        = COMPOUND_STRING("Events"),
    [EC_GROUP_MOVE_1]        = COMPOUND_STRING("Move 1"),
    [EC_GROUP_MOVE_2]        = COMPOUND_STRING("Move 2"),
    [EC_GROUP_TRENDY_SAYING] = COMPOUND_STRING("Trendy Saying"),
    [EC_GROUP_POKEMON_2]     = COMPOUND_STRING("Pokémon2"),
};

static const u16 sDefaultProfileWords[] = {
    EC_WORD_I_AM,
    EC_WORD_A,
    EC_WORD_POKEMON,
    EC_WORD_FRIEND,
};

static const u16 sDeoxysValue[] = {
    SPECIES_DEOXYS,
};

static bool32 IsECWordInvalid(u16 easyChatWord)
{
    u16 i;
    u8 groupId;
    u32 index;
    u16 numWords;
    const u16 *list;
    
    if (easyChatWord == EC_WORD_UNDEFINED)
        return FALSE;

    groupId = EC_GROUP(easyChatWord);
    index = EC_INDEX(easyChatWord);
    if (groupId >= EC_NUM_GROUPS)
        return TRUE;

    numWords = sEasyChatGroups[groupId].numWords;
    switch (groupId)
    {
    case EC_GROUP_POKEMON:
    case EC_GROUP_POKEMON_2:
    case EC_GROUP_MOVE_1:
    case EC_GROUP_MOVE_2:
        list = sEasyChatGroups[groupId].wordData.valueList;
        for (i = 0; i < numWords; i++)
        {
            if (index == list[i])
                return FALSE;
        }
        return TRUE;
    default:
        if (index >= numWords)
            return TRUE;
        else
            return FALSE;
    }
}

static const u8 *GetEasyChatWord(u32 groupId, u16 index)
{
    switch (groupId)
    {
    case EC_GROUP_POKEMON:
    case EC_GROUP_POKEMON_2:
        return gSpeciesInfo[index].name;
    case EC_GROUP_MOVE_1:
    case EC_GROUP_MOVE_2:
        return gBattleMoves[index].name;
    default:
        return sEasyChatGroups[groupId].wordData.words[index].text;
    }
}

u8 *CopyEasyChatWord(u8 *dest, u16 easyChatWord)
{
    u8 *resultStr;
    
    if (IsECWordInvalid(easyChatWord))
        resultStr = StringCopy(dest, sText_ThreeQuestionMarks);
    else if (easyChatWord != EC_WORD_UNDEFINED)
    {
        u16 index = EC_INDEX(easyChatWord);
        u8 groupId = EC_GROUP(easyChatWord);
        resultStr = StringCopy(dest, GetEasyChatWord(groupId, index));
    }
    else
    {
        *dest = EOS;
        resultStr = dest;
    }

    return resultStr;
}

u8 *ConvertEasyChatWordsToString(u8 *dest, const u16 *src, u16 columns, u16 rows)
{
    u16 i, j;
    u16 numColumns = columns - 1;

    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < numColumns; j++)
        {
            dest = CopyEasyChatWord(dest, *src);
            if (*src != EC_WORD_UNDEFINED)
            {
                *dest = CHAR_SPACE;
                dest++;
            }

            src++;
        }

        dest = CopyEasyChatWord(dest, *(src++));
        *dest = CHAR_NEWLINE;
        dest++;
    }

    dest--;
    *dest = EOS;
    return dest;
}

static u16 GetEasyChatWordStringLength(u16 easyChatWord)
{
    if (easyChatWord == EC_WORD_UNDEFINED)
        return 0;

    if (IsECWordInvalid(easyChatWord))
        return StringLength(sText_ThreeQuestionMarks);
    else
    {
        u16 index = EC_INDEX(easyChatWord);
        u8 groupId = EC_GROUP(easyChatWord);
        return StringLength(GetEasyChatWord(groupId, index));
    }
}

bool32 EC_DoesEasyChatStringFitOnLine(const u16 *easyChatWords, u8 columns, u8 rows, u16 maxLength)
{
    u32 i, j;

    for (i = 0; i < rows; i++)
    {
        u16 totalLength = columns - 1;
        for (j = 0; j < columns; j++)
            totalLength += GetEasyChatWordStringLength(*(easyChatWords++));

        if (totalLength > maxLength)
            return TRUE;
    }
    return FALSE;
}

void InitEasyChatPhrases(void)
{
    u32 i, j;

    for (i = 0; i < 4; i++)
        gSaveBlock1Ptr->easyChatProfile[i] = sDefaultProfileWords[i];

    for (i = 0; i < MAIL_COUNT; i++)
    {
        for (j = 0; j < MAIL_WORDS_COUNT; j++)
            gSaveBlock1Ptr->mail[i].words[j] = EC_WORD_UNDEFINED;
    }
    for (i = 0; i < ARRAY_COUNT(gSaveBlock1Ptr->additionalPhrases); i++)
        gSaveBlock1Ptr->additionalPhrases[i] = 0;
}

bool32 InitEasyChatSelection(void)
{
    sEasyChatSelectionData = Alloc(sizeof(*sEasyChatSelectionData));
    if (sEasyChatSelectionData == NULL)
        return FALSE;

    PopulateECGroups();
    PopulateAlphabeticalGroups();
    return TRUE;
}

void DestroyEasyChatSelectionData(void)
{
    if (sEasyChatSelectionData != NULL)
        Free(sEasyChatSelectionData);
}

static void PopulateECGroups(void)
{
    int i;

    sEasyChatSelectionData->numGroups = 0;
    
    if (GetNationalPokedexCount(FLAG_GET_SEEN))
        sEasyChatSelectionData->groups[sEasyChatSelectionData->numGroups++] = EC_GROUP_POKEMON;

    for (i = EC_GROUP_TRAINER; i <= EC_GROUP_ADJECTIVES; i++)
        sEasyChatSelectionData->groups[sEasyChatSelectionData->numGroups++] = i;

    if (FlagGet(FLAG_SYS_GAME_CLEAR))
    {
        sEasyChatSelectionData->groups[sEasyChatSelectionData->numGroups++] = EC_GROUP_EVENTS;
        sEasyChatSelectionData->groups[sEasyChatSelectionData->numGroups++] = EC_GROUP_MOVE_1;
        sEasyChatSelectionData->groups[sEasyChatSelectionData->numGroups++] = EC_GROUP_MOVE_2;
    }

    if (FlagGet(FLAG_SYS_POKEDEX_GET))
        sEasyChatSelectionData->groups[sEasyChatSelectionData->numGroups++] = EC_GROUP_POKEMON_2;
}

u32 GetNumDisplayableGroups(void)
{
    return sEasyChatSelectionData->numGroups;
}

u32 GetSelectedGroupByIndex(u32 index)
{
    if (index >= sEasyChatSelectionData->numGroups)
        return EC_NUM_GROUPS;
    else
        return sEasyChatSelectionData->groups[index];
}

const u8 *GetEasyChatWordGroupName(u32 groupId)
{
    return sEasyChatGroupNamePointers[groupId];
}

u8 *CopyEasyChatWordPadded(u8 *dest, u16 easyChatWord, u16 totalChars)
{
    u16 i;
    u8 *str = CopyEasyChatWord(dest, easyChatWord);
    for (i = str - dest; i < totalChars; i++)
    {
        *str = CHAR_SPACE;
        str++;
    }

    *str = EOS;
    return str;
}

static void PopulateAlphabeticalGroups(void)
{
    static int i;
    static int j;
    static int k;
    static int index;
    static int numWords;
    static int numToProcess;
    static const u16 *words;

    for (i = 0; i < 27; i++)
    {
        numWords = sEasyChatWordsByLetterPointers[i].numWords;
        words = sEasyChatWordsByLetterPointers[i].words;
        sEasyChatSelectionData->alphabeticalGroups[i] = 0;
        index = 0;
        for (j = 0; j < numWords; )
        {
            if (*words == EC_WORD_UNDEFINED)
            {
                words++;
                numToProcess = *words++;
                j += 2;
            }
            else
            {
                numToProcess = 1;
            }

            for (k = 0; k < numToProcess; k++)
            {
                if (IsWordUnlocked(words[k]))
                {
                    sEasyChatSelectionData->alphabeticalWordsByGroup[i][index++] = words[k];
                    sEasyChatSelectionData->alphabeticalGroups[i]++;
                    break;
                }
            }

            words += numToProcess;
            j += numToProcess;
        }
    }
}

void GetUnlockedECWords(bool32 isAlphabetical, u32 groupId)
{
    if (!isAlphabetical)
        sEasyChatSelectionData->totalWords = GetUnlockedWordsInECGroup(groupId);
    else
        sEasyChatSelectionData->totalWords = GetUnlockedWordsInAlphabeticalGroup(groupId);
}

u16 GetDisplayedWordByIndex(u16 index)
{
    if (index >= sEasyChatSelectionData->totalWords)
        return EC_WORD_UNDEFINED;
    else
        return sEasyChatSelectionData->allWords[index];
}

u16 GetNumDisplayedWords(void)
{
    return sEasyChatSelectionData->totalWords;
}

static u16 GetUnlockedWordsInECGroup(u16 groupId)
{
    u16 i;
    u16 totalWords;
    const u16 *list;
    const struct EasyChatWordInfo * wordInfo;
    u16 numWords = sEasyChatGroups[groupId].numWords;

    if (groupId == EC_GROUP_POKEMON_2 || groupId == EC_GROUP_POKEMON
     || groupId == EC_GROUP_MOVE_1  || groupId == EC_GROUP_MOVE_2)
    {
        list = sEasyChatGroups[groupId].wordData.valueList;
        for (i = 0, totalWords = 0; i < numWords; i++)
        {
            if (UnlockedECMonOrMove(list[i], groupId))
                sEasyChatSelectionData->allWords[totalWords++] = EC_WORD(groupId, list[i]);
        }

        return totalWords;
    }
    else
    {
        wordInfo = sEasyChatGroups[groupId].wordData.words;
        for (i = 0, totalWords = 0; i < numWords; i++)
        {
            u16 alphabeticalOrder = wordInfo[i].alphabeticalOrder;
            if (UnlockedECMonOrMove(alphabeticalOrder, groupId))
                sEasyChatSelectionData->allWords[totalWords++] = EC_WORD(groupId, alphabeticalOrder);
        }

        return totalWords;
    }
}

static u16 GetUnlockedWordsInAlphabeticalGroup(u16 alphabeticalGroup)
{
    u16 i;
    u16 totalWords;

    for (i = 0, totalWords = 0; i < sEasyChatSelectionData->alphabeticalGroups[alphabeticalGroup]; i++)
        sEasyChatSelectionData->allWords[totalWords++] = sEasyChatSelectionData->alphabeticalWordsByGroup[alphabeticalGroup][i];

    return totalWords;
}

static bool32 IsGroupSelectable(u32 groupIdx)
{
    u32 i;
    
    for (i = 0; i < sEasyChatSelectionData->numGroups; i++)
    {
        if (sEasyChatSelectionData->groups[i] == groupIdx)
            return TRUE;
    }
    return FALSE;
}

static bool32 UnlockedECMonOrMove(u16 wordIndex, u32 groupId)
{
    switch (groupId)
    {
    case EC_GROUP_POKEMON:
        return GetSetPokedexFlag(SpeciesToNationalPokedexNum(wordIndex), FLAG_GET_SEEN);
    case EC_GROUP_POKEMON_2:
        if (EC_IsDeoxys(wordIndex))
            return GetSetPokedexFlag(SpeciesToNationalPokedexNum(wordIndex), FLAG_GET_SEEN);
        return TRUE;
    case EC_GROUP_MOVE_1:
    case EC_GROUP_MOVE_2:
        return TRUE;
    default:
        return sEasyChatGroups[groupId].wordData.words[wordIndex].enabled;
    }
}

static bool32 EC_IsDeoxys(u32 species)
{
    u32 i;
    
    for (i = 0; i < ARRAY_COUNT(sDeoxysValue); i++)
    {
        if (sDeoxysValue[i] == species)
            return TRUE;
    }
    return FALSE;
}

static bool32 IsWordUnlocked(u16 easyChatWord)
{
    u8 groupId = EC_GROUP(easyChatWord);
    u32 index = EC_INDEX(easyChatWord);
    if (!IsGroupSelectable(groupId))
        return FALSE;
    else
        return UnlockedECMonOrMove(index, groupId);
}

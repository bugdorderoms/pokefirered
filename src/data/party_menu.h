static const struct BgTemplate sPartyMenuBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 0,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0
    },
    {
        .bg = 2,
        .charBaseIndex = 0,
        .mapBaseIndex = 28,
        .screenSize = 1,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
};

enum
{
    PARTY_BOX_LEFT_COLUMN,
    PARTY_BOX_RIGHT_COLUMN,
};

static const struct PartyMenuBoxInfoRects sPartyBoxInfoRects[] =
{
    [PARTY_BOX_LEFT_COLUMN] = 
    {
        BlitBitmapToPartyWindow_LeftColumn, 
        {
            // The below are the x, y, width, and height for each of the following info
            24, 11, 40, 13, // Nickname
            32, 20, 32,  8, // Level
            64, 20,  8,  8, // Gender
            38, 36, 24,  8, // HP
            53, 36, 24,  8, // Max HP
            24, 35, 48,  3  // HP bar
        }, 
        12, 34, 64, 16      // Description text (e.g. NO USE)
    },
    [PARTY_BOX_RIGHT_COLUMN] = 
    {
        BlitBitmapToPartyWindow_RightColumn, 
        {
             // The below are the x, y, width, and height for each of the following info
             22,  3, 40, 13, // Nickname
             32, 12, 32,  8, // Level
             64, 12,  8,  8, // Gender
            102, 12, 24,  8, // HP
            117, 12, 24,  8, // Max HP
             88, 10, 48,  3  // HP bar
        }, 
        77, 4, 64, 16        // Description text
    },
};

static const u8 sPartyMenuSpriteCoords[PARTY_LAYOUT_COUNT][PARTY_SIZE][4 * 2] =
{
    [PARTY_LAYOUT_SINGLE] = 
    {
        { 16,  40,  20,  50,  56,  52,  16,  34},
        {104,  18, 108,  28, 144,  27, 102,  25},
        {104,  42, 108,  52, 144,  51, 102,  49},
        {104,  66, 108,  76, 144,  75, 102,  73},
        {104,  90, 108, 100, 144,  99, 102,  97},
        {104, 114, 108, 124, 144, 123, 102, 121},
    },
    [PARTY_LAYOUT_DOUBLE] = 
    {
        { 16,  24,  20,  34,  56,  36,  16,  18},
        { 16,  80,  20,  90,  56,  92,  16,  74},
        {104,  18, 108,  28, 144,  27, 102,  25},
        {104,  50, 108,  60, 144,  59, 102,  57},
        {104,  82, 108,  92, 144,  91, 102,  89},
        {104, 114, 108, 124, 144, 123, 102, 121},
    },
    [PARTY_LAYOUT_MULTI] = 
    {
        { 16,  24,  20,  34,  56,  36,  16,  18},
        { 16,  80,  20,  90,  56,  92,  16,  74},
        {104,  26, 106,  36, 144,  35, 102,  33},
        {104,  50, 106,  60, 144,  59, 102,  57},
        {104,  82, 106,  92, 144,  91, 102,  89},
        {104, 106, 106, 116, 144, 115, 102, 113},
    },
    [PARTY_LAYOUT_MULTI_SHOWCASE] = 
    {
        { 16,  32,  20,  42,  56,  44,  16,  26},
        {104,  34, 106,  44, 144,  43, 102,  41},
        {104,  58, 106,  68, 144,  67, 102,  65},
        { 16, 104,  20, 114,  56, 116,  16,  98},
        {104, 106, 106, 116, 144, 115, 102, 113},
        {104, 130, 106, 140, 144, 139, 102, 137},
    },
};

static const u32 sConfirmButton_Tilemap[] = INCBIN_U32("graphics/interface/party_menu_confirm_button.bin");
static const u32 sCancelButton_Tilemap[] = INCBIN_U32("graphics/interface/party_menu_cancel_button.bin");

static const u8 sFontColorTable[][3] =
{
    {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_LIGHT_GRAY, TEXT_COLOR_DARK_GRAY},  // Default
    {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE,      TEXT_COLOR_GREEN},      // Unused
    {TEXT_COLOR_TRANSPARENT, TEXT_DYNAMIC_COLOR_2,  TEXT_DYNAMIC_COLOR_3},  // Gender symbol
    {TEXT_COLOR_WHITE,       TEXT_COLOR_DARK_GRAY,  TEXT_COLOR_LIGHT_GRAY}, // Selection actions
    {TEXT_COLOR_WHITE,       TEXT_COLOR_BLUE,       TEXT_COLOR_LIGHT_BLUE}, // Field moves
    {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE,      TEXT_COLOR_DARK_GRAY},  // Unused
};

static const struct WindowTemplate sSinglePartyMenuWindowTemplate[] =
{
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 3,
        .width = 10,
        .height = 7,
        .paletteNum = 3,
        .baseBlock = 0x63,
    },
    {
        .bg = 0,
        .tilemapLeft = 12,
        .tilemapTop = 1,
        .width = 18,
        .height = 3,
        .paletteNum = 4,
        .baseBlock = 0xA9,
    },
    {
        .bg = 0,
        .tilemapLeft = 12,
        .tilemapTop = 4,
        .width = 18,
        .height = 3,
        .paletteNum = 5,
        .baseBlock = 0xDF,
    },
    {
        .bg = 0,
        .tilemapLeft = 12,
        .tilemapTop = 7,
        .width = 18,
        .height = 3,
        .paletteNum = 6,
        .baseBlock = 0x115,
    },
    {
        .bg = 0,
        .tilemapLeft = 12,
        .tilemapTop = 10,
        .width = 18,
        .height = 3,
        .paletteNum = 7,
        .baseBlock = 0x14B,
    },
    {
        .bg = 0,
        .tilemapLeft = 12,
        .tilemapTop = 13,
        .width = 18,
        .height = 3,
        .paletteNum = 8,
        .baseBlock = 0x181,
    },
    {
        .bg = 2,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = 28,
        .height = 4,
        .paletteNum = 14,
        .baseBlock = 0x1DF,
    },
    DUMMY_WIN_TEMPLATE,
};

static const struct WindowTemplate sDoublePartyMenuWindowTemplate[] =
{
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 10,
        .height = 7,
        .paletteNum = 3,
        .baseBlock = 0x63,
    },
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 8,
        .width = 10,
        .height = 7,
        .paletteNum = 4,
        .baseBlock = 0xA9,
    },
    {
        .bg = 0,
        .tilemapLeft = 12,
        .tilemapTop = 1,
        .width = 18,
        .height = 3,
        .paletteNum = 5,
        .baseBlock = 0xEF,
    },
    {
        .bg = 0,
        .tilemapLeft = 12,
        .tilemapTop = 5,
        .width = 18,
        .height = 3,
        .paletteNum = 6,
        .baseBlock = 0x125,
    },
    {
        .bg = 0,
        .tilemapLeft = 12,
        .tilemapTop = 9,
        .width = 18,
        .height = 3,
        .paletteNum = 7,
        .baseBlock = 0x15B,
    },
    {
        .bg = 0,
        .tilemapLeft = 12,
        .tilemapTop = 13,
        .width = 18,
        .height = 3,
        .paletteNum = 8,
        .baseBlock = 0x191,
    },
    {
        .bg = 2,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = 28,
        .height = 4,
        .paletteNum = 14,
        .baseBlock = 0x1DF,
    },
    DUMMY_WIN_TEMPLATE,
};

static const struct WindowTemplate sMultiPartyMenuWindowTemplate[] =
{
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 10,
        .height = 7,
        .paletteNum = 3,
        .baseBlock = 0x63,
    },
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 8,
        .width = 10,
        .height = 7,
        .paletteNum = 4,
        .baseBlock = 0xA9,
    },
    {
        .bg = 0,
        .tilemapLeft = 12,
        .tilemapTop = 2,
        .width = 18,
        .height = 3,
        .paletteNum = 5,
        .baseBlock = 0xEF,
    },
    {
        .bg = 0,
        .tilemapLeft = 12,
        .tilemapTop = 5,
        .width = 18,
        .height = 3,
        .paletteNum = 6,
        .baseBlock = 0x125,
    },
    {
        .bg = 0,
        .tilemapLeft = 12,
        .tilemapTop = 9,
        .width = 18,
        .height = 3,
        .paletteNum = 7,
        .baseBlock = 0x15B,
    },
    {
        .bg = 0,
        .tilemapLeft = 12,
        .tilemapTop = 12,
        .width = 18,
        .height = 3,
        .paletteNum = 8,
        .baseBlock = 0x191,
    },
    {
        .bg = 2,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = 28,
        .height = 4,
        .paletteNum = 14,
        .baseBlock = 0x1DF,
    },
    DUMMY_WIN_TEMPLATE
};

static const struct WindowTemplate sShowcaseMultiPartyMenuWindowTemplate[] =
{
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 2,
        .width = 10,
        .height = 7,
        .paletteNum = 3,
        .baseBlock = 0x63,
    },
    {
        .bg = 0,
        .tilemapLeft = 12,
        .tilemapTop = 3,
        .width = 18,
        .height = 3,
        .paletteNum = 5,
        .baseBlock = 0xA9,
    },
    {
        .bg = 0,
        .tilemapLeft = 12,
        .tilemapTop = 6,
        .width = 18,
        .height = 3,
        .paletteNum = 6,
        .baseBlock = 0xDF,
    },
    {
        .bg = 2,
        .tilemapLeft = 1,
        .tilemapTop = 11,
        .width = 10,
        .height = 7,
        .paletteNum = 4,
        .baseBlock = 0x115,
    },
    {
        .bg = 2,
        .tilemapLeft = 12,
        .tilemapTop = 12,
        .width = 18,
        .height = 3,
        .paletteNum = 7,
        .baseBlock = 0x16B,
    },
    {
        .bg = 2,
        .tilemapLeft = 12,
        .tilemapTop = 15,
        .width = 18,
        .height = 3,
        .paletteNum = 8,
        .baseBlock = 0x1A1,
    },
    DUMMY_WIN_TEMPLATE
};

static const struct WindowTemplate sCancelButtonWindowTemplate =
{
    .bg = 0,
    .tilemapLeft = 24,
    .tilemapTop = 17,
    .width = 6,
    .height = 2,
    .paletteNum = 3,
    .baseBlock = 0x1C7,
};

static const struct WindowTemplate sMultiCancelButtonWindowTemplate =
{
    .bg = 0,
    .tilemapLeft = 24,
    .tilemapTop = 18,
    .width = 6,
    .height = 2,
    .paletteNum = 3,
    .baseBlock = 0x1C7,
};

static const struct WindowTemplate sConfirmButtonWindowTemplate =
{
    .bg = 0,
    .tilemapLeft = 24,
    .tilemapTop = 16,
    .width = 6,
    .height = 2,
    .paletteNum = 3,
    .baseBlock = 0x1D3,
};

static const struct WindowTemplate sDefaultPartyMsgWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 1,
    .tilemapTop = 17,
    .width = 21,
    .height = 2,
    .paletteNum = 15,
    .baseBlock = 0x24F,
};

static const struct WindowTemplate sDoWhatWithMonMsgWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 1,
    .tilemapTop = 17,
    .width = 16,
    .height = 2,
    .paletteNum = 15,
    .baseBlock = 0x279,
};

static const struct WindowTemplate sDoWhatWithItemMsgWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 1,
    .tilemapTop = 17,
    .width = 19,
    .height = 2,
    .paletteNum = 15,
    .baseBlock = 0x299,
};

static const struct WindowTemplate sDoWhatWithMailMsgWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 1,
    .tilemapTop = 17,
    .width = 16,
    .height = 2,
    .paletteNum = 15,
    .baseBlock = 0x299,
};

static const struct WindowTemplate sWhichMoveMsgWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 1,
    .tilemapTop = 17,
    .width = 15,
    .height = 2,
    .paletteNum = 15,
    .baseBlock = 0x299,
};

static const struct WindowTemplate sItemGiveTakeWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 22,
    .tilemapTop = 11,
    .width = 7,
    .height = 8,
    .paletteNum = 14,
    .baseBlock = 0x373,
};

static const struct WindowTemplate sMoveRelearnDeleteWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 22,
    .tilemapTop = 13,
    .width = 7,
    .height = 6,
    .paletteNum = 14,
    .baseBlock = 0x373,
};

static const struct WindowTemplate sMailReadTakeWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 19,
    .tilemapTop = 13,
    .width = 10,
    .height = 6,
    .paletteNum = 14,
    .baseBlock = 0x373,
};

static const struct WindowTemplate sMoveSelectWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 18,
    .tilemapTop = 11,
    .width = 11,
    .height = 8,
    .paletteNum = 14,
    .baseBlock = 0x2BF,
};

static const struct WindowTemplate sPartyMenuYesNoWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 21,
    .tilemapTop = 9,
    .width = 6,
    .height = 4,
    .paletteNum = 14,
    .baseBlock = 0x2BF,
};

static const struct WindowTemplate sLevelUpStatsWindowTemplate =
{
    .bg = 2,
    .tilemapLeft = 19,
    .tilemapTop = 1,
    .width = 10,
    .height = 11,
    .paletteNum = 14,
    .baseBlock = 0x2BF,
};

static const struct WindowTemplate sWindowTemplate_FirstBattleOakVoiceover =
{
    .bg = 2,
    .tilemapLeft = 2,
    .tilemapTop = 15,
    .width = 26,
    .height = 4,
    .paletteNum = 14,
    .baseBlock = 0x1DF,
};

static const struct WindowTemplate gUnknown_845A178 =
{
    .bg = 2,
    .tilemapLeft = 0,
    .tilemapTop = 13,
    .width = 18,
    .height = 3,
    .paletteNum = 12,
    .baseBlock = 0x373,
};

static const u8 sMainSlotTileNums[] =
{
    24, 25, 25, 25, 25, 25, 25, 25, 25, 26,
    32, 33, 33, 33, 33, 33, 33, 33, 33, 34,
    32, 33, 33, 33, 33, 33, 33, 33, 33, 34,
    32, 33, 33, 33, 33, 33, 33, 33, 33, 34,
    40, 59, 60, 58, 58, 58, 58, 58, 58, 61,
    15, 16, 16, 16, 16, 16, 16, 16, 16, 17,
    46, 47, 47, 47, 47, 47, 47, 47, 47, 48,
};

static const u8 sMainSlotTileNums_Egg[] =
{
    24, 25, 25, 25, 25, 25, 25, 25, 25, 26,
    32, 33, 33, 33, 33, 33, 33, 33, 33, 34,
    32, 33, 33, 33, 33, 33, 33, 33, 33, 34,
    32, 33, 33, 33, 33, 33, 33, 33, 33, 34,
    40, 41, 41, 41, 41, 41, 41, 41, 41, 42,
    15, 16, 16, 16, 16, 16, 16, 16, 16, 17,
    46, 47, 47, 47, 47, 47, 47, 47, 47, 48,
};

static const u8 sOtherSlotsTileNums[] =
{
    43, 44, 44, 44, 44, 44, 44, 44, 44, 44,
    44, 44, 44, 44, 44, 44, 44, 45, 49, 33,
    33, 33, 33, 33, 33, 33, 33, 52, 53, 51,
    51, 51, 51, 51, 51, 54, 55, 56, 56, 56,
    56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
    56, 56, 56, 57,
};

static const u8 sOtherSlotsTileNums_Egg[] =
{
    43, 44, 44, 44, 44, 44, 44, 44, 44, 44,
    44, 44, 44, 44, 44, 44, 44, 45, 49, 33,
    33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
    33, 33, 33, 33, 33, 50, 55, 56, 56, 56,
    56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
    56, 56, 56, 57,
};

static const u8 sEmptySlotTileNums[] = 
{
    21, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 23, 30,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0, 31, 37, 38, 38, 38,
    38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
    38, 38, 38, 39,
};

static const u8 sGenderPalOffsets[] = {11, 12};

static const u8 sHPBarPalOffsets[] = {9, 10};

static const u8 sPartyBoxPalOffsets1[] = {4, 5, 6};

static const u8 sPartyBoxPalOffsets2[] = {1, 7, 8};

static const u8 sGenderMalePalIds[] = {59, 60};

static const u8 sGenderFemalePalIds[] = {75, 76};

static const u8 sHPBarGreenPalIds[] = {57, 58};

static const u8 sHPBarYellowPalIds[] = {73, 74};

static const u8 sHPBarRedPalIds[] = {89, 90};

static const u8 sPartyBoxEmptySlotPalIds1[] = {52, 53, 54};

static const u8 sPartyBoxMultiPalIds1[] = {68, 69, 70};

static const u8 sPartyBoxFaintedPalIds1[] = {84, 85, 86};

static const u8 sPartyBoxCurrSelectionPalIds1[] = {116, 117, 118};

static const u8 sPartyBoxCurrSelectionMultiPalIds[] = {132, 133, 134};

static const u8 sPartyBoxCurrSelectionFaintedPalIds[] = {148, 149, 150};

static const u8 sPartyBoxSelectedForActionPalIds1[] = {100, 101, 102};

static const u8 sPartyBoxEmptySlotPalIds2[] = {49, 55, 56};

static const u8 sPartyBoxMultiPalIds2[] = {65, 71, 72};

static const u8 sPartyBoxFaintedPalIds2[] = {81, 87, 88};

static const u8 sPartyBoxCurrSelectionPalIds2[] = {97, 103, 104};

static const u8 sPartyBoxSelectedForActionPalIds2[] = {161, 167, 168};

static const u8 *const sActionStringTable[] =
{
    [PARTY_MSG_CHOOSE_MON]             = COMPOUND_STRING("Choose a Pokémon."),
    [PARTY_MSG_CHOOSE_MON_OR_CANCEL]   = COMPOUND_STRING("Choose Pokémon or Cancel."),
    [PARTY_MSG_CHOOSE_MON_AND_CONFIRM] = COMPOUND_STRING("Choose Pokémon and confirm."),
    [PARTY_MSG_MOVE_TO_WHERE]          = COMPOUND_STRING("Move to where?"),
    [PARTY_MSG_TEACH_WHICH_MON]        = COMPOUND_STRING("Teach which Pokémon?"),
    [PARTY_MSG_USE_ON_WHICH_MON]       = COMPOUND_STRING("Use on which Pokémon?"),
    [PARTY_MSG_GIVE_TO_WHICH_MON]      = COMPOUND_STRING("Give to which Pokémon?"),
    [PARTY_MSG_NOTHING_TO_CUT]         = COMPOUND_STRING("There's nothing to Cut."),
    [PARTY_MSG_CURRENT_TOO_FAST]       = COMPOUND_STRING("The current is much too fast!"),
    [PARTY_MSG_ENJOY_CYCLING]          = COMPOUND_STRING("Let's enjoy cycling!"),
    [PARTY_MSG_ALREADY_IN_USE]         = COMPOUND_STRING("This is in use already."),
    [PARTY_MSG_CANT_USE_HERE]          = gText_CantUseHere,
    [PARTY_MSG_NO_MON_FOR_BATTLE]      = COMPOUND_STRING("No battling this way!"),
    [PARTY_MSG_CHOOSE_MON_FOR_DAYCARE] = COMPOUND_STRING("Choose a Pokémon."),
    [PARTY_MSG_NOT_ENOUGH_HP]          = COMPOUND_STRING("Not enough HP‥"),
    [PARTY_MSG_THREE_MONS_ARE_NEEDED]  = COMPOUND_STRING("Three {PKMN} are needed."),
    [PARTY_MSG_TWO_MONS_ARE_NEEDED]    = COMPOUND_STRING("Two Pokémon are needed."),
    [PARTY_MSG_MONS_CANT_BE_SAME]      = COMPOUND_STRING("{PKMN} can't be the same."),
    [PARTY_MSG_NO_SAME_HOLD_ITEMS]     = COMPOUND_STRING("No same hold items!"),
    [PARTY_MSG_UNUSED]                 = COMPOUND_STRING(""),
    [PARTY_MSG_DO_WHAT_WITH_MON]       = COMPOUND_STRING("Do what with this {PKMN}?"),
    [PARTY_MSG_RESTORE_WHICH_MOVE]     = COMPOUND_STRING("Restore which move?"),
    [PARTY_MSG_BOOST_PP_WHICH_MOVE]    = COMPOUND_STRING("Boost PP of which?"),
    [PARTY_MSG_DO_WHAT_WITH_ITEM]      = COMPOUND_STRING("Do what with an item?"),
    [PARTY_MSG_DO_WHAT_WITH_MAIL]      = COMPOUND_STRING("Do what with the Mail?"),
    [PARTY_MSG_FUSE_WITH_WHICH]        = COMPOUND_STRING("Fuse with which Pokémon?"),
    [PARTY_MSG_DO_WHAT_WITH_MOVES]     = COMPOUND_STRING("Do what with the moves?"),
};

static const u8 *const sDescriptionStringTable[] =
{
    [PARTYBOX_DESC_NO_USE]     = COMPOUND_STRING("No use."),
    [PARTYBOX_DESC_ABLE_3]     = COMPOUND_STRING("Able"),
    [PARTYBOX_DESC_FIRST]      = COMPOUND_STRING("First"),
    [PARTYBOX_DESC_SECOND]     = COMPOUND_STRING("Second"),
    [PARTYBOX_DESC_THIRD]      = COMPOUND_STRING("Third"),
    [PARTYBOX_DESC_ABLE]       = COMPOUND_STRING("Able"),
    [PARTYBOX_DESC_NOT_ABLE]   = COMPOUND_STRING("Not Able"),
    [PARTYBOX_DESC_ABLE_2]     = COMPOUND_STRING("Able!"),
    [PARTYBOX_DESC_NOT_ABLE_2] = COMPOUND_STRING("Not Able!"),
    [PARTYBOX_DESC_LEARNED]    = COMPOUND_STRING("Learned"),
};

static const u8 *const sHMDescriptionTable[] =
{
    [FIELD_MOVE_FLASH]       = COMPOUND_STRING("Light up darkness."),
    [FIELD_MOVE_CUT]         = COMPOUND_STRING("Cut a tree or grass."),
    [FIELD_MOVE_WATERFALL]   = COMPOUND_STRING("Climb a waterfall."),
    [FIELD_MOVE_TELEPORT]    = COMPOUND_STRING("Return to a healing spot."),
    [FIELD_MOVE_DIG]         = COMPOUND_STRING("Escape from here."),
    [FIELD_MOVE_MILK_DRINK]  = COMPOUND_STRING("Share HP."),
    [FIELD_MOVE_SOFT_BOILED] = COMPOUND_STRING("Share HP."),
    [FIELD_MOVE_SWEET_SCENT] = COMPOUND_STRING("Lure wild Pokémon."),
    [FIELD_MOVE_DEFOG]       = COMPOUND_STRING("Blew away fog."),
};

static const struct OamData sOamData_MenuPokeball =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sPokeballAnim_Closed[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd sPokeballAnim_Open[] =
{
    ANIMCMD_FRAME(16, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_MenuPokeball[] =
{
    sPokeballAnim_Closed,
    sPokeballAnim_Open,
};

static const struct CompressedSpriteSheet sSpriteSheet_MenuPokeball =
{
    gPartyMenuPokeball_Gfx, 0x400, 0x04b0
};

static const struct CompressedSpritePalette sSpritePalette_MenuPokeball =
{
    gPartyMenuPokeball_Pal, 0x04b0
};

// Used for the pokeball sprite on each party slot / Cancel button
static const struct SpriteTemplate sSpriteTemplate_MenuPokeball =
{
    .tileTag = 0x04b0,
    .paletteTag = 0x04b0,
    .oam = &sOamData_MenuPokeball,
    .anims = sSpriteAnimTable_MenuPokeball,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct OamData sOamData_MenuPokeballSmall =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sSmallPokeballAnim_Closed[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd sSmallPokeballAnim_Open[] =
{
    ANIMCMD_FRAME(4, 0),
    ANIMCMD_END
};
static const union AnimCmd *const sSpriteAnimTable_MenuPokeballSmall[] =
{
    sSmallPokeballAnim_Closed,
    sSmallPokeballAnim_Open
};

static const struct CompressedSpriteSheet sSpriteSheet_MenuPokeballSmall =
{
    gPartyMenuPokeballSmall_Gfx, 0x0300, 0x04b1
};

// Used for the pokeball sprite next to Cancel and Confirm when both are present, otherwise sSpriteTemplate_MenuPokeball is used
static const struct SpriteTemplate sSpriteTemplate_MenuPokeballSmall =
{
    .tileTag = 1201,
    .paletteTag = 1200,
    .oam = &sOamData_MenuPokeballSmall,
    .anims = sSpriteAnimTable_MenuPokeballSmall,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct OamData sOamData_StatusCondition =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = SPRITE_SHAPE(32x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x8),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sSpriteAnim_StatusPoison[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusParalyzed[] =
{
    ANIMCMD_FRAME(4, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusSleep[] =
{
    ANIMCMD_FRAME(8, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusFrozen[] =
{
    ANIMCMD_FRAME(12, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusBurn[] =
{
    ANIMCMD_FRAME(16, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusPokerus[] =
{
    ANIMCMD_FRAME(20, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StatusFaint[] =
{
    ANIMCMD_FRAME(24, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_Blank[] =
{
    ANIMCMD_FRAME(28, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteTemplate_StatusCondition[] =
{
    sSpriteAnim_StatusPoison,
    sSpriteAnim_StatusParalyzed,
    sSpriteAnim_StatusSleep,
    sSpriteAnim_StatusFrozen,
    sSpriteAnim_StatusBurn,
    sSpriteAnim_StatusPokerus,
    sSpriteAnim_StatusFaint,
    sSpriteAnim_Blank,
};

static const struct CompressedSpriteSheet sSpriteSheet_StatusIcons =
{
    gStatusGfx_Icons, 0x400, 1202
};

static const struct CompressedSpritePalette sSpritePalette_StatusIcons =
{
    gStatusPal_Icons, 1202
};

static const struct SpriteTemplate sSpriteTemplate_StatusIcons =
{
    .tileTag = 1202,
    .paletteTag = 1202,
    .oam = &sOamData_StatusCondition,
    .anims = sSpriteTemplate_StatusCondition,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

enum
{
    MENU_SUMMARY,
    MENU_NICKNAME,
    MENU_SWITCH,
    MENU_CANCEL1,
    MENU_ITEM,
    MENU_GIVE,
    MENU_TAKE_ITEM,
    MENU_MOVE_ITEM,
    MENU_MOVES,
    MENU_REMEMBER_MOVE,
    MENU_FORGET_MOVE,
    MENU_MAIL,
    MENU_TAKE_MAIL,
    MENU_READ,
    MENU_CANCEL2,
    MENU_SHIFT,
    MENU_SEND_OUT,
    MENU_ENTER,
    MENU_NO_ENTRY,
    MENU_STORE,
    MENU_REGISTER,
    MENU_TRADE1,
    MENU_FIELD_MOVES,
};

static struct
{
    const u8 *text;
    TaskFunc func;
} const sCursorOptions[MENU_FIELD_MOVES] =
{
    [MENU_SUMMARY]       = {COMPOUND_STRING("Summary"),  CursorCB_Summary},
    [MENU_NICKNAME]      = {COMPOUND_STRING("Nickname"), CursorCB_Nickname},
    [MENU_SWITCH]        = {COMPOUND_STRING("Switch"),   CursorCB_Switch},
    [MENU_CANCEL1]       = {gMenuText_Cancel,            CursorCB_Cancel1},
    [MENU_ITEM]          = {COMPOUND_STRING("Item"),     CursorCB_Item},
    [MENU_GIVE]          = {COMPOUND_STRING("Give"),     CursorCB_Give},
    [MENU_TAKE_ITEM]     = {COMPOUND_STRING("Take"),     CursorCB_TakeItem},
    [MENU_MOVE_ITEM]     = {COMPOUND_STRING("Move"),     CursorCB_MoveItem},
    [MENU_MOVES]         = {COMPOUND_STRING("Moves"),    CursorCB_Moves},
    [MENU_REMEMBER_MOVE] = {COMPOUND_STRING("Remember"), CursorCB_RelearnMove},
    [MENU_FORGET_MOVE]   = {COMPOUND_STRING("Forget"),   CursorCB_DeleteMove},
    [MENU_MAIL]          = {COMPOUND_STRING("Mail"),     CursorCB_Mail},
    [MENU_TAKE_MAIL]     = {COMPOUND_STRING("Take"),     CursorCB_TakeMail},
    [MENU_READ]          = {COMPOUND_STRING("Read"),     CursorCB_Read},
    [MENU_CANCEL2]       = {gMenuText_Cancel,            CursorCB_Cancel2},
    [MENU_SHIFT]         = {COMPOUND_STRING("Shift"),    CursorCB_SendMon},
    [MENU_SEND_OUT]      = {COMPOUND_STRING("Send Out"), CursorCB_SendMon},
    [MENU_ENTER]         = {COMPOUND_STRING("Enter"),    CursorCB_Enter},
    [MENU_NO_ENTRY]      = {COMPOUND_STRING("No Entry"), CursorCB_NoEntry},
    [MENU_STORE]         = {COMPOUND_STRING("Store"),    CursorCB_Store},
    [MENU_REGISTER]      = {COMPOUND_STRING("Register"), CursorCB_Register},
    [MENU_TRADE1]        = {COMPOUND_STRING("Trade"),    CursorCB_Trade1},
};

static const u8 sPartyMenuAction_SummarySwitchCancel[] = {MENU_SUMMARY, MENU_SWITCH, MENU_CANCEL1};
static const u8 sPartyMenuAction_ShiftSummaryCancel[] = {MENU_SHIFT, MENU_SUMMARY, MENU_CANCEL1};
static const u8 sPartyMenuAction_SendOutSummaryCancel[] = {MENU_SEND_OUT, MENU_SUMMARY, MENU_CANCEL1};
static const u8 sPartyMenuAction_SummaryCancel[] = {MENU_SUMMARY, MENU_CANCEL1};
static const u8 sPartyMenuAction_EnterSummaryCancel[] = {MENU_ENTER, MENU_SUMMARY, MENU_CANCEL1};
static const u8 sPartyMenuAction_NoEntrySummaryCancel[] = {MENU_NO_ENTRY, MENU_SUMMARY, MENU_CANCEL1};
static const u8 sPartyMenuAction_StoreSummaryCancel[] = {MENU_STORE, MENU_SUMMARY, MENU_CANCEL1};
static const u8 sPartyMenuAction_GiveTakeItemCancel[] = {MENU_GIVE, MENU_TAKE_ITEM, MENU_MOVE_ITEM, MENU_CANCEL2};
static const u8 sPartyMenuAction_RelearnDeleteMoveCancel[] = {MENU_REMEMBER_MOVE, MENU_FORGET_MOVE, MENU_CANCEL2};
static const u8 sPartyMenuAction_ReadTakeMailCancel[] = {MENU_READ, MENU_TAKE_MAIL, MENU_CANCEL2};
static const u8 sPartyMenuAction_RegisterSummaryCancel[] = {MENU_REGISTER, MENU_SUMMARY, MENU_CANCEL1};
static const u8 sPartyMenuAction_TradeSummaryCancel1[] = {MENU_TRADE1, MENU_SUMMARY, MENU_CANCEL1};

// IDs for the action lists that appear when a party mon is selected
enum
{
    ACTIONS_NONE,
    ACTIONS_SWITCH,
    ACTIONS_SHIFT,
    ACTIONS_SEND_OUT,
    ACTIONS_ENTER,
    ACTIONS_NO_ENTRY,
    ACTIONS_STORE,
    ACTIONS_SUMMARY_ONLY,
    ACTIONS_ITEM,
    ACTIONS_MOVES,
    ACTIONS_MAIL,
    ACTIONS_REGISTER,
    ACTIONS_TRADE,
};

static const u8 *const sPartyMenuActions[] =
{
    [ACTIONS_NONE]          = NULL,
    [ACTIONS_SWITCH]        = sPartyMenuAction_SummarySwitchCancel,
    [ACTIONS_SHIFT]         = sPartyMenuAction_ShiftSummaryCancel,
    [ACTIONS_SEND_OUT]      = sPartyMenuAction_SendOutSummaryCancel,
    [ACTIONS_ENTER]         = sPartyMenuAction_EnterSummaryCancel,
    [ACTIONS_NO_ENTRY]      = sPartyMenuAction_NoEntrySummaryCancel,
    [ACTIONS_STORE]         = sPartyMenuAction_StoreSummaryCancel,
    [ACTIONS_SUMMARY_ONLY]  = sPartyMenuAction_SummaryCancel,
    [ACTIONS_ITEM]          = sPartyMenuAction_GiveTakeItemCancel,
    [ACTIONS_MOVES]         = sPartyMenuAction_RelearnDeleteMoveCancel,
    [ACTIONS_MAIL]          = sPartyMenuAction_ReadTakeMailCancel,
    [ACTIONS_REGISTER]      = sPartyMenuAction_RegisterSummaryCancel,
    [ACTIONS_TRADE]         = sPartyMenuAction_TradeSummaryCancel1,
};

static const u8 sPartyMenuActionCounts[] =
{
    [ACTIONS_NONE]          = 0,
    [ACTIONS_SWITCH]        = ARRAY_COUNT(sPartyMenuAction_SummarySwitchCancel),
    [ACTIONS_SHIFT]         = ARRAY_COUNT(sPartyMenuAction_ShiftSummaryCancel),
    [ACTIONS_SEND_OUT]      = ARRAY_COUNT(sPartyMenuAction_SendOutSummaryCancel),
    [ACTIONS_ENTER]         = ARRAY_COUNT(sPartyMenuAction_EnterSummaryCancel),
    [ACTIONS_NO_ENTRY]      = ARRAY_COUNT(sPartyMenuAction_NoEntrySummaryCancel),
    [ACTIONS_STORE]         = ARRAY_COUNT(sPartyMenuAction_StoreSummaryCancel),
    [ACTIONS_SUMMARY_ONLY]  = ARRAY_COUNT(sPartyMenuAction_SummaryCancel),
    [ACTIONS_ITEM]          = ARRAY_COUNT(sPartyMenuAction_GiveTakeItemCancel),
    [ACTIONS_MOVES]         = ARRAY_COUNT(sPartyMenuAction_RelearnDeleteMoveCancel),
    [ACTIONS_MAIL]          = ARRAY_COUNT(sPartyMenuAction_ReadTakeMailCancel),
    [ACTIONS_REGISTER]      = ARRAY_COUNT(sPartyMenuAction_RegisterSummaryCancel),
    [ACTIONS_TRADE]         = ARRAY_COUNT(sPartyMenuAction_TradeSummaryCancel1),
};

static const u16 sFieldMoves[] =
{
    [FIELD_MOVE_FLASH]       = MOVE_FLASH,
    [FIELD_MOVE_CUT]         = MOVE_CUT,
    [FIELD_MOVE_WATERFALL]   = MOVE_WATERFALL,
    [FIELD_MOVE_TELEPORT]    = MOVE_TELEPORT,
    [FIELD_MOVE_DIG]         = MOVE_DIG,
    [FIELD_MOVE_MILK_DRINK]  = MOVE_MILK_DRINK,
    [FIELD_MOVE_SOFT_BOILED] = MOVE_SOFT_BOILED,
    [FIELD_MOVE_SWEET_SCENT] = MOVE_SWEET_SCENT,
    [FIELD_MOVE_DEFOG]       = MOVE_DEFOG,
    [FIELD_MOVE_END]         = FIELD_MOVE_END
};

static struct
{
    bool32 (*fieldMoveFunc)(void);
    u8 msgId;
} const sFieldMoveCursorCallbacks[] =
{
    [FIELD_MOVE_FLASH]        = {SetUpFieldMove_Flash,       PARTY_MSG_CANT_USE_HERE},
    [FIELD_MOVE_CUT]          = {SetUpFieldMove_Cut,         PARTY_MSG_NOTHING_TO_CUT},
    [FIELD_MOVE_WATERFALL]    = {SetUpFieldMove_Waterfall,   PARTY_MSG_CANT_USE_HERE},
    [FIELD_MOVE_TELEPORT]     = {SetUpFieldMove_Teleport,    PARTY_MSG_CANT_USE_HERE},
    [FIELD_MOVE_DIG]          = {SetUpFieldMove_Dig,         PARTY_MSG_CANT_USE_HERE},
    [FIELD_MOVE_MILK_DRINK]   = {SetUpFieldMove_SoftBoiled,  PARTY_MSG_NOT_ENOUGH_HP},
    [FIELD_MOVE_SOFT_BOILED]  = {SetUpFieldMove_SoftBoiled,  PARTY_MSG_NOT_ENOUGH_HP},
    [FIELD_MOVE_SWEET_SCENT]  = {SetUpFieldMove_SweetScent,  PARTY_MSG_CANT_USE_HERE},
    [FIELD_MOVE_DEFOG]        = {SetUpFieldMove_Defog,       PARTY_MSG_CANT_USE_HERE},
};

static const u8 sText_CantTradeWithTrainer[] = _("You can't trade with that\nTrainer now.");

static const u8 *const sUnionRoomTradeMessages[] =
{
    [UR_TRADE_MSG_NOT_MON_PARTNER_WANTS - 1]       = COMPOUND_STRING("That isn't the type of Pokémon\nthat the other Trainer wants."),
    [UR_TRADE_MSG_NOT_EGG - 1]                     = COMPOUND_STRING("That isn't an Egg."),
    [UR_TRADE_MSG_MON_CANT_BE_TRADED_1 - 1]        = gText_PkmnCantBeTradedNow,
    [UR_TRADE_MSG_MON_CANT_BE_TRADED_2 - 1]        = gText_PkmnCantBeTradedNow,
    [UR_TRADE_MSG_PARTNERS_MON_CANT_BE_TRADED - 1] = gText_OtherTrainersPkmnCantBeTraded,
    [UR_TRADE_MSG_EGG_CANT_BE_TRADED -1]           = gText_EggCantBeTradedNow,
    [UR_TRADE_MSG_PARTNER_CANT_ACCEPT_MON - 1]     = COMPOUND_STRING("The other Trainer can't accept\nthat Pokémon now."),
    [UR_TRADE_MSG_CANT_TRADE_WITH_PARTNER_1 - 1]   = sText_CantTradeWithTrainer,
    [UR_TRADE_MSG_CANT_TRADE_WITH_PARTNER_2 - 1]   = sText_CantTradeWithTrainer,
};

#include "global.h"
#include "gflib.h"
#include "data.h"
#include "menu.h"
#include "task.h"
#include "item.h"
#include "item_menu_icons.h"
#include "script_menu.h"
#include "new_menu_helpers.h"
#include "event_data.h"
#include "script.h"
#include "trainer_pokemon_sprites.h"
#include "field_effect.h"
#include "field_specials.h"
#include "overworld.h"
#include "event_scripts.h"
#include "constants/songs.h"
#include "constants/seagallop.h"
#include "constants/menu.h"
#include "constants/species.h"
#include "constants/items.h"

#define TAG_MUSEUM_FOSSIL_PIC 7000

struct MultichoiceListStruct
{
    const struct MenuAction * list;
    u8 count;
};

static void Task_MultichoiceMenu_HandleInput(u32 taskId);
static void Task_YesNoMenu_HandleInput(u32 taskId);
static void Task_ScriptShowPic(u32 taskId);
static bool32 PicboxWaitFunc(void);

static const struct MenuAction sScriptMultiChoiceMenu_YesNo[] = {
    { gMenuText_Yes },
    { gMenuText_No }
};

static const struct MenuAction sScriptMultiChoiceMenu_TrainerCardIconTint[] = {
    { COMPOUND_STRING("Normal") },
    { COMPOUND_STRING("Black") },
    { COMPOUND_STRING("Pink") },
    { COMPOUND_STRING("Sepia") }
};

static const struct MenuAction sScriptMultiChoiceMenu_HOF_Quit[] = {
    { COMPOUND_STRING("Hall Of Fame") },
    { COMPOUND_STRING("Quit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_Eggs_Quit[] = {
    { COMPOUND_STRING("Eggs") },
    { COMPOUND_STRING("Quit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_Victories_Quit[] = {
    { COMPOUND_STRING("Victories") },
    { COMPOUND_STRING("Quit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_HOF_Eggs_Quit[] = {
    { COMPOUND_STRING("Hall Of Fame") },
    { COMPOUND_STRING("Eggs") },
    { COMPOUND_STRING("Quit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_HOF_Victories_Quit[] = {
    { COMPOUND_STRING("Hall Of Fame") },
    { COMPOUND_STRING("Victories") },
    { COMPOUND_STRING("Quit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_Eggs_Victories_Quit[] = {
    { COMPOUND_STRING("Eggs") },
    { COMPOUND_STRING("Victories") },
    { COMPOUND_STRING("Quit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_HOF_Eggs_Victories_Quit[] = {
    { COMPOUND_STRING("Hall Of Fame") },
    { COMPOUND_STRING("Eggs") },
    { COMPOUND_STRING("Victories") },
    { COMPOUND_STRING("Quit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_TrainerSchoolWhiteboard[] = {
    { COMPOUND_STRING("Slp") },
    { COMPOUND_STRING("Psn") },
    { COMPOUND_STRING("Par") },
    { COMPOUND_STRING("Brn") },
    { COMPOUND_STRING("Frz") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_YesNoInfo[] = {
    { gMenuText_Yes },
    { gMenuText_No },
    { COMPOUND_STRING("Info") }
};

static const struct MenuAction sScriptMultiChoiceMenu_SingleDoubleMultiInfoExit[] = {
    { COMPOUND_STRING("Single Battle") },
    { COMPOUND_STRING("Double Battle") },
    { COMPOUND_STRING("Multi Battle") },
    { COMPOUND_STRING("Info") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_YesNoInfo2[] = {
    { gMenuText_Yes },
    { gMenuText_No },
    { COMPOUND_STRING("Info") }
};

static const struct MenuAction sScriptMultiChoiceMenu_ChallengeInfoExit[] = {
    { COMPOUND_STRING("Make a challenge.") },
    { COMPOUND_STRING("Info") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_RooftopB1F[] = {
    { gText_Rooftop },
    { gText_B1F },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_Helix[] = {
    { COMPOUND_STRING("Helix Fossil") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_Dome[] = {
    { COMPOUND_STRING("Dome Fossil") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_Amber[] = {
    { COMPOUND_STRING("Old Amber") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_HelixAmber[] = {
    { COMPOUND_STRING("Helix Fossil") },
    { COMPOUND_STRING("Old Amber") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_DomeAmber[] = {
    { COMPOUND_STRING("Dome Fossil") },
    { COMPOUND_STRING("Old Amber") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_CeladonVendingMachine[] = {
    { COMPOUND_STRING("Fresh Water{CLEAR_TO 0x57}{FONT_SMALL}¥200") },
    { COMPOUND_STRING("Soda Pop{CLEAR_TO 0x57}{FONT_SMALL}¥300") },
    { COMPOUND_STRING("Lemonade{CLEAR_TO 0x57}{FONT_SMALL}¥350") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_GameCornerTMPrizes[] = {
    { COMPOUND_STRING("TM13{CLEAR_TO 0x48}{FONT_SMALL}4,000 Coins") },
    { COMPOUND_STRING("TM23{CLEAR_TO 0x48}{FONT_SMALL}3,500 Coins") },
    { COMPOUND_STRING("TM24{CLEAR_TO 0x48}{FONT_SMALL}4,000 Coins") },
    { COMPOUND_STRING("TM30{CLEAR_TO 0x48}{FONT_SMALL}4,500 Coins") },
    { COMPOUND_STRING("TM35{CLEAR_TO 0x48}{FONT_SMALL}4,000 Coins") },
    { COMPOUND_STRING("No Thanks") }
};

static const struct MenuAction sScriptMultiChoiceMenu_GameCornerBattleItemPrizes[] = {
    { COMPOUND_STRING("Smoke Ball{CLEAR_TO 0x5A}{FONT_SMALL}800 Coins") },
    { COMPOUND_STRING("Miracle Seed{CLEAR_TO 0x50}{FONT_SMALL}1,000 Coins") },
    { COMPOUND_STRING("Charcoal{CLEAR_TO 0x50}{FONT_SMALL}1,000 Coins") },
    { COMPOUND_STRING("Mystic Water{CLEAR_TO 0x50}{FONT_SMALL}1,000 Coins") },
    { COMPOUND_STRING("Yellow Flute{CLEAR_TO 0x50}{FONT_SMALL}1,600 Coins") },
    { COMPOUND_STRING("No Thanks") }
};

static const struct MenuAction sScriptMultiChoiceMenu_GameCornerCoinPurchaseCounter[] = {
    { COMPOUND_STRING("{FONT_SMALL} 50 Coins{CLEAR_TO 0x45}¥1,000") },
    { COMPOUND_STRING("{FONT_SMALL}500 Coins{CLEAR_TO 0x40}¥10,000") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_Excellent_NotSoBad[] = {
    { COMPOUND_STRING("Excellent") },
    { COMPOUND_STRING("Not so bad") }
};

static const struct MenuAction sScriptMultiChoiceMenu_RightLeft[] = {
    { COMPOUND_STRING("Right") },
    { COMPOUND_STRING("Left") }
};

static const struct MenuAction sScriptMultiChoiceMenu_DeptStoreElevator[] = {
    { gText_5F },
    { gText_4F },
    { gText_3F },
    { gText_2F },
    { gText_1F },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlFreshWater[] = {
    { COMPOUND_STRING("Fresh Water") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlSodaPop[] = {
    { COMPOUND_STRING("Soda Pop") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlFreshWaterSodaPop[] = {
    { COMPOUND_STRING("Fresh Water") },
    { COMPOUND_STRING("Soda Pop") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlLemonade[] = {
    { COMPOUND_STRING("Lemonade") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlFreshWaterLemonade[] = {
    { COMPOUND_STRING("Fresh Water") },
    { COMPOUND_STRING("Lemonade") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlSodaPopLemonade[] = {
    { COMPOUND_STRING("Soda Pop") },
    { COMPOUND_STRING("Lemonade") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlFreshWaterSodaPopLemonade[] = {
    { COMPOUND_STRING("Fresh Water") },
    { COMPOUND_STRING("Soda Pop") },
    { COMPOUND_STRING("Lemonade") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_Eeveelutions[] = {
    { COMPOUND_STRING("Eevee") },
    { COMPOUND_STRING("Flareon") },
    { COMPOUND_STRING("Jolteon") },
    { COMPOUND_STRING("Vaporeon") },
    { COMPOUND_STRING("Quit looking.") }
};

static const struct MenuAction sScriptMultiChoiceMenu_BikeShop[] = {
    { COMPOUND_STRING("Bicycle{CLEAR_TO 0x49}{FONT_SMALL}¥1,000,000") },
    { COMPOUND_STRING("No Thanks") }
};

static const struct MenuAction sScriptMultiChoiceMenu_GameCornerPokemonPrizes[] = {
#if defined(FIRERED)
    { COMPOUND_STRING("Abra{CLEAR_TO 0x55}{FONT_SMALL} 180 Coins") },
    { COMPOUND_STRING("Clefairy{CLEAR_TO 0x55}{FONT_SMALL} 500 Coins") },
    { COMPOUND_STRING("Dratini{CLEAR_TO 0x4B}{FONT_SMALL} 2,800 Coins") },
    { COMPOUND_STRING("Scyther{CLEAR_TO 0x4B}{FONT_SMALL} 5,500 Coins") },
    { COMPOUND_STRING("Porygon{CLEAR_TO 0x4B}{FONT_SMALL} 9,999 Coins") },
#elif defined(LEAFGREEN)
    { COMPOUND_STRING("Abra{CLEAR_TO 0x55}{FONT_SMALL} 120 Coins") },
    { COMPOUND_STRING("Clefairy{CLEAR_TO 0x55}{FONT_SMALL} 750 Coins") },
    { COMPOUND_STRING("Pinsir{CLEAR_TO 0x4B}{FONT_SMALL} 2,500 Coins") },
    { COMPOUND_STRING("Dratini{CLEAR_TO 0x4B}{FONT_SMALL} 4,600 Coins") },
    { COMPOUND_STRING("Porygon{CLEAR_TO 0x4B}{FONT_SMALL} 6,500 Coins") },
#endif
    { COMPOUND_STRING("No Thanks") }
};

static const struct MenuAction sScriptMultiChoiceMenu_TradeCenter_Colosseum[] = {
    { COMPOUND_STRING("Trade Center") },
    { COMPOUND_STRING("Colosseum") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_Link_Wireless[] = {
    { COMPOUND_STRING("Game Link cable") },
    { COMPOUND_STRING("Wireless") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_RocketHideoutElevator[] = {
    { gText_B1F },
    { gText_B2F },
    { gText_B4F },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_LinkedDirectUnion[] = {
    { COMPOUND_STRING("Linked Game Play") },
    { COMPOUND_STRING("Direct Corner") },
    { COMPOUND_STRING("Union Room") },
    { COMPOUND_STRING("Quit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_Island23[] = {
    { COMPOUND_STRING("Two Island") },
    { COMPOUND_STRING("Three Island") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_Island13[] = {
    { COMPOUND_STRING("One Island") },
    { COMPOUND_STRING("Three Island") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_Island12[] = {
    { COMPOUND_STRING("One Island") },
    { COMPOUND_STRING("Two Island") },
    { COMPOUND_STRING("Exit") }
};

// Unused
static const struct MenuAction sScriptMultiChoiceMenu_TradeColosseumCrush[] = {
    { COMPOUND_STRING("Trade Center") },
    { COMPOUND_STRING("Colosseum") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_48[] = {
    { COMPOUND_STRING("") },
    { COMPOUND_STRING("") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_TradeColosseum_2[] = {
    { COMPOUND_STRING("Trade Center") },
    { COMPOUND_STRING("Colosseum") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_PokejumpDodrio[] = {
    { COMPOUND_STRING("Pokémon Jump") },
    { COMPOUND_STRING("Dodrio Berry-Picking") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_Mushrooms[] = {
    { COMPOUND_STRING("2 Tinymushrooms") },
    { COMPOUND_STRING("1 Big Mushroom") }
};

static const struct MenuAction sScriptMultiChoiceMenu_TradeColosseumBlank[] = {
    { COMPOUND_STRING("Trade Center") },
    { COMPOUND_STRING("Colosseum") },
    { COMPOUND_STRING("") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeviiNavel[] = {
    { COMPOUND_STRING("Sevii Island") },
    { COMPOUND_STRING("Navel Rock") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeviiBirth[] = {
    { COMPOUND_STRING("Sevii Island") },
    { COMPOUND_STRING("Birth Island") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeviiNavelBirth[] = {
    { COMPOUND_STRING("Sevii Island") },
    { COMPOUND_STRING("Navel Rock") },
    { COMPOUND_STRING("Birth Island") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_Seagallop123[] = {
    { COMPOUND_STRING("One Island") },
    { COMPOUND_STRING("Two Island") },
    { COMPOUND_STRING("Three Island") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeagallopV23[] = {
    { COMPOUND_STRING("Vermilion") },
    { COMPOUND_STRING("Two Island") },
    { COMPOUND_STRING("Three Island") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeagallopV13[] = {
    { COMPOUND_STRING("Vermilion") },
    { COMPOUND_STRING("One Island") },
    { COMPOUND_STRING("Three Island") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeagallopV12[] = {
    { COMPOUND_STRING("Vermilion") },
    { COMPOUND_STRING("One Island") },
    { COMPOUND_STRING("Two Island") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeagallopVermilion[] = {
    { COMPOUND_STRING("Vermilion") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_62[] = {
    { COMPOUND_STRING("") },
    { COMPOUND_STRING("") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_JoinOrLead[] = {
    { COMPOUND_STRING("Join Group") },
    { COMPOUND_STRING("Become Leader") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_TrainerTowerMode[] = {
    { COMPOUND_STRING("Single") },
    { COMPOUND_STRING("Double") },
    { COMPOUND_STRING("Knockout") },
    { COMPOUND_STRING("Mixed") },
    { COMPOUND_STRING("Exit") }
};

static const struct MenuAction sScriptMultiChoiceMenu_Exit[] = {
    { COMPOUND_STRING("Exit") }
};

const struct MultichoiceListStruct gScriptMultiChoiceMenus[] = {
    { sScriptMultiChoiceMenu_YesNo,  ARRAY_COUNT(sScriptMultiChoiceMenu_YesNo)  },
    { sScriptMultiChoiceMenu_Eeveelutions,  ARRAY_COUNT(sScriptMultiChoiceMenu_Eeveelutions)  },
    { sScriptMultiChoiceMenu_TrainerCardIconTint,  ARRAY_COUNT(sScriptMultiChoiceMenu_TrainerCardIconTint)  },
    { sScriptMultiChoiceMenu_HOF_Quit,  ARRAY_COUNT(sScriptMultiChoiceMenu_HOF_Quit)  },
    { sScriptMultiChoiceMenu_Eggs_Quit,  ARRAY_COUNT(sScriptMultiChoiceMenu_Eggs_Quit)  },
    { sScriptMultiChoiceMenu_Victories_Quit,  ARRAY_COUNT(sScriptMultiChoiceMenu_Victories_Quit)  },
    { sScriptMultiChoiceMenu_HOF_Eggs_Quit,  ARRAY_COUNT(sScriptMultiChoiceMenu_HOF_Eggs_Quit)  },
    { sScriptMultiChoiceMenu_HOF_Victories_Quit,  ARRAY_COUNT(sScriptMultiChoiceMenu_HOF_Victories_Quit)  },
    { sScriptMultiChoiceMenu_Eggs_Victories_Quit,  ARRAY_COUNT(sScriptMultiChoiceMenu_Eggs_Victories_Quit)  },
    { sScriptMultiChoiceMenu_HOF_Eggs_Victories_Quit,  ARRAY_COUNT(sScriptMultiChoiceMenu_HOF_Eggs_Victories_Quit)  },
    { sScriptMultiChoiceMenu_Exit, ARRAY_COUNT(sScriptMultiChoiceMenu_Exit) },
    { sScriptMultiChoiceMenu_Exit, ARRAY_COUNT(sScriptMultiChoiceMenu_Exit) },
    { sScriptMultiChoiceMenu_Exit, ARRAY_COUNT(sScriptMultiChoiceMenu_Exit) },
    { sScriptMultiChoiceMenu_BikeShop, ARRAY_COUNT(sScriptMultiChoiceMenu_BikeShop) },
    { sScriptMultiChoiceMenu_GameCornerPokemonPrizes, ARRAY_COUNT(sScriptMultiChoiceMenu_GameCornerPokemonPrizes) },
    { sScriptMultiChoiceMenu_TrainerSchoolWhiteboard, ARRAY_COUNT(sScriptMultiChoiceMenu_TrainerSchoolWhiteboard) },
    { sScriptMultiChoiceMenu_YesNoInfo, ARRAY_COUNT(sScriptMultiChoiceMenu_YesNoInfo) },
    { sScriptMultiChoiceMenu_SingleDoubleMultiInfoExit, ARRAY_COUNT(sScriptMultiChoiceMenu_SingleDoubleMultiInfoExit) },
    { sScriptMultiChoiceMenu_YesNoInfo2, ARRAY_COUNT(sScriptMultiChoiceMenu_YesNoInfo2) },
    { sScriptMultiChoiceMenu_ChallengeInfoExit, ARRAY_COUNT(sScriptMultiChoiceMenu_ChallengeInfoExit) },
    { sScriptMultiChoiceMenu_RooftopB1F, ARRAY_COUNT(sScriptMultiChoiceMenu_RooftopB1F) },
    { sScriptMultiChoiceMenu_Helix, ARRAY_COUNT(sScriptMultiChoiceMenu_Helix) },
    { sScriptMultiChoiceMenu_Dome, ARRAY_COUNT(sScriptMultiChoiceMenu_Dome) },
    { sScriptMultiChoiceMenu_Amber, ARRAY_COUNT(sScriptMultiChoiceMenu_Amber) },
    { sScriptMultiChoiceMenu_HelixAmber, ARRAY_COUNT(sScriptMultiChoiceMenu_HelixAmber) },
    { sScriptMultiChoiceMenu_DomeAmber, ARRAY_COUNT(sScriptMultiChoiceMenu_DomeAmber) },
    { sScriptMultiChoiceMenu_CeladonVendingMachine, ARRAY_COUNT(sScriptMultiChoiceMenu_CeladonVendingMachine) },
    { sScriptMultiChoiceMenu_GameCornerCoinPurchaseCounter, ARRAY_COUNT(sScriptMultiChoiceMenu_GameCornerCoinPurchaseCounter) },
    { sScriptMultiChoiceMenu_Excellent_NotSoBad, ARRAY_COUNT(sScriptMultiChoiceMenu_Excellent_NotSoBad) },
    { sScriptMultiChoiceMenu_RightLeft, ARRAY_COUNT(sScriptMultiChoiceMenu_RightLeft) },
    { sScriptMultiChoiceMenu_GameCornerTMPrizes, ARRAY_COUNT(sScriptMultiChoiceMenu_GameCornerTMPrizes) },
    { sScriptMultiChoiceMenu_DeptStoreElevator, ARRAY_COUNT(sScriptMultiChoiceMenu_DeptStoreElevator) },
    { sScriptMultiChoiceMenu_ThirstyGirlFreshWater, ARRAY_COUNT(sScriptMultiChoiceMenu_ThirstyGirlFreshWater) },
    { sScriptMultiChoiceMenu_ThirstyGirlSodaPop, ARRAY_COUNT(sScriptMultiChoiceMenu_ThirstyGirlSodaPop) },
    { sScriptMultiChoiceMenu_ThirstyGirlFreshWaterSodaPop, ARRAY_COUNT(sScriptMultiChoiceMenu_ThirstyGirlFreshWaterSodaPop) },
    { sScriptMultiChoiceMenu_ThirstyGirlLemonade, ARRAY_COUNT(sScriptMultiChoiceMenu_ThirstyGirlLemonade) },
    { sScriptMultiChoiceMenu_ThirstyGirlFreshWaterLemonade, ARRAY_COUNT(sScriptMultiChoiceMenu_ThirstyGirlFreshWaterLemonade) },
    { sScriptMultiChoiceMenu_ThirstyGirlSodaPopLemonade, ARRAY_COUNT(sScriptMultiChoiceMenu_ThirstyGirlSodaPopLemonade) },
    { sScriptMultiChoiceMenu_ThirstyGirlFreshWaterSodaPopLemonade, ARRAY_COUNT(sScriptMultiChoiceMenu_ThirstyGirlFreshWaterSodaPopLemonade) },
    { sScriptMultiChoiceMenu_TradeCenter_Colosseum, ARRAY_COUNT(sScriptMultiChoiceMenu_TradeCenter_Colosseum) },
    { sScriptMultiChoiceMenu_Link_Wireless, ARRAY_COUNT(sScriptMultiChoiceMenu_Link_Wireless) },
    { sScriptMultiChoiceMenu_GameCornerBattleItemPrizes, ARRAY_COUNT(sScriptMultiChoiceMenu_GameCornerBattleItemPrizes) },
    { sScriptMultiChoiceMenu_RocketHideoutElevator, ARRAY_COUNT(sScriptMultiChoiceMenu_RocketHideoutElevator) },
    { sScriptMultiChoiceMenu_LinkedDirectUnion, ARRAY_COUNT(sScriptMultiChoiceMenu_LinkedDirectUnion) },
    { sScriptMultiChoiceMenu_Island23, ARRAY_COUNT(sScriptMultiChoiceMenu_Island23) },
    { sScriptMultiChoiceMenu_Island13, ARRAY_COUNT(sScriptMultiChoiceMenu_Island13) },
    { sScriptMultiChoiceMenu_Island12, ARRAY_COUNT(sScriptMultiChoiceMenu_Island12) },
    { sScriptMultiChoiceMenu_TradeColosseumCrush, ARRAY_COUNT(sScriptMultiChoiceMenu_TradeColosseumCrush) },
    { sScriptMultiChoiceMenu_48, ARRAY_COUNT(sScriptMultiChoiceMenu_48) },
    { sScriptMultiChoiceMenu_PokejumpDodrio, ARRAY_COUNT(sScriptMultiChoiceMenu_PokejumpDodrio) },
    { sScriptMultiChoiceMenu_TradeColosseum_2, ARRAY_COUNT(sScriptMultiChoiceMenu_TradeColosseum_2) },
    { sScriptMultiChoiceMenu_Mushrooms, ARRAY_COUNT(sScriptMultiChoiceMenu_Mushrooms) },
    { sScriptMultiChoiceMenu_TradeColosseumBlank, ARRAY_COUNT(sScriptMultiChoiceMenu_TradeColosseumBlank) },
    { sScriptMultiChoiceMenu_SeviiNavel, ARRAY_COUNT(sScriptMultiChoiceMenu_SeviiNavel) },
    { sScriptMultiChoiceMenu_SeviiBirth, ARRAY_COUNT(sScriptMultiChoiceMenu_SeviiBirth) },
    { sScriptMultiChoiceMenu_SeviiNavelBirth, ARRAY_COUNT(sScriptMultiChoiceMenu_SeviiNavelBirth) },
    { sScriptMultiChoiceMenu_Seagallop123, ARRAY_COUNT(sScriptMultiChoiceMenu_Seagallop123) },
    { sScriptMultiChoiceMenu_SeagallopV23, ARRAY_COUNT(sScriptMultiChoiceMenu_SeagallopV23) },
    { sScriptMultiChoiceMenu_SeagallopV13, ARRAY_COUNT(sScriptMultiChoiceMenu_SeagallopV13) },
    { sScriptMultiChoiceMenu_SeagallopV12, ARRAY_COUNT(sScriptMultiChoiceMenu_SeagallopV12) },
    { sScriptMultiChoiceMenu_SeagallopVermilion, ARRAY_COUNT(sScriptMultiChoiceMenu_SeagallopVermilion) },
    { sScriptMultiChoiceMenu_62, ARRAY_COUNT(sScriptMultiChoiceMenu_62) },
    { sScriptMultiChoiceMenu_JoinOrLead, ARRAY_COUNT(sScriptMultiChoiceMenu_JoinOrLead) },
    { sScriptMultiChoiceMenu_TrainerTowerMode, ARRAY_COUNT(sScriptMultiChoiceMenu_TrainerTowerMode) }
};

// From Cool to Berries goes unused
const u8 *const gStdStringPtrs[] = {
    [STDSTRING_COOL]             = COMPOUND_STRING("Cool"),
    [STDSTRING_BEAUTY]           = COMPOUND_STRING("Beauty"),
    [STDSTRING_CUTE]             = COMPOUND_STRING("Cute"),
    [STDSTRING_SMART]            = COMPOUND_STRING("Smart"),
    [STDSTRING_TOUGH]            = COMPOUND_STRING("Tough"),
    [STDSTRING_COOL2]            = COMPOUND_STRING("Cool"),
    [STDSTRING_BEAUTY2]          = COMPOUND_STRING("Beauty"),
    [STDSTRING_CUTE2]            = COMPOUND_STRING("Cute"),
    [STDSTRING_SMART2]           = COMPOUND_STRING("Smart"),
    [STDSTRING_TOUGH2]           = COMPOUND_STRING("Tough"),
    [STDSTRING_ITEMS]            = COMPOUND_STRING("Items"),
    [STDSTRING_KEY_ITEMS]        = COMPOUND_STRING("Key Items"),
    [STDSTRING_POKEBALLS]        = COMPOUND_STRING("Poké Balls"),
    [STDSTRING_TMHMS]            = COMPOUND_STRING("TMs & HMs"),
    [STDSTRING_BERRIES]          = COMPOUND_STRING("Berries"),
    [STDSTRING_BOULDER_BADGE]    = COMPOUND_STRING("Boulderbadge"),
    [STDSTRING_CASCADE_BADGE]    = COMPOUND_STRING("Cascadebadge"),
    [STDSTRING_THUNDER_BADGE]    = COMPOUND_STRING("Thunderbadge"),
    [STDSTRING_RAINBOW_BADGE]    = COMPOUND_STRING("Rainbowbadge"),
    [STDSTRING_SOUL_BADGE]       = COMPOUND_STRING("Soulbadge"),
    [STDSTRING_MARSH_BADGE]      = COMPOUND_STRING("Marshbadge"),
    [STDSTRING_VOLCANO_BADGE]    = COMPOUND_STRING("Volcanobadge"),
    [STDSTRING_EARTH_BADGE]      = COMPOUND_STRING("Earthbadge"),
    [STDSTRING_COINS]            = COMPOUND_STRING("Coins"),
    [STDSTRING_ITEMS_POCKET]     = COMPOUND_STRING("Items Pocket"),
    [STDSTRING_KEY_ITEMS_POCKET] = COMPOUND_STRING("Key Items Pocket"),
    [STDSTRING_POKEBALLS_POCKET] = COMPOUND_STRING("Poké Balls Pocket"),
    [STDSTRING_TM_CASE]          = COMPOUND_STRING("TM Case"),
    [STDSTRING_BERRY_POUCH]      = COMPOUND_STRING("Berry Pouch")
};

static const union AnimCmd sMuseumFossilAnim0[] = {
    ANIMCMD_FRAME(0, 10),
    ANIMCMD_END
};

static const union AnimCmd *const sMuseumFossilAnimCmdTable[] = {
    sMuseumFossilAnim0
};

static const struct OamData sMuseumFossilOamData = {
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64)
};

static const struct SpriteTemplate sMuseumFossilSprTemplate = {
    .tileTag = TAG_MUSEUM_FOSSIL_PIC,
    .paletteTag = 0xFFFF,
    .oam = &sMuseumFossilOamData,
    .anims = sMuseumFossilAnimCmdTable,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

static const u16 sMuseumAerodactylSprTiles[] = INCBIN_U16("graphics/script_menu/aerodactyl_fossil.4bpp");
static const u16 sMuseumAerodactylSprPalette[] = INCBIN_U16("graphics/script_menu/aerodactyl_fossil.gbapal");
static const u16 sMuseumKabutopsSprTiles[] = INCBIN_U16("graphics/script_menu/kabutops_fossil.4bpp");
static const u16 sMuseumKabutopsSprPalette[] = INCBIN_U16("graphics/script_menu/kabutops_fossil.gbapal");

static const struct SpriteSheet sMuseumKabutopsSprSheets[] = {
    {sMuseumKabutopsSprTiles, 0x800, TAG_MUSEUM_FOSSIL_PIC},
    {}
};

static const struct SpriteSheet sMuseumAerodactylSprSheets[] = {
    {sMuseumAerodactylSprTiles, 0x800, TAG_MUSEUM_FOSSIL_PIC},
    {}
};

static const u8 *const sSeagallopDestStrings[] = {
    COMPOUND_STRING("Vermilion"),
    COMPOUND_STRING("One Island"),
    COMPOUND_STRING("Two Island"),
    COMPOUND_STRING("Three Island"),
    COMPOUND_STRING("Four Island"),
    COMPOUND_STRING("Five Island"),
    COMPOUND_STRING("Six Island"),
    COMPOUND_STRING("Seven Island")
};

static u16 GetStringTilesWide(const u8 *str)
{
    return (GetStringWidth(1, str, 0) + 7) / 8;
}

static u32 GetMenuWidthFromList(const struct MenuAction * items, u32 count)
{
    u32 i;
    u8 width = GetStringTilesWide(items[0].text);
    u8 tmp;

    for (i = 1; i < count; i++)
    {
        tmp = GetStringTilesWide(items[i].text);
        if (width < tmp)
            width = tmp;
    }
    return width;
}

static u32 CreateMultichoiceWindow(u32 bg, u8 x, u8 y, u8 width, u8 height, u16 baseBlock, u32 palNum)
{
    struct WindowTemplate template = SetWindowTemplateFields(bg, x + 1, y + 1, width, height, palNum, baseBlock);
    u32 windowId = AddWindow(&template);
    
    PutWindowTilemap(windowId);
    
    return windowId;
}

static void DestroyMultichoiceWindow(u32 windowId)
{
    ClearWindowTilemap(windowId);
    ClearStdWindowAndFrameToTransparent(windowId, TRUE);
    RemoveWindow(windowId);
}

bool32 ScriptMenu_Multichoice(u8 x, u8 y, u32 mcId, bool32 ignoreBPress, u32 defaultOpt, u32 perRowItems)
{
    if (!FuncIsActiveTask(Task_MultichoiceMenu_HandleInput))
    {
        InitMultichoice(gScriptMultiChoiceMenus[mcId].list, gScriptMultiChoiceMenus[mcId].count, 0, x, y, ignoreBPress, defaultOpt, perRowItems, MULTICHOICE_DEFAULT_BASE_BLOCK, 15);
        return TRUE;
    }
    return FALSE;
}

#define tIgnoreBPress data[0]
#define tWindowId     data[1]

static u32 CreateMultichoiceInputTask(bool32 ignoreBPress, u32 windowId)
{
    u32 taskId = CreateTask(Task_MultichoiceMenu_HandleInput, 80);
    
    gTasks[taskId].tIgnoreBPress = ignoreBPress;
    gTasks[taskId].tWindowId = windowId;
    
    return taskId;
}

static void InitDefaultMultichoiceOnLeftTop(const struct MenuAction * items, u32 count)
{
    InitMultichoice(items, count, 0, 0, 0, FALSE, 0, 1, MULTICHOICE_DEFAULT_BASE_BLOCK, 15);
}

u32 InitMultichoice(const struct MenuAction * items, u32 count, u32 bg, u8 x, u8 y, bool32 ignoreBPress, u32 defaultOpt, u32 perRowItems, u16 baseBlock, u32 palNum)
{
    u32 windowId, taskId = 0xFF;
    u8 width, rowCount;
    
    gSpecialVar_Result = SCR_MENU_UNSET;
    
    width = GetMenuWidthFromList(items, count) + 1;
    rowCount = count / perRowItems;
    
    windowId = CreateMultichoiceWindow(bg, x, y, width * perRowItems, rowCount * 2, baseBlock, palNum);
    SetStdWindowBorderStyle(windowId, FALSE);
    
    taskId = CreateMultichoiceInputTask(ignoreBPress, windowId);
    
    MultichoiceGrid_PrintItems(windowId, 1, width * 8, 16, perRowItems, rowCount, items);
    MultichoiceGrid_InitCursor(windowId, 1, 0, 1, width * 8, perRowItems, rowCount, defaultOpt);
    
    ScheduleBgCopyTilemapToVram(bg);

    return taskId;
}

static void Task_MultichoiceMenu_HandleInput(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    s8 input = Menu_ProcessInputGridLayout();
    
    switch (input)
    {
        case MENU_NOTHING_CHOSEN:
            return;
        case MENU_B_PRESSED:
            if (!tIgnoreBPress)
            {
                PlaySE(SE_SELECT);
                gSpecialVar_Result = SCR_MENU_CANCEL;
                break;
            }
            return;
        default:
            gSpecialVar_Result = input;
            break;
    }
    DestroyMultichoiceWindow(tWindowId);
    EnableBothScriptContexts();
    DestroyTask(taskId);
}

static void CreatePCMenuMultichoice(void)
{
    u32 nItems = 0;
    struct MenuAction menuItems[5]; // max of items
    
    // "Bill" or "Someone"'s PC
    menuItems[nItems++].text = FlagGet(FLAG_SYS_NOT_SOMEONES_PC) ? gText_BillSPc : gText_SomeoneSPc;
    
    // "Player"'s PC
    StringCopy(gStringVar4, gSaveBlock2Ptr->playerName);
    StringAppend(gStringVar4, gText_SPc);
    menuItems[nItems++].text = gStringVar4;
    
    // Prof Oak's PC
    if (FlagGet(FLAG_SYS_POKEDEX_GET))
        menuItems[nItems++].text = gText_ProfOakSPc;
    
    // Hall Of Fame
    if (FlagGet(FLAG_SYS_GAME_CLEAR))
        menuItems[nItems++].text = COMPOUND_STRING("Hall Of Fame");
    
    // Log off
    menuItems[nItems++].text = COMPOUND_STRING("Log Off");
    
    InitDefaultMultichoiceOnLeftTop(menuItems, nItems);
}

bool32 CreatePCMenu(void)
{
    if (!FuncIsActiveTask(Task_MultichoiceMenu_HandleInput))
    {
        CreatePCMenuMultichoice();
        return TRUE;
    }
    return FALSE;
}

void ScriptMenu_DisplayPCStartupPrompt(void)
{
    LoadMessageBoxAndFrameGfx(0, TRUE);
    AddTextPrinterParameterized2(0, 2, Text_AccessWhichPC, 0, NULL, 2, 1, 3);
}

void DrawSeagallopDestinationMenu(void)
{
    // 8004 = Starting location
    // 8005 = Page (0: Verm, One, Two, Three, Four, Other, Exit; 1: Four, Five, Six, Seven, Other, Exit)
    u32 i, windowId, defaultOpt, cursorWidth, nItems = 5, y = 0;
    
    if (gSpecialVar_0x8005 == 1)
    {
        defaultOpt = gSpecialVar_0x8004 < SEAGALLOP_FIVE_ISLAND ? SEAGALLOP_FIVE_ISLAND : SEAGALLOP_FOUR_ISLAND;
        y = 2;
    }
    else
    {
        defaultOpt = SEAGALLOP_VERMILION_CITY;
        ++nItems;
    }
    cursorWidth = GetMenuCursorDimensionByFont(2, 0);
    windowId = CreateMultichoiceWindow(0, 17, y, 11, nItems * 2, MULTICHOICE_DEFAULT_BASE_BLOCK, 15);
    SetStdWindowBorderStyle(windowId, FALSE);
    
    // -2 excludes "Other" and "Exit", appended after the loop
    for (i = 0; i < nItems - 2; i++)
    {
        if (defaultOpt != gSpecialVar_0x8004)
            AddTextPrinterParameterized(windowId, 2, sSeagallopDestStrings[defaultOpt], cursorWidth, i * 16 + 2, 0xFF, NULL);
        else
            --i;
        
        ++defaultOpt;
        
        // Wrap around
        if (defaultOpt == SEAGALLOP_CINNABAR_ISLAND)
            defaultOpt = SEAGALLOP_VERMILION_CITY;
    }
    AddTextPrinterParameterized(windowId, 2, COMPOUND_STRING("Other"), cursorWidth, i * 16 + 2, 0xFF, NULL);
    AddTextPrinterParameterized(windowId, 2, COMPOUND_STRING("Exit"), cursorWidth, (i + 1) * 16 + 2, 0xFF, NULL);
    Menu_InitCursor(windowId, 2, 0, 2, 16, nItems, 0);
    CreateMultichoiceInputTask(FALSE, windowId);
    ScheduleBgCopyTilemapToVram(0);
}

u32 GetSelectedSeagallopDestination(void)
{
    // 8004 = Starting location
    // 8005 = Page (0: Verm, One, Two, Three, Four, Other, Exit; 1: Four, Five, Six, Seven, Other, Exit)
    
    if (gSpecialVar_Result == SCR_MENU_CANCEL)
        return SCR_MENU_CANCEL;
    else if (gSpecialVar_0x8005 == 1)
    {
        switch (gSpecialVar_Result)
        {
            case 0:
                if (gSpecialVar_0x8004 > SEAGALLOP_FOUR_ISLAND)
                    return SEAGALLOP_FOUR_ISLAND;
                else
                    return SEAGALLOP_FIVE_ISLAND;
            case 1:
                if (gSpecialVar_0x8004 > SEAGALLOP_FIVE_ISLAND)
                    return SEAGALLOP_FIVE_ISLAND;
                else
                    return SEAGALLOP_SIX_ISLAND;
            case 2:
                if (gSpecialVar_0x8004 > SEAGALLOP_SIX_ISLAND)
                    return SEAGALLOP_SIX_ISLAND;
                else
                    return SEAGALLOP_SEVEN_ISLAND;
            case 3:
                return SEAGALLOP_MORE;
            case 4:
                return SCR_MENU_CANCEL;
        }
    }
    else
    {
        switch (gSpecialVar_Result)
        {
            case 4:
                return SEAGALLOP_MORE;
            case 5:
                return SCR_MENU_CANCEL;
            default:
                if (gSpecialVar_Result >= gSpecialVar_0x8004)
                    return gSpecialVar_Result + 1;
                else
                    return gSpecialVar_Result;
        }
    }
    return SEAGALLOP_VERMILION_CITY;
}

void DrawRepelMultichoiseMenu(void)
{
    u32 i, count = 0;
    struct MenuAction menuItems[LAST_REPEL_ITEM - FIRST_REPEL_ITEM];
    
    gSpecialVar_Result = FALSE;
    
    for (i = FIRST_REPEL_ITEM; i <= LAST_REPEL_ITEM; i++)
    {
        if (CheckBagHasItem(i, 1))
        {
            VarSet(VAR_0x8004 + count, i);
            menuItems[count++].text = ItemId_GetName(i);
        }
    }
    if (count > 1)
    {
        InitDefaultMultichoiceOnLeftTop(menuItems, count);
        gSpecialVar_Result = TRUE;
    }
}

void HandleRepelUseAnother(void)
{
    gSpecialVar_0x8004 = VarGet(VAR_0x8004 + gSpecialVar_Result);
    VarSet(VAR_REPEL_STEP_COUNT, ItemId_GetHoldEffectParam(gSpecialVar_0x8004));
}

#undef tIgnoreBPress
#undef tWindowId

bool32 ScriptMenu_YesNo(void)
{
    if (!FuncIsActiveTask(Task_YesNoMenu_HandleInput))
    {
        gSpecialVar_Result = SCR_MENU_UNSET;
        DisplayYesNoMenuDefaultYes();
        CreateTask(Task_YesNoMenu_HandleInput, 80);
        return TRUE;
    }
    return FALSE;
}

static void Task_YesNoMenu_HandleInput(u32 taskId)
{
    if (gTasks[taskId].data[2] < 5)
        ++gTasks[taskId].data[2];
    else
    {
        switch (Menu_ProcessInputNoWrapClearOnChoose())
        {
            case MENU_NOTHING_CHOSEN:
                return;
            case MENU_B_PRESSED:
            case MENU_ACTION_NO:
                PlaySE(SE_SELECT);
                gSpecialVar_Result = FALSE;
                break;
            case MENU_ACTION_YES:
                gSpecialVar_Result = TRUE;
                break;
        }
        DestroyTask(taskId);
        EnableBothScriptContexts();
    }
}

static u32 CreateMenuMonPic(u32 species, u8 x, u8 y)
{
    u32 spriteId = CreateMonPicSprite(species, FALSE, 0x8000, TRUE, 8 * x + 40, 8 * y + 40, 0, ITEMICON_TAG);

    gSprites[spriteId].callback = SpriteCallbackDummy;
    gSprites[spriteId].oam.priority = 0;
    
    if (Overworld_GetFlashLevel() > 0)
        gSprites[spriteId].copyToObjWin = TRUE;
    
    return spriteId;
}

static u32 CreateMenuItemPic(u32 itemId, u16 x, u16 y)
{
    u32 spriteId = AddItemIconObject(ITEMICON_TAG, ITEMICON_TAG, itemId);
    
    x = 8 * x + 20;
    y = 8 * y + 20;
    
    gSprites[spriteId].x = x;
    gSprites[spriteId].y = y;
    gSprites[spriteId].oam.priority = 0;
    
    if (Overworld_GetFlashLevel() > 0)
        gSprites[spriteId].copyToObjWin = TRUE;
    
    return spriteId;
}

enum
{
    PIC_TYPE_POKEMON,
    PIC_TYPE_MUSEUM_FOSSIL,
    PIC_TYPE_ITEM,
};

static void DestroyPicboxPic(u32 picType, u32 spriteId)
{
    struct Sprite *sprite = &gSprites[spriteId];
    
    switch (picType)
    {
        case PIC_TYPE_POKEMON:
            FreeResourcesAndDestroySprite(sprite, spriteId);
            break;
        case PIC_TYPE_MUSEUM_FOSSIL:
            DestroySprite(sprite);
            FreeSpriteTilesByTag(TAG_MUSEUM_FOSSIL_PIC);
            break;
        case PIC_TYPE_ITEM:
            DestroySpriteAndFreeResources(sprite);
            break;
    }
}

#define tWindowId  data[0]
#define tSpriteId  data[1]
#define tState     data[2]
#define tPicType   data[3]

static void CreatePicBox(u32 spriteId, s16 x, s16 y, u8 width, u8 height, u32 picType)
{
    s16 *data = gTasks[CreateTask(Task_ScriptShowPic, 80)].data;
    
    tWindowId = CreateMultichoiceWindow(0, x, y, width, height, MULTICHOICE_DEFAULT_BASE_BLOCK, 15);
    tSpriteId = spriteId;
    tState = 0;
    tPicType = picType;
    
    SetStdWindowBorderStyle(tWindowId, TRUE);
    ScheduleBgCopyTilemapToVram(0);
}

bool32 ScriptMenu_ShowPokemonPic(u32 species, u8 x, u8 y)
{
    if (FindTaskIdByFunc(Task_ScriptShowPic) != 0xFF)
        return FALSE;
    else
    {
        CreatePicBox(CreateMenuMonPic(species, x, y), x, y, 8, 8, PIC_TYPE_POKEMON);
        return TRUE;
    }
}

static void Task_ScriptShowPic(u32 taskId)
{
    s16 *data = gTasks[taskId].data;
    
    switch (tState)
    {
        case 0:
            ++tState;
            break;
        case 1:
            break;
        case 2:
            DestroyPicboxPic(tPicType, tSpriteId);
            ++tState;
            break;
        case 3:
            DestroyMultichoiceWindow(tWindowId);
            DestroyTask(taskId);
            break;
    }
}

bool32 (*ScriptMenu_GetPicboxWaitFunc(void))(void)
{
    u32 taskId = FindTaskIdByFunc(Task_ScriptShowPic);
    
    if (taskId == 0xFF)
        return NULL;
    
    ++gTasks[taskId].tState;
    
    return PicboxWaitFunc;
}

static bool32 PicboxWaitFunc(void)
{
    return (FindTaskIdByFunc(Task_ScriptShowPic) == 0xFF);
}

void PicboxCancel(void)
{
    u32 taskId = FindTaskIdByFunc(Task_ScriptShowPic);
    s16 *data;
    
    if (taskId != 0xFF)
    {
        data = gTasks[taskId].data;
        
        switch (tState)
        {
            case 0:
            case 1:
            case 2:
                DestroyPicboxPic(tPicType, tSpriteId);
                // fallthrought
            case 3:
                DestroyMultichoiceWindow(tWindowId);
                DestroyTask(taskId);
                break;
        }
    }
}

void RemovePokemonSpeciesOnPicbox(void)
{
    u32 taskId = FindTaskIdByFunc(Task_ScriptShowPic);
    
    if (taskId != 0xFF)
        DestroyPicboxPic(PIC_TYPE_POKEMON, gTasks[taskId].tSpriteId);
}

void UpdatePokemonSpeciesOnPicbox(u32 species, u8 x, u8 y)
{
    u32 taskId = FindTaskIdByFunc(Task_ScriptShowPic);
    
    if (taskId != 0xFF)
        gTasks[taskId].tSpriteId = CreateMenuMonPic(species, x, y);
}

bool32 OpenMuseumFossilPic(void)
{
    u32 spriteId;
    
    if (FindTaskIdByFunc(Task_ScriptShowPic) != 0xFF)
        return FALSE;
    else
    {
        switch (gSpecialVar_0x8004)
        {
            case SPECIES_KABUTOPS:
                LoadSpriteSheets(sMuseumKabutopsSprSheets);
                LoadPalette(sMuseumKabutopsSprPalette, 0x1D0, 0x20);
                break;
            case SPECIES_AERODACTYL:
                LoadSpriteSheets(sMuseumAerodactylSprSheets);
                LoadPalette(sMuseumAerodactylSprPalette, 0x1D0, 0x20);
                break;
            default:
                return FALSE;
        }
        spriteId = CreateSprite(&sMuseumFossilSprTemplate, gSpecialVar_0x8005 * 8 + 40, gSpecialVar_0x8006 * 8 + 40, 0);
        gSprites[spriteId].oam.paletteNum = 13;
        
        CreatePicBox(spriteId, gSpecialVar_0x8005, gSpecialVar_0x8006, 8, 8, PIC_TYPE_MUSEUM_FOSSIL);

        return TRUE;
    }
}

bool32 CloseMuseumFossilPic(void)
{
    u32 taskId = FindTaskIdByFunc(Task_ScriptShowPic);
    
    if (taskId == 0xFF)
        return FALSE;
    
    ++gTasks[taskId].tState;
    
    return TRUE;
}

bool32 ScriptMenu_ShowItemPic(u32 itemId, u8 x, u8 y)
{
    if (FindTaskIdByFunc(Task_ScriptShowPic) != 0xFF)
        return FALSE;
    else
    {
        CreatePicBox(CreateMenuItemPic(itemId, x, y), x, y, 3, 3, PIC_TYPE_ITEM);
        return TRUE;
    }
}

#undef tWindowId
#undef tSpriteId
#undef tState
#undef tPicType

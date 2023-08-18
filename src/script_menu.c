#include "global.h"
#include "gflib.h"
#include "data.h"
#include "menu.h"
#include "task.h"
#include "item.h"
#include "script_menu.h"
#include "quest_log.h"
#include "new_menu_helpers.h"
#include "event_data.h"
#include "script.h"
#include "trainer_pokemon_sprites.h"
#include "strings.h"
#include "field_effect.h"
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

static void Task_MultichoiceMenu_HandleInput(u8 taskId);
static void Task_YesNoMenu_HandleInput(u8 taskId);
static void Task_ScriptShowMonPic(u8 taskId);
static bool8 PicboxWaitFunc(void);

static const struct MenuAction sScriptMultiChoiceMenu_YesNo[] = {
    { gText_Yes },
    { gText_No }
};

static const struct MenuAction sScriptMultiChoiceMenu_TrainerCardIconTint[] = {
    { gText_Normal },
    { gText_Black },
    { gText_Pink },
    { gText_Sepia }
};

static const struct MenuAction sScriptMultiChoiceMenu_HOF_Quit[] = {
    { gText_HallOfFame },
    { gText_Quit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Eggs_Quit[] = {
    { gText_Eggs },
    { gText_Quit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Victories_Quit[] = {
    { gText_Victories },
    { gText_Quit }
};

static const struct MenuAction sScriptMultiChoiceMenu_HOF_Eggs_Quit[] = {
    { gText_HallOfFame },
    { gText_Eggs },
    { gText_Quit }
};

static const struct MenuAction sScriptMultiChoiceMenu_HOF_Victories_Quit[] = {
    { gText_HallOfFame },
    { gText_Victories },
    { gText_Quit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Eggs_Victories_Quit[] = {
    { gText_Eggs },
    { gText_Victories },
    { gText_Quit }
};

static const struct MenuAction sScriptMultiChoiceMenu_HOF_Eggs_Victories_Quit[] = {
    { gText_HallOfFame },
    { gText_Eggs },
    { gText_Victories },
    { gText_Quit }
};

static const struct MenuAction sScriptMultiChoiceMenu_TrainerSchoolWhiteboard[] = {
    { gText_Slp },
    { gText_Psn },
    { gText_Par },
    { gText_Brn },
    { gText_Frz },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_YesNoInfo[] = {
    { gText_Yes },
    { gText_No },
    { gText_Info }
};

static const struct MenuAction sScriptMultiChoiceMenu_SingleDoubleMultiInfoExit[] = {
    { gText_SingleBattle },
    { gText_DoubleBattle },
    { gText_MultiBattle },
    { gText_Info },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_YesNoInfo2[] = {
    { gText_Yes },
    { gText_No },
    { gText_Info }
};

static const struct MenuAction sScriptMultiChoiceMenu_ChallengeInfoExit[] = {
    { gText_MakeAChallenge },
    { gText_Info_2 },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_RooftopB1F[] = {
    { gText_Rooftop },
    { gText_B1F },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Helix[] = {
    { gText_HelixFossil },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Dome[] = {
    { gText_DomeFossil },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Amber[] = {
    { gText_OldAmber },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_HelixAmber[] = {
    { gText_HelixFossil },
    { gText_OldAmber },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_DomeAmber[] = {
    { gText_DomeFossil },
    { gText_OldAmber },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_CeladonVendingMachine[] = {
    { gText_FreshWater_200 },
    { gText_SodaPop_300 },
    { gText_Lemonade_350 },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_GameCornerTMPrizes[] = {
    { gText_Tm13_4000Coins },
    { gText_Tm23_3500Coins },
    { gText_Tm24_4000Coins },
    { gText_Tm30_4500Coins },
    { gText_Tm35_4000Coins },
    { gText_NoThanks_2 }
};

static const struct MenuAction sScriptMultiChoiceMenu_GameCornerBattleItemPrizes[] = {
    { gText_SmokeBall_800Coins },
    { gText_MiracleSeed_1000Coins },
    { gText_Charcoal_1000Coins },
    { gText_MysticWater_1000Coins },
    { gText_YellowFlute_1600Coins },
    { gText_NoThanks_2 }
};

static const struct MenuAction sScriptMultiChoiceMenu_GameCornerCoinPurchaseCounter[] = {
    { gText_50Coins_1000 },
    { gText_500Coins_10000 },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Excellent_NotSoBad[] = {
    { gText_Excellent },
    { gText_NotSoBad }
};

static const struct MenuAction sScriptMultiChoiceMenu_RightLeft[] = {
    { gText_Right },
    { gText_Left }
};

static const struct MenuAction sScriptMultiChoiceMenu_DeptStoreElevator[] = {
    { gText_5F_2 },
    { gText_4F_2 },
    { gText_3F_2 },
    { gText_2F_2 },
    { gText_1F_2 },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlFreshWater[] = {
    { gText_FreshWater },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlSodaPop[] = {
    { gText_SodaPop },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlFreshWaterSodaPop[] = {
    { gText_FreshWater },
    { gText_SodaPop },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlLemonade[] = {
    { gText_Lemonade },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlFreshWaterLemonade[] = {
    { gText_FreshWater },
    { gText_Lemonade },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlSodaPopLemonade[] = {
    { gText_SodaPop },
    { gText_Lemonade },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_ThirstyGirlFreshWaterSodaPopLemonade[] = {
    { gText_FreshWater },
    { gText_SodaPop },
    { gText_Lemonade },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Unref_Shards_0[] = {
    { gText_GreenShard },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Unref_Shards_1[] = {
    { gText_RedShard },
    { gText_GreenShard },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Unref_Shards_2[] = {
    { gText_YellowShard },
    { gText_GreenShard },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Unref_Shards_3[] = {
    { gText_RedShard },
    { gText_YellowShard },
    { gText_GreenShard },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Unref_Shards_4[] = {
    { gText_BlueShard },
    { gText_GreenShard },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Unref_Shards_5[] = {
    { gText_RedShard },
    { gText_BlueShard },
    { gText_GreenShard },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Unref_Shards_6[] = {
    { gText_YellowShard },
    { gText_BlueShard },
    { gText_GreenShard },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Unref_Shards_7[] = {
    { gText_RedShard },
    { gText_YellowShard },
    { gText_BlueShard },
    { gText_GreenShard },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Eeveelutions[] = {
    { gText_Eevee },
    { gText_Flareon },
    { gText_Jolteon },
    { gText_Vaporeon },
    { gText_QuitLooking }
};

static const struct MenuAction sScriptMultiChoiceMenu_BikeShop[] = {
    { gText_Bicycle_1000000 },
    { gText_NoThanks }
};

static const struct MenuAction sScriptMultiChoiceMenu_GameCornerPokemonPrizes[] = {
#if defined(FIRERED)
    { gText_Abra_180Coins },
    { gText_Clefairy_500Coins },
    { gText_Dratini_2800Coins },
    { gText_Scyther_5500Coins },
    { gText_Porygon_9999Coins },
#elif defined(LEAFGREEN)
    { gText_Abra_120Coins },
    { gText_Clefairy_750Coins },
    { gText_Pinsir_2500Coins },
    { gText_Dratini_4600Coins },
    { gText_Porygon_6500Coins },
#endif
    { gText_NoThanks_2 }
};

static const struct MenuAction sScriptMultiChoiceMenu_TradeCenter_Colosseum[] = {
    { gText_TradeCenter },
    { gText_Colosseum },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Link_Wireless[] = {
    { gText_GameLinkCable },
    { gText_Wireless },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_RocketHideoutElevator[] = {
    { gText_B1F },
    { gText_B2F },
    { gText_B4F },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_LinkedDirectUnion[] = {
    { gText_LinkedGamePlay },
    { gText_DirectCorner },
    { gText_UnionRoom },
    { gOtherText_Quit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Island23[] = {
    { gText_TwoIsland },
    { gText_ThreeIsland },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Island13[] = {
    { gText_OneIsland },
    { gText_ThreeIsland },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Island12[] = {
    { gText_OneIsland },
    { gText_TwoIsland },
    { gOtherText_Exit }
};

// Unused
static const struct MenuAction sScriptMultiChoiceMenu_TradeColosseumCrush[] = {
    { gText_TradeCenter },
    { gText_Colosseum_2 },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_48[] = {
    { gText_Dummy_8417E66 },
    { gText_Dummy_8417E67 },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_TradeColosseum_2[] = {
    { gText_TradeCenter },
    { gText_Colosseum_2 },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_PokejumpDodrio[] = {
    { gText_PokemonJump },
    { gText_DodrioBerryPicking },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Mushrooms[] = {
    { gText_2Tinymushrooms },
    { gText_1BigMushroom }
};

static const struct MenuAction sScriptMultiChoiceMenu_TradeColosseumBlank[] = {
    { gText_TradeCenter },
    { gText_Colosseum_2 },
    { gText_Dummy_8417E71 },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeviiNavel[] = {
    { gText_SeviiIslands },
    { gText_NavelRock },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeviiBirth[] = {
    { gText_SeviiIslands },
    { gText_BirthIsland },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeviiNavelBirth[] = {
    { gText_SeviiIslands },
    { gText_NavelRock },
    { gText_BirthIsland },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Seagallop123[] = {
    { gText_OneIsland },
    { gText_TwoIsland },
    { gText_ThreeIsland },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeagallopV23[] = {
    { gText_Vermilion },
    { gText_TwoIsland },
    { gText_ThreeIsland },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeagallopV13[] = {
    { gText_Vermilion },
    { gText_OneIsland },
    { gText_ThreeIsland },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeagallopV12[] = {
    { gText_Vermilion },
    { gText_OneIsland },
    { gText_TwoIsland },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_SeagallopVermilion[] = {
    { gText_Vermilion },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_62[] = {
    { gText_Dummy_8417F68 },
    { gText_Dummy_8417F67 },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_JoinOrLead[] = {
    { gText_JoinGroup },
    { gText_BecomeLeader },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_TrainerTowerMode[] = {
    { gOtherText_Single },
    { gOtherText_Double },
    { gOtherText_Knockout },
    { gOtherText_Mixed },
    { gOtherText_Exit }
};

static const struct MenuAction sScriptMultiChoiceMenu_Exit[] = {
    { gOtherText_Exit }
};

const struct MultichoiceListStruct gScriptMultiChoiceMenus[] = {
    { sScriptMultiChoiceMenu_YesNo,  NELEMS(sScriptMultiChoiceMenu_YesNo)  },
    { sScriptMultiChoiceMenu_Eeveelutions,  NELEMS(sScriptMultiChoiceMenu_Eeveelutions)  },
    { sScriptMultiChoiceMenu_TrainerCardIconTint,  NELEMS(sScriptMultiChoiceMenu_TrainerCardIconTint)  },
    { sScriptMultiChoiceMenu_HOF_Quit,  NELEMS(sScriptMultiChoiceMenu_HOF_Quit)  },
    { sScriptMultiChoiceMenu_Eggs_Quit,  NELEMS(sScriptMultiChoiceMenu_Eggs_Quit)  },
    { sScriptMultiChoiceMenu_Victories_Quit,  NELEMS(sScriptMultiChoiceMenu_Victories_Quit)  },
    { sScriptMultiChoiceMenu_HOF_Eggs_Quit,  NELEMS(sScriptMultiChoiceMenu_HOF_Eggs_Quit)  },
    { sScriptMultiChoiceMenu_HOF_Victories_Quit,  NELEMS(sScriptMultiChoiceMenu_HOF_Victories_Quit)  },
    { sScriptMultiChoiceMenu_Eggs_Victories_Quit,  NELEMS(sScriptMultiChoiceMenu_Eggs_Victories_Quit)  },
    { sScriptMultiChoiceMenu_HOF_Eggs_Victories_Quit,  NELEMS(sScriptMultiChoiceMenu_HOF_Eggs_Victories_Quit)  },
    { sScriptMultiChoiceMenu_Exit, NELEMS(sScriptMultiChoiceMenu_Exit) },
    { sScriptMultiChoiceMenu_Exit, NELEMS(sScriptMultiChoiceMenu_Exit) },
    { sScriptMultiChoiceMenu_Exit, NELEMS(sScriptMultiChoiceMenu_Exit) },
    { sScriptMultiChoiceMenu_BikeShop, NELEMS(sScriptMultiChoiceMenu_BikeShop) },
    { sScriptMultiChoiceMenu_GameCornerPokemonPrizes, NELEMS(sScriptMultiChoiceMenu_GameCornerPokemonPrizes) },
    { sScriptMultiChoiceMenu_TrainerSchoolWhiteboard, NELEMS(sScriptMultiChoiceMenu_TrainerSchoolWhiteboard) },
    { sScriptMultiChoiceMenu_YesNoInfo, NELEMS(sScriptMultiChoiceMenu_YesNoInfo) },
    { sScriptMultiChoiceMenu_SingleDoubleMultiInfoExit, NELEMS(sScriptMultiChoiceMenu_SingleDoubleMultiInfoExit) },
    { sScriptMultiChoiceMenu_YesNoInfo2, NELEMS(sScriptMultiChoiceMenu_YesNoInfo2) },
    { sScriptMultiChoiceMenu_ChallengeInfoExit, NELEMS(sScriptMultiChoiceMenu_ChallengeInfoExit) },
    { sScriptMultiChoiceMenu_RooftopB1F, NELEMS(sScriptMultiChoiceMenu_RooftopB1F) },
    { sScriptMultiChoiceMenu_Helix, NELEMS(sScriptMultiChoiceMenu_Helix) },
    { sScriptMultiChoiceMenu_Dome, NELEMS(sScriptMultiChoiceMenu_Dome) },
    { sScriptMultiChoiceMenu_Amber, NELEMS(sScriptMultiChoiceMenu_Amber) },
    { sScriptMultiChoiceMenu_HelixAmber, NELEMS(sScriptMultiChoiceMenu_HelixAmber) },
    { sScriptMultiChoiceMenu_DomeAmber, NELEMS(sScriptMultiChoiceMenu_DomeAmber) },
    { sScriptMultiChoiceMenu_CeladonVendingMachine, NELEMS(sScriptMultiChoiceMenu_CeladonVendingMachine) },
    { sScriptMultiChoiceMenu_GameCornerCoinPurchaseCounter, NELEMS(sScriptMultiChoiceMenu_GameCornerCoinPurchaseCounter) },
    { sScriptMultiChoiceMenu_Excellent_NotSoBad, NELEMS(sScriptMultiChoiceMenu_Excellent_NotSoBad) },
    { sScriptMultiChoiceMenu_RightLeft, NELEMS(sScriptMultiChoiceMenu_RightLeft) },
    { sScriptMultiChoiceMenu_GameCornerTMPrizes, NELEMS(sScriptMultiChoiceMenu_GameCornerTMPrizes) },
    { sScriptMultiChoiceMenu_DeptStoreElevator, NELEMS(sScriptMultiChoiceMenu_DeptStoreElevator) },
    { sScriptMultiChoiceMenu_ThirstyGirlFreshWater, NELEMS(sScriptMultiChoiceMenu_ThirstyGirlFreshWater) },
    { sScriptMultiChoiceMenu_ThirstyGirlSodaPop, NELEMS(sScriptMultiChoiceMenu_ThirstyGirlSodaPop) },
    { sScriptMultiChoiceMenu_ThirstyGirlFreshWaterSodaPop, NELEMS(sScriptMultiChoiceMenu_ThirstyGirlFreshWaterSodaPop) },
    { sScriptMultiChoiceMenu_ThirstyGirlLemonade, NELEMS(sScriptMultiChoiceMenu_ThirstyGirlLemonade) },
    { sScriptMultiChoiceMenu_ThirstyGirlFreshWaterLemonade, NELEMS(sScriptMultiChoiceMenu_ThirstyGirlFreshWaterLemonade) },
    { sScriptMultiChoiceMenu_ThirstyGirlSodaPopLemonade, NELEMS(sScriptMultiChoiceMenu_ThirstyGirlSodaPopLemonade) },
    { sScriptMultiChoiceMenu_ThirstyGirlFreshWaterSodaPopLemonade, NELEMS(sScriptMultiChoiceMenu_ThirstyGirlFreshWaterSodaPopLemonade) },
    { sScriptMultiChoiceMenu_TradeCenter_Colosseum, NELEMS(sScriptMultiChoiceMenu_TradeCenter_Colosseum) },
    { sScriptMultiChoiceMenu_Link_Wireless, NELEMS(sScriptMultiChoiceMenu_Link_Wireless) },
    { sScriptMultiChoiceMenu_GameCornerBattleItemPrizes, NELEMS(sScriptMultiChoiceMenu_GameCornerBattleItemPrizes) },
    { sScriptMultiChoiceMenu_RocketHideoutElevator, NELEMS(sScriptMultiChoiceMenu_RocketHideoutElevator) },
    { sScriptMultiChoiceMenu_LinkedDirectUnion, NELEMS(sScriptMultiChoiceMenu_LinkedDirectUnion) },
    { sScriptMultiChoiceMenu_Island23, NELEMS(sScriptMultiChoiceMenu_Island23) },
    { sScriptMultiChoiceMenu_Island13, NELEMS(sScriptMultiChoiceMenu_Island13) },
    { sScriptMultiChoiceMenu_Island12, NELEMS(sScriptMultiChoiceMenu_Island12) },
    { sScriptMultiChoiceMenu_TradeColosseumCrush, NELEMS(sScriptMultiChoiceMenu_TradeColosseumCrush) },
    { sScriptMultiChoiceMenu_48, NELEMS(sScriptMultiChoiceMenu_48) },
    { sScriptMultiChoiceMenu_PokejumpDodrio, NELEMS(sScriptMultiChoiceMenu_PokejumpDodrio) },
    { sScriptMultiChoiceMenu_TradeColosseum_2, NELEMS(sScriptMultiChoiceMenu_TradeColosseum_2) },
    { sScriptMultiChoiceMenu_Mushrooms, NELEMS(sScriptMultiChoiceMenu_Mushrooms) },
    { sScriptMultiChoiceMenu_TradeColosseumBlank, NELEMS(sScriptMultiChoiceMenu_TradeColosseumBlank) },
    { sScriptMultiChoiceMenu_SeviiNavel, NELEMS(sScriptMultiChoiceMenu_SeviiNavel) },
    { sScriptMultiChoiceMenu_SeviiBirth, NELEMS(sScriptMultiChoiceMenu_SeviiBirth) },
    { sScriptMultiChoiceMenu_SeviiNavelBirth, NELEMS(sScriptMultiChoiceMenu_SeviiNavelBirth) },
    { sScriptMultiChoiceMenu_Seagallop123, NELEMS(sScriptMultiChoiceMenu_Seagallop123) },
    { sScriptMultiChoiceMenu_SeagallopV23, NELEMS(sScriptMultiChoiceMenu_SeagallopV23) },
    { sScriptMultiChoiceMenu_SeagallopV13, NELEMS(sScriptMultiChoiceMenu_SeagallopV13) },
    { sScriptMultiChoiceMenu_SeagallopV12, NELEMS(sScriptMultiChoiceMenu_SeagallopV12) },
    { sScriptMultiChoiceMenu_SeagallopVermilion, NELEMS(sScriptMultiChoiceMenu_SeagallopVermilion) },
    { sScriptMultiChoiceMenu_62, NELEMS(sScriptMultiChoiceMenu_62) },
    { sScriptMultiChoiceMenu_JoinOrLead, NELEMS(sScriptMultiChoiceMenu_JoinOrLead) },
    { sScriptMultiChoiceMenu_TrainerTowerMode, NELEMS(sScriptMultiChoiceMenu_TrainerTowerMode) }
};

// From Cool to Berries goes unused
const u8 *const gStdStringPtrs[] = {
    [STDSTRING_COOL]             = gText_Cool,
    [STDSTRING_BEAUTY]           = gText_Beauty,
    [STDSTRING_CUTE]             = gText_Cute,
    [STDSTRING_SMART]            = gText_Smart,
    [STDSTRING_TOUGH]            = gText_Tough,
    [STDSTRING_COOL2]            = gText_Cool_2,
    [STDSTRING_BEAUTY2]          = gText_Beauty_2,
    [STDSTRING_CUTE2]            = gText_Cute_2,
    [STDSTRING_SMART2]           = gText_Smart_2,
    [STDSTRING_TOUGH2]           = gText_Tough_2,
    [STDSTRING_ITEMS]            = gText_Items,
    [STDSTRING_KEY_ITEMS]        = gText_KeyItems,
    [STDSTRING_POKEBALLS]        = gText_PokeBalls,
    [STDSTRING_TMHMS]            = gText_TMsAndHMs,
    [STDSTRING_BERRIES]          = gText_Berries,
    [STDSTRING_BOULDER_BADGE]    = gText_Boulderbadge,
    [STDSTRING_CASCADE_BADGE]    = gText_Cascadebadge,
    [STDSTRING_THUNDER_BADGE]    = gText_Thunderbadge,
    [STDSTRING_RAINBOW_BADGE]    = gText_Rainbowbadge,
    [STDSTRING_SOUL_BADGE]       = gText_Soulbadge,
    [STDSTRING_MARSH_BADGE]      = gText_Marshbadge,
    [STDSTRING_VOLCANO_BADGE]    = gText_Volcanobadge,
    [STDSTRING_EARTH_BADGE]      = gText_Earthbadge,
    [STDSTRING_COINS]            = gText_Coins_2,
    [STDSTRING_ITEMS_POCKET]     = gText_ItemsPocket,
    [STDSTRING_KEY_ITEMS_POCKET] = gText_KeyItemsPocket,
    [STDSTRING_POKEBALLS_POCKET] = gText_PokeBallsPocket,
    [STDSTRING_TM_CASE]          = gText_TmCase,
    [STDSTRING_BERRY_POUCH]      = gText_BerryPouch_2
};

static const u8 *const sDescriptionPtrs_CableClub_TradeBattleCancel[] = {
    CableClub_Text_TradeMonsUsingLinkCable,
    CableClub_Text_BattleUsingLinkCable,
    CableClub_Text_CancelSelectedItem
};

static const u8 *const sDescriptionPtrs_WirelessCenter_TradeBattleCrushCancel[] = {
    CableClub_Text_YouMayTradeHere,
    CableClub_Text_YouMayBattleHere,
    CableClub_Text_CanMakeBerryPowder,
    CableClub_Text_CancelSelectedItem
};

static const u8 *const sDescriptionPtrs_WirelessCenter_TradeBattleCancel[] = {
    CableClub_Text_YouMayTradeHere,
    CableClub_Text_YouMayBattleHere,
    CableClub_Text_CancelSelectedItem
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
    .tileTag = 7000,
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
    {sMuseumKabutopsSprTiles, 0x800, 7000},
    {}
};

static const struct SpriteSheet sMuseumAerodactylSprSheets[] = {
    {sMuseumAerodactylSprTiles, 0x800, 7000},
    {}
};

static const u8 *const sSeagallopDestStrings[] = {
    gText_Vermilion,
    gText_OneIsland,
    gText_TwoIsland,
    gText_ThreeIsland,
    gText_FourIsland,
    gText_FiveIsland,
    gText_SixIsland,
    gText_SevenIsland
};

static u16 GetStringTilesWide(const u8 *str)
{
    return (GetStringWidth(1, str, 0) + 7) / 8;
}

static u8 GetMenuWidthFromList(const struct MenuAction * items, u8 count)
{
    u16 i;
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

static u8 CreateMultichoiceWindow(u8 bg, u8 x, u8 y, u8 width, u8 height, u16 baseBlock, u8 palNum)
{
	struct WindowTemplate template = SetWindowTemplateFields(bg, x + 1, y + 1, width, height, palNum, baseBlock);
	u8 windowId = AddWindow(&template);
	
    PutWindowTilemap(windowId);
	
    return windowId;
}

static void DestroyMultichoiceWindow(u8 windowId)
{
	ClearWindowTilemap(windowId);
    ClearStdWindowAndFrameToTransparent(windowId, TRUE);
    RemoveWindow(windowId);
}

bool8 ScriptMenu_Multichoice(u8 x, u8 y, u8 mcId, bool8 ignoreBPress, u8 defaultOpt, u8 perRowItems)
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

static u8 CreateMultichoiceInputTask(bool8 ignoreBPress, u8 windowId)
{
	u8 taskId = CreateTask(Task_MultichoiceMenu_HandleInput, 80);
	
	gTasks[taskId].tIgnoreBPress = ignoreBPress;
	gTasks[taskId].tWindowId = windowId;
	
	return taskId;
}

static void InitDefaultMultichoiceOnLeftTop(const struct MenuAction * items, u8 count)
{
	InitMultichoice(items, count, 0, 0, 0, FALSE, 0, 1, MULTICHOICE_DEFAULT_BASE_BLOCK, 15);
}

u8 InitMultichoice(const struct MenuAction * items, u8 count, u8 bg, u8 x, u8 y, bool8 ignoreBPress, u8 defaultOpt, u8 perRowItems, u16 baseBlock, u8 palNum)
{
	u8 width, rowCount, windowId, taskId = 0xFF;
	
	gSpecialVar_Result = SCR_MENU_UNSET;
	
	if (!QuestLog_SchedulePlaybackCB(QLPlaybackCB_DestroyScriptMenuMonPicSprites))
	{
		width = GetMenuWidthFromList(items, count) + 1;
		rowCount = count / perRowItems;
		
		windowId = CreateMultichoiceWindow(bg, x, y, width * perRowItems, rowCount * 2, baseBlock, palNum);
		SetStdWindowBorderStyle(windowId, FALSE);
		
		taskId = CreateMultichoiceInputTask(ignoreBPress, windowId);

		MultichoiceGrid_PrintItems(windowId, 1, width * 8, 16, perRowItems, rowCount, items);
		MultichoiceGrid_InitCursor(windowId, 1, 0, 1, width * 8, perRowItems, rowCount, defaultOpt);
			
		ScheduleBgCopyTilemapToVram(bg);
	}
	return taskId;
}

static void Task_MultichoiceMenu_HandleInput(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	s8 input = Menu_ProcessInputGridLayout();
	
	switch (input)
	{
		case -2:
		    return;
		case -1:
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
	u8 nItems = 0;
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
		menuItems[nItems++].text = gText_HallOfFame_2;
	
	// Log off
	menuItems[nItems++].text = gText_LogOff;
	
	InitDefaultMultichoiceOnLeftTop(menuItems, nItems);
}

bool8 CreatePCMenu(void)
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
	sub_80F7768(0, TRUE);
	AddTextPrinterParameterized2(0, 2, Text_AccessWhichPC, 0, NULL, 2, 1, 3);
}

void DrawSeagallopDestinationMenu(void)
{
	// 8004 = Starting location
    // 8005 = Page (0: Verm, One, Two, Three, Four, Other, Exit; 1: Four, Five, Six, Seven, Other, Exit)
	u8 i, windowId, defaultOpt, cursorWidth, nItems = 5, y = 0;
	
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
	
	for (i = 0; i < nItems - 2; i++)
	{
		if (defaultOpt != gSpecialVar_0x8004)
			AddTextPrinterParameterized(windowId, 2, sSeagallopDestStrings[defaultOpt], cursorWidth, i * 16 + 2, 0xFF, NULL);
		else
			--i;
		
		++defaultOpt;
		
		if (defaultOpt == SEAGALLOP_CINNABAR_ISLAND)
			defaultOpt = SEAGALLOP_VERMILION_CITY;
	}
	AddTextPrinterParameterized(windowId, 2, gText_Other, cursorWidth, i * 16 + 2, 0xFF, NULL);
	AddTextPrinterParameterized(windowId, 2, gOtherText_Exit, cursorWidth, (i + 1) * 16 + 2, 0xFF, NULL);
	Menu_InitCursor(windowId, 2, 0, 2, 16, nItems, 0);
	CreateMultichoiceInputTask(FALSE, windowId);
	ScheduleBgCopyTilemapToVram(0);
}

u16 GetSelectedSeagallopDestination(void)
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
	u8 i, count;
	u16 repelItems[] = {ITEM_REPEL, ITEM_SUPER_REPEL, ITEM_MAX_REPEL};
	struct MenuAction menuItems[3]; // max of items
	
	gSpecialVar_Result = FALSE;
	
	for (i = 0, count = 0; i < NELEMS(repelItems); i++)
	{
		if (CheckBagHasItem(repelItems[i], 1))
		{
			VarSet(VAR_0x8004 + count, repelItems[i]);
			menuItems[count++].text = ItemId_GetName(repelItems[i]);
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

bool8 ScriptMenu_YesNo(u8 unused, u8 stuff)
{
    if (!FuncIsActiveTask(Task_YesNoMenu_HandleInput))
	{
		gSpecialVar_Result = SCR_MENU_UNSET;
		
		if (!QuestLog_SchedulePlaybackCB(QLPlaybackCB_DestroyScriptMenuMonPicSprites))
		{
			DisplayYesNoMenuDefaultYes();
			CreateTask(Task_YesNoMenu_HandleInput, 80);
		}
		return TRUE;
	}
	return FALSE;
}

static void Task_YesNoMenu_HandleInput(u8 taskId)
{
	if (gTasks[taskId].data[2] < 5)
		++gTasks[taskId].data[2];
	else
	{
		switch (Menu_ProcessInputNoWrapClearOnChoose())
		{
			case -2:
			    return;
			case -1:
			case 1:
			    PlaySE(SE_SELECT);
				gSpecialVar_Result = FALSE;
				break;
			case 0:
			    gSpecialVar_Result = TRUE;
				break;
		}
		DestroyTask(taskId);
		EnableBothScriptContexts();
	}
}

static u8 CreateMonSprite_PicBox(u16 species, s16 x, s16 y)
{
    return CreateMonPicSprite(species, 0, 0x8000, TRUE, 8 * x + 40, 8 * y + 40, 0, gMonPaletteTable[species].tag);
}

static u8 CreateMenuMonPic(u16 species, u8 x, u8 y)
{
	u16 reg1 = GetGpuReg(REG_OFFSET_DISPCNT), reg2 = GetGpuReg(REG_OFFSET_WINOUT);
	u8 spriteId2 = MAX_SPRITES, spriteId = CreateMonSprite_PicBox(species, x, y);
	
	if (Overworld_GetFlashLevel() > 0)
	{
		SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJWIN_ON);
		SetGpuRegBits(REG_OFFSET_WINOUT, WINOUT_WINOBJ_OBJ);
		
		spriteId2 = CreateMonSprite_PicBox(species, x, y);
		
		if (spriteId2 != MAX_SPRITES)
		{
			gSprites[spriteId2].callback = SpriteCallbackDummy;
			gSprites[spriteId2].oam.priority = 0;
			gSprites[spriteId2].oam.objMode = ST_OAM_OBJ_WINDOW;
		}
	}
	gSprites[spriteId].callback = SpriteCallbackDummy;
	gSprites[spriteId].oam.priority = 0;
	gSprites[spriteId].data[0] = reg1;
	gSprites[spriteId].data[1] = reg2;
	gSprites[spriteId].data[2] = spriteId2;
	
	return spriteId;
}

static void DestroyPicboxMonPic(bool8 isMuseumPic, u8 spriteId)
{
	struct Sprite *sprite = &gSprites[spriteId];
	
	if (isMuseumPic)
	{
		DestroySprite(sprite);
		FreeSpriteTilesByTag(TAG_MUSEUM_FOSSIL_PIC);
	}
	else
	{
		if (sprite->data[2] != MAX_SPRITES)
		{
			SetGpuReg(REG_OFFSET_DISPCNT, sprite->data[0]);
			SetGpuReg(REG_OFFSET_WINOUT, sprite->data[1]);
			
			FreeResourcesAndDestroySprite(&gSprites[sprite->data[2]], sprite->data[2]);
		}
		FreeResourcesAndDestroySprite(sprite, spriteId);
	}
}

#define tWindowId  data[0]
#define tSpriteId  data[1]
#define tState     data[2]
#define tMuseumPic data[3]

void QLPlaybackCB_DestroyScriptMenuMonPicSprites(void)
{
	u8 taskId = FindTaskIdByFunc(Task_ScriptShowMonPic);
	
	ScriptContext1_SetupScript(EventScript_ReleaseEnd);
	
	if (taskId != 0xFF && gTasks[taskId].tState < 2)
		DestroyPicboxMonPic(gTasks[taskId].tMuseumPic, gTasks[taskId].tSpriteId);
}

bool8 ScriptMenu_ShowPokemonPic(u16 species, u8 x, u8 y)
{
	u8 spriteId;
	s16 *data;
	
	if (QuestLog_SchedulePlaybackCB(QLPlaybackCB_DestroyScriptMenuMonPicSprites))
		return TRUE;
	else if (FindTaskIdByFunc(Task_ScriptShowMonPic) != 0xFF)
		return FALSE;
	else
	{
		spriteId = CreateMenuMonPic(species, x, y);
		
		data = gTasks[CreateTask(Task_ScriptShowMonPic, 80)].data;
		tWindowId = CreateMultichoiceWindow(0, x, y, 8, 8, MULTICHOICE_DEFAULT_BASE_BLOCK, 15);
		tSpriteId = spriteId;
		tState = 0;
		tMuseumPic = FALSE;
		
		SetStdWindowBorderStyle(tWindowId, TRUE);
		ScheduleBgCopyTilemapToVram(0);
		
		return TRUE;
	}
}

static void Task_ScriptShowMonPic(u8 taskId)
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
		    DestroyPicboxMonPic(tMuseumPic, tSpriteId);
		    ++tState;
			break;
		case 3:
		    DestroyMultichoiceWindow(tWindowId);
			DestroyTask(taskId);
			break;
	}
}

bool8 (*ScriptMenu_GetPicboxWaitFunc(void))(void)
{
	u8 taskId = FindTaskIdByFunc(Task_ScriptShowMonPic);
	
	if (taskId == 0xFF)
		return NULL;
	
	++gTasks[taskId].tState;
	
	return PicboxWaitFunc;
}

static bool8 PicboxWaitFunc(void)
{
	return (FindTaskIdByFunc(Task_ScriptShowMonPic) == 0xFF);
}

void PicboxCancel(void)
{
	u8 taskId = FindTaskIdByFunc(Task_ScriptShowMonPic);
	s16 *data;
	
	if (taskId != 0xFF)
	{
		data = gTasks[taskId].data;
		
		switch (tState)
		{
			case 0:
			case 1:
			case 2:
			    DestroyPicboxMonPic(tMuseumPic, tSpriteId);
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
	u8 taskId = FindTaskIdByFunc(Task_ScriptShowMonPic);
	
	if (taskId != 0xFF)
		DestroyPicboxMonPic(FALSE, gTasks[taskId].tSpriteId);
}

void UpdatePokemonSpeciesOnPicbox(u16 species, u8 x, u8 y)
{
	u8 taskId = FindTaskIdByFunc(Task_ScriptShowMonPic);
	
	if (taskId != 0xFF)
		gTasks[taskId].tSpriteId = CreateMenuMonPic(species, x, y);
}

bool8 OpenMuseumFossilPic(void)
{
	u8 spriteId;
	s16 *data;
	
	if (QuestLog_SchedulePlaybackCB(QLPlaybackCB_DestroyScriptMenuMonPicSprites))
		return TRUE;
	else if (FindTaskIdByFunc(Task_ScriptShowMonPic) != 0xFF)
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
		
		data = gTasks[CreateTask(Task_ScriptShowMonPic, 80)].data;
		tWindowId = CreateMultichoiceWindow(0, gSpecialVar_0x8005, gSpecialVar_0x8006, 8, 8, MULTICHOICE_DEFAULT_BASE_BLOCK, 15);
		tSpriteId = spriteId;
		tState = 0;
		tMuseumPic = TRUE;
		
		SetStdWindowBorderStyle(tWindowId, TRUE);
		ScheduleBgCopyTilemapToVram(0);
		
		return TRUE;
	}
}

bool8 CloseMuseumFossilPic(void)
{
	u8 taskId = FindTaskIdByFunc(Task_ScriptShowMonPic);
	
    if (taskId == 0xFF)
        return FALSE;
	
    ++gTasks[taskId].tState;
	
    return TRUE;
}

#undef tWindowId
#undef tSpriteId
#undef tState
#undef tMuseumPic

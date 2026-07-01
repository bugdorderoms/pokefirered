#include "global.h"
#include "constants/union_room.h"

ALIGNED(4) const u8 gUnionRoomActivity_Blank[] = _("");
ALIGNED(4) const u8 gUnknown_84571B4[] = _("{ID}");
ALIGNED(4) const u8 gUnknown_8457234[] = _("{STR_VAR_1}! Awaiting\ncommunication from another player.");
ALIGNED(4) const u8 gUnknown_8457264[] = _("{STR_VAR_1}! Awaiting link!\nPress START when everyone's ready.");

const u8 gText_1PlayerNeeded[] = _("1 player\nneeded.");
const u8 gText_2PlayersNeeded[] = _("2 players\nneeded.");
const u8 gText_3PlayersNeeded[] = _("3 players\nneeded.");
const u8 gText_2PlayerMode[] = _("2-Player\nMode");
const u8 gText_3PlayerMode[] = _("3-Player\nMode");
const u8 gText_4PlayerMode[] = _("4-Player\nMode");
const u8 gText_5PlayerMode[] = _("5-Player\nMode");

const u8 *const gUnknown_845742C[][5] = {
    {
        gText_1PlayerNeeded,
        gText_2PlayerMode
    }, {
        gText_3PlayersNeeded,
        gText_2PlayersNeeded,
        gText_1PlayerNeeded,
        gText_4PlayerMode
    }, {
        gText_1PlayerNeeded,
        gText_2PlayerMode,
        gText_3PlayerMode,
        gText_4PlayerMode,
        gText_5PlayerMode
    }, {
        gText_2PlayersNeeded,
        gText_1PlayerNeeded,
        gText_3PlayerMode,
        gText_4PlayerMode,
        gText_5PlayerMode
    }
};

ALIGNED(4) const u8 gUnknown_845747C[] = _("{B_BUTTON}Cancel");
ALIGNED(4) const u8 gUnknown_84574A0[] = _("{STR_VAR_2} contacted you for\n{STR_VAR_1}. Accept?");
ALIGNED(4) const u8 gUnknown_84574C4[] = _("{STR_VAR_2} contacted you.\nWill you share {STR_VAR_1}?");
ALIGNED(4) const u8 gUnknown_84574EC[] = _("{STR_VAR_2} contacted you.\nAdd to the members?");
ALIGNED(4) const u8 gUnknown_8457514[] = _("{STR_VAR_1}!\nAre these members OK?");
ALIGNED(4) const u8 gUnknown_8457530[] = _("Cancel {STR_VAR_1} mode\nwith these members?");
ALIGNED(4) const u8 gUnknown_8457554[] = _("An “OK” was sent\nto {STR_VAR_1}.");

ALIGNED(4) const u8 gUnknown_845756C[] = _("The other Trainer doesn't appear\nto be available now…\p");
ALIGNED(4) const u8 gUnknown_84575A4[] = _("You can't transmit with a Trainer\nwho is too far away.\p");
ALIGNED(4) const u8 gUnknown_84575DC[] = _("The other Trainer(s) is/are not\nready yet.\p");

const u8 *const gUnknown_8457608[] = {
    gUnknown_84575A4,
    gUnknown_84575DC
};

ALIGNED(4) const u8 gUnknown_8457610[] = _("The {STR_VAR_1} mode with\nthese members will be canceled.{PAUSE 90}");
ALIGNED(4) const u8 gUnknown_8457644[] = _("There is a member who can no\nlonger remain available.\p");

const u8 *const gUnknown_845767C[] = {
    gUnknown_845756C,
    gUnknown_8457644
};

ALIGNED(4) const u8 gUnknown_8457684[] = _("The other Trainer appears\nunavailable…\p");
ALIGNED(4) const u8 gUnknown_84576AC[] = _("{STR_VAR_1} sent back an “OK”!");
ALIGNED(4) const u8 gUnknown_84576C4[] = _("{STR_VAR_1} OK'd your registration as\na member.");
ALIGNED(4) const u8 gUnknown_84576EC[] = _("{STR_VAR_1} replied, “No…”\p");
ALIGNED(4) const u8 gUnknown_8457700[] = _("{STR_VAR_1}!\nAwaiting other members!");
ALIGNED(4) const u8 gUnknown_845771C[] = _("Quit being a member?");
ALIGNED(4) const u8 gUnknown_8457734[] = _("You stopped being a member.\p");

const u8 *const gUnknown_8457754[] = {
    NULL,
    gUnknown_8457644,
    gUnknown_8457684,
    NULL,
    NULL,
    NULL,
    gUnknown_84576EC,
    NULL,
    NULL,
    gUnknown_8457734
};

ALIGNED(4) const u8 gUnknown_8457860[] = _("Do you want the {STR_VAR_2}\nmode?");
ALIGNED(4) const u8 gUnknown_845787C[] = _("Do you want the {STR_VAR_2}\nmode?");

const u8 *const gUnknown_8457898[] = {
    gUnknown_8457860,
    gUnknown_845787C
};

ALIGNED(4) const u8 gUnknown_84578BC[] = _("Awaiting {STR_VAR_1}'s response about\nthe trade…");

ALIGNED(4) const u8 gUnknown_84578E4[] = _("Communicating{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.\n{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.");
ALIGNED(4) const u8 gUnknown_8457950[] = _("Communicating with {STR_VAR_1}{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.\n{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.");
ALIGNED(4) const u8 gUnknown_84579BC[] = _("Please wait a while{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.\n{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.");

const u8 *const gURText_PleaseWaitMsgs[] = {
    gUnknown_84578E4,
    gUnknown_8457950,
    gUnknown_84579BC
};

ALIGNED(4) const u8 gUnknown_8457A40[] = _("Hiya! Is there something that you\nwanted to do?");
ALIGNED(4) const u8 gUnknown_8457A70[] = _("Hello!\nWould you like to do something?");
ALIGNED(4) const u8 gUnknown_8457A98[] = _("{STR_VAR_1}: Hiya, we meet again!\nWhat are you up for this time?");
ALIGNED(4) const u8 gUnknown_8457AD0[] = _("{STR_VAR_1}: Oh! {PLAYER}, hello!\nWould you like to do something?");

const u8 *const gURText_FriendPromptsForActivity[][2] = {
    {
        gUnknown_8457A40,
        gUnknown_8457A70
    }, {
        gUnknown_8457A98,
        gUnknown_8457AD0
    }
};

ALIGNED(4) const u8 gUnknown_8457B14[] = _("Want to do something?");
ALIGNED(4) const u8 gUnknown_8457B2C[] = _("Would you like to do something?");
ALIGNED(4) const u8 gUnknown_8457B4C[] = _("{STR_VAR_1}: What would you like to\ndo now?");

const u8 *const gUnknown_8457B80[] = {
    gUnknown_8457B14,
    gUnknown_8457B2C,
    gUnknown_8457B4C,
    gUnknown_8457B4C
};

ALIGNED(4) const u8 gUnknown_8457B90[] = _("Somebody has contacted you.{PAUSE 60}");
ALIGNED(4) const u8 gUnknown_8457BA0[] = _("{STR_VAR_1} has contacted you.{PAUSE 60}");

const u8 *const gUnknown_8457BCC[] = {
    gUnknown_8457B90,
    gUnknown_8457BA0
};

ALIGNED(4) const u8 gUnknown_8457BD4[] = _("Awaiting a response from\nthe other Trainer…");
ALIGNED(4) const u8 gUnknown_8457C00[] = _("Awaiting a response from\n{STR_VAR_1}…");

const u8 *const gUnknown_8457C20[] = {
    gUnknown_8457BD4,
    gUnknown_8457C00
};

ALIGNED(4) const u8 gUnknown_8457C48[] = _("The other Trainer showed\nyou their Trainer Card.\pWould you like to show your\nTrainer Card?");
ALIGNED(4) const u8 gUnknown_8457CA4[] = _("The other Trainer challenges you\nto battle.\pWill you accept the battle\nchallenge?");
ALIGNED(4) const u8 gUnknown_8457CF8[] = _("The other Trainer invites you\nto chat.\pWill you accept the chat\ninvitation?");
ALIGNED(4) const u8 gUnknown_8457D44[] = _("There is an offer to trade your\nregistered Lv. {DYNAMIC 0} {DYNAMIC 1}\pin exchange for a\nLv. {DYNAMIC 2} {DYNAMIC 3}.\pWill you accept this trade\noffer?");
ALIGNED(4) const u8 gUnknown_8457DB8[] = _("There is an offer to trade your\nregistered Egg.\lWill you accept this trade offer?");
ALIGNED(4) const u8 gUnknown_8457E0C[] = _("The chat has been dropped.\p");
ALIGNED(4) const u8 gUnknown_8457E28[] = _("You declined the offer.\p");
ALIGNED(4) const u8 gUnknown_8457E60[] = _("The chat was ended.\p");

const u8 *const gUnknown_8457E78[] = {
    gUnknown_8457C48,
    gUnknown_8457CA4,
    gUnknown_8457CF8,
    gUnknown_8457D44
};

ALIGNED(4) const u8 gUnknown_8457E88[] = _("Oh, hey! We're in a chat right now.\nWant to join us?");
ALIGNED(4) const u8 gUnknown_8457EC0[] = _("{STR_VAR_1}: Hey, {PLAYER}!\nWe're having a chat right now.\lWant to join us?");
ALIGNED(4) const u8 gUnknown_8457F00[] = _("Oh, hi! We're having a chat now.\nWould you like to join us?");
ALIGNED(4) const u8 gUnknown_8457F3C[] = _("{STR_VAR_1}: Oh, hi, {PLAYER}!\nWe're having a chat now.\lWould you like to join us?");

const u8 *const gUnknown_8457F80[][2] = {
    {
        gUnknown_8457E88,
        gUnknown_8457F00
    }, {
        gUnknown_8457EC0,
        gUnknown_8457F3C
    }
};

ALIGNED(4) const u8 gUnknown_8457F90[] = _("……\nThe Trainer appears to be busy…\p");

ALIGNED(4) const u8 gUnknown_8457FB4[] = _("A battle, huh?\nAll right, just give me some time.");
ALIGNED(4) const u8 gUnknown_8457FE8[] = _("You want to chat, huh?\nSure, just wait a little.");
ALIGNED(4) const u8 gUnknown_845801C[] = _("Sure thing! As my “Greetings,”\nhere's my Trainer Card.");
ALIGNED(4) const u8 gUnknown_8458054[] = _("A battle? Of course, but I need\ntime to get ready.");
ALIGNED(4) const u8 gUnknown_8458088[] = _("Did you want to chat?\nOkay, but please wait a moment.");
ALIGNED(4) const u8 gUnknown_84580C0[] = _("As my introduction, I'll show you\nmy Trainer Card.");

const u8 *const gResponseToURoomActivityInviteStringPtrs[][4] = {
    {
        gUnknown_8457FB4,
        gUnknown_8457FE8,
        NULL,
        gUnknown_845801C
    }, {
        gUnknown_8458054,
        gUnknown_8458088,
        NULL,
        gUnknown_84580C0
    }
};

ALIGNED(4) const u8 gUnknown_845812C[] = _("Thanks for waiting!\nLet's get our battle started!{PAUSE 60}");
ALIGNED(4) const u8 gUnknown_8458164[] = _("All right!\nLet's chat!{PAUSE 60}");
ALIGNED(4) const u8 gUnknown_8458180[] = _("Sorry I made you wait!\nLet's get started!{PAUSE 60}");
ALIGNED(4) const u8 gUnknown_84581B0[] = _("Sorry I made you wait!\nLet's chat.{PAUSE 60}");
ALIGNED(4) const u8 gUnknown_84581D8[] = _("The trade will be started.{PAUSE 60}");
ALIGNED(4) const u8 gUnknown_84581F8[] = _("The battle will be started.{PAUSE 60}");
ALIGNED(4) const u8 gUnknown_8458218[] = _("Entering the chat…{PAUSE 60}");

const u8 *const gUnknown_8458230[][2][3] = {
    {
        {
            gUnknown_84581F8,
            gUnknown_8458218,
            gUnknown_84581D8
        }, {
            gUnknown_84581F8,
            gUnknown_8458218,
            gUnknown_84581D8
        }
    }, {
        {
            gUnknown_845812C,
            gUnknown_8458164,
            gUnknown_84581D8
        }, {
            gUnknown_8458180,
            gUnknown_84581B0,
            gUnknown_84581D8
        }
    }
};

ALIGNED(4) const u8 gUnknown_8458260[] = _("Sorry! My Pokémon don't seem to\nbe feeling too well right now.\lLet me battle you another time.\p");
ALIGNED(4) const u8 gUnknown_84582C0[] = _("I'm terribly sorry, but my Pokémon\naren't feeling well…\pLet's battle another time.\p");

const u8 *const gUnknown_8458314[] = {
    gUnknown_8458260,
    gUnknown_84582C0
};

ALIGNED(4) const u8 gUnknown_845831C[] = _("Huh? My Trainer Card…\nWhere'd it go now?\lSorry! I'll show you another time!\p");
ALIGNED(4) const u8 gUnknown_845836C[] = _("Oh? Now where did I put my\nTrainer Card?…\lSorry! I'll show you later!\p");

const u8 *const gUnknown_84583B4[] = {
    gUnknown_845831C,
    gUnknown_845836C
};

ALIGNED(4) const u8 gUnknown_84583BC[] = _("If you want to do something with\nme, just give me a shout!\p");
ALIGNED(4) const u8 gUnknown_84583F8[] = _("If you want to do something with\nme, don't be shy.\p");

const u8 *const gUnknown_845842C[] = {
    gUnknown_84583BC,
    gUnknown_84583F8
};

ALIGNED(4) const u8 gUnknown_8458434[] = _("Whoops! Sorry, but I have to do\nsomething else.\lAnother time, okay?\p");
ALIGNED(4) const u8 gUnknown_845847C[] = _("If you want to battle, you need\ntwo Pokémon that are below\lLv. 30.\p");
ALIGNED(4) const u8 gUnknown_84584C0[] = _("For a battle, you need two\nPokémon that are below Lv. 30.\p");
ALIGNED(4) const u8 gUnknown_84584FC[] = _("Oh, all right.\nCome see me anytime, okay?\p");
ALIGNED(4) const u8 gUnknown_8458528[] = _("Oh…\nPlease come by anytime.\p");

const u8 *const gUnknown_8458548[] = {
    gUnknown_84584FC,
    gUnknown_8458528
};

ALIGNED(4) const u8 gUnknown_8458550[] = _("Oh, sorry!\nI just can't right this instant.\lLet's chat another time.\p");
ALIGNED(4) const u8 gUnknown_8458598[] = _("Oh, I'm sorry.\nI have too much to do right now.\lLet's chat some other time.\p");

const u8 *const gUnknown_84585E8[] = {
    gUnknown_8458550,
    gUnknown_8458598
};

ALIGNED(4) const u8 gUnknown_84585F0[] = _("Whoa!\nI can tell you're pretty tough!\p");
ALIGNED(4) const u8 gUnknown_8458618[] = _("You used that move?\nThat's good strategy!\p");
ALIGNED(4) const u8 gUnknown_8458644[] = _("Way to go!\nThat was an eye-opener!\p");
ALIGNED(4) const u8 gUnknown_8458668[] = _("Oh! How could you use that\nPokémon in that situation?\p");
ALIGNED(4) const u8 gUnknown_84586A0[] = _("That Pokémon…\nIt's been raised really well!\p");
ALIGNED(4) const u8 gUnknown_84586D0[] = _("That's it!\nThis is the right move now!\p");
ALIGNED(4) const u8 gUnknown_84586F8[] = _("That's awesome!\nYou can battle that way?\p");
ALIGNED(4) const u8 gUnknown_8458724[] = _("You have exquisite timing for\nswitching Pokémon!\p");

const u8 *const gUnknown_8458758[][4] = {
    {
        gUnknown_84585F0,
        gUnknown_8458618,
        gUnknown_8458644,
        gUnknown_8458668
    }, {
        gUnknown_84586A0,
        gUnknown_84586D0,
        gUnknown_84586F8,
        gUnknown_8458724
    }
};

ALIGNED(4) const u8 gUnknown_8458778[] = _("Oh, I see!\nThis is educational!\p");
ALIGNED(4) const u8 gUnknown_845879C[] = _("Don't say anything funny anymore!\nI'm sore from laughing!\p");
ALIGNED(4) const u8 gUnknown_84587D8[] = _("Oh?\nSomething like that happened.\p");
ALIGNED(4) const u8 gUnknown_84587FC[] = _("Hmhm… What?\nSo is this what you're saying?\p");
ALIGNED(4) const u8 gUnknown_8458828[] = _("Is that right?\nI didn't know that.\p");
ALIGNED(4) const u8 gUnknown_845884C[] = _("Ahaha!\nWhat is that about?\p");
ALIGNED(4) const u8 gUnknown_8458868[] = _("Yes, that's exactly it!\nThat's what I meant.\p");
ALIGNED(4) const u8 gUnknown_8458898[] = _("In other words…\nYes! That's right!\p");

const u8 *const gUnknown_84588BC[][4] = {
    {
        gUnknown_8458778,
        gUnknown_845879C,
        gUnknown_84587D8,
        gUnknown_84587FC
    }, {
        gUnknown_8458828,
        gUnknown_845884C,
        gUnknown_8458868,
        gUnknown_8458898
    }
};

ALIGNED(4) const u8 gUnknown_84588DC[] = _("I'm just showing my Trainer Card\nas my way of greeting.\p");
ALIGNED(4) const u8 gUnknown_8458918[] = _("I hope I get to know you better!\p");
ALIGNED(4) const u8 gUnknown_845893C[] = _("We're showing each other our\nTrainer Cards to get acquainted.\p");
ALIGNED(4) const u8 gUnknown_845897C[] = _("Glad to meet you.\nPlease don't be a stranger!\p");

const u8 *const gUnknown_84589AC[][2] = {
    {
        gUnknown_84588DC,
        gUnknown_8458918
    }, {
        gUnknown_845893C,
        gUnknown_845897C
    }
};

ALIGNED(4) const u8 gUnknown_84589BC[] = _("Yeahah!\nI really wanted this Pokémon!\p");
ALIGNED(4) const u8 gUnknown_84589E4[] = _("Finally, a trade got me that\nPokémon I'd wanted a long time.\p");
ALIGNED(4) const u8 gUnknown_8458A24[] = _("I'm trading Pokémon right now.\p");
ALIGNED(4) const u8 gUnknown_8458A44[] = _("I finally got that Pokémon I\nwanted in a trade!\p");

const u8 *const gUnknown_8458A78[][4] = {
    {
        gUnknown_84589BC,
        gUnknown_84589E4
    }, {
        gUnknown_8458A24,
        gUnknown_8458A44
    }
};

const u8 gUnknown_8458A98[] = _("{STR_VAR_1} checked the\nTrading Board.\p");
ALIGNED(4) const u8 gUnknown_8458AB8[] = _("Welcome to the Trading Board.\pYou may register your Pokémon\nand offer it up for a trade.\pWould you like to register one of\nyour Pokémon?");
ALIGNED(4) const u8 gUnknown_8458B44[] = _("This Trading Board is used for\noffering a Pokémon for a trade.\pAll you need to do is register a\nPokémon for a trade.\pAnother Trainer may offer a party\nPokémon in return for the trade.\pWe hope you will register Pokémon\nand trade them with many, many\lother Trainers.\pWould you like to register one of\nyour Pokémon?");

ALIGNED(4) const u8 gURText_PleaseChooseTypeOfMon[] = _("Please choose the type of Pokémon\nthat you would like in the trade.\n");
ALIGNED(4) const u8 gUnknown_8458D1C[] = _("Which of your party Pokémon will\nyou offer in trade?\p");
ALIGNED(4) const u8 gUnknown_8458D54[] = _("Registration has been canceled.\p");
ALIGNED(4) const u8 gUnknown_8458D78[] = _("Registration has been completed.\p");
ALIGNED(4) const u8 gUnknown_8458D9C[] = _("The trade has been canceled.\p");
ALIGNED(4) const u8 gUnknown_8458DBC[] = _("Cancel the registration of your\nLv. {STR_VAR_2} {STR_VAR_1}?");
ALIGNED(4) const u8 gUnknown_8458DE8[] = _("Cancel the registration of your\nEgg?");
ALIGNED(4) const u8 gUnknown_8458E10[] = _("The registration has been canceled.\p");
ALIGNED(4) const u8 gUnknown_8458E70[] = _("Would you like to ask {STR_VAR_1} to\nmake a trade?");
ALIGNED(4) const u8 gUnknown_8458ED0[] = _("You don't have a {STR_VAR_2}-type\nPokémon that {STR_VAR_1} wants.\p");
ALIGNED(4) const u8 gUnknown_8458F04[] = _("You don't have an Egg that\n{STR_VAR_1} wants.\p");
ALIGNED(4) const u8 gUnknown_8458F2C[] = _("{STR_VAR_1} can't make a trade for\nyour Pokémon right now.\p");
ALIGNED(4) const u8 gUnknown_8458F60[] = _("You can't make a trade for\n{STR_VAR_1}'s Pokémon right now.\p");

const u8 *const gUnknown_8458F94[] = {
    gUnknown_8458F2C,
    gUnknown_8458F60
};

ALIGNED(4) const u8 gUnknown_8458F9C[] = _("Your trade offer was rejected.\p");
ALIGNED(4) const u8 gText_EggTrade[] = _("Egg Trade");
ALIGNED(4) const u8 gUnknown_8458FC8[] = _("{DPAD_UPDOWN}Choose  {A_BUTTON}Join  {B_BUTTON}Cancel");

ALIGNED(4) const u8 gUnknown_8459000[] = _("Please choose a Trainer for\na Single Battle.");
ALIGNED(4) const u8 gUnknown_8459030[] = _("Please choose a Trainer for\na Double Battle.");
ALIGNED(4) const u8 gUnknown_8459060[] = _("Please choose the Leader\nfor a Multi Battle.");
ALIGNED(4) const u8 gUnknown_8459090[] = _("Please choose the Trainer to\ntrade with.");
ALIGNED(4) const u8 gUnknown_84590BC[] = _("Please choose the Trainer who is\nsharing Wonder Cards.");
ALIGNED(4) const u8 gUnknown_84590F4[] = _("Please choose the Trainer who is\nsharing Wonder News.");
ALIGNED(4) const u8 gUnknown_845912C[] = _("Jump with mini Pokémon!\nPlease choose the Leader.");
ALIGNED(4) const u8 gUnknown_8459160[] = _("Berry Crush!\nPlease choose the Leader.");
ALIGNED(4) const u8 gUnknown_8459188[] = _("Dodrio Berry-Picking!\nPlease choose the Leader.");

const u8 *const gUnknown_84591B8[] = {
    [LINK_GROUP_SINGLE_BATTLE] = gUnknown_8459000,
    [LINK_GROUP_DOUBLE_BATTLE] = gUnknown_8459030,
    [LINK_GROUP_MULTI_BATTLE]  = gUnknown_8459060,
    [LINK_GROUP_TRADE]         = gUnknown_8459090,
    [LINK_GROUP_POKEMON_JUMP]  = gUnknown_845912C,
    [LINK_GROUP_BERRY_CRUSH]   = gUnknown_8459160,
    [LINK_GROUP_BERRY_PICKING] = gUnknown_8459188,
    [LINK_GROUP_WONDER_CARD]   = gUnknown_84590BC,
    [LINK_GROUP_WONDER_NEWS]   = gUnknown_84590F4
};

ALIGNED(4) const u8 gUnknown_8459238[] = _("Awaiting {STR_VAR_1}'s response…");
ALIGNED(4) const u8 gUnknown_8459250[] = _("{STR_VAR_1} has been asked to register\nyou as a member. Please wait.");

ALIGNED(4) const u8 gUnknown_8459344[] = _("Battle");
ALIGNED(4) const u8 gUnknown_845934C[] = _("Chat");
ALIGNED(4) const u8 gUnknown_8459354[] = _("Greetings");

ALIGNED(4) const u8 gUnknown_8459378[] = _("Name{CLEAR_TO 0x3C}Wanted{CLEAR_TO 0x6E}Offer{CLEAR_TO 0xC6}Lv.");

ALIGNED(4) const u8 gUnknown_845944C[] = _("It's a Normal Card.");
ALIGNED(4) const u8 gUnknown_8459460[] = _("It's a Bronze Card!");
ALIGNED(4) const u8 gUnknown_8459474[] = _("It's a Copper Card!");
ALIGNED(4) const u8 gUnknown_8459488[] = _("It's a Silver Card!");
ALIGNED(4) const u8 gUnknown_845949C[] = _("It's a Gold Card!");

const u8 *const gUnknown_84594B0[] = {
    gUnknown_845944C,
    gUnknown_8459460,
    gUnknown_8459474,
    gUnknown_8459488,
    gUnknown_845949C
};

ALIGNED(4) const u8 gUnknown_84594C4[] = _("This is {SPECIAL_F7 0x00} {SPECIAL_F7 0x01}'s\nTrainer Card…\l{SPECIAL_F7 0x02}\pPokédex: {SPECIAL_F7 0x03}\nTime:    {SPECIAL_F7 0x04}:{SPECIAL_F7 0x05}\p");
ALIGNED(4) const u8 gUnknown_8459504[] = _("Battles: {SPECIAL_F7 0x00} Wins  {SPECIAL_F7 0x02} Losses\nTrades:  {SPECIAL_F7 0x03} times\p“{SPECIAL_F7 0x04} {SPECIAL_F7 0x05}\n{SPECIAL_F7 0x06} {SPECIAL_F7 0x07}”\p");

ALIGNED(4) const u8 gUnknown_8459544[] = _("{SPECIAL_F7 0x01}: Glad to have met you!{PAUSE 60}");
ALIGNED(4) const u8 gUnknown_8459564[] = _("{SPECIAL_F7 0x01}: Glad to meet you!{PAUSE 60}");

const u8 *const gUnknown_8459580[] = {
    gUnknown_8459544,
    gUnknown_8459564
};

ALIGNED(4) const u8 gUnknown_8459588[] = _("Finished checking {SPECIAL_F7 0x01}'s\nTrainer Card.{PAUSE 60}");

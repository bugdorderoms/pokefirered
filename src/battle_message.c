#include "global.h"
#include "gflib.h"
#include "battle.h"
#include "battle_anim.h"
#include "strings.h"
#include "battle_message.h"
#include "link.h"
#include "event_scripts.h"
#include "event_data.h"
#include "item.h"
#include "battle_tower.h"
#include "battle_setup.h"
#include "field_specials.h"
#include "new_menu_helpers.h"
#include "battle_controllers.h"
#include "graphics.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/trainers.h"
#include "constants/weather.h"
#include "constants/battle_string_ids.h"

struct BattleWindowText
{
    u8 fillValue;
    u8 fontId;
    u8 x;
    u8 y;
    u8 letterSpacing;
    u8 lineSpacing;
    u8 speed;
    u8 fgColor;
    u8 bgColor;
    u8 shadowColor;
};

static void ExpandBattleTextBuffPlaceholders(const u8 *src, u8 *dst);

static EWRAM_DATA u16 sBattlerAbilities[MAX_BATTLERS_COUNT] = {};
static EWRAM_DATA struct BattleMsgData *sBattleMsgDataPtr = NULL;

static const struct BattleWindowText sTextOnWindowsInfo_Normal[] =
{
    [B_WIN_MSG] = {
        .fillValue = PIXEL_FILL(0xf),
        .fontId = 2,
        .x = 2,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 2,
        .speed = 1,
        .fgColor = 1,
        .bgColor = 15,
        .shadowColor = 6,
    },
    [B_WIN_ACTION_PROMPT] = {
        .fillValue = PIXEL_FILL(0xf),
        .fontId = 2,
        .x = 2,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 2,
        .speed = 0,
        .fgColor = 1,
        .bgColor = 15,
        .shadowColor = 6,
    },
    [B_WIN_ACTION_MENU] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = 1,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 2,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_MOVE_NAME_1] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = 0,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_MOVE_NAME_2] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = 0,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_MOVE_NAME_3] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = 0,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_MOVE_NAME_4] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = 0,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_PP] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = 0,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 12,
        .bgColor = 14,
        .shadowColor = 11,
    },
    [B_WIN_MOVE_TYPE] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = 0,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_PP_REMAINING] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = 1,
        .x = 10,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 2,
        .speed = 0,
        .fgColor = 12,
        .bgColor = 14,
        .shadowColor = 11,
    },
    [B_WIN_DUMMY] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = 1,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 2,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_SWITCH_PROMPT] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = 1,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 2,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_LEVEL_UP_BOX] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = 2,
        .x = 0,
        .y = 0,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_LEVEL_UP_BANNER] = {
        .fillValue = PIXEL_FILL(0x0),
        .fontId = 0,
        .x = 0x20,
        .y = 0,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 1,
        .bgColor = 0,
        .shadowColor = 2,
    },
    [B_WIN_YESNO] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = 2,
        .x = 0,
        .y = 2,
        .letterSpacing = 1,
        .lineSpacing = 2,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_VS_PLAYER] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = 2,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_VS_OPPONENT] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = 2,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_1] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = 2,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_2] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = 2,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_3] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = 2,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_4] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = 2,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_VS_OUTCOME_DRAW] = {
        .fillValue = PIXEL_FILL(0x0),
        .fontId = 2,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 1,
        .bgColor = 0,
        .shadowColor = 6,
    },
    [B_WIN_VS_OUTCOME_LEFT] = {
        .fillValue = PIXEL_FILL(0x0),
        .fontId = 2,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 1,
        .bgColor = 0,
        .shadowColor = 6,
    },
    [B_WIN_VS_OUTCOME_RIGHT] = {
        .fillValue = PIXEL_FILL(0x0),
        .fontId = 2,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 1,
        .bgColor = 0,
        .shadowColor = 6,
    },
    [B_WIN_OAK_OLD_MAN] = {
        .fillValue = PIXEL_FILL(0x1),
        .fontId = 4,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 1,
        .speed = 1,
        .fgColor = 2,
        .bgColor = 1,
        .shadowColor = 3,
    }
};

static const u8 sNpcTextColorToFont[] = {0x04, 0x05, 0x02, 0x02};

// Strings
const u8 gText_EmptyString[] = _("");
const u8 gText_Win[] = _("{HIGHLIGHT 0}Win");
const u8 gText_Loss[] = _("{HIGHLIGHT 0}Loss");
const u8 gText_Draw[] = _("{HIGHLIGHT 0}Draw");
const u8 gText_ForPetesSake[] = _("Oak: Oh, for Pete's sake…\nSo pushy, as always.\p{B_PLAYER_NAME}.\pYou've never had a Pokémon battle before, have you?\nA Pokémon battle is when Trainers pit their Pokémon against each other.\p");
const u8 gText_TheTrainerThat[] = _("The Trainer that makes the other Trainer's Pokémon faint by lowering their HP to “0,” wins.\p");
const u8 gText_TryBattling[] = _("But rather than talking about it, you'll learn more from experience. Try battling and see for yourself.\p");
const u8 gText_InflictingDamageIsKey[] = _("Oak: Inflicting damage on the foe is the key to any battle.\p");
const u8 gText_LoweringStats[] = _("Oak: Lowering the foe's stats will put you at an advantage.\p");
const u8 gText_KeepAnEyeOnHP[] = _("Oak: Keep your eyes on your Pokémon's HP. It will faint if the HP drops to “0.”\p");
const u8 gText_OakNoRunningFromATrainer[] = _("Oak: No! There's no running away from a Trainer Pokémon battle!\p");
const u8 gText_WinEarnsPrizeMoney[] = _("Oak: Hm! Excellent!\nIf you win, you earn prize money, and your Pokémon will grow!\nBattle other Trainers and make your Pokémon strong!\p");
const u8 gText_HowDissapointing[] = _("Oak: Hm…\nHow disappointing…\nIf you win, you earn prize money, and your Pokémon grow. But if you lose, {B_PLAYER_NAME}, you end up paying prize money…\nHowever, since you had no warning this time, I'll pay for you.\nBut things won't be this way once you step outside these doors. That's why you must strengthen your Pokémon by battling wild Pokémon.\p");
const u8 gText_MoveInterfacePP[] = _("PP ");
const u8 gText_MoveInterfaceType[] = _("Type/");
const u8 gText_MoveInterfaceEffectiveness[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 9 14 8}");
const u8 gText_MoveInterfaceMoveNamesColor[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}");
const u8 gText_MoveInfoPowerAndAccuracy[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Power: {B_BUFF1}\nAcc.: {B_BUFF2}");
const u8 gText_MoveInfoPriorityAndCategory[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Prio.: {B_BUFF1}\nCat/{B_BUFF2}");
const u8 gText_MoveInfoTarget[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Target:\n{B_BUFF1}");
const u8 gText_BattleYesNoChoice[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Yes\nNo");
const u8 gText_BattleSwitchWhich[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Switch\nwhich?");
const u8 gText_PkmnIsEvolving[] = _("What?\n{STR_VAR_1} is evolving!");
const u8 gText_CongratsPkmnEvolved[] = _("Congratulations! Your {STR_VAR_1}\nevolved into {STR_VAR_2}!{WAIT_SE}\p");
const u8 gText_PkmnStoppedEvolving[] = _("Huh? {STR_VAR_1}\nstopped evolving!\p");
const u8 gText_EllipsisQuestionMark[] = _("……?\p");
const u8 gText_WhatWillBufferDo[] = _("What will\n{B_BUFF1} do?");
const u8 gText_WhatWillPlayerThrow[] = _("What will {B_PLAYER_NAME}\nthrow?");
const u8 gText_TheOldMan[] = _("the old man");
const u8 gText_LinkStandby[] = _("{PAUSE 16}Link standby…");
const u8 gText_BattleMenu[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Fight{CLEAR_TO 56}Bag\nPokémon{CLEAR_TO 56}Run");
const u8 gText_SafariMenu[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Ball{CLEAR_TO 56}Bait\nRock{CLEAR_TO 56}Run");
static const u8 sText_Trainer1WantsToBattle[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME} would like to battle!\p");
static const u8 sText_GoPkmn[] = _("Go! {B_BUFF1}!");
static const u8 sText_Trainer1SentOutPkmn[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME} sent out {B_OPPONENT_MON1_NAME}!{PAUSE 60}");
static const u8 sText_LinkTrainerSentOutPkmn[] = _("{B_LINK_OPPONENT1_NAME} sent out {B_BUFF1}!");
static const u8 sText_Trainer1SentOutPkmn2[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME} sent out {B_BUFF1}!");
static const u8 sText_PlayerDefeatedLinkTrainerTrainer1[] = _("Player defeated {B_TRAINER1_CLASS} {B_TRAINER1_NAME}!\p");
static const u8 sText_FoePkmnPrefix[] = _("Foe ");
static const u8 sText_WildPkmnPrefix[] = _("Wild ");
static const u8 sText_YourTeamPrefix[] = _("Your team");
static const u8 sText_TheOpposingTeamPrefix[] = _("The opposing team");
static const u8 sText_GotAwaySafely[] = _("{PLAY_SE SE_FLEE}Got away safely!\p");








static const u8 sText_PkmnUproarKeptAwake[] = _("But {B_SCR_ACTIVE_NAME_WITH_PREFIX}'s Uproar\nkept it awake!");
static const u8 sText_PkmnCausedUproar[] = _("{B_ATK_NAME_WITH_PREFIX} caused\nan Uproar!");
static const u8 sText_PkmnMakingUproar[] = _("{B_ATK_NAME_WITH_PREFIX} is making\nan Uproar!");
static const u8 sText_PkmnCalmedDown[] = _("{B_ATK_NAME_WITH_PREFIX} calmed down.");
static const u8 sText_PkmnCantSleepInUproar[] = _("But {B_DEF_NAME_WITH_PREFIX} can't\nsleep in an Uproar!");
static const u8 sText_PkmnCantSleepInUproar2[] = _("But {B_DEF_NAME_WITH_PREFIX} can't\nsleep in an Uproar!");
static const u8 sText_UproarKeptPkmnAwake[] = _("But the Uproar kept\n{B_DEF_NAME_WITH_PREFIX} awake!");
static const u8 sText_PkmnStayedAwakeUsing[] = _("{B_DEF_NAME_WITH_PREFIX} stayed awake\nusing its {B_DEF_ABILITY}!");
static const u8 sText_PkmnSwitchedItems[] = _("{B_ATK_NAME_WITH_PREFIX} switched\nitems with its opponent!");
static const u8 sText_PkmnObtainedX[] = _("{B_ATK_NAME_WITH_PREFIX} obtained\n{B_BUFF1}.");
static const u8 sText_PkmnObtainedX2[] = _("{B_DEF_NAME_WITH_PREFIX} obtained\n{B_BUFF2}.");
static const u8 sText_PkmnObtainedXYObtainedZ[] = _("{B_ATK_NAME_WITH_PREFIX} obtained\n{B_BUFF1}.\p{B_DEF_NAME_WITH_PREFIX} obtained\n{B_BUFF2}.");
static const u8 sText_PkmnKnockedOff[] = _("{B_ATK_NAME_WITH_PREFIX} knocked off\n{B_DEF_NAME_WITH_PREFIX}'s {B_LAST_ITEM}!");
static const u8 sText_PkmnCutsAttackWith[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\ncuts {B_DEF_NAME_WITH_PREFIX}'s Attack!");
static const u8 sText_PkmnFledUsingIts[] = _("{PLAY_SE SE_FLEE}{B_ATK_NAME_WITH_PREFIX} fled\nusing its {B_LAST_ITEM}!\p");
static const u8 sText_PkmnFledUsing[] = _("{PLAY_SE SE_FLEE}{B_ATK_NAME_WITH_PREFIX} fled\nusing {B_ATK_ABILITY}!\p");
static const u8 sText_Trainer1WithdrewPkmn[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}\nwithdrew {B_BUFF1}!");
static const u8 sText_PkmnShookOffTaunt[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} shook off\nthe taunt!");
static const u8 sText_PkmnsItemCuredParalysis[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\ncured paralysis!");
static const u8 sText_PkmnsItemCuredPoison[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\ncured poison!");
static const u8 sText_PkmnsItemHealedBurn[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nhealed its burn!");
static const u8 sText_PkmnsItemDefrostedIt[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\ndefrosted it!");
static const u8 sText_PkmnsItemWokeIt[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nwoke it from its sleep!");
static const u8 sText_PkmnsItemSnappedOut[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nsnapped it out of confusion!");
static const u8 sText_PkmnsItemCuredProblem[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\ncured its {B_BUFF1} problem!");
static const u8 sText_PkmnsItemNormalizedStatus[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nnormalized its status!");
static const u8 sText_PkmnsItemRestoredHealth[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nrestored health!");
static const u8 sText_PkmnsItemRestoredPP[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nrestored {B_BUFF1}'s PP!");
static const u8 sText_PkmnsItemRestoredStatus[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nrestored its status!");
static const u8 sText_PkmnsItemRestoredHPALittle[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nrestored its HP a little!");






const u16 gUproarOverTurnStringIds[] = {
    STRINGID_PKMNMAKINGUPROAR,
    STRINGID_PKMNCALMEDDOWN
};

const u16 gUproarAwakeStringIds[] = {
    STRINGID_PKMNCANTSLEEPINUPROAR2,
    STRINGID_UPROARKEPTPKMNAWAKE,
    STRINGID_PKMNSTAYEDAWAKEUSING,
	STRINGID_ITDOESNTAFFECT
};








// Multistring chooser tables
const u16 gMissStringIds[] =
{
    [B_MSG_MISSED]      = STRINGID_ATTACKMISSED,
    [B_MSG_PROTECTED]   = STRINGID_DEFPROTECTEDITSELF,
    [B_MSG_AVOIDED_ATK] = STRINGID_DEFAVOIDEDATTACK
};

const u16 gGotDefrostedStringIds[] =
{
    [B_MSG_DEFROST]         = STRINGID_BUFF1WASDEFROSTED,
    [B_MSG_DEFROST_BY_MOVE] = STRINGID_ATKDEFROSTEDBYCURRMOVE
};

const u16 gStatusHealStringIds[] =
{
	[B_MSG_WOKEUP_EFFECT] = STRINGID_EFFWOKEUP,
	[B_MSG_CURED_BUFF1]   = STRINGID_EFFCUREDOFBUFF1
};

const u16 gReflectLightScreenSafeguardStringIds[] =
{
    [B_MSG_REFLECT_RAISED]      = STRINGID_BUFF1RAISEDATKTEAMBUFF2,
    [B_MSG_SAFEGUARD_COVERED]   = STRINGID_ATKTEAMCLOAKEDINMYSTICALVEIL
};

const u16 gSideStatusWoreOffStringIds[] =
{
	[B_MSG_SIDE_STATUS_WORE_OFF] = STRINGID_ATKTEAMBUFF1WOREOFF,
	[B_MSG_SAFEGUARD_WORE_OFF]   = STRINGID_ATKTEAMSAFEGUARDEXPIRED,
	[B_MSG_TAILWIND_PETERED_OUT] = STRINGID_ATKTEAMTAILWINDPETEREDOUT
};

const u16 gRestUsedStringIds[] =
{
    [B_MSG_WENT_TO_SLEEP] = STRINGID_DEFWENTTOSLEEP,
    [B_MSG_SLEPT_HEALTHY] = STRINGID_DEFSLEPTHEALTHY
};

const u16 gFirstTurnOfTwoStringIds[] =
{
    [B_MSG_WHIPPED_WHIRLWIND]   = STRINGID_ATKWHIPPEDWHIRLWIND,
    [B_MSG_CLOAKED_IN_SUNLIGHT] = STRINGID_ATKBECAMECLOAKEDINSUNLIGHT,
	[B_MSG_BURROWED_IN_GROUND]  = STRINGID_ATKBURROWEDUNDERTHEGROUND,
    [B_MSG_TUCKED_HEAD]         = STRINGID_ATKTUCKEDINHEAD,
	[B_MSG_SPRANG_UP]           = STRINGID_ATKSPRANGUP,
	[B_MSG_HID_UNDERWATER]      = STRINGID_ATKHIDUNDERWATER,
	[B_MSG_FLEW_UP_HIGH]        = STRINGID_ATKFLEWHIGH,
    [B_MSG_ABSORBED_LIGHT]      = STRINGID_ATKABSORBEDLIGHT
};

const u16 gWrappedStringIds[] =
{
    [TRAP_ID_BIND]      = STRINGID_EFFSQUEEZEDBYPKMN,
    [TRAP_ID_WRAP]      = STRINGID_EFFWRAPPEDBYPKMN,
    [TRAP_ID_FIRE_SPIN] = STRINGID_EFFTRAPPEDINVORTEX,
	[TRAP_ID_WHIRLPOOL] = STRINGID_EFFTRAPPEDINVORTEX,
    [TRAP_ID_CLAMP]     = STRINGID_PKMNCLAMPEDEFF,
    [TRAP_ID_SAND_TOMB] = STRINGID_EFFTRAPPEDBYCURRMOVE
};

const u16 gSandstormHailDmgStringIds[] =
{
    [B_MSG_DAMAGED_BY_SANDSTORM] = STRINGID_ATKBUFFETEDBYSANDSTORM,
    [B_MSG_DAMAGED_BY_HAIL]      = STRINGID_ATKPELTEDBYHAIL
};

const u16 gLeechSeedStringIds[] =
{
    [B_MSG_SEEDED]                  = STRINGID_DEFWASSEEDED,
    [B_MSG_EVADED_ATTACK]           = STRINGID_DEFEVADEDATTACK,
    [B_MSG_LEECH_SEED_DOEST_AFFECT] = STRINGID_ITDOESNTAFFECT
};

const u16 gWokeUpStringIds[] =
{
    [B_MSG_WOKEUP]        = STRINGID_ATKWOKEUP,
    [B_MSG_UPROAR_WOKEUP] = STRINGID_ATKWOKEUPINUPROAR
};

const u16 gPrimalWeatherBlocksAttackStringIds[] =
{
	[B_MSG_FIZZLES_ATTACK]   = STRINGID_FIZZLESBUFF1TYPEATTACK,
	[B_MSG_EVAPORATE_ATTACK] = STRINGID_EVAPORATEBUFF1TYPEATTACK
};

const u16 gKOFailedStringIds[] =
{
    [B_MSG_KO_MISSED]  = STRINGID_ATTACKMISSED,
    [B_MSG_UNAFFECTED] = STRINGID_DEFUNAFFECTED
};

const u16 gSubsituteUsedStringIds[] =
{
    [B_MSG_MADE_SUBSTITUTE]        = STRINGID_ATKMADESUBSTITUTE,
    [B_MSG_TOO_WEAK_TO_SUBSTITUTE] = STRINGID_TOOWEAKFORSUBSTITUTE,
	[B_MSG_HAS_SUBSTITUTE]         = STRINGID_ATKHASSUBSTITUTE
};

const u16 gProtectLikeUsedStringIds[] =
{
    [B_MSG_USED_PROTECT]   = STRINGID_DEFPROTECTEDITSELF,
    [B_MSG_BRACED_ITSELF]  = STRINGID_ATKBRACEDITSELF
};

const u16 gFutureMoveUsedStringIds[] =
{
    [B_MSG_FORESAW_ATTACK]   = STRINGID_ATKFORESAWATTACK,
    [B_MSG_CHOSE_AS_DESTINY] = STRINGID_ATKCHOSECURRMOVEASDESTINY
};

const u16 gFlashFireStringIds[] =
{
    [B_MSG_FLASH_FIRE_BOOST]       = STRINGID_POWEROFDEFBUFF1TYPEMOVESROSE,
    [B_MSG_FLASH_FIRE_DONT_AFFECT] = STRINGID_ITDOESNTAFFECT
};

const u16 gCaughtMonStringIds[] =
{
    [B_MSG_TRANSFERRED_TO_SOMEONE_PC] = STRINGID_PKMNTRANSFERREDSOMEONESPC,
    [B_MSG_TRANSFERRED_TO_BILL_PC]    = STRINGID_PKMNTRANSFERREDBILLSPC,
    [B_MSG_SOMEONE_PC_BOX_FULL]       = STRINGID_PKMNBOXSOMEONESPCFULL,
    [B_MSG_BILL_PC_BOX_FULL]          = STRINGID_PKMNBOXBILLSPCFULL
};

const u16 gBallEscapeStringIds[] =
{
    [BALL_NO_SHAKES]     = STRINGID_POKEMONBROKEFREE,
    [BALL_1_SHAKE]       = STRINGID_ITAPPEAREDCAUGHT,
    [BALL_2_SHAKES]      = STRINGID_AARGHALMOSTHADIT,
    [BALL_3_SHAKES_FAIL] = STRINGID_SHOOTSOCLOSE
};

const u16 gSafariPokeblockResultStringIds[] =
{
    [B_MSG_WATCHING_CAREFULLY] = STRINGID_PKMNWATCHINGCAREFULLY,
    [B_MSG_ANGRY]              = STRINGID_PKMNANGRY,
    [B_MSG_EATING]             = STRINGID_PKMNEATING
};

const u16 gNoEscapeStringIds[] =
{
    [B_MSG_CANT_ESCAPE]            = STRINGID_CANTESCAPE,
    [B_MSG_DONT_LEAVE_BIRTH]       = STRINGID_DONTLEAVEBIRCH,
    [B_MSG_ABILITY_PREVENT_ESCAPE] = STRINGID_PKMNPREVENTSESCAPEWITHABL,
    [B_MSG_ATK_CANT_ESCAPE]        = STRINGID_ATKCANTESCAPE
};

const u16 gWeatherContinuesStringIds[] =
{
    [B_MSG_RAIN_CONTINUES]         = STRINGID_RAINCONTINUES,
    [B_MSG_RAIN_STOPPED]           = STRINGID_RAINSTOPPED,
	[B_MSG_SUN_CONTINUES]          = STRINGID_SUNLIGHTSTRONG,
	[B_MSG_SUN_STOPPED]            = STRINGID_SUNLIGHTFADED,
	[B_MSG_SANDSTORM_CONTINUES]    = STRINGID_SANDSTORMRAGES,
	[B_MSG_SANDSTORM_STOPPED]      = STRINGID_SANDSTORMSUBSIDED,
	[B_MSG_HAIL_CONTINUES]         = STRINGID_HAILCONTINUES,
	[B_MSG_HAIL_STOPPED]           = STRINGID_HAILSTOPPED,
	[B_MSG_FOG_CONTINUES]          = STRINGID_DEEPFOG,
	[B_MSG_FOG_STOPPED]            = STRINGID_FOGENDED,
	[B_MSG_STRONG_WINDS_CONTINUES] = STRINGID_STRONGWINDSCONTINUES
};

const u16 gWeatherStartsStringIds[] =
{
    [B_MSG_STARTED_RAIN]             = STRINGID_STARTEDTORAIN,
    [B_MSG_WEATHER_MOVE_FAIL]        = STRINGID_BUTITFAILED,
    [B_MSG_SANDSTORM_UP]             = STRINGID_ASANDSTORMKICKEDUP,
    [B_MSG_SUN_TURN_HARSH]           = STRINGID_SUNLIGHTTURNEDHARSH,
    [B_MSG_STARTED_HAIL]             = STRINGID_STARTEDHAIL,
	[B_MSG_DEEP_FOG]                 = STRINGID_DEEPFOG,
	[B_MSG_HEAVY_RAIN]               = STRINGID_HEAVYRAINFALL,
	[B_MSG_SUN_TURN_EXTREMELY_HARSH] = STRINGID_SUNLIGHTTURNEDEXTREMELYHARSH,
	[B_MSG_STRONG_WINDS_PROTECTING]  = STRINGID_STRONGWINDSPROTECTBUFF1TYPE
};

const u16 gPartyStatusHealStringIds[] =
{
    [B_MSG_BELL_CHIMED]    = STRINGID_BELLCHIMED,
    [B_MSG_SOOTHING_AROMA] = STRINGID_SOOTHINGAROMA
};

const u16 gCantUseItemStringIds[] =
{
	[B_MSG_CANT_USE_ITEM] = STRINGID_ITEMSCANTBEUSEDNOW,
	[B_MSG_BOX_FULL]      = STRINGID_BOXISFULL
};

const u16 gNotAllowedMoveStringIds[] =
{
	[B_MSG_MOVE_DISABLED]           = STRINGID_ATKCURRMOVEISDISABLED,
	[B_MSG_NO_MOVES]                = STRINGID_ATKHASNOMOVESLEFT,
	[B_MSG_MOVE_TORMENTED]          = STRINGID_ATKCANTUSEMOVETORMENT,
	[B_MSG_MOVE_TAUNTED]            = STRINGID_ATKCANTUSECURRMOVETAUNT,
	[B_MSG_MOVE_SEALED]             = STRINGID_ATKCANTUSESEALEDCURRMOVE,
	[B_MSG_MOVE_LOCKED_CHOICE_ITEM] = STRINGID_LASTITEMALLOWSONLYCURRMOVE,
	[B_MSG_MOVE_NO_PP]              = STRINGID_NOPPLEFT,
	[B_MSG_MOVE_IN_GRAVITY]         = STRINGID_ATKCANTUSECURRMOVEINGRAVITY,
	[B_MSG_MOVE_LOCKED_ABILITY]     = STRINGID_ATKABLALLOWSONLYCURRMOVE,
	[B_MSG_MOVE_HEAL_BLOCKED]       = STRINGID_ATKPREVENTEDFROMHEALING
};

const u16 gStockpileUsedStringIds[] =
{
    [B_MSG_ATK_STOCKPILED]     = STRINGID_ATKSTOCKPILED,
    [B_MSG_ATK_CANT_STOCKPILE] = STRINGID_ATKCANTSTOCKPILE
};

const u16 gSwallowFailStringIds[] =
{
    [B_MSG_FAILED_TO_SWALLOW] = STRINGID_BUTFAILEDTOCURRMOVEATHING,
    [B_MSG_SWALLOW_FULL_HP]   = STRINGID_DEFHPFULL
};

const u16 gFieldSportUsedStringIds[] =
{
    [B_MSG_ELECTRICITY_WEAKENED] = STRINGID_ELECTRICITYPOWERWEAKENED,
    [B_MSG_FIRE_WEAKENED]        = STRINGID_FIREPOWERWEAKENED
};

const u16 gFieldStatusWoreOffStringIds[] =
{
	[B_MSG_FIELD_EFFECTS_WORE_OFF] = STRINGID_BUFF1EFFECTSWOREOFF,
	[B_MSG_GRAVITY_NORMALIZED]     = STRINGID_GRAVITYRETURNEDTONORMAL
};

const u16 gSwitchInAbilitiesMsgStringIds[] =
{
	[B_MSG_BREAKS_THE_MOLD]          = STRINGID_PKMNBREAKSTHEMOLD,
	[B_MSG_CANT_GET_GOING]           = STRINGID_PKMNCANTGETGOING,
	[B_MSG_BLAZING_AURA]             = STRINGID_PKMNRADIATINGABLAZINGAURA,
	[B_MSG_NERVOUS_TO_EAT]           = STRINGID_BUFF1ISNERVOUSTOEATBERRIES,
	[B_MSG_BURSTING_AURA]            = STRINGID_PKMNRADIATINGABURSTINGAURA,
	[B_MSG_DARK_AURA]                = STRINGID_PKMNRADIATINGADARKAURA,
	[B_MSG_FAIRY_AURA]               = STRINGID_PKMNRADIATINGAFAIRYAURA,
	[B_MSG_REVERSED_AURAS]           = STRINGID_PKMNREVERSEDALLOTHERAURAS,
	[B_MSG_PKMN_DROWSING]            = STRINGID_PKMNISDROWSING,
	[B_MSG_UNDERWENT_TRANSFORMATION] = STRINGID_PKMNUNDERWENTTRANSFORMATION,
	[B_MSG_SHUDDERED]                = STRINGID_PKMNSHUDDERED,
	[B_MSG_ALERTED]                  = STRINGID_ALERTEDTOPKMNBUFF1,
	[B_MSG_SCREENS_CLEANSED]         = STRINGID_ALLSCREENSCLEANSED,
	[B_MSG_STAT_CHANGES_RESETED]     = STRINGID_PKMNSTATCHANGESWERERESET,
	[B_MSG_HAS_TWO_ABILITIES]        = STRINGID_PKMNHASTWOABILITIES,
	[B_MSG_EXERTING_ABILITY]         = STRINGID_PKMNEXERTINGITSABL,
	[B_MSG_GAINED_STRENGTH]          = STRINGID_PKMNGAINEDSTRENGTHFROMFALLEN,
	[B_MSG_WEAKENED_STAT]            = STRINGID_PKMNABLWEAKENEDBUFF1OFALL,
	[B_MSG_COPIED_STAT_CHANGES]      = STRINGID_PKMNCOPIEDBUFF1STATCHANGES
};

const u16 gHealingWishLunarDanceStringIds[] =
{
	[B_MSG_HEALING_WISH_TRUE] = STRINGID_HEALINGWISHCAMETRUEFORPKMN
};

const u16 gEntryHazardsDmgStringIds[] =
{
	[B_MSG_HURT_BY_SPIKES]           = STRINGID_PKMNHURTBYSPIKES,
	[B_MSG_ABSORBED_TOXIC_SPIKES]    = STRINGID_POISONSPIKESDISAPPEARED
};

const u16 gFormChangeAbilitiesStringIds[] =
{
	[B_MSG_ABL_TRIGGERED]     = STRINGID_PKMNABLTRIGGERED,
	[B_MSG_FORMED_SCHOOL]     = STRINGID_PKMNFORMEDASCHOOL,
	[B_MSG_STOPPED_SCHOOLING] = STRINGID_PKMNSTOPPEDSCHOOLING,
	[B_MSG_ABL_ACTIVATED]     = STRINGID_PKMNABLACTIVATED,
	[B_MSG_ABL_DEACTIVATED]   = STRINGID_PKMNABLDEACTIVATED,
	[B_MSG_TRANSFORMED]       = STRINGID_PKMNTRANSFORMED
};

const u16 gStanceChangeStringIds[] =
{
	[B_MSG_CHANGED_TO_BLADE]  = STRINGID_CHANGEDTOBLADEFORME,
	[B_MSG_CHANGED_TO_SHIELD] = STRINGID_CHANGEDTOSHIELDFORME
};

const u16 gSplitSwapMovesStringIds[] =
{
	[B_MSG_SWITCHED_STATS]        = STRINGID_ATKSWITCHEDBUFF1ANDBUFF2,
	[B_MSG_SWITCHED_STAT_CHANGES] = STRINGID_ATKSWITCHBUFF1ANDBUFF2CHANGES
};

const u16 gStatusCureAbilityStringIds[] =
{
	[B_MSG_CURED_ITS_PROBLEM]     = STRINGID_PKMNABLCUREDITSBUFF1PROBLEM,
	[B_MSG_CURED_POKEMON_PROBLEM] = STRINGID_DEFABLCUREDPKMNBUFF1PROBLEM,
	[B_MSG_CURED_MENTAL_STATE]    = STRINGID_PKMNABLCUREDITSMENTALSTATE,
	[B_MSG_CURED_CONFUSION]       = STRINGID_PKMNABLCUREDITSCONFUSION,
	[B_MSG_CURED_OF_ITS_PROBLEM]  = STRINGID_PKMNCUREDOFITSBUFF1
};

// String tables
static const u8 *const sBattleStatNamesTable[] =
{
    [STAT_HP]      = COMPOUND_STRING("HP"),
    [STAT_ATK]     = COMPOUND_STRING("Attack"),
    [STAT_DEF]     = COMPOUND_STRING("Defense"),
    [STAT_SPEED]   = COMPOUND_STRING("Speed"),
    [STAT_SPATK]   = COMPOUND_STRING("Sp. Atk"),
    [STAT_SPDEF]   = COMPOUND_STRING("Sp. Def"),
    [STAT_ACC]     = COMPOUND_STRING("accuracy"),
    [STAT_EVASION] = COMPOUND_STRING("evasiveness")
};

static const u8 *const sPokeblockWasTooXStringTable[] =
{
    [FLAVOR_SPICY]  = COMPOUND_STRING("was too spicy!"),
    [FLAVOR_DRY]    = COMPOUND_STRING("was too dry!"),
    [FLAVOR_SWEET]  = COMPOUND_STRING("was too sweet!"),
    [FLAVOR_BITTER] = COMPOUND_STRING("was too bitter!"),
    [FLAVOR_SOUR]   = COMPOUND_STRING("was too sour!")
};

const u8 *const gBattleStringsTable[] =
{
	[STRINGID_TRAINER1LOSETEXT - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_TRAINER1_LOSE_TEXT}"),
	[STRINGID_PKMNGAINEDEXP - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("{B_BUFF1} gained{B_BUFF2} {B_BUFF3} Exp. Points!\p"),
	[STRINGID_PKMNGREWTOLV - BATTLESTRINGS_ID_ADDER]                  = COMPOUND_STRING("{B_BUFF1} grew\nto Lv. {B_BUFF2}!{WAIT_SE}\p"),
	[STRINGID_PKMNLEARNEDMOVE - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_BUFF1} learned {B_BUFF2}!{WAIT_SE}\p"),
	[STRINGID_TRYTOLEARNMOVE1 - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_BUFF1} is trying to learn {B_BUFF2}.\p"),
	[STRINGID_TRYTOLEARNMOVE2 - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("But, {B_BUFF1} can't learn more than four moves.\p"),
	[STRINGID_TRYTOLEARNMOVE3 - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("Delete a move to make room for {B_BUFF2}?"),
	[STRINGID_PKMNFORGOTMOVE - BATTLESTRINGS_ID_ADDER]                = COMPOUND_STRING("{B_BUFF1} forgot\n{B_BUFF2}.\p"),
	[STRINGID_STOPLEARNINGMOVE - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{PAUSE 32}Stop learning\n{B_BUFF2}?"),
	[STRINGID_DIDNOTLEARNMOVE - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_BUFF1} did not learn\n{B_BUFF2}.\p"),
	[STRINGID_ATKLEARNEDBUFF1 - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} learned {B_BUFF1}!"),
	[STRINGID_ATTACKMISSED - BATTLESTRINGS_ID_ADDER]                  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s attack missed!"),
	[STRINGID_DEFPROTECTEDITSELF - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} protected itself!"),
	[STRINGID_ITDOESNTAFFECT - BATTLESTRINGS_ID_ADDER]                = COMPOUND_STRING("It doesn't affect {B_DEF_NAME_WITH_PREFIX}…"),
	[STRINGID_ATKFAINTED - BATTLESTRINGS_ID_ADDER]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} fainted!\p"),
	[STRINGID_DEFFAINTED - BATTLESTRINGS_ID_ADDER]                    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} fainted!\p"),
	[STRINGID_PLAYERGOTMONEY - BATTLESTRINGS_ID_ADDER]                = COMPOUND_STRING("{B_PLAYER_NAME} got ¥{B_BUFF1} for winning!\p"),
	[STRINGID_PLAYERWHITEOUT - BATTLESTRINGS_ID_ADDER]                = COMPOUND_STRING("{B_PLAYER_NAME} is out of usable Pokémon!\p"),
	[STRINGID_PLAYERPANICKEDANDLOST - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("{B_PLAYER_NAME} panicked and lost ¥{B_BUFF1}…\p… … … …\p{B_PLAYER_NAME} whited out!{PAUSE_UNTIL_PRESS}"),
	[STRINGID_PKMNPREVENTSESCAPEWITHABL - BATTLESTRINGS_ID_ADDER]     = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} prevents escape with {B_SCR_ACTIVE_ABILITY}!"),
	[STRINGID_HITXTIMES - BATTLESTRINGS_ID_ADDER]                     = COMPOUND_STRING("Hit {B_BUFF1} time(s)!"),
	[STRINGID_EFFFELLASLEEP - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} fell asleep!"),
	[STRINGID_DEFALREADYASLEEP - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} is already asleep!"),
	[STRINGID_DEFWASNTAFFECTED - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} wasn't affected!"),
	[STRINGID_EFFWASPOISONED - BATTLESTRINGS_ID_ADDER]                = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} was poisoned!"),
	[STRINGID_ATKHURTBYPOISON - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is hurt by poison!"),
	[STRINGID_DEFALREADYPOISONED - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} is already poisoned."),
	[STRINGID_EFFBADLYPOISONED - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} is badly poisoned!"),
	[STRINGID_DEFENERGYDRAINED - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} had its energy drained!"),
	[STRINGID_EFFWASBURNED - BATTLESTRINGS_ID_ADDER]                  = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} was burned!"),
	[STRINGID_ATKHURTBYBURN - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is hurt by its burn!"),
	[STRINGID_EFFWASFROZEN - BATTLESTRINGS_ID_ADDER]                  = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} was frozen solid!"),
	[STRINGID_ATKISFROZEN - BATTLESTRINGS_ID_ADDER]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is frozen solid!"),
	[STRINGID_BUFF1WASDEFROSTED - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_BUFF1} was defrosted!"),
	[STRINGID_ATKDEFROSTEDBYCURRMOVE - BATTLESTRINGS_ID_ADDER]        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} was defrosted by {B_CURRENT_MOVE}!"),
	[STRINGID_EFFWASPARALYZED - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} is paralyzed!\nIt may be unable to move!"),
	[STRINGID_ATKISPARALYZED - BATTLESTRINGS_ID_ADDER]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is paralyzed!\nIt can't move!"),
	[STRINGID_DEFISALREADYPARALYZED - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} is already paralyzed!"),
	[STRINGID_EFFABLPREVENTSBURNS - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}'s {B_EFF_ABILITY} prevents burns!"),
	[STRINGID_EFFCUREDOFBUFF1 - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} was cured of {B_BUFF1}."),
	[STRINGID_EFFWOKEUP - BATTLESTRINGS_ID_ADDER]                     = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} woke up!"),
	[STRINGID_ATKISCONFUSED - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is confused!"),
	[STRINGID_ATKSNAPPEDOUTOFCONFUSION - BATTLESTRINGS_ID_ADDER]      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} snapped out of confusion!"),
	[STRINGID_EFFBECAMECONFUSED - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} became confused!"),
	[STRINGID_DEFALREADYCONFUSED - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} is already confused!"),
	[STRINGID_DEFFELLINLOVE - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} fell in love!"),
	[STRINGID_ATKISINLOVEWITHPKMN - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is in love with {B_SCR_ACTIVE_NAME_WITH_PREFIX}!"),
	[STRINGID_ATKIMMOBILIZEDBYLOVE - BATTLESTRINGS_ID_ADDER]          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is immobilized by love!"),
	[STRINGID_PKMNTYPECHANGEDTOBUFF1 - BATTLESTRINGS_ID_ADDER]        = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s type changed to {B_BUFF1}!"),
	[STRINGID_ATKFLINCHED - BATTLESTRINGS_ID_ADDER]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} flinched!"),
	[STRINGID_DEFREGAINEDHEALTH - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} regained health!"),
	[STRINGID_DEFHPFULL - BATTLESTRINGS_ID_ADDER]                     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s HP is full!"),
	[STRINGID_ATKTEAMCLOAKEDINMYSTICALVEIL - BATTLESTRINGS_ID_ADDER]  = COMPOUND_STRING("{B_ATK_TEAM_PREFIX} became cloaked in a mystical veil!"),
	[STRINGID_BUFF1RAISEDATKTEAMBUFF2 - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("{B_BUFF1} raised {B_ATK_TEAM_PREFIX}'s {B_BUFF2}!"),
	[STRINGID_DEFTEAMSAFEGUARDPROTECTED - BATTLESTRINGS_ID_ADDER]     = COMPOUND_STRING("{B_DEF_TEAM_PREFIX} is protected by Safeguard!"),
	[STRINGID_ATKTEAMSAFEGUARDEXPIRED - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("{B_ATK_TEAM_PREFIX} is no longer protected by Safeguard!"),
	[STRINGID_ATKTEAMBUFF1WOREOFF - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_ATK_TEAM_PREFIX}'s {B_BUFF1} wore off!"),
	[STRINGID_DEFWENTTOSLEEP - BATTLESTRINGS_ID_ADDER]                = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} went to sleep!"),
	[STRINGID_DEFSLEPTHEALTHY - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} slept and became healthy!"),
	[STRINGID_BUTITFAILED - BATTLESTRINGS_ID_ADDER]                   = COMPOUND_STRING("But it failed!"),
	[STRINGID_ATKWHIPPEDWHIRLWIND - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} whipped up a whirlwind!"),
	[STRINGID_ATKBECAMECLOAKEDINSUNLIGHT - BATTLESTRINGS_ID_ADDER]    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} became cloaked in a harsh light!"),
	[STRINGID_ATKBURROWEDUNDERTHEGROUND - BATTLESTRINGS_ID_ADDER]     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} burrowed its way under the ground!"),
	[STRINGID_ATKTUCKEDINHEAD - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} tucked in its head!"),
	[STRINGID_ATKSPRANGUP - BATTLESTRINGS_ID_ADDER]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} sprang up!"),
	[STRINGID_ATKHIDUNDERWATER - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} hid underwater!"),
	[STRINGID_ATKFLEWHIGH - BATTLESTRINGS_ID_ADDER]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} flew up high!"),
	[STRINGID_ATKABSORBEDLIGHT - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} absorbed light!"),
	[STRINGID_EFFSQUEEZEDBYPKMN - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} was squeezed by {B_SCR_ACTIVE_NAME_WITH_PREFIX}!"),
	[STRINGID_EFFWRAPPEDBYPKMN - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} was wrapped by {B_SCR_ACTIVE_NAME_WITH_PREFIX}!"),
	[STRINGID_EFFTRAPPEDINVORTEX - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} became trapped in the vortex!"),
	[STRINGID_PKMNCLAMPEDEFF - BATTLESTRINGS_ID_ADDER]                = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} clamped {B_EFF_NAME_WITH_PREFIX}!"),
	[STRINGID_EFFTRAPPEDBYCURRMOVE - BATTLESTRINGS_ID_ADDER]          = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} became trapped by {B_CURRENT_MOVE}!"),
	[STRINGID_ATKHURTBYBUFF1 - BATTLESTRINGS_ID_ADDER]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is hurt by {B_BUFF1}!"),
	[STRINGID_ATKFREEDFROMBUFF1 - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} was freed from {B_BUFF1}!"),
	[STRINGID_ATKKEPTANDCRASHED - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} kept going and crashed!"),
	[STRINGID_ATKTEAMSHROUDEDINMIST - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("{B_ATK_TEAM_PREFIX} became shrouded in mist!"),
	[STRINGID_ATKGETTINGPUMPED - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is getting pumped!"),
	[STRINGID_ATKHITWITHRECOIL - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is hit with recoil!"),
	[STRINGID_ATKBUFFETEDBYSANDSTORM - BATTLESTRINGS_ID_ADDER]        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is buffeted by the sandstorm!"),
	[STRINGID_ATKPELTEDBYHAIL - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is pelted by hail!"),
	[STRINGID_DEFWASSEEDED - BATTLESTRINGS_ID_ADDER]                  = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} was seeded!"),
	[STRINGID_DEFEVADEDATTACK - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} evaded the attack!"),
	[STRINGID_DEFSAPPEDBYLEECHSEED - BATTLESTRINGS_ID_ADDER]          = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s health is sapped by Leech Seed!"),
	[STRINGID_ATKFASTASLEEP - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is fast asleep."),
	[STRINGID_ATKWOKEUP - BATTLESTRINGS_ID_ADDER]                     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} woke up!"),
	[STRINGID_ATKWOKEUPINUPROAR - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} woke up in the uproar!"),
	[STRINGID_DEFENDUREDHIT - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} endured the hit!"),
	[STRINGID_ONEHITKO - BATTLESTRINGS_ID_ADDER]                      = COMPOUND_STRING("It's a one-hit KO!"),
	[STRINGID_ATKSTORINGENERGY - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is storing energy!"),
	[STRINGID_ATKUNLEASHEDENERGY - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} unleashed energy!"),
	[STRINGID_FIZZLESBUFF1TYPEATTACK - BATTLESTRINGS_ID_ADDER]        = COMPOUND_STRING("The {B_BUFF1}-type attack fizzled out in the heavy rain!"),
	[STRINGID_EVAPORATEBUFF1TYPEATTACK - BATTLESTRINGS_ID_ADDER]      = COMPOUND_STRING("The {B_BUFF1}-type attack evaporated in the harsh sunlight!"),
	[STRINGID_ATKFATIGUECONFUSION - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} became confused due to fatigue!"),
	[STRINGID_PLAYERPICKEDUPMONEY - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_PLAYER_NAME} picked up ¥{B_BUFF1}!\p"),
	[STRINGID_DEFUNAFFECTED - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} is unaffected!"),
	[STRINGID_ATKTRANSFORMEDINTOBUFF1 - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} transformed into {B_BUFF1}!"),
	[STRINGID_ATKMADESUBSTITUTE - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} put in a substitute!"),
	[STRINGID_ATKHASSUBSTITUTE - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} already has a substitute!"),
	[STRINGID_TOOWEAKFORSUBSTITUTE - BATTLESTRINGS_ID_ADDER]          = COMPOUND_STRING("It was too weak to make a substitute!"),
	[STRINGID_SUBSTITUTEDAMAGED - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("The substitute took damage for {B_DEF_NAME_WITH_PREFIX}!\p"),
	[STRINGID_DEFSUBSTITUTEFADED - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s substitute faded!\p"),
	[STRINGID_ATKMUSTRECHARGE - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} must recharge!"),
	[STRINGID_ATKTOOKAIMATDEF - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} took aim at {B_DEF_NAME_WITH_PREFIX}!"),
	[STRINGID_DEFBUFF1WASDISABLED - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1} was disabled!"),
	[STRINGID_ATKCURRMOVEISDISABLED - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s {B_CURRENT_MOVE} is disabled!"),
	[STRINGID_ATKDISABLEDNOMORE - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is disabled no more!"),
	[STRINGID_DEFRECEIVEDENCORE - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} received an encore!"),
	[STRINGID_ATKENCOREENDED - BATTLESTRINGS_ID_ADDER]                = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s encore ended!"),
	[STRINGID_ATKTRYINGTOTAKEFOEDOWN - BATTLESTRINGS_ID_ADDER]        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is trying to take its foe down with it!"),
	[STRINGID_DEFTOOKATTACKERWITHIT - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} took its attacker down with it!"),
	[STRINGID_REDUCEDPPOFDEFBUFF1BYBUFF2 - BATTLESTRINGS_ID_ADDER]    = COMPOUND_STRING("It reduced the PP of the {B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1} by {B_BUFF2}!"),
	[STRINGID_ATKSTOLEDEFLASTITEM - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} stole {B_DEF_NAME_WITH_PREFIX}'s {B_LAST_ITEM}!"),
	[STRINGID_DEFCANNOLONGERESCAPE - BATTLESTRINGS_ID_ADDER]          = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} can no longer escape!"),
	[STRINGID_DEFBEGANHAVINGNIGHTMARE - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} began having a nightmare!"),
	[STRINGID_ATKLOCKEDINNIGHTMARE - BATTLESTRINGS_ID_ADDER]          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is locked in a nightmare!"),
	[STRINGID_SPIKESSCATTERED - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("Spikes were scattered all around the feet of the foe's team!"),
	[STRINGID_DEFWASIDENTIFIED - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} was identified!"),
	[STRINGID_ATKPERISHCOUNTFELLTOBUFF1 - BATTLESTRINGS_ID_ADDER]     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s perish count fell to {B_BUFF1}!"),
	[STRINGID_ATKBRACEDITSELF - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} braced itself!"),
	[STRINGID_MAGNITUDEBUFF1 - BATTLESTRINGS_ID_ADDER]                = COMPOUND_STRING("Magnitude {B_BUFF1}!"),
	[STRINGID_PKMNCOPIEDBUFF1STATCHANGES - BATTLESTRINGS_ID_ADDER]    = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} copied {B_BUFF1}'s stat changes!"),
	[STRINGID_ATKFLEDFROMBATTLE - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} fled from battle!"),
	[STRINGID_ATKFORESAWATTACK - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} foresaw an attack!"),
	[STRINGID_ATKCHOSECURRMOVEASDESTINY - BATTLESTRINGS_ID_ADDER]     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} chose {B_CURRENT_MOVE} as its destiny!"),
	[STRINGID_DEFTOOKTHECURRMOVEATTACK - BATTLESTRINGS_ID_ADDER]      = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} took the {B_CURRENT_MOVE} attack!"),
	[STRINGID_ATKBECAMETHECENTERATTENTION - BATTLESTRINGS_ID_ADDER]   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} became the center of attention!"),
	[STRINGID_BUFF1TURNEDINTOCURRMOVE - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("{B_BUFF1} turned into {B_CURRENT_MOVE}!"),
    [STRINGID_ATKHASNOMOVESLEFT - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} has no moves left!"),
    [STRINGID_DEFSUBJECTEDTOTORMENT - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} was subjected to torment!"),
    [STRINGID_ATKCANTUSEMOVETORMENT - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} can't use the same move in a row due to the torment!"),
    [STRINGID_ATKTIGHTENINGFOCUS - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is tightening its focus!"),
    [STRINGID_DEFFELLFORTAUNT - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} fell for the taunt!"),
    [STRINGID_ATKCANTUSECURRMOVETAUNT - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} can't use {B_CURRENT_MOVE} after the taunt!"),
    [STRINGID_ATKREADYTOHELPDEF - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is ready to help {B_DEF_NAME_WITH_PREFIX}!"),
    [STRINGID_ATKCOPIEDDEFABL - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} copied the {B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}!"),
    [STRINGID_BUFF1WISHCAMETRUE - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_BUFF1}'s wish came true!"),
    [STRINGID_ATKPLANTEDROOTS - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} planted its roots!"),
    [STRINGID_ATKABSORBEDNUTRIENTS - BATTLESTRINGS_ID_ADDER]          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} absorbed nutrients with its roots!"),
    [STRINGID_DEFANCHOREDITSELF - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} anchored itself with its roots!"),
    [STRINGID_DEFGREWDROWSY - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} grew drowsy!"),
    [STRINGID_ATKSWAPPEDABILITIESWITHTARGET - BATTLESTRINGS_ID_ADDER] = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} swapped Abilities with its target!"),
	[STRINGID_DEFBOUNCEDCURRMOVEBACK - BATTLESTRINGS_ID_ADDER]        = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} bounced the {B_CURRENT_MOVE} back!"),
	[STRINGID_ATKSHROUDEDWITHCURRMOVE - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} shrouded itself with {B_CURRENT_MOVE}!"),
	[STRINGID_CRITICALHIT - BATTLESTRINGS_ID_ADDER]                   = COMPOUND_STRING("{COLOR 2}{SHADOW 9}A critical hit!"),
	[STRINGID_CRITICALHITONDEF - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{COLOR 2}{SHADOW 9}A critical hit{COLOR 1}{SHADOW 6} on {B_DEF_NAME_WITH_PREFIX}!"),
    [STRINGID_123POOF - BATTLESTRINGS_ID_ADDER]                       = COMPOUND_STRING("{PAUSE 32}1, {PAUSE 15}2, and{PAUSE 15}… {PAUSE 15}… {PAUSE 15}… {PAUSE 15}{PLAY_SE SE_BALL_BOUNCE_1}Poof!\p"),
    [STRINGID_ANDELLIPSIS - BATTLESTRINGS_ID_ADDER]                   = COMPOUND_STRING("And…\p"),
    [STRINGID_NOTVERYEFFECTIVE - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("It's not very effective…"),
	[STRINGID_NOTVERYEFFECTIVEONDEF - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("It's not very effective on {B_DEF_NAME_WITH_PREFIX}…"),
    [STRINGID_SUPEREFFECTIVE - BATTLESTRINGS_ID_ADDER]                = COMPOUND_STRING("It's super effective!"),
	[STRINGID_SUPEREFFECTIVEONDEF - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("It's super effective on {B_DEF_NAME_WITH_PREFIX}!"),
	[STRINGID_ATKISLOAFINGAROUND - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is loafing around!"),
	[STRINGID_ATKWONTOBEY - BATTLESTRINGS_ID_ADDER]                   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} won't obey!"),
	[STRINGID_ATKTURNEDAWAY - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} turned away!"),
	[STRINGID_ATKPRETENDNOTNOTICE - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} pretended not to notice!"),
	[STRINGID_BUFF1ISNERVOUSTOEATBERRIES - BATTLESTRINGS_ID_ADDER]    = COMPOUND_STRING("{B_BUFF1} is too nervous to eat berries!"),
	[STRINGID_EFFPREVENTSPOISONINGWITH - BATTLESTRINGS_ID_ADDER]      = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}'s {B_EFF_ABILITY} prevents poisoning!"),
	[STRINGID_EMPTYSTRING - BATTLESTRINGS_ID_ADDER]                   = gText_EmptyString,
	[STRINGID_ABOOSTED - BATTLESTRINGS_ID_ADDER]                      = COMPOUND_STRING(" a boosted"),
	[STRINGID_STARTEDTORAIN - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("It started to rain!"),
	[STRINGID_GOTAWAYSAFELY - BATTLESTRINGS_ID_ADDER]                 = sText_GotAwaySafely,
    [STRINGID_WILDBUFF1FLED - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("{PLAY_SE SE_FLEE}Wild {B_BUFF1} fled!"),
    [STRINGID_DEFPROTECTEDBYDEFABL - BATTLESTRINGS_ID_ADDER]          = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} was protected by {B_DEF_ABILITY}!"),
    [STRINGID_PKMNABLPREVENTSATKUSECURRMOVE - BATTLESTRINGS_ID_ADDER] = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY} prevents {B_ATK_NAME_WITH_PREFIX} from using {B_CURRENT_MOVE}!"),
	[STRINGID_BUTNOTHINGHAPPENED - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("But nothing happened!"),
    [STRINGID_ITHURTITSELFINCONFUSION - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("It hurt itself in its confusion!"),
    [STRINGID_THECURRMOVEFAILED - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("The {B_CURRENT_MOVE} failed!"),
    [STRINGID_DEFHADHPRESTORED - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} had its HP restored."),
	[STRINGID_POWEROFDEFBUFF1TYPEMOVESROSE - BATTLESTRINGS_ID_ADDER]  = COMPOUND_STRING("The power of {B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1}-type moves rose!"),
	[STRINGID_ITTRACEDDEFABL - BATTLESTRINGS_ID_ADDER]                = COMPOUND_STRING("It traced {B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}!"),
	[STRINGID_ATKWASHURT - BATTLESTRINGS_ID_ADDER]                    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} was hurt!"),
	[STRINGID_MONHEARINGFLUTEAWOKE - BATTLESTRINGS_ID_ADDER]          = COMPOUND_STRING("The Pokémon hearing the flute awoke!"),
	[STRINGID_GOTCHADEFCAUGHT - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("Gotcha!\n{B_DEF_NAME} was caught!{WAIT_SE}{PLAY_BGM MUS_CAUGHT}\p"),
	[STRINGID_DEFDATAADDEDTODEX - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_DEF_NAME}'s data was added to the Pokédex.\p"),
	[STRINGID_PKMNTRANSFERREDSOMEONESPC - BATTLESTRINGS_ID_ADDER]     = Text_MonSentToBoxInSomeonesPC,
    [STRINGID_PKMNTRANSFERREDBILLSPC - BATTLESTRINGS_ID_ADDER]        = Text_MonSentToBoxInBillsPC,
    [STRINGID_PKMNBOXSOMEONESPCFULL - BATTLESTRINGS_ID_ADDER]         = Text_MonSentToBoxSomeonesBoxFull,
    [STRINGID_PKMNBOXBILLSPCFULL - BATTLESTRINGS_ID_ADDER]            = Text_MonSentToBoxBillsBoxFull,
	[STRINGID_OUTOFSAFARIBALLS - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{PLAY_SE SE_DING_DONG}Announcer: You're out of Safari Balls! Game over!\p"),
	[STRINGID_POKEMONBROKEFREE - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("Oh, no!\nThe Pokémon broke free!"),
	[STRINGID_ITAPPEAREDCAUGHT - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("Aww!\nIt appeared to be caught!"),
    [STRINGID_AARGHALMOSTHADIT - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("Aargh!\nAlmost had it!"),
    [STRINGID_SHOOTSOCLOSE - BATTLESTRINGS_ID_ADDER]                  = COMPOUND_STRING("Shoot!\nIt was so close, too!"),
	[STRINGID_TRAINERBLOCKEDBALL - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("The Trainer blocked the Ball!"),
    [STRINGID_DONTBEATHIEF - BATTLESTRINGS_ID_ADDER]                  = COMPOUND_STRING("Don't be a thief!"),
	[STRINGID_ITDODGEDBALL - BATTLESTRINGS_ID_ADDER]                  = COMPOUND_STRING("It dodged the thrown Ball!\nThis Pokémon can't be caught!"),
	[STRINGID_PKMNWATCHINGCAREFULLY - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("{B_OPPONENT_MON1_NAME} is watching carefully!"),
    [STRINGID_PKMNANGRY - BATTLESTRINGS_ID_ADDER]                     = COMPOUND_STRING("{B_OPPONENT_MON1_NAME} is angry!"),
    [STRINGID_PKMNEATING - BATTLESTRINGS_ID_ADDER]                    = COMPOUND_STRING("{B_OPPONENT_MON1_NAME} is eating!"),
	[STRINGID_THREWROCK - BATTLESTRINGS_ID_ADDER]                     = COMPOUND_STRING("{B_PLAYER_NAME} threw a Rock at the {B_OPPONENT_MON1_NAME}!"),
    [STRINGID_THREWBAIT - BATTLESTRINGS_ID_ADDER]                     = COMPOUND_STRING("{B_PLAYER_NAME} threw some Bait at the {B_OPPONENT_MON1_NAME}!"),
	[STRINGID_YOUTHROWABALLNOWRIGHT - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("You throw a Ball now, right?\nI… I'll do my best!"),
    [STRINGID_NORUNNINGFROMTRAINERS - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("No! There's no running from a Trainer battle!\p"),
    [STRINGID_CANTESCAPE - BATTLESTRINGS_ID_ADDER]                    = COMPOUND_STRING("Can't escape!"),
    [STRINGID_DONTLEAVEBIRCH - BATTLESTRINGS_ID_ADDER]                = gText_EmptyString, // Handled by oak can't escape string
	[STRINGID_ATKCANTESCAPE - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} can't escape!"),
	[STRINGID_RAINCONTINUES - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("Rain continues to fall."),
	[STRINGID_RAINSTOPPED - BATTLESTRINGS_ID_ADDER]                   = COMPOUND_STRING("The rain stopped."),
	[STRINGID_SUNLIGHTSTRONG - BATTLESTRINGS_ID_ADDER]                = COMPOUND_STRING("The sunlight is strong."),
    [STRINGID_SUNLIGHTFADED - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("The sunlight faded."),
	[STRINGID_SANDSTORMRAGES - BATTLESTRINGS_ID_ADDER]                = COMPOUND_STRING("The sandstorm rages."),
    [STRINGID_SANDSTORMSUBSIDED - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("The sandstorm subsided."),
	[STRINGID_HAILCONTINUES - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("Hail continues to fall."),
    [STRINGID_HAILSTOPPED - BATTLESTRINGS_ID_ADDER]                   = COMPOUND_STRING("The hail stopped."),
	[STRINGID_DEEPFOG - BATTLESTRINGS_ID_ADDER]                       = COMPOUND_STRING("The fog is deep…"),
	[STRINGID_FOGENDED - BATTLESTRINGS_ID_ADDER]                      = COMPOUND_STRING("The fog disappeared."),
	[STRINGID_STRONGWINDSCONTINUES - BATTLESTRINGS_ID_ADDER]          = COMPOUND_STRING("The mysterious strong winds continues to blow!"),
	[STRINGID_ASANDSTORMKICKEDUP - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("A sandstorm kicked up!"),
	[STRINGID_SUNLIGHTTURNEDHARSH - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("The sunlight turned harsh!"),
	[STRINGID_STARTEDHAIL - BATTLESTRINGS_ID_ADDER]                   = COMPOUND_STRING("It started to hail!"),
	[STRINGID_COINSSCATTERED - BATTLESTRINGS_ID_ADDER]                = COMPOUND_STRING("Coins scattered everywhere!"),
	[STRINGID_GIVENICKNAMECAPTURED - BATTLESTRINGS_ID_ADDER]          = COMPOUND_STRING("Give a nickname to the captured {B_DEF_NAME}?"),
	[STRINGID_BATTLERSSHAREDTHEIRPAIN - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("The battlers shared their pain!"),
	[STRINGID_BELLCHIMED - BATTLESTRINGS_ID_ADDER]                    = COMPOUND_STRING("A bell chimed!"),
	[STRINGID_SOOTHINGAROMA - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("A soothing aroma wafted through the area!"),
	[STRINGID_STATCHANGESELIMINATED - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("All stat changes were eliminated!"),
    [STRINGID_ALLPOKEMONFAINTINTHREETURNS - BATTLESTRINGS_ID_ADDER]   = COMPOUND_STRING("All Pokémon hearing the song will faint in three turns!"),
    [STRINGID_NOPPLEFT - BATTLESTRINGS_ID_ADDER]                      = COMPOUND_STRING("There's no PP left for this move!"),
    [STRINGID_BUTNOPPLEFT - BATTLESTRINGS_ID_ADDER]                   = COMPOUND_STRING("But there was no PP left for the move!"),
    [STRINGID_ATKIGNOREDORDERSASLEEP - BATTLESTRINGS_ID_ADDER]        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} ignored orders while asleep!"),
    [STRINGID_ATKIGNOREDORDERS - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} ignored orders!"),
    [STRINGID_ATKBEGANTONAP - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} began to nap!"),
    [STRINGID_ENEMYABOUTTOSWITCHPKMN - BATTLESTRINGS_ID_ADDER]        = COMPOUND_STRING("{B_TRAINER1_CLASS} {B_TRAINER1_NAME} is\nabout to use {B_BUFF2}.\pWill {B_PLAYER_NAME} change Pokémon?"),
    [STRINGID_LASTITEMALLOWSONLYCURRMOVE - BATTLESTRINGS_ID_ADDER]    = COMPOUND_STRING("{B_LAST_ITEM}'s effect allows only {B_CURRENT_MOVE} to be used!"),
	[STRINGID_ITEMSCANTBEUSEDNOW - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("Items can't be used now.{PAUSE 64}"),
	[STRINGID_BOXISFULL - BATTLESTRINGS_ID_ADDER]                     = COMPOUND_STRING("The Box is full!\nYou can't catch any more!"),
	[STRINGID_EFFSTATCHANGED - BATTLESTRINGS_ID_ADDER]                = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}'s {B_BUFF1}{B_BUFF2}!"),
	[STRINGID_EFFSTATCANTCHANGE - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}'s {B_BUFF1} {B_BUFF2}!"),
	[STRINGID_DEFSTATSWONTCHANGE - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s stats won't change anymore!"),
	[STRINGID_EFFCUTHPMAXEDBUFF1 - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} cut its own HP and maximized {B_BUFF1}!"),
	[STRINGID_ATKGOTFREEOFPKMNBUFF1 - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} got free of {B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_BUFF1}!"),
	[STRINGID_ATKSHEDLEECHSEED - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} shed Leech Seed!"),
	[STRINGID_ATKBLEWAWAYHAZARDS - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} blew away its hazards!"),
	[STRINGID_ATKBEGANCHARGINGPOWER - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} began charging power!"),
	[STRINGID_EFFPROTECTEDBYMIST - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} is protected by Mist!"),
	[STRINGID_EFFPREVENTSSTATLOSS - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} prevents stat loss!"),
	[STRINGID_EFFPREVENTSBUFF1LOSS - BATTLESTRINGS_ID_ADDER]          = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} prevents {B_BUFF1} loss!"),
	[STRINGID_EFFRAGEBUILDING - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}'s rage is building!"),
	[STRINGID_DEFPREVENTEDFROMWORKING - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY} prevented {B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY} from working!"),
	[STRINGID_EFFMAXEDSTAT - BATTLESTRINGS_ID_ADDER]                  = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} {B_BUFF2} its {B_BUFF1}!"),
	[STRINGID_DEFALREADYHASBURN - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} already has a burn."),
	[STRINGID_PKMNABLMADEITINEFFECTIVE - BATTLESTRINGS_ID_ADDER]      = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY} made it ineffective!"),
	[STRINGID_ATKSTOCKPILED - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} stockpiled {B_BUFF1}!"),
    [STRINGID_ATKCANTSTOCKPILE - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} can't stockpile any more!"),
	[STRINGID_BUTFAILEDTOCURRMOVEATHING - BATTLESTRINGS_ID_ADDER]     = COMPOUND_STRING("But it failed to {B_CURRENT_MOVE} a thing!"),
	[STRINGID_ELECTRICITYPOWERWEAKENED - BATTLESTRINGS_ID_ADDER]      = COMPOUND_STRING("Electricity's power was weakened!"),
	[STRINGID_FIREPOWERWEAKENED - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("Fire's power was weakened!"),
	[STRINGID_BUFF1EFFECTSWOREOFF - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_BUFF1}'s effects wore off!"),
	[STRINGID_DEFTOOKTHEATTACK - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} took the attack!"),
	[STRINGID_USENEXTPKMN - BATTLESTRINGS_ID_ADDER]                   = COMPOUND_STRING("Use next Pokémon?"),
	[STRINGID_PLAYERDEFEATEDTRAINER1 - BATTLESTRINGS_ID_ADDER]        = sText_PlayerDefeatedLinkTrainerTrainer1,
	[STRINGID_TRAINER1MON1COMEBACK - BATTLESTRINGS_ID_ADDER]          = COMPOUND_STRING("{B_TRAINER1_NAME}: {B_OPPONENT_MON1_NAME}, come back!"),
	[STRINGID_TRAINER1WINTEXT - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_TRAINER1_WIN_TEXT}"),
	[STRINGID_PLAYERLOSTAGAINSTENEMYTRAINER - BATTLESTRINGS_ID_ADDER] = COMPOUND_STRING("{B_PLAYER_NAME} is out of usable Pokémon!\pPlayer lost against {B_TRAINER1_CLASS} {B_TRAINER1_NAME}!{PAUSE_UNTIL_PRESS}"),
	[STRINGID_PLAYERPAIDPRIZEMONEY - BATTLESTRINGS_ID_ADDER]          = COMPOUND_STRING("{B_PLAYER_NAME} paid ¥{B_BUFF1} as the prize money…\p… … … …\p{B_PLAYER_NAME} whited out!{PAUSE_UNTIL_PRESS}"),
	[STRINGID_PLAYERWHITEDOUT - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_PLAYER_NAME} whited out!{PAUSE_UNTIL_PRESS}"),
	[STRINGID_ATKLOSTITSFOCUS - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} lost its focus and couldn't move!"),
	[STRINGID_DEFWASDRAGGEDOUT - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} was dragged out!\p"),
	[STRINGID_ITSUCKEDTHEDEFABL - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("It sucked up the {B_DEF_ABILITY}!"),
	[STRINGID_THEWALLSHATTERED - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("The wall shattered!"),
	[STRINGID_ATKTOOSCAREDTOMOVE - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is too scared to move!"),
    [STRINGID_GHOSTGETOUTGETOUT - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("Ghost: Get out…… Get out……"),
    [STRINGID_LASTITEMUNVEILEDGHOST - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("{B_LAST_ITEM} unveiled the Ghost's identity!"),
    [STRINGID_THEGHOSTWASDEF - BATTLESTRINGS_ID_ADDER]                = COMPOUND_STRING("The Ghost was {B_DEF_NAME}!\p\n"),
	[STRINGID_DEFPROTECTEDBYAROMATICVEIL - BATTLESTRINGS_ID_ADDER]    = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} is protected by an aromatic veil!"),
	[STRINGID_EFFSURROUNDEDINVEILOFPETALS - BATTLESTRINGS_ID_ADDER]   = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} surrounded itself with a veil of petals!"),
	[STRINGID_DEFSURROUNDEDINSWEETVEIL - BATTLESTRINGS_ID_ADDER]      = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} surrounded itself with a veil of sweetness!"),
	[STRINGID_HEAVYRAINLIFTED - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("The heavy rain has lifted!{PAUSE 64}"),
	[STRINGID_EXTREMESUNLIGHTFADED - BATTLESTRINGS_ID_ADDER]          = COMPOUND_STRING("The extreme sunlight faded.{PAUSE 64}"),
	[STRINGID_STRONGWINDSDISSIPATED - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("The mysterious strong winds have dissipated!{PAUSE 64}"),
	[STRINGID_BUTNOEFFECT - BATTLESTRINGS_ID_ADDER]                   = COMPOUND_STRING("But it had no effect!"),
    [STRINGID_DEFHUNGONUSINGLASTITEM - BATTLESTRINGS_ID_ADDER]        = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} hung on using its {B_LAST_ITEM}!"),
    [STRINGID_PKMNABLBLOCKSCURRMOVE - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY} blocks {B_CURRENT_MOVE}!"),
    [STRINGID_ATKCUTHPANDLAIDCURSEONDEF - BATTLESTRINGS_ID_ADDER]     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} cut its own HP and laid a curse on {B_DEF_NAME_WITH_PREFIX}!"),
    [STRINGID_ATKAFFLICTEDBYCURSE - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} is afflicted by the curse!"),
	[STRINGID_DEFSNATCHEDPKMNMOVE - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} snatched {B_SCR_ACTIVE_NAME_WITH_PREFIX}'s move!"),
	[STRINGID_WEATHEREFFECTSDISAPPEARED - BATTLESTRINGS_ID_ADDER]     = COMPOUND_STRING("The effects of weather disappeared!"),
	[STRINGID_PKMNBREAKSTHEMOLD - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} breaks the mold!"),
	[STRINGID_PKMNCANTGETGOING - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} can't get it going!"),
	[STRINGID_PKMNRADIATINGABLAZINGAURA - BATTLESTRINGS_ID_ADDER]     = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} is radiating a blazing aura!"),
	[STRINGID_PKMNRADIATINGABURSTINGAURA - BATTLESTRINGS_ID_ADDER]    = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} is radiating a bursting aura!"),
	[STRINGID_PKMNRADIATINGADARKAURA - BATTLESTRINGS_ID_ADDER]        = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} is radiating a dark aura!"),
	[STRINGID_PKMNRADIATINGAFAIRYAURA - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} is radiating a fairy aura!"),
	[STRINGID_PKMNREVERSEDALLOTHERAURAS - BATTLESTRINGS_ID_ADDER]     = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} reversed all other Pokémon's auras!"),
	[STRINGID_PKMNISDROWSING - BATTLESTRINGS_ID_ADDER]                = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} is drowsing!"),
	[STRINGID_PKMNUNDERWENTTRANSFORMATION - BATTLESTRINGS_ID_ADDER]   = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} underwent a heroic transformation!"),
	[STRINGID_PKMNSHUDDERED - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} shuddered!"),
	[STRINGID_ALERTEDTOPKMNBUFF1 - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("It was alerted to {B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_BUFF1}!"),
	[STRINGID_ALLSCREENSCLEANSED - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("All screens on the field were cleansed!"),
	[STRINGID_PKMNSTATCHANGESWERERESET - BATTLESTRINGS_ID_ADDER]      = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s stat changes were reset!"),
	[STRINGID_PKMNHASTWOABILITIES - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} has two Abilities!"),
	[STRINGID_PKMNEXERTINGITSABL - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} is exerting its {B_SCR_ACTIVE_ABILITY}!"),
	[STRINGID_PKMNGAINEDSTRENGTHFROMFALLEN - BATTLESTRINGS_ID_ADDER]  = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} gained strength from the fallen!"),
	[STRINGID_PKMNABLWEAKENEDBUFF1OFALL - BATTLESTRINGS_ID_ADDER]     = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY} weakened the {B_BUFF1} of all surrounding Pokémon!"),
	[STRINGID_PKMNILLUSIONOFF - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s illusion wore off!"),
	[STRINGID_ATKABILITYBECAMEABL - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s Ability became {B_ATK_ABILITY}!"),
	[STRINGID_ATKGOTITSACTTOGETHER - BATTLESTRINGS_ID_ADDER]          = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} got its act together!"),
	[STRINGID_DEFISTORMENTED - BATTLESTRINGS_ID_ADDER]                = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} is tormented!"),
	[STRINGID_DISGUISESERVEDASDECOY - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("Its disguise served it as a decoy!"),
	[STRINGID_DEFDISGUISEBUSTED - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s disguise was busted!"),
	[STRINGID_GRAVITYINTENSIFIED - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("Gravity intensified!"),
	[STRINGID_PKMNCANTSTAYINAIR - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} can't stay airbone because of gravity!"),
	[STRINGID_ATKCANTUSECURRMOVEINGRAVITY - BATTLESTRINGS_ID_ADDER]   = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} can't use {B_CURRENT_MOVE} because of gravity!"),
	[STRINGID_GRAVITYRETURNEDTONORMAL - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("Gravity returned to normal!"),
	[STRINGID_MOVECANTBESELECTED - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("{B_BUFF1}\p"),
	[STRINGID_ATKCANTUSESEALEDCURRMOVE - BATTLESTRINGS_ID_ADDER]      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} can't use the sealed {B_CURRENT_MOVE}!"),
	[STRINGID_HEALINGWISHCAMETRUEFORPKMN - BATTLESTRINGS_ID_ADDER]    = COMPOUND_STRING("The healing wish came true for {B_SCR_ACTIVE_NAME_WITH_PREFIX}!"),
	[STRINGID_PKMNHURTBYSPIKES - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} is hurt by spikes!"),
	[STRINGID_EFFFELLFORTHEFEINT - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX} fell for the feint!"),
	[STRINGID_TAILWINDBLEWBEHINDATKTEAM - BATTLESTRINGS_ID_ADDER]     = COMPOUND_STRING("The tailwind blew from behind {B_ATK_TEAM_PREFIX}!"),
	[STRINGID_ATKTEAMTAILWINDPETEREDOUT - BATTLESTRINGS_ID_ADDER]     = COMPOUND_STRING("{B_ATK_TEAM_PREFIX}'s tailwind petered out!"),
	[STRINGID_CURRMOVECHARGEDPKMNPOWER - BATTLESTRINGS_ID_ADDER]      = COMPOUND_STRING("Being hit by {B_CURRENT_MOVE} charged {B_SCR_ACTIVE_NAME_WITH_PREFIX} with power!"),
	[STRINGID_DEFCANTUSEITEMSANYMORE - BATTLESTRINGS_ID_ADDER]        = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} can't use items anymore!"),
	[STRINGID_ATKCANUSEITEMSAGAIN - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} can use items again!"),
	[STRINGID_ATKBUFF1WASHEALED - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s {B_BUFF1} was healed."),
	[STRINGID_ATKABLALLOWSONLYCURRMOVE - BATTLESTRINGS_ID_ADDER]      = COMPOUND_STRING("{B_ATK_ABILITY} allows only {B_CURRENT_MOVE} to be used!"),
	[STRINGID_ATKBECAMECHARGEDDUETOITSBOND - BATTLESTRINGS_ID_ADDER]  = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} became fully charged due to its bond with its Trainer!"),
	[STRINGID_PKMNABLTRIGGERED - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_SCR_ACTIVE_ABILITY} triggered!"),
	[STRINGID_PKMNFORMEDASCHOOL - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} formed a school!"),
	[STRINGID_PKMNSTOPPEDSCHOOLING - BATTLESTRINGS_ID_ADDER]          = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} stopped schooling!"),
	[STRINGID_PKMNABLACTIVATED - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_SCR_ACTIVE_ABILITY} activated!"),
	[STRINGID_PKMNABLDEACTIVATED - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("{B_SCR_ACTIVE_ABILITY} deactivated!"),
	[STRINGID_PKMNTRANSFORMED - BATTLESTRINGS_ID_ADDER]               = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} transformed!"),
	[STRINGID_CHANGEDTOBLADEFORME - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("Changed to Blade Forme!"),
	[STRINGID_CHANGEDTOSHIELDFORME - BATTLESTRINGS_ID_ADDER]          = COMPOUND_STRING("Changed to Shield Forme!"),
	[STRINGID_ATKBECAMEASHBUFF1 - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} became Ash-{B_BUFF1}!"),
	[STRINGID_DEFPREVENTEDFROMHEALING - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} was prevented from healing!"),
	[STRINGID_ATKHEALBLOCKWOREOFF - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s heal block wore off!"),
	[STRINGID_ATKPREVENTEDFROMHEALING - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} was prevented from healing!"),
	[STRINGID_ATKSWITCHEDBUFF1ANDBUFF2 - BATTLESTRINGS_ID_ADDER]      = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} switched its {B_BUFF1} and {B_BUFF2}!"),
	[STRINGID_DEFABILITYWASSUPPRESSED - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s Ability was suppressed!"),
	[STRINGID_CURRMOVESHIELDEDTEAMFROMCRITS - BATTLESTRINGS_ID_ADDER] = COMPOUND_STRING("The {B_CURRENT_MOVE} shielded {B_ATK_TEAM_PREFIX} from critical hits!"),
	[STRINGID_HEAVYRAINFALL - BATTLESTRINGS_ID_ADDER]                 = COMPOUND_STRING("A heavy rain began to fall!"),
	[STRINGID_SUNLIGHTTURNEDEXTREMELYHARSH - BATTLESTRINGS_ID_ADDER]  = COMPOUND_STRING("The sunlight turned extremely harsh!"),
	[STRINGID_STRONGWINDSPROTECTBUFF1TYPE - BATTLESTRINGS_ID_ADDER]   = COMPOUND_STRING("Mysterious strong winds are protecting {B_BUFF1}-type Pokémon!"),
	[STRINGID_ATKSEALEDOPPONENTSMOVE - BATTLESTRINGS_ID_ADDER]        = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} sealed the opponent's move(s)!"),
	[STRINGID_ATKWANTSTARGETTOBEARGRUDGE - BATTLESTRINGS_ID_ADDER]    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} wants its target to bear a grudge!"),
	[STRINGID_ATKBUFF1LOSTPPDUETOGRUDGE - BATTLESTRINGS_ID_ADDER]     = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s {B_BUFF1} lost all its PP due to the grudge!"),
	[STRINGID_ATKWAITSFORTARGETTOMOVE - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} waits for a target to make a move!"),
	[STRINGID_DEFABLDISABLEDATKCURRMOVE - BATTLESTRINGS_ID_ADDER]     = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY} disabled {B_ATK_NAME_WITH_PREFIX}'s {B_CURRENT_MOVE}!"),
	[STRINGID_ATKFRISKEDDEFFOUNDLASTITEM - BATTLESTRINGS_ID_ADDER]    = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} frisked {B_DEF_NAME_WITH_PREFIX} and found its {B_LAST_ITEM}!"),
	[STRINGID_BOTHPOKEMONPERISHINTHREETURNS - BATTLESTRINGS_ID_ADDER] = COMPOUND_STRING("Both Pokémon will perish in three turns!"),
	[STRINGID_ATKABLFILLEDTHEAREA - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_ATK_ABILITY} filled the area!"),
	[STRINGID_THEEFFECTSOFBUFF1WOREOFF - BATTLESTRINGS_ID_ADDER]      = COMPOUND_STRING("The effects of {B_BUFF1} wore off!"),
	[STRINGID_PKMNABLWASTAKENOVER - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY} was taken over!"),
	[STRINGID_ITEMPKMNHADHPRESTORED - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("{B_ITEM_USE_SPECIES_NAME} had its HP restored!"),
	[STRINGID_ITEMPKMNHADSTATUSHEALED - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("{B_ITEM_USE_SPECIES_NAME} had its status healed!"),
	[STRINGID_ITEMPKMNHADPPRESTORED - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("{B_ITEM_USE_SPECIES_NAME} had its PP restored!"),
	[STRINGID_DEFABLCUREDPKMNBUFF1PROBLEM - BATTLESTRINGS_ID_ADDER]   = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY} cured {B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_BUFF1} problem!"),
	[STRINGID_PKMNABLCUREDITSBUFF1PROBLEM - BATTLESTRINGS_ID_ADDER]   = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY} cured its {B_BUFF1} problem!"),
	[STRINGID_ATKHARVESTEDITSLASTITEM - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} harvested its {B_LAST_ITEM}!"),
	[STRINGID_ATKFOUNDONELASTITEM - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX}'s found one {B_LAST_ITEM}!"),
	[STRINGID_BUFF2CUREDOFITSBUFF1 - BATTLESTRINGS_ID_ADDER]          = COMPOUND_STRING("{B_BUFF2} was cured of its {B_BUFF1}."),
	[STRINGID_DEFANCHORSITSELFWITHABL - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} anchors itself with {B_DEF_ABILITY}!"),
	[STRINGID_DEFABLMADECURRMOVEINEFFECTIVE - BATTLESTRINGS_ID_ADDER] = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY} made {B_CURRENT_MOVE} ineffective!"),
	[STRINGID_DEFAVOIDEDATTACK - BATTLESTRINGS_ID_ADDER]              = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} avoided the attack!"),
	[STRINGID_EFFABLPREVENTSFLINCHING - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}'s won't flinch because of its {B_EFF_ABILITY}!"),
	[STRINGID_EFFABLPREVENTSPARALYSIS - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("{B_EFF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY} prevents paralysis!"),
    [STRINGID_DEFABLPREVENTSCONFUSION - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY} prevents confusion!"),
	[STRINGID_DEFPROTECTEDBYPASTELVEIL - BATTLESTRINGS_ID_ADDER]      = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} is protected by a pastel veil!"),
	[STRINGID_ATKLOSTSOMEOFITSHP - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} lost some of its HP!"),
	[STRINGID_ATKGAINSOMEOFITSHP - BATTLESTRINGS_ID_ADDER]            = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} gain some of its HP!"),
	[STRINGID_PKMNABLCUREDITSMENTALSTATE - BATTLESTRINGS_ID_ADDER]    = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY} cured its mental state!"),
	[STRINGID_PKMNABLCUREDITSCONFUSION - BATTLESTRINGS_ID_ADDER]      = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY} cured its confusion!"),
	[STRINGID_PKMNCUREDOFITSBUFF1 - BATTLESTRINGS_ID_ADDER]           = COMPOUND_STRING("{B_SCR_ACTIVE_NAME_WITH_PREFIX} was cured of its {B_BUFF1}!"),
	[STRINGID_ATKSWITCHBUFF1ANDBUFF2CHANGES - BATTLESTRINGS_ID_ADDER] = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} switched all changes to its {B_BUFF1} and {B_BUFF2} with the target!"),
	[STRINGID_DEFACQUIREDDEFABL - BATTLESTRINGS_ID_ADDER]             = COMPOUND_STRING("{B_DEF_NAME_WITH_PREFIX} acquired {B_DEF_ABILITY}!"),
	[STRINGID_POISONSPIKESSCATTERED - BATTLESTRINGS_ID_ADDER]         = COMPOUND_STRING("Poison spikes were scattered all around the feet of the foe's team!"),
	[STRINGID_POISONSPIKESDISAPPEARED - BATTLESTRINGS_ID_ADDER]       = COMPOUND_STRING("The poison spikes disappeared from around the feet of the foe's team!"),
	[STRINGID_BUFF1DRANKALLMATCHAPKMNMADE - BATTLESTRINGS_ID_ADDER]   = COMPOUND_STRING("{B_BUFF1} drank down all the matcha that {B_SCR_ACTIVE_NAME_WITH_PREFIX} made!"),
	
	/*
    [STRINGID_PKMNFLEDUSINGITS - BATTLESTRINGS_ID_ADDER]              = sText_PkmnFledUsingIts,
    [STRINGID_PKMNFLEDUSING - BATTLESTRINGS_ID_ADDER]                 = sText_PkmnFledUsing,
    [STRINGID_PKMNSITEMNORMALIZEDSTATUS - BATTLESTRINGS_ID_ADDER]     = sText_PkmnsItemNormalizedStatus,
    [STRINGID_PKMNSXPREVENTSYSZ - BATTLESTRINGS_ID_ADDER]             = sText_PkmnsXPreventsYsZ,
    [STRINGID_PKMNSXCUREDITSYPROBLEM - BATTLESTRINGS_ID_ADDER]        = sText_PkmnsXCuredItsYProblem,
    [STRINGID_PKMNOBTAINEDX - BATTLESTRINGS_ID_ADDER]                 = sText_PkmnObtainedX,
    [STRINGID_PKMNOBTAINEDX2 - BATTLESTRINGS_ID_ADDER]                = sText_PkmnObtainedX2,
    [STRINGID_PKMNOBTAINEDXYOBTAINEDZ - BATTLESTRINGS_ID_ADDER]       = sText_PkmnObtainedXYObtainedZ,
	[STRINGID_PKMNUPROARKEPTAWAKE - BATTLESTRINGS_ID_ADDER]           = sText_PkmnUproarKeptAwake,
    [STRINGID_PKMNCAUSEDUPROAR - BATTLESTRINGS_ID_ADDER]              = sText_PkmnCausedUproar,
    [STRINGID_PKMNMAKINGUPROAR - BATTLESTRINGS_ID_ADDER]              = sText_PkmnMakingUproar,
    [STRINGID_PKMNCALMEDDOWN - BATTLESTRINGS_ID_ADDER]                = sText_PkmnCalmedDown,
    [STRINGID_PKMNCANTSLEEPINUPROAR - BATTLESTRINGS_ID_ADDER]         = sText_PkmnCantSleepInUproar,
    [STRINGID_PKMNCANTSLEEPINUPROAR2 - BATTLESTRINGS_ID_ADDER]        = sText_PkmnCantSleepInUproar2,
    [STRINGID_UPROARKEPTPKMNAWAKE - BATTLESTRINGS_ID_ADDER]           = sText_UproarKeptPkmnAwake,
    [STRINGID_PKMNSTAYEDAWAKEUSING - BATTLESTRINGS_ID_ADDER]          = sText_PkmnStayedAwakeUsing,
	[STRINGID_PKMNSWITCHEDITEMS - BATTLESTRINGS_ID_ADDER]             = sText_PkmnSwitchedItems,
	[STRINGID_PKMNKNOCKEDOFF - BATTLESTRINGS_ID_ADDER]                = sText_PkmnKnockedOff,
    [STRINGID_PKMNCUTSATTACKWITH - BATTLESTRINGS_ID_ADDER]            = sText_PkmnCutsAttackWith,
	[STRINGID_PKMNSITEMCUREDPARALYSIS - BATTLESTRINGS_ID_ADDER]       = sText_PkmnsItemCuredParalysis,
    [STRINGID_PKMNSITEMCUREDPOISON - BATTLESTRINGS_ID_ADDER]          = sText_PkmnsItemCuredPoison,
    [STRINGID_PKMNSITEMHEALEDBURN - BATTLESTRINGS_ID_ADDER]           = sText_PkmnsItemHealedBurn,
    [STRINGID_PKMNSITEMDEFROSTEDIT - BATTLESTRINGS_ID_ADDER]          = sText_PkmnsItemDefrostedIt,
    [STRINGID_PKMNSITEMWOKEIT - BATTLESTRINGS_ID_ADDER]               = sText_PkmnsItemWokeIt,
    [STRINGID_PKMNSITEMSNAPPEDOUT - BATTLESTRINGS_ID_ADDER]           = sText_PkmnsItemSnappedOut,
    [STRINGID_PKMNSITEMCUREDPROBLEM - BATTLESTRINGS_ID_ADDER]         = sText_PkmnsItemCuredProblem,
    [STRINGID_PKMNSITEMRESTOREDHEALTH - BATTLESTRINGS_ID_ADDER]       = sText_PkmnsItemRestoredHealth,
    [STRINGID_PKMNSITEMRESTOREDPP - BATTLESTRINGS_ID_ADDER]           = sText_PkmnsItemRestoredPP,
    [STRINGID_PKMNSITEMRESTOREDSTATUS - BATTLESTRINGS_ID_ADDER]       = sText_PkmnsItemRestoredStatus,
    [STRINGID_PKMNSITEMRESTOREDHPALITTLE - BATTLESTRINGS_ID_ADDER]    = sText_PkmnsItemRestoredHPALittle,*/
};

// String buffer helpers

// Get nickname, checking for Illusion
static inline void GetBattlerNick(u8 battlerId, u8 *dst)
{
    GetMonData(GetBattlerIllusionPartyIndexPtr(battlerId), MON_DATA_NICKNAME, dst);
    StringGet_Nickname(dst);
}

// Same as above, but with the prefix
#define HANDLE_NICKNAME_STRING_CASE(battlerId)                                                         \
    if (GetBattlerSide(battlerId) != B_SIDE_PLAYER)                                                    \
    {                                                                                                  \
	    toCpy = (gBattleTypeFlags & BATTLE_TYPE_TRAINER) ? sText_FoePkmnPrefix : sText_WildPkmnPrefix; \
			                                                                                           \
        while (*toCpy != EOS)                                                                          \
        {                                                                                              \
            dst[dstId] = *toCpy;                                                                       \
            dstId++;                                                                                   \
            toCpy++;                                                                                   \
        }                                                                                              \
    }                                                                                                  \
    GetBattlerNick(battlerId, text);                                                                   \
    toCpy = text

static void IllusionNickHack(u8 battlerId, u8 partyId, u8 *dst)
{
	struct Pokemon *mon = &gEnemyParty[partyId];
	u8 id = GetPartyMonIdForIllusion(battlerId, gEnemyParty, gEnemyPartyCount, mon);
	
	if (id != PARTY_SIZE)
		GetMonData(&gEnemyParty[id], MON_DATA_NICKNAME, dst);
	else
	{
		GetMonData(mon, MON_DATA_NICKNAME, dst);
		StringGet_Nickname(dst);
	}
}

static void HandlePartyMonNickBuffer(u8 battlerId, u8 partyId, u8 *dst, bool8 activeInBattle)
{
	if (activeInBattle) // If in battle, handle Illusion nickname
		GetBattlerNick(battlerId, dst);
	else if (gBattleScripting.illusionNickHack) // for STRINGID_ENEMYABOUTTOSWITCHPKMN
	{
		gBattleScripting.illusionNickHack = FALSE;
		IllusionNickHack(battlerId, partyId, dst);
	}
	else
	{
		GetMonData(&GetBattlerParty(battlerId)[partyId], MON_DATA_NICKNAME, dst);
		StringGet_Nickname(dst);
	}
}

static void CopyTrainerStringCaseToBuffer1(void)
{
	if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
			StringCopy(gBattleTextBuff1, COMPOUND_STRING("The Poké Dude"));
		else if (gBattleTypeFlags & BATTLE_TYPE_OLD_MAN_TUTORIAL)
			StringCopy(gBattleTextBuff1, COMPOUND_STRING("The old man"));
		else
			StringCopy(gBattleTextBuff1, COMPOUND_STRING("You"));
	}
	else
		StringCopy(gBattleTextBuff1, COMPOUND_STRING("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}"));
}

// String buffers
void BufferStringBattle(u8 battlerId, u16 stringId)
{
	s32 i;
    const u8 *stringPtr = NULL;

    sBattleMsgDataPtr = (struct BattleMsgData*)(&gBattleBufferA[battlerId][4]);
    gLastUsedItem = sBattleMsgDataPtr->lastItem;
    gBattleScripting.battler = sBattleMsgDataPtr->scrActive;
    gBattleStruct->hpScale = sBattleMsgDataPtr->hpScale;

    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
        sBattlerAbilities[i] = sBattleMsgDataPtr->abilities[i];

    for (i = 0; i < TEXT_BUFF_ARRAY_COUNT; i++)
	{
		gBattleTextBuff1[i] = sBattleMsgDataPtr->textBuffs[0][i];
		gBattleTextBuff2[i] = sBattleMsgDataPtr->textBuffs[1][i];
		gBattleTextBuff3[i] = sBattleMsgDataPtr->textBuffs[2][i];
	}
		
    switch (stringId)
    {
    case STRINGID_INTROMSG:
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        {
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = COMPOUND_STRING("{B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME} want to battle!");
                else                                                                                        // Link trainer
					stringPtr = gTrainerBattleOpponent_A == TRAINER_UNION_ROOM ? sText_Trainer1WantsToBattle : COMPOUND_STRING("{B_LINK_OPPONENT1_NAME} wants to battle!");
            }
            else
                stringPtr = sText_Trainer1WantsToBattle;
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_GHOST)
				stringPtr = (gBattleTypeFlags & BATTLE_TYPE_GHOST_UNVEILED) ? COMPOUND_STRING("The Ghost appeared!\p") : COMPOUND_STRING("The Ghost appeared!\pDarn!\nThe Ghost can't be ID'd!\p");
            else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                stringPtr = COMPOUND_STRING("Wild {B_OPPONENT_MON1_NAME} and {B_OPPONENT_MON2_NAME} appeared!\p");
            else if (gBattleTypeFlags & BATTLE_TYPE_OLD_MAN_TUTORIAL)
                stringPtr = COMPOUND_STRING("Wild {B_OPPONENT_MON1_NAME} appeared!{PAUSE 127}"); // Same as bellow, but with a pausse
            else
                stringPtr = COMPOUND_STRING("Wild {B_OPPONENT_MON1_NAME} appeared!\p");
        }
        break;
    case STRINGID_INTROSENDOUT:
        if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
        {
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
				if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
					stringPtr = COMPOUND_STRING("{B_LINK_PARTNER_NAME} sent out {B_LINK_PLAYER_MON2_NAME}!\nGo! {B_LINK_PLAYER_MON1_NAME}!");
				else
					stringPtr = COMPOUND_STRING("Go! {B_PLAYER_MON1_NAME} and {B_PLAYER_MON2_NAME}!");
            }
			else
			{
				GetBattlerNick(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), gBattleTextBuff1);
                stringPtr = sText_GoPkmn;
			}
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = COMPOUND_STRING("{B_LINK_OPPONENT1_NAME} sent out {B_LINK_OPPONENT_MON1_NAME}!\n{B_LINK_OPPONENT2_NAME} sent out {B_LINK_OPPONENT_MON2_NAME}!");
                else if (gBattleTypeFlags & BATTLE_TYPE_LINK)
                    stringPtr = COMPOUND_STRING("{B_LINK_OPPONENT1_NAME} sent out {B_OPPONENT_MON1_NAME} and {B_OPPONENT_MON2_NAME}!");
                else
                    stringPtr = COMPOUND_STRING("{B_TRAINER1_CLASS} {B_TRAINER1_NAME} sent out {B_OPPONENT_MON1_NAME} and {B_OPPONENT_MON2_NAME}!{PAUSE 60}");
            }
            else
            {
                if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
                    stringPtr = sText_Trainer1SentOutPkmn;
                else if (gTrainerBattleOpponent_A == TRAINER_UNION_ROOM)
                    stringPtr = sText_Trainer1SentOutPkmn;
                else
				{
					GetBattlerNick(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), gBattleTextBuff1);
                    stringPtr = sText_LinkTrainerSentOutPkmn;
				}
            }
        }
        break;
    case STRINGID_RETURNMON:
        if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
        {
            if (gBattleStruct->hpScale == 0)
                stringPtr = COMPOUND_STRING("{B_BUFF1}, that's enough!\nCome back!");
            else if (gBattleStruct->hpScale == 1 || gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                stringPtr = COMPOUND_STRING("{B_BUFF1}, come back!");
            else if (gBattleStruct->hpScale == 2)
                stringPtr = COMPOUND_STRING("{B_BUFF1}, OK!\nCome back!");
            else
                stringPtr = COMPOUND_STRING("{B_BUFF1}, good!\nCome back!");
        }
        else
        {
            if (gTrainerBattleOpponent_A == TRAINER_LINK_OPPONENT)
			{
				if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
					stringPtr = COMPOUND_STRING("{B_LINK_SCR_TRAINER_NAME} withdrew {B_BUFF1}!");
				else
					stringPtr = COMPOUND_STRING("{B_LINK_OPPONENT1_NAME} withdrew {B_BUFF1}!");
			}
            else
                stringPtr = sText_Trainer1WithdrewPkmn;
        }
        break;
    case STRINGID_SWITCHINMON:
        if (GetBattlerSide(gBattleScripting.battler) == B_SIDE_PLAYER)
        {
            if (gBattleStruct->hpScale == 0 || gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                stringPtr = sText_GoPkmn;
            else if (gBattleStruct->hpScale == 1)
                stringPtr = COMPOUND_STRING("Do it! {B_BUFF1}!");
            else if (gBattleStruct->hpScale == 2)
                stringPtr = COMPOUND_STRING("Go for it, {B_BUFF1}!");
            else
                stringPtr = COMPOUND_STRING("Your foe's weak!\nGet 'em, {B_BUFF1}!");
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = COMPOUND_STRING("{B_LINK_SCR_TRAINER_NAME} sent out {B_BUFF1}!");
                else if (gTrainerBattleOpponent_A == TRAINER_UNION_ROOM)
                    stringPtr = sText_Trainer1SentOutPkmn2;
                else
                    stringPtr = sText_LinkTrainerSentOutPkmn;
            }
            else
                stringPtr = sText_Trainer1SentOutPkmn2;
        }
        break;
    case STRINGID_USEDMOVE:
	    StringCopy(gBattleTextBuff2, gBattleMoves[sBattleMsgDataPtr->currentMove].name);
        stringPtr = COMPOUND_STRING("{B_ATK_NAME_WITH_PREFIX} used {B_BUFF2}!");
        break;
    case STRINGID_BATTLEEND:
        if (gBattleTextBuff1[0] & B_OUTCOME_LINK_BATTLE_RAN)
        {
            gBattleTextBuff1[0] &= ~(B_OUTCOME_LINK_BATTLE_RAN);
			
            if (GetBattlerSide(battlerId) == B_SIDE_OPPONENT && gBattleTextBuff1[0] != B_OUTCOME_DREW)
                gBattleTextBuff1[0] ^= (B_OUTCOME_LOST | B_OUTCOME_WON);

            if (gBattleTextBuff1[0] == B_OUTCOME_LOST || gBattleTextBuff1[0] == B_OUTCOME_DREW)
                stringPtr = sText_GotAwaySafely;
            else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                stringPtr = COMPOUND_STRING("{PLAY_SE SE_FLEE}{B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME} fled!");
            else if (gTrainerBattleOpponent_A == TRAINER_UNION_ROOM)
                stringPtr = COMPOUND_STRING("{PLAY_SE SE_FLEE}{B_TRAINER1_CLASS} {B_TRAINER1_NAME} fled!");
            else
                stringPtr = COMPOUND_STRING("{PLAY_SE SE_FLEE}{B_LINK_OPPONENT1_NAME} fled!");
        }
        else
        {
            if (GetBattlerSide(battlerId) == B_SIDE_OPPONENT && gBattleTextBuff1[0] != B_OUTCOME_DREW)
                gBattleTextBuff1[0] ^= (B_OUTCOME_LOST | B_OUTCOME_WON);

            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = COMPOUND_STRING("Player beat {B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME}!");
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = COMPOUND_STRING("Player lost to {B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME}!");
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = COMPOUND_STRING("Player battled to a draw against {B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME}!");
                    break;
                }
            }
            else if (gTrainerBattleOpponent_A == TRAINER_UNION_ROOM)
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = sText_PlayerDefeatedLinkTrainerTrainer1;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = COMPOUND_STRING("Player lost against {B_TRAINER1_CLASS} {B_TRAINER1_NAME}!");
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = COMPOUND_STRING("Player battled to a draw against {B_TRAINER1_CLASS} {B_TRAINER1_NAME}!");
                    break;
                }
            }
            else
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = COMPOUND_STRING("Player defeated {B_LINK_OPPONENT1_NAME}!");
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = COMPOUND_STRING("Player lost against {B_LINK_OPPONENT1_NAME}!");
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = COMPOUND_STRING("Player battled to a draw against {B_LINK_OPPONENT1_NAME}!");
                    break;
                }
            }
        }
        break;
	case STRINGID_TRAINERSLIDE:
	    stringPtr = gBattleStruct->trainerSlideMsg;
		break;
	case STRINGID_TRAINERUSEDITEM:
		CopyTrainerStringCaseToBuffer1();
		stringPtr = COMPOUND_STRING("{B_BUFF1} used {B_LAST_ITEM}!");
		break;
	case STRINGID_ITEMCATCHYTUNE:
		CopyTrainerStringCaseToBuffer1();
		stringPtr = COMPOUND_STRING("{B_BUFF1} played the {B_LAST_ITEM}. Now, that's a catchy tune!");
		break;
	case STRINGID_ITEMPLAYED:
	    CopyTrainerStringCaseToBuffer1();
		stringPtr = COMPOUND_STRING("{B_BUFF1} played the {B_LAST_ITEM}.");
		break;
    default:
        if (stringId >= BATTLESTRINGS_COUNT)
        {
            gDisplayedStringBattle[0] = EOS;
            return;
        }
        else
            stringPtr = gBattleStringsTable[stringId - BATTLESTRINGS_ID_ADDER];
        break;
    }
    BattleStringExpandPlaceholdersToDisplayedString(stringPtr);
}

void BattleStringExpandPlaceholdersToDisplayedString(const u8* src)
{
	// This buffer may hold either the name of a trainer, Pokémon, or item.
	u8 text[max(max(max(32, 11), POKEMON_NAME_LENGTH + 1), ITEM_NAME_LENGTH)];
	u8 *dst = gDisplayedStringBattle;
	u32 dstId = 0;
	const u8 *toCpy = NULL;
	u8 multiplayerId = GetMultiplayerId();
	
	while (*src != EOS)
	{
		if (*src == PLACEHOLDER_BEGIN)
		{
			++src;
			
			switch (*src)
			{
				case B_TXT_BUFF1:
				    if (gBattleTextBuff1[0] == B_BUFF_PLACEHOLDER_BEGIN)
					{
						ExpandBattleTextBuffPlaceholders(gBattleTextBuff1, gStringVar1);
						toCpy = gStringVar1;
					}
					else
						toCpy = gBattleTextBuff1;
					break;
				case B_TXT_BUFF2:
                    if (gBattleTextBuff2[0] == B_BUFF_PLACEHOLDER_BEGIN)
					{
						ExpandBattleTextBuffPlaceholders(gBattleTextBuff2, gStringVar2);
						toCpy = gStringVar2;
					}
					else
						toCpy = gBattleTextBuff2;
					break;
				case B_TXT_BUFF3:
				    if (gBattleTextBuff3[0] == B_BUFF_PLACEHOLDER_BEGIN)
					{
						ExpandBattleTextBuffPlaceholders(gBattleTextBuff3, gStringVar3);
						toCpy = gStringVar3;
					}
					else
						toCpy = gBattleTextBuff3;
					break;
				case B_TXT_PLAYER_MON1_NAME:
                    GetBattlerNick(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), text);
					toCpy = text;
					break;
				case B_TXT_OPPONENT_MON1_NAME:
                    GetBattlerNick(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), text);
					toCpy = text;
					break;
				case B_TXT_PLAYER_MON2_NAME:
                    GetBattlerNick(GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT), text);
					toCpy = text;
					break;
				case B_TXT_OPPONENT_MON2_NAME:
                    GetBattlerNick(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT), text);
					toCpy = text;
					break;
				case B_TXT_LINK_PLAYER_MON1_NAME:
                    GetBattlerNick(gLinkPlayers[multiplayerId].id, text);
					toCpy = text;
					break;
				case B_TXT_LINK_OPPONENT_MON1_NAME:
                    GetBattlerNick(gLinkPlayers[multiplayerId].id ^ 1, text);
					toCpy = text;
					break;
				case B_TXT_LINK_PLAYER_MON2_NAME:
                    GetBattlerNick(gLinkPlayers[multiplayerId].id ^ 2, text);
					toCpy = text;
					break;
				case B_TXT_LINK_OPPONENT_MON2_NAME:
                    GetBattlerNick(gLinkPlayers[multiplayerId].id ^ 3, text);
					toCpy = text;
					break;
				case B_TXT_ATK_NAME_WITH_PREFIX:
                    HANDLE_NICKNAME_STRING_CASE(gBattlerAttacker);
					break;
				case B_TXT_DEF_NAME_WITH_PREFIX:
                    HANDLE_NICKNAME_STRING_CASE(gBattlerTarget);
					break;
				case B_TXT_EFF_NAME_WITH_PREFIX:
                    HANDLE_NICKNAME_STRING_CASE(gEffectBattler);
					break;
				case B_TXT_SCR_ACTIVE_NAME_WITH_PREFIX:
                    HANDLE_NICKNAME_STRING_CASE(gBattleScripting.battler);
					break;
				case B_TXT_CURRENT_MOVE:
                    toCpy = gBattleMoves[sBattleMsgDataPtr->currentMove].name;
					break;
				case B_TXT_CHOSEN_MOVE:
                    toCpy = gBattleMoves[sBattleMsgDataPtr->chosenMove].name;
					break;
                case B_TXT_LAST_ITEM:
				    CopyItemName(gLastUsedItem, text);
					toCpy = text;
					break;
				case B_TXT_ATK_ABILITY:
                    toCpy = gAbilities[sBattlerAbilities[gBattlerAttacker]].name;
					break;
				case B_TXT_DEF_ABILITY:
                    toCpy = gAbilities[sBattlerAbilities[gBattlerTarget]].name;
					break;
				case B_TXT_SCR_ACTIVE_ABILITY:
                    toCpy = gAbilities[sBattlerAbilities[gBattleScripting.battler]].name;
					break;
				case B_TXT_EFF_ABILITY:
                    toCpy = gAbilities[sBattlerAbilities[gEffectBattler]].name;
					break;
				case B_TXT_TRAINER1_CLASS:
                    if (gTrainerBattleOpponent_A == TRAINER_UNION_ROOM)
						toCpy = gTrainerClassNames[GetUnionRoomTrainerClass()];
					else if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_TOWER)
						toCpy = gTrainerClassNames[GetBattleTowerTrainerClassNameId()];
					else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
						toCpy = gTrainerClassNames[GetEreaderTrainerClassId()];
					else
						toCpy = gTrainerClassNames[gTrainers[gTrainerBattleOpponent_A].trainerClass];
					break;
				case B_TXT_TRAINER1_NAME:
                    if (gTrainerBattleOpponent_A == TRAINER_UNION_ROOM)
						toCpy = gLinkPlayers[BATTLE_OPPOSITE(multiplayerId)].name;
					else if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_TOWER)
						GetBattleTowerTrainerName(text);
					else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
					{
						CopyEReaderTrainerName5(text);
						toCpy = text;
					}
					else
					{
						if (gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_RIVAL_EARLY
						|| gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_RIVAL_LATE
						|| gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_CHAMPION)
						    toCpy = GetExpandedPlaceholder(PLACEHOLDER_ID_RIVAL);
						else
							toCpy = gTrainers[gTrainerBattleOpponent_A].trainerName;
					}
					break;
				case B_TXT_LINK_PLAYER_NAME:
				    toCpy = gLinkPlayers[multiplayerId].name;
					break;
				case B_TXT_LINK_PARTNER_NAME:
				    toCpy = gLinkPlayers[GetBattlerMultiplayerId(BATTLE_PARTNER(gLinkPlayers[multiplayerId].id))].name;
					break;
				case B_TXT_LINK_OPPONENT1_NAME:
                    toCpy = gLinkPlayers[GetBattlerMultiplayerId(BATTLE_OPPOSITE(gLinkPlayers[multiplayerId].id))].name;
					break;
				case B_TXT_LINK_OPPONENT2_NAME:
                    toCpy = gLinkPlayers[GetBattlerMultiplayerId(BATTLE_PARTNER(BATTLE_OPPOSITE(gLinkPlayers[multiplayerId].id)))].name;
					break;
				case B_TXT_LINK_SCR_TRAINER_NAME:
				    toCpy = gLinkPlayers[GetBattlerMultiplayerId(gBattleScripting.battler)].name;
					break;
				case B_TXT_PLAYER_NAME:
				    toCpy = gSaveBlock2Ptr->playerName;
					break;
				case B_TXT_TRAINER1_LOSE_TEXT:
				    toCpy = GetTrainerALoseText();
					break;
				case B_TXT_TRAINER1_WIN_TEXT:
				    toCpy = GetTrainerWonSpeech();
					break;
				case B_TXT_ITEM_USE_SPECIES_NAME:
				{
					u8 battler = GetItemUseBattler(gBattlerAttacker);
					HandlePartyMonNickBuffer(battler, gBattleStruct->itemPartyIndex[gBattlerAttacker], text, (battler != MAX_BATTLERS_COUNT));
					toCpy = text;
					break;
				}
				case B_TXT_ATK_TEAM_PREFIX:
				    toCpy = GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER ? sText_YourTeamPrefix : sText_TheOpposingTeamPrefix;
					break;
				case B_TXT_DEF_TEAM_PREFIX:
				    toCpy = GetBattlerSide(gBattlerTarget) == B_SIDE_PLAYER ? sText_YourTeamPrefix : sText_TheOpposingTeamPrefix;
					break;
				case B_TXT_DEF_NAME:
				    GetBattlerNick(gBattlerTarget, text);
					toCpy = text;
					break;
			}
			
			if (toCpy != NULL)
			{
				while (*toCpy != EOS)
				{
					dst[dstId] = *toCpy;
					dstId++;
					toCpy++;
				}
			}
			
			if (*src == B_TXT_TRAINER1_LOSE_TEXT || *src == B_TXT_TRAINER1_WIN_TEXT)
            {
                dst[dstId++] = EXT_CTRL_CODE_BEGIN;
                dst[dstId++] = EXT_CTRL_CODE_WAIT_BUTTON;
            }
		}
		else
			dst[dstId++] = *src;
		
		src++;
	}
	dst[dstId++] = *src;
    
	ReformatStringToMaxChars(dst, sTextOnWindowsInfo_Normal[B_WIN_MSG].fontId, 40, TRUE);
	StringCopy(dst, gStringVar4);
}

static void ExpandBattleTextBuffPlaceholders(const u8 *src, u8 *dst)
{
    u32 value, srcId = 1;
    u8 text[POKEMON_NAME_LENGTH + 1];

    *dst = EOS;
	
    while (src[srcId] != B_BUFF_EOS)
    {
        switch (src[srcId])
        {
        case B_BUFF_STRING:
            StringAppend(dst, gBattleStringsTable[READ_16(&src[srcId + 1]) - BATTLESTRINGS_ID_ADDER]);
            srcId += 3;
            break;
        case B_BUFF_NUMBER:
            switch (src[srcId + 1])
            {
            case 1:
                value = src[srcId + 3];
                break;
            case 2:
                value = READ_16(&src[srcId + 3]);
                break;
            case 4:
                value = READ_32(&src[srcId + 3]);
                break;
            }
            ConvertIntToDecimalStringN(dst, value, STR_CONV_MODE_LEFT_ALIGN, src[srcId + 2]);
            srcId += src[srcId + 1] + 3;
            break;
        case B_BUFF_MOVE:
            StringAppend(dst, gBattleMoves[READ_16(&src[srcId + 1])].name);
            srcId += 3;
            break;
        case B_BUFF_TYPE:
            StringAppend(dst, gTypeNames[src[srcId + 1]]);
            srcId += 2;
            break;
        case B_BUFF_MON_NICK_WITH_PREFIX:
            if (GetBattlerSide(src[srcId + 1]) == B_SIDE_PLAYER)
                GetMonData(&gPlayerParty[src[srcId + 2]], MON_DATA_NICKNAME, text);
            else
            {
				StringAppend(dst, (gBattleTypeFlags & BATTLE_TYPE_TRAINER) ? sText_FoePkmnPrefix : sText_WildPkmnPrefix);
                GetMonData(&gEnemyParty[src[srcId + 2]], MON_DATA_NICKNAME, text);
            }
			StringGet_Nickname(text);
            StringAppend(dst, text);
            srcId += 3;
            break;
        case B_BUFF_STAT:
            StringAppend(dst, sBattleStatNamesTable[src[srcId + 1]]);
            srcId += 2;
            break;
        case B_BUFF_SPECIES:
            GetSpeciesName(dst, READ_16(&src[srcId + 1]));
            srcId += 3;
            break;
        case B_BUFF_MON_NICK:
		    HandlePartyMonNickBuffer(src[srcId + 1], src[srcId + 2], dst, (src[srcId + 2] == gBattlerPartyIndexes[src[srcId + 1]]));
            srcId += 3;
            break;
        case B_BUFF_NEGATIVE_FLAVOR:
            StringAppend(dst, sPokeblockWasTooXStringTable[src[srcId + 1]]);
            srcId += 2;
            break;
        case B_BUFF_ABILITY:
            StringAppend(dst, gAbilities[READ_16(&src[srcId + 1])].name);
            srcId += 3;
            break;
        case B_BUFF_ITEM:
			CopyItemName(READ_16(&src[srcId + 1]), dst);
            srcId += 3;
            break;
		case B_BUFF_MON_NICK_NO_ILLUSION:
		    GetMonData(&GetBattlerParty(src[srcId + 1])[src[srcId + 2]], MON_DATA_NICKNAME, dst);
			StringGet_Nickname(dst);
            srcId += 3;
            break;
		case B_BUFF_MON_TEAM_PREFIX:
		    StringAppend(dst, GetBattlerSide(src[srcId + 1]) == B_SIDE_PLAYER ? sText_YourTeamPrefix : sText_TheOpposingTeamPrefix);
			srcId += 2;
			break;
        }
    }
}

// windowId: Upper 2 bits are text flags
//   x40: Use NPC context-defined font
//   x80: Inhibit window clear
void BattlePutTextOnWindow(const u8 *text, u8 windowId)
{
    struct TextPrinterTemplate printerTemplate;
    u8 speed;
    int x;
    u8 textFlags = (windowId & (B_TEXT_FLAG_NPC_CONTEXT_FONT | B_TEXT_FLAG_WINDOW_CLEAR));
    
	windowId &= 0x3F;
	
    if (!(textFlags & B_TEXT_FLAG_WINDOW_CLEAR))
        FillWindowPixelBuffer(windowId, sTextOnWindowsInfo_Normal[windowId].fillValue);
    
	printerTemplate.fontId = (textFlags & B_TEXT_FLAG_NPC_CONTEXT_FONT) ? sNpcTextColorToFont[ContextNpcGetTextColor()] : sTextOnWindowsInfo_Normal[windowId].fontId;

    switch (windowId)
    {
    case B_WIN_VS_PLAYER ... B_WIN_VS_MULTI_PLAYER_4:
        x = (48 - GetStringWidth(sTextOnWindowsInfo_Normal[windowId].fontId, text, sTextOnWindowsInfo_Normal[windowId].letterSpacing)) / 2;
        break;
    case B_WIN_VS_OUTCOME_DRAW ... B_WIN_VS_OUTCOME_RIGHT:
        x = (64 - GetStringWidth(sTextOnWindowsInfo_Normal[windowId].fontId, text, sTextOnWindowsInfo_Normal[windowId].letterSpacing)) / 2;
        break;
    default:
        x = sTextOnWindowsInfo_Normal[windowId].x;
        break;
    }
    if (x < 0)
        x = 0;
	
    printerTemplate.currentChar = text;
    printerTemplate.windowId = windowId;
    printerTemplate.x = x;
    printerTemplate.y = sTextOnWindowsInfo_Normal[windowId].y;
    printerTemplate.currentX = printerTemplate.x;
    printerTemplate.currentY = printerTemplate.y;
    printerTemplate.letterSpacing = sTextOnWindowsInfo_Normal[windowId].letterSpacing;
    printerTemplate.lineSpacing = sTextOnWindowsInfo_Normal[windowId].lineSpacing;
    printerTemplate.unk = 0;
    printerTemplate.fgColor = sTextOnWindowsInfo_Normal[windowId].fgColor;
    printerTemplate.bgColor = sTextOnWindowsInfo_Normal[windowId].bgColor;
    printerTemplate.shadowColor = sTextOnWindowsInfo_Normal[windowId].shadowColor;
	
	gTextFlags.useAlternateDownArrow = (windowId != B_WIN_OAK_OLD_MAN);

    if ((gBattleTypeFlags & BATTLE_TYPE_LINK) || ((gBattleTypeFlags & BATTLE_TYPE_POKEDUDE) && windowId != B_WIN_OAK_OLD_MAN))
        gTextFlags.autoScroll = TRUE;
    else
        gTextFlags.autoScroll = FALSE;

    if (windowId == B_WIN_MSG || windowId == B_WIN_OAK_OLD_MAN)
    {
		speed = (gBattleTypeFlags & BATTLE_TYPE_LINK) ? 1 : GetTextSpeedSetting();
        gTextFlags.canABSpeedUpPrint = TRUE;
    }
    else
    {
        speed = sTextOnWindowsInfo_Normal[windowId].speed;
        gTextFlags.canABSpeedUpPrint = FALSE;
    }

    AddTextPrinter(&printerTemplate, speed, NULL);
	
    if (!(textFlags & B_TEXT_FLAG_WINDOW_CLEAR))
    {
        PutWindowTilemap(windowId);
        CopyWindowToVram(windowId, COPYWIN_BOTH);
    }
}

bool8 BattleStringShouldBeColored(u16 stringId)
{
    if (stringId == STRINGID_TRAINER1LOSETEXT || stringId == STRINGID_TRAINER1WINTEXT)
        return TRUE;
    return FALSE;
}

static u8 GetCurrentPpToMaxPpState(u8 currentPp, u8 maxPp)
{
    if (maxPp == currentPp)
        return 3;
    else if (maxPp <= 2)
    {
        if (currentPp > 1)
            return 3;
        else
            return 2 - currentPp;
    }
    else if (maxPp <= 7)
    {
        if (currentPp > 2)
            return 3;
        else
            return 2 - currentPp;
    }
    else
    {
        if (currentPp == 0)
            return 2;
        else if (currentPp <= maxPp / 4)
            return 1;
        else if (currentPp > maxPp / 2)
            return 3;
    }
    return 0;
}

void SetPpNumbersPaletteInMoveSelection(u8 battlerId)
{
    struct ChooseMoveStruct *chooseMoveStruct = (struct ChooseMoveStruct*)(&gBattleBufferA[battlerId][4]);
    u8 var = GetCurrentPpToMaxPpState(chooseMoveStruct->currentPp[gMoveSelectionCursor[battlerId]], chooseMoveStruct->maxPp[gMoveSelectionCursor[battlerId]]);

    gPlttBufferUnfaded[92] = gUnknown_8D2FBB4[(var * 2) + 0];
    gPlttBufferUnfaded[91] = gUnknown_8D2FBB4[(var * 2) + 1];

    CpuCopy16(&gPlttBufferUnfaded[92], &gPlttBufferFaded[92], sizeof(u16));
    CpuCopy16(&gPlttBufferUnfaded[91], &gPlttBufferFaded[91], sizeof(u16));
}

/*
static const u8* TryGetStatusString(u8 *src)
{
    u32 i;
    u8 status[] = _("$$$$$$$");
    u32 chars1, chars2;
    u8* statusPtr;

    statusPtr = status;
    for (i = 0; i < 8; i++)
    {
        if (*src == EOS)
            break;
        *statusPtr = *src;
        src++;
        statusPtr++;
    }

    chars1 = *(u32*)(&status[0]);
    chars2 = *(u32*)(&status[4]);

    for (i = 0; i < ARRAY_COUNT(gStatusConditionStringsTable); i++)
    {
        if (chars1 == *(u32*)(&gStatusConditionStringsTable[i][0][0])
            && chars2 == *(u32*)(&gStatusConditionStringsTable[i][0][4]))
            return gStatusConditionStringsTable[i][1];
    }
    return NULL;
}*/

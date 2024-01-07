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
const u8 gText_SafariBalls[] = _("{HIGHLIGHT 2}Safari Balls");
const u8 gText_HighlightRed_Left[] = _("{HIGHLIGHT 2}Left: ");
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
static const u8 sText_TwoLinkTrainersWantToBattle[] = _("{B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME} want to battle!");
static const u8 sText_Trainer1WantsToBattle[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME} would like to battle!\p");
static const u8 sText_LinkTrainerWantsToBattle[] = _("{B_LINK_OPPONENT1_NAME} wants to battle!");
static const u8 sText_GhostAppearedCantId[] = _("The Ghost appeared!\pDarn!\nThe Ghost can't be ID'd!\p");
static const u8 sText_TheGhostAppeared[] = _("The Ghost appeared!\p");
static const u8 sText_TwoWildPkmnAppeared[] = _("Wild {B_OPPONENT_MON1_NAME} and {B_OPPONENT_MON2_NAME} appeared!\p");
static const u8 sText_WildPkmnAppeared[] = _("Wild {B_OPPONENT_MON1_NAME} appeared!\p");
static const u8 sText_WildPkmnAppearedPause[] = _("Wild {B_OPPONENT_MON1_NAME} appeared!{PAUSE 127}");
static const u8 sText_LinkPartnerSentOutPkmnGoPkmn[] = _("{B_LINK_PARTNER_NAME} sent out {B_LINK_PLAYER_MON2_NAME}!\nGo! {B_LINK_PLAYER_MON1_NAME}!");
static const u8 sText_GoPkmn[] = _("Go! {B_BUFF1}!");
static const u8 sText_GoTwoPkmn[] = _("Go! {B_PLAYER_MON1_NAME} and {B_PLAYER_MON2_NAME}!");
static const u8 sText_TwoLinkTrainersSentOutPkmn[] = _("{B_LINK_OPPONENT1_NAME} sent out {B_LINK_OPPONENT_MON1_NAME}!\n{B_LINK_OPPONENT2_NAME} sent out {B_LINK_OPPONENT_MON2_NAME}!");
static const u8 sText_LinkTrainerSentOutTwoPkmn[] = _("{B_LINK_OPPONENT1_NAME} sent out {B_OPPONENT_MON1_NAME} and {B_OPPONENT_MON2_NAME}!");
static const u8 sText_Trainer1SentOutTwoPkmn[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME} sent out {B_OPPONENT_MON1_NAME} and {B_OPPONENT_MON2_NAME}!{PAUSE 60}");
static const u8 sText_Trainer1SentOutPkmn[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME} sent out {B_OPPONENT_MON1_NAME}!{PAUSE 60}");
static const u8 sText_LinkTrainerSentOutPkmn[] = _("{B_LINK_OPPONENT1_NAME} sent out {B_BUFF1}!");
static const u8 sText_PkmnThatsEnough[] = _("{B_BUFF1}, that's enough!\nCome back!");
static const u8 sText_PkmnComeBack[] = _("{B_BUFF1}, come back!");
static const u8 sText_PkmnOkComeBack[] = _("{B_BUFF1}, OK!\nCome back!");
static const u8 sText_PkmnGoodComeBack[] = _("{B_BUFF1}, good!\nCome back!");
static const u8 sText_LinkTrainer1WithdrewPkmn[] = _("{B_LINK_OPPONENT1_NAME} withdrew {B_BUFF1}!");
static const u8 sText_LinkTrainer2WithdrewPkmn[] = _("{B_LINK_SCR_TRAINER_NAME} withdrew {B_BUFF1}!");
static const u8 sText_DoItPkmn[] = _("Do it! {B_BUFF1}!");
static const u8 sText_GoForItPkmn[] = _("Go for it, {B_BUFF1}!");
static const u8 sText_YourFoesWeakGetEmPkmn[] = _("Your foe's weak!\nGet 'em, {B_BUFF1}!");
static const u8 sText_LinkTrainerMultiSentOutPkmn[] = _("{B_LINK_SCR_TRAINER_NAME} sent out {B_BUFF1}!");
static const u8 sText_Trainer1SentOutPkmn2[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME} sent out {B_BUFF1}!");
static const u8 sText_AttackerUsedX[] = _("{B_ATK_NAME_WITH_PREFIX} used {B_BUFF2}!");
static const u8 sText_TwoWildFled[] = _("{PLAY_SE SE_FLEE}{B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME} fled!");
static const u8 sText_UnionRoomTrainerFled[] = _("{PLAY_SE SE_FLEE}{B_TRAINER1_CLASS} {B_TRAINER1_NAME} fled!");
static const u8 sText_WildFled[] = _("{PLAY_SE SE_FLEE}{B_LINK_OPPONENT1_NAME} fled!");
static const u8 sText_TwoLinkTrainersDefeated[] = _("Player beat {B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME}!");
static const u8 sText_PlayerLostToTwo[] = _("Player lost to {B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME}!");
static const u8 sText_PlayerBattledToDrawVsTwo[] = _("Player battled to a draw against {B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME}!");
static const u8 sText_PlayerDefeatedLinkTrainerTrainer1[] = _("Player defeated {B_TRAINER1_CLASS} {B_TRAINER1_NAME}!\p");
static const u8 sText_PlayerLostAgainstTrainer1[] = _("Player lost against {B_TRAINER1_CLASS} {B_TRAINER1_NAME}!");
static const u8 sText_PlayerBattledToDrawTrainer1[] = _("Player battled to a draw against {B_TRAINER1_CLASS} {B_TRAINER1_NAME}!");
static const u8 sText_PlayerDefeatedLinkTrainer[] = _("Player defeated {B_LINK_OPPONENT1_NAME}!");
static const u8 sText_PlayerLostAgainstLinkTrainer[] = _("Player lost against {B_LINK_OPPONENT1_NAME}!");
static const u8 sText_PlayerBattledToDrawLinkTrainer[] = _("Player battled to a draw against {B_LINK_OPPONENT1_NAME}!");
static const u8 sText_ThePokeDude[] = _("The Poké Dude");
static const u8 sText_TheOldMan[] = _("The old man");
static const u8 sText_You[] = _("You");
static const u8 sText_Trainer1[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}");
static const u8 sText_Buff1UsedLastItem[] = _("{B_BUFF1} used {B_LAST_ITEM}!");
static const u8 sText_Buff1PlayedLastItem[] = _("{B_BUFF1} played the {B_LAST_ITEM}.");
static const u8 sText_Buff1PlayedLastItemCatchyTune[] = _("{B_BUFF1} played the {B_LAST_ITEM}. Now, that's a catchy tune!");
static const u8 sText_FoePkmnPrefix[] = _("Foe ");
static const u8 sText_WildPkmnPrefix[] = _("Wild ");
static const u8 sText_YourTeamPrefix[] = _("Your team");
static const u8 sText_TheOpposingTeamPrefix[] = _("The opposing team");
static const u8 sText_Trainer1LoseText[] = _("{B_TRAINER1_LOSE_TEXT}");
static const u8 sText_PkmnGainedEXP[] = _("{B_BUFF1} gained{B_BUFF2} {B_BUFF3} Exp. Points!\p");
static const u8 sText_PkmnGrewToLv[] = _("{B_BUFF1} grew\nto Lv. {B_BUFF2}!{WAIT_SE}\p");
static const u8 sText_PkmnLearnedMove[] = _("{B_BUFF1} learned {B_BUFF2}!{WAIT_SE}\p");
static const u8 sText_TryToLearnMove1[] = _("{B_BUFF1} is trying to learn {B_BUFF2}.\p");
static const u8 sText_TryToLearnMove2[] = _("But, {B_BUFF1} can't learn more than four moves.\p");
static const u8 sText_TryToLearnMove3[] = _("Delete a move to make room for {B_BUFF2}?");
static const u8 sText_AtkLearnedBuff1[] = _("{B_ATK_NAME_WITH_PREFIX} learned {B_BUFF1}!");
static const u8 sText_AttackMissed[] = _("{B_ATK_NAME_WITH_PREFIX}'s attack missed!");
static const u8 sText_DefProtectedItself[] = _("{B_DEF_NAME_WITH_PREFIX} protected itself!");
static const u8 sText_ItDoesntAffect[] = _("It doesn't affect {B_DEF_NAME_WITH_PREFIX}…");
static const u8 sText_AtkFainted[] = _("{B_ATK_NAME_WITH_PREFIX} fainted!\p");
static const u8 sText_DefFainted[] = _("{B_DEF_NAME_WITH_PREFIX} fainted!\p");
static const u8 sText_PlayerGotMoney[] = _("{B_PLAYER_NAME} got ¥{B_BUFF1} for winning!\p");
static const u8 sText_PlayerWhiteout[] = _("{B_PLAYER_NAME} is out of usable Pokémon!\p");
static const u8 sText_PlayerPanicked[] = _("{B_PLAYER_NAME} panicked and lost ¥{B_BUFF1}…\p… … … …\p{B_PLAYER_NAME} whited out!{PAUSE_UNTIL_PRESS}");
static const u8 sText_PkmnPreventsEscapeWithAbl[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} prevents escape with {B_SCR_ACTIVE_ABILITY}!\p");
static const u8 sText_HitXTimes[] = _("Hit {B_BUFF1} time(s)!");
static const u8 sText_EffFellAsleep[] = _("{B_EFF_NAME_WITH_PREFIX} fell asleep!");
static const u8 sText_DefAlreadyAsleep[] = _("{B_DEF_NAME_WITH_PREFIX} is already asleep!");
static const u8 sText_DefWasntAffected[] = _("{B_DEF_NAME_WITH_PREFIX} wasn't affected!");
static const u8 sText_EffWasPoisoned[] = _("{B_EFF_NAME_WITH_PREFIX} was poisoned!");
static const u8 sText_AtkHurtByPoison[] = _("{B_ATK_NAME_WITH_PREFIX} is hurt by poison!");
static const u8 sText_DefAlreadyPoisoned[] = _("{B_DEF_NAME_WITH_PREFIX} is already poisoned.");
static const u8 sText_EffBadlyPoisoned[] = _("{B_EFF_NAME_WITH_PREFIX} is badly poisoned!");
static const u8 sText_DefEnergyDrained[] = _("{B_DEF_NAME_WITH_PREFIX} had its energy drained!");
static const u8 sText_EffWasBurned[] = _("{B_EFF_NAME_WITH_PREFIX} was burned!");
static const u8 sText_AtkHurtByBurn[] = _("{B_ATK_NAME_WITH_PREFIX} is hurt by its burn!");
static const u8 sText_EffWasFrozen[] = _("{B_EFF_NAME_WITH_PREFIX} was frozen solid!");
static const u8 sText_AtkIsFrozen[] = _("{B_ATK_NAME_WITH_PREFIX} is frozen solid!");
static const u8 sText_Buff1WasDefrosted[] = _("{B_BUFF1} was defrosted!");
static const u8 sText_AtkDefrostedByCurrMove[] = _("{B_ATK_NAME_WITH_PREFIX} was defrosted by {B_CURRENT_MOVE}!");
static const u8 sText_EffWasParalyzed[] = _("{B_EFF_NAME_WITH_PREFIX} is paralyzed!\nIt may be unable to move!");
static const u8 sText_AtkIsParalyzed[] = _("{B_ATK_NAME_WITH_PREFIX} is paralyzed!\nIt can't move!");
static const u8 sText_DefIsAlreadyParalyzed[] = _("{B_DEF_NAME_WITH_PREFIX} is already paralyzed!");
static const u8 sText_EffAblPreventsBurns[] = _("{B_EFF_NAME_WITH_PREFIX}'s {B_EFF_ABILITY} prevents burns!");
static const u8 sText_EffCuredOfBuff1[] = _("{B_EFF_NAME_WITH_PREFIX} was cured of {B_BUFF1}.");
static const u8 sText_EffWokeUp[] = _("{B_EFF_NAME_WITH_PREFIX} woke up!");
static const u8 sText_AtkIsConfused[] = _("{B_ATK_NAME_WITH_PREFIX} is confused!");
static const u8 sText_AtkSnappedOutOfConfusion[] = _("{B_ATK_NAME_WITH_PREFIX} snapped out of confusion!");
static const u8 sText_EffBecameConfused[] = _("{B_EFF_NAME_WITH_PREFIX} became confused!");
static const u8 sText_DefAlreadyConfused[] = _("{B_DEF_NAME_WITH_PREFIX} is already confused!");
static const u8 sText_DefFellInLove[] = _("{B_DEF_NAME_WITH_PREFIX} fell in love!");
static const u8 sText_AtkIsInLoveWithPkmn[] = _("{B_ATK_NAME_WITH_PREFIX} is in love with {B_SCR_ACTIVE_NAME_WITH_PREFIX}!");
static const u8 sText_AtkImmobilizedByLove[] = _("{B_ATK_NAME_WITH_PREFIX} is immobilized by love!");
static const u8 sText_PkmnTypeChangedToBuff1[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s type changed to {B_BUFF1}!");
static const u8 sText_AtkFlinched[] = _("{B_ATK_NAME_WITH_PREFIX} flinched!");
static const u8 sText_DefRegainedHealth[] = _("{B_DEF_NAME_WITH_PREFIX} regained health!");
static const u8 sText_DefHPFull[] = _("{B_DEF_NAME_WITH_PREFIX}'s HP is full!");
static const u8 sText_AtkTeamCloakedInMysticalVeil[] = _("{B_ATK_TEAM_PREFIX} became cloaked in a mystical veil!");
static const u8 sText_Buff1RaisedAtkTeamBuff2[] = _("{B_BUFF1} raised {B_ATK_TEAM_PREFIX}'s {B_BUFF2}!");
static const u8 sText_DefTeamSafeguardProtected[] = _("{B_DEF_TEAM_PREFIX} is protected by Safeguard!");
static const u8 sText_AtkTeamSafeguardExpired[] = _("{B_ATK_TEAM_PREFIX} is no longer protected by Safeguard!");
static const u8 sText_AtkTeamBuff1WoreOff[] = _("{B_ATK_TEAM_PREFIX}'s {B_BUFF1} wore off!");
static const u8 sText_DefWentToSleep[] = _("{B_DEF_NAME_WITH_PREFIX} went to sleep!");
static const u8 sText_DefSleptHealthy[] = _("{B_DEF_NAME_WITH_PREFIX} slept and became healthy!");
static const u8 sText_ButItFailed[] = _("But it failed!");
static const u8 sText_AtkWhippedWhirlwind[] = _("{B_ATK_NAME_WITH_PREFIX} whipped up a whirlwind!");
static const u8 sText_AtkBecameCloakedInSunlight[] = _("{B_ATK_NAME_WITH_PREFIX} became cloaked in a harsh light!");
static const u8 sText_AtkBurrowedUnderTheGround[] = _("{B_ATK_NAME_WITH_PREFIX} burrowed its way under the ground!");
static const u8 sText_AtkTuckedInItsHead[] = _("{B_ATK_NAME_WITH_PREFIX} tucked in its head!");
static const u8 sText_AtkSprangUp[] = _("{B_ATK_NAME_WITH_PREFIX} sprang up!");
static const u8 sText_AtkHidUnderwater[] = _("{B_ATK_NAME_WITH_PREFIX} hid underwater!");
static const u8 sText_AtkFlewHigh[] = _("{B_ATK_NAME_WITH_PREFIX} flew up high!");
static const u8 sText_AtkAbsorbedLight[] = _("{B_ATK_NAME_WITH_PREFIX} absorbed light!");
static const u8 sText_EffSqueezedByPkmn[] = _("{B_EFF_NAME_WITH_PREFIX} was squeezed by {B_SCR_ACTIVE_NAME_WITH_PREFIX}!");
static const u8 sText_EffWrappedByPkmn[] = _("{B_EFF_NAME_WITH_PREFIX} was wrapped by {B_SCR_ACTIVE_NAME_WITH_PREFIX}!");
static const u8 sText_EffTrappedInVortex[] = _("{B_EFF_NAME_WITH_PREFIX} became trapped in the vortex!");
static const u8 sText_PkmnClampedEff[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} clamped {B_EFF_NAME_WITH_PREFIX}!");
static const u8 sText_EffTrappedByCurrMove[] = _("{B_EFF_NAME_WITH_PREFIX} became trapped by {B_CURRENT_MOVE}!");
static const u8 sText_AtkHurtByBuff1[] = _("{B_ATK_NAME_WITH_PREFIX} is hurt by {B_BUFF1}!");
static const u8 sText_AtkFreedFromBuff1[] = _("{B_ATK_NAME_WITH_PREFIX} was freed from {B_BUFF1}!");
static const u8 sText_AtkKeptAndCrashed[] = _("{B_ATK_NAME_WITH_PREFIX} kept going and crashed!");
static const u8 sText_AtkTeamShroudedInMist[] = _("{B_ATK_TEAM_PREFIX} became shrouded in mist!");
static const u8 sText_AtkGetPumped[] = _("{B_ATK_NAME_WITH_PREFIX} is getting pumped!");
static const u8 sText_AtkHitWithRecoil[] = _("{B_ATK_NAME_WITH_PREFIX} is hit with recoil!");
static const u8 sText_AtkBuffetedBySandstorm[] = _("{B_ATK_NAME_WITH_PREFIX} is buffeted by the sandstorm!");
static const u8 sText_AtkPeltedByHail[] = _("{B_ATK_NAME_WITH_PREFIX} is pelted by hail!");
static const u8 sText_DefWasSeeded[] = _("{B_DEF_NAME_WITH_PREFIX} was seeded!");
static const u8 sText_DefEvadedAttack[] = _("{B_DEF_NAME_WITH_PREFIX} evaded the attack!");
static const u8 sText_DefSappedByLeechSeed[] = _("{B_DEF_NAME_WITH_PREFIX}'s health is sapped by Leech Seed!");
static const u8 sText_AtkFastAsleep[] = _("{B_ATK_NAME_WITH_PREFIX} is fast asleep.");
static const u8 sText_AtkWokeUp[] = _("{B_ATK_NAME_WITH_PREFIX} woke up!");
static const u8 sText_AtkWokeUpInUproar[] = _("{B_ATK_NAME_WITH_PREFIX} woke up in the uproar!");
static const u8 sText_DefEnduredHit[] = _("{B_DEF_NAME_WITH_PREFIX} endured the hit!");
static const u8 sText_OneHitKO[] = _("It's a one-hit KO!");
static const u8 sText_AtkStoringEnergy[] = _("{B_ATK_NAME_WITH_PREFIX} is storing energy!");
static const u8 sText_AtkUnleashedEnergy[] = _("{B_ATK_NAME_WITH_PREFIX} unleashed energy!");
static const u8 sText_FizzlesBuff1TypeAttack[] = _("The {B_BUFF1}-type attack fizzled out in the heavy rain!");
static const u8 sText_EvaporateBuff1TypeAttack[] = _("The {B_BUFF1}-type attack evaporated in the harsh sunlight!");
static const u8 sText_AtkFatigueConfusion[] = _("{B_ATK_NAME_WITH_PREFIX} became confused due to fatigue!");
static const u8 sText_PlayerPickedUpMoney[] = _("{B_PLAYER_NAME} picked up ¥{B_BUFF1}!\p");
static const u8 sText_DefUnaffected[] = _("{B_DEF_NAME_WITH_PREFIX} is unaffected!");
static const u8 sText_AtkTransformedIntoBuff1[] = _("{B_ATK_NAME_WITH_PREFIX} transformed into {B_BUFF1}!");
static const u8 sText_AtkMadeSubstitute[] = _("{B_ATK_NAME_WITH_PREFIX} put in a substitute!");
static const u8 sText_AtkHasSubstitute[] = _("{B_ATK_NAME_WITH_PREFIX} already has a substitute!");
static const u8 sText_TooWeakForSubstitute[] = _("It was too weak to make a substitute!");
static const u8 sText_SubstituteTookDamageForDef[] = _("The substitute took damage for {B_DEF_NAME_WITH_PREFIX}!\p");
static const u8 sText_DefSubstituteFaded[] = _("{B_DEF_NAME_WITH_PREFIX}'s substitute faded!\p");
static const u8 sText_AtkMustRecharge[] = _("{B_ATK_NAME_WITH_PREFIX} must recharge!");
static const u8 sText_AtkTookAimAtDef[] = _("{B_ATK_NAME_WITH_PREFIX} took aim at {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_DefBuff1WasDisabled[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1} was disabled!");
static const u8 sText_AtkCurrMoveIsDisabled[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_CURRENT_MOVE} is disabled!\p");
static const u8 sText_AtkDisabledNoMore[] = _("{B_ATK_NAME_WITH_PREFIX} is disabled no more!");
static const u8 sText_DefReceivedEncore[] = _("{B_DEF_NAME_WITH_PREFIX} received an encore!");
static const u8 sText_AtkEncoreEnded[] = _("{B_ATK_NAME_WITH_PREFIX}'s encore ended!");
static const u8 sText_AtkTryingToTakeFoeDown[] = _("{B_ATK_NAME_WITH_PREFIX} is trying to take its foe down with it!");
static const u8 sText_DefTookAttackerDownWithIt[] = _("{B_DEF_NAME_WITH_PREFIX} took its attacker down with it!");
static const u8 sText_ReducedPPOfDefBuff1ByBuff2[] = _("It reduced the PP of the {B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1} by {B_BUFF2}!");
static const u8 sText_AtkStoleDefLastItem[] = _("{B_ATK_NAME_WITH_PREFIX} stole {B_DEF_NAME_WITH_PREFIX}'s {B_LAST_ITEM}!");
static const u8 sText_DefCanNoLongerEscape[] = _("{B_DEF_NAME_WITH_PREFIX} can no longer escape!");
static const u8 sText_DefBeganHavingNightmare[] = _("{B_DEF_NAME_WITH_PREFIX} began having a nightmare!");
static const u8 sText_AtkLockedInNightmare[] = _("{B_ATK_NAME_WITH_PREFIX} is locked in a nightmare!");
static const u8 sText_SpikesScattered[] = _("Spikes were scattered all around the feet of the foe's team!");
static const u8 sText_DefWasIdentified[] = _("{B_DEF_NAME_WITH_PREFIX} was identified!");
static const u8 sText_AtkPerishCountFellToBuff1[] = _("{B_ATK_NAME_WITH_PREFIX}'s perish count fell to {B_BUFF1}!");
static const u8 sText_AtkBracedItself[] = _("{B_ATK_NAME_WITH_PREFIX} braced itself!");
static const u8 sText_MagnitudeBuff1[] = _("Magnitude {B_BUFF1}!");
static const u8 sText_AtkCopiedDefStatChanges[] = _("{B_ATK_NAME_WITH_PREFIX} copied {B_DEF_NAME_WITH_PREFIX}'s stat changes!");
static const u8 sText_AtkFledFromBattle[] = _("{B_ATK_NAME_WITH_PREFIX} fled from battle!");
static const u8 sText_AtkForesawAttack[] = _("{B_ATK_NAME_WITH_PREFIX} foresaw an attack!");
static const u8 sText_AtkChoseCurrMoveAsDestiny[] = _("{B_ATK_NAME_WITH_PREFIX} chose {B_CURRENT_MOVE} as its destiny!");
static const u8 sText_DefTookTheCurrMoveAttack[] = _("{B_DEF_NAME_WITH_PREFIX} took the {B_CURRENT_MOVE} attack!");
static const u8 sText_AtkBecameCenterOfAttention[] = _("{B_ATK_NAME_WITH_PREFIX} became the center of attention!");
static const u8 sText_Buff1TurnedIntoCurrMove[] = _("{B_BUFF1} turned into {B_CURRENT_MOVE}!");
static const u8 sText_AtkHasNoMovesLeft[] = _("{B_ATK_NAME_WITH_PREFIX} has no moves left!\p");
static const u8 sText_DefSubjectedToTorment[] = _("{B_DEF_NAME_WITH_PREFIX} was subjected to torment!");
static const u8 sText_AtkCantUseMoveTorment[] = _("{B_ATK_NAME_WITH_PREFIX} can't use the same move in a row due to the torment!\p");
static const u8 sText_AtkTighteningFocus[] = _("{B_ATK_NAME_WITH_PREFIX} is tightening its focus!");
static const u8 sText_DefFellForTheTaunt[] = _("{B_DEF_NAME_WITH_PREFIX} fell for the taunt!");
static const u8 sText_AtkCantUseCurrMoveAfterTaunt[] = _("{B_ATK_NAME_WITH_PREFIX} can't use {B_CURRENT_MOVE} after the taunt!\p");
static const u8 sText_AtkReadyToHelpDef[] = _("{B_ATK_NAME_WITH_PREFIX} is ready to help {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_AtkCopiedDefAbl[] = _("{B_ATK_NAME_WITH_PREFIX} copied the {B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}!");
static const u8 sText_Buff1WishCameTrue[] = _("{B_BUFF1}'s wish came true!");
static const u8 sText_AtkPlantedRoots[] = _("{B_ATK_NAME_WITH_PREFIX} planted its roots!");
static const u8 sText_AtkAbsorbedNutrients[] = _("{B_ATK_NAME_WITH_PREFIX} absorbed nutrients with its roots!");
static const u8 sText_DefAnchoredItself[] = _("{B_DEF_NAME_WITH_PREFIX} anchored itself with its roots!");
static const u8 sText_DefGrewDrowsy[] = _("{B_DEF_NAME_WITH_PREFIX} grew drowsy!");
static const u8 sText_AtkSwappedAbilitiesWithTarget[] = _("{B_ATK_NAME_WITH_PREFIX} swapped abilities with its target!");
static const u8 sText_DefBouncedCurrMoveBack[] = _("{B_DEF_NAME_WITH_PREFIX} bounced the {B_CURRENT_MOVE} back!");
static const u8 sText_AtkShroudedItselfWithCurrMove[] = _("{B_ATK_NAME_WITH_PREFIX} shrouded itself with {B_CURRENT_MOVE}!");
static const u8 sText_CriticalHit[] = _("{COLOR 2}{SHADOW 9}A critical hit!");
static const u8 sText_CriticalHitOnDef[] = _("{COLOR 2}{SHADOW 9}A critical hit{COLOR 1}{SHADOW 6} on {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_123Poof[] = _("{PAUSE 32}1, {PAUSE 15}2, and{PAUSE 15}… {PAUSE 15}… {PAUSE 15}… {PAUSE 15}{PLAY_SE SE_BALL_BOUNCE_1}Poof!\p");
static const u8 sText_AndEllipsis[] = _("And…\p");
static const u8 sText_NotVeryEffective[] = _("It's not very effective…");
static const u8 sText_NotVeryEffectiveOnDef[] = _("It's not very effective on {B_DEF_NAME_WITH_PREFIX}…");
static const u8 sText_SuperEffective[] = _("It's super effective!");
static const u8 sText_SuperEffectiveOnDef[] = _("It's super effective on {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_AtkIsLoafingAround[] = _("{B_ATK_NAME_WITH_PREFIX} is loafing around!");
static const u8 sText_AtkWontObey[] = _("{B_ATK_NAME_WITH_PREFIX} won't obey!");
static const u8 sText_AtkTurnedAway[] = _("{B_ATK_NAME_WITH_PREFIX} turned away!");
static const u8 sText_AtkPretendNotNotice[] = _("{B_ATK_NAME_WITH_PREFIX} pretended not to notice!");
static const u8 sText_Buff1IsTooNervousToEatBerries[] = _("{B_BUFF1} is too nervous to eat berries!");
static const u8 sText_EffPreventsPoisoningWith[] = _("{B_EFF_NAME_WITH_PREFIX}'s {B_EFF_ABILITY} prevents poisoning!");
static const u8 sText_ABoosted[] = _(" a boosted");
static const u8 sText_StartedToRain[] = _("It started to rain!");
static const u8 sText_GotAwaySafely[] = _("{PLAY_SE SE_FLEE}Got away safely!\p");
static const u8 sText_WildBuff1Fled[] = _("{PLAY_SE SE_FLEE}Wild {B_BUFF1} fled!");
static const u8 sText_DefProtectedByDefAbl[] = _("{B_DEF_NAME_WITH_PREFIX} was protected by {B_DEF_ABILITY}!");
static const u8 sText_PkmnAblPreventsAtkFromUseCurrMove[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY} prevents {B_ATK_NAME_WITH_PREFIX} from using {B_CURRENT_MOVE}!");
static const u8 sText_ButNothingHappened[] = _("But nothing happened!");
static const u8 sText_ItHurtItselfInConfusion[] = _("It hurt itself in its confusion!");
static const u8 sText_TheCurrMoveFailed[] = _("The {B_CURRENT_MOVE} failed!");
static const u8 sText_DefHadHPRestored[] = _("{B_DEF_NAME_WITH_PREFIX} had its HP restored.");
static const u8 sText_PowerOfDefBuff1TypeMovesRose[] = _("The power of {B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1}-type moves rose!");
static const u8 sText_ItTracedDefAbl[] = _("It traced {B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}!");
static const u8 sText_AtkWasHurt[] = _("{B_ATK_NAME_WITH_PREFIX} was hurt!");
static const u8 sText_PokemonHearingFluteAwoke[] = _("The Pokémon hearing the flute awoke!");
static const u8 sText_GotchaDefCaught[] = _("Gotcha!\n{B_DEF_NAME} was caught!{WAIT_SE}{PLAY_BGM MUS_CAUGHT}\p");
static const u8 sText_DefDataAddedToDex[] = _("{B_DEF_NAME}'s data was added to the Pokédex.\p");
static const u8 sText_OutOfSafariBalls[] = _("{PLAY_SE SE_DING_DONG}Announcer: You're out of Safari Balls! Game over!\p");
static const u8 sText_OhNoPokemonBrokeFree[] = _("Oh, no!\nThe Pokémon broke free!");
static const u8 sText_ItAppearedToBeCaught[] = _("Aww!\nIt appeared to be caught!");
static const u8 sText_AarghAlmostHadIt[] = _("Aargh!\nAlmost had it!");
static const u8 sText_ShootSoClose[] = _("Shoot!\nIt was so close, too!");
static const u8 sText_TrainerBlockedBall[] = _("The Trainer blocked the Ball!");
static const u8 sText_DontBeAThief[] = _("Don't be a thief!");
static const u8 sText_ItDodgedBall[] = _("It dodged the thrown Ball!\nThis Pokémon can't be caught!");
static const u8 sText_PkmnWatchingCarefully[] = _("{B_OPPONENT_MON1_NAME} is watching carefully!");
static const u8 sText_PkmnIsAngry[] = _("{B_OPPONENT_MON1_NAME} is angry!");
static const u8 sText_PkmnIsEating[] = _("{B_OPPONENT_MON1_NAME} is eating!");
static const u8 sText_ThrewARock[] = _("{B_PLAYER_NAME} threw a Rock at the {B_OPPONENT_MON1_NAME}!");
static const u8 sText_ThrewSomeBait[] = _("{B_PLAYER_NAME} threw some Bait at the {B_OPPONENT_MON1_NAME}!");
static const u8 sText_YouThrowABallNowRight[] = _("You throw a Ball now, right?\nI… I'll do my best!");
static const u8 sText_NoRunningFromTrainers[] = _("No! There's no running from a Trainer battle!\p");
static const u8 sText_CantEscape[] = _("Can't escape!\p");
static const u8 sText_AtkCantEscape[] = _("{B_ATK_NAME_WITH_PREFIX} can't escape!");
static const u8 sText_RainContinues[] = _("Rain continues to fall.");
static const u8 sText_RainStopped[] = _("The rain stopped.");
static const u8 sText_SunlightStrong[] = _("The sunlight is strong.");
static const u8 sText_SunlightFaded[] = _("The sunlight faded.");
static const u8 sText_SandstormRages[] = _("The sandstorm rages.");
static const u8 sText_SandstormSubsided[] = _("The sandstorm subsided.");
static const u8 sText_HailContinues[] = _("Hail continues to fall.");
static const u8 sText_HailStopped[] = _("The hail stopped.");
static const u8 sText_FogIsDeep[] = _("The fog is deep…");
static const u8 sText_FogEnded[] = _("The fog disappeared.");
static const u8 sText_MysteriousAirContinues[] = _("The mysterious air current continues to blow!");
static const u8 sText_SandstormKickedUp[] = _("A sandstorm kicked up!");
static const u8 sText_SunlightTurnedHarsh[] = _("The sunlight turned harsh!");
static const u8 sText_StartedHail[] = _("It started to hail!");
static const u8 sText_CoinsScattered[] = _("Coins scattered everywhere!");
static const u8 sText_GiveNicknameCaptured[] = _("Give a nickname to the captured {B_DEF_NAME}?");
static const u8 sText_TheBattlersSharedTheirPain[] = _("The battlers shared their pain!");
static const u8 sText_ABellChimed[] = _("A bell chimed!");
static const u8 sText_SoothingAroma[] = _("A soothing aroma wafted through the area!");
static const u8 sText_AllStatChangesEliminated[] = _("All stat changes were eliminated!");
static const u8 sText_AllPokemonFaintInThreeTurns[] = _("All Pokémon hearing the song will faint in three turns!");
static const u8 sText_NoPPLeft[] = _("There's no PP left for this move!\p");
static const u8 sText_ButNoPPLeft[] = _("But there was no PP left for the move!");
static const u8 sText_AtkIgnoredOrdersAsleep[] = _("{B_ATK_NAME_WITH_PREFIX} ignored orders while asleep!");
static const u8 sText_AtkIgnoredOrders[] = _("{B_ATK_NAME_WITH_PREFIX} ignored orders!");
static const u8 sText_AtkBeganToNap[] = _("{B_ATK_NAME_WITH_PREFIX} began to nap!");
static const u8 sText_EnemyAboutToSwitchPkmn[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME} is\nabout to use {B_BUFF2}.\pWill {B_PLAYER_NAME} change Pokémon?");
static const u8 sText_LastItemAllowsOnlyCurrMove[] = _("{B_LAST_ITEM}'s effect allows only {B_CURRENT_MOVE} to be used!\p");
static const u8 sText_ItemsCantBeUsedNow[] = _("Items can't be used now.{PAUSE 64}");
static const u8 sText_BoxIsFull[] = _("The Box is full!\nYou can't catch any more!\p");
static const u8 sText_EffStatChanged[] = _("{B_EFF_NAME_WITH_PREFIX}'s {B_BUFF1}{B_BUFF2}!");
static const u8 sText_EffStatCantChange[] = _("{B_EFF_NAME_WITH_PREFIX}'s {B_BUFF1} {B_BUFF2}!");
static const u8 sText_DefStatsWontChangeAnymore[] = _("{B_DEF_NAME_WITH_PREFIX}'s stats won't change anymore!");
static const u8 sText_EffCutHPMaxedBuff1[] = _("{B_EFF_NAME_WITH_PREFIX} cut its own HP and maximized {B_BUFF1}!");
static const u8 sText_AtkGotFreeOfPkmnBuff1[] = _("{B_ATK_NAME_WITH_PREFIX} got free of {B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_BUFF1}!");
static const u8 sText_AtkShedLeechSeed[] = _("{B_ATK_NAME_WITH_PREFIX} shed Leech Seed!");
static const u8 sText_AtkBlewAwayHazards[] = _("{B_ATK_NAME_WITH_PREFIX} blew away its hazards!");
static const u8 sText_AtkBeganChargingPower[] = _("{B_ATK_NAME_WITH_PREFIX} began charging power!");
static const u8 sText_EffProtectedByMist[] = _("{B_EFF_NAME_WITH_PREFIX} is protected by Mist!");
static const u8 sText_EffPreventsStatLoss[] = _("{B_EFF_NAME_WITH_PREFIX} prevents stat loss!");
static const u8 sText_EffPreventsBuff1Loss[] = _("{B_EFF_NAME_WITH_PREFIX} prevents {B_BUFF1} loss!");
static const u8 sText_EffRageIsBuilding[] = _("{B_EFF_NAME_WITH_PREFIX}'s rage is building!");
static const u8 sText_EffPreventedFromWorking[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY} prevented {B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY} from working!");
static const u8 sText_EffMaxedItsBuff1[] = _("{B_EFF_NAME_WITH_PREFIX} {B_BUFF2} its {B_BUFF1}!");
static const u8 sText_DefAlreadyHasBurn[] = _("{B_DEF_NAME_WITH_PREFIX} already has a burn.");
static const u8 sText_PkmnAblMadeItIneffective[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY} made it ineffective!");
static const u8 sText_AtkStockpiled[] = _("{B_ATK_NAME_WITH_PREFIX} stockpiled {B_BUFF1}!");
static const u8 sText_AtkCantStockpile[] = _("{B_ATK_NAME_WITH_PREFIX} can't stockpile any more!");
static const u8 sText_ButFailedToCurrMoveAThing[] = _("But it failed to {B_CURRENT_MOVE} a thing!");
static const u8 sText_ElectricityPowerWeakened[] = _("Electricity's power was weakened!");
static const u8 sText_FirePowerWeakened[] = _("Fire's power was weakened!");
static const u8 sText_Buff1EffectsWoreOff[] = _("{B_BUFF1}'s effects wore off!");
static const u8 sText_DefTookTheAttack[] = _("{B_DEF_NAME_WITH_PREFIX} took the attack!");
static const u8 sText_UseNextPkmn[] = _("Use next Pokémon?");
static const u8 sText_Trainer1Pkmn1ComeBack[] = _("{B_TRAINER1_NAME}: {B_OPPONENT_MON1_NAME}, come back!");
static const u8 sText_Trainer1WinText[] = _("{B_TRAINER1_WIN_TEXT}");
static const u8 sText_PlayerWhiteoutAgainstTrainer[] = _("{B_PLAYER_NAME} is out of usable Pokémon!\pPlayer lost against {B_TRAINER1_CLASS} {B_TRAINER1_NAME}!{PAUSE_UNTIL_PRESS}");
static const u8 sText_PlayerPaidAsPrizeMoney[] = _("{B_PLAYER_NAME} paid ¥{B_BUFF1} as the prize money…\p… … … …\p{B_PLAYER_NAME} whited out!{PAUSE_UNTIL_PRESS}");
static const u8 sText_PlayerWhitedOut[] = _("{B_PLAYER_NAME} whited out!{PAUSE_UNTIL_PRESS}");


static const u8 sText_Trainer1RecallPkmn2[] = _("{B_TRAINER1_NAME}: {B_OPPONENT_MON2_NAME}, come back!");
static const u8 sText_Trainer1RecallBoth[] = _("{B_TRAINER1_NAME}: {B_OPPONENT_MON1_NAME} and\n{B_OPPONENT_MON2_NAME}, come back!");
static const u8 sText_PkmnForgotMove[] = _("{B_BUFF1} forgot\n{B_BUFF2}.\p");
static const u8 sText_StopLearningMove[] = _("{PAUSE 32}Stop learning\n{B_BUFF2}?");
static const u8 sText_DidNotLearnMove[] = _("{B_BUFF1} did not learn\n{B_BUFF2}.\p");
static const u8 sText_AvoidedDamage[] = _("{B_DEF_NAME_WITH_PREFIX} avoided\ndamage with {B_DEF_ABILITY}!");
static const u8 sText_PkmnMakesGroundMiss[] = _("{B_DEF_NAME_WITH_PREFIX} makes Ground\nmoves miss with {B_DEF_ABILITY}!");
static const u8 sText_PkmnAvoidedAttack[] = _("{B_DEF_NAME_WITH_PREFIX} avoided\nthe attack!");
static const u8 sText_StatsWontIncrease[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_BUFF1}\nwon't go higher!");
static const u8 sText_StatsWontDecrease[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1}\nwon't go lower!");
static const u8 sText_PkmnUproarKeptAwake[] = _("But {B_SCR_ACTIVE_NAME_WITH_PREFIX}'s Uproar\nkept it awake!");
static const u8 sText_PkmnCausedUproar[] = _("{B_ATK_NAME_WITH_PREFIX} caused\nan Uproar!");
static const u8 sText_PkmnMakingUproar[] = _("{B_ATK_NAME_WITH_PREFIX} is making\nan Uproar!");
static const u8 sText_PkmnCalmedDown[] = _("{B_ATK_NAME_WITH_PREFIX} calmed down.");
static const u8 sText_PkmnCantSleepInUproar[] = _("But {B_DEF_NAME_WITH_PREFIX} can't\nsleep in an Uproar!");
static const u8 sText_PkmnCantSleepInUproar2[] = _("But {B_DEF_NAME_WITH_PREFIX} can't\nsleep in an Uproar!");
static const u8 sText_UproarKeptPkmnAwake[] = _("But the Uproar kept\n{B_DEF_NAME_WITH_PREFIX} awake!");
static const u8 sText_PkmnStayedAwakeUsing[] = _("{B_DEF_NAME_WITH_PREFIX} stayed awake\nusing its {B_DEF_ABILITY}!");
static const u8 sText_PkmnLaidCurse[] = _("{B_ATK_NAME_WITH_PREFIX} cut its own HP and\nlaid a Curse on {B_DEF_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnAfflictedByCurse[] = _("{B_ATK_NAME_WITH_PREFIX} is afflicted\nby the Curse!");
static const u8 sText_PkmnHurtBySpikes[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} is hurt\nby Spikes!");
static const u8 sText_PkmnSwitchedItems[] = _("{B_ATK_NAME_WITH_PREFIX} switched\nitems with its opponent!");
static const u8 sText_PkmnObtainedX[] = _("{B_ATK_NAME_WITH_PREFIX} obtained\n{B_BUFF1}.");
static const u8 sText_PkmnObtainedX2[] = _("{B_DEF_NAME_WITH_PREFIX} obtained\n{B_BUFF2}.");
static const u8 sText_PkmnObtainedXYObtainedZ[] = _("{B_ATK_NAME_WITH_PREFIX} obtained\n{B_BUFF1}.\p{B_DEF_NAME_WITH_PREFIX} obtained\n{B_BUFF2}.");
static const u8 sText_PkmnKnockedOff[] = _("{B_ATK_NAME_WITH_PREFIX} knocked off\n{B_DEF_NAME_WITH_PREFIX}'s {B_LAST_ITEM}!");
static const u8 sText_PkmnSealedOpponentMove[] = _("{B_ATK_NAME_WITH_PREFIX} sealed the\nopponent's moveシsス!");
static const u8 sText_PkmnWantsGrudge[] = _("{B_ATK_NAME_WITH_PREFIX} wants the\nopponent to bear a Grudge!");
static const u8 sText_PkmnLostPPGrudge[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_BUFF1} lost\nall its PP due to the Grudge!");
static const u8 sText_PkmnWaitsForTarget[] = _("{B_ATK_NAME_WITH_PREFIX} waits for its foe\nto make a move!");
static const u8 sText_PkmnSnatchedMove[] = _("{B_DEF_NAME_WITH_PREFIX} Snatched\n{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s move!");
static const u8 sText_XFoundOneY[] = _("{B_ATK_NAME_WITH_PREFIX} found\none {B_LAST_ITEM}!");
static const u8 sText_ForXCommaYZ[] = _("For {B_SCR_ACTIVE_NAME_WITH_PREFIX},\n{B_LAST_ITEM} {B_BUFF1}");
static const u8 sText_PkmnUsedXToGetPumped[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} used\n{B_LAST_ITEM} to hustle!");
static const u8 sText_PkmnLostFocus[] = _("{B_ATK_NAME_WITH_PREFIX} lost its\nfocus and couldn't move!");
static const u8 sText_PkmnWasDraggedOut[] = _("{B_DEF_NAME_WITH_PREFIX} was\ndragged out!\p");
static const u8 sText_TheWallShattered[] = _("The wall shattered!");
static const u8 sText_ButNoEffect[] = _("But it had no effect!");
static const u8 sText_PkmnCantUseMoveSealed[] = _("{B_ATK_NAME_WITH_PREFIX} can't use the\nsealed {B_CURRENT_MOVE}!\p");
static const u8 sText_PkmnRaisedSpeed[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY}\nraised its Speed!");
static const u8 sText_PkmnsXMadeYUseless[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nmade {B_CURRENT_MOVE} useless!");
static const u8 sText_PkmnPreventsParalysisWith[] = _("{B_EFF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevents paralysis!");
static const u8 sText_PkmnPreventsRomanceWith[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevents romance!");
static const u8 sText_PkmnPreventsConfusionWith[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevents confusion!");
static const u8 sText_PkmnAnchorsItselfWith[] = _("{B_DEF_NAME_WITH_PREFIX} anchors\nitself with {B_DEF_ABILITY}!");
static const u8 sText_PkmnCutsAttackWith[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\ncuts {B_DEF_NAME_WITH_PREFIX}'s Attack!");
static const u8 sText_PkmnsXBlocksY[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nblocks {B_CURRENT_MOVE}!");
static const u8 sText_PkmnsXBlocksY2[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nblocks {B_CURRENT_MOVE}!");
static const u8 sText_PkmnsXRestoredHPALittle2[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY}\nrestored its HP a little!");
static const u8 sText_PkmnsXWhippedUpSandstorm[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nwhipped up a sandstorm!");
static const u8 sText_PkmnsXIntensifiedSun[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nintensified the sun's rays!");
static const u8 sText_PkmnsXInfatuatedY[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\ninfatuated {B_ATK_NAME_WITH_PREFIX}!");
static const u8 sText_PkmnsXMadeYIneffective[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nmade {B_CURRENT_MOVE} ineffective!");
static const u8 sText_PkmnsXCuredYProblem[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\ncured its {B_BUFF1} problem!");
static const u8 sText_ItSuckedLiquidOoze[] = _("It sucked up the\n{B_DEF_ABILITY}!");
static const u8 sText_PkmnTransformed[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} transformed!");
const u8 gText_PkmnsXPreventsSwitching[] = _("{B_BUFF1}'s {B_LAST_ABILITY}\nprevents switching!\p");
static const u8 sText_PkmnsXPreventsFlinching[] = _("{B_EFF_NAME_WITH_PREFIX}'s won't flinch\nbecause of its {B_EFF_ABILITY}!");
static const u8 sText_PkmnsXPreventsYsZ[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY}\nprevents {B_DEF_NAME_WITH_PREFIX}'s\l{B_DEF_ABILITY} from working!"); // unused
static const u8 sText_PkmnsXCuredItsYProblem[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\ncured its {B_BUFF1} problem!");
static const u8 sText_PkmnsXHadNoEffectOnY[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nhad no effect on {B_EFF_NAME_WITH_PREFIX}!"); // unused
static const u8 sText_TooScaredToMove[] = _("{B_ATK_NAME_WITH_PREFIX} is too scared to move!");
static const u8 sText_GetOutGetOut[] = _("Ghost: Get out…… Get out……");
static const u8 sText_StatSharply[] = _("sharply ");
const u8 gBattleText_Rose[] = _("rose!");
static const u8 sText_StatHarshly[] = _("harshly ");
static const u8 sText_StatFell[] = _("fell!");
static const u8 sText_PkmnsStatChanged[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_BUFF1}\n{B_BUFF2}");
const u8 gText_PkmnsStatChanged2[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1}\n{B_BUFF2}");
static const u8 sText_UsingXTheYOfZN[] = _("Using {B_LAST_ITEM}, the {B_BUFF1}\nof {B_SCR_ACTIVE_NAME_WITH_PREFIX} {B_BUFF2}");
static const u8 sText_PkmnsStatChanged3[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_BUFF1}\n{B_BUFF2}");
static const u8 sText_PkmnsStatChanged4[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1}\n{B_BUFF2}");
static const u8 sText_StatsWontDecrease2[] = _("{B_DEF_NAME_WITH_PREFIX}'s stats won't\ngo any lower!");
static const u8 sText_HMMovesCantBeForgotten[] = _("HM moves can't be\nforgotten now.\p");
static const u8 sText_PkmnFledUsingIts[] = _("{PLAY_SE SE_FLEE}{B_ATK_NAME_WITH_PREFIX} fled\nusing its {B_LAST_ITEM}!\p");
static const u8 sText_PkmnFledUsing[] = _("{PLAY_SE SE_FLEE}{B_ATK_NAME_WITH_PREFIX} fled\nusing {B_ATK_ABILITY}!\p");
static const u8 sText_SilphScopeUnveil[] = _("Silph Scope unveiled the Ghost's\nidentity!");
static const u8 sText_TheGhostWas[] = _("The Ghost was Marowak!\p\n");
static const u8 sText_Trainer1WithdrewPkmn[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}\nwithdrew {B_BUFF1}!");
static const u8 sText_AromaVeilProtected[] = _("{B_DEF_NAME_WITH_PREFIX} is protected\nby an aromatic veil!");
static const u8 sText_FlowerVeilProtected[] = _("{B_EFF_NAME_WITH_PREFIX} surrounded\nitself with a veil of petals!");
static const u8 sText_SweetVeilProtected[] = _("{B_DEF_NAME_WITH_PREFIX} surrounded\nitself with a veil of sweetness!");
static const u8 sText_HeavyRainLifted[] = _("The heavy rain has lifted!{PAUSE 64}");
static const u8 sText_ExtremeSunlightFaded[] = _("The extreme sunlight faded.{PAUSE 64}");
static const u8 sText_StrongWindsDissipated[] = _("The mysterious strong winds\nhave dissipated!{PAUSE 64}");
static const u8 sText_PkmnXMadeItHail[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nmade it hail!");
static const u8 sText_HeavyRainBeganFall[] = _("A heavy rain began to fall!");
static const u8 sText_SunlightTurnedExtremelyHarsh[] = _("The sunlight turned\nextremely harsh!");
static const u8 sText_MysteriousAirProtectingPkmn[] = _("A mysterious air current is\nprotecting Flying-type Pokémon!");
static const u8 sText_WeatherEffectsDisappeared[] = _("The effects of weather\ndisappeared!");
static const u8 sText_PkmnBreaksTheMold[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} breaks the mold!");
static const u8 sText_PkmnCantGetGoing[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} can't\nget it going!");
static const u8 sText_RadiatingABlazingAura[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} is radiating\na blazing aura!");
static const u8 sText_RadiatingABurstingAura[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} is radiating\na bursting aura!");
static const u8 sText_RadiatingADarkAura[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} is radiating\na dark aura!");
static const u8 sText_RadiatingAFairyAura[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} is radiating\na fairy aura!");
static const u8 sText_PkmnReversedAuras[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} reversed all\nother Pokémon's auras!");
static const u8 sText_PkmnIsDrowsing[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} is drowsing!");
static const u8 sText_PkmnUnderwentATransformation[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} underwent\na heroic transformation!");
static const u8 sText_AbilityTriggered[] = _("{B_SCR_ACTIVE_ABILITY} triggered!");
static const u8 sText_PkmnGainSomeOfHp[] = _("{B_ATK_NAME_WITH_PREFIX} gain some\nof its HP!");
static const u8 sText_PkmnLostSomeOfHp[] = _("{B_ATK_NAME_WITH_PREFIX} lost some\nof its HP!");
static const u8 sText_AtkAblCuredEffProblem[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY}\ncured {B_EFF_NAME_WITH_PREFIX}'s\l{B_BUFF1} problem!");
static const u8 sText_PkmnGotItsActTogether[] = _("{B_ATK_NAME_WITH_PREFIX} got its\nact together!");
static const u8 sText_DefIsTormented[] = _("{B_DEF_NAME_WITH_PREFIX} is\ntormented!");
static const u8 sText_AtkHarvestedItsLastItem[] = _("{B_ATK_NAME_WITH_PREFIX} harvested\nits {B_LAST_ITEM}!");
static const u8 sText_AtkCantUseCurrMove[] = _("{B_ATK_NAME_WITH_PREFIX} cannot\nuse {B_CURRENT_MOVE}!");
static const u8 sText_DefLastAblRaisedItsBuff1[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_LAST_ABILITY}\nraised its {B_BUFF1}!");
static const u8 sText_DefAblDisabledAtkCurrMove[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\ndisabled {B_ATK_NAME_WITH_PREFIX}'s\l{B_CURRENT_MOVE}!");
static const u8 sText_PkmnIllusionOff[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s illusion wore off!");
static const u8 sText_AtkAbilityBecameAbl[] = _("{B_ATK_NAME_WITH_PREFIX}'s ability became\n{B_ATK_ABILITY}!");
static const u8 sText_PkmnShuddered[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} shuddered!");
static const u8 sText_AttackerAbilityRaisedStat[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY}\nraised its {B_BUFF1}!");
static const u8 sText_AlertedToDefBuff1[] = _("It was alerted to\n{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1}!");
static const u8 sText_AtkFriskedFoundLastItem[] = _("{B_ATK_NAME_WITH_PREFIX} frisked {B_DEF_NAME_WITH_PREFIX}\nand found its {B_LAST_ITEM}!");
static const u8 sText_AtkCutHpMinimizedAttack[] = _("{B_ATK_NAME_WITH_PREFIX} cut its own HP\nand minimized Attack!");
static const u8 sText_AtkAblRaisedItsSpeed[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY}\nraised its Speed!");
static const u8 sText_AllScreensCleansed[] = _("All screens on the field were\ncleansed!");
static const u8 sText_PerishInThree[] = _("Both Pokémon will perish\nin three turns!");
static const u8 sText_DefStatChangesWereReset[] = _("{B_DEF_NAME_WITH_PREFIX}'s\nstat changes were reset!");
static const u8 sText_CurrMoveChargedDefPower[] = _("Being hit by {B_CURRENT_MOVE}\ncharged {B_DEF_NAME_WITH_PREFIX} with power!");
static const u8 sText_NeutralizingGasOver[] = _("The effects of\n{B_BUFF1} wore off!");
static const u8 sText_PkmnAblFilledTheArea[] = _("{B_SCR_ACTIVE_ABILITY} filled the area!");
static const u8 sText_PastelVeilProtected[] = _("{B_DEF_NAME_WITH_PREFIX} is protected\nby a pastel veil!");
static const u8 sText_PastelVeilCuredStatus[] = _("{B_DEF_NAME_WITH_PREFIX} was cured\nof its poisoning!");
static const u8 sText_StartedSchooling[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} formed\na school!");
static const u8 sText_StoppedSchooling[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} stopped\nschooling!");
static const u8 sText_DisguiseServedAsDecoy[] = _("Its disguise served it as\na decoy!");
static const u8 sText_DisguiseBusted[] = _("{B_DEF_NAME_WITH_PREFIX}'s disguise\nwas busted!");
static const u8 sText_PkmnGotOverInfatuation[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} got over\nits infatuation!");
static const u8 sText_PkmnShookOffTaunt[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} shook off\nthe taunt!");
static const u8 sText_pkmnExertingPressure[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} is exerting\nits {B_SCR_ACTIVE_ABILITY}!");
static const u8 sText_ChangedToBlade[] = _("Changed to Blade Forme!");
static const u8 sText_ChangedToShield[] = _("Changed to Shield Forme!");
static const u8 sText_PkmnGainedStrength[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} gained\nstrength from the fallen!");
static const u8 sText_PkmnAblWeakenedBuff1OfAllPkmn[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nweakened the {B_BUFF1} of all\lsurrounding Pokémon!");
static const u8 sText_AtkFoundOneLastItem[] = _("{B_ATK_NAME_WITH_PREFIX}'s found\none {B_LAST_ITEM}!");
static const u8 sText_PkmnHasTwoAbilities[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} has two\nabilities!");
static const u8 sText_PkmnAblWasTakenOver[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nwas taken over!");
static const u8 sText_ItemRestoredAtkHealth[] = _("{B_ITEM_USE_SPECIES_NAME} had its\nHP restored!");
static const u8 sText_ItemCuredAtkStatus[] = _("{B_ITEM_USE_SPECIES_NAME} had\nits status healed!");
static const u8 sText_ItemRestoredAtkPP[] = _("{B_ITEM_USE_SPECIES_NAME} had its\nPP restored!");
static const u8 sText_Buff2CuredItsBuff1Problem[] = _("{B_BUFF2} was cured\nof its {B_BUFF1}.");
static const u8 sText_AbilityActivated[] = _("{B_SCR_ACTIVE_ABILITY} activated!");
static const u8 sText_AbilityDeactivated[] = _("{B_SCR_ACTIVE_ABILITY} deactivated!");
const u8 gStatusConditionString_MentalState[] = _("mental");

static const u8 sText_HP2[] = _("HP");
static const u8 sText_Attack2[] = _("Attack");
static const u8 sText_Defense2[] = _("Defense");
static const u8 sText_Speed[] = _("Speed");
static const u8 sText_SpAtk2[] = _("Sp. Atk");
static const u8 sText_SpDef2[] = _("Sp. Def");
static const u8 sText_Accuracy[] = _("accuracy");
static const u8 sText_Evasiveness[] = _("evasiveness");

static const u8 *const sBattleStatNamesTable[NUM_BATTLE_STATS] =
{
    [STAT_HP]      = sText_HP2,
    [STAT_ATK]     = sText_Attack2,
    [STAT_DEF]     = sText_Defense2,
    [STAT_SPEED]   = sText_Speed,
    [STAT_SPATK]   = sText_SpAtk2,
    [STAT_SPDEF]   = sText_SpDef2,
    [STAT_ACC]     = sText_Accuracy,
    [STAT_EVASION] = sText_Evasiveness
};

static const u8 sText_PokeblockWasTooSpicy[] = _("was too spicy!"); //
static const u8 sText_PokeblockWasTooDry[] = _("was too dry!");
static const u8 sText_PokeblockWasTooSweet[] = _("was too sweet!");
static const u8 sText_PokeblockWasTooBitter[] = _("was too bitter!");
static const u8 sText_PokeblockWasTooSour[] = _("was too sour!");

const u8 *const gPokeblockWasTooXStringTable[] = {
    sText_PokeblockWasTooSpicy,
    sText_PokeblockWasTooDry,
    sText_PokeblockWasTooSweet,
    sText_PokeblockWasTooBitter,
    sText_PokeblockWasTooSour
};

const u8 gUnknown_83FD78A[] = _("よけられた!\nこいつは つかまりそうにないぞ!");
static const u8 gUnknown_83FD8B6[] = _("ナゾ");
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
static const u8 sText_PkmnHungOnWithX[] = _("{B_DEF_NAME_WITH_PREFIX} hung on\nusing its {B_LAST_ITEM}!");

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

const u16 gStatUpStringIds[] = {
    STRINGID_PKMNSSTATCHANGED,
    STRINGID_PKMNSSTATCHANGED2,
    STRINGID_STATSWONTINCREASE,
    STRINGID_EMPTYSTRING,
    STRINGID_USINGXTHEYOFZN,
    STRINGID_PKMNUSEDXTOGETPUMPED
};

const u16 gStatDownStringIds[] = {
    STRINGID_PKMNSSTATCHANGED3,
    STRINGID_PKMNSSTATCHANGED4,
    STRINGID_STATSWONTDECREASE,
    STRINGID_EMPTYSTRING
};

const u16 gBerryEffectStringIds[] = {
    STRINGID_PKMNSITEMCUREDPROBLEM,
    STRINGID_PKMNSITEMNORMALIZEDSTATUS
};

const u16 gItemSwapStringIds[] = {
    STRINGID_PKMNOBTAINEDX,
    STRINGID_PKMNOBTAINEDX2,
    STRINGID_PKMNOBTAINEDXYOBTAINEDZ
};

const u16 gSwitchInWeatherAbilitiesStrings[] = {
	STRINGID_STARTEDTORAIN,
	STRINGID_PKMNSXWHIPPEDUPSANDSTORM,
	STRINGID_PKMNSXINTENSIFIEDSUN,
	STRINGID_PKMNSXMADEITHAIL,
	STRINGID_HEAVYRAINBEGANFALL,
	STRINGID_EXTREHARSHSUNLIGHT,
	STRINGID_MYSTERIOUSAIRPROTECTINGPKMN
};

const u16 gSwitchInAbilitiesMsgStringIds[] = {
	STRINGID_PKMNBREAKSTHEMOLD,
	STRINGID_PKMNCANTGETGOING,
	STRINGID_BUFF1ISNERVOUSTOEATBERRIES,
	STRINGID_RADIATINGABLAZINGAURA,
	STRINGID_RADIATINGABURSTINGAURA,
	STRINGID_RADIATINGADARKAURA,
	STRINGID_RADIATINGAFAIRYAURA,
	STRINGID_REVERSEDALLOTHERAURAS,
	STRINGID_PKMNISDROWSING,
	STRINGID_PKMNUNDERWENTTRANSFORMATION,
	STRINGID_PKMNSHUDDERED,
	STRINGID_ALERTEDTODEFBUFF1,
	STRINGID_ALLSCREENSCLEANSED,
	STRINGID_DEFSTATCHANGESWERERESET,
	STRINGID_ATKCOPIEDDEFSTATCHANGES,
	STRINGID_PKMNHASTWOABILITIES,
	STRINGID_PKMNEXERTINGPRESSURE,
	STRINGID_PKMNGAINEDSTRENGTH,
	STRINGID_PKMNABLWEAKENEDBUFF1,
};

const u16 gRainDishStringIds[] = {
	STRINGID_PKMNSXRESTOREDHPALITTLE2,
	STRINGID_PKMNGAINSOMEOFHP,
};

const u16 gFormChangeAbilitiesStrings[] = {
	STRINGID_ABILITYTRIGGERED,
	STRINGID_PKMNTRANSFORMED,
	STRINGID_CHANGEDTOBLADE,
	STRINGID_CHANGEDTOSHIELD,
	STRINGID_ABILITYACTIVATED,
	STRINGID_ABILITYDEACTIVATED,
};

const u16 gStartedSchoolingStringIds[] = {
	STRINGID_STARTEDSCHOOLING,
	STRINGID_STOPPEDSCHOOLING,
};

const u8 gText_PkmnIsEvolving[] = _("What?\n{STR_VAR_1} is evolving!");
const u8 gText_CongratsPkmnEvolved[] = _("Congratulations! Your {STR_VAR_1}\nevolved into {STR_VAR_2}!{WAIT_SE}\p");
const u8 gText_PkmnStoppedEvolving[] = _("Huh? {STR_VAR_1}\nstopped evolving!\p");
const u8 gText_EllipsisQuestionMark[] = _("……?\p");
const u8 gText_WhatWillBufferDo[] = _("What will\n{B_BUFF1} do?");
const u8 gText_WhatWillPlayerThrow[] = _("What will {B_PLAYER_NAME}\nthrow?");
const u8 gText_TheOldMan[] = _("the old man");
const u8 gText_LinkStandby[] = _("{PAUSE 16}Link standby…");
const u8 gText_BattleMenu[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Fight{CLEAR_TO 56}Bag\nPokémon{CLEAR_TO 56}Run");
const u8 gUnknown_83FE747[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Ball{CLEAR_TO 56}Bait\nRock{CLEAR_TO 56}Run");
const u8 gText_MoveInterfacePP[] = _("PP ");
const u8 gText_MoveInterfaceType[] = _("Type/");
const u8 gText_MoveInfoPowerAndAccuracy[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Power: {B_BUFF1}\nAcc.: {B_BUFF2}");
const u8 gText_MoveInfoPriorityAndCategory[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Prio.: {B_BUFF1}\nCat/{B_BUFF2}");
const u8 gText_MoveInfoTarget[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Target:\n{B_BUFF1}");
const u8 gText_MoveTargetSelected[] = _("Selected");
const u8 gText_MoveTargetDepends[] = _("Depends");
const u8 gText_MoveTargetUserOrSelected[] = _("User o Sel.");
const u8 gText_MoveTargetRandom[] = _("Random");
const u8 gText_MoveTargetOpponentsField[] = _("Foe's Side");
const u8 gText_MoveTargetBoth[] = _("Both Foes");
const u8 gText_MoveTargetUser[] = _("User");
const u8 gText_MoveTargetAlly[] = _("User's Ally");
const u8 gText_MoveTargetFoesAndAlly[] = _("Foes & Ally");
const u8 gText_MoveTargetUserOrAlly[] = _("User o Ally");
const u8 gText_MoveTargetAllBattlers[] = _("All Field");
const u8 gUnknown_83FE770[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}");
const u8 gMoveEffectiveness[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 9 14 8}");
const u8 gUnknown_83FE779[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}どの わざを\nわすれさせたい?");
const u8 gText_BattleYesNoChoice[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Yes\nNo");
const u8 gText_BattleSwitchWhich[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Switch\nwhich?");
const u8 gUnknown_83FE7B6[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}");
const u8 gUnknown_83FE7BF[] = _("{RIGHT_ARROW_2}");
const u8 gUnknown_83FE7C2[] = _("{PLUS}");
const u8 gUnknown_83FE7C5[] = _("-");

const u8 gUnknown_83FE7C7[] = _("{SIZE 0}Max{SIZE 2} HP");
const u8 gUnknown_83FE7D4[] = _("Attack ");
const u8 gUnknown_83FE7DC[] = _("Defense");
const u8 gUnknown_83FE7E4[] = _("Sp. Atk");
const u8 gUnknown_83FE7EC[] = _("Sp. Def");

const u8 *const gUnknown_83FE7F4[] = {
    gUnknown_83FE7C7,
    gUnknown_83FE7E4,
    gUnknown_83FE7D4,
    gUnknown_83FE7EC,
    gUnknown_83FE7DC,
    sText_Speed
};

const u8 gText_Sleep[] = _("sleep");
const u8 gText_Poison[] = _("poison");
const u8 gText_Burn[] = _("burn");
const u8 gText_Paralysis[] = _("paralysis");
const u8 gText_Ice[] = _("ice");
const u8 gText_Confusion[] = _("confusion");
const u8 gText_Love[] = _("love");
const u8 gUnknown_83FE870[] = _("ミツル");
const u8 gText_ClearTime[] = _("Clear Time"); // Unused
const u8 gUnknown_83FE9A9[] = _("1F");
const u8 gUnknown_83FE9AC[] = _("2F");
const u8 gUnknown_83FE9AF[] = _("3F");
const u8 gUnknown_83FE9B2[] = _("4F");
const u8 gUnknown_83FE9B5[] = _("5F");
const u8 gUnknown_83FE9B8[] = _("6F");
const u8 gUnknown_83FE9BB[] = _("7F");
const u8 gUnknown_83FE9BE[] = _("8F");








// Multistring chooser tables
const u16 gMissStringIds[] =
{
    [B_MSG_MISSED]      = STRINGID_ATTACKMISSED,
    [B_MSG_PROTECTED]   = STRINGID_DEFPROTECTEDITSELF,
    [B_MSG_AVOIDED_ATK] = STRINGID_PKMNAVOIDEDATTACK
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
	[B_MSG_SAFEGUARD_WORE_OFF]   = STRINGID_ATKTEAMSAFEGUARDEXPIRED
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
	[B_MSG_STRONG_WINDS_CONTINUES] = STRINGID_MYSTERIOUSAIRCONTINUES
};

const u16 gMoveWeatherChangeStringIds[] =
{
    [B_MSG_STARTED_RAIN]      = STRINGID_STARTEDTORAIN,
    [B_MSG_WEATHER_MOVE_FAIL] = STRINGID_BUTITFAILED,
    [B_MSG_SANDSTORM_UP]      = STRINGID_ASANDSTORMKICKEDUP,
    [B_MSG_SUN_TURN_HARSH]    = STRINGID_SUNLIGHTTURNEDHARSH,
    [B_MSG_STARTED_HAIL]      = STRINGID_STARTEDHAIL
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
	[B_MSG_MOVE_SEALED]             = STRINGID_PKMNCANTUSEMOVESEALED,
	[B_MSG_MOVE_LOCKED_CHOICE_ITEM] = STRINGID_LASTITEMALLOWSONLYCURRMOVE,
	[B_MSG_MOVE_NO_PP]              = STRINGID_NOPPLEFT
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
	[B_MSG_FIELD_EFFECTS_WORE_OFF] = STRINGID_BUFF1EFFECTSWOREOFF
};

// Strings table
const u8 *const gBattleStringsTable[] =
{
    [STRINGID_TRAINER1LOSETEXT - BATTLESTRINGS_ID_ADDER]              = sText_Trainer1LoseText,
	[STRINGID_PKMNGAINEDEXP - BATTLESTRINGS_ID_ADDER]                 = sText_PkmnGainedEXP,
    [STRINGID_PKMNGREWTOLV - BATTLESTRINGS_ID_ADDER]                  = sText_PkmnGrewToLv,
    [STRINGID_PKMNLEARNEDMOVE - BATTLESTRINGS_ID_ADDER]               = sText_PkmnLearnedMove,
    [STRINGID_TRYTOLEARNMOVE1 - BATTLESTRINGS_ID_ADDER]               = sText_TryToLearnMove1,
    [STRINGID_TRYTOLEARNMOVE2 - BATTLESTRINGS_ID_ADDER]               = sText_TryToLearnMove2,
    [STRINGID_TRYTOLEARNMOVE3 - BATTLESTRINGS_ID_ADDER]               = sText_TryToLearnMove3,
    [STRINGID_PKMNFORGOTMOVE - BATTLESTRINGS_ID_ADDER]                = sText_PkmnForgotMove,
    [STRINGID_STOPLEARNINGMOVE - BATTLESTRINGS_ID_ADDER]              = sText_StopLearningMove,
    [STRINGID_DIDNOTLEARNMOVE - BATTLESTRINGS_ID_ADDER]               = sText_DidNotLearnMove,
    [STRINGID_ATKLEARNEDBUFF1 - BATTLESTRINGS_ID_ADDER]               = sText_AtkLearnedBuff1,
    [STRINGID_ATTACKMISSED - BATTLESTRINGS_ID_ADDER]                  = sText_AttackMissed,
    [STRINGID_DEFPROTECTEDITSELF - BATTLESTRINGS_ID_ADDER]            = sText_DefProtectedItself,
    [STRINGID_ITDOESNTAFFECT - BATTLESTRINGS_ID_ADDER]                = sText_ItDoesntAffect,
    [STRINGID_ATKFAINTED - BATTLESTRINGS_ID_ADDER]                    = sText_AtkFainted,
    [STRINGID_DEFFAINTED - BATTLESTRINGS_ID_ADDER]                    = sText_DefFainted,
    [STRINGID_PLAYERGOTMONEY - BATTLESTRINGS_ID_ADDER]                = sText_PlayerGotMoney,
    [STRINGID_PLAYERWHITEOUT - BATTLESTRINGS_ID_ADDER]                = sText_PlayerWhiteout,
    [STRINGID_PLAYERPANICKEDANDLOST - BATTLESTRINGS_ID_ADDER]         = sText_PlayerPanicked,
    [STRINGID_PKMNPREVENTSESCAPEWITHABL - BATTLESTRINGS_ID_ADDER]     = sText_PkmnPreventsEscapeWithAbl,
    [STRINGID_HITXTIMES - BATTLESTRINGS_ID_ADDER]                     = sText_HitXTimes,
    [STRINGID_EFFFELLASLEEP - BATTLESTRINGS_ID_ADDER]                 = sText_EffFellAsleep,
    [STRINGID_DEFALREADYASLEEP - BATTLESTRINGS_ID_ADDER]              = sText_DefAlreadyAsleep,
    [STRINGID_DEFWASNTAFFECTED - BATTLESTRINGS_ID_ADDER]              = sText_DefWasntAffected,
    [STRINGID_EFFWASPOISONED - BATTLESTRINGS_ID_ADDER]                = sText_EffWasPoisoned,
    [STRINGID_ATKHURTBYPOISON - BATTLESTRINGS_ID_ADDER]               = sText_AtkHurtByPoison,
    [STRINGID_DEFALREADYPOISONED - BATTLESTRINGS_ID_ADDER]            = sText_DefAlreadyPoisoned,
    [STRINGID_EFFBADLYPOISONED - BATTLESTRINGS_ID_ADDER]              = sText_EffBadlyPoisoned,
    [STRINGID_DEFENERGYDRAINED - BATTLESTRINGS_ID_ADDER]              = sText_DefEnergyDrained,
    [STRINGID_EFFWASBURNED - BATTLESTRINGS_ID_ADDER]                  = sText_EffWasBurned,
    [STRINGID_ATKHURTBYBURN - BATTLESTRINGS_ID_ADDER]                 = sText_AtkHurtByBurn,
    [STRINGID_EFFWASFROZEN - BATTLESTRINGS_ID_ADDER]                  = sText_EffWasFrozen,
    [STRINGID_ATKISFROZEN - BATTLESTRINGS_ID_ADDER]                   = sText_AtkIsFrozen,
    [STRINGID_BUFF1WASDEFROSTED - BATTLESTRINGS_ID_ADDER]             = sText_Buff1WasDefrosted,
    [STRINGID_ATKDEFROSTEDBYCURRMOVE - BATTLESTRINGS_ID_ADDER]        = sText_AtkDefrostedByCurrMove,
    [STRINGID_EFFWASPARALYZED - BATTLESTRINGS_ID_ADDER]               = sText_EffWasParalyzed,
    [STRINGID_ATKISPARALYZED - BATTLESTRINGS_ID_ADDER]                = sText_AtkIsParalyzed,
    [STRINGID_DEFISALREADYPARALYZED - BATTLESTRINGS_ID_ADDER]         = sText_DefIsAlreadyParalyzed,
	[STRINGID_EFFABLPREVENTSBURNS - BATTLESTRINGS_ID_ADDER]           = sText_EffAblPreventsBurns,
	[STRINGID_EFFCUREDOFBUFF1 - BATTLESTRINGS_ID_ADDER]               = sText_EffCuredOfBuff1,
	[STRINGID_EFFWOKEUP - BATTLESTRINGS_ID_ADDER]                     = sText_EffWokeUp,
    [STRINGID_ATKISCONFUSED - BATTLESTRINGS_ID_ADDER]                 = sText_AtkIsConfused,
    [STRINGID_ATKSNAPPEDOUTOFCONFUSION - BATTLESTRINGS_ID_ADDER]      = sText_AtkSnappedOutOfConfusion,
    [STRINGID_EFFBECAMECONFUSED - BATTLESTRINGS_ID_ADDER]             = sText_EffBecameConfused,
    [STRINGID_DEFALREADYCONFUSED - BATTLESTRINGS_ID_ADDER]            = sText_DefAlreadyConfused,
    [STRINGID_DEFFELLINLOVE - BATTLESTRINGS_ID_ADDER]                 = sText_DefFellInLove,
    [STRINGID_ATKISINLOVEWITHPKMN - BATTLESTRINGS_ID_ADDER]           = sText_AtkIsInLoveWithPkmn,
    [STRINGID_ATKIMMOBILIZEDBYLOVE - BATTLESTRINGS_ID_ADDER]          = sText_AtkImmobilizedByLove,
    [STRINGID_PKMNTYPECHANGEDTOBUFF1 - BATTLESTRINGS_ID_ADDER]        = sText_PkmnTypeChangedToBuff1,
    [STRINGID_ATKFLINCHED - BATTLESTRINGS_ID_ADDER]                   = sText_AtkFlinched,
    [STRINGID_DEFREGAINEDHEALTH - BATTLESTRINGS_ID_ADDER]             = sText_DefRegainedHealth,
    [STRINGID_DEFHPFULL - BATTLESTRINGS_ID_ADDER]                     = sText_DefHPFull,
    [STRINGID_ATKTEAMCLOAKEDINMYSTICALVEIL - BATTLESTRINGS_ID_ADDER]  = sText_AtkTeamCloakedInMysticalVeil,
	[STRINGID_BUFF1RAISEDATKTEAMBUFF2 - BATTLESTRINGS_ID_ADDER]       = sText_Buff1RaisedAtkTeamBuff2,
    [STRINGID_DEFTEAMSAFEGUARDPROTECTED - BATTLESTRINGS_ID_ADDER]     = sText_DefTeamSafeguardProtected,
    [STRINGID_ATKTEAMSAFEGUARDEXPIRED - BATTLESTRINGS_ID_ADDER]       = sText_AtkTeamSafeguardExpired,
	[STRINGID_ATKTEAMBUFF1WOREOFF - BATTLESTRINGS_ID_ADDER]           = sText_AtkTeamBuff1WoreOff,
    [STRINGID_DEFWENTTOSLEEP - BATTLESTRINGS_ID_ADDER]                = sText_DefWentToSleep,
    [STRINGID_DEFSLEPTHEALTHY - BATTLESTRINGS_ID_ADDER]               = sText_DefSleptHealthy,
	[STRINGID_BUTITFAILED - BATTLESTRINGS_ID_ADDER]                   = sText_ButItFailed,
    [STRINGID_ATKWHIPPEDWHIRLWIND - BATTLESTRINGS_ID_ADDER]           = sText_AtkWhippedWhirlwind,
    [STRINGID_ATKBECAMECLOAKEDINSUNLIGHT - BATTLESTRINGS_ID_ADDER]    = sText_AtkBecameCloakedInSunlight,
	[STRINGID_ATKBURROWEDUNDERTHEGROUND - BATTLESTRINGS_ID_ADDER]     = sText_AtkBurrowedUnderTheGround,
    [STRINGID_ATKTUCKEDINHEAD - BATTLESTRINGS_ID_ADDER]               = sText_AtkTuckedInItsHead,
	[STRINGID_ATKSPRANGUP - BATTLESTRINGS_ID_ADDER]                   = sText_AtkSprangUp,
	[STRINGID_ATKHIDUNDERWATER - BATTLESTRINGS_ID_ADDER]              = sText_AtkHidUnderwater,
	[STRINGID_ATKFLEWHIGH - BATTLESTRINGS_ID_ADDER]                   = sText_AtkFlewHigh,
    [STRINGID_ATKABSORBEDLIGHT - BATTLESTRINGS_ID_ADDER]              = sText_AtkAbsorbedLight,
    [STRINGID_EFFSQUEEZEDBYPKMN - BATTLESTRINGS_ID_ADDER]             = sText_EffSqueezedByPkmn,
	[STRINGID_EFFWRAPPEDBYPKMN - BATTLESTRINGS_ID_ADDER]              = sText_EffWrappedByPkmn,
    [STRINGID_EFFTRAPPEDINVORTEX - BATTLESTRINGS_ID_ADDER]            = sText_EffTrappedInVortex,
    [STRINGID_PKMNCLAMPEDEFF - BATTLESTRINGS_ID_ADDER]                = sText_PkmnClampedEff,
	[STRINGID_EFFTRAPPEDBYCURRMOVE - BATTLESTRINGS_ID_ADDER]          = sText_EffTrappedByCurrMove,
    [STRINGID_ATKHURTBYBUFF1 - BATTLESTRINGS_ID_ADDER]                = sText_AtkHurtByBuff1,
    [STRINGID_ATKFREEDFROMBUFF1 - BATTLESTRINGS_ID_ADDER]             = sText_AtkFreedFromBuff1,
    [STRINGID_ATKKEPTANDCRASHED - BATTLESTRINGS_ID_ADDER]             = sText_AtkKeptAndCrashed,
    [STRINGID_ATKTEAMSHROUDEDINMIST - BATTLESTRINGS_ID_ADDER]         = sText_AtkTeamShroudedInMist,
    [STRINGID_ATKGETTINGPUMPED - BATTLESTRINGS_ID_ADDER]              = sText_AtkGetPumped,
    [STRINGID_ATKHITWITHRECOIL - BATTLESTRINGS_ID_ADDER]              = sText_AtkHitWithRecoil,
    [STRINGID_ATKBUFFETEDBYSANDSTORM - BATTLESTRINGS_ID_ADDER]        = sText_AtkBuffetedBySandstorm,
    [STRINGID_ATKPELTEDBYHAIL - BATTLESTRINGS_ID_ADDER]               = sText_AtkPeltedByHail,
    [STRINGID_DEFWASSEEDED - BATTLESTRINGS_ID_ADDER]                  = sText_DefWasSeeded,
    [STRINGID_DEFEVADEDATTACK - BATTLESTRINGS_ID_ADDER]               = sText_DefEvadedAttack,
    [STRINGID_DEFSAPPEDBYLEECHSEED - BATTLESTRINGS_ID_ADDER]          = sText_DefSappedByLeechSeed,
    [STRINGID_ATKFASTASLEEP - BATTLESTRINGS_ID_ADDER]                 = sText_AtkFastAsleep,
    [STRINGID_ATKWOKEUP - BATTLESTRINGS_ID_ADDER]                     = sText_AtkWokeUp,
	[STRINGID_ATKWOKEUPINUPROAR - BATTLESTRINGS_ID_ADDER]             = sText_AtkWokeUpInUproar,
	[STRINGID_DEFENDUREDHIT - BATTLESTRINGS_ID_ADDER]                 = sText_DefEnduredHit,
	[STRINGID_ONEHITKO - BATTLESTRINGS_ID_ADDER]                      = sText_OneHitKO,
	[STRINGID_ATKSTORINGENERGY - BATTLESTRINGS_ID_ADDER]              = sText_AtkStoringEnergy,
    [STRINGID_ATKUNLEASHEDENERGY - BATTLESTRINGS_ID_ADDER]            = sText_AtkUnleashedEnergy,
	[STRINGID_FIZZLESBUFF1TYPEATTACK - BATTLESTRINGS_ID_ADDER]        = sText_FizzlesBuff1TypeAttack,
	[STRINGID_EVAPORATEBUFF1TYPEATTACK - BATTLESTRINGS_ID_ADDER]      = sText_EvaporateBuff1TypeAttack,
    [STRINGID_ATKFATIGUECONFUSION - BATTLESTRINGS_ID_ADDER]           = sText_AtkFatigueConfusion,
    [STRINGID_PLAYERPICKEDUPMONEY - BATTLESTRINGS_ID_ADDER]           = sText_PlayerPickedUpMoney,
    [STRINGID_DEFUNAFFECTED - BATTLESTRINGS_ID_ADDER]                 = sText_DefUnaffected,
    [STRINGID_ATKTRANSFORMEDINTOBUFF1 - BATTLESTRINGS_ID_ADDER]       = sText_AtkTransformedIntoBuff1,
    [STRINGID_ATKMADESUBSTITUTE - BATTLESTRINGS_ID_ADDER]             = sText_AtkMadeSubstitute,
    [STRINGID_ATKHASSUBSTITUTE - BATTLESTRINGS_ID_ADDER]              = sText_AtkHasSubstitute,
	[STRINGID_TOOWEAKFORSUBSTITUTE - BATTLESTRINGS_ID_ADDER]          = sText_TooWeakForSubstitute,
    [STRINGID_SUBSTITUTEDAMAGED - BATTLESTRINGS_ID_ADDER]             = sText_SubstituteTookDamageForDef,
    [STRINGID_DEFSUBSTITUTEFADED - BATTLESTRINGS_ID_ADDER]            = sText_DefSubstituteFaded,
    [STRINGID_ATKMUSTRECHARGE - BATTLESTRINGS_ID_ADDER]               = sText_AtkMustRecharge,
	[STRINGID_ATKTOOKAIMATDEF - BATTLESTRINGS_ID_ADDER]               = sText_AtkTookAimAtDef,
	[STRINGID_DEFBUFF1WASDISABLED - BATTLESTRINGS_ID_ADDER]           = sText_DefBuff1WasDisabled,
    [STRINGID_ATKCURRMOVEISDISABLED - BATTLESTRINGS_ID_ADDER]         = sText_AtkCurrMoveIsDisabled,
    [STRINGID_ATKDISABLEDNOMORE - BATTLESTRINGS_ID_ADDER]             = sText_AtkDisabledNoMore,
    [STRINGID_DEFRECEIVEDENCORE - BATTLESTRINGS_ID_ADDER]             = sText_DefReceivedEncore,
    [STRINGID_ATKENCOREENDED - BATTLESTRINGS_ID_ADDER]                = sText_AtkEncoreEnded,
    [STRINGID_ATKTRYINGTOTAKEFOEDOWN - BATTLESTRINGS_ID_ADDER]        = sText_AtkTryingToTakeFoeDown,
    [STRINGID_DEFTOOKATTACKERWITHIT - BATTLESTRINGS_ID_ADDER]         = sText_DefTookAttackerDownWithIt,
    [STRINGID_REDUCEDPPOFDEFBUFF1BYBUFF2 - BATTLESTRINGS_ID_ADDER]    = sText_ReducedPPOfDefBuff1ByBuff2,
    [STRINGID_ATKSTOLEDEFLASTITEM - BATTLESTRINGS_ID_ADDER]           = sText_AtkStoleDefLastItem,
    [STRINGID_DEFCANNOLONGERESCAPE - BATTLESTRINGS_ID_ADDER]          = sText_DefCanNoLongerEscape,
    [STRINGID_DEFBEGANHAVINGNIGHTMARE - BATTLESTRINGS_ID_ADDER]       = sText_DefBeganHavingNightmare,
    [STRINGID_ATKLOCKEDINNIGHTMARE - BATTLESTRINGS_ID_ADDER]          = sText_AtkLockedInNightmare,
    [STRINGID_SPIKESSCATTERED - BATTLESTRINGS_ID_ADDER]               = sText_SpikesScattered,
    [STRINGID_DEFWASIDENTIFIED - BATTLESTRINGS_ID_ADDER]              = sText_DefWasIdentified,
    [STRINGID_ATKPERISHCOUNTFELLTOBUFF1 - BATTLESTRINGS_ID_ADDER]     = sText_AtkPerishCountFellToBuff1,
    [STRINGID_ATKBRACEDITSELF - BATTLESTRINGS_ID_ADDER]               = sText_AtkBracedItself,
    [STRINGID_MAGNITUDEBUFF1 - BATTLESTRINGS_ID_ADDER]                = sText_MagnitudeBuff1,
	[STRINGID_ATKCOPIEDDEFSTATCHANGES - BATTLESTRINGS_ID_ADDER]       = sText_AtkCopiedDefStatChanges,
    [STRINGID_ATKFLEDFROMBATTLE - BATTLESTRINGS_ID_ADDER]             = sText_AtkFledFromBattle,
    [STRINGID_ATKFORESAWATTACK - BATTLESTRINGS_ID_ADDER]              = sText_AtkForesawAttack,
	[STRINGID_ATKCHOSECURRMOVEASDESTINY - BATTLESTRINGS_ID_ADDER]     = sText_AtkChoseCurrMoveAsDestiny,
    [STRINGID_DEFTOOKTHECURRMOVEATTACK - BATTLESTRINGS_ID_ADDER]      = sText_DefTookTheCurrMoveAttack,
    [STRINGID_ATKBECAMETHECENTERATTENTION - BATTLESTRINGS_ID_ADDER]   = sText_AtkBecameCenterOfAttention,
    [STRINGID_BUFF1TURNEDINTOCURRMOVE - BATTLESTRINGS_ID_ADDER]       = sText_Buff1TurnedIntoCurrMove,
    [STRINGID_ATKHASNOMOVESLEFT - BATTLESTRINGS_ID_ADDER]             = sText_AtkHasNoMovesLeft,
    [STRINGID_DEFSUBJECTEDTOTORMENT - BATTLESTRINGS_ID_ADDER]         = sText_DefSubjectedToTorment,
    [STRINGID_ATKCANTUSEMOVETORMENT - BATTLESTRINGS_ID_ADDER]         = sText_AtkCantUseMoveTorment,
    [STRINGID_ATKTIGHTENINGFOCUS - BATTLESTRINGS_ID_ADDER]            = sText_AtkTighteningFocus,
    [STRINGID_DEFFELLFORTAUNT - BATTLESTRINGS_ID_ADDER]               = sText_DefFellForTheTaunt,
    [STRINGID_ATKCANTUSECURRMOVETAUNT - BATTLESTRINGS_ID_ADDER]       = sText_AtkCantUseCurrMoveAfterTaunt,
    [STRINGID_ATKREADYTOHELPDEF - BATTLESTRINGS_ID_ADDER]             = sText_AtkReadyToHelpDef,
    [STRINGID_ATKCOPIEDDEFABL - BATTLESTRINGS_ID_ADDER]               = sText_AtkCopiedDefAbl,
    [STRINGID_BUFF1WISHCAMETRUE - BATTLESTRINGS_ID_ADDER]             = sText_Buff1WishCameTrue,
    [STRINGID_ATKPLANTEDROOTS - BATTLESTRINGS_ID_ADDER]               = sText_AtkPlantedRoots,
    [STRINGID_ATKABSORBEDNUTRIENTS - BATTLESTRINGS_ID_ADDER]          = sText_AtkAbsorbedNutrients,
    [STRINGID_DEFANCHOREDITSELF - BATTLESTRINGS_ID_ADDER]             = sText_DefAnchoredItself,
    [STRINGID_DEFGREWDROWSY - BATTLESTRINGS_ID_ADDER]                 = sText_DefGrewDrowsy,
    [STRINGID_ATKSWAPPEDABILITIESWITHTARGET - BATTLESTRINGS_ID_ADDER] = sText_AtkSwappedAbilitiesWithTarget,
	[STRINGID_DEFBOUNCEDCURRMOVEBACK - BATTLESTRINGS_ID_ADDER]        = sText_DefBouncedCurrMoveBack,
	[STRINGID_ATKSHROUDEDWITHCURRMOVE - BATTLESTRINGS_ID_ADDER]       = sText_AtkShroudedItselfWithCurrMove,
	[STRINGID_CRITICALHIT - BATTLESTRINGS_ID_ADDER]                   = sText_CriticalHit,
	[STRINGID_CRITICALHITONDEF - BATTLESTRINGS_ID_ADDER]              = sText_CriticalHitOnDef,
    [STRINGID_123POOF - BATTLESTRINGS_ID_ADDER]                       = sText_123Poof,
    [STRINGID_ANDELLIPSIS - BATTLESTRINGS_ID_ADDER]                   = sText_AndEllipsis,
    [STRINGID_NOTVERYEFFECTIVE - BATTLESTRINGS_ID_ADDER]              = sText_NotVeryEffective,
	[STRINGID_NOTVERYEFFECTIVEONDEF - BATTLESTRINGS_ID_ADDER]         = sText_NotVeryEffectiveOnDef,
    [STRINGID_SUPEREFFECTIVE - BATTLESTRINGS_ID_ADDER]                = sText_SuperEffective,
	[STRINGID_SUPEREFFECTIVEONDEF - BATTLESTRINGS_ID_ADDER]           = sText_SuperEffectiveOnDef,
	[STRINGID_ATKISLOAFINGAROUND - BATTLESTRINGS_ID_ADDER]            = sText_AtkIsLoafingAround,
	[STRINGID_ATKWONTOBEY - BATTLESTRINGS_ID_ADDER]                   = sText_AtkWontObey,
	[STRINGID_ATKTURNEDAWAY - BATTLESTRINGS_ID_ADDER]                 = sText_AtkTurnedAway,
	[STRINGID_ATKPRETENDNOTNOTICE - BATTLESTRINGS_ID_ADDER]           = sText_AtkPretendNotNotice,
	[STRINGID_BUFF1ISNERVOUSTOEATBERRIES - BATTLESTRINGS_ID_ADDER]    = sText_Buff1IsTooNervousToEatBerries,
	[STRINGID_EFFPREVENTSPOISONINGWITH - BATTLESTRINGS_ID_ADDER]      = sText_EffPreventsPoisoningWith,
	[STRINGID_EMPTYSTRING - BATTLESTRINGS_ID_ADDER]                   = gText_EmptyString,
	[STRINGID_ABOOSTED - BATTLESTRINGS_ID_ADDER]                      = sText_ABoosted,
	[STRINGID_STARTEDTORAIN - BATTLESTRINGS_ID_ADDER]                 = sText_StartedToRain,
	[STRINGID_GOTAWAYSAFELY - BATTLESTRINGS_ID_ADDER]                 = sText_GotAwaySafely,
    [STRINGID_WILDBUFF1FLED - BATTLESTRINGS_ID_ADDER]                 = sText_WildBuff1Fled,
    [STRINGID_DEFPROTECTEDBYDEFABL - BATTLESTRINGS_ID_ADDER]          = sText_DefProtectedByDefAbl,
    [STRINGID_PKMNABLPREVENTSATKUSECURRMOVE - BATTLESTRINGS_ID_ADDER] = sText_PkmnAblPreventsAtkFromUseCurrMove,
	[STRINGID_BUTNOTHINGHAPPENED - BATTLESTRINGS_ID_ADDER]            = sText_ButNothingHappened,
    [STRINGID_ITHURTITSELFINCONFUSION - BATTLESTRINGS_ID_ADDER]       = sText_ItHurtItselfInConfusion,
    [STRINGID_THECURRMOVEFAILED - BATTLESTRINGS_ID_ADDER]             = sText_TheCurrMoveFailed,
    [STRINGID_DEFHADHPRESTORED - BATTLESTRINGS_ID_ADDER]              = sText_DefHadHPRestored,
	[STRINGID_POWEROFDEFBUFF1TYPEMOVESROSE - BATTLESTRINGS_ID_ADDER]  = sText_PowerOfDefBuff1TypeMovesRose,
	[STRINGID_ITTRACEDDEFABL - BATTLESTRINGS_ID_ADDER]                = sText_ItTracedDefAbl,
	[STRINGID_ATKWASHURT - BATTLESTRINGS_ID_ADDER]                    = sText_AtkWasHurt,
	[STRINGID_MONHEARINGFLUTEAWOKE - BATTLESTRINGS_ID_ADDER]          = sText_PokemonHearingFluteAwoke,
	[STRINGID_GOTCHADEFCAUGHT - BATTLESTRINGS_ID_ADDER]               = sText_GotchaDefCaught,
	[STRINGID_DEFDATAADDEDTODEX - BATTLESTRINGS_ID_ADDER]             = sText_DefDataAddedToDex,
	[STRINGID_PKMNTRANSFERREDSOMEONESPC - BATTLESTRINGS_ID_ADDER]     = Text_MonSentToBoxInSomeonesPC,
    [STRINGID_PKMNTRANSFERREDBILLSPC - BATTLESTRINGS_ID_ADDER]        = Text_MonSentToBoxInBillsPC,
    [STRINGID_PKMNBOXSOMEONESPCFULL - BATTLESTRINGS_ID_ADDER]         = Text_MonSentToBoxSomeonesBoxFull,
    [STRINGID_PKMNBOXBILLSPCFULL - BATTLESTRINGS_ID_ADDER]            = Text_MonSentToBoxBillsBoxFull,
	[STRINGID_OUTOFSAFARIBALLS - BATTLESTRINGS_ID_ADDER]              = sText_OutOfSafariBalls,
	[STRINGID_POKEMONBROKEFREE - BATTLESTRINGS_ID_ADDER]              = sText_OhNoPokemonBrokeFree,
	[STRINGID_ITAPPEAREDCAUGHT - BATTLESTRINGS_ID_ADDER]              = sText_ItAppearedToBeCaught,
    [STRINGID_AARGHALMOSTHADIT - BATTLESTRINGS_ID_ADDER]              = sText_AarghAlmostHadIt,
    [STRINGID_SHOOTSOCLOSE - BATTLESTRINGS_ID_ADDER]                  = sText_ShootSoClose,
	[STRINGID_TRAINERBLOCKEDBALL - BATTLESTRINGS_ID_ADDER]            = sText_TrainerBlockedBall,
    [STRINGID_DONTBEATHIEF - BATTLESTRINGS_ID_ADDER]                  = sText_DontBeAThief,
	[STRINGID_ITDODGEDBALL - BATTLESTRINGS_ID_ADDER]                  = sText_ItDodgedBall,
	[STRINGID_PKMNWATCHINGCAREFULLY - BATTLESTRINGS_ID_ADDER]         = sText_PkmnWatchingCarefully,
    [STRINGID_PKMNANGRY - BATTLESTRINGS_ID_ADDER]                     = sText_PkmnIsAngry,
    [STRINGID_PKMNEATING - BATTLESTRINGS_ID_ADDER]                    = sText_PkmnIsEating,
	[STRINGID_THREWROCK - BATTLESTRINGS_ID_ADDER]                     = sText_ThrewARock,
    [STRINGID_THREWBAIT - BATTLESTRINGS_ID_ADDER]                     = sText_ThrewSomeBait,
	[STRINGID_YOUTHROWABALLNOWRIGHT - BATTLESTRINGS_ID_ADDER]         = sText_YouThrowABallNowRight,
    [STRINGID_NORUNNINGFROMTRAINERS - BATTLESTRINGS_ID_ADDER]         = sText_NoRunningFromTrainers,
    [STRINGID_CANTESCAPE - BATTLESTRINGS_ID_ADDER]                    = sText_CantEscape,
    [STRINGID_DONTLEAVEBIRCH - BATTLESTRINGS_ID_ADDER]                = gText_EmptyString,
	[STRINGID_ATKCANTESCAPE - BATTLESTRINGS_ID_ADDER]                 = sText_AtkCantEscape,
	[STRINGID_RAINCONTINUES - BATTLESTRINGS_ID_ADDER]                 = sText_RainContinues,
	[STRINGID_RAINSTOPPED - BATTLESTRINGS_ID_ADDER]                   = sText_RainStopped,
	[STRINGID_SUNLIGHTSTRONG - BATTLESTRINGS_ID_ADDER]                = sText_SunlightStrong,
    [STRINGID_SUNLIGHTFADED - BATTLESTRINGS_ID_ADDER]                 = sText_SunlightFaded,
	[STRINGID_SANDSTORMRAGES - BATTLESTRINGS_ID_ADDER]                = sText_SandstormRages,
    [STRINGID_SANDSTORMSUBSIDED - BATTLESTRINGS_ID_ADDER]             = sText_SandstormSubsided,
	[STRINGID_HAILCONTINUES - BATTLESTRINGS_ID_ADDER]                 = sText_HailContinues,
    [STRINGID_HAILSTOPPED - BATTLESTRINGS_ID_ADDER]                   = sText_HailStopped,
	[STRINGID_DEEPFOG - BATTLESTRINGS_ID_ADDER]                       = sText_FogIsDeep,
	[STRINGID_FOGENDED - BATTLESTRINGS_ID_ADDER]                      = sText_FogEnded,
	[STRINGID_MYSTERIOUSAIRCONTINUES - BATTLESTRINGS_ID_ADDER]        = sText_MysteriousAirContinues,
	[STRINGID_ASANDSTORMKICKEDUP - BATTLESTRINGS_ID_ADDER]            = sText_SandstormKickedUp,
	[STRINGID_SUNLIGHTTURNEDHARSH - BATTLESTRINGS_ID_ADDER]           = sText_SunlightTurnedHarsh,
	[STRINGID_STARTEDHAIL - BATTLESTRINGS_ID_ADDER]                   = sText_StartedHail,
	[STRINGID_COINSSCATTERED - BATTLESTRINGS_ID_ADDER]                = sText_CoinsScattered,
	[STRINGID_GIVENICKNAMECAPTURED - BATTLESTRINGS_ID_ADDER]          = sText_GiveNicknameCaptured,
	[STRINGID_BATTLERSSHAREDTHEIRPAIN - BATTLESTRINGS_ID_ADDER]       = sText_TheBattlersSharedTheirPain,
	[STRINGID_BELLCHIMED - BATTLESTRINGS_ID_ADDER]                    = sText_ABellChimed,
	[STRINGID_SOOTHINGAROMA - BATTLESTRINGS_ID_ADDER]                 = sText_SoothingAroma,
	[STRINGID_STATCHANGESELIMINATED - BATTLESTRINGS_ID_ADDER]         = sText_AllStatChangesEliminated,
    [STRINGID_ALLPOKEMONFAINTINTHREETURNS - BATTLESTRINGS_ID_ADDER]   = sText_AllPokemonFaintInThreeTurns,
    [STRINGID_NOPPLEFT - BATTLESTRINGS_ID_ADDER]                      = sText_NoPPLeft,
    [STRINGID_BUTNOPPLEFT - BATTLESTRINGS_ID_ADDER]                   = sText_ButNoPPLeft,
    [STRINGID_ATKIGNOREDORDERSASLEEP - BATTLESTRINGS_ID_ADDER]        = sText_AtkIgnoredOrdersAsleep,
    [STRINGID_ATKIGNOREDORDERS - BATTLESTRINGS_ID_ADDER]              = sText_AtkIgnoredOrders,
    [STRINGID_ATKBEGANTONAP - BATTLESTRINGS_ID_ADDER]                 = sText_AtkBeganToNap,
    [STRINGID_ENEMYABOUTTOSWITCHPKMN - BATTLESTRINGS_ID_ADDER]        = sText_EnemyAboutToSwitchPkmn,
    [STRINGID_LASTITEMALLOWSONLYCURRMOVE - BATTLESTRINGS_ID_ADDER]    = sText_LastItemAllowsOnlyCurrMove,
	[STRINGID_ITEMSCANTBEUSEDNOW - BATTLESTRINGS_ID_ADDER]            = sText_ItemsCantBeUsedNow,
	[STRINGID_BOXISFULL - BATTLESTRINGS_ID_ADDER]                     = sText_BoxIsFull,
	[STRINGID_EFFSTATCHANGED - BATTLESTRINGS_ID_ADDER]                = sText_EffStatChanged,
	[STRINGID_EFFSTATCANTCHANGE - BATTLESTRINGS_ID_ADDER]             = sText_EffStatCantChange,
	[STRINGID_DEFSTATSWONTCHANGE - BATTLESTRINGS_ID_ADDER]            = sText_DefStatsWontChangeAnymore,
	[STRINGID_EFFCUTHPMAXEDBUFF1 - BATTLESTRINGS_ID_ADDER]            = sText_EffCutHPMaxedBuff1,
	[STRINGID_ATKGOTFREEOFPKMNBUFF1 - BATTLESTRINGS_ID_ADDER]         = sText_AtkGotFreeOfPkmnBuff1,
	[STRINGID_ATKSHEDLEECHSEED - BATTLESTRINGS_ID_ADDER]              = sText_AtkShedLeechSeed,
	[STRINGID_ATKBLEWAWAYHAZARDS - BATTLESTRINGS_ID_ADDER]            = sText_AtkBlewAwayHazards,
	[STRINGID_ATKBEGANCHARGINGPOWER - BATTLESTRINGS_ID_ADDER]         = sText_AtkBeganChargingPower,
	[STRINGID_EFFPROTECTEDBYMIST - BATTLESTRINGS_ID_ADDER]            = sText_EffProtectedByMist,
	[STRINGID_EFFPREVENTSSTATLOSS - BATTLESTRINGS_ID_ADDER]           = sText_EffPreventsStatLoss,
	[STRINGID_EFFPREVENTSBUFF1LOSS - BATTLESTRINGS_ID_ADDER]          = sText_EffPreventsBuff1Loss,
	[STRINGID_EFFRAGEBUILDING - BATTLESTRINGS_ID_ADDER]               = sText_EffRageIsBuilding,
	[STRINGID_DEFPREVENTEDFROMWORKING - BATTLESTRINGS_ID_ADDER]       = sText_EffPreventedFromWorking,
	[STRINGID_EFFMAXEDSTAT - BATTLESTRINGS_ID_ADDER]                  = sText_EffMaxedItsBuff1,
	[STRINGID_DEFALREADYHASBURN - BATTLESTRINGS_ID_ADDER]             = sText_DefAlreadyHasBurn,
	[STRINGID_PKMNABLMADEITINEFFECTIVE - BATTLESTRINGS_ID_ADDER]      = sText_PkmnAblMadeItIneffective,
	[STRINGID_ATKSTOCKPILED - BATTLESTRINGS_ID_ADDER]                 = sText_AtkStockpiled,
    [STRINGID_ATKCANTSTOCKPILE - BATTLESTRINGS_ID_ADDER]              = sText_AtkCantStockpile,
	[STRINGID_BUTFAILEDTOCURRMOVEATHING - BATTLESTRINGS_ID_ADDER]     = sText_ButFailedToCurrMoveAThing,
	[STRINGID_ELECTRICITYPOWERWEAKENED - BATTLESTRINGS_ID_ADDER]      = sText_ElectricityPowerWeakened,
	[STRINGID_FIREPOWERWEAKENED - BATTLESTRINGS_ID_ADDER]             = sText_FirePowerWeakened,
	[STRINGID_BUFF1EFFECTSWOREOFF - BATTLESTRINGS_ID_ADDER]           = sText_Buff1EffectsWoreOff,
	[STRINGID_DEFTOOKTHEATTACK - BATTLESTRINGS_ID_ADDER]              = sText_DefTookTheAttack,
	[STRINGID_USENEXTPKMN - BATTLESTRINGS_ID_ADDER]                   = sText_UseNextPkmn,
	[STRINGID_PLAYERDEFEATEDTRAINER1 - BATTLESTRINGS_ID_ADDER]        = sText_PlayerDefeatedLinkTrainerTrainer1,
	[STRINGID_TRAINER1MON1COMEBACK - BATTLESTRINGS_ID_ADDER]          = sText_Trainer1Pkmn1ComeBack,
	[STRINGID_TRAINER1WINTEXT - BATTLESTRINGS_ID_ADDER]               = sText_Trainer1WinText,
	[STRINGID_PLAYERLOSTAGAINSTENEMYTRAINER - BATTLESTRINGS_ID_ADDER] = sText_PlayerWhiteoutAgainstTrainer,
	[STRINGID_PLAYERPAIDPRIZEMONEY - BATTLESTRINGS_ID_ADDER]          = sText_PlayerPaidAsPrizeMoney,
	[STRINGID_PLAYERWHITEDOUT - BATTLESTRINGS_ID_ADDER]               = sText_PlayerWhitedOut,
	
	
    [STRINGID_PKMNHUNGONWITHX - BATTLESTRINGS_ID_ADDER]               = sText_PkmnHungOnWithX,
    [STRINGID_PKMNSXBLOCKSY - BATTLESTRINGS_ID_ADDER]                 = sText_PkmnsXBlocksY,
    [STRINGID_PKMNSXRESTOREDHPALITTLE2 - BATTLESTRINGS_ID_ADDER]      = sText_PkmnsXRestoredHPALittle2,
    [STRINGID_PKMNSXWHIPPEDUPSANDSTORM - BATTLESTRINGS_ID_ADDER]      = sText_PkmnsXWhippedUpSandstorm,
    [STRINGID_PKMNSXINFATUATEDY - BATTLESTRINGS_ID_ADDER]             = sText_PkmnsXInfatuatedY,
    [STRINGID_PKMNSXMADEYINEFFECTIVE - BATTLESTRINGS_ID_ADDER]        = sText_PkmnsXMadeYIneffective,
    [STRINGID_PKMNSXCUREDYPROBLEM - BATTLESTRINGS_ID_ADDER]           = sText_PkmnsXCuredYProblem,
    [STRINGID_ITSUCKEDLIQUIDOOZE - BATTLESTRINGS_ID_ADDER]            = sText_ItSuckedLiquidOoze,
    [STRINGID_PKMNTRANSFORMED - BATTLESTRINGS_ID_ADDER]               = sText_PkmnTransformed,
    [STRINGID_HMMOVESCANTBEFORGOTTEN - BATTLESTRINGS_ID_ADDER]        = sText_HMMovesCantBeForgotten,
    [STRINGID_XFOUNDONEY - BATTLESTRINGS_ID_ADDER]                    = sText_XFoundOneY,
    [STRINGID_FORXCOMMAYZ - BATTLESTRINGS_ID_ADDER]                   = sText_ForXCommaYZ,
    [STRINGID_USINGXTHEYOFZN - BATTLESTRINGS_ID_ADDER]                = sText_UsingXTheYOfZN,
    [STRINGID_PKMNUSEDXTOGETPUMPED - BATTLESTRINGS_ID_ADDER]          = sText_PkmnUsedXToGetPumped,
    [STRINGID_PKMNSXMADEYUSELESS - BATTLESTRINGS_ID_ADDER]            = sText_PkmnsXMadeYUseless,
    [STRINGID_PKMNSXINTENSIFIEDSUN - BATTLESTRINGS_ID_ADDER]          = sText_PkmnsXIntensifiedSun,
    [STRINGID_PKMNMAKESGROUNDMISS - BATTLESTRINGS_ID_ADDER]           = sText_PkmnMakesGroundMiss,
    [STRINGID_PKMNLOSTFOCUS - BATTLESTRINGS_ID_ADDER]                 = sText_PkmnLostFocus,
    [STRINGID_PKMNFLEDUSINGITS - BATTLESTRINGS_ID_ADDER]              = sText_PkmnFledUsingIts,
    [STRINGID_PKMNFLEDUSING - BATTLESTRINGS_ID_ADDER]                 = sText_PkmnFledUsing,
    [STRINGID_PKMNWASDRAGGEDOUT - BATTLESTRINGS_ID_ADDER]             = sText_PkmnWasDraggedOut,
    [STRINGID_PKMNSITEMNORMALIZEDSTATUS - BATTLESTRINGS_ID_ADDER]     = sText_PkmnsItemNormalizedStatus,
    [STRINGID_PKMNAVOIDEDATTACK - BATTLESTRINGS_ID_ADDER]             = sText_PkmnAvoidedAttack,
    [STRINGID_PKMNSXPREVENTSFLINCHING - BATTLESTRINGS_ID_ADDER]       = sText_PkmnsXPreventsFlinching,
    [STRINGID_STATSWONTDECREASE2 - BATTLESTRINGS_ID_ADDER]            = sText_StatsWontDecrease2,
    [STRINGID_PKMNSXBLOCKSY2 - BATTLESTRINGS_ID_ADDER]                = sText_PkmnsXBlocksY2,
    [STRINGID_THEWALLSHATTERED - BATTLESTRINGS_ID_ADDER]              = sText_TheWallShattered,
    [STRINGID_PKMNSXPREVENTSYSZ - BATTLESTRINGS_ID_ADDER]             = sText_PkmnsXPreventsYsZ,
    [STRINGID_PKMNSXCUREDITSYPROBLEM - BATTLESTRINGS_ID_ADDER]        = sText_PkmnsXCuredItsYProblem,
    [STRINGID_PKMNOBTAINEDX - BATTLESTRINGS_ID_ADDER]                 = sText_PkmnObtainedX,
    [STRINGID_PKMNOBTAINEDX2 - BATTLESTRINGS_ID_ADDER]                = sText_PkmnObtainedX2,
    [STRINGID_PKMNOBTAINEDXYOBTAINEDZ - BATTLESTRINGS_ID_ADDER]       = sText_PkmnObtainedXYObtainedZ,
    [STRINGID_BUTNOEFFECT - BATTLESTRINGS_ID_ADDER]                   = sText_ButNoEffect,
    [STRINGID_PKMNSXHADNOEFFECTONY - BATTLESTRINGS_ID_ADDER]          = sText_PkmnsXHadNoEffectOnY,
    [STRINGID_OAKPLAYERWON - BATTLESTRINGS_ID_ADDER]                  = gText_WinEarnsPrizeMoney,
    [STRINGID_OAKPLAYERLOST - BATTLESTRINGS_ID_ADDER]                 = gText_HowDissapointing,
    [STRINGID_TRAINER2NAME - BATTLESTRINGS_ID_ADDER]                  = gText_EmptyString,
    [STRINGID_MONTOOSCAREDTOMOVE - BATTLESTRINGS_ID_ADDER]            = sText_TooScaredToMove,
    [STRINGID_GHOSTGETOUTGETOUT - BATTLESTRINGS_ID_ADDER]             = sText_GetOutGetOut,
    [STRINGID_SILPHSCOPEUNVEILED - BATTLESTRINGS_ID_ADDER]            = sText_SilphScopeUnveil,
    [STRINGID_GHOSTWASMAROWAK - BATTLESTRINGS_ID_ADDER]               = sText_TheGhostWas,
    [STRINGID_TRAINER1MON2COMEBACK - BATTLESTRINGS_ID_ADDER]          = sText_Trainer1RecallPkmn2,
    [STRINGID_TRAINER1MON1AND2COMEBACK - BATTLESTRINGS_ID_ADDER]      = sText_Trainer1RecallBoth,
	[STRINGID_AROMAVEILPROTECTED - BATTLESTRINGS_ID_ADDER]            = sText_AromaVeilProtected,
	[STRINGID_FLOWERVEILPROTECTED - BATTLESTRINGS_ID_ADDER]           = sText_FlowerVeilProtected,
	[STRINGID_SWEETVEILPROTECTED - BATTLESTRINGS_ID_ADDER]            = sText_SweetVeilProtected,
	[STRINGID_HEAVYRAINLIFTED - BATTLESTRINGS_ID_ADDER]               = sText_HeavyRainLifted,
	[STRINGID_EXTREMESUNLIGHTFADED - BATTLESTRINGS_ID_ADDER]          = sText_ExtremeSunlightFaded,
	[STRINGID_STRONGWINDSDISSIPATED - BATTLESTRINGS_ID_ADDER]         = sText_StrongWindsDissipated,
	[STRINGID_PKMNSXMADEITHAIL - BATTLESTRINGS_ID_ADDER]              = sText_PkmnXMadeItHail,
	[STRINGID_HEAVYRAINBEGANFALL - BATTLESTRINGS_ID_ADDER]            = sText_HeavyRainBeganFall,
	[STRINGID_EXTREHARSHSUNLIGHT - BATTLESTRINGS_ID_ADDER]            = sText_SunlightTurnedExtremelyHarsh,
	[STRINGID_MYSTERIOUSAIRPROTECTINGPKMN - BATTLESTRINGS_ID_ADDER]   = sText_MysteriousAirProtectingPkmn,
	[STRINGID_WEATHEREFFECTSDISAPPEARED - BATTLESTRINGS_ID_ADDER]     = sText_WeatherEffectsDisappeared,
	[STRINGID_PKMNBREAKSTHEMOLD - BATTLESTRINGS_ID_ADDER]             = sText_PkmnBreaksTheMold,
	[STRINGID_PKMNCANTGETGOING - BATTLESTRINGS_ID_ADDER]              = sText_PkmnCantGetGoing,
	[STRINGID_RADIATINGABLAZINGAURA - BATTLESTRINGS_ID_ADDER]         = sText_RadiatingABlazingAura,
	[STRINGID_RADIATINGABURSTINGAURA - BATTLESTRINGS_ID_ADDER]        = sText_RadiatingABurstingAura,
	[STRINGID_RADIATINGADARKAURA - BATTLESTRINGS_ID_ADDER]            = sText_RadiatingADarkAura,
	[STRINGID_RADIATINGAFAIRYAURA - BATTLESTRINGS_ID_ADDER]           = sText_RadiatingAFairyAura,
	[STRINGID_REVERSEDALLOTHERAURAS - BATTLESTRINGS_ID_ADDER]         = sText_PkmnReversedAuras,
	[STRINGID_PKMNISDROWSING - BATTLESTRINGS_ID_ADDER]                = sText_PkmnIsDrowsing,
	[STRINGID_PKMNUNDERWENTTRANSFORMATION - BATTLESTRINGS_ID_ADDER]   = sText_PkmnUnderwentATransformation,
	[STRINGID_ABILITYTRIGGERED - BATTLESTRINGS_ID_ADDER]              = sText_AbilityTriggered,
	[STRINGID_PKMNGAINSOMEOFHP - BATTLESTRINGS_ID_ADDER]              = sText_PkmnGainSomeOfHp,
	[STRINGID_PKMNLOSTSOMEOFHP - BATTLESTRINGS_ID_ADDER]              = sText_PkmnLostSomeOfHp,
	[STRINGID_ATKABLCUREDEFFPROBLEM - BATTLESTRINGS_ID_ADDER]         = sText_AtkAblCuredEffProblem,
	[STRINGID_PKMNGOTITSACT - BATTLESTRINGS_ID_ADDER]                 = sText_PkmnGotItsActTogether,
	[STRINGID_DEFISTORMENTED - BATTLESTRINGS_ID_ADDER]                = sText_DefIsTormented,
	[STRINGID_ATKHARVESTEDITSITEM - BATTLESTRINGS_ID_ADDER]           = sText_AtkHarvestedItsLastItem,
	[STRINGID_ATKCANTUSECURRMOVE - BATTLESTRINGS_ID_ADDER]            = sText_AtkCantUseCurrMove,
	[STRINGID_DEFLASTABLRAISEDBUFF1 - BATTLESTRINGS_ID_ADDER]         = sText_DefLastAblRaisedItsBuff1,
	[STRINGID_DEFABLDISABLEDATKCURRMOVE - BATTLESTRINGS_ID_ADDER]     = sText_DefAblDisabledAtkCurrMove,
	[STRINGID_PKMNILLUSIONOFF - BATTLESTRINGS_ID_ADDER]               = sText_PkmnIllusionOff,
	[STRINGID_ATKABILITYBECAMEABL - BATTLESTRINGS_ID_ADDER]           = sText_AtkAbilityBecameAbl,
	[STRINGID_PKMNSHUDDERED - BATTLESTRINGS_ID_ADDER]                 = sText_PkmnShuddered,
	[STRINGID_ATTACKERABILITYSTATRAISE - BATTLESTRINGS_ID_ADDER]      = sText_AttackerAbilityRaisedStat,
	[STRINGID_ALERTEDTODEFBUFF1 - BATTLESTRINGS_ID_ADDER]             = sText_AlertedToDefBuff1,
	[STRINGID_ATKFRISKEDDEFFOUNDLASTITEM - BATTLESTRINGS_ID_ADDER]    = sText_AtkFriskedFoundLastItem,
	[STRINGID_ATKCUTHPMINIMIZEDATTACK - BATTLESTRINGS_ID_ADDER]       = sText_AtkCutHpMinimizedAttack,
	[STRINGID_ATKABLRAISEDITSSPEED - BATTLESTRINGS_ID_ADDER]          = sText_AtkAblRaisedItsSpeed,
	[STRINGID_ALLSCREENSCLEANSED - BATTLESTRINGS_ID_ADDER]            = sText_AllScreensCleansed,
	[STRINGID_BOTHWILLPERISHIN3TURNS - BATTLESTRINGS_ID_ADDER]        = sText_PerishInThree,
	[STRINGID_DEFSTATCHANGESWERERESET - BATTLESTRINGS_ID_ADDER]       = sText_DefStatChangesWereReset,
	[STRINGID_CURRMOVECHARGEDDEFPOWER - BATTLESTRINGS_ID_ADDER]       = sText_CurrMoveChargedDefPower,
	[STRINGID_NEUTRALIZINGGASOVER - BATTLESTRINGS_ID_ADDER]           = sText_NeutralizingGasOver,
	[STRINGID_PKMNABLFILLEDTHEAREA - BATTLESTRINGS_ID_ADDER]          = sText_PkmnAblFilledTheArea,
	[STRINGID_PASTELVEILPROTECTED - BATTLESTRINGS_ID_ADDER]           = sText_PastelVeilProtected,
	[STRINGID_PASTELVEILCUREDSTATUS - BATTLESTRINGS_ID_ADDER]         = sText_PastelVeilCuredStatus,
	[STRINGID_STARTEDSCHOOLING - BATTLESTRINGS_ID_ADDER]              = sText_StartedSchooling,
	[STRINGID_STOPPEDSCHOOLING - BATTLESTRINGS_ID_ADDER]              = sText_StoppedSchooling,
	[STRINGID_DISGUISESERVEDASDECOY - BATTLESTRINGS_ID_ADDER]         = sText_DisguiseServedAsDecoy,
	[STRINGID_DISGUISEBUSTED - BATTLESTRINGS_ID_ADDER]                = sText_DisguiseBusted,
	[STRINGID_PKMNGOTOVERINFATUATION - BATTLESTRINGS_ID_ADDER]        = sText_PkmnGotOverInfatuation,
	[STRINGID_PKMNSHOOKOFFTAUNT - BATTLESTRINGS_ID_ADDER]             = sText_PkmnShookOffTaunt,
	[STRINGID_PKMNEXERTINGPRESSURE - BATTLESTRINGS_ID_ADDER]          = sText_pkmnExertingPressure,
	[STRINGID_CHANGEDTOBLADE - BATTLESTRINGS_ID_ADDER]                = sText_ChangedToBlade,
	[STRINGID_CHANGEDTOSHIELD - BATTLESTRINGS_ID_ADDER]               = sText_ChangedToShield,
	[STRINGID_PKMNGAINEDSTRENGTH - BATTLESTRINGS_ID_ADDER]            = sText_PkmnGainedStrength,
	[STRINGID_PKMNABLWEAKENEDBUFF1 - BATTLESTRINGS_ID_ADDER]          = sText_PkmnAblWeakenedBuff1OfAllPkmn,
	[STRINGID_ATKFOUNDLASTITEM - BATTLESTRINGS_ID_ADDER]              = sText_AtkFoundOneLastItem,
	[STRINGID_PKMNHASTWOABILITIES - BATTLESTRINGS_ID_ADDER]           = sText_PkmnHasTwoAbilities,
	[STRINGID_PKMNABLWASTAKENOVER - BATTLESTRINGS_ID_ADDER]           = sText_PkmnAblWasTakenOver,
	[STRINGID_ITEMRESTOREDSPECIESHEALTH - BATTLESTRINGS_ID_ADDER]     = sText_ItemRestoredAtkHealth,
	[STRINGID_ITEMCUREDSPECIESSTATUS - BATTLESTRINGS_ID_ADDER]        = sText_ItemCuredAtkStatus,
	[STRINGID_ITEMRESTOREDSPECIESPP - BATTLESTRINGS_ID_ADDER]         = sText_ItemRestoredAtkPP,
	[STRINGID_BUFF2CUREDOFITSBUFF1 - BATTLESTRINGS_ID_ADDER]          = sText_Buff2CuredItsBuff1Problem,
	[STRINGID_ABILITYACTIVATED - BATTLESTRINGS_ID_ADDER]              = sText_AbilityActivated,
	[STRINGID_ABILITYDEACTIVATED - BATTLESTRINGS_ID_ADDER]            = sText_AbilityDeactivated,
	
	
	/*[STRINGID_PKMNRAISEDSPEED - BATTLESTRINGS_ID_ADDER]               = sText_PkmnRaisedSpeed,
	[STRINGID_PKMNHURTBYSPIKES - BATTLESTRINGS_ID_ADDER]              = sText_PkmnHurtBySpikes,
	[STRINGID_STATSWONTINCREASE - BATTLESTRINGS_ID_ADDER]             = sText_StatsWontIncrease,
    [STRINGID_STATSWONTDECREASE - BATTLESTRINGS_ID_ADDER]             = sText_StatsWontDecrease,
	[STRINGID_PKMNUPROARKEPTAWAKE - BATTLESTRINGS_ID_ADDER]           = sText_PkmnUproarKeptAwake,
    [STRINGID_PKMNCAUSEDUPROAR - BATTLESTRINGS_ID_ADDER]              = sText_PkmnCausedUproar,
    [STRINGID_PKMNMAKINGUPROAR - BATTLESTRINGS_ID_ADDER]              = sText_PkmnMakingUproar,
    [STRINGID_PKMNCALMEDDOWN - BATTLESTRINGS_ID_ADDER]                = sText_PkmnCalmedDown,
    [STRINGID_PKMNCANTSLEEPINUPROAR - BATTLESTRINGS_ID_ADDER]         = sText_PkmnCantSleepInUproar,
    [STRINGID_PKMNCANTSLEEPINUPROAR2 - BATTLESTRINGS_ID_ADDER]        = sText_PkmnCantSleepInUproar2,
    [STRINGID_UPROARKEPTPKMNAWAKE - BATTLESTRINGS_ID_ADDER]           = sText_UproarKeptPkmnAwake,
    [STRINGID_PKMNSTAYEDAWAKEUSING - BATTLESTRINGS_ID_ADDER]          = sText_PkmnStayedAwakeUsing,
	[STRINGID_PKMNLAIDCURSE - BATTLESTRINGS_ID_ADDER]                 = sText_PkmnLaidCurse,
    [STRINGID_PKMNAFFLICTEDBYCURSE - BATTLESTRINGS_ID_ADDER]          = sText_PkmnAfflictedByCurse,
	[STRINGID_PKMNSWITCHEDITEMS - BATTLESTRINGS_ID_ADDER]             = sText_PkmnSwitchedItems,
	[STRINGID_PKMNKNOCKEDOFF - BATTLESTRINGS_ID_ADDER]                = sText_PkmnKnockedOff,
	[STRINGID_PKMNSEALEDOPPONENTMOVE - BATTLESTRINGS_ID_ADDER]        = sText_PkmnSealedOpponentMove,
    [STRINGID_PKMNCANTUSEMOVESEALED - BATTLESTRINGS_ID_ADDER]         = sText_PkmnCantUseMoveSealed,
    [STRINGID_PKMNWANTSGRUDGE - BATTLESTRINGS_ID_ADDER]               = sText_PkmnWantsGrudge,
    [STRINGID_PKMNLOSTPPGRUDGE - BATTLESTRINGS_ID_ADDER]              = sText_PkmnLostPPGrudge,
    [STRINGID_PKMNWAITSFORTARGET - BATTLESTRINGS_ID_ADDER]            = sText_PkmnWaitsForTarget,
    [STRINGID_PKMNSNATCHEDMOVE - BATTLESTRINGS_ID_ADDER]              = sText_PkmnSnatchedMove,
	[STRINGID_STATSHARPLY - BATTLESTRINGS_ID_ADDER]                   = sText_StatSharply,
    [STRINGID_STATROSE - BATTLESTRINGS_ID_ADDER]                      = gBattleText_Rose,
    [STRINGID_STATHARSHLY - BATTLESTRINGS_ID_ADDER]                   = sText_StatHarshly,
    [STRINGID_STATFELL - BATTLESTRINGS_ID_ADDER]                      = sText_StatFell,
    [STRINGID_PKMNSSTATCHANGED - BATTLESTRINGS_ID_ADDER]              = sText_PkmnsStatChanged,
    [STRINGID_PKMNSSTATCHANGED2 - BATTLESTRINGS_ID_ADDER]             = gText_PkmnsStatChanged2,
    [STRINGID_PKMNSSTATCHANGED3 - BATTLESTRINGS_ID_ADDER]             = sText_PkmnsStatChanged3,
    [STRINGID_PKMNSSTATCHANGED4 - BATTLESTRINGS_ID_ADDER]             = sText_PkmnsStatChanged4,
	[STRINGID_PKMNPREVENTSPARALYSISWITH - BATTLESTRINGS_ID_ADDER]     = sText_PkmnPreventsParalysisWith,
    [STRINGID_PKMNPREVENTSROMANCEWITH - BATTLESTRINGS_ID_ADDER]       = sText_PkmnPreventsRomanceWith,
    [STRINGID_PKMNPREVENTSCONFUSIONWITH - BATTLESTRINGS_ID_ADDER]     = sText_PkmnPreventsConfusionWith,
    [STRINGID_PKMNANCHORSITSELFWITH - BATTLESTRINGS_ID_ADDER]         = sText_PkmnAnchorsItselfWith,
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
			StringCopy(gBattleTextBuff1, sText_ThePokeDude);
		else if (gBattleTypeFlags & BATTLE_TYPE_OLD_MAN_TUTORIAL)
			StringCopy(gBattleTextBuff1, sText_TheOldMan);
		else
			StringCopy(gBattleTextBuff1, sText_You);
	}
	else
		StringCopy(gBattleTextBuff1, sText_Trainer1);
}

// String buffers
void BufferStringBattle(u8 battlerId, u16 stringId)
{
	s32 i;
    const u8 *stringPtr = NULL;

    sBattleMsgDataPtr = (struct BattleMsgData*)(&gBattleBufferA[battlerId][4]);
    gLastUsedItem = sBattleMsgDataPtr->lastItem;
    gLastUsedAbility = sBattleMsgDataPtr->lastAbility;
    gBattleScripting.battler = sBattleMsgDataPtr->scrActive;
    gBattleStruct->hpScale = sBattleMsgDataPtr->hpScale;
    gBattleStruct->stringMoveType = sBattleMsgDataPtr->moveType;

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
                    stringPtr = sText_TwoLinkTrainersWantToBattle;
                else
					stringPtr = gTrainerBattleOpponent_A == TRAINER_UNION_ROOM ? sText_Trainer1WantsToBattle : sText_LinkTrainerWantsToBattle;
            }
            else
                stringPtr = sText_Trainer1WantsToBattle;
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_GHOST)
				stringPtr = (gBattleTypeFlags & BATTLE_TYPE_GHOST_UNVEILED) ? sText_TheGhostAppeared : sText_GhostAppearedCantId;
            else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                stringPtr = sText_TwoWildPkmnAppeared;
            else if (gBattleTypeFlags & BATTLE_TYPE_OLD_MAN_TUTORIAL)
                stringPtr = sText_WildPkmnAppearedPause;
            else
                stringPtr = sText_WildPkmnAppeared;
        }
        break;
    case STRINGID_INTROSENDOUT:
        if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
        {
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
				stringPtr = (gBattleTypeFlags & BATTLE_TYPE_MULTI) ? sText_LinkPartnerSentOutPkmnGoPkmn : sText_GoTwoPkmn;
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
                    stringPtr = sText_TwoLinkTrainersSentOutPkmn;
                else if (gBattleTypeFlags & BATTLE_TYPE_LINK)
                    stringPtr = sText_LinkTrainerSentOutTwoPkmn;
                else
                    stringPtr = sText_Trainer1SentOutTwoPkmn;
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
                stringPtr = sText_PkmnThatsEnough;
            else if (gBattleStruct->hpScale == 1 || gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                stringPtr = sText_PkmnComeBack;
            else if (gBattleStruct->hpScale == 2)
                stringPtr = sText_PkmnOkComeBack;
            else
                stringPtr = sText_PkmnGoodComeBack;
        }
        else
        {
            if (gTrainerBattleOpponent_A == TRAINER_LINK_OPPONENT)
				stringPtr = (gBattleTypeFlags & BATTLE_TYPE_MULTI) ? sText_LinkTrainer2WithdrewPkmn : sText_LinkTrainer1WithdrewPkmn;
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
                stringPtr = sText_DoItPkmn;
            else if (gBattleStruct->hpScale == 2)
                stringPtr = sText_GoForItPkmn;
            else
                stringPtr = sText_YourFoesWeakGetEmPkmn;
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = sText_LinkTrainerMultiSentOutPkmn;
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
	    StringCopy(gBattleTextBuff2, gMoveNames[sBattleMsgDataPtr->currentMove]);
        stringPtr = sText_AttackerUsedX;
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
                stringPtr = sText_TwoWildFled;
            else if (gTrainerBattleOpponent_A == TRAINER_UNION_ROOM)
                stringPtr = sText_UnionRoomTrainerFled;
            else
                stringPtr = sText_WildFled;
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
                    stringPtr = sText_TwoLinkTrainersDefeated;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostToTwo;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawVsTwo;
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
                    stringPtr = sText_PlayerLostAgainstTrainer1;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawTrainer1;
                    break;
                }
            }
            else
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = sText_PlayerDefeatedLinkTrainer;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostAgainstLinkTrainer;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawLinkTrainer;
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
		stringPtr = sText_Buff1UsedLastItem;
		break;
	case STRINGID_ITEMCATCHYTUNE:
		CopyTrainerStringCaseToBuffer1();
		stringPtr = sText_Buff1PlayedLastItemCatchyTune;
		break;
	case STRINGID_ITEMPLAYED:
	    CopyTrainerStringCaseToBuffer1();
		stringPtr = sText_Buff1PlayedLastItem;
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
    BattleStringExpandPlaceholders(src, gDisplayedStringBattle);
}

void BattleStringExpandPlaceholders(const u8 *src, u8 *dst)
{
	// This buffer may hold either the name of a trainer, Pokémon, or item.
	u8 text[max(max(max(32, 11), POKEMON_NAME_LENGTH + 1), ITEM_NAME_LENGTH)];
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
                    toCpy = gMoveNames[sBattleMsgDataPtr->currentMove];
					break;
				case B_TXT_CHOSEN_MOVE:
                    toCpy = gMoveNames[sBattleMsgDataPtr->chosenMove];
					break;
                case B_TXT_LAST_ITEM:
				    CopyItemName(gLastUsedItem, text);
					toCpy = text;
					break;
				case B_TXT_LAST_ABILITY:
                    toCpy = gAbilityNames[gLastUsedAbility];
					break;
				case B_TXT_ATK_ABILITY:
                    toCpy = gAbilityNames[sBattlerAbilities[gBattlerAttacker]];
					break;
				case B_TXT_DEF_ABILITY:
                    toCpy = gAbilityNames[sBattlerAbilities[gBattlerTarget]];
					break;
				case B_TXT_SCR_ACTIVE_ABILITY:
                    toCpy = gAbilityNames[sBattlerAbilities[gBattleScripting.battler]];
					break;
				case B_TXT_EFF_ABILITY:
                    toCpy = gAbilityNames[sBattlerAbilities[gEffectBattler]];
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
            StringAppend(dst, gMoveNames[READ_16(&src[srcId + 1])]);
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
            StringAppend(dst, gPokeblockWasTooXStringTable[src[srcId + 1]]);
            srcId += 2;
            break;
        case B_BUFF_ABILITY:
            StringAppend(dst, gAbilityNames[READ_16(&src[srcId + 1])]);
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
    if (stringId == STRINGID_TRAINER1LOSETEXT || stringId == STRINGID_TRAINER1WINTEXT || stringId == STRINGID_TRAINER2NAME)
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

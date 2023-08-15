#ifndef GUARD_BATTLE_INTERFACE_H
#define GUARD_BATTLE_INTERFACE_H

#include "global.h"
#include "battle_controllers.h"

// data fields for healthboxMain
#define hMain_HealthBoxOtherSpriteId oam.affineParam // Holds sprite id of healthboxRight
#define hMain_HealthBoxSlideSpeedX   data[4] // For slide anim
#define hMain_HealthBarSpriteId      data[5] // Holds sprite id of healthbar
#define hMain_Battler                data[6] // Holds the battler id
#define hMain_IndicatorSpriteId      data[7] // Holds indicator sprite id

// data fields for healthboxRight
#define hOther_HealthBoxSpriteId     data[5] // Holds sprite id of healthboxMain

// data fields for healthbar
#define hBar_HealthBoxSpriteId       data[5] // Holds sprite id of healthboxMain
#define hBar_Type                    data[6] // Holds the healthbar type id (player singles, player doubles or opponent)

// Args for UpdateHpTextInHealthbox
enum
{
    HP_CURRENT,
    HP_MAX
};

// Args for MoveBattleBar
enum
{
    HEALTH_BAR,
    EXP_BAR
};

// Return values of GetHPBarLevel
enum
{
    HP_BAR_EMPTY,
    HP_BAR_RED,
    HP_BAR_YELLOW,
    HP_BAR_GREEN,
    HP_BAR_FULL,
};

// Health box
#define TAG_HEALTHBOX_PLAYER1_TILE              0xD6FF
#define TAG_HEALTHBOX_PLAYER2_TILE              0xD700
#define TAG_HEALTHBOX_OPPONENT1_TILE            0xD701
#define TAG_HEALTHBOX_OPPONENT2_TILE            0xD702
#define TAG_HEALTHBOX_SAFARI_TILE               TAG_HEALTHBOX_PLAYER1_TILE

#define TAG_HEALTHBOX_PALS_1                    0xD703
#define TAG_HEALTHBOX_PALS_2                    0xD704
#define TAG_HEALTHBOX_PAL                       TAG_HEALTHBOX_PLAYER1_TILE

// Health bar
#define TAG_HEALTHBAR_PLAYER1_TILE              0xD705
#define TAG_HEALTHBAR_OPPONENT1_TILE            0xD706
#define TAG_HEALTHBAR_PLAYER2_TILE              0xD707
#define TAG_HEALTHBAR_OPPONENT2_TILE            0xD708

#define TAG_HEALTHBAR_PAL                       TAG_HEALTHBAR_PLAYER1_TILE

// Summary bar
#define TAG_PARTY_SUMMARY_BAR_PLAYER_TILE       0xD709
#define TAG_PARTY_SUMMARY_BAR_OPPONENT_TILE     0xD70A

#define TAG_PARTY_SUMMARY_BAR_PLAYER_PAL        TAG_PARTY_SUMMARY_BAR_PLAYER_TILE
#define TAG_PARTY_SUMMARY_BAR_OPPONENT_PAL      TAG_PARTY_SUMMARY_BAR_OPPONENT_TILE

// Summary ball
#define TAG_PARTY_SUMMARY_BALL_PLAYER_TILE      0xD70B
#define TAG_PARTY_SUMMARY_BALL_OPPONENT_TILE    0xD70C

#define TAG_PARTY_SUMMARY_BALL_PLAYER_PAL       TAG_PARTY_SUMMARY_BALL_PLAYER_TILE
#define TAG_PARTY_SUMMARY_BALL_OPPONENT_PAL     TAG_PARTY_SUMMARY_BALL_OPPONENT_TILE

// Weather icon
#define TAG_WEATHER_ICON_GFX                    0xD70D

// Args for UpdateHealthboxAttribute
enum
{
    HEALTHBOX_ALL,
    HEALTHBOX_CURRENT_HP,
    HEALTHBOX_MAX_HP,
    HEALTHBOX_LEVEL,
    HEALTHBOX_NICK,
    HEALTHBOX_HEALTH_BAR,
    HEALTHBOX_EXP_BAR,
    HEALTHBOX_STATUS_ICON,
    HEALTHBOX_SAFARI_ALL_TEXT,
    HEALTHBOX_SAFARI_BALLS_TEXT
};

void Task_HidePartyStatusSummary(u8 taskId);
u8 CreateBattlerHealthboxSprites(u8 battlerId);
u8 CreateSafariPlayerHealthboxSprites(void);
void SetBattleBarStruct(u8 battlerId, s32 maxVal, s32 currVal, s32 receivedValue);
void SetHealthboxSpriteInvisible(u8 healthboxSpriteId);
void SetHealthboxSpriteVisible(u8 healthboxSpriteId);
void DestoryHealthboxSprite(u8 healthboxSpriteId);
void UpdateOamPriorityInAllHealthboxes(u8 priority, bool8 hideHpBoxes);
void InitBattlerHealthboxCoords(u8 battlerId);
void UpdateHpTextInHealthbox(u8 healthboxSpriteId, s16 value, u8 maxOrCurrent);
void SwapHpBarsWithHpText(void);
u8 CreatePartyStatusSummarySprites(u8 battlerId, struct HpAndStatus *partyInfo, u8 isSwitchingMons, bool8 isBattleStart);
void UpdateHealthboxAttribute(u8 battlerId, u8 elementId);
u8 GetScaledHPFraction(s16 hp, s16 maxhp, u8 scale);
u8 GetHPBarLevel(s16 hp, s16 maxhp);
void TryAddPokeballIconToHealthbox(u8 spriteId, u8);
s32 MoveBattleBar(u8 battler, u8 healthboxSpriteId, u8 whichBar);
void StartHealthboxSlideIn(u8 battler);
void TryCreateWeatherAnimIcon(void);
void ShowOrHideWeatherAnimIcon(bool8 hide);

#endif // GUARD_BATTLE_INTERFACE_H

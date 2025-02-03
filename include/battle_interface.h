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

enum
{   // Corresponds to gHealthboxElementsGfxTable (and the tables after it) in graphics.c
    // These are indexes into the tables, which are filled with 8x8 square pixel data.
    HEALTHBOX_GFX_0, //hp bar [black section]
    HEALTHBOX_GFX_1, //hp bar "H"
    HEALTHBOX_GFX_2, //hp bar "P"
    HEALTHBOX_GFX_HP_BAR_GREEN, //hp bar [0 pixels]
    HEALTHBOX_GFX_4,  //hp bar [1 pixels]
    HEALTHBOX_GFX_5,  //hp bar [2 pixels]
    HEALTHBOX_GFX_6,  //hp bar [3 pixels]
    HEALTHBOX_GFX_7,  //hp bar [4 pixels]
    HEALTHBOX_GFX_8,  //hp bar [5 pixels]
    HEALTHBOX_GFX_9,  //hp bar [6 pixels]
    HEALTHBOX_GFX_10, //hp bar [7 pixels]
    HEALTHBOX_GFX_11, //hp bar [8 pixels]
    HEALTHBOX_GFX_12, //exp bar [0 pixels]
    HEALTHBOX_GFX_13, //exp bar [1 pixels]
    HEALTHBOX_GFX_14, //exp bar [2 pixels]
    HEALTHBOX_GFX_15, //exp bar [3 pixels]
    HEALTHBOX_GFX_16, //exp bar [4 pixels]
    HEALTHBOX_GFX_17, //exp bar [5 pixels]
    HEALTHBOX_GFX_18, //exp bar [6 pixels]
    HEALTHBOX_GFX_19, //exp bar [7 pixels]
    HEALTHBOX_GFX_20, //exp bar [8 pixels]
    HEALTHBOX_GFX_STATUS_PSN_BATTLER0,  //status psn "(P"
    HEALTHBOX_GFX_22,                   //status psn "SN"
    HEALTHBOX_GFX_23,                   //status psn "|)""
    HEALTHBOX_GFX_STATUS_PRZ_BATTLER0,  //status prz
    HEALTHBOX_GFX_25,
    HEALTHBOX_GFX_26,
    HEALTHBOX_GFX_STATUS_SLP_BATTLER0,  //status slp
    HEALTHBOX_GFX_28,
    HEALTHBOX_GFX_29,
    HEALTHBOX_GFX_STATUS_FRZ_BATTLER0,  //status frz
    HEALTHBOX_GFX_31,
    HEALTHBOX_GFX_32,
    HEALTHBOX_GFX_STATUS_BRN_BATTLER0,  //status brn
    HEALTHBOX_GFX_34,
    HEALTHBOX_GFX_35,
    HEALTHBOX_GFX_36, //misc [Black section]
    HEALTHBOX_GFX_37, //misc [Black section]
    HEALTHBOX_GFX_38, //misc [Black section]
    HEALTHBOX_GFX_39, //misc [Blank Health Window?]
    HEALTHBOX_GFX_40, //misc [Blank Health Window?]
    HEALTHBOX_GFX_41, //misc [Blank Health Window?]
    HEALTHBOX_GFX_42, //misc [Blank Health Window?]
    HEALTHBOX_GFX_43, //misc [Top of Health Window?]
    HEALTHBOX_GFX_44, //misc [Top of Health Window?]
    HEALTHBOX_GFX_45, //misc [Top of Health Window?]
    HEALTHBOX_GFX_46, //misc [Blank Health Window?]
    HEALTHBOX_GFX_HP_BAR_YELLOW, //hp bar yellow [0 pixels]
    HEALTHBOX_GFX_48, //hp bar yellow [1 pixels]
    HEALTHBOX_GFX_49, //hp bar yellow [2 pixels]
    HEALTHBOX_GFX_50, //hp bar yellow [3 pixels]
    HEALTHBOX_GFX_51, //hp bar yellow [4 pixels]
    HEALTHBOX_GFX_52, //hp bar yellow [5 pixels]
    HEALTHBOX_GFX_53, //hp bar yellow [6 pixels]
    HEALTHBOX_GFX_54, //hp bar yellow [7 pixels]
    HEALTHBOX_GFX_55, //hp bar yellow [8 pixels]
    HEALTHBOX_GFX_HP_BAR_RED,  //hp bar red [0 pixels]
    HEALTHBOX_GFX_57, //hp bar red [1 pixels]
    HEALTHBOX_GFX_58, //hp bar red [2 pixels]
    HEALTHBOX_GFX_59, //hp bar red [3 pixels]
    HEALTHBOX_GFX_60, //hp bar red [4 pixels]
    HEALTHBOX_GFX_61, //hp bar red [5 pixels]
    HEALTHBOX_GFX_62, //hp bar red [6 pixels]
    HEALTHBOX_GFX_63, //hp bar red [7 pixels]
    HEALTHBOX_GFX_64, //hp bar red [8 pixels]
    HEALTHBOX_GFX_65, //hp bar frame end
    HEALTHBOX_GFX_66, //status ball [full]
    HEALTHBOX_GFX_67, //status ball [empty]
    HEALTHBOX_GFX_68, //status ball [fainted]
    HEALTHBOX_GFX_69, //status ball [statused]
    HEALTHBOX_GFX_70, //status ball [unused extra]
    HEALTHBOX_GFX_STATUS_PSN_BATTLER1, //status2 "PSN"
    HEALTHBOX_GFX_72,
    HEALTHBOX_GFX_73,
    HEALTHBOX_GFX_STATUS_PRZ_BATTLER1, //status2 "PRZ"
    HEALTHBOX_GFX_75,
    HEALTHBOX_GFX_76,
    HEALTHBOX_GFX_STATUS_SLP_BATTLER1, //status2 "SLP"
    HEALTHBOX_GFX_78,
    HEALTHBOX_GFX_79,
    HEALTHBOX_GFX_STATUS_FRZ_BATTLER1, //status2 "FRZ"
    HEALTHBOX_GFX_81,
    HEALTHBOX_GFX_82,
    HEALTHBOX_GFX_STATUS_BRN_BATTLER1, //status2 "BRN"
    HEALTHBOX_GFX_84,
    HEALTHBOX_GFX_85,
    HEALTHBOX_GFX_STATUS_PSN_BATTLER2, //status3 "PSN"
    HEALTHBOX_GFX_87,
    HEALTHBOX_GFX_88,
    HEALTHBOX_GFX_STATUS_PRZ_BATTLER2, //status3 "PRZ"
    HEALTHBOX_GFX_90,
    HEALTHBOX_GFX_91,
    HEALTHBOX_GFX_STATUS_SLP_BATTLER2, //status3 "SLP"
    HEALTHBOX_GFX_93,
    HEALTHBOX_GFX_94,
    HEALTHBOX_GFX_STATUS_FRZ_BATTLER2, //status3 "FRZ"
    HEALTHBOX_GFX_96,
    HEALTHBOX_GFX_97,
    HEALTHBOX_GFX_STATUS_BRN_BATTLER2, //status3 "BRN"
    HEALTHBOX_GFX_99,
    HEALTHBOX_GFX_100,
    HEALTHBOX_GFX_STATUS_PSN_BATTLER3, //status4 "PSN"
    HEALTHBOX_GFX_102,
    HEALTHBOX_GFX_103,
    HEALTHBOX_GFX_STATUS_PRZ_BATTLER3, //status4 "PRZ"
    HEALTHBOX_GFX_105,
    HEALTHBOX_GFX_106,
    HEALTHBOX_GFX_STATUS_SLP_BATTLER3, //status4 "SLP"
    HEALTHBOX_GFX_108,
    HEALTHBOX_GFX_109,
    HEALTHBOX_GFX_STATUS_FRZ_BATTLER3, //status4 "FRZ"
    HEALTHBOX_GFX_111,
    HEALTHBOX_GFX_112,
    HEALTHBOX_GFX_STATUS_BRN_BATTLER3, //status4 "BRN"
    HEALTHBOX_GFX_114,
    HEALTHBOX_GFX_115,
    HEALTHBOX_GFX_116, //unknown_D12FEC
    HEALTHBOX_GFX_117, //unknown_D1300C
};

// Args for UpdateHpTextInHealthbox
enum
{
    HP_CURRENT,
    HP_MAX,
	HP_BOTH,
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
#define TAG_SHADOW_PAL                          TAG_HEALTHBOX_PLAYER1_TILE

#define TAG_SHADOW_TILE                         0xD759

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

// Move info trigger
#define TAG_MOVEINFO_TRIGGER_GFX                0xD70E

#define ENEMY_SHADOW_Y_OFFSET                   27

void Task_HidePartyStatusSummary(u8 taskId);
u8 CreateBattlerHealthboxSprites(u8 battlerId);
u8 CreateSafariPlayerHealthboxSprites(void);
void SetBattleBarStruct(u8 battlerId, s32 maxVal, s32 currVal, s32 receivedValue);
void SetHealthboxSpriteInvisible(u8 healthboxSpriteId);
void SetHealthboxSpriteVisible(u8 healthboxSpriteId);
void DestoryHealthboxSprite(u8 healthboxSpriteId);
void UpdateOamPriorityInAllHealthboxes(u8 priority, bool8 hideHpBoxes);
void InitBattlerHealthboxCoords(u8 battlerId);
void UpdateHpTextInHealthbox(u8 healthboxSpriteId, u8 maxOrCurrent, s16 currHP, s16 maxHP);
void SwapHpBarsWithHpText(void);
u8 CreatePartyStatusSummarySprites(u8 battlerId, struct HpAndStatus *partyInfo, u8 isSwitchingMons, bool8 isBattleStart);
void UpdateHealthboxAttribute(u8 battlerId, u8 elementId);
u8 GetScaledHPFraction(s16 hp, s16 maxhp, u8 scale);
u8 GetHPBarLevel(s16 hp, s16 maxhp);
void TryAddPokeballIconToHealthbox(u8 spriteId, bool8 noStatus);
s32 MoveBattleBar(u8 battler, u8 healthboxSpriteId, u8 whichBar);
void StartHealthboxSlideIn(u8 battler);
void TryCreateWeatherAnimIcon(void);
void ShowOrHideWeatherAnimIcon(bool8 hide);
void CreateMoveInfoTriggerSprite(u8 battlerId);
void ShowOrHideMoveInfoTriggerSprite(bool8 hide);

#endif // GUARD_BATTLE_INTERFACE_H

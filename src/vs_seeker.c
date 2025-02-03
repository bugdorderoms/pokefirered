#include "global.h"
#include "gflib.h"
#include "constants/songs.h"
#include "task.h"
#include "event_object_movement.h"
#include "new_menu_helpers.h"
#include "item_use.h"
#include "event_scripts.h"
#include "event_data.h"
#include "script.h"
#include "event_object_lock.h"
#include "field_specials.h"
#include "item.h"
#include "item_menu.h"
#include "field_effect.h"
#include "script_movement.h"
#include "battle.h"
#include "battle_setup.h"
#include "random.h"
#include "field_player_avatar.h"
#include "vs_seeker.h"
#include "ride_pager.h"
#include "strings.h"
#include "constants/event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/maps.h"
#include "constants/items.h"
#include "constants/trainer_types.h"

enum
{
   VSSEEKER_NOT_CHARGED,
   VSSEEKER_NO_ONE_IN_RANGE,
   VSSEEKER_CAN_USE,
};

typedef enum
{
    VSSEEKER_SINGLE_RESP_RAND,
    VSSEEKER_SINGLE_RESP_NO,
    VSSEEKER_SINGLE_RESP_YES
} VsSeekerSingleRespCode;

typedef enum
{
    VSSEEKER_RESPONSE_NO_RESPONSE,
    VSSEEKER_RESPONSE_UNFOUGHT_TRAINERS,
    VSSEEKER_RESPONSE_FOUND_REMATCHES
} VsSeekerResponseCode;

// static types
typedef struct VsSeekerData
{
    u16 trainerIdxs[6];
} VsSeekerData;

struct VsSeekerTrainerInfo
{
    const u8 *script;
    u16 trainerIdx;
    u8 localId;
    u8 objectEventId;
    s16 xCoord;
    s16 yCoord;
    u8 graphicsId;
};

struct VsSeekerStruct
{
    struct VsSeekerTrainerInfo trainerInfo[OBJECT_EVENTS_COUNT];
    u16 trainerIdxArray[OBJECT_EVENTS_COUNT];
    u8 runningBehaviourEtcArray[OBJECT_EVENTS_COUNT];
    u8 numRematchableTrainers;
    u8 trainerHasNotYetBeenFought:1;
    u8 trainerDoesNotWantRematch:1;
    u8 trainerWantsRematch:1;
    u8 responseCode:5;
};

// static declarations
static EWRAM_DATA struct VsSeekerStruct *sVsSeeker = NULL;

static void VsSeekerResetInBagStepCounter(void);
static void VsSeekerResetChargingStepCounter(void);
static void Task_ResetObjectsRematchWantedState(u8 taskId);
static void ResetMovementOfRematchableTrainers(void);
static void Task_VsSeeker_1(u8 taskId);
static void Task_VsSeeker_2(u8 taskId);
static void GatherNearbyTrainerInfo(void);
static void Task_VsSeeker_3(u8 taskId);
static bool8 CanUseVsSeeker(void);
static u8 GetVsSeekerResponseInArea(const VsSeekerData * vsSeekerData);
static u8 GetRematchTrainerIdGivenGameState(const u16 *trainerIdxs, u8 rematchIdx);
static u8 ShouldTryRematchBattleInternal(const VsSeekerData * vsSeekerData, u16 trainerBattleOpponent);
static u8 HasRematchTrainerAlreadyBeenFought(const VsSeekerData * vsSeekerData, u16 trainerBattleOpponent);
static int LookupVsSeekerOpponentInArray(const VsSeekerData * array, u16 trainerId);
static bool8 IsTrainerReadyForRematchInternal(const VsSeekerData * array, u16 trainerIdx);
static u8 GetRunningBehaviorFromGraphicsId(u8 graphicsId);
static u16 GetTrainerFlagFromScript(const u8 * script);
static int GetRematchIdx(const VsSeekerData * vsSeekerData, u16 trainerFlagIdx);
static bool32 IsThisTrainerRematchable(u32 localId);
static void ClearAllTrainerRematchStates(void);
static bool8 IsTrainerVisibleOnScreen(struct VsSeekerTrainerInfo * trainerInfo);
static u8 GetNextAvailableRematchTrainer(const VsSeekerData * vsSeekerData, u16 trainerFlagNo, u8 * idxPtr);
static u8 GetRematchableTrainerLocalId(void);
static void StartTrainerObjectMovementScript(struct VsSeekerTrainerInfo * trainerInfo, const u8 * script);
static u8 GetCurVsSeekerResponse(s32 vsSeekerIdx, u16 trainerIdx);
static void StartAllRespondantIdleMovements(void);
static bool8 ObjectEventIdIsSane(u8 objectEventId);
static u8 GetRandomFaceDirectionMovementType(void);

// rodata
#define VS_SEEKER_DATA(...) { .trainerIdxs = {__VA_ARGS__}, }

static const VsSeekerData sVsSeekerData[] =
{
   VS_SEEKER_DATA(TRAINER_YOUNGSTER_BEN, TRAINER_YOUNGSTER_BEN_2, 0xFFFF, TRAINER_YOUNGSTER_BEN_3, TRAINER_YOUNGSTER_BEN_4),
   VS_SEEKER_DATA(TRAINER_YOUNGSTER_CALVIN, TRAINER_YOUNGSTER_CALVIN),
   VS_SEEKER_DATA(TRAINER_BUG_CATCHER_COLTON, TRAINER_BUG_CATCHER_COLTON_2, 0xFFFF, TRAINER_BUG_CATCHER_COLTON_3, 0xFFFF, TRAINER_BUG_CATCHER_COLTON_4),
   VS_SEEKER_DATA(TRAINER_BUG_CATCHER_GREG, TRAINER_BUG_CATCHER_GREG),
   VS_SEEKER_DATA(TRAINER_BUG_CATCHER_JAMES, TRAINER_BUG_CATCHER_JAMES),
   VS_SEEKER_DATA(TRAINER_LASS_JANICE, TRAINER_LASS_JANICE_2, 0xFFFF, TRAINER_LASS_JANICE_3),
   VS_SEEKER_DATA(TRAINER_LASS_SALLY, TRAINER_LASS_SALLY),
   VS_SEEKER_DATA(TRAINER_LASS_ROBIN, TRAINER_LASS_ROBIN),
   VS_SEEKER_DATA(TRAINER_LASS_CRISSY, TRAINER_LASS_CRISSY),
   VS_SEEKER_DATA(TRAINER_YOUNGSTER_TIMMY, TRAINER_YOUNGSTER_TIMMY_2, 0xFFFF, TRAINER_YOUNGSTER_TIMMY_3, 0xFFFF, TRAINER_YOUNGSTER_TIMMY_4),
   VS_SEEKER_DATA(TRAINER_BUG_CATCHER_CALE, TRAINER_BUG_CATCHER_CALE),
   VS_SEEKER_DATA(TRAINER_LASS_RELI, TRAINER_LASS_RELI_2, 0xFFFF, TRAINER_LASS_RELI_3),
   VS_SEEKER_DATA(TRAINER_LASS_ALI, TRAINER_LASS_ALI),
   VS_SEEKER_DATA(TRAINER_CAMPER_SHANE, TRAINER_CAMPER_SHANE),
   VS_SEEKER_DATA(TRAINER_CAMPER_ETHAN, TRAINER_CAMPER_ETHAN),
   VS_SEEKER_DATA(TRAINER_YOUNGSTER_JOEY, TRAINER_YOUNGSTER_JOEY),
   VS_SEEKER_DATA(TRAINER_YOUNGSTER_DAN, TRAINER_YOUNGSTER_DAN),
   VS_SEEKER_DATA(TRAINER_YOUNGSTER_CHAD, TRAINER_YOUNGSTER_CHAD_2, 0xFFFF, TRAINER_YOUNGSTER_CHAD_3, TRAINER_YOUNGSTER_CHAD_4),
   VS_SEEKER_DATA(TRAINER_PICNICKER_KELSEY, TRAINER_PICNICKER_KELSEY_2, 0xFFFF, TRAINER_PICNICKER_KELSEY_3, TRAINER_PICNICKER_KELSEY_4),
   VS_SEEKER_DATA(TRAINER_LASS_HALEY, TRAINER_LASS_HALEY),
   VS_SEEKER_DATA(TRAINER_HIKER_FRANKLIN, 0xFFFF, TRAINER_HIKER_FRANKLIN_2),
   VS_SEEKER_DATA(TRAINER_HIKER_NOB, TRAINER_HIKER_NOB),
   VS_SEEKER_DATA(TRAINER_HIKER_WAYNE, TRAINER_HIKER_WAYNE),
   VS_SEEKER_DATA(TRAINER_CAMPER_FLINT, TRAINER_CAMPER_FLINT),
   VS_SEEKER_DATA(TRAINER_BUG_CATCHER_KEIGO, TRAINER_BUG_CATCHER_KEIGO),
   VS_SEEKER_DATA(TRAINER_BUG_CATCHER_ELIJAH, TRAINER_BUG_CATCHER_ELIJAH),
   VS_SEEKER_DATA(TRAINER_CAMPER_RICKY, TRAINER_CAMPER_RICKY_2, 0xFFFF, TRAINER_CAMPER_RICKY_3, 0xFFFF, TRAINER_CAMPER_RICKY_4),
   VS_SEEKER_DATA(TRAINER_CAMPER_JEFF, TRAINER_CAMPER_JEFF_2, 0xFFFF, TRAINER_CAMPER_JEFF_3, 0xFFFF, TRAINER_CAMPER_JEFF_4),
   VS_SEEKER_DATA(TRAINER_PICNICKER_NANCY, TRAINER_PICNICKER_NANCY),
   VS_SEEKER_DATA(TRAINER_PICNICKER_ISABELLE, TRAINER_PICNICKER_ISABELLE_2, 0xFFFF, TRAINER_PICNICKER_ISABELLE_3, TRAINER_PICNICKER_ISABELLE_4),
   VS_SEEKER_DATA(TRAINER_YOUNGSTER_EDDIE, TRAINER_YOUNGSTER_EDDIE),
   VS_SEEKER_DATA(TRAINER_YOUNGSTER_DILLON, TRAINER_YOUNGSTER_DILLON),
   VS_SEEKER_DATA(TRAINER_YOUNGSTER_YASU, 0xFFFF, TRAINER_YOUNGSTER_YASU_2, 0xFFFF, TRAINER_YOUNGSTER_YASU_3),
   VS_SEEKER_DATA(TRAINER_YOUNGSTER_DAVE, TRAINER_YOUNGSTER_DAVE),
   VS_SEEKER_DATA(TRAINER_ENGINEER_BRAXTON, TRAINER_ENGINEER_BRAXTON),
   VS_SEEKER_DATA(TRAINER_ENGINEER_BERNIE, 0xFFFF, 0xFFFF, TRAINER_ENGINEER_BERNIE_2),
   VS_SEEKER_DATA(TRAINER_GAMER_HUGO, TRAINER_GAMER_HUGO),
   VS_SEEKER_DATA(TRAINER_GAMER_JASPER, TRAINER_GAMER_JASPER),
   VS_SEEKER_DATA(TRAINER_GAMER_DIRK, TRAINER_GAMER_DIRK),
   VS_SEEKER_DATA(TRAINER_GAMER_DARIAN, 0xFFFF, 0xFFFF, TRAINER_GAMER_DARIAN_2),
   VS_SEEKER_DATA(TRAINER_BUG_CATCHER_BRENT, TRAINER_BUG_CATCHER_BRENT),
   VS_SEEKER_DATA(TRAINER_BUG_CATCHER_CONNER, TRAINER_BUG_CATCHER_CONNER),
   VS_SEEKER_DATA(TRAINER_CAMPER_CHRIS, 0xFFFF, TRAINER_CAMPER_CHRIS_2, TRAINER_CAMPER_CHRIS_3, 0xFFFF, TRAINER_CAMPER_CHRIS_4),
   VS_SEEKER_DATA(TRAINER_CAMPER_DREW, TRAINER_CAMPER_DREW),
   VS_SEEKER_DATA(TRAINER_PICNICKER_ALICIA, 0xFFFF, TRAINER_PICNICKER_ALICIA_2, TRAINER_PICNICKER_ALICIA_3, 0xFFFF, TRAINER_PICNICKER_ALICIA_4),
   VS_SEEKER_DATA(TRAINER_PICNICKER_CAITLIN, TRAINER_PICNICKER_CAITLIN),
   VS_SEEKER_DATA(TRAINER_HIKER_ALAN, TRAINER_HIKER_ALAN),
   VS_SEEKER_DATA(TRAINER_HIKER_BRICE, TRAINER_HIKER_BRICE),
   VS_SEEKER_DATA(TRAINER_HIKER_JEREMY, 0xFFFF, 0xFFFF, TRAINER_HIKER_JEREMY_2),
   VS_SEEKER_DATA(TRAINER_PICNICKER_HEIDI, TRAINER_PICNICKER_HEIDI),
   VS_SEEKER_DATA(TRAINER_PICNICKER_CAROL, TRAINER_PICNICKER_CAROL),
   VS_SEEKER_DATA(TRAINER_POKEMANIAC_MARK, 0xFFFF, 0xFFFF, TRAINER_POKEMANIAC_MARK_2, 0xFFFF, TRAINER_POKEMANIAC_MARK_3),
   VS_SEEKER_DATA(TRAINER_POKEMANIAC_HERMAN, 0xFFFF, 0xFFFF, TRAINER_POKEMANIAC_HERMAN_2, 0xFFFF, TRAINER_POKEMANIAC_HERMAN_3),
   VS_SEEKER_DATA(TRAINER_HIKER_CLARK, TRAINER_HIKER_CLARK),
   VS_SEEKER_DATA(TRAINER_HIKER_TRENT, 0xFFFF, 0xFFFF, TRAINER_HIKER_TRENT_2),
   VS_SEEKER_DATA(TRAINER_LASS_PAIGE, TRAINER_LASS_PAIGE),
   VS_SEEKER_DATA(TRAINER_LASS_ANDREA, TRAINER_LASS_ANDREA),
   VS_SEEKER_DATA(TRAINER_LASS_MEGAN, 0xFFFF, TRAINER_LASS_MEGAN_2, 0xFFFF, TRAINER_LASS_MEGAN_3),
   VS_SEEKER_DATA(TRAINER_LASS_JULIA, TRAINER_LASS_JULIA),
   VS_SEEKER_DATA(TRAINER_SUPER_NERD_AIDAN, TRAINER_SUPER_NERD_AIDAN),
   VS_SEEKER_DATA(TRAINER_SUPER_NERD_GLENN, 0xFFFF, 0xFFFF, TRAINER_SUPER_NERD_GLENN_2),
   VS_SEEKER_DATA(TRAINER_SUPER_NERD_LESLIE, TRAINER_SUPER_NERD_LESLIE),
   VS_SEEKER_DATA(TRAINER_GAMER_STAN, TRAINER_GAMER_STAN),
   VS_SEEKER_DATA(TRAINER_GAMER_RICH, 0xFFFF, 0xFFFF, TRAINER_GAMER_RICH_2),
   VS_SEEKER_DATA(TRAINER_TWINS_ELI_ANNE, 0xFFFF, 0xFFFF, TRAINER_TWINS_ELI_ANNE_2),
   VS_SEEKER_DATA(TRAINER_BIKER_RICARDO, TRAINER_BIKER_RICARDO),
   VS_SEEKER_DATA(TRAINER_BIKER_JAREN, 0xFFFF, 0xFFFF, TRAINER_BIKER_JAREN_2),
   VS_SEEKER_DATA(TRAINER_FISHERMAN_NED, TRAINER_FISHERMAN_NED),
   VS_SEEKER_DATA(TRAINER_FISHERMAN_CHIP, TRAINER_FISHERMAN_CHIP),
   VS_SEEKER_DATA(TRAINER_FISHERMAN_HANK, TRAINER_FISHERMAN_HANK),
   VS_SEEKER_DATA(TRAINER_FISHERMAN_ELLIOT, 0xFFFF, 0xFFFF, TRAINER_FISHERMAN_ELLIOT_2),
   VS_SEEKER_DATA(TRAINER_FISHERMAN_ANDREW, TRAINER_FISHERMAN_ANDREW),
   VS_SEEKER_DATA(TRAINER_ROCKER_LUCA, 0xFFFF, 0xFFFF, TRAINER_ROCKER_LUCA_2),
   VS_SEEKER_DATA(TRAINER_CAMPER_JUSTIN, TRAINER_CAMPER_JUSTIN),
   VS_SEEKER_DATA(TRAINER_YOUNG_COUPLE_GIA_JES, 0xFFFF, 0xFFFF, TRAINER_YOUNG_COUPLE_GIA_JES_2, 0xFFFF, TRAINER_YOUNG_COUPLE_GIA_JES_3),
   VS_SEEKER_DATA(TRAINER_BIKER_JARED, TRAINER_BIKER_JARED),
   VS_SEEKER_DATA(TRAINER_BEAUTY_LOLA, TRAINER_BEAUTY_LOLA),
   VS_SEEKER_DATA(TRAINER_BEAUTY_SHEILA, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_BEAUTY_SHEILA_2),
   VS_SEEKER_DATA(TRAINER_BIRD_KEEPER_SEBASTIAN, TRAINER_BIRD_KEEPER_SEBASTIAN),
   VS_SEEKER_DATA(TRAINER_BIRD_KEEPER_PERRY, TRAINER_BIRD_KEEPER_PERRY),
   VS_SEEKER_DATA(TRAINER_BIRD_KEEPER_ROBERT, 0xFFFF, 0xFFFF, TRAINER_BIRD_KEEPER_ROBERT_2, TRAINER_BIRD_KEEPER_ROBERT_3),
   VS_SEEKER_DATA(TRAINER_PICNICKER_ALMA, TRAINER_PICNICKER_ALMA),
   VS_SEEKER_DATA(TRAINER_PICNICKER_SUSIE, 0xFFFF, 0xFFFF, TRAINER_PICNICKER_SUSIE_2, TRAINER_PICNICKER_SUSIE_3, TRAINER_PICNICKER_SUSIE_4),
   VS_SEEKER_DATA(TRAINER_PICNICKER_VALERIE, TRAINER_PICNICKER_VALERIE),
   VS_SEEKER_DATA(TRAINER_PICNICKER_GWEN, TRAINER_PICNICKER_GWEN),
   VS_SEEKER_DATA(TRAINER_BIKER_MALIK, TRAINER_BIKER_MALIK),
   VS_SEEKER_DATA(TRAINER_BIKER_LUKAS, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_BIKER_LUKAS_2),
   VS_SEEKER_DATA(TRAINER_BIKER_ISAAC, TRAINER_BIKER_ISAAC),
   VS_SEEKER_DATA(TRAINER_BIKER_GERALD, TRAINER_BIKER_GERALD),
   VS_SEEKER_DATA(TRAINER_BIRD_KEEPER_DONALD, TRAINER_BIRD_KEEPER_DONALD),
   VS_SEEKER_DATA(TRAINER_BIRD_KEEPER_BENNY, 0xFFFF, 0xFFFF, TRAINER_BIRD_KEEPER_BENNY_2, TRAINER_BIRD_KEEPER_BENNY_3),
   VS_SEEKER_DATA(TRAINER_BIRD_KEEPER_CARTER, TRAINER_BIRD_KEEPER_CARTER),
   VS_SEEKER_DATA(TRAINER_BIRD_KEEPER_MITCH, TRAINER_BIRD_KEEPER_MITCH),
   VS_SEEKER_DATA(TRAINER_BIRD_KEEPER_BECK, TRAINER_BIRD_KEEPER_BECK),
   VS_SEEKER_DATA(TRAINER_BIRD_KEEPER_MARLON, 0xFFFF, 0xFFFF, TRAINER_BIRD_KEEPER_MARLON_2, TRAINER_BIRD_KEEPER_MARLON_3),
   VS_SEEKER_DATA(TRAINER_TWINS_KIRI_JAN, TRAINER_TWINS_KIRI_JAN),
   VS_SEEKER_DATA(TRAINER_BIKER_ERNEST, TRAINER_BIKER_ERNEST),
   VS_SEEKER_DATA(TRAINER_BIKER_ALEX, TRAINER_BIKER_ALEX),
   VS_SEEKER_DATA(TRAINER_BEAUTY_GRACE, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_BEAUTY_GRACE_2),
   VS_SEEKER_DATA(TRAINER_BEAUTY_OLIVIA, TRAINER_BEAUTY_OLIVIA),
   VS_SEEKER_DATA(TRAINER_BIRD_KEEPER_EDWIN, TRAINER_BIRD_KEEPER_EDWIN),
   VS_SEEKER_DATA(TRAINER_BIRD_KEEPER_CHESTER, 0xFFFF, 0xFFFF, TRAINER_BIRD_KEEPER_CHESTER_2, TRAINER_BIRD_KEEPER_CHESTER_3),
   VS_SEEKER_DATA(TRAINER_PICNICKER_YAZMIN, TRAINER_PICNICKER_YAZMIN),
   VS_SEEKER_DATA(TRAINER_PICNICKER_KINDRA, TRAINER_PICNICKER_KINDRA),
   VS_SEEKER_DATA(TRAINER_PICNICKER_BECKY, 0xFFFF, 0xFFFF, TRAINER_PICNICKER_BECKY_2, TRAINER_PICNICKER_BECKY_3, TRAINER_PICNICKER_BECKY_4),
   VS_SEEKER_DATA(TRAINER_PICNICKER_CELIA, TRAINER_PICNICKER_CELIA),
   VS_SEEKER_DATA(TRAINER_CRUSH_KIN_RON_MYA, 0xFFFF, 0xFFFF, TRAINER_CRUSH_KIN_RON_MYA_2, TRAINER_CRUSH_KIN_RON_MYA_3, TRAINER_CRUSH_KIN_RON_MYA_4),
   VS_SEEKER_DATA(TRAINER_BIKER_LAO, TRAINER_BIKER_LAO),
   VS_SEEKER_DATA(TRAINER_BIKER_HIDEO, TRAINER_BIKER_HIDEO),
   VS_SEEKER_DATA(TRAINER_BIKER_RUBEN, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_BIKER_RUBEN_2),
   VS_SEEKER_DATA(TRAINER_CUE_BALL_KOJI, TRAINER_CUE_BALL_KOJI),
   VS_SEEKER_DATA(TRAINER_CUE_BALL_LUKE, TRAINER_CUE_BALL_LUKE),
   VS_SEEKER_DATA(TRAINER_CUE_BALL_CAMRON, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_CUE_BALL_CAMRON_2),
   VS_SEEKER_DATA(TRAINER_YOUNG_COUPLE_LEA_JED, TRAINER_YOUNG_COUPLE_LEA_JED),
   VS_SEEKER_DATA(TRAINER_BIKER_BILLY, TRAINER_BIKER_BILLY),
   VS_SEEKER_DATA(TRAINER_BIKER_NIKOLAS, TRAINER_BIKER_NIKOLAS),
   VS_SEEKER_DATA(TRAINER_BIKER_JAXON, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_BIKER_JAXON_2),
   VS_SEEKER_DATA(TRAINER_BIKER_WILLIAM, TRAINER_BIKER_WILLIAM),
   VS_SEEKER_DATA(TRAINER_CUE_BALL_RAUL, TRAINER_CUE_BALL_RAUL),
   VS_SEEKER_DATA(TRAINER_CUE_BALL_ISAIAH, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_CUE_BALL_ISAIAH_2),
   VS_SEEKER_DATA(TRAINER_CUE_BALL_ZEEK, TRAINER_CUE_BALL_ZEEK),
   VS_SEEKER_DATA(TRAINER_CUE_BALL_JAMAL, TRAINER_CUE_BALL_JAMAL),
   VS_SEEKER_DATA(TRAINER_CUE_BALL_COREY, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_CUE_BALL_COREY_2),
   VS_SEEKER_DATA(TRAINER_BIKER_VIRGIL, TRAINER_BIKER_VIRGIL),
   VS_SEEKER_DATA(TRAINER_BIRD_KEEPER_WILTON, TRAINER_BIRD_KEEPER_WILTON),
   VS_SEEKER_DATA(TRAINER_BIRD_KEEPER_RAMIRO, TRAINER_BIRD_KEEPER_RAMIRO),
   VS_SEEKER_DATA(TRAINER_BIRD_KEEPER_JACOB, 0xFFFF, 0xFFFF, TRAINER_BIRD_KEEPER_JACOB_2, TRAINER_BIRD_KEEPER_JACOB_3),
   VS_SEEKER_DATA(TRAINER_SWIMMER_MALE_RICHARD, TRAINER_SWIMMER_MALE_RICHARD),
   VS_SEEKER_DATA(TRAINER_SWIMMER_MALE_REECE, TRAINER_SWIMMER_MALE_REECE),
   VS_SEEKER_DATA(TRAINER_SWIMMER_MALE_MATTHEW, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_SWIMMER_MALE_MATTHEW_2),
   VS_SEEKER_DATA(TRAINER_SWIMMER_MALE_DOUGLAS, TRAINER_SWIMMER_MALE_DOUGLAS),
   VS_SEEKER_DATA(TRAINER_SWIMMER_MALE_DAVID, TRAINER_SWIMMER_MALE_DAVID),
   VS_SEEKER_DATA(TRAINER_SWIMMER_MALE_TONY, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_SWIMMER_MALE_TONY_2),
   VS_SEEKER_DATA(TRAINER_SWIMMER_MALE_AXLE, TRAINER_SWIMMER_MALE_AXLE),
   VS_SEEKER_DATA(TRAINER_SWIMMER_FEMALE_ANYA, TRAINER_SWIMMER_FEMALE_ANYA),
   VS_SEEKER_DATA(TRAINER_SWIMMER_FEMALE_ALICE, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_SWIMMER_FEMALE_ALICE_2),
   VS_SEEKER_DATA(TRAINER_SWIMMER_FEMALE_CONNIE, TRAINER_SWIMMER_FEMALE_CONNIE),
   VS_SEEKER_DATA(TRAINER_SIS_AND_BRO_LIA_LUC, TRAINER_SIS_AND_BRO_LIA_LUC),
   VS_SEEKER_DATA(TRAINER_SWIMMER_MALE_BARRY, TRAINER_SWIMMER_MALE_BARRY),
   VS_SEEKER_DATA(TRAINER_SWIMMER_MALE_DEAN, TRAINER_SWIMMER_MALE_DEAN),
   VS_SEEKER_DATA(TRAINER_SWIMMER_MALE_DARRIN, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_SWIMMER_MALE_DARRIN_2),
   VS_SEEKER_DATA(TRAINER_SWIMMER_FEMALE_TIFFANY, TRAINER_SWIMMER_FEMALE_TIFFANY),
   VS_SEEKER_DATA(TRAINER_SWIMMER_FEMALE_NORA, TRAINER_SWIMMER_FEMALE_NORA),
   VS_SEEKER_DATA(TRAINER_SWIMMER_FEMALE_MELISSA, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_SWIMMER_FEMALE_MELISSA_2),
   VS_SEEKER_DATA(TRAINER_SWIMMER_FEMALE_SHIRLEY, TRAINER_SWIMMER_FEMALE_SHIRLEY),
   VS_SEEKER_DATA(TRAINER_BIRD_KEEPER_ROGER, TRAINER_BIRD_KEEPER_ROGER),
   VS_SEEKER_DATA(TRAINER_PICNICKER_MISSY, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_PICNICKER_MISSY_2, TRAINER_PICNICKER_MISSY_3),
   VS_SEEKER_DATA(TRAINER_PICNICKER_IRENE, TRAINER_PICNICKER_IRENE),
   VS_SEEKER_DATA(TRAINER_FISHERMAN_RONALD, TRAINER_FISHERMAN_RONALD),
   VS_SEEKER_DATA(TRAINER_FISHERMAN_CLAUDE, TRAINER_FISHERMAN_CLAUDE),
   VS_SEEKER_DATA(TRAINER_FISHERMAN_WADE, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_FISHERMAN_WADE_2),
   VS_SEEKER_DATA(TRAINER_FISHERMAN_NOLAN, TRAINER_FISHERMAN_NOLAN),
   VS_SEEKER_DATA(TRAINER_SWIMMER_MALE_SPENCER, TRAINER_SWIMMER_MALE_SPENCER),
   VS_SEEKER_DATA(TRAINER_SWIMMER_MALE_JACK, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_SWIMMER_MALE_JACK_2),
   VS_SEEKER_DATA(TRAINER_SWIMMER_MALE_JEROME, TRAINER_SWIMMER_MALE_JEROME),
   VS_SEEKER_DATA(TRAINER_SWIMMER_MALE_ROLAND, TRAINER_SWIMMER_MALE_ROLAND),
   VS_SEEKER_DATA(TRAINER_SIS_AND_BRO_LIL_IAN, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_SIS_AND_BRO_LIL_IAN_2, TRAINER_SIS_AND_BRO_LIL_IAN_3),
   VS_SEEKER_DATA(TRAINER_SWIMMER_FEMALE_MARIA, TRAINER_SWIMMER_FEMALE_MARIA),
   VS_SEEKER_DATA(TRAINER_SWIMMER_FEMALE_ABIGAIL, TRAINER_SWIMMER_FEMALE_ABIGAIL),
   VS_SEEKER_DATA(TRAINER_SWIMMER_MALE_FINN, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_SWIMMER_MALE_FINN_2),
   VS_SEEKER_DATA(TRAINER_SWIMMER_MALE_GARRETT, TRAINER_SWIMMER_MALE_GARRETT),
   VS_SEEKER_DATA(TRAINER_FISHERMAN_TOMMY, TRAINER_FISHERMAN_TOMMY),
   VS_SEEKER_DATA(TRAINER_CRUSH_GIRL_SHARON, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_CRUSH_GIRL_SHARON_2, TRAINER_CRUSH_GIRL_SHARON_3),
   VS_SEEKER_DATA(TRAINER_CRUSH_GIRL_TANYA, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_CRUSH_GIRL_TANYA_2, TRAINER_CRUSH_GIRL_TANYA_3),
   VS_SEEKER_DATA(TRAINER_BLACK_BELT_SHEA, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_BLACK_BELT_SHEA_2, TRAINER_BLACK_BELT_SHEA_3),
   VS_SEEKER_DATA(TRAINER_BLACK_BELT_HUGH, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_BLACK_BELT_HUGH_2, TRAINER_BLACK_BELT_HUGH_3),
   VS_SEEKER_DATA(TRAINER_CAMPER_BRYCE, TRAINER_CAMPER_BRYCE),
   VS_SEEKER_DATA(TRAINER_PICNICKER_CLAIRE, TRAINER_PICNICKER_CLAIRE),
   VS_SEEKER_DATA(TRAINER_CRUSH_KIN_MIK_KIA, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_CRUSH_KIN_MIK_KIA_2, TRAINER_CRUSH_KIN_MIK_KIA_3),
   VS_SEEKER_DATA(TRAINER_SWIMMER_FEMALE_AMARA, TRAINER_SWIMMER_FEMALE_AMARA),
   VS_SEEKER_DATA(TRAINER_AROMA_LADY_NIKKI, TRAINER_AROMA_LADY_NIKKI),
   VS_SEEKER_DATA(TRAINER_AROMA_LADY_VIOLET, TRAINER_AROMA_LADY_VIOLET),
   VS_SEEKER_DATA(TRAINER_TUBER_AMIRA, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_TUBER_AMIRA_2),
   VS_SEEKER_DATA(TRAINER_TUBER_ALEXIS, TRAINER_TUBER_ALEXIS),
   VS_SEEKER_DATA(TRAINER_SWIMMER_FEMALE_TISHA, TRAINER_SWIMMER_FEMALE_TISHA),
   VS_SEEKER_DATA(TRAINER_TWINS_JOY_MEG, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_TWINS_JOY_MEG_2),
   VS_SEEKER_DATA(TRAINER_PAINTER_DAISY, TRAINER_PAINTER_DAISY),
   VS_SEEKER_DATA(TRAINER_PAINTER_CELINA, TRAINER_PAINTER_CELINA),
   VS_SEEKER_DATA(TRAINER_PAINTER_RAYNA, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_PAINTER_RAYNA_2),
   VS_SEEKER_DATA(TRAINER_LADY_JACKI, TRAINER_LADY_JACKI),
   VS_SEEKER_DATA(TRAINER_LADY_GILLIAN, TRAINER_LADY_GILLIAN),
   VS_SEEKER_DATA(TRAINER_YOUNGSTER_DESTIN, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_YOUNGSTER_DESTIN_2),
   VS_SEEKER_DATA(TRAINER_SWIMMER_MALE_TOBY, TRAINER_SWIMMER_MALE_TOBY),
   VS_SEEKER_DATA(TRAINER_PKMN_BREEDER_ALIZE, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_PKMN_BREEDER_ALIZE_2),
   VS_SEEKER_DATA(TRAINER_BIRD_KEEPER_MILO, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_BIRD_KEEPER_MILO_2),
   VS_SEEKER_DATA(TRAINER_BIRD_KEEPER_CHAZ, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_BIRD_KEEPER_CHAZ_2),
   VS_SEEKER_DATA(TRAINER_BIRD_KEEPER_HAROLD, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_BIRD_KEEPER_HAROLD_2),
   VS_SEEKER_DATA(TRAINER_FISHERMAN_TYLOR, TRAINER_FISHERMAN_TYLOR),
   VS_SEEKER_DATA(TRAINER_SWIMMER_MALE_MYMO, TRAINER_SWIMMER_MALE_MYMO),
   VS_SEEKER_DATA(TRAINER_SWIMMER_FEMALE_NICOLE, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_SWIMMER_FEMALE_NICOLE_2),
   VS_SEEKER_DATA(TRAINER_SIS_AND_BRO_AVA_GEB, TRAINER_SIS_AND_BRO_AVA_GEB),
   VS_SEEKER_DATA(TRAINER_PSYCHIC_JACLYN, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_PSYCHIC_JACLYN_2),
   VS_SEEKER_DATA(TRAINER_AROMA_LADY_ROSE, TRAINER_AROMA_LADY_ROSE),
   VS_SEEKER_DATA(TRAINER_JUGGLER_EDWARD, TRAINER_JUGGLER_EDWARD),
   VS_SEEKER_DATA(TRAINER_SWIMMER_MALE_SAMIR, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_SWIMMER_MALE_SAMIR_2),
   VS_SEEKER_DATA(TRAINER_SWIMMER_FEMALE_DENISE, TRAINER_SWIMMER_FEMALE_DENISE),
   VS_SEEKER_DATA(TRAINER_TWINS_MIU_MIA, TRAINER_TWINS_MIU_MIA),
   VS_SEEKER_DATA(TRAINER_HIKER_EARL, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_HIKER_EARL_2),
   VS_SEEKER_DATA(TRAINER_RUIN_MANIAC_STANLY, TRAINER_RUIN_MANIAC_STANLY),
   VS_SEEKER_DATA(TRAINER_RUIN_MANIAC_FOSTER, TRAINER_RUIN_MANIAC_FOSTER),
   VS_SEEKER_DATA(TRAINER_RUIN_MANIAC_LARRY, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_RUIN_MANIAC_LARRY_2),
   VS_SEEKER_DATA(TRAINER_HIKER_DARYL, TRAINER_HIKER_DARYL),
   VS_SEEKER_DATA(TRAINER_POKEMANIAC_HECTOR, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_POKEMANIAC_HECTOR_2),
   VS_SEEKER_DATA(TRAINER_PSYCHIC_DARIO, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_PSYCHIC_DARIO_2),
   VS_SEEKER_DATA(TRAINER_PSYCHIC_RODETTE, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_PSYCHIC_RODETTE_2),
   VS_SEEKER_DATA(TRAINER_AROMA_LADY_MIAH, TRAINER_AROMA_LADY_MIAH),
   VS_SEEKER_DATA(TRAINER_YOUNG_COUPLE_EVE_JON, TRAINER_YOUNG_COUPLE_EVE_JON),
   VS_SEEKER_DATA(TRAINER_JUGGLER_MASON, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_JUGGLER_MASON_2),
   VS_SEEKER_DATA(TRAINER_PKMN_RANGER_NICOLAS, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_PKMN_RANGER_NICOLAS_2),
   VS_SEEKER_DATA(TRAINER_PKMN_RANGER_MADELINE, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_PKMN_RANGER_MADELINE_2),
   VS_SEEKER_DATA(TRAINER_CRUSH_GIRL_CYNDY, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_CRUSH_GIRL_CYNDY_2),
   VS_SEEKER_DATA(TRAINER_TAMER_EVAN, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_TAMER_EVAN_2),
   VS_SEEKER_DATA(TRAINER_PKMN_RANGER_JACKSON, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_PKMN_RANGER_JACKSON_2),
   VS_SEEKER_DATA(TRAINER_PKMN_RANGER_KATELYN, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_PKMN_RANGER_KATELYN_2),
   VS_SEEKER_DATA(TRAINER_COOLTRAINER_LEROY, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_COOLTRAINER_LEROY_2),
   VS_SEEKER_DATA(TRAINER_COOLTRAINER_MICHELLE, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_COOLTRAINER_MICHELLE_2),
   VS_SEEKER_DATA(TRAINER_COOL_COUPLE_LEX_NYA, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, TRAINER_COOL_COUPLE_LEX_NYA_2),
   VS_SEEKER_DATA(TRAINER_RUIN_MANIAC_BRANDON, TRAINER_RUIN_MANIAC_BRANDON),
   VS_SEEKER_DATA(TRAINER_RUIN_MANIAC_BENJAMIN, TRAINER_RUIN_MANIAC_BENJAMIN),
   VS_SEEKER_DATA(TRAINER_PAINTER_EDNA, TRAINER_PAINTER_EDNA),
   VS_SEEKER_DATA(TRAINER_GENTLEMAN_CLIFFORD, TRAINER_GENTLEMAN_CLIFFORD),
};

static const u8 sMovementScript_Wait48[] = {
    MOVEMENT_ACTION_DELAY_16,
    MOVEMENT_ACTION_DELAY_16,
    MOVEMENT_ACTION_DELAY_16,
    MOVEMENT_ACTION_STEP_END
};

static const u8 sMovementScript_TrainerUnfought[] = {
    MOVEMENT_ACTION_EMOTE_EXCLAMATION_MARK,
    MOVEMENT_ACTION_STEP_END
};

static const u8 sMovementScript_TrainerNoRematch[] = {
    MOVEMENT_ACTION_EMOTE_X,
    MOVEMENT_ACTION_STEP_END
};

static const u8 sMovementScript_TrainerRematch[] = {
    MOVEMENT_ACTION_WALK_IN_PLACE_FASTEST_DOWN,
    MOVEMENT_ACTION_EMOTE_DOUBLE_EXCL_MARK,
    MOVEMENT_ACTION_STEP_END
};

static const u8 sFaceDirectionMovementTypeByFacingDirection[] = {
    MOVEMENT_TYPE_FACE_DOWN,
    MOVEMENT_TYPE_FACE_DOWN,
    MOVEMENT_TYPE_FACE_UP,
    MOVEMENT_TYPE_FACE_LEFT,
    MOVEMENT_TYPE_FACE_RIGHT
};

// text
void VsSeekerFreezeObjectsAfterChargeComplete(void)
{
    CreateTask(Task_ResetObjectsRematchWantedState, 80);
}

static void Task_ResetObjectsRematchWantedState(u8 taskId)
{
    struct Task * task = &gTasks[taskId];
    u8 i;

    if (task->data[0] == 0 && walkrun_is_standing_still())
    {
        HandleEnforcedLookDirectionOnPlayerStopMoving();
        task->data[0] = 1;
    }

    if (task->data[1] == 0)
    {
        for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
        {
            if (ObjectEventIdIsSane(i))
            {
                if (gObjectEvents[i].singleMovementActive)
                    return;
				
                FreezeObjectEvent(&gObjectEvents[i]);
            }
        }
    }
    task->data[1] = 1;
	
    if (task->data[0] != 0)
    {
        DestroyTask(taskId);
        StopPlayerAvatar();
        EnableBothScriptContexts();
    }
}

void VsSeekerResetObjectMovementAfterChargeComplete(void)
{
   u8 i;
   u8 movementType;
   u8 objEventId;
   struct ObjectEventTemplate * templates = gSaveBlock1Ptr->objectEventTemplates;

   for (i = 0; i < gMapHeader.events->objectEventCount; i++)
   {
      if ((templates[i].objUnion.normal.trainerType == TRAINER_TYPE_NORMAL || templates[i].objUnion.normal.trainerType == TRAINER_TYPE_BURIED)
      && (templates[i].objUnion.normal.movementType == MOVEMENT_TYPE_VS_SEEKER_4D || templates[i].objUnion.normal.movementType == MOVEMENT_TYPE_VS_SEEKER_4E
      || templates[i].objUnion.normal.movementType == MOVEMENT_TYPE_VS_SEEKER_4F))
      {
         movementType = GetRandomFaceDirectionMovementType();
         TryGetObjectEventIdByLocalIdAndMap(templates[i].localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, &objEventId);
         
         if (ObjectEventIdIsSane(objEventId))
            SetTrainerMovementType(&gObjectEvents[objEventId], movementType);
            
         templates[i].objUnion.normal.movementType = movementType;
      }
   }
}

bool8 UpdateVsSeekerStepCounter(void)
{
    u8 x = 0;

    if (CheckBagHasItem(ITEM_VS_SEEKER, 1))
    {
        if ((gSaveBlock1Ptr->trainerRematchStepCounter & 0xFF) < 100)
            gSaveBlock1Ptr->trainerRematchStepCounter++;
    }

    if (FlagGet(FLAG_SYS_VS_SEEKER_CHARGING))
    {
        if (((gSaveBlock1Ptr->trainerRematchStepCounter >> 8) & 0xFF) < 100)
        {
            x = (((gSaveBlock1Ptr->trainerRematchStepCounter >> 8) & 0xFF) + 1);
            gSaveBlock1Ptr->trainerRematchStepCounter = (gSaveBlock1Ptr->trainerRematchStepCounter & 0xFF) | (x << 8);
        }
        if (((gSaveBlock1Ptr->trainerRematchStepCounter >> 8) & 0xFF) == 100)
        {
            FlagClear(FLAG_SYS_VS_SEEKER_CHARGING);
            VsSeekerResetChargingStepCounter();
            ClearAllTrainerRematchStates();
            return TRUE;
        }
    }
    return FALSE;
}

void MapResetTrainerRematches(void)
{
    FlagClear(FLAG_SYS_VS_SEEKER_CHARGING);
    VsSeekerResetChargingStepCounter();
    ClearAllTrainerRematchStates();
    ResetMovementOfRematchableTrainers();
}

static void ResetMovementOfRematchableTrainers(void)
{
    u8 i;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        struct ObjectEvent * objectEvent = &gObjectEvents[i];
		
        if (objectEvent->movementType == MOVEMENT_TYPE_VS_SEEKER_4D || objectEvent->movementType == MOVEMENT_TYPE_VS_SEEKER_4E || objectEvent->movementType == MOVEMENT_TYPE_VS_SEEKER_4F)
        {
            if (objectEvent->active && gSprites[objectEvent->spriteId].data[0] == i)
            {
                gSprites[objectEvent->spriteId].x2 = 0;
                gSprites[objectEvent->spriteId].y2 = 0;
                SetTrainerMovementType(objectEvent, GetRandomFaceDirectionMovementType());
            }
        }
    }
}

static void VsSeekerResetInBagStepCounter(void)
{
    gSaveBlock1Ptr->trainerRematchStepCounter &= 0xFF00;
}

static void VsSeekerSetStepCounterInBagFull(void)
{
    gSaveBlock1Ptr->trainerRematchStepCounter &= 0xFF00;
    gSaveBlock1Ptr->trainerRematchStepCounter |= 100;
}

static void VsSeekerResetChargingStepCounter(void)
{
    gSaveBlock1Ptr->trainerRematchStepCounter &= 0x00FF;
}

static void VsSeekerSetStepCounterFullyCharged(void)
{
    gSaveBlock1Ptr->trainerRematchStepCounter &= 0x00FF;
    gSaveBlock1Ptr->trainerRematchStepCounter |= (100 << 8);
}

void Task_VsSeeker_0(u8 taskId)
{
   u8 i;

   for (i = 0; i < 16; i++)
      gTasks[taskId].data[i] = 0;

   sVsSeeker = AllocZeroed(sizeof(struct VsSeekerStruct));
   GatherNearbyTrainerInfo();
	
	switch (CanUseVsSeeker())
	{
		case VSSEEKER_NOT_CHARGED:
			Free(sVsSeeker);
			DisplayItemMessageOnField(taskId, 2, VSSeeker_Text_BatteryNotChargedNeedXSteps, Task_ItemUse_CloseMessageBoxAndReturnToField);
			break;
		case VSSEEKER_NO_ONE_IN_RANGE:
			Free(sVsSeeker);
			DisplayItemMessageOnField(taskId, 2, VSSeeker_Text_NoTrainersWithinRange, Task_ItemUse_CloseMessageBoxAndReturnToField);
			break;
		case VSSEEKER_CAN_USE:
			if (!TryDismountPokeRide())
			{
				Free(sVsSeeker);
				DisplayItemMessageOnField(taskId, 2, gText_OakForbidsUseOfItemHere, Task_ItemUse_CloseMessageBoxAndReturnToField);
			}
			else
			{
				FieldEffectStart(FLDEFF_USE_VS_SEEKER);
				gTasks[taskId].func = Task_VsSeeker_1;
				gTasks[taskId].data[0] = 15;
			}
			break;
	}
}

static void Task_VsSeeker_1(u8 taskId)
{
    if (--gTasks[taskId].data[0] == 0)
    {
        gTasks[taskId].func = Task_VsSeeker_2;
        gTasks[taskId].data[1] = 16;
    }
}

static void Task_VsSeeker_2(u8 taskId)
{
    s16 * data = gTasks[taskId].data;

    if (data[2] != 2 && --data[1] == 0)
    {
        PlaySE(SE_CONTEST_MONS_TURN);
        data[1] = 11;
        data[2]++;
    }

    if (!FieldEffectActiveListContains(FLDEFF_USE_VS_SEEKER))
    {
        data[1] = 0;
        data[2] = 0;
        VsSeekerResetInBagStepCounter();
        sVsSeeker->responseCode = GetVsSeekerResponseInArea(sVsSeekerData);
        ScriptMovement_StartObjectMovementScript(0xFF, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, sMovementScript_Wait48);
        gTasks[taskId].func = Task_VsSeeker_3;
    }
}

static void GatherNearbyTrainerInfo(void)
{
    struct ObjectEventTemplate *templates = gSaveBlock1Ptr->objectEventTemplates;
    u8 objectEventId = 0;
    u8 vsSeekerObjectIdx = 0;
    s32 objectEventIdx;

    for (objectEventIdx = 0; objectEventIdx < gMapHeader.events->objectEventCount; objectEventIdx++)
    {
        if (templates[objectEventIdx].objUnion.normal.trainerType == TRAINER_TYPE_NORMAL || templates[objectEventIdx].objUnion.normal.trainerType == TRAINER_TYPE_BURIED)
        {
            sVsSeeker->trainerInfo[vsSeekerObjectIdx].script = templates[objectEventIdx].script;
            sVsSeeker->trainerInfo[vsSeekerObjectIdx].trainerIdx = GetTrainerFlagFromScript(templates[objectEventIdx].script);
            sVsSeeker->trainerInfo[vsSeekerObjectIdx].localId = templates[objectEventIdx].localId;
            TryGetObjectEventIdByLocalIdAndMap(templates[objectEventIdx].localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, &objectEventId);
            sVsSeeker->trainerInfo[vsSeekerObjectIdx].objectEventId = objectEventId;
            sVsSeeker->trainerInfo[vsSeekerObjectIdx].xCoord = gObjectEvents[objectEventId].currentCoords.x - 7;
            sVsSeeker->trainerInfo[vsSeekerObjectIdx].yCoord = gObjectEvents[objectEventId].currentCoords.y - 7;
            sVsSeeker->trainerInfo[vsSeekerObjectIdx].graphicsId = templates[objectEventIdx].graphicsId;
            vsSeekerObjectIdx++;
        }
    }
    sVsSeeker->trainerInfo[vsSeekerObjectIdx].localId = 0xFF;
}

static void Task_VsSeeker_3(u8 taskId)
{
    if (ScriptMovement_IsObjectMovementFinished(0xFF, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup))
    {
        if (sVsSeeker->responseCode == VSSEEKER_RESPONSE_NO_RESPONSE)
            DisplayItemMessageOnField(taskId, 2, VSSeeker_Text_TrainersNotReady, Task_ItemUse_CloseMessageBoxAndReturnToField);
        else
        {
            if (sVsSeeker->responseCode == VSSEEKER_RESPONSE_FOUND_REMATCHES)
                StartAllRespondantIdleMovements();
			
            ClearDialogWindowAndFrame(0, TRUE);
            ClearPlayerHeldMovementAndUnfreezeObjectEvents();
            ScriptContext2_Disable();
            DestroyTask(taskId);
        }
        Free(sVsSeeker);
    }
}

static u8 CanUseVsSeeker(void)
{
    u8 vsSeekerChargeSteps = gSaveBlock1Ptr->trainerRematchStepCounter;
	
    if (vsSeekerChargeSteps == 100)
    {
        if (GetRematchableTrainerLocalId() == 0xFF)
            return VSSEEKER_NO_ONE_IN_RANGE;
        else
            return VSSEEKER_CAN_USE;
    }
    else
    {
        TV_PrintIntToStringVar(0, 100 - vsSeekerChargeSteps);
        return VSSEEKER_NOT_CHARGED;
    }
}

static u8 GetVsSeekerResponseInArea(const VsSeekerData * vsSeekerData)
{
    u16 trainerIdx = 0;
    u16 rval = 0;
    u8 rematchTrainerIdx;
    u8 unusedIdx = 0;
    u8 response = 0;
    s32 vsSeekerIdx = 0;

    while (sVsSeeker->trainerInfo[vsSeekerIdx].localId != 0xFF)
    {
        if (IsTrainerVisibleOnScreen(&sVsSeeker->trainerInfo[vsSeekerIdx]) == TRUE)
        {
            trainerIdx = sVsSeeker->trainerInfo[vsSeekerIdx].trainerIdx;
			
            if (!HasTrainerBeenFought(trainerIdx))
            {
                StartTrainerObjectMovementScript(&sVsSeeker->trainerInfo[vsSeekerIdx], sMovementScript_TrainerUnfought);
                sVsSeeker->trainerHasNotYetBeenFought = 1;
                vsSeekerIdx++;
                continue;
            }
            rematchTrainerIdx = GetNextAvailableRematchTrainer(vsSeekerData, trainerIdx, &unusedIdx);
			
            if (rematchTrainerIdx == 0)
            {
                StartTrainerObjectMovementScript(&sVsSeeker->trainerInfo[vsSeekerIdx], sMovementScript_TrainerNoRematch);
                sVsSeeker->trainerDoesNotWantRematch = 1;
            }
            else
            {
                rval = RandomMax(100); // Even if it's overwritten below, it progresses the RNG.
                response = GetCurVsSeekerResponse(vsSeekerIdx, trainerIdx);
				
                if (response == VSSEEKER_SINGLE_RESP_YES)
                    rval = 100; // Definitely yes
                else if (response == VSSEEKER_SINGLE_RESP_NO)
                    rval = 0; // Definitely no
				
                // Otherwise it's a 70% chance to want a rematch
                if (rval < 30)
                {
                    StartTrainerObjectMovementScript(&sVsSeeker->trainerInfo[vsSeekerIdx], sMovementScript_TrainerNoRematch);
                    sVsSeeker->trainerDoesNotWantRematch = 1;
                }
                else
                {
                    gSaveBlock1Ptr->trainerRematches[sVsSeeker->trainerInfo[vsSeekerIdx].localId] = rematchTrainerIdx;
                    ShiftStillObjectEventCoords(&gObjectEvents[sVsSeeker->trainerInfo[vsSeekerIdx].objectEventId]);
                    StartTrainerObjectMovementScript(&sVsSeeker->trainerInfo[vsSeekerIdx], sMovementScript_TrainerRematch);
                    sVsSeeker->trainerIdxArray[sVsSeeker->numRematchableTrainers] = trainerIdx;
                    sVsSeeker->runningBehaviourEtcArray[sVsSeeker->numRematchableTrainers] = GetRunningBehaviorFromGraphicsId(sVsSeeker->trainerInfo[vsSeekerIdx].graphicsId);
                    sVsSeeker->numRematchableTrainers++;
                    sVsSeeker->trainerWantsRematch = 1;
                }
            }
        }
        vsSeekerIdx++;
    }

    if (sVsSeeker->trainerWantsRematch)
    {
        PlaySE(SE_PIN);
        FlagSet(FLAG_SYS_VS_SEEKER_CHARGING);
        VsSeekerResetChargingStepCounter();
        return VSSEEKER_RESPONSE_FOUND_REMATCHES;
    }
    if (sVsSeeker->trainerHasNotYetBeenFought)
        return VSSEEKER_RESPONSE_UNFOUGHT_TRAINERS;
	
    return VSSEEKER_RESPONSE_NO_RESPONSE;
}

void ClearRematchStateByTrainerId(void)
{
    u8 objEventId = 0;
    struct ObjectEventTemplate *objectEventTemplates = gSaveBlock1Ptr->objectEventTemplates;
    int vsSeekerDataIdx = LookupVsSeekerOpponentInArray(sVsSeekerData, gTrainerBattleOpponent_A);

    if (vsSeekerDataIdx != -1)
    {
        int i;

        for (i = 0; i < gMapHeader.events->objectEventCount; i++)
        {
            if ((objectEventTemplates[i].objUnion.normal.trainerType == TRAINER_TYPE_NORMAL || objectEventTemplates[i].objUnion.normal.trainerType == TRAINER_TYPE_BURIED)
			&& vsSeekerDataIdx == LookupVsSeekerOpponentInArray(sVsSeekerData, GetTrainerFlagFromScript(objectEventTemplates[i].script)))
            {
                struct ObjectEvent *objectEvent;

                TryGetObjectEventIdByLocalIdAndMap(objectEventTemplates[i].localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, &objEventId);
                objectEvent = &gObjectEvents[objEventId];
                GetRandomFaceDirectionMovementType();
                OverrideMovementTypeForObjectEvent(objectEvent, sFaceDirectionMovementTypeByFacingDirection[objectEvent->facingDirection]);
                gSaveBlock1Ptr->trainerRematches[objectEventTemplates[i].localId] = 0;
                
				if (gSelectedObjectEvent == objEventId)
                    objectEvent->movementType = sFaceDirectionMovementTypeByFacingDirection[objectEvent->facingDirection];
                else
                    objectEvent->movementType = MOVEMENT_TYPE_FACE_DOWN;
            }
        }
    }
}

static void TryGetRematchTrainerIdGivenGameState(const u16 * trainerIdxs, u8 * rematchIdx_p)
{
    switch (*rematchIdx_p)
    {
        case 0:
            break;
        case 1:
            if (!FlagGet(FLAG_GOT_VS_SEEKER))
                *rematchIdx_p = GetRematchTrainerIdGivenGameState(trainerIdxs, *rematchIdx_p);
            break;
        case 2:
            if (!FlagGet(FLAG_WORLD_MAP_CELADON_CITY))
                *rematchIdx_p = GetRematchTrainerIdGivenGameState(trainerIdxs, *rematchIdx_p);
            break;
        case 3:
            if (!FlagGet(FLAG_WORLD_MAP_FUCHSIA_CITY))
                *rematchIdx_p = GetRematchTrainerIdGivenGameState(trainerIdxs, *rematchIdx_p);
            break;
        case 4:
            if (!FlagGet(FLAG_SYS_GAME_CLEAR))
                *rematchIdx_p = GetRematchTrainerIdGivenGameState(trainerIdxs, *rematchIdx_p);
            break;
        case 5:
            if (!FlagGet(FLAG_SYS_CAN_LINK_WITH_RS))
                *rematchIdx_p = GetRematchTrainerIdGivenGameState(trainerIdxs, *rematchIdx_p);
            break;
    }
}

static u8 GetRematchTrainerIdGivenGameState(const u16 *trainerIdxs, u8 rematchIdx)
{
    while (--rematchIdx != 0)
    {
        const u16 *rematch_p = trainerIdxs + rematchIdx;
        if (*rematch_p != 0xFFFF)
            return rematchIdx;
    }
    return 0;
}

bool8 ShouldTryRematchBattle(void)
{
    if (ShouldTryRematchBattleInternal(sVsSeekerData, gTrainerBattleOpponent_A))
        return TRUE;

    return HasRematchTrainerAlreadyBeenFought(sVsSeekerData, gTrainerBattleOpponent_A);
}

static bool8 ShouldTryRematchBattleInternal(const VsSeekerData *vsSeekerData, u16 trainerBattleOpponent)
{
    s32 rematchIdx = GetRematchIdx(vsSeekerData, trainerBattleOpponent);

    if (rematchIdx == -1)
        return FALSE;
	
    if (rematchIdx >= 0 && rematchIdx < ARRAY_COUNT(sVsSeekerData))
    {
        if (IsThisTrainerRematchable(gSpecialVar_LastTalked))
            return TRUE;
    }
    return FALSE;
}

static bool8 HasRematchTrainerAlreadyBeenFought(const VsSeekerData *vsSeekerData, u16 trainerBattleOpponent)
{
    s32 rematchIdx = GetRematchIdx(vsSeekerData, trainerBattleOpponent);

    if (rematchIdx == -1 || !HasTrainerBeenFought(vsSeekerData[rematchIdx].trainerIdxs[0]))
        return FALSE;
	
    return TRUE;
}

void ClearRematchStateOfLastTalked(void)
{
    gSaveBlock1Ptr->trainerRematches[gSpecialVar_LastTalked] = 0;
    SetBattledTrainerFlag();
}

static int LookupVsSeekerOpponentInArray(const VsSeekerData * array, u16 trainerId)
{
    int i, j;
	u16 testTrainerId;

    for (i = 0; i < ARRAY_COUNT(sVsSeekerData); i++)
    {
        for (j = 0; j < 6; j++)
        {
            if (array[i].trainerIdxs[j] == 0)
                break;
			
            testTrainerId = array[i].trainerIdxs[j];
			
            if (testTrainerId == 0xFFFF)
                continue;
			
            if (testTrainerId == trainerId)
                return i;
        }
    }
    return -1;
}

int GetRematchTrainerId(u16 trainerId)
{
    u8 i, j = GetNextAvailableRematchTrainer(sVsSeekerData, trainerId, &i);

    if (!j)
        return 0;
	
    TryGetRematchTrainerIdGivenGameState(sVsSeekerData[i].trainerIdxs, &j);
	
    return sVsSeekerData[i].trainerIdxs[j];
}

u8 IsTrainerReadyForRematch(void)
{
    return IsTrainerReadyForRematchInternal(sVsSeekerData, gTrainerBattleOpponent_A);
}

static bool8 IsTrainerReadyForRematchInternal(const VsSeekerData * array, u16 trainerId)
{
    int rematchTrainerIdx = LookupVsSeekerOpponentInArray(array, trainerId);

    if (rematchTrainerIdx == -1)
        return FALSE;
    if (rematchTrainerIdx >= ARRAY_COUNT(sVsSeekerData))
        return FALSE;
    if (!IsThisTrainerRematchable(gSpecialVar_LastTalked))
        return FALSE;
    return TRUE;
}

static bool8 ObjectEventIdIsSane(u8 objectEventId)
{
    struct ObjectEvent *objectEvent = &gObjectEvents[objectEventId];

    if (objectEvent->active && gMapHeader.events->objectEventCount >= objectEvent->localId && gSprites[objectEvent->spriteId].data[0] == objectEventId)
        return TRUE;
    return FALSE;
}

static u8 GetRandomFaceDirectionMovementType(void)
{
    switch (RandomMax(4))
    {
        case 0:
            return MOVEMENT_TYPE_FACE_UP;
        case 1:
            return MOVEMENT_TYPE_FACE_DOWN;
        case 2:
            return MOVEMENT_TYPE_FACE_LEFT;
        case 3:
            return MOVEMENT_TYPE_FACE_RIGHT;
        default:
            return MOVEMENT_TYPE_FACE_DOWN;
    }
}

static u8 GetRunningBehaviorFromGraphicsId(u8 graphicsId)
{
    switch (graphicsId)
    {
        case OBJ_EVENT_GFX_LITTLE_GIRL:
        case OBJ_EVENT_GFX_YOUNGSTER:
        case OBJ_EVENT_GFX_BOY:
        case OBJ_EVENT_GFX_BUG_CATCHER:
        case OBJ_EVENT_GFX_LASS:
        case OBJ_EVENT_GFX_WOMAN_1:
        case OBJ_EVENT_GFX_BATTLE_GIRL:
        case OBJ_EVENT_GFX_MAN:
        case OBJ_EVENT_GFX_ROCKER:
        case OBJ_EVENT_GFX_WOMAN_2:
        case OBJ_EVENT_GFX_BEAUTY:
        case OBJ_EVENT_GFX_BALDING_MAN:
        case OBJ_EVENT_GFX_TUBER_F:
        case OBJ_EVENT_GFX_CAMPER:
        case OBJ_EVENT_GFX_PICNICKER:
        case OBJ_EVENT_GFX_COOLTRAINER_M:
        case OBJ_EVENT_GFX_COOLTRAINER_F:
        case OBJ_EVENT_GFX_SWIMMER_M_LAND:
        case OBJ_EVENT_GFX_SWIMMER_F_LAND:
        case OBJ_EVENT_GFX_BLACKBELT:
        case OBJ_EVENT_GFX_HIKER:
        case OBJ_EVENT_GFX_SAILOR:
            return MOVEMENT_TYPE_VS_SEEKER_4E;
        case OBJ_EVENT_GFX_TUBER_M_WATER:
        case OBJ_EVENT_GFX_SWIMMER_M_WATER:
        case OBJ_EVENT_GFX_SWIMMER_F_WATER:
            return MOVEMENT_TYPE_VS_SEEKER_4F;
        default:
            return MOVEMENT_TYPE_VS_SEEKER_4D;
    }
}

/*
 * The trainer flag is a little-endian short located +2 from
 * the script pointer, assuming the trainerbattle command is
 * first in the script.  Because scripts are unaligned, and
 * because the ARM processor requires shorts to be 16-bit
 * aligned, this function needs to perform explicit bitwise
 * operations to get the correct flag.
 *
 * 5c XX YY ZZ ...
 *       -- --
 */
static u16 GetTrainerFlagFromScript(const u8 *script)
{
    u16 trainerFlag;
    script += 2;
    trainerFlag = script[0];
    trainerFlag |= script[1] << 8;
    return trainerFlag;
}

static int GetRematchIdx(const VsSeekerData * vsSeekerData, u16 trainerFlagIdx)
{
    int i;

    for (i = 0; i < ARRAY_COUNT(sVsSeekerData); i++)
    {
        if (vsSeekerData[i].trainerIdxs[0] == trainerFlagIdx)
            return i;
    }
    return -1;
}

static bool32 IsThisTrainerRematchable(u32 localId)
{
    if (!gSaveBlock1Ptr->trainerRematches[localId])
        return FALSE;
    return TRUE;
}

static void ClearAllTrainerRematchStates(void)
{
    u8 i;

    for (i = 0; i < ARRAY_COUNT(gSaveBlock1Ptr->trainerRematches); i++)
        gSaveBlock1Ptr->trainerRematches[i] = 0;
}

static bool8 IsTrainerVisibleOnScreen(struct VsSeekerTrainerInfo * trainerInfo)
{
    s16 x;
    s16 y;

    PlayerGetDestCoords(&x, &y);
    x -= 7;
    y -= 7;

    if (x - 7 <= trainerInfo->xCoord && x + 7 >= trainerInfo->xCoord && y - 5 <= trainerInfo->yCoord && y + 5 >= trainerInfo->yCoord && ObjectEventIdIsSane(trainerInfo->objectEventId) == 1)
        return TRUE;
	
    return FALSE;
}

static u8 GetNextAvailableRematchTrainer(const VsSeekerData * vsSeekerData, u16 trainerFlagNo, u8 * idxPtr)
{
    int i, j;

    for (i = 0; i < ARRAY_COUNT(sVsSeekerData); i++)
    {
        if (vsSeekerData[i].trainerIdxs[0] == trainerFlagNo)
        {
            *idxPtr = i;
			
            for (j = 1; j < 6; j++)
            {
                if (vsSeekerData[i].trainerIdxs[j] == 0)
                    return j - 1;
				
                if (vsSeekerData[i].trainerIdxs[j] == 0xffff)
                    continue;
				
                if (HasTrainerBeenFought(vsSeekerData[i].trainerIdxs[j]))
                    continue;
				
                return j;
            }
            return j - 1;
        }
    }
    *idxPtr = 0;
    return 0;
}

static u8 GetRematchableTrainerLocalId(void)
{
    u8 i, idx;

    for (i = 0; sVsSeeker->trainerInfo[i].localId != 0xFF; i++)
    {
        if (IsTrainerVisibleOnScreen(&sVsSeeker->trainerInfo[i]) == 1)
        {
            if (HasTrainerBeenFought(sVsSeeker->trainerInfo[i].trainerIdx) != 1 || GetNextAvailableRematchTrainer(sVsSeekerData, sVsSeeker->trainerInfo[i].trainerIdx, &idx))
                return sVsSeeker->trainerInfo[i].localId;
        }
    }
    return 0xFF;
}

static void StartTrainerObjectMovementScript(struct VsSeekerTrainerInfo * trainerInfo, const u8 * script)
{
    UnfreezeObjectEvent(&gObjectEvents[trainerInfo->objectEventId]);
    ScriptMovement_StartObjectMovementScript(trainerInfo->localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, script);
}

static u8 GetCurVsSeekerResponse(s32 vsSeekerIdx, u16 trainerIdx)
{
    s32 i, j;

    for (i = 0; i < vsSeekerIdx; i++)
    {
        if (IsTrainerVisibleOnScreen(&sVsSeeker->trainerInfo[i]) == 1 && sVsSeeker->trainerInfo[i].trainerIdx == trainerIdx)
        {
            for (j = 0; j < sVsSeeker->numRematchableTrainers; j++)
            {
                if (sVsSeeker->trainerIdxArray[j] == sVsSeeker->trainerInfo[i].trainerIdx)
                    return VSSEEKER_SINGLE_RESP_YES;
            }
            return VSSEEKER_SINGLE_RESP_NO;
        }
    }
    return VSSEEKER_SINGLE_RESP_RAND;
}

static void StartAllRespondantIdleMovements(void)
{
    u8 dummy = 0;
    s32 i, j;

    for (i = 0; i < sVsSeeker->numRematchableTrainers; i++)
    {
        for (j = 0; sVsSeeker->trainerInfo[j].localId != 0xFF; j++)
        {
            if (sVsSeeker->trainerInfo[j].trainerIdx == sVsSeeker->trainerIdxArray[i])
            {
                struct ObjectEvent *objectEvent = &gObjectEvents[sVsSeeker->trainerInfo[j].objectEventId];

                if (ObjectEventIdIsSane(sVsSeeker->trainerInfo[j].objectEventId) == 1)
                    SetTrainerMovementType(objectEvent, sVsSeeker->runningBehaviourEtcArray[i]);
				
                OverrideMovementTypeForObjectEvent(objectEvent, sVsSeeker->runningBehaviourEtcArray[i]);
                gSaveBlock1Ptr->trainerRematches[sVsSeeker->trainerInfo[j].localId] = GetNextAvailableRematchTrainer(sVsSeekerData, sVsSeeker->trainerInfo[j].trainerIdx, &dummy);
            }
        }
    }
}

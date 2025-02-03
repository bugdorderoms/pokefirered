#include "global.h"
#include "gflib.h"
#include "item.h"
#include "util.h"
#include "random.h"
#include "pokedex.h"
#include "money.h"
#include "battle_main.h"
#include "battle_gimmicks.h"
#include "battle_setup.h"
#include "pokemon_icon.h"
#include "mail.h"
#include "mail_data.h"
#include "event_data.h"
#include "battle_move_effects.h"
#include "strings.h"
#include "wild_encounter.h"
#include "pokemon_special_anim.h"
#include "pokemon_storage_system.h"
#include "pokemon_summary_screen.h"
#include "task.h"
#include "naming_screen.h"
#include "dns.h"
#include "overworld.h"
#include "evolution.h"
#include "form_change.h"
#include "party_menu.h"
#include "battle_queued_effects.h"
#include "trainer_pokemon_sprites.h"
#include "field_specials.h"
#include "battle.h"
#include "pokemon.h"
#include "battle_message.h"
#include "battle_anim.h"
#include "battle_ai_util.h"
#include "calculate_base_damage.h"
#include "battle_scripts.h"
#include "trainer_slide.h"
#include "set_effect.h"
#include "reshow_battle_screen.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "constants/battle_anim.h"
#include "constants/battle_script_commands.h"
#include "constants/form_change.h"
#include "constants/items.h"
#include "constants/hold_effects.h"
#include "constants/songs.h"
#include "constants/moves.h"
#include "constants/abilities.h"
#include "constants/pokemon.h"
#include "constants/maps.h"
#include "constants/battle_string_ids.h"
#include "constants/pokedex.h"

// Helper for accessing command arguments and advancing gBattlescriptCurrInstr.
//
// For example jumpifability is defined as:
//
//     .macro jumpifability battler:req, ability:req, ptr:req
//	   .byte 0x1e
//	   .byte \battler
//     .2byte \ability
//     .4byte \ptr
//	   .endm
//
// Which corresponds to:
//
//     CMD_ARGS(u8 battler, u16 ability, const u8 *ptr);
//
// The arguments can be accessed as cmd->battler, cmd->ability and cmd->ptr.
// gBattlescriptCurrInstr = cmd->nextInstr; advances to the next instruction.
#define CMD_ARGS(...) const struct __attribute__((packed)) { u8 opcode; RECURSIVELY(R_FOR_EACH(APPEND_SEMICOLON, __VA_ARGS__)) const u8 nextInstr[0]; } *const cmd UNUSED = (const void *)gBattlescriptCurrInstr
#define VARIOUS_ARGS(...) CMD_ARGS(u8 battler, u8 id, ##__VA_ARGS__)
#define NATIVE_ARGS(...) CMD_ARGS(void (*func)(void), ##__VA_ARGS__)

struct StatFractions
{
    u8 dividend;
    u8 divisor;
};

struct PickupItem
{
    u16 itemId;
    u8 chance;
};

static void TrySetDestinyBondToHappen(void);
static void InitLvlUpBox(void);
static bool8 SlideInLvlUpBox(void);
static bool8 SlideOutLvlUpBox(void);
static void DrawLevelUpWindow1(void);
static void DrawLevelUpWindow2(void);
static void PutMonIconOnLvlUpBox(void);
static void PutLevelAndGenderOnLvlUpBox(void);
static bool8 NoTargetPresent(void);
static u8 GetNextTarget(u8 moveTarget, bool8 excludeCurrent);
static void SpriteCB_MonIconOnLvlUpBox(struct Sprite *sprite);

static void atk00_attackcanceler(void);
static void atk01_accuracycheck(void);
static void atk02_attackstring(void);
static void atk03_ppreduce(void);
static void atk04_critcalc(void);
static void atk05_damagecalc(void);
static void atk06_typecalc(void);
static void atk07_adjustdamage(void);
static void atk08_setstatchanger(void);
static void atk09_attackanimation(void);
static void atk0A_lunardancepprestore(void);
static void atk0B_healthbarupdate(void);
static void atk0C_datahpupdate(void);
static void atk0D_critmessage(void);
static void atk0E_effectivenesssound(void);
static void atk0F_resultmessage(void);
static void atk10_printstring(void);
static void atk11_tryacupressure(void);
static void atk12_waitmessage(void);
static void atk13_printfromtable(void);
static void atk14_printselectionstringfromtable(void);
static void atk15_setadditionaleffects(void);
static void atk16_setspecialmoveeffect(void);
static void atk17_setsplitswap(void);
static void atk18_clearbattlerstatus(void);
static void atk19_tryfaintmon(void);
static void atk1A_jumpifweathercheckchargeeffects(void);
static void atk1B_doeffectsonfaint(void);
static void atk1C_jumpbasedonstatus(void);
static void atk1D_jumpbasedonstatus1(void);
static void atk1E_jumpifability(void);
static void atk1F_jumpifsideaffecting(void);
static void atk20_jumpifstat(void);
static void atk21_applyqueuedstatboosts(void);
static void atk22_jumpbasedontype(void);
static void atk23_getexp(void);
static void atk24_checkteamslot(void);
static void atk25_jumpifabilityonside(void);
static void atk26_setmoveeffect(void);
static void atk27_nop(void);
static void atk28_goto(void);
static void atk29_jumpifbyte(void);
static void atk2A_jumpifhalfword(void);
static void atk2B_jumpifword(void);
static void atk2C_jumpifarrayequal(void);
static void atk2D_jumpifarraynotequal(void);
static void atk2E_setbyte(void);
static void atk2F_addbyte(void);
static void atk30_subbyte(void);
static void atk31_copyarray(void);
static void atk32_sethalfword(void);
static void atk33_orbyte(void);
static void atk34_orhalfword(void);
static void atk35_orword(void);
static void atk36_bicbyte(void);
static void atk37_bichalfword(void);
static void atk38_bicword(void);
static void atk39_pause(void);
static void atk3A_waitstate(void);
static void atk3B_nop(void);
static void atk3C_return(void);
static void atk3D_end(void);
static void atk3E_end2(void);
static void atk3F_end3(void);
static void atk40_nop(void);
static void atk41_call(void);
static void atk42_trysetsleep(void);
static void atk43_trysetconfusion(void);
static void atk44_nop(void);
static void atk45_playanimation(void);
static void atk46_playanimation2(void);
static void atk47_prefaintmoveend(void);
static void atk48_playstatchangeanimation(void);
static void atk49_moveend(void);
static void atk4A_formchange(void);
static void atk4B_returnatktoball(void);
static void atk4C_getswitchedmondata(void);
static void atk4D_switchindataupdate(void);
static void atk4E_switchinanim(void);
static void atk4F_jumpifcantswitch(void);
static void atk50_openpartyscreen(void);
static void atk51_switchhandleorder(void);
static void atk52_switchineffects(void);
static void atk53_nop(void);
static void atk54_playse(void);
static void atk55_fanfare(void);
static void atk56_nop(void);
static void atk57_endlinkbattle(void);
static void atk58_returntoball(void);
static void atk59_handlelearnnewmove(void);
static void atk5A_yesnoboxlearnmove(void);
static void atk5B_yesnoboxstoplearningmove(void);
static void atk5C_nop(void);
static void atk5D_getmoneyreward(void);
static void atk5E_nop(void);
static void atk5F_swapattackerwithtarget(void);
static void atk60_nop(void);
static void atk61_drawpartystatussummary(void);
static void atk62_hidepartystatussummary(void);
static void atk63_jumptocalledmove(void);
static void atk64_nop(void);
static void atk65_nop(void);
static void atk66_chosenstatusanimation(void);
static void atk67_yesnobox(void);
static void atk68_nop(void);
static void atk69_nop(void);
static void atk6A_consumeitem(void);
static void atk6B_nop(void);
static void atk6C_drawlvlupbox(void);
static void atk6D_nop(void);
static void atk6E_nop(void);
static void atk6F_makevisible(void);
static void atk70_recordbattlerability(void);
static void atk71_nop(void);
static void atk72_jumpifplayerran(void);
static void atk73_hpthresholds(void);
static void atk74_hpthresholds2(void);
static void atk75_nop(void);
static void atk76_various(void);
static void atk77_setprotectlike(void);
static void atk78_instanthpdrop(void);
static void atk79_setatkhptozero(void);
static void atk7A_nop(void);
static void atk7B_tryhealhalfhealth(void);
static void atk7C_trymirrormove(void);
static void atk7D_nop(void);
static void atk7E_trysetreflect(void);
static void atk7F_setseeded(void);
static void atk80_manipulatedamage(void);
static void atk81_trysetrest(void);
static void atk82_nop(void);
static void atk83_handletrainerslidecase(void);
static void atk84_trysetpoison(void);
static void atk85_stockpile(void);
static void atk86_stockpiletobasedamage(void);
static void atk87_stockpiletohpheal(void);
static void atk88_nop(void);
static void atk89_statbuffchange(void);
static void atk8A_displaystatchangestring(void);
static void atk8B_nop(void);
static void atk8C_nop(void);
static void atk8D_nop(void);
static void atk8E_nop(void);
static void atk8F_forcerandomswitch(void);
static void atk90_nop(void);
static void atk91_givepaydaymoney(void);
static void atk92_trysetlightscreen(void);
static void atk93_tryKO(void);
static void atk94_nop(void);
static void atk95_setweather(void);
static void atk96_nop(void);
static void atk97_tryinfatuating(void);
static void atk98_updatestatusicon(void);
static void atk99_nop(void);
static void atk9A_nop(void);
static void atk9B_nop(void);
static void atk9C_setsubstitute(void);
static void atk9D_mimicattackcopy(void);
static void atk9E_nop(void);
static void atk9F_nop(void);
static void atkA0_nop(void);
static void atkA1_trycounterattack(void);
static void atkA2_nop(void);
static void atkA3_disablelastusedattack(void);
static void atkA4_trysetencore(void);
static void atkA5_painsplitdmgcalc(void);
static void atkA6_settypetorandomresistance(void);
static void atkA7_trysetalwayshitflag(void);
static void atkA8_copymovepermanently(void);
static void atkA9_trychoosesleeptalkmove(void);
static void atkAA_nop(void);
static void atkAB_nop(void);
static void atkAC_trysetburn(void);
static void atkAD_tryspiteppreduce(void);
static void atkAE_healpartystatus(void);
static void atkAF_cursetarget(void);
static void atkB0_trysetspikes(void);
static void atkB1_nop(void);
static void atkB2_trysetperishsong(void);
static void atkB3_handlerollout(void);
static void atkB4_nop(void);
static void atkB5_handlefurycutter(void);
static void atkB6_trysetparalyze(void);
static void atkB7_presentcalc(void);
static void atkB8_trysetsafeguard(void);
static void atkB9_magnitudedamagecalculation(void);
static void atkBA_jumpifnopursuitswitchdmg(void);
static void atkBB_nop(void);
static void atkBC_maxattackhalvehp(void);
static void atkBD_nop(void);
static void atkBE_rapidspinfree(void);
static void atkBF_nop(void);
static void atkC0_recoverbasedonweather(void);
static void atkC1_nop(void);
static void atkC2_nop(void);
static void atkC3_trysetfutureattack(void);
static void atkC4_nop(void);
static void atkC5_setsemiinvulnerablebit(void);
static void atkC6_nop(void);
static void atkC7_nop(void);
static void atkC8_nop(void);
static void atkC9_nop(void);
static void atkCA_nop(void);
static void atkCB_nop(void);
static void atkCC_nop(void);
static void atkCD_nop(void);
static void atkCE_settorment(void);
static void atkCF_nop(void);
static void atkD0_settaunt(void);
static void atkD1_nop(void);
static void atkD2_tryswapitems(void);
static void atkD3_copyability(void);
static void atkD4_trywish(void);
static void atkD5_trysetroots(void);
static void atkD6_nop(void);
static void atkD7_setyawn(void);
static void atkD8_nop(void);
static void atkD9_nop(void);
static void atkDA_swapabilities(void);
static void atkDB_tryimprison(void);
static void atkDC_trysetgrudge(void);
static void atkDD_mefirstattackselect(void);
static void atkDE_assistattackselect(void);
static void atkDF_trysetmagiccoat(void);
static void atkE0_trysetsnatch(void);
static void atkE1_nop(void);
static void atkE2_switchoutabilities(void);
static void atkE3_nop(void);
static void atkE4_nop(void);
static void atkE5_pickup(void);
static void atkE6_nop(void);
static void atkE7_nop(void);
static void atkE8_setfieldsport(void);
static void atkE9_nop(void);
static void atkEA_tryrecycleitem(void);
static void atkEB_settypetoterrain(void);
static void atkEC_nop(void);
static void atkED_nop(void);
static void atkEE_nop(void);
static void atkEF_handleballthrow(void);
static void atkF0_givecaughtmon(void);
static void atkF1_trysetcaughtmondexflags(void);
static void atkF2_displaydexinfo(void);
static void atkF3_trygivecaughtmonnick(void);
static void atkF4_nop(void);
static void atkF5_nop(void);
static void atkF6_finishaction(void);
static void atkF7_finishturn(void);
static void atkF8_callfunction(void);
static void atkF9_cureprimarystatus(void);
static void atkFA_setword(void);
static void atkFB_jumpifsubstituteblocks(void);
static void atkFC_handleabilitypopup(void);
static void atkFD_nop(void);
static void atkFE_nop(void);
static void atkFF_nop(void);

void (* const gBattleScriptingCommandsTable[])(void) =
{
    atk00_attackcanceler,
    atk01_accuracycheck,
    atk02_attackstring,
    atk03_ppreduce,
    atk04_critcalc,
    atk05_damagecalc,
    atk06_typecalc,
    atk07_adjustdamage,
    atk08_setstatchanger,
    atk09_attackanimation,
    atk0A_lunardancepprestore,
    atk0B_healthbarupdate,
    atk0C_datahpupdate,
    atk0D_critmessage,
    atk0E_effectivenesssound,
    atk0F_resultmessage,
    atk10_printstring,
    atk11_tryacupressure,
    atk12_waitmessage,
    atk13_printfromtable,
    atk14_printselectionstringfromtable,
    atk15_setadditionaleffects,
    atk16_setspecialmoveeffect,
    atk17_setsplitswap,
    atk18_clearbattlerstatus,
    atk19_tryfaintmon,
    atk1A_jumpifweathercheckchargeeffects,
    atk1B_doeffectsonfaint,
    atk1C_jumpbasedonstatus,
    atk1D_jumpbasedonstatus1,
    atk1E_jumpifability,
    atk1F_jumpifsideaffecting,
    atk20_jumpifstat,
    atk21_applyqueuedstatboosts,
    atk22_jumpbasedontype,
    atk23_getexp,
    atk24_checkteamslot,
    atk25_jumpifabilityonside,
    atk26_setmoveeffect,
    atk27_nop,
    atk28_goto,
    atk29_jumpifbyte,
    atk2A_jumpifhalfword,
    atk2B_jumpifword,
    atk2C_jumpifarrayequal,
    atk2D_jumpifarraynotequal,
    atk2E_setbyte,
    atk2F_addbyte,
    atk30_subbyte,
    atk31_copyarray,
    atk32_sethalfword,
    atk33_orbyte,
    atk34_orhalfword,
    atk35_orword,
    atk36_bicbyte,
    atk37_bichalfword,
    atk38_bicword,
    atk39_pause,
    atk3A_waitstate,
    atk3B_nop,
    atk3C_return,
    atk3D_end,
    atk3E_end2,
    atk3F_end3,
    atk40_nop,
    atk41_call,
    atk42_trysetsleep,
    atk43_trysetconfusion,
    atk44_nop,
    atk45_playanimation,
    atk46_playanimation2,
    atk47_prefaintmoveend,
    atk48_playstatchangeanimation,
    atk49_moveend,
    atk4A_formchange,
    atk4B_returnatktoball,
    atk4C_getswitchedmondata,
    atk4D_switchindataupdate,
    atk4E_switchinanim,
    atk4F_jumpifcantswitch,
    atk50_openpartyscreen,
    atk51_switchhandleorder,
    atk52_switchineffects,
    atk53_nop,
    atk54_playse,
    atk55_fanfare,
    atk56_nop,
    atk57_endlinkbattle,
    atk58_returntoball,
    atk59_handlelearnnewmove,
    atk5A_yesnoboxlearnmove,
    atk5B_yesnoboxstoplearningmove,
    atk5C_nop,
    atk5D_getmoneyreward,
    atk5E_nop,
    atk5F_swapattackerwithtarget,
    atk60_nop,
    atk61_drawpartystatussummary,
    atk62_hidepartystatussummary,
    atk63_jumptocalledmove,
    atk64_nop,
    atk65_nop,
    atk66_chosenstatusanimation,
    atk67_yesnobox,
    atk68_nop,
    atk69_nop,
    atk6A_consumeitem,
    atk6B_nop,
    atk6C_drawlvlupbox,
    atk6D_nop,
    atk6E_nop,
    atk6F_makevisible,
    atk70_recordbattlerability,
    atk71_nop,
    atk72_jumpifplayerran,
    atk73_hpthresholds,
    atk74_hpthresholds2,
    atk75_nop,
    atk76_various,
    atk77_setprotectlike,
    atk78_instanthpdrop,
    atk79_setatkhptozero,
    atk7A_nop,
    atk7B_tryhealhalfhealth,
    atk7C_trymirrormove,
    atk7D_nop,
    atk7E_trysetreflect,
    atk7F_setseeded,
    atk80_manipulatedamage,
    atk81_trysetrest,
    atk82_nop,
    atk83_handletrainerslidecase,
    atk84_trysetpoison,
    atk85_stockpile,
    atk86_stockpiletobasedamage,
    atk87_stockpiletohpheal,
    atk88_nop,
    atk89_statbuffchange,
    atk8A_displaystatchangestring,
    atk8B_nop,
    atk8C_nop,
    atk8D_nop,
    atk8E_nop,
    atk8F_forcerandomswitch,
    atk90_nop,
    atk91_givepaydaymoney,
    atk92_trysetlightscreen,
    atk93_tryKO,
    atk94_nop,
    atk95_setweather,
    atk96_nop,
    atk97_tryinfatuating,
    atk98_updatestatusicon,
    atk99_nop,
    atk9A_nop,
    atk9B_nop,
    atk9C_setsubstitute,
    atk9D_mimicattackcopy,
    atk9E_nop,
    atk9F_nop,
    atkA0_nop,
    atkA1_trycounterattack,
    atkA2_nop,
    atkA3_disablelastusedattack,
    atkA4_trysetencore,
    atkA5_painsplitdmgcalc,
    atkA6_settypetorandomresistance,
    atkA7_trysetalwayshitflag,
    atkA8_copymovepermanently,
    atkA9_trychoosesleeptalkmove,
    atkAA_nop,
    atkAB_nop,
    atkAC_trysetburn,
    atkAD_tryspiteppreduce,
    atkAE_healpartystatus,
    atkAF_cursetarget,
    atkB0_trysetspikes,
    atkB1_nop,
    atkB2_trysetperishsong,
    atkB3_handlerollout,
    atkB4_nop,
    atkB5_handlefurycutter,
    atkB6_trysetparalyze,
    atkB7_presentcalc,
    atkB8_trysetsafeguard,
    atkB9_magnitudedamagecalculation,
    atkBA_jumpifnopursuitswitchdmg,
    atkBB_nop,
    atkBC_maxattackhalvehp,
    atkBD_nop,
    atkBE_rapidspinfree,
    atkBF_nop,
    atkC0_recoverbasedonweather,
    atkC1_nop,
    atkC2_nop,
    atkC3_trysetfutureattack,
    atkC4_nop,
    atkC5_setsemiinvulnerablebit,
    atkC6_nop,
    atkC7_nop,
    atkC8_nop,
    atkC9_nop,
    atkCA_nop,
    atkCB_nop,
    atkCC_nop,
    atkCD_nop,
    atkCE_settorment,
    atkCF_nop,
    atkD0_settaunt,
    atkD1_nop,
    atkD2_tryswapitems,
    atkD3_copyability,
    atkD4_trywish,
    atkD5_trysetroots,
    atkD6_nop,
    atkD7_setyawn,
    atkD8_nop,
    atkD9_nop,
    atkDA_swapabilities,
    atkDB_tryimprison,
    atkDC_trysetgrudge,
    atkDD_mefirstattackselect,
    atkDE_assistattackselect,
    atkDF_trysetmagiccoat,
    atkE0_trysetsnatch,
    atkE1_nop,
    atkE2_switchoutabilities,
    atkE3_nop,
    atkE4_nop,
    atkE5_pickup,
    atkE6_nop,
    atkE7_nop,
    atkE8_setfieldsport,
    atkE9_nop,
    atkEA_tryrecycleitem,
    atkEB_settypetoterrain,
    atkEC_nop,
    atkED_nop,
    atkEE_nop,
    atkEF_handleballthrow,
    atkF0_givecaughtmon,
    atkF1_trysetcaughtmondexflags,
    atkF2_displaydexinfo,
    atkF3_trygivecaughtmonnick,
    atkF4_nop,
    atkF5_nop,
    atkF6_finishaction,
    atkF7_finishturn,
    atkF8_callfunction,
    atkF9_cureprimarystatus,
    atkFA_setword,
    atkFB_jumpifsubstituteblocks,
    atkFC_handleabilitypopup,
    atkFD_nop,
    atkFE_nop,
	atkFF_nop,
};

#include "data/move/battle_move_effects.h"

static const struct StatFractions sAccuracyStageRatios[] =
{
    {  33, 100 }, // -6
    {  36, 100 }, // -5
    {  43, 100 }, // -4
    {  50, 100 }, // -3
    {  60, 100 }, // -2
    {  75, 100 }, // -1
    {   1,   1 }, //  0
    { 133, 100 }, // +1
    { 166, 100 }, // +2
    {   2,   1 }, // +3
    { 233, 100 }, // +4
    { 133,  50 }, // +5
    {   3,   1 }, // +6
};

// The chance is 1/N for each stage.
static const u8 sCriticalHitChance[] = { 24, 8, 2, 1, 1 };

static const u16 sUnknownBattleboxPal[] = INCBIN_U16("graphics/battle_interface/unk_battlebox.gbapal");
static const u32 sUnknownBattleboxGfx[] = INCBIN_U32("graphics/battle_interface/unk_battlebox.4bpp.lz");

static const struct OamData sOamData_MonIconOnLvlUpBox =
{
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
};

#define MON_ICON_LVLUP_BOX_TAG 0xD75A

static const struct SpriteTemplate sSpriteTemplate_MonIconOnLvlUpBox =
{
    .tileTag = MON_ICON_LVLUP_BOX_TAG,
    .paletteTag = MON_ICON_LVLUP_BOX_TAG,
    .oam = &sOamData_MonIconOnLvlUpBox,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_MonIconOnLvlUpBox
};

static const u8 sExpBlockLevels[] =
{
    16, //badge 1
    20, //badge 2
    25, //badge 3
    30, //badge 4
    35, //badge 5
    40, //badge 6
    45, //badge 7
    50, //badge 8
};

static const struct PickupItem sPickupItems[] =
{
    { ITEM_ORAN_BERRY,   15 },
    { ITEM_CHERI_BERRY,  25 },
    { ITEM_CHESTO_BERRY, 35 },
    { ITEM_PECHA_BERRY,  45 },
    { ITEM_RAWST_BERRY,  55 },
    { ITEM_ASPEAR_BERRY, 65 },
    { ITEM_PERSIM_BERRY, 75 },
    { NUM_TO_TM(10),     80 },
    { ITEM_PP_UP,        85 },
    { ITEM_RARE_CANDY,   90 },
    { ITEM_NUGGET,       95 },
    { ITEM_SPELON_BERRY, 96 },
    { ITEM_PAMTRE_BERRY, 97 },
    { ITEM_WATMEL_BERRY, 98 },
    { ITEM_DURIN_BERRY,  99 },
    { ITEM_BELUE_BERRY,  1  },
};

u8 GetCurrentLevelCapLevel(void)
{
#if EXP_BLOCK
	u8 numBadges = GetNumOfBadges();
	
	if (numBadges != NUM_BADGES)
		return sExpBlockLevels[numBadges];
#endif

	return MAX_LEVEL;
}

static void TrySetDestinyBondToHappen(void)
{
    if ((gBattleMons[gBattlerTarget].status2 & STATUS2_DESTINY_BOND) && GetBattlerSide(gBattlerAttacker) != GetBattlerSide(gBattlerTarget) && !(gHitMarker & HITMARKER_IGNORE_GRUDGE))
        gHitMarker |= HITMARKER_DESTINYBOND;
}

static void MoveValuesCleanUp(void)
{
    gMoveResultFlags = 0;
    gIsCriticalHit = FALSE;
    gBattleStruct->moveEffect.moveEffectByte = MOVE_EFFECT_NONE;
	gBattleStruct->moveEffect2.moveEffectByte = MOVE_EFFECT_NONE;
    gBattleCommunication[MISS_TYPE] = B_MSG_MISSED;
    gHitMarker &= ~(HITMARKER_DESTINYBOND | HITMARKER_SYNCHRONISE_EFFECT);
}

static bool8 NoTargetPresent(void)
{
	if (!IsBattlerAlive(gBattlerTarget))
		gBattlerTarget = GetMoveTarget(gCurrentMove, 0);
	
	switch (GetBattlerMoveTargetType(gBattlerAttacker, gCurrentMove))
	{
		case MOVE_TARGET_BOTH:
		case MOVE_TARGET_OPPONENTS:
		    if (CountAliveMonsInBattle(gBattlerTarget, BATTLE_ALIVE_SIDE) == 0)
				return TRUE;
			break;
		case MOVE_TARGET_SELECTED:
		case MOVE_TARGET_DEPENDS:
		case MOVE_TARGET_RANDOM:
		case MOVE_TARGET_ALLY:
		case MOVE_TARGET_SELECTED_OPPONENT:
            if (!IsBattlerAlive(gBattlerTarget))
				return TRUE;
			break;
		case MOVE_TARGET_FOES_AND_ALLY:
            if (CountAliveMonsInBattle(gBattlerAttacker, BATTLE_ALIVE_EXCEPT_BATTLER) == 0)
				return TRUE;
			break;
	}
	return FALSE;
}

static bool8 IsMoveAffectedByParentalBond(u8 attacker, u8 defender, u16 move)
{
	if (!IS_MOVE_STATUS(move) && gBattleMoves[move].effect != EFFECT_SEMI_INVULNERABLE && gBattleMoves[move].effect != EFFECT_EXPLOSION
	&& gBattleMoves[move].effect != EFFECT_MULTI_HIT && gBattleMoves[move].effect != EFFECT_OHKO && !(gBattleMoves[move].flags.strikeCount > 1)
	&& !gBattleMoves[move].flags.forbiddenParentalBond && !IsZMove(move) && !IsMaxMove(move) && DoesSpreadMoveStrikesOnlyOnce(attacker, defender, move, FALSE))
	    return TRUE;
	return FALSE;
}

static void atk00_attackcanceler(void)
{
	CMD_ARGS();
	
	u8 i;
	u16 atkAbility;
	
	// Finished action
    if (gBattleOutcome)
    {
        gCurrentActionFuncId = B_ACTION_FINISHED;
        return;
    }
	
	// Attacker fainted
    if (!IsBattlerAlive(gBattlerAttacker) && !(gHitMarker & HITMARKER_NO_ATTACKSTRING))
    {
        gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
        gBattlescriptCurrInstr = BattleScript_MoveEnd;
        return;
    }
	
	// Check effects like flinch, freeze, etc.
    if (AtkCanceller_UnableToUseMove())
        return;
	
	atkAbility = GetBattlerAbility(gBattlerAttacker);
	
	// Check Parental Bond
	if (gSpecialStatuses[gBattlerAttacker].parentalBondState == PARENTAL_BOND_OFF && atkAbility == ABILITY_PARENTAL_BOND && IsBattlerAlive(gBattlerTarget)
	&& IsMoveAffectedByParentalBond(gBattlerAttacker, gBattlerTarget, gCurrentMove))
	{
		gSpecialStatuses[gBattlerAttacker].parentalBondState = PARENTAL_BOND_1ST_HIT;
        gMultiHitCounter = 2;
		PrepareByteNumberBuffer(gBattleScripting.multihitString, 1, 0);
	}
	
	// Try Activate Protean
	if (gCurrentMove != MOVE_STRUGGLE && !IsBattlerOfType(gBattlerAttacker, gBattleStruct->dynamicMoveType) && gDisableStructs[gBattlerAttacker].canProteanActivate
	&& (atkAbility == ABILITY_PROTEAN || atkAbility == ABILITY_LIBERO) && !GET_MOVE_MOVEEFFECT_TABLE(gCurrentMove).callOtherMove) // Wait the move call the other move to change the type based on it instead
	{
		gDisableStructs[gBattlerAttacker].canProteanActivate = FALSE;
		SetBattlerType(gBattlerAttacker, gBattleStruct->dynamicMoveType);
		BattleScriptCall(BattleScript_ProteanActivates);
        return;
	}
	
	// Check abilities that blocks the move
	if (AbilityBattleEffects(ABILITYEFFECT_MOVES_BLOCK, gBattlerTarget))
		return;
	
	// Check no PP for move
    if (!gBattleMons[gBattlerAttacker].pp[gCurrMovePos] && !(gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS) 
	&& gCurrentMove != MOVE_STRUGGLE && !(gHitMarker & (HITMARKER_ALLOW_NO_PP | HITMARKER_NO_ATTACKSTRING)))
    {
        gBattlescriptCurrInstr = BattleScript_NoPPForMove;
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        return;
    }
	gHitMarker &= ~(HITMARKER_ALLOW_NO_PP);
	
	// Check if obeys
    if (!(gHitMarker & HITMARKER_OBEYS) && !(gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS))
    {
        switch (IsMonDisobedient())
        {
			case 0:
			    break;
			case 2:
			    gHitMarker |= HITMARKER_OBEYS;
				return;
			default:
			    gMoveResultFlags |= MOVE_RESULT_MISSED;
				return;
        }
    }
    gHitMarker |= HITMARKER_OBEYS;
	
	if ((NoTargetPresent() && (!GET_MOVE_MOVEEFFECT_TABLE(gCurrentMove).twoTurnsEffect || (gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS)))
		/*|| IsMoveNotAllowedInSkyBattle(gCurrentMove)*/)
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailedAtkStringPpReduce;
		if (!GET_MOVE_MOVEEFFECT_TABLE(gCurrentMove).twoTurnsEffect || (gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS))
            CancelMultiTurnMoves(gBattlerAttacker);
        return;
	}
	
	// Try Bounce the move back
	if (gBattleMoves[gCurrentMove].flags.magicCoatAffected && !gProtectStructs[gBattlerAttacker].usesBouncedMove)
	{
		bool8 effect = FALSE;
		
		if (gProtectStructs[gBattlerTarget].bounceMove)
		{
			gBattleScripting.bypassAbilityPopUp = TRUE;
			effect = TRUE;
		}
		else if (GetBattlerAbility(gBattlerTarget) == ABILITY_MAGIC_BOUNCE && !(gStatuses3[gBattlerTarget] & STATUS3_SEMI_INVULNERABLE))
			effect = TRUE;
		
		if (effect)
		{
			gProtectStructs[gBattlerTarget].usesBouncedMove = TRUE;
			BattleScriptCall(BattleScript_MagicCoatBounce);
			return;
		}
	}
	
	// Try Snatch move
	if (gBattleMoves[gCurrentMove].flags.snatchAffected)
	{
		for (i = 0; i < gBattlersCount; ++i)
		{
			if (gProtectStructs[gBattlerByTurnOrder[i]].stealMove)
			{
				gProtectStructs[gBattlerByTurnOrder[i]].stealMove = FALSE;
				gBattleScripting.battler = gBattlerByTurnOrder[i];
				BattleScriptCall(BattleScript_SnatchedMove);
				return;
			}
		}
	}
	
	// Check Lightning Rod/Storm Drain and Protect
    if (gSpecialStatuses[gBattlerTarget].abilityRedirected)
    {
        gSpecialStatuses[gBattlerTarget].abilityRedirected = FALSE;
		BattleScriptCall(BattleScript_TookAttack);
    }
    else
    {
		if (IsBattlerProtected(gBattlerAttacker, gBattlerTarget, gCurrentMove) && ((!GET_MOVE_MOVEEFFECT_TABLE(gCurrentMove).twoTurnsEffect
		|| (gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS))))
		{
			CancelMultiTurnMoves(gBattlerAttacker);
			gMoveResultFlags |= MOVE_RESULT_MISSED;
			gBattleStruct->battlers[gBattlerTarget].lastLandedMove = 0;
			gBattleStruct->battlers[gBattlerTarget].lastHitMoveType = 0;
			gBattleCommunication[MISS_TYPE] = B_MSG_PROTECTED;
			
			if (gSpecialStatuses[gBattlerAttacker].parentalBondState == PARENTAL_BOND_1ST_HIT)
			{
				gSpecialStatuses[gBattlerAttacker].parentalBondState = PARENTAL_BOND_OFF; // No second hit if first hit was blocked
				gSpecialStatuses[gBattlerAttacker].multiHitOn = FALSE;
				gMultiHitCounter = 0;
			}
		}
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void JumpIfMoveFailed(u8 adder, const u8 *jumpStr)
{
    const u8 *BS_ptr = gBattlescriptCurrInstr + adder;

    if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
    {
        gBattleStruct->battlers[gBattlerTarget].lastLandedMove = 0;
        gBattleStruct->battlers[gBattlerTarget].lastHitMoveType = 0;
        BS_ptr = jumpStr;
    }
    else
    {
        TrySetDestinyBondToHappen();
		
        if (AbilityBattleEffects(ABILITYEFFECT_ABSORBING, gBattlerTarget))
            return;
    }
    gBattlescriptCurrInstr = BS_ptr;
}

bool8 JumpIfMoveAffectedByProtect(u8 addr, const u8 *jumpStr)
{
    if (IsBattlerProtected(gBattlerAttacker, gBattlerTarget, gCurrentMove))
    {
		gMoveResultFlags |= MOVE_RESULT_MISSED;
        JumpIfMoveFailed(addr, jumpStr);
        gBattleCommunication[MISS_TYPE] = B_MSG_PROTECTED;
        return TRUE;
    }
    return FALSE;
}

static bool8 AccuracyCalcHelper(const u8 *jumpStr)
{
	if (gStatuses3[gBattlerTarget] & STATUS3_COMMANDING)
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		JumpIfMoveFailed(6, jumpStr);
	}
    else if ((gStatuses3[gBattlerAttacker] & STATUS3_ALWAYS_HITS) && gDisableStructs[gBattlerAttacker].battlerWithSureHit == gBattlerTarget) // Check Lock On
        JumpIfMoveFailed(6, jumpStr);
	else if (GetBattlerAbility(gBattlerAttacker) == ABILITY_NO_GUARD || GetBattlerAbility(gBattlerTarget) == ABILITY_NO_GUARD) // Check No Guard
		JumpIfMoveFailed(6, jumpStr);
	else if (IsBattlerOfType(gBattlerAttacker, TYPE_POISON) && gCurrentMove == MOVE_TOXIC)
		JumpIfMoveFailed(6, jumpStr);
	else if ((!gBattleMoves[gCurrentMove].flags.hitInAir && !gBattleMoves[gCurrentMove].flags.hitInAirDoubleDmg && (gStatuses3[gBattlerTarget] & STATUS3_ON_AIR))
		|| (!gBattleMoves[gCurrentMove].flags.hitUnderground && (gStatuses3[gBattlerTarget] & STATUS3_UNDERGROUND))
	    || (!gBattleMoves[gCurrentMove].flags.hitUnderwater && (gStatuses3[gBattlerTarget] & STATUS3_UNDERWATER))) // Check if semi-invulnerable
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
		JumpIfMoveFailed(6, jumpStr);
	}
	else if (gBattleMoves[gCurrentMove].effect == EFFECT_NEVER_MISS_IN_WEATHER && IsBattlerWeatherAffected(gBattlerAttacker, gBattleMoves[gCurrentMove].argument.neverMissInWeather.weather)) // Check never misses on weather
		JumpIfMoveFailed(6, jumpStr);
	else if ((gStatuses3[gBattlerTarget] & STATUS3_MINIMIZED) && gBattleMoves[gCurrentMove].flags.dmgMinimize) // Check never misses on minimize 
		JumpIfMoveFailed(6, jumpStr);
	else if (!gBattleMoves[gCurrentMove].accuracy) // Check moves that never misses
	    JumpIfMoveFailed(6, jumpStr);
	else
		return FALSE;
	
	return TRUE;
}

u32 CalcMoveTotalAccuracy(u16 move, u8 attacker, u8 defender)
{
	u8 holdEffect, moveAcc = gBattleMoves[move].accuracy;
	u8 accuracyStatStages = gBattleMons[attacker].statStages[STAT_ACC], evasionStatStages = gBattleMons[defender].statStages[STAT_EVASION];
	u16 holdEffectParam, atkAbility = GetBattlerAbility(attacker), defAbility = GetBattlerAbility(defender);
	u32 totalAccuracy;
	
	// Check effects that ignore the user's accuracy stat stages
	if (defAbility == ABILITY_UNAWARE)
		accuracyStatStages = DEFAULT_STAT_STAGES; // Set accuracy stages to default
	
	// Check effects that ignore the target's evasion stat stages
	if (atkAbility == ABILITY_KEEN_EYE || atkAbility == ABILITY_UNAWARE || atkAbility == ABILITY_ILLUMINATE || atkAbility == ABILITY_MINDS_EYE
	|| ((gBattleMons[defender].status2 & (STATUS2_FORESIGHT | STATUS2_MIRACLE_EYE)) && evasionStatStages > DEFAULT_STAT_STAGES))
		evasionStatStages = DEFAULT_STAT_STAGES; // Set evasion stages to default
	else
		evasionStatStages = accuracyStatStages + DEFAULT_STAT_STAGES - evasionStatStages;
	
	evasionStatStages = min(MAX_STAT_STAGES, evasionStatStages); // Clamp evasion stages to the max
	
	// Check effects that alter the move's accuracy
	if ((gBattleMoves[move].effect == EFFECT_NEVER_MISS_IN_WEATHER && IsBattlerWeatherAffected(attacker, gBattleMoves[move].argument.neverMissInWeather.debuffWeather))
	|| (defAbility == ABILITY_WONDER_SKIN && IS_MOVE_STATUS(move) && moveAcc > 50))
		moveAcc = 50;
	
	// Apply evasion stat stages into the move's accuracy
	totalAccuracy = (sAccuracyStageRatios[evasionStatStages].dividend * moveAcc) / sAccuracyStageRatios[evasionStatStages].divisor;
	
	// Check effects that increases/decreases the move's total accuracy
	// Check attacker's ability
	switch (atkAbility)
	{
		case ABILITY_COMPOUND_EYES:
		    totalAccuracy = (totalAccuracy * 130) / 100; // 1.3 boost
			break;
		case ABILITY_VICTORY_STAR:
		    totalAccuracy = (totalAccuracy * 110) / 100; // 1.1 boost
			break;
		case ABILITY_HUSTLE:
		    if (IS_MOVE_PHYSICAL(move))
				totalAccuracy = (totalAccuracy * 80) / 100; // 1.2 loss
			break;
	}
	// Check attacker's ally ability
	if (IsBattlerAlive(BATTLE_PARTNER(attacker)))
	{
		switch (GetBattlerAbility(BATTLE_PARTNER(attacker)))
		{
			case ABILITY_VICTORY_STAR:
			    totalAccuracy = (totalAccuracy * 110) / 100; // 1.1 boost
				break;
		}
	}
	// Check defender's ability
	switch (defAbility)
	{
		case ABILITY_TANGLED_FEET:
		    if (gBattleMons[defender].status2 & STATUS2_CONFUSION)
				totalAccuracy /= 2; // 1/2 loss
			break;
		case ABILITY_SAND_VEIL:
		    if (IsBattlerWeatherAffected(defender, B_WEATHER_SANDSTORM))
				totalAccuracy = (totalAccuracy * 80) / 100; // 1.2 loss
			break;
		case ABILITY_SNOW_CLOAK:
		    if (IsBattlerWeatherAffected(defender, B_WEATHER_HAIL))
				totalAccuracy = (totalAccuracy * 80) / 100; // 1.2 loss
			break;
	}
	// Check defender's items
	holdEffect = GetBattlerItemHoldEffect(defender, TRUE);
	holdEffectParam = ItemId_GetHoldEffectParam(gBattleMons[defender].item);
	
	switch (holdEffect)
	{
		case HOLD_EFFECT_EVASION_UP:
		    totalAccuracy = (totalAccuracy * (100 - holdEffectParam)) / 100;
			break;
	}
	
	// General effects
	if (IsBattlerWeatherAffected(attacker, B_WEATHER_FOG))
		totalAccuracy = (totalAccuracy * 60) / 100; // 0.6 loss
	
	if (gFieldStatus & STATUS_FIELD_GRAVITY)
		totalAccuracy = (totalAccuracy * 5) / 3; // 1.66 boost
	
	return totalAccuracy;
}

static void atk01_accuracycheck(void)
{
	CMD_ARGS(const u8 *jumpStr, bool8 noAccCal);
	
	// Poke Dude and first battle moves never misses in certain states
    if ((gBattleTypeFlags & BATTLE_TYPE_POKEDUDE) || ((gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE) && !BtlCtrl_OakOldMan_TestState2Flag(FIRST_BATTLE_MSG_FLAG_INFLICT_DMG) 
	&& !IS_MOVE_STATUS(gCurrentMove) && GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER) || ((gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE)
	&& !BtlCtrl_OakOldMan_TestState2Flag(FIRST_BATTLE_MSG_FLAG_STAT_CHG) && IS_MOVE_STATUS(gCurrentMove) && GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER))
        JumpIfMoveFailed(6, cmd->jumpStr);
    else if (cmd->noAccCal)
    {
		if (gStatuses3[gBattlerTarget] & STATUS3_COMMANDING)
			gBattlescriptCurrInstr = cmd->jumpStr;
        else if ((gStatuses3[gBattlerAttacker] & STATUS3_ALWAYS_HITS) && gDisableStructs[gBattlerAttacker].battlerWithSureHit == gBattlerTarget)
            gBattlescriptCurrInstr = cmd->nextInstr; // Only checks for Lock On, no acc calc
        else if (gStatuses3[gBattlerTarget] & STATUS3_SEMI_INVULNERABLE) // Check semi-invulnerable
            gBattlescriptCurrInstr = cmd->jumpStr;
        else if (!JumpIfMoveAffectedByProtect(6, cmd->jumpStr)) // Check Protect
            gBattlescriptCurrInstr = cmd->nextInstr;
    }
	else if (gSpecialStatuses[gBattlerAttacker].parentalBondState == PARENTAL_BOND_2ND_HIT || (gSpecialStatuses[gBattlerAttacker].multiHitOn
	&& (gBattleMoves[gCurrentMove].effect != EFFECT_TRIPLE_KICK || GetBattlerAbility(gBattlerAttacker) == ABILITY_SKILL_LINK))) // Check Skill Link on multi-hit moves
		gBattlescriptCurrInstr = cmd->nextInstr;
	else if (JumpIfMoveAffectedByProtect(6, cmd->jumpStr) || AccuracyCalcHelper(cmd->jumpStr)) // Check Protect and effects that cause the move to aways hit
	    return;
    else
    {
		if ((RandomMax(100) + 1) > CalcMoveTotalAccuracy(gCurrentMove, gBattlerAttacker, gBattlerTarget)) // final calculation, determines if the move misses
		{
			u16 flags;
			u8 moveTarget = GetBattlerMoveTargetType(gBattlerAttacker, gCurrentMove);
			
			gMoveResultFlags |= MOVE_RESULT_MISSED;
			
			if (IsDoubleBattleForBattler(gBattlerTarget) && (moveTarget == MOVE_TARGET_BOTH || moveTarget == MOVE_TARGET_FOES_AND_ALLY))
				gBattleCommunication[MISS_TYPE] = B_MSG_AVOIDED_ATK;
			else
				gBattleCommunication[MISS_TYPE] = B_MSG_MISSED;
			
			if (!IS_MOVE_STATUS(gCurrentMove) && TypeCalc(gCurrentMove, gBattleStruct->dynamicMoveType, gBattlerAttacker, gBattlerTarget, TRUE, &flags) == TYPE_MUL_NO_EFFECT)
			{
				gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
				gProtectStructs[gBattlerAttacker].targetNotAffected = TRUE;
			}
		}
		JumpIfMoveFailed(6, cmd->jumpStr);
    }
}

static void atk02_attackstring(void)
{
	CMD_ARGS();
	
    if (!gBattleControllerExecFlags)
    {
        if (!(gHitMarker & (HITMARKER_NO_ATTACKSTRING | HITMARKER_ATTACKSTRING_PRINTED)))
        {
			BattleAI_RecordMoveUsed(gBattlerAttacker, FindMoveSlotInBattlerMoveset(gBattlerAttacker, gCurrentMove));
            PrepareStringBattle(STRINGID_USEDMOVE, gBattlerAttacker);
            gHitMarker |= HITMARKER_ATTACKSTRING_PRINTED;
        }
        gBattleCommunication[MSG_DISPLAY] = FALSE;
		gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atk03_ppreduce(void)
{
	CMD_ARGS();
	
    u8 i, ppToDeduct, atkSide, moveTarget;

    if (!gBattleControllerExecFlags)
    {
		atkSide = GetBattlerSide(gBattlerAttacker);
		moveTarget = GetBattlerMoveTargetType(gBattlerAttacker, gCurrentMove);
		ppToDeduct = 1;
		
		// Check Pressure PP usage
		if (gBattleMoves[gCurrentMove].flags.forcePressure || moveTarget == MOVE_TARGET_BOTH || moveTarget == MOVE_TARGET_FOES_AND_ALLY || moveTarget == MOVE_TARGET_OPPONENTS)
		{
			for (i = 0; i < gBattlersCount; i++)
			{
				if (IsBattlerAlive(i) && atkSide != GetBattlerSide(i) && GetBattlerAbility(i) == ABILITY_PRESSURE) // Can't affect ally's moves
					++ppToDeduct;
			}
		}
		else if (moveTarget != MOVE_TARGET_OPPONENTS_FIELD && atkSide != GetBattlerSide(gBattlerTarget) && GetBattlerAbility(gBattlerTarget) == ABILITY_PRESSURE)
			++ppToDeduct;
		
		// Decreases the PP
        if (!(gHitMarker & (HITMARKER_NO_PPDEDUCT | HITMARKER_NO_ATTACKSTRING)) && (gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS)
		&& gBattleMons[gBattlerAttacker].pp[gCurrMovePos])
        {
            if (gBattleMons[gBattlerAttacker].pp[gCurrMovePos] > ppToDeduct)
                gBattleMons[gBattlerAttacker].pp[gCurrMovePos] -= ppToDeduct;
            else
                gBattleMons[gBattlerAttacker].pp[gCurrMovePos] = 0;
			
			gProtectStructs[gBattlerAttacker].notFirstStrike = 1;
		
            if (MOVE_IS_PERMANENT(gBattlerAttacker, gCurrMovePos))
            {
                BtlController_EmitSetMonData(gBattlerAttacker, BUFFER_A, REQUEST_PPMOVE1_BATTLE + gCurrMovePos, 0, sizeof(gBattleMons[gBattlerAttacker].pp[gCurrMovePos]), &gBattleMons[gBattlerAttacker].pp[gCurrMovePos]);
                MarkBattlerForControllerExec(gBattlerAttacker);
            }
        }
        gHitMarker &= ~(HITMARKER_NO_PPDEDUCT);
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atk04_critcalc(void)
{
	CMD_ARGS();
	
	if ((gBattleTypeFlags & (BATTLE_TYPE_OLD_MAN_TUTORIAL | BATTLE_TYPE_POKEDUDE)) || ((gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE) && !BtlCtrl_OakOldMan_TestState2Flag(FIRST_BATTLE_MSG_FLAG_INFLICT_DMG)))
		gIsCriticalHit = FALSE;
	else
	{
		gIsCriticalHit = CalcMoveIsCritical(gBattlerAttacker, gBattlerTarget, gCurrentMove);
		
		if (gIsCriticalHit)
		{
			// counter for EVO_CRITICAL_HITS
			if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER && !(gBattleTypeFlags & (BATTLE_TYPE_FIRST_BATTLE | BATTLE_TYPE_LINK | BATTLE_TYPE_MULTI))
			&& gPartyCriticalHits[gBattlerPartyIndexes[gBattlerAttacker]] < 255)
				++gPartyCriticalHits[gBattlerPartyIndexes[gBattlerAttacker]];
			
			// Signal for trainer slide
			if (GetBattlerSide(gBattlerTarget) == B_SIDE_OPPONENT && gBattleStruct->firstCritcalHitTakenMsgState == 0)
				++gBattleStruct->firstCritcalHitTakenMsgState;
		}
	}
	gBattlescriptCurrInstr = cmd->nextInstr;
}

bool8 CalcMoveIsCritical(u8 battlerAtk, u8 battlerDef, u16 move)
{
	u16 critStage, atkAbility = GetBattlerAbility(battlerAtk), defAbility = GetBattlerAbility(battlerDef);
	
	// Effects that block critical hits
	if (defAbility != ABILITY_BATTLE_ARMOR && defAbility != ABILITY_SHELL_ARMOR && !(gSideStatuses[GetBattlerSide(battlerDef)] & SIDE_STATUS_LUCKY_CHANT))
	{
		// Effects that cause the move to aways be critical
		if (atkAbility == ABILITY_MERCILESS && (gBattleMons[battlerDef].status1.id == STATUS1_POISON || gBattleMons[battlerDef].status1.id == STATUS1_TOXIC_POISON))
			return TRUE;
		
		// Calc move's crit stages
		critStage = gBattleMoves[move].flags.critStage;
		
		// Check battle effects
		if (gBattleMons[battlerAtk].status2 & STATUS2_FOCUS_ENERGY)
			critStage += 2;
		
		// Check attacker's ability
		switch (atkAbility)
		{
			case ABILITY_SUPER_LUCK:
				++critStage;
				break;
		}
		
		// Check attacker's item
		switch (GetBattlerItemHoldEffect(battlerAtk, TRUE))
		{
			case HOLD_EFFECT_SCOPE_LENS:
				++critStage;
				break;
			case HOLD_EFFECT_LUCKY_PUNCH:
				if (SpeciesToNationalPokedexNum(gBattleMons[battlerAtk].species) == NATIONAL_DEX_CHANSEY)
					critStage += 2;
				break;
			case HOLD_EFFECT_STICK:
				if (SpeciesToNationalPokedexNum(gBattleMons[battlerAtk].species) == NATIONAL_DEX_FARFETCHD
				|| SpeciesToNationalPokedexNum(gBattleMons[battlerAtk].species) == NATIONAL_DEX_SIRFETCHD)
					critStage += 2;
				break;
		}
		
		// Crit chance can't be higer than table's lenght
		if (critStage >= ARRAY_COUNT(sCriticalHitChance))
			critStage = ARRAY_COUNT(sCriticalHitChance) - 1;
		
		if (!(RandomMax(sCriticalHitChance[critStage])))
			return TRUE;
	}
	return FALSE;
}

static void atk05_damagecalc(void)
{
	CMD_ARGS();
    gBattleMoveDamage = CalculateMoveDamage(gCurrentMove, gBattleStruct->dynamicMoveType, gBattlerAttacker, gBattlerTarget, gIsCriticalHit);
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk06_typecalc(void)
{
	CMD_ARGS();
	
	// Calculate type effectiveness
	TypeCalc(gCurrentMove, gBattleStruct->dynamicMoveType, gBattlerAttacker, gBattlerTarget, TRUE, &gMoveResultFlags);
	
	if (gMoveResultFlags & MOVE_RESULT_MISSED)
	{
		gBattleStruct->battlers[gBattlerTarget].lastLandedMove = 0;
		gBattleStruct->battlers[gBattlerTarget].lastHitMoveType = 0;
	}
	
	if (gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE)
		gProtectStructs[gBattlerAttacker].targetNotAffected = TRUE;
	
    gBattlescriptCurrInstr = cmd->nextInstr;
}

bool8 SubsBlockMove(u8 attacker, u8 defender, u16 move) // Check if substitute can block the move
{
	if ((!(gBattleMons[defender].status2 & STATUS2_SUBSTITUTE) || gBattleMoves[move].flags.soundMove || (GetBattlerAbility(attacker) == ABILITY_INFILTRATOR
	&& gBattleMoves[move].effect != EFFECT_TRANSFORM && gBattleMoves[move].effect != EFFECT_SKY_DROP)))
		return FALSE;
	return TRUE;
}

u8 GetHitDamageResult(u8 battlerId, u16 move, bool8 checkSturdy)
{
	if (gBattleMoves[move].effect == EFFECT_FALSE_SWIPE)
		return 1;
	else if (gProtectStructs[battlerId].endured)
		return 2;
	else if (GetBattlerItemHoldEffect(battlerId, TRUE) == HOLD_EFFECT_FOCUS_BAND && RandomPercent(ItemId_GetHoldEffectParam(gBattleMons[battlerId].item)))
		return 3;
	else if (checkSturdy && GetBattlerAbility(battlerId) == ABILITY_STURDY && BATTLER_MAX_HP(battlerId))
		return 4;
	
	return 0;
}

static bool8 SetHitDamageResult(u8 battlerId, u16 move, bool8 checkSturdy)
{
	bool8 survive = TRUE;
	
	switch (GetHitDamageResult(battlerId, move, checkSturdy))
	{
		case 0:
			survive = FALSE;
			break;
		case 1: // False Swipe
			break;
		case 2: // Endure
			gMoveResultFlags |= MOVE_RESULT_FOE_ENDURED;
			break;
		case 3: // Focus Band
			gSpecialStatuses[battlerId].focusBanded = TRUE;
			gMoveResultFlags |= MOVE_RESULT_FOE_HUNG_ON;
			gLastUsedItem = gBattleMons[battlerId].item;
			break;
		case 4: // Sturdy
			gMoveResultFlags |= MOVE_RESULT_FOE_STURDIED;
			break;
	}
	return survive;
}

static bool8 TryDisplayDamageChangeMessage(void)
{
	// Try display strong winds weakened attack message
	if (GetBattlerSide(gBattlerAttacker) != GetBattlerSide(gBattlerTarget) && gBattleStruct->strongWindsMessageState == 1)
	{
		++gBattleStruct->strongWindsMessageState;
		BattleScriptCall(BattleScript_StrongWindsWeakenedAttack);
		return TRUE;
	}
	return FALSE;
}

static void atk07_adjustdamage(void) // Check for effects that prevent the target from faint
{
	CMD_ARGS();
	
    if (gBattleMons[gBattlerTarget].hp <= gBattleMoveDamage && !SubsBlockMove(gBattlerAttacker, gBattlerTarget, gCurrentMove))
	{
		if (SetHitDamageResult(gBattlerTarget, gCurrentMove, TRUE))
			gBattleMoveDamage = gBattleMons[gBattlerTarget].hp - 1;
	}
	
	if (!TryDisplayDamageChangeMessage())
		gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk08_setstatchanger(void)
{
	CMD_ARGS(u8 statId, s8 buff);
	SetStatChanger(cmd->statId, cmd->buff);
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk09_attackanimation(void)
{
	CMD_ARGS();
	
    if (!gBattleControllerExecFlags)
    {
		gBattlescriptCurrInstr = cmd->nextInstr;
		
        if (!IsBattleAnimationsOn() && gBattleMoves[gCurrentMove].effect != EFFECT_TRANSFORM && gBattleMoves[gCurrentMove].effect != EFFECT_SUBSTITUTE)
        {
			BattleScriptCall(BattleScript_Pausex20);
            ++gBattleScripting.animTurn;
            ++gBattleScripting.animTargetsHit;
        }
        else
        {
			u8 moveTarget = GetBattlerMoveTargetType(gBattlerAttacker, gCurrentMove);
			
            if (((moveTarget == MOVE_TARGET_BOTH || moveTarget == MOVE_TARGET_FOES_AND_ALLY || moveTarget == MOVE_TARGET_DEPENDS)
				&& gBattleScripting.animTargetsHit) || gSpecialStatuses[gBattlerAttacker].parentalBondState == PARENTAL_BOND_2ND_HIT)
                return;
			
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
            {
                BtlController_EmitMoveAnimation(gBattlerAttacker, BUFFER_A, gCurrentMove, gBattleScripting.animTurn, gBattleMovePower, gBattleMoveDamage, gBattleMons[gBattlerAttacker].friendship, &gDisableStructs[gBattlerAttacker]);
                MarkBattlerForControllerExec(gBattlerAttacker);
				
				++gBattleScripting.animTurn;
                ++gBattleScripting.animTargetsHit;
            }
            else
				BattleScriptCall(BattleScript_Pausex20);
        }
    }
}

static void atk0A_lunardancepprestore(void)
{
	CMD_ARGS(u8 battler);

	if (!gBattleControllerExecFlags)
	{
		u8 i, battlerId = GetBattlerForBattleScript(cmd->battler);
		
		for (i = 0; i < MAX_MON_MOVES; i++)
		{
			u16 move = gBattleMons[battlerId].moves[i];
			
			if (move)
			{
				u8 maxPP = CalculatePPWithBonus(move, gBattleMons[battlerId].ppBonuses, i);
				
				if ((gBattleMons[battlerId].status2 & STATUS2_TRANSFORMED) && maxPP > 5)
					maxPP = 5;
				
				if (gBattleMons[battlerId].pp[i] != maxPP)
				{
					gBattleMons[battlerId].pp[i] = maxPP;
					BtlController_EmitSetMonData(battlerId, BUFFER_A, REQUEST_PPMOVE1_BATTLE + i, 0, sizeof(gBattleMons[battlerId].pp[i]), &maxPP);
					MarkBattlerForControllerExec(battlerId);
					return;
				}
		    }
		}
		gBattlescriptCurrInstr = cmd->nextInstr;
	}
}

static bool8 CanBattlerIceFaceBlockMove(u8 battlerId, u16 move)
{
	if ((!(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE) || gProtectStructs[battlerId].confusionSelfDmg) && !(gBattleMons[battlerId].status2 & STATUS2_TRANSFORMED)
		&& gBattleMons[battlerId].species == SPECIES_EISCUE && IS_MOVE_PHYSICAL(move) && GetBattlerAbility(battlerId) == ABILITY_ICE_FACE)
	    return TRUE;
	return FALSE;
}

static bool8 BattlerIsDisguised(u8 battlerId)
{
	if (!(gHitMarker & HITMARKER_IGNORE_DISGUISE) && !(gBattleMons[battlerId].status2 & STATUS2_TRANSFORMED)
		&& gBattleMons[battlerId].species == SPECIES_MIMIKYU && GetBattlerAbility(battlerId) == ABILITY_DISGUISE)
		return TRUE;
	return FALSE;
}

static void atk0B_healthbarupdate(void)
{
	CMD_ARGS(u8 battler);
	
    if (!gBattleControllerExecFlags)
    {
        if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
        {
            u8 battlerId = GetBattlerForBattleScript(cmd->battler);
			
			// Check effects that blocks the move
			if (!(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE) && SubsBlockMove(gBattlerAttacker, battlerId, gCurrentMove) && gDisableStructs[battlerId].substituteHP)
				PrepareStringBattle(STRINGID_SUBSTITUTEDAMAGED, battlerId); // Hit the substitute
            else if (!CanBattlerIceFaceBlockMove(battlerId, gCurrentMove) && !BattlerIsDisguised(battlerId)) // Skip the health bar update
            {
                s32 maxPossibleDmgValue = 10000; // Not present in R/S, ensures that huge damage values don't change sign
				
				// gBattleMoveDamage can't be higher than maxPossibleDmgValue
                BtlController_EmitHealthBarUpdate(battlerId, BUFFER_A, gBattleMoveDamage <= maxPossibleDmgValue ? gBattleMoveDamage : maxPossibleDmgValue);
                MarkBattlerForControllerExec(battlerId);
            }
        }
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void UpdateDamageVars(u8 battlerId, u8 didDamageBattlerId)
{
	if (IS_MOVE_PHYSICAL(gCurrentMove))
	{
		gProtectStructs[battlerId].physicalDmg = gHpDealt;
		gSpecialStatuses[battlerId].physicalDmg = gHpDealt;
		gProtectStructs[battlerId].physicalBattlerId = didDamageBattlerId;
		gSpecialStatuses[battlerId].physicalBattlerId = didDamageBattlerId;
	}
	else
	{
		gProtectStructs[battlerId].specialDmg = gHpDealt;
		gSpecialStatuses[battlerId].specialDmg = gHpDealt;
		gProtectStructs[battlerId].specialBattlerId = didDamageBattlerId;
		gSpecialStatuses[battlerId].specialBattlerId = didDamageBattlerId;
	}
}

static void atk0C_datahpupdate(void)
{
	CMD_ARGS(u8 battler);
	
    if (!gBattleControllerExecFlags)
    {
		u8 battlerId = GetBattlerForBattleScript(cmd->battler);
	    
        if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
        {
			if (!(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE) && SubsBlockMove(gBattlerAttacker, battlerId, gCurrentMove)
			&& gDisableStructs[battlerId].substituteHP) // Check Substitute
			{
				if (gDisableStructs[battlerId].substituteHP >= gBattleMoveDamage)
				{
					gDisableStructs[battlerId].substituteHP -= gBattleMoveDamage;
					
					if (gSpecialStatuses[battlerId].dmg == 0)
						gSpecialStatuses[battlerId].dmg = gBattleMoveDamage;
					
					gHpDealt = gBattleMoveDamage;
				}
				else
				{
					if (gSpecialStatuses[battlerId].dmg == 0)
						gSpecialStatuses[battlerId].dmg = gDisableStructs[battlerId].substituteHP;
					
					gHpDealt = gDisableStructs[battlerId].substituteHP;
					gDisableStructs[battlerId].substituteHP = 0;
				}
				
				// Check substitute fading
				if (gDisableStructs[battlerId].substituteHP == 0)
				{
					gBattlescriptCurrInstr = cmd->nextInstr;
					BattleScriptCall(BattleScript_SubstituteFade);
					return;
				}
			}
			else if (BattlerIsDisguised(battlerId)) // Check Disguise
			{
				gBattleMoveDamage = gBattleMons[battlerId].maxHP / 8;
				if (gBattleMoveDamage == 0)
					gBattleMoveDamage = 1;

				gHpDealt = gBattleMoveDamage;

				if (gSpecialStatuses[battlerId].dmg == 0)
					gSpecialStatuses[battlerId].dmg = 1;

				gMoveResultFlags = 0;
				
				UpdateDamageVars(battlerId, gBattlerAttacker);

				DoBattleFormChange(battlerId, SPECIES_MIMIKYU_BUSTED, FALSE, FALSE, FALSE);

				gBattlescriptCurrInstr = cmd->nextInstr;
				BattleScriptCall(BattleScript_DisguiseBusted);
				return;
			}
			else if (CanBattlerIceFaceBlockMove(battlerId, gCurrentMove)) // Check Ice Face
			{
				if (gSpecialStatuses[battlerId].dmg == 0)
					gSpecialStatuses[battlerId].dmg = 1;

				gHpDealt = 1;
				gMoveResultFlags = 0;

				UpdateDamageVars(battlerId, gBattlerAttacker);

				gBattleScripting.battler = battlerId;
				DoBattleFormChange(battlerId, SPECIES_EISCUE_NOICE_FACE, FALSE, TRUE, FALSE);

				gBattlescriptCurrInstr = cmd->nextInstr;
				BattleScriptCall(BattleScript_IceFaceFade);
				return;
			}
            else
            {
                gHitMarker &= ~(HITMARKER_IGNORE_SUBSTITUTE | HITMARKER_IGNORE_DISGUISE);
				
                if (gBattleMoveDamage < 0) // HP goes up
                {
                    gBattleMons[battlerId].hp -= gBattleMoveDamage;
                    if (gBattleMons[battlerId].hp > gBattleMons[battlerId].maxHP)
                        gBattleMons[battlerId].hp = gBattleMons[battlerId].maxHP;
                }
                else // HP goes down
                {
					if (gHitMarker & HITMARKER_SKIP_DMG_TRACK)
                        gHitMarker &= ~(HITMARKER_SKIP_DMG_TRACK);
                    else
                    {
                        gBattleStruct->battlers[battlerId].bideTakenDamage += gBattleMoveDamage;
						gBattleStruct->battlers[battlerId].bideTakenDamageBattler = cmd->battler == BS_TARGET ? gBattlerAttacker : gBattlerTarget;
                    }

                    if (gBattleMons[battlerId].hp > gBattleMoveDamage)
                    {
                        gBattleMons[battlerId].hp -= gBattleMoveDamage;
                        gHpDealt = gBattleMoveDamage;
						
						if (gHpDealt == 0 && gDisableStructs[battlerId].enduredHit)
						{
							gDisableStructs[battlerId].enduredHit = FALSE;
							gHpDealt = 1;
						}
                    }
                    else
                    {
                        gHpDealt = gBattleMons[battlerId].hp;
                        gBattleMons[battlerId].hp = 0;
                    }
					
					if (!(gHitMarker & HITMARKER_PASSIVE_DAMAGE))
					{
						if (!gSpecialStatuses[battlerId].dmg)
							gSpecialStatuses[battlerId].dmg = gHpDealt;
						
						UpdateDamageVars(battlerId, cmd->battler == BS_TARGET ? gBattlerAttacker : gBattlerTarget);
					}
                }
                gHitMarker &= ~(HITMARKER_PASSIVE_DAMAGE);
                BtlController_EmitSetMonData(battlerId, BUFFER_A, REQUEST_HP_BATTLE, 0, sizeof(gBattleMons[battlerId].hp), &gBattleMons[battlerId].hp);
                MarkBattlerForControllerExec(battlerId);
            }
        }
        else
        {
            if (gSpecialStatuses[battlerId].dmg == 0)
                gSpecialStatuses[battlerId].dmg = 0xFFFF;
        }
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atk0D_critmessage(void)
{
	CMD_ARGS();

    if (!gBattleControllerExecFlags)
    {
        if (gIsCriticalHit && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
        {
            PrepareStringBattle(DoesSpreadMoveStrikesOnlyOnce(gBattlerAttacker, gBattlerTarget, gCurrentMove, TRUE) ? STRINGID_CRITICALHIT : STRINGID_CRITICALHITONDEF, gBattlerAttacker);
            gBattleCommunication[MSG_DISPLAY] = TRUE;
        }
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atk0E_effectivenesssound(void)
{
	CMD_ARGS();

    if (!gBattleControllerExecFlags)
    {
        if (!(gMoveResultFlags & MOVE_RESULT_MISSED))
        {
            switch (gMoveResultFlags & ~(MOVE_RESULT_MISSED))
            {
				case MOVE_RESULT_SUPER_EFFECTIVE:
				    BtlController_EmitPlaySE(gBattlerTarget, BUFFER_A, SE_SUPER_EFFECTIVE);
					MarkBattlerForControllerExec(gBattlerTarget);
					break;
				case MOVE_RESULT_NOT_VERY_EFFECTIVE:
                    BtlController_EmitPlaySE(gBattlerTarget, BUFFER_A, SE_NOT_EFFECTIVE);
					MarkBattlerForControllerExec(gBattlerTarget);
					break;
				case MOVE_RESULT_DOESNT_AFFECT_FOE:
				case MOVE_RESULT_FAILED: // no sound
                    break;
				default:
				    if (gMoveResultFlags & MOVE_RESULT_SUPER_EFFECTIVE)
					{
						BtlController_EmitPlaySE(gBattlerTarget, BUFFER_A, SE_SUPER_EFFECTIVE);
						MarkBattlerForControllerExec(gBattlerTarget);
					}
					else if (gMoveResultFlags & MOVE_RESULT_NOT_VERY_EFFECTIVE)
					{
						BtlController_EmitPlaySE(gBattlerTarget, BUFFER_A, SE_NOT_EFFECTIVE);
						MarkBattlerForControllerExec(gBattlerTarget);
					}
					else if (!(gMoveResultFlags & (MOVE_RESULT_DOESNT_AFFECT_FOE | MOVE_RESULT_FAILED)))
					{
						BtlController_EmitPlaySE(gBattlerTarget, BUFFER_A, SE_EFFECTIVE);
						MarkBattlerForControllerExec(gBattlerTarget);
					}
					break;
            }
        }
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

#define DO_EFFECTIVENESS_MESSAGE ((gBattleStruct->pursuitSwitchDmg || !gMultiHitCounter))

static void atk0F_resultmessage(void)
{
    CMD_ARGS();

    if (!gBattleControllerExecFlags)
    {
		u16 stringId = 0;
		
		gBattleCommunication[MSG_DISPLAY] = TRUE;
		
        if ((gMoveResultFlags & MOVE_RESULT_MISSED) && (!(gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE) || gBattleCommunication[MISS_TYPE] == B_MSG_ABILITY_AVOID))
		{
			if (gBattleCommunication[MISS_TYPE] == B_MSG_ABILITY_AVOID) // Levitate/Wonder Guard
			{
				gBattlescriptCurrInstr = cmd->nextInstr;
				BattleScriptCall(BattleScript_AvoidMoveWithAbility);
				gBattleCommunication[MSG_DISPLAY] = FALSE;
				return;
			}
			else
				stringId = gMissStringIds[gBattleCommunication[MISS_TYPE]];
		}
        else
        {
            switch (gMoveResultFlags & ~(MOVE_RESULT_MISSED))
            {
				case MOVE_RESULT_SUPER_EFFECTIVE:
				    if (DO_EFFECTIVENESS_MESSAGE) // Don't print effectiveness on each hit in a multi hit attack
					{
						stringId = DoesSpreadMoveStrikesOnlyOnce(gBattlerAttacker, gBattlerTarget, gCurrentMove, TRUE) ? STRINGID_SUPEREFFECTIVE : STRINGID_SUPEREFFECTIVEONDEF;
						
						// Signal for trainer slide
						if (GetBattlerSide(gBattlerTarget) != B_SIDE_PLAYER && gBattleStruct->firstSuperEffectiveHitTakenMsgState == 0)
							++gBattleStruct->firstSuperEffectiveHitTakenMsgState;
						
						gSpecialStatuses[gBattlerTarget].hitBySuperEffective = TRUE; // For sos battles
					}
					break;
				case MOVE_RESULT_NOT_VERY_EFFECTIVE:
				    if (DO_EFFECTIVENESS_MESSAGE)
						stringId = DoesSpreadMoveStrikesOnlyOnce(gBattlerAttacker, gBattlerTarget, gCurrentMove, TRUE) ? STRINGID_NOTVERYEFFECTIVE : STRINGID_NOTVERYEFFECTIVEONDEF;
					break;
				case MOVE_RESULT_ONE_HIT_KO:
				    stringId = STRINGID_ONEHITKO;
					break;
				case MOVE_RESULT_FOE_ENDURED:
				    gDisableStructs[gBattlerTarget].enduredHit = TRUE; // So effects that require damage can activate
				    stringId = STRINGID_DEFENDUREDHIT;
					break;
				case MOVE_RESULT_FAILED:
				    stringId = STRINGID_BUTITFAILED;
					break;
				case MOVE_RESULT_DOESNT_AFFECT_FOE:
				    stringId = STRINGID_ITDOESNTAFFECT;
					break;
				case MOVE_RESULT_FOE_HUNG_ON:
				    gLastUsedItem = gBattleMons[gBattlerTarget].item;
					gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
					BattleScriptCall(BattleScript_HangedOnMsg);
					return;
				default:
				    if (gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE)
						stringId = STRINGID_ITDOESNTAFFECT;
					else if (gMoveResultFlags & MOVE_RESULT_ONE_HIT_KO)
					{
						gMoveResultFlags &= ~(MOVE_RESULT_ONE_HIT_KO | MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE);
						BattleScriptCall(BattleScript_OneHitKOMsg);
						return;
					}
					else if (gMoveResultFlags & MOVE_RESULT_FOE_STURDIED)
					{
						gMoveResultFlags &= ~(MOVE_RESULT_FOE_STURDIED | MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
						BattleScriptCall(BattleScript_SturdiedMsg);
						return;
					}
					else if (gMoveResultFlags & MOVE_RESULT_FOE_ENDURED)
					{
						gDisableStructs[gBattlerTarget].enduredHit = TRUE; // So effects that require damage can activate
						gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
						BattleScriptCall(BattleScript_EnduredMsg);
						return;
					}
					else if (gMoveResultFlags & MOVE_RESULT_FOE_HUNG_ON)
					{
						gLastUsedItem = gBattleMons[gBattlerTarget].item;
						gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
						BattleScriptCall(BattleScript_HangedOnMsg);
						return;
					}
					else if (gMoveResultFlags & MOVE_RESULT_FAILED)
						stringId = STRINGID_BUTITFAILED;
					else
						gBattleCommunication[MSG_DISPLAY] = FALSE;
            }
        }
        if (stringId)
            PrepareStringBattle(stringId, gBattlerAttacker);
		
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atk10_printstring(void)
{
	CMD_ARGS(u16 stringId);

    if (!gBattleControllerExecFlags)
    {
        u16 stringId = cmd->stringId;
        gBattlescriptCurrInstr = cmd->nextInstr;
        PrepareStringBattle(stringId, gBattlerAttacker);
        gBattleCommunication[MSG_DISPLAY] = TRUE;
    }
}

static void atk11_tryacupressure(void)
{
	CMD_ARGS(const u8 *failStr);
	
	if (gBattlerTarget != gBattlerAttacker && (SubsBlockMove(gBattlerAttacker, gBattlerTarget, gCurrentMove)
	|| (gSideStatuses[GetBattlerSide(gBattlerTarget)] & SIDE_STATUS_CRAFTY_SHIELD)))
		gBattlescriptCurrInstr = cmd->failStr;
	else
	{
		u8 statId;
		u32 bits = 0;

		for (statId = STAT_ATK; statId < NUM_BATTLE_STATS; statId++)
		{
			if (CompareStat(gBattlerTarget, statId, MAX_STAT_STAGES, CMP_LESS_THAN))
				bits |= gBitTable[statId];
		}
		
		if (bits)
		{
			do
			{
				statId = RandomRange(STAT_ATK, NUM_BATTLE_STATS - 1);
			}
			while (!(bits & gBitTable[statId]));
			
			SetStatChanger(statId, +2);
			gBattlescriptCurrInstr = cmd->nextInstr;
		}
		else
			gBattlescriptCurrInstr = cmd->failStr;
	}
}

static void atk12_waitmessage(void)
{
	CMD_ARGS(u16 numFrames);

    if (!gBattleControllerExecFlags)
    {
        if (!gBattleCommunication[MSG_DISPLAY])
            gBattlescriptCurrInstr = cmd->nextInstr;
        else if (++gPauseCounterBattle >= cmd->numFrames)
        {
			gPauseCounterBattle = 0;
			gBattleCommunication[MSG_DISPLAY] = FALSE;
			gBattlescriptCurrInstr = cmd->nextInstr;
        }
    }
}

static void atk13_printfromtable(void)
{
	CMD_ARGS(const u16 *ptr);

    if (!gBattleControllerExecFlags)
    {
        const u16 *ptr = cmd->ptr;
        ptr += gBattleCommunication[MULTISTRING_CHOOSER];
		
		gBattlescriptCurrInstr = cmd->nextInstr;
        PrepareStringBattle(*ptr, gBattlerAttacker);
        gBattleCommunication[MSG_DISPLAY] = TRUE;
    }
}

static void atk14_printselectionstringfromtable(void)
{
	CMD_ARGS(const u16 *ptr);

    if (!gBattleControllerExecFlags)
    {
        const u16 *ptr = cmd->ptr;
        ptr += gBattleCommunication[MULTISTRING_CHOOSER];

        BtlController_EmitPrintSelectionString(gBattlerAttacker, BUFFER_A, *ptr);
        MarkBattlerForControllerExec(gBattlerAttacker);
		gBattleCommunication[MSG_DISPLAY] = TRUE;
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static bool8 CanApplyAdditionalEffect(const struct AdditionalEffect *additionalEffect)
{
	u8 moveTarget = GetBattlerMoveTargetType(gBattlerAttacker, gCurrentMove);
	
	// Self-targeting move effects only apply after the last mon has been hit
	if (additionalEffect->self && IsDoubleBattleForBattler(gBattlerTarget) && (moveTarget == MOVE_TARGET_BOTH || moveTarget == MOVE_TARGET_FOES_AND_ALLY)
	&& GetNextTarget(moveTarget, TRUE) != MAX_BATTLERS_COUNT)
		return FALSE;
		
	// Certain move effects only apply if the target raised stats this turn (e.g. Burning Jealousy)
	/*if (additionalEffect->onlyIfTargetRaisedStats && !gProtectStructs[gBattlerTarget].statRaised)
		return FALSE;*/
	
	// Certain additional effects only apply on a two-turn move's charge turn
	if (additionalEffect->onChargeTurnOnly != gProtectStructs[gBattlerAttacker].chargingTurn)
		return FALSE;
	
	// Certain additional effects only apply on the final hit of a multi-hit move
	if (additionalEffect->onFinalMultiHitOnly && !IS_MULTIHIT_FINAL_STRIKE)
		return FALSE;
	
	// Certain additional effects only apply later on
	if (additionalEffect->afterTargetItemsOnly)
	{
		gBattleStruct->moveEffect2.moveEffectByte = additionalEffect->moveEffect;
		gBattleStruct->moveEffect2.affectsUser = additionalEffect->self;
		gBattleStruct->moveEffect2.certain = TRUE;
		return FALSE;
	}
	return TRUE;
}

static void atk15_setadditionaleffects(void)
{
	CMD_ARGS();
	
	if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && !ReceiveSheerForceBoost(gBattlerAttacker, gCurrentMove))
	{
		if (gBattleMoves[gCurrentMove].numAdditionalEffects > gBattleStruct->moveEffect.additionalEffectsCounter)
		{
			const u8 *savedScriptInstr = gBattlescriptCurrInstr;
			const struct AdditionalEffect *additionalEffect = &gBattleMoves[gCurrentMove].additionalEffects[gBattleStruct->moveEffect.additionalEffectsCounter];
			
			// Various checks for if this move effect can be applied this turn
			if (CanApplyAdditionalEffect(additionalEffect))
			{
				u32 percentChance = CalcSecondaryEffectChance(gBattlerAttacker, additionalEffect->moveEffect, additionalEffect->chance);
				bool8 certain = (percentChance == 0);
				
				// Activate effect if it's certain (chance == 0) or if RNGesus says so
				if (certain || RandomMax(100) <= percentChance)
				{
					if (percentChance >= 100)
						certain = TRUE;
					
					SetMoveEffect(additionalEffect->moveEffect, additionalEffect->self, certain);
					DoMoveEffect(FALSE, cmd->nextInstr, 0);
				}
			}
			// Call setadditionaleffects again in the case of a move with multiple effects
			gBattleStruct->moveEffect.additionalEffectsCounter++;
			
			if (gBattleMoves[gCurrentMove].numAdditionalEffects > gBattleStruct->moveEffect.additionalEffectsCounter)
				gBattleStruct->moveEffect.continueAdditionalEffectsLoop = TRUE;
			else
			{
				gBattleStruct->moveEffect.continueAdditionalEffectsLoop = FALSE;
				gBattleStruct->moveEffect.moveEffectByte = MOVE_EFFECT_NONE;
				gBattleStruct->moveEffect.additionalEffectsCounter = 0;
			}
			
			// Move script along if we haven't jumped elsewhere
			if (savedScriptInstr == gBattlescriptCurrInstr)
				gBattlescriptCurrInstr = cmd->nextInstr;
			
			return;
		}
	}
	gBattleStruct->moveEffect.moveEffectByte = MOVE_EFFECT_NONE;
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk16_setspecialmoveeffect(void)
{
	CMD_ARGS(bool8 primary, u32 flags);
	
	const u8 *savedScriptInstr = gBattlescriptCurrInstr;
	
    DoMoveEffect(cmd->primary, cmd->nextInstr, cmd->flags);
	
	if (savedScriptInstr == gBattlescriptCurrInstr)
		gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk17_setsplitswap(void)
{
	CMD_ARGS();

	u8 i, temp8;
	u16 temp16;
	
	switch (gBattleMoves[gCurrentMove].effect)
	{
		case EFFECT_POWER_TRICK:
		    gStatuses3[gBattlerAttacker] ^= STATUS3_POWER_TRICK;
			SWAP(gBattleMons[gBattlerAttacker].attack, gBattleMons[gBattlerAttacker].defense, temp16);
			PrepareStatBuffer(gBattleTextBuff1, STAT_ATK);
			PrepareStatBuffer(gBattleTextBuff2, STAT_DEF);
			gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SWITCHED_STATS;
			break;
		case EFFECT_POWER_SWAP:
		    SWAP(gBattleMons[gBattlerAttacker].statStages[STAT_ATK], gBattleMons[gBattlerTarget].statStages[STAT_ATK], temp8);
			SWAP(gBattleMons[gBattlerAttacker].statStages[STAT_SPATK], gBattleMons[gBattlerTarget].statStages[STAT_SPATK], temp8);
			PrepareStatBuffer(gBattleTextBuff1, STAT_ATK);
			PrepareStatBuffer(gBattleTextBuff2, STAT_SPATK);
			gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SWITCHED_STAT_CHANGES;
			break;
		case EFFECT_GUARD_SWAP:
		    SWAP(gBattleMons[gBattlerAttacker].statStages[STAT_DEF], gBattleMons[gBattlerTarget].statStages[STAT_DEF], temp8);
			SWAP(gBattleMons[gBattlerAttacker].statStages[STAT_SPDEF], gBattleMons[gBattlerTarget].statStages[STAT_SPDEF], temp8);
			PrepareStatBuffer(gBattleTextBuff1, STAT_DEF);
			PrepareStatBuffer(gBattleTextBuff2, STAT_SPDEF);
			gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SWITCHED_STAT_CHANGES;
			break;
		case EFFECT_HEART_SWAP:
			for (i = STAT_ATK; i < NUM_BATTLE_STATS; i++)
				SWAP(gBattleMons[gBattlerAttacker].statStages[i], gBattleMons[gBattlerTarget].statStages[i], temp8);
			
			gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SWITCHED_ALL_STAT_CHANGES;
			break;
	}
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk18_clearbattlerstatus(void)
{
	CMD_ARGS(u8 battler, u8 statusId, u32 status);

    u8 battlerId = GetBattlerForBattleScript(cmd->battler);
	u32 status = cmd->status;
	
	switch (cmd->statusId)
	{
		case ID_STATUS2:
		    gBattleMons[battlerId].status2 &= ~(status);
			break;
	}
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk19_tryfaintmon(void)
{
	CMD_ARGS(u8 battler);

    u8 attacker, target;
    const u8* BS_ptr;
	
	if (cmd->battler == BS_ATTACKER)
	{
		attacker = gBattlerAttacker;
		target = gBattlerTarget;
		BS_ptr = BattleScript_FaintAttacker;
	}
	else
	{
		attacker = gBattlerTarget;
		target = gBattlerAttacker;
		BS_ptr = BattleScript_FaintTarget;
	}
	
	if (!IsBattlerAlive(attacker))
	{
		gHitMarker |= HITMARKER_FAINTED(attacker);
		gBattlescriptCurrInstr = cmd->nextInstr;
		BattleScriptCall(BS_ptr);
		
		if (GetBattlerSide(attacker) == B_SIDE_PLAYER)
		{
			gHitMarker |= HITMARKER_PLAYER_FAINTED;
			AdjustFriendshipOnBattleFaint(attacker);
		}

		if ((gHitMarker & HITMARKER_DESTINYBOND) && IsBattlerAlive(gBattlerAttacker))
		{
			gHitMarker &= ~(HITMARKER_DESTINYBOND);
			gBattleMoveDamage = gBattleMons[target].hp;
			gBattlescriptCurrInstr = BattleScript_DestinyBondTakesLife;
			BattleScriptCall(BS_ptr);
		}

		if ((gStatuses3[gBattlerTarget] & STATUS3_GRUDGE) && !(gHitMarker & HITMARKER_IGNORE_GRUDGE) && gCurrentMove != MOVE_STRUGGLE && !IsZMove(gCurrentMove)
		&& GetBattlerSide(gBattlerAttacker) != GetBattlerSide(gBattlerTarget) && IsBattlerAlive(gBattlerAttacker))
		{
			u8 moveIndex = gBattleStruct->battlers[gBattlerAttacker].chosenMovePosition;

			PrepareMoveBuffer(gBattleTextBuff1, gBattleMons[gBattlerAttacker].moves[moveIndex]);

			gBattleMons[gBattlerAttacker].pp[moveIndex] = 0;
			BtlController_EmitSetMonData(gBattlerAttacker, BUFFER_A, moveIndex + REQUEST_PPMOVE1_BATTLE, 0, sizeof(gBattleMons[gBattlerAttacker].pp[moveIndex]), &gBattleMons[gBattlerAttacker].pp[moveIndex]);
			MarkBattlerForControllerExec(gBattlerAttacker);

			BattleScriptCall(BattleScript_GrudgeTakesPp);
		}
	}
	else
		gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk1A_jumpifweathercheckchargeeffects(void)
{
	CMD_ARGS(u8 battler, bool8 checkChargeTurnEffects, const u8 *jumpInstr);
	
	/* If this is NOT semi-invulnerable move and we don't have charge turn effects
    yet to fire, we can fire the move right away so long as the weather matches
    the argument and the battler is affected by it (not blocked by Cloud Nine etc) */
	if (!GET_MOVE_MOVEEFFECT_TABLE(gCurrentMove).semiInvulnerableEffect && !(cmd->checkChargeTurnEffects && MoveHasChargeTurnMoveEffect(gCurrentMove))
	&& IsBattlerWeatherAffected(cmd->battler, gBattleMoves[gCurrentMove].argument.twoTurns.weather))
	{
		gBattleScripting.animTurn = 1;
		gBattlescriptCurrInstr = cmd->jumpInstr;
	}
	else
		gBattlescriptCurrInstr = cmd->nextInstr;
}

static void RestoreCommanderBattlerSprite(u8 battlerId)
{
	gStatuses3[battlerId] &= ~(STATUS3_COMMANDING);
	BtlController_EmitSpriteInvisibility(battlerId, BUFFER_A, FALSE);
	MarkBattlerForControllerExec(battlerId);
}

static void atk1B_doeffectsonfaint(void)
{
	CMD_ARGS(u8 battler);

    if (!gBattleControllerExecFlags)
    {
        u8 battlerId = GetBattlerForBattleScript(cmd->battler);
		
		switch (gBattleCommunication[FAINT_EFFECTS_STATE])
		{
			case 0: // Remove Illusion
				TryRemoveIllusion(battlerId);
				++gBattleCommunication[FAINT_EFFECTS_STATE];
				break;
			case 1: // Play fainting cry and wait
				BtlController_EmitFaintingCry(battlerId, BUFFER_A);
				MarkBattlerForControllerExec(battlerId);
				BattleScriptCall(BattleScript_Pausex40);
				++gBattleCommunication[FAINT_EFFECTS_STATE];
				break;
			case 2: // Do faint animation
				BtlController_EmitFaintAnimation(battlerId, BUFFER_A);
				MarkBattlerForControllerExec(battlerId);
				++gBattleCommunication[FAINT_EFFECTS_STATE];
				break;
			case 3: // Faint string
				gBattleScripting.battler = battlerId;
				BattleScriptCall(BattleScript_PrintBattlerFainted);
				++gBattleCommunication[FAINT_EFFECTS_STATE];
				break;
			case 4: // Clear battle effects
				gBattleMons[battlerId].status1.id = STATUS1_NONE;
				
				BtlController_EmitSetMonData(battlerId, BUFFER_A, REQUEST_STATUS_BATTLE, 0, sizeof(gBattleMons[battlerId].status1), &gBattleMons[battlerId].status1);
				MarkBattlerForControllerExec(battlerId);
				
				FaintClearSetData(battlerId); // Effects like attractions, trapping, etc.
				
				if (gBattleStruct->sides[GetBattlerSide(battlerId)].faintCounter < 100)
					++gBattleStruct->sides[GetBattlerSide(battlerId)].faintCounter;
				
				++gBattleCommunication[FAINT_EFFECTS_STATE];
				break;
			case 5: // Restore Commander battler sprite
				if (IsBattlerBeingCommanded(battlerId)) // Battler is being commanded, restore ally sprite
					RestoreCommanderBattlerSprite(BATTLE_PARTNER(battlerId));
				else if (gStatuses3[battlerId] & STATUS3_COMMANDING) // Battler is commanding, restore its sprite
					RestoreCommanderBattlerSprite(battlerId);
					
				++gBattleCommunication[FAINT_EFFECTS_STATE];
				break;
			case 6: // Revert forms, first step
				if (DoSpecialFormChange(battlerId, gBattlerPartyIndexes[battlerId], FORM_CHANGE_FAINT))
					++gBattleCommunication[FAINT_EFFECTS_STATE];
				else
					gBattleCommunication[FAINT_EFFECTS_STATE] += 3; // Skip fainting form change
				break;
			case 7: // Revert forms, second step
			{
				struct Pokemon *mon = GetBattlerPartyIndexPtr(battlerId);
				
				// Reload all stats
				CalculateMonStats(mon);
				BtlController_EmitSetRawMonData(battlerId, BUFFER_A, offsetof(struct Pokemon, attack), sizeof(gBattleMons[battlerId].attack) * (NUM_STATS - 1), &mon->attack);
				MarkBattlerForControllerExec(battlerId);
				
				++gBattleCommunication[FAINT_EFFECTS_STATE];
				break;
			}
			case 8: // Revert forms, final step
			{
				struct Pokemon *mon = GetBattlerPartyIndexPtr(battlerId);
				u16 newHP = min(GetMonData(mon, MON_DATA_MAX_HP), GetMonData(mon, MON_DATA_HP));
				
				BtlController_EmitSetMonData(battlerId, BUFFER_A, REQUEST_HP_BATTLE, 0, sizeof(newHP), &newHP);
				MarkBattlerForControllerExec(battlerId);
				
				++gBattleCommunication[FAINT_EFFECTS_STATE];
				break;
			}
			case 9: // Try activate Soul Heart
				while (gBattleStruct->soulHeartBattlerId < gBattlersCount)
				{
					gBattleScripting.battler = gBattleStruct->soulHeartBattlerId++;
					
					if (IsBattlerAlive(gBattleScripting.battler) && GetBattlerAbility(gBattleScripting.battler) == ABILITY_SOUL_HEART
					&& !NoAliveMonsForEitherParty() && CompareStat(gBattleScripting.battler, STAT_SPATK, MAX_STAT_STAGES, CMP_LESS_THAN))
					{
						SetStatChanger(STAT_SPATK, +1);
						BattleScriptCall(BattleScript_SoulHeartActivates);
						return;
					}
				}
				gBattleStruct->soulHeartBattlerId = 0;
				++gBattleCommunication[FAINT_EFFECTS_STATE];
				break;
			case 10: // Try activate Receiver and Power Of Alchemy
				if (!gAbilities[gBattleMons[battlerId].ability].cantBeCopied && IsBattlerAlive(BATTLE_PARTNER(battlerId)))
				{
					u16 ability = GetBattlerAbility(BATTLE_PARTNER(battlerId));
					
					if (ability == ABILITY_RECEIVER || ability == ABILITY_POWER_OF_ALCHEMY)
					{
						gBattleScripting.battler = battlerId;
						BattleScriptCall(BattleScript_ReceiverActivates);
					}
				}
				++gBattleCommunication[FAINT_EFFECTS_STATE];
				break;
			case 11:
				if (ShouldDoTrainerSlide(battlerId, TRAINER_SLIDE_FIRST_MON_DOWN))
					BattleScriptCall(BattleScript_TrainerSlideMsg);
				
				++gBattleCommunication[FAINT_EFFECTS_STATE];
				break;
			case 12: // End command
				gBattlescriptCurrInstr = cmd->nextInstr;
				break;
		}
    }
}

static void atk1C_jumpbasedonstatus(void)
{
	CMD_ARGS(u8 battler, u8 statusId, u8 cmpKind, u32 status, const u8 *ptr);

    u8 battlerId = GetBattlerForBattleScript(cmd->battler);
	
	if (IsBattlerAlive(battlerId))
	{
		u8 cmpKind = cmd->cmpKind;
		u32 status = cmd->status;
		const u8 *jumpPtr = cmd->ptr;
		
		switch (cmd->statusId)
		{
			case ID_STATUS2:
			    if (JumpBasedOnKind(gBattleMons[battlerId].status2, cmpKind, status))
				{
					gBattlescriptCurrInstr = jumpPtr;
					return;
				}
				break;
			case ID_STATUS3:
			    if (JumpBasedOnKind(gStatuses3[battlerId], cmpKind, status))
				{
					gBattlescriptCurrInstr = jumpPtr;
					return;
				}
				break;
		}
	}
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk1D_jumpbasedonstatus1(void)
{
	CMD_ARGS(u8 battler, u8 cmpKind, u8 statusId, const u8 *ptr);

    u8 battlerId = GetBattlerForBattleScript(cmd->battler);
	
	if (IsBattlerAlive(battlerId) && JumpBasedOnKind(gBattleMons[battlerId].status1.id, cmd->cmpKind, cmd->statusId))
		gBattlescriptCurrInstr = cmd->ptr;
	else
		gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk1E_jumpifability(void)
{
	CMD_ARGS(u8 battler, u16 ability, const u8 *ptr);

    u8 battlerId = GetBattlerForBattleScript(cmd->battler);

	if (GetBattlerAbility(battlerId) == cmd->ability)
	{
		gBattleScripting.battlerWithAbility = battlerId;
		gBattlescriptCurrInstr = cmd->ptr;
    }
	else
		gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk1F_jumpifsideaffecting(void)
{
	CMD_ARGS(u8 battler, u16 sideStatus, const u8 *ptr);

    if (gSideStatuses[GetBattlerSide(GetBattlerForBattleScript(cmd->battler))] & cmd->sideStatus)
        gBattlescriptCurrInstr = cmd->ptr;
    else
        gBattlescriptCurrInstr = cmd->nextInstr;
}

bool8 CompareStat(u8 battlerId, u8 statId, s8 cmpTo, u8 cmpKind)
{
	if (GetBattlerAbility(battlerId) == ABILITY_CONTRARY)
    {
		cmpTo -= MAX_STAT_STAGES;
		cmpTo *= -1;
    }
	return JumpBasedOnKind(gBattleMons[battlerId].statStages[statId], cmpKind, cmpTo);
}

static void atk20_jumpifstat(void)
{
	CMD_ARGS(u8 battler, u8 cmpKind, u8 statId, u8 cmpTo, const u8 *ptr);

	if (CompareStat(GetBattlerForBattleScript(cmd->battler), cmd->statId, cmd->cmpTo, cmd->cmpKind))
		gBattlescriptCurrInstr = cmd->ptr;
	else
		gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk21_applyqueuedstatboosts(void)
{
	CMD_ARGS(u8 battler, const u8 *boostScript);
	
	u8 i, battlerId = GetBattlerForBattleScript(cmd->battler);
	
	if (gQueuedStatBoosts[battlerId].stats)
	{
		for (i = STAT_ATK; i < NUM_BATTLE_STATS; i++)
		{
			if (gQueuedStatBoosts[battlerId].stats & gBitTable[i])
			{
				gQueuedStatBoosts[battlerId].stats &= ~(gBitTable[i]);
				SetStatChanger(i, gQueuedStatBoosts[battlerId].statChanges[i]);
				BattleScriptCall(cmd->boostScript);
				return;
			}
		}
	}
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk22_jumpbasedontype(void)
{
	CMD_ARGS(u8 battler, u8 type, bool8 jumpIfHasType, const u8 *ptr);

	u8 battler = GetBattlerForBattleScript(cmd->battler), type = cmd->type;
	
	if (cmd->jumpIfHasType)
	{
		if (IsBattlerOfType(battler, type))
			gBattlescriptCurrInstr = cmd->ptr;
		else
			gBattlescriptCurrInstr = cmd->nextInstr;
	}
	else
	{
		if (!IsBattlerOfType(battler, type))
			gBattlescriptCurrInstr = cmd->ptr;
		else
			gBattlescriptCurrInstr = cmd->nextInstr;
	}
}

static inline bool8 MonCanReceiveExp(struct Pokemon *mon)
{
	// Check if mon was sent to battle or is in the level cap
	if ((!gSaveBlock2Ptr->expShare && !(gBattleStruct->sentInPokes & 1)) || GetMonData(mon, MON_DATA_LEVEL) >= GetCurrentLevelCapLevel())
		return FALSE;
	return TRUE;
}

static void UpdateBattlerStatsOnLvlUp(u8 battler)
{
	gBattleMons[battler].level = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_LEVEL);
	gBattleMons[battler].hp = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_HP);
	gBattleMons[battler].maxHP = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_MAX_HP);
	gBattleMons[battler].attack = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_ATK);
	gBattleMons[battler].defense = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_DEF);
	gBattleMons[battler].speed = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPEED);
	gBattleMons[battler].spAttack = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPATK);
	gBattleMons[battler].spDefense = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPDEF);
	
	if (gBattleStruct->battlers[battler].payDayLevel) // Update pay day level
		gBattleStruct->battlers[battler].payDayLevel = gBattleMons[battler].level;
}

static void atk23_getexp(void)
{
	CMD_ARGS(u8 battler);

	u8 i, viaSentIn, viaExpShare;
	u16 calculatedExp;
	s32 sentIn;
	
    gBattlerFainted = GetBattlerForBattleScript(cmd->battler);
    sentIn = gSentPokesToOpponent[(gBattlerFainted & 2) >> 1];
	
    switch (gBattleScripting.atk23_state)
    {
		case 0: // Check if should receive exp at all
            if (GetBattlerSide(gBattlerFainted) != B_SIDE_OPPONENT || (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_SAFARI)))
			    gBattleScripting.atk23_state = 6; // Goto last case
			else
			{
				++gBattleScripting.atk23_state;
				gBattleStruct->givenExpMons |= gBitTable[gBattlerPartyIndexes[gBattlerFainted]];
			}
			break;
		case 1: // Calculate experience points to redistribute
			gBattleStruct->sentInPokes = sentIn;

            for (viaSentIn = 0, i = 0; i < PARTY_SIZE; ++i)
            {
				if (MonCanBattle(&gPlayerParty[i]) && (gBitTable[i] & sentIn))
					++viaSentIn;
            }
            calculatedExp = gSpeciesInfo[gBattleMons[gBattlerFainted].species].expYield * gBattleMons[gBattlerFainted].level / 5;
		
            if (gSaveBlock2Ptr->expShare) // Exp share is turned on
            {
				u16 exp = calculatedExp / 2;

				gExpShareExp = exp;
                if (gExpShareExp == 0)
                    gExpShareExp = 1;

                gBattleStruct->expValue = SAFE_DIV(exp, viaSentIn);
                if (gBattleStruct->expValue == 0)
                    gBattleStruct->expValue = 1;

                viaExpShare = gSaveBlock1Ptr->playerPartyCount;
                
            }
            else
            {
#if FULL_EXP_FOR_PARTICIPATING
                gBattleStruct->expValue = calculatedExp;
#else
		        gBattleStruct->expValue = SAFE_DIV(calculatedExp, viaSentIn);
#endif
                if (gBattleStruct->expValue == 0)
                    gBattleStruct->expValue = 1;

                gExpShareExp = 0;
            }
            gBattleStruct->expGetterMonId = 0;
			++gBattleScripting.atk23_state;
        // Fall through
		case 2: // Set exp value to the poke in expgetter_id and print message
			if (!gBattleControllerExecFlags)
			{
				if (!MonCanReceiveExp(&gPlayerParty[gBattleStruct->expGetterMonId]))
				{
					*(&gBattleStruct->sentInPokes) >>= 1;
					gBattleScripting.atk23_state = 5;
					gBattleMoveDamage = 0; // Used for exp
				}
                else
				{
					// Music change in wild battle after fainting a poke
					if (!(gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_POKEDUDE)) && (IsBattlerAlive(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT))
					|| (IsDoubleBattleOnSide(B_SIDE_PLAYER) && IsBattlerAlive(GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT))))
					&& !gBattleStruct->wildVictorySong && CountAliveMonsInBattle(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), BATTLE_ALIVE_SIDE) == 0)
					{
						BattleStopLowHpSound();
						PlayBGM(MUS_VICTORY_WILD);
						gBattleStruct->wildVictorySong = TRUE;
					}

					if (MonCanBattle(&gPlayerParty[gBattleStruct->expGetterMonId]))
					{
						u16 stringId;
						u8 holdEffect = ItemId_GetHoldEffect(GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_HELD_ITEM));

						if (gBattleStruct->sentInPokes & 1)
							gBattleMoveDamage = gBattleStruct->expValue;
						else
							gBattleMoveDamage = 0;
						
						if (gSaveBlock2Ptr->expShare)
							gBattleMoveDamage += gExpShareExp;
						
						if (holdEffect == HOLD_EFFECT_LUCKY_EGG)
							gBattleMoveDamage = (gBattleMoveDamage * 150) / 100;
						
						if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
							gBattleMoveDamage = (gBattleMoveDamage * 150) / 100;
						
						if (IsTradedMon(&gPlayerParty[gBattleStruct->expGetterMonId]) && !(gBattleTypeFlags & BATTLE_TYPE_POKEDUDE))
						{
							gBattleMoveDamage = (gBattleMoveDamage * 150) / 100;
							stringId = STRINGID_ABOOSTED;
						}
						else
							stringId = STRINGID_EMPTYSTRING;

						// Get exp getter battlerId
						if (IsDoubleBattleOnSide(B_SIDE_PLAYER))
						{
							if (!(gBattlerPartyIndexes[2] != gBattleStruct->expGetterMonId) && !(gAbsentBattlerFlags & gBitTable[2]))
								gBattleStruct->expGetterBattlerId = 2;
							else
							{
								if (!(gAbsentBattlerFlags & gBitTable[0]))
									gBattleStruct->expGetterBattlerId = 0;
								else
									gBattleStruct->expGetterBattlerId = 2;
							}
						}
						else
							gBattleStruct->expGetterBattlerId = 0;
						
						PrepareMonNickWithPrefixBuffer(gBattleTextBuff1, gBattleStruct->expGetterBattlerId, gBattleStruct->expGetterMonId);
						// Buffer 'gained' or 'gained a boosted'
						PrepareStringBuffer(gBattleTextBuff2, stringId);
						PrepareWordNumberBuffer(gBattleTextBuff3, 5, gBattleMoveDamage);
						PrepareStringBattle(STRINGID_PKMNGAINEDEXP, gBattleStruct->expGetterBattlerId);
						MonGainEVs(&gPlayerParty[gBattleStruct->expGetterMonId], gBattleMons[gBattlerFainted].species);
					}
					gBattleStruct->sentInPokes >>= 1;
					++gBattleScripting.atk23_state;
				}
			}
			break;
		case 3: // Set stats and give exp
		    if (!gBattleControllerExecFlags)
			{
				gBattleBufferB[gBattleStruct->expGetterBattlerId][0] = 0;
				
				if (GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_HP) && GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_LEVEL) != MAX_LEVEL)
				{
					gBattleResources->beforeLvlUp->stats[STAT_HP]    = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_MAX_HP);
					gBattleResources->beforeLvlUp->stats[STAT_ATK]   = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_ATK);
					gBattleResources->beforeLvlUp->stats[STAT_DEF]   = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_DEF);
					gBattleResources->beforeLvlUp->stats[STAT_SPEED] = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPEED);
					gBattleResources->beforeLvlUp->stats[STAT_SPATK] = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPATK);
					gBattleResources->beforeLvlUp->stats[STAT_SPDEF] = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPDEF);
					
					BtlController_EmitExpUpdate(gBattleStruct->expGetterBattlerId, BUFFER_A, gBattleStruct->expGetterMonId, gBattleMoveDamage);
					MarkBattlerForControllerExec(gBattleStruct->expGetterBattlerId);
				}
				++gBattleScripting.atk23_state;
			}
			break;
		case 4: // Lvl up if necessary
		    if (!gBattleControllerExecFlags)
			{
				u8 battler = gBattleStruct->expGetterBattlerId;
				
				if (gBattleBufferB[battler][0] == CONTROLLER_TWORETURNVALUES && gBattleBufferB[battler][1] == RET_VALUE_LEVELED_UP)
				{
					if ((gBattleTypeFlags & BATTLE_TYPE_TRAINER) && gBattlerPartyIndexes[battler] == gBattleStruct->expGetterMonId)
						HandleLowHpMusicChange(&gPlayerParty[gBattlerPartyIndexes[battler]], battler);
					
					PrepareMonNickWithPrefixBuffer(gBattleTextBuff1, battler, gBattleStruct->expGetterMonId);
					PrepareByteNumberBuffer(gBattleTextBuff2, 3, GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_LEVEL));
					BattleScriptCall(BattleScript_LevelUp);

					gLeveledUpInBattle |= gBitTable[gBattleStruct->expGetterMonId];
					gBattleMoveDamage = (gBattleBufferB[battler][2] | (gBattleBufferB[battler][3] << 8));

					AdjustFriendship(&gPlayerParty[gBattleStruct->expGetterMonId], FRIENDSHIP_EVENT_GROW_LEVEL);

					// Update battle mon structure after level up
					if (gBattlerPartyIndexes[0] == gBattleStruct->expGetterMonId && IsBattlerAlive(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)))
						UpdateBattlerStatsOnLvlUp(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT));
					else if (IsDoubleBattleOnSide(B_SIDE_PLAYER) && gBattlerPartyIndexes[2] == gBattleStruct->expGetterMonId && IsBattlerAlive(GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)))
						UpdateBattlerStatsOnLvlUp(GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT));

					gBattleScripting.atk23_state = 5;
				}
				else
				{
					gBattleMoveDamage = 0;
					gBattleScripting.atk23_state = 5;
				}
			}
			break;
		case 5: // Loop increment
		    if (gBattleMoveDamage) // There is exp to give, goto case 3 that gives exp
			    gBattleScripting.atk23_state = 3;
			else
			{
				++gBattleStruct->expGetterMonId;
				
				if (gBattleStruct->expGetterMonId < PARTY_SIZE)
					gBattleScripting.atk23_state = 2; // Loop again
				else
					gBattleScripting.atk23_state = 6; // We're done
			}
			break;
		case 6: // Increment instruction
            if (!gBattleControllerExecFlags)
			{
				// Not sure why gf clears the item and ability here
				gBattleMons[gBattlerFainted].item = ITEM_NONE;
				gBattleMons[gBattlerFainted].ability = ABILITY_NONE;
				gBattlescriptCurrInstr = cmd->nextInstr;
			}
			break;
	}
}

static void atk24_checkteamslot(void)
{
	CMD_ARGS(const u8 *ptr);

    if (!gBattleControllerExecFlags)
    {
		u8 i, foundPlayer, foundOpponent;

        if (NoAliveMonsForParty(gPlayerParty))
            gBattleOutcome |= B_OUTCOME_LOST;
		
        if (NoAliveMonsForParty(gEnemyParty))
            gBattleOutcome |= B_OUTCOME_WON;
		
        if (gBattleOutcome == 0 && (gBattleTypeFlags & BATTLE_TYPE_LINK))
        {
            for (foundPlayer = 0, i = 0; i < gBattlersCount; i += 2)
			{
				u32 *ptr = &gHitMarker;
				u32 hitMarkerUnk = 0x10000000;
				
				if (((hitMarkerUnk << i) & *ptr) && !gSpecialStatuses[i].faintedHasReplacement)
					foundPlayer++;
			}
			
            for (foundOpponent = 0, i = 1; i < gBattlersCount; i += 2)
			{
				u32 *ptr = &gHitMarker;
				u32 hitMarkerUnk = 0x10000000;
				
				if (((hitMarkerUnk << i) & *ptr) && !gSpecialStatuses[i].faintedHasReplacement)
					foundOpponent++;
			}
				
            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                if (foundOpponent + foundPlayer > 1)
                    gBattlescriptCurrInstr = cmd->ptr;
                else
                    gBattlescriptCurrInstr = cmd->nextInstr;
            }
            else
            {
                if (foundOpponent && foundPlayer)
                    gBattlescriptCurrInstr = cmd->ptr;
                else
                    gBattlescriptCurrInstr = cmd->nextInstr;
            }
        }
        else
            gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atk25_jumpifabilityonside(void)
{
	CMD_ARGS(u8 battler, u16 ability, const u8 *ptr);

	u8 ret = ABILITY_ON_SIDE(GetBattlerForBattleScript(cmd->battler), cmd->ability);
	
	if (ret)
	{
		gBattleScripting.battler = ret - 1;
		gBattlescriptCurrInstr = cmd->ptr;
	}
	else
		gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk26_setmoveeffect(void)
{
	CMD_ARGS(u8 effect, bool8 affectsUser, bool8 certain);
	SetMoveEffect(cmd->effect, cmd->affectsUser, cmd->certain);
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk27_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk28_goto(void)
{
	CMD_ARGS(const u8 *ptr);
    gBattlescriptCurrInstr = cmd->ptr;
}

static void atk29_jumpifbyte(void)
{
	CMD_ARGS(u8 cmpKind, const u8 *memPtr, u8 cmpTo, const u8 *ptr);

	if (JumpBasedOnKind(*cmd->memPtr, cmd->cmpKind, cmd->cmpTo))
		gBattlescriptCurrInstr = cmd->ptr;
	else
		gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk2A_jumpifhalfword(void)
{
	CMD_ARGS(u8 cmpKind, const u16 *memPtr, u16 cmpTo, const u8 *ptr);

	if (JumpBasedOnKind(*cmd->memPtr, cmd->cmpKind, cmd->cmpTo))
		gBattlescriptCurrInstr = cmd->ptr;
	else
		gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk2B_jumpifword(void)
{
	CMD_ARGS(u8 cmpKind, const u32 *memPtr, u32 cmpTo, const u8 *ptr);

    if (JumpBasedOnKind(*cmd->memPtr, cmd->cmpKind, cmd->cmpTo))
		gBattlescriptCurrInstr = cmd->ptr;
	else
		gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk2C_jumpifarrayequal(void)
{
	CMD_ARGS(const u8 *memPtr1, const u8 *memPtr2, u8 size, const u8 *ptr);

    u8 i;
    const u8 *mem1 = cmd->memPtr1;
    const u8 *mem2 = cmd->memPtr2;
    u32 size = cmd->size;

    for (i = 0; i < size; ++i, ++mem1, ++mem2)
    {
        if (*mem1 != *mem2)
        {
            gBattlescriptCurrInstr = cmd->nextInstr;
            break;
        }
    }
    if (i == size)
        gBattlescriptCurrInstr = cmd->ptr;
}

static void atk2D_jumpifarraynotequal(void)
{
	CMD_ARGS(const u8 *memPtr1, const u8 *memPtr2, u8 size, const u8 *ptr);

    u8 i, equalBytes = 0;
    const u8 *mem1 = cmd->memPtr1;
    const u8 *mem2 = cmd->memPtr2;
    u32 size = cmd->size;

    for (i = 0; i < size; ++i, ++mem1, ++mem2)
    {
        if (*mem1 == *mem2)
            ++equalBytes;
    }
    if (equalBytes != size)
        gBattlescriptCurrInstr = cmd->ptr;
    else
        gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk2E_setbyte(void)
{
	CMD_ARGS(u8 *memPtr, u8 value);
    *cmd->memPtr = cmd->value;
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk2F_addbyte(void)
{
    CMD_ARGS(u8 *memPtr, u8 value);
    *cmd->memPtr += cmd->value;
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk30_subbyte(void)
{
    CMD_ARGS(u8 *memPtr, u8 value);
    *cmd->memPtr -= cmd->value;
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk31_copyarray(void)
{
	CMD_ARGS(u8 *dest, const u8 *src, u8 size);

    u8 *dest = cmd->dest;
    const u8 *src = cmd->src;
    s32 i, size = cmd->size;

    for (i = 0; i < size; ++i)
        dest[i] = src[i];
	
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk32_sethalfword(void)
{
    CMD_ARGS(u16 *memPtr, u16 value);
    *cmd->memPtr = cmd->value;
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk33_orbyte(void)
{
    CMD_ARGS(u8 *memPtr, u8 value);
    *cmd->memPtr |= cmd->value;
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk34_orhalfword(void)
{
    CMD_ARGS(u16 *memPtr, u16 value);
    *cmd->memPtr |= cmd->value;
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk35_orword(void)
{
    CMD_ARGS(u32 *memPtr, u32 value);
    *cmd->memPtr |= cmd->value;
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk36_bicbyte(void)
{
	CMD_ARGS(u8 *memPtr, u8 value);
    *cmd->memPtr &= ~(cmd->value);
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk37_bichalfword(void)
{
    CMD_ARGS(u16 *memPtr, u16 value);
    *cmd->memPtr &= ~(cmd->value);
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk38_bicword(void)
{
    CMD_ARGS(u32 *memPtr, u32 value);
    *cmd->memPtr &= ~(cmd->value);
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk39_pause(void)
{
	CMD_ARGS(u16 numFrames);

    if (!gBattleControllerExecFlags)
    {
        if (++gPauseCounterBattle >= cmd->numFrames)
        {
            gPauseCounterBattle = 0;
            gBattlescriptCurrInstr = cmd->nextInstr;
        }
    }
}

static void atk3A_waitstate(void)
{
	CMD_ARGS();

    if (!gBattleControllerExecFlags)
        gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk3B_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk3C_return(void)
{
	CMD_ARGS();
    BattleScriptPop();
}

static void atk3D_end(void)
{
	CMD_ARGS();
    gMoveResultFlags = 0;
    gCurrentActionFuncId = B_ACTION_TRY_FINISH;
}

static void atk3E_end2(void)
{
	CMD_ARGS();
    gCurrentActionFuncId = B_ACTION_TRY_FINISH;
}

static void atk3F_end3(void) // pops the main function stack
{
	CMD_ARGS();

    BattleScriptPop();

    if (gBattleResources->battleCallbackStack->size != 0)
        gBattleResources->battleCallbackStack->size--;
    gBattleMainFunc = gBattleResources->battleCallbackStack->function[gBattleResources->battleCallbackStack->size];
}

static void atk40_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk41_call(void)
{
	CMD_ARGS(const u8 *ptr);
	const u8 *ptr = cmd->ptr;
	gBattlescriptCurrInstr = cmd->nextInstr;
	BattleScriptCall(ptr);
}

static void atk42_trysetsleep(void)
{
	CMD_ARGS(u8 battler, u32 flags);

	u8 battlerId = GetBattlerForBattleScript(cmd->battler);
	u32 flags = cmd->flags;
	
	switch (CanBePutToSleep(gBattlerAttacker, battlerId, flags))
	{
		case STATUS_CHANGE_WORKED:
			if (!(flags & STATUS_CHANGE_FLAG_IGNORE_SUBSTITUTE) && SubsBlockMove(gBattlerAttacker, battlerId, gCurrentMove))
				gBattlescriptCurrInstr = BattleScript_ButItFailed;
			else
				gBattlescriptCurrInstr = cmd->nextInstr;
			break;
		case STATUS_CHANGE_FAIL_SPECIFIC_STATUSED:
			gBattlescriptCurrInstr = BattleScript_AlreadyAsleep;
			break;
		case STATUS_CHANGE_FAIL_ALREADY_STATUSED:
			gBattlescriptCurrInstr = BattleScript_ButItFailed;
			break;
		case STATUS_CHANGE_FAIL_SAFEGUARD_PROTECTED:
			gBattlescriptCurrInstr = BattleScript_SafeguardProtected;
			break;
		case STATUS_CHANGE_FAIL_UPROAR:
			gBattlescriptCurrInstr = BattleScript_CantMakeAsleep;
			break;
		case STATUS_CHANGE_FAIL_ABILITY_PREVENTED:
			gBattlescriptCurrInstr = BattleScript_AbilityPreventSleep;
			break;
		case STATUS_CHANGE_FAIL_SWEET_VEIL_ON_SIDE:
			gBattleScripting.battler = gBattleScripting.savedBattler;
			gBattlescriptCurrInstr = BattleScript_TeamProtectedBySweetVeil;
			break;
		case STATUS_CHANGE_FAIL_FLOWER_VEIL_ON_SIDE:
		    gEffectBattler = battlerId;
			gBattleScripting.battler = gBattleScripting.savedBattler;
			gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
			break;
	}
}

static void atk43_trysetconfusion(void)
{
	CMD_ARGS(u8 battler);

	u8 battlerId = GetBattlerForBattleScript(cmd->battler);
	
	switch (CanBecameConfused(gBattlerAttacker, battlerId, 0))
	{
		case STATUS_CHANGE_WORKED:
			if (SubsBlockMove(gBattlerAttacker, battlerId, gCurrentMove))
				gBattlescriptCurrInstr = BattleScript_ButItFailed;
			else
				gBattlescriptCurrInstr = cmd->nextInstr;
			break;
		case STATUS_CHANGE_FAIL_ABILITY_PREVENTED:
			gBattlescriptCurrInstr = BattleScript_OwnTempoPrevents;
			break;
		case STATUS_CHANGE_FAIL_SPECIFIC_STATUSED:
			gBattlescriptCurrInstr = BattleScript_AlreadyConfused;
			break;
		case STATUS_CHANGE_FAIL_SAFEGUARD_PROTECTED:
		    gBattlescriptCurrInstr = BattleScript_SafeguardProtected;
			break;
	}
}

static void atk44_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

// unify playanimation and playanimation2 into a single function
static void PlayAnimationInternal(u8 battlerId, u8 animId, u16 argument)
{
	if (gBattleAnims_General[animId].shouldBePlayed)
    {
        BtlController_EmitBattleAnimation(battlerId, BUFFER_A, animId, argument);
        MarkBattlerForControllerExec(battlerId);
    }
    else if (!IsBattleAnimationsOn() && animId != B_ANIM_MON_HIT)
		BattleScriptCall(BattleScript_Pausex20);
    else if (gBattleAnims_General[animId].ignoreSemiInvulnerability)
    {
        BtlController_EmitBattleAnimation(battlerId, BUFFER_A, animId, argument);
        MarkBattlerForControllerExec(battlerId);
    }
    else if (!(gStatuses3[battlerId] & (STATUS3_SEMI_INVULNERABLE | STATUS3_COMMANDING)))
    {
        BtlController_EmitBattleAnimation(battlerId, BUFFER_A, animId, argument);
        MarkBattlerForControllerExec(battlerId);
    }
}

static void atk45_playanimation(void)
{
	CMD_ARGS(u8 battler, u8 animId, const u16 *animParamPtr);
	gBattlescriptCurrInstr = cmd->nextInstr;
	PlayAnimationInternal(GetBattlerForBattleScript(cmd->battler), cmd->animId, *cmd->animParamPtr);
}

static void atk46_playanimation2(void) // Animation Id is stored in the first pointer
{
	CMD_ARGS(u8 battler, const u8 *animIdPtr, const u16 *animParamPtr);
	gBattlescriptCurrInstr = cmd->nextInstr;
	PlayAnimationInternal(GetBattlerForBattleScript(cmd->battler), *cmd->animIdPtr, *cmd->animParamPtr);
}

static void atk47_prefaintmoveend(void)
{
	CMD_ARGS(u8 state);

	u8 state = cmd->state;
	bool8 effect = FALSE;
	
	do
	{
	    switch (gBattleScripting.atk47_state)
	    {
	    	case ATK47_SET_UP:
	    	    gBattleScripting.savedDmg += gHpDealt; // For recoil damage
	    		++gBattleScripting.atk47_state;
	    		break;
	    	case ATK47_ATTACKER_ABILITIES:
	    	    if (state != MOVEEND_FUTURE_ATTACK && AbilityBattleEffects(ABILITYEFFECT_MOVE_END_ATTACKER, gBattlerAttacker))
	    			effect = TRUE;
	    		++gBattleScripting.atk47_state;
	    		break;
	    	case ATK47_PROTECTION_EFFECTS:
	    	    ++gBattleScripting.atk47_state;
	    		break;
	    	case ATK47_RAGE:
	    	    if (BATTLER_TURN_DAMAGED(gBattlerTarget) && IsBattlerAlive(gBattlerTarget) && (gBattleMons[gBattlerTarget].status2 & STATUS2_RAGE)
	    		&& GetBattlerSide(gBattlerAttacker) != GetBattlerSide(gBattlerTarget) && CompareStat(gBattlerTarget, STAT_ATK, MAX_STAT_STAGES, CMP_LESS_THAN))
	    		{
	    			BattleScriptCall(BattleScript_RageIsBuilding);
	    			effect = TRUE;
	    		}
	    		++gBattleScripting.atk47_state;
	    		break;
	    	case ATK47_SYNCHRONIZE_TARGET:
	    	    if ((gBattleMoves[gCurrentMove].effect != EFFECT_PSYCHO_SHIFT || (gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
	    		&& AbilityBattleEffects(ABILITYEFFECT_SYNCHRONIZE, gBattlerTarget))
	    		{
	    			gBattleStruct->moveEffect.affectsUser = TRUE;
	    			effect = TRUE;
	    		}
	    		++gBattleScripting.atk47_state;
	    		break;
	    	case ATK47_BEAK_BLAST:
	    	    ++gBattleScripting.atk47_state;
	    		break;
	    	case ATK47_SYNCHRONIZE_ATTACKER:
	    	case ATK47_SYNCHRONIZE_ATTACKER_2:
	    	    if (AbilityBattleEffects(ABILITYEFFECT_SYNCHRONIZE, gBattlerAttacker))
	    			effect = TRUE;
	    		++gBattleScripting.atk47_state;
	    		break;
	    	case ATK47_TARGET_ABILITIES:
	    	    if (AbilityBattleEffects(ABILITYEFFECT_MOVE_END_TARGET, gBattlerTarget))
	    			effect = TRUE;
	    		++gBattleScripting.atk47_state;
	    		break;
	    	case ATK47_TARGET_CONTACT_ITEMS:
	    	    ++gBattleScripting.atk47_state;
	    		break;
	    }
	} while (gBattleScripting.atk47_state != ATK47_COUNT && !effect);
	
	if (gBattleScripting.atk47_state == ATK47_COUNT && !effect)
		gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk48_playstatchangeanimation(void)
{
	CMD_ARGS(u32 bits, u8 flags);

	struct StatChange savedStatChange;
	u8 temp, flags, changeableStatsCount, currStat;
	u16 animId;
	u32 bits;
	
	if (gBattleStruct->statChange.mirrorArmorState == 1)
	{
		SWAP(gBattlerAttacker, gBattlerTarget, temp);
		++gBattleStruct->statChange.mirrorArmorState; // pop-up showed
		BattleScriptCall(BattleScript_MirrorArmorBounceBack);
		return;
	}
	flags = cmd->flags;
	
	if (!gBattleStruct->statChange.statAnimPlayed || (flags & ATK48_IGNORE_ANIM_PLAYED))
	{
		if (gBattleStruct->statChange.result == STAT_CHANGE_WORKED)
		{
			animId = gBattleStruct->statChange.statId;
			
			// Save original stat buff
			memcpy(&savedStatChange, &gBattleStruct->statChange, sizeof(struct StatChange));
			
			// Check multiples stat changes
			changeableStatsCount = 0;
			currStat = STAT_ATK;
			bits = cmd->bits;
			while (bits != 0)
	    	{
	    		if (bits & 1)
	    		{
	    			SetStatChanger(currStat, (flags & ATK48_STAT_NEGATIVE) ? -1 : +1);
	    			
	    			if (ChangeStatBuffs(savedStatChange.flags, TRUE)) // Check if other stats can be changed
	    				++changeableStatsCount;
	    		}
	    		bits >>= 1;
	    		++currStat;
	    	}
			
			if (changeableStatsCount > 1) // Multiple buffs
	    		animId = 0;
			
			// Restore original stat buff
			memcpy(&gBattleStruct->statChange, &savedStatChange, sizeof(struct StatChange));
		}
		else
			animId = 0xFFFF;
	    
	    if (animId != 0xFFFF)
	    {
	    	BtlController_EmitBattleAnimation(gEffectBattler, BUFFER_A, B_ANIM_STATS_CHANGE, animId);
            MarkBattlerForControllerExec(gEffectBattler);
			
			if (flags & ATK48_SET_ANIM_PLAYED)
				gBattleStruct->statChange.statAnimPlayed = TRUE;
	    }
	}
	
	if (flags & ATK48_CLEAR_ANIM_PLAYED)
		gBattleStruct->statChange.statAnimPlayed = FALSE;
	
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static u8 GetNextTarget(u8 moveTarget, bool8 excludeCurrent)
{
    u8 i;
	
    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
    {
        if (i != gBattlerAttacker && !(excludeCurrent && i == gBattlerTarget) && IsBattlerAlive(i) && !(gBattleStruct->battlers[gBattlerAttacker].targetsDone & gBitTable[i])
		&& (GetBattlerSide(i) != GetBattlerSide(gBattlerAttacker) || moveTarget == MOVE_TARGET_FOES_AND_ALLY))
			break;
    }
    return i;
}

static void SetNextTarget(u8 nextTarget)
{
	gBattleStruct->battlers[gBattlerAttacker].moveTarget = gBattlerTarget = nextTarget;
	gHitMarker |= HITMARKER_NO_ATTACKSTRING;
	gBattleScripting.atk47_state = gBattleScripting.atk49_state = 0;
	MoveValuesCleanUp();
	BattleScriptPush(GET_MOVE_BATTLESCRIPT(gCurrentMove));
	gBattlescriptCurrInstr = BattleScript_FlushMessageBox;
}

static bool8 CanBurnHitThaw(u16 move)
{
	u8 i;
	
	for (i = 0; i < gBattleMoves[move].numAdditionalEffects; i++)
	{
		if (gBattleMoves[move].additionalEffects[i].moveEffect == MOVE_EFFECT_BURN)
			return TRUE;
	}
	return FALSE;
}

static void atk49_moveend(void)
{
	CMD_ARGS(u8 state, u8 lastCase);

	u16 originallyUsedMove;
	u8 i, state, lastCase;
	bool8 effect;
	
	// Apply it if not called before
	if (!gBattleScripting.atk47_state)
	{
		BattleScriptCall(BattleScript_DoPreFaintEffects);
		return;
	}
	originallyUsedMove = gChosenMove == MOVE_UNAVAILABLE ? MOVE_NONE : gChosenMove;
	state = cmd->state, lastCase = cmd->lastCase;
	effect = FALSE;
	
	do
	{
		switch (gBattleScripting.atk49_state)
		{
			case ATK49_SKY_DROP:
			    ++gBattleScripting.atk49_state;
				break;
			case ATK49_ATTACKER_INVISIBLE:
			    ++gBattleScripting.atk49_state;
			
			    if ((gStatuses3[gBattlerAttacker] & STATUS3_SEMI_INVULNERABLE) && !IsBattleAnimationsOn())
				{
					BtlController_EmitSpriteInvisibility(gBattlerAttacker, BUFFER_A, TRUE);
					MarkBattlerForControllerExec(gBattlerAttacker);
					return;
				}
				break;
			case ATK49_ATTACKER_VISIBLE:
			    ++gBattleScripting.atk49_state;
				
				if ((gMoveResultFlags & MOVE_RESULT_NO_EFFECT) || !(gStatuses3[gBattlerAttacker] & STATUS3_SEMI_INVULNERABLE) || WasUnableToUseMove(gBattlerAttacker))
				{
					BtlController_EmitSpriteInvisibility(gBattlerAttacker, BUFFER_A, FALSE);
					MarkBattlerForControllerExec(gBattlerAttacker);
					gStatuses3[gBattlerAttacker] &= ~(STATUS3_SEMI_INVULNERABLE);
					gSpecialStatuses[gBattlerAttacker].restoredBattlerSprite = TRUE;
					return;
				}
				break;
			case ATK49_TARGET_VISIBLE:
			    ++gBattleScripting.atk49_state;
				
				if (!gSpecialStatuses[gBattlerTarget].restoredBattlerSprite && gBattlerTarget < gBattlersCount && !(gStatuses3[gBattlerTarget] & (STATUS3_SEMI_INVULNERABLE | STATUS3_COMMANDING)))
				{
					BtlController_EmitSpriteInvisibility(gBattlerTarget, BUFFER_A, FALSE);
					MarkBattlerForControllerExec(gBattlerTarget);
					return;
				}
				break;
			case ATK49_CHOICE_MOVE:
			    if (state != MOVEEND_FUTURE_ATTACK)
				{
					if ((GetBattlerItemHoldEffect(gBattlerAttacker, TRUE) == HOLD_EFFECT_CHOICE_ITEM || GetBattlerAbility(gBattlerAttacker) == ABILITY_GORILLA_TACTICS)
				    && (!gBattleStruct->battlers[gBattlerAttacker].choicedMove || gBattleStruct->battlers[gBattlerAttacker].choicedMove == MOVE_UNAVAILABLE)
					&& (gHitMarker & HITMARKER_OBEYS) && gChosenMove != MOVE_STRUGGLE)
					{
						if (gBattleMoves[gChosenMove].effect == EFFECT_BATON_PASS && !(gMoveResultFlags & MOVE_RESULT_FAILED))
						{
							++gBattleScripting.atk49_state;
							break;
						}
						gBattleStruct->battlers[gBattlerAttacker].choicedMove = gChosenMove;
					}
					
					// Check mon don't knows the choice move
					if (FindMoveSlotInBattlerMoveset(gBattlerAttacker, gBattleStruct->battlers[gBattlerAttacker].choicedMove) == MAX_MON_MOVES)
						gBattleStruct->battlers[gBattlerAttacker].choicedMove = MOVE_NONE;
				}
				++gBattleScripting.atk49_state;
				break;
			case ATK49_TARGET_ENDTURN_ITEMS:
			    ++gBattleScripting.atk49_state;
				break;
			case ATK49_KINGS_ROCK:
			    ++gBattleScripting.atk49_state;
				break;
			case ATK49_ATTACKER_ENDTURN_ITEMS:
			case ATK49_ATTACKER_ENDTURN_ITEMS_2:
			case ATK49_ATTACKER_ENDTURN_ITEMS_3:
			    ++gBattleScripting.atk49_state;
				break;
			case ATK49_STATUS_IMMUNITY_ABILITIES:
			    if (AbilityBattleEffects(ABILITYEFFECT_IMMUNITY, 0))
					effect = TRUE; // It loops through all battlers, so we increment after its done with all battlers
				else
					++gBattleScripting.atk49_state;
				break;
			case ATK49_UPDATE_LAST_MOVES:
			    if (state != MOVEEND_FUTURE_ATTACK)
				{
					if (!gBattleStruct->pursuitSwitchDmg && (gHitMarker & HITMARKER_SWAP_ATTACKER_TARGET))
					{
						gHitMarker &= ~(HITMARKER_SWAP_ATTACKER_TARGET);
						SWAP(gBattlerAttacker, gBattlerTarget, i);
					}

					if (gHitMarker & HITMARKER_ATTACKSTRING_PRINTED)
					{
						gLastUsedMove = gCurrentMove;
						gBattleStruct->battlers[gBattlerAttacker].lastPrintedMove = gChosenMove;
						gBattleStruct->battlers[gBattlerAttacker].lastUsedMoveType = gBattleStruct->dynamicMoveType;
						
						if (!IsZMove(gCurrentMove))
							gDisableStructs[gBattlerAttacker].usedMoveIndices |= gBitTable[gCurrMovePos];
					}
					
					if (!(gAbsentBattlerFlags & gBitTable[gBattlerAttacker]) && !(gBattleStruct->absentBattlerFlags & gBitTable[gBattlerAttacker])
					&& gBattleMoves[originallyUsedMove].effect != EFFECT_BATON_PASS)
					{
						if (gHitMarker & HITMARKER_OBEYS)
						{
							gBattleStruct->battlers[gBattlerAttacker].lastMove = gChosenMove;
							gBattleStruct->battlers[gBattlerAttacker].lastResultingMove = gCurrentMove;
						}
						else
						{
							gBattleStruct->battlers[gBattlerAttacker].lastMove = MOVE_UNAVAILABLE;
							gBattleStruct->battlers[gBattlerAttacker].lastResultingMove = MOVE_UNAVAILABLE;
						}
						
						if ((gHitMarker & HITMARKER_OBEYS) && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
						{
							if (gChosenMove == MOVE_UNAVAILABLE)
								gBattleStruct->battlers[gBattlerTarget].lastLandedMove = gChosenMove;
							else
							{
								gBattleStruct->battlers[gBattlerTarget].lastLandedMove = gCurrentMove;
								gBattleStruct->battlers[gBattlerTarget].lastHitMoveType = gBattleStruct->dynamicMoveType;
							}
						}
						else
							gBattleStruct->battlers[gBattlerTarget].lastLandedMove = MOVE_UNAVAILABLE;
						
						if (!(gHitMarker & HITMARKER_FAINTED(gBattlerTarget)))
							gBattleStruct->battlers[gBattlerTarget].lastHitBattler = gBattlerAttacker;
					}
				}
				++gBattleScripting.atk49_state;
				break;
			case ATK49_MIRROR_MOVE:
			    if (state != MOVEEND_FUTURE_ATTACK && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && !(gAbsentBattlerFlags & gBitTable[gBattlerAttacker])
				&& !(gBattleStruct->absentBattlerFlags & gBitTable[gBattlerAttacker]) && originallyUsedMove && !gBattleMoves[originallyUsedMove].flags.forbiddenMirrorMove
			    && (gHitMarker & HITMARKER_OBEYS) && gBattlerAttacker != gBattlerTarget && !(gHitMarker & HITMARKER_FAINTED(gBattlerTarget)))
				{
					gBattleStruct->battlers[gBattlerTarget].lastTakenMove = gChosenMove;
					gBattleStruct->battlers[gBattlerTarget].lastTakenMoveFrom[gBattlerAttacker] = gChosenMove;
				}
				++gBattleScripting.atk49_state;
				break;
			case ATK49_MULTIHIT_MOVE: // For multi-hit moves
				if (!gBattleStruct->pursuitSwitchDmg)
				{
					if (state != MOVEEND_FUTURE_ATTACK && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && !(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE)
					&& gMultiHitCounter && !(gBattleMoves[gCurrentMove].effect == EFFECT_PRESENT && !gBattleStruct->presentBasePower)) // Silly edge case
					{
						++gBattleScripting.multihitString[4];
						
						if (--gMultiHitCounter == 0)
						{
							BattleScriptCall(BattleScript_MultiHitPrintStrings);
							effect = TRUE;
						}
						else
						{
							if (IsBattlerAlive(gBattlerAttacker) && IsBattlerAlive(gBattlerTarget) && (gBattleMoves[gChosenMove].effect == EFFECT_SLEEP_TALK
							|| gBattleMoves[gChosenMove].effect == EFFECT_SNORE || gBattleMons[gBattlerAttacker].status1.id != STATUS1_SLEEP)
							&& gBattleMons[gBattlerAttacker].status1.id != STATUS1_FREEZE)
							{
								if (gSpecialStatuses[gBattlerAttacker].parentalBondState)
									--gSpecialStatuses[gBattlerAttacker].parentalBondState;
								
								gHitMarker |= (HITMARKER_NO_PPDEDUCT);
								SetNextTarget(gBattlerTarget); // Same target as now, uses the function to set proper variables
								gSpecialStatuses[gBattlerTarget].focusBanded = FALSE;
								gSpecialStatuses[gBattlerAttacker].multiHitOn = TRUE;
								return;
							}
							else
							{
								BattleScriptCall(BattleScript_MultiHitPrintStrings);
								effect = TRUE;
							}
						}
					}
					gMultiHitCounter = 0;
					gSpecialStatuses[gBattlerAttacker].parentalBondState = PARENTAL_BOND_OFF;
					gSpecialStatuses[gBattlerAttacker].multiHitOn = FALSE;
				}
				++gBattleScripting.atk49_state;
				break;
			case ATK49_DEFROST:
			    if (BATTLER_TURN_DAMAGED(gBattlerTarget) && IsBattlerAlive(gBattlerTarget) && gBattleMons[gBattlerTarget].status1.id == STATUS1_FREEZE
				&& (gBattleStruct->dynamicMoveType == TYPE_FIRE || CanBurnHitThaw(gCurrentMove)))
				{
					ClearBattlerStatus(gBattlerTarget);
					PrepareMonNickWithPrefixBuffer(gBattleTextBuff1, gBattlerTarget, gBattlerPartyIndexes[gBattlerTarget]);
					BattleScriptCall(BattleScript_DefrostedViaFireMove);
					effect = TRUE;
				}
				++gBattleScripting.atk49_state;
				break;
			case ATK49_SECOND_MOVE_EFFECT:
				if (gBattleStruct->moveEffect2.moveEffectByte)
				{
					SetMoveEffect(gBattleStruct->moveEffect2.moveEffectByte, gBattleStruct->moveEffect2.affectsUser, gBattleStruct->moveEffect2.certain);
					effect = DoMoveEffect(FALSE, gBattlescriptCurrInstr, 0);
					gBattleStruct->moveEffect2.moveEffectByte = MOVE_EFFECT_NONE;
				}
				++gBattleScripting.atk49_state;
				break;
			case ATK49_MAGICIAN:
				if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && IsBattlerAlive(gBattlerAttacker) && BATTLER_TURN_DAMAGED(gBattlerTarget))
				{
					u16 ability = GetBattlerAbility(gBattlerAttacker);

					switch (ability)
					{
						case ABILITY_MAGICIAN:
							if (state != MOVEEND_FUTURE_ATTACK)
							{
								SetMoveEffect(MOVE_EFFECT_STEAL_ITEM, FALSE, FALSE);
								
								if (DoMoveEffect(TRUE, NULL, 0))
								{
									BattleScriptCall(BattleScript_MagicianActivates);
									effect = TRUE;
								}
							}
							break;
						case ABILITY_MOXIE:
						case ABILITY_CHILLING_NEIGH:
						case ABILITY_AS_ONE_ICE_RIDER:
							if ((state != MOVEEND_FUTURE_ATTACK || gBattleStruct->battlers[gBattlerTarget].futureSightAttacker == gBattlerPartyIndexes[gBattlerAttacker])
							&& !NoAliveMonsForEitherParty() && !IsBattlerAlive(gBattlerTarget) && CompareStat(gBattlerAttacker, STAT_ATK, MAX_STAT_STAGES, CMP_LESS_THAN)
							&& !IS_MOVE_STATUS(gCurrentMove))
							{
								SetStatChanger(STAT_ATK, +1);

								if (ability == ABILITY_AS_ONE_ICE_RIDER) // Print Chilling Neigh if As One activates
									gBattleStruct->battlers[gBattlerAttacker].abilityOverride = ABILITY_CHILLING_NEIGH;

								BattleScriptCall(BattleScript_RaiseStatOnFaintingTarget);
								effect = TRUE;
							}
							break;
						case ABILITY_GRIM_NEIGH:
						case ABILITY_AS_ONE_SHADOW_RIDER:
							if ((state != MOVEEND_FUTURE_ATTACK || gBattleStruct->battlers[gBattlerTarget].futureSightAttacker == gBattlerPartyIndexes[gBattlerAttacker])
							&& !NoAliveMonsForEitherParty() && !IsBattlerAlive(gBattlerTarget) && CompareStat(gBattlerAttacker, STAT_SPATK, MAX_STAT_STAGES, CMP_LESS_THAN)
							&& !IS_MOVE_STATUS(gCurrentMove))
							{
								SetStatChanger(STAT_SPATK, +1);

								if (ability == ABILITY_AS_ONE_SHADOW_RIDER) // Print Grim Neigh if As One activates
									gBattleStruct->battlers[gBattlerAttacker].abilityOverride = ABILITY_GRIM_NEIGH;
								
								BattleScriptCall(BattleScript_RaiseStatOnFaintingTarget);
								effect = TRUE;
							}
							break;
						case ABILITY_BATTLE_BOND:
							if ((state != MOVEEND_FUTURE_ATTACK || gBattleStruct->battlers[gBattlerTarget].futureSightAttacker == gBattlerPartyIndexes[gBattlerAttacker])
							&& !gBattleStruct->sides[GetBattlerSide(gBattlerAttacker)].party[gBattlerPartyIndexes[gBattlerAttacker]].battleBondActivated
							&& !NoAliveMonsForEitherParty() && !IsBattlerAlive(gBattlerTarget) && !IS_MOVE_STATUS(gCurrentMove))
							{
#if BATTLE_BOND_TRANSFORM
								u16 newSpecies = TryDoBattleFormChange(gBattlerAttacker, FORM_CHANGE_FAINT_TARGET);
								
								if (newSpecies)
								{
									DoBattleFormChange(gBattlerAttacker, newSpecies, FALSE, TRUE, FALSE);
									PrepareMonNickBuffer(gBattleTextBuff1, gBattlerAttacker, gBattlerPartyIndexes[gBattlerAttacker]);
									gBattleStruct->sides[GetBattlerSide(gBattlerAttacker)].party[gBattlerPartyIndexes[gBattlerAttacker]].battleBondActivated = TRUE;
									BattleScriptCall(BattleScript_BattleBondTransform);
									effect = TRUE;
								}
#else
								if (CompareStat(gBattlerAttacker, STAT_ATK, MAX_STAT_STAGES, CMP_LESS_THAN) || CompareStat(gBattlerAttacker, STAT_SPATK, MAX_STAT_STAGES, CMP_LESS_THAN)
								|| CompareStat(gBattlerAttacker, STAT_SPEED, MAX_STAT_STAGES, CMP_LESS_THAN))
								{
									gBattleStruct->sides[GetBattlerSide(gBattlerAttacker)].party[gBattlerPartyIndexes[gBattlerAttacker]].battleBondActivated = TRUE;
									BattleScriptCall(BattleScript_BattleBondBoost);
									effect = TRUE;
								}
#endif
							}
							break;
						case ABILITY_BEAST_BOOST:
							if ((state != MOVEEND_FUTURE_ATTACK || gBattleStruct->battlers[gBattlerTarget].futureSightAttacker == gBattlerPartyIndexes[gBattlerAttacker])
							&& !NoAliveMonsForEitherParty() && !IsBattlerAlive(gBattlerTarget) && !IS_MOVE_STATUS(gCurrentMove))
							{
								u8 highestStatId = GetBattlerHighestStatId(gBattlerAttacker);
				
								if (CompareStat(gBattlerAttacker, highestStatId, MAX_STAT_STAGES, CMP_LESS_THAN))
								{
									SetStatChanger(highestStatId, +1);
									BattleScriptCall(BattleScript_RaiseStatOnFaintingTarget);
									effect = TRUE;
								}
							}
							break;
					}
				}
				++gBattleScripting.atk49_state;
				break;
			case ATK49_NEXT_TARGET: // For moves with MOVE_TARGET_BOTH and MOVE_TARGET_FOES_AND_ALLY.
			    gBattleStruct->battlers[gBattlerAttacker].targetsDone |= gBitTable[gBattlerTarget];
				
			    if (!(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE) && IsDoubleBattleForBattler(gBattlerTarget) && !gProtectStructs[gBattlerAttacker].chargingTurn)
				{
					u8 nextTarget, moveTarget = GetBattlerMoveTargetType(gBattlerAttacker, gCurrentMove);
					
					if (moveTarget == MOVE_TARGET_BOTH || moveTarget == MOVE_TARGET_FOES_AND_ALLY)
					{
						nextTarget = GetNextTarget(moveTarget, FALSE);
						
						if (nextTarget != MAX_BATTLERS_COUNT)
						{
							SetNextTarget(nextTarget);
							return;
						}
					}
				}
				++gBattleScripting.atk49_state;
				break;
			case ATK49_RECOIL: // Recoil damage from move
			    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && !(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE) && IsBattlerAlive(gBattlerAttacker)
				&& gBattleScripting.savedDmg && gBattleMoves[gCurrentMove].flags.recoilDivisor)
				{
					if (gCurrentMove == MOVE_STRUGGLE)
					{
						gBattleMoveDamage = max(1, gBattleMons[gBattlerAttacker].maxHP / gBattleMoves[gCurrentMove].flags.recoilDivisor);
						BattleScriptCall(BattleScript_MoveEffectRecoil);
						effect = TRUE;
					}
					else
					{
						u16 atkAbility = GetBattlerAbility(gBattlerAttacker);
						
						if (atkAbility != ABILITY_ROCK_HEAD && atkAbility != ABILITY_MAGIC_GUARD)
						{
							gBattleMoveDamage = max(1, gBattleScripting.savedDmg / gBattleMoves[gCurrentMove].flags.recoilDivisor);
							BattleScriptCall(BattleScript_MoveEffectRecoil);
							effect = TRUE;
						}
					}
				}
				++gBattleScripting.atk49_state;
				break;
			case ATK49_EJECT_BUTTON:
			case ATK49_RED_CARD:
			case ATK49_EJECT_PACK:
			    ++gBattleScripting.atk49_state;
				break;
			case ATK49_SHELL_BELL_LIFE_ORB:
			    ++gBattleScripting.atk49_state;
				break;
			case ATK49_EMERGENCY_EXIT:
			    if (!gBattleStruct->pursuitSwitchDmg && !ReceiveSheerForceBoost(gBattlerAttacker, gCurrentMove))
				{
					for (i = 0; i < gBattlersCount; ++i)
					{
						u8 battler = gBattlerByTurnOrder[i];
						
						if (battler != gBattlerAttacker && TryActivateEmergencyExit(battler) && BATTLER_TURN_DAMAGED(battler)
						&& !SubsBlockMove(gBattlerAttacker, battler, gCurrentMove) && !gSpecialStatuses[battler].emergencyExited)
						{
							gSpecialStatuses[battler].emergencyExited = TRUE;
							gBattlerTarget = battler;
							BattleScriptCall(BattleScript_EmergencyExit);
							effect = TRUE;
							return;
						}
					}
				}
				++gBattleScripting.atk49_state;
				break;
			case ATK49_PICKPOCKET:
				if (!ReceiveSheerForceBoost(gBattlerAttacker, gCurrentMove) && IsMoveMakingContact(gBattlerAttacker, gCurrentMove))
				{
					u8 target, battlers[MAX_BATTLERS_COUNT] = {0, 1, 2, 3};
					
					SortBattlersBySpeed(battlers, FALSE);
					SetMoveEffect(MOVE_EFFECT_STEAL_ITEM, TRUE, FALSE);
					
					for (i = 0; i < gBattlersCount; ++i)
					{
						target = battlers[i];
						
						if (target != gBattlerAttacker && IsBattlerAlive(target) && GetBattlerAbility(target) == ABILITY_PICKPOCKET
						&& BATTLER_TURN_DAMAGED(target) && !SubsBlockMove(gBattlerAttacker, target, gCurrentMove))
						{
							u8 savedTarget = gBattlerTarget;
							gBattlerTarget = target;
							
							effect = DoMoveEffect(TRUE, NULL, 0);
							
							gBattlerTarget = savedTarget;
							
							if (effect)
							{
								SaveTargetToStack(target);
								BattleScriptCall(BattleScript_PickpocketActivation);
								break;
							}
						}
					}
				}
				++gBattleScripting.atk49_state;
				break;
			case ATK49_SUBSTITUTE:
                for (i = 0; i < gBattlersCount; ++i)
				{
					if (gDisableStructs[i].substituteHP == 0)
						gBattleMons[i].status2 &= ~(STATUS2_SUBSTITUTE);
				}
				++gBattleScripting.atk49_state;
				break;
			case ATK49_EVOLUTIONS_TRACKER:
			    if (originallyUsedMove)
					TryUpdateEvolutionTracker(EVO_REQ_USE_MOVE_X_TIMES, 1, originallyUsedMove);
				
				if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && BATTLER_TURN_DAMAGED(gBattlerTarget))
					TryUpdateEvolutionTracker(EVO_REQ_DEFEAT_SPECIES_X_TIMES, 1, gBattleMons[gBattlerTarget].species);

				++gBattleScripting.atk49_state;
				break;
			case ATK49_CLEAR_BITS: // Clear bits after attack ends
				if ((gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && MoveHasMoveEffect(gCurrentMove, MOVE_EFFECT_THRASH, TRUE)
				&& (gBattleMons[gBattlerAttacker].status2 & STATUS2_LOCK_CONFUSE) != STATUS2_LOCK_CONFUSE_TURN(1))
					CancelMultiTurnMoves(gBattlerAttacker);
					
				if (gBattleStruct->dynamicMoveType == TYPE_ELECTRIC)
					gStatuses3[gBattlerAttacker] &= ~(STATUS3_CHARGED_UP);
				
				gBattleStruct->strongWindsMessageState = 0;
				gBattleStruct->meFirstBoost = FALSE;
				gBattleStruct->poisonPuppeteerConfusion = FALSE;
				gBattleStruct->moveEffect.additionalEffectsCounter = 0;
				gProtectStructs[gBattlerAttacker].usesBouncedMove = FALSE;
				++gBattleScripting.atk49_state;
				break;
		}
		
		if ((state == MOVEEND_CASE && !effect) || (state == MOVEEND_TO && gBattleScripting.atk49_state == lastCase))
			gBattleScripting.atk49_state = ATK49_COUNT;
		
	} while (gBattleScripting.atk49_state != ATK49_COUNT && !effect);
	
	if (gBattleScripting.atk49_state == ATK49_COUNT && !effect)
        gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk4A_formchange(void)
{
	CMD_ARGS(u8 battler, u16 baseSpecies, u16 targetSpecies, bool8 reloadTypes, bool8 reloadStats, bool8 reloadAbility, const u8 *failPtr);

	if (!gBattleControllerExecFlags)
	{
		u8 battlerId = GetBattlerForBattleScript(cmd->battler);
		
		if (!IsBattlerAlive(battlerId) || gBattleMons[battlerId].species != cmd->baseSpecies)
			gBattlescriptCurrInstr = cmd->failPtr;
		else
		{
			DoBattleFormChange(battlerId, cmd->targetSpecies, cmd->reloadTypes, cmd->reloadStats, cmd->reloadAbility);
			gBattlescriptCurrInstr = cmd->nextInstr;
		}
	}
}

static void atk4B_returnatktoball(void)
{
	CMD_ARGS();

    if (!(gHitMarker & HITMARKER_FAINTED(gBattlerAttacker)))
    {
        BtlController_EmitReturnMonToBall(gBattlerAttacker, BUFFER_A, 0);
        MarkBattlerForControllerExec(gBattlerAttacker);
    }
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk4C_getswitchedmondata(void)
{
	CMD_ARGS(u8 battler);

    if (!gBattleControllerExecFlags)
    {
        u8 battlerId = GetBattlerForBattleScript(cmd->battler);
		
        gBattlerPartyIndexes[battlerId] = gBattleStruct->battlers[battlerId].monToSwitchIntoId;
        BtlController_EmitGetMonData(battlerId, BUFFER_A, REQUEST_ALL_BATTLE, gBitTable[gBattlerPartyIndexes[battlerId]]);
        MarkBattlerForControllerExec(battlerId);
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atk4D_switchindataupdate(void)
{
	CMD_ARGS(u8 battler);

    struct BattlePokemon oldData;
    u8 i;

    if (!gBattleControllerExecFlags)
    {
        u8 battlerId = GetBattlerForBattleScript(cmd->battler);

        oldData = gBattleMons[battlerId];
		memcpy(&gBattleMons[battlerId], &gBattleBufferB[battlerId][4], sizeof(struct BattlePokemon));
		SetBattlerInitialTypes(battlerId);
        gBattleMons[battlerId].ability = GetAbilityBySpecies(gBattleMons[battlerId].species, gBattleMons[battlerId].abilityNum, gBattleMons[battlerId].abilityHidden);
        
        if (gBattleMoves[gCurrentMove].effect == EFFECT_BATON_PASS)
        {
            for (i = 0; i < NUM_BATTLE_STATS; ++i)
                gBattleMons[battlerId].statStages[i] = oldData.statStages[i];
			
            gBattleMons[battlerId].status2 = oldData.status2;
        }
        SwitchInClearSetData(battlerId);

        gBattleScripting.battler = battlerId;
        PrepareMonNickBuffer(gBattleTextBuff1, battlerId, gBattlerPartyIndexes[battlerId]);

        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atk4E_switchinanim(void)
{
	CMD_ARGS(u8 battler, bool8 dontClearSubstituteBit);

    if (!gBattleControllerExecFlags)
    {
        u8 battlerId = GetBattlerForBattleScript(cmd->battler);
	    
        if (GetBattlerSide(battlerId) == B_SIDE_OPPONENT && !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_OLD_MAN_TUTORIAL | BATTLE_TYPE_POKEDUDE | BATTLE_TYPE_GHOST)))
            HandleSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[battlerId].species), FLAG_SET_SEEN, gBattleMons[battlerId].personality);
			
        gAbsentBattlerFlags &= ~(gBitTable[battlerId]);

        BtlController_EmitSwitchInAnim(battlerId, BUFFER_A, gBattlerPartyIndexes[battlerId], cmd->dontClearSubstituteBit);
        MarkBattlerForControllerExec(battlerId);

        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atk4F_jumpifcantswitch(void)
{
	CMD_ARGS(u8 battler, const u8 *ptr);

	u8 byte = cmd->battler;
    u8 battlerId = GetBattlerForBattleScript(byte & ~(ATK4F_DONT_CHECK_STATUSES));
	
	if (!CanBattlerSwitchOut(battlerId, !(byte & ATK4F_DONT_CHECK_STATUSES)))
		gBattlescriptCurrInstr = cmd->ptr;
	else
		gBattlescriptCurrInstr = cmd->nextInstr;
}

// Opens the party screen to choose a new Pokémon to send out.
// slotId is the Pokémon to replace.
// Note that this is not used by the Switch action, only replacing fainted Pokémon or Baton Pass
static void ChooseMonToSendOut(u8 battlerId, u8 slotId)
{
    gBattleStruct->battlers[battlerId].partyIndex = gBattlerPartyIndexes[battlerId];
    BtlController_EmitChoosePokemon(battlerId, BUFFER_A, PARTY_ACTION_SEND_OUT, slotId, gBattleStruct->battlerPartyOrders[battlerId]);
    MarkBattlerForControllerExec(battlerId);
}

static void atk50_openpartyscreen(void)
{
	CMD_ARGS(u8 battler, const u8 *ptr);

    u8 i, flag40_0, flag40_1, flag40_2, flag40_3, hitmarkerFaintBits, battlerId;
    const u8 *jumpPtr = cmd->ptr;
    u32 flags = 0;

    if (cmd->battler == BS_UNKNOWN_5)
    {
        if ((gBattleTypeFlags & (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI)) != BATTLE_TYPE_DOUBLE)
        {
            for (battlerId = 0; battlerId < gBattlersCount; ++battlerId)
            {
                if (gHitMarker & HITMARKER_FAINTED(battlerId))
                {
                    if (HasNoMonsToSwitch(battlerId, PARTY_SIZE, PARTY_SIZE))
                    {
                        gAbsentBattlerFlags |= gBitTable[battlerId];
                        gHitMarker &= ~(HITMARKER_FAINTED(battlerId));
                        BtlController_EmitLinkStandbyMsg(battlerId, BUFFER_A, 2);
                        MarkBattlerForControllerExec(battlerId);
                    }
                    else if (!gSpecialStatuses[battlerId].faintedHasReplacement)
                    {
                        ChooseMonToSendOut(battlerId, PARTY_SIZE);
                        gSpecialStatuses[battlerId].faintedHasReplacement = TRUE;
                    }
                }
                else
                {
                    BtlController_EmitLinkStandbyMsg(battlerId, BUFFER_A, 2);
                    MarkBattlerForControllerExec(battlerId);
                }
            }
        }
        else if (IsDoubleBattleOnSide(B_SIDE_OPPONENT))
        {
            hitmarkerFaintBits = gHitMarker >> 0x1C;
		
            if (gBitTable[0] & hitmarkerFaintBits)
            {
                battlerId = 0;
		    
                if (HasNoMonsToSwitch(battlerId, PARTY_SIZE, PARTY_SIZE))
                {
                    gAbsentBattlerFlags |= gBitTable[battlerId];
                    gHitMarker &= ~(HITMARKER_FAINTED(battlerId));
                }
                else if (!gSpecialStatuses[battlerId].faintedHasReplacement)
                {
                    ChooseMonToSendOut(battlerId, gBattleStruct->battlers[2].monToSwitchIntoId);
                    gSpecialStatuses[battlerId].faintedHasReplacement = TRUE;
                }
                else
                {
                    BtlController_EmitLinkStandbyMsg(battlerId, BUFFER_A, 2);
                    MarkBattlerForControllerExec(battlerId);
                    flags |= 1;
                }
            }

            if ((gBitTable[2] & hitmarkerFaintBits) && !(gBitTable[0] & hitmarkerFaintBits))
            {
                battlerId = 2;
		    
                if (HasNoMonsToSwitch(battlerId, PARTY_SIZE, PARTY_SIZE))
                {
                    gAbsentBattlerFlags |= gBitTable[battlerId];
                    gHitMarker &= ~(HITMARKER_FAINTED(battlerId));
                }
                else if (!gSpecialStatuses[battlerId].faintedHasReplacement)
                {
                    ChooseMonToSendOut(battlerId, gBattleStruct->battlers[0].monToSwitchIntoId);
                    gSpecialStatuses[battlerId].faintedHasReplacement = TRUE;
                }
                else if (!(flags & 1))
                {
                    BtlController_EmitLinkStandbyMsg(battlerId, BUFFER_A, 2);
                    MarkBattlerForControllerExec(battlerId);
                }
            }

            if (gBitTable[1] & hitmarkerFaintBits)
            {
                battlerId = 1;
		    
                if (HasNoMonsToSwitch(battlerId, PARTY_SIZE, PARTY_SIZE))
                {
                    gAbsentBattlerFlags |= gBitTable[battlerId];
                    gHitMarker &= ~(HITMARKER_FAINTED(battlerId));
                }
                else if (!gSpecialStatuses[battlerId].faintedHasReplacement)
                {
                    ChooseMonToSendOut(battlerId, gBattleStruct->battlers[3].monToSwitchIntoId);
                    gSpecialStatuses[battlerId].faintedHasReplacement = TRUE;
                }
                else
                {
                    BtlController_EmitLinkStandbyMsg(battlerId, BUFFER_A, 2);
                    MarkBattlerForControllerExec(battlerId);
                    flags |= 2;
                }
            }

            if ((gBitTable[3] & hitmarkerFaintBits) && !(gBitTable[1] & hitmarkerFaintBits))
            {
                battlerId = 3;
		    
                if (HasNoMonsToSwitch(battlerId, PARTY_SIZE, PARTY_SIZE))
                {
                    gAbsentBattlerFlags |= gBitTable[battlerId];
                    gHitMarker &= ~(HITMARKER_FAINTED(battlerId));
                }
                else if (!gSpecialStatuses[battlerId].faintedHasReplacement)
                {
                    ChooseMonToSendOut(battlerId, gBattleStruct->battlers[1].monToSwitchIntoId);
                    gSpecialStatuses[battlerId].faintedHasReplacement = TRUE;
                }
                else if (!(flags & 2))
                {
                    BtlController_EmitLinkStandbyMsg(battlerId, BUFFER_A, 2);
                    MarkBattlerForControllerExec(battlerId);
                }
            }
            flag40_0 = gSpecialStatuses[0].faintedHasReplacement;
		
            if (!flag40_0)
            {
                flag40_2 = gSpecialStatuses[2].faintedHasReplacement;
		    
                if (!flag40_2 && hitmarkerFaintBits != 0)
                {
                    if (gAbsentBattlerFlags & gBitTable[0])
                        battlerId = 2;
                    else
                        battlerId = 0;

                    BtlController_EmitLinkStandbyMsg(battlerId, BUFFER_A, 2);
                    MarkBattlerForControllerExec(battlerId);
                }

            }
            flag40_1 = gSpecialStatuses[1].faintedHasReplacement;
		
            if (!flag40_1)
            {
                flag40_3 = gSpecialStatuses[3].faintedHasReplacement;
		    
                if (!flag40_3 && hitmarkerFaintBits != 0)
                {
                    if (gAbsentBattlerFlags & gBitTable[1])
                        battlerId = 3;
                    else
                        battlerId = 1;
					
                    BtlController_EmitLinkStandbyMsg(battlerId, BUFFER_A, 2);
                    MarkBattlerForControllerExec(battlerId);
                }
            }
        }
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
    else if (cmd->battler == BS_UNKNOWN_6)
    {
        if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
        {
            if (IsDoubleBattleOnSide(B_SIDE_OPPONENT))
            {
                hitmarkerFaintBits = gHitMarker >> 0x1C;
		    
                if ((gBitTable[2] & hitmarkerFaintBits) && (gBitTable[0] & hitmarkerFaintBits))
                {
                    battlerId = 2;
			
                    if (HasNoMonsToSwitch(battlerId, gBattleBufferB[0][1], PARTY_SIZE))
                    {
                        gAbsentBattlerFlags |= gBitTable[battlerId];
                        gHitMarker &= ~(HITMARKER_FAINTED(battlerId));
                    }
                    else if (!gSpecialStatuses[battlerId].faintedHasReplacement)
                    {
                        ChooseMonToSendOut(battlerId, gBattleStruct->battlers[0].monToSwitchIntoId);
                        gSpecialStatuses[battlerId].faintedHasReplacement = TRUE;
                    }
                }

                if ((gBitTable[3] & hitmarkerFaintBits) && (hitmarkerFaintBits & gBitTable[1]))
                {
                    battlerId = 3;
			
                    if (HasNoMonsToSwitch(battlerId, gBattleBufferB[1][1], PARTY_SIZE))
                    {
                        gAbsentBattlerFlags |= gBitTable[battlerId];
                        gHitMarker &= ~(HITMARKER_FAINTED(battlerId));
                    }
                    else if (!gSpecialStatuses[battlerId].faintedHasReplacement)
                    {
                        ChooseMonToSendOut(battlerId, gBattleStruct->battlers[1].monToSwitchIntoId);
                        gSpecialStatuses[battlerId].faintedHasReplacement = TRUE;
                    }
                }
                gBattlescriptCurrInstr = cmd->nextInstr;
            }
            else
                gBattlescriptCurrInstr = cmd->nextInstr;
        }
        else
            gBattlescriptCurrInstr = cmd->nextInstr;

        hitmarkerFaintBits = gHitMarker >> 0x1C;
        gBattlerFainted = 0;
	    
        while (TRUE)
        {
            if ((gBitTable[gBattlerFainted] & hitmarkerFaintBits) || gBattlerFainted >= gBattlersCount)
                break;
            ++gBattlerFainted;
        }

        if (gBattlerFainted == gBattlersCount)
            gBattlescriptCurrInstr = jumpPtr;
    }
    else
    {
        if (cmd->battler & OPEN_PARTY_ALLOW_CANCEL)
            hitmarkerFaintBits = PARTY_ACTION_CHOOSE_MON; // Used here as the caseId for the EmitChoose function.
        else
            hitmarkerFaintBits = PARTY_ACTION_SEND_OUT;
		
        battlerId = GetBattlerForBattleScript(cmd->battler & ~(OPEN_PARTY_ALLOW_CANCEL));
	    
        if (gSpecialStatuses[battlerId].faintedHasReplacement)
            gBattlescriptCurrInstr = cmd->nextInstr;
        else if (HasNoMonsToSwitch(battlerId, PARTY_SIZE, PARTY_SIZE))
        {
            gAbsentBattlerFlags |= gBitTable[battlerId];
            gHitMarker &= ~(HITMARKER_FAINTED(battlerId));
            gBattlescriptCurrInstr = jumpPtr;
        }
        else
        {
            gBattleStruct->battlers[battlerId].partyIndex = gBattlerPartyIndexes[battlerId];
            BtlController_EmitChoosePokemon(battlerId, BUFFER_A, hitmarkerFaintBits, gBattleStruct->battlers[BATTLE_PARTNER(battlerId)].monToSwitchIntoId, gBattleStruct->battlerPartyOrders[battlerId]);
            MarkBattlerForControllerExec(battlerId);
            gBattlescriptCurrInstr = cmd->nextInstr;

            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                for (i = 0; i < gBattlersCount; ++i)
                {
                    if (i != battlerId)
                    {
                        BtlController_EmitLinkStandbyMsg(i, BUFFER_A, 2);
                        MarkBattlerForControllerExec(i);
                    }
                }
            }
            else
            {
                battlerId = GetBattlerAtPosition(BATTLE_OPPOSITE(GetBattlerPosition(battlerId)));
		    
                if (gAbsentBattlerFlags & gBitTable[battlerId])
                    battlerId = BATTLE_PARTNER(battlerId);

                BtlController_EmitLinkStandbyMsg(battlerId, BUFFER_A, 2);
                MarkBattlerForControllerExec(battlerId);
            }
        }
    }
}

static void atk51_switchhandleorder(void)
{
	CMD_ARGS(u8 battler, u8 caseId);

    if (!gBattleControllerExecFlags)
    {
        u8 i, battlerId = GetBattlerForBattleScript(cmd->battler);
	    
        switch (cmd->caseId)
        {
			case 0:
                for (i = 0; i < gBattlersCount; ++i)
				{
					if (gBattleBufferB[i][0] == 0x22)
						gBattleStruct->battlers[i].monToSwitchIntoId = gBattleBufferB[i][1];
				}
				break;
			case 1:
                if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
					UpdatePartyOwnerOnSwitch_NonMulti(battlerId);
				break;
			case 2:
                gBattleCommunication[MULTIUSE_STATE] = gBattleBufferB[battlerId][1];
                gBattleStruct->battlers[battlerId].monToSwitchIntoId = gBattleBufferB[battlerId][1];
				
				if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
				{
					*(battlerId * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) &= 0xF;
					*(battlerId * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) |= (gBattleBufferB[battlerId][2] & 0xF0);
					*(battlerId * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 1) = gBattleBufferB[battlerId][3];
					*((BATTLE_PARTNER(battlerId)) * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) &= (0xF0);
					*((BATTLE_PARTNER(battlerId)) * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) |= (gBattleBufferB[battlerId][2] & 0xF0) >> 4;
					*((BATTLE_PARTNER(battlerId)) * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 2) = gBattleBufferB[battlerId][3];
				}
				else
					UpdatePartyOwnerOnSwitch_NonMulti(battlerId);
				
				PrepareSpeciesBuffer(gBattleTextBuff1, gBattleMons[gBattlerAttacker].species);
				PrepareMonNickBuffer(gBattleTextBuff2, battlerId, gBattleBufferB[battlerId][1]);
				break;
        }
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static bool8 ShouldPostponeSwitchInAbilities(u8 battlerId)
{
	u8 battler1 = BATTLE_OPPOSITE(battlerId), battler2 = BATTLE_PARTNER(battler1);
	bool8 aliveFirstMon = IsBattlerAlive(battler1);
	bool8 aliveSecondMon = IsBattlerAlive(battler2);
	
	// No pokemon on opposing side - postopone.
    if (!aliveFirstMon && !aliveSecondMon)
        return TRUE;
	
	// Checks for double battle, so abilities like Intimidate wait until all battlers are switched-in before activating.
    if (IsDoubleBattleForBattler(battler1))
    {
        if (aliveFirstMon && !aliveSecondMon && !HasNoMonsToSwitch(battler1, PARTY_SIZE, PARTY_SIZE))
            return TRUE;
        if (!aliveFirstMon && aliveSecondMon && !HasNoMonsToSwitch(battler2, PARTY_SIZE, PARTY_SIZE))
            return TRUE;
    }
    return FALSE;
}

enum
{
	SWITCHIN_UPDATE_VARS,
	SWITCHIN_NEUTRALIZING_GAS,
	SWITCHIN_UNNERVE,
	SWITCHIN_HEALING_WISH,
	SWITCHIN_LUNAR_DANCE,
	SWITCHIN_HEAL_REPLACEMENT,
	SWITCHIN_HAZARDS,
	SWITCHIN_EMERGENCY_EXIT,
	SWITCHIN_TRUANT,
	SWITCHIN_ABILITIES,
	SWITCHIN_COUNT,
};

static bool8 IsBattlerPPMaxed(u8 battlerId)
{
	u8 i;
	u16 move;
	
	for (i = 0; i < MAX_MON_MOVES; i++)
	{
		move = gBattleMons[battlerId].moves[i];
		
		if (move && gBattleMons[battlerId].pp[i] < CalculatePPWithBonus(move, gBattleMons[battlerId].ppBonuses, i))
			return FALSE;
	}
	return TRUE;
}

static void atk52_switchineffects(void)
{
	CMD_ARGS(u8 battler);

	u8 i, battlerId = GetBattlerForBattleScript(cmd->battler);
	
	switch (gBattleScripting.switchinEffectState)
	{
		case SWITCHIN_UPDATE_VARS:
		    UpdateSentPokesToOpponentValue(battlerId);
			
			gHitMarker &= ~(HITMARKER_FAINTED(battlerId));
			gSpecialStatuses[battlerId].faintedHasReplacement = FALSE;
			gBattleStruct->sides[GetBattlerSide(battlerId)].party[gBattlerPartyIndexes[battlerId]].appearedInBattle = TRUE;
			
			if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
				BattleAI_RecordPartyIndex(battlerId);

			ResetAllQueuedEffectsDone();
			SaveBattlersHps(); // For Emergency Exit
				
			++gBattleScripting.switchinEffectState;
			// Fallthrough
		case SWITCHIN_NEUTRALIZING_GAS: // Neutralizing Gas announces itself before hazards
		    if (GetBattlerAbility(battlerId) == ABILITY_NEUTRALIZING_GAS)
			{
				SaveAttackerToStack(battlerId);
				BattleScriptCall(BattleScript_NeutralizingGasActivatesRet);
			}
			++gBattleScripting.switchinEffectState;
			break;
		case SWITCHIN_UNNERVE: // Unnerve announces itself before hazards
		{
			u16 ability = GetBattlerAbility(battlerId);

		    if (ability == ABILITY_UNNERVE || ability == ABILITY_AS_ONE_ICE_RIDER || ability == ABILITY_AS_ONE_SHADOW_RIDER)
			{
				if (ability == ABILITY_UNNERVE)
				{
					PrepareMonTeamPrefixBuffer(gBattleTextBuff1, BATTLE_OPPOSITE(battlerId));
					gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_NERVOUS_TO_EAT; // Unnerve message
				}
				else
					gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_HAS_TWO_ABILITIES; // As One message
				
				gBattleScripting.battler = battlerId;
				BattleScriptCall(BattleScript_SwitchInAbilityMsgRet);
			}
			++gBattleScripting.switchinEffectState;
			break;
		}
		case SWITCHIN_HEALING_WISH:
		    if (gBattleStruct->battlers[battlerId].storedHealingWish && (!BATTLER_MAX_HP(battlerId) || gBattleMons[battlerId].status1.id))
			{
				gBattleStruct->battlers[battlerId].storedHealingWish = FALSE;
				gBattleScripting.battler = battlerId;
				gBattleMoveDamage = gBattleMons[battlerId].maxHP * -1;
				gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_HEALING_WISH_TRUE;
				gBattleScripting.animArg1 = B_ANIM_HEALING_WISH_HEAL;
				BattleScriptCall(BattleScript_HealingWishActivates);
			}
			++gBattleScripting.switchinEffectState;
			break;
		case SWITCHIN_LUNAR_DANCE:
		    if (gBattleStruct->battlers[battlerId].storedLunarDance && (!BATTLER_MAX_HP(battlerId) || gBattleMons[battlerId].status1.id || !IsBattlerPPMaxed(battlerId)))
			{
				gBattleStruct->battlers[battlerId].storedLunarDance = FALSE;
				gBattleScripting.battler = battlerId;
				gBattleMoveDamage = gBattleMons[battlerId].maxHP * -1;
				BattleScriptCall(BattleScript_LunarDanceActivates);
			}
		    ++gBattleScripting.switchinEffectState;
			break;
		case SWITCHIN_HEAL_REPLACEMENT: // TODO: Z-Healing Wish
		    ++gBattleScripting.switchinEffectState;
			break;
		case SWITCHIN_HAZARDS:
		    if (!TryDoQueuedBattleEffectsInSideList(battlerId, gEntryHazardsQueuedEffectIds, QueuedEffects_DoEntryHazardsEffects))
				++gBattleScripting.switchinEffectState;
			break;
		case SWITCHIN_EMERGENCY_EXIT:
		    if (TryActivateEmergencyExit(battlerId))
			{
				gBattlerTarget = battlerId;
				BattleScriptCall(BattleScript_EmergencyExit);
			}
			++gBattleScripting.switchinEffectState;
			break;
		case SWITCHIN_TRUANT:
		    if (GetBattlerAbility(battlerId) == ABILITY_TRUANT)
				gDisableStructs[battlerId].truantCounter = 1;
			
			++gBattleScripting.switchinEffectState;
			// Fallthrough
		case SWITCHIN_ABILITIES:
		    // Don't activate switch-in abilities if the opposing field is empty.
			// This could happen when a mon uses explosion and causes everyone to faint.
			if (ShouldPostponeSwitchInAbilities(battlerId) || gBattleStruct->switchInAbilityPostponed)
				gBattleStruct->switchInAbilityPostponed |= gBitTable[battlerId];
			else if (DoSwitchInAbilitiesItems(battlerId))
				break;
			
			++gBattleScripting.switchinEffectState;
			// Fallthrough
		case SWITCHIN_COUNT:
		    for (i = 0; i < gBattlersCount; ++i)
			{
				if (gBattlerByTurnOrder[i] == battlerId)
					gActionsByTurnOrder[i] = B_ACTION_CANCEL_PARTNER;
				
				gBattleStruct->sides[GetBattlerSide(i)].hpOnSwitchout = gBattleMons[i].hp;
			}
			if (cmd->battler == BS_UNKNOWN_5)
			{
				u32 hitmarkerFaintBits = gHitMarker >> 0x1C;
				
				++gBattlerFainted;
				while (TRUE)
				{
					if (((hitmarkerFaintBits & gBitTable[gBattlerFainted]) && !(gAbsentBattlerFlags & gBitTable[gBattlerFainted])) || gBattlerFainted >= gBattlersCount)
						break;
					++gBattlerFainted;
				}
			}
			gBattlescriptCurrInstr = cmd->nextInstr;
			break;
	}
}

static void atk53_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk54_playse(void)
{
	CMD_ARGS(u16 songId);
    BtlController_EmitPlaySE(gBattlerAttacker, BUFFER_A, cmd->songId);
    MarkBattlerForControllerExec(gBattlerAttacker);
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk55_fanfare(void)
{
	CMD_ARGS(u16 songId);
    BtlController_EmitPlayFanfare(gBattlerAttacker, BUFFER_A, cmd->songId);
    MarkBattlerForControllerExec(gBattlerAttacker);
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk56_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk57_endlinkbattle(void)
{
	CMD_ARGS();
    u8 battlerId = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
    BtlController_EmitEndLinkBattle(battlerId, BUFFER_A, gBattleOutcome);
    MarkBattlerForControllerExec(battlerId);
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk58_returntoball(void)
{
	CMD_ARGS(u8 battler);
    u8 battlerId = GetBattlerForBattleScript(cmd->battler);
	BtlController_EmitReturnMonToBall(battlerId, BUFFER_A, 1);
	MarkBattlerForControllerExec(battlerId);
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void SetBattlerMoveSlot(u8 battlerId, u16 move, u8 slot)
{
    gBattleMons[battlerId].moves[slot] = move;
    gBattleMons[battlerId].pp[slot] = gBattleMoves[move].pp;
}

static void GiveMoveToBattler(u8 battlerId, u16 move)
{
    u8 i;

    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (!gBattleMons[battlerId].moves[i])
        {
			SetBattlerMoveSlot(battlerId, move, i);
            return;
        }
    }
}

static void atk59_handlelearnnewmove(void)
{
	CMD_ARGS(const u8 *learnedMovePtr, const u8 *failPtr, bool8 isFirstMove);

    u8 battlerId, ret = MonTryLearningNewMove(&gPlayerParty[gBattleStruct->expGetterMonId], cmd->isFirstMove);
    
    while (ret == MON_ALREADY_KNOWS_MOVE)
        ret = MonTryLearningNewMove(&gPlayerParty[gBattleStruct->expGetterMonId], FALSE);

	switch (ret)
	{
		case MON_DONT_FIND_MOVE_TO_LEARN:
			gBattlescriptCurrInstr = cmd->failPtr;
			break;
		case MON_HAS_MAX_MOVES:
			gBattlescriptCurrInstr = cmd->nextInstr;
			break;
		default:
			battlerId = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
	    
			if (gBattlerPartyIndexes[battlerId] == gBattleStruct->expGetterMonId && !(gBattleMons[battlerId].status2 & STATUS2_TRANSFORMED))
				GiveMoveToBattler(battlerId, gMoveToLearn);
			
			if (IsDoubleBattleForBattler(battlerId))
			{
				battlerId = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
			
				if (gBattlerPartyIndexes[battlerId] == gBattleStruct->expGetterMonId && !(gBattleMons[battlerId].status2 & STATUS2_TRANSFORMED))
					GiveMoveToBattler(battlerId, gMoveToLearn);
			}
			gBattlescriptCurrInstr = cmd->learnedMovePtr;
	}
}

static inline void RemoveBattlerPPBonus(u8 battlerId, u8 moveIndex)
{
    gBattleMons[battlerId].ppBonuses &= gPPUpSetMask[moveIndex];
}

static void atk5A_yesnoboxlearnmove(void)
{
	CMD_ARGS(const u8 *learnedMovePtr);
	
    switch (gBattleScripting.learnMoveState)
    {
    case 0:
        HandleBattleWindow(0x17, 8, 0x1D, 0xD, 0);
        BattlePutTextOnWindow(gText_BattleYesNoChoice, B_WIN_YESNO);
        gBattleCommunication[CURSOR_POSITION] = 0;
        BattleCreateYesNoCursorAt();
		++gBattleScripting.learnMoveState;
        break;
    case 1:
        if (JOY_NEW(DPAD_UP) && gBattleCommunication[CURSOR_POSITION])
        {
            PlaySE(SE_SELECT);
            BattleDestroyYesNoCursorAt();
            gBattleCommunication[CURSOR_POSITION] = 0;
            BattleCreateYesNoCursorAt();
        }
        if (JOY_NEW(DPAD_DOWN) && gBattleCommunication[CURSOR_POSITION] == 0)
        {
            PlaySE(SE_SELECT);
            BattleDestroyYesNoCursorAt();
            gBattleCommunication[CURSOR_POSITION] = 1;
            BattleCreateYesNoCursorAt();
        }
        if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);

            if (gBattleCommunication[CURSOR_POSITION] == 0)
            {
                HandleBattleWindow(0x17, 0x8, 0x1D, 0xD, WINDOW_CLEAR);
                BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
                ++gBattleScripting.learnMoveState;
            }
            else
                gBattleScripting.learnMoveState = 4;
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            gBattleScripting.learnMoveState = 4;
        }
        break;
    case 2:
        if (!gPaletteFade.active)
        {
            FreeAllWindowBuffers();
            ShowSelectMovePokemonSummaryScreen(gPlayerParty, gBattleStruct->expGetterMonId, gPlayerPartyCount - 1, ReshowBattleScreenAfterMenu, gMoveToLearn);
            ++gBattleScripting.learnMoveState;
        }
        break;
    case 3:
        if (!gPaletteFade.active && gMain.callback2 == BattleMainCB2)
        {
            u8 movePosition = GetMoveSlotToReplace();

            if (movePosition == MAX_MON_MOVES)
                gBattleScripting.learnMoveState = 4;
            else
            {
				PrepareMoveBuffer(gBattleTextBuff2, GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_MOVE1 + movePosition));
				RemoveMonPPBonus(&gPlayerParty[gBattleStruct->expGetterMonId], movePosition);
				SetMonMoveSlot(&gPlayerParty[gBattleStruct->expGetterMonId], gMoveToLearn, movePosition);
				
				if (gBattlerPartyIndexes[0] == gBattleStruct->expGetterMonId && MOVE_IS_PERMANENT(0, movePosition))
				{
					RemoveBattlerPPBonus(0, movePosition);
					SetBattlerMoveSlot(0, gMoveToLearn, movePosition);
				}

				if (IsDoubleBattleOnSide(B_SIDE_PLAYER) && gBattlerPartyIndexes[2] == gBattleStruct->expGetterMonId && MOVE_IS_PERMANENT(2, movePosition))
				{
					RemoveBattlerPPBonus(2, movePosition);
					SetBattlerMoveSlot(2, gMoveToLearn, movePosition);
				}
				gBattlescriptCurrInstr = cmd->learnedMovePtr;
            }
        }
        break;
    case 4:
        HandleBattleWindow(0x17, 8, 0x1D, 0xD, WINDOW_CLEAR);
        gBattlescriptCurrInstr = cmd->nextInstr;
        break;
    case 5:
        if (!gBattleControllerExecFlags)
            gBattleScripting.learnMoveState = 2;
        break;
    }
}

static void atk5B_yesnoboxstoplearningmove(void)
{
	CMD_ARGS(const u8 *ptr);

    switch (gBattleScripting.learnMoveState)
    {
    case 0:
        HandleBattleWindow(0x17, 8, 0x1D, 0xD, 0);
        BattlePutTextOnWindow(gText_BattleYesNoChoice, B_WIN_YESNO);
        gBattleCommunication[CURSOR_POSITION] = 0;
        BattleCreateYesNoCursorAt();
		++gBattleScripting.learnMoveState;
        break;
    case 1:
        if (JOY_NEW(DPAD_UP) && gBattleCommunication[CURSOR_POSITION])
        {
            PlaySE(SE_SELECT);
            BattleDestroyYesNoCursorAt();
            gBattleCommunication[CURSOR_POSITION] = 0;
            BattleCreateYesNoCursorAt();
        }
        if (JOY_NEW(DPAD_DOWN) && gBattleCommunication[CURSOR_POSITION] == 0)
        {
            PlaySE(SE_SELECT);
            BattleDestroyYesNoCursorAt();
            gBattleCommunication[CURSOR_POSITION] = 1;
            BattleCreateYesNoCursorAt();
        }
        if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);

            if (gBattleCommunication[CURSOR_POSITION])
                gBattlescriptCurrInstr = cmd->ptr;
            else
                gBattlescriptCurrInstr = cmd->nextInstr;
			
            HandleBattleWindow(0x17, 0x8, 0x1D, 0xD, WINDOW_CLEAR);
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            gBattlescriptCurrInstr = cmd->ptr;
            HandleBattleWindow(0x17, 0x8, 0x1D, 0xD, WINDOW_CLEAR);
        }
        break;
    }
}

static void atk5C_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk5D_getmoneyreward(void)
{
	CMD_ARGS(const u8 *ptr);

    u32 moneyReward;

    if (gBattleOutcome == B_OUTCOME_WON)
    {
		moneyReward = 4 * GetTrainerPartyMonLevel(gTrainers[gTrainerBattleOpponent_A].party[gTrainers[gTrainerBattleOpponent_A].partySize - 1]);
		moneyReward *= gTrainerClassTable[gTrainers[gTrainerBattleOpponent_A].trainerClass].moneyMultiplier;
		
		if (gBattleStruct->moneyMultiplier)
			moneyReward *= 2;
		
		if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			moneyReward *= 2;

        AddMoney(&gSaveBlock1Ptr->money, moneyReward);
    }
    else
        moneyReward = ComputeWhiteOutMoneyLoss();
	
    PrepareWordNumberBuffer(gBattleTextBuff1, 5, moneyReward);
	
    if (moneyReward)
        gBattlescriptCurrInstr = cmd->nextInstr;
    else
        gBattlescriptCurrInstr = cmd->ptr;
}

static void atk5E_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk5F_swapattackerwithtarget(void)
{
	CMD_ARGS();
	u8 temp;
	SWAP(gBattlerAttacker, gBattlerTarget, temp);
	gHitMarker ^= HITMARKER_SWAP_ATTACKER_TARGET;
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk60_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk61_drawpartystatussummary(void)
{
	CMD_ARGS(u8 battler);

    if (!gBattleControllerExecFlags)
    {
        u8 i, battlerId = GetBattlerForBattleScript(cmd->battler);
		struct Pokemon *party = GetBattlerParty(battlerId);
		struct HpAndStatus hpStatuses[PARTY_SIZE];

        for (i = 0; i < PARTY_SIZE; ++i)
        {
            if (!IsMonValidSpecies(&party[i]))
            {
                hpStatuses[i].hp = 0xFFFF;
                hpStatuses[i].status.id = 0;
				hpStatuses[i].status.counter = 0;
            }
            else
            {
                hpStatuses[i].hp = GetMonData(&party[i], MON_DATA_HP);
                hpStatuses[i].status.id = GetMonData(&party[i], MON_DATA_STATUS_ID);
				hpStatuses[i].status.counter = GetMonData(&party[i], MON_DATA_STATUS_COUNTER);
            }
        }
        BtlController_EmitDrawPartyStatusSummary(battlerId, BUFFER_A, hpStatuses, 1);
        MarkBattlerForControllerExec(battlerId);
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atk62_hidepartystatussummary(void)
{
	CMD_ARGS(u8 battler);
    u8 battlerId = GetBattlerForBattleScript(cmd->battler);
    BtlController_EmitHidePartyStatusSummary(battlerId, BUFFER_A);
    MarkBattlerForControllerExec(battlerId);
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void CallAnotherMove(u16 move)
{
	gCurrentMove = move;
	gBattlerTarget = GetMoveTarget(move, 0);
	SetTypeBeforeUsingMove(move, gBattlerAttacker);
	gBattleStruct->atkCancellerTracker = CANCELLER_RECALL_CASEID;
	gBattleStruct->moveEffect.moveEffectByte = MOVE_EFFECT_NONE;
	gSpecialStatuses[gBattlerAttacker].parentalBondState = PARENTAL_BOND_OFF;
	gMultiHitCounter = 0;
	gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
	gBattlescriptCurrInstr = GET_MOVE_BATTLESCRIPT(move);
}

static void atk63_jumptocalledmove(void)
{
	CMD_ARGS(bool8 setMoveChosen);

	if (cmd->setMoveChosen)
		gChosenMove = gCalledMove;
	
	CallAnotherMove(gCalledMove);
}

static void atk64_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk65_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk66_chosenstatusanimation(void)
{
	CMD_ARGS(u8 battler, u8 statusId, u32 status);

    if (!gBattleControllerExecFlags)
    {
        u8 battlerId = GetBattlerForBattleScript(cmd->battler);
	    
        if (!(gStatuses3[battlerId] & (STATUS3_SEMI_INVULNERABLE | STATUS3_COMMANDING)) && !gDisableStructs[battlerId].substituteHP && IsBattleAnimationsOn())
        {
            BtlController_EmitStatusAnimation(battlerId, BUFFER_A, cmd->statusId, cmd->status);
            MarkBattlerForControllerExec(battlerId);
        }
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atk67_yesnobox(void)
{
	CMD_ARGS();

    switch (gBattleCommunication[MULTIUSE_STATE])
    {
		case 0:
            HandleBattleWindow(0x17, 8, 0x1D, 0xD, 0);
			BattlePutTextOnWindow(gText_BattleYesNoChoice, B_WIN_YESNO);
			gBattleCommunication[CURSOR_POSITION] = 0;
			BattleCreateYesNoCursorAt();
			++gBattleCommunication[MULTIUSE_STATE];
			break;
		case 1:
            if (JOY_NEW(DPAD_UP) && gBattleCommunication[CURSOR_POSITION])
			{
				PlaySE(SE_SELECT);
				BattleDestroyYesNoCursorAt();
				gBattleCommunication[CURSOR_POSITION] = 0;
				BattleCreateYesNoCursorAt();
			}
			if (JOY_NEW(DPAD_DOWN) && gBattleCommunication[CURSOR_POSITION] == 0)
			{
				PlaySE(SE_SELECT);
				BattleDestroyYesNoCursorAt();
				gBattleCommunication[CURSOR_POSITION] = 1;
				BattleCreateYesNoCursorAt();
			}
			if (JOY_NEW(B_BUTTON))
			{
				gBattleCommunication[CURSOR_POSITION] = 1;
				PlaySE(SE_SELECT);
				HandleBattleWindow(0x17, 8, 0x1D, 0xD, WINDOW_CLEAR);
				gBattlescriptCurrInstr = cmd->nextInstr;
			}
			else if (JOY_NEW(A_BUTTON))
			{
				PlaySE(SE_SELECT);
				HandleBattleWindow(0x17, 8, 0x1D, 0xD, WINDOW_CLEAR);
				gBattlescriptCurrInstr = cmd->nextInstr;
			}
			break;
    }
}

static void atk68_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk69_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void TryCheekPouch(u8 battlerId, u16 itemId)
{
	if (GetBattlerAbility(battlerId) == ABILITY_CHEEK_POUCH && ItemId_GetPocket(itemId) == POCKET_BERRY_POUCH && !(gStatuses3[battlerId] & STATUS3_HEAL_BLOCK) && !BATTLER_MAX_HP(battlerId))
	{
		gBattleMoveDamage = gBattleMons[battlerId].maxHP / 3;
		if (gBattleMoveDamage == 0)
			gBattleMoveDamage = 1;
		gBattleMoveDamage *= -1;
		
		SaveTargetToStack(battlerId);
		BattleScriptCall(BattleScript_CheekPouchActivates);
	}
}

static void TrySymbiosis(u8 battlerId)
{
	u8 partner = BATTLE_PARTNER(battlerId);
	
	if (IsBattlerAlive(partner) && GetBattlerAbility(partner) == ABILITY_SYMBIOSIS && gBattleMons[partner].item && CanBattlerGetOrLoseItem(battlerId, gBattleMons[partner].item)
	&& CanBattlerGetOrLoseItem(partner, gBattleMons[partner].item))
	{
		GiveItemToBattler(battlerId, gBattleMons[partner].item);
		RemoveBattlerItem(partner);
		
		SaveAttackerToStack(battlerId);
		SaveTargetToStack(partner);
		
		BattleScriptCall(BattleScript_SymbiosisActivation);
	}
}

static void atk6A_consumeitem(void)
{
	CMD_ARGS(u8 battler, u8 flags);

    u8 battlerId = GetBattlerForBattleScript(cmd->battler), flags = cmd->flags;
	
	*GetUsedHeldItemPtr(battlerId) = gBattleMons[battlerId].item;
	RemoveBattlerItem(battlerId);
	RemoveOrAddBattlerOnPickupStack(battlerId, TRUE);
		
	// if (flags & ATK6A_SET_BELCH)
		// Set belch bit
	
	gBattlescriptCurrInstr = cmd->nextInstr;
	
	if (flags & ATK6A_FLAG_CHECK_CHEEK_POUCH)
		TryCheekPouch(battlerId, gLastUsedItem);
	
	if (!(flags & ATK6A_FLAG_DONT_CHECK_SYMBIOSIS))
		TrySymbiosis(battlerId);
}

static void atk6B_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk6C_drawlvlupbox(void)
{
	CMD_ARGS();

    if (gBattleScripting.atk6C_state == 0)
		gBattleScripting.atk6C_state = IsMonGettingExpSentOut() ? 3 : 1;

    switch (gBattleScripting.atk6C_state)
    {
		case 1:
            gBattle_BG2_Y = 0x60;
			SetBgAttribute(2, BG_ATTR_PRIORITY, 0);
			ShowBg(2);
			InitLvlUpBox();
			gBattleScripting.atk6C_state = 2;
			break;
		case 2:
            if (!SlideInLvlUpBox())
				gBattleScripting.atk6C_state = 3;
			break;
		case 3:
            gBattle_BG1_X = 0;
			gBattle_BG1_Y = 0x100;
			SetBgAttribute(0, BG_ATTR_PRIORITY, 1);
			SetBgAttribute(1, BG_ATTR_PRIORITY, 0);
			ShowBg(0);
			ShowBg(1);
			HandleBattleWindow(18, 7, 0x1D, 0x13, WINDOW_x80);
			gBattleScripting.atk6C_state = 4;
			break;
		case 4:
            DrawLevelUpWindow1();
			PutWindowTilemap(12);
			CopyWindowToVram(12, COPYWIN_BOTH);
			++gBattleScripting.atk6C_state;
			break;
		case 5:
		case 7:
		    if (!IsDma3ManagerBusyWithBgCopy())
			{
				gBattle_BG1_Y = 0;
				++gBattleScripting.atk6C_state;
			}
			break;
		case 6:
            if (gMain.newKeys)
			{
				PlaySE(SE_SELECT);
				DrawLevelUpWindow2();
				CopyWindowToVram(12, COPYWIN_GFX);
				++gBattleScripting.atk6C_state;
			}
			break;
		case 8:
            if (gMain.newKeys)
			{
				PlaySE(SE_SELECT);
				HandleBattleWindow(18, 7, 0x1D, 0x13, WINDOW_x80 | WINDOW_CLEAR);
				++gBattleScripting.atk6C_state;
			}
			break;
		case 9:
            if (!SlideOutLvlUpBox())
			{
				ClearWindowTilemap(13);
				CopyWindowToVram(13, COPYWIN_MAP);
				ClearWindowTilemap(12);
				CopyWindowToVram(12, COPYWIN_MAP);
				SetBgAttribute(2, BG_ATTR_PRIORITY, 2);
				ShowBg(2);
				gBattleScripting.atk6C_state = 10;
			}
			break;
		case 10:
            if (!IsDma3ManagerBusyWithBgCopy())
			{
				SetBgAttribute(0, BG_ATTR_PRIORITY, 0);
				SetBgAttribute(1, BG_ATTR_PRIORITY, 1);
				ShowBg(0);
				ShowBg(1);
				gBattlescriptCurrInstr = cmd->nextInstr;
			}
			break;
    }
}

static void DrawLevelUpWindow1(void)
{
    u16 currStats[NUM_STATS];
    GetMonLevelUpWindowStats(&gPlayerParty[gBattleStruct->expGetterMonId], currStats);
    DrawLevelUpWindowPg1(12, gBattleResources->beforeLvlUp->stats, currStats, 0xE, 0xD, 0xF);
}

static void DrawLevelUpWindow2(void)
{
    u16 currStats[NUM_STATS];
    GetMonLevelUpWindowStats(&gPlayerParty[gBattleStruct->expGetterMonId], currStats);
    DrawLevelUpWindowPg2(12, currStats, 0xE, 0xD, 0xF);
}

static void InitLvlUpBox(void)
{
    gBattle_BG2_Y = 0;
    gBattle_BG2_X = 0x1A0;
    LoadPalette(sUnknownBattleboxPal, 0x60, 0x20);
    CopyToWindowPixelBuffer(13, sUnknownBattleboxGfx, 0, 0);
    PutWindowTilemap(13);
    CopyWindowToVram(13, COPYWIN_BOTH);
    PutMonIconOnLvlUpBox();
}

static bool8 SlideInLvlUpBox(void)
{
    if (IsDma3ManagerBusyWithBgCopy())
        return TRUE;
    else if (gBattle_BG2_X == 0x200)
        return FALSE;
    else if (gBattle_BG2_X == 0x1A0)
		PutLevelAndGenderOnLvlUpBox();
	
    gBattle_BG2_X += 8;
    if (gBattle_BG2_X >= 0x200)
        gBattle_BG2_X = 0x200;
	
    return (gBattle_BG2_X != 0x200);
}

static void PutLevelAndGenderOnLvlUpBox(void)
{
    u8 monGender, *txtPtr, *txtPtr2;
    struct TextPrinterTemplate printerTemplate = {0};

    GetMonNickname(&gPlayerParty[gBattleStruct->expGetterMonId], gStringVar4);
	
    printerTemplate.currentChar = gStringVar4;
    printerTemplate.windowId = 13;
    printerTemplate.x = 32;
    printerTemplate.currentX = 32;
    printerTemplate.fgColor = TEXT_COLOR_WHITE;
    printerTemplate.bgColor = TEXT_COLOR_TRANSPARENT;
    printerTemplate.shadowColor = TEXT_COLOR_DARK_GRAY;
    AddTextPrinter(&printerTemplate, 0xFF, NULL);
    txtPtr = gStringVar4;
    gStringVar4[0] = 0xF9;
    *++txtPtr = 5;
    *++txtPtr = 0;
    txtPtr2 = txtPtr + 1;
    txtPtr = ConvertIntToDecimalStringN(++txtPtr, GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_LEVEL), STR_CONV_MODE_LEFT_ALIGN, 3);
    txtPtr = StringFill(txtPtr, 0, 5);
    txtPtr = txtPtr2 + 4;
	
	monGender = GetMonGender(&gPlayerParty[gBattleStruct->expGetterMonId]);
	
    if (monGender != MON_GENDERLESS)
    {
        if (monGender == MON_MALE)
        {
            txtPtr = WriteColorChangeControlCode(txtPtr, 0, 0xC);
            txtPtr = WriteColorChangeControlCode(txtPtr, 1, 0xD);
            *(txtPtr++) = CHAR_MALE;
        }
        else
        {
            txtPtr = WriteColorChangeControlCode(txtPtr, 0, 0xE);
            txtPtr = WriteColorChangeControlCode(txtPtr, 1, 0xF);
            *(txtPtr++) = CHAR_FEMALE;
        }
        *(txtPtr++) = EOS;
    }
    printerTemplate.y = 10;
    printerTemplate.currentY = 10;
    AddTextPrinter(&printerTemplate, TEXT_SPEED_FF, NULL);
    CopyWindowToVram(13, COPYWIN_GFX);
}

static bool8 SlideOutLvlUpBox(void)
{
    if (gBattle_BG2_X == 0x1A0)
        return FALSE;
    else if (gBattle_BG2_X - 16 < 0x1A0)
        gBattle_BG2_X = 0x1A0;
    else
        gBattle_BG2_X -= 16;
	
    return (gBattle_BG2_X != 0x1A0);
}

#define sDestroy                    data[0]
#define sSavedLvlUpBoxXPosition     data[1]

static void PutMonIconOnLvlUpBox(void)
{
    u8 spriteId;
    struct SpriteSheet iconSheet;
    struct SpritePalette iconPalSheet;
    u16 species = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPECIES);

    iconSheet.data = GetMonIconPtr(species);
    iconSheet.size = 0x200;
    iconSheet.tag = MON_ICON_LVLUP_BOX_TAG;
	
    iconPalSheet.data = GetValidMonIconPalettePtr(species);
    iconPalSheet.tag = MON_ICON_LVLUP_BOX_TAG;
	
    LoadSpriteSheet(&iconSheet);
    LoadSpritePalette(&iconPalSheet);
	
    spriteId = CreateSprite(&sSpriteTemplate_MonIconOnLvlUpBox, 256, 10, 0);
    gSprites[spriteId].sDestroy = FALSE;
    gSprites[spriteId].sSavedLvlUpBoxXPosition = gBattle_BG2_X;
}

static void SpriteCB_MonIconOnLvlUpBox(struct Sprite* sprite)
{
    sprite->x2 = sprite->sSavedLvlUpBoxXPosition - gBattle_BG2_X;
	
    if (sprite->x2 != 0)
        sprite->sDestroy = TRUE;
    else if (sprite->sDestroy)
    {
        DestroySprite(sprite);
        FreeSpriteTilesByTag(MON_ICON_LVLUP_BOX_TAG);
        FreeSpritePaletteByTag(MON_ICON_LVLUP_BOX_TAG);
    }
}

bool32 IsMonGettingExpSentOut(void)
{
    if (gBattlerPartyIndexes[0] == gBattleStruct->expGetterMonId || (IsDoubleBattleOnSide(B_SIDE_PLAYER) && gBattlerPartyIndexes[2] == gBattleStruct->expGetterMonId))
        return TRUE;
	return FALSE;
}

static void atk6D_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk6E_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk6F_makevisible(void)
{
	CMD_ARGS(u8 battler);

    u8 battlerId = GetBattlerForBattleScript(cmd->battler);
	
	if (!(gStatuses3[battlerId] & STATUS3_COMMANDING))
	{
		BtlController_EmitSpriteInvisibility(battlerId, BUFFER_A, FALSE);
		MarkBattlerForControllerExec(battlerId);
	}
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk70_recordbattlerability(void)
{
	CMD_ARGS(u8 battler);
    BattleAI_RecordAbility(GetBattlerForBattleScript(cmd->battler));
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk71_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk72_jumpifplayerran(void)
{
	CMD_ARGS(const u8 *ptr);

    if (TryRunFromBattle(gBattlerFainted) && gProtectStructs[gBattlerFainted].fleeFlag != 2) // It don't count flee using Run Away in gen 5 onwards
        gBattlescriptCurrInstr = cmd->ptr;
    else
        gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk73_hpthresholds(void)
{
	CMD_ARGS(u8 battler);
	
	u8 opposingBattler = BATTLE_OPPOSITE(GetBattlerForBattleScript(cmd->battler));

    if (!IsDoubleBattleForBattler(opposingBattler))
    {
        s32 result = gBattleMons[opposingBattler].hp * 100 / gBattleMons[opposingBattler].maxHP;
	    
        if (result == 0)
            result = 1;
		
        if (result > 69 || !gBattleMons[opposingBattler].hp)
            gBattleStruct->hpScale = 0;
        else if (result > 39)
            gBattleStruct->hpScale = 1;
        else if (result > 9)
            gBattleStruct->hpScale = 2;
        else
            gBattleStruct->hpScale = 3;
    }
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk74_hpthresholds2(void)
{
	CMD_ARGS(u8 battler);
	
	u8 opposingBattler = BATTLE_OPPOSITE(GetBattlerForBattleScript(cmd->battler));
	
    if (!IsDoubleBattleForBattler(opposingBattler))
    {
        u16 hpSwitchout = gBattleStruct->sides[GetBattlerSide(opposingBattler)].hpOnSwitchout;
        s32 result = (hpSwitchout - gBattleMons[opposingBattler].hp) * 100 / hpSwitchout;

        if (gBattleMons[opposingBattler].hp >= hpSwitchout)
            gBattleStruct->hpScale = 0;
        else if (result <= 29)
            gBattleStruct->hpScale = 1;
        else if (result <= 69)
            gBattleStruct->hpScale = 2;
        else
            gBattleStruct->hpScale = 3;
    }
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk75_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

void BufferMoveToLearnIntoBattleTextBuff2(void)
{
    PrepareMoveBuffer(gBattleTextBuff2, gMoveToLearn);
}

static void atk76_various(void)
{
	CMD_ARGS(u8 battler, u8 id);

	s32 i;
	u8 battlerId = GetBattlerForBattleScript(cmd->battler);

    switch (cmd->id)
    {
		case VARIOUS_RESET_INTIMIDATE_TRACE_BITS:
		{
			VARIOUS_ARGS();
			gSpecialStatuses[battlerId].switchInAbilityDone = FALSE;
			break;
		}
		case VARIOUS_UPDATE_CHOICE_MOVE_ON_LVL_UP:
		{
			VARIOUS_ARGS();

            if (gBattlerPartyIndexes[0] == gBattleStruct->expGetterMonId || gBattlerPartyIndexes[2] == gBattleStruct->expGetterMonId)
			{
				battlerId = gBattlerPartyIndexes[0] == gBattleStruct->expGetterMonId ? 0 : 2;
				
				if (FindMoveSlotInBattlerMoveset(battlerId, gBattleStruct->battlers[battlerId].choicedMove) == MAX_MON_MOVES)
					gBattleStruct->battlers[battlerId].choicedMove = MOVE_NONE;
			}
			break;
		}
		case VARIOUS_RESET_PLAYER_FAINTED_FLAG:
		{
			VARIOUS_ARGS();

		    if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_DOUBLE)) && (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
			&& gBattleMons[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)].hp && gBattleMons[GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)].hp)
			    gHitMarker &= ~(HITMARKER_PLAYER_FAINTED);
				
			break;
		}
		case VARIOUS_CHECK_POKEFLUTE:
		{
			VARIOUS_ARGS();

			u32 data;
			struct Status1 status = {0};
			
			gBattleCommunication[MULTIUSE_STATE] = FALSE;
			
			// Cure all battlers
			for (i = 0; i < gBattlersCount; ++i)
			{
				if (GetBattlerAbility(i) != ABILITY_SOUNDPROOF && gBattleMons[i].status1.id == STATUS1_SLEEP)
					ClearBattlerStatus(i);
			}

			// Cure player's party
			for (i = 0, data = 0; i < PARTY_SIZE; ++i)
			{
				if (IsMonValidSpecies(&gPlayerParty[i]) && GetMonData(&gPlayerParty[i], MON_DATA_STATUS_ID) && GetMonAbility(&gPlayerParty[i]) != ABILITY_SOUNDPROOF)
					data |= (1 << i);
			}

			if (data)
			{
				u8 battler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
				BtlController_EmitSetMonData(battler, BUFFER_A, REQUEST_STATUS_BATTLE, data, sizeof(status), &status);
				MarkBattlerForControllerExec(battler);
				gBattleCommunication[MULTIUSE_STATE] = TRUE;
			}

			// Cure enemy's party
			for (i = 0, data = 0; i < PARTY_SIZE; ++i)
			{
				if (IsMonValidSpecies(&gEnemyParty[i]) && GetMonData(&gEnemyParty[i], MON_DATA_STATUS_ID) && GetMonAbility(&gEnemyParty[i]) != ABILITY_SOUNDPROOF)
					data |= (1 << i);
			}

			if (data)
			{
				u8 battler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
				BtlController_EmitSetMonData(battler, BUFFER_A, REQUEST_STATUS_BATTLE, data, sizeof(status), &status);
				MarkBattlerForControllerExec(battler);
				gBattleCommunication[MULTIUSE_STATE] = TRUE;
			}
			break;
		}
		case VARIOUS_WAIT_FANFARE:
		{
			VARIOUS_ARGS();

			if (!IsFanfareTaskInactive())
				return;
			
			break;
		}
		case VARIOUS_JUMP_IF_TARGET_ALLY:
		{
			VARIOUS_ARGS(const u8 *ptr);

			if (GetBattlerSide(gBattlerAttacker) != GetBattlerSide(gBattlerTarget))
				gBattlescriptCurrInstr = cmd->nextInstr;
			else
				gBattlescriptCurrInstr = cmd->ptr;
			
			return;
		}
		case VARIOUS_TRY_LAST_MON_TRAINER_SLIDE:
		{
			VARIOUS_ARGS();

			if (ShouldDoTrainerSlide(battlerId, TRAINER_SLIDE_LAST_MON_SEND_OUT))
			{
				gBattlescriptCurrInstr = cmd->nextInstr;
				BattleScriptCall(BattleScript_TrainerSlideMsg);
				return;
			}
			break;
		}
		case VARIOUS_UPDATE_HEALTHBOX_ATTRIBUTE:
		{
			VARIOUS_ARGS(u8 attributeId);
			UpdateHealthboxAttribute(battlerId, cmd->attributeId);
			gBattlescriptCurrInstr = cmd->nextInstr;
			return;
		}
		case VARIOUS_TRY_REMOVE_ILLUSION:
		{
			VARIOUS_ARGS();
			gBattlescriptCurrInstr = cmd->nextInstr;
			TryRemoveIllusion(battlerId);
			return;
		}
		case VARIOUS_PLAY_MOVE_ANIMATION:
		{
			VARIOUS_ARGS(u16 move);
			BtlController_EmitMoveAnimation(battlerId, BUFFER_A, cmd->move, gBattleScripting.animTurn, 0, 0, gBattleMons[battlerId].friendship, &gDisableStructs[battlerId]);
			MarkBattlerForControllerExec(battlerId);
			gBattlescriptCurrInstr = cmd->nextInstr;
			return;
		}
		case VARIOUS_JUMP_IF_BATTLE_END:
		{
			VARIOUS_ARGS(const u8 *ptr);

			if (NoAliveMonsForEitherParty())
				gBattlescriptCurrInstr = cmd->ptr;
			else
				gBattlescriptCurrInstr = cmd->nextInstr;
			
			return;
		}
		case VARIOUS_TRY_REMOVE_PRIMAL_WEATHER:
		{
			VARIOUS_ARGS();

			bool8 shouldNotClear = FALSE;

			for (i = 0; i < gBattlersCount; i++)
			{
				if (IsBattlerAlive(i))
				{
					u16 ability = GetBattlerAbility(i);
					
					if ((((gBattleWeather & B_WEATHER_RAIN_PRIMAL) && ability == ABILITY_PRIMORDIAL_SEA) || ((gBattleWeather & B_WEATHER_SUN_PRIMAL)
					&& ability == ABILITY_DESOLATE_LAND) || ((gBattleWeather & B_WEATHER_STRONG_WINDS) && ability == ABILITY_DELTA_STREAM)))
						shouldNotClear = TRUE;
				}
			}

			if (!shouldNotClear)
			{
				if (gBattleWeather & B_WEATHER_RAIN_PRIMAL)
				{
					gBattleWeather &= ~(B_WEATHER_RAIN_PRIMAL);
					PrepareStringBattle(STRINGID_HEAVYRAINLIFTED, battlerId);
					gBattleCommunication[MSG_DISPLAY] = TRUE;
				}
				else if (gBattleWeather & B_WEATHER_SUN_PRIMAL)
				{
					gBattleWeather &= ~(B_WEATHER_SUN_PRIMAL);
					PrepareStringBattle(STRINGID_EXTREMESUNLIGHTFADED, battlerId);
					gBattleCommunication[MSG_DISPLAY] = TRUE;
				}
				else if (gBattleWeather & B_WEATHER_STRONG_WINDS)
				{
					gBattleWeather &= ~(B_WEATHER_STRONG_WINDS);
					PrepareStringBattle(STRINGID_STRONGWINDSDISSIPATED, battlerId);
					gBattleCommunication[MSG_DISPLAY] = TRUE;
				}
			}
			break;
		}
		case VARIOUS_JUMP_IF_NOT_ALIVE:
		{
			VARIOUS_ARGS(const u8 *ptr);

			if (!IsBattlerAlive(battlerId))
				gBattlescriptCurrInstr = cmd->ptr;
			else
				gBattlescriptCurrInstr = cmd->nextInstr;
			
			return;
		}
		case VARIOUS_TRY_FRISK_TARGET:
		{
			VARIOUS_ARGS(const u8 *failPtr);

			if (gBattleMons[battlerId].item)
			{
				gLastUsedItem = gBattleMons[battlerId].item;
				BattleAI_RecordHoldEffect(battlerId);
				gBattlescriptCurrInstr = cmd->nextInstr;
			}
			else
				gBattlescriptCurrInstr = cmd->failPtr;
			
			return;
		}
		case VARIOUS_SWITCHIN_ABILITIES: // Activates switch in abilities on ability change
		{
			VARIOUS_ARGS();

			gBattlescriptCurrInstr = cmd->nextInstr;
		
			if (IsBattlerAlive(battlerId))
			{
				AbilityBattleEffects(ABILITYEFFECT_NEUTRALIZING_GAS, 0);
				AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, battlerId);
			}
			return;
		}
		case VARIOUS_TRY_END_NEUTRALIZING_GAS: // Try remove it when ability changed or suppresed
		{
			VARIOUS_ARGS();

			if (gSpecialStatuses[battlerId].removedNeutralizingGas)
			{
				gSpecialStatuses[battlerId].removedNeutralizingGas = FALSE;
				PrepareAbilityBuffer(gBattleTextBuff1, ABILITY_NEUTRALIZING_GAS);
				SaveAttackerToStack(battlerId);
				gBattlescriptCurrInstr = cmd->nextInstr;
				BattleScriptCall(BattleScript_NeutralizingGasExits);
				return;
			}
			break;
		}
		case VARIOUS_SET_TELEPORT_OUTCOME:
		{
			VARIOUS_ARGS();

			if (GetBattlerSide(battlerId) == B_SIDE_OPPONENT)
			{
				// Don't end the battle if one of the wild mons teleported from the wild double battle and its partner is still alive.
				if (IsBattlerAlive(BATTLE_PARTNER(battlerId)))
				{
					gAbsentBattlerFlags |= gBitTable[battlerId];
					gHitMarker |= HITMARKER_FAINTED(battlerId);
					gBattleMons[battlerId].hp = 0;
					SetMonData(&gEnemyParty[gBattlerPartyIndexes[battlerId]], MON_DATA_HP, &gBattleMons[battlerId].hp);
					SetHealthboxSpriteInvisible(gHealthboxSpriteIds[battlerId]);
					FaintClearSetData(battlerId);
				}
				else
					gBattleOutcome = B_OUTCOME_MON_TELEPORTED;
			}
			else
				gBattleOutcome = B_OUTCOME_PLAYER_TELEPORTED;
			
			break;
		}
		case VARIOUS_TRY_FAINT_ON_SPIKES_DAMAGE:
		{
			VARIOUS_ARGS(const u8 *ptr);

			if (gHitMarker & HITMARKER_FAINTED(battlerId))
			{
				const u8 *BS_ptr = cmd->ptr;
				BattleScriptPop();
				gBattlescriptCurrInstr = BS_ptr;
			}
			else
				gBattlescriptCurrInstr = cmd->nextInstr;
			
			return;
		}
		case VARIOUS_ACTIVATE_WEATHER_ABILITIES:
		{
			VARIOUS_ARGS();
			gBattlescriptCurrInstr = cmd->nextInstr;
			AbilityBattleEffects(ABILITYEFFECT_ON_WEATHER, battlerId);
			return;
		}
		case VARIOUS_TRY_NEUTRALIZING_GAS_SUPPRESSION:
		{
		    VARIOUS_ARGS(const u8 *failPtr);

			if (!gAbilities[gBattleMons[battlerId].ability].cantBeSuppressed)
			{
				SuppressBattlerAbility(battlerId);
				gBattlescriptCurrInstr = cmd->nextInstr;
			}
			else
				gBattlescriptCurrInstr = cmd->failPtr;
			
			return;
		}
		case VARIOUS_TRY_ABILITY_SUPPRESSION:
		{
		    VARIOUS_ARGS(const u8 *failPtr);

			if (!gAbilities[gBattleMons[battlerId].ability].cantBeSuppressed)
			{
				gStatuses3[battlerId] |= STATUS3_GASTRO_ACID;
				SuppressBattlerAbility(battlerId);
				gBattlescriptCurrInstr = cmd->nextInstr;
			}
			else
				gBattlescriptCurrInstr = cmd->failPtr;
			
			return;
		}
		case VARIOUS_JUMP_IF_BATTLER_REVIVED:
		{
		    VARIOUS_ARGS(const u8 *ptr);

			if (gBattleStruct->battlers[battlerId].usedReviveItem)
			{
				gBattleStruct->battlers[battlerId].usedReviveItem = FALSE;
				gAbsentBattlerFlags &= ~(gBitTable[gBattleScripting.battler]);
				gBattlescriptCurrInstr = cmd->ptr;
			}
			else
				gBattlescriptCurrInstr = cmd->nextInstr;
			
			return;
		}
		case VARIOUS_JUMP_IF_WEATHER_AFFECTED:
		{
			VARIOUS_ARGS(u16 weatherFlags, const u8 *ptr);

			if (IsBattlerWeatherAffected(battlerId, cmd->weatherFlags))
				gBattlescriptCurrInstr = cmd->ptr;
			else
				gBattlescriptCurrInstr = cmd->nextInstr;
			
			return;
		}
		case VARIOUS_GET_BATTLER_SIDE:
		{
			VARIOUS_ARGS();
			gBattleCommunication[MULTIUSE_STATE] = GetBattlerSide(battlerId);
			break;
		}
		case VARIOUS_JUMP_IF_PARENTAL_BOND_COUNTER:
		{
			VARIOUS_ARGS(u8 counter, const u8 *ptr);

			// Some effects only occours in certain hits of Parental Bond, so a way to check this in scripts are usefull
			if (gSpecialStatuses[battlerId].parentalBondState == cmd->counter && IsBattlerAlive(gBattlerTarget))
				gBattlescriptCurrInstr = cmd->ptr;
			else
				gBattlescriptCurrInstr = cmd->nextInstr;
			
			return;
		}
		case VARIOUS_JUMP_IF_NOT_FIRST_TURN:
		{
			VARIOUS_ARGS(const u8 *ptr);

			if (gDisableStructs[battlerId].isFirstTurn)
				gBattlescriptCurrInstr = cmd->nextInstr;
			else
				gBattlescriptCurrInstr = cmd->ptr;
			
			return;
		}
		case VARIOUS_TRY_CONVERSION_TYPE_CHANGE:
		{
			VARIOUS_ARGS(const u8 *failPtr);

			u8 moveType = gBattleMoves[gBattleMons[battlerId].moves[0]].type; // Always first slot move's type
			
			if (IsBattlerOfType(battlerId, moveType))
				gBattlescriptCurrInstr = cmd->failPtr;
			else
			{
				SetBattlerType(battlerId, moveType);
				gBattlescriptCurrInstr = cmd->nextInstr;
			}
			return;
		}
		case VARIOUS_TRY_SET_DESTINY_BOND:
		{
		    VARIOUS_ARGS(const u8 *failPtr);

			if (gDisableStructs[battlerId].destinyBondCounter) // Fail if was successfully executed on the previous turn
				gBattlescriptCurrInstr = cmd->failPtr;
			else
			{
				gDisableStructs[battlerId].destinyBondCounter = 2;
				gBattleMons[battlerId].status2 |= STATUS2_DESTINY_BOND;
				gBattlescriptCurrInstr = cmd->nextInstr;
			}
			return;
		}
		case VARIOUS_TRY_COPY_ABILITY:
		{
			VARIOUS_ARGS(const u8 *failPtr);

			u16 abilityAttacker = gBattleMons[gBattlerAttacker].ability, abilityTarget = gBattleMons[gBattlerTarget].ability;
			
		    if (abilityAttacker == abilityTarget || gAbilities[abilityAttacker].cantBeSuppressed || gAbilities[abilityTarget].cantBeCopied)
				gBattlescriptCurrInstr = cmd->failPtr;
			else
				gBattlescriptCurrInstr = cmd->nextInstr;
			
			return;
		}
		case VARIOUS_JUMP_IF_CANT_GIVE_NICK:
		{
		    VARIOUS_ARGS(const u8 *ptr);

			if (gSaveBlock2Ptr->optionsSkipPkmnNickname)
				gBattlescriptCurrInstr = cmd->ptr;
			else
				gBattlescriptCurrInstr = cmd->nextInstr;
			
			return;
		}
		case VARIOUS_JUMP_IF_NO_DAMAGE:
		{
		    VARIOUS_ARGS(const u8 *ptr);

			if (gProtectStructs[gBattlerAttacker].physicalDmg || gProtectStructs[gBattlerAttacker].specialDmg)
				gBattlescriptCurrInstr = cmd->nextInstr;
			else
				gBattlescriptCurrInstr = cmd->ptr;
			
			return;
		}
		case VARIOUS_JUMP_IF_ENDEAVOR_FAIL:
		{
		    VARIOUS_ARGS(const u8 *ptr);

			if (gBattleMons[gBattlerAttacker].hp >= gBattleMons[gBattlerTarget].hp)
				gBattlescriptCurrInstr = cmd->ptr;
			else
				gBattlescriptCurrInstr = cmd->nextInstr;
			
			return;
		}
		case VARIOUS_TRY_SWAP_ABILITIES:
		{
		    VARIOUS_ARGS(const u8 *failPtr);

			if (gAbilities[gBattleMons[gBattlerAttacker].ability].cantBeSwapped || gAbilities[gBattleMons[gBattlerTarget].ability].cantBeSwapped)
				gBattlescriptCurrInstr = cmd->failPtr;
			else
				gBattlescriptCurrInstr = cmd->nextInstr;
			
			return;
		}
		case VARIOUS_JUMP_IF_DEFIANT_ACTIVATE:
		{
		    VARIOUS_ARGS(const u8 *ptr);

			if (GetBattlerSide(battlerId) != GetBattlerSide(gSpecialStatuses[battlerId].changedStatsBattlerId) && gBattleStruct->statChange.buff < 0)
			{
				u8 statId = STAT_HP;
				
				switch (GetBattlerAbility(battlerId))
				{
					case ABILITY_DEFIANT:
					    statId = STAT_ATK;
						break;
					case ABILITY_COMPETITIVE:
					    statId = STAT_SPATK;
						break;
				}
				
				if (statId != STAT_HP && CompareStat(battlerId, statId, MAX_STAT_STAGES, CMP_LESS_THAN))
				{
					SetStatChanger(statId, +2);
					gBattlescriptCurrInstr = cmd->ptr;
					return;
				}
			}
			gBattlescriptCurrInstr = cmd->nextInstr;
			return;
		}
		case VARIOUS_JUMP_IF_CONFUSED_AND_STAT_MAXED:
		{
		    VARIOUS_ARGS(u8 statId, const u8 *ptr);

			if ((gBattleMons[battlerId].status2 & STATUS2_CONFUSION) && !CompareStat(battlerId, cmd->statId, MAX_STAT_STAGES, CMP_LESS_THAN))
				gBattlescriptCurrInstr = cmd->ptr;
			else
				gBattlescriptCurrInstr = cmd->nextInstr;
			
			return;
		}
		case VARIOUS_TRY_RATTLED_ON_INTIMIDATE:
		{
		    VARIOUS_ARGS(const u8 *ptr);

			if (gBattleStruct->statChange.result == STAT_CHANGE_WORKED && GetBattlerAbility(battlerId) == ABILITY_RATTLED
			&& CompareStat(battlerId, STAT_SPEED, MAX_STAT_STAGES, CMP_LESS_THAN))
				gBattlescriptCurrInstr = cmd->ptr;
			else
				gBattlescriptCurrInstr = cmd->nextInstr;
			
			return;
		}
		case VARIOUS_TRY_ACTIVATE_SAP_SIPPER:
		{
		    VARIOUS_ARGS(const u8 *ptr);

			if (battlerId != MAX_BATTLERS_COUNT && gBattlerAttacker != battlerId && gBattleStruct->dynamicMoveType == TYPE_GRASS
			&& GetBattlerAbility(battlerId) == ABILITY_SAP_SIPPER && CompareStat(battlerId, STAT_ATK, MAX_STAT_STAGES, CMP_LESS_THAN))
			{
				SetStatChanger(STAT_ATK, +1);
				gBattlescriptCurrInstr = cmd->ptr;
			}
			else
				gBattlescriptCurrInstr = cmd->nextInstr;
			
			return;
		}
		case VARIOUS_TRY_SET_GRAVITY:
		{
		    VARIOUS_ARGS(const u8 *failPtr);

			if (!(gFieldStatus & STATUS_FIELD_GRAVITY))
			{
				gFieldStatus |= STATUS_FIELD_GRAVITY;
				gFieldTimers.gravityTimer = 5;
				gBattlescriptCurrInstr = cmd->nextInstr;
			}
			else
				gBattlescriptCurrInstr = cmd->failPtr;
			
			return;
		}
		case VARIOUS_TRY_BRING_DOWN_IN_AIR:
		{
		    VARIOUS_ARGS(const u8 *failPtr);

			if ((gStatuses3[battlerId] & (STATUS3_ON_AIR | STATUS3_MAGNET_RISE | STATUS3_TELEKINESIS)))
			{
				if ((gStatuses3[battlerId] & STATUS3_ON_AIR) && !(gStatuses3[battlerId] & STATUS3_SKY_DROPPED))
					CancelMultiTurnMoves(battlerId);

				gStatuses3[battlerId] &= ~(STATUS3_ON_AIR | STATUS3_MAGNET_RISE | STATUS3_TELEKINESIS | STATUS3_SKY_DROPPED);
				gBattlescriptCurrInstr = cmd->nextInstr;
			}
			else
				gBattlescriptCurrInstr = cmd->failPtr;
			
			return;
		}
		case VARIOUS_TRY_ACTIVATE_WIND_ABILITIES:
		{
		    VARIOUS_ARGS();

			if (GetBattlerSide(gBattlerAttacker) == GetBattlerSide(battlerId))
			{
				switch (GetBattlerAbility(battlerId))
				{
					case ABILITY_WIND_RIDER:
					    if (CompareStat(battlerId, STAT_ATK, MAX_STAT_STAGES, CMP_LESS_THAN))
						{
							SetStatChanger(STAT_ATK, +1);
							gBattlescriptCurrInstr = cmd->nextInstr;
							BattleScriptCall(BattleScript_SoulHeartActivates);
							return;
						}
						break;
					case ABILITY_WIND_POWER:
					    gBattlescriptCurrInstr = cmd->nextInstr;
						BattleScriptCall(BattleScript_WindPowerActivates);
						return;
				}
			}
			break;
		}
		case VARIOUS_JUMP_IF_EMERGENCY_EXITED:
		{
		    VARIOUS_ARGS(const u8 *ptr);

			if (gSpecialStatuses[battlerId].emergencyExited)
				gBattlescriptCurrInstr = cmd->ptr;
			else
				gBattlescriptCurrInstr = cmd->nextInstr;
			
			return;
		}
		case VARIOUS_TRY_STATUS_TRANSFER:
		{
			VARIOUS_ARGS(const u8 *failPtr);
			
			u8 moveEffect = MOVE_EFFECT_NONE;

			switch (gBattleMons[battlerId].status1.id)
			{
				case STATUS1_SLEEP:
					if (CanBePutToSleep(battlerId, gBattlerTarget, STATUS_CHANGE_FLAG_CHECK_UPROAR) == STATUS_CHANGE_WORKED)
						moveEffect = MOVE_EFFECT_SLEEP;
					break;
				case STATUS1_POISON:
					if (CanBePoisoned(battlerId, gBattlerTarget, 0) == STATUS_CHANGE_WORKED)
						moveEffect = MOVE_EFFECT_POISON;
					break;
				case STATUS1_BURN:
					if (CanBeBurned(battlerId, gBattlerTarget, 0) == STATUS_CHANGE_WORKED)
						moveEffect = MOVE_EFFECT_BURN;
					break;
				case STATUS1_PARALYSIS:
					if (CanBeParalyzed(battlerId, gBattlerTarget, 0) == STATUS_CHANGE_WORKED)
						moveEffect = MOVE_EFFECT_PARALYSIS;
					break;
				case STATUS1_TOXIC_POISON:
					if (CanBePoisoned(battlerId, gBattlerTarget, 0) == STATUS_CHANGE_WORKED)
						moveEffect = MOVE_EFFECT_TOXIC;
					break;
			}
			
			if (moveEffect != MOVE_EFFECT_NONE)
			{
				SetMoveEffect(moveEffect, FALSE, TRUE);
				gBattlescriptCurrInstr = cmd->nextInstr;
			}
			else
				gBattlescriptCurrInstr = cmd->failPtr;

			return;
		}
    }
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk77_setprotectlike(void)
{
	CMD_ARGS();

	u8 i, divisor = 1;
	
    if (!gBattleMoves[gBattleStruct->battlers[gBattlerAttacker].lastResultingMove].flags.protectionMove)
        gDisableStructs[gBattlerAttacker].protectUses = 0;
	
	if (!GET_MOVE_MOVEEFFECT_TABLE(gCurrentMove).affectsUserSide) // effects that protects the user's side increase the fail rate, but can't fail themselves
	{
		for (i = 0; i < gDisableStructs[gBattlerAttacker].protectUses && i <= 6; ++i)
			divisor *= 3;
	}
	
    if ((0xFFFF / divisor) >= Random() && gCurrentTurnActionNumber != (gBattlersCount - 1))
    {
		switch (gBattleMoves[gCurrentMove].effect)
		{
			case EFFECT_PROTECT:
				gProtectStructs[gBattlerAttacker].protected = TRUE;
				gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_USED_PROTECT;
				break;
			case EFFECT_ENDURE:
				gProtectStructs[gBattlerAttacker].endured = TRUE;
				gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_BRACED_ITSELF;
				break;
		}
		++gDisableStructs[gBattlerAttacker].protectUses;
		gBattlescriptCurrInstr = cmd->nextInstr;
    }
    else
    {
        gDisableStructs[gBattlerAttacker].protectUses = 0;
		gBattlescriptCurrInstr = BattleScript_ButItFailed;
    }
}

static void atk78_instanthpdrop(void)
{
	CMD_ARGS();

	if (IsBattlerAlive(gBattlerAttacker))
	{
		gBattleMoveDamage = gBattleMons[gBattlerAttacker].hp;
		BtlController_EmitHealthBarUpdate(gBattlerAttacker, BUFFER_A, INSTANT_HP_BAR_DROP);
		MarkBattlerForControllerExec(gBattlerAttacker);
	}
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk79_setatkhptozero(void)
{
	CMD_ARGS();

    if (!gBattleControllerExecFlags)
    {
		if (IsBattlerAlive(gBattlerAttacker))
		{
			gBattleMons[gBattlerAttacker].hp = 0;
			BtlController_EmitSetMonData(gBattlerAttacker, BUFFER_A, REQUEST_HP_BATTLE, 0, sizeof(gBattleMons[gBattlerAttacker].hp), &gBattleMons[gBattlerAttacker].hp);
			MarkBattlerForControllerExec(gBattlerAttacker);
		}
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atk7A_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk7B_tryhealhalfhealth(void)
{
	CMD_ARGS(u8 battler, const u8 *failPtr);

	u8 battlerId = GetBattlerForBattleScript(cmd->battler);
	
	if (BATTLER_MAX_HP(battlerId))
		gBattlescriptCurrInstr = cmd->failPtr;
	else
	{
		gBattleMoveDamage = gBattleMons[battlerId].maxHP / 2;
		if (gBattleMoveDamage == 0)
			gBattleMoveDamage = 1;
		gBattleMoveDamage *= -1;

		gBattlescriptCurrInstr = cmd->nextInstr;
	}
}

static void atk7C_trymirrormove(void)
{
	CMD_ARGS();

	u8 i, validMovesCount;
    u16 move, newMove, movesArray[MAX_MON_MOVES - 1];

    for (i = 0; i < MAX_MON_MOVES - 1; ++i)
        movesArray[i] = MOVE_NONE;
	
    for (i = 0, validMovesCount = 0; i < gBattlersCount; ++i)
    {
        if (i != gBattlerAttacker)
        {
            move = gBattleStruct->battlers[gBattlerAttacker].lastTakenMoveFrom[i];
			
            if (move && move != MOVE_UNAVAILABLE)
                movesArray[validMovesCount++] = move;
        }
    }
    move = gBattleStruct->battlers[gBattlerAttacker].lastTakenMove;
	
    if (move && move != MOVE_UNAVAILABLE)
		newMove = move;
    else if (validMovesCount)
        newMove = movesArray[RandomMax(validMovesCount)];
    else // No valid moves find
    {
        gBattlescriptCurrInstr = cmd->nextInstr;
		return;
    }
	CallAnotherMove(newMove);
}

static void atk7D_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk7E_trysetreflect(void)
{
	CMD_ARGS(const u8 *failPtr);

	u8 battlerSide = GetBattlerSide(gBattlerAttacker);
	
    if (gSideStatuses[battlerSide] & SIDE_STATUS_REFLECT)
		gBattlescriptCurrInstr = cmd->failPtr;
    else
    {
        gSideStatuses[battlerSide] |= SIDE_STATUS_REFLECT;
        gSideTimers[battlerSide].reflectTimer = 5;
        gSideTimers[battlerSide].reflectBattlerId = gBattlerAttacker;
		PrepareMoveBuffer(gBattleTextBuff1, MOVE_REFLECT);
		PrepareStatBuffer(gBattleTextBuff2, STAT_DEF);
		gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_REFLECT_RAISED;
		gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atk7F_setseeded(void)
{
	CMD_ARGS();

    if ((gMoveResultFlags & MOVE_RESULT_NO_EFFECT) || (gStatuses3[gBattlerTarget] & STATUS3_LEECHSEED))
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_EVADED_ATTACK;
    }
    else if (IsBattlerOfType(gBattlerTarget, TYPE_GRASS))
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_LEECH_SEED_DOEST_AFFECT;
    }
    else
    {
        gStatuses3[gBattlerTarget] |= STATUS3_LEECHSEED;
		gDisableStructs[gBattlerTarget].leechSeedBattler = gBattlerAttacker;
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SEEDED;
    }
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk80_manipulatedamage(void)
{
	CMD_ARGS(u8 caseId);

    switch (cmd->caseId)
    {
		case ATK80_DMG_CHANGE_SIGN:
            gBattleMoveDamage *= -1;
            break;
		case ATK80_DMG_HALF_USER_HP:
			gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 2;
			if (gBattleMoveDamage == 0)
				gBattleMoveDamage = 1;
			break;
		case ATK80_DMG_1_8_TARGET_MAX_HP:
			gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP / 8;
			if (gBattleMoveDamage == 0)
				gBattleMoveDamage = 1;
			break;
		case ATK48_DMG_HALF_TARGET_HP:
		    gBattleMoveDamage = gBattleMons[gBattlerTarget].hp / 2;
			if (gBattleMoveDamage == 0)
				gBattleMoveDamage = 1;
			break;
		case ATK48_DMG_ATK_LEVEL:
		    gBattleMoveDamage = gBattleMons[gBattlerAttacker].level;
			break;
		case ATK80_DMG_DRAINED:
			gBattleMoveDamage = (gHpDealt * gBattleMoves[gCurrentMove].argument.absorb.percentage / 100);
			
			if (gBattleMoveDamage == 0)
				gBattleMoveDamage = 1;
			// Fallthrough
		case ATK48_DMG_BIG_ROOT:
			gBattleMoveDamage = GetDrainedBigRootHp(gBattlerAttacker, gBattleMoveDamage);
			break;
		case ATK80_DMG_HEALTH_DIFFERENCE:
		    gBattleMoveDamage = gBattleMons[gBattlerTarget].hp - gBattleMons[gBattlerAttacker].hp;
			break;
    }
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk81_trysetrest(void)
{
	CMD_ARGS(const u8 *failPtr);

    if (BATTLER_MAX_HP(gBattlerTarget))
        gBattlescriptCurrInstr = cmd->failPtr;
    else
    {
		gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP * -1;
		
        if (gBattleMons[gBattlerTarget].status1.id && gBattleMons[gBattlerTarget].status1.id != STATUS1_SLEEP)
            gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SLEPT_HEALTHY;
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_WENT_TO_SLEEP;
		
		gBattleMons[gBattlerTarget].status1.id = STATUS1_SLEEP;
        gBattleMons[gBattlerTarget].status1.counter = 3;

        BtlController_EmitSetMonData(gBattlerTarget, BUFFER_A, REQUEST_STATUS_BATTLE, 0, sizeof(gBattleMons[gBattlerTarget].status1), &gBattleMons[gBattlerTarget].status1);
        MarkBattlerForControllerExec(gBattlerTarget);

        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atk82_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk83_handletrainerslidecase(void)
{
	CMD_ARGS(u8 battlerOrPosition, u8 caseId);

	u8 i, battlerId;
	
	switch (cmd->caseId)
	{
		case ATK83_TRAINER_SLIDE_CASE_SAVE_SPRITES:
			battlerId = GetBattlerForBattleScript(cmd->battlerOrPosition);
			
		    // Save sprite Id's, because trainer slide in will overwrite gBattlerSpriteIds variable.
			for (i = 0; i < NUM_BATTLERS_PER_SIDE; i++, battlerId = BATTLE_PARTNER(battlerId))
				gBattleStruct->battlers[battlerId].savedSpriteId = gBattlerSpriteIds[battlerId];
		    break;
		case ATK83_TRAINER_SLIDE_CASE_SLIDE_IN:
		    battlerId = GetBattlerAtPosition(cmd->battlerOrPosition);
			BtlController_EmitTrainerSlide(battlerId, BUFFER_A);
			MarkBattlerForControllerExec(battlerId);
			break;
		case ATK83_TRAINER_SLIDE_CASE_PRINT_STRING:
			battlerId = GetBattlerForBattleScript(cmd->battlerOrPosition);
		    PrepareStringBattle(STRINGID_TRAINERSLIDE, battlerId);
			gBattleCommunication[MSG_DISPLAY] = TRUE;
		    break;
		case ATK83_TRAINER_SLIDE_CASE_SLIDE_OUT:
		    battlerId = GetBattlerAtPosition(cmd->battlerOrPosition);
			BtlController_EmitTrainerSlideBack(battlerId, BUFFER_A);
			MarkBattlerForControllerExec(battlerId);
			break;
		case ATK83_TRAINER_SLIDE_CASE_RESTORE_SPRITES:
			battlerId = GetBattlerForBattleScript(cmd->battlerOrPosition);
		
			// Restore sprite ids
			for (i = 0; i < NUM_BATTLERS_PER_SIDE; i++, battlerId = BATTLE_PARTNER(battlerId))
			{
				gBattlerSpriteIds[battlerId] = gBattleStruct->battlers[battlerId].savedSpriteId;
				
				if (IsBattlerAlive(battlerId))
					BattleLoadMonSpriteGfx(battlerId);
			}
		    break;
	}
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk84_trysetpoison(void)
{
	CMD_ARGS(u8 battler);

	u8 battlerId = GetBattlerForBattleScript(cmd->battler);
	
	switch (CanBePoisoned(gBattlerAttacker, battlerId, 0))
	{
		case STATUS_CHANGE_WORKED:
			if (SubsBlockMove(gBattlerAttacker, battlerId, gCurrentMove))
				gBattlescriptCurrInstr = BattleScript_ButItFailed;
			else
				gBattlescriptCurrInstr = cmd->nextInstr;
			break;
		case STATUS_CHANGE_FAIL_SPECIFIC_STATUSED:
			gBattlescriptCurrInstr = BattleScript_AlreadyPoisoned;
			break;
		case STATUS_CHANGE_FAIL_ALREADY_STATUSED:
			gBattlescriptCurrInstr = BattleScript_ButItFailed;
			break;
		case STATUS_CHANGE_FAIL_TYPE_NOT_AFFECTED:
			gBattlescriptCurrInstr = BattleScript_NotAffected;
			break;
		case STATUS_CHANGE_FAIL_SAFEGUARD_PROTECTED:
			gBattlescriptCurrInstr = BattleScript_SafeguardProtected;
			break;
		case STATUS_CHANGE_FAIL_ABILITY_PREVENTED:
			gBattlescriptCurrInstr = BattleScript_ImmunityProtected;
			break;
		case STATUS_CHANGE_FAIL_FLOWER_VEIL_ON_SIDE:
		    gEffectBattler = battlerId;
			gBattleScripting.battler = gBattleScripting.savedBattler;
			gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
			break;
		case STATUS_CHANGE_FAIL_PASTEL_VEIL_ON_SIDE:
			gBattleScripting.battler = gBattleScripting.savedBattler;
			gBattlescriptCurrInstr = BattleScript_TeamProtectedByPastelVeil;
			break;
	}
}

static void atk85_stockpile(void)
{
	CMD_ARGS(u8 caseId, const u8 *ptr);

	switch (cmd->caseId)
	{
		case 0: // Stockpiled
		    if (gDisableStructs[gBattlerAttacker].stockpileCounter == 3)
            {
                gMoveResultFlags |= MOVE_RESULT_MISSED;
                gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_ATK_CANT_STOCKPILE;
            }
            else
            {
                PrepareByteNumberBuffer(gBattleTextBuff1, 1, ++gDisableStructs[gBattlerAttacker].stockpileCounter);
                gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_ATK_STOCKPILED;
	        	
	        	if (CompareStat(gBattlerAttacker, STAT_DEF, MAX_STAT_STAGES, CMP_LESS_THAN))
	        		++gDisableStructs[gBattlerAttacker].stockpiledDef;
	        	
	        	if (CompareStat(gBattlerAttacker, STAT_SPDEF, MAX_STAT_STAGES, CMP_LESS_THAN))
	        		++gDisableStructs[gBattlerAttacker].stockpiledSpDef;
            }
			break;
		case 1: // Reset def Stockpile
		    if (gDisableStructs[gEffectBattler].stockpiledDef)
			{
				SetStatChanger(STAT_DEF, -gDisableStructs[gEffectBattler].stockpiledDef);
				gDisableStructs[gEffectBattler].stockpiledDef = 0;
			}
			else
			{
				gBattlescriptCurrInstr = cmd->ptr;
				return;
			}
			break;
		case 2: // Reset Sp Def Stockpile
		    if (gDisableStructs[gEffectBattler].stockpiledSpDef)
			{
				SetStatChanger(STAT_SPDEF, -gDisableStructs[gEffectBattler].stockpiledSpDef);
				gDisableStructs[gEffectBattler].stockpiledSpDef = 0;
			}
			else
			{
				gBattlescriptCurrInstr = cmd->ptr;
				return;
			}
			break;
	}
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk86_stockpiletobasedamage(void)
{
	CMD_ARGS(const u8 *failPtr);

    if (!gDisableStructs[gBattlerAttacker].stockpileCounter)
	{
		gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattlescriptCurrInstr = cmd->failPtr;
	}
    else
    {
        if (gBattleCommunication[MISS_TYPE] != B_MSG_PROTECTED)
            gBattleScripting.animTurn = gDisableStructs[gBattlerAttacker].stockpileCounter;
		
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atk87_stockpiletohpheal(void)
{
	CMD_ARGS(const u8 *failPtr);

    const u8 *jumpPtr = cmd->failPtr;

    if (!gDisableStructs[gBattlerAttacker].stockpileCounter)
    {
        gBattlescriptCurrInstr = jumpPtr;
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_FAILED_TO_SWALLOW;
    }
    else if (BATTLER_MAX_HP(gBattlerAttacker))
    {
        gBattlescriptCurrInstr = jumpPtr;
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SWALLOW_FULL_HP;
    }
    else
    {
		gBattleScripting.animTurn = gDisableStructs[gBattlerAttacker].stockpileCounter;
        gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / (1 << (3 - gDisableStructs[gBattlerAttacker].stockpileCounter));
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        gBattleMoveDamage *= -1;
		gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atk88_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk89_statbuffchange(void)
{
	CMD_ARGS(u8 flags, const u8 *failPtr);

	u8 flags = cmd->flags;
	
	if (!ChangeStatBuffs(flags, FALSE))
	{
		if (flags & STAT_CHANGE_FLAG_UPDATE_RESULT)
			gMoveResultFlags |= MOVE_RESULT_MISSED;
		
		gBattleStruct->statChange.str = cmd->failPtr; // Fail jump if stat failed to apply
	}
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk8A_displaystatchangestring(void)
{
	CMD_ARGS(u16 stringId);

	const u8 *script;
	
	PrepareStatBuffer(gBattleTextBuff1, gBattleStruct->statChange.statId);
	gBattleTextBuff2[0] = EOS;
	
    switch (gBattleStruct->statChange.result)
	{
		case STAT_CHANGE_WORKED:
		{
		    u16 stringId = cmd->stringId;
			
		    if (!stringId)
			{
				s8 buff = gBattleStruct->statChange.buff;
			    
				if (gBattleStruct->statChange.maxOut)
				{
					StringAppend(gBattleTextBuff2, buff < 0 ? COMPOUND_STRING("minimized") : COMPOUND_STRING("maxed"));
					stringId = STRINGID_EFFMAXEDSTAT;
				}
			    else
				{
					if (buff == +2 || buff == -2)
						StringAppend(gBattleTextBuff2, COMPOUND_STRING(" sharply"));
					else if (buff >= +3 || buff <= -3)
						StringAppend(gBattleTextBuff2, COMPOUND_STRING(" severely"));
					
					StringAppend(gBattleTextBuff2, buff < 0 ? COMPOUND_STRING(" fell") : COMPOUND_STRING(" rose"));
					
					stringId = STRINGID_EFFSTATCHANGED;
				}
			}
			gBattlescriptCurrInstr = cmd->nextInstr;
			PrepareStringBattle(stringId, gBattlerAttacker);
			gBattleCommunication[MSG_DISPLAY] = TRUE;
			BattleScriptCall(BattleScript_StatChangeString);
			return;
		}
		case STAT_CHANGE_FAIL_WONT_CHANGE:
		    StringAppend(gBattleTextBuff2, gBattleStruct->statChange.buff < 0 ? COMPOUND_STRING("won't go lower") : COMPOUND_STRING("won't go higher"));
			gBattlescriptCurrInstr = cmd->nextInstr;
			BattleScriptCall(BattleScript_StatCantChangeString);
			return;
		case STAT_CHANGE_FAIL_ABILITY_PREVENT_SPECIFIC_STAT:
		    gBattlescriptCurrInstr = cmd->nextInstr;
			BattleScriptCall(BattleScript_AbilityNoSpecificStatLoss);
			return;
		case STAT_CHANGE_FAIL_PROTECTED:
			BattleScriptPush(BattleScript_MoveEnd);
			gBattlescriptCurrInstr = BattleScript_Protected;
			return;
		case STAT_CHANGE_FAIL_ABILITY_PREVENTED:
		    script = BattleScript_AbilityNoStatLoss;
		    break;
		case STAT_CHANGE_FAIL_FLOWER_VEIL:
		    gBattleScripting.battler = gBattleScripting.savedBattler;
			script = BattleScript_TeamProtectedByFlowerVeilStatChange;
			break;
		case STAT_CHANGE_FAIL_MIST:
		    script = BattleScript_MistProtected;
		    break;
	}

	if (gBattleStruct->statChange.str != NULL)
	{
		BattleScriptPush(gBattleStruct->statChange.str);
		gBattlescriptCurrInstr = script;
	}
	else
	{
		gBattlescriptCurrInstr = cmd->nextInstr;
		BattleScriptCall(script);
	}
}

static void atk8B_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk8C_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk8D_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk8E_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void ForceSwitchOut(void)
{
	gBattleStruct->battlers[gBattlerTarget].partyIndex = gBattlerPartyIndexes[gBattlerTarget];
	gBattlescriptCurrInstr = BattleScript_SuccessForceOut;
}

static void atk8F_forcerandomswitch(void)
{
	CMD_ARGS(const u8 *failPtr);
	
	u8 atkSide = GetBattlerSide(gBattlerAttacker), defSide = GetBattlerSide(gBattlerTarget);
	
	if (IsBattlerBeingCommanded(gBattlerTarget) || ((gBattleTypeFlags & BATTLE_TYPE_SOS) && atkSide == B_SIDE_PLAYER))
		gBattlescriptCurrInstr = cmd->failPtr;
	else if (IS_WHOLE_SIDE_ALIVE(gBattlerAttacker) && IS_WHOLE_SIDE_ALIVE(gBattlerTarget) && ((atkSide == B_SIDE_PLAYER && defSide == B_SIDE_OPPONENT)
	|| (atkSide == B_SIDE_OPPONENT && defSide == B_SIDE_OPPONENT)))
		gBattlescriptCurrInstr = cmd->failPtr;
	else
	{
		// Swapping pokemon happens in:
		// Trainer battles
		// One of two opposing Pokémon uses it against one of the two alive player mons
		// One of the player's Pokémon uses it against its partner
		if ((gBattleTypeFlags & BATTLE_TYPE_TRAINER) || (atkSide == B_SIDE_OPPONENT && defSide == B_SIDE_PLAYER && IsDoubleBattleOnSide(atkSide)
		&& IS_WHOLE_SIDE_ALIVE(gBattlerTarget)) || (atkSide == B_SIDE_PLAYER && defSide == B_SIDE_PLAYER && IsDoubleBattleOnSide(defSide)))
		{
			u8 validMons[PARTY_SIZE], validMonsCount = CountUsablePartyMons(gBattlerTarget, validMons);

			if (validMonsCount == 0)
				gBattlescriptCurrInstr = cmd->failPtr;
			else
			{
				u8 monToSwitchInto = validMons[RandomMax(validMonsCount)]; // Choose one mon at random
				
				ForceSwitchOut();
				gBattleStruct->battlers[gBattlerTarget].monToSwitchIntoId = monToSwitchInto;
				
				if (!IsMultiBattle())
					UpdatePartyOwnerOnSwitch_NonMulti(gBattlerTarget);
				
				SwitchPartyOrderLinkMulti(gBattlerTarget, monToSwitchInto, 0);
				SwitchPartyOrderLinkMulti(BATTLE_PARTNER(gBattlerTarget), monToSwitchInto, 1);
			}
		}
		else
		{
			if (gBattleMons[gBattlerAttacker].level < gBattleMons[gBattlerTarget].level)
				gBattlescriptCurrInstr = cmd->failPtr; // Fail if attacker level < Defender level
			else
				ForceSwitchOut(); // Success force switch
		}
	}
}

static void atk90_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk91_givepaydaymoney(void)
{
	CMD_ARGS();

	gBattlescriptCurrInstr = cmd->nextInstr;

    if (!(gBattleTypeFlags & BATTLE_TYPE_LINK) && gPaydayMoney)
    {
        u32 bonusMoney = gPaydayMoney;
		
		if (gBattleStruct->moneyMultiplier)
			bonusMoney *= 2;

        AddMoney(&gSaveBlock1Ptr->money, bonusMoney);
        PrepareHWordNumberBuffer(gBattleTextBuff1, 5, bonusMoney);
		BattleScriptCall(BattleScript_PrintPayDayMoneyString);
    }
}

static void atk92_trysetlightscreen(void)
{
	CMD_ARGS(const u8 *failPtr);

	u8 battlerSide = GetBattlerSide(gBattlerAttacker);
	
    if (gSideStatuses[battlerSide] & SIDE_STATUS_LIGHTSCREEN)
		gBattlescriptCurrInstr = cmd->failPtr;
    else
    {
        gSideStatuses[battlerSide] |= SIDE_STATUS_LIGHTSCREEN;
        gSideTimers[battlerSide].lightscreenTimer = 5;
        gSideTimers[battlerSide].lightscreenBattlerId = gBattlerAttacker;
		PrepareMoveBuffer(gBattleTextBuff1, MOVE_LIGHT_SCREEN);
		PrepareStatBuffer(gBattleTextBuff2, STAT_SPDEF);
		gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_REFLECT_RAISED;
		gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

u16 GetOHKOChance(u8 attacker, u8 target, u16 move)
{
	u8 acc = gBattleMoves[move].accuracy;
	
	if (move == MOVE_SHEER_COLD && !IsBattlerOfType(attacker, TYPE_ICE))
		acc = 20;
	
	return acc + (gBattleMons[attacker].level - gBattleMons[target].level);
}

bool8 KanOHKOBattler(u8 attacker, u8 target, u16 move, bool8 checkKOAcc)
{
	// Ice type mons are'nt affected by Sheer Cold
	if (move == MOVE_SHEER_COLD && IsBattlerOfType(target, TYPE_ICE))
		return FALSE;
	
	// Target is with always hit flag set
	if ((gStatuses3[attacker] & STATUS3_ALWAYS_HITS) && gDisableStructs[attacker].battlerWithSureHit == target && gBattleMons[attacker].level >= gBattleMons[target].level)
		return TRUE;
	
	if (!checkKOAcc || (RandomMax(100) + 1 < GetOHKOChance(attacker, target, move) && gBattleMons[attacker].level >= gBattleMons[target].level))
		return TRUE;
	
	return FALSE;
}

static void atk93_tryKO(void)
{
	CMD_ARGS(const u8 *failPtr);

    if (GetBattlerAbility(gBattlerTarget) == ABILITY_STURDY)
    {
        gMoveResultFlags |= MOVE_RESULT_FOE_STURDIED;
        gBattlescriptCurrInstr = BattleScript_SturdyPreventsOHKO;
    }
    else
    {
        if (KanOHKOBattler(gBattlerAttacker, gBattlerTarget, gCurrentMove, TRUE))
        {
			if (SetHitDamageResult(gBattlerTarget, gCurrentMove, FALSE))
				gBattleMoveDamage = gBattleMons[gBattlerTarget].hp - 1;
			else
			{
				gBattleMoveDamage = gBattleMons[gBattlerTarget].hp;
                gMoveResultFlags |= MOVE_RESULT_ONE_HIT_KO;
			}
			TrySetDestinyBondToHappen();
			
			gBattlescriptCurrInstr = cmd->failPtr;
        }
        else
        {
            gMoveResultFlags |= MOVE_RESULT_MISSED;
		
            if (gBattleMons[gBattlerAttacker].level >= gBattleMons[gBattlerTarget].level)
                gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_KO_MISSED;
            else
                gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_UNAFFECTED;
			
            gBattlescriptCurrInstr = cmd->nextInstr;
        }
    }
}

static void atk94_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk95_setweather(void)
{
	CMD_ARGS();

    if (!TryChangeBattleWeather(gBattlerAttacker, gBattleMoves[gCurrentMove].argument.setWeather.weatherId))
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_WEATHER_MOVE_FAIL;
    }
    else
		gBattleCommunication[MULTISTRING_CHOOSER] = gBattleMoves[gCurrentMove].argument.setWeather.stringId;

    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk96_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk97_tryinfatuating(void)
{
	CMD_ARGS(u8 battler);

	u8 battler = GetBattlerForBattleScript(cmd->battler);
	
	switch (CanBeInfatuatedBy(battler, gBattlerAttacker))
	{
		case STATUS_CHANGE_WORKED:
		    gBattleMons[battler].status2 |= STATUS2_INFATUATION;
			gDisableStructs[battler].infatuatedWith = gBattlerAttacker;
		    gBattlescriptCurrInstr = cmd->nextInstr;
			break;
		case STATUS_CHANGE_FAIL_ABILITY_PREVENTED:
		    gBattlescriptCurrInstr = BattleScript_ObliviousPrevents;
			break;
		case STATUS_CHANGE_FAIL_AROMA_VEIL_ON_SIDE:
		    gBattleScripting.battler = gBattleScripting.savedBattler;
			gBattlescriptCurrInstr = BattleScript_AromaVeilProtects;
			break;
		case STATUS_CHANGE_FAIL_ALREADY_STATUSED:
		case STATUS_CHANGE_FAIL_TYPE_NOT_AFFECTED: // gender fail
		    gBattlescriptCurrInstr = BattleScript_ButItFailed;
			break;
	}
}

static void atk98_updatestatusicon(void)
{
	CMD_ARGS(u8 battler);
	
	u8 battlerId;
	
    if (!gBattleControllerExecFlags)
    {
		switch (cmd->battler)
		{
			case BS_PLAYER2:
				for (battlerId = gBattleControllerExecFlags; battlerId < gBattlersCount; ++battlerId)
				{
					if (IsBattlerAlive(battlerId))
					{
						BtlController_EmitStatusIconUpdate(battlerId, BUFFER_A, gBattleMons[battlerId].status1.id, gBattleMons[battlerId].status2);
						MarkBattlerForControllerExec(battlerId);
					}
				}
				break;
			case BS_ATTACKER_WITH_PARTNER:
				if (!(gAbsentBattlerFlags & gBitTable[gBattlerAttacker]))
				{
					BtlController_EmitStatusIconUpdate(gBattlerAttacker, BUFFER_A, gBattleMons[gBattlerAttacker].status1.id, gBattleMons[gBattlerAttacker].status2);
					MarkBattlerForControllerExec(gBattlerAttacker);
				}

				if (IsDoubleBattleForBattler(gBattlerAttacker))
				{
					battlerId = BATTLE_PARTNER(gBattlerAttacker);
					
					if (!(gAbsentBattlerFlags & gBitTable[battlerId]))
					{
						BtlController_EmitStatusIconUpdate(battlerId, BUFFER_A, gBattleMons[battlerId].status1.id, gBattleMons[battlerId].status2);
						MarkBattlerForControllerExec(battlerId);
					}
				}
				break;
			default:
				battlerId = GetBattlerForBattleScript(cmd->battler);
				BtlController_EmitStatusIconUpdate(battlerId, BUFFER_A, gBattleMons[battlerId].status1.id, gBattleMons[battlerId].status2);
				MarkBattlerForControllerExec(battlerId);
				break;
		}
		gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atk99_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk9A_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk9B_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk9C_setsubstitute(void)
{
	CMD_ARGS();

	if (gBattleMons[gBattlerAttacker].status2 & STATUS2_SUBSTITUTE)
		gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_HAS_SUBSTITUTE;
	else
	{
		u32 hp = gBattleMons[gBattlerAttacker].maxHP / 4;
		if (hp == 0)
			hp = 1;
		
		if (gBattleMons[gBattlerAttacker].hp <= hp)
			gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_TOO_WEAK_TO_SUBSTITUTE;
		else
		{
			gBattleMons[gBattlerAttacker].status2 |= STATUS2_SUBSTITUTE;
			gDisableStructs[gBattlerAttacker].wrapTurns = 0;
			gDisableStructs[gBattlerAttacker].substituteHP = gBattleMoveDamage = hp;
			gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_MADE_SUBSTITUTE;
		}
	}
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk9D_mimicattackcopy(void)
{
	CMD_ARGS(const u8 *failPtr);
	
	u16 lastMove = gBattleStruct->battlers[gBattlerTarget].lastMove;
	
    gChosenMove = MOVE_UNAVAILABLE;
	
    if (!lastMove || lastMove == MOVE_UNAVAILABLE || gBattleMoves[lastMove].flags.forbiddenMimic || (gBattleMons[gBattlerAttacker].status2 & STATUS2_TRANSFORMED))
        gBattlescriptCurrInstr = cmd->failPtr;
    else
    {
        if (FindMoveSlotInBattlerMoveset(gBattlerAttacker, lastMove) == MAX_MON_MOVES)
        {
            gBattleMons[gBattlerAttacker].moves[gCurrMovePos] = lastMove;
			gBattleMons[gBattlerAttacker].pp[gCurrMovePos] = gBattleMoves[lastMove].pp;
            PrepareMoveBuffer(gBattleTextBuff1, lastMove);
            gDisableStructs[gBattlerAttacker].mimickedMoves |= gBitTable[gCurrMovePos];
            gBattlescriptCurrInstr = cmd->nextInstr;
        }
        else
            gBattlescriptCurrInstr = cmd->failPtr;
    }
}

static void atk9E_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atk9F_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkA0_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static bool8 TryCounterAttack(u8 battler, u32 dmg, u8 multiplier)
{
	u8 sideTarget = GetBattlerSide(battler);
	
	if (dmg && GetBattlerSide(gBattlerAttacker) != sideTarget && IsBattlerAlive(battler))
	{
		gBattleMoveDamage = dmg * multiplier;
		
		if (IsBattlerAffectedByFollowMe(gBattlerAttacker, sideTarget, gCurrentMove))
			gBattlerTarget = gSideTimers[sideTarget].followmeTarget;
		else
			gBattlerTarget = battler;
		
		return TRUE;
	}
	return FALSE;
}

static void atkA1_trycounterattack(void)
{
	CMD_ARGS(const u8 *failPtr);

	switch (gBattleMoves[gCurrentMove].argument.counterAttack.split)
	{
		case SPLIT_PHYSICAL:
		    if (TryCounterAttack(gProtectStructs[gBattlerAttacker].physicalBattlerId, gProtectStructs[gBattlerAttacker].physicalDmg, 2))
				gBattlescriptCurrInstr = cmd->nextInstr;
			else
				gBattlescriptCurrInstr = cmd->failPtr;
			break;
		case SPLIT_SPECIAL:
		    if (TryCounterAttack(gProtectStructs[gBattlerAttacker].specialBattlerId, gProtectStructs[gBattlerAttacker].specialDmg, 2))
				gBattlescriptCurrInstr = cmd->nextInstr;
			else
				gBattlescriptCurrInstr = cmd->failPtr;
			break;
		case SPLIT_STATUS: // Both damages
		    if (TryCounterAttack(gProtectStructs[gBattlerAttacker].physicalBattlerId, gProtectStructs[gBattlerAttacker].physicalDmg, 15)
			|| TryCounterAttack(gProtectStructs[gBattlerAttacker].specialBattlerId, gProtectStructs[gBattlerAttacker].specialDmg, 15))
			{
				gBattleMoveDamage /= 10;
				gBattlescriptCurrInstr = cmd->nextInstr;
			}
			else
				gBattlescriptCurrInstr = cmd->failPtr;
			break;
	}
	
}

static void atkA2_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkA3_disablelastusedattack(void)
{
	CMD_ARGS(u8 battler, const u8 *failPtr);

    u8 battlerId = GetBattlerForBattleScript(cmd->battler), movePos = FindMoveSlotInBattlerMoveset(battlerId, gBattleStruct->battlers[battlerId].lastMove);

	if (movePos != MAX_MON_MOVES && TryDisableMove(battlerId, movePos, gBattleMons[battlerId].moves[movePos]))
    {
        PrepareMoveBuffer(gBattleTextBuff1, gBattleMons[battlerId].moves[movePos]);
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
    else
        gBattlescriptCurrInstr = cmd->failPtr;
}

static void atkA4_trysetencore(void)
{
	CMD_ARGS(const u8 *failPtr);

    u8 movePos;
	u16 lastMove = gBattleStruct->battlers[gBattlerTarget].lastMove;

    if (lastMove == MOVE_STRUGGLE || lastMove == MOVE_ENCORE || lastMove == MOVE_MIRROR_MOVE || lastMove == MOVE_TRANSFORM || lastMove == MOVE_MIMIC
	|| lastMove == MOVE_SKETCH || lastMove == MOVE_DYNAMAX_CANNON || GET_MOVE_MOVEEFFECT_TABLE(lastMove).callOtherMove)
        movePos = MAX_MON_MOVES;
	else
		movePos = FindMoveSlotInBattlerMoveset(gBattlerTarget, lastMove);
	
    if (!gDisableStructs[gBattlerTarget].encoredMove && movePos != MAX_MON_MOVES && gBattleMons[gBattlerTarget].pp[movePos])
    {
        gDisableStructs[gBattlerTarget].encoredMove = gBattleMons[gBattlerTarget].moves[movePos];
        gDisableStructs[gBattlerTarget].encoredMovePos = movePos;
        gDisableStructs[gBattlerTarget].encoreTimer = 3;
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
    else
        gBattlescriptCurrInstr = cmd->failPtr;
}

static void atkA5_painsplitdmgcalc(void)
{
	CMD_ARGS(const u8 *failPtr);

    if (!SubsBlockMove(gBattlerAttacker, gBattlerTarget, gCurrentMove))
    {
        s32 hpDiff = (gBattleMons[gBattlerAttacker].hp + gBattleMons[gBattlerTarget].hp) / 2;
		s32 painSplitHp = gBattleMons[gBattlerTarget].hp - hpDiff;
		u8 *storeLoc = (void *)(&gBattleScripting.painSplitHp);

        storeLoc[0] = (painSplitHp);
        storeLoc[1] = (painSplitHp & 0x0000FF00) >> 8;
        storeLoc[2] = (painSplitHp & 0x00FF0000) >> 16;
        storeLoc[3] = (painSplitHp & 0xFF000000) >> 24;
        gBattleMoveDamage = gBattleMons[gBattlerAttacker].hp - hpDiff;
        gSpecialStatuses[gBattlerTarget].dmg = 0xFFFF;
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
    else
        gBattlescriptCurrInstr = cmd->failPtr;
}

static void atkA6_settypetorandomresistance(void) // conversion 2
{
	CMD_ARGS(const u8 *failPtr);

	u8 i, lastUsedMoveType;
    u32 resistTypes;
	u16 lastMove = gBattleStruct->battlers[gBattlerTarget].lastMove;
	
	if (!lastMove || lastMove == MOVE_UNAVAILABLE || lastMove == MOVE_STRUGGLE)
		gBattlescriptCurrInstr = cmd->failPtr;
	else
	{
		lastUsedMoveType = gBattleStruct->battlers[gBattlerTarget].lastUsedMoveType;
		
		if (lastUsedMoveType != TYPE_MYSTERY && lastUsedMoveType != TYPE_STELLAR)
		{
		    for (i = 0, resistTypes = 0; i < NUMBER_OF_MON_TYPES - 1; i++)
		    {
		    	switch (GetTypeModifier(lastUsedMoveType, i))
		    	{
		    		case TYPE_MUL_NO_EFFECT:
		    		case TYPE_MUL_NOT_EFFECTIVE:
		    		    resistTypes |= gBitTable[i];
		    		    break;
		    	}
		    }
		    
		    while (resistTypes != 0)
		    {
		    	i = RandomMax(NUMBER_OF_MON_TYPES - 1);
		    	
		    	if (resistTypes & gBitTable[i])
		    	{
		    		if (IsBattlerOfType(gBattlerAttacker, i))
		    			resistTypes &= ~(gBitTable[i]);
		    		else
		    		{
		    			SetBattlerType(gBattlerAttacker, i);
		    			gBattlescriptCurrInstr = cmd->nextInstr;
		    			return;
		    		}
		    	}
		    }
		}
        gBattlescriptCurrInstr = cmd->failPtr;
	}
}

static void atkA7_trysetalwayshitflag(void)
{
	CMD_ARGS(const u8 *failPtr);

	if ((gStatuses3[gBattlerAttacker] & STATUS3_ALWAYS_HITS) && gDisableStructs[gBattlerAttacker].battlerWithSureHit == gBattlerTarget)
		gBattlescriptCurrInstr = cmd->failPtr;
	else
	{
		gStatuses3[gBattlerAttacker] &= ~(STATUS3_ALWAYS_HITS);
		gStatuses3[gBattlerAttacker] |= STATUS3_ALWAYS_HITS_TURN(2);
		gDisableStructs[gBattlerAttacker].battlerWithSureHit = gBattlerTarget;
		gBattlescriptCurrInstr = cmd->nextInstr;
	}
}

static void atkA8_copymovepermanently(void) // sketch
{
	CMD_ARGS(const u8 *failPtr);
	
	u16 lastPrintedMove = gBattleStruct->battlers[gBattlerTarget].lastPrintedMove;
	
    gChosenMove = MOVE_UNAVAILABLE;
	
    if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_TRANSFORMED) && lastPrintedMove && lastPrintedMove != MOVE_UNAVAILABLE && lastPrintedMove != MOVE_STRUGGLE
	&& lastPrintedMove != MOVE_CHATTER && lastPrintedMove != gCurrentMove)
    {
        if (FindMoveSlotInBattlerMoveset(gBattlerAttacker, lastPrintedMove) != MAX_MON_MOVES)
            gBattlescriptCurrInstr = cmd->failPtr;
        else // sketch worked
        {
			u8 i;
            struct MovePpInfo movePpData;

            gBattleMons[gBattlerAttacker].moves[gCurrMovePos] = lastPrintedMove;
            gBattleMons[gBattlerAttacker].pp[gCurrMovePos] = gBattleMoves[lastPrintedMove].pp;
		
            for (i = 0; i < MAX_MON_MOVES; ++i)
            {
                movePpData.moves[i] = gBattleMons[gBattlerAttacker].moves[i];
                movePpData.pp[i] = gBattleMons[gBattlerAttacker].pp[i];
            }
            movePpData.ppBonuses = gBattleMons[gBattlerAttacker].ppBonuses;
            BtlController_EmitSetMonData(gBattlerAttacker, BUFFER_A, REQUEST_MOVES_PP_BATTLE, 0, sizeof(struct MovePpInfo), &movePpData);
            MarkBattlerForControllerExec(gBattlerAttacker);
            PrepareMoveBuffer(gBattleTextBuff1, lastPrintedMove);
            gBattlescriptCurrInstr = cmd->nextInstr;
        }
    }
    else
        gBattlescriptCurrInstr = cmd->failPtr;
}

static void atkA9_trychoosesleeptalkmove(void)
{
	CMD_ARGS(const u8 *failPtr);

    u8 i, movePosition, unusableMovesBits = 0;

    for (i = 0; i < MAX_MON_MOVES; ++i)
    {
        if (!gBattleMons[gBattlerAttacker].moves[i] || gBattleMoves[gBattleMons[gBattlerAttacker].moves[i]].flags.forbiddenSleepTalk
		|| GET_MOVE_MOVEEFFECT_TABLE(gBattleMons[gBattlerAttacker].moves[i]).twoTurnsEffect)
            unusableMovesBits |= gBitTable[i];
    }
    unusableMovesBits |= CheckMoveLimitations(gBattlerAttacker, (MOVE_LIMITATION_IGNORE_NO_PP | MOVE_LIMITATION_IGNORE_IMPRISON));
	
    if (unusableMovesBits == MOVE_LIMITATION_ALL_MOVES_MASK) // all 4 moves cannot be chosen
	    gBattlescriptCurrInstr = cmd->failPtr;
    else // at least one move can be chosen
    {
        do
            movePosition = RandomMax(MAX_MON_MOVES);
        while (gBitTable[movePosition] & unusableMovesBits);
		
		gCurrMovePos = movePosition;
        gCalledMove = gBattleMons[gBattlerAttacker].moves[movePosition];
		BattleAI_RecordMoveUsed(gBattlerAttacker, movePosition);
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atkAA_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkAB_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkAC_trysetburn(void)
{
	CMD_ARGS(u8 battler);

	u8 battlerId = GetBattlerForBattleScript(cmd->battler);
	
	switch (CanBeBurned(gBattlerAttacker, battlerId, 0))
	{
		case STATUS_CHANGE_WORKED:
			if (SubsBlockMove(gBattlerAttacker, battlerId, gCurrentMove))
				gBattlescriptCurrInstr = BattleScript_ButItFailed;
			else
				gBattlescriptCurrInstr = cmd->nextInstr;
			break;
		case STATUS_CHANGE_FAIL_SPECIFIC_STATUSED:
			gBattlescriptCurrInstr = BattleScript_AlreadyBurned;
			break;
		case STATUS_CHANGE_FAIL_ALREADY_STATUSED:
			gBattlescriptCurrInstr = BattleScript_ButItFailed;
			break;
		case STATUS_CHANGE_FAIL_TYPE_NOT_AFFECTED:
			gBattlescriptCurrInstr = BattleScript_NotAffected;
			break;
		case STATUS_CHANGE_FAIL_SAFEGUARD_PROTECTED:
			gBattlescriptCurrInstr = BattleScript_SafeguardProtected;
			break;
		case STATUS_CHANGE_FAIL_ABILITY_PREVENTED:
			gBattlescriptCurrInstr = BattleScript_WaterVeilPrevents;
			break;
		case STATUS_CHANGE_FAIL_FLOWER_VEIL_ON_SIDE:
		    gEffectBattler = battlerId;
			gBattleScripting.battler = gBattleScripting.savedBattler;
			gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
			break;
	}
}

static void atkAD_tryspiteppreduce(void)
{
    CMD_ARGS(const u8 *failPtr);
	
	u16 lastMove = gBattleStruct->battlers[gBattlerTarget].lastMove;
	
    if (lastMove && lastMove != MOVE_UNAVAILABLE)
    {
		u8 movePos = FindMoveSlotInBattlerMoveset(gBattlerTarget, lastMove);
		
        if (movePos != MAX_MON_MOVES && gBattleMons[gBattlerTarget].pp[movePos])
        {
            u8 ppToDeduct = gBattleMoves[gCurrentMove].argument.spite.ppToDeduct;

            if (gBattleMons[gBattlerTarget].pp[movePos] < ppToDeduct)
                ppToDeduct = gBattleMons[gBattlerTarget].pp[movePos];
			
            PrepareMoveBuffer(gBattleTextBuff1, lastMove);
            ConvertIntToDecimalStringN(gBattleTextBuff2, ppToDeduct, STR_CONV_MODE_LEFT_ALIGN, 1);
            PrepareByteNumberBuffer(gBattleTextBuff2, 1, ppToDeduct);
            gBattleMons[gBattlerTarget].pp[movePos] -= ppToDeduct;
			
            if (MOVE_IS_PERMANENT(gBattlerTarget, movePos))
            {
                BtlController_EmitSetMonData(gBattlerTarget, BUFFER_A, REQUEST_PPMOVE1_BATTLE + movePos, 0, sizeof(gBattleMons[gBattlerTarget].pp[movePos]), &gBattleMons[gBattlerTarget].pp[movePos]);
                MarkBattlerForControllerExec(gBattlerTarget);
            }
			
			if (!gBattleMons[gBattlerTarget].pp[movePos])
                CancelMultiTurnMoves(gBattlerTarget);
			
            gBattlescriptCurrInstr = cmd->nextInstr;
			return;
        }
    }
	gBattlescriptCurrInstr = cmd->failPtr;
}

static void atkAE_healpartystatus(void)
{
	CMD_ARGS(u8 caseId);
	
	switch (cmd->caseId)
	{
		case 0: // Choose string
			switch (gCurrentMove)
			{
				case MOVE_HEAL_BELL:
				    gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_BELL_CHIMED;
					break;
				case MOVE_AROMATHERAPY:
				    gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SOOTHING_AROMA;
					break;
			}
			gBattlescriptCurrInstr = cmd->nextInstr;
			break;
		case 1: // Heal status
		    while (gBattleCommunication[MULTIUSE_STATE] < PARTY_SIZE)
			{
				u8 partyId = gBattleCommunication[MULTIUSE_STATE]++;
				
				if (partyId == gBattlerPartyIndexes[gBattlerAttacker])
					gBattleScripting.battler = gBattlerAttacker;
				else if (IsBattlerAlive(BATTLE_PARTNER(gBattlerAttacker)) && partyId == gBattlerPartyIndexes[BATTLE_PARTNER(gBattlerAttacker)])
					gBattleScripting.battler = BATTLE_PARTNER(gBattlerAttacker);
				else
					gBattleScripting.battler = MAX_BATTLERS_COUNT;
				
				if (gBattleScripting.battler != MAX_BATTLERS_COUNT)
				{
					if (gBattleMons[gBattleScripting.battler].status1.id)
					{
					    if (partyId != gBattlerPartyIndexes[gBattlerAttacker]) // Can't block self
					    {
							// Check Substitute and Soundproof
					    	if (GetBattlerAbility(gBattleScripting.battler) == ABILITY_SOUNDPROOF && gBattleMoves[gCurrentMove].flags.soundMove)
					        {
					        	BattleScriptCall(BattleScript_SoundproofBlocksString);
					        	break;
					        }
					    	else if (SubsBlockMove(gBattlerAttacker, gBattleScripting.battler, gCurrentMove))
							{
								BattleScriptCall(BattleScript_SubstituteBlocksHealBell);
								break;
							}
					    }
						PrepareMonNickBuffer(gBattleTextBuff2, gBattleScripting.battler, partyId);
						BattleScriptCall(BattleScript_HealBellActivateOnBattler);
						break;
					}
				}
				else
				{
					struct Pokemon *mon = &GetBattlerParty(gBattlerAttacker)[partyId];
					
					if (IsMonValidSpecies(mon) && !HealStatusConditions(mon, STATUS1_NONE, MAX_BATTLERS_COUNT))
					{
						PrepareMonNickBuffer(gBattleTextBuff2, gBattlerAttacker, partyId);
						BattleScriptCall(BattleScript_HealBellActivateOnParty);
						break;
					}
				}
			}
			if (gBattleCommunication[MULTIUSE_STATE] >= PARTY_SIZE)
				gBattlescriptCurrInstr = cmd->nextInstr;
			break;
	}
}

static void atkAF_cursetarget(void)
{
	CMD_ARGS(const u8 *failPtr);

    if (gBattleMons[gBattlerTarget].status2 & STATUS2_CURSED)
        gBattlescriptCurrInstr = cmd->failPtr;
    else
    {
        gBattleMons[gBattlerTarget].status2 |= STATUS2_CURSED;
		
        gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 2;
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
		
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atkB0_trysetspikes(void)
{
	CMD_ARGS(const u8 *failPtr);

    u8 targetSide = GetBattlerSide(BATTLE_OPPOSITE(gBattlerAttacker));

    if (gSideTimers[targetSide].spikesAmount == 3)
        gBattlescriptCurrInstr = cmd->failPtr;
    else
    {
		++gSideTimers[targetSide].spikesAmount;
        gSideStatuses[targetSide] |= SIDE_STATUS_SPIKES;
		AddBattleEffectToSideQueueList(targetSide, B_SIDE_QUEUED_SPIKES);
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atkB1_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkB2_trysetperishsong(void)
{
	CMD_ARGS(const u8 *failPtr);

    u8 i, notAffectedCount;

    for (i = 0, notAffectedCount = 0; i < gBattlersCount; ++i)
    {
        if ((gStatuses3[i] & STATUS3_PERISH_SONG) || (GetBattlerAbility(i) == ABILITY_SOUNDPROOF && i != gBattlerAttacker)) // Can't prevent the user
            ++notAffectedCount;
        else
        {
            gStatuses3[i] |= STATUS3_PERISH_SONG;
            gDisableStructs[i].perishSongTimer = 3;
        }
    }
	
    if (notAffectedCount == gBattlersCount)
        gBattlescriptCurrInstr = cmd->failPtr;
    else
        gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkB3_handlerollout(void)
{
	CMD_ARGS();

    if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
    {
        CancelMultiTurnMoves(gBattlerAttacker);
        gBattlescriptCurrInstr = BattleScript_MoveMissedPause;
    }
    else
    {
        if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS)) // first hit
        {
			gBattleMons[gBattlerAttacker].status2 |= STATUS2_MULTIPLETURNS;
            gDisableStructs[gBattlerAttacker].rolloutTimer = 5;
            gBattleStruct->battlers[gBattlerAttacker].lockedMove = gCurrentMove;
        }
        if (--gDisableStructs[gBattlerAttacker].rolloutTimer == 0) // last hit
            gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_MULTIPLETURNS);
		
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atkB4_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkB5_handlefurycutter(void)
{
	CMD_ARGS();

    if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
    {
        gDisableStructs[gBattlerAttacker].furyCutterCounter = 0;
        gBattlescriptCurrInstr = BattleScript_MoveMissedPause;
    }
    else
    {
		if (gBattleStruct->battlers[gBattlerAttacker].lastPrintedMove != gCurrentMove) // reset counter if another move was selected
		    gDisableStructs[gBattlerAttacker].furyCutterCounter = 0;
		
		// Don't increase counter on second hit of Parental Bond
        if (gDisableStructs[gBattlerAttacker].furyCutterCounter != 3 && gSpecialStatuses[gBattlerAttacker].parentalBondState != PARENTAL_BOND_2ND_HIT)
            ++gDisableStructs[gBattlerAttacker].furyCutterCounter;
		
		gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atkB6_trysetparalyze(void)
{
	CMD_ARGS(u8 battler);

	u8 battlerId = GetBattlerForBattleScript(cmd->battler);
	
	switch (CanBeParalyzed(gBattlerAttacker, battlerId, 0))
	{
		case STATUS_CHANGE_WORKED:
		    if (SubsBlockMove(gBattlerAttacker, battlerId, gCurrentMove))
				gBattlescriptCurrInstr = BattleScript_ButItFailed;
			else
				gBattlescriptCurrInstr = cmd->nextInstr;
			break;
		case STATUS_CHANGE_FAIL_SPECIFIC_STATUSED:
			gBattlescriptCurrInstr = BattleScript_AlreadyParalyzed;
			break;
		case STATUS_CHANGE_FAIL_ALREADY_STATUSED:
			gBattlescriptCurrInstr = BattleScript_ButItFailed;
			break;
		case STATUS_CHANGE_FAIL_TYPE_NOT_AFFECTED:
			gBattlescriptCurrInstr = BattleScript_NotAffected;
			break;
		case STATUS_CHANGE_FAIL_SAFEGUARD_PROTECTED:
			gBattlescriptCurrInstr = BattleScript_SafeguardProtected;
			break;
		case STATUS_CHANGE_FAIL_ABILITY_PREVENTED:
			gBattlescriptCurrInstr = BattleScript_LimberProtected;
			break;
		case STATUS_CHANGE_FAIL_FLOWER_VEIL_ON_SIDE:
		    gEffectBattler = battlerId;
			gBattleScripting.battler = gBattleScripting.savedBattler;
			gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
			break;
	}
}

static void atkB7_presentcalc(void)
{
	CMD_ARGS();
	
	if (gSpecialStatuses[gBattlerAttacker].parentalBondState != PARENTAL_BOND_2ND_HIT)
	{
		s32 rand = Random() & 0xFF;
		
		if (rand < 102)
            gBattleStruct->presentBasePower = 40;
        else if (rand < 178)
            gBattleStruct->presentBasePower = 80;
        else if (rand < 204)
            gBattleStruct->presentBasePower = 120;
        else
        {
            gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP / 4;
            if (gBattleMoveDamage == 0)
                gBattleMoveDamage = 1;
            gBattleMoveDamage *= -1;
            gBattleStruct->presentBasePower = 0;
        }
	}

	if (gBattleStruct->presentBasePower)
		gBattlescriptCurrInstr = BattleScript_HitFromCritCalc;
	else if (gStatuses3[gBattlerTarget] & STATUS3_HEAL_BLOCK)
		gBattlescriptCurrInstr = BattleScript_ButItFailed;
	else if (BATTLER_MAX_HP(gBattlerTarget))
		gBattlescriptCurrInstr = BattleScript_AlreadyAtFullHp;
	else
		gBattlescriptCurrInstr = BattleScript_PresentHealTarget;
}

static void atkB8_trysetsafeguard(void)
{
	CMD_ARGS(const u8 *failPtr);

	u8 battlerSide = GetBattlerSide(gBattlerAttacker);
	
    if (gSideStatuses[battlerSide] & SIDE_STATUS_SAFEGUARD)
		gBattlescriptCurrInstr = cmd->failPtr;
    else
    {
        gSideStatuses[battlerSide] |= SIDE_STATUS_SAFEGUARD;
        gSideTimers[battlerSide].safeguardTimer = 5;
        gSideTimers[battlerSide].safeguardBattlerId = gBattlerAttacker;
        gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_SAFEGUARD_COVERED;
		gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atkB9_magnitudedamagecalculation(void)
{
	CMD_ARGS();

	gBattlescriptCurrInstr = cmd->nextInstr;

	if (!gBattleStruct->magnitudeBasePower) // Check power has't been calculated
	{
		u8 power;
		s32 magnitude = RandomMax(100);
		
		if (magnitude < 5)
		{
			power = 10;
			magnitude = 4;
		}
		else if (magnitude < 15)
		{
			power = 30;
			magnitude = 5;
		}
		else if (magnitude < 35)
		{
			power = 50;
			magnitude = 6;
		}
		else if (magnitude < 65)
		{
			power = 70;
			magnitude = 7;
		}
		else if (magnitude < 85)
		{
			power = 90;
			magnitude = 8;
		}
		else if (magnitude < 95)
		{
			power = 110;
			magnitude = 9;
		}
		else
		{
			power = 150;
			magnitude = 10;
		}
		gBattleStruct->magnitudeBasePower = power;
		PrepareByteNumberBuffer(gBattleTextBuff1, 2, magnitude);
		BattleScriptCall(BattleScript_MagnitudeString);
	}
}

static void atkBA_jumpifnopursuitswitchdmg(void)
{
	CMD_ARGS(const u8 *ptr);
	
	s32 i;
	u8 toDoAction;
	
    if (gMultiHitCounter == 1)
    {
        if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
            gBattlerTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
        else
            gBattlerTarget = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
    }
    else
    {
        if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
            gBattlerTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
        else
            gBattlerTarget = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
    }
	
	toDoAction = B_ACTION_USE_MOVE;
	for (i = gBattlersCount - 1; i >= 0; --i)
	{
		if (gBattlerByTurnOrder[i] == gBattlerTarget)
		{
			toDoAction = gActionsByTurnOrder[i];
			break;
		}
		else if (gBattlerByTurnOrder[i] == gBattlerAttacker) // If the attacker is reached it means the Pursuiter already attacked
		{
			toDoAction = B_ACTION_FINISHED;
			break;
		}
	}
	
    if (IsBattlerAlive(gBattlerAttacker) && gBattleStruct->battlers[gBattlerTarget].chosenAction == B_ACTION_USE_MOVE && toDoAction == B_ACTION_USE_MOVE
	&& gBattleMoves[gBattleStruct->battlers[gBattlerTarget].chosenMove].effect == EFFECT_PURSUIT && !gDisableStructs[gBattlerTarget].truantCounter
	&& gBattleMons[gBattlerTarget].status1.id != STATUS1_SLEEP && gBattleMons[gBattlerTarget].status1.id != STATUS1_FREEZE)
    {
        for (i = 0; i < gBattlersCount; ++i)
		{
            if (gBattlerByTurnOrder[i] == gBattlerTarget)
                gActionsByTurnOrder[i] = B_ACTION_TRY_FINISH;
		}
        gCurrentMove = gBattleStruct->battlers[gBattlerTarget].chosenMove;
        gCurrMovePos = gChosenMovePos = gBattleStruct->battlers[gBattlerTarget].chosenMovePosition;
        gBattleScripting.animTurn = 1;
        gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED | HITMARKER_NO_ATTACKSTRING);
		SetTypeBeforeUsingMove(gCurrentMove, gBattlerTarget);
		gBattlescriptCurrInstr = cmd->nextInstr;
    }
    else
        gBattlescriptCurrInstr = cmd->ptr;
}

static void atkBB_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkBC_maxattackhalvehp(void)
{
	CMD_ARGS(const u8 *failPtr);

    u32 halfHp = gBattleMons[gBattlerAttacker].maxHP / 2;
    if (halfHp == 0)
        halfHp = 1;
	
    if (gBattleMons[gBattlerAttacker].statStages[STAT_ATK] < MAX_STAT_STAGES && gBattleMons[gBattlerAttacker].hp > halfHp) // Ignore Contrary
    {
		gBattleMoveDamage = halfHp;
		gBattlescriptCurrInstr = cmd->nextInstr;
    }
    else
        gBattlescriptCurrInstr = cmd->failPtr;
}

static void atkBD_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkBE_rapidspinfree(void)
{
	CMD_ARGS(u8 battler);
	
	u8 battlerId = GetBattlerForBattleScript(cmd->battler);

    if (gDisableStructs[battlerId].wrapTurns)
    {
		gDisableStructs[battlerId].wrapTurns = 0;
		PrepareMonNickBuffer(gBattleTextBuff1, gDisableStructs[battlerId].wrappedBy, gBattlerPartyIndexes[gDisableStructs[battlerId].wrappedBy]);
		PrepareMoveBuffer(gBattleTextBuff2, gDisableStructs[battlerId].wrappedMove);
		BattleScriptCall(BattleScript_WrapFree);
    }
    else if (gStatuses3[battlerId] & STATUS3_LEECHSEED)
    {
        gStatuses3[battlerId] &= ~(STATUS3_LEECHSEED);
		BattleScriptCall(BattleScript_LeechSeedFree);
    }
    else if (TryRemoveEntryHazards(battlerId))
		BattleScriptCall(BattleScript_SpikesFree);
    else
        gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkBF_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkC0_recoverbasedonweather(void)
{
	CMD_ARGS(const u8 *failPtr);

	if (BATTLER_MAX_HP(gBattlerTarget))
		gBattlescriptCurrInstr = cmd->failPtr;
	else
	{
		if (IsBattlerWeatherAffected(gBattlerTarget, B_WEATHER_SUN_ANY))
			gBattleMoveDamage = (20 * gBattleMons[gBattlerTarget].maxHP) / 30; // Sun
		else if (IsBattlerWeatherAffected(gBattlerTarget, B_WEATHER_ANY & ~(B_WEATHER_STRONG_WINDS)))
			gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP / 4; // Any other weather, except Strong Winds
		else
			gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP / 2; // No Weather or Strong Winds
		
		if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        gBattleMoveDamage *= -1;
		
		gBattlescriptCurrInstr = cmd->nextInstr;
	}
}

static void atkC1_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkC2_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkC3_trysetfutureattack(void)
{
	CMD_ARGS(const u8 *failPtr);

    if (gBattleStruct->battlers[gBattlerTarget].futureSightCounter)
        gBattlescriptCurrInstr = cmd->failPtr;
    else
    {
		gBattleStruct->battlers[gBattlerTarget].futureSightCounter = 3;
        gBattleStruct->battlers[gBattlerTarget].futureSightMove = gCurrentMove;
        gBattleStruct->battlers[gBattlerTarget].futureSightAttacker = gBattlerAttacker;
		AddBattleEffectToBattlerQueueList(gBattlerTarget, B_BATTLER_QUEUED_FUTURE_SIGHT);
		gBattleCommunication[MULTISTRING_CHOOSER] = gBattleMoves[gCurrentMove].argument.futureAttack.stringId; // Get string to print
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atkC4_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkC5_setsemiinvulnerablebit(void)
{
	CMD_ARGS(bool8 clear);
	
	if (GET_MOVE_MOVEEFFECT_TABLE(gCurrentMove).semiInvulnerableEffect)
	{
		u32 semiInvulnerableEffect = UNCOMPRESS_BITS(gBattleMoves[gCurrentMove].argument.semiInvulnerable.status);
		
		if (cmd->clear)
			gStatuses3[gBattlerAttacker] &= ~(semiInvulnerableEffect);
		else
			gStatuses3[gBattlerAttacker] |= semiInvulnerableEffect;
	}
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkC6_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkC7_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkC8_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkC9_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkCA_nop(void)
{
    CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkCB_nop(void)
{
    CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkCC_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkCD_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkCE_settorment(void)
{
	CMD_ARGS(const u8 *failPtr);

    if (gBattleMons[gBattlerTarget].status2 & STATUS2_TORMENT)
        gBattlescriptCurrInstr = cmd->failPtr;
    else
    {
        gBattleMons[gBattlerTarget].status2 |= STATUS2_TORMENT;
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atkCF_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkD0_settaunt(void)
{
	CMD_ARGS(const u8 *failPtr);

    if (!gDisableStructs[gBattlerTarget].tauntTimer)
    {
        gDisableStructs[gBattlerTarget].tauntTimer = GetBattlerTurnOrderNum(gBattlerTarget) > gCurrentTurnActionNumber ? 3 : 4;
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
    else
        gBattlescriptCurrInstr = cmd->failPtr;
}

static void atkD1_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkD2_tryswapitems(void) // trick
{
    u8 sideAttacker = GetBattlerSide(gBattlerAttacker), sideTarget = GetBattlerSide(gBattlerTarget);
    u16 oldItemAtk, *newItemAtk;
	
    // opponent can't swap items with player in regular battles
    if ((GetBattlerSide(gBattlerAttacker) == B_SIDE_OPPONENT && !(gBattleTypeFlags & BATTLE_TYPE_LINK)))
        gBattlescriptCurrInstr = READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        // can't swap if two pokemon don't have an item
        // or if either of them is an enigma berry or a mail
        if ((gBattleMons[gBattlerAttacker].item == 0 && gBattleMons[gBattlerTarget].item == 0) || ItemIsMail(gBattleMons[gBattlerAttacker].item)
		 || ItemIsMail(gBattleMons[gBattlerTarget].item))
            gBattlescriptCurrInstr = READ_PTR(gBattlescriptCurrInstr + 1);
        // check if ability prevents swapping
        else if (GetBattlerAbility(gBattlerTarget) == ABILITY_STICKY_HOLD)
            gBattlescriptCurrInstr = BattleScript_StickyHoldActivates;
        // took a while, but all checks passed and items can be safely swapped
        else
        {
            /*newItemAtk = &gBattleStruct->changedItems[gBattlerAttacker];
            oldItemAtk = gBattleMons[gBattlerAttacker].item;
            *newItemAtk = gBattleMons[gBattlerTarget].item;
            gBattleMons[gBattlerAttacker].item = 0;
            gBattleMons[gBattlerTarget].item = oldItemAtk;
            BtlController_EmitSetMonData(gBattlerAttacker, BUFFER_A, REQUEST_HELDITEM_BATTLE, 0, sizeof(gBattleMons[gBattlerTarget].item), newItemAtk);
            MarkBattlerForControllerExec(gBattlerAttacker);
            BtlController_EmitSetMonData(gBattlerTarget, BUFFER_A, REQUEST_HELDITEM_BATTLE, 0, sizeof(gBattleMons[gBattlerTarget].item), &gBattleMons[gBattlerTarget].item);
            MarkBattlerForControllerExec(gBattlerTarget);
			// Check Gorilla Tactics here
            gBattleStruct->battlers[gBattlerTarget].choicedMove = MOVE_NONE;
            gBattleStruct->battlers[gBattlerAttacker].choicedMove = MOVE_NONE;
            gBattlescriptCurrInstr += 5;
            PrepareItemBuffer(gBattleTextBuff1, *newItemAtk);
            PrepareItemBuffer(gBattleTextBuff2, oldItemAtk);
            if (oldItemAtk != ITEM_NONE && *newItemAtk != ITEM_NONE)
                gBattleCommunication[MULTISTRING_CHOOSER] = 2; // attacker's item -> <- target's item
            else if (oldItemAtk == ITEM_NONE && *newItemAtk != ITEM_NONE)
			{
				if (GetBattlerAbility(gBattlerAttacker) == ABILITY_UNBURDEN && gDisableStructs[gBattlerAttacker].unburdenBoost)
					gDisableStructs[gBattlerAttacker].unburdenBoost = FALSE;
				
				gBattleCommunication[MULTISTRING_CHOOSER] = 0; // nothing -> <- target's item
			}
            else
			{
				CheckSetBattlerUnburden(gBattlerAttacker);
                gBattleCommunication[MULTISTRING_CHOOSER] = 1; // attacker's item -> <- nothing
			}*/
        }
    }
}

static void atkD3_copyability(void) // role play
{
	CMD_ARGS(u8 attacker, u8 target);
	u8 attacker = GetBattlerForBattleScript(cmd->attacker);
	gBattleStruct->battlers[attacker].abilityOverride = SetBattlerAbility(attacker, gBattleMons[GetBattlerForBattleScript(cmd->target)].ability);
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkD4_trywish(void)
{
	CMD_ARGS(u8 caseId, const u8 *failPtr);

    switch (cmd->caseId)
    {
		case 0: // use wish
			if (!gBattleStruct->battlers[gBattlerAttacker].wishCounter)
			{
				gBattleStruct->battlers[gBattlerAttacker].wishCounter = 2;
				gBattleStruct->battlers[gBattlerAttacker].wishMonId = gBattlerPartyIndexes[gBattlerAttacker];
				AddBattleEffectToBattlerQueueList(gBattlerAttacker, B_BATTLER_QUEUED_WISH);
				gBattlescriptCurrInstr = cmd->nextInstr;
			}
			else
				gBattlescriptCurrInstr = cmd->failPtr;
			break;
		case 1: // heal effect
			PrepareMonNickWithPrefixBuffer(gBattleTextBuff1, gBattlerTarget, gBattleStruct->battlers[gBattlerTarget].wishMonId);
			
			if (BATTLER_MAX_HP(gBattlerTarget))
				gBattlescriptCurrInstr = cmd->failPtr;
			else
			{
				gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP / 2;
				if (gBattleMoveDamage == 0)
					gBattleMoveDamage = 1;
				gBattleMoveDamage *= -1;
				
				gBattlescriptCurrInstr = cmd->nextInstr;
			}
			break;
    }
}

static void atkD5_trysetroots(void) // ingrain
{
	CMD_ARGS(const u8 *failPtr);

    if (gStatuses3[gBattlerAttacker] & STATUS3_ROOTED)
        gBattlescriptCurrInstr = cmd->failPtr;
    else
    {
        gStatuses3[gBattlerAttacker] |= STATUS3_ROOTED;
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atkD6_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkD7_setyawn(void)
{
	CMD_ARGS(const u8 *failPtr);

    if (gStatuses3[gBattlerTarget] & STATUS3_YAWN)
        gBattlescriptCurrInstr = cmd->failPtr;
    else
    {
        gStatuses3[gBattlerTarget] |= STATUS3_YAWN_TURN(2);
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atkD8_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkD9_nop(void)
{
    CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkDA_swapabilities(void) // skill swap
{
	CMD_ARGS();
    gBattleStruct->battlers[gBattlerAttacker].abilityOverride = SetBattlerAbility(gBattlerAttacker, gBattleMons[gBattlerTarget].ability);
	gBattleStruct->battlers[gBattlerTarget].abilityOverride = SetBattlerAbility(gBattlerTarget, gBattleStruct->battlers[gBattlerAttacker].abilityOverride);
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkDB_tryimprison(void)
{
	CMD_ARGS(const u8 *failPtr);

    if (gStatuses3[gBattlerAttacker] & STATUS3_IMPRISONED_OTHERS)
        gBattlescriptCurrInstr = cmd->failPtr;
    else
    {
		gStatuses3[gBattlerAttacker] |= STATUS3_IMPRISONED_OTHERS;
		gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atkDC_trysetgrudge(void)
{
	CMD_ARGS(const u8 *failPtr);

    if (gStatuses3[gBattlerAttacker] & STATUS3_GRUDGE)
        gBattlescriptCurrInstr = cmd->failPtr;
    else
    {
        gStatuses3[gBattlerAttacker] |= STATUS3_GRUDGE;
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atkDD_mefirstattackselect(void)
{
	CMD_ARGS(const u8 *failPtr);

	u16 move = gBattleStruct->battlers[gBattlerTarget].chosenMove;
	
	if (GetBattlerTurnOrderNum(gBattlerTarget) < gCurrentTurnActionNumber || !move || IS_MOVE_STATUS(move) || gBattleMoves[move].flags.forbiddenMeFirst)
		gBattlescriptCurrInstr = cmd->failPtr;
	else
	{
		gCalledMove = move;
		gBattleStruct->meFirstBoost = TRUE;
		gBattlescriptCurrInstr = cmd->nextInstr;
	}
}

static void atkDE_assistattackselect(void)
{
	CMD_ARGS(const u8 *failPtr);

	u8 monId, moveId, chooseableMovesNo = 0;
    u16 move, *movesArray = Alloc(sizeof(u16) * PARTY_SIZE * MAX_MON_MOVES); // 6 mons, each of them knowing 4 moves
    struct Pokemon *party;

    if (movesArray != NULL)
	{
		party = GetBattlerParty(gBattlerAttacker);
		
		for (monId = 0; monId < PARTY_SIZE; ++monId)
		{
			if (monId != gBattlerPartyIndexes[gBattlerAttacker] && IsMonValidSpecies(&party[monId]))
			{
				for (moveId = 0; moveId < MAX_MON_MOVES; ++moveId)
				{
					move = GetMonData(&party[monId], MON_DATA_MOVE1 + moveId);
					
					if (move && move != MOVE_STRUGGLE && !gBattleMoves[move].flags.forbiddenAssist)
						movesArray[chooseableMovesNo++] = move;
				}
			}
		}
	}
    if (chooseableMovesNo)
    {
        gCalledMove = movesArray[RandomMax(chooseableMovesNo)];
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
    else
        gBattlescriptCurrInstr = cmd->failPtr;
	
	FREE_IF_NOT_NULL(movesArray);
}

static void atkDF_trysetmagiccoat(void)
{
	CMD_ARGS(const u8 *failPtr);

    gBattlerTarget = gBattlerAttacker;
	
    if (gCurrentTurnActionNumber == gBattlersCount - 1) // moves last turn
        gBattlescriptCurrInstr = cmd->failPtr;
    else
    {
        gProtectStructs[gBattlerAttacker].bounceMove = TRUE;
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atkE0_trysetsnatch(void) // snatch
{
	CMD_ARGS(const u8 *failPtr);

    if (gCurrentTurnActionNumber == gBattlersCount - 1) // moves last turn
        gBattlescriptCurrInstr = cmd->failPtr;
    else
    {
        gProtectStructs[gBattlerAttacker].stealMove = 1;
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atkE1_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkE2_switchoutabilities(void)
{
	CMD_ARGS(u8 battler);

	u16 temp, ability;
	u8 battlerId = GetBattlerForBattleScript(cmd->battler);
	
	// For effects that need gBattlerPartyIndexes to be properly set
	SWAP(gBattlerPartyIndexes[battlerId], gBattleStruct->battlers[battlerId].partyIndex, temp);
	
	ability = GetBattlerAbility(battlerId);
	
	switch (ability)
	{
		case ABILITY_NATURAL_CURE:
		    gBattleMons[battlerId].status1.id = STATUS1_NONE;
			
			BtlController_EmitSetMonData(battlerId, BUFFER_A, REQUEST_STATUS_BATTLE, gBitTable[gBattlerPartyIndexes[battlerId]], sizeof(gBattleMons[battlerId].status1), &gBattleMons[battlerId].status1);
			MarkBattlerForControllerExec(battlerId);
			
			BattleAI_RecordAbility(battlerId);
			break;
		case ABILITY_REGENERATOR:
		    gBattleMoveDamage = (gBattleMons[battlerId].maxHP / 3) + gBattleMons[battlerId].hp;
			if (gBattleMoveDamage > gBattleMons[battlerId].maxHP)
				gBattleMoveDamage = gBattleMons[battlerId].maxHP;
			
			BtlController_EmitSetMonData(battlerId, BUFFER_A, REQUEST_HP_BATTLE, gBitTable[gBattlerPartyIndexes[battlerId]], sizeof(gBattleMons[battlerId].hp), &gBattleMoveDamage);
			MarkBattlerForControllerExec(battlerId);
			
			BattleAI_RecordAbility(battlerId);
			break;
		case ABILITY_ZERO_TO_HERO:
		{
		    u16 newSpecies = TryDoBattleFormChange(battlerId, FORM_CHANGE_SWITCH_OUT);
			
			if (newSpecies)
			{
				DoBattleFormChange(battlerId, newSpecies, FALSE, TRUE, FALSE);
				gBattleStruct->sides[GetBattlerSide(battlerId)].party[gBattlerPartyIndexes[battlerId]].zeroToHeroActivated = TRUE; // for switch in message activate
			}
			break;
		}
	    case ABILITY_NEUTRALIZING_GAS:
		    PrepareAbilityBuffer(gBattleTextBuff1, ability);
			
			gBattleMons[battlerId].ability = ABILITY_NONE; // for don't reactivate and stay in a infinite loop
			
			SaveAttackerToStack(battlerId);
			BattleScriptCall(BattleScript_NeutralizingGasExits);
			return;
	}
	DoSpecialFormChange(battlerId, gBattlerPartyIndexes[battlerId], FORM_CHANGE_SWITCH_OUT);
	
	SWAP(gBattlerPartyIndexes[battlerId], gBattleStruct->battlers[battlerId].partyIndex, temp);
	
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkE3_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkE4_nop(void)
{
    CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void TryPickupItem(struct Pokemon *mon, u16 itemId)
{
#if PICKUP_ITEM_TO_BAG
	if (AddBagItem(itemId, 1))
		return;
#endif

	if (!GetMonData(mon, MON_DATA_HELD_ITEM))
		SetMonData(mon, MON_DATA_HELD_ITEM, &itemId);
}

static void atkE5_pickup(void)
{
	CMD_ARGS();
	
	u8 i;
	u32 j;
	
    for (i = 0; i < PARTY_SIZE; ++i)
    {
		struct Pokemon *mon = &gPlayerParty[i];
		
		if (IsMonValidSpecies(mon))
		{
			switch (GetMonAbility(mon))
			{
				case ABILITY_PICKUP:
					if (!RandomMax(10))
					{
						for (j = 0; j < 15; ++j)
						{
							if (sPickupItems[j].chance > RandomMax(100))
								break;
						}
						TryPickupItem(mon, sPickupItems[j].itemId);
					}
					break;
				case ABILITY_HONEY_GATHER:
					{
						u8 chance = (GetMonData(mon, MON_DATA_LEVEL) - 1) / 10;
						if (chance > 9)
							chance = 9;
						
						if ((chance + 1) * 5 > RandomMax(100))
							TryPickupItem(mon, ITEM_HONEY);
					}
					break;
				default:
#if SHUCKLE_MAKES_BERRY_JUICE_CHANCE != 0
					if (RandomPercent(SHUCKLE_MAKES_BERRY_JUICE_CHANCE) && GetMonData(mon, MON_DATA_SPECIES2) == SPECIES_SHUCKLE
					&& ItemId_GetPocket(GetMonData(mon, MON_DATA_HELD_ITEM)) == POCKET_BERRY_POUCH)
					{
						u16 item = ITEM_BERRY_JUICE;
						SetMonData(mon, MON_DATA_HELD_ITEM, &item);
					}
#endif
					break;
			}
		}
	}
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkE6_nop(void)
{
    CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkE7_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkE8_setfieldsport(void)
{
	CMD_ARGS(const u8 *failPtr);

    bool8 worked = FALSE;
	
	switch (gBattleMoves[gCurrentMove].effect)
	{
		case EFFECT_MUD_SPORT:
		    if (!(gFieldStatus & STATUS_FIELD_MUDSPORT))
			{
				gFieldStatus |= STATUS_FIELD_MUDSPORT;
				gFieldTimers.mudSportTimer = 5;
				gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_ELECTRICITY_WEAKENED;
				worked = TRUE;
			}
			break;
		case EFFECT_WATER_SPORT:
		    if (!(gFieldStatus & STATUS_FIELD_WATERSPORT))
		    {
		    	gFieldStatus |= STATUS_FIELD_WATERSPORT;
		    	gFieldTimers.waterSportTimer = 5;
		    	gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_FIRE_WEAKENED;
		    	worked = TRUE;
		    }
			break;
	}
	
    if (!worked)
        gBattlescriptCurrInstr = cmd->failPtr;
	else
		gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkE9_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkEA_tryrecycleitem(void)
{
	CMD_ARGS(const u8 *failPtr);

	if (TryRecycleBattlerItem(gBattlerAttacker, gBattlerAttacker))
		gBattlescriptCurrInstr = cmd->nextInstr;
	else
		gBattlescriptCurrInstr = cmd->failPtr;
}

static void atkEB_settypetoterrain(void)
{
	CMD_ARGS(const u8 *failPtr);

	u8 type = gBattleTerrainTable[gBattleTerrain].camouflageType;
	
    if (!IsBattlerOfType(gBattlerAttacker, type))
    {
		SetBattlerType(gBattlerAttacker, type);
        gBattlescriptCurrInstr = cmd->nextInstr;
    }
    else
        gBattlescriptCurrInstr = cmd->failPtr;
}

static void atkEC_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkED_nop(void)
{
    CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkEE_nop(void)
{
    CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

u8 GetCatchingBattler(void)
{
	u8 leftOpponent = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
	return IsBattlerAlive(leftOpponent) ? leftOpponent : GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
}

static void DoMonCaughtEffects(void)
{
	struct Pokemon *mon = GetBattlerPartyIndexPtr(gBattlerTarget);
	
	if (CalculatePlayerPartyCount() == PARTY_SIZE)
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	else
		gBattleCommunication[MULTISTRING_CHOOSER] = 1;
	
	SetMonData(mon, MON_DATA_POKEBALL, &gLastUsedItem);
	
	switch (gLastUsedItem)
	{
		case ITEM_HEAL_BALL:
		    MonRestorePP(mon);
			HealStatusConditions(mon, STATUS1_NONE, gBattlerTarget);
			gBattleMons[gBattlerTarget].hp = gBattleMons[gBattlerTarget].maxHP;
			SetMonData(mon, MON_DATA_HP, &gBattleMons[gBattlerTarget].hp);
			break;
		case ITEM_FRIEND_BALL:
		{
		    u8 friendBallFriendship = 150;
		    SetMonData(mon, MON_DATA_FRIENDSHIP, &friendBallFriendship);
			break;
		}
	}
	
	if (gLastUsedItem != ITEM_SAFARI_BALL && GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
		IncrementGameStat(GAME_STAT_POKEMON_CAPTURES);
	
	gBattlescriptCurrInstr = BattleScript_SuccessBallThrow;
}

static void atkEF_handleballthrow(void)
{
	CMD_ARGS();

    if (!gBattleControllerExecFlags)
    {
        gBattlerTarget = GetCatchingBattler();
	    
        if (gBattleTypeFlags & BATTLE_TYPE_GHOST) // Can't throw ball in ghost battle
        {
            BtlController_EmitBallThrowAnim(gBattlerAttacker, BUFFER_A, BALL_GHOST_DODGE);
            MarkBattlerForControllerExec(gBattlerAttacker);
            gBattlescriptCurrInstr = BattleScript_GhostBallDodge;
        }
        else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER) // Can't throw ball in trainer battles
        {
#if RESTORE_BALL_TRAINER_BLOCK
			AddBagItem(gLastUsedItem, 1);
#endif
            BtlController_EmitBallThrowAnim(gBattlerAttacker, BUFFER_A, BALL_TRAINER_BLOCK);
            MarkBattlerForControllerExec(gBattlerAttacker);
            gBattlescriptCurrInstr = BattleScript_TrainerBallBlock;
        }
        else if (gBattleTypeFlags & (BATTLE_TYPE_POKEDUDE | BATTLE_TYPE_OLD_MAN_TUTORIAL)) // Poke Dude and Old Man battles aways success the ball throw
        {
            BtlController_EmitBallThrowAnim(gBattlerAttacker, BUFFER_A, BALL_3_SHAKES_SUCCESS);
            MarkBattlerForControllerExec(gBattlerAttacker);
            gBattlescriptCurrInstr = BattleScript_OldMan_Pokedude_CaughtMessage;
        }
        else
        {
			u8 catchRate, shakes;
			s32 ballMultiplier;
			u32 odds;
			s8 ballAdition = 0;
			
			// Get ball catch Rate
			if (gLastUsedItem == ITEM_SAFARI_BALL)
				catchRate = gBattleStruct->safariCatchFactor * 1275 / 100;
			else
				catchRate = gSpeciesInfo[GetMonData(GetBattlerPartyIndexPtr(gBattlerTarget), MON_DATA_SPECIES)].catchRate; // Don't copy catchRate if transformed
			
			// Get ball catch multiplier
			ballMultiplier = 100; // default multiplier x1.0
			
			// Check Ultra Beast
			if (gSpeciesInfo[gBattleMons[gBattlerTarget].species].flags & SPECIES_FLAG_ULTRA_BEAST)
			{
				if (gLastUsedItem == ITEM_BEAST_BALL)
					ballMultiplier = 500;
				else
					ballMultiplier = 10;
			}
			else
			{
				switch (gLastUsedItem)
			    {
			    	case ITEM_ULTRA_BALL:
			    	    ballMultiplier = 200;
			    		break;
			    	case ITEM_GREAT_BALL:
			    	case ITEM_SAFARI_BALL:
			    	    ballMultiplier = 150;
			    		break;
			    	case ITEM_NET_BALL:
			    	    if (IsBattlerAnyType(gBattlerTarget, TYPE_WATER, TYPE_BUG))
			    			ballMultiplier = 350;
			    		break;
			    	case ITEM_DIVE_BALL:
			    	    if (GetCurrentMapType() == MAP_TYPE_UNDERWATER || gIsFishingEncounter || gIsSurfingEncounter)
			    			ballMultiplier = 350;
			    		break;
			    	case ITEM_NEST_BALL:
			    	    if (gBattleMons[gBattlerTarget].level < 30)
			    			ballMultiplier = 410 - (gBattleMons[gBattlerTarget].level * 10);
			    		break;
			    	case ITEM_REPEAT_BALL:
			    	    if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[gBattlerTarget].species), FLAG_GET_CAUGHT))
			    			ballMultiplier = 350;
			    		break;
			    	case ITEM_TIMER_BALL:
			    	    ballMultiplier = (gBattleStruct->battleTurnCounter * 30) + 100;
                        if (ballMultiplier > 400)
                            ballMultiplier = 400;
                        break;
			    	case ITEM_DUSK_BALL:
			    	    if (gMapHeader.cave || GetCurrentMapType() == MAP_TYPE_UNDERGROUND || GetDNSTimeLapseDayOrNight() == TIME_NIGHT)
			    			ballMultiplier = 300;
			    		break;
			    	case ITEM_QUICK_BALL:
			    	    if (gBattleStruct->battleTurnCounter == 0)
			    			ballMultiplier = 500;
			    		break;
			    	case ITEM_FAST_BALL:
			    	    if (gSpeciesInfo[gBattleMons[gBattlerTarget].species].baseSpeed >= 100)
			    			ballMultiplier = 400;
			    		break;
			    	case ITEM_LEVEL_BALL:
			    	    if (gBattleMons[gBattlerAttacker].level >= 4 * gBattleMons[gBattlerTarget].level)
                            ballMultiplier = 800;
                        else if (gBattleMons[gBattlerAttacker].level > 2 * gBattleMons[gBattlerTarget].level)
                            ballMultiplier = 400;
                        else if (gBattleMons[gBattlerAttacker].level > gBattleMons[gBattlerTarget].level)
                            ballMultiplier = 200;
                        break;
			    	case ITEM_LURE_BALL:
			    	    if (gIsFishingEncounter)
			    			ballMultiplier = 500;
			    		break;
			    	case ITEM_HEAVY_BALL:
					{
			    	    u32 weight = gSpeciesInfo[gBattleMons[gBattlerTarget].species].weight;
			    	
			    	    if (weight < 1000)
			    			ballAdition = -20;
			    		else if (weight < 2000)
			    			ballAdition = 0;
			    		else if (weight < 3000)
			    			ballAdition = 20;
			    		else
			    			ballAdition = 30;
			    		break;
					}
			    	case ITEM_LOVE_BALL:
			    	    if (gBattleMons[gBattlerAttacker].species == gBattleMons[gBattlerTarget].species)
			    		{
							u8 atkGender = GetBattlerGender(gBattlerAttacker), defGender = GetBattlerGender(gBattlerTarget);
							
			    			if (atkGender != MON_GENDERLESS && defGender != MON_GENDERLESS && atkGender != defGender)
			    			    ballMultiplier = 800;
			    		}
			    		break;
			    	case ITEM_MOON_BALL:
					{
			    	    const u8 *evolutions = gSpeciesInfo[gBattleMons[gBattlerTarget].species].evolutions;
			    		
			    		if (evolutions != NULL)
			    		{
							while (*evolutions != EVOLUTIONS_END)
							{
								if (*evolutions == EVO_REQ_ITEM && READ_16(evolutions + 1) == ITEM_MOON_STONE)
								{
									ballMultiplier = 400;
									break;
								}
								evolutions += gEvolutionCmdArgumentsSize[*evolutions] + 1;
							}
			    		}
			    		break;
					}
			    	case ITEM_BEAST_BALL:
			    	    ballMultiplier = 10;
			    		break;
			    	case ITEM_DREAM_BALL:
			    	    if (gBattleMons[gBattlerTarget].status1.id == STATUS1_SLEEP || GetBattlerAbility(gBattlerTarget) == ABILITY_COMATOSE)
			    			ballMultiplier = 40;
			    		break;
			    }
			}
			// catchRate is unsigned, which means that it may potentially overflow if sum is applied directly.
			if (catchRate < 21 && ballAdition == -20)
				catchRate = 1;
			else
				catchRate += ballAdition;
			
			odds = (catchRate * ballMultiplier / 100) * (gBattleMons[gBattlerTarget].maxHP * 3 - gBattleMons[gBattlerTarget].hp * 2) / (3 * gBattleMons[gBattlerTarget].maxHP);
			
			switch (gBattleMons[gBattlerTarget].status1.id)
			{
				case STATUS1_NONE:
					break;
				case STATUS1_SLEEP:
				case STATUS1_FREEZE:
					odds *= 2;
					break;
				default:
					odds = (odds * 15) / 10;
					break;
			}

            if (odds > 254) // mon caught
			{
				BtlController_EmitBallThrowAnim(gBattlerAttacker, BUFFER_A, BALL_3_SHAKES_SUCCESS);
                MarkBattlerForControllerExec(gBattlerAttacker);
			    DoMonCaughtEffects();
            }
            else // mon may be caught, calculate shakes
            {
                odds = Sqrt(Sqrt(16711680 / odds));
                odds = 1048560 / odds;
				
				if (gLastUsedItem == ITEM_MASTER_BALL)
					shakes = BALL_3_SHAKES_SUCCESS;
				else
					for (shakes = 0; shakes < 4 && Random() < odds; ++shakes);
				
                BtlController_EmitBallThrowAnim(gBattlerAttacker, BUFFER_A, shakes);
                MarkBattlerForControllerExec(gBattlerAttacker);
				
				if (shakes == BALL_3_SHAKES_SUCCESS) // mon caught
					DoMonCaughtEffects();
                else // not caught
                {
					gBattleStruct->lastFailedBallThrow = gLastUsedItem;
                    gBattleCommunication[MULTISTRING_CHOOSER] = shakes;
                    gBattlescriptCurrInstr = BattleScript_ShakeBallThrow;
                }
            }
        }
    }
}

static void atkF0_givecaughtmon(void)
{
	CMD_ARGS();

	u8 battler = GetCatchingBattler();
	
	DoSpecialFormChange(battler, gBattlerPartyIndexes[battler], FORM_CHANGE_END_BATTLE);
	
    if (GiveMonToPlayer(&gEnemyParty[gBattlerPartyIndexes[battler]]) != MON_GIVEN_TO_PARTY)
    {
		bool8 isBillsPc = FlagGet(FLAG_SYS_NOT_SOMEONES_PC);
		
        if (!ShouldShowBoxWasFullMessage())
            gBattleCommunication[MULTISTRING_CHOOSER] = isBillsPc ? B_MSG_TRANSFERRED_TO_BILL_PC : B_MSG_TRANSFERRED_TO_SOMEONE_PC;
        else
        {
			gBattleCommunication[MULTISTRING_CHOOSER] = isBillsPc ? B_MSG_BILL_PC_BOX_FULL : B_MSG_SOMEONE_PC_BOX_FULL;
            StringCopy(gStringVar3, GetBoxNamePtr(GetPCBoxToSendMon())); //box the mon was going to be sent to
        }
		StringCopy(gStringVar1, GetBoxNamePtr(VarGet(VAR_PC_BOX_TO_SEND_MON))); // box the mon was sent to
		GetMonData(&gEnemyParty[gBattlerPartyIndexes[battler]], MON_DATA_NICKNAME, gStringVar2);
    }
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkF1_trysetcaughtmondexflags(void)
{
	CMD_ARGS(const u8 *ptr);

	u8 battler = GetCatchingBattler();
    u16 natDexNum = SpeciesToNationalPokedexNum(GetMonData(&gEnemyParty[gBattlerPartyIndexes[battler]], MON_DATA_SPECIES));

    if (GetSetPokedexFlag(natDexNum, FLAG_GET_CAUGHT))
        gBattlescriptCurrInstr = cmd->ptr;
    else
    {
        HandleSetPokedexFlag(natDexNum, FLAG_SET_CAUGHT, GetMonData(&gEnemyParty[gBattlerPartyIndexes[battler]], MON_DATA_PERSONALITY));
	    
		if (!FlagGet(FLAG_SYS_POKEDEX_GET)) // don't show dex entry if don't have the pokedex
			gBattlescriptCurrInstr = cmd->ptr;
		else
			gBattlescriptCurrInstr = cmd->nextInstr;
    }
}

static void atkF2_displaydexinfo(void)
{
	CMD_ARGS();

	u8 battler = GetCatchingBattler();

    switch (gBattleCommunication[MULTIUSE_STATE])
    {
		case 0:
			BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_WHITE);
			++gBattleCommunication[MULTIUSE_STATE];
			break;
		case 1:
			if (!gPaletteFade.active)
			{
				FreeAllWindowBuffers();
				gBattleCommunication[TASK_ID] = DexScreen_RegisterMonToPokedex(GetMonData(&gEnemyParty[gBattlerPartyIndexes[battler]], MON_DATA_SPECIES));
				++gBattleCommunication[MULTIUSE_STATE];
			}
			break;
		case 2:
			if (!gPaletteFade.active && gMain.callback2 == BattleMainCB2 && !gTasks[gBattleCommunication[TASK_ID]].isActive)
			{
				CpuFill32(0, (void *)VRAM, VRAM_SIZE);
				SetVBlankCallback(VBlankCB_Battle);
				++gBattleCommunication[MULTIUSE_STATE];
			}
			break;
		case 3:
			InitBattleBgsVideo();
			LoadBattleTextboxAndBackground();
			gBattle_BG3_X = 0x100;
			++gBattleCommunication[MULTIUSE_STATE];
			break;
		case 4:
			if (!IsDma3ManagerBusyWithBgCopy())
			{
				CreateMonPicSprite(GetMonData(&gEnemyParty[gBattlerPartyIndexes[battler]], MON_DATA_SPECIES), GetMonData(&gEnemyParty[gBattlerPartyIndexes[battler]], MON_DATA_IS_SHINY), gBattleMons[battler].personality, TRUE, 120, 64, 0, 0xFFFF);
				CpuFill32(0, gPlttBufferFaded, BG_PLTT_SIZE);
				BeginNormalPaletteFade(0x1FFFF, 0, 0x10, 0, RGB_BLACK);
				ShowBg(0);
				ShowBg(3);
				++gBattleCommunication[MULTIUSE_STATE];
			}
			break;
		case 5:
			if (!gPaletteFade.active)
				gBattlescriptCurrInstr = cmd->nextInstr;
			break;
    }
}

void HandleBattleWindow(u8 xStart, u8 yStart, u8 xEnd, u8 yEnd, u8 flags)
{
    u16 var = 0;
    s32 destY, destX;

    for (destY = yStart; destY <= yEnd; ++destY)
    {
        for (destX = xStart; destX <= xEnd; ++destX)
        {
            if (destY == yStart)
            {
                if (destX == xStart)
                    var = 0x1022;
                else if (destX == xEnd)
                    var = 0x1024;
                else
                    var = 0x1023;
            }
            else if (destY == yEnd)
            {
                if (destX == xStart)
                    var = 0x1028;
                else if (destX == xEnd)
                    var = 0x102A;
                else
                    var = 0x1029;
            }
            else
            {
                if (destX == xStart)
                    var = 0x1025;
                else if (destX == xEnd)
                    var = 0x1027;
                else
                    var = 0x1026;
            }
            if (flags & WINDOW_CLEAR)
                var = 0;
			
            if (flags & WINDOW_x80)
                CopyToBgTilemapBufferRect_ChangePalette(1, &var, destX, destY, 1, 1, 0x11);
            else
                CopyToBgTilemapBufferRect_ChangePalette(0, &var, destX, destY, 1, 1, 0x11);
        }
    }
    CopyBgTilemapBufferToVram(1);
}

void BattleCreateYesNoCursorAt(void)
{
    u16 src[2] = {1, 2};
    CopyToBgTilemapBufferRect_ChangePalette(0, src, 0x18, 9 + (2 * gBattleCommunication[CURSOR_POSITION]), 1, 2, 0x11);
    CopyBgTilemapBufferToVram(0);
}

void BattleDestroyYesNoCursorAt(void)
{
    u16 src[2] = {32, 32};
    CopyToBgTilemapBufferRect_ChangePalette(0, src, 0x18, 9 + (2 * gBattleCommunication[CURSOR_POSITION]), 1, 2, 0x11);
    CopyBgTilemapBufferToVram(0);
}

static void atkF3_trygivecaughtmonnick(void)
{
	CMD_ARGS(const u8 *ptr);

	if (gSaveBlock2Ptr->optionsSkipPkmnNickname)
		gBattleCommunication[MULTIUSE_STATE] = 4;
	
    switch (gBattleCommunication[MULTIUSE_STATE])
    {
		case 0:
			HandleBattleWindow(0x17, 8, 0x1D, 0xD, 0);
			BattlePutTextOnWindow(gText_BattleYesNoChoice, B_WIN_YESNO);
			gBattleCommunication[CURSOR_POSITION] = 0;
			BattleCreateYesNoCursorAt();
			++gBattleCommunication[MULTIUSE_STATE];
			break;
		case 1:
			if (JOY_NEW(DPAD_UP) && gBattleCommunication[CURSOR_POSITION])
			{
				PlaySE(SE_SELECT);
				BattleDestroyYesNoCursorAt();
				gBattleCommunication[CURSOR_POSITION] = 0;
				BattleCreateYesNoCursorAt();
			}
			if (JOY_NEW(DPAD_DOWN) && gBattleCommunication[CURSOR_POSITION] == 0)
			{
				PlaySE(SE_SELECT);
				BattleDestroyYesNoCursorAt();
				gBattleCommunication[CURSOR_POSITION] = 1;
				BattleCreateYesNoCursorAt();
			}
			if (JOY_NEW(A_BUTTON))
			{
				PlaySE(SE_SELECT);
				
				if (gBattleCommunication[CURSOR_POSITION] == 0)
				{
					++gBattleCommunication[MULTIUSE_STATE];
					BeginFastPaletteFade(FAST_FADE_OUT_TO_BLACK);
				}
				else
					gBattleCommunication[MULTIUSE_STATE] = 4;
			}
			else if (JOY_NEW(B_BUTTON))
			{
				PlaySE(SE_SELECT);
				gBattleCommunication[MULTIUSE_STATE] = 4;
			}
			break;
		case 2:
			if (!gPaletteFade.active)
			{
				u8 battler = GetCatchingBattler();
				
				GetMonData(&gEnemyParty[gBattlerPartyIndexes[battler]], MON_DATA_NICKNAME, gBattleStruct->caughtMonNick);
				FreeAllWindowBuffers();
				DoNamingScreen(NAMING_SCREEN_CAUGHT_MON, gBattleStruct->caughtMonNick, GetMonData(&gEnemyParty[gBattlerPartyIndexes[battler]], MON_DATA_SPECIES),
                               GetMonGender(&gEnemyParty[gBattlerPartyIndexes[battler]]), BattleMainCB2);
				++gBattleCommunication[MULTIUSE_STATE];
			}
			break;
		case 3:
			if (gMain.callback2 == BattleMainCB2 && !gPaletteFade.active)
			{
				SetMonData(&gEnemyParty[gBattlerPartyIndexes[GetCatchingBattler()]], MON_DATA_NICKNAME, gBattleStruct->caughtMonNick);
				gBattlescriptCurrInstr = cmd->ptr;
			}
			break;
		case 4:
			if (CalculatePlayerPartyCount() == PARTY_SIZE)
				gBattlescriptCurrInstr = cmd->nextInstr;
			else
				gBattlescriptCurrInstr = cmd->ptr;
			
			BattleScriptCall(BattleScript_Pausex40);
			break;
    }
}

static void atkF4_nop(void)
{
	CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkF5_nop(void)
{
    CMD_ARGS();
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkF6_finishaction(void)
{
	CMD_ARGS();
    gCurrentActionFuncId = B_ACTION_FINISHED;
}

static void atkF7_finishturn(void)
{
	CMD_ARGS();
    gCurrentActionFuncId = B_ACTION_FINISHED;
    gCurrentTurnActionNumber = gBattlersCount;
}

static void atkF8_callfunction(void)
{
	CMD_ARGS(void (*func)(void));
    void (*func)(void) = cmd->func;
    func();
}

static void atkF9_cureprimarystatus(void)
{
	CMD_ARGS(u8 battler, const u8 *ptr);

	if (!gBattleControllerExecFlags)
	{
		u8 battlerId = GetBattlerForBattleScript(cmd->battler);
		
		if (gBattleMons[battlerId].status1.id) 
		{
			PrepareMonNickBuffer(gBattleTextBuff2, battlerId, gBattlerPartyIndexes[battlerId]);
			ClearBattlerStatus(battlerId);
			gBattlescriptCurrInstr = cmd->nextInstr;
		}
		else
			gBattlescriptCurrInstr = cmd->ptr;
	}
}

static void atkFA_setword(void)
{
	CMD_ARGS(u32 *memPtr, u32 value);
    *cmd->memPtr = cmd->value;
    gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkFB_jumpifsubstituteblocks(void)
{
	CMD_ARGS(const u8 *ptr);

	if (SubsBlockMove(gBattlerAttacker, gBattlerTarget, gCurrentMove)) 
		gBattlescriptCurrInstr = cmd->ptr;
	else
		gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkFC_handleabilitypopup(void)
{
	CMD_ARGS(u8 battler);

	if (!gBattleControllerExecFlags)
	{
		if (!gBattleScripting.bypassAbilityPopUp)
		{
			u8 byte = cmd->battler;
			u8 battlerId = GetBattlerForBattleScript(byte & ~(ATKFC_REMOVE_POP_UP | ATKFC_UPDATE_POP_UP));
			u16 animId;
			
			if (byte & ATKFC_REMOVE_POP_UP)
				animId = B_ANIM_REMOVE_ABILITY_POP_UP;
			else if (byte & ATKFC_UPDATE_POP_UP)
				animId = B_ANIM_UPDATE_ABILITY_POP_UP;
			else
			{
				if (gBattleScripting.fixedAbilityPopUp && (gActiveAbilityPopUps & gBitTable[battlerId]))
				{
					gBattlescriptCurrInstr = cmd->nextInstr;
					return;
				}
				animId = B_ANIM_LOAD_ABILITY_POP_UP;
			}
			BtlController_EmitBattleAnimation(battlerId, BUFFER_A, animId, 0);
			MarkBattlerForControllerExec(battlerId);
		}
		gBattlescriptCurrInstr = cmd->nextInstr;
	}
}

static void atkFD_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkFE_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

static void atkFF_nop(void)
{
	CMD_ARGS();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

// Callfunction funcs
void BS_SetMinimize(void)
{
	NATIVE_ARGS();

	if (gHitMarker & HITMARKER_OBEYS)
        gStatuses3[gBattlerAttacker] |= STATUS3_MINIMIZED;
	
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_MoveValuesCleanUp(void)
{
	NATIVE_ARGS();
	MoveValuesCleanUp();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_ItemIncreaseStat(void)
{
	NATIVE_ARGS();
	SetStatChanger(ItemId_GetHoldEffectParam(gLastUsedItem), GetItemStatChangeStages(gLastUsedItem));
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_IncrementGameStat(void)
{
	NATIVE_ARGS(u8 gameStatId);
	if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
        IncrementGameStat(cmd->gameStatId);
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_SwitchSpriteIgnore0HpBit(void)
{
	NATIVE_ARGS();
	gBattleStruct->spriteIgnore0Hp ^= TRUE;
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_SwitchPursuitDamageBit(void)
{
	NATIVE_ARGS();
	gBattleStruct->pursuitSwitchDmg ^= TRUE;
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_AtkNameInBuff1(void)
{
	NATIVE_ARGS();
	PrepareMonNickBuffer(gBattleTextBuff1, gBattlerAttacker, gBattlerPartyIndexes[gBattlerAttacker]);
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_ResetSentMonsValue(void)
{
	NATIVE_ARGS();
	ResetSentPokesToOpponentValue();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_SetAtkToPlayer0(void)
{
	NATIVE_ARGS();
	gBattlerAttacker = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_BufferMoveToLearn(void)
{
	NATIVE_ARGS();
	BufferMoveToLearnIntoBattleTextBuff2();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_TrySetDestinyBondToHappen(void)
{
	NATIVE_ARGS();
	TrySetDestinyBondToHappen();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_SetDefenseCurl(void)
{
	NATIVE_ARGS();
	gBattleMons[gBattlerAttacker].status2 |= STATUS2_DEFENSE_CURL;
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_SetNightmare(void)
{
	NATIVE_ARGS();
	gBattleMons[gBattlerTarget].status2 |= STATUS2_NIGHTMARE;
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_NormaliseAllBuffs(void)
{
	NATIVE_ARGS();

	u8 i;

    for (i = 0; i < gBattlersCount; ++i)
		TryResetBattlerStatChanges(i);
	
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_SetChargingTurn(void)
{
	NATIVE_ARGS();

	gHitMarker |= HITMARKER_CHARGING;
	
	if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS))
	{
		gBattleMons[gBattlerAttacker].status2 |= STATUS2_MULTIPLETURNS;
		gBattleStruct->battlers[gBattlerAttacker].lockedMove = gCurrentMove;
		gProtectStructs[gBattlerAttacker].chargingTurn = TRUE;
	}
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_ClearChargingTurn(void)
{
	NATIVE_ARGS();
	gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_MULTIPLETURNS);
	gProtectStructs[gBattlerAttacker].chargingTurn = FALSE;
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_SetRage(void)
{
	NATIVE_ARGS();
	gBattleMons[gBattlerAttacker].status2 |= STATUS2_RAGE;
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_SetEscapePrevention(void)
{
	NATIVE_ARGS();
	gBattleMons[gBattlerTarget].status2 |= STATUS2_ESCAPE_PREVENTION;
	gDisableStructs[gBattlerTarget].battlerPreventingEscape = gBattlerAttacker;
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_TrySetForesight(void)
{
	NATIVE_ARGS(const u8 *failPtr);

	if (gBattleMons[gBattlerTarget].status2 & STATUS2_FORESIGHT)
		gBattlescriptCurrInstr = cmd->failPtr;
	else
	{
		gBattleMons[gBattlerTarget].status2 |= STATUS2_FORESIGHT;
		gBattlescriptCurrInstr = cmd->nextInstr;
	}
}

void BS_SetForcedTarget(void)
{
	NATIVE_ARGS();
	u8 side = GetBattlerSide(gBattlerAttacker);
	gSideTimers[side].followmeSet = TRUE;
    gSideTimers[side].followmeTarget = gBattlerAttacker;
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_CallTerrainAttack(void)
{
	NATIVE_ARGS();
	PrepareMoveBuffer(gBattleTextBuff1, gCurrentMove);
	CallAnotherMove(gBattleTerrainTable[gBattleTerrain].naturePowerMove);
	BattleScriptCall(BattleScript_NaturePowerString);
}

void BS_GetTwoTurnsMoveString(void)
{
	NATIVE_ARGS();
	gBattleCommunication[MULTISTRING_CHOOSER] = gBattleMoves[gCurrentMove].argument.twoTurns.stringId;
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_SetMagicCoatTarget(void)
{
	NATIVE_ARGS();

	u8 side, attacker = gBattlerAttacker;
	
	gBattlerAttacker = gBattlerTarget;
	
	side = GetBattlerSide(BATTLE_OPPOSITE(gBattlerAttacker));
	
	if (IsBattlerAffectedByFollowMe(gBattlerAttacker, side, gCurrentMove))
		gBattlerTarget = gSideTimers[side].followmeTarget;
	else
		gBattlerTarget = attacker;
	
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_SetFocusEnergy(void)
{
	NATIVE_ARGS();
	gBattleMons[gBattlerAttacker].status2 |= STATUS2_FOCUS_ENERGY;
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_CancelAllActions(void)
{
	NATIVE_ARGS();

	u8 i;

    for (i = 0; i < gBattlersCount; ++i)
        gActionsByTurnOrder[i] = B_ACTION_CANCEL_PARTNER;
	
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_SetBide(void)
{
	NATIVE_ARGS();
	gHitMarker |= HITMARKER_CHARGING;
    gBattleMons[gBattlerAttacker].status2 |= (STATUS2_MULTIPLETURNS | STATUS2_BIDE_TURN(2));
    gBattleStruct->battlers[gBattlerAttacker].lockedMove = gCurrentMove;
    gBattleStruct->battlers[gBattlerAttacker].bideTakenDamage = 0;
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_SetMist(void)
{
	NATIVE_ARGS();
	u8 side = GetBattlerSide(gBattlerAttacker);
	gSideStatuses[side] |= SIDE_STATUS_MIST;
	gSideTimers[side].mistTimer = 5;
	gSideTimers[side].mistBattlerId = gBattlerAttacker;
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_Metronome(void)
{
	NATIVE_ARGS();

    while (TRUE)
    {
        u16 move = RandomRange(MOVE_NONE + 1, METRONOME_MOVES_COUNT);
	    
        if (!gBattleMoves[move].flags.forbiddenMetronome)
		{
			CallAnotherMove(move);
            return;
        }
    }
}

void BS_PsywaveDamageEffect(void)
{
	NATIVE_ARGS();

	u32 rand = RandomMax(101) + 50;
	
	gBattleMoveDamage = (gBattleMons[gBattlerAttacker].level * rand) / 100;
    if (gBattleMoveDamage == 0)
		gBattleMoveDamage = 1;
	
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_CopyFoeStats(void)
{
	NATIVE_ARGS();
	gBattleScripting.battler = gBattlerAttacker;
	CopyBattlerStatChanges(gBattlerAttacker, gBattlerTarget);
	CopyBattlerCritModifier(gBattlerAttacker, gBattlerTarget);
	PrepareMonNickWithPrefixBuffer(gBattleTextBuff1, gBattlerTarget, gBattlerPartyIndexes[gBattlerTarget]);
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_SetCharge(void)
{
	NATIVE_ARGS(u8 battler);
	gStatuses3[GetBattlerForBattleScript(cmd->battler)] |= STATUS3_CHARGED_UP;
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_RemoveScreens(void)
{
	NATIVE_ARGS();

	if (TryRemoveScreens(gBattlerTarget, FALSE))
    {
        gBattleScripting.animTurn = 1;
        gBattleScripting.animTargetsHit = 1;
    }
    else
    {
        gBattleScripting.animTurn = 0;
        gBattleScripting.animTargetsHit = 0;
    }
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_EndSelectionScript(void)
{
	NATIVE_ARGS();
	gBattleStruct->battlers[gBattlerAttacker].selectionScriptFinished = TRUE;
	// No script incremment
}

void BS_SnatchSetBattlers(void)
{
	NATIVE_ARGS();

	u8 attacker = gBattlerAttacker;
	
    if (gBattlerAttacker == gBattlerTarget)
        gBattlerAttacker = gBattlerTarget = gBattleScripting.battler;
    else
        gBattlerTarget = gBattleScripting.battler;
	
    gBattleScripting.battler = attacker;
	
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_TryRestoreMirrorArmorOriginalAttacker(void)
{
	NATIVE_ARGS();

	u8 temp;
	
	if (gBattleStruct->statChange.mirrorArmorState == 2) // Restore original attacker
		SWAP(gBattlerAttacker, gBattlerTarget, temp);
		
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_ArgumentToMoveDamage(void)
{
	NATIVE_ARGS();
	gBattleMoveDamage = gBattleMoves[gCurrentMove].argument.fixedDamage.amount;
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_SetStatChangerFromMultiuseState(void)
{
	NATIVE_ARGS(s8 buff);
	SetStatChanger(gBattleCommunication[MULTIUSE_STATE], cmd->buff);
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_SetTargetAlly(void)
{
	NATIVE_ARGS();
	gBattlerTarget = BATTLE_PARTNER(gBattlerAttacker);
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_SetHelpingHand(void)
{
	NATIVE_ARGS();
	++gProtectStructs[gBattlerTarget].helpingHandUses;
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_TransformDataExecution(void)
{
	NATIVE_ARGS();

	gChosenMove = MOVE_UNAVAILABLE;
	
    if (TryTransformIntoBattler(gBattlerAttacker, gBattlerTarget))
		gBattlescriptCurrInstr = cmd->nextInstr;
	else
		gBattlescriptCurrInstr = BattleScript_ButItFailed;
}

void BS_SetRoost(void)
{
	NATIVE_ARGS();
	gDisableStructs[gBattlerAttacker].roostActive = TRUE;
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_TrySetMiracleEye(void)
{
	NATIVE_ARGS(const u8 *ptr);

	if (gBattleMons[gBattlerTarget].status2 & STATUS2_MIRACLE_EYE)
		gBattlescriptCurrInstr = cmd->ptr;
	else
	{
		gBattleMons[gBattlerTarget].status2 |= STATUS2_MIRACLE_EYE;
		gBattlescriptCurrInstr = cmd->nextInstr;
	}
}

void BS_SetHealingWish(void)
{
	NATIVE_ARGS();

	switch (gCurrentMove)
	{
		case MOVE_LUNAR_DANCE:
		    gBattleStruct->battlers[gBattlerAttacker].storedLunarDance = TRUE;
			break;
		default:
		    gBattleStruct->battlers[gBattlerAttacker].storedHealingWish = TRUE;
			break;
	}
	gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP;
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_TrySetTailwind(void)
{
	NATIVE_ARGS(const u8 *ptr);

	u8 side = GetBattlerSide(gBattlerAttacker);
	
	if (gSideStatuses[side] & SIDE_STATUS_TAILWIND)
		gBattlescriptCurrInstr = cmd->ptr;
	else
	{
		gSideStatuses[side] |= SIDE_STATUS_TAILWIND;
		gSideTimers[side].tailwindTimer = 4;
		gSideTimers[side].tailwindBattlerId = gBattlerAttacker;
		gBattlescriptCurrInstr = cmd->nextInstr;
	}
}

void BS_TrySetEmbargo(void)
{
	NATIVE_ARGS(const u8 *ptr);

	if (gStatuses3[gBattlerTarget] & STATUS3_EMBARGO)
		gBattlescriptCurrInstr = cmd->ptr;
	else
	{
		gStatuses3[gBattlerTarget] |= STATUS3_EMBARGO;
		gDisableStructs[gBattlerTarget].embargoTimer = 5;
		gBattlescriptCurrInstr = cmd->nextInstr;
	}
}

void BS_TrySetHealBlock(void)
{
	NATIVE_ARGS(const u8 *ptr);

	if (gStatuses3[gBattlerTarget] & STATUS3_HEAL_BLOCK)
		gBattlescriptCurrInstr = cmd->ptr;
	else
	{
		gStatuses3[gBattlerTarget] |= STATUS3_HEAL_BLOCK;
		gDisableStructs[gBattlerTarget].healBlockTimer = 5;
		gBattlescriptCurrInstr = cmd->nextInstr;
	}
}

void BS_TrySetLuckyChant(void)
{
	NATIVE_ARGS(const u8 *ptr);

	u8 side = GetBattlerSide(gBattlerAttacker);
	
	if (gSideStatuses[side] & SIDE_STATUS_LUCKY_CHANT)
		gBattlescriptCurrInstr = cmd->ptr;
	else
	{
		gSideStatuses[side] |= SIDE_STATUS_LUCKY_CHANT;
		gSideTimers[side].luckyChantTimer = 5;
		gSideTimers[side].luckyChantBattlerId = gBattlerAttacker;
		gBattlescriptCurrInstr = cmd->nextInstr;
	}
}

void BS_SaveAttackerOnStack(void)
{
	NATIVE_ARGS();
	SaveAttackerToStack(gBattlerAttacker);
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_RestoreAttackerFromStack(void)
{
	NATIVE_ARGS();
	RestoreAttackerFromStack();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_SaveTargetOnStack(void)
{
	NATIVE_ARGS();
	SaveTargetToStack(gBattlerTarget);
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_RestoreTargetFromStack(void)
{
	NATIVE_ARGS();
	RestoreTargetFromStack();
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_SwitchOutPartyOrderSwap(void)
{
	NATIVE_ARGS();

	u16 temp;
	
	if (!gSpecialStatuses[gBattlerAttacker].removedNeutralizingGas) // From switch out
		SWAP(gBattlerPartyIndexes[gBattlerAttacker], gBattleStruct->battlers[gBattlerAttacker].partyIndex, temp);
	
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_TryCopycat(void)
{
	NATIVE_ARGS(const u8 *ptr);

	if (!gLastUsedMove || gLastUsedMove == MOVE_UNAVAILABLE || gBattleMoves[gLastUsedMove].flags.forbiddenCopycat || IsZMove(gLastUsedMove))
		gBattlescriptCurrInstr = cmd->ptr;
	else
	{
		gCalledMove = gLastUsedMove;
		gBattlescriptCurrInstr = cmd->nextInstr;
	}
}

void BS_TryLastResort(void)
{
	NATIVE_ARGS(const u8 *ptr);

	if (!CanUseLastResort(gBattlerAttacker))
		gBattlescriptCurrInstr = cmd->ptr;
	else
		gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_TrySetAbility(void)
{
	NATIVE_ARGS(const u8 *ptr);

	u16 newAbility = gBattleMoves[gCurrentMove].argument.setAbility.abilityId;
	
	if (gBattleMons[gBattlerTarget].ability == newAbility || gAbilities[gBattleMons[gBattlerTarget].ability].cantBeOverwritten)
		gBattlescriptCurrInstr = cmd->ptr;
	else
	{
		gBattleStruct->battlers[gBattlerTarget].abilityOverride = SetBattlerAbility(gBattlerTarget, newAbility);
		gBattlescriptCurrInstr = cmd->nextInstr;
	}
}

void BS_TrySuckerPunch(void)
{
	NATIVE_ARGS(const u8 *ptr);

	u16 move = gBattleStruct->battlers[gBattlerTarget].chosenMove;
	
	if (GetBattlerTurnOrderNum(gBattlerTarget) < gCurrentTurnActionNumber || !move || (IS_MOVE_STATUS(move) && move != MOVE_ME_FIRST))
		gBattlescriptCurrInstr = cmd->ptr;
	else
		gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_TrySetToxicSpikes(void)
{
	NATIVE_ARGS(const u8 *ptr);

	if (!TrySetToxicSpikesOnBattlerSide(BATTLE_OPPOSITE(gBattlerAttacker)))
		gBattlescriptCurrInstr = cmd->ptr;
	else
		gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_GetFutureAttackAnim(void)
{
	NATIVE_ARGS();
	gBattleScripting.animArg1 = gBattleMoves[gCurrentMove].argument.futureAttack.animationId;
	gBattleScripting.animArg2 = gBattlerTarget;
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_UpdateRecoilEvolutionTracker(void)
{
	NATIVE_ARGS();
	TryUpdateEvolutionTracker(EVO_REQ_RECOIL_DAMAGE, gBattleMoveDamage, 0);
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_IncrementTripleKickPower(void)
{
	NATIVE_ARGS();
	gBattleScripting.tripleKickPower += gBattleMoves[gCurrentMove].argument.tripleKick.increment;
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_JumpIfAdditionalEffectsLoops(void)
{
	NATIVE_ARGS(const u8 *ptr);
	
	if (gBattleStruct->moveEffect.continueAdditionalEffectsLoop)
		gBattlescriptCurrInstr = cmd->ptr;
	else
		gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_JumpIfSosOpponentFainted(void)
{
	NATIVE_ARGS(u8 battler, const u8 *ptr);
	
	u8 battlerId = GetBattlerForBattleScript(cmd->battler);
	
	if (IsBattlerTotemPokemon(battlerId))
		gBattleStruct->sos.totemBattlerId = MAX_BATTLERS_COUNT;
	
	if ((gBattleTypeFlags & BATTLE_TYPE_SOS) && GetBattlerSide(battlerId) == B_SIDE_OPPONENT)
		gBattlescriptCurrInstr = cmd->ptr;
	else
		gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_SosCallForAlly(void)
{
	NATIVE_ARGS();
	
	switch (gBattleCommunication[MULTIUSE_STATE])
	{
		case 0:
			BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
			SetMainCallback2(CB2_SosCall);
			gBattleCommunication[MULTIUSE_STATE]++;
			break;
		case 1:
			if (gMain.callback2 == BattleMainCB2 && !gPaletteFade.active)
				gBattleCommunication[MULTIUSE_STATE]++;
			break;
		case 2:
			gBattlescriptCurrInstr = cmd->nextInstr;
			break;
	}
}

void BS_PlayBattlerCry(void)
{
	NATIVE_ARGS(u8 battler);
	u8 battlerId = GetBattlerForBattleScript(cmd->battler);
	PlayCry_Normal(gBattleMons[battlerId].species, GetBattlerSide(battlerId) == B_SIDE_PLAYER ? -25 : 25);
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_TrySetAdrenalineOrbEffect(void)
{
	NATIVE_ARGS();
	
	if (gBattleStruct->sos.usedAdrenalineOrb)
	{
		AddBagItem(gLastUsedItem, 1); // It's not consumed if failed
		gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_BUT_HAD_NO_EFFECT;
	}
	else
	{
		gBattleStruct->sos.usedAdrenalineOrb = TRUE;
		gBattleCommunication[MULTISTRING_CHOOSER] = B_MSG_MAKES_WILD_NERVOUS;
	}
	gBattlescriptCurrInstr = cmd->nextInstr;
}

void BS_TryTeleport(void)
{
	NATIVE_ARGS(u8 battler);
	
	u8 battlerId = GetBattlerForBattleScript(cmd->battler);
			
	// It fails in sos battles if there's more than one opponent.
	if (gBattleTypeFlags & BATTLE_TYPE_SOS)
	{
		if (IS_WHOLE_SIDE_ALIVE(battlerId))
		{
			gBattlescriptCurrInstr = BattleScript_ButItFailed;
			return;
		}
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed;
		return;
	}
	
	switch (IsRunningFromBattleImpossible(battlerId, FALSE))
	{
		case BATTLE_RUN_SUCCESS:
			break;
		case BATTLE_RUN_FORBIDDEN:
			gBattlescriptCurrInstr = BattleScript_ButItFailed;
			return;
		case BATTLE_RUN_FAILURE:
			gBattlescriptCurrInstr = BattleScript_PrintAbilityMadeIneffective;
			return;
	}
	gBattlescriptCurrInstr = cmd->nextInstr;
}

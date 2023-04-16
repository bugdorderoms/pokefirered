#include "global.h"
#include "gflib.h"
#include "item.h"
#include "util.h"
#include "random.h"
#include "pokedex.h"
#include "money.h"
#include "pokemon_icon.h"
#include "mail.h"
#include "event_data.h"
#include "strings.h"
#include "pokemon_special_anim.h"
#include "pokemon_storage_system.h"
#include "pokemon_summary_screen.h"
#include "task.h"
#include "naming_screen.h"
#include "overworld.h"
#include "form_change.h"
#include "party_menu.h"
#include "trainer_pokemon_sprites.h"
#include "field_specials.h"
#include "battle.h"
#include "pokemon.h"
#include "battle_message.h"
#include "battle_anim.h"
#include "battle_main.h"
#include "battle_ai_script_commands.h"
#include "calculate_base_damage.h"
#include "battle_scripts.h"
#include "trainer_slide.h"
#include "battle_string_ids.h"
#include "reshow_battle_screen.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "constants/battle_anim.h"
#include "constants/battle_move_effects.h"
#include "constants/battle_script_commands.h"
#include "constants/items.h"
#include "constants/hold_effects.h"
#include "constants/songs.h"
#include "constants/moves.h"
#include "constants/abilities.h"
#include "constants/pokemon.h"
#include "constants/maps.h"
#include "constants/inserts.h"
#include "constants/battle_move_effects.h"

#define DEFENDER_IS_PROTECTED ((gProtectStructs[gBattlerTarget].protected) && (gBattleMoves[gCurrentMove].flags & FLAG_PROTECT_AFFECTED))

extern const u8 *const gBattleScriptsForMoveEffects[];

//used strings
static const u8 sAnticipationString[] = _("{B_ATK_NAME_WITH_PREFIX} shuddered!");
static const u8 sDownloadString[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY}\nraised its {B_BUFF1}!");
static const u8 sForewarnString[] = _("It was alerted to\n{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1}!");
static const u8 sFriskString[] = _("{B_ATK_NAME_WITH_PREFIX} frisked {B_DEF_NAME_WITH_PREFIX}\nand found its {B_LAST_ITEM}!");
static const u8 sBellyDrumContraryString[] = _("{B_ATK_NAME_WITH_PREFIX} cut its own HP\nand minimized Attack!");
static const u8 sMagicBounceString[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_CURRENT_MOVE} was\nbounced back by {B_DEF_NAME_WITH_PREFIX}'s\l{B_DEF_ABILITY}!");
static const u8 sStanceChangeToShieldString[] = _("Changed to Shield Forme!");
static const u8 sStanceChangeToBladeString[] = _("Changed to Blade Forme!");
static const u8 sFizzlesFireTypeString[] = _("The Fire-type attack fizzled out\nin the heavy rain!");
static const u8 sEvaporateWaterTypeString[] = _("The Water-type attack evaporated\nin the harsh sunlight!");

static bool8 IsTwoTurnsMove(u16 move);
static void TrySetDestinyBondToHappen(void);
static u8 AttacksThisTurn(u8 battlerId, u16 move); // Note: returns 1 if it's a charging turn, otherwise 2.
static void CheckWonderGuardAndLevitate(void);
static u8 ChangeStatBuffs(s8 statValue, u8 statId, u8, const u8 *BS_ptr);
static void sub_8026480(void);
static bool8 sub_80264D0(void);
static void DrawLevelUpWindow1(void);
static void DrawLevelUpWindow2(void);
static bool8 sub_8026648(void);
static void PutMonIconOnLvlUpBox(void);
static void PutLevelAndGenderOnLvlUpBox(void);
static bool8 AnticipationTypeCalc(u8 battler);
static u16 GetForewarnMovePower(u16 move);
static bool8 HasAttackerFaintedTarget(void);

static void SpriteCB_MonIconOnLvlUpBox(struct Sprite *sprite);

static void atk00_attackcanceler(void);
static void atk01_accuracycheck(void);
static void atk02_attackstring(void);
static void atk03_ppreduce(void);
static void atk04_critcalc(void);
static void atk05_damagecalc(void);
static void atk06_typecalc(void);
static void atk07_adjustdamage(void);
static void atk08_nop(void);
static void atk09_attackanimation(void);
static void atk0A_waitanimation(void);
static void atk0B_healthbarupdate(void);
static void atk0C_datahpupdate(void);
static void atk0D_critmessage(void);
static void atk0E_effectivenesssound(void);
static void atk0F_resultmessage(void);
static void atk10_printstring(void);
static void atk11_printselectionstring(void);
static void atk12_waitmessage(void);
static void atk13_printfromtable(void);
static void atk14_printselectionstringfromtable(void);
static void atk15_seteffectwithchance(void);
static void atk16_seteffectprimary(void);
static void atk17_seteffectsecondary(void);
static void atk18_clearstatusfromeffect(void);
static void atk19_tryfaintmon(void);
static void atk1A_dofaintanimation(void);
static void atk1B_cleareffectsonfaint(void);
static void atk1C_jumpifstatus(void);
static void atk1D_jumpifstatus2(void);
static void atk1E_jumpifability(void);
static void atk1F_jumpifsideaffecting(void);
static void atk20_jumpifstat(void);
static void atk21_jumpifstatus3condition(void);
static void atk22_jumpbasedontype(void);
static void atk23_getexp(void);
static void atk24(void);
static void atk25_movevaluescleanup(void);
static void atk26_setmultihit(void);
static void atk27_decrementmultihit(void);
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
static void atk32_copyarraywithindex(void);
static void atk33_orbyte(void);
static void atk34_orhalfword(void);
static void atk35_orword(void);
static void atk36_bicbyte(void);
static void atk37_bichalfword(void);
static void atk38_bicword(void);
static void atk39_pause(void);
static void atk3A_waitstate(void);
static void atk3B_healthbar_update(void);
static void atk3C_return(void);
static void atk3D_end(void);
static void atk3E_end2(void);
static void atk3F_end3(void);
static void atk40_jumpifaffectedbyprotect(void);
static void atk41_call(void);
static void atk42_trysetsleep(void);
static void atk43_jumpifabilitypresent(void);
static void atk44_endselectionscript(void);
static void atk45_playanimation(void);
static void atk46_playanimation2(void);
static void atk47_setgraphicalstatchangevalues(void);
static void atk48_playstatchangeanimation(void);
static void atk49_moveend(void);
static void atk4A_nop(void);
static void atk4B_returnatktoball(void);
static void atk4C_getswitchedmondata(void);
static void atk4D_switchindataupdate(void);
static void atk4E_switchinanim(void);
static void atk4F_jumpifcantswitch(void);
static void atk50_openpartyscreen(void);
static void atk51_switchhandleorder(void);
static void atk52_switchineffects(void);
static void atk53_trainerslide(void);
static void atk54_playse(void);
static void atk55_fanfare(void);
static void atk56_playfaintcry(void);
static void atk57(void);
static void atk58_returntoball(void);
static void atk59_handlelearnnewmove(void);
static void atk5A_yesnoboxlearnmove(void);
static void atk5B_yesnoboxstoplearningmove(void);
static void atk5C_hitanimation(void);
static void atk5D_getmoneyreward(void);
static void atk5E(void);
static void atk5F_swapattackerwithtarget(void);
static void atk60_incrementgamestat(void);
static void atk61_drawpartystatussummary(void);
static void atk62_hidepartystatussummary(void);
static void atk63_jumptocalledmove(void);
static void atk64_statusanimation(void);
static void atk65_status2animation(void);
static void atk66_chosenstatusanimation(void);
static void atk67_yesnobox(void);
static void atk68_cancelallactions(void);
static void atk69_nop(void);
static void atk6A_removeitem(void);
static void atk6B_atknameinbuff1(void);
static void atk6C_drawlvlupbox(void);
static void atk6D_resetsentmonsvalue(void);
static void atk6E_setatktoplayer0(void);
static void atk6F_makevisible(void);
static void atk70_recordlastability(void);
static void atk71_buffermovetolearn(void);
static void atk72_jumpifplayerran(void);
static void atk73_hpthresholds(void);
static void atk74_hpthresholds2(void);
static void atk75_useitemonopponent(void);
static void atk76_various(void);
static void atk77_setprotectlike(void);
static void atk78_faintifabilitynotdamp(void);
static void atk79_setatkhptozero(void);
static void atk7A_jumpifnexttargetvalid(void);
static void atk7B_tryhealhalfhealth(void);
static void atk7C_trymirrormove(void);
static void atk7D_setrain(void);
static void atk7E_setreflect(void);
static void atk7F_setseeded(void);
static void atk80_manipulatedamage(void);
static void atk81_trysetrest(void);
static void atk82_jumpifnotfirstturn(void);
static void atk83_handletrainerslidecase(void);
static void atk84_trysetpoison(void);
static void atk85_stockpile(void);
static void atk86_stockpiletobasedamage(void);
static void atk87_stockpiletohpheal(void);
static void atk88_negativedamage(void);
static void atk89_statbuffchange(void);
static void atk8A_normalisebuffs(void);
static void atk8B_setbide(void);
static void atk8C_confuseifrepeatingattackends(void);
static void atk8D_setmultihitcounter(void);
static void atk8E_initmultihitstring(void);
static void atk8F_forcerandomswitch(void);
static void atk90_tryconversiontypechange(void);
static void atk91_givepaydaymoney(void);
static void atk92_setlightscreen(void);
static void atk93_tryKO(void);
static void atk94_damagetohalftargethp(void);
static void atk95_setsandstorm(void);
static void atk96_weatherdamage(void);
static void atk97_tryinfatuating(void);
static void atk98_updatestatusicon(void);
static void atk99_setmist(void);
static void atk9A_setfocusenergy(void);
static void atk9B_transformdataexecution(void);
static void atk9C_setsubstitute(void);
static void atk9D_mimicattackcopy(void);
static void atk9E_metronome(void);
static void atk9F_dmgtolevel(void);
static void atkA0_psywavedamageeffect(void);
static void atkA1_counterdamagecalculator(void);
static void atkA2_mirrorcoatdamagecalculator(void);
static void atkA3_disablelastusedattack(void);
static void atkA4_trysetencore(void);
static void atkA5_painsplitdmgcalc(void);
static void atkA6_settypetorandomresistance(void);
static void atkA7_setalwayshitflag(void);
static void atkA8_copymovepermanently(void);
static void atkA9_trychoosesleeptalkmove(void);
static void atkAA_setdestinybond(void);
static void atkAB_trysetdestinybondtohappen(void);
static void atkAC_trysetburn(void);
static void atkAD_tryspiteppreduce(void);
static void atkAE_healpartystatus(void);
static void atkAF_cursetarget(void);
static void atkB0_trysetspikes(void);
static void atkB1_setforesight(void);
static void atkB2_trysetperishsong(void);
static void atkB3_handlerollout(void);
static void atkB4_jumpifconfusedandstatmaxed(void);
static void atkB5_handlefurycutter(void);
static void atkB6_trysetparalyze(void);
static void atkB7_presentcalc(void);
static void atkB8_setsafeguard(void);
static void atkB9_magnitudedamagecalculation(void);
static void atkBA_jumpifnopursuitswitchdmg(void);
static void atkBB_setsunny(void);
static void atkBC_maxattackhalvehp(void);
static void atkBD_copyfoestats(void);
static void atkBE_rapidspinfree(void);
static void atkBF_setdefensecurlbit(void);
static void atkC0_recoverbasedonsunlight(void);
static void atkC1_nop(void);
static void atkC2_selectfirstvalidtarget(void);
static void atkC3_trysetfutureattack(void);
static void atkC4_trydobeatup(void);
static void atkC5_setsemiinvulnerablebit(void);
static void atkC6_clearsemiinvulnerablebit(void);
static void atkC7_setminimize(void);
static void atkC8_sethail(void);
static void atkC9_jumpifattackandspecialattackcannotfall(void);
static void atkCA_setforcedtarget(void);
static void atkCB_setcharge(void);
static void atkCC_callterrainattack(void);
static void atkCD_multihitresultmessage(void);
static void atkCE_settorment(void);
static void atkCF_jumpifnodamage(void);
static void atkD0_settaunt(void);
static void atkD1_trysethelpinghand(void);
static void atkD2_tryswapitems(void);
static void atkD3_trycopyability(void);
static void atkD4_trywish(void);
static void atkD5_trysetroots(void);
static void atkD6_nop(void);
static void atkD7_setyawn(void);
static void atkD8_setdamagetohealthdifference(void);
static void atkD9_nop(void);
static void atkDA_tryswapabilities(void);
static void atkDB_tryimprison(void);
static void atkDC_trysetgrudge(void);
static void atkDD_nop(void);
static void atkDE_assistattackselect(void);
static void atkDF_trysetmagiccoat(void);
static void atkE0_trysetsnatch(void);
static void atkE1_trygetintimidatetarget(void);
static void atkE2_switchoutabilities(void);
static void atkE3_jumpifhasnohp(void);
static void atkE4_getsecretpowereffect(void);
static void atkE5_pickup(void);
static void atkE6_docastformchangeanimation(void);
static void atkE7_trycastformdatachange(void);
static void atkE8_settypebasedhalvers(void);
static void atkE9_nop(void);
static void atkEA_tryrecycleitem(void);
static void atkEB_settypetoterrain(void);
static void atkEC_pursuitrelated(void);
static void atkED_snatchsetbattlers(void);
static void atkEE_removelightscreenreflect(void);
static void atkEF_handleballthrow(void);
static void atkF0_givecaughtmon(void);
static void atkF1_trysetcaughtmondexflags(void);
static void atkF2_displaydexinfo(void);
static void atkF3_trygivecaughtmonnick(void);
static void atkF4_subattackerhpbydmg(void);
static void atkF5_removeattackerstatus1(void);
static void atkF6_finishaction(void);
static void atkF7_finishturn(void);
static void atkF8_callasm(void);
static void atkF9_cureprimarystatus(void);
static void atkFA_setword(void);
static void atkFB_jumpifsubstituteblocks(void);
static void atkFC_loadabilitypopup(void);
static void atkFD_nop(void);
static void atkFE_nop(void);

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
    atk08_nop,
    atk09_attackanimation,
    atk0A_waitanimation,
    atk0B_healthbarupdate,
    atk0C_datahpupdate,
    atk0D_critmessage,
    atk0E_effectivenesssound,
    atk0F_resultmessage,
    atk10_printstring,
    atk11_printselectionstring,
    atk12_waitmessage,
    atk13_printfromtable,
    atk14_printselectionstringfromtable,
    atk15_seteffectwithchance,
    atk16_seteffectprimary,
    atk17_seteffectsecondary,
    atk18_clearstatusfromeffect,
    atk19_tryfaintmon,
    atk1A_dofaintanimation,
    atk1B_cleareffectsonfaint,
    atk1C_jumpifstatus,
    atk1D_jumpifstatus2,
    atk1E_jumpifability,
    atk1F_jumpifsideaffecting,
    atk20_jumpifstat,
    atk21_jumpifstatus3condition,
    atk22_jumpbasedontype,
    atk23_getexp,
    atk24,
    atk25_movevaluescleanup,
    atk26_setmultihit,
    atk27_decrementmultihit,
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
    atk32_copyarraywithindex,
    atk33_orbyte,
    atk34_orhalfword,
    atk35_orword,
    atk36_bicbyte,
    atk37_bichalfword,
    atk38_bicword,
    atk39_pause,
    atk3A_waitstate,
    atk3B_healthbar_update,
    atk3C_return,
    atk3D_end,
    atk3E_end2,
    atk3F_end3,
    atk40_jumpifaffectedbyprotect,
    atk41_call,
    atk42_trysetsleep,
    atk43_jumpifabilitypresent,
    atk44_endselectionscript,
    atk45_playanimation,
    atk46_playanimation2,
    atk47_setgraphicalstatchangevalues,
    atk48_playstatchangeanimation,
    atk49_moveend,
    atk4A_nop,
    atk4B_returnatktoball,
    atk4C_getswitchedmondata,
    atk4D_switchindataupdate,
    atk4E_switchinanim,
    atk4F_jumpifcantswitch,
    atk50_openpartyscreen,
    atk51_switchhandleorder,
    atk52_switchineffects,
    atk53_trainerslide,
    atk54_playse,
    atk55_fanfare,
    atk56_playfaintcry,
    atk57,
    atk58_returntoball,
    atk59_handlelearnnewmove,
    atk5A_yesnoboxlearnmove,
    atk5B_yesnoboxstoplearningmove,
    atk5C_hitanimation,
    atk5D_getmoneyreward,
    atk5E,
    atk5F_swapattackerwithtarget,
    atk60_incrementgamestat,
    atk61_drawpartystatussummary,
    atk62_hidepartystatussummary,
    atk63_jumptocalledmove,
    atk64_statusanimation,
    atk65_status2animation,
    atk66_chosenstatusanimation,
    atk67_yesnobox,
    atk68_cancelallactions,
    atk69_nop,
    atk6A_removeitem,
    atk6B_atknameinbuff1,
    atk6C_drawlvlupbox,
    atk6D_resetsentmonsvalue,
    atk6E_setatktoplayer0,
    atk6F_makevisible,
    atk70_recordlastability,
    atk71_buffermovetolearn,
    atk72_jumpifplayerran,
    atk73_hpthresholds,
    atk74_hpthresholds2,
    atk75_useitemonopponent,
    atk76_various,
    atk77_setprotectlike,
    atk78_faintifabilitynotdamp,
    atk79_setatkhptozero,
    atk7A_jumpifnexttargetvalid,
    atk7B_tryhealhalfhealth,
    atk7C_trymirrormove,
    atk7D_setrain,
    atk7E_setreflect,
    atk7F_setseeded,
    atk80_manipulatedamage,
    atk81_trysetrest,
    atk82_jumpifnotfirstturn,
    atk83_handletrainerslidecase,
    atk84_trysetpoison,
    atk85_stockpile,
    atk86_stockpiletobasedamage,
    atk87_stockpiletohpheal,
    atk88_negativedamage,
    atk89_statbuffchange,
    atk8A_normalisebuffs,
    atk8B_setbide,
    atk8C_confuseifrepeatingattackends,
    atk8D_setmultihitcounter,
    atk8E_initmultihitstring,
    atk8F_forcerandomswitch,
    atk90_tryconversiontypechange,
    atk91_givepaydaymoney,
    atk92_setlightscreen,
    atk93_tryKO,
    atk94_damagetohalftargethp,
    atk95_setsandstorm,
    atk96_weatherdamage,
    atk97_tryinfatuating,
    atk98_updatestatusicon,
    atk99_setmist,
    atk9A_setfocusenergy,
    atk9B_transformdataexecution,
    atk9C_setsubstitute,
    atk9D_mimicattackcopy,
    atk9E_metronome,
    atk9F_dmgtolevel,
    atkA0_psywavedamageeffect,
    atkA1_counterdamagecalculator,
    atkA2_mirrorcoatdamagecalculator,
    atkA3_disablelastusedattack,
    atkA4_trysetencore,
    atkA5_painsplitdmgcalc,
    atkA6_settypetorandomresistance,
    atkA7_setalwayshitflag,
    atkA8_copymovepermanently,
    atkA9_trychoosesleeptalkmove,
    atkAA_setdestinybond,
    atkAB_trysetdestinybondtohappen,
    atkAC_trysetburn,
    atkAD_tryspiteppreduce,
    atkAE_healpartystatus,
    atkAF_cursetarget,
    atkB0_trysetspikes,
    atkB1_setforesight,
    atkB2_trysetperishsong,
    atkB3_handlerollout,
    atkB4_jumpifconfusedandstatmaxed,
    atkB5_handlefurycutter,
    atkB6_trysetparalyze,
    atkB7_presentcalc,
    atkB8_setsafeguard,
    atkB9_magnitudedamagecalculation,
    atkBA_jumpifnopursuitswitchdmg,
    atkBB_setsunny,
    atkBC_maxattackhalvehp,
    atkBD_copyfoestats,
    atkBE_rapidspinfree,
    atkBF_setdefensecurlbit,
    atkC0_recoverbasedonsunlight,
    atkC1_nop,
    atkC2_selectfirstvalidtarget,
    atkC3_trysetfutureattack,
    atkC4_trydobeatup,
    atkC5_setsemiinvulnerablebit,
    atkC6_clearsemiinvulnerablebit,
    atkC7_setminimize,
    atkC8_sethail,
    atkC9_jumpifattackandspecialattackcannotfall,
    atkCA_setforcedtarget,
    atkCB_setcharge,
    atkCC_callterrainattack,
    atkCD_multihitresultmessage,
    atkCE_settorment,
    atkCF_jumpifnodamage,
    atkD0_settaunt,
    atkD1_trysethelpinghand,
    atkD2_tryswapitems,
    atkD3_trycopyability,
    atkD4_trywish,
    atkD5_trysetroots,
    atkD6_nop,
    atkD7_setyawn,
    atkD8_setdamagetohealthdifference,
    atkD9_nop,
    atkDA_tryswapabilities,
    atkDB_tryimprison,
    atkDC_trysetgrudge,
    atkDD_nop,
    atkDE_assistattackselect,
    atkDF_trysetmagiccoat,
    atkE0_trysetsnatch,
    atkE1_trygetintimidatetarget,
    atkE2_switchoutabilities,
    atkE3_jumpifhasnohp,
    atkE4_getsecretpowereffect,
    atkE5_pickup,
    atkE6_docastformchangeanimation,
    atkE7_trycastformdatachange,
    atkE8_settypebasedhalvers,
    atkE9_nop,
    atkEA_tryrecycleitem,
    atkEB_settypetoterrain,
    atkEC_pursuitrelated,
    atkED_snatchsetbattlers,
    atkEE_removelightscreenreflect,
    atkEF_handleballthrow,
    atkF0_givecaughtmon,
    atkF1_trysetcaughtmondexflags,
    atkF2_displaydexinfo,
    atkF3_trygivecaughtmonnick,
    atkF4_subattackerhpbydmg,
    atkF5_removeattackerstatus1,
    atkF6_finishaction,
    atkF7_finishturn,
    atkF8_callasm,
    atkF9_cureprimarystatus,
    atkFA_setword,
    atkFB_jumpifsubstituteblocks,
    atkFC_loadabilitypopup,
    atkFD_nop,
    atkFE_nop,
};

struct StatFractions
{
    u8 dividend;
    u8 divisor;
};

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
static const u16 sCriticalHitChance[] = { 16, 8, 4, 3, 2 };

static const u32 sStatusFlagsForMoveEffects[NUM_MOVE_EFFECTS] =
{
    [MOVE_EFFECT_SLEEP]          = STATUS1_SLEEP,
    [MOVE_EFFECT_POISON]         = STATUS1_POISON,
    [MOVE_EFFECT_BURN]           = STATUS1_BURN,
    [MOVE_EFFECT_FREEZE]         = STATUS1_FREEZE,
    [MOVE_EFFECT_PARALYSIS]      = STATUS1_PARALYSIS,
    [MOVE_EFFECT_TOXIC]          = STATUS1_TOXIC_POISON,
    [MOVE_EFFECT_CONFUSION]      = STATUS2_CONFUSION,
    [MOVE_EFFECT_FLINCH]         = STATUS2_FLINCHED,
    [MOVE_EFFECT_UPROAR]         = STATUS2_UPROAR,
    [MOVE_EFFECT_CHARGING]       = STATUS2_MULTIPLETURNS,
    [MOVE_EFFECT_WRAP]           = STATUS2_WRAPPED,
    [MOVE_EFFECT_RECHARGE]       = STATUS2_RECHARGE,
    [MOVE_EFFECT_PREVENT_ESCAPE] = STATUS2_ESCAPE_PREVENTION,
    [MOVE_EFFECT_NIGHTMARE]      = STATUS2_NIGHTMARE,
    [MOVE_EFFECT_THRASH]         = STATUS2_LOCK_CONFUSE,
};

static const u8 *const sMoveEffectBS_Ptrs[] =
{
    [MOVE_EFFECT_SLEEP]     = BattleScript_MoveEffectSleep,
    [MOVE_EFFECT_POISON]    = BattleScript_MoveEffectPoison,
    [MOVE_EFFECT_BURN]      = BattleScript_MoveEffectBurn,
    [MOVE_EFFECT_FREEZE]    = BattleScript_MoveEffectFreeze,
    [MOVE_EFFECT_PARALYSIS] = BattleScript_MoveEffectParalysis,
    [MOVE_EFFECT_TOXIC]     = BattleScript_MoveEffectToxic,
    [MOVE_EFFECT_CONFUSION] = BattleScript_MoveEffectConfusion,
    [MOVE_EFFECT_UPROAR]    = BattleScript_MoveEffectUproar,
    [MOVE_EFFECT_PAYDAY]    = BattleScript_MoveEffectPayDay,
    [MOVE_EFFECT_WRAP]      = BattleScript_MoveEffectWrap,
    [MOVE_EFFECT_RECOIL_25] = BattleScript_MoveEffectRecoil,
    [MOVE_EFFECT_RECOIL_33] = BattleScript_MoveEffectRecoil,
};

static const u16 sUnknownBattleboxPal[] = INCBIN_U16("graphics/battle_interface/unk_battlebox.gbapal");
static const u32 sUnknownBattleboxGfx[] = INCBIN_U32("graphics/battle_interface/unk_battlebox.4bpp.lz");

static const struct OamData sOamData_MonIconOnLvlUpBox =
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
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
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

static const u16 sProtectSuccessRates[] =
{ 
    USHRT_MAX,
    USHRT_MAX / 2, 
    USHRT_MAX / 4, 
    USHRT_MAX / 8
};

#define FORBIDDEN_MIMIC         (1 << 0)
#define FORBIDDEN_METRONOME     (1 << 1)
#define FORBIDDEN_ASSIST        (1 << 2)
#define FORBIDDEN_COPYCAT       (1 << 3)
#define FORBIDDEN_SLEEP_TALK    (1 << 4)
#define FORBIDDEN_INSTRUCT      (1 << 5)
#define FORBIDDEN_PARENTAL_BOND (1 << 6)

static const u16 sForbiddenMoves[MOVES_COUNT] =
{
    [MOVE_NONE] = 0xFFFF, // Can't use a non-move lol
    [MOVE_STRUGGLE] = 0xFFFF, // Neither Struggle
    [MOVE_AFTER_YOU] = FORBIDDEN_METRONOME,
    [MOVE_APPLE_ACID] = FORBIDDEN_METRONOME,
    [MOVE_ARM_THRUST] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_ASSIST] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT,
    [MOVE_ASTRAL_BARRAGE] = FORBIDDEN_METRONOME,
    [MOVE_AURA_WHEEL] = FORBIDDEN_METRONOME,
    [MOVE_BADDY_BAD] = FORBIDDEN_METRONOME,
    [MOVE_BANEFUL_BUNKER] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_BARRAGE] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_BEAK_BLAST] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT,
    [MOVE_BEAT_UP] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_BEHEMOTH_BASH] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_BEHEMOTH_BLADE] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_BELCH] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT,
    [MOVE_BESTOW] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_BIDE] = FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT | FORBIDDEN_PARENTAL_BOND, // Note: Bide should work with Parental Bond. This will be addressed in future.
    [MOVE_BLAST_BURN] = FORBIDDEN_INSTRUCT,
    [MOVE_BODY_PRESS] = FORBIDDEN_METRONOME,
    [MOVE_BONE_RUSH] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_BONEMERANG] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_BOUNCE] = FORBIDDEN_ASSIST | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT | FORBIDDEN_PARENTAL_BOND,
    [MOVE_BOUNCY_BUBBLE] = FORBIDDEN_METRONOME,
    [MOVE_BRANCH_POKE] = FORBIDDEN_METRONOME,
    [MOVE_BREAKING_SWIPE] = FORBIDDEN_METRONOME,
    [MOVE_BULLET_SEED] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_BUZZY_BUZZ] = FORBIDDEN_METRONOME,
    [MOVE_CELEBRATE] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT,
    [MOVE_CHATTER] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_MIMIC | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT,
    [MOVE_CIRCLE_THROW] = FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_CLANGOROUS_SOUL] = FORBIDDEN_METRONOME,
    [MOVE_COMET_PUNCH] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_COPYCAT] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT,
    [MOVE_COUNTER] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_COVET] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_CRAFTY_SHIELD] = FORBIDDEN_METRONOME,
    [MOVE_DECORATE] = FORBIDDEN_METRONOME,
    [MOVE_DESTINY_BOND] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_DETECT] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_DIAMOND_STORM] = FORBIDDEN_METRONOME,
    [MOVE_DIG] = FORBIDDEN_ASSIST | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT | FORBIDDEN_PARENTAL_BOND,
    [MOVE_DIVE] = FORBIDDEN_ASSIST | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT | FORBIDDEN_PARENTAL_BOND,
    [MOVE_DOUBLE_IRON_BASH] = FORBIDDEN_METRONOME | FORBIDDEN_PARENTAL_BOND,
    [MOVE_DOUBLE_HIT] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_DOUBLE_KICK] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_DOUBLE_SLAP] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_DRAGON_ASCENT] = FORBIDDEN_METRONOME,
    [MOVE_DRAGON_ENERGY] = FORBIDDEN_METRONOME,
    [MOVE_DRAGON_DARTS] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_DRAGON_TAIL] = FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_DRUM_BEATING] = FORBIDDEN_METRONOME,
    [MOVE_DUAL_CHOP] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_DUAL_WINGBEAT] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_DYNAMAX_CANNON] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT,
    [MOVE_ENDEAVOR] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_ENDURE] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_ETERNABEAM] = FORBIDDEN_METRONOME | FORBIDDEN_INSTRUCT,
    [MOVE_EXPLOSION] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_FALSE_SURRENDER] = FORBIDDEN_METRONOME,
    [MOVE_FEINT] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_FIERY_WRATH] = FORBIDDEN_METRONOME,
    [MOVE_FINAL_GAMBIT] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_FISSURE] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_FLEUR_CANNON] = FORBIDDEN_METRONOME,
    [MOVE_FLING] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_FLOATY_FALL] = FORBIDDEN_METRONOME,
    [MOVE_FLY] = FORBIDDEN_ASSIST | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT | FORBIDDEN_PARENTAL_BOND,
    [MOVE_FOCUS_PUNCH] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT,
    [MOVE_FOLLOW_ME] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_FREEZE_SHOCK] = FORBIDDEN_METRONOME | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT| FORBIDDEN_PARENTAL_BOND,
    [MOVE_FREEZING_GLARE] = FORBIDDEN_METRONOME,
    [MOVE_FREEZY_FROST] = FORBIDDEN_METRONOME,
    [MOVE_FURY_ATTACK] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_FURY_SWIPES] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_GEAR_GRIND] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_GEOMANCY] = FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT | FORBIDDEN_PARENTAL_BOND,
    [MOVE_GIGA_IMPACT] = FORBIDDEN_INSTRUCT,
    [MOVE_GLACIAL_LANCE] = FORBIDDEN_METRONOME,
    [MOVE_GLITZY_GLOW] = FORBIDDEN_METRONOME,
    [MOVE_GRAV_APPLE] = FORBIDDEN_METRONOME,
    [MOVE_GUILLOTINE] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_HELPING_HAND] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_HOLD_HANDS] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT,
    [MOVE_HORN_DRILL] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_HYDRO_CANNON] = FORBIDDEN_INSTRUCT,
    [MOVE_HYPER_BEAM] = FORBIDDEN_INSTRUCT,
    [MOVE_HYPERSPACE_FURY] = FORBIDDEN_METRONOME,
    [MOVE_HYPERSPACE_HOLE] = FORBIDDEN_METRONOME,
    [MOVE_ICE_BALL] = FORBIDDEN_INSTRUCT | FORBIDDEN_PARENTAL_BOND,
    [MOVE_ICE_BURN] = FORBIDDEN_METRONOME | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT | FORBIDDEN_PARENTAL_BOND,
    [MOVE_ICICLE_SPEAR] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_INSTRUCT] = FORBIDDEN_METRONOME | FORBIDDEN_INSTRUCT,
    [MOVE_JUNGLE_HEALING] = FORBIDDEN_METRONOME,
    [MOVE_KINGS_SHIELD] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_INSTRUCT,
    [MOVE_LIFE_DEW] = FORBIDDEN_METRONOME,
    [MOVE_LIGHT_OF_RUIN] = FORBIDDEN_METRONOME,
    [MOVE_MAT_BLOCK] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_ME_FIRST] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT,
    [MOVE_METEOR_ASSAULT] = FORBIDDEN_METRONOME | FORBIDDEN_INSTRUCT,
    [MOVE_METEOR_BEAM] = FORBIDDEN_INSTRUCT | FORBIDDEN_PARENTAL_BOND,
    [MOVE_METRONOME] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT,
    [MOVE_MIMIC] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_MIMIC | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT,
    [MOVE_MIND_BLOWN] = FORBIDDEN_METRONOME,
    [MOVE_MIRROR_COAT] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_MIRROR_MOVE] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT,
    [MOVE_MISTY_EXPLOSION] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_MOONGEIST_BEAM] = FORBIDDEN_METRONOME,
    [MOVE_NATURE_POWER] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT,
    [MOVE_NATURES_MADNESS] = FORBIDDEN_METRONOME,
    [MOVE_OBSTRUCT] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_INSTRUCT,
    [MOVE_ORIGIN_PULSE] = FORBIDDEN_METRONOME,
    [MOVE_OUTRAGE] = FORBIDDEN_INSTRUCT,
    [MOVE_OVERDRIVE] = FORBIDDEN_METRONOME,
    [MOVE_PHANTOM_FORCE] = FORBIDDEN_ASSIST | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT | FORBIDDEN_PARENTAL_BOND,
    [MOVE_PHOTON_GEYSER] = FORBIDDEN_METRONOME,
    [MOVE_PRISMATIC_LASER] = FORBIDDEN_INSTRUCT,
    [MOVE_PROTECT] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_PYRO_BALL] = FORBIDDEN_METRONOME,
    [MOVE_QUASH] = FORBIDDEN_METRONOME,
    [MOVE_QUICK_GUARD] = FORBIDDEN_METRONOME,
    [MOVE_RAGE_POWDER] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_RAZOR_WIND] = FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT | FORBIDDEN_PARENTAL_BOND,
    [MOVE_RELIC_SONG] = FORBIDDEN_METRONOME,
    [MOVE_ROAR] = FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_ROAR_OF_TIME] = FORBIDDEN_INSTRUCT,
    [MOVE_ROCK_BLAST] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_ROCK_WRECKER] = FORBIDDEN_INSTRUCT,
    [MOVE_ROLLOUT] = FORBIDDEN_INSTRUCT | FORBIDDEN_PARENTAL_BOND,
    [MOVE_SAPPY_SEED] = FORBIDDEN_METRONOME,
    [MOVE_SCALE_SHOT] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_SECRET_SWORD] = FORBIDDEN_METRONOME,
    [MOVE_SELF_DESTRUCT] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_SHADOW_FORCE] = FORBIDDEN_ASSIST | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT | FORBIDDEN_PARENTAL_BOND,
    [MOVE_SHEER_COLD] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_SHELL_TRAP] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT,
    [MOVE_SIZZLY_SLIDE] = FORBIDDEN_METRONOME,
    [MOVE_SKETCH] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_MIMIC | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT,
    [MOVE_SKULL_BASH] = FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT | FORBIDDEN_PARENTAL_BOND,
    [MOVE_SKY_ATTACK] = FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT | FORBIDDEN_PARENTAL_BOND,
    [MOVE_SKY_DROP] = FORBIDDEN_ASSIST | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT | FORBIDDEN_PARENTAL_BOND,
    [MOVE_SLEEP_TALK] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT,
    [MOVE_SNAP_TRAP] = FORBIDDEN_METRONOME,
    [MOVE_SNARL] = FORBIDDEN_METRONOME,
    [MOVE_SNATCH] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_INSTRUCT,
    [MOVE_SNORE] = FORBIDDEN_METRONOME,
    [MOVE_SOLAR_BEAM] = FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT | FORBIDDEN_PARENTAL_BOND,
    [MOVE_SOLAR_BLADE] = FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT | FORBIDDEN_PARENTAL_BOND,
    [MOVE_SPARKLY_SWIRL] = FORBIDDEN_METRONOME,
    [MOVE_SPECTRAL_THIEF] = FORBIDDEN_METRONOME,
    [MOVE_SPIKE_CANNON] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_SPIKY_SHIELD] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_SPIRIT_BREAK] = FORBIDDEN_METRONOME,
    [MOVE_SPLISHY_SPLASH] = FORBIDDEN_METRONOME,
    [MOVE_SPOTLIGHT] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_STEAM_ERUPTION] = FORBIDDEN_METRONOME,
    [MOVE_STEEL_BEAM] = FORBIDDEN_METRONOME,
    [MOVE_STRANGE_STEAM] = FORBIDDEN_METRONOME,
    [MOVE_SUNSTEEL_STRIKE] = FORBIDDEN_METRONOME,
    [MOVE_SURGING_STRIKES] = FORBIDDEN_METRONOME | FORBIDDEN_PARENTAL_BOND,
    [MOVE_SWITCHEROO] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_TAIL_SLAP] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_TECHNO_BLAST] = FORBIDDEN_METRONOME,
    [MOVE_THIEF] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_THOUSAND_ARROWS] = FORBIDDEN_METRONOME,
    [MOVE_THOUSAND_WAVES] = FORBIDDEN_METRONOME,
    [MOVE_THRASH] = FORBIDDEN_INSTRUCT,
    [MOVE_THUNDER_CAGE] = FORBIDDEN_METRONOME,
    [MOVE_THUNDEROUS_KICK] = FORBIDDEN_METRONOME,
    [MOVE_TRANSFORM] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT | FORBIDDEN_MIMIC | FORBIDDEN_INSTRUCT,
    [MOVE_TRICK] = FORBIDDEN_METRONOME | FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_TRIPLE_AXEL] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_TRIPLE_KICK] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_TWINEEDLE] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_UPROAR] = FORBIDDEN_SLEEP_TALK | FORBIDDEN_INSTRUCT | FORBIDDEN_PARENTAL_BOND,
    [MOVE_V_CREATE] = FORBIDDEN_METRONOME,
    [MOVE_VEEVEE_VOLLEY] = FORBIDDEN_METRONOME,
    [MOVE_WATER_SHURIKEN] = FORBIDDEN_PARENTAL_BOND,
    [MOVE_WHIRLWIND] = FORBIDDEN_ASSIST | FORBIDDEN_COPYCAT,
    [MOVE_WICKED_BLOW] = FORBIDDEN_METRONOME,
    [MOVE_WIDE_GUARD] = FORBIDDEN_METRONOME,
    [MOVE_ZIPPY_ZAP] = FORBIDDEN_METRONOME,
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

static const u16 sNaturePowerMoves[] =
{
    MOVE_STUN_SPORE,
    MOVE_RAZOR_LEAF,
    MOVE_EARTHQUAKE,
    MOVE_HYDRO_PUMP,
    MOVE_SURF,
    MOVE_BUBBLE_BEAM,
    MOVE_ROCK_SLIDE,
    MOVE_SHADOW_BALL,
    MOVE_SWIFT,
    MOVE_SWIFT
};

struct PickupItem
{
    u16 itemId;
    u8 chance;
};

static const struct PickupItem sPickupItems[] =
{
    { ITEM_ORAN_BERRY, 15 },
    { ITEM_CHERI_BERRY, 25 },
    { ITEM_CHESTO_BERRY, 35 },
    { ITEM_PECHA_BERRY, 45 },
    { ITEM_RAWST_BERRY, 55 },
    { ITEM_ASPEAR_BERRY, 65 },
    { ITEM_PERSIM_BERRY, 75 },
    { ITEM_TM10, 80 },
    { ITEM_PP_UP, 85 },
    { ITEM_RARE_CANDY, 90 },
    { ITEM_NUGGET, 95 },
    { ITEM_SPELON_BERRY, 96 },
    { ITEM_PAMTRE_BERRY, 97 },
    { ITEM_WATMEL_BERRY, 98 },
    { ITEM_DURIN_BERRY, 99 },
    { ITEM_BELUE_BERRY, 1 },
};

static const u8 sTerrainToType[] =
{
    TYPE_GRASS,  // tall grass
    TYPE_GRASS,  // long grass
    TYPE_GROUND, // sand
    TYPE_WATER,  // underwater
    TYPE_WATER,  // water
    TYPE_WATER,  // pond water
    TYPE_ROCK,   // rock
    TYPE_ROCK,   // cave
    TYPE_NORMAL, // building
    TYPE_NORMAL, // plain
};

static const u8 sBallCatchBonuses[] =
{
    20, 15, 10, 15 // Ultra, Great, Poke, Safari
};

static bool8 HasAttackerFaintedTarget(void)
{
	if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && gBattleMoves[gCurrentMove].power && (gLastHitBy[gBattlerTarget] == 0xFF || gLastHitBy[gBattlerTarget] == gBattlerAttacker)
		&& gBattleStruct->moveTarget[gBattlerAttacker] == gBattlerTarget && gBattlerTarget != gBattlerAttacker && gCurrentTurnActionNumber == GetBattlerTurnOrderNum(gBattlerAttacker)
	    && (gChosenMove == gChosenMoveByBattler[gBattlerAttacker] || gChosenMove == gBattleMons[gBattlerAttacker].moves[gChosenMovePos]))
		return TRUE;
	return FALSE;
}

static void atk00_attackcanceler(void)
{
    s32 i;
	u8 moveType = gBattleStruct->dynamicMoveType;
	
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
	// Check primal weather
	if (!IS_MOVE_STATUS(gCurrentMove))
	{
		switch (moveType)
		{
			case TYPE_FIRE:
			    if (IsBattlerWeatherAffected(gBattlerAttacker, WEATHER_RAIN_PRIMAL))
				{
					BattleScriptPushCursor();
					gSetWordLoc = sFizzlesFireTypeString;
					gBattlescriptCurrInstr = BattleScript_PrimordialSeaFizzlesOutFireTypeMoves;
					return;
				}
				break;
			case TYPE_WATER:
			    if (IsBattlerWeatherAffected(gBattlerAttacker, WEATHER_SUN_PRIMAL))
				{
					BattleScriptPushCursor();
					gSetWordLoc = sEvaporateWaterTypeString;
					gBattlescriptCurrInstr = BattleScript_DesolateLandEvaporatesWaterTypeMoves;
					return;
				}
				break;
		}
	}
	// Check effects like flinch, freeze, etc.
    if (AtkCanceller_UnableToUseMove())
        return;
	// Try Activate Protean
	if (gCurrentMove != MOVE_STRUGGLE && !IS_BATTLER_OF_TYPE(gBattlerAttacker, moveType) && gBattleMons[gBattlerAttacker].type3 != TYPE_MYSTERY
	&& gDisableStructs[gBattlerAttacker].canProteanActivate && (GetBattlerAbility(gBattlerAttacker) == ABILITY_PROTEAN || GetBattlerAbility(gBattlerAttacker) == ABILITY_LIBERO))
	{
		gLastUsedAbility = GetBattlerAbility(gBattlerAttacker);
		gDisableStructs[gBattlerAttacker].canProteanActivate = FALSE;
		SET_BATTLER_TYPE(gBattlerAttacker, moveType);
		PREPARE_TYPE_BUFFER(gBattleTextBuff1, moveType);
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_ProteanActivates;
        return;
	}
	// Check abilities that blocks the move
	if (AbilityBattleEffects(ABILITYEFFECT_MOVES_BLOCK, gBattlerTarget, 0, 0, 0))
		return;
	// Check no PP for move
    if (!gBattleMons[gBattlerAttacker].pp[gCurrMovePos] && !(gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS) 
	&& gCurrentMove != MOVE_STRUGGLE && !(gHitMarker & (HITMARKER_x800000 | HITMARKER_NO_ATTACKSTRING)))
    {
        gBattlescriptCurrInstr = BattleScript_NoPPForMove;
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        return;
    }
	gHitMarker &= ~(HITMARKER_x800000);
	
	// Try activate Stance Change
	if (GetBattlerAbility(gBattlerAttacker) == ABILITY_STANCE_CHANGE && !(gBattleMons[gBattlerAttacker].status2 & STATUS2_TRANSFORMED))
	{
		switch (gBattleMons[gBattlerAttacker].species)
		{
			case SPECIES_AEGISLASH:
			    if (!IS_MOVE_STATUS(gCurrentMove))
				{
					DoBattleFormChange(gBattlerAttacker, SPECIES_AEGISLASH_BLADE, FALSE, TRUE, FALSE);
					gSetWordLoc = sStanceChangeToBladeString;
					gLastUsedAbility = ABILITY_STANCE_CHANGE;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_AttackerFormChange;
					return;
				}
				break;
			case SPECIES_AEGISLASH_BLADE:
			    if (gCurrentMove == MOVE_KINGS_SHIELD)
				{
					DoBattleFormChange(gBattlerAttacker, SPECIES_AEGISLASH, FALSE, TRUE, FALSE);
					gSetWordLoc = sStanceChangeToShieldString;
					gLastUsedAbility = ABILITY_STANCE_CHANGE;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_AttackerFormChange;
					return;
				}
				break;
		}
	}
	// Check if obeys
    if (!(gHitMarker & HITMARKER_OBEYS) && !(gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS))
    {
        i = IsMonDisobedient();
	    
        switch (i)
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
	
	// Try Bounce the move back
	if (gBattleMoves[gCurrentMove].flags & FLAG_MAGICCOAT_AFFECTED && !gProtectStructs[gBattlerAttacker].usesBouncedMove)
	{
		if (gProtectStructs[gBattlerTarget].bounceMove)
		{
			gProtectStructs[gBattlerTarget].usesBouncedMove = TRUE;
			PressurePPLose(gBattlerAttacker, gBattlerTarget, MOVE_MAGIC_COAT);
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_MagicCoatBounce;
			return;
		}
		else if (GetBattlerAbility(gBattlerTarget) == ABILITY_MAGIC_BOUNCE && !(gStatuses3[gBattlerTarget] & STATUS3_SEMI_INVULNERABLE))
		{
			RecordAbilityBattle(gBattlerTarget, ABILITY_MAGIC_BOUNCE);
			gProtectStructs[gBattlerTarget].usesBouncedMove = TRUE;
			gSetWordLoc = sMagicBounceString;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_MagicBounce;
			return;
		}
	}
	// Try Snatch move
    for (i = 0; i < gBattlersCount; ++i)
    {
        if (gBattleMoves[gCurrentMove].flags & FLAG_SNATCH_AFFECTED && (gProtectStructs[gBattlerByTurnOrder[i]].stealMove))
        {
			gProtectStructs[gBattlerByTurnOrder[i]].stealMove = FALSE;
            PressurePPLose(gBattlerAttacker, gBattlerByTurnOrder[i], MOVE_SNATCH);
            gBattleScripting.battler = gBattlerByTurnOrder[i];
            BattleScriptPushCursor();
            gBattlescriptCurrInstr = BattleScript_SnatchedMove;
            return;
        }
    }
	// Check Lightning Rod, Storm Drain and Protect
    if (gSpecialStatuses[gBattlerTarget].lightningRodRedirected)
    {
        gSpecialStatuses[gBattlerTarget].lightningRodRedirected = FALSE;
        gLastUsedAbility = ABILITY_LIGHTNING_ROD;
        BattleScriptPushCursor();
        gBattlescriptCurrInstr = BattleScript_TookAttack;
        RecordAbilityBattle(gBattlerTarget, gLastUsedAbility);
    }
    else if (gSpecialStatuses[gBattlerTarget].stormDrainRedirected)
    {
		gSpecialStatuses[gBattlerTarget].stormDrainRedirected = FALSE;
		gLastUsedAbility = ABILITY_STORM_DRAIN;
        BattleScriptPushCursor();
        gBattlescriptCurrInstr = BattleScript_TookAttack;
        RecordAbilityBattle(gBattlerTarget, gLastUsedAbility);
    }
    else if (DEFENDER_IS_PROTECTED && (gCurrentMove != MOVE_CURSE || IS_BATTLER_OF_TYPE(gBattlerAttacker, TYPE_GHOST))
          && ((!IsTwoTurnsMove(gCurrentMove) || (gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS))))
    {
        CancelMultiTurnMoves(gBattlerAttacker);
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gLastLandedMoves[gBattlerTarget] = 0;
        gLastHitByType[gBattlerTarget] = 0;
        gBattleCommunication[6] = 1;
        ++gBattlescriptCurrInstr;
    }
    else
        ++gBattlescriptCurrInstr;
}

static void JumpIfMoveFailed(u8 adder, u16 move)
{
    const u8 *BS_ptr = gBattlescriptCurrInstr + adder;

    if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
    {
        gLastLandedMoves[gBattlerTarget] = 0;
        gLastHitByType[gBattlerTarget] = 0;
        BS_ptr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else
    {
        TrySetDestinyBondToHappen();
        if (AbilityBattleEffects(ABILITYEFFECT_ABSORBING, gBattlerTarget, 0, 0, move))
            return;
    }
    gBattlescriptCurrInstr = BS_ptr;
}

static bool8 JumpIfMoveAffectedByProtect(u16 move)
{
    bool8 affected = FALSE;

    if (DEFENDER_IS_PROTECTED)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        JumpIfMoveFailed(7, move);
        gBattleCommunication[6] = 1;
        affected = TRUE;
    }
    return affected;
}

static bool8 AccuracyCalcHelper(u16 move)
{
	// Check Lock On
    if (gStatuses3[gBattlerTarget] & STATUS3_ALWAYS_HITS && gDisableStructs[gBattlerTarget].battlerWithSureHit == gBattlerAttacker)
    {
        JumpIfMoveFailed(7, move);
        return TRUE;
    }
	// Always hit if attacker or target have No Guard
    if (GetBattlerAbility(gBattlerAttacker) != ABILITY_NO_GUARD && GetBattlerAbility(gBattlerTarget) != ABILITY_NO_GUARD)
    {
		// Check if semi-invulnerable
	    if (!(gHitMarker & HITMARKER_IGNORE_ON_AIR) && gStatuses3[gBattlerTarget] & STATUS3_ON_AIR)
	    {
		    gMoveResultFlags |= MOVE_RESULT_MISSED;
		    JumpIfMoveFailed(7, move);
		    return TRUE;
	    }
	    gHitMarker &= ~(HITMARKER_IGNORE_ON_AIR);
		
	    if (!(gHitMarker & HITMARKER_IGNORE_UNDERGROUND) && gStatuses3[gBattlerTarget] & STATUS3_UNDERGROUND)
	    {
		    gMoveResultFlags |= MOVE_RESULT_MISSED;
		    JumpIfMoveFailed(7, move);
		    return TRUE;
	    }
	    gHitMarker &= ~(HITMARKER_IGNORE_UNDERGROUND);
		
	    if (!(gHitMarker & HITMARKER_IGNORE_UNDERWATER) && gStatuses3[gBattlerTarget] & STATUS3_UNDERWATER)
	    {
		    gMoveResultFlags |= MOVE_RESULT_MISSED;
		    JumpIfMoveFailed(7, move);
		    return TRUE;
	    }
	    gHitMarker &= ~(HITMARKER_IGNORE_UNDERWATER);
		
		// Check Thunder on rain and moves that never miss
	    if ((IsBattlerWeatherAffected(gBattlerAttacker, WEATHER_RAIN_ANY) && gBattleMoves[move].effect == EFFECT_THUNDER) || (gBattleMoves[move].accuracy == 0))
	    {
		    JumpIfMoveFailed(7, move);
		    return TRUE;
	    }
    }
    return FALSE;
}

static void atk01_accuracycheck(void)
{
    u8 buff, acc, type, moveAcc, holdEffect, param;
    u16 calc, move = T2_READ_16(gBattlescriptCurrInstr + 5);
    
	// Poke Dude and first battle moves never misses
    if ((gBattleTypeFlags & BATTLE_TYPE_POKEDUDE) || (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE && !BtlCtrl_OakOldMan_TestState2Flag(1) 
         && gBattleMoves[move].power != 0 && GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER) || (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE 
         && !BtlCtrl_OakOldMan_TestState2Flag(2) && gBattleMoves[move].power == 0 && GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER))
    {
        JumpIfMoveFailed(7, move);
        return;
    }
    if (move == NO_ACC_CALC || move == NO_ACC_CALC_CHECK_LOCK_ON)
    {
        if (move == NO_ACC_CALC_CHECK_LOCK_ON && gStatuses3[gBattlerTarget] & STATUS3_ALWAYS_HITS && gDisableStructs[gBattlerTarget].battlerWithSureHit == gBattlerAttacker)
            gBattlescriptCurrInstr += 7; // Only checks for Lock On, no acc calc
        else if (gStatuses3[gBattlerTarget] & (STATUS3_SEMI_INVULNERABLE) && GetBattlerAbility(gBattlerTarget) != ABILITY_NO_GUARD
	      && GetBattlerAbility(gBattlerAttacker) != ABILITY_NO_GUARD) // Check semi-invulnerable
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        else if (!JumpIfMoveAffectedByProtect(0)) // Check Protect
            gBattlescriptCurrInstr += 7;
    }
    else
    {
		if (move == MOVE_NONE)
			move = gCurrentMove;  
	    
		// Check Skill Link on second hit of multi-strike moves
		if ((gBattleMoves[move].effect == EFFECT_MULTI_HIT || gBattleMoves[move].effect == EFFECT_TRIPLE_KICK)
			&& GetBattlerAbility(gBattlerAttacker) == ABILITY_SKILL_LINK)
		{
			gBattlescriptCurrInstr += 7;
			return;
		}
        type = gBattleStruct->dynamicMoveType;
		moveAcc = gBattleMoves[move].accuracy;
	    
        if (JumpIfMoveAffectedByProtect(move) || AccuracyCalcHelper(move)) // Check Protect and semi-invulnerable
            return;
		
		acc = gBattleMons[gBattlerAttacker].statStages[STAT_ACC];
		
		if (GetBattlerAbility(gBattlerTarget) == ABILITY_UNAWARE) // Target's Unaware ignore the user's accuracy stat stages
			acc = 6;
		if (gBattleMons[gBattlerTarget].status2 & STATUS2_FORESIGHT || GetBattlerAbility(gBattlerAttacker) == ABILITY_UNAWARE) // Checks User's Foresight and Unaware
            buff = acc; // Ignore the target's evasion stat stages
        else
            buff = acc + 6 - gBattleMons[gBattlerTarget].statStages[STAT_EVASION];
	    
        if (buff > 0xC)
            buff = 0xC; // Buff can't be higer than 0xC
	    
        // Check Thunder on sunny
        if (IsBattlerWeatherAffected(gBattlerAttacker, WEATHER_SUN_ANY) && gBattleMoves[move].effect == EFFECT_THUNDER)
			moveAcc = 50;
		// Check Wonder Skin
		if (GetBattlerAbility(gBattlerTarget) == ABILITY_WONDER_SKIN && IS_MOVE_STATUS(move) && moveAcc > 50)
			moveAcc = 50;
	    
		// Calculate accuracy
        calc = sAccuracyStageRatios[buff].dividend * moveAcc;
        calc /= sAccuracyStageRatios[buff].divisor;
	    
		// Check abilities
        if (GetBattlerAbility(gBattlerAttacker) == ABILITY_COMPOUND_EYES)
            calc = (calc * 130) / 100; // 1.3 Compound Eyes boost
		if (GetBattlerAbility(gBattlerAttacker) == ABILITY_VICTORY_STAR)
			calc = (calc * 110) / 100; // 1.1 Victory Star boost
		if (IsBattlerAlive(BATTLE_PARTNER(gBattlerAttacker)) && GetBattlerAbility(BATTLE_PARTNER(gBattlerAttacker)) == ABILITY_VICTORY_STAR)
			calc = (calc * 110) / 100; // 1.1 ally's Victory Star boost
		if (GetBattlerAbility(gBattlerAttacker) == ABILITY_HUSTLE && IS_MOVE_PHYSICAL(move))
            calc = (calc * 80) / 100; // 1.2 Hustle loss
		if (GetBattlerAbility(gBattlerTarget) == ABILITY_TANGLED_FEET && gBattleMons[gBattlerTarget].status2 & STATUS2_CONFUSION)
			calc /= 2; // Tangled Feet halved
		if (IsBattlerWeatherAffected(gBattlerTarget, WEATHER_SANDSTORM_ANY) && GetBattlerAbility(gBattlerTarget) == ABILITY_SAND_VEIL)
			calc = (calc * 80) / 100; // 1.2 Sand Veil loss
		if (IsBattlerWeatherAffected(gBattlerTarget, WEATHER_HAIL_ANY) && GetBattlerAbility(gBattlerTarget) == ABILITY_SNOW_CLOAK)
			calc = (calc * 80) / 100; // 1.2 Snow Cloak loss
		
		holdEffect = GetBattlerItemHoldEffect(gBattlerTarget, TRUE);
		param = GetBattlerHoldEffectParam(gBattlerTarget);
		
		gPotentialItemEffectBattler = gBattlerTarget;
		
		// Check items
		if (holdEffect == HOLD_EFFECT_EVASION_UP)
			calc = (calc * (100 - param)) / 100;
		
		// Check fog weather
		if (IsBattlerWeatherAffected(gBattlerAttacker, WEATHER_FOG_ANY))
			calc = (calc * 60) / 100; // 0.6 fog loss
		
		// final calculation, determines if the move misses
		if ((Random() % 100 + 1) > calc)
		{
			gMoveResultFlags |= MOVE_RESULT_MISSED;
			
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && (gBattleMoves[move].target == MOVE_TARGET_BOTH || gBattleMoves[move].target == MOVE_TARGET_FOES_AND_ALLY))
				gBattleCommunication[6] = 2;
			else
				gBattleCommunication[6] = 0;
			CheckWonderGuardAndLevitate();
		}
		JumpIfMoveFailed(7, move);
    }
}

static void atk02_attackstring(void)
{
    if (!gBattleControllerExecFlags)
    {
        if (!(gHitMarker & (HITMARKER_NO_ATTACKSTRING | HITMARKER_ATTACKSTRING_PRINTED)))
        {
            PrepareStringBattle(STRINGID_USEDMOVE, gBattlerAttacker);
            gHitMarker |= HITMARKER_ATTACKSTRING_PRINTED;
        }
        ++gBattlescriptCurrInstr;
        gBattleCommunication[MSG_DISPLAY] = 0;
    }
}

 static void atk03_ppreduce(void)
{
    s32 ppToDeduct = 1;

    if (!gBattleControllerExecFlags)
    {
		// Check Pressure PP usage
        if (!gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure)
        {
            switch (gBattleMoves[gCurrentMove].target)
            {
            case MOVE_TARGET_FOES_AND_ALLY:
                ppToDeduct += AbilityBattleEffects(ABILITYEFFECT_COUNT_ON_FIELD, gBattlerAttacker, ABILITY_PRESSURE, 0, 0);
                break;
            case MOVE_TARGET_BOTH:
            case MOVE_TARGET_OPPONENTS_FIELD:
                ppToDeduct += AbilityBattleEffects(ABILITYEFFECT_COUNT_OTHER_SIDE, gBattlerAttacker, ABILITY_PRESSURE, 0, 0);
                break;
            default:
                if (gBattlerAttacker != gBattlerTarget && GetBattlerAbility(gBattlerTarget) == ABILITY_PRESSURE)
                    ++ppToDeduct;
                break;
            }
        }
		// Decreases the PP
        if (!(gHitMarker & (HITMARKER_NO_PPDEDUCT | HITMARKER_NO_ATTACKSTRING)) && gBattleMons[gBattlerAttacker].pp[gCurrMovePos])
        {
            if (gBattleMons[gBattlerAttacker].pp[gCurrMovePos] > ppToDeduct)
                gBattleMons[gBattlerAttacker].pp[gCurrMovePos] -= ppToDeduct;
            else
                gBattleMons[gBattlerAttacker].pp[gCurrMovePos] = 0;
			
			gProtectStructs[gBattlerAttacker].notFirstStrike = 1;
		
            if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_TRANSFORMED)
                && !((gDisableStructs[gBattlerAttacker].mimickedMoves) & gBitTable[gCurrMovePos]))
            {
                gActiveBattler = gBattlerAttacker;
                BtlController_EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + gCurrMovePos, 0, 1, &gBattleMons[gBattlerAttacker].pp[gCurrMovePos]);
                MarkBattlerForControllerExec(gBattlerAttacker);
            }
        }
        gHitMarker &= ~(HITMARKER_NO_PPDEDUCT);
        ++gBattlescriptCurrInstr;
    }
}

static void atk04_critcalc(void)
{
    u8 holdEffect = GetBattlerItemHoldEffect(gBattlerAttacker, TRUE);
    u16 critChance;
    gPotentialItemEffectBattler = gBattlerAttacker;
	
	// Check all effects that increases the crit chance
    critChance  = 2 * ((gBattleMons[gBattlerAttacker].status2 & STATUS2_FOCUS_ENERGY) != 0) // Focus Energy
	        + (GetBattlerAbility(gBattlerAttacker) == ABILITY_SUPER_LUCK) // Super Luck
                + ((gBattleMoves[gCurrentMove].flags & FLAG_HIGH_CRIT) != 0) // High Crit move
                + (holdEffect == HOLD_EFFECT_SCOPE_LENS) // Scope Lens
                + 2 * (holdEffect == HOLD_EFFECT_LUCKY_PUNCH && gBattleMons[gBattlerAttacker].species == SPECIES_CHANSEY) // Lucky Punch
                + 2 * (holdEffect == HOLD_EFFECT_STICK && gBattleMons[gBattlerAttacker].species == SPECIES_FARFETCHD); // Stick
	
    if (critChance >= NELEMS(sCriticalHitChance))
        critChance = NELEMS(sCriticalHitChance) - 1; // Crit chance can't be higer than table's lenght
	
	// Check if can be crit
    if (!(Random() % sCriticalHitChance[critChance])
	&& (GetBattlerAbility(gBattlerTarget) != ABILITY_BATTLE_ARMOR && GetBattlerAbility(gBattlerTarget) != ABILITY_SHELL_ARMOR)
	&& !(gStatuses3[gBattlerAttacker] & STATUS3_CANT_SCORE_A_CRIT)
	&& !(gBattleTypeFlags & BATTLE_TYPE_OLD_MAN_TUTORIAL)
	&& !(gBattleTypeFlags & BATTLE_TYPE_POKEDUDE)
	&& (!(gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE) || BtlCtrl_OakOldMan_TestState2Flag(1)))
        gCritMultiplier = 2;
    else
        gCritMultiplier = 1;
    ++gBattlescriptCurrInstr;
}

static void atk05_damagecalc(void)
{
	// Calculate move damage
    gBattleMoveDamage = CalculateBaseDamage(gCurrentMove, gBattleStruct->dynamicMoveType, gBattlerAttacker, gBattlerTarget, FALSE, gCritMultiplier == 2 ? TRUE : FALSE, TRUE);
    ++gBattlescriptCurrInstr;
}

void AI_CalcDmg(u8 attacker, u8 defender, u16 move)
{
    u8 moveType = gBattleMoves[move].type;
    u16 flags = 0;
    
	// Calculate move damage and type effectiveness for the AI
    gBattleScripting.dmgMultiplier = 1;
    gMoveResultFlags = 0;
    gBattleMoveDamage = CalculateBaseDamage(move, moveType, attacker, defender, FALSE, FALSE, FALSE);
    TypeCalc(move, moveType, attacker, defender, FALSE, FALSE, &flags);
}

static void atk06_typecalc(void)
{
	if (gCurrentMove != MOVE_STRUGGLE) // except Struggle
    {
		// Calculate type effectiveness
        TypeCalc(gCurrentMove, gBattleStruct->dynamicMoveType, gBattlerAttacker, gBattlerTarget, TRUE, TRUE, &gMoveResultFlags);
		
		if (gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE)
			gProtectStructs[gBattlerAttacker].targetNotAffected = 1;
	}
    ++gBattlescriptCurrInstr;
}

#define TYPE_AFFECTED_LEVITATE     (1 << 0)
#define TYPE_AFFECTED_WONDER_GUARD (1 << 1)

static void CheckWonderGuardAndLevitate(void) // Check for Wonder Guard and Levitate for acurracy calc
{
    u16 flags = 0;
    u8 affectedBy;
    s32 saved = gBattleMoveDamage;
    
    if (gCurrentMove == MOVE_STRUGGLE || !gBattleMoves[gCurrentMove].power)
        return;
	
    affectedBy = TypeCalc(gCurrentMove, gBattleStruct->dynamicMoveType, gBattlerAttacker, gBattlerTarget, TRUE, FALSE, &flags);
    gBattleMoveDamage = saved;
    
    if (affectedBy & TYPE_AFFECTED_LEVITATE)
        flags &= ~(MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
	
    if (affectedBy & TYPE_AFFECTED_WONDER_GUARD)
        flags &= ~(MOVE_RESULT_MISSED);
	
    if (!(affectedBy & TYPE_AFFECTED_LEVITATE) && flags & MOVE_RESULT_DOESNT_AFFECT_FOE)
    {
        gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
		gProtectStructs[gBattlerAttacker].targetNotAffected = 1;
    }
}

static void ModulateDmgByType(u8 multiplier, u16 move, u16 *flags) // Update effectiveness and do damage multiplier for the type calc
{
    gBattleMoveDamage = gBattleMoveDamage * multiplier / 10;
    if (gBattleMoveDamage == 0 && multiplier != 0)
        gBattleMoveDamage = 1;
	
    switch (multiplier)
    {
		case TYPE_MUL_NO_EFFECT:
		    *flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
			*flags &= ~MOVE_RESULT_NOT_VERY_EFFECTIVE;
			*flags &= ~MOVE_RESULT_SUPER_EFFECTIVE;
			break;
		case TYPE_MUL_NOT_EFFECTIVE:
		    if (gBattleMoves[move].power && !(*flags & MOVE_RESULT_NO_EFFECT))
			{
				if (*flags & MOVE_RESULT_SUPER_EFFECTIVE)
					*flags &= ~MOVE_RESULT_SUPER_EFFECTIVE;
				else
					*flags |= MOVE_RESULT_NOT_VERY_EFFECTIVE;
			}
			break;
		case TYPE_MUL_SUPER_EFFECTIVE:
		    if (gBattleMoves[move].power && !(*flags & MOVE_RESULT_NO_EFFECT))
			{
				if (*flags & MOVE_RESULT_NOT_VERY_EFFECTIVE)
					*flags &= ~MOVE_RESULT_NOT_VERY_EFFECTIVE;
				else
					*flags |= MOVE_RESULT_SUPER_EFFECTIVE;
			}
			break;
    }
}

u8 GetTypeModifier(u8 atkType, u8 defType)
{
	return gTypeEffectiveness[atkType][defType]; // Get effectiveness betwen the move's type and the defender's type
}

static void MulByTypeEffectiveness(u16 move, u8 moveType, u8 attacker, u8 defender, u8 defenderType, u16 *flags)
{
	u8 mod = GetTypeModifier(moveType, defenderType);
	
	if ((moveType == TYPE_FIGHTING || moveType == TYPE_NORMAL) && defenderType == TYPE_GHOST && mod == TYPE_MUL_NO_EFFECT
	&& (gBattleMons[defender].status2 & STATUS2_FORESIGHT || GetBattlerAbility(attacker) == ABILITY_SCRAPPY)) // Check Foresight and Scrappy on Ghost types
	    mod = TYPE_MUL_NORMAL;
	
	// Check strong winds
	if (IsBattlerWeatherAffected(defender, WEATHER_STRONG_WINDS) && defenderType == TYPE_FLYING && mod == TYPE_MUL_SUPER_EFFECTIVE)
		mod = TYPE_MUL_NORMAL;
	
	ModulateDmgByType(mod, move, flags);
}

u8 CalcTypeEffectivenessMultiplierInternal(u16 move, u8 moveType, u8 attacker, u8 defender, bool8 recordAbilities, u16 *flags)
{
	u16 defAbility = GetBattlerAbility(defender);
	u8 affectedBy = 0;
	
	MulByTypeEffectiveness(move, moveType, attacker, defender, gBattleMons[defender].type1, flags); // Multiplier interation betwen type1
	
	if (gBattleMons[defender].type1 != gBattleMons[defender].type2)
		MulByTypeEffectiveness(move, moveType, attacker, defender, gBattleMons[defender].type2, flags); // Multiplier interation betwen type2
	
	// Check Levitate
	if (defAbility == ABILITY_LEVITATE && moveType == TYPE_GROUND)
	{
		*flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
		affectedBy = TYPE_AFFECTED_LEVITATE;
		
		if (recordAbilities)
		{
			gLastUsedAbility = defAbility;
			gBattleCommunication[6] = 4;
			RecordAbilityBattle(defender, gLastUsedAbility);
		}
	}
	// Check Wonder Guard
	if (defAbility == ABILITY_WONDER_GUARD && AttacksThisTurn(attacker, move) == 2
	&& (!(*flags & MOVE_RESULT_SUPER_EFFECTIVE) || ((*flags & (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)) == (MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE)))
	&& gBattleMoves[move].power)
	{
		*flags |= MOVE_RESULT_MISSED;
		affectedBy = TYPE_AFFECTED_WONDER_GUARD;
		
		if (recordAbilities)
		{
			gLastUsedAbility = defAbility;
			gBattleCommunication[6] = 3;
			RecordAbilityBattle(defender, gLastUsedAbility);
		}
	}
	return affectedBy;
}

u8 TypeCalc(u16 move, u8 moveType, u8 attacker, u8 defender, bool8 recordAbilities, bool8 resetFlags, u16 *flags)
{
	u8 affectedBy = 0;
	
	if (move != MOVE_STRUGGLE)
	{
		affectedBy |= CalcTypeEffectivenessMultiplierInternal(move, moveType, attacker, defender, recordAbilities, flags);
		
		if (resetFlags && affectedBy & (TYPE_AFFECTED_LEVITATE | TYPE_AFFECTED_WONDER_GUARD))
		{
			gLastLandedMoves[defender] = 0;
			gLastHitByType[defender] = 0;
		}
	}
	return affectedBy;
}

u16 AI_TypeCalc(u16 move, u16 targetSpecies, u16 targetAbility)
{
    u8 moveType = gBattleMoves[move].type;
	u16 flags = 0;

    if (move != MOVE_STRUGGLE)
	{
		MulByTypeEffectiveness(move, moveType, 0, 0, gBaseStats[targetSpecies].type1, &flags);
		
		if (gBaseStats[targetSpecies].type1 != gBaseStats[targetSpecies].type2)
			MulByTypeEffectiveness(move, moveType, 0, 0, gBaseStats[targetSpecies].type2, &flags);
		
		if (moveType == TYPE_GROUND && targetAbility == ABILITY_LEVITATE)
			flags |= (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE);
		if (targetAbility == ABILITY_WONDER_GUARD && gBattleMoves[move].power)
			flags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
	}
	return flags;
}

bool8 SubsBlockMove(u8 attacker, u8 defender, u16 move) // Check if substitute can block the move
{
	if ((!(gBattleMons[defender].status2 & STATUS2_SUBSTITUTE) || gBattleMoves[move].flags & FLAG_SOUND || (GetBattlerAbility(attacker) == ABILITY_INFILTRATOR && move != MOVE_TRANSFORM && move != MOVE_SKY_DROP)))
		return FALSE;
	else
		return TRUE;
}

static void atk07_adjustdamage(void) // Check for effects that prevent the target from faint
{
    u8 holdEffect = GetBattlerItemHoldEffect(gBattlerTarget, TRUE), param = GetBattlerHoldEffectParam(gBattlerTarget);
	
    gPotentialItemEffectBattler = gBattlerTarget;
	
	// Check Focus Band
    if (holdEffect == HOLD_EFFECT_FOCUS_BAND && (Random() % 100) < param)
    {
        RecordItemEffectBattle(gBattlerTarget, holdEffect);
        gSpecialStatuses[gBattlerTarget].focusBanded = 1;
    }
	// Effect that cause the target to survive with 1 HP
    if (gBattleMons[gBattlerTarget].hp <= gBattleMoveDamage && !SubsBlockMove(gBattlerAttacker, gBattlerTarget, gCurrentMove)
	&& ((gBattleMoves[gCurrentMove].effect == EFFECT_FALSE_SWIPE || gProtectStructs[gBattlerTarget].endured
	|| gSpecialStatuses[gBattlerTarget].focusBanded) || (GetBattlerAbility(gBattlerTarget) == ABILITY_STURDY
	&& gBattleMons[gBattlerTarget].hp == gBattleMons[gBattlerTarget].maxHP)))
    {
        gBattleMoveDamage = gBattleMons[gBattlerTarget].hp - 1; // Survive with 1 HP
	    
        if (gProtectStructs[gBattlerTarget].endured)
            gMoveResultFlags |= MOVE_RESULT_FOE_ENDURED;
        else if (gSpecialStatuses[gBattlerTarget].focusBanded)
        {
            gMoveResultFlags |= MOVE_RESULT_FOE_HUNG_ON;
            gLastUsedItem = gBattleMons[gBattlerTarget].item;
        }
		else if (GetBattlerAbility(gBattlerTarget) == ABILITY_STURDY)
		{
			gLastUsedAbility = ABILITY_STURDY;
			RecordAbilityBattle(gBattlerTarget, gLastUsedAbility);
			gSpecialStatuses[gBattlerTarget].sturdied = 1;
			gMoveResultFlags |= MOVE_RESULT_FOE_STURDIED;
		}
    }
    ++gBattlescriptCurrInstr;
}

static void atk08_nop(void)
{
	++gBattlescriptCurrInstr;
}

static void atk09_attackanimation(void)
{
    u8 i;	
	
    if (!gBattleControllerExecFlags)
    {
        if ((gHitMarker & HITMARKER_NO_ANIMATIONS) && (gCurrentMove != MOVE_TRANSFORM && gCurrentMove != MOVE_SUBSTITUTE))
        {
            BattleScriptPush(gBattlescriptCurrInstr + 1);
            gBattlescriptCurrInstr = BattleScript_Pausex20;
            ++gBattleScripting.animTurn;
            ++gBattleScripting.animTargetsHit;
        }
        else
        {
            if (gBattleMoves[gCurrentMove].target & (MOVE_TARGET_BOTH | MOVE_TARGET_FOES_AND_ALLY | MOVE_TARGET_DEPENDS) && gBattleScripting.animTargetsHit)
            {
                ++gBattlescriptCurrInstr;
                return;
            }
            if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
            {
#if DISAPPEAR_HP_BAR_DURING_ANIMS
                for (i = 0; i < MAX_BATTLERS_COUNT; i++)
				{
					if (IsBattlerAlive(i))
						SetHealthboxSpriteInvisible(gHealthboxSpriteIds[i]);
				}
#endif
                gActiveBattler = gBattlerAttacker;
                BtlController_EmitMoveAnimation(0, gCurrentMove, gBattleScripting.animTurn, gBattleMovePower, gBattleMoveDamage, gBattleMons[gBattlerAttacker].friendship, &gDisableStructs[gBattlerAttacker]);
                ++gBattleScripting.animTurn;
                ++gBattleScripting.animTargetsHit;
                MarkBattlerForControllerExec(gBattlerAttacker);
                ++gBattlescriptCurrInstr;
            }
            else
            {
                BattleScriptPush(gBattlescriptCurrInstr + 1);
                gBattlescriptCurrInstr = BattleScript_Pausex20;
            }
        }
    }
}

static void atk0A_waitanimation(void)
{
    u8 i;
	
    if (!gBattleControllerExecFlags)
    {
#if DISAPPEAR_HP_BAR_DURING_ANIMS
	    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
	    {
		    if (IsBattlerAlive(i))
			    SetHealthboxSpriteVisible(gHealthboxSpriteIds[i]);
	    }
#endif
        ++gBattlescriptCurrInstr;
    }
}

static void atk0B_healthbarupdate(void)
{
    s16 healthValue;
    s32 currDmg, maxPossibleDmgValue;
	
    if (!gBattleControllerExecFlags)
    {
        if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
        {
            gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

            if (SubsBlockMove(gBattlerAttacker, gActiveBattler, gCurrentMove) && gDisableStructs[gActiveBattler].substituteHP && !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE))
                PrepareStringBattle(STRINGID_SUBSTITUTEDAMAGED, gActiveBattler); // Hit the substitute
            else
            {
                currDmg = gBattleMoveDamage;
                maxPossibleDmgValue = 10000; // Not present in R/S, ensures that huge damage values don't change sign
                
				// currDmg can't be higher than maxPossibleDmgValue
                if (currDmg <= maxPossibleDmgValue)
                    healthValue = currDmg;
                else
                    healthValue = maxPossibleDmgValue;
		    
                BtlController_EmitHealthBarUpdate(0, healthValue);
                MarkBattlerForControllerExec(gActiveBattler);
		    
                if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER && gBattleMoveDamage > 0)
                    gBattleResults.playerMonWasDamaged = TRUE;
            }
        }
        gBattlescriptCurrInstr += 2;
    }
}

static void atk0C_datahpupdate(void)
{
    u8 moveType;
    
    if (!gBattleControllerExecFlags)
    {
		moveType = gBattleStruct->dynamicMoveType;
		gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	    
        if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
        {
            if (SubsBlockMove(gBattlerAttacker, gActiveBattler, gCurrentMove) && gDisableStructs[gActiveBattler].substituteHP && !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE))
            {
                if (gDisableStructs[gActiveBattler].substituteHP >= gBattleMoveDamage)
                {
                    if (gSpecialStatuses[gActiveBattler].dmg == 0)
                        gSpecialStatuses[gActiveBattler].dmg = gBattleMoveDamage;
                    gDisableStructs[gActiveBattler].substituteHP -= gBattleMoveDamage;
                    gHpDealt = gBattleMoveDamage;
                }
                else
                {
                    if (gSpecialStatuses[gActiveBattler].dmg == 0)
                        gSpecialStatuses[gActiveBattler].dmg = gDisableStructs[gActiveBattler].substituteHP;
                    gHpDealt = gDisableStructs[gActiveBattler].substituteHP;
                    gDisableStructs[gActiveBattler].substituteHP = 0;
                }
                // Check substitute fading
                if (gDisableStructs[gActiveBattler].substituteHP == 0)
                {
                    gBattlescriptCurrInstr += 2;
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_SubstituteFade;
                    return;
                }
            }
            else
            {
                gHitMarker &= ~(HITMARKER_IGNORE_SUBSTITUTE);
                if (gBattleMoveDamage < 0) // HP goes up
                {
                    gBattleMons[gActiveBattler].hp -= gBattleMoveDamage;
                    if (gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP)
                        gBattleMons[gActiveBattler].hp = gBattleMons[gActiveBattler].maxHP;

                }
                else // HP goes down
                {
					if (gHitMarker & HITMARKER_x20)
                        gHitMarker &= ~(HITMARKER_x20);
                    else
                    {
                        gTakenDmg[gActiveBattler] += gBattleMoveDamage;
                        if (gBattlescriptCurrInstr[1] == BS_TARGET)
                            gTakenDmgByBattler[gActiveBattler] = gBattlerAttacker;
                        else
                            gTakenDmgByBattler[gActiveBattler] = gBattlerTarget;
                    }

                    if (gBattleMons[gActiveBattler].hp > gBattleMoveDamage)
                    {
                        gBattleMons[gActiveBattler].hp -= gBattleMoveDamage;
                        gHpDealt = gBattleMoveDamage;
                    }
                    else
                    {
                        gHpDealt = gBattleMons[gActiveBattler].hp;
                        gBattleMons[gActiveBattler].hp = 0;
                    }
                    if (!gSpecialStatuses[gActiveBattler].dmg && !(gHitMarker & HITMARKER_PASSIVE_DAMAGE))
                        gSpecialStatuses[gActiveBattler].dmg = gHpDealt;
                    if (IS_MOVE_PHYSICAL(gCurrentMove) && !(gHitMarker & HITMARKER_PASSIVE_DAMAGE) && gCurrentMove != MOVE_PAIN_SPLIT)
                    {
                        gProtectStructs[gActiveBattler].physicalDmg = gHpDealt;
                        gSpecialStatuses[gActiveBattler].physicalDmg = gHpDealt;
			    
                        if (gBattlescriptCurrInstr[1] == BS_TARGET)
                        {
                            gProtectStructs[gActiveBattler].physicalBattlerId = gBattlerAttacker;
                            gSpecialStatuses[gActiveBattler].physicalBattlerId = gBattlerAttacker;
                        }
                        else
                        {
                            gProtectStructs[gActiveBattler].physicalBattlerId = gBattlerTarget;
                            gSpecialStatuses[gActiveBattler].physicalBattlerId = gBattlerTarget;
                        }
                    }
                    else if (IS_MOVE_SPECIAL(gCurrentMove) && !(gHitMarker & HITMARKER_PASSIVE_DAMAGE))
                    {
                        gProtectStructs[gActiveBattler].specialDmg = gHpDealt;
                        gSpecialStatuses[gActiveBattler].specialDmg = gHpDealt;
			    
                        if (gBattlescriptCurrInstr[1] == BS_TARGET)
                        {
                            gProtectStructs[gActiveBattler].specialBattlerId = gBattlerAttacker;
                            gSpecialStatuses[gActiveBattler].specialBattlerId = gBattlerAttacker;
                        }
                        else
                        {
                            gProtectStructs[gActiveBattler].specialBattlerId = gBattlerTarget;
                            gSpecialStatuses[gActiveBattler].specialBattlerId = gBattlerTarget;
                        }
                    }
                }
                gHitMarker &= ~(HITMARKER_PASSIVE_DAMAGE);
                BtlController_EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].hp);
                MarkBattlerForControllerExec(gActiveBattler);
            }
        }
        else
        {
            if (gSpecialStatuses[gActiveBattler].dmg == 0)
                gSpecialStatuses[gActiveBattler].dmg = 0xFFFF;
        }
        gBattlescriptCurrInstr += 2;
    }
}

static void atk0D_critmessage(void)
{
    if (!gBattleControllerExecFlags)
    {
        if (gCritMultiplier == 2 && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
        {
            PrepareStringBattle(STRINGID_CRITICALHIT, gBattlerAttacker);
            gBattleCommunication[MSG_DISPLAY] = 1;
        }
        ++gBattlescriptCurrInstr;
    }
}

static void atk0E_effectivenesssound(void)
{
    if (!gBattleControllerExecFlags)
    {
        gActiveBattler = gBattlerTarget;
	    
        if (!(gMoveResultFlags & MOVE_RESULT_MISSED))
        {
            switch (gMoveResultFlags & (u8)(~(MOVE_RESULT_MISSED)))
            {
				case MOVE_RESULT_SUPER_EFFECTIVE:
				    BtlController_EmitPlaySE(0, SE_SUPER_EFFECTIVE);
					MarkBattlerForControllerExec(gActiveBattler);
					break;
				case MOVE_RESULT_NOT_VERY_EFFECTIVE:
                    BtlController_EmitPlaySE(0, SE_NOT_EFFECTIVE);
					MarkBattlerForControllerExec(gActiveBattler);
					break;
				case MOVE_RESULT_DOESNT_AFFECT_FOE:
				case MOVE_RESULT_FAILED: // no sound
                    break;
				case MOVE_RESULT_FOE_ENDURED:
				case MOVE_RESULT_ONE_HIT_KO:
				case MOVE_RESULT_FOE_HUNG_ON:
				case MOVE_RESULT_FOE_STURDIED:
				default:
				    if (gMoveResultFlags & MOVE_RESULT_SUPER_EFFECTIVE)
					{
						BtlController_EmitPlaySE(0, SE_SUPER_EFFECTIVE);
						MarkBattlerForControllerExec(gActiveBattler);
					}
					else if (gMoveResultFlags & MOVE_RESULT_NOT_VERY_EFFECTIVE)
					{
						BtlController_EmitPlaySE(0, SE_NOT_EFFECTIVE);
						MarkBattlerForControllerExec(gActiveBattler);
					}
					else if (!(gMoveResultFlags & (MOVE_RESULT_DOESNT_AFFECT_FOE | MOVE_RESULT_FAILED)))
					{
						BtlController_EmitPlaySE(0, SE_EFFECTIVE);
						MarkBattlerForControllerExec(gActiveBattler);
					}
                break;
            }
        }
        ++gBattlescriptCurrInstr;
    }
}

static void atk0F_resultmessage(void)
{
    u32 stringId = 0;

    if (!gBattleControllerExecFlags)
    {
        if (gMoveResultFlags & MOVE_RESULT_MISSED && (!(gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE) || gBattleCommunication[6] > 2))
        {
            stringId = gMissStringIds[gBattleCommunication[6]];
            gBattleCommunication[MSG_DISPLAY] = 1;
        }
        else
        {
            gBattleCommunication[MSG_DISPLAY] = 1;
			
            switch (gMoveResultFlags & (~MOVE_RESULT_MISSED))
            {
            case MOVE_RESULT_SUPER_EFFECTIVE:
                stringId = STRINGID_SUPEREFFECTIVE;
                break;
            case MOVE_RESULT_NOT_VERY_EFFECTIVE:
                stringId = STRINGID_NOTVERYEFFECTIVE;
                break;
            case MOVE_RESULT_ONE_HIT_KO:
                stringId = STRINGID_ONEHITKO;
                break;
            case MOVE_RESULT_FAILED:
                stringId = STRINGID_BUTITFAILED;
                break;
            case MOVE_RESULT_DOESNT_AFFECT_FOE:
                stringId = STRINGID_ITDOESNTAFFECT;
                break;
            case MOVE_RESULT_FOE_HUNG_ON:
                gLastUsedItem = gBattleMons[gBattlerTarget].item;
                gPotentialItemEffectBattler = gBattlerTarget;
                gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_HangedOnMsg;
                return;
            default:
                if (gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE)
                    stringId = STRINGID_ITDOESNTAFFECT;
                else if (gMoveResultFlags & MOVE_RESULT_FAILED)
					stringId = STRINGID_BUTITFAILED;
                else if (gMoveResultFlags & MOVE_RESULT_ONE_HIT_KO)
                {
                    gMoveResultFlags &= ~(MOVE_RESULT_ONE_HIT_KO);
                    gMoveResultFlags &= ~(MOVE_RESULT_SUPER_EFFECTIVE);
                    gMoveResultFlags &= ~(MOVE_RESULT_NOT_VERY_EFFECTIVE);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_OneHitKOMsg;
                    return;
                }
				else if (gMoveResultFlags & MOVE_RESULT_FOE_STURDIED)
				{
					gMoveResultFlags &= ~(MOVE_RESULT_FOE_STURDIED | MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
					gSpecialStatuses[gBattlerTarget].sturdied = 0;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_SturdiedMsg;
					return;
				}
                else if (gMoveResultFlags & MOVE_RESULT_FOE_ENDURED)
                {
                    gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_EnduredMsg;
                    return;
                }
                else if (gMoveResultFlags & MOVE_RESULT_FOE_HUNG_ON)
                {
                    gLastUsedItem = gBattleMons[gBattlerTarget].item;
                    gPotentialItemEffectBattler = gBattlerTarget;
                    gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_HangedOnMsg;
                    return;
                }
                else
                    gBattleCommunication[MSG_DISPLAY] = 0;
            }
        }
        if (stringId)
            PrepareStringBattle(stringId, gBattlerAttacker);
        ++gBattlescriptCurrInstr;
    }
}

static void atk10_printstring(void)
{
    if (!gBattleControllerExecFlags)
    {
        u16 var = T2_READ_16(gBattlescriptCurrInstr + 1);

        gBattlescriptCurrInstr += 3;
        PrepareStringBattle(var, gBattlerAttacker);
        gBattleCommunication[MSG_DISPLAY] = 1;
    }
}

static void atk11_printselectionstring(void)
{
    gActiveBattler = gBattlerAttacker;
    BtlController_EmitPrintSelectionString(0, T2_READ_16(gBattlescriptCurrInstr + 1));
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 3;
    gBattleCommunication[MSG_DISPLAY] = 1;
}

static void atk12_waitmessage(void)
{
    if (!gBattleControllerExecFlags)
    {
        if (!gBattleCommunication[MSG_DISPLAY])
            gBattlescriptCurrInstr += 3;
        else
        {
            u16 toWait = T2_READ_16(gBattlescriptCurrInstr + 1);

            if (++gPauseCounterBattle >= toWait)
            {
                gPauseCounterBattle = 0;
                gBattlescriptCurrInstr += 3;
                gBattleCommunication[MSG_DISPLAY] = 0;
            }
        }
    }
}

static void atk13_printfromtable(void)
{
    if (!gBattleControllerExecFlags)
    {
        const u16 *ptr = (const u16 *) T1_READ_PTR(gBattlescriptCurrInstr + 1);
        ptr += gBattleCommunication[MULTISTRING_CHOOSER];
		
		gBattlescriptCurrInstr += 5;
        PrepareStringBattle(*ptr, gBattlerAttacker);
        gBattleCommunication[MSG_DISPLAY] = 1;
    }
}

static void atk14_printselectionstringfromtable(void)
{
    if (!gBattleControllerExecFlags)
    {
        const u16 *ptr = (const u16 *) T1_READ_PTR(gBattlescriptCurrInstr + 1);

        ptr += gBattleCommunication[MULTISTRING_CHOOSER];
        gActiveBattler = gBattlerAttacker;
        BtlController_EmitPrintSelectionString(0, *ptr);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 5;
        gBattleCommunication[MSG_DISPLAY] = 1;
    }
}

u8 GetBattlerTurnOrderNum(u8 battlerId)
{
    s32 i;
    
    for (i = 0; i < gBattlersCount && gBattlerByTurnOrder[i] != battlerId; ++i) {};
	
	return i;
}

#define SAFEGUARD_CHECK                                \
{                                                      \
	if (gHitMarker & HITMARKER_IGNORE_SAFEGUARD)       \
	{                                                  \
		gBattleCommunication[MULTISTRING_CHOOSER] = 1; \
		gHitMarker &= ~(HITMARKER_IGNORE_SAFEGUARD);   \
	}                                                  \
	else                                               \
		gBattleCommunication[MULTISTRING_CHOOSER] = 0; \
}

void SetMoveEffect(bool8 primary, u8 certain)
{
    u8 side, affectsUser = 0; // 0x40 otherwise
    u16 PayDay;
    bool32 statusChanged = FALSE;

    // Check if the effect affects the user
    if (gBattleCommunication[MOVE_EFFECT_BYTE] & MOVE_EFFECT_AFFECTS_USER)
    {
        gEffectBattler = gBattlerAttacker; // BattlerId that effects get applied on
        gBattleCommunication[MOVE_EFFECT_BYTE] &= ~(MOVE_EFFECT_AFFECTS_USER);
        affectsUser = MOVE_EFFECT_AFFECTS_USER;
        gBattleScripting.battler = gBattlerTarget; // Theoretically the attacker
    }
    else
    {
        gEffectBattler = gBattlerTarget;
        gBattleScripting.battler = gBattlerAttacker;
    }
    if (gBattleTypeFlags & BATTLE_TYPE_POKEDUDE && gBattleCommunication[MOVE_EFFECT_BYTE] != 1 && GetBattlerSide(gEffectBattler) == B_SIDE_OPPONENT)
    {
        ++gBattlescriptCurrInstr;
        return;
    }
	// Check Shield Dust
    if (GetBattlerAbility(gEffectBattler) == ABILITY_SHIELD_DUST && !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD) && !primary
	&& gBattleCommunication[MOVE_EFFECT_BYTE] <= MOVE_EFFECT_TRI_ATTACK)
    {
        ++gBattlescriptCurrInstr;
        return;
    }
	// Check Safeguard
    if (gSideStatuses[GET_BATTLER_SIDE(gEffectBattler)] & SIDE_STATUS_SAFEGUARD && !(gHitMarker & HITMARKER_IGNORE_SAFEGUARD) && !primary
	&& gBattleCommunication[MOVE_EFFECT_BYTE] <= MOVE_EFFECT_CONFUSION && GetBattlerAbility(gBattlerAttacker) != ABILITY_INFILTRATOR)
    {
        ++gBattlescriptCurrInstr;
        return;
    }
	// Check if target is alive
    if (!IsBattlerAlive(gEffectBattler) && gBattleCommunication[MOVE_EFFECT_BYTE] != MOVE_EFFECT_PAYDAY && gBattleCommunication[MOVE_EFFECT_BYTE] != MOVE_EFFECT_STEAL_ITEM)
    {
        ++gBattlescriptCurrInstr;
        return;
    }
	// Check Substitute
    if (SubsBlockMove(gBattlerAttacker, gEffectBattler, gCurrentMove) && affectsUser != MOVE_EFFECT_AFFECTS_USER)
    {
        ++gBattlescriptCurrInstr;
        return;
    }
    if (gBattleCommunication[MOVE_EFFECT_BYTE] <= MOVE_EFFECT_TOXIC) // Status change
    {
        switch (sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]])
        {
			case STATUS1_SLEEP:
			    // Check active uproar
				if (GetBattlerAbility(gEffectBattler) != ABILITY_SOUNDPROOF)
					for (gActiveBattler = 0; gActiveBattler < gBattlersCount && !(gBattleMons[gActiveBattler].status2 & STATUS2_UPROAR); ++gActiveBattler);
				else
					gActiveBattler = gBattlersCount;
				
				if (gActiveBattler != gBattlersCount || !CanBePutToSleep(gEffectBattler, FALSE))
					break;
				CancelMultiTurnMoves(gEffectBattler);
				statusChanged = TRUE;
				break;
			case STATUS1_POISON:
			    // Check poison immunity abilities
			    if ((GetBattlerAbility(gEffectBattler) == ABILITY_IMMUNITY || GetBattlerAbility(gEffectBattler) == ABILITY_PASTEL_VEIL) && (primary == TRUE || certain == MOVE_EFFECT_CERTAIN))
				{
					gLastUsedAbility = GetBattlerAbility(gEffectBattler);
					RecordAbilityBattle(gEffectBattler, gLastUsedAbility);
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = BattleScript_PSNPrevention;
					SAFEGUARD_CHECK;
					return;
				}
				if (CanBePoisoned(gEffectBattler, gBattlerAttacker, FALSE))
					statusChanged = TRUE;
				break;
			case STATUS1_TOXIC_POISON:
			    // Check poison immunity abilities
			    if ((GetBattlerAbility(gEffectBattler) == ABILITY_IMMUNITY || GetBattlerAbility(gEffectBattler) == ABILITY_PASTEL_VEIL) && (primary == TRUE || certain == MOVE_EFFECT_CERTAIN))
				{
					gLastUsedAbility = GetBattlerAbility(gEffectBattler);
					RecordAbilityBattle(gEffectBattler, gLastUsedAbility);
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = BattleScript_PSNPrevention;
					SAFEGUARD_CHECK;
					return;
				}
				if (CanBePoisoned(gEffectBattler, gBattlerAttacker, FALSE))
					statusChanged = TRUE;
				else
					gMoveResultFlags |= MOVE_RESULT_DOESNT_AFFECT_FOE;
				break;
			case STATUS1_BURN:
			    // Check burn immunity abilities
				if ((GetBattlerAbility(gEffectBattler) == ABILITY_WATER_VEIL || GetBattlerAbility(gEffectBattler) == ABILITY_WATER_BUBBLE) && (primary == TRUE || certain == MOVE_EFFECT_CERTAIN))
				{
					gLastUsedAbility = GetBattlerAbility(gEffectBattler);
					RecordAbilityBattle(gEffectBattler, gLastUsedAbility);
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = BattleScript_BRNPrevention;
					SAFEGUARD_CHECK;
					return;
				}
				if (CanBeBurned(gEffectBattler, FALSE))
					statusChanged = TRUE;
				break;
			case STATUS1_FREEZE:
			    if (CanBeFrozen(gEffectBattler, FALSE))
				{
					CancelMultiTurnMoves(gEffectBattler);
					statusChanged = TRUE;
				}
				break;
			case STATUS1_PARALYSIS:
			    // Check paralysis immunity abilities
				if (GetBattlerAbility(gEffectBattler) == ABILITY_LIMBER)
				{
					if (primary == TRUE || certain == MOVE_EFFECT_CERTAIN)
					{
						gLastUsedAbility = GetBattlerAbility(gEffectBattler);
						RecordAbilityBattle(gEffectBattler, gLastUsedAbility);
						BattleScriptPush(gBattlescriptCurrInstr + 1);
						gBattlescriptCurrInstr = BattleScript_PRLZPrevention;
						SAFEGUARD_CHECK;
						return;
					}
					else
						break;
				}
				if (CanBeParalyzed(gEffectBattler, FALSE))
					statusChanged = TRUE;
				break;
		}
        if (statusChanged == TRUE)
        {
            BattleScriptPush(gBattlescriptCurrInstr + 1);
			
            if (sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]] == STATUS1_SLEEP) // Set sleep turns
#if SLEEP_UPDATE
                gBattleMons[gEffectBattler].status1 |= ((Random() & 2) + 1);
#else
	            gBattleMons[gEffectBattler].status1 |= ((Random() & 3) + 2);
#endif
            else
                gBattleMons[gEffectBattler].status1 |= sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]];
			
            gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
            gActiveBattler = gEffectBattler;
            BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gEffectBattler].status1);
            MarkBattlerForControllerExec(gActiveBattler);
            // For synchronize
            if (gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_POISON
             || gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_TOXIC
             || gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_PARALYSIS
             || gBattleCommunication[MOVE_EFFECT_BYTE] == MOVE_EFFECT_BURN)
             {
                gBattleStruct->synchronizeMoveEffect = gBattleCommunication[MOVE_EFFECT_BYTE];
                gHitMarker |= HITMARKER_SYNCHRONISE_EFFECT;
             }
        }
        else if (statusChanged == FALSE)
            ++gBattlescriptCurrInstr;
        return;
    }
    else
    {
        if (gBattleMons[gEffectBattler].status2 & sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]])
            ++gBattlescriptCurrInstr;
        else
        {
            switch (gBattleCommunication[MOVE_EFFECT_BYTE])
            {
				case MOVE_EFFECT_CONFUSION:
				    if (GetBattlerAbility(gEffectBattler) == ABILITY_OWN_TEMPO || gBattleMons[gEffectBattler].status2 & STATUS2_CONFUSION)
						++gBattlescriptCurrInstr;
					else
					{
						gBattleMons[gEffectBattler].status2 |= (((Random()) % 0x4)) + 2;
						BattleScriptPush(gBattlescriptCurrInstr + 1);
						gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
					}
					break;
				case MOVE_EFFECT_FLINCH:
				    if (GetBattlerAbility(gEffectBattler) == ABILITY_INNER_FOCUS)
					{
						if (primary == TRUE || certain == MOVE_EFFECT_CERTAIN)
						{
							gLastUsedAbility = ABILITY_INNER_FOCUS;
							RecordAbilityBattle(gEffectBattler, ABILITY_INNER_FOCUS);
							gBattlescriptCurrInstr = BattleScript_FlinchPrevention;
						}
						else
							++gBattlescriptCurrInstr;
					}
					else
					{
						if (GetBattlerTurnOrderNum(gEffectBattler) > gCurrentTurnActionNumber)
							gBattleMons[gEffectBattler].status2 |= sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]];
						++gBattlescriptCurrInstr;
					}
					break;
				case MOVE_EFFECT_UPROAR:
				    if (!(gBattleMons[gEffectBattler].status2 & STATUS2_UPROAR))
					{
						gBattleMons[gEffectBattler].status2 |= STATUS2_MULTIPLETURNS;
						gBattleMons[gEffectBattler].status2 |= ((Random() & 3) + 2) << 4;
						gLockedMoves[gEffectBattler] = gCurrentMove;
						BattleScriptPush(gBattlescriptCurrInstr + 1);
						gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
					}
					else
						++gBattlescriptCurrInstr;
					break;
				case MOVE_EFFECT_PAYDAY:
				    if (GET_BATTLER_SIDE(gBattlerAttacker) == B_SIDE_PLAYER)
					{
						PayDay = gPaydayMoney;
						gPaydayMoney += (gBattleMons[gBattlerAttacker].level * 5);
						if (PayDay > gPaydayMoney)
							gPaydayMoney = 0xFFFF;
					}
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
					break;
				case MOVE_EFFECT_TRI_ATTACK:
				    if (gBattleMons[gEffectBattler].status1)
						++gBattlescriptCurrInstr;
					else
					{
						gBattleCommunication[MOVE_EFFECT_BYTE] = Random() % 3 + 3;
						SetMoveEffect(FALSE, 0);
					}
					break;
				case MOVE_EFFECT_CHARGING:
				    gBattleMons[gEffectBattler].status2 |= STATUS2_MULTIPLETURNS;
					gLockedMoves[gEffectBattler] = gCurrentMove;
					gProtectStructs[gEffectBattler].chargingTurn = 1;
					++gBattlescriptCurrInstr;
					break;
				case MOVE_EFFECT_WRAP:
				    if (gBattleMons[gEffectBattler].status2 & STATUS2_WRAPPED)
						++gBattlescriptCurrInstr;
					else
					{
						gBattleMons[gEffectBattler].status2 |= ((Random() & 3) + 3) << 0xD;
						*(gBattleStruct->wrappedMove + gEffectBattler * 2 + 0) = gCurrentMove;
						*(gBattleStruct->wrappedMove + gEffectBattler * 2 + 1) = gCurrentMove >> 8;
						*(gBattleStruct->wrappedBy + gEffectBattler) = gBattlerAttacker;
						BattleScriptPush(gBattlescriptCurrInstr + 1);
						gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
						
						for (gBattleCommunication[MULTISTRING_CHOOSER] = 0; ; ++gBattleCommunication[MULTISTRING_CHOOSER])
						{
							if (gBattleCommunication[MULTISTRING_CHOOSER] > 4 || gTrappingMoves[gBattleCommunication[MULTISTRING_CHOOSER]] == gCurrentMove)
								break;
						}
					}
					break;
				case MOVE_EFFECT_RECOIL_25: // 25% recoil
				    gBattleMoveDamage = (gHpDealt) / 4;
					
					if (gBattleMoveDamage == 0)
						gBattleMoveDamage = 1;
					
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
					break;
				case MOVE_EFFECT_ATK_PLUS_1:
				case MOVE_EFFECT_DEF_PLUS_1:
				case MOVE_EFFECT_SPD_PLUS_1:
				case MOVE_EFFECT_SP_ATK_PLUS_1:
				case MOVE_EFFECT_SP_DEF_PLUS_1:
				case MOVE_EFFECT_ACC_PLUS_1:
				case MOVE_EFFECT_EVS_PLUS_1:
                    if (NoAliveMonsForEitherParty() || ChangeStatBuffs(SET_STAT_BUFF_VALUE(1), gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_PLUS_1 + 1, affectsUser, NULL))
						++gBattlescriptCurrInstr;
					else
					{
						gBattleScripting.animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
						gBattleScripting.animArg2 = 0;
						BattleScriptPush(gBattlescriptCurrInstr + 1);
						gBattlescriptCurrInstr = BattleScript_StatUp;
					}
					break;
				case MOVE_EFFECT_ATK_MINUS_1:
				case MOVE_EFFECT_DEF_MINUS_1:
				case MOVE_EFFECT_SPD_MINUS_1:
				case MOVE_EFFECT_SP_ATK_MINUS_1:
				case MOVE_EFFECT_SP_DEF_MINUS_1:
				case MOVE_EFFECT_ACC_MINUS_1:
				case MOVE_EFFECT_EVS_MINUS_1:
				    if (NoAliveMonsForEitherParty() || ChangeStatBuffs(SET_STAT_BUFF_VALUE(1) | STAT_BUFF_NEGATIVE, gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_MINUS_1 + 1, affectsUser, NULL))
						++gBattlescriptCurrInstr;
					else
					{
						gBattleScripting.animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
						gBattleScripting.animArg2 = 0;
						BattleScriptPush(gBattlescriptCurrInstr + 1);
						gBattlescriptCurrInstr = BattleScript_StatDown;
					}
					break;
				case MOVE_EFFECT_ATK_PLUS_2:
				case MOVE_EFFECT_DEF_PLUS_2:
				case MOVE_EFFECT_SPD_PLUS_2:
				case MOVE_EFFECT_SP_ATK_PLUS_2:
				case MOVE_EFFECT_SP_DEF_PLUS_2:
				case MOVE_EFFECT_ACC_PLUS_2:
				case MOVE_EFFECT_EVS_PLUS_2:
				    if (NoAliveMonsForEitherParty() || ChangeStatBuffs(SET_STAT_BUFF_VALUE(2), gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_PLUS_2 + 1, affectsUser, NULL))
						++gBattlescriptCurrInstr;
					else
					{
						gBattleScripting.animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
						gBattleScripting.animArg2 = 0;
						BattleScriptPush(gBattlescriptCurrInstr + 1);
						gBattlescriptCurrInstr = BattleScript_StatUp;
					}
					break;
				case MOVE_EFFECT_ATK_MINUS_2:
				case MOVE_EFFECT_DEF_MINUS_2:
				case MOVE_EFFECT_SPD_MINUS_2:
				case MOVE_EFFECT_SP_ATK_MINUS_2:
				case MOVE_EFFECT_SP_DEF_MINUS_2:
				case MOVE_EFFECT_ACC_MINUS_2:
				case MOVE_EFFECT_EVS_MINUS_2:
				    if (NoAliveMonsForEitherParty() || ChangeStatBuffs(SET_STAT_BUFF_VALUE(2) | STAT_BUFF_NEGATIVE, gBattleCommunication[MOVE_EFFECT_BYTE] - MOVE_EFFECT_ATK_MINUS_2 + 1, affectsUser, NULL))
						++gBattlescriptCurrInstr;
					else
					{
						gBattleScripting.animArg1 = gBattleCommunication[MOVE_EFFECT_BYTE] & ~(MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN);
						gBattleScripting.animArg2 = 0;
						BattleScriptPush(gBattlescriptCurrInstr + 1);
						gBattlescriptCurrInstr = BattleScript_StatDown;
					}
					break;
				case MOVE_EFFECT_RECHARGE:
				    gBattleMons[gEffectBattler].status2 |= STATUS2_RECHARGE;
					gDisableStructs[gEffectBattler].rechargeTimer = 2;
					gLockedMoves[gEffectBattler] = gCurrentMove;
					++gBattlescriptCurrInstr;
					break;
				case MOVE_EFFECT_RAGE:
				    gBattleMons[gBattlerAttacker].status2 |= STATUS2_RAGE;
					++gBattlescriptCurrInstr;
					break;
				case MOVE_EFFECT_STEAL_ITEM:
				{
                    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                    {
                        ++gBattlescriptCurrInstr;
                        break;
                    }
                    side = GetBattlerSide(gBattlerAttacker);
			
                    if (side == B_SIDE_OPPONENT && !(gBattleTypeFlags & (BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_LINK))
                     && gTrainerBattleOpponent_A != 0x400)
                        ++gBattlescriptCurrInstr;
                    else if (!(gBattleTypeFlags & (BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_LINK)) && gTrainerBattleOpponent_A != 0x400
			     && (gWishFutureKnock.knockedOffMons[side] & gBitTable[gBattlerPartyIndexes[gBattlerAttacker]]))
                        ++gBattlescriptCurrInstr;
                    else if (gBattleMons[gBattlerTarget].item && GetBattlerAbility(gBattlerTarget) == ABILITY_STICKY_HOLD)
                    {
                        gBattlescriptCurrInstr = BattleScript_StickyHoldActivates;
                        gLastUsedAbility = gBattleMons[gBattlerTarget].ability;
                        RecordAbilityBattle(gBattlerTarget, gLastUsedAbility);
                    }
                    else if (gBattleMons[gBattlerAttacker].item != ITEM_NONE || gBattleMons[gBattlerTarget].item == ITEM_ENIGMA_BERRY
                          || IS_ITEM_MAIL(gBattleMons[gBattlerTarget].item) || gBattleMons[gBattlerTarget].item == ITEM_NONE)
                        ++gBattlescriptCurrInstr;
                    else
                    {
                        gLastUsedItem = gBattleStruct->changedItems[gBattlerAttacker] = gBattleMons[gBattlerTarget].item;
                        gBattleMons[gBattlerTarget].item = ITEM_NONE;
						TryGiveUnburdenBoostToMon(gBattlerTarget); // Try give target Unburden boost
						TryGiveUnburdenBoostToMon(gBattlerAttacker); // Try remove attacker's Unburden boost
                        gActiveBattler = gBattlerAttacker;
                        BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gLastUsedItem);
                        MarkBattlerForControllerExec(gBattlerAttacker);
                        gActiveBattler = gBattlerTarget;
                        BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gBattlerTarget].item);
                        MarkBattlerForControllerExec(gBattlerTarget);
                        BattleScriptPush(gBattlescriptCurrInstr + 1);
                        gBattlescriptCurrInstr = BattleScript_ItemSteal;
                        *(u8 *)((u8 *)(&gBattleStruct->choicedMove[gBattlerTarget]) + 0) = 0;
                        *(u8 *)((u8 *)(&gBattleStruct->choicedMove[gBattlerTarget]) + 1) = 0;
                    }
                }
                break;
				case MOVE_EFFECT_PREVENT_ESCAPE:
                    gBattleMons[gBattlerTarget].status2 |= STATUS2_ESCAPE_PREVENTION;
					gDisableStructs[gBattlerTarget].battlerPreventingEscape = gBattlerAttacker;
					++gBattlescriptCurrInstr;
					break;
				case MOVE_EFFECT_NIGHTMARE:
				    gBattleMons[gBattlerTarget].status2 |= STATUS2_NIGHTMARE;
					++gBattlescriptCurrInstr;
					break;
				case MOVE_EFFECT_ALL_STATS_UP:
				    if (!NoAliveMonsForEitherParty())
					{
						BattleScriptPush(gBattlescriptCurrInstr + 1);
						gBattlescriptCurrInstr = BattleScript_AllStatsUp;
					}
					break;
				case MOVE_EFFECT_RAPIDSPIN:
				    BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = BattleScript_RapidSpinAway;
					break;
				case MOVE_EFFECT_REMOVE_PARALYSIS: // Smelling Salts
                    if (!(gBattleMons[gBattlerTarget].status1 & STATUS1_PARALYSIS))
						++gBattlescriptCurrInstr;
					else
					{
						gBattleMons[gBattlerTarget].status1 &= ~(STATUS1_PARALYSIS);
						gActiveBattler = gBattlerTarget;
						BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
						MarkBattlerForControllerExec(gActiveBattler);
						BattleScriptPush(gBattlescriptCurrInstr + 1);
						gBattlescriptCurrInstr = BattleScript_TargetPRLZHeal;
					}
					break;
				case MOVE_EFFECT_ATK_DEF_DOWN: // SuperPower
				    if (!NoAliveMonsForEitherParty())
					{
						BattleScriptPush(gBattlescriptCurrInstr + 1);
						gBattlescriptCurrInstr = BattleScript_AtkDefDown;
					}
					break;
				case MOVE_EFFECT_RECOIL_33: // Double Edge
				    gBattleMoveDamage = gHpDealt / 3;
					
					if (gBattleMoveDamage == 0)
						gBattleMoveDamage = 1;
					
					BattleScriptPush(gBattlescriptCurrInstr + 1);
					gBattlescriptCurrInstr = sMoveEffectBS_Ptrs[gBattleCommunication[MOVE_EFFECT_BYTE]];
					break;
				case MOVE_EFFECT_THRASH:
				    if (gBattleMons[gEffectBattler].status2 & STATUS2_LOCK_CONFUSE)
						++gBattlescriptCurrInstr;
					else
					{
						gBattleMons[gEffectBattler].status2 |= STATUS2_MULTIPLETURNS;
						gBattleMons[gEffectBattler].status2 |= (((Random() & 1) + 2) << 0xA);
						gLockedMoves[gEffectBattler] = gCurrentMove;
					}
					break;
				case MOVE_EFFECT_KNOCK_OFF:
				    if (gBattleMons[gEffectBattler].item)
					{
						if (GetBattlerAbility(gEffectBattler) == ABILITY_STICKY_HOLD)
						{
							gLastUsedAbility = ABILITY_STICKY_HOLD;
							gBattlescriptCurrInstr = BattleScript_StickyHoldActivates;
							RecordAbilityBattle(gEffectBattler, ABILITY_STICKY_HOLD);
						}
						else
						{
							side = GetBattlerSide(gEffectBattler);
							gLastUsedItem = gBattleMons[gEffectBattler].item;
							gBattleMons[gEffectBattler].item = ITEM_NONE;
							TryGiveUnburdenBoostToMon(gEffectBattler); // Try give target unburden boost
							gWishFutureKnock.knockedOffMons[side] |= gBitTable[gBattlerPartyIndexes[gEffectBattler]];
							BattleScriptPush(gBattlescriptCurrInstr + 1);
							gBattlescriptCurrInstr = BattleScript_KnockedOff;
							*(u8 *)((u8 *)(&gBattleStruct->choicedMove[gEffectBattler]) + 0) = 0;
							*(u8 *)((u8 *)(&gBattleStruct->choicedMove[gEffectBattler]) + 1) = 0;
						}
					}
					else
						++gBattlescriptCurrInstr;
					break;
				case MOVE_EFFECT_SP_ATK_TWO_DOWN: // Overheat
			        if (!NoAliveMonsForEitherParty())
					{
						BattleScriptPush(gBattlescriptCurrInstr + 1);
						gBattlescriptCurrInstr = BattleScript_SAtkDown2;
					}
					break;
            }
        }
    }
}

static void atk15_seteffectwithchance(void)
{
    u32 percentChance;
	
	if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && !ReceiveSheerForceBoost(gBattlerAttacker, gCurrentMove))
	{
		percentChance = gBattleMoves[gCurrentMove].secondaryEffectChance;
		
		if (GetBattlerAbility(gBattlerAttacker) == ABILITY_SERENE_GRACE) // Check Serene Grace
			percentChance *= 2;
			
		if (gBattleCommunication[MOVE_EFFECT_BYTE] & MOVE_EFFECT_CERTAIN)
	    {
		    gBattleCommunication[MOVE_EFFECT_BYTE] &= ~(MOVE_EFFECT_CERTAIN);
		    SetMoveEffect(0, MOVE_EFFECT_CERTAIN);
	    }
	    else if (Random() % 100 <= percentChance && gBattleCommunication[MOVE_EFFECT_BYTE])
	    {
		    if (percentChance >= 100)
			    SetMoveEffect(0, MOVE_EFFECT_CERTAIN);
		    else
			    SetMoveEffect(0, 0);
	    }
		else
			++gBattlescriptCurrInstr;
	}
	else
		++gBattlescriptCurrInstr;
    
    gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
    gBattleScripting.multihitMoveEffect = 0;
}

static void atk16_seteffectprimary(void)
{
    SetMoveEffect(TRUE, 0);
}

static void atk17_seteffectsecondary(void)
{
    SetMoveEffect(FALSE, 0);
}

static void atk18_clearstatusfromeffect(void)
{
    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

    if (gBattleCommunication[MOVE_EFFECT_BYTE] <= MOVE_EFFECT_TOXIC)
        gBattleMons[gActiveBattler].status1 &= ~(sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]]);
    else
        gBattleMons[gActiveBattler].status2 &= ~(sStatusFlagsForMoveEffects[gBattleCommunication[MOVE_EFFECT_BYTE]]);
    gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
    gBattlescriptCurrInstr += 2;
    gBattleScripting.multihitMoveEffect = 0;
}

static void atk19_tryfaintmon(void)
{
    u8 battlerId, moveIndex;
    const u8 *BS_ptr;

    if (gBattlescriptCurrInstr[2] != 0)
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
		
        if (gHitMarker & HITMARKER_FAINTED(gActiveBattler))
        {
            BS_ptr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
            BattleScriptPop();
            gBattlescriptCurrInstr = BS_ptr;
            gSideStatuses[GetBattlerSide(gActiveBattler)] &= ~(SIDE_STATUS_SPIKES_DAMAGED);
        }
        else
            gBattlescriptCurrInstr += 7;
    }
    else
    {
        if (gBattlescriptCurrInstr[1] == BS_ATTACKER)
        {
            gActiveBattler = gBattlerAttacker;
            battlerId = gBattlerTarget;
            BS_ptr = BattleScript_FaintAttacker;
        }
        else
        {
            gActiveBattler = gBattlerTarget;
            battlerId = gBattlerAttacker;
            BS_ptr = BattleScript_FaintTarget;
        }
        if (!IsBattlerAlive(gActiveBattler))
        {
            gHitMarker |= HITMARKER_FAINTED(gActiveBattler);
            BattleScriptPush(gBattlescriptCurrInstr + 7);
            gBattlescriptCurrInstr = BS_ptr;
		
            if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
            {
                gHitMarker |= HITMARKER_PLAYER_FAINTED;
                if (gBattleResults.playerFaintCounter < 0xFF)
                    ++gBattleResults.playerFaintCounter;
                AdjustFriendshipOnBattleFaint(gActiveBattler);
            }
            else
            {
                if (gBattleResults.opponentFaintCounter < 0xFF)
                    ++gBattleResults.opponentFaintCounter;
                gBattleResults.lastOpponentSpecies = GetMonData(&gEnemyParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_SPECIES);
                *(u8 *)(&gBattleStruct->field_182) = gBattlerAttacker;
            }
            if ((gHitMarker & HITMARKER_DESTINYBOND) && gBattleMons[gBattlerAttacker].hp != 0)
            {
                gHitMarker &= ~(HITMARKER_DESTINYBOND);
                BattleScriptPush(gBattlescriptCurrInstr);
                gBattleMoveDamage = gBattleMons[battlerId].hp;
                gBattlescriptCurrInstr = BattleScript_DestinyBondTakesLife;
            }
            if ((gStatuses3[gBattlerTarget] & STATUS3_GRUDGE) && !(gHitMarker & HITMARKER_GRUDGE) && gCurrentMove != MOVE_STRUGGLE
			&& GetBattlerSide(gBattlerAttacker) != GetBattlerSide(gBattlerTarget) && gBattleMons[gBattlerAttacker].hp != 0)
            {
                moveIndex = *(gBattleStruct->chosenMovePositions + gBattlerAttacker);

                gBattleMons[gBattlerAttacker].pp[moveIndex] = 0;
                BattleScriptPush(gBattlescriptCurrInstr);
                gBattlescriptCurrInstr = BattleScript_GrudgeTakesPp;
                gActiveBattler = gBattlerAttacker;
                BtlController_EmitSetMonData(0, moveIndex + REQUEST_PPMOVE1_BATTLE, 0, 1, &gBattleMons[gActiveBattler].pp[moveIndex]);
                MarkBattlerForControllerExec(gActiveBattler);
                PREPARE_MOVE_BUFFER(gBattleTextBuff1, gBattleMons[gBattlerAttacker].moves[moveIndex])
            }
        }
        else
            gBattlescriptCurrInstr += 7;
    }
}

static void atk1A_dofaintanimation(void)
{
    if (!gBattleControllerExecFlags)
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
        BtlController_EmitFaintAnimation(0);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 2;
    }
}

static void atk1B_cleareffectsonfaint(void)
{
    if (!gBattleControllerExecFlags)
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
        gBattleMons[gActiveBattler].status1 = 0;
        BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 0x4, &gBattleMons[gActiveBattler].status1);
        MarkBattlerForControllerExec(gActiveBattler);
        FaintClearSetData(); // Effects like attractions, trapping, etc.
		TryDoBattleFormChange(gActiveBattler, BATTLE_FORM_CHANGE_FAINT);
        gBattlescriptCurrInstr += 2;
    }
}

static void atk1C_jumpifstatus(void)
{
    u8 battlerId = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    u32 flags = T2_READ_32(gBattlescriptCurrInstr + 2);
    const u8 *jumpPtr = T2_READ_PTR(gBattlescriptCurrInstr + 6);

    if (gBattleMons[battlerId].status1 & flags && IsBattlerAlive(battlerId))
        gBattlescriptCurrInstr = jumpPtr;
    else
        gBattlescriptCurrInstr += 10;
}

static void atk1D_jumpifstatus2(void)
{
    u8 battlerId = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    u32 flags = T2_READ_32(gBattlescriptCurrInstr + 2);
    const u8 *jumpPtr = T2_READ_PTR(gBattlescriptCurrInstr + 6);

    if (gBattleMons[battlerId].status2 & flags && IsBattlerAlive(battlerId))
        gBattlescriptCurrInstr = jumpPtr;
    else
        gBattlescriptCurrInstr += 10;
}

static void atk1E_jumpifability(void)
{
    u8 battlerId;
    u16 ability = gBattlescriptCurrInstr[2];
    const u8 *jumpPtr = T2_READ_PTR(gBattlescriptCurrInstr + 4);

    if (gBattlescriptCurrInstr[1] == BS_ATTACKER_SIDE)
    {
        battlerId = AbilityBattleEffects(ABILITYEFFECT_CHECK_BATTLER_SIDE, gBattlerAttacker, ability, 0, 0);
	    
        if (battlerId)
        {
            gLastUsedAbility = ability;
            gBattlescriptCurrInstr = jumpPtr;
            RecordAbilityBattle(battlerId - 1, gLastUsedAbility);
            gBattleScripting.battlerWithAbility = battlerId - 1;
			return;
        }
    }
    else if (gBattlescriptCurrInstr[1] == BS_NOT_ATTACKER_SIDE)
    {
        battlerId = AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, gBattlerAttacker, ability, 0, 0);
	    
        if (battlerId)
        {
            gLastUsedAbility = ability;
            gBattlescriptCurrInstr = jumpPtr;
            RecordAbilityBattle(battlerId - 1, gLastUsedAbility);
            gBattleScripting.battlerWithAbility = battlerId - 1;
			return;
        }
    }
    else
    {
        battlerId = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	    
        if (GetBattlerAbility(battlerId) == ability)
        {
            gLastUsedAbility = ability;
            gBattlescriptCurrInstr = jumpPtr;
            RecordAbilityBattle(battlerId, gLastUsedAbility);
            gBattleScripting.battlerWithAbility = battlerId;
			return;
        }
    }
	gBattlescriptCurrInstr += 8;
}

static void atk1F_jumpifsideaffecting(void)
{
    u8 side;
    u16 flags = T2_READ_16(gBattlescriptCurrInstr + 2);
    const u8 *jumpPtr = T2_READ_PTR(gBattlescriptCurrInstr + 4);

    if (gBattlescriptCurrInstr[1] == BS_ATTACKER)
        side = GET_BATTLER_SIDE(gBattlerAttacker);
    else
    {
        side = GET_BATTLER_SIDE(gBattlerTarget);
	
        if (GetBattlerAbility(gBattlerTarget) == ABILITY_INFILTRATOR)
			flags &= ~(SIDE_STATUS_SAFEGUARD | SIDE_STATUS_MIST);
    }

    if (gSideStatuses[side] & flags)
        gBattlescriptCurrInstr = jumpPtr;
    else
        gBattlescriptCurrInstr += 8;
}

static void atk20_jumpifstat(void)
{
    u8 ret = 0;
    u8 battlerId = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    u8 value = gBattleMons[battlerId].statStages[gBattlescriptCurrInstr[3]];
    s8 comparer = gBattlescriptCurrInstr[4];
	
    if (GetBattlerAbility(battlerId) == ABILITY_CONTRARY)
    {
	    comparer -= 12;
	    comparer *= -1;
    }
	
    switch (gBattlescriptCurrInstr[2])
    {
    case CMP_EQUAL:
        if (value == comparer)
            ++ret;
        break;
    case CMP_NOT_EQUAL:
        if (value != comparer)
            ++ret;
        break;
    case CMP_GREATER_THAN:
        if (value > comparer)
            ++ret;
        break;
    case CMP_LESS_THAN:
        if (value < comparer)
            ++ret;
        break;
    case CMP_COMMON_BITS:
        if (value & comparer)
            ++ret;
        break;
    case CMP_NO_COMMON_BITS:
        if (!(value & comparer))
            ++ret;
        break;
    }
    if (ret)
        gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 5);
    else
        gBattlescriptCurrInstr += 9;
}

static void atk21_jumpifstatus3condition(void)
{
    u32 flags;
    const u8 *jumpPtr;

    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    flags = T2_READ_32(gBattlescriptCurrInstr + 2);
    jumpPtr = T2_READ_PTR(gBattlescriptCurrInstr + 7);
	
    if (gBattlescriptCurrInstr[6])
    {
        if (gStatuses3[gActiveBattler] & flags)
            gBattlescriptCurrInstr += 11;
        else
            gBattlescriptCurrInstr = jumpPtr;
    }
    else
    {
        if (gStatuses3[gActiveBattler] & flags)
            gBattlescriptCurrInstr = jumpPtr;
        else
            gBattlescriptCurrInstr += 11;
    }
}

static void atk22_jumpbasedontype(void)
{
    u8 battlerId = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    u8 type = gBattlescriptCurrInstr[2];
    const u8 *jumpPtr = T2_READ_PTR(gBattlescriptCurrInstr + 4);

    if (!gBattlescriptCurrInstr[3]) // jumpiftype
    {
	    if (IS_BATTLER_OF_TYPE(battlerId, type))
		    gBattlescriptCurrInstr = jumpPtr;
	    else
		    gBattlescriptCurrInstr += 8;
    }
    else // jumpifnotype
    {
	    if (!IS_BATTLER_OF_TYPE(battlerId, type))
		    gBattlescriptCurrInstr = jumpPtr;
	    else
		    gBattlescriptCurrInstr += 8;
    }
}

static void atk23_getexp(void)
{
    u8 holdEffect;
    u16 item, calculatedExp;
    u16 *exp = &gBattleStruct->expValue;
    s32 viaSentIn, sentIn, viaExpShare = 0;
    s32 i; // Also used as stringId

    gBattlerFainted = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    sentIn = gSentPokesToOpponent[(gBattlerFainted & 2) >> 1];
	
    switch (gBattleScripting.atk23_state)
    {
		case 0: // Check if should receive exp at all
            if (GetBattlerSide(gBattlerFainted) != B_SIDE_OPPONENT
			|| (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_SAFARI | BATTLE_TYPE_EREADER_TRAINER)))
			    gBattleScripting.atk23_state = 6; // Goto last case
			else
			{
				++gBattleScripting.atk23_state;
				gBattleStruct->givenExpMons |= gBitTable[gBattlerPartyIndexes[gBattlerFainted]];
			}
			break;
		case 1: // Calculate experience points to redistribute
		{
            for (viaSentIn = 0, i = 0; i < PARTY_SIZE; ++i)
            {
                if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) != SPECIES_NONE && GetMonData(&gPlayerParty[i], MON_DATA_HP) != 0)
				{
                    if (gBitTable[i] & sentIn)
                        ++viaSentIn;
                }
            }
            calculatedExp = gBaseStats[gBattleMons[gBattlerFainted].species].expYield * gBattleMons[gBattlerFainted].level / 7;
		
            if (gSaveBlock2Ptr->expShare) // Exp share is turned on
            {
                *exp = SAFE_DIV(calculatedExp / 2, viaSentIn);
				
                if (*exp == 0)
                    *exp = 1;
                viaExpShare = gSaveBlock1Ptr->playerPartyCount;
                gExpShareExp = calculatedExp / 2;
                if (gExpShareExp == 0)
                    gExpShareExp = 1;
            }
            else
            {
#if FULL_EXP_FOR_PARTICIPATING
                *exp = calculatedExp;
#else
		        *exp = SAFE_DIV(calculatedExp, viaSentIn);
#endif
                if (*exp == 0)
                    *exp = 1;
                gExpShareExp = 0;
            }
            ++gBattleScripting.atk23_state;
            gBattleStruct->expGetterMonId = 0;
            gBattleStruct->sentInPokes = sentIn;
        }
        // Fall through
		case 2: // Set exp value to the poke in expgetter_id and print message
			if (!gBattleControllerExecFlags)
			{
				i = GetNumOfBadges();
				item = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_HELD_ITEM);
				
				if (item == ITEM_ENIGMA_BERRY)
					holdEffect = gSaveBlock1Ptr->enigmaBerry.holdEffect;
				else
					holdEffect = ItemId_GetHoldEffect(item);
				
				if (!gSaveBlock2Ptr->expShare && !(gBattleStruct->sentInPokes & 1))
				{
					*(&gBattleStruct->sentInPokes) >>= 1;
					gBattleScripting.atk23_state = 5;
					gBattleMoveDamage = 0; // Used for exp
				}     
				else if (GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_LEVEL) == MAX_LEVEL)
				{
					*(&gBattleStruct->sentInPokes) >>= 1;
					gBattleScripting.atk23_state = 5;
					gBattleMoveDamage = 0; // Used for exp
				}
#if EXP_BLOCK
                else if (i != NUM_BADGES && GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_LEVEL) >= sExpBlockLevels[i])
				{
					*(&gBattleStruct->sentInPokes) >>= 1;
					gBattleScripting.atk23_state = 5;
					gBattleMoveDamage = 0; // Used for exp
				}
#endif    
                else
				{
					// Music change in wild battle after fainting a poke
					if (!(gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_POKEDUDE)) && gBattleMons[0].hp && !gBattleStruct->wildVictorySong)
					{
						BattleStopLowHpSound();
						PlayBGM(MUS_VICTORY_WILD);
						++gBattleStruct->wildVictorySong;
					}
					if (GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_HP) && !GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_IS_EGG))
					{
						if (gBattleStruct->sentInPokes & 1)
							gBattleMoveDamage = *exp;
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
							i = STRINGID_ABOOSTED;
						}
						else
							i = STRINGID_EMPTYSTRING4;
						// Get exp getter battlerId
						if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
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
						
						PREPARE_MON_NICK_WITH_PREFIX_BUFFER(gBattleTextBuff1, gBattleStruct->expGetterBattlerId, gBattleStruct->expGetterMonId);
						// Buffer 'gained' or 'gained a boosted'
						PREPARE_STRING_BUFFER(gBattleTextBuff2, i);
						PREPARE_WORD_NUMBER_BUFFER(gBattleTextBuff3, 5, gBattleMoveDamage);
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
					gActiveBattler = gBattleStruct->expGetterBattlerId;
					BtlController_EmitExpUpdate(0, gBattleStruct->expGetterMonId, gBattleMoveDamage);
					MarkBattlerForControllerExec(gActiveBattler);
				}
				++gBattleScripting.atk23_state;
			}
			break;
		case 4: // Lvl up if necessary
		    if (!gBattleControllerExecFlags)
			{
				gActiveBattler = gBattleStruct->expGetterBattlerId;
				
				if (gBattleBufferB[gActiveBattler][0] == CONTROLLER_TWORETURNVALUES && gBattleBufferB[gActiveBattler][1] == RET_VALUE_LEVELED_UP)
				{
					if (gBattleTypeFlags & BATTLE_TYPE_TRAINER && gBattlerPartyIndexes[gActiveBattler] == gBattleStruct->expGetterMonId)
						HandleLowHpMusicChange(&gPlayerParty[gBattlerPartyIndexes[gActiveBattler]], gActiveBattler);
					
					PREPARE_MON_NICK_WITH_PREFIX_BUFFER(gBattleTextBuff1, gActiveBattler, gBattleStruct->expGetterMonId);
					PREPARE_BYTE_NUMBER_BUFFER(gBattleTextBuff2, 3, GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_LEVEL));
					BattleScriptPushCursor();
					gLeveledUpInBattle |= gBitTable[gBattleStruct->expGetterMonId];
					gBattlescriptCurrInstr = BattleScript_LevelUp;
					gBattleMoveDamage = (gBattleBufferB[gActiveBattler][2] | (gBattleBufferB[gActiveBattler][3] << 8));
					AdjustFriendship(&gPlayerParty[gBattleStruct->expGetterMonId], FRIENDSHIP_EVENT_GROW_LEVEL);
					// Update battle mon structure after level up
					if (gBattlerPartyIndexes[0] == gBattleStruct->expGetterMonId && gBattleMons[0].hp)
					{
						gBattleMons[0].level = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_LEVEL);
						gBattleMons[0].hp = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_HP);
						gBattleMons[0].maxHP = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_MAX_HP);
						gBattleMons[0].attack = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_ATK);
						gBattleMons[0].defense = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_DEF);
						gBattleMons[0].speed = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPEED);
						gBattleMons[0].spAttack = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPATK);
						gBattleMons[0].spDefense = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPDEF);
					}
					else if (gBattlerPartyIndexes[2] == gBattleStruct->expGetterMonId && gBattleMons[2].hp && (gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
					{
						gBattleMons[2].level = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_LEVEL);
						gBattleMons[2].hp = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_HP);
						gBattleMons[2].maxHP = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_MAX_HP);
						gBattleMons[2].attack = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_ATK);
						gBattleMons[2].defense = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_DEF);
						gBattleMons[2].speed = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPEED);
						gBattleMons[2].spAttack = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPATK);
						gBattleMons[2].spDefense = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPDEF);
					}
					gBattleScripting.atk23_state = 5;
				}
				else
				{
					gBattleMoveDamage = 0;
					gBattleScripting.atk23_state = 5;
				}
			}
			break;
		case 5: // Looper increment
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
				gBattlescriptCurrInstr += 2;
			}
			break;
	}
}

static void atk24(void)
{
    u32 hitMarkerUnk;
    s32 i, foundPlayer, foundOpponent;

    if (!gBattleControllerExecFlags)
    {
        if (NoAliveMonsForParty(gPlayerParty))
            gBattleOutcome |= B_OUTCOME_LOST;
		
        if (NoAliveMonsForParty(gEnemyParty))
            gBattleOutcome |= B_OUTCOME_WON;
		
        if (gBattleOutcome == 0 && (gBattleTypeFlags & BATTLE_TYPE_LINK))
        {
			u32 *ptr = &gHitMarker;
            hitMarkerUnk = 0x10000000;
		
            for (foundPlayer = 0, i = 0; i < gBattlersCount; i += 2)
            {
                ++i, --i;
				
                if ((hitMarkerUnk << i) & *ptr && !gSpecialStatuses[i].flag40)
                    ++foundPlayer;
            }
            for (foundOpponent = 0, i = 1; i < gBattlersCount; i += 2)
            {
                if ((hitMarkerUnk << i) & *ptr && !gSpecialStatuses[i].flag40)
                    ++foundOpponent;
            }
            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                if (foundOpponent + foundPlayer > 1)
                    gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 1);
                else
                    gBattlescriptCurrInstr += 5;
            }
            else
            {
                if (foundOpponent != 0 && foundPlayer != 0)
                    gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 1);
                else
                    gBattlescriptCurrInstr += 5;
            }
        }
        else
            gBattlescriptCurrInstr += 5;
    }
}

static void MoveValuesCleanUp(void)
{
    gMoveResultFlags = 0;
    gBattleScripting.dmgMultiplier = 1;
    gCritMultiplier = 1;
    gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
    gBattleCommunication[6] = 0;
    gHitMarker &= ~(HITMARKER_DESTINYBOND);
    gHitMarker &= ~(HITMARKER_SYNCHRONISE_EFFECT);
}

static void atk25_movevaluescleanup(void)
{
    MoveValuesCleanUp();
    gBattlescriptCurrInstr += 1;
}

static void atk26_setmultihit(void)
{
    gMultiHitCounter = gBattlescriptCurrInstr[1];
    gBattlescriptCurrInstr += 2;
}

static void atk27_decrementmultihit(void)
{
    if (--gMultiHitCounter == 0)
        gBattlescriptCurrInstr += 5;
    else
        gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atk28_goto(void)
{
    gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atk29_jumpifbyte(void)
{
    u8 caseID = gBattlescriptCurrInstr[1], value = gBattlescriptCurrInstr[6];
    const u8 *memByte = T2_READ_PTR(gBattlescriptCurrInstr + 2);
    const u8 *jumpPtr = T2_READ_PTR(gBattlescriptCurrInstr + 7);

    gBattlescriptCurrInstr += 11;

    switch (caseID)
    {
    case CMP_EQUAL:
        if (*memByte == value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_NOT_EQUAL:
        if (*memByte != value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_GREATER_THAN:
        if (*memByte > value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_LESS_THAN:
        if (*memByte < value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_COMMON_BITS:
        if (*memByte & value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_NO_COMMON_BITS:
        if (!(*memByte & value))
            gBattlescriptCurrInstr = jumpPtr;
        break;
    }
}

static void atk2A_jumpifhalfword(void)
{
    u8 caseID = gBattlescriptCurrInstr[1];
    const u8 *jumpPtr = T2_READ_PTR(gBattlescriptCurrInstr + 8);
    u16 value = T2_READ_16(gBattlescriptCurrInstr + 6);
    const u16 *memHword = T2_READ_PTR(gBattlescriptCurrInstr + 2);

    gBattlescriptCurrInstr += 12;
	
    switch (caseID)
    {
    case CMP_EQUAL:
        if (*memHword == value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_NOT_EQUAL:
        if (*memHword != value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_GREATER_THAN:
        if (*memHword > value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_LESS_THAN:
        if (*memHword < value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_COMMON_BITS:
        if (*memHword & value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_NO_COMMON_BITS:
        if (!(*memHword & value))
            gBattlescriptCurrInstr = jumpPtr;
        break;
    }
}

static void atk2B_jumpifword(void)
{
    u8 caseID = gBattlescriptCurrInstr[1];
    const u8 *jumpPtr = T2_READ_PTR(gBattlescriptCurrInstr + 10);
    u32 value = T1_READ_32(gBattlescriptCurrInstr + 6);
    const u32 *memWord = T2_READ_PTR(gBattlescriptCurrInstr + 2);

    gBattlescriptCurrInstr += 14;
	
    switch (caseID)
    {
    case CMP_EQUAL:
        if (*memWord == value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_NOT_EQUAL:
        if (*memWord != value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_GREATER_THAN:
        if (*memWord > value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_LESS_THAN:
        if (*memWord < value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_COMMON_BITS:
        if (*memWord & value)
            gBattlescriptCurrInstr = jumpPtr;
        break;
    case CMP_NO_COMMON_BITS:
        if (!(*memWord & value))
            gBattlescriptCurrInstr = jumpPtr;
        break;
    }
}

static void atk2C_jumpifarrayequal(void)
{
    u8 i;
    const u8 *mem1 = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    const u8 *mem2 = T2_READ_PTR(gBattlescriptCurrInstr + 5);
    const u8 *jumpPtr = T2_READ_PTR(gBattlescriptCurrInstr + 10);
    u32 size = gBattlescriptCurrInstr[9];

    for (i = 0; i < size; ++i, ++mem1, ++mem2)
    {
        if (*mem1 != *mem2)
        {
            gBattlescriptCurrInstr += 14;
            break;
        }
    }
    if (i == size)
        gBattlescriptCurrInstr = jumpPtr;
}

static void atk2D_jumpifarraynotequal(void)
{
    u8 i, equalBytes = 0;
    const u8 *mem1 = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    const u8 *mem2 = T2_READ_PTR(gBattlescriptCurrInstr + 5);
    const u8 *jumpPtr = T2_READ_PTR(gBattlescriptCurrInstr + 10);
    u32 size = gBattlescriptCurrInstr[9];

    for (i = 0; i < size; ++i, ++mem1, ++mem2)
    {
        if (*mem1 == *mem2)
            ++equalBytes;
    }
    if (equalBytes != size)
        gBattlescriptCurrInstr = jumpPtr;
    else
        gBattlescriptCurrInstr += 14;
}

static void atk2E_setbyte(void)
{
    u8 *memByte = T2_READ_PTR(gBattlescriptCurrInstr + 1);

    *memByte = gBattlescriptCurrInstr[5];
    gBattlescriptCurrInstr += 6;
}

static void atk2F_addbyte(void)
{
    u8 *memByte = T2_READ_PTR(gBattlescriptCurrInstr + 1);

    *memByte += gBattlescriptCurrInstr[5];
    gBattlescriptCurrInstr += 6;
}

static void atk30_subbyte(void)
{
    u8 *memByte = T2_READ_PTR(gBattlescriptCurrInstr + 1);

    *memByte -= gBattlescriptCurrInstr[5];
    gBattlescriptCurrInstr += 6;
}

static void atk31_copyarray(void)
{
    u8 *dest = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    const u8 *src = T2_READ_PTR(gBattlescriptCurrInstr + 5);
    s32 i, size = gBattlescriptCurrInstr[9];

    for (i = 0; i < size; ++i)
        dest[i] = src[i];
    gBattlescriptCurrInstr += 10;
}

static void atk32_copyarraywithindex(void)
{
    u8 *dest = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    const u8 *src = T2_READ_PTR(gBattlescriptCurrInstr + 5);
    const u8 *index = T2_READ_PTR(gBattlescriptCurrInstr + 9);
    s32 i, size = gBattlescriptCurrInstr[13];

    for (i = 0; i < size; ++i)
        dest[i] = src[i + *index];
    gBattlescriptCurrInstr += 14;
}

static void atk33_orbyte(void)
{
    u8 *memByte = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    *memByte |= gBattlescriptCurrInstr[5];
    gBattlescriptCurrInstr += 6;
}

static void atk34_orhalfword(void)
{
    u16 *memHword = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    u16 val = T2_READ_16(gBattlescriptCurrInstr + 5);

    *memHword |= val;
    gBattlescriptCurrInstr += 7;
}

static void atk35_orword(void)
{
    u32 *memWord = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    u32 val = T2_READ_32(gBattlescriptCurrInstr + 5);

    *memWord |= val;
    gBattlescriptCurrInstr += 9;
}

static void atk36_bicbyte(void)
{
    u8 *memByte = T2_READ_PTR(gBattlescriptCurrInstr + 1);

    *memByte &= ~(gBattlescriptCurrInstr[5]);
    gBattlescriptCurrInstr += 6;
}

static void atk37_bichalfword(void)
{
    u16 *memHword = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    u16 val = T2_READ_16(gBattlescriptCurrInstr + 5);

    *memHword &= ~(val);
    gBattlescriptCurrInstr += 7;
}

static void atk38_bicword(void)
{
    u32 *memWord = T2_READ_PTR(gBattlescriptCurrInstr + 1);
    u32 val = T2_READ_32(gBattlescriptCurrInstr + 5);

    *memWord &= ~(val);
    gBattlescriptCurrInstr += 9;
}

static void atk39_pause(void)
{
    if (!gBattleControllerExecFlags)
    {
        if (++gPauseCounterBattle >= T2_READ_16(gBattlescriptCurrInstr + 1))
        {
            gPauseCounterBattle = 0;
            gBattlescriptCurrInstr += 3;
        }
    }
}

static void atk3A_waitstate(void)
{
    if (!gBattleControllerExecFlags)
        ++gBattlescriptCurrInstr;
}

static void atk3B_healthbar_update(void)
{
    if (gBattlescriptCurrInstr[1] == BS_TARGET)
        gActiveBattler = gBattlerTarget;
    else
        gActiveBattler = gBattlerAttacker;
    BtlController_EmitHealthBarUpdate(0, gBattleMoveDamage);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

static void atk3C_return(void)
{
    BattleScriptPop();
}

static void atk3D_end(void)
{
    gMoveResultFlags = 0;
    gActiveBattler = 0;
    gCurrentActionFuncId = B_ACTION_TRY_FINISH;
}

static void atk3E_end2(void)
{
    gActiveBattler = 0;
    gCurrentActionFuncId = B_ACTION_TRY_FINISH;
}

static void atk3F_end3(void) // pops the main function stack
{
    BattleScriptPop();
    if (gBattleResources->battleCallbackStack->size != 0)
        gBattleResources->battleCallbackStack->size--;
    gBattleMainFunc = gBattleResources->battleCallbackStack->function[gBattleResources->battleCallbackStack->size];
}

static void atk40_jumpifaffectedbyprotect(void)
{
    if (DEFENDER_IS_PROTECTED)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        JumpIfMoveFailed(5, 0);
        gBattleCommunication[6] = 1;
    }
    else
        gBattlescriptCurrInstr += 5;
}

static void atk41_call(void)
{
    BattleScriptPush(gBattlescriptCurrInstr + 5);
    gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atk42_trysetsleep(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	const u8 *jumpPtr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	
	if (gBattleMons[bank].status1 & STATUS1_SLEEP)
	{
		gBattlescriptCurrInstr = BattleScript_AlreadyAsleep;
		return;
	}
	else if (gBattleMons[bank].status1 & STATUS1_ANY)
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed;
		return;
	}
	else if ((gSideStatuses[GET_BATTLER_SIDE(bank)] & SIDE_STATUS_SAFEGUARD) && GetBattlerAbility(gBattlerAttacker) != ABILITY_INFILTRATOR && gBattlerAttacker != bank)
	{
		gBattlescriptCurrInstr = BattleScript_SafeguardProtected;
		return;
	}
	else if (UproarWakeUpCheck(bank))
	{
		gBattlescriptCurrInstr = jumpPtr;
		return;
	}
	else
	{
		switch (GetBattlerAbility(bank))
		{
			case ABILITY_INSOMNIA:
			case ABILITY_VITAL_SPIRIT:
				gLastUsedAbility = gBattleMons[bank].ability;
				gBattleCommunication[MULTISTRING_CHOOSER] = 2;
				gBattlescriptCurrInstr = jumpPtr;
				RecordAbilityBattle(bank, gLastUsedAbility);
				return;
			case ABILITY_LEAF_GUARD:
				if (IsBattlerWeatherAffected(bank, WEATHER_SUN_ANY))
				{
					gLastUsedAbility = gBattleMons[bank].ability;
					gBattleCommunication[MULTISTRING_CHOOSER] = 2;
					gBattlescriptCurrInstr = jumpPtr;
					RecordAbilityBattle(bank, gLastUsedAbility);
					return;
				}
				break;
			case ABILITY_FLOWER_VEIL:
			    if (IS_BATTLER_OF_TYPE(bank, TYPE_GRASS))
				{
					gLastUsedAbility = gBattleMons[bank].ability;
					gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
					RecordAbilityBattle(bank, gLastUsedAbility);
					return;
				}
				break;
			case ABILITY_SWEET_VEIL:
			    gLastUsedAbility = gBattleMons[bank].ability;
				gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
				RecordAbilityBattle(bank, gLastUsedAbility);
				return;
		}
		if (IsBattlerAlive(BATTLE_PARTNER(bank)))
		{
			switch (GetBattlerAbility(BATTLE_PARTNER(bank)))
			{
				case ABILITY_FLOWER_VEIL:
				    if (IS_BATTLER_OF_TYPE(bank, TYPE_GRASS))
					{
						gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
						gLastUsedAbility = gBattleMons[BATTLE_PARTNER(bank)].ability;
						RecordAbilityBattle(BATTLE_PARTNER(bank), gLastUsedAbility);
						return;
					}
					break;
				case ABILITY_SWEET_VEIL:
				    gBattlescriptCurrInstr = BattleScript_TeamProtectedBySweetVeil;
					gLastUsedAbility = gBattleMons[BATTLE_PARTNER(bank)].ability;
					RecordAbilityBattle(BATTLE_PARTNER(bank), gLastUsedAbility);
					return;
			}
		}
	}
	gBattlescriptCurrInstr += 6;
}

static void atk43_jumpifabilitypresent(void)
{
    if (AbilityBattleEffects(ABILITYEFFECT_CHECK_ON_FIELD, 0, gBattlescriptCurrInstr[1], 0, 0))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
    else
        gBattlescriptCurrInstr += 7;
}

static void atk44_endselectionscript(void)
{
    *(gBattlerAttacker + gBattleStruct->selectionScriptFinished) = TRUE;
}

static void atk45_playanimation(void)
{
    const u16 *argumentPtr;

    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    argumentPtr = T2_READ_PTR(gBattlescriptCurrInstr + 3);
	
    if (gBattlescriptCurrInstr[2] == B_ANIM_STATS_CHANGE || gBattlescriptCurrInstr[2] == B_ANIM_SNATCH_MOVE || gBattlescriptCurrInstr[2] == B_ANIM_SUBSTITUTE_FADE
     || gBattlescriptCurrInstr[2] == B_ANIM_SILPH_SCOPED || gBattlescriptCurrInstr[2] == B_ANIM_ILLUSION_OFF || gBattlescriptCurrInstr[2] == B_ANIM_FORM_CHANGE)
    {
        BtlController_EmitBattleAnimation(0, gBattlescriptCurrInstr[2], *argumentPtr);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 7;
    }
    else if (gHitMarker & HITMARKER_NO_ANIMATIONS)
    {
        BattleScriptPush(gBattlescriptCurrInstr + 7);
        gBattlescriptCurrInstr = BattleScript_Pausex20;
    }
    else if (gBattlescriptCurrInstr[2] == B_ANIM_RAIN_CONTINUES || gBattlescriptCurrInstr[2] == B_ANIM_SUN_CONTINUES
	     || gBattlescriptCurrInstr[2] == B_ANIM_SANDSTORM_CONTINUES || gBattlescriptCurrInstr[2] == B_ANIM_HAIL_CONTINUES
		 || gBattlescriptCurrInstr[2] == B_ANIM_FOG_CONTINUES)
    {
        BtlController_EmitBattleAnimation(0, gBattlescriptCurrInstr[2], *argumentPtr);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 7;
    }
    else if (gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE)
        gBattlescriptCurrInstr += 7;
    else
    {
        BtlController_EmitBattleAnimation(0, gBattlescriptCurrInstr[2], *argumentPtr);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 7;
    }
}

static void atk46_playanimation2(void) // Animation Id is stored in the first pointer
{
    const u16 *argumentPtr;
    const u8 *animationIdPtr;

    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    animationIdPtr = T2_READ_PTR(gBattlescriptCurrInstr + 2);
    argumentPtr = T2_READ_PTR(gBattlescriptCurrInstr + 6);
	
    if (*animationIdPtr == B_ANIM_STATS_CHANGE || *animationIdPtr == B_ANIM_SNATCH_MOVE || *animationIdPtr == B_ANIM_SUBSTITUTE_FADE)
    {
        BtlController_EmitBattleAnimation(0, *animationIdPtr, *argumentPtr);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 10;
    }
    else if (gHitMarker & HITMARKER_NO_ANIMATIONS)
        gBattlescriptCurrInstr += 10;
    else if (*animationIdPtr == B_ANIM_RAIN_CONTINUES || *animationIdPtr == B_ANIM_SUN_CONTINUES || *animationIdPtr == B_ANIM_SANDSTORM_CONTINUES
	     || *animationIdPtr == B_ANIM_HAIL_CONTINUES || *animationIdPtr == B_ANIM_FOG_CONTINUES)
    {
        BtlController_EmitBattleAnimation(0, *animationIdPtr, *argumentPtr);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 10;
    }
    else if (gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE)
        gBattlescriptCurrInstr += 10;
    else
    {
        BtlController_EmitBattleAnimation(0, *animationIdPtr, *argumentPtr);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 10;
    }
}

static void atk47_setgraphicalstatchangevalues(void)
{
    u8 value = 0;

    switch (GET_STAT_BUFF_VALUE2(gBattleScripting.statChanger))
    {
    case SET_STAT_BUFF_VALUE(1): // +1
        value = STAT_ANIM_PLUS1;
        break;
    case SET_STAT_BUFF_VALUE(2): // +2
        value = STAT_ANIM_PLUS2;
        break;
    case SET_STAT_BUFF_VALUE(1) | STAT_BUFF_NEGATIVE: // -1
        value = STAT_ANIM_MINUS1;
        break;
    case SET_STAT_BUFF_VALUE(2) | STAT_BUFF_NEGATIVE: // -2
        value = STAT_ANIM_MINUS2;
        break;
    }
    gBattleScripting.animArg1 = GET_STAT_BUFF_ID(gBattleScripting.statChanger) + value - 1;
    gBattleScripting.animArg2 = 0;
    ++gBattlescriptCurrInstr;
}

static void atk48_playstatchangeanimation(void)
{
    u8 statsToCheck = 0, flag = gBattlescriptCurrInstr[3];
    u16 statAnimId = 0;
    s16 startingStatAnimId;
    u32 currStat = 0;
    s32 changeableStatsCount = 0;

    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    statsToCheck = gBattlescriptCurrInstr[2];
	
    if (GetBattlerAbility(gActiveBattler) == ABILITY_SIMPLE)
	    flag |= ATK48_STAT_BY_TWO;
	
    if (GetBattlerAbility(gActiveBattler) == ABILITY_CONTRARY)
		flag ^= ATK48_STAT_NEGATIVE;
	
    if (flag & ATK48_STAT_NEGATIVE) // Goes down
    {
        if (flag & ATK48_STAT_BY_TWO)
            startingStatAnimId = STAT_ANIM_MINUS2 - 1;
        else
            startingStatAnimId = STAT_ANIM_MINUS1 - 1;

        while (statsToCheck != 0)
        {
            if (statsToCheck & 1)
            {
                if (flag & ATK48_DONT_CHECK_LOWER)
                {
                    if (gBattleMons[gActiveBattler].statStages[currStat] > 0)
                    {
                        statAnimId = startingStatAnimId + currStat;
                        ++changeableStatsCount;
                    }
                }
                else if (!gSideTimers[GET_BATTLER_SIDE(gActiveBattler)].mistTimer && GetBattlerAbility(gActiveBattler) != ABILITY_CLEAR_BODY
			 && GetBattlerAbility(gActiveBattler) != ABILITY_WHITE_SMOKE 
			 && !(GetBattlerAbility(gActiveBattler) == ABILITY_KEEN_EYE && currStat == STAT_ACC)
			 && !(GetBattlerAbility(gActiveBattler) == ABILITY_HYPER_CUTTER && currStat == STAT_ATK)
			 && !(GetBattlerAbility(gActiveBattler) == ABILITY_BIG_PECKS && currStat == STAT_DEF)
			 && !(GetBattlerAbility(gActiveBattler) == ABILITY_FLOWER_VEIL && IS_BATTLER_OF_TYPE(gActiveBattler, TYPE_GRASS))
			 && !(IsBattlerAlive(BATTLE_PARTNER(gActiveBattler)) && GetBattlerAbility(BATTLE_PARTNER(gActiveBattler)) == ABILITY_FLOWER_VEIL && IS_BATTLER_OF_TYPE(gActiveBattler, TYPE_GRASS)))
                {
                    if (gBattleMons[gActiveBattler].statStages[currStat] > 0)
                    {
                        statAnimId = startingStatAnimId + currStat;
                        ++changeableStatsCount;
                    }
                }
            }
            statsToCheck >>= 1;
            ++currStat;
        }

        if (changeableStatsCount > 1) // More than one stat, so the color is gray
        {
            if (flag & ATK48_STAT_BY_TWO)
                statAnimId = STAT_ANIM_MULTIPLE_MINUS2;
            else
                statAnimId = STAT_ANIM_MULTIPLE_MINUS1;
        }
    }
    else // Goes up
    {
        if (flag & ATK48_STAT_BY_TWO)
            startingStatAnimId = STAT_ANIM_PLUS2 - 1;
        else
            startingStatAnimId = STAT_ANIM_PLUS1 - 1;

        while (statsToCheck != 0)
        {
            if (statsToCheck & 1 && gBattleMons[gActiveBattler].statStages[currStat] < 0xC)
            {
                statAnimId = startingStatAnimId + currStat;
                ++changeableStatsCount;
            }
            statsToCheck >>= 1;
            ++currStat;
        }
        if (changeableStatsCount > 1) // More than one stat, so the color is gray
        {
            if (flag & ATK48_STAT_BY_TWO)
                statAnimId = STAT_ANIM_MULTIPLE_PLUS2;
            else
                statAnimId = STAT_ANIM_MULTIPLE_PLUS1;
        }
    }
    if (flag & ATK48_ONLY_MULTIPLE && changeableStatsCount < 2)
        gBattlescriptCurrInstr += 4;
    else if (changeableStatsCount != 0 && !gBattleScripting.statAnimPlayed)
    {
        BtlController_EmitBattleAnimation(0, B_ANIM_STATS_CHANGE, statAnimId);
        MarkBattlerForControllerExec(gActiveBattler);
        if (flag & ATK48_ONLY_MULTIPLE && changeableStatsCount > 1)
            gBattleScripting.statAnimPlayed = TRUE;
        gBattlescriptCurrInstr += 4;
    }
    else
        gBattlescriptCurrInstr += 4;
}

static void atk49_moveend(void)
{
    u8 arg1, arg2, target, targetPartner, attacker, holdEffectAtk = 0, moveType = gBattleStruct->dynamicMoveType;
    u16 originallyUsedMove, *changedItem, *choicedMoveAtk = NULL;
    s32 i;
    bool32 effect = FALSE;

    if (gChosenMove == 0xFFFF)
        originallyUsedMove = MOVE_NONE;
    else
        originallyUsedMove = gChosenMove;
	
    arg1 = gBattlescriptCurrInstr[1];
    arg2 = gBattlescriptCurrInstr[2];
	holdEffectAtk = GetBattlerItemHoldEffect(gBattlerAttacker, TRUE);
    choicedMoveAtk = &gBattleStruct->choicedMove[gBattlerAttacker];
    
    do
    {
        switch (gBattleScripting.atk49_state)
        {
        case ATK49_RAGE: // rage check
            if (gBattleMons[gBattlerTarget].status2 & STATUS2_RAGE && IsBattlerAlive(gBattlerTarget) && BattlerStatCanRaise(gBattlerTarget, STAT_ATK)
		&& gBattlerAttacker != gBattlerTarget && GetBattlerSide(gBattlerAttacker) != GetBattlerSide(gBattlerTarget) && TARGET_TURN_DAMAGED
		&& !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && gBattleMoves[gCurrentMove].power)
            {
		BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_RageIsBuilding;
                effect = TRUE;
            }
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_DEFROST: // defrosting check
            if (gBattleMons[gBattlerTarget].status1 & STATUS1_FREEZE && IsBattlerAlive(gBattlerTarget) && gBattlerAttacker != gBattlerTarget
		&& gSpecialStatuses[gBattlerTarget].specialDmg && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) && moveType == TYPE_FIRE)
            {
                gBattleMons[gBattlerTarget].status1 &= ~(STATUS1_FREEZE);
                gActiveBattler = gBattlerTarget;
                BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gBattlerTarget].status1);
                MarkBattlerForControllerExec(gActiveBattler);
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_DefrostedViaFireMove;
                effect = TRUE;
            }
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_SYNCHRONIZE_TARGET: // target synchronize
            if (AbilityBattleEffects(ABILITYEFFECT_SYNCHRONIZE, gBattlerTarget, 0, 0, 0))
                effect = TRUE;
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_MOVE_END_ABILITIES: // Such as abilities activating on contact(Poison Spore, Rough Skin, etc.).
            if (AbilityBattleEffects(ABILITYEFFECT_MOVE_END, gBattlerTarget, 0, 0, 0))
                effect = TRUE;
            ++gBattleScripting.atk49_state;
            break;
	case ATK49_MOVE_END_ABILITIES_ATTACKER: // Same as above, but for attacker(Poison Touch and Stench).
	    if (AbilityBattleEffects(ABILITYEFFECT_MOVE_END_ATTACKER, gBattlerAttacker, 0, 0, 0))
		effect = TRUE;
	    ++gBattleScripting.atk49_state;
            break;
        case ATK49_STATUS_IMMUNITY_ABILITIES: // status immunities
            if (AbilityBattleEffects(ABILITYEFFECT_IMMUNITY, 0, 0, 0, 0))
                effect = TRUE; // it loops through all battlers, so we increment after its done with all battlers
            else
                ++gBattleScripting.atk49_state;
            break;
        case ATK49_SYNCHRONIZE_ATTACKER: // attacker synchronize
            if (AbilityBattleEffects(ABILITYEFFECT_ATK_SYNCHRONIZE, gBattlerAttacker, 0, 0, 0))
                effect = TRUE;
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_CHOICE_MOVE: // update choice band move
            if (gHitMarker & HITMARKER_OBEYS && holdEffectAtk == HOLD_EFFECT_CHOICE_BAND && gChosenMove != MOVE_STRUGGLE 
		&& (*choicedMoveAtk == 0 || *choicedMoveAtk == 0xFFFF))
            {
                if (gChosenMove == MOVE_BATON_PASS && !(gMoveResultFlags & MOVE_RESULT_FAILED))
                {
                    ++gBattleScripting.atk49_state;
                    break;
                }
                *choicedMoveAtk = gChosenMove;
            }
            for (i = 0; i < MAX_MON_MOVES; ++i)
            {
                if (gBattleMons[gBattlerAttacker].moves[i] == *choicedMoveAtk)
                    break;
            }
            if (i == MAX_MON_MOVES)
                *choicedMoveAtk = 0;
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_CHANGED_ITEMS: // changed held items
            for (i = 0; i < gBattlersCount; ++i)
            {
                changedItem = &gBattleStruct->changedItems[i];
                
                if (*changedItem != 0)
                {
                    gBattleMons[i].item = *changedItem;
                    *changedItem = 0;
                }
            }
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_ITEM_EFFECTS_ALL: // item effects for all battlers
            if (ItemBattleEffects(ITEMEFFECT_MOVE_END, 0, FALSE))
                effect = TRUE;
            else
                ++gBattleScripting.atk49_state;
            break;
        case ATK49_KINGSROCK_SHELLBELL: // king's rock and shell bell
            if (ItemBattleEffects(ITEMEFFECT_KINGSROCK_SHELLBELL, gBattlerAttacker, FALSE))
                effect = TRUE;
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_ATTACKER_INVISIBLE: // make attacker sprite invisible
            if (gStatuses3[gBattlerAttacker] & (STATUS3_SEMI_INVULNERABLE) && gHitMarker & HITMARKER_NO_ANIMATIONS)
            {
                gActiveBattler = gBattlerAttacker;
                BtlController_EmitSpriteInvisibility(0, TRUE);
                MarkBattlerForControllerExec(gActiveBattler);
                ++gBattleScripting.atk49_state;
                return;
            }
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_ATTACKER_VISIBLE: // make attacker sprite visible
            if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT || !(gStatuses3[gBattlerAttacker] & (STATUS3_SEMI_INVULNERABLE)) || WasUnableToUseMove(gBattlerAttacker))
            {
                gActiveBattler = gBattlerAttacker;
                BtlController_EmitSpriteInvisibility(0, FALSE);
                MarkBattlerForControllerExec(gActiveBattler);
                gStatuses3[gBattlerAttacker] &= ~(STATUS3_SEMI_INVULNERABLE);
                gSpecialStatuses[gBattlerAttacker].restoredBattlerSprite = 1;
                ++gBattleScripting.atk49_state;
                return;
            }
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_TARGET_VISIBLE: // make target sprite visible
            if (!gSpecialStatuses[gBattlerTarget].restoredBattlerSprite && gBattlerTarget < gBattlersCount && !(gStatuses3[gBattlerTarget] & STATUS3_SEMI_INVULNERABLE))
            {
                gActiveBattler = gBattlerTarget;
                BtlController_EmitSpriteInvisibility(0, FALSE);
                MarkBattlerForControllerExec(gActiveBattler);
                gStatuses3[gBattlerTarget] &= ~(STATUS3_SEMI_INVULNERABLE);
                ++gBattleScripting.atk49_state;
                return;
            }
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_SUBSTITUTE: // update substitute
            for (i = 0; i < gBattlersCount; ++i)
            {
                if (gDisableStructs[i].substituteHP == 0)
                    gBattleMons[i].status2 &= ~(STATUS2_SUBSTITUTE);
            }
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_UPDATE_LAST_MOVES:
            if (gHitMarker & HITMARKER_SWAP_ATTACKER_TARGET)
            {
                gActiveBattler = gBattlerAttacker;
                gBattlerAttacker = gBattlerTarget;
                gBattlerTarget = gActiveBattler;
                gHitMarker &= ~(HITMARKER_SWAP_ATTACKER_TARGET);
            }
            if (gHitMarker & HITMARKER_ATTACKSTRING_PRINTED)
            {
                gLastPrintedMoves[gBattlerAttacker] = gChosenMove;
            }
            if (!(gAbsentBattlerFlags & gBitTable[gBattlerAttacker]) && !(gBattleStruct->absentBattlerFlags & gBitTable[gBattlerAttacker])
		&& gBattleMoves[originallyUsedMove].effect != EFFECT_BATON_PASS)
            {
                if (gHitMarker & HITMARKER_OBEYS)
                {
                    gLastMoves[gBattlerAttacker] = gChosenMove;
                    gLastResultingMoves[gBattlerAttacker] = gCurrentMove;
                }
                else
                {
                    gLastMoves[gBattlerAttacker] = 0xFFFF;
                    gLastResultingMoves[gBattlerAttacker] = 0xFFFF;
                }
                if (!(gHitMarker & HITMARKER_FAINTED(gBattlerTarget)))
                    gLastHitBy[gBattlerTarget] = gBattlerAttacker;

                if (gHitMarker & HITMARKER_OBEYS && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
                {
                    if (gChosenMove == 0xFFFF)
                        gLastLandedMoves[gBattlerTarget] = gChosenMove;
                    else
                    {
                        gLastLandedMoves[gBattlerTarget] = gCurrentMove;
			gLastHitByType[gBattlerTarget] = gBattleStruct->dynamicMoveType;
                    }
                }
                else
                    gLastLandedMoves[gBattlerTarget] = 0xFFFF;
            }
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_MIRROR_MOVE: // mirror move
            if (!(gAbsentBattlerFlags & gBitTable[gBattlerAttacker]) && !(gBattleStruct->absentBattlerFlags & gBitTable[gBattlerAttacker])
		&& gBattleMoves[originallyUsedMove].flags & FLAG_MIRROR_MOVE_AFFECTED && gHitMarker & HITMARKER_OBEYS && gBattlerAttacker != gBattlerTarget
		&& !(gHitMarker & HITMARKER_FAINTED(gBattlerTarget)) && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
            {
		target = gBattlerTarget;
                attacker = gBattlerAttacker;
                *(gBattleStruct->lastTakenMove + target * 2 + 0) = gChosenMove;
                *(gBattleStruct->lastTakenMove + target * 2 + 1) = gChosenMove >> 8;
                *(attacker * 2 + target * 8 + (gBattleStruct->lastTakenMoveFrom) + 0) = gChosenMove;
                *(attacker * 2 + target * 8 + (gBattleStruct->lastTakenMoveFrom) + 1) = gChosenMove >> 8;
            }
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_NEXT_TARGET: // For moves hitting two opposing Pokemon.
            if (!(gHitMarker & (HITMARKER_UNABLE_TO_USE_MOVE | HITMARKER_NO_ATTACKSTRING)) && gBattleTypeFlags & BATTLE_TYPE_DOUBLE
		&& !gProtectStructs[gBattlerAttacker].chargingTurn && gBattleMoves[gCurrentMove].target == MOVE_TARGET_BOTH)
            {
                targetPartner = GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(gBattlerTarget)));
		gHitMarker |= HITMARKER_NO_ATTACKSTRING;
		    
                if (IsBattlerAlive(targetPartner))
                {
                    gBattlerTarget = targetPartner;
                    gBattleScripting.atk49_state = 0;
                    MoveValuesCleanUp();
                    BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
                    gBattlescriptCurrInstr = BattleScript_FlushMessageBox;
                    return;
                }
            }
            ++gBattleScripting.atk49_state;
            break;
        case ATK49_COUNT:
		    gProtectStructs[gBattlerAttacker].usesBouncedMove = FALSE;
            break;
        }
        if (arg1 == 1 && effect == FALSE)
            gBattleScripting.atk49_state = ATK49_COUNT;
        if (arg1 == 2 && arg2 == gBattleScripting.atk49_state)
            gBattleScripting.atk49_state = ATK49_COUNT;
    }
    while (gBattleScripting.atk49_state != ATK49_COUNT && effect == FALSE);
    if (gBattleScripting.atk49_state == ATK49_COUNT && effect == FALSE)
        gBattlescriptCurrInstr += 3;
}

static void atk4A_nop(void)
{
    ++gBattlescriptCurrInstr;
}

static void atk4B_returnatktoball(void)
{
    gActiveBattler = gBattlerAttacker;
	
    if (!(gHitMarker & HITMARKER_FAINTED(gActiveBattler)))
    {
        BtlController_EmitReturnMonToBall(0, 0);
        MarkBattlerForControllerExec(gActiveBattler);
    }
    ++gBattlescriptCurrInstr;
}

static void atk4C_getswitchedmondata(void)
{
    if (!gBattleControllerExecFlags)
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
        gBattlerPartyIndexes[gActiveBattler] = *(gBattleStruct->monToSwitchIntoId + gActiveBattler);
        BtlController_EmitGetMonData(0, REQUEST_ALL_BATTLE, gBitTable[gBattlerPartyIndexes[gActiveBattler]]);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 2;
    }
}

static void atk4D_switchindataupdate(void)
{
    struct BattlePokemon oldData;
    u8 *monData;
    s32 i;

    if (!gBattleControllerExecFlags)
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
        oldData = gBattleMons[gActiveBattler];
        monData = (u8 *)(&gBattleMons[gActiveBattler]);
        for (i = 0; i < sizeof(struct BattlePokemon); ++i)
            monData[i] = gBattleBufferB[gActiveBattler][4 + i];
        gBattleMons[gActiveBattler].type1 = gBaseStats[gBattleMons[gActiveBattler].species].type1;
        gBattleMons[gActiveBattler].type2 = gBaseStats[gBattleMons[gActiveBattler].species].type2;
        gBattleMons[gActiveBattler].ability = GetAbilityBySpecies(gBattleMons[gActiveBattler].species, gBattleMons[gActiveBattler].abilityNum, gBattleMons[gActiveBattler].abilityHidden);
        // check knocked off item
        i = GetBattlerSide(gActiveBattler);
        if (gWishFutureKnock.knockedOffMons[i] & gBitTable[gBattlerPartyIndexes[gActiveBattler]])
            gBattleMons[gActiveBattler].item = 0;
        if (gBattleMoves[gCurrentMove].effect == EFFECT_BATON_PASS)
        {
            for (i = 0; i < NUM_BATTLE_STATS; ++i)
                gBattleMons[gActiveBattler].statStages[i] = oldData.statStages[i];
            gBattleMons[gActiveBattler].status2 = oldData.status2;
        }
        SwitchInClearSetData();
        gBattleScripting.battler = gActiveBattler;
        PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, gActiveBattler, gBattlerPartyIndexes[gActiveBattler]);
        gBattlescriptCurrInstr += 2;
    }
}

static void atk4E_switchinanim(void)
{
    if (!gBattleControllerExecFlags)
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	    
        if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT 
	    && !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_LEGENDARY | BATTLE_TYPE_OLD_MAN_TUTORIAL | BATTLE_TYPE_POKEDUDE | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_GHOST)))
            HandleSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[gActiveBattler].species), FLAG_SET_SEEN, gBattleMons[gActiveBattler].personality);
        gAbsentBattlerFlags &= ~(gBitTable[gActiveBattler]);
        BtlController_EmitSwitchInAnim(0, gBattlerPartyIndexes[gActiveBattler], gBattlescriptCurrInstr[2]);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 3;
    }
}

static void atk4F_jumpifcantswitch(void)
{
    u8 battlerIn1, battlerIn2;
    s32 i, lastMonId;
    struct Pokemon *party;

    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1] & ~(ATK4F_DONT_CHECK_STATUSES));
	
    if (!(gBattlescriptCurrInstr[1] & ATK4F_DONT_CHECK_STATUSES) && ((gBattleMons[gActiveBattler].status2 & (STATUS2_WRAPPED | STATUS2_ESCAPE_PREVENTION))
        || (gStatuses3[gActiveBattler] & STATUS3_ROOTED)))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
    else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
    {
        if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT)
            party = gEnemyParty;
        else
            party = gPlayerParty;

        i = 0;
        if (GetLinkTrainerFlankId(GetBattlerMultiplayerId(gActiveBattler)) == TRUE)
            i = 3;
        for (lastMonId = i + 3; i < lastMonId; ++i)
        {
            if (GetMonData(&party[i], MON_DATA_SPECIES) != SPECIES_NONE && !GetMonData(&party[i], MON_DATA_IS_EGG)
		&& GetMonData(&party[i], MON_DATA_HP) != 0 && gBattlerPartyIndexes[gActiveBattler] != i)
                break;
        }
        if (i == lastMonId)
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
        else
            gBattlescriptCurrInstr += 6;
    }
    else
    {
        if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT)
        {
            battlerIn1 = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
		
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                battlerIn2 = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
            else
                battlerIn2 = battlerIn1;
            party = gEnemyParty;
        }
        else
        {
            battlerIn1 = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
		
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                battlerIn2 = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
            else
                battlerIn2 = battlerIn1;
            party = gPlayerParty;
        }
        for (i = 0; i < PARTY_SIZE; ++i)
        {
            if (GetMonData(&party[i], MON_DATA_HP) != 0 && GetMonData(&party[i], MON_DATA_SPECIES) != SPECIES_NONE
		&& !GetMonData(&party[i], MON_DATA_IS_EGG) && i != gBattlerPartyIndexes[battlerIn1] && i != gBattlerPartyIndexes[battlerIn2])
                break;
        }
        if (i == 6)
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
        else
            gBattlescriptCurrInstr += 6;
    }
}

static void sub_8024398(u8 arg0)
{
    *(gBattleStruct->battlerPartyIndexes + gActiveBattler) = gBattlerPartyIndexes[gActiveBattler];
    BtlController_EmitChoosePokemon(0, PARTY_ACTION_SEND_OUT, arg0, 0, gBattleStruct->battlerPartyOrders[gActiveBattler]);
    MarkBattlerForControllerExec(gActiveBattler);
}

static void atk50_openpartyscreen(void)
{
    u8 flag40_0, flag40_1, flag40_2, flag40_3, hitmarkerFaintBits, battlerId = 0;
    const u8 *jumpPtr;
    u32 flags = 0;

    jumpPtr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
	
    if (gBattlescriptCurrInstr[1] == 5)
    {
        if ((gBattleTypeFlags & (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI)) != BATTLE_TYPE_DOUBLE)
        {
            for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
            {
                if (gHitMarker & HITMARKER_FAINTED(gActiveBattler))
                {
                    if (HasNoMonsToSwitch(gActiveBattler, 6, 6))
                    {
                        gAbsentBattlerFlags |= gBitTable[gActiveBattler];
                        gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
                        BtlController_EmitLinkStandbyMsg(0, 2);
                        MarkBattlerForControllerExec(gActiveBattler);
                    }
                    else if (!gSpecialStatuses[gActiveBattler].flag40)
                    {
                        sub_8024398(6);
                        gSpecialStatuses[gActiveBattler].flag40 = 1;
                    }
                }
                else
                {
                    BtlController_EmitLinkStandbyMsg(0, 2);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
            }
        }
        else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
        {
            hitmarkerFaintBits = gHitMarker >> 0x1C;
		
            if (gBitTable[0] & hitmarkerFaintBits)
            {
                gActiveBattler = 0;
		    
                if (HasNoMonsToSwitch(0, 6, 6))
                {
                    gAbsentBattlerFlags |= gBitTable[gActiveBattler];
                    gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
                    BtlController_EmitCmd42(0);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
                else if (!gSpecialStatuses[gActiveBattler].flag40)
                {
                    sub_8024398(gBattleStruct->monToSwitchIntoId[2]);
                    gSpecialStatuses[gActiveBattler].flag40 = 1;
                }
                else
                {
                    BtlController_EmitLinkStandbyMsg(0, 2);
                    MarkBattlerForControllerExec(gActiveBattler);
                    flags |= 1;
                }
            }
            if (gBitTable[2] & hitmarkerFaintBits && !(gBitTable[0] & hitmarkerFaintBits))
            {
                gActiveBattler = 2;
		    
                if (HasNoMonsToSwitch(2, 6, 6))
                {
                    gAbsentBattlerFlags |= gBitTable[gActiveBattler];
                    gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
                    BtlController_EmitCmd42(0);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
                else if (!gSpecialStatuses[gActiveBattler].flag40)
                {
                    sub_8024398(gBattleStruct->monToSwitchIntoId[0]);
                    gSpecialStatuses[gActiveBattler].flag40 = 1;
                }
                else if (!(flags & 1))
                {
                    BtlController_EmitLinkStandbyMsg(0, 2);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
            }
            if (gBitTable[1] & hitmarkerFaintBits)
            {
                gActiveBattler = 1;
		    
                if (HasNoMonsToSwitch(1, 6, 6))
                {
                    gAbsentBattlerFlags |= gBitTable[gActiveBattler];
                    gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
                    BtlController_EmitCmd42(0);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
                else if (!gSpecialStatuses[gActiveBattler].flag40)
                {
                    sub_8024398(gBattleStruct->monToSwitchIntoId[3]);
                    gSpecialStatuses[gActiveBattler].flag40 = 1;
                }
                else
                {
                    BtlController_EmitLinkStandbyMsg(0, 2);
                    MarkBattlerForControllerExec(gActiveBattler);
                    flags |= 2;
                }
            }
            if (gBitTable[3] & hitmarkerFaintBits && !(gBitTable[1] & hitmarkerFaintBits))
            {
                gActiveBattler = 3;
		    
                if (HasNoMonsToSwitch(3, 6, 6))
                {
                    gAbsentBattlerFlags |= gBitTable[gActiveBattler];
                    gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
                    BtlController_EmitCmd42(0);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
                else if (!gSpecialStatuses[gActiveBattler].flag40)
                {
                    sub_8024398(gBattleStruct->monToSwitchIntoId[1]);
                    gSpecialStatuses[gActiveBattler].flag40 = 1;
                }
                else if (!(flags & 2))
                {
                    BtlController_EmitLinkStandbyMsg(0, 2);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
            }
            flag40_0 = gSpecialStatuses[0].flag40;
		
            if (!flag40_0)
            {
                flag40_2 = gSpecialStatuses[2].flag40;
		    
                if (!flag40_2 && hitmarkerFaintBits != 0)
                {
                    if (gAbsentBattlerFlags & gBitTable[0])
                        gActiveBattler = 2;
                    else
                        gActiveBattler = 0;

                    BtlController_EmitLinkStandbyMsg(0, 2);
                    MarkBattlerForControllerExec(gActiveBattler);
                }

            }
            flag40_1 = gSpecialStatuses[1].flag40;
		
            if (!flag40_1)
            {
                flag40_3 = gSpecialStatuses[3].flag40;
		    
                if (!flag40_3 && hitmarkerFaintBits != 0)
                {
                    if (gAbsentBattlerFlags & gBitTable[1])
                        gActiveBattler = 3;
                    else
                        gActiveBattler = 1;
                    BtlController_EmitLinkStandbyMsg(0, 2);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
            }
        }
        gBattlescriptCurrInstr += 6;
    }
    else if (gBattlescriptCurrInstr[1] == 6)
    {
        if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
        {
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            {
                hitmarkerFaintBits = gHitMarker >> 0x1C;
		    
                if (gBitTable[2] & hitmarkerFaintBits && gBitTable[0] & hitmarkerFaintBits)
                {
                    gActiveBattler = 2;
			
                    if (HasNoMonsToSwitch(2, gBattleBufferB[0][1], 6))
                    {
                        gAbsentBattlerFlags |= gBitTable[gActiveBattler];
                        gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
                        BtlController_EmitCmd42(0);
                        MarkBattlerForControllerExec(gActiveBattler);
                    }
                    else if (!gSpecialStatuses[gActiveBattler].flag40)
                    {
                        sub_8024398(gBattleStruct->monToSwitchIntoId[0]);
                        gSpecialStatuses[gActiveBattler].flag40 = 1;
                    }
                }
                if (gBitTable[3] & hitmarkerFaintBits && hitmarkerFaintBits & gBitTable[1])
                {
                    gActiveBattler = 3;
			
                    if (HasNoMonsToSwitch(3, gBattleBufferB[1][1], 6))
                    {
                        gAbsentBattlerFlags |= gBitTable[gActiveBattler];
                        gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
                        BtlController_EmitCmd42(0);
                        MarkBattlerForControllerExec(gActiveBattler);
                    }
                    else if (!gSpecialStatuses[gActiveBattler].flag40)
                    {
                        sub_8024398(gBattleStruct->monToSwitchIntoId[1]);
                        gSpecialStatuses[gActiveBattler].flag40 = 1;
                    }
                }
                gBattlescriptCurrInstr += 6;
            }
            else
                gBattlescriptCurrInstr += 6;
        }
        else
            gBattlescriptCurrInstr += 6;

        hitmarkerFaintBits = gHitMarker >> 0x1C;
        gBattlerFainted = 0;
	    
        while (1)
        {
            if (gBitTable[gBattlerFainted] & hitmarkerFaintBits || gBattlerFainted >= gBattlersCount)
                break;
            ++gBattlerFainted;
        }

        if (gBattlerFainted == gBattlersCount)
            gBattlescriptCurrInstr = jumpPtr;
    }
    else
    {
        if (gBattlescriptCurrInstr[1] & OPEN_PARTY_ALLOW_CANCEL)
            hitmarkerFaintBits = PARTY_ACTION_CHOOSE_MON; // Used here as the caseId for the EmitChoose function.
        else
            hitmarkerFaintBits = PARTY_ACTION_SEND_OUT;
        battlerId = GetBattlerForBattleScript(gBattlescriptCurrInstr[1] & ~(OPEN_PARTY_ALLOW_CANCEL));
	    
        if (gSpecialStatuses[battlerId].flag40)
            gBattlescriptCurrInstr += 6;
        else if (HasNoMonsToSwitch(battlerId, 6, 6))
        {
            gActiveBattler = battlerId;
            gAbsentBattlerFlags |= gBitTable[gActiveBattler];
            gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
            gBattlescriptCurrInstr = jumpPtr;
        }
        else
        {
            gActiveBattler = battlerId;
            *(gBattleStruct->battlerPartyIndexes + gActiveBattler) = gBattlerPartyIndexes[gActiveBattler];
            BtlController_EmitChoosePokemon(0, hitmarkerFaintBits, *(gBattleStruct->monToSwitchIntoId + (gActiveBattler ^ 2)), 0, gBattleStruct->battlerPartyOrders[gActiveBattler]);
            MarkBattlerForControllerExec(gActiveBattler);
            gBattlescriptCurrInstr += 6;
            if (GetBattlerPosition(gActiveBattler) == B_POSITION_PLAYER_LEFT && gBattleResults.playerSwitchesCounter < 0xFF)
                ++gBattleResults.playerSwitchesCounter;

            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                for (gActiveBattler = 0; gActiveBattler < gBattlersCount; ++gActiveBattler)
                {
                    if (gActiveBattler != battlerId)
                    {
                        BtlController_EmitLinkStandbyMsg(0, 2);
                        MarkBattlerForControllerExec(gActiveBattler);
                    }
                }
            }
            else
            {
                gActiveBattler = GetBattlerAtPosition(BATTLE_OPPOSITE(GetBattlerPosition(battlerId)));
		    
                if (gAbsentBattlerFlags & gBitTable[gActiveBattler])
                    gActiveBattler = BATTLE_PARTNER(gActiveBattler);
                BtlController_EmitLinkStandbyMsg(0, 2);
                MarkBattlerForControllerExec(gActiveBattler);
            }
        }
    }
}

static void atk51_switchhandleorder(void)
{
    s32 i;

    if (!gBattleControllerExecFlags)
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	    
        switch (gBattlescriptCurrInstr[2])
        {
        case 0:
            for (i = 0; i < gBattlersCount; ++i)
                if (gBattleBufferB[i][0] == 0x22)
                    *(gBattleStruct->monToSwitchIntoId + i) = gBattleBufferB[i][1];
            break;
        case 1:
            if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
                UpdatePartyOwnerOnSwitch_NonMulti(gActiveBattler);
            break;
        case 2:
            gBattleCommunication[0] = gBattleBufferB[gActiveBattler][1];
            *(gBattleStruct->monToSwitchIntoId + gActiveBattler) = gBattleBufferB[gActiveBattler][1];

            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                *(gActiveBattler * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) &= 0xF;
                *(gActiveBattler * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) |= (gBattleBufferB[gActiveBattler][2] & 0xF0);
                *(gActiveBattler * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 1) = gBattleBufferB[gActiveBattler][3];
                *((BATTLE_PARTNER(gActiveBattler)) * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) &= (0xF0);
                *((BATTLE_PARTNER(gActiveBattler)) * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 0) |= (gBattleBufferB[gActiveBattler][2] & 0xF0) >> 4;
                *((BATTLE_PARTNER(gActiveBattler)) * 3 + (u8 *)(gBattleStruct->battlerPartyOrders) + 2) = gBattleBufferB[gActiveBattler][3];
            }
            else
                UpdatePartyOwnerOnSwitch_NonMulti(gActiveBattler);
            PREPARE_SPECIES_BUFFER(gBattleTextBuff1, gBattleMons[gBattlerAttacker].species)
            PREPARE_MON_NICK_BUFFER(gBattleTextBuff2, gActiveBattler, gBattleBufferB[gActiveBattler][1])
            break;
        }
        gBattlescriptCurrInstr += 3;
    }
}

static void atk52_switchineffects(void)
{
    u8 spikesDmg;
    u32 hitmarkerFaintBits;
    s32 i;

    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    UpdateSentPokesToOpponentValue(gActiveBattler);
    gHitMarker &= ~(HITMARKER_FAINTED(gActiveBattler));
    gSpecialStatuses[gActiveBattler].flag40 = 0;
	
    if (!(gSideStatuses[GetBattlerSide(gActiveBattler)] & SIDE_STATUS_SPIKES_DAMAGED) && (gSideStatuses[GetBattlerSide(gActiveBattler)] & SIDE_STATUS_SPIKES)
	&& !IS_BATTLER_OF_TYPE(gActiveBattler, TYPE_FLYING) && GetBattlerAbility(gActiveBattler) != ABILITY_LEVITATE)
    {
        gSideStatuses[GetBattlerSide(gActiveBattler)] |= SIDE_STATUS_SPIKES_DAMAGED;
        spikesDmg = (5 - gSideTimers[GetBattlerSide(gActiveBattler)].spikesAmount) * 2;
        gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / (spikesDmg);
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        gBattleScripting.battler = gActiveBattler;
        BattleScriptPushCursor();
        if (gBattlescriptCurrInstr[1] == BS_TARGET)
            gBattlescriptCurrInstr = BattleScript_SpikesOnTarget;
        else if (gBattlescriptCurrInstr[1] == BS_ATTACKER)
            gBattlescriptCurrInstr = BattleScript_SpikesOnAttacker;
        else
            gBattlescriptCurrInstr = BattleScript_SpikesOnFaintedBattler;
    }
    else
    {
        if (GetBattlerAbility(gActiveBattler) == ABILITY_TRUANT)
            gDisableStructs[gActiveBattler].truantCounter = 1;
        if (!AbilityBattleEffects(ABILITYEFFECT_ON_SWITCHIN, gActiveBattler, 0, 0, 0) && !ItemBattleEffects(ITEMEFFECT_ON_SWITCH_IN, gActiveBattler, FALSE))
        {
            gSideStatuses[GetBattlerSide(gActiveBattler)] &= ~(SIDE_STATUS_SPIKES_DAMAGED);

            for (i = 0; i < gBattlersCount; ++i)
            {
                if (gBattlerByTurnOrder[i] == gActiveBattler)
                    gActionsByTurnOrder[i] = B_ACTION_CANCEL_PARTNER;
            }
            for (i = 0; i < gBattlersCount; ++i)
                gBattleStruct->hpOnSwitchout[GetBattlerSide(i)] = gBattleMons[i].hp;

            if (gBattlescriptCurrInstr[1] == 5)
            {
                hitmarkerFaintBits = gHitMarker >> 0x1C;
                ++gBattlerFainted;
		    
                while (TRUE)
                {
                    if ((hitmarkerFaintBits & gBitTable[gBattlerFainted] && !(gAbsentBattlerFlags & gBitTable[gBattlerFainted])) || gBattlerFainted >= gBattlersCount)
                        break;
                    ++gBattlerFainted;
                }
            }
            gBattlescriptCurrInstr += 2;
        }
    }
}

static void atk53_trainerslide(void)
{
	u8 slideAnim = gBattlescriptCurrInstr[1];
	bool8 slideOut = slideAnim & ATK53_TRAINER_SLIDE_OUT;
	slideAnim &= ~(ATK53_TRAINER_SLIDE_OUT);
	
	if (!slideAnim)
		gActiveBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
	else
		gActiveBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
	if (!slideOut)
		BtlController_EmitTrainerSlide(0);
	else
		BtlController_EmitTrainerSlideBack(0);
	MarkBattlerForControllerExec(gActiveBattler);
	gBattlescriptCurrInstr += 2;
}

static void atk54_playse(void)
{
    gActiveBattler = gBattlerAttacker;
    BtlController_EmitPlaySE(0, T2_READ_16(gBattlescriptCurrInstr + 1));
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 3;
}

static void atk55_fanfare(void)
{
    gActiveBattler = gBattlerAttacker;
    BtlController_EmitPlayFanfare(0, T2_READ_16(gBattlescriptCurrInstr + 1));
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 3;
}

static void atk56_playfaintcry(void)
{
    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    BtlController_EmitFaintingCry(0);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

static void atk57(void)
{
    gActiveBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
    BtlController_EmitCmd55(0, gBattleOutcome);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 1;
}

static void atk58_returntoball(void)
{
    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    BtlController_EmitReturnMonToBall(0, 1);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

static void atk59_handlelearnnewmove(void)
{
    const u8 *jumpPtr1 = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    const u8 *jumpPtr2 = T1_READ_PTR(gBattlescriptCurrInstr + 5);
    u16 ret = MonTryLearningNewMove(&gPlayerParty[gBattleStruct->expGetterMonId], gBattlescriptCurrInstr[9]);
    
    while (ret == 0xFFFE)
        ret = MonTryLearningNewMove(&gPlayerParty[gBattleStruct->expGetterMonId], 0);
    if (ret == 0)
        gBattlescriptCurrInstr = jumpPtr2;
    else if (ret == 0xFFFF)
        gBattlescriptCurrInstr += 10;
    else
    {
        gActiveBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
	    
        if (gBattlerPartyIndexes[gActiveBattler] == gBattleStruct->expGetterMonId && !(gBattleMons[gActiveBattler].status2 & STATUS2_TRANSFORMED))
            GiveMoveToBattleMon(&gBattleMons[gActiveBattler], ret);
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
        {
            gActiveBattler = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
		
            if (gBattlerPartyIndexes[gActiveBattler] == gBattleStruct->expGetterMonId && !(gBattleMons[gActiveBattler].status2 & STATUS2_TRANSFORMED))
                GiveMoveToBattleMon(&gBattleMons[gActiveBattler], ret);
        }
        gBattlescriptCurrInstr = jumpPtr1;
    }
}

static void atk5A_yesnoboxlearnmove(void)
{
    u8 movePosition;
    u16 moveId;
	
    gActiveBattler = 0;

    switch (gBattleScripting.learnMoveState)
    {
    case 0:
        HandleBattleWindow(0x17, 8, 0x1D, 0xD, 0);
        BattlePutTextOnWindow(gText_BattleYesNoChoice, 0xE);
        ++gBattleScripting.learnMoveState;
        gBattleCommunication[CURSOR_POSITION] = 0;
        BattleCreateYesNoCursorAt();
        break;
    case 1:
        if (JOY_NEW(DPAD_UP) && gBattleCommunication[CURSOR_POSITION] != 0)
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
            if (gBattleCommunication[1] == 0)
            {
                HandleBattleWindow(0x17, 0x8, 0x1D, 0xD, WINDOW_CLEAR);
                BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 0x10, RGB_BLACK);
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
            movePosition = GetMoveSlotToReplace();

            if (movePosition == 4)
                gBattleScripting.learnMoveState = 4;
            else
            {
                moveId = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_MOVE1 + movePosition);
                
                if (IsHMMove2(moveId))
                {
                    PrepareStringBattle(STRINGID_HMMOVESCANTBEFORGOTTEN, gActiveBattler);
                    gBattleScripting.learnMoveState = 5;
                }
                else
                {
                    gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
                    PREPARE_MOVE_BUFFER(gBattleTextBuff2, moveId)
                    RemoveMonPPBonus(&gPlayerParty[gBattleStruct->expGetterMonId], movePosition);
                    SetMonMoveSlot(&gPlayerParty[gBattleStruct->expGetterMonId], gMoveToLearn, movePosition);
                    if (gBattlerPartyIndexes[0] == gBattleStruct->expGetterMonId && !(gBattleMons[0].status2 & STATUS2_TRANSFORMED)
			&& !(gDisableStructs[0].mimickedMoves & gBitTable[movePosition]))
                    {
                        RemoveBattleMonPPBonus(&gBattleMons[0], movePosition);
                        SetBattleMonMoveSlot(&gBattleMons[0], gMoveToLearn, movePosition);
                    }
                    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && gBattlerPartyIndexes[2] == gBattleStruct->expGetterMonId
			&& !(gBattleMons[2].status2 & STATUS2_TRANSFORMED) && !(gDisableStructs[2].mimickedMoves & gBitTable[movePosition]))
                    {
                        RemoveBattleMonPPBonus(&gBattleMons[2], movePosition);
                        SetBattleMonMoveSlot(&gBattleMons[2], gMoveToLearn, movePosition);
                    }
                }
            }
        }
        break;
    case 4:
        HandleBattleWindow(0x17, 8, 0x1D, 0xD, WINDOW_CLEAR);
        gBattlescriptCurrInstr += 5;
        break;
    case 5:
        if (!gBattleControllerExecFlags)
            gBattleScripting.learnMoveState = 2;
        break;
    }
}

static void atk5B_yesnoboxstoplearningmove(void)
{
    switch (gBattleScripting.learnMoveState)
    {
    case 0:
        HandleBattleWindow(0x17, 8, 0x1D, 0xD, 0);
        BattlePutTextOnWindow(gText_BattleYesNoChoice, 0xE);
        ++gBattleScripting.learnMoveState;
        gBattleCommunication[CURSOR_POSITION] = 0;
        BattleCreateYesNoCursorAt();
        break;
    case 1:
        if (JOY_NEW(DPAD_UP) && gBattleCommunication[CURSOR_POSITION] != 0)
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

            if (gBattleCommunication[1] != 0)
                gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
            else
                gBattlescriptCurrInstr += 5;
            HandleBattleWindow(0x17, 0x8, 0x1D, 0xD, WINDOW_CLEAR);
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
            HandleBattleWindow(0x17, 0x8, 0x1D, 0xD, WINDOW_CLEAR);
        }
        break;
    }
}

static void atk5C_hitanimation(void)
{
    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	
    if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
        gBattlescriptCurrInstr += 2;
    else if (!(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE) || !SubsBlockMove(gBattlerAttacker, gActiveBattler, gCurrentMove)
	     || gDisableStructs[gActiveBattler].substituteHP == 0)
    {
        BtlController_EmitHitAnimation(0);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 2;
    }
    else
        gBattlescriptCurrInstr += 2;
}

static void atk5D_getmoneyreward(void)
{ 
    u8 lastMonLevel = 0;
    u32 moneyReward, i = 0;
    const struct TrainerMonItemCustomMoves *party4; //This needs to be out here

    if (gBattleOutcome == B_OUTCOME_WON)
    {
        if (gTrainerBattleOpponent_A == TRAINER_SECRET_BASE)
            moneyReward = gBattleResources->secretBase->party.levels[0] * 20 * gBattleStruct->moneyMultiplier;
        else
        {
            switch (gTrainers[gTrainerBattleOpponent_A].partyFlags)
            {
            case 0:
                {
                    const struct TrainerMonNoItemDefaultMoves *party1 = gTrainers[gTrainerBattleOpponent_A].party.NoItemDefaultMoves;
                    lastMonLevel = party1[gTrainers[gTrainerBattleOpponent_A].partySize - 1].lvl;
                }
                break;
            case F_TRAINER_PARTY_CUSTOM_MOVESET:
                {
                    const struct TrainerMonNoItemCustomMoves *party2 = gTrainers[gTrainerBattleOpponent_A].party.NoItemCustomMoves;
                    lastMonLevel = party2[gTrainers[gTrainerBattleOpponent_A].partySize - 1].lvl;
                }
                break;
            case F_TRAINER_PARTY_HELD_ITEM:
                {
                    const struct TrainerMonItemDefaultMoves *party3 = gTrainers[gTrainerBattleOpponent_A].party.ItemDefaultMoves;
                    lastMonLevel = party3[gTrainers[gTrainerBattleOpponent_A].partySize - 1].lvl;
                }
                break;
            case (F_TRAINER_PARTY_CUSTOM_MOVESET | F_TRAINER_PARTY_HELD_ITEM):
                {
                    party4 = gTrainers[gTrainerBattleOpponent_A].party.ItemCustomMoves;
                    lastMonLevel = party4[gTrainers[gTrainerBattleOpponent_A].partySize - 1].lvl;
                }
                break;
            }
            for (; gTrainerMoneyAndBallTable[i].classId != 0xFF; i++)
            {
                if (gTrainerMoneyAndBallTable[i].classId == gTrainers[gTrainerBattleOpponent_A].trainerClass)
                    break;
            }
            party4 = gTrainers[gTrainerBattleOpponent_A].party.ItemCustomMoves; // Needed to Match. Has no effect.
            moneyReward = 4 * lastMonLevel * gBattleStruct->moneyMultiplier * (gBattleTypeFlags & BATTLE_TYPE_DOUBLE ? 2 : 1) * gTrainerMoneyAndBallTable[i].value;
        }
        AddMoney(&gSaveBlock1Ptr->money, moneyReward);
    }
    else
        moneyReward = ComputeWhiteOutMoneyLoss();
    PREPARE_WORD_NUMBER_BUFFER(gBattleTextBuff1, 5, moneyReward);
    if (moneyReward)
        gBattlescriptCurrInstr += 5;
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atk5E(void)
{
    s32 i;
	
    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

    switch (gBattleCommunication[0])
    {
    case 0:
        BtlController_EmitGetMonData(0, REQUEST_ALL_BATTLE, 0);
        MarkBattlerForControllerExec(gActiveBattler);
        ++gBattleCommunication[0];
        break;
    case 1:
         if (!gBattleControllerExecFlags)
         {
            struct BattlePokemon *bufferPoke = (struct BattlePokemon *) &gBattleBufferB[gActiveBattler][4];
            
            for (i = 0; i < MAX_MON_MOVES; ++i)
            {
                gBattleMons[gActiveBattler].moves[i] = bufferPoke->moves[i];
                gBattleMons[gActiveBattler].pp[i] = bufferPoke->pp[i];
            }
            gBattlescriptCurrInstr += 2;
         }
         break;
    }
}

static void atk5F_swapattackerwithtarget(void)
{
    gActiveBattler = gBattlerAttacker;
    gBattlerAttacker = gBattlerTarget;
    gBattlerTarget = gActiveBattler;
	
    if (gHitMarker & HITMARKER_SWAP_ATTACKER_TARGET)
        gHitMarker &= ~(HITMARKER_SWAP_ATTACKER_TARGET);
    else
        gHitMarker |= HITMARKER_SWAP_ATTACKER_TARGET;
    ++gBattlescriptCurrInstr;
}

static void atk60_incrementgamestat(void)
{
    if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
        IncrementGameStat(gBattlescriptCurrInstr[1]);
    gBattlescriptCurrInstr += 2;
}

static void atk61_drawpartystatussummary(void)
{
    s32 i;
    struct Pokemon *party;
    struct HpAndStatus hpStatuses[PARTY_SIZE];

    if (!gBattleControllerExecFlags)
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

        if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
            party = gPlayerParty;
        else
            party = gEnemyParty;

        for (i = 0; i < PARTY_SIZE; ++i)
        {
            if (GetMonData(&party[i], MON_DATA_SPECIES2) == SPECIES_NONE || GetMonData(&party[i], MON_DATA_SPECIES2) == SPECIES_EGG)
            {
                hpStatuses[i].hp = 0xFFFF;
                hpStatuses[i].status = 0;
            }
            else
            {
                hpStatuses[i].hp = GetMonData(&party[i], MON_DATA_HP);
                hpStatuses[i].status = GetMonData(&party[i], MON_DATA_STATUS);
            }
        }
        BtlController_EmitDrawPartyStatusSummary(0, hpStatuses, 1);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 2;
    }
}

static void atk62_hidepartystatussummary(void)
{
    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    BtlController_EmitHidePartyStatusSummary(0);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

static void atk63_jumptocalledmove(void)
{
    if (gBattlescriptCurrInstr[1])
        gCurrentMove = gCalledMove;
    else
        gChosenMove = gCurrentMove = gCalledMove;
    gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
}

static void atk64_statusanimation(void)
{
    if (!gBattleControllerExecFlags)
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	    
        if (!(gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE) && gDisableStructs[gActiveBattler].substituteHP == 0 && !(gHitMarker & HITMARKER_NO_ANIMATIONS))
        {
            BtlController_EmitStatusAnimation(0, FALSE, gBattleMons[gActiveBattler].status1);
            MarkBattlerForControllerExec(gActiveBattler);
        }
        gBattlescriptCurrInstr += 2;
    }
}

static void atk65_status2animation(void)
{
    u32 wantedToAnimate;

    if (!gBattleControllerExecFlags)
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
        wantedToAnimate = T1_READ_32(gBattlescriptCurrInstr + 2);
	    
        if (!(gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE) && gDisableStructs[gActiveBattler].substituteHP == 0 && !(gHitMarker & HITMARKER_NO_ANIMATIONS))
        {
            BtlController_EmitStatusAnimation(0, TRUE, gBattleMons[gActiveBattler].status2 & wantedToAnimate);
            MarkBattlerForControllerExec(gActiveBattler);
        }
        gBattlescriptCurrInstr += 6;
    }
}

static void atk66_chosenstatusanimation(void)
{
    u32 wantedStatus;

    if (!gBattleControllerExecFlags)
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
        wantedStatus = T1_READ_32(gBattlescriptCurrInstr + 3);
	    
        if (!(gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE) && gDisableStructs[gActiveBattler].substituteHP == 0 && !(gHitMarker & HITMARKER_NO_ANIMATIONS))
        {
            BtlController_EmitStatusAnimation(0, gBattlescriptCurrInstr[2], wantedStatus);
            MarkBattlerForControllerExec(gActiveBattler);
        }
        gBattlescriptCurrInstr += 7;
    }
}

static void atk67_yesnobox(void)
{
    switch (gBattleCommunication[0])
    {
    case 0:
        HandleBattleWindow(0x17, 8, 0x1D, 0xD, 0);
        BattlePutTextOnWindow(gText_BattleYesNoChoice, 0xE);
        ++gBattleCommunication[0];
        gBattleCommunication[CURSOR_POSITION] = 0;
        BattleCreateYesNoCursorAt();
        break;
    case 1:
        if (JOY_NEW(DPAD_UP) && gBattleCommunication[CURSOR_POSITION] != 0)
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
            ++gBattlescriptCurrInstr;
        }
        else if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            HandleBattleWindow(0x17, 8, 0x1D, 0xD, WINDOW_CLEAR);
            ++gBattlescriptCurrInstr;
        }
        break;
    }
}

static void atk68_cancelallactions(void)
{
    s32 i;

    for (i = 0; i < gBattlersCount; ++i)
        gActionsByTurnOrder[i] = B_ACTION_CANCEL_PARTNER;
    ++gBattlescriptCurrInstr;
}

static void atk69_nop(void)
{
    ++gBattlescriptCurrInstr;
}

static void atk6A_removeitem(void)
{
    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    gBattleStruct->usedHeldItems[gBattlerPartyIndexes[gActiveBattler]][GetBattlerSide(gActiveBattler)] = gBattleMons[gActiveBattler].item;
    gBattleMons[gActiveBattler].item = ITEM_NONE;
    TryGiveUnburdenBoostToMon(gActiveBattler); // try give unburden boost to mon
    BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

static void atk6B_atknameinbuff1(void)
{
    PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, gBattlerAttacker, gBattlerPartyIndexes[gBattlerAttacker])
    ++gBattlescriptCurrInstr;
}

static void atk6C_drawlvlupbox(void)
{
    if (gBattleScripting.atk6C_state == 0)
    {
        if (IsMonGettingExpSentOut())
            gBattleScripting.atk6C_state = 3;
        else
            gBattleScripting.atk6C_state = 1;
    }

    switch (gBattleScripting.atk6C_state)
    {
    case 1:
        gBattle_BG2_Y = 0x60;
        SetBgAttribute(2, BG_ATTR_PRIORITY, 0);
        ShowBg(2);
        sub_8026480();
        gBattleScripting.atk6C_state = 2;
        break;
    case 2:
        if (!sub_80264D0())
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
        if (!sub_8026648())
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
            ++gBattlescriptCurrInstr;
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

static void sub_8026480(void)
{
    gBattle_BG2_Y = 0;
    gBattle_BG2_X = 0x1A0;
    LoadPalette(sUnknownBattleboxPal, 0x60, 0x20);
    CopyToWindowPixelBuffer(13, sUnknownBattleboxGfx, 0, 0);
    PutWindowTilemap(13);
    CopyWindowToVram(13, COPYWIN_BOTH);
    PutMonIconOnLvlUpBox();
}

static bool8 sub_80264D0(void)
{
    if (IsDma3ManagerBusyWithBgCopy())
        return TRUE;
    if (gBattle_BG2_X == 0x200)
        return FALSE;
    if (gBattle_BG2_X == 0x1A0)
        PutLevelAndGenderOnLvlUpBox();
    gBattle_BG2_X += 8;
    if (gBattle_BG2_X >= 0x200)
        gBattle_BG2_X = 0x200;
    return (gBattle_BG2_X != 0x200);
}

static void PutLevelAndGenderOnLvlUpBox(void)
{
    u8 monGender, *txtPtr, *txtPtr2;
    u16 monLevel;
    struct TextPrinterTemplate printerTemplate;

    monLevel = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_LEVEL);
    monGender = GetMonGender(&gPlayerParty[gBattleStruct->expGetterMonId]);
    GetMonNickname(&gPlayerParty[gBattleStruct->expGetterMonId], gStringVar4);
    printerTemplate.currentChar = gStringVar4;
    printerTemplate.windowId = 13;
    printerTemplate.fontId = 0;
    printerTemplate.x = 32;
    printerTemplate.y = 0;
    printerTemplate.currentX = 32;
    printerTemplate.currentY = 0;
    printerTemplate.letterSpacing = 0;
    printerTemplate.lineSpacing = 0;
    printerTemplate.unk = 0;
    printerTemplate.fgColor = TEXT_COLOR_WHITE;
    printerTemplate.bgColor = TEXT_COLOR_TRANSPARENT;
    printerTemplate.shadowColor = TEXT_COLOR_DARK_GRAY;
    AddTextPrinter(&printerTemplate, 0xFF, NULL);
    txtPtr = gStringVar4;
    gStringVar4[0] = 0xF9;
    *++txtPtr = 5;
    *++txtPtr = 0;
    txtPtr2 = txtPtr + 1;
    txtPtr = ConvertIntToDecimalStringN(++txtPtr, monLevel, STR_CONV_MODE_LEFT_ALIGN, 3);
    txtPtr = StringFill(txtPtr, 0, 5);
    txtPtr = txtPtr2 + 4;
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

static bool8 sub_8026648(void)
{
    if (gBattle_BG2_X == 0x1A0)
        return FALSE;
    if (gBattle_BG2_X - 16 < 0x1A0)
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
    const u16 *iconPal;
    struct SpriteSheet iconSheet;
    struct SpritePalette iconPalSheet;
    u16 species = GetMonData(&gPlayerParty[gBattleStruct->expGetterMonId], MON_DATA_SPECIES);
    const u8 *iconPtr = GetMonIconPtr(species);

    iconSheet.data = iconPtr;
    iconSheet.size = 0x200;
    iconSheet.tag = MON_ICON_LVLUP_BOX_TAG;
    iconPal = GetValidMonIconPalettePtr(species);
    iconPalSheet.data = iconPal;
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
    if (gBattlerPartyIndexes[0] == gBattleStruct->expGetterMonId || (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && gBattlerPartyIndexes[2] == gBattleStruct->expGetterMonId))
        return TRUE;
    else
	return FALSE;
}

static void atk6D_resetsentmonsvalue(void)
{
    ResetSentPokesToOpponentValue();
    ++gBattlescriptCurrInstr;
}

static void atk6E_setatktoplayer0(void)
{
    gBattlerAttacker = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
    ++gBattlescriptCurrInstr;
}

static void atk6F_makevisible(void)
{
    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    BtlController_EmitSpriteInvisibility(0, FALSE);
    MarkBattlerForControllerExec(gActiveBattler);
    gBattlescriptCurrInstr += 2;
}

static void atk70_recordlastability(void)
{
    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
    RecordAbilityBattle(gActiveBattler, gLastUsedAbility);
    gBattlescriptCurrInstr += 2;
}

void BufferMoveToLearnIntoBattleTextBuff2(void)
{
    PREPARE_MOVE_BUFFER(gBattleTextBuff2, gMoveToLearn);
}

static void atk71_buffermovetolearn(void)
{
    BufferMoveToLearnIntoBattleTextBuff2();
    ++gBattlescriptCurrInstr;
}

static void atk72_jumpifplayerran(void)
{
    if (TryRunFromBattle(gBattlerFainted))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
        gBattlescriptCurrInstr += 5;
}

static void atk73_hpthresholds(void)
{
    u8 opposingBattler;
    s32 result;

    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
        opposingBattler = BATTLE_OPPOSITE(gActiveBattler);
        result = gBattleMons[opposingBattler].hp * 100 / gBattleMons[opposingBattler].maxHP;
	    
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
    gBattlescriptCurrInstr += 2;
}

static void atk74_hpthresholds2(void)
{
    u8 hpSwitchout, opposingBattler;
    s32 result;

    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
    {
        gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
        opposingBattler = BATTLE_OPPOSITE(gActiveBattler);
        hpSwitchout = *(gBattleStruct->hpOnSwitchout + GetBattlerSide(opposingBattler));
        result = (hpSwitchout - gBattleMons[opposingBattler].hp) * 100 / hpSwitchout;

        if (gBattleMons[opposingBattler].hp >= hpSwitchout)
            gBattleStruct->hpScale = 0;
        else if (result <= 29)
            gBattleStruct->hpScale = 1;
        else if (result <= 69)
            gBattleStruct->hpScale = 2;
        else
            gBattleStruct->hpScale = 3;
    }
    gBattlescriptCurrInstr += 2;
}

static void atk75_useitemonopponent(void)
{
    gBattlerInMenuId = gBattlerAttacker;
    PokemonUseItemEffects(&gEnemyParty[gBattlerPartyIndexes[gBattlerAttacker]], gLastUsedItem, gBattlerPartyIndexes[gBattlerAttacker], 0, 1);
    ++gBattlescriptCurrInstr;
}

static void atk76_various(void)
{
    u8 byte;
    u16 species, *choicedMove;
    u32 monToCheck, status;
    s32 i;
    struct Pokemon *mon;

    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

    switch (gBattlescriptCurrInstr[2])
    {
    case VARIOUS_CANCEL_MULTI_TURN_MOVES:
        CancelMultiTurnMoves(gActiveBattler);
        break;
    case VARIOUS_SET_MAGIC_COAT_TARGET:
	    gBattleStruct->attackerBeforeBounce = gActiveBattler;
        gBattlerAttacker = gBattlerTarget;
        byte = BATTLE_OPPOSITE(GetBattlerSide(gBattlerAttacker));
		    
        if (gSideTimers[byte].followmeTimer != 0 && IsBattlerAlive(gSideTimers[byte].followmeTarget))
            gBattlerTarget = gSideTimers[byte].followmeTarget;
        else
            gBattlerTarget = gActiveBattler;
        break;
    case VARIOUS_IS_RUNNING_IMPOSSIBLE:
        gBattleCommunication[0] = IsRunningFromBattleImpossible();
        break;
    case VARIOUS_GET_MOVE_TARGET:
        gBattlerTarget = GetMoveTarget(gCurrentMove, 0);
        break;
    case VARIOUS_CASE_4:
        if (gHitMarker & HITMARKER_FAINTED(gActiveBattler))
            gBattleCommunication[0] = 1;
        else
            gBattleCommunication[0] = 0;
        break;
    case VARIOUS_RESET_INTIMIDATE_TRACE_BITS:
        gSpecialStatuses[gActiveBattler].intimidatedMon = 0;
        gSpecialStatuses[gActiveBattler].traced = 0;
        break;
    case VARIOUS_UPDATE_CHOICE_MOVE_ON_LVL_UP:
        if (gBattlerPartyIndexes[0] == gBattleStruct->expGetterMonId || gBattlerPartyIndexes[2] == gBattleStruct->expGetterMonId)
        {
            if (gBattlerPartyIndexes[0] == gBattleStruct->expGetterMonId)
                gActiveBattler = 0;
            else
                gActiveBattler = 2;
            choicedMove = &gBattleStruct->choicedMove[gActiveBattler];
            for (i = 0; i < MAX_MON_MOVES; ++i)
            {
                if (gBattleMons[gActiveBattler].moves[i] == *choicedMove)
                    break;
            }
            if (i == MAX_MON_MOVES)
                *choicedMove = MOVE_NONE;
        }
        break;
    case VARIOUS_RESET_PLAYER_FAINTED_FLAG:
        if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_DOUBLE)) && gBattleTypeFlags & BATTLE_TYPE_TRAINER
	    && gBattleMons[0].hp != 0 && gBattleMons[1].hp != 0) 
		gHitMarker &= ~(HITMARKER_PLAYER_FAINTED);
        break;
    case VARIOUS_CASE_8:
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        gActiveBattler = 1;
        for (i = 0; gActiveBattler < MAX_BATTLERS_COUNT; gActiveBattler += 2, i++)
        {
            if (IsBattlerAlive(gActiveBattler))
                gBattleCommunication[MULTISTRING_CHOOSER] |= gBitTable[i];
        }
        break;
    case VARIOUS_RETURN_OPPONENT_MON1:
        gActiveBattler = 1;
        if (gBattleMons[gActiveBattler].hp != 0)
        {
            BtlController_EmitReturnMonToBall(0, 0);
            MarkBattlerForControllerExec(gActiveBattler);
        }
        break;
    case VARIOUS_RETURN_OPPONENT_MON2:
        if (gBattlersCount > 3)
        {
            gActiveBattler = 3;
            if (gBattleMons[gActiveBattler].hp != 0)
            {
                BtlController_EmitReturnMonToBall(0, 0);
                MarkBattlerForControllerExec(gActiveBattler);
            }
        }
        break;
    case VARIOUS_CHECK_POKEFLUTE:
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        monToCheck = 0;
        for (i = 0; i < gBattlersCount; ++i)
        {
            if (GetBattlerAbility(i) != ABILITY_SOUNDPROOF)
            {
                gBattleMons[i].status1 &= ~STATUS1_SLEEP;
                gBattleMons[i].status2 &= ~STATUS2_NIGHTMARE;

            }
        }
        for (i = 0; i < PARTY_SIZE; ++i)
        {
            species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2);
            byte = GetMonData(&gPlayerParty[i], MON_DATA_ABILITY_NUM);
            status = GetMonData(&gPlayerParty[i], MON_DATA_STATUS);
		
            if (species != SPECIES_NONE && species != SPECIES_EGG && status & AILMENT_FNT && GetAbilityBySpecies(species, byte, GetMonData(&gPlayerParty[i], MON_DATA_ABILITY_HIDDEN)) != ABILITY_SOUNDPROOF)
                monToCheck |= (1 << i);
        }
        if (monToCheck)
        {
            gActiveBattler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
            status = 0;
            BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, monToCheck, 4, &status);
            MarkBattlerForControllerExec(gActiveBattler);
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
        }
        monToCheck = 0;
		    
        for (i = 0; i < PARTY_SIZE; ++i)
        {
            species = GetMonData(&gEnemyParty[i], MON_DATA_SPECIES2);
            byte = GetMonData(&gEnemyParty[i], MON_DATA_ABILITY_NUM);
            status = GetMonData(&gEnemyParty[i], MON_DATA_STATUS);

            if (species != SPECIES_NONE && species != SPECIES_EGG && status & AILMENT_FNT && GetAbilityBySpecies(species, byte, GetMonData(&gEnemyParty[i], MON_DATA_ABILITY_HIDDEN)) != ABILITY_SOUNDPROOF)
                monToCheck |= (1 << i);
        }
        if (monToCheck)
        {
            gActiveBattler = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
            status = 0;
            BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, monToCheck, 4, &status);
            MarkBattlerForControllerExec(gActiveBattler);
            gBattleCommunication[5] = 1;
        }
        break;
    case VARIOUS_WAIT_FANFARE:
        if (!IsFanfareTaskInactive())
            return;
        break;
    case VARIOUS_TRY_ACTIVATE_MOXIE:
	    if (GetBattlerAbility(gActiveBattler) == ABILITY_MOXIE && HasAttackerFaintedTarget() && !NoAliveMonsForEitherParty() && gBattleMons[gActiveBattler].statStages[STAT_ATK] < 0xC)
		{
			gBattleMons[gActiveBattler].statStages[STAT_ATK]++;
			SET_STATCHANGER(STAT_ATK, 1, FALSE);
            PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_ATK);
			gSetWordLoc = sDownloadString;
            BattleScriptPush(gBattlescriptCurrInstr + 3);
			gLastUsedAbility = GetBattlerAbility(gActiveBattler);
			gBattlescriptCurrInstr = BattleScript_RaiseStatOnFaintingTarget;
            return;
		}
	    break;
    case VARIOUS_JUMP_IF_TARGET_ALLY:
	if (GetBattlerSide(gBattlerAttacker) != GetBattlerSide(gBattlerTarget))
		gBattlescriptCurrInstr += 7;
	else
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
	return;
    case VARIOUS_TRAINER_SLIDE_FIRST_MON_DOWN:
	if (ShouldDoTrainerSlide(gActiveBattler, gTrainerBattleOpponent_A, TRAINER_SLIDE_FIRST_MON_DOWN))
	{
		BattleScriptPush(gBattlescriptCurrInstr + 3);
		gBattlescriptCurrInstr = BattleScript_TrainerSlideMsg;
		return;
	}
	break;
    case VARIOUS_TRAINER_SLIDE_LAST_MON:
        if (ShouldDoTrainerSlide(gActiveBattler, gTrainerBattleOpponent_A, TRAINER_SLIDE_LAST_MON))
	{
		BattleScriptPush(gBattlescriptCurrInstr + 3);
		gBattlescriptCurrInstr = BattleScript_TrainerSlideMsg;
		return;
	}
	break;
    case VARIOUS_UPDATE_NICK:
	UpdateHealthboxAttribute(gHealthboxSpriteIds[gActiveBattler], GetBattlerPartyIndexPtr(gActiveBattler), HEALTHBOX_NICK);
	break;
    case VARIOUS_SET_SPRITEIGNORE0HP:
        gBattleStruct->spriteIgnore0Hp ^= 1;
        break;
    case VARIOUS_TRY_REMOVE_ILLUSION:
        gBattlescriptCurrInstr += 3;
	TryRemoveIllusion(gActiveBattler);
	return;
	case VARIOUS_PLAY_TRANSFORM_ANIMATION:
        BtlController_EmitMoveAnimation(0, MOVE_TRANSFORM, 0, 1, 1, 0xFF, &gDisableStructs[gActiveBattler]);
	MarkBattlerForControllerExec(gActiveBattler);
	break;
	case VARIOUS_JUMP_IF_BATTLE_END:
	    if (NoAliveMonsForEitherParty())
			gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
		else
			gBattlescriptCurrInstr += 7;
		return;
	case VARIOUS_TRY_REMOVE_PRIMAL_WEATHER:
	{
		bool8 shouldNotClear = FALSE;
		
		for (i = 0; i < gBattlersCount; i++)
		{
			if (IsBattlerAlive(i) && ((gBattleWeather & WEATHER_RAIN_PRIMAL && GetBattlerAbility(i) == ABILITY_PRIMORDIAL_SEA) || (gBattleWeather & WEATHER_SUN_PRIMAL
			&& GetBattlerAbility(i) == ABILITY_DESOLATE_LAND) || (gBattleWeather & WEATHER_STRONG_WINDS && GetBattlerAbility(i) == ABILITY_DELTA_STREAM)))
			    shouldNotClear = TRUE;
		}
		if (!shouldNotClear)
		{
			if (gBattleWeather & WEATHER_RAIN_PRIMAL)
			{
				gBattleWeather &= ~(WEATHER_RAIN_PRIMAL);
				PrepareStringBattle(STRINGID_HEAVYRAINLIFTED, gActiveBattler);
				gBattleCommunication[MSG_DISPLAY] = 1;
			}
			else if (gBattleWeather & WEATHER_SUN_PRIMAL)
			{
				gBattleWeather &= ~(WEATHER_SUN_PRIMAL);
				PrepareStringBattle(STRINGID_EXTREMESUNLIGHTFADED, gActiveBattler);
				gBattleCommunication[MSG_DISPLAY] = 1;
			}
			else if (gBattleWeather & WEATHER_STRONG_WINDS)
			{
				gBattleWeather &= ~(WEATHER_STRONG_WINDS);
				PrepareStringBattle(STRINGID_STRONGWINDSDISSIPATED, gActiveBattler);
				gBattleCommunication[MSG_DISPLAY] = 1;
			}
		}
		break;
	}
    }
    gBattlescriptCurrInstr += 3;
}

static void atk77_setprotectlike(void)
{
    bool8 notLastTurn = TRUE;
    u16 lastMove = gLastResultingMoves[gBattlerAttacker];

    if (lastMove != MOVE_PROTECT && lastMove != MOVE_DETECT && lastMove != MOVE_ENDURE)
        gDisableStructs[gBattlerAttacker].protectUses = 0;
    if (gCurrentTurnActionNumber == (gBattlersCount - 1))
        notLastTurn = FALSE;
    if (sProtectSuccessRates[gDisableStructs[gBattlerAttacker].protectUses] >= Random() && notLastTurn)
    {
        if (gBattleMoves[gCurrentMove].effect == EFFECT_PROTECT)
        {
            gProtectStructs[gBattlerAttacker].protected = 1;
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        }
        if (gBattleMoves[gCurrentMove].effect == EFFECT_ENDURE)
        {
            gProtectStructs[gBattlerAttacker].endured = 1;
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
        }
        ++gDisableStructs[gBattlerAttacker].protectUses;
    }
    else
    {
        gDisableStructs[gBattlerAttacker].protectUses = 0;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
        gMoveResultFlags |= MOVE_RESULT_MISSED;
    }
    ++gBattlescriptCurrInstr;
}

static void atk78_faintifabilitynotdamp(void)
{
    if (!gBattleControllerExecFlags)
    {
        for (gBattlerTarget = 0; gBattlerTarget < gBattlersCount; ++gBattlerTarget)
            if (GetBattlerAbility(gBattlerTarget) == ABILITY_DAMP)
                break;
        if (gBattlerTarget == gBattlersCount)
        {
            gActiveBattler = gBattlerAttacker;
            gBattleMoveDamage = gBattleMons[gActiveBattler].hp;
            BtlController_EmitHealthBarUpdate(0, INSTANT_HP_BAR_DROP);
            MarkBattlerForControllerExec(gActiveBattler);
            ++gBattlescriptCurrInstr;

            for (gBattlerTarget = 0; gBattlerTarget < gBattlersCount; ++gBattlerTarget)
                if (gBattlerTarget != gBattlerAttacker && !(gAbsentBattlerFlags & gBitTable[gBattlerTarget]))
                    break;
        }
        else
        {
            gLastUsedAbility = ABILITY_DAMP;
            RecordAbilityBattle(gBattlerTarget, gBattleMons[gBattlerTarget].ability);
            gBattlescriptCurrInstr = BattleScript_DampStopsExplosion;
        }
    }
}

static void atk79_setatkhptozero(void)
{
    if (!gBattleControllerExecFlags)
    {
        gActiveBattler = gBattlerAttacker;
        gBattleMons[gActiveBattler].hp = 0;
        BtlController_EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].hp);
        MarkBattlerForControllerExec(gActiveBattler);
        ++gBattlescriptCurrInstr;
    }
}

static void atk7A_jumpifnexttargetvalid(void)
{
    const u8 *jumpPtr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        for (++gBattlerTarget; ; ++gBattlerTarget)
            if (gBattlerTarget != gBattlerAttacker && !(gAbsentBattlerFlags & gBitTable[gBattlerTarget]))
                break;
        if (gBattlerTarget >= gBattlersCount)
            gBattlescriptCurrInstr += 5;
        else
            gBattlescriptCurrInstr = jumpPtr;
    }
    else
    {
        gBattlescriptCurrInstr += 5;
    }
}

static void atk7B_tryhealhalfhealth(void)
{
    const u8 *failPtr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

    if (gBattlescriptCurrInstr[5] == BS_ATTACKER)
        gBattlerTarget = gBattlerAttacker;
    gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP / 2;
    if (gBattleMoveDamage == 0)
        gBattleMoveDamage = 1;
    gBattleMoveDamage *= -1;
    if (gBattleMons[gBattlerTarget].hp == gBattleMons[gBattlerTarget].maxHP)
        gBattlescriptCurrInstr = failPtr;
    else
        gBattlescriptCurrInstr += 6;
}

static void atk7C_trymirrormove(void)
{
    u16 move, movesArray[3];
    s32 i, validMovesCount;

    for (i = 0; i < 3; ++i)
        movesArray[i] = 0;
    for (validMovesCount = 0, i = 0; i < gBattlersCount; ++i)
    {
        if (i != gBattlerAttacker)
        {
            move = T1_READ_16(i * 2 + gBattlerAttacker * 8 + gBattleStruct->lastTakenMoveFrom);
		
            if (move != MOVE_NONE && move != 0xFFFF)
                movesArray[validMovesCount++] = move;
        }
    }
    move = T1_READ_16(gBattleStruct->lastTakenMove + gBattlerAttacker * 2);
    move++;
    move--;
	
    if (move != MOVE_NONE && move != 0xFFFF)
    {
	gCurrentMove = move;
        gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
        gBattlerTarget = GetMoveTarget(gCurrentMove, 0);
        gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
    }
    else if (validMovesCount != 0)
    {
        gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
        i = Random() % validMovesCount;
        gCurrentMove = movesArray[i];
        gBattlerTarget = GetMoveTarget(gCurrentMove, 0);
        gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
    }
    else
    {
        gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = TRUE;
        ++gBattlescriptCurrInstr;
    }
}

static void atk7D_setrain(void)
{
    if (!TryChangeBattleWeather(gBattlerAttacker, ENUM_WEATHER_RAIN))
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
    }
    else
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	
    ++gBattlescriptCurrInstr;
}

static void atk7E_setreflect(void)
{
    if (gSideStatuses[GET_BATTLER_SIDE(gBattlerAttacker)] & SIDE_STATUS_REFLECT)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
    else
    {
        gSideStatuses[GET_BATTLER_SIDE(gBattlerAttacker)] |= SIDE_STATUS_REFLECT;
        gSideTimers[GET_BATTLER_SIDE(gBattlerAttacker)].reflectTimer = 5;
        gSideTimers[GET_BATTLER_SIDE(gBattlerAttacker)].reflectBattlerId = gBattlerAttacker;

        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && CountAliveMonsInBattle(BATTLE_ALIVE_ATK_SIDE) == 2)
            gBattleCommunication[MULTISTRING_CHOOSER] = 2;
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    ++gBattlescriptCurrInstr;
}

static void atk7F_setseeded(void)
{
    if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT || gStatuses3[gBattlerTarget] & STATUS3_LEECHSEED)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    else if (IS_BATTLER_OF_TYPE(gBattlerTarget, TYPE_GRASS))
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
    }
    else
    {
        gStatuses3[gBattlerTarget] |= gBattlerAttacker;
        gStatuses3[gBattlerTarget] |= STATUS3_LEECHSEED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
    ++gBattlescriptCurrInstr;
}

static void atk80_manipulatedamage(void)
{
    switch (gBattlescriptCurrInstr[1])
    {
    case ATK80_DMG_CHANGE_SIGN:
        gBattleMoveDamage *= -1;
        break;
    case ATK80_DMG_HALF_BY_TWO_NOT_MORE_THAN_HALF_MAX_HP:
        gBattleMoveDamage /= 2;
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        if ((gBattleMons[gBattlerTarget].maxHP / 2) < gBattleMoveDamage)
            gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP / 2;
        break;
    case ATK80_DMG_DOUBLED:
        gBattleMoveDamage *= 2;
        break;
	case ATK80_DMG_1_8_MAX_HP:
	    gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP / 8;
		if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
		break;
    }
    gBattlescriptCurrInstr += 2;
}

static void atk81_trysetrest(void)
{
    const u8 *failJump = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    
    gActiveBattler = gBattlerTarget = gBattlerAttacker;
    gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP * (-1);
	
    if (gBattleMons[gBattlerTarget].hp == gBattleMons[gBattlerTarget].maxHP)
        gBattlescriptCurrInstr = failJump;
    else
    {
        if (gBattleMons[gBattlerTarget].status1 & ((u8)(~STATUS1_SLEEP)))
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        gBattleMons[gBattlerTarget].status1 = 3;
        BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 5;
    }
}

static void atk82_jumpifnotfirstturn(void)
{
    const u8 *failJump = T1_READ_PTR(gBattlescriptCurrInstr + 1);

    if (gDisableStructs[gBattlerAttacker].isFirstTurn)
        gBattlescriptCurrInstr += 5;
    else
        gBattlescriptCurrInstr = failJump;
}

static void atk83_handletrainerslidecase(void)
{
	gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	
	switch (gBattlescriptCurrInstr[2])
	{
		case 0:
		    gBattleMoveDamage = gBattlerSpriteIds[gActiveBattler];
		    HideBattlerShadowSprite(gActiveBattler);
		    break;
		case 1:
		    PrepareStringBattle(STRINGID_SETWORDSTRING, gActiveBattler);
		    break;
		case 2:
		    gBattlerSpriteIds[gActiveBattler] = gBattleMoveDamage;
		    
		    if (IsBattlerAlive(gActiveBattler))
		    {
			    SetBattlerShadowSpriteCallback(gActiveBattler, gBattleMons[gActiveBattler].species);
			    BattleLoadOpponentMonSpriteGfx(&gEnemyParty[gBattlerPartyIndexes[gActiveBattler]], gActiveBattler);
		    }
		    break;
	}
	gBattlescriptCurrInstr += 3;
}

bool8 UproarWakeUpCheck(u8 battlerId)
{
    s32 i;

    for (i = 0; i < gBattlersCount; ++i)
    {
        if (!(gBattleMons[i].status2 & STATUS2_UPROAR) || GetBattlerAbility(battlerId) == ABILITY_SOUNDPROOF)
            continue;
        gBattleScripting.battler = i;

        if (gBattlerTarget == 0xFF)
            gBattlerTarget = i;
        else if (gBattlerTarget == i)
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
        break;
    }
    if (i == gBattlersCount)
        return FALSE;
    else
        return TRUE;
}

static void atk84_trysetpoison(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	
	if (gBattleMons[bank].status1 & STATUS1_PSN_ANY)
	{
		gBattlescriptCurrInstr = BattleScript_AlreadyPoisoned;
		return;
	}
	else if (gBattleMons[bank].status1 & STATUS1_ANY)
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed;
		return;
	}
	else if (IS_BATTLER_OF_TYPE(bank, TYPE_POISON) || IS_BATTLER_OF_TYPE(bank, TYPE_STEEL))
	{
		gBattlescriptCurrInstr = BattleScript_NotAffected;
		return;
	}
	else if ((gSideStatuses[GET_BATTLER_SIDE(bank)] & SIDE_STATUS_SAFEGUARD) && GetBattlerAbility(gBattlerAttacker) != ABILITY_INFILTRATOR)
	{
		gBattlescriptCurrInstr = BattleScript_SafeguardProtected;
		return;
	}
	else
	{
		switch (GetBattlerAbility(bank))
		{
			case ABILITY_IMMUNITY:
				gLastUsedAbility = gBattleMons[bank].ability;
				gBattlescriptCurrInstr = BattleScript_ImmunityProtected;
				return;
			case ABILITY_LEAF_GUARD:
				if (IsBattlerWeatherAffected(bank, WEATHER_SUN_ANY))
				{
					gLastUsedAbility = gBattleMons[bank].ability;
					gBattlescriptCurrInstr = BattleScript_ImmunityProtected;
					return;
				}
				break;
			case ABILITY_FLOWER_VEIL:
			    if (IS_BATTLER_OF_TYPE(bank, TYPE_GRASS))
				{
					gLastUsedAbility = gBattleMons[bank].ability;
					gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
					return;
				}
				break;
		}
		if (IsBattlerAlive(BATTLE_PARTNER(bank)) && GetBattlerAbility(BATTLE_PARTNER(bank)) == ABILITY_FLOWER_VEIL && IS_BATTLER_OF_TYPE(bank, TYPE_GRASS))
		{
			gLastUsedAbility = gBattleMons[BATTLE_PARTNER(bank)].ability;
			gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
			return;
		}
	}
	gBattlescriptCurrInstr += 2;
}

static void atk85_stockpile(void)
{
    if (gDisableStructs[gBattlerAttacker].stockpileCounter == 3)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    else
    {
        ++gDisableStructs[gBattlerAttacker].stockpileCounter;
        PREPARE_BYTE_NUMBER_BUFFER(gBattleTextBuff1, 1, gDisableStructs[gBattlerAttacker].stockpileCounter)
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
    ++gBattlescriptCurrInstr;
}

static void atk86_stockpiletobasedamage(void)
{
    const u8 *jumpPtr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

    if (gDisableStructs[gBattlerAttacker].stockpileCounter == 0)
        gBattlescriptCurrInstr = jumpPtr;
    else
    {
        if (gBattleCommunication[6] != 1)
        {
            gBattleMoveDamage = CalculateBaseDamage(gCurrentMove, gBattleStruct->dynamicMoveType, gBattlerAttacker, gBattlerTarget, FALSE, FALSE, TRUE);
            gBattleScripting.animTurn = gDisableStructs[gBattlerAttacker].stockpileCounter;
        }
        gDisableStructs[gBattlerAttacker].stockpileCounter = 0;
        gBattlescriptCurrInstr += 5;
    }
}

static void atk87_stockpiletohpheal(void)
{
    const u8 *jumpPtr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

    if (gDisableStructs[gBattlerAttacker].stockpileCounter == 0)
    {
        gBattlescriptCurrInstr = jumpPtr;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
    else if (gBattleMons[gBattlerAttacker].maxHP == gBattleMons[gBattlerAttacker].hp)
    {
        gDisableStructs[gBattlerAttacker].stockpileCounter = 0;
        gBattlescriptCurrInstr = jumpPtr;
        gBattlerTarget = gBattlerAttacker;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    else
    {
        gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / (1 << (3 - gDisableStructs[gBattlerAttacker].stockpileCounter));
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        gBattleMoveDamage *= -1;
        gBattleScripting.animTurn = gDisableStructs[gBattlerAttacker].stockpileCounter;
        gDisableStructs[gBattlerAttacker].stockpileCounter = 0;
        gBattlescriptCurrInstr += 5;
        gBattlerTarget = gBattlerAttacker;
    }
}

static void atk88_negativedamage(void)
{
    gBattleMoveDamage = -(gHpDealt / 2);
    if (gBattleMoveDamage == 0)
        gBattleMoveDamage = -1;
    ++gBattlescriptCurrInstr;
}

#define STAT_CHANGE_WORKED      0
#define STAT_CHANGE_DIDNT_WORK  1

static u8 ChangeStatBuffs(s8 statValue, u8 statId, u8 flags, const u8 *BS_ptr)
{
    bool8 certain = FALSE;
    bool8 notProtectAffected = FALSE;
    u32 index;

    if (flags & MOVE_EFFECT_AFFECTS_USER)
        gActiveBattler = gBattlerAttacker;
    else
        gActiveBattler = gBattlerTarget;
	
	gSpecialStatuses[gActiveBattler].changedStatsBattlerId = gBattlerAttacker;
	
    flags &= ~(MOVE_EFFECT_AFFECTS_USER);
    if (flags & MOVE_EFFECT_CERTAIN)
        ++certain;
    flags &= ~(MOVE_EFFECT_CERTAIN);
    if (flags & STAT_CHANGE_NOT_PROTECT_AFFECTED)
        ++notProtectAffected;
    flags &= ~(STAT_CHANGE_NOT_PROTECT_AFFECTED);
    PREPARE_STAT_BUFFER(gBattleTextBuff1, statId)
    if ((statValue >= 0 && GetBattlerAbility(gActiveBattler) == ABILITY_CONTRARY) || (statValue <= -1 && GetBattlerAbility(gActiveBattler) != ABILITY_CONTRARY)) 
    {   
	// Stat decrease.
        if (gSideTimers[GET_BATTLER_SIDE(gActiveBattler)].mistTimer && !certain && gCurrentMove != MOVE_CURSE && GetBattlerAbility(gBattlerAttacker) != ABILITY_INFILTRATOR)
        {
            if (flags == STAT_CHANGE_BS_PTR)
            {
                if (gSpecialStatuses[gActiveBattler].statLowered)
                    gBattlescriptCurrInstr = BS_ptr;
                else
                {
                    BattleScriptPush(BS_ptr);
                    gBattleScripting.battler = gActiveBattler;
                    gBattlescriptCurrInstr = BattleScript_MistProtected;
                    gSpecialStatuses[gActiveBattler].statLowered = 1;
                }
            }
            return STAT_CHANGE_DIDNT_WORK;
        }
        else if (gCurrentMove != MOVE_CURSE && notProtectAffected != TRUE && JumpIfMoveAffectedByProtect(0))
        {
            gBattlescriptCurrInstr = BattleScript_ButItFailed;
            return STAT_CHANGE_DIDNT_WORK;
        }
        else if ((GetBattlerAbility(gActiveBattler) == ABILITY_CLEAR_BODY || GetBattlerAbility(gActiveBattler) == ABILITY_WHITE_SMOKE
		 || (GetBattlerAbility(gActiveBattler) == ABILITY_FLOWER_VEIL && IS_BATTLER_OF_TYPE(gActiveBattler, TYPE_GRASS)))
		 && !certain && gCurrentMove != MOVE_CURSE)
        {
            if (flags == STAT_CHANGE_BS_PTR)
            {
                if (gSpecialStatuses[gActiveBattler].statLowered)
                    gBattlescriptCurrInstr = BS_ptr;
                else
                {
                    BattleScriptPush(BS_ptr);
                    gBattleScripting.battler = gActiveBattler;
                    gBattlescriptCurrInstr = BattleScript_AbilityNoStatLoss;
                    gLastUsedAbility = gBattleMons[gActiveBattler].ability;
                    RecordAbilityBattle(gActiveBattler, gLastUsedAbility);
                    gSpecialStatuses[gActiveBattler].statLowered = 1;
                }
            }
            return STAT_CHANGE_DIDNT_WORK;
        }
		else if (IsBattlerAlive(BATTLE_PARTNER(gActiveBattler)) && GetBattlerAbility(BATTLE_PARTNER(gActiveBattler)) == ABILITY_FLOWER_VEIL
		&& IS_BATTLER_OF_TYPE(gActiveBattler, TYPE_GRASS) && !certain && gCurrentMove != MOVE_CURSE)
		{
			if (flags == STAT_CHANGE_BS_PTR)
            {
                if (gSpecialStatuses[gActiveBattler].statLowered)
                    gBattlescriptCurrInstr = BS_ptr;
                else
                {
                    BattleScriptPush(BS_ptr);
                    gBattleScripting.battler = BATTLE_PARTNER(gActiveBattler);
                    gBattlescriptCurrInstr = BattleScript_AbilityNoStatLoss;
                    gLastUsedAbility = gBattleMons[BATTLE_PARTNER(gActiveBattler)].ability;
                    RecordAbilityBattle(BATTLE_PARTNER(gActiveBattler), gLastUsedAbility);
                    gSpecialStatuses[gActiveBattler].statLowered = 1;
                }
            }
            return STAT_CHANGE_DIDNT_WORK;
		}
        else if (!certain && ((GetBattlerAbility(gActiveBattler) == ABILITY_KEEN_EYE && statId == STAT_ACC)
		|| (GetBattlerAbility(gActiveBattler) == ABILITY_HYPER_CUTTER && statId == STAT_ATK)
		|| (GetBattlerAbility(gActiveBattler) == ABILITY_BIG_PECKS && statId == STAT_DEF)))
        {
            if (flags == STAT_CHANGE_BS_PTR)
            {
                BattleScriptPush(BS_ptr);
                gBattleScripting.battler = gActiveBattler;
                gBattlescriptCurrInstr = BattleScript_AbilityNoSpecificStatLoss;
                gLastUsedAbility = gBattleMons[gActiveBattler].ability;
                RecordAbilityBattle(gActiveBattler, gLastUsedAbility);
            }
            return STAT_CHANGE_DIDNT_WORK;
        }
        else if ((GetBattlerAbility(gActiveBattler) == ABILITY_SHIELD_DUST || ReceiveSheerForceBoost(gBattlerAttacker, gCurrentMove)) && !flags)
            return STAT_CHANGE_DIDNT_WORK;
        else // try to decrease
        {
            statValue = -GET_STAT_BUFF_VALUE(statValue);
		
	    if (GetBattlerAbility(gActiveBattler) == ABILITY_SIMPLE)
		    statValue *= 2;
		
            gBattleTextBuff2[0] = B_BUFF_PLACEHOLDER_BEGIN;
            index = 1;
            if (statValue == -2 || statValue == -4)
            {
                gBattleTextBuff2[1] = B_BUFF_STRING;
                gBattleTextBuff2[2] = STRINGID_STATHARSHLY;
                gBattleTextBuff2[3] = STRINGID_STATHARSHLY >> 8;
                index = 4;
            }
            gBattleTextBuff2[index++] = B_BUFF_STRING;
            gBattleTextBuff2[index++] = STRINGID_STATFELL;
            gBattleTextBuff2[index++] = STRINGID_STATFELL >> 8;
            gBattleTextBuff2[index] = B_BUFF_EOS;
            if (gBattleMons[gActiveBattler].statStages[statId] == 0)
                gBattleCommunication[MULTISTRING_CHOOSER] = 2;
            else
                gBattleCommunication[MULTISTRING_CHOOSER] = (gBattlerTarget == gActiveBattler);
        }
    }
    else // stat increase
    {
        statValue = GET_STAT_BUFF_VALUE(statValue);
	   
	if (GetBattlerAbility(gActiveBattler) == ABILITY_SIMPLE)
		statValue *= 2;
	    
        gBattleTextBuff2[0] = B_BUFF_PLACEHOLDER_BEGIN;
        index = 1;
        if (statValue == 2 || statValue == 4)
        {
            gBattleTextBuff2[1] = B_BUFF_STRING;
            gBattleTextBuff2[2] = STRINGID_STATSHARPLY;
            gBattleTextBuff2[3] = STRINGID_STATSHARPLY >> 8;
            index = 4;
        }
        gBattleTextBuff2[index++] = B_BUFF_STRING;
        gBattleTextBuff2[index++] = STRINGID_STATROSE;
        gBattleTextBuff2[index++] = STRINGID_STATROSE >> 8;
        gBattleTextBuff2[index] = B_BUFF_EOS;
        if (gBattleMons[gActiveBattler].statStages[statId] == 0xC)
            gBattleCommunication[MULTISTRING_CHOOSER] = 2;
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = (gBattlerTarget == gActiveBattler);
    }
    gBattleMons[gActiveBattler].statStages[statId] += statValue;
    if (gBattleMons[gActiveBattler].statStages[statId] < 0)
        gBattleMons[gActiveBattler].statStages[statId] = 0;
    if (gBattleMons[gActiveBattler].statStages[statId] > 0xC)
        gBattleMons[gActiveBattler].statStages[statId] = 0xC;
    if (gBattleCommunication[MULTISTRING_CHOOSER] == 2 && flags & STAT_CHANGE_BS_PTR)
        gMoveResultFlags |= MOVE_RESULT_MISSED;
    if (gBattleCommunication[MULTISTRING_CHOOSER] == 2 && !(flags & STAT_CHANGE_BS_PTR))
        return STAT_CHANGE_DIDNT_WORK;
    return STAT_CHANGE_WORKED;
}

static void atk89_statbuffchange(void)
{
    const u8 *jumpPtr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
    if (ChangeStatBuffs(gBattleScripting.statChanger & 0xF0, GET_STAT_BUFF_ID(gBattleScripting.statChanger), gBattlescriptCurrInstr[1], jumpPtr) == STAT_CHANGE_WORKED)
        gBattlescriptCurrInstr += 6;
}

static void atk8A_normalisebuffs(void) // haze
{
    s32 i, j;

    for (i = 0; i < gBattlersCount; ++i)
        for (j = 0; j < NUM_BATTLE_STATS; ++j)
            gBattleMons[i].statStages[j] = 6;
    ++gBattlescriptCurrInstr;
}

static void atk8B_setbide(void)
{
    gBattleMons[gBattlerAttacker].status2 |= STATUS2_MULTIPLETURNS;
    gBattleMons[gBattlerAttacker].status2 |= (STATUS2_BIDE - 0x100); // 2 turns
    gLockedMoves[gBattlerAttacker] = gCurrentMove;
    gTakenDmg[gBattlerAttacker] = 0;
    ++gBattlescriptCurrInstr;
}

static void atk8C_confuseifrepeatingattackends(void)
{
    if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_LOCK_CONFUSE))
        gBattleCommunication[MOVE_EFFECT_BYTE] = (MOVE_EFFECT_THRASH | MOVE_EFFECT_AFFECTS_USER);
    ++gBattlescriptCurrInstr;
}

static void atk8D_setmultihitcounter(void)
{
    if (gBattlescriptCurrInstr[1])
        gMultiHitCounter = gBattlescriptCurrInstr[1];
    else if (GetBattlerAbility(gBattlerAttacker) == ABILITY_SKILL_LINK)
	gMultiHitCounter = 5;
    else
    {
        gMultiHitCounter = Random() & 3;
	    
        if (gMultiHitCounter > 1)
            gMultiHitCounter = (Random() & 3) + 2;
        else
            gMultiHitCounter += 2;
    }
    gBattlescriptCurrInstr += 2;
}

static void atk8E_initmultihitstring(void)
{
    PREPARE_BYTE_NUMBER_BUFFER(gBattleScripting.multihitString, 1, 0)
    ++gBattlescriptCurrInstr;
}

static bool8 TryDoForceSwitchOut(void)
{
    u16 random;
	
    if (gBattleMons[gBattlerAttacker].level >= gBattleMons[gBattlerTarget].level)
        *(gBattleStruct->battlerPartyIndexes + gBattlerTarget) = gBattlerPartyIndexes[gBattlerTarget];
    else
    {
        random = Random() & 0xFF;
        
        if ((u32)((random * (gBattleMons[gBattlerAttacker].level + gBattleMons[gBattlerTarget].level) >> 8) + 1) <= (gBattleMons[gBattlerTarget].level / 4))
        {
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
            return FALSE;
        }
        *(gBattleStruct->battlerPartyIndexes + gBattlerTarget) = gBattlerPartyIndexes[gBattlerTarget];
    }
    gBattlescriptCurrInstr = BattleScript_SuccessForceOut;
    return TRUE;
}

#define MON_CAN_BATTLE(mon) (((GetMonData(mon, MON_DATA_SPECIES) && GetMonData(mon, MON_DATA_IS_EGG) != TRUE && GetMonData(mon, MON_DATA_HP))))

static void atk8F_forcerandomswitch(void)
{
    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
    {
        u8 i, valid, val;
        struct Pokemon *party;
       
        if (GetBattlerSide(gBattlerTarget) == B_SIDE_PLAYER)
            party = gPlayerParty;
        else
            party = gEnemyParty;
        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
        {
            valid = 0;
            val = 0;
		
            if (GetLinkTrainerFlankId(GetBattlerMultiplayerId(gBattlerTarget)) == 1)
                val = 3;
            for (i = val; i < val + 3; ++i)
            {
                if (MON_CAN_BATTLE(&party[i]))
                    ++valid;
            }
        }
        else
        {
            valid = 0;
		
            for (i = 0; i < PARTY_SIZE; ++i)
            {
                if (MON_CAN_BATTLE(&party[i]))
                    ++valid;
            }
        }

        if ((valid < 2 && (gBattleTypeFlags & (BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI)) != BATTLE_TYPE_DOUBLE)
	    || (valid < 3 && (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && !(gBattleTypeFlags & BATTLE_TYPE_MULTI)))
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        else if (TryDoForceSwitchOut())
        {
            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                do
                {
                    val = Random() % 3;
			
                    if (GetLinkTrainerFlankId(GetBattlerMultiplayerId(gBattlerTarget)) == 1)
                        i = val + 3;
                    else
                        i = val;
                }
                while (i == gBattlerPartyIndexes[gBattlerTarget] || i == gBattlerPartyIndexes[BATTLE_PARTNER(gBattlerTarget)] || !MON_CAN_BATTLE(&party[i]));
            }
            else
            {
                if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                {
                    do
                    {
                        i = Random() % 6;
                    }
                    while (i == gBattlerPartyIndexes[gBattlerTarget] || i == gBattlerPartyIndexes[BATTLE_PARTNER(gBattlerTarget)] || !MON_CAN_BATTLE(&party[i]));
                }
                else
                {
                    do
                    {
                        i = Random() % 6;
                    }
                    while (i == gBattlerPartyIndexes[gBattlerTarget] || !MON_CAN_BATTLE(&party[i]));
                }
            }
            *(gBattleStruct->monToSwitchIntoId + gBattlerTarget) = i;
            if (!IsMultiBattle())
                UpdatePartyOwnerOnSwitch_NonMulti(gBattlerTarget);
            SwitchPartyOrderLinkMulti(gBattlerTarget, i, 0);
            SwitchPartyOrderLinkMulti(gBattlerTarget ^ 2, i, 1);
        }
    }
    else
        TryDoForceSwitchOut();
}

static void atk90_tryconversiontypechange(void) // randomly changes user's type to one of its moves' type
{
    u8 moveChecked, moveType, validMoves = 0;

    while (validMoves < MAX_MON_MOVES)
    {
        if (gBattleMons[gBattlerAttacker].moves[validMoves] == MOVE_NONE)
            break;
        ++validMoves;
    }
    for (moveChecked = 0; moveChecked < validMoves; ++moveChecked)
    {
        moveType = gBattleMoves[gBattleMons[gBattlerAttacker].moves[moveChecked]].type;
	    
        if (moveType == TYPE_MYSTERY)
        {
            if (IS_BATTLER_OF_TYPE(gBattlerAttacker, TYPE_GHOST))
                moveType = TYPE_GHOST;
            else
                moveType = TYPE_NORMAL;
        }
        if (moveType != gBattleMons[gBattlerAttacker].type1 && moveType != gBattleMons[gBattlerAttacker].type2)
            break;
    }
    if (moveChecked == validMoves)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        do
        {
            while ((moveChecked = Random() & 3) >= validMoves);
            moveType = gBattleMoves[gBattleMons[gBattlerAttacker].moves[moveChecked]].type;
		
            if (moveType == TYPE_MYSTERY)
            {
                if (IS_BATTLER_OF_TYPE(gBattlerAttacker, TYPE_GHOST))
                    moveType = TYPE_GHOST;
                else
                    moveType = TYPE_NORMAL;
            }
        }
        while (moveType == gBattleMons[gBattlerAttacker].type1 || moveType == gBattleMons[gBattlerAttacker].type2);
        SET_BATTLER_TYPE(gBattlerAttacker, moveType);
        PREPARE_TYPE_BUFFER(gBattleTextBuff1, moveType);
        gBattlescriptCurrInstr += 5;
    }
}

static void atk91_givepaydaymoney(void)
{
    if (!(gBattleTypeFlags & BATTLE_TYPE_LINK) && gPaydayMoney != 0)
    {
        u32 bonusMoney = gPaydayMoney * gBattleStruct->moneyMultiplier;

        AddMoney(&gSaveBlock1Ptr->money, bonusMoney);
        PREPARE_HWORD_NUMBER_BUFFER(gBattleTextBuff1, 5, bonusMoney)
        BattleScriptPush(gBattlescriptCurrInstr + 1);
        gBattlescriptCurrInstr = BattleScript_PrintPayDayMoneyString;
    }
    else
        ++gBattlescriptCurrInstr;
}

static void atk92_setlightscreen(void)
{
    if (gSideStatuses[GET_BATTLER_SIDE(gBattlerAttacker)] & SIDE_STATUS_LIGHTSCREEN)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
    else
    {
        gSideStatuses[GET_BATTLER_SIDE(gBattlerAttacker)] |= SIDE_STATUS_LIGHTSCREEN;
        gSideTimers[GET_BATTLER_SIDE(gBattlerAttacker)].lightscreenTimer = 5;
        gSideTimers[GET_BATTLER_SIDE(gBattlerAttacker)].lightscreenBattlerId = gBattlerAttacker;
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && CountAliveMonsInBattle(BATTLE_ALIVE_ATK_SIDE) == 2)
            gBattleCommunication[MULTISTRING_CHOOSER] = 4;
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = 3;
    }
    ++gBattlescriptCurrInstr;
}

static void atk93_tryKO(void)
{
    u8 holdEffect = GetBattlerItemHoldEffect(gBattlerTarget, TRUE), param = GetBattlerHoldEffectParam(gBattlerTarget);
    u16 chance;
    
    gPotentialItemEffectBattler = gBattlerTarget;
	
    if (holdEffect == HOLD_EFFECT_FOCUS_BAND && (Random() % 100) < param)
    {
        RecordItemEffectBattle(gBattlerTarget, HOLD_EFFECT_FOCUS_BAND);
        gSpecialStatuses[gBattlerTarget].focusBanded = 1;
    }

    if (GetBattlerAbility(gBattlerTarget) == ABILITY_STURDY)
    {
        gMoveResultFlags |= MOVE_RESULT_FOE_STURDIED;
        gLastUsedAbility = ABILITY_STURDY;
        gBattlescriptCurrInstr = BattleScript_SturdyPreventsOHKO;
        RecordAbilityBattle(gBattlerTarget, ABILITY_STURDY);
    }
    else
    {
        if (!(gStatuses3[gBattlerTarget] & STATUS3_ALWAYS_HITS))
        {
            chance = gBattleMoves[gCurrentMove].accuracy + (gBattleMons[gBattlerAttacker].level - gBattleMons[gBattlerTarget].level);
		
            if (Random() % 100 + 1 < chance && gBattleMons[gBattlerAttacker].level >= gBattleMons[gBattlerTarget].level)
                chance = TRUE;
            else
                chance = FALSE;
        }
        else if (gDisableStructs[gBattlerTarget].battlerWithSureHit == gBattlerAttacker && gBattleMons[gBattlerAttacker].level >= gBattleMons[gBattlerTarget].level)
            chance = TRUE;
        else
        {
            chance = gBattleMoves[gCurrentMove].accuracy + (gBattleMons[gBattlerAttacker].level - gBattleMons[gBattlerTarget].level);
		
            if (Random() % 100 + 1 < chance && gBattleMons[gBattlerAttacker].level >= gBattleMons[gBattlerTarget].level)
                chance = TRUE;
            else
                chance = FALSE;
        }
        if (chance)
        {
            if (gProtectStructs[gBattlerTarget].endured)
            {
                gBattleMoveDamage = gBattleMons[gBattlerTarget].hp - 1;
                gMoveResultFlags |= MOVE_RESULT_FOE_ENDURED;
            }
            else if (gSpecialStatuses[gBattlerTarget].focusBanded)
            {
                gBattleMoveDamage = gBattleMons[gBattlerTarget].hp - 1;
                gMoveResultFlags |= MOVE_RESULT_FOE_HUNG_ON;
                gLastUsedItem = gBattleMons[gBattlerTarget].item;
            }
            else
            {
                gBattleMoveDamage = gBattleMons[gBattlerTarget].hp;
                gMoveResultFlags |= MOVE_RESULT_ONE_HIT_KO;
            }
            gBattlescriptCurrInstr += 5;
        }
        else
        {
            gMoveResultFlags |= MOVE_RESULT_MISSED;
		
            if (gBattleMons[gBattlerAttacker].level >= gBattleMons[gBattlerTarget].level)
                gBattleCommunication[MULTISTRING_CHOOSER] = 0;
            else
                gBattleCommunication[MULTISTRING_CHOOSER] = 1;
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        }
    }
}

static void atk94_damagetohalftargethp(void) // super fang
{
    gBattleMoveDamage = gBattleMons[gBattlerTarget].hp / 2;
    if (gBattleMoveDamage == 0)
        gBattleMoveDamage = 1;
    ++gBattlescriptCurrInstr;
}

static void atk95_setsandstorm(void)
{
    if (!TryChangeBattleWeather(gBattlerAttacker, ENUM_WEATHER_SANDSTORM))
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
    }
    else
        gBattleCommunication[MULTISTRING_CHOOSER] = 3;
	
    ++gBattlescriptCurrInstr;
}

static void atk96_weatherdamage(void)
{
    if (IS_BATTLE_TYPE_GHOST_WITHOUT_SCOPE(gBattleTypeFlags) && (GetBattlerSide(gBattlerAttacker) == B_SIDE_OPPONENT))
    {
        gBattleMoveDamage = 0;
        ++gBattlescriptCurrInstr;
        return;
    }
	if (IsBattlerWeatherAffected(gBattlerAttacker, WEATHER_SANDSTORM_ANY))
	{
		if (!IS_BATTLER_OF_TYPE(gBattlerAttacker, TYPE_ROCK) && !IS_BATTLER_OF_TYPE(gBattlerAttacker, TYPE_STEEL) && !IS_BATTLER_OF_TYPE(gBattlerAttacker, TYPE_GROUND)
			&& GetBattlerAbility(gBattlerAttacker) != ABILITY_SAND_VEIL && GetBattlerAbility(gBattlerAttacker) != ABILITY_MAGIC_GUARD && GetBattlerAbility(gBattlerAttacker) != ABILITY_OVERCOAT
		    && GetBattlerAbility(gBattlerAttacker) != ABILITY_SAND_RUSH && GetBattlerAbility(gBattlerAttacker) != ABILITY_SAND_FORCE
			&& !(gStatuses3[gBattlerAttacker] & (STATUS3_UNDERGROUND | STATUS3_UNDERWATER)))
            {
                gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 16;
                if (gBattleMoveDamage == 0)
                    gBattleMoveDamage = 1;
            }
            else
                gBattleMoveDamage = 0;
	}
	else if (IsBattlerWeatherAffected(gBattlerAttacker, WEATHER_HAIL_ANY))
	{
		if (!IS_BATTLER_OF_TYPE(gBattlerAttacker, TYPE_ICE) && GetBattlerAbility(gBattlerAttacker) != ABILITY_ICE_BODY && GetBattlerAbility(gBattlerAttacker) != ABILITY_MAGIC_GUARD
		    && GetBattlerAbility(gBattlerAttacker) != ABILITY_SNOW_CLOAK && GetBattlerAbility(gBattlerAttacker) != ABILITY_OVERCOAT && !(gStatuses3[gBattlerAttacker] & (STATUS3_UNDERGROUND | STATUS3_UNDERWATER)))
            {
                gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 16;
                if (gBattleMoveDamage == 0)
                    gBattleMoveDamage = 1;
            }
            else
                gBattleMoveDamage = 0;
	}
    else
        gBattleMoveDamage = 0;
	
    if (gAbsentBattlerFlags & gBitTable[gBattlerAttacker])
        gBattleMoveDamage = 0;
    ++gBattlescriptCurrInstr;
}

static void atk97_tryinfatuating(void)
{
    struct Pokemon *monAttacker, *monTarget;
    u16 speciesAttacker, speciesTarget;
    u32 personalityAttacker, personalityTarget;

    monAttacker = GetBattlerPartyIndexPtr(gBattlerAttacker);
	monTarget = GetBattlerPartyIndexPtr(gBattlerTarget);
    speciesAttacker = GetMonData(monAttacker, MON_DATA_SPECIES);
    personalityAttacker = GetMonData(monAttacker, MON_DATA_PERSONALITY);
    speciesTarget = GetMonData(monTarget, MON_DATA_SPECIES);
    personalityTarget = GetMonData(monTarget, MON_DATA_PERSONALITY);
	
    if (GetBattlerAbility(gBattlerTarget) == ABILITY_OBLIVIOUS)
    {
        gBattlescriptCurrInstr = BattleScript_ObliviousPreventsAttraction;
        gLastUsedAbility = ABILITY_OBLIVIOUS;
        RecordAbilityBattle(gBattlerTarget, ABILITY_OBLIVIOUS);
    }
    else
    {
        if (GetGenderFromSpeciesAndPersonality(speciesAttacker, personalityAttacker) == GetGenderFromSpeciesAndPersonality(speciesTarget, personalityTarget)
	    || gBattleMons[gBattlerTarget].status2 & STATUS2_INFATUATION || GetGenderFromSpeciesAndPersonality(speciesAttacker, personalityAttacker) == MON_GENDERLESS
	    || GetGenderFromSpeciesAndPersonality(speciesTarget, personalityTarget) == MON_GENDERLESS)
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        else
        {
            gBattleMons[gBattlerTarget].status2 |= STATUS2_INFATUATED_WITH(gBattlerAttacker);
            gBattlescriptCurrInstr += 5;
        }
    }
}

static void atk98_updatestatusicon(void)
{
    if (!gBattleControllerExecFlags)
    {
        if (gBattlescriptCurrInstr[1] == BS_PLAYER2)
        {
            for (gActiveBattler = gBattleControllerExecFlags; gActiveBattler < gBattlersCount; ++gActiveBattler)
            {
                if (!(gAbsentBattlerFlags & gBitTable[gActiveBattler]))
                {
                    BtlController_EmitStatusIconUpdate(0, gBattleMons[gActiveBattler].status1, gBattleMons[gActiveBattler].status2);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
            }
            gBattlescriptCurrInstr += 2;
        }
        else if (gBattlescriptCurrInstr[1] == BS_ATTACKER_WITH_PARTNER)
        {
            gActiveBattler = gBattlerAttacker;
		
            if (!(gAbsentBattlerFlags & gBitTable[gActiveBattler]))
            {
                BtlController_EmitStatusIconUpdate(0, gBattleMons[gActiveBattler].status1, gBattleMons[gActiveBattler].status2);
                MarkBattlerForControllerExec(gActiveBattler);
            }
            if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
            {
                gActiveBattler = GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(gBattlerAttacker)));
                if (!(gAbsentBattlerFlags & gBitTable[gActiveBattler]))
                {
                    BtlController_EmitStatusIconUpdate(0, gBattleMons[gActiveBattler].status1, gBattleMons[gActiveBattler].status2);
                    MarkBattlerForControllerExec(gActiveBattler);
                }
            }
            gBattlescriptCurrInstr += 2;
        }
        else
        {
            gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
            BtlController_EmitStatusIconUpdate(0, gBattleMons[gActiveBattler].status1, gBattleMons[gActiveBattler].status2);
            MarkBattlerForControllerExec(gActiveBattler);
            gBattlescriptCurrInstr += 2;
        }
    }
}

static void atk99_setmist(void)
{
    if (gSideTimers[GET_BATTLER_SIDE(gBattlerAttacker)].mistTimer)
    {
        gMoveResultFlags |= MOVE_RESULT_FAILED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    else
    {
        gSideTimers[GET_BATTLER_SIDE(gBattlerAttacker)].mistTimer = 5;
        gSideTimers[GET_BATTLER_SIDE(gBattlerAttacker)].mistBattlerId = gBattlerAttacker;
        gSideStatuses[GET_BATTLER_SIDE(gBattlerAttacker)] |= SIDE_STATUS_MIST;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
    ++gBattlescriptCurrInstr;
}

static void atk9A_setfocusenergy(void)
{
    if (gBattleMons[gBattlerAttacker].status2 & STATUS2_FOCUS_ENERGY)
    {
        gMoveResultFlags |= MOVE_RESULT_FAILED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    else
    {
        gBattleMons[gBattlerAttacker].status2 |= STATUS2_FOCUS_ENERGY;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
    ++gBattlescriptCurrInstr;
}

static void atk9B_transformdataexecution(void)
{
    u8 *battleMonAttacker, *battleMonTarget;
    s32 i;
	
    gChosenMove = 0xFFFF;
    ++gBattlescriptCurrInstr;
	
    if (gBattleMons[gBattlerTarget].status2 & STATUS2_TRANSFORMED || gStatuses3[gBattlerTarget] & STATUS3_SEMI_INVULNERABLE || gNewBattleStruct.illusion[gBattlerTarget].on)
    {
        gMoveResultFlags |= MOVE_RESULT_FAILED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    else
    {
        gBattleMons[gBattlerAttacker].status2 |= STATUS2_TRANSFORMED;
        gDisableStructs[gBattlerAttacker].disabledMove = MOVE_NONE;
        gDisableStructs[gBattlerAttacker].disableTimer = 0;
        gDisableStructs[gBattlerAttacker].transformedMonPersonality = gBattleMons[gBattlerTarget].personality;
        gDisableStructs[gBattlerAttacker].mimickedMoves = 0;
        PREPARE_SPECIES_BUFFER(gBattleTextBuff1, gBattleMons[gBattlerTarget].species)
        battleMonAttacker = (u8 *)(&gBattleMons[gBattlerAttacker]);
        battleMonTarget = (u8 *)(&gBattleMons[gBattlerTarget]);
        for (i = 0; i < offsetof(struct BattlePokemon, pp); ++i)
            battleMonAttacker[i] = battleMonTarget[i];
        for (i = 0; i < MAX_MON_MOVES; ++i)
        {
            if (gBattleMoves[gBattleMons[gBattlerAttacker].moves[i]].pp < 5)
                gBattleMons[gBattlerAttacker].pp[i] = gBattleMoves[gBattleMons[gBattlerAttacker].moves[i]].pp;
            else
                gBattleMons[gBattlerAttacker].pp[i] = 5;
        }
        gActiveBattler = gBattlerAttacker;
        BtlController_EmitResetActionMoveSelection(0, RESET_MOVE_SELECTION);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
}

static void atk9C_setsubstitute(void)
{
    u32 hp = gBattleMons[gBattlerAttacker].maxHP / 4;

    if (gBattleMons[gBattlerAttacker].maxHP / 4 == 0)
        hp = 1;
    if (gBattleMons[gBattlerAttacker].hp <= hp)
    {
        gBattleMoveDamage = 0;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    else
    {
        gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 4; // one bit value will only work for pokemon which max hp can go to 1020(which is more than possible in games)
        
	if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        gBattleMons[gBattlerAttacker].status2 |= STATUS2_SUBSTITUTE;
        gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_WRAPPED);
        gDisableStructs[gBattlerAttacker].substituteHP = gBattleMoveDamage;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        gHitMarker |= HITMARKER_IGNORE_SUBSTITUTE;
    }
    ++gBattlescriptCurrInstr;
}

static void atk9D_mimicattackcopy(void)
{ 
    s32 i;
	
    gChosenMove = 0xFFFF;
	
    if (sForbiddenMoves[gLastMoves[gBattlerTarget]] & FORBIDDEN_MIMIC || gBattleMons[gBattlerAttacker].status2 & STATUS2_TRANSFORMED
	|| gLastMoves[gBattlerTarget] == 0 || gLastMoves[gBattlerTarget] == 0xFFFF)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        for (i = 0; i < MAX_MON_MOVES; ++i)
        {
            if (gBattleMons[gBattlerAttacker].moves[i] == gLastMoves[gBattlerTarget])
                break;
        }
        if (i == MAX_MON_MOVES)
        {
            gBattleMons[gBattlerAttacker].moves[gCurrMovePos] = gLastMoves[gBattlerTarget];
            if (gBattleMoves[gLastMoves[gBattlerTarget]].pp < 5)
                gBattleMons[gBattlerAttacker].pp[gCurrMovePos] = gBattleMoves[gLastMoves[gBattlerTarget]].pp;
            else
                gBattleMons[gBattlerAttacker].pp[gCurrMovePos] = 5;
            PREPARE_MOVE_BUFFER(gBattleTextBuff1, gLastMoves[gBattlerTarget])
            gDisableStructs[gBattlerAttacker].mimickedMoves |= gBitTable[gCurrMovePos];
            gBattlescriptCurrInstr += 5;
        }
        else
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

static void atk9E_metronome(void)
{
    while (TRUE)
    {
        gCurrentMove = (Random() % (MOVES_COUNT - 1)) + 1;
	    
        if (!(sForbiddenMoves[gCurrentMove] & FORBIDDEN_METRONOME))
        {
            gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
            gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
            gBattlerTarget = GetMoveTarget(gCurrentMove, 0);
            return;
        }
    }
}

static void atk9F_dmgtolevel(void)
{
    gBattleMoveDamage = gBattleMons[gBattlerAttacker].level;
    ++gBattlescriptCurrInstr;
}

static void atkA0_psywavedamageeffect(void)
{
    s32 randDamage;

    while ((randDamage = (Random() & 0xF)) > 10);
    randDamage *= 10;
    gBattleMoveDamage = gBattleMons[gBattlerAttacker].level * (randDamage + 50) / 100;
    ++gBattlescriptCurrInstr;
}

static void atkA1_counterdamagecalculator(void)
{
    u8 sideAttacker = GetBattlerSide(gBattlerAttacker);
    u8 sideTarget = GetBattlerSide(gProtectStructs[gBattlerAttacker].physicalBattlerId);

    if (gProtectStructs[gBattlerAttacker].physicalDmg && sideAttacker != sideTarget
	&& IsBattlerAlive(gProtectStructs[gBattlerAttacker].physicalBattlerId))
    {
        gBattleMoveDamage = gProtectStructs[gBattlerAttacker].physicalDmg * 2;

        if (gSideTimers[sideTarget].followmeTimer && IsBattlerAlive(gSideTimers[sideTarget].followmeTarget))
            gBattlerTarget = gSideTimers[sideTarget].followmeTarget;
        else
            gBattlerTarget = gProtectStructs[gBattlerAttacker].physicalBattlerId;

        gBattlescriptCurrInstr += 5;
    }
    else
    {
        gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

// a copy of atkA1 with the physical -> special field changes
static void atkA2_mirrorcoatdamagecalculator(void)
{
    u8 sideAttacker = GetBattlerSide(gBattlerAttacker);
    u8 sideTarget = GetBattlerSide(gProtectStructs[gBattlerAttacker].specialBattlerId);

    if (gProtectStructs[gBattlerAttacker].specialDmg && sideAttacker != sideTarget && IsBattlerAlive(gProtectStructs[gBattlerAttacker].specialBattlerId))
    {
        gBattleMoveDamage = gProtectStructs[gBattlerAttacker].specialDmg * 2;
	    
        if (gSideTimers[sideTarget].followmeTimer && IsBattlerAlive(gSideTimers[sideTarget].followmeTarget))
            gBattlerTarget = gSideTimers[sideTarget].followmeTarget;
        else
            gBattlerTarget = gProtectStructs[gBattlerAttacker].specialBattlerId;
        gBattlescriptCurrInstr += 5;
    }
    else
    {
        gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

static void atkA3_disablelastusedattack(void)
{
    s32 i;
    u8 battler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

    for (i = 0; i < MAX_MON_MOVES; ++i)
    {
        if (gBattleMons[battler].moves[i] == gLastMoves[battler])
            break;
    }
    if (gDisableStructs[battler].disabledMove == MOVE_NONE && i != MAX_MON_MOVES && gBattleMons[battler].pp[i] != 0)
    {
        PREPARE_MOVE_BUFFER(gBattleTextBuff1, gBattleMons[battler].moves[i])
        gDisableStructs[battler].disabledMove = gBattleMons[battler].moves[i];
        gDisableStructs[battler].disableTimer = 4;
        gBattlescriptCurrInstr += 6;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
}

static void atkA4_trysetencore(void)
{
    s32 i;

    for (i = 0; i < MAX_MON_MOVES; ++i)
        if (gBattleMons[gBattlerTarget].moves[i] == gLastMoves[gBattlerTarget])
            break;
    if (gLastMoves[gBattlerTarget] == MOVE_STRUGGLE || gLastMoves[gBattlerTarget] == MOVE_ENCORE || gLastMoves[gBattlerTarget] == MOVE_MIRROR_MOVE)
        i = 4;
    if (gDisableStructs[gBattlerTarget].encoredMove == MOVE_NONE && i != 4 && gBattleMons[gBattlerTarget].pp[i] != 0)
    {
        gDisableStructs[gBattlerTarget].encoredMove = gBattleMons[gBattlerTarget].moves[i];
        gDisableStructs[gBattlerTarget].encoredMovePos = i;
        gDisableStructs[gBattlerTarget].encoreTimer = 3;
        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkA5_painsplitdmgcalc(void)
{
    if (!SubsBlockMove(gBattlerAttacker, gBattlerTarget, gCurrentMove))
    {
        s32 hpDiff = (gBattleMons[gBattlerAttacker].hp + gBattleMons[gBattlerTarget].hp) / 2;
        s32 painSplitHp = gBattleMoveDamage = gBattleMons[gBattlerTarget].hp - hpDiff;
        u8 *storeLoc = (void *)(&gBattleScripting.painSplitHp);

        storeLoc[0] = (painSplitHp);
        storeLoc[1] = (painSplitHp & 0x0000FF00) >> 8;
        storeLoc[2] = (painSplitHp & 0x00FF0000) >> 16;
        storeLoc[3] = (painSplitHp & 0xFF000000) >> 24;
        gBattleMoveDamage = gBattleMons[gBattlerAttacker].hp - hpDiff;
        gSpecialStatuses[gBattlerTarget].dmg = 0xFFFF;
        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkA6_settypetorandomresistance(void) // conversion 2
{
    u32 i, resistTypes, hitByType;
	
    if (gLastLandedMoves[gBattlerAttacker] == MOVE_NONE || gLastLandedMoves[gBattlerAttacker] == 0xFFFF)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else if (IsTwoTurnsMove(gLastLandedMoves[gBattlerAttacker]) && gBattleMons[gLastHitBy[gBattlerAttacker]].status2 & STATUS2_MULTIPLETURNS)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
		hitByType = gLastHitByType[gBattlerAttacker];
		
		for (i = 0, resistTypes = 0; i < NUMBER_OF_MON_TYPES; i++)
		{
			switch (GetTypeModifier(hitByType, i))
			{
				case TYPE_MUL_NO_EFFECT:
				case TYPE_MUL_NOT_EFFECTIVE:
				    resistTypes |= gBitTable[i];
				    break;
			}
		}
		
		while (resistTypes != 0)
		{
			i = Random() % NUMBER_OF_MON_TYPES;
			
			if (resistTypes & gBitTable[i])
			{
				if (IS_BATTLER_OF_TYPE(gBattlerAttacker, i))
					resistTypes &= ~(gBitTable[i]);
				else
				{
					SET_BATTLER_TYPE(gBattlerAttacker, i);
					PREPARE_TYPE_BUFFER(gBattleTextBuff1, i);
					gBattlescriptCurrInstr += 5;
					return;
				}
			}
		}
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

static void atkA7_setalwayshitflag(void)
{
    gStatuses3[gBattlerTarget] &= ~(STATUS3_ALWAYS_HITS);
    gStatuses3[gBattlerTarget] |= 0x10;
    gDisableStructs[gBattlerTarget].battlerWithSureHit = gBattlerAttacker;
    ++gBattlescriptCurrInstr;
}

static void atkA8_copymovepermanently(void) // sketch
{
    s32 i;
	
    gChosenMove = 0xFFFF;
	
    if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_TRANSFORMED) && gLastPrintedMoves[gBattlerTarget] != MOVE_STRUGGLE
	&& gLastPrintedMoves[gBattlerTarget] != 0 && gLastPrintedMoves[gBattlerTarget] != 0xFFFF
	&& gLastPrintedMoves[gBattlerTarget] != MOVE_SKETCH)
    {
        for (i = 0; i < MAX_MON_MOVES; ++i)
            if (gBattleMons[gBattlerAttacker].moves[i] != MOVE_SKETCH && gBattleMons[gBattlerAttacker].moves[i] == gLastPrintedMoves[gBattlerTarget])
                break;
        if (i != MAX_MON_MOVES)
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        else // sketch worked
        {
            struct MovePpInfo movePpData;

            gBattleMons[gBattlerAttacker].moves[gCurrMovePos] = gLastPrintedMoves[gBattlerTarget];
            gBattleMons[gBattlerAttacker].pp[gCurrMovePos] = gBattleMoves[gLastPrintedMoves[gBattlerTarget]].pp;
            gActiveBattler = gBattlerAttacker;
		
            for (i = 0; i < MAX_MON_MOVES; ++i)
            {
                movePpData.moves[i] = gBattleMons[gBattlerAttacker].moves[i];
                movePpData.pp[i] = gBattleMons[gBattlerAttacker].pp[i];
            }
            movePpData.ppBonuses = gBattleMons[gBattlerAttacker].ppBonuses;
            BtlController_EmitSetMonData(0, REQUEST_MOVES_PP_BATTLE, 0, sizeof(struct MovePpInfo), &movePpData);
            MarkBattlerForControllerExec(gActiveBattler);
            PREPARE_MOVE_BUFFER(gBattleTextBuff1, gLastPrintedMoves[gBattlerTarget])
            gBattlescriptCurrInstr += 5;
        }
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static bool8 IsTwoTurnsMove(u16 move)
{
    if (gBattleMoves[move].effect == EFFECT_SKULL_BASH
	|| gBattleMoves[move].effect == EFFECT_RAZOR_WIND
	|| gBattleMoves[move].effect == EFFECT_SKY_ATTACK
	|| gBattleMoves[move].effect == EFFECT_SOLARBEAM
	|| gBattleMoves[move].effect == EFFECT_SEMI_INVULNERABLE
	|| gBattleMoves[move].effect == EFFECT_BIDE)
        return TRUE;
    else
        return FALSE;
}

static u8 AttacksThisTurn(u8 battlerId, u16 move) // Note: returns 1 if it's a charging turn, otherwise 2
{
    if (gBattleMoves[move].effect == EFFECT_SOLARBEAM && IsBattlerWeatherAffected(battlerId, WEATHER_SUN_ANY))
        return 2;
    if (gBattleMoves[move].effect == EFFECT_SKULL_BASH
	|| gBattleMoves[move].effect == EFFECT_RAZOR_WIND
	|| gBattleMoves[move].effect == EFFECT_SKY_ATTACK
	|| gBattleMoves[move].effect == EFFECT_SOLARBEAM
	|| gBattleMoves[move].effect == EFFECT_SEMI_INVULNERABLE
	|| gBattleMoves[move].effect == EFFECT_BIDE)
        if ((gHitMarker & HITMARKER_CHARGING))
            return 1;
    return 2;
}

static void atkA9_trychoosesleeptalkmove(void)
{
    u8 unusableMovesBits = 0;
    u32 movePosition;
    s32 i;

    for (i = 0; i < MAX_MON_MOVES; ++i)
    {
        if (sForbiddenMoves[gBattleMons[gBattlerAttacker].moves[i]] & FORBIDDEN_SLEEP_TALK || IsTwoTurnsMove(gBattleMons[gBattlerAttacker].moves[i]))
            unusableMovesBits |= gBitTable[i];
    }
    unusableMovesBits = CheckMoveLimitations(gBattlerAttacker, unusableMovesBits, ~(MOVE_LIMITATION_PP));
	
    if (unusableMovesBits == 0xF) // all 4 moves cannot be chosen
        gBattlescriptCurrInstr += 5;
    else // at least one move can be chosen
    {
        do
            movePosition = Random() & 3;
        while ((gBitTable[movePosition] & unusableMovesBits));
	gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
	gCurrMovePos = movePosition;
        gCalledMove = gBattleMons[gBattlerAttacker].moves[movePosition];
        gBattlerTarget = GetMoveTarget(gCalledMove, 0);
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

static void atkAA_setdestinybond(void)
{
    gBattleMons[gBattlerAttacker].status2 |= STATUS2_DESTINY_BOND;
    ++gBattlescriptCurrInstr;
}

static void TrySetDestinyBondToHappen(void)
{
    u8 sideAttacker = GetBattlerSide(gBattlerAttacker);
    u8 sideTarget = GetBattlerSide(gBattlerTarget);

    if (gBattleMons[gBattlerTarget].status2 & STATUS2_DESTINY_BOND && sideAttacker != sideTarget && !(gHitMarker & HITMARKER_GRUDGE))
        gHitMarker |= HITMARKER_DESTINYBOND;
}

static void atkAB_trysetdestinybondtohappen(void)
{
    TrySetDestinyBondToHappen();
    ++gBattlescriptCurrInstr;
}

static void atkAC_trysetburn(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	
	if (gBattleMons[bank].status1 & STATUS1_BURN)
	{
		gBattlescriptCurrInstr = BattleScript_AlreadyBurned;
		return;
	}
	else if (gBattleMons[bank].status1 & STATUS1_ANY)
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed;
		return;
	}
	else if (IS_BATTLER_OF_TYPE(bank, TYPE_FIRE))
	{
		gBattlescriptCurrInstr = BattleScript_NotAffected;
		return;
	}
	else if ((gSideStatuses[GET_BATTLER_SIDE(bank)] & SIDE_STATUS_SAFEGUARD) && GetBattlerAbility(gBattlerAttacker) != ABILITY_INFILTRATOR)
	{
		gBattlescriptCurrInstr = BattleScript_SafeguardProtected;
		return;
	}
	else
	{
		switch (GetBattlerAbility(bank))
		{
			case ABILITY_WATER_VEIL:
				gLastUsedAbility = gBattleMons[bank].ability;
				gBattlescriptCurrInstr = BattleScript_WaterVeilPrevents;
				return;
			case ABILITY_LEAF_GUARD:
				if (IsBattlerWeatherAffected(bank, WEATHER_SUN_ANY))
				{
					gLastUsedAbility = gBattleMons[bank].ability;
					gBattlescriptCurrInstr = BattleScript_WaterVeilPrevents;
					return;
				}
				break;
			case ABILITY_FLOWER_VEIL:
			    if (IS_BATTLER_OF_TYPE(bank, TYPE_GRASS))
				{
					gLastUsedAbility = gBattleMons[bank].ability;
					gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
					return;
				}
				break;
		}
		if (IsBattlerAlive(BATTLE_PARTNER(bank)) && GetBattlerAbility(BATTLE_PARTNER(bank)) == ABILITY_FLOWER_VEIL && IS_BATTLER_OF_TYPE(bank, TYPE_GRASS))
		{
			gLastUsedAbility = gBattleMons[BATTLE_PARTNER(bank)].ability;
			gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
			return;
		}
	}
	gBattlescriptCurrInstr += 2;
}

static void atkAD_tryspiteppreduce(void)
{
    s32 i, ppToDeduct;
	
    if (gLastMoves[gBattlerTarget] != MOVE_NONE && gLastMoves[gBattlerTarget] != 0xFFFF)
    {
        for (i = 0; i < MAX_MON_MOVES; ++i)
            if (gLastMoves[gBattlerTarget] == gBattleMons[gBattlerTarget].moves[i])
                break;
        if (i != MAX_MON_MOVES && gBattleMons[gBattlerTarget].pp[i] > 1)
        {
            ppToDeduct = (Random() & 3) + 2;

            if (gBattleMons[gBattlerTarget].pp[i] < ppToDeduct)
                ppToDeduct = gBattleMons[gBattlerTarget].pp[i];
            PREPARE_MOVE_BUFFER(gBattleTextBuff1, gLastMoves[gBattlerTarget])
            ConvertIntToDecimalStringN(gBattleTextBuff2, ppToDeduct, 0, 1);
            PREPARE_BYTE_NUMBER_BUFFER(gBattleTextBuff2, 1, ppToDeduct)
            gBattleMons[gBattlerTarget].pp[i] -= ppToDeduct;
            gActiveBattler = gBattlerTarget;
            if (!(gDisableStructs[gActiveBattler].mimickedMoves & gBitTable[i]) && !(gBattleMons[gActiveBattler].status2 & STATUS2_TRANSFORMED))
            {
                BtlController_EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + i, 0, 1, &gBattleMons[gActiveBattler].pp[i]);
                MarkBattlerForControllerExec(gActiveBattler);
            }
            gBattlescriptCurrInstr += 5;
            if (gBattleMons[gBattlerTarget].pp[i] == 0)
                CancelMultiTurnMoves(gBattlerTarget);
        }
        else
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkAE_healpartystatus(void)
{
    u8 abilityNum, toHeal = 0;
    u16 species, ability;
    u32 zero = 0;
    s32 i;
	
    if (gCurrentMove == MOVE_HEAL_BELL)
    {
        struct Pokemon *party;

        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	    
        if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
            party = gPlayerParty;
        else
            party = gEnemyParty;
        if (GetBattlerAbility(gBattlerAttacker) != ABILITY_SOUNDPROOF)
        {
            gBattleMons[gBattlerAttacker].status1 = 0;
            gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_NIGHTMARE);
        }
        else
        {
            RecordAbilityBattle(gBattlerAttacker, gBattleMons[gBattlerAttacker].ability);
            gBattleCommunication[MULTISTRING_CHOOSER] |= 1;
        }
        gActiveBattler = gBattleScripting.battler = GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(gBattlerAttacker)));
	    
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !(gAbsentBattlerFlags & gBitTable[gActiveBattler]))
        {
            if (GetBattlerAbility(gActiveBattler) != ABILITY_SOUNDPROOF)
            {
                gBattleMons[gActiveBattler].status1 = 0;
                gBattleMons[gActiveBattler].status2 &= ~(STATUS2_NIGHTMARE);
            }
            else
            {
                RecordAbilityBattle(gActiveBattler, gBattleMons[gActiveBattler].ability);
                gBattleCommunication[MULTISTRING_CHOOSER] |= 2;
            }
        }
        for (i = 0; i < PARTY_SIZE; ++i)
        {
            species = GetMonData(&party[i], MON_DATA_SPECIES2);
            abilityNum = GetMonData(&party[i], MON_DATA_ABILITY_NUM);

            if (species != SPECIES_NONE && species != SPECIES_EGG)
            {
                if (gBattlerPartyIndexes[gBattlerAttacker] == i)
                    ability = GetBattlerAbility(gBattlerAttacker);
                else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && gBattlerPartyIndexes[gActiveBattler] == i && !(gAbsentBattlerFlags & gBitTable[gActiveBattler]))
                    ability = GetBattlerAbility(gActiveBattler);
                else
                    ability = GetAbilityBySpecies(species, abilityNum, GetMonData(&party[i], MON_DATA_ABILITY_HIDDEN));
                if (ability != ABILITY_SOUNDPROOF)
                    toHeal |= (1 << i);
            }
        }
    }
    else // Aromatherapy
    {
        gBattleCommunication[MULTISTRING_CHOOSER] = 4;
        toHeal = 0x3F;
        gBattleMons[gBattlerAttacker].status1 = 0;
        gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_NIGHTMARE);
        gActiveBattler = GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(gBattlerAttacker)));
        if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !(gAbsentBattlerFlags & gBitTable[gActiveBattler]))
        {
            gBattleMons[gActiveBattler].status1 = 0;
            gBattleMons[gActiveBattler].status2 &= ~(STATUS2_NIGHTMARE);
        }

    }
    if (toHeal)
    {
        gActiveBattler = gBattlerAttacker;
        BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, toHeal, 4, &zero);
        MarkBattlerForControllerExec(gActiveBattler);
    }
    ++gBattlescriptCurrInstr;
}

static void atkAF_cursetarget(void)
{
    if (gBattleMons[gBattlerTarget].status2 & STATUS2_CURSED)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        gBattleMons[gBattlerTarget].status2 |= STATUS2_CURSED;
        gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 2;
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkB0_trysetspikes(void)
{
    u8 targetSide = BATTLE_OPPOSITE(GetBattlerSide(gBattlerAttacker));

    if (gSideTimers[targetSide].spikesAmount == 3)
    {
        gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else
    {
        gSideStatuses[targetSide] |= SIDE_STATUS_SPIKES;
        ++gSideTimers[targetSide].spikesAmount;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkB1_setforesight(void)
{
    gBattleMons[gBattlerTarget].status2 |= STATUS2_FORESIGHT;
    ++gBattlescriptCurrInstr;
}

static void atkB2_trysetperishsong(void)
{
    s32 i, notAffectedCount = 0;

    for (i = 0; i < gBattlersCount; ++i)
    {
        if (gStatuses3[i] & STATUS3_PERISH_SONG || GetBattlerAbility(i) == ABILITY_SOUNDPROOF)
            ++notAffectedCount;
        else
        {
            gStatuses3[i] |= STATUS3_PERISH_SONG;
            gDisableStructs[i].perishSongTimer = 3;
        }
    }
    PressurePPLoseOnUsingPerishSong(gBattlerAttacker);
	
    if (notAffectedCount == gBattlersCount)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
        gBattlescriptCurrInstr += 5;
}

static void atkB3_handlerollout(void)
{
    if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
    {
        CancelMultiTurnMoves(gBattlerAttacker);
        gBattlescriptCurrInstr = BattleScript_MoveMissedPause;
    }
    else
    {
        if (!(gBattleMons[gBattlerAttacker].status2 & STATUS2_MULTIPLETURNS)) // first hit
        {
            gDisableStructs[gBattlerAttacker].rolloutTimer = 5;
            gDisableStructs[gBattlerAttacker].rolloutTimerStartValue = 5;
            gBattleMons[gBattlerAttacker].status2 |= STATUS2_MULTIPLETURNS;
            gLockedMoves[gBattlerAttacker] = gCurrentMove;
        }
        if (--gDisableStructs[gBattlerAttacker].rolloutTimer == 0) // last hit
            gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_MULTIPLETURNS);
        ++gBattlescriptCurrInstr;
    }
}

static void atkB4_jumpifconfusedandstatmaxed(void)
{
    if (gBattleMons[gBattlerTarget].status2 & STATUS2_CONFUSION && !BattlerStatCanRaise(gBattlerTarget, gBattlescriptCurrInstr[1]))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
    else
        gBattlescriptCurrInstr += 6;
}

static void atkB5_handlefurycutter(void)
{
    s32 i;
	
    if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
    {
        gDisableStructs[gBattlerAttacker].furyCutterCounter = 0;
        gBattlescriptCurrInstr = BattleScript_MoveMissedPause;
    }
    else
    {
        if (gDisableStructs[gBattlerAttacker].furyCutterCounter != 5)
            ++gDisableStructs[gBattlerAttacker].furyCutterCounter;
        ++gBattlescriptCurrInstr;
    }
}

static void atkB6_trysetparalyze(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	
	if (gBattleMons[bank].status1 & STATUS1_PARALYSIS)
	{
		gBattlescriptCurrInstr = BattleScript_AlreadyParalyzed;
		return;
	}
	else if (gBattleMons[bank].status1 & STATUS1_ANY)
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed;
		return;
	}
	else if (IS_BATTLER_OF_TYPE(bank, TYPE_ELECTRIC))
	{
		gBattlescriptCurrInstr = BattleScript_NotAffected;
		return;
	}
	else if ((gSideStatuses[GET_BATTLER_SIDE(bank)] & SIDE_STATUS_SAFEGUARD) && GetBattlerAbility(gBattlerAttacker) != ABILITY_INFILTRATOR)
	{
		gBattlescriptCurrInstr = BattleScript_SafeguardProtected;
		return;
	}
	else
	{
		switch (GetBattlerAbility(bank))
		{
			case ABILITY_LIMBER:
				gLastUsedAbility = gBattleMons[bank].ability;
				gBattlescriptCurrInstr = BattleScript_LimberProtected;
				return;
			case ABILITY_LEAF_GUARD:
				if (IsBattlerWeatherAffected(bank, WEATHER_SUN_ANY))
				{
					gLastUsedAbility = gBattleMons[bank].ability;
					gBattlescriptCurrInstr = BattleScript_LimberProtected;
					return;
				}
				break;
			case ABILITY_FLOWER_VEIL:
			    if (IS_BATTLER_OF_TYPE(bank, TYPE_GRASS))
				{
					gLastUsedAbility = gBattleMons[bank].ability;
					gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
					return;
				}
				break;
		}
		if (IsBattlerAlive(BATTLE_PARTNER(bank)) && GetBattlerAbility(BATTLE_PARTNER(bank)) == ABILITY_FLOWER_VEIL && IS_BATTLER_OF_TYPE(bank, TYPE_GRASS))
		{
			gLastUsedAbility = gBattleMons[BATTLE_PARTNER(bank)].ability;
			gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
			return;
		}
	}
	gBattlescriptCurrInstr += 2;
}

static void atkB7_presentcalc(void)
{
    s32 rand = Random() & 0xFF;

    if (rand >= 204)
    {
	    gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP / 4;
	    if (gBattleMoveDamage == 0)
		    gBattleMoveDamage = 1;
	    gBattleMoveDamage *= -1;
	    
	    if (gBattleMons[gBattlerTarget].maxHP == gBattleMons[gBattlerTarget].hp)
		    gBattlescriptCurrInstr = BattleScript_AlreadyAtFullHp;
	    else
		    gBattlescriptCurrInstr = BattleScript_PresentHealTarget;
    }
    else
	    gBattlescriptCurrInstr = BattleScript_HitFromCritCalc;
}

static void atkB8_setsafeguard(void)
{
    if (gSideStatuses[GET_BATTLER_SIDE(gBattlerAttacker)] & SIDE_STATUS_SAFEGUARD)
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
    else
    {
        gSideStatuses[GET_BATTLER_SIDE(gBattlerAttacker)] |= SIDE_STATUS_SAFEGUARD;
        gSideTimers[GET_BATTLER_SIDE(gBattlerAttacker)].safeguardTimer = 5;
        gSideTimers[GET_BATTLER_SIDE(gBattlerAttacker)].safeguardBattlerId = gBattlerAttacker;
        gBattleCommunication[MULTISTRING_CHOOSER] = 5;
    }
    ++gBattlescriptCurrInstr;
}

static void atkB9_magnitudedamagecalculation(void)
{
    s32 magnitude = Random() % 100;
    u8 power;
	
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
    PREPARE_BYTE_NUMBER_BUFFER(gBattleTextBuff1, 2, magnitude)
	    
    for (gBattlerTarget = 0; gBattlerTarget < gBattlersCount; ++gBattlerTarget)
        if (gBattlerTarget != gBattlerAttacker && !(gAbsentBattlerFlags & gBitTable[gBattlerTarget])) // a valid target was found
            break;
    ++gBattlescriptCurrInstr;
}

static void atkBA_jumpifnopursuitswitchdmg(void)
{
    s32 i;
	
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
    if (gChosenActionByBattler[gBattlerTarget] == B_ACTION_USE_MOVE && gBattlerAttacker == *(gBattleStruct->moveTarget + gBattlerTarget)
	&& !(gBattleMons[gBattlerTarget].status1 & (STATUS1_SLEEP | STATUS1_FREEZE)) && gBattleMons[gBattlerAttacker].hp
	&& !gDisableStructs[gBattlerTarget].truantCounter && gChosenMoveByBattler[gBattlerTarget] == MOVE_PURSUIT)
    {
        for (i = 0; i < gBattlersCount; ++i)
            if (gBattlerByTurnOrder[i] == gBattlerTarget)
                gActionsByTurnOrder[i] = 11;
        gCurrentMove = MOVE_PURSUIT;
        gCurrMovePos = gChosenMovePos = *(gBattleStruct->chosenMovePositions + gBattlerTarget);
        gBattlescriptCurrInstr += 5;
        gBattleScripting.animTurn = 1;
        gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkBB_setsunny(void)
{
    if (!TryChangeBattleWeather(gBattlerAttacker, ENUM_WEATHER_SUN))
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
    }
    else
        gBattleCommunication[MULTISTRING_CHOOSER] = 4;
	
    ++gBattlescriptCurrInstr;
}

static void atkBC_maxattackhalvehp(void) // belly drum
{
    u32 halfHp = gBattleMons[gBattlerAttacker].maxHP / 2;

    if (halfHp == 0)
        halfHp = 1;
    if (BattlerStatCanRaise(gBattlerAttacker, STAT_ATK) && gBattleMons[gBattlerAttacker].hp > halfHp)
    {
	if (GetBattlerAbility(gBattlerAttacker) != ABILITY_CONTRARY)
		gBattleMons[gBattlerAttacker].statStages[STAT_ATK] = 12;
	else
	{
		gBattleMons[gBattlerAttacker].statStages[STAT_ATK] = 0;
		gSetWordLoc = sBellyDrumContraryString;
	}
        gBattleMoveDamage = halfHp;
        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkBD_copyfoestats(void) // psych up
{
    s32 i;

    for (i = 0; i < NUM_BATTLE_STATS; ++i)
        gBattleMons[gBattlerAttacker].statStages[i] = gBattleMons[gBattlerTarget].statStages[i];
    gBattlescriptCurrInstr += 5; // Has an unused jump ptr(possibly for a failed attempt) parameter.
}

static void atkBE_rapidspinfree(void)
{
    if (gBattleMons[gBattlerAttacker].status2 & STATUS2_WRAPPED)
    {
        gBattleScripting.battler = gBattlerTarget;
        gBattleMons[gBattlerAttacker].status2 &= ~(STATUS2_WRAPPED);
        gBattlerTarget = *(gBattleStruct->wrappedBy + gBattlerAttacker);
        gBattleTextBuff1[0] = B_BUFF_PLACEHOLDER_BEGIN;
        gBattleTextBuff1[1] = B_BUFF_MOVE;
        gBattleTextBuff1[2] = *(gBattleStruct->wrappedMove + gBattlerAttacker * 2 + 0);
        gBattleTextBuff1[3] = *(gBattleStruct->wrappedMove + gBattlerAttacker * 2 + 1);
        gBattleTextBuff1[4] = B_BUFF_EOS;
        BattleScriptPushCursor();
        gBattlescriptCurrInstr = BattleScript_WrapFree;
    }
    else if (gStatuses3[gBattlerAttacker] & STATUS3_LEECHSEED)
    {
        gStatuses3[gBattlerAttacker] &= ~(STATUS3_LEECHSEED);
        gStatuses3[gBattlerAttacker] &= ~(STATUS3_LEECHSEED_BATTLER);
        BattleScriptPushCursor();
        gBattlescriptCurrInstr = BattleScript_LeechSeedFree;
    }
    else if (gSideStatuses[GetBattlerSide(gBattlerAttacker)] & SIDE_STATUS_SPIKES)
    {
        gSideStatuses[GetBattlerSide(gBattlerAttacker)] &= ~(SIDE_STATUS_SPIKES);
        gSideTimers[GetBattlerSide(gBattlerAttacker)].spikesAmount = 0;
        BattleScriptPushCursor();
        gBattlescriptCurrInstr = BattleScript_SpikesFree;
    }
    else
    {
        ++gBattlescriptCurrInstr;
    }
}

static void atkBF_setdefensecurlbit(void)
{
    gBattleMons[gBattlerAttacker].status2 |= STATUS2_DEFENSE_CURL;
    ++gBattlescriptCurrInstr;
}

static void atkC0_recoverbasedonsunlight(void)
{
    gBattlerTarget = gBattlerAttacker;
	
    if (gBattleMons[gBattlerAttacker].hp != gBattleMons[gBattlerAttacker].maxHP)
    {
        if (!IsBattlerWeatherAffected(gBattlerAttacker, WEATHER_ANY))
            gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 2;
        else if (IsBattlerWeatherAffected(gBattlerAttacker, WEATHER_SUN_ANY))
            gBattleMoveDamage = 20 * gBattleMons[gBattlerAttacker].maxHP / 30;
        else // not sunny weather
            gBattleMoveDamage = gBattleMons[gBattlerAttacker].maxHP / 4;

        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        gBattleMoveDamage *= -1;

        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkC1_nop(void)
{
        ++gBattlescriptCurrInstr;
}

static void atkC2_selectfirstvalidtarget(void)
{
    for (gBattlerTarget = 0; gBattlerTarget < gBattlersCount; ++gBattlerTarget)
        if (gBattlerTarget != gBattlerAttacker && !(gAbsentBattlerFlags & gBitTable[gBattlerTarget]))
            break;
    ++gBattlescriptCurrInstr;
}

static void atkC3_trysetfutureattack(void)
{
    if (gWishFutureKnock.futureSightCounter[gBattlerTarget] != 0)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        gWishFutureKnock.futureSightMove[gBattlerTarget] = gCurrentMove;
        gWishFutureKnock.futureSightAttacker[gBattlerTarget] = gBattlerAttacker;
        gWishFutureKnock.futureSightCounter[gBattlerTarget] = 3;
        gWishFutureKnock.futureSightDmg[gBattlerTarget] = CalculateBaseDamage(gCurrentMove, gBattleStruct->dynamicMoveType, gBattlerAttacker, gBattlerTarget, FALSE, FALSE, TRUE);
        if (gCurrentMove == MOVE_DOOM_DESIRE)
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkC4_trydobeatup(void)
{
    struct Pokemon *party;
    u8 AbilityNum, beforeLoop;
	
    if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
        party = gPlayerParty;
    else
        party = gEnemyParty;
    if (!IsBattlerAlive(gBattlerTarget))
    {
	gCritMultiplier = 1;
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else
    {
        beforeLoop = gBattleCommunication[0];

        for (;gBattleCommunication[0] < PARTY_SIZE; ++gBattleCommunication[0])
        {
            if (GetMonData(&party[gBattleCommunication[0]], MON_DATA_HP) && GetMonData(&party[gBattleCommunication[0]], MON_DATA_SPECIES2)
		&& GetMonData(&party[gBattleCommunication[0]], MON_DATA_SPECIES2) != SPECIES_EGG
		&& !GetMonData(&party[gBattleCommunication[0]], MON_DATA_STATUS))
                break;
        }
        if (gBattleCommunication[0] < PARTY_SIZE)
        {
            PREPARE_MON_NICK_WITH_PREFIX_BUFFER(gBattleTextBuff1, gBattlerAttacker, gBattleCommunication[0])
            gBattlescriptCurrInstr += 9;
            gBattleMoveDamage = gBaseStats[GetMonData(&party[gBattleCommunication[0]], MON_DATA_SPECIES)].baseAttack;
            gBattleMoveDamage *= gBattleMoves[gCurrentMove].power;
            gBattleMoveDamage *= (GetMonData(&party[gBattleCommunication[0]], MON_DATA_LEVEL) * 2 / 5 + 2);
            gBattleMoveDamage /= gBaseStats[gBattleMons[gBattlerTarget].species].baseDefense;
	    gBattleMoveDamage *= gCritMultiplier;
		
	    AbilityNum = GetMonData(&party[gBattleCommunication[0]], MON_DATA_ABILITY_NUM);
		
	    if (GetAbilityBySpecies(GetMonData(&party[gBattleCommunication[0]], MON_DATA_SPECIES), AbilityNum, GetMonData(&party[gBattleCommunication[0]], MON_DATA_ABILITY_HIDDEN)) == ABILITY_SNIPER && gCritMultiplier == 2)
		    gBattleMoveDamage = (gBattleMoveDamage * 15) / 10;
            gBattleMoveDamage = (gBattleMoveDamage / 50) + 2;
            if (gProtectStructs[gBattlerAttacker].helpingHand)
                gBattleMoveDamage = gBattleMoveDamage * 15 / 10;
            ++gBattleCommunication[0];
        }
        else if (beforeLoop != 0)
	{
	    gCritMultiplier = 1;
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
        else
	{
	    gCritMultiplier = 1;
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 5);
	}
    }
}

static void atkC5_setsemiinvulnerablebit(void)
{
    switch (gCurrentMove)
    {
    case MOVE_FLY:
    case MOVE_BOUNCE:
        gStatuses3[gBattlerAttacker] |= STATUS3_ON_AIR;
        break;
    case MOVE_DIG:
        gStatuses3[gBattlerAttacker] |= STATUS3_UNDERGROUND;
        break;
    case MOVE_DIVE:
        gStatuses3[gBattlerAttacker] |= STATUS3_UNDERWATER;
        break;
    }
    ++gBattlescriptCurrInstr;
}

static void atkC6_clearsemiinvulnerablebit(void)
{
    switch (gCurrentMove)
    {
    case MOVE_FLY:
    case MOVE_BOUNCE:
        gStatuses3[gBattlerAttacker] &= ~STATUS3_ON_AIR;
        break;
    case MOVE_DIG:
        gStatuses3[gBattlerAttacker] &= ~STATUS3_UNDERGROUND;
        break;
    case MOVE_DIVE:
        gStatuses3[gBattlerAttacker] &= ~STATUS3_UNDERWATER;
        break;
    }
    ++gBattlescriptCurrInstr;
}

static void atkC7_setminimize(void)
{
    if (gHitMarker & HITMARKER_OBEYS)
        gStatuses3[gBattlerAttacker] |= STATUS3_MINIMIZED;
    ++gBattlescriptCurrInstr;
}

static void atkC8_sethail(void)
{
    if (!TryChangeBattleWeather(gBattlerAttacker, ENUM_WEATHER_HAIL))
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
    }
    else
        gBattleCommunication[MULTISTRING_CHOOSER] = 5;
	
    ++gBattlescriptCurrInstr;
}

static void atkC9_jumpifattackandspecialattackcannotfall(void) // memento
{
    if (!BattlerStatCanFall(gBattlerTarget, STAT_ATK) && !BattlerStatCanFall(gBattlerTarget, STAT_SPATK) && gBattleCommunication[6] != 1)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        gActiveBattler = gBattlerAttacker;
        gBattleMoveDamage = gBattleMons[gActiveBattler].hp;
        BtlController_EmitHealthBarUpdate(0, INSTANT_HP_BAR_DROP);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 5;
    }
}

static void atkCA_setforcedtarget(void) // follow me
{
    gSideTimers[GetBattlerSide(gBattlerAttacker)].followmeTimer = 1;
    gSideTimers[GetBattlerSide(gBattlerAttacker)].followmeTarget = gBattlerAttacker;
    ++gBattlescriptCurrInstr;
}

static void atkCB_setcharge(void)
{
    gStatuses3[gBattlerAttacker] |= STATUS3_CHARGED_UP;
    gDisableStructs[gBattlerAttacker].chargeTimer = 2;
    ++gBattlescriptCurrInstr;
}

static void atkCC_callterrainattack(void) // nature power
{
    gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
    gCurrentMove = sNaturePowerMoves[gBattleTerrain];
    gBattlerTarget = GetMoveTarget(gCurrentMove, 0);
    BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
    ++gBattlescriptCurrInstr;
}

static void atkCD_multihitresultmessage(void)
{
	if (!gBattleControllerExecFlags)
	{
		if (!(gMoveResultFlags & MOVE_RESULT_FAILED) && !(gMoveResultFlags & MOVE_RESULT_FOE_ENDURED))
		{
			if (gMoveResultFlags & MOVE_RESULT_FOE_STURDIED)
			{
				gMoveResultFlags &= ~(MOVE_RESULT_FOE_STURDIED | MOVE_RESULT_FOE_HUNG_ON);
				gSpecialStatuses[gBattlerTarget].sturdied = 0;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_SturdiedMsg;
				return;
			}
			else if (gMoveResultFlags & MOVE_RESULT_FOE_HUNG_ON)
			{
				gLastUsedItem = gBattleMons[gBattlerTarget].item;
				gPotentialItemEffectBattler = gBattlerTarget;
				gMoveResultFlags &= ~(MOVE_RESULT_FOE_STURDIED | MOVE_RESULT_FOE_HUNG_ON);
				gSpecialStatuses[gBattlerTarget].focusBanded = 0;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_HangedOnMsg;
				return;
			}
		}
		++gBattlescriptCurrInstr;
	}
}

static void atkCE_settorment(void)
{
    if (gBattleMons[gBattlerTarget].status2 & STATUS2_TORMENT)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        gBattleMons[gBattlerTarget].status2 |= STATUS2_TORMENT;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkCF_jumpifnodamage(void)
{
    if (gProtectStructs[gBattlerAttacker].physicalDmg || gProtectStructs[gBattlerAttacker].specialDmg)
        gBattlescriptCurrInstr += 5;
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkD0_settaunt(void)
{
    if (gDisableStructs[gBattlerTarget].tauntTimer == 0)
    {
        gDisableStructs[gBattlerTarget].tauntTimer = 2;
        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkD1_trysethelpinghand(void)
{
    gBattlerTarget = GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(gBattlerAttacker)));
	
    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !(gAbsentBattlerFlags & gBitTable[gBattlerTarget]) && !gProtectStructs[gBattlerAttacker].helpingHand
	&& !gProtectStructs[gBattlerTarget].helpingHand)
    {
        gProtectStructs[gBattlerTarget].helpingHand = 1;
        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkD2_tryswapitems(void) // trick
{
    u8 sideAttacker = GetBattlerSide(gBattlerAttacker), sideTarget = GetBattlerSide(gBattlerTarget);
    u16 oldItemAtk, *newItemAtk;
	
    // opponent can't swap items with player in regular battles
    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER || (GetBattlerSide(gBattlerAttacker) == B_SIDE_OPPONENT
        && !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_EREADER_TRAINER))
        && gTrainerBattleOpponent_A != TRAINER_SECRET_BASE))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        // you can't swap items if they were knocked off in regular battles
        if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_EREADER_TRAINER))
	    && gTrainerBattleOpponent_A != TRAINER_SECRET_BASE && (gWishFutureKnock.knockedOffMons[sideAttacker] & gBitTable[gBattlerPartyIndexes[gBattlerAttacker]]
            || gWishFutureKnock.knockedOffMons[sideTarget] & gBitTable[gBattlerPartyIndexes[gBattlerTarget]]))
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        // can't swap if two pokemon don't have an item
        // or if either of them is an enigma berry or a mail
        else if ((gBattleMons[gBattlerAttacker].item == 0 && gBattleMons[gBattlerTarget].item == 0) || gBattleMons[gBattlerAttacker].item == ITEM_ENIGMA_BERRY
		 || gBattleMons[gBattlerTarget].item == ITEM_ENIGMA_BERRY || IS_ITEM_MAIL(gBattleMons[gBattlerAttacker].item)
		 || IS_ITEM_MAIL(gBattleMons[gBattlerTarget].item))
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        // check if ability prevents swapping
        else if (GetBattlerAbility(gBattlerTarget) == ABILITY_STICKY_HOLD)
        {
            gBattlescriptCurrInstr = BattleScript_StickyHoldActivates;
            gLastUsedAbility = gBattleMons[gBattlerTarget].ability;
            RecordAbilityBattle(gBattlerTarget, gLastUsedAbility);
        }
        // took a while, but all checks passed and items can be safely swapped
        else
        {
            newItemAtk = &gBattleStruct->changedItems[gBattlerAttacker];
            oldItemAtk = gBattleMons[gBattlerAttacker].item;
            *newItemAtk = gBattleMons[gBattlerTarget].item;
            gBattleMons[gBattlerAttacker].item = 0;
            gBattleMons[gBattlerTarget].item = oldItemAtk;
	    TryGiveUnburdenBoostToMon(gBattlerAttacker);
	    TryGiveUnburdenBoostToMon(gBattlerTarget);
            gActiveBattler = gBattlerAttacker;
            BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, newItemAtk);
            MarkBattlerForControllerExec(gBattlerAttacker);
            gActiveBattler = gBattlerTarget;
            BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gBattlerTarget].item);
            MarkBattlerForControllerExec(gBattlerTarget);
            *(u8 *)((u8 *)(&gBattleStruct->choicedMove[gBattlerTarget]) + 0) = 0;
            *(u8 *)((u8 *)(&gBattleStruct->choicedMove[gBattlerTarget]) + 1) = 0;
            *(u8 *)((u8 *)(&gBattleStruct->choicedMove[gBattlerAttacker]) + 0) = 0;
            *(u8 *)((u8 *)(&gBattleStruct->choicedMove[gBattlerAttacker]) + 1) = 0;
            gBattlescriptCurrInstr += 5;
            PREPARE_ITEM_BUFFER(gBattleTextBuff1, *newItemAtk)
            PREPARE_ITEM_BUFFER(gBattleTextBuff2, oldItemAtk)
            if (oldItemAtk != ITEM_NONE && *newItemAtk != ITEM_NONE)
                gBattleCommunication[MULTISTRING_CHOOSER] = 2; // attacker's item -> <- target's item
            else if (oldItemAtk == ITEM_NONE && *newItemAtk != ITEM_NONE)
                gBattleCommunication[MULTISTRING_CHOOSER] = 0; // nothing -> <- target's item
            else
                gBattleCommunication[MULTISTRING_CHOOSER] = 1; // attacker's item -> <- nothing
        }
    }
}

static void atkD3_trycopyability(void) // role play
{
    if (gBattleMons[gBattlerTarget].ability != ABILITY_NONE && 
	gBattleMons[gBattlerTarget].ability != ABILITY_WONDER_GUARD && 
	gBattleMons[gBattlerTarget].ability != ABILITY_FLOWER_GIFT)
    {
        gLastUsedAbility = gBattleMons[gBattlerAttacker].ability = gBattleMons[gBattlerTarget].ability;
	ResetVarsForAbilityChange(gBattlerAttacker);
        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkD4_trywish(void)
{
    switch (gBattlescriptCurrInstr[1])
    {
    case 0: // use wish
        if (gWishFutureKnock.wishCounter[gBattlerAttacker] == 0)
        {
            gWishFutureKnock.wishCounter[gBattlerAttacker] = 2;
            gWishFutureKnock.wishMonId[gBattlerAttacker] = gBattlerPartyIndexes[gBattlerAttacker];
            gBattlescriptCurrInstr += 6;
        }
        else
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
        break;
    case 1: // heal effect
        PREPARE_MON_NICK_WITH_PREFIX_BUFFER(gBattleTextBuff1, gBattlerTarget, gWishFutureKnock.wishMonId[gBattlerTarget])
        gBattleMoveDamage = gBattleMons[gBattlerTarget].maxHP / 2;
        if (gBattleMoveDamage == 0)
            gBattleMoveDamage = 1;
        gBattleMoveDamage *= -1;
        if (gBattleMons[gBattlerTarget].hp == gBattleMons[gBattlerTarget].maxHP)
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
        else
            gBattlescriptCurrInstr += 6;
        break;
    }
}

static void atkD5_trysetroots(void) // ingrain
{
    if (gStatuses3[gBattlerAttacker] & STATUS3_ROOTED)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        gStatuses3[gBattlerAttacker] |= STATUS3_ROOTED;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkD6_nop(void)
{
    ++gBattlescriptCurrInstr;
}

static void atkD7_setyawn(void)
{
    if (gStatuses3[gBattlerTarget] & STATUS3_YAWN || gBattleMons[gBattlerTarget].status1 & STATUS1_ANY)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        gStatuses3[gBattlerTarget] |= 0x1000;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkD8_setdamagetohealthdifference(void)
{
    if (gBattleMons[gBattlerTarget].hp <= gBattleMons[gBattlerAttacker].hp)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        gBattleMoveDamage = gBattleMons[gBattlerTarget].hp - gBattleMons[gBattlerAttacker].hp;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkD9_nop(void)
{
    ++gBattlescriptCurrInstr;
}

static void atkDA_tryswapabilities(void) // skill swap
{
    u16 abilityAtk;
	
    if ((gBattleMons[gBattlerAttacker].ability == 0 && gBattleMons[gBattlerTarget].ability == 0) || gBattleMons[gBattlerAttacker].ability == ABILITY_WONDER_GUARD
	|| gBattleMons[gBattlerTarget].ability == ABILITY_WONDER_GUARD || gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
         gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        abilityAtk = gBattleMons[gBattlerAttacker].ability;
        
        gBattleMons[gBattlerAttacker].ability = gBattleMons[gBattlerTarget].ability;
        gBattleMons[gBattlerTarget].ability = abilityAtk;
        ResetVarsForAbilityChange(gBattlerAttacker);
	ResetVarsForAbilityChange(gBattlerTarget);
	    
        gBattlescriptCurrInstr += 5;
    }
}

static void atkDB_tryimprison(void)
{
    u8 battlerId, sideAttacker;
    s32 i, attackerMoveId;
	
    if ((gStatuses3[gBattlerAttacker] & STATUS3_IMPRISONED_OTHERS))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        sideAttacker = GetBattlerSide(gBattlerAttacker);
        PressurePPLoseOnUsingImprison(gBattlerAttacker);
	    
        for (battlerId = 0; battlerId < gBattlersCount; ++battlerId)
        {
            if (sideAttacker != GetBattlerSide(battlerId))
            {
                for (attackerMoveId = 0; attackerMoveId < MAX_MON_MOVES; ++attackerMoveId)
                {
                    for (i = 0; i < MAX_MON_MOVES; ++i)
                    {
                        if (gBattleMons[gBattlerAttacker].moves[attackerMoveId] == gBattleMons[battlerId].moves[i]
			    && gBattleMons[gBattlerAttacker].moves[attackerMoveId] != MOVE_NONE)
                            break;
                    }
                    if (i != MAX_MON_MOVES)
                        break;
                }
                if (attackerMoveId != MAX_MON_MOVES)
                {
                    gStatuses3[gBattlerAttacker] |= STATUS3_IMPRISONED_OTHERS;
                    gBattlescriptCurrInstr += 5;
                    break;
                }
            }
        }
        if (battlerId == gBattlersCount) // In Generation 3 games, Imprison fails if the user doesn't share any moves with any of the foes
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

static void atkDC_trysetgrudge(void)
{
    if (gStatuses3[gBattlerAttacker] & STATUS3_GRUDGE)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        gStatuses3[gBattlerAttacker] |= STATUS3_GRUDGE;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkDD_nop(void)
{
    ++gBattlescriptCurrInstr;
}

static void atkDE_assistattackselect(void)
{
    u16 move, *movesArray = gBattleStruct->assistPossibleMoves;
    struct Pokemon *party;
    s32 i, monId, moveId, chooseableMovesNo = 0;

    if (GET_BATTLER_SIDE(gBattlerAttacker) != B_SIDE_PLAYER)
        party = gEnemyParty;
    else
        party = gPlayerParty;
    for (monId = 0; monId < PARTY_SIZE; ++monId)
    {
        if (monId == gBattlerPartyIndexes[gBattlerAttacker] || GetMonData(&party[monId], MON_DATA_SPECIES2) == SPECIES_NONE
	    || GetMonData(&party[monId], MON_DATA_SPECIES2) == SPECIES_EGG)
            continue;
        for (moveId = 0; moveId < MAX_MON_MOVES; ++moveId)
        {
            move = GetMonData(&party[monId], MON_DATA_MOVE1 + moveId);

            if (sForbiddenMoves[move] & FORBIDDEN_ASSIST)
                continue;
			
            movesArray[chooseableMovesNo] = move;
            ++chooseableMovesNo;
        }
    }
    if (chooseableMovesNo)
    {
        gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
        gCalledMove = movesArray[((Random() & 0xFF) * chooseableMovesNo) >> 8];
        gBattlerTarget = GetMoveTarget(gCalledMove, 0);
        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkDF_trysetmagiccoat(void)
{
    gBattlerTarget = gBattlerAttacker;
    gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
	
    if (gCurrentTurnActionNumber == gBattlersCount - 1) // moves last turn
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        gProtectStructs[gBattlerAttacker].bounceMove = 1;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkE0_trysetsnatch(void) // snatch
{
    gSpecialStatuses[gBattlerAttacker].ppNotAffectedByPressure = 1;
	
    if (gCurrentTurnActionNumber == gBattlersCount - 1) // moves last turn
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        gProtectStructs[gBattlerAttacker].stealMove = 1;
        gBattlescriptCurrInstr += 5;
    }
}

static void atkE1_trygetintimidatetarget(void)
{
    u8 side;

    gBattleScripting.battler = gBattleStruct->intimidateBattler;
    side = GetBattlerSide(gBattleScripting.battler);
    PREPARE_ABILITY_BUFFER(gBattleTextBuff1, gBattleMons[gBattleScripting.battler].ability)
	    
    for (; gBattlerTarget < gBattlersCount; ++gBattlerTarget)
        if (GetBattlerSide(gBattlerTarget) != side && !(gAbsentBattlerFlags & gBitTable[gBattlerTarget]))
            break;
    if (gBattlerTarget >= gBattlersCount)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
        gBattlescriptCurrInstr += 5;
}

static void atkE2_switchoutabilities(void)
{
	gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	
	switch (GetBattlerAbility(gActiveBattler))
	{
		case ABILITY_NATURAL_CURE:
			gBattleMons[gActiveBattler].status1 = 0;
			BtlController_EmitSetMonData(0, REQUEST_STATUS_BATTLE, gBitTable[*(gBattleStruct->battlerPartyIndexes + gActiveBattler)], 4, &gBattleMons[gActiveBattler].status1);
			MarkBattlerForControllerExec(gActiveBattler);
			break;
		case ABILITY_REGENERATOR:
			gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP / 3;
			gBattleMoveDamage += gBattleMons[gActiveBattler].hp;
			if (gBattleMoveDamage > gBattleMons[gActiveBattler].maxHP)
				gBattleMoveDamage = gBattleMons[gActiveBattler].maxHP;
			BtlController_EmitSetMonData(0, REQUEST_HP_BATTLE, gBitTable[*(gBattleStruct->battlerPartyIndexes + gActiveBattler)], 2, &gBattleMoveDamage);
			MarkBattlerForControllerExec(gActiveBattler);
			break;
    }
	TryDoBattleFormChange(gActiveBattler, BATTLE_FORM_CHANGE_SWITCH_OUT);
	
    gBattlescriptCurrInstr += 2;
}

static void atkE3_jumpifhasnohp(void)
{
    gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);

    if (gBattleMons[gActiveBattler].hp == 0)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
    else
        gBattlescriptCurrInstr += 6;
}

static void atkE4_getsecretpowereffect(void)
{
    switch (gBattleTerrain)
    {
    case BATTLE_TERRAIN_GRASS:
        gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_POISON;
        break;
    case BATTLE_TERRAIN_LONG_GRASS:
        gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_SLEEP;
        break;
    case BATTLE_TERRAIN_SAND:
        gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_ACC_MINUS_1;
        break;
    case BATTLE_TERRAIN_UNDERWATER:
        gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_DEF_MINUS_1;
        break;
    case BATTLE_TERRAIN_WATER:
        gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_ATK_MINUS_1;
        break;
    case BATTLE_TERRAIN_POND:
        gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_SPD_MINUS_1;
        break;
    case BATTLE_TERRAIN_MOUNTAIN:
        gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_CONFUSION;
        break;
    case BATTLE_TERRAIN_CAVE:
        gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_FLINCH;
        break;
    default:
        gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_PARALYSIS;
        break;
    }
    ++gBattlescriptCurrInstr;
}

static void atkE5_pickup(void)
{
    u8 level, chance, levelmax;
    u16 species, heldItem, ability;
    u32 j;
    s32 i, random;

    for (i = 0; i < PARTY_SIZE; ++i)
    {
	random = Random() % 100;
        species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2);
        heldItem = GetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM);
	ability = GetAbilityBySpecies(species, GetMonData(&gPlayerParty[i], MON_DATA_ABILITY_NUM), GetMonData(&gPlayerParty[i], MON_DATA_ABILITY_HIDDEN));    
        
	if (species != SPECIES_NONE && species != SPECIES_EGG && heldItem == ITEM_NONE)
	{
		if (ability == ABILITY_PICKUP && !(Random() % 10))
		{
			for (j = 0; j < 15; ++j)
			{
				if (sPickupItems[j].chance > random)
					break;
			}
			SetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM, &sPickupItems[j]);
		}
		else if (ability == ABILITY_HONEY_GATHER)
		{
			level = GetMonData(&gPlayerParty[i], MON_DATA_LEVEL);
			
			for (chance = 5, levelmax = 10; level > levelmax; chance += 5, levelmax += 10)
			{
				if (levelmax == MAX_LEVEL)
					break;
			}
			if (chance > random)
			{
				heldItem = ITEM_HONEY;
				SetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM, &heldItem);
			}
		}
	}
#if SHUCKLE_MAKES_BERRY_JUICE_CHANCE != 0
	else if (species == SPECIES_SHUCKLE && heldItem >= FIRST_BERRY_INDEX && heldItem <= LAST_BERRY_INDEX && Random() % 100 < SHUCKLE_MAKES_BERRY_JUICE_CHANCE)
	{
		heldItem = ITEM_BERRY_JUICE;
		SetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM, &heldItem);
	}
#endif
    }
    ++gBattlescriptCurrInstr;
}

static void atkE6_docastformchangeanimation(void)
{
    gActiveBattler = gBattleScripting.battler;
	
    if (gBattleMons[gActiveBattler].status2 & STATUS2_SUBSTITUTE)
        *(&gBattleStruct->formToChangeInto) |= 0x80;
    BtlController_EmitBattleAnimation(0, B_ANIM_CASTFORM_CHANGE, gBattleStruct->formToChangeInto);
    MarkBattlerForControllerExec(gActiveBattler);
    ++gBattlescriptCurrInstr;
}

static void atkE7_trycastformdatachange(void)
{
    u8 form;

    ++gBattlescriptCurrInstr;
    form = CastformDataTypeChange(gBattleScripting.battler);
    if (form)
    {
        BattleScriptPushCursorAndCallback(BattleScript_CastformChange);
        *(&gBattleStruct->formToChangeInto) = form - 1;
    }
}

static void atkE8_settypebasedhalvers(void) // water and mud sport
{
    bool8 worked = FALSE;

    if (gBattleMoves[gCurrentMove].effect == EFFECT_MUD_SPORT)
    {
        if (!(gStatuses3[gBattlerAttacker] & STATUS3_MUDSPORT))
        {
            gStatuses3[gBattlerAttacker] |= STATUS3_MUDSPORT;
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
            worked = TRUE;
        }
    }
    else // water sport
    {
        if (!(gStatuses3[gBattlerAttacker] & STATUS3_WATERSPORT))
        {
            gStatuses3[gBattlerAttacker] |= STATUS3_WATERSPORT;
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
            worked = TRUE;
        }
    }
    if (worked)
        gBattlescriptCurrInstr += 5;
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkE9_nop(void)
{
	++gBattlescriptCurrInstr;
}

static void atkEA_tryrecycleitem(void)
{
    u16 *usedHeldItem;

    gActiveBattler = gBattlerAttacker;
    usedHeldItem = &gBattleStruct->usedHeldItems[gBattlerPartyIndexes[gActiveBattler]][GetBattlerSide(gActiveBattler)];
	
    if (*usedHeldItem != ITEM_NONE && gBattleMons[gActiveBattler].item == ITEM_NONE)
    {
        gLastUsedItem = *usedHeldItem;
        *usedHeldItem = ITEM_NONE;
        gBattleMons[gActiveBattler].item = gLastUsedItem;
	TryGiveUnburdenBoostToMon(gActiveBattler); // try remove attacker's unburden boost
        BtlController_EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
        MarkBattlerForControllerExec(gActiveBattler);
        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkEB_settypetoterrain(void)
{
    if (!IS_BATTLER_OF_TYPE(gBattlerAttacker, sTerrainToType[gBattleTerrain]))
    {
        SET_BATTLER_TYPE(gBattlerAttacker, sTerrainToType[gBattleTerrain]);
        PREPARE_TYPE_BUFFER(gBattleTextBuff1, sTerrainToType[gBattleTerrain]);
        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkEC_pursuitrelated(void)
{
    gActiveBattler = GetBattlerAtPosition(BATTLE_PARTNER(GetBattlerPosition(gBattlerAttacker)));

    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !(gAbsentBattlerFlags & gBitTable[gActiveBattler])
	&& gChosenActionByBattler[gActiveBattler] == 0 && gChosenMoveByBattler[gActiveBattler] == MOVE_PURSUIT)
    {
        gActionsByTurnOrder[gActiveBattler] = 11;
        gCurrentMove = MOVE_PURSUIT;
        gBattlescriptCurrInstr += 5;
        gBattleScripting.animTurn = 1;
        gBattleScripting.field_20 = gBattlerAttacker;
        gBattlerAttacker = gActiveBattler;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

static void atkED_snatchsetbattlers(void)
{
    gEffectBattler = gBattlerAttacker;
	
    if (gBattlerAttacker == gBattlerTarget)
        gBattlerAttacker = gBattlerTarget = gBattleScripting.battler;
    else
        gBattlerTarget = gBattleScripting.battler;
    gBattleScripting.battler = gEffectBattler;
    ++gBattlescriptCurrInstr;
}

static void atkEE_removelightscreenreflect(void) // brick break
{
    u8 opposingSide = BATTLE_OPPOSITE(GetBattlerSide(gBattlerAttacker));

    if (gSideTimers[opposingSide].reflectTimer || gSideTimers[opposingSide].lightscreenTimer)
    {
        gSideStatuses[opposingSide] &= ~(SIDE_STATUS_REFLECT);
        gSideStatuses[opposingSide] &= ~(SIDE_STATUS_LIGHTSCREEN);
        gSideTimers[opposingSide].reflectTimer = 0;
        gSideTimers[opposingSide].lightscreenTimer = 0;
        gBattleScripting.animTurn = 1;
        gBattleScripting.animTargetsHit = 1;
    }
    else
    {
        gBattleScripting.animTurn = 0;
        gBattleScripting.animTargetsHit = 0;
    }
    ++gBattlescriptCurrInstr;
}

static void atkEF_handleballthrow(void)
{
    u8 shakes, catchRate, ballMultiplier = 0;
    u32 odds;

    if (!gBattleControllerExecFlags)
    {
        gActiveBattler = gBattlerAttacker;
        gBattlerTarget = BATTLE_OPPOSITE(gBattlerAttacker);
	    
        if (gBattleTypeFlags & BATTLE_TYPE_GHOST)
        {
            BtlController_EmitBallThrowAnim(0, BALL_GHOST_DODGE);
            MarkBattlerForControllerExec(gActiveBattler);
            gBattlescriptCurrInstr = BattleScript_GhostBallDodge;
        }
        else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        {
            BtlController_EmitBallThrowAnim(0, BALL_TRAINER_BLOCK);
            MarkBattlerForControllerExec(gActiveBattler);
            gBattlescriptCurrInstr = BattleScript_TrainerBallBlock;
        }
        else if (gBattleTypeFlags & (BATTLE_TYPE_POKEDUDE | BATTLE_TYPE_OLD_MAN_TUTORIAL))
        {
            BtlController_EmitBallThrowAnim(0, BALL_3_SHAKES_SUCCESS);
            MarkBattlerForControllerExec(gActiveBattler);
            gBattlescriptCurrInstr = BattleScript_OldMan_Pokedude_CaughtMessage;
        }
        else
        {
            if (gLastUsedItem == ITEM_SAFARI_BALL)
                catchRate = gBattleStruct->safariCatchFactor * 1275 / 100;
            else
                catchRate = gBaseStats[gBattleMons[gBattlerTarget].species].catchRate;
            if (gLastUsedItem > ITEM_SAFARI_BALL)
            {
                switch (gLastUsedItem)
                {
                case ITEM_NET_BALL:
                    if (IS_BATTLER_OF_TYPE(gBattlerTarget, TYPE_WATER) || IS_BATTLER_OF_TYPE(gBattlerTarget, TYPE_BUG))
                        ballMultiplier = 30;
                    else
                        ballMultiplier = 10;
                    break;
                case ITEM_DIVE_BALL:
                    if (GetCurrentMapType() == MAP_TYPE_UNDERWATER)
                        ballMultiplier = 35;
                    else
                        ballMultiplier = 10;
                    break;
                case ITEM_NEST_BALL:
                    if (gBattleMons[gBattlerTarget].level < 40)
                    {
                        ballMultiplier = 40 - gBattleMons[gBattlerTarget].level;
                        if (ballMultiplier <= 9)
                            ballMultiplier = 10;
                    }
                    else
                        ballMultiplier = 10;
                    break;
                case ITEM_REPEAT_BALL:
                    if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(gBattleMons[gBattlerTarget].species), FLAG_GET_CAUGHT))
                        ballMultiplier = 30;
                    else
                        ballMultiplier = 10;
                    break;
                case ITEM_TIMER_BALL:
                    ballMultiplier = gBattleResults.battleTurnCounter + 10;
                    if (ballMultiplier > 40)
                        ballMultiplier = 40;
                    break;
                case ITEM_LUXURY_BALL:
                case ITEM_PREMIER_BALL:
                    ballMultiplier = 10;
                    break;
                }
            }
            else
                ballMultiplier = sBallCatchBonuses[gLastUsedItem - 2];
            odds = (catchRate * ballMultiplier / 10)
                    * (gBattleMons[gBattlerTarget].maxHP * 3 - gBattleMons[gBattlerTarget].hp * 2)
                    / (3 * gBattleMons[gBattlerTarget].maxHP);
            if (gBattleMons[gBattlerTarget].status1 & (STATUS1_SLEEP | STATUS1_FREEZE))
                odds *= 2;
            if (gBattleMons[gBattlerTarget].status1 & (STATUS1_POISON | STATUS1_BURN | STATUS1_PARALYSIS | STATUS1_TOXIC_POISON))
                odds = (odds * 15) / 10;
            if (gLastUsedItem != ITEM_SAFARI_BALL)
            {
                if (gLastUsedItem == ITEM_MASTER_BALL)
                    gBattleResults.usedMasterBall = TRUE;
                else
                {
                    if (gBattleResults.catchAttempts[gLastUsedItem - ITEM_ULTRA_BALL] < 0xFF)
                        ++gBattleResults.catchAttempts[gLastUsedItem - ITEM_ULTRA_BALL];
                }
            }
            if (odds > 254) // mon caught
            {
                BtlController_EmitBallThrowAnim(0, BALL_3_SHAKES_SUCCESS);
                MarkBattlerForControllerExec(gActiveBattler);
                gBattlescriptCurrInstr = BattleScript_SuccessBallThrow;
                SetMonData(&gEnemyParty[gBattlerPartyIndexes[gBattlerTarget]], MON_DATA_POKEBALL, &gLastUsedItem);
                if (CalculatePlayerPartyCount() == 6)
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                else
                    gBattleCommunication[MULTISTRING_CHOOSER] = 1;
            }
            else // mon may be caught, calculate shakes
            {
                odds = Sqrt(Sqrt(16711680 / odds));
                odds = 1048560 / odds;
                for (shakes = 0; shakes < 4 && Random() < odds; ++shakes);
                if (gLastUsedItem == ITEM_MASTER_BALL)
                    shakes = BALL_3_SHAKES_SUCCESS; // why calculate the shakes before that check?
                BtlController_EmitBallThrowAnim(0, shakes);
                MarkBattlerForControllerExec(gActiveBattler);
                if (shakes == BALL_3_SHAKES_SUCCESS) // mon caught, copy of the code above
                {
                    gBattlescriptCurrInstr = BattleScript_SuccessBallThrow;
                    SetMonData(&gEnemyParty[gBattlerPartyIndexes[gBattlerTarget]], MON_DATA_POKEBALL, &gLastUsedItem);
                    if (CalculatePlayerPartyCount() == 6)
                        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                    else
                        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                }
                else // not caught
                {
                    gBattleCommunication[MULTISTRING_CHOOSER] = shakes;
                    gBattlescriptCurrInstr = BattleScript_ShakeBallThrow;
                }
            }
        }
    }
}

static void atkF0_givecaughtmon(void)
{
    if (GiveMonToPlayer(&gEnemyParty[gBattlerPartyIndexes[BATTLE_OPPOSITE(gBattlerAttacker)]]) != MON_GIVEN_TO_PARTY)
    {
        if (!ShouldShowBoxWasFullMessage())
        {
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
            StringCopy(gStringVar1, GetBoxNamePtr(VarGet(VAR_PC_BOX_TO_SEND_MON)));
            GetMonData(&gEnemyParty[gBattlerPartyIndexes[BATTLE_OPPOSITE(gBattlerAttacker)]], MON_DATA_NICKNAME, gStringVar2);
        }
        else
        {
            StringCopy(gStringVar1, GetBoxNamePtr(VarGet(VAR_PC_BOX_TO_SEND_MON))); // box the mon was sent to
            GetMonData(&gEnemyParty[gBattlerPartyIndexes[BATTLE_OPPOSITE(gBattlerAttacker)]], MON_DATA_NICKNAME, gStringVar2);
            StringCopy(gStringVar3, GetBoxNamePtr(GetPCBoxToSendMon())); //box the mon was going to be sent to
            gBattleCommunication[MULTISTRING_CHOOSER] = 2;
        }
        if (FlagGet(FLAG_SYS_NOT_SOMEONES_PC))
            ++gBattleCommunication[MULTISTRING_CHOOSER];
    }
    gBattleResults.caughtMonSpecies = gBattleMons[BATTLE_OPPOSITE(gBattlerAttacker)].species;
    GetMonData(&gEnemyParty[gBattlerPartyIndexes[BATTLE_OPPOSITE(gBattlerAttacker)]], MON_DATA_NICKNAME, gBattleResults.caughtMonNick);
    ++gBattlescriptCurrInstr;
}

static void atkF1_trysetcaughtmondexflags(void)
{
    u16 species = GetMonData(&gEnemyParty[0], MON_DATA_SPECIES, NULL);
    u32 personality = GetMonData(&gEnemyParty[0], MON_DATA_PERSONALITY, NULL);

    if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    else
    {
        HandleSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_SET_CAUGHT, personality);
        gBattlescriptCurrInstr += 5;
    }
}

static void atkF2_displaydexinfo(void)
{
    u16 species = GetMonData(&gEnemyParty[0], MON_DATA_SPECIES, NULL);

    switch (gBattleCommunication[0])
    {
    case 0:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 0x10, RGB_WHITE);
        ++gBattleCommunication[0];
        break;
    case 1:
        if (!gPaletteFade.active)
        {
            FreeAllWindowBuffers();
            gBattleCommunication[TASK_ID] = DexScreen_RegisterMonToPokedex(species);
            ++gBattleCommunication[0];
        }
        break;
    case 2:
        if (!gPaletteFade.active && gMain.callback2 == BattleMainCB2 && !gTasks[gBattleCommunication[TASK_ID]].isActive)
        {
            CpuFill32(0, (void *)VRAM, VRAM_SIZE);
            SetVBlankCallback(VBlankCB_Battle);
            ++gBattleCommunication[0];
        }
        break;
    case 3:
        InitBattleBgsVideo();
        LoadBattleTextboxAndBackground();
        gBattle_BG3_X = 0x100;
        ++gBattleCommunication[0];
        break;
    case 4:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            CreateMonPicSprite(species, gBattleMons[B_POSITION_OPPONENT_LEFT].otId, gBattleMons[B_POSITION_OPPONENT_LEFT].personality, TRUE, 120, 64, 0, 0xFFFF);
            CpuFill32(0, gPlttBufferFaded, BG_PLTT_SIZE);
            BeginNormalPaletteFade(0x1FFFF, 0, 0x10, 0, RGB_BLACK);
            ShowBg(0);
            ShowBg(3);
            ++gBattleCommunication[0];
        }
        break;
    case 5:
        if (!gPaletteFade.active)
            ++gBattlescriptCurrInstr;
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
    u16 src[2];

    src[0] = 1;
    src[1] = 2;
    CopyToBgTilemapBufferRect_ChangePalette(0, src, 0x18, 9 + (2 * gBattleCommunication[1]), 1, 2, 0x11);
    CopyBgTilemapBufferToVram(0);
}

void BattleDestroyYesNoCursorAt(void)
{
    u16 src[2];

    src[0] = 32;
    src[1] = 32;
    CopyToBgTilemapBufferRect_ChangePalette(0, src, 0x18, 9 + (2 * gBattleCommunication[1]), 1, 2, 0x11);
    CopyBgTilemapBufferToVram(0);
}

static void atkF3_trygivecaughtmonnick(void)
{
    switch (gBattleCommunication[MULTIUSE_STATE])
    {
    case 0:
        HandleBattleWindow(0x17, 8, 0x1D, 0xD, 0);
        BattlePutTextOnWindow(gText_BattleYesNoChoice, 0xE);
        ++gBattleCommunication[MULTIUSE_STATE];
        gBattleCommunication[CURSOR_POSITION] = 0;
        BattleCreateYesNoCursorAt();
        break;
    case 1:
        if (JOY_NEW(DPAD_UP) && gBattleCommunication[CURSOR_POSITION] != 0)
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
                BeginFastPaletteFade(3);
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
            GetMonData(&gEnemyParty[gBattlerPartyIndexes[BATTLE_OPPOSITE(gBattlerAttacker)]], MON_DATA_NICKNAME, gBattleStruct->caughtMonNick);
            FreeAllWindowBuffers();
            DoNamingScreen(NAMING_SCREEN_CAUGHT_MON, gBattleStruct->caughtMonNick,
                           GetMonData(&gEnemyParty[gBattlerPartyIndexes[BATTLE_OPPOSITE(gBattlerAttacker)]], MON_DATA_SPECIES),
                           GetMonGender(&gEnemyParty[gBattlerPartyIndexes[BATTLE_OPPOSITE(gBattlerAttacker)]]), BattleMainCB2);
            ++gBattleCommunication[MULTIUSE_STATE];
        }
        break;
    case 3:
        if (gMain.callback2 == BattleMainCB2 && !gPaletteFade.active)
        {
            SetMonData(&gEnemyParty[gBattlerPartyIndexes[BATTLE_OPPOSITE(gBattlerAttacker)]], MON_DATA_NICKNAME, gBattleStruct->caughtMonNick);
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        }
        break;
    case 4:
        if (CalculatePlayerPartyCount() == PARTY_SIZE)
            gBattlescriptCurrInstr += 5;
        else
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        break;
    }
}

static void atkF4_subattackerhpbydmg(void)
{
    gBattleMons[gBattlerAttacker].hp -= gBattleMoveDamage;
    ++gBattlescriptCurrInstr;
}

static void atkF5_removeattackerstatus1(void)
{
    gBattleMons[gBattlerAttacker].status1 = 0;
    ++gBattlescriptCurrInstr;
}

static void atkF6_finishaction(void)
{
    gCurrentActionFuncId = B_ACTION_FINISHED;
}

static void atkF7_finishturn(void)
{
    gCurrentActionFuncId = B_ACTION_FINISHED;
    gCurrentTurnActionNumber = gBattlersCount;
}

static void atkF8_callasm(void)
{
	void (*func )(void) = ((void (*)(void))T1_READ_PTR(gBattlescriptCurrInstr + 1));
	func();
	gBattlescriptCurrInstr += 5;
}

static void atkF9_cureprimarystatus(void)
{
	u8 bank = GetBattlerForBattleScript(gBattlescriptCurrInstr[1]);
	
	if (gBattleMons[bank].status1 & STATUS1_ANY) 
	{
		ClearBattlerStatus(bank);
		gBattlescriptCurrInstr += 6;
	}
	else
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
}

static void atkFA_setword(void)
{
	u32 *dest = T2_READ_PTR(gBattlescriptCurrInstr + 1);
	u32 word = T1_READ_32(gBattlescriptCurrInstr + 5);
	*dest = word;
	gBattlescriptCurrInstr += 9;
}

static void atkFB_jumpifsubstituteblocks(void)
{
	if (SubsBlockMove(gBattlerAttacker, gBattlerTarget, gCurrentMove)) 
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	else
		gBattlescriptCurrInstr += 5;
}

static void atkFC_loadabilitypopup(void)
{
	u8 animId;
	const u16 *argumentPtr = NULL;
	
	gActiveBattler = GetBattlerForBattleScript(gBattlescriptCurrInstr[2]);
	
	if (gBattlescriptCurrInstr[1] != REMOVE_POP_UP)
	{
		if (gBattleScripting.fixedAbilityPopUp)
		{
			gBattlescriptCurrInstr += 5;
			return;
		}
		animId = B_ANIM_LOAD_ABILITY_POP_UP;
	}
	else
		animId = B_ANIM_REMOVE_ABILITY_POP_UP;
	
	BtlController_EmitBattleAnimation(0, animId, *argumentPtr);
	MarkBattlerForControllerExec(gActiveBattler);
	gBattlescriptCurrInstr += 5;
}

static void atkFD_nop(void)
{
	++gBattlescriptCurrInstr;
}

static void atkFE_nop(void)
{
	++gBattlescriptCurrInstr;
}

//callasm command asm's
static bool8 AnticipationTypeCalc(u8 battler)
{
	u8 i, moveType;
	u16 moveId, flags = 0;
	
        for (i = 0; i < MAX_MON_MOVES; i++)
	{
		moveId = gBattleMons[battler].moves[i];
		
		if (moveId != MOVE_NONE)
		{
			if (gBattleMoves[moveId].effect == EFFECT_OHKO)
				return TRUE;
			
			if (gBattleMoves[moveId].power)
			{
				if (gBattleMoves[moveId].effect == EFFECT_HIDDEN_POWER)
					moveType = GetHiddenPowerType(GetBattlerPartyIndexPtr(battler));
				else
					moveType = gBattleMoves[moveId].type;
				
				TypeCalc(moveId, moveType, battler, gBattlerAttacker, FALSE, FALSE, &flags);
				
				if (flags & MOVE_RESULT_SUPER_EFFECTIVE)
					return TRUE;
			}
		}
	}
	return FALSE;
}

void TryDoAnticipationShudder(void)
{
	if (AnticipationTypeCalc(gBattlerTarget) || (IsBattlerAlive(BATTLE_PARTNER(gBattlerTarget)) && AnticipationTypeCalc(BATTLE_PARTNER(gBattlerTarget))))
		gSetWordLoc = sAnticipationString;
	else
		gBattlescriptCurrInstr = BattleScript_AnticipationReturn;
}

void GetStatRaiseDownload(void)
{
	u8 bank2 = BATTLE_PARTNER(gBattlerTarget);
	u32 def, spdef, def2, spdef2;
	
	def = gBattleMons[gBattlerTarget].defense;
	APPLY_STAT_MOD(def, &gBattleMons[gBattlerTarget], def, STAT_DEF);
	spdef = gBattleMons[gBattlerTarget].spDefense;
	APPLY_STAT_MOD(spdef, &gBattleMons[gBattlerTarget], spdef, STAT_SPDEF);
	
	if (IsBattlerAlive(bank2))
	{
		def2 = gBattleMons[bank2].defense;
		APPLY_STAT_MOD(def2, &gBattleMons[bank2], def2, STAT_DEF);
		spdef2 = gBattleMons[bank2].spDefense;
		APPLY_STAT_MOD(spdef2, &gBattleMons[bank2], spdef2, STAT_SPDEF);
		
		def += def2;
		spdef += spdef2;
	}
	
	gSetWordLoc = sDownloadString;
	
	if (def >= spdef)
		gBattlescriptCurrInstr = BattleScript_DownloadRaiseSpAttack;
}

static u16 GetForewarnMovePower(u16 move)
{
	switch (gBattleMoves[move].effect)
	{
		case EFFECT_OHKO:
		    return 150;
		case EFFECT_COUNTER:
		case EFFECT_MIRROR_COAT:
		case EFFECT_METAL_BURST:
			return 120;
		default:
		    if (gBattleMoves[move].power == 1)
				return 80;
			else
				return gBattleMoves[move].power;
	}
}

void GetStrongestMoveForewarn(void)
{
	u8 strongesttarget, i, bank2 = BATTLE_PARTNER(gBattlerTarget);
	u16 move, strongestmove = MOVE_NONE, power, maxpower = 0;
	
	for (i = 0; i < MAX_MON_MOVES; i++)
	{
		move = gBattleMons[gBattlerTarget].moves[i];
		power = GetForewarnMovePower(move);
		
		if (strongestmove == MOVE_NONE)
		{
			strongestmove = move;
			maxpower = power;
			strongesttarget = gBattlerTarget;
		}
		else if (power > maxpower || (power == maxpower && Random() & 1))
		{
			strongestmove = move;
			maxpower = power;
			strongesttarget = gBattlerTarget;
		}
		
		if (IsBattlerAlive(bank2))
		{
			move = gBattleMons[bank2].moves[i];
			
			if (move != MOVE_NONE)
			{
				power = GetForewarnMovePower(move);
				
				if (strongestmove == MOVE_NONE)
				{
					strongestmove = move;
					maxpower = power;
					strongesttarget = bank2;
				}
				else if (power > maxpower || (power == maxpower && Random() & 1))
				{
					strongestmove = move;
					maxpower = power;
					strongesttarget = bank2;
				}
			}
		}
	}
	gBattlerTarget = strongesttarget;
	PREPARE_MOVE_BUFFER(gBattleTextBuff1, strongestmove);
	gSetWordLoc = sForewarnString;
}

void TryFriskFirstTarget(void)
{
	u8 target2 = BATTLE_PARTNER(gBattlerTarget);
	gSetWordLoc = sFriskString;
	
	if (!(gBattleMons[gBattlerTarget].item))
	{
		if (IsBattlerAlive(target2) && gBattleMons[target2].item)
		{
			gLastUsedItem = gBattleMons[target2].item;
			gBattlerTarget = target2;
			gBattlescriptCurrInstr = BattleScript_TryFriskSecondTarget;
		}
		else
			gBattlescriptCurrInstr = BattleScript_AnticipationReturn;
	}
	else
		gLastUsedItem = gBattleMons[gBattlerTarget].item;
}

void TryFriskSecondTarget(void)
{
	if (IsBattlerAlive(BATTLE_PARTNER(gBattlerTarget)) && gBattleMons[BATTLE_PARTNER(gBattlerTarget)].item)
	{
		gBattlerTarget = BATTLE_PARTNER(gBattlerTarget);
		gLastUsedItem = gBattleMons[gBattlerTarget].item;
	}
	else
		gBattlescriptCurrInstr = BattleScript_AnticipationReturn;
}

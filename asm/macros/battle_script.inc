@ Define these here since misc_constants.inc conflicts with the C headers
	.set NULL, 0
	.set FALSE, 0
	.set TRUE, 1

@ commands
	.macro attackcanceler
	.byte 0x0
	.endm

	.macro accuracycheck jumpStr:req, noAccCal=FALSE
	.byte 0x1
	.4byte \jumpStr
	.byte \noAccCal
	.endm

	.macro attackstring
	.byte 0x2
	.endm

	.macro ppreduce
	.byte 0x3
	.endm

	.macro critcalc
	.byte 0x4
	.endm

	.macro damagecalc
	.byte 0x5
	.endm

	.macro typecalc
	.byte 0x6
	.endm

    .macro adjustdamage
	.byte 0x7
	.endm
	
	.macro waitstate
	.byte 0x8
	.endm

	.macro attackanimation
	.byte 0x9
	.endm
	
	.macro pause numFrames:req
	.byte 0xa
	.2byte \numFrames
	.endm

	.macro healthbarupdate battler:req
	.byte 0xb
	.byte \battler
	.endm

	.macro datahpupdate battler:req
	.byte 0xc
	.byte \battler
	.endm

	.macro critmessage
	.byte 0xd
	.endm

	.macro effectivenesssound
	.byte 0xe
	.endm

	.macro resultmessage
	.byte 0xf
	.endm

	.macro printstring stringId:req
	.byte 0x10
	.2byte \stringId
	.endm
	
	.macro printselectionstringfromtable ptr:req
	.byte 0x11
	.4byte \ptr
	.endm
	
	.macro waitmessage numFrames:req
	.byte 0x12
	.2byte \numFrames
	.endm

	.macro printfromtable ptr:req
	.byte 0x13
	.4byte \ptr
	.endm

	.macro setmoveeffect effect:req, affectsUser=FALSE, certain=FALSE
	.byte 0x14
	.byte \effect
	.byte \affectsUser
	.byte \certain
	.endm
	
	.macro setadditionaleffects
	1:
	.byte 0x15
	jumpifadditionaleffectsloops 1b
	.endm

	.macro setspecialmoveeffect primary:req, flags:req
	.byte 0x16
	.byte \primary
	.4byte \flags
	.endm
	
	.macro jumpifadditionaleffectsloops ptr:req
	.byte 0x17
	.4byte \ptr
	.endm
	
	.macro doeffectsonfaint battler:req
	.byte 0x18
	.byte \battler
	.endm

	.macro tryfaintmon battler:req
	.byte 0x19
	.byte \battler
	.endm
	
	.macro clearbattlerstatus battler:req, statusId:req, status:req
	.byte 0x1a
	.byte \battler
	.byte \statusId
	.4byte \status
	.endm

	.macro chosenstatusanimation battler:req, statusId:req, status:req
	.byte 0x1b
	.byte \battler
	.byte \statusId
	.4byte \status
	.endm

	.macro jumpbasedonstatus battler:req, statusId:req, cmpKind:req, status:req, ptr:req
	.byte 0x1c
	.byte \battler
	.byte \statusId
	.byte \cmpKind
	.4byte \status
	.4byte \ptr
	.endm

	.macro jumpbasedonstatus1 battler:req, cmpKind:req, statusId:req, ptr:req
	.byte 0x1d
	.byte \battler
	.byte \cmpKind
	.byte \statusId
	.4byte \ptr
	.endm
	
	.macro jumpifsideaffecting battler:req, sidestatus:req, ptr:req
	.byte 0x1e
	.byte \battler
	.2byte \sidestatus
	.4byte \ptr
	.endm

	.macro jumpbasedontype battler:req, type:req, jumpIfHasType:req, ptr:req
    .byte 0x1f
	.byte \battler
	.byte \type
	.byte \jumpIfHasType
	.4byte \ptr
	.endm

	.macro jumpifstat battler:req, cmpKind:req, statId:req, cmpTo:req, ptr:req
	.byte 0x20
	.byte \battler
	.byte \cmpKind
	.byte \statId
	.byte \cmpTo
	.4byte \ptr
	.endm
	
	.macro jumpifability battler:req, ability:req, ptr:req, record=FALSE
	.byte 0x21
	.byte \battler
	.2byte \ability
	.4byte \ptr
	.byte \record
	.endm

    .macro jumpifabilityonside battler:req, abilityId:req, ptr:req, record=FALSE
	.byte 0x22
	.byte \battler
	.2byte \abilityId
	.4byte \ptr
	.byte \record
	.endm

	.macro getexp battler:req
	.byte 0x23
	.byte \battler
	.endm

	.macro checkteamslot ptr:req
	.byte 0x24
	.4byte \ptr
	.endm

	.macro saveattacker
	.byte 0x25
	.endm

	.macro restoreattacker
	.byte 0x26
	.endm

	.macro savetarget
	.byte 0x27
	.endm
	
	.macro restoretarget
	.byte 0x28
	.endm

	.macro jumpifbyte cmpKind:req, memPtr:req, cmpTo:req, ptr:req
	.byte 0x29
	.byte \cmpKind
	.4byte \memPtr
	.byte \cmpTo
	.4byte \ptr
	.endm

	.macro jumpifhalfword cmpKind:req, memPtr:req, cmpTo:req, ptr:req
	.byte 0x2a
	.byte \cmpKind
	.4byte \memPtr
	.2byte \cmpTo
	.4byte \ptr
	.endm

	.macro jumpifword cmpKind:req, memPtr:req, cmpTo:req, ptr:req
	.byte 0x2b
	.byte \cmpKind
	.4byte \memPtr
	.4byte \cmpTo
	.4byte \ptr
	.endm

	.macro jumpifarrayequal memPtr1:req, memPtr2:req, size:req, ptr:req
	.byte 0x2c
	.4byte \memPtr1
	.4byte \memPtr2
	.byte \size
	.4byte \ptr
	.endm

	.macro jumpifarraynotequal memPtr1:req, memPtr2:req, size:req, ptr:req
	.byte 0x2d
	.4byte \memPtr1
	.4byte \memPtr2
	.byte \size
	.4byte \ptr
	.endm

	.macro setbyte memPtr:req, value:req
	.byte 0x2e
	.4byte \memPtr
	.byte \value
	.endm

	.macro addbyte memPtr:req, value:req
	.byte 0x2f
	.4byte \memPtr
	.byte \value
	.endm

	.macro subbyte memPtr:req, value:req
	.byte 0x30
	.4byte \memPtr
	.byte \value
	.endm

	.macro copyarray dest:req, src:req, size:req
	.byte 0x31
	.4byte \dest
	.4byte \src
	.byte \size
	.endm
	
	.macro sethword memPtr:req, value:req
	.byte 0x32
	.4byte \memPtr
	.2byte \value
	.endm
	
	.macro setword dst:req, value:req
	.byte 0x33
	.4byte \dst
	.4byte \value
	.endm
	
	.macro orbyte memPtr:req, value:req
	.byte 0x34
	.4byte \memPtr
	.byte \value
	.endm

	.macro orhalfword memPtr:req, value:req
	.byte 0x35
	.4byte \memPtr
	.2byte \value
	.endm

	.macro orword memPtr:req, value:req
	.byte 0x36
	.4byte \memPtr
	.4byte \value
	.endm

	.macro bicbyte memPtr:req, value:req
	.byte 0x37
	.4byte \memPtr
	.byte \value
	.endm

	.macro bichalfword memPtr:req, value:req
	.byte 0x38
	.4byte \memPtr
	.2byte \value
	.endm

	.macro bicword memPtr:req, value:req
	.byte 0x39
	.4byte \memPtr
	.4byte \value
	.endm

	.macro jumpifweathercheckchargeeffects battler:req, checkChargeTurnEffects:req, jumpInstr:req
	.byte 0x3a
	.byte \battler
	.byte \checkChargeTurnEffects
	.4byte \jumpInstr
	.endm

	.macro setstatusfromargument battler:req, failPtr:req
	.byte 0x3b
	.byte \battler
	.4byte \failPtr
	.endm

	.macro return
	.byte 0x3c
	.endm

	.macro end
	.byte 0x3d
	.endm

	.macro end2
	.byte 0x3e
	.endm

	.macro end3
	.byte 0x3f
	.endm

	.macro goto ptr:req
	.byte 0x40
	.4byte \ptr
	.endm

	.macro call ptr:req
	.byte 0x41
	.4byte \ptr
	.endm

	.macro trysetsleep battler:req, flags:req
	.byte 0x42
	.byte \battler
	.4byte \flags
	.endm

	.macro trysetconfusion battler:req
	.byte 0x43
	.byte \battler
	.endm

	.macro trysetpoison battler:req
	.byte 0x44
	.byte \battler
	.endm
	
	.macro trysetburn battler:req
	.byte 0x45
	.byte \battler
	.endm
	
	.macro trysetparalyze battler:req
	.byte 0x46
	.byte \battler
	.endm
	
	.macro endlinkbattle
	.byte 0x47
	.endm
	
	.macro prefaintmoveend state:req
	.byte 0x48
	.byte \state
	.endm

	.macro moveend state:req, case:req
	.byte 0x49
	.byte \state
	.byte \case
	.endm

	@ Help macros for uses of moveend commands

	@ All cases
	.macro prefaintmoveendall
	setbyte sPREFAINT_MOVEEND_STATE, 0
	prefaintmoveend MOVEEND_ALL
	.endm
	
	.macro moveendall
	setbyte sMOVEEND_STATE, 0
	moveend MOVEEND_ALL, 0
	.endm

	@ Chosen case
	.macro moveendcase case:req
	setbyte sMOVEEND_STATE, \case
	moveend MOVEEND_CASE, 0
	.endm

	@ All cases from (inclusive)
	.macro moveendfrom from:req
	setbyte sMOVEEND_STATE, \from
	moveend MOVEEND_ALL, 0
	.endm

	@ All cases from 0 to (not inclusive)
	.macro moveendto to:req
	setbyte sMOVEEND_STATE, 0
	moveend MOVEEND_TO, \to
	.endm

	@ Cases from (inclusive) to (not inclusive)
	.macro moveendfromto from:req, to:req
	setbyte sMOVEEND_STATE, \from
	moveend MOVEEND_TO, \to
	.endm
	
	@ Cases when on a future attack hit
	.macro prefaintmoveendfutureattack
	setbyte sPREFAINT_MOVEEND_STATE, 0
	prefaintmoveend MOVEEND_FUTURE_ATTACK
	.endm
	
	.macro moveendfutureattack
	setbyte sMOVEEND_STATE, 0
	moveend MOVEEND_FUTURE_ATTACK, 0
	.endm
	
	.macro returntoball battler:req
	.byte 0x4a
	.byte \battler
	.endm

	.macro returnatktoball
	.byte 0x4b
	.endm

	.macro getswitchedmondata battler:req
	.byte 0x4c
	.byte \battler
	.endm

	.macro switchindataupdate battler:req
	.byte 0x4d
	.byte \battler
	.endm

	.macro switchinanim battler:req, dontclearsubstitutebit:req
	.byte 0x4e
	.byte \battler
	.byte \dontclearsubstitutebit
	.endm

	.macro jumpifcantswitch battler:req, ptr:req
	.byte 0x4f
	.byte \battler
	.4byte \ptr
	.endm

	.macro openpartyscreen param0:req, param1:req
	.byte 0x50
	.byte \param0
	.4byte \param1
	.endm

	.macro switchhandleorder battler:req, caseId:req
	.byte 0x51
	.byte \battler
	.byte \caseId
	.endm

	.macro switchineffectsstate battler:req
	.byte 0x52
	.byte \battler
	.endm

	.macro switchoutabilities battler:req
	.byte 0x53
	.byte \battler
	.endm

	.macro playse songId:req
	.byte 0x54
	.2byte \songId
	.endm

	.macro fanfare songId:req
	.byte 0x55
	.2byte \songId
	.endm

	.macro formchange battler:req, basespecies:req, targetspecies:req, reloadtypes:req, reloadstats:req, reloadability:req, failPtr:req
	.byte 0x56
	.byte \battler
	.2byte \basespecies
	.2byte \targetspecies
	.byte \reloadtypes
	.byte \reloadstats
	.byte \reloadability
	.4byte \failPtr
	.endm

	.macro playanimation battler:req, animId:req, animParamPtr=NULL
	.byte 0x57
	.byte \battler
	.byte \animId
	.4byte \animParamPtr
	.endm
	
	.macro playanimation2 battler:req, animIdPtr:req, animParamPtr=NULL
	.byte 0x58
	.byte \battler
	.4byte \animIdPtr
	.4byte \animParamPtr
	.endm

	.macro handlelearnnewmove learnedMovePtr:req, failPtr:req, isFirstMove:req
	.byte 0x59
	.4byte \learnedMovePtr
	.4byte \failPtr
	.byte \isFirstMove
	.endm

	.macro yesnoboxlearnmove learnedMovePtr:req
	.byte 0x5a
	.4byte \learnedMovePtr
	.endm

	.macro yesnoboxstoplearningmove ptr:req
	.byte 0x5b
	.4byte \ptr
	.endm

	.macro drawlvlupbox
	.byte 0x5c
	.endm

	.macro getmoneyreward ptr:req
	.byte 0x5d
	.4byte \ptr
	.endm

	.macro cureprimarystatus battler:req, ptr:req
	.byte 0x5e
	.byte \battler
	.4byte \ptr
	.endm

	.macro swapattackerwithtarget
	.byte 0x5f
	.endm

	.macro handleabilitypopup battler:req
	.byte 0x60
	.byte \battler
	.endm

	.macro drawpartystatussummary battler:req
	.byte 0x61
	.byte \battler
	.endm

	.macro hidepartystatussummary battler:req
	.byte 0x62
	.byte \battler
	.endm

	.macro jumpifsubstituteblocks ptr:req
	.byte 0x63
	.4byte \ptr
	.endm
	
	.macro setstatchanger statId:req, buff:req
	.byte 0x64
	.byte \statId
	.byte \buff
	.endm

	.macro statbuffchange flags=0, failPtr=NULL
	.byte 0x65
	.byte \flags
	.4byte \failPtr
	.endm
	
	.macro playstatchangeanimation bits=0, flags=0
	.byte 0x66
	.4byte \bits
	.byte \flags
	.endm

	.macro displaystatchangestring stringId=0
	.byte 0x67
	.2byte \stringId
	.endm

	.macro applyqueuedstatboosts battler:req, boostscript:req
	.byte 0x68
	.byte \battler
	.4byte \boostscript
	.endm

	.macro yesnobox
	.byte 0x69
	.endm

	.macro consumeitem battler:req, flags=0
	.byte 0x6a
	.byte \battler
	.byte \flags
	.endm

	.macro handletrainerslidecase battler:req, caseId:req
	.byte 0x6b
	.byte \battler
	.byte \caseId
	.endm
	
	.macro callfunction ptr:req
	.byte 0x6c
	.4byte \ptr
	.endm

	.macro givepaydaymoney
	.byte 0x6d
	.endm

	.macro updatestatusicon battler:req
	.byte 0x6e
	.byte \battler
	.endm

	.macro makevisible battler:req
	.byte 0x6f
	.byte \battler
	.endm

	.macro finishaction
	.byte 0x70
	.endm

	.macro finishturn
	.byte 0x71
	.endm

	.macro jumpifplayerran ptr:req
	.byte 0x72
	.4byte \ptr
	.endm

	.macro hpthresholds battler:req
	.byte 0x73
	.byte \battler
	.endm

	.macro hpthresholds2 battler:req
	.byte 0x74
	.byte \battler
	.endm

	.macro setsplitswap
	.byte 0x75
	.endm

	.macro various battler:req, id:req
	.byte 0x76
	.byte \battler
	.byte \id
	.endm

	.macro setprotectlike
	.byte 0x77
	.endm

	.macro instanthpdrop
	.byte 0x78
	.endm

	.macro setatkhptozero
	.byte 0x79
	.endm

	.macro lunardancepprestore battler:req
	.byte 0x7a
	.byte \battler
	.endm

	.macro tryhealhalfhealth battler:req, failPtr:req
	.byte 0x7b
	.byte \battler
	.4byte \failPtr
	.endm

	.macro trymirrormove
	.byte 0x7c
	.endm

	.macro tryacupressure failStr:req
	.byte 0x7d
	.4byte \failStr
	.endm

	.macro trysetreflect failPtr:req
	.byte 0x7e
	.4byte \failPtr
	.endm

	.macro setseeded
	.byte 0x7f
	.endm

	.macro manipulatedamage caseId:req
	.byte 0x80
	.byte \caseId
	.endm

	.macro trysetrest failPtr:req
	.byte 0x81
	.4byte \failPtr
	.endm

	.macro settypetoterrain ptr:req
	.byte 0x82
	.4byte \ptr
	.endm

	.macro tryrecycleitem ptr:req
	.byte 0x83
	.4byte \ptr
	.endm

	.macro setfieldsport ptr:req
	.byte 0x84
	.4byte \ptr
	.endm

	.macro stockpile caseId:req, ptr=NULL
	.byte 0x85
	.byte \caseId
	.4byte \ptr
	.endm

	.macro stockpiletobasedamage failPtr:req
	.byte 0x86
	.4byte \failPtr
	.endm

	.macro stockpiletohpheal failPtr:req
	.byte 0x87
	.4byte \failPtr
	.endm

	.macro pickup
	.byte 0x88
	.endm

	.macro trysetsnatch ptr:req
	.byte 0x89
	.4byte \ptr
	.endm

	.macro trysetmagiccoat ptr:req
	.byte 0x8a
	.4byte \ptr
	.endm

	.macro assistattackselect ptr:req
	.byte 0x8b
	.4byte \ptr
	.endm

	.macro nop8c
	.byte 0x8c
	.endm

	.macro nop8d
	.byte 0x8d
	.endm

	.macro nop8e
	.byte 0x8e
	.endm

	.macro forcerandomswitch failPtr:req
	.byte 0x8f
	.4byte \failPtr
	.endm

	.macro nop90
	.byte 0x90
	.endm

	.macro nop91
	.byte 0x91
	.endm

	.macro trysetlightscreen failPtr:req
	.byte 0x92
	.4byte \failPtr
	.endm

	.macro tryKO failPtr:req
	.byte 0x93
	.4byte \failPtr
	.endm

	.macro nop94
	.byte 0x94
	.endm

	.macro setweather
	.byte 0x95
	.endm

	.macro weatherdamage
	.byte 0x96
	.endm

	.macro tryinfatuating battler:req
	.byte 0x97
	.byte \battler
	.endm

	.macro nop98
	.byte 0x98
	.endm

	.macro nop99
	.byte 0x99
	.endm

	.macro nop9a
	.byte 0x9a
	.endm

	.macro nop9b
	.byte 0x9b
	.endm

	.macro setsubstitute
	.byte 0x9c
	.endm

	.macro mimicattackcopy failPtr:req
	.byte 0x9d
	.4byte \failPtr
	.endm

	.macro nop9e
	.byte 0x9e
	.endm

	.macro nop9f
	.byte 0x9f
	.endm

	.macro nopa0
	.byte 0xa0
	.endm

	.macro trycounterattack failPtr:req
	.byte 0xa1
	.4byte \failPtr
	.endm

	.macro nopa2
	.byte 0xa2
	.endm

	.macro disablelastusedattack battler:req, failPtr:req
	.byte 0xa3
	.byte \battler
	.4byte \failPtr
	.endm

	.macro trysetencore failPtr:req
	.byte 0xa4
	.4byte \failPtr
	.endm

	.macro painsplitdmgcalc failPtr:req
	.byte 0xa5
	.4byte \failPtr
	.endm

	.macro settypetorandomresistance failPtr:req
	.byte 0xa6
	.4byte \failPtr
	.endm

	.macro trysetalwayshitflag ptr:req
	.byte 0xa7
	.4byte \ptr
	.endm

	.macro copymovepermanently ptr:req
	.byte 0xa8
	.4byte \ptr
	.endm

	.macro trychoosesleeptalkmove ptr:req
	.byte 0xa9
	.4byte \ptr
	.endm

	.macro nopaa
	.byte 0xaa
	.endm

	.macro nopab
	.byte 0xab
	.endm

	.macro nopac
	.byte 0xac
	.endm

	.macro tryspiteppreduce ptr:req
	.byte 0xad
	.4byte \ptr
	.endm

	.macro healpartystatus caseId:req
	.byte 0xae
	.byte \caseId
	.endm

	.macro nopaf
	.byte 0xaf
	.endm

	.macro trysetspikes ptr:req
	.byte 0xb0
	.4byte \ptr
	.endm

	.macro nopb1
	.byte 0xb1
	.endm

	.macro trysetperishsong ptr:req
	.byte 0xb2
	.4byte \ptr
	.endm

	.macro handlerollout
	.byte 0xb3
	.endm

	.macro nopb4
	.byte 0xb4
	.endm

	.macro handlefurycutter
	.byte 0xb5
	.endm

	.macro nopb6
	.byte 0xb6
	.endm

	.macro presentcalc
	.byte 0xb7
	.endm

	.macro trysetsafeguard ptr:req
	.byte 0xb8
	.4byte \ptr
	.endm

	.macro magnitudedamagecalculation
	.byte 0xb9
	.endm

	.macro jumpifnopursuitswitchdmg ptr:req
	.byte 0xba
	.4byte \ptr
	.endm

	.macro nopbb
	.byte 0xbb
	.endm

	.macro maxattackhalvehp failPtr:req
	.byte 0xbc
	.4byte \failPtr
	.endm

	.macro nopbd
	.byte 0xbd
	.endm

	.macro rapidspinfree battler:req
	.byte 0xbe
	.byte \battler
	.endm

	.macro nopbf
	.byte 0xbf
	.endm

	.macro recoverbasedonweather failPtr:req
	.byte 0xc0
	.4byte \failPtr
	.endm

	.macro nopc1
	.byte 0xc1
	.endm

	.macro nopc2
	.byte 0xc2
	.endm

	.macro trysetfutureattack failPtr:req
	.byte 0xc3
	.4byte \failPtr
	.endm

	.macro nopc4
	.byte 0xc4
	.endm

	.macro setsemiinvulnerablebit clear=FALSE
	.byte 0xc5
	.byte \clear
	.endm

	.macro nopc6
	.byte 0xc6
	.endm

	.macro nopc7
	.byte 0xc7
	.endm

	.macro nopc8
	.byte 0xc8
	.endm

	.macro nopc9
	.byte 0xc9
	.endm

	.macro nopca
	.byte 0xca
	.endm

	.macro nopcb
	.byte 0xcb
	.endm

	.macro nopcc
	.byte 0xcc
	.endm

	.macro nopcd
	.byte 0xcd
	.endm

	.macro nopce
	.byte 0xce
	.endm

	.macro nopcf
	.byte 0xcf
	.endm

	.macro settaunt failPtr:req
	.byte 0xd0
	.4byte \failPtr
	.endm

	.macro nopd1
	.byte 0xd1
	.endm

	.macro tryswapitems param0:req
	.byte 0xd2
	.4byte \param0
	.endm

	.macro copyability attacker:req, target:req
	.byte 0xd3
	.byte \attacker
	.byte \target
	.endm

	.macro trywish caseId:req, failPtr:req
	.byte 0xd4
	.byte \caseId
	.4byte \failPtr
	.endm

	.macro nopd5
	.byte 0xd5
	.endm

	.macro nopd6
	.byte 0xd6
	.endm

	.macro setyawn failPtr:req
	.byte 0xd7
	.4byte \failPtr
	.endm

	.macro nopd8
	.byte 0xd8
	.endm

	.macro nopd9
	.byte 0xd9
	.endm

	.macro swapabilities
	.byte 0xda
	.endm

	.macro nopdb
	.byte 0xdb
	.endm

	.macro nopdc
	.byte 0xdc
	.endm

	.macro mefirstattackselect ptr:req
	.byte 0xdd
	.4byte \ptr
	.endm

	.macro nopde
	.byte 0xde
	.endm

	.macro nopdf
	.byte 0xdf
	.endm

	.macro nope0
	.byte 0xe0
	.endm

	.macro nope1
	.byte 0xe1
	.endm

	.macro nope2
	.byte 0xe2
	.endm

	.macro nope3
	.byte 0xe3
	.endm

	.macro nope4
	.byte 0xe4
	.endm

	.macro nope5
	.byte 0xe5
	.endm

	.macro nope6
	.byte 0xe6
	.endm

	.macro nope7
	.byte 0xe7
	.endm

	.macro nope8
	.byte 0xe8
	.endm

	.macro nope9
	.byte 0xe9
	.endm

	.macro nopea
	.byte 0xea
	.endm

	.macro nopeb
	.byte 0xeb
	.endm

	.macro nopec
	.byte 0xec
	.endm

	.macro noped
	.byte 0xed
	.endm

	.macro nopee
	.byte 0xee
	.endm

	.macro handleballthrow
	.byte 0xef
	.endm

	.macro givecaughtmon
	.byte 0xf0
	.endm

	.macro trysetcaughtmondexflags ptr:req
	.byte 0xf1
	.4byte \ptr
	.endm

	.macro displaydexinfo
	.byte 0xf2
	.endm

	.macro trygivecaughtmonnick ptr:req
	.byte 0xf3
	.4byte \ptr
	.endm

	.macro nopf4
	.byte 0xf4
	.endm

	.macro nopf5
	.byte 0xf5
	.endm

	.macro nopf6
	.byte 0xf6
	.endm

	.macro nopf7
	.byte 0xf7
	.endm

    .macro nopf8
	.byte 0xf8
	.endm
	 
	.macro nopf9
	.byte 0xf9
	.endm
	 
	.macro nopfa
	.byte 0xfa 
	.endm
	 
    .macro nopfb
	.byte 0xfb
	.endm
	
	.macro nopfc
	.byte 0xfc
	.endm
	
	.macro nopfd
	.byte 0xfd
	.endm
	
	.macro nopfe
	.byte 0xfe
	.endm
	
@ various command changed to more readable macros
	.macro resetintimidatetracebits battler:req
	various \battler, VARIOUS_RESET_INTIMIDATE_TRACE_BITS
	.endm

	.macro resetplayerfainted
	various BS_ATTACKER, VARIOUS_RESET_PLAYER_FAINTED_FLAG
	.endm

	.macro checkpokeflute battler:req
	various \battler, VARIOUS_CHECK_POKEFLUTE
	.endm

	.macro waitfanfare battler:req
	various \battler, VARIOUS_WAIT_FANFARE
	.endm
	
	.macro trydolastmontrainerslide battler:req
	various \battler, VARIOUS_TRY_LAST_MON_TRAINER_SLIDE
    .endm

    .macro updatehealthboxattribute battler:req, attributeId:req
	various \battler, VARIOUS_UPDATE_HEALTHBOX_ATTRIBUTE
	.byte \attributeId
	.endm
	
	.macro tryremoveillusion battler:req
	various \battler, VARIOUS_TRY_REMOVE_ILLUSION
	.endm
	
	.macro playmoveanimation battler:req, move:req
	various \battler, VARIOUS_PLAY_MOVE_ANIMATION
	.2byte \move
	.endm
	
	.macro jumpiftargetally ptr:req
	various BS_ATTACKER, VARIOUS_JUMP_IF_TARGET_ALLY
	.4byte \ptr
	.endm

    .macro jumpifbattleend ptr:req
	various BS_ATTACKER, VARIOUS_JUMP_IF_BATTLE_END
	.4byte \ptr
	.endm
	
	.macro tryremoveprimalweather battler:req
	various \battler, VARIOUS_TRY_REMOVE_PRIMAL_WEATHER
	flushmessagebox
	.endm
	
	.macro jumpifabsent battler:req, ptr:req
	various \battler, VARIOUS_JUMP_IF_NOT_ALIVE
	.4byte \ptr
	.endm

    .macro tryfrisktarget ptr:req
	various BS_TARGET, VARIOUS_TRY_FRISK_TARGET
    .4byte \ptr
    .endm

    .macro switchinabilities battler:req
	various \battler, VARIOUS_SWITCHIN_ABILITIES
	.endm

    .macro tryendneutralizinggas battler:req
	various \battler, VARIOUS_TRY_END_NEUTRALIZING_GAS
	.endm

    .macro setteleportoutcome battler:req
	various \battler, VARIOUS_SET_TELEPORT_OUTCOME
	.endm
	
	.macro tryfaintonspikesdamage battler:req, ptr:req
	various \battler, VARIOUS_TRY_FAINT_ON_SPIKES_DAMAGE
	.4byte \ptr
	.endm
	
	.macro activateweatherchangeabilities battler:req
	various \battler, VARIOUS_ACTIVATE_WEATHER_ABILITIES
	.endm
	
	.macro tryneutralizinggassuppression battler:req, ptr:req
	various \battler, VARIOUS_TRY_NEUTRALIZING_GAS_SUPPRESSION
	.4byte \ptr
	.endm
	
	.macro jumpifrevived ptr:req
	various BS_ATTACKER, VARIOUS_JUMP_IF_BATTLER_REVIVED
	.4byte \ptr
	.endm
	
	.macro jumpifweatheraffected battler:req, weatherFlags:req, ptr:req
	various \battler, VARIOUS_JUMP_IF_WEATHER_AFFECTED
	.2byte \weatherFlags
	.4byte \ptr
	.endm
	
	.macro jumpifside battler:req, side:req, ptr:req
	various \battler, VARIOUS_GET_BATTLER_SIDE
	jumpifbyte CMP_EQUAL, sMULTIUSE_STATE, \side, \ptr
	.endm
	
	.macro jumpifparentalbondcounter counter:req, ptr:req
	various BS_ATTACKER, VARIOUS_JUMP_IF_PARENTAL_BOND_COUNTER
	.byte \counter
	.4byte \ptr
	.endm
	
	.macro jumpifnotfirstturn ptr:req
	various BS_ATTACKER, VARIOUS_JUMP_IF_NOT_FIRST_TURN
	.4byte \ptr
	.endm
	
	.macro tryconversiontypechange ptr:req
	various BS_ATTACKER, VARIOUS_TRY_CONVERSION_TYPE_CHANGE
	.4byte \ptr
	.endm
	
	.macro trysetdestinybond ptr:req
	various BS_ATTACKER, VARIOUS_TRY_SET_DESTINY_BOND
	.4byte \ptr
	.endm
	
	.macro trycopyability ptr:req
	various BS_ATTACKER, VARIOUS_TRY_COPY_ABILITY
	.4byte \ptr
	copyability BS_ATTACKER, BS_TARGET
	.endm
	
	.macro jumpifnodamage ptr:req
	various BS_ATTACKER, VARIOUS_JUMP_IF_NO_DAMAGE
	.4byte \ptr
	.endm
	
	.macro tryswapabilities ptr:req
	various BS_ATTACKER, VARIOUS_TRY_SWAP_ABILITIES
	.4byte \ptr
	swapabilities
	.endm
	
	.macro jumpifdefiantactivate ptr:req
	various BS_EFFECT_BATTLER, VARIOUS_JUMP_IF_DEFIANT_ACTIVATE
	.4byte \ptr
	.endm
	
	.macro jumpifconfusedandstatmaxed battler:req, statId:req, ptr:req
	various \battler, VARIOUS_JUMP_IF_CONFUSED_AND_STAT_MAXED
	.byte \statId
	.4byte \ptr
	.endm
	
	.macro tryactivaterattledonintimidate ptr:req
	various BS_TARGET, VARIOUS_TRY_RATTLED_ON_INTIMIDATE
	.4byte \ptr
	.endm
	
	.macro tryactivatesapsipper battler:req, ptr:req
	various \battler, VARIOUS_TRY_ACTIVATE_SAP_SIPPER
	.4byte \ptr
	.endm
	
	.macro trysetgravity ptr:req
	various BS_ATTACKER, VARIOUS_TRY_SET_GRAVITY
	.4byte \ptr
	.endm
	
	.macro trybringdowninairbattler battler:req, ptr:req
	various \battler, VARIOUS_TRY_BRING_DOWN_IN_AIR
	.4byte \ptr
	.endm
	
	.macro tryactivatewindabilities battler:req
	various \battler, VARIOUS_TRY_ACTIVATE_WIND_ABILITIES
	.endm
	
	.macro jumpifemergencyexited battler:req, ptr:req
	various \battler, VARIOUS_JUMP_IF_EMERGENCY_EXITED
	.4byte \ptr
	.endm
	
	.macro trystatustransfer ptr:req
	various BS_ATTACKER, VARIOUS_TRY_STATUS_TRANSFER
	.4byte \ptr
	.endm
	
	.macro tryabilitysuppression battler:req, ptr:req
	various \battler, VARIOUS_TRY_ABILITY_SUPPRESSION
	.4byte \ptr
	.endm
	
@ callfunction macros
    .macro setminimize
	callfunction BS_SetMinimize
	.endm
	
	.macro movevaluescleanup
	callfunction BS_MoveValuesCleanUp
	.endm
	
	.macro itemincreasestat
	callfunction BS_ItemIncreaseStat
	.endm
	
	.macro incrementgamestat gameStatId:req
	callfunction BS_IncrementGameStat
	.byte \gameStatId
	.endm
	
	.macro spriteignore0hp
	callfunction BS_SwitchSpriteIgnore0HpBit
	.endm
	
	.macro atknameinbuff1
	callfunction BS_AtkNameInBuff1
	.endm
	
	.macro resetsentmonsvalue
	callfunction BS_ResetSentMonsValue
	.endm
	
	.macro setatktoplayer0
	callfunction BS_SetAtkToPlayer0
	.endm
	
	.macro buffermovetolearn
	callfunction BS_BufferMoveToLearn
	.endm
	
	.macro trysetdestinybondtohappen
	callfunction BS_TrySetDestinyBondToHappen
	.endm
	
	.macro normaliseallbuffs
	callfunction BS_NormaliseAllBuffs
	.endm
	
	.macro setchargingturn
	callfunction BS_SetChargingTurn
	.endm
	
	.macro clearchargingturn
	callfunction BS_ClearChargingTurn
	.endm
	
	.macro setescapeprevention
	callfunction BS_SetEscapePrevention
	.endm
	
	.macro setforcedtarget
	callfunction BS_SetForcedTarget
	.endm
	
	.macro callterrainattack
	callfunction BS_CallTerrainAttack
	.endm
	
	.macro gettwotunsmovestring
	callFunction BS_GetTwoTurnsMoveString
	.endm
	
	.macro setmagiccoattarget
	callfunction BS_SetMagicCoatTarget
	.endm
	
	.macro cancelallactions
	callfunction BS_CancelAllActions
	.endm
	
	.macro setbide
	callfunction BS_SetBide
	.endm
	
	.macro setmist
	callfunction BS_SetMist
	.endm
	
	.macro metronome
	callfunction BS_Metronome
	.endm
	
	.macro psywavedamageeffect
	callfunction BS_PsywaveDamageEffect
	.endm
	
	.macro copyfoestats
	callfunction BS_CopyFoeStats
	.endm
	
	.macro setcharge battler:req
	callfunction BS_SetCharge
	.byte \battler
	.endm
	
	.macro removescreens
	callfunction BS_RemoveScreens
	.endm
	
	.macro endselectionscript
	callfunction BS_EndSelectionScript
	.endm
	
	.macro snatchsetbattlers
	callfunction BS_SnatchSetBattlers
	.endm
	
	.macro tryrestoremirrorarmororiginalattacker
	callfunction BS_TryRestoreMirrorArmorOriginalAttacker
	.endm
	
	.macro argumenttomovedamage
	callfunction BS_ArgumentToMoveDamage
	.endm
	
	.macro setstatchangerfrommultiusestate buff:req
	callfunction BS_SetStatChangerFromMultiuseState
	.byte \buff
	.endm
	
	.macro settargetally
	callfunction BS_SetTargetAlly
	.endm
	
	.macro sethelpinghand
	callfunction BS_SetHelpingHand
	.endm
	
	.macro transformdataexecution
	callfunction BS_TransformDataExecution
	.endm
	
	.macro setroost
	callfunction BS_SetRoost
	.endm
	
	.macro sethealingwish
	callfunction BS_SetHealingWish
	.endm
	
	.macro trysettailwind ptr:req
	callfunction BS_TrySetTailwind
	.4byte \ptr
	.endm
	
	.macro trysetembargo ptr:req
	callfunction BS_TrySetEmbargo
	.4byte \ptr
	.endm
	
	.macro trysethealblock ptr:req
	callfunction BS_TrySetHealBlock
	.4byte \ptr
	.endm
	
	.macro trysetluckychant ptr:req
	callfunction BS_TrySetLuckyChant
	.4byte \ptr
	.endm
	
	.macro switchoutpartyorderswap
	callfunction BS_SwitchOutPartyOrderSwap
	.endm
	
	.macro trycopycat ptr:req
	callfunction BS_TryCopycat
	.4byte \ptr
	.endm
	
	.macro trylastresort ptr:req
	callfunction BS_TryLastResort
	.4byte \ptr
	.endm
	
	.macro trysetability ptr:req
	callfunction BS_TrySetAbility
	.4byte \ptr
	.endm
	
	.macro trysuckerpunch ptr:req
	callfunction BS_TrySuckerPunch
	.4byte \ptr
	.endm
	
	.macro trysettoxicspikes ptr:req
	callfunction BS_TrySetToxicSpikes
	.4byte \ptr
	.endm
	
	.macro getfutureattackanim
	callfunction BS_GetFutureAttackAnim
	.endm
	
	.macro updaterecoilevolutiontracker
	callfunction BS_UpdateRecoilEvolutionTracker
	.endm
	
	.macro incrementtriplekickpower
	callfunction BS_IncrementTripleKickPower
	.endm
	
	.macro switchpursuitdmgbit
	callfunction BS_SwitchPursuitDamageBit
	.endm
	
	.macro jumpifsosopponentfainted battler:req, ptr:req
	callfunction BS_JumpIfSosOpponentFainted
	.byte \battler
	.4byte \ptr
	.endm
	
	.macro soscallforally
	setbyte sMULTIUSE_STATE, 0
	callfunction BS_SosCallForAlly
	.endm
	
	.macro trysetadrenalineorbeffect
	callfunction BS_TrySetAdrenalineOrbEffect
	.endm
	
	.macro tryteleport battler:req
	callfunction BS_TryTeleport
	.byte \battler
	.endm
	
	.macro playcry battler:req
	callfunction BS_PlayBattlerCry
	.byte \battler
	.endm
	
	.macro jumptocalledmove
	callfunction BS_JumpToCalledMove
	.endm
	
	.macro updatechoicemoveonlvlup
	callfunction BS_UpdateChoiceMoveOnLvlUp
	.endm
	
	.macro jumpifcantgivenickname ptr:req
	callfunction BS_JumpIfCantGiveNick
	.4byte \ptr
	.endm
	
	.macro jumpifendeavorfail ptr:req
	callfunction BS_JumpIfEndeavorFail
	.4byte \ptr
	.endm
	
	.macro trysetmagnetrise failPtr:req
	callfunction BS_TrySetMagnetRise
	.4byte \failPtr
	.endm
	
@ helpful macros
	.macro copybyte dst:req, src:req
	copyarray \dst, \src, 0x1
	.endm

	.macro copyhword dst:req, src:req
	copyarray \dst, \src, 0x2
	.endm

	.macro copyword dst:req, src:req
	copyarray \dst, \src, 0x4
	.endm

	.macro jumpifbytenotequal byte1:req, byte2:req, jumpptr:req
	jumpifarraynotequal \byte1, \byte2, 0x1, \jumpptr
	.endm

	.macro jumpifbyteequal byte1:req, byte2:req, jumpptr:req
	jumpifarrayequal \byte1, \byte2, 0x1, \jumpptr
	.endm

	.macro jumpifmove move:req, jumpptr:req
	jumpifhalfword CMP_EQUAL, gCurrentMove, \move, \jumpptr
	.endm

	.macro jumpifnotmove move:req, jumpptr:req
	jumpifhalfword CMP_NOT_EQUAL, gCurrentMove, \move, \jumpptr
	.endm

    .macro jumpiftype battler:req, type:req, jumpptr:req
	jumpbasedontype \battler, \type, TRUE, \jumpptr
	.endm
	
	.macro jumpifnotype battler:req, type:req, jumpptr:req
	jumpbasedontype \battler, \type, FALSE, \jumpptr
	.endm
	
	.macro jumpifstatus battler:req, statusId:req, ptr:req
	jumpbasedonstatus1 \battler, CMP_EQUAL, \statusId, \ptr
	.endm
	
	.macro jumpifnostatus battler:req, statusId:req, ptr:req
	jumpbasedonstatus1 \battler, CMP_NOT_EQUAL, \statusId, \ptr
	.endm
	
	.macro jumpifstatus2 battler:req, status2:req, ptr:req
	jumpbasedonstatus \battler, ID_STATUS2, CMP_COMMON_BITS, \status2, \ptr
	.endm
	
	.macro jumpifnostatus2 battler:req, status2:req, ptr:req
	jumpbasedonstatus \battler, ID_STATUS2, CMP_NO_COMMON_BITS, \status2, \ptr
	.endm
	
	.macro jumpifstatus3 battler:req, status3:req, ptr:req
	jumpbasedonstatus \battler, ID_STATUS3, CMP_COMMON_BITS, \status3, \ptr
	.endm

	.macro jumpifnostatus3 battler:req, status3:req, ptr:req
	jumpbasedonstatus \battler, ID_STATUS3, CMP_NO_COMMON_BITS, \status3, \ptr
	.endm
	
	.macro jumpifmovehadnoeffect jumpptr:req
	jumpifhalfword CMP_COMMON_BITS, gMoveResultFlags, MOVE_RESULT_NO_EFFECT, \jumpptr
	.endm

	.macro jumpifbattletype flags:req, jumpptr:req
	jumpifword CMP_COMMON_BITS, gBattleTypeFlags, \flags, \jumpptr
	.endm

	.macro jumpifnotbattletype flags:req, jumpptr:req
	jumpifword CMP_NO_COMMON_BITS, gBattleTypeFlags, \flags, \jumpptr
	.endm
	
	.macro seteffectprimary flags=0
	setspecialmoveeffect TRUE, \flags
	.endm
	
	.macro seteffectsecondary flags=0
	setspecialmoveeffect FALSE, \flags
	.endm
	
    .macro loadabilitypopup battler:req
	handleabilitypopup \battler
	.endm

    .macro removeabilitypopup battler:req
	handleabilitypopup ATK60_REMOVE_POP_UP | \battler
	.endm
	
	.macro updateabilitypopup battler:req
	handleabilitypopup ATK60_UPDATE_POP_UP | \battler
	.endm
	
	.macro abilitysetpopup battler:req, stringId:req
	loadabilitypopup \battler
	pause B_WAIT_TIME_SHORT
	updateabilitypopup \battler
	printstring \stringId
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup \battler
	.endm
	
	.macro abilitycopypopup attacker:req, defender:req, stringId:req
	loadabilitypopup \attacker
	pause B_WAIT_TIME_SHORT
	loadabilitypopup \defender
	pause B_WAIT_TIME_SHORT
	updateabilitypopup \attacker
	printstring \stringId
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup \defender
	pause B_WAIT_TIME_SHORT
	removeabilitypopup \attacker
	.endm
	
	.macro abilityswappopup attacker:req, defender:req, stringId:req
	loadabilitypopup \attacker
	pause B_WAIT_TIME_SHORT
	loadabilitypopup \defender
	pause B_WAIT_TIME_SHORT
	updateabilitypopup \attacker
	updateabilitypopup \defender
	printstring \stringId
	waitmessage B_WAIT_TIME_LONG
	removeabilitypopup \defender
	pause B_WAIT_TIME_SHORT
	removeabilitypopup \attacker
	.endm
	
	.macro instantfaintattacker
	instanthpdrop
	setatkhptozero
	.endm
	
	.macro tryendeffectonabilitychange battler:req
	tryendneutralizinggas \battler @ switch in abilities still show it's pop ups
	setbyte sBYPASS_ABILITY_POP_UP, TRUE @ don't show ability pop up. I'm not sure if it's show or not, but added this bc yes...
	tryremoveprimalweather \battler
	tryremoveillusion \battler
	activateweatherchangeabilities \battler
	setbyte sBYPASS_ABILITY_POP_UP, FALSE
	.endm
	
	.macro flushmessagebox
	printstring STRINGID_EMPTYSTRING
	waitmessage 1
	.endm
	
	.macro clearsemiinvulnerablebit
	setsemiinvulnerablebit TRUE
	.endm
	
	.macro damageformula
	critcalc
	damagecalc
	typecalc
	adjustdamage
	.endm
	
	.macro statchangeanimandstring bits=0, flags=0, stringId=0
	playstatchangeanimation \bits, \flags
	waitstate
	displaystatchangestring \stringId
	.endm
	
	.macro switchineffects battler:req
	setbyte sSWITCHIN_EFFECTS_STATE, 0
	switchineffectsstate \battler
	.endm
	
	.macro savebattlers
	saveattacker
	savetarget
	.endm
	
	.macro restorebattlers
	restoreattacker
	restoretarget
	.endm

@ commands
	.macro attackcanceler
	.byte 0x0
	.endm

	.macro accuracycheck ptr:req, param1=ACC_CURR_MOVE
	.byte 0x1
	.4byte \ptr
	.byte \param1
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

	.macro nop08
	.byte 0x8
	.endm

	.macro attackanimation
	.byte 0x9
	.endm

	.macro nop0a
	.byte 0xa
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

	.macro printstring id:req
	.byte 0x10
	.2byte \id
	.endm

	.macro printselectionstring id:req
	.byte 0x11
	.2byte \id
	.endm

	.macro waitmessage param0:req
	.byte 0x12
	.2byte \param0
	.endm

	.macro printfromtable ptr:req
	.byte 0x13
	.4byte \ptr
	.endm

	.macro printselectionstringfromtable ptr:req
	.byte 0x14
	.4byte \ptr
	.endm
	
	.macro seteffectwithchance
	.byte 0x15
	.endm

	.macro seteffectprimary
	.byte 0x16
	.endm

	.macro seteffectsecondary
	.byte 0x17
	.endm

	.macro clearstatusfromeffect battler:req
	.byte 0x18
	.byte \battler
	.endm

	.macro tryfaintmon battler:req
	.byte 0x19
	.byte \battler
	.endm

	.macro dofaintanimation battler:req
	.byte 0x1a
	.byte \battler
	.endm

	.macro cleareffectsonfaint battler:req
	.byte 0x1b
	.byte \battler
	.endm

	.macro jumpifstatus battler:req, status1:req, ptr:req
	.byte 0x1c
	.byte \battler
	.4byte \status1
	.4byte \ptr
	.endm

	.macro jumpifstatus2 battler:req, status2:req, ptr:req
	.byte 0x1d
	.byte \battler
	.4byte \status2
	.4byte \ptr
	.endm

	.macro jumpifability param0:req, ability:req, ptr:req
	.byte 0x1e
	.byte \param0
	.2byte \ability
	.4byte \ptr
	.endm

	.macro jumpifsideaffecting battler:req, sidestatus:req, ptr:req
	.byte 0x1f
	.byte \battler
	.2byte \sidestatus
	.4byte \ptr
	.endm

	.macro jumpifstat battler:req, ifflag:req, stat:req, value:req, ptr
	.byte 0x20
	.byte \battler
	.byte \ifflag
	.byte \stat
	.byte \value
	.4byte \ptr
	.endm

	.macro jumpifstatus3condition battler:req, status3:req, param2:req, ptr:req
	.byte 0x21
	.byte \battler
	.4byte \status3
	.byte \param2
	.4byte \ptr
	.endm

    .macro jumpbasedontype battler:req, type:req, case:req, ptr:req
    .byte 0x22
	.byte \battler
	.byte \type
	.byte \case
	.4byte \ptr
	.endm

	.macro getexp battler:req
	.byte 0x23
	.byte \battler
	.endm

	.macro checkteamslot ptr:req
	.byte 0x24
	.4byte \ptr
	.endm

	.macro nop25
	.byte 0x25
	.endm

	.macro setmoveeffect effect:req, affectsUser=FALSE, certain=FALSE
	.byte 0x26
	.byte \effect
	.byte \affectsUser
	.byte \certain
	.endm

	.macro nop27
	.byte 0x27
	.endm

	.macro goto ptr:req
	.byte 0x28
	.4byte \ptr
	.endm

	.macro jumpifbyte ifflag:req, param1:req, param2:req, param3:req
	.byte 0x29
	.byte \ifflag
	.4byte \param1
	.byte \param2
	.4byte \param3
	.endm

	.macro jumpifhalfword ifflag:req, param1:req, param2:req, param3:req
	.byte 0x2a
	.byte \ifflag
	.4byte \param1
	.2byte \param2
	.4byte \param3
	.endm

	.macro jumpifword ifflag:req, param1:req, param2:req, param3:req
	.byte 0x2b
	.byte \ifflag
	.4byte \param1
	.4byte \param2
	.4byte \param3
	.endm

	.macro jumpifarrayequal param0:req, param1:req, param2:req, param3:req
	.byte 0x2c
	.4byte \param0
	.4byte \param1
	.byte \param2
	.4byte \param3
	.endm

	.macro jumpifarraynotequal param0:req, param1:req, param2:req, param3:req
	.byte 0x2d
	.4byte \param0
	.4byte \param1
	.byte \param2
	.4byte \param3
	.endm

	.macro setbyte ptr:req, param1:req
	.byte 0x2e
	.4byte \ptr
	.byte \param1
	.endm

	.macro addbyte ptr:req, param1:req
	.byte 0x2f
	.4byte \ptr
	.byte \param1
	.endm

	.macro subbyte ptr:req, param1:req
	.byte 0x30
	.4byte \ptr
	.byte \param1
	.endm

	.macro copyarray param0:req, param1:req, param2:req
	.byte 0x31
	.4byte \param0
	.4byte \param1
	.byte \param2
	.endm
	
	.macro sethword ptr:req, value:req
	.byte 0x32
	.4byte \ptr
	.2byte \value
	.endm
	
	.macro nop32
	.byte 0x32
	.endm

	.macro orbyte ptr:req, param1:req
	.byte 0x33
	.4byte \ptr
	.byte \param1
	.endm

	.macro orhalfword ptr:req, param1:req
	.byte 0x34
	.4byte \ptr
	.2byte \param1
	.endm

	.macro orword ptr:req, param1:req
	.byte 0x35
	.4byte \ptr
	.4byte \param1
	.endm

	.macro bicbyte ptr:req, param1:req
	.byte 0x36
	.4byte \ptr
	.byte \param1
	.endm

	.macro bichalfword ptr:req, param1:req
	.byte 0x37
	.4byte \ptr
	.2byte \param1
	.endm

	.macro bicword ptr:req, param1:req
	.byte 0x38
	.4byte \ptr
	.4byte \param1
	.endm

	.macro pause param0:req
	.byte 0x39
	.2byte \param0
	.endm

	.macro waitstate
	.byte 0x3a
	.endm

	.macro nop3b
	.byte 0x3b
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

	.macro jumpifaffectedbyprotect ptr:req
	.byte 0x40
	.4byte \ptr
	.endm

	.macro call ptr:req
	.byte 0x41
	.4byte \ptr
	.endm

	.macro trysetsleep battler:req, ptr:req
	.byte 0x42
	.byte \battler
	.4byte \ptr
	.endm

	.macro nop43
	.byte 0x43
	.endm

	.macro endselectionscript
	.byte 0x44
	.endm

	.macro playanimation battler:req, param1:req, param2:req
	.byte 0x45
	.byte \battler
	.byte \param1
	.4byte \param2
	.endm

	.macro playanimation2 battler:req, param1:req, param2:req
	.byte 0x46
	.byte \battler
	.4byte \param1
	.4byte \param2
	.endm

	.macro setgraphicalstatchangevalues
	.byte 0x47
	.endm

	.macro playstatchangeanimation battler:req, param1:req, param2:req
	.byte 0x48
	.byte \battler
	.byte \param1
	.byte \param2
	.endm

	.macro moveend param0:req, param1:req
	.byte 0x49
	.byte \param0
	.byte \param1
	.endm

	@ Help macros for 5 uses of moveend command

	@ All cases
	.macro moveendall
	setbyte sMOVEEND_STATE, 0
	moveend ATK49_MOVEEND_ALL, 0
	.endm

	@ Chosen case
	.macro moveendcase case:req
	setbyte sMOVEEND_STATE, \case
	moveend ATK49_MOVEEND_CASE, 0
	.endm

	@ All cases from (inclusive)
	.macro moveendfrom from:req
	setbyte sMOVEEND_STATE, \from
	moveend ATK49_MOVEEND_ALL, 0
	.endm

	@ All cases from 0 to (not inclusive)
	.macro moveendto to:req
	setbyte sMOVEEND_STATE, 0
	moveend ATK49_MOVEEND_TO, \to
	.endm

	@ Cases from (inclusive) to (not inclusive)
	.macro moveendfromto from:req, to:req
	setbyte sMOVEEND_STATE, \from
	moveend ATK49_MOVEEND_TO, \to
	.endm

	.macro formchange battler:req, basespecies:req, targetspecies:req, reloadtypes:req, reloadability:req, ptr:req
	.byte 0x4a
	.byte \battler
	.2byte \basespecies
	.2byte \targetspecies
	.byte \reloadtypes
	.byte \reloadability
	.4byte \ptr
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

	.macro switchhandleorder battler:req, param1:req
	.byte 0x51
	.byte \battler
	.byte \param1
	.endm

	.macro switchineffects battler:req
	.byte 0x52
	.byte \battler
	.endm

	.macro nop53
	.byte 0x53
	.endm

	.macro playse song:req
	.byte 0x54
	.2byte \song
	.endm

	.macro fanfare song:req
	.byte 0x55
	.2byte \song
	.endm

	.macro playfaintcry battler:req
	.byte 0x56
	.byte \battler
	.endm

	.macro endlinkbattle
	.byte 0x57
	.endm

	.macro returntoball battler:req
	.byte 0x58
	.byte \battler
	.endm

	.macro handlelearnnewmove param0:req, param1:req, param2:req
	.byte 0x59
	.4byte \param0
	.4byte \param1
	.byte \param2
	.endm

	.macro yesnoboxlearnmove param0:req
	.byte 0x5a
	.4byte \param0
	.endm

	.macro yesnoboxstoplearningmove param0:req
	.byte 0x5b
	.4byte \param0
	.endm

	.macro nop5c
	.byte 0x5c
	.endm

	.macro getmoneyreward addr
	.byte 0x5d
	.4byte \addr
	.endm

	.macro nop5e
	.byte 0x5e
	.endm

	.macro swapattackerwithtarget
	.byte 0x5f
	.endm

	.macro nop60
	.byte 0x60
	.endm

	.macro drawpartystatussummary battler:req
	.byte 0x61
	.byte \battler
	.endm

	.macro hidepartystatussummary battler:req
	.byte 0x62
	.byte \battler
	.endm

	.macro jumptocalledmove param0:req
	.byte 0x63
	.byte \param0
	.endm
	
	.macro argumenttomoveeffect
	.byte 0x64
	.endm

	.macro argumenttomovedamage
	.byte 0x65
	.endm

	.macro chosenstatusanimation battler:req, param1:req, param2:req
	.byte 0x66
	.byte \battler
	.byte \param1
	.4byte \param2
	.endm

	.macro yesnobox
	.byte 0x67
	.endm

	.macro cancelallactions
	.byte 0x68
	.endm

	.macro nop69
	.byte 0x69
	.endm

	.macro removeitem battler:req
	.byte 0x6a
	.byte \battler
	.endm

	.macro nop6b
	.byte 0x6b
	.endm

	.macro drawlvlupbox
	.byte 0x6c
	.endm

	.macro nop6d
	.byte 0x6d
	.endm

	.macro nop6e
	.byte 0x6e
	.endm

	.macro makevisible battler:req
	.byte 0x6f
	.byte \battler
	.endm

	.macro recordlastability battler:req
	.byte 0x70
	.byte \battler
	.endm

	.macro nop71
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

	.macro nop75
	.byte 0x75
	.endm

	.macro various battler:req, param1:req
	.byte 0x76
	.byte \battler
	.byte \param1
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

	.macro nop7a
	.byte 0x7a
	.endm

	.macro tryhealhalfhealth battler:req, ptr:req
	.byte 0x7b
	.byte \battler
	.4byte \ptr
	.endm

	.macro trymirrormove
	.byte 0x7c
	.endm

	.macro setrain
	.byte 0x7d
	.endm

	.macro setreflect
	.byte 0x7e
	.endm

	.macro setseeded
	.byte 0x7f
	.endm

	.macro manipulatedamage param0:req
	.byte 0x80
	.byte \param0
	.endm

	.macro trysetrest param0:req
	.byte 0x81
	.4byte \param0
	.endm

	.macro nop82
	.byte 0x82
	.endm

	.macro handletrainerslidecase battler:req, caseId:req
	.byte 0x83
	.byte \battler
	.byte \caseId
	.endm

	.macro trysetpoison battler:req
	.byte 0x84
	.byte \battler
	.endm

	.macro stockpile
	.byte 0x85
	.endm

	.macro stockpiletobasedamage param0:req
	.byte 0x86
	.4byte \param0
	.endm

	.macro stockpiletohpheal param0:req
	.byte 0x87
	.4byte \param0
	.endm

	.macro nop88
	.byte 0x88
	.endm

	.macro statbuffchange param0:req, param1:req
	.byte 0x89
	.byte \param0
	.4byte \param1
	.endm

	.macro normalisebuffs
	.byte 0x8a
	.endm

	.macro setbide
	.byte 0x8b
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

	.macro forcerandomswitch param0:req
	.byte 0x8f
	.4byte \param0
	.endm

	.macro tryconversiontypechange param0:req
	.byte 0x90
	.4byte \param0
	.endm

	.macro givepaydaymoney
	.byte 0x91
	.endm

	.macro setlightscreen
	.byte 0x92
	.endm

	.macro tryKO param0:req
	.byte 0x93
	.4byte \param0
	.endm

	.macro nop94
	.byte 0x94
	.endm

	.macro setsandstorm
	.byte 0x95
	.endm

	.macro weatherdamage
	.byte 0x96
	.endm

	.macro tryinfatuating param0:req
	.byte 0x97
	.4byte \param0
	.endm

	.macro updatestatusicon battler:req
	.byte 0x98
	.byte \battler
	.endm

	.macro setmist
	.byte 0x99
	.endm

	.macro setfocusenergy
	.byte 0x9a
	.endm

	.macro transformdataexecution
	.byte 0x9b
	.endm

	.macro setsubstitute
	.byte 0x9c
	.endm

	.macro mimicattackcopy param0:req
	.byte 0x9d
	.4byte \param0
	.endm

	.macro metronome
	.byte 0x9e
	.endm

	.macro nop9f
	.byte 0x9f
	.endm

	.macro psywavedamageeffect
	.byte 0xa0
	.endm

	.macro trycounterattack split:req, ptr:req
	.byte 0xa1
	.byte \split
	.4byte \ptr
	.endm

	.macro nopa2
	.byte 0xa2
	.endm

	.macro disablelastusedattack bank:req, param1:req
	.byte 0xa3
	.byte \bank
	.4byte \param1
	.endm

	.macro trysetencore param0:req
	.byte 0xa4
	.4byte \param0
	.endm

	.macro painsplitdmgcalc param0:req
	.byte 0xa5
	.4byte \param0
	.endm

	.macro settypetorandomresistance param0:req
	.byte 0xa6
	.4byte \param0
	.endm

	.macro setalwayshitflag
	.byte 0xa7
	.endm

	.macro copymovepermanently param0:req
	.byte 0xa8
	.4byte \param0
	.endm

	.macro trychoosesleeptalkmove param0:req
	.byte 0xa9
	.4byte \param0
	.endm

	.macro nopaa
	.byte 0xaa
	.endm

	.macro nopab
	.byte 0xab
	.endm

	.macro trysetburn battler:req
	.byte 0xac
	.byte \battler
	.endm

	.macro tryspiteppreduce param0:req
	.byte 0xad
	.4byte \param0
	.endm

	.macro healpartystatus
	.byte 0xae
	.endm

	.macro cursetarget param0:req
	.byte 0xaf
	.4byte \param0
	.endm

	.macro trysetspikes param0:req
	.byte 0xb0
	.4byte \param0
	.endm

	.macro setforesight
	.byte 0xb1
	.endm

	.macro trysetperishsong param0:req
	.byte 0xb2
	.4byte \param0
	.endm

	.macro handlerollout
	.byte 0xb3
	.endm

	.macro jumpifconfusedandstatmaxed stat:req, ptr:req
	.byte 0xb4
	.byte \stat
	.4byte \ptr
	.endm

	.macro handlefurycutter
	.byte 0xb5
	.endm

	.macro trysetparalyze battler:req
	.byte 0xb6
	.byte \battler
	.endm

	.macro presentcalc
	.byte 0xb7
	.endm

	.macro setsafeguard
	.byte 0xb8
	.endm

	.macro magnitudedamagecalculation
	.byte 0xb9
	.endm

	.macro jumpifnopursuitswitchdmg param0:req
	.byte 0xba
	.4byte \param0
	.endm

	.macro setsunny
	.byte 0xbb
	.endm

	.macro maxattackhalvehp param0:req
	.byte 0xbc
	.4byte \param0
	.endm

	.macro copyfoestats
	.byte 0xbd
	.endm

	.macro rapidspinfree
	.byte 0xbe
	.endm

	.macro nopbf
	.byte 0xbf
	.endm

	.macro recoverbasedonsunlight param0:req
	.byte 0xc0
	.4byte \param0
	.endm

	.macro nopc1
	.byte 0xc1
	.endm

	.macro selectfirstvalidtarget
	.byte 0xc2
	.endm

	.macro trysetfutureattack param0:req
	.byte 0xc3
	.4byte \param0
	.endm

	.macro trydobeatup param0:req, param1:req
	.byte 0xc4
	.4byte \param0
	.4byte \param1
	.endm

	.macro setsemiinvulnerablebit
	.byte 0xc5
	.endm

	.macro clearsemiinvulnerablebit
	.byte 0xc6
	.endm

	.macro nopc7
	.byte 0xc7
	.endm

	.macro sethail
	.byte 0xc8
	.endm

	.macro jumpifattackandspecialattackcannotfall ptr:req
	.byte 0xc9
	.4byte \ptr
	.endm

	.macro setforcedtarget
	.byte 0xca
	.endm

	.macro setcharge
	.byte 0xcb
	.endm

	.macro callterrainattack
	.byte 0xcc
	.endm

	.macro nopcd
	.byte 0xcd
	.endm

	.macro settorment param0:req
	.byte 0xce
	.4byte \param0
	.endm

	.macro jumpifnodamage param0:req
	.byte 0xcf
	.4byte \param0
	.endm

	.macro settaunt param0:req
	.byte 0xd0
	.4byte \param0
	.endm

	.macro trysethelpinghand param0:req
	.byte 0xd1
	.4byte \param0
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

	.macro trywish param0:req, param1:req
	.byte 0xd4
	.byte \param0
	.4byte \param1
	.endm

	.macro trysetroots param0:req
	.byte 0xd5
	.4byte \param0
	.endm

	.macro nopd6
	.byte 0xd6
	.endm

	.macro setyawn param0:req
	.byte 0xd7
	.4byte \param0
	.endm

	.macro setdamagetohealthdifference param0:req
	.byte 0xd8
	.4byte \param0
	.endm

	.macro nopd9
	.byte 0xd9
	.endm

	.macro swapabilities
	.byte 0xda
	.endm

	.macro tryimprison param0:req
	.byte 0xdb
	.4byte \param0
	.endm

	.macro trysetgrudge param0:req
	.byte 0xdc
	.4byte \param0
	.endm

	.macro nopdd
	.byte 0xdd
	.endm

	.macro assistattackselect param0:req
	.byte 0xde
	.4byte \param0
	.endm

	.macro trysetmagiccoat param0:req
	.byte 0xdf
	.4byte \param0
	.endm

	.macro trysetsnatch param0:req
	.byte 0xe0
	.4byte \param0
	.endm

	.macro nope1
	.byte 0xe1
	.endm

	.macro switchoutabilities battler:req
	.byte 0xe2
	.byte \battler
	.endm

	.macro nope3
	.byte 0xe3
	.endm

	.macro getsecretpowereffect
	.byte 0xe4
	.endm

	.macro pickup
	.byte 0xe5
	.endm

	.macro nope6
	.byte 0xe6
	.endm

	.macro nope7
	.byte 0xe7
	.endm

	.macro settypebasedhalvers param0:req
	.byte 0xe8
	.4byte \param0
	.endm

	.macro nope9
	.byte 0xe9
	.endm

	.macro tryrecycleitem param0:req
	.byte 0xea
	.4byte \param0
	.endm

	.macro settypetoterrain param0:req
	.byte 0xeb
	.4byte \param0
	.endm

	.macro pursuitrelated param0:req
	.byte 0xec
	.4byte \param0
	.endm

	.macro snatchsetbattlers
	.byte 0xed
	.endm

	.macro removelightscreenreflect
	.byte 0xee
	.endm

	.macro handleballthrow
	.byte 0xef
	.endm

	.macro givecaughtmon
	.byte 0xf0
	.endm

	.macro trysetcaughtmondexflags param0:req
	.byte 0xf1
	.4byte \param0
	.endm

	.macro displaydexinfo
	.byte 0xf2
	.endm

	.macro trygivecaughtmonnick param0:req
	.byte 0xf3
	.4byte \param0
	.endm

	.macro nopf4
	.byte 0xf4
	.endm

	.macro nopf5
	.byte 0xf5
	.endm

	.macro finishaction
	.byte 0xf6
	.endm

	.macro finishturn
	.byte 0xf7
	.endm

@new commands
    .macro callfunction ptr:req
	.byte 0xf8
	.4byte \ptr
	.endm
	 
	.macro cureprimarystatus battler:req, ptr:req
	.byte 0xf9
	.byte \battler
	.4byte \ptr
	.endm
	 
	.macro setword dst:req, value:req
	.byte 0xfa 
	.4byte \dst
	.4byte \value
	.endm
	 
    .macro jumpifsubstituteblocks ptr:req
	.byte 0xfb
	.4byte \ptr
	.endm
	
	.macro handleabilitypopup battler:req
	.byte 0xfc
	.byte \battler
	.endm
	
	.macro nopfd
	.byte 0xfd
	.endm
	
	.macro nopfe
	.byte 0xfe
	.endm
	
@ various command changed to more readable macros
	.macro cancelmultiturnmoves battler:req
	various \battler, VARIOUS_CANCEL_MULTI_TURN_MOVES
	.endm

	.macro setmagiccoattarget battler:req
	various \battler, VARIOUS_SET_MAGIC_COAT_TARGET
	.endm

	.macro getifcantrunfrombattle battler:req, checkIngrain:req
	various \battler, VARIOUS_IS_RUNNING_IMPOSSIBLE
	.byte \checkIngrain
	.endm

	.macro getbattlerfainted battler:req
	various \battler, VARIOUS_GET_BATTLER_FAINTED
	.endm

	.macro resetintimidatetracebits battler:req
	various \battler, VARIOUS_RESET_INTIMIDATE_TRACE_BITS
	.endm

	.macro updatechoicemoveonlvlup battler:req
	various \battler, VARIOUS_UPDATE_CHOICE_MOVE_ON_LVL_UP
	.endm

	.macro resetplayerfainted
	various BS_ATTACKER, VARIOUS_RESET_PLAYER_FAINTED_FLAG
	.endm

	.macro getbattlersforrecall
	various BS_ATTACKER, VARIOUS_GET_BATTLERS_FOR_RECALL
	.endm

	.macro returnopponentmon1toball battler:req
	various \battler, VARIOUS_RETURN_OPPONENT_MON1
	.endm

	.macro returnopponentmon2toball battler:req
	various \battler, VARIOUS_RETURN_OPPONENT_MON2
	.endm

	.macro checkpokeflute battler:req
	various \battler, VARIOUS_CHECK_POKEFLUTE
	.endm

	.macro waitfanfare battler:req
	various \battler, VARIOUS_WAIT_FANFARE
	.endm
	
    .macro tryactivatemoxie battler:req
	various \battler, VARIOUS_TRY_ACTIVATE_MOXIE
	.endm
	
	.macro tryactivategrimneigh battler:req
	various \battler, VARIOUS_TRY_ACTIVATE_GRIM_NEIGH
	.endm
	
	.macro tryactivatesoulheart
	various BS_ATTACKER, VARIOUS_TRY_ACTIVATE_SOUL_HEART
	.endm
	
	.macro tryactivatereceiver battler:req
	various \battler, VARIOUS_TRY_ACTIVATE_RECEIVER
	.endm
	
	.macro tryactivatebeastboost battler:req
	various \battler, VARIOUS_TRY_ACTIVATE_BEAST_BOOST
	.endm

    .macro tryfirstmondowntrainerslide battler:req
	various \battler, VARIOUS_TRAINER_SLIDE_FIRST_MON_DOWN
    .endm
	
	.macro trylastmontrainerslide battler:req
	various \battler, VARIOUS_TRAINER_SLIDE_LAST_MON
    .endm

    .macro updatenick battler:req
	various \battler, VARIOUS_UPDATE_NICK
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
	
	.macro activateweatherchangeabilities
	various BS_SCRIPTING, VARIOUS_ACTIVATE_WEATHER_ABILITIES
	.endm
	
	.macro tryneutralizinggassuppression battler:req, ptr:req
	various \battler, VARIOUS_TRY_NEUTRALIZING_GAS_SUPPRESSION
	.4byte \ptr
	.endm
	
	.macro jumpifrevived ptr:req
	various BS_ATTACKER, VARIOUS_JUMP_IF_BATTLER_REVIVED
	.4byte \ptr
	.endm
	
	.macro jumpifroarfails ptr:req
	various BS_ATTACKER, VARIOUS_JUMP_IF_ROAR_FAILS
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
	
	.macro incrementgamestat param0:req
	callfunction BS_IncrementGameStat
	.byte \param0
	.endm
	
	.macro getmovetarget
	callfunction BS_GetMoveTarget
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
	
	.macro setdestinybond
	callfunction BS_SetDestinyBond
	.endm
	
	.macro trysetdestinybondtohappen
	callfunction BS_TrySetDestinyBondToHappen
	.endm
	
	.macro setdefensecurlbit
	callfunction BS_SetDefenseCurl
	.endm
	
@ helpful macros
	.macro setstatchanger stat:req, stages:req, down:req
	setbyte sSTATCHANGER \stat | \stages << 4 | \down << 7
	.endm

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

	.macro jumpifstatus3 battler:req, status:req, jumpptr:req
	jumpifstatus3condition \battler, \status, 0x0, \jumpptr
	.endm

	.macro jumpifnostatus3 battler:req, status:req, jumpptr:req
	jumpifstatus3condition \battler, \status, 0x1, \jumpptr
	.endm

    .macro jumpiftype battler:req, type:req, jumpptr:req
	jumpbasedontype \battler, \type, 0x0, \jumpptr
	.endm
	
	.macro jumpifnotype battler:req, type:req, jumpptr:req
	jumpbasedontype \battler, \type, 0x1, \jumpptr
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

    .macro loadabilitypopup battler:req
	handleabilitypopup \battler
	.endm

    .macro removeabilitypopup battler:req
	handleabilitypopup ATKFC_REMOVE_POP_UP | \battler
	.endm
	
	.macro abilitycopypopup attacker:req, defender:req, stringId:req
	loadabilitypopup \attacker
	pause B_WAIT_TIME_SHORT
	loadabilitypopup \defender
	pause B_WAIT_TIME_SHORT
	removeabilitypopup \attacker
	pause B_WAIT_TIME_SHORT
	loadabilitypopup \attacker
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
	removeabilitypopup \attacker
	pause B_WAIT_TIME_SHORT
	loadabilitypopup \attacker
	pause B_WAIT_TIME_SHORT
	removeabilitypopup \defender
	pause B_WAIT_TIME_SHORT
	loadabilitypopup \defender
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
	
	.macro typecalc2
	typecalc
	bichalfword gMoveResultFlags, MOVE_RESULT_SUPER_EFFECTIVE | MOVE_RESULT_NOT_VERY_EFFECTIVE
	.endm
	
	.macro tryendeffectonabilitychange battler:req
	tryendneutralizinggas \battler @ switch in abilities still show it's pop ups
	setbyte sBYPASS_ABILITY_POP_UP, TRUE @ don't show ability pop up. I'm not sure if it's show or not, but added this bc yes...
	tryremoveprimalweather \battler
	tryremoveillusion \battler
	copybyte sBATTLER, \battler
	activateweatherchangeabilities
	setbyte sBYPASS_ABILITY_POP_UP, FALSE
	.endm
	
	.macro flushmessagebox
	printstring STRINGID_EMPTYSTRING3
	waitmessage 1
	.endm
	
	.macro switchoutmon battler:req, stringId:req, handleOrderCase:req, dontClearSubstituteBit:req
	switchoutabilities \battler
	waitstate
	switchhandleorder \battler, \handleOrderCase
	returntoball \battler
	getswitchedmondata \battler
	switchindataupdate \battler
	hpthresholds \battler
	printstring \stringId
	switchinanim \battler, \dontClearSubstituteBit
	waitstate
	switchineffects \battler
	.endm
	
	.macro savebattlers
	copybyte sSAVED_DMG, gBattlerAttacker
	copybyte sGIVEEXP_STATE, gBattlerTarget
	.endm
	
	.macro restorebattlers
	copybyte gBattlerAttacker, sSAVED_DMG
	copybyte gBattlerTarget, sGIVEEXP_STATE
	.endm

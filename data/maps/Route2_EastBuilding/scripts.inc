.equ REQUIRED_SEEN_MONS, 10

Route2_EastBuilding_MapScripts:: @ 816F67E
	.byte 0

Route2_EastBuilding_EventScript_Aide:: @ 816F67F
	lock
	faceplayer
	call Route2_EastBuilding_EventScript_GetAideRequestInfo
	goto_if_set FLAG_GOT_FLASH, Route2_EastBuilding_EventScript_AlreadyGotHM05
	msgbox Route2_EastBuilding_Text_GiveHM05IfSeen10Mons, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq Aide_EventScript_DeclineCheckMons
	setvar VAR_0x8004, 0
	specialvar VAR_RESULT, GetPokedexCount
	getnumberstring 2, VAR_0x8006
	call Route2_EastBuilding_EventScript_GetAideRequestInfo
	compare VAR_0x8006, REQUIRED_SEEN_MONS
	goto_if_lt Aide_EventScript_HaventCaughtEnough
	msgbox Route2_EastBuilding_Text_GreatHereYouGo
	checkitemspace NUM_TO_TM(102), 1
	compare VAR_RESULT, FALSE
	goto_if_eq Aide_EventScript_NoRoomForItem
	giveitem_msg Route2_EastBuilding_Text_ReceivedHM05FromAide, NUM_TO_TM(102)
	setflag FLAG_GOT_FLASH
	msgbox Route2_EastBuilding_Text_ExplainHM05
	release
	end

Route2_EastBuilding_EventScript_AlreadyGotHM05:: @ 816F701
	msgbox Route2_EastBuilding_Text_ExplainHM05
	release
	end

Route2_EastBuilding_EventScript_GetAideRequestInfo:: @ 816F70B
	getnumberstring 0, REQUIRED_SEEN_MONS
	getitemname 1, NUM_TO_TM(102)
	return

Route2_EastBuilding_EventScript_Rocker:: @ 816F714
	msgbox Route2_EastBuilding_Text_CanGetThroughRockTunnel, MSGBOX_NPC
	end

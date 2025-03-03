FuchsiaCity_WardensHouse_MapScripts:: @ 816D74B
	.byte 0

FuchsiaCity_WardensHouse_EventScript_Warden:: @ 816D74C
	lock
	faceplayer
	goto_if_set FLAG_MACHAMP_RIDE_GET, FuchsiaCity_WardensHouse_EventScript_ExplainStrength
	goto_if_set FLAG_HIDE_SAFARI_ZONE_WEST_GOLD_TEETH, FuchsiaCity_WardensHouse_EventScript_GiveGoldTeeth
	msgbox FuchsiaCity_WardensHouse_Text_HifFuffHefifoo, MSGBOX_YESNO
	compare VAR_RESULT, YES
	call_if_eq FuchsiaCity_WardensHouse_EventScript_WardenYes
	compare VAR_RESULT, NO
	call_if_eq FuchsiaCity_WardensHouse_EventScript_WardenNo
	release
	end

FuchsiaCity_WardensHouse_EventScript_GiveGoldTeeth:: @ 816D780
	textcolor 3
	playfanfare MUS_OBTAIN_TMHM
	message FuchsiaCity_WardensHouse_Text_GaveGoldTeethToWarden
	waitmessage
	waitfanfare
	msgbox FuchsiaCity_WardensHouse_Text_WardenPoppedInHisTeeth
	call EventScript_RestorePrevTextColor
	checkplayergender
	compare VAR_RESULT, MALE
	call_if_eq FuchsiaCity_WardensHouse_EventScript_WardenThanksMale
	compare VAR_RESULT, FEMALE
	call_if_eq FuchsiaCity_WardensHouse_EventScript_WardenThanksFemale
	setflag FLAG_MACHAMP_RIDE_GET
	removeitem ITEM_GOLD_TEETH, 1
	release
	end

FuchsiaCity_WardensHouse_EventScript_WardenThanksMale:: @ 816D7D6
	msgbox FuchsiaCity_WardensHouse_Text_ThanksSonGiveYouSomething
	return

FuchsiaCity_WardensHouse_EventScript_WardenThanksFemale:: @ 816D7DF
	msgbox FuchsiaCity_WardensHouse_Text_ThanksLassieGiveYouSomething
	return

FuchsiaCity_WardensHouse_EventScript_WardenYes:: @ 816D7E8
	msgbox FuchsiaCity_WardensHouse_Text_AhHowheeHoHoo
	return

FuchsiaCity_WardensHouse_EventScript_WardenNo:: @ 816D7F1
	msgbox FuchsiaCity_WardensHouse_Text_HeOhayHeHaHoo
	return

FuchsiaCity_WardensHouse_EventScript_ExplainStrength:: @ 816D7FA
	msgbox FuchsiaCity_WardensHouse_Text_ExplainStrength
	release
	end

FuchsiaCity_WardensHouse_EventScript_DisplaySign1:: @ 816D804
	msgbox FuchsiaCity_WardensHouse_Text_MonPhotosFossilsOnDisplay, MSGBOX_SIGN
	end

FuchsiaCity_WardensHouse_EventScript_DisplaySign2:: @ 816D80D
	msgbox FuchsiaCity_WardensHouse_Text_OldMonMerchandiseOnDisplay, MSGBOX_SIGN
	end

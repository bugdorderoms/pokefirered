Route12_FishingHouse_MapScripts:: @ 816FF0B
	.byte 0

Route12_FishingHouse_EventScript_FishingGuruBrother:: @ 816FF0C
	lock
	faceplayer
	goto_if_set FLAG_GOT_SUPER_ROD, Route12_FishingHouse_EventScript_End
	msgbox Route12_FishingHouse_Text_DoYouLikeToFish, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq Route12_FishingHouse_EventScript_GiveSuperRod
	msgbox Route12_FishingHouse_Text_OhThatsDisappointing
Route12_FishingHouse_EventScript_End::
	release
	end

Route12_FishingHouse_EventScript_GiveSuperRod:: @ 816FF34
	checkitemspace ITEM_SUPER_ROD, 1
	compare VAR_RESULT, FALSE
	goto_if_eq Route12_FishingHouse_EventScript_NoRoomForSuperRod
	additem ITEM_SUPER_ROD
	msgbox Route12_FishingHouse_Text_TakeThisAndFish
	msgreceiveditem Route12_FishingHouse_Text_ReceivedSuperRod, ITEM_SUPER_ROD
	setflag FLAG_GOT_SUPER_ROD
	release
	end

Route12_FishingHouse_EventScript_NoRoomForSuperRod:: @ 816FF75
	msgbox Route12_FishingHouse_Text_NoRoomForGift
	release
	end

CeruleanCity_House5_MapScripts:: @ 816AE4F
	.byte 0

CeruleanCity_House5_EventScript_BerryPowderMan:: @ 816AE50
	lock
	faceplayer
	msgbox CeruleanCity_House1_Text_AnyInterestInBerries, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq CeruleanCity_House5_EventScript_NoInterestInBerries
	msgbox CeruleanCity_House1_Text_HaveJustTheThing
	giveitem ITEM_ENIGMA_BERRY
	compare VAR_RESULT, FALSE
	goto_if_eq EventScript_BagIsFull
	msgbox CeruleanCity_House1_Text_HaveANiceDay
	release
	end

CeruleanCity_House5_EventScript_NoInterestInBerries:: @ 816AEAD
	msgbox CeruleanCity_House1_Text_TakeInterestInAllSortsOfThings
	release
	end

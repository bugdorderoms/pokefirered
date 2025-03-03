CeladonCity_DepartmentStore_Roof_MapScripts:: @ 816BCCC
	.byte 0

CeladonCity_DepartmentStore_Roof_EventScript_ThirstyGirl:: @ 816BCCD
	lock
	faceplayer
	call CeladonCity_DepartmentStore_Roof_EventScript_CheckPlayerHasDrinks
	compare VAR_TEMP_1, 0
	goto_if_eq CeladonCity_DepartmentStore_Roof_EventScript_IWantDrink
	goto CeladonCity_DepartmentStore_Roof_EventScript_AskGiveDrink
	end

CeladonCity_DepartmentStore_Roof_EventScript_CheckPlayerHasDrinks:: @ 816BCF6
	setvar VAR_TEMP_1, 0
	checkitem ITEM_FRESH_WATER, 1
	compare VAR_RESULT, TRUE
	call_if_eq CeladonCity_DepartmentStore_Roof_EventScript_SetHasFreshWater
	checkitem ITEM_SODA_POP, 1
	compare VAR_RESULT, TRUE
	call_if_eq CeladonCity_DepartmentStore_Roof_EventScript_SetHasSodaPop
	checkitem ITEM_LEMONADE, 1
	compare VAR_RESULT, TRUE
	call_if_eq CeladonCity_DepartmentStore_Roof_EventScript_SetHasLemonade
	return

CeladonCity_DepartmentStore_Roof_EventScript_SetHasFreshWater:: @ 816BD2C
	addvar VAR_TEMP_1, 1
	return

CeladonCity_DepartmentStore_Roof_EventScript_SetHasSodaPop:: @ 816BD32
	addvar VAR_TEMP_1, 2
	return

CeladonCity_DepartmentStore_Roof_EventScript_SetHasLemonade:: @ 816BD38
	addvar VAR_TEMP_1, 4
	return

CeladonCity_DepartmentStore_Roof_EventScript_AskGiveDrink:: @ 816BD3E
	msgbox CeladonCity_DepartmentStore_Roof_Text_ImThirstyGiveHerDrink, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq CeladonCity_DepartmentStore_Roof_EventScript_DontGiveDrink
	textcolor 3
	message CeladonCity_DepartmentStore_Roof_Text_GiveWhichDrink
	waitmessage
	textcolor 1
	switch VAR_TEMP_1
	case 1, CeladonCity_DepartmentStore_Roof_EventScript_AskGiveFreshWater
	case 2, CeladonCity_DepartmentStore_Roof_EventScript_AskGiveSodaPop
	case 3, CeladonCity_DepartmentStore_Roof_EventScript_AskGiveFreshWaterSodaPop
	case 4, CeladonCity_DepartmentStore_Roof_EventScript_AskGiveLemonade
	case 5, CeladonCity_DepartmentStore_Roof_EventScript_AskGiveFreshWaterLemonade
	case 6, CeladonCity_DepartmentStore_Roof_EventScript_AskGiveSodaPopLemonade
	case 7, CeladonCity_DepartmentStore_Roof_EventScript_AskGiveAllDrinks
	end

CeladonCity_DepartmentStore_Roof_EventScript_AskGiveFreshWater:: @ 816BDAE
	multichoice 0, 0, MULTICHOICE_THIRSTY_GIRL_FRESH_WATER, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_DepartmentStore_Roof_EventScript_GiveFreshWater
	case 1, CeladonCity_DepartmentStore_Roof_EventScript_DontGiveDrink
	case 127, CeladonCity_DepartmentStore_Roof_EventScript_DontGiveDrink
	end

CeladonCity_DepartmentStore_Roof_EventScript_AskGiveSodaPop:: @ 816BDDA
	multichoice 0, 0, MULTICHOICE_THIRSTY_GIRL_SODA_POP, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_DepartmentStore_Roof_EventScript_GiveSodaPop
	case 1, CeladonCity_DepartmentStore_Roof_EventScript_DontGiveDrink
	case 127, CeladonCity_DepartmentStore_Roof_EventScript_DontGiveDrink
	end

CeladonCity_DepartmentStore_Roof_EventScript_AskGiveFreshWaterSodaPop:: @ 816BE06
	multichoice 0, 0, MULTICHOICE_THIRSTY_GIRL_FRESH_WATER_SODA_POP, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_DepartmentStore_Roof_EventScript_GiveFreshWater
	case 1, CeladonCity_DepartmentStore_Roof_EventScript_GiveSodaPop
	case 2, CeladonCity_DepartmentStore_Roof_EventScript_DontGiveDrink
	case 127, CeladonCity_DepartmentStore_Roof_EventScript_DontGiveDrink
	end

CeladonCity_DepartmentStore_Roof_EventScript_AskGiveLemonade:: @ 816BE3D
	multichoice 0, 0, MULTICHOICE_THIRSTY_GIRL_LEMONADE, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_DepartmentStore_Roof_EventScript_GiveLemonade
	case 1, CeladonCity_DepartmentStore_Roof_EventScript_DontGiveDrink
	case 127, CeladonCity_DepartmentStore_Roof_EventScript_DontGiveDrink
	end

CeladonCity_DepartmentStore_Roof_EventScript_AskGiveFreshWaterLemonade:: @ 816BE69
	multichoice 0, 0, MULTICHOICE_THIRSTY_GIRL_FRESH_WATER_LEMONADE, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_DepartmentStore_Roof_EventScript_GiveFreshWater
	case 1, CeladonCity_DepartmentStore_Roof_EventScript_GiveLemonade
	case 2, CeladonCity_DepartmentStore_Roof_EventScript_DontGiveDrink
	case 127, CeladonCity_DepartmentStore_Roof_EventScript_DontGiveDrink
	end

CeladonCity_DepartmentStore_Roof_EventScript_AskGiveSodaPopLemonade:: @ 816BEA0
	multichoice 0, 0, MULTICHOICE_THIRSTY_GIRL_SODA_POP_LEMONADE, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_DepartmentStore_Roof_EventScript_GiveSodaPop
	case 1, CeladonCity_DepartmentStore_Roof_EventScript_GiveLemonade
	case 2, CeladonCity_DepartmentStore_Roof_EventScript_DontGiveDrink
	case 127, CeladonCity_DepartmentStore_Roof_EventScript_DontGiveDrink
	end

CeladonCity_DepartmentStore_Roof_EventScript_AskGiveAllDrinks:: @ 816BED7
	multichoice 0, 0, MULTICHOICE_THIRSTY_GIRL_FRESH_WATER_SODA_POP_LEMONADE, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_DepartmentStore_Roof_EventScript_GiveFreshWater
	case 1, CeladonCity_DepartmentStore_Roof_EventScript_GiveSodaPop
	case 2, CeladonCity_DepartmentStore_Roof_EventScript_GiveLemonade
	case 3, CeladonCity_DepartmentStore_Roof_EventScript_DontGiveDrink
	case 127, CeladonCity_DepartmentStore_Roof_EventScript_DontGiveDrink
	end

CeladonCity_DepartmentStore_Roof_EventScript_GiveFreshWater:: @ 816BF19
	setvar VAR_0x8008, ITEM_FRESH_WATER
	setvar VAR_0x8009, NUM_TO_TM(16)
	goto_if_set FLAG_GOT_TM16_FROM_THIRSTY_GIRL, CeladonCity_DepartmentStore_Roof_EventScript_NotThirstyAfterAll
	msgbox CeladonCity_DepartmentStore_Roof_Text_YayFreshWaterHaveThis
	goto CeladonCity_DepartmentStore_Roof_EventScript_GiveDrink
	end

CeladonCity_DepartmentStore_Roof_EventScript_GiveSodaPop:: @ 816BF3A
	setvar VAR_0x8008, ITEM_SODA_POP
	setvar VAR_0x8009, NUM_TO_TM(20)
	goto_if_set FLAG_GOT_TM20_FROM_THIRSTY_GIRL, CeladonCity_DepartmentStore_Roof_EventScript_NotThirstyAfterAll
	msgbox CeladonCity_DepartmentStore_Roof_Text_YaySodaPopHaveThis
	goto CeladonCity_DepartmentStore_Roof_EventScript_GiveDrink
	end

CeladonCity_DepartmentStore_Roof_EventScript_GiveLemonade:: @ 816BF5B
	setvar VAR_0x8008, ITEM_LEMONADE
	setvar VAR_0x8009, NUM_TO_TM(33)
	goto_if_set FLAG_GOT_TM33_FROM_THIRSTY_GIRL, CeladonCity_DepartmentStore_Roof_EventScript_NotThirstyAfterAll
	msgbox CeladonCity_DepartmentStore_Roof_Text_YayLemonadeHaveThis
	goto CeladonCity_DepartmentStore_Roof_EventScript_GiveDrink
	end

CeladonCity_DepartmentStore_Roof_EventScript_GiveDrink:: @ 816BF7C
	getitemname 0, VAR_0x8008
	getitemname 1, VAR_0x8009
	removeitem VAR_0x8008, 1
	checkitemspace VAR_0x8009, 1
	compare VAR_RESULT, FALSE
	goto_if_eq CeladonCity_DepartmentStore_Roof_EventScript_NoRoomForReward
	additem VAR_0x8009
	textcolor 3
	playfanfare MUS_LEVEL_UP
	message Text_ReceivedItemFromLittleGirl
	waitmessage
	waitfanfare
	putitemaway VAR_0x8009
	call EventScript_RestorePrevTextColor
	compare VAR_0x8008, ITEM_FRESH_WATER
	call_if_eq CeladonCity_DepartmentStore_Roof_EventScript_ExplainTM16
	compare VAR_0x8008, ITEM_SODA_POP
	call_if_eq CeladonCity_DepartmentStore_Roof_EventScript_ExplainTM20
	compare VAR_0x8008, ITEM_LEMONADE
	call_if_eq CeladonCity_DepartmentStore_Roof_EventScript_ExplainTM33
	release
	end

CeladonCity_DepartmentStore_Roof_EventScript_ExplainTM16:: @ 816BFDE
	setflag FLAG_GOT_TM16_FROM_THIRSTY_GIRL
	msgbox CeladonCity_DepartmentStore_Roof_Text_ExplainTM16
	return

CeladonCity_DepartmentStore_Roof_EventScript_ExplainTM20:: @ 816BFEA
	setflag FLAG_GOT_TM20_FROM_THIRSTY_GIRL
	msgbox CeladonCity_DepartmentStore_Roof_Text_ExplainTM20
	return

CeladonCity_DepartmentStore_Roof_EventScript_ExplainTM33:: @ 816BFF6
	setflag FLAG_GOT_TM33_FROM_THIRSTY_GIRL
	msgbox CeladonCity_DepartmentStore_Roof_Text_ExplainTM33
	return

CeladonCity_DepartmentStore_Roof_EventScript_NoRoomForReward:: @ 816C002
	msgbox CeladonCity_DepartmentStore_Roof_Text_DontHaveSpaceForThis
	release
	end

CeladonCity_DepartmentStore_Roof_EventScript_DontGiveDrink:: @ 816C00C
	release
	end

CeladonCity_DepartmentStore_Roof_EventScript_IWantDrink:: @ 816C00E
	msgbox CeladonCity_DepartmentStore_Roof_Text_ImThirstyIWantDrink
	release
	end

CeladonCity_DepartmentStore_Roof_EventScript_NotThirstyAfterAll:: @ 816C018
	msgbox CeladonCity_DepartmentStore_Roof_Text_ImNotThirstyAfterAll
	release
	end

CeladonCity_DepartmentStore_Roof_EventScript_CooltrainerM:: @ 816C022
	msgbox CeladonCity_DepartmentStore_Roof_Text_MySisterIsImmature, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_Roof_EventScript_FloorSign:: @ 816C02B
	msgbox CeladonCity_DepartmentStore_Roof_Text_FloorSign, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_Roof_EventScript_VendingMachine:: @ 816C034
	lockall
	message CeladonCity_DepartmentStore_Roof_Text_VendingMachineWhatDoesItHave
	waitmessage
	showmoneybox 0, 0, 0
	goto CeladonCity_DepartmentStore_Roof_EventScript_ChooseDrink
	end

CeladonCity_DepartmentStore_Roof_EventScript_ChooseDrink:: @ 816C045
	multichoice 12, 0, MULTICHOICE_CELADON_VENDING_MACHINE, FALSE
	copyvar VAR_TEMP_1, VAR_RESULT
	switch VAR_TEMP_1
	case 0, CeladonCity_DepartmentStore_Roof_EventScript_BuyFreshWater
	case 1, CeladonCity_DepartmentStore_Roof_EventScript_BuySodaPop
	case 2, CeladonCity_DepartmentStore_Roof_EventScript_BuyLemonade
	msgbox CeladonCity_DepartmentStore_Roof_Text_NotThirsty
	goto CeladonCity_DepartmentStore_Roof_EventScript_ExitVendingMachine
	end

CeladonCity_DepartmentStore_Roof_EventScript_BuyFreshWater:: @ 816C083
	setvar VAR_TEMP_0, ITEM_FRESH_WATER
	checkmoney 200, 0
	goto CeladonCity_DepartmentStore_Roof_EventScript_TryBuyDrink
	end

CeladonCity_DepartmentStore_Roof_EventScript_BuySodaPop:: @ 816C094
	setvar VAR_TEMP_0, ITEM_SODA_POP
	checkmoney 300, 0
	goto CeladonCity_DepartmentStore_Roof_EventScript_TryBuyDrink
	end

CeladonCity_DepartmentStore_Roof_EventScript_BuyLemonade:: @ 816C0A5
	setvar VAR_TEMP_0, ITEM_LEMONADE
	checkmoney 350, 0
	goto CeladonCity_DepartmentStore_Roof_EventScript_TryBuyDrink
	end

CeladonCity_DepartmentStore_Roof_EventScript_RemoveMoneyFreshWater:: @ 816C0B6
	removemoney 200, 0
	return

CeladonCity_DepartmentStore_Roof_EventScript_RemoveMoneySodaPop:: @ 816C0BD
	removemoney 300, 0
	return

CeladonCity_DepartmentStore_Roof_EventScript_RemoveMoneyLemonade:: @ 816C0C4
	removemoney 350, 0
	return

CeladonCity_DepartmentStore_Roof_EventScript_TryBuyDrink:: @ 816C0CB
	compare VAR_RESULT, FALSE
	goto_if_eq CeladonCity_DepartmentStore_Roof_EventScript_NotEnoughMoney
	checkitemspace VAR_TEMP_0, 1
	compare VAR_RESULT, FALSE
	goto_if_eq CeladonCity_DepartmentStore_Roof_EventScript_NoRoomForDrink
	compare VAR_TEMP_1, 0
	call_if_eq CeladonCity_DepartmentStore_Roof_EventScript_RemoveMoneyFreshWater
	compare VAR_TEMP_1, 1
	call_if_eq CeladonCity_DepartmentStore_Roof_EventScript_RemoveMoneySodaPop
	compare VAR_TEMP_1, 2
	call_if_eq CeladonCity_DepartmentStore_Roof_EventScript_RemoveMoneyLemonade
	updatemoneybox 0, 0, 0
	getitemname 0, VAR_TEMP_0
	playse SE_VEND
	msgbox CeladonCity_DepartmentStore_Roof_Text_DrinkCanPoppedOut
	additem VAR_TEMP_0
	goto CeladonCity_DepartmentStore_Roof_EventScript_ChooseNewDrink
	end

CeladonCity_DepartmentStore_Roof_EventScript_ChooseNewDrink:: @ 816C125
	message CeladonCity_DepartmentStore_Roof_Text_VendingMachineWhatDoesItHave
	waitmessage
	goto CeladonCity_DepartmentStore_Roof_EventScript_ChooseDrink
	end

CeladonCity_DepartmentStore_Roof_EventScript_NotEnoughMoney:: @ 816C131
	msgbox CeladonCity_DepartmentStore_Roof_Text_NotEnoughMoney
	goto CeladonCity_DepartmentStore_Roof_EventScript_ExitVendingMachine
	end

CeladonCity_DepartmentStore_Roof_EventScript_NoRoomForDrink:: @ 816C13F
	msgbox CeladonCity_DepartmentStore_Roof_Text_NoMoreRoomForStuff
	goto CeladonCity_DepartmentStore_Roof_EventScript_ExitVendingMachine
	end

CeladonCity_DepartmentStore_Roof_EventScript_ExitVendingMachine:: @ 816C14D
	hidemoneybox 0, 0
	releaseall
	end

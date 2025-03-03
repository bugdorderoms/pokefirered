Route5_PokemonDayCare_EventScript_DaycareMan:: @ 81BF398
	lock
	faceplayer
	showmoneybox 0, 0, 0
	specialvar VAR_RESULT, IsThereMonInRoute5Daycare
	compare VAR_RESULT, TRUE
	goto_if_eq Route5_PokemonDayCare_EventScript_CheckOnMon
	msgbox Route5_PokemonDayCare_Text_WantMeToRaiseMon, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq Route5_PokemonDayCare_EventScript_TryGiveMon
	msgbox Route5_PokemonDayCare_Text_ComeAgain
	goto Route5_PokemonDayCare_EventScript_CloseMoneyBox
	end

Route5_PokemonDayCare_EventScript_TryGiveMon:: @ 81BF3DD
	specialvar VAR_RESULT, CountPartyNonEggMons
	compare VAR_RESULT, 1
	goto_if_eq Route5_PokemonDayCare_EventScript_OnlyOneMonInParty
	msgbox Route5_PokemonDayCare_Text_WhichMonShouldIRaise
	fadescreen FADE_TO_BLACK
	hidemoneybox 0, 0
	special ChooseSendDaycareMon
	waitstate
	showmoneybox 0, 0, 0
	compare VAR_0x8004, PARTY_SIZE
	goto_if_ge Route5_PokemonDayCare_EventScript_ComeAgain
	specialvar VAR_RESULT, CountPartyAliveNonEggMons_IgnoreVar0x8004Slot
	compare VAR_RESULT, 0
	goto_if_eq Route5_PokemonDayCare_EventScript_OnlyOneAliveMonInParty
	specialvar VAR_0x8005, GetSelectedMonNicknameAndSpecies
	msgbox Route5_PokemonDayCare_Text_LookAfterMonForAWhile
	waitse
	playmoncry VAR_0x8005, CRY_MODE_NORMAL
	msgbox Route5_PokemonDayCare_Text_ComeSeeMeInAWhile
	waitmoncry
	special PutMonInRoute5Daycare
	incrementgamestat GAME_STAT_USED_DAYCARE
	goto Route5_PokemonDayCare_EventScript_CloseMoneyBox
	end

Route5_PokemonDayCare_EventScript_ComeAgain:: @ 81BF444
	msgbox Route5_PokemonDayCare_Text_ComeAnytimeYouLike
	goto Route5_PokemonDayCare_EventScript_CloseMoneyBox
	end

Route5_PokemonDayCare_EventScript_OnlyOneMonInParty:: @ 81BF452
	msgbox Route5_PokemonDayCare_Text_OnlyHaveOneMonWithYou
	goto Route5_PokemonDayCare_EventScript_CloseMoneyBox
	end

Route5_PokemonDayCare_EventScript_OnlyOneAliveMonInParty:: @ 81BF460
	msgbox Route5_PokemonDayCare_Text_WhatWillYouBattleWith
	goto Route5_PokemonDayCare_EventScript_CloseMoneyBox
	end

Route5_PokemonDayCare_EventScript_CheckOnMon:: @ 81BF46E
	setvar VAR_0x8004, 0
	specialvar VAR_RESULT, GetNumLevelsGainedForRoute5DaycareMon
	compare VAR_RESULT, 0
	call_if_ne Route5_PokemonDayCare_EventScript_MonHasGrownByXLevels
	compare VAR_RESULT, 0
	call_if_eq Route5_PokemonDayCare_EventScript_NotEnoughTime
	special GetCostToWithdrawRoute5DaycareMon
	msgbox Route5_PokemonDayCare_Text_OweMeXForMonsReturn, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq Route5_PokemonDayCare_EventScript_TryRetrieveMon
	goto Route5_PokemonDayCare_EventScript_ComeAgain
	end

Route5_PokemonDayCare_EventScript_MonHasGrownByXLevels:: @ 81BF4AA
	msgbox Route5_PokemonDayCare_Text_MonHasGrownByXLevels
	return

Route5_PokemonDayCare_EventScript_NotEnoughTime:: @ 81BF4B3
	msgbox Route5_PokemonDayCare_Text_MonNeedsToSpendMoreTime
	return

Route5_PokemonDayCare_EventScript_TryRetrieveMon:: @ 81BF4BC
	specialvar VAR_RESULT, CalculatePlayerPartyCount
	compare VAR_RESULT, PARTY_SIZE
	goto_if_eq Route5_PokemonDayCare_EventScript_NoRoomInParty
	specialvar VAR_RESULT, IsEnoughForCostInVar0x8005
	compare VAR_RESULT, TRUE
	goto_if_eq Route5_PokemonDayCare_EventScript_RetrieveMon
	msgbox Route5_PokemonDayCare_Text_DontHaveEnoughMoney
	goto Route5_PokemonDayCare_EventScript_CloseMoneyBox
	end

Route5_PokemonDayCare_EventScript_RetrieveMon:: @ 81BF4EA
	setvar VAR_0x8004, 0
	specialvar VAR_RESULT, TakePokemonFromRoute5Daycare
	special SubtractMoneyFromVar0x8005
	updatemoneybox 0, 0, 0
	copyvar VAR_0x8008, VAR_RESULT
	getpartysize
	subvar VAR_RESULT, 1
	getpartymonname 0, VAR_RESULT
	copyvar VAR_RESULT, VAR_0x8008
	msgbox Route5_PokemonDayCare_Text_ThankYouHeresMon
	textcolor 3
	waitse
	playmoncry VAR_RESULT, CRY_MODE_NORMAL
	msgbox Route5_PokemonDayCare_Text_PlayerGotMonBack
	call EventScript_RestorePrevTextColor
	waitmoncry
	goto Route5_PokemonDayCare_EventScript_CloseMoneyBox
	end

Route5_PokemonDayCare_EventScript_NoRoomInParty:: @ 81BF533
	msgbox Route5_PokemonDayCare_Text_YouveGotNoRoomForIt
	goto Route5_PokemonDayCare_EventScript_CloseMoneyBox
	end

Route5_PokemonDayCare_EventScript_CloseMoneyBox:: @ 81BF541
	hidemoneybox 0, 0
	release
	end

EventScript_EggHatch:: @ 81BF546
	lockall
	msgbox DayCare_Text_Huh
	special EggHatch
	waitstate
	releaseall
	end

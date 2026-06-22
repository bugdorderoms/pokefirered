EventScript_RaidDen::
	lockall
	specialvar VAR_RESULT, IsRaidBattleAvailable
	compare VAR_RESULT, FALSE
	goto_if_eq EventScript_RaidDen_Cleared
	call EventScript_AskSaveGame
	compare VAR_RESULT, FALSE
	goto_if_eq EventScript_RaidDen_End
	fadescreen FADE_TO_BLACK
	special RaidDenInitIntro
	waitstate
	compare VAR_0x8004, FALSE
	goto_if_eq EventScript_RaidDen_End
	specialvar VAR_RESULT, WasRaidBossDefeated
	compare VAR_RESULT, FALSE
	goto_if_eq EventScript_RaidDen_SaveGameAndEnd
	call EventScript_RaidBattle_GiveRewardsAndMon
	delay 15
	special SetRaidClearedFlag
	applymovement VAR_LAST_TALKED, Movement_ChangeRaidDenState
	waitmovement 0
EventScript_RaidDen_SaveGameAndEnd::
	call EventScript_SaveGame
EventScript_RaidDen_End::
	releaseall
	end

EventScript_RaidBattle_GiveRewardsAndMon::
	specialvar VAR_RESULT, WasRaidBossCaught
	compare VAR_RESULT, FALSE
	goto_if_eq EventScript_RaidBattle_GiveRewards
	specialvar VAR_RESULT, GiveRaidBattleBoss
	compare VAR_RESULT, 0
	goto_if_eq EventScript_RaidBattle_GiveMonToParty
EventScript_RaidBattle_GiveMonToBox::
	call EventScript_NameReceivedBoxMon
	call EventScript_TransferredToPC
	goto EventScript_RaidBattle_GiveRewards

EventScript_RaidBattle_GiveMonToParty::
	getspeciesname 0, VAR_TEMP_1
	givenickmsg EventScript_RaidBattle_ChangeMonNickname, EventScript_RaidBattle_GiveRewards
EventScript_RaidBattle_ChangeMonNickname::
	call EventScript_GetGiftMonPartySlot
	call EventScript_ChangePokemonNickname
EventScript_RaidBattle_GiveRewards::
	setvar VAR_TEMP_0, 0 @ Loop counter
EventScript_RaidBattle_GiveRewardsLoop::
	special GiveRaidBattleRewards
	compare VAR_RESULT, TRUE
	goto_if_eq EventScript_RaidBattle_GiveRewards_End @ Done giving rewards
	giveitem VAR_TEMP_1, VAR_TEMP_2
	delay 5
	goto EventScript_RaidBattle_GiveRewardsLoop
EventScript_RaidBattle_GiveRewards_End::
	return

EventScript_RaidDen_Cleared::
	checkitem ITEM_WISHING_PIECE, 1
	compare VAR_RESULT, FALSE
	goto_if_eq EventScript_RaidDen_PrintAnythingInDen
	getitemname 0, ITEM_WISHING_PIECE
	msgbox Text_AskThrowWishingPiece, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq EventScript_RaidDen_DontThrowWishingPiece
	call EventScript_ThrowWishingPiece
	goto EventScript_RaidDen_End
	
EventScript_RaidDen_PrintAnythingInDen::
	msgbox Text_AnythingInDen, MSGBOX_SIGN
	goto EventScript_RaidDen_End

EventScript_RaidDen_DontThrowWishingPiece::
	closemessage
	goto EventScript_RaidDen_End

EventScript_ThrowWishingPiece::
	getitemname 0, ITEM_WISHING_PIECE
	removeitem ITEM_WISHING_PIECE, 1
	playse SE_USE_ITEM
	waitse
	msgbox Text_ThrowWishingPiece
	closemessage
	special ClearRaidClearedFlag
	special RollWishingPieceRandomValue
	applymovement VAR_LAST_TALKED, Movement_ChangeRaidDenState
	waitmovement 0
	return

Movement_ChangeRaidDenState::
	change_raid_den
	step_end

EventScript_WishingPiece::
	lockall
	call EventScript_ThrowWishingPiece
	releaseall
	end

Text_AnythingInDen::
	.string "There doesn't seem to be anything\n"
	.string "in the den...$"

Text_AskThrowWishingPiece::
	.string "There doesn't seem to be anything\n"
	.string "in the den...\p"
	.string "Want to throw in a {STR_VAR_1}?$"

Text_ThrowWishingPiece::
	.string "{PLAYER} threw a {STR_VAR_1}\n"
	.string "into the den!$"

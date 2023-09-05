CeruleanCity_House4_MapScripts:: @ 816ACEE
	.byte 0

CeruleanCity_House4_EventScript_WonderNewsBerryMan:: @ 816ACEF
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	lock
	msgbox CeruleanCity_House4_Text_NothingEntertaining
	release
	end

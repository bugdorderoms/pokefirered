SixIsland_Mart_MapScripts:: @ 8171E65
	.byte 0

SixIsland_Mart_EventScript_Clerk:: @ 8171E66
	lock
	faceplayer
	message Text_MayIHelpYou
	waitmessage
	pokemart SixIsland_Mart_Items
	msgbox Text_PleaseComeAgain
	release
	end

	.align 2
SixIsland_Mart_Items:: @ 8171E8C
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_MAX_POTION
	.2byte ITEM_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_DREAM_MAIL
	.2byte ITEM_NONE
	release
	end

SixIsland_Mart_EventScript_Picnicker:: @ 8171EA0
	msgbox SixIsland_Mart_Text_ShouldBuyMailAndWriteLetter, MSGBOX_NPC
	end

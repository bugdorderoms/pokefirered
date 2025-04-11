#include "global.h"
#include "gflib.h"
#include "mail.h"
#include "mail_data.h"
#include "item.h"
#include "item_use.h"
#include "constants/items.h"
#include "pokemon_icon.h"

void ClearMailData(void)
{
    u32 i;

    for (i = 0; i < MAIL_COUNT; i++)
        ClearMailStruct(&gSaveBlock1Ptr->mail[i]);
}

void ClearMailStruct(struct Mail *mail)
{
    u32 i;

    for (i = 0; i < MAIL_WORDS_COUNT; i++)
        mail->words[i] = 0xFFFF;
	
    for (i = 0; i < PLAYER_NAME_LENGTH + 1; i++)
        mail->playerName[i] = EOS;
	
    for (i = 0; i < 4; i++)
        mail->trainerId[i] = 0;
	
    mail->species = SPECIES_BULBASAUR;
    mail->itemId = ITEM_NONE;
}

bool32 MonHasMail(struct Pokemon *mon)
{
    if (ItemIsMail(GetMonData(mon, MON_DATA_HELD_ITEM)) && GetMonData(mon, MON_DATA_MAIL) != 0xFF)
        return TRUE;
    else
        return FALSE;
}

u32 GiveMailToMon(struct Pokemon *mon, u32 itemId)
{
    u32 id, i;

    for (id = 0; id < PARTY_SIZE; id++)
    {
        if (!gSaveBlock1Ptr->mail[id].itemId)
        {
            for (i = 0; i < MAIL_WORDS_COUNT; i++)
                gSaveBlock1Ptr->mail[id].words[i] = 0xFFFF;
			
            for (i = 0; i < PLAYER_NAME_LENGTH && gSaveBlock2Ptr->playerName[i] != EOS; i++)
                gSaveBlock1Ptr->mail[id].playerName[i] = gSaveBlock2Ptr->playerName[i];
			
            for (; i <= 5; i++)
            	gSaveBlock1Ptr->mail[id].playerName[i] = CHAR_SPACE;
			
            gSaveBlock1Ptr->mail[id].playerName[i] = EOS;
			
            for (i = 0; i < 4; i++)
                gSaveBlock1Ptr->mail[id].trainerId[i] = gSaveBlock2Ptr->playerTrainerId[i];

            gSaveBlock1Ptr->mail[id].species = GetMonData(mon, MON_DATA_SPECIES);
            gSaveBlock1Ptr->mail[id].itemId = itemId;
			
            SetMonData(mon, MON_DATA_MAIL, &id);
            SetMonData(mon, MON_DATA_HELD_ITEM, &itemId);
			
            return id;
        }
    }
    return 0xFF;
}

u32 GiveMailToMon2(struct Pokemon *mon, struct Mail *mail)
{
    u32 itemId = mail->itemId;
    u32 mailId = GiveMailToMon(mon, itemId);

    if (mailId == 0xFF)
        return 0xFF;
	
    gSaveBlock1Ptr->mail[mailId] = *mail;
	
    SetMonData(mon, MON_DATA_MAIL, &mailId);
    SetMonData(mon, MON_DATA_HELD_ITEM, &itemId);

    return mailId;
}

void TakeMailFromMon(struct Pokemon *mon)
{
    if (MonHasMail(mon))
    {
        u32 val = GetMonData(mon, MON_DATA_MAIL);
		
        gSaveBlock1Ptr->mail[val].itemId = ITEM_NONE;
		
        val = 0xFF;
        SetMonData(mon, MON_DATA_MAIL, &val);
		
		val = ITEM_NONE;
        SetMonData(mon, MON_DATA_HELD_ITEM, &val);
    }
}

u32 TakeMailFromMon2(struct Pokemon *mon)
{
	u32 i, val = GetMonData(mon, MON_DATA_MAIL);
	
    for (i = PARTY_SIZE; i < MAIL_COUNT; i++)
    {
        if (!gSaveBlock1Ptr->mail[i].itemId)
        {
            memcpy(&gSaveBlock1Ptr->mail[i], &gSaveBlock1Ptr->mail[val], sizeof(struct Mail));
			
            gSaveBlock1Ptr->mail[val].itemId = ITEM_NONE;
			
			val = 0xFF;
            SetMonData(mon, MON_DATA_MAIL, &val);
			
			val = ITEM_NONE;
            SetMonData(mon, MON_DATA_HELD_ITEM, &val);
			
            return i;
        }
    }
    return 0xFF;
}

bool32 ItemIsMail(u32 itemId)
{
	return (ItemId_GetFieldFunc(itemId) == FieldUseFunc_OrangeMail);
}

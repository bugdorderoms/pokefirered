#ifndef GUARD_MYSTERY_GIFT_MENU_H
#define GUARD_MYSTERY_GIFT_MENU_H

enum
{
	MYSTERY_GIFT_CODE_INVALID, // Code does't exist
	MYSTERY_GIFT_CODE_ALREADY_OBTAINED, // Gift already obtained
	MYSTERY_GIFT_CODE_FAILED, // Failed due to specific things, like don't have space for pokemon or item
	MYSTERY_GIFT_CODE_SUCCESS, // Success on receive the gift
};

extern bool8 gGiftIsFromEReader;

void MainCB_FreeAllBuffersAndReturnToInitTitleScreen(void);
void PrintMysteryGiftOrEReaderTopMenu(bool8, bool32);
void c2_mystery_gift_e_reader_run(void);
u8 GetMysteryGiftCodeState(const u8 *code);

#endif //GUARD_MYSTERY_GIFT_MENU_H

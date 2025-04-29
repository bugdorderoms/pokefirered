#ifndef GUARD_SCRIPT_MENU_H
#define GUARD_SCRIPT_MENU_H

#include "global.h"
#include "menu.h"

extern const u8 *const gStdStringPtrs[];

#define MULTICHOICE_DEFAULT_BASE_BLOCK 0x038

// Multichoice
u32 InitMultichoice(const struct MenuAction * items, u32 count, u32 bg, u8 x, u8 y, bool32 ignoreBPress, u32 defaultOpt, u32 perRowItems, u16 baseBlock, u32 palNum);
bool32 ScriptMenu_Multichoice(u8 x, u8 y, u32 mcId, bool32 ignoreBPress, u32 defaultOpt, u32 perRowItems);
bool32 CreatePCMenu(void);
void ScriptMenu_DisplayPCStartupPrompt(void);
void DrawRepelMultichoiseMenu(void);
void HandleRepelUseAnother(void);

// List Menu
u32 InitFormChangeListMenu(u32 listId);
u32 GetFormChangeListMenuSpecies(u32 listId);
void Task_DestroyListMenu(u32 taskId, bool32 enableScripts);

// Yes No menu
bool32 ScriptMenu_YesNo(void);

// Pokemon pic
bool32 ScriptMenu_ShowPokemonPic(u32 var1, u8 var2, u8 var3);
bool32 (*ScriptMenu_GetPicboxWaitFunc(void))(void);
void PicboxCancel(void);
void RemovePokemonSpeciesOnPicbox(void);
void UpdatePokemonSpeciesOnPicbox(u32 species, u8 x, u8 y);

// Item pic
bool32 ScriptMenu_ShowItemPic(u32 itemId, u8 x, u8 y);

#endif //GUARD_SCRIPT_MENU_H

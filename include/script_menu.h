#ifndef GUARD_SCRIPT_MENU_H
#define GUARD_SCRIPT_MENU_H

#include "global.h"
#include "menu.h"

extern const u8 *const gStdStringPtrs[];

#define MULTICHOICE_DEFAULT_BASE_BLOCK 0x038

// Multichoice
u8 InitMultichoice(const struct MenuAction * items, u8 count, u8 bg, u8 x, u8 y, bool8 ignoreBPress, u8 defaultOpt, u8 perRowItems, u16 baseBlock, u8 palNum);
bool8 ScriptMenu_Multichoice(u8 x, u8 y, u8 mcId, bool8 ignoreBPress, u8 defaultOpt, u8 perRowItems);
bool8 CreatePCMenu(void);
void ScriptMenu_DisplayPCStartupPrompt(void);
void DrawRepelMultichoiseMenu(void);
void HandleRepelUseAnother(void);

// List Menu
u8 InitFormChangeListMenu(u8 listId);
u16 GetFormChangeListMenuSpecies(u8 listId);
void Task_DestroyListMenu(u8 taskId, bool8 enableScripts);

// Yes No menu
bool8 ScriptMenu_YesNo(u8 var1, u8 var2);

// Pokemon pic
bool8 ScriptMenu_ShowPokemonPic(u16 var1, u8 var2, u8 var3);
bool8 (*ScriptMenu_GetPicboxWaitFunc(void))(void);
void QLPlaybackCB_DestroyScriptMenuMonPicSprites(void);
void PicboxCancel(void);
void RemovePokemonSpeciesOnPicbox(void);
void UpdatePokemonSpeciesOnPicbox(u16 species, u8 x, u8 y);

// Item pic
bool8 ScriptMenu_ShowItemPic(u16 itemId, u8 x, u8 y);

#endif //GUARD_SCRIPT_MENU_H

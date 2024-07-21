#ifndef GUARD_ITEM_MENU_ICONS
#define GUARD_ITEM_MENU_ICONS

#include "global.h"

#define ITEMICON_TAG 0xD750

extern const struct CompressedSpriteSheet gSpriteSheet_Backpack;
extern const struct CompressedSpriteSheet gSpriteSheet_Satchel;
extern const struct CompressedSpritePalette gSpritePalette_BagOrSatchel;

extern const struct CompressedSpriteSheet gBagSwapSpriteSheet;
extern const struct CompressedSpritePalette gBagSwapSpritePalette;

void ResetItemMenuIconState(void);
void ItemMenuIcons_CreateInsertIndicatorBarHidden(void);
void DestroyItemMenuIcon(bool8 a0);
void CreateItemMenuIcon(u16 itemId, bool8 a0);
u8 AddItemIconObject(u16 tilesTag, u16 paletteTag, u16 itemId);
u8 AddItemIconObjectWithCustomObjectTemplate(const struct SpriteTemplate * origTemplate, u16 tilesTag, u16 paletteTag, u16 itemId);
void sub_80989A0(u16 itemId, u8 idx);
void ItemMenuIcons_MoveInsertIndicatorBar(s16 x, u16 y);
void ItemMenuIcons_ToggleInsertIndicatorBarVisibility(bool8 invisible);
void SetBagVisualPocketId(u8);
void ShakeBagSprite(void);
const u32 *GetItemIconPic(u16 itemId);
const u32 *GetItemIconPalette(u16 itemId);
void CreateBagOrSatchelSprite(u8 animNum);
void CreateItemIconOnFindMessage(void);
void DestroyItemIconOnFindMessage(void);

#endif // GUARD_ITEM_MENU_ICONS

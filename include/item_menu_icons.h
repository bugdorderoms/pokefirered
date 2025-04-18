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
void DestroyItemMenuIcon(u32 a0);
void CreateItemMenuIcon(u32 itemId, u32 a0);
u32 AddItemIconObject(u32 tilesTag, u32 paletteTag, u32 itemId);
u32 AddItemIconObjectWithCustomObjectTemplate(const struct SpriteTemplate * origTemplate, u32 tilesTag, u32 paletteTag, u32 itemId);
void DestroyItemIconObj(struct Sprite *sprite, u32 tilesTag, u32 paletteTag);
void sub_80989A0(u32 itemId, u32 idx);
void ItemMenuIcons_MoveInsertIndicatorBar(s16 x, u32 y);
void ItemMenuIcons_ToggleInsertIndicatorBarVisibility(bool32 invisible);
void SetBagVisualPocketId(u32);
void ShakeBagSprite(void);
const u32 *GetItemIconPic(u32 itemId);
const u32 *GetItemIconPalette(u32 itemId);
void CreateBagOrSatchelSprite(u32 animNum);
void CreateItemIconOnFindMessage(void);
void DestroyItemIconOnFindMessage(void);

#endif // GUARD_ITEM_MENU_ICONS

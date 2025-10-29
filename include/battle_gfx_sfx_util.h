#ifndef GUARD_BATTLE_GFX_SFX_UTIL_H
#define GUARD_BATTLE_GFX_SFX_UTIL_H

void AllocateBattleSpritesData(void);
void FreeBattleSpritesData(void);
void SpriteCB_WaitForBattlerBallReleaseAnim(struct Sprite *sprite);
void SpriteCB_TrainerSlideIn(struct Sprite *sprite);
void InitAndLaunchChosenStatusAnimation(u32 battlerId, u32 animStatusId, u32 status);
bool32 TryHandleLaunchBattleTableAnimation(u32 battlerId, u32 tableId, u32 argument);
void InitAndLaunchSpecialAnimation(u32 attacker, u32 defender, u32 tableId);
bool32 IsBattleSEPlaying(u32 battlerId);
void BattleLoadMonSpriteGfx(u32 battlerId);
void DecompressGhostFrontPic(u32 battlerId);
void DecompressTrainerFrontPic(u32 frontPicId, u32 battlerId);
void DecompressTrainerBackPalette(u32 index, u32 palette);
void FreeTrainerFrontPicPaletteAndTile(u32 frontPicId);
bool32 BattleLoadAllHealthBoxesGfx(u32 state);
void LoadBattleBarGfx(void);
bool32 BattleInitAllSprites(u8 *state, u8 *battlerId);
void ClearSpritesHealthboxAnimData(void);
void CopyAllBattleSpritesInvisibilities(void);
void CopyBattleSpriteInvisibility(u32 battlerId);
void HandleSpeciesGfxDataChange(u32 battlerAtk, u32 battlerDef, u32 flags);
void BattleLoadSubstituteOrMonSpriteGfx(u32 battlerId, bool32 loadMonSprite);
void LoadBattleMonGfxAndAnimate(u32 battlerId, bool32 loadMonSprite, u32 spriteId);
void TrySetBehindSubstituteSpriteBit(u32 battlerId, u32 move);
void ClearBehindSubstituteBit(u32 battlerId);
void HandleLowHpMusicChange(u32 battlerId, struct Pokemon *mon);
void BattleStopLowHpSound(void);
void HandleBattleLowHpMusicChange(void);
void SetBattlerSpriteAffineMode(u32 affineMode);
void LoadAndCreateEnemyShadowSprites(void);
void SpriteCB_SetInvisible(struct Sprite *sprite);
void SetBattlerShadowSpriteCallback(u32 battlerId);
void HideBattlerShadowSprite(u32 battlerId);
void BattleInterfaceSetWindowPals(void);
void ClearTemporarySpeciesSpriteData(u32 battlerId, bool32 dontClearSubstitute);
void AllocateMonSpritesGfx(void);
void FreeMonSpritesGfx(void);
bool32 ShouldPlayNormalPokeCry(struct Pokemon *mon);
u32 GetBattlerVisualSpecies(u32 battlerId);

#endif // GUARD_BATTLE_GFX_SFX_UTIL_H

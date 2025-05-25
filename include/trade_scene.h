#ifndef GUARD_TRADE_SCENE_H
#define GUARD_TRADE_SCENE_H

#define TRADE_MON_NATURE(nature) (nature + 1)
#define TRADE_MON_TERA_TYPE(type) (type + 1)

void CB2_InitTradeAnim_LinkTrade(void);
void CreateInGameTradePokemon(void);
void DoInGameTradeScene(void);
void DrawTextOnTradeWindow(u32 windowId, const u8 *str, s8 speed);
u32 GetInGameTradeSpeciesInfo(void);
u32 GetTradeSpecies(void);
void InitTradeSequenceBgGpuRegs(void);
void LinkTradeDrawWindow(void);
void LoadTradeAnimGfx(void);

extern const u16 gTradeOrHatchMonShadowTilemap[];
extern const struct WindowTemplate gTradeEvolutionSceneYesNoWindowTemplate;

#endif //GUARD_TRADE_SCENE_H

#ifndef GUARD_TRADE_H
#define GUARD_TRADE_H

#include "global.h"
#include "link_rfu.h"
#include "constants/trade.h"

extern struct Mail gLinkPartnerMail[6];
extern u8 gSelectedTradeMonPositions[2];

extern const u8 gText_MaleSymbol4[];
extern const u8 gText_FemaleSymbol4[];
extern const u8 gText_GenderlessSymbol[];

extern const u16 gTradeOrHatchMonShadowTilemap[];
void CB2_ReturnFromLinkTrade(void);
s32 Trade_CalcLinkPlayerCompatibilityParam(void);
s32 CanRegisterMonForTradingBoard(struct GFtgtGnameSub rfuPlayer, u16 species2, u16 species);
s32 GetUnionRoomTradeMessageId(struct GFtgtGnameSub rfuPlayer, struct GFtgtGnameSub rfuPartner, u16 playerSpecies2, u16 partnerSpecies, u8 requestedType, u16 playerSpecies);
void CB2_ReturnToTradeMenuFromSummary(void);

#endif //GUARD_TRADE_H

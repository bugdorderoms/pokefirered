#include "global.h"
#include "battle_gimmicks.h"
#include "constants/moves.h"

////////////////////
// MEGA EVOLUTION //
////////////////////



////////////
// Z-MOVE //
////////////

bool32 IsZMove(u32 move)
{
	return (move >= FIRST_Z_MOVE && move <= LAST_Z_MOVE);
}

bool32 IsTypeBasedZMove(u32 move)
{
	return (move >= FIRST_Z_MOVE && move <= LAST_TYPED_Z_MOVE);
}

/////////////
// DYNAMAX //
/////////////

bool32 IsMaxMove(u32 move)
{
	return (move >= FIRST_GMAX_MOVE && move <= LAST_GMAX_MOVE);
}

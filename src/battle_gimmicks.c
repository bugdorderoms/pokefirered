#include "global.h"
#include "battle_gimmicks.h"
#include "constants/moves.h"

////////////////////
// MEGA EVOLUTION //
////////////////////



////////////
// Z-MOVE //
////////////

bool8 IsZMove(u16 move)
{
	return (move >= FIRST_Z_MOVE && move <= LAST_Z_MOVE);
}

/////////////
// DYNAMAX //
/////////////

bool8 IsMaxMove(u16 move)
{
	return (move >= FIRST_GMAX_MOVE && move <= LAST_GMAX_MOVE);
}

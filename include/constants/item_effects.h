#ifndef GUARD_CONSTANTS_ITEM_EFFECTS_H
#define GUARD_CONSTANTS_ITEM_EFFECTS_H

#define STORE_WORD(word)       \
	(word & 0x000000FF),       \
	(word & 0x0000FF00) >> 8,  \
	(word & 0x00FF0000) >> 16, \
	(word & 0xFF000000) >> 24

#define ITEMEFFECT_HEAL_HP           0x0  // Heals the mon HP by the given amount
#define ITEMEFFECT_CURE_STATUS       0x1  // Cures the given status mask of the mon
#define ITEMEFFECT_REVIVE            0x2  // Revives the mon, param is the HP divisor
#define ITEMEFFECT_CHANGE_FRIENDSHIP 0x3  // Changes the mon's friendship, param = See defines for it(acordly with it's value the next bytes are used to determine the increase/decrease amount)
#define ITEMEFFECT_RESTORE_PP        0x4  // Restore the mon's PP by the given amount
#define ITEMEFFECT_UP_DYNAMAX_LEVEL  0x5  // Increases the mon's dynamax level by the given amount
#define ITEMEFFECT_CHANGE_EV         0x6  // Changes the mon's EV of the specified stat by the given amount
#define ITEMEFFECT_GIVE_EXPERIENCE   0x7  // Gives the given amount of exp to the mon (if 0 it automatically levels up)
#define ITEMEFFECT_INCREASE_PP       0x8  // Increases the mon's PP, param is FALSE for PP Up or TRUE for PP Max
#define ITEMEFFECT_GIVE_GMAX_FACTOR  0x9  // Switches the mon's gigantamax factor bit
#define ITEMEFFECT_SET_TERA_TYPE     0x10 // Sets the mon's tera type to the given type
#define ITEMEFFECT_HEAL_HP_PERCENT   0x11 // Heals the mon HP by the given percentage amount
#define ITEMEFFECT_END               0xFF // Ends item effect

// defines for ITEMEFFECT_CHANGE_FRIENDSHIP
#define ITEMEFFECT_FRIENDSHIP_LOW    (1 << 0)
#define ITEMEFFECT_FRIENDSHIP_MID    (1 << 1)
#define ITEMEFFECT_FRIENDSHIP_HIGH   (1 << 2)
#define ITEMEFFECT_FRIENDSHIP_MAIN   (1 << 3)
#define ITEMEFFECT_FRIENDSHIP_ALL    (ITEMEFFECT_FRIENDSHIP_LOW | ITEMEFFECT_FRIENDSHIP_MID | ITEMEFFECT_FRIENDSHIP_HIGH)

// defines for ITEMEFFECT_INCREASE_PP
#define ITEMEFFECT_PP_UP  FALSE
#define ITEMEFFECT_PP_MAX TRUE

#define ITEM_EFFECT(...) (const u8[]) {__VA_ARGS__, ITEMEFFECT_END}

#define HEAL_HP(...)           \
	ITEMEFFECT_HEAL_HP,        \
	DEFAULT(0xFF, __VA_ARGS__)

#define CURE_STATUS(status1, ...)        \
	ITEMEFFECT_CURE_STATUS,              \
	ID_STATUS1,                          \
	status1,                             \
	ID_STATUS2,                          \
	STORE_WORD(DEFAULT(0, __VA_ARGS__)), \
	ITEMEFFECT_END // Signal to end loop

#define REVIVE(hpDivisor) \
	ITEMEFFECT_REVIVE,    \
	hpDivisor
	
#define CHANGE_FRIENDSHIP(mask, ...) \
	ITEMEFFECT_CHANGE_FRIENDSHIP,    \
	mask,                            \
	DEFAULT(0, __VA_ARGS__),         \
	DEFAULT_2(0, __VA_ARGS__),       \
	DEFAULT_3(0, __VA_ARGS__)

#define RESTORE_PP(amount) \
	ITEMEFFECT_RESTORE_PP, \
	amount

#define UP_DYNAMAX_LEVEL(amount) \
	ITEMEFFECT_UP_DYNAMAX_LEVEL, \
	amount

#define CHANGE_EV(statId, amount) \
	ITEMEFFECT_CHANGE_EV,         \
	statId,                       \
	amount

#define GIVE_EXP(...)                           \
	ITEMEFFECT_GIVE_EXPERIENCE,                 \
	(DEFAULT(0, __VA_ARGS__) & 0x000000FF),     \
	(DEFAULT(0, __VA_ARGS__) & 0x0000FF00) >> 8

#define INCREASE_PP(upOrMax) \
	ITEMEFFECT_INCREASE_PP,  \
	upOrMax

#define GIGANTAMAX_FACTOR_SWITCH \
	ITEMEFFECT_GIVE_GMAX_FACTOR

#define SET_TERA_TYPE(type)   \
	ITEMEFFECT_SET_TERA_TYPE, \
	type

#define HEAL_HP_PERCENT(percentage) \
	ITEMEFFECT_HEAL_HP_PERCENT,     \
	percentage

#endif // GUARD_CONSTANTS_ITEM_EFFECTS_H

#ifndef GUARD_CONSTANTS_ITEM_EFFECTS_H
#define GUARD_CONSTANTS_ITEM_EFFECTS_H

#define ITEMEFFECT_HEAL_HP           0x0  // itemHoldEffectParam = Heal amount
#define ITEMEFFECT_CURE_STATUS       0x1  // nextByte = See defines for it
#define ITEMEFFECT_REVIVE            0x2  // itemHoldEffectParam = HP divisor
#define ITEMEFFECT_CHANGE_FRIENDSHIP 0x3  // nextByte = See defines for it(acordly with it's value the next bytes are used to determine the increase/decrease amount)
#define ITEMEFFECT_RESTORE_PP        0x4  // itemHoldEffectParam = PP amount, nextByte = TRUE if restore all moves's PP or FALSE if only for the seected move
#define ITEMEFFECT_UP_DYNAMAX_LEVEL  0x5  // itemHoldEffectParam = Increase amount
#define ITEMEFFECT_CHANGE_EV         0x6  // itemHoldEffectParam = StatId, nextByte = Amount
#define ITEMEFFECT_GIVE_EXPERIENCE   0x7  // nextHWord = Exp amount(if 0 it automatically levels up)
#define ITEMEFFECT_INCREASE_PP       0x8  // nextByte = FALSE for PP Up or TRUE for PP Max
#define ITEMEFFECT_GIVE_GMAX_FACTOR  0x9  // No param
#define ITEMEFFECT_EVOLUTION         0x10 // No param
#define ITEMEFFECT_SET_TERA_TYPE     0x11 // itemHoldEffectParam = TypeId
#define ITEMEFFECT_HEAL_HP_PERCENT   0x12 // itemHoldEffectParam = Heal percent
#define ITEMEFFECT_END               0xFF

// defines for ITEMEFFECT_CURE_STATUS
#define ITEMEFFECT_STATUS_POISON     0x1
#define ITEMEFFECT_STATUS_BURN       0x2
#define ITEMEFFECT_STATUS_FREEZE     0x4
#define ITEMEFFECT_STATUS_SLEEP      0x8
#define ITEMEFFECT_STATUS_PARALYSIS  0x10
#define ITEMEFFECT_STATUS_CONFUSION  0x20
#define ITEMEFFECT_STATUS_LOVE       0x40
#define ITEMEFFECT_STATUS_ALL        (ITEMEFFECT_STATUS_POISON | ITEMEFFECT_STATUS_BURN | ITEMEFFECT_STATUS_FREEZE | ITEMEFFECT_STATUS_SLEEP \
                                     | ITEMEFFECT_STATUS_PARALYSIS | ITEMEFFECT_STATUS_CONFUSION)

// defines for ITEMEFFECT_CHANGE_FRIENDSHIP
#define ITEMEFFECT_FRIENDSHIP_LOW    0x1
#define ITEMEFFECT_FRIENDSHIP_MID    0x2
#define ITEMEFFECT_FRIENDSHIP_HIGH   0x4
#define ITEMEFFECT_FRIENDSHIP_MAIN   0x8

#define ITEMEFFECT_FRIENDSHIP_ALL    (ITEMEFFECT_FRIENDSHIP_LOW | ITEMEFFECT_FRIENDSHIP_MID | ITEMEFFECT_FRIENDSHIP_HIGH)

#endif // GUARD_CONSTANTS_ITEM_EFFECTS_H

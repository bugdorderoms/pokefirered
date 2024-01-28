#ifndef GUARD_CONSTANTS_FORM_CHANGE_H
#define GUARD_CONSTANTS_FORM_CHANGE_H

// form change based on the pokemon's gender.
// param = gender
#define FORM_CHANGE_GENDER        0x0000

// form change based on the pokemon's personality.
// param = nº forms (eg. Unown have 28 forms so the param is 28)
#define FORM_CHANGE_PERSONALITY   0x0001

// form change based on current season of the year.
// param = seasonId
#define FORM_CHANGE_SEASON        0x0002

// form change based on the pokemon's item hold.
// param = itemId (ITEM_NONE is used to determine it revert if no item held or any item other than the specifieds below)
// param2 is optionally passed to check for an specific pokemon's ability
#define FORM_CHANGE_HOLD_ITEM     0x0003

// form change based on terrain. (It's only used for player's party Burmy change on battle end)
// param = terrainId
#define FORM_CHANGE_TERRAIN       0x0004

// form change based on pokemon's HP in battle.
// param = HP divisor (eg. When HP drops to 50%(2), 25%(4), etc)
// param2 = HP_FORM_GT or HP_FORM_LS
#define FORM_CHANGE_HP            0x0005

// form change based on current weather in battle.
// param = weather
#define FORM_CHANGE_WEATHER       0x0006

// form change used internaly in battle when a pokemon is switched out.
// no param is necessary
// if defined in an species form change table the species don't revert form when switched out. (It's not necessary to define an species as target, unless the species have an switch out form that are handled separated like Palafin)
#define FORM_CHANGE_SWITCH_OUT    0x0007

// form change used internaly in battle when a pokemon faints.
// no param is necessary
// if defined in an species form change table the species don't revert form when faints. (It's not necessary to define an species as target)
#define FORM_CHANGE_FAINT         0x0008

// form change when the battle starts. (It will revert to it's original form when the battle ends)
// no param is necessary
#define FORM_CHANGE_START_BATTLE  0x0009

// form change used internaly in battle when it ends.
// no param or target species is necessary
#define FORM_CHANGE_END_BATTLE    0x000A

// form change based on the pokemon's nature.
// param = NATURE_FORM_AMPED or NATURE_FORM_LOW_KEY
#define FORM_CHANGE_NATURE        0x000B

// form change when the pokemon is send out to pc or withdraw from daycare.
// no param is necessary
#define FORM_CHANGE_WITHDRAW      0x000C

// form change based on current time. (It's only called when the battle start, it don't revert to it's original form when the battle ends)
// param = TIME_FORM_DAY or TIME_FORM_NIGHT
#define FORM_CHANGE_TIME          0x000D

// form change when the pokemon knows or not an move.
// param = moveId
// param2 = KNOW_MOVE_FORM_DONT_KNOW or KNOW_MOVE_FORM_KNOW
#define FORM_CHANGE_KNOW_MOVE     0x000E

// form change using a item on the pokemon.
// param = itemId
// param2 is optionally passed as an species if the pokemon can revert form if uses the item in it again
#define FORM_CHANGE_USE_ITEM      0x000F

// form change when a battle turn passes. (It will switch betwen the forms on each turn)
// param = actual species
#define FORM_CHANGE_ENDTURN       0x0010

// form change when the pokemon uses an move.
// param = moveId
#define FORM_CHANGE_MOVE          0x0011

// form change when the pokemon uses an move with split.
// param = split
// param2 = if TRUE it will transform when use a move with diferent split
#define FORM_CHANGE_MOVE_SPLIT    0x0012

// form change when the pokemon knocks out another.
// no param is necessary
#define FORM_CHANGE_FAINT_TARGET  0x0013

// battle mechanic forms. TODO:

// form change for gigantamax in battle.
// no param is necessary
#define FORM_CHANGE_GIGANTAMAX    0xFFFA

// form change for ultra burst in battle.
// param = itemId
#define FORM_CHANGE_ULTRA_BURST   0xFFFB

// form change for primal reversion in battle.
// param = itemId
#define FORM_CHANGE_PRIMAL        0xFFFC

// form change for mega evolution using a move in battle.
// param = moveId
#define FORM_CHANGE_MOVE_MEGA_EVO 0xFFFD

// form change for mega evolution in battle.
// param = itemId
#define FORM_CHANGE_MEGA_EVO      0xFFFE

// no form change, determine the end of the species's form change table
#define FORM_CHANGE_TERMINATOR    0xFFFF

// modes for FORM_CHANGE_HP
#define HP_FORM_GT 0
#define HP_FORM_LS 1

// modes for FORM_CHANGE_NATURE
#define NATURE_FORM_AMPED   0
#define NATURE_FORM_LOW_KEY 1

// modes for FORM_CHANGE_TIME
#define TIME_FORM_DAY   0
#define TIME_FORM_NIGHT 1

// modes for FORM_CHANGE_KNOW_MOVE
#define KNOW_MOVE_FORM_DONT_KNOW 0
#define KNOW_MOVE_FORM_KNOW      1

#endif

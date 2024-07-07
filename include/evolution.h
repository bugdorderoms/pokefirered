#ifndef GUARD_EVOLUTION_H
#define GUARD_EVOLUTION_H

#define EVO_TARGET_SPECIES             0x00 // Determine the target species
// Determines the type of evolution, (level up, item or trade)
#define EVO_REQ_LEVEL                  0x01 // Requires levels up or reach specified level to evolve
#define EVO_REQ_ITEM                   0x02 // Requires specific item be used on it to evolve
#define EVO_REQ_TRADE_WITH_SPECIES     0x03 // Requires being traded with specific species to evolve
// Evo requirement commands
#define EVO_REQ_FRIENDSHIP             0x04 // Requires high friendship to evolve
#define EVO_REQ_TIME                   0x05 // Requires specific time to evolve
#define EVO_REQ_STAT_X_STAT            0x06 // Requires one stat to be X compared to other stat to evolve
#define EVO_REQ_GENDER                 0x07 // Requires specific gender to evolve
#define EVO_REQ_ITEM_HOLD              0x08 // Requires specific hold item to evolve
#define EVO_REQ_MOVE                   0x09 // Requires specific move to evolve
#define EVO_REQ_MOVE_WITH_TYPE         0x0A // Requires move with specific type to evolve
#define EVO_REQ_DEFEAT_SPECIES_X_TIMES 0x0B // Requires defeat an species X times to evolve
#define EVO_REQ_WEATHER                0x0C // Requires specific weather in the overworld to evolve
#define EVO_REQ_SPECIES_IN_PARTY       0x0D // Requires specific species in party to evolve
#define EVO_REQ_SPECIES_WITH_TYPE      0x0E // Requires an species with specific type in party to evolve
#define EVO_REQ_X_CRITICAL_HITS        0x0F // Requires landing X critical hits to evolve
#define EVO_REQ_USE_MOVE_X_TIMES       0x10 // Requires use specific move X times to evolve
#define EVO_REQ_RECOIL_DAMAGE          0x11 // Requires being damaged by an specific amount of recoil damage to evolve
#define EVO_CREATE_SPECIES             0x12 // Creates an species in the party
#define EVO_REGIONAL_FORM              0x13 // Apply form change based on region
#define EVO_NATURE_FORM                0x14 // Apply form change based on nature
#define EVO_PERSONALITY_FORM           0x15 // Apply form change based on personality
#define EVO_REQ_MOON_PHASE             0x16 // Requires specific phase of the moon to evolve
#define EVO_REQ_DAMAGE_HP              0x17 // Requires being damaged by an specific amount to evolve
#define EVO_REQ_FOLLOW_STEPS           0x18 // Requires X steps following the player in the overworld to evolve
#define EVOLUTIONS_END                 0xFF // Marks the end of evolution table

// Evo requirement macros
#define EVO_TARGET(species)  \
	EVO_TARGET_SPECIES,      \
	(u8)species,             \
	(species & 0xFF00) >> 8

#define REQ_LEVEL(...)      \
    EVO_REQ_LEVEL,          \
	DEFAULT(0, __VA_ARGS__)

#define REQ_ITEM(item)   \
    EVO_REQ_ITEM,        \
    (u8)item,            \
	(item & 0xFF00) >> 8

#define REQ_TRADE_WITH_SPECIES(species) \
    EVO_REQ_TRADE_WITH_SPECIES,         \
	(u8)species,                        \
	(species & 0xFF00) >> 8

#define REQ_FRIENDSHIP \
    EVO_REQ_FRIENDSHIP

#define REQ_TIME(time) \
	EVO_REQ_TIME,      \
	time

#define REQ_STAT_X_STAT(stat1, cmp, stat2) \
	EVO_REQ_STAT_X_STAT,                   \
	stat1,                                 \
	cmp,                                   \
	stat2

#define REQ_GENDER(gender) \
    EVO_REQ_GENDER,        \
	gender

#define REQ_ITEM_HOLD(item) \
    EVO_REQ_ITEM_HOLD,      \
	(u8)item,               \
	(item & 0xFF00) >> 8

#define REQ_MOVE(move)   \
    EVO_REQ_MOVE,        \
	(u8)move,            \
	(move & 0xFF00) >> 8

#define REQ_MOVE_WITH_TYPE(type) \
    EVO_REQ_MOVE_WITH_TYPE,      \
	type

#define REQ_DEFEAT_SPECIES(species, xTimes) \
    EVO_REQ_DEFEAT_SPECIES_X_TIMES,         \
	(u8)species,                            \
	(species & 0xFF00) >> 8,                \
	xTimes

#define REQ_WEATHER(weatherBits) \
    EVO_REQ_WEATHER,             \
	weatherBits

#define REQ_SPECIES_IN_PARTY(species) \
    EVO_REQ_SPECIES_IN_PARTY,         \
	(u8)species,                      \
	(species & 0xFF00) >> 8

#define REQ_SPECIES_WITH_TYPE(type) \
    EVO_REQ_SPECIES_WITH_TYPE,      \
	type

#define REQ_X_CRITICAL_HITS(numCriticals) \
    EVO_REQ_X_CRITICAL_HITS,              \
	numCriticals

#define REQ_USE_MOVE_X_TIMES(move, xTimes) \
    EVO_REQ_USE_MOVE_X_TIMES,              \
	(u8)move,                              \
	(move & 0xFF00) >> 8,                  \
	xTimes

#define REQ_RECOIL_DAMAGE(damageAmount) \
    EVO_REQ_RECOIL_DAMAGE,              \
	(u8)damageAmount,                   \
	(damageAmount & 0xFF00) >> 8

// Evo special macros
#define CREATE_SPECIES(species) \
    EVO_CREATE_SPECIES,         \
	(u8)species,                \
	(species & 0xFF00) >> 8

#define REGIONAL_FORM \
    EVO_REGIONAL_FORM
	
#define NATURE_FORM \
    EVO_NATURE_FORM

#define PERSONALITY_FORM \
	EVO_PERSONALITY_FORM

#define REQ_MOON_PHASE(phase) \
    EVO_REQ_MOON_PHASE,       \
	phase

#define REQ_DAMAGE_HP(damageAmount) \
    EVO_REQ_DAMAGE_HP,              \
	(u8)damageAmount,               \
	(damageAmount & 0xFF00) >> 8

#define REQ_FOLLOW_STEPS(numSteps) \
    EVO_REQ_FOLLOW_STEPS,          \
	(u8)numSteps,                  \
	(numSteps & 0xFF00) >> 8

// Modes for GetEvolutionTargetSpecies
#define EVO_MODE_NORMAL         0 // Levels up
#define EVO_MODE_TRADE          1 // Trade
#define EVO_MODE_ITEM_USE       2 // Use item
#define EVO_MODE_ITEM_CHECK     3 // If an Everstone is being held, still want to show that the stone *could* be used on that Pokémon to evolve
#define EVO_MODE_BATTLE_SPECIAL 4 // Evolves without levels up

extern const u8 gEvolutionCmdArgumentsSize[];

#endif // GUARD_EVOLUTION_H

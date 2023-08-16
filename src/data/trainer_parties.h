// If no nature was given to it it'll be based on the generated personality.
#define TRAINER_MON_NATURE(nature) (nature + 1)

// If no ability was given to it it'll be based on the generated personality.
#define TRAINER_MON_ABILITY(abilityNum) (abilityNum + 1)

// If no nickname was given to it the pokemon will have its species name. Seting it as TRAINER_MON_NICKNAME("") will cause the pokemon to have no name.
#define TRAINER_MON_NICKNAME(str) (const u8[]) _(str)

// If the iv field are defined as TRAINER_MON_IVS(0, 0, 0, 0, 0, 0) the pokemon will have random ivs. So in't possible to have a Pokémon with zeroed ivs.
#define TRAINER_MON_IVS(hp, atk, def, speed, spatk, spdef) (hp | (atk << 5) | (def << 10) | (speed << 15) | (spatk << 20) | (spdef << 25))

#define TRAINER_MON_EVS(hp, atk, def, speed, spatk, spdef) ((const u8[NUM_STATS]) {hp, atk, def, spatk, spdef, speed})

/* Example of a fully customized trainer mon
static const struct TrainerMon sParty_BugCatcherRick[] = {
    {
        .iv = TRAINER_MON_IVS(31, 9, 3, 29, 31, 1),
        .lvl = 6,
        .species = SPECIES_WEEDLE,
		.nickname = TRAINER_MON_NICKNAME("Weed"),
		.ev = TRAINER_MON_EVS(252, 10, 10, 10, 10, 10),
		.heldItem = ITEM_POISON_BARB,
		.moves = {MOVE_STRING_SHOT, MOVE_TACKLE},
		.abilityNum = TRAINER_MON_ABILITY(2), // hidden
		.nature = TRAINER_MON_NATURE(NATURE_SASSY),
		.isShiny = TRUE,
		.ballId = ITEM_MASTER_BALL,
		.gender = TRAINER_MON_FEMALE,
    },
};*/

static const struct TrainerMon sParty_YoungsterBen[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 11,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 11,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMon sParty_YoungsterCalvin[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 14,
        .species = SPECIES_SPEAROW,
    },
};

static const struct TrainerMon sParty_YoungsterJosh[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 10,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 10,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 10,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMon sParty_YoungsterTimmy[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 14,
        .species = SPECIES_SANDSHREW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 14,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMon sParty_YoungsterJoey[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 15,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 15,
        .species = SPECIES_SPEAROW,
    },
};

static const struct TrainerMon sParty_YoungsterDan[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_SLOWPOKE,
    },
};

static const struct TrainerMon sParty_YoungsterChad[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 14,
        .species = SPECIES_EKANS,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 14,
        .species = SPECIES_SANDSHREW,
    },
};

static const struct TrainerMon sParty_YoungsterTyler[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_NIDORAN_M,
    },
};

static const struct TrainerMon sParty_YoungsterEddie[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMon sParty_YoungsterDillon[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_SANDSHREW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMon sParty_YoungsterYasu[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMon sParty_YoungsterDave[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_NIDORAN_M,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_NIDORINO,
    },
};

static const struct TrainerMon sParty_YoungsterBen2[] = {
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMon sParty_BugCatcherRick[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 6,
        .species = SPECIES_WEEDLE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 6,
        .species = SPECIES_CATERPIE,
    },
};

static const struct TrainerMon sParty_BugCatcherDoug[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 7,
        .species = SPECIES_WEEDLE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 7,
        .species = SPECIES_KAKUNA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 7,
        .species = SPECIES_WEEDLE,
    },
};

static const struct TrainerMon sParty_BugCatcherSammy[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 9,
        .species = SPECIES_WEEDLE,
    },
};

static const struct TrainerMon sParty_BugCatcherColton[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 10,
        .species = SPECIES_CATERPIE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 10,
        .species = SPECIES_WEEDLE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 10,
        .species = SPECIES_CATERPIE,
    },
};

static const struct TrainerMon sParty_BugCatcherGreg[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 9,
        .species = SPECIES_WEEDLE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 9,
        .species = SPECIES_KAKUNA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 9,
        .species = SPECIES_CATERPIE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 9,
        .species = SPECIES_METAPOD,
    },
};

static const struct TrainerMon sParty_BugCatcherJames[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 11,
        .species = SPECIES_CATERPIE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 11,
        .species = SPECIES_METAPOD,
    },
};

static const struct TrainerMon sParty_BugCatcherKent[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 11,
        .species = SPECIES_WEEDLE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 11,
        .species = SPECIES_KAKUNA,
    },
};

static const struct TrainerMon sParty_BugCatcherRobby[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 10,
        .species = SPECIES_CATERPIE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 10,
        .species = SPECIES_METAPOD,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 10,
        .species = SPECIES_CATERPIE,
    },
};

static const struct TrainerMon sParty_BugCatcherCale[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 10,
        .species = SPECIES_CATERPIE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 10,
        .species = SPECIES_WEEDLE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 10,
        .species = SPECIES_METAPOD,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 10,
        .species = SPECIES_KAKUNA,
    },
};

static const struct TrainerMon sParty_BugCatcherKeigo[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 16,
        .species = SPECIES_WEEDLE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 16,
        .species = SPECIES_CATERPIE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 16,
        .species = SPECIES_WEEDLE,
    },
};

static const struct TrainerMon sParty_BugCatcherElijah[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_BUTTERFREE,
    },
};

static const struct TrainerMon sParty_BugCatcherBrent[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_BEEDRILL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_BEEDRILL,
    },
};

static const struct TrainerMon sParty_BugCatcherConner[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_CATERPIE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_WEEDLE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_VENONAT,
    },
};

static const struct TrainerMon sParty_LassJanice[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 9,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 9,
        .species = SPECIES_PIDGEY,
    },
};

static const struct TrainerMon sParty_LassSally[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 10,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 10,
        .species = SPECIES_NIDORAN_F,
    },
};

static const struct TrainerMon sParty_LassRobin[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 14,
        .species = SPECIES_JIGGLYPUFF,
    },
};

static const struct TrainerMon sParty_LassCrissy[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_PARAS,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_PARAS,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_PARASECT,
    },
};

static const struct TrainerMon sParty_LassMiriam[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 11,
        .species = SPECIES_ODDISH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 11,
        .species = SPECIES_BELLSPROUT,
    },
};

static const struct TrainerMon sParty_LassIris[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 14,
        .species = SPECIES_CLEFAIRY,
    },
};

static const struct TrainerMon sParty_LassReli[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 16,
        .species = SPECIES_NIDORAN_M,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 16,
        .species = SPECIES_NIDORAN_F,
    },
};

static const struct TrainerMon sParty_LassAli[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 12,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 12,
        .species = SPECIES_ODDISH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 12,
        .species = SPECIES_BELLSPROUT,
    },
};

static const struct TrainerMon sParty_LassHaley[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 13,
        .species = SPECIES_ODDISH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 13,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 13,
        .species = SPECIES_ODDISH,
    },
};

static const struct TrainerMon sParty_LassAnn[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_NIDORAN_F,
    },
};

static const struct TrainerMon sParty_LassDawn[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_PIKACHU,
    },
};

static const struct TrainerMon sParty_LassPaige[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_NIDORAN_F,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_NIDORINA,
    },
};

static const struct TrainerMon sParty_LassAndrea[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_MEOWTH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_MEOWTH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_MEOWTH,
    },
};

static const struct TrainerMon sParty_LassMegan[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_NIDORAN_M,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_MEOWTH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_PIKACHU,
    },
};

static const struct TrainerMon sParty_LassJulia[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_CLEFAIRY,
    },
};

static const struct TrainerMon sParty_LassKay[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_BELLSPROUT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_WEEPINBELL,
    },
};

static const struct TrainerMon sParty_LassLisa[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_ODDISH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_GLOOM,
    },
};

static const struct TrainerMon sParty_SailorEdmond[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_MACHOP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_SHELLDER,
    },
};

static const struct TrainerMon sParty_SailorTrevor[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_MACHOP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_TENTACOOL,
    },
};

static const struct TrainerMon sParty_SailorLeonard[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_SHELLDER,
    },
};

static const struct TrainerMon sParty_SailorDuncan[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_HORSEA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_SHELLDER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_TENTACOOL,
    },
};

static const struct TrainerMon sParty_SailorHuey[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_TENTACOOL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_STARYU,
    },
};

static const struct TrainerMon sParty_SailorDylan[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_HORSEA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_HORSEA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_HORSEA,
    },
};

static const struct TrainerMon sParty_SailorPhillip[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_MACHOP,
    },
};

static const struct TrainerMon sParty_SailorDwayne[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_PIKACHU,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_PIKACHU,
    },
};

static const struct TrainerMon sParty_CamperLiam[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 10,
        .species = SPECIES_GEODUDE,
        .moves = {MOVE_TACKLE, MOVE_DEFENSE_CURL, MOVE_NONE, MOVE_NONE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 11,
        .species = SPECIES_SANDSHREW,
        .moves = {MOVE_SCRATCH, MOVE_DEFENSE_CURL, MOVE_SAND_ATTACK, MOVE_NONE},
    },
};

static const struct TrainerMon sParty_CamperShane[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 14,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 14,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMon sParty_CamperEthan[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_MANKEY,
    },
};

static const struct TrainerMon sParty_CamperRicky[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_SQUIRTLE,
    },
};

static const struct TrainerMon sParty_CamperJeff[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 16,
        .species = SPECIES_SPEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 16,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMon sParty_CamperChris[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_CHARMANDER,
    },
};

static const struct TrainerMon sParty_CamperDrew[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_SANDSHREW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_EKANS,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_SANDSHREW,
    },
};

static const struct TrainerMon sParty_PicnickerDiana[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_GOLDEEN,
    },
};

static const struct TrainerMon sParty_PicnickerNancy[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 16,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 16,
        .species = SPECIES_PIKACHU,
    },
};

static const struct TrainerMon sParty_PicnickerIsabelle[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 16,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 16,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 16,
        .species = SPECIES_PIDGEY,
    },
};

static const struct TrainerMon sParty_PicnickerKelsey[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 15,
        .species = SPECIES_NIDORAN_M,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 15,
        .species = SPECIES_NIDORAN_F,
    },
};

static const struct TrainerMon sParty_PicnickerAlicia[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_ODDISH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_BELLSPROUT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_ODDISH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_BELLSPROUT,
    },
};

static const struct TrainerMon sParty_PicnickerCaitlin[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_MEOWTH,
    },
};

static const struct TrainerMon sParty_PicnickerHeidi[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_PIKACHU,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_CLEFAIRY,
    },
};

static const struct TrainerMon sParty_PicnickerCarol[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMon sParty_PicnickerSofia[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_JIGGLYPUFF,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_MEOWTH,
    },
};

static const struct TrainerMon sParty_PicnickerMartha[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_ODDISH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_BULBASAUR,
    },
};

static const struct TrainerMon sParty_PicnickerTina[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_BULBASAUR,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_IVYSAUR,
    },
};

static const struct TrainerMon sParty_PicnickerHannah[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_MEOWTH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_PIKACHU,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_MEOWTH,
    },
};

static const struct TrainerMon sParty_PokemaniacMark[] = {
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_RHYHORN,
    },
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_LICKITUNG,
    },
};

static const struct TrainerMon sParty_PokemaniacHerman[] = {
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_CUBONE,
    },
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_SLOWPOKE,
    },
};

static const struct TrainerMon sParty_PokemaniacCooper[] = {
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_SLOWPOKE,
    },
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_SLOWPOKE,
    },
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_SLOWPOKE,
    },
};

static const struct TrainerMon sParty_PokemaniacSteve[] = {
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_CHARMANDER,
    },
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_CUBONE,
    },
};

static const struct TrainerMon sParty_PokemaniacWinston[] = {
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_SLOWPOKE,
    },
};

static const struct TrainerMon sParty_PokemaniacDawson[] = {
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 40,
        .species = SPECIES_CHARMELEON,
    },
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 40,
        .species = SPECIES_LAPRAS,
    },
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 40,
        .species = SPECIES_LICKITUNG,
    },
};

static const struct TrainerMon sParty_PokemaniacAshton[] = {
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_CUBONE,
    },
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_SLOWPOKE,
    },
};

static const struct TrainerMon sParty_SuperNerdJovan[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 11,
        .species = SPECIES_MAGNEMITE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 11,
        .species = SPECIES_VOLTORB,
    },
};

static const struct TrainerMon sParty_SuperNerdMiguel[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 12,
        .species = SPECIES_GRIMER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 12,
        .species = SPECIES_VOLTORB,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 12,
        .species = SPECIES_KOFFING,
    },
};

static const struct TrainerMon sParty_SuperNerdAidan[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SONIC_BOOM, MOVE_SCREECH, MOVE_TACKLE, MOVE_CHARGE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE, MOVE_POISON_GAS},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SONIC_BOOM, MOVE_SCREECH, MOVE_TACKLE, MOVE_CHARGE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_MAGNEMITE,
        .moves = {MOVE_SONIC_BOOM, MOVE_SUPERSONIC, MOVE_THUNDER_SHOCK, MOVE_TACKLE},
    },
};

static const struct TrainerMon sParty_SuperNerdGlenn[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_GRIMER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_MUK,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_GRIMER,
    },
};

static const struct TrainerMon sParty_SuperNerdLeslie[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SLUDGE, MOVE_SMOKESCREEN, MOVE_SMOG, MOVE_SELF_DESTRUCT},
    },
};

static const struct TrainerMon sParty_SuperNerdErik[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 36,
        .species = SPECIES_VULPIX,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 36,
        .species = SPECIES_VULPIX,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 36,
        .species = SPECIES_NINETALES,
    },
};

static const struct TrainerMon sParty_SuperNerdAvery[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_PONYTA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_CHARMANDER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_VULPIX,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_GROWLITHE,
    },
};

static const struct TrainerMon sParty_SuperNerdDerek[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 41,
        .species = SPECIES_RAPIDASH,
    },
};

static const struct TrainerMon sParty_SuperNerdZac[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_VULPIX,
    },
};

static const struct TrainerMon sParty_HikerMarcos[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 10,
        .species = SPECIES_GEODUDE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 10,
        .species = SPECIES_GEODUDE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 10,
        .species = SPECIES_ONIX,
    },
};

static const struct TrainerMon sParty_HikerFranklin[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 15,
        .species = SPECIES_MACHOP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 15,
        .species = SPECIES_GEODUDE,
    },
};

static const struct TrainerMon sParty_HikerNob[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 13,
        .species = SPECIES_GEODUDE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 13,
        .species = SPECIES_GEODUDE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 13,
        .species = SPECIES_MACHOP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 13,
        .species = SPECIES_GEODUDE,
    },
};

static const struct TrainerMon sParty_HikerWayne[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_ONIX,
    },
};

static const struct TrainerMon sParty_HikerAlan[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_GEODUDE,
        .moves = {MOVE_MAGNITUDE, MOVE_ROCK_THROW, MOVE_MUD_SPORT, MOVE_DEFENSE_CURL},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_ONIX,
        .moves = {MOVE_HARDEN, MOVE_ROCK_THROW, MOVE_BIND, MOVE_SCREECH},
    },
};

static const struct TrainerMon sParty_HikerBrice[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_GEODUDE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_MACHOP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_GEODUDE,
    },
};

static const struct TrainerMon sParty_HikerClark[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_GEODUDE,
        .moves = {MOVE_MAGNITUDE, MOVE_ROCK_THROW, MOVE_MUD_SPORT, MOVE_DEFENSE_CURL},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_ONIX,
        .moves = {MOVE_HARDEN, MOVE_ROCK_THROW, MOVE_BIND, MOVE_SCREECH},
    },
};

static const struct TrainerMon sParty_HikerTrent[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_ONIX,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_GRAVELER,
    },
};

static const struct TrainerMon sParty_HikerDudley[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_GEODUDE,
        .moves = {MOVE_MAGNITUDE, MOVE_ROCK_THROW, MOVE_MUD_SPORT, MOVE_DEFENSE_CURL},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_GEODUDE,
        .moves = {MOVE_MAGNITUDE, MOVE_ROCK_THROW, MOVE_MUD_SPORT, MOVE_DEFENSE_CURL},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_GRAVELER,
        .moves = {MOVE_MAGNITUDE, MOVE_ROCK_THROW, MOVE_MUD_SPORT, MOVE_DEFENSE_CURL},
    },
};

static const struct TrainerMon sParty_HikerAllen[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_GEODUDE,
        .moves = {MOVE_MAGNITUDE, MOVE_ROCK_THROW, MOVE_MUD_SPORT, MOVE_DEFENSE_CURL},
    },
};

static const struct TrainerMon sParty_HikerEric[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_MACHOP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_ONIX,
    },
};

static const struct TrainerMon sParty_HikerLenny[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_GEODUDE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_MACHOP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_GEODUDE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_GEODUDE,
    },
};

static const struct TrainerMon sParty_HikerOliver[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_ONIX,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_ONIX,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_GEODUDE,
    },
};

static const struct TrainerMon sParty_HikerLucas[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_GEODUDE,
        .moves = {MOVE_MAGNITUDE, MOVE_ROCK_THROW, MOVE_MUD_SPORT, MOVE_DEFENSE_CURL},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_GRAVELER,
        .moves = {MOVE_MAGNITUDE, MOVE_ROCK_THROW, MOVE_MUD_SPORT, MOVE_DEFENSE_CURL},
    },
};

static const struct TrainerMon sParty_BikerJared[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_POISON_GAS},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
};

static const struct TrainerMon sParty_BikerMalik[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_SCREECH, MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE},
    },
};

static const struct TrainerMon sParty_BikerErnest[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE, MOVE_POUND},
    },
};

static const struct TrainerMon sParty_BikerAlex[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_SCREECH, MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
};

static const struct TrainerMon sParty_BikerLao[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_SCREECH, MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
};

static const struct TrainerMon sParty_BikerHideo[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_WEEZING,
    },
};

static const struct TrainerMon sParty_BikerRuben[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
};

static const struct TrainerMon sParty_BikerBilly[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_MUK,
    },
};

static const struct TrainerMon sParty_BikerNikolas[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SPARK, MOVE_SONIC_BOOM, MOVE_SCREECH, MOVE_CHARGE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SPARK, MOVE_SONIC_BOOM, MOVE_SCREECH, MOVE_CHARGE},
    },
};

static const struct TrainerMon sParty_BikerJaxon[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SLUDGE, MOVE_SMOKESCREEN, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_MUK,
        .moves = {MOVE_SCREECH, MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE},
    },
};

static const struct TrainerMon sParty_BikerWilliam[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SELF_DESTRUCT, MOVE_TACKLE},
    },
};

static const struct TrainerMon sParty_BikerLukas[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_SCREECH, MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
};

static const struct TrainerMon sParty_BikerIsaac[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_SCREECH, MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_SCREECH, MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
};

static const struct TrainerMon sParty_BikerGerald[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_MUK,
        .moves = {MOVE_SCREECH, MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE},
    },
};

static const struct TrainerMon sParty_BurglarQuinn[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 36,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 36,
        .species = SPECIES_VULPIX,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 36,
        .species = SPECIES_NINETALES,
    },
};

static const struct TrainerMon sParty_BurglarRamon[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 41,
        .species = SPECIES_PONYTA,
    },
};

static const struct TrainerMon sParty_BurglarDusty[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_VULPIX,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_GROWLITHE,
    },
};

static const struct TrainerMon sParty_BurglarArnie[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_CHARMANDER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_CHARMELEON,
    },
};

static const struct TrainerMon sParty_BurglarSimon[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_NINETALES,
    },
};

static const struct TrainerMon sParty_BurglarLewis[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_PONYTA,
    },
};

static const struct TrainerMon sParty_EngineerBaily[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_VOLTORB,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_MAGNEMITE,
    },
};

static const struct TrainerMon sParty_EngineerBraxton[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_MAGNEMITE,
    },
};

static const struct TrainerMon sParty_EngineerBernie[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_MAGNEMITE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_MAGNEMITE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_MAGNETON,
    },
};

static const struct TrainerMon sParty_FishermanDale[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_GOLDEEN,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_TENTACOOL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_GOLDEEN,
    },
};

static const struct TrainerMon sParty_FishermanBarny[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_TENTACOOL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_STARYU,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_SHELLDER,
    },
};

static const struct TrainerMon sParty_FishermanNed[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_GOLDEEN,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_POLIWAG,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_GOLDEEN,
    },
};

static const struct TrainerMon sParty_FishermanChip[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_TENTACOOL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_GOLDEEN,
    },
};

static const struct TrainerMon sParty_FishermanHank[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_GOLDEEN,
    },
};

static const struct TrainerMon sParty_FishermanElliot[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_POLIWAG,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_SHELLDER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_GOLDEEN,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_HORSEA,
    },
};

static const struct TrainerMon sParty_FishermanRonald[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_SEAKING,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_GOLDEEN,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_SEAKING,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMon sParty_FishermanClaude[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_SHELLDER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_CLOYSTER,
    },
};

static const struct TrainerMon sParty_FishermanWade[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_MAGIKARP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_MAGIKARP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_MAGIKARP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_MAGIKARP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_MAGIKARP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_MAGIKARP,
    },
};

static const struct TrainerMon sParty_FishermanNolan[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_SEAKING,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_GOLDEEN,
    },
};

static const struct TrainerMon sParty_FishermanAndrew[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_MAGIKARP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_MAGIKARP,
    },
};

static const struct TrainerMon sParty_SwimmerMaleLuis[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 16,
        .species = SPECIES_HORSEA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 16,
        .species = SPECIES_SHELLDER,
    },
};

static const struct TrainerMon sParty_SwimmerMaleRichard[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_TENTACOOL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_SHELLDER,
    },
};

static const struct TrainerMon sParty_SwimmerMaleReece[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_GOLDEEN,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_HORSEA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_STARYU,
    },
};

static const struct TrainerMon sParty_SwimmerMaleMatthew[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_POLIWAG,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_POLIWHIRL,
    },
};

static const struct TrainerMon sParty_SwimmerMaleDouglas[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_HORSEA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_TENTACOOL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_TENTACOOL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_GOLDEEN,
    },
};

static const struct TrainerMon sParty_SwimmerMaleDavid[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_GOLDEEN,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_SHELLDER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMon sParty_SwimmerMaleTony[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_HORSEA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_HORSEA,
    },
};

static const struct TrainerMon sParty_SwimmerMaleAxle[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_TENTACOOL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_TENTACOOL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_STARYU,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_HORSEA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_TENTACRUEL,
    },
};

static const struct TrainerMon sParty_SwimmerMaleBarry[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_SHELLDER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_CLOYSTER,
    },
};

static const struct TrainerMon sParty_SwimmerMaleDean[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_STARYU,
    },
};

static const struct TrainerMon sParty_SwimmerMaleDarrin[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_HORSEA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_HORSEA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_SEADRA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_HORSEA,
    },
};

static const struct TrainerMon sParty_SwimmerMaleSpencer[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_SEADRA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_TENTACRUEL,
    },
};

static const struct TrainerMon sParty_SwimmerMaleJack[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_STARMIE,
    },
};

static const struct TrainerMon sParty_SwimmerMaleJerome[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_STARYU,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_WARTORTLE,
    },
};

static const struct TrainerMon sParty_SwimmerMaleRoland[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 32,
        .species = SPECIES_POLIWHIRL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 32,
        .species = SPECIES_TENTACOOL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 32,
        .species = SPECIES_SEADRA,
    },
};

static const struct TrainerMon sParty_CueBallKoji[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_MACHOP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_MANKEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_MACHOP,
    },
};

static const struct TrainerMon sParty_CueBallLuke[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_MANKEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_MACHOP,
    },
};

static const struct TrainerMon sParty_CueBallCamron[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_MANKEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_MACHOP,
    },
};

static const struct TrainerMon sParty_CueBallRaul[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_MANKEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_PRIMEAPE,
    },
};

static const struct TrainerMon sParty_CueBallIsaiah[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_MACHOP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_MACHAMP,
    },
};

static const struct TrainerMon sParty_CueBallZeek[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_MACHOKE,
    },
};

static const struct TrainerMon sParty_CueBallJamal[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_MANKEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_MANKEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_MACHAMP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_MACHOP,
    },
};

static const struct TrainerMon sParty_CueBallCorey[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_PRIMEAPE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_MACHOKE,
    },
};

static const struct TrainerMon sParty_CueBallChase[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_TENTACOOL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_TENTACOOL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_TENTACRUEL,
    },
};

static const struct TrainerMon sParty_GamerHugo[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_POLIWAG,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_HORSEA,
    },
};

static const struct TrainerMon sParty_GamerJasper[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_BELLSPROUT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_ODDISH,
    },
};

static const struct TrainerMon sParty_GamerDirk[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_VOLTORB,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_MAGNEMITE,
    },
};

static const struct TrainerMon sParty_GamerDarian[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_VULPIX,
    },
};

static const struct TrainerMon sParty_GamerStan[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_POLIWAG,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_POLIWAG,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_POLIWHIRL,
    },
};

static const struct TrainerMon sParty_GamerRich[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_VULPIX,
    },
};

static const struct TrainerMon sParty_BeautyBridget[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_ODDISH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_BELLSPROUT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_ODDISH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_BELLSPROUT,
    },
};

static const struct TrainerMon sParty_BeautyTamia[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_BELLSPROUT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_BELLSPROUT,
    },
};

static const struct TrainerMon sParty_BeautyLori[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_EXEGGCUTE,
    },
};

static const struct TrainerMon sParty_BeautyLola[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_PIKACHU,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_RATTATA,
    },
};

static const struct TrainerMon sParty_BeautySheila[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_MEOWTH,
    },
};

static const struct TrainerMon sParty_SwimmerFemaleTiffany[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMon sParty_SwimmerFemaleNora[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_SHELLDER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_SHELLDER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_CLOYSTER,
    },
};

static const struct TrainerMon sParty_SwimmerFemaleMelissa[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_POLIWAG,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMon sParty_BeautyGrace[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_WIGGLYTUFF,
    },
};

static const struct TrainerMon sParty_BeautyOlivia[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_BULBASAUR,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_IVYSAUR,
    },
};

static const struct TrainerMon sParty_BeautyLauren[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_WEEPINBELL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_BELLSPROUT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_WEEPINBELL,
    },
};

static const struct TrainerMon sParty_SwimmerFemaleAnya[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_POLIWAG,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_GOLDEEN,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_SEAKING,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_GOLDEEN,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_POLIWAG,
    },
};

static const struct TrainerMon sParty_SwimmerFemaleAlice[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_GOLDEEN,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMon sParty_SwimmerFemaleConnie[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_STARYU,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_STARYU,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_STARYU,
    },
};

static const struct TrainerMon sParty_SwimmerFemaleShirley[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_SEADRA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_HORSEA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_SEADRA,
    },
};

static const struct TrainerMon sParty_PsychicJohan[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_KADABRA,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_SLOWPOKE,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_MR_MIME,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_KADABRA,
    },
};

static const struct TrainerMon sParty_PsychicTyron[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_MR_MIME,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_KADABRA,
    },
};

static const struct TrainerMon sParty_PsychicCameron[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_SLOWPOKE,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_SLOWPOKE,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_SLOWBRO,
    },
};

static const struct TrainerMon sParty_PsychicPreston[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_SLOWBRO,
    },
};

static const struct TrainerMon sParty_RockerRandall[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_VOLTORB,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_MAGNEMITE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_VOLTORB,
    },
};

static const struct TrainerMon sParty_RockerLuca[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_VOLTORB,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_ELECTRODE,
    },
};

static const struct TrainerMon sParty_JugglerDalton[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_KADABRA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_MR_MIME,
    },
};

static const struct TrainerMon sParty_JugglerNelson[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 41,
        .species = SPECIES_DROWZEE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 41,
        .species = SPECIES_HYPNO,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 41,
        .species = SPECIES_KADABRA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 41,
        .species = SPECIES_KADABRA,
    },
};

static const struct TrainerMon sParty_JugglerKirk[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_DROWZEE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_DROWZEE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_KADABRA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_DROWZEE,
    },
};

static const struct TrainerMon sParty_JugglerShawn[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_DROWZEE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_HYPNO,
    },
};

static const struct TrainerMon sParty_JugglerGregory[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_MR_MIME,
        .moves = {MOVE_PSYCHIC, MOVE_ROLE_PLAY, MOVE_DOUBLE_SLAP, MOVE_ENCORE},
    },
};

static const struct TrainerMon sParty_JugglerEdward[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 46,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SWIFT, MOVE_LIGHT_SCREEN, MOVE_SPARK, MOVE_SONIC_BOOM},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 46,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SWIFT, MOVE_LIGHT_SCREEN, MOVE_SPARK, MOVE_SONIC_BOOM},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_ELECTRODE,
        .moves = {MOVE_SWIFT, MOVE_SPARK, MOVE_SELF_DESTRUCT, MOVE_SONIC_BOOM},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_MR_MIME,
        .moves = {MOVE_PSYCHIC, MOVE_ROLE_PLAY, MOVE_REFLECT, MOVE_ENCORE},
    },
};

static const struct TrainerMon sParty_JugglerKayden[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_HYPNO,
    },
};

static const struct TrainerMon sParty_JugglerNate[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_DROWZEE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_KADABRA,
    },
};

static const struct TrainerMon sParty_TamerPhil[] = {
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_SANDSLASH,
    },
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_ARBOK,
    },
};

static const struct TrainerMon sParty_TamerEdgar[] = {
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_ARBOK,
    },
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_SANDSLASH,
    },
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_ARBOK,
    },
};

static const struct TrainerMon sParty_TamerJason[] = {
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 43,
        .species = SPECIES_RHYHORN,
    },
};

static const struct TrainerMon sParty_TamerCole[] = {
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 39,
        .species = SPECIES_ARBOK,
    },
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 39,
        .species = SPECIES_TAUROS,
    },
};

static const struct TrainerMon sParty_TamerVincent[] = {
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 44,
        .species = SPECIES_PERSIAN,
    },
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 44,
        .species = SPECIES_GOLDUCK,
    },
};

static const struct TrainerMon sParty_TamerJohn[] = {
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_RHYHORN,
    },
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_PRIMEAPE,
    },
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_ARBOK,
    },
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_TAUROS,
    },
};

static const struct TrainerMon sParty_BirdKeeperSebastian[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMon sParty_BirdKeeperPerry[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_SPEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_SPEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_SPEAROW,
    },
};

static const struct TrainerMon sParty_BirdKeeperRobert[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_SPEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMon sParty_BirdKeeperDonald[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_FARFETCHD,
    },
};

static const struct TrainerMon sParty_BirdKeeperBenny[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_SPEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMon sParty_BirdKeeperEdwin[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_FARFETCHD,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_DODUO,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_PIDGEY,
    },
};

static const struct TrainerMon sParty_BirdKeeperChester[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_DODRIO,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_DODUO,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_DODUO,
    },
};

static const struct TrainerMon sParty_BirdKeeperWilton[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_SPEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMon sParty_BirdKeeperRamiro[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_DODRIO,
    },
};

static const struct TrainerMon sParty_BirdKeeperJacob[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_SPEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_SPEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_SPEAROW,
    },
};

static const struct TrainerMon sParty_BirdKeeperRoger[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMon sParty_BirdKeeperReed[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 39,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 39,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 39,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 39,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMon sParty_BirdKeeperKeith[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 39,
        .species = SPECIES_FARFETCHD,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 39,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMon sParty_BirdKeeperCarter[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_DODUO,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMon sParty_BirdKeeperMitch[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_SPEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMon sParty_BirdKeeperBeck[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMon sParty_BirdKeeperMarlon[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_SPEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_DODUO,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMon sParty_BlackBeltKoichi[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_BlackBeltMike[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_MANKEY,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_MANKEY,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_BlackBeltHideki[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 32,
        .species = SPECIES_MACHOP,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 32,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_BlackBeltAaron[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 36,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_BlackBeltHitoshi[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_MACHOP,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_MANKEY,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_BlackBeltAtsushi[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 40,
        .species = SPECIES_MACHOP,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 40,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_BlackBeltKiyo[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 43,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_BlackBeltTakashi[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_MACHOP,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_BlackBeltDaisuke[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 43,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 43,
        .species = SPECIES_MACHOP,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 43,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_RivalOaksLabSquirtle[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 5,
        .species = SPECIES_SQUIRTLE,
    },
};

static const struct TrainerMon sParty_RivalOaksLabBulbasaur[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 5,
        .species = SPECIES_BULBASAUR,
    },
};

static const struct TrainerMon sParty_RivalOaksLabCharmander[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 5,
        .species = SPECIES_CHARMANDER,
    },
};

static const struct TrainerMon sParty_RivalRoute22EarlySquirtle[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 9,
        .species = SPECIES_PIDGEY,
        .moves = {MOVE_TACKLE, MOVE_SAND_ATTACK, MOVE_NONE, MOVE_NONE},
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 9,
        .species = SPECIES_SQUIRTLE,
        .moves = {MOVE_TACKLE, MOVE_TAIL_WHIP, MOVE_NONE, MOVE_NONE},
    },
};

static const struct TrainerMon sParty_RivalRoute22EarlyBulbasaur[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 9,
        .species = SPECIES_PIDGEY,
        .moves = {MOVE_TACKLE, MOVE_SAND_ATTACK, MOVE_NONE, MOVE_NONE},
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 9,
        .species = SPECIES_BULBASAUR,
        .moves = {MOVE_TACKLE, MOVE_GROWL, MOVE_NONE, MOVE_NONE},
    },
};

static const struct TrainerMon sParty_RivalRoute22EarlyCharmander[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 9,
        .species = SPECIES_PIDGEY,
        .moves = {MOVE_TACKLE, MOVE_SAND_ATTACK, MOVE_NONE, MOVE_NONE},
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 9,
        .species = SPECIES_CHARMANDER,
        .moves = {MOVE_SCRATCH, MOVE_GROWL, MOVE_NONE, MOVE_NONE},
    },
};

static const struct TrainerMon sParty_RivalCeruleanSquirtle[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_PIDGEOTTO,
        .moves = {MOVE_TACKLE, MOVE_SAND_ATTACK, MOVE_GUST, MOVE_QUICK_ATTACK},
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 16,
        .species = SPECIES_ABRA,
        .moves = {MOVE_TELEPORT, MOVE_NONE, MOVE_NONE, MOVE_NONE},
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 15,
        .species = SPECIES_RATTATA,
        .moves = {MOVE_TACKLE, MOVE_TAIL_WHIP, MOVE_QUICK_ATTACK, MOVE_NONE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_SQUIRTLE,
        .moves = {MOVE_TACKLE, MOVE_TAIL_WHIP, MOVE_WITHDRAW, MOVE_WATER_GUN},
    },
};

static const struct TrainerMon sParty_RivalCeruleanBulbasaur[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_PIDGEOTTO,
        .moves = {MOVE_TACKLE, MOVE_SAND_ATTACK, MOVE_GUST, MOVE_QUICK_ATTACK},
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 16,
        .species = SPECIES_ABRA,
        .moves = {MOVE_TELEPORT, MOVE_NONE, MOVE_NONE, MOVE_NONE},
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 15,
        .species = SPECIES_RATTATA,
        .moves = {MOVE_TACKLE, MOVE_TAIL_WHIP, MOVE_QUICK_ATTACK, MOVE_NONE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_BULBASAUR,
        .moves = {MOVE_SLEEP_POWDER, MOVE_POISON_POWDER, MOVE_VINE_WHIP, MOVE_LEECH_SEED},
    },
};

static const struct TrainerMon sParty_RivalCeruleanCharmander[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_PIDGEOTTO,
        .moves = {MOVE_TACKLE, MOVE_SAND_ATTACK, MOVE_GUST, MOVE_QUICK_ATTACK},
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 16,
        .species = SPECIES_ABRA,
        .moves = {MOVE_TELEPORT, MOVE_NONE, MOVE_NONE, MOVE_NONE},
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 15,
        .species = SPECIES_RATTATA,
        .moves = {MOVE_TACKLE, MOVE_TAIL_WHIP, MOVE_QUICK_ATTACK, MOVE_NONE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_CHARMANDER,
        .moves = {MOVE_METAL_CLAW, MOVE_EMBER, MOVE_GROWL, MOVE_SCRATCH},
    },
};

static const struct TrainerMon sParty_ScientistTed[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_ELECTRODE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_WEEZING,
    },
};

static const struct TrainerMon sParty_ScientistConnor[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_SCREECH, MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_SELF_DESTRUCT},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
};

static const struct TrainerMon sParty_ScientistJerry[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_MAGNEMITE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_VOLTORB,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_MAGNETON,
    },
};

static const struct TrainerMon sParty_ScientistJose[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_ELECTRODE,
        .moves = {MOVE_SPARK, MOVE_SONIC_BOOM, MOVE_SCREECH, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
};

static const struct TrainerMon sParty_ScientistRodney[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_ELECTRODE,
    },
};

static const struct TrainerMon sParty_ScientistBeau[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_MAGNETON,
        .moves = {MOVE_SPARK, MOVE_THUNDER_WAVE, MOVE_SONIC_BOOM, MOVE_SUPERSONIC},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_MAGNEMITE,
        .moves = {MOVE_SPARK, MOVE_THUNDER_WAVE, MOVE_SONIC_BOOM, MOVE_SUPERSONIC},
    },
};

static const struct TrainerMon sParty_ScientistTaylor[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SPARK, MOVE_SONIC_BOOM, MOVE_SCREECH, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_MAGNETON,
        .moves = {MOVE_THUNDER_WAVE, MOVE_SONIC_BOOM, MOVE_SUPERSONIC, MOVE_THUNDER_SHOCK},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_MAGNEMITE,
        .moves = {MOVE_THUNDER_WAVE, MOVE_SONIC_BOOM, MOVE_SUPERSONIC, MOVE_THUNDER_SHOCK},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_SELF_DESTRUCT},
    },
};

static const struct TrainerMon sParty_ScientistJoshua[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_ELECTRODE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_MUK,
    },
};

static const struct TrainerMon sParty_ScientistParker[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_GRIMER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_ELECTRODE,
    },
};

static const struct TrainerMon sParty_ScientistEd[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SPARK, MOVE_SONIC_BOOM, MOVE_SCREECH, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_TACKLE, MOVE_SMOG},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_MAGNETON,
        .moves = {MOVE_SPARK, MOVE_THUNDER_WAVE, MOVE_SONIC_BOOM, MOVE_SUPERSONIC},
    },
};

static const struct TrainerMon sParty_ScientistTravis[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_MAGNEMITE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_KOFFING,
    },
};

static const struct TrainerMon sParty_ScientistBraydon[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_MAGNEMITE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_MAGNETON,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_VOLTORB,
    },
};

static const struct TrainerMon sParty_ScientistIvan[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_MAGNEMITE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_ELECTRODE,
    },
};

static const struct TrainerMon sParty_BossGiovanni[] = {
    {
        .iv = TRAINER_MON_IVS(26, 7, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_ONIX,
    },
    {
        .iv = TRAINER_MON_IVS(26, 7, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_RHYHORN,
    },
    {
        .iv = TRAINER_MON_IVS(26, 7, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_KANGASKHAN,
    },
};

static const struct TrainerMon sParty_BossGiovanni2[] = {
    {
        .iv = TRAINER_MON_IVS(26, 7, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_NIDORINO,
    },
    {
        .iv = TRAINER_MON_IVS(26, 7, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_KANGASKHAN,
    },
    {
        .iv = TRAINER_MON_IVS(26, 7, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_RHYHORN,
    },
    {
        .iv = TRAINER_MON_IVS(26, 7, 0, 0, 0, 0),
        .lvl = 41,
        .species = SPECIES_NIDOQUEEN,
    },
};

static const struct TrainerMon sParty_LeaderGiovanni[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 45,
        .species = SPECIES_RHYHORN,
        .moves = {MOVE_TAKE_DOWN, MOVE_ROCK_BLAST, MOVE_SCARY_FACE, MOVE_EARTHQUAKE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_DUGTRIO,
        .moves = {MOVE_SLASH, MOVE_SAND_TOMB, MOVE_MUD_SLAP, MOVE_EARTHQUAKE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 44,
        .species = SPECIES_NIDOQUEEN,
        .moves = {MOVE_BODY_SLAM, MOVE_DOUBLE_KICK, MOVE_POISON_STING, MOVE_EARTHQUAKE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 45,
        .species = SPECIES_NIDOKING,
        .moves = {MOVE_THRASH, MOVE_DOUBLE_KICK, MOVE_POISON_STING, MOVE_EARTHQUAKE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_RHYHORN,
        .moves = {MOVE_TAKE_DOWN, MOVE_ROCK_BLAST, MOVE_SCARY_FACE, MOVE_EARTHQUAKE},
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 13,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 13,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt2[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 11,
        .species = SPECIES_SANDSHREW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 11,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 11,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt3[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 11,
        .species = SPECIES_ZUBAT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 11,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt4[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 13,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 13,
        .species = SPECIES_SANDSHREW,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt5[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_MACHOP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_DROWZEE,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt6[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 15,
        .species = SPECIES_EKANS,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 15,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt7[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt8[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_DROWZEE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_MACHOP,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt9[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt10[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE, MOVE_HARDEN},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOG, MOVE_TACKLE, MOVE_POISON_GAS, MOVE_NONE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOG, MOVE_TACKLE, MOVE_POISON_GAS, MOVE_NONE},
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt11[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_RATTATA,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt12[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE, MOVE_HARDEN},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE, MOVE_POISON_GAS},
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt13[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_ZUBAT,
        .moves = {MOVE_BITE, MOVE_ASTONISH, MOVE_SUPERSONIC, MOVE_LEECH_LIFE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOG, MOVE_TACKLE, MOVE_POISON_GAS, MOVE_NONE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_SLUDGE, MOVE_DISABLE, MOVE_HARDEN, MOVE_POUND},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_ZUBAT,
        .moves = {MOVE_BITE, MOVE_ASTONISH, MOVE_SUPERSONIC, MOVE_LEECH_LIFE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_RATICATE,
        .moves = {MOVE_HYPER_FANG, MOVE_QUICK_ATTACK, MOVE_TAIL_WHIP, MOVE_TACKLE},
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt14[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_DROWZEE,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt15[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_MACHOP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_MACHOP,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt16[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_SANDSHREW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_EKANS,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_SANDSLASH,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt17[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_EKANS,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_SANDSHREW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_ARBOK,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt18[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_KOFFING,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt19[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_ZUBAT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_ZUBAT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_GOLBAT,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt20[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_KOFFING,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_DROWZEE,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt21[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_ZUBAT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt22[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_DROWZEE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_KOFFING,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt23[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_CUBONE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt24[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_GOLBAT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_ZUBAT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_ZUBAT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt25[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_HYPNO,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt26[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_MACHOP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_DROWZEE,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt27[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_EKANS,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_ZUBAT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_CUBONE,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt28[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_ARBOK,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt29[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_HYPNO,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt30[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_MACHOP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_MACHOKE,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt31[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_ZUBAT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_ZUBAT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_GOLBAT,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt32[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_RATICATE,
        .moves = {MOVE_SCARY_FACE, MOVE_HYPER_FANG, MOVE_QUICK_ATTACK, MOVE_TAIL_WHIP},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_ARBOK,
        .moves = {MOVE_GLARE, MOVE_BITE, MOVE_POISON_STING, MOVE_LEER},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_GOLBAT,
        .moves = {MOVE_WING_ATTACK, MOVE_BITE, MOVE_ASTONISH, MOVE_SUPERSONIC},
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt33[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_CUBONE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_CUBONE,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt34[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_SANDSHREW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_SANDSLASH,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt35[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_ZUBAT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_GOLBAT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_RATTATA,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt36[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_WEEZING,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_GOLBAT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_KOFFING,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt37[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_DROWZEE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_GRIMER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_MACHOP,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt38[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_GOLBAT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_DROWZEE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_HYPNO,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt39[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_MACHOKE,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt40[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_ZUBAT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt41[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 32,
        .species = SPECIES_CUBONE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 32,
        .species = SPECIES_DROWZEE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 32,
        .species = SPECIES_MAROWAK,
    },
};

static const struct TrainerMon sParty_CooltrainerSamuel[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_SANDSLASH,
        .moves = {MOVE_SLASH, MOVE_SWIFT, MOVE_SAND_ATTACK, MOVE_POISON_STING},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_SANDSLASH,
        .moves = {MOVE_SLASH, MOVE_SWIFT, MOVE_SAND_ATTACK, MOVE_POISON_STING},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_RHYHORN,
        .moves = {MOVE_TAKE_DOWN, MOVE_ROCK_BLAST, MOVE_FURY_ATTACK, MOVE_SCARY_FACE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 39,
        .species = SPECIES_NIDORINO,
        .moves = {MOVE_FURY_ATTACK, MOVE_HORN_ATTACK, MOVE_POISON_STING, MOVE_DOUBLE_KICK},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 39,
        .species = SPECIES_NIDOKING,
        .moves = {MOVE_THRASH, MOVE_DOUBLE_KICK, MOVE_POISON_STING, MOVE_FOCUS_ENERGY},
    },
};

static const struct TrainerMon sParty_CooltrainerGeorge[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_EXEGGUTOR,
        .moves = {MOVE_EGG_BOMB, MOVE_CONFUSION, MOVE_STUN_SPORE, MOVE_SLEEP_POWDER},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_SANDSLASH,
        .moves = {MOVE_FURY_SWIPES, MOVE_SWIFT, MOVE_POISON_STING, MOVE_SAND_ATTACK},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_CLOYSTER,
        .moves = {MOVE_SPIKE_CANNON, MOVE_SPIKES, MOVE_AURORA_BEAM, MOVE_SUPERSONIC},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_ELECTRODE,
        .moves = {MOVE_SPARK, MOVE_SONIC_BOOM, MOVE_SCREECH, MOVE_LIGHT_SCREEN},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_ARCANINE,
        .moves = {MOVE_FLAME_WHEEL, MOVE_ROAR, MOVE_BITE, MOVE_TAKE_DOWN},
    },
};

static const struct TrainerMon sParty_CooltrainerColby[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 41,
        .species = SPECIES_KINGLER,
        .moves = {MOVE_GUILLOTINE, MOVE_STOMP, MOVE_MUD_SHOT, MOVE_BUBBLE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_POLIWHIRL,
        .moves = {MOVE_BODY_SLAM, MOVE_DOUBLE_SLAP, MOVE_WATER_GUN, MOVE_HYPNOSIS},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_TENTACRUEL,
        .moves = {MOVE_BARRIER, MOVE_WRAP, MOVE_BUBBLE_BEAM, MOVE_ACID},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_SEADRA,
        .moves = {MOVE_WATER_GUN, MOVE_SMOKESCREEN, MOVE_TWISTER, MOVE_LEER},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 43,
        .species = SPECIES_BLASTOISE,
        .moves = {MOVE_WATER_GUN, MOVE_BITE, MOVE_RAPID_SPIN, MOVE_RAIN_DANCE},
    },
};

static const struct TrainerMon sParty_CooltrainerPaul[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_SLOWPOKE,
        .moves = {MOVE_HEADBUTT, MOVE_CONFUSION, MOVE_WATER_GUN, MOVE_DISABLE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_SHELLDER,
        .moves = {MOVE_AURORA_BEAM, MOVE_CLAMP, MOVE_SUPERSONIC, MOVE_LEER},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_KINGLER,
        .moves = {MOVE_GUILLOTINE, MOVE_STOMP, MOVE_MUD_SHOT, MOVE_BUBBLE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_STARMIE,
        .moves = {MOVE_BUBBLE_BEAM, MOVE_SWIFT, MOVE_RECOVER, MOVE_RAPID_SPIN},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_GOLDUCK,
        .moves = {MOVE_CONFUSION, MOVE_SCRATCH, MOVE_SCREECH, MOVE_DISABLE},
    },
};

static const struct TrainerMon sParty_CooltrainerRolando[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_RATICATE,
        .moves = {MOVE_SUPER_FANG, MOVE_PURSUIT, MOVE_SCARY_FACE, MOVE_QUICK_ATTACK},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_IVYSAUR,
        .moves = {MOVE_RAZOR_LEAF, MOVE_SLEEP_POWDER, MOVE_SWEET_SCENT, MOVE_SYNTHESIS},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_WARTORTLE,
        .moves = {MOVE_WATER_GUN, MOVE_BITE, MOVE_RAPID_SPIN, MOVE_TAIL_WHIP},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_CHARMELEON,
        .moves = {MOVE_FLAMETHROWER, MOVE_SLASH, MOVE_SMOKESCREEN, MOVE_SCARY_FACE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_CHARIZARD,
        .moves = {MOVE_FLAMETHROWER, MOVE_WING_ATTACK, MOVE_SMOKESCREEN, MOVE_SCARY_FACE},
    },
};

static const struct TrainerMon sParty_CooltrainerGilbert[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_PIDGEOTTO,
        .moves = {MOVE_WING_ATTACK, MOVE_FEATHER_DANCE, MOVE_WHIRLWIND, MOVE_QUICK_ATTACK},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_FEAROW,
        .moves = {MOVE_DRILL_PECK, MOVE_MIRROR_MOVE, MOVE_PURSUIT, MOVE_LEER},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_PERSIAN,
        .moves = {MOVE_PAY_DAY, MOVE_FAINT_ATTACK, MOVE_SCREECH, MOVE_BITE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_LICKITUNG,
        .moves = {MOVE_SLAM, MOVE_DISABLE, MOVE_WRAP, MOVE_SUPERSONIC},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_TAUROS,
        .moves = {MOVE_HORN_ATTACK, MOVE_SCARY_FACE, MOVE_SWAGGER, MOVE_TAIL_WHIP},
    },
};

static const struct TrainerMon sParty_CooltrainerOwen[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_NIDORINO,
        .moves = {MOVE_SCRATCH, MOVE_POISON_STING, MOVE_DOUBLE_KICK, MOVE_BITE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_NIDORINA,
        .moves = {MOVE_HORN_ATTACK, MOVE_POISON_STING, MOVE_DOUBLE_KICK, MOVE_LEER},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_RATICATE,
        .moves = {MOVE_SUPER_FANG, MOVE_PURSUIT, MOVE_SCARY_FACE, MOVE_QUICK_ATTACK},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_SANDSLASH,
        .moves = {MOVE_FURY_SWIPES, MOVE_SWIFT, MOVE_SLASH, MOVE_POISON_STING},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_RHYHORN,
        .moves = {MOVE_ROCK_BLAST, MOVE_SCARY_FACE, MOVE_STOMP, MOVE_TAIL_WHIP},
    },
};

static const struct TrainerMon sParty_CooltrainerBerke[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_SEEL,
        .moves = {MOVE_TAKE_DOWN, MOVE_AURORA_BEAM, MOVE_ICY_WIND, MOVE_GROWL},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_GRAVELER,
        .moves = {MOVE_ROCK_BLAST, MOVE_MAGNITUDE, MOVE_ROCK_THROW, MOVE_MUD_SPORT},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_KINGLER,
        .moves = {MOVE_GUILLOTINE, MOVE_STOMP, MOVE_MUD_SHOT, MOVE_BUBBLE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_ONIX,
        .moves = {MOVE_SLAM, MOVE_SANDSTORM, MOVE_DRAGON_BREATH, MOVE_ROCK_THROW},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_CLOYSTER,
        .moves = {MOVE_SPIKE_CANNON, MOVE_AURORA_BEAM, MOVE_SUPERSONIC, MOVE_PROTECT},
    },
};

static const struct TrainerMon sParty_CooltrainerYuji[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_SANDSLASH,
        .moves = {MOVE_SLASH, MOVE_SWIFT, MOVE_SAND_ATTACK, MOVE_POISON_STING},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_GRAVELER,
        .moves = {MOVE_ROCK_BLAST, MOVE_MAGNITUDE, MOVE_MUD_SPORT, MOVE_DEFENSE_CURL},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_ONIX,
        .moves = {MOVE_DRAGON_BREATH, MOVE_SANDSTORM, MOVE_ROCK_THROW, MOVE_BIND},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_GRAVELER,
        .moves = {MOVE_ROCK_BLAST, MOVE_MAGNITUDE, MOVE_ROLLOUT, MOVE_DEFENSE_CURL},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_MAROWAK,
        .moves = {MOVE_BONEMERANG, MOVE_HEADBUTT, MOVE_LEER, MOVE_GROWL},
    },
};

static const struct TrainerMon sParty_CooltrainerWarren[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_MAROWAK,
        .moves = {MOVE_BONEMERANG, MOVE_HEADBUTT, MOVE_LEER, MOVE_GROWL},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_MAROWAK,
        .moves = {MOVE_BONEMERANG, MOVE_HEADBUTT, MOVE_LEER, MOVE_GROWL},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_RHYHORN,
        .moves = {MOVE_TAKE_DOWN, MOVE_ROCK_BLAST, MOVE_FURY_ATTACK, MOVE_SCARY_FACE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 39,
        .species = SPECIES_NIDORINA,
        .moves = {MOVE_FURY_SWIPES, MOVE_BITE, MOVE_POISON_STING, MOVE_DOUBLE_KICK},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 39,
        .species = SPECIES_NIDOQUEEN,
        .moves = {MOVE_BODY_SLAM, MOVE_BITE, MOVE_POISON_STING, MOVE_DOUBLE_KICK},
    },
};

static const struct TrainerMon sParty_CooltrainerMary[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_BELLSPROUT,
        .moves = {MOVE_WRAP, MOVE_STUN_SPORE, MOVE_POISON_POWDER, MOVE_GROWTH},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_ODDISH,
        .moves = {MOVE_POISON_POWDER, MOVE_STUN_SPORE, MOVE_ABSORB, MOVE_SWEET_SCENT},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_WEEPINBELL,
        .moves = {MOVE_VINE_WHIP, MOVE_STUN_SPORE, MOVE_POISON_POWDER, MOVE_GROWTH},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_GLOOM,
        .moves = {MOVE_ABSORB, MOVE_STUN_SPORE, MOVE_POISON_POWDER, MOVE_SWEET_SCENT},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_IVYSAUR,
        .moves = {MOVE_RAZOR_LEAF, MOVE_SLEEP_POWDER, MOVE_VINE_WHIP, MOVE_LEECH_SEED},
    },
};

static const struct TrainerMon sParty_CooltrainerCaroline[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_BELLSPROUT,
        .moves = {MOVE_RAZOR_LEAF, MOVE_ACID, MOVE_STUN_SPORE, MOVE_POISON_POWDER},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_WEEPINBELL,
        .moves = {MOVE_RAZOR_LEAF, MOVE_ACID, MOVE_SLEEP_POWDER, MOVE_POISON_POWDER},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_VICTREEBEL,
        .moves = {MOVE_RAZOR_LEAF, MOVE_ACID, MOVE_STUN_SPORE, MOVE_SLEEP_POWDER},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_PARAS,
        .moves = {MOVE_GROWTH, MOVE_SLASH, MOVE_LEECH_LIFE, MOVE_STUN_SPORE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_PARASECT,
        .moves = {MOVE_SPORE, MOVE_SLASH, MOVE_LEECH_LIFE, MOVE_POISON_POWDER},
    },
};

static const struct TrainerMon sParty_CooltrainerAlexa[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_CLEFAIRY,
        .moves = {MOVE_LIGHT_SCREEN, MOVE_COSMIC_POWER, MOVE_DOUBLE_SLAP, MOVE_ENCORE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_JIGGLYPUFF,
        .moves = {MOVE_SING, MOVE_BODY_SLAM, MOVE_ROLLOUT, MOVE_DISABLE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_PERSIAN,
        .moves = {MOVE_PAY_DAY, MOVE_FAINT_ATTACK, MOVE_BITE, MOVE_SCREECH},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_DEWGONG,
        .moves = {MOVE_SHEER_COLD, MOVE_TAKE_DOWN, MOVE_REST, MOVE_AURORA_BEAM},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_CHANSEY,
        .moves = {MOVE_SING, MOVE_EGG_BOMB, MOVE_SOFT_BOILED, MOVE_MINIMIZE},
    },
};

static const struct TrainerMon sParty_CooltrainerShannon[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_BEEDRILL,
        .moves = {MOVE_PIN_MISSILE, MOVE_TWINEEDLE, MOVE_AGILITY, MOVE_PURSUIT},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_BUTTERFREE,
        .moves = {MOVE_SAFEGUARD, MOVE_PSYBEAM, MOVE_GUST, MOVE_SUPERSONIC},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_PARASECT,
        .moves = {MOVE_SPORE, MOVE_LEECH_LIFE, MOVE_SLASH, MOVE_GROWTH},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_VENONAT,
        .moves = {MOVE_PSYBEAM, MOVE_STUN_SPORE, MOVE_LEECH_LIFE, MOVE_DISABLE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_VENOMOTH,
        .moves = {MOVE_PSYBEAM, MOVE_GUST, MOVE_SUPERSONIC, MOVE_LEECH_LIFE},
    },
};

static const struct TrainerMon sParty_CooltrainerNaomi[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_PERSIAN,
        .moves = {MOVE_FURY_SWIPES, MOVE_SCREECH, MOVE_FAINT_ATTACK, MOVE_PAY_DAY},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_PONYTA,
        .moves = {MOVE_AGILITY, MOVE_TAKE_DOWN, MOVE_FIRE_SPIN, MOVE_STOMP},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_RAPIDASH,
        .moves = {MOVE_FURY_ATTACK, MOVE_FIRE_SPIN, MOVE_STOMP, MOVE_GROWL},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_VULPIX,
        .moves = {MOVE_FLAMETHROWER, MOVE_CONFUSE_RAY, MOVE_QUICK_ATTACK, MOVE_IMPRISON},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_NINETALES,
        .moves = {MOVE_SAFEGUARD, MOVE_WILL_O_WISP, MOVE_CONFUSE_RAY, MOVE_FIRE_SPIN},
    },
};

static const struct TrainerMon sParty_CooltrainerBrooke[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_TANGELA,
        .moves = {MOVE_SLAM, MOVE_BIND, MOVE_MEGA_DRAIN, MOVE_INGRAIN},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_GLOOM,
        .moves = {MOVE_ACID, MOVE_MOONLIGHT, MOVE_SLEEP_POWDER, MOVE_STUN_SPORE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_VILEPLUME,
        .moves = {MOVE_MEGA_DRAIN, MOVE_ACID, MOVE_STUN_SPORE, MOVE_AROMATHERAPY},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_IVYSAUR,
        .moves = {MOVE_RAZOR_LEAF, MOVE_SWEET_SCENT, MOVE_GROWL, MOVE_LEECH_SEED},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_VENUSAUR,
        .moves = {MOVE_RAZOR_LEAF, MOVE_GROWTH, MOVE_SLEEP_POWDER, MOVE_POISON_POWDER},
    },
};

static const struct TrainerMon sParty_CooltrainerAustina[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_RHYHORN,
        .moves = {MOVE_HORN_DRILL, MOVE_ROCK_BLAST, MOVE_SCARY_FACE, MOVE_STOMP},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_NIDORINA,
        .moves = {MOVE_DOUBLE_KICK, MOVE_FURY_SWIPES, MOVE_BITE, MOVE_FLATTER},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_NIDOQUEEN,
        .moves = {MOVE_BODY_SLAM, MOVE_DOUBLE_KICK, MOVE_BITE, MOVE_GROWL},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_NIDORINO,
        .moves = {MOVE_HORN_ATTACK, MOVE_POISON_STING, MOVE_FOCUS_ENERGY, MOVE_LEER},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_NIDOKING,
        .moves = {MOVE_THRASH, MOVE_DOUBLE_KICK, MOVE_POISON_STING, MOVE_PECK},
    },
};

static const struct TrainerMon sParty_CooltrainerJulie[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_PERSIAN,
        .moves = {MOVE_FURY_SWIPES, MOVE_BITE, MOVE_SCREECH, MOVE_FAINT_ATTACK},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_NINETALES,
        .moves = {MOVE_FLAMETHROWER, MOVE_WILL_O_WISP, MOVE_CONFUSE_RAY, MOVE_GRUDGE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_RAPIDASH,
        .moves = {MOVE_FURY_ATTACK, MOVE_FIRE_SPIN, MOVE_TAKE_DOWN, MOVE_AGILITY},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_PIKACHU,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDER_WAVE, MOVE_DOUBLE_TEAM, MOVE_QUICK_ATTACK},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_RAICHU,
        .moves = {MOVE_THUNDER, MOVE_THUNDER_WAVE, MOVE_SLAM, MOVE_DOUBLE_TEAM},
    },
};

static const struct TrainerMon sParty_EliteFourLorelei[] = {
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_DEWGONG,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_ICE_BEAM, MOVE_SURF, MOVE_HAIL, MOVE_SAFEGUARD},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 0, 0, 0),
        .lvl = 51,
        .species = SPECIES_CLOYSTER,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_SPIKES, MOVE_PROTECT, MOVE_HAIL, MOVE_DIVE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_SLOWBRO,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_ICE_BEAM, MOVE_SURF, MOVE_AMNESIA, MOVE_YAWN},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_JYNX,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_ICE_PUNCH, MOVE_DOUBLE_SLAP, MOVE_LOVELY_KISS, MOVE_ATTRACT},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_LAPRAS,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CONFUSE_RAY, MOVE_ICE_BEAM, MOVE_SURF, MOVE_BODY_SLAM},
    },
};

static const struct TrainerMon sParty_EliteFourBruno[] = {
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 0, 0, 0),
        .lvl = 51,
        .species = SPECIES_ONIX,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_EARTHQUAKE, MOVE_ROCK_TOMB, MOVE_IRON_TAIL, MOVE_ROAR},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_SKY_UPPERCUT, MOVE_MACH_PUNCH, MOVE_ROCK_TOMB, MOVE_COUNTER},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_MEGA_KICK, MOVE_FORESIGHT, MOVE_BRICK_BREAK, MOVE_FACADE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_ONIX,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_DOUBLE_EDGE, MOVE_EARTHQUAKE, MOVE_IRON_TAIL, MOVE_SAND_TOMB},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 0, 0, 0),
        .lvl = 56,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CROSS_CHOP, MOVE_BULK_UP, MOVE_SCARY_FACE, MOVE_ROCK_TOMB},
    },
};

static const struct TrainerMon sParty_EliteFourAgatha[] = {
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_SHADOW_PUNCH, MOVE_CONFUSE_RAY, MOVE_TOXIC, MOVE_DOUBLE_TEAM},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_GOLBAT,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_CONFUSE_RAY, MOVE_POISON_FANG, MOVE_AIR_CUTTER, MOVE_BITE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_HAUNTER,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYPNOSIS, MOVE_DREAM_EATER, MOVE_CURSE, MOVE_MEAN_LOOK},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 0, 0, 0),
        .lvl = 56,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SCREECH, MOVE_IRON_TAIL, MOVE_BITE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 0, 0, 0),
        .lvl = 58,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHADOW_BALL, MOVE_SLUDGE_BOMB, MOVE_HYPNOSIS, MOVE_NIGHTMARE},
    },
};

static const struct TrainerMon sParty_EliteFourLance[] = {
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 0, 0, 0),
        .lvl = 56,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYPER_BEAM, MOVE_DRAGON_RAGE, MOVE_TWISTER, MOVE_BITE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_DRAGONAIR,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYPER_BEAM, MOVE_SAFEGUARD, MOVE_DRAGON_RAGE, MOVE_OUTRAGE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_DRAGONAIR,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYPER_BEAM, MOVE_SAFEGUARD, MOVE_THUNDER_WAVE, MOVE_OUTRAGE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 0, 0, 0),
        .lvl = 58,
        .species = SPECIES_AERODACTYL,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYPER_BEAM, MOVE_ANCIENT_POWER, MOVE_WING_ATTACK, MOVE_SCARY_FACE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 0, 0, 0),
        .lvl = 60,
        .species = SPECIES_DRAGONITE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYPER_BEAM, MOVE_SAFEGUARD, MOVE_OUTRAGE, MOVE_WING_ATTACK},
    },
};

static const struct TrainerMon sParty_LeaderBrock[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 12,
        .species = SPECIES_GEODUDE,
        .moves = {MOVE_TACKLE, MOVE_DEFENSE_CURL, MOVE_NONE, MOVE_NONE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 14,
        .species = SPECIES_ONIX,
        .moves = {MOVE_TACKLE, MOVE_BIND, MOVE_ROCK_TOMB, MOVE_NONE},
    },
};

static const struct TrainerMon sParty_LeaderMisty[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_STARYU,
        .moves = {MOVE_TACKLE, MOVE_HARDEN, MOVE_RECOVER, MOVE_WATER_PULSE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_STARMIE,
        .moves = {MOVE_SWIFT, MOVE_RECOVER, MOVE_RAPID_SPIN, MOVE_WATER_PULSE},
    },
};

static const struct TrainerMon sParty_LeaderLtSurge[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SONIC_BOOM, MOVE_TACKLE, MOVE_SCREECH, MOVE_SHOCK_WAVE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_PIKACHU,
        .moves = {MOVE_QUICK_ATTACK, MOVE_THUNDER_WAVE, MOVE_DOUBLE_TEAM, MOVE_SHOCK_WAVE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_RAICHU,
        .moves = {MOVE_QUICK_ATTACK, MOVE_THUNDER_WAVE, MOVE_DOUBLE_TEAM, MOVE_SHOCK_WAVE},
    },
};

static const struct TrainerMon sParty_LeaderErika[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_VICTREEBEL,
        .moves = {MOVE_STUN_SPORE, MOVE_ACID, MOVE_POISON_POWDER, MOVE_GIGA_DRAIN},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_TANGELA,
        .moves = {MOVE_POISON_POWDER, MOVE_CONSTRICT, MOVE_INGRAIN, MOVE_GIGA_DRAIN},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_VILEPLUME,
        .moves = {MOVE_SLEEP_POWDER, MOVE_ACID, MOVE_STUN_SPORE, MOVE_GIGA_DRAIN},
    },
};

static const struct TrainerMon sParty_LeaderKoga[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SELF_DESTRUCT, MOVE_SLUDGE, MOVE_SMOKESCREEN, MOVE_TOXIC},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 39,
        .species = SPECIES_MUK,
        .moves = {MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_ACID_ARMOR, MOVE_TOXIC},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SELF_DESTRUCT, MOVE_SLUDGE, MOVE_SMOKESCREEN, MOVE_TOXIC},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 43,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_TACKLE, MOVE_SLUDGE, MOVE_SMOKESCREEN, MOVE_TOXIC},
    },
};

static const struct TrainerMon sParty_LeaderBlaine[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_GROWLITHE,
        .moves = {MOVE_BITE, MOVE_ROAR, MOVE_TAKE_DOWN, MOVE_FIRE_BLAST},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 40,
        .species = SPECIES_PONYTA,
        .moves = {MOVE_STOMP, MOVE_BOUNCE, MOVE_FIRE_SPIN, MOVE_FIRE_BLAST},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 42,
        .species = SPECIES_RAPIDASH,
        .moves = {MOVE_STOMP, MOVE_BOUNCE, MOVE_FIRE_SPIN, MOVE_FIRE_BLAST},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_ARCANINE,
        .moves = {MOVE_BITE, MOVE_ROAR, MOVE_TAKE_DOWN, MOVE_FIRE_BLAST},
    },
};

static const struct TrainerMon sParty_LeaderSabrina[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_KADABRA,
        .moves = {MOVE_PSYBEAM, MOVE_REFLECT, MOVE_FUTURE_SIGHT, MOVE_CALM_MIND},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_MR_MIME,
        .moves = {MOVE_BARRIER, MOVE_PSYBEAM, MOVE_BATON_PASS, MOVE_CALM_MIND},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_VENOMOTH,
        .moves = {MOVE_PSYBEAM, MOVE_GUST, MOVE_LEECH_LIFE, MOVE_SUPERSONIC},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 43,
        .species = SPECIES_ALAKAZAM,
        .moves = {MOVE_PSYCHIC, MOVE_RECOVER, MOVE_FUTURE_SIGHT, MOVE_CALM_MIND},
    },
};

static const struct TrainerMon sParty_GentlemanThomas[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_GROWLITHE,
    },
};

static const struct TrainerMon sParty_GentlemanArthur[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_NIDORAN_M,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_NIDORAN_F,
    },
};

static const struct TrainerMon sParty_GentlemanTucker[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_PIKACHU,
    },
};

static const struct TrainerMon sParty_GentlemanNorton[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_PERSIAN,
    },
};

static const struct TrainerMon sParty_GentlemanWalter[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_PONYTA,
    },
};

static const struct TrainerMon sParty_RivalSsAnneSquirtle[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 16,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_KADABRA,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_WARTORTLE,
    },
};

static const struct TrainerMon sParty_RivalSsAnneBulbasaur[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 16,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_KADABRA,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_IVYSAUR,
    },
};

static const struct TrainerMon sParty_RivalSsAnneCharmander[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 16,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_KADABRA,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_CHARMELEON,
    },
};

static const struct TrainerMon sParty_RivalPokenonTowerSquirtle[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_EXEGGCUTE,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_KADABRA,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_WARTORTLE,
    },
};

static const struct TrainerMon sParty_RivalPokenonTowerBulbasaur[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_GYARADOS,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_KADABRA,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_IVYSAUR,
    },
};

static const struct TrainerMon sParty_RivalPokenonTowerCharmander[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_EXEGGCUTE,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_GYARADOS,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_KADABRA,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_CHARMELEON,
    },
};

static const struct TrainerMon sParty_RivalSilphSquirtle[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_PIDGEOT,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_EXEGGCUTE,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_ALAKAZAM,
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 40,
        .species = SPECIES_BLASTOISE,
    },
};

static const struct TrainerMon sParty_RivalSilphBulbasaur[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_PIDGEOT,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_GYARADOS,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_ALAKAZAM,
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 40,
        .species = SPECIES_VENUSAUR,
    },
};

static const struct TrainerMon sParty_RivalSilphCharmander[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_PIDGEOT,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_EXEGGCUTE,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_GYARADOS,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_ALAKAZAM,
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 40,
        .species = SPECIES_CHARIZARD,
    },
};

static const struct TrainerMon sParty_RivalRoute22LateSquirtle[] = {
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_PIDGEOT,
        .moves = {MOVE_FEATHER_DANCE, MOVE_WING_ATTACK, MOVE_GUST, MOVE_QUICK_ATTACK},
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 45,
        .species = SPECIES_RHYHORN,
        .moves = {MOVE_TAKE_DOWN, MOVE_HORN_DRILL, MOVE_ROCK_BLAST, MOVE_FURY_ATTACK},
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 45,
        .species = SPECIES_GROWLITHE,
        .moves = {MOVE_FLAME_WHEEL, MOVE_TAKE_DOWN, MOVE_LEER, MOVE_AGILITY},
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 45,
        .species = SPECIES_EXEGGCUTE,
        .moves = {MOVE_SOLAR_BEAM, MOVE_SLEEP_POWDER, MOVE_POISON_POWDER, MOVE_STUN_SPORE},
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_ALAKAZAM,
        .moves = {MOVE_PSYCHIC, MOVE_CALM_MIND, MOVE_FUTURE_SIGHT, MOVE_DISABLE},
    },
    {
        .iv = TRAINER_MON_IVS(26, 7, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_BLASTOISE,
        .moves = {MOVE_WATER_GUN, MOVE_RAIN_DANCE, MOVE_BITE, MOVE_RAPID_SPIN},
    },
};

static const struct TrainerMon sParty_RivalRoute22LateBulbasaur[] = {
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_PIDGEOT,
        .moves = {MOVE_FEATHER_DANCE, MOVE_WING_ATTACK, MOVE_GUST, MOVE_QUICK_ATTACK},
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 45,
        .species = SPECIES_RHYHORN,
        .moves = {MOVE_TAKE_DOWN, MOVE_HORN_DRILL, MOVE_ROCK_BLAST, MOVE_FURY_ATTACK},
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 45,
        .species = SPECIES_GYARADOS,
        .moves = {MOVE_HYDRO_PUMP, MOVE_TWISTER, MOVE_LEER, MOVE_RAIN_DANCE},
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 45,
        .species = SPECIES_GROWLITHE,
        .moves = {MOVE_FLAME_WHEEL, MOVE_TAKE_DOWN, MOVE_LEER, MOVE_AGILITY},
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_ALAKAZAM,
        .moves = {MOVE_PSYCHIC, MOVE_CALM_MIND, MOVE_FUTURE_SIGHT, MOVE_DISABLE},
    },
    {
        .iv = TRAINER_MON_IVS(26, 7, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_VENUSAUR,
        .moves = {MOVE_RAZOR_LEAF, MOVE_SWEET_SCENT, MOVE_GROWTH, MOVE_SYNTHESIS},
    },
};

static const struct TrainerMon sParty_RivalRoute22LateCharmander[] = {
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_PIDGEOT,
        .moves = {MOVE_FEATHER_DANCE, MOVE_WING_ATTACK, MOVE_GUST, MOVE_QUICK_ATTACK},
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 45,
        .species = SPECIES_RHYHORN,
        .moves = {MOVE_TAKE_DOWN, MOVE_HORN_DRILL, MOVE_ROCK_BLAST, MOVE_FURY_ATTACK},
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 45,
        .species = SPECIES_EXEGGCUTE,
        .moves = {MOVE_SOLAR_BEAM, MOVE_SLEEP_POWDER, MOVE_POISON_POWDER, MOVE_STUN_SPORE},
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 45,
        .species = SPECIES_GYARADOS,
        .moves = {MOVE_HYDRO_PUMP, MOVE_TWISTER, MOVE_LEER, MOVE_RAIN_DANCE},
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_ALAKAZAM,
        .moves = {MOVE_PSYCHIC, MOVE_CALM_MIND, MOVE_FUTURE_SIGHT, MOVE_DISABLE},
    },
    {
        .iv = TRAINER_MON_IVS(26, 7, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_CHARIZARD,
        .moves = {MOVE_FLAMETHROWER, MOVE_WING_ATTACK, MOVE_SLASH, MOVE_SCARY_FACE},
    },
};

static const struct TrainerMon sParty_ChampionFirstSquirtle[] = {
    {
        .iv = TRAINER_MON_IVS(31, 31, 0, 0, 0, 0),
        .lvl = 59,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_AERIAL_ACE, MOVE_FEATHER_DANCE, MOVE_SAND_ATTACK, MOVE_WHIRLWIND},
    },
    {
        .iv = TRAINER_MON_IVS(31, 31, 0, 0, 0, 0),
        .lvl = 57,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_PSYCHIC, MOVE_FUTURE_SIGHT, MOVE_RECOVER, MOVE_REFLECT},
    },
    {
        .iv = TRAINER_MON_IVS(31, 31, 0, 0, 0, 0),
        .lvl = 59,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_TAKE_DOWN, MOVE_EARTHQUAKE, MOVE_ROCK_TOMB, MOVE_SCARY_FACE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 31, 0, 0, 0, 0),
        .lvl = 59,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_EXTREME_SPEED, MOVE_FLAMETHROWER, MOVE_ROAR, MOVE_BITE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 31, 0, 0, 0, 0),
        .lvl = 61,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_GIGA_DRAIN, MOVE_EGG_BOMB, MOVE_SLEEP_POWDER, MOVE_LIGHT_SCREEN},
    },
    {
        .iv = TRAINER_MON_IVS(31, 31, 0, 0, 0, 0),
        .lvl = 63,
        .species = SPECIES_BLASTOISE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_RAIN_DANCE, MOVE_SKULL_BASH, MOVE_BITE},
    },
};

static const struct TrainerMon sParty_ChampionFirstBulbasaur[] = {
    {
        .iv = TRAINER_MON_IVS(31, 31, 0, 0, 0, 0),
        .lvl = 59,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_AERIAL_ACE, MOVE_FEATHER_DANCE, MOVE_SAND_ATTACK, MOVE_WHIRLWIND},
    },
    {
        .iv = TRAINER_MON_IVS(31, 31, 0, 0, 0, 0),
        .lvl = 57,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_PSYCHIC, MOVE_FUTURE_SIGHT, MOVE_RECOVER, MOVE_REFLECT},
    },
    {
        .iv = TRAINER_MON_IVS(31, 31, 0, 0, 0, 0),
        .lvl = 59,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_TAKE_DOWN, MOVE_EARTHQUAKE, MOVE_ROCK_TOMB, MOVE_SCARY_FACE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 31, 0, 0, 0, 0),
        .lvl = 59,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYDRO_PUMP, MOVE_DRAGON_RAGE, MOVE_BITE, MOVE_THRASH},
    },
    {
        .iv = TRAINER_MON_IVS(31, 31, 0, 0, 0, 0),
        .lvl = 61,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_EXTREME_SPEED, MOVE_FLAMETHROWER, MOVE_ROAR, MOVE_BITE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 31, 0, 0, 0, 0),
        .lvl = 63,
        .species = SPECIES_VENUSAUR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SOLAR_BEAM, MOVE_SYNTHESIS, MOVE_SUNNY_DAY, MOVE_GROWTH},
    },
};

static const struct TrainerMon sParty_ChampionFirstCharmander[] = {
    {
        .iv = TRAINER_MON_IVS(31, 31, 0, 0, 0, 0),
        .lvl = 59,
        .species = SPECIES_PIDGEOT,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_AERIAL_ACE, MOVE_FEATHER_DANCE, MOVE_SAND_ATTACK, MOVE_WHIRLWIND},
    },
    {
        .iv = TRAINER_MON_IVS(31, 31, 0, 0, 0, 0),
        .lvl = 57,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_PSYCHIC, MOVE_FUTURE_SIGHT, MOVE_RECOVER, MOVE_REFLECT},
    },
    {
        .iv = TRAINER_MON_IVS(31, 31, 0, 0, 0, 0),
        .lvl = 59,
        .species = SPECIES_RHYDON,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_TAKE_DOWN, MOVE_EARTHQUAKE, MOVE_ROCK_TOMB, MOVE_SCARY_FACE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 31, 0, 0, 0, 0),
        .lvl = 59,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_GIGA_DRAIN, MOVE_EGG_BOMB, MOVE_SLEEP_POWDER, MOVE_LIGHT_SCREEN},
    },
    {
        .iv = TRAINER_MON_IVS(31, 31, 0, 0, 0, 0),
        .lvl = 61,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYDRO_PUMP, MOVE_DRAGON_RAGE, MOVE_BITE, MOVE_THRASH},
    },
    {
        .iv = TRAINER_MON_IVS(31, 31, 0, 0, 0, 0),
        .lvl = 63,
        .species = SPECIES_CHARIZARD,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_AERIAL_ACE, MOVE_SLASH, MOVE_FIRE_SPIN},
    },
};

static const struct TrainerMon sParty_ChannelerPatricia[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMon sParty_ChannelerCarly[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMon sParty_ChannelerHope[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMon sParty_ChannelerPaula[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMon sParty_ChannelerLaurel[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_GASTLY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMon sParty_ChannelerJody[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMon sParty_ChannelerTammy[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_HAUNTER,
    },
};

static const struct TrainerMon sParty_ChannelerRuth[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMon sParty_ChannelerKarina[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMon sParty_ChannelerJanae[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMon sParty_ChannelerAngelica[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_GASTLY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_GASTLY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMon sParty_ChannelerEmilia[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMon sParty_ChannelerJennifer[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMon sParty_ChannelerAmanda[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_GASTLY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_HAUNTER,
    },
};

static const struct TrainerMon sParty_ChannelerStacy[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_HAUNTER,
    },
};

static const struct TrainerMon sParty_ChannelerTasha[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_GASTLY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_GASTLY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_HAUNTER,
    },
};

static const struct TrainerMon sParty_HikerJeremy[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_MACHOP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_ONIX,
    },
};

static const struct TrainerMon sParty_PicnickerAlma[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_GOLDEEN,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_POLIWAG,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_HORSEA,
    },
};

static const struct TrainerMon sParty_PicnickerSusie[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_MEOWTH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_PIKACHU,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_MEOWTH,
    },
};

static const struct TrainerMon sParty_PicnickerValerie[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_POLIWAG,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_POLIWAG,
    },
};

static const struct TrainerMon sParty_PicnickerGwen[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_MEOWTH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMon sParty_BikerVirgil[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
};

static const struct TrainerMon sParty_CamperFlint[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 14,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 14,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMon sParty_PicnickerMissy[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_GOLDEEN,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMon sParty_PicnickerIrene[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_TENTACOOL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_HORSEA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_SEEL,
    },
};

static const struct TrainerMon sParty_PicnickerDana[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_MEOWTH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_ODDISH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_PIDGEY,
    },
};

static const struct TrainerMon sParty_PicnickerAriana[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_BELLSPROUT,
    },
};

static const struct TrainerMon sParty_PicnickerLeah[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_BELLSPROUT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_CLEFAIRY,
    },
};

static const struct TrainerMon sParty_CamperJustin[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_NIDORAN_M,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_NIDORINO,
    },
};

static const struct TrainerMon sParty_PicnickerYazmin[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_BELLSPROUT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_ODDISH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_TANGELA,
    },
};

static const struct TrainerMon sParty_PicnickerKindra[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_GLOOM,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_ODDISH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_ODDISH,
    },
};

static const struct TrainerMon sParty_PicnickerBecky[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_PIKACHU,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_RAICHU,
    },
};

static const struct TrainerMon sParty_PicnickerCelia[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_CLEFAIRY,
    },
};

static const struct TrainerMon sParty_GentlemanBrooks[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_PIKACHU,
    },
};

static const struct TrainerMon sParty_GentlemanLamar[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 17,
        .species = SPECIES_PONYTA,
    },
};

static const struct TrainerMon sParty_TwinsEliAnne[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_JIGGLYPUFF,
    },
};

static const struct TrainerMon sParty_CoolCoupleRayTyra[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 45,
        .species = SPECIES_NIDOQUEEN,
        .moves = {MOVE_SUPERPOWER, MOVE_BODY_SLAM, MOVE_DOUBLE_KICK, MOVE_POISON_STING},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 45,
        .species = SPECIES_NIDOKING,
        .moves = {MOVE_MEGAHORN, MOVE_THRASH, MOVE_DOUBLE_KICK, MOVE_POISON_STING},
    },
};

static const struct TrainerMon sParty_YoungCoupleGiaJes[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_NIDORAN_M,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_NIDORAN_F,
    },
};

static const struct TrainerMon sParty_TwinsKiriJan[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_CHARMANDER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_SQUIRTLE,
    },
};

static const struct TrainerMon sParty_CrushKinRonMya[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_YoungCoupleLeaJed[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_RAPIDASH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_NINETALES,
    },
};

static const struct TrainerMon sParty_SisAndBroLiaLuc[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_GOLDEEN,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMon sParty_SisAndBroLilIan[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_SEADRA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_STARMIE,
    },
};

static const struct TrainerMon sParty_YoungsterBen3[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMon sParty_YoungsterBen4[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_ARBOK,
    },
};

static const struct TrainerMon sParty_YoungsterChad2[] = {
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_EKANS,
    },
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_SANDSHREW,
    },
};

static const struct TrainerMon sParty_LassReli2[] = {
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_NIDORAN_F,
    },
};

static const struct TrainerMon sParty_LassReli3[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_NIDORINA,
    },
};

static const struct TrainerMon sParty_YoungsterTimmy2[] = {
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_EKANS,
    },
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMon sParty_YoungsterTimmy3[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_EKANS,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_GOLBAT,
    },
};

static const struct TrainerMon sParty_YoungsterTimmy4[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_ARBOK,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_GOLBAT,
    },
};

static const struct TrainerMon sParty_YoungsterChad3[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_ARBOK,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_SANDSHREW,
    },
};

static const struct TrainerMon sParty_LassJanice2[] = {
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 20,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMon sParty_LassJanice3[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMon sParty_YoungsterChad4[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_ARBOK,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_SANDSLASH,
    },
};

static const struct TrainerMon sParty_HikerFranklin2[] = {
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_MACHOKE,
    },
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_GRAVELER,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt42[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_HOUNDOUR,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_HOUNDOUR,
    },
};

static const struct TrainerMon sParty_PsychicJaclyn[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_NATU,
        .moves = {MOVE_NIGHT_SHADE, MOVE_CONFUSE_RAY, MOVE_FUTURE_SIGHT, MOVE_WISH},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_SLOWBRO,
        .moves = {MOVE_PSYCHIC, MOVE_HEADBUTT, MOVE_AMNESIA, MOVE_YAWN},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_KADABRA,
        .moves = {MOVE_PSYCHIC, MOVE_FUTURE_SIGHT, MOVE_RECOVER, MOVE_REFLECT},
    },
};

static const struct TrainerMon sParty_CrushGirlSharon[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_MANKEY,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_TuberAmira[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_POLIWAG,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_POLIWHIRL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_POLIWAG,
    },
};

static const struct TrainerMon sParty_PkmnBreederAlize[] = {
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_PIKACHU,
    },
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_MARILL,
    },
};

static const struct TrainerMon sParty_PkmnRangerNicolas[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 51,
        .species = SPECIES_WEEPINBELL,
        .moves = {MOVE_RAZOR_LEAF, MOVE_ACID, MOVE_SWEET_SCENT, MOVE_WRAP},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 51,
        .species = SPECIES_VICTREEBEL,
        .moves = {MOVE_RAZOR_LEAF, MOVE_ACID, MOVE_SLEEP_POWDER, MOVE_STUN_SPORE},
    },
};

static const struct TrainerMon sParty_PkmnRangerMadeline[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 51,
        .species = SPECIES_GLOOM,
        .moves = {MOVE_PETAL_DANCE, MOVE_ACID, MOVE_SWEET_SCENT, MOVE_POISON_POWDER},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 51,
        .species = SPECIES_VILEPLUME,
        .moves = {MOVE_PETAL_DANCE, MOVE_MOONLIGHT, MOVE_ACID, MOVE_STUN_SPORE},
    },
};

static const struct TrainerMon sParty_AromaLadyNikki[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_BELLSPROUT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_WEEPINBELL,
    },
};

static const struct TrainerMon sParty_RuinManiacStanly[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_GRAVELER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_ONIX,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_GRAVELER,
    },
};

static const struct TrainerMon sParty_LadyJacki[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_HOPPIP,
        .heldItem = ITEM_STARDUST,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_SKIPLOOM,
        .heldItem = ITEM_STARDUST,
    },
};

static const struct TrainerMon sParty_PainterDaisy[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_SMEARGLE,
        .moves = {MOVE_DYNAMIC_PUNCH, MOVE_DIZZY_PUNCH, MOVE_FOCUS_PUNCH, MOVE_MEGA_PUNCH},
    },
};

static const struct TrainerMon sParty_BikerGoon[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_HAZE, MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_TACKLE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_ACID_ARMOR, MOVE_SCREECH, MOVE_MINIMIZE, MOVE_SLUDGE},
    },
};

static const struct TrainerMon sParty_BikerGoon2[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_HAZE, MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_TACKLE},
    },
};

static const struct TrainerMon sParty_BikerGoon3[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_GRIMER,
    },
};

static const struct TrainerMon sParty_BugCatcherAnthony[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 7,
        .species = SPECIES_CATERPIE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 8,
        .species = SPECIES_CATERPIE,
    },
};

static const struct TrainerMon sParty_BugCatcherCharlie[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 7,
        .species = SPECIES_METAPOD,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 7,
        .species = SPECIES_CATERPIE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 7,
        .species = SPECIES_METAPOD,
    },
};

static const struct TrainerMon sParty_TwinsEliAnne2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_JIGGLYPUFF,
    },
};

static const struct TrainerMon sParty_YoungsterJohnson[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_EKANS,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_EKANS,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMon sParty_BikerRicardo[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE, MOVE_POISON_GAS},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE, MOVE_POISON_GAS},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE, MOVE_POUND},
    },
};

static const struct TrainerMon sParty_BikerJaren[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_GRIMER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_GRIMER,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt43[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_CUBONE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_MAROWAK,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt44[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_SANDSHREW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_SANDSLASH,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt45[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_ZUBAT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_ZUBAT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_GOLBAT,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt46[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_MUK,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_GOLBAT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt47[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_MACHOP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_MACHOP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_MACHOKE,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt48[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_HYPNO,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_HYPNO,
    },
};

static const struct TrainerMon sParty_TeamRocketAdmin[] = {
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_MUK,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_SCREECH, MOVE_MINIMIZE, MOVE_ROCK_TOMB},
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_ARBOK,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_BITE, MOVE_EARTHQUAKE, MOVE_IRON_TAIL},
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_VILEPLUME,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_GIGA_DRAIN, MOVE_SLEEP_POWDER, MOVE_STUN_SPORE},
    },
};

static const struct TrainerMon sParty_TeamRocketAdmin2[] = {
    {
        .iv = TRAINER_MON_IVS(8, 6, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_GOLBAT,
        .moves = {MOVE_CONFUSE_RAY, MOVE_SLUDGE_BOMB, MOVE_AIR_CUTTER, MOVE_SHADOW_BALL},
    },
    {
        .iv = TRAINER_MON_IVS(8, 6, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_THUNDERBOLT, MOVE_EXPLOSION, MOVE_SHADOW_BALL},
    },
    {
        .iv = TRAINER_MON_IVS(8, 6, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_HOUNDOOM,
        .moves = {MOVE_FLAMETHROWER, MOVE_CRUNCH, MOVE_IRON_TAIL, MOVE_SHADOW_BALL},
    },
};

static const struct TrainerMon sParty_ScientistGideon[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 46,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SWIFT, MOVE_SCREECH, MOVE_SPARK, MOVE_SONIC_BOOM},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 46,
        .species = SPECIES_ELECTRODE,
        .moves = {MOVE_SPARK, MOVE_SONIC_BOOM, MOVE_SCREECH, MOVE_CHARGE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 46,
        .species = SPECIES_MAGNEMITE,
        .moves = {MOVE_SCREECH, MOVE_SWIFT, MOVE_SPARK, MOVE_THUNDER_WAVE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 46,
        .species = SPECIES_MAGNETON,
        .moves = {MOVE_TRI_ATTACK, MOVE_SPARK, MOVE_THUNDER_WAVE, MOVE_SONIC_BOOM},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 46,
        .species = SPECIES_PORYGON,
        .moves = {MOVE_TRI_ATTACK, MOVE_CONVERSION, MOVE_RECOVER, MOVE_PSYBEAM},
    },
};

static const struct TrainerMon sParty_SwimmerFemaleAmara[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 36,
        .species = SPECIES_SEEL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 36,
        .species = SPECIES_SEEL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 36,
        .species = SPECIES_DEWGONG,
    },
};

static const struct TrainerMon sParty_SwimmerFemaleMaria[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_SEADRA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_SEADRA,
    },
};

static const struct TrainerMon sParty_SwimmerFemaleAbigail[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_PSYDUCK,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 36,
        .species = SPECIES_PSYDUCK,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_GOLDUCK,
    },
};

static const struct TrainerMon sParty_SwimmerMaleFinn[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_STARMIE,
    },
};

static const struct TrainerMon sParty_SwimmerMaleGarrett[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_SHELLDER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_CLOYSTER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_WARTORTLE,
    },
};

static const struct TrainerMon sParty_FishermanTommy[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_GOLDEEN,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_GOLDEEN,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_SEAKING,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_SEAKING,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMon sParty_CrushGirlTanya[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_BlackBeltShea[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_MACHOP,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_BlackBeltHugh[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_MACHOP,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_CamperBryce[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 36,
        .species = SPECIES_NIDORINO,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 36,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 36,
        .species = SPECIES_SANDSLASH,
    },
};

static const struct TrainerMon sParty_PicnickerClaire[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_MEOWTH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_MEOWTH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_PIKACHU,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 35,
        .species = SPECIES_CLEFAIRY,
    },
};

static const struct TrainerMon sParty_CrushKinMikKia[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 39,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 39,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_AromaLadyViolet[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 36,
        .species = SPECIES_BULBASAUR,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 36,
        .species = SPECIES_IVYSAUR,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 36,
        .species = SPECIES_IVYSAUR,
    },
};

static const struct TrainerMon sParty_TuberAlexis[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_STARYU,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_STARYU,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_KRABBY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 34,
        .species = SPECIES_KRABBY,
    },
};

static const struct TrainerMon sParty_TwinsJoyMeg[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_CLEFAIRY,
    },
};

static const struct TrainerMon sParty_SwimmerFemaleTisha[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_KINGLER,
    },
};

static const struct TrainerMon sParty_PainterCelina[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_SMEARGLE,
        .moves = {MOVE_FLY, MOVE_DIG, MOVE_DIVE, MOVE_BOUNCE},
    },
};

static const struct TrainerMon sParty_PainterRayna[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_SMEARGLE,
        .moves = {MOVE_CROSS_CHOP, MOVE_MEGAHORN, MOVE_DOUBLE_EDGE, MOVE_SELF_DESTRUCT},
    },
};

static const struct TrainerMon sParty_LadyGillian[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_MAREEP,
        .heldItem = ITEM_STARDUST,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_MAREEP,
        .heldItem = ITEM_STARDUST,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_FLAAFFY,
        .heldItem = ITEM_NUGGET,
    },
};

static const struct TrainerMon sParty_YoungsterDestin[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMon sParty_SwimmerMaleToby[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_POLIWHIRL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_TENTACOOL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_TENTACRUEL,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt49[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_GRIMER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_MUK,
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt50[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_MEMENTO, MOVE_HAZE, MOVE_SMOKESCREEN, MOVE_SLUDGE},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_HAZE, MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SELF_DESTRUCT},
    },
};

static const struct TrainerMon sParty_TeamRocketGrunt51[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_EKANS,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_GLOOM,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_GLOOM,
    },
};

static const struct TrainerMon sParty_BirdKeeperMilo[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMon sParty_BirdKeeperChaz[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_SPEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMon sParty_BirdKeeperHarold[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_HOOTHOOT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_NOCTOWL,
    },
};

static const struct TrainerMon sParty_FishermanTylor[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_QWILFISH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_QWILFISH,
    },
};

static const struct TrainerMon sParty_SwimmerMaleMymo[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_KINGLER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_WARTORTLE,
    },
};

static const struct TrainerMon sParty_SwimmerFemaleNicole[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_MARILL,
    },
};

static const struct TrainerMon sParty_SisAndBroAvaGeb[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_POLIWHIRL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_STARMIE,
    },
};

static const struct TrainerMon sParty_AromaLadyRose[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_SUNKERN,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_SUNFLORA,
    },
};

static const struct TrainerMon sParty_SwimmerMaleSamir[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_GYARADOS,
    },
};

static const struct TrainerMon sParty_SwimmerFemaleDenise[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_CHINCHOU,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_LANTURN,
    },
};

static const struct TrainerMon sParty_TwinsMiuMia[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_PIKACHU,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_PIKACHU,
    },
};

static const struct TrainerMon sParty_HikerEarl[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_ONIX,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_MACHOKE,
    },
};

static const struct TrainerMon sParty_RuinManiacFoster[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_GOLEM,
    },
};

static const struct TrainerMon sParty_RuinManiacLarry[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_MACHOKE,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_MACHOKE,
    },
};

static const struct TrainerMon sParty_HikerDaryl[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_SUDOWOODO,
    },
};

static const struct TrainerMon sParty_PokemaniacHector[] = {
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_RHYHORN,
    },
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_KANGASKHAN,
    },
};

static const struct TrainerMon sParty_PsychicDario[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_GIRAFARIG,
        .moves = {MOVE_CRUNCH, MOVE_PSYBEAM, MOVE_ODOR_SLEUTH, MOVE_AGILITY},
    },
};

static const struct TrainerMon sParty_PsychicRodette[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_NATU,
        .moves = {MOVE_NIGHT_SHADE, MOVE_CONFUSE_RAY, MOVE_WISH, MOVE_FUTURE_SIGHT},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_DROWZEE,
        .moves = {MOVE_PSYCHIC, MOVE_DISABLE, MOVE_PSYCH_UP, MOVE_FUTURE_SIGHT},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_HYPNO,
        .moves = {MOVE_PSYCHIC, MOVE_HYPNOSIS, MOVE_PSYCH_UP, MOVE_FUTURE_SIGHT},
    },
};

static const struct TrainerMon sParty_AromaLadyMiah[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_BELLOSSOM,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_BELLOSSOM,
    },
};

static const struct TrainerMon sParty_YoungCoupleEveJon[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_GOLDUCK,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_PSYDUCK,
    },
};

static const struct TrainerMon sParty_JugglerMason[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SWIFT, MOVE_LIGHT_SCREEN, MOVE_SPARK, MOVE_SONIC_BOOM},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_PINECO,
        .moves = {MOVE_SPIKES, MOVE_BIDE, MOVE_RAPID_SPIN, MOVE_TAKE_DOWN},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SWIFT, MOVE_LIGHT_SCREEN, MOVE_SPARK, MOVE_SONIC_BOOM},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_PINECO,
        .moves = {MOVE_SPIKES, MOVE_BIDE, MOVE_RAPID_SPIN, MOVE_EXPLOSION},
    },
};

static const struct TrainerMon sParty_CrushGirlCyndy[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_HITMONTOP,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_CrushGirlJocelyn[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_TamerEvan[] = {
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_SANDSLASH,
    },
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_LICKITUNG,
    },
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_URSARING,
    },
};

static const struct TrainerMon sParty_PokemaniacMark2[] = {
    {
        .iv = TRAINER_MON_IVS(26, 2, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_RHYHORN,
    },
    {
        .iv = TRAINER_MON_IVS(26, 2, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_LICKITUNG,
    },
};

static const struct TrainerMon sParty_PkmnRangerLogan[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 37,
        .species = SPECIES_EXEGGCUTE,
        .moves = {MOVE_SLEEP_POWDER, MOVE_POISON_POWDER, MOVE_STUN_SPORE, MOVE_CONFUSION},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 40,
        .species = SPECIES_EXEGGUTOR,
        .moves = {MOVE_EGG_BOMB, MOVE_STOMP, MOVE_CONFUSION, MOVE_HYPNOSIS},
    },
};

static const struct TrainerMon sParty_PkmnRangerJackson[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_TANGELA,
        .moves = {MOVE_SLAM, MOVE_MEGA_DRAIN, MOVE_BIND, MOVE_INGRAIN},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_EXEGGCUTE,
        .moves = {MOVE_CONFUSION, MOVE_POISON_POWDER, MOVE_BARRAGE, MOVE_REFLECT},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_EXEGGUTOR,
        .moves = {MOVE_CONFUSION, MOVE_EGG_BOMB, MOVE_SLEEP_POWDER, MOVE_STOMP},
    },
};

static const struct TrainerMon sParty_PkmnRangerBeth[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_BELLSPROUT,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_GLOOM,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 38,
        .species = SPECIES_GLOOM,
    },
};

static const struct TrainerMon sParty_PkmnRangerKatelyn[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_CHANSEY,
        .moves = {MOVE_EGG_BOMB, MOVE_DEFENSE_CURL, MOVE_MINIMIZE, MOVE_SOFT_BOILED},
    },
};

static const struct TrainerMon sParty_CooltrainerLeroy[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_RHYDON,
        .moves = {MOVE_TAKE_DOWN, MOVE_HORN_DRILL, MOVE_ROCK_BLAST, MOVE_SCARY_FACE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_SLOWBRO,
        .moves = {MOVE_PSYCHIC, MOVE_HEADBUTT, MOVE_AMNESIA, MOVE_DISABLE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_KANGASKHAN,
        .moves = {MOVE_DIZZY_PUNCH, MOVE_BITE, MOVE_ENDURE, MOVE_REVERSAL},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_MACHOKE,
        .moves = {MOVE_CROSS_CHOP, MOVE_VITAL_THROW, MOVE_REVENGE, MOVE_SEISMIC_TOSS},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_URSARING,
        .moves = {MOVE_SLASH, MOVE_FAINT_ATTACK, MOVE_SNORE, MOVE_REST},
    },
};

static const struct TrainerMon sParty_CooltrainerMichelle[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_PERSIAN,
        .moves = {MOVE_SLASH, MOVE_SCREECH, MOVE_FAINT_ATTACK, MOVE_BITE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_DEWGONG,
        .moves = {MOVE_ICE_BEAM, MOVE_TAKE_DOWN, MOVE_ICY_WIND, MOVE_GROWL},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_NINETALES,
        .moves = {MOVE_FLAMETHROWER, MOVE_CONFUSE_RAY, MOVE_WILL_O_WISP, MOVE_GRUDGE},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_RAPIDASH,
        .moves = {MOVE_BOUNCE, MOVE_AGILITY, MOVE_FIRE_SPIN, MOVE_TAKE_DOWN},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_GIRAFARIG,
        .moves = {MOVE_CRUNCH, MOVE_PSYBEAM, MOVE_STOMP, MOVE_ODOR_SLEUTH},
    },
};

static const struct TrainerMon sParty_CoolCoupleLexNya[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_MILTANK,
        .moves = {MOVE_BODY_SLAM, MOVE_MILK_DRINK, MOVE_GROWL, MOVE_DEFENSE_CURL},
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_TAUROS,
        .moves = {MOVE_THRASH, MOVE_HORN_ATTACK, MOVE_PURSUIT, MOVE_SWAGGER},
    },
};

static const struct TrainerMon sParty_RuinManiacBrandon[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_ONIX,
    },
};

static const struct TrainerMon sParty_RuinManiacBenjamin[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_GEODUDE,
        .moves = {MOVE_EARTHQUAKE, MOVE_ROCK_BLAST, MOVE_ROLLOUT, MOVE_SELF_DESTRUCT},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_GRAVELER,
        .moves = {MOVE_EARTHQUAKE, MOVE_ROCK_BLAST, MOVE_ROCK_THROW, MOVE_SELF_DESTRUCT},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_GRAVELER,
        .moves = {MOVE_EARTHQUAKE, MOVE_ROCK_BLAST, MOVE_ROCK_THROW, MOVE_SELF_DESTRUCT},
    },
};

static const struct TrainerMon sParty_PainterEdna[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_SMEARGLE,
        .moves = {MOVE_FAKE_OUT, MOVE_EXTREME_SPEED, MOVE_PROTECT, MOVE_QUICK_ATTACK},
    },
};

static const struct TrainerMon sParty_GentlemanClifford[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_MAROWAK,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_GOLDUCK,
    },
};

static const struct TrainerMon sParty_LadySelphy[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 31, 0, 0),
        .lvl = 49,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_NUGGET,
        .moves = {MOVE_PAY_DAY, MOVE_BITE, MOVE_TAUNT, MOVE_TORMENT},
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 31, 0, 0),
        .lvl = 49,
        .species = SPECIES_PERSIAN,
        .heldItem = ITEM_NUGGET,
        .moves = {MOVE_PAY_DAY, MOVE_SCRATCH, MOVE_TORMENT, MOVE_TAUNT},
    },
};

static const struct TrainerMon sParty_RuinManiacLawson[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_ONIX,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_GRAVELER,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_MAROWAK,
    },
};

static const struct TrainerMon sParty_PsychicLaura[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_NATU,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_NATU,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_XATU,
    },
};

static const struct TrainerMon sParty_PkmnBreederBethany[] = {
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_CHANSEY,
    },
};

static const struct TrainerMon sParty_PkmnBreederAllison[] = {
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .iv = TRAINER_MON_IVS(30, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_CLEFABLE,
    },
};

static const struct TrainerMon sParty_BugCatcherGarret[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_HERACROSS,
    },
};

static const struct TrainerMon sParty_BugCatcherJonah[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 45,
        .species = SPECIES_YANMA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 45,
        .species = SPECIES_BEEDRILL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 46,
        .species = SPECIES_YANMA,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_BEEDRILL,
    },
};

static const struct TrainerMon sParty_BugCatcherVance[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_VENONAT,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_VENOMOTH,
    },
};

static const struct TrainerMon sParty_YoungsterNash[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_WEEPINBELL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_WEEPINBELL,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_VICTREEBEL,
    },
};

static const struct TrainerMon sParty_YoungsterCordell[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_FARFETCHD,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_FARFETCHD,
    },
};

static const struct TrainerMon sParty_LassDalia[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 46,
        .species = SPECIES_HOPPIP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_HOPPIP,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_SKIPLOOM,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_SKIPLOOM,
    },
};

static const struct TrainerMon sParty_LassJoana[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_SNUBBULL,
    },
};

static const struct TrainerMon sParty_CamperRiley[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_PINSIR,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_HERACROSS,
    },
};

static const struct TrainerMon sParty_PicnickerMarcy[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_PARAS,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_PARAS,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_PARASECT,
    },
};

static const struct TrainerMon sParty_RuinManiacLayton[] = {
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_SANDSLASH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_ONIX,
    },
    {
        .iv = TRAINER_MON_IVS(0, 0, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_SANDSLASH,
    },
};

static const struct TrainerMon sParty_PicnickerKelsey2[] = {
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_NIDORAN_M,
    },
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_NIDORAN_F,
    },
};

static const struct TrainerMon sParty_PicnickerKelsey3[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_NIDORINO,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_NIDORINA,
    },
};

static const struct TrainerMon sParty_PicnickerKelsey4[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_NIDORINO,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_NIDORINA,
    },
};

static const struct TrainerMon sParty_CamperRicky2[] = {
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_SQUIRTLE,
    },
};

static const struct TrainerMon sParty_CamperRicky3[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_WARTORTLE,
    },
};

static const struct TrainerMon sParty_CamperRicky4[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_WARTORTLE,
    },
};

static const struct TrainerMon sParty_CamperJeff2[] = {
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_SPEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMon sParty_CamperJeff3[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMon sParty_CamperJeff4[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMon sParty_PicnickerIsabelle2[] = {
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 18,
        .species = SPECIES_PIDGEY,
    },
};

static const struct TrainerMon sParty_PicnickerIsabelle3[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 26,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMon sParty_PicnickerIsabelle4[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_PIDGEOT,
    },
};

static const struct TrainerMon sParty_YoungsterYasu2[] = {
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMon sParty_YoungsterYasu3[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMon sParty_EngineerBernie2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_MAGNETON,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_MAGNETON,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_MAGNETON,
    },
};

static const struct TrainerMon sParty_GamerDarian2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_VULPIX,
    },
};

static const struct TrainerMon sParty_CamperChris2[] = {
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 24,
        .species = SPECIES_CHARMANDER,
    },
};

static const struct TrainerMon sParty_CamperChris3[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_CHARMELEON,
    },
};

static const struct TrainerMon sParty_CamperChris4[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_ARCANINE,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_CHARMELEON,
    },
};

static const struct TrainerMon sParty_PicnickerAlicia2[] = {
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 25,
        .species = SPECIES_MEOWTH,
    },
};

static const struct TrainerMon sParty_PicnickerAlicia3[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_PERSIAN,
    },
};

static const struct TrainerMon sParty_PicnickerAlicia4[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_PERSIAN,
    },
};

static const struct TrainerMon sParty_HikerJeremy2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_MACHOKE,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_ONIX,
    },
};

static const struct TrainerMon sParty_PokemaniacMark3[] = {
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_RHYDON,
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_LICKITUNG,
    },
};

static const struct TrainerMon sParty_PokemaniacHerman2[] = {
    {
        .iv = TRAINER_MON_IVS(26, 2, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_MAROWAK,
    },
    {
        .iv = TRAINER_MON_IVS(26, 2, 0, 0, 0, 0),
        .lvl = 29,
        .species = SPECIES_SLOWBRO,
    },
};

static const struct TrainerMon sParty_PokemaniacHerman3[] = {
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_MAROWAK,
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_SLOWBRO,
    },
};

static const struct TrainerMon sParty_HikerTrent2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_ONIX,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 31,
        .species = SPECIES_GRAVELER,
    },
};

static const struct TrainerMon sParty_LassMegan2[] = {
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 23,
        .species = SPECIES_NIDORAN_M,
    },
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 21,
        .species = SPECIES_MEOWTH,
    },
    {
        .iv = TRAINER_MON_IVS(8, 1, 0, 0, 0, 0),
        .lvl = 22,
        .species = SPECIES_PIKACHU,
    },
};

static const struct TrainerMon sParty_LassMegan3[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 46,
        .species = SPECIES_PIDGEOT,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_NIDORINO,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_PERSIAN,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_RAICHU,
    },
};

static const struct TrainerMon sParty_SuperNerdGlenn2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_MUK,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_MUK,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_MUK,
    },
};

static const struct TrainerMon sParty_GamerRich2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_VULPIX,
    },
};

static const struct TrainerMon sParty_BikerJaren2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_MUK,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_MUK,
    },
};

static const struct TrainerMon sParty_FishermanElliot2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_POLIWHIRL,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_CLOYSTER,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_SEAKING,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_SEADRA,
    },
};

static const struct TrainerMon sParty_RockerLuca2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_ELECTRODE,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_ELECTRODE,
    },
};

static const struct TrainerMon sParty_BeautySheila2[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_PERSIAN,
    },
};

static const struct TrainerMon sParty_BirdKeeperRobert2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMon sParty_BirdKeeperRobert3[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_PIDGEOT,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_PIDGEOT,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMon sParty_PicnickerSusie2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_MEOWTH,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_PIKACHU,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_MEOWTH,
    },
};

static const struct TrainerMon sParty_PicnickerSusie3[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_PERSIAN,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_PIKACHU,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_PERSIAN,
    },
};

static const struct TrainerMon sParty_PicnickerSusie4[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_PIDGEOT,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_PERSIAN,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_RAICHU,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_PERSIAN,
    },
};

static const struct TrainerMon sParty_BikerLukas2[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_KOFFING,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_KOFFING,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_MUK,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_WEEZING,
    },
};

static const struct TrainerMon sParty_BirdKeeperBenny2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 32,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 32,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMon sParty_BirdKeeperBenny3[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMon sParty_BirdKeeperMarlon2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_DODUO,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMon sParty_BirdKeeperMarlon3[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_DODRIO,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMon sParty_BeautyGrace2[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_PIDGEOT,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_WIGGLYTUFF,
    },
};

static const struct TrainerMon sParty_BirdKeeperChester2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_DODRIO,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_DODRIO,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_DODUO,
    },
};

static const struct TrainerMon sParty_BirdKeeperChester3[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_DODRIO,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_DODRIO,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_DODRIO,
    },
};

static const struct TrainerMon sParty_PicnickerBecky2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 32,
        .species = SPECIES_PIKACHU,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 32,
        .species = SPECIES_RAICHU,
    },
};

static const struct TrainerMon sParty_PicnickerBecky3[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_PIKACHU,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_RAICHU,
    },
};

static const struct TrainerMon sParty_PicnickerBecky4[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_RAICHU,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_RAICHU,
    },
};

static const struct TrainerMon sParty_CrushKinRonMya2[] = {
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(4, 3, 0, 0, 0, 0),
        .lvl = 33,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_CrushKinRonMya3[] = {
    {
        .iv = TRAINER_MON_IVS(2, 4, 0, 0, 0, 0),
        .lvl = 51,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(2, 4, 0, 0, 0, 0),
        .lvl = 51,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_CrushKinRonMya4[] = {
    {
        .iv = TRAINER_MON_IVS(10, 5, 0, 0, 0, 0),
        .lvl = 56,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(10, 5, 0, 0, 0, 0),
        .lvl = 56,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_BikerRuben2[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_WEEZING,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_WEEZING,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 48,
        .species = SPECIES_WEEZING,
    },
};

static const struct TrainerMon sParty_CueBallCamron2[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_PRIMEAPE,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_MACHOKE,
    },
};

static const struct TrainerMon sParty_BikerJaxon2[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_WEEZING,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_MUK,
    },
};

static const struct TrainerMon sParty_CueBallIsaiah2[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_MACHOKE,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_MACHAMP,
    },
};

static const struct TrainerMon sParty_CueBallCorey2[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_PRIMEAPE,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_MACHAMP,
    },
};

static const struct TrainerMon sParty_BirdKeeperJacob2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_SPEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 28,
        .species = SPECIES_SPEAROW,
    },
};

static const struct TrainerMon sParty_BirdKeeperJacob3[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMon sParty_SwimmerFemaleAlice2[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_SEAKING,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMon sParty_SwimmerMaleDarrin2[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_SEADRA,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_SEADRA,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_SEADRA,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_SEADRA,
    },
};

static const struct TrainerMon sParty_PicnickerMissy2[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_SEAKING,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMon sParty_PicnickerMissy3[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_SEAKING,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMon sParty_FishermanWade2[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_MAGIKARP,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_MAGIKARP,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_MAGIKARP,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_MAGIKARP,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_MAGIKARP,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_MAGIKARP,
    },
};

static const struct TrainerMon sParty_SwimmerMaleJack2[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_STARMIE,
    },
};

static const struct TrainerMon sParty_SisAndBroLilIan2[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_SEADRA,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_STARMIE,
    },
};

static const struct TrainerMon sParty_SisAndBroLilIan3[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_SEADRA,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_STARMIE,
    },
};

static const struct TrainerMon sParty_SwimmerMaleFinn2[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_STARMIE,
    },
};

static const struct TrainerMon sParty_CrushGirlSharon2[] = {
    {
        .iv = TRAINER_MON_IVS(2, 4, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_MANKEY,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(2, 4, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_CrushGirlSharon3[] = {
    {
        .iv = TRAINER_MON_IVS(10, 5, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(10, 5, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_CrushGirlTanya2[] = {
    {
        .iv = TRAINER_MON_IVS(2, 4, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(2, 4, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_CrushGirlTanya3[] = {
    {
        .iv = TRAINER_MON_IVS(10, 5, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(10, 5, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_BlackBeltShea2[] = {
    {
        .iv = TRAINER_MON_IVS(20, 5, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(20, 5, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_BlackBeltShea3[] = {
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_BlackBeltHugh2[] = {
    {
        .iv = TRAINER_MON_IVS(20, 5, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(20, 5, 0, 0, 0, 0),
        .lvl = 50,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_BlackBeltHugh3[] = {
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_CrushKinMikKia2[] = {
    {
        .iv = TRAINER_MON_IVS(2, 4, 0, 0, 0, 0),
        .lvl = 51,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(2, 4, 0, 0, 0, 0),
        .lvl = 51,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_CrushKinMikKia3[] = {
    {
        .iv = TRAINER_MON_IVS(10, 5, 0, 0, 0, 0),
        .lvl = 56,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(10, 5, 0, 0, 0, 0),
        .lvl = 56,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_TuberAmira2[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_POLIWHIRL,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_POLIWHIRL,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 47,
        .species = SPECIES_POLIWHIRL,
    },
};

static const struct TrainerMon sParty_TwinsJoyMeg2[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_CLEFAIRY,
    },
};

static const struct TrainerMon sParty_PainterRayna2[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_SMEARGLE,
        .moves = {MOVE_CROSS_CHOP, MOVE_MEGAHORN, MOVE_DOUBLE_EDGE, MOVE_SELF_DESTRUCT},
    },
};

static const struct TrainerMon sParty_YoungsterDestin2[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_PIDGEOT,
    },
};

static const struct TrainerMon sParty_PkmnBreederAlize2[] = {
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_PIKACHU,
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_MARILL,
    },
};

static const struct TrainerMon sParty_YoungCoupleGiaJes2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_NIDORINA,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 30,
        .species = SPECIES_NIDORINO,
    },
};

static const struct TrainerMon sParty_YoungCoupleGiaJes3[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_NIDOKING,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_NIDOQUEEN,
    },
};

static const struct TrainerMon sParty_BirdKeeperMilo2[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_PIDGEOT,
    },
};

static const struct TrainerMon sParty_BirdKeeperChaz2[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMon sParty_BirdKeeperHarold2[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_NOCTOWL,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_NOCTOWL,
    },
};

static const struct TrainerMon sParty_SwimmerFemaleNicole2[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_MARILL,
    },
};

static const struct TrainerMon sParty_PsychicJaclyn2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_NATU,
        .moves = {MOVE_PSYCHIC, MOVE_CONFUSE_RAY, MOVE_FUTURE_SIGHT, MOVE_WISH},
    },
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_SLOWBRO,
        .moves = {MOVE_PSYCHIC, MOVE_HEADBUTT, MOVE_AMNESIA, MOVE_YAWN},
    },
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_KADABRA,
        .moves = {MOVE_PSYCHIC, MOVE_FUTURE_SIGHT, MOVE_RECOVER, MOVE_REFLECT},
    },
};

static const struct TrainerMon sParty_SwimmerMaleSamir2[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_GYARADOS,
    },
};

static const struct TrainerMon sParty_HikerEarl2[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_ONIX,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_MACHAMP,
    },
};

static const struct TrainerMon sParty_RuinManiacLarry2[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_MACHOKE,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_MACHOKE,
    },
};

static const struct TrainerMon sParty_PokemaniacHector2[] = {
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_RHYDON,
    },
    {
        .iv = TRAINER_MON_IVS(22, 4, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_KANGASKHAN,
    },
};

static const struct TrainerMon sParty_PsychicDario2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 56,
        .species = SPECIES_GIRAFARIG,
        .moves = {MOVE_CRUNCH, MOVE_PSYBEAM, MOVE_ODOR_SLEUTH, MOVE_AGILITY},
    },
};

static const struct TrainerMon sParty_PsychicRodette2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_NATU,
        .moves = {MOVE_PSYCHIC, MOVE_CONFUSE_RAY, MOVE_WISH, MOVE_FUTURE_SIGHT},
    },
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_HYPNO,
        .moves = {MOVE_PSYCHIC, MOVE_DISABLE, MOVE_PSYCH_UP, MOVE_FUTURE_SIGHT},
    },
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_HYPNO,
        .moves = {MOVE_PSYCHIC, MOVE_HYPNOSIS, MOVE_PSYCH_UP, MOVE_FUTURE_SIGHT},
    },
};

static const struct TrainerMon sParty_JugglerMason2[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_ELECTRODE,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_PINECO,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_ELECTRODE,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_PINECO,
    },
};

static const struct TrainerMon sParty_PkmnRangerNicolas2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_VICTREEBEL,
        .moves = {MOVE_RAZOR_LEAF, MOVE_ACID, MOVE_STUN_SPORE, MOVE_WRAP},
    },
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_VICTREEBEL,
        .moves = {MOVE_RAZOR_LEAF, MOVE_ACID, MOVE_SLEEP_POWDER, MOVE_SLAM},
    },
};

static const struct TrainerMon sParty_PkmnRangerMadeline2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_VILEPLUME,
        .moves = {MOVE_PETAL_DANCE, MOVE_MOONLIGHT, MOVE_ACID, MOVE_SLEEP_POWDER},
    },
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_VILEPLUME,
        .moves = {MOVE_PETAL_DANCE, MOVE_MOONLIGHT, MOVE_ACID, MOVE_STUN_SPORE},
    },
};

static const struct TrainerMon sParty_CrushGirlCyndy2[] = {
    {
        .iv = TRAINER_MON_IVS(10, 5, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(10, 5, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_HITMONTOP,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = TRAINER_MON_IVS(10, 5, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMon sParty_TamerEvan2[] = {
    {
        .iv = TRAINER_MON_IVS(0, 5, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_SANDSLASH,
    },
    {
        .iv = TRAINER_MON_IVS(0, 5, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_LICKITUNG,
    },
    {
        .iv = TRAINER_MON_IVS(0, 5, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_URSARING,
    },
};

static const struct TrainerMon sParty_PkmnRangerJackson2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_TANGELA,
        .moves = {MOVE_SLAM, MOVE_MEGA_DRAIN, MOVE_BIND, MOVE_INGRAIN},
    },
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_EXEGGUTOR,
        .moves = {MOVE_CONFUSION, MOVE_EGG_BOMB, MOVE_STUN_SPORE, MOVE_REFLECT},
    },
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_EXEGGUTOR,
        .moves = {MOVE_CONFUSION, MOVE_EGG_BOMB, MOVE_SLEEP_POWDER, MOVE_STOMP},
    },
};

static const struct TrainerMon sParty_PkmnRangerKatelyn2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 56,
        .species = SPECIES_CHANSEY,
        .moves = {MOVE_EGG_BOMB, MOVE_DEFENSE_CURL, MOVE_MINIMIZE, MOVE_SOFT_BOILED},
    },
};

static const struct TrainerMon sParty_CooltrainerLeroy2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_RHYDON,
        .moves = {MOVE_EARTHQUAKE, MOVE_HORN_DRILL, MOVE_ROCK_BLAST, MOVE_SCARY_FACE},
    },
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_SLOWBRO,
        .moves = {MOVE_PSYCHIC, MOVE_HEADBUTT, MOVE_AMNESIA, MOVE_DISABLE},
    },
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_KANGASKHAN,
        .moves = {MOVE_DIZZY_PUNCH, MOVE_BITE, MOVE_ENDURE, MOVE_REVERSAL},
    },
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 52,
        .species = SPECIES_MACHAMP,
        .moves = {MOVE_CROSS_CHOP, MOVE_VITAL_THROW, MOVE_REVENGE, MOVE_SEISMIC_TOSS},
    },
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 55,
        .species = SPECIES_URSARING,
        .moves = {MOVE_SLASH, MOVE_FAINT_ATTACK, MOVE_SNORE, MOVE_REST},
    },
};

static const struct TrainerMon sParty_CooltrainerMichelle2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_PERSIAN,
        .moves = {MOVE_SLASH, MOVE_SCREECH, MOVE_FAINT_ATTACK, MOVE_BITE},
    },
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 53,
        .species = SPECIES_DEWGONG,
        .moves = {MOVE_ICE_BEAM, MOVE_TAKE_DOWN, MOVE_ICY_WIND, MOVE_SHEER_COLD},
    },
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_NINETALES,
        .moves = {MOVE_FLAMETHROWER, MOVE_CONFUSE_RAY, MOVE_WILL_O_WISP, MOVE_GRUDGE},
    },
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_RAPIDASH,
        .moves = {MOVE_BOUNCE, MOVE_AGILITY, MOVE_FIRE_SPIN, MOVE_TAKE_DOWN},
    },
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 56,
        .species = SPECIES_GIRAFARIG,
        .moves = {MOVE_CRUNCH, MOVE_PSYBEAM, MOVE_STOMP, MOVE_ODOR_SLEUTH},
    },
};

static const struct TrainerMon sParty_CoolCoupleLexNya2[] = {
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 57,
        .species = SPECIES_MILTANK,
        .moves = {MOVE_BODY_SLAM, MOVE_MILK_DRINK, MOVE_GROWL, MOVE_DEFENSE_CURL},
    },
    {
        .iv = TRAINER_MON_IVS(28, 6, 0, 0, 0, 0),
        .lvl = 57,
        .species = SPECIES_TAUROS,
        .moves = {MOVE_TAKE_DOWN, MOVE_SCARY_FACE, MOVE_PURSUIT, MOVE_SWAGGER},
    },
};

static const struct TrainerMon sParty_BugCatcherColton2[] = {
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_METAPOD,
    },
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_WEEDLE,
    },
    {
        .iv = TRAINER_MON_IVS(20, 0, 0, 0, 0, 0),
        .lvl = 19,
        .species = SPECIES_METAPOD,
    },
};

static const struct TrainerMon sParty_BugCatcherColton3[] = {
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_BUTTERFREE,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_KAKUNA,
    },
    {
        .iv = TRAINER_MON_IVS(28, 1, 0, 0, 0, 0),
        .lvl = 27,
        .species = SPECIES_BUTTERFREE,
    },
};

static const struct TrainerMon sParty_BugCatcherColton4[] = {
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 51,
        .species = SPECIES_BUTTERFREE,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 54,
        .species = SPECIES_BEEDRILL,
    },
    {
        .iv = TRAINER_MON_IVS(24, 3, 0, 0, 0, 0),
        .lvl = 51,
        .species = SPECIES_BUTTERFREE,
    },
};

static const struct TrainerMon sParty_SwimmerMaleMatthew2[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_POLIWHIRL,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_POLIWRATH,
    },
};

static const struct TrainerMon sParty_SwimmerMaleTony2[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_SEADRA,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_SEADRA,
    },
};

static const struct TrainerMon sParty_SwimmerFemaleMelissa2[] = {
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_POLIWHIRL,
    },
    {
        .iv = TRAINER_MON_IVS(16, 2, 0, 0, 0, 0),
        .lvl = 49,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMon sParty_EliteFourLorelei2[] = {
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 31, 0, 0),
        .lvl = 64,
        .species = SPECIES_DEWGONG,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_ICE_BEAM, MOVE_SURF, MOVE_SIGNAL_BEAM, MOVE_DOUBLE_TEAM},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 31, 0, 0),
        .lvl = 63,
        .species = SPECIES_CLOYSTER,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_ICE_BEAM, MOVE_SURF, MOVE_SUPERSONIC, MOVE_RAIN_DANCE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 31, 0, 0),
        .lvl = 63,
        .species = SPECIES_PILOSWINE,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_BLIZZARD, MOVE_EARTHQUAKE, MOVE_DOUBLE_EDGE, MOVE_ROCK_SLIDE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 31, 0, 0),
        .lvl = 66,
        .species = SPECIES_JYNX,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_ICE_BEAM, MOVE_PSYCHIC, MOVE_LOVELY_KISS, MOVE_ATTRACT},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 31, 0, 0),
        .lvl = 66,
        .species = SPECIES_LAPRAS,
        .heldItem = ITEM_CHERI_BERRY,
        .moves = {MOVE_ICE_BEAM, MOVE_SURF, MOVE_PSYCHIC, MOVE_THUNDER},
    },
};

static const struct TrainerMon sParty_EliteFourBruno2[] = {
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 31, 0, 0),
        .lvl = 65,
        .species = SPECIES_STEELIX,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_EARTHQUAKE, MOVE_IRON_TAIL, MOVE_CRUNCH, MOVE_ROCK_TOMB},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 31, 0, 0),
        .lvl = 65,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_SKY_UPPERCUT, MOVE_MACH_PUNCH, MOVE_ROCK_SLIDE, MOVE_COUNTER},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 31, 0, 0),
        .lvl = 65,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_MEGA_KICK, MOVE_FORESIGHT, MOVE_EARTHQUAKE, MOVE_ROCK_SLIDE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 31, 0, 0),
        .lvl = 66,
        .species = SPECIES_STEELIX,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_EARTHQUAKE, MOVE_IRON_TAIL, MOVE_CRUNCH, MOVE_DRAGON_BREATH},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 31, 0, 0),
        .lvl = 68,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_PERSIM_BERRY,
        .moves = {MOVE_CROSS_CHOP, MOVE_EARTHQUAKE, MOVE_BRICK_BREAK, MOVE_ROCK_SLIDE},
    },
};

static const struct TrainerMon sParty_EliteFourAgatha2[] = {
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 31, 0, 0),
        .lvl = 66,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_SHADOW_BALL, MOVE_PSYCHIC, MOVE_CONFUSE_RAY, MOVE_HYPNOSIS},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 31, 0, 0),
        .lvl = 66,
        .species = SPECIES_CROBAT,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_AIR_CUTTER, MOVE_SHADOW_BALL, MOVE_CONFUSE_RAY},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 31, 0, 0),
        .lvl = 65,
        .species = SPECIES_MISDREAVUS,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_SHADOW_BALL, MOVE_PSYCHIC, MOVE_THUNDERBOLT, MOVE_ATTRACT},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 31, 0, 0),
        .lvl = 68,
        .species = SPECIES_ARBOK,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_SLUDGE_BOMB, MOVE_EARTHQUAKE, MOVE_GIGA_DRAIN, MOVE_DOUBLE_TEAM},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 31, 0, 0),
        .lvl = 70,
        .species = SPECIES_GENGAR,
        .heldItem = ITEM_CHESTO_BERRY,
        .moves = {MOVE_SHADOW_BALL, MOVE_PSYCHIC, MOVE_THUNDERBOLT, MOVE_SLUDGE_BOMB},
    },
};

static const struct TrainerMon sParty_EliteFourLance2[] = {
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 31, 0, 0),
        .lvl = 68,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYPER_BEAM, MOVE_DRAGON_DANCE, MOVE_EARTHQUAKE, MOVE_THUNDER_WAVE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 31, 0, 0),
        .lvl = 66,
        .species = SPECIES_DRAGONITE,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYPER_BEAM, MOVE_EARTHQUAKE, MOVE_DRAGON_CLAW, MOVE_FLAMETHROWER},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 31, 0, 0),
        .lvl = 66,
        .species = SPECIES_KINGDRA,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYPER_BEAM, MOVE_DRAGON_DANCE, MOVE_SURF, MOVE_ICE_BEAM},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 31, 0, 0),
        .lvl = 70,
        .species = SPECIES_AERODACTYL,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYPER_BEAM, MOVE_ANCIENT_POWER, MOVE_AERIAL_ACE, MOVE_EARTHQUAKE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 0, 0, 31, 0, 0),
        .lvl = 72,
        .species = SPECIES_DRAGONITE,
        .heldItem = ITEM_PERSIM_BERRY,
        .moves = {MOVE_HYPER_BEAM, MOVE_OUTRAGE, MOVE_THUNDERBOLT, MOVE_ICE_BEAM},
    },
};

static const struct TrainerMon sParty_ChampionRematchSquirtle[] = {
    {
        .iv = TRAINER_MON_IVS(31, 10, 0, 31, 10, 0),
        .lvl = 72,
        .species = SPECIES_HERACROSS,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_MEGAHORN, MOVE_EARTHQUAKE, MOVE_COUNTER, MOVE_ROCK_TOMB},
    },
    {
        .iv = TRAINER_MON_IVS(31, 10, 0, 31, 10, 0),
        .lvl = 73,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_CALM_MIND, MOVE_REFLECT},
    },
    {
        .iv = TRAINER_MON_IVS(31, 10, 0, 31, 10, 0),
        .lvl = 72,
        .species = SPECIES_TYRANITAR,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_CRUNCH, MOVE_EARTHQUAKE, MOVE_THUNDERBOLT, MOVE_AERIAL_ACE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 10, 0, 31, 10, 0),
        .lvl = 73,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_EXTREME_SPEED, MOVE_OVERHEAT, MOVE_AERIAL_ACE, MOVE_IRON_TAIL},
    },
    {
        .iv = TRAINER_MON_IVS(31, 10, 0, 31, 10, 0),
        .lvl = 73,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_GIGA_DRAIN, MOVE_PSYCHIC, MOVE_SLEEP_POWDER, MOVE_LIGHT_SCREEN},
    },
    {
        .iv = TRAINER_MON_IVS(31, 10, 0, 31, 10, 0),
        .lvl = 75,
        .species = SPECIES_BLASTOISE,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDRO_PUMP, MOVE_ICE_BEAM, MOVE_EARTHQUAKE, MOVE_RAIN_DANCE},
    },
};

static const struct TrainerMon sParty_ChampionRematchBulbasaur[] = {
    {
        .iv = TRAINER_MON_IVS(31, 10, 0, 31, 10, 0),
        .lvl = 72,
        .species = SPECIES_HERACROSS,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_MEGAHORN, MOVE_EARTHQUAKE, MOVE_COUNTER, MOVE_ROCK_TOMB},
    },
    {
        .iv = TRAINER_MON_IVS(31, 10, 0, 31, 10, 0),
        .lvl = 73,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_CALM_MIND, MOVE_REFLECT},
    },
    {
        .iv = TRAINER_MON_IVS(31, 10, 0, 31, 10, 0),
        .lvl = 72,
        .species = SPECIES_TYRANITAR,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_CRUNCH, MOVE_EARTHQUAKE, MOVE_THUNDERBOLT, MOVE_AERIAL_ACE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 10, 0, 31, 10, 0),
        .lvl = 73,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYDRO_PUMP, MOVE_DRAGON_DANCE, MOVE_EARTHQUAKE, MOVE_HYPER_BEAM},
    },
    {
        .iv = TRAINER_MON_IVS(31, 10, 0, 31, 10, 0),
        .lvl = 73,
        .species = SPECIES_ARCANINE,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_EXTREME_SPEED, MOVE_OVERHEAT, MOVE_AERIAL_ACE, MOVE_IRON_TAIL},
    },
    {
        .iv = TRAINER_MON_IVS(31, 10, 0, 31, 10, 0),
        .lvl = 75,
        .species = SPECIES_VENUSAUR,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SOLAR_BEAM, MOVE_SLUDGE_BOMB, MOVE_EARTHQUAKE, MOVE_SUNNY_DAY},
    },
};

static const struct TrainerMon sParty_ChampionRematchCharmander[] = {
    {
        .iv = TRAINER_MON_IVS(31, 10, 0, 31, 10, 0),
        .lvl = 72,
        .species = SPECIES_HERACROSS,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_MEGAHORN, MOVE_EARTHQUAKE, MOVE_COUNTER, MOVE_ROCK_TOMB},
    },
    {
        .iv = TRAINER_MON_IVS(31, 10, 0, 31, 10, 0),
        .lvl = 73,
        .species = SPECIES_ALAKAZAM,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOW_BALL, MOVE_CALM_MIND, MOVE_REFLECT},
    },
    {
        .iv = TRAINER_MON_IVS(31, 10, 0, 31, 10, 0),
        .lvl = 72,
        .species = SPECIES_TYRANITAR,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_CRUNCH, MOVE_EARTHQUAKE, MOVE_THUNDERBOLT, MOVE_AERIAL_ACE},
    },
    {
        .iv = TRAINER_MON_IVS(31, 10, 0, 31, 10, 0),
        .lvl = 73,
        .species = SPECIES_EXEGGUTOR,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_GIGA_DRAIN, MOVE_PSYCHIC, MOVE_SLEEP_POWDER, MOVE_LIGHT_SCREEN},
    },
    {
        .iv = TRAINER_MON_IVS(31, 10, 0, 31, 10, 0),
        .lvl = 73,
        .species = SPECIES_GYARADOS,
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYDRO_PUMP, MOVE_DRAGON_DANCE, MOVE_EARTHQUAKE, MOVE_HYPER_BEAM},
    },
    {
        .iv = TRAINER_MON_IVS(31, 10, 0, 31, 10, 0),
        .lvl = 75,
        .species = SPECIES_CHARIZARD,
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FIRE_BLAST, MOVE_DRAGON_CLAW, MOVE_AERIAL_ACE, MOVE_EARTHQUAKE},
    },
};

static const struct TrainerMon sParty_CueBallPaxton[] = {
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 39,
        .species = SPECIES_WEEZING,
    },
    {
        .iv = TRAINER_MON_IVS(18, 1, 0, 0, 0, 0),
        .lvl = 39,
        .species = SPECIES_MUK,
    },
};

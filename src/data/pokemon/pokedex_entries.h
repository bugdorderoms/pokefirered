const struct PokedexEntry gPokedexEntries[] =
{
    [NATIONAL_DEX_NONE] =
    {
        .categoryName = _("Unknown"),
        .height = 0,
        .weight = 0,
        .description = gDummyPokedexText,
    },

    [NATIONAL_DEX_BULBASAUR] =
    {
        .categoryName = _("Seed"),
        .height = 7,
        .weight = 69,
        .description = gBulbasaurPokedexText,
    },

    [NATIONAL_DEX_IVYSAUR] =
    {
        .categoryName = _("Seed"),
        .height = 10,
        .weight = 130,
        .description = gIvysaurPokedexText,
    },

    [NATIONAL_DEX_VENUSAUR] =
    {
        .categoryName = _("Seed"),
        .height = 20,
        .weight = 1000,
        .description = gVenusaurPokedexText,
    },

    [NATIONAL_DEX_CHARMANDER] =
    {
        .categoryName = _("Lizard"),
        .height = 6,
        .weight = 85,
        .description = gCharmanderPokedexText,
    },

    [NATIONAL_DEX_CHARMELEON] =
    {
        .categoryName = _("Flame"),
        .height = 11,
        .weight = 190,
        .description = gCharmeleonPokedexText,
    },

    [NATIONAL_DEX_CHARIZARD] =
    {
        .categoryName = _("Flame"),
        .height = 17,
        .weight = 905,
        .description = gCharizardPokedexText,
    },

    [NATIONAL_DEX_SQUIRTLE] =
    {
        .categoryName = _("Tiny Turtle"),
        .height = 5,
        .weight = 90,
        .description = gSquirtlePokedexText,
    },

    [NATIONAL_DEX_WARTORTLE] =
    {
        .categoryName = _("Turtle"),
        .height = 10,
        .weight = 225,
        .description = gWartortlePokedexText,
    },

    [NATIONAL_DEX_BLASTOISE] =
    {
        .categoryName = _("Shellfish"),
        .height = 16,
        .weight = 855,
        .description = gBlastoisePokedexText,
    },

    [NATIONAL_DEX_CATERPIE] =
    {
        .categoryName = _("Worm"),
        .height = 3,
        .weight = 29,
        .description = gCaterpiePokedexText,
    },

    [NATIONAL_DEX_METAPOD] =
    {
        .categoryName = _("Cocoon"),
        .height = 7,
        .weight = 99,
        .description = gMetapodPokedexText,
    },

    [NATIONAL_DEX_BUTTERFREE] =
    {
        .categoryName = _("Butterfly"),
        .height = 11,
        .weight = 320,
        .description = gButterfreePokedexText,
    },

    [NATIONAL_DEX_WEEDLE] =
    {
        .categoryName = _("Hairy Bug"),
        .height = 3,
        .weight = 32,
        .description = gWeedlePokedexText,
    },

    [NATIONAL_DEX_KAKUNA] =
    {
        .categoryName = _("Cocoon"),
        .height = 6,
        .weight = 100,
        .description = gKakunaPokedexText,
    },

    [NATIONAL_DEX_BEEDRILL] =
    {
        .categoryName = _("Poison Bee"),
        .height = 10,
        .weight = 295,
        .description = gBeedrillPokedexText,
    },

    [NATIONAL_DEX_PIDGEY] =
    {
        .categoryName = _("Tiny Bird"),
        .height = 3,
        .weight = 18,
        .description = gPidgeyPokedexText,
    },

    [NATIONAL_DEX_PIDGEOTTO] =
    {
        .categoryName = _("Bird"),
        .height = 11,
        .weight = 300,
        .description = gPidgeottoPokedexText,
    },

    [NATIONAL_DEX_PIDGEOT] =
    {
        .categoryName = _("Bird"),
        .height = 15,
        .weight = 395,
        .description = gPidgeotPokedexText,
    },

    [NATIONAL_DEX_RATTATA] =
    {
        .categoryName = _("Mouse"),
        .height = 3,
        .weight = 35,
        .description = gRattataPokedexText,
    },

    [NATIONAL_DEX_RATICATE] =
    {
        .categoryName = _("Mouse"),
        .height = 7,
        .weight = 185,
        .description = gRaticatePokedexText,
    },

    [NATIONAL_DEX_SPEAROW] =
    {
        .categoryName = _("Tiny Bird"),
        .height = 3,
        .weight = 20,
        .description = gSpearowPokedexText,
    },

    [NATIONAL_DEX_FEAROW] =
    {
        .categoryName = _("Beak"),
        .height = 12,
        .weight = 380,
        .description = gFearowPokedexText,
    },

    [NATIONAL_DEX_EKANS] =
    {
        .categoryName = _("Snake"),
        .height = 20,
        .weight = 69,
        .description = gEkansPokedexText,
    },

    [NATIONAL_DEX_ARBOK] =
    {
        .categoryName = _("Cobra"),
        .height = 35,
        .weight = 650,
        .description = gArbokPokedexText,
    },

    [NATIONAL_DEX_PIKACHU] =
    {
        .categoryName = _("Mouse"),
        .height = 4,
        .weight = 60,
        .description = gPikachuPokedexText,
    },

    [NATIONAL_DEX_RAICHU] =
    {
        .categoryName = _("Mouse"),
        .height = 8,
        .weight = 300,
        .description = gRaichuPokedexText,
    },

    [NATIONAL_DEX_SANDSHREW] =
    {
        .categoryName = _("Mouse"),
        .height = 6,
        .weight = 120,
        .description = gSandshrewPokedexText,
    },

    [NATIONAL_DEX_SANDSLASH] =
    {
        .categoryName = _("Mouse"),
        .height = 10,
        .weight = 295,
        .description = gSandslashPokedexText,
    },

    [NATIONAL_DEX_NIDORAN_F] =
    {
        .categoryName = _("Poison Pin"),
        .height = 4,
        .weight = 70,
        .description = gNidoranFPokedexText,
    },

    [NATIONAL_DEX_NIDORINA] =
    {
        .categoryName = _("Poison Pin"),
        .height = 8,
        .weight = 200,
        .description = gNidorinaPokedexText,
    },

    [NATIONAL_DEX_NIDOQUEEN] =
    {
        .categoryName = _("Drill"),
        .height = 13,
        .weight = 600,
        .description = gNidoqueenPokedexText,
    },

    [NATIONAL_DEX_NIDORAN_M] =
    {
        .categoryName = _("Poison Pin"),
        .height = 5,
        .weight = 90,
        .description = gNidoranMPokedexText,
    },

    [NATIONAL_DEX_NIDORINO] =
    {
        .categoryName = _("Poison Pin"),
        .height = 9,
        .weight = 195,
        .description = gNidorinoPokedexText,
    },

    [NATIONAL_DEX_NIDOKING] =
    {
        .categoryName = _("Drill"),
        .height = 14,
        .weight = 620,
        .description = gNidokingPokedexText,
    },

    [NATIONAL_DEX_CLEFAIRY] =
    {
        .categoryName = _("Fairy"),
        .height = 6,
        .weight = 75,
        .description = gClefairyPokedexText,
    },

    [NATIONAL_DEX_CLEFABLE] =
    {
        .categoryName = _("Fairy"),
        .height = 13,
        .weight = 400,
        .description = gClefablePokedexText,
    },

    [NATIONAL_DEX_VULPIX] =
    {
        .categoryName = _("Fox"),
        .height = 6,
        .weight = 99,
        .description = gVulpixPokedexText,
    },

    [NATIONAL_DEX_NINETALES] =
    {
        .categoryName = _("Fox"),
        .height = 11,
        .weight = 199,
        .description = gNinetalesPokedexText,
    },

    [NATIONAL_DEX_JIGGLYPUFF] =
    {
        .categoryName = _("Balloon"),
        .height = 5,
        .weight = 55,
        .description = gJigglypuffPokedexText,
    },

    [NATIONAL_DEX_WIGGLYTUFF] =
    {
        .categoryName = _("Balloon"),
        .height = 10,
        .weight = 120,
        .description = gWigglytuffPokedexText,
    },

    [NATIONAL_DEX_ZUBAT] =
    {
        .categoryName = _("Bat"),
        .height = 8,
        .weight = 75,
        .description = gZubatPokedexText,
    },

    [NATIONAL_DEX_GOLBAT] =
    {
        .categoryName = _("Bat"),
        .height = 16,
        .weight = 550,
        .description = gGolbatPokedexText,
    },

    [NATIONAL_DEX_ODDISH] =
    {
        .categoryName = _("Weed"),
        .height = 5,
        .weight = 54,
        .description = gOddishPokedexText,
    },

    [NATIONAL_DEX_GLOOM] =
    {
        .categoryName = _("Weed"),
        .height = 8,
        .weight = 86,
        .description = gGloomPokedexText,
    },

    [NATIONAL_DEX_VILEPLUME] =
    {
        .categoryName = _("Flower"),
        .height = 12,
        .weight = 186,
        .description = gVileplumePokedexText,
    },

    [NATIONAL_DEX_PARAS] =
    {
        .categoryName = _("Mushroom"),
        .height = 3,
        .weight = 54,
        .description = gParasPokedexText,
    },

    [NATIONAL_DEX_PARASECT] =
    {
        .categoryName = _("Mushroom"),
        .height = 10,
        .weight = 295,
        .description = gParasectPokedexText,
    },

    [NATIONAL_DEX_VENONAT] =
    {
        .categoryName = _("Insect"),
        .height = 10,
        .weight = 300,
        .description = gVenonatPokedexText,
    },

    [NATIONAL_DEX_VENOMOTH] =
    {
        .categoryName = _("Poison Moth"),
        .height = 15,
        .weight = 125,
        .description = gVenomothPokedexText,
    },

    [NATIONAL_DEX_DIGLETT] =
    {
        .categoryName = _("Mole"),
        .height = 2,
        .weight = 8,
        .description = gDiglettPokedexText,
    },

    [NATIONAL_DEX_DUGTRIO] =
    {
        .categoryName = _("Mole"),
        .height = 7,
        .weight = 333,
        .description = gDugtrioPokedexText,
    },

    [NATIONAL_DEX_MEOWTH] =
    {
        .categoryName = _("Scratch Cat"),
        .height = 4,
        .weight = 42,
        .description = gMeowthPokedexText,
    },

    [NATIONAL_DEX_PERSIAN] =
    {
        .categoryName = _("Classy Cat"),
        .height = 10,
        .weight = 320,
        .description = gPersianPokedexText,
    },

    [NATIONAL_DEX_PSYDUCK] =
    {
        .categoryName = _("Duck"),
        .height = 8,
        .weight = 196,
        .description = gPsyduckPokedexText,
    },

    [NATIONAL_DEX_GOLDUCK] =
    {
        .categoryName = _("Duck"),
        .height = 17,
        .weight = 766,
        .description = gGolduckPokedexText,
    },

    [NATIONAL_DEX_MANKEY] =
    {
        .categoryName = _("Pig Monkey"),
        .height = 5,
        .weight = 280,
        .description = gMankeyPokedexText,
    },

    [NATIONAL_DEX_PRIMEAPE] =
    {
        .categoryName = _("Pig Monkey"),
        .height = 10,
        .weight = 320,
        .description = gPrimeapePokedexText,
    },

    [NATIONAL_DEX_GROWLITHE] =
    {
        .categoryName = _("Puppy"),
        .height = 7,
        .weight = 190,
        .description = gGrowlithePokedexText,
    },

    [NATIONAL_DEX_ARCANINE] =
    {
        .categoryName = _("Legendary"),
        .height = 19,
        .weight = 1550,
        .description = gArcaninePokedexText,
    },

    [NATIONAL_DEX_POLIWAG] =
    {
        .categoryName = _("Tadpole"),
        .height = 6,
        .weight = 124,
        .description = gPoliwagPokedexText,
    },

    [NATIONAL_DEX_POLIWHIRL] =
    {
        .categoryName = _("Tadpole"),
        .height = 10,
        .weight = 200,
        .description = gPoliwhirlPokedexText,
    },

    [NATIONAL_DEX_POLIWRATH] =
    {
        .categoryName = _("Tadpole"),
        .height = 13,
        .weight = 540,
        .description = gPoliwrathPokedexText,
    },

    [NATIONAL_DEX_ABRA] =
    {
        .categoryName = _("Psi"),
        .height = 9,
        .weight = 195,
        .description = gAbraPokedexText,
    },

    [NATIONAL_DEX_KADABRA] =
    {
        .categoryName = _("Psi"),
        .height = 13,
        .weight = 565,
        .description = gKadabraPokedexText,
    },

    [NATIONAL_DEX_ALAKAZAM] =
    {
        .categoryName = _("Psi"),
        .height = 15,
        .weight = 480,
        .description = gAlakazamPokedexText,
    },

    [NATIONAL_DEX_MACHOP] =
    {
        .categoryName = _("Superpower"),
        .height = 8,
        .weight = 195,
        .description = gMachopPokedexText,
    },

    [NATIONAL_DEX_MACHOKE] =
    {
        .categoryName = _("Superpower"),
        .height = 15,
        .weight = 705,
        .description = gMachokePokedexText,
    },

    [NATIONAL_DEX_MACHAMP] =
    {
        .categoryName = _("Superpower"),
        .height = 16,
        .weight = 1300,
        .description = gMachampPokedexText,
    },

    [NATIONAL_DEX_BELLSPROUT] =
    {
        .categoryName = _("Flower"),
        .height = 7,
        .weight = 40,
        .description = gBellsproutPokedexText,
    },

    [NATIONAL_DEX_WEEPINBELL] =
    {
        .categoryName = _("Flycatcher"),
        .height = 10,
        .weight = 64,
        .description = gWeepinbellPokedexText,
    },

    [NATIONAL_DEX_VICTREEBEL] =
    {
        .categoryName = _("Flycatcher"),
        .height = 17,
        .weight = 155,
        .description = gVictreebelPokedexText,
    },

    [NATIONAL_DEX_TENTACOOL] =
    {
        .categoryName = _("Jellyfish"),
        .height = 9,
        .weight = 455,
        .description = gTentacoolPokedexText,
    },

    [NATIONAL_DEX_TENTACRUEL] =
    {
        .categoryName = _("Jellyfish"),
        .height = 16,
        .weight = 550,
        .description = gTentacruelPokedexText,
    },

    [NATIONAL_DEX_GEODUDE] =
    {
        .categoryName = _("Rock"),
        .height = 4,
        .weight = 200,
        .description = gGeodudePokedexText,
    },

    [NATIONAL_DEX_GRAVELER] =
    {
        .categoryName = _("Rock"),
        .height = 10,
        .weight = 1050,
        .description = gGravelerPokedexText,
    },

    [NATIONAL_DEX_GOLEM] =
    {
        .categoryName = _("Megaton"),
        .height = 14,
        .weight = 3000,
        .description = gGolemPokedexText,
    },

    [NATIONAL_DEX_PONYTA] =
    {
        .categoryName = _("Fire Horse"),
        .height = 10,
        .weight = 300,
        .description = gPonytaPokedexText,
    },

    [NATIONAL_DEX_RAPIDASH] =
    {
        .categoryName = _("Fire Horse"),
        .height = 17,
        .weight = 950,
        .description = gRapidashPokedexText,
    },

    [NATIONAL_DEX_SLOWPOKE] =
    {
        .categoryName = _("Dopey"),
        .height = 12,
        .weight = 360,
        .description = gSlowpokePokedexText,
    },

    [NATIONAL_DEX_SLOWBRO] =
    {
        .categoryName = _("Hermit Crab"),
        .height = 16,
        .weight = 785,
        .description = gSlowbroPokedexText,
    },

    [NATIONAL_DEX_MAGNEMITE] =
    {
        .categoryName = _("Magnet"),
        .height = 3,
        .weight = 60,
        .description = gMagnemitePokedexText,
    },

    [NATIONAL_DEX_MAGNETON] =
    {
        .categoryName = _("Magnet"),
        .height = 10,
        .weight = 600,
        .description = gMagnetonPokedexText,
    },

    [NATIONAL_DEX_FARFETCHD] =
    {
        .categoryName = _("Wild Duck"),
        .height = 8,
        .weight = 150,
        .description = gFarfetchdPokedexText,
    },

    [NATIONAL_DEX_DODUO] =
    {
        .categoryName = _("Twin Bird"),
        .height = 14,
        .weight = 392,
        .description = gDoduoPokedexText,
    },

    [NATIONAL_DEX_DODRIO] =
    {
        .categoryName = _("Triple Bird"),
        .height = 18,
        .weight = 852,
        .description = gDodrioPokedexText,
    },

    [NATIONAL_DEX_SEEL] =
    {
        .categoryName = _("Sea Lion"),
        .height = 11,
        .weight = 900,
        .description = gSeelPokedexText,
    },

    [NATIONAL_DEX_DEWGONG] =
    {
        .categoryName = _("Sea Lion"),
        .height = 17,
        .weight = 1200,
        .description = gDewgongPokedexText,
    },

    [NATIONAL_DEX_GRIMER] =
    {
        .categoryName = _("Sludge"),
        .height = 9,
        .weight = 300,
        .description = gGrimerPokedexText,
    },

    [NATIONAL_DEX_MUK] =
    {
        .categoryName = _("Sludge"),
        .height = 12,
        .weight = 300,
        .description = gMukPokedexText,
    },

    [NATIONAL_DEX_SHELLDER] =
    {
        .categoryName = _("Bivalve"),
        .height = 3,
        .weight = 40,
        .description = gShellderPokedexText,
    },

    [NATIONAL_DEX_CLOYSTER] =
    {
        .categoryName = _("Bivalve"),
        .height = 15,
        .weight = 1325,
        .description = gCloysterPokedexText,
    },

    [NATIONAL_DEX_GASTLY] =
    {
        .categoryName = _("Gas"),
        .height = 13,
        .weight = 1,
        .description = gGastlyPokedexText,
    },

    [NATIONAL_DEX_HAUNTER] =
    {
        .categoryName = _("Gas"),
        .height = 16,
        .weight = 1,
        .description = gHaunterPokedexText,
    },

    [NATIONAL_DEX_GENGAR] =
    {
        .categoryName = _("Shadow"),
        .height = 15,
        .weight = 405,
        .description = gGengarPokedexText,
    },

    [NATIONAL_DEX_ONIX] =
    {
        .categoryName = _("Rock Snake"),
        .height = 88,
        .weight = 2100,
        .description = gOnixPokedexText,
    },

    [NATIONAL_DEX_DROWZEE] =
    {
        .categoryName = _("Hypnosis"),
        .height = 10,
        .weight = 324,
        .description = gDrowzeePokedexText,
    },

    [NATIONAL_DEX_HYPNO] =
    {
        .categoryName = _("Hypnosis"),
        .height = 16,
        .weight = 756,
        .description = gHypnoPokedexText,
    },

    [NATIONAL_DEX_KRABBY] =
    {
        .categoryName = _("River Crab"),
        .height = 4,
        .weight = 65,
        .description = gKrabbyPokedexText,
    },

    [NATIONAL_DEX_KINGLER] =
    {
        .categoryName = _("Pincer"),
        .height = 13,
        .weight = 600,
        .description = gKinglerPokedexText,
    },

    [NATIONAL_DEX_VOLTORB] =
    {
        .categoryName = _("Ball"),
        .height = 5,
        .weight = 104,
        .description = gVoltorbPokedexText,
    },

    [NATIONAL_DEX_ELECTRODE] =
    {
        .categoryName = _("Ball"),
        .height = 12,
        .weight = 666,
        .description = gElectrodePokedexText,
    },

    [NATIONAL_DEX_EXEGGCUTE] =
    {
        .categoryName = _("Egg"),
        .height = 4,
        .weight = 25,
        .description = gExeggcutePokedexText,
    },

    [NATIONAL_DEX_EXEGGUTOR] =
    {
        .categoryName = _("Coconut"),
        .height = 20,
        .weight = 1200,
        .description = gExeggutorPokedexText,
    },

    [NATIONAL_DEX_CUBONE] =
    {
        .categoryName = _("Lonely"),
        .height = 4,
        .weight = 65,
        .description = gCubonePokedexText,
    },

    [NATIONAL_DEX_MAROWAK] =
    {
        .categoryName = _("Bone Keeper"),
        .height = 10,
        .weight = 450,
        .description = gMarowakPokedexText,
    },

    [NATIONAL_DEX_HITMONLEE] =
    {
        .categoryName = _("Kicking"),
        .height = 15,
        .weight = 498,
        .description = gHitmonleePokedexText,
    },

    [NATIONAL_DEX_HITMONCHAN] =
    {
        .categoryName = _("Punching"),
        .height = 14,
        .weight = 502,
        .description = gHitmonchanPokedexText,
    },

    [NATIONAL_DEX_LICKITUNG] =
    {
        .categoryName = _("Licking"),
        .height = 12,
        .weight = 655,
        .description = gLickitungPokedexText,
    },

    [NATIONAL_DEX_KOFFING] =
    {
        .categoryName = _("Poison Gas"),
        .height = 6,
        .weight = 10,
        .description = gKoffingPokedexText,
    },

    [NATIONAL_DEX_WEEZING] =
    {
        .categoryName = _("Poison Gas"),
        .height = 12,
        .weight = 95,
        .description = gWeezingPokedexText,
    },

    [NATIONAL_DEX_RHYHORN] =
    {
        .categoryName = _("Spikes"),
        .height = 10,
        .weight = 1150,
        .description = gRhyhornPokedexText,
    },

    [NATIONAL_DEX_RHYDON] =
    {
        .categoryName = _("Drill"),
        .height = 19,
        .weight = 1200,
        .description = gRhydonPokedexText,
    },

    [NATIONAL_DEX_CHANSEY] =
    {
        .categoryName = _("Egg"),
        .height = 11,
        .weight = 346,
        .description = gChanseyPokedexText,
    },

    [NATIONAL_DEX_TANGELA] =
    {
        .categoryName = _("Vine"),
        .height = 10,
        .weight = 350,
        .description = gTangelaPokedexText,
    },

    [NATIONAL_DEX_KANGASKHAN] =
    {
        .categoryName = _("Parent"),
        .height = 22,
        .weight = 800,
        .description = gKangaskhanPokedexText,
    },

    [NATIONAL_DEX_HORSEA] =
    {
        .categoryName = _("Dragon"),
        .height = 4,
        .weight = 80,
        .description = gHorseaPokedexText,
    },

    [NATIONAL_DEX_SEADRA] =
    {
        .categoryName = _("Dragon"),
        .height = 12,
        .weight = 250,
        .description = gSeadraPokedexText,
    },

    [NATIONAL_DEX_GOLDEEN] =
    {
        .categoryName = _("Goldfish"),
        .height = 6,
        .weight = 150,
        .description = gGoldeenPokedexText,
    },

    [NATIONAL_DEX_SEAKING] =
    {
        .categoryName = _("Goldfish"),
        .height = 13,
        .weight = 390,
        .description = gSeakingPokedexText,
    },

    [NATIONAL_DEX_STARYU] =
    {
        .categoryName = _("Star Shape"),
        .height = 8,
        .weight = 345,
        .description = gStaryuPokedexText,
    },

    [NATIONAL_DEX_STARMIE] =
    {
        .categoryName = _("Mysterious"),
        .height = 11,
        .weight = 800,
        .description = gStarmiePokedexText,
    },

    [NATIONAL_DEX_MR_MIME] =
    {
        .categoryName = _("Barrier"),
        .height = 13,
        .weight = 545,
        .description = gMrMimePokedexText,
    },

    [NATIONAL_DEX_SCYTHER] =
    {
        .categoryName = _("Mantis"),
        .height = 15,
        .weight = 560,
        .description = gScytherPokedexText,
    },

    [NATIONAL_DEX_JYNX] =
    {
        .categoryName = _("Human Shape"),
        .height = 14,
        .weight = 406,
        .description = gJynxPokedexText,
    },

    [NATIONAL_DEX_ELECTABUZZ] =
    {
        .categoryName = _("Electric"),
        .height = 11,
        .weight = 300,
        .description = gElectabuzzPokedexText,
    },

    [NATIONAL_DEX_MAGMAR] =
    {
        .categoryName = _("Spitfire"),
        .height = 13,
        .weight = 445,
        .description = gMagmarPokedexText,
    },

    [NATIONAL_DEX_PINSIR] =
    {
        .categoryName = _("Stag Beetle"),
        .height = 15,
        .weight = 550,
        .description = gPinsirPokedexText,
    },

    [NATIONAL_DEX_TAUROS] =
    {
        .categoryName = _("Wild Bull"),
        .height = 14,
        .weight = 884,
        .description = gTaurosPokedexText,
    },

    [NATIONAL_DEX_MAGIKARP] =
    {
        .categoryName = _("Fish"),
        .height = 9,
        .weight = 100,
        .description = gMagikarpPokedexText,
    },

    [NATIONAL_DEX_GYARADOS] =
    {
        .categoryName = _("Atrocious"),
        .height = 65,
        .weight = 2350,
        .description = gGyaradosPokedexText,
    },

    [NATIONAL_DEX_LAPRAS] =
    {
        .categoryName = _("Transport"),
        .height = 25,
        .weight = 2200,
        .description = gLaprasPokedexText,
    },

    [NATIONAL_DEX_DITTO] =
    {
        .categoryName = _("Transform"),
        .height = 3,
        .weight = 40,
        .description = gDittoPokedexText,
    },

    [NATIONAL_DEX_EEVEE] =
    {
        .categoryName = _("Evolution"),
        .height = 3,
        .weight = 65,
        .description = gEeveePokedexText,
    },

    [NATIONAL_DEX_VAPOREON] =
    {
        .categoryName = _("Bubble Jet"),
        .height = 10,
        .weight = 290,
        .description = gVaporeonPokedexText,
    },

    [NATIONAL_DEX_JOLTEON] =
    {
        .categoryName = _("Lightning"),
        .height = 8,
        .weight = 245,
        .description = gJolteonPokedexText,
    },

    [NATIONAL_DEX_FLAREON] =
    {
        .categoryName = _("Flame"),
        .height = 9,
        .weight = 250,
        .description = gFlareonPokedexText,
    },

    [NATIONAL_DEX_PORYGON] =
    {
        .categoryName = _("Virtual"),
        .height = 8,
        .weight = 365,
        .description = gPorygonPokedexText,
    },

    [NATIONAL_DEX_OMANYTE] =
    {
        .categoryName = _("Spiral"),
        .height = 4,
        .weight = 75,
        .description = gOmanytePokedexText,
    },

    [NATIONAL_DEX_OMASTAR] =
    {
        .categoryName = _("Spiral"),
        .height = 10,
        .weight = 350,
        .description = gOmastarPokedexText,
    },

    [NATIONAL_DEX_KABUTO] =
    {
        .categoryName = _("Shellfish"),
        .height = 5,
        .weight = 115,
        .description = gKabutoPokedexText,
    },

    [NATIONAL_DEX_KABUTOPS] =
    {
        .categoryName = _("Shellfish"),
        .height = 13,
        .weight = 405,
        .description = gKabutopsPokedexText,
    },

    [NATIONAL_DEX_AERODACTYL] =
    {
        .categoryName = _("Fossil"),
        .height = 18,
        .weight = 590,
        .description = gAerodactylPokedexText,
    },

    [NATIONAL_DEX_SNORLAX] =
    {
        .categoryName = _("Sleeping"),
        .height = 21,
        .weight = 4600,
        .description = gSnorlaxPokedexText,
    },

    [NATIONAL_DEX_ARTICUNO] =
    {
        .categoryName = _("Freeze"),
        .height = 17,
        .weight = 554,
        .description = gArticunoPokedexText,
    },

    [NATIONAL_DEX_ZAPDOS] =
    {
        .categoryName = _("Electric"),
        .height = 16,
        .weight = 526,
        .description = gZapdosPokedexText,
    },

    [NATIONAL_DEX_MOLTRES] =
    {
        .categoryName = _("Flame"),
        .height = 20,
        .weight = 600,
        .description = gMoltresPokedexText,
    },

    [NATIONAL_DEX_DRATINI] =
    {
        .categoryName = _("Dragon"),
        .height = 18,
        .weight = 33,
        .description = gDratiniPokedexText,
    },

    [NATIONAL_DEX_DRAGONAIR] =
    {
        .categoryName = _("Dragon"),
        .height = 40,
        .weight = 165,
        .description = gDragonairPokedexText,
    },

    [NATIONAL_DEX_DRAGONITE] =
    {
        .categoryName = _("Dragon"),
        .height = 22,
        .weight = 2100,
        .description = gDragonitePokedexText,
    },

    [NATIONAL_DEX_MEWTWO] =
    {
        .categoryName = _("Genetic"),
        .height = 20,
        .weight = 1220,
        .description = gMewtwoPokedexText,
    },

    [NATIONAL_DEX_MEW] =
    {
        .categoryName = _("New Species"),
        .height = 4,
        .weight = 40,
        .description = gMewPokedexText,
    },

    [NATIONAL_DEX_CHIKORITA] =
    {
        .categoryName = _("Leaf"),
        .height = 9,
        .weight = 64,
        .description = gChikoritaPokedexText,
    },

    [NATIONAL_DEX_BAYLEEF] =
    {
        .categoryName = _("Leaf"),
        .height = 12,
        .weight = 158,
        .description = gBayleefPokedexText,
    },

    [NATIONAL_DEX_MEGANIUM] =
    {
        .categoryName = _("Herb"),
        .height = 18,
        .weight = 1005,
        .description = gMeganiumPokedexText,
    },

    [NATIONAL_DEX_CYNDAQUIL] =
    {
        .categoryName = _("Fire Mouse"),
        .height = 5,
        .weight = 79,
        .description = gCyndaquilPokedexText,
    },

    [NATIONAL_DEX_QUILAVA] =
    {
        .categoryName = _("Volcano"),
        .height = 9,
        .weight = 190,
        .description = gQuilavaPokedexText,
    },

    [NATIONAL_DEX_TYPHLOSION] =
    {
        .categoryName = _("Volcano"),
        .height = 17,
        .weight = 795,
        .description = gTyphlosionPokedexText,
    },

    [NATIONAL_DEX_TOTODILE] =
    {
        .categoryName = _("Big Jaw"),
        .height = 6,
        .weight = 95,
        .description = gTotodilePokedexText,
    },

    [NATIONAL_DEX_CROCONAW] =
    {
        .categoryName = _("Big Jaw"),
        .height = 11,
        .weight = 250,
        .description = gCroconawPokedexText,
    },

    [NATIONAL_DEX_FERALIGATR] =
    {
        .categoryName = _("Big Jaw"),
        .height = 23,
        .weight = 888,
        .description = gFeraligatrPokedexText,
    },

    [NATIONAL_DEX_SENTRET] =
    {
        .categoryName = _("Scout"),
        .height = 8,
        .weight = 60,
        .description = gSentretPokedexText,
    },

    [NATIONAL_DEX_FURRET] =
    {
        .categoryName = _("Long Body"),
        .height = 18,
        .weight = 325,
        .description = gFurretPokedexText,
    },

    [NATIONAL_DEX_HOOTHOOT] =
    {
        .categoryName = _("Owl"),
        .height = 7,
        .weight = 212,
        .description = gHoothootPokedexText,
    },

    [NATIONAL_DEX_NOCTOWL] =
    {
        .categoryName = _("Owl"),
        .height = 16,
        .weight = 408,
        .description = gNoctowlPokedexText,
    },

    [NATIONAL_DEX_LEDYBA] =
    {
        .categoryName = _("Five Star"),
        .height = 10,
        .weight = 108,
        .description = gLedybaPokedexText,
    },

    [NATIONAL_DEX_LEDIAN] =
    {
        .categoryName = _("Five Star"),
        .height = 14,
        .weight = 356,
        .description = gLedianPokedexText,
    },

    [NATIONAL_DEX_SPINARAK] =
    {
        .categoryName = _("String Spit"),
        .height = 5,
        .weight = 85,
        .description = gSpinarakPokedexText,
    },

    [NATIONAL_DEX_ARIADOS] =
    {
        .categoryName = _("Long Leg"),
        .height = 11,
        .weight = 335,
        .description = gAriadosPokedexText,
    },

    [NATIONAL_DEX_CROBAT] =
    {
        .categoryName = _("Bat"),
        .height = 18,
        .weight = 750,
        .description = gCrobatPokedexText,
    },

    [NATIONAL_DEX_CHINCHOU] =
    {
        .categoryName = _("Angler"),
        .height = 5,
        .weight = 120,
        .description = gChinchouPokedexText,
    },

    [NATIONAL_DEX_LANTURN] =
    {
        .categoryName = _("Light"),
        .height = 12,
        .weight = 225,
        .description = gLanturnPokedexText,
    },

    [NATIONAL_DEX_PICHU] =
    {
        .categoryName = _("Tiny Mouse"),
        .height = 3,
        .weight = 20,
        .description = gPichuPokedexText,
    },

    [NATIONAL_DEX_CLEFFA] =
    {
        .categoryName = _("Star Shape"),
        .height = 3,
        .weight = 30,
        .description = gCleffaPokedexText,
    },

    [NATIONAL_DEX_IGGLYBUFF] =
    {
        .categoryName = _("Balloon"),
        .height = 3,
        .weight = 10,
        .description = gIgglybuffPokedexText,
    },

    [NATIONAL_DEX_TOGEPI] =
    {
        .categoryName = _("Spike Ball"),
        .height = 3,
        .weight = 15,
        .description = gTogepiPokedexText,
    },

    [NATIONAL_DEX_TOGETIC] =
    {
        .categoryName = _("Happiness"),
        .height = 6,
        .weight = 32,
        .description = gTogeticPokedexText,
    },

    [NATIONAL_DEX_NATU] =
    {
        .categoryName = _("Tiny Bird"),
        .height = 2,
        .weight = 20,
        .description = gNatuPokedexText,
    },

    [NATIONAL_DEX_XATU] =
    {
        .categoryName = _("Mystic"),
        .height = 15,
        .weight = 150,
        .description = gXatuPokedexText,
    },

    [NATIONAL_DEX_MAREEP] =
    {
        .categoryName = _("Wool"),
        .height = 6,
        .weight = 78,
        .description = gMareepPokedexText,
    },

    [NATIONAL_DEX_FLAAFFY] =
    {
        .categoryName = _("Wool"),
        .height = 8,
        .weight = 133,
        .description = gFlaaffyPokedexText,
    },

    [NATIONAL_DEX_AMPHAROS] =
    {
        .categoryName = _("Light"),
        .height = 14,
        .weight = 615,
        .description = gAmpharosPokedexText,
    },

    [NATIONAL_DEX_BELLOSSOM] =
    {
        .categoryName = _("Flower"),
        .height = 4,
        .weight = 58,
        .description = gBellossomPokedexText,
    },

    [NATIONAL_DEX_MARILL] =
    {
        .categoryName = _("Aqua Mouse"),
        .height = 4,
        .weight = 85,
        .description = gMarillPokedexText,
    },

    [NATIONAL_DEX_AZUMARILL] =
    {
        .categoryName = _("Aqua Rabbit"),
        .height = 8,
        .weight = 285,
        .description = gAzumarillPokedexText,
    },

    [NATIONAL_DEX_SUDOWOODO] =
    {
        .categoryName = _("Imitation"),
        .height = 12,
        .weight = 380,
        .description = gSudowoodoPokedexText,
    },

    [NATIONAL_DEX_POLITOED] =
    {
        .categoryName = _("Frog"),
        .height = 11,
        .weight = 339,
        .description = gPolitoedPokedexText,
    },

    [NATIONAL_DEX_HOPPIP] =
    {
        .categoryName = _("Cottonweed"),
        .height = 4,
        .weight = 5,
        .description = gHoppipPokedexText,
    },

    [NATIONAL_DEX_SKIPLOOM] =
    {
        .categoryName = _("Cottonweed"),
        .height = 6,
        .weight = 10,
        .description = gSkiploomPokedexText,
    },

    [NATIONAL_DEX_JUMPLUFF] =
    {
        .categoryName = _("Cottonweed"),
        .height = 8,
        .weight = 30,
        .description = gJumpluffPokedexText,
    },

    [NATIONAL_DEX_AIPOM] =
    {
        .categoryName = _("Long Tail"),
        .height = 8,
        .weight = 115,
        .description = gAipomPokedexText,
    },

    [NATIONAL_DEX_SUNKERN] =
    {
        .categoryName = _("Seed"),
        .height = 3,
        .weight = 18,
        .description = gSunkernPokedexText,
    },

    [NATIONAL_DEX_SUNFLORA] =
    {
        .categoryName = _("Sun"),
        .height = 8,
        .weight = 85,
        .description = gSunfloraPokedexText,
    },

    [NATIONAL_DEX_YANMA] =
    {
        .categoryName = _("Clear Wing"),
        .height = 12,
        .weight = 380,
        .description = gYanmaPokedexText,
    },

    [NATIONAL_DEX_WOOPER] =
    {
        .categoryName = _("Water Fish"),
        .height = 4,
        .weight = 85,
        .description = gWooperPokedexText,
    },

    [NATIONAL_DEX_QUAGSIRE] =
    {
        .categoryName = _("Water Fish"),
        .height = 14,
        .weight = 750,
        .description = gQuagsirePokedexText,
    },

    [NATIONAL_DEX_ESPEON] =
    {
        .categoryName = _("Sun"),
        .height = 9,
        .weight = 265,
        .description = gEspeonPokedexText,
    },

    [NATIONAL_DEX_UMBREON] =
    {
        .categoryName = _("Moonlight"),
        .height = 10,
        .weight = 270,
        .description = gUmbreonPokedexText,
    },

    [NATIONAL_DEX_MURKROW] =
    {
        .categoryName = _("Darkness"),
        .height = 5,
        .weight = 21,
        .description = gMurkrowPokedexText,
    },

    [NATIONAL_DEX_SLOWKING] =
    {
        .categoryName = _("Royal"),
        .height = 20,
        .weight = 795,
        .description = gSlowkingPokedexText,
    },

    [NATIONAL_DEX_MISDREAVUS] =
    {
        .categoryName = _("Screech"),
        .height = 7,
        .weight = 10,
        .description = gMisdreavusPokedexText,
    },

    [NATIONAL_DEX_UNOWN] =
    {
        .categoryName = _("Symbol"),
        .height = 5,
        .weight = 50,
        .description = gUnownPokedexText,
    },

    [NATIONAL_DEX_WOBBUFFET] =
    {
        .categoryName = _("Patient"),
        .height = 13,
        .weight = 285,
        .description = gWobbuffetPokedexText,
    },

    [NATIONAL_DEX_GIRAFARIG] =
    {
        .categoryName = _("Long Neck"),
        .height = 15,
        .weight = 415,
        .description = gGirafarigPokedexText,
    },

    [NATIONAL_DEX_PINECO] =
    {
        .categoryName = _("Bagworm"),
        .height = 6,
        .weight = 72,
        .description = gPinecoPokedexText,
    },

    [NATIONAL_DEX_FORRETRESS] =
    {
        .categoryName = _("Bagworm"),
        .height = 12,
        .weight = 1258,
        .description = gForretressPokedexText,
    },

    [NATIONAL_DEX_DUNSPARCE] =
    {
        .categoryName = _("Land Snake"),
        .height = 15,
        .weight = 140,
        .description = gDunsparcePokedexText,
    },

    [NATIONAL_DEX_GLIGAR] =
    {
        .categoryName = _("Fly Scorpion"),
        .height = 11,
        .weight = 648,
        .description = gGligarPokedexText,
    },

    [NATIONAL_DEX_STEELIX] =
    {
        .categoryName = _("Iron Snake"),
        .height = 92,
        .weight = 4000,
        .description = gSteelixPokedexText,
    },

    [NATIONAL_DEX_SNUBBULL] =
    {
        .categoryName = _("Fairy"),
        .height = 6,
        .weight = 78,
        .description = gSnubbullPokedexText,
    },

    [NATIONAL_DEX_GRANBULL] =
    {
        .categoryName = _("Fairy"),
        .height = 14,
        .weight = 487,
        .description = gGranbullPokedexText,
    },

    [NATIONAL_DEX_QWILFISH] =
    {
        .categoryName = _("Balloon"),
        .height = 5,
        .weight = 39,
        .description = gQwilfishPokedexText,
    },

    [NATIONAL_DEX_SCIZOR] =
    {
        .categoryName = _("Pincer"),
        .height = 18,
        .weight = 1180,
        .description = gScizorPokedexText,
    },

    [NATIONAL_DEX_SHUCKLE] =
    {
        .categoryName = _("Mold"),
        .height = 6,
        .weight = 205,
        .description = gShucklePokedexText,
    },

    [NATIONAL_DEX_HERACROSS] =
    {
        .categoryName = _("Single Horn"),
        .height = 15,
        .weight = 540,
        .description = gHeracrossPokedexText,
    },

    [NATIONAL_DEX_SNEASEL] =
    {
        .categoryName = _("Sharp Claw"),
        .height = 9,
        .weight = 280,
        .description = gSneaselPokedexText,
    },

    [NATIONAL_DEX_TEDDIURSA] =
    {
        .categoryName = _("Little Bear"),
        .height = 6,
        .weight = 88,
        .description = gTeddiursaPokedexText,
    },

    [NATIONAL_DEX_URSARING] =
    {
        .categoryName = _("Hibernator"),
        .height = 18,
        .weight = 1258,
        .description = gUrsaringPokedexText,
    },

    [NATIONAL_DEX_SLUGMA] =
    {
        .categoryName = _("Lava"),
        .height = 7,
        .weight = 350,
        .description = gSlugmaPokedexText,
    },

    [NATIONAL_DEX_MAGCARGO] =
    {
        .categoryName = _("Lava"),
        .height = 8,
        .weight = 550,
        .description = gMagcargoPokedexText,
    },

    [NATIONAL_DEX_SWINUB] =
    {
        .categoryName = _("Pig"),
        .height = 4,
        .weight = 65,
        .description = gSwinubPokedexText,
    },

    [NATIONAL_DEX_PILOSWINE] =
    {
        .categoryName = _("Swine"),
        .height = 11,
        .weight = 558,
        .description = gPiloswinePokedexText,
    },

    [NATIONAL_DEX_CORSOLA] =
    {
        .categoryName = _("Coral"),
        .height = 6,
        .weight = 50,
        .description = gCorsolaPokedexText,
    },

    [NATIONAL_DEX_REMORAID] =
    {
        .categoryName = _("Jet"),
        .height = 6,
        .weight = 120,
        .description = gRemoraidPokedexText,
    },

    [NATIONAL_DEX_OCTILLERY] =
    {
        .categoryName = _("Jet"),
        .height = 9,
        .weight = 285,
        .description = gOctilleryPokedexText,
    },

    [NATIONAL_DEX_DELIBIRD] =
    {
        .categoryName = _("Delivery"),
        .height = 9,
        .weight = 160,
        .description = gDelibirdPokedexText,
    },

    [NATIONAL_DEX_MANTINE] =
    {
        .categoryName = _("Kite"),
        .height = 21,
        .weight = 2200,
        .description = gMantinePokedexText,
    },

    [NATIONAL_DEX_SKARMORY] =
    {
        .categoryName = _("Armor Bird"),
        .height = 17,
        .weight = 505,
        .description = gSkarmoryPokedexText,
    },

    [NATIONAL_DEX_HOUNDOUR] =
    {
        .categoryName = _("Dark"),
        .height = 6,
        .weight = 108,
        .description = gHoundourPokedexText,
    },

    [NATIONAL_DEX_HOUNDOOM] =
    {
        .categoryName = _("Dark"),
        .height = 14,
        .weight = 350,
        .description = gHoundoomPokedexText,
    },

    [NATIONAL_DEX_KINGDRA] =
    {
        .categoryName = _("Dragon"),
        .height = 18,
        .weight = 1520,
        .description = gKingdraPokedexText,
    },

    [NATIONAL_DEX_PHANPY] =
    {
        .categoryName = _("Long Nose"),
        .height = 5,
        .weight = 335,
        .description = gPhanpyPokedexText,
    },

    [NATIONAL_DEX_DONPHAN] =
    {
        .categoryName = _("Armor"),
        .height = 11,
        .weight = 1200,
        .description = gDonphanPokedexText,
    },

    [NATIONAL_DEX_PORYGON2] =
    {
        .categoryName = _("Virtual"),
        .height = 6,
        .weight = 325,
        .description = gPorygon2PokedexText,
    },

    [NATIONAL_DEX_STANTLER] =
    {
        .categoryName = _("Big Horn"),
        .height = 14,
        .weight = 712,
        .description = gStantlerPokedexText,
    },

    [NATIONAL_DEX_SMEARGLE] =
    {
        .categoryName = _("Painter"),
        .height = 12,
        .weight = 580,
        .description = gSmearglePokedexText,
    },

    [NATIONAL_DEX_TYROGUE] =
    {
        .categoryName = _("Scuffle"),
        .height = 7,
        .weight = 210,
        .description = gTyroguePokedexText,
    },

    [NATIONAL_DEX_HITMONTOP] =
    {
        .categoryName = _("Handstand"),
        .height = 14,
        .weight = 480,
        .description = gHitmontopPokedexText,
    },

    [NATIONAL_DEX_SMOOCHUM] =
    {
        .categoryName = _("Kiss"),
        .height = 4,
        .weight = 60,
        .description = gSmoochumPokedexText,
    },

    [NATIONAL_DEX_ELEKID] =
    {
        .categoryName = _("Electric"),
        .height = 6,
        .weight = 235,
        .description = gElekidPokedexText,
    },

    [NATIONAL_DEX_MAGBY] =
    {
        .categoryName = _("Live Coal"),
        .height = 7,
        .weight = 214,
        .description = gMagbyPokedexText,
    },

    [NATIONAL_DEX_MILTANK] =
    {
        .categoryName = _("Milk Cow"),
        .height = 12,
        .weight = 755,
        .description = gMiltankPokedexText,
    },

    [NATIONAL_DEX_BLISSEY] =
    {
        .categoryName = _("Happiness"),
        .height = 15,
        .weight = 468,
        .description = gBlisseyPokedexText,
    },

    [NATIONAL_DEX_RAIKOU] =
    {
        .categoryName = _("Thunder"),
        .height = 19,
        .weight = 1780,
        .description = gRaikouPokedexText,
    },

    [NATIONAL_DEX_ENTEI] =
    {
        .categoryName = _("Volcano"),
        .height = 21,
        .weight = 1980,
        .description = gEnteiPokedexText,
    },

    [NATIONAL_DEX_SUICUNE] =
    {
        .categoryName = _("Aurora"),
        .height = 20,
        .weight = 1870,
        .description = gSuicunePokedexText,
    },

    [NATIONAL_DEX_LARVITAR] =
    {
        .categoryName = _("Rock Skin"),
        .height = 6,
        .weight = 720,
        .description = gLarvitarPokedexText,
    },

    [NATIONAL_DEX_PUPITAR] =
    {
        .categoryName = _("Hard Shell"),
        .height = 12,
        .weight = 1520,
        .description = gPupitarPokedexText,
    },

    [NATIONAL_DEX_TYRANITAR] =
    {
        .categoryName = _("Armor"),
        .height = 20,
        .weight = 2020,
        .description = gTyranitarPokedexText,
    },

    [NATIONAL_DEX_LUGIA] =
    {
        .categoryName = _("Diving"),
        .height = 52,
        .weight = 2160,
        .description = gLugiaPokedexText,
    },

    [NATIONAL_DEX_HO_OH] =
    {
        .categoryName = _("Rainbow"),
        .height = 38,
        .weight = 1990,
        .description = gHoOhPokedexText,
    },

    [NATIONAL_DEX_CELEBI] =
    {
        .categoryName = _("Time Travel"),
        .height = 6,
        .weight = 50,
        .description = gCelebiPokedexText,
    },

    [NATIONAL_DEX_TREECKO] =
    {
        .categoryName = _("Wood Gecko"),
        .height = 5,
        .weight = 50,
        .description = gTreeckoPokedexText,
    },

    [NATIONAL_DEX_GROVYLE] =
    {
        .categoryName = _("Wood Gecko"),
        .height = 9,
        .weight = 216,
        .description = gGrovylePokedexText,
    },

    [NATIONAL_DEX_SCEPTILE] =
    {
        .categoryName = _("Forest"),
        .height = 17,
        .weight = 522,
        .description = gSceptilePokedexText,
    },

    [NATIONAL_DEX_TORCHIC] =
    {
        .categoryName = _("Chick"),
        .height = 4,
        .weight = 25,
        .description = gTorchicPokedexText,
    },

    [NATIONAL_DEX_COMBUSKEN] =
    {
        .categoryName = _("Young Fowl"),
        .height = 9,
        .weight = 195,
        .description = gCombuskenPokedexText,
    },

    [NATIONAL_DEX_BLAZIKEN] =
    {
        .categoryName = _("Blaze"),
        .height = 19,
        .weight = 520,
        .description = gBlazikenPokedexText,
    },

    [NATIONAL_DEX_MUDKIP] =
    {
        .categoryName = _("Mud Fish"),
        .height = 4,
        .weight = 76,
        .description = gMudkipPokedexText,
    },

    [NATIONAL_DEX_MARSHTOMP] =
    {
        .categoryName = _("Mud Fish"),
        .height = 7,
        .weight = 280,
        .description = gMarshtompPokedexText,
    },

    [NATIONAL_DEX_SWAMPERT] =
    {
        .categoryName = _("Mud Fish"),
        .height = 15,
        .weight = 819,
        .description = gSwampertPokedexText,
    },

    [NATIONAL_DEX_POOCHYENA] =
    {
        .categoryName = _("Bite"),
        .height = 5,
        .weight = 136,
        .description = gPoochyenaPokedexText,
    },

    [NATIONAL_DEX_MIGHTYENA] =
    {
        .categoryName = _("Bite"),
        .height = 10,
        .weight = 370,
        .description = gMightyenaPokedexText,
    },

    [NATIONAL_DEX_ZIGZAGOON] =
    {
        .categoryName = _("Tiny Raccoon"),
        .height = 4,
        .weight = 175,
        .description = gZigzagoonPokedexText,
    },

    [NATIONAL_DEX_LINOONE] =
    {
        .categoryName = _("Rushing"),
        .height = 5,
        .weight = 325,
        .description = gLinoonePokedexText,
    },

    [NATIONAL_DEX_WURMPLE] =
    {
        .categoryName = _("Worm"),
        .height = 3,
        .weight = 36,
        .description = gWurmplePokedexText,
    },

    [NATIONAL_DEX_SILCOON] =
    {
        .categoryName = _("Cocoon"),
        .height = 6,
        .weight = 100,
        .description = gSilcoonPokedexText,
    },

    [NATIONAL_DEX_BEAUTIFLY] =
    {
        .categoryName = _("Butterfly"),
        .height = 10,
        .weight = 284,
        .description = gBeautiflyPokedexText,
    },

    [NATIONAL_DEX_CASCOON] =
    {
        .categoryName = _("Cocoon"),
        .height = 7,
        .weight = 115,
        .description = gCascoonPokedexText,
    },

    [NATIONAL_DEX_DUSTOX] =
    {
        .categoryName = _("Poison Moth"),
        .height = 12,
        .weight = 316,
        .description = gDustoxPokedexText,
    },

    [NATIONAL_DEX_LOTAD] =
    {
        .categoryName = _("Water Weed"),
        .height = 5,
        .weight = 26,
        .description = gLotadPokedexText,
    },

    [NATIONAL_DEX_LOMBRE] =
    {
        .categoryName = _("Jolly"),
        .height = 12,
        .weight = 325,
        .description = gLombrePokedexText,
    },

    [NATIONAL_DEX_LUDICOLO] =
    {
        .categoryName = _("Carefree"),
        .height = 15,
        .weight = 550,
        .description = gLudicoloPokedexText,
    },

    [NATIONAL_DEX_SEEDOT] =
    {
        .categoryName = _("Acorn"),
        .height = 5,
        .weight = 40,
        .description = gSeedotPokedexText,
    },

    [NATIONAL_DEX_NUZLEAF] =
    {
        .categoryName = _("Wily"),
        .height = 10,
        .weight = 280,
        .description = gNuzleafPokedexText,
    },

    [NATIONAL_DEX_SHIFTRY] =
    {
        .categoryName = _("Wicked"),
        .height = 13,
        .weight = 596,
        .description = gShiftryPokedexText,
    },

    [NATIONAL_DEX_TAILLOW] =
    {
        .categoryName = _("Tiny Swallow"),
        .height = 3,
        .weight = 23,
        .description = gTaillowPokedexText,
    },

    [NATIONAL_DEX_SWELLOW] =
    {
        .categoryName = _("Swallow"),
        .height = 7,
        .weight = 198,
        .description = gSwellowPokedexText,
    },

    [NATIONAL_DEX_WINGULL] =
    {
        .categoryName = _("Seagull"),
        .height = 6,
        .weight = 95,
        .description = gWingullPokedexText,
    },

    [NATIONAL_DEX_PELIPPER] =
    {
        .categoryName = _("Water Bird"),
        .height = 12,
        .weight = 280,
        .description = gPelipperPokedexText,
    },

    [NATIONAL_DEX_RALTS] =
    {
        .categoryName = _("Feeling"),
        .height = 4,
        .weight = 66,
        .description = gRaltsPokedexText,
    },

    [NATIONAL_DEX_KIRLIA] =
    {
        .categoryName = _("Emotion"),
        .height = 8,
        .weight = 202,
        .description = gKirliaPokedexText,
    },

    [NATIONAL_DEX_GARDEVOIR] =
    {
        .categoryName = _("Embrace"),
        .height = 16,
        .weight = 484,
        .description = gGardevoirPokedexText,
    },

    [NATIONAL_DEX_SURSKIT] =
    {
        .categoryName = _("Pond Skater"),
        .height = 5,
        .weight = 17,
        .description = gSurskitPokedexText,
    },

    [NATIONAL_DEX_MASQUERAIN] =
    {
        .categoryName = _("Eyeball"),
        .height = 8,
        .weight = 36,
        .description = gMasquerainPokedexText,
    },

    [NATIONAL_DEX_SHROOMISH] =
    {
        .categoryName = _("Mushroom"),
        .height = 4,
        .weight = 45,
        .description = gShroomishPokedexText,
    },

    [NATIONAL_DEX_BRELOOM] =
    {
        .categoryName = _("Mushroom"),
        .height = 12,
        .weight = 392,
        .description = gBreloomPokedexText,
    },

    [NATIONAL_DEX_SLAKOTH] =
    {
        .categoryName = _("Slacker"),
        .height = 8,
        .weight = 240,
        .description = gSlakothPokedexText,
    },

    [NATIONAL_DEX_VIGOROTH] =
    {
        .categoryName = _("Wild Monkey"),
        .height = 14,
        .weight = 465,
        .description = gVigorothPokedexText,
    },

    [NATIONAL_DEX_SLAKING] =
    {
        .categoryName = _("Lazy"),
        .height = 20,
        .weight = 1305,
        .description = gSlakingPokedexText,
    },

    [NATIONAL_DEX_NINCADA] =
    {
        .categoryName = _("Trainee"),
        .height = 5,
        .weight = 55,
        .description = gNincadaPokedexText,
    },

    [NATIONAL_DEX_NINJASK] =
    {
        .categoryName = _("Ninja"),
        .height = 8,
        .weight = 120,
        .description = gNinjaskPokedexText,
    },

    [NATIONAL_DEX_SHEDINJA] =
    {
        .categoryName = _("Shed"),
        .height = 8,
        .weight = 12,
        .description = gShedinjaPokedexText,
    },

    [NATIONAL_DEX_WHISMUR] =
    {
        .categoryName = _("Whisper"),
        .height = 6,
        .weight = 163,
        .description = gWhismurPokedexText,
    },

    [NATIONAL_DEX_LOUDRED] =
    {
        .categoryName = _("Big Voice"),
        .height = 10,
        .weight = 405,
        .description = gLoudredPokedexText,
    },

    [NATIONAL_DEX_EXPLOUD] =
    {
        .categoryName = _("Loud Noise"),
        .height = 15,
        .weight = 840,
        .description = gExploudPokedexText,
    },

    [NATIONAL_DEX_MAKUHITA] =
    {
        .categoryName = _("Guts"),
        .height = 10,
        .weight = 864,
        .description = gMakuhitaPokedexText,
    },

    [NATIONAL_DEX_HARIYAMA] =
    {
        .categoryName = _("Arm Thrust"),
        .height = 23,
        .weight = 2538,
        .description = gHariyamaPokedexText,
    },

    [NATIONAL_DEX_AZURILL] =
    {
        .categoryName = _("Polka Dot"),
        .height = 2,
        .weight = 20,
        .description = gAzurillPokedexText,
    },

    [NATIONAL_DEX_NOSEPASS] =
    {
        .categoryName = _("Compass"),
        .height = 10,
        .weight = 970,
        .description = gNosepassPokedexText,
    },

    [NATIONAL_DEX_SKITTY] =
    {
        .categoryName = _("Kitten"),
        .height = 6,
        .weight = 110,
        .description = gSkittyPokedexText,
    },

    [NATIONAL_DEX_DELCATTY] =
    {
        .categoryName = _("Prim"),
        .height = 11,
        .weight = 326,
        .description = gDelcattyPokedexText,
    },

    [NATIONAL_DEX_SABLEYE] =
    {
        .categoryName = _("Darkness"),
        .height = 5,
        .weight = 110,
        .description = gSableyePokedexText,
    },

    [NATIONAL_DEX_MAWILE] =
    {
        .categoryName = _("Deceiver"),
        .height = 6,
        .weight = 115,
        .description = gMawilePokedexText,
    },

    [NATIONAL_DEX_ARON] =
    {
        .categoryName = _("Iron Armor"),
        .height = 4,
        .weight = 600,
        .description = gAronPokedexText,
    },

    [NATIONAL_DEX_LAIRON] =
    {
        .categoryName = _("Iron Armor"),
        .height = 9,
        .weight = 1200,
        .description = gLaironPokedexText,
    },

    [NATIONAL_DEX_AGGRON] =
    {
        .categoryName = _("Iron Armor"),
        .height = 21,
        .weight = 3600,
        .description = gAggronPokedexText,
    },

    [NATIONAL_DEX_MEDITITE] =
    {
        .categoryName = _("Meditate"),
        .height = 6,
        .weight = 112,
        .description = gMedititePokedexText,
    },

    [NATIONAL_DEX_MEDICHAM] =
    {
        .categoryName = _("Meditate"),
        .height = 13,
        .weight = 315,
        .description = gMedichamPokedexText,
    },

    [NATIONAL_DEX_ELECTRIKE] =
    {
        .categoryName = _("Lightning"),
        .height = 6,
        .weight = 152,
        .description = gElectrikePokedexText,
    },

    [NATIONAL_DEX_MANECTRIC] =
    {
        .categoryName = _("Discharge"),
        .height = 15,
        .weight = 402,
        .description = gManectricPokedexText,
    },

    [NATIONAL_DEX_PLUSLE] =
    {
        .categoryName = _("Cheering"),
        .height = 4,
        .weight = 42,
        .description = gPluslePokedexText,
    },

    [NATIONAL_DEX_MINUN] =
    {
        .categoryName = _("Cheering"),
        .height = 4,
        .weight = 42,
        .description = gMinunPokedexText,
    },

    [NATIONAL_DEX_VOLBEAT] =
    {
        .categoryName = _("Firefly"),
        .height = 7,
        .weight = 177,
        .description = gVolbeatPokedexText,
    },

    [NATIONAL_DEX_ILLUMISE] =
    {
        .categoryName = _("Firefly"),
        .height = 6,
        .weight = 177,
        .description = gIllumisePokedexText,
    },

    [NATIONAL_DEX_ROSELIA] =
    {
        .categoryName = _("Thorn"),
        .height = 3,
        .weight = 20,
        .description = gRoseliaPokedexText,
    },

    [NATIONAL_DEX_GULPIN] =
    {
        .categoryName = _("Stomach"),
        .height = 4,
        .weight = 103,
        .description = gGulpinPokedexText,
    },

    [NATIONAL_DEX_SWALOT] =
    {
        .categoryName = _("Poison Bag"),
        .height = 17,
        .weight = 800,
        .description = gSwalotPokedexText,
    },

    [NATIONAL_DEX_CARVANHA] =
    {
        .categoryName = _("Savage"),
        .height = 8,
        .weight = 208,
        .description = gCarvanhaPokedexText,
    },

    [NATIONAL_DEX_SHARPEDO] =
    {
        .categoryName = _("Brutal"),
        .height = 18,
        .weight = 888,
        .description = gSharpedoPokedexText,
    },

    [NATIONAL_DEX_WAILMER] =
    {
        .categoryName = _("Ball Whale"),
        .height = 20,
        .weight = 1300,
        .description = gWailmerPokedexText,
    },

    [NATIONAL_DEX_WAILORD] =
    {
        .categoryName = _("Float Whale"),
        .height = 145,
        .weight = 3980,
        .description = gWailordPokedexText,
    },

    [NATIONAL_DEX_NUMEL] =
    {
        .categoryName = _("Numb"),
        .height = 7,
        .weight = 240,
        .description = gNumelPokedexText,
    },

    [NATIONAL_DEX_CAMERUPT] =
    {
        .categoryName = _("Eruption"),
        .height = 19,
        .weight = 2200,
        .description = gCameruptPokedexText,
    },

    [NATIONAL_DEX_TORKOAL] =
    {
        .categoryName = _("Coal"),
        .height = 5,
        .weight = 804,
        .description = gTorkoalPokedexText,
    },

    [NATIONAL_DEX_SPOINK] =
    {
        .categoryName = _("Bounce"),
        .height = 7,
        .weight = 306,
        .description = gSpoinkPokedexText,
    },

    [NATIONAL_DEX_GRUMPIG] =
    {
        .categoryName = _("Manipulate"),
        .height = 9,
        .weight = 715,
        .description = gGrumpigPokedexText,
    },

    [NATIONAL_DEX_SPINDA] =
    {
        .categoryName = _("Spot Panda"),
        .height = 11,
        .weight = 50,
        .description = gSpindaPokedexText,
    },

    [NATIONAL_DEX_TRAPINCH] =
    {
        .categoryName = _("Ant Pit"),
        .height = 7,
        .weight = 150,
        .description = gTrapinchPokedexText,
    },

    [NATIONAL_DEX_VIBRAVA] =
    {
        .categoryName = _("Vibration"),
        .height = 11,
        .weight = 153,
        .description = gVibravaPokedexText,
    },

    [NATIONAL_DEX_FLYGON] =
    {
        .categoryName = _("Mystic"),
        .height = 20,
        .weight = 820,
        .description = gFlygonPokedexText,
    },

    [NATIONAL_DEX_CACNEA] =
    {
        .categoryName = _("Cactus"),
        .height = 4,
        .weight = 513,
        .description = gCacneaPokedexText,
    },

    [NATIONAL_DEX_CACTURNE] =
    {
        .categoryName = _("Scarecrow"),
        .height = 13,
        .weight = 774,
        .description = gCacturnePokedexText,
    },

    [NATIONAL_DEX_SWABLU] =
    {
        .categoryName = _("Cotton Bird"),
        .height = 4,
        .weight = 12,
        .description = gSwabluPokedexText,
    },

    [NATIONAL_DEX_ALTARIA] =
    {
        .categoryName = _("Humming"),
        .height = 11,
        .weight = 206,
        .description = gAltariaPokedexText,
    },

    [NATIONAL_DEX_ZANGOOSE] =
    {
        .categoryName = _("Cat Ferret"),
        .height = 13,
        .weight = 403,
        .description = gZangoosePokedexText,
    },

    [NATIONAL_DEX_SEVIPER] =
    {
        .categoryName = _("Fang Snake"),
        .height = 27,
        .weight = 525,
        .description = gSeviperPokedexText,
    },

    [NATIONAL_DEX_LUNATONE] =
    {
        .categoryName = _("Meteorite"),
        .height = 10,
        .weight = 1680,
        .description = gLunatonePokedexText,
    },

    [NATIONAL_DEX_SOLROCK] =
    {
        .categoryName = _("Meteorite"),
        .height = 12,
        .weight = 1540,
        .description = gSolrockPokedexText,
    },

    [NATIONAL_DEX_BARBOACH] =
    {
        .categoryName = _("Whiskers"),
        .height = 4,
        .weight = 19,
        .description = gBarboachPokedexText,
    },

    [NATIONAL_DEX_WHISCASH] =
    {
        .categoryName = _("Whiskers"),
        .height = 9,
        .weight = 236,
        .description = gWhiscashPokedexText,
    },

    [NATIONAL_DEX_CORPHISH] =
    {
        .categoryName = _("Ruffian"),
        .height = 6,
        .weight = 115,
        .description = gCorphishPokedexText,
    },

    [NATIONAL_DEX_CRAWDAUNT] =
    {
        .categoryName = _("Rogue"),
        .height = 11,
        .weight = 328,
        .description = gCrawdauntPokedexText,
    },

    [NATIONAL_DEX_BALTOY] =
    {
        .categoryName = _("Clay Doll"),
        .height = 5,
        .weight = 215,
        .description = gBaltoyPokedexText,
    },

    [NATIONAL_DEX_CLAYDOL] =
    {
        .categoryName = _("Clay Doll"),
        .height = 15,
        .weight = 1080,
        .description = gClaydolPokedexText,
    },

    [NATIONAL_DEX_LILEEP] =
    {
        .categoryName = _("Sea Lily"),
        .height = 10,
        .weight = 238,
        .description = gLileepPokedexText,
    },

    [NATIONAL_DEX_CRADILY] =
    {
        .categoryName = _("Barnacle"),
        .height = 15,
        .weight = 604,
        .description = gCradilyPokedexText,
    },

    [NATIONAL_DEX_ANORITH] =
    {
        .categoryName = _("Old Shrimp"),
        .height = 7,
        .weight = 125,
        .description = gAnorithPokedexText,
    },

    [NATIONAL_DEX_ARMALDO] =
    {
        .categoryName = _("Plate"),
        .height = 15,
        .weight = 682,
        .description = gArmaldoPokedexText,
    },

    [NATIONAL_DEX_FEEBAS] =
    {
        .categoryName = _("Fish"),
        .height = 6,
        .weight = 74,
        .description = gFeebasPokedexText,
    },

    [NATIONAL_DEX_MILOTIC] =
    {
        .categoryName = _("Tender"),
        .height = 62,
        .weight = 1620,
        .description = gMiloticPokedexText,
    },

    [NATIONAL_DEX_CASTFORM] =
    {
        .categoryName = _("Weather"),
        .height = 3,
        .weight = 8,
        .description = gCastformPokedexText,
    },

    [NATIONAL_DEX_KECLEON] =
    {
        .categoryName = _("Color Swap"),
        .height = 10,
        .weight = 220,
        .description = gKecleonPokedexText,
    },

    [NATIONAL_DEX_SHUPPET] =
    {
        .categoryName = _("Puppet"),
        .height = 6,
        .weight = 23,
        .description = gShuppetPokedexText,
    },

    [NATIONAL_DEX_BANETTE] =
    {
        .categoryName = _("Marionette"),
        .height = 11,
        .weight = 125,
        .description = gBanettePokedexText,
    },

    [NATIONAL_DEX_DUSKULL] =
    {
        .categoryName = _("Requiem"),
        .height = 8,
        .weight = 150,
        .description = gDuskullPokedexText,
    },

    [NATIONAL_DEX_DUSCLOPS] =
    {
        .categoryName = _("Beckon"),
        .height = 16,
        .weight = 306,
        .description = gDusclopsPokedexText,
    },

    [NATIONAL_DEX_TROPIUS] =
    {
        .categoryName = _("Fruit"),
        .height = 20,
        .weight = 1000,
        .description = gTropiusPokedexText,
    },

    [NATIONAL_DEX_CHIMECHO] =
    {
        .categoryName = _("Wind Chime"),
        .height = 6,
        .weight = 10,
        .description = gChimechoPokedexText,
    },

    [NATIONAL_DEX_ABSOL] =
    {
        .categoryName = _("Disaster"),
        .height = 12,
        .weight = 470,
        .description = gAbsolPokedexText,
    },

    [NATIONAL_DEX_WYNAUT] =
    {
        .categoryName = _("Bright"),
        .height = 6,
        .weight = 140,
        .description = gWynautPokedexText,
    },

    [NATIONAL_DEX_SNORUNT] =
    {
        .categoryName = _("Snow Hat"),
        .height = 7,
        .weight = 168,
        .description = gSnoruntPokedexText,
    },

    [NATIONAL_DEX_GLALIE] =
    {
        .categoryName = _("Face"),
        .height = 15,
        .weight = 2565,
        .description = gGlaliePokedexText,
    },

    [NATIONAL_DEX_SPHEAL] =
    {
        .categoryName = _("Clap"),
        .height = 8,
        .weight = 395,
        .description = gSphealPokedexText,
    },

    [NATIONAL_DEX_SEALEO] =
    {
        .categoryName = _("Ball Roll"),
        .height = 11,
        .weight = 876,
        .description = gSealeoPokedexText,
    },

    [NATIONAL_DEX_WALREIN] =
    {
        .categoryName = _("Ice Break"),
        .height = 14,
        .weight = 1506,
        .description = gWalreinPokedexText,
    },

    [NATIONAL_DEX_CLAMPERL] =
    {
        .categoryName = _("Bivalve"),
        .height = 4,
        .weight = 525,
        .description = gClamperlPokedexText,
    },

    [NATIONAL_DEX_HUNTAIL] =
    {
        .categoryName = _("Deep Sea"),
        .height = 17,
        .weight = 270,
        .description = gHuntailPokedexText,
    },

    [NATIONAL_DEX_GOREBYSS] =
    {
        .categoryName = _("South Sea"),
        .height = 18,
        .weight = 226,
        .description = gGorebyssPokedexText,
    },

    [NATIONAL_DEX_RELICANTH] =
    {
        .categoryName = _("Longevity"),
        .height = 10,
        .weight = 234,
        .description = gRelicanthPokedexText,
    },

    [NATIONAL_DEX_LUVDISC] =
    {
        .categoryName = _("Rendezvous"),
        .height = 6,
        .weight = 87,
        .description = gLuvdiscPokedexText,
    },

    [NATIONAL_DEX_BAGON] =
    {
        .categoryName = _("Rock Head"),
        .height = 6,
        .weight = 421,
        .description = gBagonPokedexText,
    },

    [NATIONAL_DEX_SHELGON] =
    {
        .categoryName = _("Endurance"),
        .height = 11,
        .weight = 1105,
        .description = gShelgonPokedexText,
    },

    [NATIONAL_DEX_SALAMENCE] =
    {
        .categoryName = _("Dragon"),
        .height = 15,
        .weight = 1026,
        .description = gSalamencePokedexText,
    },

    [NATIONAL_DEX_BELDUM] =
    {
        .categoryName = _("Iron Ball"),
        .height = 6,
        .weight = 952,
        .description = gBeldumPokedexText,
    },

    [NATIONAL_DEX_METANG] =
    {
        .categoryName = _("Iron Claw"),
        .height = 12,
        .weight = 2025,
        .description = gMetangPokedexText,
    },

    [NATIONAL_DEX_METAGROSS] =
    {
        .categoryName = _("Iron Leg"),
        .height = 16,
        .weight = 5500,
        .description = gMetagrossPokedexText,
    },

    [NATIONAL_DEX_REGIROCK] =
    {
        .categoryName = _("Rock Peak"),
        .height = 17,
        .weight = 2300,
        .description = gRegirockPokedexText,
    },

    [NATIONAL_DEX_REGICE] =
    {
        .categoryName = _("Iceberg"),
        .height = 18,
        .weight = 1750,
        .description = gRegicePokedexText,
    },

    [NATIONAL_DEX_REGISTEEL] =
    {
        .categoryName = _("Iron"),
        .height = 19,
        .weight = 2050,
        .description = gRegisteelPokedexText,
    },

    [NATIONAL_DEX_LATIAS] =
    {
        .categoryName = _("Eon"),
        .height = 14,
        .weight = 400,
        .description = gLatiasPokedexText,
    },

    [NATIONAL_DEX_LATIOS] =
    {
        .categoryName = _("Eon"),
        .height = 20,
        .weight = 600,
        .description = gLatiosPokedexText,
    },

    [NATIONAL_DEX_KYOGRE] =
    {
        .categoryName = _("Sea Basin"),
        .height = 45,
        .weight = 3520,
        .description = gKyogrePokedexText,
    },

    [NATIONAL_DEX_GROUDON] =
    {
        .categoryName = _("Continent"),
        .height = 35,
        .weight = 9500,
        .description = gGroudonPokedexText,
    },

    [NATIONAL_DEX_RAYQUAZA] =
    {
        .categoryName = _("Sky High"),
        .height = 70,
        .weight = 2065,
        .description = gRayquazaPokedexText,
    },

    [NATIONAL_DEX_JIRACHI] =
    {
        .categoryName = _("Wish"),
        .height = 3,
        .weight = 11,
        .description = gJirachiPokedexText,
    },

    [NATIONAL_DEX_DEOXYS] =
    {
        .categoryName = _("DNA"),
        .height = 17,
        .weight = 608,
        .description = gDeoxysPokedexText,
    },

    [NATIONAL_DEX_TURTWIG] =
    {
        .categoryName = _("Tiny Leaf"),
        .height = 4,
        .weight = 102,
        .description = gTurtwigPokedexText,
    },

    [NATIONAL_DEX_GROTLE] =
    {
        .categoryName = _("Grove"),
        .height = 11,
        .weight = 970,
        .description = gGrotlePokedexText,
    },

    [NATIONAL_DEX_TORTERRA] =
    {
        .categoryName = _("Continent"),
        .height = 22,
        .weight = 3100,
        .description = gTorterraPokedexText,
    },

    [NATIONAL_DEX_CHIMCHAR] =
    {
        .categoryName = _("Chimp"),
        .height = 5,
        .weight = 62,
        .description = gChimcharPokedexText,
    },

    [NATIONAL_DEX_MONFERNO] =
    {
        .categoryName = _("Playful"),
        .height = 9,
        .weight = 220,
        .description = gMonfernoPokedexText,
    },

    [NATIONAL_DEX_INFERNAPE] =
    {
        .categoryName = _("Flame"),
        .height = 12,
        .weight = 550,
        .description = gInfernapePokedexText,
    },

    [NATIONAL_DEX_PIPLUP] =
    {
        .categoryName = _("Penguin"),
        .height = 4,
        .weight = 52,
        .description = gPiplupPokedexText,
    },

    [NATIONAL_DEX_PRINPLUP] =
    {
        .categoryName = _("Penguin"),
        .height = 8,
        .weight = 230,
        .description = gPrinplupPokedexText,
    },

    [NATIONAL_DEX_EMPOLEON] =
    {
        .categoryName = _("Emperor"),
        .height = 17,
        .weight = 845,
        .description = gEmpoleonPokedexText,
    },

    [NATIONAL_DEX_STARLY] =
    {
        .categoryName = _("Starling"),
        .height = 3,
        .weight = 20,
        .description = gStarlyPokedexText,
    },

    [NATIONAL_DEX_STARAVIA] =
    {
        .categoryName = _("Starling"),
        .height = 6,
        .weight = 155,
        .description = gStaraviaPokedexText,
    },

    [NATIONAL_DEX_STARAPTOR] =
    {
        .categoryName = _("Predator"),
        .height = 12,
        .weight = 249,
        .description = gStaraptorPokedexText,
    },

    [NATIONAL_DEX_BIDOOF] =
    {
        .categoryName = _("Plump Mouse"),
        .height = 5,
        .weight = 200,
        .description = gBidoofPokedexText,
    },

    [NATIONAL_DEX_BIBAREL] =
    {
        .categoryName = _("Beaver"),
        .height = 10,
        .weight = 315,
        .description = gBibarelPokedexText,
    },

    [NATIONAL_DEX_KRICKETOT] =
    {
        .categoryName = _("Cricket"),
        .height = 3,
        .weight = 22,
        .description = gKricketotPokedexText,
    },

    [NATIONAL_DEX_KRICKETUNE] =
    {
        .categoryName = _("Cricket"),
        .height = 10,
        .weight = 255,
        .description = gKricketunePokedexText,
    },

    [NATIONAL_DEX_SHINX] =
    {
        .categoryName = _("Flash"),
        .height = 5,
        .weight = 95,
        .description = gShinxPokedexText,
    },

    [NATIONAL_DEX_LUXIO] =
    {
        .categoryName = _("Spark"),
        .height = 9,
        .weight = 305,
        .description = gLuxioPokedexText,
    },

    [NATIONAL_DEX_LUXRAY] =
    {
        .categoryName = _("Gleam Eyes"),
        .height = 14,
        .weight = 420,
        .description = gLuxrayPokedexText,
    },

    [NATIONAL_DEX_BUDEW] =
    {
        .categoryName = _("Bud"),
        .height = 2,
        .weight = 12,
        .description = gBudewPokedexText,
    },

    [NATIONAL_DEX_ROSERADE] =
    {
        .categoryName = _("Bouquet"),
        .height = 9,
        .weight = 145,
        .description = gRoseradePokedexText,
    },

    [NATIONAL_DEX_CRANIDOS] =
    {
        .categoryName = _("Head Butt"),
        .height = 9,
        .weight = 315,
        .description = gCranidosPokedexText,
    },

    [NATIONAL_DEX_RAMPARDOS] =
    {
        .categoryName = _("Head Butt"),
        .height = 16,
        .weight = 1025,
        .description = gRampardosPokedexText,
    },

    [NATIONAL_DEX_SHIELDON] =
    {
        .categoryName = _("Shield"),
        .height = 5,
        .weight = 570,
        .description = gShieldonPokedexText,
    },

    [NATIONAL_DEX_BASTIODON] =
    {
        .categoryName = _("Shield"),
        .height = 13,
        .weight = 1495,
        .description = gBastiodonPokedexText,
    },

    [NATIONAL_DEX_BURMY] =
    {
        .categoryName = _("Bagworm"),
        .height = 2,
        .weight = 34,
        .description = gBurmyPokedexText,
    },

    [NATIONAL_DEX_WORMADAM] =
    {
        .categoryName = _("Bagworm"),
        .height = 5,
        .weight = 65,
        .description = gWormadamPokedexText,
    },

    [NATIONAL_DEX_MOTHIM] =
    {
        .categoryName = _("Moth"),
        .height = 9,
        .weight = 233,
        .description = gMothimPokedexText,
    },

    [NATIONAL_DEX_COMBEE] =
    {
        .categoryName = _("Tiny Bee"),
        .height = 3,
        .weight = 55,
        .description = gCombeePokedexText,
    },

    [NATIONAL_DEX_VESPIQUEN] =
    {
        .categoryName = _("Beehive"),
        .height = 12,
        .weight = 385,
        .description = gVespiquenPokedexText,
    },

    [NATIONAL_DEX_PACHIRISU] =
    {
        .categoryName = _("EleSquirrel"),
        .height = 4,
        .weight = 39,
        .description = gPachirisuPokedexText,
    },

    [NATIONAL_DEX_BUIZEL] =
    {
        .categoryName = _("Sea Weasel"),
        .height = 7,
        .weight = 295,
        .description = gBuizelPokedexText,
    },

    [NATIONAL_DEX_FLOATZEL] =
    {
        .categoryName = _("Sea Weasel"),
        .height = 11,
        .weight = 335,
        .description = gFloatzelPokedexText,
    },

    [NATIONAL_DEX_CHERUBI] =
    {
        .categoryName = _("Cherry"),
        .height = 4,
        .weight = 33,
        .description = gCherubiPokedexText,
    },

    [NATIONAL_DEX_CHERRIM] =
    {
        .categoryName = _("Blossom"),
        .height = 5,
        .weight = 93,
        .description = gCherrimPokedexText,
    },

    [NATIONAL_DEX_SHELLOS] =
    {
        .categoryName = _("Sea Slug"),
        .height = 3,
        .weight = 63,
        .description = gShellosPokedexText,
    },

    [NATIONAL_DEX_GASTRODON] =
    {
        .categoryName = _("Sea Slug"),
        .height = 9,
        .weight = 299,
        .description = gGastrodonPokedexText,
    },

    [NATIONAL_DEX_AMBIPOM] =
    {
        .categoryName = _("Long Tail"),
        .height = 12,
        .weight = 203,
        .description = gAmbipomPokedexText,
    },

    [NATIONAL_DEX_DRIFLOON] =
    {
        .categoryName = _("Balloon"),
        .height = 4,
        .weight = 12,
        .description = gDrifloonPokedexText,
    },

    [NATIONAL_DEX_DRIFBLIM] =
    {
        .categoryName = _("Blimp"),
        .height = 12,
        .weight = 150,
        .description = gDrifblimPokedexText,
    },

    [NATIONAL_DEX_BUNEARY] =
    {
        .categoryName = _("Rabbit"),
        .height = 4,
        .weight = 55,
        .description = gBunearyPokedexText,
    },

    [NATIONAL_DEX_LOPUNNY] =
    {
        .categoryName = _("Rabbit"),
        .height = 12,
        .weight = 333,
        .description = gLopunnyPokedexText,
    },

    [NATIONAL_DEX_MISMAGIUS] =
    {
        .categoryName = _("Magical"),
        .height = 9,
        .weight = 44,
        .description = gMismagiusPokedexText,
    },

    [NATIONAL_DEX_HONCHKROW] =
    {
        .categoryName = _("Big Boss"),
        .height = 9,
        .weight = 273,
        .description = gHonchkrowPokedexText,
    },

    [NATIONAL_DEX_GLAMEOW] =
    {
        .categoryName = _("Catty"),
        .height = 5,
        .weight = 39,
        .description = gGlameowPokedexText,
    },

    [NATIONAL_DEX_PURUGLY] =
    {
        .categoryName = _("Tiger Cat"),
        .height = 10,
        .weight = 438,
        .description = gPuruglyPokedexText,
    },

    [NATIONAL_DEX_CHINGLING] =
    {
        .categoryName = _("Bell"),
        .height = 2,
        .weight = 6,
        .description = gChinglingPokedexText,
    },

    [NATIONAL_DEX_STUNKY] =
    {
        .categoryName = _("Skunk"),
        .height = 4,
        .weight = 192,
        .description = gStunkyPokedexText,
    },

    [NATIONAL_DEX_SKUNTANK] =
    {
        .categoryName = _("Skunk"),
        .height = 10,
        .weight = 380,
        .description = gSkuntankPokedexText,
    },

    [NATIONAL_DEX_BRONZOR] =
    {
        .categoryName = _("Bronze"),
        .height = 5,
        .weight = 605,
        .description = gBronzorPokedexText,
    },

    [NATIONAL_DEX_BRONZONG] =
    {
        .categoryName = _("Bronze Bell"),
        .height = 13,
        .weight = 1870,
        .description = gBronzongPokedexText,
    },

    [NATIONAL_DEX_BONSLY] =
    {
        .categoryName = _("Bonsai"),
        .height = 5,
        .weight = 150,
        .description = gBonslyPokedexText,
    },

    [NATIONAL_DEX_MIME_JR] =
    {
        .categoryName = _("Mime"),
        .height = 6,
        .weight = 130,
        .description = gMimeJrPokedexText,
    },

    [NATIONAL_DEX_HAPPINY] =
    {
        .categoryName = _("Playhouse"),
        .height = 6,
        .weight = 244,
        .description = gHappinyPokedexText,
    },

    [NATIONAL_DEX_CHATOT] =
    {
        .categoryName = _("Music Note"),
        .height = 5,
        .weight = 19,
        .description = gChatotPokedexText,
    },

    [NATIONAL_DEX_SPIRITOMB] =
    {
        .categoryName = _("Forbidden"),
        .height = 10,
        .weight = 1080,
        .description = gSpiritombPokedexText,
    },

    [NATIONAL_DEX_GIBLE] =
    {
        .categoryName = _("Land Shark"),
        .height = 7,
        .weight = 205,
        .description = gGiblePokedexText,
    },

    [NATIONAL_DEX_GABITE] =
    {
        .categoryName = _("Cave"),
        .height = 14,
        .weight = 560,
        .description = gGabitePokedexText,
    },

    [NATIONAL_DEX_GARCHOMP] =
    {
        .categoryName = _("Mach"),
        .height = 19,
        .weight = 950,
        .description = gGarchompPokedexText,
    },

    [NATIONAL_DEX_MUNCHLAX] =
    {
        .categoryName = _("Big Eater"),
        .height = 6,
        .weight = 1050,
        .description = gMunchlaxPokedexText,
    },

    [NATIONAL_DEX_RIOLU] =
    {
        .categoryName = _("Emanation"),
        .height = 7,
        .weight = 202,
        .description = gRioluPokedexText,
    },

    [NATIONAL_DEX_LUCARIO] =
    {
        .categoryName = _("Aura"),
        .height = 12,
        .weight = 540,
        .description = gLucarioPokedexText,
    },

    [NATIONAL_DEX_HIPPOPOTAS] =
    {
        .categoryName = _("Hippo"),
        .height = 8,
        .weight = 495,
        .description = gHippopotasPokedexText,
    },

    [NATIONAL_DEX_HIPPOWDON] =
    {
        .categoryName = _("Heavyweight"),
        .height = 20,
        .weight = 3000,
        .description = gHippowdonPokedexText,
    },

    [NATIONAL_DEX_SKORUPI] =
    {
        .categoryName = _("Scorpion"),
        .height = 8,
        .weight = 120,
        .description = gSkorupiPokedexText,
    },

    [NATIONAL_DEX_DRAPION] =
    {
        .categoryName = _("Ogre Scorp"),
        .height = 13,
        .weight = 615,
        .description = gDrapionPokedexText,
    },

    [NATIONAL_DEX_CROAGUNK] =
    {
        .categoryName = _("Toxic Mouth"),
        .height = 7,
        .weight = 230,
        .description = gCroagunkPokedexText,
    },

    [NATIONAL_DEX_TOXICROAK] =
    {
        .categoryName = _("Toxic Mouth"),
        .height = 13,
        .weight = 444,
        .description = gToxicroakPokedexText,
    },

    [NATIONAL_DEX_CARNIVINE] =
    {
        .categoryName = _("Bug Catcher"),
        .height = 14,
        .weight = 270,
        .description = gCarnivinePokedexText,
    },

    [NATIONAL_DEX_FINNEON] =
    {
        .categoryName = _("Wing Fish"),
        .height = 4,
        .weight = 70,
        .description = gFinneonPokedexText,
    },

    [NATIONAL_DEX_LUMINEON] =
    {
        .categoryName = _("Neon"),
        .height = 12,
        .weight = 240,
        .description = gLumineonPokedexText,
    },

    [NATIONAL_DEX_MANTYKE] =
    {
        .categoryName = _("Kite"),
        .height = 10,
        .weight = 650,
        .description = gMantykePokedexText,
    },

    [NATIONAL_DEX_SNOVER] =
    {
        .categoryName = _("Frost Tree"),
        .height = 10,
        .weight = 505,
        .description = gSnoverPokedexText,
    },

    [NATIONAL_DEX_ABOMASNOW] =
    {
        .categoryName = _("Frost Tree"),
        .height = 22,
        .weight = 1355,
        .description = gAbomasnowPokedexText,
    },

    [NATIONAL_DEX_WEAVILE] =
    {
        .categoryName = _("Sharp Claw"),
        .height = 11,
        .weight = 340,
        .description = gWeavilePokedexText,
    },

    [NATIONAL_DEX_MAGNEZONE] =
    {
        .categoryName = _("Magnet Area"),
        .height = 12,
        .weight = 1800,
        .description = gMagnezonePokedexText,
    },

    [NATIONAL_DEX_LICKILICKY] =
    {
        .categoryName = _("Licking"),
        .height = 17,
        .weight = 1400,
        .description = gLickilickyPokedexText,
    },

    [NATIONAL_DEX_RHYPERIOR] =
    {
        .categoryName = _("Drill"),
        .height = 24,
        .weight = 2828,
        .description = gRhyperiorPokedexText,
    },

    [NATIONAL_DEX_TANGROWTH] =
    {
        .categoryName = _("Vine"),
        .height = 20,
        .weight = 1286,
        .description = gTangrowthPokedexText,
    },

    [NATIONAL_DEX_ELECTIVIRE] =
    {
        .categoryName = _("Thunderbolt"),
        .height = 18,
        .weight = 1386,
        .description = gElectivirePokedexText,
    },

    [NATIONAL_DEX_MAGMORTAR] =
    {
        .categoryName = _("Blast"),
        .height = 16,
        .weight = 680,
        .description = gMagmortarPokedexText,
    },

    [NATIONAL_DEX_TOGEKISS] =
    {
        .categoryName = _("Jubilee"),
        .height = 15,
        .weight = 380,
        .description = gTogekissPokedexText,
    },

    [NATIONAL_DEX_YANMEGA] =
    {
        .categoryName = _("Ogre Darner"),
        .height = 19,
        .weight = 515,
        .description = gYanmegaPokedexText,
    },

    [NATIONAL_DEX_LEAFEON] =
    {
        .categoryName = _("Verdant"),
        .height = 10,
        .weight = 255,
        .description = gLeafeonPokedexText,
    },

    [NATIONAL_DEX_GLACEON] =
    {
        .categoryName = _("Fresh Snow"),
        .height = 8,
        .weight = 259,
        .description = gGlaceonPokedexText,
    },

    [NATIONAL_DEX_GLISCOR] =
    {
        .categoryName = _("Fang Scorp"),
        .height = 20,
        .weight = 425,
        .description = gGliscorPokedexText,
    },

    [NATIONAL_DEX_MAMOSWINE] =
    {
        .categoryName = _("Twin Tusk"),
        .height = 25,
        .weight = 2910,
        .description = gMamoswinePokedexText,
    },

    [NATIONAL_DEX_PORYGON_Z] =
    {
        .categoryName = _("Virtual"),
        .height = 9,
        .weight = 340,
        .description = gPorygonZPokedexText,
    },

    [NATIONAL_DEX_GALLADE] =
    {
        .categoryName = _("Blade"),
        .height = 16,
        .weight = 520,
        .description = gGalladePokedexText,
    },

    [NATIONAL_DEX_PROBOPASS] =
    {
        .categoryName = _("Compass"),
        .height = 14,
        .weight = 3400,
        .description = gProbopassPokedexText,
    },

    [NATIONAL_DEX_DUSKNOIR] =
    {
        .categoryName = _("Gripper"),
        .height = 22,
        .weight = 1066,
        .description = gDusknoirPokedexText,
    },

    [NATIONAL_DEX_FROSLASS] =
    {
        .categoryName = _("Snow Land"),
        .height = 13,
        .weight = 266,
        .description = gFroslassPokedexText,
    },

    [NATIONAL_DEX_ROTOM] =
    {
        .categoryName = _("Plasma"),
        .height = 3,
        .weight = 3,
        .description = gRotomPokedexText,
    },

    [NATIONAL_DEX_UXIE] =
    {
        .categoryName = _("Knowledge"),
        .height = 3,
        .weight = 3,
        .description = gUxiePokedexText,
    },

    [NATIONAL_DEX_MESPRIT] =
    {
        .categoryName = _("Emotion"),
        .height = 3,
        .weight = 3,
        .description = gMespritPokedexText,
    },

    [NATIONAL_DEX_AZELF] =
    {
        .categoryName = _("Willpower"),
        .height = 3,
        .weight = 3,
        .description = gAzelfPokedexText,
    },

    [NATIONAL_DEX_DIALGA] =
    {
        .categoryName = _("Temporal"),
        .height = 54,
        .weight = 6830,
        .description = gDialgaPokedexText,
    },

    [NATIONAL_DEX_PALKIA] =
    {
        .categoryName = _("Spatial"),
        .height = 42,
        .weight = 3360,
        .description = gPalkiaPokedexText,
    },

    [NATIONAL_DEX_HEATRAN] =
    {
        .categoryName = _("Lava Dome"),
        .height = 17,
        .weight = 4300,
        .description = gHeatranPokedexText,
    },

    [NATIONAL_DEX_REGIGIGAS] =
    {
        .categoryName = _("Colossal"),
        .height = 37,
        .weight = 4200,
        .description = gRegigigasPokedexText,
    },

    [NATIONAL_DEX_GIRATINA] =
    {
        .categoryName = _("Renegade"),
        .height = 45,
        .weight = 7500,
        .description = gGiratinaPokedexText,
    },

    [NATIONAL_DEX_CRESSELIA] =
    {
        .categoryName = _("Lunar"),
        .height = 15,
        .weight = 856,
        .description = gCresseliaPokedexText,
    },

    [NATIONAL_DEX_PHIONE] =
    {
        .categoryName = _("Sea Drifter"),
        .height = 4,
        .weight = 31,
        .description = gPhionePokedexText,
    },

    [NATIONAL_DEX_MANAPHY] =
    {
        .categoryName = _("Seafaring"),
        .height = 3,
        .weight = 14,
        .description = gManaphyPokedexText,
    },

    [NATIONAL_DEX_DARKRAI] =
    {
        .categoryName = _("Pitch-Black"),
        .height = 15,
        .weight = 505,
        .description = gDarkraiPokedexText,
    },

    [NATIONAL_DEX_SHAYMIN] =
    {
        .categoryName = _("Gratitude"),
        .height = 2,
        .weight = 21,
        .description = gShayminPokedexText,
    },

    [NATIONAL_DEX_ARCEUS] =
    {
        .categoryName = _("Alpha"),
        .height = 32,
        .weight = 3200,
        .description = gArceusPokedexText,
    },

    [NATIONAL_DEX_VICTINI] =
    {
        .categoryName = _("Victory"),
        .height = 4,
        .weight = 40,
        .description = gVictiniPokedexText,
    },

    [NATIONAL_DEX_SNIVY] =
    {
        .categoryName = _("Grass Snake"),
        .height = 6,
        .weight = 81,
        .description = gSnivyPokedexText,
    },

    [NATIONAL_DEX_SERVINE] =
    {
        .categoryName = _("Grass Snake"),
        .height = 8,
        .weight = 160,
        .description = gServinePokedexText,
    },

    [NATIONAL_DEX_SERPERIOR] =
    {
        .categoryName = _("Regal"),
        .height = 33,
        .weight = 630,
        .description = gSerperiorPokedexText,
    },

    [NATIONAL_DEX_TEPIG] =
    {
        .categoryName = _("Fire Pig"),
        .height = 5,
        .weight = 99,
        .description = gTepigPokedexText,
    },

    [NATIONAL_DEX_PIGNITE] =
    {
        .categoryName = _("Fire Pig"),
        .height = 10,
        .weight = 555,
        .description = gPignitePokedexText,
    },

    [NATIONAL_DEX_EMBOAR] =
    {
        .categoryName = _("Fire Pig"),
        .height = 16,
        .weight = 1500,
        .description = gEmboarPokedexText,
    },

    [NATIONAL_DEX_OSHAWOTT] =
    {
        .categoryName = _("Sea Otter"),
        .height = 5,
        .weight = 59,
        .description = gOshawottPokedexText,
    },

    [NATIONAL_DEX_DEWOTT] =
    {
        .categoryName = _("Discipline"),
        .height = 8,
        .weight = 245,
        .description = gDewottPokedexText,
    },

    [NATIONAL_DEX_SAMUROTT] =
    {
        .categoryName = _("Formidable"),
        .height = 15,
        .weight = 946,
        .description = gSamurottPokedexText,
    },

    [NATIONAL_DEX_PATRAT] =
    {
        .categoryName = _("Scout"),
        .height = 5,
        .weight = 116,
        .description = gPatratPokedexText,
    },

    [NATIONAL_DEX_WATCHOG] =
    {
        .categoryName = _("Lookout"),
        .height = 11,
        .weight = 270,
        .description = gWatchogPokedexText,
    },

    [NATIONAL_DEX_LILLIPUP] =
    {
        .categoryName = _("Puppy"),
        .height = 4,
        .weight = 41,
        .description = gLillipupPokedexText,
    },

    [NATIONAL_DEX_HERDIER] =
    {
        .categoryName = _("Loyal Dog"),
        .height = 9,
        .weight = 147,
        .description = gHerdierPokedexText,
    },

    [NATIONAL_DEX_STOUTLAND] =
    {
        .categoryName = _("Big-Hearted"),
        .height = 12,
        .weight = 610,
        .description = gStoutlandPokedexText,
    },

    [NATIONAL_DEX_PURRLOIN] =
    {
        .categoryName = _("Devious"),
        .height = 4,
        .weight = 101,
        .description = gPurrloinPokedexText,
    },

    [NATIONAL_DEX_LIEPARD] =
    {
        .categoryName = _("Cruel"),
        .height = 11,
        .weight = 375,
        .description = gLiepardPokedexText,
    },

    [NATIONAL_DEX_PANSAGE] =
    {
        .categoryName = _("Grass Monkey"),
        .height = 6,
        .weight = 105,
        .description = gPansagePokedexText,
    },

    [NATIONAL_DEX_SIMISAGE] =
    {
        .categoryName = _("Thorn Monkey"),
        .height = 11,
        .weight = 305,
        .description = gSimisagePokedexText,
    },

    [NATIONAL_DEX_PANSEAR] =
    {
        .categoryName = _("High Temp"),
        .height = 6,
        .weight = 110,
        .description = gPansearPokedexText,
    },

    [NATIONAL_DEX_SIMISEAR] =
    {
        .categoryName = _("Ember"),
        .height = 10,
        .weight = 280,
        .description = gSimisearPokedexText,
    },

    [NATIONAL_DEX_PANPOUR] =
    {
        .categoryName = _("Spray"),
        .height = 6,
        .weight = 135,
        .description = gPanpourPokedexText,
    },

    [NATIONAL_DEX_SIMIPOUR] =
    {
        .categoryName = _("Geyser"),
        .height = 10,
        .weight = 290,
        .description = gSimipourPokedexText,
    },

    [NATIONAL_DEX_MUNNA] =
    {
        .categoryName = _("Dream Eater"),
        .height = 6,
        .weight = 233,
        .description = gMunnaPokedexText,
    },

    [NATIONAL_DEX_MUSHARNA] =
    {
        .categoryName = _("Drowsing"),
        .height = 11,
        .weight = 605,
        .description = gMusharnaPokedexText,
    },

    [NATIONAL_DEX_PIDOVE] =
    {
        .categoryName = _("Tiny Pigeon"),
        .height = 3,
        .weight = 21,
        .description = gPidovePokedexText,
    },

    [NATIONAL_DEX_TRANQUILL] =
    {
        .categoryName = _("Wild Pigeon"),
        .height = 6,
        .weight = 150,
        .description = gTranquillPokedexText,
    },

    [NATIONAL_DEX_UNFEZANT] =
    {
        .categoryName = _("Proud"),
        .height = 12,
        .weight = 290,
        .description = gUnfezantPokedexText,
    },

    [NATIONAL_DEX_BLITZLE] =
    {
        .categoryName = _("Electrified"),
        .height = 8,
        .weight = 298,
        .description = gBlitzlePokedexText,
    },

    [NATIONAL_DEX_ZEBSTRIKA] =
    {
        .categoryName = _("Thunderbolt"),
        .height = 16,
        .weight = 795,
        .description = gZebstrikaPokedexText,
    },

    [NATIONAL_DEX_ROGGENROLA] =
    {
        .categoryName = _("Mantle"),
        .height = 4,
        .weight = 180,
        .description = gRoggenrolaPokedexText,
    },

    [NATIONAL_DEX_BOLDORE] =
    {
        .categoryName = _("Ore"),
        .height = 9,
        .weight = 1020,
        .description = gBoldorePokedexText,
    },

    [NATIONAL_DEX_GIGALITH] =
    {
        .categoryName = _("Compressed"),
        .height = 17,
        .weight = 2600,
        .description = gGigalithPokedexText,
    },

    [NATIONAL_DEX_WOOBAT] =
    {
        .categoryName = _("Bat"),
        .height = 4,
        .weight = 21,
        .description = gWoobatPokedexText,
    },

    [NATIONAL_DEX_SWOOBAT] =
    {
        .categoryName = _("Courting"),
        .height = 9,
        .weight = 105,
        .description = gSwoobatPokedexText,
    },

    [NATIONAL_DEX_DRILBUR] =
    {
        .categoryName = _("Mole"),
        .height = 3,
        .weight = 85,
        .description = gDrilburPokedexText,
    },

    [NATIONAL_DEX_EXCADRILL] =
    {
        .categoryName = _("Subterrene"),
        .height = 7,
        .weight = 404,
        .description = gExcadrillPokedexText,
    },

    [NATIONAL_DEX_AUDINO] =
    {
        .categoryName = _("Hearing"),
        .height = 11,
        .weight = 310,
        .description = gAudinoPokedexText,
    },

    [NATIONAL_DEX_TIMBURR] =
    {
        .categoryName = _("Muscular"),
        .height = 6,
        .weight = 125,
        .description = gTimburrPokedexText,
    },

    [NATIONAL_DEX_GURDURR] =
    {
        .categoryName = _("Muscular"),
        .height = 12,
        .weight = 400,
        .description = gGurdurrPokedexText,
    },

    [NATIONAL_DEX_CONKELDURR] =
    {
        .categoryName = _("Muscular"),
        .height = 14,
        .weight = 870,
        .description = gConkeldurrPokedexText,
    },

    [NATIONAL_DEX_TYMPOLE] =
    {
        .categoryName = _("Tadpole"),
        .height = 5,
        .weight = 45,
        .description = gTympolePokedexText,
    },

    [NATIONAL_DEX_PALPITOAD] =
    {
        .categoryName = _("Vibration"),
        .height = 8,
        .weight = 170,
        .description = gPalpitoadPokedexText,
    },

    [NATIONAL_DEX_SEISMITOAD] =
    {
        .categoryName = _("Vibration"),
        .height = 15,
        .weight = 620,
        .description = gSeismitoadPokedexText,
    },

    [NATIONAL_DEX_THROH] =
    {
        .categoryName = _("Judo"),
        .height = 13,
        .weight = 555,
        .description = gThrohPokedexText,
    },

    [NATIONAL_DEX_SAWK] =
    {
        .categoryName = _("Karate"),
        .height = 14,
        .weight = 510,
        .description = gSawkPokedexText,
    },

    [NATIONAL_DEX_SEWADDLE] =
    {
        .categoryName = _("Sewing"),
        .height = 3,
        .weight = 25,
        .description = gSewaddlePokedexText,
    },

    [NATIONAL_DEX_SWADLOON] =
    {
        .categoryName = _("Leaf-Wrapped"),
        .height = 5,
        .weight = 73,
        .description = gSwadloonPokedexText,
    },

    [NATIONAL_DEX_LEAVANNY] =
    {
        .categoryName = _("Nurturing"),
        .height = 12,
        .weight = 205,
        .description = gLeavannyPokedexText,
    },

    [NATIONAL_DEX_VENIPEDE] =
    {
        .categoryName = _("Centipede"),
        .height = 4,
        .weight = 53,
        .description = gVenipedePokedexText,
    },

    [NATIONAL_DEX_WHIRLIPEDE] =
    {
        .categoryName = _("Curlipede"),
        .height = 12,
        .weight = 585,
        .description = gWhirlipedePokedexText,
    },

    [NATIONAL_DEX_SCOLIPEDE] =
    {
        .categoryName = _("Megapede"),
        .height = 25,
        .weight = 2005,
        .description = gScolipedePokedexText,
    },

    [NATIONAL_DEX_COTTONEE] =
    {
        .categoryName = _("Cotton Puff"),
        .height = 3,
        .weight = 6,
        .description = gCottoneePokedexText,
    },

    [NATIONAL_DEX_WHIMSICOTT] =
    {
        .categoryName = _("Windveiled"),
        .height = 7,
        .weight = 66,
        .description = gWhimsicottPokedexText,
    },

    [NATIONAL_DEX_PETILIL] =
    {
        .categoryName = _("Bulb"),
        .height = 5,
        .weight = 66,
        .description = gPetililPokedexText,
    },

    [NATIONAL_DEX_LILLIGANT] =
    {
        .categoryName = _("Flowering"),
        .height = 11,
        .weight = 163,
        .description = gLilligantPokedexText,
    },

    [NATIONAL_DEX_BASCULIN] =
    {
        .categoryName = _("Hostile"),
        .height = 10,
        .weight = 180,
        .description = gBasculinPokedexText,
    },

    [NATIONAL_DEX_SANDILE] =
    {
        .categoryName = _("Desert Croc"),
        .height = 7,
        .weight = 152,
        .description = gSandilePokedexText,
    },

    [NATIONAL_DEX_KROKOROK] =
    {
        .categoryName = _("Desert Croc"),
        .height = 10,
        .weight = 334,
        .description = gKrokorokPokedexText,
    },

    [NATIONAL_DEX_KROOKODILE] =
    {
        .categoryName = _("Intimidate"),
        .height = 15,
        .weight = 963,
        .description = gKrookodilePokedexText,
    },

    [NATIONAL_DEX_DARUMAKA] =
    {
        .categoryName = _("Zen Charm"),
        .height = 6,
        .weight = 375,
        .description = gDarumakaPokedexText,
    },

    [NATIONAL_DEX_DARMANITAN] =
    {
        .categoryName = _("Blazing"),
        .height = 13,
        .weight = 929,
        .description = gDarmanitanPokedexText,
    },

    [NATIONAL_DEX_MARACTUS] =
    {
        .categoryName = _("Cactus"),
        .height = 10,
        .weight = 280,
        .description = gMaractusPokedexText,
    },

    [NATIONAL_DEX_DWEBBLE] =
    {
        .categoryName = _("Rock Inn"),
        .height = 3,
        .weight = 145,
        .description = gDwebblePokedexText,
    },

    [NATIONAL_DEX_CRUSTLE] =
    {
        .categoryName = _("Stone Home"),
        .height = 14,
        .weight = 2000,
        .description = gCrustlePokedexText,
    },

    [NATIONAL_DEX_SCRAGGY] =
    {
        .categoryName = _("Shedding"),
        .height = 6,
        .weight = 118,
        .description = gScraggyPokedexText,
    },

    [NATIONAL_DEX_SCRAFTY] =
    {
        .categoryName = _("Hoodlum"),
        .height = 11,
        .weight = 300,
        .description = gScraftyPokedexText,
    },

    [NATIONAL_DEX_SIGILYPH] =
    {
        .categoryName = _("Avianoid"),
        .height = 14,
        .weight = 140,
        .description = gSigilyphPokedexText,
    },

    [NATIONAL_DEX_YAMASK] =
    {
        .categoryName = _("Spirit"),
        .height = 5,
        .weight = 15,
        .description = gYamaskPokedexText,
    },

    [NATIONAL_DEX_COFAGRIGUS] =
    {
        .categoryName = _("Coffin"),
        .height = 17,
        .weight = 765,
        .description = gCofagrigusPokedexText,
    },

    [NATIONAL_DEX_TIRTOUGA] =
    {
        .categoryName = _("Prototurtle"),
        .height = 7,
        .weight = 165,
        .description = gTirtougaPokedexText,
    },

    [NATIONAL_DEX_CARRACOSTA] =
    {
        .categoryName = _("Prototurtle"),
        .height = 12,
        .weight = 810,
        .description = gCarracostaPokedexText,
    },

    [NATIONAL_DEX_ARCHEN] =
    {
        .categoryName = _("First Bird"),
        .height = 5,
        .weight = 95,
        .description = gArchenPokedexText,
    },

    [NATIONAL_DEX_ARCHEOPS] =
    {
        .categoryName = _("First Bird"),
        .height = 14,
        .weight = 320,
        .description = gArcheopsPokedexText,
    },

    [NATIONAL_DEX_TRUBBISH] =
    {
        .categoryName = _("Trash Bag"),
        .height = 6,
        .weight = 310,
        .description = gTrubbishPokedexText,
    },

    [NATIONAL_DEX_GARBODOR] =
    {
        .categoryName = _("Trash Heap"),
        .height = 19,
        .weight = 1073,
        .description = gGarbodorPokedexText,
    },

    [NATIONAL_DEX_ZORUA] =
    {
        .categoryName = _("Tricky Fox"),
        .height = 7,
        .weight = 125,
        .description = gZoruaPokedexText,
    },

    [NATIONAL_DEX_ZOROARK] =
    {
        .categoryName = _("Illusion Fox"),
        .height = 16,
        .weight = 811,
        .description = gZoroarkPokedexText,
    },

    [NATIONAL_DEX_MINCCINO] =
    {
        .categoryName = _("Chinchilla"),
        .height = 4,
        .weight = 58,
        .description = gMinccinoPokedexText,
    },

    [NATIONAL_DEX_CINCCINO] =
    {
        .categoryName = _("Scarf"),
        .height = 5,
        .weight = 75,
        .description = gCinccinoPokedexText,
    },

    [NATIONAL_DEX_GOTHITA] =
    {
        .categoryName = _("Fixation"),
        .height = 4,
        .weight = 58,
        .description = gGothitaPokedexText,
    },

    [NATIONAL_DEX_GOTHORITA] =
    {
        .categoryName = _("Manipulate"),
        .height = 7,
        .weight = 180,
        .description = gGothoritaPokedexText,
    },

    [NATIONAL_DEX_GOTHITELLE] =
    {
        .categoryName = _("Astral Body"),
        .height = 15,
        .weight = 440,
        .description = gGothitellePokedexText,
    },

    [NATIONAL_DEX_SOLOSIS] =
    {
        .categoryName = _("Cell"),
        .height = 3,
        .weight = 10,
        .description = gSolosisPokedexText,
    },

    [NATIONAL_DEX_DUOSION] =
    {
        .categoryName = _("Mitosis"),
        .height = 6,
        .weight = 80,
        .description = gDuosionPokedexText,
    },

    [NATIONAL_DEX_REUNICLUS] =
    {
        .categoryName = _("Multiplying"),
        .height = 10,
        .weight = 201,
        .description = gReuniclusPokedexText,
    },

    [NATIONAL_DEX_DUCKLETT] =
    {
        .categoryName = _("Water Bird"),
        .height = 5,
        .weight = 55,
        .description = gDucklettPokedexText,
    },

    [NATIONAL_DEX_SWANNA] =
    {
        .categoryName = _("White Bird"),
        .height = 13,
        .weight = 242,
        .description = gSwannaPokedexText,
    },

    [NATIONAL_DEX_VANILLITE] =
    {
        .categoryName = _("Fresh Snow"),
        .height = 4,
        .weight = 57,
        .description = gVanillitePokedexText,
    },

    [NATIONAL_DEX_VANILLISH] =
    {
        .categoryName = _("Icy Snow"),
        .height = 11,
        .weight = 410,
        .description = gVanillishPokedexText,
    },

    [NATIONAL_DEX_VANILLUXE] =
    {
        .categoryName = _("Snowstorm"),
        .height = 13,
        .weight = 575,
        .description = gVanilluxePokedexText,
    },

    [NATIONAL_DEX_DEERLING] =
    {
        .categoryName = _("Season"),
        .height = 6,
        .weight = 195,
        .description = gDeerlingPokedexText,
    },

    [NATIONAL_DEX_SAWSBUCK] =
    {
        .categoryName = _("Season"),
        .height = 19,
        .weight = 925,
        .description = gSawsbuckPokedexText,
    },

    [NATIONAL_DEX_EMOLGA] =
    {
        .categoryName = _("Sky Squirrel"),
        .height = 4,
        .weight = 50,
        .description = gEmolgaPokedexText,
    },

    [NATIONAL_DEX_KARRABLAST] =
    {
        .categoryName = _("Clamping"),
        .height = 5,
        .weight = 59,
        .description = gKarrablastPokedexText,
    },

    [NATIONAL_DEX_ESCAVALIER] =
    {
        .categoryName = _("Cavalry"),
        .height = 10,
        .weight = 330,
        .description = gEscavalierPokedexText,
    },

    [NATIONAL_DEX_FOONGUS] =
    {
        .categoryName = _("Mushroom"),
        .height = 2,
        .weight = 10,
        .description = gFoongusPokedexText,
    },

    [NATIONAL_DEX_AMOONGUSS] =
    {
        .categoryName = _("Mushroom"),
        .height = 6,
        .weight = 105,
        .description = gAmoongussPokedexText,
    },

    [NATIONAL_DEX_FRILLISH] =
    {
        .categoryName = _("Floating"),
        .height = 12,
        .weight = 330,
        .description = gFrillishPokedexText,
    },

    [NATIONAL_DEX_JELLICENT] =
    {
        .categoryName = _("Floating"),
        .height = 22,
        .weight = 1350,
        .description = gJellicentPokedexText,
    },

    [NATIONAL_DEX_ALOMOMOLA] =
    {
        .categoryName = _("Caring"),
        .height = 12,
        .weight = 316,
        .description = gAlomomolaPokedexText,
    },

    [NATIONAL_DEX_JOLTIK] =
    {
        .categoryName = _("Attaching"),
        .height = 1,
        .weight = 6,
        .description = gJoltikPokedexText,
    },

    [NATIONAL_DEX_GALVANTULA] =
    {
        .categoryName = _("EleSpider"),
        .height = 8,
        .weight = 143,
        .description = gGalvantulaPokedexText,
    },

    [NATIONAL_DEX_FERROSEED] =
    {
        .categoryName = _("Thorn Seed"),
        .height = 6,
        .weight = 188,
        .description = gFerroseedPokedexText,
    },

    [NATIONAL_DEX_FERROTHORN] =
    {
        .categoryName = _("Thorn Pod"),
        .height = 10,
        .weight = 1100,
        .description = gFerrothornPokedexText,
    },

    [NATIONAL_DEX_KLINK] =
    {
        .categoryName = _("Gear"),
        .height = 3,
        .weight = 210,
        .description = gKlinkPokedexText,
    },

    [NATIONAL_DEX_KLANG] =
    {
        .categoryName = _("Gear"),
        .height = 6,
        .weight = 510,
        .description = gKlangPokedexText,
    },

    [NATIONAL_DEX_KLINKLANG] =
    {
        .categoryName = _("Gear"),
        .height = 6,
        .weight = 810,
        .description = gKlinklangPokedexText,
    },

    [NATIONAL_DEX_TYNAMO] =
    {
        .categoryName = _("EleFish"),
        .height = 2,
        .weight = 3,
        .description = gTynamoPokedexText,
    },

    [NATIONAL_DEX_EELEKTRIK] =
    {
        .categoryName = _("EleFish"),
        .height = 12,
        .weight = 220,
        .description = gEelektrikPokedexText,
    },

    [NATIONAL_DEX_EELEKTROSS] =
    {
        .categoryName = _("EleFish"),
        .height = 21,
        .weight = 805,
        .description = gEelektrossPokedexText,
    },

    [NATIONAL_DEX_ELGYEM] =
    {
        .categoryName = _("Cerebral"),
        .height = 5,
        .weight = 90,
        .description = gElgyemPokedexText,
    },

    [NATIONAL_DEX_BEHEEYEM] =
    {
        .categoryName = _("Cerebral"),
        .height = 10,
        .weight = 345,
        .description = gBeheeyemPokedexText,
    },

    [NATIONAL_DEX_LITWICK] =
    {
        .categoryName = _("Candle"),
        .height = 3,
        .weight = 31,
        .description = gLitwickPokedexText,
    },

    [NATIONAL_DEX_LAMPENT] =
    {
        .categoryName = _("Lamp"),
        .height = 6,
        .weight = 130,
        .description = gLampentPokedexText,
    },

    [NATIONAL_DEX_CHANDELURE] =
    {
        .categoryName = _("Luring"),
        .height = 10,
        .weight = 343,
        .description = gChandelurePokedexText,
    },

    [NATIONAL_DEX_AXEW] =
    {
        .categoryName = _("Tusk"),
        .height = 6,
        .weight = 180,
        .description = gAxewPokedexText,
    },

    [NATIONAL_DEX_FRAXURE] =
    {
        .categoryName = _("Axe Jaw"),
        .height = 10,
        .weight = 360,
        .description = gFraxurePokedexText,
    },

    [NATIONAL_DEX_HAXORUS] =
    {
        .categoryName = _("Axe Jaw"),
        .height = 18,
        .weight = 1055,
        .description = gHaxorusPokedexText,
    },

    [NATIONAL_DEX_CUBCHOO] =
    {
        .categoryName = _("Chill"),
        .height = 5,
        .weight = 85,
        .description = gCubchooPokedexText,
    },

    [NATIONAL_DEX_BEARTIC] =
    {
        .categoryName = _("Freezing"),
        .height = 26,
        .weight = 2600,
        .description = gBearticPokedexText,
    },

    [NATIONAL_DEX_CRYOGONAL] =
    {
        .categoryName = _("Crystallize"),
        .height = 11,
        .weight = 1480,
        .description = gCryogonalPokedexText,
    },

    [NATIONAL_DEX_SHELMET] =
    {
        .categoryName = _("Snail"),
        .height = 4,
        .weight = 77,
        .description = gShelmetPokedexText,
    },

    [NATIONAL_DEX_ACCELGOR] =
    {
        .categoryName = _("Shell Out"),
        .height = 8,
        .weight = 253,
        .description = gAccelgorPokedexText,
    },

    [NATIONAL_DEX_STUNFISK] =
    {
        .categoryName = _("Trap"),
        .height = 7,
        .weight = 110,
        .description = gStunfiskPokedexText,
    },

    [NATIONAL_DEX_MIENFOO] =
    {
        .categoryName = _("Martial Arts"),
        .height = 9,
        .weight = 200,
        .description = gMienfooPokedexText,
    },

    [NATIONAL_DEX_MIENSHAO] =
    {
        .categoryName = _("Martial Arts"),
        .height = 14,
        .weight = 355,
        .description = gMienshaoPokedexText,
    },

    [NATIONAL_DEX_DRUDDIGON] =
    {
        .categoryName = _("Cave"),
        .height = 16,
        .weight = 1390,
        .description = gDruddigonPokedexText,
    },

    [NATIONAL_DEX_GOLETT] =
    {
        .categoryName = _("Automaton"),
        .height = 10,
        .weight = 920,
        .description = gGolettPokedexText,
    },

    [NATIONAL_DEX_GOLURK] =
    {
        .categoryName = _("Automaton"),
        .height = 28,
        .weight = 3300,
        .description = gGolurkPokedexText,
    },

    [NATIONAL_DEX_PAWNIARD] =
    {
        .categoryName = _("Sharp Blade"),
        .height = 5,
        .weight = 102,
        .description = gPawniardPokedexText,
    },

    [NATIONAL_DEX_BISHARP] =
    {
        .categoryName = _("Sword Blade"),
        .height = 16,
        .weight = 700,
        .description = gBisharpPokedexText,
    },

    [NATIONAL_DEX_BOUFFALANT] =
    {
        .categoryName = _("Bash Buffalo"),
        .height = 16,
        .weight = 946,
        .description = gBouffalantPokedexText,
    },

    [NATIONAL_DEX_RUFFLET] =
    {
        .categoryName = _("Eaglet"),
        .height = 5,
        .weight = 105,
        .description = gRuffletPokedexText,
    },

    [NATIONAL_DEX_BRAVIARY] =
    {
        .categoryName = _("Valiant"),
        .height = 15,
        .weight = 410,
        .description = gBraviaryPokedexText,
    },

    [NATIONAL_DEX_VULLABY] =
    {
        .categoryName = _("Diapered"),
        .height = 5,
        .weight = 90,
        .description = gVullabyPokedexText,
    },

    [NATIONAL_DEX_MANDIBUZZ] =
    {
        .categoryName = _("Bone Vulture"),
        .height = 12,
        .weight = 395,
        .description = gMandibuzzPokedexText,
    },

    [NATIONAL_DEX_HEATMOR] =
    {
        .categoryName = _("Anteater"),
        .height = 14,
        .weight = 580,
        .description = gHeatmorPokedexText,
    },

    [NATIONAL_DEX_DURANT] =
    {
        .categoryName = _("Iron Ant"),
        .height = 3,
        .weight = 330,
        .description = gDurantPokedexText,
    },

    [NATIONAL_DEX_DEINO] =
    {
        .categoryName = _("Irate"),
        .height = 8,
        .weight = 173,
        .description = gDeinoPokedexText,
    },

    [NATIONAL_DEX_ZWEILOUS] =
    {
        .categoryName = _("Hostile"),
        .height = 14,
        .weight = 500,
        .description = gZweilousPokedexText,
    },

    [NATIONAL_DEX_HYDREIGON] =
    {
        .categoryName = _("Brutal"),
        .height = 18,
        .weight = 1600,
        .description = gHydreigonPokedexText,
    },

    [NATIONAL_DEX_LARVESTA] =
    {
        .categoryName = _("Torch"),
        .height = 11,
        .weight = 288,
        .description = gLarvestaPokedexText,
    },

    [NATIONAL_DEX_VOLCARONA] =
    {
        .categoryName = _("Sun"),
        .height = 16,
        .weight = 460,
        .description = gVolcaronaPokedexText,
    },

    [NATIONAL_DEX_COBALION] =
    {
        .categoryName = _("Iron Will"),
        .height = 21,
        .weight = 2500,
        .description = gCobalionPokedexText,
    },

    [NATIONAL_DEX_TERRAKION] =
    {
        .categoryName = _("Cavern"),
        .height = 19,
        .weight = 2600,
        .description = gTerrakionPokedexText,
    },

    [NATIONAL_DEX_VIRIZION] =
    {
        .categoryName = _("Grassland"),
        .height = 20,
        .weight = 2000,
        .description = gVirizionPokedexText,
    },

    [NATIONAL_DEX_TORNADUS] =
    {
        .categoryName = _("Cyclone"),
        .height = 15,
        .weight = 630,
        .description = gTornadusPokedexText,
    },

    [NATIONAL_DEX_THUNDURUS] =
    {
        .categoryName = _("Bolt Strike"),
        .height = 15,
        .weight = 610,
        .description = gThundurusPokedexText,
    },

    [NATIONAL_DEX_RESHIRAM] =
    {
        .categoryName = _("Vast White"),
        .height = 32,
        .weight = 3300,
        .description = gReshiramPokedexText,
    },

    [NATIONAL_DEX_ZEKROM] =
    {
        .categoryName = _("Deep Black"),
        .height = 29,
        .weight = 3450,
        .description = gZekromPokedexText,
    },

    [NATIONAL_DEX_LANDORUS] =
    {
        .categoryName = _("Abundance"),
        .height = 15,
        .weight = 680,
        .description = gLandorusPokedexText,
    },

    [NATIONAL_DEX_KYUREM] =
    {
        .categoryName = _("Boundary"),
        .height = 30,
        .weight = 3250,
        .description = gKyuremPokedexText,
    },

    [NATIONAL_DEX_KELDEO] =
    {
        .categoryName = _("Colt"),
        .height = 14,
        .weight = 485,
        .description = gKeldeoPokedexText,
    },

    [NATIONAL_DEX_MELOETTA] =
    {
        .categoryName = _("Melody"),
        .height = 6,
        .weight = 65,
        .description = gMeloettaPokedexText,
    },

    [NATIONAL_DEX_GENESECT] =
    {
        .categoryName = _("Paleozoic"),
        .height = 15,
        .weight = 825,
        .description = gGenesectPokedexText,
    },

    [NATIONAL_DEX_CHESPIN] =
    {
        .categoryName = _("Spiny Nut"),
        .height = 4,
        .weight = 90,
        .description = gChespinPokedexText,
    },

    [NATIONAL_DEX_QUILLADIN] =
    {
        .categoryName = _("Spiny Armor"),
        .height = 7,
        .weight = 290,
        .description = gQuilladinPokedexText,
    },

    [NATIONAL_DEX_CHESNAUGHT] =
    {
        .categoryName = _("Spiny Armor"),
        .height = 16,
        .weight = 900,
        .description = gChesnaughtPokedexText,
    },

    [NATIONAL_DEX_FENNEKIN] =
    {
        .categoryName = _("Fox"),
        .height = 4,
        .weight = 94,
        .description = gFennekinPokedexText,
    },

    [NATIONAL_DEX_BRAIXEN] =
    {
        .categoryName = _("Fox"),
        .height = 10,
        .weight = 145,
        .description = gBraixenPokedexText,
    },

    [NATIONAL_DEX_DELPHOX] =
    {
        .categoryName = _("Fox"),
        .height = 15,
        .weight = 390,
        .description = gDelphoxPokedexText,
    },

    [NATIONAL_DEX_FROAKIE] =
    {
        .categoryName = _("Bubble Frog"),
        .height = 3,
        .weight = 70,
        .description = gFroakiePokedexText,
    },

    [NATIONAL_DEX_FROGADIER] =
    {
        .categoryName = _("Bubble Frog"),
        .height = 6,
        .weight = 109,
        .description = gFrogadierPokedexText,
    },

    [NATIONAL_DEX_GRENINJA] =
    {
        .categoryName = _("Ninja"),
        .height = 15,
        .weight = 400,
        .description = gGreninjaPokedexText,
    },

    [NATIONAL_DEX_BUNNELBY] =
    {
        .categoryName = _("Digging"),
        .height = 4,
        .weight = 50,
        .description = gBunnelbyPokedexText,
    },

    [NATIONAL_DEX_DIGGERSBY] =
    {
        .categoryName = _("Digging"),
        .height = 10,
        .weight = 424,
        .description = gDiggersbyPokedexText,
    },

    [NATIONAL_DEX_FLETCHLING] =
    {
        .categoryName = _("Tiny Robin"),
        .height = 3,
        .weight = 17,
        .description = gFletchlingPokedexText,
    },

    [NATIONAL_DEX_FLETCHINDER] =
    {
        .categoryName = _("Ember"),
        .height = 7,
        .weight = 160,
        .description = gFletchinderPokedexText,
    },

    [NATIONAL_DEX_TALONFLAME] =
    {
        .categoryName = _("Scorching"),
        .height = 12,
        .weight = 245,
        .description = gTalonflamePokedexText,
    },

    [NATIONAL_DEX_SCATTERBUG] =
    {
        .categoryName = _("Scatterdust"),
        .height = 3,
        .weight = 25,
        .description = gScatterbugPokedexText,
    },

    [NATIONAL_DEX_SPEWPA] =
    {
        .categoryName = _("Scatterdust"),
        .height = 3,
        .weight = 84,
        .description = gSpewpaPokedexText,
    },

    [NATIONAL_DEX_VIVILLON] =
    {
        .categoryName = _("Scale"),
        .height = 12,
        .weight = 170,
        .description = gVivillonPokedexText,
    },

    [NATIONAL_DEX_LITLEO] =
    {
        .categoryName = _("Lion Cub"),
        .height = 6,
        .weight = 135,
        .description = gLitleoPokedexText,
    },

    [NATIONAL_DEX_PYROAR] =
    {
        .categoryName = _("Royal"),
        .height = 15,
        .weight = 815,
        .description = gPyroarPokedexText,
    },

    [NATIONAL_DEX_FLABEBE] =
    {
        .categoryName = _("Single Bloom"),
        .height = 1,
        .weight = 1,
        .description = gFlabebePokedexText,
    },

    [NATIONAL_DEX_FLOETTE] =
    {
        .categoryName = _("Single Bloom"),
        .height = 2,
        .weight = 9,
        .description = gFloettePokedexText,
    },

    [NATIONAL_DEX_FLORGES] =
    {
        .categoryName = _("Garden"),
        .height = 11,
        .weight = 100,
        .description = gFlorgesPokedexText,
    },

    [NATIONAL_DEX_SKIDDO] =
    {
        .categoryName = _("Mount"),
        .height = 9,
        .weight = 310,
        .description = gSkiddoPokedexText,
    },

    [NATIONAL_DEX_GOGOAT] =
    {
        .categoryName = _("Mount"),
        .height = 17,
        .weight = 910,
        .description = gGogoatPokedexText,
    },

    [NATIONAL_DEX_PANCHAM] =
    {
        .categoryName = _("Playful"),
        .height = 6,
        .weight = 80,
        .description = gPanchamPokedexText,
    },

    [NATIONAL_DEX_PANGORO] =
    {
        .categoryName = _("Daunting"),
        .height = 21,
        .weight = 1360,
        .description = gPangoroPokedexText,
    },

    [NATIONAL_DEX_FURFROU] =
    {
        .categoryName = _("Poodle"),
        .height = 12,
        .weight = 280,
        .description = gFurfrouPokedexText,
    },

    [NATIONAL_DEX_ESPURR] =
    {
        .categoryName = _("Restraint"),
        .height = 3,
        .weight = 35,
        .description = gEspurrPokedexText,
    },

    [NATIONAL_DEX_MEOWSTIC] =
    {
        .categoryName = _("Constraint"),
        .height = 6,
        .weight = 85,
        .description = gMeowsticPokedexText,
    },

    [NATIONAL_DEX_HONEDGE] =
    {
        .categoryName = _("Sword"),
        .height = 8,
        .weight = 20,
        .description = gHonedgePokedexText,
    },

    [NATIONAL_DEX_DOUBLADE] =
    {
        .categoryName = _("Sword"),
        .height = 8,
        .weight = 45,
        .description = gDoubladePokedexText,
    },

    [NATIONAL_DEX_AEGISLASH] =
    {
        .categoryName = _("Royal Sword"),
        .height = 17,
        .weight = 530,
        .description = gAegislashPokedexText,
    },

    [NATIONAL_DEX_SPRITZEE] =
    {
        .categoryName = _("Perfume"),
        .height = 2,
        .weight = 5,
        .description = gSpritzeePokedexText,
    },

    [NATIONAL_DEX_AROMATISSE] =
    {
        .categoryName = _("Fragrance"),
        .height = 8,
        .weight = 155,
        .description = gAromatissePokedexText,
    },

    [NATIONAL_DEX_SWIRLIX] =
    {
        .categoryName = _("Cotton Candy"),
        .height = 4,
        .weight = 35,
        .description = gSwirlixPokedexText,
    },

    [NATIONAL_DEX_SLURPUFF] =
    {
        .categoryName = _("Meringue"),
        .height = 8,
        .weight = 50,
        .description = gSlurpuffPokedexText,
    },

    [NATIONAL_DEX_INKAY] =
    {
        .categoryName = _("Revolving"),
        .height = 4,
        .weight = 35,
        .description = gInkayPokedexText,
    },

    [NATIONAL_DEX_MALAMAR] =
    {
        .categoryName = _("Overturning"),
        .height = 15,
        .weight = 470,
        .description = gMalamarPokedexText,
    },

    [NATIONAL_DEX_BINACLE] =
    {
        .categoryName = _("Two-Handed"),
        .height = 5,
        .weight = 310,
        .description = gBinaclePokedexText,
    },

    [NATIONAL_DEX_BARBARACLE] =
    {
        .categoryName = _("Collective"),
        .height = 13,
        .weight = 960,
        .description = gBarbaraclePokedexText,
    },

    [NATIONAL_DEX_SKRELP] =
    {
        .categoryName = _("Mock Kelp"),
        .height = 5,
        .weight = 73,
        .description = gSkrelpPokedexText,
    },

    [NATIONAL_DEX_DRAGALGE] =
    {
        .categoryName = _("Mock Kelp"),
        .height = 18,
        .weight = 815,
        .description = gDragalgePokedexText,
    },

    [NATIONAL_DEX_CLAUNCHER] =
    {
        .categoryName = _("Water Gun"),
        .height = 5,
        .weight = 83,
        .description = gClauncherPokedexText,
    },

    [NATIONAL_DEX_CLAWITZER] =
    {
        .categoryName = _("Howitzer"),
        .height = 13,
        .weight = 353,
        .description = gClawitzerPokedexText,
    },

    [NATIONAL_DEX_HELIOPTILE] =
    {
        .categoryName = _("Generator"),
        .height = 5,
        .weight = 60,
        .description = gHelioptilePokedexText,
    },

    [NATIONAL_DEX_HELIOLISK] =
    {
        .categoryName = _("Generator"),
        .height = 10,
        .weight = 210,
        .description = gHelioliskPokedexText,
    },

    [NATIONAL_DEX_TYRUNT] =
    {
        .categoryName = _("Royal Heir"),
        .height = 8,
        .weight = 260,
        .description = gTyruntPokedexText,
    },

    [NATIONAL_DEX_TYRANTRUM] =
    {
        .categoryName = _("Despot"),
        .height = 25,
        .weight = 2700,
        .description = gTyrantrumPokedexText,
    },

    [NATIONAL_DEX_AMAURA] =
    {
        .categoryName = _("Tundra"),
        .height = 13,
        .weight = 252,
        .description = gAmauraPokedexText,
    },

    [NATIONAL_DEX_AURORUS] =
    {
        .categoryName = _("Tundra"),
        .height = 27,
        .weight = 2250,
        .description = gAurorusPokedexText,
    },

    [NATIONAL_DEX_SYLVEON] =
    {
        .categoryName = _("Intertwine"),
        .height = 10,
        .weight = 235,
        .description = gSylveonPokedexText,
    },

    [NATIONAL_DEX_HAWLUCHA] =
    {
        .categoryName = _("Wrestling"),
        .height = 8,
        .weight = 215,
        .description = gHawluchaPokedexText,
    },

    [NATIONAL_DEX_DEDENNE] =
    {
        .categoryName = _("Antenna"),
        .height = 2,
        .weight = 22,
        .description = gDedennePokedexText,
    },

    [NATIONAL_DEX_CARBINK] =
    {
        .categoryName = _("Jewel"),
        .height = 3,
        .weight = 57,
        .description = gCarbinkPokedexText,
    },

    [NATIONAL_DEX_GOOMY] =
    {
        .categoryName = _("Soft Tissue"),
        .height = 3,
        .weight = 28,
        .description = gGoomyPokedexText,
    },

    [NATIONAL_DEX_SLIGGOO] =
    {
        .categoryName = _("Soft Tissue"),
        .height = 8,
        .weight = 175,
        .description = gSliggooPokedexText,
    },

    [NATIONAL_DEX_GOODRA] =
    {
        .categoryName = _("Dragon"),
        .height = 20,
        .weight = 1505,
        .description = gGoodraPokedexText,
    },

    [NATIONAL_DEX_KLEFKI] =
    {
        .categoryName = _("Key Ring"),
        .height = 2,
        .weight = 30,
        .description = gKlefkiPokedexText,
    },

    [NATIONAL_DEX_PHANTUMP] =
    {
        .categoryName = _("Stump"),
        .height = 4,
        .weight = 70,
        .description = gPhantumpPokedexText,
    },

    [NATIONAL_DEX_TREVENANT] =
    {
        .categoryName = _("Elder Tree"),
        .height = 15,
        .weight = 710,
        .description = gTrevenantPokedexText,
    },

    [NATIONAL_DEX_PUMPKABOO] =
    {
        .categoryName = _("Pumpkin"),
        .height = 4,
        .weight = 50,
        .description = gPumpkabooPokedexText,
    },

    [NATIONAL_DEX_GOURGEIST] =
    {
        .categoryName = _("Pumpkin"),
        .height = 9,
        .weight = 125,
        .description = gGourgeistPokedexText,
    },

    [NATIONAL_DEX_BERGMITE] =
    {
        .categoryName = _("Ice Chunk"),
        .height = 10,
        .weight = 995,
        .description = gBergmitePokedexText,
    },

    [NATIONAL_DEX_AVALUGG] =
    {
        .categoryName = _("Iceberg"),
        .height = 20,
        .weight = 5050,
        .description = gAvaluggPokedexText,
    },

    [NATIONAL_DEX_NOIBAT] =
    {
        .categoryName = _("Sound Wave"),
        .height = 5,
        .weight = 80,
        .description = gNoibatPokedexText,
    },

    [NATIONAL_DEX_NOIVERN] =
    {
        .categoryName = _("Sound Wave"),
        .height = 15,
        .weight = 850,
        .description = gNoivernPokedexText,
    },

    [NATIONAL_DEX_XERNEAS] =
    {
        .categoryName = _("Life"),
        .height = 30,
        .weight = 2150,
        .description = gXerneasPokedexText,
    },

    [NATIONAL_DEX_YVELTAL] =
    {
        .categoryName = _("Destruction"),
        .height = 58,
        .weight = 2030,
        .description = gYveltalPokedexText,
    },

    [NATIONAL_DEX_ZYGARDE] =
    {
        .categoryName = _("Order"),
        .height = 50,
        .weight = 3050,
        .description = gZygardePokedexText,
    },

    [NATIONAL_DEX_DIANCIE] =
    {
        .categoryName = _("Jewel"),
        .height = 7,
        .weight = 88,
        .description = gDianciePokedexText,
    },

    [NATIONAL_DEX_HOOPA] =
    {
        .categoryName = _("Mischief"),
        .height = 5,
        .weight = 90,
        .description = gHoopaPokedexText,
    },

    [NATIONAL_DEX_VOLCANION] =
    {
        .categoryName = _("Steam"),
        .height = 17,
        .weight = 1950,
        .description = gVolcanionPokedexText,
    },

    [NATIONAL_DEX_ROWLET] =
    {
        .categoryName = _("Grass Quill"),
        .height = 3,
        .weight = 15,
        .description = gRowletPokedexText,
    },

    [NATIONAL_DEX_DARTRIX] =
    {
        .categoryName = _("Blade Quill"),
        .height = 7,
        .weight = 160,
        .description = gDartrixPokedexText,
    },

    [NATIONAL_DEX_DECIDUEYE] =
    {
        .categoryName = _("Arrow Quill"),
        .height = 16,
        .weight = 366,
        .description = gDecidueyePokedexText,
    },

    [NATIONAL_DEX_LITTEN] =
    {
        .categoryName = _("Fire Cat"),
        .height = 4,
        .weight = 43,
        .description = gLittenPokedexText,
    },

    [NATIONAL_DEX_TORRACAT] =
    {
        .categoryName = _("Fire Cat"),
        .height = 7,
        .weight = 250,
        .description = gTorracatPokedexText,
    },

    [NATIONAL_DEX_INCINEROAR] =
    {
        .categoryName = _("Heel"),
        .height = 18,
        .weight = 830,
        .description = gIncineroarPokedexText,
    },

    [NATIONAL_DEX_POPPLIO] =
    {
        .categoryName = _("Sea Lion"),
        .height = 4,
        .weight = 75,
        .description = gPopplioPokedexText,
    },

    [NATIONAL_DEX_BRIONNE] =
    {
        .categoryName = _("Pop Star"),
        .height = 6,
        .weight = 175,
        .description = gBrionnePokedexText,
    },

    [NATIONAL_DEX_PRIMARINA] =
    {
        .categoryName = _("Soloist"),
        .height = 18,
        .weight = 440,
        .description = gPrimarinaPokedexText,
    },

    [NATIONAL_DEX_PIKIPEK] =
    {
        .categoryName = _("Woodpecker"),
        .height = 3,
        .weight = 12,
        .description = gPikipekPokedexText,
    },

    [NATIONAL_DEX_TRUMBEAK] =
    {
        .categoryName = _("Bugle Beak"),
        .height = 6,
        .weight = 148,
        .description = gTrumbeakPokedexText,
    },

    [NATIONAL_DEX_TOUCANNON] =
    {
        .categoryName = _("Cannon"),
        .height = 11,
        .weight = 260,
        .description = gToucannonPokedexText,
    },

    [NATIONAL_DEX_YUNGOOS] =
    {
        .categoryName = _("Loitering"),
        .height = 4,
        .weight = 60,
        .description = gYungoosPokedexText,
    },

    [NATIONAL_DEX_GUMSHOOS] =
    {
        .categoryName = _("Stakeout"),
        .height = 7,
        .weight = 142,
        .description = gGumshoosPokedexText,
    },

    [NATIONAL_DEX_GRUBBIN] =
    {
        .categoryName = _("Larva"),
        .height = 4,
        .weight = 44,
        .description = gGrubbinPokedexText,
    },

    [NATIONAL_DEX_CHARJABUG] =
    {
        .categoryName = _("Battery"),
        .height = 5,
        .weight = 105,
        .description = gCharjabugPokedexText,
    },

    [NATIONAL_DEX_VIKAVOLT] =
    {
        .categoryName = _("Stag Beetle"),
        .height = 15,
        .weight = 450,
        .description = gVikavoltPokedexText,
    },

    [NATIONAL_DEX_CRABRAWLER] =
    {
        .categoryName = _("Boxing"),
        .height = 6,
        .weight = 70,
        .description = gCrabrawlerPokedexText,
    },

    [NATIONAL_DEX_CRABOMINABLE] =
    {
        .categoryName = _("Woolly Crab"),
        .height = 17,
        .weight = 1800,
        .description = gCrabominablePokedexText,
    },

    [NATIONAL_DEX_ORICORIO] =
    {
        .categoryName = _("Dancing"),
        .height = 6,
        .weight = 34,
        .description = gOricorioPokedexText,
    },

    [NATIONAL_DEX_CUTIEFLY] =
    {
        .categoryName = _("Bee Fly"),
        .height = 1,
        .weight = 2,
        .description = gCutieflyPokedexText,
    },

    [NATIONAL_DEX_RIBOMBEE] =
    {
        .categoryName = _("Bee Fly"),
        .height = 2,
        .weight = 5,
        .description = gRibombeePokedexText,
    },

    [NATIONAL_DEX_ROCKRUFF] =
    {
        .categoryName = _("Puppy"),
        .height = 5,
        .weight = 92,
        .description = gRockruffPokedexText,
    },

    [NATIONAL_DEX_LYCANROC] =
    {
        .categoryName = _("Wolf"),
        .height = 8,
        .weight = 250,
        .description = gLycanrocPokedexText,
    },

    [NATIONAL_DEX_WISHIWASHI] =
    {
        .categoryName = _("Small Fry"),
        .height = 2,
        .weight = 3,
        .description = gWishiwashiPokedexText,
    },

    [NATIONAL_DEX_MAREANIE] =
    {
        .categoryName = _("Brutal Star"),
        .height = 4,
        .weight = 80,
        .description = gMareaniePokedexText,
    },

    [NATIONAL_DEX_TOXAPEX] =
    {
        .categoryName = _("Brutal Star"),
        .height = 7,
        .weight = 145,
        .description = gToxapexPokedexText,
    },

    [NATIONAL_DEX_MUDBRAY] =
    {
        .categoryName = _("Donkey"),
        .height = 10,
        .weight = 1100,
        .description = gMudbrayPokedexText,
    },

    [NATIONAL_DEX_MUDSDALE] =
    {
        .categoryName = _("Draft Horse"),
        .height = 25,
        .weight = 9200,
        .description = gMudsdalePokedexText,
    },

    [NATIONAL_DEX_DEWPIDER] =
    {
        .categoryName = _("Water Bubble"),
        .height = 3,
        .weight = 40,
        .description = gDewpiderPokedexText,
    },

    [NATIONAL_DEX_ARAQUANID] =
    {
        .categoryName = _("Water Bubble"),
        .height = 18,
        .weight = 820,
        .description = gAraquanidPokedexText,
    },

    [NATIONAL_DEX_FOMANTIS] =
    {
        .categoryName = _("Sickle Grass"),
        .height = 3,
        .weight = 15,
        .description = gFomantisPokedexText,
    },

    [NATIONAL_DEX_LURANTIS] =
    {
        .categoryName = _("Bloom Sickle"),
        .height = 9,
        .weight = 185,
        .description = gLurantisPokedexText,
    },

    [NATIONAL_DEX_MORELULL] =
    {
        .categoryName = _("Illuminate"),
        .height = 2,
        .weight = 15,
        .description = gMorelullPokedexText,
    },

    [NATIONAL_DEX_SHIINOTIC] =
    {
        .categoryName = _("Illuminate"),
        .height = 10,
        .weight = 115,
        .description = gShiinoticPokedexText,
    },

    [NATIONAL_DEX_SALANDIT] =
    {
        .categoryName = _("Toxic Lizard"),
        .height = 6,
        .weight = 48,
        .description = gSalanditPokedexText,
    },

    [NATIONAL_DEX_SALAZZLE] =
    {
        .categoryName = _("Toxic Lizard"),
        .height = 12,
        .weight = 222,
        .description = gSalazzlePokedexText,
    },

    [NATIONAL_DEX_STUFFUL] =
    {
        .categoryName = _("Flailing"),
        .height = 5,
        .weight = 68,
        .description = gStuffulPokedexText,
    },

    [NATIONAL_DEX_BEWEAR] =
    {
        .categoryName = _("Strong Arm"),
        .height = 21,
        .weight = 1350,
        .description = gBewearPokedexText,
    },

    [NATIONAL_DEX_BOUNSWEET] =
    {
        .categoryName = _("Fruit"),
        .height = 3,
        .weight = 32,
        .description = gBounsweetPokedexText,
    },

    [NATIONAL_DEX_STEENEE] =
    {
        .categoryName = _("Fruit"),
        .height = 7,
        .weight = 82,
        .description = gSteeneePokedexText,
    },

    [NATIONAL_DEX_TSAREENA] =
    {
        .categoryName = _("Fruit"),
        .height = 12,
        .weight = 214,
        .description = gTsareenaPokedexText,
    },

    [NATIONAL_DEX_COMFEY] =
    {
        .categoryName = _("Posy Picker"),
        .height = 1,
        .weight = 3,
        .description = gComfeyPokedexText,
    },

    [NATIONAL_DEX_ORANGURU] =
    {
        .categoryName = _("Sage"),
        .height = 15,
        .weight = 760,
        .description = gOranguruPokedexText,
    },

    [NATIONAL_DEX_PASSIMIAN] =
    {
        .categoryName = _("Teamwork"),
        .height = 20,
        .weight = 828,
        .description = gPassimianPokedexText,
    },

    [NATIONAL_DEX_WIMPOD] =
    {
        .categoryName = _("Turn Tail"),
        .height = 5,
        .weight = 120,
        .description = gWimpodPokedexText,
    },

    [NATIONAL_DEX_GOLISOPOD] =
    {
        .categoryName = _("Hard Scale"),
        .height = 20,
        .weight = 1080,
        .description = gGolisopodPokedexText,
    },

    [NATIONAL_DEX_SANDYGAST] =
    {
        .categoryName = _("Sand Heap"),
        .height = 5,
        .weight = 700,
        .description = gSandygastPokedexText,
    },

    [NATIONAL_DEX_PALOSSAND] =
    {
        .categoryName = _("Sand Castle"),
        .height = 13,
        .weight = 2500,
        .description = gPalossandPokedexText,
    },

    [NATIONAL_DEX_PYUKUMUKU] =
    {
        .categoryName = _("Sea Cucumber"),
        .height = 3,
        .weight = 12,
        .description = gPyukumukuPokedexText,
    },

    [NATIONAL_DEX_TYPE_NULL] =
    {
        .categoryName = _("Synthetic"),
        .height = 19,
        .weight = 1205,
        .description = gTypeNullPokedexText,
    },

    [NATIONAL_DEX_SILVALLY] =
    {
        .categoryName = _("Synthetic"),
        .height = 23,
        .weight = 1005,
        .description = gSilvallyPokedexText,
    },

    [NATIONAL_DEX_MINIOR] =
    {
        .categoryName = _("Meteor"),
        .height = 3,
        .weight = 400,
        .description = gMiniorPokedexText,
    },

    [NATIONAL_DEX_KOMALA] =
    {
        .categoryName = _("Drowsing"),
        .height = 4,
        .weight = 199,
        .description = gKomalaPokedexText,
    },

    [NATIONAL_DEX_TURTONATOR] =
    {
        .categoryName = _("Blast Turtle"),
        .height = 20,
        .weight = 2120,
        .description = gTurtonatorPokedexText,
    },

    [NATIONAL_DEX_TOGEDEMARU] =
    {
        .categoryName = _("Roly-Poly"),
        .height = 3,
        .weight = 33,
        .description = gTogedemaruPokedexText,
    },

    [NATIONAL_DEX_MIMIKYU] =
    {
        .categoryName = _("Disguise"),
        .height = 2,
        .weight = 7,
        .description = gMimikyuPokedexText,
    },

    [NATIONAL_DEX_BRUXISH] =
    {
        .categoryName = _("Gnash Teeth"),
        .height = 9,
        .weight = 190,
        .description = gBruxishPokedexText,
    },

    [NATIONAL_DEX_DRAMPA] =
    {
        .categoryName = _("Placid"),
        .height = 30,
        .weight = 1850,
        .description = gDrampaPokedexText,
    },

    [NATIONAL_DEX_DHELMISE] =
    {
        .categoryName = _("Sea Creeper"),
        .height = 39,
        .weight = 2100,
        .description = gDhelmisePokedexText,
    },

    [NATIONAL_DEX_JANGMO_O] =
    {
        .categoryName = _("Scaly"),
        .height = 6,
        .weight = 297,
        .description = gJangmooPokedexText,
    },

    [NATIONAL_DEX_HAKAMO_O] =
    {
        .categoryName = _("Scaly"),
        .height = 12,
        .weight = 470,
        .description = gHakamooPokedexText,
    },

    [NATIONAL_DEX_KOMMO_O] =
    {
        .categoryName = _("Scaly"),
        .height = 16,
        .weight = 782,
        .description = gKommooPokedexText,
    },

    [NATIONAL_DEX_TAPU_KOKO] =
    {
        .categoryName = _("Land Spirit"),
        .height = 18,
        .weight = 205,
        .description = gTapuKokoPokedexText,
    },

    [NATIONAL_DEX_TAPU_LELE] =
    {
        .categoryName = _("Land Spirit"),
        .height = 12,
        .weight = 186,
        .description = gTapuLelePokedexText,
    },

    [NATIONAL_DEX_TAPU_BULU] =
    {
        .categoryName = _("Land Spirit"),
        .height = 19,
        .weight = 455,
        .description = gTapuBuluPokedexText,
    },

    [NATIONAL_DEX_TAPU_FINI] =
    {
        .categoryName = _("Land Spirit"),
        .height = 13,
        .weight = 212,
        .description = gTapuFiniPokedexText,
    },

    [NATIONAL_DEX_COSMOG] =
    {
        .categoryName = _("Nebula"),
        .height = 2,
        .weight = 1,
        .description = gCosmogPokedexText,
    },

    [NATIONAL_DEX_COSMOEM] =
    {
        .categoryName = _("Protostar"),
        .height = 1,
        .weight = 9999,
        .description = gCosmoemPokedexText,
    },

    [NATIONAL_DEX_SOLGALEO] =
    {
        .categoryName = _("Sunne"),
        .height = 34,
        .weight = 2300,
        .description = gSolgaleoPokedexText,
    },

    [NATIONAL_DEX_LUNALA] =
    {
        .categoryName = _("Moone"),
        .height = 40,
        .weight = 1200,
        .description = gLunalaPokedexText,
    },

    [NATIONAL_DEX_NIHILEGO] =
    {
        .categoryName = _("Parasite"),
        .height = 12,
        .weight = 555,
        .description = gNihilegoPokedexText,
    },

    [NATIONAL_DEX_BUZZWOLE] =
    {
        .categoryName = _("Swollen"),
        .height = 24,
        .weight = 3336,
        .description = gBuzzwolePokedexText,
    },

    [NATIONAL_DEX_PHEROMOSA] =
    {
        .categoryName = _("Lissome"),
        .height = 18,
        .weight = 250,
        .description = gPheromosaPokedexText,
    },

    [NATIONAL_DEX_XURKITREE] =
    {
        .categoryName = _("Glowing"),
        .height = 38,
        .weight = 1000,
        .description = gXurkitreePokedexText,
    },

    [NATIONAL_DEX_CELESTEELA] =
    {
        .categoryName = _("Launch"),
        .height = 92,
        .weight = 9999,
        .description = gCelesteelaPokedexText,
    },

    [NATIONAL_DEX_KARTANA] =
    {
        .categoryName = _("Drawn Sword"),
        .height = 3,
        .weight = 1,
        .description = gKartanaPokedexText,
    },

    [NATIONAL_DEX_GUZZLORD] =
    {
        .categoryName = _("Junkivore"),
        .height = 55,
        .weight = 8880,
        .description = gGuzzlordPokedexText,
    },

    [NATIONAL_DEX_NECROZMA] =
    {
        .categoryName = _("Prism"),
        .height = 24,
        .weight = 2300,
        .description = gNecrozmaPokedexText,
    },

    [NATIONAL_DEX_MAGEARNA] =
    {
        .categoryName = _("Artificial"),
        .height = 10,
        .weight = 805,
        .description = gMagearnaPokedexText,
    },

    [NATIONAL_DEX_MARSHADOW] =
    {
        .categoryName = _("Gloomdwellr"),
        .height = 7,
        .weight = 222,
        .description = gMarshadowPokedexText,
    },

    [NATIONAL_DEX_POIPOLE] =
    {
        .categoryName = _("Poison Pin"),
        .height = 6,
        .weight = 18,
        .description = gPoipolePokedexText,
    },

    [NATIONAL_DEX_NAGANADEL] =
    {
        .categoryName = _("Poison Pin"),
        .height = 36,
        .weight = 1500,
        .description = gNaganadelPokedexText,
    },

    [NATIONAL_DEX_STAKATAKA] =
    {
        .categoryName = _("Rampart"),
        .height = 55,
        .weight = 8200,
        .description = gStakatakaPokedexText,
    },

    [NATIONAL_DEX_BLACEPHALON] =
    {
        .categoryName = _("Fireworks"),
        .height = 18,
        .weight = 130,
        .description = gBlacephalonPokedexText,
    },

    [NATIONAL_DEX_ZERAORA] =
    {
        .categoryName = _("Thunderclap"),
        .height = 15,
        .weight = 445,
        .description = gZeraoraPokedexText,
    },

    [NATIONAL_DEX_MELTAN] =
    {
        .categoryName = _("Hex Nut"),
        .height = 2,
        .weight = 80,
        .description = gMeltanPokedexText,
    },

    [NATIONAL_DEX_MELMETAL] =
    {
        .categoryName = _("Hex Nut"),
        .height = 25,
        .weight = 800,
        .description = gMelmetalPokedexText,
    },

    [NATIONAL_DEX_GROOKEY] =
    {
        .categoryName = _("Chimp"),
        .height = 3,
        .weight = 50,
        .description = gGrookeyPokedexText,
    },

    [NATIONAL_DEX_THWACKEY] =
    {
        .categoryName = _("Beat"),
        .height = 7,
        .weight = 140,
        .description = gThwackeyPokedexText,
    },

    [NATIONAL_DEX_RILLABOOM] =
    {
        .categoryName = _("Drummer"),
        .height = 21,
        .weight = 900,
        .description = gRillaboomPokedexText,
    },

    [NATIONAL_DEX_SCORBUNNY] =
    {
        .categoryName = _("Rabbit"),
        .height = 3,
        .weight = 45,
        .description = gScorbunnyPokedexText,
    },

    [NATIONAL_DEX_RABOOT] =
    {
        .categoryName = _("Rabbit"),
        .height = 6,
        .weight = 90,
        .description = gRabootPokedexText,
    },

    [NATIONAL_DEX_CINDERACE] =
    {
        .categoryName = _("Striker"),
        .height = 14,
        .weight = 330,
        .description = gCinderacePokedexText,
    },

    [NATIONAL_DEX_SOBBLE] =
    {
        .categoryName = _("Water Lizard"),
        .height = 3,
        .weight = 40,
        .description = gSobblePokedexText,
    },

    [NATIONAL_DEX_DRIZZILE] =
    {
        .categoryName = _("Water Lizard"),
        .height = 7,
        .weight = 115,
        .description = gDrizzilePokedexText,
    },

    [NATIONAL_DEX_INTELEON] =
    {
        .categoryName = _("Secret Agent"),
        .height = 19,
        .weight = 452,
        .description = gInteleonPokedexText,
    },

    [NATIONAL_DEX_SKWOVET] =
    {
        .categoryName = _("Cheeky"),
        .height = 3,
        .weight = 25,
        .description = gSkwovetPokedexText,
    },

    [NATIONAL_DEX_GREEDENT] =
    {
        .categoryName = _("Greedy"),
        .height = 6,
        .weight = 60,
        .description = gGreedentPokedexText,
    },

    [NATIONAL_DEX_ROOKIDEE] =
    {
        .categoryName = _("Tiny Bird"),
        .height = 2,
        .weight = 18,
        .description = gRookideePokedexText,
    },

    [NATIONAL_DEX_CORVISQUIRE] =
    {
        .categoryName = _("Raven"),
        .height = 8,
        .weight = 160,
        .description = gCorvisquirePokedexText,
    },

    [NATIONAL_DEX_CORVIKNIGHT] =
    {
        .categoryName = _("Raven"),
        .height = 22,
        .weight = 750,
        .description = gCorviknightPokedexText,
    },

    [NATIONAL_DEX_BLIPBUG] =
    {
        .categoryName = _("Larva"),
        .height = 4,
        .weight = 80,
        .description = gBlipbugPokedexText,
    },

    [NATIONAL_DEX_DOTTLER] =
    {
        .categoryName = _("Radome"),
        .height = 4,
        .weight = 195,
        .description = gDottlerPokedexText,
    },

    [NATIONAL_DEX_ORBEETLE] =
    {
        .categoryName = _("Seven Spot"),
        .height = 4,
        .weight = 408,
        .description = gOrbeetlePokedexText,
    },

    [NATIONAL_DEX_NICKIT] =
    {
        .categoryName = _("Fox"),
        .height = 6,
        .weight = 89,
        .description = gNickitPokedexText,
    },

    [NATIONAL_DEX_THIEVUL] =
    {
        .categoryName = _("Fox"),
        .height = 12,
        .weight = 199,
        .description = gThievulPokedexText,
    },

    [NATIONAL_DEX_GOSSIFLEUR] =
    {
        .categoryName = _("Flowering"),
        .height = 4,
        .weight = 22,
        .description = gGossifleurPokedexText,
    },

    [NATIONAL_DEX_ELDEGOSS] =
    {
        .categoryName = _("Cotton Bloom"),
        .height = 5,
        .weight = 25,
        .description = gEldegossPokedexText,
    },

    [NATIONAL_DEX_WOOLOO] =
    {
        .categoryName = _("Sheep"),
        .height = 6,
        .weight = 60,
        .description = gWoolooPokedexText,
    },

    [NATIONAL_DEX_DUBWOOL] =
    {
        .categoryName = _("Sheep"),
        .height = 13,
        .weight = 430,
        .description = gDubwoolPokedexText,
    },

    [NATIONAL_DEX_CHEWTLE] =
    {
        .categoryName = _("Snapping"),
        .height = 3,
        .weight = 85,
        .description = gChewtlePokedexText,
    },

    [NATIONAL_DEX_DREDNAW] =
    {
        .categoryName = _("Bite"),
        .height = 10,
        .weight = 1155,
        .description = gDrednawPokedexText,
    },

    [NATIONAL_DEX_YAMPER] =
    {
        .categoryName = _("Puppy"),
        .height = 3,
        .weight = 135,
        .description = gYamperPokedexText,
    },

    [NATIONAL_DEX_BOLTUND] =
    {
        .categoryName = _("Dog"),
        .height = 10,
        .weight = 340,
        .description = gBoltundPokedexText,
    },

    [NATIONAL_DEX_ROLYCOLY] =
    {
        .categoryName = _("Coal"),
        .height = 3,
        .weight = 120,
        .description = gRolycolyPokedexText,
    },

    [NATIONAL_DEX_CARKOL] =
    {
        .categoryName = _("Coal"),
        .height = 11,
        .weight = 780,
        .description = gCarkolPokedexText,
    },

    [NATIONAL_DEX_COALOSSAL] =
    {
        .categoryName = _("Coal"),
        .height = 28,
        .weight = 3105,
        .description = gCoalossalPokedexText,
    },

    [NATIONAL_DEX_APPLIN] =
    {
        .categoryName = _("Apple Core"),
        .height = 2,
        .weight = 5,
        .description = gApplinPokedexText,
    },

    [NATIONAL_DEX_FLAPPLE] =
    {
        .categoryName = _("Apple Wing"),
        .height = 3,
        .weight = 10,
        .description = gFlapplePokedexText,
    },

    [NATIONAL_DEX_APPLETUN] =
    {
        .categoryName = _("Apple Nectar"),
        .height = 4,
        .weight = 130,
        .description = gAppletunPokedexText,
    },

    [NATIONAL_DEX_SILICOBRA] =
    {
        .categoryName = _("Sand Snake"),
        .height = 22,
        .weight = 76,
        .description = gSilicobraPokedexText,
    },

    [NATIONAL_DEX_SANDACONDA] =
    {
        .categoryName = _("Sand Snake"),
        .height = 38,
        .weight = 655,
        .description = gSandacondaPokedexText,
    },

    [NATIONAL_DEX_CRAMORANT] =
    {
        .categoryName = _("Gulp"),
        .height = 8,
        .weight = 180,
        .description = gCramorantPokedexText,
    },

    [NATIONAL_DEX_ARROKUDA] =
    {
        .categoryName = _("Rush"),
        .height = 5,
        .weight = 10,
        .description = gArrokudaPokedexText,
    },

    [NATIONAL_DEX_BARRASKEWDA] =
    {
        .categoryName = _("Skewer"),
        .height = 13,
        .weight = 300,
        .description = gBarraskewdaPokedexText,
    },

    [NATIONAL_DEX_TOXEL] =
    {
        .categoryName = _("Baby"),
        .height = 4,
        .weight = 110,
        .description = gToxelPokedexText,
    },

    [NATIONAL_DEX_TOXTRICITY] =
    {
        .categoryName = _("Punk"),
        .height = 16,
        .weight = 400,
        .description = gToxtricityPokedexText,
    },

    [NATIONAL_DEX_SIZZLIPEDE] =
    {
        .categoryName = _("Radiator"),
        .height = 7,
        .weight = 10,
        .description = gSizzlipedePokedexText,
    },

    [NATIONAL_DEX_CENTISKORCH] =
    {
        .categoryName = _("Radiator"),
        .height = 30,
        .weight = 1200,
        .description = gCentiskorchPokedexText,
    },

    [NATIONAL_DEX_CLOBBOPUS] =
    {
        .categoryName = _("Tantrum"),
        .height = 6,
        .weight = 40,
        .description = gClobbopusPokedexText,
    },

    [NATIONAL_DEX_GRAPPLOCT] =
    {
        .categoryName = _("Jujitsu"),
        .height = 16,
        .weight = 390,
        .description = gGrapploctPokedexText,
    },

    [NATIONAL_DEX_SINISTEA] =
    {
        .categoryName = _("Black Tea"),
        .height = 1,
        .weight = 2,
        .description = gSinisteaPokedexText,
    },

    [NATIONAL_DEX_POLTEAGEIST] =
    {
        .categoryName = _("Black Tea"),
        .height = 2,
        .weight = 4,
        .description = gPolteageistPokedexText,
    },

    [NATIONAL_DEX_HATENNA] =
    {
        .categoryName = _("Calm"),
        .height = 4,
        .weight = 34,
        .description = gHatennaPokedexText,
    },

    [NATIONAL_DEX_HATTREM] =
    {
        .categoryName = _("Serene"),
        .height = 6,
        .weight = 48,
        .description = gHattremPokedexText,
    },

    [NATIONAL_DEX_HATTERENE] =
    {
        .categoryName = _("Silent"),
        .height = 21,
        .weight = 51,
        .description = gHatterenePokedexText,
    },

    [NATIONAL_DEX_IMPIDIMP] =
    {
        .categoryName = _("Wily"),
        .height = 4,
        .weight = 55,
        .description = gImpidimpPokedexText,
    },

    [NATIONAL_DEX_MORGREM] =
    {
        .categoryName = _("Devious"),
        .height = 8,
        .weight = 125,
        .description = gMorgremPokedexText,
    },

    [NATIONAL_DEX_GRIMMSNARL] =
    {
        .categoryName = _("Bulk Up"),
        .height = 15,
        .weight = 610,
        .description = gGrimmsnarlPokedexText,
    },

    [NATIONAL_DEX_OBSTAGOON] =
    {
        .categoryName = _("Blocking"),
        .height = 16,
        .weight = 460,
        .description = gObstagoonPokedexText,
    },

    [NATIONAL_DEX_PERRSERKER] =
    {
        .categoryName = _("Viking"),
        .height = 8,
        .weight = 280,
        .description = gPerrserkerPokedexText,
    },

    [NATIONAL_DEX_CURSOLA] =
    {
        .categoryName = _("Coral"),
        .height = 10,
        .weight = 4,
        .description = gCursolaPokedexText,
    },

    [NATIONAL_DEX_SIRFETCHD] =
    {
        .categoryName = _("Wild Duck"),
        .height = 8,
        .weight = 1170,
        .description = gSirfetchdPokedexText,
    },

    [NATIONAL_DEX_MR_RIME] =
    {
        .categoryName = _("Comedian"),
        .height = 15,
        .weight = 582,
        .description = gMrRimePokedexText,
    },

    [NATIONAL_DEX_RUNERIGUS] =
    {
        .categoryName = _("Grudge"),
        .height = 16,
        .weight = 666,
        .description = gRunerigusPokedexText,
    },

    [NATIONAL_DEX_MILCERY] =
    {
        .categoryName = _("Cream"),
        .height = 2,
        .weight = 3,
        .description = gMilceryPokedexText,
    },

    [NATIONAL_DEX_ALCREMIE] =
    {
        .categoryName = _("Cream"),
        .height = 3,
        .weight = 5,
        .description = gAlcremiePokedexText,
    },

    [NATIONAL_DEX_FALINKS] =
    {
        .categoryName = _("Formation"),
        .height = 30,
        .weight = 620,
        .description = gFalinksPokedexText,
    },

    [NATIONAL_DEX_PINCURCHIN] =
    {
        .categoryName = _("Sea Urchin"),
        .height = 3,
        .weight = 10,
        .description = gPincurchinPokedexText,
    },

    [NATIONAL_DEX_SNOM] =
    {
        .categoryName = _("Worm"),
        .height = 3,
        .weight = 38,
        .description = gSnomPokedexText,
    },

    [NATIONAL_DEX_FROSMOTH] =
    {
        .categoryName = _("Frost Moth"),
        .height = 13,
        .weight = 420,
        .description = gFrosmothPokedexText,
    },

    [NATIONAL_DEX_STONJOURNER] =
    {
        .categoryName = _("Big Rock"),
        .height = 25,
        .weight = 5200,
        .description = gStonjournerPokedexText,
    },

    [NATIONAL_DEX_EISCUE] =
    {
        .categoryName = _("Penguin"),
        .height = 14,
        .weight = 890,
        .description = gEiscuePokedexText,
    },

    [NATIONAL_DEX_INDEEDEE] =
    {
        .categoryName = _("Emotion"),
        .height = 9,
        .weight = 280,
        .description = gIndeedeePokedexText,
    },

    [NATIONAL_DEX_MORPEKO] =
    {
        .categoryName = _("Two-Sided"),
        .height = 3,
        .weight = 30,
        .description = gMorpekoPokedexText,
    },

    [NATIONAL_DEX_CUFANT] =
    {
        .categoryName = _("Copperderm"),
        .height = 12,
        .weight = 1000,
        .description = gCufantPokedexText,
    },

    [NATIONAL_DEX_COPPERAJAH] =
    {
        .categoryName = _("Copperderm"),
        .height = 30,
        .weight = 6500,
        .description = gCopperajahPokedexText,
    },

    [NATIONAL_DEX_DRACOZOLT] =
    {
        .categoryName = _("Fossil"),
        .height = 18,
        .weight = 1900,
        .description = gDracozoltPokedexText,
    },

    [NATIONAL_DEX_ARCTOZOLT] =
    {
        .categoryName = _("Fossil"),
        .height = 23,
        .weight = 1500,
        .description = gArctozoltPokedexText,
    },

    [NATIONAL_DEX_DRACOVISH] =
    {
        .categoryName = _("Fossil"),
        .height = 23,
        .weight = 2150,
        .description = gDracovishPokedexText,
    },

    [NATIONAL_DEX_ARCTOVISH] =
    {
        .categoryName = _("Fossil"),
        .height = 20,
        .weight = 1750,
        .description = gArctovishPokedexText,
    },

    [NATIONAL_DEX_DURALUDON] =
    {
        .categoryName = _("Alloy"),
        .height = 18,
        .weight = 400,
        .description = gDuraludonPokedexText,
    },

    [NATIONAL_DEX_DREEPY] =
    {
        .categoryName = _("Lingering"),
        .height = 5,
        .weight = 20,
        .description = gDreepyPokedexText,
    },

    [NATIONAL_DEX_DRAKLOAK] =
    {
        .categoryName = _("Caretaker"),
        .height = 14,
        .weight = 110,
        .description = gDrakloakPokedexText,
    },

    [NATIONAL_DEX_DRAGAPULT] =
    {
        .categoryName = _("Stealth"),
        .height = 30,
        .weight = 500,
        .description = gDragapultPokedexText,
    },

    [NATIONAL_DEX_ZACIAN] =
    {
        .categoryName = _("Warrior"),
        .height = 28,
        .weight = 1100,
        .description = gZacianPokedexText,
    },

    [NATIONAL_DEX_ZAMAZENTA] =
    {
        .categoryName = _("Warrior"),
        .height = 29,
        .weight = 2100,
        .description = gZamazentaPokedexText,
    },

    [NATIONAL_DEX_ETERNATUS] =
    {
        .categoryName = _("Gigantic"),
        .height = 200,
        .weight = 9500,
        .description = gEternatusPokedexText,
    },

    [NATIONAL_DEX_KUBFU] =
    {
        .categoryName = _("Wushu"),
        .height = 6,
        .weight = 120,
        .description = gKubfuPokedexText,
    },

    [NATIONAL_DEX_URSHIFU] =
    {
        .categoryName = _("Wushu"),
        .height = 19,
        .weight = 1050,
        .description = gUrshifuPokedexText,
    },

    [NATIONAL_DEX_ZARUDE] =
    {
        .categoryName = _("Rogue Monkey"),
        .height = 18,
        .weight = 700,
        .description = gZarudePokedexText,
    },

    [NATIONAL_DEX_REGIELEKI] =
    {
        .categoryName = _("Electron"),
        .height = 12,
        .weight = 1450,
        .description = gRegielekiPokedexText,
    },

    [NATIONAL_DEX_REGIDRAGO] =
    {
        .categoryName = _("Dragon Orb"),
        .height = 21,
        .weight = 2000,
        .description = gRegidragoPokedexText,
    },

    [NATIONAL_DEX_GLASTRIER] =
    {
        .categoryName = _("Wild Horse"),
        .height = 22,
        .weight = 8000,
        .description = gGlastrierPokedexText,
    },

    [NATIONAL_DEX_SPECTRIER] =
    {
        .categoryName = _("Swift Horse"),
        .height = 20,
        .weight = 445,
        .description = gSpectrierPokedexText,
    },

    [NATIONAL_DEX_CALYREX] =
    {
        .categoryName = _("King"),
        .height = 11,
        .weight = 77,
        .description = gCalyrexPokedexText,
    },

    [NATIONAL_DEX_WYRDEER] =
    {
        .categoryName = _("Big Horn"),
        .height = 18,
        .weight = 951,
        .description = gWyrdeerPokedexText,
    },

    [NATIONAL_DEX_KLEAVOR] =
    {
        .categoryName = _("Axe"),
        .height = 18,
        .weight = 890,
        .description = gKleavorPokedexText,
    },

    [NATIONAL_DEX_URSALUNA] =
    {
        .categoryName = _("Peat"),
        .height = 24,
        .weight = 2900,
        .description = gUrsalunaPokedexText,
    },

    [NATIONAL_DEX_BASCULEGION] =
    {
        .categoryName = _("Big Fish"),
        .height = 30,
        .weight = 1100,
        .description = gBasculegionPokedexText,
    },

    [NATIONAL_DEX_SNEASLER] =
    {
        .categoryName = _("Free Climb"),
        .height = 13,
        .weight = 430,
        .description = gSneaslerPokedexText,
    },

    [NATIONAL_DEX_OVERQWIL] =
    {
        .categoryName = _("Pin Cluster"),
        .height = 25,
        .weight = 605,
        .description = gOverqwilPokedexText,
    },

    [NATIONAL_DEX_ENAMORUS] =
    {
        .categoryName = _("Love-Hate"),
        .height = 16,
        .weight = 480,
        .description = gEnamorusPokedexText,
    },
};
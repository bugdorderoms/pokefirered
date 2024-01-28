static const u8 sCloudNineDescription[]     = _("Eliminates the effects of weather.");
static const u8 sClearBodyDescription[]     = _("Prevents other Pokémon's moves or Abilities\nfrom lowering the Pokémon's stats.");
static const u8 sPlusAndMinusDescription[]  = _("Boosts the Pokémon's Sp. Atk stat if an ally\nwith the Plus or Minus Ability is in battle.");
static const u8 sFilterDescription[]        = _("Reduces the power of “super effective”\nattacks taken.");
static const u8 sMultiscaleDescription[]    = _("Reduces the amount of damage the Pokémon\ntakes while its HP is full.");
static const u8 sTurboblazeDescription[]    = _("The Pokémon's moves are unimpeded by the\nAbility of the target.");
static const u8 sProteanDescription[]       = _("Changes the Pokémon's type to the type of the\nmove it's about to use.");
static const u8 sReceiverDescription[]      = _("The Pokémon copies the Ability of a defeated\nally.");
static const u8 sPropellerTailDescription[] = _("Ignores the effects of opposing Pokémon's\nAbilities and moves that draw in moves.");

const struct Ability gAbilities[ABILITIES_COUNT] =
{
    [ABILITY_NONE] =
    {
        .name = _("-------"),
        .description = COMPOUND_STRING("No special Ability."),
    },
    [ABILITY_STENCH] =
    {
        .name = _("Stench"),
        .description = COMPOUND_STRING("By releasing stench when attacking, this\nPokémon may cause the target to flinch."),
    },
    [ABILITY_DRIZZLE] =
    {
        .name = _("Drizzle"),
        .description = COMPOUND_STRING("The Pokémon makes it rain when it enters a\nbattle."),
    },
    [ABILITY_SPEED_BOOST] =
    {
        .name = _("Speed Boost"),
        .description = COMPOUND_STRING("Its Speed stat is boosted every turn."),
    },
    [ABILITY_BATTLE_ARMOR] =
    {
        .name = _("Battle Armor"),
        .description = COMPOUND_STRING("Hard armor protects the Pokémon from critical\nhits."),
		.breakable = TRUE,
    },
    [ABILITY_STURDY] =
    {
        .name = _("Sturdy"),
        .description = COMPOUND_STRING("It cannot be knocked out with one hit. One-hit\nKO moves cannot knock it out, either."),
		.breakable = TRUE,
    },
    [ABILITY_DAMP] =
    {
        .name = _("Damp"),
        .description = COMPOUND_STRING("Prevents the use of explosive moves, such as\nSelf-Destruct, by dampening its surroundings."),
		.breakable = TRUE,
    },
    [ABILITY_LIMBER] =
    {
        .name = _("Limber"),
        .description = COMPOUND_STRING("Its limber body protects the Pokémon from\nparalysis."),
		.breakable = TRUE,
    },
    [ABILITY_SAND_VEIL] =
    {
        .name = _("Sand Veil"),
        .description = COMPOUND_STRING("Boosts the Pokémon's evasiveness in a\nsandstorm."),
		.breakable = TRUE,
    },
    [ABILITY_STATIC] =
    {
        .name = _("Static"),
        .description = COMPOUND_STRING("The Pokémon is charged with static electricity,\nso contact with it may cause paralysis."),
    },
    [ABILITY_VOLT_ABSORB] =
    {
        .name = _("Volt Absorb"),
        .description = COMPOUND_STRING("Restores HP if hit by an Electric-type move\ninstead of taking damage."),
		.breakable = TRUE,
    },
    [ABILITY_WATER_ABSORB] =
    {
        .name = _("Water Absorb"),
        .description = COMPOUND_STRING("Restores HP if hit by a Water-type move\ninstead of taking damage."),
		.breakable = TRUE,
    },
    [ABILITY_OBLIVIOUS] =
    {
        .name = _("Oblivious"),
        .description = COMPOUND_STRING("The Pokémon is oblivious, and that keeps it\nfrom being infatuated or falling for taunts."),
		.breakable = TRUE,
    },
    [ABILITY_CLOUD_NINE] =
    {
        .name = _("Cloud Nine"),
        .description = sCloudNineDescription,
    },
    [ABILITY_COMPOUND_EYES] =
    {
        .name = _("Compound Eyes"),
        .description = COMPOUND_STRING("The Pokémon's compound eyes boost its\naccuracy."),
    },
    [ABILITY_INSOMNIA] =
    {
        .name = _("Insomnia"),
        .description = COMPOUND_STRING("The Pokémon is suffering from insomnia and\ncannot fall asleep."),
		.breakable = TRUE,
    },
    [ABILITY_COLOR_CHANGE] =
    {
        .name = _("Color Change"),
        .description = COMPOUND_STRING("The Pokémon's type becomes the type of the\nmove used on it."),
    },
    [ABILITY_IMMUNITY] =
    {
        .name = _("Immunity"),
        .description = COMPOUND_STRING("The immune system of the Pokémon prevents it\nfrom getting poisoned."),
		.breakable = TRUE,
    },
    [ABILITY_FLASH_FIRE] =
    {
        .name = _("Flash Fire"),
        .description = COMPOUND_STRING("Powers up the Pokémon's Fire-type moves if\nit's hit by one."),
		.breakable = TRUE,
    },
    [ABILITY_SHIELD_DUST] =
    {
        .name = _("Shield Dust"),
        .description = COMPOUND_STRING("This Pokémon's dust blocks the additional\neffects of attacks taken."),
		.breakable = TRUE,
    },
    [ABILITY_OWN_TEMPO] =
    {
        .name = _("Own Tempo"),
        .description = COMPOUND_STRING("This Pokémon has its own tempo, and that\nprevents it from becoming confused."),
		.breakable = TRUE,
    },
    [ABILITY_SUCTION_CUPS] =
    {
        .name = _("Suction Cups"),
        .description = COMPOUND_STRING("This Pokémon uses suction cups to stay in one\nspot to negate moves that force switching out."),
		.breakable = TRUE,
    },
    [ABILITY_INTIMIDATE] =
    {
        .name = _("Intimidate"),
        .description = COMPOUND_STRING("The Pokémon intimidates opposing Pokémon upon\nentering battle, lowering their Attack stat."),
    },
    [ABILITY_SHADOW_TAG] =
    {
        .name = _("Shadow Tag"),
        .description = COMPOUND_STRING("This Pokémon steps on the opposing Pokémon's\nshadow to prevent it from escaping."),
    },
    [ABILITY_ROUGH_SKIN] =
    {
        .name = _("Rough Skin"),
        .description = COMPOUND_STRING("This Pokémon inflicts damage with its rough\nskin to the attacker on contact."),
    },
    [ABILITY_WONDER_GUARD] =
    {
        .name = _("Wonder Guard"),
        .description = COMPOUND_STRING("Its mysterious power only lets\n“super effective” moves hit the Pokémon."),
		.cantBeCopied = TRUE,
		.cantBeSwapped = TRUE,
		.breakable = TRUE,
    },
    [ABILITY_LEVITATE] =
    {
        .name = _("Levitate"),
        .description = COMPOUND_STRING("By floating in the air, the Pokémon receives\nfull immunity to all Ground-type moves."),
		.breakable = TRUE,
    },
    [ABILITY_EFFECT_SPORE] =
    {
        .name = _("Effect Spore"),
        .description = COMPOUND_STRING("Contact with the Pokémon may inflict poison,\nsleep, or paralysis on its attacker."),
    },
    [ABILITY_SYNCHRONIZE] =
    {
        .name = _("Synchronize"),
        .description = COMPOUND_STRING("The attacker will receive the same status\ncondition that inflicts to the Pokémon."),
    },
    [ABILITY_CLEAR_BODY] =
    {
        .name = _("Clear Body"),
        .description = sClearBodyDescription,
		.breakable = TRUE,
    },
    [ABILITY_NATURAL_CURE] =
    {
        .name = _("Natural Cure"),
        .description = COMPOUND_STRING("All status conditions heal when the Pokémon\nswitches out."),
    },
    [ABILITY_LIGHTNING_ROD] =
    {
        .name = _("Lightning Rod"),
        .description = COMPOUND_STRING("The Pokémon draws in all Electric-type moves.\nInstead of being hit, it boosts its Sp. Atk."),
		.breakable = TRUE,
    },
    [ABILITY_SERENE_GRACE] =
    {
        .name = _("Serene Grace"),
        .description = COMPOUND_STRING("Boosts the likelihood of additional effects\noccurring when attacking."),
    },
    [ABILITY_SWIFT_SWIM] =
    {
        .name = _("Swift Swim"),
        .description = COMPOUND_STRING("Boosts the Pokémon's Speed stat in rain."),
    },
    [ABILITY_CHLOROPHYLL] =
    {
        .name = _("Chlorophyll"),
        .description = COMPOUND_STRING("Boosts the Pokémon's Speed stat in harsh\nsunlight."),
    },
    [ABILITY_ILLUMINATE] =
    {
        .name = _("Illuminate"),
        .description = COMPOUND_STRING("By illuminating its surroundings, the Pokémon\nprevents its accuracy from being lowered."),
		.breakable = TRUE,
    },
    [ABILITY_TRACE] =
    {
        .name = _("Trace"),
        .description = COMPOUND_STRING("When it enters a battle, the Pokémon copies an\nopposing Pokémon's Ability."),
		.cantBeCopied = TRUE,
		.cantBeTraced = TRUE,
    },
    [ABILITY_HUGE_POWER] =
    {
        .name = _("Huge Power"),
        .description = COMPOUND_STRING("Doubles the Pokémon's Attack stat."),
    },
    [ABILITY_POISON_POINT] =
    {
        .name = _("Poison Point"),
        .description = COMPOUND_STRING("Contact with the Pokémon may poison the\nattacker."),
    },
    [ABILITY_INNER_FOCUS] =
    {
        .name = _("Inner Focus"),
        .description = COMPOUND_STRING("The Pokémon's intensely focused, and that\nprotects the Pokémon from flinching."),
		.breakable = TRUE,
    },
    [ABILITY_MAGMA_ARMOR] =
    {
        .name = _("Magma Armor"),
        .description = COMPOUND_STRING("The Pokémon is covered with hot magma, which\nprevents the Pokémon from becoming frozen."),
		.breakable = TRUE,
    },
    [ABILITY_WATER_VEIL] =
    {
        .name = _("Water Veil"),
        .description = COMPOUND_STRING("The Pokémon is covered with a water veil,\nwhich prevents the Pokémon from getting burn."),
		.breakable = TRUE,
    },
    [ABILITY_MAGNET_PULL] =
    {
        .name = _("Magnet Pull"),
        .description = COMPOUND_STRING("Prevents Steel-type Pokémon from escaping\nusing its magnetic force."),
    },
    [ABILITY_SOUNDPROOF] =
    {
        .name = _("Soundproof"),
        .description = COMPOUND_STRING("Soundproofing gives the Pokémon full immunity\nto all sound-based moves."),
		.breakable = TRUE,
    },
    [ABILITY_RAIN_DISH] =
    {
        .name = _("Rain Dish"),
        .description = COMPOUND_STRING("The Pokémon gradually regains HP in rain."),
    },
    [ABILITY_SAND_STREAM] =
    {
        .name = _("Sand Stream"),
        .description = COMPOUND_STRING("The Pokémon summons a sandstorm when it enters\na battle."),
    },
    [ABILITY_PRESSURE] =
    {
        .name = _("Pressure"),
        .description = COMPOUND_STRING("By putting pressure on the opposing Pokémon,\nit raises their PP usage."),
    },
    [ABILITY_THICK_FAT] =
    {
        .name = _("Thick Fat"),
        .description = COMPOUND_STRING("The Pokémon is protected by a thick fat, which\nhalves the damage of Fire and Ice-type moves."),
		.breakable = TRUE,
    },
    [ABILITY_EARLY_BIRD] =
    {
        .name = _("Early Bird"),
        .description = COMPOUND_STRING("The Pokémon awakens from sleep twice as fast\nas other Pokémon."),
    },
    [ABILITY_FLAME_BODY] =
    {
        .name = _("Flame Body"),
        .description = COMPOUND_STRING("Contact with the Pokémon may burn the\nattacker."),
    },
    [ABILITY_RUN_AWAY] =
    {
        .name = _("Run Away"),
        .description = COMPOUND_STRING("Enables a sure getaway from wild Pokémon."),
    },
    [ABILITY_KEEN_EYE] =
    {
        .name = _("Keen Eye"),
        .description = COMPOUND_STRING("Keen eyes prevent other Pokémon from lowering\nthis Pokémon's accuracy."),
		.breakable = TRUE,
    },
    [ABILITY_HYPER_CUTTER] =
    {
        .name = _("Hyper Cutter"),
        .description = COMPOUND_STRING("The Pokémon's proud of its pincers. They\nprevent the foe from lowering its Attack stat."),
		.breakable = TRUE,
    },
    [ABILITY_PICKUP] =
    {
        .name = _("Pickup"),
        .description = COMPOUND_STRING("The Pokémon may pick up the item an opposing\nPokémon used during a battle."),
    },
    [ABILITY_TRUANT] =
    {
        .name = _("Truant"),
        .description = COMPOUND_STRING("The Pokémon can't use a move if it had used a\nmove on the previous turn."),
		.cantBeOverwritten = TRUE,
    },
    [ABILITY_HUSTLE] =
    {
        .name = _("Hustle"),
        .description = COMPOUND_STRING("Boosts the Attack stat, but lowers accuracy."),
    },
    [ABILITY_CUTE_CHARM] =
    {
        .name = _("Cute Charm"),
        .description = COMPOUND_STRING("Contact with the Pokémon may cause\ninfatuation."),
    },
    [ABILITY_PLUS] =
    {
        .name = _("Plus"),
        .description = sPlusAndMinusDescription,
    },
    [ABILITY_MINUS] =
    {
        .name = _("Minus"),
        .description = sPlusAndMinusDescription,
    },
    [ABILITY_FORECAST] =
    {
        .name = _("Forecast"),
        .description = COMPOUND_STRING("The Pokémon transforms with the weather to\nchange its type to Water, Fire, or Ice."),
		.cantBeCopied = TRUE,
		.cantBeTraced = TRUE,
    },
    [ABILITY_STICKY_HOLD] =
    {
        .name = _("Sticky Hold"),
        .description = COMPOUND_STRING("Items held by the Pokémon are stuck fast and\ncannot be removed by other Pokémon."),
		.breakable = TRUE,
    },
    [ABILITY_SHED_SKIN] =
    {
        .name = _("Shed Skin"),
        .description = COMPOUND_STRING("The Pokémon may heal its own status conditions\nby shedding its skin."),
    },
    [ABILITY_GUTS] =
    {
        .name = _("Guts"),
        .description = COMPOUND_STRING("It's so gutsy that having a status condition\nboosts the Pokémon's Attack stat."),
    },
    [ABILITY_MARVEL_SCALE] =
    {
        .name = _("Marmel Scale"),
        .description = COMPOUND_STRING("The Pokémon's marvelous scales boost the\nDefense stat if it has a status condition."),
		.breakable = TRUE,
    },
    [ABILITY_LIQUID_OOZE] =
    {
        .name = _("Liquid Ooze"),
        .description = COMPOUND_STRING("The oozed liquid has a strong stench, which\ndamages attackers using any draining move."),
    },
    [ABILITY_OVERGROW] =
    {
        .name = _("Overgrow"),
        .description = COMPOUND_STRING("Powers up Grass-type moves when the Pokémon's\nHP is low."),
    },
    [ABILITY_BLAZE] =
    {
        .name = _("Blaze"),
        .description = COMPOUND_STRING("Powers up Fire-type moves when the Pokémon's\nHP is low."),
    },
    [ABILITY_TORRENT] =
    {
        .name = _("Torrent"),
        .description = COMPOUND_STRING("Powers up Water-type moves when the Pokémon's\nHP is low."),
    },
    [ABILITY_SWARM] =
    {
        .name = _("Swarm"),
        .description = COMPOUND_STRING("Powers up Bug-type moves when the Pokémon's\nHP is low."),
    },
    [ABILITY_ROCK_HEAD] =
    {
        .name = _("Rock Head"),
        .description = COMPOUND_STRING("Protects the Pokémon from recoil damage."),
    },
    [ABILITY_DROUGHT] =
    {
        .name = _("Drought"),
        .description = COMPOUND_STRING("Turns the sunlight harsh when enters a battle."),
    },
    [ABILITY_ARENA_TRAP] =
    {
        .name = _("Arena Trap"),
        .description = COMPOUND_STRING("Prevents opposing Pokémon from fleeing."),
    },
    [ABILITY_VITAL_SPIRIT] =
    {
        .name = _("Vital Spirit"),
        .description = COMPOUND_STRING("The Pokémon is full of vitality, and that\nprevents it from falling asleep."),
		.breakable = TRUE,
    },
    [ABILITY_WHITE_SMOKE] =
    {
        .name = _("White Smoke"),
        .description = COMPOUND_STRING("The Pokémon's white smoke protect it, which\nprevents the foe from lowering its stats."),
		.breakable = TRUE,
    },
    [ABILITY_PURE_POWER] =
    {
        .name = _("Pure Power"),
        .description = COMPOUND_STRING("Using its pure power, the Pokémon doubles its\nAttack stat."),
    },
    [ABILITY_SHELL_ARMOR] =
    {
        .name = _("Shell Armor"),
        .description = COMPOUND_STRING("A hard shell protects the Pokémon from\ncritical hits."),
		.breakable = TRUE,
    },
    [ABILITY_AIR_LOCK] =
    {
        .name = _("Air Lock"),
        .description = sCloudNineDescription,
    },
    [ABILITY_TANGLED_FEET] =
    {
        .name = _("Tangled Feet"),
        .description = COMPOUND_STRING("Raises evasiveness if the Pokémon is confused."),
		.breakable = TRUE,
    },
    [ABILITY_MOTOR_DRIVE] =
    {
        .name = _("Motor Drive"),
        .description = COMPOUND_STRING("Boosts its Speed stat if hit by an\nElectric-type move instead of taking damage."),
		.breakable = TRUE,
    },
    [ABILITY_RIVALRY] =
    {
        .name = _("Rivalry"),
        .description = COMPOUND_STRING("Deals more damage if the Pokémon has the same\ngender, but deal less damage to opposite."),
    },
    [ABILITY_STEADFAST] =
    {
        .name = _("Steadfast"),
        .description = COMPOUND_STRING("The Pokémon's determination boosts the Speed\nstat each time the Pokémon flinches."),
    },
    [ABILITY_SNOW_CLOAK] =
    {
        .name = _("Snow Cloak"),
        .description = COMPOUND_STRING("Boosts evasiveness in a hailstorm."),
		.breakable = TRUE,
    },
    [ABILITY_GLUTTONY] =
    {
        .name = _("Gluttony"),
        .description = COMPOUND_STRING("Makes the Pokémon eat a held Berry when its\nHP drops to half or less, instead of usual."),
    },
    [ABILITY_ANGER_POINT] =
    {
        .name = _("Anger Point"),
        .description = COMPOUND_STRING("The Pokémon is angered when it takes a\ncritical hit, and that maxes its Attack stat."),
    },
    [ABILITY_UNBURDEN] =
    {
        .name = _("Unburden"),
        .description = COMPOUND_STRING("Boosts the Speed stat if the Pokémon's held\nitem is used or lost."),
    },
    [ABILITY_HEATPROOF] =
    {
        .name = _("Heatproof"),
        .description = COMPOUND_STRING("The heatproof body of the Pokémon halves the\ndamage from Fire-type moves that hit it."),
		.breakable = TRUE,
    },
    [ABILITY_SIMPLE] =
    {
        .name = _("Simple"),
        .description = COMPOUND_STRING("The stat changes the Pokémon receives are\ndoubled."),
		.breakable = TRUE,
    },
    [ABILITY_DRY_SKIN] =
    {
        .name = _("Dry Skin"),
        .description = COMPOUND_STRING("Restores HP with water, losts HP in sunlight.\nReceives more damage from Fire-type moves."),
		.breakable = TRUE,
    },
    [ABILITY_DOWNLOAD] =
    {
        .name = _("Download"),
        .description = COMPOUND_STRING("Raises the Pokémon's Attack or Sp. Atk stat\nto whichever will be more effective."),
    },
    [ABILITY_IRON_FIST] =
    {
        .name = _("Iron Fist"),
        .description = COMPOUND_STRING("Powers up “punching” moves."),
    },
    [ABILITY_POISON_HEAL] =
    {
        .name = _("Poison Heal"),
        .description = COMPOUND_STRING("Restores HP if the Pokémon is poisoned instead\nof losing HP."),
    },
    [ABILITY_ADAPTABILITY] =
    {
        .name = _("Adaptability"),
        .description = COMPOUND_STRING("Powers up moves of the same type as the\nPokémon."),
    },
    [ABILITY_SKILL_LINK] =
    {
        .name = _("Skill Link"),
        .description = COMPOUND_STRING("Maximizes the number of times multistrike\nmoves hit."),
    },
    [ABILITY_HYDRATION] =
    {
        .name = _("Hydration"),
        .description = COMPOUND_STRING("Heals status conditions if it's raining."),
    },
    [ABILITY_SOLAR_POWER] =
    {
        .name = _("Solar Power"),
        .description = COMPOUND_STRING("Boosts the Sp. Atk stat in harsh sunlight, but\nHP decreases every turn."),
    },
    [ABILITY_QUICK_FEET] =
    {
        .name = _("Quick Feet"),
        .description = COMPOUND_STRING("Boosts the Speed stat if the Pokémon has a\nstatus condition."),
    },
    [ABILITY_NORMALIZE] =
    {
        .name = _("Normalize"),
        .description = COMPOUND_STRING("All the Pokémon's moves become Normal type.\nThe power of those moves is boosted a little."),
    },
    [ABILITY_SNIPER] =
    {
        .name = _("Sniper"),
        .description = COMPOUND_STRING("Powers up moves if they become critical hits\nwhen attacking."),
    },
    [ABILITY_MAGIC_GUARD] =
    {
        .name = _("Magic Guard"),
        .description = COMPOUND_STRING("The Pokémon only takes damage from attacks."),
    },
    [ABILITY_NO_GUARD] =
    {
        .name = _("No Guard"),
        .description = COMPOUND_STRING("The Pokémon employs no-guard tactics to ensure\nincoming and outgoing attacks always land."),
    },
    [ABILITY_STALL] =
    {
        .name = _("Stall"),
        .description = COMPOUND_STRING("The Pokémon moves after all other Pokémon do."),
    },
    [ABILITY_TECHNICIAN] =
    {
        .name = _("Technician"),
        .description = COMPOUND_STRING("Powers up the Pokémon's weaker moves."),
    },
    [ABILITY_LEAF_GUARD] =
    {
        .name = _("Leaf Guard"),
        .description = COMPOUND_STRING("Prevents status conditions in harsh sunlight."),
		.breakable = TRUE,
    },
    [ABILITY_KLUTZ] =
    {
        .name = _("Klutz"),
        .description = COMPOUND_STRING("The Pokémon can't use any held items."),
    },
    [ABILITY_MOLD_BREAKER] =
    {
        .name = _("Mold Breaker"),
        .description = COMPOUND_STRING("Moves can be used on the target regardless of\nits Abilities."),
    },
    [ABILITY_SUPER_LUCK] =
    {
        .name = _("Super Luck"),
        .description = COMPOUND_STRING("The Pokémon is so lucky that the critical-hit\nratios of its moves are boosted."),
    },
    [ABILITY_AFTERMATH] =
    {
        .name = _("Aftermath"),
        .description = COMPOUND_STRING("Damages the attacker if it contacts the\nPokémon with a finishing hit."),
    },
    [ABILITY_ANTICIPATION] =
    {
        .name = _("Anticipation"),
        .description = COMPOUND_STRING("The Pokémon can sense an opposing Pokémon's\ndangerous moves."),
    },
    [ABILITY_FOREWARN] =
    {
        .name = _("Forewarn"),
        .description = COMPOUND_STRING("When it enters a battle, the Pokémon can tell\none of the moves an opposing Pokémon has."),
    },
    [ABILITY_UNAWARE] =
    {
        .name = _("Unaware"),
        .description = COMPOUND_STRING("When attacking, the Pokémon ignores the target\nPokémon's stat changes."),
		.breakable = TRUE,
    },
    [ABILITY_TINTED_LENS] =
    {
        .name = _("Tinted Lens"),
        .description = COMPOUND_STRING("The Pokémon can use “not very effective” moves\nto deal regular damage."),
    },
    [ABILITY_FILTER] =
    {
        .name = _("Filter"),
        .description = sFilterDescription,
		.breakable = TRUE,
    },
    [ABILITY_SLOW_START] =
    {
        .name = _("Slow Start"),
        .description = COMPOUND_STRING("The Pokémon's Attack and Speed stats are\nhalved until it gets going."),
    },
    [ABILITY_SCRAPPY] =
    {
        .name = _("Scrappy"),
        .description = COMPOUND_STRING("The Pokémon can hit Ghost-type Pokémon with\nNormal and Fighting-type moves."),
    },
    [ABILITY_STORM_DRAIN] =
    {
        .name = _("Storm Drain"),
        .description = COMPOUND_STRING("The Pokémon draws in all Water-type moves.\nInstead of being hit, it boosts its Sp. Atk."),
		.breakable = TRUE,
    },
    [ABILITY_ICE_BODY] =
    {
        .name = _("Ice Body"),
        .description = COMPOUND_STRING("The Pokémon gradually regains HP in a\nhailstorm."),
    },
    [ABILITY_SOLID_ROCK] =
    {
        .name = _("Solid Rock"),
        .description = sFilterDescription,
		.breakable = TRUE,
    },
    [ABILITY_SNOW_WARNING] =
    {
        .name = _("Snow Warning"),
        .description = COMPOUND_STRING("The Pokémon summons a hailstorm when it enters\na battle."),
    },
    [ABILITY_HONEY_GATHER] =
    {
        .name = _("Honey Gather"),
        .description = COMPOUND_STRING("The Pokémon may gather Honey after a battle."),
    },
    [ABILITY_FRISK] =
    {
        .name = _("Frisk"),
        .description = COMPOUND_STRING("When it enters a battle, the Pokémon can\ncheck an opposing Pokémon's held item."),
    },
    [ABILITY_RECKLESS] =
    {
        .name = _("Reckless"),
        .description = COMPOUND_STRING("Powers up moves that have recoil damage."),
    },
    [ABILITY_MULTITYPE] =
    {
        .name = _("Multitype"),
        .description = COMPOUND_STRING("Changes the Pokémon's type to match the Plate\nor Z-Crystal it holds."),
		.cantBeCopied = TRUE,
		.cantBeSwapped = TRUE,
		.cantBeTraced = TRUE,
		.cantBeSuppressed = TRUE,
		.cantBeOverwritten = TRUE,
    },
    [ABILITY_FLOWER_GIFT] =
    {
        .name = _("Flower Gift"),
        .description = COMPOUND_STRING("Boosts the Attack and Sp. Def stats of itself\nand allies in harsh sunlight."),
		.cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
        .breakable = TRUE,
    },
    [ABILITY_BAD_DREAMS] =
    {
        .name = _("Bad Dreams"),
        .description = COMPOUND_STRING("Reduces the HP of sleeping opposing Pokémon."),
    },
    [ABILITY_PICKPOCKET] =
    {
        .name = _("Pickpocket"),
        .description = COMPOUND_STRING("Steals an item from an attacker that made\ndirect contact."),
    },
    [ABILITY_SHEER_FORCE] =
    {
        .name = _("Sheer Force"),
        .description = COMPOUND_STRING("Removes additional effects to increase the\npower of moves when attacking."),
    },
    [ABILITY_CONTRARY] =
    {
        .name = _("Contrary"),
        .description = COMPOUND_STRING("Makes stat changes have an opposite effect."),
		.breakable = TRUE,
    },
    [ABILITY_UNNERVE] =
    {
        .name = _("Unnerve"),
        .description = COMPOUND_STRING("Unnerves opposing Pokémon and makes them\nunable to eat Berries."),
    },
    [ABILITY_DEFIANT] =
    {
        .name = _("Defiant"),
        .description = COMPOUND_STRING("Boosts the Pokémon's Attack stat sharply when\nits stats are lowered."),
    },
    [ABILITY_DEFEATIST] =
    {
        .name = _("Defeatist"),
        .description = COMPOUND_STRING("Halves the Pokémon's Attack and Sp. Atk stats\nwhen its HP becomes half or less."),
    },
    [ABILITY_CURSED_BODY] =
    {
        .name = _("Cursed Body"),
        .description = COMPOUND_STRING("May disable a move used on the Pokémon."),
    },
    [ABILITY_HEALER] =
    {
        .name = _("Healer"),
        .description = COMPOUND_STRING("Sometimes heals an ally's status condition."),
    },
    [ABILITY_FRIEND_GUARD] =
    {
        .name = _("Friend Guard"),
        .description = COMPOUND_STRING("Reduces damage done to allies."),
		.breakable = TRUE,
    },
    [ABILITY_WEAK_ARMOR] =
    {
        .name = _("Weak Armor"),
        .description = COMPOUND_STRING("Physical attacks to the Pokémon lower its\nDefense stat but sharply raise its Speed stat."),
    },
    [ABILITY_HEAVY_METAL] =
    {
        .name = _("Heavy Metal"),
        .description = COMPOUND_STRING("Doubles the Pokémon's weight."),
		.breakable = TRUE,
    },
    [ABILITY_LIGHT_METAL] =
    {
        .name = _("Light Metal"),
        .description = COMPOUND_STRING("Halves the Pokémon's weight."),
		.breakable = TRUE,
    },
    [ABILITY_MULTISCALE] =
    {
        .name = _("Multiscale"),
        .description = sMultiscaleDescription,
		.breakable = TRUE,
    },
    [ABILITY_TOXIC_BOOST] =
    {
        .name = _("Toxic Boost"),
        .description = COMPOUND_STRING("Powers up physical attacks when the Pokémon is\npoisoned."),
    },
    [ABILITY_FLARE_BOOST] =
    {
        .name = _("Flare Boost"),
        .description = COMPOUND_STRING("Powers up special attacks when the Pokémon is\nburned."),
    },
    [ABILITY_HARVEST] =
    {
        .name = _("Harvest"),
        .description = COMPOUND_STRING("May create another Berry after one is used."),
    },
    [ABILITY_TELEPATHY] =
    {
        .name = _("Telepathy"),
        .description = COMPOUND_STRING("Anticipates an ally's attack and dodges it."),
		.breakable = TRUE,
    },
    [ABILITY_MOODY] =
    {
        .name = _("Moody"),
        .description = COMPOUND_STRING("Raises one stat sharply and lowers another\nevery turn."),
    },
    [ABILITY_OVERCOAT] =
    {
        .name = _("Overcoat"),
        .description = COMPOUND_STRING("Protects the Pokémon from things like sand,\nhail, and “powder”."),
		.breakable = TRUE,
    },
    [ABILITY_POISON_TOUCH] =
    {
        .name = _("Poison Touch"),
        .description = COMPOUND_STRING("May poison a target when the Pokémon makes\ncontact."),
    },
    [ABILITY_REGENERATOR] =
    {
        .name = _("Regenerator"),
        .description = COMPOUND_STRING("Restores a little HP when withdrawn from\nbattle."),
    },
    [ABILITY_BIG_PECKS] =
    {
        .name = _("Big Pecks"),
        .description = COMPOUND_STRING("Protects the Pokémon from Defense-lowering\neffects."),
		.breakable = TRUE,
    },
    [ABILITY_SAND_RUSH] =
    {
        .name = _("Sand Rush"),
        .description = COMPOUND_STRING("Boosts the Pokémon's Speed stat in a\nsandstorm."),
    },
    [ABILITY_WONDER_SKIN] =
    {
        .name = _("Wonder Skin"),
        .description = COMPOUND_STRING("Makes status moves more likely to miss."),
		.breakable = TRUE,
    },
    [ABILITY_ANALYTIC] =
    {
        .name = _("Analytic"),
        .description = COMPOUND_STRING("Boosts move power when the Pokémon moves\nlast."),
    },
    [ABILITY_ILLUSION] =
    {
        .name = _("Illusion"),
        .description = COMPOUND_STRING("Comes out disguised as the Pokémon in the\nparty's last spot."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },
    [ABILITY_IMPOSTER] =
    {
        .name = _("Imposter"),
        .description = COMPOUND_STRING("The Pokémon transforms itself into the Pokémon\nit's facing."),
		.cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },
    [ABILITY_INFILTRATOR] =
    {
        .name = _("Infiltrator"),
        .description = COMPOUND_STRING("The Pokémon's moves are unaffected by the\ntarget's barriers, substitutes, and the like."),
    },
	[ABILITY_MUMMY] =
    {
        .name = _("Mummy"),
        .description = COMPOUND_STRING("Contact with the Pokémon changes the\nattacker's Ability to Mummy."),
    },
	[ABILITY_MOXIE] =
    {
        .name = _("Moxie"),
        .description = COMPOUND_STRING("The Pokémon shows moxie, and that boosts the\nAttack stat after knocking out any Pokémon."),
    },
    [ABILITY_JUSTIFIED] =
    {
        .name = _("Justified"),
        .description = COMPOUND_STRING("Being hit by a Dark-type move boosts the\nAttack stat of the Pokémon, for justice."),
    },
    [ABILITY_RATTLED] =
    {
        .name = _("Rattled"),
        .description = COMPOUND_STRING("Dark, Ghost, and Bug-type moves scare the\nPokémon and boost its Speed stat."),
    },
    [ABILITY_MAGIC_BOUNCE] =
    {
        .name = _("Magic Bounce"),
        .description = COMPOUND_STRING("The Pokémon reflects status moves instead of\ngetting hit by them."),
		.breakable = TRUE,
    },
    [ABILITY_SAP_SIPPER] =
    {
        .name = _("Sap Sipper"),
        .description = COMPOUND_STRING("Boosts the Attack stat if hit by a Grass-type\nmove instead of taking damage."),
		.breakable = TRUE,
    },
    [ABILITY_PRANKSTER] =
    {
        .name = _("Prankster"),
        .description = COMPOUND_STRING("Gives priority to the Pokémon's status moves."),
    },
    [ABILITY_SAND_FORCE] =
    {
        .name = _("Sand Force"),
        .description = COMPOUND_STRING("Boosts the power of Rock, Ground, and\nSteel-type moves in a sandstorm."),
    },
    [ABILITY_IRON_BARBS] =
    {
        .name = _("Iron Barbs"),
        .description = COMPOUND_STRING("The Pokémon's iron barbs damage the attacker\nif it makes direct contact."),
    },
    [ABILITY_ZEN_MODE] =
    {
        .name = _("Zen Mode"),
        .description = COMPOUND_STRING("Changes the Pokémon's shape when its HP drops\nto half or less."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
    },
    [ABILITY_VICTORY_STAR] =
    {
        .name = _("Victory Star"),
        .description = COMPOUND_STRING("Boosts the accuracy of the Pokémon and its\nallies."),
    },
    [ABILITY_TURBOBLAZE] =
    {
        .name = _("Turboblaze"),
        .description = sTurboblazeDescription,
    },
    [ABILITY_TERAVOLT] =
    {
        .name = _("Teravolt"),
        .description = sTurboblazeDescription,
    },
    [ABILITY_AROMA_VEIL] =
    {
        .name = _("Aroma Veil"),
        .description = COMPOUND_STRING("Protects the Pokémon and its allies from\neffects that prevent the use of moves."),
		.breakable = TRUE,
    },
    [ABILITY_FLOWER_VEIL] =
    {
        .name = _("Flower Veil"),
        .description = COMPOUND_STRING("Ally Grass-type Pokémon are protected from\nstatus conditions and lowering stat effects."),
		.breakable = TRUE,
    },
    [ABILITY_CHEEK_POUCH] =
    {
        .name = _("Cheek Pouch"),
        .description = COMPOUND_STRING("Restores HP as well when the Pokémon eats a\nBerry."),
    },
    [ABILITY_PROTEAN] =
    {
        .name = _("Protean"),
        .description = sProteanDescription,
    },
    [ABILITY_FUR_COAT] =
    {
        .name = _("Fur Coat"),
        .description = COMPOUND_STRING("Halves the damage from physical moves."),
		.breakable = TRUE,
    },
    [ABILITY_MAGICIAN] =
    {
        .name = _("Magician"),
        .description = COMPOUND_STRING("The Pokémon steals the held item from any\ntarget it hits with a move."),
    },
    [ABILITY_BULLETPROOF] =
    {
        .name = _("Bulletproof"),
        .description = COMPOUND_STRING("Protects the Pokémon from “ball” and “bomb”\nmoves."),
		.breakable = TRUE,
    },
    [ABILITY_COMPETITIVE] =
    {
        .name = _("Competitive"),
        .description = COMPOUND_STRING("Boosts the Pokémon's Sp. Atk stat sharply\nwhen its stats are lowered by an Pokémon."),
    },
    [ABILITY_STRONG_JAW] =
    {
        .name = _("Strong Jaw"),
        .description = COMPOUND_STRING("The Pokémon's strong jaw boosts the power of\nits “biting” moves."),
    },
    [ABILITY_REFRIGERATE] =
    {
        .name = _("Refrigerate"),
        .description = COMPOUND_STRING("Normal-type moves become Ice-type moves. The\npower of those moves is boosted a little."),
    },
    [ABILITY_SWEET_VEIL] =
    {
        .name = _("Sweet Veil"),
        .description = COMPOUND_STRING("Prevents the Pokémon and its allies from\nfalling asleep."),
		.breakable = TRUE,
    },
    [ABILITY_STANCE_CHANGE] =
    {
        .name = _("Stance Change"),
        .description = COMPOUND_STRING("Changes the Pokémon's form to Shield using\nKing's Shield or to Blade using other move."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },
    [ABILITY_GALE_WINGS] =
    {
        .name = _("Gale Wings"),
        .description = COMPOUND_STRING("Gives priority to the Pokémon's Flying-type\nmoves while its HP is full."),
    },
    [ABILITY_MEGA_LAUNCHER] =
    {
        .name = _("Mega Launcher"),
        .description = COMPOUND_STRING("Powers up “aura” and “pulse” moves."),
    },
    [ABILITY_GRASS_PELT] =
    {
        .name = _("Grass Pelt"),
        .description = COMPOUND_STRING("Boosts the Pokémon's Defense stat on Grassy\nTerrain."),
		.breakable = TRUE,
    },
    [ABILITY_SYMBIOSIS] =
    {
        .name = _("Symbiosis"),
        .description = COMPOUND_STRING("The Pokémon passes its held item to an ally\nthat has used up an item."),
    },
    [ABILITY_TOUGH_CLAWS] =
    {
        .name = _("Tough Claws"),
        .description = COMPOUND_STRING("Powers up moves that make direct contact."),
    },
    [ABILITY_PIXILATE] =
    {
        .name = _("Pixilate"),
        .description = COMPOUND_STRING("Normal-type moves become Fairy-type moves. The\npower of those moves is boosted a little."),
    },
    [ABILITY_GOOEY] =
    {
        .name = _("Gooey"),
        .description = COMPOUND_STRING("Contact with the Pokémon lowers the\nattacker's Speed stat."),
    },
    [ABILITY_AERILATE] =
    {
        .name = _("Aerilate"),
        .description = COMPOUND_STRING("Normal-type moves become Flying-type moves.\nThe power of those moves is boosted a little."),
    },
    [ABILITY_PARENTAL_BOND] =
    {
        .name = _("Parental Bond"),
        .description = COMPOUND_STRING("The parent and child attack one after the\nother."),
    },
    [ABILITY_DARK_AURA] =
    {
        .name = _("Dark Aura"),
        .description = COMPOUND_STRING("Powers up the Dark-type moves of all Pokémon\non the field."),
		.breakable = TRUE,
    },
    [ABILITY_FAIRY_AURA] =
    {
        .name = _("Fairy Aura"),
        .description = COMPOUND_STRING("Powers up the Fairy-type moves of all Pokémon\non the field."),
		.breakable = TRUE,
    },
    [ABILITY_AURA_BREAK] =
    {
        .name = _("Aura Break"),
        .description = COMPOUND_STRING("The effects of “aura” Abilities are reversed\nto lower the power of affected moves."),
		.breakable = TRUE,
    },
    [ABILITY_PRIMORDIAL_SEA] =
    {
        .name = _("Primordial Sea"),
        .description = COMPOUND_STRING("The Pokémon changes the weather to nullify\nFire-type attacks."),
    },
    [ABILITY_DESOLATE_LAND] =
    {
        .name = _("Desolate Land"),
        .description = COMPOUND_STRING("The Pokémon changes the weather to nullify\nWater-type attacks."),
    },
    [ABILITY_DELTA_STREAM] =
    {
        .name = _("Delta Stream"),
        .description = COMPOUND_STRING("Changes the weather so that no moves are\n“super effective” against the Flying type."),
    },
    [ABILITY_STAMINA] =
    {
        .name = _("Stamina"),
        .description = COMPOUND_STRING("Boosts the Defense stat when the Pokémon is\nhit by an attack."),
    },
    [ABILITY_WIMP_OUT] =
    {
        .name = _("Wimp Out"),
        .description = COMPOUND_STRING("The Pokémon cowardly switches out when its HP\ndrops to half or less."),
    },
    [ABILITY_EMERGENCY_EXIT] =
    {
        .name = _("Emergency Exit"),
        .description = COMPOUND_STRING("The Pokémon, sensing danger, switches out when\nits HP drops to half or less."),
    },
    [ABILITY_WATER_COMPACTION] =
    {
        .name = _("Water Compaction"),
        .description = COMPOUND_STRING("Boosts the Defense stat sharply when the\nPokémon is hit by a Water-type move."),
    },
    [ABILITY_MERCILESS] =
    {
        .name = _("Merciless"),
        .description = COMPOUND_STRING("The Pokémon's attacks become critical hits if\nthe target is poisoned."),
    },
    [ABILITY_SHIELDS_DOWN] =
    {
        .name = _("Shields Down"),
        .description = COMPOUND_STRING("When its HP drops to half or less, the\nPokémon's shell breaks and it becomes angry."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },
    [ABILITY_STAKEOUT] =
    {
        .name = _("Stakeout"),
        .description = COMPOUND_STRING("Doubles the damage dealt to a target that has\njust switched into battle."),
    },
    [ABILITY_WATER_BUBBLE] =
    {
        .name = _("Water Bubble"),
        .description = COMPOUND_STRING("Lowers the power of Fire-type moves that hit\nthe Pokémon and prevents it from being burned."),
		.breakable = TRUE,
    },
    [ABILITY_STEELWORKER] =
    {
        .name = _("Steelworker"),
        .description = COMPOUND_STRING("Powers up Steel-type moves."),
    },
    [ABILITY_BERSERK] =
    {
        .name = _("Berserk"),
        .description = COMPOUND_STRING("Boosts the Pokémon's Sp. Atk stat when takes\na hit that causes its HP drop to half or less."),
    },
    [ABILITY_SLUSH_RUSH] =
    {
        .name = _("Slush Rush"),
        .description = COMPOUND_STRING("Boosts the Pokémon's Speed stat in snow."),
    },
    [ABILITY_LONG_REACH] =
    {
        .name = _("Long Reach"),
        .description = COMPOUND_STRING("The Pokémon uses its moves without making\ncontact with the target."),
    },
    [ABILITY_LIQUID_VOICE] =
    {
        .name = _("Liquid Voice"),
        .description = COMPOUND_STRING("Sound-based moves become Water-type moves."),
    },
    [ABILITY_TRIAGE] =
    {
        .name = _("Triage"),
        .description = COMPOUND_STRING("Gives priority to the Pokémon's healing moves."),
    },
    [ABILITY_GALVANIZE] =
    {
        .name = _("Galvanize"),
        .description = COMPOUND_STRING("Normal-type moves become Electric-type moves.\nThe power of those moves is boosted a little."),
    },
    [ABILITY_SURGE_SURFER] =
    {
        .name = _("Surge Surfer"),
        .description = COMPOUND_STRING("Doubles the Pokémon's Speed stat on Electric\nTerrain."),
    },
    [ABILITY_SCHOOLING] =
    {
        .name = _("Schooling"),
        .description = COMPOUND_STRING("When it has a lot of HP, the Pokémon forms a\npowerful school. Stoping when its HP is low."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },
    [ABILITY_DISGUISE] =
    {
        .name = _("Disguise"),
        .description = COMPOUND_STRING("Once per battle, the shroud that covers the\nPokémon can protect it from an attack."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
		.breakable = TRUE,
    },
    [ABILITY_BATTLE_BOND] =
    {
        .name = _("Battle Bond"),
#if BATTLE_BOND_TRANSFORM
        .description = COMPOUND_STRING("When the Pokémon knocks out a target, it\nbecomes Ash-Greninja."),
#else
        .description = COMPOUND_STRING("When the Pokémon knocks out a target, its\nAttack, Sp. Atk, and Speed stats are boosted."),
#endif
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },
    [ABILITY_POWER_CONSTRUCT] =
    {
        .name = _("Power Construct"),
        .description = COMPOUND_STRING("Cells gather to aid when its HP becomes half\nor less. Changing its form to Complete Forme."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },
    [ABILITY_CORROSION] =
    {
        .name = _("Corrosion"),
        .description = COMPOUND_STRING("The Pokémon can poison the target even if it's\na Steel or Poison type."),
    },
    [ABILITY_COMATOSE]  =
	{
		.name = _("Comatose"),
		.description = COMPOUND_STRING("The Pokémon is always drowsing and will never\nwake up. It can attack while sleeping."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
	},
    [ABILITY_QUEENLY_MAJESTY] =
    {
        .name = _("Queenly Majesty"),
        .description = COMPOUND_STRING("Its majesty pressures the foes, making them\nunable to attack using priority moves."),
		.breakable = TRUE,
    },
    [ABILITY_INNARDS_OUT] =
    {
        .name = _("Innards Out"),
        .description = COMPOUND_STRING("Damages the attacker landing the finishing hit\nby the amount equal to its last HP."),
    },
    [ABILITY_DANCER] =
    {
        .name = _("Dancer"),
        .description = COMPOUND_STRING("When another Pokémon uses a “dance” move, you\ncan use a move following it."),
    },
    [ABILITY_BATTERY] =
    {
        .name = _("Battery"),
        .description = COMPOUND_STRING("Powers up ally Pokémon's special moves."),
    },
    [ABILITY_FLUFFY] =
    {
        .name = _("Fluffy"),
        .description = COMPOUND_STRING("Halves the damage taken from moves that make\ncontact, but doubles that of Fire-type moves."),
		.breakable = TRUE,
    },
    [ABILITY_DAZZLING] =
    {
        .name = _("Dazzling"),
        .description = COMPOUND_STRING("The Pokémon dazzles its foes, making them\nunable to attack using priority moves."),
		.breakable = TRUE,
    },
    [ABILITY_SOUL_HEART] =
    {
        .name = _("Soul-Heart"),
        .description = COMPOUND_STRING("Boosts the Pokémon's Sp. Atk stat every time\nanother Pokémon faints."),
    },
    [ABILITY_TANGLING_HAIR] =
    {
        .name = _("Tangling Hair"),
        .description = COMPOUND_STRING("Contact with the Pokémon lowers the\nattacker's Speed stat."),
    },
    [ABILITY_RECEIVER] =
    {
        .name = _("Receiver"),
        .description = sReceiverDescription,
		.cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },
    [ABILITY_POWER_OF_ALCHEMY] =
    {
        .name = _("Power of Alchemy"),
        .description = sReceiverDescription,
		.cantBeCopied = TRUE,
        .cantBeTraced = TRUE,
    },
    [ABILITY_BEAST_BOOST] =
    {
        .name = _("Beast Boost"),
        .description = COMPOUND_STRING("Boosts the Pokémon's most proficient stat\nevery time it knocks out a target."),
    },
    [ABILITY_RKS_SYSTEM] =
    {
        .name = _("RKS System"),
        .description = COMPOUND_STRING("Changes the Pokémon's type to match the\nmemory disc it holds."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },
    [ABILITY_ELECTRIC_SURGE] =
    {
        .name = _("Electric Surge"),
        .description = COMPOUND_STRING("Turns the ground into Electric Terrain when\nthe Pokémon enters a battle."),
    },
    [ABILITY_PSYCHIC_SURGE] =
    {
        .name = _("Psychic Surge"),
        .description = COMPOUND_STRING("Turns the ground into Psychic Terrain when\nthe Pokémon enters a battle."),
    },
    [ABILITY_MISTY_SURGE] =
    {
        .name = _("Misty Surge"),
        .description = COMPOUND_STRING("Turns the ground into Misty Terrain when the\nPokémon enters a battle."),
    },
    [ABILITY_GRASSY_SURGE] =
    {
        .name = _("Grassy Surge"),
        .description = COMPOUND_STRING("Turns the ground into Grassy Terrain when the\nPokémon enters a battle."),
    },
    [ABILITY_FULL_METAL_BODY] =
    {
        .name = _("Full Metal Body"),
        .description = sClearBodyDescription,
    },
    [ABILITY_SHADOW_SHIELD] =
    {
        .name = _("Shadow Shield"),
        .description = sMultiscaleDescription,
    },
    [ABILITY_PRISM_ARMOR] =
    {
        .name = _("Prism Armor"),
        .description = sFilterDescription,
    },
    [ABILITY_NEUROFORCE] =
    {
        .name = _("Neuroforce"),
        .description = COMPOUND_STRING("Powers up the Pokémon's “super effective”\nattacks even further."),
    },
    [ABILITY_INTREPID_SWORD] =
    {
        .name = _("Intrepid Sword"),
        .description = COMPOUND_STRING("Boosts the Pokémon's Attack stat the first\ntime the Pokémon enters a battle."),
    },
    [ABILITY_DAUNTLESS_SHIELD] =
    {
        .name = _("Dauntless Shield"),
        .description = COMPOUND_STRING("Boosts the Pokémon's Defense stat the first\ntime the Pokémon enters a battle."),
    },
    [ABILITY_LIBERO] =
    {
        .name = _("Libero"),
        .description = sProteanDescription,
    },
    [ABILITY_BALL_FETCH] =
    {
        .name = _("Ball Fetch"),
        .description = COMPOUND_STRING("If the Pokémon not have an item, it will fetch\nthe Poké Ball from the first failed throw."),
    },
    [ABILITY_COTTON_DOWN] =
    {
        .name = _("Cotton Down"),
        .description = COMPOUND_STRING("When attacked, it scatters cotton around and\nlowers the Speed stats of all on the field."),
    },
    [ABILITY_PROPELLER_TAIL] =
    {
        .name = _("Propeller Tail"),
        .description = sPropellerTailDescription,
    },
    [ABILITY_MIRROR_ARMOR] =
    {
        .name = _("Mirror Armor"),
        .description = COMPOUND_STRING("Bounces back only the stat-lowering effects\nthat the Pokémon receives."),
		.breakable = TRUE,
    },
    [ABILITY_GULP_MISSILE] =
    {
        .name = _("Gulp Missile"),
        .description = COMPOUND_STRING("When it uses Surf or Dive, it will come back\nwith a prey. When attacked spits it out."),
		.cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },
    [ABILITY_STALWART] =
    {
        .name = _("Stalwart"),
        .description = sPropellerTailDescription,
    },
    [ABILITY_STEAM_ENGINE] =
    {
        .name = _("Steam Engine"),
        .description = COMPOUND_STRING("Boosts the Speed stat drastically when the\nPokémon is hit by a Fire or Water-type move."),
    },
    [ABILITY_PUNK_ROCK] =
    {
        .name = _("Punk Rock"),
        .description = COMPOUND_STRING("Boosts the power of sound-based moves. Also\ntakes half the damage from these moves."),
		.breakable = TRUE,
    },
    [ABILITY_SAND_SPIT] =
    {
        .name = _("Sand Spit"),
        .description = COMPOUND_STRING("The Pokémon creates a sandstorm when it's hit\nby an attack."),
    },
    [ABILITY_ICE_SCALES] =
    {
        .name = _("Ice Scales"),
        .description = COMPOUND_STRING("The Pokémon is protected by ice scales, which\nhalve the damage taken from special moves."),
		.breakable = TRUE,
    },
    [ABILITY_RIPEN] =
    {
        .name = _("Ripen"),
        .description = COMPOUND_STRING("Ripens Berries and doubles their effect."),
    },
    [ABILITY_ICE_FACE] =
    {
        .name = _("Ice Face"),
        .description = COMPOUND_STRING("It's ice head can take a physical attack as a\nsubstitute, but it changes it's appearance."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
        .breakable = TRUE,
    },
    [ABILITY_POWER_SPOT] =
    {
        .name = _("Power Spot"),
        .description = COMPOUND_STRING("Just being next to the Pokémon powers up\nmoves."),
    },
    [ABILITY_MIMICRY] =
    {
        .name = _("Mimicry"),
        .description = COMPOUND_STRING("Changes the Pokémon's type depending on the\nterrain."),
    },
    [ABILITY_SCREEN_CLEANER] =
    {
        .name = _("Screen Cleaner"),
        .description = COMPOUND_STRING("When the Pokémon enters a battle, the effects\nof screens are nullified for both side."),
    },
    [ABILITY_STEELY_SPIRIT] =
    {
        .name = _("Steely Spirit"),
        .description = COMPOUND_STRING("Powers up the Steel-type moves of the Pokémon\nand its allies."),
    },
    [ABILITY_PERISH_BODY] =
    {
        .name = _("Perish Body"),
        .description = COMPOUND_STRING("When hit by a move that makes contact, he and\nthe attacker will faint after three turns."),
    },
    [ABILITY_WANDERING_SPIRIT] =
    {
        .name = _("Wandering Spirit"),
        .description = COMPOUND_STRING("The Pokémon exchanges Abilities with a Pokémon\nthat makes direct contact with it."),
    },
    [ABILITY_GORILLA_TACTICS] =
    {
        .name = _("Gorilla Tactics"),
        .description = COMPOUND_STRING("Boosts the Pokémon's Attack stat but only\nallows the use of the first selected move."),
    },
    [ABILITY_NEUTRALIZING_GAS] =
    {
        .name = _("Neutralizing Gas"),
        .description = COMPOUND_STRING("While it's in the battle, the effects of all\nother Pokémon's Abilities will be nullified."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },
    [ABILITY_PASTEL_VEIL] =
    {
        .name = _("Pastel Veil"),
        .description = COMPOUND_STRING("Prevents the Pokémon and its allies from being\npoisoned."),
		.breakable = TRUE,
    },
    [ABILITY_HUNGER_SWITCH] =
    {
        .name = _("Hunger Switch"),
        .description = COMPOUND_STRING("The Pokémon changes between its Full Belly and\nHangry Mode after the end of every turn."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },
    [ABILITY_QUICK_DRAW] =
    {
        .name = _("Quick Draw"),
        .description = COMPOUND_STRING("Enables the Pokémon to move first\noccasionally."),
    },
    [ABILITY_UNSEEN_FIST] =
    {
        .name = _("Unseen Fist"),
        .description = COMPOUND_STRING("If it use moves that make contact, it can\nattack even if the target protects itself."),
    },
    [ABILITY_CURIOUS_MEDICINE] =
    {
        .name = _("Curious Medicine"),
        .description = COMPOUND_STRING("When the Pokémon enters a battle, its shell's\nmedicine removes all stat changes from allies."),
    },
    [ABILITY_TRANSISTOR] =
    {
        .name = _("Transistor"),
        .description = COMPOUND_STRING("Powers up Electric-type moves."),
    },
    [ABILITY_DRAGONS_MAW] =
    {
        .name = _("Dragon's Maw"),
        .description = COMPOUND_STRING("Powers up Dragon-type moves."),
    },
    [ABILITY_CHILLING_NEIGH] =
    {
        .name = _("Chilling Neigh"),
        .description = COMPOUND_STRING("When it knocks out a target, it utters a\nchilling neigh, boosting its Attack stat."),
    },
    [ABILITY_GRIM_NEIGH] =
    {
        .name = _("Grim Neigh"),
        .description = COMPOUND_STRING("When it knocks out a target, it utters a\nterrifying neigh, boosting its Sp. Atk stat."),
    },
    [ABILITY_AS_ONE_ICE_RIDER] =
    {
        .name = _("As One"),
        .description = COMPOUND_STRING("This Ability combines the effects of Calyrex's\nUnnerve and Glastrier's Chilling Neigh."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },
    [ABILITY_AS_ONE_SHADOW_RIDER] =
    {
        .name = _("As One"),
        .description = COMPOUND_STRING("This Ability combines the effects of Calyrex's\nUnnerve and Spectrier's Grim Neigh."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },
    [ABILITY_LINGERING_AROMA] =
    {
        .name = _("Lingering Aroma"),
        .description = COMPOUND_STRING("Contact with the Pokémon changes the\nattacker's Ability to Lingering Aroma."),
    },
    [ABILITY_SEED_SOWER] =
    {
        .name = _("Seed Sower"),
        .description = COMPOUND_STRING("Turns the ground into Grassy Terrain when the\nPokémon is hit by an attack."),
    },
    [ABILITY_THERMAL_EXCHANGE] =
    {
        .name = _("Thermal Exchange"),
        .description = COMPOUND_STRING("Boosts the Attack stat when hit by a Fire-type\nmove. It also cannot be burned."),
		.breakable = TRUE,
    },
    [ABILITY_ANGER_SHELL] =
    {
        .name = _("Anger Shell"),
        .description = COMPOUND_STRING("When low HP, its defensive stats are lowered\nbut boosts its ofensive stats and its Speed."),
    },
    [ABILITY_PURIFYING_SALT] =
    {
        .name = _("Purifying Salt"),
        .description = COMPOUND_STRING("Its pure salt protects it from status and\nhalves the damage from Ghost-type moves."),
		.breakable = TRUE,
    },
    [ABILITY_WELL_BAKED_BODY] =
    {
        .name = _("Well-Baked Body"),
        .description = COMPOUND_STRING("It takes no damage by Fire-type moves.\nInstead, its Defense stat is sharply boosted."),
		.breakable = TRUE,
    },
    [ABILITY_WIND_RIDER] =
    {
        .name = _("Wind Rider"),
        .description = COMPOUND_STRING("Boosts it's Attack stat if Tailwind takes\neffect or if hit by a “wind” move."),
		.breakable = TRUE,
    },
    [ABILITY_GUARD_DOG] =
    {
        .name = _("Guard Dog"),
        .description = COMPOUND_STRING("Geting intimidated boosts its Attack stat. It\nalso cannot be forced to switch out."),
		.breakable = TRUE,
    },
    [ABILITY_ROCKY_PAYLOAD] =
    {
        .name = _("Rocky Payload"),
        .description = COMPOUND_STRING("Powers up Rock-type moves."),
    },
    [ABILITY_WIND_POWER] =
    {
        .name = _("Wind Power"),
        .description = COMPOUND_STRING("Becomes charged when hit by a “wind” move, it\nboosts the next Electric-type move's power."),
    },
    [ABILITY_ZERO_TO_HERO] =
    {
        .name = _("Zero to Hero"),
        .description = COMPOUND_STRING("The Pokémon transforms into its Hero Form when\nit switches out."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
    },
    [ABILITY_COMMANDER] =
    {
        .name = _("Commander"),
        .description = COMPOUND_STRING("When it enters a battle, it goes inside of an\nally Dondozo and commands from there."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },
    [ABILITY_ELECTROMORPHOSIS] =
    {
        .name = _("Electromorphosis"),
        .description = COMPOUND_STRING("Becomes charged when it takes damage, it\nboosts the next Electric-type move's power."),
    },
    [ABILITY_PROTOSYNTHESIS] =
    {
        .name = _("Protosynthesis"),
        .description = COMPOUND_STRING("Boosts the Pokémon's most proficient stat in\nharsh sunlight or if holding Booster Energy."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },
    [ABILITY_QUARK_DRIVE] =
    {
        .name = _("Quark Drive"),
        .description = COMPOUND_STRING("Boosts the Pokémon's most proficient stat on\nElectric Terrain or if holding Booster Energy."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
    },
    [ABILITY_GOOD_AS_GOLD] =
    {
        .name = _("Good as Gold"),
        .description = COMPOUND_STRING("A body of pure, solid gold gives the Pokémon\nfull immunity to other Pokémon's status moves."),
		.breakable = TRUE,
    },
    [ABILITY_VESSEL_OF_RUIN] =
    {
        .name = _("Vessel of Ruin"),
        .description = COMPOUND_STRING("The power of its ruinous vessel lowers the\nSp. Atk stats of all Pokémon except itself."),
		.breakable = TRUE,
    },
    [ABILITY_SWORD_OF_RUIN] =
    {
        .name = _("Sword of Ruin"),
        .description = COMPOUND_STRING("The power of its ruinous sword lowers the\nDefense stats of all Pokémon except itself."),
		.breakable = TRUE,
    },
    [ABILITY_TABLETS_OF_RUIN] =
    {
        .name = _("Tablets of Ruin"),
        .description = COMPOUND_STRING("The power of its ruinous wooden tablets lowers\nthe Attack stats of all Pokémon except itself."),
		.breakable = TRUE,
    },
    [ABILITY_BEADS_OF_RUIN] =
    {
        .name = _("Beads of Ruin"),
        .description = COMPOUND_STRING("The power of its ruinous beads lowers the\nSp. Def stats of all Pokémon except itself."),
		.breakable = TRUE,
    },
    [ABILITY_ORICHALCUM_PULSE] =
    {
        .name = _("Orichalcum Pulse"),
        .description = COMPOUND_STRING("Turns the sunlight harsh when enters a battle,\nboosting its Attack stat."),
    },
    [ABILITY_HADRON_ENGINE] =
    {
        .name = _("Hadron Engine"),
        .description = COMPOUND_STRING("Turns the ground into Electric Terrain when\nenters a battle, boosting its Sp. Atk stat."),
    },
    [ABILITY_OPPORTUNIST] =
    {
        .name = _("Opportunist"),
        .description = COMPOUND_STRING("If an foe's stat is boosted, it seizes the\nopportunity to boost the same stat for itself."),
    },
    [ABILITY_CUD_CHEW] =
    {
        .name = _("Cud Chew"),
        .description = COMPOUND_STRING("When it eats a Berry, it will regurgitate that\nBerry in the next turn and eat it again."),
    },
    [ABILITY_SHARPNESS] =
    {
        .name = _("Sharpness"),
        .description = COMPOUND_STRING("Powers up “slicing” moves."),
    },
    [ABILITY_SUPREME_OVERLORD] =
    {
        .name = _("Supreme Overlord"),
        .description = COMPOUND_STRING("When it enters a battle, boosts its Attack and\nSp. Atk stats for each fainted party member."),
    },
    [ABILITY_COSTAR] =
    {
        .name = _("Costar"),
        .description = COMPOUND_STRING("When the Pokémon enters a battle, it copies an\nally's stat changes."),
    },
    [ABILITY_TOXIC_DEBRIS] =
    {
        .name = _("Toxic Debris"),
        .description = COMPOUND_STRING("Scatters poison spikes on the foe's feet when\nthe Pokémon takes damage from physical moves."),
    },
    [ABILITY_ARMOR_TAIL] =
    {
        .name = _("Armor Tail"),
        .description = COMPOUND_STRING("It's mysterious tail makes opponents unable to\nuse priority moves against it or its allies."),
		.breakable = TRUE,
    },
    [ABILITY_EARTH_EATER] =
    {
        .name = _("Earth Eater"),
        .description = COMPOUND_STRING("If hit by a Ground-type move, the Pokémon has\nits HP restored instead of taking damage."),
		.breakable = TRUE,
    },
    [ABILITY_MYCELIUM_MIGHT] =
    {
        .name = _("Mycelium Might"),
        .description = COMPOUND_STRING("Acts more slowly with status moves, but these\nmoves can't be impeded by the foe's Ability."),
    },
	[ABILITY_HOSPITALITY] =
	{
		.name = _("Hospitality"),
		.description = COMPOUND_STRING("When the Pokémon enters a battle, it showers\nits ally with hospitality, restoring its HP."),
	},
	[ABILITY_MINDS_EYE] =
	{
		.name = _("Mind's Eye"),
		.description = COMPOUND_STRING("Ignores changes to foe's evasiveness, its\naccuracy can't be lowered, and hit Ghost types."),
		.breakable = TRUE,
	},
	[ABILITY_EMBODY_ASPECT] =
	{
		.name = _("Embody Aspect"),
		.description = COMPOUND_STRING("Its heart fills with memories, causing its\nMask to shine and boost the Pokémon's stat."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
	},
	[ABILITY_TOXIC_CHAIN] =
	{
		.name = _("Toxic Chain"),
		.description = COMPOUND_STRING("The Pokémon's toxic chain may badly poison\nany target the Pokémon hits with a move."),
	},
	[ABILITY_SUPERSWEET_SYRUP] =
	{
		.name = _("Supersweet Syrup"),
		.description = COMPOUND_STRING("When it enters a battle, a sickly sweet scent\nlowers the evasiveness of opposing Pokémon."),
	},
	[ABILITY_TERA_SHIFT] =
	{
		.name = _("Tera Shift"),
		.description = COMPOUND_STRING("When it enters a battle, it absorbs the energy\naround and transforms into its Terastal Form."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .cantBeSuppressed = TRUE,
        .cantBeOverwritten = TRUE,
	},
	[ABILITY_TERA_SHELL] =
	{
		.name = _("Tera Shell"),
		.description = COMPOUND_STRING("All damage-dealing moves that hit the Pokémon\nwhen its HP is full will not be very effective."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
        .breakable = TRUE,
	},
	[ABILITY_TERAFORM_ZERO] =
	{
		.name = _("Teraform Zero"),
		.description = COMPOUND_STRING("When it changes to its Stellar Form, its power\neliminate all effects of weather and terrain."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
	},
	[ABILITY_POISON_PUPPETEER] =
	{
		.name = _("Poison Puppeteer"),
		.description = COMPOUND_STRING("Pokémon poisoned by its moves will also\nbecome confused."),
		.cantBeCopied = TRUE,
        .cantBeSwapped = TRUE,
        .cantBeTraced = TRUE,
	},
};

// If this setting is enabled, it blocks the experience gain based on the number of badges you have.
// To do so, go to "src/battle_script_commands.c" and edit the levels for each badge in "sExpBlockLevels".
#define EXP_BLOCK TRUE

// If this setting is enabled, Pokémons will not faint due to poison while in the overworld.
#define POISON_SURVIVAL TRUE

// If this setting is enabled, the sleep counter for Pokémon lasts for 1-3 turns instead of 2-5.
#define SLEEP_UPDATE TRUE

// If this setting is enabled, the speed reduction from paralysis for Pokémon will be 50% instead of 75%.
#define PARALYSIS_UPDATE TRUE

// If this setting is enabled, the amount of damage caused by burn for Pokémon will be 1/16 of their maximum HP instead of 1/8.
#define BURN_UPDATE TRUE

// If this setting is enabled, the chance of a Pokémon being damaged by confusion becomes 33% instead of 50%.
#define CONFUSION_UPDATE TRUE

// If this setting is enabled, it will apply the boosts given by each badge.
#define BADGE_BOOST TRUE

// If this setting is enabled, when you overwrite a move with another move that has a higher PP, the remaining PP of the original move will be retained.
#define DONT_REPLENISH_MOVE_PP TRUE

// If this setting is enabled, all participating Pokémon in a battle will receive full experience points.
#define FULL_EXP_FOR_PARTICIPATING TRUE

// If this setting is enabled, Pichu eggs will hatch with the Volt Tackle move.
#define VOLT_TACKLE_BY_BREEDING TRUE

// If this setting is enabled, some species's eggs will change depending if it's holding a specific incense item.
#define INCENSE_ITEM_BREEDING TRUE

// If this setting is enabled, eggs will inherit any TM the father knows like prior to Pokémon X and Y.
#define INHERIT_TMS FALSE

// This is the percentage chance that a wild Pokémon will appear with its hidden ability, if it has one.
// If the value is set to 0, then the Pokémon will not appear with its hidden ability.
#define WILD_HIDDEN_ABILITY_CHANCE 5

// This is the percentage chance that a Shuckle in the player's party will create a Berry Juice from its held berry at the end of each battle,
// similar to how it worked in Generation II. If the value is set to 0, then the Shuckle will not create a Berry Juice.
#define SHUCKLE_MAKES_BERRY_JUICE_CHANCE 25

// If this setting is enabled, sunny weather in battle will boost the speed of Grass-type Pokémon by 33%, similar to the effect in Pokémon Legends Arceus.
#define SUN_BOOST_SPEED TRUE

// If this setting is enabled, hail weather in battle will boost the speed of Ice-type Pokémon by 33%, similar to the effect in Pokémon Scarlet and Violet.
#define HAIL_BOOST_SPEED TRUE

// If this setting is enabled, hail weather in battle will boost the defense of Ice-type Pokémon by 50%, similar to the effect in Pokémon Legends Arceus.
#define HAIL_BOOST_DEFENSE TRUE

// If this setting is enabled, the overworld weather conditions WEATHER_FOG_HORIZONTAL and WEATHER_FOG_DIAGONAL will cause a foggy weather effect in battle,
// which will decrease the accuracy of moves.
#define FOG_IN_BATTLE TRUE

// This is the percentage chance that a wild battle will become a wild double battle. If the value is set to 0,
// then a wild double battle will never occur unless it is called by a scripted wild battle.
#define DOUBLE_WILD_BATTLE_CHANCE 5

// If this setting is enabled, in a double wild battle where two natural enemies are present, the opposing Pokémon will attack each other
// instead of the player's Pokémon. To do so, go to "src/battle_controller_opponent.c" and edit the natural enemies in "sNaturalEnemySpecies".
#define DOUBLE_WILD_ATTACK_NATURAL_ENEMY TRUE

// If this setting is enabled, when a Pokémon's trainer blocks a Poké Ball, the item will not be lost and will return to the player's bag,
// similar to the behavior in Generation IV and onwards.
#define RESTORE_BALL_TRAINER_BLOCK TRUE

// If this setting is enabled, when using a medicine item on a Pokémon, it will remain in the party menu instead of returning to the bag.
#define REPEATED_MEDICINE_USE TRUE

// If this setting is enabled, pressing the L button while choosing a move will display its information.
#define BATTLE_MOVE_INFO TRUE

// This setting determines the number of submenus to display move information.
#define NUM_MOVEINFO_SUBMENUS 3

// If this setting is enabled, the naming screen will automatically switch to lower case when you start typing the first character.
#define PAGE_SWAP TRUE

// If this setting is enabled, the Pokémon Center animation will not count unhatched eggs.
#define NO_HEAL_EGGS TRUE

// If this setting is enabled, the color of a move's type name will change based on its effectiveness in battle.
// To do so, go to "src/battle_controller_player.c" and edit each effectiveness's color in "sEffectivenessColours".
#define EFFECTIVENESS_ON_MENU TRUE

// If this setting is enabled, the stats in the summary screen will be colored based on the Pokémon's nature.
#define NATURE_COLOURS TRUE

// If this setting is enabled, a ranking based on your Pokémon's IVs is displayed in the summary screen.
#define IVS_IN_THE_SUMMARY TRUE

// If this setting is enabled, the icons of the party Pokémon will be displayed on the back of the trainer card.
#define MON_ICON_ON_TRAINER_CARD TRUE

// If this setting is enabled, when choosing the player name, cancelling the selection will return the player to the gender selection screen
// instead of the name selection screen.
#define EM_STYLE_GENDER_SELECT_INTRO TRUE

// If this setting is enabled, it automatically starts a new game if not saved before, otherwise it will display the new game/options menu like EM.
#define AUTO_INIT_NEW_GAME TRUE

// If this setting is enabled, an icon will be created during battle to show the current weather condition.
#define WEATHER_ICON_IN_BATTLE TRUE

// If this setting is enabled, the day and night system will take affect in battles.
#define USE_DNS_IN_BATTLE TRUE

// If this setting is enabled, the windows of the house will light up at night time.
#define LIT_UP_WINDOWS TRUE

// If this setting is enabled, the captured Pokémon of trainers will have their Poké Ball based on their trainer class.
// Otherwise, the Poké Ball will be customizable in the trainer data.
#define CLASS_BASED_BALLS TRUE

// If this setting is enabled, the captured Pokémon of trainers will have their level based on the player's party Pokémon with highest level.
// Otherwise, the level will be customizable in the trainer data.
#define DYNAMIC_LEVEL TRUE

// This setting determines the price divisor when selling an item. It's 2 in Generation III games, but 4 in Pokémon Legends Arceus. 
#define ITEM_SELL_DIVISOR 2

// If this setting is enabled, when searching for a Pokémon on the Dexnav you must need to press the A button to creeping.
// Otherwise, the Pokémon will run away.
#define DEXNAV_CREEPING_SEARCH TRUE

// If this setting is enabled, random weathers will start on enter maps that don't have a weather defined.
// Those weather are choose based on current time and season.
#define DYNAMIC_WEATHER TRUE

// If this setting is enabled, when a Greninja knocks out a target with a move, it will transform itself into Ash-Greninja.
// Otherwise, it will increase it's Attack, Sp. Attack and Speed by one stage each.
#define BATTLE_BOND_TRANSFORM TRUE

// Value of friendship the Pokémon must have to trigger its evolution, in Generation IX onward it was decreased from 220 to 160.
#define FRIENDSHIP_EVO_THRESHOLD 220

// If this setting is enabled, evolutions will be able to requires a moon phase in order to evolve.
// Otherwise, this requirement will be ignored if set.
#define MOON_PHASE_EVO_REQUIREMENT TRUE

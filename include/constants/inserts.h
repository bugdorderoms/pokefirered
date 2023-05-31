// engine changes
#define EXP_BLOCK TRUE // if true it blocks the Exp gain based on the number of badges you has. Go to src/battle_script_commands.c and edit the levels for each badge in sExpBlockLevels.
#define POISON_SURVIVAL TRUE // if true the pokemons don't die from poison in the overworld.
#define SLEEP_UPDATE TRUE // if true the sleep counter last for 1-3 turns instead of 2-5.
#define PARALYSIS_UPDATE TRUE // if true the speed drop of paralysis will be 50% instead of 75%.
#define BURN_UPDATE TRUE // if true the burn damage will be 1/16 of max HP instead of 1/8.
#define CONFUSION_UPDATE TRUE // if true the chance that the pokemon will be damaged by confusion becames 33% instead of 50%.
#define BADGE_BOOST TRUE // if true it applies the boosts given by each badge.
#define DONT_REPLENISH_MOVE_PP TRUE // if true when you overwrite a move with another with higher PP the actual PP will Remains.
#define DELETABLE_HMS TRUE // if true all HM moves will be deleteable like any other TM.
#define REMOVE_FLASHBACKS TRUE // if true it removes the flashbacks when load a saved game.
#define FULL_EXP_FOR_PARTICIPATING TRUE // if true it gives full exp for the participating in the battle.
#define VOLT_TACKLE_BY_BREEDING TRUE // if true when a pichu egg is hatched it will come with the volt tackle move.
#define WILD_HIDDEN_ABILITY_CHANCE 5 // % chance that a wild pokemon has to appears with it's hidden ability, if it has one. If 0 it will't appears with it's hidden ability.
#define SHUCKLE_MAKES_BERRY_JUICE_CHANCE 25 // % chance that a Shuckle on the player's party makes a Berry Juice from it's held berry at the end of each battle, like in gen2. If 0 it will't crete a Berry Juice.
#define SUN_BOOST_SPEED TRUE // if true the sunny weathers boost the speed of grass type pokemons in battle by 33%, like legends Arceus.
#define HAIL_BOOST_SPEED TRUE // if true the hail weather boost the speed of ice type pokemons in battle by 33%, like SV.
#define HAIL_BOOST_DEFENSE TRUE // if true the hail weather boost the defense of ice type pokemons in battle by 50%, like legends Arceus.
#define FOG_IN_BATTLE TRUE // if true the overworld weathers WEATHER_FOG_HORIZONTAL and WEATHER_FOG_DIAGONAL will start weather fog in battle, like rain and snow.
#define DOUBLE_WILD_BATTLE_CHANCE 5 // % chance that a wild battle become into a wild double battle. If 0 it will never start a wild double battle, unless it's called by an scripted wild battle.
#define DOUBLE_WILD_ATTACK_NATURAL_ENEMY TRUE // if true when a double wild battle occours with two natural enemys, it will attack it's enemy instead of the player's pokemon.

// grafical changes
#define PAGE_SWAP TRUE // if true it auttomaticaly swap the naming screen page to lower case on write the first caracter.
#define NO_HEAL_EGGS TRUE // if true the pokemon center animation don't count eggs.
#define EFFECTIVENESS_ON_MENU TRUE // if true the move type name trade its color based on the move effectiveness in battle.
#define NATURE_COLOURS TRUE // if true it colours the stat in the summary screen depending on the nature.
#define IVS_IN_THE_SUMMARY TRUE // if true a raking based on your poke's Iv is showed in the summary screen. Go to graphics/interface and change the dex_pokeball,_types,_category_and_IVs_ranking.png to you liking.
#define MON_ICON_ON_TRAINER_CARD TRUE // if true it draw the party mon icons on the trainer card back.
#define DISAPPEAR_HP_BAR_DURING_ANIMS TRUE // if true the hp bars disppears when a move anim is displayed.
#define EM_STYLE_GENDER_SELECT_INTRO TRUE // if true giving the player name if you cancel it the game will returns to choose the player gender instead of the name again.
#define MON_ICON_ON_CONTINUE_SCREEN TRUE // if true it draw the party mon icons on the continue saved game screen.
#define WEATHER_ICON_IN_BATTLE TRUE // if true it create an icon in battle showing the current weather it is in.

#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// You can run the game with ./gamefile.exe <filehere> | <filehere> is only csv formatted file for the game. Try savegame.text.
void flush_input() // this code just 'flushes' the buffer to prevent overflow. Maybe unnecessary?
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) // use ' ' for \n
        ;
}

typedef enum
{
    ALIVE,
    DEAD
} PlayerStatus;

typedef struct
{
    char class[50];
    PlayerStatus status;
    int hp;
    int maxhp;
    int level;
    int gold;
    int moxie;
    int magic;
    int str;
    int mana;
    int maxmana;
    struct
    {
        char itemName[50];
        int quantity;
        char location[50];
        char spells[50];
        int check;
        int wins;
        int loss;
    } items[10]; // Struct inside another struct. As of now, we only have 10 items. Player.items[0] = first item!
} Player;

void save_game(Player *player, const char *filename, const char *csvfilename)
{
    FILE *file = fopen(filename, "w");
    FILE *csvfile = fopen(csvfilename, "w");

    if (file == NULL || csvfile == NULL)
    {
        perror("Error opening file");
        return;
    }

    // Writing header to textfile
    fprintf(file, "Class,Item,Howmany,HP,MaxHP,Gold,Level,Moxie,Magic,Str,Mana,MaxMana,Spells,Check,Location,Wins,Loss\n");

    // Writing header to csvfile
    fprintf(csvfile, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
            "Class", "Item", "Howmany", "HP", "MaxHP", "Gold", "Level",
            "Moxie", "Magic", "Str", "Mana", "MaxMana", "Spells",
            "Check", "Location", "Wins", "Loss");

    // Writing player data to file
    for (int i = 0; i < 10; i++)
    {
        if (i == 0)
        {
            // // Write class and HP only once, in the first line
            // fprintf(file, "%s,%s,%d,%d\n", player->class, player->items[i].itemName, player->items[i].quantity, player->hp);

            // Writing player data to csvfile, this code below will only work if there is a game logic to update location, spells, check, wins, loss, otherwise it will just output the address of the struct

            // Write class and HP only once, in the first line
            fprintf(file, "%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%d,%s,%d,%d\n",
                    player->class,
                    player->items[i].itemName,
                    player->items[i].quantity,
                    player->hp,
                    player->maxhp,
                    player->gold,
                    player->level,
                    player->moxie,
                    player->magic,
                    player->str,
                    player->mana,
                    player->maxmana,
                    player->items[i].spells,
                    player->items[i].check,
                    player->items[i].location,
                    player->items[i].wins,
                    player->items[i].loss);

            // Writing player data to csvfile, this code below will only work if there is a game logic to update location, spells, check, wins, loss, otherwise it will just output the address of the struct
            // fprintf(csvfile, "%s,%s,%d,%d,%s,%s,%s,%d,%d,%d\n",
            //         player->class, player->items[i].itemName, player->items[i].quantity, player->hp,
            //         player->location, player->spells, player->check, player->level, player->wins, player->loss);

            fprintf(csvfile, "%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%d,%s,%d,%d\n",
                    player->class,
                    player->items[i].itemName,
                    player->items[i].quantity,
                    player->hp,
                    player->maxhp,
                    player->gold,
                    player->level,
                    player->moxie,
                    player->magic,
                    player->str,
                    player->mana,
                    player->maxmana,
                    player->items[i].spells,
                    player->items[i].check,
                    player->items[i].location,
                    player->items[i].wins,
                    player->items[i].loss);
        }
        else
        {
            // Write only item details in subsequent lines
            fprintf(file, "-,%s,%d,-,-,-,-,-,-,-,-,-,%s,%d,%s,%d,%d\n", player->items[i].itemName, player->items[i].quantity, player->items[i].spells, player->items[i].check, player->items[i].location, player->items[i].wins, player->items[i].loss);

            fprintf(csvfile, "-,%s,%d,-,-,-,-,-,-,-,-,-,%s,%d,%s,%d,%d\n", player->items[i].itemName, player->items[i].quantity, player->items[i].spells, player->items[i].check, player->items[i].location, player->items[i].wins, player->items[i].loss);
        }
    }

    fclose(file);
    fclose(csvfile);
}

int load_game(Player *player, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return -1;
    }

    char buffer[128];
    // Skipping the header
    fgets(buffer, 128, file);

    // Reading first line with class, HP, MaxHP...MaxMana
    fscanf(file, "%49[^,],%49[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%49[^,],%d,%49[^,],%d,%d\n",
           player->class,              // %s - string
           player->items[0].itemName,  // %s - string
           &player->items[0].quantity, // %d - integer
           &player->hp,                // %d - integer
           &player->maxhp,             // %d - integer
           &player->level,             // %d - integer
           &player->gold,              // %d - integer
           &player->moxie,             // %d - integer
           &player->magic,             // %d - integer
           &player->str,               // %d - integer
           &player->mana,              // %d - integer
           &player->maxmana,           // %d - integer
           player->items[0].spells,    // %s - string
           &player->items[0].check,    // %d - integer
           player->items[0].location,  // %s - string
           &player->items[0].wins,     // %d - integer
           &player->items[0].loss);    // %d - integer

    // Reading remaining item details
    for (int i = 1; i < 10; i++)
    {
        fscanf(file, "%*[^,],%49[^,],%d,%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%49[^,],%d,%49[^,],%d,%d\n",
               player->items[i].itemName,  // %s - string
               &player->items[i].quantity, // %d - integer
               // Skipping 9 integer fields
               player->items[i].spells,   // %s - string
               &player->items[i].check,   // %d - integer
               player->items[i].location, // %s - string
               &player->items[i].wins,    // %d - integer
               &player->items[i].loss);   // %d - integer
    }

    fclose(file);
    return 0;
}

void print_spell(Player *player)
{
    int i;
    int j;
    printf("Spells: ");
    for (i = 0; i < 10; i++)
    {
        if (player->items[i].check > 0)
        {
            for (j = 0; player->items[i].spells[j] != '\0'; j++)
            {
                printf("%c", player->items[i].spells[j]); // forgot to add format specifier
            }
            printf(" | ");
        }
    }
    printf("\n");
}

void attainspell(Player *player, char *spellname) // This is a helper function, so we assume that all inputs are valid.
{
    int i;
    for (i = 0; i < 10; i++)
    {
        if (strcmp(player->items[i].spells, spellname) == 0)
        {
            player->items[i].check++; // adds 1 to the check count.
            break;
        }
    }
}

int itemhave(Player *player, char *item) // This is a helper function to check if the player has the right item.
{
    int i;
    for (i = 0; i < 10; i++)
    {
        if (strcmp(player->items[i].itemName, item) == 0)
        {
            if (player->items[i].quantity > 0)
            { // They have the item
                return 1;
            }
            return 0; // No item. Return 0
        }
    }
    return 0;
}

typedef enum
{
    INITIAL_STATE,  // We use a FSM to track enemy behaviour and to determine if the fight is over
    PLEADING_STATE, // Enemy pleads for their lives if their HP drops below 25%. They can either surrender or attack you and rob your hp!
    DEFEATED_STATE  // Terminates if the enemy is defeated
} Enemybehaviour;

typedef struct
{
    Enemybehaviour Current_State;
    int level;
    int enemyhp; // tracks the enemy's hp
    int enemymaxhp;
    int strength;
} SpecificEnemy;

char *fightpool[] = {"Easy", "Easy", "Easy", "Easy", "Normal", "Normal", "Normal", "Hard", "Hard", "Very Hard"}; // the different pools for each fight

void init_enemy(SpecificEnemy *Enemy, int level)
{
    char *difficulty = fightpool[(rand() % 10)]; // we choose a "pool" for the fight.
    if (level == 30)                             // if the level entered is 30, assume it's the final boss
    {
        difficulty = "Boss";
    }
    Enemy->level = level;
    Enemy->enemyhp = level * 10;
    Enemy->strength = level * 5;
    if (strcmp(difficulty, "Easy"))
    {
        printf("You're fighting an easy enemy!\n");
    }

    else if (strcmp(difficulty, "Normal"))
    {
        printf("You're fighting a Normal enemy!\n");
        Enemy->enemyhp += level * 1;
        Enemy->strength += level * 1;
    }

    else if (strcmp(difficulty, "Hard"))
    {
        printf("You're fighting a Hard enemy!\n");
        Enemy->enemyhp += level * 3;
        Enemy->strength += level * 3;
    }

    else if (strcmp(difficulty, "Very Hard"))
    {
        printf("You're fighting a Very Hard enemy!\n");
        Enemy->enemyhp += level * 4;
        Enemy->strength += level * 4;
    }

    else if (strcmp(difficulty, "Boss"))
    {
        printf("You're fighting the final boss!\n");
        Enemy->enemyhp += level * 5;
        Enemy->strength += level * 5;
    }
    Enemy->enemymaxhp = Enemy->enemyhp; // We track the maxhp.
    Enemy->Current_State = INITIAL_STATE;
}

void EnemyState(SpecificEnemy *Enemy)
{
    float ratio = (float)(Enemy->enemyhp) / (float)(Enemy->enemymaxhp);
    switch (Enemy->Current_State)
    {
    case INITIAL_STATE:
        // printf("\n");
        // printf("The Enemy is still READY TO FIGHT!\n");
        // printf("HP: enemyhp %d", Enemy->enemyhp);
        // printf("HP: enemyMAXhp %d", Enemy->enemymaxhp);
        // printf("HP: FFFF %f", ratio);
        if (Enemy->enemyhp > 0 && ratio <= 0.25) // enemy less than 0.25
        {
            Enemy->Current_State = PLEADING_STATE; // Enemy pleads for their lives if their hp is 25%!
        }
        else if (Enemy->enemyhp <= 0)
        {
            Enemy->Current_State = DEFEATED_STATE;
        }
        break;

    case PLEADING_STATE:
        printf("PLEADING STATE!\n");
        if (ratio >= 0.25) // enemy more than 0.25
        {
            Enemy->Current_State = INITIAL_STATE; // If Enemy recovers, they go back to the Initial State
        }
        else if (Enemy->enemyhp <= 0)
        {
            Enemy->Current_State = DEFEATED_STATE; // Else, the Enemy dies
        }
        break;

    case DEFEATED_STATE:
        printf("You defeated the monster!\n");
    }
}

int MoxieRoll(Player *player)
{
    int valuetobeat = rand() % 200;
    if (player->moxie >= valuetobeat - 40)
    {
        printf("CRIT ROLL SUCCESS!");
        return rand() % 2 + 1;
    }
    return 0;
}

void Enemyattack(SpecificEnemy *Enemy, Player *player)
{
    printf("Enemy attacks!\n");
    int damagedealt = Enemy->strength - player->level * MoxieRoll(player);
    printf("Enemy dealt %d DAMAGE!\n", damagedealt);
    player->hp -= damagedealt;
    if (player->hp <= 0)
    {
        player->status = DEAD;
    }
}

void EnemyPleading(SpecificEnemy *Enemy, Player *player)
{
    int pleadsuccess = rand() % 100;
    if (pleadsuccess > 50)
    {
        printf("You successfully convinced the enemy to surrender!\n");
        player->gold += Enemy->level * 5;
        Enemy->enemyhp = 0; // Enemy is DEFEATED!
    }
    else
    {
        printf("Your persuasion failed! Received DAMAGE! Enemy stole your lifeforce when you were caught offguard!\n");
        Enemyattack(Enemy, player);
        Enemy->enemyhp += Enemy->level * 3;
        if (player->hp <= 0)
        {
            player->status = DEAD;
        }
    }
}

void PlayerAttack(SpecificEnemy *Enemy, Player *player)
{
    int damagedealt = player->str + player->level * MoxieRoll(player);
    printf("\nYou dealt: %d DAMAGE!\n", damagedealt);
    Enemy->enemyhp -= damagedealt;
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("You did not enter the right number of parameters. Ending program.");
        return 1;
    }

    const char *filename = argv[1];
    const char *csvfile = argv[2];
    char playerchoice[50]; // Increased buffer size to accommodate longer item names
    int keeplaying = 1;
    int keepchoosingbattle = 1;
    char *location;
    srand((time(NULL))); // we randomise a seed
    Player player;
    player.status = ALIVE; // set player to ALIVE.
    SpecificEnemy enemy;
    load_game(&player, filename);
    if (strcmp(player.class, "NewPlayer") == 0) // Checks if the Player is New
    {
        const char *patternW = "^(I (choose|pick|select|want) )?(Warrior|W|\\(W\\)|Warrior \\(W\\))$";
        regex_t regexW;
        int resultW = regcomp(&regexW, patternW, REG_EXTENDED | REG_NOSUB | REG_ICASE);

        const char *patternM = "^(I (choose|pick|select|want) )?(Mage|M|\\(M\\)|Mage \\(M\\))$";
        regex_t regexM;
        int resultM = regcomp(&regexM, patternM, REG_EXTENDED | REG_NOSUB | REG_ICASE);

        const char *patternT = "^(I (choose|pick|select|want) )?(Thief|T|\\(T\\)|Thief \\(T\\))$";
        regex_t regexT;
        int resultT = regcomp(&regexT, patternT, REG_EXTENDED | REG_NOSUB | REG_ICASE);
        printf("Welcome to Fantasy Adventure. Choose your Class: \n");
        printf("Warrior (W), Mage (M), Thief (T) \n");
        int validclassflag = 1; // check if the player has a valid class.
        while (validclassflag)
        {

            fgets(playerchoice, sizeof(playerchoice), stdin);
            playerchoice[strcspn(playerchoice, "\n")] = '\0'; // Remove trailing newline
            resultW = regexec(&regexW, playerchoice, 0, NULL, 0);
            resultM = regexec(&regexM, playerchoice, 0, NULL, 0);
            resultT = regexec(&regexT, playerchoice, 0, NULL, 0);

            if (resultW == 0)
            {
                strcpy(player.class, "Warrior");
                validclassflag = 0;
                player.str += 10;
                attainspell(&player, "Great Slash");
                regfree(&regexW);
                regfree(&regexM);
                regfree(&regexT);
                printf("Congrats, you are a Warrior! Find keys to unlock the FINAL DUNGEON and defeat the Evil Lord!\n");
                printf("\n");
            }

            else if (resultM == 0)
            {
                strcpy(player.class, "Mage");
                validclassflag = 0;
                player.magic += 10;
                attainspell(&player, "Fireball");
                attainspell(&player, "Lightning");
                regfree(&regexW);
                regfree(&regexM);
                regfree(&regexT);
                printf("Congrats, you are a Mage! Find keys to unlock the FINAL DUNGEON and defeat the Evil Lord!\n");
                printf("\n");
            }

            else if (resultT == 0)
            {
                strcpy(player.class, "Thief");
                validclassflag = 0;
                player.moxie += 10;
                attainspell(&player, "Bullet Barrage");
                regfree(&regexW);
                regfree(&regexM);
                regfree(&regexT);
                printf("Congrats, you are a Thief! Find keys to unlock the FINAL DUNGEON and defeat the Evil Lord!\n");
                printf("\n");
            }

            else
            {
                printf("You entered an invalid input! Enter a valid one!\n");
            }
        }
    }

    while (keeplaying)
    {
        if (player.status == DEAD)
        {
            printf("You have been revived in the Town. Be careful next time!\n");
            player.hp = player.maxhp;
            player.mana = player.maxmana;
            player.status = ALIVE;
        }
        printf("What will you do now?\n");
        printf("YOU are in TOWN. You can Quit (1), Save (2), Purchase (3), Battle(4), Talk (5) or Heal (6). \n");
        printf("Your current HP, mana and gold are: %d, %d, %d\n", player.hp, player.mana, player.gold);
        print_spell(&player);
        printf("\n");
        fgets(playerchoice, sizeof(playerchoice), stdin);
        playerchoice[strcspn(playerchoice, "\n")] = '\0'; // Remove trailing newline

        if (strcmp(playerchoice, "Quit") == 0 || strcmp(playerchoice, "quit") == 0 || strcmp(playerchoice, "1") == 0 || strcmp(playerchoice, "Q") == 0 || strcmp(playerchoice, "q") == 0)
        {
            keeplaying = 0;
            printf("You quit the game!\n");
        }
        else if (strcmp(playerchoice, "Save") == 0 || strcmp(playerchoice, "save") == 0 || strcmp(playerchoice, "2") == 0 || strcmp(playerchoice, "S") == 0 || strcmp(playerchoice, "s") == 0)
        {
            save_game(&player, filename, csvfile);
        }
        else if (strcmp(playerchoice, "Purchase") == 0 || strcmp(playerchoice, "purchase") == 0 || strcmp(playerchoice, "3") == 0 || strcmp(playerchoice, "P") == 0 || strcmp(playerchoice, "p") == 0)
        {
            printf("You bought some stuff!");
        }
        else if (strcmp(playerchoice, "Battle") == 0 || strcmp(playerchoice, "battle") == 0 || strcmp(playerchoice, "4") == 0 || strcmp(playerchoice, "B") == 0 || strcmp(playerchoice, "b") == 0)
        {
            keepchoosingbattle = 1;
            while (keepchoosingbattle)
            {
                printf("You have a few locations to select from!\n");
                printf("GreenWood (1) - Lvl 1-3, Graveyard (2) - Lvl 4-6, Forest (3) - Lvl 7-9, Volcano (4) - Lvl 10-12, Castle (5) - Lvl 13-15\n");
                printf("FairyTrove (6) - Lvl 16-18, GoblinCave (7) - Lvl 19-21, DragonNest (8) - Lvl 22-24, Valley (9) - Lvl 25-27, FinalDungeon (10) - Lvl 30\n");
                printf("\n");
                printf("Choose your option: ");
                fgets(playerchoice, sizeof(playerchoice), stdin);
                playerchoice[strcspn(playerchoice, "\n")] = '\0'; // Remove trailing newline

                if (strcmp(playerchoice, "GreenWood") == 0 || strcmp(playerchoice, "greenwood") == 0 || strcmp(playerchoice, "GW") == 0 || strcmp(playerchoice, "gw") == 0 || strcmp(playerchoice, "1") == 0)
                {
                    init_enemy(&enemy, 3 - rand() % 3);
                    keepchoosingbattle = 0;
                    location = "GreenWood";
                }
                else if (strcmp(playerchoice, "GraveYard") == 0 || strcmp(playerchoice, "graveyard") == 0 || strcmp(playerchoice, "GY") == 0 || strcmp(playerchoice, "gy") == 0 || strcmp(playerchoice, "2") == 0)
                {
                    init_enemy(&enemy, 6 - rand() % 3);
                    keepchoosingbattle = 0;
                    location = "Graveyard";
                }
                else if (strcmp(playerchoice, "Forest") == 0 || strcmp(playerchoice, "forest") == 0 || strcmp(playerchoice, "F") == 0 || strcmp(playerchoice, "f") == 0 || strcmp(playerchoice, "3") == 0)
                {
                    init_enemy(&enemy, 9 - rand() % 3);
                    keepchoosingbattle = 0;
                    location = "Forest";
                }
                else if (strcmp(playerchoice, "Volcano") == 0 || strcmp(playerchoice, "volcano") == 0 || strcmp(playerchoice, "V") == 0 || strcmp(playerchoice, "v") == 0 || strcmp(playerchoice, "4") == 0)
                {
                    init_enemy(&enemy, 12 - rand() % 3);
                    keepchoosingbattle = 0;
                    location = "Volcano";
                }
                else if (strcmp(playerchoice, "Castle") == 0 || strcmp(playerchoice, "castle") == 0 || strcmp(playerchoice, "C") == 0 || strcmp(playerchoice, "c") == 0 || strcmp(playerchoice, "5") == 0)
                {
                    init_enemy(&enemy, 15 - rand() % 3);
                    keepchoosingbattle = 0;
                    location = "Castle";
                }
                else if (strcmp(playerchoice, "FairyTrove") == 0 || strcmp(playerchoice, "fairytrove") == 0 || strcmp(playerchoice, "FT") == 0 || strcmp(playerchoice, "ft") == 0 || strcmp(playerchoice, "6") == 0)
                {
                    init_enemy(&enemy, 18 - rand() % 3);
                    keepchoosingbattle = 0;
                    location = "FairyTrove";
                }
                else if (strcmp(playerchoice, "GoblinCave") == 0 || strcmp(playerchoice, "goblincave") == 0 || strcmp(playerchoice, "GC") == 0 || strcmp(playerchoice, "gc") == 0 || strcmp(playerchoice, "7") == 0)
                {
                    init_enemy(&enemy, 21 - rand() % 3);
                    keepchoosingbattle = 0;
                    location = "GoblinCave";
                }
                else if (strcmp(playerchoice, "DragonNest") == 0 || strcmp(playerchoice, "dragonnest") == 0 || strcmp(playerchoice, "DN") == 0 || strcmp(playerchoice, "dn") == 0 || strcmp(playerchoice, "8") == 0)
                {
                    init_enemy(&enemy, 24 - rand() % 3);
                    keepchoosingbattle = 0;
                    location = "DragonNest";
                }
                else if (strcmp(playerchoice, "Vast Valley") == 0 || strcmp(playerchoice, "vast valley") == 0 || strcmp(playerchoice, "VV") == 0 || strcmp(playerchoice, "vv") == 0 || strcmp(playerchoice, "9") == 0)
                {
                    init_enemy(&enemy, 27 - rand() % 3);
                    keepchoosingbattle = 0;
                    location = "Vast Valley";
                }
                else if (strcmp(playerchoice, "FinalDungeon") == 0 || strcmp(playerchoice, "finaldungeon") == 0 || strcmp(playerchoice, "FD") == 0 || strcmp(playerchoice, "fd") == 0 || strcmp(playerchoice, "10") == 0)
                {
                    if (itemhave(&player, "Dungeon Key"))
                    {
                        keepchoosingbattle = 0;
                        init_enemy(&enemy, 30);
                        location = "FinalDungeon";
                        printf("At last, you can challenge the BOSS!\n");
                    }
                    else
                    {
                        printf("You have NOT exchanged all keys for the Dungeon Key yet!");
                    }
                }
                else
                {
                    printf("You entered an invalid input! Try again!\n");
                    printf("\n");
                }
            }
            printf("You are fighting a foe from %s!\n", location);
            printf("Make your Move!\n");
            printf("\n");
            while (enemy.Current_State != DEFEATED_STATE && player.status == ALIVE) // while the enemy is still alive and the player is not dead!
            {
                printf("Your current HP, and mana are: %d, %d\n", player.hp, player.mana);
                printf("Enemy's current lvl is %d, HP is %d\n", enemy.level, enemy.enemyhp);
                fgets(playerchoice, sizeof(playerchoice), stdin);
                playerchoice[strcspn(playerchoice, "\n")] = '\0'; // Remove trailing newline
                printf("INITIAL STATE: %d\n", enemy.Current_State);
                PlayerAttack(&enemy, &player);
                EnemyState(&enemy); // we check the enemystate after the attack!
                printf("CHANGED STATE: %d\n", enemy.Current_State);
                if (enemy.Current_State == INITIAL_STATE)
                {
                    Enemyattack(&enemy, &player);
                }
                else if (enemy.Current_State == PLEADING_STATE)
                {
                    printf("The enemy suddenly shrinks back, pleading for you to show mercy!\n");
                }
            }
        }
        else if (strcmp(playerchoice, "Talk") == 0 || strcmp(playerchoice, "talk") == 0 || strcmp(playerchoice, "5") == 0 || strcmp(playerchoice, "T") == 0 || strcmp(playerchoice, "t") == 0)
        {
            printf("You talked for a while!");
        }
        else if (strcmp(playerchoice, "Heal") == 0 || strcmp(playerchoice, "heal") == 0 || strcmp(playerchoice, "6") == 0 || strcmp(playerchoice, "H") == 0 || strcmp(playerchoice, "h") == 0)
        {
            player.hp = player.maxhp;
            player.mana = player.maxmana;
            printf("You Healed up!\n");
        }
        else
        {
            printf("You entered an invalid input!!!\n");
        }
    }
    return 0;
}
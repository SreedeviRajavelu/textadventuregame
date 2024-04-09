#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "pbPlots.h"
#include "supportLib.h"

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

void stock_item(Player *player, const char *filename, const char *csvfilename)
{
    // Player *player;
    char itemchoice[50];
    fgets(itemchoice, sizeof(itemchoice), stdin);
    int found = 0;
    for (int i = 0; i < 10; i++)
    {
        // need to read thru textfile
        printf("looping to get item\n");
        printf("%s \n", player->items[i].itemName);
        itemchoice[strcspn(itemchoice, "\n")] = '\0';
        printf("%s \n", itemchoice);
        if (strcmp(itemchoice, player->items[i].itemName) == 0)
        {
            player->items[i].quantity++;
            printf("You obtained %s !\n", itemchoice);
            found = 1;
            break;
        }
    }
    if (!found)
    {
        printf("Item not found!\n");
    }
}
// Function to draw the title on the bar graph
// void DrawTitle(RGBABitmapImage *canvas, const char *title)
// {
//     double canvasWidth = ImageWidth(canvas);
//     double xPadding = canvasWidth / 10.0;
//     double yPadding = xPadding;

//     DrawText(canvas, floor(canvasWidth / 2.0 - GetTextWidth(title, strlen(title)) / 2.0), floor(yPadding / 3.0), title, strlen(title), GetBlack());
// }
void plot_graph(Player *player, const char *filename, const char *csvfilename)
{
    printf("Enter name for graph file to be save and then press enter:\n");
    char graphName[50];
    fgets(graphName, sizeof(graphName), stdin);
    graphName[strcspn(graphName, "\n")] = '\0';

    // prepare data for the bar graph
    size_t numItems = 0;
    double quantities[10];
    const char *itemNames[15];
    // wchar_t *itemNames[10]; // Change type to wchar_t*
    // extract item names and quantities from the player's inventory
    for (int i = 0; i < 10; i++)
    {
        if (player->items[i].quantity > 0)
        {
            // convert item names to wchar_t*
            // size_t len = strlen(player->items[i].itemName) + 1;
            // itemNames[numItems] = (wchar_t *)malloc(len * sizeof(wchar_t));
            // mbstowcs(itemNames[numItems], player->items[i].itemName, len);
            itemNames[numItems] = player->items[i].itemName;
            quantities[numItems] = player->items[i].quantity;
            numItems++;
        }

        // create the bar plot

        double width = 1000;
        // double height = 1400;
        double height = 1800;
        RGBABitmapImage *barPlotImage = DrawBarPlot(width, height, quantities, numItems);

        // Add title to the bar graph using the custom function
        // DrawTitle(barPlotImage, "Inventory");
        // Reference to code from pbPlots.c
        // DrawText(canvas, floor(ImageWidth(canvas)/2.0 - GetTextWidth(settings->title, settings->titleLength)/2.0), floor(yPadding/3.0), settings->title, settings->titleLength, GetBlack());
        // DrawTextUpwards(canvas, 10.0, floor(ImageHeight(canvas)/2.0 - GetTextWidth(settings->yLabel, settings->yLabelLength)/2.0), settings->yLabel, settings->yLabelLength, GetBlack());

        // Add labels to the x-axis (item names)
        // double charWidth = 8.0;
        // double spacing = 2.0;
        // for (size_t i = 0; i < numItems; i++)
        // {
        //     DrawText(barPlotImage, 70 + i * (charWidth + spacing), 550, itemNames[i], strlen(itemNames[i]), CreateRGBColor(0, 0, 0)); // DrawText(barPlotImage, 70 + i * 70, 550, itemNames[i], 12, CreateRGBColor(0, 0, 0), width / 100);
        // }

        // // Free allocated memory for item names
        // for (size_t i = 0; i < numItems; i++)
        // {
        //     free(itemNames[i]);
        // }
        // // Add label to the y-axis
        // DrawText(barPlotImage, 20, 300, "Quantity", 12, CreateRGBColor(0, 0, 0), width / 100);
        // Convert graph name to wchar_t*
        // size_t len = strlen(graphName) + 1;
        // wchar_t *graphNameW = (wchar_t *)malloc(len * sizeof(wchar_t));
        // mbstowcs(graphNameW, graphName, len);

        // Save the bar plot image to a file

        size_t length;
        double *pngData = ConvertToPNG(&length, barPlotImage);

        // Save the bar plot image to a file
        WriteToFile(pngData, length, graphName);
        // Free allocated memory
        // free(graphNameW);
    }
}

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
           &player->gold,              // %d - integer
           &player->level,             // %d - integer
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

void winloss(Player *player, char *loc_name) // This is a helper function to update win/loss per location.
{
    int i;
    for (i = 0; i < 10; i++)
    {
        if (strcmp(player->items[i].location, loc_name) == 0)
        {
            if (player->status == ALIVE)
            {
                player->items[i].wins++;
                return;
            }
            player->items[i].loss++;
        }
    }
}

int spellhave(Player *player, char *spell) // This is a helper function to check if the player has the right spell.
{
    int i;
    for (i = 0; i < 10; i++)
    {
        if (strcmp(player->items[i].spells, spell) == 0)
        {
            if (player->items[i].check > 0)
            { // They have the spell
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
    char *pool;
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
    Enemy->pool = difficulty;
    Enemy->level = level;
    Enemy->enemyhp = level * 20;
    Enemy->strength = level * 2;
    if (strcmp(Enemy->pool, "Easy") == 0)
    {
        printf("You're fighting an easy enemy!\n");
        // printf("The enemy is %s", Enemy->pool);
    }

    else if (strcmp(Enemy->pool, "Normal") == 0)
    {
        printf("You're fighting a Normal enemy!\n");
        // printf("The enemy is %s", Enemy->pool);
        Enemy->enemyhp += level * 3;
        Enemy->strength += level * 2 + 10;
    }

    else if (strcmp(Enemy->pool, "Hard") == 0)
    {
        printf("You're fighting a Hard enemy!\n");
        // printf("The enemy is %s", Enemy->pool);
        Enemy->enemyhp += level * 6;
        Enemy->strength += level * 3 + 5;
    }

    else if (strcmp(Enemy->pool, "Very Hard") == 0)
    {
        printf("You're fighting a Very Hard enemy!\n");
        // printf("The enemy is %s", Enemy->pool);
        Enemy->enemyhp += level * 8;
        Enemy->strength += level * 4;
    }

    else if (strcmp(Enemy->pool, "Boss") == 0)
    {
        printf("You're fighting the final boss!\n");
        Enemy->enemyhp += level * 10;
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
    int pleadsuccess = rand() % 100 + player->moxie / 10;
    // printf("Your moxie is groovy, %d\n", player->moxie);
    printf("You plead for your enemy to surrender with a value of %d", pleadsuccess);
    if (pleadsuccess > 100)
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

void PlayerAttack(SpecificEnemy *Enemy, Player *player, int mode, int add) // mode = 1/2 1 = str 2 = magic
{
    int calc;
    if (mode == 1)
    {
        calc = player->str + add;
    }
    else if (mode == 2)
    {
        calc = player->magic + add;
    }
    int damagedealt = calc + player->level * MoxieRoll(player);
    printf("\nYou dealt: %d DAMAGE!\n\n", damagedealt);
    Enemy->enemyhp -= damagedealt;
}

int manaoverload(int manacost, int currentmana)
{
    if (manacost > currentmana)
    {
        return 1;
    }
    return 0;
}
void spellcast(SpecificEnemy *Enemy, Player *player, char *spell)
{
    printf("\nThe spell casted was %s\n", spell);
    if (!spellhave(player, spell))
    {
        printf("You don't have this spell! You fumbled and were attacked by the Enemy!"); // Spell rejected
        return;
    }
    // if (strcmp(spell, "Fireball")) A mistake strcmp needs == 0
    // {
    //     printf("HUH??????????? %s", spell);
    // }
    if (strcmp(spell, "Fireball") == 0)
    {
        if (manaoverload(10, player->mana))
        {
            printf("Not enough mana to cast %s! You fumbled!\n", spell);
            return;
        }
        PlayerAttack(Enemy, player, 2, 10);
        printf("Burn your opponents! %s was cast!\n", spell);
        player->mana -= 10;
    }

    else if (strcmp(spell, "Lightning") == 0)
    {
        if (manaoverload(20, player->mana))
        {
            printf("Not enough mana to cast %s! You fumbled!\n", spell);
            return;
        }
        PlayerAttack(Enemy, player, 2, player->level * 5);
        printf("Feel the power of lightning! %s was cast!\n", spell);
        player->mana -= 20;
    }

    else if (strcmp(spell, "Great Slash") == 0)
    {
        if (manaoverload(15, player->mana))
        {
            printf("Not enough mana to cast %s! You fumbled!\n", spell);
            return;
        }
        PlayerAttack(Enemy, player, 1, 20);
        printf("A powerful Slash! %s was cast!\n", spell);
        player->mana -= 15;
    }

    else if (strcmp(spell, "Bullet Barrage") == 0)
    {
        if (manaoverload(15, player->mana))
        {
            printf("Not enough mana to cast %s! You fumbled!\n", spell);
            return;
        }
        int bulletsfired = rand() % 3 + 1;
        int i;
        for (i = 0; i < bulletsfired; i++)
        {
            PlayerAttack(Enemy, player, 1, -(player->str / 2));
        }
        printf("%s was cast! %d Bullets was fired!\n", spell, bulletsfired);
        player->mana -= 15;
    }

    else if (strcmp(spell, "Heaven Heal") == 0)
    {
        int manacost = player->level * 2;
        if (manaoverload(manacost, player->mana))
        {
            printf("Not enough mana to cast %s! You fumbled!\n", spell);
            return;
        }
        printf("Healed up! %s was cast!\n", spell);
        player->mana -= manacost;
        int Healamount = player->level * 10;
        if (Healamount + player->hp > player->maxhp)
        {
            player->hp = player->maxhp;
        }
        else
        {
            player->hp += Healamount;
        }
    }

    else if (strcmp(spell, "Blood Slash") == 0)
    {
        if (manaoverload(player->maxhp / 5, player->hp))
        {
            printf("Not enough hp to cast %s! You fumbled!\n", spell);
            return;
        }
        PlayerAttack(Enemy, player, 1, player->maxhp / 5);
        printf("Sacrifice HP! %s was cast!\n", spell);
        player->hp -= player->maxhp / 5;
    }

    else if (strcmp(spell, "Crit Strike") == 0)
    {
        if (manaoverload(30, player->mana))
        {
            printf("Not enough mana to cast %s! You fumbled!\n", spell);
            return;
        }
        player->moxie += 50;
        PlayerAttack(Enemy, player, 1, 0);
        printf("%s was cast! Increased Moxie by 50 to boost Crits!\n", spell);
        player->moxie -= 50;
        player->mana -= 30;
    }

    else if (strcmp(spell, "All or Nothing!") == 0)
    {
        if (manaoverload(20, player->mana))
        {
            printf("Not enough mana to cast %s! You fumbled!\n", spell);
            return;
        }
        int allroll = rand() % 2; // either 0 or 1
        if (!allroll)
        {
            printf("Lost the 50/50! You built up too much power in charging %s! You fumbled!\n", spell);
            player->mana -= 20;
            return;
        }
        printf("ALL OR NOTHING! It's an ALLOUT ATTACK!\n");
        PlayerAttack(Enemy, player, 1, player->str + player->magic + player->level);
        printf("%s was cast!\n", spell);
        player->mana -= 20;
    }

    else if (strcmp(spell, "Dragon Punch") == 0)
    {
        if (manaoverload(50, player->mana))
        {
            printf("Not enough mana to cast %s! You fumbled!\n", spell);
            return;
        }
        PlayerAttack(Enemy, player, 1, player->magic * 2);
        printf("%s was cast! A Devestating blow!\n", spell);
        player->mana -= 50;
    }
    else if (strcmp(spell, "Pyroblast") == 0)
    {
        if (manaoverload(120, player->mana))
        {
            printf("Not enough mana to cast %s! You fumbled!\n", spell);
            return;
        }
        PlayerAttack(Enemy, player, 2, player->magic * 3);
        printf("%s was cast! The greatest spell!!\n", spell);
        player->mana -= 120;
    }
}

void levelup(Player *player)
{
    printf("You leveled up!\n\n");
    player->level++;
    if (strcmp(player->class, "Warrior") == 0)
    {
        printf("Warrior BOOST!");
        player->str += rand() % 3 + 5;
        player->magic += rand() % 1 + 1;
        player->maxmana += 2;
        player->moxie += rand() % 2 + 1;
        player->maxhp += 20;
    }

    else if (strcmp(player->class, "Mage") == 0)
    {
        printf("Mage BOOST!");
        player->str += rand() % 1 + 2;
        player->magic += rand() % 3 + 3;
        player->maxmana += 10;
        player->moxie += rand() % 3 + 3;
        player->maxhp += 10;
    }

    else if (strcmp(player->class, "Thief") == 0)
    {
        printf("Thief BOOST!");
        player->str += rand() % 2 + 2;
        player->magic += rand() % 2 + 2;
        player->maxmana += 5;
        player->moxie += rand() % 5 + 5;
        player->maxhp += 8;
    }
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
    int run;
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
            printf("\nYou lost!");
            printf("You have been revived in the Town. Be careful next time!\n");
            player.hp = player.maxhp;
            player.mana = player.maxmana;
            player.status = ALIVE;
        }
        printf("What will you do now?\n\n");
        printf("YOU are in TOWN. You can Quit (1), Save (2), Purchase (3), Battle(4), Talk (5) or Heal (6) or Stock Up (7) or Graph (8). \n");
        // printf("Your current HP, mana, level and gold are: %d, %d, %d, %d\n", player.hp, player.mana, player.level, player.gold);
        printf("Your current HP: %d\n", player.hp);
        printf("Your current mana: %d\n", player.mana);
        printf("Your current level: %d\n", player.level);
        printf("Your current gold: %d\n", player.gold);
        printf("Your str:%d, magic:%d and moxie:%d\n", player.str, player.magic, player.moxie);
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
                        printf("You have NOT exchanged all keys for the Dungeon Key yet!\n");
                    }
                }
                else
                {
                    printf("You entered an invalid input! Try again!\n");
                    printf("\n");
                }
            }
            printf("You're fighting a level %d foe from %s!\n", enemy.level, location);
            printf("Make your Move!\n");
            printf("\n");
            while (enemy.Current_State != DEFEATED_STATE && player.status == ALIVE) // while the enemy is still alive and the player is not dead!
            {
                printf("\nYour current HP: %d\n", player.hp);
                printf("Your current mana: %d\n", player.mana);
                printf("Your str:%d, magic:%d and moxie:%d\n", player.str, player.magic, player.moxie);
                print_spell(&player);
                printf("\nEnemy's current HP: %d\n\n", enemy.enemyhp);
                // printf("INITIAL STATE: %d\n", enemy.Current_State);
                printf("You can:\n");
                printf("1 - Attack\n2 - Run\n");
                if (enemy.Current_State == PLEADING_STATE)
                {
                    printf("3 - Spare your enemy!");
                }
                printf("Other - Use your spells! (Name Them)\n");
                fgets(playerchoice, sizeof(playerchoice), stdin);
                playerchoice[strcspn(playerchoice, "\n")] = '\0'; // Remove trailing newline
                if (strcmp(playerchoice, "1") == 0 || strcmp(playerchoice, "Attack") == 0 || strcmp(playerchoice, "attack") == 0)
                {
                    PlayerAttack(&enemy, &player, 1, 0);
                }
                else if (strcmp(playerchoice, "2") == 0 || strcmp(playerchoice, "Run") == 0 || strcmp(playerchoice, "run") == 0)
                {
                    run = rand() % 10;
                    if (run > 3)
                    {
                        printf("You escaped!\n");
                        break;
                    }
                    printf("You tripped and fell!\n");
                }
                else if (strcmp(playerchoice, "Fireball") == 0 || strcmp(playerchoice, "fireball") == 0 || strcmp(playerchoice, "F") == 0 || strcmp(playerchoice, "f") == 0)
                {
                    spellcast(&enemy, &player, "Fireball");
                }
                else if (strcmp(playerchoice, "Lightning") == 0 || strcmp(playerchoice, "lightning") == 0 || strcmp(playerchoice, "L") == 0 || strcmp(playerchoice, "l") == 0)
                {
                    spellcast(&enemy, &player, "Lightning");
                }
                else if (strcmp(playerchoice, "Great Slash") == 0 || strcmp(playerchoice, "great slash") == 0 || strcmp(playerchoice, "GS") == 0 || strcmp(playerchoice, "gs") == 0)
                {
                    spellcast(&enemy, &player, "Great Slash");
                }
                else if (strcmp(playerchoice, "Bullet Barrage") == 0 || strcmp(playerchoice, "bullet barrage") == 0 || strcmp(playerchoice, "BB") == 0 || strcmp(playerchoice, "bb") == 0)
                {
                    spellcast(&enemy, &player, "Bullet Barrage");
                }
                else if (strcmp(playerchoice, "Heaven Heal") == 0 || strcmp(playerchoice, "heaven heal") == 0 || strcmp(playerchoice, "HH") == 0 || strcmp(playerchoice, "hh") == 0)
                {
                    spellcast(&enemy, &player, "Heaven Heal");
                }
                else if (strcmp(playerchoice, "Blood Slash") == 0 || strcmp(playerchoice, "blood slash") == 0 || strcmp(playerchoice, "BS") == 0 || strcmp(playerchoice, "bs") == 0)
                {
                    spellcast(&enemy, &player, "Blood Slash");
                }
                else if (strcmp(playerchoice, "Crit Strike") == 0 || strcmp(playerchoice, "crit strike") == 0 || strcmp(playerchoice, "CS") == 0 || strcmp(playerchoice, "cs") == 0)
                {
                    spellcast(&enemy, &player, "Crit Strike");
                }
                else if (strcmp(playerchoice, "All or Nothing") == 0 || strcmp(playerchoice, "all or nothing") == 0 || strcmp(playerchoice, "AON") == 0 || strcmp(playerchoice, "aon") == 0)
                {
                    spellcast(&enemy, &player, "All or Nothing");
                }
                else if (strcmp(playerchoice, "Dragon Punch") == 0 || strcmp(playerchoice, "dragon punch") == 0 || strcmp(playerchoice, "DP") == 0 || strcmp(playerchoice, "dp") == 0)
                {
                    spellcast(&enemy, &player, "Dragon Punch");
                }
                else if (strcmp(playerchoice, "Pyroblast") == 0 || strcmp(playerchoice, "pyroblast") == 0 || strcmp(playerchoice, "P") == 0 || strcmp(playerchoice, "p") == 0)
                {
                    spellcast(&enemy, &player, "Pyroblast");
                }

                else if (enemy.Current_State == PLEADING_STATE && (strcmp(playerchoice, "3") == 0 || strcmp(playerchoice, "Spare") == 0 || strcmp(playerchoice, "spare") == 0 || strcmp(playerchoice, "S") == 0 || strcmp(playerchoice, "s") == 0))
                {
                    EnemyPleading(&enemy, &player);
                }
                else
                {
                    printf("You posted an invalid input!\n");
                }
                EnemyState(&enemy); // we check the enemystate after the attack!
                // printf("CHANGED STATE: %d\n", enemy.Current_State);
                if (enemy.Current_State == INITIAL_STATE)
                {
                    Enemyattack(&enemy, &player);
                }
                else if (enemy.Current_State == PLEADING_STATE)
                {
                    printf("The enemy shrinks back, pleading for you to show mercy!\n");
                }
            }
            winloss(&player, location);
            if (enemy.Current_State == DEFEATED_STATE)
            {
                if (strcmp(enemy.pool, "Easy") == 0)
                {
                    levelup(&player);
                    printf("Defeated an %s enemy!\n", enemy.pool);
                    printf("Gained %d gold!\n", enemy.level * 10);
                    player.gold += enemy.level * 10;
                }
                else if (strcmp(enemy.pool, "Normal") == 0)
                {
                    levelup(&player);
                    printf("Defeated an %s enemy!\n", enemy.pool);
                    printf("Gained %d gold!\n", enemy.level * 15);
                    player.gold += enemy.level * 15;
                }
                else if (strcmp(enemy.pool, "Hard") == 0)
                {
                    levelup(&player);
                    levelup(&player);
                    printf("Defeated an %s enemy!\n", enemy.pool);
                    printf("Gained %d gold!\n", enemy.level * 20);
                    player.gold += enemy.level * 20;
                }
                else if (strcmp(enemy.pool, "Very Hard") == 0)
                {
                    levelup(&player);
                    levelup(&player);
                    levelup(&player);
                    printf("Defeated an %s enemy!\n", enemy.pool);

                    printf("Gained %d gold!\n", enemy.level * 30);
                    player.gold += enemy.level * 30;
                }

                else if (strcmp(enemy.pool, "Boss") == 0)
                {
                    printf("You beat the game! Congrats! Now, back to Town!\n");
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
        else if (strcmp(playerchoice, "7") == 0 || strcmp(playerchoice, "Stock Up") == 0 || strcmp(playerchoice, "stock up") == 0 || strcmp(playerchoice, "STOCK UP") == 0 || strcmp(playerchoice, "su") == 0 || strcmp(playerchoice, "SU") == 0)
        {

            printf("What item do you want to get? Type name of one item and press enter. Entry is case-sensitive. \n");
            printf("Key | Potion | Sword | Shield | Helmet | Car | Bike | Gun | Explosive | DungeonKey \n");
            stock_item(&player, filename, csvfile);
            // printf("You obtained item  !\n");
        }
        else if (strcmp(playerchoice, "8") == 0 || strcmp(playerchoice, "Graph") == 0 || strcmp(playerchoice, "graph") == 0 || strcmp(playerchoice, "GRAPH") == 0)
        {
            printf("Plotting graph for inventory of items, wins and losses. \n");
            plot_graph(&player, filename, csvfile);
        }
        else if (strcmp(playerchoice, "9") == 0)
        {
            player.hp = 1000000;
            player.moxie += 1000; // This is a cheat that lets you activate the pleading state.
            printf("You cheated !\n");
        }
        else
        {
            printf("You entered an invalid input!!!\n");
        }
    }
    return 0;
}
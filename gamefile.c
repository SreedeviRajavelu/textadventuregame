#include <stdio.h>
#include <string.h>

// You can run the game with ./gamefile.exe <filehere> | <filehere> is only csv formatted file for the game. Try savegame.text.
void flush_input() // this code just 'flushes' the buffer to prevent overflow. Maybe unnecessary?
{
    int ch;
    while ((ch = getchar() != '\n' && ch != EOF)) // use ' ' for \n
        ;
}

typedef struct
{
    char class[50];
    int hp;
    struct
    {
        char itemName[50];
        int quantity;
    } items[10]; // Struct inside another struct. As of now, we only have 10 items. Player.items[0] = first item!
    char location[50];
    char spells[50];
    char check[50];
    int level;
    int wins;
    int loss;
} Player;

void save_game(Player *player, const char *filename, const char *csvfilename) /* currently need 3rd argument for csv file name can later be used as the same as textfile */
{
    FILE *file = fopen(filename, "w");
    FILE *csvfile = fopen(csvfilename, "w"); /* to write to csvfile*/

    // FILE *file = fopen(filename, "a");       // Open in append mode to preserve existing data
    // FILE *csvfile = fopen(csvfilename, "a"); // Open in append mode to preserve existing data

    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    if (csvfile == NULL)
    {
        perror("Error opening csvfile");
        return;
    }

    // Writing header to textfile
    fprintf(file, "Class,Item,Howmany,HP\n");

    // /*Writing header to csvfile*/
    fprintf(csvfile, "%s,%s, %s, %s,%s,%s, %s, %s,%s, %s\n",
            "Class", "Item", "Howmany", "HP", "Location", "Spells", "Check", "Level", "Wins", "Loss");

    // Writing player data to file
    for (int i = 0; i < 10; i++)
    {
        if (i == 0)
        {
            // Write class and HP only once, in the first line
            fprintf(file, "%s,%s,%d,%d, %s, %s, %s, %d, %d, %d\n", player->class, player->items[i].itemName, player->items[i].quantity, player->hp, player->location, player->spells, player->check, player->level, player->wins, player->loss);

            /* writing to csv file*/
            // fprintf(csvfile, "%s,%s, %s, %s\n",
            //         player->class, player->items[i].itemName, player->items[i].quantity, player->hp);

            // Write class and HP only once, in the first line
            fprintf(csvfile, "%s,%s,%d,%d\n", player->class, player->items[i].itemName, player->items[i].quantity, player->hp, player->location, player->spells, player->check, player->level, player->wins, player->loss);
        }
        else
        {
            // Write only item details in subsequent lines
            fprintf(file, "-,%s,%d,-\n", player->items[i].itemName, player->items[i].quantity);

            // Write only item details in subsequent lines
            fprintf(csvfile, "-,%s,%d,-\n", player->items[i].itemName, player->items[i].quantity);
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

    // Reading first line with class and HP
    fscanf(file, "%49[^,],%49[^,],%d,%d\n", player->class, player->items[0].itemName, &player->items[0].quantity, &player->hp); //%49. Max character to be saved is 49!

    // Reading remaining item details
    for (int i = 1; i < 10; i++)
    {
        fscanf(file, "%*[^,],%49[^,],%d,%*[^,\n]\n", player->items[i].itemName, &player->items[i].quantity); //%* means to skip this part. We don't want to read the first and last columns!
    }

    fclose(file);
    return 0;
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
    Player player;
    load_game(&player, filename);
    printf("Welcome!\n");
    printf("Your current HP is: %d\n", player.hp);

    while (keeplaying)
    {
        printf("What will you do now?\n");
        fgets(playerchoice, sizeof(playerchoice), stdin);
        playerchoice[strcspn(playerchoice, "\n")] = '\0'; // Remove trailing newline

        if (strcmp(playerchoice, "Quit") == 0 || strcmp(playerchoice, "quit") == 0)
        {
            keeplaying = 0;
            printf("You quit the game!\n");
        }
        else if (strcmp(playerchoice, "Save") == 0 || strcmp(playerchoice, "save") == 0)
        {
            save_game(&player, filename, csvfile);
        }
        else if (strcmp(playerchoice, "Battle") == 0 || strcmp(playerchoice, "battle") == 0)
        {
            player.hp += 10;
            printf("You battled some monsters and gained 10 more health!\n");
        }
        else // Assume it's an item name
        {
            int found = 0;
            for (int i = 0; i < 10; i++)
            {
                if (strcmp(playerchoice, player.items[i].itemName) == 0)
                {
                    player.items[i].quantity++;
                    printf("You obtained %s!\n", playerchoice);
                    found = 1;
                    break;
                }
            }
            if (!found)
            {
                printf("Item not found!\n");
            }
        }
    }
    return 0;
}

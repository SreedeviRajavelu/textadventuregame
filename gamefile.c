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
} Player;

void save_game(Player *player, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    // Writing header
    fprintf(file, "Class,Item,Howmany,HP\n");

    // Writing player data to file
    for (int i = 0; i < 10; i++)
    {
        if (i == 0)
        {
            // Write class and HP only once, in the first line
            fprintf(file, "%s,%s,%d,%d\n", player->class, player->items[i].itemName, player->items[i].quantity, player->hp);
        }
        else
        {
            // Write only item details in subsequent lines
            fprintf(file, "-,%s,%d,-\n", player->items[i].itemName, player->items[i].quantity);
        }
    }

    fclose(file);
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

    if (argc != 2)
    {
        printf("You did not enter the right number of parameters. Ending program.");
        return 1;
    }

    const char *filename = argv[1];
    char playerchoice[20];
    int keeplaying = 1;
    Player player;
    load_game(&player, filename);
    printf("Welcome!\n");
    printf("Your current HP is: %d \n", player.hp);

    while (keeplaying)
    {
        printf("What will you do now?\n");
        fgets(playerchoice, 256, stdin);
        // playerchoice[strcspn(playerchoice, "\n")] = 0;

        playerchoice[strlen(playerchoice) - 1] = 0; // we remove the newline
        if (strcmp(playerchoice, "Quit") == 0)      // we use strcmp here.
        {
            keeplaying = 0;
            printf("You quit the game!\n");
        }

        if (strcmp(playerchoice, "Save") == 0)
        {
            save_game(&player, filename);
        }

        if (strcmp(playerchoice, "Battle") == 0)
        {
            player.hp = player.hp + 10;
            printf("You battled some monsters and gained 10 more health!\n");
        }
    }
    return 0;
}

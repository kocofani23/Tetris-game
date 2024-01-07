#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define ROWS 50
#define COLS 50
#define MAX_SCORES_NUM 10

int game[ROWS][COLS];
double scores[MAX_SCORES_NUM] = {0.0};
char player_name[50];
int player_count = -1;

void menu(const int tetris_blocks[7][4][4]);
void play(const int tetris_blocks[7][4][4]);
void show_scores();
void put_blocks(const int tetris_blocks[7][4][4], int rows, int cols);
int is_game_complete(int cols);
int clear_filled_lines(int rows, int cols);
void show_game_field(int rows, int cols);
void create_game_field(int rows, int cols);
void sort_scores();
void print_green(char *text);
void print_red(char *text);

int main()
{

    const int tetris_blocks[7][4][4] = {
        //square block
        {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {1, 1, 0, 0},
            {1, 1, 0, 0},
        },

        // z block
        {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 1, 0},
        },

        //s block
        {
            {0, 0, 0, 0},
            {0, 1, 0, 0},
            {1, 1, 0, 0},
            {1, 0, 0, 0},
        },

        //line block
        {
            {1, 0, 0, 0},
            {1, 0, 0, 0},
            {1, 0, 0, 0},
            {1, 0, 0, 0},
        },

        //L block
        {
            {0, 0, 0, 0},
            {1, 0, 0, 0},
            {1, 0, 0, 0},
            {1, 1, 0, 0},
        },

        //reverse T block
        {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 1, 0, 0},
            {1, 1, 1, 0}
        }

    };

    menu(tetris_blocks);

    return 0;
}

// Function to print red text
void print_red(char *text)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    printf("%s", text);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

// Function to print green text
void print_green(char *text)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    printf("%s", text);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

//binary sort to sort scores from highest to lowest
void sort_scores()
{
    int i,j;
    double temp;
    for(i=0; i<MAX_SCORES_NUM-1; i++){
        for(j=i+1; j<MAX_SCORES_NUM; j++){
            if(scores[j] > scores[i]){
                temp = scores[j];
                scores[j] = scores[i];
                scores[i] = temp;
            }
        }
    }
}

//function to create game field
void create_game_field(int rows, int cols)
{
    int i,j;

    for(i=0; i<rows; i++){
        for(j=0; j<cols; j++){
            game[i][j] = 0;
        }
    }

}

//function to print game field
void show_game_field(int rows, int cols)
{
    int i, j;
    printf("\n******************* Game Field *******************\n");
    printf("  |");
    for(i=0; i<cols; i++){
        printf("%d|", i);
    }
    printf("\n");

    for(i=0; i<rows; i++){
        printf("%2d|", i);
        for(j=0; j<cols; j++){
                if(game[i][j] == 1){
                    print_red("1|");
                }
                else if(game[i][j] == 0){
                    print_green("0|");
                }
            }
        printf("\n");
    }
}

//function to clear filled lines
int clear_filled_lines(int rows, int cols)
{
    int lines_cleared = 0;
    int i,j,k;
    for(i=rows - 1; i>= 0; i--){
        int rowComplete = 1;
        for(j=0; j<cols; j++){
            if(!game[i][j]){
                rowComplete = 0;
                break;
            }
        }
        if(rowComplete){
           lines_cleared++;
            for(k=i; k>0; k--){
                for(j=0; j<cols; j++){
                    game[k][j] = game[k - 1][j];
                }
            }
            for(j=0; j<cols; j++){
                game[0][j] = 0;
            }
            i++; // Check the same row again after shifting rows down
        }
    }
    printf("******************* Cleared lines : %d *******************\n", lines_cleared);
    return lines_cleared;
}

//function to check whether game is complete or not
int is_game_complete(int cols)
{
    int i,j;
    for(j=0; j<cols; j++){
        if(game[0][j] == 1){
            return 1;
        }
    }
    return 0;
}

//function to put blocks in game field
void put_blocks(const int tetris_blocks[7][4][4], int rows, int cols)
{
    int i, j, random_index;
    int block_x, block_y;
    char decision;
    int temporary_block[4][4];
    int temp;

    srand(time(NULL));
    random_index = rand() % 6;
        printf("Block: \n");
        for(i=0; i<4; i++){
            for(j=0; j<4; j++){
                printf("%d", tetris_blocks[random_index][i][j]);
            }
            printf("\n");
        }

    printf("Do you want to rotate block or not (Y-yes, N-no): ");
    scanf(" %c", &decision);

    if(decision == 'y' || decision == 'Y'){

        for(i=0; i<4; i++){
            for(j=0; j<4; j++){
                temporary_block[i][j] = tetris_blocks[random_index][j][i];
            }
        }
        for(i=0; i<4; i++){
            for(j=0; j<4/2; j++){
            // Swap elements symmetrically across the row
                temp = temporary_block[i][j];
                temporary_block[i][j] = temporary_block[i][4 - 1 - j];
                temporary_block[i][4 - 1 - j] = temp;
            }
        }

        printf("Temporary Block: \n");
        for(i=0; i<4; i++){
            for(j=0; j<4; j++){
                printf("%d", temporary_block[i][j]);
            }
            printf("\n");
        }

        printf("Enter column to put block in game field: ");
        scanf("%d", &block_y);
        if(block_y < 0 || block_y > cols){
            printf("\nInvalid coordinates.\n");
            return;
        }

    block_x = 0;

    while(block_x + 4 < rows && game[block_x + 4][block_y] == 0){
        block_x++;
    }

    for(i=0; i<4; i++){
       for(j=0; j<4; j++){
            if(block_x + i < rows && block_y + j < cols && temporary_block[i][j] == 1){
                game[block_x + i][block_y + j] = temporary_block[i][j];
                }
            }
        }
    }

    else{
        printf("Enter column to put block in game field: ");
        scanf("%d", &block_y);
        if(block_y < 0 || block_y > cols){
            printf("\nInvalid coordinates.\n");
            return;
        }

    block_x = 0;

    while(block_x + 4 < rows && game[block_x + 4][block_y] == 0){
        block_x++;
    }

    for(i=0; i<4; i++){
       for(j=0; j<4; j++){
            if(block_x + i < rows && block_y + j < cols && tetris_blocks[random_index][i][j] == 1){
                game[block_x + i][block_y + j] = tetris_blocks[random_index][i][j];
              }
          }
       }
    }

}

//function to show scores of the games
void show_scores()
{
    int i=0;
    sort_scores();
    printf("\n******************* Scores *******************\n");
    for(i=0; i<MAX_SCORES_NUM; i++){
        printf("%2d. Score: %.2lf\n", i+1, scores[i]);
    }

}

//function to play game
void play(const int tetris_blocks[7][4][4])
{
    int rows, cols;
    int i,j;
    int random_index;
    char decision;
    double score = 0.0;
    int flag;

    printf("Enter name: ");
    scanf("%s", player_name);
    printf("Enter size of game field: ");
    scanf("%d%d", &rows, &cols);
    if(rows >= ROWS || cols >= COLS){
        printf("Invalid game sizes. Should be less than 50...");
        return;
    }

    create_game_field(rows, cols);
    player_count++;

    while(!(flag)){

        flag = is_game_complete(cols);
        show_game_field(rows, cols);

        put_blocks(tetris_blocks, rows, cols);

        score = (double)(10 * cols * clear_filled_lines(rows, cols));
        scores[player_count] += score;

        printf("Do you want to end the game or not?(Y-yes, N-no): ");
        scanf(" %c", &decision);

        if(decision == 'Y' || decision == 'y'){
            flag = 1;
        }

    }
    printf("Game Over...\nRedirecting to menu.\n");
    main();

}

//menu function in which game is started
void menu(const int tetris_blocks[7][4][4])
{
    int choice, rows, cols;
    printf("Welcome player to TETRIS\n");
    printf("1. Play Game\n2. View Scores\n3.Exit\n");
    printf("Choice: ");
    scanf("%d", &choice);
    switch(choice)
    {
    case 1:
        play(tetris_blocks);
        break;
    case 2:
        show_scores();
        break;
    case 3:
        printf("Exiting game...\n");
        exit(0);
        break;

    }
}

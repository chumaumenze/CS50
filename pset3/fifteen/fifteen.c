/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

//#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// cs50 replica
int get_int();

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
int move(int tile);
int won(void);
int swap(int *first, int *second);

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (1)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    int tiles = (d * d) - 1;
    // for rows
    for(int i = 0; i < d; i++)
    {
        // for columns
        for(int j = 0; j < d; j++)
        {
            board[i][j] = tiles;
            //printf("tile:%d; i:%d\n\n", tiles, i);
            tiles--;
            
            if(i == (d - 1) && j == (d - 1))
            {
                // using underscore for blank tile (95 == _ in ascii)
                board[d - 1][d - 1] = 95;
                // swapping last two tiles if dimension is even
                if((d % 2) == 0)
                {
                    swap(&board[i][j - 1], &board[i][j - 2]);
                    /*
                    board[i][j - 1] = board[i][j - 1] ^ board[i][j - 2];
                    board[i][j - 2] = board[i][j - 1] ^ board[i][j - 2];
                    board[i][j - 1] = board[i][j - 1] ^ board[i][j - 2];
                    */
                }
            }
        }
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            (board[i][j] == 95) ? printf("    %c", (char) board[i][j]) : printf("%5d", board[i][j]);
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
int move(int tile)
{
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            // find tile
            int match = (board[i][j] == tile) ? 1 : 0;
            if (match)
            {
                int legal = 0;
                //check west
                (board[i][j - 1] == 95) ? legal = swap(&board[i][j - 1], &board[i][j]) : 0 ;
                //check north
                (board[i - 1][j] == 95) ? legal = swap(&board[i - 1][j], &board[i][j]) : 0 ;
                //check east
                (board[i + 1][j] == 95) ? legal = swap(&board[i + 1][j], &board[i][j]) : 0 ;
                //check south
                (board[i][j + 1] == 95) ? legal = swap(&board[i][j + 1], &board[i][j]) : 0 ;
                
                // returns on legal moves
                if(legal == 1)
                    return 1;
            }
        }
    }
    return 0;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
int won(void)
{
    int success = 0;
    int tile = 1;
    int x = (d * d);
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            success += (board[i][j] == tile) ? 1 : 0;
            
            tile++;
	    
	    // printf("tile:%d\n", tile);
        }
    }
    success += (board[d - 1][d - 1] == 95) ? 1 : 0;
    
    // printf("success:%d, x:%d\n", success, x);
    if(success == x)
    {
        return 1;
    }
    return 0;
}

int swap(int *first, int *second)
{
    int temp = *second;
    *second = *first;
    *first = temp;
    return 1;
}

int get_int()
{
    int temp;
    scanf("%7d", &temp);
    return temp;
}

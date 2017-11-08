/*
 * 
 * name: init.c
 * @param 
 * @return
 * 
 */

#include <stdio.h>
#include <stdlib.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

int board[DIM_MAX][DIM_MAX];

int d;

// cs50 get_int()
int get_int();

int swap(int *first, int *second);

void init(void);
void draw(void);
void clear(void);
int move(int tile);
int won(void);

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
	printf("Usage: ./part d\n");
	return 0;
    }
    d = atoi(argv[1]);
    if(!d || d < DIM_MIN || d > DIM_MAX)
    {
	printf("Enter valid number between 3 and 9\n");
	return 0;
    }
    /*
    do{
	printf("Board dimension: ");
	d = get_int();
    }
    while(d < DIM_MIN || d > DIM_MAX);
    */
    
    init();

    while(1)
    {
	//clear();
	
	draw();
	
	if(won())
	{
	    printf("You win!!!\n");
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
	
	// move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n\n");
        }
    }
    
    return 0;
}




/* for init() function */
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

/* for draw() function */
void draw(void)
{
    printf("\n");
    for(int i = 0; i < d; i++)
    {
	for(int j = 0; j < d; j++)
	{
	    (board[i][j] == 95) ? printf("    %c", (char) board[i][j]) : printf("%5d", board[i][j]);
	}
	printf("\n\n");
    }
}

int move (int tile)
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

int won(void)
{
    // TODO
    int success = 0;
    int tile = 1;
    int x = (d * d);
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            success += (board[i][j] == tile) ? 1 : 0;
            
            tile++;
	    
	    printf("tile:%d\n", tile);
        }
    }
    success += (board[d - 1][d - 1] == 95) ? 1 : 0;
    
    printf("success:%d, x:%d\n", success, x);
    if(success == x)
    {
        return 1;
    }
    return 0;
}

void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

int get_int()
{
    int temp = 0;
    scanf("%7d", &temp);
    return temp;
}

int swap(int *first, int *second)
{
    int temp = *second;
    *second = *first;
    *first = temp;
    return 1;
}


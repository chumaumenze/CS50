#include <stdio.h>
//#include <cs50.h>

void print_space(int n);
void print_hash(int n);

int main()
{
    int height, row, space, hash;
    do
    {
        printf ("Height: ");
        scanf("%10d", &height);
        // height = get_int();
    }
    while (height > 23 || height < 0); //filters user inputs; get_int rejects numbers less than zero

    int real_height = height + 1;

    for (row = 1; row < real_height; row++)
    {
        print_space(real_height - row);
        print_hash(row);
        printf("\n");
    }
}


void print_space(int n)
{
    for (int space = 0; space < n; space++)
    {
        printf(" ");
    }
}

void print_hash(int n)
{
    for (int hash = 0; hash < n; hash++)
    {
        printf ("#");
    }
}
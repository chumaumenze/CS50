#include <stdio.h>
#include <cs50.h>

int main ()
    {
        int n;
        do
        {
            printf("Minutes: ");
            n = get_int();
        }
        while (n < 0);
        
        printf("Bottles: %i\n", n*12);
    }
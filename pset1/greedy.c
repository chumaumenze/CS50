#include <stdio.h>
#include <cs50.h>
#include <math.h>

#define K25 25
#define K10 10
#define K5 5

int main(void)
{
    float balance;
    do
    {
        printf("Gimme a float: ");
        balance = get_float();
    }
    while(balance <= 0);
    
    // convert balance to kobo
    int kobo = (int)round(balance * 100);
    
    // count coins
    int count = 0;
    
    // store the remainders
    int remains = 0;
    
    // for 25 kobo
    count += kobo / K25;
    remains = kobo % K25;
    
    // for 10 kobo
    count += remains / K10;
    remains = remains % K10;
    
    // for 5 kobo
    count += remains / K5;
    remains = remains % K5;
    
    // for 1 kobo
    count += remains;
    
    
    
    printf("%d\n", count);
}
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <cs50.h>

int main ()
{
    
    //printf("Gimme a name: ");
    string s = get_string();
    
    if ((s != NULL)) {
        //checks if first character is an alphabet. prints if true. 
        if(isalpha(s[0]))
            printf("%c", toupper(s[0]));
        
        //Iterating input    
        for (int i = 0, len = (int) strlen(s); i < len; i++)
        {
            //checks if current character is space or punctuations and if next character is an alphabet, print next alphabet.
            if (((isspace(s[i])) || (ispunct(s[i]))) && (isalpha(s[i+1])))
            {
                    printf("%c", toupper(s[i+1]));
            }
        }
        printf("\n");
    }
}
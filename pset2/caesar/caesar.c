#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <cs50.h>

#define ALPHA 65
#define ZETA 91


int main (int argc, char **argv)
{
    //checks argument count
    if ((argc != 2))
    {
        printf("Usage: \n\t./caesar [key]\n");
        return 1;
    }
    else
    {
        
        int key_len = (int)strlen(argv[1]);
        
        for (int i = 0; i < key_len; i++)
        {
            //checking for non-numerical character to print usage
            if (!isdigit(argv[1][i]))
            {
                printf("Usage: \n\t./caesar [key]\n");
                return 1;
            }
        }
        
        
        printf("plaintext: ");
        string text = get_string();
        char cipher[100] = "";
        
        int text_len = (int)strlen(text);
        int key = atoi(argv[1]);
        
        //key_guage to control wrapping around of characters in keyword.
        for (int i = 0, key_guage = 0; i != text_len; i++, key_guage++)
        {
            //checking if the current character is an alphabet
            if ((isalpha(text[i])))
            {
                //Adds keysize with the current character of input
                int temp = ((int)key + (int)(toupper(text[i])));
                
                //To ensure the characters wrap around, remove ZETA (i.e. 91) and readds ALPHA (i.e. 'A' or 65 in ASCII)
                while (temp > 90)
                    temp = (temp - ZETA) + ALPHA;
                
                //If the current character is upper/lower case, restores casing.    
                if (islower(text[i]))
                {
                    cipher[i] = (char)tolower(temp);
                }
                else
                {
                    cipher[i] = (char)toupper(temp);
                }
            }
            else
            {
                //stores the character is they are not alphabet
                cipher[i] = (char)text[i];
                key_guage--;
            }
        }
        printf("ciphertext: %s\n", cipher);
    }
}

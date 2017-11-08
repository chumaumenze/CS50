#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define ALPHA 65
#define ZETA 91


int main (int argc, char **argv)
{
    char text[100] = "";
    char cipher[100] = "";
    
    
    if ((argc != 2))
    {
        printf("Usage: \n\t./vigenere.c [keyword]\n");
        return 1;
    }
    else
    {
        
        int y = (int)strlen(argv[1]);
        int yx = 0;
        
        for (int i = 0; i < y; i++)
        {
            if (!isalpha(argv[1][i]))
            {
                yx += 1;
                printf("Usage: \n\t./vigenere.c [keyword]\n");
                return 1;
            }
        }
        
        
        printf("Enter text: ");
        scanf("%[a-z A-Z,.;\'\":]", text);
        
        int x = (int)strlen(text);
        
        
        for (int i = 0, j = 0; i != x; i++, j++)
        {
            
            if (isalpha(text[i]))
            {
                int aa = ((int)((toupper(argv[1][j % y])) - ALPHA) + (int)(toupper(text[i])));
                if (aa > 90)
                   aa = (aa - ZETA) + ALPHA;
                if (islower(text[i]))
                    cipher[i] = (char)tolower(aa);
                else
                    cipher[i] = (char)toupper(aa);
            }
            else
            {
                cipher[i] = (char)text[i];
                j--;
            }
        }
        printf("Cipher: %s\n", cipher);
    }
}
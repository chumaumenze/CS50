#define _XOPEN_SOURCE
#include <unistd.h>

#include <stdio.h>
#include <ctype.h>
#include <string.h>

int crack(char *word, char *hash);

int main (int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./crack [hash]\n");
        return 1;
    }
    if (strlen(argv[1]) != 13)
    {
        printf("Hash incomplete: Usage: ./crack [hash]\n");
        return 1;
    }
    
    char hash[14];
    strcpy(hash, argv[1]);
    
    // using wordlist
    FILE *fp = fopen("./wordlist.txt", "r");
    if (fp != NULL)
    {
        int success = 0;
        while (success == 0)
        {
            char word[5];
            fscanf(fp, "%s", word);
            success = crack(word, hash);
            if(success == 1)
            {
                printf("%s\n", word);
            }
        }
    }
    fclose(fp);
}

int crack(char *word, char *hash)
{
    // Extracting salt from hash
    char salt[3];
    strncpy(salt, hash, 2);
    char *new_hash = (char *)crypt(word, salt);
    if(strcmp(new_hash, hash) == 0)
    {
        //printf("password match\n");
        return 1;
    }
    else
    {
        //printf("%s is not the password\n", word);
        return 0;
    }
}

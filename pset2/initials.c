#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main ()
{
		 //string
    char s[100];
    
		//request user input
    printf("Gimme a name: ");
    scanf("%90[a-z',.!\"/ \\ A-Z]", s);
    
		//check if input is correct
    if (!s) {
				  //print first alphabet
        if(isalpha(s[0]))
            printf("%c", toupper(s[0]));
				  //traverse string
        for (int i = 0, len = (int) strlen(s); i < len; i++)
        {
						   //if i is non-alpha and i+1 alpha
            if (((isspace(s[i])) || (ispunct(s[i]))) && (isalpha(s[i+1])))
            {
								        //print i+1 character
                    printf("%c", toupper(s[i+1]));
            }
        }
    }
}
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <stdlib.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

// default dictionary
#define DICTIONARY dictionaries/dict

// maximum capacity of hash table (26 alphabet * 2)
#define TABLE_MAX 52

typedef struct link
{
	struct link *prev;
	char *word;
	struct link *next;
}
node;

// Hash function
unsigned int hash(const char *word, unsigned int table_size);
// checks for word in dictionary
bool check(const char *word);
// Creates a link
node *create(const char *value);
// Inserts link to linked list
node *insert(node *link, const node *listhead, bool sort);
// Unloads dictionaries and frees memory
bool unload(void);
// removes linked list
bool trash(node *list, bool *single, void *word);

unsigned int size(void);

int delete(node *list);

// hash table (array of pointer to linked lists); initialized to NULL
node *table[TABLE_MAX] = {(void *) 0};


int main(int argc, char *argv[])
{
	char *dictionary;
	char *essay;
	if (argc == 3)
	{
		dictionary = argv[1];
		essay = argv[2];
	}
	else
	{
		fprintf(stderr, "Usage: ./temp dictionary text\n");
		return 404;
	}

	// Open and load dictionary
	FILE *fdict = fopen(dictionary, "r");
	if(fdict == NULL)
	{
		fprintf(stderr, "Unable to open dictionary: %s\n", dictionary);
		return false;
	}
	// Open essay
	FILE *ftext = fopen(essay, "r");
	if(ftext == NULL)
	{
		fprintf(stderr, "Unable to open dictionary: %s\n", essay);
		return false;
	}

	// temporarily store words from dictionary
	char word[LENGTH + 1] = "";

	(table[0] == NULL) ? printf("NULL\n") : printf("NOT NULL\n") ;

	//store dictionary to linked list
	while(fscanf(fdict, "%s", word) != EOF)
	{
		// list in table to store the word
		int list = hash(word, TABLE_MAX);

		if (list == 501)
		{
			unload();
			fprintf(stderr, "%s:%d: Incorrect hash, Dictionary contains invalid word\n", __FILE__, __LINE__);
			return false;
		}
		else
		{
			printf("%s:%d: hashing %s to %d\n",  __FILE__, __LINE__, word, list);
		}

		// create a node
		node *link = create(word);
		if (link == NULL)
		{
			unload();
			fprintf(stderr, "%s:%d: Unable to allocate memory\n", __FILE__, __LINE__);
			return false;
		}

		// Insert link into linked list return the new list head
		table[list] = insert(link, table[list], false);
	}

	fclose(fdict);

	int misspellings = 0, index = 0;
	int words = 0;
	for (int c = fgetc(ftext); c != EOF; c = fgetc(ftext))
    {
        // allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // append character to word
            word[index] = c;
            index++;

            // ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // consume remainder of alphabetical string
                while ((c = fgetc(ftext)) != EOF && isalpha(c));

                // prepare for new word
                index = 0;
            }
        }

        // ignore words with numbers (like MS Word can)
        else if (isdigit(c))
        {
            // consume remainder of alphanumeric string
            while ((c = fgetc(ftext)) != EOF && isalnum(c));

            // prepare for new word
            index = 0;
        }

        // we must have found a whole word
        else if (index > 0)
        {
            // terminate current word
            word[index] = '\0';

            // update counter
            words++;

            // check word's spelling
            bool misspelled = !check(word);

            // print word if misspelled
            if (misspelled)
            {
                // printf("%s\n", word);
                misspellings++;
            }

            // prepare for next word
            index = 0;
        }
    }

    fclose(ftext);

    bool stats = unload();
    
    printf("words:%d, misspelled:%d, unloaded:%s\n", words, misspellings, (stats) ? "true" : "false");
    
    // printf("words:%d, misspelled:%d\n", words, misspellings);
}

bool unload(void)
{
	int erased = 0;
    for (int i = 0; i <= TABLE_MAX; ++i)
    {
    	node *cursor = table[i];
    	printf("unloading table[%d]\n", i);
    	while(cursor != NULL)
    	{
    		node *link = cursor;
    		cursor = cursor->next;
    		free(link->word);
    		free(link);
    		erased++;
    	}
    }
    printf("unload():%d: loaded:%d, unloaded %d links\n", __LINE__, size(), erased);
    return true;



	/*int erased = 0;
    for (int i = 0; i <= TABLE_MAX-1; ++i)
    {
    	printf("deleting table[%d]\n", i);
    	erased += delete(table[i]);
    }

    printf("unload():%d: loaded:%d, unloaded %d links\n", __LINE__, size(), erased);
    return true;*/
}


int delete(node *list)
{
	int erased = 0;
	if (list == NULL)
	{
		// free(list);
		return 0;
	}
	else
	{
		// delete other list
		delete(list->next);
		// delete current list
		free(list);
		erased++;
	}
	return erased;
}

bool trash(node *list, bool *single, void *word)
{
	return false;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
	int list = hash(word, TABLE_MAX);
	if (list == 501)
	{
		unload();
		fprintf(stderr, "%s:%d: Incorrect hash, Essay contains invalid word\n", __FILE__, __LINE__);
		return false;
	}
	node *cursor = table[list];
	if (cursor == NULL || table[list] == NULL)
	{
		fprintf(stderr, "Word is not in dictionary\n");
	}
	while(cursor != NULL)
	{
		if (strcasecmp(cursor->word, word) == 0)
		{
			printf("check():%d: at table[%d]: %s is in the dictionary\n", __LINE__, list, word);
			return true;
		}

		cursor = cursor->next;
	}
	printf("check():%d: at table[%d]: %s is not in the dictionary\n", __LINE__, list, word);
    return false;
}

unsigned int hash(const char *word, unsigned int table_size)
{
	// printf("hashing word\n");
	int word_length = strlen(word);
	if ( word_length > 0 && word_length <= LENGTH && isalpha(word[0]) && word != NULL)
	{
		int index = (toupper(word[0]) % 'A');

		return (word_length > 1 && isalpha(word[1]) && (toupper(word[1]) % 'A') >= 12 ) ? (index + (table_size / 2)) % table_size : index % (table_size / 2);
	}

	return 52;
}

node *create(const char *value)
{
	printf("creating link\n");
	char *temp = (char *) malloc(strlen(value) + 1);
	if (temp == NULL)
	{
		return NULL;
	}
	strcpy(temp, value);
	node *new_link = (node *) malloc(sizeof(node));
	if (new_link == NULL)
	{
		return NULL;
	}

	new_link->prev = NULL;
	new_link->word = temp;
	new_link->next = NULL;
	// printf("%s:%d: new_link->word is %s\n", __FILE__, __LINE__, new_link->word);

	return new_link;
}

/**
 * Inserts link in table. Sort defines if the insertion should be done in a
 * sorted manner or at the beginning. Returns pointer to new list head
 */
node *insert(node *link, const node *listhead, bool sort)
{
	printf("insert():%d: starting insertion\n", __LINE__);

	if (link == NULL/* || head == NULL*/)
	{
		fprintf(stderr, "insert(): link parameter probably points to NULL\n");
		return NULL;
	}

	node *head = (node *) listhead;

	(head == NULL) ? printf("insert():%d: head is NULL\n", __LINE__) : printf("insert():%d: head is NOT NULL\n", __LINE__) ;

	printf("insert():%d: inserting link..\n", __LINE__);
	if ( sort == false)
	{
		if (head == NULL)
		{
			// point link's next to where list's head points
			link->next = head;

			// point link's prev to where list's head points
			link->prev = head;
		}
		else
		{
			// point link's next to where list's head points
			link->next = head;

			// point link's prev to where list's head points
			link->prev = head->prev;

			// existing link's prev points to new link
			head->prev = link;
		}

		// point the list head to the new link
		head = link;

		printf("insert():%d: finish insertion in unsort mode\n", __LINE__);

		return head;
	}
	else
	{
		// traverse linked list and insert link at proper location
		printf("insert():%d: finished insertion in sort mode\n", __LINE__);
		return NULL;
	}	
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
	int words = 0;
    for (int i = 0; i <= TABLE_MAX; ++i)
    {
    	node *cursor = table[i];
    	while(cursor != NULL)
    	{
    		words++;
    		cursor = cursor->next;
    	}
    }
    return words;
}
/**
 * Implements a dictionary's functionality.
 */
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
// #include <strings.h>
#include <stdlib.h>

#include "dictionary.h"

// hash table (array of pointer to linked lists); initialized to NULL
node *table[TABLE_MAX] = {(void *) 0};

// temporarily store words from dictionary
char word[LENGTH + 1] = "";

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
		return false;
	}
	while(cursor != NULL)
	{
		if (strcasecmp(cursor->word, word) == 0)
		{
			// printf("check():%d: at table[%d]: %s is in the dictionary\n", __LINE__, list, word);
			return true;
		}

		cursor = cursor->next;
	}
	// printf("check():%d: at table[%d]: %s is not in the dictionary\n", __LINE__, list, word);
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
	FILE *fdict = fopen(dictionary, "r");
	if(fdict == NULL)
	{
		fprintf(stderr, "Unable to open dictionary: %s\n", dictionary);
		return false;
	}

	//store word to linked list
	while(fscanf(fdict, "%s", word) != EOF)
	{
		// list in table to store the word
		int list = hash(word, TABLE_MAX);

		if (list == 501)
		{
			unload();
			fprintf(stderr, "%s:%d: Incorrect hash\n", __FILE__, __LINE__);
			return false;
		}

		// printf("%s:%d: hashing %s to %d\n",  __FILE__, __LINE__, word, list);

		// create a node
		node *link = create(word);
		if (link == NULL)
		{
			unload();
			fprintf(stderr, "%s:%d: Unable to allocate memory\n", __FILE__, __LINE__);
			return false;
		}

		table[list] = insert(link, table[list], false);

		// printf("%s:%d: hash for %s is %d\n", __FILE__, __LINE__, link->word, list);
	}
    
    fclose(fdict);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
	int words = 0;
    for (int i = 0; i <= TABLE_MAX - 1; ++i)
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

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    int erased = 0;
    for (int i = 0; i <= TABLE_MAX - 1; ++i)
    {
    	node *cursor = table[i];
    	while(cursor != NULL)
    	{
    		node *link = cursor; // store current link
    		cursor = cursor->next; // shift cursor to next link
    		free(link->word); // unload word from current link
    		free(link); // unload link from list
    		erased++;
    	}
    }
    printf("unload():%d: loaded:%d, unloaded %d links\n", __LINE__, size(), erased);
    return true;
}


/***************custom functions**********************/

unsigned int hash(const char *word, unsigned int table_size)
{
	int word_length = strlen(word);

	// check the length of the word and character
	if ( word_length > 0 && word_length <= LENGTH && isalpha(word[0]) && word != NULL)
	{
		// hash the first character (where [A-Z] == [0-25])
		int index = (toupper(word[0]) % 'A');

		// the second letter is hashes to number greater than/equal twelve,
		// return hash of first letter + half the table (i.e. second half of
		// the table) ; else return the hash of first letter (i.e. first
		// half of the table)
		return (word_length > 1 && isalpha(word[1]) && (toupper(word[1]) % 'A') >= 12 ) ? (index + (table_size / 2)) % table_size : index % (table_size / 2);
	}

	return 501;
}

node *create(const char *value)
{
	char *temp = (char *) malloc(strlen(value) + 1);
	if (temp == NULL)
	{
		return NULL;
	}
	strcpy(temp, value);
	node *item = (node *) malloc(sizeof(node));
	if (item == NULL)
	{
		return NULL;
	}

	item->prev = NULL;
	item->word = temp;
	item->next = NULL;
	// printf("%s:%d: item->word is %s\n", __FILE__, __LINE__, item->word);

	return item;
}

/**
 * Inserts link in table. Sort defines if the insertion should be done in a
 * sorted manner or at the beginning. Returns pointer to new list head
 */
node *insert(node *link, const node *listhead, bool sort)
{
	// printf("insert():%d: starting insertion\n", __LINE__);

	if (link == NULL/* || head == NULL*/)
	{
		fprintf(stderr, "insert(): link parameter probably points to NULL\n");
		return NULL;
	}

	node *head = (node *) listhead;

	// (head == NULL) ? printf("insert():%d: head is NULL\n", __LINE__) : printf("insert():%d: head is NOT NULL\n", __LINE__) ;

	// printf("insert():%d: inserting link..\n", __LINE__);
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

		// printf("insert():%d: finish insertion in unsort mode\n", __LINE__);

		return head;
	}
	else
	{
		// TODO: traverse linked list and insert link at proper location
		printf("insert():%d: finished insertion in sort mode\n", __LINE__);
		return NULL;
	}	
}
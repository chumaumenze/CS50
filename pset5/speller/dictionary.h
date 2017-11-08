/**
 * Declares a dictionary's functionality.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

// maximum capacity of hash table (26 alphabet * 2)
#define TABLE_MAX 52

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word);

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);

#endif // DICTIONARY_H

typedef struct link
{
	struct link *prev;
	char *word;
	struct link *next;
}
node;

/**
 *Hashes a word. Returns the index (a number) for the hash table
 */
unsigned int hash(const char *word, unsigned int table_size);

/**
 * Creates a node or link for a doubly-linked list. On success, returns a node pointer,
 * else NULL
 */
node *create(const char *word);

/**
 * Inserts link in table. Sort defines if the insertion should be done in a
 * sorted manner or at the beginning. Returns pointer to new list head
 */
node *insert(node *link, const node *listhead, bool sort);
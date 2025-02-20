// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 101;

// Hash table
node *table[N];

// --- size count
int sizeCount = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // printf("SPRAWDZAM: %s\n", word);
    int wordHash = hash(word);

    node *pointer = table[wordHash];

    while (pointer != NULL)
    {
        if (strcasecmp(pointer->word, word) == 0)
        {
            return true;
        }
        pointer = pointer->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int ASCIIsum = 0;
    for (int i = 0; i < 3 && word[i]; i++)
    {
        ASCIIsum = ASCIIsum + toupper(word[i]);
    }

    return ASCIIsum % 100;
    // return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        fclose(file);
        return false;
    }

    char buffer[LENGTH + 1];

    while (fscanf(file, "%s", buffer) != EOF)
    {
        node *newNode = malloc(sizeof(node));

        strcpy(newNode->word, buffer);

        int hashedValue = hash(buffer);

        newNode->next = table[hashedValue];
        table[hashedValue] = newNode;

        sizeCount++;
        // printf("SŁOWO ZE SŁOWNIKA: %s\n", buffer);
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return sizeCount;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {

        node *pointer = table[i];
        while (pointer != NULL)
        {
            node *previous = pointer;
            pointer = pointer->next;
            free(previous);
        }
        table[i] = NULL;
    }
    return true;
}

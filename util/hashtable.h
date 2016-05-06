/* ========================================================================== */
/* File: hashtable.h
 *
 * Component name: Crawler
 *
 * Refactored hashtable to work with any kind of node
 *
 */
/* ========================================================================== */
#ifndef HASHTABLE_H
#define HASHTABLE_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include "common.h"                          // common functionality
#include <string.h>
#include <stdio.h>   

// ---------------- Constants
#define MAX_HASH_SLOT 10000                // number of "buckets" 10000 

// ---------------- Structures/Types

/* For working with generics */
typedef void* element_t;

typedef struct HashTableNode {
    element_t data;                               // object hashed
    struct HashTableNode *next;              // pointer to next node
} HashTableNode;

typedef struct HashTable {
    HashTableNode *table[MAX_HASH_SLOT];     // actual hashtable
    unsigned int size;
    unsigned int n;
} HashTable;



// ---------------- Public Variables
 //extern HashTable ht;

// ---------------- Prototypes/Macros

/* URL Hashtable wrappers */
int HashURL(const element_t strv, int mod);
int HashTableAddURL(HashTable* ht, char* url);
int HashTableLookUpURL(HashTable* ht, char* url);
void cleanHashURL(HashTable* ht);




/* Wordnode Hashtable wrappers */


/* Hashtable macros */
HashTable* initHashTable();
int HashTableAdd(HashTable* ht, element_t key, int (*f)(element_t, int));
int HashTableLookUp(HashTable* ht, element_t key, int (*f)(element_t, int), int (*g)(element_t, element_t));
void cleanHash();


/* Hashtable helpers */
int cmpStrings(element_t av, element_t bv);


#endif // HASHTABLE_H